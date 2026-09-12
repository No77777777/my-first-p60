#ifndef __HALCLKHWIOASM_H__
#define __HALCLKHWIOASM_H__
/*
===========================================================================
*/
/**
    @file HALclkHWIOASM.h
    @brief Auto-generated HWIO interface include file.

    Reference chip release:
        SM6225 (Divar) [divar_v1.0_p3q2r40_BTO]
 
    This file contains HWIO register definitions for the following modules:
        MSS_QDSP6V671SS_PUB
        MSS_QDSP6SS_QDSP6V671SS_PLL
        MSS_QDSP6SS_QDSP6SS_QTMR_AC
        MSS_QDSP6SS_QTMR_F0_0
        MSS_QDSP6SS_QTMR_F1_1
        MSS_QDSP6SS_QTMR_F2_2


    Generation parameters: 
    { 'filename': 'HALclkHWIOASM.h',
      'module-filter-exclude': {},
      'module-filter-include': {},
      'modules': [ 'MSS_QDSP6V671SS_PUB',
                   'MSS_QDSP6SS_QDSP6V671SS_PLL',
                   'MSS_QDSP6SS_QDSP6SS_QTMR_AC',
                   'MSS_QDSP6SS_QTMR_F0_0',
                   'MSS_QDSP6SS_QTMR_F1_1',
                   'MSS_QDSP6SS_QTMR_F2_2'],
      'output-offsets': True,
      'rmsk-input': True}
*/
/*
    ===========================================================================

    Copyright (c) 2021 Qualcomm Technologies, Inc.
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

    $Header: //components/rel/core.mpss/10.0/systemdrivers/clock/image/mss/hw/divar/HALclkHWIOASM.h#1 $
    $DateTime: 2021/05/25 00:21:19 $
    $Author: pwbldsvc $

    ===========================================================================
*/

#define MSS_TOP_BASE_PHYS      0x06000000
#define MSS_TOP_BASE           MSS_TOP_BASE_PHYS

/*----------------------------------------------------------------------------
 * MODULE: MSS_QDSP6V671SS_PUB
 *--------------------------------------------------------------------------*/

#define MSS_QDSP6V671SS_PUB_REG_BASE                                              (MSS_TOP_BASE            + 0x00080000)
#define MSS_QDSP6V671SS_PUB_REG_BASE_SIZE                                         0x10000
#define MSS_QDSP6V671SS_PUB_REG_BASE_USED                                         0x204c
#define MSS_QDSP6V671SS_PUB_REG_BASE_OFFS                                         0x00080000

#define HWIO_MSS_QDSP6SS_VERSION_ADDR                                             (MSS_QDSP6V671SS_PUB_REG_BASE            + 0x0)
#define HWIO_MSS_QDSP6SS_VERSION_OFFS                                             (MSS_QDSP6V671SS_PUB_REG_BASE_OFFS + 0x0)
#define HWIO_MSS_QDSP6SS_VERSION_RMSK                                             0xffffffff
#define HWIO_MSS_QDSP6SS_VERSION_IN                    \
                in_dword_masked(HWIO_MSS_QDSP6SS_VERSION_ADDR, HWIO_MSS_QDSP6SS_VERSION_RMSK)
#define HWIO_MSS_QDSP6SS_VERSION_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_VERSION_ADDR, m)
#define HWIO_MSS_QDSP6SS_VERSION_MAJOR_BMSK                                       0xf0000000
#define HWIO_MSS_QDSP6SS_VERSION_MAJOR_SHFT                                               28
#define HWIO_MSS_QDSP6SS_VERSION_MINOR_BMSK                                        0xfff0000
#define HWIO_MSS_QDSP6SS_VERSION_MINOR_SHFT                                               16
#define HWIO_MSS_QDSP6SS_VERSION_STEP_BMSK                                            0xffff
#define HWIO_MSS_QDSP6SS_VERSION_STEP_SHFT                                                 0

#define HWIO_MSS_QDSP6SS_RST_EVB_ADDR                                             (MSS_QDSP6V671SS_PUB_REG_BASE            + 0x10)
#define HWIO_MSS_QDSP6SS_RST_EVB_OFFS                                             (MSS_QDSP6V671SS_PUB_REG_BASE_OFFS + 0x10)
#define HWIO_MSS_QDSP6SS_RST_EVB_RMSK                                              0xffffff0
#define HWIO_MSS_QDSP6SS_RST_EVB_IN                    \
                in_dword_masked(HWIO_MSS_QDSP6SS_RST_EVB_ADDR, HWIO_MSS_QDSP6SS_RST_EVB_RMSK)
#define HWIO_MSS_QDSP6SS_RST_EVB_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_RST_EVB_ADDR, m)
#define HWIO_MSS_QDSP6SS_RST_EVB_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_RST_EVB_ADDR,v)
#define HWIO_MSS_QDSP6SS_RST_EVB_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_QDSP6SS_RST_EVB_ADDR,m,v,HWIO_MSS_QDSP6SS_RST_EVB_IN)
#define HWIO_MSS_QDSP6SS_RST_EVB_EVB_BMSK                                          0xffffff0
#define HWIO_MSS_QDSP6SS_RST_EVB_EVB_SHFT                                                  4

#define HWIO_MSS_QDSP6SS_CLADE2_ARES_CFG_ADDR                                     (MSS_QDSP6V671SS_PUB_REG_BASE            + 0x14)
#define HWIO_MSS_QDSP6SS_CLADE2_ARES_CFG_OFFS                                     (MSS_QDSP6V671SS_PUB_REG_BASE_OFFS + 0x14)
#define HWIO_MSS_QDSP6SS_CLADE2_ARES_CFG_RMSK                                            0x1
#define HWIO_MSS_QDSP6SS_CLADE2_ARES_CFG_IN                    \
                in_dword_masked(HWIO_MSS_QDSP6SS_CLADE2_ARES_CFG_ADDR, HWIO_MSS_QDSP6SS_CLADE2_ARES_CFG_RMSK)
#define HWIO_MSS_QDSP6SS_CLADE2_ARES_CFG_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_CLADE2_ARES_CFG_ADDR, m)
#define HWIO_MSS_QDSP6SS_CLADE2_ARES_CFG_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_CLADE2_ARES_CFG_ADDR,v)
#define HWIO_MSS_QDSP6SS_CLADE2_ARES_CFG_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_QDSP6SS_CLADE2_ARES_CFG_ADDR,m,v,HWIO_MSS_QDSP6SS_CLADE2_ARES_CFG_IN)
#define HWIO_MSS_QDSP6SS_CLADE2_ARES_CFG_CLADE2_ARES_ENA_BMSK                            0x1
#define HWIO_MSS_QDSP6SS_CLADE2_ARES_CFG_CLADE2_ARES_ENA_SHFT                              0

#define HWIO_MSS_QDSP6SS_DBG_CFG_ADDR                                             (MSS_QDSP6V671SS_PUB_REG_BASE            + 0x18)
#define HWIO_MSS_QDSP6SS_DBG_CFG_OFFS                                             (MSS_QDSP6V671SS_PUB_REG_BASE_OFFS + 0x18)
#define HWIO_MSS_QDSP6SS_DBG_CFG_RMSK                                             0xffff0002
#define HWIO_MSS_QDSP6SS_DBG_CFG_IN                    \
                in_dword_masked(HWIO_MSS_QDSP6SS_DBG_CFG_ADDR, HWIO_MSS_QDSP6SS_DBG_CFG_RMSK)
#define HWIO_MSS_QDSP6SS_DBG_CFG_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_DBG_CFG_ADDR, m)
#define HWIO_MSS_QDSP6SS_DBG_CFG_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_DBG_CFG_ADDR,v)
#define HWIO_MSS_QDSP6SS_DBG_CFG_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_QDSP6SS_DBG_CFG_ADDR,m,v,HWIO_MSS_QDSP6SS_DBG_CFG_IN)
#define HWIO_MSS_QDSP6SS_DBG_CFG_DBG_SW_REG_BMSK                                  0xff000000
#define HWIO_MSS_QDSP6SS_DBG_CFG_DBG_SW_REG_SHFT                                          24
#define HWIO_MSS_QDSP6SS_DBG_CFG_DBG_SPARE_BMSK                                     0xff0000
#define HWIO_MSS_QDSP6SS_DBG_CFG_DBG_SPARE_SHFT                                           16
#define HWIO_MSS_QDSP6SS_DBG_CFG_CTI_TIHS_ENA_BMSK                                       0x2
#define HWIO_MSS_QDSP6SS_DBG_CFG_CTI_TIHS_ENA_SHFT                                         1

#define HWIO_MSS_QDSP6SS_RET_CFG_ADDR                                             (MSS_QDSP6V671SS_PUB_REG_BASE            + 0x1c)
#define HWIO_MSS_QDSP6SS_RET_CFG_OFFS                                             (MSS_QDSP6V671SS_PUB_REG_BASE_OFFS + 0x1c)
#define HWIO_MSS_QDSP6SS_RET_CFG_RMSK                                                    0x3
#define HWIO_MSS_QDSP6SS_RET_CFG_IN                    \
                in_dword_masked(HWIO_MSS_QDSP6SS_RET_CFG_ADDR, HWIO_MSS_QDSP6SS_RET_CFG_RMSK)
#define HWIO_MSS_QDSP6SS_RET_CFG_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_RET_CFG_ADDR, m)
#define HWIO_MSS_QDSP6SS_RET_CFG_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_RET_CFG_ADDR,v)
#define HWIO_MSS_QDSP6SS_RET_CFG_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_QDSP6SS_RET_CFG_ADDR,m,v,HWIO_MSS_QDSP6SS_RET_CFG_IN)
#define HWIO_MSS_QDSP6SS_RET_CFG_NRET_ARES_ENA_BMSK                                      0x2
#define HWIO_MSS_QDSP6SS_RET_CFG_NRET_ARES_ENA_SHFT                                        1
#define HWIO_MSS_QDSP6SS_RET_CFG_RET_ARES_ENA_BMSK                                       0x1
#define HWIO_MSS_QDSP6SS_RET_CFG_RET_ARES_ENA_SHFT                                         0

#define HWIO_MSS_QDSP6SS_CORE_CBCR_ADDR                                           (MSS_QDSP6V671SS_PUB_REG_BASE            + 0x20)
#define HWIO_MSS_QDSP6SS_CORE_CBCR_OFFS                                           (MSS_QDSP6V671SS_PUB_REG_BASE_OFFS + 0x20)
#define HWIO_MSS_QDSP6SS_CORE_CBCR_RMSK                                           0x80000001
#define HWIO_MSS_QDSP6SS_CORE_CBCR_IN                    \
                in_dword_masked(HWIO_MSS_QDSP6SS_CORE_CBCR_ADDR, HWIO_MSS_QDSP6SS_CORE_CBCR_RMSK)
#define HWIO_MSS_QDSP6SS_CORE_CBCR_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_CORE_CBCR_ADDR, m)
#define HWIO_MSS_QDSP6SS_CORE_CBCR_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_CORE_CBCR_ADDR,v)
#define HWIO_MSS_QDSP6SS_CORE_CBCR_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_QDSP6SS_CORE_CBCR_ADDR,m,v,HWIO_MSS_QDSP6SS_CORE_CBCR_IN)
#define HWIO_MSS_QDSP6SS_CORE_CBCR_CLKOFF_BMSK                                    0x80000000
#define HWIO_MSS_QDSP6SS_CORE_CBCR_CLKOFF_SHFT                                            31
#define HWIO_MSS_QDSP6SS_CORE_CBCR_CLKEN_BMSK                                            0x1
#define HWIO_MSS_QDSP6SS_CORE_CBCR_CLKEN_SHFT                                              0

#define HWIO_MSS_QDSP6SS_CORE_CMD_RCGR_ADDR                                       (MSS_QDSP6V671SS_PUB_REG_BASE            + 0x28)
#define HWIO_MSS_QDSP6SS_CORE_CMD_RCGR_OFFS                                       (MSS_QDSP6V671SS_PUB_REG_BASE_OFFS + 0x28)
#define HWIO_MSS_QDSP6SS_CORE_CMD_RCGR_RMSK                                       0x80000013
#define HWIO_MSS_QDSP6SS_CORE_CMD_RCGR_IN                    \
                in_dword_masked(HWIO_MSS_QDSP6SS_CORE_CMD_RCGR_ADDR, HWIO_MSS_QDSP6SS_CORE_CMD_RCGR_RMSK)
#define HWIO_MSS_QDSP6SS_CORE_CMD_RCGR_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_CORE_CMD_RCGR_ADDR, m)
#define HWIO_MSS_QDSP6SS_CORE_CMD_RCGR_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_CORE_CMD_RCGR_ADDR,v)
#define HWIO_MSS_QDSP6SS_CORE_CMD_RCGR_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_QDSP6SS_CORE_CMD_RCGR_ADDR,m,v,HWIO_MSS_QDSP6SS_CORE_CMD_RCGR_IN)
#define HWIO_MSS_QDSP6SS_CORE_CMD_RCGR_ROOT_OFF_BMSK                              0x80000000
#define HWIO_MSS_QDSP6SS_CORE_CMD_RCGR_ROOT_OFF_SHFT                                      31
#define HWIO_MSS_QDSP6SS_CORE_CMD_RCGR_DIRTY_CFG_RCGR_BMSK                              0x10
#define HWIO_MSS_QDSP6SS_CORE_CMD_RCGR_DIRTY_CFG_RCGR_SHFT                                 4
#define HWIO_MSS_QDSP6SS_CORE_CMD_RCGR_ROOT_EN_BMSK                                      0x2
#define HWIO_MSS_QDSP6SS_CORE_CMD_RCGR_ROOT_EN_SHFT                                        1
#define HWIO_MSS_QDSP6SS_CORE_CMD_RCGR_UPDATE_BMSK                                       0x1
#define HWIO_MSS_QDSP6SS_CORE_CMD_RCGR_UPDATE_SHFT                                         0

#define HWIO_MSS_QDSP6SS_CORE_CFG_RCGR_ADDR                                       (MSS_QDSP6V671SS_PUB_REG_BASE            + 0x2c)
#define HWIO_MSS_QDSP6SS_CORE_CFG_RCGR_OFFS                                       (MSS_QDSP6V671SS_PUB_REG_BASE_OFFS + 0x2c)
#define HWIO_MSS_QDSP6SS_CORE_CFG_RCGR_RMSK                                         0x11071f
#define HWIO_MSS_QDSP6SS_CORE_CFG_RCGR_IN                    \
                in_dword_masked(HWIO_MSS_QDSP6SS_CORE_CFG_RCGR_ADDR, HWIO_MSS_QDSP6SS_CORE_CFG_RCGR_RMSK)
#define HWIO_MSS_QDSP6SS_CORE_CFG_RCGR_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_CORE_CFG_RCGR_ADDR, m)
#define HWIO_MSS_QDSP6SS_CORE_CFG_RCGR_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_CORE_CFG_RCGR_ADDR,v)
#define HWIO_MSS_QDSP6SS_CORE_CFG_RCGR_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_QDSP6SS_CORE_CFG_RCGR_ADDR,m,v,HWIO_MSS_QDSP6SS_CORE_CFG_RCGR_IN)
#define HWIO_MSS_QDSP6SS_CORE_CFG_RCGR_HW_CLK_CONTROL_BMSK                          0x100000
#define HWIO_MSS_QDSP6SS_CORE_CFG_RCGR_HW_CLK_CONTROL_SHFT                                20
#define HWIO_MSS_QDSP6SS_CORE_CFG_RCGR_RCGLITE_DISABLE_BMSK                          0x10000
#define HWIO_MSS_QDSP6SS_CORE_CFG_RCGR_RCGLITE_DISABLE_SHFT                               16
#define HWIO_MSS_QDSP6SS_CORE_CFG_RCGR_SRC_SEL_BMSK                                    0x700
#define HWIO_MSS_QDSP6SS_CORE_CFG_RCGR_SRC_SEL_SHFT                                        8
#define HWIO_MSS_QDSP6SS_CORE_CFG_RCGR_SRC_DIV_BMSK                                     0x1f
#define HWIO_MSS_QDSP6SS_CORE_CFG_RCGR_SRC_DIV_SHFT                                        0

#define HWIO_MSS_QDSP6SS_XO_CBCR_ADDR                                             (MSS_QDSP6V671SS_PUB_REG_BASE            + 0x38)
#define HWIO_MSS_QDSP6SS_XO_CBCR_OFFS                                             (MSS_QDSP6V671SS_PUB_REG_BASE_OFFS + 0x38)
#define HWIO_MSS_QDSP6SS_XO_CBCR_RMSK                                             0x80000001
#define HWIO_MSS_QDSP6SS_XO_CBCR_IN                    \
                in_dword_masked(HWIO_MSS_QDSP6SS_XO_CBCR_ADDR, HWIO_MSS_QDSP6SS_XO_CBCR_RMSK)
#define HWIO_MSS_QDSP6SS_XO_CBCR_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_XO_CBCR_ADDR, m)
#define HWIO_MSS_QDSP6SS_XO_CBCR_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_XO_CBCR_ADDR,v)
#define HWIO_MSS_QDSP6SS_XO_CBCR_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_QDSP6SS_XO_CBCR_ADDR,m,v,HWIO_MSS_QDSP6SS_XO_CBCR_IN)
#define HWIO_MSS_QDSP6SS_XO_CBCR_CLKOFF_BMSK                                      0x80000000
#define HWIO_MSS_QDSP6SS_XO_CBCR_CLKOFF_SHFT                                              31
#define HWIO_MSS_QDSP6SS_XO_CBCR_CLKEN_BMSK                                              0x1
#define HWIO_MSS_QDSP6SS_XO_CBCR_CLKEN_SHFT                                                0

#define HWIO_MSS_QDSP6SS_SLEEP_CBCR_ADDR                                          (MSS_QDSP6V671SS_PUB_REG_BASE            + 0x3c)
#define HWIO_MSS_QDSP6SS_SLEEP_CBCR_OFFS                                          (MSS_QDSP6V671SS_PUB_REG_BASE_OFFS + 0x3c)
#define HWIO_MSS_QDSP6SS_SLEEP_CBCR_RMSK                                          0x80000001
#define HWIO_MSS_QDSP6SS_SLEEP_CBCR_IN                    \
                in_dword_masked(HWIO_MSS_QDSP6SS_SLEEP_CBCR_ADDR, HWIO_MSS_QDSP6SS_SLEEP_CBCR_RMSK)
#define HWIO_MSS_QDSP6SS_SLEEP_CBCR_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_SLEEP_CBCR_ADDR, m)
#define HWIO_MSS_QDSP6SS_SLEEP_CBCR_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_SLEEP_CBCR_ADDR,v)
#define HWIO_MSS_QDSP6SS_SLEEP_CBCR_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_QDSP6SS_SLEEP_CBCR_ADDR,m,v,HWIO_MSS_QDSP6SS_SLEEP_CBCR_IN)
#define HWIO_MSS_QDSP6SS_SLEEP_CBCR_CLKOFF_BMSK                                   0x80000000
#define HWIO_MSS_QDSP6SS_SLEEP_CBCR_CLKOFF_SHFT                                           31
#define HWIO_MSS_QDSP6SS_SLEEP_CBCR_CLKEN_BMSK                                           0x1
#define HWIO_MSS_QDSP6SS_SLEEP_CBCR_CLKEN_SHFT                                             0

#define HWIO_MSS_QDSP6SS_NMI_ADDR                                                 (MSS_QDSP6V671SS_PUB_REG_BASE            + 0x40)
#define HWIO_MSS_QDSP6SS_NMI_OFFS                                                 (MSS_QDSP6V671SS_PUB_REG_BASE_OFFS + 0x40)
#define HWIO_MSS_QDSP6SS_NMI_RMSK                                                        0x3
#define HWIO_MSS_QDSP6SS_NMI_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_NMI_ADDR,v)
#define HWIO_MSS_QDSP6SS_NMI_CLEAR_STATUS_BMSK                                           0x2
#define HWIO_MSS_QDSP6SS_NMI_CLEAR_STATUS_SHFT                                             1
#define HWIO_MSS_QDSP6SS_NMI_SET_NMI_BMSK                                                0x1
#define HWIO_MSS_QDSP6SS_NMI_SET_NMI_SHFT                                                  0

#define HWIO_MSS_QDSP6SS_NMI_STATUS_ADDR                                          (MSS_QDSP6V671SS_PUB_REG_BASE            + 0x44)
#define HWIO_MSS_QDSP6SS_NMI_STATUS_OFFS                                          (MSS_QDSP6V671SS_PUB_REG_BASE_OFFS + 0x44)
#define HWIO_MSS_QDSP6SS_NMI_STATUS_RMSK                                                 0x7
#define HWIO_MSS_QDSP6SS_NMI_STATUS_IN                    \
                in_dword_masked(HWIO_MSS_QDSP6SS_NMI_STATUS_ADDR, HWIO_MSS_QDSP6SS_NMI_STATUS_RMSK)
#define HWIO_MSS_QDSP6SS_NMI_STATUS_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_NMI_STATUS_ADDR, m)
#define HWIO_MSS_QDSP6SS_NMI_STATUS_EXT_DBG_TRIG_BMSK                                    0x4
#define HWIO_MSS_QDSP6SS_NMI_STATUS_EXT_DBG_TRIG_SHFT                                      2
#define HWIO_MSS_QDSP6SS_NMI_STATUS_WDOG_TRIG_BMSK                                       0x2
#define HWIO_MSS_QDSP6SS_NMI_STATUS_WDOG_TRIG_SHFT                                         1
#define HWIO_MSS_QDSP6SS_NMI_STATUS_PUBCSR_TRIG_BMSK                                     0x1
#define HWIO_MSS_QDSP6SS_NMI_STATUS_PUBCSR_TRIG_SHFT                                       0

#define HWIO_MSS_QDSP6SS_SPDM_MON_CBCR_ADDR                                       (MSS_QDSP6V671SS_PUB_REG_BASE            + 0x48)
#define HWIO_MSS_QDSP6SS_SPDM_MON_CBCR_OFFS                                       (MSS_QDSP6V671SS_PUB_REG_BASE_OFFS + 0x48)
#define HWIO_MSS_QDSP6SS_SPDM_MON_CBCR_RMSK                                       0x80000001
#define HWIO_MSS_QDSP6SS_SPDM_MON_CBCR_IN                    \
                in_dword_masked(HWIO_MSS_QDSP6SS_SPDM_MON_CBCR_ADDR, HWIO_MSS_QDSP6SS_SPDM_MON_CBCR_RMSK)
#define HWIO_MSS_QDSP6SS_SPDM_MON_CBCR_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_SPDM_MON_CBCR_ADDR, m)
#define HWIO_MSS_QDSP6SS_SPDM_MON_CBCR_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_SPDM_MON_CBCR_ADDR,v)
#define HWIO_MSS_QDSP6SS_SPDM_MON_CBCR_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_QDSP6SS_SPDM_MON_CBCR_ADDR,m,v,HWIO_MSS_QDSP6SS_SPDM_MON_CBCR_IN)
#define HWIO_MSS_QDSP6SS_SPDM_MON_CBCR_CLKOFF_BMSK                                0x80000000
#define HWIO_MSS_QDSP6SS_SPDM_MON_CBCR_CLKOFF_SHFT                                        31
#define HWIO_MSS_QDSP6SS_SPDM_MON_CBCR_CLKEN_BMSK                                        0x1
#define HWIO_MSS_QDSP6SS_SPDM_MON_CBCR_CLKEN_SHFT                                          0

#define HWIO_MSS_QDSP6SS_INTF_HALTREQ_ADDR                                        (MSS_QDSP6V671SS_PUB_REG_BASE            + 0x88)
#define HWIO_MSS_QDSP6SS_INTF_HALTREQ_OFFS                                        (MSS_QDSP6V671SS_PUB_REG_BASE_OFFS + 0x88)
#define HWIO_MSS_QDSP6SS_INTF_HALTREQ_RMSK                                               0x7
#define HWIO_MSS_QDSP6SS_INTF_HALTREQ_IN                    \
                in_dword_masked(HWIO_MSS_QDSP6SS_INTF_HALTREQ_ADDR, HWIO_MSS_QDSP6SS_INTF_HALTREQ_RMSK)
#define HWIO_MSS_QDSP6SS_INTF_HALTREQ_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_INTF_HALTREQ_ADDR, m)
#define HWIO_MSS_QDSP6SS_INTF_HALTREQ_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_INTF_HALTREQ_ADDR,v)
#define HWIO_MSS_QDSP6SS_INTF_HALTREQ_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_QDSP6SS_INTF_HALTREQ_ADDR,m,v,HWIO_MSS_QDSP6SS_INTF_HALTREQ_IN)
#define HWIO_MSS_QDSP6SS_INTF_HALTREQ_AXIM2_BMSK                                         0x4
#define HWIO_MSS_QDSP6SS_INTF_HALTREQ_AXIM2_SHFT                                           2
#define HWIO_MSS_QDSP6SS_INTF_HALTREQ_AXIM_BMSK                                          0x2
#define HWIO_MSS_QDSP6SS_INTF_HALTREQ_AXIM_SHFT                                            1
#define HWIO_MSS_QDSP6SS_INTF_HALTREQ_ALL_BMSK                                           0x1
#define HWIO_MSS_QDSP6SS_INTF_HALTREQ_ALL_SHFT                                             0

#define HWIO_MSS_QDSP6SS_INTF_HALTACK_ADDR                                        (MSS_QDSP6V671SS_PUB_REG_BASE            + 0x8c)
#define HWIO_MSS_QDSP6SS_INTF_HALTACK_OFFS                                        (MSS_QDSP6V671SS_PUB_REG_BASE_OFFS + 0x8c)
#define HWIO_MSS_QDSP6SS_INTF_HALTACK_RMSK                                               0x7
#define HWIO_MSS_QDSP6SS_INTF_HALTACK_IN                    \
                in_dword_masked(HWIO_MSS_QDSP6SS_INTF_HALTACK_ADDR, HWIO_MSS_QDSP6SS_INTF_HALTACK_RMSK)
#define HWIO_MSS_QDSP6SS_INTF_HALTACK_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_INTF_HALTACK_ADDR, m)
#define HWIO_MSS_QDSP6SS_INTF_HALTACK_AXIM2_BMSK                                         0x4
#define HWIO_MSS_QDSP6SS_INTF_HALTACK_AXIM2_SHFT                                           2
#define HWIO_MSS_QDSP6SS_INTF_HALTACK_AXIM_BMSK                                          0x2
#define HWIO_MSS_QDSP6SS_INTF_HALTACK_AXIM_SHFT                                            1
#define HWIO_MSS_QDSP6SS_INTF_HALTACK_ALL_BMSK                                           0x1
#define HWIO_MSS_QDSP6SS_INTF_HALTACK_ALL_SHFT                                             0

#define HWIO_MSS_QDSP6SS_INTFCLAMP_SET_ADDR                                       (MSS_QDSP6V671SS_PUB_REG_BASE            + 0x90)
#define HWIO_MSS_QDSP6SS_INTFCLAMP_SET_OFFS                                       (MSS_QDSP6V671SS_PUB_REG_BASE_OFFS + 0x90)
#define HWIO_MSS_QDSP6SS_INTFCLAMP_SET_RMSK                                              0xf
#define HWIO_MSS_QDSP6SS_INTFCLAMP_SET_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_INTFCLAMP_SET_ADDR,v)
#define HWIO_MSS_QDSP6SS_INTFCLAMP_SET_AXIM2_CLAMP_E_BMSK                                0x8
#define HWIO_MSS_QDSP6SS_INTFCLAMP_SET_AXIM2_CLAMP_E_SHFT                                  3
#define HWIO_MSS_QDSP6SS_INTFCLAMP_SET_AXIM2_CLAMP_L_BMSK                                0x4
#define HWIO_MSS_QDSP6SS_INTFCLAMP_SET_AXIM2_CLAMP_L_SHFT                                  2
#define HWIO_MSS_QDSP6SS_INTFCLAMP_SET_AXIM_CLAMP_E_BMSK                                 0x2
#define HWIO_MSS_QDSP6SS_INTFCLAMP_SET_AXIM_CLAMP_E_SHFT                                   1
#define HWIO_MSS_QDSP6SS_INTFCLAMP_SET_AXIM_CLAMP_L_BMSK                                 0x1
#define HWIO_MSS_QDSP6SS_INTFCLAMP_SET_AXIM_CLAMP_L_SHFT                                   0

#define HWIO_MSS_QDSP6SS_INTFCLAMP_CLEAR_ADDR                                     (MSS_QDSP6V671SS_PUB_REG_BASE            + 0x94)
#define HWIO_MSS_QDSP6SS_INTFCLAMP_CLEAR_OFFS                                     (MSS_QDSP6V671SS_PUB_REG_BASE_OFFS + 0x94)
#define HWIO_MSS_QDSP6SS_INTFCLAMP_CLEAR_RMSK                                            0xf
#define HWIO_MSS_QDSP6SS_INTFCLAMP_CLEAR_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_INTFCLAMP_CLEAR_ADDR,v)
#define HWIO_MSS_QDSP6SS_INTFCLAMP_CLEAR_AXIM2_CLAMP_E_BMSK                              0x8
#define HWIO_MSS_QDSP6SS_INTFCLAMP_CLEAR_AXIM2_CLAMP_E_SHFT                                3
#define HWIO_MSS_QDSP6SS_INTFCLAMP_CLEAR_AXIM2_CLAMP_L_BMSK                              0x4
#define HWIO_MSS_QDSP6SS_INTFCLAMP_CLEAR_AXIM2_CLAMP_L_SHFT                                2
#define HWIO_MSS_QDSP6SS_INTFCLAMP_CLEAR_AXIM_CLAMP_E_BMSK                               0x2
#define HWIO_MSS_QDSP6SS_INTFCLAMP_CLEAR_AXIM_CLAMP_E_SHFT                                 1
#define HWIO_MSS_QDSP6SS_INTFCLAMP_CLEAR_AXIM_CLAMP_L_BMSK                               0x1
#define HWIO_MSS_QDSP6SS_INTFCLAMP_CLEAR_AXIM_CLAMP_L_SHFT                                 0

#define HWIO_MSS_QDSP6SS_INTFCLAMP_STATUS_ADDR                                    (MSS_QDSP6V671SS_PUB_REG_BASE            + 0x98)
#define HWIO_MSS_QDSP6SS_INTFCLAMP_STATUS_OFFS                                    (MSS_QDSP6V671SS_PUB_REG_BASE_OFFS + 0x98)
#define HWIO_MSS_QDSP6SS_INTFCLAMP_STATUS_RMSK                                           0xf
#define HWIO_MSS_QDSP6SS_INTFCLAMP_STATUS_IN                    \
                in_dword_masked(HWIO_MSS_QDSP6SS_INTFCLAMP_STATUS_ADDR, HWIO_MSS_QDSP6SS_INTFCLAMP_STATUS_RMSK)
#define HWIO_MSS_QDSP6SS_INTFCLAMP_STATUS_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_INTFCLAMP_STATUS_ADDR, m)
#define HWIO_MSS_QDSP6SS_INTFCLAMP_STATUS_AXIM2_CLAMP_E_BMSK                             0x8
#define HWIO_MSS_QDSP6SS_INTFCLAMP_STATUS_AXIM2_CLAMP_E_SHFT                               3
#define HWIO_MSS_QDSP6SS_INTFCLAMP_STATUS_AXIM2_CLAMP_L_BMSK                             0x4
#define HWIO_MSS_QDSP6SS_INTFCLAMP_STATUS_AXIM2_CLAMP_L_SHFT                               2
#define HWIO_MSS_QDSP6SS_INTFCLAMP_STATUS_AXIM_CLAMP_E_BMSK                              0x2
#define HWIO_MSS_QDSP6SS_INTFCLAMP_STATUS_AXIM_CLAMP_E_SHFT                                1
#define HWIO_MSS_QDSP6SS_INTFCLAMP_STATUS_AXIM_CLAMP_L_BMSK                              0x1
#define HWIO_MSS_QDSP6SS_INTFCLAMP_STATUS_AXIM_CLAMP_L_SHFT                                0

#define HWIO_MSS_QDSP6SS_INTF_FIFO_RESET_ADDR                                     (MSS_QDSP6V671SS_PUB_REG_BASE            + 0x9c)
#define HWIO_MSS_QDSP6SS_INTF_FIFO_RESET_OFFS                                     (MSS_QDSP6V671SS_PUB_REG_BASE_OFFS + 0x9c)
#define HWIO_MSS_QDSP6SS_INTF_FIFO_RESET_RMSK                                            0x3
#define HWIO_MSS_QDSP6SS_INTF_FIFO_RESET_IN                    \
                in_dword_masked(HWIO_MSS_QDSP6SS_INTF_FIFO_RESET_ADDR, HWIO_MSS_QDSP6SS_INTF_FIFO_RESET_RMSK)
#define HWIO_MSS_QDSP6SS_INTF_FIFO_RESET_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_INTF_FIFO_RESET_ADDR, m)
#define HWIO_MSS_QDSP6SS_INTF_FIFO_RESET_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_INTF_FIFO_RESET_ADDR,v)
#define HWIO_MSS_QDSP6SS_INTF_FIFO_RESET_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_QDSP6SS_INTF_FIFO_RESET_ADDR,m,v,HWIO_MSS_QDSP6SS_INTF_FIFO_RESET_IN)
#define HWIO_MSS_QDSP6SS_INTF_FIFO_RESET_AXIM2_BMSK                                      0x2
#define HWIO_MSS_QDSP6SS_INTF_FIFO_RESET_AXIM2_SHFT                                        1
#define HWIO_MSS_QDSP6SS_INTF_FIFO_RESET_AXIM_BMSK                                       0x1
#define HWIO_MSS_QDSP6SS_INTF_FIFO_RESET_AXIM_SHFT                                         0

#define HWIO_MSS_QDSP6SS_STATERET_CTL_ADDR                                        (MSS_QDSP6V671SS_PUB_REG_BASE            + 0xa0)
#define HWIO_MSS_QDSP6SS_STATERET_CTL_OFFS                                        (MSS_QDSP6V671SS_PUB_REG_BASE_OFFS + 0xa0)
#define HWIO_MSS_QDSP6SS_STATERET_CTL_RMSK                                               0xf
#define HWIO_MSS_QDSP6SS_STATERET_CTL_IN                    \
                in_dword_masked(HWIO_MSS_QDSP6SS_STATERET_CTL_ADDR, HWIO_MSS_QDSP6SS_STATERET_CTL_RMSK)
#define HWIO_MSS_QDSP6SS_STATERET_CTL_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_STATERET_CTL_ADDR, m)
#define HWIO_MSS_QDSP6SS_STATERET_CTL_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_STATERET_CTL_ADDR,v)
#define HWIO_MSS_QDSP6SS_STATERET_CTL_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_QDSP6SS_STATERET_CTL_ADDR,m,v,HWIO_MSS_QDSP6SS_STATERET_CTL_IN)
#define HWIO_MSS_QDSP6SS_STATERET_CTL_WAKEUP_IN_BMSK                                     0x8
#define HWIO_MSS_QDSP6SS_STATERET_CTL_WAKEUP_IN_SHFT                                       3
#define HWIO_MSS_QDSP6SS_STATERET_CTL_WAKE_IRQ_BMSK                                      0x4
#define HWIO_MSS_QDSP6SS_STATERET_CTL_WAKE_IRQ_SHFT                                        2
#define HWIO_MSS_QDSP6SS_STATERET_CTL_RESTORE_BMSK                                       0x2
#define HWIO_MSS_QDSP6SS_STATERET_CTL_RESTORE_SHFT                                         1
#define HWIO_MSS_QDSP6SS_STATERET_CTL_SAVE_BMSK                                          0x1
#define HWIO_MSS_QDSP6SS_STATERET_CTL_SAVE_SHFT                                            0

#define HWIO_MSS_QDSP6SS_CGC_OVERRIDE_ADDR                                        (MSS_QDSP6V671SS_PUB_REG_BASE            + 0xb0)
#define HWIO_MSS_QDSP6SS_CGC_OVERRIDE_OFFS                                        (MSS_QDSP6V671SS_PUB_REG_BASE_OFFS + 0xb0)
#define HWIO_MSS_QDSP6SS_CGC_OVERRIDE_RMSK                                            0xffff
#define HWIO_MSS_QDSP6SS_CGC_OVERRIDE_IN                    \
                in_dword_masked(HWIO_MSS_QDSP6SS_CGC_OVERRIDE_ADDR, HWIO_MSS_QDSP6SS_CGC_OVERRIDE_RMSK)
