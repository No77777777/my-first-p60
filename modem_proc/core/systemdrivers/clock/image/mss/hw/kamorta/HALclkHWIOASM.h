#ifndef __HALCLKHWIOASM_H__
#define __HALCLKHWIOASM_H__
/*
===========================================================================
*/
/**
  @file HALclkHWIOASM.h
  @brief Auto-generated HWIO interface include file.

  Reference chip release:
    SM4250 (Kamorta) [kamorta_v1.0_p3q2r48]
 
  This file contains HWIO register definitions for the following modules:
    MSS_QDSP6V67SS_PUB
    MSS_QDSP6SS_QDSP6SS_QTMR_AC


  Generation parameters: 
  { 'filename': 'HALclkHWIOASM.h',
    'module-filter-exclude': {},
    'module-filter-include': {},
    'modules': ['MSS_QDSP6V67SS_PUB', 'MSS_QDSP6SS_QDSP6SS_QTMR_AC']}
*/
/*
  ===========================================================================

  Copyright (c) 2019 Qualcomm Technologies, Inc.
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

  $Header: //components/rel/core.mpss/10.0/systemdrivers/clock/image/mss/hw/kamorta/HALclkHWIOASM.h#3 $
  $DateTime: 2019/09/04 05:16:40 $
  $Author: pwbldsvc $

  ===========================================================================
*/

#define MSS_TOP_BASE_PHYS      0x06000000
#define MSS_TOP_BASE           MSS_TOP_BASE_PHYS



/*----------------------------------------------------------------------------
 * MODULE: MSS_QDSP6V67SS_PUB
 *--------------------------------------------------------------------------*/

#define MSS_QDSP6V67SS_PUB_REG_BASE                                               (MSS_TOP_BASE      + 0x00080000)
#define MSS_QDSP6V67SS_PUB_REG_BASE_SIZE                                          0x10000
#define MSS_QDSP6V67SS_PUB_REG_BASE_USED                                          0x204c

#define HWIO_MSS_QDSP6SS_VERSION_ADDR                                             (MSS_QDSP6V67SS_PUB_REG_BASE      + 0x00000000)
#define HWIO_MSS_QDSP6SS_VERSION_RMSK                                             0xffffffff
#define HWIO_MSS_QDSP6SS_VERSION_IN          \
        in_dword(HWIO_MSS_QDSP6SS_VERSION_ADDR)
#define HWIO_MSS_QDSP6SS_VERSION_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_VERSION_ADDR, m)
#define HWIO_MSS_QDSP6SS_VERSION_MAJOR_BMSK                                       0xf0000000
#define HWIO_MSS_QDSP6SS_VERSION_MAJOR_SHFT                                             0x1c
#define HWIO_MSS_QDSP6SS_VERSION_MINOR_BMSK                                        0xfff0000
#define HWIO_MSS_QDSP6SS_VERSION_MINOR_SHFT                                             0x10
#define HWIO_MSS_QDSP6SS_VERSION_STEP_BMSK                                            0xffff
#define HWIO_MSS_QDSP6SS_VERSION_STEP_SHFT                                               0x0

#define HWIO_MSS_QDSP6SS_RST_EVB_ADDR                                             (MSS_QDSP6V67SS_PUB_REG_BASE      + 0x00000010)
#define HWIO_MSS_QDSP6SS_RST_EVB_RMSK                                              0xffffff0
#define HWIO_MSS_QDSP6SS_RST_EVB_IN          \
        in_dword(HWIO_MSS_QDSP6SS_RST_EVB_ADDR)
#define HWIO_MSS_QDSP6SS_RST_EVB_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_RST_EVB_ADDR, m)
#define HWIO_MSS_QDSP6SS_RST_EVB_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_RST_EVB_ADDR,v)
#define HWIO_MSS_QDSP6SS_RST_EVB_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_RST_EVB_ADDR,m,v,HWIO_MSS_QDSP6SS_RST_EVB_IN)
#define HWIO_MSS_QDSP6SS_RST_EVB_EVB_BMSK                                          0xffffff0
#define HWIO_MSS_QDSP6SS_RST_EVB_EVB_SHFT                                                0x4

#define HWIO_MSS_QDSP6SS_CLADE2_ARES_CFG_ADDR                                     (MSS_QDSP6V67SS_PUB_REG_BASE      + 0x00000014)
#define HWIO_MSS_QDSP6SS_CLADE2_ARES_CFG_RMSK                                            0x1
#define HWIO_MSS_QDSP6SS_CLADE2_ARES_CFG_IN          \
        in_dword(HWIO_MSS_QDSP6SS_CLADE2_ARES_CFG_ADDR)
#define HWIO_MSS_QDSP6SS_CLADE2_ARES_CFG_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_CLADE2_ARES_CFG_ADDR, m)
#define HWIO_MSS_QDSP6SS_CLADE2_ARES_CFG_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_CLADE2_ARES_CFG_ADDR,v)
#define HWIO_MSS_QDSP6SS_CLADE2_ARES_CFG_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_CLADE2_ARES_CFG_ADDR,m,v,HWIO_MSS_QDSP6SS_CLADE2_ARES_CFG_IN)
#define HWIO_MSS_QDSP6SS_CLADE2_ARES_CFG_CLADE2_ARES_ENA_BMSK                            0x1
#define HWIO_MSS_QDSP6SS_CLADE2_ARES_CFG_CLADE2_ARES_ENA_SHFT                            0x0

#define HWIO_MSS_QDSP6SS_DBG_CFG_ADDR                                             (MSS_QDSP6V67SS_PUB_REG_BASE      + 0x00000018)
#define HWIO_MSS_QDSP6SS_DBG_CFG_RMSK                                             0xffff0002
#define HWIO_MSS_QDSP6SS_DBG_CFG_IN          \
        in_dword(HWIO_MSS_QDSP6SS_DBG_CFG_ADDR)
#define HWIO_MSS_QDSP6SS_DBG_CFG_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_DBG_CFG_ADDR, m)
#define HWIO_MSS_QDSP6SS_DBG_CFG_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_DBG_CFG_ADDR,v)
#define HWIO_MSS_QDSP6SS_DBG_CFG_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_DBG_CFG_ADDR,m,v,HWIO_MSS_QDSP6SS_DBG_CFG_IN)
#define HWIO_MSS_QDSP6SS_DBG_CFG_DBG_SW_REG_BMSK                                  0xff000000
#define HWIO_MSS_QDSP6SS_DBG_CFG_DBG_SW_REG_SHFT                                        0x18
#define HWIO_MSS_QDSP6SS_DBG_CFG_DBG_SPARE_BMSK                                     0xff0000
#define HWIO_MSS_QDSP6SS_DBG_CFG_DBG_SPARE_SHFT                                         0x10
#define HWIO_MSS_QDSP6SS_DBG_CFG_CTI_TIHS_ENA_BMSK                                       0x2
#define HWIO_MSS_QDSP6SS_DBG_CFG_CTI_TIHS_ENA_SHFT                                       0x1

#define HWIO_MSS_QDSP6SS_RET_CFG_ADDR                                             (MSS_QDSP6V67SS_PUB_REG_BASE      + 0x0000001c)
#define HWIO_MSS_QDSP6SS_RET_CFG_RMSK                                                    0x3
#define HWIO_MSS_QDSP6SS_RET_CFG_IN          \
        in_dword(HWIO_MSS_QDSP6SS_RET_CFG_ADDR)
#define HWIO_MSS_QDSP6SS_RET_CFG_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_RET_CFG_ADDR, m)
#define HWIO_MSS_QDSP6SS_RET_CFG_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_RET_CFG_ADDR,v)
#define HWIO_MSS_QDSP6SS_RET_CFG_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_RET_CFG_ADDR,m,v,HWIO_MSS_QDSP6SS_RET_CFG_IN)
#define HWIO_MSS_QDSP6SS_RET_CFG_NRET_ARES_ENA_BMSK                                      0x2
#define HWIO_MSS_QDSP6SS_RET_CFG_NRET_ARES_ENA_SHFT                                      0x1
#define HWIO_MSS_QDSP6SS_RET_CFG_RET_ARES_ENA_BMSK                                       0x1
#define HWIO_MSS_QDSP6SS_RET_CFG_RET_ARES_ENA_SHFT                                       0x0

#define HWIO_MSS_QDSP6SS_CORE_CBCR_ADDR                                           (MSS_QDSP6V67SS_PUB_REG_BASE      + 0x00000020)
#define HWIO_MSS_QDSP6SS_CORE_CBCR_RMSK                                           0x80000001
#define HWIO_MSS_QDSP6SS_CORE_CBCR_IN          \
        in_dword(HWIO_MSS_QDSP6SS_CORE_CBCR_ADDR)
#define HWIO_MSS_QDSP6SS_CORE_CBCR_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_CORE_CBCR_ADDR, m)
#define HWIO_MSS_QDSP6SS_CORE_CBCR_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_CORE_CBCR_ADDR,v)
#define HWIO_MSS_QDSP6SS_CORE_CBCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_CORE_CBCR_ADDR,m,v,HWIO_MSS_QDSP6SS_CORE_CBCR_IN)
#define HWIO_MSS_QDSP6SS_CORE_CBCR_CLKOFF_BMSK                                    0x80000000
#define HWIO_MSS_QDSP6SS_CORE_CBCR_CLKOFF_SHFT                                          0x1f
#define HWIO_MSS_QDSP6SS_CORE_CBCR_CLKEN_BMSK                                            0x1
#define HWIO_MSS_QDSP6SS_CORE_CBCR_CLKEN_SHFT                                            0x0

#define HWIO_MSS_QDSP6SS_CORE_CMD_RCGR_ADDR                                       (MSS_QDSP6V67SS_PUB_REG_BASE      + 0x00000028)
#define HWIO_MSS_QDSP6SS_CORE_CMD_RCGR_RMSK                                       0x80000013
#define HWIO_MSS_QDSP6SS_CORE_CMD_RCGR_IN          \
        in_dword(HWIO_MSS_QDSP6SS_CORE_CMD_RCGR_ADDR)
#define HWIO_MSS_QDSP6SS_CORE_CMD_RCGR_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_CORE_CMD_RCGR_ADDR, m)
#define HWIO_MSS_QDSP6SS_CORE_CMD_RCGR_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_CORE_CMD_RCGR_ADDR,v)
#define HWIO_MSS_QDSP6SS_CORE_CMD_RCGR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_CORE_CMD_RCGR_ADDR,m,v,HWIO_MSS_QDSP6SS_CORE_CMD_RCGR_IN)
#define HWIO_MSS_QDSP6SS_CORE_CMD_RCGR_ROOT_OFF_BMSK                              0x80000000
#define HWIO_MSS_QDSP6SS_CORE_CMD_RCGR_ROOT_OFF_SHFT                                    0x1f
#define HWIO_MSS_QDSP6SS_CORE_CMD_RCGR_DIRTY_CFG_RCGR_BMSK                              0x10
#define HWIO_MSS_QDSP6SS_CORE_CMD_RCGR_DIRTY_CFG_RCGR_SHFT                               0x4
#define HWIO_MSS_QDSP6SS_CORE_CMD_RCGR_ROOT_EN_BMSK                                      0x2
#define HWIO_MSS_QDSP6SS_CORE_CMD_RCGR_ROOT_EN_SHFT                                      0x1
#define HWIO_MSS_QDSP6SS_CORE_CMD_RCGR_UPDATE_BMSK                                       0x1
#define HWIO_MSS_QDSP6SS_CORE_CMD_RCGR_UPDATE_SHFT                                       0x0

#define HWIO_MSS_QDSP6SS_CORE_CFG_RCGR_ADDR                                       (MSS_QDSP6V67SS_PUB_REG_BASE      + 0x0000002c)
#define HWIO_MSS_QDSP6SS_CORE_CFG_RCGR_RMSK                                         0x11071f
#define HWIO_MSS_QDSP6SS_CORE_CFG_RCGR_IN          \
        in_dword(HWIO_MSS_QDSP6SS_CORE_CFG_RCGR_ADDR)
#define HWIO_MSS_QDSP6SS_CORE_CFG_RCGR_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_CORE_CFG_RCGR_ADDR, m)
#define HWIO_MSS_QDSP6SS_CORE_CFG_RCGR_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_CORE_CFG_RCGR_ADDR,v)
#define HWIO_MSS_QDSP6SS_CORE_CFG_RCGR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_CORE_CFG_RCGR_ADDR,m,v,HWIO_MSS_QDSP6SS_CORE_CFG_RCGR_IN)
#define HWIO_MSS_QDSP6SS_CORE_CFG_RCGR_HW_CLK_CONTROL_BMSK                          0x100000
#define HWIO_MSS_QDSP6SS_CORE_CFG_RCGR_HW_CLK_CONTROL_SHFT                              0x14
#define HWIO_MSS_QDSP6SS_CORE_CFG_RCGR_RCGLITE_DISABLE_BMSK                          0x10000
#define HWIO_MSS_QDSP6SS_CORE_CFG_RCGR_RCGLITE_DISABLE_SHFT                             0x10
#define HWIO_MSS_QDSP6SS_CORE_CFG_RCGR_SRC_SEL_BMSK                                    0x700
#define HWIO_MSS_QDSP6SS_CORE_CFG_RCGR_SRC_SEL_SHFT                                      0x8
#define HWIO_MSS_QDSP6SS_CORE_CFG_RCGR_SRC_DIV_BMSK                                     0x1f
#define HWIO_MSS_QDSP6SS_CORE_CFG_RCGR_SRC_DIV_SHFT                                      0x0

#define HWIO_MSS_QDSP6SS_SPDM_MON_CBCR_ADDR                                       (MSS_QDSP6V67SS_PUB_REG_BASE      + 0x00000048)
#define HWIO_MSS_QDSP6SS_SPDM_MON_CBCR_RMSK                                       0x80000001
#define HWIO_MSS_QDSP6SS_SPDM_MON_CBCR_IN          \
        in_dword(HWIO_MSS_QDSP6SS_SPDM_MON_CBCR_ADDR)
#define HWIO_MSS_QDSP6SS_SPDM_MON_CBCR_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_SPDM_MON_CBCR_ADDR, m)
#define HWIO_MSS_QDSP6SS_SPDM_MON_CBCR_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_SPDM_MON_CBCR_ADDR,v)
#define HWIO_MSS_QDSP6SS_SPDM_MON_CBCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_SPDM_MON_CBCR_ADDR,m,v,HWIO_MSS_QDSP6SS_SPDM_MON_CBCR_IN)
#define HWIO_MSS_QDSP6SS_SPDM_MON_CBCR_CLKOFF_BMSK                                0x80000000
#define HWIO_MSS_QDSP6SS_SPDM_MON_CBCR_CLKOFF_SHFT                                      0x1f
#define HWIO_MSS_QDSP6SS_SPDM_MON_CBCR_CLKEN_BMSK                                        0x1
#define HWIO_MSS_QDSP6SS_SPDM_MON_CBCR_CLKEN_SHFT                                        0x0

#define HWIO_MSS_QDSP6SS_XO_CBCR_ADDR                                             (MSS_QDSP6V67SS_PUB_REG_BASE      + 0x00000038)
#define HWIO_MSS_QDSP6SS_XO_CBCR_RMSK                                             0x80000001
#define HWIO_MSS_QDSP6SS_XO_CBCR_IN          \
        in_dword(HWIO_MSS_QDSP6SS_XO_CBCR_ADDR)
#define HWIO_MSS_QDSP6SS_XO_CBCR_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_XO_CBCR_ADDR, m)
#define HWIO_MSS_QDSP6SS_XO_CBCR_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_XO_CBCR_ADDR,v)
#define HWIO_MSS_QDSP6SS_XO_CBCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_XO_CBCR_ADDR,m,v,HWIO_MSS_QDSP6SS_XO_CBCR_IN)
#define HWIO_MSS_QDSP6SS_XO_CBCR_CLKOFF_BMSK                                      0x80000000
#define HWIO_MSS_QDSP6SS_XO_CBCR_CLKOFF_SHFT                                            0x1f
#define HWIO_MSS_QDSP6SS_XO_CBCR_CLKEN_BMSK                                              0x1
#define HWIO_MSS_QDSP6SS_XO_CBCR_CLKEN_SHFT                                              0x0

#define HWIO_MSS_QDSP6SS_SLEEP_CBCR_ADDR                                          (MSS_QDSP6V67SS_PUB_REG_BASE      + 0x0000003c)
#define HWIO_MSS_QDSP6SS_SLEEP_CBCR_RMSK                                          0x80000001
#define HWIO_MSS_QDSP6SS_SLEEP_CBCR_IN          \
        in_dword(HWIO_MSS_QDSP6SS_SLEEP_CBCR_ADDR)
#define HWIO_MSS_QDSP6SS_SLEEP_CBCR_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_SLEEP_CBCR_ADDR, m)
#define HWIO_MSS_QDSP6SS_SLEEP_CBCR_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_SLEEP_CBCR_ADDR,v)
#define HWIO_MSS_QDSP6SS_SLEEP_CBCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_SLEEP_CBCR_ADDR,m,v,HWIO_MSS_QDSP6SS_SLEEP_CBCR_IN)
#define HWIO_MSS_QDSP6SS_SLEEP_CBCR_CLKOFF_BMSK                                   0x80000000
#define HWIO_MSS_QDSP6SS_SLEEP_CBCR_CLKOFF_SHFT                                         0x1f
#define HWIO_MSS_QDSP6SS_SLEEP_CBCR_CLKEN_BMSK                                           0x1
#define HWIO_MSS_QDSP6SS_SLEEP_CBCR_CLKEN_SHFT                                           0x0

#define HWIO_MSS_QDSP6SS_NMI_ADDR                                                 (MSS_QDSP6V67SS_PUB_REG_BASE      + 0x00000040)
#define HWIO_MSS_QDSP6SS_NMI_RMSK                                                        0x3
#define HWIO_MSS_QDSP6SS_NMI_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_NMI_ADDR,v)
#define HWIO_MSS_QDSP6SS_NMI_CLEAR_STATUS_BMSK                                           0x2
#define HWIO_MSS_QDSP6SS_NMI_CLEAR_STATUS_SHFT                                           0x1
#define HWIO_MSS_QDSP6SS_NMI_SET_NMI_BMSK                                                0x1
#define HWIO_MSS_QDSP6SS_NMI_SET_NMI_SHFT                                                0x0

#define HWIO_MSS_QDSP6SS_NMI_STATUS_ADDR                                          (MSS_QDSP6V67SS_PUB_REG_BASE      + 0x00000044)
#define HWIO_MSS_QDSP6SS_NMI_STATUS_RMSK                                                 0x7
#define HWIO_MSS_QDSP6SS_NMI_STATUS_IN          \
        in_dword(HWIO_MSS_QDSP6SS_NMI_STATUS_ADDR)
#define HWIO_MSS_QDSP6SS_NMI_STATUS_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_NMI_STATUS_ADDR, m)
#define HWIO_MSS_QDSP6SS_NMI_STATUS_EXT_DBG_TRIG_BMSK                                    0x4
#define HWIO_MSS_QDSP6SS_NMI_STATUS_EXT_DBG_TRIG_SHFT                                    0x2
#define HWIO_MSS_QDSP6SS_NMI_STATUS_WDOG_TRIG_BMSK                                       0x2
#define HWIO_MSS_QDSP6SS_NMI_STATUS_WDOG_TRIG_SHFT                                       0x1
#define HWIO_MSS_QDSP6SS_NMI_STATUS_PUBCSR_TRIG_BMSK                                     0x1
#define HWIO_MSS_QDSP6SS_NMI_STATUS_PUBCSR_TRIG_SHFT                                     0x0

#define HWIO_MSS_QDSP6SS_INTF_HALTREQ_ADDR                                        (MSS_QDSP6V67SS_PUB_REG_BASE      + 0x00000088)
#define HWIO_MSS_QDSP6SS_INTF_HALTREQ_RMSK                                               0x7
#define HWIO_MSS_QDSP6SS_INTF_HALTREQ_IN          \
        in_dword(HWIO_MSS_QDSP6SS_INTF_HALTREQ_ADDR)
#define HWIO_MSS_QDSP6SS_INTF_HALTREQ_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_INTF_HALTREQ_ADDR, m)
#define HWIO_MSS_QDSP6SS_INTF_HALTREQ_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_INTF_HALTREQ_ADDR,v)
#define HWIO_MSS_QDSP6SS_INTF_HALTREQ_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_INTF_HALTREQ_ADDR,m,v,HWIO_MSS_QDSP6SS_INTF_HALTREQ_IN)
#define HWIO_MSS_QDSP6SS_INTF_HALTREQ_AXIM2_BMSK                                         0x4
#define HWIO_MSS_QDSP6SS_INTF_HALTREQ_AXIM2_SHFT                                         0x2
#define HWIO_MSS_QDSP6SS_INTF_HALTREQ_AXIM_BMSK                                          0x2
#define HWIO_MSS_QDSP6SS_INTF_HALTREQ_AXIM_SHFT                                          0x1
#define HWIO_MSS_QDSP6SS_INTF_HALTREQ_ALL_BMSK                                           0x1
#define HWIO_MSS_QDSP6SS_INTF_HALTREQ_ALL_SHFT                                           0x0

#define HWIO_MSS_QDSP6SS_INTF_HALTACK_ADDR                                        (MSS_QDSP6V67SS_PUB_REG_BASE      + 0x0000008c)
#define HWIO_MSS_QDSP6SS_INTF_HALTACK_RMSK                                               0x7
#define HWIO_MSS_QDSP6SS_INTF_HALTACK_IN          \
        in_dword(HWIO_MSS_QDSP6SS_INTF_HALTACK_ADDR)
#define HWIO_MSS_QDSP6SS_INTF_HALTACK_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_INTF_HALTACK_ADDR, m)
#define HWIO_MSS_QDSP6SS_INTF_HALTACK_AXIM2_BMSK                                         0x4
#define HWIO_MSS_QDSP6SS_INTF_HALTACK_AXIM2_SHFT                                         0x2
#define HWIO_MSS_QDSP6SS_INTF_HALTACK_AXIM_BMSK                                          0x2
#define HWIO_MSS_QDSP6SS_INTF_HALTACK_AXIM_SHFT                                          0x1
#define HWIO_MSS_QDSP6SS_INTF_HALTACK_ALL_BMSK                                           0x1
#define HWIO_MSS_QDSP6SS_INTF_HALTACK_ALL_SHFT                                           0x0

#define HWIO_MSS_QDSP6SS_INTFCLAMP_SET_ADDR                                       (MSS_QDSP6V67SS_PUB_REG_BASE      + 0x00000090)
#define HWIO_MSS_QDSP6SS_INTFCLAMP_SET_RMSK                                              0xf
#define HWIO_MSS_QDSP6SS_INTFCLAMP_SET_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_INTFCLAMP_SET_ADDR,v)
#define HWIO_MSS_QDSP6SS_INTFCLAMP_SET_AXIM2_CLAMP_E_BMSK                                0x8
#define HWIO_MSS_QDSP6SS_INTFCLAMP_SET_AXIM2_CLAMP_E_SHFT                                0x3
#define HWIO_MSS_QDSP6SS_INTFCLAMP_SET_AXIM2_CLAMP_L_BMSK                                0x4
#define HWIO_MSS_QDSP6SS_INTFCLAMP_SET_AXIM2_CLAMP_L_SHFT                                0x2
#define HWIO_MSS_QDSP6SS_INTFCLAMP_SET_AXIM_CLAMP_E_BMSK                                 0x2
#define HWIO_MSS_QDSP6SS_INTFCLAMP_SET_AXIM_CLAMP_E_SHFT                                 0x1
#define HWIO_MSS_QDSP6SS_INTFCLAMP_SET_AXIM_CLAMP_L_BMSK                                 0x1
#define HWIO_MSS_QDSP6SS_INTFCLAMP_SET_AXIM_CLAMP_L_SHFT                                 0x0

#define HWIO_MSS_QDSP6SS_INTFCLAMP_CLEAR_ADDR                                     (MSS_QDSP6V67SS_PUB_REG_BASE      + 0x00000094)
#define HWIO_MSS_QDSP6SS_INTFCLAMP_CLEAR_RMSK                                            0xf
#define HWIO_MSS_QDSP6SS_INTFCLAMP_CLEAR_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_INTFCLAMP_CLEAR_ADDR,v)
#define HWIO_MSS_QDSP6SS_INTFCLAMP_CLEAR_AXIM2_CLAMP_E_BMSK                              0x8
#define HWIO_MSS_QDSP6SS_INTFCLAMP_CLEAR_AXIM2_CLAMP_E_SHFT                              0x3
#define HWIO_MSS_QDSP6SS_INTFCLAMP_CLEAR_AXIM2_CLAMP_L_BMSK                              0x4
#define HWIO_MSS_QDSP6SS_INTFCLAMP_CLEAR_AXIM2_CLAMP_L_SHFT                              0x2
#define HWIO_MSS_QDSP6SS_INTFCLAMP_CLEAR_AXIM_CLAMP_E_BMSK                               0x2
#define HWIO_MSS_QDSP6SS_INTFCLAMP_CLEAR_AXIM_CLAMP_E_SHFT                               0x1
#define HWIO_MSS_QDSP6SS_INTFCLAMP_CLEAR_AXIM_CLAMP_L_BMSK                               0x1
#define HWIO_MSS_QDSP6SS_INTFCLAMP_CLEAR_AXIM_CLAMP_L_SHFT                               0x0

