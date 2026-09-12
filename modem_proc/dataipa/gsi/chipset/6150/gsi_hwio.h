#ifndef __GSI_HWIO_H__
#define __GSI_HWIO_H__
/*
===========================================================================
*/
/**
  @file gsi_hwio.h
  @brief Auto-generated HWIO interface include file.

  This file contains HWIO register definitions for the following modules:
    .*

  'Include' filters applied: <none>
  'Exclude' filters applied: RESERVED DUMMY 

  Attribute definitions for the HWIO_*_ATTR macros are as follows:
    0x0: Command register
    0x1: Read-Only
    0x2: Write-Only
    0x3: Read/Write
*/
/*
  ===========================================================================

  Copyright (c) 2016 Qualcomm Technologies Incorporated.
  All Rights Reserved.
  QUALCOMM Proprietary and Confidential.

  ===========================================================================

  $Header: //components/rel/dataipa.mpss/3.2/gsi/chipset/6150/gsi_hwio.h#1 $
  $DateTime: 2019/07/05 03:48:36 $
  $Author: pwbldsvc $

  ===========================================================================
*/

/*----------------------------------------------------------------------------
 * MODULE: GSI
 *--------------------------------------------------------------------------*/

#define GSI_REG_BASE                                                               (GSI_TOP_BASE      + 0x00004000)
#define GSI_REG_BASE_PHYS                                                          (GSI_TOP_BASE_PHYS + 0x00004000)
#define GSI_REG_BASE_OFFS                                                          0x00004000

#define HWIO_GSI_CFG_ADDR                                                          (GSI_REG_BASE      + 0x00000000)
#define HWIO_GSI_CFG_PHYS                                                          (GSI_REG_BASE_PHYS + 0x00000000)
#define HWIO_GSI_CFG_OFFS                                                          (GSI_REG_BASE_OFFS + 0x00000000)
#define HWIO_GSI_CFG_RMSK                                                                0x3f
#define HWIO_GSI_CFG_ATTR                                                                 0x3
#define HWIO_GSI_CFG_IN          \
        in_dword_masked(HWIO_GSI_CFG_ADDR, HWIO_GSI_CFG_RMSK)
#define HWIO_GSI_CFG_INM(m)      \
        in_dword_masked(HWIO_GSI_CFG_ADDR, m)
#define HWIO_GSI_CFG_OUT(v)      \
        out_dword(HWIO_GSI_CFG_ADDR,v)
#define HWIO_GSI_CFG_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GSI_CFG_ADDR,m,v,HWIO_GSI_CFG_IN)
#define HWIO_GSI_CFG_BP_MTRIX_DISABLE_BMSK                                               0x20
#define HWIO_GSI_CFG_BP_MTRIX_DISABLE_SHFT                                                0x5
#define HWIO_GSI_CFG_GSI_PWR_CLPS_BMSK                                                   0x10
#define HWIO_GSI_CFG_GSI_PWR_CLPS_SHFT                                                    0x4
#define HWIO_GSI_CFG_UC_IS_MCS_BMSK                                                       0x8
#define HWIO_GSI_CFG_UC_IS_MCS_SHFT                                                       0x3
#define HWIO_GSI_CFG_DOUBLE_MCS_CLK_FREQ_BMSK                                             0x4
#define HWIO_GSI_CFG_DOUBLE_MCS_CLK_FREQ_SHFT                                             0x2
#define HWIO_GSI_CFG_MCS_ENABLE_BMSK                                                      0x2
#define HWIO_GSI_CFG_MCS_ENABLE_SHFT                                                      0x1
#define HWIO_GSI_CFG_GSI_ENABLE_BMSK                                                      0x1
#define HWIO_GSI_CFG_GSI_ENABLE_SHFT                                                      0x0

#define HWIO_GSI_MANAGER_MCS_CODE_VER_ADDR                                         (GSI_REG_BASE      + 0x00000008)
#define HWIO_GSI_MANAGER_MCS_CODE_VER_PHYS                                         (GSI_REG_BASE_PHYS + 0x00000008)
#define HWIO_GSI_MANAGER_MCS_CODE_VER_OFFS                                         (GSI_REG_BASE_OFFS + 0x00000008)
#define HWIO_GSI_MANAGER_MCS_CODE_VER_RMSK                                         0xffffffff
#define HWIO_GSI_MANAGER_MCS_CODE_VER_ATTR                                                0x3
#define HWIO_GSI_MANAGER_MCS_CODE_VER_IN          \
        in_dword_masked(HWIO_GSI_MANAGER_MCS_CODE_VER_ADDR, HWIO_GSI_MANAGER_MCS_CODE_VER_RMSK)
#define HWIO_GSI_MANAGER_MCS_CODE_VER_INM(m)      \
        in_dword_masked(HWIO_GSI_MANAGER_MCS_CODE_VER_ADDR, m)
#define HWIO_GSI_MANAGER_MCS_CODE_VER_OUT(v)      \
        out_dword(HWIO_GSI_MANAGER_MCS_CODE_VER_ADDR,v)
#define HWIO_GSI_MANAGER_MCS_CODE_VER_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GSI_MANAGER_MCS_CODE_VER_ADDR,m,v,HWIO_GSI_MANAGER_MCS_CODE_VER_IN)
#define HWIO_GSI_MANAGER_MCS_CODE_VER_VER_BMSK                                     0xffffffff
#define HWIO_GSI_MANAGER_MCS_CODE_VER_VER_SHFT                                            0x0

#define HWIO_GSI_ZEROS_ADDR                                                        (GSI_REG_BASE      + 0x00000010)
#define HWIO_GSI_ZEROS_PHYS                                                        (GSI_REG_BASE_PHYS + 0x00000010)
#define HWIO_GSI_ZEROS_OFFS                                                        (GSI_REG_BASE_OFFS + 0x00000010)
#define HWIO_GSI_ZEROS_RMSK                                                        0xffffffff
#define HWIO_GSI_ZEROS_ATTR                                                               0x1
#define HWIO_GSI_ZEROS_IN          \
        in_dword_masked(HWIO_GSI_ZEROS_ADDR, HWIO_GSI_ZEROS_RMSK)
#define HWIO_GSI_ZEROS_INM(m)      \
        in_dword_masked(HWIO_GSI_ZEROS_ADDR, m)
#define HWIO_GSI_ZEROS_ZEROS_BMSK                                                  0xffffffff
#define HWIO_GSI_ZEROS_ZEROS_SHFT                                                         0x0

#define HWIO_GSI_PERIPH_BASE_ADDR_LSB_ADDR                                         (GSI_REG_BASE      + 0x00000018)
#define HWIO_GSI_PERIPH_BASE_ADDR_LSB_PHYS                                         (GSI_REG_BASE_PHYS + 0x00000018)
#define HWIO_GSI_PERIPH_BASE_ADDR_LSB_OFFS                                         (GSI_REG_BASE_OFFS + 0x00000018)
#define HWIO_GSI_PERIPH_BASE_ADDR_LSB_RMSK                                         0xffffffff
#define HWIO_GSI_PERIPH_BASE_ADDR_LSB_ATTR                                                0x3
#define HWIO_GSI_PERIPH_BASE_ADDR_LSB_IN          \
        in_dword_masked(HWIO_GSI_PERIPH_BASE_ADDR_LSB_ADDR, HWIO_GSI_PERIPH_BASE_ADDR_LSB_RMSK)
#define HWIO_GSI_PERIPH_BASE_ADDR_LSB_INM(m)      \
        in_dword_masked(HWIO_GSI_PERIPH_BASE_ADDR_LSB_ADDR, m)
#define HWIO_GSI_PERIPH_BASE_ADDR_LSB_OUT(v)      \
        out_dword(HWIO_GSI_PERIPH_BASE_ADDR_LSB_ADDR,v)
#define HWIO_GSI_PERIPH_BASE_ADDR_LSB_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GSI_PERIPH_BASE_ADDR_LSB_ADDR,m,v,HWIO_GSI_PERIPH_BASE_ADDR_LSB_IN)
#define HWIO_GSI_PERIPH_BASE_ADDR_LSB_BASE_ADDR_BMSK                               0xffffffff
#define HWIO_GSI_PERIPH_BASE_ADDR_LSB_BASE_ADDR_SHFT                                      0x0

#define HWIO_GSI_PERIPH_BASE_ADDR_MSB_ADDR                                         (GSI_REG_BASE      + 0x0000001c)
#define HWIO_GSI_PERIPH_BASE_ADDR_MSB_PHYS                                         (GSI_REG_BASE_PHYS + 0x0000001c)
#define HWIO_GSI_PERIPH_BASE_ADDR_MSB_OFFS                                         (GSI_REG_BASE_OFFS + 0x0000001c)
#define HWIO_GSI_PERIPH_BASE_ADDR_MSB_RMSK                                         0xffffffff
#define HWIO_GSI_PERIPH_BASE_ADDR_MSB_ATTR                                                0x3
#define HWIO_GSI_PERIPH_BASE_ADDR_MSB_IN          \
        in_dword_masked(HWIO_GSI_PERIPH_BASE_ADDR_MSB_ADDR, HWIO_GSI_PERIPH_BASE_ADDR_MSB_RMSK)
#define HWIO_GSI_PERIPH_BASE_ADDR_MSB_INM(m)      \
        in_dword_masked(HWIO_GSI_PERIPH_BASE_ADDR_MSB_ADDR, m)
#define HWIO_GSI_PERIPH_BASE_ADDR_MSB_OUT(v)      \
        out_dword(HWIO_GSI_PERIPH_BASE_ADDR_MSB_ADDR,v)
#define HWIO_GSI_PERIPH_BASE_ADDR_MSB_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GSI_PERIPH_BASE_ADDR_MSB_ADDR,m,v,HWIO_GSI_PERIPH_BASE_ADDR_MSB_IN)
#define HWIO_GSI_PERIPH_BASE_ADDR_MSB_BASE_ADDR_BMSK                               0xffffffff
#define HWIO_GSI_PERIPH_BASE_ADDR_MSB_BASE_ADDR_SHFT                                      0x0

#define HWIO_GSI_PERIPH_PENDING_ADDR                                               (GSI_REG_BASE      + 0x00000020)
#define HWIO_GSI_PERIPH_PENDING_PHYS                                               (GSI_REG_BASE_PHYS + 0x00000020)
#define HWIO_GSI_PERIPH_PENDING_OFFS                                               (GSI_REG_BASE_OFFS + 0x00000020)
#define HWIO_GSI_PERIPH_PENDING_RMSK                                               0xffffffff
#define HWIO_GSI_PERIPH_PENDING_ATTR                                                      0x1
#define HWIO_GSI_PERIPH_PENDING_IN          \
        in_dword_masked(HWIO_GSI_PERIPH_PENDING_ADDR, HWIO_GSI_PERIPH_PENDING_RMSK)
#define HWIO_GSI_PERIPH_PENDING_INM(m)      \
        in_dword_masked(HWIO_GSI_PERIPH_PENDING_ADDR, m)
#define HWIO_GSI_PERIPH_PENDING_CHID_BIT_MAP_BMSK                                  0xffffffff
#define HWIO_GSI_PERIPH_PENDING_CHID_BIT_MAP_SHFT                                         0x0

#define HWIO_GSI_MOQA_CFG_ADDR                                                     (GSI_REG_BASE      + 0x00000030)
#define HWIO_GSI_MOQA_CFG_PHYS                                                     (GSI_REG_BASE_PHYS + 0x00000030)
#define HWIO_GSI_MOQA_CFG_OFFS                                                     (GSI_REG_BASE_OFFS + 0x00000030)
#define HWIO_GSI_MOQA_CFG_RMSK                                                       0xffffff
#define HWIO_GSI_MOQA_CFG_ATTR                                                            0x3
#define HWIO_GSI_MOQA_CFG_IN          \
        in_dword_masked(HWIO_GSI_MOQA_CFG_ADDR, HWIO_GSI_MOQA_CFG_RMSK)
#define HWIO_GSI_MOQA_CFG_INM(m)      \
        in_dword_masked(HWIO_GSI_MOQA_CFG_ADDR, m)
#define HWIO_GSI_MOQA_CFG_OUT(v)      \
        out_dword(HWIO_GSI_MOQA_CFG_ADDR,v)
#define HWIO_GSI_MOQA_CFG_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GSI_MOQA_CFG_ADDR,m,v,HWIO_GSI_MOQA_CFG_IN)
#define HWIO_GSI_MOQA_CFG_CLIENT_OOWR_BMSK                                           0xff0000
#define HWIO_GSI_MOQA_CFG_CLIENT_OOWR_SHFT                                               0x10
#define HWIO_GSI_MOQA_CFG_CLIENT_OORD_BMSK                                             0xff00
#define HWIO_GSI_MOQA_CFG_CLIENT_OORD_SHFT                                                0x8
#define HWIO_GSI_MOQA_CFG_CLIENT_REQ_PRIO_BMSK                                           0xff
#define HWIO_GSI_MOQA_CFG_CLIENT_REQ_PRIO_SHFT                                            0x0

#define HWIO_GSI_REE_CFG_ADDR                                                      (GSI_REG_BASE      + 0x00000038)
#define HWIO_GSI_REE_CFG_PHYS                                                      (GSI_REG_BASE_PHYS + 0x00000038)
#define HWIO_GSI_REE_CFG_OFFS                                                      (GSI_REG_BASE_OFFS + 0x00000038)
#define HWIO_GSI_REE_CFG_RMSK                                                          0xff01
#define HWIO_GSI_REE_CFG_ATTR                                                             0x3
#define HWIO_GSI_REE_CFG_IN          \
        in_dword_masked(HWIO_GSI_REE_CFG_ADDR, HWIO_GSI_REE_CFG_RMSK)
#define HWIO_GSI_REE_CFG_INM(m)      \
        in_dword_masked(HWIO_GSI_REE_CFG_ADDR, m)
#define HWIO_GSI_REE_CFG_OUT(v)      \
        out_dword(HWIO_GSI_REE_CFG_ADDR,v)
#define HWIO_GSI_REE_CFG_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GSI_REE_CFG_ADDR,m,v,HWIO_GSI_REE_CFG_IN)
#define HWIO_GSI_REE_CFG_MAX_BURST_SIZE_BMSK                                           0xff00
#define HWIO_GSI_REE_CFG_MAX_BURST_SIZE_SHFT                                              0x8
#define HWIO_GSI_REE_CFG_MOVE_TO_ESC_CLR_MODE_TRSH_BMSK                                   0x1
#define HWIO_GSI_REE_CFG_MOVE_TO_ESC_CLR_MODE_TRSH_SHFT                                   0x0

#define HWIO_GSI_SHRAM_WR_WRR_ADDR                                                 (GSI_REG_BASE      + 0x00000050)
#define HWIO_GSI_SHRAM_WR_WRR_PHYS                                                 (GSI_REG_BASE_PHYS + 0x00000050)
#define HWIO_GSI_SHRAM_WR_WRR_OFFS                                                 (GSI_REG_BASE_OFFS + 0x00000050)
#define HWIO_GSI_SHRAM_WR_WRR_RMSK                                                     0xffff
#define HWIO_GSI_SHRAM_WR_WRR_ATTR                                                        0x3
#define HWIO_GSI_SHRAM_WR_WRR_IN          \
        in_dword_masked(HWIO_GSI_SHRAM_WR_WRR_ADDR, HWIO_GSI_SHRAM_WR_WRR_RMSK)
#define HWIO_GSI_SHRAM_WR_WRR_INM(m)      \
        in_dword_masked(HWIO_GSI_SHRAM_WR_WRR_ADDR, m)
#define HWIO_GSI_SHRAM_WR_WRR_OUT(v)      \
        out_dword(HWIO_GSI_SHRAM_WR_WRR_ADDR,v)
#define HWIO_GSI_SHRAM_WR_WRR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GSI_SHRAM_WR_WRR_ADDR,m,v,HWIO_GSI_SHRAM_WR_WRR_IN)
#define HWIO_GSI_SHRAM_WR_WRR_CLIENT3_WR_WEIGHT_BMSK                                   0xf000
#define HWIO_GSI_SHRAM_WR_WRR_CLIENT3_WR_WEIGHT_SHFT                                      0xc
#define HWIO_GSI_SHRAM_WR_WRR_CLIENT2_WR_WEIGHT_BMSK                                    0xf00
#define HWIO_GSI_SHRAM_WR_WRR_CLIENT2_WR_WEIGHT_SHFT                                      0x8
#define HWIO_GSI_SHRAM_WR_WRR_CLIENT1_WR_WEIGHT_BMSK                                     0xf0
#define HWIO_GSI_SHRAM_WR_WRR_CLIENT1_WR_WEIGHT_SHFT                                      0x4
#define HWIO_GSI_SHRAM_WR_WRR_CLIENT0_WR_WEIGHT_BMSK                                      0xf
#define HWIO_GSI_SHRAM_WR_WRR_CLIENT0_WR_WEIGHT_SHFT                                      0x0

#define HWIO_GSI_SHRAM_RD_WRR_ADDR                                                 (GSI_REG_BASE      + 0x00000058)
#define HWIO_GSI_SHRAM_RD_WRR_PHYS                                                 (GSI_REG_BASE_PHYS + 0x00000058)
#define HWIO_GSI_SHRAM_RD_WRR_OFFS                                                 (GSI_REG_BASE_OFFS + 0x00000058)
#define HWIO_GSI_SHRAM_RD_WRR_RMSK                                                   0xffffff
#define HWIO_GSI_SHRAM_RD_WRR_ATTR                                                        0x3
#define HWIO_GSI_SHRAM_RD_WRR_IN          \
        in_dword_masked(HWIO_GSI_SHRAM_RD_WRR_ADDR, HWIO_GSI_SHRAM_RD_WRR_RMSK)
#define HWIO_GSI_SHRAM_RD_WRR_INM(m)      \
        in_dword_masked(HWIO_GSI_SHRAM_RD_WRR_ADDR, m)
#define HWIO_GSI_SHRAM_RD_WRR_OUT(v)      \
        out_dword(HWIO_GSI_SHRAM_RD_WRR_ADDR,v)
#define HWIO_GSI_SHRAM_RD_WRR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GSI_SHRAM_RD_WRR_ADDR,m,v,HWIO_GSI_SHRAM_RD_WRR_IN)
#define HWIO_GSI_SHRAM_RD_WRR_ACH_SHRAM_RD_WEIGHT_BMSK                               0xf00000
#define HWIO_GSI_SHRAM_RD_WRR_ACH_SHRAM_RD_WEIGHT_SHFT                                   0x14
#define HWIO_GSI_SHRAM_RD_WRR_IE_SHRAM_RD_WEIGHT_BMSK                                 0xf0000
#define HWIO_GSI_SHRAM_RD_WRR_IE_SHRAM_RD_WEIGHT_SHFT                                    0x10
#define HWIO_GSI_SHRAM_RD_WRR_CSR_SHRAM_RD_WEIGHT_BMSK                                 0xf000
#define HWIO_GSI_SHRAM_RD_WRR_CSR_SHRAM_RD_WEIGHT_SHFT                                    0xc
#define HWIO_GSI_SHRAM_RD_WRR_RE_CNTXT_SHRAM_RD_WEIGHT_BMSK                             0xf00
#define HWIO_GSI_SHRAM_RD_WRR_RE_CNTXT_SHRAM_RD_WEIGHT_SHFT                               0x8
#define HWIO_GSI_SHRAM_RD_WRR_MCS_LD_SHRAM_RD_WEIGHT_BMSK                                0xf0
#define HWIO_GSI_SHRAM_RD_WRR_MCS_LD_SHRAM_RD_WEIGHT_SHFT                                 0x4
#define HWIO_GSI_SHRAM_RD_WRR_EV_ENG_SHRAM_RD_WEIGHT_BMSK                                 0xf
#define HWIO_GSI_SHRAM_RD_WRR_EV_ENG_SHRAM_RD_WEIGHT_SHFT                                 0x0

#define HWIO_GSI_CGC_CTRL_ADDR                                                     (GSI_REG_BASE      + 0x00000060)
#define HWIO_GSI_CGC_CTRL_PHYS                                                     (GSI_REG_BASE_PHYS + 0x00000060)
#define HWIO_GSI_CGC_CTRL_OFFS                                                     (GSI_REG_BASE_OFFS + 0x00000060)
#define HWIO_GSI_CGC_CTRL_RMSK                                                          0x1ff
#define HWIO_GSI_CGC_CTRL_ATTR                                                            0x3
#define HWIO_GSI_CGC_CTRL_IN          \
        in_dword_masked(HWIO_GSI_CGC_CTRL_ADDR, HWIO_GSI_CGC_CTRL_RMSK)
#define HWIO_GSI_CGC_CTRL_INM(m)      \
        in_dword_masked(HWIO_GSI_CGC_CTRL_ADDR, m)
#define HWIO_GSI_CGC_CTRL_OUT(v)      \
        out_dword(HWIO_GSI_CGC_CTRL_ADDR,v)
#define HWIO_GSI_CGC_CTRL_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GSI_CGC_CTRL_ADDR,m,v,HWIO_GSI_CGC_CTRL_IN)
#define HWIO_GSI_CGC_CTRL_REGION_9_HW_CGC_EN_BMSK                                       0x100
#define HWIO_GSI_CGC_CTRL_REGION_9_HW_CGC_EN_SHFT                                         0x8
#define HWIO_GSI_CGC_CTRL_REGION_8_HW_CGC_EN_BMSK                                        0x80
#define HWIO_GSI_CGC_CTRL_REGION_8_HW_CGC_EN_SHFT                                         0x7
#define HWIO_GSI_CGC_CTRL_REGION_7_HW_CGC_EN_BMSK                                        0x40
#define HWIO_GSI_CGC_CTRL_REGION_7_HW_CGC_EN_SHFT                                         0x6
#define HWIO_GSI_CGC_CTRL_REGION_6_HW_CGC_EN_BMSK                                        0x20
#define HWIO_GSI_CGC_CTRL_REGION_6_HW_CGC_EN_SHFT                                         0x5
#define HWIO_GSI_CGC_CTRL_REGION_5_HW_CGC_EN_BMSK                                        0x10
#define HWIO_GSI_CGC_CTRL_REGION_5_HW_CGC_EN_SHFT                                         0x4
#define HWIO_GSI_CGC_CTRL_REGION_4_HW_CGC_EN_BMSK                                         0x8
#define HWIO_GSI_CGC_CTRL_REGION_4_HW_CGC_EN_SHFT                                         0x3
#define HWIO_GSI_CGC_CTRL_REGION_3_HW_CGC_EN_BMSK                                         0x4
#define HWIO_GSI_CGC_CTRL_REGION_3_HW_CGC_EN_SHFT                                         0x2
#define HWIO_GSI_CGC_CTRL_REGION_2_HW_CGC_EN_BMSK                                         0x2
#define HWIO_GSI_CGC_CTRL_REGION_2_HW_CGC_EN_SHFT                                         0x1
#define HWIO_GSI_CGC_CTRL_REGION_1_HW_CGC_EN_BMSK                                         0x1
#define HWIO_GSI_CGC_CTRL_REGION_1_HW_CGC_EN_SHFT                                         0x0

#define HWIO_GSI_MSI_CACHEATTR_ADDR                                                (GSI_REG_BASE      + 0x00000080)
#define HWIO_GSI_MSI_CACHEATTR_PHYS                                                (GSI_REG_BASE_PHYS + 0x00000080)
#define HWIO_GSI_MSI_CACHEATTR_OFFS                                                (GSI_REG_BASE_OFFS + 0x00000080)
#define HWIO_GSI_MSI_CACHEATTR_RMSK                                                      0x3f
#define HWIO_GSI_MSI_CACHEATTR_ATTR                                                       0x3
#define HWIO_GSI_MSI_CACHEATTR_IN          \
        in_dword_masked(HWIO_GSI_MSI_CACHEATTR_ADDR, HWIO_GSI_MSI_CACHEATTR_RMSK)
#define HWIO_GSI_MSI_CACHEATTR_INM(m)      \
        in_dword_masked(HWIO_GSI_MSI_CACHEATTR_ADDR, m)
#define HWIO_GSI_MSI_CACHEATTR_OUT(v)      \
        out_dword(HWIO_GSI_MSI_CACHEATTR_ADDR,v)
#define HWIO_GSI_MSI_CACHEATTR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GSI_MSI_CACHEATTR_ADDR,m,v,HWIO_GSI_MSI_CACHEATTR_IN)
#define HWIO_GSI_MSI_CACHEATTR_AREQPRIORITY_BMSK                                         0x30
#define HWIO_GSI_MSI_CACHEATTR_AREQPRIORITY_SHFT                                          0x4
#define HWIO_GSI_MSI_CACHEATTR_ATRANSIENT_BMSK                                            0x8
#define HWIO_GSI_MSI_CACHEATTR_ATRANSIENT_SHFT                                            0x3
#define HWIO_GSI_MSI_CACHEATTR_ANOALLOCATE_BMSK                                           0x4
#define HWIO_GSI_MSI_CACHEATTR_ANOALLOCATE_SHFT                                           0x2
#define HWIO_GSI_MSI_CACHEATTR_AINNERSHARED_BMSK                                          0x2
#define HWIO_GSI_MSI_CACHEATTR_AINNERSHARED_SHFT                                          0x1
#define HWIO_GSI_MSI_CACHEATTR_ASHARED_BMSK                                               0x1
#define HWIO_GSI_MSI_CACHEATTR_ASHARED_SHFT                                               0x0

#define HWIO_GSI_EVENT_CACHEATTR_ADDR                                              (GSI_REG_BASE      + 0x00000084)
#define HWIO_GSI_EVENT_CACHEATTR_PHYS                                              (GSI_REG_BASE_PHYS + 0x00000084)
#define HWIO_GSI_EVENT_CACHEATTR_OFFS                                              (GSI_REG_BASE_OFFS + 0x00000084)
#define HWIO_GSI_EVENT_CACHEATTR_RMSK                                                    0x3f
#define HWIO_GSI_EVENT_CACHEATTR_ATTR                                                     0x3
#define HWIO_GSI_EVENT_CACHEATTR_IN          \
        in_dword_masked(HWIO_GSI_EVENT_CACHEATTR_ADDR, HWIO_GSI_EVENT_CACHEATTR_RMSK)
#define HWIO_GSI_EVENT_CACHEATTR_INM(m)      \
        in_dword_masked(HWIO_GSI_EVENT_CACHEATTR_ADDR, m)
#define HWIO_GSI_EVENT_CACHEATTR_OUT(v)      \
        out_dword(HWIO_GSI_EVENT_CACHEATTR_ADDR,v)
#define HWIO_GSI_EVENT_CACHEATTR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GSI_EVENT_CACHEATTR_ADDR,m,v,HWIO_GSI_EVENT_CACHEATTR_IN)
#define HWIO_GSI_EVENT_CACHEATTR_AREQPRIORITY_BMSK                                       0x30
#define HWIO_GSI_EVENT_CACHEATTR_AREQPRIORITY_SHFT                                        0x4
#define HWIO_GSI_EVENT_CACHEATTR_ATRANSIENT_BMSK                                          0x8
#define HWIO_GSI_EVENT_CACHEATTR_ATRANSIENT_SHFT                                          0x3
#define HWIO_GSI_EVENT_CACHEATTR_ANOALLOCATE_BMSK                                         0x4
#define HWIO_GSI_EVENT_CACHEATTR_ANOALLOCATE_SHFT                                         0x2
#define HWIO_GSI_EVENT_CACHEATTR_AINNERSHARED_BMSK                                        0x2
#define HWIO_GSI_EVENT_CACHEATTR_AINNERSHARED_SHFT                                        0x1
#define HWIO_GSI_EVENT_CACHEATTR_ASHARED_BMSK                                             0x1
#define HWIO_GSI_EVENT_CACHEATTR_ASHARED_SHFT                                             0x0

#define HWIO_GSI_DATA_CACHEATTR_ADDR                                               (GSI_REG_BASE      + 0x00000088)
#define HWIO_GSI_DATA_CACHEATTR_PHYS                                               (GSI_REG_BASE_PHYS + 0x00000088)
#define HWIO_GSI_DATA_CACHEATTR_OFFS                                               (GSI_REG_BASE_OFFS + 0x00000088)
#define HWIO_GSI_DATA_CACHEATTR_RMSK                                                     0x3f
#define HWIO_GSI_DATA_CACHEATTR_ATTR                                                      0x3
#define HWIO_GSI_DATA_CACHEATTR_IN          \
        in_dword_masked(HWIO_GSI_DATA_CACHEATTR_ADDR, HWIO_GSI_DATA_CACHEATTR_RMSK)
#define HWIO_GSI_DATA_CACHEATTR_INM(m)      \
        in_dword_masked(HWIO_GSI_DATA_CACHEATTR_ADDR, m)
#define HWIO_GSI_DATA_CACHEATTR_OUT(v)      \
        out_dword(HWIO_GSI_DATA_CACHEATTR_ADDR,v)
#define HWIO_GSI_DATA_CACHEATTR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GSI_DATA_CACHEATTR_ADDR,m,v,HWIO_GSI_DATA_CACHEATTR_IN)
#define HWIO_GSI_DATA_CACHEATTR_AREQPRIORITY_BMSK                                        0x30
#define HWIO_GSI_DATA_CACHEATTR_AREQPRIORITY_SHFT                                         0x4
#define HWIO_GSI_DATA_CACHEATTR_ATRANSIENT_BMSK                                           0x8
#define HWIO_GSI_DATA_CACHEATTR_ATRANSIENT_SHFT                                           0x3
#define HWIO_GSI_DATA_CACHEATTR_ANOALLOCATE_BMSK                                          0x4
#define HWIO_GSI_DATA_CACHEATTR_ANOALLOCATE_SHFT                                          0x2
#define HWIO_GSI_DATA_CACHEATTR_AINNERSHARED_BMSK                                         0x2
#define HWIO_GSI_DATA_CACHEATTR_AINNERSHARED_SHFT                                         0x1
#define HWIO_GSI_DATA_CACHEATTR_ASHARED_BMSK                                              0x1
#define HWIO_GSI_DATA_CACHEATTR_ASHARED_SHFT                                              0x0

#define HWIO_GSI_TRE_CACHEATTR_ADDR                                                (GSI_REG_BASE      + 0x00000090)
#define HWIO_GSI_TRE_CACHEATTR_PHYS                                                (GSI_REG_BASE_PHYS + 0x00000090)
#define HWIO_GSI_TRE_CACHEATTR_OFFS                                                (GSI_REG_BASE_OFFS + 0x00000090)
#define HWIO_GSI_TRE_CACHEATTR_RMSK                                                      0x3f
#define HWIO_GSI_TRE_CACHEATTR_ATTR                                                       0x3
#define HWIO_GSI_TRE_CACHEATTR_IN          \
        in_dword_masked(HWIO_GSI_TRE_CACHEATTR_ADDR, HWIO_GSI_TRE_CACHEATTR_RMSK)
#define HWIO_GSI_TRE_CACHEATTR_INM(m)      \
        in_dword_masked(HWIO_GSI_TRE_CACHEATTR_ADDR, m)
#define HWIO_GSI_TRE_CACHEATTR_OUT(v)      \
        out_dword(HWIO_GSI_TRE_CACHEATTR_ADDR,v)
#define HWIO_GSI_TRE_CACHEATTR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GSI_TRE_CACHEATTR_ADDR,m,v,HWIO_GSI_TRE_CACHEATTR_IN)
#define HWIO_GSI_TRE_CACHEATTR_AREQPRIORITY_BMSK                                         0x30
#define HWIO_GSI_TRE_CACHEATTR_AREQPRIORITY_SHFT                                          0x4
#define HWIO_GSI_TRE_CACHEATTR_ATRANSIENT_BMSK                                            0x8
#define HWIO_GSI_TRE_CACHEATTR_ATRANSIENT_SHFT                                            0x3
#define HWIO_GSI_TRE_CACHEATTR_ANOALLOCATE_BMSK                                           0x4
#define HWIO_GSI_TRE_CACHEATTR_ANOALLOCATE_SHFT                                           0x2
#define HWIO_GSI_TRE_CACHEATTR_AINNERSHARED_BMSK                                          0x2
#define HWIO_GSI_TRE_CACHEATTR_AINNERSHARED_SHFT                                          0x1
#define HWIO_GSI_TRE_CACHEATTR_ASHARED_BMSK                                               0x1
#define HWIO_GSI_TRE_CACHEATTR_ASHARED_SHFT                                               0x0

#define HWIO_IC_DISABLE_CHNL_BCK_PRS_LSB_ADDR                                      (GSI_REG_BASE      + 0x000000a0)
#define HWIO_IC_DISABLE_CHNL_BCK_PRS_LSB_PHYS                                      (GSI_REG_BASE_PHYS + 0x000000a0)
#define HWIO_IC_DISABLE_CHNL_BCK_PRS_LSB_OFFS                                      (GSI_REG_BASE_OFFS + 0x000000a0)
#define HWIO_IC_DISABLE_CHNL_BCK_PRS_LSB_RMSK                                      0x3ffc1047
#define HWIO_IC_DISABLE_CHNL_BCK_PRS_LSB_ATTR                                             0x3
#define HWIO_IC_DISABLE_CHNL_BCK_PRS_LSB_IN          \
        in_dword_masked(HWIO_IC_DISABLE_CHNL_BCK_PRS_LSB_ADDR, HWIO_IC_DISABLE_CHNL_BCK_PRS_LSB_RMSK)
#define HWIO_IC_DISABLE_CHNL_BCK_PRS_LSB_INM(m)      \
        in_dword_masked(HWIO_IC_DISABLE_CHNL_BCK_PRS_LSB_ADDR, m)
#define HWIO_IC_DISABLE_CHNL_BCK_PRS_LSB_OUT(v)      \
        out_dword(HWIO_IC_DISABLE_CHNL_BCK_PRS_LSB_ADDR,v)
#define HWIO_IC_DISABLE_CHNL_BCK_PRS_LSB_OUTM(m,v) \
        out_dword_masked_ns(HWIO_IC_DISABLE_CHNL_BCK_PRS_LSB_ADDR,m,v,HWIO_IC_DISABLE_CHNL_BCK_PRS_LSB_IN)
#define HWIO_IC_DISABLE_CHNL_BCK_PRS_LSB_TLV_INT_BMSK                              0x3f000000
#define HWIO_IC_DISABLE_CHNL_BCK_PRS_LSB_TLV_INT_SHFT                                    0x18
#define HWIO_IC_DISABLE_CHNL_BCK_PRS_LSB_CSR_INT_BMSK                                0xfc0000
#define HWIO_IC_DISABLE_CHNL_BCK_PRS_LSB_CSR_INT_SHFT                                    0x12
#define HWIO_IC_DISABLE_CHNL_BCK_PRS_LSB_INT_END_INT_BMSK                              0x1000
#define HWIO_IC_DISABLE_CHNL_BCK_PRS_LSB_INT_END_INT_SHFT                                 0xc
#define HWIO_IC_DISABLE_CHNL_BCK_PRS_LSB_EV_ENG_INT_BMSK                                 0x40
#define HWIO_IC_DISABLE_CHNL_BCK_PRS_LSB_EV_ENG_INT_SHFT                                  0x6
#define HWIO_IC_DISABLE_CHNL_BCK_PRS_LSB_REE_INT_BMSK                                     0x7
#define HWIO_IC_DISABLE_CHNL_BCK_PRS_LSB_REE_INT_SHFT                                     0x0

#define HWIO_IC_DISABLE_CHNL_BCK_PRS_MSB_ADDR                                      (GSI_REG_BASE      + 0x000000a4)
#define HWIO_IC_DISABLE_CHNL_BCK_PRS_MSB_PHYS                                      (GSI_REG_BASE_PHYS + 0x000000a4)
#define HWIO_IC_DISABLE_CHNL_BCK_PRS_MSB_OFFS                                      (GSI_REG_BASE_OFFS + 0x000000a4)
#define HWIO_IC_DISABLE_CHNL_BCK_PRS_MSB_RMSK                                        0xfc3041
#define HWIO_IC_DISABLE_CHNL_BCK_PRS_MSB_ATTR                                             0x3
#define HWIO_IC_DISABLE_CHNL_BCK_PRS_MSB_IN          \
        in_dword_masked(HWIO_IC_DISABLE_CHNL_BCK_PRS_MSB_ADDR, HWIO_IC_DISABLE_CHNL_BCK_PRS_MSB_RMSK)
#define HWIO_IC_DISABLE_CHNL_BCK_PRS_MSB_INM(m)      \
        in_dword_masked(HWIO_IC_DISABLE_CHNL_BCK_PRS_MSB_ADDR, m)
#define HWIO_IC_DISABLE_CHNL_BCK_PRS_MSB_OUT(v)      \
        out_dword(HWIO_IC_DISABLE_CHNL_BCK_PRS_MSB_ADDR,v)
#define HWIO_IC_DISABLE_CHNL_BCK_PRS_MSB_OUTM(m,v) \
        out_dword_masked_ns(HWIO_IC_DISABLE_CHNL_BCK_PRS_MSB_ADDR,m,v,HWIO_IC_DISABLE_CHNL_BCK_PRS_MSB_IN)
#define HWIO_IC_DISABLE_CHNL_BCK_PRS_MSB_UCONTROLLER_INT_BMSK                        0xfc0000
#define HWIO_IC_DISABLE_CHNL_BCK_PRS_MSB_UCONTROLLER_INT_SHFT                            0x12
#define HWIO_IC_DISABLE_CHNL_BCK_PRS_MSB_RD_WR_INT_BMSK                                0x3000
#define HWIO_IC_DISABLE_CHNL_BCK_PRS_MSB_RD_WR_INT_SHFT                                   0xc
#define HWIO_IC_DISABLE_CHNL_BCK_PRS_MSB_DB_ENG_INT_BMSK                                 0x40
#define HWIO_IC_DISABLE_CHNL_BCK_PRS_MSB_DB_ENG_INT_SHFT                                  0x6
#define HWIO_IC_DISABLE_CHNL_BCK_PRS_MSB_TIMER_INT_BMSK                                   0x1
#define HWIO_IC_DISABLE_CHNL_BCK_PRS_MSB_TIMER_INT_SHFT                                   0x0

#define HWIO_IC_GEN_EVNT_BCK_PRS_LSB_ADDR                                          (GSI_REG_BASE      + 0x000000a8)
#define HWIO_IC_GEN_EVNT_BCK_PRS_LSB_PHYS                                          (GSI_REG_BASE_PHYS + 0x000000a8)
#define HWIO_IC_GEN_EVNT_BCK_PRS_LSB_OFFS                                          (GSI_REG_BASE_OFFS + 0x000000a8)
#define HWIO_IC_GEN_EVNT_BCK_PRS_LSB_RMSK                                          0x3ffc1047
#define HWIO_IC_GEN_EVNT_BCK_PRS_LSB_ATTR                                                 0x3
#define HWIO_IC_GEN_EVNT_BCK_PRS_LSB_IN          \
        in_dword_masked(HWIO_IC_GEN_EVNT_BCK_PRS_LSB_ADDR, HWIO_IC_GEN_EVNT_BCK_PRS_LSB_RMSK)
#define HWIO_IC_GEN_EVNT_BCK_PRS_LSB_INM(m)      \
        in_dword_masked(HWIO_IC_GEN_EVNT_BCK_PRS_LSB_ADDR, m)
#define HWIO_IC_GEN_EVNT_BCK_PRS_LSB_OUT(v)      \
        out_dword(HWIO_IC_GEN_EVNT_BCK_PRS_LSB_ADDR,v)
#define HWIO_IC_GEN_EVNT_BCK_PRS_LSB_OUTM(m,v) \
        out_dword_masked_ns(HWIO_IC_GEN_EVNT_BCK_PRS_LSB_ADDR,m,v,HWIO_IC_GEN_EVNT_BCK_PRS_LSB_IN)
#define HWIO_IC_GEN_EVNT_BCK_PRS_LSB_TLV_INT_BMSK                                  0x3f000000
#define HWIO_IC_GEN_EVNT_BCK_PRS_LSB_TLV_INT_SHFT                                        0x18
#define HWIO_IC_GEN_EVNT_BCK_PRS_LSB_CSR_INT_BMSK                                    0xfc0000
#define HWIO_IC_GEN_EVNT_BCK_PRS_LSB_CSR_INT_SHFT                                        0x12
#define HWIO_IC_GEN_EVNT_BCK_PRS_LSB_INT_END_INT_BMSK                                  0x1000
#define HWIO_IC_GEN_EVNT_BCK_PRS_LSB_INT_END_INT_SHFT                                     0xc
#define HWIO_IC_GEN_EVNT_BCK_PRS_LSB_EV_ENG_INT_BMSK                                     0x40
#define HWIO_IC_GEN_EVNT_BCK_PRS_LSB_EV_ENG_INT_SHFT                                      0x6
#define HWIO_IC_GEN_EVNT_BCK_PRS_LSB_REE_INT_BMSK                                         0x7
#define HWIO_IC_GEN_EVNT_BCK_PRS_LSB_REE_INT_SHFT                                         0x0

#define HWIO_IC_GEN_EVNT_BCK_PRS_MSB_ADDR                                          (GSI_REG_BASE      + 0x000000ac)
#define HWIO_IC_GEN_EVNT_BCK_PRS_MSB_PHYS                                          (GSI_REG_BASE_PHYS + 0x000000ac)
#define HWIO_IC_GEN_EVNT_BCK_PRS_MSB_OFFS                                          (GSI_REG_BASE_OFFS + 0x000000ac)
#define HWIO_IC_GEN_EVNT_BCK_PRS_MSB_RMSK                                            0xfc3041
#define HWIO_IC_GEN_EVNT_BCK_PRS_MSB_ATTR                                                 0x3
#define HWIO_IC_GEN_EVNT_BCK_PRS_MSB_IN          \
        in_dword_masked(HWIO_IC_GEN_EVNT_BCK_PRS_MSB_ADDR, HWIO_IC_GEN_EVNT_BCK_PRS_MSB_RMSK)
#define HWIO_IC_GEN_EVNT_BCK_PRS_MSB_INM(m)      \
        in_dword_masked(HWIO_IC_GEN_EVNT_BCK_PRS_MSB_ADDR, m)
#define HWIO_IC_GEN_EVNT_BCK_PRS_MSB_OUT(v)      \
        out_dword(HWIO_IC_GEN_EVNT_BCK_PRS_MSB_ADDR,v)
#define HWIO_IC_GEN_EVNT_BCK_PRS_MSB_OUTM(m,v) \
        out_dword_masked_ns(HWIO_IC_GEN_EVNT_BCK_PRS_MSB_ADDR,m,v,HWIO_IC_GEN_EVNT_BCK_PRS_MSB_IN)
#define HWIO_IC_GEN_EVNT_BCK_PRS_MSB_UCONTROLLER_INT_BMSK                            0xfc0000
#define HWIO_IC_GEN_EVNT_BCK_PRS_MSB_UCONTROLLER_INT_SHFT                                0x12
#define HWIO_IC_GEN_EVNT_BCK_PRS_MSB_RD_WR_INT_BMSK                                    0x3000
#define HWIO_IC_GEN_EVNT_BCK_PRS_MSB_RD_WR_INT_SHFT                                       0xc
#define HWIO_IC_GEN_EVNT_BCK_PRS_MSB_DB_ENG_INT_BMSK                                     0x40
#define HWIO_IC_GEN_EVNT_BCK_PRS_MSB_DB_ENG_INT_SHFT                                      0x6
#define HWIO_IC_GEN_EVNT_BCK_PRS_MSB_TIMER_INT_BMSK                                       0x1
#define HWIO_IC_GEN_EVNT_BCK_PRS_MSB_TIMER_INT_SHFT                                       0x0

#define HWIO_IC_GEN_INT_BCK_PRS_LSB_ADDR                                           (GSI_REG_BASE      + 0x000000b0)
#define HWIO_IC_GEN_INT_BCK_PRS_LSB_PHYS                                           (GSI_REG_BASE_PHYS + 0x000000b0)
#define HWIO_IC_GEN_INT_BCK_PRS_LSB_OFFS                                           (GSI_REG_BASE_OFFS + 0x000000b0)
#define HWIO_IC_GEN_INT_BCK_PRS_LSB_RMSK                                           0x3ffc1047
#define HWIO_IC_GEN_INT_BCK_PRS_LSB_ATTR                                                  0x3
#define HWIO_IC_GEN_INT_BCK_PRS_LSB_IN          \
        in_dword_masked(HWIO_IC_GEN_INT_BCK_PRS_LSB_ADDR, HWIO_IC_GEN_INT_BCK_PRS_LSB_RMSK)
#define HWIO_IC_GEN_INT_BCK_PRS_LSB_INM(m)      \
        in_dword_masked(HWIO_IC_GEN_INT_BCK_PRS_LSB_ADDR, m)
#define HWIO_IC_GEN_INT_BCK_PRS_LSB_OUT(v)      \
        out_dword(HWIO_IC_GEN_INT_BCK_PRS_LSB_ADDR,v)
#define HWIO_IC_GEN_INT_BCK_PRS_LSB_OUTM(m,v) \
        out_dword_masked_ns(HWIO_IC_GEN_INT_BCK_PRS_LSB_ADDR,m,v,HWIO_IC_GEN_INT_BCK_PRS_LSB_IN)
#define HWIO_IC_GEN_INT_BCK_PRS_LSB_TLV_INT_BMSK                                   0x3f000000
#define HWIO_IC_GEN_INT_BCK_PRS_LSB_TLV_INT_SHFT                                         0x18
#define HWIO_IC_GEN_INT_BCK_PRS_LSB_CSR_INT_BMSK                                     0xfc0000
#define HWIO_IC_GEN_INT_BCK_PRS_LSB_CSR_INT_SHFT                                         0x12
#define HWIO_IC_GEN_INT_BCK_PRS_LSB_INT_END_INT_BMSK                                   0x1000
#define HWIO_IC_GEN_INT_BCK_PRS_LSB_INT_END_INT_SHFT                                      0xc
#define HWIO_IC_GEN_INT_BCK_PRS_LSB_EV_ENG_INT_BMSK                                      0x40
#define HWIO_IC_GEN_INT_BCK_PRS_LSB_EV_ENG_INT_SHFT                                       0x6
#define HWIO_IC_GEN_INT_BCK_PRS_LSB_REE_INT_BMSK                                          0x7
#define HWIO_IC_GEN_INT_BCK_PRS_LSB_REE_INT_SHFT                                          0x0

#define HWIO_IC_GEN_INT_BCK_PRS_MSB_ADDR                                           (GSI_REG_BASE      + 0x000000b4)
#define HWIO_IC_GEN_INT_BCK_PRS_MSB_PHYS                                           (GSI_REG_BASE_PHYS + 0x000000b4)
#define HWIO_IC_GEN_INT_BCK_PRS_MSB_OFFS                                           (GSI_REG_BASE_OFFS + 0x000000b4)
#define HWIO_IC_GEN_INT_BCK_PRS_MSB_RMSK                                             0xfc3041
#define HWIO_IC_GEN_INT_BCK_PRS_MSB_ATTR                                                  0x3
#define HWIO_IC_GEN_INT_BCK_PRS_MSB_IN          \
        in_dword_masked(HWIO_IC_GEN_INT_BCK_PRS_MSB_ADDR, HWIO_IC_GEN_INT_BCK_PRS_MSB_RMSK)
#define HWIO_IC_GEN_INT_BCK_PRS_MSB_INM(m)      \
        in_dword_masked(HWIO_IC_GEN_INT_BCK_PRS_MSB_ADDR, m)
#define HWIO_IC_GEN_INT_BCK_PRS_MSB_OUT(v)      \
        out_dword(HWIO_IC_GEN_INT_BCK_PRS_MSB_ADDR,v)
#define HWIO_IC_GEN_INT_BCK_PRS_MSB_OUTM(m,v) \
        out_dword_masked_ns(HWIO_IC_GEN_INT_BCK_PRS_MSB_ADDR,m,v,HWIO_IC_GEN_INT_BCK_PRS_MSB_IN)
#define HWIO_IC_GEN_INT_BCK_PRS_MSB_UCONTROLLER_INT_BMSK                             0xfc0000
#define HWIO_IC_GEN_INT_BCK_PRS_MSB_UCONTROLLER_INT_SHFT                                 0x12
#define HWIO_IC_GEN_INT_BCK_PRS_MSB_RD_WR_INT_BMSK                                     0x3000
#define HWIO_IC_GEN_INT_BCK_PRS_MSB_RD_WR_INT_SHFT                                        0xc
#define HWIO_IC_GEN_INT_BCK_PRS_MSB_DB_ENG_INT_BMSK                                      0x40
#define HWIO_IC_GEN_INT_BCK_PRS_MSB_DB_ENG_INT_SHFT                                       0x6
#define HWIO_IC_GEN_INT_BCK_PRS_MSB_TIMER_INT_BMSK                                        0x1
#define HWIO_IC_GEN_INT_BCK_PRS_MSB_TIMER_INT_SHFT                                        0x0

#define HWIO_IC_STOP_INT_MOD_BCK_PRS_LSB_ADDR                                      (GSI_REG_BASE      + 0x000000b8)
#define HWIO_IC_STOP_INT_MOD_BCK_PRS_LSB_PHYS                                      (GSI_REG_BASE_PHYS + 0x000000b8)
#define HWIO_IC_STOP_INT_MOD_BCK_PRS_LSB_OFFS                                      (GSI_REG_BASE_OFFS + 0x000000b8)
#define HWIO_IC_STOP_INT_MOD_BCK_PRS_LSB_RMSK                                      0x3ffc1047
#define HWIO_IC_STOP_INT_MOD_BCK_PRS_LSB_ATTR                                             0x3
#define HWIO_IC_STOP_INT_MOD_BCK_PRS_LSB_IN          \
        in_dword_masked(HWIO_IC_STOP_INT_MOD_BCK_PRS_LSB_ADDR, HWIO_IC_STOP_INT_MOD_BCK_PRS_LSB_RMSK)
#define HWIO_IC_STOP_INT_MOD_BCK_PRS_LSB_INM(m)      \
        in_dword_masked(HWIO_IC_STOP_INT_MOD_BCK_PRS_LSB_ADDR, m)
#define HWIO_IC_STOP_INT_MOD_BCK_PRS_LSB_OUT(v)      \
        out_dword(HWIO_IC_STOP_INT_MOD_BCK_PRS_LSB_ADDR,v)
#define HWIO_IC_STOP_INT_MOD_BCK_PRS_LSB_OUTM(m,v) \
        out_dword_masked_ns(HWIO_IC_STOP_INT_MOD_BCK_PRS_LSB_ADDR,m,v,HWIO_IC_STOP_INT_MOD_BCK_PRS_LSB_IN)
#define HWIO_IC_STOP_INT_MOD_BCK_PRS_LSB_TLV_INT_BMSK                              0x3f000000
#define HWIO_IC_STOP_INT_MOD_BCK_PRS_LSB_TLV_INT_SHFT                                    0x18
#define HWIO_IC_STOP_INT_MOD_BCK_PRS_LSB_CSR_INT_BMSK                                0xfc0000
#define HWIO_IC_STOP_INT_MOD_BCK_PRS_LSB_CSR_INT_SHFT                                    0x12
#define HWIO_IC_STOP_INT_MOD_BCK_PRS_LSB_INT_END_INT_BMSK                              0x1000
#define HWIO_IC_STOP_INT_MOD_BCK_PRS_LSB_INT_END_INT_SHFT                                 0xc
#define HWIO_IC_STOP_INT_MOD_BCK_PRS_LSB_EV_ENG_INT_BMSK                                 0x40
#define HWIO_IC_STOP_INT_MOD_BCK_PRS_LSB_EV_ENG_INT_SHFT                                  0x6
#define HWIO_IC_STOP_INT_MOD_BCK_PRS_LSB_REE_INT_BMSK                                     0x7
#define HWIO_IC_STOP_INT_MOD_BCK_PRS_LSB_REE_INT_SHFT                                     0x0

#define HWIO_IC_STOP_INT_MOD_BCK_PRS_MSB_ADDR                                      (GSI_REG_BASE      + 0x000000bc)
#define HWIO_IC_STOP_INT_MOD_BCK_PRS_MSB_PHYS                                      (GSI_REG_BASE_PHYS + 0x000000bc)
#define HWIO_IC_STOP_INT_MOD_BCK_PRS_MSB_OFFS                                      (GSI_REG_BASE_OFFS + 0x000000bc)
#define HWIO_IC_STOP_INT_MOD_BCK_PRS_MSB_RMSK                                        0xfc3041
#define HWIO_IC_STOP_INT_MOD_BCK_PRS_MSB_ATTR                                             0x3
#define HWIO_IC_STOP_INT_MOD_BCK_PRS_MSB_IN          \
        in_dword_masked(HWIO_IC_STOP_INT_MOD_BCK_PRS_MSB_ADDR, HWIO_IC_STOP_INT_MOD_BCK_PRS_MSB_RMSK)
#define HWIO_IC_STOP_INT_MOD_BCK_PRS_MSB_INM(m)      \
        in_dword_masked(HWIO_IC_STOP_INT_MOD_BCK_PRS_MSB_ADDR, m)
#define HWIO_IC_STOP_INT_MOD_BCK_PRS_MSB_OUT(v)      \
        out_dword(HWIO_IC_STOP_INT_MOD_BCK_PRS_MSB_ADDR,v)
#define HWIO_IC_STOP_INT_MOD_BCK_PRS_MSB_OUTM(m,v) \
        out_dword_masked_ns(HWIO_IC_STOP_INT_MOD_BCK_PRS_MSB_ADDR,m,v,HWIO_IC_STOP_INT_MOD_BCK_PRS_MSB_IN)
#define HWIO_IC_STOP_INT_MOD_BCK_PRS_MSB_UCONTROLLER_INT_BMSK                        0xfc0000
#define HWIO_IC_STOP_INT_MOD_BCK_PRS_MSB_UCONTROLLER_INT_SHFT                            0x12
#define HWIO_IC_STOP_INT_MOD_BCK_PRS_MSB_RD_WR_INT_BMSK                                0x3000
#define HWIO_IC_STOP_INT_MOD_BCK_PRS_MSB_RD_WR_INT_SHFT                                   0xc
#define HWIO_IC_STOP_INT_MOD_BCK_PRS_MSB_DB_ENG_INT_BMSK                                 0x40
#define HWIO_IC_STOP_INT_MOD_BCK_PRS_MSB_DB_ENG_INT_SHFT                                  0x6
#define HWIO_IC_STOP_INT_MOD_BCK_PRS_MSB_TIMER_INT_BMSK                                   0x1
#define HWIO_IC_STOP_INT_MOD_BCK_PRS_MSB_TIMER_INT_SHFT                                   0x0

#define HWIO_IC_PROCESS_DESC_BCK_PRS_LSB_ADDR                                      (GSI_REG_BASE      + 0x000000c0)
#define HWIO_IC_PROCESS_DESC_BCK_PRS_LSB_PHYS                                      (GSI_REG_BASE_PHYS + 0x000000c0)
#define HWIO_IC_PROCESS_DESC_BCK_PRS_LSB_OFFS                                      (GSI_REG_BASE_OFFS + 0x000000c0)
#define HWIO_IC_PROCESS_DESC_BCK_PRS_LSB_RMSK                                      0x3ffc1047
#define HWIO_IC_PROCESS_DESC_BCK_PRS_LSB_ATTR                                             0x3
#define HWIO_IC_PROCESS_DESC_BCK_PRS_LSB_IN          \
        in_dword_masked(HWIO_IC_PROCESS_DESC_BCK_PRS_LSB_ADDR, HWIO_IC_PROCESS_DESC_BCK_PRS_LSB_RMSK)
#define HWIO_IC_PROCESS_DESC_BCK_PRS_LSB_INM(m)      \
        in_dword_masked(HWIO_IC_PROCESS_DESC_BCK_PRS_LSB_ADDR, m)
#define HWIO_IC_PROCESS_DESC_BCK_PRS_LSB_OUT(v)      \
        out_dword(HWIO_IC_PROCESS_DESC_BCK_PRS_LSB_ADDR,v)
#define HWIO_IC_PROCESS_DESC_BCK_PRS_LSB_OUTM(m,v) \
        out_dword_masked_ns(HWIO_IC_PROCESS_DESC_BCK_PRS_LSB_ADDR,m,v,HWIO_IC_PROCESS_DESC_BCK_PRS_LSB_IN)
#define HWIO_IC_PROCESS_DESC_BCK_PRS_LSB_TLV_INT_BMSK                              0x3f000000
#define HWIO_IC_PROCESS_DESC_BCK_PRS_LSB_TLV_INT_SHFT                                    0x18
#define HWIO_IC_PROCESS_DESC_BCK_PRS_LSB_CSR_INT_BMSK                                0xfc0000
#define HWIO_IC_PROCESS_DESC_BCK_PRS_LSB_CSR_INT_SHFT                                    0x12
#define HWIO_IC_PROCESS_DESC_BCK_PRS_LSB_INT_END_INT_BMSK                              0x1000
#define HWIO_IC_PROCESS_DESC_BCK_PRS_LSB_INT_END_INT_SHFT                                 0xc
#define HWIO_IC_PROCESS_DESC_BCK_PRS_LSB_EV_ENG_INT_BMSK                                 0x40
#define HWIO_IC_PROCESS_DESC_BCK_PRS_LSB_EV_ENG_INT_SHFT                                  0x6
#define HWIO_IC_PROCESS_DESC_BCK_PRS_LSB_REE_INT_BMSK                                     0x7
#define HWIO_IC_PROCESS_DESC_BCK_PRS_LSB_REE_INT_SHFT                                     0x0

#define HWIO_IC_PROCESS_DESC_BCK_PRS_MSB_ADDR                                      (GSI_REG_BASE      + 0x000000c4)
#define HWIO_IC_PROCESS_DESC_BCK_PRS_MSB_PHYS                                      (GSI_REG_BASE_PHYS + 0x000000c4)
#define HWIO_IC_PROCESS_DESC_BCK_PRS_MSB_OFFS                                      (GSI_REG_BASE_OFFS + 0x000000c4)
#define HWIO_IC_PROCESS_DESC_BCK_PRS_MSB_RMSK                                        0xfc3041
#define HWIO_IC_PROCESS_DESC_BCK_PRS_MSB_ATTR                                             0x3
#define HWIO_IC_PROCESS_DESC_BCK_PRS_MSB_IN          \
        in_dword_masked(HWIO_IC_PROCESS_DESC_BCK_PRS_MSB_ADDR, HWIO_IC_PROCESS_DESC_BCK_PRS_MSB_RMSK)
#define HWIO_IC_PROCESS_DESC_BCK_PRS_MSB_INM(m)      \
        in_dword_masked(HWIO_IC_PROCESS_DESC_BCK_PRS_MSB_ADDR, m)
#define HWIO_IC_PROCESS_DESC_BCK_PRS_MSB_OUT(v)      \
        out_dword(HWIO_IC_PROCESS_DESC_BCK_PRS_MSB_ADDR,v)
#define HWIO_IC_PROCESS_DESC_BCK_PRS_MSB_OUTM(m,v) \
        out_dword_masked_ns(HWIO_IC_PROCESS_DESC_BCK_PRS_MSB_ADDR,m,v,HWIO_IC_PROCESS_DESC_BCK_PRS_MSB_IN)
#define HWIO_IC_PROCESS_DESC_BCK_PRS_MSB_UCONTROLLER_INT_BMSK                        0xfc0000
#define HWIO_IC_PROCESS_DESC_BCK_PRS_MSB_UCONTROLLER_INT_SHFT                            0x12
#define HWIO_IC_PROCESS_DESC_BCK_PRS_MSB_RD_WR_INT_BMSK                                0x3000
#define HWIO_IC_PROCESS_DESC_BCK_PRS_MSB_RD_WR_INT_SHFT                                   0xc
#define HWIO_IC_PROCESS_DESC_BCK_PRS_MSB_DB_ENG_INT_BMSK                                 0x40
#define HWIO_IC_PROCESS_DESC_BCK_PRS_MSB_DB_ENG_INT_SHFT                                  0x6
#define HWIO_IC_PROCESS_DESC_BCK_PRS_MSB_TIMER_INT_BMSK                                   0x1
#define HWIO_IC_PROCESS_DESC_BCK_PRS_MSB_TIMER_INT_SHFT                                   0x0

#define HWIO_IC_TLV_STOP_BCK_PRS_LSB_ADDR                                          (GSI_REG_BASE      + 0x000000c8)
#define HWIO_IC_TLV_STOP_BCK_PRS_LSB_PHYS                                          (GSI_REG_BASE_PHYS + 0x000000c8)
#define HWIO_IC_TLV_STOP_BCK_PRS_LSB_OFFS                                          (GSI_REG_BASE_OFFS + 0x000000c8)
#define HWIO_IC_TLV_STOP_BCK_PRS_LSB_RMSK                                          0x3ffc1047
#define HWIO_IC_TLV_STOP_BCK_PRS_LSB_ATTR                                                 0x3
#define HWIO_IC_TLV_STOP_BCK_PRS_LSB_IN          \
        in_dword_masked(HWIO_IC_TLV_STOP_BCK_PRS_LSB_ADDR, HWIO_IC_TLV_STOP_BCK_PRS_LSB_RMSK)
#define HWIO_IC_TLV_STOP_BCK_PRS_LSB_INM(m)      \
        in_dword_masked(HWIO_IC_TLV_STOP_BCK_PRS_LSB_ADDR, m)
#define HWIO_IC_TLV_STOP_BCK_PRS_LSB_OUT(v)      \
        out_dword(HWIO_IC_TLV_STOP_BCK_PRS_LSB_ADDR,v)
#define HWIO_IC_TLV_STOP_BCK_PRS_LSB_OUTM(m,v) \
        out_dword_masked_ns(HWIO_IC_TLV_STOP_BCK_PRS_LSB_ADDR,m,v,HWIO_IC_TLV_STOP_BCK_PRS_LSB_IN)
#define HWIO_IC_TLV_STOP_BCK_PRS_LSB_TLV_INT_BMSK                                  0x3f000000
#define HWIO_IC_TLV_STOP_BCK_PRS_LSB_TLV_INT_SHFT                                        0x18
#define HWIO_IC_TLV_STOP_BCK_PRS_LSB_CSR_INT_BMSK                                    0xfc0000
#define HWIO_IC_TLV_STOP_BCK_PRS_LSB_CSR_INT_SHFT                                        0x12
#define HWIO_IC_TLV_STOP_BCK_PRS_LSB_INT_END_INT_BMSK                                  0x1000
#define HWIO_IC_TLV_STOP_BCK_PRS_LSB_INT_END_INT_SHFT                                     0xc
#define HWIO_IC_TLV_STOP_BCK_PRS_LSB_EV_ENG_INT_BMSK                                     0x40
#define HWIO_IC_TLV_STOP_BCK_PRS_LSB_EV_ENG_INT_SHFT                                      0x6
#define HWIO_IC_TLV_STOP_BCK_PRS_LSB_REE_INT_BMSK                                         0x7
#define HWIO_IC_TLV_STOP_BCK_PRS_LSB_REE_INT_SHFT                                         0x0

#define HWIO_IC_TLV_STOP_BCK_PRS_MSB_ADDR                                          (GSI_REG_BASE      + 0x000000cc)
#define HWIO_IC_TLV_STOP_BCK_PRS_MSB_PHYS                                          (GSI_REG_BASE_PHYS + 0x000000cc)
#define HWIO_IC_TLV_STOP_BCK_PRS_MSB_OFFS                                          (GSI_REG_BASE_OFFS + 0x000000cc)
#define HWIO_IC_TLV_STOP_BCK_PRS_MSB_RMSK                                            0xfc3041
#define HWIO_IC_TLV_STOP_BCK_PRS_MSB_ATTR                                                 0x3
#define HWIO_IC_TLV_STOP_BCK_PRS_MSB_IN          \
        in_dword_masked(HWIO_IC_TLV_STOP_BCK_PRS_MSB_ADDR, HWIO_IC_TLV_STOP_BCK_PRS_MSB_RMSK)
#define HWIO_IC_TLV_STOP_BCK_PRS_MSB_INM(m)      \
        in_dword_masked(HWIO_IC_TLV_STOP_BCK_PRS_MSB_ADDR, m)
#define HWIO_IC_TLV_STOP_BCK_PRS_MSB_OUT(v)      \
        out_dword(HWIO_IC_TLV_STOP_BCK_PRS_MSB_ADDR,v)
#define HWIO_IC_TLV_STOP_BCK_PRS_MSB_OUTM(m,v) \
        out_dword_masked_ns(HWIO_IC_TLV_STOP_BCK_PRS_MSB_ADDR,m,v,HWIO_IC_TLV_STOP_BCK_PRS_MSB_IN)
#define HWIO_IC_TLV_STOP_BCK_PRS_MSB_UCONTROLLER_INT_BMSK                            0xfc0000
#define HWIO_IC_TLV_STOP_BCK_PRS_MSB_UCONTROLLER_INT_SHFT                                0x12
#define HWIO_IC_TLV_STOP_BCK_PRS_MSB_RD_WR_INT_BMSK                                    0x3000
#define HWIO_IC_TLV_STOP_BCK_PRS_MSB_RD_WR_INT_SHFT                                       0xc
#define HWIO_IC_TLV_STOP_BCK_PRS_MSB_DB_ENG_INT_BMSK                                     0x40
#define HWIO_IC_TLV_STOP_BCK_PRS_MSB_DB_ENG_INT_SHFT                                      0x6
#define HWIO_IC_TLV_STOP_BCK_PRS_MSB_TIMER_INT_BMSK                                       0x1
#define HWIO_IC_TLV_STOP_BCK_PRS_MSB_TIMER_INT_SHFT                                       0x0

#define HWIO_IC_TLV_RESET_BCK_PRS_LSB_ADDR                                         (GSI_REG_BASE      + 0x000000d0)
#define HWIO_IC_TLV_RESET_BCK_PRS_LSB_PHYS                                         (GSI_REG_BASE_PHYS + 0x000000d0)
#define HWIO_IC_TLV_RESET_BCK_PRS_LSB_OFFS                                         (GSI_REG_BASE_OFFS + 0x000000d0)
#define HWIO_IC_TLV_RESET_BCK_PRS_LSB_RMSK                                         0x3ffc1047
#define HWIO_IC_TLV_RESET_BCK_PRS_LSB_ATTR                                                0x3
#define HWIO_IC_TLV_RESET_BCK_PRS_LSB_IN          \
        in_dword_masked(HWIO_IC_TLV_RESET_BCK_PRS_LSB_ADDR, HWIO_IC_TLV_RESET_BCK_PRS_LSB_RMSK)
#define HWIO_IC_TLV_RESET_BCK_PRS_LSB_INM(m)      \
        in_dword_masked(HWIO_IC_TLV_RESET_BCK_PRS_LSB_ADDR, m)
#define HWIO_IC_TLV_RESET_BCK_PRS_LSB_OUT(v)      \
        out_dword(HWIO_IC_TLV_RESET_BCK_PRS_LSB_ADDR,v)
#define HWIO_IC_TLV_RESET_BCK_PRS_LSB_OUTM(m,v) \
        out_dword_masked_ns(HWIO_IC_TLV_RESET_BCK_PRS_LSB_ADDR,m,v,HWIO_IC_TLV_RESET_BCK_PRS_LSB_IN)
#define HWIO_IC_TLV_RESET_BCK_PRS_LSB_TLV_INT_BMSK                                 0x3f000000
#define HWIO_IC_TLV_RESET_BCK_PRS_LSB_TLV_INT_SHFT                                       0x18
#define HWIO_IC_TLV_RESET_BCK_PRS_LSB_CSR_INT_BMSK                                   0xfc0000
#define HWIO_IC_TLV_RESET_BCK_PRS_LSB_CSR_INT_SHFT                                       0x12
#define HWIO_IC_TLV_RESET_BCK_PRS_LSB_INT_END_INT_BMSK                                 0x1000
#define HWIO_IC_TLV_RESET_BCK_PRS_LSB_INT_END_INT_SHFT                                    0xc
#define HWIO_IC_TLV_RESET_BCK_PRS_LSB_EV_ENG_INT_BMSK                                    0x40
#define HWIO_IC_TLV_RESET_BCK_PRS_LSB_EV_ENG_INT_SHFT                                     0x6
#define HWIO_IC_TLV_RESET_BCK_PRS_LSB_REE_INT_BMSK                                        0x7
#define HWIO_IC_TLV_RESET_BCK_PRS_LSB_REE_INT_SHFT                                        0x0

#define HWIO_IC_TLV_RESET_BCK_PRS_MSB_ADDR                                         (GSI_REG_BASE      + 0x000000d4)
#define HWIO_IC_TLV_RESET_BCK_PRS_MSB_PHYS                                         (GSI_REG_BASE_PHYS + 0x000000d4)
#define HWIO_IC_TLV_RESET_BCK_PRS_MSB_OFFS                                         (GSI_REG_BASE_OFFS + 0x000000d4)
#define HWIO_IC_TLV_RESET_BCK_PRS_MSB_RMSK                                           0xfc3041
#define HWIO_IC_TLV_RESET_BCK_PRS_MSB_ATTR                                                0x3
#define HWIO_IC_TLV_RESET_BCK_PRS_MSB_IN          \
        in_dword_masked(HWIO_IC_TLV_RESET_BCK_PRS_MSB_ADDR, HWIO_IC_TLV_RESET_BCK_PRS_MSB_RMSK)
#define HWIO_IC_TLV_RESET_BCK_PRS_MSB_INM(m)      \
        in_dword_masked(HWIO_IC_TLV_RESET_BCK_PRS_MSB_ADDR, m)
#define HWIO_IC_TLV_RESET_BCK_PRS_MSB_OUT(v)      \
        out_dword(HWIO_IC_TLV_RESET_BCK_PRS_MSB_ADDR,v)
#define HWIO_IC_TLV_RESET_BCK_PRS_MSB_OUTM(m,v) \
        out_dword_masked_ns(HWIO_IC_TLV_RESET_BCK_PRS_MSB_ADDR,m,v,HWIO_IC_TLV_RESET_BCK_PRS_MSB_IN)
#define HWIO_IC_TLV_RESET_BCK_PRS_MSB_UCONTROLLER_INT_BMSK                           0xfc0000
#define HWIO_IC_TLV_RESET_BCK_PRS_MSB_UCONTROLLER_INT_SHFT                               0x12
#define HWIO_IC_TLV_RESET_BCK_PRS_MSB_RD_WR_INT_BMSK                                   0x3000
#define HWIO_IC_TLV_RESET_BCK_PRS_MSB_RD_WR_INT_SHFT                                      0xc
#define HWIO_IC_TLV_RESET_BCK_PRS_MSB_DB_ENG_INT_BMSK                                    0x40
#define HWIO_IC_TLV_RESET_BCK_PRS_MSB_DB_ENG_INT_SHFT                                     0x6
#define HWIO_IC_TLV_RESET_BCK_PRS_MSB_TIMER_INT_BMSK                                      0x1
#define HWIO_IC_TLV_RESET_BCK_PRS_MSB_TIMER_INT_SHFT                                      0x0

#define HWIO_IC_RGSTR_TIMER_BCK_PRS_LSB_ADDR                                       (GSI_REG_BASE      + 0x000000d8)
#define HWIO_IC_RGSTR_TIMER_BCK_PRS_LSB_PHYS                                       (GSI_REG_BASE_PHYS + 0x000000d8)
#define HWIO_IC_RGSTR_TIMER_BCK_PRS_LSB_OFFS                                       (GSI_REG_BASE_OFFS + 0x000000d8)
#define HWIO_IC_RGSTR_TIMER_BCK_PRS_LSB_RMSK                                       0x3ffc1047
#define HWIO_IC_RGSTR_TIMER_BCK_PRS_LSB_ATTR                                              0x3
#define HWIO_IC_RGSTR_TIMER_BCK_PRS_LSB_IN          \
        in_dword_masked(HWIO_IC_RGSTR_TIMER_BCK_PRS_LSB_ADDR, HWIO_IC_RGSTR_TIMER_BCK_PRS_LSB_RMSK)
#define HWIO_IC_RGSTR_TIMER_BCK_PRS_LSB_INM(m)      \
        in_dword_masked(HWIO_IC_RGSTR_TIMER_BCK_PRS_LSB_ADDR, m)
#define HWIO_IC_RGSTR_TIMER_BCK_PRS_LSB_OUT(v)      \
        out_dword(HWIO_IC_RGSTR_TIMER_BCK_PRS_LSB_ADDR,v)
#define HWIO_IC_RGSTR_TIMER_BCK_PRS_LSB_OUTM(m,v) \
        out_dword_masked_ns(HWIO_IC_RGSTR_TIMER_BCK_PRS_LSB_ADDR,m,v,HWIO_IC_RGSTR_TIMER_BCK_PRS_LSB_IN)
#define HWIO_IC_RGSTR_TIMER_BCK_PRS_LSB_TLV_INT_BMSK                               0x3f000000
#define HWIO_IC_RGSTR_TIMER_BCK_PRS_LSB_TLV_INT_SHFT                                     0x18
#define HWIO_IC_RGSTR_TIMER_BCK_PRS_LSB_CSR_INT_BMSK                                 0xfc0000
#define HWIO_IC_RGSTR_TIMER_BCK_PRS_LSB_CSR_INT_SHFT                                     0x12
#define HWIO_IC_RGSTR_TIMER_BCK_PRS_LSB_INT_END_INT_BMSK                               0x1000
#define HWIO_IC_RGSTR_TIMER_BCK_PRS_LSB_INT_END_INT_SHFT                                  0xc
#define HWIO_IC_RGSTR_TIMER_BCK_PRS_LSB_EV_ENG_INT_BMSK                                  0x40
#define HWIO_IC_RGSTR_TIMER_BCK_PRS_LSB_EV_ENG_INT_SHFT                                   0x6
#define HWIO_IC_RGSTR_TIMER_BCK_PRS_LSB_REE_INT_BMSK                                      0x7
#define HWIO_IC_RGSTR_TIMER_BCK_PRS_LSB_REE_INT_SHFT                                      0x0

#define HWIO_IC_RGSTR_TIMER_BCK_PRS_MSB_ADDR                                       (GSI_REG_BASE      + 0x000000dc)
#define HWIO_IC_RGSTR_TIMER_BCK_PRS_MSB_PHYS                                       (GSI_REG_BASE_PHYS + 0x000000dc)
#define HWIO_IC_RGSTR_TIMER_BCK_PRS_MSB_OFFS                                       (GSI_REG_BASE_OFFS + 0x000000dc)
#define HWIO_IC_RGSTR_TIMER_BCK_PRS_MSB_RMSK                                         0xfc3041
#define HWIO_IC_RGSTR_TIMER_BCK_PRS_MSB_ATTR                                              0x3
#define HWIO_IC_RGSTR_TIMER_BCK_PRS_MSB_IN          \
        in_dword_masked(HWIO_IC_RGSTR_TIMER_BCK_PRS_MSB_ADDR, HWIO_IC_RGSTR_TIMER_BCK_PRS_MSB_RMSK)
#define HWIO_IC_RGSTR_TIMER_BCK_PRS_MSB_INM(m)      \
        in_dword_masked(HWIO_IC_RGSTR_TIMER_BCK_PRS_MSB_ADDR, m)
#define HWIO_IC_RGSTR_TIMER_BCK_PRS_MSB_OUT(v)      \
        out_dword(HWIO_IC_RGSTR_TIMER_BCK_PRS_MSB_ADDR,v)
#define HWIO_IC_RGSTR_TIMER_BCK_PRS_MSB_OUTM(m,v) \
        out_dword_masked_ns(HWIO_IC_RGSTR_TIMER_BCK_PRS_MSB_ADDR,m,v,HWIO_IC_RGSTR_TIMER_BCK_PRS_MSB_IN)
#define HWIO_IC_RGSTR_TIMER_BCK_PRS_MSB_UCONTROLLER_INT_BMSK                         0xfc0000
#define HWIO_IC_RGSTR_TIMER_BCK_PRS_MSB_UCONTROLLER_INT_SHFT                             0x12
#define HWIO_IC_RGSTR_TIMER_BCK_PRS_MSB_RD_WR_INT_BMSK                                 0x3000
#define HWIO_IC_RGSTR_TIMER_BCK_PRS_MSB_RD_WR_INT_SHFT                                    0xc
#define HWIO_IC_RGSTR_TIMER_BCK_PRS_MSB_DB_ENG_INT_BMSK                                  0x40
#define HWIO_IC_RGSTR_TIMER_BCK_PRS_MSB_DB_ENG_INT_SHFT                                   0x6
#define HWIO_IC_RGSTR_TIMER_BCK_PRS_MSB_TIMER_INT_BMSK                                    0x1
#define HWIO_IC_RGSTR_TIMER_BCK_PRS_MSB_TIMER_INT_SHFT                                    0x0

#define HWIO_IC_READ_BCK_PRS_LSB_ADDR                                              (GSI_REG_BASE      + 0x000000e0)
#define HWIO_IC_READ_BCK_PRS_LSB_PHYS                                              (GSI_REG_BASE_PHYS + 0x000000e0)
#define HWIO_IC_READ_BCK_PRS_LSB_OFFS                                              (GSI_REG_BASE_OFFS + 0x000000e0)
#define HWIO_IC_READ_BCK_PRS_LSB_RMSK                                              0x3ffc1047
#define HWIO_IC_READ_BCK_PRS_LSB_ATTR                                                     0x3
#define HWIO_IC_READ_BCK_PRS_LSB_IN          \
        in_dword_masked(HWIO_IC_READ_BCK_PRS_LSB_ADDR, HWIO_IC_READ_BCK_PRS_LSB_RMSK)
#define HWIO_IC_READ_BCK_PRS_LSB_INM(m)      \
        in_dword_masked(HWIO_IC_READ_BCK_PRS_LSB_ADDR, m)
#define HWIO_IC_READ_BCK_PRS_LSB_OUT(v)      \
        out_dword(HWIO_IC_READ_BCK_PRS_LSB_ADDR,v)
#define HWIO_IC_READ_BCK_PRS_LSB_OUTM(m,v) \
        out_dword_masked_ns(HWIO_IC_READ_BCK_PRS_LSB_ADDR,m,v,HWIO_IC_READ_BCK_PRS_LSB_IN)
#define HWIO_IC_READ_BCK_PRS_LSB_TLV_INT_BMSK                                      0x3f000000
#define HWIO_IC_READ_BCK_PRS_LSB_TLV_INT_SHFT                                            0x18
#define HWIO_IC_READ_BCK_PRS_LSB_CSR_INT_BMSK                                        0xfc0000
#define HWIO_IC_READ_BCK_PRS_LSB_CSR_INT_SHFT                                            0x12
#define HWIO_IC_READ_BCK_PRS_LSB_INT_END_INT_BMSK                                      0x1000
#define HWIO_IC_READ_BCK_PRS_LSB_INT_END_INT_SHFT                                         0xc
#define HWIO_IC_READ_BCK_PRS_LSB_EV_ENG_INT_BMSK                                         0x40
#define HWIO_IC_READ_BCK_PRS_LSB_EV_ENG_INT_SHFT                                          0x6
#define HWIO_IC_READ_BCK_PRS_LSB_REE_INT_BMSK                                             0x7
#define HWIO_IC_READ_BCK_PRS_LSB_REE_INT_SHFT                                             0x0

#define HWIO_IC_READ_BCK_PRS_MSB_ADDR                                              (GSI_REG_BASE      + 0x000000e4)
#define HWIO_IC_READ_BCK_PRS_MSB_PHYS                                              (GSI_REG_BASE_PHYS + 0x000000e4)
#define HWIO_IC_READ_BCK_PRS_MSB_OFFS                                              (GSI_REG_BASE_OFFS + 0x000000e4)
#define HWIO_IC_READ_BCK_PRS_MSB_RMSK                                                0xfc3041
#define HWIO_IC_READ_BCK_PRS_MSB_ATTR                                                     0x3
#define HWIO_IC_READ_BCK_PRS_MSB_IN          \
        in_dword_masked(HWIO_IC_READ_BCK_PRS_MSB_ADDR, HWIO_IC_READ_BCK_PRS_MSB_RMSK)
#define HWIO_IC_READ_BCK_PRS_MSB_INM(m)      \
        in_dword_masked(HWIO_IC_READ_BCK_PRS_MSB_ADDR, m)
#define HWIO_IC_READ_BCK_PRS_MSB_OUT(v)      \
        out_dword(HWIO_IC_READ_BCK_PRS_MSB_ADDR,v)
#define HWIO_IC_READ_BCK_PRS_MSB_OUTM(m,v) \
        out_dword_masked_ns(HWIO_IC_READ_BCK_PRS_MSB_ADDR,m,v,HWIO_IC_READ_BCK_PRS_MSB_IN)
#define HWIO_IC_READ_BCK_PRS_MSB_UCONTROLLER_INT_BMSK                                0xfc0000
#define HWIO_IC_READ_BCK_PRS_MSB_UCONTROLLER_INT_SHFT                                    0x12
#define HWIO_IC_READ_BCK_PRS_MSB_RD_WR_INT_BMSK                                        0x3000
#define HWIO_IC_READ_BCK_PRS_MSB_RD_WR_INT_SHFT                                           0xc
#define HWIO_IC_READ_BCK_PRS_MSB_DB_ENG_INT_BMSK                                         0x40
#define HWIO_IC_READ_BCK_PRS_MSB_DB_ENG_INT_SHFT                                          0x6
#define HWIO_IC_READ_BCK_PRS_MSB_TIMER_INT_BMSK                                           0x1
#define HWIO_IC_READ_BCK_PRS_MSB_TIMER_INT_SHFT                                           0x0

#define HWIO_IC_WRITE_BCK_PRS_LSB_ADDR                                             (GSI_REG_BASE      + 0x000000e8)
#define HWIO_IC_WRITE_BCK_PRS_LSB_PHYS                                             (GSI_REG_BASE_PHYS + 0x000000e8)
#define HWIO_IC_WRITE_BCK_PRS_LSB_OFFS                                             (GSI_REG_BASE_OFFS + 0x000000e8)
#define HWIO_IC_WRITE_BCK_PRS_LSB_RMSK                                             0x3ffc1047
#define HWIO_IC_WRITE_BCK_PRS_LSB_ATTR                                                    0x3
#define HWIO_IC_WRITE_BCK_PRS_LSB_IN          \
        in_dword_masked(HWIO_IC_WRITE_BCK_PRS_LSB_ADDR, HWIO_IC_WRITE_BCK_PRS_LSB_RMSK)
#define HWIO_IC_WRITE_BCK_PRS_LSB_INM(m)      \
        in_dword_masked(HWIO_IC_WRITE_BCK_PRS_LSB_ADDR, m)
#define HWIO_IC_WRITE_BCK_PRS_LSB_OUT(v)      \
        out_dword(HWIO_IC_WRITE_BCK_PRS_LSB_ADDR,v)
#define HWIO_IC_WRITE_BCK_PRS_LSB_OUTM(m,v) \
        out_dword_masked_ns(HWIO_IC_WRITE_BCK_PRS_LSB_ADDR,m,v,HWIO_IC_WRITE_BCK_PRS_LSB_IN)
#define HWIO_IC_WRITE_BCK_PRS_LSB_TLV_INT_BMSK                                     0x3f000000
#define HWIO_IC_WRITE_BCK_PRS_LSB_TLV_INT_SHFT                                           0x18
#define HWIO_IC_WRITE_BCK_PRS_LSB_CSR_INT_BMSK                                       0xfc0000
#define HWIO_IC_WRITE_BCK_PRS_LSB_CSR_INT_SHFT                                           0x12
#define HWIO_IC_WRITE_BCK_PRS_LSB_INT_END_INT_BMSK                                     0x1000
#define HWIO_IC_WRITE_BCK_PRS_LSB_INT_END_INT_SHFT                                        0xc
#define HWIO_IC_WRITE_BCK_PRS_LSB_EV_ENG_INT_BMSK                                        0x40
#define HWIO_IC_WRITE_BCK_PRS_LSB_EV_ENG_INT_SHFT                                         0x6
#define HWIO_IC_WRITE_BCK_PRS_LSB_REE_INT_BMSK                                            0x7
#define HWIO_IC_WRITE_BCK_PRS_LSB_REE_INT_SHFT                                            0x0

#define HWIO_IC_WRITE_BCK_PRS_MSB_ADDR                                             (GSI_REG_BASE      + 0x000000ec)
#define HWIO_IC_WRITE_BCK_PRS_MSB_PHYS                                             (GSI_REG_BASE_PHYS + 0x000000ec)
#define HWIO_IC_WRITE_BCK_PRS_MSB_OFFS                                             (GSI_REG_BASE_OFFS + 0x000000ec)
#define HWIO_IC_WRITE_BCK_PRS_MSB_RMSK                                               0xfc3041
#define HWIO_IC_WRITE_BCK_PRS_MSB_ATTR                                                    0x3
#define HWIO_IC_WRITE_BCK_PRS_MSB_IN          \
        in_dword_masked(HWIO_IC_WRITE_BCK_PRS_MSB_ADDR, HWIO_IC_WRITE_BCK_PRS_MSB_RMSK)
#define HWIO_IC_WRITE_BCK_PRS_MSB_INM(m)      \
        in_dword_masked(HWIO_IC_WRITE_BCK_PRS_MSB_ADDR, m)
#define HWIO_IC_WRITE_BCK_PRS_MSB_OUT(v)      \
        out_dword(HWIO_IC_WRITE_BCK_PRS_MSB_ADDR,v)
#define HWIO_IC_WRITE_BCK_PRS_MSB_OUTM(m,v) \
        out_dword_masked_ns(HWIO_IC_WRITE_BCK_PRS_MSB_ADDR,m,v,HWIO_IC_WRITE_BCK_PRS_MSB_IN)
#define HWIO_IC_WRITE_BCK_PRS_MSB_UCONTROLLER_INT_BMSK                               0xfc0000
#define HWIO_IC_WRITE_BCK_PRS_MSB_UCONTROLLER_INT_SHFT                                   0x12
#define HWIO_IC_WRITE_BCK_PRS_MSB_RD_WR_INT_BMSK                                       0x3000
#define HWIO_IC_WRITE_BCK_PRS_MSB_RD_WR_INT_SHFT                                          0xc
#define HWIO_IC_WRITE_BCK_PRS_MSB_DB_ENG_INT_BMSK                                        0x40
#define HWIO_IC_WRITE_BCK_PRS_MSB_DB_ENG_INT_SHFT                                         0x6
#define HWIO_IC_WRITE_BCK_PRS_MSB_TIMER_INT_BMSK                                          0x1
#define HWIO_IC_WRITE_BCK_PRS_MSB_TIMER_INT_SHFT                                          0x0

#define HWIO_IC_UCONTROLLER_GPR_BCK_PRS_LSB_ADDR                                   (GSI_REG_BASE      + 0x000000f0)
#define HWIO_IC_UCONTROLLER_GPR_BCK_PRS_LSB_PHYS                                   (GSI_REG_BASE_PHYS + 0x000000f0)
#define HWIO_IC_UCONTROLLER_GPR_BCK_PRS_LSB_OFFS                                   (GSI_REG_BASE_OFFS + 0x000000f0)
#define HWIO_IC_UCONTROLLER_GPR_BCK_PRS_LSB_RMSK                                   0x3ffc1047
#define HWIO_IC_UCONTROLLER_GPR_BCK_PRS_LSB_ATTR                                          0x3
#define HWIO_IC_UCONTROLLER_GPR_BCK_PRS_LSB_IN          \
        in_dword_masked(HWIO_IC_UCONTROLLER_GPR_BCK_PRS_LSB_ADDR, HWIO_IC_UCONTROLLER_GPR_BCK_PRS_LSB_RMSK)
#define HWIO_IC_UCONTROLLER_GPR_BCK_PRS_LSB_INM(m)      \
        in_dword_masked(HWIO_IC_UCONTROLLER_GPR_BCK_PRS_LSB_ADDR, m)
#define HWIO_IC_UCONTROLLER_GPR_BCK_PRS_LSB_OUT(v)      \
        out_dword(HWIO_IC_UCONTROLLER_GPR_BCK_PRS_LSB_ADDR,v)
#define HWIO_IC_UCONTROLLER_GPR_BCK_PRS_LSB_OUTM(m,v) \
        out_dword_masked_ns(HWIO_IC_UCONTROLLER_GPR_BCK_PRS_LSB_ADDR,m,v,HWIO_IC_UCONTROLLER_GPR_BCK_PRS_LSB_IN)
#define HWIO_IC_UCONTROLLER_GPR_BCK_PRS_LSB_TLV_INT_BMSK                           0x3f000000
#define HWIO_IC_UCONTROLLER_GPR_BCK_PRS_LSB_TLV_INT_SHFT                                 0x18
#define HWIO_IC_UCONTROLLER_GPR_BCK_PRS_LSB_CSR_INT_BMSK                             0xfc0000
#define HWIO_IC_UCONTROLLER_GPR_BCK_PRS_LSB_CSR_INT_SHFT                                 0x12
#define HWIO_IC_UCONTROLLER_GPR_BCK_PRS_LSB_INT_END_INT_BMSK                           0x1000
#define HWIO_IC_UCONTROLLER_GPR_BCK_PRS_LSB_INT_END_INT_SHFT                              0xc
#define HWIO_IC_UCONTROLLER_GPR_BCK_PRS_LSB_EV_ENG_INT_BMSK                              0x40
#define HWIO_IC_UCONTROLLER_GPR_BCK_PRS_LSB_EV_ENG_INT_SHFT                               0x6
#define HWIO_IC_UCONTROLLER_GPR_BCK_PRS_LSB_REE_INT_BMSK                                  0x7
#define HWIO_IC_UCONTROLLER_GPR_BCK_PRS_LSB_REE_INT_SHFT                                  0x0

#define HWIO_IC_UCONTROLLER_GPR_BCK_PRS_MSB_ADDR                                   (GSI_REG_BASE      + 0x000000f4)
#define HWIO_IC_UCONTROLLER_GPR_BCK_PRS_MSB_PHYS                                   (GSI_REG_BASE_PHYS + 0x000000f4)
#define HWIO_IC_UCONTROLLER_GPR_BCK_PRS_MSB_OFFS                                   (GSI_REG_BASE_OFFS + 0x000000f4)
#define HWIO_IC_UCONTROLLER_GPR_BCK_PRS_MSB_RMSK                                     0xfc3041
#define HWIO_IC_UCONTROLLER_GPR_BCK_PRS_MSB_ATTR                                          0x3
#define HWIO_IC_UCONTROLLER_GPR_BCK_PRS_MSB_IN          \
        in_dword_masked(HWIO_IC_UCONTROLLER_GPR_BCK_PRS_MSB_ADDR, HWIO_IC_UCONTROLLER_GPR_BCK_PRS_MSB_RMSK)
#define HWIO_IC_UCONTROLLER_GPR_BCK_PRS_MSB_INM(m)      \
        in_dword_masked(HWIO_IC_UCONTROLLER_GPR_BCK_PRS_MSB_ADDR, m)
#define HWIO_IC_UCONTROLLER_GPR_BCK_PRS_MSB_OUT(v)      \
        out_dword(HWIO_IC_UCONTROLLER_GPR_BCK_PRS_MSB_ADDR,v)
#define HWIO_IC_UCONTROLLER_GPR_BCK_PRS_MSB_OUTM(m,v) \
        out_dword_masked_ns(HWIO_IC_UCONTROLLER_GPR_BCK_PRS_MSB_ADDR,m,v,HWIO_IC_UCONTROLLER_GPR_BCK_PRS_MSB_IN)
#define HWIO_IC_UCONTROLLER_GPR_BCK_PRS_MSB_UCONTROLLER_INT_BMSK                     0xfc0000
#define HWIO_IC_UCONTROLLER_GPR_BCK_PRS_MSB_UCONTROLLER_INT_SHFT                         0x12
#define HWIO_IC_UCONTROLLER_GPR_BCK_PRS_MSB_RD_WR_INT_BMSK                             0x3000
#define HWIO_IC_UCONTROLLER_GPR_BCK_PRS_MSB_RD_WR_INT_SHFT                                0xc
#define HWIO_IC_UCONTROLLER_GPR_BCK_PRS_MSB_DB_ENG_INT_BMSK                              0x40
#define HWIO_IC_UCONTROLLER_GPR_BCK_PRS_MSB_DB_ENG_INT_SHFT                               0x6
#define HWIO_IC_UCONTROLLER_GPR_BCK_PRS_MSB_TIMER_INT_BMSK                                0x1
#define HWIO_IC_UCONTROLLER_GPR_BCK_PRS_MSB_TIMER_INT_SHFT                                0x0

#define HWIO_IC_INT_WEIGHT_REE_ADDR                                                (GSI_REG_BASE      + 0x00000100)
#define HWIO_IC_INT_WEIGHT_REE_PHYS                                                (GSI_REG_BASE_PHYS + 0x00000100)
#define HWIO_IC_INT_WEIGHT_REE_OFFS                                                (GSI_REG_BASE_OFFS + 0x00000100)
#define HWIO_IC_INT_WEIGHT_REE_RMSK                                                     0xfff
#define HWIO_IC_INT_WEIGHT_REE_ATTR                                                       0x3
#define HWIO_IC_INT_WEIGHT_REE_IN          \
        in_dword_masked(HWIO_IC_INT_WEIGHT_REE_ADDR, HWIO_IC_INT_WEIGHT_REE_RMSK)
#define HWIO_IC_INT_WEIGHT_REE_INM(m)      \
        in_dword_masked(HWIO_IC_INT_WEIGHT_REE_ADDR, m)
#define HWIO_IC_INT_WEIGHT_REE_OUT(v)      \
        out_dword(HWIO_IC_INT_WEIGHT_REE_ADDR,v)
#define HWIO_IC_INT_WEIGHT_REE_OUTM(m,v) \
        out_dword_masked_ns(HWIO_IC_INT_WEIGHT_REE_ADDR,m,v,HWIO_IC_INT_WEIGHT_REE_IN)
#define HWIO_IC_INT_WEIGHT_REE_CH_EMPTY_INT_WEIGHT_BMSK                                 0xf00
#define HWIO_IC_INT_WEIGHT_REE_CH_EMPTY_INT_WEIGHT_SHFT                                   0x8
#define HWIO_IC_INT_WEIGHT_REE_NEW_RE_INT_WEIGHT_BMSK                                    0xf0
#define HWIO_IC_INT_WEIGHT_REE_NEW_RE_INT_WEIGHT_SHFT                                     0x4
#define HWIO_IC_INT_WEIGHT_REE_STOP_CH_COMP_INT_WEIGHT_BMSK                               0xf
#define HWIO_IC_INT_WEIGHT_REE_STOP_CH_COMP_INT_WEIGHT_SHFT                               0x0

#define HWIO_IC_INT_WEIGHT_EVT_ENG_ADDR                                            (GSI_REG_BASE      + 0x00000104)
#define HWIO_IC_INT_WEIGHT_EVT_ENG_PHYS                                            (GSI_REG_BASE_PHYS + 0x00000104)
#define HWIO_IC_INT_WEIGHT_EVT_ENG_OFFS                                            (GSI_REG_BASE_OFFS + 0x00000104)
#define HWIO_IC_INT_WEIGHT_EVT_ENG_RMSK                                                   0xf
#define HWIO_IC_INT_WEIGHT_EVT_ENG_ATTR                                                   0x3
#define HWIO_IC_INT_WEIGHT_EVT_ENG_IN          \
        in_dword_masked(HWIO_IC_INT_WEIGHT_EVT_ENG_ADDR, HWIO_IC_INT_WEIGHT_EVT_ENG_RMSK)
#define HWIO_IC_INT_WEIGHT_EVT_ENG_INM(m)      \
        in_dword_masked(HWIO_IC_INT_WEIGHT_EVT_ENG_ADDR, m)
#define HWIO_IC_INT_WEIGHT_EVT_ENG_OUT(v)      \
        out_dword(HWIO_IC_INT_WEIGHT_EVT_ENG_ADDR,v)
#define HWIO_IC_INT_WEIGHT_EVT_ENG_OUTM(m,v) \
        out_dword_masked_ns(HWIO_IC_INT_WEIGHT_EVT_ENG_ADDR,m,v,HWIO_IC_INT_WEIGHT_EVT_ENG_IN)
#define HWIO_IC_INT_WEIGHT_EVT_ENG_EVNT_ENG_INT_WEIGHT_BMSK                               0xf
#define HWIO_IC_INT_WEIGHT_EVT_ENG_EVNT_ENG_INT_WEIGHT_SHFT                               0x0

#define HWIO_IC_INT_WEIGHT_INT_ENG_ADDR                                            (GSI_REG_BASE      + 0x00000108)
#define HWIO_IC_INT_WEIGHT_INT_ENG_PHYS                                            (GSI_REG_BASE_PHYS + 0x00000108)
#define HWIO_IC_INT_WEIGHT_INT_ENG_OFFS                                            (GSI_REG_BASE_OFFS + 0x00000108)
#define HWIO_IC_INT_WEIGHT_INT_ENG_RMSK                                                   0xf
#define HWIO_IC_INT_WEIGHT_INT_ENG_ATTR                                                   0x3
#define HWIO_IC_INT_WEIGHT_INT_ENG_IN          \
        in_dword_masked(HWIO_IC_INT_WEIGHT_INT_ENG_ADDR, HWIO_IC_INT_WEIGHT_INT_ENG_RMSK)
#define HWIO_IC_INT_WEIGHT_INT_ENG_INM(m)      \
        in_dword_masked(HWIO_IC_INT_WEIGHT_INT_ENG_ADDR, m)
#define HWIO_IC_INT_WEIGHT_INT_ENG_OUT(v)      \
        out_dword(HWIO_IC_INT_WEIGHT_INT_ENG_ADDR,v)
#define HWIO_IC_INT_WEIGHT_INT_ENG_OUTM(m,v) \
        out_dword_masked_ns(HWIO_IC_INT_WEIGHT_INT_ENG_ADDR,m,v,HWIO_IC_INT_WEIGHT_INT_ENG_IN)
#define HWIO_IC_INT_WEIGHT_INT_ENG_INT_ENG_INT_WEIGHT_BMSK                                0xf
#define HWIO_IC_INT_WEIGHT_INT_ENG_INT_ENG_INT_WEIGHT_SHFT                                0x0

#define HWIO_IC_INT_WEIGHT_CSR_ADDR                                                (GSI_REG_BASE      + 0x0000010c)
#define HWIO_IC_INT_WEIGHT_CSR_PHYS                                                (GSI_REG_BASE_PHYS + 0x0000010c)
#define HWIO_IC_INT_WEIGHT_CSR_OFFS                                                (GSI_REG_BASE_OFFS + 0x0000010c)
#define HWIO_IC_INT_WEIGHT_CSR_RMSK                                                      0xff
#define HWIO_IC_INT_WEIGHT_CSR_ATTR                                                       0x3
#define HWIO_IC_INT_WEIGHT_CSR_IN          \
        in_dword_masked(HWIO_IC_INT_WEIGHT_CSR_ADDR, HWIO_IC_INT_WEIGHT_CSR_RMSK)
#define HWIO_IC_INT_WEIGHT_CSR_INM(m)      \
        in_dword_masked(HWIO_IC_INT_WEIGHT_CSR_ADDR, m)
#define HWIO_IC_INT_WEIGHT_CSR_OUT(v)      \
        out_dword(HWIO_IC_INT_WEIGHT_CSR_ADDR,v)
#define HWIO_IC_INT_WEIGHT_CSR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_IC_INT_WEIGHT_CSR_ADDR,m,v,HWIO_IC_INT_WEIGHT_CSR_IN)
#define HWIO_IC_INT_WEIGHT_CSR_EE_GENERIC_INT_WEIGHT_BMSK                                0xf0
#define HWIO_IC_INT_WEIGHT_CSR_EE_GENERIC_INT_WEIGHT_SHFT                                 0x4
#define HWIO_IC_INT_WEIGHT_CSR_CH_CMD_INT_WEIGHT_BMSK                                     0xf
#define HWIO_IC_INT_WEIGHT_CSR_CH_CMD_INT_WEIGHT_SHFT                                     0x0

#define HWIO_IC_INT_WEIGHT_TLV_ENG_ADDR                                            (GSI_REG_BASE      + 0x00000110)
#define HWIO_IC_INT_WEIGHT_TLV_ENG_PHYS                                            (GSI_REG_BASE_PHYS + 0x00000110)
#define HWIO_IC_INT_WEIGHT_TLV_ENG_OFFS                                            (GSI_REG_BASE_OFFS + 0x00000110)
#define HWIO_IC_INT_WEIGHT_TLV_ENG_RMSK                                                 0xfff
#define HWIO_IC_INT_WEIGHT_TLV_ENG_ATTR                                                   0x3
#define HWIO_IC_INT_WEIGHT_TLV_ENG_IN          \
        in_dword_masked(HWIO_IC_INT_WEIGHT_TLV_ENG_ADDR, HWIO_IC_INT_WEIGHT_TLV_ENG_RMSK)
#define HWIO_IC_INT_WEIGHT_TLV_ENG_INM(m)      \
        in_dword_masked(HWIO_IC_INT_WEIGHT_TLV_ENG_ADDR, m)
#define HWIO_IC_INT_WEIGHT_TLV_ENG_OUT(v)      \
        out_dword(HWIO_IC_INT_WEIGHT_TLV_ENG_ADDR,v)
#define HWIO_IC_INT_WEIGHT_TLV_ENG_OUTM(m,v) \
        out_dword_masked_ns(HWIO_IC_INT_WEIGHT_TLV_ENG_ADDR,m,v,HWIO_IC_INT_WEIGHT_TLV_ENG_IN)
#define HWIO_IC_INT_WEIGHT_TLV_ENG_TLV_2_INT_WEIGHT_BMSK                                0xf00
#define HWIO_IC_INT_WEIGHT_TLV_ENG_TLV_2_INT_WEIGHT_SHFT                                  0x8
#define HWIO_IC_INT_WEIGHT_TLV_ENG_TLV_1_INT_WEIGHT_BMSK                                 0xf0
#define HWIO_IC_INT_WEIGHT_TLV_ENG_TLV_1_INT_WEIGHT_SHFT                                  0x4
#define HWIO_IC_INT_WEIGHT_TLV_ENG_TLV_0_INT_WEIGHT_BMSK                                  0xf
#define HWIO_IC_INT_WEIGHT_TLV_ENG_TLV_0_INT_WEIGHT_SHFT                                  0x0

#define HWIO_IC_INT_WEIGHT_TIMER_ENG_ADDR                                          (GSI_REG_BASE      + 0x00000114)
#define HWIO_IC_INT_WEIGHT_TIMER_ENG_PHYS                                          (GSI_REG_BASE_PHYS + 0x00000114)
#define HWIO_IC_INT_WEIGHT_TIMER_ENG_OFFS                                          (GSI_REG_BASE_OFFS + 0x00000114)
#define HWIO_IC_INT_WEIGHT_TIMER_ENG_RMSK                                                 0xf
#define HWIO_IC_INT_WEIGHT_TIMER_ENG_ATTR                                                 0x3
#define HWIO_IC_INT_WEIGHT_TIMER_ENG_IN          \
        in_dword_masked(HWIO_IC_INT_WEIGHT_TIMER_ENG_ADDR, HWIO_IC_INT_WEIGHT_TIMER_ENG_RMSK)
#define HWIO_IC_INT_WEIGHT_TIMER_ENG_INM(m)      \
        in_dword_masked(HWIO_IC_INT_WEIGHT_TIMER_ENG_ADDR, m)
#define HWIO_IC_INT_WEIGHT_TIMER_ENG_OUT(v)      \
        out_dword(HWIO_IC_INT_WEIGHT_TIMER_ENG_ADDR,v)
#define HWIO_IC_INT_WEIGHT_TIMER_ENG_OUTM(m,v) \
        out_dword_masked_ns(HWIO_IC_INT_WEIGHT_TIMER_ENG_ADDR,m,v,HWIO_IC_INT_WEIGHT_TIMER_ENG_IN)
#define HWIO_IC_INT_WEIGHT_TIMER_ENG_TIMER_INT_WEIGHT_BMSK                                0xf
#define HWIO_IC_INT_WEIGHT_TIMER_ENG_TIMER_INT_WEIGHT_SHFT                                0x0

#define HWIO_IC_INT_WEIGHT_DB_ENG_ADDR                                             (GSI_REG_BASE      + 0x00000118)
#define HWIO_IC_INT_WEIGHT_DB_ENG_PHYS                                             (GSI_REG_BASE_PHYS + 0x00000118)
#define HWIO_IC_INT_WEIGHT_DB_ENG_OFFS                                             (GSI_REG_BASE_OFFS + 0x00000118)
#define HWIO_IC_INT_WEIGHT_DB_ENG_RMSK                                                    0xf
#define HWIO_IC_INT_WEIGHT_DB_ENG_ATTR                                                    0x3
#define HWIO_IC_INT_WEIGHT_DB_ENG_IN          \
        in_dword_masked(HWIO_IC_INT_WEIGHT_DB_ENG_ADDR, HWIO_IC_INT_WEIGHT_DB_ENG_RMSK)
#define HWIO_IC_INT_WEIGHT_DB_ENG_INM(m)      \
        in_dword_masked(HWIO_IC_INT_WEIGHT_DB_ENG_ADDR, m)
#define HWIO_IC_INT_WEIGHT_DB_ENG_OUT(v)      \
        out_dword(HWIO_IC_INT_WEIGHT_DB_ENG_ADDR,v)
#define HWIO_IC_INT_WEIGHT_DB_ENG_OUTM(m,v) \
        out_dword_masked_ns(HWIO_IC_INT_WEIGHT_DB_ENG_ADDR,m,v,HWIO_IC_INT_WEIGHT_DB_ENG_IN)
#define HWIO_IC_INT_WEIGHT_DB_ENG_NEW_DB_INT_WEIGHT_BMSK                                  0xf
#define HWIO_IC_INT_WEIGHT_DB_ENG_NEW_DB_INT_WEIGHT_SHFT                                  0x0

#define HWIO_IC_INT_WEIGHT_RD_WR_ENG_ADDR                                          (GSI_REG_BASE      + 0x0000011c)
#define HWIO_IC_INT_WEIGHT_RD_WR_ENG_PHYS                                          (GSI_REG_BASE_PHYS + 0x0000011c)
#define HWIO_IC_INT_WEIGHT_RD_WR_ENG_OFFS                                          (GSI_REG_BASE_OFFS + 0x0000011c)
#define HWIO_IC_INT_WEIGHT_RD_WR_ENG_RMSK                                                0xff
#define HWIO_IC_INT_WEIGHT_RD_WR_ENG_ATTR                                                 0x3
#define HWIO_IC_INT_WEIGHT_RD_WR_ENG_IN          \
        in_dword_masked(HWIO_IC_INT_WEIGHT_RD_WR_ENG_ADDR, HWIO_IC_INT_WEIGHT_RD_WR_ENG_RMSK)
#define HWIO_IC_INT_WEIGHT_RD_WR_ENG_INM(m)      \
        in_dword_masked(HWIO_IC_INT_WEIGHT_RD_WR_ENG_ADDR, m)
#define HWIO_IC_INT_WEIGHT_RD_WR_ENG_OUT(v)      \
        out_dword(HWIO_IC_INT_WEIGHT_RD_WR_ENG_ADDR,v)
#define HWIO_IC_INT_WEIGHT_RD_WR_ENG_OUTM(m,v) \
        out_dword_masked_ns(HWIO_IC_INT_WEIGHT_RD_WR_ENG_ADDR,m,v,HWIO_IC_INT_WEIGHT_RD_WR_ENG_IN)
#define HWIO_IC_INT_WEIGHT_RD_WR_ENG_WRITE_INT_WEIGHT_BMSK                               0xf0
#define HWIO_IC_INT_WEIGHT_RD_WR_ENG_WRITE_INT_WEIGHT_SHFT                                0x4
#define HWIO_IC_INT_WEIGHT_RD_WR_ENG_READ_INT_WEIGHT_BMSK                                 0xf
#define HWIO_IC_INT_WEIGHT_RD_WR_ENG_READ_INT_WEIGHT_SHFT                                 0x0

#define HWIO_IC_INT_WEIGHT_UCONTROLLER_ENG_ADDR                                    (GSI_REG_BASE      + 0x00000120)
#define HWIO_IC_INT_WEIGHT_UCONTROLLER_ENG_PHYS                                    (GSI_REG_BASE_PHYS + 0x00000120)
#define HWIO_IC_INT_WEIGHT_UCONTROLLER_ENG_OFFS                                    (GSI_REG_BASE_OFFS + 0x00000120)
#define HWIO_IC_INT_WEIGHT_UCONTROLLER_ENG_RMSK                                           0xf
#define HWIO_IC_INT_WEIGHT_UCONTROLLER_ENG_ATTR                                           0x3
#define HWIO_IC_INT_WEIGHT_UCONTROLLER_ENG_IN          \
        in_dword_masked(HWIO_IC_INT_WEIGHT_UCONTROLLER_ENG_ADDR, HWIO_IC_INT_WEIGHT_UCONTROLLER_ENG_RMSK)
#define HWIO_IC_INT_WEIGHT_UCONTROLLER_ENG_INM(m)      \
        in_dword_masked(HWIO_IC_INT_WEIGHT_UCONTROLLER_ENG_ADDR, m)
#define HWIO_IC_INT_WEIGHT_UCONTROLLER_ENG_OUT(v)      \
        out_dword(HWIO_IC_INT_WEIGHT_UCONTROLLER_ENG_ADDR,v)
#define HWIO_IC_INT_WEIGHT_UCONTROLLER_ENG_OUTM(m,v) \
        out_dword_masked_ns(HWIO_IC_INT_WEIGHT_UCONTROLLER_ENG_ADDR,m,v,HWIO_IC_INT_WEIGHT_UCONTROLLER_ENG_IN)
#define HWIO_IC_INT_WEIGHT_UCONTROLLER_ENG_UCONTROLLER_GP_INT_WEIGHT_BMSK                 0xf
#define HWIO_IC_INT_WEIGHT_UCONTROLLER_ENG_UCONTROLLER_GP_INT_WEIGHT_SHFT                 0x0

#define HWIO_GSI_MANAGER_EE_QOS_n_ADDR(n)                                          (GSI_REG_BASE      + 0x00000300 + 0x4 * (n))
#define HWIO_GSI_MANAGER_EE_QOS_n_PHYS(n)                                          (GSI_REG_BASE_PHYS + 0x00000300 + 0x4 * (n))
#define HWIO_GSI_MANAGER_EE_QOS_n_OFFS(n)                                          (GSI_REG_BASE_OFFS + 0x00000300 + 0x4 * (n))
#define HWIO_GSI_MANAGER_EE_QOS_n_RMSK                                               0x1f1f03
#define HWIO_GSI_MANAGER_EE_QOS_n_MAXn                                                      2
#define HWIO_GSI_MANAGER_EE_QOS_n_INI(n)        \
        in_dword_masked(HWIO_GSI_MANAGER_EE_QOS_n_ADDR(n), HWIO_GSI_MANAGER_EE_QOS_n_RMSK)
#define HWIO_GSI_MANAGER_EE_QOS_n_INMI(n,mask)    \
        in_dword_masked(HWIO_GSI_MANAGER_EE_QOS_n_ADDR(n), mask)
#define HWIO_GSI_MANAGER_EE_QOS_n_OUTI(n,val)    \
        out_dword(HWIO_GSI_MANAGER_EE_QOS_n_ADDR(n),val)
#define HWIO_GSI_MANAGER_EE_QOS_n_OUTMI(n,mask,val) \
        out_dword_masked_ns(HWIO_GSI_MANAGER_EE_QOS_n_ADDR(n),mask,val,HWIO_GSI_MANAGER_EE_QOS_n_INI(n))
#define HWIO_GSI_MANAGER_EE_QOS_n_MAX_EV_ALLOC_BMSK                                  0x1f0000
#define HWIO_GSI_MANAGER_EE_QOS_n_MAX_EV_ALLOC_SHFT                                      0x10
#define HWIO_GSI_MANAGER_EE_QOS_n_MAX_CH_ALLOC_BMSK                                    0x1f00
#define HWIO_GSI_MANAGER_EE_QOS_n_MAX_CH_ALLOC_SHFT                                       0x8
#define HWIO_GSI_MANAGER_EE_QOS_n_EE_PRIO_BMSK                                            0x3
#define HWIO_GSI_MANAGER_EE_QOS_n_EE_PRIO_SHFT                                            0x0

#define HWIO_GSI_SHRAM_PTR_CH_CNTXT_BASE_ADDR_ADDR                                 (GSI_REG_BASE      + 0x00000200)
#define HWIO_GSI_SHRAM_PTR_CH_CNTXT_BASE_ADDR_PHYS                                 (GSI_REG_BASE_PHYS + 0x00000200)
#define HWIO_GSI_SHRAM_PTR_CH_CNTXT_BASE_ADDR_OFFS                                 (GSI_REG_BASE_OFFS + 0x00000200)
#define HWIO_GSI_SHRAM_PTR_CH_CNTXT_BASE_ADDR_RMSK                                     0xffff
#define HWIO_GSI_SHRAM_PTR_CH_CNTXT_BASE_ADDR_ATTR                                        0x3
#define HWIO_GSI_SHRAM_PTR_CH_CNTXT_BASE_ADDR_IN          \
        in_dword_masked(HWIO_GSI_SHRAM_PTR_CH_CNTXT_BASE_ADDR_ADDR, HWIO_GSI_SHRAM_PTR_CH_CNTXT_BASE_ADDR_RMSK)
#define HWIO_GSI_SHRAM_PTR_CH_CNTXT_BASE_ADDR_INM(m)      \
        in_dword_masked(HWIO_GSI_SHRAM_PTR_CH_CNTXT_BASE_ADDR_ADDR, m)
#define HWIO_GSI_SHRAM_PTR_CH_CNTXT_BASE_ADDR_OUT(v)      \
        out_dword(HWIO_GSI_SHRAM_PTR_CH_CNTXT_BASE_ADDR_ADDR,v)
#define HWIO_GSI_SHRAM_PTR_CH_CNTXT_BASE_ADDR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GSI_SHRAM_PTR_CH_CNTXT_BASE_ADDR_ADDR,m,v,HWIO_GSI_SHRAM_PTR_CH_CNTXT_BASE_ADDR_IN)
#define HWIO_GSI_SHRAM_PTR_CH_CNTXT_BASE_ADDR_SHRAM_PTR_BMSK                           0xffff
#define HWIO_GSI_SHRAM_PTR_CH_CNTXT_BASE_ADDR_SHRAM_PTR_SHFT                              0x0

#define HWIO_GSI_SHRAM_PTR_EV_CNTXT_BASE_ADDR_ADDR                                 (GSI_REG_BASE      + 0x00000204)
#define HWIO_GSI_SHRAM_PTR_EV_CNTXT_BASE_ADDR_PHYS                                 (GSI_REG_BASE_PHYS + 0x00000204)
#define HWIO_GSI_SHRAM_PTR_EV_CNTXT_BASE_ADDR_OFFS                                 (GSI_REG_BASE_OFFS + 0x00000204)
#define HWIO_GSI_SHRAM_PTR_EV_CNTXT_BASE_ADDR_RMSK                                     0xffff
#define HWIO_GSI_SHRAM_PTR_EV_CNTXT_BASE_ADDR_ATTR                                        0x3
#define HWIO_GSI_SHRAM_PTR_EV_CNTXT_BASE_ADDR_IN          \
        in_dword_masked(HWIO_GSI_SHRAM_PTR_EV_CNTXT_BASE_ADDR_ADDR, HWIO_GSI_SHRAM_PTR_EV_CNTXT_BASE_ADDR_RMSK)
#define HWIO_GSI_SHRAM_PTR_EV_CNTXT_BASE_ADDR_INM(m)      \
        in_dword_masked(HWIO_GSI_SHRAM_PTR_EV_CNTXT_BASE_ADDR_ADDR, m)
#define HWIO_GSI_SHRAM_PTR_EV_CNTXT_BASE_ADDR_OUT(v)      \
        out_dword(HWIO_GSI_SHRAM_PTR_EV_CNTXT_BASE_ADDR_ADDR,v)
#define HWIO_GSI_SHRAM_PTR_EV_CNTXT_BASE_ADDR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GSI_SHRAM_PTR_EV_CNTXT_BASE_ADDR_ADDR,m,v,HWIO_GSI_SHRAM_PTR_EV_CNTXT_BASE_ADDR_IN)
#define HWIO_GSI_SHRAM_PTR_EV_CNTXT_BASE_ADDR_SHRAM_PTR_BMSK                           0xffff
#define HWIO_GSI_SHRAM_PTR_EV_CNTXT_BASE_ADDR_SHRAM_PTR_SHFT                              0x0

#define HWIO_GSI_SHRAM_PTR_RE_STORAGE_BASE_ADDR_ADDR                               (GSI_REG_BASE      + 0x00000208)
#define HWIO_GSI_SHRAM_PTR_RE_STORAGE_BASE_ADDR_PHYS                               (GSI_REG_BASE_PHYS + 0x00000208)
#define HWIO_GSI_SHRAM_PTR_RE_STORAGE_BASE_ADDR_OFFS                               (GSI_REG_BASE_OFFS + 0x00000208)
#define HWIO_GSI_SHRAM_PTR_RE_STORAGE_BASE_ADDR_RMSK                                   0xffff
#define HWIO_GSI_SHRAM_PTR_RE_STORAGE_BASE_ADDR_ATTR                                      0x3
#define HWIO_GSI_SHRAM_PTR_RE_STORAGE_BASE_ADDR_IN          \
        in_dword_masked(HWIO_GSI_SHRAM_PTR_RE_STORAGE_BASE_ADDR_ADDR, HWIO_GSI_SHRAM_PTR_RE_STORAGE_BASE_ADDR_RMSK)
#define HWIO_GSI_SHRAM_PTR_RE_STORAGE_BASE_ADDR_INM(m)      \
        in_dword_masked(HWIO_GSI_SHRAM_PTR_RE_STORAGE_BASE_ADDR_ADDR, m)
#define HWIO_GSI_SHRAM_PTR_RE_STORAGE_BASE_ADDR_OUT(v)      \
        out_dword(HWIO_GSI_SHRAM_PTR_RE_STORAGE_BASE_ADDR_ADDR,v)
#define HWIO_GSI_SHRAM_PTR_RE_STORAGE_BASE_ADDR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GSI_SHRAM_PTR_RE_STORAGE_BASE_ADDR_ADDR,m,v,HWIO_GSI_SHRAM_PTR_RE_STORAGE_BASE_ADDR_IN)
#define HWIO_GSI_SHRAM_PTR_RE_STORAGE_BASE_ADDR_SHRAM_PTR_BMSK                         0xffff
#define HWIO_GSI_SHRAM_PTR_RE_STORAGE_BASE_ADDR_SHRAM_PTR_SHFT                            0x0

#define HWIO_GSI_SHRAM_PTR_RE_ESC_BUF_BASE_ADDR_ADDR                               (GSI_REG_BASE      + 0x0000020c)
#define HWIO_GSI_SHRAM_PTR_RE_ESC_BUF_BASE_ADDR_PHYS                               (GSI_REG_BASE_PHYS + 0x0000020c)
#define HWIO_GSI_SHRAM_PTR_RE_ESC_BUF_BASE_ADDR_OFFS                               (GSI_REG_BASE_OFFS + 0x0000020c)
#define HWIO_GSI_SHRAM_PTR_RE_ESC_BUF_BASE_ADDR_RMSK                                   0xffff
#define HWIO_GSI_SHRAM_PTR_RE_ESC_BUF_BASE_ADDR_ATTR                                      0x3
#define HWIO_GSI_SHRAM_PTR_RE_ESC_BUF_BASE_ADDR_IN          \
        in_dword_masked(HWIO_GSI_SHRAM_PTR_RE_ESC_BUF_BASE_ADDR_ADDR, HWIO_GSI_SHRAM_PTR_RE_ESC_BUF_BASE_ADDR_RMSK)
#define HWIO_GSI_SHRAM_PTR_RE_ESC_BUF_BASE_ADDR_INM(m)      \
        in_dword_masked(HWIO_GSI_SHRAM_PTR_RE_ESC_BUF_BASE_ADDR_ADDR, m)
#define HWIO_GSI_SHRAM_PTR_RE_ESC_BUF_BASE_ADDR_OUT(v)      \
        out_dword(HWIO_GSI_SHRAM_PTR_RE_ESC_BUF_BASE_ADDR_ADDR,v)
#define HWIO_GSI_SHRAM_PTR_RE_ESC_BUF_BASE_ADDR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GSI_SHRAM_PTR_RE_ESC_BUF_BASE_ADDR_ADDR,m,v,HWIO_GSI_SHRAM_PTR_RE_ESC_BUF_BASE_ADDR_IN)
#define HWIO_GSI_SHRAM_PTR_RE_ESC_BUF_BASE_ADDR_SHRAM_PTR_BMSK                         0xffff
#define HWIO_GSI_SHRAM_PTR_RE_ESC_BUF_BASE_ADDR_SHRAM_PTR_SHFT                            0x0

#define HWIO_GSI_SHRAM_PTR_EE_SCRACH_BASE_ADDR_ADDR                                (GSI_REG_BASE      + 0x00000240)
#define HWIO_GSI_SHRAM_PTR_EE_SCRACH_BASE_ADDR_PHYS                                (GSI_REG_BASE_PHYS + 0x00000240)
#define HWIO_GSI_SHRAM_PTR_EE_SCRACH_BASE_ADDR_OFFS                                (GSI_REG_BASE_OFFS + 0x00000240)
#define HWIO_GSI_SHRAM_PTR_EE_SCRACH_BASE_ADDR_RMSK                                    0xffff
#define HWIO_GSI_SHRAM_PTR_EE_SCRACH_BASE_ADDR_ATTR                                       0x3
#define HWIO_GSI_SHRAM_PTR_EE_SCRACH_BASE_ADDR_IN          \
        in_dword_masked(HWIO_GSI_SHRAM_PTR_EE_SCRACH_BASE_ADDR_ADDR, HWIO_GSI_SHRAM_PTR_EE_SCRACH_BASE_ADDR_RMSK)
#define HWIO_GSI_SHRAM_PTR_EE_SCRACH_BASE_ADDR_INM(m)      \
        in_dword_masked(HWIO_GSI_SHRAM_PTR_EE_SCRACH_BASE_ADDR_ADDR, m)
#define HWIO_GSI_SHRAM_PTR_EE_SCRACH_BASE_ADDR_OUT(v)      \
        out_dword(HWIO_GSI_SHRAM_PTR_EE_SCRACH_BASE_ADDR_ADDR,v)
#define HWIO_GSI_SHRAM_PTR_EE_SCRACH_BASE_ADDR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GSI_SHRAM_PTR_EE_SCRACH_BASE_ADDR_ADDR,m,v,HWIO_GSI_SHRAM_PTR_EE_SCRACH_BASE_ADDR_IN)
#define HWIO_GSI_SHRAM_PTR_EE_SCRACH_BASE_ADDR_SHRAM_PTR_BMSK                          0xffff
#define HWIO_GSI_SHRAM_PTR_EE_SCRACH_BASE_ADDR_SHRAM_PTR_SHFT                             0x0

#define HWIO_GSI_SHRAM_PTR_FUNC_STACK_BASE_ADDR_ADDR                               (GSI_REG_BASE      + 0x00000244)
#define HWIO_GSI_SHRAM_PTR_FUNC_STACK_BASE_ADDR_PHYS                               (GSI_REG_BASE_PHYS + 0x00000244)
#define HWIO_GSI_SHRAM_PTR_FUNC_STACK_BASE_ADDR_OFFS                               (GSI_REG_BASE_OFFS + 0x00000244)
#define HWIO_GSI_SHRAM_PTR_FUNC_STACK_BASE_ADDR_RMSK                                   0xffff
#define HWIO_GSI_SHRAM_PTR_FUNC_STACK_BASE_ADDR_ATTR                                      0x3
#define HWIO_GSI_SHRAM_PTR_FUNC_STACK_BASE_ADDR_IN          \
        in_dword_masked(HWIO_GSI_SHRAM_PTR_FUNC_STACK_BASE_ADDR_ADDR, HWIO_GSI_SHRAM_PTR_FUNC_STACK_BASE_ADDR_RMSK)
#define HWIO_GSI_SHRAM_PTR_FUNC_STACK_BASE_ADDR_INM(m)      \
        in_dword_masked(HWIO_GSI_SHRAM_PTR_FUNC_STACK_BASE_ADDR_ADDR, m)
#define HWIO_GSI_SHRAM_PTR_FUNC_STACK_BASE_ADDR_OUT(v)      \
        out_dword(HWIO_GSI_SHRAM_PTR_FUNC_STACK_BASE_ADDR_ADDR,v)
#define HWIO_GSI_SHRAM_PTR_FUNC_STACK_BASE_ADDR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GSI_SHRAM_PTR_FUNC_STACK_BASE_ADDR_ADDR,m,v,HWIO_GSI_SHRAM_PTR_FUNC_STACK_BASE_ADDR_IN)
#define HWIO_GSI_SHRAM_PTR_FUNC_STACK_BASE_ADDR_SHRAM_PTR_BMSK                         0xffff
#define HWIO_GSI_SHRAM_PTR_FUNC_STACK_BASE_ADDR_SHRAM_PTR_SHFT                            0x0

#define HWIO_GSI_SHRAM_PTR_MCS_SCRATCH_BASE_ADDR_ADDR                              (GSI_REG_BASE      + 0x00000248)
#define HWIO_GSI_SHRAM_PTR_MCS_SCRATCH_BASE_ADDR_PHYS                              (GSI_REG_BASE_PHYS + 0x00000248)
#define HWIO_GSI_SHRAM_PTR_MCS_SCRATCH_BASE_ADDR_OFFS                              (GSI_REG_BASE_OFFS + 0x00000248)
#define HWIO_GSI_SHRAM_PTR_MCS_SCRATCH_BASE_ADDR_RMSK                                  0xffff
#define HWIO_GSI_SHRAM_PTR_MCS_SCRATCH_BASE_ADDR_ATTR                                     0x3
#define HWIO_GSI_SHRAM_PTR_MCS_SCRATCH_BASE_ADDR_IN          \
        in_dword_masked(HWIO_GSI_SHRAM_PTR_MCS_SCRATCH_BASE_ADDR_ADDR, HWIO_GSI_SHRAM_PTR_MCS_SCRATCH_BASE_ADDR_RMSK)
#define HWIO_GSI_SHRAM_PTR_MCS_SCRATCH_BASE_ADDR_INM(m)      \
        in_dword_masked(HWIO_GSI_SHRAM_PTR_MCS_SCRATCH_BASE_ADDR_ADDR, m)
#define HWIO_GSI_SHRAM_PTR_MCS_SCRATCH_BASE_ADDR_OUT(v)      \
        out_dword(HWIO_GSI_SHRAM_PTR_MCS_SCRATCH_BASE_ADDR_ADDR,v)
#define HWIO_GSI_SHRAM_PTR_MCS_SCRATCH_BASE_ADDR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GSI_SHRAM_PTR_MCS_SCRATCH_BASE_ADDR_ADDR,m,v,HWIO_GSI_SHRAM_PTR_MCS_SCRATCH_BASE_ADDR_IN)
#define HWIO_GSI_SHRAM_PTR_MCS_SCRATCH_BASE_ADDR_SHRAM_PTR_BMSK                        0xffff
#define HWIO_GSI_SHRAM_PTR_MCS_SCRATCH_BASE_ADDR_SHRAM_PTR_SHFT                           0x0

#define HWIO_GSI_IRAM_PTR_CH_CMD_ADDR                                              (GSI_REG_BASE      + 0x00000400)
#define HWIO_GSI_IRAM_PTR_CH_CMD_PHYS                                              (GSI_REG_BASE_PHYS + 0x00000400)
#define HWIO_GSI_IRAM_PTR_CH_CMD_OFFS                                              (GSI_REG_BASE_OFFS + 0x00000400)
#define HWIO_GSI_IRAM_PTR_CH_CMD_RMSK                                                   0xfff
#define HWIO_GSI_IRAM_PTR_CH_CMD_ATTR                                                     0x3
#define HWIO_GSI_IRAM_PTR_CH_CMD_IN          \
        in_dword_masked(HWIO_GSI_IRAM_PTR_CH_CMD_ADDR, HWIO_GSI_IRAM_PTR_CH_CMD_RMSK)
#define HWIO_GSI_IRAM_PTR_CH_CMD_INM(m)      \
        in_dword_masked(HWIO_GSI_IRAM_PTR_CH_CMD_ADDR, m)
#define HWIO_GSI_IRAM_PTR_CH_CMD_OUT(v)      \
        out_dword(HWIO_GSI_IRAM_PTR_CH_CMD_ADDR,v)
#define HWIO_GSI_IRAM_PTR_CH_CMD_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GSI_IRAM_PTR_CH_CMD_ADDR,m,v,HWIO_GSI_IRAM_PTR_CH_CMD_IN)
#define HWIO_GSI_IRAM_PTR_CH_CMD_IRAM_PTR_BMSK                                          0xfff
#define HWIO_GSI_IRAM_PTR_CH_CMD_IRAM_PTR_SHFT                                            0x0

#define HWIO_GSI_IRAM_PTR_EE_GENERIC_CMD_ADDR                                      (GSI_REG_BASE      + 0x00000404)
#define HWIO_GSI_IRAM_PTR_EE_GENERIC_CMD_PHYS                                      (GSI_REG_BASE_PHYS + 0x00000404)
#define HWIO_GSI_IRAM_PTR_EE_GENERIC_CMD_OFFS                                      (GSI_REG_BASE_OFFS + 0x00000404)
#define HWIO_GSI_IRAM_PTR_EE_GENERIC_CMD_RMSK                                           0xfff
#define HWIO_GSI_IRAM_PTR_EE_GENERIC_CMD_ATTR                                             0x3
#define HWIO_GSI_IRAM_PTR_EE_GENERIC_CMD_IN          \
        in_dword_masked(HWIO_GSI_IRAM_PTR_EE_GENERIC_CMD_ADDR, HWIO_GSI_IRAM_PTR_EE_GENERIC_CMD_RMSK)
#define HWIO_GSI_IRAM_PTR_EE_GENERIC_CMD_INM(m)      \
        in_dword_masked(HWIO_GSI_IRAM_PTR_EE_GENERIC_CMD_ADDR, m)
#define HWIO_GSI_IRAM_PTR_EE_GENERIC_CMD_OUT(v)      \
        out_dword(HWIO_GSI_IRAM_PTR_EE_GENERIC_CMD_ADDR,v)
#define HWIO_GSI_IRAM_PTR_EE_GENERIC_CMD_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GSI_IRAM_PTR_EE_GENERIC_CMD_ADDR,m,v,HWIO_GSI_IRAM_PTR_EE_GENERIC_CMD_IN)
#define HWIO_GSI_IRAM_PTR_EE_GENERIC_CMD_IRAM_PTR_BMSK                                  0xfff
#define HWIO_GSI_IRAM_PTR_EE_GENERIC_CMD_IRAM_PTR_SHFT                                    0x0

#define HWIO_GSI_IRAM_PTR_CH_DB_ADDR                                               (GSI_REG_BASE      + 0x00000418)
#define HWIO_GSI_IRAM_PTR_CH_DB_PHYS                                               (GSI_REG_BASE_PHYS + 0x00000418)
#define HWIO_GSI_IRAM_PTR_CH_DB_OFFS                                               (GSI_REG_BASE_OFFS + 0x00000418)
#define HWIO_GSI_IRAM_PTR_CH_DB_RMSK                                                    0xfff
#define HWIO_GSI_IRAM_PTR_CH_DB_ATTR                                                      0x3
#define HWIO_GSI_IRAM_PTR_CH_DB_IN          \
        in_dword_masked(HWIO_GSI_IRAM_PTR_CH_DB_ADDR, HWIO_GSI_IRAM_PTR_CH_DB_RMSK)
#define HWIO_GSI_IRAM_PTR_CH_DB_INM(m)      \
        in_dword_masked(HWIO_GSI_IRAM_PTR_CH_DB_ADDR, m)
#define HWIO_GSI_IRAM_PTR_CH_DB_OUT(v)      \
        out_dword(HWIO_GSI_IRAM_PTR_CH_DB_ADDR,v)
#define HWIO_GSI_IRAM_PTR_CH_DB_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GSI_IRAM_PTR_CH_DB_ADDR,m,v,HWIO_GSI_IRAM_PTR_CH_DB_IN)
#define HWIO_GSI_IRAM_PTR_CH_DB_IRAM_PTR_BMSK                                           0xfff
#define HWIO_GSI_IRAM_PTR_CH_DB_IRAM_PTR_SHFT                                             0x0

#define HWIO_GSI_IRAM_PTR_EV_DB_ADDR                                               (GSI_REG_BASE      + 0x0000041c)
#define HWIO_GSI_IRAM_PTR_EV_DB_PHYS                                               (GSI_REG_BASE_PHYS + 0x0000041c)
#define HWIO_GSI_IRAM_PTR_EV_DB_OFFS                                               (GSI_REG_BASE_OFFS + 0x0000041c)
#define HWIO_GSI_IRAM_PTR_EV_DB_RMSK                                                    0xfff
#define HWIO_GSI_IRAM_PTR_EV_DB_ATTR                                                      0x3
#define HWIO_GSI_IRAM_PTR_EV_DB_IN          \
        in_dword_masked(HWIO_GSI_IRAM_PTR_EV_DB_ADDR, HWIO_GSI_IRAM_PTR_EV_DB_RMSK)
#define HWIO_GSI_IRAM_PTR_EV_DB_INM(m)      \
        in_dword_masked(HWIO_GSI_IRAM_PTR_EV_DB_ADDR, m)
#define HWIO_GSI_IRAM_PTR_EV_DB_OUT(v)      \
        out_dword(HWIO_GSI_IRAM_PTR_EV_DB_ADDR,v)
#define HWIO_GSI_IRAM_PTR_EV_DB_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GSI_IRAM_PTR_EV_DB_ADDR,m,v,HWIO_GSI_IRAM_PTR_EV_DB_IN)
#define HWIO_GSI_IRAM_PTR_EV_DB_IRAM_PTR_BMSK                                           0xfff
#define HWIO_GSI_IRAM_PTR_EV_DB_IRAM_PTR_SHFT                                             0x0

#define HWIO_GSI_IRAM_PTR_NEW_RE_ADDR                                              (GSI_REG_BASE      + 0x00000420)
#define HWIO_GSI_IRAM_PTR_NEW_RE_PHYS                                              (GSI_REG_BASE_PHYS + 0x00000420)
#define HWIO_GSI_IRAM_PTR_NEW_RE_OFFS                                              (GSI_REG_BASE_OFFS + 0x00000420)
#define HWIO_GSI_IRAM_PTR_NEW_RE_RMSK                                                   0xfff
#define HWIO_GSI_IRAM_PTR_NEW_RE_ATTR                                                     0x3
#define HWIO_GSI_IRAM_PTR_NEW_RE_IN          \
        in_dword_masked(HWIO_GSI_IRAM_PTR_NEW_RE_ADDR, HWIO_GSI_IRAM_PTR_NEW_RE_RMSK)
#define HWIO_GSI_IRAM_PTR_NEW_RE_INM(m)      \
        in_dword_masked(HWIO_GSI_IRAM_PTR_NEW_RE_ADDR, m)
#define HWIO_GSI_IRAM_PTR_NEW_RE_OUT(v)      \
        out_dword(HWIO_GSI_IRAM_PTR_NEW_RE_ADDR,v)
#define HWIO_GSI_IRAM_PTR_NEW_RE_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GSI_IRAM_PTR_NEW_RE_ADDR,m,v,HWIO_GSI_IRAM_PTR_NEW_RE_IN)
#define HWIO_GSI_IRAM_PTR_NEW_RE_IRAM_PTR_BMSK                                          0xfff
#define HWIO_GSI_IRAM_PTR_NEW_RE_IRAM_PTR_SHFT                                            0x0

#define HWIO_GSI_IRAM_PTR_CH_DIS_COMP_ADDR                                         (GSI_REG_BASE      + 0x00000424)
#define HWIO_GSI_IRAM_PTR_CH_DIS_COMP_PHYS                                         (GSI_REG_BASE_PHYS + 0x00000424)
#define HWIO_GSI_IRAM_PTR_CH_DIS_COMP_OFFS                                         (GSI_REG_BASE_OFFS + 0x00000424)
#define HWIO_GSI_IRAM_PTR_CH_DIS_COMP_RMSK                                              0xfff
#define HWIO_GSI_IRAM_PTR_CH_DIS_COMP_ATTR                                                0x3
#define HWIO_GSI_IRAM_PTR_CH_DIS_COMP_IN          \
        in_dword_masked(HWIO_GSI_IRAM_PTR_CH_DIS_COMP_ADDR, HWIO_GSI_IRAM_PTR_CH_DIS_COMP_RMSK)
#define HWIO_GSI_IRAM_PTR_CH_DIS_COMP_INM(m)      \
        in_dword_masked(HWIO_GSI_IRAM_PTR_CH_DIS_COMP_ADDR, m)
#define HWIO_GSI_IRAM_PTR_CH_DIS_COMP_OUT(v)      \
        out_dword(HWIO_GSI_IRAM_PTR_CH_DIS_COMP_ADDR,v)
#define HWIO_GSI_IRAM_PTR_CH_DIS_COMP_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GSI_IRAM_PTR_CH_DIS_COMP_ADDR,m,v,HWIO_GSI_IRAM_PTR_CH_DIS_COMP_IN)
#define HWIO_GSI_IRAM_PTR_CH_DIS_COMP_IRAM_PTR_BMSK                                     0xfff
#define HWIO_GSI_IRAM_PTR_CH_DIS_COMP_IRAM_PTR_SHFT                                       0x0

#define HWIO_GSI_IRAM_PTR_CH_EMPTY_ADDR                                            (GSI_REG_BASE      + 0x00000428)
#define HWIO_GSI_IRAM_PTR_CH_EMPTY_PHYS                                            (GSI_REG_BASE_PHYS + 0x00000428)
#define HWIO_GSI_IRAM_PTR_CH_EMPTY_OFFS                                            (GSI_REG_BASE_OFFS + 0x00000428)
#define HWIO_GSI_IRAM_PTR_CH_EMPTY_RMSK                                                 0xfff
#define HWIO_GSI_IRAM_PTR_CH_EMPTY_ATTR                                                   0x3
#define HWIO_GSI_IRAM_PTR_CH_EMPTY_IN          \
        in_dword_masked(HWIO_GSI_IRAM_PTR_CH_EMPTY_ADDR, HWIO_GSI_IRAM_PTR_CH_EMPTY_RMSK)
#define HWIO_GSI_IRAM_PTR_CH_EMPTY_INM(m)      \
        in_dword_masked(HWIO_GSI_IRAM_PTR_CH_EMPTY_ADDR, m)
#define HWIO_GSI_IRAM_PTR_CH_EMPTY_OUT(v)      \
        out_dword(HWIO_GSI_IRAM_PTR_CH_EMPTY_ADDR,v)
#define HWIO_GSI_IRAM_PTR_CH_EMPTY_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GSI_IRAM_PTR_CH_EMPTY_ADDR,m,v,HWIO_GSI_IRAM_PTR_CH_EMPTY_IN)
#define HWIO_GSI_IRAM_PTR_CH_EMPTY_IRAM_PTR_BMSK                                        0xfff
#define HWIO_GSI_IRAM_PTR_CH_EMPTY_IRAM_PTR_SHFT                                          0x0

#define HWIO_GSI_IRAM_PTR_EVENT_GEN_COMP_ADDR                                      (GSI_REG_BASE      + 0x0000042c)
#define HWIO_GSI_IRAM_PTR_EVENT_GEN_COMP_PHYS                                      (GSI_REG_BASE_PHYS + 0x0000042c)
#define HWIO_GSI_IRAM_PTR_EVENT_GEN_COMP_OFFS                                      (GSI_REG_BASE_OFFS + 0x0000042c)
#define HWIO_GSI_IRAM_PTR_EVENT_GEN_COMP_RMSK                                           0xfff
#define HWIO_GSI_IRAM_PTR_EVENT_GEN_COMP_ATTR                                             0x3
#define HWIO_GSI_IRAM_PTR_EVENT_GEN_COMP_IN          \
        in_dword_masked(HWIO_GSI_IRAM_PTR_EVENT_GEN_COMP_ADDR, HWIO_GSI_IRAM_PTR_EVENT_GEN_COMP_RMSK)
#define HWIO_GSI_IRAM_PTR_EVENT_GEN_COMP_INM(m)      \
        in_dword_masked(HWIO_GSI_IRAM_PTR_EVENT_GEN_COMP_ADDR, m)
#define HWIO_GSI_IRAM_PTR_EVENT_GEN_COMP_OUT(v)      \
        out_dword(HWIO_GSI_IRAM_PTR_EVENT_GEN_COMP_ADDR,v)
#define HWIO_GSI_IRAM_PTR_EVENT_GEN_COMP_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GSI_IRAM_PTR_EVENT_GEN_COMP_ADDR,m,v,HWIO_GSI_IRAM_PTR_EVENT_GEN_COMP_IN)
#define HWIO_GSI_IRAM_PTR_EVENT_GEN_COMP_IRAM_PTR_BMSK                                  0xfff
#define HWIO_GSI_IRAM_PTR_EVENT_GEN_COMP_IRAM_PTR_SHFT                                    0x0

#define HWIO_GSI_IRAM_PTR_PERIPH_IF_TLV_IN_0_ADDR                                  (GSI_REG_BASE      + 0x00000430)
#define HWIO_GSI_IRAM_PTR_PERIPH_IF_TLV_IN_0_PHYS                                  (GSI_REG_BASE_PHYS + 0x00000430)
#define HWIO_GSI_IRAM_PTR_PERIPH_IF_TLV_IN_0_OFFS                                  (GSI_REG_BASE_OFFS + 0x00000430)
#define HWIO_GSI_IRAM_PTR_PERIPH_IF_TLV_IN_0_RMSK                                       0xfff
#define HWIO_GSI_IRAM_PTR_PERIPH_IF_TLV_IN_0_ATTR                                         0x3
#define HWIO_GSI_IRAM_PTR_PERIPH_IF_TLV_IN_0_IN          \
        in_dword_masked(HWIO_GSI_IRAM_PTR_PERIPH_IF_TLV_IN_0_ADDR, HWIO_GSI_IRAM_PTR_PERIPH_IF_TLV_IN_0_RMSK)
#define HWIO_GSI_IRAM_PTR_PERIPH_IF_TLV_IN_0_INM(m)      \
        in_dword_masked(HWIO_GSI_IRAM_PTR_PERIPH_IF_TLV_IN_0_ADDR, m)
#define HWIO_GSI_IRAM_PTR_PERIPH_IF_TLV_IN_0_OUT(v)      \
        out_dword(HWIO_GSI_IRAM_PTR_PERIPH_IF_TLV_IN_0_ADDR,v)
#define HWIO_GSI_IRAM_PTR_PERIPH_IF_TLV_IN_0_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GSI_IRAM_PTR_PERIPH_IF_TLV_IN_0_ADDR,m,v,HWIO_GSI_IRAM_PTR_PERIPH_IF_TLV_IN_0_IN)
#define HWIO_GSI_IRAM_PTR_PERIPH_IF_TLV_IN_0_IRAM_PTR_BMSK                              0xfff
#define HWIO_GSI_IRAM_PTR_PERIPH_IF_TLV_IN_0_IRAM_PTR_SHFT                                0x0

#define HWIO_GSI_IRAM_PTR_PERIPH_IF_TLV_IN_2_ADDR                                  (GSI_REG_BASE      + 0x00000434)
#define HWIO_GSI_IRAM_PTR_PERIPH_IF_TLV_IN_2_PHYS                                  (GSI_REG_BASE_PHYS + 0x00000434)
#define HWIO_GSI_IRAM_PTR_PERIPH_IF_TLV_IN_2_OFFS                                  (GSI_REG_BASE_OFFS + 0x00000434)
#define HWIO_GSI_IRAM_PTR_PERIPH_IF_TLV_IN_2_RMSK                                       0xfff
#define HWIO_GSI_IRAM_PTR_PERIPH_IF_TLV_IN_2_ATTR                                         0x3
#define HWIO_GSI_IRAM_PTR_PERIPH_IF_TLV_IN_2_IN          \
        in_dword_masked(HWIO_GSI_IRAM_PTR_PERIPH_IF_TLV_IN_2_ADDR, HWIO_GSI_IRAM_PTR_PERIPH_IF_TLV_IN_2_RMSK)
#define HWIO_GSI_IRAM_PTR_PERIPH_IF_TLV_IN_2_INM(m)      \
        in_dword_masked(HWIO_GSI_IRAM_PTR_PERIPH_IF_TLV_IN_2_ADDR, m)
#define HWIO_GSI_IRAM_PTR_PERIPH_IF_TLV_IN_2_OUT(v)      \
        out_dword(HWIO_GSI_IRAM_PTR_PERIPH_IF_TLV_IN_2_ADDR,v)
#define HWIO_GSI_IRAM_PTR_PERIPH_IF_TLV_IN_2_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GSI_IRAM_PTR_PERIPH_IF_TLV_IN_2_ADDR,m,v,HWIO_GSI_IRAM_PTR_PERIPH_IF_TLV_IN_2_IN)
#define HWIO_GSI_IRAM_PTR_PERIPH_IF_TLV_IN_2_IRAM_PTR_BMSK                              0xfff
#define HWIO_GSI_IRAM_PTR_PERIPH_IF_TLV_IN_2_IRAM_PTR_SHFT                                0x0

#define HWIO_GSI_IRAM_PTR_PERIPH_IF_TLV_IN_1_ADDR                                  (GSI_REG_BASE      + 0x00000438)
#define HWIO_GSI_IRAM_PTR_PERIPH_IF_TLV_IN_1_PHYS                                  (GSI_REG_BASE_PHYS + 0x00000438)
#define HWIO_GSI_IRAM_PTR_PERIPH_IF_TLV_IN_1_OFFS                                  (GSI_REG_BASE_OFFS + 0x00000438)
#define HWIO_GSI_IRAM_PTR_PERIPH_IF_TLV_IN_1_RMSK                                       0xfff
#define HWIO_GSI_IRAM_PTR_PERIPH_IF_TLV_IN_1_ATTR                                         0x3
#define HWIO_GSI_IRAM_PTR_PERIPH_IF_TLV_IN_1_IN          \
        in_dword_masked(HWIO_GSI_IRAM_PTR_PERIPH_IF_TLV_IN_1_ADDR, HWIO_GSI_IRAM_PTR_PERIPH_IF_TLV_IN_1_RMSK)
#define HWIO_GSI_IRAM_PTR_PERIPH_IF_TLV_IN_1_INM(m)      \
        in_dword_masked(HWIO_GSI_IRAM_PTR_PERIPH_IF_TLV_IN_1_ADDR, m)
#define HWIO_GSI_IRAM_PTR_PERIPH_IF_TLV_IN_1_OUT(v)      \
        out_dword(HWIO_GSI_IRAM_PTR_PERIPH_IF_TLV_IN_1_ADDR,v)
#define HWIO_GSI_IRAM_PTR_PERIPH_IF_TLV_IN_1_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GSI_IRAM_PTR_PERIPH_IF_TLV_IN_1_ADDR,m,v,HWIO_GSI_IRAM_PTR_PERIPH_IF_TLV_IN_1_IN)
#define HWIO_GSI_IRAM_PTR_PERIPH_IF_TLV_IN_1_IRAM_PTR_BMSK                              0xfff
#define HWIO_GSI_IRAM_PTR_PERIPH_IF_TLV_IN_1_IRAM_PTR_SHFT                                0x0

#define HWIO_GSI_IRAM_PTR_TIMER_EXPIRED_ADDR                                       (GSI_REG_BASE      + 0x0000043c)
#define HWIO_GSI_IRAM_PTR_TIMER_EXPIRED_PHYS                                       (GSI_REG_BASE_PHYS + 0x0000043c)
#define HWIO_GSI_IRAM_PTR_TIMER_EXPIRED_OFFS                                       (GSI_REG_BASE_OFFS + 0x0000043c)
#define HWIO_GSI_IRAM_PTR_TIMER_EXPIRED_RMSK                                            0xfff
#define HWIO_GSI_IRAM_PTR_TIMER_EXPIRED_ATTR                                              0x3
#define HWIO_GSI_IRAM_PTR_TIMER_EXPIRED_IN          \
        in_dword_masked(HWIO_GSI_IRAM_PTR_TIMER_EXPIRED_ADDR, HWIO_GSI_IRAM_PTR_TIMER_EXPIRED_RMSK)
#define HWIO_GSI_IRAM_PTR_TIMER_EXPIRED_INM(m)      \
        in_dword_masked(HWIO_GSI_IRAM_PTR_TIMER_EXPIRED_ADDR, m)
#define HWIO_GSI_IRAM_PTR_TIMER_EXPIRED_OUT(v)      \
        out_dword(HWIO_GSI_IRAM_PTR_TIMER_EXPIRED_ADDR,v)
#define HWIO_GSI_IRAM_PTR_TIMER_EXPIRED_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GSI_IRAM_PTR_TIMER_EXPIRED_ADDR,m,v,HWIO_GSI_IRAM_PTR_TIMER_EXPIRED_IN)
#define HWIO_GSI_IRAM_PTR_TIMER_EXPIRED_IRAM_PTR_BMSK                                   0xfff
#define HWIO_GSI_IRAM_PTR_TIMER_EXPIRED_IRAM_PTR_SHFT                                     0x0

#define HWIO_GSI_IRAM_PTR_WRITE_ENG_COMP_ADDR                                      (GSI_REG_BASE      + 0x00000440)
#define HWIO_GSI_IRAM_PTR_WRITE_ENG_COMP_PHYS                                      (GSI_REG_BASE_PHYS + 0x00000440)
#define HWIO_GSI_IRAM_PTR_WRITE_ENG_COMP_OFFS                                      (GSI_REG_BASE_OFFS + 0x00000440)
#define HWIO_GSI_IRAM_PTR_WRITE_ENG_COMP_RMSK                                           0xfff
#define HWIO_GSI_IRAM_PTR_WRITE_ENG_COMP_ATTR                                             0x3
#define HWIO_GSI_IRAM_PTR_WRITE_ENG_COMP_IN          \
        in_dword_masked(HWIO_GSI_IRAM_PTR_WRITE_ENG_COMP_ADDR, HWIO_GSI_IRAM_PTR_WRITE_ENG_COMP_RMSK)
#define HWIO_GSI_IRAM_PTR_WRITE_ENG_COMP_INM(m)      \
        in_dword_masked(HWIO_GSI_IRAM_PTR_WRITE_ENG_COMP_ADDR, m)
#define HWIO_GSI_IRAM_PTR_WRITE_ENG_COMP_OUT(v)      \
        out_dword(HWIO_GSI_IRAM_PTR_WRITE_ENG_COMP_ADDR,v)
#define HWIO_GSI_IRAM_PTR_WRITE_ENG_COMP_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GSI_IRAM_PTR_WRITE_ENG_COMP_ADDR,m,v,HWIO_GSI_IRAM_PTR_WRITE_ENG_COMP_IN)
#define HWIO_GSI_IRAM_PTR_WRITE_ENG_COMP_IRAM_PTR_BMSK                                  0xfff
#define HWIO_GSI_IRAM_PTR_WRITE_ENG_COMP_IRAM_PTR_SHFT                                    0x0

#define HWIO_GSI_IRAM_PTR_READ_ENG_COMP_ADDR                                       (GSI_REG_BASE      + 0x00000444)
#define HWIO_GSI_IRAM_PTR_READ_ENG_COMP_PHYS                                       (GSI_REG_BASE_PHYS + 0x00000444)
#define HWIO_GSI_IRAM_PTR_READ_ENG_COMP_OFFS                                       (GSI_REG_BASE_OFFS + 0x00000444)
#define HWIO_GSI_IRAM_PTR_READ_ENG_COMP_RMSK                                            0xfff
#define HWIO_GSI_IRAM_PTR_READ_ENG_COMP_ATTR                                              0x3
#define HWIO_GSI_IRAM_PTR_READ_ENG_COMP_IN          \
        in_dword_masked(HWIO_GSI_IRAM_PTR_READ_ENG_COMP_ADDR, HWIO_GSI_IRAM_PTR_READ_ENG_COMP_RMSK)
#define HWIO_GSI_IRAM_PTR_READ_ENG_COMP_INM(m)      \
        in_dword_masked(HWIO_GSI_IRAM_PTR_READ_ENG_COMP_ADDR, m)
#define HWIO_GSI_IRAM_PTR_READ_ENG_COMP_OUT(v)      \
        out_dword(HWIO_GSI_IRAM_PTR_READ_ENG_COMP_ADDR,v)
#define HWIO_GSI_IRAM_PTR_READ_ENG_COMP_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GSI_IRAM_PTR_READ_ENG_COMP_ADDR,m,v,HWIO_GSI_IRAM_PTR_READ_ENG_COMP_IN)
#define HWIO_GSI_IRAM_PTR_READ_ENG_COMP_IRAM_PTR_BMSK                                   0xfff
#define HWIO_GSI_IRAM_PTR_READ_ENG_COMP_IRAM_PTR_SHFT                                     0x0

#define HWIO_GSI_IRAM_PTR_UC_GP_INT_ADDR                                           (GSI_REG_BASE      + 0x00000448)
#define HWIO_GSI_IRAM_PTR_UC_GP_INT_PHYS                                           (GSI_REG_BASE_PHYS + 0x00000448)
#define HWIO_GSI_IRAM_PTR_UC_GP_INT_OFFS                                           (GSI_REG_BASE_OFFS + 0x00000448)
#define HWIO_GSI_IRAM_PTR_UC_GP_INT_RMSK                                                0xfff
#define HWIO_GSI_IRAM_PTR_UC_GP_INT_ATTR                                                  0x3
#define HWIO_GSI_IRAM_PTR_UC_GP_INT_IN          \
        in_dword_masked(HWIO_GSI_IRAM_PTR_UC_GP_INT_ADDR, HWIO_GSI_IRAM_PTR_UC_GP_INT_RMSK)
#define HWIO_GSI_IRAM_PTR_UC_GP_INT_INM(m)      \
        in_dword_masked(HWIO_GSI_IRAM_PTR_UC_GP_INT_ADDR, m)
#define HWIO_GSI_IRAM_PTR_UC_GP_INT_OUT(v)      \
        out_dword(HWIO_GSI_IRAM_PTR_UC_GP_INT_ADDR,v)
#define HWIO_GSI_IRAM_PTR_UC_GP_INT_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GSI_IRAM_PTR_UC_GP_INT_ADDR,m,v,HWIO_GSI_IRAM_PTR_UC_GP_INT_IN)
#define HWIO_GSI_IRAM_PTR_UC_GP_INT_IRAM_PTR_BMSK                                       0xfff
#define HWIO_GSI_IRAM_PTR_UC_GP_INT_IRAM_PTR_SHFT                                         0x0

#define HWIO_GSI_IRAM_PTR_INT_MOD_STOPED_ADDR                                      (GSI_REG_BASE      + 0x0000044c)
#define HWIO_GSI_IRAM_PTR_INT_MOD_STOPED_PHYS                                      (GSI_REG_BASE_PHYS + 0x0000044c)
#define HWIO_GSI_IRAM_PTR_INT_MOD_STOPED_OFFS                                      (GSI_REG_BASE_OFFS + 0x0000044c)
#define HWIO_GSI_IRAM_PTR_INT_MOD_STOPED_RMSK                                           0xfff
#define HWIO_GSI_IRAM_PTR_INT_MOD_STOPED_ATTR                                             0x3
#define HWIO_GSI_IRAM_PTR_INT_MOD_STOPED_IN          \
        in_dword_masked(HWIO_GSI_IRAM_PTR_INT_MOD_STOPED_ADDR, HWIO_GSI_IRAM_PTR_INT_MOD_STOPED_RMSK)
#define HWIO_GSI_IRAM_PTR_INT_MOD_STOPED_INM(m)      \
        in_dword_masked(HWIO_GSI_IRAM_PTR_INT_MOD_STOPED_ADDR, m)
#define HWIO_GSI_IRAM_PTR_INT_MOD_STOPED_OUT(v)      \
        out_dword(HWIO_GSI_IRAM_PTR_INT_MOD_STOPED_ADDR,v)
#define HWIO_GSI_IRAM_PTR_INT_MOD_STOPED_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GSI_IRAM_PTR_INT_MOD_STOPED_ADDR,m,v,HWIO_GSI_IRAM_PTR_INT_MOD_STOPED_IN)
#define HWIO_GSI_IRAM_PTR_INT_MOD_STOPED_IRAM_PTR_BMSK                                  0xfff
#define HWIO_GSI_IRAM_PTR_INT_MOD_STOPED_IRAM_PTR_SHFT                                    0x0

#define HWIO_GSI_INST_RAM_n_ADDR(n)                                                (GSI_REG_BASE      + 0x00004000 + 0x4 * (n))
#define HWIO_GSI_INST_RAM_n_PHYS(n)                                                (GSI_REG_BASE_PHYS + 0x00004000 + 0x4 * (n))
#define HWIO_GSI_INST_RAM_n_OFFS(n)                                                (GSI_REG_BASE_OFFS + 0x00004000 + 0x4 * (n))
#define HWIO_GSI_INST_RAM_n_RMSK                                                   0xffffffff
#define HWIO_GSI_INST_RAM_n_MAXn                                                         4095
#define HWIO_GSI_INST_RAM_n_INI(n)        \
        in_dword_masked(HWIO_GSI_INST_RAM_n_ADDR(n), HWIO_GSI_INST_RAM_n_RMSK)
#define HWIO_GSI_INST_RAM_n_INMI(n,mask)    \
        in_dword_masked(HWIO_GSI_INST_RAM_n_ADDR(n), mask)
#define HWIO_GSI_INST_RAM_n_OUTI(n,val)    \
        out_dword(HWIO_GSI_INST_RAM_n_ADDR(n),val)
#define HWIO_GSI_INST_RAM_n_OUTMI(n,mask,val) \
        out_dword_masked_ns(HWIO_GSI_INST_RAM_n_ADDR(n),mask,val,HWIO_GSI_INST_RAM_n_INI(n))
#define HWIO_GSI_INST_RAM_n_INST_BYTE_3_BMSK                                       0xff000000
#define HWIO_GSI_INST_RAM_n_INST_BYTE_3_SHFT                                             0x18
#define HWIO_GSI_INST_RAM_n_INST_BYTE_2_BMSK                                         0xff0000
#define HWIO_GSI_INST_RAM_n_INST_BYTE_2_SHFT                                             0x10
#define HWIO_GSI_INST_RAM_n_INST_BYTE_1_BMSK                                           0xff00
#define HWIO_GSI_INST_RAM_n_INST_BYTE_1_SHFT                                              0x8
#define HWIO_GSI_INST_RAM_n_INST_BYTE_0_BMSK                                             0xff
#define HWIO_GSI_INST_RAM_n_INST_BYTE_0_SHFT                                              0x0

#define HWIO_GSI_SHRAM_n_ADDR(n)                                                   (GSI_REG_BASE      + 0x00002000 + 0x4 * (n))
#define HWIO_GSI_SHRAM_n_PHYS(n)                                                   (GSI_REG_BASE_PHYS + 0x00002000 + 0x4 * (n))
#define HWIO_GSI_SHRAM_n_OFFS(n)                                                   (GSI_REG_BASE_OFFS + 0x00002000 + 0x4 * (n))
#define HWIO_GSI_SHRAM_n_RMSK                                                      0xffffffff
#define HWIO_GSI_SHRAM_n_MAXn                                                             767
#define HWIO_GSI_SHRAM_n_INI(n)        \
        in_dword_masked(HWIO_GSI_SHRAM_n_ADDR(n), HWIO_GSI_SHRAM_n_RMSK)
#define HWIO_GSI_SHRAM_n_INMI(n,mask)    \
        in_dword_masked(HWIO_GSI_SHRAM_n_ADDR(n), mask)
#define HWIO_GSI_SHRAM_n_OUTI(n,val)    \
        out_dword(HWIO_GSI_SHRAM_n_ADDR(n),val)
#define HWIO_GSI_SHRAM_n_OUTMI(n,mask,val) \
        out_dword_masked_ns(HWIO_GSI_SHRAM_n_ADDR(n),mask,val,HWIO_GSI_SHRAM_n_INI(n))
#define HWIO_GSI_SHRAM_n_SHRAM_BMSK                                                0xffffffff
#define HWIO_GSI_SHRAM_n_SHRAM_SHFT                                                       0x0

#define HWIO_GSI_TEST_BUS_SEL_ADDR                                                 (GSI_REG_BASE      + 0x00001000)
#define HWIO_GSI_TEST_BUS_SEL_PHYS                                                 (GSI_REG_BASE_PHYS + 0x00001000)
#define HWIO_GSI_TEST_BUS_SEL_OFFS                                                 (GSI_REG_BASE_OFFS + 0x00001000)
#define HWIO_GSI_TEST_BUS_SEL_RMSK                                                    0xf00ff
#define HWIO_GSI_TEST_BUS_SEL_ATTR                                                        0x3
#define HWIO_GSI_TEST_BUS_SEL_IN          \
        in_dword_masked(HWIO_GSI_TEST_BUS_SEL_ADDR, HWIO_GSI_TEST_BUS_SEL_RMSK)
#define HWIO_GSI_TEST_BUS_SEL_INM(m)      \
        in_dword_masked(HWIO_GSI_TEST_BUS_SEL_ADDR, m)
#define HWIO_GSI_TEST_BUS_SEL_OUT(v)      \
        out_dword(HWIO_GSI_TEST_BUS_SEL_ADDR,v)
#define HWIO_GSI_TEST_BUS_SEL_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GSI_TEST_BUS_SEL_ADDR,m,v,HWIO_GSI_TEST_BUS_SEL_IN)
#define HWIO_GSI_TEST_BUS_SEL_GSI_HW_EVENTS_SEL_BMSK                                  0xf0000
#define HWIO_GSI_TEST_BUS_SEL_GSI_HW_EVENTS_SEL_SHFT                                     0x10
#define HWIO_GSI_TEST_BUS_SEL_GSI_TESTBUS_SEL_BMSK                                       0xff
#define HWIO_GSI_TEST_BUS_SEL_GSI_TESTBUS_SEL_SHFT                                        0x0
#define HWIO_GSI_TEST_BUS_SEL_GSI_TESTBUS_SEL_ZEROS_FVAL                                  0x0
#define HWIO_GSI_TEST_BUS_SEL_GSI_TESTBUS_SEL_MCS_0_FVAL                                  0x1
#define HWIO_GSI_TEST_BUS_SEL_GSI_TESTBUS_SEL_MCS_1_FVAL                                  0x2
#define HWIO_GSI_TEST_BUS_SEL_GSI_TESTBUS_SEL_MCS_2_FVAL                                  0x3
#define HWIO_GSI_TEST_BUS_SEL_GSI_TESTBUS_SEL_MCS_3_FVAL                                  0x4
#define HWIO_GSI_TEST_BUS_SEL_GSI_TESTBUS_SEL_MCS_4_FVAL                                  0x5
#define HWIO_GSI_TEST_BUS_SEL_GSI_TESTBUS_SEL_DB_ENG_FVAL                                 0x9
#define HWIO_GSI_TEST_BUS_SEL_GSI_TESTBUS_SEL_REE_0_FVAL                                  0xb
#define HWIO_GSI_TEST_BUS_SEL_GSI_TESTBUS_SEL_REE_1_FVAL                                  0xc
#define HWIO_GSI_TEST_BUS_SEL_GSI_TESTBUS_SEL_REE_2_FVAL                                  0xd
#define HWIO_GSI_TEST_BUS_SEL_GSI_TESTBUS_SEL_REE_3_FVAL                                  0xe
#define HWIO_GSI_TEST_BUS_SEL_GSI_TESTBUS_SEL_EVE_0_FVAL                                 0x13
#define HWIO_GSI_TEST_BUS_SEL_GSI_TESTBUS_SEL_EVE_1_FVAL                                 0x14
#define HWIO_GSI_TEST_BUS_SEL_GSI_TESTBUS_SEL_EVE_2_FVAL                                 0x15
#define HWIO_GSI_TEST_BUS_SEL_GSI_TESTBUS_SEL_EVE_3_FVAL                                 0x16
#define HWIO_GSI_TEST_BUS_SEL_GSI_TESTBUS_SEL_EVE_4_FVAL                                 0x17
#define HWIO_GSI_TEST_BUS_SEL_GSI_TESTBUS_SEL_EVE_5_FVAL                                 0x18
#define HWIO_GSI_TEST_BUS_SEL_GSI_TESTBUS_SEL_IE_0_FVAL                                  0x1b
#define HWIO_GSI_TEST_BUS_SEL_GSI_TESTBUS_SEL_IE_1_FVAL                                  0x1c
#define HWIO_GSI_TEST_BUS_SEL_GSI_TESTBUS_SEL_IC_0_FVAL                                  0x1f
#define HWIO_GSI_TEST_BUS_SEL_GSI_TESTBUS_SEL_IC_1_FVAL                                  0x20
#define HWIO_GSI_TEST_BUS_SEL_GSI_TESTBUS_SEL_IC_2_FVAL                                  0x21
#define HWIO_GSI_TEST_BUS_SEL_GSI_TESTBUS_SEL_IC_3_FVAL                                  0x22
#define HWIO_GSI_TEST_BUS_SEL_GSI_TESTBUS_SEL_IC_4_FVAL                                  0x23
#define HWIO_GSI_TEST_BUS_SEL_GSI_TESTBUS_SEL_MOQA_0_FVAL                                0x27
#define HWIO_GSI_TEST_BUS_SEL_GSI_TESTBUS_SEL_MOQA_1_FVAL                                0x28
#define HWIO_GSI_TEST_BUS_SEL_GSI_TESTBUS_SEL_MOQA_2_FVAL                                0x29
#define HWIO_GSI_TEST_BUS_SEL_GSI_TESTBUS_SEL_MOQA_3_FVAL                                0x2a
#define HWIO_GSI_TEST_BUS_SEL_GSI_TESTBUS_SEL_TMR_0_FVAL                                 0x2b
#define HWIO_GSI_TEST_BUS_SEL_GSI_TESTBUS_SEL_TMR_1_FVAL                                 0x2c
#define HWIO_GSI_TEST_BUS_SEL_GSI_TESTBUS_SEL_TMR_2_FVAL                                 0x2d
#define HWIO_GSI_TEST_BUS_SEL_GSI_TESTBUS_SEL_TMR_3_FVAL                                 0x2e
#define HWIO_GSI_TEST_BUS_SEL_GSI_TESTBUS_SEL_RD_WR_0_FVAL                               0x33
#define HWIO_GSI_TEST_BUS_SEL_GSI_TESTBUS_SEL_RD_WR_1_FVAL                               0x34
#define HWIO_GSI_TEST_BUS_SEL_GSI_TESTBUS_SEL_RD_WR_2_FVAL                               0x35
#define HWIO_GSI_TEST_BUS_SEL_GSI_TESTBUS_SEL_RD_WR_3_FVAL                               0x36
#define HWIO_GSI_TEST_BUS_SEL_GSI_TESTBUS_SEL_CSR_FVAL                                   0x3a

#define HWIO_GSI_TEST_BUS_REG_ADDR                                                 (GSI_REG_BASE      + 0x00001008)
#define HWIO_GSI_TEST_BUS_REG_PHYS                                                 (GSI_REG_BASE_PHYS + 0x00001008)
#define HWIO_GSI_TEST_BUS_REG_OFFS                                                 (GSI_REG_BASE_OFFS + 0x00001008)
#define HWIO_GSI_TEST_BUS_REG_RMSK                                                 0xffffffff
#define HWIO_GSI_TEST_BUS_REG_ATTR                                                        0x1
#define HWIO_GSI_TEST_BUS_REG_IN          \
        in_dword_masked(HWIO_GSI_TEST_BUS_REG_ADDR, HWIO_GSI_TEST_BUS_REG_RMSK)
#define HWIO_GSI_TEST_BUS_REG_INM(m)      \
        in_dword_masked(HWIO_GSI_TEST_BUS_REG_ADDR, m)
#define HWIO_GSI_TEST_BUS_REG_GSI_TESTBUS_REG_BMSK                                 0xffffffff
#define HWIO_GSI_TEST_BUS_REG_GSI_TESTBUS_REG_SHFT                                        0x0

#define HWIO_GSI_DEBUG_BUSY_REG_ADDR                                               (GSI_REG_BASE      + 0x00001010)
#define HWIO_GSI_DEBUG_BUSY_REG_PHYS                                               (GSI_REG_BASE_PHYS + 0x00001010)
#define HWIO_GSI_DEBUG_BUSY_REG_OFFS                                               (GSI_REG_BASE_OFFS + 0x00001010)
#define HWIO_GSI_DEBUG_BUSY_REG_RMSK                                                     0xff
#define HWIO_GSI_DEBUG_BUSY_REG_ATTR                                                      0x1
#define HWIO_GSI_DEBUG_BUSY_REG_IN          \
        in_dword_masked(HWIO_GSI_DEBUG_BUSY_REG_ADDR, HWIO_GSI_DEBUG_BUSY_REG_RMSK)
#define HWIO_GSI_DEBUG_BUSY_REG_INM(m)      \
        in_dword_masked(HWIO_GSI_DEBUG_BUSY_REG_ADDR, m)
#define HWIO_GSI_DEBUG_BUSY_REG_REE_PWR_CLPS_BUSY_BMSK                                   0x80
#define HWIO_GSI_DEBUG_BUSY_REG_REE_PWR_CLPS_BUSY_SHFT                                    0x7
#define HWIO_GSI_DEBUG_BUSY_REG_INT_ENG_BUSY_BMSK                                        0x40
#define HWIO_GSI_DEBUG_BUSY_REG_INT_ENG_BUSY_SHFT                                         0x6
#define HWIO_GSI_DEBUG_BUSY_REG_EV_ENG_BUSY_BMSK                                         0x20
#define HWIO_GSI_DEBUG_BUSY_REG_EV_ENG_BUSY_SHFT                                          0x5
#define HWIO_GSI_DEBUG_BUSY_REG_RD_WR_BUSY_BMSK                                          0x10
#define HWIO_GSI_DEBUG_BUSY_REG_RD_WR_BUSY_SHFT                                           0x4
#define HWIO_GSI_DEBUG_BUSY_REG_TIMER_BUSY_BMSK                                           0x8
#define HWIO_GSI_DEBUG_BUSY_REG_TIMER_BUSY_SHFT                                           0x3
#define HWIO_GSI_DEBUG_BUSY_REG_MCS_BUSY_BMSK                                             0x4
#define HWIO_GSI_DEBUG_BUSY_REG_MCS_BUSY_SHFT                                             0x2
#define HWIO_GSI_DEBUG_BUSY_REG_REE_BUSY_BMSK                                             0x2
#define HWIO_GSI_DEBUG_BUSY_REG_REE_BUSY_SHFT                                             0x1
#define HWIO_GSI_DEBUG_BUSY_REG_CSR_BUSY_BMSK                                             0x1
#define HWIO_GSI_DEBUG_BUSY_REG_CSR_BUSY_SHFT                                             0x0

#define HWIO_GSI_DEBUG_EVENT_PENDING_ADDR                                          (GSI_REG_BASE      + 0x00001014)
#define HWIO_GSI_DEBUG_EVENT_PENDING_PHYS                                          (GSI_REG_BASE_PHYS + 0x00001014)
#define HWIO_GSI_DEBUG_EVENT_PENDING_OFFS                                          (GSI_REG_BASE_OFFS + 0x00001014)
#define HWIO_GSI_DEBUG_EVENT_PENDING_RMSK                                          0xffffffff
#define HWIO_GSI_DEBUG_EVENT_PENDING_ATTR                                                 0x1
#define HWIO_GSI_DEBUG_EVENT_PENDING_IN          \
        in_dword_masked(HWIO_GSI_DEBUG_EVENT_PENDING_ADDR, HWIO_GSI_DEBUG_EVENT_PENDING_RMSK)
#define HWIO_GSI_DEBUG_EVENT_PENDING_INM(m)      \
        in_dword_masked(HWIO_GSI_DEBUG_EVENT_PENDING_ADDR, m)
#define HWIO_GSI_DEBUG_EVENT_PENDING_CHID_BIT_MAP_BMSK                             0xffffffff
#define HWIO_GSI_DEBUG_EVENT_PENDING_CHID_BIT_MAP_SHFT                                    0x0

#define HWIO_GSI_DEBUG_TIMER_PENDING_ADDR                                          (GSI_REG_BASE      + 0x00001018)
#define HWIO_GSI_DEBUG_TIMER_PENDING_PHYS                                          (GSI_REG_BASE_PHYS + 0x00001018)
#define HWIO_GSI_DEBUG_TIMER_PENDING_OFFS                                          (GSI_REG_BASE_OFFS + 0x00001018)
#define HWIO_GSI_DEBUG_TIMER_PENDING_RMSK                                          0xffffffff
#define HWIO_GSI_DEBUG_TIMER_PENDING_ATTR                                                 0x1
#define HWIO_GSI_DEBUG_TIMER_PENDING_IN          \
        in_dword_masked(HWIO_GSI_DEBUG_TIMER_PENDING_ADDR, HWIO_GSI_DEBUG_TIMER_PENDING_RMSK)
#define HWIO_GSI_DEBUG_TIMER_PENDING_INM(m)      \
        in_dword_masked(HWIO_GSI_DEBUG_TIMER_PENDING_ADDR, m)
#define HWIO_GSI_DEBUG_TIMER_PENDING_CHID_BIT_MAP_BMSK                             0xffffffff
#define HWIO_GSI_DEBUG_TIMER_PENDING_CHID_BIT_MAP_SHFT                                    0x0

#define HWIO_GSI_DEBUG_RD_WR_PENDING_ADDR                                          (GSI_REG_BASE      + 0x0000101c)
#define HWIO_GSI_DEBUG_RD_WR_PENDING_PHYS                                          (GSI_REG_BASE_PHYS + 0x0000101c)
#define HWIO_GSI_DEBUG_RD_WR_PENDING_OFFS                                          (GSI_REG_BASE_OFFS + 0x0000101c)
#define HWIO_GSI_DEBUG_RD_WR_PENDING_RMSK                                          0xffffffff
#define HWIO_GSI_DEBUG_RD_WR_PENDING_ATTR                                                 0x1
#define HWIO_GSI_DEBUG_RD_WR_PENDING_IN          \
        in_dword_masked(HWIO_GSI_DEBUG_RD_WR_PENDING_ADDR, HWIO_GSI_DEBUG_RD_WR_PENDING_RMSK)
#define HWIO_GSI_DEBUG_RD_WR_PENDING_INM(m)      \
        in_dword_masked(HWIO_GSI_DEBUG_RD_WR_PENDING_ADDR, m)
#define HWIO_GSI_DEBUG_RD_WR_PENDING_CHID_BIT_MAP_BMSK                             0xffffffff
#define HWIO_GSI_DEBUG_RD_WR_PENDING_CHID_BIT_MAP_SHFT                                    0x0

#define HWIO_GSI_DEBUG_COUNTER_CFGn_ADDR(n)                                        (GSI_REG_BASE      + 0x00001200 + 0x4 * (n))
#define HWIO_GSI_DEBUG_COUNTER_CFGn_PHYS(n)                                        (GSI_REG_BASE_PHYS + 0x00001200 + 0x4 * (n))
#define HWIO_GSI_DEBUG_COUNTER_CFGn_OFFS(n)                                        (GSI_REG_BASE_OFFS + 0x00001200 + 0x4 * (n))
#define HWIO_GSI_DEBUG_COUNTER_CFGn_RMSK                                              0x7fff7
#define HWIO_GSI_DEBUG_COUNTER_CFGn_MAXn                                                    7
#define HWIO_GSI_DEBUG_COUNTER_CFGn_INI(n)        \
        in_dword_masked(HWIO_GSI_DEBUG_COUNTER_CFGn_ADDR(n), HWIO_GSI_DEBUG_COUNTER_CFGn_RMSK)
#define HWIO_GSI_DEBUG_COUNTER_CFGn_INMI(n,mask)    \
        in_dword_masked(HWIO_GSI_DEBUG_COUNTER_CFGn_ADDR(n), mask)
#define HWIO_GSI_DEBUG_COUNTER_CFGn_OUTI(n,val)    \
        out_dword(HWIO_GSI_DEBUG_COUNTER_CFGn_ADDR(n),val)
#define HWIO_GSI_DEBUG_COUNTER_CFGn_OUTMI(n,mask,val) \
        out_dword_masked_ns(HWIO_GSI_DEBUG_COUNTER_CFGn_ADDR(n),mask,val,HWIO_GSI_DEBUG_COUNTER_CFGn_INI(n))
#define HWIO_GSI_DEBUG_COUNTER_CFGn_VIRTUAL_CHNL_BMSK                                 0x7f000
#define HWIO_GSI_DEBUG_COUNTER_CFGn_VIRTUAL_CHNL_SHFT                                     0xc
#define HWIO_GSI_DEBUG_COUNTER_CFGn_EE_BMSK                                             0xf00
#define HWIO_GSI_DEBUG_COUNTER_CFGn_EE_SHFT                                               0x8
#define HWIO_GSI_DEBUG_COUNTER_CFGn_EVNT_TYPE_BMSK                                       0xf0
#define HWIO_GSI_DEBUG_COUNTER_CFGn_EVNT_TYPE_SHFT                                        0x4
#define HWIO_GSI_DEBUG_COUNTER_CFGn_CLR_AT_READ_BMSK                                      0x4
#define HWIO_GSI_DEBUG_COUNTER_CFGn_CLR_AT_READ_SHFT                                      0x2
#define HWIO_GSI_DEBUG_COUNTER_CFGn_STOP_AT_WRAP_ARND_BMSK                                0x2
#define HWIO_GSI_DEBUG_COUNTER_CFGn_STOP_AT_WRAP_ARND_SHFT                                0x1
#define HWIO_GSI_DEBUG_COUNTER_CFGn_ENABLE_BMSK                                           0x1
#define HWIO_GSI_DEBUG_COUNTER_CFGn_ENABLE_SHFT                                           0x0

#define HWIO_GSI_DEBUG_COUNTERn_ADDR(n)                                            (GSI_REG_BASE      + 0x00001240 + 0x4 * (n))
#define HWIO_GSI_DEBUG_COUNTERn_PHYS(n)                                            (GSI_REG_BASE_PHYS + 0x00001240 + 0x4 * (n))
#define HWIO_GSI_DEBUG_COUNTERn_OFFS(n)                                            (GSI_REG_BASE_OFFS + 0x00001240 + 0x4 * (n))
#define HWIO_GSI_DEBUG_COUNTERn_RMSK                                                   0xffff
#define HWIO_GSI_DEBUG_COUNTERn_MAXn                                                        7
#define HWIO_GSI_DEBUG_COUNTERn_INI(n)        \
        in_dword_masked(HWIO_GSI_DEBUG_COUNTERn_ADDR(n), HWIO_GSI_DEBUG_COUNTERn_RMSK)
#define HWIO_GSI_DEBUG_COUNTERn_INMI(n,mask)    \
        in_dword_masked(HWIO_GSI_DEBUG_COUNTERn_ADDR(n), mask)
#define HWIO_GSI_DEBUG_COUNTERn_COUNTER_VALUE_BMSK                                     0xffff
#define HWIO_GSI_DEBUG_COUNTERn_COUNTER_VALUE_SHFT                                        0x0

#define HWIO_GSI_DEBUG_PC_FROM_SW_ADDR                                             (GSI_REG_BASE      + 0x00001040)
#define HWIO_GSI_DEBUG_PC_FROM_SW_PHYS                                             (GSI_REG_BASE_PHYS + 0x00001040)
#define HWIO_GSI_DEBUG_PC_FROM_SW_OFFS                                             (GSI_REG_BASE_OFFS + 0x00001040)
#define HWIO_GSI_DEBUG_PC_FROM_SW_RMSK                                                  0xfff
#define HWIO_GSI_DEBUG_PC_FROM_SW_ATTR                                                    0x3
#define HWIO_GSI_DEBUG_PC_FROM_SW_IN          \
        in_dword_masked(HWIO_GSI_DEBUG_PC_FROM_SW_ADDR, HWIO_GSI_DEBUG_PC_FROM_SW_RMSK)
#define HWIO_GSI_DEBUG_PC_FROM_SW_INM(m)      \
        in_dword_masked(HWIO_GSI_DEBUG_PC_FROM_SW_ADDR, m)
#define HWIO_GSI_DEBUG_PC_FROM_SW_OUT(v)      \
        out_dword(HWIO_GSI_DEBUG_PC_FROM_SW_ADDR,v)
#define HWIO_GSI_DEBUG_PC_FROM_SW_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GSI_DEBUG_PC_FROM_SW_ADDR,m,v,HWIO_GSI_DEBUG_PC_FROM_SW_IN)
#define HWIO_GSI_DEBUG_PC_FROM_SW_IRAM_PTR_BMSK                                         0xfff
#define HWIO_GSI_DEBUG_PC_FROM_SW_IRAM_PTR_SHFT                                           0x0

#define HWIO_GSI_DEBUG_SW_STALL_ADDR                                               (GSI_REG_BASE      + 0x00001044)
#define HWIO_GSI_DEBUG_SW_STALL_PHYS                                               (GSI_REG_BASE_PHYS + 0x00001044)
#define HWIO_GSI_DEBUG_SW_STALL_OFFS                                               (GSI_REG_BASE_OFFS + 0x00001044)
#define HWIO_GSI_DEBUG_SW_STALL_RMSK                                                      0x1
#define HWIO_GSI_DEBUG_SW_STALL_ATTR                                                      0x3
#define HWIO_GSI_DEBUG_SW_STALL_IN          \
        in_dword_masked(HWIO_GSI_DEBUG_SW_STALL_ADDR, HWIO_GSI_DEBUG_SW_STALL_RMSK)
#define HWIO_GSI_DEBUG_SW_STALL_INM(m)      \
        in_dword_masked(HWIO_GSI_DEBUG_SW_STALL_ADDR, m)
#define HWIO_GSI_DEBUG_SW_STALL_OUT(v)      \
        out_dword(HWIO_GSI_DEBUG_SW_STALL_ADDR,v)
#define HWIO_GSI_DEBUG_SW_STALL_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GSI_DEBUG_SW_STALL_ADDR,m,v,HWIO_GSI_DEBUG_SW_STALL_IN)
#define HWIO_GSI_DEBUG_SW_STALL_MCS_STALL_BMSK                                            0x1
#define HWIO_GSI_DEBUG_SW_STALL_MCS_STALL_SHFT                                            0x0

#define HWIO_GSI_DEBUG_PC_FOR_DEBUG_ADDR                                           (GSI_REG_BASE      + 0x00001048)
#define HWIO_GSI_DEBUG_PC_FOR_DEBUG_PHYS                                           (GSI_REG_BASE_PHYS + 0x00001048)
#define HWIO_GSI_DEBUG_PC_FOR_DEBUG_OFFS                                           (GSI_REG_BASE_OFFS + 0x00001048)
#define HWIO_GSI_DEBUG_PC_FOR_DEBUG_RMSK                                                0xfff
#define HWIO_GSI_DEBUG_PC_FOR_DEBUG_ATTR                                                  0x1
#define HWIO_GSI_DEBUG_PC_FOR_DEBUG_IN          \
        in_dword_masked(HWIO_GSI_DEBUG_PC_FOR_DEBUG_ADDR, HWIO_GSI_DEBUG_PC_FOR_DEBUG_RMSK)
#define HWIO_GSI_DEBUG_PC_FOR_DEBUG_INM(m)      \
        in_dword_masked(HWIO_GSI_DEBUG_PC_FOR_DEBUG_ADDR, m)
#define HWIO_GSI_DEBUG_PC_FOR_DEBUG_IRAM_PTR_BMSK                                       0xfff
#define HWIO_GSI_DEBUG_PC_FOR_DEBUG_IRAM_PTR_SHFT                                         0x0

#define HWIO_GSI_DEBUG_QSB_LOG_SEL_ADDR                                            (GSI_REG_BASE      + 0x00001050)
#define HWIO_GSI_DEBUG_QSB_LOG_SEL_PHYS                                            (GSI_REG_BASE_PHYS + 0x00001050)
#define HWIO_GSI_DEBUG_QSB_LOG_SEL_OFFS                                            (GSI_REG_BASE_OFFS + 0x00001050)
#define HWIO_GSI_DEBUG_QSB_LOG_SEL_RMSK                                              0xffff01
#define HWIO_GSI_DEBUG_QSB_LOG_SEL_ATTR                                                   0x3
#define HWIO_GSI_DEBUG_QSB_LOG_SEL_IN          \
        in_dword_masked(HWIO_GSI_DEBUG_QSB_LOG_SEL_ADDR, HWIO_GSI_DEBUG_QSB_LOG_SEL_RMSK)
#define HWIO_GSI_DEBUG_QSB_LOG_SEL_INM(m)      \
        in_dword_masked(HWIO_GSI_DEBUG_QSB_LOG_SEL_ADDR, m)
#define HWIO_GSI_DEBUG_QSB_LOG_SEL_OUT(v)      \
        out_dword(HWIO_GSI_DEBUG_QSB_LOG_SEL_ADDR,v)
#define HWIO_GSI_DEBUG_QSB_LOG_SEL_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GSI_DEBUG_QSB_LOG_SEL_ADDR,m,v,HWIO_GSI_DEBUG_QSB_LOG_SEL_IN)
#define HWIO_GSI_DEBUG_QSB_LOG_SEL_SEL_MID_BMSK                                      0xff0000
#define HWIO_GSI_DEBUG_QSB_LOG_SEL_SEL_MID_SHFT                                          0x10
#define HWIO_GSI_DEBUG_QSB_LOG_SEL_SEL_TID_BMSK                                        0xff00
#define HWIO_GSI_DEBUG_QSB_LOG_SEL_SEL_TID_SHFT                                           0x8
#define HWIO_GSI_DEBUG_QSB_LOG_SEL_SEL_WRITE_BMSK                                         0x1
#define HWIO_GSI_DEBUG_QSB_LOG_SEL_SEL_WRITE_SHFT                                         0x0

#define HWIO_GSI_DEBUG_QSB_LOG_CLR_ADDR                                            (GSI_REG_BASE      + 0x00001058)
#define HWIO_GSI_DEBUG_QSB_LOG_CLR_PHYS                                            (GSI_REG_BASE_PHYS + 0x00001058)
#define HWIO_GSI_DEBUG_QSB_LOG_CLR_OFFS                                            (GSI_REG_BASE_OFFS + 0x00001058)
#define HWIO_GSI_DEBUG_QSB_LOG_CLR_RMSK                                                   0x1
#define HWIO_GSI_DEBUG_QSB_LOG_CLR_ATTR                                                   0x2
#define HWIO_GSI_DEBUG_QSB_LOG_CLR_OUT(v)      \
        out_dword(HWIO_GSI_DEBUG_QSB_LOG_CLR_ADDR,v)
#define HWIO_GSI_DEBUG_QSB_LOG_CLR_LOG_CLR_BMSK                                           0x1
#define HWIO_GSI_DEBUG_QSB_LOG_CLR_LOG_CLR_SHFT                                           0x0

#define HWIO_GSI_DEBUG_QSB_LOG_ERR_TRNS_ID_ADDR                                    (GSI_REG_BASE      + 0x00001060)
#define HWIO_GSI_DEBUG_QSB_LOG_ERR_TRNS_ID_PHYS                                    (GSI_REG_BASE_PHYS + 0x00001060)
#define HWIO_GSI_DEBUG_QSB_LOG_ERR_TRNS_ID_OFFS                                    (GSI_REG_BASE_OFFS + 0x00001060)
#define HWIO_GSI_DEBUG_QSB_LOG_ERR_TRNS_ID_RMSK                                     0x1ffff01
#define HWIO_GSI_DEBUG_QSB_LOG_ERR_TRNS_ID_ATTR                                           0x1
#define HWIO_GSI_DEBUG_QSB_LOG_ERR_TRNS_ID_IN          \
        in_dword_masked(HWIO_GSI_DEBUG_QSB_LOG_ERR_TRNS_ID_ADDR, HWIO_GSI_DEBUG_QSB_LOG_ERR_TRNS_ID_RMSK)
#define HWIO_GSI_DEBUG_QSB_LOG_ERR_TRNS_ID_INM(m)      \
        in_dword_masked(HWIO_GSI_DEBUG_QSB_LOG_ERR_TRNS_ID_ADDR, m)
#define HWIO_GSI_DEBUG_QSB_LOG_ERR_TRNS_ID_ERR_SAVED_BMSK                           0x1000000
#define HWIO_GSI_DEBUG_QSB_LOG_ERR_TRNS_ID_ERR_SAVED_SHFT                                0x18
#define HWIO_GSI_DEBUG_QSB_LOG_ERR_TRNS_ID_ERR_MID_BMSK                              0xff0000
#define HWIO_GSI_DEBUG_QSB_LOG_ERR_TRNS_ID_ERR_MID_SHFT                                  0x10
#define HWIO_GSI_DEBUG_QSB_LOG_ERR_TRNS_ID_ERR_TID_BMSK                                0xff00
#define HWIO_GSI_DEBUG_QSB_LOG_ERR_TRNS_ID_ERR_TID_SHFT                                   0x8
#define HWIO_GSI_DEBUG_QSB_LOG_ERR_TRNS_ID_ERR_WRITE_BMSK                                 0x1
#define HWIO_GSI_DEBUG_QSB_LOG_ERR_TRNS_ID_ERR_WRITE_SHFT                                 0x0

#define HWIO_GSI_DEBUG_QSB_LOG_0_ADDR                                              (GSI_REG_BASE      + 0x00001064)
#define HWIO_GSI_DEBUG_QSB_LOG_0_PHYS                                              (GSI_REG_BASE_PHYS + 0x00001064)
#define HWIO_GSI_DEBUG_QSB_LOG_0_OFFS                                              (GSI_REG_BASE_OFFS + 0x00001064)
#define HWIO_GSI_DEBUG_QSB_LOG_0_RMSK                                              0xffffffff
#define HWIO_GSI_DEBUG_QSB_LOG_0_ATTR                                                     0x1
#define HWIO_GSI_DEBUG_QSB_LOG_0_IN          \
        in_dword_masked(HWIO_GSI_DEBUG_QSB_LOG_0_ADDR, HWIO_GSI_DEBUG_QSB_LOG_0_RMSK)
#define HWIO_GSI_DEBUG_QSB_LOG_0_INM(m)      \
        in_dword_masked(HWIO_GSI_DEBUG_QSB_LOG_0_ADDR, m)
#define HWIO_GSI_DEBUG_QSB_LOG_0_ADDR_31_0_BMSK                                    0xffffffff
#define HWIO_GSI_DEBUG_QSB_LOG_0_ADDR_31_0_SHFT                                           0x0

#define HWIO_GSI_DEBUG_QSB_LOG_1_ADDR                                              (GSI_REG_BASE      + 0x00001068)
#define HWIO_GSI_DEBUG_QSB_LOG_1_PHYS                                              (GSI_REG_BASE_PHYS + 0x00001068)
#define HWIO_GSI_DEBUG_QSB_LOG_1_OFFS                                              (GSI_REG_BASE_OFFS + 0x00001068)
#define HWIO_GSI_DEBUG_QSB_LOG_1_RMSK                                              0xfff7ffff
#define HWIO_GSI_DEBUG_QSB_LOG_1_ATTR                                                     0x1
#define HWIO_GSI_DEBUG_QSB_LOG_1_IN          \
        in_dword_masked(HWIO_GSI_DEBUG_QSB_LOG_1_ADDR, HWIO_GSI_DEBUG_QSB_LOG_1_RMSK)
#define HWIO_GSI_DEBUG_QSB_LOG_1_INM(m)      \
        in_dword_masked(HWIO_GSI_DEBUG_QSB_LOG_1_ADDR, m)
#define HWIO_GSI_DEBUG_QSB_LOG_1_AREQPRIORITY_BMSK                                 0xf0000000
#define HWIO_GSI_DEBUG_QSB_LOG_1_AREQPRIORITY_SHFT                                       0x1c
#define HWIO_GSI_DEBUG_QSB_LOG_1_ASIZE_BMSK                                         0xf000000
#define HWIO_GSI_DEBUG_QSB_LOG_1_ASIZE_SHFT                                              0x18
#define HWIO_GSI_DEBUG_QSB_LOG_1_ALEN_BMSK                                           0xf00000
#define HWIO_GSI_DEBUG_QSB_LOG_1_ALEN_SHFT                                               0x14
#define HWIO_GSI_DEBUG_QSB_LOG_1_AOOOWR_BMSK                                          0x40000
#define HWIO_GSI_DEBUG_QSB_LOG_1_AOOOWR_SHFT                                             0x12
#define HWIO_GSI_DEBUG_QSB_LOG_1_AOOORD_BMSK                                          0x20000
#define HWIO_GSI_DEBUG_QSB_LOG_1_AOOORD_SHFT                                             0x11
#define HWIO_GSI_DEBUG_QSB_LOG_1_ATRANSIENT_BMSK                                      0x10000
#define HWIO_GSI_DEBUG_QSB_LOG_1_ATRANSIENT_SHFT                                         0x10
#define HWIO_GSI_DEBUG_QSB_LOG_1_ACACHEABLE_BMSK                                       0x8000
#define HWIO_GSI_DEBUG_QSB_LOG_1_ACACHEABLE_SHFT                                          0xf
#define HWIO_GSI_DEBUG_QSB_LOG_1_ASHARED_BMSK                                          0x4000
#define HWIO_GSI_DEBUG_QSB_LOG_1_ASHARED_SHFT                                             0xe
#define HWIO_GSI_DEBUG_QSB_LOG_1_ANOALLOCATE_BMSK                                      0x2000
#define HWIO_GSI_DEBUG_QSB_LOG_1_ANOALLOCATE_SHFT                                         0xd
#define HWIO_GSI_DEBUG_QSB_LOG_1_AINNERSHARED_BMSK                                     0x1000
#define HWIO_GSI_DEBUG_QSB_LOG_1_AINNERSHARED_SHFT                                        0xc
#define HWIO_GSI_DEBUG_QSB_LOG_1_ADDR_43_32_BMSK                                        0xfff
#define HWIO_GSI_DEBUG_QSB_LOG_1_ADDR_43_32_SHFT                                          0x0

#define HWIO_GSI_DEBUG_QSB_LOG_2_ADDR                                              (GSI_REG_BASE      + 0x0000106c)
#define HWIO_GSI_DEBUG_QSB_LOG_2_PHYS                                              (GSI_REG_BASE_PHYS + 0x0000106c)
#define HWIO_GSI_DEBUG_QSB_LOG_2_OFFS                                              (GSI_REG_BASE_OFFS + 0x0000106c)
#define HWIO_GSI_DEBUG_QSB_LOG_2_RMSK                                                  0xffff
#define HWIO_GSI_DEBUG_QSB_LOG_2_ATTR                                                     0x1
#define HWIO_GSI_DEBUG_QSB_LOG_2_IN          \
        in_dword_masked(HWIO_GSI_DEBUG_QSB_LOG_2_ADDR, HWIO_GSI_DEBUG_QSB_LOG_2_RMSK)
#define HWIO_GSI_DEBUG_QSB_LOG_2_INM(m)      \
        in_dword_masked(HWIO_GSI_DEBUG_QSB_LOG_2_ADDR, m)
#define HWIO_GSI_DEBUG_QSB_LOG_2_AMEMTYPE_BMSK                                         0xf000
#define HWIO_GSI_DEBUG_QSB_LOG_2_AMEMTYPE_SHFT                                            0xc
#define HWIO_GSI_DEBUG_QSB_LOG_2_AMMUSID_BMSK                                           0xfff
#define HWIO_GSI_DEBUG_QSB_LOG_2_AMMUSID_SHFT                                             0x0

#define HWIO_GSI_DEBUG_QSB_LOG_LAST_MISC_IDn_ADDR(n)                               (GSI_REG_BASE      + 0x00001070 + 0x4 * (n))
#define HWIO_GSI_DEBUG_QSB_LOG_LAST_MISC_IDn_PHYS(n)                               (GSI_REG_BASE_PHYS + 0x00001070 + 0x4 * (n))
#define HWIO_GSI_DEBUG_QSB_LOG_LAST_MISC_IDn_OFFS(n)                               (GSI_REG_BASE_OFFS + 0x00001070 + 0x4 * (n))
#define HWIO_GSI_DEBUG_QSB_LOG_LAST_MISC_IDn_RMSK                                  0xffffffff
#define HWIO_GSI_DEBUG_QSB_LOG_LAST_MISC_IDn_MAXn                                           3
#define HWIO_GSI_DEBUG_QSB_LOG_LAST_MISC_IDn_INI(n)        \
        in_dword_masked(HWIO_GSI_DEBUG_QSB_LOG_LAST_MISC_IDn_ADDR(n), HWIO_GSI_DEBUG_QSB_LOG_LAST_MISC_IDn_RMSK)
#define HWIO_GSI_DEBUG_QSB_LOG_LAST_MISC_IDn_INMI(n,mask)    \
        in_dword_masked(HWIO_GSI_DEBUG_QSB_LOG_LAST_MISC_IDn_ADDR(n), mask)
#define HWIO_GSI_DEBUG_QSB_LOG_LAST_MISC_IDn_MID_BMSK                              0xf8000000
#define HWIO_GSI_DEBUG_QSB_LOG_LAST_MISC_IDn_MID_SHFT                                    0x1b
#define HWIO_GSI_DEBUG_QSB_LOG_LAST_MISC_IDn_TID_BMSK                               0x7c00000
#define HWIO_GSI_DEBUG_QSB_LOG_LAST_MISC_IDn_TID_SHFT                                    0x16
#define HWIO_GSI_DEBUG_QSB_LOG_LAST_MISC_IDn_WRITE_BMSK                              0x200000
#define HWIO_GSI_DEBUG_QSB_LOG_LAST_MISC_IDn_WRITE_SHFT                                  0x15
#define HWIO_GSI_DEBUG_QSB_LOG_LAST_MISC_IDn_ADDR_20_0_BMSK                          0x1fffff
#define HWIO_GSI_DEBUG_QSB_LOG_LAST_MISC_IDn_ADDR_20_0_SHFT                               0x0

#define HWIO_GSI_DEBUG_SW_RF_n_WRITE_ADDR(n)                                       (GSI_REG_BASE      + 0x00001080 + 0x4 * (n))
#define HWIO_GSI_DEBUG_SW_RF_n_WRITE_PHYS(n)                                       (GSI_REG_BASE_PHYS + 0x00001080 + 0x4 * (n))
#define HWIO_GSI_DEBUG_SW_RF_n_WRITE_OFFS(n)                                       (GSI_REG_BASE_OFFS + 0x00001080 + 0x4 * (n))
#define HWIO_GSI_DEBUG_SW_RF_n_WRITE_RMSK                                          0xffffffff
#define HWIO_GSI_DEBUG_SW_RF_n_WRITE_MAXn                                                  31
#define HWIO_GSI_DEBUG_SW_RF_n_WRITE_OUTI(n,val)    \
        out_dword(HWIO_GSI_DEBUG_SW_RF_n_WRITE_ADDR(n),val)
#define HWIO_GSI_DEBUG_SW_RF_n_WRITE_DATA_IN_BMSK                                  0xffffffff
#define HWIO_GSI_DEBUG_SW_RF_n_WRITE_DATA_IN_SHFT                                         0x0

#define HWIO_GSI_DEBUG_SW_RF_n_READ_ADDR(n)                                        (GSI_REG_BASE      + 0x00001100 + 0x4 * (n))
#define HWIO_GSI_DEBUG_SW_RF_n_READ_PHYS(n)                                        (GSI_REG_BASE_PHYS + 0x00001100 + 0x4 * (n))
#define HWIO_GSI_DEBUG_SW_RF_n_READ_OFFS(n)                                        (GSI_REG_BASE_OFFS + 0x00001100 + 0x4 * (n))
#define HWIO_GSI_DEBUG_SW_RF_n_READ_RMSK                                           0xffffffff
#define HWIO_GSI_DEBUG_SW_RF_n_READ_MAXn                                                   31
#define HWIO_GSI_DEBUG_SW_RF_n_READ_INI(n)        \
        in_dword_masked(HWIO_GSI_DEBUG_SW_RF_n_READ_ADDR(n), HWIO_GSI_DEBUG_SW_RF_n_READ_RMSK)
#define HWIO_GSI_DEBUG_SW_RF_n_READ_INMI(n,mask)    \
        in_dword_masked(HWIO_GSI_DEBUG_SW_RF_n_READ_ADDR(n), mask)
#define HWIO_GSI_DEBUG_SW_RF_n_READ_RF_REG_BMSK                                    0xffffffff
#define HWIO_GSI_DEBUG_SW_RF_n_READ_RF_REG_SHFT                                           0x0

#define HWIO_GSI_DEBUG_EE_n_CH_k_VP_TABLE_ADDR(n,k)                                (GSI_REG_BASE      + 0x00001400 + 0x80 * (n) + 0x4 * (k))
#define HWIO_GSI_DEBUG_EE_n_CH_k_VP_TABLE_PHYS(n,k)                                (GSI_REG_BASE_PHYS + 0x00001400 + 0x80 * (n) + 0x4 * (k))
#define HWIO_GSI_DEBUG_EE_n_CH_k_VP_TABLE_OFFS(n,k)                                (GSI_REG_BASE_OFFS + 0x00001400 + 0x80 * (n) + 0x4 * (k))
#define HWIO_GSI_DEBUG_EE_n_CH_k_VP_TABLE_RMSK                                           0x3f
#define HWIO_GSI_DEBUG_EE_n_CH_k_VP_TABLE_MAXn                                              3
#define HWIO_GSI_DEBUG_EE_n_CH_k_VP_TABLE_MAXk                                             19
#define HWIO_GSI_DEBUG_EE_n_CH_k_VP_TABLE_INI2(n,k)        \
        in_dword_masked(HWIO_GSI_DEBUG_EE_n_CH_k_VP_TABLE_ADDR(n,k), HWIO_GSI_DEBUG_EE_n_CH_k_VP_TABLE_RMSK)
#define HWIO_GSI_DEBUG_EE_n_CH_k_VP_TABLE_INMI2(n,k,mask)    \
        in_dword_masked(HWIO_GSI_DEBUG_EE_n_CH_k_VP_TABLE_ADDR(n,k), mask)
#define HWIO_GSI_DEBUG_EE_n_CH_k_VP_TABLE_VALID_BMSK                                     0x20
#define HWIO_GSI_DEBUG_EE_n_CH_k_VP_TABLE_VALID_SHFT                                      0x5
#define HWIO_GSI_DEBUG_EE_n_CH_k_VP_TABLE_PHY_CH_BMSK                                    0x1f
#define HWIO_GSI_DEBUG_EE_n_CH_k_VP_TABLE_PHY_CH_SHFT                                     0x0

#define HWIO_GSI_DEBUG_EE_n_EV_k_VP_TABLE_ADDR(n,k)                                (GSI_REG_BASE      + 0x00001600 + 0x80 * (n) + 0x4 * (k))
#define HWIO_GSI_DEBUG_EE_n_EV_k_VP_TABLE_PHYS(n,k)                                (GSI_REG_BASE_PHYS + 0x00001600 + 0x80 * (n) + 0x4 * (k))
#define HWIO_GSI_DEBUG_EE_n_EV_k_VP_TABLE_OFFS(n,k)                                (GSI_REG_BASE_OFFS + 0x00001600 + 0x80 * (n) + 0x4 * (k))
#define HWIO_GSI_DEBUG_EE_n_EV_k_VP_TABLE_RMSK                                           0x3f
#define HWIO_GSI_DEBUG_EE_n_EV_k_VP_TABLE_MAXn                                              3
#define HWIO_GSI_DEBUG_EE_n_EV_k_VP_TABLE_MAXk                                             13
#define HWIO_GSI_DEBUG_EE_n_EV_k_VP_TABLE_INI2(n,k)        \
        in_dword_masked(HWIO_GSI_DEBUG_EE_n_EV_k_VP_TABLE_ADDR(n,k), HWIO_GSI_DEBUG_EE_n_EV_k_VP_TABLE_RMSK)
#define HWIO_GSI_DEBUG_EE_n_EV_k_VP_TABLE_INMI2(n,k,mask)    \
        in_dword_masked(HWIO_GSI_DEBUG_EE_n_EV_k_VP_TABLE_ADDR(n,k), mask)
#define HWIO_GSI_DEBUG_EE_n_EV_k_VP_TABLE_VALID_BMSK                                     0x20
#define HWIO_GSI_DEBUG_EE_n_EV_k_VP_TABLE_VALID_SHFT                                      0x5
#define HWIO_GSI_DEBUG_EE_n_EV_k_VP_TABLE_PHY_EV_CH_BMSK                                 0x1f
#define HWIO_GSI_DEBUG_EE_n_EV_k_VP_TABLE_PHY_EV_CH_SHFT                                  0x0

#define HWIO_GSI_UC_SRC_IRQ_ADDR                                                   (GSI_REG_BASE      + 0x00000500)
#define HWIO_GSI_UC_SRC_IRQ_PHYS                                                   (GSI_REG_BASE_PHYS + 0x00000500)
#define HWIO_GSI_UC_SRC_IRQ_OFFS                                                   (GSI_REG_BASE_OFFS + 0x00000500)
#define HWIO_GSI_UC_SRC_IRQ_RMSK                                                          0xf
#define HWIO_GSI_UC_SRC_IRQ_ATTR                                                          0x1
#define HWIO_GSI_UC_SRC_IRQ_IN          \
        in_dword_masked(HWIO_GSI_UC_SRC_IRQ_ADDR, HWIO_GSI_UC_SRC_IRQ_RMSK)
#define HWIO_GSI_UC_SRC_IRQ_INM(m)      \
        in_dword_masked(HWIO_GSI_UC_SRC_IRQ_ADDR, m)
#define HWIO_GSI_UC_SRC_IRQ_IC_2_UC_MCS_INT_VLD_BMSK                                      0x8
#define HWIO_GSI_UC_SRC_IRQ_IC_2_UC_MCS_INT_VLD_SHFT                                      0x3
#define HWIO_GSI_UC_SRC_IRQ_ACC_2_UC_MCS_GO_ACK_BMSK                                      0x4
#define HWIO_GSI_UC_SRC_IRQ_ACC_2_UC_MCS_GO_ACK_SHFT                                      0x2
#define HWIO_GSI_UC_SRC_IRQ_UC_ACC_CMPLT_BMSK                                             0x2
#define HWIO_GSI_UC_SRC_IRQ_UC_ACC_CMPLT_SHFT                                             0x1
#define HWIO_GSI_UC_SRC_IRQ_UC_ACC_GO_BMSK                                                0x1
#define HWIO_GSI_UC_SRC_IRQ_UC_ACC_GO_SHFT                                                0x0

#define HWIO_GSI_UC_SRC_IRQ_MSK_ADDR                                               (GSI_REG_BASE      + 0x00000504)
#define HWIO_GSI_UC_SRC_IRQ_MSK_PHYS                                               (GSI_REG_BASE_PHYS + 0x00000504)
#define HWIO_GSI_UC_SRC_IRQ_MSK_OFFS                                               (GSI_REG_BASE_OFFS + 0x00000504)
#define HWIO_GSI_UC_SRC_IRQ_MSK_RMSK                                                      0xf
#define HWIO_GSI_UC_SRC_IRQ_MSK_ATTR                                                      0x3
#define HWIO_GSI_UC_SRC_IRQ_MSK_IN          \
        in_dword_masked(HWIO_GSI_UC_SRC_IRQ_MSK_ADDR, HWIO_GSI_UC_SRC_IRQ_MSK_RMSK)
#define HWIO_GSI_UC_SRC_IRQ_MSK_INM(m)      \
        in_dword_masked(HWIO_GSI_UC_SRC_IRQ_MSK_ADDR, m)
#define HWIO_GSI_UC_SRC_IRQ_MSK_OUT(v)      \
        out_dword(HWIO_GSI_UC_SRC_IRQ_MSK_ADDR,v)
#define HWIO_GSI_UC_SRC_IRQ_MSK_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GSI_UC_SRC_IRQ_MSK_ADDR,m,v,HWIO_GSI_UC_SRC_IRQ_MSK_IN)
#define HWIO_GSI_UC_SRC_IRQ_MSK_IC_2_UC_MCS_INT_VLD_BMSK                                  0x8
#define HWIO_GSI_UC_SRC_IRQ_MSK_IC_2_UC_MCS_INT_VLD_SHFT                                  0x3
#define HWIO_GSI_UC_SRC_IRQ_MSK_ACC_2_UC_MCS_GO_ACK_BMSK                                  0x4
#define HWIO_GSI_UC_SRC_IRQ_MSK_ACC_2_UC_MCS_GO_ACK_SHFT                                  0x2
#define HWIO_GSI_UC_SRC_IRQ_MSK_UC_ACC_CMPLT_BMSK                                         0x2
#define HWIO_GSI_UC_SRC_IRQ_MSK_UC_ACC_CMPLT_SHFT                                         0x1
#define HWIO_GSI_UC_SRC_IRQ_MSK_UC_ACC_GO_BMSK                                            0x1
#define HWIO_GSI_UC_SRC_IRQ_MSK_UC_ACC_GO_SHFT                                            0x0

#define HWIO_GSI_UC_SRC_IRQ_CLR_ADDR                                               (GSI_REG_BASE      + 0x00000508)
#define HWIO_GSI_UC_SRC_IRQ_CLR_PHYS                                               (GSI_REG_BASE_PHYS + 0x00000508)
#define HWIO_GSI_UC_SRC_IRQ_CLR_OFFS                                               (GSI_REG_BASE_OFFS + 0x00000508)
#define HWIO_GSI_UC_SRC_IRQ_CLR_RMSK                                                      0xf
#define HWIO_GSI_UC_SRC_IRQ_CLR_ATTR                                                      0x2
#define HWIO_GSI_UC_SRC_IRQ_CLR_OUT(v)      \
        out_dword(HWIO_GSI_UC_SRC_IRQ_CLR_ADDR,v)
#define HWIO_GSI_UC_SRC_IRQ_CLR_IC_2_UC_MCS_INT_VLD_BMSK                                  0x8
#define HWIO_GSI_UC_SRC_IRQ_CLR_IC_2_UC_MCS_INT_VLD_SHFT                                  0x3
#define HWIO_GSI_UC_SRC_IRQ_CLR_ACC_2_UC_MCS_GO_ACK_BMSK                                  0x4
#define HWIO_GSI_UC_SRC_IRQ_CLR_ACC_2_UC_MCS_GO_ACK_SHFT                                  0x2
#define HWIO_GSI_UC_SRC_IRQ_CLR_UC_ACC_CMPLT_BMSK                                         0x2
#define HWIO_GSI_UC_SRC_IRQ_CLR_UC_ACC_CMPLT_SHFT                                         0x1
#define HWIO_GSI_UC_SRC_IRQ_CLR_UC_ACC_GO_BMSK                                            0x1
#define HWIO_GSI_UC_SRC_IRQ_CLR_UC_ACC_GO_SHFT                                            0x0

#define HWIO_GSI_ACC_ARGS_n_ADDR(n)                                                (GSI_REG_BASE      + 0x0000050c + 0x4 * (n))
#define HWIO_GSI_ACC_ARGS_n_PHYS(n)                                                (GSI_REG_BASE_PHYS + 0x0000050c + 0x4 * (n))
#define HWIO_GSI_ACC_ARGS_n_OFFS(n)                                                (GSI_REG_BASE_OFFS + 0x0000050c + 0x4 * (n))
#define HWIO_GSI_ACC_ARGS_n_RMSK                                                   0xffffffff
#define HWIO_GSI_ACC_ARGS_n_MAXn                                                            5
#define HWIO_GSI_ACC_ARGS_n_INI(n)        \
        in_dword_masked(HWIO_GSI_ACC_ARGS_n_ADDR(n), HWIO_GSI_ACC_ARGS_n_RMSK)
#define HWIO_GSI_ACC_ARGS_n_INMI(n,mask)    \
        in_dword_masked(HWIO_GSI_ACC_ARGS_n_ADDR(n), mask)
#define HWIO_GSI_ACC_ARGS_n_OUTI(n,val)    \
        out_dword(HWIO_GSI_ACC_ARGS_n_ADDR(n),val)
#define HWIO_GSI_ACC_ARGS_n_OUTMI(n,mask,val) \
        out_dword_masked_ns(HWIO_GSI_ACC_ARGS_n_ADDR(n),mask,val,HWIO_GSI_ACC_ARGS_n_INI(n))
#define HWIO_GSI_ACC_ARGS_n_GSI_ACC_ARGS_BMSK                                      0xffffffff
#define HWIO_GSI_ACC_ARGS_n_GSI_ACC_ARGS_SHFT                                             0x0

#define HWIO_GSI_ACC_ROUTINE_ADDR                                                  (GSI_REG_BASE      + 0x00000524)
#define HWIO_GSI_ACC_ROUTINE_PHYS                                                  (GSI_REG_BASE_PHYS + 0x00000524)
#define HWIO_GSI_ACC_ROUTINE_OFFS                                                  (GSI_REG_BASE_OFFS + 0x00000524)
#define HWIO_GSI_ACC_ROUTINE_RMSK                                                  0xffffffff
#define HWIO_GSI_ACC_ROUTINE_ATTR                                                         0x3
#define HWIO_GSI_ACC_ROUTINE_IN          \
        in_dword_masked(HWIO_GSI_ACC_ROUTINE_ADDR, HWIO_GSI_ACC_ROUTINE_RMSK)
#define HWIO_GSI_ACC_ROUTINE_INM(m)      \
        in_dword_masked(HWIO_GSI_ACC_ROUTINE_ADDR, m)
#define HWIO_GSI_ACC_ROUTINE_OUT(v)      \
        out_dword(HWIO_GSI_ACC_ROUTINE_ADDR,v)
#define HWIO_GSI_ACC_ROUTINE_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GSI_ACC_ROUTINE_ADDR,m,v,HWIO_GSI_ACC_ROUTINE_IN)
#define HWIO_GSI_ACC_ROUTINE_GSI_ACC_ROUTINE_BMSK                                  0xffffffff
#define HWIO_GSI_ACC_ROUTINE_GSI_ACC_ROUTINE_SHFT                                         0x0

#define HWIO_GSI_ACC_GO_ADDR                                                       (GSI_REG_BASE      + 0x00000528)
#define HWIO_GSI_ACC_GO_PHYS                                                       (GSI_REG_BASE_PHYS + 0x00000528)
#define HWIO_GSI_ACC_GO_OFFS                                                       (GSI_REG_BASE_OFFS + 0x00000528)
#define HWIO_GSI_ACC_GO_RMSK                                                             0x7f
#define HWIO_GSI_ACC_GO_ATTR                                                              0x2
#define HWIO_GSI_ACC_GO_OUT(v)      \
        out_dword(HWIO_GSI_ACC_GO_ADDR,v)
#define HWIO_GSI_ACC_GO_TIMER_GO_BMSK                                                    0x40
#define HWIO_GSI_ACC_GO_TIMER_GO_SHFT                                                     0x6
#define HWIO_GSI_ACC_GO_RW_ENG_GO_BMSK                                                   0x20
#define HWIO_GSI_ACC_GO_RW_ENG_GO_SHFT                                                    0x5
#define HWIO_GSI_ACC_GO_INT_ENG_GO_BMSK                                                  0x10
#define HWIO_GSI_ACC_GO_INT_ENG_GO_SHFT                                                   0x4
#define HWIO_GSI_ACC_GO_TLV_OUT_GO_BMSK                                                   0x8
#define HWIO_GSI_ACC_GO_TLV_OUT_GO_SHFT                                                   0x3
#define HWIO_GSI_ACC_GO_CSR_GO_BMSK                                                       0x4
#define HWIO_GSI_ACC_GO_CSR_GO_SHFT                                                       0x2
#define HWIO_GSI_ACC_GO_RE_ENG_GO_BMSK                                                    0x2
#define HWIO_GSI_ACC_GO_RE_ENG_GO_SHFT                                                    0x1
#define HWIO_GSI_ACC_GO_EV_ENG_GO_BMSK                                                    0x1
#define HWIO_GSI_ACC_GO_EV_ENG_GO_SHFT                                                    0x0

#define HWIO_GSI_ACC_2_UC_MCS_STTS_ADDR                                            (GSI_REG_BASE      + 0x0000052c)
#define HWIO_GSI_ACC_2_UC_MCS_STTS_PHYS                                            (GSI_REG_BASE_PHYS + 0x0000052c)
#define HWIO_GSI_ACC_2_UC_MCS_STTS_OFFS                                            (GSI_REG_BASE_OFFS + 0x0000052c)
#define HWIO_GSI_ACC_2_UC_MCS_STTS_RMSK                                            0xffffffff
#define HWIO_GSI_ACC_2_UC_MCS_STTS_ATTR                                                   0x1
#define HWIO_GSI_ACC_2_UC_MCS_STTS_IN          \
        in_dword_masked(HWIO_GSI_ACC_2_UC_MCS_STTS_ADDR, HWIO_GSI_ACC_2_UC_MCS_STTS_RMSK)
#define HWIO_GSI_ACC_2_UC_MCS_STTS_INM(m)      \
        in_dword_masked(HWIO_GSI_ACC_2_UC_MCS_STTS_ADDR, m)
#define HWIO_GSI_ACC_2_UC_MCS_STTS_GSI_ACC_2_UC_MCS_STTS_BMSK                      0xffffffff
#define HWIO_GSI_ACC_2_UC_MCS_STTS_GSI_ACC_2_UC_MCS_STTS_SHFT                             0x0

#define HWIO_GSI_ACC_2_UC_MCS_RET_VAL_LSB_ADDR                                     (GSI_REG_BASE      + 0x00000530)
#define HWIO_GSI_ACC_2_UC_MCS_RET_VAL_LSB_PHYS                                     (GSI_REG_BASE_PHYS + 0x00000530)
#define HWIO_GSI_ACC_2_UC_MCS_RET_VAL_LSB_OFFS                                     (GSI_REG_BASE_OFFS + 0x00000530)
#define HWIO_GSI_ACC_2_UC_MCS_RET_VAL_LSB_RMSK                                     0xffffffff
#define HWIO_GSI_ACC_2_UC_MCS_RET_VAL_LSB_ATTR                                            0x1
#define HWIO_GSI_ACC_2_UC_MCS_RET_VAL_LSB_IN          \
        in_dword_masked(HWIO_GSI_ACC_2_UC_MCS_RET_VAL_LSB_ADDR, HWIO_GSI_ACC_2_UC_MCS_RET_VAL_LSB_RMSK)
#define HWIO_GSI_ACC_2_UC_MCS_RET_VAL_LSB_INM(m)      \
        in_dword_masked(HWIO_GSI_ACC_2_UC_MCS_RET_VAL_LSB_ADDR, m)
#define HWIO_GSI_ACC_2_UC_MCS_RET_VAL_LSB_GSI_ACC_2_UC_MCS_RET_VAL_BMSK            0xffffffff
#define HWIO_GSI_ACC_2_UC_MCS_RET_VAL_LSB_GSI_ACC_2_UC_MCS_RET_VAL_SHFT                   0x0

#define HWIO_GSI_ACC_2_UC_MCS_RET_VAL_MSB_ADDR                                     (GSI_REG_BASE      + 0x00000534)
#define HWIO_GSI_ACC_2_UC_MCS_RET_VAL_MSB_PHYS                                     (GSI_REG_BASE_PHYS + 0x00000534)
#define HWIO_GSI_ACC_2_UC_MCS_RET_VAL_MSB_OFFS                                     (GSI_REG_BASE_OFFS + 0x00000534)
#define HWIO_GSI_ACC_2_UC_MCS_RET_VAL_MSB_RMSK                                     0xffffffff
#define HWIO_GSI_ACC_2_UC_MCS_RET_VAL_MSB_ATTR                                            0x1
#define HWIO_GSI_ACC_2_UC_MCS_RET_VAL_MSB_IN          \
        in_dword_masked(HWIO_GSI_ACC_2_UC_MCS_RET_VAL_MSB_ADDR, HWIO_GSI_ACC_2_UC_MCS_RET_VAL_MSB_RMSK)
#define HWIO_GSI_ACC_2_UC_MCS_RET_VAL_MSB_INM(m)      \
        in_dword_masked(HWIO_GSI_ACC_2_UC_MCS_RET_VAL_MSB_ADDR, m)
#define HWIO_GSI_ACC_2_UC_MCS_RET_VAL_MSB_GSI_ACC_2_UC_MCS_RET_VAL_BMSK            0xffffffff
#define HWIO_GSI_ACC_2_UC_MCS_RET_VAL_MSB_GSI_ACC_2_UC_MCS_RET_VAL_SHFT                   0x0

#define HWIO_GSI_IC_2_UC_MCS_VLD_ADDR                                              (GSI_REG_BASE      + 0x00000538)
#define HWIO_GSI_IC_2_UC_MCS_VLD_PHYS                                              (GSI_REG_BASE_PHYS + 0x00000538)
#define HWIO_GSI_IC_2_UC_MCS_VLD_OFFS                                              (GSI_REG_BASE_OFFS + 0x00000538)
#define HWIO_GSI_IC_2_UC_MCS_VLD_RMSK                                              0xffffffff
#define HWIO_GSI_IC_2_UC_MCS_VLD_ATTR                                                     0x1
#define HWIO_GSI_IC_2_UC_MCS_VLD_IN          \
        in_dword_masked(HWIO_GSI_IC_2_UC_MCS_VLD_ADDR, HWIO_GSI_IC_2_UC_MCS_VLD_RMSK)
#define HWIO_GSI_IC_2_UC_MCS_VLD_INM(m)      \
        in_dword_masked(HWIO_GSI_IC_2_UC_MCS_VLD_ADDR, m)
#define HWIO_GSI_IC_2_UC_MCS_VLD_GSI_IC_2_UC_MCS_VLD_BMSK                          0xffffffff
#define HWIO_GSI_IC_2_UC_MCS_VLD_GSI_IC_2_UC_MCS_VLD_SHFT                                 0x0

#define HWIO_GSI_IC_2_UC_MCS_PC_ADDR                                               (GSI_REG_BASE      + 0x0000053c)
#define HWIO_GSI_IC_2_UC_MCS_PC_PHYS                                               (GSI_REG_BASE_PHYS + 0x0000053c)
#define HWIO_GSI_IC_2_UC_MCS_PC_OFFS                                               (GSI_REG_BASE_OFFS + 0x0000053c)
#define HWIO_GSI_IC_2_UC_MCS_PC_RMSK                                               0xffffffff
#define HWIO_GSI_IC_2_UC_MCS_PC_ATTR                                                      0x1
#define HWIO_GSI_IC_2_UC_MCS_PC_IN          \
        in_dword_masked(HWIO_GSI_IC_2_UC_MCS_PC_ADDR, HWIO_GSI_IC_2_UC_MCS_PC_RMSK)
#define HWIO_GSI_IC_2_UC_MCS_PC_INM(m)      \
        in_dword_masked(HWIO_GSI_IC_2_UC_MCS_PC_ADDR, m)
#define HWIO_GSI_IC_2_UC_MCS_PC_GSI_IC_2_UC_MCS_PC_BMSK                            0xffffffff
#define HWIO_GSI_IC_2_UC_MCS_PC_GSI_IC_2_UC_MCS_PC_SHFT                                   0x0

#define HWIO_GSI_IC_2_UC_MCS_ARGS_n_ADDR(n)                                        (GSI_REG_BASE      + 0x00000540 + 0x4 * (n))
#define HWIO_GSI_IC_2_UC_MCS_ARGS_n_PHYS(n)                                        (GSI_REG_BASE_PHYS + 0x00000540 + 0x4 * (n))
#define HWIO_GSI_IC_2_UC_MCS_ARGS_n_OFFS(n)                                        (GSI_REG_BASE_OFFS + 0x00000540 + 0x4 * (n))
#define HWIO_GSI_IC_2_UC_MCS_ARGS_n_RMSK                                           0xffffffff
#define HWIO_GSI_IC_2_UC_MCS_ARGS_n_MAXn                                                    5
#define HWIO_GSI_IC_2_UC_MCS_ARGS_n_INI(n)        \
        in_dword_masked(HWIO_GSI_IC_2_UC_MCS_ARGS_n_ADDR(n), HWIO_GSI_IC_2_UC_MCS_ARGS_n_RMSK)
#define HWIO_GSI_IC_2_UC_MCS_ARGS_n_INMI(n,mask)    \
        in_dword_masked(HWIO_GSI_IC_2_UC_MCS_ARGS_n_ADDR(n), mask)
#define HWIO_GSI_IC_2_UC_MCS_ARGS_n_GSI_IC_2_UC_MCS_ARGS_BMSK                      0xffffffff
#define HWIO_GSI_IC_2_UC_MCS_ARGS_n_GSI_IC_2_UC_MCS_ARGS_SHFT                             0x0

#define HWIO_GSI_UC_TLV_IN_VLD_ADDR                                                (GSI_REG_BASE      + 0x00000558)
#define HWIO_GSI_UC_TLV_IN_VLD_PHYS                                                (GSI_REG_BASE_PHYS + 0x00000558)
#define HWIO_GSI_UC_TLV_IN_VLD_OFFS                                                (GSI_REG_BASE_OFFS + 0x00000558)
#define HWIO_GSI_UC_TLV_IN_VLD_RMSK                                                       0x1
#define HWIO_GSI_UC_TLV_IN_VLD_ATTR                                                       0x3
#define HWIO_GSI_UC_TLV_IN_VLD_IN          \
        in_dword_masked(HWIO_GSI_UC_TLV_IN_VLD_ADDR, HWIO_GSI_UC_TLV_IN_VLD_RMSK)
#define HWIO_GSI_UC_TLV_IN_VLD_INM(m)      \
        in_dword_masked(HWIO_GSI_UC_TLV_IN_VLD_ADDR, m)
#define HWIO_GSI_UC_TLV_IN_VLD_OUT(v)      \
        out_dword(HWIO_GSI_UC_TLV_IN_VLD_ADDR,v)
#define HWIO_GSI_UC_TLV_IN_VLD_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GSI_UC_TLV_IN_VLD_ADDR,m,v,HWIO_GSI_UC_TLV_IN_VLD_IN)
#define HWIO_GSI_UC_TLV_IN_VLD_GSI_UC_TLV_IN_VLD_BMSK                                     0x1
#define HWIO_GSI_UC_TLV_IN_VLD_GSI_UC_TLV_IN_VLD_SHFT                                     0x0

#define HWIO_GSI_UC_TLV_IN_ROUTINE_ADDR                                            (GSI_REG_BASE      + 0x0000055c)
#define HWIO_GSI_UC_TLV_IN_ROUTINE_PHYS                                            (GSI_REG_BASE_PHYS + 0x0000055c)
#define HWIO_GSI_UC_TLV_IN_ROUTINE_OFFS                                            (GSI_REG_BASE_OFFS + 0x0000055c)
#define HWIO_GSI_UC_TLV_IN_ROUTINE_RMSK                                            0xffffffff
#define HWIO_GSI_UC_TLV_IN_ROUTINE_ATTR                                                   0x3
#define HWIO_GSI_UC_TLV_IN_ROUTINE_IN          \
        in_dword_masked(HWIO_GSI_UC_TLV_IN_ROUTINE_ADDR, HWIO_GSI_UC_TLV_IN_ROUTINE_RMSK)
#define HWIO_GSI_UC_TLV_IN_ROUTINE_INM(m)      \
        in_dword_masked(HWIO_GSI_UC_TLV_IN_ROUTINE_ADDR, m)
#define HWIO_GSI_UC_TLV_IN_ROUTINE_OUT(v)      \
        out_dword(HWIO_GSI_UC_TLV_IN_ROUTINE_ADDR,v)
#define HWIO_GSI_UC_TLV_IN_ROUTINE_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GSI_UC_TLV_IN_ROUTINE_ADDR,m,v,HWIO_GSI_UC_TLV_IN_ROUTINE_IN)
#define HWIO_GSI_UC_TLV_IN_ROUTINE_GSI_UC_TLV_IN_ROUTINE_BMSK                      0xffffffff
#define HWIO_GSI_UC_TLV_IN_ROUTINE_GSI_UC_TLV_IN_ROUTINE_SHFT                             0x0

#define HWIO_GSI_UC_TLV_IN_ARGS_n_ADDR(n)                                          (GSI_REG_BASE      + 0x00000560 + 0x4 * (n))
#define HWIO_GSI_UC_TLV_IN_ARGS_n_PHYS(n)                                          (GSI_REG_BASE_PHYS + 0x00000560 + 0x4 * (n))
#define HWIO_GSI_UC_TLV_IN_ARGS_n_OFFS(n)                                          (GSI_REG_BASE_OFFS + 0x00000560 + 0x4 * (n))
#define HWIO_GSI_UC_TLV_IN_ARGS_n_RMSK                                             0xffffffff
#define HWIO_GSI_UC_TLV_IN_ARGS_n_MAXn                                                      5
#define HWIO_GSI_UC_TLV_IN_ARGS_n_INI(n)        \
        in_dword_masked(HWIO_GSI_UC_TLV_IN_ARGS_n_ADDR(n), HWIO_GSI_UC_TLV_IN_ARGS_n_RMSK)
#define HWIO_GSI_UC_TLV_IN_ARGS_n_INMI(n,mask)    \
        in_dword_masked(HWIO_GSI_UC_TLV_IN_ARGS_n_ADDR(n), mask)
#define HWIO_GSI_UC_TLV_IN_ARGS_n_OUTI(n,val)    \
        out_dword(HWIO_GSI_UC_TLV_IN_ARGS_n_ADDR(n),val)
#define HWIO_GSI_UC_TLV_IN_ARGS_n_OUTMI(n,mask,val) \
        out_dword_masked_ns(HWIO_GSI_UC_TLV_IN_ARGS_n_ADDR(n),mask,val,HWIO_GSI_UC_TLV_IN_ARGS_n_INI(n))
#define HWIO_GSI_UC_TLV_IN_ARGS_n_GSI_UC_TLV_IN_ARGS_BMSK                          0xffffffff
#define HWIO_GSI_UC_TLV_IN_ARGS_n_GSI_UC_TLV_IN_ARGS_SHFT                                 0x0

#define HWIO_EE_n_GSI_CH_k_CNTXT_0_ADDR(n,k)                                       (GSI_REG_BASE      + 0x0001c000 + 0x4000 * (n) + 0x80 * (k))
#define HWIO_EE_n_GSI_CH_k_CNTXT_0_PHYS(n,k)                                       (GSI_REG_BASE_PHYS + 0x0001c000 + 0x4000 * (n) + 0x80 * (k))
#define HWIO_EE_n_GSI_CH_k_CNTXT_0_OFFS(n,k)                                       (GSI_REG_BASE_OFFS + 0x0001c000 + 0x4000 * (n) + 0x80 * (k))
#define HWIO_EE_n_GSI_CH_k_CNTXT_0_RMSK                                            0xfff7dfff
#define HWIO_EE_n_GSI_CH_k_CNTXT_0_MAXn                                                     2
#define HWIO_EE_n_GSI_CH_k_CNTXT_0_MAXk                                                    19
#define HWIO_EE_n_GSI_CH_k_CNTXT_0_INI2(n,k)        \
        in_dword_masked(HWIO_EE_n_GSI_CH_k_CNTXT_0_ADDR(n,k), HWIO_EE_n_GSI_CH_k_CNTXT_0_RMSK)
#define HWIO_EE_n_GSI_CH_k_CNTXT_0_INMI2(n,k,mask)    \
        in_dword_masked(HWIO_EE_n_GSI_CH_k_CNTXT_0_ADDR(n,k), mask)
#define HWIO_EE_n_GSI_CH_k_CNTXT_0_OUTI2(n,k,val)    \
        out_dword(HWIO_EE_n_GSI_CH_k_CNTXT_0_ADDR(n,k),val)
#define HWIO_EE_n_GSI_CH_k_CNTXT_0_OUTMI2(n,k,mask,val) \
        out_dword_masked_ns(HWIO_EE_n_GSI_CH_k_CNTXT_0_ADDR(n,k),mask,val,HWIO_EE_n_GSI_CH_k_CNTXT_0_INI2(n,k))
#define HWIO_EE_n_GSI_CH_k_CNTXT_0_ELEMENT_SIZE_BMSK                               0xff000000
#define HWIO_EE_n_GSI_CH_k_CNTXT_0_ELEMENT_SIZE_SHFT                                     0x18
#define HWIO_EE_n_GSI_CH_k_CNTXT_0_CHSTATE_BMSK                                      0xf00000
#define HWIO_EE_n_GSI_CH_k_CNTXT_0_CHSTATE_SHFT                                          0x14
#define HWIO_EE_n_GSI_CH_k_CNTXT_0_CHSTATE_NOT_ALLOCATED_FVAL                             0x0
#define HWIO_EE_n_GSI_CH_k_CNTXT_0_CHSTATE_ALLOCATED_FVAL                                 0x1
#define HWIO_EE_n_GSI_CH_k_CNTXT_0_CHSTATE_STARTED_FVAL                                   0x2
#define HWIO_EE_n_GSI_CH_k_CNTXT_0_CHSTATE_STOPED_FVAL                                    0x3
#define HWIO_EE_n_GSI_CH_k_CNTXT_0_CHSTATE_STOP_IN_PROC_FVAL                              0x4
#define HWIO_EE_n_GSI_CH_k_CNTXT_0_CHSTATE_ERROR_FVAL                                     0xf
#define HWIO_EE_n_GSI_CH_k_CNTXT_0_ERINDEX_BMSK                                       0x7c000
#define HWIO_EE_n_GSI_CH_k_CNTXT_0_ERINDEX_SHFT                                           0xe
#define HWIO_EE_n_GSI_CH_k_CNTXT_0_CHID_BMSK                                           0x1f00
#define HWIO_EE_n_GSI_CH_k_CNTXT_0_CHID_SHFT                                              0x8
#define HWIO_EE_n_GSI_CH_k_CNTXT_0_EE_BMSK                                               0xf0
#define HWIO_EE_n_GSI_CH_k_CNTXT_0_EE_SHFT                                                0x4
#define HWIO_EE_n_GSI_CH_k_CNTXT_0_CHTYPE_DIR_BMSK                                        0x8
#define HWIO_EE_n_GSI_CH_k_CNTXT_0_CHTYPE_DIR_SHFT                                        0x3
#define HWIO_EE_n_GSI_CH_k_CNTXT_0_CHTYPE_DIR_INBOUND_FVAL                                0x0
#define HWIO_EE_n_GSI_CH_k_CNTXT_0_CHTYPE_DIR_OUTBOUND_FVAL                               0x1
#define HWIO_EE_n_GSI_CH_k_CNTXT_0_CHTYPE_PROTOCOL_BMSK                                   0x7
#define HWIO_EE_n_GSI_CH_k_CNTXT_0_CHTYPE_PROTOCOL_SHFT                                   0x0
#define HWIO_EE_n_GSI_CH_k_CNTXT_0_CHTYPE_PROTOCOL_MHI_FVAL                               0x0
#define HWIO_EE_n_GSI_CH_k_CNTXT_0_CHTYPE_PROTOCOL_XHCI_FVAL                              0x1
#define HWIO_EE_n_GSI_CH_k_CNTXT_0_CHTYPE_PROTOCOL_GPI_FVAL                               0x2
#define HWIO_EE_n_GSI_CH_k_CNTXT_0_CHTYPE_PROTOCOL_XDCI_FVAL                              0x3

#define HWIO_EE_n_GSI_CH_k_CNTXT_1_ADDR(n,k)                                       (GSI_REG_BASE      + 0x0001c004 + 0x4000 * (n) + 0x80 * (k))
#define HWIO_EE_n_GSI_CH_k_CNTXT_1_PHYS(n,k)                                       (GSI_REG_BASE_PHYS + 0x0001c004 + 0x4000 * (n) + 0x80 * (k))
#define HWIO_EE_n_GSI_CH_k_CNTXT_1_OFFS(n,k)                                       (GSI_REG_BASE_OFFS + 0x0001c004 + 0x4000 * (n) + 0x80 * (k))
#define HWIO_EE_n_GSI_CH_k_CNTXT_1_RMSK                                                0xffff
#define HWIO_EE_n_GSI_CH_k_CNTXT_1_MAXn                                                     2
#define HWIO_EE_n_GSI_CH_k_CNTXT_1_MAXk                                                    19
#define HWIO_EE_n_GSI_CH_k_CNTXT_1_INI2(n,k)        \
        in_dword_masked(HWIO_EE_n_GSI_CH_k_CNTXT_1_ADDR(n,k), HWIO_EE_n_GSI_CH_k_CNTXT_1_RMSK)
#define HWIO_EE_n_GSI_CH_k_CNTXT_1_INMI2(n,k,mask)    \
        in_dword_masked(HWIO_EE_n_GSI_CH_k_CNTXT_1_ADDR(n,k), mask)
#define HWIO_EE_n_GSI_CH_k_CNTXT_1_OUTI2(n,k,val)    \
        out_dword(HWIO_EE_n_GSI_CH_k_CNTXT_1_ADDR(n,k),val)
#define HWIO_EE_n_GSI_CH_k_CNTXT_1_OUTMI2(n,k,mask,val) \
        out_dword_masked_ns(HWIO_EE_n_GSI_CH_k_CNTXT_1_ADDR(n,k),mask,val,HWIO_EE_n_GSI_CH_k_CNTXT_1_INI2(n,k))
#define HWIO_EE_n_GSI_CH_k_CNTXT_1_R_LENGTH_BMSK                                       0xffff
#define HWIO_EE_n_GSI_CH_k_CNTXT_1_R_LENGTH_SHFT                                          0x0

#define HWIO_EE_n_GSI_CH_k_CNTXT_2_ADDR(n,k)                                       (GSI_REG_BASE      + 0x0001c008 + 0x4000 * (n) + 0x80 * (k))
#define HWIO_EE_n_GSI_CH_k_CNTXT_2_PHYS(n,k)                                       (GSI_REG_BASE_PHYS + 0x0001c008 + 0x4000 * (n) + 0x80 * (k))
#define HWIO_EE_n_GSI_CH_k_CNTXT_2_OFFS(n,k)                                       (GSI_REG_BASE_OFFS + 0x0001c008 + 0x4000 * (n) + 0x80 * (k))
#define HWIO_EE_n_GSI_CH_k_CNTXT_2_RMSK                                            0xffffffff
#define HWIO_EE_n_GSI_CH_k_CNTXT_2_MAXn                                                     2
#define HWIO_EE_n_GSI_CH_k_CNTXT_2_MAXk                                                    19
#define HWIO_EE_n_GSI_CH_k_CNTXT_2_INI2(n,k)        \
        in_dword_masked(HWIO_EE_n_GSI_CH_k_CNTXT_2_ADDR(n,k), HWIO_EE_n_GSI_CH_k_CNTXT_2_RMSK)
#define HWIO_EE_n_GSI_CH_k_CNTXT_2_INMI2(n,k,mask)    \
        in_dword_masked(HWIO_EE_n_GSI_CH_k_CNTXT_2_ADDR(n,k), mask)
#define HWIO_EE_n_GSI_CH_k_CNTXT_2_OUTI2(n,k,val)    \
        out_dword(HWIO_EE_n_GSI_CH_k_CNTXT_2_ADDR(n,k),val)
#define HWIO_EE_n_GSI_CH_k_CNTXT_2_OUTMI2(n,k,mask,val) \
        out_dword_masked_ns(HWIO_EE_n_GSI_CH_k_CNTXT_2_ADDR(n,k),mask,val,HWIO_EE_n_GSI_CH_k_CNTXT_2_INI2(n,k))
#define HWIO_EE_n_GSI_CH_k_CNTXT_2_R_BASE_ADDR_LSBS_BMSK                           0xffffffff
#define HWIO_EE_n_GSI_CH_k_CNTXT_2_R_BASE_ADDR_LSBS_SHFT                                  0x0

#define HWIO_EE_n_GSI_CH_k_CNTXT_3_ADDR(n,k)                                       (GSI_REG_BASE      + 0x0001c00c + 0x4000 * (n) + 0x80 * (k))
#define HWIO_EE_n_GSI_CH_k_CNTXT_3_PHYS(n,k)                                       (GSI_REG_BASE_PHYS + 0x0001c00c + 0x4000 * (n) + 0x80 * (k))
#define HWIO_EE_n_GSI_CH_k_CNTXT_3_OFFS(n,k)                                       (GSI_REG_BASE_OFFS + 0x0001c00c + 0x4000 * (n) + 0x80 * (k))
#define HWIO_EE_n_GSI_CH_k_CNTXT_3_RMSK                                            0xffffffff
#define HWIO_EE_n_GSI_CH_k_CNTXT_3_MAXn                                                     2
#define HWIO_EE_n_GSI_CH_k_CNTXT_3_MAXk                                                    19
#define HWIO_EE_n_GSI_CH_k_CNTXT_3_INI2(n,k)        \
        in_dword_masked(HWIO_EE_n_GSI_CH_k_CNTXT_3_ADDR(n,k), HWIO_EE_n_GSI_CH_k_CNTXT_3_RMSK)
#define HWIO_EE_n_GSI_CH_k_CNTXT_3_INMI2(n,k,mask)    \
        in_dword_masked(HWIO_EE_n_GSI_CH_k_CNTXT_3_ADDR(n,k), mask)
#define HWIO_EE_n_GSI_CH_k_CNTXT_3_OUTI2(n,k,val)    \
        out_dword(HWIO_EE_n_GSI_CH_k_CNTXT_3_ADDR(n,k),val)
#define HWIO_EE_n_GSI_CH_k_CNTXT_3_OUTMI2(n,k,mask,val) \
        out_dword_masked_ns(HWIO_EE_n_GSI_CH_k_CNTXT_3_ADDR(n,k),mask,val,HWIO_EE_n_GSI_CH_k_CNTXT_3_INI2(n,k))
#define HWIO_EE_n_GSI_CH_k_CNTXT_3_R_BASE_ADDR_MSBS_BMSK                           0xffffffff
#define HWIO_EE_n_GSI_CH_k_CNTXT_3_R_BASE_ADDR_MSBS_SHFT                                  0x0

#define HWIO_EE_n_GSI_CH_k_CNTXT_4_ADDR(n,k)                                       (GSI_REG_BASE      + 0x0001c010 + 0x4000 * (n) + 0x80 * (k))
#define HWIO_EE_n_GSI_CH_k_CNTXT_4_PHYS(n,k)                                       (GSI_REG_BASE_PHYS + 0x0001c010 + 0x4000 * (n) + 0x80 * (k))
#define HWIO_EE_n_GSI_CH_k_CNTXT_4_OFFS(n,k)                                       (GSI_REG_BASE_OFFS + 0x0001c010 + 0x4000 * (n) + 0x80 * (k))
#define HWIO_EE_n_GSI_CH_k_CNTXT_4_RMSK                                            0xffffffff
#define HWIO_EE_n_GSI_CH_k_CNTXT_4_MAXn                                                     2
#define HWIO_EE_n_GSI_CH_k_CNTXT_4_MAXk                                                    19
#define HWIO_EE_n_GSI_CH_k_CNTXT_4_INI2(n,k)        \
        in_dword_masked(HWIO_EE_n_GSI_CH_k_CNTXT_4_ADDR(n,k), HWIO_EE_n_GSI_CH_k_CNTXT_4_RMSK)
#define HWIO_EE_n_GSI_CH_k_CNTXT_4_INMI2(n,k,mask)    \
        in_dword_masked(HWIO_EE_n_GSI_CH_k_CNTXT_4_ADDR(n,k), mask)
#define HWIO_EE_n_GSI_CH_k_CNTXT_4_OUTI2(n,k,val)    \
        out_dword(HWIO_EE_n_GSI_CH_k_CNTXT_4_ADDR(n,k),val)
#define HWIO_EE_n_GSI_CH_k_CNTXT_4_OUTMI2(n,k,mask,val) \
        out_dword_masked_ns(HWIO_EE_n_GSI_CH_k_CNTXT_4_ADDR(n,k),mask,val,HWIO_EE_n_GSI_CH_k_CNTXT_4_INI2(n,k))
#define HWIO_EE_n_GSI_CH_k_CNTXT_4_READ_PTR_LSB_BMSK                               0xffffffff
#define HWIO_EE_n_GSI_CH_k_CNTXT_4_READ_PTR_LSB_SHFT                                      0x0

#define HWIO_EE_n_GSI_CH_k_CNTXT_5_ADDR(n,k)                                       (GSI_REG_BASE      + 0x0001c014 + 0x4000 * (n) + 0x80 * (k))
#define HWIO_EE_n_GSI_CH_k_CNTXT_5_PHYS(n,k)                                       (GSI_REG_BASE_PHYS + 0x0001c014 + 0x4000 * (n) + 0x80 * (k))
#define HWIO_EE_n_GSI_CH_k_CNTXT_5_OFFS(n,k)                                       (GSI_REG_BASE_OFFS + 0x0001c014 + 0x4000 * (n) + 0x80 * (k))
#define HWIO_EE_n_GSI_CH_k_CNTXT_5_RMSK                                            0xffffffff
#define HWIO_EE_n_GSI_CH_k_CNTXT_5_MAXn                                                     2
#define HWIO_EE_n_GSI_CH_k_CNTXT_5_MAXk                                                    19
#define HWIO_EE_n_GSI_CH_k_CNTXT_5_INI2(n,k)        \
        in_dword_masked(HWIO_EE_n_GSI_CH_k_CNTXT_5_ADDR(n,k), HWIO_EE_n_GSI_CH_k_CNTXT_5_RMSK)
#define HWIO_EE_n_GSI_CH_k_CNTXT_5_INMI2(n,k,mask)    \
        in_dword_masked(HWIO_EE_n_GSI_CH_k_CNTXT_5_ADDR(n,k), mask)
#define HWIO_EE_n_GSI_CH_k_CNTXT_5_READ_PTR_MSB_BMSK                               0xffffffff
#define HWIO_EE_n_GSI_CH_k_CNTXT_5_READ_PTR_MSB_SHFT                                      0x0

#define HWIO_EE_n_GSI_CH_k_CNTXT_6_ADDR(n,k)                                       (GSI_REG_BASE      + 0x0001c018 + 0x4000 * (n) + 0x80 * (k))
#define HWIO_EE_n_GSI_CH_k_CNTXT_6_PHYS(n,k)                                       (GSI_REG_BASE_PHYS + 0x0001c018 + 0x4000 * (n) + 0x80 * (k))
#define HWIO_EE_n_GSI_CH_k_CNTXT_6_OFFS(n,k)                                       (GSI_REG_BASE_OFFS + 0x0001c018 + 0x4000 * (n) + 0x80 * (k))
#define HWIO_EE_n_GSI_CH_k_CNTXT_6_RMSK                                            0xffffffff
#define HWIO_EE_n_GSI_CH_k_CNTXT_6_MAXn                                                     2
#define HWIO_EE_n_GSI_CH_k_CNTXT_6_MAXk                                                    19
#define HWIO_EE_n_GSI_CH_k_CNTXT_6_INI2(n,k)        \
        in_dword_masked(HWIO_EE_n_GSI_CH_k_CNTXT_6_ADDR(n,k), HWIO_EE_n_GSI_CH_k_CNTXT_6_RMSK)
#define HWIO_EE_n_GSI_CH_k_CNTXT_6_INMI2(n,k,mask)    \
        in_dword_masked(HWIO_EE_n_GSI_CH_k_CNTXT_6_ADDR(n,k), mask)
#define HWIO_EE_n_GSI_CH_k_CNTXT_6_WRITE_PTR_LSB_BMSK                              0xffffffff
#define HWIO_EE_n_GSI_CH_k_CNTXT_6_WRITE_PTR_LSB_SHFT                                     0x0

#define HWIO_EE_n_GSI_CH_k_CNTXT_7_ADDR(n,k)                                       (GSI_REG_BASE      + 0x0001c01c + 0x4000 * (n) + 0x80 * (k))
#define HWIO_EE_n_GSI_CH_k_CNTXT_7_PHYS(n,k)                                       (GSI_REG_BASE_PHYS + 0x0001c01c + 0x4000 * (n) + 0x80 * (k))
#define HWIO_EE_n_GSI_CH_k_CNTXT_7_OFFS(n,k)                                       (GSI_REG_BASE_OFFS + 0x0001c01c + 0x4000 * (n) + 0x80 * (k))
#define HWIO_EE_n_GSI_CH_k_CNTXT_7_RMSK                                            0xffffffff
#define HWIO_EE_n_GSI_CH_k_CNTXT_7_MAXn                                                     2
#define HWIO_EE_n_GSI_CH_k_CNTXT_7_MAXk                                                    19
#define HWIO_EE_n_GSI_CH_k_CNTXT_7_INI2(n,k)        \
        in_dword_masked(HWIO_EE_n_GSI_CH_k_CNTXT_7_ADDR(n,k), HWIO_EE_n_GSI_CH_k_CNTXT_7_RMSK)
#define HWIO_EE_n_GSI_CH_k_CNTXT_7_INMI2(n,k,mask)    \
        in_dword_masked(HWIO_EE_n_GSI_CH_k_CNTXT_7_ADDR(n,k), mask)
#define HWIO_EE_n_GSI_CH_k_CNTXT_7_WRITE_PTR_MSB_BMSK                              0xffffffff
#define HWIO_EE_n_GSI_CH_k_CNTXT_7_WRITE_PTR_MSB_SHFT                                     0x0

#define HWIO_EE_n_GSI_CH_k_RE_FETCH_READ_PTR_ADDR(n,k)                             (GSI_REG_BASE      + 0x0001c054 + 0x4000 * (n) + 0x80 * (k))
#define HWIO_EE_n_GSI_CH_k_RE_FETCH_READ_PTR_PHYS(n,k)                             (GSI_REG_BASE_PHYS + 0x0001c054 + 0x4000 * (n) + 0x80 * (k))
#define HWIO_EE_n_GSI_CH_k_RE_FETCH_READ_PTR_OFFS(n,k)                             (GSI_REG_BASE_OFFS + 0x0001c054 + 0x4000 * (n) + 0x80 * (k))
#define HWIO_EE_n_GSI_CH_k_RE_FETCH_READ_PTR_RMSK                                      0xffff
#define HWIO_EE_n_GSI_CH_k_RE_FETCH_READ_PTR_MAXn                                           2
#define HWIO_EE_n_GSI_CH_k_RE_FETCH_READ_PTR_MAXk                                          19
#define HWIO_EE_n_GSI_CH_k_RE_FETCH_READ_PTR_INI2(n,k)        \
        in_dword_masked(HWIO_EE_n_GSI_CH_k_RE_FETCH_READ_PTR_ADDR(n,k), HWIO_EE_n_GSI_CH_k_RE_FETCH_READ_PTR_RMSK)
#define HWIO_EE_n_GSI_CH_k_RE_FETCH_READ_PTR_INMI2(n,k,mask)    \
        in_dword_masked(HWIO_EE_n_GSI_CH_k_RE_FETCH_READ_PTR_ADDR(n,k), mask)
#define HWIO_EE_n_GSI_CH_k_RE_FETCH_READ_PTR_OUTI2(n,k,val)    \
        out_dword(HWIO_EE_n_GSI_CH_k_RE_FETCH_READ_PTR_ADDR(n,k),val)
#define HWIO_EE_n_GSI_CH_k_RE_FETCH_READ_PTR_OUTMI2(n,k,mask,val) \
        out_dword_masked_ns(HWIO_EE_n_GSI_CH_k_RE_FETCH_READ_PTR_ADDR(n,k),mask,val,HWIO_EE_n_GSI_CH_k_RE_FETCH_READ_PTR_INI2(n,k))
#define HWIO_EE_n_GSI_CH_k_RE_FETCH_READ_PTR_READ_PTR_BMSK                             0xffff
#define HWIO_EE_n_GSI_CH_k_RE_FETCH_READ_PTR_READ_PTR_SHFT                                0x0

#define HWIO_EE_n_GSI_CH_k_RE_FETCH_WRITE_PTR_ADDR(n,k)                            (GSI_REG_BASE      + 0x0001c058 + 0x4000 * (n) + 0x80 * (k))
#define HWIO_EE_n_GSI_CH_k_RE_FETCH_WRITE_PTR_PHYS(n,k)                            (GSI_REG_BASE_PHYS + 0x0001c058 + 0x4000 * (n) + 0x80 * (k))
#define HWIO_EE_n_GSI_CH_k_RE_FETCH_WRITE_PTR_OFFS(n,k)                            (GSI_REG_BASE_OFFS + 0x0001c058 + 0x4000 * (n) + 0x80 * (k))
#define HWIO_EE_n_GSI_CH_k_RE_FETCH_WRITE_PTR_RMSK                                     0xffff
#define HWIO_EE_n_GSI_CH_k_RE_FETCH_WRITE_PTR_MAXn                                          2
#define HWIO_EE_n_GSI_CH_k_RE_FETCH_WRITE_PTR_MAXk                                         19
#define HWIO_EE_n_GSI_CH_k_RE_FETCH_WRITE_PTR_INI2(n,k)        \
        in_dword_masked(HWIO_EE_n_GSI_CH_k_RE_FETCH_WRITE_PTR_ADDR(n,k), HWIO_EE_n_GSI_CH_k_RE_FETCH_WRITE_PTR_RMSK)
#define HWIO_EE_n_GSI_CH_k_RE_FETCH_WRITE_PTR_INMI2(n,k,mask)    \
        in_dword_masked(HWIO_EE_n_GSI_CH_k_RE_FETCH_WRITE_PTR_ADDR(n,k), mask)
#define HWIO_EE_n_GSI_CH_k_RE_FETCH_WRITE_PTR_OUTI2(n,k,val)    \
        out_dword(HWIO_EE_n_GSI_CH_k_RE_FETCH_WRITE_PTR_ADDR(n,k),val)
#define HWIO_EE_n_GSI_CH_k_RE_FETCH_WRITE_PTR_OUTMI2(n,k,mask,val) \
        out_dword_masked_ns(HWIO_EE_n_GSI_CH_k_RE_FETCH_WRITE_PTR_ADDR(n,k),mask,val,HWIO_EE_n_GSI_CH_k_RE_FETCH_WRITE_PTR_INI2(n,k))
#define HWIO_EE_n_GSI_CH_k_RE_FETCH_WRITE_PTR_RE_INTR_DB_BMSK                          0xffff
#define HWIO_EE_n_GSI_CH_k_RE_FETCH_WRITE_PTR_RE_INTR_DB_SHFT                             0x0

#define HWIO_EE_n_GSI_CH_k_QOS_ADDR(n,k)                                           (GSI_REG_BASE      + 0x0001c05c + 0x4000 * (n) + 0x80 * (k))
#define HWIO_EE_n_GSI_CH_k_QOS_PHYS(n,k)                                           (GSI_REG_BASE_PHYS + 0x0001c05c + 0x4000 * (n) + 0x80 * (k))
#define HWIO_EE_n_GSI_CH_k_QOS_OFFS(n,k)                                           (GSI_REG_BASE_OFFS + 0x0001c05c + 0x4000 * (n) + 0x80 * (k))
#define HWIO_EE_n_GSI_CH_k_QOS_RMSK                                                     0x30f
#define HWIO_EE_n_GSI_CH_k_QOS_MAXn                                                         2
#define HWIO_EE_n_GSI_CH_k_QOS_MAXk                                                        19
#define HWIO_EE_n_GSI_CH_k_QOS_INI2(n,k)        \
        in_dword_masked(HWIO_EE_n_GSI_CH_k_QOS_ADDR(n,k), HWIO_EE_n_GSI_CH_k_QOS_RMSK)
#define HWIO_EE_n_GSI_CH_k_QOS_INMI2(n,k,mask)    \
        in_dword_masked(HWIO_EE_n_GSI_CH_k_QOS_ADDR(n,k), mask)
#define HWIO_EE_n_GSI_CH_k_QOS_OUTI2(n,k,val)    \
        out_dword(HWIO_EE_n_GSI_CH_k_QOS_ADDR(n,k),val)
#define HWIO_EE_n_GSI_CH_k_QOS_OUTMI2(n,k,mask,val) \
        out_dword_masked_ns(HWIO_EE_n_GSI_CH_k_QOS_ADDR(n,k),mask,val,HWIO_EE_n_GSI_CH_k_QOS_INI2(n,k))
#define HWIO_EE_n_GSI_CH_k_QOS_USE_DB_ENG_BMSK                                          0x200
#define HWIO_EE_n_GSI_CH_k_QOS_USE_DB_ENG_SHFT                                            0x9
#define HWIO_EE_n_GSI_CH_k_QOS_MAX_PREFETCH_BMSK                                        0x100
#define HWIO_EE_n_GSI_CH_k_QOS_MAX_PREFETCH_SHFT                                          0x8
#define HWIO_EE_n_GSI_CH_k_QOS_MAX_PREFETCH_ONE_PREFETCH_SEG_FVAL                         0x0
#define HWIO_EE_n_GSI_CH_k_QOS_MAX_PREFETCH_TWO_PREFETCH_SEG_FVAL                         0x1
#define HWIO_EE_n_GSI_CH_k_QOS_WRR_WEIGHT_BMSK                                            0xf
#define HWIO_EE_n_GSI_CH_k_QOS_WRR_WEIGHT_SHFT                                            0x0

#define HWIO_EE_n_GSI_CH_k_SCRATCH_0_ADDR(n,k)                                     (GSI_REG_BASE      + 0x0001c060 + 0x4000 * (n) + 0x80 * (k))
#define HWIO_EE_n_GSI_CH_k_SCRATCH_0_PHYS(n,k)                                     (GSI_REG_BASE_PHYS + 0x0001c060 + 0x4000 * (n) + 0x80 * (k))
#define HWIO_EE_n_GSI_CH_k_SCRATCH_0_OFFS(n,k)                                     (GSI_REG_BASE_OFFS + 0x0001c060 + 0x4000 * (n) + 0x80 * (k))
#define HWIO_EE_n_GSI_CH_k_SCRATCH_0_RMSK                                          0xffffffff
#define HWIO_EE_n_GSI_CH_k_SCRATCH_0_MAXn                                                   2
#define HWIO_EE_n_GSI_CH_k_SCRATCH_0_MAXk                                                  19
#define HWIO_EE_n_GSI_CH_k_SCRATCH_0_INI2(n,k)        \
        in_dword_masked(HWIO_EE_n_GSI_CH_k_SCRATCH_0_ADDR(n,k), HWIO_EE_n_GSI_CH_k_SCRATCH_0_RMSK)
#define HWIO_EE_n_GSI_CH_k_SCRATCH_0_INMI2(n,k,mask)    \
        in_dword_masked(HWIO_EE_n_GSI_CH_k_SCRATCH_0_ADDR(n,k), mask)
#define HWIO_EE_n_GSI_CH_k_SCRATCH_0_OUTI2(n,k,val)    \
        out_dword(HWIO_EE_n_GSI_CH_k_SCRATCH_0_ADDR(n,k),val)
#define HWIO_EE_n_GSI_CH_k_SCRATCH_0_OUTMI2(n,k,mask,val) \
        out_dword_masked_ns(HWIO_EE_n_GSI_CH_k_SCRATCH_0_ADDR(n,k),mask,val,HWIO_EE_n_GSI_CH_k_SCRATCH_0_INI2(n,k))
#define HWIO_EE_n_GSI_CH_k_SCRATCH_0_SCRATCH_BMSK                                  0xffffffff
#define HWIO_EE_n_GSI_CH_k_SCRATCH_0_SCRATCH_SHFT                                         0x0

#define HWIO_EE_n_GSI_CH_k_SCRATCH_1_ADDR(n,k)                                     (GSI_REG_BASE      + 0x0001c064 + 0x4000 * (n) + 0x80 * (k))
#define HWIO_EE_n_GSI_CH_k_SCRATCH_1_PHYS(n,k)                                     (GSI_REG_BASE_PHYS + 0x0001c064 + 0x4000 * (n) + 0x80 * (k))
#define HWIO_EE_n_GSI_CH_k_SCRATCH_1_OFFS(n,k)                                     (GSI_REG_BASE_OFFS + 0x0001c064 + 0x4000 * (n) + 0x80 * (k))
#define HWIO_EE_n_GSI_CH_k_SCRATCH_1_RMSK                                          0xffffffff
#define HWIO_EE_n_GSI_CH_k_SCRATCH_1_MAXn                                                   2
#define HWIO_EE_n_GSI_CH_k_SCRATCH_1_MAXk                                                  19
#define HWIO_EE_n_GSI_CH_k_SCRATCH_1_INI2(n,k)        \
        in_dword_masked(HWIO_EE_n_GSI_CH_k_SCRATCH_1_ADDR(n,k), HWIO_EE_n_GSI_CH_k_SCRATCH_1_RMSK)
#define HWIO_EE_n_GSI_CH_k_SCRATCH_1_INMI2(n,k,mask)    \
        in_dword_masked(HWIO_EE_n_GSI_CH_k_SCRATCH_1_ADDR(n,k), mask)
#define HWIO_EE_n_GSI_CH_k_SCRATCH_1_OUTI2(n,k,val)    \
        out_dword(HWIO_EE_n_GSI_CH_k_SCRATCH_1_ADDR(n,k),val)
#define HWIO_EE_n_GSI_CH_k_SCRATCH_1_OUTMI2(n,k,mask,val) \
        out_dword_masked_ns(HWIO_EE_n_GSI_CH_k_SCRATCH_1_ADDR(n,k),mask,val,HWIO_EE_n_GSI_CH_k_SCRATCH_1_INI2(n,k))
#define HWIO_EE_n_GSI_CH_k_SCRATCH_1_SCRATCH_BMSK                                  0xffffffff
#define HWIO_EE_n_GSI_CH_k_SCRATCH_1_SCRATCH_SHFT                                         0x0

#define HWIO_EE_n_GSI_CH_k_SCRATCH_2_ADDR(n,k)                                     (GSI_REG_BASE      + 0x0001c068 + 0x4000 * (n) + 0x80 * (k))
#define HWIO_EE_n_GSI_CH_k_SCRATCH_2_PHYS(n,k)                                     (GSI_REG_BASE_PHYS + 0x0001c068 + 0x4000 * (n) + 0x80 * (k))
#define HWIO_EE_n_GSI_CH_k_SCRATCH_2_OFFS(n,k)                                     (GSI_REG_BASE_OFFS + 0x0001c068 + 0x4000 * (n) + 0x80 * (k))
#define HWIO_EE_n_GSI_CH_k_SCRATCH_2_RMSK                                          0xffffffff
#define HWIO_EE_n_GSI_CH_k_SCRATCH_2_MAXn                                                   2
#define HWIO_EE_n_GSI_CH_k_SCRATCH_2_MAXk                                                  19
#define HWIO_EE_n_GSI_CH_k_SCRATCH_2_INI2(n,k)        \
        in_dword_masked(HWIO_EE_n_GSI_CH_k_SCRATCH_2_ADDR(n,k), HWIO_EE_n_GSI_CH_k_SCRATCH_2_RMSK)
#define HWIO_EE_n_GSI_CH_k_SCRATCH_2_INMI2(n,k,mask)    \
        in_dword_masked(HWIO_EE_n_GSI_CH_k_SCRATCH_2_ADDR(n,k), mask)
#define HWIO_EE_n_GSI_CH_k_SCRATCH_2_OUTI2(n,k,val)    \
        out_dword(HWIO_EE_n_GSI_CH_k_SCRATCH_2_ADDR(n,k),val)
#define HWIO_EE_n_GSI_CH_k_SCRATCH_2_OUTMI2(n,k,mask,val) \
        out_dword_masked_ns(HWIO_EE_n_GSI_CH_k_SCRATCH_2_ADDR(n,k),mask,val,HWIO_EE_n_GSI_CH_k_SCRATCH_2_INI2(n,k))
#define HWIO_EE_n_GSI_CH_k_SCRATCH_2_SCRATCH_BMSK                                  0xffffffff
#define HWIO_EE_n_GSI_CH_k_SCRATCH_2_SCRATCH_SHFT                                         0x0

#define HWIO_EE_n_GSI_CH_k_SCRATCH_3_ADDR(n,k)                                     (GSI_REG_BASE      + 0x0001c06c + 0x4000 * (n) + 0x80 * (k))
#define HWIO_EE_n_GSI_CH_k_SCRATCH_3_PHYS(n,k)                                     (GSI_REG_BASE_PHYS + 0x0001c06c + 0x4000 * (n) + 0x80 * (k))
#define HWIO_EE_n_GSI_CH_k_SCRATCH_3_OFFS(n,k)                                     (GSI_REG_BASE_OFFS + 0x0001c06c + 0x4000 * (n) + 0x80 * (k))
#define HWIO_EE_n_GSI_CH_k_SCRATCH_3_RMSK                                          0xffffffff
#define HWIO_EE_n_GSI_CH_k_SCRATCH_3_MAXn                                                   2
#define HWIO_EE_n_GSI_CH_k_SCRATCH_3_MAXk                                                  19
#define HWIO_EE_n_GSI_CH_k_SCRATCH_3_INI2(n,k)        \
        in_dword_masked(HWIO_EE_n_GSI_CH_k_SCRATCH_3_ADDR(n,k), HWIO_EE_n_GSI_CH_k_SCRATCH_3_RMSK)
#define HWIO_EE_n_GSI_CH_k_SCRATCH_3_INMI2(n,k,mask)    \
        in_dword_masked(HWIO_EE_n_GSI_CH_k_SCRATCH_3_ADDR(n,k), mask)
#define HWIO_EE_n_GSI_CH_k_SCRATCH_3_OUTI2(n,k,val)    \
        out_dword(HWIO_EE_n_GSI_CH_k_SCRATCH_3_ADDR(n,k),val)
#define HWIO_EE_n_GSI_CH_k_SCRATCH_3_OUTMI2(n,k,mask,val) \
        out_dword_masked_ns(HWIO_EE_n_GSI_CH_k_SCRATCH_3_ADDR(n,k),mask,val,HWIO_EE_n_GSI_CH_k_SCRATCH_3_INI2(n,k))
#define HWIO_EE_n_GSI_CH_k_SCRATCH_3_SCRATCH_BMSK                                  0xffffffff
#define HWIO_EE_n_GSI_CH_k_SCRATCH_3_SCRATCH_SHFT                                         0x0

#define HWIO_EE_n_GSI_CH_k_DB_ENG_WRITE_PTR_ADDR(n,k)                              (GSI_REG_BASE      + 0x0001c070 + 0x4000 * (n) + 0x80 * (k))
#define HWIO_EE_n_GSI_CH_k_DB_ENG_WRITE_PTR_PHYS(n,k)                              (GSI_REG_BASE_PHYS + 0x0001c070 + 0x4000 * (n) + 0x80 * (k))
#define HWIO_EE_n_GSI_CH_k_DB_ENG_WRITE_PTR_OFFS(n,k)                              (GSI_REG_BASE_OFFS + 0x0001c070 + 0x4000 * (n) + 0x80 * (k))
#define HWIO_EE_n_GSI_CH_k_DB_ENG_WRITE_PTR_RMSK                                       0xffff
#define HWIO_EE_n_GSI_CH_k_DB_ENG_WRITE_PTR_MAXn                                            2
#define HWIO_EE_n_GSI_CH_k_DB_ENG_WRITE_PTR_MAXk                                           19
#define HWIO_EE_n_GSI_CH_k_DB_ENG_WRITE_PTR_INI2(n,k)        \
        in_dword_masked(HWIO_EE_n_GSI_CH_k_DB_ENG_WRITE_PTR_ADDR(n,k), HWIO_EE_n_GSI_CH_k_DB_ENG_WRITE_PTR_RMSK)
#define HWIO_EE_n_GSI_CH_k_DB_ENG_WRITE_PTR_INMI2(n,k,mask)    \
        in_dword_masked(HWIO_EE_n_GSI_CH_k_DB_ENG_WRITE_PTR_ADDR(n,k), mask)
#define HWIO_EE_n_GSI_CH_k_DB_ENG_WRITE_PTR_OUTI2(n,k,val)    \
        out_dword(HWIO_EE_n_GSI_CH_k_DB_ENG_WRITE_PTR_ADDR(n,k),val)
#define HWIO_EE_n_GSI_CH_k_DB_ENG_WRITE_PTR_OUTMI2(n,k,mask,val) \
        out_dword_masked_ns(HWIO_EE_n_GSI_CH_k_DB_ENG_WRITE_PTR_ADDR(n,k),mask,val,HWIO_EE_n_GSI_CH_k_DB_ENG_WRITE_PTR_INI2(n,k))
#define HWIO_EE_n_GSI_CH_k_DB_ENG_WRITE_PTR_LAST_DB_2_MCS_BMSK                         0xffff
#define HWIO_EE_n_GSI_CH_k_DB_ENG_WRITE_PTR_LAST_DB_2_MCS_SHFT                            0x0

#define HWIO_EE_n_EV_CH_k_CNTXT_0_ADDR(n,k)                                        (GSI_REG_BASE      + 0x0001d000 + 0x4000 * (n) + 0x80 * (k))
#define HWIO_EE_n_EV_CH_k_CNTXT_0_PHYS(n,k)                                        (GSI_REG_BASE_PHYS + 0x0001d000 + 0x4000 * (n) + 0x80 * (k))
#define HWIO_EE_n_EV_CH_k_CNTXT_0_OFFS(n,k)                                        (GSI_REG_BASE_OFFS + 0x0001d000 + 0x4000 * (n) + 0x80 * (k))
#define HWIO_EE_n_EV_CH_k_CNTXT_0_RMSK                                             0xfff1ffff
#define HWIO_EE_n_EV_CH_k_CNTXT_0_MAXn                                                      2
#define HWIO_EE_n_EV_CH_k_CNTXT_0_MAXk                                                     13
#define HWIO_EE_n_EV_CH_k_CNTXT_0_INI2(n,k)        \
        in_dword_masked(HWIO_EE_n_EV_CH_k_CNTXT_0_ADDR(n,k), HWIO_EE_n_EV_CH_k_CNTXT_0_RMSK)
#define HWIO_EE_n_EV_CH_k_CNTXT_0_INMI2(n,k,mask)    \
        in_dword_masked(HWIO_EE_n_EV_CH_k_CNTXT_0_ADDR(n,k), mask)
#define HWIO_EE_n_EV_CH_k_CNTXT_0_OUTI2(n,k,val)    \
        out_dword(HWIO_EE_n_EV_CH_k_CNTXT_0_ADDR(n,k),val)
#define HWIO_EE_n_EV_CH_k_CNTXT_0_OUTMI2(n,k,mask,val) \
        out_dword_masked_ns(HWIO_EE_n_EV_CH_k_CNTXT_0_ADDR(n,k),mask,val,HWIO_EE_n_EV_CH_k_CNTXT_0_INI2(n,k))
#define HWIO_EE_n_EV_CH_k_CNTXT_0_ELEMENT_SIZE_BMSK                                0xff000000
#define HWIO_EE_n_EV_CH_k_CNTXT_0_ELEMENT_SIZE_SHFT                                      0x18
#define HWIO_EE_n_EV_CH_k_CNTXT_0_CHSTATE_BMSK                                       0xf00000
#define HWIO_EE_n_EV_CH_k_CNTXT_0_CHSTATE_SHFT                                           0x14
#define HWIO_EE_n_EV_CH_k_CNTXT_0_CHSTATE_NOT_ALLOCATED_FVAL                              0x0
#define HWIO_EE_n_EV_CH_k_CNTXT_0_CHSTATE_ALLOCATED_FVAL                                  0x1
#define HWIO_EE_n_EV_CH_k_CNTXT_0_INTYPE_BMSK                                         0x10000
#define HWIO_EE_n_EV_CH_k_CNTXT_0_INTYPE_SHFT                                            0x10
#define HWIO_EE_n_EV_CH_k_CNTXT_0_INTYPE_MSI_FVAL                                         0x0
#define HWIO_EE_n_EV_CH_k_CNTXT_0_INTYPE_IRQ_FVAL                                         0x1
#define HWIO_EE_n_EV_CH_k_CNTXT_0_EVCHID_BMSK                                          0xff00
#define HWIO_EE_n_EV_CH_k_CNTXT_0_EVCHID_SHFT                                             0x8
#define HWIO_EE_n_EV_CH_k_CNTXT_0_EE_BMSK                                                0xf0
#define HWIO_EE_n_EV_CH_k_CNTXT_0_EE_SHFT                                                 0x4
#define HWIO_EE_n_EV_CH_k_CNTXT_0_CHTYPE_BMSK                                             0xf
#define HWIO_EE_n_EV_CH_k_CNTXT_0_CHTYPE_SHFT                                             0x0
#define HWIO_EE_n_EV_CH_k_CNTXT_0_CHTYPE_MHI_EV_FVAL                                      0x0
#define HWIO_EE_n_EV_CH_k_CNTXT_0_CHTYPE_XHCI_EV_FVAL                                     0x1
#define HWIO_EE_n_EV_CH_k_CNTXT_0_CHTYPE_GPI_EV_FVAL                                      0x2
#define HWIO_EE_n_EV_CH_k_CNTXT_0_CHTYPE_XDCI_FVAL                                        0x3

#define HWIO_EE_n_EV_CH_k_CNTXT_1_ADDR(n,k)                                        (GSI_REG_BASE      + 0x0001d004 + 0x4000 * (n) + 0x80 * (k))
#define HWIO_EE_n_EV_CH_k_CNTXT_1_PHYS(n,k)                                        (GSI_REG_BASE_PHYS + 0x0001d004 + 0x4000 * (n) + 0x80 * (k))
#define HWIO_EE_n_EV_CH_k_CNTXT_1_OFFS(n,k)                                        (GSI_REG_BASE_OFFS + 0x0001d004 + 0x4000 * (n) + 0x80 * (k))
#define HWIO_EE_n_EV_CH_k_CNTXT_1_RMSK                                                 0xffff
#define HWIO_EE_n_EV_CH_k_CNTXT_1_MAXn                                                      2
#define HWIO_EE_n_EV_CH_k_CNTXT_1_MAXk                                                     13
#define HWIO_EE_n_EV_CH_k_CNTXT_1_INI2(n,k)        \
        in_dword_masked(HWIO_EE_n_EV_CH_k_CNTXT_1_ADDR(n,k), HWIO_EE_n_EV_CH_k_CNTXT_1_RMSK)
#define HWIO_EE_n_EV_CH_k_CNTXT_1_INMI2(n,k,mask)    \
        in_dword_masked(HWIO_EE_n_EV_CH_k_CNTXT_1_ADDR(n,k), mask)
#define HWIO_EE_n_EV_CH_k_CNTXT_1_OUTI2(n,k,val)    \
        out_dword(HWIO_EE_n_EV_CH_k_CNTXT_1_ADDR(n,k),val)
#define HWIO_EE_n_EV_CH_k_CNTXT_1_OUTMI2(n,k,mask,val) \
        out_dword_masked_ns(HWIO_EE_n_EV_CH_k_CNTXT_1_ADDR(n,k),mask,val,HWIO_EE_n_EV_CH_k_CNTXT_1_INI2(n,k))
#define HWIO_EE_n_EV_CH_k_CNTXT_1_R_LENGTH_BMSK                                        0xffff
#define HWIO_EE_n_EV_CH_k_CNTXT_1_R_LENGTH_SHFT                                           0x0

#define HWIO_EE_n_EV_CH_k_CNTXT_2_ADDR(n,k)                                        (GSI_REG_BASE      + 0x0001d008 + 0x4000 * (n) + 0x80 * (k))
#define HWIO_EE_n_EV_CH_k_CNTXT_2_PHYS(n,k)                                        (GSI_REG_BASE_PHYS + 0x0001d008 + 0x4000 * (n) + 0x80 * (k))
#define HWIO_EE_n_EV_CH_k_CNTXT_2_OFFS(n,k)                                        (GSI_REG_BASE_OFFS + 0x0001d008 + 0x4000 * (n) + 0x80 * (k))
#define HWIO_EE_n_EV_CH_k_CNTXT_2_RMSK                                             0xffffffff
#define HWIO_EE_n_EV_CH_k_CNTXT_2_MAXn                                                      2
#define HWIO_EE_n_EV_CH_k_CNTXT_2_MAXk                                                     13
#define HWIO_EE_n_EV_CH_k_CNTXT_2_INI2(n,k)        \
        in_dword_masked(HWIO_EE_n_EV_CH_k_CNTXT_2_ADDR(n,k), HWIO_EE_n_EV_CH_k_CNTXT_2_RMSK)
#define HWIO_EE_n_EV_CH_k_CNTXT_2_INMI2(n,k,mask)    \
        in_dword_masked(HWIO_EE_n_EV_CH_k_CNTXT_2_ADDR(n,k), mask)
#define HWIO_EE_n_EV_CH_k_CNTXT_2_OUTI2(n,k,val)    \
        out_dword(HWIO_EE_n_EV_CH_k_CNTXT_2_ADDR(n,k),val)
#define HWIO_EE_n_EV_CH_k_CNTXT_2_OUTMI2(n,k,mask,val) \
        out_dword_masked_ns(HWIO_EE_n_EV_CH_k_CNTXT_2_ADDR(n,k),mask,val,HWIO_EE_n_EV_CH_k_CNTXT_2_INI2(n,k))
#define HWIO_EE_n_EV_CH_k_CNTXT_2_R_BASE_ADDR_LSBS_BMSK                            0xffffffff
#define HWIO_EE_n_EV_CH_k_CNTXT_2_R_BASE_ADDR_LSBS_SHFT                                   0x0

#define HWIO_EE_n_EV_CH_k_CNTXT_3_ADDR(n,k)                                        (GSI_REG_BASE      + 0x0001d00c + 0x4000 * (n) + 0x80 * (k))
#define HWIO_EE_n_EV_CH_k_CNTXT_3_PHYS(n,k)                                        (GSI_REG_BASE_PHYS + 0x0001d00c + 0x4000 * (n) + 0x80 * (k))
#define HWIO_EE_n_EV_CH_k_CNTXT_3_OFFS(n,k)                                        (GSI_REG_BASE_OFFS + 0x0001d00c + 0x4000 * (n) + 0x80 * (k))
#define HWIO_EE_n_EV_CH_k_CNTXT_3_RMSK                                             0xffffffff
#define HWIO_EE_n_EV_CH_k_CNTXT_3_MAXn                                                      2
#define HWIO_EE_n_EV_CH_k_CNTXT_3_MAXk                                                     13
#define HWIO_EE_n_EV_CH_k_CNTXT_3_INI2(n,k)        \
        in_dword_masked(HWIO_EE_n_EV_CH_k_CNTXT_3_ADDR(n,k), HWIO_EE_n_EV_CH_k_CNTXT_3_RMSK)
#define HWIO_EE_n_EV_CH_k_CNTXT_3_INMI2(n,k,mask)    \
        in_dword_masked(HWIO_EE_n_EV_CH_k_CNTXT_3_ADDR(n,k), mask)
#define HWIO_EE_n_EV_CH_k_CNTXT_3_OUTI2(n,k,val)    \
        out_dword(HWIO_EE_n_EV_CH_k_CNTXT_3_ADDR(n,k),val)
#define HWIO_EE_n_EV_CH_k_CNTXT_3_OUTMI2(n,k,mask,val) \
        out_dword_masked_ns(HWIO_EE_n_EV_CH_k_CNTXT_3_ADDR(n,k),mask,val,HWIO_EE_n_EV_CH_k_CNTXT_3_INI2(n,k))
#define HWIO_EE_n_EV_CH_k_CNTXT_3_R_BASE_ADDR_MSBS_BMSK                            0xffffffff
#define HWIO_EE_n_EV_CH_k_CNTXT_3_R_BASE_ADDR_MSBS_SHFT                                   0x0

#define HWIO_EE_n_EV_CH_k_CNTXT_4_ADDR(n,k)                                        (GSI_REG_BASE      + 0x0001d010 + 0x4000 * (n) + 0x80 * (k))
#define HWIO_EE_n_EV_CH_k_CNTXT_4_PHYS(n,k)                                        (GSI_REG_BASE_PHYS + 0x0001d010 + 0x4000 * (n) + 0x80 * (k))
#define HWIO_EE_n_EV_CH_k_CNTXT_4_OFFS(n,k)                                        (GSI_REG_BASE_OFFS + 0x0001d010 + 0x4000 * (n) + 0x80 * (k))
#define HWIO_EE_n_EV_CH_k_CNTXT_4_RMSK                                             0xffffffff
#define HWIO_EE_n_EV_CH_k_CNTXT_4_MAXn                                                      2
#define HWIO_EE_n_EV_CH_k_CNTXT_4_MAXk                                                     13
#define HWIO_EE_n_EV_CH_k_CNTXT_4_INI2(n,k)        \
        in_dword_masked(HWIO_EE_n_EV_CH_k_CNTXT_4_ADDR(n,k), HWIO_EE_n_EV_CH_k_CNTXT_4_RMSK)
#define HWIO_EE_n_EV_CH_k_CNTXT_4_INMI2(n,k,mask)    \
        in_dword_masked(HWIO_EE_n_EV_CH_k_CNTXT_4_ADDR(n,k), mask)
#define HWIO_EE_n_EV_CH_k_CNTXT_4_OUTI2(n,k,val)    \
        out_dword(HWIO_EE_n_EV_CH_k_CNTXT_4_ADDR(n,k),val)
#define HWIO_EE_n_EV_CH_k_CNTXT_4_OUTMI2(n,k,mask,val) \
        out_dword_masked_ns(HWIO_EE_n_EV_CH_k_CNTXT_4_ADDR(n,k),mask,val,HWIO_EE_n_EV_CH_k_CNTXT_4_INI2(n,k))
#define HWIO_EE_n_EV_CH_k_CNTXT_4_READ_PTR_LSB_BMSK                                0xffffffff
#define HWIO_EE_n_EV_CH_k_CNTXT_4_READ_PTR_LSB_SHFT                                       0x0

#define HWIO_EE_n_EV_CH_k_CNTXT_5_ADDR(n,k)                                        (GSI_REG_BASE      + 0x0001d014 + 0x4000 * (n) + 0x80 * (k))
#define HWIO_EE_n_EV_CH_k_CNTXT_5_PHYS(n,k)                                        (GSI_REG_BASE_PHYS + 0x0001d014 + 0x4000 * (n) + 0x80 * (k))
#define HWIO_EE_n_EV_CH_k_CNTXT_5_OFFS(n,k)                                        (GSI_REG_BASE_OFFS + 0x0001d014 + 0x4000 * (n) + 0x80 * (k))
#define HWIO_EE_n_EV_CH_k_CNTXT_5_RMSK                                             0xffffffff
#define HWIO_EE_n_EV_CH_k_CNTXT_5_MAXn                                                      2
#define HWIO_EE_n_EV_CH_k_CNTXT_5_MAXk                                                     13
#define HWIO_EE_n_EV_CH_k_CNTXT_5_INI2(n,k)        \
        in_dword_masked(HWIO_EE_n_EV_CH_k_CNTXT_5_ADDR(n,k), HWIO_EE_n_EV_CH_k_CNTXT_5_RMSK)
#define HWIO_EE_n_EV_CH_k_CNTXT_5_INMI2(n,k,mask)    \
        in_dword_masked(HWIO_EE_n_EV_CH_k_CNTXT_5_ADDR(n,k), mask)
#define HWIO_EE_n_EV_CH_k_CNTXT_5_READ_PTR_MSB_BMSK                                0xffffffff
#define HWIO_EE_n_EV_CH_k_CNTXT_5_READ_PTR_MSB_SHFT                                       0x0

#define HWIO_EE_n_EV_CH_k_CNTXT_6_ADDR(n,k)                                        (GSI_REG_BASE      + 0x0001d018 + 0x4000 * (n) + 0x80 * (k))
#define HWIO_EE_n_EV_CH_k_CNTXT_6_PHYS(n,k)                                        (GSI_REG_BASE_PHYS + 0x0001d018 + 0x4000 * (n) + 0x80 * (k))
#define HWIO_EE_n_EV_CH_k_CNTXT_6_OFFS(n,k)                                        (GSI_REG_BASE_OFFS + 0x0001d018 + 0x4000 * (n) + 0x80 * (k))
#define HWIO_EE_n_EV_CH_k_CNTXT_6_RMSK                                             0xffffffff
#define HWIO_EE_n_EV_CH_k_CNTXT_6_MAXn                                                      2
#define HWIO_EE_n_EV_CH_k_CNTXT_6_MAXk                                                     13
#define HWIO_EE_n_EV_CH_k_CNTXT_6_INI2(n,k)        \
        in_dword_masked(HWIO_EE_n_EV_CH_k_CNTXT_6_ADDR(n,k), HWIO_EE_n_EV_CH_k_CNTXT_6_RMSK)
#define HWIO_EE_n_EV_CH_k_CNTXT_6_INMI2(n,k,mask)    \
        in_dword_masked(HWIO_EE_n_EV_CH_k_CNTXT_6_ADDR(n,k), mask)
#define HWIO_EE_n_EV_CH_k_CNTXT_6_WRITE_PTR_LSB_BMSK                               0xffffffff
#define HWIO_EE_n_EV_CH_k_CNTXT_6_WRITE_PTR_LSB_SHFT                                      0x0

#define HWIO_EE_n_EV_CH_k_CNTXT_7_ADDR(n,k)                                        (GSI_REG_BASE      + 0x0001d01c + 0x4000 * (n) + 0x80 * (k))
#define HWIO_EE_n_EV_CH_k_CNTXT_7_PHYS(n,k)                                        (GSI_REG_BASE_PHYS + 0x0001d01c + 0x4000 * (n) + 0x80 * (k))
#define HWIO_EE_n_EV_CH_k_CNTXT_7_OFFS(n,k)                                        (GSI_REG_BASE_OFFS + 0x0001d01c + 0x4000 * (n) + 0x80 * (k))
#define HWIO_EE_n_EV_CH_k_CNTXT_7_RMSK                                             0xffffffff
#define HWIO_EE_n_EV_CH_k_CNTXT_7_MAXn                                                      2
#define HWIO_EE_n_EV_CH_k_CNTXT_7_MAXk                                                     13
#define HWIO_EE_n_EV_CH_k_CNTXT_7_INI2(n,k)        \
        in_dword_masked(HWIO_EE_n_EV_CH_k_CNTXT_7_ADDR(n,k), HWIO_EE_n_EV_CH_k_CNTXT_7_RMSK)
#define HWIO_EE_n_EV_CH_k_CNTXT_7_INMI2(n,k,mask)    \
        in_dword_masked(HWIO_EE_n_EV_CH_k_CNTXT_7_ADDR(n,k), mask)
#define HWIO_EE_n_EV_CH_k_CNTXT_7_WRITE_PTR_MSB_BMSK                               0xffffffff
#define HWIO_EE_n_EV_CH_k_CNTXT_7_WRITE_PTR_MSB_SHFT                                      0x0

#define HWIO_EE_n_EV_CH_k_CNTXT_8_ADDR(n,k)                                        (GSI_REG_BASE      + 0x0001d020 + 0x4000 * (n) + 0x80 * (k))
#define HWIO_EE_n_EV_CH_k_CNTXT_8_PHYS(n,k)                                        (GSI_REG_BASE_PHYS + 0x0001d020 + 0x4000 * (n) + 0x80 * (k))
#define HWIO_EE_n_EV_CH_k_CNTXT_8_OFFS(n,k)                                        (GSI_REG_BASE_OFFS + 0x0001d020 + 0x4000 * (n) + 0x80 * (k))
#define HWIO_EE_n_EV_CH_k_CNTXT_8_RMSK                                             0xffffffff
#define HWIO_EE_n_EV_CH_k_CNTXT_8_MAXn                                                      2
#define HWIO_EE_n_EV_CH_k_CNTXT_8_MAXk                                                     13
#define HWIO_EE_n_EV_CH_k_CNTXT_8_INI2(n,k)        \
        in_dword_masked(HWIO_EE_n_EV_CH_k_CNTXT_8_ADDR(n,k), HWIO_EE_n_EV_CH_k_CNTXT_8_RMSK)
#define HWIO_EE_n_EV_CH_k_CNTXT_8_INMI2(n,k,mask)    \
        in_dword_masked(HWIO_EE_n_EV_CH_k_CNTXT_8_ADDR(n,k), mask)
#define HWIO_EE_n_EV_CH_k_CNTXT_8_OUTI2(n,k,val)    \
        out_dword(HWIO_EE_n_EV_CH_k_CNTXT_8_ADDR(n,k),val)
#define HWIO_EE_n_EV_CH_k_CNTXT_8_OUTMI2(n,k,mask,val) \
        out_dword_masked_ns(HWIO_EE_n_EV_CH_k_CNTXT_8_ADDR(n,k),mask,val,HWIO_EE_n_EV_CH_k_CNTXT_8_INI2(n,k))
#define HWIO_EE_n_EV_CH_k_CNTXT_8_INT_MOD_CNT_BMSK                                 0xff000000
#define HWIO_EE_n_EV_CH_k_CNTXT_8_INT_MOD_CNT_SHFT                                       0x18
#define HWIO_EE_n_EV_CH_k_CNTXT_8_INT_MODC_BMSK                                      0xff0000
#define HWIO_EE_n_EV_CH_k_CNTXT_8_INT_MODC_SHFT                                          0x10
#define HWIO_EE_n_EV_CH_k_CNTXT_8_INT_MODT_BMSK                                        0xffff
#define HWIO_EE_n_EV_CH_k_CNTXT_8_INT_MODT_SHFT                                           0x0

#define HWIO_EE_n_EV_CH_k_CNTXT_9_ADDR(n,k)                                        (GSI_REG_BASE      + 0x0001d024 + 0x4000 * (n) + 0x80 * (k))
#define HWIO_EE_n_EV_CH_k_CNTXT_9_PHYS(n,k)                                        (GSI_REG_BASE_PHYS + 0x0001d024 + 0x4000 * (n) + 0x80 * (k))
#define HWIO_EE_n_EV_CH_k_CNTXT_9_OFFS(n,k)                                        (GSI_REG_BASE_OFFS + 0x0001d024 + 0x4000 * (n) + 0x80 * (k))
#define HWIO_EE_n_EV_CH_k_CNTXT_9_RMSK                                             0xffffffff
#define HWIO_EE_n_EV_CH_k_CNTXT_9_MAXn                                                      2
#define HWIO_EE_n_EV_CH_k_CNTXT_9_MAXk                                                     13
#define HWIO_EE_n_EV_CH_k_CNTXT_9_INI2(n,k)        \
        in_dword_masked(HWIO_EE_n_EV_CH_k_CNTXT_9_ADDR(n,k), HWIO_EE_n_EV_CH_k_CNTXT_9_RMSK)
#define HWIO_EE_n_EV_CH_k_CNTXT_9_INMI2(n,k,mask)    \
        in_dword_masked(HWIO_EE_n_EV_CH_k_CNTXT_9_ADDR(n,k), mask)
#define HWIO_EE_n_EV_CH_k_CNTXT_9_OUTI2(n,k,val)    \
        out_dword(HWIO_EE_n_EV_CH_k_CNTXT_9_ADDR(n,k),val)
#define HWIO_EE_n_EV_CH_k_CNTXT_9_OUTMI2(n,k,mask,val) \
        out_dword_masked_ns(HWIO_EE_n_EV_CH_k_CNTXT_9_ADDR(n,k),mask,val,HWIO_EE_n_EV_CH_k_CNTXT_9_INI2(n,k))
#define HWIO_EE_n_EV_CH_k_CNTXT_9_INTVEC_BMSK                                      0xffffffff
#define HWIO_EE_n_EV_CH_k_CNTXT_9_INTVEC_SHFT                                             0x0

#define HWIO_EE_n_EV_CH_k_CNTXT_10_ADDR(n,k)                                       (GSI_REG_BASE      + 0x0001d028 + 0x4000 * (n) + 0x80 * (k))
#define HWIO_EE_n_EV_CH_k_CNTXT_10_PHYS(n,k)                                       (GSI_REG_BASE_PHYS + 0x0001d028 + 0x4000 * (n) + 0x80 * (k))
#define HWIO_EE_n_EV_CH_k_CNTXT_10_OFFS(n,k)                                       (GSI_REG_BASE_OFFS + 0x0001d028 + 0x4000 * (n) + 0x80 * (k))
#define HWIO_EE_n_EV_CH_k_CNTXT_10_RMSK                                            0xffffffff
#define HWIO_EE_n_EV_CH_k_CNTXT_10_MAXn                                                     2
#define HWIO_EE_n_EV_CH_k_CNTXT_10_MAXk                                                    13
#define HWIO_EE_n_EV_CH_k_CNTXT_10_INI2(n,k)        \
        in_dword_masked(HWIO_EE_n_EV_CH_k_CNTXT_10_ADDR(n,k), HWIO_EE_n_EV_CH_k_CNTXT_10_RMSK)
#define HWIO_EE_n_EV_CH_k_CNTXT_10_INMI2(n,k,mask)    \
        in_dword_masked(HWIO_EE_n_EV_CH_k_CNTXT_10_ADDR(n,k), mask)
#define HWIO_EE_n_EV_CH_k_CNTXT_10_OUTI2(n,k,val)    \
        out_dword(HWIO_EE_n_EV_CH_k_CNTXT_10_ADDR(n,k),val)
#define HWIO_EE_n_EV_CH_k_CNTXT_10_OUTMI2(n,k,mask,val) \
        out_dword_masked_ns(HWIO_EE_n_EV_CH_k_CNTXT_10_ADDR(n,k),mask,val,HWIO_EE_n_EV_CH_k_CNTXT_10_INI2(n,k))
#define HWIO_EE_n_EV_CH_k_CNTXT_10_MSI_ADDR_LSB_BMSK                               0xffffffff
#define HWIO_EE_n_EV_CH_k_CNTXT_10_MSI_ADDR_LSB_SHFT                                      0x0

#define HWIO_EE_n_EV_CH_k_CNTXT_11_ADDR(n,k)                                       (GSI_REG_BASE      + 0x0001d02c + 0x4000 * (n) + 0x80 * (k))
#define HWIO_EE_n_EV_CH_k_CNTXT_11_PHYS(n,k)                                       (GSI_REG_BASE_PHYS + 0x0001d02c + 0x4000 * (n) + 0x80 * (k))
#define HWIO_EE_n_EV_CH_k_CNTXT_11_OFFS(n,k)                                       (GSI_REG_BASE_OFFS + 0x0001d02c + 0x4000 * (n) + 0x80 * (k))
#define HWIO_EE_n_EV_CH_k_CNTXT_11_RMSK                                            0xffffffff
#define HWIO_EE_n_EV_CH_k_CNTXT_11_MAXn                                                     2
#define HWIO_EE_n_EV_CH_k_CNTXT_11_MAXk                                                    13
#define HWIO_EE_n_EV_CH_k_CNTXT_11_INI2(n,k)        \
        in_dword_masked(HWIO_EE_n_EV_CH_k_CNTXT_11_ADDR(n,k), HWIO_EE_n_EV_CH_k_CNTXT_11_RMSK)
#define HWIO_EE_n_EV_CH_k_CNTXT_11_INMI2(n,k,mask)    \
        in_dword_masked(HWIO_EE_n_EV_CH_k_CNTXT_11_ADDR(n,k), mask)
#define HWIO_EE_n_EV_CH_k_CNTXT_11_OUTI2(n,k,val)    \
        out_dword(HWIO_EE_n_EV_CH_k_CNTXT_11_ADDR(n,k),val)
#define HWIO_EE_n_EV_CH_k_CNTXT_11_OUTMI2(n,k,mask,val) \
        out_dword_masked_ns(HWIO_EE_n_EV_CH_k_CNTXT_11_ADDR(n,k),mask,val,HWIO_EE_n_EV_CH_k_CNTXT_11_INI2(n,k))
#define HWIO_EE_n_EV_CH_k_CNTXT_11_MSI_ADDR_MSB_BMSK                               0xffffffff
#define HWIO_EE_n_EV_CH_k_CNTXT_11_MSI_ADDR_MSB_SHFT                                      0x0

#define HWIO_EE_n_EV_CH_k_CNTXT_12_ADDR(n,k)                                       (GSI_REG_BASE      + 0x0001d030 + 0x4000 * (n) + 0x80 * (k))
#define HWIO_EE_n_EV_CH_k_CNTXT_12_PHYS(n,k)                                       (GSI_REG_BASE_PHYS + 0x0001d030 + 0x4000 * (n) + 0x80 * (k))
#define HWIO_EE_n_EV_CH_k_CNTXT_12_OFFS(n,k)                                       (GSI_REG_BASE_OFFS + 0x0001d030 + 0x4000 * (n) + 0x80 * (k))
#define HWIO_EE_n_EV_CH_k_CNTXT_12_RMSK                                            0xffffffff
#define HWIO_EE_n_EV_CH_k_CNTXT_12_MAXn                                                     2
#define HWIO_EE_n_EV_CH_k_CNTXT_12_MAXk                                                    13
#define HWIO_EE_n_EV_CH_k_CNTXT_12_INI2(n,k)        \
        in_dword_masked(HWIO_EE_n_EV_CH_k_CNTXT_12_ADDR(n,k), HWIO_EE_n_EV_CH_k_CNTXT_12_RMSK)
#define HWIO_EE_n_EV_CH_k_CNTXT_12_INMI2(n,k,mask)    \
        in_dword_masked(HWIO_EE_n_EV_CH_k_CNTXT_12_ADDR(n,k), mask)
#define HWIO_EE_n_EV_CH_k_CNTXT_12_OUTI2(n,k,val)    \
        out_dword(HWIO_EE_n_EV_CH_k_CNTXT_12_ADDR(n,k),val)
#define HWIO_EE_n_EV_CH_k_CNTXT_12_OUTMI2(n,k,mask,val) \
        out_dword_masked_ns(HWIO_EE_n_EV_CH_k_CNTXT_12_ADDR(n,k),mask,val,HWIO_EE_n_EV_CH_k_CNTXT_12_INI2(n,k))
#define HWIO_EE_n_EV_CH_k_CNTXT_12_RP_UPDATE_ADDR_LSB_BMSK                         0xffffffff
#define HWIO_EE_n_EV_CH_k_CNTXT_12_RP_UPDATE_ADDR_LSB_SHFT                                0x0

#define HWIO_EE_n_EV_CH_k_CNTXT_13_ADDR(n,k)                                       (GSI_REG_BASE      + 0x0001d034 + 0x4000 * (n) + 0x80 * (k))
#define HWIO_EE_n_EV_CH_k_CNTXT_13_PHYS(n,k)                                       (GSI_REG_BASE_PHYS + 0x0001d034 + 0x4000 * (n) + 0x80 * (k))
#define HWIO_EE_n_EV_CH_k_CNTXT_13_OFFS(n,k)                                       (GSI_REG_BASE_OFFS + 0x0001d034 + 0x4000 * (n) + 0x80 * (k))
#define HWIO_EE_n_EV_CH_k_CNTXT_13_RMSK                                            0xffffffff
#define HWIO_EE_n_EV_CH_k_CNTXT_13_MAXn                                                     2
#define HWIO_EE_n_EV_CH_k_CNTXT_13_MAXk                                                    13
#define HWIO_EE_n_EV_CH_k_CNTXT_13_INI2(n,k)        \
        in_dword_masked(HWIO_EE_n_EV_CH_k_CNTXT_13_ADDR(n,k), HWIO_EE_n_EV_CH_k_CNTXT_13_RMSK)
#define HWIO_EE_n_EV_CH_k_CNTXT_13_INMI2(n,k,mask)    \
        in_dword_masked(HWIO_EE_n_EV_CH_k_CNTXT_13_ADDR(n,k), mask)
#define HWIO_EE_n_EV_CH_k_CNTXT_13_OUTI2(n,k,val)    \
        out_dword(HWIO_EE_n_EV_CH_k_CNTXT_13_ADDR(n,k),val)
#define HWIO_EE_n_EV_CH_k_CNTXT_13_OUTMI2(n,k,mask,val) \
        out_dword_masked_ns(HWIO_EE_n_EV_CH_k_CNTXT_13_ADDR(n,k),mask,val,HWIO_EE_n_EV_CH_k_CNTXT_13_INI2(n,k))
#define HWIO_EE_n_EV_CH_k_CNTXT_13_RP_UPDATE_ADDR_MSB_BMSK                         0xffffffff
#define HWIO_EE_n_EV_CH_k_CNTXT_13_RP_UPDATE_ADDR_MSB_SHFT                                0x0

#define HWIO_EE_n_EV_CH_k_SCRATCH_0_ADDR(n,k)                                      (GSI_REG_BASE      + 0x0001d048 + 0x4000 * (n) + 0x80 * (k))
#define HWIO_EE_n_EV_CH_k_SCRATCH_0_PHYS(n,k)                                      (GSI_REG_BASE_PHYS + 0x0001d048 + 0x4000 * (n) + 0x80 * (k))
#define HWIO_EE_n_EV_CH_k_SCRATCH_0_OFFS(n,k)                                      (GSI_REG_BASE_OFFS + 0x0001d048 + 0x4000 * (n) + 0x80 * (k))
#define HWIO_EE_n_EV_CH_k_SCRATCH_0_RMSK                                           0xffffffff
#define HWIO_EE_n_EV_CH_k_SCRATCH_0_MAXn                                                    2
#define HWIO_EE_n_EV_CH_k_SCRATCH_0_MAXk                                                   13
#define HWIO_EE_n_EV_CH_k_SCRATCH_0_INI2(n,k)        \
        in_dword_masked(HWIO_EE_n_EV_CH_k_SCRATCH_0_ADDR(n,k), HWIO_EE_n_EV_CH_k_SCRATCH_0_RMSK)
#define HWIO_EE_n_EV_CH_k_SCRATCH_0_INMI2(n,k,mask)    \
        in_dword_masked(HWIO_EE_n_EV_CH_k_SCRATCH_0_ADDR(n,k), mask)
#define HWIO_EE_n_EV_CH_k_SCRATCH_0_OUTI2(n,k,val)    \
        out_dword(HWIO_EE_n_EV_CH_k_SCRATCH_0_ADDR(n,k),val)
#define HWIO_EE_n_EV_CH_k_SCRATCH_0_OUTMI2(n,k,mask,val) \
        out_dword_masked_ns(HWIO_EE_n_EV_CH_k_SCRATCH_0_ADDR(n,k),mask,val,HWIO_EE_n_EV_CH_k_SCRATCH_0_INI2(n,k))
#define HWIO_EE_n_EV_CH_k_SCRATCH_0_SCRATCH_BMSK                                   0xffffffff
#define HWIO_EE_n_EV_CH_k_SCRATCH_0_SCRATCH_SHFT                                          0x0

#define HWIO_EE_n_EV_CH_k_SCRATCH_1_ADDR(n,k)                                      (GSI_REG_BASE      + 0x0001d04c + 0x4000 * (n) + 0x80 * (k))
#define HWIO_EE_n_EV_CH_k_SCRATCH_1_PHYS(n,k)                                      (GSI_REG_BASE_PHYS + 0x0001d04c + 0x4000 * (n) + 0x80 * (k))
#define HWIO_EE_n_EV_CH_k_SCRATCH_1_OFFS(n,k)                                      (GSI_REG_BASE_OFFS + 0x0001d04c + 0x4000 * (n) + 0x80 * (k))
#define HWIO_EE_n_EV_CH_k_SCRATCH_1_RMSK                                           0xffffffff
#define HWIO_EE_n_EV_CH_k_SCRATCH_1_MAXn                                                    2
#define HWIO_EE_n_EV_CH_k_SCRATCH_1_MAXk                                                   13
#define HWIO_EE_n_EV_CH_k_SCRATCH_1_INI2(n,k)        \
        in_dword_masked(HWIO_EE_n_EV_CH_k_SCRATCH_1_ADDR(n,k), HWIO_EE_n_EV_CH_k_SCRATCH_1_RMSK)
#define HWIO_EE_n_EV_CH_k_SCRATCH_1_INMI2(n,k,mask)    \
        in_dword_masked(HWIO_EE_n_EV_CH_k_SCRATCH_1_ADDR(n,k), mask)
#define HWIO_EE_n_EV_CH_k_SCRATCH_1_OUTI2(n,k,val)    \
        out_dword(HWIO_EE_n_EV_CH_k_SCRATCH_1_ADDR(n,k),val)
#define HWIO_EE_n_EV_CH_k_SCRATCH_1_OUTMI2(n,k,mask,val) \
        out_dword_masked_ns(HWIO_EE_n_EV_CH_k_SCRATCH_1_ADDR(n,k),mask,val,HWIO_EE_n_EV_CH_k_SCRATCH_1_INI2(n,k))
#define HWIO_EE_n_EV_CH_k_SCRATCH_1_SCRATCH_BMSK                                   0xffffffff
#define HWIO_EE_n_EV_CH_k_SCRATCH_1_SCRATCH_SHFT                                          0x0

#define HWIO_EE_n_GSI_CH_k_DOORBELL_0_ADDR(n,k)                                    (GSI_REG_BASE      + 0x0001e000 + 0x4000 * (n) + 0x8 * (k))
#define HWIO_EE_n_GSI_CH_k_DOORBELL_0_PHYS(n,k)                                    (GSI_REG_BASE_PHYS + 0x0001e000 + 0x4000 * (n) + 0x8 * (k))
#define HWIO_EE_n_GSI_CH_k_DOORBELL_0_OFFS(n,k)                                    (GSI_REG_BASE_OFFS + 0x0001e000 + 0x4000 * (n) + 0x8 * (k))
#define HWIO_EE_n_GSI_CH_k_DOORBELL_0_RMSK                                         0xffffffff
#define HWIO_EE_n_GSI_CH_k_DOORBELL_0_MAXn                                                  2
#define HWIO_EE_n_GSI_CH_k_DOORBELL_0_MAXk                                                 19
#define HWIO_EE_n_GSI_CH_k_DOORBELL_0_OUTI2(n,k,val)    \
        out_dword(HWIO_EE_n_GSI_CH_k_DOORBELL_0_ADDR(n,k),val)
#define HWIO_EE_n_GSI_CH_k_DOORBELL_0_WRITE_PTR_LSB_BMSK                           0xffffffff
#define HWIO_EE_n_GSI_CH_k_DOORBELL_0_WRITE_PTR_LSB_SHFT                                  0x0

#define HWIO_EE_n_GSI_CH_k_DOORBELL_1_ADDR(n,k)                                    (GSI_REG_BASE      + 0x0001e004 + 0x4000 * (n) + 0x8 * (k))
#define HWIO_EE_n_GSI_CH_k_DOORBELL_1_PHYS(n,k)                                    (GSI_REG_BASE_PHYS + 0x0001e004 + 0x4000 * (n) + 0x8 * (k))
#define HWIO_EE_n_GSI_CH_k_DOORBELL_1_OFFS(n,k)                                    (GSI_REG_BASE_OFFS + 0x0001e004 + 0x4000 * (n) + 0x8 * (k))
#define HWIO_EE_n_GSI_CH_k_DOORBELL_1_RMSK                                         0xffffffff
#define HWIO_EE_n_GSI_CH_k_DOORBELL_1_MAXn                                                  2
#define HWIO_EE_n_GSI_CH_k_DOORBELL_1_MAXk                                                 19
#define HWIO_EE_n_GSI_CH_k_DOORBELL_1_OUTI2(n,k,val)    \
        out_dword(HWIO_EE_n_GSI_CH_k_DOORBELL_1_ADDR(n,k),val)
#define HWIO_EE_n_GSI_CH_k_DOORBELL_1_WRITE_PTR_MSB_BMSK                           0xffffffff
#define HWIO_EE_n_GSI_CH_k_DOORBELL_1_WRITE_PTR_MSB_SHFT                                  0x0

#define HWIO_EE_n_EV_CH_k_DOORBELL_0_ADDR(n,k)                                     (GSI_REG_BASE      + 0x0001e100 + 0x4000 * (n) + 0x8 * (k))
#define HWIO_EE_n_EV_CH_k_DOORBELL_0_PHYS(n,k)                                     (GSI_REG_BASE_PHYS + 0x0001e100 + 0x4000 * (n) + 0x8 * (k))
#define HWIO_EE_n_EV_CH_k_DOORBELL_0_OFFS(n,k)                                     (GSI_REG_BASE_OFFS + 0x0001e100 + 0x4000 * (n) + 0x8 * (k))
#define HWIO_EE_n_EV_CH_k_DOORBELL_0_RMSK                                          0xffffffff
#define HWIO_EE_n_EV_CH_k_DOORBELL_0_MAXn                                                   2
#define HWIO_EE_n_EV_CH_k_DOORBELL_0_MAXk                                                  13
#define HWIO_EE_n_EV_CH_k_DOORBELL_0_OUTI2(n,k,val)    \
        out_dword(HWIO_EE_n_EV_CH_k_DOORBELL_0_ADDR(n,k),val)
#define HWIO_EE_n_EV_CH_k_DOORBELL_0_WRITE_PTR_LSB_BMSK                            0xffffffff
#define HWIO_EE_n_EV_CH_k_DOORBELL_0_WRITE_PTR_LSB_SHFT                                   0x0

#define HWIO_EE_n_EV_CH_k_DOORBELL_1_ADDR(n,k)                                     (GSI_REG_BASE      + 0x0001e104 + 0x4000 * (n) + 0x8 * (k))
#define HWIO_EE_n_EV_CH_k_DOORBELL_1_PHYS(n,k)                                     (GSI_REG_BASE_PHYS + 0x0001e104 + 0x4000 * (n) + 0x8 * (k))
#define HWIO_EE_n_EV_CH_k_DOORBELL_1_OFFS(n,k)                                     (GSI_REG_BASE_OFFS + 0x0001e104 + 0x4000 * (n) + 0x8 * (k))
#define HWIO_EE_n_EV_CH_k_DOORBELL_1_RMSK                                          0xffffffff
#define HWIO_EE_n_EV_CH_k_DOORBELL_1_MAXn                                                   2
#define HWIO_EE_n_EV_CH_k_DOORBELL_1_MAXk                                                  13
#define HWIO_EE_n_EV_CH_k_DOORBELL_1_OUTI2(n,k,val)    \
        out_dword(HWIO_EE_n_EV_CH_k_DOORBELL_1_ADDR(n,k),val)
#define HWIO_EE_n_EV_CH_k_DOORBELL_1_WRITE_PTR_MSB_BMSK                            0xffffffff
#define HWIO_EE_n_EV_CH_k_DOORBELL_1_WRITE_PTR_MSB_SHFT                                   0x0

#define HWIO_EE_n_GSI_STATUS_ADDR(n)                                               (GSI_REG_BASE      + 0x0001f000 + 0x4000 * (n))
#define HWIO_EE_n_GSI_STATUS_PHYS(n)                                               (GSI_REG_BASE_PHYS + 0x0001f000 + 0x4000 * (n))
#define HWIO_EE_n_GSI_STATUS_OFFS(n)                                               (GSI_REG_BASE_OFFS + 0x0001f000 + 0x4000 * (n))
#define HWIO_EE_n_GSI_STATUS_RMSK                                                         0x1
#define HWIO_EE_n_GSI_STATUS_MAXn                                                           2
#define HWIO_EE_n_GSI_STATUS_INI(n)        \
        in_dword_masked(HWIO_EE_n_GSI_STATUS_ADDR(n), HWIO_EE_n_GSI_STATUS_RMSK)
#define HWIO_EE_n_GSI_STATUS_INMI(n,mask)    \
        in_dword_masked(HWIO_EE_n_GSI_STATUS_ADDR(n), mask)
#define HWIO_EE_n_GSI_STATUS_ENABLED_BMSK                                                 0x1
#define HWIO_EE_n_GSI_STATUS_ENABLED_SHFT                                                 0x0

#define HWIO_EE_n_GSI_CH_CMD_ADDR(n)                                               (GSI_REG_BASE      + 0x0001f008 + 0x4000 * (n))
#define HWIO_EE_n_GSI_CH_CMD_PHYS(n)                                               (GSI_REG_BASE_PHYS + 0x0001f008 + 0x4000 * (n))
#define HWIO_EE_n_GSI_CH_CMD_OFFS(n)                                               (GSI_REG_BASE_OFFS + 0x0001f008 + 0x4000 * (n))
#define HWIO_EE_n_GSI_CH_CMD_RMSK                                                  0xff0000ff
#define HWIO_EE_n_GSI_CH_CMD_MAXn                                                           2
#define HWIO_EE_n_GSI_CH_CMD_OUTI(n,val)    \
        out_dword(HWIO_EE_n_GSI_CH_CMD_ADDR(n),val)
#define HWIO_EE_n_GSI_CH_CMD_OPCODE_BMSK                                           0xff000000
#define HWIO_EE_n_GSI_CH_CMD_OPCODE_SHFT                                                 0x18
#define HWIO_EE_n_GSI_CH_CMD_OPCODE_ALLOCATE_FVAL                                         0x0
#define HWIO_EE_n_GSI_CH_CMD_OPCODE_START_FVAL                                            0x1
#define HWIO_EE_n_GSI_CH_CMD_OPCODE_STOP_FVAL                                             0x2
#define HWIO_EE_n_GSI_CH_CMD_OPCODE_RESET_FVAL                                            0x9
#define HWIO_EE_n_GSI_CH_CMD_OPCODE_DE_ALLOC_FVAL                                         0xa
#define HWIO_EE_n_GSI_CH_CMD_OPCODE_DB_STOP_FVAL                                          0xb
#define HWIO_EE_n_GSI_CH_CMD_CHID_BMSK                                                   0xff
#define HWIO_EE_n_GSI_CH_CMD_CHID_SHFT                                                    0x0

#define HWIO_EE_n_EV_CH_CMD_ADDR(n)                                                (GSI_REG_BASE      + 0x0001f010 + 0x4000 * (n))
#define HWIO_EE_n_EV_CH_CMD_PHYS(n)                                                (GSI_REG_BASE_PHYS + 0x0001f010 + 0x4000 * (n))
#define HWIO_EE_n_EV_CH_CMD_OFFS(n)                                                (GSI_REG_BASE_OFFS + 0x0001f010 + 0x4000 * (n))
#define HWIO_EE_n_EV_CH_CMD_RMSK                                                   0xff0000ff
#define HWIO_EE_n_EV_CH_CMD_MAXn                                                            2
#define HWIO_EE_n_EV_CH_CMD_OUTI(n,val)    \
        out_dword(HWIO_EE_n_EV_CH_CMD_ADDR(n),val)
#define HWIO_EE_n_EV_CH_CMD_OPCODE_BMSK                                            0xff000000
#define HWIO_EE_n_EV_CH_CMD_OPCODE_SHFT                                                  0x18
#define HWIO_EE_n_EV_CH_CMD_OPCODE_ALLOCATE_FVAL                                          0x0
#define HWIO_EE_n_EV_CH_CMD_OPCODE_RESET_FVAL                                             0x9
#define HWIO_EE_n_EV_CH_CMD_OPCODE_DE_ALLOC_FVAL                                          0xa
#define HWIO_EE_n_EV_CH_CMD_CHID_BMSK                                                    0xff
#define HWIO_EE_n_EV_CH_CMD_CHID_SHFT                                                     0x0

#define HWIO_EE_n_GSI_EE_GENERIC_CMD_ADDR(n)                                       (GSI_REG_BASE      + 0x0001f018 + 0x4000 * (n))
#define HWIO_EE_n_GSI_EE_GENERIC_CMD_PHYS(n)                                       (GSI_REG_BASE_PHYS + 0x0001f018 + 0x4000 * (n))
#define HWIO_EE_n_GSI_EE_GENERIC_CMD_OFFS(n)                                       (GSI_REG_BASE_OFFS + 0x0001f018 + 0x4000 * (n))
#define HWIO_EE_n_GSI_EE_GENERIC_CMD_RMSK                                          0xffffffff
#define HWIO_EE_n_GSI_EE_GENERIC_CMD_MAXn                                                   2
#define HWIO_EE_n_GSI_EE_GENERIC_CMD_OUTI(n,val)    \
        out_dword(HWIO_EE_n_GSI_EE_GENERIC_CMD_ADDR(n),val)
#define HWIO_EE_n_GSI_EE_GENERIC_CMD_OPCODE_BMSK                                   0xffffffff
#define HWIO_EE_n_GSI_EE_GENERIC_CMD_OPCODE_SHFT                                          0x0

#define HWIO_EE_n_GSI_HW_PARAM_0_ADDR(n)                                           (GSI_REG_BASE      + 0x0001f038 + 0x4000 * (n))
#define HWIO_EE_n_GSI_HW_PARAM_0_PHYS(n)                                           (GSI_REG_BASE_PHYS + 0x0001f038 + 0x4000 * (n))
#define HWIO_EE_n_GSI_HW_PARAM_0_OFFS(n)                                           (GSI_REG_BASE_OFFS + 0x0001f038 + 0x4000 * (n))
#define HWIO_EE_n_GSI_HW_PARAM_0_RMSK                                              0xffffffff
#define HWIO_EE_n_GSI_HW_PARAM_0_MAXn                                                       2
#define HWIO_EE_n_GSI_HW_PARAM_0_INI(n)        \
        in_dword_masked(HWIO_EE_n_GSI_HW_PARAM_0_ADDR(n), HWIO_EE_n_GSI_HW_PARAM_0_RMSK)
#define HWIO_EE_n_GSI_HW_PARAM_0_INMI(n,mask)    \
        in_dword_masked(HWIO_EE_n_GSI_HW_PARAM_0_ADDR(n), mask)
#define HWIO_EE_n_GSI_HW_PARAM_0_USE_AXI_M_BMSK                                    0x80000000
#define HWIO_EE_n_GSI_HW_PARAM_0_USE_AXI_M_SHFT                                          0x1f
#define HWIO_EE_n_GSI_HW_PARAM_0_PERIPH_SEC_GRP_BMSK                               0x7c000000
#define HWIO_EE_n_GSI_HW_PARAM_0_PERIPH_SEC_GRP_SHFT                                     0x1a
#define HWIO_EE_n_GSI_HW_PARAM_0_PERIPH_CONF_ADDR_BUS_W_BMSK                        0x3e00000
#define HWIO_EE_n_GSI_HW_PARAM_0_PERIPH_CONF_ADDR_BUS_W_SHFT                             0x15
#define HWIO_EE_n_GSI_HW_PARAM_0_NUM_EES_BMSK                                        0x1f0000
#define HWIO_EE_n_GSI_HW_PARAM_0_NUM_EES_SHFT                                            0x10
#define HWIO_EE_n_GSI_HW_PARAM_0_GSI_CH_NUM_BMSK                                       0xff00
#define HWIO_EE_n_GSI_HW_PARAM_0_GSI_CH_NUM_SHFT                                          0x8
#define HWIO_EE_n_GSI_HW_PARAM_0_GSI_EV_CH_NUM_BMSK                                      0xff
#define HWIO_EE_n_GSI_HW_PARAM_0_GSI_EV_CH_NUM_SHFT                                       0x0

#define HWIO_EE_n_GSI_HW_PARAM_1_ADDR(n)                                           (GSI_REG_BASE      + 0x0001f040 + 0x4000 * (n))
#define HWIO_EE_n_GSI_HW_PARAM_1_PHYS(n)                                           (GSI_REG_BASE_PHYS + 0x0001f040 + 0x4000 * (n))
#define HWIO_EE_n_GSI_HW_PARAM_1_OFFS(n)                                           (GSI_REG_BASE_OFFS + 0x0001f040 + 0x4000 * (n))
#define HWIO_EE_n_GSI_HW_PARAM_1_RMSK                                              0xffffffff
#define HWIO_EE_n_GSI_HW_PARAM_1_MAXn                                                       2
#define HWIO_EE_n_GSI_HW_PARAM_1_INI(n)        \
        in_dword_masked(HWIO_EE_n_GSI_HW_PARAM_1_ADDR(n), HWIO_EE_n_GSI_HW_PARAM_1_RMSK)
#define HWIO_EE_n_GSI_HW_PARAM_1_INMI(n,mask)    \
        in_dword_masked(HWIO_EE_n_GSI_HW_PARAM_1_ADDR(n), mask)
#define HWIO_EE_n_GSI_HW_PARAM_1_GSI_BLK_INT_ACCESS_REGION_2_EN_BMSK               0x80000000
#define HWIO_EE_n_GSI_HW_PARAM_1_GSI_BLK_INT_ACCESS_REGION_2_EN_SHFT                     0x1f
#define HWIO_EE_n_GSI_HW_PARAM_1_GSI_BLK_INT_ACCESS_REGION_1_EN_BMSK               0x40000000
#define HWIO_EE_n_GSI_HW_PARAM_1_GSI_BLK_INT_ACCESS_REGION_1_EN_SHFT                     0x1e
#define HWIO_EE_n_GSI_HW_PARAM_1_GSI_SIMPLE_RD_WR_BMSK                             0x20000000
#define HWIO_EE_n_GSI_HW_PARAM_1_GSI_SIMPLE_RD_WR_SHFT                                   0x1d
#define HWIO_EE_n_GSI_HW_PARAM_1_GSI_ESCAPE_BUF_ONLY_BMSK                          0x10000000
#define HWIO_EE_n_GSI_HW_PARAM_1_GSI_ESCAPE_BUF_ONLY_SHFT                                0x1c
#define HWIO_EE_n_GSI_HW_PARAM_1_GSI_USE_UC_IF_BMSK                                 0x8000000
#define HWIO_EE_n_GSI_HW_PARAM_1_GSI_USE_UC_IF_SHFT                                      0x1b
#define HWIO_EE_n_GSI_HW_PARAM_1_GSI_USE_DB_ENG_BMSK                                0x4000000
#define HWIO_EE_n_GSI_HW_PARAM_1_GSI_USE_DB_ENG_SHFT                                     0x1a
#define HWIO_EE_n_GSI_HW_PARAM_1_GSI_USE_BP_MTRIX_BMSK                              0x2000000
#define HWIO_EE_n_GSI_HW_PARAM_1_GSI_USE_BP_MTRIX_SHFT                                   0x19
#define HWIO_EE_n_GSI_HW_PARAM_1_GSI_NUM_TIMERS_BMSK                                0x1f00000
#define HWIO_EE_n_GSI_HW_PARAM_1_GSI_NUM_TIMERS_SHFT                                     0x14
#define HWIO_EE_n_GSI_HW_PARAM_1_GSI_USE_XPU_BMSK                                     0x80000
#define HWIO_EE_n_GSI_HW_PARAM_1_GSI_USE_XPU_SHFT                                        0x13
#define HWIO_EE_n_GSI_HW_PARAM_1_GSI_QRIB_EN_BMSK                                     0x40000
#define HWIO_EE_n_GSI_HW_PARAM_1_GSI_QRIB_EN_SHFT                                        0x12
#define HWIO_EE_n_GSI_HW_PARAM_1_GSI_VMIDACR_EN_BMSK                                  0x20000
#define HWIO_EE_n_GSI_HW_PARAM_1_GSI_VMIDACR_EN_SHFT                                     0x11
#define HWIO_EE_n_GSI_HW_PARAM_1_GSI_SEC_EN_BMSK                                      0x10000
#define HWIO_EE_n_GSI_HW_PARAM_1_GSI_SEC_EN_SHFT                                         0x10
#define HWIO_EE_n_GSI_HW_PARAM_1_GSI_NONSEC_EN_BMSK                                    0xf000
#define HWIO_EE_n_GSI_HW_PARAM_1_GSI_NONSEC_EN_SHFT                                       0xc
#define HWIO_EE_n_GSI_HW_PARAM_1_GSI_NUM_QAD_BMSK                                       0xf00
#define HWIO_EE_n_GSI_HW_PARAM_1_GSI_NUM_QAD_SHFT                                         0x8
#define HWIO_EE_n_GSI_HW_PARAM_1_GSI_M_DATA_BUS_W_BMSK                                   0xff
#define HWIO_EE_n_GSI_HW_PARAM_1_GSI_M_DATA_BUS_W_SHFT                                    0x0

#define HWIO_EE_n_GSI_SW_VERSION_ADDR(n)                                           (GSI_REG_BASE      + 0x0001f044 + 0x4000 * (n))
#define HWIO_EE_n_GSI_SW_VERSION_PHYS(n)                                           (GSI_REG_BASE_PHYS + 0x0001f044 + 0x4000 * (n))
#define HWIO_EE_n_GSI_SW_VERSION_OFFS(n)                                           (GSI_REG_BASE_OFFS + 0x0001f044 + 0x4000 * (n))
#define HWIO_EE_n_GSI_SW_VERSION_RMSK                                              0xffffffff
#define HWIO_EE_n_GSI_SW_VERSION_MAXn                                                       2
#define HWIO_EE_n_GSI_SW_VERSION_INI(n)        \
        in_dword_masked(HWIO_EE_n_GSI_SW_VERSION_ADDR(n), HWIO_EE_n_GSI_SW_VERSION_RMSK)
#define HWIO_EE_n_GSI_SW_VERSION_INMI(n,mask)    \
        in_dword_masked(HWIO_EE_n_GSI_SW_VERSION_ADDR(n), mask)
#define HWIO_EE_n_GSI_SW_VERSION_MAJOR_BMSK                                        0xf0000000
#define HWIO_EE_n_GSI_SW_VERSION_MAJOR_SHFT                                              0x1c
#define HWIO_EE_n_GSI_SW_VERSION_MINOR_BMSK                                         0xfff0000
#define HWIO_EE_n_GSI_SW_VERSION_MINOR_SHFT                                              0x10
#define HWIO_EE_n_GSI_SW_VERSION_STEP_BMSK                                             0xffff
#define HWIO_EE_n_GSI_SW_VERSION_STEP_SHFT                                                0x0

#define HWIO_EE_n_GSI_MCS_CODE_VER_ADDR(n)                                         (GSI_REG_BASE      + 0x0001f048 + 0x4000 * (n))
#define HWIO_EE_n_GSI_MCS_CODE_VER_PHYS(n)                                         (GSI_REG_BASE_PHYS + 0x0001f048 + 0x4000 * (n))
#define HWIO_EE_n_GSI_MCS_CODE_VER_OFFS(n)                                         (GSI_REG_BASE_OFFS + 0x0001f048 + 0x4000 * (n))
#define HWIO_EE_n_GSI_MCS_CODE_VER_RMSK                                            0xffffffff
#define HWIO_EE_n_GSI_MCS_CODE_VER_MAXn                                                     2
#define HWIO_EE_n_GSI_MCS_CODE_VER_INI(n)        \
        in_dword_masked(HWIO_EE_n_GSI_MCS_CODE_VER_ADDR(n), HWIO_EE_n_GSI_MCS_CODE_VER_RMSK)
#define HWIO_EE_n_GSI_MCS_CODE_VER_INMI(n,mask)    \
        in_dword_masked(HWIO_EE_n_GSI_MCS_CODE_VER_ADDR(n), mask)
#define HWIO_EE_n_GSI_MCS_CODE_VER_VER_BMSK                                        0xffffffff
#define HWIO_EE_n_GSI_MCS_CODE_VER_VER_SHFT                                               0x0

#define HWIO_EE_n_CNTXT_TYPE_IRQ_ADDR(n)                                           (GSI_REG_BASE      + 0x0001f080 + 0x4000 * (n))
#define HWIO_EE_n_CNTXT_TYPE_IRQ_PHYS(n)                                           (GSI_REG_BASE_PHYS + 0x0001f080 + 0x4000 * (n))
#define HWIO_EE_n_CNTXT_TYPE_IRQ_OFFS(n)                                           (GSI_REG_BASE_OFFS + 0x0001f080 + 0x4000 * (n))
#define HWIO_EE_n_CNTXT_TYPE_IRQ_RMSK                                                    0x7f
#define HWIO_EE_n_CNTXT_TYPE_IRQ_MAXn                                                       2
#define HWIO_EE_n_CNTXT_TYPE_IRQ_INI(n)        \
        in_dword_masked(HWIO_EE_n_CNTXT_TYPE_IRQ_ADDR(n), HWIO_EE_n_CNTXT_TYPE_IRQ_RMSK)
#define HWIO_EE_n_CNTXT_TYPE_IRQ_INMI(n,mask)    \
        in_dword_masked(HWIO_EE_n_CNTXT_TYPE_IRQ_ADDR(n), mask)
#define HWIO_EE_n_CNTXT_TYPE_IRQ_GENERAL_BMSK                                            0x40
#define HWIO_EE_n_CNTXT_TYPE_IRQ_GENERAL_SHFT                                             0x6
#define HWIO_EE_n_CNTXT_TYPE_IRQ_INTER_EE_EV_CTRL_BMSK                                   0x20
#define HWIO_EE_n_CNTXT_TYPE_IRQ_INTER_EE_EV_CTRL_SHFT                                    0x5
#define HWIO_EE_n_CNTXT_TYPE_IRQ_INTER_EE_CH_CTRL_BMSK                                   0x10
#define HWIO_EE_n_CNTXT_TYPE_IRQ_INTER_EE_CH_CTRL_SHFT                                    0x4
#define HWIO_EE_n_CNTXT_TYPE_IRQ_IEOB_BMSK                                                0x8
#define HWIO_EE_n_CNTXT_TYPE_IRQ_IEOB_SHFT                                                0x3
#define HWIO_EE_n_CNTXT_TYPE_IRQ_GLOB_EE_BMSK                                             0x4
#define HWIO_EE_n_CNTXT_TYPE_IRQ_GLOB_EE_SHFT                                             0x2
#define HWIO_EE_n_CNTXT_TYPE_IRQ_EV_CTRL_BMSK                                             0x2
#define HWIO_EE_n_CNTXT_TYPE_IRQ_EV_CTRL_SHFT                                             0x1
#define HWIO_EE_n_CNTXT_TYPE_IRQ_CH_CTRL_BMSK                                             0x1
#define HWIO_EE_n_CNTXT_TYPE_IRQ_CH_CTRL_SHFT                                             0x0

#define HWIO_EE_n_CNTXT_TYPE_IRQ_MSK_ADDR(n)                                       (GSI_REG_BASE      + 0x0001f088 + 0x4000 * (n))
#define HWIO_EE_n_CNTXT_TYPE_IRQ_MSK_PHYS(n)                                       (GSI_REG_BASE_PHYS + 0x0001f088 + 0x4000 * (n))
#define HWIO_EE_n_CNTXT_TYPE_IRQ_MSK_OFFS(n)                                       (GSI_REG_BASE_OFFS + 0x0001f088 + 0x4000 * (n))
#define HWIO_EE_n_CNTXT_TYPE_IRQ_MSK_RMSK                                                0x7f
#define HWIO_EE_n_CNTXT_TYPE_IRQ_MSK_MAXn                                                   2
#define HWIO_EE_n_CNTXT_TYPE_IRQ_MSK_INI(n)        \
        in_dword_masked(HWIO_EE_n_CNTXT_TYPE_IRQ_MSK_ADDR(n), HWIO_EE_n_CNTXT_TYPE_IRQ_MSK_RMSK)
#define HWIO_EE_n_CNTXT_TYPE_IRQ_MSK_INMI(n,mask)    \
        in_dword_masked(HWIO_EE_n_CNTXT_TYPE_IRQ_MSK_ADDR(n), mask)
#define HWIO_EE_n_CNTXT_TYPE_IRQ_MSK_OUTI(n,val)    \
        out_dword(HWIO_EE_n_CNTXT_TYPE_IRQ_MSK_ADDR(n),val)
#define HWIO_EE_n_CNTXT_TYPE_IRQ_MSK_OUTMI(n,mask,val) \
        out_dword_masked_ns(HWIO_EE_n_CNTXT_TYPE_IRQ_MSK_ADDR(n),mask,val,HWIO_EE_n_CNTXT_TYPE_IRQ_MSK_INI(n))
#define HWIO_EE_n_CNTXT_TYPE_IRQ_MSK_GENERAL_BMSK                                        0x40
#define HWIO_EE_n_CNTXT_TYPE_IRQ_MSK_GENERAL_SHFT                                         0x6
#define HWIO_EE_n_CNTXT_TYPE_IRQ_MSK_INTER_EE_EV_CTRL_BMSK                               0x20
#define HWIO_EE_n_CNTXT_TYPE_IRQ_MSK_INTER_EE_EV_CTRL_SHFT                                0x5
#define HWIO_EE_n_CNTXT_TYPE_IRQ_MSK_INTER_EE_CH_CTRL_BMSK                               0x10
#define HWIO_EE_n_CNTXT_TYPE_IRQ_MSK_INTER_EE_CH_CTRL_SHFT                                0x4
#define HWIO_EE_n_CNTXT_TYPE_IRQ_MSK_IEOB_BMSK                                            0x8
#define HWIO_EE_n_CNTXT_TYPE_IRQ_MSK_IEOB_SHFT                                            0x3
#define HWIO_EE_n_CNTXT_TYPE_IRQ_MSK_GLOB_EE_BMSK                                         0x4
#define HWIO_EE_n_CNTXT_TYPE_IRQ_MSK_GLOB_EE_SHFT                                         0x2
#define HWIO_EE_n_CNTXT_TYPE_IRQ_MSK_EV_CTRL_BMSK                                         0x2
#define HWIO_EE_n_CNTXT_TYPE_IRQ_MSK_EV_CTRL_SHFT                                         0x1
#define HWIO_EE_n_CNTXT_TYPE_IRQ_MSK_CH_CTRL_BMSK                                         0x1
#define HWIO_EE_n_CNTXT_TYPE_IRQ_MSK_CH_CTRL_SHFT                                         0x0

#define HWIO_EE_n_CNTXT_SRC_GSI_CH_IRQ_ADDR(n)                                     (GSI_REG_BASE      + 0x0001f090 + 0x4000 * (n))
#define HWIO_EE_n_CNTXT_SRC_GSI_CH_IRQ_PHYS(n)                                     (GSI_REG_BASE_PHYS + 0x0001f090 + 0x4000 * (n))
#define HWIO_EE_n_CNTXT_SRC_GSI_CH_IRQ_OFFS(n)                                     (GSI_REG_BASE_OFFS + 0x0001f090 + 0x4000 * (n))
#define HWIO_EE_n_CNTXT_SRC_GSI_CH_IRQ_RMSK                                        0xffffffff
#define HWIO_EE_n_CNTXT_SRC_GSI_CH_IRQ_MAXn                                                 2
#define HWIO_EE_n_CNTXT_SRC_GSI_CH_IRQ_INI(n)        \
        in_dword_masked(HWIO_EE_n_CNTXT_SRC_GSI_CH_IRQ_ADDR(n), HWIO_EE_n_CNTXT_SRC_GSI_CH_IRQ_RMSK)
#define HWIO_EE_n_CNTXT_SRC_GSI_CH_IRQ_INMI(n,mask)    \
        in_dword_masked(HWIO_EE_n_CNTXT_SRC_GSI_CH_IRQ_ADDR(n), mask)
#define HWIO_EE_n_CNTXT_SRC_GSI_CH_IRQ_GSI_CH_BIT_MAP_BMSK                         0xffffffff
#define HWIO_EE_n_CNTXT_SRC_GSI_CH_IRQ_GSI_CH_BIT_MAP_SHFT                                0x0

#define HWIO_EE_n_CNTXT_SRC_EV_CH_IRQ_ADDR(n)                                      (GSI_REG_BASE      + 0x0001f094 + 0x4000 * (n))
#define HWIO_EE_n_CNTXT_SRC_EV_CH_IRQ_PHYS(n)                                      (GSI_REG_BASE_PHYS + 0x0001f094 + 0x4000 * (n))
#define HWIO_EE_n_CNTXT_SRC_EV_CH_IRQ_OFFS(n)                                      (GSI_REG_BASE_OFFS + 0x0001f094 + 0x4000 * (n))
#define HWIO_EE_n_CNTXT_SRC_EV_CH_IRQ_RMSK                                         0xffffffff
#define HWIO_EE_n_CNTXT_SRC_EV_CH_IRQ_MAXn                                                  2
#define HWIO_EE_n_CNTXT_SRC_EV_CH_IRQ_INI(n)        \
        in_dword_masked(HWIO_EE_n_CNTXT_SRC_EV_CH_IRQ_ADDR(n), HWIO_EE_n_CNTXT_SRC_EV_CH_IRQ_RMSK)
#define HWIO_EE_n_CNTXT_SRC_EV_CH_IRQ_INMI(n,mask)    \
        in_dword_masked(HWIO_EE_n_CNTXT_SRC_EV_CH_IRQ_ADDR(n), mask)
#define HWIO_EE_n_CNTXT_SRC_EV_CH_IRQ_EV_CH_BIT_MAP_BMSK                           0xffffffff
#define HWIO_EE_n_CNTXT_SRC_EV_CH_IRQ_EV_CH_BIT_MAP_SHFT                                  0x0

#define HWIO_EE_n_CNTXT_SRC_GSI_CH_IRQ_MSK_ADDR(n)                                 (GSI_REG_BASE      + 0x0001f098 + 0x4000 * (n))
#define HWIO_EE_n_CNTXT_SRC_GSI_CH_IRQ_MSK_PHYS(n)                                 (GSI_REG_BASE_PHYS + 0x0001f098 + 0x4000 * (n))
#define HWIO_EE_n_CNTXT_SRC_GSI_CH_IRQ_MSK_OFFS(n)                                 (GSI_REG_BASE_OFFS + 0x0001f098 + 0x4000 * (n))
#define HWIO_EE_n_CNTXT_SRC_GSI_CH_IRQ_MSK_RMSK                                    0xffffffff
#define HWIO_EE_n_CNTXT_SRC_GSI_CH_IRQ_MSK_MAXn                                             2
#define HWIO_EE_n_CNTXT_SRC_GSI_CH_IRQ_MSK_INI(n)        \
        in_dword_masked(HWIO_EE_n_CNTXT_SRC_GSI_CH_IRQ_MSK_ADDR(n), HWIO_EE_n_CNTXT_SRC_GSI_CH_IRQ_MSK_RMSK)
#define HWIO_EE_n_CNTXT_SRC_GSI_CH_IRQ_MSK_INMI(n,mask)    \
        in_dword_masked(HWIO_EE_n_CNTXT_SRC_GSI_CH_IRQ_MSK_ADDR(n), mask)
#define HWIO_EE_n_CNTXT_SRC_GSI_CH_IRQ_MSK_OUTI(n,val)    \
        out_dword(HWIO_EE_n_CNTXT_SRC_GSI_CH_IRQ_MSK_ADDR(n),val)
#define HWIO_EE_n_CNTXT_SRC_GSI_CH_IRQ_MSK_OUTMI(n,mask,val) \
        out_dword_masked_ns(HWIO_EE_n_CNTXT_SRC_GSI_CH_IRQ_MSK_ADDR(n),mask,val,HWIO_EE_n_CNTXT_SRC_GSI_CH_IRQ_MSK_INI(n))
#define HWIO_EE_n_CNTXT_SRC_GSI_CH_IRQ_MSK_GSI_CH_BIT_MAP_MSK_BMSK                 0xffffffff
#define HWIO_EE_n_CNTXT_SRC_GSI_CH_IRQ_MSK_GSI_CH_BIT_MAP_MSK_SHFT                        0x0

#define HWIO_EE_n_CNTXT_SRC_EV_CH_IRQ_MSK_ADDR(n)                                  (GSI_REG_BASE      + 0x0001f09c + 0x4000 * (n))
#define HWIO_EE_n_CNTXT_SRC_EV_CH_IRQ_MSK_PHYS(n)                                  (GSI_REG_BASE_PHYS + 0x0001f09c + 0x4000 * (n))
#define HWIO_EE_n_CNTXT_SRC_EV_CH_IRQ_MSK_OFFS(n)                                  (GSI_REG_BASE_OFFS + 0x0001f09c + 0x4000 * (n))
#define HWIO_EE_n_CNTXT_SRC_EV_CH_IRQ_MSK_RMSK                                     0xffffffff
#define HWIO_EE_n_CNTXT_SRC_EV_CH_IRQ_MSK_MAXn                                              2
#define HWIO_EE_n_CNTXT_SRC_EV_CH_IRQ_MSK_INI(n)        \
        in_dword_masked(HWIO_EE_n_CNTXT_SRC_EV_CH_IRQ_MSK_ADDR(n), HWIO_EE_n_CNTXT_SRC_EV_CH_IRQ_MSK_RMSK)
#define HWIO_EE_n_CNTXT_SRC_EV_CH_IRQ_MSK_INMI(n,mask)    \
        in_dword_masked(HWIO_EE_n_CNTXT_SRC_EV_CH_IRQ_MSK_ADDR(n), mask)
#define HWIO_EE_n_CNTXT_SRC_EV_CH_IRQ_MSK_OUTI(n,val)    \
        out_dword(HWIO_EE_n_CNTXT_SRC_EV_CH_IRQ_MSK_ADDR(n),val)
#define HWIO_EE_n_CNTXT_SRC_EV_CH_IRQ_MSK_OUTMI(n,mask,val) \
        out_dword_masked_ns(HWIO_EE_n_CNTXT_SRC_EV_CH_IRQ_MSK_ADDR(n),mask,val,HWIO_EE_n_CNTXT_SRC_EV_CH_IRQ_MSK_INI(n))
#define HWIO_EE_n_CNTXT_SRC_EV_CH_IRQ_MSK_EV_CH_BIT_MAP_MSK_BMSK                   0xffffffff
#define HWIO_EE_n_CNTXT_SRC_EV_CH_IRQ_MSK_EV_CH_BIT_MAP_MSK_SHFT                          0x0

#define HWIO_EE_n_CNTXT_SRC_GSI_CH_IRQ_CLR_ADDR(n)                                 (GSI_REG_BASE      + 0x0001f0a0 + 0x4000 * (n))
#define HWIO_EE_n_CNTXT_SRC_GSI_CH_IRQ_CLR_PHYS(n)                                 (GSI_REG_BASE_PHYS + 0x0001f0a0 + 0x4000 * (n))
#define HWIO_EE_n_CNTXT_SRC_GSI_CH_IRQ_CLR_OFFS(n)                                 (GSI_REG_BASE_OFFS + 0x0001f0a0 + 0x4000 * (n))
#define HWIO_EE_n_CNTXT_SRC_GSI_CH_IRQ_CLR_RMSK                                    0xffffffff
#define HWIO_EE_n_CNTXT_SRC_GSI_CH_IRQ_CLR_MAXn                                             2
#define HWIO_EE_n_CNTXT_SRC_GSI_CH_IRQ_CLR_OUTI(n,val)    \
        out_dword(HWIO_EE_n_CNTXT_SRC_GSI_CH_IRQ_CLR_ADDR(n),val)
#define HWIO_EE_n_CNTXT_SRC_GSI_CH_IRQ_CLR_GSI_CH_BIT_MAP_BMSK                     0xffffffff
#define HWIO_EE_n_CNTXT_SRC_GSI_CH_IRQ_CLR_GSI_CH_BIT_MAP_SHFT                            0x0

#define HWIO_EE_n_CNTXT_SRC_EV_CH_IRQ_CLR_ADDR(n)                                  (GSI_REG_BASE      + 0x0001f0a4 + 0x4000 * (n))
#define HWIO_EE_n_CNTXT_SRC_EV_CH_IRQ_CLR_PHYS(n)                                  (GSI_REG_BASE_PHYS + 0x0001f0a4 + 0x4000 * (n))
#define HWIO_EE_n_CNTXT_SRC_EV_CH_IRQ_CLR_OFFS(n)                                  (GSI_REG_BASE_OFFS + 0x0001f0a4 + 0x4000 * (n))
#define HWIO_EE_n_CNTXT_SRC_EV_CH_IRQ_CLR_RMSK                                     0xffffffff
#define HWIO_EE_n_CNTXT_SRC_EV_CH_IRQ_CLR_MAXn                                              2
#define HWIO_EE_n_CNTXT_SRC_EV_CH_IRQ_CLR_OUTI(n,val)    \
        out_dword(HWIO_EE_n_CNTXT_SRC_EV_CH_IRQ_CLR_ADDR(n),val)
#define HWIO_EE_n_CNTXT_SRC_EV_CH_IRQ_CLR_EV_CH_BIT_MAP_BMSK                       0xffffffff
#define HWIO_EE_n_CNTXT_SRC_EV_CH_IRQ_CLR_EV_CH_BIT_MAP_SHFT                              0x0

#define HWIO_EE_n_CNTXT_SRC_IEOB_IRQ_ADDR(n)                                       (GSI_REG_BASE      + 0x0001f0b0 + 0x4000 * (n))
#define HWIO_EE_n_CNTXT_SRC_IEOB_IRQ_PHYS(n)                                       (GSI_REG_BASE_PHYS + 0x0001f0b0 + 0x4000 * (n))
#define HWIO_EE_n_CNTXT_SRC_IEOB_IRQ_OFFS(n)                                       (GSI_REG_BASE_OFFS + 0x0001f0b0 + 0x4000 * (n))
#define HWIO_EE_n_CNTXT_SRC_IEOB_IRQ_RMSK                                          0xffffffff
#define HWIO_EE_n_CNTXT_SRC_IEOB_IRQ_MAXn                                                   2
#define HWIO_EE_n_CNTXT_SRC_IEOB_IRQ_INI(n)        \
        in_dword_masked(HWIO_EE_n_CNTXT_SRC_IEOB_IRQ_ADDR(n), HWIO_EE_n_CNTXT_SRC_IEOB_IRQ_RMSK)
#define HWIO_EE_n_CNTXT_SRC_IEOB_IRQ_INMI(n,mask)    \
        in_dword_masked(HWIO_EE_n_CNTXT_SRC_IEOB_IRQ_ADDR(n), mask)
#define HWIO_EE_n_CNTXT_SRC_IEOB_IRQ_EV_CH_BIT_MAP_BMSK                            0xffffffff
#define HWIO_EE_n_CNTXT_SRC_IEOB_IRQ_EV_CH_BIT_MAP_SHFT                                   0x0

#define HWIO_EE_n_CNTXT_SRC_IEOB_IRQ_MSK_ADDR(n)                                   (GSI_REG_BASE      + 0x0001f0b8 + 0x4000 * (n))
#define HWIO_EE_n_CNTXT_SRC_IEOB_IRQ_MSK_PHYS(n)                                   (GSI_REG_BASE_PHYS + 0x0001f0b8 + 0x4000 * (n))
#define HWIO_EE_n_CNTXT_SRC_IEOB_IRQ_MSK_OFFS(n)                                   (GSI_REG_BASE_OFFS + 0x0001f0b8 + 0x4000 * (n))
#define HWIO_EE_n_CNTXT_SRC_IEOB_IRQ_MSK_RMSK                                      0xffffffff
#define HWIO_EE_n_CNTXT_SRC_IEOB_IRQ_MSK_MAXn                                               2
#define HWIO_EE_n_CNTXT_SRC_IEOB_IRQ_MSK_INI(n)        \
        in_dword_masked(HWIO_EE_n_CNTXT_SRC_IEOB_IRQ_MSK_ADDR(n), HWIO_EE_n_CNTXT_SRC_IEOB_IRQ_MSK_RMSK)
#define HWIO_EE_n_CNTXT_SRC_IEOB_IRQ_MSK_INMI(n,mask)    \
        in_dword_masked(HWIO_EE_n_CNTXT_SRC_IEOB_IRQ_MSK_ADDR(n), mask)
#define HWIO_EE_n_CNTXT_SRC_IEOB_IRQ_MSK_OUTI(n,val)    \
        out_dword(HWIO_EE_n_CNTXT_SRC_IEOB_IRQ_MSK_ADDR(n),val)
#define HWIO_EE_n_CNTXT_SRC_IEOB_IRQ_MSK_OUTMI(n,mask,val) \
        out_dword_masked_ns(HWIO_EE_n_CNTXT_SRC_IEOB_IRQ_MSK_ADDR(n),mask,val,HWIO_EE_n_CNTXT_SRC_IEOB_IRQ_MSK_INI(n))
#define HWIO_EE_n_CNTXT_SRC_IEOB_IRQ_MSK_EV_CH_BIT_MAP_MSK_BMSK                    0xffffffff
#define HWIO_EE_n_CNTXT_SRC_IEOB_IRQ_MSK_EV_CH_BIT_MAP_MSK_SHFT                           0x0

#define HWIO_EE_n_CNTXT_SRC_IEOB_IRQ_CLR_ADDR(n)                                   (GSI_REG_BASE      + 0x0001f0c0 + 0x4000 * (n))
#define HWIO_EE_n_CNTXT_SRC_IEOB_IRQ_CLR_PHYS(n)                                   (GSI_REG_BASE_PHYS + 0x0001f0c0 + 0x4000 * (n))
#define HWIO_EE_n_CNTXT_SRC_IEOB_IRQ_CLR_OFFS(n)                                   (GSI_REG_BASE_OFFS + 0x0001f0c0 + 0x4000 * (n))
#define HWIO_EE_n_CNTXT_SRC_IEOB_IRQ_CLR_RMSK                                      0xffffffff
#define HWIO_EE_n_CNTXT_SRC_IEOB_IRQ_CLR_MAXn                                               2
#define HWIO_EE_n_CNTXT_SRC_IEOB_IRQ_CLR_OUTI(n,val)    \
        out_dword(HWIO_EE_n_CNTXT_SRC_IEOB_IRQ_CLR_ADDR(n),val)
#define HWIO_EE_n_CNTXT_SRC_IEOB_IRQ_CLR_EV_CH_BIT_MAP_BMSK                        0xffffffff
#define HWIO_EE_n_CNTXT_SRC_IEOB_IRQ_CLR_EV_CH_BIT_MAP_SHFT                               0x0

#define HWIO_EE_n_CNTXT_GLOB_IRQ_STTS_ADDR(n)                                      (GSI_REG_BASE      + 0x0001f100 + 0x4000 * (n))
#define HWIO_EE_n_CNTXT_GLOB_IRQ_STTS_PHYS(n)                                      (GSI_REG_BASE_PHYS + 0x0001f100 + 0x4000 * (n))
#define HWIO_EE_n_CNTXT_GLOB_IRQ_STTS_OFFS(n)                                      (GSI_REG_BASE_OFFS + 0x0001f100 + 0x4000 * (n))
#define HWIO_EE_n_CNTXT_GLOB_IRQ_STTS_RMSK                                                0xf
#define HWIO_EE_n_CNTXT_GLOB_IRQ_STTS_MAXn                                                  2
#define HWIO_EE_n_CNTXT_GLOB_IRQ_STTS_INI(n)        \
        in_dword_masked(HWIO_EE_n_CNTXT_GLOB_IRQ_STTS_ADDR(n), HWIO_EE_n_CNTXT_GLOB_IRQ_STTS_RMSK)
#define HWIO_EE_n_CNTXT_GLOB_IRQ_STTS_INMI(n,mask)    \
        in_dword_masked(HWIO_EE_n_CNTXT_GLOB_IRQ_STTS_ADDR(n), mask)
#define HWIO_EE_n_CNTXT_GLOB_IRQ_STTS_GP_INT3_BMSK                                        0x8
#define HWIO_EE_n_CNTXT_GLOB_IRQ_STTS_GP_INT3_SHFT                                        0x3
#define HWIO_EE_n_CNTXT_GLOB_IRQ_STTS_GP_INT2_BMSK                                        0x4
#define HWIO_EE_n_CNTXT_GLOB_IRQ_STTS_GP_INT2_SHFT                                        0x2
#define HWIO_EE_n_CNTXT_GLOB_IRQ_STTS_GP_INT1_BMSK                                        0x2
#define HWIO_EE_n_CNTXT_GLOB_IRQ_STTS_GP_INT1_SHFT                                        0x1
#define HWIO_EE_n_CNTXT_GLOB_IRQ_STTS_ERROR_INT_BMSK                                      0x1
#define HWIO_EE_n_CNTXT_GLOB_IRQ_STTS_ERROR_INT_SHFT                                      0x0

#define HWIO_EE_n_CNTXT_GLOB_IRQ_EN_ADDR(n)                                        (GSI_REG_BASE      + 0x0001f108 + 0x4000 * (n))
#define HWIO_EE_n_CNTXT_GLOB_IRQ_EN_PHYS(n)                                        (GSI_REG_BASE_PHYS + 0x0001f108 + 0x4000 * (n))
#define HWIO_EE_n_CNTXT_GLOB_IRQ_EN_OFFS(n)                                        (GSI_REG_BASE_OFFS + 0x0001f108 + 0x4000 * (n))
#define HWIO_EE_n_CNTXT_GLOB_IRQ_EN_RMSK                                                  0xf
#define HWIO_EE_n_CNTXT_GLOB_IRQ_EN_MAXn                                                    2
#define HWIO_EE_n_CNTXT_GLOB_IRQ_EN_INI(n)        \
        in_dword_masked(HWIO_EE_n_CNTXT_GLOB_IRQ_EN_ADDR(n), HWIO_EE_n_CNTXT_GLOB_IRQ_EN_RMSK)
#define HWIO_EE_n_CNTXT_GLOB_IRQ_EN_INMI(n,mask)    \
        in_dword_masked(HWIO_EE_n_CNTXT_GLOB_IRQ_EN_ADDR(n), mask)
#define HWIO_EE_n_CNTXT_GLOB_IRQ_EN_OUTI(n,val)    \
        out_dword(HWIO_EE_n_CNTXT_GLOB_IRQ_EN_ADDR(n),val)
#define HWIO_EE_n_CNTXT_GLOB_IRQ_EN_OUTMI(n,mask,val) \
        out_dword_masked_ns(HWIO_EE_n_CNTXT_GLOB_IRQ_EN_ADDR(n),mask,val,HWIO_EE_n_CNTXT_GLOB_IRQ_EN_INI(n))
#define HWIO_EE_n_CNTXT_GLOB_IRQ_EN_GP_INT3_BMSK                                          0x8
#define HWIO_EE_n_CNTXT_GLOB_IRQ_EN_GP_INT3_SHFT                                          0x3
#define HWIO_EE_n_CNTXT_GLOB_IRQ_EN_GP_INT2_BMSK                                          0x4
#define HWIO_EE_n_CNTXT_GLOB_IRQ_EN_GP_INT2_SHFT                                          0x2
#define HWIO_EE_n_CNTXT_GLOB_IRQ_EN_GP_INT1_BMSK                                          0x2
#define HWIO_EE_n_CNTXT_GLOB_IRQ_EN_GP_INT1_SHFT                                          0x1
#define HWIO_EE_n_CNTXT_GLOB_IRQ_EN_ERROR_INT_BMSK                                        0x1
#define HWIO_EE_n_CNTXT_GLOB_IRQ_EN_ERROR_INT_SHFT                                        0x0

#define HWIO_EE_n_CNTXT_GLOB_IRQ_CLR_ADDR(n)                                       (GSI_REG_BASE      + 0x0001f110 + 0x4000 * (n))
#define HWIO_EE_n_CNTXT_GLOB_IRQ_CLR_PHYS(n)                                       (GSI_REG_BASE_PHYS + 0x0001f110 + 0x4000 * (n))
#define HWIO_EE_n_CNTXT_GLOB_IRQ_CLR_OFFS(n)                                       (GSI_REG_BASE_OFFS + 0x0001f110 + 0x4000 * (n))
#define HWIO_EE_n_CNTXT_GLOB_IRQ_CLR_RMSK                                                 0xf
#define HWIO_EE_n_CNTXT_GLOB_IRQ_CLR_MAXn                                                   2
#define HWIO_EE_n_CNTXT_GLOB_IRQ_CLR_OUTI(n,val)    \
        out_dword(HWIO_EE_n_CNTXT_GLOB_IRQ_CLR_ADDR(n),val)
#define HWIO_EE_n_CNTXT_GLOB_IRQ_CLR_GP_INT3_BMSK                                         0x8
#define HWIO_EE_n_CNTXT_GLOB_IRQ_CLR_GP_INT3_SHFT                                         0x3
#define HWIO_EE_n_CNTXT_GLOB_IRQ_CLR_GP_INT2_BMSK                                         0x4
#define HWIO_EE_n_CNTXT_GLOB_IRQ_CLR_GP_INT2_SHFT                                         0x2
#define HWIO_EE_n_CNTXT_GLOB_IRQ_CLR_GP_INT1_BMSK                                         0x2
#define HWIO_EE_n_CNTXT_GLOB_IRQ_CLR_GP_INT1_SHFT                                         0x1
#define HWIO_EE_n_CNTXT_GLOB_IRQ_CLR_ERROR_INT_BMSK                                       0x1
#define HWIO_EE_n_CNTXT_GLOB_IRQ_CLR_ERROR_INT_SHFT                                       0x0

#define HWIO_EE_n_CNTXT_GSI_IRQ_STTS_ADDR(n)                                       (GSI_REG_BASE      + 0x0001f118 + 0x4000 * (n))
#define HWIO_EE_n_CNTXT_GSI_IRQ_STTS_PHYS(n)                                       (GSI_REG_BASE_PHYS + 0x0001f118 + 0x4000 * (n))
#define HWIO_EE_n_CNTXT_GSI_IRQ_STTS_OFFS(n)                                       (GSI_REG_BASE_OFFS + 0x0001f118 + 0x4000 * (n))
#define HWIO_EE_n_CNTXT_GSI_IRQ_STTS_RMSK                                                 0xf
#define HWIO_EE_n_CNTXT_GSI_IRQ_STTS_MAXn                                                   2
#define HWIO_EE_n_CNTXT_GSI_IRQ_STTS_INI(n)        \
        in_dword_masked(HWIO_EE_n_CNTXT_GSI_IRQ_STTS_ADDR(n), HWIO_EE_n_CNTXT_GSI_IRQ_STTS_RMSK)
#define HWIO_EE_n_CNTXT_GSI_IRQ_STTS_INMI(n,mask)    \
        in_dword_masked(HWIO_EE_n_CNTXT_GSI_IRQ_STTS_ADDR(n), mask)
#define HWIO_EE_n_CNTXT_GSI_IRQ_STTS_GSI_MCS_STACK_OVRFLOW_BMSK                           0x8
#define HWIO_EE_n_CNTXT_GSI_IRQ_STTS_GSI_MCS_STACK_OVRFLOW_SHFT                           0x3
#define HWIO_EE_n_CNTXT_GSI_IRQ_STTS_GSI_CMD_FIFO_OVRFLOW_BMSK                            0x4
#define HWIO_EE_n_CNTXT_GSI_IRQ_STTS_GSI_CMD_FIFO_OVRFLOW_SHFT                            0x2
#define HWIO_EE_n_CNTXT_GSI_IRQ_STTS_GSI_BUS_ERROR_BMSK                                   0x2
#define HWIO_EE_n_CNTXT_GSI_IRQ_STTS_GSI_BUS_ERROR_SHFT                                   0x1
#define HWIO_EE_n_CNTXT_GSI_IRQ_STTS_GSI_BREAK_POINT_BMSK                                 0x1
#define HWIO_EE_n_CNTXT_GSI_IRQ_STTS_GSI_BREAK_POINT_SHFT                                 0x0

#define HWIO_EE_n_CNTXT_GSI_IRQ_EN_ADDR(n)                                         (GSI_REG_BASE      + 0x0001f120 + 0x4000 * (n))
#define HWIO_EE_n_CNTXT_GSI_IRQ_EN_PHYS(n)                                         (GSI_REG_BASE_PHYS + 0x0001f120 + 0x4000 * (n))
#define HWIO_EE_n_CNTXT_GSI_IRQ_EN_OFFS(n)                                         (GSI_REG_BASE_OFFS + 0x0001f120 + 0x4000 * (n))
#define HWIO_EE_n_CNTXT_GSI_IRQ_EN_RMSK                                                   0xf
#define HWIO_EE_n_CNTXT_GSI_IRQ_EN_MAXn                                                     2
#define HWIO_EE_n_CNTXT_GSI_IRQ_EN_INI(n)        \
        in_dword_masked(HWIO_EE_n_CNTXT_GSI_IRQ_EN_ADDR(n), HWIO_EE_n_CNTXT_GSI_IRQ_EN_RMSK)
#define HWIO_EE_n_CNTXT_GSI_IRQ_EN_INMI(n,mask)    \
        in_dword_masked(HWIO_EE_n_CNTXT_GSI_IRQ_EN_ADDR(n), mask)
#define HWIO_EE_n_CNTXT_GSI_IRQ_EN_OUTI(n,val)    \
        out_dword(HWIO_EE_n_CNTXT_GSI_IRQ_EN_ADDR(n),val)
#define HWIO_EE_n_CNTXT_GSI_IRQ_EN_OUTMI(n,mask,val) \
        out_dword_masked_ns(HWIO_EE_n_CNTXT_GSI_IRQ_EN_ADDR(n),mask,val,HWIO_EE_n_CNTXT_GSI_IRQ_EN_INI(n))
#define HWIO_EE_n_CNTXT_GSI_IRQ_EN_GSI_MCS_STACK_OVRFLOW_BMSK                             0x8
#define HWIO_EE_n_CNTXT_GSI_IRQ_EN_GSI_MCS_STACK_OVRFLOW_SHFT                             0x3
#define HWIO_EE_n_CNTXT_GSI_IRQ_EN_GSI_CMD_FIFO_OVRFLOW_BMSK                              0x4
#define HWIO_EE_n_CNTXT_GSI_IRQ_EN_GSI_CMD_FIFO_OVRFLOW_SHFT                              0x2
#define HWIO_EE_n_CNTXT_GSI_IRQ_EN_GSI_BUS_ERROR_BMSK                                     0x2
#define HWIO_EE_n_CNTXT_GSI_IRQ_EN_GSI_BUS_ERROR_SHFT                                     0x1
#define HWIO_EE_n_CNTXT_GSI_IRQ_EN_GSI_BREAK_POINT_BMSK                                   0x1
#define HWIO_EE_n_CNTXT_GSI_IRQ_EN_GSI_BREAK_POINT_SHFT                                   0x0

#define HWIO_EE_n_CNTXT_GSI_IRQ_CLR_ADDR(n)                                        (GSI_REG_BASE      + 0x0001f128 + 0x4000 * (n))
#define HWIO_EE_n_CNTXT_GSI_IRQ_CLR_PHYS(n)                                        (GSI_REG_BASE_PHYS + 0x0001f128 + 0x4000 * (n))
#define HWIO_EE_n_CNTXT_GSI_IRQ_CLR_OFFS(n)                                        (GSI_REG_BASE_OFFS + 0x0001f128 + 0x4000 * (n))
#define HWIO_EE_n_CNTXT_GSI_IRQ_CLR_RMSK                                                  0xf
#define HWIO_EE_n_CNTXT_GSI_IRQ_CLR_MAXn                                                    2
#define HWIO_EE_n_CNTXT_GSI_IRQ_CLR_OUTI(n,val)    \
        out_dword(HWIO_EE_n_CNTXT_GSI_IRQ_CLR_ADDR(n),val)
#define HWIO_EE_n_CNTXT_GSI_IRQ_CLR_GSI_MCS_STACK_OVRFLOW_BMSK                            0x8
#define HWIO_EE_n_CNTXT_GSI_IRQ_CLR_GSI_MCS_STACK_OVRFLOW_SHFT                            0x3
#define HWIO_EE_n_CNTXT_GSI_IRQ_CLR_GSI_CMD_FIFO_OVRFLOW_BMSK                             0x4
#define HWIO_EE_n_CNTXT_GSI_IRQ_CLR_GSI_CMD_FIFO_OVRFLOW_SHFT                             0x2
#define HWIO_EE_n_CNTXT_GSI_IRQ_CLR_GSI_BUS_ERROR_BMSK                                    0x2
#define HWIO_EE_n_CNTXT_GSI_IRQ_CLR_GSI_BUS_ERROR_SHFT                                    0x1
#define HWIO_EE_n_CNTXT_GSI_IRQ_CLR_GSI_BREAK_POINT_BMSK                                  0x1
#define HWIO_EE_n_CNTXT_GSI_IRQ_CLR_GSI_BREAK_POINT_SHFT                                  0x0

#define HWIO_EE_n_CNTXT_INTSET_ADDR(n)                                             (GSI_REG_BASE      + 0x0001f180 + 0x4000 * (n))
#define HWIO_EE_n_CNTXT_INTSET_PHYS(n)                                             (GSI_REG_BASE_PHYS + 0x0001f180 + 0x4000 * (n))
#define HWIO_EE_n_CNTXT_INTSET_OFFS(n)                                             (GSI_REG_BASE_OFFS + 0x0001f180 + 0x4000 * (n))
#define HWIO_EE_n_CNTXT_INTSET_RMSK                                                       0x1
#define HWIO_EE_n_CNTXT_INTSET_MAXn                                                         2
#define HWIO_EE_n_CNTXT_INTSET_INI(n)        \
        in_dword_masked(HWIO_EE_n_CNTXT_INTSET_ADDR(n), HWIO_EE_n_CNTXT_INTSET_RMSK)
#define HWIO_EE_n_CNTXT_INTSET_INMI(n,mask)    \
        in_dword_masked(HWIO_EE_n_CNTXT_INTSET_ADDR(n), mask)
#define HWIO_EE_n_CNTXT_INTSET_OUTI(n,val)    \
        out_dword(HWIO_EE_n_CNTXT_INTSET_ADDR(n),val)
#define HWIO_EE_n_CNTXT_INTSET_OUTMI(n,mask,val) \
        out_dword_masked_ns(HWIO_EE_n_CNTXT_INTSET_ADDR(n),mask,val,HWIO_EE_n_CNTXT_INTSET_INI(n))
#define HWIO_EE_n_CNTXT_INTSET_INTYPE_BMSK                                                0x1
#define HWIO_EE_n_CNTXT_INTSET_INTYPE_SHFT                                                0x0
#define HWIO_EE_n_CNTXT_INTSET_INTYPE_MSI_FVAL                                            0x0
#define HWIO_EE_n_CNTXT_INTSET_INTYPE_IRQ_FVAL                                            0x1

#define HWIO_EE_n_CNTXT_MSI_BASE_LSB_ADDR(n)                                       (GSI_REG_BASE      + 0x0001f188 + 0x4000 * (n))
#define HWIO_EE_n_CNTXT_MSI_BASE_LSB_PHYS(n)                                       (GSI_REG_BASE_PHYS + 0x0001f188 + 0x4000 * (n))
#define HWIO_EE_n_CNTXT_MSI_BASE_LSB_OFFS(n)                                       (GSI_REG_BASE_OFFS + 0x0001f188 + 0x4000 * (n))
#define HWIO_EE_n_CNTXT_MSI_BASE_LSB_RMSK                                          0xffffffff
#define HWIO_EE_n_CNTXT_MSI_BASE_LSB_MAXn                                                   2
#define HWIO_EE_n_CNTXT_MSI_BASE_LSB_INI(n)        \
        in_dword_masked(HWIO_EE_n_CNTXT_MSI_BASE_LSB_ADDR(n), HWIO_EE_n_CNTXT_MSI_BASE_LSB_RMSK)
#define HWIO_EE_n_CNTXT_MSI_BASE_LSB_INMI(n,mask)    \
        in_dword_masked(HWIO_EE_n_CNTXT_MSI_BASE_LSB_ADDR(n), mask)
#define HWIO_EE_n_CNTXT_MSI_BASE_LSB_OUTI(n,val)    \
        out_dword(HWIO_EE_n_CNTXT_MSI_BASE_LSB_ADDR(n),val)
#define HWIO_EE_n_CNTXT_MSI_BASE_LSB_OUTMI(n,mask,val) \
        out_dword_masked_ns(HWIO_EE_n_CNTXT_MSI_BASE_LSB_ADDR(n),mask,val,HWIO_EE_n_CNTXT_MSI_BASE_LSB_INI(n))
#define HWIO_EE_n_CNTXT_MSI_BASE_LSB_MSI_ADDR_LSB_BMSK                             0xffffffff
#define HWIO_EE_n_CNTXT_MSI_BASE_LSB_MSI_ADDR_LSB_SHFT                                    0x0

#define HWIO_EE_n_CNTXT_MSI_BASE_MSB_ADDR(n)                                       (GSI_REG_BASE      + 0x0001f18c + 0x4000 * (n))
#define HWIO_EE_n_CNTXT_MSI_BASE_MSB_PHYS(n)                                       (GSI_REG_BASE_PHYS + 0x0001f18c + 0x4000 * (n))
#define HWIO_EE_n_CNTXT_MSI_BASE_MSB_OFFS(n)                                       (GSI_REG_BASE_OFFS + 0x0001f18c + 0x4000 * (n))
#define HWIO_EE_n_CNTXT_MSI_BASE_MSB_RMSK                                          0xffffffff
#define HWIO_EE_n_CNTXT_MSI_BASE_MSB_MAXn                                                   2
#define HWIO_EE_n_CNTXT_MSI_BASE_MSB_INI(n)        \
        in_dword_masked(HWIO_EE_n_CNTXT_MSI_BASE_MSB_ADDR(n), HWIO_EE_n_CNTXT_MSI_BASE_MSB_RMSK)
#define HWIO_EE_n_CNTXT_MSI_BASE_MSB_INMI(n,mask)    \
        in_dword_masked(HWIO_EE_n_CNTXT_MSI_BASE_MSB_ADDR(n), mask)
#define HWIO_EE_n_CNTXT_MSI_BASE_MSB_OUTI(n,val)    \
        out_dword(HWIO_EE_n_CNTXT_MSI_BASE_MSB_ADDR(n),val)
#define HWIO_EE_n_CNTXT_MSI_BASE_MSB_OUTMI(n,mask,val) \
        out_dword_masked_ns(HWIO_EE_n_CNTXT_MSI_BASE_MSB_ADDR(n),mask,val,HWIO_EE_n_CNTXT_MSI_BASE_MSB_INI(n))
#define HWIO_EE_n_CNTXT_MSI_BASE_MSB_MSI_ADDR_MSB_BMSK                             0xffffffff
#define HWIO_EE_n_CNTXT_MSI_BASE_MSB_MSI_ADDR_MSB_SHFT                                    0x0

#define HWIO_EE_n_CNTXT_INT_VEC_ADDR(n)                                            (GSI_REG_BASE      + 0x0001f190 + 0x4000 * (n))
#define HWIO_EE_n_CNTXT_INT_VEC_PHYS(n)                                            (GSI_REG_BASE_PHYS + 0x0001f190 + 0x4000 * (n))
#define HWIO_EE_n_CNTXT_INT_VEC_OFFS(n)                                            (GSI_REG_BASE_OFFS + 0x0001f190 + 0x4000 * (n))
#define HWIO_EE_n_CNTXT_INT_VEC_RMSK                                               0xffffffff
#define HWIO_EE_n_CNTXT_INT_VEC_MAXn                                                        2
#define HWIO_EE_n_CNTXT_INT_VEC_INI(n)        \
        in_dword_masked(HWIO_EE_n_CNTXT_INT_VEC_ADDR(n), HWIO_EE_n_CNTXT_INT_VEC_RMSK)
#define HWIO_EE_n_CNTXT_INT_VEC_INMI(n,mask)    \
        in_dword_masked(HWIO_EE_n_CNTXT_INT_VEC_ADDR(n), mask)
#define HWIO_EE_n_CNTXT_INT_VEC_OUTI(n,val)    \
        out_dword(HWIO_EE_n_CNTXT_INT_VEC_ADDR(n),val)
#define HWIO_EE_n_CNTXT_INT_VEC_OUTMI(n,mask,val) \
        out_dword_masked_ns(HWIO_EE_n_CNTXT_INT_VEC_ADDR(n),mask,val,HWIO_EE_n_CNTXT_INT_VEC_INI(n))
#define HWIO_EE_n_CNTXT_INT_VEC_INT_VEC_BMSK                                       0xffffffff
#define HWIO_EE_n_CNTXT_INT_VEC_INT_VEC_SHFT                                              0x0

#define HWIO_EE_n_ERROR_LOG_ADDR(n)                                                (GSI_REG_BASE      + 0x0001f200 + 0x4000 * (n))
#define HWIO_EE_n_ERROR_LOG_PHYS(n)                                                (GSI_REG_BASE_PHYS + 0x0001f200 + 0x4000 * (n))
#define HWIO_EE_n_ERROR_LOG_OFFS(n)                                                (GSI_REG_BASE_OFFS + 0x0001f200 + 0x4000 * (n))
#define HWIO_EE_n_ERROR_LOG_RMSK                                                   0xffffffff
#define HWIO_EE_n_ERROR_LOG_MAXn                                                            2
#define HWIO_EE_n_ERROR_LOG_INI(n)        \
        in_dword_masked(HWIO_EE_n_ERROR_LOG_ADDR(n), HWIO_EE_n_ERROR_LOG_RMSK)
#define HWIO_EE_n_ERROR_LOG_INMI(n,mask)    \
        in_dword_masked(HWIO_EE_n_ERROR_LOG_ADDR(n), mask)
#define HWIO_EE_n_ERROR_LOG_OUTI(n,val)    \
        out_dword(HWIO_EE_n_ERROR_LOG_ADDR(n),val)
#define HWIO_EE_n_ERROR_LOG_OUTMI(n,mask,val) \
        out_dword_masked_ns(HWIO_EE_n_ERROR_LOG_ADDR(n),mask,val,HWIO_EE_n_ERROR_LOG_INI(n))
#define HWIO_EE_n_ERROR_LOG_TODO_BMSK                                              0xffffffff
#define HWIO_EE_n_ERROR_LOG_TODO_SHFT                                                     0x0

#define HWIO_EE_n_ERROR_LOG_CLR_ADDR(n)                                            (GSI_REG_BASE      + 0x0001f210 + 0x4000 * (n))
#define HWIO_EE_n_ERROR_LOG_CLR_PHYS(n)                                            (GSI_REG_BASE_PHYS + 0x0001f210 + 0x4000 * (n))
#define HWIO_EE_n_ERROR_LOG_CLR_OFFS(n)                                            (GSI_REG_BASE_OFFS + 0x0001f210 + 0x4000 * (n))
#define HWIO_EE_n_ERROR_LOG_CLR_RMSK                                               0xffffffff
#define HWIO_EE_n_ERROR_LOG_CLR_MAXn                                                        2
#define HWIO_EE_n_ERROR_LOG_CLR_OUTI(n,val)    \
        out_dword(HWIO_EE_n_ERROR_LOG_CLR_ADDR(n),val)
#define HWIO_EE_n_ERROR_LOG_CLR_TODO_BMSK                                          0xffffffff
#define HWIO_EE_n_ERROR_LOG_CLR_TODO_SHFT                                                 0x0

#define HWIO_EE_n_CNTXT_SCRATCH_0_ADDR(n)                                          (GSI_REG_BASE      + 0x0001f400 + 0x4000 * (n))
#define HWIO_EE_n_CNTXT_SCRATCH_0_PHYS(n)                                          (GSI_REG_BASE_PHYS + 0x0001f400 + 0x4000 * (n))
#define HWIO_EE_n_CNTXT_SCRATCH_0_OFFS(n)                                          (GSI_REG_BASE_OFFS + 0x0001f400 + 0x4000 * (n))
#define HWIO_EE_n_CNTXT_SCRATCH_0_RMSK                                             0xffffffff
#define HWIO_EE_n_CNTXT_SCRATCH_0_MAXn                                                      2
#define HWIO_EE_n_CNTXT_SCRATCH_0_INI(n)        \
        in_dword_masked(HWIO_EE_n_CNTXT_SCRATCH_0_ADDR(n), HWIO_EE_n_CNTXT_SCRATCH_0_RMSK)
#define HWIO_EE_n_CNTXT_SCRATCH_0_INMI(n,mask)    \
        in_dword_masked(HWIO_EE_n_CNTXT_SCRATCH_0_ADDR(n), mask)
#define HWIO_EE_n_CNTXT_SCRATCH_0_OUTI(n,val)    \
        out_dword(HWIO_EE_n_CNTXT_SCRATCH_0_ADDR(n),val)
#define HWIO_EE_n_CNTXT_SCRATCH_0_OUTMI(n,mask,val) \
        out_dword_masked_ns(HWIO_EE_n_CNTXT_SCRATCH_0_ADDR(n),mask,val,HWIO_EE_n_CNTXT_SCRATCH_0_INI(n))
#define HWIO_EE_n_CNTXT_SCRATCH_0_SCRATCH_BMSK                                     0xffffffff
#define HWIO_EE_n_CNTXT_SCRATCH_0_SCRATCH_SHFT                                            0x0

#define HWIO_EE_n_CNTXT_SCRATCH_1_ADDR(n)                                          (GSI_REG_BASE      + 0x0001f404 + 0x4000 * (n))
#define HWIO_EE_n_CNTXT_SCRATCH_1_PHYS(n)                                          (GSI_REG_BASE_PHYS + 0x0001f404 + 0x4000 * (n))
#define HWIO_EE_n_CNTXT_SCRATCH_1_OFFS(n)                                          (GSI_REG_BASE_OFFS + 0x0001f404 + 0x4000 * (n))
#define HWIO_EE_n_CNTXT_SCRATCH_1_RMSK                                             0xffffffff
#define HWIO_EE_n_CNTXT_SCRATCH_1_MAXn                                                      2
#define HWIO_EE_n_CNTXT_SCRATCH_1_INI(n)        \
        in_dword_masked(HWIO_EE_n_CNTXT_SCRATCH_1_ADDR(n), HWIO_EE_n_CNTXT_SCRATCH_1_RMSK)
#define HWIO_EE_n_CNTXT_SCRATCH_1_INMI(n,mask)    \
        in_dword_masked(HWIO_EE_n_CNTXT_SCRATCH_1_ADDR(n), mask)
#define HWIO_EE_n_CNTXT_SCRATCH_1_OUTI(n,val)    \
        out_dword(HWIO_EE_n_CNTXT_SCRATCH_1_ADDR(n),val)
#define HWIO_EE_n_CNTXT_SCRATCH_1_OUTMI(n,mask,val) \
        out_dword_masked_ns(HWIO_EE_n_CNTXT_SCRATCH_1_ADDR(n),mask,val,HWIO_EE_n_CNTXT_SCRATCH_1_INI(n))
#define HWIO_EE_n_CNTXT_SCRATCH_1_SCRATCH_BMSK                                     0xffffffff
#define HWIO_EE_n_CNTXT_SCRATCH_1_SCRATCH_SHFT                                            0x0

#define HWIO_GSI_MCS_CFG_ADDR                                                      (GSI_REG_BASE      + 0x0000b000)
#define HWIO_GSI_MCS_CFG_PHYS                                                      (GSI_REG_BASE_PHYS + 0x0000b000)
#define HWIO_GSI_MCS_CFG_OFFS                                                      (GSI_REG_BASE_OFFS + 0x0000b000)
#define HWIO_GSI_MCS_CFG_RMSK                                                             0x1
#define HWIO_GSI_MCS_CFG_ATTR                                                             0x3
#define HWIO_GSI_MCS_CFG_IN          \
        in_dword_masked(HWIO_GSI_MCS_CFG_ADDR, HWIO_GSI_MCS_CFG_RMSK)
#define HWIO_GSI_MCS_CFG_INM(m)      \
        in_dword_masked(HWIO_GSI_MCS_CFG_ADDR, m)
#define HWIO_GSI_MCS_CFG_OUT(v)      \
        out_dword(HWIO_GSI_MCS_CFG_ADDR,v)
#define HWIO_GSI_MCS_CFG_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GSI_MCS_CFG_ADDR,m,v,HWIO_GSI_MCS_CFG_IN)
#define HWIO_GSI_MCS_CFG_MCS_ENABLE_BMSK                                                  0x1
#define HWIO_GSI_MCS_CFG_MCS_ENABLE_SHFT                                                  0x0

#define HWIO_INTER_EE_n_ORIGINATOR_EE_ADDR(n)                                      (GSI_REG_BASE      + 0x0000c000 + 0x1000 * (n))
#define HWIO_INTER_EE_n_ORIGINATOR_EE_PHYS(n)                                      (GSI_REG_BASE_PHYS + 0x0000c000 + 0x1000 * (n))
#define HWIO_INTER_EE_n_ORIGINATOR_EE_OFFS(n)                                      (GSI_REG_BASE_OFFS + 0x0000c000 + 0x1000 * (n))
#define HWIO_INTER_EE_n_ORIGINATOR_EE_RMSK                                                0xf
#define HWIO_INTER_EE_n_ORIGINATOR_EE_MAXn                                                  2
#define HWIO_INTER_EE_n_ORIGINATOR_EE_INI(n)        \
        in_dword_masked(HWIO_INTER_EE_n_ORIGINATOR_EE_ADDR(n), HWIO_INTER_EE_n_ORIGINATOR_EE_RMSK)
#define HWIO_INTER_EE_n_ORIGINATOR_EE_INMI(n,mask)    \
        in_dword_masked(HWIO_INTER_EE_n_ORIGINATOR_EE_ADDR(n), mask)
#define HWIO_INTER_EE_n_ORIGINATOR_EE_OUTI(n,val)    \
        out_dword(HWIO_INTER_EE_n_ORIGINATOR_EE_ADDR(n),val)
#define HWIO_INTER_EE_n_ORIGINATOR_EE_OUTMI(n,mask,val) \
        out_dword_masked_ns(HWIO_INTER_EE_n_ORIGINATOR_EE_ADDR(n),mask,val,HWIO_INTER_EE_n_ORIGINATOR_EE_INI(n))
#define HWIO_INTER_EE_n_ORIGINATOR_EE_EE_NUMBER_BMSK                                      0xf
#define HWIO_INTER_EE_n_ORIGINATOR_EE_EE_NUMBER_SHFT                                      0x0

#define HWIO_INTER_EE_n_GSI_CH_CMD_ADDR(n)                                         (GSI_REG_BASE      + 0x0000c008 + 0x1000 * (n))
#define HWIO_INTER_EE_n_GSI_CH_CMD_PHYS(n)                                         (GSI_REG_BASE_PHYS + 0x0000c008 + 0x1000 * (n))
#define HWIO_INTER_EE_n_GSI_CH_CMD_OFFS(n)                                         (GSI_REG_BASE_OFFS + 0x0000c008 + 0x1000 * (n))
#define HWIO_INTER_EE_n_GSI_CH_CMD_RMSK                                            0xff0000ff
#define HWIO_INTER_EE_n_GSI_CH_CMD_MAXn                                                     2
#define HWIO_INTER_EE_n_GSI_CH_CMD_OUTI(n,val)    \
        out_dword(HWIO_INTER_EE_n_GSI_CH_CMD_ADDR(n),val)
#define HWIO_INTER_EE_n_GSI_CH_CMD_OPCODE_BMSK                                     0xff000000
#define HWIO_INTER_EE_n_GSI_CH_CMD_OPCODE_SHFT                                           0x18
#define HWIO_INTER_EE_n_GSI_CH_CMD_OPCODE_START_FVAL                                      0x1
#define HWIO_INTER_EE_n_GSI_CH_CMD_OPCODE_STOP_FVAL                                       0x2
#define HWIO_INTER_EE_n_GSI_CH_CMD_OPCODE_RESET_FVAL                                      0x9
#define HWIO_INTER_EE_n_GSI_CH_CMD_OPCODE_DE_ALLOC_FVAL                                   0xa
#define HWIO_INTER_EE_n_GSI_CH_CMD_OPCODE_DB_STOP_FVAL                                    0xb
#define HWIO_INTER_EE_n_GSI_CH_CMD_CHID_BMSK                                             0xff
#define HWIO_INTER_EE_n_GSI_CH_CMD_CHID_SHFT                                              0x0

#define HWIO_INTER_EE_n_EV_CH_CMD_ADDR(n)                                          (GSI_REG_BASE      + 0x0000c010 + 0x1000 * (n))
#define HWIO_INTER_EE_n_EV_CH_CMD_PHYS(n)                                          (GSI_REG_BASE_PHYS + 0x0000c010 + 0x1000 * (n))
#define HWIO_INTER_EE_n_EV_CH_CMD_OFFS(n)                                          (GSI_REG_BASE_OFFS + 0x0000c010 + 0x1000 * (n))
#define HWIO_INTER_EE_n_EV_CH_CMD_RMSK                                             0xff0000ff
#define HWIO_INTER_EE_n_EV_CH_CMD_MAXn                                                      2
#define HWIO_INTER_EE_n_EV_CH_CMD_OUTI(n,val)    \
        out_dword(HWIO_INTER_EE_n_EV_CH_CMD_ADDR(n),val)
#define HWIO_INTER_EE_n_EV_CH_CMD_OPCODE_BMSK                                      0xff000000
#define HWIO_INTER_EE_n_EV_CH_CMD_OPCODE_SHFT                                            0x18
#define HWIO_INTER_EE_n_EV_CH_CMD_OPCODE_RESET_FVAL                                       0x9
#define HWIO_INTER_EE_n_EV_CH_CMD_OPCODE_DE_ALLOC_FVAL                                    0xa
#define HWIO_INTER_EE_n_EV_CH_CMD_CHID_BMSK                                              0xff
#define HWIO_INTER_EE_n_EV_CH_CMD_CHID_SHFT                                               0x0

#define HWIO_INTER_EE_n_SRC_GSI_CH_IRQ_ADDR(n)                                     (GSI_REG_BASE      + 0x0000c018 + 0x1000 * (n))
#define HWIO_INTER_EE_n_SRC_GSI_CH_IRQ_PHYS(n)                                     (GSI_REG_BASE_PHYS + 0x0000c018 + 0x1000 * (n))
#define HWIO_INTER_EE_n_SRC_GSI_CH_IRQ_OFFS(n)                                     (GSI_REG_BASE_OFFS + 0x0000c018 + 0x1000 * (n))
#define HWIO_INTER_EE_n_SRC_GSI_CH_IRQ_RMSK                                        0xffffffff
#define HWIO_INTER_EE_n_SRC_GSI_CH_IRQ_MAXn                                                 2
#define HWIO_INTER_EE_n_SRC_GSI_CH_IRQ_INI(n)        \
        in_dword_masked(HWIO_INTER_EE_n_SRC_GSI_CH_IRQ_ADDR(n), HWIO_INTER_EE_n_SRC_GSI_CH_IRQ_RMSK)
#define HWIO_INTER_EE_n_SRC_GSI_CH_IRQ_INMI(n,mask)    \
        in_dword_masked(HWIO_INTER_EE_n_SRC_GSI_CH_IRQ_ADDR(n), mask)
#define HWIO_INTER_EE_n_SRC_GSI_CH_IRQ_GSI_CH_BIT_MAP_BMSK                         0xffffffff
#define HWIO_INTER_EE_n_SRC_GSI_CH_IRQ_GSI_CH_BIT_MAP_SHFT                                0x0

#define HWIO_INTER_EE_n_SRC_EV_CH_IRQ_ADDR(n)                                      (GSI_REG_BASE      + 0x0000c01c + 0x1000 * (n))
#define HWIO_INTER_EE_n_SRC_EV_CH_IRQ_PHYS(n)                                      (GSI_REG_BASE_PHYS + 0x0000c01c + 0x1000 * (n))
#define HWIO_INTER_EE_n_SRC_EV_CH_IRQ_OFFS(n)                                      (GSI_REG_BASE_OFFS + 0x0000c01c + 0x1000 * (n))
#define HWIO_INTER_EE_n_SRC_EV_CH_IRQ_RMSK                                         0xffffffff
#define HWIO_INTER_EE_n_SRC_EV_CH_IRQ_MAXn                                                  2
#define HWIO_INTER_EE_n_SRC_EV_CH_IRQ_INI(n)        \
        in_dword_masked(HWIO_INTER_EE_n_SRC_EV_CH_IRQ_ADDR(n), HWIO_INTER_EE_n_SRC_EV_CH_IRQ_RMSK)
#define HWIO_INTER_EE_n_SRC_EV_CH_IRQ_INMI(n,mask)    \
        in_dword_masked(HWIO_INTER_EE_n_SRC_EV_CH_IRQ_ADDR(n), mask)
#define HWIO_INTER_EE_n_SRC_EV_CH_IRQ_EV_CH_BIT_MAP_BMSK                           0xffffffff
#define HWIO_INTER_EE_n_SRC_EV_CH_IRQ_EV_CH_BIT_MAP_SHFT                                  0x0

#define HWIO_INTER_EE_n_SRC_GSI_CH_IRQ_MSK_ADDR(n)                                 (GSI_REG_BASE      + 0x0000c020 + 0x1000 * (n))
#define HWIO_INTER_EE_n_SRC_GSI_CH_IRQ_MSK_PHYS(n)                                 (GSI_REG_BASE_PHYS + 0x0000c020 + 0x1000 * (n))
#define HWIO_INTER_EE_n_SRC_GSI_CH_IRQ_MSK_OFFS(n)                                 (GSI_REG_BASE_OFFS + 0x0000c020 + 0x1000 * (n))
#define HWIO_INTER_EE_n_SRC_GSI_CH_IRQ_MSK_RMSK                                    0xffffffff
#define HWIO_INTER_EE_n_SRC_GSI_CH_IRQ_MSK_MAXn                                             2
#define HWIO_INTER_EE_n_SRC_GSI_CH_IRQ_MSK_INI(n)        \
        in_dword_masked(HWIO_INTER_EE_n_SRC_GSI_CH_IRQ_MSK_ADDR(n), HWIO_INTER_EE_n_SRC_GSI_CH_IRQ_MSK_RMSK)
#define HWIO_INTER_EE_n_SRC_GSI_CH_IRQ_MSK_INMI(n,mask)    \
        in_dword_masked(HWIO_INTER_EE_n_SRC_GSI_CH_IRQ_MSK_ADDR(n), mask)
#define HWIO_INTER_EE_n_SRC_GSI_CH_IRQ_MSK_OUTI(n,val)    \
        out_dword(HWIO_INTER_EE_n_SRC_GSI_CH_IRQ_MSK_ADDR(n),val)
#define HWIO_INTER_EE_n_SRC_GSI_CH_IRQ_MSK_OUTMI(n,mask,val) \
        out_dword_masked_ns(HWIO_INTER_EE_n_SRC_GSI_CH_IRQ_MSK_ADDR(n),mask,val,HWIO_INTER_EE_n_SRC_GSI_CH_IRQ_MSK_INI(n))
#define HWIO_INTER_EE_n_SRC_GSI_CH_IRQ_MSK_GSI_CH_BIT_MAP_MSK_BMSK                 0xffffffff
#define HWIO_INTER_EE_n_SRC_GSI_CH_IRQ_MSK_GSI_CH_BIT_MAP_MSK_SHFT                        0x0

#define HWIO_INTER_EE_n_SRC_EV_CH_IRQ_MSK_ADDR(n)                                  (GSI_REG_BASE      + 0x0000c024 + 0x1000 * (n))
#define HWIO_INTER_EE_n_SRC_EV_CH_IRQ_MSK_PHYS(n)                                  (GSI_REG_BASE_PHYS + 0x0000c024 + 0x1000 * (n))
#define HWIO_INTER_EE_n_SRC_EV_CH_IRQ_MSK_OFFS(n)                                  (GSI_REG_BASE_OFFS + 0x0000c024 + 0x1000 * (n))
#define HWIO_INTER_EE_n_SRC_EV_CH_IRQ_MSK_RMSK                                     0xffffffff
#define HWIO_INTER_EE_n_SRC_EV_CH_IRQ_MSK_MAXn                                              2
#define HWIO_INTER_EE_n_SRC_EV_CH_IRQ_MSK_INI(n)        \
        in_dword_masked(HWIO_INTER_EE_n_SRC_EV_CH_IRQ_MSK_ADDR(n), HWIO_INTER_EE_n_SRC_EV_CH_IRQ_MSK_RMSK)
#define HWIO_INTER_EE_n_SRC_EV_CH_IRQ_MSK_INMI(n,mask)    \
        in_dword_masked(HWIO_INTER_EE_n_SRC_EV_CH_IRQ_MSK_ADDR(n), mask)
#define HWIO_INTER_EE_n_SRC_EV_CH_IRQ_MSK_OUTI(n,val)    \
        out_dword(HWIO_INTER_EE_n_SRC_EV_CH_IRQ_MSK_ADDR(n),val)
#define HWIO_INTER_EE_n_SRC_EV_CH_IRQ_MSK_OUTMI(n,mask,val) \
        out_dword_masked_ns(HWIO_INTER_EE_n_SRC_EV_CH_IRQ_MSK_ADDR(n),mask,val,HWIO_INTER_EE_n_SRC_EV_CH_IRQ_MSK_INI(n))
#define HWIO_INTER_EE_n_SRC_EV_CH_IRQ_MSK_EV_CH_BIT_MAP_MSK_BMSK                   0xffffffff
#define HWIO_INTER_EE_n_SRC_EV_CH_IRQ_MSK_EV_CH_BIT_MAP_MSK_SHFT                          0x0

#define HWIO_INTER_EE_n_SRC_GSI_CH_IRQ_CLR_ADDR(n)                                 (GSI_REG_BASE      + 0x0000c028 + 0x1000 * (n))
#define HWIO_INTER_EE_n_SRC_GSI_CH_IRQ_CLR_PHYS(n)                                 (GSI_REG_BASE_PHYS + 0x0000c028 + 0x1000 * (n))
#define HWIO_INTER_EE_n_SRC_GSI_CH_IRQ_CLR_OFFS(n)                                 (GSI_REG_BASE_OFFS + 0x0000c028 + 0x1000 * (n))
#define HWIO_INTER_EE_n_SRC_GSI_CH_IRQ_CLR_RMSK                                    0xffffffff
#define HWIO_INTER_EE_n_SRC_GSI_CH_IRQ_CLR_MAXn                                             2
#define HWIO_INTER_EE_n_SRC_GSI_CH_IRQ_CLR_OUTI(n,val)    \
        out_dword(HWIO_INTER_EE_n_SRC_GSI_CH_IRQ_CLR_ADDR(n),val)
#define HWIO_INTER_EE_n_SRC_GSI_CH_IRQ_CLR_GSI_CH_BIT_MAP_BMSK                     0xffffffff
#define HWIO_INTER_EE_n_SRC_GSI_CH_IRQ_CLR_GSI_CH_BIT_MAP_SHFT                            0x0

#define HWIO_INTER_EE_n_SRC_EV_CH_IRQ_CLR_ADDR(n)                                  (GSI_REG_BASE      + 0x0000c02c + 0x1000 * (n))
#define HWIO_INTER_EE_n_SRC_EV_CH_IRQ_CLR_PHYS(n)                                  (GSI_REG_BASE_PHYS + 0x0000c02c + 0x1000 * (n))
#define HWIO_INTER_EE_n_SRC_EV_CH_IRQ_CLR_OFFS(n)                                  (GSI_REG_BASE_OFFS + 0x0000c02c + 0x1000 * (n))
#define HWIO_INTER_EE_n_SRC_EV_CH_IRQ_CLR_RMSK                                     0xffffffff
#define HWIO_INTER_EE_n_SRC_EV_CH_IRQ_CLR_MAXn                                              2
#define HWIO_INTER_EE_n_SRC_EV_CH_IRQ_CLR_OUTI(n,val)    \
        out_dword(HWIO_INTER_EE_n_SRC_EV_CH_IRQ_CLR_ADDR(n),val)
#define HWIO_INTER_EE_n_SRC_EV_CH_IRQ_CLR_EV_CH_BIT_MAP_BMSK                       0xffffffff
#define HWIO_INTER_EE_n_SRC_EV_CH_IRQ_CLR_EV_CH_BIT_MAP_SHFT                              0x0

/*----------------------------------------------------------------------------
 * MODULE: GSI_XPU2
 *--------------------------------------------------------------------------*/

#define GSI_XPU2_REG_BASE                                    (GSI_TOP_BASE      + 0x00000000)
#define GSI_XPU2_REG_BASE_PHYS                               (GSI_TOP_BASE_PHYS + 0x00000000)
#define GSI_XPU2_REG_BASE_OFFS                               0x00000000

#define HWIO_GSI_XPU_SCR_ADDR                                (GSI_XPU2_REG_BASE      + 0x00000000)
#define HWIO_GSI_XPU_SCR_PHYS                                (GSI_XPU2_REG_BASE_PHYS + 0x00000000)
#define HWIO_GSI_XPU_SCR_OFFS                                (GSI_XPU2_REG_BASE_OFFS + 0x00000000)
#define HWIO_GSI_XPU_SCR_RMSK                                     0x77f
#define HWIO_GSI_XPU_SCR_ATTR                                       0x3
#define HWIO_GSI_XPU_SCR_IN          \
        in_dword_masked(HWIO_GSI_XPU_SCR_ADDR, HWIO_GSI_XPU_SCR_RMSK)
#define HWIO_GSI_XPU_SCR_INM(m)      \
        in_dword_masked(HWIO_GSI_XPU_SCR_ADDR, m)
#define HWIO_GSI_XPU_SCR_OUT(v)      \
        out_dword(HWIO_GSI_XPU_SCR_ADDR,v)
#define HWIO_GSI_XPU_SCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GSI_XPU_SCR_ADDR,m,v,HWIO_GSI_XPU_SCR_IN)
#define HWIO_GSI_XPU_SCR_SCLEIE_BMSK                              0x400
#define HWIO_GSI_XPU_SCR_SCLEIE_SHFT                                0xa
#define HWIO_GSI_XPU_SCR_SCFGEIE_BMSK                             0x200
#define HWIO_GSI_XPU_SCR_SCFGEIE_SHFT                               0x9
#define HWIO_GSI_XPU_SCR_DYNAMIC_CLK_EN_BMSK                      0x100
#define HWIO_GSI_XPU_SCR_DYNAMIC_CLK_EN_SHFT                        0x8
#define HWIO_GSI_XPU_SCR_NSRGCLEE_BMSK                             0x40
#define HWIO_GSI_XPU_SCR_NSRGCLEE_SHFT                              0x6
#define HWIO_GSI_XPU_SCR_NSCFGE_BMSK                               0x20
#define HWIO_GSI_XPU_SCR_NSCFGE_SHFT                                0x5
#define HWIO_GSI_XPU_SCR_SDCDEE_BMSK                               0x10
#define HWIO_GSI_XPU_SCR_SDCDEE_SHFT                                0x4
#define HWIO_GSI_XPU_SCR_SEIE_BMSK                                  0x8
#define HWIO_GSI_XPU_SCR_SEIE_SHFT                                  0x3
#define HWIO_GSI_XPU_SCR_SCLERE_BMSK                                0x4
#define HWIO_GSI_XPU_SCR_SCLERE_SHFT                                0x2
#define HWIO_GSI_XPU_SCR_SCFGERE_BMSK                               0x2
#define HWIO_GSI_XPU_SCR_SCFGERE_SHFT                               0x1
#define HWIO_GSI_XPU_SCR_XPUNSE_BMSK                                0x1
#define HWIO_GSI_XPU_SCR_XPUNSE_SHFT                                0x0

#define HWIO_GSI_XPU_SWDR_ADDR                               (GSI_XPU2_REG_BASE      + 0x00000004)
#define HWIO_GSI_XPU_SWDR_PHYS                               (GSI_XPU2_REG_BASE_PHYS + 0x00000004)
#define HWIO_GSI_XPU_SWDR_OFFS                               (GSI_XPU2_REG_BASE_OFFS + 0x00000004)
#define HWIO_GSI_XPU_SWDR_RMSK                                      0x1
#define HWIO_GSI_XPU_SWDR_ATTR                                      0x3
#define HWIO_GSI_XPU_SWDR_IN          \
        in_dword_masked(HWIO_GSI_XPU_SWDR_ADDR, HWIO_GSI_XPU_SWDR_RMSK)
#define HWIO_GSI_XPU_SWDR_INM(m)      \
        in_dword_masked(HWIO_GSI_XPU_SWDR_ADDR, m)
#define HWIO_GSI_XPU_SWDR_OUT(v)      \
        out_dword(HWIO_GSI_XPU_SWDR_ADDR,v)
#define HWIO_GSI_XPU_SWDR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GSI_XPU_SWDR_ADDR,m,v,HWIO_GSI_XPU_SWDR_IN)
#define HWIO_GSI_XPU_SWDR_SCFGWD_BMSK                               0x1
#define HWIO_GSI_XPU_SWDR_SCFGWD_SHFT                               0x0

#define HWIO_GSI_XPU_SEAR0_ADDR                              (GSI_XPU2_REG_BASE      + 0x00000040)
#define HWIO_GSI_XPU_SEAR0_PHYS                              (GSI_XPU2_REG_BASE_PHYS + 0x00000040)
#define HWIO_GSI_XPU_SEAR0_OFFS                              (GSI_XPU2_REG_BASE_OFFS + 0x00000040)
#define HWIO_GSI_XPU_SEAR0_RMSK                              0xffffffff
#define HWIO_GSI_XPU_SEAR0_ATTR                                     0x1
#define HWIO_GSI_XPU_SEAR0_IN          \
        in_dword_masked(HWIO_GSI_XPU_SEAR0_ADDR, HWIO_GSI_XPU_SEAR0_RMSK)
#define HWIO_GSI_XPU_SEAR0_INM(m)      \
        in_dword_masked(HWIO_GSI_XPU_SEAR0_ADDR, m)
#define HWIO_GSI_XPU_SEAR0_PA_BMSK                           0xffffffff
#define HWIO_GSI_XPU_SEAR0_PA_SHFT                                  0x0

#define HWIO_GSI_XPU_SESR_ADDR                               (GSI_XPU2_REG_BASE      + 0x00000048)
#define HWIO_GSI_XPU_SESR_PHYS                               (GSI_XPU2_REG_BASE_PHYS + 0x00000048)
#define HWIO_GSI_XPU_SESR_OFFS                               (GSI_XPU2_REG_BASE_OFFS + 0x00000048)
#define HWIO_GSI_XPU_SESR_RMSK                               0x8000000f
#define HWIO_GSI_XPU_SESR_ATTR                                      0x3
#define HWIO_GSI_XPU_SESR_IN          \
        in_dword_masked(HWIO_GSI_XPU_SESR_ADDR, HWIO_GSI_XPU_SESR_RMSK)
#define HWIO_GSI_XPU_SESR_INM(m)      \
        in_dword_masked(HWIO_GSI_XPU_SESR_ADDR, m)
#define HWIO_GSI_XPU_SESR_OUT(v)      \
        out_dword(HWIO_GSI_XPU_SESR_ADDR,v)
#define HWIO_GSI_XPU_SESR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GSI_XPU_SESR_ADDR,m,v,HWIO_GSI_XPU_SESR_IN)
#define HWIO_GSI_XPU_SESR_MULTI_BMSK                         0x80000000
#define HWIO_GSI_XPU_SESR_MULTI_SHFT                               0x1f
#define HWIO_GSI_XPU_SESR_CLMULTI_BMSK                              0x8
#define HWIO_GSI_XPU_SESR_CLMULTI_SHFT                              0x3
#define HWIO_GSI_XPU_SESR_CFGMULTI_BMSK                             0x4
#define HWIO_GSI_XPU_SESR_CFGMULTI_SHFT                             0x2
#define HWIO_GSI_XPU_SESR_CLIENT_BMSK                               0x2
#define HWIO_GSI_XPU_SESR_CLIENT_SHFT                               0x1
#define HWIO_GSI_XPU_SESR_CFG_BMSK                                  0x1
#define HWIO_GSI_XPU_SESR_CFG_SHFT                                  0x0

#define HWIO_GSI_XPU_SESRRESTORE_ADDR                        (GSI_XPU2_REG_BASE      + 0x0000004c)
#define HWIO_GSI_XPU_SESRRESTORE_PHYS                        (GSI_XPU2_REG_BASE_PHYS + 0x0000004c)
#define HWIO_GSI_XPU_SESRRESTORE_OFFS                        (GSI_XPU2_REG_BASE_OFFS + 0x0000004c)
#define HWIO_GSI_XPU_SESRRESTORE_RMSK                        0x8000000f
#define HWIO_GSI_XPU_SESRRESTORE_ATTR                               0x3
#define HWIO_GSI_XPU_SESRRESTORE_IN          \
        in_dword_masked(HWIO_GSI_XPU_SESRRESTORE_ADDR, HWIO_GSI_XPU_SESRRESTORE_RMSK)
#define HWIO_GSI_XPU_SESRRESTORE_INM(m)      \
        in_dword_masked(HWIO_GSI_XPU_SESRRESTORE_ADDR, m)
#define HWIO_GSI_XPU_SESRRESTORE_OUT(v)      \
        out_dword(HWIO_GSI_XPU_SESRRESTORE_ADDR,v)
#define HWIO_GSI_XPU_SESRRESTORE_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GSI_XPU_SESRRESTORE_ADDR,m,v,HWIO_GSI_XPU_SESRRESTORE_IN)
#define HWIO_GSI_XPU_SESRRESTORE_MULTI_BMSK                  0x80000000
#define HWIO_GSI_XPU_SESRRESTORE_MULTI_SHFT                        0x1f
#define HWIO_GSI_XPU_SESRRESTORE_CLMULTI_BMSK                       0x8
#define HWIO_GSI_XPU_SESRRESTORE_CLMULTI_SHFT                       0x3
#define HWIO_GSI_XPU_SESRRESTORE_CFGMULTI_BMSK                      0x4
#define HWIO_GSI_XPU_SESRRESTORE_CFGMULTI_SHFT                      0x2
#define HWIO_GSI_XPU_SESRRESTORE_CLIENT_BMSK                        0x2
#define HWIO_GSI_XPU_SESRRESTORE_CLIENT_SHFT                        0x1
#define HWIO_GSI_XPU_SESRRESTORE_CFG_BMSK                           0x1
#define HWIO_GSI_XPU_SESRRESTORE_CFG_SHFT                           0x0

#define HWIO_GSI_XPU_SESYNR0_ADDR                            (GSI_XPU2_REG_BASE      + 0x00000050)
#define HWIO_GSI_XPU_SESYNR0_PHYS                            (GSI_XPU2_REG_BASE_PHYS + 0x00000050)
#define HWIO_GSI_XPU_SESYNR0_OFFS                            (GSI_XPU2_REG_BASE_OFFS + 0x00000050)
#define HWIO_GSI_XPU_SESYNR0_RMSK                            0xffffffff
#define HWIO_GSI_XPU_SESYNR0_ATTR                                   0x1
#define HWIO_GSI_XPU_SESYNR0_IN          \
        in_dword_masked(HWIO_GSI_XPU_SESYNR0_ADDR, HWIO_GSI_XPU_SESYNR0_RMSK)
#define HWIO_GSI_XPU_SESYNR0_INM(m)      \
        in_dword_masked(HWIO_GSI_XPU_SESYNR0_ADDR, m)
#define HWIO_GSI_XPU_SESYNR0_ATID_BMSK                       0xff000000
#define HWIO_GSI_XPU_SESYNR0_ATID_SHFT                             0x18
#define HWIO_GSI_XPU_SESYNR0_AVMID_BMSK                        0xff0000
#define HWIO_GSI_XPU_SESYNR0_AVMID_SHFT                            0x10
#define HWIO_GSI_XPU_SESYNR0_ABID_BMSK                           0xe000
#define HWIO_GSI_XPU_SESYNR0_ABID_SHFT                              0xd
#define HWIO_GSI_XPU_SESYNR0_APID_BMSK                           0x1f00
#define HWIO_GSI_XPU_SESYNR0_APID_SHFT                              0x8
#define HWIO_GSI_XPU_SESYNR0_AMID_BMSK                             0xff
#define HWIO_GSI_XPU_SESYNR0_AMID_SHFT                              0x0

#define HWIO_GSI_XPU_SESYNR1_ADDR                            (GSI_XPU2_REG_BASE      + 0x00000054)
#define HWIO_GSI_XPU_SESYNR1_PHYS                            (GSI_XPU2_REG_BASE_PHYS + 0x00000054)
#define HWIO_GSI_XPU_SESYNR1_OFFS                            (GSI_XPU2_REG_BASE_OFFS + 0x00000054)
#define HWIO_GSI_XPU_SESYNR1_RMSK                            0xffffffff
#define HWIO_GSI_XPU_SESYNR1_ATTR                                   0x1
#define HWIO_GSI_XPU_SESYNR1_IN          \
        in_dword_masked(HWIO_GSI_XPU_SESYNR1_ADDR, HWIO_GSI_XPU_SESYNR1_RMSK)
#define HWIO_GSI_XPU_SESYNR1_INM(m)      \
        in_dword_masked(HWIO_GSI_XPU_SESYNR1_ADDR, m)
#define HWIO_GSI_XPU_SESYNR1_DCD_BMSK                        0x80000000
#define HWIO_GSI_XPU_SESYNR1_DCD_SHFT                              0x1f
#define HWIO_GSI_XPU_SESYNR1_AC_BMSK                         0x40000000
#define HWIO_GSI_XPU_SESYNR1_AC_SHFT                               0x1e
#define HWIO_GSI_XPU_SESYNR1_BURSTLEN_BMSK                   0x20000000
#define HWIO_GSI_XPU_SESYNR1_BURSTLEN_SHFT                         0x1d
#define HWIO_GSI_XPU_SESYNR1_ARDALLOCATE_BMSK                0x10000000
#define HWIO_GSI_XPU_SESYNR1_ARDALLOCATE_SHFT                      0x1c
#define HWIO_GSI_XPU_SESYNR1_ABURST_BMSK                      0x8000000
#define HWIO_GSI_XPU_SESYNR1_ABURST_SHFT                           0x1b
#define HWIO_GSI_XPU_SESYNR1_AEXCLUSIVE_BMSK                  0x4000000
#define HWIO_GSI_XPU_SESYNR1_AEXCLUSIVE_SHFT                       0x1a
#define HWIO_GSI_XPU_SESYNR1_AWRITE_BMSK                      0x2000000
#define HWIO_GSI_XPU_SESYNR1_AWRITE_SHFT                           0x19
#define HWIO_GSI_XPU_SESYNR1_AFULL_BMSK                       0x1000000
#define HWIO_GSI_XPU_SESYNR1_AFULL_SHFT                            0x18
#define HWIO_GSI_XPU_SESYNR1_ARDBEADNDXEN_BMSK                 0x800000
#define HWIO_GSI_XPU_SESYNR1_ARDBEADNDXEN_SHFT                     0x17
#define HWIO_GSI_XPU_SESYNR1_AOOO_BMSK                         0x400000
#define HWIO_GSI_XPU_SESYNR1_AOOO_SHFT                             0x16
#define HWIO_GSI_XPU_SESYNR1_APREQPRIORITY_BMSK                0x380000
#define HWIO_GSI_XPU_SESYNR1_APREQPRIORITY_SHFT                    0x13
#define HWIO_GSI_XPU_SESYNR1_ASIZE_BMSK                         0x70000
#define HWIO_GSI_XPU_SESYNR1_ASIZE_SHFT                            0x10
#define HWIO_GSI_XPU_SESYNR1_AMSSSELFAUTH_BMSK                   0x8000
#define HWIO_GSI_XPU_SESYNR1_AMSSSELFAUTH_SHFT                      0xf
#define HWIO_GSI_XPU_SESYNR1_ALEN_BMSK                           0x7f00
#define HWIO_GSI_XPU_SESYNR1_ALEN_SHFT                              0x8
#define HWIO_GSI_XPU_SESYNR1_AINST_BMSK                            0x80
#define HWIO_GSI_XPU_SESYNR1_AINST_SHFT                             0x7
#define HWIO_GSI_XPU_SESYNR1_APROTNS_BMSK                          0x40
#define HWIO_GSI_XPU_SESYNR1_APROTNS_SHFT                           0x6
#define HWIO_GSI_XPU_SESYNR1_APRIV_BMSK                            0x20
#define HWIO_GSI_XPU_SESYNR1_APRIV_SHFT                             0x5
#define HWIO_GSI_XPU_SESYNR1_AINNERSHARED_BMSK                     0x10
#define HWIO_GSI_XPU_SESYNR1_AINNERSHARED_SHFT                      0x4
#define HWIO_GSI_XPU_SESYNR1_ASHARED_BMSK                           0x8
#define HWIO_GSI_XPU_SESYNR1_ASHARED_SHFT                           0x3
#define HWIO_GSI_XPU_SESYNR1_AMEMTYPE_BMSK                          0x7
#define HWIO_GSI_XPU_SESYNR1_AMEMTYPE_SHFT                          0x0

#define HWIO_GSI_XPU_SESYNR2_ADDR                            (GSI_XPU2_REG_BASE      + 0x00000058)
#define HWIO_GSI_XPU_SESYNR2_PHYS                            (GSI_XPU2_REG_BASE_PHYS + 0x00000058)
#define HWIO_GSI_XPU_SESYNR2_OFFS                            (GSI_XPU2_REG_BASE_OFFS + 0x00000058)
#define HWIO_GSI_XPU_SESYNR2_RMSK                                   0x7
#define HWIO_GSI_XPU_SESYNR2_ATTR                                   0x1
#define HWIO_GSI_XPU_SESYNR2_IN          \
        in_dword_masked(HWIO_GSI_XPU_SESYNR2_ADDR, HWIO_GSI_XPU_SESYNR2_RMSK)
#define HWIO_GSI_XPU_SESYNR2_INM(m)      \
        in_dword_masked(HWIO_GSI_XPU_SESYNR2_ADDR, m)
#define HWIO_GSI_XPU_SESYNR2_MODEM_PRT_HIT_BMSK                     0x4
#define HWIO_GSI_XPU_SESYNR2_MODEM_PRT_HIT_SHFT                     0x2
#define HWIO_GSI_XPU_SESYNR2_SECURE_PRT_HIT_BMSK                    0x2
#define HWIO_GSI_XPU_SESYNR2_SECURE_PRT_HIT_SHFT                    0x1
#define HWIO_GSI_XPU_SESYNR2_NONSECURE_PRT_HIT_BMSK                 0x1
#define HWIO_GSI_XPU_SESYNR2_NONSECURE_PRT_HIT_SHFT                 0x0

#define HWIO_GSI_XPU_MCR_ADDR                                (GSI_XPU2_REG_BASE      + 0x00000100)
#define HWIO_GSI_XPU_MCR_PHYS                                (GSI_XPU2_REG_BASE_PHYS + 0x00000100)
#define HWIO_GSI_XPU_MCR_OFFS                                (GSI_XPU2_REG_BASE_OFFS + 0x00000100)
#define HWIO_GSI_XPU_MCR_RMSK                                     0x71f
#define HWIO_GSI_XPU_MCR_ATTR                                       0x3
#define HWIO_GSI_XPU_MCR_IN          \
        in_dword_masked(HWIO_GSI_XPU_MCR_ADDR, HWIO_GSI_XPU_MCR_RMSK)
#define HWIO_GSI_XPU_MCR_INM(m)      \
        in_dword_masked(HWIO_GSI_XPU_MCR_ADDR, m)
#define HWIO_GSI_XPU_MCR_OUT(v)      \
        out_dword(HWIO_GSI_XPU_MCR_ADDR,v)
#define HWIO_GSI_XPU_MCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GSI_XPU_MCR_ADDR,m,v,HWIO_GSI_XPU_MCR_IN)
#define HWIO_GSI_XPU_MCR_CLEIE_BMSK                               0x400
#define HWIO_GSI_XPU_MCR_CLEIE_SHFT                                 0xa
#define HWIO_GSI_XPU_MCR_CFGEIE_BMSK                              0x200
#define HWIO_GSI_XPU_MCR_CFGEIE_SHFT                                0x9
#define HWIO_GSI_XPU_MCR_DYNAMIC_CLK_EN_BMSK                      0x100
#define HWIO_GSI_XPU_MCR_DYNAMIC_CLK_EN_SHFT                        0x8
#define HWIO_GSI_XPU_MCR_DCDEE_BMSK                                0x10
#define HWIO_GSI_XPU_MCR_DCDEE_SHFT                                 0x4
#define HWIO_GSI_XPU_MCR_EIE_BMSK                                   0x8
#define HWIO_GSI_XPU_MCR_EIE_SHFT                                   0x3
#define HWIO_GSI_XPU_MCR_CLERE_BMSK                                 0x4
#define HWIO_GSI_XPU_MCR_CLERE_SHFT                                 0x2
#define HWIO_GSI_XPU_MCR_CFGERE_BMSK                                0x2
#define HWIO_GSI_XPU_MCR_CFGERE_SHFT                                0x1
#define HWIO_GSI_XPU_MCR_XPUMSAE_BMSK                               0x1
#define HWIO_GSI_XPU_MCR_XPUMSAE_SHFT                               0x0

#define HWIO_GSI_XPU_MEAR0_ADDR                              (GSI_XPU2_REG_BASE      + 0x00000140)
#define HWIO_GSI_XPU_MEAR0_PHYS                              (GSI_XPU2_REG_BASE_PHYS + 0x00000140)
#define HWIO_GSI_XPU_MEAR0_OFFS                              (GSI_XPU2_REG_BASE_OFFS + 0x00000140)
#define HWIO_GSI_XPU_MEAR0_RMSK                              0xffffffff
#define HWIO_GSI_XPU_MEAR0_ATTR                                     0x1
#define HWIO_GSI_XPU_MEAR0_IN          \
        in_dword_masked(HWIO_GSI_XPU_MEAR0_ADDR, HWIO_GSI_XPU_MEAR0_RMSK)
#define HWIO_GSI_XPU_MEAR0_INM(m)      \
        in_dword_masked(HWIO_GSI_XPU_MEAR0_ADDR, m)
#define HWIO_GSI_XPU_MEAR0_PA_BMSK                           0xffffffff
#define HWIO_GSI_XPU_MEAR0_PA_SHFT                                  0x0

#define HWIO_GSI_XPU_MESR_ADDR                               (GSI_XPU2_REG_BASE      + 0x00000148)
#define HWIO_GSI_XPU_MESR_PHYS                               (GSI_XPU2_REG_BASE_PHYS + 0x00000148)
#define HWIO_GSI_XPU_MESR_OFFS                               (GSI_XPU2_REG_BASE_OFFS + 0x00000148)
#define HWIO_GSI_XPU_MESR_RMSK                               0x8000000f
#define HWIO_GSI_XPU_MESR_ATTR                                      0x3
#define HWIO_GSI_XPU_MESR_IN          \
        in_dword_masked(HWIO_GSI_XPU_MESR_ADDR, HWIO_GSI_XPU_MESR_RMSK)
#define HWIO_GSI_XPU_MESR_INM(m)      \
        in_dword_masked(HWIO_GSI_XPU_MESR_ADDR, m)
#define HWIO_GSI_XPU_MESR_OUT(v)      \
        out_dword(HWIO_GSI_XPU_MESR_ADDR,v)
#define HWIO_GSI_XPU_MESR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GSI_XPU_MESR_ADDR,m,v,HWIO_GSI_XPU_MESR_IN)
#define HWIO_GSI_XPU_MESR_MULTI_BMSK                         0x80000000
#define HWIO_GSI_XPU_MESR_MULTI_SHFT                               0x1f
#define HWIO_GSI_XPU_MESR_CLMULTI_BMSK                              0x8
#define HWIO_GSI_XPU_MESR_CLMULTI_SHFT                              0x3
#define HWIO_GSI_XPU_MESR_CFGMULTI_BMSK                             0x4
#define HWIO_GSI_XPU_MESR_CFGMULTI_SHFT                             0x2
#define HWIO_GSI_XPU_MESR_CLIENT_BMSK                               0x2
#define HWIO_GSI_XPU_MESR_CLIENT_SHFT                               0x1
#define HWIO_GSI_XPU_MESR_CFG_BMSK                                  0x1
#define HWIO_GSI_XPU_MESR_CFG_SHFT                                  0x0

#define HWIO_GSI_XPU_MESRRESTORE_ADDR                        (GSI_XPU2_REG_BASE      + 0x0000014c)
#define HWIO_GSI_XPU_MESRRESTORE_PHYS                        (GSI_XPU2_REG_BASE_PHYS + 0x0000014c)
#define HWIO_GSI_XPU_MESRRESTORE_OFFS                        (GSI_XPU2_REG_BASE_OFFS + 0x0000014c)
#define HWIO_GSI_XPU_MESRRESTORE_RMSK                        0x8000000f
#define HWIO_GSI_XPU_MESRRESTORE_ATTR                               0x3
#define HWIO_GSI_XPU_MESRRESTORE_IN          \
        in_dword_masked(HWIO_GSI_XPU_MESRRESTORE_ADDR, HWIO_GSI_XPU_MESRRESTORE_RMSK)
#define HWIO_GSI_XPU_MESRRESTORE_INM(m)      \
        in_dword_masked(HWIO_GSI_XPU_MESRRESTORE_ADDR, m)
#define HWIO_GSI_XPU_MESRRESTORE_OUT(v)      \
        out_dword(HWIO_GSI_XPU_MESRRESTORE_ADDR,v)
#define HWIO_GSI_XPU_MESRRESTORE_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GSI_XPU_MESRRESTORE_ADDR,m,v,HWIO_GSI_XPU_MESRRESTORE_IN)
#define HWIO_GSI_XPU_MESRRESTORE_MULTI_BMSK                  0x80000000
#define HWIO_GSI_XPU_MESRRESTORE_MULTI_SHFT                        0x1f
#define HWIO_GSI_XPU_MESRRESTORE_CLMULTI_BMSK                       0x8
#define HWIO_GSI_XPU_MESRRESTORE_CLMULTI_SHFT                       0x3
#define HWIO_GSI_XPU_MESRRESTORE_CFGMULTI_BMSK                      0x4
#define HWIO_GSI_XPU_MESRRESTORE_CFGMULTI_SHFT                      0x2
#define HWIO_GSI_XPU_MESRRESTORE_CLIENT_BMSK                        0x2
#define HWIO_GSI_XPU_MESRRESTORE_CLIENT_SHFT                        0x1
#define HWIO_GSI_XPU_MESRRESTORE_CFG_BMSK                           0x1
#define HWIO_GSI_XPU_MESRRESTORE_CFG_SHFT                           0x0

#define HWIO_GSI_XPU_MESYNR0_ADDR                            (GSI_XPU2_REG_BASE      + 0x00000150)
#define HWIO_GSI_XPU_MESYNR0_PHYS                            (GSI_XPU2_REG_BASE_PHYS + 0x00000150)
#define HWIO_GSI_XPU_MESYNR0_OFFS                            (GSI_XPU2_REG_BASE_OFFS + 0x00000150)
#define HWIO_GSI_XPU_MESYNR0_RMSK                            0xffffffff
#define HWIO_GSI_XPU_MESYNR0_ATTR                                   0x1
#define HWIO_GSI_XPU_MESYNR0_IN          \
        in_dword_masked(HWIO_GSI_XPU_MESYNR0_ADDR, HWIO_GSI_XPU_MESYNR0_RMSK)
#define HWIO_GSI_XPU_MESYNR0_INM(m)      \
        in_dword_masked(HWIO_GSI_XPU_MESYNR0_ADDR, m)
#define HWIO_GSI_XPU_MESYNR0_ATID_BMSK                       0xff000000
#define HWIO_GSI_XPU_MESYNR0_ATID_SHFT                             0x18
#define HWIO_GSI_XPU_MESYNR0_AVMID_BMSK                        0xff0000
#define HWIO_GSI_XPU_MESYNR0_AVMID_SHFT                            0x10
#define HWIO_GSI_XPU_MESYNR0_ABID_BMSK                           0xe000
#define HWIO_GSI_XPU_MESYNR0_ABID_SHFT                              0xd
#define HWIO_GSI_XPU_MESYNR0_APID_BMSK                           0x1f00
#define HWIO_GSI_XPU_MESYNR0_APID_SHFT                              0x8
#define HWIO_GSI_XPU_MESYNR0_AMID_BMSK                             0xff
#define HWIO_GSI_XPU_MESYNR0_AMID_SHFT                              0x0

#define HWIO_GSI_XPU_MESYNR1_ADDR                            (GSI_XPU2_REG_BASE      + 0x00000154)
#define HWIO_GSI_XPU_MESYNR1_PHYS                            (GSI_XPU2_REG_BASE_PHYS + 0x00000154)
#define HWIO_GSI_XPU_MESYNR1_OFFS                            (GSI_XPU2_REG_BASE_OFFS + 0x00000154)
#define HWIO_GSI_XPU_MESYNR1_RMSK                            0xffffffff
#define HWIO_GSI_XPU_MESYNR1_ATTR                                   0x1
#define HWIO_GSI_XPU_MESYNR1_IN          \
        in_dword_masked(HWIO_GSI_XPU_MESYNR1_ADDR, HWIO_GSI_XPU_MESYNR1_RMSK)
#define HWIO_GSI_XPU_MESYNR1_INM(m)      \
        in_dword_masked(HWIO_GSI_XPU_MESYNR1_ADDR, m)
#define HWIO_GSI_XPU_MESYNR1_DCD_BMSK                        0x80000000
#define HWIO_GSI_XPU_MESYNR1_DCD_SHFT                              0x1f
#define HWIO_GSI_XPU_MESYNR1_AC_BMSK                         0x40000000
#define HWIO_GSI_XPU_MESYNR1_AC_SHFT                               0x1e
#define HWIO_GSI_XPU_MESYNR1_BURSTLEN_BMSK                   0x20000000
#define HWIO_GSI_XPU_MESYNR1_BURSTLEN_SHFT                         0x1d
#define HWIO_GSI_XPU_MESYNR1_ARDALLOCATE_BMSK                0x10000000
#define HWIO_GSI_XPU_MESYNR1_ARDALLOCATE_SHFT                      0x1c
#define HWIO_GSI_XPU_MESYNR1_ABURST_BMSK                      0x8000000
#define HWIO_GSI_XPU_MESYNR1_ABURST_SHFT                           0x1b
#define HWIO_GSI_XPU_MESYNR1_AEXCLUSIVE_BMSK                  0x4000000
#define HWIO_GSI_XPU_MESYNR1_AEXCLUSIVE_SHFT                       0x1a
#define HWIO_GSI_XPU_MESYNR1_AWRITE_BMSK                      0x2000000
#define HWIO_GSI_XPU_MESYNR1_AWRITE_SHFT                           0x19
#define HWIO_GSI_XPU_MESYNR1_AFULL_BMSK                       0x1000000
#define HWIO_GSI_XPU_MESYNR1_AFULL_SHFT                            0x18
#define HWIO_GSI_XPU_MESYNR1_ARDBEADNDXEN_BMSK                 0x800000
#define HWIO_GSI_XPU_MESYNR1_ARDBEADNDXEN_SHFT                     0x17
#define HWIO_GSI_XPU_MESYNR1_AOOO_BMSK                         0x400000
#define HWIO_GSI_XPU_MESYNR1_AOOO_SHFT                             0x16
#define HWIO_GSI_XPU_MESYNR1_APREQPRIORITY_BMSK                0x380000
#define HWIO_GSI_XPU_MESYNR1_APREQPRIORITY_SHFT                    0x13
#define HWIO_GSI_XPU_MESYNR1_ASIZE_BMSK                         0x70000
#define HWIO_GSI_XPU_MESYNR1_ASIZE_SHFT                            0x10
#define HWIO_GSI_XPU_MESYNR1_AMSSSELFAUTH_BMSK                   0x8000
#define HWIO_GSI_XPU_MESYNR1_AMSSSELFAUTH_SHFT                      0xf
#define HWIO_GSI_XPU_MESYNR1_ALEN_BMSK                           0x7f00
#define HWIO_GSI_XPU_MESYNR1_ALEN_SHFT                              0x8
#define HWIO_GSI_XPU_MESYNR1_AINST_BMSK                            0x80
#define HWIO_GSI_XPU_MESYNR1_AINST_SHFT                             0x7
#define HWIO_GSI_XPU_MESYNR1_APROTNS_BMSK                          0x40
#define HWIO_GSI_XPU_MESYNR1_APROTNS_SHFT                           0x6
#define HWIO_GSI_XPU_MESYNR1_APRIV_BMSK                            0x20
#define HWIO_GSI_XPU_MESYNR1_APRIV_SHFT                             0x5
#define HWIO_GSI_XPU_MESYNR1_AINNERSHARED_BMSK                     0x10
#define HWIO_GSI_XPU_MESYNR1_AINNERSHARED_SHFT                      0x4
#define HWIO_GSI_XPU_MESYNR1_ASHARED_BMSK                           0x8
#define HWIO_GSI_XPU_MESYNR1_ASHARED_SHFT                           0x3
#define HWIO_GSI_XPU_MESYNR1_AMEMTYPE_BMSK                          0x7
#define HWIO_GSI_XPU_MESYNR1_AMEMTYPE_SHFT                          0x0

#define HWIO_GSI_XPU_MESYNR2_ADDR                            (GSI_XPU2_REG_BASE      + 0x00000158)
#define HWIO_GSI_XPU_MESYNR2_PHYS                            (GSI_XPU2_REG_BASE_PHYS + 0x00000158)
#define HWIO_GSI_XPU_MESYNR2_OFFS                            (GSI_XPU2_REG_BASE_OFFS + 0x00000158)
#define HWIO_GSI_XPU_MESYNR2_RMSK                                   0x7
#define HWIO_GSI_XPU_MESYNR2_ATTR                                   0x1
#define HWIO_GSI_XPU_MESYNR2_IN          \
        in_dword_masked(HWIO_GSI_XPU_MESYNR2_ADDR, HWIO_GSI_XPU_MESYNR2_RMSK)
#define HWIO_GSI_XPU_MESYNR2_INM(m)      \
        in_dword_masked(HWIO_GSI_XPU_MESYNR2_ADDR, m)
#define HWIO_GSI_XPU_MESYNR2_MODEM_PRT_HIT_BMSK                     0x4
#define HWIO_GSI_XPU_MESYNR2_MODEM_PRT_HIT_SHFT                     0x2
#define HWIO_GSI_XPU_MESYNR2_SECURE_PRT_HIT_BMSK                    0x2
#define HWIO_GSI_XPU_MESYNR2_SECURE_PRT_HIT_SHFT                    0x1
#define HWIO_GSI_XPU_MESYNR2_NONSECURE_PRT_HIT_BMSK                 0x1
#define HWIO_GSI_XPU_MESYNR2_NONSECURE_PRT_HIT_SHFT                 0x0

#define HWIO_GSI_XPU_CR_ADDR                                 (GSI_XPU2_REG_BASE      + 0x00000080)
#define HWIO_GSI_XPU_CR_PHYS                                 (GSI_XPU2_REG_BASE_PHYS + 0x00000080)
#define HWIO_GSI_XPU_CR_OFFS                                 (GSI_XPU2_REG_BASE_OFFS + 0x00000080)
#define HWIO_GSI_XPU_CR_RMSK                                    0x1071f
#define HWIO_GSI_XPU_CR_ATTR                                        0x3
#define HWIO_GSI_XPU_CR_IN          \
        in_dword_masked(HWIO_GSI_XPU_CR_ADDR, HWIO_GSI_XPU_CR_RMSK)
#define HWIO_GSI_XPU_CR_INM(m)      \
        in_dword_masked(HWIO_GSI_XPU_CR_ADDR, m)
#define HWIO_GSI_XPU_CR_OUT(v)      \
        out_dword(HWIO_GSI_XPU_CR_ADDR,v)
#define HWIO_GSI_XPU_CR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GSI_XPU_CR_ADDR,m,v,HWIO_GSI_XPU_CR_IN)
#define HWIO_GSI_XPU_CR_MSAE_BMSK                               0x10000
#define HWIO_GSI_XPU_CR_MSAE_SHFT                                  0x10
#define HWIO_GSI_XPU_CR_CLEIE_BMSK                                0x400
#define HWIO_GSI_XPU_CR_CLEIE_SHFT                                  0xa
#define HWIO_GSI_XPU_CR_CFGEIE_BMSK                               0x200
#define HWIO_GSI_XPU_CR_CFGEIE_SHFT                                 0x9
#define HWIO_GSI_XPU_CR_DYNAMIC_CLK_EN_BMSK                       0x100
#define HWIO_GSI_XPU_CR_DYNAMIC_CLK_EN_SHFT                         0x8
#define HWIO_GSI_XPU_CR_DCDEE_BMSK                                 0x10
#define HWIO_GSI_XPU_CR_DCDEE_SHFT                                  0x4
#define HWIO_GSI_XPU_CR_EIE_BMSK                                    0x8
#define HWIO_GSI_XPU_CR_EIE_SHFT                                    0x3
#define HWIO_GSI_XPU_CR_CLERE_BMSK                                  0x4
#define HWIO_GSI_XPU_CR_CLERE_SHFT                                  0x2
#define HWIO_GSI_XPU_CR_CFGERE_BMSK                                 0x2
#define HWIO_GSI_XPU_CR_CFGERE_SHFT                                 0x1
#define HWIO_GSI_XPU_CR_XPUVMIDE_BMSK                               0x1
#define HWIO_GSI_XPU_CR_XPUVMIDE_SHFT                               0x0

#define HWIO_GSI_XPU_RPU_ACRn_ADDR(n)                        (GSI_XPU2_REG_BASE      + 0x000000a0 + 0x4 * (n))
#define HWIO_GSI_XPU_RPU_ACRn_PHYS(n)                        (GSI_XPU2_REG_BASE_PHYS + 0x000000a0 + 0x4 * (n))
#define HWIO_GSI_XPU_RPU_ACRn_OFFS(n)                        (GSI_XPU2_REG_BASE_OFFS + 0x000000a0 + 0x4 * (n))
#define HWIO_GSI_XPU_RPU_ACRn_RMSK                           0xffffffff
#define HWIO_GSI_XPU_RPU_ACRn_MAXn                                    0
#define HWIO_GSI_XPU_RPU_ACRn_INI(n)        \
        in_dword_masked(HWIO_GSI_XPU_RPU_ACRn_ADDR(n), HWIO_GSI_XPU_RPU_ACRn_RMSK)
#define HWIO_GSI_XPU_RPU_ACRn_INMI(n,mask)    \
        in_dword_masked(HWIO_GSI_XPU_RPU_ACRn_ADDR(n), mask)
#define HWIO_GSI_XPU_RPU_ACRn_OUTI(n,val)    \
        out_dword(HWIO_GSI_XPU_RPU_ACRn_ADDR(n),val)
#define HWIO_GSI_XPU_RPU_ACRn_OUTMI(n,mask,val) \
        out_dword_masked_ns(HWIO_GSI_XPU_RPU_ACRn_ADDR(n),mask,val,HWIO_GSI_XPU_RPU_ACRn_INI(n))
#define HWIO_GSI_XPU_RPU_ACRn_RWE_BMSK                       0xffffffff
#define HWIO_GSI_XPU_RPU_ACRn_RWE_SHFT                              0x0

#define HWIO_GSI_XPU_EAR0_ADDR                               (GSI_XPU2_REG_BASE      + 0x000000c0)
#define HWIO_GSI_XPU_EAR0_PHYS                               (GSI_XPU2_REG_BASE_PHYS + 0x000000c0)
#define HWIO_GSI_XPU_EAR0_OFFS                               (GSI_XPU2_REG_BASE_OFFS + 0x000000c0)
#define HWIO_GSI_XPU_EAR0_RMSK                               0xffffffff
#define HWIO_GSI_XPU_EAR0_ATTR                                      0x1
#define HWIO_GSI_XPU_EAR0_IN          \
        in_dword_masked(HWIO_GSI_XPU_EAR0_ADDR, HWIO_GSI_XPU_EAR0_RMSK)
#define HWIO_GSI_XPU_EAR0_INM(m)      \
        in_dword_masked(HWIO_GSI_XPU_EAR0_ADDR, m)
#define HWIO_GSI_XPU_EAR0_PA_BMSK                            0xffffffff
#define HWIO_GSI_XPU_EAR0_PA_SHFT                                   0x0

#define HWIO_GSI_XPU_ESR_ADDR                                (GSI_XPU2_REG_BASE      + 0x000000c8)
#define HWIO_GSI_XPU_ESR_PHYS                                (GSI_XPU2_REG_BASE_PHYS + 0x000000c8)
#define HWIO_GSI_XPU_ESR_OFFS                                (GSI_XPU2_REG_BASE_OFFS + 0x000000c8)
#define HWIO_GSI_XPU_ESR_RMSK                                0x8000000f
#define HWIO_GSI_XPU_ESR_ATTR                                       0x3
#define HWIO_GSI_XPU_ESR_IN          \
        in_dword_masked(HWIO_GSI_XPU_ESR_ADDR, HWIO_GSI_XPU_ESR_RMSK)
#define HWIO_GSI_XPU_ESR_INM(m)      \
        in_dword_masked(HWIO_GSI_XPU_ESR_ADDR, m)
#define HWIO_GSI_XPU_ESR_OUT(v)      \
        out_dword(HWIO_GSI_XPU_ESR_ADDR,v)
#define HWIO_GSI_XPU_ESR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GSI_XPU_ESR_ADDR,m,v,HWIO_GSI_XPU_ESR_IN)
#define HWIO_GSI_XPU_ESR_MULTI_BMSK                          0x80000000
#define HWIO_GSI_XPU_ESR_MULTI_SHFT                                0x1f
#define HWIO_GSI_XPU_ESR_CLMULTI_BMSK                               0x8
#define HWIO_GSI_XPU_ESR_CLMULTI_SHFT                               0x3
#define HWIO_GSI_XPU_ESR_CFGMULTI_BMSK                              0x4
#define HWIO_GSI_XPU_ESR_CFGMULTI_SHFT                              0x2
#define HWIO_GSI_XPU_ESR_CLIENT_BMSK                                0x2
#define HWIO_GSI_XPU_ESR_CLIENT_SHFT                                0x1
#define HWIO_GSI_XPU_ESR_CFG_BMSK                                   0x1
#define HWIO_GSI_XPU_ESR_CFG_SHFT                                   0x0

#define HWIO_GSI_XPU_ESRRESTORE_ADDR                         (GSI_XPU2_REG_BASE      + 0x000000cc)
#define HWIO_GSI_XPU_ESRRESTORE_PHYS                         (GSI_XPU2_REG_BASE_PHYS + 0x000000cc)
#define HWIO_GSI_XPU_ESRRESTORE_OFFS                         (GSI_XPU2_REG_BASE_OFFS + 0x000000cc)
#define HWIO_GSI_XPU_ESRRESTORE_RMSK                         0x8000000f
#define HWIO_GSI_XPU_ESRRESTORE_ATTR                                0x3
#define HWIO_GSI_XPU_ESRRESTORE_IN          \
        in_dword_masked(HWIO_GSI_XPU_ESRRESTORE_ADDR, HWIO_GSI_XPU_ESRRESTORE_RMSK)
#define HWIO_GSI_XPU_ESRRESTORE_INM(m)      \
        in_dword_masked(HWIO_GSI_XPU_ESRRESTORE_ADDR, m)
#define HWIO_GSI_XPU_ESRRESTORE_OUT(v)      \
        out_dword(HWIO_GSI_XPU_ESRRESTORE_ADDR,v)
#define HWIO_GSI_XPU_ESRRESTORE_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GSI_XPU_ESRRESTORE_ADDR,m,v,HWIO_GSI_XPU_ESRRESTORE_IN)
#define HWIO_GSI_XPU_ESRRESTORE_MULTI_BMSK                   0x80000000
#define HWIO_GSI_XPU_ESRRESTORE_MULTI_SHFT                         0x1f
#define HWIO_GSI_XPU_ESRRESTORE_CLMULTI_BMSK                        0x8
#define HWIO_GSI_XPU_ESRRESTORE_CLMULTI_SHFT                        0x3
#define HWIO_GSI_XPU_ESRRESTORE_CFGMULTI_BMSK                       0x4
#define HWIO_GSI_XPU_ESRRESTORE_CFGMULTI_SHFT                       0x2
#define HWIO_GSI_XPU_ESRRESTORE_CLIENT_BMSK                         0x2
#define HWIO_GSI_XPU_ESRRESTORE_CLIENT_SHFT                         0x1
#define HWIO_GSI_XPU_ESRRESTORE_CFG_BMSK                            0x1
#define HWIO_GSI_XPU_ESRRESTORE_CFG_SHFT                            0x0

#define HWIO_GSI_XPU_ESYNR0_ADDR                             (GSI_XPU2_REG_BASE      + 0x000000d0)
#define HWIO_GSI_XPU_ESYNR0_PHYS                             (GSI_XPU2_REG_BASE_PHYS + 0x000000d0)
#define HWIO_GSI_XPU_ESYNR0_OFFS                             (GSI_XPU2_REG_BASE_OFFS + 0x000000d0)
#define HWIO_GSI_XPU_ESYNR0_RMSK                             0xffffffff
#define HWIO_GSI_XPU_ESYNR0_ATTR                                    0x1
#define HWIO_GSI_XPU_ESYNR0_IN          \
        in_dword_masked(HWIO_GSI_XPU_ESYNR0_ADDR, HWIO_GSI_XPU_ESYNR0_RMSK)
#define HWIO_GSI_XPU_ESYNR0_INM(m)      \
        in_dword_masked(HWIO_GSI_XPU_ESYNR0_ADDR, m)
#define HWIO_GSI_XPU_ESYNR0_ATID_BMSK                        0xff000000
#define HWIO_GSI_XPU_ESYNR0_ATID_SHFT                              0x18
#define HWIO_GSI_XPU_ESYNR0_AVMID_BMSK                         0xff0000
#define HWIO_GSI_XPU_ESYNR0_AVMID_SHFT                             0x10
#define HWIO_GSI_XPU_ESYNR0_ABID_BMSK                            0xe000
#define HWIO_GSI_XPU_ESYNR0_ABID_SHFT                               0xd
#define HWIO_GSI_XPU_ESYNR0_APID_BMSK                            0x1f00
#define HWIO_GSI_XPU_ESYNR0_APID_SHFT                               0x8
#define HWIO_GSI_XPU_ESYNR0_AMID_BMSK                              0xff
#define HWIO_GSI_XPU_ESYNR0_AMID_SHFT                               0x0

#define HWIO_GSI_XPU_ESYNR1_ADDR                             (GSI_XPU2_REG_BASE      + 0x000000d4)
#define HWIO_GSI_XPU_ESYNR1_PHYS                             (GSI_XPU2_REG_BASE_PHYS + 0x000000d4)
#define HWIO_GSI_XPU_ESYNR1_OFFS                             (GSI_XPU2_REG_BASE_OFFS + 0x000000d4)
#define HWIO_GSI_XPU_ESYNR1_RMSK                             0xffffffff
#define HWIO_GSI_XPU_ESYNR1_ATTR                                    0x1
#define HWIO_GSI_XPU_ESYNR1_IN          \
        in_dword_masked(HWIO_GSI_XPU_ESYNR1_ADDR, HWIO_GSI_XPU_ESYNR1_RMSK)
#define HWIO_GSI_XPU_ESYNR1_INM(m)      \
        in_dword_masked(HWIO_GSI_XPU_ESYNR1_ADDR, m)
#define HWIO_GSI_XPU_ESYNR1_DCD_BMSK                         0x80000000
#define HWIO_GSI_XPU_ESYNR1_DCD_SHFT                               0x1f
#define HWIO_GSI_XPU_ESYNR1_AC_BMSK                          0x40000000
#define HWIO_GSI_XPU_ESYNR1_AC_SHFT                                0x1e
#define HWIO_GSI_XPU_ESYNR1_BURSTLEN_BMSK                    0x20000000
#define HWIO_GSI_XPU_ESYNR1_BURSTLEN_SHFT                          0x1d
#define HWIO_GSI_XPU_ESYNR1_ARDALLOCATE_BMSK                 0x10000000
#define HWIO_GSI_XPU_ESYNR1_ARDALLOCATE_SHFT                       0x1c
#define HWIO_GSI_XPU_ESYNR1_ABURST_BMSK                       0x8000000
#define HWIO_GSI_XPU_ESYNR1_ABURST_SHFT                            0x1b
#define HWIO_GSI_XPU_ESYNR1_AEXCLUSIVE_BMSK                   0x4000000
#define HWIO_GSI_XPU_ESYNR1_AEXCLUSIVE_SHFT                        0x1a
#define HWIO_GSI_XPU_ESYNR1_AWRITE_BMSK                       0x2000000
#define HWIO_GSI_XPU_ESYNR1_AWRITE_SHFT                            0x19
#define HWIO_GSI_XPU_ESYNR1_AFULL_BMSK                        0x1000000
#define HWIO_GSI_XPU_ESYNR1_AFULL_SHFT                             0x18
#define HWIO_GSI_XPU_ESYNR1_ARDBEADNDXEN_BMSK                  0x800000
#define HWIO_GSI_XPU_ESYNR1_ARDBEADNDXEN_SHFT                      0x17
#define HWIO_GSI_XPU_ESYNR1_AOOO_BMSK                          0x400000
#define HWIO_GSI_XPU_ESYNR1_AOOO_SHFT                              0x16
#define HWIO_GSI_XPU_ESYNR1_APREQPRIORITY_BMSK                 0x380000
#define HWIO_GSI_XPU_ESYNR1_APREQPRIORITY_SHFT                     0x13
#define HWIO_GSI_XPU_ESYNR1_ASIZE_BMSK                          0x70000
#define HWIO_GSI_XPU_ESYNR1_ASIZE_SHFT                             0x10
#define HWIO_GSI_XPU_ESYNR1_AMSSSELFAUTH_BMSK                    0x8000
#define HWIO_GSI_XPU_ESYNR1_AMSSSELFAUTH_SHFT                       0xf
#define HWIO_GSI_XPU_ESYNR1_ALEN_BMSK                            0x7f00
#define HWIO_GSI_XPU_ESYNR1_ALEN_SHFT                               0x8
#define HWIO_GSI_XPU_ESYNR1_AINST_BMSK                             0x80
#define HWIO_GSI_XPU_ESYNR1_AINST_SHFT                              0x7
#define HWIO_GSI_XPU_ESYNR1_APROTNS_BMSK                           0x40
#define HWIO_GSI_XPU_ESYNR1_APROTNS_SHFT                            0x6
#define HWIO_GSI_XPU_ESYNR1_APRIV_BMSK                             0x20
#define HWIO_GSI_XPU_ESYNR1_APRIV_SHFT                              0x5
#define HWIO_GSI_XPU_ESYNR1_AINNERSHARED_BMSK                      0x10
#define HWIO_GSI_XPU_ESYNR1_AINNERSHARED_SHFT                       0x4
#define HWIO_GSI_XPU_ESYNR1_ASHARED_BMSK                            0x8
#define HWIO_GSI_XPU_ESYNR1_ASHARED_SHFT                            0x3
#define HWIO_GSI_XPU_ESYNR1_AMEMTYPE_BMSK                           0x7
#define HWIO_GSI_XPU_ESYNR1_AMEMTYPE_SHFT                           0x0

#define HWIO_GSI_XPU_ESYNR2_ADDR                             (GSI_XPU2_REG_BASE      + 0x000000d8)
#define HWIO_GSI_XPU_ESYNR2_PHYS                             (GSI_XPU2_REG_BASE_PHYS + 0x000000d8)
#define HWIO_GSI_XPU_ESYNR2_OFFS                             (GSI_XPU2_REG_BASE_OFFS + 0x000000d8)
#define HWIO_GSI_XPU_ESYNR2_RMSK                                    0x7
#define HWIO_GSI_XPU_ESYNR2_ATTR                                    0x1
#define HWIO_GSI_XPU_ESYNR2_IN          \
        in_dword_masked(HWIO_GSI_XPU_ESYNR2_ADDR, HWIO_GSI_XPU_ESYNR2_RMSK)
#define HWIO_GSI_XPU_ESYNR2_INM(m)      \
        in_dword_masked(HWIO_GSI_XPU_ESYNR2_ADDR, m)
#define HWIO_GSI_XPU_ESYNR2_MODEM_PRT_HIT_BMSK                      0x4
#define HWIO_GSI_XPU_ESYNR2_MODEM_PRT_HIT_SHFT                      0x2
#define HWIO_GSI_XPU_ESYNR2_SECURE_PRT_HIT_BMSK                     0x2
#define HWIO_GSI_XPU_ESYNR2_SECURE_PRT_HIT_SHFT                     0x1
#define HWIO_GSI_XPU_ESYNR2_NONSECURE_PRT_HIT_BMSK                  0x1
#define HWIO_GSI_XPU_ESYNR2_NONSECURE_PRT_HIT_SHFT                  0x0

#define HWIO_GSI_XPU_IDR0_ADDR                               (GSI_XPU2_REG_BASE      + 0x00000074)
#define HWIO_GSI_XPU_IDR0_PHYS                               (GSI_XPU2_REG_BASE_PHYS + 0x00000074)
#define HWIO_GSI_XPU_IDR0_OFFS                               (GSI_XPU2_REG_BASE_OFFS + 0x00000074)
#define HWIO_GSI_XPU_IDR0_RMSK                               0xc000bfff
#define HWIO_GSI_XPU_IDR0_ATTR                                      0x1
#define HWIO_GSI_XPU_IDR0_IN          \
        in_dword_masked(HWIO_GSI_XPU_IDR0_ADDR, HWIO_GSI_XPU_IDR0_RMSK)
#define HWIO_GSI_XPU_IDR0_INM(m)      \
        in_dword_masked(HWIO_GSI_XPU_IDR0_ADDR, m)
#define HWIO_GSI_XPU_IDR0_CLIENTREQ_HALT_ACK_HW_EN_BMSK      0x80000000
#define HWIO_GSI_XPU_IDR0_CLIENTREQ_HALT_ACK_HW_EN_SHFT            0x1f
#define HWIO_GSI_XPU_IDR0_SAVERESTORE_HW_EN_BMSK             0x40000000
#define HWIO_GSI_XPU_IDR0_SAVERESTORE_HW_EN_SHFT                   0x1e
#define HWIO_GSI_XPU_IDR0_BLED_BMSK                              0x8000
#define HWIO_GSI_XPU_IDR0_BLED_SHFT                                 0xf
#define HWIO_GSI_XPU_IDR0_XPUT_BMSK                              0x3000
#define HWIO_GSI_XPU_IDR0_XPUT_SHFT                                 0xc
#define HWIO_GSI_XPU_IDR0_PT_BMSK                                 0x800
#define HWIO_GSI_XPU_IDR0_PT_SHFT                                   0xb
#define HWIO_GSI_XPU_IDR0_MV_BMSK                                 0x400
#define HWIO_GSI_XPU_IDR0_MV_SHFT                                   0xa
#define HWIO_GSI_XPU_IDR0_NRG_BMSK                                0x3ff
#define HWIO_GSI_XPU_IDR0_NRG_SHFT                                  0x0

#define HWIO_GSI_XPU_IDR1_ADDR                               (GSI_XPU2_REG_BASE      + 0x00000078)
#define HWIO_GSI_XPU_IDR1_PHYS                               (GSI_XPU2_REG_BASE_PHYS + 0x00000078)
#define HWIO_GSI_XPU_IDR1_OFFS                               (GSI_XPU2_REG_BASE_OFFS + 0x00000078)
#define HWIO_GSI_XPU_IDR1_RMSK                               0x7f3ffeff
#define HWIO_GSI_XPU_IDR1_ATTR                                      0x1
#define HWIO_GSI_XPU_IDR1_IN          \
        in_dword_masked(HWIO_GSI_XPU_IDR1_ADDR, HWIO_GSI_XPU_IDR1_RMSK)
#define HWIO_GSI_XPU_IDR1_INM(m)      \
        in_dword_masked(HWIO_GSI_XPU_IDR1_ADDR, m)
#define HWIO_GSI_XPU_IDR1_AMT_HW_ENABLE_BMSK                 0x40000000
#define HWIO_GSI_XPU_IDR1_AMT_HW_ENABLE_SHFT                       0x1e
#define HWIO_GSI_XPU_IDR1_CLIENT_ADDR_WIDTH_BMSK             0x3f000000
#define HWIO_GSI_XPU_IDR1_CLIENT_ADDR_WIDTH_SHFT                   0x18
#define HWIO_GSI_XPU_IDR1_CONFIG_ADDR_WIDTH_BMSK               0x3f0000
#define HWIO_GSI_XPU_IDR1_CONFIG_ADDR_WIDTH_SHFT                   0x10
#define HWIO_GSI_XPU_IDR1_QRIB_EN_BMSK                           0x8000
#define HWIO_GSI_XPU_IDR1_QRIB_EN_SHFT                              0xf
#define HWIO_GSI_XPU_IDR1_ASYNC_MODE_BMSK                        0x4000
#define HWIO_GSI_XPU_IDR1_ASYNC_MODE_SHFT                           0xe
#define HWIO_GSI_XPU_IDR1_CONFIG_TYPE_BMSK                       0x2000
#define HWIO_GSI_XPU_IDR1_CONFIG_TYPE_SHFT                          0xd
#define HWIO_GSI_XPU_IDR1_CLIENT_PIPELINE_ENABLED_BMSK           0x1000
#define HWIO_GSI_XPU_IDR1_CLIENT_PIPELINE_ENABLED_SHFT              0xc
#define HWIO_GSI_XPU_IDR1_MSA_CHECK_HW_ENABLE_BMSK                0x800
#define HWIO_GSI_XPU_IDR1_MSA_CHECK_HW_ENABLE_SHFT                  0xb
#define HWIO_GSI_XPU_IDR1_XPU_SYND_REG_ABSENT_BMSK                0x400
#define HWIO_GSI_XPU_IDR1_XPU_SYND_REG_ABSENT_SHFT                  0xa
#define HWIO_GSI_XPU_IDR1_TZXPU_BMSK                              0x200
#define HWIO_GSI_XPU_IDR1_TZXPU_SHFT                                0x9
#define HWIO_GSI_XPU_IDR1_NVMID_BMSK                               0xff
#define HWIO_GSI_XPU_IDR1_NVMID_SHFT                                0x0

#define HWIO_GSI_XPU_REV_ADDR                                (GSI_XPU2_REG_BASE      + 0x0000007c)
#define HWIO_GSI_XPU_REV_PHYS                                (GSI_XPU2_REG_BASE_PHYS + 0x0000007c)
#define HWIO_GSI_XPU_REV_OFFS                                (GSI_XPU2_REG_BASE_OFFS + 0x0000007c)
#define HWIO_GSI_XPU_REV_RMSK                                0xffffffff
#define HWIO_GSI_XPU_REV_ATTR                                       0x1
#define HWIO_GSI_XPU_REV_IN          \
        in_dword_masked(HWIO_GSI_XPU_REV_ADDR, HWIO_GSI_XPU_REV_RMSK)
#define HWIO_GSI_XPU_REV_INM(m)      \
        in_dword_masked(HWIO_GSI_XPU_REV_ADDR, m)
#define HWIO_GSI_XPU_REV_MAJOR_BMSK                          0xf0000000
#define HWIO_GSI_XPU_REV_MAJOR_SHFT                                0x1c
#define HWIO_GSI_XPU_REV_MINOR_BMSK                           0xfff0000
#define HWIO_GSI_XPU_REV_MINOR_SHFT                                0x10
#define HWIO_GSI_XPU_REV_STEP_BMSK                               0xffff
#define HWIO_GSI_XPU_REV_STEP_SHFT                                  0x0

#define HWIO_GSI_XPU_RGn_RACRm_ADDR(n,m)                     (GSI_XPU2_REG_BASE      + 0x00000200 + 0x80 * (n) + 0x4 * (m))
#define HWIO_GSI_XPU_RGn_RACRm_PHYS(n,m)                     (GSI_XPU2_REG_BASE_PHYS + 0x00000200 + 0x80 * (n) + 0x4 * (m))
#define HWIO_GSI_XPU_RGn_RACRm_OFFS(n,m)                     (GSI_XPU2_REG_BASE_OFFS + 0x00000200 + 0x80 * (n) + 0x4 * (m))
#define HWIO_GSI_XPU_RGn_RACRm_RMSK                          0xffffffff
#define HWIO_GSI_XPU_RGn_RACRm_MAXn                                  20
#define HWIO_GSI_XPU_RGn_RACRm_MAXm                                   0
#define HWIO_GSI_XPU_RGn_RACRm_INI2(n,m)        \
        in_dword_masked(HWIO_GSI_XPU_RGn_RACRm_ADDR(n,m), HWIO_GSI_XPU_RGn_RACRm_RMSK)
#define HWIO_GSI_XPU_RGn_RACRm_INMI2(n,m,mask)    \
        in_dword_masked(HWIO_GSI_XPU_RGn_RACRm_ADDR(n,m), mask)
#define HWIO_GSI_XPU_RGn_RACRm_OUTI2(n,m,val)    \
        out_dword(HWIO_GSI_XPU_RGn_RACRm_ADDR(n,m),val)
#define HWIO_GSI_XPU_RGn_RACRm_OUTMI2(n,m,mask,val) \
        out_dword_masked_ns(HWIO_GSI_XPU_RGn_RACRm_ADDR(n,m),mask,val,HWIO_GSI_XPU_RGn_RACRm_INI2(n,m))
#define HWIO_GSI_XPU_RGn_RACRm_RWE_BMSK                      0xffffffff
#define HWIO_GSI_XPU_RGn_RACRm_RWE_SHFT                             0x0

#define HWIO_GSI_XPU_RGn_SCR_ADDR(n)                         (GSI_XPU2_REG_BASE      + 0x00000250 + 0x80 * (n))
#define HWIO_GSI_XPU_RGn_SCR_PHYS(n)                         (GSI_XPU2_REG_BASE_PHYS + 0x00000250 + 0x80 * (n))
#define HWIO_GSI_XPU_RGn_SCR_OFFS(n)                         (GSI_XPU2_REG_BASE_OFFS + 0x00000250 + 0x80 * (n))
#define HWIO_GSI_XPU_RGn_SCR_RMSK                                  0x3f
#define HWIO_GSI_XPU_RGn_SCR_MAXn                                    20
#define HWIO_GSI_XPU_RGn_SCR_INI(n)        \
        in_dword_masked(HWIO_GSI_XPU_RGn_SCR_ADDR(n), HWIO_GSI_XPU_RGn_SCR_RMSK)
#define HWIO_GSI_XPU_RGn_SCR_INMI(n,mask)    \
        in_dword_masked(HWIO_GSI_XPU_RGn_SCR_ADDR(n), mask)
#define HWIO_GSI_XPU_RGn_SCR_OUTI(n,val)    \
        out_dword(HWIO_GSI_XPU_RGn_SCR_ADDR(n),val)
#define HWIO_GSI_XPU_RGn_SCR_OUTMI(n,mask,val) \
        out_dword_masked_ns(HWIO_GSI_XPU_RGn_SCR_ADDR(n),mask,val,HWIO_GSI_XPU_RGn_SCR_INI(n))
#define HWIO_GSI_XPU_RGn_SCR_SCLROE_BMSK                           0x20
#define HWIO_GSI_XPU_RGn_SCR_SCLROE_SHFT                            0x5
#define HWIO_GSI_XPU_RGn_SCR_VMIDCLROE_BMSK                        0x10
#define HWIO_GSI_XPU_RGn_SCR_VMIDCLROE_SHFT                         0x4
#define HWIO_GSI_XPU_RGn_SCR_MSACLROE_BMSK                          0x8
#define HWIO_GSI_XPU_RGn_SCR_MSACLROE_SHFT                          0x3
#define HWIO_GSI_XPU_RGn_SCR_VMIDCLRWE_BMSK                         0x4
#define HWIO_GSI_XPU_RGn_SCR_VMIDCLRWE_SHFT                         0x2
#define HWIO_GSI_XPU_RGn_SCR_MSACLRWE_BMSK                          0x2
#define HWIO_GSI_XPU_RGn_SCR_MSACLRWE_SHFT                          0x1
#define HWIO_GSI_XPU_RGn_SCR_NS_BMSK                                0x1
#define HWIO_GSI_XPU_RGn_SCR_NS_SHFT                                0x0

#define HWIO_GSI_XPU_RGn_MCR_ADDR(n)                         (GSI_XPU2_REG_BASE      + 0x00000254 + 0x80 * (n))
#define HWIO_GSI_XPU_RGn_MCR_PHYS(n)                         (GSI_XPU2_REG_BASE_PHYS + 0x00000254 + 0x80 * (n))
#define HWIO_GSI_XPU_RGn_MCR_OFFS(n)                         (GSI_XPU2_REG_BASE_OFFS + 0x00000254 + 0x80 * (n))
#define HWIO_GSI_XPU_RGn_MCR_RMSK                                  0x3f
#define HWIO_GSI_XPU_RGn_MCR_MAXn                                    20
#define HWIO_GSI_XPU_RGn_MCR_INI(n)        \
        in_dword_masked(HWIO_GSI_XPU_RGn_MCR_ADDR(n), HWIO_GSI_XPU_RGn_MCR_RMSK)
#define HWIO_GSI_XPU_RGn_MCR_INMI(n,mask)    \
        in_dword_masked(HWIO_GSI_XPU_RGn_MCR_ADDR(n), mask)
#define HWIO_GSI_XPU_RGn_MCR_OUTI(n,val)    \
        out_dword(HWIO_GSI_XPU_RGn_MCR_ADDR(n),val)
#define HWIO_GSI_XPU_RGn_MCR_OUTMI(n,mask,val) \
        out_dword_masked_ns(HWIO_GSI_XPU_RGn_MCR_ADDR(n),mask,val,HWIO_GSI_XPU_RGn_MCR_INI(n))
#define HWIO_GSI_XPU_RGn_MCR_MSACLROE_BMSK                         0x20
#define HWIO_GSI_XPU_RGn_MCR_MSACLROE_SHFT                          0x5
#define HWIO_GSI_XPU_RGn_MCR_VMIDCLROE_BMSK                        0x10
#define HWIO_GSI_XPU_RGn_MCR_VMIDCLROE_SHFT                         0x4
#define HWIO_GSI_XPU_RGn_MCR_SCLROE_BMSK                            0x8
#define HWIO_GSI_XPU_RGn_MCR_SCLROE_SHFT                            0x3
#define HWIO_GSI_XPU_RGn_MCR_VMIDCLE_BMSK                           0x4
#define HWIO_GSI_XPU_RGn_MCR_VMIDCLE_SHFT                           0x2
#define HWIO_GSI_XPU_RGn_MCR_SCLE_BMSK                              0x2
#define HWIO_GSI_XPU_RGn_MCR_SCLE_SHFT                              0x1
#define HWIO_GSI_XPU_RGn_MCR_MSAE_BMSK                              0x1
#define HWIO_GSI_XPU_RGn_MCR_MSAE_SHFT                              0x0


#endif /* __GSI_HWIO_H__ */