#define HWIO_MSS_QDSP6SS_CGC_OVERRIDE_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_CGC_OVERRIDE_ADDR, m)
#define HWIO_MSS_QDSP6SS_CGC_OVERRIDE_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_CGC_OVERRIDE_ADDR,v)
#define HWIO_MSS_QDSP6SS_CGC_OVERRIDE_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_QDSP6SS_CGC_OVERRIDE_ADDR,m,v,HWIO_MSS_QDSP6SS_CGC_OVERRIDE_IN)
#define HWIO_MSS_QDSP6SS_CGC_OVERRIDE_CP0_CLK_EN_BMSK                                 0x8000
#define HWIO_MSS_QDSP6SS_CGC_OVERRIDE_CP0_CLK_EN_SHFT                                     15
#define HWIO_MSS_QDSP6SS_CGC_OVERRIDE_SPARE_BIT14_BMSK                                0x4000
#define HWIO_MSS_QDSP6SS_CGC_OVERRIDE_SPARE_BIT14_SHFT                                    14
#define HWIO_MSS_QDSP6SS_CGC_OVERRIDE_AXIS_CLK_EN_BMSK                                0x2000
#define HWIO_MSS_QDSP6SS_CGC_OVERRIDE_AXIS_CLK_EN_SHFT                                    13
#define HWIO_MSS_QDSP6SS_CGC_OVERRIDE_SPARE_BIT12_BMSK                                0x1000
#define HWIO_MSS_QDSP6SS_CGC_OVERRIDE_SPARE_BIT12_SHFT                                    12
#define HWIO_MSS_QDSP6SS_CGC_OVERRIDE_AHBM_CLK_EN_BMSK                                 0x800
#define HWIO_MSS_QDSP6SS_CGC_OVERRIDE_AHBM_CLK_EN_SHFT                                    11
#define HWIO_MSS_QDSP6SS_CGC_OVERRIDE_AHBS_CLK_EN_BMSK                                 0x400
#define HWIO_MSS_QDSP6SS_CGC_OVERRIDE_AHBS_CLK_EN_SHFT                                    10
#define HWIO_MSS_QDSP6SS_CGC_OVERRIDE_PRIV_AHBS_CLK_EN_BMSK                            0x300
#define HWIO_MSS_QDSP6SS_CGC_OVERRIDE_PRIV_AHBS_CLK_EN_SHFT                                8
#define HWIO_MSS_QDSP6SS_CGC_OVERRIDE_SPARE_BIT7_BMSK                                   0x80
#define HWIO_MSS_QDSP6SS_CGC_OVERRIDE_SPARE_BIT7_SHFT                                      7
#define HWIO_MSS_QDSP6SS_CGC_OVERRIDE_L2VIC_AHBS_CLK_EN_BMSK                            0x60
#define HWIO_MSS_QDSP6SS_CGC_OVERRIDE_L2VIC_AHBS_CLK_EN_SHFT                               5
#define HWIO_MSS_QDSP6SS_CGC_OVERRIDE_TEST_AHBS_CLK_EN_BMSK                             0x10
#define HWIO_MSS_QDSP6SS_CGC_OVERRIDE_TEST_AHBS_CLK_EN_SHFT                                4
#define HWIO_MSS_QDSP6SS_CGC_OVERRIDE_PUB_AHBS_CLK_EN_BMSK                               0x8
#define HWIO_MSS_QDSP6SS_CGC_OVERRIDE_PUB_AHBS_CLK_EN_SHFT                                 3
#define HWIO_MSS_QDSP6SS_CGC_OVERRIDE_CORE_BUS_EN_BMSK                                   0x4
#define HWIO_MSS_QDSP6SS_CGC_OVERRIDE_CORE_BUS_EN_SHFT                                     2
#define HWIO_MSS_QDSP6SS_CGC_OVERRIDE_CORE_RCLK_EN_BMSK                                  0x2
#define HWIO_MSS_QDSP6SS_CGC_OVERRIDE_CORE_RCLK_EN_SHFT                                    1
#define HWIO_MSS_QDSP6SS_CGC_OVERRIDE_CORE_CLK_EN_BMSK                                   0x1
#define HWIO_MSS_QDSP6SS_CGC_OVERRIDE_CORE_CLK_EN_SHFT                                     0

#define HWIO_MSS_QDSP6SS_CORE_BHS_CTL_ADDR                                        (MSS_QDSP6V671SS_PUB_REG_BASE            + 0xc0)
#define HWIO_MSS_QDSP6SS_CORE_BHS_CTL_OFFS                                        (MSS_QDSP6V671SS_PUB_REG_BASE_OFFS + 0xc0)
#define HWIO_MSS_QDSP6SS_CORE_BHS_CTL_RMSK                                           0x3f3ff
#define HWIO_MSS_QDSP6SS_CORE_BHS_CTL_IN                    \
                in_dword_masked(HWIO_MSS_QDSP6SS_CORE_BHS_CTL_ADDR, HWIO_MSS_QDSP6SS_CORE_BHS_CTL_RMSK)
#define HWIO_MSS_QDSP6SS_CORE_BHS_CTL_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_CORE_BHS_CTL_ADDR, m)
#define HWIO_MSS_QDSP6SS_CORE_BHS_CTL_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_CORE_BHS_CTL_ADDR,v)
#define HWIO_MSS_QDSP6SS_CORE_BHS_CTL_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_QDSP6SS_CORE_BHS_CTL_ADDR,m,v,HWIO_MSS_QDSP6SS_CORE_BHS_CTL_IN)
#define HWIO_MSS_QDSP6SS_CORE_BHS_CTL_DELAY_PROG_BMSK                                0x3f000
#define HWIO_MSS_QDSP6SS_CORE_BHS_CTL_DELAY_PROG_SHFT                                     12
#define HWIO_MSS_QDSP6SS_CORE_BHS_CTL_ENF_PROG_BMSK                                    0x3f0
#define HWIO_MSS_QDSP6SS_CORE_BHS_CTL_ENF_PROG_SHFT                                        4
#define HWIO_MSS_QDSP6SS_CORE_BHS_CTL_DRIVE_SEL_BMSK                                     0xf
#define HWIO_MSS_QDSP6SS_CORE_BHS_CTL_DRIVE_SEL_SHFT                                       0

#define HWIO_MSS_QDSP6SS_CPR_OVERRIDE_ADDR                                        (MSS_QDSP6V671SS_PUB_REG_BASE            + 0xd0)
#define HWIO_MSS_QDSP6SS_CPR_OVERRIDE_OFFS                                        (MSS_QDSP6V671SS_PUB_REG_BASE_OFFS + 0xd0)
#define HWIO_MSS_QDSP6SS_CPR_OVERRIDE_RMSK                                               0x3
#define HWIO_MSS_QDSP6SS_CPR_OVERRIDE_IN                    \
                in_dword_masked(HWIO_MSS_QDSP6SS_CPR_OVERRIDE_ADDR, HWIO_MSS_QDSP6SS_CPR_OVERRIDE_RMSK)
#define HWIO_MSS_QDSP6SS_CPR_OVERRIDE_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_CPR_OVERRIDE_ADDR, m)
#define HWIO_MSS_QDSP6SS_CPR_OVERRIDE_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_CPR_OVERRIDE_ADDR,v)
#define HWIO_MSS_QDSP6SS_CPR_OVERRIDE_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_QDSP6SS_CPR_OVERRIDE_ADDR,m,v,HWIO_MSS_QDSP6SS_CPR_OVERRIDE_IN)
#define HWIO_MSS_QDSP6SS_CPR_OVERRIDE_EXT_ABYP_DIS_BMSK                                  0x2
#define HWIO_MSS_QDSP6SS_CPR_OVERRIDE_EXT_ABYP_DIS_SHFT                                    1
#define HWIO_MSS_QDSP6SS_CPR_OVERRIDE_LOCAL_BYP_DIS_BMSK                                 0x1
#define HWIO_MSS_QDSP6SS_CPR_OVERRIDE_LOCAL_BYP_DIS_SHFT                                   0

#define HWIO_MSS_QDSP6SS_STRAP_TCM_ADDR                                           (MSS_QDSP6V671SS_PUB_REG_BASE            + 0x100)
#define HWIO_MSS_QDSP6SS_STRAP_TCM_OFFS                                           (MSS_QDSP6V671SS_PUB_REG_BASE_OFFS + 0x100)
#define HWIO_MSS_QDSP6SS_STRAP_TCM_RMSK                                           0xfffc0000
#define HWIO_MSS_QDSP6SS_STRAP_TCM_IN                    \
                in_dword_masked(HWIO_MSS_QDSP6SS_STRAP_TCM_ADDR, HWIO_MSS_QDSP6SS_STRAP_TCM_RMSK)
#define HWIO_MSS_QDSP6SS_STRAP_TCM_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_STRAP_TCM_ADDR, m)
#define HWIO_MSS_QDSP6SS_STRAP_TCM_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_STRAP_TCM_ADDR,v)
#define HWIO_MSS_QDSP6SS_STRAP_TCM_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_QDSP6SS_STRAP_TCM_ADDR,m,v,HWIO_MSS_QDSP6SS_STRAP_TCM_IN)
#define HWIO_MSS_QDSP6SS_STRAP_TCM_BASE_ADDR_BMSK                                 0xfffc0000
#define HWIO_MSS_QDSP6SS_STRAP_TCM_BASE_ADDR_SHFT                                         18

#define HWIO_MSS_QDSP6SS_STRAP_AHBUPPER_ADDR                                      (MSS_QDSP6V671SS_PUB_REG_BASE            + 0x104)
#define HWIO_MSS_QDSP6SS_STRAP_AHBUPPER_OFFS                                      (MSS_QDSP6V671SS_PUB_REG_BASE_OFFS + 0x104)
#define HWIO_MSS_QDSP6SS_STRAP_AHBUPPER_RMSK                                      0xffffc000
#define HWIO_MSS_QDSP6SS_STRAP_AHBUPPER_IN                    \
                in_dword_masked(HWIO_MSS_QDSP6SS_STRAP_AHBUPPER_ADDR, HWIO_MSS_QDSP6SS_STRAP_AHBUPPER_RMSK)
#define HWIO_MSS_QDSP6SS_STRAP_AHBUPPER_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_STRAP_AHBUPPER_ADDR, m)
#define HWIO_MSS_QDSP6SS_STRAP_AHBUPPER_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_STRAP_AHBUPPER_ADDR,v)
#define HWIO_MSS_QDSP6SS_STRAP_AHBUPPER_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_QDSP6SS_STRAP_AHBUPPER_ADDR,m,v,HWIO_MSS_QDSP6SS_STRAP_AHBUPPER_IN)
#define HWIO_MSS_QDSP6SS_STRAP_AHBUPPER_UPPER_ADDR_BMSK                           0xffffc000
#define HWIO_MSS_QDSP6SS_STRAP_AHBUPPER_UPPER_ADDR_SHFT                                   14

#define HWIO_MSS_QDSP6SS_STRAP_AHBLOWER_ADDR                                      (MSS_QDSP6V671SS_PUB_REG_BASE            + 0x108)
#define HWIO_MSS_QDSP6SS_STRAP_AHBLOWER_OFFS                                      (MSS_QDSP6V671SS_PUB_REG_BASE_OFFS + 0x108)
#define HWIO_MSS_QDSP6SS_STRAP_AHBLOWER_RMSK                                      0xffffc000
#define HWIO_MSS_QDSP6SS_STRAP_AHBLOWER_IN                    \
                in_dword_masked(HWIO_MSS_QDSP6SS_STRAP_AHBLOWER_ADDR, HWIO_MSS_QDSP6SS_STRAP_AHBLOWER_RMSK)
#define HWIO_MSS_QDSP6SS_STRAP_AHBLOWER_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_STRAP_AHBLOWER_ADDR, m)
#define HWIO_MSS_QDSP6SS_STRAP_AHBLOWER_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_STRAP_AHBLOWER_ADDR,v)
#define HWIO_MSS_QDSP6SS_STRAP_AHBLOWER_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_QDSP6SS_STRAP_AHBLOWER_ADDR,m,v,HWIO_MSS_QDSP6SS_STRAP_AHBLOWER_IN)
#define HWIO_MSS_QDSP6SS_STRAP_AHBLOWER_LOWER_ADDR_BMSK                           0xffffc000
#define HWIO_MSS_QDSP6SS_STRAP_AHBLOWER_LOWER_ADDR_SHFT                                   14

#define HWIO_MSS_QDSP6SS_DCC_CTRL_ADDR                                            (MSS_QDSP6V671SS_PUB_REG_BASE            + 0x118)
#define HWIO_MSS_QDSP6SS_DCC_CTRL_OFFS                                            (MSS_QDSP6V671SS_PUB_REG_BASE_OFFS + 0x118)
#define HWIO_MSS_QDSP6SS_DCC_CTRL_RMSK                                            0xffffffff
#define HWIO_MSS_QDSP6SS_DCC_CTRL_IN                    \
                in_dword_masked(HWIO_MSS_QDSP6SS_DCC_CTRL_ADDR, HWIO_MSS_QDSP6SS_DCC_CTRL_RMSK)
#define HWIO_MSS_QDSP6SS_DCC_CTRL_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_DCC_CTRL_ADDR, m)
#define HWIO_MSS_QDSP6SS_DCC_CTRL_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_DCC_CTRL_ADDR,v)
#define HWIO_MSS_QDSP6SS_DCC_CTRL_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_QDSP6SS_DCC_CTRL_ADDR,m,v,HWIO_MSS_QDSP6SS_DCC_CTRL_IN)
#define HWIO_MSS_QDSP6SS_DCC_CTRL_CTRL_BMSK                                       0xffffffff
#define HWIO_MSS_QDSP6SS_DCC_CTRL_CTRL_SHFT                                                0

#define HWIO_MSS_QDSP6SS_STRAP_AXIM2UPPER_ADDR                                    (MSS_QDSP6V671SS_PUB_REG_BASE            + 0x11c)
#define HWIO_MSS_QDSP6SS_STRAP_AXIM2UPPER_OFFS                                    (MSS_QDSP6V671SS_PUB_REG_BASE_OFFS + 0x11c)
#define HWIO_MSS_QDSP6SS_STRAP_AXIM2UPPER_RMSK                                    0xffffc000
#define HWIO_MSS_QDSP6SS_STRAP_AXIM2UPPER_IN                    \
                in_dword_masked(HWIO_MSS_QDSP6SS_STRAP_AXIM2UPPER_ADDR, HWIO_MSS_QDSP6SS_STRAP_AXIM2UPPER_RMSK)
#define HWIO_MSS_QDSP6SS_STRAP_AXIM2UPPER_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_STRAP_AXIM2UPPER_ADDR, m)
#define HWIO_MSS_QDSP6SS_STRAP_AXIM2UPPER_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_STRAP_AXIM2UPPER_ADDR,v)
#define HWIO_MSS_QDSP6SS_STRAP_AXIM2UPPER_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_QDSP6SS_STRAP_AXIM2UPPER_ADDR,m,v,HWIO_MSS_QDSP6SS_STRAP_AXIM2UPPER_IN)
#define HWIO_MSS_QDSP6SS_STRAP_AXIM2UPPER_UPPER_ADDR_BMSK                         0xffffc000
#define HWIO_MSS_QDSP6SS_STRAP_AXIM2UPPER_UPPER_ADDR_SHFT                                 14

#define HWIO_MSS_QDSP6SS_STRAP_AXIM2LOWER_ADDR                                    (MSS_QDSP6V671SS_PUB_REG_BASE            + 0x120)
#define HWIO_MSS_QDSP6SS_STRAP_AXIM2LOWER_OFFS                                    (MSS_QDSP6V671SS_PUB_REG_BASE_OFFS + 0x120)
#define HWIO_MSS_QDSP6SS_STRAP_AXIM2LOWER_RMSK                                    0xffffc000
#define HWIO_MSS_QDSP6SS_STRAP_AXIM2LOWER_IN                    \
                in_dword_masked(HWIO_MSS_QDSP6SS_STRAP_AXIM2LOWER_ADDR, HWIO_MSS_QDSP6SS_STRAP_AXIM2LOWER_RMSK)
#define HWIO_MSS_QDSP6SS_STRAP_AXIM2LOWER_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_STRAP_AXIM2LOWER_ADDR, m)
#define HWIO_MSS_QDSP6SS_STRAP_AXIM2LOWER_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_STRAP_AXIM2LOWER_ADDR,v)
#define HWIO_MSS_QDSP6SS_STRAP_AXIM2LOWER_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_QDSP6SS_STRAP_AXIM2LOWER_ADDR,m,v,HWIO_MSS_QDSP6SS_STRAP_AXIM2LOWER_IN)
#define HWIO_MSS_QDSP6SS_STRAP_AXIM2LOWER_LOWER_ADDR_BMSK                         0xffffc000
#define HWIO_MSS_QDSP6SS_STRAP_AXIM2LOWER_LOWER_ADDR_SHFT                                 14

#define HWIO_MSS_QDSP6SS_QMC_SVS_CTL_ADDR                                         (MSS_QDSP6V671SS_PUB_REG_BASE            + 0x130)
#define HWIO_MSS_QDSP6SS_QMC_SVS_CTL_OFFS                                         (MSS_QDSP6V671SS_PUB_REG_BASE_OFFS + 0x130)
#define HWIO_MSS_QDSP6SS_QMC_SVS_CTL_RMSK                                                0x3
#define HWIO_MSS_QDSP6SS_QMC_SVS_CTL_IN                    \
                in_dword_masked(HWIO_MSS_QDSP6SS_QMC_SVS_CTL_ADDR, HWIO_MSS_QDSP6SS_QMC_SVS_CTL_RMSK)
#define HWIO_MSS_QDSP6SS_QMC_SVS_CTL_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_QMC_SVS_CTL_ADDR, m)
#define HWIO_MSS_QDSP6SS_QMC_SVS_CTL_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_QMC_SVS_CTL_ADDR,v)
#define HWIO_MSS_QDSP6SS_QMC_SVS_CTL_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_QDSP6SS_QMC_SVS_CTL_ADDR,m,v,HWIO_MSS_QDSP6SS_QMC_SVS_CTL_IN)
#define HWIO_MSS_QDSP6SS_QMC_SVS_CTL_QMC_MEM_SVS_SEL_BMSK                                0x2
#define HWIO_MSS_QDSP6SS_QMC_SVS_CTL_QMC_MEM_SVS_SEL_SHFT                                  1
#define HWIO_MSS_QDSP6SS_QMC_SVS_CTL_QMC_MEM_SVS_BMSK                                    0x1
#define HWIO_MSS_QDSP6SS_QMC_SVS_CTL_QMC_MEM_SVS_SHFT                                      0

#define HWIO_MSS_QDSP6SS_CORE_MEM_STAGGER_CTL_ADDR                                (MSS_QDSP6V671SS_PUB_REG_BASE            + 0x140)
#define HWIO_MSS_QDSP6SS_CORE_MEM_STAGGER_CTL_OFFS                                (MSS_QDSP6V671SS_PUB_REG_BASE_OFFS + 0x140)
#define HWIO_MSS_QDSP6SS_CORE_MEM_STAGGER_CTL_RMSK                                       0xf
#define HWIO_MSS_QDSP6SS_CORE_MEM_STAGGER_CTL_IN                    \
                in_dword_masked(HWIO_MSS_QDSP6SS_CORE_MEM_STAGGER_CTL_ADDR, HWIO_MSS_QDSP6SS_CORE_MEM_STAGGER_CTL_RMSK)
#define HWIO_MSS_QDSP6SS_CORE_MEM_STAGGER_CTL_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_CORE_MEM_STAGGER_CTL_ADDR, m)
#define HWIO_MSS_QDSP6SS_CORE_MEM_STAGGER_CTL_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_CORE_MEM_STAGGER_CTL_ADDR,v)
#define HWIO_MSS_QDSP6SS_CORE_MEM_STAGGER_CTL_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_QDSP6SS_CORE_MEM_STAGGER_CTL_ADDR,m,v,HWIO_MSS_QDSP6SS_CORE_MEM_STAGGER_CTL_IN)
#define HWIO_MSS_QDSP6SS_CORE_MEM_STAGGER_CTL_DIV_BMSK                                   0xf
#define HWIO_MSS_QDSP6SS_CORE_MEM_STAGGER_CTL_DIV_SHFT                                     0

#define HWIO_MSS_QDSP6SS_PLL_RCG_UPDATE_STATUS_ADDR                               (MSS_QDSP6V671SS_PUB_REG_BASE            + 0x260)
#define HWIO_MSS_QDSP6SS_PLL_RCG_UPDATE_STATUS_OFFS                               (MSS_QDSP6V671SS_PUB_REG_BASE_OFFS + 0x260)
#define HWIO_MSS_QDSP6SS_PLL_RCG_UPDATE_STATUS_RMSK                                      0x3
#define HWIO_MSS_QDSP6SS_PLL_RCG_UPDATE_STATUS_IN                    \
                in_dword_masked(HWIO_MSS_QDSP6SS_PLL_RCG_UPDATE_STATUS_ADDR, HWIO_MSS_QDSP6SS_PLL_RCG_UPDATE_STATUS_RMSK)
#define HWIO_MSS_QDSP6SS_PLL_RCG_UPDATE_STATUS_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_PLL_RCG_UPDATE_STATUS_ADDR, m)
#define HWIO_MSS_QDSP6SS_PLL_RCG_UPDATE_STATUS_RCG2PLL_UPD_STATUS_BMSK                   0x2
#define HWIO_MSS_QDSP6SS_PLL_RCG_UPDATE_STATUS_RCG2PLL_UPD_STATUS_SHFT                     1
#define HWIO_MSS_QDSP6SS_PLL_RCG_UPDATE_STATUS_PLL2RCG_UPD_STATUS_BMSK                   0x1
#define HWIO_MSS_QDSP6SS_PLL_RCG_UPDATE_STATUS_PLL2RCG_UPD_STATUS_SHFT                     0

#define HWIO_MSS_QDSP6SS_PLL_RCG_UPDATE_CFG_ADDR                                  (MSS_QDSP6V671SS_PUB_REG_BASE            + 0x264)
#define HWIO_MSS_QDSP6SS_PLL_RCG_UPDATE_CFG_OFFS                                  (MSS_QDSP6V671SS_PUB_REG_BASE_OFFS + 0x264)
#define HWIO_MSS_QDSP6SS_PLL_RCG_UPDATE_CFG_RMSK                                         0x3
#define HWIO_MSS_QDSP6SS_PLL_RCG_UPDATE_CFG_IN                    \
                in_dword_masked(HWIO_MSS_QDSP6SS_PLL_RCG_UPDATE_CFG_ADDR, HWIO_MSS_QDSP6SS_PLL_RCG_UPDATE_CFG_RMSK)
#define HWIO_MSS_QDSP6SS_PLL_RCG_UPDATE_CFG_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_PLL_RCG_UPDATE_CFG_ADDR, m)
#define HWIO_MSS_QDSP6SS_PLL_RCG_UPDATE_CFG_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_PLL_RCG_UPDATE_CFG_ADDR,v)
#define HWIO_MSS_QDSP6SS_PLL_RCG_UPDATE_CFG_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_QDSP6SS_PLL_RCG_UPDATE_CFG_ADDR,m,v,HWIO_MSS_QDSP6SS_PLL_RCG_UPDATE_CFG_IN)
#define HWIO_MSS_QDSP6SS_PLL_RCG_UPDATE_CFG_RCG2PLL_UPD_EN_BMSK                          0x2
#define HWIO_MSS_QDSP6SS_PLL_RCG_UPDATE_CFG_RCG2PLL_UPD_EN_SHFT                            1
#define HWIO_MSS_QDSP6SS_PLL_RCG_UPDATE_CFG_PLL2RCG_UPD_EN_BMSK                          0x1
#define HWIO_MSS_QDSP6SS_PLL_RCG_UPDATE_CFG_PLL2RCG_UPD_EN_SHFT                            0

#define HWIO_MSS_QDSP6SS_PLL_RCG_UPDATE_DLYCTL_ADDR                               (MSS_QDSP6V671SS_PUB_REG_BASE            + 0x268)
#define HWIO_MSS_QDSP6SS_PLL_RCG_UPDATE_DLYCTL_OFFS                               (MSS_QDSP6V671SS_PUB_REG_BASE_OFFS + 0x268)
#define HWIO_MSS_QDSP6SS_PLL_RCG_UPDATE_DLYCTL_RMSK                                     0x1f
#define HWIO_MSS_QDSP6SS_PLL_RCG_UPDATE_DLYCTL_IN                    \
                in_dword_masked(HWIO_MSS_QDSP6SS_PLL_RCG_UPDATE_DLYCTL_ADDR, HWIO_MSS_QDSP6SS_PLL_RCG_UPDATE_DLYCTL_RMSK)
#define HWIO_MSS_QDSP6SS_PLL_RCG_UPDATE_DLYCTL_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_PLL_RCG_UPDATE_DLYCTL_ADDR, m)
#define HWIO_MSS_QDSP6SS_PLL_RCG_UPDATE_DLYCTL_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_PLL_RCG_UPDATE_DLYCTL_ADDR,v)
#define HWIO_MSS_QDSP6SS_PLL_RCG_UPDATE_DLYCTL_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_QDSP6SS_PLL_RCG_UPDATE_DLYCTL_ADDR,m,v,HWIO_MSS_QDSP6SS_PLL_RCG_UPDATE_DLYCTL_IN)
#define HWIO_MSS_QDSP6SS_PLL_RCG_UPDATE_DLYCTL_MATCH_VALUE_BMSK                         0x1f
#define HWIO_MSS_QDSP6SS_PLL_RCG_UPDATE_DLYCTL_MATCH_VALUE_SHFT                            0

#define HWIO_MSS_QDSP6SS_PLL_RCG_UPDATE_CMD_ADDR                                  (MSS_QDSP6V671SS_PUB_REG_BASE            + 0x26c)
#define HWIO_MSS_QDSP6SS_PLL_RCG_UPDATE_CMD_OFFS                                  (MSS_QDSP6V671SS_PUB_REG_BASE_OFFS + 0x26c)
#define HWIO_MSS_QDSP6SS_PLL_RCG_UPDATE_CMD_RMSK                                         0x1
#define HWIO_MSS_QDSP6SS_PLL_RCG_UPDATE_CMD_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_PLL_RCG_UPDATE_CMD_ADDR,v)
#define HWIO_MSS_QDSP6SS_PLL_RCG_UPDATE_CMD_CMD_BMSK                                     0x1
#define HWIO_MSS_QDSP6SS_PLL_RCG_UPDATE_CMD_CMD_SHFT                                       0

#define HWIO_MSS_QDSP6SS_DBG_NMI_CFG_ADDR                                         (MSS_QDSP6V671SS_PUB_REG_BASE            + 0x300)
#define HWIO_MSS_QDSP6SS_DBG_NMI_CFG_OFFS                                         (MSS_QDSP6V671SS_PUB_REG_BASE_OFFS + 0x300)
#define HWIO_MSS_QDSP6SS_DBG_NMI_CFG_RMSK                                                0xf
#define HWIO_MSS_QDSP6SS_DBG_NMI_CFG_IN                    \
                in_dword_masked(HWIO_MSS_QDSP6SS_DBG_NMI_CFG_ADDR, HWIO_MSS_QDSP6SS_DBG_NMI_CFG_RMSK)
#define HWIO_MSS_QDSP6SS_DBG_NMI_CFG_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_DBG_NMI_CFG_ADDR, m)
#define HWIO_MSS_QDSP6SS_DBG_NMI_CFG_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_DBG_NMI_CFG_ADDR,v)
#define HWIO_MSS_QDSP6SS_DBG_NMI_CFG_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_QDSP6SS_DBG_NMI_CFG_ADDR,m,v,HWIO_MSS_QDSP6SS_DBG_NMI_CFG_IN)
#define HWIO_MSS_QDSP6SS_DBG_NMI_CFG_DBG_NMI_EN_IN_APCR_BMSK                             0x8
#define HWIO_MSS_QDSP6SS_DBG_NMI_CFG_DBG_NMI_EN_IN_APCR_SHFT                               3
#define HWIO_MSS_QDSP6SS_DBG_NMI_CFG_DBG_NMI_EN_IN_PWR_COLLAPSE_BMSK                     0x4
#define HWIO_MSS_QDSP6SS_DBG_NMI_CFG_DBG_NMI_EN_IN_PWR_COLLAPSE_SHFT                       2
#define HWIO_MSS_QDSP6SS_DBG_NMI_CFG_DBG_NMI_DISABLE_BMSK                                0x2
#define HWIO_MSS_QDSP6SS_DBG_NMI_CFG_DBG_NMI_DISABLE_SHFT                                  1
#define HWIO_MSS_QDSP6SS_DBG_NMI_CFG_DBG_NMI_WAKEUP_DISABLE_BMSK                         0x1
#define HWIO_MSS_QDSP6SS_DBG_NMI_CFG_DBG_NMI_WAKEUP_DISABLE_SHFT                           0

#define HWIO_MSS_QDSP6SS_DBG_NMI_PWR_STATUS_ADDR                                  (MSS_QDSP6V671SS_PUB_REG_BASE            + 0x304)
#define HWIO_MSS_QDSP6SS_DBG_NMI_PWR_STATUS_OFFS                                  (MSS_QDSP6V671SS_PUB_REG_BASE_OFFS + 0x304)
#define HWIO_MSS_QDSP6SS_DBG_NMI_PWR_STATUS_RMSK                                     0x7000f
#define HWIO_MSS_QDSP6SS_DBG_NMI_PWR_STATUS_IN                    \
                in_dword_masked(HWIO_MSS_QDSP6SS_DBG_NMI_PWR_STATUS_ADDR, HWIO_MSS_QDSP6SS_DBG_NMI_PWR_STATUS_RMSK)
#define HWIO_MSS_QDSP6SS_DBG_NMI_PWR_STATUS_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_DBG_NMI_PWR_STATUS_ADDR, m)
#define HWIO_MSS_QDSP6SS_DBG_NMI_PWR_STATUS_RSC_IN_SLEEP_BMSK                        0x40000
#define HWIO_MSS_QDSP6SS_DBG_NMI_PWR_STATUS_RSC_IN_SLEEP_SHFT                             18
#define HWIO_MSS_QDSP6SS_DBG_NMI_PWR_STATUS_QDSP6_CLK_GATING_BMSK                    0x20000
#define HWIO_MSS_QDSP6SS_DBG_NMI_PWR_STATUS_QDSP6_CLK_GATING_SHFT                         17
#define HWIO_MSS_QDSP6SS_DBG_NMI_PWR_STATUS_QDSP6_CORE_STATE_BMSK                    0x10000
#define HWIO_MSS_QDSP6SS_DBG_NMI_PWR_STATUS_QDSP6_CORE_STATE_SHFT                         16
#define HWIO_MSS_QDSP6SS_DBG_NMI_PWR_STATUS_NMI_CAUSE_BMSK                               0xc
#define HWIO_MSS_QDSP6SS_DBG_NMI_PWR_STATUS_NMI_CAUSE_SHFT                                 2
#define HWIO_MSS_QDSP6SS_DBG_NMI_PWR_STATUS_QDSP6_PWR_COLLAPSE_BMSK                      0x2
#define HWIO_MSS_QDSP6SS_DBG_NMI_PWR_STATUS_QDSP6_PWR_COLLAPSE_SHFT                        1
#define HWIO_MSS_QDSP6SS_DBG_NMI_PWR_STATUS_QDSP6_IN_SLEEP_BMSK                          0x1
#define HWIO_MSS_QDSP6SS_DBG_NMI_PWR_STATUS_QDSP6_IN_SLEEP_SHFT                            0

#define HWIO_MSS_QDSP6SS_BOOT_CORE_START_ADDR                                     (MSS_QDSP6V671SS_PUB_REG_BASE            + 0x400)
#define HWIO_MSS_QDSP6SS_BOOT_CORE_START_OFFS                                     (MSS_QDSP6V671SS_PUB_REG_BASE_OFFS + 0x400)
#define HWIO_MSS_QDSP6SS_BOOT_CORE_START_RMSK                                            0x1
#define HWIO_MSS_QDSP6SS_BOOT_CORE_START_IN                    \
                in_dword_masked(HWIO_MSS_QDSP6SS_BOOT_CORE_START_ADDR, HWIO_MSS_QDSP6SS_BOOT_CORE_START_RMSK)
#define HWIO_MSS_QDSP6SS_BOOT_CORE_START_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_BOOT_CORE_START_ADDR, m)
#define HWIO_MSS_QDSP6SS_BOOT_CORE_START_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_BOOT_CORE_START_ADDR,v)
#define HWIO_MSS_QDSP6SS_BOOT_CORE_START_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_QDSP6SS_BOOT_CORE_START_ADDR,m,v,HWIO_MSS_QDSP6SS_BOOT_CORE_START_IN)
#define HWIO_MSS_QDSP6SS_BOOT_CORE_START_START_BMSK                                      0x1
#define HWIO_MSS_QDSP6SS_BOOT_CORE_START_START_SHFT                                        0

#define HWIO_MSS_QDSP6SS_BOOT_CMD_ADDR                                            (MSS_QDSP6V671SS_PUB_REG_BASE            + 0x404)
#define HWIO_MSS_QDSP6SS_BOOT_CMD_OFFS                                            (MSS_QDSP6V671SS_PUB_REG_BASE_OFFS + 0x404)
#define HWIO_MSS_QDSP6SS_BOOT_CMD_RMSK                                                   0x1
#define HWIO_MSS_QDSP6SS_BOOT_CMD_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_BOOT_CMD_ADDR,v)
#define HWIO_MSS_QDSP6SS_BOOT_CMD_CMD_BMSK                                               0x1
#define HWIO_MSS_QDSP6SS_BOOT_CMD_CMD_SHFT                                                 0

#define HWIO_MSS_QDSP6SS_BOOT_STATUS_ADDR                                         (MSS_QDSP6V671SS_PUB_REG_BASE            + 0x408)
#define HWIO_MSS_QDSP6SS_BOOT_STATUS_OFFS                                         (MSS_QDSP6V671SS_PUB_REG_BASE_OFFS + 0x408)
#define HWIO_MSS_QDSP6SS_BOOT_STATUS_RMSK                                                0x1
#define HWIO_MSS_QDSP6SS_BOOT_STATUS_IN                    \
                in_dword_masked(HWIO_MSS_QDSP6SS_BOOT_STATUS_ADDR, HWIO_MSS_QDSP6SS_BOOT_STATUS_RMSK)
#define HWIO_MSS_QDSP6SS_BOOT_STATUS_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_BOOT_STATUS_ADDR, m)
#define HWIO_MSS_QDSP6SS_BOOT_STATUS_STATUS_BMSK                                         0x1
#define HWIO_MSS_QDSP6SS_BOOT_STATUS_STATUS_SHFT                                           0

#define HWIO_MSS_QDSP6SS_CORE_BHS_CFG_ADDR                                        (MSS_QDSP6V671SS_PUB_REG_BASE            + 0x410)
#define HWIO_MSS_QDSP6SS_CORE_BHS_CFG_OFFS                                        (MSS_QDSP6V671SS_PUB_REG_BASE_OFFS + 0x410)
#define HWIO_MSS_QDSP6SS_CORE_BHS_CFG_RMSK                                               0x1
#define HWIO_MSS_QDSP6SS_CORE_BHS_CFG_IN                    \
                in_dword_masked(HWIO_MSS_QDSP6SS_CORE_BHS_CFG_ADDR, HWIO_MSS_QDSP6SS_CORE_BHS_CFG_RMSK)
#define HWIO_MSS_QDSP6SS_CORE_BHS_CFG_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_CORE_BHS_CFG_ADDR, m)
#define HWIO_MSS_QDSP6SS_CORE_BHS_CFG_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_CORE_BHS_CFG_ADDR,v)
#define HWIO_MSS_QDSP6SS_CORE_BHS_CFG_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_QDSP6SS_CORE_BHS_CFG_ADDR,m,v,HWIO_MSS_QDSP6SS_CORE_BHS_CFG_IN)
#define HWIO_MSS_QDSP6SS_CORE_BHS_CFG_BHS_ON_BMSK                                        0x1
#define HWIO_MSS_QDSP6SS_CORE_BHS_CFG_BHS_ON_SHFT                                          0

#define HWIO_MSS_QDSP6SS_CORE_BHS_CMD_ADDR                                        (MSS_QDSP6V671SS_PUB_REG_BASE            + 0x414)
#define HWIO_MSS_QDSP6SS_CORE_BHS_CMD_OFFS                                        (MSS_QDSP6V671SS_PUB_REG_BASE_OFFS + 0x414)
#define HWIO_MSS_QDSP6SS_CORE_BHS_CMD_RMSK                                               0x1
#define HWIO_MSS_QDSP6SS_CORE_BHS_CMD_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_CORE_BHS_CMD_ADDR,v)
#define HWIO_MSS_QDSP6SS_CORE_BHS_CMD_UPDATE_BMSK                                        0x1
#define HWIO_MSS_QDSP6SS_CORE_BHS_CMD_UPDATE_SHFT                                          0

#define HWIO_MSS_QDSP6SS_CORE_BHS_STATUS_ADDR                                     (MSS_QDSP6V671SS_PUB_REG_BASE            + 0x418)
#define HWIO_MSS_QDSP6SS_CORE_BHS_STATUS_OFFS                                     (MSS_QDSP6V671SS_PUB_REG_BASE_OFFS + 0x418)
#define HWIO_MSS_QDSP6SS_CORE_BHS_STATUS_RMSK                                            0x1
#define HWIO_MSS_QDSP6SS_CORE_BHS_STATUS_IN                    \
                in_dword_masked(HWIO_MSS_QDSP6SS_CORE_BHS_STATUS_ADDR, HWIO_MSS_QDSP6SS_CORE_BHS_STATUS_RMSK)
#define HWIO_MSS_QDSP6SS_CORE_BHS_STATUS_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_CORE_BHS_STATUS_ADDR, m)
#define HWIO_MSS_QDSP6SS_CORE_BHS_STATUS_BHS_ON_BMSK                                     0x1
#define HWIO_MSS_QDSP6SS_CORE_BHS_STATUS_BHS_ON_SHFT                                       0

#define HWIO_MSS_QDSP6SS_LDO_CTL_CFG_ADDR                                         (MSS_QDSP6V671SS_PUB_REG_BASE            + 0x420)
#define HWIO_MSS_QDSP6SS_LDO_CTL_CFG_OFFS                                         (MSS_QDSP6V671SS_PUB_REG_BASE_OFFS + 0x420)
#define HWIO_MSS_QDSP6SS_LDO_CTL_CFG_RMSK                                                0x7
#define HWIO_MSS_QDSP6SS_LDO_CTL_CFG_IN                    \
                in_dword_masked(HWIO_MSS_QDSP6SS_LDO_CTL_CFG_ADDR, HWIO_MSS_QDSP6SS_LDO_CTL_CFG_RMSK)
#define HWIO_MSS_QDSP6SS_LDO_CTL_CFG_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_LDO_CTL_CFG_ADDR, m)
#define HWIO_MSS_QDSP6SS_LDO_CTL_CFG_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_LDO_CTL_CFG_ADDR,v)
#define HWIO_MSS_QDSP6SS_LDO_CTL_CFG_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_QDSP6SS_LDO_CTL_CFG_ADDR,m,v,HWIO_MSS_QDSP6SS_LDO_CTL_CFG_IN)
#define HWIO_MSS_QDSP6SS_LDO_CTL_CFG_LDO_PWR_UP_BG_BMSK                                  0x4
#define HWIO_MSS_QDSP6SS_LDO_CTL_CFG_LDO_PWR_UP_BG_SHFT                                    2
#define HWIO_MSS_QDSP6SS_LDO_CTL_CFG_LDO_PWR_UP_BMSK                                     0x2
#define HWIO_MSS_QDSP6SS_LDO_CTL_CFG_LDO_PWR_UP_SHFT                                       1
#define HWIO_MSS_QDSP6SS_LDO_CTL_CFG_LDO_BYP_BMSK                                        0x1
#define HWIO_MSS_QDSP6SS_LDO_CTL_CFG_LDO_BYP_SHFT                                          0