#define HWIO_MSS_QDSP6SS_INTFCLAMP_STATUS_ADDR                                    (MSS_QDSP6V67SS_PUB_REG_BASE      + 0x00000098)
#define HWIO_MSS_QDSP6SS_INTFCLAMP_STATUS_RMSK                                           0xf
#define HWIO_MSS_QDSP6SS_INTFCLAMP_STATUS_IN          \
        in_dword(HWIO_MSS_QDSP6SS_INTFCLAMP_STATUS_ADDR)
#define HWIO_MSS_QDSP6SS_INTFCLAMP_STATUS_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_INTFCLAMP_STATUS_ADDR, m)
#define HWIO_MSS_QDSP6SS_INTFCLAMP_STATUS_AXIM2_CLAMP_E_BMSK                             0x8
#define HWIO_MSS_QDSP6SS_INTFCLAMP_STATUS_AXIM2_CLAMP_E_SHFT                             0x3
#define HWIO_MSS_QDSP6SS_INTFCLAMP_STATUS_AXIM2_CLAMP_L_BMSK                             0x4
#define HWIO_MSS_QDSP6SS_INTFCLAMP_STATUS_AXIM2_CLAMP_L_SHFT                             0x2
#define HWIO_MSS_QDSP6SS_INTFCLAMP_STATUS_AXIM_CLAMP_E_BMSK                              0x2
#define HWIO_MSS_QDSP6SS_INTFCLAMP_STATUS_AXIM_CLAMP_E_SHFT                              0x1
#define HWIO_MSS_QDSP6SS_INTFCLAMP_STATUS_AXIM_CLAMP_L_BMSK                              0x1
#define HWIO_MSS_QDSP6SS_INTFCLAMP_STATUS_AXIM_CLAMP_L_SHFT                              0x0

#define HWIO_MSS_QDSP6SS_INTF_FIFO_RESET_ADDR                                     (MSS_QDSP6V67SS_PUB_REG_BASE      + 0x0000009c)
#define HWIO_MSS_QDSP6SS_INTF_FIFO_RESET_RMSK                                            0x3
#define HWIO_MSS_QDSP6SS_INTF_FIFO_RESET_IN          \
        in_dword(HWIO_MSS_QDSP6SS_INTF_FIFO_RESET_ADDR)
#define HWIO_MSS_QDSP6SS_INTF_FIFO_RESET_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_INTF_FIFO_RESET_ADDR, m)
#define HWIO_MSS_QDSP6SS_INTF_FIFO_RESET_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_INTF_FIFO_RESET_ADDR,v)
#define HWIO_MSS_QDSP6SS_INTF_FIFO_RESET_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_INTF_FIFO_RESET_ADDR,m,v,HWIO_MSS_QDSP6SS_INTF_FIFO_RESET_IN)
#define HWIO_MSS_QDSP6SS_INTF_FIFO_RESET_AXIM2_BMSK                                      0x2
#define HWIO_MSS_QDSP6SS_INTF_FIFO_RESET_AXIM2_SHFT                                      0x1
#define HWIO_MSS_QDSP6SS_INTF_FIFO_RESET_AXIM_BMSK                                       0x1
#define HWIO_MSS_QDSP6SS_INTF_FIFO_RESET_AXIM_SHFT                                       0x0

#define HWIO_MSS_QDSP6SS_STATERET_CTL_ADDR                                        (MSS_QDSP6V67SS_PUB_REG_BASE      + 0x000000a0)
#define HWIO_MSS_QDSP6SS_STATERET_CTL_RMSK                                               0xf
#define HWIO_MSS_QDSP6SS_STATERET_CTL_IN          \
        in_dword(HWIO_MSS_QDSP6SS_STATERET_CTL_ADDR)
#define HWIO_MSS_QDSP6SS_STATERET_CTL_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_STATERET_CTL_ADDR, m)
#define HWIO_MSS_QDSP6SS_STATERET_CTL_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_STATERET_CTL_ADDR,v)
#define HWIO_MSS_QDSP6SS_STATERET_CTL_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_STATERET_CTL_ADDR,m,v,HWIO_MSS_QDSP6SS_STATERET_CTL_IN)
#define HWIO_MSS_QDSP6SS_STATERET_CTL_WAKEUP_IN_BMSK                                     0x8
#define HWIO_MSS_QDSP6SS_STATERET_CTL_WAKEUP_IN_SHFT                                     0x3
#define HWIO_MSS_QDSP6SS_STATERET_CTL_WAKE_IRQ_BMSK                                      0x4
#define HWIO_MSS_QDSP6SS_STATERET_CTL_WAKE_IRQ_SHFT                                      0x2
#define HWIO_MSS_QDSP6SS_STATERET_CTL_RESTORE_BMSK                                       0x2
#define HWIO_MSS_QDSP6SS_STATERET_CTL_RESTORE_SHFT                                       0x1
#define HWIO_MSS_QDSP6SS_STATERET_CTL_SAVE_BMSK                                          0x1
#define HWIO_MSS_QDSP6SS_STATERET_CTL_SAVE_SHFT                                          0x0

#define HWIO_MSS_QDSP6SS_CGC_OVERRIDE_ADDR                                        (MSS_QDSP6V67SS_PUB_REG_BASE      + 0x000000b0)
#define HWIO_MSS_QDSP6SS_CGC_OVERRIDE_RMSK                                            0xffff
#define HWIO_MSS_QDSP6SS_CGC_OVERRIDE_IN          \
        in_dword(HWIO_MSS_QDSP6SS_CGC_OVERRIDE_ADDR)
#define HWIO_MSS_QDSP6SS_CGC_OVERRIDE_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_CGC_OVERRIDE_ADDR, m)
#define HWIO_MSS_QDSP6SS_CGC_OVERRIDE_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_CGC_OVERRIDE_ADDR,v)
#define HWIO_MSS_QDSP6SS_CGC_OVERRIDE_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_CGC_OVERRIDE_ADDR,m,v,HWIO_MSS_QDSP6SS_CGC_OVERRIDE_IN)
#define HWIO_MSS_QDSP6SS_CGC_OVERRIDE_CP0_CLK_EN_BMSK                                 0x8000
#define HWIO_MSS_QDSP6SS_CGC_OVERRIDE_CP0_CLK_EN_SHFT                                    0xf
#define HWIO_MSS_QDSP6SS_CGC_OVERRIDE_SPARE_BIT14_BMSK                                0x4000
#define HWIO_MSS_QDSP6SS_CGC_OVERRIDE_SPARE_BIT14_SHFT                                   0xe
#define HWIO_MSS_QDSP6SS_CGC_OVERRIDE_AXIS_CLK_EN_BMSK                                0x2000
#define HWIO_MSS_QDSP6SS_CGC_OVERRIDE_AXIS_CLK_EN_SHFT                                   0xd
#define HWIO_MSS_QDSP6SS_CGC_OVERRIDE_SPARE_BIT12_BMSK                                0x1000
#define HWIO_MSS_QDSP6SS_CGC_OVERRIDE_SPARE_BIT12_SHFT                                   0xc
#define HWIO_MSS_QDSP6SS_CGC_OVERRIDE_AHBM_CLK_EN_BMSK                                 0x800
#define HWIO_MSS_QDSP6SS_CGC_OVERRIDE_AHBM_CLK_EN_SHFT                                   0xb
#define HWIO_MSS_QDSP6SS_CGC_OVERRIDE_AHBS_CLK_EN_BMSK                                 0x400
#define HWIO_MSS_QDSP6SS_CGC_OVERRIDE_AHBS_CLK_EN_SHFT                                   0xa
#define HWIO_MSS_QDSP6SS_CGC_OVERRIDE_PRIV_AHBS_CLK_EN_BMSK                            0x300
#define HWIO_MSS_QDSP6SS_CGC_OVERRIDE_PRIV_AHBS_CLK_EN_SHFT                              0x8
#define HWIO_MSS_QDSP6SS_CGC_OVERRIDE_SPARE_BIT7_BMSK                                   0x80
#define HWIO_MSS_QDSP6SS_CGC_OVERRIDE_SPARE_BIT7_SHFT                                    0x7
#define HWIO_MSS_QDSP6SS_CGC_OVERRIDE_L2VIC_AHBS_CLK_EN_BMSK                            0x60
#define HWIO_MSS_QDSP6SS_CGC_OVERRIDE_L2VIC_AHBS_CLK_EN_SHFT                             0x5
#define HWIO_MSS_QDSP6SS_CGC_OVERRIDE_TEST_AHBS_CLK_EN_BMSK                             0x10
#define HWIO_MSS_QDSP6SS_CGC_OVERRIDE_TEST_AHBS_CLK_EN_SHFT                              0x4
#define HWIO_MSS_QDSP6SS_CGC_OVERRIDE_PUB_AHBS_CLK_EN_BMSK                               0x8
#define HWIO_MSS_QDSP6SS_CGC_OVERRIDE_PUB_AHBS_CLK_EN_SHFT                               0x3
#define HWIO_MSS_QDSP6SS_CGC_OVERRIDE_CORE_BUS_EN_BMSK                                   0x4
#define HWIO_MSS_QDSP6SS_CGC_OVERRIDE_CORE_BUS_EN_SHFT                                   0x2
#define HWIO_MSS_QDSP6SS_CGC_OVERRIDE_CORE_RCLK_EN_BMSK                                  0x2
#define HWIO_MSS_QDSP6SS_CGC_OVERRIDE_CORE_RCLK_EN_SHFT                                  0x1
#define HWIO_MSS_QDSP6SS_CGC_OVERRIDE_CORE_CLK_EN_BMSK                                   0x1
#define HWIO_MSS_QDSP6SS_CGC_OVERRIDE_CORE_CLK_EN_SHFT                                   0x0

#define HWIO_MSS_QDSP6SS_CORE_BHS_CTL_ADDR                                        (MSS_QDSP6V67SS_PUB_REG_BASE      + 0x000000c0)
#define HWIO_MSS_QDSP6SS_CORE_BHS_CTL_RMSK                                           0x3f3ff
#define HWIO_MSS_QDSP6SS_CORE_BHS_CTL_IN          \
        in_dword(HWIO_MSS_QDSP6SS_CORE_BHS_CTL_ADDR)
#define HWIO_MSS_QDSP6SS_CORE_BHS_CTL_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_CORE_BHS_CTL_ADDR, m)
#define HWIO_MSS_QDSP6SS_CORE_BHS_CTL_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_CORE_BHS_CTL_ADDR,v)
#define HWIO_MSS_QDSP6SS_CORE_BHS_CTL_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_CORE_BHS_CTL_ADDR,m,v,HWIO_MSS_QDSP6SS_CORE_BHS_CTL_IN)
#define HWIO_MSS_QDSP6SS_CORE_BHS_CTL_DELAY_PROG_BMSK                                0x3f000
#define HWIO_MSS_QDSP6SS_CORE_BHS_CTL_DELAY_PROG_SHFT                                    0xc
#define HWIO_MSS_QDSP6SS_CORE_BHS_CTL_ENF_PROG_BMSK                                    0x3f0
#define HWIO_MSS_QDSP6SS_CORE_BHS_CTL_ENF_PROG_SHFT                                      0x4
#define HWIO_MSS_QDSP6SS_CORE_BHS_CTL_DRIVE_SEL_BMSK                                     0xf
#define HWIO_MSS_QDSP6SS_CORE_BHS_CTL_DRIVE_SEL_SHFT                                     0x0

#define HWIO_MSS_QDSP6SS_CPR_OVERRIDE_ADDR                                        (MSS_QDSP6V67SS_PUB_REG_BASE      + 0x000000d0)
#define HWIO_MSS_QDSP6SS_CPR_OVERRIDE_RMSK                                               0x3
#define HWIO_MSS_QDSP6SS_CPR_OVERRIDE_IN          \
        in_dword(HWIO_MSS_QDSP6SS_CPR_OVERRIDE_ADDR)
#define HWIO_MSS_QDSP6SS_CPR_OVERRIDE_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_CPR_OVERRIDE_ADDR, m)
#define HWIO_MSS_QDSP6SS_CPR_OVERRIDE_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_CPR_OVERRIDE_ADDR,v)
#define HWIO_MSS_QDSP6SS_CPR_OVERRIDE_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_CPR_OVERRIDE_ADDR,m,v,HWIO_MSS_QDSP6SS_CPR_OVERRIDE_IN)
#define HWIO_MSS_QDSP6SS_CPR_OVERRIDE_EXT_ABYP_DIS_BMSK                                  0x2
#define HWIO_MSS_QDSP6SS_CPR_OVERRIDE_EXT_ABYP_DIS_SHFT                                  0x1
#define HWIO_MSS_QDSP6SS_CPR_OVERRIDE_LOCAL_BYP_DIS_BMSK                                 0x1
#define HWIO_MSS_QDSP6SS_CPR_OVERRIDE_LOCAL_BYP_DIS_SHFT                                 0x0

#define HWIO_MSS_QDSP6SS_STRAP_TCM_ADDR                                           (MSS_QDSP6V67SS_PUB_REG_BASE      + 0x00000100)
#define HWIO_MSS_QDSP6SS_STRAP_TCM_RMSK                                           0xfffc0000
#define HWIO_MSS_QDSP6SS_STRAP_TCM_IN          \
        in_dword(HWIO_MSS_QDSP6SS_STRAP_TCM_ADDR)
#define HWIO_MSS_QDSP6SS_STRAP_TCM_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_STRAP_TCM_ADDR, m)
#define HWIO_MSS_QDSP6SS_STRAP_TCM_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_STRAP_TCM_ADDR,v)
#define HWIO_MSS_QDSP6SS_STRAP_TCM_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_STRAP_TCM_ADDR,m,v,HWIO_MSS_QDSP6SS_STRAP_TCM_IN)
#define HWIO_MSS_QDSP6SS_STRAP_TCM_BASE_ADDR_BMSK                                 0xfffc0000
#define HWIO_MSS_QDSP6SS_STRAP_TCM_BASE_ADDR_SHFT                                       0x12

#define HWIO_MSS_QDSP6SS_STRAP_AHBUPPER_ADDR                                      (MSS_QDSP6V67SS_PUB_REG_BASE      + 0x00000104)
#define HWIO_MSS_QDSP6SS_STRAP_AHBUPPER_RMSK                                      0xffffc000
#define HWIO_MSS_QDSP6SS_STRAP_AHBUPPER_IN          \
        in_dword(HWIO_MSS_QDSP6SS_STRAP_AHBUPPER_ADDR)
#define HWIO_MSS_QDSP6SS_STRAP_AHBUPPER_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_STRAP_AHBUPPER_ADDR, m)
#define HWIO_MSS_QDSP6SS_STRAP_AHBUPPER_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_STRAP_AHBUPPER_ADDR,v)
#define HWIO_MSS_QDSP6SS_STRAP_AHBUPPER_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_STRAP_AHBUPPER_ADDR,m,v,HWIO_MSS_QDSP6SS_STRAP_AHBUPPER_IN)
#define HWIO_MSS_QDSP6SS_STRAP_AHBUPPER_UPPER_ADDR_BMSK                           0xffffc000
#define HWIO_MSS_QDSP6SS_STRAP_AHBUPPER_UPPER_ADDR_SHFT                                  0xe

#define HWIO_MSS_QDSP6SS_STRAP_AHBLOWER_ADDR                                      (MSS_QDSP6V67SS_PUB_REG_BASE      + 0x00000108)
#define HWIO_MSS_QDSP6SS_STRAP_AHBLOWER_RMSK                                      0xffffc000
#define HWIO_MSS_QDSP6SS_STRAP_AHBLOWER_IN          \
        in_dword(HWIO_MSS_QDSP6SS_STRAP_AHBLOWER_ADDR)
#define HWIO_MSS_QDSP6SS_STRAP_AHBLOWER_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_STRAP_AHBLOWER_ADDR, m)
#define HWIO_MSS_QDSP6SS_STRAP_AHBLOWER_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_STRAP_AHBLOWER_ADDR,v)
#define HWIO_MSS_QDSP6SS_STRAP_AHBLOWER_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_STRAP_AHBLOWER_ADDR,m,v,HWIO_MSS_QDSP6SS_STRAP_AHBLOWER_IN)
#define HWIO_MSS_QDSP6SS_STRAP_AHBLOWER_LOWER_ADDR_BMSK                           0xffffc000
#define HWIO_MSS_QDSP6SS_STRAP_AHBLOWER_LOWER_ADDR_SHFT                                  0xe

#define HWIO_MSS_QDSP6SS_DCC_CTRL_ADDR                                            (MSS_QDSP6V67SS_PUB_REG_BASE      + 0x00000118)
#define HWIO_MSS_QDSP6SS_DCC_CTRL_RMSK                                            0xffffffff
#define HWIO_MSS_QDSP6SS_DCC_CTRL_IN          \
        in_dword(HWIO_MSS_QDSP6SS_DCC_CTRL_ADDR)
#define HWIO_MSS_QDSP6SS_DCC_CTRL_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_DCC_CTRL_ADDR, m)
#define HWIO_MSS_QDSP6SS_DCC_CTRL_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_DCC_CTRL_ADDR,v)
#define HWIO_MSS_QDSP6SS_DCC_CTRL_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_DCC_CTRL_ADDR,m,v,HWIO_MSS_QDSP6SS_DCC_CTRL_IN)
#define HWIO_MSS_QDSP6SS_DCC_CTRL_CTRL_BMSK                                       0xffffffff
#define HWIO_MSS_QDSP6SS_DCC_CTRL_CTRL_SHFT                                              0x0

#define HWIO_MSS_QDSP6SS_STRAP_AXIM2UPPER_ADDR                                    (MSS_QDSP6V67SS_PUB_REG_BASE      + 0x0000011c)
#define HWIO_MSS_QDSP6SS_STRAP_AXIM2UPPER_RMSK                                    0xffffc000
#define HWIO_MSS_QDSP6SS_STRAP_AXIM2UPPER_IN          \
        in_dword(HWIO_MSS_QDSP6SS_STRAP_AXIM2UPPER_ADDR)
#define HWIO_MSS_QDSP6SS_STRAP_AXIM2UPPER_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_STRAP_AXIM2UPPER_ADDR, m)
#define HWIO_MSS_QDSP6SS_STRAP_AXIM2UPPER_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_STRAP_AXIM2UPPER_ADDR,v)
#define HWIO_MSS_QDSP6SS_STRAP_AXIM2UPPER_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_STRAP_AXIM2UPPER_ADDR,m,v,HWIO_MSS_QDSP6SS_STRAP_AXIM2UPPER_IN)
#define HWIO_MSS_QDSP6SS_STRAP_AXIM2UPPER_UPPER_ADDR_BMSK                         0xffffc000
#define HWIO_MSS_QDSP6SS_STRAP_AXIM2UPPER_UPPER_ADDR_SHFT                                0xe

#define HWIO_MSS_QDSP6SS_STRAP_AXIM2LOWER_ADDR                                    (MSS_QDSP6V67SS_PUB_REG_BASE      + 0x00000120)
#define HWIO_MSS_QDSP6SS_STRAP_AXIM2LOWER_RMSK                                    0xffffc000
#define HWIO_MSS_QDSP6SS_STRAP_AXIM2LOWER_IN          \
        in_dword(HWIO_MSS_QDSP6SS_STRAP_AXIM2LOWER_ADDR)
#define HWIO_MSS_QDSP6SS_STRAP_AXIM2LOWER_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_STRAP_AXIM2LOWER_ADDR, m)
#define HWIO_MSS_QDSP6SS_STRAP_AXIM2LOWER_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_STRAP_AXIM2LOWER_ADDR,v)
#define HWIO_MSS_QDSP6SS_STRAP_AXIM2LOWER_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_STRAP_AXIM2LOWER_ADDR,m,v,HWIO_MSS_QDSP6SS_STRAP_AXIM2LOWER_IN)
#define HWIO_MSS_QDSP6SS_STRAP_AXIM2LOWER_LOWER_ADDR_BMSK                         0xffffc000
#define HWIO_MSS_QDSP6SS_STRAP_AXIM2LOWER_LOWER_ADDR_SHFT                                0xe

#define HWIO_MSS_QDSP6SS_QMC_SVS_CTL_ADDR                                         (MSS_QDSP6V67SS_PUB_REG_BASE      + 0x00000130)
#define HWIO_MSS_QDSP6SS_QMC_SVS_CTL_RMSK                                                0x3
#define HWIO_MSS_QDSP6SS_QMC_SVS_CTL_IN          \
        in_dword(HWIO_MSS_QDSP6SS_QMC_SVS_CTL_ADDR)
#define HWIO_MSS_QDSP6SS_QMC_SVS_CTL_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_QMC_SVS_CTL_ADDR, m)
#define HWIO_MSS_QDSP6SS_QMC_SVS_CTL_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_QMC_SVS_CTL_ADDR,v)
#define HWIO_MSS_QDSP6SS_QMC_SVS_CTL_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_QMC_SVS_CTL_ADDR,m,v,HWIO_MSS_QDSP6SS_QMC_SVS_CTL_IN)
#define HWIO_MSS_QDSP6SS_QMC_SVS_CTL_QMC_MEM_SVS_SEL_BMSK                                0x2
#define HWIO_MSS_QDSP6SS_QMC_SVS_CTL_QMC_MEM_SVS_SEL_SHFT                                0x1
#define HWIO_MSS_QDSP6SS_QMC_SVS_CTL_QMC_MEM_SVS_BMSK                                    0x1
#define HWIO_MSS_QDSP6SS_QMC_SVS_CTL_QMC_MEM_SVS_SHFT                                    0x0

#define HWIO_MSS_QDSP6SS_CORE_MEM_STAGGER_CTL_ADDR                                (MSS_QDSP6V67SS_PUB_REG_BASE      + 0x00000140)
#define HWIO_MSS_QDSP6SS_CORE_MEM_STAGGER_CTL_RMSK                                       0xf
#define HWIO_MSS_QDSP6SS_CORE_MEM_STAGGER_CTL_IN          \
        in_dword(HWIO_MSS_QDSP6SS_CORE_MEM_STAGGER_CTL_ADDR)
#define HWIO_MSS_QDSP6SS_CORE_MEM_STAGGER_CTL_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_CORE_MEM_STAGGER_CTL_ADDR, m)
#define HWIO_MSS_QDSP6SS_CORE_MEM_STAGGER_CTL_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_CORE_MEM_STAGGER_CTL_ADDR,v)
#define HWIO_MSS_QDSP6SS_CORE_MEM_STAGGER_CTL_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_CORE_MEM_STAGGER_CTL_ADDR,m,v,HWIO_MSS_QDSP6SS_CORE_MEM_STAGGER_CTL_IN)
#define HWIO_MSS_QDSP6SS_CORE_MEM_STAGGER_CTL_DIV_BMSK                                   0xf
#define HWIO_MSS_QDSP6SS_CORE_MEM_STAGGER_CTL_DIV_SHFT                                   0x0

#define HWIO_MSS_QDSP6SS_PLL_MODE_ADDR                                            (MSS_QDSP6V67SS_PUB_REG_BASE      + 0x00000200)
#define HWIO_MSS_QDSP6SS_PLL_MODE_RMSK                                            0xffffffff
#define HWIO_MSS_QDSP6SS_PLL_MODE_IN          \
        in_dword(HWIO_MSS_QDSP6SS_PLL_MODE_ADDR)