#define HWIO_MSS_QDSP6SS_LDO_CTL_CMD_ADDR                                         (MSS_QDSP6V671SS_PUB_REG_BASE            + 0x424)
#define HWIO_MSS_QDSP6SS_LDO_CTL_CMD_OFFS                                         (MSS_QDSP6V671SS_PUB_REG_BASE_OFFS + 0x424)
#define HWIO_MSS_QDSP6SS_LDO_CTL_CMD_RMSK                                                0x7
#define HWIO_MSS_QDSP6SS_LDO_CTL_CMD_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_LDO_CTL_CMD_ADDR,v)
#define HWIO_MSS_QDSP6SS_LDO_CTL_CMD_LDO_PWR_UP_BG_UPDATE_BMSK                           0x4
#define HWIO_MSS_QDSP6SS_LDO_CTL_CMD_LDO_PWR_UP_BG_UPDATE_SHFT                             2
#define HWIO_MSS_QDSP6SS_LDO_CTL_CMD_LDO_PWR_UP_UPDATE_BMSK                              0x2
#define HWIO_MSS_QDSP6SS_LDO_CTL_CMD_LDO_PWR_UP_UPDATE_SHFT                                1
#define HWIO_MSS_QDSP6SS_LDO_CTL_CMD_LDO_BYP_UPDATE_BMSK                                 0x1
#define HWIO_MSS_QDSP6SS_LDO_CTL_CMD_LDO_BYP_UPDATE_SHFT                                   0

#define HWIO_MSS_QDSP6SS_LDO_CTL_STATUS_ADDR                                      (MSS_QDSP6V671SS_PUB_REG_BASE            + 0x428)
#define HWIO_MSS_QDSP6SS_LDO_CTL_STATUS_OFFS                                      (MSS_QDSP6V671SS_PUB_REG_BASE_OFFS + 0x428)
#define HWIO_MSS_QDSP6SS_LDO_CTL_STATUS_RMSK                                             0x7
#define HWIO_MSS_QDSP6SS_LDO_CTL_STATUS_IN                    \
                in_dword_masked(HWIO_MSS_QDSP6SS_LDO_CTL_STATUS_ADDR, HWIO_MSS_QDSP6SS_LDO_CTL_STATUS_RMSK)
#define HWIO_MSS_QDSP6SS_LDO_CTL_STATUS_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_LDO_CTL_STATUS_ADDR, m)
#define HWIO_MSS_QDSP6SS_LDO_CTL_STATUS_LDO_PWR_UP_BG_BMSK                               0x4
#define HWIO_MSS_QDSP6SS_LDO_CTL_STATUS_LDO_PWR_UP_BG_SHFT                                 2
#define HWIO_MSS_QDSP6SS_LDO_CTL_STATUS_LDO_PWR_UP_BMSK                                  0x2
#define HWIO_MSS_QDSP6SS_LDO_CTL_STATUS_LDO_PWR_UP_SHFT                                    1
#define HWIO_MSS_QDSP6SS_LDO_CTL_STATUS_LDO_BYP_BMSK                                     0x1
#define HWIO_MSS_QDSP6SS_LDO_CTL_STATUS_LDO_BYP_SHFT                                       0

#define HWIO_MSS_QDSP6SS_MEM_CFG_ADDR                                             (MSS_QDSP6V671SS_PUB_REG_BASE            + 0x430)
#define HWIO_MSS_QDSP6SS_MEM_CFG_OFFS                                             (MSS_QDSP6V671SS_PUB_REG_BASE_OFFS + 0x430)
#define HWIO_MSS_QDSP6SS_MEM_CFG_RMSK                                               0x1f001f
#define HWIO_MSS_QDSP6SS_MEM_CFG_IN                    \
                in_dword_masked(HWIO_MSS_QDSP6SS_MEM_CFG_ADDR, HWIO_MSS_QDSP6SS_MEM_CFG_RMSK)
#define HWIO_MSS_QDSP6SS_MEM_CFG_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_MEM_CFG_ADDR, m)
#define HWIO_MSS_QDSP6SS_MEM_CFG_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_MEM_CFG_ADDR,v)
#define HWIO_MSS_QDSP6SS_MEM_CFG_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_QDSP6SS_MEM_CFG_ADDR,m,v,HWIO_MSS_QDSP6SS_MEM_CFG_IN)
#define HWIO_MSS_QDSP6SS_MEM_CFG_L2MEM_SLP_RET_N_BMSK                               0x1e0000
#define HWIO_MSS_QDSP6SS_MEM_CFG_L2MEM_SLP_RET_N_SHFT                                     17
#define HWIO_MSS_QDSP6SS_MEM_CFG_MEM_SLP_RET_N_BMSK                                  0x10000
#define HWIO_MSS_QDSP6SS_MEM_CFG_MEM_SLP_RET_N_SHFT                                       16
#define HWIO_MSS_QDSP6SS_MEM_CFG_L2MEM_SLP_NRET_N_BMSK                                  0x1e
#define HWIO_MSS_QDSP6SS_MEM_CFG_L2MEM_SLP_NRET_N_SHFT                                     1
#define HWIO_MSS_QDSP6SS_MEM_CFG_MEM_SLP_NRET_N_BMSK                                     0x1
#define HWIO_MSS_QDSP6SS_MEM_CFG_MEM_SLP_NRET_N_SHFT                                       0

#define HWIO_MSS_QDSP6SS_MEM_CMD_ADDR                                             (MSS_QDSP6V671SS_PUB_REG_BASE            + 0x434)
#define HWIO_MSS_QDSP6SS_MEM_CMD_OFFS                                             (MSS_QDSP6V671SS_PUB_REG_BASE_OFFS + 0x434)
#define HWIO_MSS_QDSP6SS_MEM_CMD_RMSK                                                    0x3
#define HWIO_MSS_QDSP6SS_MEM_CMD_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_MEM_CMD_ADDR,v)
#define HWIO_MSS_QDSP6SS_MEM_CMD_UPDATE_SLP_RET_N_BMSK                                   0x2
#define HWIO_MSS_QDSP6SS_MEM_CMD_UPDATE_SLP_RET_N_SHFT                                     1
#define HWIO_MSS_QDSP6SS_MEM_CMD_UPDATE_SLP_NRET_N_BMSK                                  0x1
#define HWIO_MSS_QDSP6SS_MEM_CMD_UPDATE_SLP_NRET_N_SHFT                                    0

#define HWIO_MSS_QDSP6SS_MEM_STATUS_ADDR                                          (MSS_QDSP6V671SS_PUB_REG_BASE            + 0x438)
#define HWIO_MSS_QDSP6SS_MEM_STATUS_OFFS                                          (MSS_QDSP6V671SS_PUB_REG_BASE_OFFS + 0x438)
#define HWIO_MSS_QDSP6SS_MEM_STATUS_RMSK                                            0x1f001f
#define HWIO_MSS_QDSP6SS_MEM_STATUS_IN                    \
                in_dword_masked(HWIO_MSS_QDSP6SS_MEM_STATUS_ADDR, HWIO_MSS_QDSP6SS_MEM_STATUS_RMSK)
#define HWIO_MSS_QDSP6SS_MEM_STATUS_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_MEM_STATUS_ADDR, m)
#define HWIO_MSS_QDSP6SS_MEM_STATUS_L2MEM_SLP_RET_N_BMSK                            0x1e0000
#define HWIO_MSS_QDSP6SS_MEM_STATUS_L2MEM_SLP_RET_N_SHFT                                  17
#define HWIO_MSS_QDSP6SS_MEM_STATUS_MEM_SLP_RET_N_BMSK                               0x10000
#define HWIO_MSS_QDSP6SS_MEM_STATUS_MEM_SLP_RET_N_SHFT                                    16
#define HWIO_MSS_QDSP6SS_MEM_STATUS_L2MEM_SLP_NRET_N_BMSK                               0x1e
#define HWIO_MSS_QDSP6SS_MEM_STATUS_L2MEM_SLP_NRET_N_SHFT                                  1
#define HWIO_MSS_QDSP6SS_MEM_STATUS_MEM_SLP_NRET_N_BMSK                                  0x1
#define HWIO_MSS_QDSP6SS_MEM_STATUS_MEM_SLP_NRET_N_SHFT                                    0

#define HWIO_MSS_QDSP6SS_RESET_CFG_ADDR                                           (MSS_QDSP6V671SS_PUB_REG_BASE            + 0x440)
#define HWIO_MSS_QDSP6SS_RESET_CFG_OFFS                                           (MSS_QDSP6V671SS_PUB_REG_BASE_OFFS + 0x440)
#define HWIO_MSS_QDSP6SS_RESET_CFG_RMSK                                              0x1ffff
#define HWIO_MSS_QDSP6SS_RESET_CFG_IN                    \
                in_dword_masked(HWIO_MSS_QDSP6SS_RESET_CFG_ADDR, HWIO_MSS_QDSP6SS_RESET_CFG_RMSK)
#define HWIO_MSS_QDSP6SS_RESET_CFG_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_RESET_CFG_ADDR, m)
#define HWIO_MSS_QDSP6SS_RESET_CFG_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_RESET_CFG_ADDR,v)
#define HWIO_MSS_QDSP6SS_RESET_CFG_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_QDSP6SS_RESET_CFG_ADDR,m,v,HWIO_MSS_QDSP6SS_RESET_CFG_IN)
#define HWIO_MSS_QDSP6SS_RESET_CFG_CLADE2_BMSK                                       0x10000
#define HWIO_MSS_QDSP6SS_RESET_CFG_CLADE2_SHFT                                            16
#define HWIO_MSS_QDSP6SS_RESET_CFG_VPEVU_BMSK                                         0x8000
#define HWIO_MSS_QDSP6SS_RESET_CFG_VPEVU_SHFT                                             15
#define HWIO_MSS_QDSP6SS_RESET_CFG_NRET_BMSK                                          0x4000
#define HWIO_MSS_QDSP6SS_RESET_CFG_NRET_SHFT                                              14
#define HWIO_MSS_QDSP6SS_RESET_CFG_MNOC_BMSK                                          0x2000
#define HWIO_MSS_QDSP6SS_RESET_CFG_MNOC_SHFT                                              13
#define HWIO_MSS_QDSP6SS_RESET_CFG_APB_BMSK                                           0x1000
#define HWIO_MSS_QDSP6SS_RESET_CFG_APB_SHFT                                               12
#define HWIO_MSS_QDSP6SS_RESET_CFG_AXIS2_BMSK                                          0x800
#define HWIO_MSS_QDSP6SS_RESET_CFG_AXIS2_SHFT                                             11
#define HWIO_MSS_QDSP6SS_RESET_CFG_CSI1_BMSK                                           0x400
#define HWIO_MSS_QDSP6SS_RESET_CFG_CSI1_SHFT                                              10
#define HWIO_MSS_QDSP6SS_RESET_CFG_CSI0_BMSK                                           0x200
#define HWIO_MSS_QDSP6SS_RESET_CFG_CSI0_SHFT                                               9
#define HWIO_MSS_QDSP6SS_RESET_CFG_ARCH_BMSK                                           0x100
#define HWIO_MSS_QDSP6SS_RESET_CFG_ARCH_SHFT                                               8
#define HWIO_MSS_QDSP6SS_RESET_CFG_CORE_BMSK                                            0x80
#define HWIO_MSS_QDSP6SS_RESET_CFG_CORE_SHFT                                               7
#define HWIO_MSS_QDSP6SS_RESET_CFG_AXIM2_BMSK                                           0x40
#define HWIO_MSS_QDSP6SS_RESET_CFG_AXIM2_SHFT                                              6
#define HWIO_MSS_QDSP6SS_RESET_CFG_AXIS_BMSK                                            0x20
#define HWIO_MSS_QDSP6SS_RESET_CFG_AXIS_SHFT                                               5
#define HWIO_MSS_QDSP6SS_RESET_CFG_ATBM_BMSK                                            0x10
#define HWIO_MSS_QDSP6SS_RESET_CFG_ATBM_SHFT                                               4
#define HWIO_MSS_QDSP6SS_RESET_CFG_AHBM_BMSK                                             0x8
#define HWIO_MSS_QDSP6SS_RESET_CFG_AHBM_SHFT                                               3
#define HWIO_MSS_QDSP6SS_RESET_CFG_AXIM_BMSK                                             0x4
#define HWIO_MSS_QDSP6SS_RESET_CFG_AXIM_SHFT                                               2
#define HWIO_MSS_QDSP6SS_RESET_CFG_ISDB_ETM_BMSK                                         0x2
#define HWIO_MSS_QDSP6SS_RESET_CFG_ISDB_ETM_SHFT                                           1
#define HWIO_MSS_QDSP6SS_RESET_CFG_RET_BMSK                                              0x1
#define HWIO_MSS_QDSP6SS_RESET_CFG_RET_SHFT                                                0

#define HWIO_MSS_QDSP6SS_RESET_CMD_ADDR                                           (MSS_QDSP6V671SS_PUB_REG_BASE            + 0x444)
#define HWIO_MSS_QDSP6SS_RESET_CMD_OFFS                                           (MSS_QDSP6V671SS_PUB_REG_BASE_OFFS + 0x444)
#define HWIO_MSS_QDSP6SS_RESET_CMD_RMSK                                                  0x1
#define HWIO_MSS_QDSP6SS_RESET_CMD_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_RESET_CMD_ADDR,v)
#define HWIO_MSS_QDSP6SS_RESET_CMD_UPDATE_BMSK                                           0x1
#define HWIO_MSS_QDSP6SS_RESET_CMD_UPDATE_SHFT                                             0

#define HWIO_MSS_QDSP6SS_RESET_STATUS_ADDR                                        (MSS_QDSP6V671SS_PUB_REG_BASE            + 0x448)
#define HWIO_MSS_QDSP6SS_RESET_STATUS_OFFS                                        (MSS_QDSP6V671SS_PUB_REG_BASE_OFFS + 0x448)
#define HWIO_MSS_QDSP6SS_RESET_STATUS_RMSK                                           0x1ffff
#define HWIO_MSS_QDSP6SS_RESET_STATUS_IN                    \
                in_dword_masked(HWIO_MSS_QDSP6SS_RESET_STATUS_ADDR, HWIO_MSS_QDSP6SS_RESET_STATUS_RMSK)
#define HWIO_MSS_QDSP6SS_RESET_STATUS_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_RESET_STATUS_ADDR, m)
#define HWIO_MSS_QDSP6SS_RESET_STATUS_CLADE2_BMSK                                    0x10000
#define HWIO_MSS_QDSP6SS_RESET_STATUS_CLADE2_SHFT                                         16
#define HWIO_MSS_QDSP6SS_RESET_STATUS_VPEVU_BMSK                                      0x8000
#define HWIO_MSS_QDSP6SS_RESET_STATUS_VPEVU_SHFT                                          15
#define HWIO_MSS_QDSP6SS_RESET_STATUS_NRET_BMSK                                       0x4000
#define HWIO_MSS_QDSP6SS_RESET_STATUS_NRET_SHFT                                           14
#define HWIO_MSS_QDSP6SS_RESET_STATUS_MNOC_BMSK                                       0x2000
#define HWIO_MSS_QDSP6SS_RESET_STATUS_MNOC_SHFT                                           13
#define HWIO_MSS_QDSP6SS_RESET_STATUS_APB_BMSK                                        0x1000
#define HWIO_MSS_QDSP6SS_RESET_STATUS_APB_SHFT                                            12
#define HWIO_MSS_QDSP6SS_RESET_STATUS_AXIS2_BMSK                                       0x800
#define HWIO_MSS_QDSP6SS_RESET_STATUS_AXIS2_SHFT                                          11
#define HWIO_MSS_QDSP6SS_RESET_STATUS_CSI1_BMSK                                        0x400
#define HWIO_MSS_QDSP6SS_RESET_STATUS_CSI1_SHFT                                           10
#define HWIO_MSS_QDSP6SS_RESET_STATUS_CSI0_BMSK                                        0x200
#define HWIO_MSS_QDSP6SS_RESET_STATUS_CSI0_SHFT                                            9
#define HWIO_MSS_QDSP6SS_RESET_STATUS_ARCH_BMSK                                        0x100
#define HWIO_MSS_QDSP6SS_RESET_STATUS_ARCH_SHFT                                            8
#define HWIO_MSS_QDSP6SS_RESET_STATUS_CORE_BMSK                                         0x80
#define HWIO_MSS_QDSP6SS_RESET_STATUS_CORE_SHFT                                            7
#define HWIO_MSS_QDSP6SS_RESET_STATUS_AXIM2_BMSK                                        0x40
#define HWIO_MSS_QDSP6SS_RESET_STATUS_AXIM2_SHFT                                           6
#define HWIO_MSS_QDSP6SS_RESET_STATUS_AXIS_BMSK                                         0x20
#define HWIO_MSS_QDSP6SS_RESET_STATUS_AXIS_SHFT                                            5
#define HWIO_MSS_QDSP6SS_RESET_STATUS_ATBM_BMSK                                         0x10
#define HWIO_MSS_QDSP6SS_RESET_STATUS_ATBM_SHFT                                            4
#define HWIO_MSS_QDSP6SS_RESET_STATUS_AHBM_BMSK                                          0x8
#define HWIO_MSS_QDSP6SS_RESET_STATUS_AHBM_SHFT                                            3
#define HWIO_MSS_QDSP6SS_RESET_STATUS_AXIM_BMSK                                          0x4
#define HWIO_MSS_QDSP6SS_RESET_STATUS_AXIM_SHFT                                            2
#define HWIO_MSS_QDSP6SS_RESET_STATUS_ISDB_ETM_BMSK                                      0x2
#define HWIO_MSS_QDSP6SS_RESET_STATUS_ISDB_ETM_SHFT                                        1
#define HWIO_MSS_QDSP6SS_RESET_STATUS_RET_BMSK                                           0x1
#define HWIO_MSS_QDSP6SS_RESET_STATUS_RET_SHFT                                             0

#define HWIO_MSS_QDSP6SS_CLAMP_CFG_ADDR                                           (MSS_QDSP6V671SS_PUB_REG_BASE            + 0x450)
#define HWIO_MSS_QDSP6SS_CLAMP_CFG_OFFS                                           (MSS_QDSP6V671SS_PUB_REG_BASE_OFFS + 0x450)
#define HWIO_MSS_QDSP6SS_CLAMP_CFG_RMSK                                                  0xd
#define HWIO_MSS_QDSP6SS_CLAMP_CFG_IN                    \
                in_dword_masked(HWIO_MSS_QDSP6SS_CLAMP_CFG_ADDR, HWIO_MSS_QDSP6SS_CLAMP_CFG_RMSK)
#define HWIO_MSS_QDSP6SS_CLAMP_CFG_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_CLAMP_CFG_ADDR, m)
#define HWIO_MSS_QDSP6SS_CLAMP_CFG_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_CLAMP_CFG_ADDR,v)
#define HWIO_MSS_QDSP6SS_CLAMP_CFG_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_QDSP6SS_CLAMP_CFG_ADDR,m,v,HWIO_MSS_QDSP6SS_CLAMP_CFG_IN)
#define HWIO_MSS_QDSP6SS_CLAMP_CFG_CLAMP_MSS2CX_BMSK                                     0x8
#define HWIO_MSS_QDSP6SS_CLAMP_CFG_CLAMP_MSS2CX_SHFT                                       3
#define HWIO_MSS_QDSP6SS_CLAMP_CFG_CLAMP_QMC_MEM_BMSK                                    0x4
#define HWIO_MSS_QDSP6SS_CLAMP_CFG_CLAMP_QMC_MEM_SHFT                                      2
#define HWIO_MSS_QDSP6SS_CLAMP_CFG_CLAMP_IO_BMSK                                         0x1
#define HWIO_MSS_QDSP6SS_CLAMP_CFG_CLAMP_IO_SHFT                                           0

#define HWIO_MSS_QDSP6SS_CLAMP_CMD_ADDR                                           (MSS_QDSP6V671SS_PUB_REG_BASE            + 0x454)
#define HWIO_MSS_QDSP6SS_CLAMP_CMD_OFFS                                           (MSS_QDSP6V671SS_PUB_REG_BASE_OFFS + 0x454)
#define HWIO_MSS_QDSP6SS_CLAMP_CMD_RMSK                                                  0xd
#define HWIO_MSS_QDSP6SS_CLAMP_CMD_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_CLAMP_CMD_ADDR,v)
#define HWIO_MSS_QDSP6SS_CLAMP_CMD_UPDATE_CLAMP_MSS2CX_BMSK                              0x8
#define HWIO_MSS_QDSP6SS_CLAMP_CMD_UPDATE_CLAMP_MSS2CX_SHFT                                3
#define HWIO_MSS_QDSP6SS_CLAMP_CMD_UPDATE_CLAMP_QMC_MEM_BMSK                             0x4
#define HWIO_MSS_QDSP6SS_CLAMP_CMD_UPDATE_CLAMP_QMC_MEM_SHFT                               2
#define HWIO_MSS_QDSP6SS_CLAMP_CMD_UPDATE_CLAMP_IO_BMSK                                  0x1
#define HWIO_MSS_QDSP6SS_CLAMP_CMD_UPDATE_CLAMP_IO_SHFT                                    0

#define HWIO_MSS_QDSP6SS_CLAMP_STATUS_ADDR                                        (MSS_QDSP6V671SS_PUB_REG_BASE            + 0x458)
#define HWIO_MSS_QDSP6SS_CLAMP_STATUS_OFFS                                        (MSS_QDSP6V671SS_PUB_REG_BASE_OFFS + 0x458)
#define HWIO_MSS_QDSP6SS_CLAMP_STATUS_RMSK                                               0xd
#define HWIO_MSS_QDSP6SS_CLAMP_STATUS_IN                    \
                in_dword_masked(HWIO_MSS_QDSP6SS_CLAMP_STATUS_ADDR, HWIO_MSS_QDSP6SS_CLAMP_STATUS_RMSK)
#define HWIO_MSS_QDSP6SS_CLAMP_STATUS_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_CLAMP_STATUS_ADDR, m)
#define HWIO_MSS_QDSP6SS_CLAMP_STATUS_CLAMP_MSS2CX_BMSK                                  0x8
#define HWIO_MSS_QDSP6SS_CLAMP_STATUS_CLAMP_MSS2CX_SHFT                                    3
#define HWIO_MSS_QDSP6SS_CLAMP_STATUS_CLAMP_QMC_MEM_BMSK                                 0x4
#define HWIO_MSS_QDSP6SS_CLAMP_STATUS_CLAMP_QMC_MEM_SHFT                                   2
#define HWIO_MSS_QDSP6SS_CLAMP_STATUS_CLAMP_IO_BMSK                                      0x1
#define HWIO_MSS_QDSP6SS_CLAMP_STATUS_CLAMP_IO_SHFT                                        0

#define HWIO_MSS_QDSP6SS_CLK_CFG_ADDR                                             (MSS_QDSP6V671SS_PUB_REG_BASE            + 0x460)
#define HWIO_MSS_QDSP6SS_CLK_CFG_OFFS                                             (MSS_QDSP6V671SS_PUB_REG_BASE_OFFS + 0x460)
#define HWIO_MSS_QDSP6SS_CLK_CFG_RMSK                                                  0x7ff
#define HWIO_MSS_QDSP6SS_CLK_CFG_IN                    \
                in_dword_masked(HWIO_MSS_QDSP6SS_CLK_CFG_ADDR, HWIO_MSS_QDSP6SS_CLK_CFG_RMSK)
#define HWIO_MSS_QDSP6SS_CLK_CFG_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_CLK_CFG_ADDR, m)
#define HWIO_MSS_QDSP6SS_CLK_CFG_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_CLK_CFG_ADDR,v)
#define HWIO_MSS_QDSP6SS_CLK_CFG_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_QDSP6SS_CLK_CFG_ADDR,m,v,HWIO_MSS_QDSP6SS_CLK_CFG_IN)
#define HWIO_MSS_QDSP6SS_CLK_CFG_VPEVU_BMSK                                            0x400
#define HWIO_MSS_QDSP6SS_CLK_CFG_VPEVU_SHFT                                               10
#define HWIO_MSS_QDSP6SS_CLK_CFG_AXIS2_BMSK                                            0x200
#define HWIO_MSS_QDSP6SS_CLK_CFG_AXIS2_SHFT                                                9
#define HWIO_MSS_QDSP6SS_CLK_CFG_CSI1_BMSK                                             0x100
#define HWIO_MSS_QDSP6SS_CLK_CFG_CSI1_SHFT                                                 8
#define HWIO_MSS_QDSP6SS_CLK_CFG_CSI0_BMSK                                              0x80
#define HWIO_MSS_QDSP6SS_CLK_CFG_CSI0_SHFT                                                 7
#define HWIO_MSS_QDSP6SS_CLK_CFG_AXIM2_BMSK                                             0x40
#define HWIO_MSS_QDSP6SS_CLK_CFG_AXIM2_SHFT                                                6
#define HWIO_MSS_QDSP6SS_CLK_CFG_AXIS_BMSK                                              0x20
#define HWIO_MSS_QDSP6SS_CLK_CFG_AXIS_SHFT                                                 5
#define HWIO_MSS_QDSP6SS_CLK_CFG_ATBM_BMSK                                              0x10
#define HWIO_MSS_QDSP6SS_CLK_CFG_ATBM_SHFT                                                 4
#define HWIO_MSS_QDSP6SS_CLK_CFG_AHBM_BMSK                                               0x8
#define HWIO_MSS_QDSP6SS_CLK_CFG_AHBM_SHFT                                                 3
#define HWIO_MSS_QDSP6SS_CLK_CFG_AXIM_BMSK                                               0x4
#define HWIO_MSS_QDSP6SS_CLK_CFG_AXIM_SHFT                                                 2
#define HWIO_MSS_QDSP6SS_CLK_CFG_APB_BMSK                                                0x2
#define HWIO_MSS_QDSP6SS_CLK_CFG_APB_SHFT                                                  1
#define HWIO_MSS_QDSP6SS_CLK_CFG_CORE_BMSK                                               0x1
#define HWIO_MSS_QDSP6SS_CLK_CFG_CORE_SHFT                                                 0

#define HWIO_MSS_QDSP6SS_CLK_CMD_ADDR                                             (MSS_QDSP6V671SS_PUB_REG_BASE            + 0x464)
#define HWIO_MSS_QDSP6SS_CLK_CMD_OFFS                                             (MSS_QDSP6V671SS_PUB_REG_BASE_OFFS + 0x464)
#define HWIO_MSS_QDSP6SS_CLK_CMD_RMSK                                                    0x1
#define HWIO_MSS_QDSP6SS_CLK_CMD_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_CLK_CMD_ADDR,v)
#define HWIO_MSS_QDSP6SS_CLK_CMD_UPDATE_CLK_BMSK                                         0x1
#define HWIO_MSS_QDSP6SS_CLK_CMD_UPDATE_CLK_SHFT                                           0

#define HWIO_MSS_QDSP6SS_CLK_STATUS_ADDR                                          (MSS_QDSP6V671SS_PUB_REG_BASE            + 0x468)
#define HWIO_MSS_QDSP6SS_CLK_STATUS_OFFS                                          (MSS_QDSP6V671SS_PUB_REG_BASE_OFFS + 0x468)
#define HWIO_MSS_QDSP6SS_CLK_STATUS_RMSK                                               0x7ff
#define HWIO_MSS_QDSP6SS_CLK_STATUS_IN                    \
                in_dword_masked(HWIO_MSS_QDSP6SS_CLK_STATUS_ADDR, HWIO_MSS_QDSP6SS_CLK_STATUS_RMSK)
#define HWIO_MSS_QDSP6SS_CLK_STATUS_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_CLK_STATUS_ADDR, m)
#define HWIO_MSS_QDSP6SS_CLK_STATUS_VPEVU_BMSK                                         0x400
#define HWIO_MSS_QDSP6SS_CLK_STATUS_VPEVU_SHFT                                            10
#define HWIO_MSS_QDSP6SS_CLK_STATUS_AXIS2_BMSK                                         0x200
#define HWIO_MSS_QDSP6SS_CLK_STATUS_AXIS2_SHFT                                             9
#define HWIO_MSS_QDSP6SS_CLK_STATUS_CSI1_BMSK                                          0x100
#define HWIO_MSS_QDSP6SS_CLK_STATUS_CSI1_SHFT                                              8
#define HWIO_MSS_QDSP6SS_CLK_STATUS_CSI0_BMSK                                           0x80
#define HWIO_MSS_QDSP6SS_CLK_STATUS_CSI0_SHFT                                              7
#define HWIO_MSS_QDSP6SS_CLK_STATUS_AXIM2_BMSK                                          0x40
#define HWIO_MSS_QDSP6SS_CLK_STATUS_AXIM2_SHFT                                             6
#define HWIO_MSS_QDSP6SS_CLK_STATUS_AXIS_BMSK                                           0x20
#define HWIO_MSS_QDSP6SS_CLK_STATUS_AXIS_SHFT                                              5
#define HWIO_MSS_QDSP6SS_CLK_STATUS_ATBM_BMSK                                           0x10
#define HWIO_MSS_QDSP6SS_CLK_STATUS_ATBM_SHFT                                              4
#define HWIO_MSS_QDSP6SS_CLK_STATUS_AHBM_BMSK                                            0x8
#define HWIO_MSS_QDSP6SS_CLK_STATUS_AHBM_SHFT                                              3
#define HWIO_MSS_QDSP6SS_CLK_STATUS_AXIM_BMSK                                            0x4
#define HWIO_MSS_QDSP6SS_CLK_STATUS_AXIM_SHFT                                              2
#define HWIO_MSS_QDSP6SS_CLK_STATUS_APB_BMSK                                             0x2
#define HWIO_MSS_QDSP6SS_CLK_STATUS_APB_SHFT                                               1
#define HWIO_MSS_QDSP6SS_CLK_STATUS_CORE_BMSK                                            0x1
#define HWIO_MSS_QDSP6SS_CLK_STATUS_CORE_SHFT                                              0

#define HWIO_MSS_QDSP6SS_MEM_STAGGER_RESET_CFG_ADDR                               (MSS_QDSP6V671SS_PUB_REG_BASE            + 0x470)
#define HWIO_MSS_QDSP6SS_MEM_STAGGER_RESET_CFG_OFFS                               (MSS_QDSP6V671SS_PUB_REG_BASE_OFFS + 0x470)
#define HWIO_MSS_QDSP6SS_MEM_STAGGER_RESET_CFG_RMSK                                      0x3
#define HWIO_MSS_QDSP6SS_MEM_STAGGER_RESET_CFG_IN                    \
                in_dword_masked(HWIO_MSS_QDSP6SS_MEM_STAGGER_RESET_CFG_ADDR, HWIO_MSS_QDSP6SS_MEM_STAGGER_RESET_CFG_RMSK)
#define HWIO_MSS_QDSP6SS_MEM_STAGGER_RESET_CFG_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_MEM_STAGGER_RESET_CFG_ADDR, m)
#define HWIO_MSS_QDSP6SS_MEM_STAGGER_RESET_CFG_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_MEM_STAGGER_RESET_CFG_ADDR,v)
#define HWIO_MSS_QDSP6SS_MEM_STAGGER_RESET_CFG_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_QDSP6SS_MEM_STAGGER_RESET_CFG_ADDR,m,v,HWIO_MSS_QDSP6SS_MEM_STAGGER_RESET_CFG_IN)
#define HWIO_MSS_QDSP6SS_MEM_STAGGER_RESET_CFG_STAGGER_ALT_ARES_BMSK                     0x2
#define HWIO_MSS_QDSP6SS_MEM_STAGGER_RESET_CFG_STAGGER_ALT_ARES_SHFT                       1
#define HWIO_MSS_QDSP6SS_MEM_STAGGER_RESET_CFG_STAGGER_FUNC_ARES_BMSK                    0x1
#define HWIO_MSS_QDSP6SS_MEM_STAGGER_RESET_CFG_STAGGER_FUNC_ARES_SHFT                      0

#define HWIO_MSS_QDSP6SS_MEM_STAGGER_RESET_CMD_ADDR                               (MSS_QDSP6V671SS_PUB_REG_BASE            + 0x474)
#define HWIO_MSS_QDSP6SS_MEM_STAGGER_RESET_CMD_OFFS                               (MSS_QDSP6V671SS_PUB_REG_BASE_OFFS + 0x474)
#define HWIO_MSS_QDSP6SS_MEM_STAGGER_RESET_CMD_RMSK                                      0x1
#define HWIO_MSS_QDSP6SS_MEM_STAGGER_RESET_CMD_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_MEM_STAGGER_RESET_CMD_ADDR,v)
#define HWIO_MSS_QDSP6SS_MEM_STAGGER_RESET_CMD_UPDATE_BMSK                               0x1
#define HWIO_MSS_QDSP6SS_MEM_STAGGER_RESET_CMD_UPDATE_SHFT                                 0

#define HWIO_MSS_QDSP6SS_MEM_STAGGER_RESET_STATUS_ADDR                            (MSS_QDSP6V671SS_PUB_REG_BASE            + 0x478)
#define HWIO_MSS_QDSP6SS_MEM_STAGGER_RESET_STATUS_OFFS                            (MSS_QDSP6V671SS_PUB_REG_BASE_OFFS + 0x478)
#define HWIO_MSS_QDSP6SS_MEM_STAGGER_RESET_STATUS_RMSK                                   0x3
#define HWIO_MSS_QDSP6SS_MEM_STAGGER_RESET_STATUS_IN                    \
                in_dword_masked(HWIO_MSS_QDSP6SS_MEM_STAGGER_RESET_STATUS_ADDR, HWIO_MSS_QDSP6SS_MEM_STAGGER_RESET_STATUS_RMSK)
#define HWIO_MSS_QDSP6SS_MEM_STAGGER_RESET_STATUS_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_MEM_STAGGER_RESET_STATUS_ADDR, m)
#define HWIO_MSS_QDSP6SS_MEM_STAGGER_RESET_STATUS_STAGGER_ALT_ARES_BMSK                  0x2
#define HWIO_MSS_QDSP6SS_MEM_STAGGER_RESET_STATUS_STAGGER_ALT_ARES_SHFT                    1
#define HWIO_MSS_QDSP6SS_MEM_STAGGER_RESET_STATUS_STAGGER_FUNC_ARES_BMSK                 0x1
#define HWIO_MSS_QDSP6SS_MEM_STAGGER_RESET_STATUS_STAGGER_FUNC_ARES_SHFT                   0

#define HWIO_MSS_QDSP6SS_MEM_STAGGER_RESTORE_CMD_ADDR                             (MSS_QDSP6V671SS_PUB_REG_BASE            + 0x484)
#define HWIO_MSS_QDSP6SS_MEM_STAGGER_RESTORE_CMD_OFFS                             (MSS_QDSP6V671SS_PUB_REG_BASE_OFFS + 0x484)
#define HWIO_MSS_QDSP6SS_MEM_STAGGER_RESTORE_CMD_RMSK                                    0x1
#define HWIO_MSS_QDSP6SS_MEM_STAGGER_RESTORE_CMD_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_MEM_STAGGER_RESTORE_CMD_ADDR,v)
#define HWIO_MSS_QDSP6SS_MEM_STAGGER_RESTORE_CMD_RESTORE_CMD_BMSK                        0x1
#define HWIO_MSS_QDSP6SS_MEM_STAGGER_RESTORE_CMD_RESTORE_CMD_SHFT                          0

#define HWIO_MSS_QDSP6SS_MEM_STAGGER_RESTORE_STATUS_ADDR                          (MSS_QDSP6V671SS_PUB_REG_BASE            + 0x488)
#define HWIO_MSS_QDSP6SS_MEM_STAGGER_RESTORE_STATUS_OFFS                          (MSS_QDSP6V671SS_PUB_REG_BASE_OFFS + 0x488)
#define HWIO_MSS_QDSP6SS_MEM_STAGGER_RESTORE_STATUS_RMSK                                 0x1
#define HWIO_MSS_QDSP6SS_MEM_STAGGER_RESTORE_STATUS_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_MEM_STAGGER_RESTORE_STATUS_ADDR,v)
#define HWIO_MSS_QDSP6SS_MEM_STAGGER_RESTORE_STATUS_RESTORE_BUSY_BMSK                    0x1
#define HWIO_MSS_QDSP6SS_MEM_STAGGER_RESTORE_STATUS_RESTORE_BUSY_SHFT                      0

#define HWIO_MSS_QDSP6SS_L2MEM_EFUSE_STATUS_ADDR                                  (MSS_QDSP6V671SS_PUB_REG_BASE            + 0x490)
#define HWIO_MSS_QDSP6SS_L2MEM_EFUSE_STATUS_OFFS                                  (MSS_QDSP6V671SS_PUB_REG_BASE_OFFS + 0x490)
#define HWIO_MSS_QDSP6SS_L2MEM_EFUSE_STATUS_RMSK                                         0xf
#define HWIO_MSS_QDSP6SS_L2MEM_EFUSE_STATUS_IN                    \
                in_dword_masked(HWIO_MSS_QDSP6SS_L2MEM_EFUSE_STATUS_ADDR, HWIO_MSS_QDSP6SS_L2MEM_EFUSE_STATUS_RMSK)
#define HWIO_MSS_QDSP6SS_L2MEM_EFUSE_STATUS_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_L2MEM_EFUSE_STATUS_ADDR, m)
#define HWIO_MSS_QDSP6SS_L2MEM_EFUSE_STATUS_EFUSE_L2TCM_EN_BMSK                          0xf
#define HWIO_MSS_QDSP6SS_L2MEM_EFUSE_STATUS_EFUSE_L2TCM_EN_SHFT                            0