#define HWIO_MSS_QDSP6SS_PLL_MODE_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_PLL_MODE_ADDR, m)
#define HWIO_MSS_QDSP6SS_PLL_MODE_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_PLL_MODE_ADDR,v)
#define HWIO_MSS_QDSP6SS_PLL_MODE_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_PLL_MODE_ADDR,m,v,HWIO_MSS_QDSP6SS_PLL_MODE_IN)
#define HWIO_MSS_QDSP6SS_PLL_MODE_PLL_LOCK_DET_BMSK                               0x80000000
#define HWIO_MSS_QDSP6SS_PLL_MODE_PLL_LOCK_DET_SHFT                                     0x1f
#define HWIO_MSS_QDSP6SS_PLL_MODE_PLL_ACTIVE_FLAG_BMSK                            0x40000000
#define HWIO_MSS_QDSP6SS_PLL_MODE_PLL_ACTIVE_FLAG_SHFT                                  0x1e
#define HWIO_MSS_QDSP6SS_PLL_MODE_PLL_ACK_LATCH_BMSK                              0x20000000
#define HWIO_MSS_QDSP6SS_PLL_MODE_PLL_ACK_LATCH_SHFT                                    0x1d
#define HWIO_MSS_QDSP6SS_PLL_MODE_RESERVE_BITS28_24_BMSK                          0x1f000000
#define HWIO_MSS_QDSP6SS_PLL_MODE_RESERVE_BITS28_24_SHFT                                0x18
#define HWIO_MSS_QDSP6SS_PLL_MODE_PLL_HW_UPDATE_LOGIC_BYPASS_BMSK                   0x800000
#define HWIO_MSS_QDSP6SS_PLL_MODE_PLL_HW_UPDATE_LOGIC_BYPASS_SHFT                       0x17
#define HWIO_MSS_QDSP6SS_PLL_MODE_PLL_UPDATE_BMSK                                   0x400000
#define HWIO_MSS_QDSP6SS_PLL_MODE_PLL_UPDATE_SHFT                                       0x16
#define HWIO_MSS_QDSP6SS_PLL_MODE_PLL_VOTE_FSM_RESET_BMSK                           0x200000
#define HWIO_MSS_QDSP6SS_PLL_MODE_PLL_VOTE_FSM_RESET_SHFT                               0x15
#define HWIO_MSS_QDSP6SS_PLL_MODE_PLL_VOTE_FSM_ENA_BMSK                             0x100000
#define HWIO_MSS_QDSP6SS_PLL_MODE_PLL_VOTE_FSM_ENA_SHFT                                 0x14
#define HWIO_MSS_QDSP6SS_PLL_MODE_PLL_BIAS_COUNT_BMSK                                0xfc000
#define HWIO_MSS_QDSP6SS_PLL_MODE_PLL_BIAS_COUNT_SHFT                                    0xe
#define HWIO_MSS_QDSP6SS_PLL_MODE_PLL_LOCK_COUNT_BMSK                                 0x3f00
#define HWIO_MSS_QDSP6SS_PLL_MODE_PLL_LOCK_COUNT_SHFT                                    0x8
#define HWIO_MSS_QDSP6SS_PLL_MODE_RESERVE_BITS7_4_BMSK                                  0xf0
#define HWIO_MSS_QDSP6SS_PLL_MODE_RESERVE_BITS7_4_SHFT                                   0x4
#define HWIO_MSS_QDSP6SS_PLL_MODE_PLL_PLLTEST_BMSK                                       0x8
#define HWIO_MSS_QDSP6SS_PLL_MODE_PLL_PLLTEST_SHFT                                       0x3
#define HWIO_MSS_QDSP6SS_PLL_MODE_PLL_RESET_N_BMSK                                       0x4
#define HWIO_MSS_QDSP6SS_PLL_MODE_PLL_RESET_N_SHFT                                       0x2
#define HWIO_MSS_QDSP6SS_PLL_MODE_PLL_BYPASSNL_BMSK                                      0x2
#define HWIO_MSS_QDSP6SS_PLL_MODE_PLL_BYPASSNL_SHFT                                      0x1
#define HWIO_MSS_QDSP6SS_PLL_MODE_PLL_OUTCTRL_BMSK                                       0x1
#define HWIO_MSS_QDSP6SS_PLL_MODE_PLL_OUTCTRL_SHFT                                       0x0

#define HWIO_MSS_QDSP6SS_PLL_L_VAL_ADDR                                           (MSS_QDSP6V67SS_PUB_REG_BASE      + 0x00000204)
#define HWIO_MSS_QDSP6SS_PLL_L_VAL_RMSK                                               0xffff
#define HWIO_MSS_QDSP6SS_PLL_L_VAL_IN          \
        in_dword(HWIO_MSS_QDSP6SS_PLL_L_VAL_ADDR)
#define HWIO_MSS_QDSP6SS_PLL_L_VAL_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_PLL_L_VAL_ADDR, m)
#define HWIO_MSS_QDSP6SS_PLL_L_VAL_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_PLL_L_VAL_ADDR,v)
#define HWIO_MSS_QDSP6SS_PLL_L_VAL_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_PLL_L_VAL_ADDR,m,v,HWIO_MSS_QDSP6SS_PLL_L_VAL_IN)
#define HWIO_MSS_QDSP6SS_PLL_L_VAL_PLL_L_BMSK                                         0xffff
#define HWIO_MSS_QDSP6SS_PLL_L_VAL_PLL_L_SHFT                                            0x0

#define HWIO_MSS_QDSP6SS_PLL_ALPHA_VAL_ADDR                                       (MSS_QDSP6V67SS_PUB_REG_BASE      + 0x00000208)
#define HWIO_MSS_QDSP6SS_PLL_ALPHA_VAL_RMSK                                       0xffffffff
#define HWIO_MSS_QDSP6SS_PLL_ALPHA_VAL_IN          \
        in_dword(HWIO_MSS_QDSP6SS_PLL_ALPHA_VAL_ADDR)
#define HWIO_MSS_QDSP6SS_PLL_ALPHA_VAL_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_PLL_ALPHA_VAL_ADDR, m)
#define HWIO_MSS_QDSP6SS_PLL_ALPHA_VAL_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_PLL_ALPHA_VAL_ADDR,v)
#define HWIO_MSS_QDSP6SS_PLL_ALPHA_VAL_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_PLL_ALPHA_VAL_ADDR,m,v,HWIO_MSS_QDSP6SS_PLL_ALPHA_VAL_IN)
#define HWIO_MSS_QDSP6SS_PLL_ALPHA_VAL_PLL_ALPHA_31_0_BMSK                        0xffffffff
#define HWIO_MSS_QDSP6SS_PLL_ALPHA_VAL_PLL_ALPHA_31_0_SHFT                               0x0

#define HWIO_MSS_QDSP6SS_PLL_ALPHA_VAL_U_ADDR                                     (MSS_QDSP6V67SS_PUB_REG_BASE      + 0x0000020c)
#define HWIO_MSS_QDSP6SS_PLL_ALPHA_VAL_U_RMSK                                           0xff
#define HWIO_MSS_QDSP6SS_PLL_ALPHA_VAL_U_IN          \
        in_dword(HWIO_MSS_QDSP6SS_PLL_ALPHA_VAL_U_ADDR)
#define HWIO_MSS_QDSP6SS_PLL_ALPHA_VAL_U_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_PLL_ALPHA_VAL_U_ADDR, m)
#define HWIO_MSS_QDSP6SS_PLL_ALPHA_VAL_U_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_PLL_ALPHA_VAL_U_ADDR,v)
#define HWIO_MSS_QDSP6SS_PLL_ALPHA_VAL_U_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_PLL_ALPHA_VAL_U_ADDR,m,v,HWIO_MSS_QDSP6SS_PLL_ALPHA_VAL_U_IN)
#define HWIO_MSS_QDSP6SS_PLL_ALPHA_VAL_U_PLL_ALPHA_39_32_BMSK                           0xff
#define HWIO_MSS_QDSP6SS_PLL_ALPHA_VAL_U_PLL_ALPHA_39_32_SHFT                            0x0

#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_ADDR                                        (MSS_QDSP6V67SS_PUB_REG_BASE      + 0x00000210)
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_RMSK                                        0xffffffff
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_IN          \
        in_dword(HWIO_MSS_QDSP6SS_PLL_USER_CTL_ADDR)
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_PLL_USER_CTL_ADDR, m)
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_PLL_USER_CTL_ADDR,v)
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_PLL_USER_CTL_ADDR,m,v,HWIO_MSS_QDSP6SS_PLL_USER_CTL_IN)
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_RESERVE_BITS31_28_BMSK                      0xf0000000
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_RESERVE_BITS31_28_SHFT                            0x1c
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_SSC_MODE_CONTROL_BMSK                        0x8000000
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_SSC_MODE_CONTROL_SHFT                             0x1b
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_RESERVE_BITS26_25_BMSK                       0x6000000
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_RESERVE_BITS26_25_SHFT                            0x19
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_ALPHA_EN_BMSK                                0x1000000
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_ALPHA_EN_SHFT                                     0x18
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_RESERVE_BITS23_22_BMSK                        0xc00000
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_RESERVE_BITS23_22_SHFT                            0x16
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_VCO_SEL_BMSK                                  0x300000
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_VCO_SEL_SHFT                                      0x14
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_AUX_PLL_OUT_POST_DIV_CTRL_BMSK                 0xf8000
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_AUX_PLL_OUT_POST_DIV_CTRL_SHFT                     0xf
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_PRE_DIV_RATIO_BMSK                              0x7000
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_PRE_DIV_RATIO_SHFT                                 0xc
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_POST_DIV_RATIO_BMSK                              0xf00
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_POST_DIV_RATIO_SHFT                                0x8
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_OUTPUT_INV_BMSK                                   0x80
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_OUTPUT_INV_SHFT                                    0x7
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_RESERVE_BITS6_5_BMSK                              0x60
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_RESERVE_BITS6_5_SHFT                               0x5
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_PLLOUT_LV_TEST_BMSK                               0x10
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_PLLOUT_LV_TEST_SHFT                                0x4
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_PLLOUT_LV_EARLY_BMSK                               0x8
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_PLLOUT_LV_EARLY_SHFT                               0x3
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_PLLOUT_LV_AUX2_BMSK                                0x4
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_PLLOUT_LV_AUX2_SHFT                                0x2
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_PLLOUT_LV_AUX_BMSK                                 0x2
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_PLLOUT_LV_AUX_SHFT                                 0x1
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_PLLOUT_LV_MAIN_BMSK                                0x1
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_PLLOUT_LV_MAIN_SHFT                                0x0

#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_U_ADDR                                      (MSS_QDSP6V67SS_PUB_REG_BASE      + 0x00000214)
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_U_RMSK                                      0xffffffff
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_U_IN          \
        in_dword(HWIO_MSS_QDSP6SS_PLL_USER_CTL_U_ADDR)
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_U_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_PLL_USER_CTL_U_ADDR, m)
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_U_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_PLL_USER_CTL_U_ADDR,v)
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_U_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_PLL_USER_CTL_U_ADDR,m,v,HWIO_MSS_QDSP6SS_PLL_USER_CTL_U_IN)
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_U_CALIBRATION_L_BMSK                        0xffff0000
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_U_CALIBRATION_L_SHFT                              0x10
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_U_RESERVE_BITS15_12_BMSK                        0xf000
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_U_RESERVE_BITS15_12_SHFT                           0xc
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_U_LATCH_INTERFACE_BYPASS_BMSK                    0x800
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_U_LATCH_INTERFACE_BYPASS_SHFT                      0xb
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_U_STATUS_REGISTER_BMSK                           0x700
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_U_STATUS_REGISTER_SHFT                             0x8
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_U_DSM_BMSK                                        0x80
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_U_DSM_SHFT                                         0x7
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_U_WRITE_STATE_BMSK                                0x40
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_U_WRITE_STATE_SHFT                                 0x6
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_U_TARGET_CTL_BMSK                                 0x38
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_U_TARGET_CTL_SHFT                                  0x3
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_U_LOCK_DET_BMSK                                    0x4
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_U_LOCK_DET_SHFT                                    0x2
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_U_FREEZE_PLL_BMSK                                  0x2
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_U_FREEZE_PLL_SHFT                                  0x1
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_U_TOGGLE_DET_BMSK                                  0x1
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_U_TOGGLE_DET_SHFT                                  0x0

#define HWIO_MSS_QDSP6SS_PLL_CONFIG_CTL_ADDR                                      (MSS_QDSP6V67SS_PUB_REG_BASE      + 0x00000218)
#define HWIO_MSS_QDSP6SS_PLL_CONFIG_CTL_RMSK                                      0xffffffff
#define HWIO_MSS_QDSP6SS_PLL_CONFIG_CTL_IN          \
        in_dword(HWIO_MSS_QDSP6SS_PLL_CONFIG_CTL_ADDR)
#define HWIO_MSS_QDSP6SS_PLL_CONFIG_CTL_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_PLL_CONFIG_CTL_ADDR, m)
#define HWIO_MSS_QDSP6SS_PLL_CONFIG_CTL_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_PLL_CONFIG_CTL_ADDR,v)
#define HWIO_MSS_QDSP6SS_PLL_CONFIG_CTL_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_PLL_CONFIG_CTL_ADDR,m,v,HWIO_MSS_QDSP6SS_PLL_CONFIG_CTL_IN)
#define HWIO_MSS_QDSP6SS_PLL_CONFIG_CTL_SINGLE_DMET_MODE_ENABLE_BMSK              0x80000000
#define HWIO_MSS_QDSP6SS_PLL_CONFIG_CTL_SINGLE_DMET_MODE_ENABLE_SHFT                    0x1f
#define HWIO_MSS_QDSP6SS_PLL_CONFIG_CTL_DMET_WINDOW_ENABLE_BMSK                   0x40000000
#define HWIO_MSS_QDSP6SS_PLL_CONFIG_CTL_DMET_WINDOW_ENABLE_SHFT                         0x1e
#define HWIO_MSS_QDSP6SS_PLL_CONFIG_CTL_TOGGLE_DET_SAMPLE_INTER_BMSK              0x3c000000
#define HWIO_MSS_QDSP6SS_PLL_CONFIG_CTL_TOGGLE_DET_SAMPLE_INTER_SHFT                    0x1a
#define HWIO_MSS_QDSP6SS_PLL_CONFIG_CTL_TOGGLE_DET_THRESHOLD_BMSK                  0x3800000
#define HWIO_MSS_QDSP6SS_PLL_CONFIG_CTL_TOGGLE_DET_THRESHOLD_SHFT                       0x17
#define HWIO_MSS_QDSP6SS_PLL_CONFIG_CTL_TOGGLE_DET_SAMPLE_BMSK                      0x700000
#define HWIO_MSS_QDSP6SS_PLL_CONFIG_CTL_TOGGLE_DET_SAMPLE_SHFT                          0x14
#define HWIO_MSS_QDSP6SS_PLL_CONFIG_CTL_LOCK_DET_THRESHOLD_BMSK                      0xff000
#define HWIO_MSS_QDSP6SS_PLL_CONFIG_CTL_LOCK_DET_THRESHOLD_SHFT                          0xc
#define HWIO_MSS_QDSP6SS_PLL_CONFIG_CTL_LOCK_DET_SAMPLE_SIZE_BMSK                      0xf00
#define HWIO_MSS_QDSP6SS_PLL_CONFIG_CTL_LOCK_DET_SAMPLE_SIZE_SHFT                        0x8
#define HWIO_MSS_QDSP6SS_PLL_CONFIG_CTL_GLITCH_THRESHOLD_BMSK                           0xc0
#define HWIO_MSS_QDSP6SS_PLL_CONFIG_CTL_GLITCH_THRESHOLD_SHFT                            0x6
#define HWIO_MSS_QDSP6SS_PLL_CONFIG_CTL_REF_CYCLE_BMSK                                  0x30
#define HWIO_MSS_QDSP6SS_PLL_CONFIG_CTL_REF_CYCLE_SHFT                                   0x4
#define HWIO_MSS_QDSP6SS_PLL_CONFIG_CTL_KFN_BMSK                                         0xf
#define HWIO_MSS_QDSP6SS_PLL_CONFIG_CTL_KFN_SHFT                                         0x0

#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_ADDR                                        (MSS_QDSP6V67SS_PUB_REG_BASE      + 0x0000021c)
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_RMSK                                        0xffffffff
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_IN          \
        in_dword(HWIO_MSS_QDSP6SS_PLL_TEST_CTL_ADDR)
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_PLL_TEST_CTL_ADDR, m)
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_PLL_TEST_CTL_ADDR,v)
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_PLL_TEST_CTL_ADDR,m,v,HWIO_MSS_QDSP6SS_PLL_TEST_CTL_IN)
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_BIAS_GEN_TRIM_BMSK                          0xe0000000
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_BIAS_GEN_TRIM_SHFT                                0x1d
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_DCO_BMSK                                    0x10000000
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_DCO_SHFT                                          0x1c
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_PROCESS_CALB_BMSK                            0xc000000
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_PROCESS_CALB_SHFT                                 0x1a
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_OVERRIDE_PROCESS_CALB_BMSK                   0x2000000
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_OVERRIDE_PROCESS_CALB_SHFT                        0x19
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_FINE_FCW_BMSK                                0x1e00000
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_FINE_FCW_SHFT                                     0x15
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_OVERRIDE_FINE_FCW_BMSK                        0x100000
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_OVERRIDE_FINE_FCW_SHFT                            0x14
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_COARSE_FCW_BMSK                                0xfe000
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_COARSE_FCW_SHFT                                    0xd
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_OVERRIDE_COARSE_BMSK                            0x1000
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_OVERRIDE_COARSE_SHFT                               0xc
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_DISABLE_LFSR_BMSK                                0x800
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_DISABLE_LFSR_SHFT                                  0xb
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_DTEST_SEL_BMSK                                   0x700
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_DTEST_SEL_SHFT                                     0x8
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_DTEST_EN_BMSK                                     0x80
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_DTEST_EN_SHFT                                      0x7
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_BYP_TESTAMP_BMSK                                  0x40
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_BYP_TESTAMP_SHFT                                   0x6
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_ATEST1_SEL_BMSK                                   0x30
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_ATEST1_SEL_SHFT                                    0x4
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_ATEST0_SEL_BMSK                                    0xc
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_ATEST0_SEL_SHFT                                    0x2
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_ATEST1_EN_BMSK                                     0x2
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_ATEST1_EN_SHFT                                     0x1
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_ATEST0_EN_BMSK                                     0x1
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_ATEST0_EN_SHFT                                     0x0

#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_U_ADDR                                      (MSS_QDSP6V67SS_PUB_REG_BASE      + 0x00000220)
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_U_RMSK                                      0xffffffff
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_U_IN          \
        in_dword(HWIO_MSS_QDSP6SS_PLL_TEST_CTL_U_ADDR)
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_U_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_PLL_TEST_CTL_U_ADDR, m)
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_U_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_PLL_TEST_CTL_U_ADDR,v)
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_U_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_PLL_TEST_CTL_U_ADDR,m,v,HWIO_MSS_QDSP6SS_PLL_TEST_CTL_U_IN)
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_U_RESERVE_BITS31_14_BMSK                    0xffffc000
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_U_RESERVE_BITS31_14_SHFT                           0xe
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_U_OVERRIDE_FINE_FCW_MSB_BMSK                    0x2000
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_U_OVERRIDE_FINE_FCW_MSB_SHFT                       0xd
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_U_DTEST_MODE_SEL_BMSK                           0x1800
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_U_DTEST_MODE_SEL_SHFT                              0xb
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_U_NMO_OSC_SEL_BMSK                               0x600
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_U_NMO_OSC_SEL_SHFT                                 0x9
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_U_NMO_EN_BMSK                                    0x100
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_U_NMO_EN_SHFT                                      0x8
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_U_NOISE_MAG_BMSK                                  0xe0
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_U_NOISE_MAG_SHFT                                   0x5
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_U_NOISE_GEN_BMSK                                  0x10
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_U_NOISE_GEN_SHFT                                   0x4
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_U_OSC_BIAS_GND_BMSK                                0x8
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_U_OSC_BIAS_GND_SHFT                                0x3
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_U_PLL_TEST_OUT_SEL_BMSK                            0x6
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_U_PLL_TEST_OUT_SEL_SHFT                            0x1
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_U_CAL_CODE_UPDATE_BMSK                             0x1
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_U_CAL_CODE_UPDATE_SHFT                             0x0

#define HWIO_MSS_QDSP6SS_PLL_STATUS_ADDR                                          (MSS_QDSP6V67SS_PUB_REG_BASE      + 0x00000224)
#define HWIO_MSS_QDSP6SS_PLL_STATUS_RMSK                                          0xffffffff
#define HWIO_MSS_QDSP6SS_PLL_STATUS_IN          \
        in_dword(HWIO_MSS_QDSP6SS_PLL_STATUS_ADDR)
#define HWIO_MSS_QDSP6SS_PLL_STATUS_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_PLL_STATUS_ADDR, m)
#define HWIO_MSS_QDSP6SS_PLL_STATUS_STATUS_31_0_BMSK                              0xffffffff
#define HWIO_MSS_QDSP6SS_PLL_STATUS_STATUS_31_0_SHFT                                     0x0

#define HWIO_MSS_QDSP6SS_PLL_FREQ_CTL_ADDR                                        (MSS_QDSP6V67SS_PUB_REG_BASE      + 0x00000228)
#define HWIO_MSS_QDSP6SS_PLL_FREQ_CTL_RMSK                                        0xffffffff
#define HWIO_MSS_QDSP6SS_PLL_FREQ_CTL_IN          \
        in_dword(HWIO_MSS_QDSP6SS_PLL_FREQ_CTL_ADDR)
#define HWIO_MSS_QDSP6SS_PLL_FREQ_CTL_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_PLL_FREQ_CTL_ADDR, m)
#define HWIO_MSS_QDSP6SS_PLL_FREQ_CTL_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_PLL_FREQ_CTL_ADDR,v)
#define HWIO_MSS_QDSP6SS_PLL_FREQ_CTL_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_PLL_FREQ_CTL_ADDR,m,v,HWIO_MSS_QDSP6SS_PLL_FREQ_CTL_IN)
#define HWIO_MSS_QDSP6SS_PLL_FREQ_CTL_FREQUENCY_CTL_WORD_BMSK                     0xffffffff
#define HWIO_MSS_QDSP6SS_PLL_FREQ_CTL_FREQUENCY_CTL_WORD_SHFT                            0x0

#define HWIO_MSS_QDSP6SS_PLL_RCG_UPDATE_STATUS_ADDR                               (MSS_QDSP6V67SS_PUB_REG_BASE      + 0x00000260)
#define HWIO_MSS_QDSP6SS_PLL_RCG_UPDATE_STATUS_RMSK                                      0x3
#define HWIO_MSS_QDSP6SS_PLL_RCG_UPDATE_STATUS_IN          \
        in_dword(HWIO_MSS_QDSP6SS_PLL_RCG_UPDATE_STATUS_ADDR)
#define HWIO_MSS_QDSP6SS_PLL_RCG_UPDATE_STATUS_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_PLL_RCG_UPDATE_STATUS_ADDR, m)
#define HWIO_MSS_QDSP6SS_PLL_RCG_UPDATE_STATUS_RCG2PLL_UPD_STATUS_BMSK                   0x2
#define HWIO_MSS_QDSP6SS_PLL_RCG_UPDATE_STATUS_RCG2PLL_UPD_STATUS_SHFT                   0x1
#define HWIO_MSS_QDSP6SS_PLL_RCG_UPDATE_STATUS_PLL2RCG_UPD_STATUS_BMSK                   0x1
#define HWIO_MSS_QDSP6SS_PLL_RCG_UPDATE_STATUS_PLL2RCG_UPD_STATUS_SHFT                   0x0

#define HWIO_MSS_QDSP6SS_PLL_RCG_UPDATE_CFG_ADDR                                  (MSS_QDSP6V67SS_PUB_REG_BASE      + 0x00000264)
#define HWIO_MSS_QDSP6SS_PLL_RCG_UPDATE_CFG_RMSK                                         0x3
#define HWIO_MSS_QDSP6SS_PLL_RCG_UPDATE_CFG_IN          \
        in_dword(HWIO_MSS_QDSP6SS_PLL_RCG_UPDATE_CFG_ADDR)
#define HWIO_MSS_QDSP6SS_PLL_RCG_UPDATE_CFG_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_PLL_RCG_UPDATE_CFG_ADDR, m)
#define HWIO_MSS_QDSP6SS_PLL_RCG_UPDATE_CFG_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_PLL_RCG_UPDATE_CFG_ADDR,v)
#define HWIO_MSS_QDSP6SS_PLL_RCG_UPDATE_CFG_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_PLL_RCG_UPDATE_CFG_ADDR,m,v,HWIO_MSS_QDSP6SS_PLL_RCG_UPDATE_CFG_IN)
#define HWIO_MSS_QDSP6SS_PLL_RCG_UPDATE_CFG_RCG2PLL_UPD_EN_BMSK                          0x2
#define HWIO_MSS_QDSP6SS_PLL_RCG_UPDATE_CFG_RCG2PLL_UPD_EN_SHFT                          0x1
#define HWIO_MSS_QDSP6SS_PLL_RCG_UPDATE_CFG_PLL2RCG_UPD_EN_BMSK                          0x1
#define HWIO_MSS_QDSP6SS_PLL_RCG_UPDATE_CFG_PLL2RCG_UPD_EN_SHFT                          0x0

#define HWIO_MSS_QDSP6SS_PLL_RCG_UPDATE_DLYCTL_ADDR                               (MSS_QDSP6V67SS_PUB_REG_BASE      + 0x00000268)
#define HWIO_MSS_QDSP6SS_PLL_RCG_UPDATE_DLYCTL_RMSK                                     0x1f
#define HWIO_MSS_QDSP6SS_PLL_RCG_UPDATE_DLYCTL_IN          \
        in_dword(HWIO_MSS_QDSP6SS_PLL_RCG_UPDATE_DLYCTL_ADDR)