#define HWIO_MSS_QDSP6SS_CP_CLK_CTL_ADDR                                          (MSS_QDSP6V671SS_PUB_REG_BASE            + 0x508)
#define HWIO_MSS_QDSP6SS_CP_CLK_CTL_OFFS                                          (MSS_QDSP6V671SS_PUB_REG_BASE_OFFS + 0x508)
#define HWIO_MSS_QDSP6SS_CP_CLK_CTL_RMSK                                                 0x1
#define HWIO_MSS_QDSP6SS_CP_CLK_CTL_IN                    \
                in_dword_masked(HWIO_MSS_QDSP6SS_CP_CLK_CTL_ADDR, HWIO_MSS_QDSP6SS_CP_CLK_CTL_RMSK)
#define HWIO_MSS_QDSP6SS_CP_CLK_CTL_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_CP_CLK_CTL_ADDR, m)
#define HWIO_MSS_QDSP6SS_CP_CLK_CTL_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_CP_CLK_CTL_ADDR,v)
#define HWIO_MSS_QDSP6SS_CP_CLK_CTL_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_QDSP6SS_CP_CLK_CTL_ADDR,m,v,HWIO_MSS_QDSP6SS_CP_CLK_CTL_IN)
#define HWIO_MSS_QDSP6SS_CP_CLK_CTL_CLK_ENA_BMSK                                         0x1
#define HWIO_MSS_QDSP6SS_CP_CLK_CTL_CLK_ENA_SHFT                                           0

#define HWIO_MSS_QDSP6SS_CPMEM_CFG_ADDR                                           (MSS_QDSP6V671SS_PUB_REG_BASE            + 0x520)
#define HWIO_MSS_QDSP6SS_CPMEM_CFG_OFFS                                           (MSS_QDSP6V671SS_PUB_REG_BASE_OFFS + 0x520)
#define HWIO_MSS_QDSP6SS_CPMEM_CFG_RMSK                                                  0x3
#define HWIO_MSS_QDSP6SS_CPMEM_CFG_IN                    \
                in_dword_masked(HWIO_MSS_QDSP6SS_CPMEM_CFG_ADDR, HWIO_MSS_QDSP6SS_CPMEM_CFG_RMSK)
#define HWIO_MSS_QDSP6SS_CPMEM_CFG_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_CPMEM_CFG_ADDR, m)
#define HWIO_MSS_QDSP6SS_CPMEM_CFG_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_CPMEM_CFG_ADDR,v)
#define HWIO_MSS_QDSP6SS_CPMEM_CFG_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_QDSP6SS_CPMEM_CFG_ADDR,m,v,HWIO_MSS_QDSP6SS_CPMEM_CFG_IN)
#define HWIO_MSS_QDSP6SS_CPMEM_CFG_VTCM_SLP_RET_N_BMSK                                   0x2
#define HWIO_MSS_QDSP6SS_CPMEM_CFG_VTCM_SLP_RET_N_SHFT                                     1
#define HWIO_MSS_QDSP6SS_CPMEM_CFG_VTCM_SLP_NRET_N_BMSK                                  0x1
#define HWIO_MSS_QDSP6SS_CPMEM_CFG_VTCM_SLP_NRET_N_SHFT                                    0

#define HWIO_MSS_QDSP6SS_CPMEM_CMD_ADDR                                           (MSS_QDSP6V671SS_PUB_REG_BASE            + 0x524)
#define HWIO_MSS_QDSP6SS_CPMEM_CMD_OFFS                                           (MSS_QDSP6V671SS_PUB_REG_BASE_OFFS + 0x524)
#define HWIO_MSS_QDSP6SS_CPMEM_CMD_RMSK                                                  0x3
#define HWIO_MSS_QDSP6SS_CPMEM_CMD_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_CPMEM_CMD_ADDR,v)
#define HWIO_MSS_QDSP6SS_CPMEM_CMD_UPDATE_VTCM_SLP_RET_N_BMSK                            0x2
#define HWIO_MSS_QDSP6SS_CPMEM_CMD_UPDATE_VTCM_SLP_RET_N_SHFT                              1
#define HWIO_MSS_QDSP6SS_CPMEM_CMD_UPDATE_VTCM_SLP_NRET_N_BMSK                           0x1
#define HWIO_MSS_QDSP6SS_CPMEM_CMD_UPDATE_VTCM_SLP_NRET_N_SHFT                             0

#define HWIO_MSS_QDSP6SS_CPMEM_STATUS_ADDR                                        (MSS_QDSP6V671SS_PUB_REG_BASE            + 0x528)
#define HWIO_MSS_QDSP6SS_CPMEM_STATUS_OFFS                                        (MSS_QDSP6V671SS_PUB_REG_BASE_OFFS + 0x528)
#define HWIO_MSS_QDSP6SS_CPMEM_STATUS_RMSK                                               0x3
#define HWIO_MSS_QDSP6SS_CPMEM_STATUS_IN                    \
                in_dword_masked(HWIO_MSS_QDSP6SS_CPMEM_STATUS_ADDR, HWIO_MSS_QDSP6SS_CPMEM_STATUS_RMSK)
#define HWIO_MSS_QDSP6SS_CPMEM_STATUS_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_CPMEM_STATUS_ADDR, m)
#define HWIO_MSS_QDSP6SS_CPMEM_STATUS_VTCM_SLP_RET_N_BMSK                                0x2
#define HWIO_MSS_QDSP6SS_CPMEM_STATUS_VTCM_SLP_RET_N_SHFT                                  1
#define HWIO_MSS_QDSP6SS_CPMEM_STATUS_VTCM_SLP_NRET_N_BMSK                               0x1
#define HWIO_MSS_QDSP6SS_CPMEM_STATUS_VTCM_SLP_NRET_N_SHFT                                 0

#define HWIO_MSS_QDSP6SS_L2ITCM_CFG_ADDR                                          (MSS_QDSP6V671SS_PUB_REG_BASE            + 0x530)
#define HWIO_MSS_QDSP6SS_L2ITCM_CFG_OFFS                                          (MSS_QDSP6V671SS_PUB_REG_BASE_OFFS + 0x530)
#define HWIO_MSS_QDSP6SS_L2ITCM_CFG_RMSK                                             0x30003
#define HWIO_MSS_QDSP6SS_L2ITCM_CFG_IN                    \
                in_dword_masked(HWIO_MSS_QDSP6SS_L2ITCM_CFG_ADDR, HWIO_MSS_QDSP6SS_L2ITCM_CFG_RMSK)
#define HWIO_MSS_QDSP6SS_L2ITCM_CFG_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_L2ITCM_CFG_ADDR, m)
#define HWIO_MSS_QDSP6SS_L2ITCM_CFG_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_L2ITCM_CFG_ADDR,v)
#define HWIO_MSS_QDSP6SS_L2ITCM_CFG_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_QDSP6SS_L2ITCM_CFG_ADDR,m,v,HWIO_MSS_QDSP6SS_L2ITCM_CFG_IN)
#define HWIO_MSS_QDSP6SS_L2ITCM_CFG_L2ITCM_SLP_RET_N_BMSK                            0x30000
#define HWIO_MSS_QDSP6SS_L2ITCM_CFG_L2ITCM_SLP_RET_N_SHFT                                 16
#define HWIO_MSS_QDSP6SS_L2ITCM_CFG_L2ITCM_SLP_NRET_N_BMSK                               0x3
#define HWIO_MSS_QDSP6SS_L2ITCM_CFG_L2ITCM_SLP_NRET_N_SHFT                                 0

#define HWIO_MSS_QDSP6SS_L2ITCM_CMD_ADDR                                          (MSS_QDSP6V671SS_PUB_REG_BASE            + 0x534)
#define HWIO_MSS_QDSP6SS_L2ITCM_CMD_OFFS                                          (MSS_QDSP6V671SS_PUB_REG_BASE_OFFS + 0x534)
#define HWIO_MSS_QDSP6SS_L2ITCM_CMD_RMSK                                                 0x3
#define HWIO_MSS_QDSP6SS_L2ITCM_CMD_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_L2ITCM_CMD_ADDR,v)
#define HWIO_MSS_QDSP6SS_L2ITCM_CMD_UPDATE_L2ITCM_SLP_RET_N_BMSK                         0x2
#define HWIO_MSS_QDSP6SS_L2ITCM_CMD_UPDATE_L2ITCM_SLP_RET_N_SHFT                           1
#define HWIO_MSS_QDSP6SS_L2ITCM_CMD_UPDATE_L2ITCM_SLP_NRET_N_BMSK                        0x1
#define HWIO_MSS_QDSP6SS_L2ITCM_CMD_UPDATE_L2ITCM_SLP_NRET_N_SHFT                          0

#define HWIO_MSS_QDSP6SS_L2ITCM_STATUS_ADDR                                       (MSS_QDSP6V671SS_PUB_REG_BASE            + 0x538)
#define HWIO_MSS_QDSP6SS_L2ITCM_STATUS_OFFS                                       (MSS_QDSP6V671SS_PUB_REG_BASE_OFFS + 0x538)
#define HWIO_MSS_QDSP6SS_L2ITCM_STATUS_RMSK                                          0x30003
#define HWIO_MSS_QDSP6SS_L2ITCM_STATUS_IN                    \
                in_dword_masked(HWIO_MSS_QDSP6SS_L2ITCM_STATUS_ADDR, HWIO_MSS_QDSP6SS_L2ITCM_STATUS_RMSK)
#define HWIO_MSS_QDSP6SS_L2ITCM_STATUS_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_L2ITCM_STATUS_ADDR, m)
#define HWIO_MSS_QDSP6SS_L2ITCM_STATUS_L2ITCM_SLP_RET_N_BMSK                         0x30000
#define HWIO_MSS_QDSP6SS_L2ITCM_STATUS_L2ITCM_SLP_RET_N_SHFT                              16
#define HWIO_MSS_QDSP6SS_L2ITCM_STATUS_L2ITCM_SLP_NRET_N_BMSK                            0x3
#define HWIO_MSS_QDSP6SS_L2ITCM_STATUS_L2ITCM_SLP_NRET_N_SHFT                              0

#define HWIO_MSS_QDSP6SS_L1SMEM_CFG_ADDR                                          (MSS_QDSP6V671SS_PUB_REG_BASE            + 0x540)
#define HWIO_MSS_QDSP6SS_L1SMEM_CFG_OFFS                                          (MSS_QDSP6V671SS_PUB_REG_BASE_OFFS + 0x540)
#define HWIO_MSS_QDSP6SS_L1SMEM_CFG_RMSK                                          0x3f1f3f1f
#define HWIO_MSS_QDSP6SS_L1SMEM_CFG_IN                    \
                in_dword_masked(HWIO_MSS_QDSP6SS_L1SMEM_CFG_ADDR, HWIO_MSS_QDSP6SS_L1SMEM_CFG_RMSK)
#define HWIO_MSS_QDSP6SS_L1SMEM_CFG_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_L1SMEM_CFG_ADDR, m)
#define HWIO_MSS_QDSP6SS_L1SMEM_CFG_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_L1SMEM_CFG_ADDR,v)
#define HWIO_MSS_QDSP6SS_L1SMEM_CFG_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_QDSP6SS_L1SMEM_CFG_ADDR,m,v,HWIO_MSS_QDSP6SS_L1SMEM_CFG_IN)
#define HWIO_MSS_QDSP6SS_L1SMEM_CFG_L1S2_SLP_RET_N_BMSK                           0x3f000000
#define HWIO_MSS_QDSP6SS_L1SMEM_CFG_L1S2_SLP_RET_N_SHFT                                   24
#define HWIO_MSS_QDSP6SS_L1SMEM_CFG_L1S_SLP_RET_N_BMSK                              0x1f0000
#define HWIO_MSS_QDSP6SS_L1SMEM_CFG_L1S_SLP_RET_N_SHFT                                    16
#define HWIO_MSS_QDSP6SS_L1SMEM_CFG_L1S2_SLP_NRET_N_BMSK                              0x3f00
#define HWIO_MSS_QDSP6SS_L1SMEM_CFG_L1S2_SLP_NRET_N_SHFT                                   8
#define HWIO_MSS_QDSP6SS_L1SMEM_CFG_L1S_SLP_NRET_N_BMSK                                 0x1f
#define HWIO_MSS_QDSP6SS_L1SMEM_CFG_L1S_SLP_NRET_N_SHFT                                    0

#define HWIO_MSS_QDSP6SS_L1SMEM_CMD_ADDR                                          (MSS_QDSP6V671SS_PUB_REG_BASE            + 0x544)
#define HWIO_MSS_QDSP6SS_L1SMEM_CMD_OFFS                                          (MSS_QDSP6V671SS_PUB_REG_BASE_OFFS + 0x544)
#define HWIO_MSS_QDSP6SS_L1SMEM_CMD_RMSK                                                 0xf
#define HWIO_MSS_QDSP6SS_L1SMEM_CMD_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_L1SMEM_CMD_ADDR,v)
#define HWIO_MSS_QDSP6SS_L1SMEM_CMD_UPDATE_L1S2_SLP_RET_N_BMSK                           0x8
#define HWIO_MSS_QDSP6SS_L1SMEM_CMD_UPDATE_L1S2_SLP_RET_N_SHFT                             3
#define HWIO_MSS_QDSP6SS_L1SMEM_CMD_UPDATE_L1S2_SLP_NRET_N_BMSK                          0x4
#define HWIO_MSS_QDSP6SS_L1SMEM_CMD_UPDATE_L1S2_SLP_NRET_N_SHFT                            2
#define HWIO_MSS_QDSP6SS_L1SMEM_CMD_UPDATE_L1S_SLP_RET_N_BMSK                            0x2
#define HWIO_MSS_QDSP6SS_L1SMEM_CMD_UPDATE_L1S_SLP_RET_N_SHFT                              1
#define HWIO_MSS_QDSP6SS_L1SMEM_CMD_UPDATE_L1S_SLP_NRET_N_BMSK                           0x1
#define HWIO_MSS_QDSP6SS_L1SMEM_CMD_UPDATE_L1S_SLP_NRET_N_SHFT                             0

#define HWIO_MSS_QDSP6SS_L1SMEM_STATUS_ADDR                                       (MSS_QDSP6V671SS_PUB_REG_BASE            + 0x548)
#define HWIO_MSS_QDSP6SS_L1SMEM_STATUS_OFFS                                       (MSS_QDSP6V671SS_PUB_REG_BASE_OFFS + 0x548)
#define HWIO_MSS_QDSP6SS_L1SMEM_STATUS_RMSK                                       0x3f1f3f1f
#define HWIO_MSS_QDSP6SS_L1SMEM_STATUS_IN                    \
                in_dword_masked(HWIO_MSS_QDSP6SS_L1SMEM_STATUS_ADDR, HWIO_MSS_QDSP6SS_L1SMEM_STATUS_RMSK)
#define HWIO_MSS_QDSP6SS_L1SMEM_STATUS_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_L1SMEM_STATUS_ADDR, m)
#define HWIO_MSS_QDSP6SS_L1SMEM_STATUS_L1S2_SLP_RET_N_BMSK                        0x3f000000
#define HWIO_MSS_QDSP6SS_L1SMEM_STATUS_L1S2_SLP_RET_N_SHFT                                24
#define HWIO_MSS_QDSP6SS_L1SMEM_STATUS_L1S_SLP_RET_N_BMSK                           0x1f0000
#define HWIO_MSS_QDSP6SS_L1SMEM_STATUS_L1S_SLP_RET_N_SHFT                                 16
#define HWIO_MSS_QDSP6SS_L1SMEM_STATUS_L1S2_SLP_NRET_N_BMSK                           0x3f00
#define HWIO_MSS_QDSP6SS_L1SMEM_STATUS_L1S2_SLP_NRET_N_SHFT                                8
#define HWIO_MSS_QDSP6SS_L1SMEM_STATUS_L1S_SLP_NRET_N_BMSK                              0x1f
#define HWIO_MSS_QDSP6SS_L1SMEM_STATUS_L1S_SLP_NRET_N_SHFT                                 0

#define HWIO_MSS_QDSP6SS_VU0_PWR_VOTE_SET_ADDR                                    (MSS_QDSP6V671SS_PUB_REG_BASE            + 0x600)
#define HWIO_MSS_QDSP6SS_VU0_PWR_VOTE_SET_OFFS                                    (MSS_QDSP6V671SS_PUB_REG_BASE_OFFS + 0x600)
#define HWIO_MSS_QDSP6SS_VU0_PWR_VOTE_SET_RMSK                                          0xff
#define HWIO_MSS_QDSP6SS_VU0_PWR_VOTE_SET_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_VU0_PWR_VOTE_SET_ADDR,v)
#define HWIO_MSS_QDSP6SS_VU0_PWR_VOTE_SET_SET_BMSK                                      0xff
#define HWIO_MSS_QDSP6SS_VU0_PWR_VOTE_SET_SET_SHFT                                         0

#define HWIO_MSS_QDSP6SS_VU0_PWR_VOTE_CLR_ADDR                                    (MSS_QDSP6V671SS_PUB_REG_BASE            + 0x604)
#define HWIO_MSS_QDSP6SS_VU0_PWR_VOTE_CLR_OFFS                                    (MSS_QDSP6V671SS_PUB_REG_BASE_OFFS + 0x604)
#define HWIO_MSS_QDSP6SS_VU0_PWR_VOTE_CLR_RMSK                                         0xfff
#define HWIO_MSS_QDSP6SS_VU0_PWR_VOTE_CLR_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_VU0_PWR_VOTE_CLR_ADDR,v)
#define HWIO_MSS_QDSP6SS_VU0_PWR_VOTE_CLR_CLR_BMSK                                     0xfff
#define HWIO_MSS_QDSP6SS_VU0_PWR_VOTE_CLR_CLR_SHFT                                         0

#define HWIO_MSS_QDSP6SS_VU0_PWR_VOTE_MASK_ADDR                                   (MSS_QDSP6V671SS_PUB_REG_BASE            + 0x608)
#define HWIO_MSS_QDSP6SS_VU0_PWR_VOTE_MASK_OFFS                                   (MSS_QDSP6V671SS_PUB_REG_BASE_OFFS + 0x608)
#define HWIO_MSS_QDSP6SS_VU0_PWR_VOTE_MASK_RMSK                                        0xfff
#define HWIO_MSS_QDSP6SS_VU0_PWR_VOTE_MASK_IN                    \
                in_dword_masked(HWIO_MSS_QDSP6SS_VU0_PWR_VOTE_MASK_ADDR, HWIO_MSS_QDSP6SS_VU0_PWR_VOTE_MASK_RMSK)
#define HWIO_MSS_QDSP6SS_VU0_PWR_VOTE_MASK_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_VU0_PWR_VOTE_MASK_ADDR, m)
#define HWIO_MSS_QDSP6SS_VU0_PWR_VOTE_MASK_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_VU0_PWR_VOTE_MASK_ADDR,v)
#define HWIO_MSS_QDSP6SS_VU0_PWR_VOTE_MASK_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_QDSP6SS_VU0_PWR_VOTE_MASK_ADDR,m,v,HWIO_MSS_QDSP6SS_VU0_PWR_VOTE_MASK_IN)
#define HWIO_MSS_QDSP6SS_VU0_PWR_VOTE_MASK_HW_VOTE_EN_BMSK                             0xf00
#define HWIO_MSS_QDSP6SS_VU0_PWR_VOTE_MASK_HW_VOTE_EN_SHFT                                 8
#define HWIO_MSS_QDSP6SS_VU0_PWR_VOTE_MASK_VOTE_EN_BMSK                                 0xff
#define HWIO_MSS_QDSP6SS_VU0_PWR_VOTE_MASK_VOTE_EN_SHFT                                    0

#define HWIO_MSS_QDSP6SS_VU0_PWR_STATUS_ADDR                                      (MSS_QDSP6V671SS_PUB_REG_BASE            + 0x60c)
#define HWIO_MSS_QDSP6SS_VU0_PWR_STATUS_OFFS                                      (MSS_QDSP6V671SS_PUB_REG_BASE_OFFS + 0x60c)
#define HWIO_MSS_QDSP6SS_VU0_PWR_STATUS_RMSK                                         0xfff1f
#define HWIO_MSS_QDSP6SS_VU0_PWR_STATUS_IN                    \
                in_dword_masked(HWIO_MSS_QDSP6SS_VU0_PWR_STATUS_ADDR, HWIO_MSS_QDSP6SS_VU0_PWR_STATUS_RMSK)
#define HWIO_MSS_QDSP6SS_VU0_PWR_STATUS_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_VU0_PWR_STATUS_ADDR, m)
#define HWIO_MSS_QDSP6SS_VU0_PWR_STATUS_VOTE_STAT_BMSK                               0xfff00
#define HWIO_MSS_QDSP6SS_VU0_PWR_STATUS_VOTE_STAT_SHFT                                     8
#define HWIO_MSS_QDSP6SS_VU0_PWR_STATUS_WAIT_NEXT_BMSK                                  0x10
#define HWIO_MSS_QDSP6SS_VU0_PWR_STATUS_WAIT_NEXT_SHFT                                     4
#define HWIO_MSS_QDSP6SS_VU0_PWR_STATUS_WAIT_PREV_BMSK                                   0x8
#define HWIO_MSS_QDSP6SS_VU0_PWR_STATUS_WAIT_PREV_SHFT                                     3
#define HWIO_MSS_QDSP6SS_VU0_PWR_STATUS_PWR_DOWN_BMSK                                    0x4
#define HWIO_MSS_QDSP6SS_VU0_PWR_STATUS_PWR_DOWN_SHFT                                      2
#define HWIO_MSS_QDSP6SS_VU0_PWR_STATUS_PWR_REQ_BMSK                                     0x2
#define HWIO_MSS_QDSP6SS_VU0_PWR_STATUS_PWR_REQ_SHFT                                       1
#define HWIO_MSS_QDSP6SS_VU0_PWR_STATUS_PWR_ON_BMSK                                      0x1
#define HWIO_MSS_QDSP6SS_VU0_PWR_STATUS_PWR_ON_SHFT                                        0

#define HWIO_MSS_QDSP6SS_VU0_PWR_DEBUG_ADDR                                       (MSS_QDSP6V671SS_PUB_REG_BASE            + 0x610)
#define HWIO_MSS_QDSP6SS_VU0_PWR_DEBUG_OFFS                                       (MSS_QDSP6V671SS_PUB_REG_BASE_OFFS + 0x610)
#define HWIO_MSS_QDSP6SS_VU0_PWR_DEBUG_RMSK                                             0xf7
#define HWIO_MSS_QDSP6SS_VU0_PWR_DEBUG_IN                    \
                in_dword_masked(HWIO_MSS_QDSP6SS_VU0_PWR_DEBUG_ADDR, HWIO_MSS_QDSP6SS_VU0_PWR_DEBUG_RMSK)
#define HWIO_MSS_QDSP6SS_VU0_PWR_DEBUG_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_VU0_PWR_DEBUG_ADDR, m)
#define HWIO_MSS_QDSP6SS_VU0_PWR_DEBUG_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_VU0_PWR_DEBUG_ADDR,v)
#define HWIO_MSS_QDSP6SS_VU0_PWR_DEBUG_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_QDSP6SS_VU0_PWR_DEBUG_ADDR,m,v,HWIO_MSS_QDSP6SS_VU0_PWR_DEBUG_IN)
#define HWIO_MSS_QDSP6SS_VU0_PWR_DEBUG_SEQ_DELAY_BMSK                                   0xf0
#define HWIO_MSS_QDSP6SS_VU0_PWR_DEBUG_SEQ_DELAY_SHFT                                      4
#define HWIO_MSS_QDSP6SS_VU0_PWR_DEBUG_CLAMP_IO_BMSK                                     0x4
#define HWIO_MSS_QDSP6SS_VU0_PWR_DEBUG_CLAMP_IO_SHFT                                       2
#define HWIO_MSS_QDSP6SS_VU0_PWR_DEBUG_CP_ARES_BMSK                                      0x2
#define HWIO_MSS_QDSP6SS_VU0_PWR_DEBUG_CP_ARES_SHFT                                        1
#define HWIO_MSS_QDSP6SS_VU0_PWR_DEBUG_CP_CLK_EN_BMSK                                    0x1
#define HWIO_MSS_QDSP6SS_VU0_PWR_DEBUG_CP_CLK_EN_SHFT                                      0

#define HWIO_MSS_QDSP6SS_VU_QOS_ADDR                                              (MSS_QDSP6V671SS_PUB_REG_BASE            + 0x6a0)
#define HWIO_MSS_QDSP6SS_VU_QOS_OFFS                                              (MSS_QDSP6V671SS_PUB_REG_BASE_OFFS + 0x6a0)
#define HWIO_MSS_QDSP6SS_VU_QOS_RMSK                                                    0x70
#define HWIO_MSS_QDSP6SS_VU_QOS_IN                    \
                in_dword_masked(HWIO_MSS_QDSP6SS_VU_QOS_ADDR, HWIO_MSS_QDSP6SS_VU_QOS_RMSK)
#define HWIO_MSS_QDSP6SS_VU_QOS_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_VU_QOS_ADDR, m)
#define HWIO_MSS_QDSP6SS_VU_QOS_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_VU_QOS_ADDR,v)
#define HWIO_MSS_QDSP6SS_VU_QOS_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_QDSP6SS_VU_QOS_ADDR,m,v,HWIO_MSS_QDSP6SS_VU_QOS_IN)
#define HWIO_MSS_QDSP6SS_VU_QOS_QOS_AXIS_BMSK                                           0x70
#define HWIO_MSS_QDSP6SS_VU_QOS_QOS_AXIS_SHFT                                              4

#define HWIO_MSS_QDSP6SS_VPE_VU_CBCR_ADDR                                         (MSS_QDSP6V671SS_PUB_REG_BASE            + 0x6b0)
#define HWIO_MSS_QDSP6SS_VPE_VU_CBCR_OFFS                                         (MSS_QDSP6V671SS_PUB_REG_BASE_OFFS + 0x6b0)
#define HWIO_MSS_QDSP6SS_VPE_VU_CBCR_RMSK                                         0x80000001
#define HWIO_MSS_QDSP6SS_VPE_VU_CBCR_IN                    \
                in_dword_masked(HWIO_MSS_QDSP6SS_VPE_VU_CBCR_ADDR, HWIO_MSS_QDSP6SS_VPE_VU_CBCR_RMSK)
#define HWIO_MSS_QDSP6SS_VPE_VU_CBCR_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_VPE_VU_CBCR_ADDR, m)
#define HWIO_MSS_QDSP6SS_VPE_VU_CBCR_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_VPE_VU_CBCR_ADDR,v)
#define HWIO_MSS_QDSP6SS_VPE_VU_CBCR_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_QDSP6SS_VPE_VU_CBCR_ADDR,m,v,HWIO_MSS_QDSP6SS_VPE_VU_CBCR_IN)
#define HWIO_MSS_QDSP6SS_VPE_VU_CBCR_CLKOFF_BMSK                                  0x80000000
#define HWIO_MSS_QDSP6SS_VPE_VU_CBCR_CLKOFF_SHFT                                          31
#define HWIO_MSS_QDSP6SS_VPE_VU_CBCR_CLKEN_BMSK                                          0x1
#define HWIO_MSS_QDSP6SS_VPE_VU_CBCR_CLKEN_SHFT                                            0

#define HWIO_MSS_QDSP6SS_TEST_BUS_CTL_ADDR                                        (MSS_QDSP6V671SS_PUB_REG_BASE            + 0x2000)
#define HWIO_MSS_QDSP6SS_TEST_BUS_CTL_OFFS                                        (MSS_QDSP6V671SS_PUB_REG_BASE_OFFS + 0x2000)
#define HWIO_MSS_QDSP6SS_TEST_BUS_CTL_RMSK                                        0xffffffff
#define HWIO_MSS_QDSP6SS_TEST_BUS_CTL_IN                    \
                in_dword_masked(HWIO_MSS_QDSP6SS_TEST_BUS_CTL_ADDR, HWIO_MSS_QDSP6SS_TEST_BUS_CTL_RMSK)
#define HWIO_MSS_QDSP6SS_TEST_BUS_CTL_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_TEST_BUS_CTL_ADDR, m)
#define HWIO_MSS_QDSP6SS_TEST_BUS_CTL_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_TEST_BUS_CTL_ADDR,v)
#define HWIO_MSS_QDSP6SS_TEST_BUS_CTL_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_QDSP6SS_TEST_BUS_CTL_ADDR,m,v,HWIO_MSS_QDSP6SS_TEST_BUS_CTL_IN)
#define HWIO_MSS_QDSP6SS_TEST_BUS_CTL_SPARE_CFG_BMSK                              0xff000000
#define HWIO_MSS_QDSP6SS_TEST_BUS_CTL_SPARE_CFG_SHFT                                      24
#define HWIO_MSS_QDSP6SS_TEST_BUS_CTL_HWE_INV_BMSK                                  0x800000
#define HWIO_MSS_QDSP6SS_TEST_BUS_CTL_HWE_INV_SHFT                                        23
#define HWIO_MSS_QDSP6SS_TEST_BUS_CTL_HWE_SEL_BMSK                                  0x7c0000
#define HWIO_MSS_QDSP6SS_TEST_BUS_CTL_HWE_SEL_SHFT                                        18
#define HWIO_MSS_QDSP6SS_TEST_BUS_CTL_TESTBUS_EN_BMSK                                0x20000
#define HWIO_MSS_QDSP6SS_TEST_BUS_CTL_TESTBUS_EN_SHFT                                     17
#define HWIO_MSS_QDSP6SS_TEST_BUS_CTL_Q6_SEL_BMSK                                    0x10000
#define HWIO_MSS_QDSP6SS_TEST_BUS_CTL_Q6_SEL_SHFT                                         16
#define HWIO_MSS_QDSP6SS_TEST_BUS_CTL_SEL_BMSK                                        0xffff
#define HWIO_MSS_QDSP6SS_TEST_BUS_CTL_SEL_SHFT                                             0

#define HWIO_MSS_QDSP6SS_TEST_BUS_VALUE_ADDR                                      (MSS_QDSP6V671SS_PUB_REG_BASE            + 0x2004)
#define HWIO_MSS_QDSP6SS_TEST_BUS_VALUE_OFFS                                      (MSS_QDSP6V671SS_PUB_REG_BASE_OFFS + 0x2004)
#define HWIO_MSS_QDSP6SS_TEST_BUS_VALUE_RMSK                                      0xffffffff
#define HWIO_MSS_QDSP6SS_TEST_BUS_VALUE_IN                    \
                in_dword_masked(HWIO_MSS_QDSP6SS_TEST_BUS_VALUE_ADDR, HWIO_MSS_QDSP6SS_TEST_BUS_VALUE_RMSK)
#define HWIO_MSS_QDSP6SS_TEST_BUS_VALUE_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_TEST_BUS_VALUE_ADDR, m)
#define HWIO_MSS_QDSP6SS_TEST_BUS_VALUE_VALUE_BMSK                                0xffffffff
#define HWIO_MSS_QDSP6SS_TEST_BUS_VALUE_VALUE_SHFT                                         0

#define HWIO_MSS_QDSP6SS_TEST_BUS_WDATA_ADDR                                      (MSS_QDSP6V671SS_PUB_REG_BASE            + 0x2008)
#define HWIO_MSS_QDSP6SS_TEST_BUS_WDATA_OFFS                                      (MSS_QDSP6V671SS_PUB_REG_BASE_OFFS + 0x2008)
#define HWIO_MSS_QDSP6SS_TEST_BUS_WDATA_RMSK                                      0xffffffff
#define HWIO_MSS_QDSP6SS_TEST_BUS_WDATA_IN                    \
                in_dword_masked(HWIO_MSS_QDSP6SS_TEST_BUS_WDATA_ADDR, HWIO_MSS_QDSP6SS_TEST_BUS_WDATA_RMSK)
#define HWIO_MSS_QDSP6SS_TEST_BUS_WDATA_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_TEST_BUS_WDATA_ADDR, m)
#define HWIO_MSS_QDSP6SS_TEST_BUS_WDATA_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_TEST_BUS_WDATA_ADDR,v)
#define HWIO_MSS_QDSP6SS_TEST_BUS_WDATA_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_QDSP6SS_TEST_BUS_WDATA_ADDR,m,v,HWIO_MSS_QDSP6SS_TEST_BUS_WDATA_IN)
#define HWIO_MSS_QDSP6SS_TEST_BUS_WDATA_VALUE_BMSK                                0xffffffff
#define HWIO_MSS_QDSP6SS_TEST_BUS_WDATA_VALUE_SHFT                                         0

#define HWIO_MSS_QDSP6SS_PWRDBG_CTL_ADDR                                          (MSS_QDSP6V671SS_PUB_REG_BASE            + 0x200c)
#define HWIO_MSS_QDSP6SS_PWRDBG_CTL_OFFS                                          (MSS_QDSP6V671SS_PUB_REG_BASE_OFFS + 0x200c)
#define HWIO_MSS_QDSP6SS_PWRDBG_CTL_RMSK                                                 0x1
#define HWIO_MSS_QDSP6SS_PWRDBG_CTL_IN                    \
                in_dword_masked(HWIO_MSS_QDSP6SS_PWRDBG_CTL_ADDR, HWIO_MSS_QDSP6SS_PWRDBG_CTL_RMSK)
#define HWIO_MSS_QDSP6SS_PWRDBG_CTL_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_PWRDBG_CTL_ADDR, m)
#define HWIO_MSS_QDSP6SS_PWRDBG_CTL_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_PWRDBG_CTL_ADDR,v)
#define HWIO_MSS_QDSP6SS_PWRDBG_CTL_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_QDSP6SS_PWRDBG_CTL_ADDR,m,v,HWIO_MSS_QDSP6SS_PWRDBG_CTL_IN)
#define HWIO_MSS_QDSP6SS_PWRDBG_CTL_ENA_BMSK                                             0x1
#define HWIO_MSS_QDSP6SS_PWRDBG_CTL_ENA_SHFT                                               0

#define HWIO_MSS_QDSP6SS_TEST_CLK_CTL_ADDR                                        (MSS_QDSP6V671SS_PUB_REG_BASE            + 0x2010)
#define HWIO_MSS_QDSP6SS_TEST_CLK_CTL_OFFS                                        (MSS_QDSP6V671SS_PUB_REG_BASE_OFFS + 0x2010)
#define HWIO_MSS_QDSP6SS_TEST_CLK_CTL_RMSK                                              0x7f
#define HWIO_MSS_QDSP6SS_TEST_CLK_CTL_IN                    \
                in_dword_masked(HWIO_MSS_QDSP6SS_TEST_CLK_CTL_ADDR, HWIO_MSS_QDSP6SS_TEST_CLK_CTL_RMSK)
#define HWIO_MSS_QDSP6SS_TEST_CLK_CTL_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_TEST_CLK_CTL_ADDR, m)
#define HWIO_MSS_QDSP6SS_TEST_CLK_CTL_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_TEST_CLK_CTL_ADDR,v)
#define HWIO_MSS_QDSP6SS_TEST_CLK_CTL_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_QDSP6SS_TEST_CLK_CTL_ADDR,m,v,HWIO_MSS_QDSP6SS_TEST_CLK_CTL_IN)
#define HWIO_MSS_QDSP6SS_TEST_CLK_CTL_Q6_DBG_CLK_EN_BMSK                                0x40
#define HWIO_MSS_QDSP6SS_TEST_CLK_CTL_Q6_DBG_CLK_EN_SHFT                                   6
#define HWIO_MSS_QDSP6SS_TEST_CLK_CTL_Q6_DBG_CLK_INV_BMSK                               0x20
#define HWIO_MSS_QDSP6SS_TEST_CLK_CTL_Q6_DBG_CLK_INV_SHFT                                  5
#define HWIO_MSS_QDSP6SS_TEST_CLK_CTL_DBG_CLK_SEL_BMSK                                  0x1f
#define HWIO_MSS_QDSP6SS_TEST_CLK_CTL_DBG_CLK_SEL_SHFT                                     0

#define HWIO_MSS_QDSP6SS_BHS_TEST_ADDR                                            (MSS_QDSP6V671SS_PUB_REG_BASE            + 0x2014)
#define HWIO_MSS_QDSP6SS_BHS_TEST_OFFS                                            (MSS_QDSP6V671SS_PUB_REG_BASE_OFFS + 0x2014)
#define HWIO_MSS_QDSP6SS_BHS_TEST_RMSK                                                   0x1
#define HWIO_MSS_QDSP6SS_BHS_TEST_IN                    \
                in_dword_masked(HWIO_MSS_QDSP6SS_BHS_TEST_ADDR, HWIO_MSS_QDSP6SS_BHS_TEST_RMSK)
#define HWIO_MSS_QDSP6SS_BHS_TEST_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_BHS_TEST_ADDR, m)
#define HWIO_MSS_QDSP6SS_BHS_TEST_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_BHS_TEST_ADDR,v)
#define HWIO_MSS_QDSP6SS_BHS_TEST_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_QDSP6SS_BHS_TEST_ADDR,m,v,HWIO_MSS_QDSP6SS_BHS_TEST_IN)
#define HWIO_MSS_QDSP6SS_BHS_TEST_INSTANT_ON_BMSK                                        0x1
#define HWIO_MSS_QDSP6SS_BHS_TEST_INSTANT_ON_SHFT                                          0

#define HWIO_MSS_QDSP6SS_ZMEAS_CONFIG_ADDR                                        (MSS_QDSP6V671SS_PUB_REG_BASE            + 0x2018)
#define HWIO_MSS_QDSP6SS_ZMEAS_CONFIG_OFFS                                        (MSS_QDSP6V671SS_PUB_REG_BASE_OFFS + 0x2018)
#define HWIO_MSS_QDSP6SS_ZMEAS_CONFIG_RMSK                                          0xffffff
#define HWIO_MSS_QDSP6SS_ZMEAS_CONFIG_IN                    \
                in_dword_masked(HWIO_MSS_QDSP6SS_ZMEAS_CONFIG_ADDR, HWIO_MSS_QDSP6SS_ZMEAS_CONFIG_RMSK)
#define HWIO_MSS_QDSP6SS_ZMEAS_CONFIG_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_ZMEAS_CONFIG_ADDR, m)
#define HWIO_MSS_QDSP6SS_ZMEAS_CONFIG_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_ZMEAS_CONFIG_ADDR,v)
#define HWIO_MSS_QDSP6SS_ZMEAS_CONFIG_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_QDSP6SS_ZMEAS_CONFIG_ADDR,m,v,HWIO_MSS_QDSP6SS_ZMEAS_CONFIG_IN)
#define HWIO_MSS_QDSP6SS_ZMEAS_CONFIG_ZMEAS_COUNT_BMSK                              0xffffe0
#define HWIO_MSS_QDSP6SS_ZMEAS_CONFIG_ZMEAS_COUNT_SHFT                                     5
#define HWIO_MSS_QDSP6SS_ZMEAS_CONFIG_MUX_SEL_BMSK                                      0x10
#define HWIO_MSS_QDSP6SS_ZMEAS_CONFIG_MUX_SEL_SHFT                                         4
#define HWIO_MSS_QDSP6SS_ZMEAS_CONFIG_ZMEAS_CLK_R_ENA_BMSK                               0x8
#define HWIO_MSS_QDSP6SS_ZMEAS_CONFIG_ZMEAS_CLK_R_ENA_SHFT                                 3
#define HWIO_MSS_QDSP6SS_ZMEAS_CONFIG_ZMEAS_CGC_EN_BMSK                                  0x4
#define HWIO_MSS_QDSP6SS_ZMEAS_CONFIG_ZMEAS_CGC_EN_SHFT                                    2
#define HWIO_MSS_QDSP6SS_ZMEAS_CONFIG_ZMEAS_BYPASS_BMSK                                  0x2
#define HWIO_MSS_QDSP6SS_ZMEAS_CONFIG_ZMEAS_BYPASS_SHFT                                    1
#define HWIO_MSS_QDSP6SS_ZMEAS_CONFIG_ZMEAS_SW_RESET_BMSK                                0x1
#define HWIO_MSS_QDSP6SS_ZMEAS_CONFIG_ZMEAS_SW_RESET_SHFT                                  0

#define HWIO_MSS_QDSP6SS_CORE_STATUS_ADDR                                         (MSS_QDSP6V671SS_PUB_REG_BASE            + 0x2028)
#define HWIO_MSS_QDSP6SS_CORE_STATUS_OFFS                                         (MSS_QDSP6V671SS_PUB_REG_BASE_OFFS + 0x2028)
#define HWIO_MSS_QDSP6SS_CORE_STATUS_RMSK                                               0x3f
#define HWIO_MSS_QDSP6SS_CORE_STATUS_IN                    \
                in_dword_masked(HWIO_MSS_QDSP6SS_CORE_STATUS_ADDR, HWIO_MSS_QDSP6SS_CORE_STATUS_RMSK)
#define HWIO_MSS_QDSP6SS_CORE_STATUS_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_CORE_STATUS_ADDR, m)
#define HWIO_MSS_QDSP6SS_CORE_STATUS_VU1_VX_IDLE_BMSK                                   0x20
#define HWIO_MSS_QDSP6SS_CORE_STATUS_VU1_VX_IDLE_SHFT                                      5
#define HWIO_MSS_QDSP6SS_CORE_STATUS_VU0_VX_IDLE_BMSK                                   0x10
#define HWIO_MSS_QDSP6SS_CORE_STATUS_VU0_VX_IDLE_SHFT                                      4
#define HWIO_MSS_QDSP6SS_CORE_STATUS_VU_OVERALL_IDLE_BMSK                                0x8
#define HWIO_MSS_QDSP6SS_CORE_STATUS_VU_OVERALL_IDLE_SHFT                                  3
#define HWIO_MSS_QDSP6SS_CORE_STATUS_CP0_IDLE_BMSK                                       0x4
#define HWIO_MSS_QDSP6SS_CORE_STATUS_CP0_IDLE_SHFT                                         2
#define HWIO_MSS_QDSP6SS_CORE_STATUS_CORE_IN_SLEEP_BMSK                                  0x2
#define HWIO_MSS_QDSP6SS_CORE_STATUS_CORE_IN_SLEEP_SHFT                                    1
#define HWIO_MSS_QDSP6SS_CORE_STATUS_CORE_STATE_BMSK                                     0x1
#define HWIO_MSS_QDSP6SS_CORE_STATUS_CORE_STATE_SHFT                                       0

#define HWIO_MSS_QDSP6SS_MEM_DEBUG_ADDR                                           (MSS_QDSP6V671SS_PUB_REG_BASE            + 0x2030)
#define HWIO_MSS_QDSP6SS_MEM_DEBUG_OFFS                                           (MSS_QDSP6V671SS_PUB_REG_BASE_OFFS + 0x2030)
#define HWIO_MSS_QDSP6SS_MEM_DEBUG_RMSK                                               0xffff
#define HWIO_MSS_QDSP6SS_MEM_DEBUG_IN                    \
                in_dword_masked(HWIO_MSS_QDSP6SS_MEM_DEBUG_ADDR, HWIO_MSS_QDSP6SS_MEM_DEBUG_RMSK)
#define HWIO_MSS_QDSP6SS_MEM_DEBUG_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_MEM_DEBUG_ADDR, m)
#define HWIO_MSS_QDSP6SS_MEM_DEBUG_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_MEM_DEBUG_ADDR,v)
#define HWIO_MSS_QDSP6SS_MEM_DEBUG_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_QDSP6SS_MEM_DEBUG_ADDR,m,v,HWIO_MSS_QDSP6SS_MEM_DEBUG_IN)
#define HWIO_MSS_QDSP6SS_MEM_DEBUG_OFF_BMSK                                           0xffff
#define HWIO_MSS_QDSP6SS_MEM_DEBUG_OFF_SHFT                                                0

#define HWIO_MSS_QDSP6SS_L2MEM_DEBUG0_ADDR                                        (MSS_QDSP6V671SS_PUB_REG_BASE            + 0x2034)
#define HWIO_MSS_QDSP6SS_L2MEM_DEBUG0_OFFS                                        (MSS_QDSP6V671SS_PUB_REG_BASE_OFFS + 0x2034)
#define HWIO_MSS_QDSP6SS_L2MEM_DEBUG0_RMSK                                        0xffffffff
#define HWIO_MSS_QDSP6SS_L2MEM_DEBUG0_IN                    \
                in_dword_masked(HWIO_MSS_QDSP6SS_L2MEM_DEBUG0_ADDR, HWIO_MSS_QDSP6SS_L2MEM_DEBUG0_RMSK)
#define HWIO_MSS_QDSP6SS_L2MEM_DEBUG0_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_L2MEM_DEBUG0_ADDR, m)
#define HWIO_MSS_QDSP6SS_L2MEM_DEBUG0_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_L2MEM_DEBUG0_ADDR,v)
#define HWIO_MSS_QDSP6SS_L2MEM_DEBUG0_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_QDSP6SS_L2MEM_DEBUG0_ADDR,m,v,HWIO_MSS_QDSP6SS_L2MEM_DEBUG0_IN)
#define HWIO_MSS_QDSP6SS_L2MEM_DEBUG0_OFF_BMSK                                    0xffffffff
#define HWIO_MSS_QDSP6SS_L2MEM_DEBUG0_OFF_SHFT                                             0

#define HWIO_MSS_QDSP6SS_L2MEM_DEBUG1_ADDR                                        (MSS_QDSP6V671SS_PUB_REG_BASE            + 0x2038)
#define HWIO_MSS_QDSP6SS_L2MEM_DEBUG1_OFFS                                        (MSS_QDSP6V671SS_PUB_REG_BASE_OFFS + 0x2038)
#define HWIO_MSS_QDSP6SS_L2MEM_DEBUG1_RMSK                                        0xffffffff
#define HWIO_MSS_QDSP6SS_L2MEM_DEBUG1_IN                    \
                in_dword_masked(HWIO_MSS_QDSP6SS_L2MEM_DEBUG1_ADDR, HWIO_MSS_QDSP6SS_L2MEM_DEBUG1_RMSK)
#define HWIO_MSS_QDSP6SS_L2MEM_DEBUG1_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_L2MEM_DEBUG1_ADDR, m)
#define HWIO_MSS_QDSP6SS_L2MEM_DEBUG1_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_L2MEM_DEBUG1_ADDR,v)
#define HWIO_MSS_QDSP6SS_L2MEM_DEBUG1_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_QDSP6SS_L2MEM_DEBUG1_ADDR,m,v,HWIO_MSS_QDSP6SS_L2MEM_DEBUG1_IN)
#define HWIO_MSS_QDSP6SS_L2MEM_DEBUG1_OFF_BMSK                                    0xffffffff
#define HWIO_MSS_QDSP6SS_L2MEM_DEBUG1_OFF_SHFT                                             0

#define HWIO_MSS_QDSP6SS_CPMEM_DEBUG_ADDR                                         (MSS_QDSP6V671SS_PUB_REG_BASE            + 0x203c)
#define HWIO_MSS_QDSP6SS_CPMEM_DEBUG_OFFS                                         (MSS_QDSP6V671SS_PUB_REG_BASE_OFFS + 0x203c)
#define HWIO_MSS_QDSP6SS_CPMEM_DEBUG_RMSK                                         0xffffffff
#define HWIO_MSS_QDSP6SS_CPMEM_DEBUG_IN                    \
                in_dword_masked(HWIO_MSS_QDSP6SS_CPMEM_DEBUG_ADDR, HWIO_MSS_QDSP6SS_CPMEM_DEBUG_RMSK)
#define HWIO_MSS_QDSP6SS_CPMEM_DEBUG_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_CPMEM_DEBUG_ADDR, m)
#define HWIO_MSS_QDSP6SS_CPMEM_DEBUG_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_CPMEM_DEBUG_ADDR,v)
#define HWIO_MSS_QDSP6SS_CPMEM_DEBUG_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_QDSP6SS_CPMEM_DEBUG_ADDR,m,v,HWIO_MSS_QDSP6SS_CPMEM_DEBUG_IN)
#define HWIO_MSS_QDSP6SS_CPMEM_DEBUG_OFF_BMSK                                     0xffffffff
#define HWIO_MSS_QDSP6SS_CPMEM_DEBUG_OFF_SHFT                                              0

#define HWIO_MSS_QDSP6SS_L1SMEM_DEBUG0_ADDR                                       (MSS_QDSP6V671SS_PUB_REG_BASE            + 0x2040)
#define HWIO_MSS_QDSP6SS_L1SMEM_DEBUG0_OFFS                                       (MSS_QDSP6V671SS_PUB_REG_BASE_OFFS + 0x2040)
#define HWIO_MSS_QDSP6SS_L1SMEM_DEBUG0_RMSK                                       0xffffffff
#define HWIO_MSS_QDSP6SS_L1SMEM_DEBUG0_IN                    \
                in_dword_masked(HWIO_MSS_QDSP6SS_L1SMEM_DEBUG0_ADDR, HWIO_MSS_QDSP6SS_L1SMEM_DEBUG0_RMSK)
#define HWIO_MSS_QDSP6SS_L1SMEM_DEBUG0_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_L1SMEM_DEBUG0_ADDR, m)
#define HWIO_MSS_QDSP6SS_L1SMEM_DEBUG0_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_L1SMEM_DEBUG0_ADDR,v)
#define HWIO_MSS_QDSP6SS_L1SMEM_DEBUG0_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_QDSP6SS_L1SMEM_DEBUG0_ADDR,m,v,HWIO_MSS_QDSP6SS_L1SMEM_DEBUG0_IN)
#define HWIO_MSS_QDSP6SS_L1SMEM_DEBUG0_OFF_BMSK                                   0xffffffff
#define HWIO_MSS_QDSP6SS_L1SMEM_DEBUG0_OFF_SHFT                                            0

#define HWIO_MSS_QDSP6SS_L1SMEM_DEBUG1_ADDR                                       (MSS_QDSP6V671SS_PUB_REG_BASE            + 0x2044)
#define HWIO_MSS_QDSP6SS_L1SMEM_DEBUG1_OFFS                                       (MSS_QDSP6V671SS_PUB_REG_BASE_OFFS + 0x2044)
#define HWIO_MSS_QDSP6SS_L1SMEM_DEBUG1_RMSK                                       0xffffffff
#define HWIO_MSS_QDSP6SS_L1SMEM_DEBUG1_IN                    \
                in_dword_masked(HWIO_MSS_QDSP6SS_L1SMEM_DEBUG1_ADDR, HWIO_MSS_QDSP6SS_L1SMEM_DEBUG1_RMSK)
#define HWIO_MSS_QDSP6SS_L1SMEM_DEBUG1_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_L1SMEM_DEBUG1_ADDR, m)
#define HWIO_MSS_QDSP6SS_L1SMEM_DEBUG1_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_L1SMEM_DEBUG1_ADDR,v)
#define HWIO_MSS_QDSP6SS_L1SMEM_DEBUG1_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_QDSP6SS_L1SMEM_DEBUG1_ADDR,m,v,HWIO_MSS_QDSP6SS_L1SMEM_DEBUG1_IN)
#define HWIO_MSS_QDSP6SS_L1SMEM_DEBUG1_OFF_BMSK                                   0xffffffff
#define HWIO_MSS_QDSP6SS_L1SMEM_DEBUG1_OFF_SHFT                                            0

#define HWIO_MSS_QDSP6SS_L1SMEM_DEBUG2_ADDR                                       (MSS_QDSP6V671SS_PUB_REG_BASE            + 0x2048)
#define HWIO_MSS_QDSP6SS_L1SMEM_DEBUG2_OFFS                                       (MSS_QDSP6V671SS_PUB_REG_BASE_OFFS + 0x2048)
#define HWIO_MSS_QDSP6SS_L1SMEM_DEBUG2_RMSK                                       0xffffffff
#define HWIO_MSS_QDSP6SS_L1SMEM_DEBUG2_IN                    \
                in_dword_masked(HWIO_MSS_QDSP6SS_L1SMEM_DEBUG2_ADDR, HWIO_MSS_QDSP6SS_L1SMEM_DEBUG2_RMSK)
#define HWIO_MSS_QDSP6SS_L1SMEM_DEBUG2_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_L1SMEM_DEBUG2_ADDR, m)
#define HWIO_MSS_QDSP6SS_L1SMEM_DEBUG2_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_L1SMEM_DEBUG2_ADDR,v)
#define HWIO_MSS_QDSP6SS_L1SMEM_DEBUG2_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_QDSP6SS_L1SMEM_DEBUG2_ADDR,m,v,HWIO_MSS_QDSP6SS_L1SMEM_DEBUG2_IN)
#define HWIO_MSS_QDSP6SS_L1SMEM_DEBUG2_OFF_BMSK                                   0xffffffff
#define HWIO_MSS_QDSP6SS_L1SMEM_DEBUG2_OFF_SHFT                                            0

#define HWIO_MSS_QDSP6SS_L1SMEM_DEBUG3_ADDR                                       (MSS_QDSP6V671SS_PUB_REG_BASE            + 0x204c)
#define HWIO_MSS_QDSP6SS_L1SMEM_DEBUG3_OFFS                                       (MSS_QDSP6V671SS_PUB_REG_BASE_OFFS + 0x204c)
#define HWIO_MSS_QDSP6SS_L1SMEM_DEBUG3_RMSK                                       0xffffffff
#define HWIO_MSS_QDSP6SS_L1SMEM_DEBUG3_IN                    \
                in_dword_masked(HWIO_MSS_QDSP6SS_L1SMEM_DEBUG3_ADDR, HWIO_MSS_QDSP6SS_L1SMEM_DEBUG3_RMSK)
#define HWIO_MSS_QDSP6SS_L1SMEM_DEBUG3_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_L1SMEM_DEBUG3_ADDR, m)
#define HWIO_MSS_QDSP6SS_L1SMEM_DEBUG3_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_L1SMEM_DEBUG3_ADDR,v)
#define HWIO_MSS_QDSP6SS_L1SMEM_DEBUG3_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_QDSP6SS_L1SMEM_DEBUG3_ADDR,m,v,HWIO_MSS_QDSP6SS_L1SMEM_DEBUG3_IN)
#define HWIO_MSS_QDSP6SS_L1SMEM_DEBUG3_OFF_BMSK                                   0xffffffff
#define HWIO_MSS_QDSP6SS_L1SMEM_DEBUG3_OFF_SHFT                                            0

/*----------------------------------------------------------------------------
 * MODULE: MSS_QDSP6SS_QDSP6V671SS_PLL
 *--------------------------------------------------------------------------*/

#define MSS_QDSP6SS_QDSP6V671SS_PLL_REG_BASE                                        (MSS_TOP_BASE            + 0x00090000)
#define MSS_QDSP6SS_QDSP6V671SS_PLL_REG_BASE_SIZE                                   0x10000
#define MSS_QDSP6SS_QDSP6V671SS_PLL_REG_BASE_USED                                   0x50
#define MSS_QDSP6SS_QDSP6V671SS_PLL_REG_BASE_OFFS                                   0x00090000

#define HWIO_MSS_QDSP6SS_PLL_MODE_ADDR                                              (MSS_QDSP6SS_QDSP6V671SS_PLL_REG_BASE            + 0x0)
#define HWIO_MSS_QDSP6SS_PLL_MODE_OFFS                                              (MSS_QDSP6SS_QDSP6V671SS_PLL_REG_BASE_OFFS + 0x0)
#define HWIO_MSS_QDSP6SS_PLL_MODE_RMSK                                              0xffffffff
#define HWIO_MSS_QDSP6SS_PLL_MODE_IN                    \
                in_dword_masked(HWIO_MSS_QDSP6SS_PLL_MODE_ADDR, HWIO_MSS_QDSP6SS_PLL_MODE_RMSK)
#define HWIO_MSS_QDSP6SS_PLL_MODE_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_PLL_MODE_ADDR, m)
#define HWIO_MSS_QDSP6SS_PLL_MODE_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_PLL_MODE_ADDR,v)
#define HWIO_MSS_QDSP6SS_PLL_MODE_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_QDSP6SS_PLL_MODE_ADDR,m,v,HWIO_MSS_QDSP6SS_PLL_MODE_IN)
#define HWIO_MSS_QDSP6SS_PLL_MODE_PLL_LOCK_DET_BMSK                                 0x80000000
#define HWIO_MSS_QDSP6SS_PLL_MODE_PLL_LOCK_DET_SHFT                                         31
#define HWIO_MSS_QDSP6SS_PLL_MODE_PLL_ACTIVE_FLAG_BMSK                              0x40000000
#define HWIO_MSS_QDSP6SS_PLL_MODE_PLL_ACTIVE_FLAG_SHFT                                      30
#define HWIO_MSS_QDSP6SS_PLL_MODE_PLL_ACK_LATCH_BMSK                                0x20000000
#define HWIO_MSS_QDSP6SS_PLL_MODE_PLL_ACK_LATCH_SHFT                                        29
#define HWIO_MSS_QDSP6SS_PLL_MODE_PLL_LOCK_DET_FINE_BMSK                            0x10000000
#define HWIO_MSS_QDSP6SS_PLL_MODE_PLL_LOCK_DET_FINE_SHFT                                    28
#define HWIO_MSS_QDSP6SS_PLL_MODE_PCAL_DONE_BMSK                                     0x8000000
#define HWIO_MSS_QDSP6SS_PLL_MODE_PCAL_DONE_SHFT                                            27
#define HWIO_MSS_QDSP6SS_PLL_MODE_RESERVE_BITS26_25_BMSK                             0x6000000
#define HWIO_MSS_QDSP6SS_PLL_MODE_RESERVE_BITS26_25_SHFT                                    25
#define HWIO_MSS_QDSP6SS_PLL_MODE_FSM_LEGACY_MODE_BMSK                               0x1000000
#define HWIO_MSS_QDSP6SS_PLL_MODE_FSM_LEGACY_MODE_SHFT                                      24
#define HWIO_MSS_QDSP6SS_PLL_MODE_PLL_HW_UPDATE_LOGIC_BYPASS_BMSK                     0x800000
#define HWIO_MSS_QDSP6SS_PLL_MODE_PLL_HW_UPDATE_LOGIC_BYPASS_SHFT                           23
#define HWIO_MSS_QDSP6SS_PLL_MODE_PLL_UPDATE_BMSK                                     0x400000
#define HWIO_MSS_QDSP6SS_PLL_MODE_PLL_UPDATE_SHFT                                           22
#define HWIO_MSS_QDSP6SS_PLL_MODE_PLL_VOTE_FSM_RESET_BMSK                             0x200000
#define HWIO_MSS_QDSP6SS_PLL_MODE_PLL_VOTE_FSM_RESET_SHFT                                   21
#define HWIO_MSS_QDSP6SS_PLL_MODE_PLL_VOTE_FSM_ENA_BMSK                               0x100000
#define HWIO_MSS_QDSP6SS_PLL_MODE_PLL_VOTE_FSM_ENA_SHFT                                     20
#define HWIO_MSS_QDSP6SS_PLL_MODE_PLL_BIAS_COUNT_BMSK                                  0xfc000
#define HWIO_MSS_QDSP6SS_PLL_MODE_PLL_BIAS_COUNT_SHFT                                       14
#define HWIO_MSS_QDSP6SS_PLL_MODE_PLL_LOCK_COUNT_BMSK                                   0x3f00
#define HWIO_MSS_QDSP6SS_PLL_MODE_PLL_LOCK_COUNT_SHFT                                        8
#define HWIO_MSS_QDSP6SS_PLL_MODE_RESERVE_BIT7_BMSK                                       0x80
#define HWIO_MSS_QDSP6SS_PLL_MODE_RESERVE_BIT7_SHFT                                          7
#define HWIO_MSS_QDSP6SS_PLL_MODE_OUT_TEST_HW_CTL_BMSK                                    0x40
#define HWIO_MSS_QDSP6SS_PLL_MODE_OUT_TEST_HW_CTL_SHFT                                       6
#define HWIO_MSS_QDSP6SS_PLL_MODE_OUT_ODD_HW_CTL_BMSK                                     0x20
#define HWIO_MSS_QDSP6SS_PLL_MODE_OUT_ODD_HW_CTL_SHFT                                        5
#define HWIO_MSS_QDSP6SS_PLL_MODE_OUT_EVEN_HW_CTL_BMSK                                    0x10
#define HWIO_MSS_QDSP6SS_PLL_MODE_OUT_EVEN_HW_CTL_SHFT                                       4
#define HWIO_MSS_QDSP6SS_PLL_MODE_OUT_MAIN_HW_CTL_BMSK                                     0x8
#define HWIO_MSS_QDSP6SS_PLL_MODE_OUT_MAIN_HW_CTL_SHFT                                       3
#define HWIO_MSS_QDSP6SS_PLL_MODE_PLL_RESET_N_BMSK                                         0x4
#define HWIO_MSS_QDSP6SS_PLL_MODE_PLL_RESET_N_SHFT                                           2
#define HWIO_MSS_QDSP6SS_PLL_MODE_RESERVE_BIT1_BMSK                                        0x2
#define HWIO_MSS_QDSP6SS_PLL_MODE_RESERVE_BIT1_SHFT                                          1
#define HWIO_MSS_QDSP6SS_PLL_MODE_PLL_OUTCTRL_BMSK                                         0x1
#define HWIO_MSS_QDSP6SS_PLL_MODE_PLL_OUTCTRL_SHFT                                           0

#define HWIO_MSS_QDSP6SS_PLL_L_VAL_ADDR                                             (MSS_QDSP6SS_QDSP6V671SS_PLL_REG_BASE            + 0x4)
#define HWIO_MSS_QDSP6SS_PLL_L_VAL_OFFS                                             (MSS_QDSP6SS_QDSP6V671SS_PLL_REG_BASE_OFFS + 0x4)
#define HWIO_MSS_QDSP6SS_PLL_L_VAL_RMSK                                                 0xffff
#define HWIO_MSS_QDSP6SS_PLL_L_VAL_IN                    \
                in_dword_masked(HWIO_MSS_QDSP6SS_PLL_L_VAL_ADDR, HWIO_MSS_QDSP6SS_PLL_L_VAL_RMSK)
#define HWIO_MSS_QDSP6SS_PLL_L_VAL_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_PLL_L_VAL_ADDR, m)
#define HWIO_MSS_QDSP6SS_PLL_L_VAL_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_PLL_L_VAL_ADDR,v)
#define HWIO_MSS_QDSP6SS_PLL_L_VAL_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_QDSP6SS_PLL_L_VAL_ADDR,m,v,HWIO_MSS_QDSP6SS_PLL_L_VAL_IN)
#define HWIO_MSS_QDSP6SS_PLL_L_VAL_PLL_L_BMSK                                           0xffff
#define HWIO_MSS_QDSP6SS_PLL_L_VAL_PLL_L_SHFT                                                0

#define HWIO_MSS_QDSP6SS_PLL_CAL_L_VAL_ADDR                                         (MSS_QDSP6SS_QDSP6V671SS_PLL_REG_BASE            + 0x8)
#define HWIO_MSS_QDSP6SS_PLL_CAL_L_VAL_OFFS                                         (MSS_QDSP6SS_QDSP6V671SS_PLL_REG_BASE_OFFS + 0x8)
#define HWIO_MSS_QDSP6SS_PLL_CAL_L_VAL_RMSK                                             0xffff
#define HWIO_MSS_QDSP6SS_PLL_CAL_L_VAL_IN                    \
                in_dword_masked(HWIO_MSS_QDSP6SS_PLL_CAL_L_VAL_ADDR, HWIO_MSS_QDSP6SS_PLL_CAL_L_VAL_RMSK)
#define HWIO_MSS_QDSP6SS_PLL_CAL_L_VAL_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_PLL_CAL_L_VAL_ADDR, m)
#define HWIO_MSS_QDSP6SS_PLL_CAL_L_VAL_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_PLL_CAL_L_VAL_ADDR,v)
#define HWIO_MSS_QDSP6SS_PLL_CAL_L_VAL_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_QDSP6SS_PLL_CAL_L_VAL_ADDR,m,v,HWIO_MSS_QDSP6SS_PLL_CAL_L_VAL_IN)
#define HWIO_MSS_QDSP6SS_PLL_CAL_L_VAL_PLL_CAL_L_BMSK                                   0xffff
#define HWIO_MSS_QDSP6SS_PLL_CAL_L_VAL_PLL_CAL_L_SHFT                                        0

#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_ADDR                                          (MSS_QDSP6SS_QDSP6V671SS_PLL_REG_BASE            + 0xc)
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_OFFS                                          (MSS_QDSP6SS_QDSP6V671SS_PLL_REG_BASE_OFFS + 0xc)
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_RMSK                                          0xffffffff
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_IN                    \
                in_dword_masked(HWIO_MSS_QDSP6SS_PLL_USER_CTL_ADDR, HWIO_MSS_QDSP6SS_PLL_USER_CTL_RMSK)
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_PLL_USER_CTL_ADDR, m)
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_PLL_USER_CTL_ADDR,v)
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_QDSP6SS_PLL_USER_CTL_ADDR,m,v,HWIO_MSS_QDSP6SS_PLL_USER_CTL_IN)
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_RESERVE_BIT31_BMSK                            0x80000000
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_RESERVE_BIT31_SHFT                                    31
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_RESERVE_BIT30_BMSK                            0x40000000
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_RESERVE_BIT30_SHFT                                    30
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_RESERVE_BIT29_BMSK                            0x20000000
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_RESERVE_BIT29_SHFT                                    29
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_RESERVE_BIT28_BMSK                            0x10000000
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_RESERVE_BIT28_SHFT                                    28
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_RESERVE_BIT27_BMSK                             0x8000000
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_RESERVE_BIT27_SHFT                                    27
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_RESERVE_BIT26_BMSK                             0x4000000
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_RESERVE_BIT26_SHFT                                    26
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_RESERVE_BIT25_BMSK                             0x2000000
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_RESERVE_BIT25_SHFT                                    25
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_RESERVE_BIT24_BMSK                             0x1000000
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_RESERVE_BIT24_SHFT                                    24
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_RESERVE_BIT23_BMSK                              0x800000
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_RESERVE_BIT23_SHFT                                    23
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_RESERVE_BIT22_BMSK                              0x400000
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_RESERVE_BIT22_SHFT                                    22
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_RESERVE_BIT21_BMSK                              0x200000
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_RESERVE_BIT21_SHFT                                    21
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_RESERVE_BIT20_BMSK                              0x100000
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_RESERVE_BIT20_SHFT                                    20
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_RESERVE_BIT19_BMSK                               0x80000
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_RESERVE_BIT19_SHFT                                    19
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_PRE_DIV_RATIO_BMSK                               0x70000
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_PRE_DIV_RATIO_SHFT                                    16
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_POST_DIV_RATIO_ODD_BMSK                           0xf000
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_POST_DIV_RATIO_ODD_SHFT                               12
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_POST_DIV_RATIO_EVEN_BMSK                           0xf00
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_POST_DIV_RATIO_EVEN_SHFT                               8
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_OUT_CLK_POLARITY_BMSK                               0x80
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_OUT_CLK_POLARITY_SHFT                                  7
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_RESERVE_BIT6_BMSK                                   0x40
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_RESERVE_BIT6_SHFT                                      6
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_RESERVE_BIT5_BMSK                                   0x20
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_RESERVE_BIT5_SHFT                                      5
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_PLLOUT_TEST_BMSK                                    0x10
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_PLLOUT_TEST_SHFT                                       4
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_RESERVE_BIT3_BMSK                                    0x8
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_RESERVE_BIT3_SHFT                                      3
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_PLLOUT_ODD_BMSK                                      0x4
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_PLLOUT_ODD_SHFT                                        2
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_PLLOUT_EVEN_BMSK                                     0x2
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_PLLOUT_EVEN_SHFT                                       1
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_PLLOUT_MAIN_BMSK                                     0x1
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_PLLOUT_MAIN_SHFT                                       0

#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_U_ADDR                                        (MSS_QDSP6SS_QDSP6V671SS_PLL_REG_BASE            + 0x10)
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_U_OFFS                                        (MSS_QDSP6SS_QDSP6V671SS_PLL_REG_BASE_OFFS + 0x10)
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_U_RMSK                                        0xffffffff
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_U_IN                    \
                in_dword_masked(HWIO_MSS_QDSP6SS_PLL_USER_CTL_U_ADDR, HWIO_MSS_QDSP6SS_PLL_USER_CTL_U_RMSK)
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_U_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_PLL_USER_CTL_U_ADDR, m)
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_U_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_PLL_USER_CTL_U_ADDR,v)
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_U_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_QDSP6SS_PLL_USER_CTL_U_ADDR,m,v,HWIO_MSS_QDSP6SS_PLL_USER_CTL_U_IN)
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_U_RESERVE_BIT63_BMSK                          0x80000000
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_U_RESERVE_BIT63_SHFT                                  31
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_U_RESERVE_BIT62_BMSK                          0x40000000
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_U_RESERVE_BIT62_SHFT                                  30
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_U_RESERVE_BIT61_BMSK                          0x20000000
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_U_RESERVE_BIT61_SHFT                                  29
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_U_RESERVE_BIT60_BMSK                          0x10000000
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_U_RESERVE_BIT60_SHFT                                  28
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_U_RESERVE_BIT59_BMSK                           0x8000000
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_U_RESERVE_BIT59_SHFT                                  27
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_U_RESERVE_BIT58_BMSK                           0x4000000
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_U_RESERVE_BIT58_SHFT                                  26
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_U_RESERVE_BIT57_BMSK                           0x2000000
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_U_RESERVE_BIT57_SHFT                                  25
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_U_RESERVE_BIT56_BMSK                           0x1000000
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_U_RESERVE_BIT56_SHFT                                  24
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_U_RESERVE_BIT55_BMSK                            0x800000
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_U_RESERVE_BIT55_SHFT                                  23
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_U_RESERVE_BIT54_BMSK                            0x400000
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_U_RESERVE_BIT54_SHFT                                  22
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_U_RESERVE_BIT53_BMSK                            0x200000
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_U_RESERVE_BIT53_SHFT                                  21
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_U_RESERVE_BIT52_BMSK                            0x100000
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_U_RESERVE_BIT52_SHFT                                  20
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_U_RESERVE_BIT51_BMSK                             0x80000
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_U_RESERVE_BIT51_SHFT                                  19
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_U_LOCK_DETECTOR_SELECTION_BMSK                   0x40000
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_U_LOCK_DETECTOR_SELECTION_SHFT                        18
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_U_TYPE_2_MODE_SELECTION_BMSK                     0x20000
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_U_TYPE_2_MODE_SELECTION_SHFT                          17
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_U_BANG_BANG_MODE_SELECTION_BMSK                  0x10000
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_U_BANG_BANG_MODE_SELECTION_SHFT                       16
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_U_FRAC_FORMAT_SEL_BMSK                            0x8000
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_U_FRAC_FORMAT_SEL_SHFT                                15
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_U_RESERVE_BIT46_BMSK                              0x4000
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_U_RESERVE_BIT46_SHFT                                  14
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_U_REF_CLK_AT_OUT_BMSK                             0x2000
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_U_REF_CLK_AT_OUT_SHFT                                 13
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_U_SCALE_FREQ_ON_RESTART_BMSK                      0x1800
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_U_SCALE_FREQ_ON_RESTART_SHFT                          11
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_U_LATCH_INTERFACE_BYPASS_BMSK                      0x400
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_U_LATCH_INTERFACE_BYPASS_SHFT                         10
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_U_STATUS_REGISTER_BMSK                             0x3e0
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_U_STATUS_REGISTER_SHFT                                 5
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_U_WRITE_STATE_EN_BMSK                               0x10
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_U_WRITE_STATE_EN_SHFT                                  4
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_U_CALIB_CTRL_BMSK                                    0xe
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_U_CALIB_CTRL_SHFT                                      1
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_U_USE_ONLY_FINE_LOCK_DET_BMSK                        0x1
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_U_USE_ONLY_FINE_LOCK_DET_SHFT                          0

#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_U1_ADDR                                       (MSS_QDSP6SS_QDSP6V671SS_PLL_REG_BASE            + 0x14)
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_U1_OFFS                                       (MSS_QDSP6SS_QDSP6V671SS_PLL_REG_BASE_OFFS + 0x14)
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_U1_RMSK                                       0xffffffff
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_U1_IN                    \
                in_dword_masked(HWIO_MSS_QDSP6SS_PLL_USER_CTL_U1_ADDR, HWIO_MSS_QDSP6SS_PLL_USER_CTL_U1_RMSK)
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_U1_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_PLL_USER_CTL_U1_ADDR, m)
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_U1_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_PLL_USER_CTL_U1_ADDR,v)
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_U1_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_QDSP6SS_PLL_USER_CTL_U1_ADDR,m,v,HWIO_MSS_QDSP6SS_PLL_USER_CTL_U1_IN)
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_U1_RESERVE_BIT95_BMSK                         0x80000000
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_U1_RESERVE_BIT95_SHFT                                 31
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_U1_RESERVE_BIT94_BMSK                         0x40000000
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_U1_RESERVE_BIT94_SHFT                                 30
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_U1_RESERVE_BIT93_BMSK                         0x20000000
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_U1_RESERVE_BIT93_SHFT                                 29
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_U1_RESERVE_BIT92_BMSK                         0x10000000
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_U1_RESERVE_BIT92_SHFT                                 28
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_U1_RESERVE_BIT91_BMSK                          0x8000000
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_U1_RESERVE_BIT91_SHFT                                 27
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_U1_RESERVE_BIT90_BMSK                          0x4000000
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_U1_RESERVE_BIT90_SHFT                                 26
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_U1_RESERVE_BIT89_BMSK                          0x2000000
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_U1_RESERVE_BIT89_SHFT                                 25
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_U1_RESERVE_BIT88_BMSK                          0x1000000
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_U1_RESERVE_BIT88_SHFT                                 24
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_U1_RESERVE_BIT87_BMSK                           0x800000
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_U1_RESERVE_BIT87_SHFT                                 23
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_U1_RESERVE_BIT86_BMSK                           0x400000
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_U1_RESERVE_BIT86_SHFT                                 22
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_U1_RESERVE_BIT85_BMSK                           0x200000
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_U1_RESERVE_BIT85_SHFT                                 21
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_U1_RESERVE_BIT84_BMSK                           0x100000
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_U1_RESERVE_BIT84_SHFT                                 20
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_U1_RESERVE_BIT83_BMSK                            0x80000
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_U1_RESERVE_BIT83_SHFT                                 19
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_U1_RESERVE_BIT82_BMSK                            0x40000
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_U1_RESERVE_BIT82_SHFT                                 18
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_U1_RESERVE_BIT81_BMSK                            0x20000
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_U1_RESERVE_BIT81_SHFT                                 17
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_U1_RESERVE_BIT80_BMSK                            0x10000
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_U1_RESERVE_BIT80_SHFT                                 16
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_U1_RESERVE_BIT79_BMSK                             0x8000
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_U1_RESERVE_BIT79_SHFT                                 15
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_U1_RESERVE_BIT78_BMSK                             0x4000
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_U1_RESERVE_BIT78_SHFT                                 14
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_U1_RESERVE_BIT77_BMSK                             0x2000
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_U1_RESERVE_BIT77_SHFT                                 13
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_U1_RESERVE_BIT76_BMSK                             0x1000
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_U1_RESERVE_BIT76_SHFT                                 12
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_U1_RESERVE_BIT75_BMSK                              0x800
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_U1_RESERVE_BIT75_SHFT                                 11
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_U1_RESERVE_BIT74_BMSK                              0x400
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_U1_RESERVE_BIT74_SHFT                                 10
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_U1_RESERVE_BIT73_BMSK                              0x200
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_U1_RESERVE_BIT73_SHFT                                  9
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_U1_RESERVE_BIT72_BMSK                              0x100
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_U1_RESERVE_BIT72_SHFT                                  8
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_U1_RESERVE_BIT71_BMSK                               0x80
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_U1_RESERVE_BIT71_SHFT                                  7
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_U1_RESERVE_BIT70_BMSK                               0x40
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_U1_RESERVE_BIT70_SHFT                                  6
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_U1_RESERVE_BIT69_BMSK                               0x20
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_U1_RESERVE_BIT69_SHFT                                  5
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_U1_RESERVE_BIT68_BMSK                               0x10
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_U1_RESERVE_BIT68_SHFT                                  4
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_U1_RESERVE_BIT67_BMSK                                0x8
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_U1_RESERVE_BIT67_SHFT                                  3
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_U1_RESERVE_BIT66_BMSK                                0x4
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_U1_RESERVE_BIT66_SHFT                                  2
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_U1_RESERVE_BIT65_BMSK                                0x2
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_U1_RESERVE_BIT65_SHFT                                  1
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_U1_RESERVE_BIT64_BMSK                                0x1
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_U1_RESERVE_BIT64_SHFT                                  0