#define HWIO_MSS_QDSP6SS_PLL_RCG_UPDATE_DLYCTL_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_PLL_RCG_UPDATE_DLYCTL_ADDR, m)
#define HWIO_MSS_QDSP6SS_PLL_RCG_UPDATE_DLYCTL_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_PLL_RCG_UPDATE_DLYCTL_ADDR,v)
#define HWIO_MSS_QDSP6SS_PLL_RCG_UPDATE_DLYCTL_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_PLL_RCG_UPDATE_DLYCTL_ADDR,m,v,HWIO_MSS_QDSP6SS_PLL_RCG_UPDATE_DLYCTL_IN)
#define HWIO_MSS_QDSP6SS_PLL_RCG_UPDATE_DLYCTL_MATCH_VALUE_BMSK                         0x1f
#define HWIO_MSS_QDSP6SS_PLL_RCG_UPDATE_DLYCTL_MATCH_VALUE_SHFT                          0x0

#define HWIO_MSS_QDSP6SS_PLL_RCG_UPDATE_CMD_ADDR                                  (MSS_QDSP6V67SS_PUB_REG_BASE      + 0x0000026c)
#define HWIO_MSS_QDSP6SS_PLL_RCG_UPDATE_CMD_RMSK                                         0x1
#define HWIO_MSS_QDSP6SS_PLL_RCG_UPDATE_CMD_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_PLL_RCG_UPDATE_CMD_ADDR,v)
#define HWIO_MSS_QDSP6SS_PLL_RCG_UPDATE_CMD_CMD_BMSK                                     0x1
#define HWIO_MSS_QDSP6SS_PLL_RCG_UPDATE_CMD_CMD_SHFT                                     0x0

#define HWIO_MSS_QDSP6SS_DBG_NMI_CFG_ADDR                                         (MSS_QDSP6V67SS_PUB_REG_BASE      + 0x00000300)
#define HWIO_MSS_QDSP6SS_DBG_NMI_CFG_RMSK                                                0xf
#define HWIO_MSS_QDSP6SS_DBG_NMI_CFG_IN          \
        in_dword(HWIO_MSS_QDSP6SS_DBG_NMI_CFG_ADDR)
#define HWIO_MSS_QDSP6SS_DBG_NMI_CFG_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_DBG_NMI_CFG_ADDR, m)
#define HWIO_MSS_QDSP6SS_DBG_NMI_CFG_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_DBG_NMI_CFG_ADDR,v)
#define HWIO_MSS_QDSP6SS_DBG_NMI_CFG_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_DBG_NMI_CFG_ADDR,m,v,HWIO_MSS_QDSP6SS_DBG_NMI_CFG_IN)
#define HWIO_MSS_QDSP6SS_DBG_NMI_CFG_DBG_NMI_EN_IN_APCR_BMSK                             0x8
#define HWIO_MSS_QDSP6SS_DBG_NMI_CFG_DBG_NMI_EN_IN_APCR_SHFT                             0x3
#define HWIO_MSS_QDSP6SS_DBG_NMI_CFG_DBG_NMI_EN_IN_PWR_COLLAPSE_BMSK                     0x4
#define HWIO_MSS_QDSP6SS_DBG_NMI_CFG_DBG_NMI_EN_IN_PWR_COLLAPSE_SHFT                     0x2
#define HWIO_MSS_QDSP6SS_DBG_NMI_CFG_DBG_NMI_DISABLE_BMSK                                0x2
#define HWIO_MSS_QDSP6SS_DBG_NMI_CFG_DBG_NMI_DISABLE_SHFT                                0x1
#define HWIO_MSS_QDSP6SS_DBG_NMI_CFG_DBG_NMI_WAKEUP_DISABLE_BMSK                         0x1
#define HWIO_MSS_QDSP6SS_DBG_NMI_CFG_DBG_NMI_WAKEUP_DISABLE_SHFT                         0x0

#define HWIO_MSS_QDSP6SS_DBG_NMI_PWR_STATUS_ADDR                                  (MSS_QDSP6V67SS_PUB_REG_BASE      + 0x00000304)
#define HWIO_MSS_QDSP6SS_DBG_NMI_PWR_STATUS_RMSK                                     0x7000f
#define HWIO_MSS_QDSP6SS_DBG_NMI_PWR_STATUS_IN          \
        in_dword(HWIO_MSS_QDSP6SS_DBG_NMI_PWR_STATUS_ADDR)
#define HWIO_MSS_QDSP6SS_DBG_NMI_PWR_STATUS_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_DBG_NMI_PWR_STATUS_ADDR, m)
#define HWIO_MSS_QDSP6SS_DBG_NMI_PWR_STATUS_RSC_IN_SLEEP_BMSK                        0x40000
#define HWIO_MSS_QDSP6SS_DBG_NMI_PWR_STATUS_RSC_IN_SLEEP_SHFT                           0x12
#define HWIO_MSS_QDSP6SS_DBG_NMI_PWR_STATUS_QDSP6_CLK_GATING_BMSK                    0x20000
#define HWIO_MSS_QDSP6SS_DBG_NMI_PWR_STATUS_QDSP6_CLK_GATING_SHFT                       0x11
#define HWIO_MSS_QDSP6SS_DBG_NMI_PWR_STATUS_QDSP6_CORE_STATE_BMSK                    0x10000
#define HWIO_MSS_QDSP6SS_DBG_NMI_PWR_STATUS_QDSP6_CORE_STATE_SHFT                       0x10
#define HWIO_MSS_QDSP6SS_DBG_NMI_PWR_STATUS_NMI_CAUSE_BMSK                               0xc
#define HWIO_MSS_QDSP6SS_DBG_NMI_PWR_STATUS_NMI_CAUSE_SHFT                               0x2
#define HWIO_MSS_QDSP6SS_DBG_NMI_PWR_STATUS_QDSP6_PWR_COLLAPSE_BMSK                      0x2
#define HWIO_MSS_QDSP6SS_DBG_NMI_PWR_STATUS_QDSP6_PWR_COLLAPSE_SHFT                      0x1
#define HWIO_MSS_QDSP6SS_DBG_NMI_PWR_STATUS_QDSP6_IN_SLEEP_BMSK                          0x1
#define HWIO_MSS_QDSP6SS_DBG_NMI_PWR_STATUS_QDSP6_IN_SLEEP_SHFT                          0x0

#define HWIO_MSS_QDSP6SS_BOOT_CORE_START_ADDR                                     (MSS_QDSP6V67SS_PUB_REG_BASE      + 0x00000400)
#define HWIO_MSS_QDSP6SS_BOOT_CORE_START_RMSK                                            0x1
#define HWIO_MSS_QDSP6SS_BOOT_CORE_START_IN          \
        in_dword(HWIO_MSS_QDSP6SS_BOOT_CORE_START_ADDR)
#define HWIO_MSS_QDSP6SS_BOOT_CORE_START_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_BOOT_CORE_START_ADDR, m)
#define HWIO_MSS_QDSP6SS_BOOT_CORE_START_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_BOOT_CORE_START_ADDR,v)
#define HWIO_MSS_QDSP6SS_BOOT_CORE_START_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_BOOT_CORE_START_ADDR,m,v,HWIO_MSS_QDSP6SS_BOOT_CORE_START_IN)
#define HWIO_MSS_QDSP6SS_BOOT_CORE_START_START_BMSK                                      0x1
#define HWIO_MSS_QDSP6SS_BOOT_CORE_START_START_SHFT                                      0x0

#define HWIO_MSS_QDSP6SS_BOOT_CMD_ADDR                                            (MSS_QDSP6V67SS_PUB_REG_BASE      + 0x00000404)
#define HWIO_MSS_QDSP6SS_BOOT_CMD_RMSK                                                   0x1
#define HWIO_MSS_QDSP6SS_BOOT_CMD_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_BOOT_CMD_ADDR,v)
#define HWIO_MSS_QDSP6SS_BOOT_CMD_CMD_BMSK                                               0x1
#define HWIO_MSS_QDSP6SS_BOOT_CMD_CMD_SHFT                                               0x0

#define HWIO_MSS_QDSP6SS_BOOT_STATUS_ADDR                                         (MSS_QDSP6V67SS_PUB_REG_BASE      + 0x00000408)
#define HWIO_MSS_QDSP6SS_BOOT_STATUS_RMSK                                                0x1
#define HWIO_MSS_QDSP6SS_BOOT_STATUS_IN          \
        in_dword(HWIO_MSS_QDSP6SS_BOOT_STATUS_ADDR)
#define HWIO_MSS_QDSP6SS_BOOT_STATUS_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_BOOT_STATUS_ADDR, m)
#define HWIO_MSS_QDSP6SS_BOOT_STATUS_STATUS_BMSK                                         0x1
#define HWIO_MSS_QDSP6SS_BOOT_STATUS_STATUS_SHFT                                         0x0

#define HWIO_MSS_QDSP6SS_CORE_BHS_CFG_ADDR                                        (MSS_QDSP6V67SS_PUB_REG_BASE      + 0x00000410)
#define HWIO_MSS_QDSP6SS_CORE_BHS_CFG_RMSK                                               0x1
#define HWIO_MSS_QDSP6SS_CORE_BHS_CFG_IN          \
        in_dword(HWIO_MSS_QDSP6SS_CORE_BHS_CFG_ADDR)
#define HWIO_MSS_QDSP6SS_CORE_BHS_CFG_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_CORE_BHS_CFG_ADDR, m)
#define HWIO_MSS_QDSP6SS_CORE_BHS_CFG_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_CORE_BHS_CFG_ADDR,v)
#define HWIO_MSS_QDSP6SS_CORE_BHS_CFG_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_CORE_BHS_CFG_ADDR,m,v,HWIO_MSS_QDSP6SS_CORE_BHS_CFG_IN)
#define HWIO_MSS_QDSP6SS_CORE_BHS_CFG_BHS_ON_BMSK                                        0x1
#define HWIO_MSS_QDSP6SS_CORE_BHS_CFG_BHS_ON_SHFT                                        0x0

#define HWIO_MSS_QDSP6SS_CORE_BHS_CMD_ADDR                                        (MSS_QDSP6V67SS_PUB_REG_BASE      + 0x00000414)
#define HWIO_MSS_QDSP6SS_CORE_BHS_CMD_RMSK                                               0x1
#define HWIO_MSS_QDSP6SS_CORE_BHS_CMD_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_CORE_BHS_CMD_ADDR,v)
#define HWIO_MSS_QDSP6SS_CORE_BHS_CMD_UPDATE_BMSK                                        0x1
#define HWIO_MSS_QDSP6SS_CORE_BHS_CMD_UPDATE_SHFT                                        0x0

#define HWIO_MSS_QDSP6SS_CORE_BHS_STATUS_ADDR                                     (MSS_QDSP6V67SS_PUB_REG_BASE      + 0x00000418)
#define HWIO_MSS_QDSP6SS_CORE_BHS_STATUS_RMSK                                            0x1
#define HWIO_MSS_QDSP6SS_CORE_BHS_STATUS_IN          \
        in_dword(HWIO_MSS_QDSP6SS_CORE_BHS_STATUS_ADDR)
#define HWIO_MSS_QDSP6SS_CORE_BHS_STATUS_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_CORE_BHS_STATUS_ADDR, m)
#define HWIO_MSS_QDSP6SS_CORE_BHS_STATUS_BHS_ON_BMSK                                     0x1
#define HWIO_MSS_QDSP6SS_CORE_BHS_STATUS_BHS_ON_SHFT                                     0x0

#define HWIO_MSS_QDSP6SS_LDO_CTL_CFG_ADDR                                         (MSS_QDSP6V67SS_PUB_REG_BASE      + 0x00000420)
#define HWIO_MSS_QDSP6SS_LDO_CTL_CFG_RMSK                                                0x7
#define HWIO_MSS_QDSP6SS_LDO_CTL_CFG_IN          \
        in_dword(HWIO_MSS_QDSP6SS_LDO_CTL_CFG_ADDR)
#define HWIO_MSS_QDSP6SS_LDO_CTL_CFG_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_LDO_CTL_CFG_ADDR, m)
#define HWIO_MSS_QDSP6SS_LDO_CTL_CFG_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_LDO_CTL_CFG_ADDR,v)
#define HWIO_MSS_QDSP6SS_LDO_CTL_CFG_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_LDO_CTL_CFG_ADDR,m,v,HWIO_MSS_QDSP6SS_LDO_CTL_CFG_IN)
#define HWIO_MSS_QDSP6SS_LDO_CTL_CFG_LDO_PWR_UP_BG_BMSK                                  0x4
#define HWIO_MSS_QDSP6SS_LDO_CTL_CFG_LDO_PWR_UP_BG_SHFT                                  0x2
#define HWIO_MSS_QDSP6SS_LDO_CTL_CFG_LDO_PWR_UP_BMSK                                     0x2
#define HWIO_MSS_QDSP6SS_LDO_CTL_CFG_LDO_PWR_UP_SHFT                                     0x1
#define HWIO_MSS_QDSP6SS_LDO_CTL_CFG_LDO_BYP_BMSK                                        0x1
#define HWIO_MSS_QDSP6SS_LDO_CTL_CFG_LDO_BYP_SHFT                                        0x0

#define HWIO_MSS_QDSP6SS_LDO_CTL_CMD_ADDR                                         (MSS_QDSP6V67SS_PUB_REG_BASE      + 0x00000424)
#define HWIO_MSS_QDSP6SS_LDO_CTL_CMD_RMSK                                                0x7
#define HWIO_MSS_QDSP6SS_LDO_CTL_CMD_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_LDO_CTL_CMD_ADDR,v)
#define HWIO_MSS_QDSP6SS_LDO_CTL_CMD_LDO_PWR_UP_BG_UPDATE_BMSK                           0x4
#define HWIO_MSS_QDSP6SS_LDO_CTL_CMD_LDO_PWR_UP_BG_UPDATE_SHFT                           0x2
#define HWIO_MSS_QDSP6SS_LDO_CTL_CMD_LDO_PWR_UP_UPDATE_BMSK                              0x2
#define HWIO_MSS_QDSP6SS_LDO_CTL_CMD_LDO_PWR_UP_UPDATE_SHFT                              0x1
#define HWIO_MSS_QDSP6SS_LDO_CTL_CMD_LDO_BYP_UPDATE_BMSK                                 0x1
#define HWIO_MSS_QDSP6SS_LDO_CTL_CMD_LDO_BYP_UPDATE_SHFT                                 0x0

#define HWIO_MSS_QDSP6SS_LDO_CTL_STATUS_ADDR                                      (MSS_QDSP6V67SS_PUB_REG_BASE      + 0x00000428)
#define HWIO_MSS_QDSP6SS_LDO_CTL_STATUS_RMSK                                             0x7
#define HWIO_MSS_QDSP6SS_LDO_CTL_STATUS_IN          \
        in_dword(HWIO_MSS_QDSP6SS_LDO_CTL_STATUS_ADDR)
#define HWIO_MSS_QDSP6SS_LDO_CTL_STATUS_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_LDO_CTL_STATUS_ADDR, m)
#define HWIO_MSS_QDSP6SS_LDO_CTL_STATUS_LDO_PWR_UP_BG_BMSK                               0x4
#define HWIO_MSS_QDSP6SS_LDO_CTL_STATUS_LDO_PWR_UP_BG_SHFT                               0x2
#define HWIO_MSS_QDSP6SS_LDO_CTL_STATUS_LDO_PWR_UP_BMSK                                  0x2
#define HWIO_MSS_QDSP6SS_LDO_CTL_STATUS_LDO_PWR_UP_SHFT                                  0x1
#define HWIO_MSS_QDSP6SS_LDO_CTL_STATUS_LDO_BYP_BMSK                                     0x1
#define HWIO_MSS_QDSP6SS_LDO_CTL_STATUS_LDO_BYP_SHFT                                     0x0

#define HWIO_MSS_QDSP6SS_MEM_CFG_ADDR                                             (MSS_QDSP6V67SS_PUB_REG_BASE      + 0x00000430)
#define HWIO_MSS_QDSP6SS_MEM_CFG_RMSK                                               0x1f001f
#define HWIO_MSS_QDSP6SS_MEM_CFG_IN          \
        in_dword(HWIO_MSS_QDSP6SS_MEM_CFG_ADDR)
#define HWIO_MSS_QDSP6SS_MEM_CFG_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_MEM_CFG_ADDR, m)
#define HWIO_MSS_QDSP6SS_MEM_CFG_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_MEM_CFG_ADDR,v)
#define HWIO_MSS_QDSP6SS_MEM_CFG_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_MEM_CFG_ADDR,m,v,HWIO_MSS_QDSP6SS_MEM_CFG_IN)
#define HWIO_MSS_QDSP6SS_MEM_CFG_L2MEM_SLP_RET_N_BMSK                               0x1e0000
#define HWIO_MSS_QDSP6SS_MEM_CFG_L2MEM_SLP_RET_N_SHFT                                   0x11
#define HWIO_MSS_QDSP6SS_MEM_CFG_MEM_SLP_RET_N_BMSK                                  0x10000
#define HWIO_MSS_QDSP6SS_MEM_CFG_MEM_SLP_RET_N_SHFT                                     0x10
#define HWIO_MSS_QDSP6SS_MEM_CFG_L2MEM_SLP_NRET_N_BMSK                                  0x1e
#define HWIO_MSS_QDSP6SS_MEM_CFG_L2MEM_SLP_NRET_N_SHFT                                   0x1
#define HWIO_MSS_QDSP6SS_MEM_CFG_MEM_SLP_NRET_N_BMSK                                     0x1
#define HWIO_MSS_QDSP6SS_MEM_CFG_MEM_SLP_NRET_N_SHFT                                     0x0

#define HWIO_MSS_QDSP6SS_MEM_CMD_ADDR                                             (MSS_QDSP6V67SS_PUB_REG_BASE      + 0x00000434)
#define HWIO_MSS_QDSP6SS_MEM_CMD_RMSK                                                    0x3
#define HWIO_MSS_QDSP6SS_MEM_CMD_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_MEM_CMD_ADDR,v)
#define HWIO_MSS_QDSP6SS_MEM_CMD_UPDATE_SLP_RET_N_BMSK                                   0x2
#define HWIO_MSS_QDSP6SS_MEM_CMD_UPDATE_SLP_RET_N_SHFT                                   0x1
#define HWIO_MSS_QDSP6SS_MEM_CMD_UPDATE_SLP_NRET_N_BMSK                                  0x1
#define HWIO_MSS_QDSP6SS_MEM_CMD_UPDATE_SLP_NRET_N_SHFT                                  0x0

#define HWIO_MSS_QDSP6SS_MEM_STATUS_ADDR                                          (MSS_QDSP6V67SS_PUB_REG_BASE      + 0x00000438)
#define HWIO_MSS_QDSP6SS_MEM_STATUS_RMSK                                            0x1f001f
#define HWIO_MSS_QDSP6SS_MEM_STATUS_IN          \
        in_dword(HWIO_MSS_QDSP6SS_MEM_STATUS_ADDR)
#define HWIO_MSS_QDSP6SS_MEM_STATUS_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_MEM_STATUS_ADDR, m)
#define HWIO_MSS_QDSP6SS_MEM_STATUS_L2MEM_SLP_RET_N_BMSK                            0x1e0000
#define HWIO_MSS_QDSP6SS_MEM_STATUS_L2MEM_SLP_RET_N_SHFT                                0x11
#define HWIO_MSS_QDSP6SS_MEM_STATUS_MEM_SLP_RET_N_BMSK                               0x10000
#define HWIO_MSS_QDSP6SS_MEM_STATUS_MEM_SLP_RET_N_SHFT                                  0x10
#define HWIO_MSS_QDSP6SS_MEM_STATUS_L2MEM_SLP_NRET_N_BMSK                               0x1e
#define HWIO_MSS_QDSP6SS_MEM_STATUS_L2MEM_SLP_NRET_N_SHFT                                0x1
#define HWIO_MSS_QDSP6SS_MEM_STATUS_MEM_SLP_NRET_N_BMSK                                  0x1
#define HWIO_MSS_QDSP6SS_MEM_STATUS_MEM_SLP_NRET_N_SHFT                                  0x0

#define HWIO_MSS_QDSP6SS_RESET_CFG_ADDR                                           (MSS_QDSP6V67SS_PUB_REG_BASE      + 0x00000440)
#define HWIO_MSS_QDSP6SS_RESET_CFG_RMSK                                              0x1ffff
#define HWIO_MSS_QDSP6SS_RESET_CFG_IN          \
        in_dword(HWIO_MSS_QDSP6SS_RESET_CFG_ADDR)
#define HWIO_MSS_QDSP6SS_RESET_CFG_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_RESET_CFG_ADDR, m)
#define HWIO_MSS_QDSP6SS_RESET_CFG_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_RESET_CFG_ADDR,v)
#define HWIO_MSS_QDSP6SS_RESET_CFG_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_RESET_CFG_ADDR,m,v,HWIO_MSS_QDSP6SS_RESET_CFG_IN)
#define HWIO_MSS_QDSP6SS_RESET_CFG_CLADE2_BMSK                                       0x10000
#define HWIO_MSS_QDSP6SS_RESET_CFG_CLADE2_SHFT                                          0x10
#define HWIO_MSS_QDSP6SS_RESET_CFG_VPEVU_BMSK                                         0x8000
#define HWIO_MSS_QDSP6SS_RESET_CFG_VPEVU_SHFT                                            0xf
#define HWIO_MSS_QDSP6SS_RESET_CFG_NRET_BMSK                                          0x4000
#define HWIO_MSS_QDSP6SS_RESET_CFG_NRET_SHFT                                             0xe
#define HWIO_MSS_QDSP6SS_RESET_CFG_MNOC_BMSK                                          0x2000
#define HWIO_MSS_QDSP6SS_RESET_CFG_MNOC_SHFT                                             0xd
#define HWIO_MSS_QDSP6SS_RESET_CFG_APB_BMSK                                           0x1000
#define HWIO_MSS_QDSP6SS_RESET_CFG_APB_SHFT                                              0xc
#define HWIO_MSS_QDSP6SS_RESET_CFG_AXIS2_BMSK                                          0x800
#define HWIO_MSS_QDSP6SS_RESET_CFG_AXIS2_SHFT                                            0xb
#define HWIO_MSS_QDSP6SS_RESET_CFG_CSI1_BMSK                                           0x400
#define HWIO_MSS_QDSP6SS_RESET_CFG_CSI1_SHFT                                             0xa
#define HWIO_MSS_QDSP6SS_RESET_CFG_CSI0_BMSK                                           0x200
#define HWIO_MSS_QDSP6SS_RESET_CFG_CSI0_SHFT                                             0x9
#define HWIO_MSS_QDSP6SS_RESET_CFG_ARCH_BMSK                                           0x100
#define HWIO_MSS_QDSP6SS_RESET_CFG_ARCH_SHFT                                             0x8
#define HWIO_MSS_QDSP6SS_RESET_CFG_CORE_BMSK                                            0x80
#define HWIO_MSS_QDSP6SS_RESET_CFG_CORE_SHFT                                             0x7
#define HWIO_MSS_QDSP6SS_RESET_CFG_AXIM2_BMSK                                           0x40
#define HWIO_MSS_QDSP6SS_RESET_CFG_AXIM2_SHFT                                            0x6
#define HWIO_MSS_QDSP6SS_RESET_CFG_AXIS_BMSK                                            0x20
#define HWIO_MSS_QDSP6SS_RESET_CFG_AXIS_SHFT                                             0x5
#define HWIO_MSS_QDSP6SS_RESET_CFG_ATBM_BMSK                                            0x10
#define HWIO_MSS_QDSP6SS_RESET_CFG_ATBM_SHFT                                             0x4
#define HWIO_MSS_QDSP6SS_RESET_CFG_AHBM_BMSK                                             0x8
#define HWIO_MSS_QDSP6SS_RESET_CFG_AHBM_SHFT                                             0x3
#define HWIO_MSS_QDSP6SS_RESET_CFG_AXIM_BMSK                                             0x4
#define HWIO_MSS_QDSP6SS_RESET_CFG_AXIM_SHFT                                             0x2
#define HWIO_MSS_QDSP6SS_RESET_CFG_ISDB_ETM_BMSK                                         0x2
#define HWIO_MSS_QDSP6SS_RESET_CFG_ISDB_ETM_SHFT                                         0x1
#define HWIO_MSS_QDSP6SS_RESET_CFG_RET_BMSK                                              0x1
#define HWIO_MSS_QDSP6SS_RESET_CFG_RET_SHFT                                              0x0

#define HWIO_MSS_QDSP6SS_RESET_CMD_ADDR                                           (MSS_QDSP6V67SS_PUB_REG_BASE      + 0x00000444)
#define HWIO_MSS_QDSP6SS_RESET_CMD_RMSK                                                  0x1
#define HWIO_MSS_QDSP6SS_RESET_CMD_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_RESET_CMD_ADDR,v)
#define HWIO_MSS_QDSP6SS_RESET_CMD_UPDATE_BMSK                                           0x1
#define HWIO_MSS_QDSP6SS_RESET_CMD_UPDATE_SHFT                                           0x0

#define HWIO_MSS_QDSP6SS_RESET_STATUS_ADDR                                        (MSS_QDSP6V67SS_PUB_REG_BASE      + 0x00000448)
#define HWIO_MSS_QDSP6SS_RESET_STATUS_RMSK                                           0x1ffff
#define HWIO_MSS_QDSP6SS_RESET_STATUS_IN          \
        in_dword(HWIO_MSS_QDSP6SS_RESET_STATUS_ADDR)
#define HWIO_MSS_QDSP6SS_RESET_STATUS_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_RESET_STATUS_ADDR, m)
#define HWIO_MSS_QDSP6SS_RESET_STATUS_CLADE2_BMSK                                    0x10000
#define HWIO_MSS_QDSP6SS_RESET_STATUS_CLADE2_SHFT                                       0x10
#define HWIO_MSS_QDSP6SS_RESET_STATUS_VPEVU_BMSK                                      0x8000
#define HWIO_MSS_QDSP6SS_RESET_STATUS_VPEVU_SHFT                                         0xf
#define HWIO_MSS_QDSP6SS_RESET_STATUS_NRET_BMSK                                       0x4000
#define HWIO_MSS_QDSP6SS_RESET_STATUS_NRET_SHFT                                          0xe
#define HWIO_MSS_QDSP6SS_RESET_STATUS_MNOC_BMSK                                       0x2000
#define HWIO_MSS_QDSP6SS_RESET_STATUS_MNOC_SHFT                                          0xd
#define HWIO_MSS_QDSP6SS_RESET_STATUS_APB_BMSK                                        0x1000
#define HWIO_MSS_QDSP6SS_RESET_STATUS_APB_SHFT                                           0xc
#define HWIO_MSS_QDSP6SS_RESET_STATUS_AXIS2_BMSK                                       0x800
#define HWIO_MSS_QDSP6SS_RESET_STATUS_AXIS2_SHFT                                         0xb
#define HWIO_MSS_QDSP6SS_RESET_STATUS_CSI1_BMSK                                        0x400
#define HWIO_MSS_QDSP6SS_RESET_STATUS_CSI1_SHFT                                          0xa
#define HWIO_MSS_QDSP6SS_RESET_STATUS_CSI0_BMSK                                        0x200
#define HWIO_MSS_QDSP6SS_RESET_STATUS_CSI0_SHFT                                          0x9
#define HWIO_MSS_QDSP6SS_RESET_STATUS_ARCH_BMSK                                        0x100
#define HWIO_MSS_QDSP6SS_RESET_STATUS_ARCH_SHFT                                          0x8
#define HWIO_MSS_QDSP6SS_RESET_STATUS_CORE_BMSK                                         0x80
#define HWIO_MSS_QDSP6SS_RESET_STATUS_CORE_SHFT                                          0x7
#define HWIO_MSS_QDSP6SS_RESET_STATUS_AXIM2_BMSK                                        0x40
#define HWIO_MSS_QDSP6SS_RESET_STATUS_AXIM2_SHFT                                         0x6
#define HWIO_MSS_QDSP6SS_RESET_STATUS_AXIS_BMSK                                         0x20
#define HWIO_MSS_QDSP6SS_RESET_STATUS_AXIS_SHFT                                          0x5
#define HWIO_MSS_QDSP6SS_RESET_STATUS_ATBM_BMSK                                         0x10
#define HWIO_MSS_QDSP6SS_RESET_STATUS_ATBM_SHFT                                          0x4
#define HWIO_MSS_QDSP6SS_RESET_STATUS_AHBM_BMSK                                          0x8
#define HWIO_MSS_QDSP6SS_RESET_STATUS_AHBM_SHFT                                          0x3
#define HWIO_MSS_QDSP6SS_RESET_STATUS_AXIM_BMSK                                          0x4
#define HWIO_MSS_QDSP6SS_RESET_STATUS_AXIM_SHFT                                          0x2
#define HWIO_MSS_QDSP6SS_RESET_STATUS_ISDB_ETM_BMSK                                      0x2
#define HWIO_MSS_QDSP6SS_RESET_STATUS_ISDB_ETM_SHFT                                      0x1
#define HWIO_MSS_QDSP6SS_RESET_STATUS_RET_BMSK                                           0x1
#define HWIO_MSS_QDSP6SS_RESET_STATUS_RET_SHFT                                           0x0

#define HWIO_MSS_QDSP6SS_CLAMP_CFG_ADDR                                           (MSS_QDSP6V67SS_PUB_REG_BASE      + 0x00000450)
#define HWIO_MSS_QDSP6SS_CLAMP_CFG_RMSK                                                  0xd
#define HWIO_MSS_QDSP6SS_CLAMP_CFG_IN          \
        in_dword(HWIO_MSS_QDSP6SS_CLAMP_CFG_ADDR)
#define HWIO_MSS_QDSP6SS_CLAMP_CFG_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_CLAMP_CFG_ADDR, m)
#define HWIO_MSS_QDSP6SS_CLAMP_CFG_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_CLAMP_CFG_ADDR,v)
#define HWIO_MSS_QDSP6SS_CLAMP_CFG_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_CLAMP_CFG_ADDR,m,v,HWIO_MSS_QDSP6SS_CLAMP_CFG_IN)
#define HWIO_MSS_QDSP6SS_CLAMP_CFG_CLAMP_MSS2CX_BMSK                                     0x8
#define HWIO_MSS_QDSP6SS_CLAMP_CFG_CLAMP_MSS2CX_SHFT                                     0x3
#define HWIO_MSS_QDSP6SS_CLAMP_CFG_CLAMP_QMC_MEM_BMSK                                    0x4
#define HWIO_MSS_QDSP6SS_CLAMP_CFG_CLAMP_QMC_MEM_SHFT                                    0x2
#define HWIO_MSS_QDSP6SS_CLAMP_CFG_CLAMP_IO_BMSK                                         0x1
#define HWIO_MSS_QDSP6SS_CLAMP_CFG_CLAMP_IO_SHFT                                         0x0

#define HWIO_MSS_QDSP6SS_CLAMP_CMD_ADDR                                           (MSS_QDSP6V67SS_PUB_REG_BASE      + 0x00000454)
#define HWIO_MSS_QDSP6SS_CLAMP_CMD_RMSK                                                  0xd
#define HWIO_MSS_QDSP6SS_CLAMP_CMD_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_CLAMP_CMD_ADDR,v)
#define HWIO_MSS_QDSP6SS_CLAMP_CMD_UPDATE_CLAMP_MSS2CX_BMSK                              0x8
#define HWIO_MSS_QDSP6SS_CLAMP_CMD_UPDATE_CLAMP_MSS2CX_SHFT                              0x3
#define HWIO_MSS_QDSP6SS_CLAMP_CMD_UPDATE_CLAMP_QMC_MEM_BMSK                             0x4
#define HWIO_MSS_QDSP6SS_CLAMP_CMD_UPDATE_CLAMP_QMC_MEM_SHFT                             0x2
#define HWIO_MSS_QDSP6SS_CLAMP_CMD_UPDATE_CLAMP_IO_BMSK                                  0x1
#define HWIO_MSS_QDSP6SS_CLAMP_CMD_UPDATE_CLAMP_IO_SHFT                                  0x0

#define HWIO_MSS_QDSP6SS_CLAMP_STATUS_ADDR                                        (MSS_QDSP6V67SS_PUB_REG_BASE      + 0x00000458)
#define HWIO_MSS_QDSP6SS_CLAMP_STATUS_RMSK                                               0xd
#define HWIO_MSS_QDSP6SS_CLAMP_STATUS_IN          \
        in_dword(HWIO_MSS_QDSP6SS_CLAMP_STATUS_ADDR)
#define HWIO_MSS_QDSP6SS_CLAMP_STATUS_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_CLAMP_STATUS_ADDR, m)
#define HWIO_MSS_QDSP6SS_CLAMP_STATUS_CLAMP_MSS2CX_BMSK                                  0x8
#define HWIO_MSS_QDSP6SS_CLAMP_STATUS_CLAMP_MSS2CX_SHFT                                  0x3
#define HWIO_MSS_QDSP6SS_CLAMP_STATUS_CLAMP_QMC_MEM_BMSK                                 0x4
#define HWIO_MSS_QDSP6SS_CLAMP_STATUS_CLAMP_QMC_MEM_SHFT                                 0x2
#define HWIO_MSS_QDSP6SS_CLAMP_STATUS_CLAMP_IO_BMSK                                      0x1
#define HWIO_MSS_QDSP6SS_CLAMP_STATUS_CLAMP_IO_SHFT                                      0x0

#define HWIO_MSS_QDSP6SS_CLK_CFG_ADDR                                             (MSS_QDSP6V67SS_PUB_REG_BASE      + 0x00000460)
#define HWIO_MSS_QDSP6SS_CLK_CFG_RMSK                                                  0x7ff
#define HWIO_MSS_QDSP6SS_CLK_CFG_IN          \
        in_dword(HWIO_MSS_QDSP6SS_CLK_CFG_ADDR)
#define HWIO_MSS_QDSP6SS_CLK_CFG_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_CLK_CFG_ADDR, m)
#define HWIO_MSS_QDSP6SS_CLK_CFG_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_CLK_CFG_ADDR,v)
#define HWIO_MSS_QDSP6SS_CLK_CFG_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_CLK_CFG_ADDR,m,v,HWIO_MSS_QDSP6SS_CLK_CFG_IN)
#define HWIO_MSS_QDSP6SS_CLK_CFG_VPEVU_BMSK                                            0x400
#define HWIO_MSS_QDSP6SS_CLK_CFG_VPEVU_SHFT                                              0xa
#define HWIO_MSS_QDSP6SS_CLK_CFG_AXIS2_BMSK                                            0x200
#define HWIO_MSS_QDSP6SS_CLK_CFG_AXIS2_SHFT                                              0x9
#define HWIO_MSS_QDSP6SS_CLK_CFG_CSI1_BMSK                                             0x100
#define HWIO_MSS_QDSP6SS_CLK_CFG_CSI1_SHFT                                               0x8
#define HWIO_MSS_QDSP6SS_CLK_CFG_CSI0_BMSK                                              0x80
#define HWIO_MSS_QDSP6SS_CLK_CFG_CSI0_SHFT                                               0x7
#define HWIO_MSS_QDSP6SS_CLK_CFG_AXIM2_BMSK                                             0x40
#define HWIO_MSS_QDSP6SS_CLK_CFG_AXIM2_SHFT                                              0x6
#define HWIO_MSS_QDSP6SS_CLK_CFG_AXIS_BMSK                                              0x20
#define HWIO_MSS_QDSP6SS_CLK_CFG_AXIS_SHFT                                               0x5
#define HWIO_MSS_QDSP6SS_CLK_CFG_ATBM_BMSK                                              0x10
#define HWIO_MSS_QDSP6SS_CLK_CFG_ATBM_SHFT                                               0x4
#define HWIO_MSS_QDSP6SS_CLK_CFG_AHBM_BMSK                                               0x8
#define HWIO_MSS_QDSP6SS_CLK_CFG_AHBM_SHFT                                               0x3
#define HWIO_MSS_QDSP6SS_CLK_CFG_AXIM_BMSK                                               0x4
#define HWIO_MSS_QDSP6SS_CLK_CFG_AXIM_SHFT                                               0x2
#define HWIO_MSS_QDSP6SS_CLK_CFG_APB_BMSK                                                0x2
#define HWIO_MSS_QDSP6SS_CLK_CFG_APB_SHFT                                                0x1
#define HWIO_MSS_QDSP6SS_CLK_CFG_CORE_BMSK                                               0x1
#define HWIO_MSS_QDSP6SS_CLK_CFG_CORE_SHFT                                               0x0

#define HWIO_MSS_QDSP6SS_CLK_CMD_ADDR                                             (MSS_QDSP6V67SS_PUB_REG_BASE      + 0x00000464)
#define HWIO_MSS_QDSP6SS_CLK_CMD_RMSK                                                    0x1
#define HWIO_MSS_QDSP6SS_CLK_CMD_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_CLK_CMD_ADDR,v)
#define HWIO_MSS_QDSP6SS_CLK_CMD_UPDATE_CLK_BMSK                                         0x1
#define HWIO_MSS_QDSP6SS_CLK_CMD_UPDATE_CLK_SHFT                                         0x0

#define HWIO_MSS_QDSP6SS_CLK_STATUS_ADDR                                          (MSS_QDSP6V67SS_PUB_REG_BASE      + 0x00000468)
#define HWIO_MSS_QDSP6SS_CLK_STATUS_RMSK                                               0x7ff
#define HWIO_MSS_QDSP6SS_CLK_STATUS_IN          \
        in_dword(HWIO_MSS_QDSP6SS_CLK_STATUS_ADDR)
#define HWIO_MSS_QDSP6SS_CLK_STATUS_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_CLK_STATUS_ADDR, m)
#define HWIO_MSS_QDSP6SS_CLK_STATUS_VPEVU_BMSK                                         0x400
#define HWIO_MSS_QDSP6SS_CLK_STATUS_VPEVU_SHFT                                           0xa
#define HWIO_MSS_QDSP6SS_CLK_STATUS_AXIS2_BMSK                                         0x200
#define HWIO_MSS_QDSP6SS_CLK_STATUS_AXIS2_SHFT                                           0x9
#define HWIO_MSS_QDSP6SS_CLK_STATUS_CSI1_BMSK                                          0x100
#define HWIO_MSS_QDSP6SS_CLK_STATUS_CSI1_SHFT                                            0x8
#define HWIO_MSS_QDSP6SS_CLK_STATUS_CSI0_BMSK                                           0x80
#define HWIO_MSS_QDSP6SS_CLK_STATUS_CSI0_SHFT                                            0x7
#define HWIO_MSS_QDSP6SS_CLK_STATUS_AXIM2_BMSK                                          0x40
#define HWIO_MSS_QDSP6SS_CLK_STATUS_AXIM2_SHFT                                           0x6
#define HWIO_MSS_QDSP6SS_CLK_STATUS_AXIS_BMSK                                           0x20
#define HWIO_MSS_QDSP6SS_CLK_STATUS_AXIS_SHFT                                            0x5
#define HWIO_MSS_QDSP6SS_CLK_STATUS_ATBM_BMSK                                           0x10
#define HWIO_MSS_QDSP6SS_CLK_STATUS_ATBM_SHFT                                            0x4
#define HWIO_MSS_QDSP6SS_CLK_STATUS_AHBM_BMSK                                            0x8
#define HWIO_MSS_QDSP6SS_CLK_STATUS_AHBM_SHFT                                            0x3
#define HWIO_MSS_QDSP6SS_CLK_STATUS_AXIM_BMSK                                            0x4
#define HWIO_MSS_QDSP6SS_CLK_STATUS_AXIM_SHFT                                            0x2
#define HWIO_MSS_QDSP6SS_CLK_STATUS_APB_BMSK                                             0x2
#define HWIO_MSS_QDSP6SS_CLK_STATUS_APB_SHFT                                             0x1
#define HWIO_MSS_QDSP6SS_CLK_STATUS_CORE_BMSK                                            0x1
#define HWIO_MSS_QDSP6SS_CLK_STATUS_CORE_SHFT                                            0x0

#define HWIO_MSS_QDSP6SS_MEM_STAGGER_RESET_CFG_ADDR                               (MSS_QDSP6V67SS_PUB_REG_BASE      + 0x00000470)
#define HWIO_MSS_QDSP6SS_MEM_STAGGER_RESET_CFG_RMSK                                      0x3
#define HWIO_MSS_QDSP6SS_MEM_STAGGER_RESET_CFG_IN          \
        in_dword(HWIO_MSS_QDSP6SS_MEM_STAGGER_RESET_CFG_ADDR)
#define HWIO_MSS_QDSP6SS_MEM_STAGGER_RESET_CFG_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_MEM_STAGGER_RESET_CFG_ADDR, m)
#define HWIO_MSS_QDSP6SS_MEM_STAGGER_RESET_CFG_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_MEM_STAGGER_RESET_CFG_ADDR,v)
#define HWIO_MSS_QDSP6SS_MEM_STAGGER_RESET_CFG_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_MEM_STAGGER_RESET_CFG_ADDR,m,v,HWIO_MSS_QDSP6SS_MEM_STAGGER_RESET_CFG_IN)
#define HWIO_MSS_QDSP6SS_MEM_STAGGER_RESET_CFG_STAGGER_ALT_ARES_BMSK                     0x2
#define HWIO_MSS_QDSP6SS_MEM_STAGGER_RESET_CFG_STAGGER_ALT_ARES_SHFT                     0x1
#define HWIO_MSS_QDSP6SS_MEM_STAGGER_RESET_CFG_STAGGER_FUNC_ARES_BMSK                    0x1
#define HWIO_MSS_QDSP6SS_MEM_STAGGER_RESET_CFG_STAGGER_FUNC_ARES_SHFT                    0x0

#define HWIO_MSS_QDSP6SS_MEM_STAGGER_RESET_CMD_ADDR                               (MSS_QDSP6V67SS_PUB_REG_BASE      + 0x00000474)
#define HWIO_MSS_QDSP6SS_MEM_STAGGER_RESET_CMD_RMSK                                      0x1
#define HWIO_MSS_QDSP6SS_MEM_STAGGER_RESET_CMD_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_MEM_STAGGER_RESET_CMD_ADDR,v)
#define HWIO_MSS_QDSP6SS_MEM_STAGGER_RESET_CMD_UPDATE_BMSK                               0x1
#define HWIO_MSS_QDSP6SS_MEM_STAGGER_RESET_CMD_UPDATE_SHFT                               0x0

#define HWIO_MSS_QDSP6SS_MEM_STAGGER_RESET_STATUS_ADDR                            (MSS_QDSP6V67SS_PUB_REG_BASE      + 0x00000478)
#define HWIO_MSS_QDSP6SS_MEM_STAGGER_RESET_STATUS_RMSK                                   0x3
#define HWIO_MSS_QDSP6SS_MEM_STAGGER_RESET_STATUS_IN          \
        in_dword(HWIO_MSS_QDSP6SS_MEM_STAGGER_RESET_STATUS_ADDR)
#define HWIO_MSS_QDSP6SS_MEM_STAGGER_RESET_STATUS_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_MEM_STAGGER_RESET_STATUS_ADDR, m)
#define HWIO_MSS_QDSP6SS_MEM_STAGGER_RESET_STATUS_STAGGER_ALT_ARES_BMSK                  0x2
#define HWIO_MSS_QDSP6SS_MEM_STAGGER_RESET_STATUS_STAGGER_ALT_ARES_SHFT                  0x1
#define HWIO_MSS_QDSP6SS_MEM_STAGGER_RESET_STATUS_STAGGER_FUNC_ARES_BMSK                 0x1
#define HWIO_MSS_QDSP6SS_MEM_STAGGER_RESET_STATUS_STAGGER_FUNC_ARES_SHFT                 0x0

#define HWIO_MSS_QDSP6SS_MEM_STAGGER_RESTORE_CMD_ADDR                             (MSS_QDSP6V67SS_PUB_REG_BASE      + 0x00000484)
#define HWIO_MSS_QDSP6SS_MEM_STAGGER_RESTORE_CMD_RMSK                                    0x1
#define HWIO_MSS_QDSP6SS_MEM_STAGGER_RESTORE_CMD_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_MEM_STAGGER_RESTORE_CMD_ADDR,v)
#define HWIO_MSS_QDSP6SS_MEM_STAGGER_RESTORE_CMD_RESTORE_CMD_BMSK                        0x1
#define HWIO_MSS_QDSP6SS_MEM_STAGGER_RESTORE_CMD_RESTORE_CMD_SHFT                        0x0

#define HWIO_MSS_QDSP6SS_MEM_STAGGER_RESTORE_STATUS_ADDR                          (MSS_QDSP6V67SS_PUB_REG_BASE      + 0x00000488)
#define HWIO_MSS_QDSP6SS_MEM_STAGGER_RESTORE_STATUS_RMSK                                 0x1
#define HWIO_MSS_QDSP6SS_MEM_STAGGER_RESTORE_STATUS_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_MEM_STAGGER_RESTORE_STATUS_ADDR,v)
#define HWIO_MSS_QDSP6SS_MEM_STAGGER_RESTORE_STATUS_RESTORE_BUSY_BMSK                    0x1
#define HWIO_MSS_QDSP6SS_MEM_STAGGER_RESTORE_STATUS_RESTORE_BUSY_SHFT                    0x0

#define HWIO_MSS_QDSP6SS_L2MEM_EFUSE_STATUS_ADDR                                  (MSS_QDSP6V67SS_PUB_REG_BASE      + 0x00000490)
#define HWIO_MSS_QDSP6SS_L2MEM_EFUSE_STATUS_RMSK                                         0xf
#define HWIO_MSS_QDSP6SS_L2MEM_EFUSE_STATUS_IN          \
        in_dword(HWIO_MSS_QDSP6SS_L2MEM_EFUSE_STATUS_ADDR)
#define HWIO_MSS_QDSP6SS_L2MEM_EFUSE_STATUS_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_L2MEM_EFUSE_STATUS_ADDR, m)
#define HWIO_MSS_QDSP6SS_L2MEM_EFUSE_STATUS_EFUSE_L2TCM_EN_BMSK                          0xf
#define HWIO_MSS_QDSP6SS_L2MEM_EFUSE_STATUS_EFUSE_L2TCM_EN_SHFT                          0x0

#define HWIO_MSS_QDSP6SS_CP_CLK_CTL_ADDR                                          (MSS_QDSP6V67SS_PUB_REG_BASE      + 0x00000508)
#define HWIO_MSS_QDSP6SS_CP_CLK_CTL_RMSK                                                 0x1
#define HWIO_MSS_QDSP6SS_CP_CLK_CTL_IN          \
        in_dword(HWIO_MSS_QDSP6SS_CP_CLK_CTL_ADDR)
#define HWIO_MSS_QDSP6SS_CP_CLK_CTL_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_CP_CLK_CTL_ADDR, m)
#define HWIO_MSS_QDSP6SS_CP_CLK_CTL_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_CP_CLK_CTL_ADDR,v)
#define HWIO_MSS_QDSP6SS_CP_CLK_CTL_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_CP_CLK_CTL_ADDR,m,v,HWIO_MSS_QDSP6SS_CP_CLK_CTL_IN)
#define HWIO_MSS_QDSP6SS_CP_CLK_CTL_CLK_ENA_BMSK                                         0x1
#define HWIO_MSS_QDSP6SS_CP_CLK_CTL_CLK_ENA_SHFT                                         0x0

#define HWIO_MSS_QDSP6SS_CPMEM_CFG_ADDR                                           (MSS_QDSP6V67SS_PUB_REG_BASE      + 0x00000520)
#define HWIO_MSS_QDSP6SS_CPMEM_CFG_RMSK                                                  0x3
#define HWIO_MSS_QDSP6SS_CPMEM_CFG_IN          \
        in_dword(HWIO_MSS_QDSP6SS_CPMEM_CFG_ADDR)
#define HWIO_MSS_QDSP6SS_CPMEM_CFG_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_CPMEM_CFG_ADDR, m)
#define HWIO_MSS_QDSP6SS_CPMEM_CFG_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_CPMEM_CFG_ADDR,v)
#define HWIO_MSS_QDSP6SS_CPMEM_CFG_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_CPMEM_CFG_ADDR,m,v,HWIO_MSS_QDSP6SS_CPMEM_CFG_IN)
#define HWIO_MSS_QDSP6SS_CPMEM_CFG_VTCM_SLP_RET_N_BMSK                                   0x2
#define HWIO_MSS_QDSP6SS_CPMEM_CFG_VTCM_SLP_RET_N_SHFT                                   0x1
#define HWIO_MSS_QDSP6SS_CPMEM_CFG_VTCM_SLP_NRET_N_BMSK                                  0x1
#define HWIO_MSS_QDSP6SS_CPMEM_CFG_VTCM_SLP_NRET_N_SHFT                                  0x0

#define HWIO_MSS_QDSP6SS_CPMEM_CMD_ADDR                                           (MSS_QDSP6V67SS_PUB_REG_BASE      + 0x00000524)
#define HWIO_MSS_QDSP6SS_CPMEM_CMD_RMSK                                                  0x3
#define HWIO_MSS_QDSP6SS_CPMEM_CMD_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_CPMEM_CMD_ADDR,v)
#define HWIO_MSS_QDSP6SS_CPMEM_CMD_UPDATE_VTCM_SLP_RET_N_BMSK                            0x2
#define HWIO_MSS_QDSP6SS_CPMEM_CMD_UPDATE_VTCM_SLP_RET_N_SHFT                            0x1
#define HWIO_MSS_QDSP6SS_CPMEM_CMD_UPDATE_VTCM_SLP_NRET_N_BMSK                           0x1
#define HWIO_MSS_QDSP6SS_CPMEM_CMD_UPDATE_VTCM_SLP_NRET_N_SHFT                           0x0

#define HWIO_MSS_QDSP6SS_CPMEM_STATUS_ADDR                                        (MSS_QDSP6V67SS_PUB_REG_BASE      + 0x00000528)
#define HWIO_MSS_QDSP6SS_CPMEM_STATUS_RMSK                                               0x3
#define HWIO_MSS_QDSP6SS_CPMEM_STATUS_IN          \
        in_dword(HWIO_MSS_QDSP6SS_CPMEM_STATUS_ADDR)