#define HWIO_MSS_QDSP6SS_PLL_CONFIG_CTL_ADDR                                        (MSS_QDSP6SS_QDSP6V671SS_PLL_REG_BASE            + 0x18)
#define HWIO_MSS_QDSP6SS_PLL_CONFIG_CTL_OFFS                                        (MSS_QDSP6SS_QDSP6V671SS_PLL_REG_BASE_OFFS + 0x18)
#define HWIO_MSS_QDSP6SS_PLL_CONFIG_CTL_RMSK                                        0xffffffff
#define HWIO_MSS_QDSP6SS_PLL_CONFIG_CTL_IN                    \
                in_dword_masked(HWIO_MSS_QDSP6SS_PLL_CONFIG_CTL_ADDR, HWIO_MSS_QDSP6SS_PLL_CONFIG_CTL_RMSK)
#define HWIO_MSS_QDSP6SS_PLL_CONFIG_CTL_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_PLL_CONFIG_CTL_ADDR, m)
#define HWIO_MSS_QDSP6SS_PLL_CONFIG_CTL_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_PLL_CONFIG_CTL_ADDR,v)
#define HWIO_MSS_QDSP6SS_PLL_CONFIG_CTL_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_QDSP6SS_PLL_CONFIG_CTL_ADDR,m,v,HWIO_MSS_QDSP6SS_PLL_CONFIG_CTL_IN)
#define HWIO_MSS_QDSP6SS_PLL_CONFIG_CTL_FINE_LDC_THRESHOLD_BMSK                     0xfc000000
#define HWIO_MSS_QDSP6SS_PLL_CONFIG_CTL_FINE_LDC_THRESHOLD_SHFT                             26
#define HWIO_MSS_QDSP6SS_PLL_CONFIG_CTL_COARSE_LDC_THRESHOLD_BMSK                    0x3c00000
#define HWIO_MSS_QDSP6SS_PLL_CONFIG_CTL_COARSE_LDC_THRESHOLD_SHFT                           22
#define HWIO_MSS_QDSP6SS_PLL_CONFIG_CTL_COARSE_LOCK_DET_NEG_THRESHOLD_BMSK            0x3c0000
#define HWIO_MSS_QDSP6SS_PLL_CONFIG_CTL_COARSE_LOCK_DET_NEG_THRESHOLD_SHFT                  18
#define HWIO_MSS_QDSP6SS_PLL_CONFIG_CTL_COARSE_LOCK_DET_POS_THRESHOLD_BMSK             0x3c000
#define HWIO_MSS_QDSP6SS_PLL_CONFIG_CTL_COARSE_LOCK_DET_POS_THRESHOLD_SHFT                  14
#define HWIO_MSS_QDSP6SS_PLL_CONFIG_CTL_FINE_LOCK_DET_THRESHOLD_BMSK                    0x3800
#define HWIO_MSS_QDSP6SS_PLL_CONFIG_CTL_FINE_LOCK_DET_THRESHOLD_SHFT                        11
#define HWIO_MSS_QDSP6SS_PLL_CONFIG_CTL_FINE_LOCK_DET_SAMPLE_SIZE_BMSK                   0x700
#define HWIO_MSS_QDSP6SS_PLL_CONFIG_CTL_FINE_LOCK_DET_SAMPLE_SIZE_SHFT                       8
#define HWIO_MSS_QDSP6SS_PLL_CONFIG_CTL_FWD_GAIN_SLEWING_KFN_BMSK                         0xf0
#define HWIO_MSS_QDSP6SS_PLL_CONFIG_CTL_FWD_GAIN_SLEWING_KFN_SHFT                            4
#define HWIO_MSS_QDSP6SS_PLL_CONFIG_CTL_FWD_GAIN_KFN_BMSK                                  0xf
#define HWIO_MSS_QDSP6SS_PLL_CONFIG_CTL_FWD_GAIN_KFN_SHFT                                    0

#define HWIO_MSS_QDSP6SS_PLL_CONFIG_CTL_U_ADDR                                      (MSS_QDSP6SS_QDSP6V671SS_PLL_REG_BASE            + 0x1c)
#define HWIO_MSS_QDSP6SS_PLL_CONFIG_CTL_U_OFFS                                      (MSS_QDSP6SS_QDSP6V671SS_PLL_REG_BASE_OFFS + 0x1c)
#define HWIO_MSS_QDSP6SS_PLL_CONFIG_CTL_U_RMSK                                      0xffffffff
#define HWIO_MSS_QDSP6SS_PLL_CONFIG_CTL_U_IN                    \
                in_dword_masked(HWIO_MSS_QDSP6SS_PLL_CONFIG_CTL_U_ADDR, HWIO_MSS_QDSP6SS_PLL_CONFIG_CTL_U_RMSK)
#define HWIO_MSS_QDSP6SS_PLL_CONFIG_CTL_U_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_PLL_CONFIG_CTL_U_ADDR, m)
#define HWIO_MSS_QDSP6SS_PLL_CONFIG_CTL_U_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_PLL_CONFIG_CTL_U_ADDR,v)
#define HWIO_MSS_QDSP6SS_PLL_CONFIG_CTL_U_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_QDSP6SS_PLL_CONFIG_CTL_U_ADDR,m,v,HWIO_MSS_QDSP6SS_PLL_CONFIG_CTL_U_IN)
#define HWIO_MSS_QDSP6SS_PLL_CONFIG_CTL_U_BIST_CFG_BMSK                             0xffff0000
#define HWIO_MSS_QDSP6SS_PLL_CONFIG_CTL_U_BIST_CFG_SHFT                                     16
#define HWIO_MSS_QDSP6SS_PLL_CONFIG_CTL_U_CASCODE_FREEZE_LOCK_DET_BMSK                  0xc000
#define HWIO_MSS_QDSP6SS_PLL_CONFIG_CTL_U_CASCODE_FREEZE_LOCK_DET_SHFT                      14
#define HWIO_MSS_QDSP6SS_PLL_CONFIG_CTL_U_OSC_SETTLING_TIME_BMSK                        0x3000
#define HWIO_MSS_QDSP6SS_PLL_CONFIG_CTL_U_OSC_SETTLING_TIME_SHFT                            12
#define HWIO_MSS_QDSP6SS_PLL_CONFIG_CTL_U_NUM_STEPS_FAST_LOCK_BMSK                       0xc00
#define HWIO_MSS_QDSP6SS_PLL_CONFIG_CTL_U_NUM_STEPS_FAST_LOCK_SHFT                          10
#define HWIO_MSS_QDSP6SS_PLL_CONFIG_CTL_U_CASC_CURRENT_CTL_B_BMSK                        0x380
#define HWIO_MSS_QDSP6SS_PLL_CONFIG_CTL_U_CASC_CURRENT_CTL_B_SHFT                            7
#define HWIO_MSS_QDSP6SS_PLL_CONFIG_CTL_U_INC_MIN_GLITCH_THRESHOLD_4X_BMSK                0x40
#define HWIO_MSS_QDSP6SS_PLL_CONFIG_CTL_U_INC_MIN_GLITCH_THRESHOLD_4X_SHFT                   6
#define HWIO_MSS_QDSP6SS_PLL_CONFIG_CTL_U_MIN_GLITCH_THRESHOLD_BMSK                       0x30
#define HWIO_MSS_QDSP6SS_PLL_CONFIG_CTL_U_MIN_GLITCH_THRESHOLD_SHFT                          4
#define HWIO_MSS_QDSP6SS_PLL_CONFIG_CTL_U_LDET_THRESH_SCALE_BMSK                           0x8
#define HWIO_MSS_QDSP6SS_PLL_CONFIG_CTL_U_LDET_THRESH_SCALE_SHFT                             3
#define HWIO_MSS_QDSP6SS_PLL_CONFIG_CTL_U_DCO_STARTUP_CONFIG_BMSK                          0x6
#define HWIO_MSS_QDSP6SS_PLL_CONFIG_CTL_U_DCO_STARTUP_CONFIG_SHFT                            1
#define HWIO_MSS_QDSP6SS_PLL_CONFIG_CTL_U_DIV_LOCK_DET_THRESHOLDS_BMSK                     0x1
#define HWIO_MSS_QDSP6SS_PLL_CONFIG_CTL_U_DIV_LOCK_DET_THRESHOLDS_SHFT                       0

#define HWIO_MSS_QDSP6SS_PLL_CONFIG_CTL_U1_ADDR                                     (MSS_QDSP6SS_QDSP6V671SS_PLL_REG_BASE            + 0x20)
#define HWIO_MSS_QDSP6SS_PLL_CONFIG_CTL_U1_OFFS                                     (MSS_QDSP6SS_QDSP6V671SS_PLL_REG_BASE_OFFS + 0x20)
#define HWIO_MSS_QDSP6SS_PLL_CONFIG_CTL_U1_RMSK                                     0xffffffff
#define HWIO_MSS_QDSP6SS_PLL_CONFIG_CTL_U1_IN                    \
                in_dword_masked(HWIO_MSS_QDSP6SS_PLL_CONFIG_CTL_U1_ADDR, HWIO_MSS_QDSP6SS_PLL_CONFIG_CTL_U1_RMSK)
#define HWIO_MSS_QDSP6SS_PLL_CONFIG_CTL_U1_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_PLL_CONFIG_CTL_U1_ADDR, m)
#define HWIO_MSS_QDSP6SS_PLL_CONFIG_CTL_U1_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_PLL_CONFIG_CTL_U1_ADDR,v)
#define HWIO_MSS_QDSP6SS_PLL_CONFIG_CTL_U1_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_QDSP6SS_PLL_CONFIG_CTL_U1_ADDR,m,v,HWIO_MSS_QDSP6SS_PLL_CONFIG_CTL_U1_IN)
#define HWIO_MSS_QDSP6SS_PLL_CONFIG_CTL_U1_BB_CAL_THRESHOLD_BMSK                    0xc0000000
#define HWIO_MSS_QDSP6SS_PLL_CONFIG_CTL_U1_BB_CAL_THRESHOLD_SHFT                            30
#define HWIO_MSS_QDSP6SS_PLL_CONFIG_CTL_U1_PCODE_OFFSET_BMSK                        0x38000000
#define HWIO_MSS_QDSP6SS_PLL_CONFIG_CTL_U1_PCODE_OFFSET_SHFT                                27
#define HWIO_MSS_QDSP6SS_PLL_CONFIG_CTL_U1_COUNTER_BLANK_BMSK                        0x4000000
#define HWIO_MSS_QDSP6SS_PLL_CONFIG_CTL_U1_COUNTER_BLANK_SHFT                               26
#define HWIO_MSS_QDSP6SS_PLL_CONFIG_CTL_U1_BB_RESYNC_EDGE_BMSK                       0x2000000
#define HWIO_MSS_QDSP6SS_PLL_CONFIG_CTL_U1_BB_RESYNC_EDGE_SHFT                              25
#define HWIO_MSS_QDSP6SS_PLL_CONFIG_CTL_U1_BB_RESYNC_SEL_BMSK                        0x1000000
#define HWIO_MSS_QDSP6SS_PLL_CONFIG_CTL_U1_BB_RESYNC_SEL_SHFT                               24
#define HWIO_MSS_QDSP6SS_PLL_CONFIG_CTL_U1_BB_LOCK_NUM_SAMPLES_BMSK                   0xc00000
#define HWIO_MSS_QDSP6SS_PLL_CONFIG_CTL_U1_BB_LOCK_NUM_SAMPLES_SHFT                         22
#define HWIO_MSS_QDSP6SS_PLL_CONFIG_CTL_U1_BB_LOCK_THRESH_BMSK                        0x380000
#define HWIO_MSS_QDSP6SS_PLL_CONFIG_CTL_U1_BB_LOCK_THRESH_SHFT                              19
#define HWIO_MSS_QDSP6SS_PLL_CONFIG_CTL_U1_RESERVE_BIT82_BMSK                          0x40000
#define HWIO_MSS_QDSP6SS_PLL_CONFIG_CTL_U1_RESERVE_BIT82_SHFT                               18
#define HWIO_MSS_QDSP6SS_PLL_CONFIG_CTL_U1_BB_CAL_NUM_SAMPLES_BMSK                     0x30000
#define HWIO_MSS_QDSP6SS_PLL_CONFIG_CTL_U1_BB_CAL_NUM_SAMPLES_SHFT                          16
#define HWIO_MSS_QDSP6SS_PLL_CONFIG_CTL_U1_BB_CAL_CORR_INDEX_BMSK                       0xc000
#define HWIO_MSS_QDSP6SS_PLL_CONFIG_CTL_U1_BB_CAL_CORR_INDEX_SHFT                           14
#define HWIO_MSS_QDSP6SS_PLL_CONFIG_CTL_U1_BB_CAL_EN_BMSK                               0x2000
#define HWIO_MSS_QDSP6SS_PLL_CONFIG_CTL_U1_BB_CAL_EN_SHFT                                   13
#define HWIO_MSS_QDSP6SS_PLL_CONFIG_CTL_U1_DELAY_AFTER_FLL_LOCKDET_BMSK                 0x1800
#define HWIO_MSS_QDSP6SS_PLL_CONFIG_CTL_U1_DELAY_AFTER_FLL_LOCKDET_SHFT                     11
#define HWIO_MSS_QDSP6SS_PLL_CONFIG_CTL_U1_BBMODE_INT_GAIN_BMSK                          0x780
#define HWIO_MSS_QDSP6SS_PLL_CONFIG_CTL_U1_BBMODE_INT_GAIN_SHFT                              7
#define HWIO_MSS_QDSP6SS_PLL_CONFIG_CTL_U1_BBMODE_PROP_GAIN_BMSK                          0x78
#define HWIO_MSS_QDSP6SS_PLL_CONFIG_CTL_U1_BBMODE_PROP_GAIN_SHFT                             3
#define HWIO_MSS_QDSP6SS_PLL_CONFIG_CTL_U1_TDC_SELECTION_BMSK                              0x4
#define HWIO_MSS_QDSP6SS_PLL_CONFIG_CTL_U1_TDC_SELECTION_SHFT                                2
#define HWIO_MSS_QDSP6SS_PLL_CONFIG_CTL_U1_RESERVE_BIT65_BMSK                              0x2
#define HWIO_MSS_QDSP6SS_PLL_CONFIG_CTL_U1_RESERVE_BIT65_SHFT                                1
#define HWIO_MSS_QDSP6SS_PLL_CONFIG_CTL_U1_FORCE_DSM_ON_BMSK                               0x1
#define HWIO_MSS_QDSP6SS_PLL_CONFIG_CTL_U1_FORCE_DSM_ON_SHFT                                 0

#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_ADDR                                          (MSS_QDSP6SS_QDSP6V671SS_PLL_REG_BASE            + 0x24)
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_OFFS                                          (MSS_QDSP6SS_QDSP6V671SS_PLL_REG_BASE_OFFS + 0x24)
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_RMSK                                          0xffffffff
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_IN                    \
                in_dword_masked(HWIO_MSS_QDSP6SS_PLL_TEST_CTL_ADDR, HWIO_MSS_QDSP6SS_PLL_TEST_CTL_RMSK)
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_PLL_TEST_CTL_ADDR, m)
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_PLL_TEST_CTL_ADDR,v)
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_QDSP6SS_PLL_TEST_CTL_ADDR,m,v,HWIO_MSS_QDSP6SS_PLL_TEST_CTL_IN)
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_ADD_REF_CYC_CALIB_STEPS_BMSK                  0xc0000000
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_ADD_REF_CYC_CALIB_STEPS_SHFT                          30
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_BYPASS_FIRST_ORDER_DSM_BMSK                   0x20000000
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_BYPASS_FIRST_ORDER_DSM_SHFT                           29
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_GLITCH_DETECTOR_COUNT_LIMIT_BMSK              0x18000000
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_GLITCH_DETECTOR_COUNT_LIMIT_SHFT                      27
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_GLITCH_PREVENTION_DIS_BMSK                     0x4000000
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_GLITCH_PREVENTION_DIS_SHFT                            26
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_ATEST_ISCALE_BMSK                              0x3000000
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_ATEST_ISCALE_SHFT                                     24
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_PLL_TEST_OUT_SEL_BMSK                           0xf00000
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_PLL_TEST_OUT_SEL_SHFT                                 20
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_RESERVE_BIT19_BMSK                               0x80000
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_RESERVE_BIT19_SHFT                                    19
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_DIV2_NMO_EN_BMSK                                 0x40000
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_DIV2_NMO_EN_SHFT                                      18
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_NMO_OSC_SEL_BMSK                                 0x30000
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_NMO_OSC_SEL_SHFT                                      16
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_NOISE_MAG_BMSK                                    0xe000
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_NOISE_MAG_SHFT                                        13
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_NOISE_GEN_EN_BMSK                                 0x1000
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_NOISE_GEN_EN_SHFT                                     12
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_DISABLE_DCAPS_BMSK                                 0x800
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_DISABLE_DCAPS_SHFT                                    11
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_RESERVE_BIT10_BMSK                                 0x400
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_RESERVE_BIT10_SHFT                                    10
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_RESERVE_BIT9_BMSK                                  0x200
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_RESERVE_BIT9_SHFT                                      9
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_BYPASS_MODE_FOR_BIAS_EN_BMSK                       0x100
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_BYPASS_MODE_FOR_BIAS_EN_SHFT                           8
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_EN_VDD_TEST_BMSK                                    0x80
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_EN_VDD_TEST_SHFT                                       7
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_EN_VREG_TEST_BMSK                                   0x40
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_EN_VREG_TEST_SHFT                                      6
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_EN_VREF_TEST_BMSK                                   0x20
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_EN_VREF_TEST_SHFT                                      5
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_EN_VDCO_TEST_BMSK                                   0x10
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_EN_VDCO_TEST_SHFT                                      4
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_BYPASS_ATEST_BUFFER_BMSK                             0x8
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_BYPASS_ATEST_BUFFER_SHFT                               3
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_EN_IVCO_TEST_BMSK                                    0x4
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_EN_IVCO_TEST_SHFT                                      2
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_ATEST1_EN_BMSK                                       0x2
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_ATEST1_EN_SHFT                                         1
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_ATEST0_EN_BMSK                                       0x1
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_ATEST0_EN_SHFT                                         0

#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_U_ADDR                                        (MSS_QDSP6SS_QDSP6V671SS_PLL_REG_BASE            + 0x28)
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_U_OFFS                                        (MSS_QDSP6SS_QDSP6V671SS_PLL_REG_BASE_OFFS + 0x28)
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_U_RMSK                                        0xffffffff
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_U_IN                    \
                in_dword_masked(HWIO_MSS_QDSP6SS_PLL_TEST_CTL_U_ADDR, HWIO_MSS_QDSP6SS_PLL_TEST_CTL_U_RMSK)
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_U_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_PLL_TEST_CTL_U_ADDR, m)
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_U_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_PLL_TEST_CTL_U_ADDR,v)
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_U_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_QDSP6SS_PLL_TEST_CTL_U_ADDR,m,v,HWIO_MSS_QDSP6SS_PLL_TEST_CTL_U_IN)
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_U_CASC_DAC_FCW_LSBS_BMSK                      0xe0000000
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_U_CASC_DAC_FCW_LSBS_SHFT                              29
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_U_OVERRIDE_CASC_DAC_BMSK                      0x10000000
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_U_OVERRIDE_CASC_DAC_SHFT                              28
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_U_COMP_COARSE_FCW_BMSK                         0xe000000
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_U_COMP_COARSE_FCW_SHFT                                25
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_U_OVERRIDE_COMP_COARSE_FCW_BMSK                0x1800000
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_U_OVERRIDE_COMP_COARSE_FCW_SHFT                       23
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_U_FINE_FCW_BMSK                                 0x7e0000
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_U_FINE_FCW_SHFT                                       17
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_U_OVERRIDE_FINE_FCW_BMSK                         0x10000
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_U_OVERRIDE_FINE_FCW_SHFT                              16
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_U_REG_COARSE_FCW_BMSK                             0xfe00
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_U_REG_COARSE_FCW_SHFT                                  9
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_U_OVERRIDE_REG_COARSE_FCW_BMSK                     0x100
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_U_OVERRIDE_REG_COARSE_FCW_SHFT                         8
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_U_PROCESS_CALIB_WORD_BMSK                           0xe0
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_U_PROCESS_CALIB_WORD_SHFT                              5
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_U_OVERRIDE_CALIB_WORD_BMSK                          0x10
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_U_OVERRIDE_CALIB_WORD_SHFT                             4
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_U_DISABLE_CASC_DAC_LATCH_BMSK                        0x8
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_U_DISABLE_CASC_DAC_LATCH_SHFT                          3
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_U_DISABLE_LFSR_BMSK                                  0x4
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_U_DISABLE_LFSR_SHFT                                    2
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_U_SKIP_FINE_TUNE_BMSK                                0x2
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_U_SKIP_FINE_TUNE_SHFT                                  1
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_U_SKIP_FAST_LOCK_BMSK                                0x1
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_U_SKIP_FAST_LOCK_SHFT                                  0

#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_U1_ADDR                                       (MSS_QDSP6SS_QDSP6V671SS_PLL_REG_BASE            + 0x2c)
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_U1_OFFS                                       (MSS_QDSP6SS_QDSP6V671SS_PLL_REG_BASE_OFFS + 0x2c)
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_U1_RMSK                                       0xffffffff
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_U1_IN                    \
                in_dword_masked(HWIO_MSS_QDSP6SS_PLL_TEST_CTL_U1_ADDR, HWIO_MSS_QDSP6SS_PLL_TEST_CTL_U1_RMSK)
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_U1_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_PLL_TEST_CTL_U1_ADDR, m)
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_U1_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_PLL_TEST_CTL_U1_ADDR,v)
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_U1_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_QDSP6SS_PLL_TEST_CTL_U1_ADDR,m,v,HWIO_MSS_QDSP6SS_PLL_TEST_CTL_U1_IN)
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_U1_RESERVE_BIT95_BMSK                         0x80000000
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_U1_RESERVE_BIT95_SHFT                                 31
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_U1_RESERVE_BIT94_BMSK                         0x40000000
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_U1_RESERVE_BIT94_SHFT                                 30
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_U1_FLL_COUNT_DISABLE_BMSK                     0x20000000
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_U1_FLL_COUNT_DISABLE_SHFT                             29
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_U1_FLL_COUNT_IN_DIV2_BMSK                     0x10000000
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_U1_FLL_COUNT_IN_DIV2_SHFT                             28
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_U1_RESERVE_BIT91_BMSK                          0x8000000
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_U1_RESERVE_BIT91_SHFT                                 27
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_U1_RESERVE_BIT90_BMSK                          0x4000000
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_U1_RESERVE_BIT90_SHFT                                 26
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_U1_RESERVE_BIT89_BMSK                          0x2000000
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_U1_RESERVE_BIT89_SHFT                                 25
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_U1_RESERVE_BIT88_BMSK                          0x1000000
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_U1_RESERVE_BIT88_SHFT                                 24
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_U1_RESERVE_BIT87_BMSK                           0x800000
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_U1_RESERVE_BIT87_SHFT                                 23
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_U1_SILENCE_REF_OUTBUF_BMSK                      0x400000
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_U1_SILENCE_REF_OUTBUF_SHFT                            22
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_U1_RESERVE_BIT85_BMSK                           0x200000
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_U1_RESERVE_BIT85_SHFT                                 21
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_U1_RESERVE_BIT84_BMSK                           0x100000
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_U1_RESERVE_BIT84_SHFT                                 20
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_U1_RESERVE_BIT83_BMSK                            0x80000
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_U1_RESERVE_BIT83_SHFT                                 19
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_U1_DISABLE_OSCBUF_DYN_SW_BMSK                    0x40000
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_U1_DISABLE_OSCBUF_DYN_SW_SHFT                         18
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_U1_REF_CLK_SEL_BMSK                              0x20000
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_U1_REF_CLK_SEL_SHFT                                   17
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_U1_DIS_DAC_AMP_STARTUP_LATCH_BMSK                0x10000
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_U1_DIS_DAC_AMP_STARTUP_LATCH_SHFT                     16
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_U1_DIS_AMP_STARTUP_BMSK                           0x8000
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_U1_DIS_AMP_STARTUP_SHFT                               15
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_U1_DISABLE_CLAMP_BMSK                             0x4000
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_U1_DISABLE_CLAMP_SHFT                                 14
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_U1_DISABLE_DAC_AMP_CASCODE_BMSK                   0x2000
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_U1_DISABLE_DAC_AMP_CASCODE_SHFT                       13
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_U1_RESERVE_BIT76_BMSK                             0x1000
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_U1_RESERVE_BIT76_SHFT                                 12
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_U1_RESERVE_BIT75_BMSK                              0x800
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_U1_RESERVE_BIT75_SHFT                                 11
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_U1_RESERVE_BIT74_BMSK                              0x400
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_U1_RESERVE_BIT74_SHFT                                 10
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_U1_RESERVE_BIT73_BMSK                              0x200
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_U1_RESERVE_BIT73_SHFT                                  9
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_U1_RESERVE_BIT72_BMSK                              0x100
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_U1_RESERVE_BIT72_SHFT                                  8
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_U1_COARSE_BIN_OVERRIDE_LOC_BMSK                     0x80
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_U1_COARSE_BIN_OVERRIDE_LOC_SHFT                        7
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_U1_ENABLE_TRAINING_SEQ_BMSK                         0x40
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_U1_ENABLE_TRAINING_SEQ_SHFT                            6
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_U1_STATUS_REG_EN_BMSK                               0x20
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_U1_STATUS_REG_EN_SHFT                                  5
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_U1_RESERVE_BIT68_BMSK                               0x10
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_U1_RESERVE_BIT68_SHFT                                  4
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_U1_RESERVE_BIT67_BMSK                                0x8
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_U1_RESERVE_BIT67_SHFT                                  3
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_U1_RESERVE_BIT66_BMSK                                0x4
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_U1_RESERVE_BIT66_SHFT                                  2
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_U1_CASC_DAC_FCW_MSBS_BMSK                            0x3
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_U1_CASC_DAC_FCW_MSBS_SHFT                              0

#define HWIO_MSS_QDSP6SS_PLL_STATUS_ADDR                                            (MSS_QDSP6SS_QDSP6V671SS_PLL_REG_BASE            + 0x30)
#define HWIO_MSS_QDSP6SS_PLL_STATUS_OFFS                                            (MSS_QDSP6SS_QDSP6V671SS_PLL_REG_BASE_OFFS + 0x30)
#define HWIO_MSS_QDSP6SS_PLL_STATUS_RMSK                                            0xffffffff
#define HWIO_MSS_QDSP6SS_PLL_STATUS_IN                    \
                in_dword_masked(HWIO_MSS_QDSP6SS_PLL_STATUS_ADDR, HWIO_MSS_QDSP6SS_PLL_STATUS_RMSK)
#define HWIO_MSS_QDSP6SS_PLL_STATUS_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_PLL_STATUS_ADDR, m)
#define HWIO_MSS_QDSP6SS_PLL_STATUS_STATUS_31_0_BMSK                                0xffffffff
#define HWIO_MSS_QDSP6SS_PLL_STATUS_STATUS_31_0_SHFT                                         0

#define HWIO_MSS_QDSP6SS_PLL_FREQ_CTL_ADDR                                          (MSS_QDSP6SS_QDSP6V671SS_PLL_REG_BASE            + 0x34)
#define HWIO_MSS_QDSP6SS_PLL_FREQ_CTL_OFFS                                          (MSS_QDSP6SS_QDSP6V671SS_PLL_REG_BASE_OFFS + 0x34)
#define HWIO_MSS_QDSP6SS_PLL_FREQ_CTL_RMSK                                          0xffffffff
#define HWIO_MSS_QDSP6SS_PLL_FREQ_CTL_IN                    \
                in_dword_masked(HWIO_MSS_QDSP6SS_PLL_FREQ_CTL_ADDR, HWIO_MSS_QDSP6SS_PLL_FREQ_CTL_RMSK)
#define HWIO_MSS_QDSP6SS_PLL_FREQ_CTL_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_PLL_FREQ_CTL_ADDR, m)
#define HWIO_MSS_QDSP6SS_PLL_FREQ_CTL_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_PLL_FREQ_CTL_ADDR,v)
#define HWIO_MSS_QDSP6SS_PLL_FREQ_CTL_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_QDSP6SS_PLL_FREQ_CTL_ADDR,m,v,HWIO_MSS_QDSP6SS_PLL_FREQ_CTL_IN)
#define HWIO_MSS_QDSP6SS_PLL_FREQ_CTL_FREQUENCY_CTL_WORD_BMSK                       0xffffffff
#define HWIO_MSS_QDSP6SS_PLL_FREQ_CTL_FREQUENCY_CTL_WORD_SHFT                                0

#define HWIO_MSS_QDSP6SS_PLL_OPMODE_ADDR                                            (MSS_QDSP6SS_QDSP6V671SS_PLL_REG_BASE            + 0x38)
#define HWIO_MSS_QDSP6SS_PLL_OPMODE_OFFS                                            (MSS_QDSP6SS_QDSP6V671SS_PLL_REG_BASE_OFFS + 0x38)
#define HWIO_MSS_QDSP6SS_PLL_OPMODE_RMSK                                                   0x7
#define HWIO_MSS_QDSP6SS_PLL_OPMODE_IN                    \
                in_dword_masked(HWIO_MSS_QDSP6SS_PLL_OPMODE_ADDR, HWIO_MSS_QDSP6SS_PLL_OPMODE_RMSK)
#define HWIO_MSS_QDSP6SS_PLL_OPMODE_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_PLL_OPMODE_ADDR, m)
#define HWIO_MSS_QDSP6SS_PLL_OPMODE_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_PLL_OPMODE_ADDR,v)
#define HWIO_MSS_QDSP6SS_PLL_OPMODE_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_QDSP6SS_PLL_OPMODE_ADDR,m,v,HWIO_MSS_QDSP6SS_PLL_OPMODE_IN)
#define HWIO_MSS_QDSP6SS_PLL_OPMODE_PLL_OPMODE_BMSK                                        0x7
#define HWIO_MSS_QDSP6SS_PLL_OPMODE_PLL_OPMODE_SHFT                                          0

#define HWIO_MSS_QDSP6SS_PLL_STATE_ADDR                                             (MSS_QDSP6SS_QDSP6V671SS_PLL_REG_BASE            + 0x3c)
#define HWIO_MSS_QDSP6SS_PLL_STATE_OFFS                                             (MSS_QDSP6SS_QDSP6V671SS_PLL_REG_BASE_OFFS + 0x3c)
#define HWIO_MSS_QDSP6SS_PLL_STATE_RMSK                                                    0x7
#define HWIO_MSS_QDSP6SS_PLL_STATE_IN                    \
                in_dword_masked(HWIO_MSS_QDSP6SS_PLL_STATE_ADDR, HWIO_MSS_QDSP6SS_PLL_STATE_RMSK)
#define HWIO_MSS_QDSP6SS_PLL_STATE_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_PLL_STATE_ADDR, m)
#define HWIO_MSS_QDSP6SS_PLL_STATE_PLL_STATE_BMSK                                          0x7
#define HWIO_MSS_QDSP6SS_PLL_STATE_PLL_STATE_SHFT                                            0

#define HWIO_MSS_QDSP6SS_PLL_ALPHA_VAL_ADDR                                         (MSS_QDSP6SS_QDSP6V671SS_PLL_REG_BASE            + 0x40)
#define HWIO_MSS_QDSP6SS_PLL_ALPHA_VAL_OFFS                                         (MSS_QDSP6SS_QDSP6V671SS_PLL_REG_BASE_OFFS + 0x40)
#define HWIO_MSS_QDSP6SS_PLL_ALPHA_VAL_RMSK                                             0xffff
#define HWIO_MSS_QDSP6SS_PLL_ALPHA_VAL_IN                    \
                in_dword_masked(HWIO_MSS_QDSP6SS_PLL_ALPHA_VAL_ADDR, HWIO_MSS_QDSP6SS_PLL_ALPHA_VAL_RMSK)
#define HWIO_MSS_QDSP6SS_PLL_ALPHA_VAL_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_PLL_ALPHA_VAL_ADDR, m)
#define HWIO_MSS_QDSP6SS_PLL_ALPHA_VAL_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_PLL_ALPHA_VAL_ADDR,v)
#define HWIO_MSS_QDSP6SS_PLL_ALPHA_VAL_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_QDSP6SS_PLL_ALPHA_VAL_ADDR,m,v,HWIO_MSS_QDSP6SS_PLL_ALPHA_VAL_IN)
#define HWIO_MSS_QDSP6SS_PLL_ALPHA_VAL_PLL_ALPHA_VAL_BMSK                               0xffff
#define HWIO_MSS_QDSP6SS_PLL_ALPHA_VAL_PLL_ALPHA_VAL_SHFT                                    0

#define HWIO_MSS_QDSP6SS_PLL_SPARE_ADDR                                             (MSS_QDSP6SS_QDSP6V671SS_PLL_REG_BASE            + 0x44)
#define HWIO_MSS_QDSP6SS_PLL_SPARE_OFFS                                             (MSS_QDSP6SS_QDSP6V671SS_PLL_REG_BASE_OFFS + 0x44)
#define HWIO_MSS_QDSP6SS_PLL_SPARE_RMSK                                                   0xff
#define HWIO_MSS_QDSP6SS_PLL_SPARE_IN                    \
                in_dword_masked(HWIO_MSS_QDSP6SS_PLL_SPARE_ADDR, HWIO_MSS_QDSP6SS_PLL_SPARE_RMSK)
#define HWIO_MSS_QDSP6SS_PLL_SPARE_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_PLL_SPARE_ADDR, m)
#define HWIO_MSS_QDSP6SS_PLL_SPARE_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_PLL_SPARE_ADDR,v)
#define HWIO_MSS_QDSP6SS_PLL_SPARE_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_QDSP6SS_PLL_SPARE_ADDR,m,v,HWIO_MSS_QDSP6SS_PLL_SPARE_IN)
#define HWIO_MSS_QDSP6SS_PLL_SPARE_SPARE_OUTPUTS_BMSK                                     0xf0
#define HWIO_MSS_QDSP6SS_PLL_SPARE_SPARE_OUTPUTS_SHFT                                        4
#define HWIO_MSS_QDSP6SS_PLL_SPARE_SPARE_INPUTS_BMSK                                       0xf
#define HWIO_MSS_QDSP6SS_PLL_SPARE_SPARE_INPUTS_SHFT                                         0

#define HWIO_MSS_QDSP6SS_PLL_SSC_DELTA_ALPHA_ADDR                                   (MSS_QDSP6SS_QDSP6V671SS_PLL_REG_BASE            + 0x48)
#define HWIO_MSS_QDSP6SS_PLL_SSC_DELTA_ALPHA_OFFS                                   (MSS_QDSP6SS_QDSP6V671SS_PLL_REG_BASE_OFFS + 0x48)
#define HWIO_MSS_QDSP6SS_PLL_SSC_DELTA_ALPHA_RMSK                                         0xff
#define HWIO_MSS_QDSP6SS_PLL_SSC_DELTA_ALPHA_IN                    \
                in_dword_masked(HWIO_MSS_QDSP6SS_PLL_SSC_DELTA_ALPHA_ADDR, HWIO_MSS_QDSP6SS_PLL_SSC_DELTA_ALPHA_RMSK)
#define HWIO_MSS_QDSP6SS_PLL_SSC_DELTA_ALPHA_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_PLL_SSC_DELTA_ALPHA_ADDR, m)
#define HWIO_MSS_QDSP6SS_PLL_SSC_DELTA_ALPHA_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_PLL_SSC_DELTA_ALPHA_ADDR,v)
#define HWIO_MSS_QDSP6SS_PLL_SSC_DELTA_ALPHA_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_QDSP6SS_PLL_SSC_DELTA_ALPHA_ADDR,m,v,HWIO_MSS_QDSP6SS_PLL_SSC_DELTA_ALPHA_IN)
#define HWIO_MSS_QDSP6SS_PLL_SSC_DELTA_ALPHA_SSC_DELTA_ALPHA_BMSK                         0xff
#define HWIO_MSS_QDSP6SS_PLL_SSC_DELTA_ALPHA_SSC_DELTA_ALPHA_SHFT                            0

#define HWIO_MSS_QDSP6SS_PLL_SSC_NUM_STEPS_ADDR                                     (MSS_QDSP6SS_QDSP6V671SS_PLL_REG_BASE            + 0x4c)
#define HWIO_MSS_QDSP6SS_PLL_SSC_NUM_STEPS_OFFS                                     (MSS_QDSP6SS_QDSP6V671SS_PLL_REG_BASE_OFFS + 0x4c)
#define HWIO_MSS_QDSP6SS_PLL_SSC_NUM_STEPS_RMSK                                            0xf
#define HWIO_MSS_QDSP6SS_PLL_SSC_NUM_STEPS_IN                    \
                in_dword_masked(HWIO_MSS_QDSP6SS_PLL_SSC_NUM_STEPS_ADDR, HWIO_MSS_QDSP6SS_PLL_SSC_NUM_STEPS_RMSK)