#define HWIO_MSS_QDSP6SS_CPMEM_STATUS_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_CPMEM_STATUS_ADDR, m)
#define HWIO_MSS_QDSP6SS_CPMEM_STATUS_VTCM_SLP_RET_N_BMSK                                0x2
#define HWIO_MSS_QDSP6SS_CPMEM_STATUS_VTCM_SLP_RET_N_SHFT                                0x1
#define HWIO_MSS_QDSP6SS_CPMEM_STATUS_VTCM_SLP_NRET_N_BMSK                               0x1
#define HWIO_MSS_QDSP6SS_CPMEM_STATUS_VTCM_SLP_NRET_N_SHFT                               0x0

#define HWIO_MSS_QDSP6SS_L2ITCM_CFG_ADDR                                          (MSS_QDSP6V67SS_PUB_REG_BASE      + 0x00000530)
#define HWIO_MSS_QDSP6SS_L2ITCM_CFG_RMSK                                             0x30003
#define HWIO_MSS_QDSP6SS_L2ITCM_CFG_IN          \
        in_dword(HWIO_MSS_QDSP6SS_L2ITCM_CFG_ADDR)
#define HWIO_MSS_QDSP6SS_L2ITCM_CFG_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_L2ITCM_CFG_ADDR, m)
#define HWIO_MSS_QDSP6SS_L2ITCM_CFG_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_L2ITCM_CFG_ADDR,v)
#define HWIO_MSS_QDSP6SS_L2ITCM_CFG_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_L2ITCM_CFG_ADDR,m,v,HWIO_MSS_QDSP6SS_L2ITCM_CFG_IN)
#define HWIO_MSS_QDSP6SS_L2ITCM_CFG_L2ITCM_SLP_RET_N_BMSK                            0x30000
#define HWIO_MSS_QDSP6SS_L2ITCM_CFG_L2ITCM_SLP_RET_N_SHFT                               0x10
#define HWIO_MSS_QDSP6SS_L2ITCM_CFG_L2ITCM_SLP_NRET_N_BMSK                               0x3
#define HWIO_MSS_QDSP6SS_L2ITCM_CFG_L2ITCM_SLP_NRET_N_SHFT                               0x0

#define HWIO_MSS_QDSP6SS_L2ITCM_CMD_ADDR                                          (MSS_QDSP6V67SS_PUB_REG_BASE      + 0x00000534)
#define HWIO_MSS_QDSP6SS_L2ITCM_CMD_RMSK                                                 0x3
#define HWIO_MSS_QDSP6SS_L2ITCM_CMD_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_L2ITCM_CMD_ADDR,v)
#define HWIO_MSS_QDSP6SS_L2ITCM_CMD_UPDATE_L2ITCM_SLP_RET_N_BMSK                         0x2
#define HWIO_MSS_QDSP6SS_L2ITCM_CMD_UPDATE_L2ITCM_SLP_RET_N_SHFT                         0x1
#define HWIO_MSS_QDSP6SS_L2ITCM_CMD_UPDATE_L2ITCM_SLP_NRET_N_BMSK                        0x1
#define HWIO_MSS_QDSP6SS_L2ITCM_CMD_UPDATE_L2ITCM_SLP_NRET_N_SHFT                        0x0

#define HWIO_MSS_QDSP6SS_L2ITCM_STATUS_ADDR                                       (MSS_QDSP6V67SS_PUB_REG_BASE      + 0x00000538)
#define HWIO_MSS_QDSP6SS_L2ITCM_STATUS_RMSK                                          0x30003
#define HWIO_MSS_QDSP6SS_L2ITCM_STATUS_IN          \
        in_dword(HWIO_MSS_QDSP6SS_L2ITCM_STATUS_ADDR)
#define HWIO_MSS_QDSP6SS_L2ITCM_STATUS_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_L2ITCM_STATUS_ADDR, m)
#define HWIO_MSS_QDSP6SS_L2ITCM_STATUS_L2ITCM_SLP_RET_N_BMSK                         0x30000
#define HWIO_MSS_QDSP6SS_L2ITCM_STATUS_L2ITCM_SLP_RET_N_SHFT                            0x10
#define HWIO_MSS_QDSP6SS_L2ITCM_STATUS_L2ITCM_SLP_NRET_N_BMSK                            0x3
#define HWIO_MSS_QDSP6SS_L2ITCM_STATUS_L2ITCM_SLP_NRET_N_SHFT                            0x0

#define HWIO_MSS_QDSP6SS_L1SMEM_CFG_ADDR                                          (MSS_QDSP6V67SS_PUB_REG_BASE      + 0x00000540)
#define HWIO_MSS_QDSP6SS_L1SMEM_CFG_RMSK                                          0x3f1f3f1f
#define HWIO_MSS_QDSP6SS_L1SMEM_CFG_IN          \
        in_dword(HWIO_MSS_QDSP6SS_L1SMEM_CFG_ADDR)
#define HWIO_MSS_QDSP6SS_L1SMEM_CFG_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_L1SMEM_CFG_ADDR, m)
#define HWIO_MSS_QDSP6SS_L1SMEM_CFG_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_L1SMEM_CFG_ADDR,v)
#define HWIO_MSS_QDSP6SS_L1SMEM_CFG_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_L1SMEM_CFG_ADDR,m,v,HWIO_MSS_QDSP6SS_L1SMEM_CFG_IN)
#define HWIO_MSS_QDSP6SS_L1SMEM_CFG_L1S2_SLP_RET_N_BMSK                           0x3f000000
#define HWIO_MSS_QDSP6SS_L1SMEM_CFG_L1S2_SLP_RET_N_SHFT                                 0x18
#define HWIO_MSS_QDSP6SS_L1SMEM_CFG_L1S_SLP_RET_N_BMSK                              0x1f0000
#define HWIO_MSS_QDSP6SS_L1SMEM_CFG_L1S_SLP_RET_N_SHFT                                  0x10
#define HWIO_MSS_QDSP6SS_L1SMEM_CFG_L1S2_SLP_NRET_N_BMSK                              0x3f00
#define HWIO_MSS_QDSP6SS_L1SMEM_CFG_L1S2_SLP_NRET_N_SHFT                                 0x8
#define HWIO_MSS_QDSP6SS_L1SMEM_CFG_L1S_SLP_NRET_N_BMSK                                 0x1f
#define HWIO_MSS_QDSP6SS_L1SMEM_CFG_L1S_SLP_NRET_N_SHFT                                  0x0

#define HWIO_MSS_QDSP6SS_L1SMEM_CMD_ADDR                                          (MSS_QDSP6V67SS_PUB_REG_BASE      + 0x00000544)
#define HWIO_MSS_QDSP6SS_L1SMEM_CMD_RMSK                                                 0xf
#define HWIO_MSS_QDSP6SS_L1SMEM_CMD_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_L1SMEM_CMD_ADDR,v)
#define HWIO_MSS_QDSP6SS_L1SMEM_CMD_UPDATE_L1S2_SLP_RET_N_BMSK                           0x8
#define HWIO_MSS_QDSP6SS_L1SMEM_CMD_UPDATE_L1S2_SLP_RET_N_SHFT                           0x3
#define HWIO_MSS_QDSP6SS_L1SMEM_CMD_UPDATE_L1S2_SLP_NRET_N_BMSK                          0x4
#define HWIO_MSS_QDSP6SS_L1SMEM_CMD_UPDATE_L1S2_SLP_NRET_N_SHFT                          0x2
#define HWIO_MSS_QDSP6SS_L1SMEM_CMD_UPDATE_L1S_SLP_RET_N_BMSK                            0x2
#define HWIO_MSS_QDSP6SS_L1SMEM_CMD_UPDATE_L1S_SLP_RET_N_SHFT                            0x1
#define HWIO_MSS_QDSP6SS_L1SMEM_CMD_UPDATE_L1S_SLP_NRET_N_BMSK                           0x1
#define HWIO_MSS_QDSP6SS_L1SMEM_CMD_UPDATE_L1S_SLP_NRET_N_SHFT                           0x0

#define HWIO_MSS_QDSP6SS_L1SMEM_STATUS_ADDR                                       (MSS_QDSP6V67SS_PUB_REG_BASE      + 0x00000548)
#define HWIO_MSS_QDSP6SS_L1SMEM_STATUS_RMSK                                       0x3f1f3f1f
#define HWIO_MSS_QDSP6SS_L1SMEM_STATUS_IN          \
        in_dword(HWIO_MSS_QDSP6SS_L1SMEM_STATUS_ADDR)
#define HWIO_MSS_QDSP6SS_L1SMEM_STATUS_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_L1SMEM_STATUS_ADDR, m)
#define HWIO_MSS_QDSP6SS_L1SMEM_STATUS_L1S2_SLP_RET_N_BMSK                        0x3f000000
#define HWIO_MSS_QDSP6SS_L1SMEM_STATUS_L1S2_SLP_RET_N_SHFT                              0x18
#define HWIO_MSS_QDSP6SS_L1SMEM_STATUS_L1S_SLP_RET_N_BMSK                           0x1f0000
#define HWIO_MSS_QDSP6SS_L1SMEM_STATUS_L1S_SLP_RET_N_SHFT                               0x10
#define HWIO_MSS_QDSP6SS_L1SMEM_STATUS_L1S2_SLP_NRET_N_BMSK                           0x3f00
#define HWIO_MSS_QDSP6SS_L1SMEM_STATUS_L1S2_SLP_NRET_N_SHFT                              0x8
#define HWIO_MSS_QDSP6SS_L1SMEM_STATUS_L1S_SLP_NRET_N_BMSK                              0x1f
#define HWIO_MSS_QDSP6SS_L1SMEM_STATUS_L1S_SLP_NRET_N_SHFT                               0x0

#define HWIO_MSS_QDSP6SS_VU0_PWR_VOTE_SET_ADDR                                    (MSS_QDSP6V67SS_PUB_REG_BASE      + 0x00000600)
#define HWIO_MSS_QDSP6SS_VU0_PWR_VOTE_SET_RMSK                                          0xff
#define HWIO_MSS_QDSP6SS_VU0_PWR_VOTE_SET_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_VU0_PWR_VOTE_SET_ADDR,v)
#define HWIO_MSS_QDSP6SS_VU0_PWR_VOTE_SET_SET_BMSK                                      0xff
#define HWIO_MSS_QDSP6SS_VU0_PWR_VOTE_SET_SET_SHFT                                       0x0

#define HWIO_MSS_QDSP6SS_VU0_PWR_VOTE_CLR_ADDR                                    (MSS_QDSP6V67SS_PUB_REG_BASE      + 0x00000604)
#define HWIO_MSS_QDSP6SS_VU0_PWR_VOTE_CLR_RMSK                                         0xfff
#define HWIO_MSS_QDSP6SS_VU0_PWR_VOTE_CLR_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_VU0_PWR_VOTE_CLR_ADDR,v)
#define HWIO_MSS_QDSP6SS_VU0_PWR_VOTE_CLR_CLR_BMSK                                     0xfff
#define HWIO_MSS_QDSP6SS_VU0_PWR_VOTE_CLR_CLR_SHFT                                       0x0

#define HWIO_MSS_QDSP6SS_VU0_PWR_VOTE_MASK_ADDR                                   (MSS_QDSP6V67SS_PUB_REG_BASE      + 0x00000608)
#define HWIO_MSS_QDSP6SS_VU0_PWR_VOTE_MASK_RMSK                                        0xfff
#define HWIO_MSS_QDSP6SS_VU0_PWR_VOTE_MASK_IN          \
        in_dword(HWIO_MSS_QDSP6SS_VU0_PWR_VOTE_MASK_ADDR)
#define HWIO_MSS_QDSP6SS_VU0_PWR_VOTE_MASK_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_VU0_PWR_VOTE_MASK_ADDR, m)
#define HWIO_MSS_QDSP6SS_VU0_PWR_VOTE_MASK_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_VU0_PWR_VOTE_MASK_ADDR,v)
#define HWIO_MSS_QDSP6SS_VU0_PWR_VOTE_MASK_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_VU0_PWR_VOTE_MASK_ADDR,m,v,HWIO_MSS_QDSP6SS_VU0_PWR_VOTE_MASK_IN)
#define HWIO_MSS_QDSP6SS_VU0_PWR_VOTE_MASK_HW_VOTE_EN_BMSK                             0xf00
#define HWIO_MSS_QDSP6SS_VU0_PWR_VOTE_MASK_HW_VOTE_EN_SHFT                               0x8
#define HWIO_MSS_QDSP6SS_VU0_PWR_VOTE_MASK_VOTE_EN_BMSK                                 0xff
#define HWIO_MSS_QDSP6SS_VU0_PWR_VOTE_MASK_VOTE_EN_SHFT                                  0x0

#define HWIO_MSS_QDSP6SS_VU0_PWR_STATUS_ADDR                                      (MSS_QDSP6V67SS_PUB_REG_BASE      + 0x0000060c)
#define HWIO_MSS_QDSP6SS_VU0_PWR_STATUS_RMSK                                         0xfff1f
#define HWIO_MSS_QDSP6SS_VU0_PWR_STATUS_IN          \
        in_dword(HWIO_MSS_QDSP6SS_VU0_PWR_STATUS_ADDR)
#define HWIO_MSS_QDSP6SS_VU0_PWR_STATUS_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_VU0_PWR_STATUS_ADDR, m)
#define HWIO_MSS_QDSP6SS_VU0_PWR_STATUS_VOTE_STAT_BMSK                               0xfff00
#define HWIO_MSS_QDSP6SS_VU0_PWR_STATUS_VOTE_STAT_SHFT                                   0x8
#define HWIO_MSS_QDSP6SS_VU0_PWR_STATUS_WAIT_NEXT_BMSK                                  0x10
#define HWIO_MSS_QDSP6SS_VU0_PWR_STATUS_WAIT_NEXT_SHFT                                   0x4
#define HWIO_MSS_QDSP6SS_VU0_PWR_STATUS_WAIT_PREV_BMSK                                   0x8
#define HWIO_MSS_QDSP6SS_VU0_PWR_STATUS_WAIT_PREV_SHFT                                   0x3
#define HWIO_MSS_QDSP6SS_VU0_PWR_STATUS_PWR_DOWN_BMSK                                    0x4
#define HWIO_MSS_QDSP6SS_VU0_PWR_STATUS_PWR_DOWN_SHFT                                    0x2
#define HWIO_MSS_QDSP6SS_VU0_PWR_STATUS_PWR_REQ_BMSK                                     0x2
#define HWIO_MSS_QDSP6SS_VU0_PWR_STATUS_PWR_REQ_SHFT                                     0x1
#define HWIO_MSS_QDSP6SS_VU0_PWR_STATUS_PWR_ON_BMSK                                      0x1
#define HWIO_MSS_QDSP6SS_VU0_PWR_STATUS_PWR_ON_SHFT                                      0x0

#define HWIO_MSS_QDSP6SS_VU0_PWR_DEBUG_ADDR                                       (MSS_QDSP6V67SS_PUB_REG_BASE      + 0x00000610)
#define HWIO_MSS_QDSP6SS_VU0_PWR_DEBUG_RMSK                                             0xf7
#define HWIO_MSS_QDSP6SS_VU0_PWR_DEBUG_IN          \
        in_dword(HWIO_MSS_QDSP6SS_VU0_PWR_DEBUG_ADDR)
#define HWIO_MSS_QDSP6SS_VU0_PWR_DEBUG_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_VU0_PWR_DEBUG_ADDR, m)
#define HWIO_MSS_QDSP6SS_VU0_PWR_DEBUG_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_VU0_PWR_DEBUG_ADDR,v)
#define HWIO_MSS_QDSP6SS_VU0_PWR_DEBUG_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_VU0_PWR_DEBUG_ADDR,m,v,HWIO_MSS_QDSP6SS_VU0_PWR_DEBUG_IN)
#define HWIO_MSS_QDSP6SS_VU0_PWR_DEBUG_SEQ_DELAY_BMSK                                   0xf0
#define HWIO_MSS_QDSP6SS_VU0_PWR_DEBUG_SEQ_DELAY_SHFT                                    0x4
#define HWIO_MSS_QDSP6SS_VU0_PWR_DEBUG_CLAMP_IO_BMSK                                     0x4
#define HWIO_MSS_QDSP6SS_VU0_PWR_DEBUG_CLAMP_IO_SHFT                                     0x2
#define HWIO_MSS_QDSP6SS_VU0_PWR_DEBUG_CP_ARES_BMSK                                      0x2
#define HWIO_MSS_QDSP6SS_VU0_PWR_DEBUG_CP_ARES_SHFT                                      0x1
#define HWIO_MSS_QDSP6SS_VU0_PWR_DEBUG_CP_CLK_EN_BMSK                                    0x1
#define HWIO_MSS_QDSP6SS_VU0_PWR_DEBUG_CP_CLK_EN_SHFT                                    0x0

#define HWIO_MSS_QDSP6SS_VU_QOS_ADDR                                              (MSS_QDSP6V67SS_PUB_REG_BASE      + 0x000006a0)
#define HWIO_MSS_QDSP6SS_VU_QOS_RMSK                                                    0x70
#define HWIO_MSS_QDSP6SS_VU_QOS_IN          \
        in_dword(HWIO_MSS_QDSP6SS_VU_QOS_ADDR)
#define HWIO_MSS_QDSP6SS_VU_QOS_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_VU_QOS_ADDR, m)
#define HWIO_MSS_QDSP6SS_VU_QOS_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_VU_QOS_ADDR,v)
#define HWIO_MSS_QDSP6SS_VU_QOS_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_VU_QOS_ADDR,m,v,HWIO_MSS_QDSP6SS_VU_QOS_IN)
#define HWIO_MSS_QDSP6SS_VU_QOS_QOS_AXIS_BMSK                                           0x70
#define HWIO_MSS_QDSP6SS_VU_QOS_QOS_AXIS_SHFT                                            0x4

#define HWIO_MSS_QDSP6SS_VPE_VU_CBCR_ADDR                                         (MSS_QDSP6V67SS_PUB_REG_BASE      + 0x000006b0)
#define HWIO_MSS_QDSP6SS_VPE_VU_CBCR_RMSK                                         0x80000001
#define HWIO_MSS_QDSP6SS_VPE_VU_CBCR_IN          \
        in_dword(HWIO_MSS_QDSP6SS_VPE_VU_CBCR_ADDR)
#define HWIO_MSS_QDSP6SS_VPE_VU_CBCR_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_VPE_VU_CBCR_ADDR, m)
#define HWIO_MSS_QDSP6SS_VPE_VU_CBCR_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_VPE_VU_CBCR_ADDR,v)
#define HWIO_MSS_QDSP6SS_VPE_VU_CBCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_VPE_VU_CBCR_ADDR,m,v,HWIO_MSS_QDSP6SS_VPE_VU_CBCR_IN)
#define HWIO_MSS_QDSP6SS_VPE_VU_CBCR_CLKOFF_BMSK                                  0x80000000
#define HWIO_MSS_QDSP6SS_VPE_VU_CBCR_CLKOFF_SHFT                                        0x1f
#define HWIO_MSS_QDSP6SS_VPE_VU_CBCR_CLKEN_BMSK                                          0x1
#define HWIO_MSS_QDSP6SS_VPE_VU_CBCR_CLKEN_SHFT                                          0x0

#define HWIO_MSS_QDSP6SS_TEST_BUS_CTL_ADDR                                        (MSS_QDSP6V67SS_PUB_REG_BASE      + 0x00002000)
#define HWIO_MSS_QDSP6SS_TEST_BUS_CTL_RMSK                                        0xffffffff
#define HWIO_MSS_QDSP6SS_TEST_BUS_CTL_IN          \
        in_dword(HWIO_MSS_QDSP6SS_TEST_BUS_CTL_ADDR)
#define HWIO_MSS_QDSP6SS_TEST_BUS_CTL_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_TEST_BUS_CTL_ADDR, m)
#define HWIO_MSS_QDSP6SS_TEST_BUS_CTL_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_TEST_BUS_CTL_ADDR,v)
#define HWIO_MSS_QDSP6SS_TEST_BUS_CTL_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_TEST_BUS_CTL_ADDR,m,v,HWIO_MSS_QDSP6SS_TEST_BUS_CTL_IN)
#define HWIO_MSS_QDSP6SS_TEST_BUS_CTL_SPARE_CFG_BMSK                              0xff000000
#define HWIO_MSS_QDSP6SS_TEST_BUS_CTL_SPARE_CFG_SHFT                                    0x18
#define HWIO_MSS_QDSP6SS_TEST_BUS_CTL_HWE_INV_BMSK                                  0x800000
#define HWIO_MSS_QDSP6SS_TEST_BUS_CTL_HWE_INV_SHFT                                      0x17
#define HWIO_MSS_QDSP6SS_TEST_BUS_CTL_HWE_SEL_BMSK                                  0x7c0000
#define HWIO_MSS_QDSP6SS_TEST_BUS_CTL_HWE_SEL_SHFT                                      0x12
#define HWIO_MSS_QDSP6SS_TEST_BUS_CTL_TESTBUS_EN_BMSK                                0x20000
#define HWIO_MSS_QDSP6SS_TEST_BUS_CTL_TESTBUS_EN_SHFT                                   0x11
#define HWIO_MSS_QDSP6SS_TEST_BUS_CTL_Q6_SEL_BMSK                                    0x10000
#define HWIO_MSS_QDSP6SS_TEST_BUS_CTL_Q6_SEL_SHFT                                       0x10
#define HWIO_MSS_QDSP6SS_TEST_BUS_CTL_SEL_BMSK                                        0xffff
#define HWIO_MSS_QDSP6SS_TEST_BUS_CTL_SEL_SHFT                                           0x0

#define HWIO_MSS_QDSP6SS_TEST_BUS_VALUE_ADDR                                      (MSS_QDSP6V67SS_PUB_REG_BASE      + 0x00002004)
#define HWIO_MSS_QDSP6SS_TEST_BUS_VALUE_RMSK                                      0xffffffff
#define HWIO_MSS_QDSP6SS_TEST_BUS_VALUE_IN          \
        in_dword(HWIO_MSS_QDSP6SS_TEST_BUS_VALUE_ADDR)
#define HWIO_MSS_QDSP6SS_TEST_BUS_VALUE_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_TEST_BUS_VALUE_ADDR, m)
#define HWIO_MSS_QDSP6SS_TEST_BUS_VALUE_VALUE_BMSK                                0xffffffff
#define HWIO_MSS_QDSP6SS_TEST_BUS_VALUE_VALUE_SHFT                                       0x0

#define HWIO_MSS_QDSP6SS_TEST_BUS_WDATA_ADDR                                      (MSS_QDSP6V67SS_PUB_REG_BASE      + 0x00002008)
#define HWIO_MSS_QDSP6SS_TEST_BUS_WDATA_RMSK                                      0xffffffff
#define HWIO_MSS_QDSP6SS_TEST_BUS_WDATA_IN          \
        in_dword(HWIO_MSS_QDSP6SS_TEST_BUS_WDATA_ADDR)
#define HWIO_MSS_QDSP6SS_TEST_BUS_WDATA_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_TEST_BUS_WDATA_ADDR, m)
#define HWIO_MSS_QDSP6SS_TEST_BUS_WDATA_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_TEST_BUS_WDATA_ADDR,v)
#define HWIO_MSS_QDSP6SS_TEST_BUS_WDATA_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_TEST_BUS_WDATA_ADDR,m,v,HWIO_MSS_QDSP6SS_TEST_BUS_WDATA_IN)
#define HWIO_MSS_QDSP6SS_TEST_BUS_WDATA_VALUE_BMSK                                0xffffffff
#define HWIO_MSS_QDSP6SS_TEST_BUS_WDATA_VALUE_SHFT                                       0x0

#define HWIO_MSS_QDSP6SS_PWRDBG_CTL_ADDR                                          (MSS_QDSP6V67SS_PUB_REG_BASE      + 0x0000200c)
#define HWIO_MSS_QDSP6SS_PWRDBG_CTL_RMSK                                                 0x1
#define HWIO_MSS_QDSP6SS_PWRDBG_CTL_IN          \
        in_dword(HWIO_MSS_QDSP6SS_PWRDBG_CTL_ADDR)
#define HWIO_MSS_QDSP6SS_PWRDBG_CTL_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_PWRDBG_CTL_ADDR, m)
#define HWIO_MSS_QDSP6SS_PWRDBG_CTL_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_PWRDBG_CTL_ADDR,v)
#define HWIO_MSS_QDSP6SS_PWRDBG_CTL_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_PWRDBG_CTL_ADDR,m,v,HWIO_MSS_QDSP6SS_PWRDBG_CTL_IN)
#define HWIO_MSS_QDSP6SS_PWRDBG_CTL_ENA_BMSK                                             0x1
#define HWIO_MSS_QDSP6SS_PWRDBG_CTL_ENA_SHFT                                             0x0