#define HWIO_MSS_QDSP6SS_PLL_SSC_NUM_STEPS_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_PLL_SSC_NUM_STEPS_ADDR, m)
#define HWIO_MSS_QDSP6SS_PLL_SSC_NUM_STEPS_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_PLL_SSC_NUM_STEPS_ADDR,v)
#define HWIO_MSS_QDSP6SS_PLL_SSC_NUM_STEPS_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_QDSP6SS_PLL_SSC_NUM_STEPS_ADDR,m,v,HWIO_MSS_QDSP6SS_PLL_SSC_NUM_STEPS_IN)
#define HWIO_MSS_QDSP6SS_PLL_SSC_NUM_STEPS_SSC_NUM_STEPS_BMSK                              0xf
#define HWIO_MSS_QDSP6SS_PLL_SSC_NUM_STEPS_SSC_NUM_STEPS_SHFT                                0

#define HWIO_MSS_QDSP6SS_PLL_SSC_UPDATE_RATE_ADDR                                   (MSS_QDSP6SS_QDSP6V671SS_PLL_REG_BASE            + 0x50)
#define HWIO_MSS_QDSP6SS_PLL_SSC_UPDATE_RATE_OFFS                                   (MSS_QDSP6SS_QDSP6V671SS_PLL_REG_BASE_OFFS + 0x50)
#define HWIO_MSS_QDSP6SS_PLL_SSC_UPDATE_RATE_RMSK                                          0xf
#define HWIO_MSS_QDSP6SS_PLL_SSC_UPDATE_RATE_IN                    \
                in_dword_masked(HWIO_MSS_QDSP6SS_PLL_SSC_UPDATE_RATE_ADDR, HWIO_MSS_QDSP6SS_PLL_SSC_UPDATE_RATE_RMSK)
#define HWIO_MSS_QDSP6SS_PLL_SSC_UPDATE_RATE_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_PLL_SSC_UPDATE_RATE_ADDR, m)
#define HWIO_MSS_QDSP6SS_PLL_SSC_UPDATE_RATE_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_PLL_SSC_UPDATE_RATE_ADDR,v)
#define HWIO_MSS_QDSP6SS_PLL_SSC_UPDATE_RATE_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_QDSP6SS_PLL_SSC_UPDATE_RATE_ADDR,m,v,HWIO_MSS_QDSP6SS_PLL_SSC_UPDATE_RATE_IN)
#define HWIO_MSS_QDSP6SS_PLL_SSC_UPDATE_RATE_SSC_UPDATE_RATE_BMSK                          0xf
#define HWIO_MSS_QDSP6SS_PLL_SSC_UPDATE_RATE_SSC_UPDATE_RATE_SHFT                            0

/*----------------------------------------------------------------------------
 * MODULE: MSS_QDSP6SS_QDSP6SS_QTMR_AC
 *--------------------------------------------------------------------------*/

#define MSS_QDSP6SS_QDSP6SS_QTMR_AC_REG_BASE                                (MSS_TOP_BASE            + 0x00120000)
#define MSS_QDSP6SS_QDSP6SS_QTMR_AC_REG_BASE_SIZE                           0x1000
#define MSS_QDSP6SS_QDSP6SS_QTMR_AC_REG_BASE_USED                           0xff0
#define MSS_QDSP6SS_QDSP6SS_QTMR_AC_REG_BASE_OFFS                           0x00120000

#define HWIO_MSS_QDSP6SS_QTMR_AC_CNTFRQ_ADDR                                (MSS_QDSP6SS_QDSP6SS_QTMR_AC_REG_BASE            + 0x0)
#define HWIO_MSS_QDSP6SS_QTMR_AC_CNTFRQ_OFFS                                (MSS_QDSP6SS_QDSP6SS_QTMR_AC_REG_BASE_OFFS + 0x0)
#define HWIO_MSS_QDSP6SS_QTMR_AC_CNTFRQ_RMSK                                0xffffffff
#define HWIO_MSS_QDSP6SS_QTMR_AC_CNTFRQ_IN                    \
                in_dword_masked(HWIO_MSS_QDSP6SS_QTMR_AC_CNTFRQ_ADDR, HWIO_MSS_QDSP6SS_QTMR_AC_CNTFRQ_RMSK)
#define HWIO_MSS_QDSP6SS_QTMR_AC_CNTFRQ_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_QTMR_AC_CNTFRQ_ADDR, m)
#define HWIO_MSS_QDSP6SS_QTMR_AC_CNTFRQ_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_QTMR_AC_CNTFRQ_ADDR,v)
#define HWIO_MSS_QDSP6SS_QTMR_AC_CNTFRQ_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_QDSP6SS_QTMR_AC_CNTFRQ_ADDR,m,v,HWIO_MSS_QDSP6SS_QTMR_AC_CNTFRQ_IN)
#define HWIO_MSS_QDSP6SS_QTMR_AC_CNTFRQ_CNTFRQ_BMSK                         0xffffffff
#define HWIO_MSS_QDSP6SS_QTMR_AC_CNTFRQ_CNTFRQ_SHFT                                  0

#define HWIO_MSS_QDSP6SS_QTMR_AC_CNTNSAR_FG0_ADDR                           (MSS_QDSP6SS_QDSP6SS_QTMR_AC_REG_BASE            + 0x4)
#define HWIO_MSS_QDSP6SS_QTMR_AC_CNTNSAR_FG0_OFFS                           (MSS_QDSP6SS_QDSP6SS_QTMR_AC_REG_BASE_OFFS + 0x4)
#define HWIO_MSS_QDSP6SS_QTMR_AC_CNTNSAR_FG0_RMSK                                  0x7
#define HWIO_MSS_QDSP6SS_QTMR_AC_CNTNSAR_FG0_IN                    \
                in_dword_masked(HWIO_MSS_QDSP6SS_QTMR_AC_CNTNSAR_FG0_ADDR, HWIO_MSS_QDSP6SS_QTMR_AC_CNTNSAR_FG0_RMSK)
#define HWIO_MSS_QDSP6SS_QTMR_AC_CNTNSAR_FG0_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_QTMR_AC_CNTNSAR_FG0_ADDR, m)
#define HWIO_MSS_QDSP6SS_QTMR_AC_CNTNSAR_FG0_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_QTMR_AC_CNTNSAR_FG0_ADDR,v)
#define HWIO_MSS_QDSP6SS_QTMR_AC_CNTNSAR_FG0_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_QDSP6SS_QTMR_AC_CNTNSAR_FG0_ADDR,m,v,HWIO_MSS_QDSP6SS_QTMR_AC_CNTNSAR_FG0_IN)
#define HWIO_MSS_QDSP6SS_QTMR_AC_CNTNSAR_FG0_NSN_BMSK                              0x7
#define HWIO_MSS_QDSP6SS_QTMR_AC_CNTNSAR_FG0_NSN_SHFT                                0

#define HWIO_MSS_QDSP6SS_QTMR_AC_CNTTIDR_FG0_ADDR                           (MSS_QDSP6SS_QDSP6SS_QTMR_AC_REG_BASE            + 0x8)
#define HWIO_MSS_QDSP6SS_QTMR_AC_CNTTIDR_FG0_OFFS                           (MSS_QDSP6SS_QDSP6SS_QTMR_AC_REG_BASE_OFFS + 0x8)
#define HWIO_MSS_QDSP6SS_QTMR_AC_CNTTIDR_FG0_RMSK                           0xffffffff
#define HWIO_MSS_QDSP6SS_QTMR_AC_CNTTIDR_FG0_IN                    \
                in_dword_masked(HWIO_MSS_QDSP6SS_QTMR_AC_CNTTIDR_FG0_ADDR, HWIO_MSS_QDSP6SS_QTMR_AC_CNTTIDR_FG0_RMSK)
#define HWIO_MSS_QDSP6SS_QTMR_AC_CNTTIDR_FG0_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_QTMR_AC_CNTTIDR_FG0_ADDR, m)
#define HWIO_MSS_QDSP6SS_QTMR_AC_CNTTIDR_FG0_F7_CFG_BMSK                    0xf0000000
#define HWIO_MSS_QDSP6SS_QTMR_AC_CNTTIDR_FG0_F7_CFG_SHFT                            28
#define HWIO_MSS_QDSP6SS_QTMR_AC_CNTTIDR_FG0_F6_CFG_BMSK                     0xf000000
#define HWIO_MSS_QDSP6SS_QTMR_AC_CNTTIDR_FG0_F6_CFG_SHFT                            24
#define HWIO_MSS_QDSP6SS_QTMR_AC_CNTTIDR_FG0_F5_CFG_BMSK                      0xf00000
#define HWIO_MSS_QDSP6SS_QTMR_AC_CNTTIDR_FG0_F5_CFG_SHFT                            20
#define HWIO_MSS_QDSP6SS_QTMR_AC_CNTTIDR_FG0_F4_CFG_BMSK                       0xf0000
#define HWIO_MSS_QDSP6SS_QTMR_AC_CNTTIDR_FG0_F4_CFG_SHFT                            16
#define HWIO_MSS_QDSP6SS_QTMR_AC_CNTTIDR_FG0_F3_CFG_BMSK                        0xf000
#define HWIO_MSS_QDSP6SS_QTMR_AC_CNTTIDR_FG0_F3_CFG_SHFT                            12
#define HWIO_MSS_QDSP6SS_QTMR_AC_CNTTIDR_FG0_F2_CFG_BMSK                         0xf00
#define HWIO_MSS_QDSP6SS_QTMR_AC_CNTTIDR_FG0_F2_CFG_SHFT                             8
#define HWIO_MSS_QDSP6SS_QTMR_AC_CNTTIDR_FG0_F1_CFG_BMSK                          0xf0
#define HWIO_MSS_QDSP6SS_QTMR_AC_CNTTIDR_FG0_F1_CFG_SHFT                             4
#define HWIO_MSS_QDSP6SS_QTMR_AC_CNTTIDR_FG0_F0_CFG_BMSK                           0xf
#define HWIO_MSS_QDSP6SS_QTMR_AC_CNTTIDR_FG0_F0_CFG_SHFT                             0

#define HWIO_MSS_QDSP6SS_QTMR_AC_CNTACRn_FG0_ADDR(n)                        (MSS_QDSP6SS_QDSP6SS_QTMR_AC_REG_BASE            + 0X40 + (0x4*(n)))
#define HWIO_MSS_QDSP6SS_QTMR_AC_CNTACRn_FG0_OFFS(n)                        (MSS_QDSP6SS_QDSP6SS_QTMR_AC_REG_BASE_OFFS + 0X40 + (0x4*(n)))
#define HWIO_MSS_QDSP6SS_QTMR_AC_CNTACRn_FG0_RMSK                                 0x3f
#define HWIO_MSS_QDSP6SS_QTMR_AC_CNTACRn_FG0_MAXn                                    2
#define HWIO_MSS_QDSP6SS_QTMR_AC_CNTACRn_FG0_INI(n)                \
                in_dword_masked(HWIO_MSS_QDSP6SS_QTMR_AC_CNTACRn_FG0_ADDR(n), HWIO_MSS_QDSP6SS_QTMR_AC_CNTACRn_FG0_RMSK)
#define HWIO_MSS_QDSP6SS_QTMR_AC_CNTACRn_FG0_INMI(n,mask)        \
                in_dword_masked(HWIO_MSS_QDSP6SS_QTMR_AC_CNTACRn_FG0_ADDR(n), mask)
#define HWIO_MSS_QDSP6SS_QTMR_AC_CNTACRn_FG0_OUTI(n,val)        \
                out_dword(HWIO_MSS_QDSP6SS_QTMR_AC_CNTACRn_FG0_ADDR(n),val)
#define HWIO_MSS_QDSP6SS_QTMR_AC_CNTACRn_FG0_OUTMI(n,mask,val) \
                out_dword_masked_ns(HWIO_MSS_QDSP6SS_QTMR_AC_CNTACRn_FG0_ADDR(n),mask,val,HWIO_MSS_QDSP6SS_QTMR_AC_CNTACRn_FG0_INI(n))
#define HWIO_MSS_QDSP6SS_QTMR_AC_CNTACRn_FG0_RWPT_BMSK                            0x20
#define HWIO_MSS_QDSP6SS_QTMR_AC_CNTACRn_FG0_RWPT_SHFT                               5
#define HWIO_MSS_QDSP6SS_QTMR_AC_CNTACRn_FG0_RWVT_BMSK                            0x10
#define HWIO_MSS_QDSP6SS_QTMR_AC_CNTACRn_FG0_RWVT_SHFT                               4
#define HWIO_MSS_QDSP6SS_QTMR_AC_CNTACRn_FG0_RVOFF_BMSK                            0x8
#define HWIO_MSS_QDSP6SS_QTMR_AC_CNTACRn_FG0_RVOFF_SHFT                              3
#define HWIO_MSS_QDSP6SS_QTMR_AC_CNTACRn_FG0_RFRQ_BMSK                             0x4
#define HWIO_MSS_QDSP6SS_QTMR_AC_CNTACRn_FG0_RFRQ_SHFT                               2
#define HWIO_MSS_QDSP6SS_QTMR_AC_CNTACRn_FG0_RPVCT_BMSK                            0x2
#define HWIO_MSS_QDSP6SS_QTMR_AC_CNTACRn_FG0_RPVCT_SHFT                              1
#define HWIO_MSS_QDSP6SS_QTMR_AC_CNTACRn_FG0_RPCT_BMSK                             0x1
#define HWIO_MSS_QDSP6SS_QTMR_AC_CNTACRn_FG0_RPCT_SHFT                               0

#define HWIO_MSS_QDSP6SS_QTMR_AC_CNTVOFF_FG0_LO_n_ADDR(n)                   (MSS_QDSP6SS_QDSP6SS_QTMR_AC_REG_BASE            + 0X80 + (0x8*(n)))
#define HWIO_MSS_QDSP6SS_QTMR_AC_CNTVOFF_FG0_LO_n_OFFS(n)                   (MSS_QDSP6SS_QDSP6SS_QTMR_AC_REG_BASE_OFFS + 0X80 + (0x8*(n)))
#define HWIO_MSS_QDSP6SS_QTMR_AC_CNTVOFF_FG0_LO_n_RMSK                      0xffffffff
#define HWIO_MSS_QDSP6SS_QTMR_AC_CNTVOFF_FG0_LO_n_MAXn                               2
#define HWIO_MSS_QDSP6SS_QTMR_AC_CNTVOFF_FG0_LO_n_INI(n)                \
                in_dword_masked(HWIO_MSS_QDSP6SS_QTMR_AC_CNTVOFF_FG0_LO_n_ADDR(n), HWIO_MSS_QDSP6SS_QTMR_AC_CNTVOFF_FG0_LO_n_RMSK)
#define HWIO_MSS_QDSP6SS_QTMR_AC_CNTVOFF_FG0_LO_n_INMI(n,mask)        \
                in_dword_masked(HWIO_MSS_QDSP6SS_QTMR_AC_CNTVOFF_FG0_LO_n_ADDR(n), mask)
#define HWIO_MSS_QDSP6SS_QTMR_AC_CNTVOFF_FG0_LO_n_OUTI(n,val)        \
                out_dword(HWIO_MSS_QDSP6SS_QTMR_AC_CNTVOFF_FG0_LO_n_ADDR(n),val)
#define HWIO_MSS_QDSP6SS_QTMR_AC_CNTVOFF_FG0_LO_n_OUTMI(n,mask,val) \
                out_dword_masked_ns(HWIO_MSS_QDSP6SS_QTMR_AC_CNTVOFF_FG0_LO_n_ADDR(n),mask,val,HWIO_MSS_QDSP6SS_QTMR_AC_CNTVOFF_FG0_LO_n_INI(n))
#define HWIO_MSS_QDSP6SS_QTMR_AC_CNTVOFF_FG0_LO_n_CNTVOFF_LO_BMSK           0xffffffff
#define HWIO_MSS_QDSP6SS_QTMR_AC_CNTVOFF_FG0_LO_n_CNTVOFF_LO_SHFT                    0

#define HWIO_MSS_QDSP6SS_QTMR_AC_CNTVOFF_FG0_HI_n_ADDR(n)                   (MSS_QDSP6SS_QDSP6SS_QTMR_AC_REG_BASE            + 0X84 + (0x8*(n)))
#define HWIO_MSS_QDSP6SS_QTMR_AC_CNTVOFF_FG0_HI_n_OFFS(n)                   (MSS_QDSP6SS_QDSP6SS_QTMR_AC_REG_BASE_OFFS + 0X84 + (0x8*(n)))
#define HWIO_MSS_QDSP6SS_QTMR_AC_CNTVOFF_FG0_HI_n_RMSK                        0xffffff
#define HWIO_MSS_QDSP6SS_QTMR_AC_CNTVOFF_FG0_HI_n_MAXn                               2
#define HWIO_MSS_QDSP6SS_QTMR_AC_CNTVOFF_FG0_HI_n_INI(n)                \
                in_dword_masked(HWIO_MSS_QDSP6SS_QTMR_AC_CNTVOFF_FG0_HI_n_ADDR(n), HWIO_MSS_QDSP6SS_QTMR_AC_CNTVOFF_FG0_HI_n_RMSK)
#define HWIO_MSS_QDSP6SS_QTMR_AC_CNTVOFF_FG0_HI_n_INMI(n,mask)        \
                in_dword_masked(HWIO_MSS_QDSP6SS_QTMR_AC_CNTVOFF_FG0_HI_n_ADDR(n), mask)
#define HWIO_MSS_QDSP6SS_QTMR_AC_CNTVOFF_FG0_HI_n_OUTI(n,val)        \
                out_dword(HWIO_MSS_QDSP6SS_QTMR_AC_CNTVOFF_FG0_HI_n_ADDR(n),val)
#define HWIO_MSS_QDSP6SS_QTMR_AC_CNTVOFF_FG0_HI_n_OUTMI(n,mask,val) \
                out_dword_masked_ns(HWIO_MSS_QDSP6SS_QTMR_AC_CNTVOFF_FG0_HI_n_ADDR(n),mask,val,HWIO_MSS_QDSP6SS_QTMR_AC_CNTVOFF_FG0_HI_n_INI(n))
#define HWIO_MSS_QDSP6SS_QTMR_AC_CNTVOFF_FG0_HI_n_CNTVOFF_HI_BMSK             0xffffff
#define HWIO_MSS_QDSP6SS_QTMR_AC_CNTVOFF_FG0_HI_n_CNTVOFF_HI_SHFT                    0

#define HWIO_MSS_QDSP6SS_QTMR_AC_CFG_ADDR                                   (MSS_QDSP6SS_QDSP6SS_QTMR_AC_REG_BASE            + 0xfc0)
#define HWIO_MSS_QDSP6SS_QTMR_AC_CFG_OFFS                                   (MSS_QDSP6SS_QDSP6SS_QTMR_AC_REG_BASE_OFFS + 0xfc0)
#define HWIO_MSS_QDSP6SS_QTMR_AC_CFG_RMSK                                          0x3
#define HWIO_MSS_QDSP6SS_QTMR_AC_CFG_IN                    \
                in_dword_masked(HWIO_MSS_QDSP6SS_QTMR_AC_CFG_ADDR, HWIO_MSS_QDSP6SS_QTMR_AC_CFG_RMSK)
#define HWIO_MSS_QDSP6SS_QTMR_AC_CFG_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_QTMR_AC_CFG_ADDR, m)
#define HWIO_MSS_QDSP6SS_QTMR_AC_CFG_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_QTMR_AC_CFG_ADDR,v)
#define HWIO_MSS_QDSP6SS_QTMR_AC_CFG_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_QDSP6SS_QTMR_AC_CFG_ADDR,m,v,HWIO_MSS_QDSP6SS_QTMR_AC_CFG_IN)
#define HWIO_MSS_QDSP6SS_QTMR_AC_CFG_DSBL_ATOMIC_BMSK                              0x2
#define HWIO_MSS_QDSP6SS_QTMR_AC_CFG_DSBL_ATOMIC_SHFT                                1
#define HWIO_MSS_QDSP6SS_QTMR_AC_CFG_TEST_BUS_EN_BMSK                              0x1
#define HWIO_MSS_QDSP6SS_QTMR_AC_CFG_TEST_BUS_EN_SHFT                                0

#define HWIO_MSS_QDSP6SS_QTMR_AC_VERSION_ADDR                               (MSS_QDSP6SS_QDSP6SS_QTMR_AC_REG_BASE            + 0xfd0)
#define HWIO_MSS_QDSP6SS_QTMR_AC_VERSION_OFFS                               (MSS_QDSP6SS_QDSP6SS_QTMR_AC_REG_BASE_OFFS + 0xfd0)
#define HWIO_MSS_QDSP6SS_QTMR_AC_VERSION_RMSK                               0xffffffff
#define HWIO_MSS_QDSP6SS_QTMR_AC_VERSION_IN                    \
                in_dword_masked(HWIO_MSS_QDSP6SS_QTMR_AC_VERSION_ADDR, HWIO_MSS_QDSP6SS_QTMR_AC_VERSION_RMSK)
#define HWIO_MSS_QDSP6SS_QTMR_AC_VERSION_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_QTMR_AC_VERSION_ADDR, m)
#define HWIO_MSS_QDSP6SS_QTMR_AC_VERSION_MAJOR_BMSK                         0xf0000000
#define HWIO_MSS_QDSP6SS_QTMR_AC_VERSION_MAJOR_SHFT                                 28
#define HWIO_MSS_QDSP6SS_QTMR_AC_VERSION_MINOR_BMSK                          0xfff0000
#define HWIO_MSS_QDSP6SS_QTMR_AC_VERSION_MINOR_SHFT                                 16
#define HWIO_MSS_QDSP6SS_QTMR_AC_VERSION_STEP_BMSK                              0xffff
#define HWIO_MSS_QDSP6SS_QTMR_AC_VERSION_STEP_SHFT                                   0

#define HWIO_MSS_QDSP6SS_QTMR_AC_HW_FRAME_SEL_1_ADDR                        (MSS_QDSP6SS_QDSP6SS_QTMR_AC_REG_BASE            + 0xfe0)
#define HWIO_MSS_QDSP6SS_QTMR_AC_HW_FRAME_SEL_1_OFFS                        (MSS_QDSP6SS_QDSP6SS_QTMR_AC_REG_BASE_OFFS + 0xfe0)
#define HWIO_MSS_QDSP6SS_QTMR_AC_HW_FRAME_SEL_1_RMSK                        0xffffffff
#define HWIO_MSS_QDSP6SS_QTMR_AC_HW_FRAME_SEL_1_IN                    \
                in_dword_masked(HWIO_MSS_QDSP6SS_QTMR_AC_HW_FRAME_SEL_1_ADDR, HWIO_MSS_QDSP6SS_QTMR_AC_HW_FRAME_SEL_1_RMSK)
#define HWIO_MSS_QDSP6SS_QTMR_AC_HW_FRAME_SEL_1_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_QTMR_AC_HW_FRAME_SEL_1_ADDR, m)
#define HWIO_MSS_QDSP6SS_QTMR_AC_HW_FRAME_SEL_1_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_QTMR_AC_HW_FRAME_SEL_1_ADDR,v)
#define HWIO_MSS_QDSP6SS_QTMR_AC_HW_FRAME_SEL_1_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_QDSP6SS_QTMR_AC_HW_FRAME_SEL_1_ADDR,m,v,HWIO_MSS_QDSP6SS_QTMR_AC_HW_FRAME_SEL_1_IN)
#define HWIO_MSS_QDSP6SS_QTMR_AC_HW_FRAME_SEL_1_HW_FRAME_SEL1_BMSK          0xffffffff
#define HWIO_MSS_QDSP6SS_QTMR_AC_HW_FRAME_SEL_1_HW_FRAME_SEL1_SHFT                   0

#define HWIO_MSS_QDSP6SS_QTMR_AC_HW_FRAME_SEL_2_ADDR                        (MSS_QDSP6SS_QDSP6SS_QTMR_AC_REG_BASE            + 0xff0)
#define HWIO_MSS_QDSP6SS_QTMR_AC_HW_FRAME_SEL_2_OFFS                        (MSS_QDSP6SS_QDSP6SS_QTMR_AC_REG_BASE_OFFS + 0xff0)
#define HWIO_MSS_QDSP6SS_QTMR_AC_HW_FRAME_SEL_2_RMSK                        0xffffffff
#define HWIO_MSS_QDSP6SS_QTMR_AC_HW_FRAME_SEL_2_IN                    \
                in_dword_masked(HWIO_MSS_QDSP6SS_QTMR_AC_HW_FRAME_SEL_2_ADDR, HWIO_MSS_QDSP6SS_QTMR_AC_HW_FRAME_SEL_2_RMSK)
#define HWIO_MSS_QDSP6SS_QTMR_AC_HW_FRAME_SEL_2_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_QTMR_AC_HW_FRAME_SEL_2_ADDR, m)
#define HWIO_MSS_QDSP6SS_QTMR_AC_HW_FRAME_SEL_2_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_QTMR_AC_HW_FRAME_SEL_2_ADDR,v)
#define HWIO_MSS_QDSP6SS_QTMR_AC_HW_FRAME_SEL_2_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_QDSP6SS_QTMR_AC_HW_FRAME_SEL_2_ADDR,m,v,HWIO_MSS_QDSP6SS_QTMR_AC_HW_FRAME_SEL_2_IN)
#define HWIO_MSS_QDSP6SS_QTMR_AC_HW_FRAME_SEL_2_HW_FRAME_SEL2_BMSK          0xffffffff
#define HWIO_MSS_QDSP6SS_QTMR_AC_HW_FRAME_SEL_2_HW_FRAME_SEL2_SHFT                   0

/*----------------------------------------------------------------------------
 * MODULE: MSS_QDSP6SS_QTMR_F0_0
 *--------------------------------------------------------------------------*/

#define MSS_QDSP6SS_QTMR_F0_0_REG_BASE                                     (MSS_TOP_BASE            + 0x00121000)
#define MSS_QDSP6SS_QTMR_F0_0_REG_BASE_SIZE                                0x1000
#define MSS_QDSP6SS_QTMR_F0_0_REG_BASE_USED                                0xfd0
#define MSS_QDSP6SS_QTMR_F0_0_REG_BASE_OFFS                                0x00121000

#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTPCT_LO_0_ADDR                          (MSS_QDSP6SS_QTMR_F0_0_REG_BASE            + 0x0)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTPCT_LO_0_OFFS                          (MSS_QDSP6SS_QTMR_F0_0_REG_BASE_OFFS + 0x0)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTPCT_LO_0_RMSK                          0xffffffff
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTPCT_LO_0_IN                    \
                in_dword_masked(HWIO_MSS_QDSP6SS_QTMR_V1_CNTPCT_LO_0_ADDR, HWIO_MSS_QDSP6SS_QTMR_V1_CNTPCT_LO_0_RMSK)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTPCT_LO_0_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_QTMR_V1_CNTPCT_LO_0_ADDR, m)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTPCT_LO_0_CNTPCT_LO_BMSK                0xffffffff
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTPCT_LO_0_CNTPCT_LO_SHFT                         0

#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTPCT_HI_0_ADDR                          (MSS_QDSP6SS_QTMR_F0_0_REG_BASE            + 0x4)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTPCT_HI_0_OFFS                          (MSS_QDSP6SS_QTMR_F0_0_REG_BASE_OFFS + 0x4)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTPCT_HI_0_RMSK                            0xffffff
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTPCT_HI_0_IN                    \
                in_dword_masked(HWIO_MSS_QDSP6SS_QTMR_V1_CNTPCT_HI_0_ADDR, HWIO_MSS_QDSP6SS_QTMR_V1_CNTPCT_HI_0_RMSK)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTPCT_HI_0_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_QTMR_V1_CNTPCT_HI_0_ADDR, m)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTPCT_HI_0_CNTPCT_HI_BMSK                  0xffffff
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTPCT_HI_0_CNTPCT_HI_SHFT                         0

#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTVCT_LO_0_ADDR                          (MSS_QDSP6SS_QTMR_F0_0_REG_BASE            + 0x8)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTVCT_LO_0_OFFS                          (MSS_QDSP6SS_QTMR_F0_0_REG_BASE_OFFS + 0x8)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTVCT_LO_0_RMSK                          0xffffffff
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTVCT_LO_0_IN                    \
                in_dword_masked(HWIO_MSS_QDSP6SS_QTMR_V1_CNTVCT_LO_0_ADDR, HWIO_MSS_QDSP6SS_QTMR_V1_CNTVCT_LO_0_RMSK)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTVCT_LO_0_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_QTMR_V1_CNTVCT_LO_0_ADDR, m)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTVCT_LO_0_CNTVCT_LO_BMSK                0xffffffff
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTVCT_LO_0_CNTVCT_LO_SHFT                         0

#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTVCT_HI_0_ADDR                          (MSS_QDSP6SS_QTMR_F0_0_REG_BASE            + 0xc)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTVCT_HI_0_OFFS                          (MSS_QDSP6SS_QTMR_F0_0_REG_BASE_OFFS + 0xc)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTVCT_HI_0_RMSK                            0xffffff
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTVCT_HI_0_IN                    \
                in_dword_masked(HWIO_MSS_QDSP6SS_QTMR_V1_CNTVCT_HI_0_ADDR, HWIO_MSS_QDSP6SS_QTMR_V1_CNTVCT_HI_0_RMSK)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTVCT_HI_0_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_QTMR_V1_CNTVCT_HI_0_ADDR, m)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTVCT_HI_0_CNTVCT_HI_BMSK                  0xffffff
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTVCT_HI_0_CNTVCT_HI_SHFT                         0

#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTFRQ_0_ADDR                             (MSS_QDSP6SS_QTMR_F0_0_REG_BASE            + 0x10)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTFRQ_0_OFFS                             (MSS_QDSP6SS_QTMR_F0_0_REG_BASE_OFFS + 0x10)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTFRQ_0_RMSK                             0xffffffff
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTFRQ_0_IN                    \
                in_dword_masked(HWIO_MSS_QDSP6SS_QTMR_V1_CNTFRQ_0_ADDR, HWIO_MSS_QDSP6SS_QTMR_V1_CNTFRQ_0_RMSK)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTFRQ_0_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_QTMR_V1_CNTFRQ_0_ADDR, m)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTFRQ_0_CNTFRQ_BMSK                      0xffffffff
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTFRQ_0_CNTFRQ_SHFT                               0

#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTPL0ACR_0_ADDR                          (MSS_QDSP6SS_QTMR_F0_0_REG_BASE            + 0x14)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTPL0ACR_0_OFFS                          (MSS_QDSP6SS_QTMR_F0_0_REG_BASE_OFFS + 0x14)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTPL0ACR_0_RMSK                               0x303
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTPL0ACR_0_IN                    \
                in_dword_masked(HWIO_MSS_QDSP6SS_QTMR_V1_CNTPL0ACR_0_ADDR, HWIO_MSS_QDSP6SS_QTMR_V1_CNTPL0ACR_0_RMSK)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTPL0ACR_0_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_QTMR_V1_CNTPL0ACR_0_ADDR, m)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTPL0ACR_0_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_QTMR_V1_CNTPL0ACR_0_ADDR,v)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTPL0ACR_0_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_QDSP6SS_QTMR_V1_CNTPL0ACR_0_ADDR,m,v,HWIO_MSS_QDSP6SS_QTMR_V1_CNTPL0ACR_0_IN)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTPL0ACR_0_PL0CTEN_BMSK                       0x200
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTPL0ACR_0_PL0CTEN_SHFT                           9
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTPL0ACR_0_PL0VTEN_BMSK                       0x100
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTPL0ACR_0_PL0VTEN_SHFT                           8
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTPL0ACR_0_PL0VCTEN_BMSK                        0x2
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTPL0ACR_0_PL0VCTEN_SHFT                          1
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTPL0ACR_0_PL0PCTEN_BMSK                        0x1
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTPL0ACR_0_PL0PCTEN_SHFT                          0

#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTVOFF_LO_0_ADDR                         (MSS_QDSP6SS_QTMR_F0_0_REG_BASE            + 0x18)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTVOFF_LO_0_OFFS                         (MSS_QDSP6SS_QTMR_F0_0_REG_BASE_OFFS + 0x18)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTVOFF_LO_0_RMSK                         0xffffffff
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTVOFF_LO_0_IN                    \
                in_dword_masked(HWIO_MSS_QDSP6SS_QTMR_V1_CNTVOFF_LO_0_ADDR, HWIO_MSS_QDSP6SS_QTMR_V1_CNTVOFF_LO_0_RMSK)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTVOFF_LO_0_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_QTMR_V1_CNTVOFF_LO_0_ADDR, m)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTVOFF_LO_0_CNTVOFF_L0_BMSK              0xffffffff
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTVOFF_LO_0_CNTVOFF_L0_SHFT                       0

#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTVOFF_HI_0_ADDR                         (MSS_QDSP6SS_QTMR_F0_0_REG_BASE            + 0x1c)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTVOFF_HI_0_OFFS                         (MSS_QDSP6SS_QTMR_F0_0_REG_BASE_OFFS + 0x1c)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTVOFF_HI_0_RMSK                           0xffffff
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTVOFF_HI_0_IN                    \
                in_dword_masked(HWIO_MSS_QDSP6SS_QTMR_V1_CNTVOFF_HI_0_ADDR, HWIO_MSS_QDSP6SS_QTMR_V1_CNTVOFF_HI_0_RMSK)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTVOFF_HI_0_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_QTMR_V1_CNTVOFF_HI_0_ADDR, m)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTVOFF_HI_0_CNTVOFF_HI_BMSK                0xffffff
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTVOFF_HI_0_CNTVOFF_HI_SHFT                       0

#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_CVAL_LO_0_ADDR                       (MSS_QDSP6SS_QTMR_F0_0_REG_BASE            + 0x20)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_CVAL_LO_0_OFFS                       (MSS_QDSP6SS_QTMR_F0_0_REG_BASE_OFFS + 0x20)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_CVAL_LO_0_RMSK                       0xffffffff
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_CVAL_LO_0_IN                    \
                in_dword_masked(HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_CVAL_LO_0_ADDR, HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_CVAL_LO_0_RMSK)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_CVAL_LO_0_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_CVAL_LO_0_ADDR, m)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_CVAL_LO_0_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_CVAL_LO_0_ADDR,v)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_CVAL_LO_0_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_CVAL_LO_0_ADDR,m,v,HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_CVAL_LO_0_IN)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_CVAL_LO_0_CNTP_CVAL_L0_BMSK          0xffffffff
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_CVAL_LO_0_CNTP_CVAL_L0_SHFT                   0

#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_CVAL_HI_0_ADDR                       (MSS_QDSP6SS_QTMR_F0_0_REG_BASE            + 0x24)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_CVAL_HI_0_OFFS                       (MSS_QDSP6SS_QTMR_F0_0_REG_BASE_OFFS + 0x24)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_CVAL_HI_0_RMSK                         0xffffff
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_CVAL_HI_0_IN                    \
                in_dword_masked(HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_CVAL_HI_0_ADDR, HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_CVAL_HI_0_RMSK)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_CVAL_HI_0_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_CVAL_HI_0_ADDR, m)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_CVAL_HI_0_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_CVAL_HI_0_ADDR,v)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_CVAL_HI_0_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_CVAL_HI_0_ADDR,m,v,HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_CVAL_HI_0_IN)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_CVAL_HI_0_CNTP_CVAL_HI_BMSK            0xffffff
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_CVAL_HI_0_CNTP_CVAL_HI_SHFT                   0

#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_TVAL_0_ADDR                          (MSS_QDSP6SS_QTMR_F0_0_REG_BASE            + 0x28)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_TVAL_0_OFFS                          (MSS_QDSP6SS_QTMR_F0_0_REG_BASE_OFFS + 0x28)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_TVAL_0_RMSK                          0xffffffff
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_TVAL_0_IN                    \
                in_dword_masked(HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_TVAL_0_ADDR, HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_TVAL_0_RMSK)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_TVAL_0_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_TVAL_0_ADDR, m)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_TVAL_0_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_TVAL_0_ADDR,v)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_TVAL_0_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_TVAL_0_ADDR,m,v,HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_TVAL_0_IN)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_TVAL_0_CNTP_TVAL_BMSK                0xffffffff
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_TVAL_0_CNTP_TVAL_SHFT                         0

#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_CTL_0_ADDR                           (MSS_QDSP6SS_QTMR_F0_0_REG_BASE            + 0x2c)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_CTL_0_OFFS                           (MSS_QDSP6SS_QTMR_F0_0_REG_BASE_OFFS + 0x2c)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_CTL_0_RMSK                                  0x7
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_CTL_0_IN                    \
                in_dword_masked(HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_CTL_0_ADDR, HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_CTL_0_RMSK)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_CTL_0_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_CTL_0_ADDR, m)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_CTL_0_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_CTL_0_ADDR,v)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_CTL_0_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_CTL_0_ADDR,m,v,HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_CTL_0_IN)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_CTL_0_ISTAT_BMSK                            0x4
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_CTL_0_ISTAT_SHFT                              2
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_CTL_0_IMSK_BMSK                             0x2
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_CTL_0_IMSK_SHFT                               1
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_CTL_0_EN_BMSK                               0x1
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_CTL_0_EN_SHFT                                 0

#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_CVAL_LO_0_ADDR                       (MSS_QDSP6SS_QTMR_F0_0_REG_BASE            + 0x30)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_CVAL_LO_0_OFFS                       (MSS_QDSP6SS_QTMR_F0_0_REG_BASE_OFFS + 0x30)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_CVAL_LO_0_RMSK                       0xffffffff
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_CVAL_LO_0_IN                    \
                in_dword_masked(HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_CVAL_LO_0_ADDR, HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_CVAL_LO_0_RMSK)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_CVAL_LO_0_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_CVAL_LO_0_ADDR, m)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_CVAL_LO_0_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_CVAL_LO_0_ADDR,v)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_CVAL_LO_0_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_CVAL_LO_0_ADDR,m,v,HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_CVAL_LO_0_IN)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_CVAL_LO_0_CNTV_CVAL_L0_BMSK          0xffffffff
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_CVAL_LO_0_CNTV_CVAL_L0_SHFT                   0