#define HWIO_MSS_QDSP6SS_TEST_CLK_CTL_ADDR                                        (MSS_QDSP6V67SS_PUB_REG_BASE      + 0x00002010)
#define HWIO_MSS_QDSP6SS_TEST_CLK_CTL_RMSK                                              0x7f
#define HWIO_MSS_QDSP6SS_TEST_CLK_CTL_IN          \
        in_dword(HWIO_MSS_QDSP6SS_TEST_CLK_CTL_ADDR)
#define HWIO_MSS_QDSP6SS_TEST_CLK_CTL_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_TEST_CLK_CTL_ADDR, m)
#define HWIO_MSS_QDSP6SS_TEST_CLK_CTL_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_TEST_CLK_CTL_ADDR,v)
#define HWIO_MSS_QDSP6SS_TEST_CLK_CTL_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_TEST_CLK_CTL_ADDR,m,v,HWIO_MSS_QDSP6SS_TEST_CLK_CTL_IN)
#define HWIO_MSS_QDSP6SS_TEST_CLK_CTL_Q6_DBG_CLK_EN_BMSK                                0x40
#define HWIO_MSS_QDSP6SS_TEST_CLK_CTL_Q6_DBG_CLK_EN_SHFT                                 0x6
#define HWIO_MSS_QDSP6SS_TEST_CLK_CTL_Q6_DBG_CLK_INV_BMSK                               0x20
#define HWIO_MSS_QDSP6SS_TEST_CLK_CTL_Q6_DBG_CLK_INV_SHFT                                0x5
#define HWIO_MSS_QDSP6SS_TEST_CLK_CTL_DBG_CLK_SEL_BMSK                                  0x1f
#define HWIO_MSS_QDSP6SS_TEST_CLK_CTL_DBG_CLK_SEL_SHFT                                   0x0

#define HWIO_MSS_QDSP6SS_BHS_TEST_ADDR                                            (MSS_QDSP6V67SS_PUB_REG_BASE      + 0x00002014)
#define HWIO_MSS_QDSP6SS_BHS_TEST_RMSK                                                   0x1
#define HWIO_MSS_QDSP6SS_BHS_TEST_IN          \
        in_dword(HWIO_MSS_QDSP6SS_BHS_TEST_ADDR)
#define HWIO_MSS_QDSP6SS_BHS_TEST_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_BHS_TEST_ADDR, m)
#define HWIO_MSS_QDSP6SS_BHS_TEST_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_BHS_TEST_ADDR,v)
#define HWIO_MSS_QDSP6SS_BHS_TEST_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_BHS_TEST_ADDR,m,v,HWIO_MSS_QDSP6SS_BHS_TEST_IN)
#define HWIO_MSS_QDSP6SS_BHS_TEST_INSTANT_ON_BMSK                                        0x1
#define HWIO_MSS_QDSP6SS_BHS_TEST_INSTANT_ON_SHFT                                        0x0

#define HWIO_MSS_QDSP6SS_ZMEAS_CONFIG_ADDR                                        (MSS_QDSP6V67SS_PUB_REG_BASE      + 0x00002018)
#define HWIO_MSS_QDSP6SS_ZMEAS_CONFIG_RMSK                                          0xffffff
#define HWIO_MSS_QDSP6SS_ZMEAS_CONFIG_IN          \
        in_dword(HWIO_MSS_QDSP6SS_ZMEAS_CONFIG_ADDR)
#define HWIO_MSS_QDSP6SS_ZMEAS_CONFIG_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_ZMEAS_CONFIG_ADDR, m)
#define HWIO_MSS_QDSP6SS_ZMEAS_CONFIG_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_ZMEAS_CONFIG_ADDR,v)
#define HWIO_MSS_QDSP6SS_ZMEAS_CONFIG_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_ZMEAS_CONFIG_ADDR,m,v,HWIO_MSS_QDSP6SS_ZMEAS_CONFIG_IN)
#define HWIO_MSS_QDSP6SS_ZMEAS_CONFIG_ZMEAS_COUNT_BMSK                              0xffffe0
#define HWIO_MSS_QDSP6SS_ZMEAS_CONFIG_ZMEAS_COUNT_SHFT                                   0x5
#define HWIO_MSS_QDSP6SS_ZMEAS_CONFIG_MUX_SEL_BMSK                                      0x10
#define HWIO_MSS_QDSP6SS_ZMEAS_CONFIG_MUX_SEL_SHFT                                       0x4
#define HWIO_MSS_QDSP6SS_ZMEAS_CONFIG_ZMEAS_CLK_R_ENA_BMSK                               0x8
#define HWIO_MSS_QDSP6SS_ZMEAS_CONFIG_ZMEAS_CLK_R_ENA_SHFT                               0x3
#define HWIO_MSS_QDSP6SS_ZMEAS_CONFIG_ZMEAS_CGC_EN_BMSK                                  0x4
#define HWIO_MSS_QDSP6SS_ZMEAS_CONFIG_ZMEAS_CGC_EN_SHFT                                  0x2
#define HWIO_MSS_QDSP6SS_ZMEAS_CONFIG_ZMEAS_BYPASS_BMSK                                  0x2
#define HWIO_MSS_QDSP6SS_ZMEAS_CONFIG_ZMEAS_BYPASS_SHFT                                  0x1
#define HWIO_MSS_QDSP6SS_ZMEAS_CONFIG_ZMEAS_SW_RESET_BMSK                                0x1
#define HWIO_MSS_QDSP6SS_ZMEAS_CONFIG_ZMEAS_SW_RESET_SHFT                                0x0

#define HWIO_MSS_QDSP6SS_CORE_STATUS_ADDR                                         (MSS_QDSP6V67SS_PUB_REG_BASE      + 0x00002028)
#define HWIO_MSS_QDSP6SS_CORE_STATUS_RMSK                                               0x3f
#define HWIO_MSS_QDSP6SS_CORE_STATUS_IN          \
        in_dword(HWIO_MSS_QDSP6SS_CORE_STATUS_ADDR)
#define HWIO_MSS_QDSP6SS_CORE_STATUS_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_CORE_STATUS_ADDR, m)
#define HWIO_MSS_QDSP6SS_CORE_STATUS_VU1_VX_IDLE_BMSK                                   0x20
#define HWIO_MSS_QDSP6SS_CORE_STATUS_VU1_VX_IDLE_SHFT                                    0x5
#define HWIO_MSS_QDSP6SS_CORE_STATUS_VU0_VX_IDLE_BMSK                                   0x10
#define HWIO_MSS_QDSP6SS_CORE_STATUS_VU0_VX_IDLE_SHFT                                    0x4
#define HWIO_MSS_QDSP6SS_CORE_STATUS_VU_OVERALL_IDLE_BMSK                                0x8
#define HWIO_MSS_QDSP6SS_CORE_STATUS_VU_OVERALL_IDLE_SHFT                                0x3
#define HWIO_MSS_QDSP6SS_CORE_STATUS_CP0_IDLE_BMSK                                       0x4
#define HWIO_MSS_QDSP6SS_CORE_STATUS_CP0_IDLE_SHFT                                       0x2
#define HWIO_MSS_QDSP6SS_CORE_STATUS_CORE_IN_SLEEP_BMSK                                  0x2
#define HWIO_MSS_QDSP6SS_CORE_STATUS_CORE_IN_SLEEP_SHFT                                  0x1
#define HWIO_MSS_QDSP6SS_CORE_STATUS_CORE_STATE_BMSK                                     0x1
#define HWIO_MSS_QDSP6SS_CORE_STATUS_CORE_STATE_SHFT                                     0x0

#define HWIO_MSS_QDSP6SS_MEM_DEBUG_ADDR                                           (MSS_QDSP6V67SS_PUB_REG_BASE      + 0x00002030)
#define HWIO_MSS_QDSP6SS_MEM_DEBUG_RMSK                                               0xffff
#define HWIO_MSS_QDSP6SS_MEM_DEBUG_IN          \
        in_dword(HWIO_MSS_QDSP6SS_MEM_DEBUG_ADDR)
#define HWIO_MSS_QDSP6SS_MEM_DEBUG_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_MEM_DEBUG_ADDR, m)
#define HWIO_MSS_QDSP6SS_MEM_DEBUG_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_MEM_DEBUG_ADDR,v)
#define HWIO_MSS_QDSP6SS_MEM_DEBUG_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_MEM_DEBUG_ADDR,m,v,HWIO_MSS_QDSP6SS_MEM_DEBUG_IN)
#define HWIO_MSS_QDSP6SS_MEM_DEBUG_OFF_BMSK                                           0xffff
#define HWIO_MSS_QDSP6SS_MEM_DEBUG_OFF_SHFT                                              0x0

#define HWIO_MSS_QDSP6SS_L2MEM_DEBUG0_ADDR                                        (MSS_QDSP6V67SS_PUB_REG_BASE      + 0x00002034)
#define HWIO_MSS_QDSP6SS_L2MEM_DEBUG0_RMSK                                        0xffffffff
#define HWIO_MSS_QDSP6SS_L2MEM_DEBUG0_IN          \
        in_dword(HWIO_MSS_QDSP6SS_L2MEM_DEBUG0_ADDR)
#define HWIO_MSS_QDSP6SS_L2MEM_DEBUG0_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_L2MEM_DEBUG0_ADDR, m)
#define HWIO_MSS_QDSP6SS_L2MEM_DEBUG0_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_L2MEM_DEBUG0_ADDR,v)
#define HWIO_MSS_QDSP6SS_L2MEM_DEBUG0_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_L2MEM_DEBUG0_ADDR,m,v,HWIO_MSS_QDSP6SS_L2MEM_DEBUG0_IN)
#define HWIO_MSS_QDSP6SS_L2MEM_DEBUG0_OFF_BMSK                                    0xffffffff
#define HWIO_MSS_QDSP6SS_L2MEM_DEBUG0_OFF_SHFT                                           0x0

#define HWIO_MSS_QDSP6SS_L2MEM_DEBUG1_ADDR                                        (MSS_QDSP6V67SS_PUB_REG_BASE      + 0x00002038)
#define HWIO_MSS_QDSP6SS_L2MEM_DEBUG1_RMSK                                        0xffffffff
#define HWIO_MSS_QDSP6SS_L2MEM_DEBUG1_IN          \
        in_dword(HWIO_MSS_QDSP6SS_L2MEM_DEBUG1_ADDR)
#define HWIO_MSS_QDSP6SS_L2MEM_DEBUG1_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_L2MEM_DEBUG1_ADDR, m)
#define HWIO_MSS_QDSP6SS_L2MEM_DEBUG1_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_L2MEM_DEBUG1_ADDR,v)
#define HWIO_MSS_QDSP6SS_L2MEM_DEBUG1_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_L2MEM_DEBUG1_ADDR,m,v,HWIO_MSS_QDSP6SS_L2MEM_DEBUG1_IN)
#define HWIO_MSS_QDSP6SS_L2MEM_DEBUG1_OFF_BMSK                                    0xffffffff
#define HWIO_MSS_QDSP6SS_L2MEM_DEBUG1_OFF_SHFT                                           0x0

#define HWIO_MSS_QDSP6SS_CPMEM_DEBUG_ADDR                                         (MSS_QDSP6V67SS_PUB_REG_BASE      + 0x0000203c)
#define HWIO_MSS_QDSP6SS_CPMEM_DEBUG_RMSK                                         0xffffffff
#define HWIO_MSS_QDSP6SS_CPMEM_DEBUG_IN          \
        in_dword(HWIO_MSS_QDSP6SS_CPMEM_DEBUG_ADDR)
#define HWIO_MSS_QDSP6SS_CPMEM_DEBUG_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_CPMEM_DEBUG_ADDR, m)
#define HWIO_MSS_QDSP6SS_CPMEM_DEBUG_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_CPMEM_DEBUG_ADDR,v)
#define HWIO_MSS_QDSP6SS_CPMEM_DEBUG_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_CPMEM_DEBUG_ADDR,m,v,HWIO_MSS_QDSP6SS_CPMEM_DEBUG_IN)
#define HWIO_MSS_QDSP6SS_CPMEM_DEBUG_OFF_BMSK                                     0xffffffff
#define HWIO_MSS_QDSP6SS_CPMEM_DEBUG_OFF_SHFT                                            0x0

#define HWIO_MSS_QDSP6SS_L1SMEM_DEBUG0_ADDR                                       (MSS_QDSP6V67SS_PUB_REG_BASE      + 0x00002040)
#define HWIO_MSS_QDSP6SS_L1SMEM_DEBUG0_RMSK                                       0xffffffff
#define HWIO_MSS_QDSP6SS_L1SMEM_DEBUG0_IN          \
        in_dword(HWIO_MSS_QDSP6SS_L1SMEM_DEBUG0_ADDR)
#define HWIO_MSS_QDSP6SS_L1SMEM_DEBUG0_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_L1SMEM_DEBUG0_ADDR, m)
#define HWIO_MSS_QDSP6SS_L1SMEM_DEBUG0_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_L1SMEM_DEBUG0_ADDR,v)
#define HWIO_MSS_QDSP6SS_L1SMEM_DEBUG0_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_L1SMEM_DEBUG0_ADDR,m,v,HWIO_MSS_QDSP6SS_L1SMEM_DEBUG0_IN)
#define HWIO_MSS_QDSP6SS_L1SMEM_DEBUG0_OFF_BMSK                                   0xffffffff
#define HWIO_MSS_QDSP6SS_L1SMEM_DEBUG0_OFF_SHFT                                          0x0

#define HWIO_MSS_QDSP6SS_L1SMEM_DEBUG1_ADDR                                       (MSS_QDSP6V67SS_PUB_REG_BASE      + 0x00002044)
#define HWIO_MSS_QDSP6SS_L1SMEM_DEBUG1_RMSK                                       0xffffffff
#define HWIO_MSS_QDSP6SS_L1SMEM_DEBUG1_IN          \
        in_dword(HWIO_MSS_QDSP6SS_L1SMEM_DEBUG1_ADDR)
#define HWIO_MSS_QDSP6SS_L1SMEM_DEBUG1_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_L1SMEM_DEBUG1_ADDR, m)
#define HWIO_MSS_QDSP6SS_L1SMEM_DEBUG1_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_L1SMEM_DEBUG1_ADDR,v)
#define HWIO_MSS_QDSP6SS_L1SMEM_DEBUG1_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_L1SMEM_DEBUG1_ADDR,m,v,HWIO_MSS_QDSP6SS_L1SMEM_DEBUG1_IN)
#define HWIO_MSS_QDSP6SS_L1SMEM_DEBUG1_OFF_BMSK                                   0xffffffff
#define HWIO_MSS_QDSP6SS_L1SMEM_DEBUG1_OFF_SHFT                                          0x0

#define HWIO_MSS_QDSP6SS_L1SMEM_DEBUG2_ADDR                                       (MSS_QDSP6V67SS_PUB_REG_BASE      + 0x00002048)
#define HWIO_MSS_QDSP6SS_L1SMEM_DEBUG2_RMSK                                       0xffffffff
#define HWIO_MSS_QDSP6SS_L1SMEM_DEBUG2_IN          \
        in_dword(HWIO_MSS_QDSP6SS_L1SMEM_DEBUG2_ADDR)
#define HWIO_MSS_QDSP6SS_L1SMEM_DEBUG2_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_L1SMEM_DEBUG2_ADDR, m)
#define HWIO_MSS_QDSP6SS_L1SMEM_DEBUG2_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_L1SMEM_DEBUG2_ADDR,v)
#define HWIO_MSS_QDSP6SS_L1SMEM_DEBUG2_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_L1SMEM_DEBUG2_ADDR,m,v,HWIO_MSS_QDSP6SS_L1SMEM_DEBUG2_IN)
#define HWIO_MSS_QDSP6SS_L1SMEM_DEBUG2_OFF_BMSK                                   0xffffffff
#define HWIO_MSS_QDSP6SS_L1SMEM_DEBUG2_OFF_SHFT                                          0x0

#define HWIO_MSS_QDSP6SS_L1SMEM_DEBUG3_ADDR                                       (MSS_QDSP6V67SS_PUB_REG_BASE      + 0x0000204c)
#define HWIO_MSS_QDSP6SS_L1SMEM_DEBUG3_RMSK                                       0xffffffff
#define HWIO_MSS_QDSP6SS_L1SMEM_DEBUG3_IN          \
        in_dword(HWIO_MSS_QDSP6SS_L1SMEM_DEBUG3_ADDR)
#define HWIO_MSS_QDSP6SS_L1SMEM_DEBUG3_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_L1SMEM_DEBUG3_ADDR, m)
#define HWIO_MSS_QDSP6SS_L1SMEM_DEBUG3_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_L1SMEM_DEBUG3_ADDR,v)
#define HWIO_MSS_QDSP6SS_L1SMEM_DEBUG3_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_L1SMEM_DEBUG3_ADDR,m,v,HWIO_MSS_QDSP6SS_L1SMEM_DEBUG3_IN)
#define HWIO_MSS_QDSP6SS_L1SMEM_DEBUG3_OFF_BMSK                                   0xffffffff
#define HWIO_MSS_QDSP6SS_L1SMEM_DEBUG3_OFF_SHFT                                          0x0

/*----------------------------------------------------------------------------
 * MODULE: MSS_QDSP6SS_QDSP6SS_QTMR_AC
 *--------------------------------------------------------------------------*/

#define MSS_QDSP6SS_QDSP6SS_QTMR_AC_REG_BASE                                (MSS_TOP_BASE      + 0x00120000)
#define MSS_QDSP6SS_QDSP6SS_QTMR_AC_REG_BASE_SIZE                           0x1000
#define MSS_QDSP6SS_QDSP6SS_QTMR_AC_REG_BASE_USED                           0xff0

#define HWIO_MSS_QDSP6SS_QTMR_AC_CNTFRQ_ADDR                                (MSS_QDSP6SS_QDSP6SS_QTMR_AC_REG_BASE      + 0x00000000)
#define HWIO_MSS_QDSP6SS_QTMR_AC_CNTFRQ_RMSK                                0xffffffff
#define HWIO_MSS_QDSP6SS_QTMR_AC_CNTFRQ_IN          \
        in_dword(HWIO_MSS_QDSP6SS_QTMR_AC_CNTFRQ_ADDR)
#define HWIO_MSS_QDSP6SS_QTMR_AC_CNTFRQ_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_QTMR_AC_CNTFRQ_ADDR, m)
#define HWIO_MSS_QDSP6SS_QTMR_AC_CNTFRQ_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_QTMR_AC_CNTFRQ_ADDR,v)
#define HWIO_MSS_QDSP6SS_QTMR_AC_CNTFRQ_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_QTMR_AC_CNTFRQ_ADDR,m,v,HWIO_MSS_QDSP6SS_QTMR_AC_CNTFRQ_IN)
#define HWIO_MSS_QDSP6SS_QTMR_AC_CNTFRQ_CNTFRQ_BMSK                         0xffffffff
#define HWIO_MSS_QDSP6SS_QTMR_AC_CNTFRQ_CNTFRQ_SHFT                                0x0

#define HWIO_MSS_QDSP6SS_QTMR_AC_CNTNSAR_FG0_ADDR                           (MSS_QDSP6SS_QDSP6SS_QTMR_AC_REG_BASE      + 0x00000004)
#define HWIO_MSS_QDSP6SS_QTMR_AC_CNTNSAR_FG0_RMSK                                  0x7
#define HWIO_MSS_QDSP6SS_QTMR_AC_CNTNSAR_FG0_IN          \
        in_dword(HWIO_MSS_QDSP6SS_QTMR_AC_CNTNSAR_FG0_ADDR)
#define HWIO_MSS_QDSP6SS_QTMR_AC_CNTNSAR_FG0_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_QTMR_AC_CNTNSAR_FG0_ADDR, m)
#define HWIO_MSS_QDSP6SS_QTMR_AC_CNTNSAR_FG0_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_QTMR_AC_CNTNSAR_FG0_ADDR,v)
#define HWIO_MSS_QDSP6SS_QTMR_AC_CNTNSAR_FG0_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_QTMR_AC_CNTNSAR_FG0_ADDR,m,v,HWIO_MSS_QDSP6SS_QTMR_AC_CNTNSAR_FG0_IN)
#define HWIO_MSS_QDSP6SS_QTMR_AC_CNTNSAR_FG0_NSN_BMSK                              0x7
#define HWIO_MSS_QDSP6SS_QTMR_AC_CNTNSAR_FG0_NSN_SHFT                              0x0

#define HWIO_MSS_QDSP6SS_QTMR_AC_CNTTIDR_FG0_ADDR                           (MSS_QDSP6SS_QDSP6SS_QTMR_AC_REG_BASE      + 0x00000008)
#define HWIO_MSS_QDSP6SS_QTMR_AC_CNTTIDR_FG0_RMSK                           0xffffffff
#define HWIO_MSS_QDSP6SS_QTMR_AC_CNTTIDR_FG0_IN          \
        in_dword(HWIO_MSS_QDSP6SS_QTMR_AC_CNTTIDR_FG0_ADDR)
#define HWIO_MSS_QDSP6SS_QTMR_AC_CNTTIDR_FG0_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_QTMR_AC_CNTTIDR_FG0_ADDR, m)
#define HWIO_MSS_QDSP6SS_QTMR_AC_CNTTIDR_FG0_F7_CFG_BMSK                    0xf0000000
#define HWIO_MSS_QDSP6SS_QTMR_AC_CNTTIDR_FG0_F7_CFG_SHFT                          0x1c
#define HWIO_MSS_QDSP6SS_QTMR_AC_CNTTIDR_FG0_F6_CFG_BMSK                     0xf000000
#define HWIO_MSS_QDSP6SS_QTMR_AC_CNTTIDR_FG0_F6_CFG_SHFT                          0x18
#define HWIO_MSS_QDSP6SS_QTMR_AC_CNTTIDR_FG0_F5_CFG_BMSK                      0xf00000
#define HWIO_MSS_QDSP6SS_QTMR_AC_CNTTIDR_FG0_F5_CFG_SHFT                          0x14
#define HWIO_MSS_QDSP6SS_QTMR_AC_CNTTIDR_FG0_F4_CFG_BMSK                       0xf0000
#define HWIO_MSS_QDSP6SS_QTMR_AC_CNTTIDR_FG0_F4_CFG_SHFT                          0x10
#define HWIO_MSS_QDSP6SS_QTMR_AC_CNTTIDR_FG0_F3_CFG_BMSK                        0xf000
#define HWIO_MSS_QDSP6SS_QTMR_AC_CNTTIDR_FG0_F3_CFG_SHFT                           0xc
#define HWIO_MSS_QDSP6SS_QTMR_AC_CNTTIDR_FG0_F2_CFG_BMSK                         0xf00
#define HWIO_MSS_QDSP6SS_QTMR_AC_CNTTIDR_FG0_F2_CFG_SHFT                           0x8
#define HWIO_MSS_QDSP6SS_QTMR_AC_CNTTIDR_FG0_F1_CFG_BMSK                          0xf0
#define HWIO_MSS_QDSP6SS_QTMR_AC_CNTTIDR_FG0_F1_CFG_SHFT                           0x4
#define HWIO_MSS_QDSP6SS_QTMR_AC_CNTTIDR_FG0_F0_CFG_BMSK                           0xf
#define HWIO_MSS_QDSP6SS_QTMR_AC_CNTTIDR_FG0_F0_CFG_SHFT                           0x0

#define HWIO_MSS_QDSP6SS_QTMR_AC_CNTACRn_FG0_ADDR(n)                        (MSS_QDSP6SS_QDSP6SS_QTMR_AC_REG_BASE      + 0x00000040 + 0x4 * (n))
#define HWIO_MSS_QDSP6SS_QTMR_AC_CNTACRn_FG0_RMSK                                 0x3f
#define HWIO_MSS_QDSP6SS_QTMR_AC_CNTACRn_FG0_MAXn                                    2
#define HWIO_MSS_QDSP6SS_QTMR_AC_CNTACRn_FG0_INI(n)        \
        in_dword_masked(HWIO_MSS_QDSP6SS_QTMR_AC_CNTACRn_FG0_ADDR(n), HWIO_MSS_QDSP6SS_QTMR_AC_CNTACRn_FG0_RMSK)
#define HWIO_MSS_QDSP6SS_QTMR_AC_CNTACRn_FG0_INMI(n,mask)    \
        in_dword_masked(HWIO_MSS_QDSP6SS_QTMR_AC_CNTACRn_FG0_ADDR(n), mask)
#define HWIO_MSS_QDSP6SS_QTMR_AC_CNTACRn_FG0_OUTI(n,val)    \
        out_dword(HWIO_MSS_QDSP6SS_QTMR_AC_CNTACRn_FG0_ADDR(n),val)
#define HWIO_MSS_QDSP6SS_QTMR_AC_CNTACRn_FG0_OUTMI(n,mask,val) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_QTMR_AC_CNTACRn_FG0_ADDR(n),mask,val,HWIO_MSS_QDSP6SS_QTMR_AC_CNTACRn_FG0_INI(n))
#define HWIO_MSS_QDSP6SS_QTMR_AC_CNTACRn_FG0_RWPT_BMSK                            0x20
#define HWIO_MSS_QDSP6SS_QTMR_AC_CNTACRn_FG0_RWPT_SHFT                             0x5
#define HWIO_MSS_QDSP6SS_QTMR_AC_CNTACRn_FG0_RWVT_BMSK                            0x10
#define HWIO_MSS_QDSP6SS_QTMR_AC_CNTACRn_FG0_RWVT_SHFT                             0x4
#define HWIO_MSS_QDSP6SS_QTMR_AC_CNTACRn_FG0_RVOFF_BMSK                            0x8
#define HWIO_MSS_QDSP6SS_QTMR_AC_CNTACRn_FG0_RVOFF_SHFT                            0x3
#define HWIO_MSS_QDSP6SS_QTMR_AC_CNTACRn_FG0_RFRQ_BMSK                             0x4
#define HWIO_MSS_QDSP6SS_QTMR_AC_CNTACRn_FG0_RFRQ_SHFT                             0x2
#define HWIO_MSS_QDSP6SS_QTMR_AC_CNTACRn_FG0_RPVCT_BMSK                            0x2
#define HWIO_MSS_QDSP6SS_QTMR_AC_CNTACRn_FG0_RPVCT_SHFT                            0x1
#define HWIO_MSS_QDSP6SS_QTMR_AC_CNTACRn_FG0_RPCT_BMSK                             0x1
#define HWIO_MSS_QDSP6SS_QTMR_AC_CNTACRn_FG0_RPCT_SHFT                             0x0

#define HWIO_MSS_QDSP6SS_QTMR_AC_CNTVOFF_FG0_LO_n_ADDR(n)                   (MSS_QDSP6SS_QDSP6SS_QTMR_AC_REG_BASE      + 0x00000080 + 0x8 * (n))
#define HWIO_MSS_QDSP6SS_QTMR_AC_CNTVOFF_FG0_LO_n_RMSK                      0xffffffff
#define HWIO_MSS_QDSP6SS_QTMR_AC_CNTVOFF_FG0_LO_n_MAXn                               2
#define HWIO_MSS_QDSP6SS_QTMR_AC_CNTVOFF_FG0_LO_n_INI(n)        \
        in_dword_masked(HWIO_MSS_QDSP6SS_QTMR_AC_CNTVOFF_FG0_LO_n_ADDR(n), HWIO_MSS_QDSP6SS_QTMR_AC_CNTVOFF_FG0_LO_n_RMSK)
#define HWIO_MSS_QDSP6SS_QTMR_AC_CNTVOFF_FG0_LO_n_INMI(n,mask)    \
        in_dword_masked(HWIO_MSS_QDSP6SS_QTMR_AC_CNTVOFF_FG0_LO_n_ADDR(n), mask)
#define HWIO_MSS_QDSP6SS_QTMR_AC_CNTVOFF_FG0_LO_n_OUTI(n,val)    \
        out_dword(HWIO_MSS_QDSP6SS_QTMR_AC_CNTVOFF_FG0_LO_n_ADDR(n),val)
#define HWIO_MSS_QDSP6SS_QTMR_AC_CNTVOFF_FG0_LO_n_OUTMI(n,mask,val) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_QTMR_AC_CNTVOFF_FG0_LO_n_ADDR(n),mask,val,HWIO_MSS_QDSP6SS_QTMR_AC_CNTVOFF_FG0_LO_n_INI(n))
#define HWIO_MSS_QDSP6SS_QTMR_AC_CNTVOFF_FG0_LO_n_CNTVOFF_LO_BMSK           0xffffffff
#define HWIO_MSS_QDSP6SS_QTMR_AC_CNTVOFF_FG0_LO_n_CNTVOFF_LO_SHFT                  0x0

#define HWIO_MSS_QDSP6SS_QTMR_AC_CNTVOFF_FG0_HI_n_ADDR(n)                   (MSS_QDSP6SS_QDSP6SS_QTMR_AC_REG_BASE      + 0x00000084 + 0x8 * (n))
#define HWIO_MSS_QDSP6SS_QTMR_AC_CNTVOFF_FG0_HI_n_RMSK                        0xffffff
#define HWIO_MSS_QDSP6SS_QTMR_AC_CNTVOFF_FG0_HI_n_MAXn                               2
#define HWIO_MSS_QDSP6SS_QTMR_AC_CNTVOFF_FG0_HI_n_INI(n)        \
        in_dword_masked(HWIO_MSS_QDSP6SS_QTMR_AC_CNTVOFF_FG0_HI_n_ADDR(n), HWIO_MSS_QDSP6SS_QTMR_AC_CNTVOFF_FG0_HI_n_RMSK)
#define HWIO_MSS_QDSP6SS_QTMR_AC_CNTVOFF_FG0_HI_n_INMI(n,mask)    \
        in_dword_masked(HWIO_MSS_QDSP6SS_QTMR_AC_CNTVOFF_FG0_HI_n_ADDR(n), mask)
#define HWIO_MSS_QDSP6SS_QTMR_AC_CNTVOFF_FG0_HI_n_OUTI(n,val)    \
        out_dword(HWIO_MSS_QDSP6SS_QTMR_AC_CNTVOFF_FG0_HI_n_ADDR(n),val)
#define HWIO_MSS_QDSP6SS_QTMR_AC_CNTVOFF_FG0_HI_n_OUTMI(n,mask,val) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_QTMR_AC_CNTVOFF_FG0_HI_n_ADDR(n),mask,val,HWIO_MSS_QDSP6SS_QTMR_AC_CNTVOFF_FG0_HI_n_INI(n))
#define HWIO_MSS_QDSP6SS_QTMR_AC_CNTVOFF_FG0_HI_n_CNTVOFF_HI_BMSK             0xffffff
#define HWIO_MSS_QDSP6SS_QTMR_AC_CNTVOFF_FG0_HI_n_CNTVOFF_HI_SHFT                  0x0

#define HWIO_MSS_QDSP6SS_QTMR_AC_CFG_ADDR                                   (MSS_QDSP6SS_QDSP6SS_QTMR_AC_REG_BASE      + 0x00000fc0)
#define HWIO_MSS_QDSP6SS_QTMR_AC_CFG_RMSK                                          0x3
#define HWIO_MSS_QDSP6SS_QTMR_AC_CFG_IN          \
        in_dword(HWIO_MSS_QDSP6SS_QTMR_AC_CFG_ADDR)
#define HWIO_MSS_QDSP6SS_QTMR_AC_CFG_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_QTMR_AC_CFG_ADDR, m)
#define HWIO_MSS_QDSP6SS_QTMR_AC_CFG_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_QTMR_AC_CFG_ADDR,v)
#define HWIO_MSS_QDSP6SS_QTMR_AC_CFG_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_QTMR_AC_CFG_ADDR,m,v,HWIO_MSS_QDSP6SS_QTMR_AC_CFG_IN)
#define HWIO_MSS_QDSP6SS_QTMR_AC_CFG_DSBL_ATOMIC_BMSK                              0x2
#define HWIO_MSS_QDSP6SS_QTMR_AC_CFG_DSBL_ATOMIC_SHFT                              0x1
#define HWIO_MSS_QDSP6SS_QTMR_AC_CFG_TEST_BUS_EN_BMSK                              0x1
#define HWIO_MSS_QDSP6SS_QTMR_AC_CFG_TEST_BUS_EN_SHFT                              0x0

#define HWIO_MSS_QDSP6SS_QTMR_AC_VERSION_ADDR                               (MSS_QDSP6SS_QDSP6SS_QTMR_AC_REG_BASE      + 0x00000fd0)
#define HWIO_MSS_QDSP6SS_QTMR_AC_VERSION_RMSK                               0xffffffff
#define HWIO_MSS_QDSP6SS_QTMR_AC_VERSION_IN          \
        in_dword(HWIO_MSS_QDSP6SS_QTMR_AC_VERSION_ADDR)
#define HWIO_MSS_QDSP6SS_QTMR_AC_VERSION_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_QTMR_AC_VERSION_ADDR, m)
#define HWIO_MSS_QDSP6SS_QTMR_AC_VERSION_MAJOR_BMSK                         0xf0000000
#define HWIO_MSS_QDSP6SS_QTMR_AC_VERSION_MAJOR_SHFT                               0x1c
#define HWIO_MSS_QDSP6SS_QTMR_AC_VERSION_MINOR_BMSK                          0xfff0000
#define HWIO_MSS_QDSP6SS_QTMR_AC_VERSION_MINOR_SHFT                               0x10
#define HWIO_MSS_QDSP6SS_QTMR_AC_VERSION_STEP_BMSK                              0xffff
#define HWIO_MSS_QDSP6SS_QTMR_AC_VERSION_STEP_SHFT                                 0x0

#define HWIO_MSS_QDSP6SS_QTMR_AC_HW_FRAME_SEL_1_ADDR                        (MSS_QDSP6SS_QDSP6SS_QTMR_AC_REG_BASE      + 0x00000fe0)
#define HWIO_MSS_QDSP6SS_QTMR_AC_HW_FRAME_SEL_1_RMSK                        0xffffffff
#define HWIO_MSS_QDSP6SS_QTMR_AC_HW_FRAME_SEL_1_IN          \
        in_dword(HWIO_MSS_QDSP6SS_QTMR_AC_HW_FRAME_SEL_1_ADDR)
#define HWIO_MSS_QDSP6SS_QTMR_AC_HW_FRAME_SEL_1_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_QTMR_AC_HW_FRAME_SEL_1_ADDR, m)
#define HWIO_MSS_QDSP6SS_QTMR_AC_HW_FRAME_SEL_1_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_QTMR_AC_HW_FRAME_SEL_1_ADDR,v)
#define HWIO_MSS_QDSP6SS_QTMR_AC_HW_FRAME_SEL_1_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_QTMR_AC_HW_FRAME_SEL_1_ADDR,m,v,HWIO_MSS_QDSP6SS_QTMR_AC_HW_FRAME_SEL_1_IN)
#define HWIO_MSS_QDSP6SS_QTMR_AC_HW_FRAME_SEL_1_HW_FRAME_SEL1_BMSK          0xffffffff
#define HWIO_MSS_QDSP6SS_QTMR_AC_HW_FRAME_SEL_1_HW_FRAME_SEL1_SHFT                 0x0

#define HWIO_MSS_QDSP6SS_QTMR_AC_HW_FRAME_SEL_2_ADDR                        (MSS_QDSP6SS_QDSP6SS_QTMR_AC_REG_BASE      + 0x00000ff0)
#define HWIO_MSS_QDSP6SS_QTMR_AC_HW_FRAME_SEL_2_RMSK                        0xffffffff
#define HWIO_MSS_QDSP6SS_QTMR_AC_HW_FRAME_SEL_2_IN          \
        in_dword(HWIO_MSS_QDSP6SS_QTMR_AC_HW_FRAME_SEL_2_ADDR)
#define HWIO_MSS_QDSP6SS_QTMR_AC_HW_FRAME_SEL_2_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_QTMR_AC_HW_FRAME_SEL_2_ADDR, m)
#define HWIO_MSS_QDSP6SS_QTMR_AC_HW_FRAME_SEL_2_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_QTMR_AC_HW_FRAME_SEL_2_ADDR,v)
#define HWIO_MSS_QDSP6SS_QTMR_AC_HW_FRAME_SEL_2_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_QTMR_AC_HW_FRAME_SEL_2_ADDR,m,v,HWIO_MSS_QDSP6SS_QTMR_AC_HW_FRAME_SEL_2_IN)
#define HWIO_MSS_QDSP6SS_QTMR_AC_HW_FRAME_SEL_2_HW_FRAME_SEL2_BMSK          0xffffffff
#define HWIO_MSS_QDSP6SS_QTMR_AC_HW_FRAME_SEL_2_HW_FRAME_SEL2_SHFT                 0x0

/*----------------------------------------------------------------------------
 * MODULE: MSS_QDSP6SS_QTMR_F1_1
 *--------------------------------------------------------------------------*/

#define MSS_QDSP6SS_QTMR_F1_1_REG_BASE                                     (MSS_TOP_BASE      + 0x00122000)
#define MSS_QDSP6SS_QTMR_F1_1_REG_BASE_SIZE                                0x1000
#define MSS_QDSP6SS_QTMR_F1_1_REG_BASE_USED                                0xfd0

#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTPCT_LO_1_ADDR                          (MSS_QDSP6SS_QTMR_F1_1_REG_BASE      + 0x00000000)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTPCT_LO_1_RMSK                          0xffffffff
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTPCT_LO_1_IN          \
        in_dword(HWIO_MSS_QDSP6SS_QTMR_V1_CNTPCT_LO_1_ADDR)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTPCT_LO_1_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_QTMR_V1_CNTPCT_LO_1_ADDR, m)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTPCT_LO_1_CNTPCT_LO_BMSK                0xffffffff
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTPCT_LO_1_CNTPCT_LO_SHFT                       0x0

#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTPCT_HI_1_ADDR                          (MSS_QDSP6SS_QTMR_F1_1_REG_BASE      + 0x00000004)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTPCT_HI_1_RMSK                            0xffffff
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTPCT_HI_1_IN          \
        in_dword(HWIO_MSS_QDSP6SS_QTMR_V1_CNTPCT_HI_1_ADDR)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTPCT_HI_1_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_QTMR_V1_CNTPCT_HI_1_ADDR, m)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTPCT_HI_1_CNTPCT_HI_BMSK                  0xffffff
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTPCT_HI_1_CNTPCT_HI_SHFT                       0x0

#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTVCT_LO_1_ADDR                          (MSS_QDSP6SS_QTMR_F1_1_REG_BASE      + 0x00000008)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTVCT_LO_1_RMSK                          0xffffffff
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTVCT_LO_1_IN          \
        in_dword(HWIO_MSS_QDSP6SS_QTMR_V1_CNTVCT_LO_1_ADDR)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTVCT_LO_1_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_QTMR_V1_CNTVCT_LO_1_ADDR, m)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTVCT_LO_1_CNTVCT_LO_BMSK                0xffffffff
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTVCT_LO_1_CNTVCT_LO_SHFT                       0x0

#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTVCT_HI_1_ADDR                          (MSS_QDSP6SS_QTMR_F1_1_REG_BASE      + 0x0000000c)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTVCT_HI_1_RMSK                            0xffffff
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTVCT_HI_1_IN          \
        in_dword(HWIO_MSS_QDSP6SS_QTMR_V1_CNTVCT_HI_1_ADDR)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTVCT_HI_1_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_QTMR_V1_CNTVCT_HI_1_ADDR, m)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTVCT_HI_1_CNTVCT_HI_BMSK                  0xffffff
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTVCT_HI_1_CNTVCT_HI_SHFT                       0x0

#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTFRQ_1_ADDR                             (MSS_QDSP6SS_QTMR_F1_1_REG_BASE      + 0x00000010)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTFRQ_1_RMSK                             0xffffffff
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTFRQ_1_IN          \
        in_dword(HWIO_MSS_QDSP6SS_QTMR_V1_CNTFRQ_1_ADDR)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTFRQ_1_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_QTMR_V1_CNTFRQ_1_ADDR, m)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTFRQ_1_CNTFRQ_BMSK                      0xffffffff
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTFRQ_1_CNTFRQ_SHFT                             0x0

#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTPL0ACR_1_ADDR                          (MSS_QDSP6SS_QTMR_F1_1_REG_BASE      + 0x00000014)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTPL0ACR_1_RMSK                               0x303
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTPL0ACR_1_IN          \
        in_dword(HWIO_MSS_QDSP6SS_QTMR_V1_CNTPL0ACR_1_ADDR)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTPL0ACR_1_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_QTMR_V1_CNTPL0ACR_1_ADDR, m)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTPL0ACR_1_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_QTMR_V1_CNTPL0ACR_1_ADDR,v)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTPL0ACR_1_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_QTMR_V1_CNTPL0ACR_1_ADDR,m,v,HWIO_MSS_QDSP6SS_QTMR_V1_CNTPL0ACR_1_IN)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTPL0ACR_1_PL0CTEN_BMSK                       0x200
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTPL0ACR_1_PL0CTEN_SHFT                         0x9
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTPL0ACR_1_PL0VTEN_BMSK                       0x100
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTPL0ACR_1_PL0VTEN_SHFT                         0x8
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTPL0ACR_1_PL0VCTEN_BMSK                        0x2
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTPL0ACR_1_PL0VCTEN_SHFT                        0x1
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTPL0ACR_1_PL0PCTEN_BMSK                        0x1
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTPL0ACR_1_PL0PCTEN_SHFT                        0x0

#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTVOFF_LO_1_ADDR                         (MSS_QDSP6SS_QTMR_F1_1_REG_BASE      + 0x00000018)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTVOFF_LO_1_RMSK                         0xffffffff
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTVOFF_LO_1_IN          \
        in_dword(HWIO_MSS_QDSP6SS_QTMR_V1_CNTVOFF_LO_1_ADDR)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTVOFF_LO_1_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_QTMR_V1_CNTVOFF_LO_1_ADDR, m)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTVOFF_LO_1_CNTVOFF_L0_BMSK              0xffffffff
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTVOFF_LO_1_CNTVOFF_L0_SHFT                     0x0

#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTVOFF_HI_1_ADDR                         (MSS_QDSP6SS_QTMR_F1_1_REG_BASE      + 0x0000001c)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTVOFF_HI_1_RMSK                           0xffffff
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTVOFF_HI_1_IN          \
        in_dword(HWIO_MSS_QDSP6SS_QTMR_V1_CNTVOFF_HI_1_ADDR)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTVOFF_HI_1_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_QTMR_V1_CNTVOFF_HI_1_ADDR, m)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTVOFF_HI_1_CNTVOFF_HI_BMSK                0xffffff
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTVOFF_HI_1_CNTVOFF_HI_SHFT                     0x0

#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_CVAL_LO_1_ADDR                       (MSS_QDSP6SS_QTMR_F1_1_REG_BASE      + 0x00000020)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_CVAL_LO_1_RMSK                       0xffffffff
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_CVAL_LO_1_IN          \
        in_dword(HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_CVAL_LO_1_ADDR)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_CVAL_LO_1_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_CVAL_LO_1_ADDR, m)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_CVAL_LO_1_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_CVAL_LO_1_ADDR,v)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_CVAL_LO_1_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_CVAL_LO_1_ADDR,m,v,HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_CVAL_LO_1_IN)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_CVAL_LO_1_CNTP_CVAL_L0_BMSK          0xffffffff
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_CVAL_LO_1_CNTP_CVAL_L0_SHFT                 0x0

#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_CVAL_HI_1_ADDR                       (MSS_QDSP6SS_QTMR_F1_1_REG_BASE      + 0x00000024)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_CVAL_HI_1_RMSK                         0xffffff
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_CVAL_HI_1_IN          \
        in_dword(HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_CVAL_HI_1_ADDR)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_CVAL_HI_1_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_CVAL_HI_1_ADDR, m)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_CVAL_HI_1_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_CVAL_HI_1_ADDR,v)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_CVAL_HI_1_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_CVAL_HI_1_ADDR,m,v,HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_CVAL_HI_1_IN)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_CVAL_HI_1_CNTP_CVAL_HI_BMSK            0xffffff
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_CVAL_HI_1_CNTP_CVAL_HI_SHFT                 0x0

#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_TVAL_1_ADDR                          (MSS_QDSP6SS_QTMR_F1_1_REG_BASE      + 0x00000028)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_TVAL_1_RMSK                          0xffffffff
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_TVAL_1_IN          \
        in_dword(HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_TVAL_1_ADDR)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_TVAL_1_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_TVAL_1_ADDR, m)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_TVAL_1_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_TVAL_1_ADDR,v)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_TVAL_1_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_TVAL_1_ADDR,m,v,HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_TVAL_1_IN)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_TVAL_1_CNTP_TVAL_BMSK                0xffffffff
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_TVAL_1_CNTP_TVAL_SHFT                       0x0

#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_CTL_1_ADDR                           (MSS_QDSP6SS_QTMR_F1_1_REG_BASE      + 0x0000002c)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_CTL_1_RMSK                                  0x7
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_CTL_1_IN          \
        in_dword(HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_CTL_1_ADDR)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_CTL_1_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_CTL_1_ADDR, m)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_CTL_1_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_CTL_1_ADDR,v)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_CTL_1_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_CTL_1_ADDR,m,v,HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_CTL_1_IN)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_CTL_1_ISTAT_BMSK                            0x4
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_CTL_1_ISTAT_SHFT                            0x2
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_CTL_1_IMSK_BMSK                             0x2
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_CTL_1_IMSK_SHFT                             0x1
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_CTL_1_EN_BMSK                               0x1
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_CTL_1_EN_SHFT                               0x0

#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_CVAL_LO_1_ADDR                       (MSS_QDSP6SS_QTMR_F1_1_REG_BASE      + 0x00000030)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_CVAL_LO_1_RMSK                       0xffffffff
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_CVAL_LO_1_IN          \
        in_dword(HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_CVAL_LO_1_ADDR)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_CVAL_LO_1_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_CVAL_LO_1_ADDR, m)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_CVAL_LO_1_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_CVAL_LO_1_ADDR,v)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_CVAL_LO_1_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_CVAL_LO_1_ADDR,m,v,HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_CVAL_LO_1_IN)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_CVAL_LO_1_CNTV_CVAL_L0_BMSK          0xffffffff
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_CVAL_LO_1_CNTV_CVAL_L0_SHFT                 0x0

#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_CVAL_HI_1_ADDR                       (MSS_QDSP6SS_QTMR_F1_1_REG_BASE      + 0x00000034)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_CVAL_HI_1_RMSK                         0xffffff
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_CVAL_HI_1_IN          \
        in_dword(HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_CVAL_HI_1_ADDR)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_CVAL_HI_1_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_CVAL_HI_1_ADDR, m)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_CVAL_HI_1_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_CVAL_HI_1_ADDR,v)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_CVAL_HI_1_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_CVAL_HI_1_ADDR,m,v,HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_CVAL_HI_1_IN)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_CVAL_HI_1_CNTV_CVAL_HI_BMSK            0xffffff
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_CVAL_HI_1_CNTV_CVAL_HI_SHFT                 0x0

#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_TVAL_1_ADDR                          (MSS_QDSP6SS_QTMR_F1_1_REG_BASE      + 0x00000038)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_TVAL_1_RMSK                          0xffffffff
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_TVAL_1_IN          \
        in_dword(HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_TVAL_1_ADDR)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_TVAL_1_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_TVAL_1_ADDR, m)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_TVAL_1_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_TVAL_1_ADDR,v)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_TVAL_1_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_TVAL_1_ADDR,m,v,HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_TVAL_1_IN)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_TVAL_1_CNTV_TVAL_BMSK                0xffffffff
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_TVAL_1_CNTV_TVAL_SHFT                       0x0

#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_CTL_1_ADDR                           (MSS_QDSP6SS_QTMR_F1_1_REG_BASE      + 0x0000003c)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_CTL_1_RMSK                                  0x7
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_CTL_1_IN          \
        in_dword(HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_CTL_1_ADDR)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_CTL_1_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_CTL_1_ADDR, m)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_CTL_1_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_CTL_1_ADDR,v)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_CTL_1_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_CTL_1_ADDR,m,v,HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_CTL_1_IN)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_CTL_1_ISTAT_BMSK                            0x4
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_CTL_1_ISTAT_SHFT                            0x2
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_CTL_1_IMSK_BMSK                             0x2
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_CTL_1_IMSK_SHFT                             0x1
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_CTL_1_EN_BMSK                               0x1
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_CTL_1_EN_SHFT                               0x0

#define HWIO_MSS_QDSP6SS_QTMR_V1_VERSION_1_ADDR                            (MSS_QDSP6SS_QTMR_F1_1_REG_BASE      + 0x00000fd0)
#define HWIO_MSS_QDSP6SS_QTMR_V1_VERSION_1_RMSK                            0xffffffff
#define HWIO_MSS_QDSP6SS_QTMR_V1_VERSION_1_IN          \
        in_dword(HWIO_MSS_QDSP6SS_QTMR_V1_VERSION_1_ADDR)
#define HWIO_MSS_QDSP6SS_QTMR_V1_VERSION_1_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_QTMR_V1_VERSION_1_ADDR, m)
#define HWIO_MSS_QDSP6SS_QTMR_V1_VERSION_1_MAJOR_BMSK                      0xf0000000
#define HWIO_MSS_QDSP6SS_QTMR_V1_VERSION_1_MAJOR_SHFT                            0x1c
#define HWIO_MSS_QDSP6SS_QTMR_V1_VERSION_1_MINOR_BMSK                       0xfff0000
#define HWIO_MSS_QDSP6SS_QTMR_V1_VERSION_1_MINOR_SHFT                            0x10
#define HWIO_MSS_QDSP6SS_QTMR_V1_VERSION_1_STEP_BMSK                           0xffff
#define HWIO_MSS_QDSP6SS_QTMR_V1_VERSION_1_STEP_SHFT                              0x0

#endif /* __HALCLKHWIOASM_H__ */