#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_CVAL_HI_0_ADDR                       (MSS_QDSP6SS_QTMR_F0_0_REG_BASE            + 0x34)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_CVAL_HI_0_OFFS                       (MSS_QDSP6SS_QTMR_F0_0_REG_BASE_OFFS + 0x34)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_CVAL_HI_0_RMSK                         0xffffff
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_CVAL_HI_0_IN                    \
                in_dword_masked(HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_CVAL_HI_0_ADDR, HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_CVAL_HI_0_RMSK)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_CVAL_HI_0_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_CVAL_HI_0_ADDR, m)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_CVAL_HI_0_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_CVAL_HI_0_ADDR,v)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_CVAL_HI_0_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_CVAL_HI_0_ADDR,m,v,HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_CVAL_HI_0_IN)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_CVAL_HI_0_CNTV_CVAL_HI_BMSK            0xffffff
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_CVAL_HI_0_CNTV_CVAL_HI_SHFT                   0

#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_TVAL_0_ADDR                          (MSS_QDSP6SS_QTMR_F0_0_REG_BASE            + 0x38)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_TVAL_0_OFFS                          (MSS_QDSP6SS_QTMR_F0_0_REG_BASE_OFFS + 0x38)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_TVAL_0_RMSK                          0xffffffff
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_TVAL_0_IN                    \
                in_dword_masked(HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_TVAL_0_ADDR, HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_TVAL_0_RMSK)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_TVAL_0_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_TVAL_0_ADDR, m)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_TVAL_0_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_TVAL_0_ADDR,v)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_TVAL_0_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_TVAL_0_ADDR,m,v,HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_TVAL_0_IN)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_TVAL_0_CNTV_TVAL_BMSK                0xffffffff
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_TVAL_0_CNTV_TVAL_SHFT                         0

#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_CTL_0_ADDR                           (MSS_QDSP6SS_QTMR_F0_0_REG_BASE            + 0x3c)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_CTL_0_OFFS                           (MSS_QDSP6SS_QTMR_F0_0_REG_BASE_OFFS + 0x3c)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_CTL_0_RMSK                                  0x7
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_CTL_0_IN                    \
                in_dword_masked(HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_CTL_0_ADDR, HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_CTL_0_RMSK)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_CTL_0_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_CTL_0_ADDR, m)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_CTL_0_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_CTL_0_ADDR,v)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_CTL_0_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_CTL_0_ADDR,m,v,HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_CTL_0_IN)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_CTL_0_ISTAT_BMSK                            0x4
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_CTL_0_ISTAT_SHFT                              2
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_CTL_0_IMSK_BMSK                             0x2
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_CTL_0_IMSK_SHFT                               1
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_CTL_0_EN_BMSK                               0x1
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_CTL_0_EN_SHFT                                 0

#define HWIO_MSS_QDSP6SS_QTMR_V1_VERSION_0_ADDR                            (MSS_QDSP6SS_QTMR_F0_0_REG_BASE            + 0xfd0)
#define HWIO_MSS_QDSP6SS_QTMR_V1_VERSION_0_OFFS                            (MSS_QDSP6SS_QTMR_F0_0_REG_BASE_OFFS + 0xfd0)
#define HWIO_MSS_QDSP6SS_QTMR_V1_VERSION_0_RMSK                            0xffffffff
#define HWIO_MSS_QDSP6SS_QTMR_V1_VERSION_0_IN                    \
                in_dword_masked(HWIO_MSS_QDSP6SS_QTMR_V1_VERSION_0_ADDR, HWIO_MSS_QDSP6SS_QTMR_V1_VERSION_0_RMSK)
#define HWIO_MSS_QDSP6SS_QTMR_V1_VERSION_0_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_QTMR_V1_VERSION_0_ADDR, m)
#define HWIO_MSS_QDSP6SS_QTMR_V1_VERSION_0_MAJOR_BMSK                      0xf0000000
#define HWIO_MSS_QDSP6SS_QTMR_V1_VERSION_0_MAJOR_SHFT                              28
#define HWIO_MSS_QDSP6SS_QTMR_V1_VERSION_0_MINOR_BMSK                       0xfff0000
#define HWIO_MSS_QDSP6SS_QTMR_V1_VERSION_0_MINOR_SHFT                              16
#define HWIO_MSS_QDSP6SS_QTMR_V1_VERSION_0_STEP_BMSK                           0xffff
#define HWIO_MSS_QDSP6SS_QTMR_V1_VERSION_0_STEP_SHFT                                0

/*----------------------------------------------------------------------------
 * MODULE: MSS_QDSP6SS_QTMR_F1_1
 *--------------------------------------------------------------------------*/

#define MSS_QDSP6SS_QTMR_F1_1_REG_BASE                                     (MSS_TOP_BASE            + 0x00122000)
#define MSS_QDSP6SS_QTMR_F1_1_REG_BASE_SIZE                                0x1000
#define MSS_QDSP6SS_QTMR_F1_1_REG_BASE_USED                                0xfd0
#define MSS_QDSP6SS_QTMR_F1_1_REG_BASE_OFFS                                0x00122000

#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTPCT_LO_1_ADDR                          (MSS_QDSP6SS_QTMR_F1_1_REG_BASE            + 0x0)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTPCT_LO_1_OFFS                          (MSS_QDSP6SS_QTMR_F1_1_REG_BASE_OFFS + 0x0)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTPCT_LO_1_RMSK                          0xffffffff
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTPCT_LO_1_IN                    \
                in_dword_masked(HWIO_MSS_QDSP6SS_QTMR_V1_CNTPCT_LO_1_ADDR, HWIO_MSS_QDSP6SS_QTMR_V1_CNTPCT_LO_1_RMSK)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTPCT_LO_1_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_QTMR_V1_CNTPCT_LO_1_ADDR, m)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTPCT_LO_1_CNTPCT_LO_BMSK                0xffffffff
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTPCT_LO_1_CNTPCT_LO_SHFT                         0

#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTPCT_HI_1_ADDR                          (MSS_QDSP6SS_QTMR_F1_1_REG_BASE            + 0x4)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTPCT_HI_1_OFFS                          (MSS_QDSP6SS_QTMR_F1_1_REG_BASE_OFFS + 0x4)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTPCT_HI_1_RMSK                            0xffffff
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTPCT_HI_1_IN                    \
                in_dword_masked(HWIO_MSS_QDSP6SS_QTMR_V1_CNTPCT_HI_1_ADDR, HWIO_MSS_QDSP6SS_QTMR_V1_CNTPCT_HI_1_RMSK)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTPCT_HI_1_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_QTMR_V1_CNTPCT_HI_1_ADDR, m)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTPCT_HI_1_CNTPCT_HI_BMSK                  0xffffff
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTPCT_HI_1_CNTPCT_HI_SHFT                         0

#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTVCT_LO_1_ADDR                          (MSS_QDSP6SS_QTMR_F1_1_REG_BASE            + 0x8)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTVCT_LO_1_OFFS                          (MSS_QDSP6SS_QTMR_F1_1_REG_BASE_OFFS + 0x8)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTVCT_LO_1_RMSK                          0xffffffff
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTVCT_LO_1_IN                    \
                in_dword_masked(HWIO_MSS_QDSP6SS_QTMR_V1_CNTVCT_LO_1_ADDR, HWIO_MSS_QDSP6SS_QTMR_V1_CNTVCT_LO_1_RMSK)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTVCT_LO_1_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_QTMR_V1_CNTVCT_LO_1_ADDR, m)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTVCT_LO_1_CNTVCT_LO_BMSK                0xffffffff
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTVCT_LO_1_CNTVCT_LO_SHFT                         0

#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTVCT_HI_1_ADDR                          (MSS_QDSP6SS_QTMR_F1_1_REG_BASE            + 0xc)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTVCT_HI_1_OFFS                          (MSS_QDSP6SS_QTMR_F1_1_REG_BASE_OFFS + 0xc)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTVCT_HI_1_RMSK                            0xffffff
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTVCT_HI_1_IN                    \
                in_dword_masked(HWIO_MSS_QDSP6SS_QTMR_V1_CNTVCT_HI_1_ADDR, HWIO_MSS_QDSP6SS_QTMR_V1_CNTVCT_HI_1_RMSK)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTVCT_HI_1_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_QTMR_V1_CNTVCT_HI_1_ADDR, m)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTVCT_HI_1_CNTVCT_HI_BMSK                  0xffffff
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTVCT_HI_1_CNTVCT_HI_SHFT                         0

#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTFRQ_1_ADDR                             (MSS_QDSP6SS_QTMR_F1_1_REG_BASE            + 0x10)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTFRQ_1_OFFS                             (MSS_QDSP6SS_QTMR_F1_1_REG_BASE_OFFS + 0x10)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTFRQ_1_RMSK                             0xffffffff
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTFRQ_1_IN                    \
                in_dword_masked(HWIO_MSS_QDSP6SS_QTMR_V1_CNTFRQ_1_ADDR, HWIO_MSS_QDSP6SS_QTMR_V1_CNTFRQ_1_RMSK)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTFRQ_1_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_QTMR_V1_CNTFRQ_1_ADDR, m)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTFRQ_1_CNTFRQ_BMSK                      0xffffffff
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTFRQ_1_CNTFRQ_SHFT                               0

#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTPL0ACR_1_ADDR                          (MSS_QDSP6SS_QTMR_F1_1_REG_BASE            + 0x14)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTPL0ACR_1_OFFS                          (MSS_QDSP6SS_QTMR_F1_1_REG_BASE_OFFS + 0x14)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTPL0ACR_1_RMSK                               0x303
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTPL0ACR_1_IN                    \
                in_dword_masked(HWIO_MSS_QDSP6SS_QTMR_V1_CNTPL0ACR_1_ADDR, HWIO_MSS_QDSP6SS_QTMR_V1_CNTPL0ACR_1_RMSK)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTPL0ACR_1_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_QTMR_V1_CNTPL0ACR_1_ADDR, m)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTPL0ACR_1_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_QTMR_V1_CNTPL0ACR_1_ADDR,v)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTPL0ACR_1_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_QDSP6SS_QTMR_V1_CNTPL0ACR_1_ADDR,m,v,HWIO_MSS_QDSP6SS_QTMR_V1_CNTPL0ACR_1_IN)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTPL0ACR_1_PL0CTEN_BMSK                       0x200
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTPL0ACR_1_PL0CTEN_SHFT                           9
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTPL0ACR_1_PL0VTEN_BMSK                       0x100
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTPL0ACR_1_PL0VTEN_SHFT                           8
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTPL0ACR_1_PL0VCTEN_BMSK                        0x2
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTPL0ACR_1_PL0VCTEN_SHFT                          1
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTPL0ACR_1_PL0PCTEN_BMSK                        0x1
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTPL0ACR_1_PL0PCTEN_SHFT                          0

#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTVOFF_LO_1_ADDR                         (MSS_QDSP6SS_QTMR_F1_1_REG_BASE            + 0x18)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTVOFF_LO_1_OFFS                         (MSS_QDSP6SS_QTMR_F1_1_REG_BASE_OFFS + 0x18)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTVOFF_LO_1_RMSK                         0xffffffff
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTVOFF_LO_1_IN                    \
                in_dword_masked(HWIO_MSS_QDSP6SS_QTMR_V1_CNTVOFF_LO_1_ADDR, HWIO_MSS_QDSP6SS_QTMR_V1_CNTVOFF_LO_1_RMSK)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTVOFF_LO_1_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_QTMR_V1_CNTVOFF_LO_1_ADDR, m)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTVOFF_LO_1_CNTVOFF_L0_BMSK              0xffffffff
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTVOFF_LO_1_CNTVOFF_L0_SHFT                       0

#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTVOFF_HI_1_ADDR                         (MSS_QDSP6SS_QTMR_F1_1_REG_BASE            + 0x1c)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTVOFF_HI_1_OFFS                         (MSS_QDSP6SS_QTMR_F1_1_REG_BASE_OFFS + 0x1c)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTVOFF_HI_1_RMSK                           0xffffff
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTVOFF_HI_1_IN                    \
                in_dword_masked(HWIO_MSS_QDSP6SS_QTMR_V1_CNTVOFF_HI_1_ADDR, HWIO_MSS_QDSP6SS_QTMR_V1_CNTVOFF_HI_1_RMSK)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTVOFF_HI_1_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_QTMR_V1_CNTVOFF_HI_1_ADDR, m)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTVOFF_HI_1_CNTVOFF_HI_BMSK                0xffffff
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTVOFF_HI_1_CNTVOFF_HI_SHFT                       0

#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_CVAL_LO_1_ADDR                       (MSS_QDSP6SS_QTMR_F1_1_REG_BASE            + 0x20)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_CVAL_LO_1_OFFS                       (MSS_QDSP6SS_QTMR_F1_1_REG_BASE_OFFS + 0x20)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_CVAL_LO_1_RMSK                       0xffffffff
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_CVAL_LO_1_IN                    \
                in_dword_masked(HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_CVAL_LO_1_ADDR, HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_CVAL_LO_1_RMSK)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_CVAL_LO_1_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_CVAL_LO_1_ADDR, m)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_CVAL_LO_1_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_CVAL_LO_1_ADDR,v)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_CVAL_LO_1_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_CVAL_LO_1_ADDR,m,v,HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_CVAL_LO_1_IN)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_CVAL_LO_1_CNTP_CVAL_L0_BMSK          0xffffffff
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_CVAL_LO_1_CNTP_CVAL_L0_SHFT                   0

#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_CVAL_HI_1_ADDR                       (MSS_QDSP6SS_QTMR_F1_1_REG_BASE            + 0x24)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_CVAL_HI_1_OFFS                       (MSS_QDSP6SS_QTMR_F1_1_REG_BASE_OFFS + 0x24)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_CVAL_HI_1_RMSK                         0xffffff
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_CVAL_HI_1_IN                    \
                in_dword_masked(HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_CVAL_HI_1_ADDR, HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_CVAL_HI_1_RMSK)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_CVAL_HI_1_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_CVAL_HI_1_ADDR, m)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_CVAL_HI_1_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_CVAL_HI_1_ADDR,v)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_CVAL_HI_1_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_CVAL_HI_1_ADDR,m,v,HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_CVAL_HI_1_IN)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_CVAL_HI_1_CNTP_CVAL_HI_BMSK            0xffffff
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_CVAL_HI_1_CNTP_CVAL_HI_SHFT                   0

#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_TVAL_1_ADDR                          (MSS_QDSP6SS_QTMR_F1_1_REG_BASE            + 0x28)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_TVAL_1_OFFS                          (MSS_QDSP6SS_QTMR_F1_1_REG_BASE_OFFS + 0x28)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_TVAL_1_RMSK                          0xffffffff
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_TVAL_1_IN                    \
                in_dword_masked(HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_TVAL_1_ADDR, HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_TVAL_1_RMSK)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_TVAL_1_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_TVAL_1_ADDR, m)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_TVAL_1_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_TVAL_1_ADDR,v)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_TVAL_1_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_TVAL_1_ADDR,m,v,HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_TVAL_1_IN)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_TVAL_1_CNTP_TVAL_BMSK                0xffffffff
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_TVAL_1_CNTP_TVAL_SHFT                         0

#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_CTL_1_ADDR                           (MSS_QDSP6SS_QTMR_F1_1_REG_BASE            + 0x2c)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_CTL_1_OFFS                           (MSS_QDSP6SS_QTMR_F1_1_REG_BASE_OFFS + 0x2c)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_CTL_1_RMSK                                  0x7
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_CTL_1_IN                    \
                in_dword_masked(HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_CTL_1_ADDR, HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_CTL_1_RMSK)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_CTL_1_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_CTL_1_ADDR, m)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_CTL_1_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_CTL_1_ADDR,v)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_CTL_1_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_CTL_1_ADDR,m,v,HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_CTL_1_IN)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_CTL_1_ISTAT_BMSK                            0x4
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_CTL_1_ISTAT_SHFT                              2
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_CTL_1_IMSK_BMSK                             0x2
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_CTL_1_IMSK_SHFT                               1
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_CTL_1_EN_BMSK                               0x1
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_CTL_1_EN_SHFT                                 0

#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_CVAL_LO_1_ADDR                       (MSS_QDSP6SS_QTMR_F1_1_REG_BASE            + 0x30)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_CVAL_LO_1_OFFS                       (MSS_QDSP6SS_QTMR_F1_1_REG_BASE_OFFS + 0x30)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_CVAL_LO_1_RMSK                       0xffffffff
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_CVAL_LO_1_IN                    \
                in_dword_masked(HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_CVAL_LO_1_ADDR, HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_CVAL_LO_1_RMSK)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_CVAL_LO_1_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_CVAL_LO_1_ADDR, m)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_CVAL_LO_1_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_CVAL_LO_1_ADDR,v)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_CVAL_LO_1_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_CVAL_LO_1_ADDR,m,v,HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_CVAL_LO_1_IN)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_CVAL_LO_1_CNTV_CVAL_L0_BMSK          0xffffffff
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_CVAL_LO_1_CNTV_CVAL_L0_SHFT                   0

#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_CVAL_HI_1_ADDR                       (MSS_QDSP6SS_QTMR_F1_1_REG_BASE            + 0x34)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_CVAL_HI_1_OFFS                       (MSS_QDSP6SS_QTMR_F1_1_REG_BASE_OFFS + 0x34)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_CVAL_HI_1_RMSK                         0xffffff
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_CVAL_HI_1_IN                    \
                in_dword_masked(HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_CVAL_HI_1_ADDR, HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_CVAL_HI_1_RMSK)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_CVAL_HI_1_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_CVAL_HI_1_ADDR, m)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_CVAL_HI_1_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_CVAL_HI_1_ADDR,v)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_CVAL_HI_1_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_CVAL_HI_1_ADDR,m,v,HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_CVAL_HI_1_IN)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_CVAL_HI_1_CNTV_CVAL_HI_BMSK            0xffffff
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_CVAL_HI_1_CNTV_CVAL_HI_SHFT                   0

#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_TVAL_1_ADDR                          (MSS_QDSP6SS_QTMR_F1_1_REG_BASE            + 0x38)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_TVAL_1_OFFS                          (MSS_QDSP6SS_QTMR_F1_1_REG_BASE_OFFS + 0x38)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_TVAL_1_RMSK                          0xffffffff
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_TVAL_1_IN                    \
                in_dword_masked(HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_TVAL_1_ADDR, HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_TVAL_1_RMSK)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_TVAL_1_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_TVAL_1_ADDR, m)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_TVAL_1_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_TVAL_1_ADDR,v)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_TVAL_1_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_TVAL_1_ADDR,m,v,HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_TVAL_1_IN)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_TVAL_1_CNTV_TVAL_BMSK                0xffffffff
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_TVAL_1_CNTV_TVAL_SHFT                         0

#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_CTL_1_ADDR                           (MSS_QDSP6SS_QTMR_F1_1_REG_BASE            + 0x3c)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_CTL_1_OFFS                           (MSS_QDSP6SS_QTMR_F1_1_REG_BASE_OFFS + 0x3c)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_CTL_1_RMSK                                  0x7
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_CTL_1_IN                    \
                in_dword_masked(HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_CTL_1_ADDR, HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_CTL_1_RMSK)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_CTL_1_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_CTL_1_ADDR, m)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_CTL_1_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_CTL_1_ADDR,v)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_CTL_1_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_CTL_1_ADDR,m,v,HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_CTL_1_IN)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_CTL_1_ISTAT_BMSK                            0x4
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_CTL_1_ISTAT_SHFT                              2
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_CTL_1_IMSK_BMSK                             0x2
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_CTL_1_IMSK_SHFT                               1
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_CTL_1_EN_BMSK                               0x1
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_CTL_1_EN_SHFT                                 0

#define HWIO_MSS_QDSP6SS_QTMR_V1_VERSION_1_ADDR                            (MSS_QDSP6SS_QTMR_F1_1_REG_BASE            + 0xfd0)
#define HWIO_MSS_QDSP6SS_QTMR_V1_VERSION_1_OFFS                            (MSS_QDSP6SS_QTMR_F1_1_REG_BASE_OFFS + 0xfd0)
#define HWIO_MSS_QDSP6SS_QTMR_V1_VERSION_1_RMSK                            0xffffffff
#define HWIO_MSS_QDSP6SS_QTMR_V1_VERSION_1_IN                    \
                in_dword_masked(HWIO_MSS_QDSP6SS_QTMR_V1_VERSION_1_ADDR, HWIO_MSS_QDSP6SS_QTMR_V1_VERSION_1_RMSK)
#define HWIO_MSS_QDSP6SS_QTMR_V1_VERSION_1_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_QTMR_V1_VERSION_1_ADDR, m)
#define HWIO_MSS_QDSP6SS_QTMR_V1_VERSION_1_MAJOR_BMSK                      0xf0000000
#define HWIO_MSS_QDSP6SS_QTMR_V1_VERSION_1_MAJOR_SHFT                              28
#define HWIO_MSS_QDSP6SS_QTMR_V1_VERSION_1_MINOR_BMSK                       0xfff0000
#define HWIO_MSS_QDSP6SS_QTMR_V1_VERSION_1_MINOR_SHFT                              16
#define HWIO_MSS_QDSP6SS_QTMR_V1_VERSION_1_STEP_BMSK                           0xffff
#define HWIO_MSS_QDSP6SS_QTMR_V1_VERSION_1_STEP_SHFT                                0

/*----------------------------------------------------------------------------
 * MODULE: MSS_QDSP6SS_QTMR_F2_2
 *--------------------------------------------------------------------------*/

#define MSS_QDSP6SS_QTMR_F2_2_REG_BASE                                     (MSS_TOP_BASE            + 0x00123000)
#define MSS_QDSP6SS_QTMR_F2_2_REG_BASE_SIZE                                0x1000
#define MSS_QDSP6SS_QTMR_F2_2_REG_BASE_USED                                0xfd0
#define MSS_QDSP6SS_QTMR_F2_2_REG_BASE_OFFS                                0x00123000

#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTPCT_LO_2_ADDR                          (MSS_QDSP6SS_QTMR_F2_2_REG_BASE            + 0x0)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTPCT_LO_2_OFFS                          (MSS_QDSP6SS_QTMR_F2_2_REG_BASE_OFFS + 0x0)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTPCT_LO_2_RMSK                          0xffffffff
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTPCT_LO_2_IN                    \
                in_dword_masked(HWIO_MSS_QDSP6SS_QTMR_V1_CNTPCT_LO_2_ADDR, HWIO_MSS_QDSP6SS_QTMR_V1_CNTPCT_LO_2_RMSK)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTPCT_LO_2_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_QTMR_V1_CNTPCT_LO_2_ADDR, m)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTPCT_LO_2_CNTPCT_LO_BMSK                0xffffffff
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTPCT_LO_2_CNTPCT_LO_SHFT                         0

#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTPCT_HI_2_ADDR                          (MSS_QDSP6SS_QTMR_F2_2_REG_BASE            + 0x4)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTPCT_HI_2_OFFS                          (MSS_QDSP6SS_QTMR_F2_2_REG_BASE_OFFS + 0x4)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTPCT_HI_2_RMSK                            0xffffff
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTPCT_HI_2_IN                    \
                in_dword_masked(HWIO_MSS_QDSP6SS_QTMR_V1_CNTPCT_HI_2_ADDR, HWIO_MSS_QDSP6SS_QTMR_V1_CNTPCT_HI_2_RMSK)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTPCT_HI_2_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_QTMR_V1_CNTPCT_HI_2_ADDR, m)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTPCT_HI_2_CNTPCT_HI_BMSK                  0xffffff
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTPCT_HI_2_CNTPCT_HI_SHFT                         0

#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTVCT_LO_2_ADDR                          (MSS_QDSP6SS_QTMR_F2_2_REG_BASE            + 0x8)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTVCT_LO_2_OFFS                          (MSS_QDSP6SS_QTMR_F2_2_REG_BASE_OFFS + 0x8)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTVCT_LO_2_RMSK                          0xffffffff
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTVCT_LO_2_IN                    \
                in_dword_masked(HWIO_MSS_QDSP6SS_QTMR_V1_CNTVCT_LO_2_ADDR, HWIO_MSS_QDSP6SS_QTMR_V1_CNTVCT_LO_2_RMSK)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTVCT_LO_2_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_QTMR_V1_CNTVCT_LO_2_ADDR, m)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTVCT_LO_2_CNTVCT_LO_BMSK                0xffffffff
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTVCT_LO_2_CNTVCT_LO_SHFT                         0

#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTVCT_HI_2_ADDR                          (MSS_QDSP6SS_QTMR_F2_2_REG_BASE            + 0xc)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTVCT_HI_2_OFFS                          (MSS_QDSP6SS_QTMR_F2_2_REG_BASE_OFFS + 0xc)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTVCT_HI_2_RMSK                            0xffffff
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTVCT_HI_2_IN                    \
                in_dword_masked(HWIO_MSS_QDSP6SS_QTMR_V1_CNTVCT_HI_2_ADDR, HWIO_MSS_QDSP6SS_QTMR_V1_CNTVCT_HI_2_RMSK)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTVCT_HI_2_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_QTMR_V1_CNTVCT_HI_2_ADDR, m)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTVCT_HI_2_CNTVCT_HI_BMSK                  0xffffff
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTVCT_HI_2_CNTVCT_HI_SHFT                         0

#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTFRQ_2_ADDR                             (MSS_QDSP6SS_QTMR_F2_2_REG_BASE            + 0x10)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTFRQ_2_OFFS                             (MSS_QDSP6SS_QTMR_F2_2_REG_BASE_OFFS + 0x10)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTFRQ_2_RMSK                             0xffffffff
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTFRQ_2_IN                    \
                in_dword_masked(HWIO_MSS_QDSP6SS_QTMR_V1_CNTFRQ_2_ADDR, HWIO_MSS_QDSP6SS_QTMR_V1_CNTFRQ_2_RMSK)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTFRQ_2_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_QTMR_V1_CNTFRQ_2_ADDR, m)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTFRQ_2_CNTFRQ_BMSK                      0xffffffff
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTFRQ_2_CNTFRQ_SHFT                               0

#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTPL0ACR_2_ADDR                          (MSS_QDSP6SS_QTMR_F2_2_REG_BASE            + 0x14)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTPL0ACR_2_OFFS                          (MSS_QDSP6SS_QTMR_F2_2_REG_BASE_OFFS + 0x14)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTPL0ACR_2_RMSK                               0x303
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTPL0ACR_2_IN                    \
                in_dword_masked(HWIO_MSS_QDSP6SS_QTMR_V1_CNTPL0ACR_2_ADDR, HWIO_MSS_QDSP6SS_QTMR_V1_CNTPL0ACR_2_RMSK)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTPL0ACR_2_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_QTMR_V1_CNTPL0ACR_2_ADDR, m)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTPL0ACR_2_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_QTMR_V1_CNTPL0ACR_2_ADDR,v)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTPL0ACR_2_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_QDSP6SS_QTMR_V1_CNTPL0ACR_2_ADDR,m,v,HWIO_MSS_QDSP6SS_QTMR_V1_CNTPL0ACR_2_IN)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTPL0ACR_2_PL0CTEN_BMSK                       0x200
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTPL0ACR_2_PL0CTEN_SHFT                           9
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTPL0ACR_2_PL0VTEN_BMSK                       0x100
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTPL0ACR_2_PL0VTEN_SHFT                           8
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTPL0ACR_2_PL0VCTEN_BMSK                        0x2
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTPL0ACR_2_PL0VCTEN_SHFT                          1
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTPL0ACR_2_PL0PCTEN_BMSK                        0x1
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTPL0ACR_2_PL0PCTEN_SHFT                          0

#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTVOFF_LO_2_ADDR                         (MSS_QDSP6SS_QTMR_F2_2_REG_BASE            + 0x18)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTVOFF_LO_2_OFFS                         (MSS_QDSP6SS_QTMR_F2_2_REG_BASE_OFFS + 0x18)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTVOFF_LO_2_RMSK                         0xffffffff
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTVOFF_LO_2_IN                    \
                in_dword_masked(HWIO_MSS_QDSP6SS_QTMR_V1_CNTVOFF_LO_2_ADDR, HWIO_MSS_QDSP6SS_QTMR_V1_CNTVOFF_LO_2_RMSK)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTVOFF_LO_2_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_QTMR_V1_CNTVOFF_LO_2_ADDR, m)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTVOFF_LO_2_CNTVOFF_L0_BMSK              0xffffffff
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTVOFF_LO_2_CNTVOFF_L0_SHFT                       0

#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTVOFF_HI_2_ADDR                         (MSS_QDSP6SS_QTMR_F2_2_REG_BASE            + 0x1c)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTVOFF_HI_2_OFFS                         (MSS_QDSP6SS_QTMR_F2_2_REG_BASE_OFFS + 0x1c)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTVOFF_HI_2_RMSK                           0xffffff
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTVOFF_HI_2_IN                    \
                in_dword_masked(HWIO_MSS_QDSP6SS_QTMR_V1_CNTVOFF_HI_2_ADDR, HWIO_MSS_QDSP6SS_QTMR_V1_CNTVOFF_HI_2_RMSK)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTVOFF_HI_2_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_QTMR_V1_CNTVOFF_HI_2_ADDR, m)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTVOFF_HI_2_CNTVOFF_HI_BMSK                0xffffff
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTVOFF_HI_2_CNTVOFF_HI_SHFT                       0

#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_CVAL_LO_2_ADDR                       (MSS_QDSP6SS_QTMR_F2_2_REG_BASE            + 0x20)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_CVAL_LO_2_OFFS                       (MSS_QDSP6SS_QTMR_F2_2_REG_BASE_OFFS + 0x20)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_CVAL_LO_2_RMSK                       0xffffffff
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_CVAL_LO_2_IN                    \
                in_dword_masked(HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_CVAL_LO_2_ADDR, HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_CVAL_LO_2_RMSK)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_CVAL_LO_2_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_CVAL_LO_2_ADDR, m)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_CVAL_LO_2_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_CVAL_LO_2_ADDR,v)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_CVAL_LO_2_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_CVAL_LO_2_ADDR,m,v,HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_CVAL_LO_2_IN)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_CVAL_LO_2_CNTP_CVAL_L0_BMSK          0xffffffff
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_CVAL_LO_2_CNTP_CVAL_L0_SHFT                   0

#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_CVAL_HI_2_ADDR                       (MSS_QDSP6SS_QTMR_F2_2_REG_BASE            + 0x24)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_CVAL_HI_2_OFFS                       (MSS_QDSP6SS_QTMR_F2_2_REG_BASE_OFFS + 0x24)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_CVAL_HI_2_RMSK                         0xffffff
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_CVAL_HI_2_IN                    \
                in_dword_masked(HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_CVAL_HI_2_ADDR, HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_CVAL_HI_2_RMSK)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_CVAL_HI_2_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_CVAL_HI_2_ADDR, m)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_CVAL_HI_2_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_CVAL_HI_2_ADDR,v)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_CVAL_HI_2_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_CVAL_HI_2_ADDR,m,v,HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_CVAL_HI_2_IN)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_CVAL_HI_2_CNTP_CVAL_HI_BMSK            0xffffff
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_CVAL_HI_2_CNTP_CVAL_HI_SHFT                   0

#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_TVAL_2_ADDR                          (MSS_QDSP6SS_QTMR_F2_2_REG_BASE            + 0x28)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_TVAL_2_OFFS                          (MSS_QDSP6SS_QTMR_F2_2_REG_BASE_OFFS + 0x28)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_TVAL_2_RMSK                          0xffffffff
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_TVAL_2_IN                    \
                in_dword_masked(HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_TVAL_2_ADDR, HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_TVAL_2_RMSK)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_TVAL_2_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_TVAL_2_ADDR, m)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_TVAL_2_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_TVAL_2_ADDR,v)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_TVAL_2_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_TVAL_2_ADDR,m,v,HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_TVAL_2_IN)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_TVAL_2_CNTP_TVAL_BMSK                0xffffffff
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_TVAL_2_CNTP_TVAL_SHFT                         0

#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_CTL_2_ADDR                           (MSS_QDSP6SS_QTMR_F2_2_REG_BASE            + 0x2c)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_CTL_2_OFFS                           (MSS_QDSP6SS_QTMR_F2_2_REG_BASE_OFFS + 0x2c)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_CTL_2_RMSK                                  0x7
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_CTL_2_IN                    \
                in_dword_masked(HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_CTL_2_ADDR, HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_CTL_2_RMSK)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_CTL_2_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_CTL_2_ADDR, m)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_CTL_2_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_CTL_2_ADDR,v)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_CTL_2_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_CTL_2_ADDR,m,v,HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_CTL_2_IN)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_CTL_2_ISTAT_BMSK                            0x4
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_CTL_2_ISTAT_SHFT                              2
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_CTL_2_IMSK_BMSK                             0x2
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_CTL_2_IMSK_SHFT                               1
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_CTL_2_EN_BMSK                               0x1
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_CTL_2_EN_SHFT                                 0

#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_CVAL_LO_2_ADDR                       (MSS_QDSP6SS_QTMR_F2_2_REG_BASE            + 0x30)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_CVAL_LO_2_OFFS                       (MSS_QDSP6SS_QTMR_F2_2_REG_BASE_OFFS + 0x30)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_CVAL_LO_2_RMSK                       0xffffffff
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_CVAL_LO_2_IN                    \
                in_dword_masked(HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_CVAL_LO_2_ADDR, HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_CVAL_LO_2_RMSK)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_CVAL_LO_2_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_CVAL_LO_2_ADDR, m)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_CVAL_LO_2_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_CVAL_LO_2_ADDR,v)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_CVAL_LO_2_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_CVAL_LO_2_ADDR,m,v,HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_CVAL_LO_2_IN)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_CVAL_LO_2_CNTV_CVAL_L0_BMSK          0xffffffff
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_CVAL_LO_2_CNTV_CVAL_L0_SHFT                   0

#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_CVAL_HI_2_ADDR                       (MSS_QDSP6SS_QTMR_F2_2_REG_BASE            + 0x34)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_CVAL_HI_2_OFFS                       (MSS_QDSP6SS_QTMR_F2_2_REG_BASE_OFFS + 0x34)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_CVAL_HI_2_RMSK                         0xffffff
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_CVAL_HI_2_IN                    \
                in_dword_masked(HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_CVAL_HI_2_ADDR, HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_CVAL_HI_2_RMSK)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_CVAL_HI_2_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_CVAL_HI_2_ADDR, m)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_CVAL_HI_2_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_CVAL_HI_2_ADDR,v)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_CVAL_HI_2_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_CVAL_HI_2_ADDR,m,v,HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_CVAL_HI_2_IN)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_CVAL_HI_2_CNTV_CVAL_HI_BMSK            0xffffff
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_CVAL_HI_2_CNTV_CVAL_HI_SHFT                   0

#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_TVAL_2_ADDR                          (MSS_QDSP6SS_QTMR_F2_2_REG_BASE            + 0x38)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_TVAL_2_OFFS                          (MSS_QDSP6SS_QTMR_F2_2_REG_BASE_OFFS + 0x38)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_TVAL_2_RMSK                          0xffffffff
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_TVAL_2_IN                    \
                in_dword_masked(HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_TVAL_2_ADDR, HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_TVAL_2_RMSK)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_TVAL_2_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_TVAL_2_ADDR, m)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_TVAL_2_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_TVAL_2_ADDR,v)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_TVAL_2_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_TVAL_2_ADDR,m,v,HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_TVAL_2_IN)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_TVAL_2_CNTV_TVAL_BMSK                0xffffffff
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_TVAL_2_CNTV_TVAL_SHFT                         0

#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_CTL_2_ADDR                           (MSS_QDSP6SS_QTMR_F2_2_REG_BASE            + 0x3c)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_CTL_2_OFFS                           (MSS_QDSP6SS_QTMR_F2_2_REG_BASE_OFFS + 0x3c)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_CTL_2_RMSK                                  0x7
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_CTL_2_IN                    \
                in_dword_masked(HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_CTL_2_ADDR, HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_CTL_2_RMSK)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_CTL_2_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_CTL_2_ADDR, m)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_CTL_2_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_CTL_2_ADDR,v)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_CTL_2_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_CTL_2_ADDR,m,v,HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_CTL_2_IN)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_CTL_2_ISTAT_BMSK                            0x4
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_CTL_2_ISTAT_SHFT                              2
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_CTL_2_IMSK_BMSK                             0x2
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_CTL_2_IMSK_SHFT                               1
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_CTL_2_EN_BMSK                               0x1
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_CTL_2_EN_SHFT                                 0

#define HWIO_MSS_QDSP6SS_QTMR_V1_VERSION_2_ADDR                            (MSS_QDSP6SS_QTMR_F2_2_REG_BASE            + 0xfd0)
#define HWIO_MSS_QDSP6SS_QTMR_V1_VERSION_2_OFFS                            (MSS_QDSP6SS_QTMR_F2_2_REG_BASE_OFFS + 0xfd0)
#define HWIO_MSS_QDSP6SS_QTMR_V1_VERSION_2_RMSK                            0xffffffff
#define HWIO_MSS_QDSP6SS_QTMR_V1_VERSION_2_IN                    \
                in_dword_masked(HWIO_MSS_QDSP6SS_QTMR_V1_VERSION_2_ADDR, HWIO_MSS_QDSP6SS_QTMR_V1_VERSION_2_RMSK)
#define HWIO_MSS_QDSP6SS_QTMR_V1_VERSION_2_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_QTMR_V1_VERSION_2_ADDR, m)
#define HWIO_MSS_QDSP6SS_QTMR_V1_VERSION_2_MAJOR_BMSK                      0xf0000000
#define HWIO_MSS_QDSP6SS_QTMR_V1_VERSION_2_MAJOR_SHFT                              28
#define HWIO_MSS_QDSP6SS_QTMR_V1_VERSION_2_MINOR_BMSK                       0xfff0000
#define HWIO_MSS_QDSP6SS_QTMR_V1_VERSION_2_MINOR_SHFT                              16
#define HWIO_MSS_QDSP6SS_QTMR_V1_VERSION_2_STEP_BMSK                           0xffff
#define HWIO_MSS_QDSP6SS_QTMR_V1_VERSION_2_STEP_SHFT                                0


#endif /* __HALCLKHWIOASM_H__ */
