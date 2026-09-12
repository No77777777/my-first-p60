#ifndef __UIMATHERTONHWIOMACROS_H__
#define __UIMATHERTONHWIOMACROS_H__
/*
===========================================================================
*/
/**
    @file uimathertonhwiomacros.h
    @brief Auto-generated HWIO interface include file.

    Reference chip release:
        SDW5100 (Atherton) [atherton_v1.0_p3q2r65]
 
    This file contains HWIO register definitions for the following modules:
        MSS_CC_MSS_CC_REG
        MSS_UIM0_UART_DM
        MSS_UIM1_UART_DM
        MSS_UIM2_UART_DM
        MSS_UIM3_UART_DM
        MSS_PERPH_MSS_PERPH
        SSC_TLMM_CSR


    Generation parameters: 
    { 'filename': 'uimathertonhwiomacros.h',
      'header': '#include "msmhwiobase.h"',
      'module-filter-exclude': {},
      'module-filter-include': {},
      'modules': [ 'MSS_CC_MSS_CC_REG',
                   'MSS_UIM0_UART_DM',
                   'MSS_UIM1_UART_DM',
                   'MSS_UIM2_UART_DM',
                   'MSS_UIM3_UART_DM',
                   'MSS_PERPH_MSS_PERPH',
                   'SSC_TLMM_CSR']}
*/
/*
    ===========================================================================

    Copyright (c) 2020 Qualcomm Technologies, Inc.
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

    $Header: //components/rel/uim.mpss/6.1.0/uimdrv/src/uimathertonhwiomacros.h#1 $
    $DateTime: 2020/12/24 03:59:46 $
    $Author: pwbldsvc $

    ===========================================================================
*/

#include "msmhwiobase.h"

/*----------------------------------------------------------------------------
 * MODULE: MSS_CC_MSS_CC_REG
 *--------------------------------------------------------------------------*/

#define MSS_CC_MSS_CC_REG_REG_BASE                                                                  (MSS_TOP_BASE            + 0x00180000)
#define MSS_CC_MSS_CC_REG_REG_BASE_SIZE                                                             0x176d
#define MSS_CC_MSS_CC_REG_REG_BASE_USED                                                             0x1420

#define HWIO_MSS_CC_XO_TO_Q6_MODEM_BCR_ADDR                                                         (MSS_CC_MSS_CC_REG_REG_BASE            + 0x21c)
#define HWIO_MSS_CC_XO_TO_Q6_MODEM_BCR_RMSK                                                                0x1
#define HWIO_MSS_CC_XO_TO_Q6_MODEM_BCR_IN                    \
                in_dword(HWIO_MSS_CC_XO_TO_Q6_MODEM_BCR_ADDR)
#define HWIO_MSS_CC_XO_TO_Q6_MODEM_BCR_INM(m)            \
                in_dword_masked(HWIO_MSS_CC_XO_TO_Q6_MODEM_BCR_ADDR, m)
#define HWIO_MSS_CC_XO_TO_Q6_MODEM_BCR_OUT(v)            \
                out_dword(HWIO_MSS_CC_XO_TO_Q6_MODEM_BCR_ADDR,v)
#define HWIO_MSS_CC_XO_TO_Q6_MODEM_BCR_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_CC_XO_TO_Q6_MODEM_BCR_ADDR,m,v,HWIO_MSS_CC_XO_TO_Q6_MODEM_BCR_IN)
#define HWIO_MSS_CC_XO_TO_Q6_MODEM_BCR_BLK_ARES_BMSK                                                       0x1
#define HWIO_MSS_CC_XO_TO_Q6_MODEM_BCR_BLK_ARES_SHFT                                                         0

#define HWIO_MSS_CC_XO_CMD_RCGR_ADDR                                                                (MSS_CC_MSS_CC_REG_REG_BASE            + 0x230)
#define HWIO_MSS_CC_XO_CMD_RCGR_RMSK                                                                0x80000013
#define HWIO_MSS_CC_XO_CMD_RCGR_IN                    \
                in_dword(HWIO_MSS_CC_XO_CMD_RCGR_ADDR)
#define HWIO_MSS_CC_XO_CMD_RCGR_INM(m)            \
                in_dword_masked(HWIO_MSS_CC_XO_CMD_RCGR_ADDR, m)
#define HWIO_MSS_CC_XO_CMD_RCGR_OUT(v)            \
                out_dword(HWIO_MSS_CC_XO_CMD_RCGR_ADDR,v)
#define HWIO_MSS_CC_XO_CMD_RCGR_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_CC_XO_CMD_RCGR_ADDR,m,v,HWIO_MSS_CC_XO_CMD_RCGR_IN)
#define HWIO_MSS_CC_XO_CMD_RCGR_ROOT_OFF_BMSK                                                       0x80000000
#define HWIO_MSS_CC_XO_CMD_RCGR_ROOT_OFF_SHFT                                                               31
#define HWIO_MSS_CC_XO_CMD_RCGR_DIRTY_CFG_RCGR_BMSK                                                       0x10
#define HWIO_MSS_CC_XO_CMD_RCGR_DIRTY_CFG_RCGR_SHFT                                                          4
#define HWIO_MSS_CC_XO_CMD_RCGR_ROOT_EN_BMSK                                                               0x2
#define HWIO_MSS_CC_XO_CMD_RCGR_ROOT_EN_SHFT                                                                 1
#define HWIO_MSS_CC_XO_CMD_RCGR_UPDATE_BMSK                                                                0x1
#define HWIO_MSS_CC_XO_CMD_RCGR_UPDATE_SHFT                                                                  0

#define HWIO_MSS_CC_XO_CFG_RCGR_ADDR                                                                (MSS_CC_MSS_CC_REG_REG_BASE            + 0x234)
#define HWIO_MSS_CC_XO_CFG_RCGR_RMSK                                                                  0x11071f
#define HWIO_MSS_CC_XO_CFG_RCGR_IN                    \
                in_dword(HWIO_MSS_CC_XO_CFG_RCGR_ADDR)
#define HWIO_MSS_CC_XO_CFG_RCGR_INM(m)            \
                in_dword_masked(HWIO_MSS_CC_XO_CFG_RCGR_ADDR, m)
#define HWIO_MSS_CC_XO_CFG_RCGR_OUT(v)            \
                out_dword(HWIO_MSS_CC_XO_CFG_RCGR_ADDR,v)
#define HWIO_MSS_CC_XO_CFG_RCGR_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_CC_XO_CFG_RCGR_ADDR,m,v,HWIO_MSS_CC_XO_CFG_RCGR_IN)
#define HWIO_MSS_CC_XO_CFG_RCGR_HW_CLK_CONTROL_BMSK                                                   0x100000
#define HWIO_MSS_CC_XO_CFG_RCGR_HW_CLK_CONTROL_SHFT                                                         20
#define HWIO_MSS_CC_XO_CFG_RCGR_RCGLITE_DISABLE_BMSK                                                   0x10000
#define HWIO_MSS_CC_XO_CFG_RCGR_RCGLITE_DISABLE_SHFT                                                        16
#define HWIO_MSS_CC_XO_CFG_RCGR_SRC_SEL_BMSK                                                             0x700
#define HWIO_MSS_CC_XO_CFG_RCGR_SRC_SEL_SHFT                                                                 8
#define HWIO_MSS_CC_XO_CFG_RCGR_SRC_DIV_BMSK                                                              0x1f
#define HWIO_MSS_CC_XO_CFG_RCGR_SRC_DIV_SHFT                                                                 0

#define HWIO_MSS_CC_QTIMER_SYNC_XO_CBCR_ADDR                                                        (MSS_CC_MSS_CC_REG_REG_BASE            + 0x258)
#define HWIO_MSS_CC_QTIMER_SYNC_XO_CBCR_RMSK                                                        0x81c00005
#define HWIO_MSS_CC_QTIMER_SYNC_XO_CBCR_IN                    \
                in_dword(HWIO_MSS_CC_QTIMER_SYNC_XO_CBCR_ADDR)
#define HWIO_MSS_CC_QTIMER_SYNC_XO_CBCR_INM(m)            \
                in_dword_masked(HWIO_MSS_CC_QTIMER_SYNC_XO_CBCR_ADDR, m)
#define HWIO_MSS_CC_QTIMER_SYNC_XO_CBCR_OUT(v)            \
                out_dword(HWIO_MSS_CC_QTIMER_SYNC_XO_CBCR_ADDR,v)
#define HWIO_MSS_CC_QTIMER_SYNC_XO_CBCR_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_CC_QTIMER_SYNC_XO_CBCR_ADDR,m,v,HWIO_MSS_CC_QTIMER_SYNC_XO_CBCR_IN)
#define HWIO_MSS_CC_QTIMER_SYNC_XO_CBCR_CLK_OFF_BMSK                                                0x80000000
#define HWIO_MSS_CC_QTIMER_SYNC_XO_CBCR_CLK_OFF_SHFT                                                        31
#define HWIO_MSS_CC_QTIMER_SYNC_XO_CBCR_IGNORE_ALL_ARES_BMSK                                         0x1000000
#define HWIO_MSS_CC_QTIMER_SYNC_XO_CBCR_IGNORE_ALL_ARES_SHFT                                                24
#define HWIO_MSS_CC_QTIMER_SYNC_XO_CBCR_IGNORE_ALL_CLK_DIS_BMSK                                       0x800000
#define HWIO_MSS_CC_QTIMER_SYNC_XO_CBCR_IGNORE_ALL_CLK_DIS_SHFT                                             23
#define HWIO_MSS_CC_QTIMER_SYNC_XO_CBCR_CLK_DIS_BMSK                                                  0x400000
#define HWIO_MSS_CC_QTIMER_SYNC_XO_CBCR_CLK_DIS_SHFT                                                        22
#define HWIO_MSS_CC_QTIMER_SYNC_XO_CBCR_CLK_ARES_BMSK                                                      0x4
#define HWIO_MSS_CC_QTIMER_SYNC_XO_CBCR_CLK_ARES_SHFT                                                        2
#define HWIO_MSS_CC_QTIMER_SYNC_XO_CBCR_CLK_ENABLE_BMSK                                                    0x1
#define HWIO_MSS_CC_QTIMER_SYNC_XO_CBCR_CLK_ENABLE_SHFT                                                      0

#define HWIO_MSS_CC_BUS_BCR_ADDR                                                                    (MSS_CC_MSS_CC_REG_REG_BASE            + 0x2a8)
#define HWIO_MSS_CC_BUS_BCR_RMSK                                                                           0x1
#define HWIO_MSS_CC_BUS_BCR_IN                    \
                in_dword(HWIO_MSS_CC_BUS_BCR_ADDR)
#define HWIO_MSS_CC_BUS_BCR_INM(m)            \
                in_dword_masked(HWIO_MSS_CC_BUS_BCR_ADDR, m)
#define HWIO_MSS_CC_BUS_BCR_OUT(v)            \
                out_dword(HWIO_MSS_CC_BUS_BCR_ADDR,v)
#define HWIO_MSS_CC_BUS_BCR_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_CC_BUS_BCR_ADDR,m,v,HWIO_MSS_CC_BUS_BCR_IN)
#define HWIO_MSS_CC_BUS_BCR_BLK_ARES_BMSK                                                                  0x1
#define HWIO_MSS_CC_BUS_BCR_BLK_ARES_SHFT                                                                    0

#define HWIO_MSS_CC_BUS_PRE_MUX_CMD_RCGR_ADDR                                                       (MSS_CC_MSS_CC_REG_REG_BASE            + 0x2bc)
#define HWIO_MSS_CC_BUS_PRE_MUX_CMD_RCGR_RMSK                                                       0x80000013
#define HWIO_MSS_CC_BUS_PRE_MUX_CMD_RCGR_IN                    \
                in_dword(HWIO_MSS_CC_BUS_PRE_MUX_CMD_RCGR_ADDR)
#define HWIO_MSS_CC_BUS_PRE_MUX_CMD_RCGR_INM(m)            \
                in_dword_masked(HWIO_MSS_CC_BUS_PRE_MUX_CMD_RCGR_ADDR, m)
#define HWIO_MSS_CC_BUS_PRE_MUX_CMD_RCGR_OUT(v)            \
                out_dword(HWIO_MSS_CC_BUS_PRE_MUX_CMD_RCGR_ADDR,v)
#define HWIO_MSS_CC_BUS_PRE_MUX_CMD_RCGR_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_CC_BUS_PRE_MUX_CMD_RCGR_ADDR,m,v,HWIO_MSS_CC_BUS_PRE_MUX_CMD_RCGR_IN)
#define HWIO_MSS_CC_BUS_PRE_MUX_CMD_RCGR_ROOT_OFF_BMSK                                              0x80000000
#define HWIO_MSS_CC_BUS_PRE_MUX_CMD_RCGR_ROOT_OFF_SHFT                                                      31
#define HWIO_MSS_CC_BUS_PRE_MUX_CMD_RCGR_DIRTY_CFG_RCGR_BMSK                                              0x10
#define HWIO_MSS_CC_BUS_PRE_MUX_CMD_RCGR_DIRTY_CFG_RCGR_SHFT                                                 4
#define HWIO_MSS_CC_BUS_PRE_MUX_CMD_RCGR_ROOT_EN_BMSK                                                      0x2
#define HWIO_MSS_CC_BUS_PRE_MUX_CMD_RCGR_ROOT_EN_SHFT                                                        1
#define HWIO_MSS_CC_BUS_PRE_MUX_CMD_RCGR_UPDATE_BMSK                                                       0x1
#define HWIO_MSS_CC_BUS_PRE_MUX_CMD_RCGR_UPDATE_SHFT                                                         0

#define HWIO_MSS_CC_BUS_PRE_MUX_CFG_RCGR_ADDR                                                       (MSS_CC_MSS_CC_REG_REG_BASE            + 0x2c0)
#define HWIO_MSS_CC_BUS_PRE_MUX_CFG_RCGR_RMSK                                                         0x11071f
#define HWIO_MSS_CC_BUS_PRE_MUX_CFG_RCGR_IN                    \
                in_dword(HWIO_MSS_CC_BUS_PRE_MUX_CFG_RCGR_ADDR)
#define HWIO_MSS_CC_BUS_PRE_MUX_CFG_RCGR_INM(m)            \
                in_dword_masked(HWIO_MSS_CC_BUS_PRE_MUX_CFG_RCGR_ADDR, m)
#define HWIO_MSS_CC_BUS_PRE_MUX_CFG_RCGR_OUT(v)            \
                out_dword(HWIO_MSS_CC_BUS_PRE_MUX_CFG_RCGR_ADDR,v)
#define HWIO_MSS_CC_BUS_PRE_MUX_CFG_RCGR_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_CC_BUS_PRE_MUX_CFG_RCGR_ADDR,m,v,HWIO_MSS_CC_BUS_PRE_MUX_CFG_RCGR_IN)
#define HWIO_MSS_CC_BUS_PRE_MUX_CFG_RCGR_HW_CLK_CONTROL_BMSK                                          0x100000
#define HWIO_MSS_CC_BUS_PRE_MUX_CFG_RCGR_HW_CLK_CONTROL_SHFT                                                20
#define HWIO_MSS_CC_BUS_PRE_MUX_CFG_RCGR_RCGLITE_DISABLE_BMSK                                          0x10000
#define HWIO_MSS_CC_BUS_PRE_MUX_CFG_RCGR_RCGLITE_DISABLE_SHFT                                               16
#define HWIO_MSS_CC_BUS_PRE_MUX_CFG_RCGR_SRC_SEL_BMSK                                                    0x700
#define HWIO_MSS_CC_BUS_PRE_MUX_CFG_RCGR_SRC_SEL_SHFT                                                        8
#define HWIO_MSS_CC_BUS_PRE_MUX_CFG_RCGR_SRC_DIV_BMSK                                                     0x1f
#define HWIO_MSS_CC_BUS_PRE_MUX_CFG_RCGR_SRC_DIV_SHFT                                                        0

#define HWIO_MSS_CC_MODEM_BUS_PRE_MUX_CMD_RCGR_ADDR                                                 (MSS_CC_MSS_CC_REG_REG_BASE            + 0x2e4)
#define HWIO_MSS_CC_MODEM_BUS_PRE_MUX_CMD_RCGR_RMSK                                                 0x80000013
#define HWIO_MSS_CC_MODEM_BUS_PRE_MUX_CMD_RCGR_IN                    \
                in_dword(HWIO_MSS_CC_MODEM_BUS_PRE_MUX_CMD_RCGR_ADDR)
#define HWIO_MSS_CC_MODEM_BUS_PRE_MUX_CMD_RCGR_INM(m)            \
                in_dword_masked(HWIO_MSS_CC_MODEM_BUS_PRE_MUX_CMD_RCGR_ADDR, m)
#define HWIO_MSS_CC_MODEM_BUS_PRE_MUX_CMD_RCGR_OUT(v)            \
                out_dword(HWIO_MSS_CC_MODEM_BUS_PRE_MUX_CMD_RCGR_ADDR,v)
#define HWIO_MSS_CC_MODEM_BUS_PRE_MUX_CMD_RCGR_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_CC_MODEM_BUS_PRE_MUX_CMD_RCGR_ADDR,m,v,HWIO_MSS_CC_MODEM_BUS_PRE_MUX_CMD_RCGR_IN)
#define HWIO_MSS_CC_MODEM_BUS_PRE_MUX_CMD_RCGR_ROOT_OFF_BMSK                                        0x80000000
#define HWIO_MSS_CC_MODEM_BUS_PRE_MUX_CMD_RCGR_ROOT_OFF_SHFT                                                31
#define HWIO_MSS_CC_MODEM_BUS_PRE_MUX_CMD_RCGR_DIRTY_CFG_RCGR_BMSK                                        0x10
#define HWIO_MSS_CC_MODEM_BUS_PRE_MUX_CMD_RCGR_DIRTY_CFG_RCGR_SHFT                                           4
#define HWIO_MSS_CC_MODEM_BUS_PRE_MUX_CMD_RCGR_ROOT_EN_BMSK                                                0x2
#define HWIO_MSS_CC_MODEM_BUS_PRE_MUX_CMD_RCGR_ROOT_EN_SHFT                                                  1
#define HWIO_MSS_CC_MODEM_BUS_PRE_MUX_CMD_RCGR_UPDATE_BMSK                                                 0x1
#define HWIO_MSS_CC_MODEM_BUS_PRE_MUX_CMD_RCGR_UPDATE_SHFT                                                   0

#define HWIO_MSS_CC_MODEM_BUS_PRE_MUX_CFG_RCGR_ADDR                                                 (MSS_CC_MSS_CC_REG_REG_BASE            + 0x2e8)
#define HWIO_MSS_CC_MODEM_BUS_PRE_MUX_CFG_RCGR_RMSK                                                   0x11071f
#define HWIO_MSS_CC_MODEM_BUS_PRE_MUX_CFG_RCGR_IN                    \
                in_dword(HWIO_MSS_CC_MODEM_BUS_PRE_MUX_CFG_RCGR_ADDR)
#define HWIO_MSS_CC_MODEM_BUS_PRE_MUX_CFG_RCGR_INM(m)            \
                in_dword_masked(HWIO_MSS_CC_MODEM_BUS_PRE_MUX_CFG_RCGR_ADDR, m)
#define HWIO_MSS_CC_MODEM_BUS_PRE_MUX_CFG_RCGR_OUT(v)            \
                out_dword(HWIO_MSS_CC_MODEM_BUS_PRE_MUX_CFG_RCGR_ADDR,v)
#define HWIO_MSS_CC_MODEM_BUS_PRE_MUX_CFG_RCGR_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_CC_MODEM_BUS_PRE_MUX_CFG_RCGR_ADDR,m,v,HWIO_MSS_CC_MODEM_BUS_PRE_MUX_CFG_RCGR_IN)
#define HWIO_MSS_CC_MODEM_BUS_PRE_MUX_CFG_RCGR_HW_CLK_CONTROL_BMSK                                    0x100000
#define HWIO_MSS_CC_MODEM_BUS_PRE_MUX_CFG_RCGR_HW_CLK_CONTROL_SHFT                                          20
#define HWIO_MSS_CC_MODEM_BUS_PRE_MUX_CFG_RCGR_RCGLITE_DISABLE_BMSK                                    0x10000
#define HWIO_MSS_CC_MODEM_BUS_PRE_MUX_CFG_RCGR_RCGLITE_DISABLE_SHFT                                         16
#define HWIO_MSS_CC_MODEM_BUS_PRE_MUX_CFG_RCGR_SRC_SEL_BMSK                                              0x700
#define HWIO_MSS_CC_MODEM_BUS_PRE_MUX_CFG_RCGR_SRC_SEL_SHFT                                                  8
#define HWIO_MSS_CC_MODEM_BUS_PRE_MUX_CFG_RCGR_SRC_DIV_BMSK                                               0x1f
#define HWIO_MSS_CC_MODEM_BUS_PRE_MUX_CFG_RCGR_SRC_DIV_SHFT                                                  0

#define HWIO_MSS_CC_COXM_BUS_CBCR_ADDR                                                              (MSS_CC_MSS_CC_REG_REG_BASE            + 0x30c)
#define HWIO_MSS_CC_COXM_BUS_CBCR_RMSK                                                              0x81c07ff5
#define HWIO_MSS_CC_COXM_BUS_CBCR_IN                    \
                in_dword(HWIO_MSS_CC_COXM_BUS_CBCR_ADDR)
#define HWIO_MSS_CC_COXM_BUS_CBCR_INM(m)            \
                in_dword_masked(HWIO_MSS_CC_COXM_BUS_CBCR_ADDR, m)
#define HWIO_MSS_CC_COXM_BUS_CBCR_OUT(v)            \
                out_dword(HWIO_MSS_CC_COXM_BUS_CBCR_ADDR,v)
#define HWIO_MSS_CC_COXM_BUS_CBCR_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_CC_COXM_BUS_CBCR_ADDR,m,v,HWIO_MSS_CC_COXM_BUS_CBCR_IN)
#define HWIO_MSS_CC_COXM_BUS_CBCR_CLK_OFF_BMSK                                                      0x80000000
#define HWIO_MSS_CC_COXM_BUS_CBCR_CLK_OFF_SHFT                                                              31
#define HWIO_MSS_CC_COXM_BUS_CBCR_IGNORE_ALL_ARES_BMSK                                               0x1000000
#define HWIO_MSS_CC_COXM_BUS_CBCR_IGNORE_ALL_ARES_SHFT                                                      24
#define HWIO_MSS_CC_COXM_BUS_CBCR_IGNORE_ALL_CLK_DIS_BMSK                                             0x800000
#define HWIO_MSS_CC_COXM_BUS_CBCR_IGNORE_ALL_CLK_DIS_SHFT                                                   23
#define HWIO_MSS_CC_COXM_BUS_CBCR_CLK_DIS_BMSK                                                        0x400000
#define HWIO_MSS_CC_COXM_BUS_CBCR_CLK_DIS_SHFT                                                              22
#define HWIO_MSS_CC_COXM_BUS_CBCR_FORCE_MEM_CORE_ON_BMSK                                                0x4000
#define HWIO_MSS_CC_COXM_BUS_CBCR_FORCE_MEM_CORE_ON_SHFT                                                    14
#define HWIO_MSS_CC_COXM_BUS_CBCR_FORCE_MEM_PERIPH_ON_BMSK                                              0x2000
#define HWIO_MSS_CC_COXM_BUS_CBCR_FORCE_MEM_PERIPH_ON_SHFT                                                  13
#define HWIO_MSS_CC_COXM_BUS_CBCR_FORCE_MEM_PERIPH_OFF_BMSK                                             0x1000
#define HWIO_MSS_CC_COXM_BUS_CBCR_FORCE_MEM_PERIPH_OFF_SHFT                                                 12
#define HWIO_MSS_CC_COXM_BUS_CBCR_WAKEUP_BMSK                                                            0xf00
#define HWIO_MSS_CC_COXM_BUS_CBCR_WAKEUP_SHFT                                                                8
#define HWIO_MSS_CC_COXM_BUS_CBCR_SLEEP_BMSK                                                              0xf0
#define HWIO_MSS_CC_COXM_BUS_CBCR_SLEEP_SHFT                                                                 4
#define HWIO_MSS_CC_COXM_BUS_CBCR_CLK_ARES_BMSK                                                            0x4
#define HWIO_MSS_CC_COXM_BUS_CBCR_CLK_ARES_SHFT                                                              2
#define HWIO_MSS_CC_COXM_BUS_CBCR_CLK_ENABLE_BMSK                                                          0x1
#define HWIO_MSS_CC_COXM_BUS_CBCR_CLK_ENABLE_SHFT                                                            0

#define HWIO_MSS_CC_COXM_BUS_SREGR_ADDR                                                             (MSS_CC_MSS_CC_REG_REG_BASE            + 0x310)
#define HWIO_MSS_CC_COXM_BUS_SREGR_RMSK                                                             0xfffffffe
#define HWIO_MSS_CC_COXM_BUS_SREGR_IN                    \
                in_dword(HWIO_MSS_CC_COXM_BUS_SREGR_ADDR)
#define HWIO_MSS_CC_COXM_BUS_SREGR_INM(m)            \
                in_dword_masked(HWIO_MSS_CC_COXM_BUS_SREGR_ADDR, m)
#define HWIO_MSS_CC_COXM_BUS_SREGR_OUT(v)            \
                out_dword(HWIO_MSS_CC_COXM_BUS_SREGR_ADDR,v)
#define HWIO_MSS_CC_COXM_BUS_SREGR_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_CC_COXM_BUS_SREGR_ADDR,m,v,HWIO_MSS_CC_COXM_BUS_SREGR_IN)
#define HWIO_MSS_CC_COXM_BUS_SREGR_SREG_PSCBC_SPARE_CTRL_OUT_BMSK                                   0xff000000
#define HWIO_MSS_CC_COXM_BUS_SREGR_SREG_PSCBC_SPARE_CTRL_OUT_SHFT                                           24
#define HWIO_MSS_CC_COXM_BUS_SREGR_SREG_PSCBC_SPARE_CTRL_IN_BMSK                                      0xff0000
#define HWIO_MSS_CC_COXM_BUS_SREGR_SREG_PSCBC_SPARE_CTRL_IN_SHFT                                            16
#define HWIO_MSS_CC_COXM_BUS_SREGR_IGNORE_GDSC_PWR_DWN_CSR_BMSK                                         0x8000
#define HWIO_MSS_CC_COXM_BUS_SREGR_IGNORE_GDSC_PWR_DWN_CSR_SHFT                                             15
#define HWIO_MSS_CC_COXM_BUS_SREGR_PSCBC_SLP_STG_MODE_CSR_BMSK                                          0x4000
#define HWIO_MSS_CC_COXM_BUS_SREGR_PSCBC_SLP_STG_MODE_CSR_SHFT                                              14
#define HWIO_MSS_CC_COXM_BUS_SREGR_MEM_CPH_RST_SW_OVERRIDE_BMSK                                         0x2000
#define HWIO_MSS_CC_COXM_BUS_SREGR_MEM_CPH_RST_SW_OVERRIDE_SHFT                                             13
#define HWIO_MSS_CC_COXM_BUS_SREGR_SW_SM_PSCBC_SEQ_IN_OVERRIDE_BMSK                                     0x1000
#define HWIO_MSS_CC_COXM_BUS_SREGR_SW_SM_PSCBC_SEQ_IN_OVERRIDE_SHFT                                         12
#define HWIO_MSS_CC_COXM_BUS_SREGR_MEM_CORE_ON_ACK_BMSK                                                  0x800
#define HWIO_MSS_CC_COXM_BUS_SREGR_MEM_CORE_ON_ACK_SHFT                                                     11
#define HWIO_MSS_CC_COXM_BUS_SREGR_MEM_PERIPH_ON_ACK_BMSK                                                0x400
#define HWIO_MSS_CC_COXM_BUS_SREGR_MEM_PERIPH_ON_ACK_SHFT                                                   10
#define HWIO_MSS_CC_COXM_BUS_SREGR_SW_DIV_RATIO_SLP_STG_CLK_BMSK                                         0x300
#define HWIO_MSS_CC_COXM_BUS_SREGR_SW_DIV_RATIO_SLP_STG_CLK_SHFT                                             8
#define HWIO_MSS_CC_COXM_BUS_SREGR_MEM_CPH_ENABLE_BMSK                                                    0x80
#define HWIO_MSS_CC_COXM_BUS_SREGR_MEM_CPH_ENABLE_SHFT                                                       7
#define HWIO_MSS_CC_COXM_BUS_SREGR_FORCE_CLK_ON_BMSK                                                      0x40
#define HWIO_MSS_CC_COXM_BUS_SREGR_FORCE_CLK_ON_SHFT                                                         6
#define HWIO_MSS_CC_COXM_BUS_SREGR_SW_RST_SEL_SLP_STG_BMSK                                                0x20
#define HWIO_MSS_CC_COXM_BUS_SREGR_SW_RST_SEL_SLP_STG_SHFT                                                   5
#define HWIO_MSS_CC_COXM_BUS_SREGR_SW_RST_SLP_STG_BMSK                                                    0x10
#define HWIO_MSS_CC_COXM_BUS_SREGR_SW_RST_SLP_STG_SHFT                                                       4
#define HWIO_MSS_CC_COXM_BUS_SREGR_SW_CTRL_PWR_DOWN_BMSK                                                   0x8
#define HWIO_MSS_CC_COXM_BUS_SREGR_SW_CTRL_PWR_DOWN_SHFT                                                     3
#define HWIO_MSS_CC_COXM_BUS_SREGR_SW_CLK_EN_SEL_SLP_STG_BMSK                                              0x4
#define HWIO_MSS_CC_COXM_BUS_SREGR_SW_CLK_EN_SEL_SLP_STG_SHFT                                                2
#define HWIO_MSS_CC_COXM_BUS_SREGR_SW_CLK_EN_SLP_STG_BMSK                                                  0x2
#define HWIO_MSS_CC_COXM_BUS_SREGR_SW_CLK_EN_SLP_STG_SHFT                                                    1

#define HWIO_MSS_CC_CRYPTO_CORE_CBCR_ADDR                                                           (MSS_CC_MSS_CC_REG_REG_BASE            + 0x320)
#define HWIO_MSS_CC_CRYPTO_CORE_CBCR_RMSK                                                           0x81c07ff5
#define HWIO_MSS_CC_CRYPTO_CORE_CBCR_IN                    \
                in_dword(HWIO_MSS_CC_CRYPTO_CORE_CBCR_ADDR)
#define HWIO_MSS_CC_CRYPTO_CORE_CBCR_INM(m)            \
                in_dword_masked(HWIO_MSS_CC_CRYPTO_CORE_CBCR_ADDR, m)
#define HWIO_MSS_CC_CRYPTO_CORE_CBCR_OUT(v)            \
                out_dword(HWIO_MSS_CC_CRYPTO_CORE_CBCR_ADDR,v)
#define HWIO_MSS_CC_CRYPTO_CORE_CBCR_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_CC_CRYPTO_CORE_CBCR_ADDR,m,v,HWIO_MSS_CC_CRYPTO_CORE_CBCR_IN)
#define HWIO_MSS_CC_CRYPTO_CORE_CBCR_CLK_OFF_BMSK                                                   0x80000000
#define HWIO_MSS_CC_CRYPTO_CORE_CBCR_CLK_OFF_SHFT                                                           31
#define HWIO_MSS_CC_CRYPTO_CORE_CBCR_IGNORE_ALL_ARES_BMSK                                            0x1000000
#define HWIO_MSS_CC_CRYPTO_CORE_CBCR_IGNORE_ALL_ARES_SHFT                                                   24
#define HWIO_MSS_CC_CRYPTO_CORE_CBCR_IGNORE_ALL_CLK_DIS_BMSK                                          0x800000
#define HWIO_MSS_CC_CRYPTO_CORE_CBCR_IGNORE_ALL_CLK_DIS_SHFT                                                23
#define HWIO_MSS_CC_CRYPTO_CORE_CBCR_CLK_DIS_BMSK                                                     0x400000
#define HWIO_MSS_CC_CRYPTO_CORE_CBCR_CLK_DIS_SHFT                                                           22
#define HWIO_MSS_CC_CRYPTO_CORE_CBCR_FORCE_MEM_CORE_ON_BMSK                                             0x4000
#define HWIO_MSS_CC_CRYPTO_CORE_CBCR_FORCE_MEM_CORE_ON_SHFT                                                 14
#define HWIO_MSS_CC_CRYPTO_CORE_CBCR_FORCE_MEM_PERIPH_ON_BMSK                                           0x2000
#define HWIO_MSS_CC_CRYPTO_CORE_CBCR_FORCE_MEM_PERIPH_ON_SHFT                                               13
#define HWIO_MSS_CC_CRYPTO_CORE_CBCR_FORCE_MEM_PERIPH_OFF_BMSK                                          0x1000
#define HWIO_MSS_CC_CRYPTO_CORE_CBCR_FORCE_MEM_PERIPH_OFF_SHFT                                              12
#define HWIO_MSS_CC_CRYPTO_CORE_CBCR_WAKEUP_BMSK                                                         0xf00
#define HWIO_MSS_CC_CRYPTO_CORE_CBCR_WAKEUP_SHFT                                                             8
#define HWIO_MSS_CC_CRYPTO_CORE_CBCR_SLEEP_BMSK                                                           0xf0
#define HWIO_MSS_CC_CRYPTO_CORE_CBCR_SLEEP_SHFT                                                              4
#define HWIO_MSS_CC_CRYPTO_CORE_CBCR_CLK_ARES_BMSK                                                         0x4
#define HWIO_MSS_CC_CRYPTO_CORE_CBCR_CLK_ARES_SHFT                                                           2
#define HWIO_MSS_CC_CRYPTO_CORE_CBCR_CLK_ENABLE_BMSK                                                       0x1
#define HWIO_MSS_CC_CRYPTO_CORE_CBCR_CLK_ENABLE_SHFT                                                         0

#define HWIO_MSS_CC_CRYPTO_CORE_SREGR_ADDR                                                          (MSS_CC_MSS_CC_REG_REG_BASE            + 0x324)
#define HWIO_MSS_CC_CRYPTO_CORE_SREGR_RMSK                                                          0xfffffffe
#define HWIO_MSS_CC_CRYPTO_CORE_SREGR_IN                    \
                in_dword(HWIO_MSS_CC_CRYPTO_CORE_SREGR_ADDR)
#define HWIO_MSS_CC_CRYPTO_CORE_SREGR_INM(m)            \
                in_dword_masked(HWIO_MSS_CC_CRYPTO_CORE_SREGR_ADDR, m)
#define HWIO_MSS_CC_CRYPTO_CORE_SREGR_OUT(v)            \
                out_dword(HWIO_MSS_CC_CRYPTO_CORE_SREGR_ADDR,v)
#define HWIO_MSS_CC_CRYPTO_CORE_SREGR_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_CC_CRYPTO_CORE_SREGR_ADDR,m,v,HWIO_MSS_CC_CRYPTO_CORE_SREGR_IN)
#define HWIO_MSS_CC_CRYPTO_CORE_SREGR_SREG_PSCBC_SPARE_CTRL_OUT_BMSK                                0xff000000
#define HWIO_MSS_CC_CRYPTO_CORE_SREGR_SREG_PSCBC_SPARE_CTRL_OUT_SHFT                                        24
#define HWIO_MSS_CC_CRYPTO_CORE_SREGR_SREG_PSCBC_SPARE_CTRL_IN_BMSK                                   0xff0000
#define HWIO_MSS_CC_CRYPTO_CORE_SREGR_SREG_PSCBC_SPARE_CTRL_IN_SHFT                                         16
#define HWIO_MSS_CC_CRYPTO_CORE_SREGR_IGNORE_GDSC_PWR_DWN_CSR_BMSK                                      0x8000
#define HWIO_MSS_CC_CRYPTO_CORE_SREGR_IGNORE_GDSC_PWR_DWN_CSR_SHFT                                          15
#define HWIO_MSS_CC_CRYPTO_CORE_SREGR_PSCBC_SLP_STG_MODE_CSR_BMSK                                       0x4000
#define HWIO_MSS_CC_CRYPTO_CORE_SREGR_PSCBC_SLP_STG_MODE_CSR_SHFT                                           14
#define HWIO_MSS_CC_CRYPTO_CORE_SREGR_MEM_CPH_RST_SW_OVERRIDE_BMSK                                      0x2000
#define HWIO_MSS_CC_CRYPTO_CORE_SREGR_MEM_CPH_RST_SW_OVERRIDE_SHFT                                          13
#define HWIO_MSS_CC_CRYPTO_CORE_SREGR_SW_SM_PSCBC_SEQ_IN_OVERRIDE_BMSK                                  0x1000
#define HWIO_MSS_CC_CRYPTO_CORE_SREGR_SW_SM_PSCBC_SEQ_IN_OVERRIDE_SHFT                                      12
#define HWIO_MSS_CC_CRYPTO_CORE_SREGR_MEM_CORE_ON_ACK_BMSK                                               0x800
#define HWIO_MSS_CC_CRYPTO_CORE_SREGR_MEM_CORE_ON_ACK_SHFT                                                  11
#define HWIO_MSS_CC_CRYPTO_CORE_SREGR_MEM_PERIPH_ON_ACK_BMSK                                             0x400
#define HWIO_MSS_CC_CRYPTO_CORE_SREGR_MEM_PERIPH_ON_ACK_SHFT                                                10
#define HWIO_MSS_CC_CRYPTO_CORE_SREGR_SW_DIV_RATIO_SLP_STG_CLK_BMSK                                      0x300
#define HWIO_MSS_CC_CRYPTO_CORE_SREGR_SW_DIV_RATIO_SLP_STG_CLK_SHFT                                          8
#define HWIO_MSS_CC_CRYPTO_CORE_SREGR_MEM_CPH_ENABLE_BMSK                                                 0x80
#define HWIO_MSS_CC_CRYPTO_CORE_SREGR_MEM_CPH_ENABLE_SHFT                                                    7
#define HWIO_MSS_CC_CRYPTO_CORE_SREGR_FORCE_CLK_ON_BMSK                                                   0x40
#define HWIO_MSS_CC_CRYPTO_CORE_SREGR_FORCE_CLK_ON_SHFT                                                      6
#define HWIO_MSS_CC_CRYPTO_CORE_SREGR_SW_RST_SEL_SLP_STG_BMSK                                             0x20
#define HWIO_MSS_CC_CRYPTO_CORE_SREGR_SW_RST_SEL_SLP_STG_SHFT                                                5
#define HWIO_MSS_CC_CRYPTO_CORE_SREGR_SW_RST_SLP_STG_BMSK                                                 0x10
#define HWIO_MSS_CC_CRYPTO_CORE_SREGR_SW_RST_SLP_STG_SHFT                                                    4
#define HWIO_MSS_CC_CRYPTO_CORE_SREGR_SW_CTRL_PWR_DOWN_BMSK                                                0x8
#define HWIO_MSS_CC_CRYPTO_CORE_SREGR_SW_CTRL_PWR_DOWN_SHFT                                                  3
#define HWIO_MSS_CC_CRYPTO_CORE_SREGR_SW_CLK_EN_SEL_SLP_STG_BMSK                                           0x4
#define HWIO_MSS_CC_CRYPTO_CORE_SREGR_SW_CLK_EN_SEL_SLP_STG_SHFT                                             2
#define HWIO_MSS_CC_CRYPTO_CORE_SREGR_SW_CLK_EN_SLP_STG_BMSK                                               0x2
#define HWIO_MSS_CC_CRYPTO_CORE_SREGR_SW_CLK_EN_SLP_STG_SHFT                                                 1

#define HWIO_MSS_CC_CRYPTO_BUS_CBCR_ADDR                                                            (MSS_CC_MSS_CC_REG_REG_BASE            + 0x334)
#define HWIO_MSS_CC_CRYPTO_BUS_CBCR_RMSK                                                            0x81c00005
#define HWIO_MSS_CC_CRYPTO_BUS_CBCR_IN                    \
                in_dword(HWIO_MSS_CC_CRYPTO_BUS_CBCR_ADDR)
#define HWIO_MSS_CC_CRYPTO_BUS_CBCR_INM(m)            \
                in_dword_masked(HWIO_MSS_CC_CRYPTO_BUS_CBCR_ADDR, m)
#define HWIO_MSS_CC_CRYPTO_BUS_CBCR_OUT(v)            \
                out_dword(HWIO_MSS_CC_CRYPTO_BUS_CBCR_ADDR,v)
#define HWIO_MSS_CC_CRYPTO_BUS_CBCR_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_CC_CRYPTO_BUS_CBCR_ADDR,m,v,HWIO_MSS_CC_CRYPTO_BUS_CBCR_IN)
#define HWIO_MSS_CC_CRYPTO_BUS_CBCR_CLK_OFF_BMSK                                                    0x80000000
#define HWIO_MSS_CC_CRYPTO_BUS_CBCR_CLK_OFF_SHFT                                                            31
#define HWIO_MSS_CC_CRYPTO_BUS_CBCR_IGNORE_ALL_ARES_BMSK                                             0x1000000
#define HWIO_MSS_CC_CRYPTO_BUS_CBCR_IGNORE_ALL_ARES_SHFT                                                    24
#define HWIO_MSS_CC_CRYPTO_BUS_CBCR_IGNORE_ALL_CLK_DIS_BMSK                                           0x800000
#define HWIO_MSS_CC_CRYPTO_BUS_CBCR_IGNORE_ALL_CLK_DIS_SHFT                                                 23
#define HWIO_MSS_CC_CRYPTO_BUS_CBCR_CLK_DIS_BMSK                                                      0x400000
#define HWIO_MSS_CC_CRYPTO_BUS_CBCR_CLK_DIS_SHFT                                                            22
#define HWIO_MSS_CC_CRYPTO_BUS_CBCR_CLK_ARES_BMSK                                                          0x4
#define HWIO_MSS_CC_CRYPTO_BUS_CBCR_CLK_ARES_SHFT                                                            2
#define HWIO_MSS_CC_CRYPTO_BUS_CBCR_CLK_ENABLE_BMSK                                                        0x1
#define HWIO_MSS_CC_CRYPTO_BUS_CBCR_CLK_ENABLE_SHFT                                                          0

#define HWIO_MSS_CC_UIM0_BUS_CBCR_ADDR                                                              (MSS_CC_MSS_CC_REG_REG_BASE            + 0x348)
#define HWIO_MSS_CC_UIM0_BUS_CBCR_RMSK                                                              0x81c07ff5
#define HWIO_MSS_CC_UIM0_BUS_CBCR_IN                    \
                in_dword(HWIO_MSS_CC_UIM0_BUS_CBCR_ADDR)
#define HWIO_MSS_CC_UIM0_BUS_CBCR_INM(m)            \
                in_dword_masked(HWIO_MSS_CC_UIM0_BUS_CBCR_ADDR, m)
#define HWIO_MSS_CC_UIM0_BUS_CBCR_OUT(v)            \
                out_dword(HWIO_MSS_CC_UIM0_BUS_CBCR_ADDR,v)
#define HWIO_MSS_CC_UIM0_BUS_CBCR_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_CC_UIM0_BUS_CBCR_ADDR,m,v,HWIO_MSS_CC_UIM0_BUS_CBCR_IN)
#define HWIO_MSS_CC_UIM0_BUS_CBCR_CLK_OFF_BMSK                                                      0x80000000
#define HWIO_MSS_CC_UIM0_BUS_CBCR_CLK_OFF_SHFT                                                              31
#define HWIO_MSS_CC_UIM0_BUS_CBCR_IGNORE_ALL_ARES_BMSK                                               0x1000000
#define HWIO_MSS_CC_UIM0_BUS_CBCR_IGNORE_ALL_ARES_SHFT                                                      24
#define HWIO_MSS_CC_UIM0_BUS_CBCR_IGNORE_ALL_CLK_DIS_BMSK                                             0x800000
#define HWIO_MSS_CC_UIM0_BUS_CBCR_IGNORE_ALL_CLK_DIS_SHFT                                                   23
#define HWIO_MSS_CC_UIM0_BUS_CBCR_CLK_DIS_BMSK                                                        0x400000
#define HWIO_MSS_CC_UIM0_BUS_CBCR_CLK_DIS_SHFT                                                              22
#define HWIO_MSS_CC_UIM0_BUS_CBCR_FORCE_MEM_CORE_ON_BMSK                                                0x4000
#define HWIO_MSS_CC_UIM0_BUS_CBCR_FORCE_MEM_CORE_ON_SHFT                                                    14
#define HWIO_MSS_CC_UIM0_BUS_CBCR_FORCE_MEM_PERIPH_ON_BMSK                                              0x2000
#define HWIO_MSS_CC_UIM0_BUS_CBCR_FORCE_MEM_PERIPH_ON_SHFT                                                  13
#define HWIO_MSS_CC_UIM0_BUS_CBCR_FORCE_MEM_PERIPH_OFF_BMSK                                             0x1000
#define HWIO_MSS_CC_UIM0_BUS_CBCR_FORCE_MEM_PERIPH_OFF_SHFT                                                 12
#define HWIO_MSS_CC_UIM0_BUS_CBCR_WAKEUP_BMSK                                                            0xf00
#define HWIO_MSS_CC_UIM0_BUS_CBCR_WAKEUP_SHFT                                                                8
#define HWIO_MSS_CC_UIM0_BUS_CBCR_SLEEP_BMSK                                                              0xf0
#define HWIO_MSS_CC_UIM0_BUS_CBCR_SLEEP_SHFT                                                                 4
#define HWIO_MSS_CC_UIM0_BUS_CBCR_CLK_ARES_BMSK                                                            0x4
#define HWIO_MSS_CC_UIM0_BUS_CBCR_CLK_ARES_SHFT                                                              2
#define HWIO_MSS_CC_UIM0_BUS_CBCR_CLK_ENABLE_BMSK                                                          0x1
#define HWIO_MSS_CC_UIM0_BUS_CBCR_CLK_ENABLE_SHFT                                                            0

#define HWIO_MSS_CC_UIM0_BUS_SREGR_ADDR                                                             (MSS_CC_MSS_CC_REG_REG_BASE            + 0x34c)
#define HWIO_MSS_CC_UIM0_BUS_SREGR_RMSK                                                             0xfffffffe
#define HWIO_MSS_CC_UIM0_BUS_SREGR_IN                    \
                in_dword(HWIO_MSS_CC_UIM0_BUS_SREGR_ADDR)
#define HWIO_MSS_CC_UIM0_BUS_SREGR_INM(m)            \
                in_dword_masked(HWIO_MSS_CC_UIM0_BUS_SREGR_ADDR, m)
#define HWIO_MSS_CC_UIM0_BUS_SREGR_OUT(v)            \
                out_dword(HWIO_MSS_CC_UIM0_BUS_SREGR_ADDR,v)
#define HWIO_MSS_CC_UIM0_BUS_SREGR_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_CC_UIM0_BUS_SREGR_ADDR,m,v,HWIO_MSS_CC_UIM0_BUS_SREGR_IN)
#define HWIO_MSS_CC_UIM0_BUS_SREGR_SREG_PSCBC_SPARE_CTRL_OUT_BMSK                                   0xff000000
#define HWIO_MSS_CC_UIM0_BUS_SREGR_SREG_PSCBC_SPARE_CTRL_OUT_SHFT                                           24
#define HWIO_MSS_CC_UIM0_BUS_SREGR_SREG_PSCBC_SPARE_CTRL_IN_BMSK                                      0xff0000
#define HWIO_MSS_CC_UIM0_BUS_SREGR_SREG_PSCBC_SPARE_CTRL_IN_SHFT                                            16
#define HWIO_MSS_CC_UIM0_BUS_SREGR_IGNORE_GDSC_PWR_DWN_CSR_BMSK                                         0x8000
#define HWIO_MSS_CC_UIM0_BUS_SREGR_IGNORE_GDSC_PWR_DWN_CSR_SHFT                                             15
#define HWIO_MSS_CC_UIM0_BUS_SREGR_PSCBC_SLP_STG_MODE_CSR_BMSK                                          0x4000
#define HWIO_MSS_CC_UIM0_BUS_SREGR_PSCBC_SLP_STG_MODE_CSR_SHFT                                              14
#define HWIO_MSS_CC_UIM0_BUS_SREGR_MEM_CPH_RST_SW_OVERRIDE_BMSK                                         0x2000
#define HWIO_MSS_CC_UIM0_BUS_SREGR_MEM_CPH_RST_SW_OVERRIDE_SHFT                                             13
#define HWIO_MSS_CC_UIM0_BUS_SREGR_SW_SM_PSCBC_SEQ_IN_OVERRIDE_BMSK                                     0x1000
#define HWIO_MSS_CC_UIM0_BUS_SREGR_SW_SM_PSCBC_SEQ_IN_OVERRIDE_SHFT                                         12
#define HWIO_MSS_CC_UIM0_BUS_SREGR_MEM_CORE_ON_ACK_BMSK                                                  0x800
#define HWIO_MSS_CC_UIM0_BUS_SREGR_MEM_CORE_ON_ACK_SHFT                                                     11
#define HWIO_MSS_CC_UIM0_BUS_SREGR_MEM_PERIPH_ON_ACK_BMSK                                                0x400
#define HWIO_MSS_CC_UIM0_BUS_SREGR_MEM_PERIPH_ON_ACK_SHFT                                                   10
#define HWIO_MSS_CC_UIM0_BUS_SREGR_SW_DIV_RATIO_SLP_STG_CLK_BMSK                                         0x300
#define HWIO_MSS_CC_UIM0_BUS_SREGR_SW_DIV_RATIO_SLP_STG_CLK_SHFT                                             8
#define HWIO_MSS_CC_UIM0_BUS_SREGR_MEM_CPH_ENABLE_BMSK                                                    0x80
#define HWIO_MSS_CC_UIM0_BUS_SREGR_MEM_CPH_ENABLE_SHFT                                                       7
#define HWIO_MSS_CC_UIM0_BUS_SREGR_FORCE_CLK_ON_BMSK                                                      0x40
#define HWIO_MSS_CC_UIM0_BUS_SREGR_FORCE_CLK_ON_SHFT                                                         6
#define HWIO_MSS_CC_UIM0_BUS_SREGR_SW_RST_SEL_SLP_STG_BMSK                                                0x20
#define HWIO_MSS_CC_UIM0_BUS_SREGR_SW_RST_SEL_SLP_STG_SHFT                                                   5
#define HWIO_MSS_CC_UIM0_BUS_SREGR_SW_RST_SLP_STG_BMSK                                                    0x10
#define HWIO_MSS_CC_UIM0_BUS_SREGR_SW_RST_SLP_STG_SHFT                                                       4
#define HWIO_MSS_CC_UIM0_BUS_SREGR_SW_CTRL_PWR_DOWN_BMSK                                                   0x8
#define HWIO_MSS_CC_UIM0_BUS_SREGR_SW_CTRL_PWR_DOWN_SHFT                                                     3
#define HWIO_MSS_CC_UIM0_BUS_SREGR_SW_CLK_EN_SEL_SLP_STG_BMSK                                              0x4
#define HWIO_MSS_CC_UIM0_BUS_SREGR_SW_CLK_EN_SEL_SLP_STG_SHFT                                                2
#define HWIO_MSS_CC_UIM0_BUS_SREGR_SW_CLK_EN_SLP_STG_BMSK                                                  0x2
#define HWIO_MSS_CC_UIM0_BUS_SREGR_SW_CLK_EN_SLP_STG_SHFT                                                    1

#define HWIO_MSS_CC_UIM1_BUS_CBCR_ADDR                                                              (MSS_CC_MSS_CC_REG_REG_BASE            + 0x35c)
#define HWIO_MSS_CC_UIM1_BUS_CBCR_RMSK                                                              0x81c07ff5
#define HWIO_MSS_CC_UIM1_BUS_CBCR_IN                    \
                in_dword(HWIO_MSS_CC_UIM1_BUS_CBCR_ADDR)
#define HWIO_MSS_CC_UIM1_BUS_CBCR_INM(m)            \
                in_dword_masked(HWIO_MSS_CC_UIM1_BUS_CBCR_ADDR, m)
#define HWIO_MSS_CC_UIM1_BUS_CBCR_OUT(v)            \
                out_dword(HWIO_MSS_CC_UIM1_BUS_CBCR_ADDR,v)
#define HWIO_MSS_CC_UIM1_BUS_CBCR_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_CC_UIM1_BUS_CBCR_ADDR,m,v,HWIO_MSS_CC_UIM1_BUS_CBCR_IN)
#define HWIO_MSS_CC_UIM1_BUS_CBCR_CLK_OFF_BMSK                                                      0x80000000
#define HWIO_MSS_CC_UIM1_BUS_CBCR_CLK_OFF_SHFT                                                              31
#define HWIO_MSS_CC_UIM1_BUS_CBCR_IGNORE_ALL_ARES_BMSK                                               0x1000000
#define HWIO_MSS_CC_UIM1_BUS_CBCR_IGNORE_ALL_ARES_SHFT                                                      24
#define HWIO_MSS_CC_UIM1_BUS_CBCR_IGNORE_ALL_CLK_DIS_BMSK                                             0x800000
#define HWIO_MSS_CC_UIM1_BUS_CBCR_IGNORE_ALL_CLK_DIS_SHFT                                                   23
#define HWIO_MSS_CC_UIM1_BUS_CBCR_CLK_DIS_BMSK                                                        0x400000
#define HWIO_MSS_CC_UIM1_BUS_CBCR_CLK_DIS_SHFT                                                              22
#define HWIO_MSS_CC_UIM1_BUS_CBCR_FORCE_MEM_CORE_ON_BMSK                                                0x4000
#define HWIO_MSS_CC_UIM1_BUS_CBCR_FORCE_MEM_CORE_ON_SHFT                                                    14
#define HWIO_MSS_CC_UIM1_BUS_CBCR_FORCE_MEM_PERIPH_ON_BMSK                                              0x2000
#define HWIO_MSS_CC_UIM1_BUS_CBCR_FORCE_MEM_PERIPH_ON_SHFT                                                  13
#define HWIO_MSS_CC_UIM1_BUS_CBCR_FORCE_MEM_PERIPH_OFF_BMSK                                             0x1000
#define HWIO_MSS_CC_UIM1_BUS_CBCR_FORCE_MEM_PERIPH_OFF_SHFT                                                 12
#define HWIO_MSS_CC_UIM1_BUS_CBCR_WAKEUP_BMSK                                                            0xf00
#define HWIO_MSS_CC_UIM1_BUS_CBCR_WAKEUP_SHFT                                                                8
#define HWIO_MSS_CC_UIM1_BUS_CBCR_SLEEP_BMSK                                                              0xf0
#define HWIO_MSS_CC_UIM1_BUS_CBCR_SLEEP_SHFT                                                                 4
#define HWIO_MSS_CC_UIM1_BUS_CBCR_CLK_ARES_BMSK                                                            0x4
#define HWIO_MSS_CC_UIM1_BUS_CBCR_CLK_ARES_SHFT                                                              2
#define HWIO_MSS_CC_UIM1_BUS_CBCR_CLK_ENABLE_BMSK                                                          0x1
#define HWIO_MSS_CC_UIM1_BUS_CBCR_CLK_ENABLE_SHFT                                                            0

#define HWIO_MSS_CC_UIM1_BUS_SREGR_ADDR                                                             (MSS_CC_MSS_CC_REG_REG_BASE            + 0x360)
#define HWIO_MSS_CC_UIM1_BUS_SREGR_RMSK                                                             0xfffffffe
#define HWIO_MSS_CC_UIM1_BUS_SREGR_IN                    \
                in_dword(HWIO_MSS_CC_UIM1_BUS_SREGR_ADDR)
#define HWIO_MSS_CC_UIM1_BUS_SREGR_INM(m)            \
                in_dword_masked(HWIO_MSS_CC_UIM1_BUS_SREGR_ADDR, m)
#define HWIO_MSS_CC_UIM1_BUS_SREGR_OUT(v)            \
                out_dword(HWIO_MSS_CC_UIM1_BUS_SREGR_ADDR,v)
#define HWIO_MSS_CC_UIM1_BUS_SREGR_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_CC_UIM1_BUS_SREGR_ADDR,m,v,HWIO_MSS_CC_UIM1_BUS_SREGR_IN)
#define HWIO_MSS_CC_UIM1_BUS_SREGR_SREG_PSCBC_SPARE_CTRL_OUT_BMSK                                   0xff000000
#define HWIO_MSS_CC_UIM1_BUS_SREGR_SREG_PSCBC_SPARE_CTRL_OUT_SHFT                                           24
#define HWIO_MSS_CC_UIM1_BUS_SREGR_SREG_PSCBC_SPARE_CTRL_IN_BMSK                                      0xff0000
#define HWIO_MSS_CC_UIM1_BUS_SREGR_SREG_PSCBC_SPARE_CTRL_IN_SHFT                                            16
#define HWIO_MSS_CC_UIM1_BUS_SREGR_IGNORE_GDSC_PWR_DWN_CSR_BMSK                                         0x8000
#define HWIO_MSS_CC_UIM1_BUS_SREGR_IGNORE_GDSC_PWR_DWN_CSR_SHFT                                             15
#define HWIO_MSS_CC_UIM1_BUS_SREGR_PSCBC_SLP_STG_MODE_CSR_BMSK                                          0x4000
#define HWIO_MSS_CC_UIM1_BUS_SREGR_PSCBC_SLP_STG_MODE_CSR_SHFT                                              14
#define HWIO_MSS_CC_UIM1_BUS_SREGR_MEM_CPH_RST_SW_OVERRIDE_BMSK                                         0x2000
#define HWIO_MSS_CC_UIM1_BUS_SREGR_MEM_CPH_RST_SW_OVERRIDE_SHFT                                             13
#define HWIO_MSS_CC_UIM1_BUS_SREGR_SW_SM_PSCBC_SEQ_IN_OVERRIDE_BMSK                                     0x1000
#define HWIO_MSS_CC_UIM1_BUS_SREGR_SW_SM_PSCBC_SEQ_IN_OVERRIDE_SHFT                                         12
#define HWIO_MSS_CC_UIM1_BUS_SREGR_MEM_CORE_ON_ACK_BMSK                                                  0x800
#define HWIO_MSS_CC_UIM1_BUS_SREGR_MEM_CORE_ON_ACK_SHFT                                                     11
#define HWIO_MSS_CC_UIM1_BUS_SREGR_MEM_PERIPH_ON_ACK_BMSK                                                0x400
#define HWIO_MSS_CC_UIM1_BUS_SREGR_MEM_PERIPH_ON_ACK_SHFT                                                   10
#define HWIO_MSS_CC_UIM1_BUS_SREGR_SW_DIV_RATIO_SLP_STG_CLK_BMSK                                         0x300
#define HWIO_MSS_CC_UIM1_BUS_SREGR_SW_DIV_RATIO_SLP_STG_CLK_SHFT                                             8
#define HWIO_MSS_CC_UIM1_BUS_SREGR_MEM_CPH_ENABLE_BMSK                                                    0x80
#define HWIO_MSS_CC_UIM1_BUS_SREGR_MEM_CPH_ENABLE_SHFT                                                       7
#define HWIO_MSS_CC_UIM1_BUS_SREGR_FORCE_CLK_ON_BMSK                                                      0x40
#define HWIO_MSS_CC_UIM1_BUS_SREGR_FORCE_CLK_ON_SHFT                                                         6
#define HWIO_MSS_CC_UIM1_BUS_SREGR_SW_RST_SEL_SLP_STG_BMSK                                                0x20
#define HWIO_MSS_CC_UIM1_BUS_SREGR_SW_RST_SEL_SLP_STG_SHFT                                                   5
#define HWIO_MSS_CC_UIM1_BUS_SREGR_SW_RST_SLP_STG_BMSK                                                    0x10
#define HWIO_MSS_CC_UIM1_BUS_SREGR_SW_RST_SLP_STG_SHFT                                                       4
#define HWIO_MSS_CC_UIM1_BUS_SREGR_SW_CTRL_PWR_DOWN_BMSK                                                   0x8
#define HWIO_MSS_CC_UIM1_BUS_SREGR_SW_CTRL_PWR_DOWN_SHFT                                                     3
#define HWIO_MSS_CC_UIM1_BUS_SREGR_SW_CLK_EN_SEL_SLP_STG_BMSK                                              0x4
#define HWIO_MSS_CC_UIM1_BUS_SREGR_SW_CLK_EN_SEL_SLP_STG_SHFT                                                2
#define HWIO_MSS_CC_UIM1_BUS_SREGR_SW_CLK_EN_SLP_STG_BMSK                                                  0x2
#define HWIO_MSS_CC_UIM1_BUS_SREGR_SW_CLK_EN_SLP_STG_SHFT                                                    1

#define HWIO_MSS_CC_CSR_BUS_CBCR_ADDR                                                               (MSS_CC_MSS_CC_REG_REG_BASE            + 0x384)
#define HWIO_MSS_CC_CSR_BUS_CBCR_RMSK                                                               0x81c00005
#define HWIO_MSS_CC_CSR_BUS_CBCR_IN                    \
                in_dword(HWIO_MSS_CC_CSR_BUS_CBCR_ADDR)
#define HWIO_MSS_CC_CSR_BUS_CBCR_INM(m)            \
                in_dword_masked(HWIO_MSS_CC_CSR_BUS_CBCR_ADDR, m)
#define HWIO_MSS_CC_CSR_BUS_CBCR_OUT(v)            \
                out_dword(HWIO_MSS_CC_CSR_BUS_CBCR_ADDR,v)
#define HWIO_MSS_CC_CSR_BUS_CBCR_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_CC_CSR_BUS_CBCR_ADDR,m,v,HWIO_MSS_CC_CSR_BUS_CBCR_IN)
#define HWIO_MSS_CC_CSR_BUS_CBCR_CLK_OFF_BMSK                                                       0x80000000
#define HWIO_MSS_CC_CSR_BUS_CBCR_CLK_OFF_SHFT                                                               31
#define HWIO_MSS_CC_CSR_BUS_CBCR_IGNORE_ALL_ARES_BMSK                                                0x1000000
#define HWIO_MSS_CC_CSR_BUS_CBCR_IGNORE_ALL_ARES_SHFT                                                       24
#define HWIO_MSS_CC_CSR_BUS_CBCR_IGNORE_ALL_CLK_DIS_BMSK                                              0x800000
#define HWIO_MSS_CC_CSR_BUS_CBCR_IGNORE_ALL_CLK_DIS_SHFT                                                    23
#define HWIO_MSS_CC_CSR_BUS_CBCR_CLK_DIS_BMSK                                                         0x400000
#define HWIO_MSS_CC_CSR_BUS_CBCR_CLK_DIS_SHFT                                                               22
#define HWIO_MSS_CC_CSR_BUS_CBCR_CLK_ARES_BMSK                                                             0x4
#define HWIO_MSS_CC_CSR_BUS_CBCR_CLK_ARES_SHFT                                                               2
#define HWIO_MSS_CC_CSR_BUS_CBCR_CLK_ENABLE_BMSK                                                           0x1
#define HWIO_MSS_CC_CSR_BUS_CBCR_CLK_ENABLE_SHFT                                                             0

#define HWIO_MSS_CC_ATB_BUS_CBCR_ADDR                                                               (MSS_CC_MSS_CC_REG_REG_BASE            + 0x398)
#define HWIO_MSS_CC_ATB_BUS_CBCR_RMSK                                                               0x81c00005
#define HWIO_MSS_CC_ATB_BUS_CBCR_IN                    \
                in_dword(HWIO_MSS_CC_ATB_BUS_CBCR_ADDR)
#define HWIO_MSS_CC_ATB_BUS_CBCR_INM(m)            \
                in_dword_masked(HWIO_MSS_CC_ATB_BUS_CBCR_ADDR, m)
#define HWIO_MSS_CC_ATB_BUS_CBCR_OUT(v)            \
                out_dword(HWIO_MSS_CC_ATB_BUS_CBCR_ADDR,v)
#define HWIO_MSS_CC_ATB_BUS_CBCR_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_CC_ATB_BUS_CBCR_ADDR,m,v,HWIO_MSS_CC_ATB_BUS_CBCR_IN)
#define HWIO_MSS_CC_ATB_BUS_CBCR_CLK_OFF_BMSK                                                       0x80000000
#define HWIO_MSS_CC_ATB_BUS_CBCR_CLK_OFF_SHFT                                                               31
#define HWIO_MSS_CC_ATB_BUS_CBCR_IGNORE_ALL_ARES_BMSK                                                0x1000000
#define HWIO_MSS_CC_ATB_BUS_CBCR_IGNORE_ALL_ARES_SHFT                                                       24
#define HWIO_MSS_CC_ATB_BUS_CBCR_IGNORE_ALL_CLK_DIS_BMSK                                              0x800000
#define HWIO_MSS_CC_ATB_BUS_CBCR_IGNORE_ALL_CLK_DIS_SHFT                                                    23
#define HWIO_MSS_CC_ATB_BUS_CBCR_CLK_DIS_BMSK                                                         0x400000
#define HWIO_MSS_CC_ATB_BUS_CBCR_CLK_DIS_SHFT                                                               22
#define HWIO_MSS_CC_ATB_BUS_CBCR_CLK_ARES_BMSK                                                             0x4
#define HWIO_MSS_CC_ATB_BUS_CBCR_CLK_ARES_SHFT                                                               2
#define HWIO_MSS_CC_ATB_BUS_CBCR_CLK_ENABLE_BMSK                                                           0x1
#define HWIO_MSS_CC_ATB_BUS_CBCR_CLK_ENABLE_SHFT                                                             0

#define HWIO_MSS_CC_BRIDGE_BUS_CBCR_ADDR                                                            (MSS_CC_MSS_CC_REG_REG_BASE            + 0x3ac)
#define HWIO_MSS_CC_BRIDGE_BUS_CBCR_RMSK                                                            0x81c00005
#define HWIO_MSS_CC_BRIDGE_BUS_CBCR_IN                    \
                in_dword(HWIO_MSS_CC_BRIDGE_BUS_CBCR_ADDR)
#define HWIO_MSS_CC_BRIDGE_BUS_CBCR_INM(m)            \
                in_dword_masked(HWIO_MSS_CC_BRIDGE_BUS_CBCR_ADDR, m)
#define HWIO_MSS_CC_BRIDGE_BUS_CBCR_OUT(v)            \
                out_dword(HWIO_MSS_CC_BRIDGE_BUS_CBCR_ADDR,v)
#define HWIO_MSS_CC_BRIDGE_BUS_CBCR_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_CC_BRIDGE_BUS_CBCR_ADDR,m,v,HWIO_MSS_CC_BRIDGE_BUS_CBCR_IN)
#define HWIO_MSS_CC_BRIDGE_BUS_CBCR_CLK_OFF_BMSK                                                    0x80000000
#define HWIO_MSS_CC_BRIDGE_BUS_CBCR_CLK_OFF_SHFT                                                            31
#define HWIO_MSS_CC_BRIDGE_BUS_CBCR_IGNORE_ALL_ARES_BMSK                                             0x1000000
#define HWIO_MSS_CC_BRIDGE_BUS_CBCR_IGNORE_ALL_ARES_SHFT                                                    24
#define HWIO_MSS_CC_BRIDGE_BUS_CBCR_IGNORE_ALL_CLK_DIS_BMSK                                           0x800000
#define HWIO_MSS_CC_BRIDGE_BUS_CBCR_IGNORE_ALL_CLK_DIS_SHFT                                                 23
#define HWIO_MSS_CC_BRIDGE_BUS_CBCR_CLK_DIS_BMSK                                                      0x400000
#define HWIO_MSS_CC_BRIDGE_BUS_CBCR_CLK_DIS_SHFT                                                            22
#define HWIO_MSS_CC_BRIDGE_BUS_CBCR_CLK_ARES_BMSK                                                          0x4
#define HWIO_MSS_CC_BRIDGE_BUS_CBCR_CLK_ARES_SHFT                                                            2
#define HWIO_MSS_CC_BRIDGE_BUS_CBCR_CLK_ENABLE_BMSK                                                        0x1
#define HWIO_MSS_CC_BRIDGE_BUS_CBCR_CLK_ENABLE_SHFT                                                          0

#define HWIO_MSS_CC_Q6_ALT_CBCR_ADDR                                                                (MSS_CC_MSS_CC_REG_REG_BASE            + 0x3c0)
#define HWIO_MSS_CC_Q6_ALT_CBCR_RMSK                                                                0x81c00005
#define HWIO_MSS_CC_Q6_ALT_CBCR_IN                    \
                in_dword(HWIO_MSS_CC_Q6_ALT_CBCR_ADDR)
#define HWIO_MSS_CC_Q6_ALT_CBCR_INM(m)            \
                in_dword_masked(HWIO_MSS_CC_Q6_ALT_CBCR_ADDR, m)
#define HWIO_MSS_CC_Q6_ALT_CBCR_OUT(v)            \
                out_dword(HWIO_MSS_CC_Q6_ALT_CBCR_ADDR,v)
#define HWIO_MSS_CC_Q6_ALT_CBCR_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_CC_Q6_ALT_CBCR_ADDR,m,v,HWIO_MSS_CC_Q6_ALT_CBCR_IN)
#define HWIO_MSS_CC_Q6_ALT_CBCR_CLK_OFF_BMSK                                                        0x80000000
#define HWIO_MSS_CC_Q6_ALT_CBCR_CLK_OFF_SHFT                                                                31
#define HWIO_MSS_CC_Q6_ALT_CBCR_IGNORE_ALL_ARES_BMSK                                                 0x1000000
#define HWIO_MSS_CC_Q6_ALT_CBCR_IGNORE_ALL_ARES_SHFT                                                        24
#define HWIO_MSS_CC_Q6_ALT_CBCR_IGNORE_ALL_CLK_DIS_BMSK                                               0x800000
#define HWIO_MSS_CC_Q6_ALT_CBCR_IGNORE_ALL_CLK_DIS_SHFT                                                     23
#define HWIO_MSS_CC_Q6_ALT_CBCR_CLK_DIS_BMSK                                                          0x400000
#define HWIO_MSS_CC_Q6_ALT_CBCR_CLK_DIS_SHFT                                                                22
#define HWIO_MSS_CC_Q6_ALT_CBCR_CLK_ARES_BMSK                                                              0x4
#define HWIO_MSS_CC_Q6_ALT_CBCR_CLK_ARES_SHFT                                                                2
#define HWIO_MSS_CC_Q6_ALT_CBCR_CLK_ENABLE_BMSK                                                            0x1
#define HWIO_MSS_CC_Q6_ALT_CBCR_CLK_ENABLE_SHFT                                                              0

#define HWIO_MSS_CC_SLAVE_TIMEOUT_BUS_CBCR_ADDR                                                     (MSS_CC_MSS_CC_REG_REG_BASE            + 0x3d4)
#define HWIO_MSS_CC_SLAVE_TIMEOUT_BUS_CBCR_RMSK                                                     0x81c00005
#define HWIO_MSS_CC_SLAVE_TIMEOUT_BUS_CBCR_IN                    \
                in_dword(HWIO_MSS_CC_SLAVE_TIMEOUT_BUS_CBCR_ADDR)
#define HWIO_MSS_CC_SLAVE_TIMEOUT_BUS_CBCR_INM(m)            \
                in_dword_masked(HWIO_MSS_CC_SLAVE_TIMEOUT_BUS_CBCR_ADDR, m)
#define HWIO_MSS_CC_SLAVE_TIMEOUT_BUS_CBCR_OUT(v)            \
                out_dword(HWIO_MSS_CC_SLAVE_TIMEOUT_BUS_CBCR_ADDR,v)
#define HWIO_MSS_CC_SLAVE_TIMEOUT_BUS_CBCR_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_CC_SLAVE_TIMEOUT_BUS_CBCR_ADDR,m,v,HWIO_MSS_CC_SLAVE_TIMEOUT_BUS_CBCR_IN)
#define HWIO_MSS_CC_SLAVE_TIMEOUT_BUS_CBCR_CLK_OFF_BMSK                                             0x80000000
#define HWIO_MSS_CC_SLAVE_TIMEOUT_BUS_CBCR_CLK_OFF_SHFT                                                     31
#define HWIO_MSS_CC_SLAVE_TIMEOUT_BUS_CBCR_IGNORE_ALL_ARES_BMSK                                      0x1000000
#define HWIO_MSS_CC_SLAVE_TIMEOUT_BUS_CBCR_IGNORE_ALL_ARES_SHFT                                             24
#define HWIO_MSS_CC_SLAVE_TIMEOUT_BUS_CBCR_IGNORE_ALL_CLK_DIS_BMSK                                    0x800000
#define HWIO_MSS_CC_SLAVE_TIMEOUT_BUS_CBCR_IGNORE_ALL_CLK_DIS_SHFT                                          23
#define HWIO_MSS_CC_SLAVE_TIMEOUT_BUS_CBCR_CLK_DIS_BMSK                                               0x400000
#define HWIO_MSS_CC_SLAVE_TIMEOUT_BUS_CBCR_CLK_DIS_SHFT                                                     22
#define HWIO_MSS_CC_SLAVE_TIMEOUT_BUS_CBCR_CLK_ARES_BMSK                                                   0x4
#define HWIO_MSS_CC_SLAVE_TIMEOUT_BUS_CBCR_CLK_ARES_SHFT                                                     2
#define HWIO_MSS_CC_SLAVE_TIMEOUT_BUS_CBCR_CLK_ENABLE_BMSK                                                 0x1
#define HWIO_MSS_CC_SLAVE_TIMEOUT_BUS_CBCR_CLK_ENABLE_SHFT                                                   0

#define HWIO_MSS_CC_NAV_BUS_CX_TO_LS_CBCR_ADDR                                                      (MSS_CC_MSS_CC_REG_REG_BASE            + 0x3fc)
#define HWIO_MSS_CC_NAV_BUS_CX_TO_LS_CBCR_RMSK                                                      0x81c00005
#define HWIO_MSS_CC_NAV_BUS_CX_TO_LS_CBCR_IN                    \
                in_dword(HWIO_MSS_CC_NAV_BUS_CX_TO_LS_CBCR_ADDR)
#define HWIO_MSS_CC_NAV_BUS_CX_TO_LS_CBCR_INM(m)            \
                in_dword_masked(HWIO_MSS_CC_NAV_BUS_CX_TO_LS_CBCR_ADDR, m)
#define HWIO_MSS_CC_NAV_BUS_CX_TO_LS_CBCR_OUT(v)            \
                out_dword(HWIO_MSS_CC_NAV_BUS_CX_TO_LS_CBCR_ADDR,v)
#define HWIO_MSS_CC_NAV_BUS_CX_TO_LS_CBCR_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_CC_NAV_BUS_CX_TO_LS_CBCR_ADDR,m,v,HWIO_MSS_CC_NAV_BUS_CX_TO_LS_CBCR_IN)
#define HWIO_MSS_CC_NAV_BUS_CX_TO_LS_CBCR_CLK_OFF_BMSK                                              0x80000000
#define HWIO_MSS_CC_NAV_BUS_CX_TO_LS_CBCR_CLK_OFF_SHFT                                                      31
#define HWIO_MSS_CC_NAV_BUS_CX_TO_LS_CBCR_IGNORE_ALL_ARES_BMSK                                       0x1000000
#define HWIO_MSS_CC_NAV_BUS_CX_TO_LS_CBCR_IGNORE_ALL_ARES_SHFT                                              24
#define HWIO_MSS_CC_NAV_BUS_CX_TO_LS_CBCR_IGNORE_ALL_CLK_DIS_BMSK                                     0x800000
#define HWIO_MSS_CC_NAV_BUS_CX_TO_LS_CBCR_IGNORE_ALL_CLK_DIS_SHFT                                           23
#define HWIO_MSS_CC_NAV_BUS_CX_TO_LS_CBCR_CLK_DIS_BMSK                                                0x400000
#define HWIO_MSS_CC_NAV_BUS_CX_TO_LS_CBCR_CLK_DIS_SHFT                                                      22
#define HWIO_MSS_CC_NAV_BUS_CX_TO_LS_CBCR_CLK_ARES_BMSK                                                    0x4
#define HWIO_MSS_CC_NAV_BUS_CX_TO_LS_CBCR_CLK_ARES_SHFT                                                      2
#define HWIO_MSS_CC_NAV_BUS_CX_TO_LS_CBCR_CLK_ENABLE_BMSK                                                  0x1
#define HWIO_MSS_CC_NAV_BUS_CX_TO_LS_CBCR_CLK_ENABLE_SHFT                                                    0

#define HWIO_MSS_CC_MGPI_BUS_CBCR_ADDR                                                              (MSS_CC_MSS_CC_REG_REG_BASE            + 0x410)
#define HWIO_MSS_CC_MGPI_BUS_CBCR_RMSK                                                              0x81c00005
#define HWIO_MSS_CC_MGPI_BUS_CBCR_IN                    \
                in_dword(HWIO_MSS_CC_MGPI_BUS_CBCR_ADDR)
#define HWIO_MSS_CC_MGPI_BUS_CBCR_INM(m)            \
                in_dword_masked(HWIO_MSS_CC_MGPI_BUS_CBCR_ADDR, m)
#define HWIO_MSS_CC_MGPI_BUS_CBCR_OUT(v)            \
                out_dword(HWIO_MSS_CC_MGPI_BUS_CBCR_ADDR,v)
#define HWIO_MSS_CC_MGPI_BUS_CBCR_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_CC_MGPI_BUS_CBCR_ADDR,m,v,HWIO_MSS_CC_MGPI_BUS_CBCR_IN)
#define HWIO_MSS_CC_MGPI_BUS_CBCR_CLK_OFF_BMSK                                                      0x80000000
#define HWIO_MSS_CC_MGPI_BUS_CBCR_CLK_OFF_SHFT                                                              31
#define HWIO_MSS_CC_MGPI_BUS_CBCR_IGNORE_ALL_ARES_BMSK                                               0x1000000
#define HWIO_MSS_CC_MGPI_BUS_CBCR_IGNORE_ALL_ARES_SHFT                                                      24
#define HWIO_MSS_CC_MGPI_BUS_CBCR_IGNORE_ALL_CLK_DIS_BMSK                                             0x800000
#define HWIO_MSS_CC_MGPI_BUS_CBCR_IGNORE_ALL_CLK_DIS_SHFT                                                   23
#define HWIO_MSS_CC_MGPI_BUS_CBCR_CLK_DIS_BMSK                                                        0x400000
#define HWIO_MSS_CC_MGPI_BUS_CBCR_CLK_DIS_SHFT                                                              22
#define HWIO_MSS_CC_MGPI_BUS_CBCR_CLK_ARES_BMSK                                                            0x4
#define HWIO_MSS_CC_MGPI_BUS_CBCR_CLK_ARES_SHFT                                                              2
#define HWIO_MSS_CC_MGPI_BUS_CBCR_CLK_ENABLE_BMSK                                                          0x1
#define HWIO_MSS_CC_MGPI_BUS_CBCR_CLK_ENABLE_SHFT                                                            0

#define HWIO_MSS_CC_MODEM_XO_CX_TO_LS_CBCR_ADDR                                                     (MSS_CC_MSS_CC_REG_REG_BASE            + 0x438)
#define HWIO_MSS_CC_MODEM_XO_CX_TO_LS_CBCR_RMSK                                                     0x81c00005
#define HWIO_MSS_CC_MODEM_XO_CX_TO_LS_CBCR_IN                    \
                in_dword(HWIO_MSS_CC_MODEM_XO_CX_TO_LS_CBCR_ADDR)
#define HWIO_MSS_CC_MODEM_XO_CX_TO_LS_CBCR_INM(m)            \
                in_dword_masked(HWIO_MSS_CC_MODEM_XO_CX_TO_LS_CBCR_ADDR, m)
#define HWIO_MSS_CC_MODEM_XO_CX_TO_LS_CBCR_OUT(v)            \
                out_dword(HWIO_MSS_CC_MODEM_XO_CX_TO_LS_CBCR_ADDR,v)
#define HWIO_MSS_CC_MODEM_XO_CX_TO_LS_CBCR_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_CC_MODEM_XO_CX_TO_LS_CBCR_ADDR,m,v,HWIO_MSS_CC_MODEM_XO_CX_TO_LS_CBCR_IN)
#define HWIO_MSS_CC_MODEM_XO_CX_TO_LS_CBCR_CLK_OFF_BMSK                                             0x80000000
#define HWIO_MSS_CC_MODEM_XO_CX_TO_LS_CBCR_CLK_OFF_SHFT                                                     31
#define HWIO_MSS_CC_MODEM_XO_CX_TO_LS_CBCR_IGNORE_ALL_ARES_BMSK                                      0x1000000
#define HWIO_MSS_CC_MODEM_XO_CX_TO_LS_CBCR_IGNORE_ALL_ARES_SHFT                                             24
#define HWIO_MSS_CC_MODEM_XO_CX_TO_LS_CBCR_IGNORE_ALL_CLK_DIS_BMSK                                    0x800000
#define HWIO_MSS_CC_MODEM_XO_CX_TO_LS_CBCR_IGNORE_ALL_CLK_DIS_SHFT                                          23
#define HWIO_MSS_CC_MODEM_XO_CX_TO_LS_CBCR_CLK_DIS_BMSK                                               0x400000
#define HWIO_MSS_CC_MODEM_XO_CX_TO_LS_CBCR_CLK_DIS_SHFT                                                     22
#define HWIO_MSS_CC_MODEM_XO_CX_TO_LS_CBCR_CLK_ARES_BMSK                                                   0x4
#define HWIO_MSS_CC_MODEM_XO_CX_TO_LS_CBCR_CLK_ARES_SHFT                                                     2
#define HWIO_MSS_CC_MODEM_XO_CX_TO_LS_CBCR_CLK_ENABLE_BMSK                                                 0x1
#define HWIO_MSS_CC_MODEM_XO_CX_TO_LS_CBCR_CLK_ENABLE_SHFT                                                   0

#define HWIO_MSS_CC_MODEM_SNOC_AXI_CBCR_ADDR                                                        (MSS_CC_MSS_CC_REG_REG_BASE            + 0x44c)
#define HWIO_MSS_CC_MODEM_SNOC_AXI_CBCR_RMSK                                                        0x81c00005
#define HWIO_MSS_CC_MODEM_SNOC_AXI_CBCR_IN                    \
                in_dword(HWIO_MSS_CC_MODEM_SNOC_AXI_CBCR_ADDR)
#define HWIO_MSS_CC_MODEM_SNOC_AXI_CBCR_INM(m)            \
                in_dword_masked(HWIO_MSS_CC_MODEM_SNOC_AXI_CBCR_ADDR, m)
#define HWIO_MSS_CC_MODEM_SNOC_AXI_CBCR_OUT(v)            \
                out_dword(HWIO_MSS_CC_MODEM_SNOC_AXI_CBCR_ADDR,v)
#define HWIO_MSS_CC_MODEM_SNOC_AXI_CBCR_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_CC_MODEM_SNOC_AXI_CBCR_ADDR,m,v,HWIO_MSS_CC_MODEM_SNOC_AXI_CBCR_IN)
#define HWIO_MSS_CC_MODEM_SNOC_AXI_CBCR_CLK_OFF_BMSK                                                0x80000000
#define HWIO_MSS_CC_MODEM_SNOC_AXI_CBCR_CLK_OFF_SHFT                                                        31
#define HWIO_MSS_CC_MODEM_SNOC_AXI_CBCR_IGNORE_ALL_ARES_BMSK                                         0x1000000
#define HWIO_MSS_CC_MODEM_SNOC_AXI_CBCR_IGNORE_ALL_ARES_SHFT                                                24
#define HWIO_MSS_CC_MODEM_SNOC_AXI_CBCR_IGNORE_ALL_CLK_DIS_BMSK                                       0x800000
#define HWIO_MSS_CC_MODEM_SNOC_AXI_CBCR_IGNORE_ALL_CLK_DIS_SHFT                                             23
#define HWIO_MSS_CC_MODEM_SNOC_AXI_CBCR_CLK_DIS_BMSK                                                  0x400000
#define HWIO_MSS_CC_MODEM_SNOC_AXI_CBCR_CLK_DIS_SHFT                                                        22
#define HWIO_MSS_CC_MODEM_SNOC_AXI_CBCR_CLK_ARES_BMSK                                                      0x4
#define HWIO_MSS_CC_MODEM_SNOC_AXI_CBCR_CLK_ARES_SHFT                                                        2
#define HWIO_MSS_CC_MODEM_SNOC_AXI_CBCR_CLK_ENABLE_BMSK                                                    0x1
#define HWIO_MSS_CC_MODEM_SNOC_AXI_CBCR_CLK_ENABLE_SHFT                                                      0

#define HWIO_MSS_CC_MODEM_BRIDGE_CX_BUS_CBCR_ADDR                                                   (MSS_CC_MSS_CC_REG_REG_BASE            + 0x460)
#define HWIO_MSS_CC_MODEM_BRIDGE_CX_BUS_CBCR_RMSK                                                   0x81c00005
#define HWIO_MSS_CC_MODEM_BRIDGE_CX_BUS_CBCR_IN                    \
                in_dword(HWIO_MSS_CC_MODEM_BRIDGE_CX_BUS_CBCR_ADDR)
#define HWIO_MSS_CC_MODEM_BRIDGE_CX_BUS_CBCR_INM(m)            \
                in_dword_masked(HWIO_MSS_CC_MODEM_BRIDGE_CX_BUS_CBCR_ADDR, m)
#define HWIO_MSS_CC_MODEM_BRIDGE_CX_BUS_CBCR_OUT(v)            \
                out_dword(HWIO_MSS_CC_MODEM_BRIDGE_CX_BUS_CBCR_ADDR,v)
#define HWIO_MSS_CC_MODEM_BRIDGE_CX_BUS_CBCR_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_CC_MODEM_BRIDGE_CX_BUS_CBCR_ADDR,m,v,HWIO_MSS_CC_MODEM_BRIDGE_CX_BUS_CBCR_IN)
#define HWIO_MSS_CC_MODEM_BRIDGE_CX_BUS_CBCR_CLK_OFF_BMSK                                           0x80000000
#define HWIO_MSS_CC_MODEM_BRIDGE_CX_BUS_CBCR_CLK_OFF_SHFT                                                   31
#define HWIO_MSS_CC_MODEM_BRIDGE_CX_BUS_CBCR_IGNORE_ALL_ARES_BMSK                                    0x1000000
#define HWIO_MSS_CC_MODEM_BRIDGE_CX_BUS_CBCR_IGNORE_ALL_ARES_SHFT                                           24
#define HWIO_MSS_CC_MODEM_BRIDGE_CX_BUS_CBCR_IGNORE_ALL_CLK_DIS_BMSK                                  0x800000
#define HWIO_MSS_CC_MODEM_BRIDGE_CX_BUS_CBCR_IGNORE_ALL_CLK_DIS_SHFT                                        23
#define HWIO_MSS_CC_MODEM_BRIDGE_CX_BUS_CBCR_CLK_DIS_BMSK                                             0x400000
#define HWIO_MSS_CC_MODEM_BRIDGE_CX_BUS_CBCR_CLK_DIS_SHFT                                                   22
#define HWIO_MSS_CC_MODEM_BRIDGE_CX_BUS_CBCR_CLK_ARES_BMSK                                                 0x4
#define HWIO_MSS_CC_MODEM_BRIDGE_CX_BUS_CBCR_CLK_ARES_SHFT                                                   2
#define HWIO_MSS_CC_MODEM_BRIDGE_CX_BUS_CBCR_CLK_ENABLE_BMSK                                               0x1
#define HWIO_MSS_CC_MODEM_BRIDGE_CX_BUS_CBCR_CLK_ENABLE_SHFT                                                 0

#define HWIO_MSS_CC_MODEM_ATB_DEBUG_CBCR_ADDR                                                       (MSS_CC_MSS_CC_REG_REG_BASE            + 0x474)
#define HWIO_MSS_CC_MODEM_ATB_DEBUG_CBCR_RMSK                                                       0x81c00005
#define HWIO_MSS_CC_MODEM_ATB_DEBUG_CBCR_IN                    \
                in_dword(HWIO_MSS_CC_MODEM_ATB_DEBUG_CBCR_ADDR)
#define HWIO_MSS_CC_MODEM_ATB_DEBUG_CBCR_INM(m)            \
                in_dword_masked(HWIO_MSS_CC_MODEM_ATB_DEBUG_CBCR_ADDR, m)
#define HWIO_MSS_CC_MODEM_ATB_DEBUG_CBCR_OUT(v)            \
                out_dword(HWIO_MSS_CC_MODEM_ATB_DEBUG_CBCR_ADDR,v)
#define HWIO_MSS_CC_MODEM_ATB_DEBUG_CBCR_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_CC_MODEM_ATB_DEBUG_CBCR_ADDR,m,v,HWIO_MSS_CC_MODEM_ATB_DEBUG_CBCR_IN)
#define HWIO_MSS_CC_MODEM_ATB_DEBUG_CBCR_CLK_OFF_BMSK                                               0x80000000
#define HWIO_MSS_CC_MODEM_ATB_DEBUG_CBCR_CLK_OFF_SHFT                                                       31
#define HWIO_MSS_CC_MODEM_ATB_DEBUG_CBCR_IGNORE_ALL_ARES_BMSK                                        0x1000000
#define HWIO_MSS_CC_MODEM_ATB_DEBUG_CBCR_IGNORE_ALL_ARES_SHFT                                               24
#define HWIO_MSS_CC_MODEM_ATB_DEBUG_CBCR_IGNORE_ALL_CLK_DIS_BMSK                                      0x800000
#define HWIO_MSS_CC_MODEM_ATB_DEBUG_CBCR_IGNORE_ALL_CLK_DIS_SHFT                                            23
#define HWIO_MSS_CC_MODEM_ATB_DEBUG_CBCR_CLK_DIS_BMSK                                                 0x400000
#define HWIO_MSS_CC_MODEM_ATB_DEBUG_CBCR_CLK_DIS_SHFT                                                       22
#define HWIO_MSS_CC_MODEM_ATB_DEBUG_CBCR_CLK_ARES_BMSK                                                     0x4
#define HWIO_MSS_CC_MODEM_ATB_DEBUG_CBCR_CLK_ARES_SHFT                                                       2
#define HWIO_MSS_CC_MODEM_ATB_DEBUG_CBCR_CLK_ENABLE_BMSK                                                   0x1
#define HWIO_MSS_CC_MODEM_ATB_DEBUG_CBCR_CLK_ENABLE_SHFT                                                     0

#define HWIO_MSS_CC_MODEM_RFFE_TWO_XO_CBCR_ADDR                                                     (MSS_CC_MSS_CC_REG_REG_BASE            + 0x488)
#define HWIO_MSS_CC_MODEM_RFFE_TWO_XO_CBCR_RMSK                                                     0x81c00005
#define HWIO_MSS_CC_MODEM_RFFE_TWO_XO_CBCR_IN                    \
                in_dword(HWIO_MSS_CC_MODEM_RFFE_TWO_XO_CBCR_ADDR)
#define HWIO_MSS_CC_MODEM_RFFE_TWO_XO_CBCR_INM(m)            \
                in_dword_masked(HWIO_MSS_CC_MODEM_RFFE_TWO_XO_CBCR_ADDR, m)
#define HWIO_MSS_CC_MODEM_RFFE_TWO_XO_CBCR_OUT(v)            \
                out_dword(HWIO_MSS_CC_MODEM_RFFE_TWO_XO_CBCR_ADDR,v)
#define HWIO_MSS_CC_MODEM_RFFE_TWO_XO_CBCR_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_CC_MODEM_RFFE_TWO_XO_CBCR_ADDR,m,v,HWIO_MSS_CC_MODEM_RFFE_TWO_XO_CBCR_IN)
#define HWIO_MSS_CC_MODEM_RFFE_TWO_XO_CBCR_CLK_OFF_BMSK                                             0x80000000
#define HWIO_MSS_CC_MODEM_RFFE_TWO_XO_CBCR_CLK_OFF_SHFT                                                     31
#define HWIO_MSS_CC_MODEM_RFFE_TWO_XO_CBCR_IGNORE_ALL_ARES_BMSK                                      0x1000000
#define HWIO_MSS_CC_MODEM_RFFE_TWO_XO_CBCR_IGNORE_ALL_ARES_SHFT                                             24
#define HWIO_MSS_CC_MODEM_RFFE_TWO_XO_CBCR_IGNORE_ALL_CLK_DIS_BMSK                                    0x800000
#define HWIO_MSS_CC_MODEM_RFFE_TWO_XO_CBCR_IGNORE_ALL_CLK_DIS_SHFT                                          23
#define HWIO_MSS_CC_MODEM_RFFE_TWO_XO_CBCR_CLK_DIS_BMSK                                               0x400000
#define HWIO_MSS_CC_MODEM_RFFE_TWO_XO_CBCR_CLK_DIS_SHFT                                                     22
#define HWIO_MSS_CC_MODEM_RFFE_TWO_XO_CBCR_CLK_ARES_BMSK                                                   0x4
#define HWIO_MSS_CC_MODEM_RFFE_TWO_XO_CBCR_CLK_ARES_SHFT                                                     2
#define HWIO_MSS_CC_MODEM_RFFE_TWO_XO_CBCR_CLK_ENABLE_BMSK                                                 0x1
#define HWIO_MSS_CC_MODEM_RFFE_TWO_XO_CBCR_CLK_ENABLE_SHFT                                                   0

#define HWIO_MSS_CC_MODEM_CORE_CBCR_ADDR                                                            (MSS_CC_MSS_CC_REG_REG_BASE            + 0x49c)
#define HWIO_MSS_CC_MODEM_CORE_CBCR_RMSK                                                            0x81c07ff5
#define HWIO_MSS_CC_MODEM_CORE_CBCR_IN                    \
                in_dword(HWIO_MSS_CC_MODEM_CORE_CBCR_ADDR)
#define HWIO_MSS_CC_MODEM_CORE_CBCR_INM(m)            \
                in_dword_masked(HWIO_MSS_CC_MODEM_CORE_CBCR_ADDR, m)
#define HWIO_MSS_CC_MODEM_CORE_CBCR_OUT(v)            \
                out_dword(HWIO_MSS_CC_MODEM_CORE_CBCR_ADDR,v)
#define HWIO_MSS_CC_MODEM_CORE_CBCR_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_CC_MODEM_CORE_CBCR_ADDR,m,v,HWIO_MSS_CC_MODEM_CORE_CBCR_IN)
#define HWIO_MSS_CC_MODEM_CORE_CBCR_CLK_OFF_BMSK                                                    0x80000000
#define HWIO_MSS_CC_MODEM_CORE_CBCR_CLK_OFF_SHFT                                                            31
#define HWIO_MSS_CC_MODEM_CORE_CBCR_IGNORE_ALL_ARES_BMSK                                             0x1000000
#define HWIO_MSS_CC_MODEM_CORE_CBCR_IGNORE_ALL_ARES_SHFT                                                    24
#define HWIO_MSS_CC_MODEM_CORE_CBCR_IGNORE_ALL_CLK_DIS_BMSK                                           0x800000
#define HWIO_MSS_CC_MODEM_CORE_CBCR_IGNORE_ALL_CLK_DIS_SHFT                                                 23
#define HWIO_MSS_CC_MODEM_CORE_CBCR_CLK_DIS_BMSK                                                      0x400000
#define HWIO_MSS_CC_MODEM_CORE_CBCR_CLK_DIS_SHFT                                                            22
#define HWIO_MSS_CC_MODEM_CORE_CBCR_FORCE_MEM_CORE_ON_BMSK                                              0x4000
#define HWIO_MSS_CC_MODEM_CORE_CBCR_FORCE_MEM_CORE_ON_SHFT                                                  14
#define HWIO_MSS_CC_MODEM_CORE_CBCR_FORCE_MEM_PERIPH_ON_BMSK                                            0x2000
#define HWIO_MSS_CC_MODEM_CORE_CBCR_FORCE_MEM_PERIPH_ON_SHFT                                                13
#define HWIO_MSS_CC_MODEM_CORE_CBCR_FORCE_MEM_PERIPH_OFF_BMSK                                           0x1000
#define HWIO_MSS_CC_MODEM_CORE_CBCR_FORCE_MEM_PERIPH_OFF_SHFT                                               12
#define HWIO_MSS_CC_MODEM_CORE_CBCR_WAKEUP_BMSK                                                          0xf00
#define HWIO_MSS_CC_MODEM_CORE_CBCR_WAKEUP_SHFT                                                              8
#define HWIO_MSS_CC_MODEM_CORE_CBCR_SLEEP_BMSK                                                            0xf0
#define HWIO_MSS_CC_MODEM_CORE_CBCR_SLEEP_SHFT                                                               4
#define HWIO_MSS_CC_MODEM_CORE_CBCR_CLK_ARES_BMSK                                                          0x4
#define HWIO_MSS_CC_MODEM_CORE_CBCR_CLK_ARES_SHFT                                                            2
#define HWIO_MSS_CC_MODEM_CORE_CBCR_CLK_ENABLE_BMSK                                                        0x1
#define HWIO_MSS_CC_MODEM_CORE_CBCR_CLK_ENABLE_SHFT                                                          0

#define HWIO_MSS_CC_MODEM_CORE_SREGR_ADDR                                                           (MSS_CC_MSS_CC_REG_REG_BASE            + 0x4a0)
#define HWIO_MSS_CC_MODEM_CORE_SREGR_RMSK                                                           0xfffffffe
#define HWIO_MSS_CC_MODEM_CORE_SREGR_IN                    \
                in_dword(HWIO_MSS_CC_MODEM_CORE_SREGR_ADDR)
#define HWIO_MSS_CC_MODEM_CORE_SREGR_INM(m)            \
                in_dword_masked(HWIO_MSS_CC_MODEM_CORE_SREGR_ADDR, m)
#define HWIO_MSS_CC_MODEM_CORE_SREGR_OUT(v)            \
                out_dword(HWIO_MSS_CC_MODEM_CORE_SREGR_ADDR,v)
#define HWIO_MSS_CC_MODEM_CORE_SREGR_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_CC_MODEM_CORE_SREGR_ADDR,m,v,HWIO_MSS_CC_MODEM_CORE_SREGR_IN)
#define HWIO_MSS_CC_MODEM_CORE_SREGR_SREG_PSCBC_SPARE_CTRL_OUT_BMSK                                 0xff000000
#define HWIO_MSS_CC_MODEM_CORE_SREGR_SREG_PSCBC_SPARE_CTRL_OUT_SHFT                                         24
#define HWIO_MSS_CC_MODEM_CORE_SREGR_SREG_PSCBC_SPARE_CTRL_IN_BMSK                                    0xff0000
#define HWIO_MSS_CC_MODEM_CORE_SREGR_SREG_PSCBC_SPARE_CTRL_IN_SHFT                                          16
#define HWIO_MSS_CC_MODEM_CORE_SREGR_IGNORE_GDSC_PWR_DWN_CSR_BMSK                                       0x8000
#define HWIO_MSS_CC_MODEM_CORE_SREGR_IGNORE_GDSC_PWR_DWN_CSR_SHFT                                           15
#define HWIO_MSS_CC_MODEM_CORE_SREGR_PSCBC_SLP_STG_MODE_CSR_BMSK                                        0x4000
#define HWIO_MSS_CC_MODEM_CORE_SREGR_PSCBC_SLP_STG_MODE_CSR_SHFT                                            14
#define HWIO_MSS_CC_MODEM_CORE_SREGR_MEM_CPH_RST_SW_OVERRIDE_BMSK                                       0x2000
#define HWIO_MSS_CC_MODEM_CORE_SREGR_MEM_CPH_RST_SW_OVERRIDE_SHFT                                           13
#define HWIO_MSS_CC_MODEM_CORE_SREGR_SW_SM_PSCBC_SEQ_IN_OVERRIDE_BMSK                                   0x1000
#define HWIO_MSS_CC_MODEM_CORE_SREGR_SW_SM_PSCBC_SEQ_IN_OVERRIDE_SHFT                                       12
#define HWIO_MSS_CC_MODEM_CORE_SREGR_MEM_CORE_ON_ACK_BMSK                                                0x800
#define HWIO_MSS_CC_MODEM_CORE_SREGR_MEM_CORE_ON_ACK_SHFT                                                   11
#define HWIO_MSS_CC_MODEM_CORE_SREGR_MEM_PERIPH_ON_ACK_BMSK                                              0x400
#define HWIO_MSS_CC_MODEM_CORE_SREGR_MEM_PERIPH_ON_ACK_SHFT                                                 10
#define HWIO_MSS_CC_MODEM_CORE_SREGR_SW_DIV_RATIO_SLP_STG_CLK_BMSK                                       0x300
#define HWIO_MSS_CC_MODEM_CORE_SREGR_SW_DIV_RATIO_SLP_STG_CLK_SHFT                                           8
#define HWIO_MSS_CC_MODEM_CORE_SREGR_MEM_CPH_ENABLE_BMSK                                                  0x80
#define HWIO_MSS_CC_MODEM_CORE_SREGR_MEM_CPH_ENABLE_SHFT                                                     7
#define HWIO_MSS_CC_MODEM_CORE_SREGR_FORCE_CLK_ON_BMSK                                                    0x40
#define HWIO_MSS_CC_MODEM_CORE_SREGR_FORCE_CLK_ON_SHFT                                                       6
#define HWIO_MSS_CC_MODEM_CORE_SREGR_SW_RST_SEL_SLP_STG_BMSK                                              0x20
#define HWIO_MSS_CC_MODEM_CORE_SREGR_SW_RST_SEL_SLP_STG_SHFT                                                 5
#define HWIO_MSS_CC_MODEM_CORE_SREGR_SW_RST_SLP_STG_BMSK                                                  0x10
#define HWIO_MSS_CC_MODEM_CORE_SREGR_SW_RST_SLP_STG_SHFT                                                     4
#define HWIO_MSS_CC_MODEM_CORE_SREGR_SW_CTRL_PWR_DOWN_BMSK                                                 0x8
#define HWIO_MSS_CC_MODEM_CORE_SREGR_SW_CTRL_PWR_DOWN_SHFT                                                   3
#define HWIO_MSS_CC_MODEM_CORE_SREGR_SW_CLK_EN_SEL_SLP_STG_BMSK                                            0x4
#define HWIO_MSS_CC_MODEM_CORE_SREGR_SW_CLK_EN_SEL_SLP_STG_SHFT                                              2
#define HWIO_MSS_CC_MODEM_CORE_SREGR_SW_CLK_EN_SLP_STG_BMSK                                                0x2
#define HWIO_MSS_CC_MODEM_CORE_SREGR_SW_CLK_EN_SLP_STG_SHFT                                                  1

#define HWIO_MSS_CC_MODEM_CCS_CBCR_ADDR                                                             (MSS_CC_MSS_CC_REG_REG_BASE            + 0x4b0)
#define HWIO_MSS_CC_MODEM_CCS_CBCR_RMSK                                                             0x81c07ff5
#define HWIO_MSS_CC_MODEM_CCS_CBCR_IN                    \
                in_dword(HWIO_MSS_CC_MODEM_CCS_CBCR_ADDR)
#define HWIO_MSS_CC_MODEM_CCS_CBCR_INM(m)            \
                in_dword_masked(HWIO_MSS_CC_MODEM_CCS_CBCR_ADDR, m)
#define HWIO_MSS_CC_MODEM_CCS_CBCR_OUT(v)            \
                out_dword(HWIO_MSS_CC_MODEM_CCS_CBCR_ADDR,v)
#define HWIO_MSS_CC_MODEM_CCS_CBCR_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_CC_MODEM_CCS_CBCR_ADDR,m,v,HWIO_MSS_CC_MODEM_CCS_CBCR_IN)
#define HWIO_MSS_CC_MODEM_CCS_CBCR_CLK_OFF_BMSK                                                     0x80000000
#define HWIO_MSS_CC_MODEM_CCS_CBCR_CLK_OFF_SHFT                                                             31
#define HWIO_MSS_CC_MODEM_CCS_CBCR_IGNORE_ALL_ARES_BMSK                                              0x1000000
#define HWIO_MSS_CC_MODEM_CCS_CBCR_IGNORE_ALL_ARES_SHFT                                                     24
#define HWIO_MSS_CC_MODEM_CCS_CBCR_IGNORE_ALL_CLK_DIS_BMSK                                            0x800000
#define HWIO_MSS_CC_MODEM_CCS_CBCR_IGNORE_ALL_CLK_DIS_SHFT                                                  23
#define HWIO_MSS_CC_MODEM_CCS_CBCR_CLK_DIS_BMSK                                                       0x400000
#define HWIO_MSS_CC_MODEM_CCS_CBCR_CLK_DIS_SHFT                                                             22
#define HWIO_MSS_CC_MODEM_CCS_CBCR_FORCE_MEM_CORE_ON_BMSK                                               0x4000
#define HWIO_MSS_CC_MODEM_CCS_CBCR_FORCE_MEM_CORE_ON_SHFT                                                   14
#define HWIO_MSS_CC_MODEM_CCS_CBCR_FORCE_MEM_PERIPH_ON_BMSK                                             0x2000
#define HWIO_MSS_CC_MODEM_CCS_CBCR_FORCE_MEM_PERIPH_ON_SHFT                                                 13
#define HWIO_MSS_CC_MODEM_CCS_CBCR_FORCE_MEM_PERIPH_OFF_BMSK                                            0x1000
#define HWIO_MSS_CC_MODEM_CCS_CBCR_FORCE_MEM_PERIPH_OFF_SHFT                                                12
#define HWIO_MSS_CC_MODEM_CCS_CBCR_WAKEUP_BMSK                                                           0xf00
#define HWIO_MSS_CC_MODEM_CCS_CBCR_WAKEUP_SHFT                                                               8
#define HWIO_MSS_CC_MODEM_CCS_CBCR_SLEEP_BMSK                                                             0xf0
#define HWIO_MSS_CC_MODEM_CCS_CBCR_SLEEP_SHFT                                                                4
#define HWIO_MSS_CC_MODEM_CCS_CBCR_CLK_ARES_BMSK                                                           0x4
#define HWIO_MSS_CC_MODEM_CCS_CBCR_CLK_ARES_SHFT                                                             2
#define HWIO_MSS_CC_MODEM_CCS_CBCR_CLK_ENABLE_BMSK                                                         0x1
#define HWIO_MSS_CC_MODEM_CCS_CBCR_CLK_ENABLE_SHFT                                                           0

#define HWIO_MSS_CC_MODEM_CCS_SREGR_ADDR                                                            (MSS_CC_MSS_CC_REG_REG_BASE            + 0x4b4)
#define HWIO_MSS_CC_MODEM_CCS_SREGR_RMSK                                                            0xfffffffe
#define HWIO_MSS_CC_MODEM_CCS_SREGR_IN                    \
                in_dword(HWIO_MSS_CC_MODEM_CCS_SREGR_ADDR)
#define HWIO_MSS_CC_MODEM_CCS_SREGR_INM(m)            \
                in_dword_masked(HWIO_MSS_CC_MODEM_CCS_SREGR_ADDR, m)
#define HWIO_MSS_CC_MODEM_CCS_SREGR_OUT(v)            \
                out_dword(HWIO_MSS_CC_MODEM_CCS_SREGR_ADDR,v)
#define HWIO_MSS_CC_MODEM_CCS_SREGR_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_CC_MODEM_CCS_SREGR_ADDR,m,v,HWIO_MSS_CC_MODEM_CCS_SREGR_IN)
#define HWIO_MSS_CC_MODEM_CCS_SREGR_SREG_PSCBC_SPARE_CTRL_OUT_BMSK                                  0xff000000
#define HWIO_MSS_CC_MODEM_CCS_SREGR_SREG_PSCBC_SPARE_CTRL_OUT_SHFT                                          24
#define HWIO_MSS_CC_MODEM_CCS_SREGR_SREG_PSCBC_SPARE_CTRL_IN_BMSK                                     0xff0000
#define HWIO_MSS_CC_MODEM_CCS_SREGR_SREG_PSCBC_SPARE_CTRL_IN_SHFT                                           16
#define HWIO_MSS_CC_MODEM_CCS_SREGR_IGNORE_GDSC_PWR_DWN_CSR_BMSK                                        0x8000
#define HWIO_MSS_CC_MODEM_CCS_SREGR_IGNORE_GDSC_PWR_DWN_CSR_SHFT                                            15
#define HWIO_MSS_CC_MODEM_CCS_SREGR_PSCBC_SLP_STG_MODE_CSR_BMSK                                         0x4000
#define HWIO_MSS_CC_MODEM_CCS_SREGR_PSCBC_SLP_STG_MODE_CSR_SHFT                                             14
#define HWIO_MSS_CC_MODEM_CCS_SREGR_MEM_CPH_RST_SW_OVERRIDE_BMSK                                        0x2000
#define HWIO_MSS_CC_MODEM_CCS_SREGR_MEM_CPH_RST_SW_OVERRIDE_SHFT                                            13
#define HWIO_MSS_CC_MODEM_CCS_SREGR_SW_SM_PSCBC_SEQ_IN_OVERRIDE_BMSK                                    0x1000
#define HWIO_MSS_CC_MODEM_CCS_SREGR_SW_SM_PSCBC_SEQ_IN_OVERRIDE_SHFT                                        12
#define HWIO_MSS_CC_MODEM_CCS_SREGR_MEM_CORE_ON_ACK_BMSK                                                 0x800
#define HWIO_MSS_CC_MODEM_CCS_SREGR_MEM_CORE_ON_ACK_SHFT                                                    11
#define HWIO_MSS_CC_MODEM_CCS_SREGR_MEM_PERIPH_ON_ACK_BMSK                                               0x400
#define HWIO_MSS_CC_MODEM_CCS_SREGR_MEM_PERIPH_ON_ACK_SHFT                                                  10
#define HWIO_MSS_CC_MODEM_CCS_SREGR_SW_DIV_RATIO_SLP_STG_CLK_BMSK                                        0x300
#define HWIO_MSS_CC_MODEM_CCS_SREGR_SW_DIV_RATIO_SLP_STG_CLK_SHFT                                            8
#define HWIO_MSS_CC_MODEM_CCS_SREGR_MEM_CPH_ENABLE_BMSK                                                   0x80
#define HWIO_MSS_CC_MODEM_CCS_SREGR_MEM_CPH_ENABLE_SHFT                                                      7
#define HWIO_MSS_CC_MODEM_CCS_SREGR_FORCE_CLK_ON_BMSK                                                     0x40
#define HWIO_MSS_CC_MODEM_CCS_SREGR_FORCE_CLK_ON_SHFT                                                        6
#define HWIO_MSS_CC_MODEM_CCS_SREGR_SW_RST_SEL_SLP_STG_BMSK                                               0x20
#define HWIO_MSS_CC_MODEM_CCS_SREGR_SW_RST_SEL_SLP_STG_SHFT                                                  5
#define HWIO_MSS_CC_MODEM_CCS_SREGR_SW_RST_SLP_STG_BMSK                                                   0x10
#define HWIO_MSS_CC_MODEM_CCS_SREGR_SW_RST_SLP_STG_SHFT                                                      4
#define HWIO_MSS_CC_MODEM_CCS_SREGR_SW_CTRL_PWR_DOWN_BMSK                                                  0x8
#define HWIO_MSS_CC_MODEM_CCS_SREGR_SW_CTRL_PWR_DOWN_SHFT                                                    3
#define HWIO_MSS_CC_MODEM_CCS_SREGR_SW_CLK_EN_SEL_SLP_STG_BMSK                                             0x4
#define HWIO_MSS_CC_MODEM_CCS_SREGR_SW_CLK_EN_SEL_SLP_STG_SHFT                                               2
#define HWIO_MSS_CC_MODEM_CCS_SREGR_SW_CLK_EN_SLP_STG_BMSK                                                 0x2
#define HWIO_MSS_CC_MODEM_CCS_SREGR_SW_CLK_EN_SLP_STG_SHFT                                                   1

#define HWIO_MSS_CC_MODEM_BUS_CBCR_ADDR                                                             (MSS_CC_MSS_CC_REG_REG_BASE            + 0x4c4)
#define HWIO_MSS_CC_MODEM_BUS_CBCR_RMSK                                                             0x81c0000f
#define HWIO_MSS_CC_MODEM_BUS_CBCR_IN                    \
                in_dword(HWIO_MSS_CC_MODEM_BUS_CBCR_ADDR)
#define HWIO_MSS_CC_MODEM_BUS_CBCR_INM(m)            \
                in_dword_masked(HWIO_MSS_CC_MODEM_BUS_CBCR_ADDR, m)
#define HWIO_MSS_CC_MODEM_BUS_CBCR_OUT(v)            \
                out_dword(HWIO_MSS_CC_MODEM_BUS_CBCR_ADDR,v)
#define HWIO_MSS_CC_MODEM_BUS_CBCR_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_CC_MODEM_BUS_CBCR_ADDR,m,v,HWIO_MSS_CC_MODEM_BUS_CBCR_IN)
#define HWIO_MSS_CC_MODEM_BUS_CBCR_CLK_OFF_BMSK                                                     0x80000000
#define HWIO_MSS_CC_MODEM_BUS_CBCR_CLK_OFF_SHFT                                                             31
#define HWIO_MSS_CC_MODEM_BUS_CBCR_IGNORE_ALL_ARES_BMSK                                              0x1000000
#define HWIO_MSS_CC_MODEM_BUS_CBCR_IGNORE_ALL_ARES_SHFT                                                     24
#define HWIO_MSS_CC_MODEM_BUS_CBCR_IGNORE_ALL_CLK_DIS_BMSK                                            0x800000
#define HWIO_MSS_CC_MODEM_BUS_CBCR_IGNORE_ALL_CLK_DIS_SHFT                                                  23
#define HWIO_MSS_CC_MODEM_BUS_CBCR_CLK_DIS_BMSK                                                       0x400000
#define HWIO_MSS_CC_MODEM_BUS_CBCR_CLK_DIS_SHFT                                                             22
#define HWIO_MSS_CC_MODEM_BUS_CBCR_SW_ONLY_EN_BMSK                                                         0x8
#define HWIO_MSS_CC_MODEM_BUS_CBCR_SW_ONLY_EN_SHFT                                                           3
#define HWIO_MSS_CC_MODEM_BUS_CBCR_CLK_ARES_BMSK                                                           0x4
#define HWIO_MSS_CC_MODEM_BUS_CBCR_CLK_ARES_SHFT                                                             2
#define HWIO_MSS_CC_MODEM_BUS_CBCR_HW_CTL_BMSK                                                             0x2
#define HWIO_MSS_CC_MODEM_BUS_CBCR_HW_CTL_SHFT                                                               1
#define HWIO_MSS_CC_MODEM_BUS_CBCR_CLK_ENABLE_BMSK                                                         0x1
#define HWIO_MSS_CC_MODEM_BUS_CBCR_CLK_ENABLE_SHFT                                                           0

#define HWIO_MSS_CC_MODEM_TX_CORE_CBCR_ADDR                                                         (MSS_CC_MSS_CC_REG_REG_BASE            + 0x4ec)
#define HWIO_MSS_CC_MODEM_TX_CORE_CBCR_RMSK                                                         0x81c07ff5
#define HWIO_MSS_CC_MODEM_TX_CORE_CBCR_IN                    \
                in_dword(HWIO_MSS_CC_MODEM_TX_CORE_CBCR_ADDR)
#define HWIO_MSS_CC_MODEM_TX_CORE_CBCR_INM(m)            \
                in_dword_masked(HWIO_MSS_CC_MODEM_TX_CORE_CBCR_ADDR, m)
#define HWIO_MSS_CC_MODEM_TX_CORE_CBCR_OUT(v)            \
                out_dword(HWIO_MSS_CC_MODEM_TX_CORE_CBCR_ADDR,v)
#define HWIO_MSS_CC_MODEM_TX_CORE_CBCR_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_CC_MODEM_TX_CORE_CBCR_ADDR,m,v,HWIO_MSS_CC_MODEM_TX_CORE_CBCR_IN)
#define HWIO_MSS_CC_MODEM_TX_CORE_CBCR_CLK_OFF_BMSK                                                 0x80000000
#define HWIO_MSS_CC_MODEM_TX_CORE_CBCR_CLK_OFF_SHFT                                                         31
#define HWIO_MSS_CC_MODEM_TX_CORE_CBCR_IGNORE_ALL_ARES_BMSK                                          0x1000000
#define HWIO_MSS_CC_MODEM_TX_CORE_CBCR_IGNORE_ALL_ARES_SHFT                                                 24
#define HWIO_MSS_CC_MODEM_TX_CORE_CBCR_IGNORE_ALL_CLK_DIS_BMSK                                        0x800000
#define HWIO_MSS_CC_MODEM_TX_CORE_CBCR_IGNORE_ALL_CLK_DIS_SHFT                                              23
#define HWIO_MSS_CC_MODEM_TX_CORE_CBCR_CLK_DIS_BMSK                                                   0x400000
#define HWIO_MSS_CC_MODEM_TX_CORE_CBCR_CLK_DIS_SHFT                                                         22
#define HWIO_MSS_CC_MODEM_TX_CORE_CBCR_FORCE_MEM_CORE_ON_BMSK                                           0x4000
#define HWIO_MSS_CC_MODEM_TX_CORE_CBCR_FORCE_MEM_CORE_ON_SHFT                                               14
#define HWIO_MSS_CC_MODEM_TX_CORE_CBCR_FORCE_MEM_PERIPH_ON_BMSK                                         0x2000
#define HWIO_MSS_CC_MODEM_TX_CORE_CBCR_FORCE_MEM_PERIPH_ON_SHFT                                             13
#define HWIO_MSS_CC_MODEM_TX_CORE_CBCR_FORCE_MEM_PERIPH_OFF_BMSK                                        0x1000
#define HWIO_MSS_CC_MODEM_TX_CORE_CBCR_FORCE_MEM_PERIPH_OFF_SHFT                                            12
#define HWIO_MSS_CC_MODEM_TX_CORE_CBCR_WAKEUP_BMSK                                                       0xf00
#define HWIO_MSS_CC_MODEM_TX_CORE_CBCR_WAKEUP_SHFT                                                           8
#define HWIO_MSS_CC_MODEM_TX_CORE_CBCR_SLEEP_BMSK                                                         0xf0
#define HWIO_MSS_CC_MODEM_TX_CORE_CBCR_SLEEP_SHFT                                                            4
#define HWIO_MSS_CC_MODEM_TX_CORE_CBCR_CLK_ARES_BMSK                                                       0x4
#define HWIO_MSS_CC_MODEM_TX_CORE_CBCR_CLK_ARES_SHFT                                                         2
#define HWIO_MSS_CC_MODEM_TX_CORE_CBCR_CLK_ENABLE_BMSK                                                     0x1
#define HWIO_MSS_CC_MODEM_TX_CORE_CBCR_CLK_ENABLE_SHFT                                                       0

#define HWIO_MSS_CC_MODEM_TX_CORE_SREGR_ADDR                                                        (MSS_CC_MSS_CC_REG_REG_BASE            + 0x4f0)
#define HWIO_MSS_CC_MODEM_TX_CORE_SREGR_RMSK                                                        0xfffffffe
#define HWIO_MSS_CC_MODEM_TX_CORE_SREGR_IN                    \
                in_dword(HWIO_MSS_CC_MODEM_TX_CORE_SREGR_ADDR)
#define HWIO_MSS_CC_MODEM_TX_CORE_SREGR_INM(m)            \
                in_dword_masked(HWIO_MSS_CC_MODEM_TX_CORE_SREGR_ADDR, m)
#define HWIO_MSS_CC_MODEM_TX_CORE_SREGR_OUT(v)            \
                out_dword(HWIO_MSS_CC_MODEM_TX_CORE_SREGR_ADDR,v)
#define HWIO_MSS_CC_MODEM_TX_CORE_SREGR_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_CC_MODEM_TX_CORE_SREGR_ADDR,m,v,HWIO_MSS_CC_MODEM_TX_CORE_SREGR_IN)
#define HWIO_MSS_CC_MODEM_TX_CORE_SREGR_SREG_PSCBC_SPARE_CTRL_OUT_BMSK                              0xff000000
#define HWIO_MSS_CC_MODEM_TX_CORE_SREGR_SREG_PSCBC_SPARE_CTRL_OUT_SHFT                                      24
#define HWIO_MSS_CC_MODEM_TX_CORE_SREGR_SREG_PSCBC_SPARE_CTRL_IN_BMSK                                 0xff0000
#define HWIO_MSS_CC_MODEM_TX_CORE_SREGR_SREG_PSCBC_SPARE_CTRL_IN_SHFT                                       16
#define HWIO_MSS_CC_MODEM_TX_CORE_SREGR_IGNORE_GDSC_PWR_DWN_CSR_BMSK                                    0x8000
#define HWIO_MSS_CC_MODEM_TX_CORE_SREGR_IGNORE_GDSC_PWR_DWN_CSR_SHFT                                        15
#define HWIO_MSS_CC_MODEM_TX_CORE_SREGR_PSCBC_SLP_STG_MODE_CSR_BMSK                                     0x4000
#define HWIO_MSS_CC_MODEM_TX_CORE_SREGR_PSCBC_SLP_STG_MODE_CSR_SHFT                                         14
#define HWIO_MSS_CC_MODEM_TX_CORE_SREGR_MEM_CPH_RST_SW_OVERRIDE_BMSK                                    0x2000
#define HWIO_MSS_CC_MODEM_TX_CORE_SREGR_MEM_CPH_RST_SW_OVERRIDE_SHFT                                        13
#define HWIO_MSS_CC_MODEM_TX_CORE_SREGR_SW_SM_PSCBC_SEQ_IN_OVERRIDE_BMSK                                0x1000
#define HWIO_MSS_CC_MODEM_TX_CORE_SREGR_SW_SM_PSCBC_SEQ_IN_OVERRIDE_SHFT                                    12
#define HWIO_MSS_CC_MODEM_TX_CORE_SREGR_MEM_CORE_ON_ACK_BMSK                                             0x800
#define HWIO_MSS_CC_MODEM_TX_CORE_SREGR_MEM_CORE_ON_ACK_SHFT                                                11
#define HWIO_MSS_CC_MODEM_TX_CORE_SREGR_MEM_PERIPH_ON_ACK_BMSK                                           0x400
#define HWIO_MSS_CC_MODEM_TX_CORE_SREGR_MEM_PERIPH_ON_ACK_SHFT                                              10
#define HWIO_MSS_CC_MODEM_TX_CORE_SREGR_SW_DIV_RATIO_SLP_STG_CLK_BMSK                                    0x300
#define HWIO_MSS_CC_MODEM_TX_CORE_SREGR_SW_DIV_RATIO_SLP_STG_CLK_SHFT                                        8
#define HWIO_MSS_CC_MODEM_TX_CORE_SREGR_MEM_CPH_ENABLE_BMSK                                               0x80
#define HWIO_MSS_CC_MODEM_TX_CORE_SREGR_MEM_CPH_ENABLE_SHFT                                                  7
#define HWIO_MSS_CC_MODEM_TX_CORE_SREGR_FORCE_CLK_ON_BMSK                                                 0x40
#define HWIO_MSS_CC_MODEM_TX_CORE_SREGR_FORCE_CLK_ON_SHFT                                                    6
#define HWIO_MSS_CC_MODEM_TX_CORE_SREGR_SW_RST_SEL_SLP_STG_BMSK                                           0x20
#define HWIO_MSS_CC_MODEM_TX_CORE_SREGR_SW_RST_SEL_SLP_STG_SHFT                                              5
#define HWIO_MSS_CC_MODEM_TX_CORE_SREGR_SW_RST_SLP_STG_BMSK                                               0x10
#define HWIO_MSS_CC_MODEM_TX_CORE_SREGR_SW_RST_SLP_STG_SHFT                                                  4
#define HWIO_MSS_CC_MODEM_TX_CORE_SREGR_SW_CTRL_PWR_DOWN_BMSK                                              0x8
#define HWIO_MSS_CC_MODEM_TX_CORE_SREGR_SW_CTRL_PWR_DOWN_SHFT                                                3
#define HWIO_MSS_CC_MODEM_TX_CORE_SREGR_SW_CLK_EN_SEL_SLP_STG_BMSK                                         0x4
#define HWIO_MSS_CC_MODEM_TX_CORE_SREGR_SW_CLK_EN_SEL_SLP_STG_SHFT                                           2
#define HWIO_MSS_CC_MODEM_TX_CORE_SREGR_SW_CLK_EN_SLP_STG_BMSK                                             0x2
#define HWIO_MSS_CC_MODEM_TX_CORE_SREGR_SW_CLK_EN_SLP_STG_SHFT                                               1

#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_CBCR_ADDR                                                    (MSS_CC_MSS_CC_REG_REG_BASE            + 0x514)
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_CBCR_RMSK                                                    0x81c07ff5
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_CBCR_IN                    \
                in_dword(HWIO_MSS_CC_MODEM_DEMBACK_CORE_CBCR_ADDR)
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_CBCR_INM(m)            \
                in_dword_masked(HWIO_MSS_CC_MODEM_DEMBACK_CORE_CBCR_ADDR, m)
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_CBCR_OUT(v)            \
                out_dword(HWIO_MSS_CC_MODEM_DEMBACK_CORE_CBCR_ADDR,v)
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_CBCR_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_CC_MODEM_DEMBACK_CORE_CBCR_ADDR,m,v,HWIO_MSS_CC_MODEM_DEMBACK_CORE_CBCR_IN)
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_CBCR_CLK_OFF_BMSK                                            0x80000000
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_CBCR_CLK_OFF_SHFT                                                    31
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_CBCR_IGNORE_ALL_ARES_BMSK                                     0x1000000
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_CBCR_IGNORE_ALL_ARES_SHFT                                            24
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_CBCR_IGNORE_ALL_CLK_DIS_BMSK                                   0x800000
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_CBCR_IGNORE_ALL_CLK_DIS_SHFT                                         23
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_CBCR_CLK_DIS_BMSK                                              0x400000
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_CBCR_CLK_DIS_SHFT                                                    22
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_CBCR_FORCE_MEM_CORE_ON_BMSK                                      0x4000
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_CBCR_FORCE_MEM_CORE_ON_SHFT                                          14
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_CBCR_FORCE_MEM_PERIPH_ON_BMSK                                    0x2000
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_CBCR_FORCE_MEM_PERIPH_ON_SHFT                                        13
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_CBCR_FORCE_MEM_PERIPH_OFF_BMSK                                   0x1000
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_CBCR_FORCE_MEM_PERIPH_OFF_SHFT                                       12
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_CBCR_WAKEUP_BMSK                                                  0xf00
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_CBCR_WAKEUP_SHFT                                                      8
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_CBCR_SLEEP_BMSK                                                    0xf0
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_CBCR_SLEEP_SHFT                                                       4
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_CBCR_CLK_ARES_BMSK                                                  0x4
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_CBCR_CLK_ARES_SHFT                                                    2
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_CBCR_CLK_ENABLE_BMSK                                                0x1
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_CBCR_CLK_ENABLE_SHFT                                                  0

#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_SREGR_ADDR                                                   (MSS_CC_MSS_CC_REG_REG_BASE            + 0x518)
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_SREGR_RMSK                                                   0xfffffffe
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_SREGR_IN                    \
                in_dword(HWIO_MSS_CC_MODEM_DEMBACK_CORE_SREGR_ADDR)
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_SREGR_INM(m)            \
                in_dword_masked(HWIO_MSS_CC_MODEM_DEMBACK_CORE_SREGR_ADDR, m)
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_SREGR_OUT(v)            \
                out_dword(HWIO_MSS_CC_MODEM_DEMBACK_CORE_SREGR_ADDR,v)
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_SREGR_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_CC_MODEM_DEMBACK_CORE_SREGR_ADDR,m,v,HWIO_MSS_CC_MODEM_DEMBACK_CORE_SREGR_IN)
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_SREGR_SREG_PSCBC_SPARE_CTRL_OUT_BMSK                         0xff000000
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_SREGR_SREG_PSCBC_SPARE_CTRL_OUT_SHFT                                 24
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_SREGR_SREG_PSCBC_SPARE_CTRL_IN_BMSK                            0xff0000
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_SREGR_SREG_PSCBC_SPARE_CTRL_IN_SHFT                                  16
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_SREGR_IGNORE_GDSC_PWR_DWN_CSR_BMSK                               0x8000
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_SREGR_IGNORE_GDSC_PWR_DWN_CSR_SHFT                                   15
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_SREGR_PSCBC_SLP_STG_MODE_CSR_BMSK                                0x4000
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_SREGR_PSCBC_SLP_STG_MODE_CSR_SHFT                                    14
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_SREGR_MEM_CPH_RST_SW_OVERRIDE_BMSK                               0x2000
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_SREGR_MEM_CPH_RST_SW_OVERRIDE_SHFT                                   13
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_SREGR_SW_SM_PSCBC_SEQ_IN_OVERRIDE_BMSK                           0x1000
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_SREGR_SW_SM_PSCBC_SEQ_IN_OVERRIDE_SHFT                               12
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_SREGR_MEM_CORE_ON_ACK_BMSK                                        0x800
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_SREGR_MEM_CORE_ON_ACK_SHFT                                           11
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_SREGR_MEM_PERIPH_ON_ACK_BMSK                                      0x400
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_SREGR_MEM_PERIPH_ON_ACK_SHFT                                         10
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_SREGR_SW_DIV_RATIO_SLP_STG_CLK_BMSK                               0x300
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_SREGR_SW_DIV_RATIO_SLP_STG_CLK_SHFT                                   8
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_SREGR_MEM_CPH_ENABLE_BMSK                                          0x80
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_SREGR_MEM_CPH_ENABLE_SHFT                                             7
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_SREGR_FORCE_CLK_ON_BMSK                                            0x40
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_SREGR_FORCE_CLK_ON_SHFT                                               6
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_SREGR_SW_RST_SEL_SLP_STG_BMSK                                      0x20
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_SREGR_SW_RST_SEL_SLP_STG_SHFT                                         5
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_SREGR_SW_RST_SLP_STG_BMSK                                          0x10
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_SREGR_SW_RST_SLP_STG_SHFT                                             4
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_SREGR_SW_CTRL_PWR_DOWN_BMSK                                         0x8
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_SREGR_SW_CTRL_PWR_DOWN_SHFT                                           3
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_SREGR_SW_CLK_EN_SEL_SLP_STG_BMSK                                    0x4
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_SREGR_SW_CLK_EN_SEL_SLP_STG_SHFT                                      2
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_SREGR_SW_CLK_EN_SLP_STG_BMSK                                        0x2
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_SREGR_SW_CLK_EN_SLP_STG_SHFT                                          1

#define HWIO_MSS_CC_MODEM_TDEC_CORE_CBCR_ADDR                                                       (MSS_CC_MSS_CC_REG_REG_BASE            + 0x53c)
#define HWIO_MSS_CC_MODEM_TDEC_CORE_CBCR_RMSK                                                       0x81c07ff5
#define HWIO_MSS_CC_MODEM_TDEC_CORE_CBCR_IN                    \
                in_dword(HWIO_MSS_CC_MODEM_TDEC_CORE_CBCR_ADDR)
#define HWIO_MSS_CC_MODEM_TDEC_CORE_CBCR_INM(m)            \
                in_dword_masked(HWIO_MSS_CC_MODEM_TDEC_CORE_CBCR_ADDR, m)
#define HWIO_MSS_CC_MODEM_TDEC_CORE_CBCR_OUT(v)            \
                out_dword(HWIO_MSS_CC_MODEM_TDEC_CORE_CBCR_ADDR,v)
#define HWIO_MSS_CC_MODEM_TDEC_CORE_CBCR_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_CC_MODEM_TDEC_CORE_CBCR_ADDR,m,v,HWIO_MSS_CC_MODEM_TDEC_CORE_CBCR_IN)
#define HWIO_MSS_CC_MODEM_TDEC_CORE_CBCR_CLK_OFF_BMSK                                               0x80000000
#define HWIO_MSS_CC_MODEM_TDEC_CORE_CBCR_CLK_OFF_SHFT                                                       31
#define HWIO_MSS_CC_MODEM_TDEC_CORE_CBCR_IGNORE_ALL_ARES_BMSK                                        0x1000000
#define HWIO_MSS_CC_MODEM_TDEC_CORE_CBCR_IGNORE_ALL_ARES_SHFT                                               24
#define HWIO_MSS_CC_MODEM_TDEC_CORE_CBCR_IGNORE_ALL_CLK_DIS_BMSK                                      0x800000
#define HWIO_MSS_CC_MODEM_TDEC_CORE_CBCR_IGNORE_ALL_CLK_DIS_SHFT                                            23
#define HWIO_MSS_CC_MODEM_TDEC_CORE_CBCR_CLK_DIS_BMSK                                                 0x400000
#define HWIO_MSS_CC_MODEM_TDEC_CORE_CBCR_CLK_DIS_SHFT                                                       22
#define HWIO_MSS_CC_MODEM_TDEC_CORE_CBCR_FORCE_MEM_CORE_ON_BMSK                                         0x4000
#define HWIO_MSS_CC_MODEM_TDEC_CORE_CBCR_FORCE_MEM_CORE_ON_SHFT                                             14
#define HWIO_MSS_CC_MODEM_TDEC_CORE_CBCR_FORCE_MEM_PERIPH_ON_BMSK                                       0x2000
#define HWIO_MSS_CC_MODEM_TDEC_CORE_CBCR_FORCE_MEM_PERIPH_ON_SHFT                                           13
#define HWIO_MSS_CC_MODEM_TDEC_CORE_CBCR_FORCE_MEM_PERIPH_OFF_BMSK                                      0x1000
#define HWIO_MSS_CC_MODEM_TDEC_CORE_CBCR_FORCE_MEM_PERIPH_OFF_SHFT                                          12
#define HWIO_MSS_CC_MODEM_TDEC_CORE_CBCR_WAKEUP_BMSK                                                     0xf00
#define HWIO_MSS_CC_MODEM_TDEC_CORE_CBCR_WAKEUP_SHFT                                                         8
#define HWIO_MSS_CC_MODEM_TDEC_CORE_CBCR_SLEEP_BMSK                                                       0xf0
#define HWIO_MSS_CC_MODEM_TDEC_CORE_CBCR_SLEEP_SHFT                                                          4
#define HWIO_MSS_CC_MODEM_TDEC_CORE_CBCR_CLK_ARES_BMSK                                                     0x4
#define HWIO_MSS_CC_MODEM_TDEC_CORE_CBCR_CLK_ARES_SHFT                                                       2
#define HWIO_MSS_CC_MODEM_TDEC_CORE_CBCR_CLK_ENABLE_BMSK                                                   0x1
#define HWIO_MSS_CC_MODEM_TDEC_CORE_CBCR_CLK_ENABLE_SHFT                                                     0

#define HWIO_MSS_CC_MODEM_TDEC_CORE_SREGR_ADDR                                                      (MSS_CC_MSS_CC_REG_REG_BASE            + 0x540)
#define HWIO_MSS_CC_MODEM_TDEC_CORE_SREGR_RMSK                                                      0xfffffffe
#define HWIO_MSS_CC_MODEM_TDEC_CORE_SREGR_IN                    \
                in_dword(HWIO_MSS_CC_MODEM_TDEC_CORE_SREGR_ADDR)
#define HWIO_MSS_CC_MODEM_TDEC_CORE_SREGR_INM(m)            \
                in_dword_masked(HWIO_MSS_CC_MODEM_TDEC_CORE_SREGR_ADDR, m)
#define HWIO_MSS_CC_MODEM_TDEC_CORE_SREGR_OUT(v)            \
                out_dword(HWIO_MSS_CC_MODEM_TDEC_CORE_SREGR_ADDR,v)
#define HWIO_MSS_CC_MODEM_TDEC_CORE_SREGR_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_CC_MODEM_TDEC_CORE_SREGR_ADDR,m,v,HWIO_MSS_CC_MODEM_TDEC_CORE_SREGR_IN)
#define HWIO_MSS_CC_MODEM_TDEC_CORE_SREGR_SREG_PSCBC_SPARE_CTRL_OUT_BMSK                            0xff000000
#define HWIO_MSS_CC_MODEM_TDEC_CORE_SREGR_SREG_PSCBC_SPARE_CTRL_OUT_SHFT                                    24
#define HWIO_MSS_CC_MODEM_TDEC_CORE_SREGR_SREG_PSCBC_SPARE_CTRL_IN_BMSK                               0xff0000
#define HWIO_MSS_CC_MODEM_TDEC_CORE_SREGR_SREG_PSCBC_SPARE_CTRL_IN_SHFT                                     16
#define HWIO_MSS_CC_MODEM_TDEC_CORE_SREGR_IGNORE_GDSC_PWR_DWN_CSR_BMSK                                  0x8000
#define HWIO_MSS_CC_MODEM_TDEC_CORE_SREGR_IGNORE_GDSC_PWR_DWN_CSR_SHFT                                      15
#define HWIO_MSS_CC_MODEM_TDEC_CORE_SREGR_PSCBC_SLP_STG_MODE_CSR_BMSK                                   0x4000
#define HWIO_MSS_CC_MODEM_TDEC_CORE_SREGR_PSCBC_SLP_STG_MODE_CSR_SHFT                                       14
#define HWIO_MSS_CC_MODEM_TDEC_CORE_SREGR_MEM_CPH_RST_SW_OVERRIDE_BMSK                                  0x2000
#define HWIO_MSS_CC_MODEM_TDEC_CORE_SREGR_MEM_CPH_RST_SW_OVERRIDE_SHFT                                      13
#define HWIO_MSS_CC_MODEM_TDEC_CORE_SREGR_SW_SM_PSCBC_SEQ_IN_OVERRIDE_BMSK                              0x1000
#define HWIO_MSS_CC_MODEM_TDEC_CORE_SREGR_SW_SM_PSCBC_SEQ_IN_OVERRIDE_SHFT                                  12
#define HWIO_MSS_CC_MODEM_TDEC_CORE_SREGR_MEM_CORE_ON_ACK_BMSK                                           0x800
#define HWIO_MSS_CC_MODEM_TDEC_CORE_SREGR_MEM_CORE_ON_ACK_SHFT                                              11
#define HWIO_MSS_CC_MODEM_TDEC_CORE_SREGR_MEM_PERIPH_ON_ACK_BMSK                                         0x400
#define HWIO_MSS_CC_MODEM_TDEC_CORE_SREGR_MEM_PERIPH_ON_ACK_SHFT                                            10
#define HWIO_MSS_CC_MODEM_TDEC_CORE_SREGR_SW_DIV_RATIO_SLP_STG_CLK_BMSK                                  0x300
#define HWIO_MSS_CC_MODEM_TDEC_CORE_SREGR_SW_DIV_RATIO_SLP_STG_CLK_SHFT                                      8
#define HWIO_MSS_CC_MODEM_TDEC_CORE_SREGR_MEM_CPH_ENABLE_BMSK                                             0x80
#define HWIO_MSS_CC_MODEM_TDEC_CORE_SREGR_MEM_CPH_ENABLE_SHFT                                                7
#define HWIO_MSS_CC_MODEM_TDEC_CORE_SREGR_FORCE_CLK_ON_BMSK                                               0x40
#define HWIO_MSS_CC_MODEM_TDEC_CORE_SREGR_FORCE_CLK_ON_SHFT                                                  6
#define HWIO_MSS_CC_MODEM_TDEC_CORE_SREGR_SW_RST_SEL_SLP_STG_BMSK                                         0x20
#define HWIO_MSS_CC_MODEM_TDEC_CORE_SREGR_SW_RST_SEL_SLP_STG_SHFT                                            5
#define HWIO_MSS_CC_MODEM_TDEC_CORE_SREGR_SW_RST_SLP_STG_BMSK                                             0x10
#define HWIO_MSS_CC_MODEM_TDEC_CORE_SREGR_SW_RST_SLP_STG_SHFT                                                4
#define HWIO_MSS_CC_MODEM_TDEC_CORE_SREGR_SW_CTRL_PWR_DOWN_BMSK                                            0x8
#define HWIO_MSS_CC_MODEM_TDEC_CORE_SREGR_SW_CTRL_PWR_DOWN_SHFT                                              3
#define HWIO_MSS_CC_MODEM_TDEC_CORE_SREGR_SW_CLK_EN_SEL_SLP_STG_BMSK                                       0x4
#define HWIO_MSS_CC_MODEM_TDEC_CORE_SREGR_SW_CLK_EN_SEL_SLP_STG_SHFT                                         2
#define HWIO_MSS_CC_MODEM_TDEC_CORE_SREGR_SW_CLK_EN_SLP_STG_BMSK                                           0x2
#define HWIO_MSS_CC_MODEM_TDEC_CORE_SREGR_SW_CLK_EN_SLP_STG_SHFT                                             1

#define HWIO_MSS_CC_Q6_BCR_ADDR                                                                     (MSS_CC_MSS_CC_REG_REG_BASE            + 0x550)
#define HWIO_MSS_CC_Q6_BCR_RMSK                                                                            0x1
#define HWIO_MSS_CC_Q6_BCR_IN                    \
                in_dword(HWIO_MSS_CC_Q6_BCR_ADDR)
#define HWIO_MSS_CC_Q6_BCR_INM(m)            \
                in_dword_masked(HWIO_MSS_CC_Q6_BCR_ADDR, m)
#define HWIO_MSS_CC_Q6_BCR_OUT(v)            \
                out_dword(HWIO_MSS_CC_Q6_BCR_ADDR,v)
#define HWIO_MSS_CC_Q6_BCR_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_CC_Q6_BCR_ADDR,m,v,HWIO_MSS_CC_Q6_BCR_IN)
#define HWIO_MSS_CC_Q6_BCR_BLK_ARES_BMSK                                                                   0x1
#define HWIO_MSS_CC_Q6_BCR_BLK_ARES_SHFT                                                                     0

#define HWIO_MSS_CC_Q6_CMD_RCGR_ADDR                                                                (MSS_CC_MSS_CC_REG_REG_BASE            + 0x564)
#define HWIO_MSS_CC_Q6_CMD_RCGR_RMSK                                                                0x80000013
#define HWIO_MSS_CC_Q6_CMD_RCGR_IN                    \
                in_dword(HWIO_MSS_CC_Q6_CMD_RCGR_ADDR)
#define HWIO_MSS_CC_Q6_CMD_RCGR_INM(m)            \
                in_dword_masked(HWIO_MSS_CC_Q6_CMD_RCGR_ADDR, m)
#define HWIO_MSS_CC_Q6_CMD_RCGR_OUT(v)            \
                out_dword(HWIO_MSS_CC_Q6_CMD_RCGR_ADDR,v)
#define HWIO_MSS_CC_Q6_CMD_RCGR_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_CC_Q6_CMD_RCGR_ADDR,m,v,HWIO_MSS_CC_Q6_CMD_RCGR_IN)
#define HWIO_MSS_CC_Q6_CMD_RCGR_ROOT_OFF_BMSK                                                       0x80000000
#define HWIO_MSS_CC_Q6_CMD_RCGR_ROOT_OFF_SHFT                                                               31
#define HWIO_MSS_CC_Q6_CMD_RCGR_DIRTY_CFG_RCGR_BMSK                                                       0x10
#define HWIO_MSS_CC_Q6_CMD_RCGR_DIRTY_CFG_RCGR_SHFT                                                          4
#define HWIO_MSS_CC_Q6_CMD_RCGR_ROOT_EN_BMSK                                                               0x2
#define HWIO_MSS_CC_Q6_CMD_RCGR_ROOT_EN_SHFT                                                                 1
#define HWIO_MSS_CC_Q6_CMD_RCGR_UPDATE_BMSK                                                                0x1
#define HWIO_MSS_CC_Q6_CMD_RCGR_UPDATE_SHFT                                                                  0

#define HWIO_MSS_CC_Q6_CFG_RCGR_ADDR                                                                (MSS_CC_MSS_CC_REG_REG_BASE            + 0x568)
#define HWIO_MSS_CC_Q6_CFG_RCGR_RMSK                                                                  0x11071f
#define HWIO_MSS_CC_Q6_CFG_RCGR_IN                    \
                in_dword(HWIO_MSS_CC_Q6_CFG_RCGR_ADDR)
#define HWIO_MSS_CC_Q6_CFG_RCGR_INM(m)            \
                in_dword_masked(HWIO_MSS_CC_Q6_CFG_RCGR_ADDR, m)
#define HWIO_MSS_CC_Q6_CFG_RCGR_OUT(v)            \
                out_dword(HWIO_MSS_CC_Q6_CFG_RCGR_ADDR,v)
#define HWIO_MSS_CC_Q6_CFG_RCGR_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_CC_Q6_CFG_RCGR_ADDR,m,v,HWIO_MSS_CC_Q6_CFG_RCGR_IN)
#define HWIO_MSS_CC_Q6_CFG_RCGR_HW_CLK_CONTROL_BMSK                                                   0x100000
#define HWIO_MSS_CC_Q6_CFG_RCGR_HW_CLK_CONTROL_SHFT                                                         20
#define HWIO_MSS_CC_Q6_CFG_RCGR_RCGLITE_DISABLE_BMSK                                                   0x10000
#define HWIO_MSS_CC_Q6_CFG_RCGR_RCGLITE_DISABLE_SHFT                                                        16
#define HWIO_MSS_CC_Q6_CFG_RCGR_SRC_SEL_BMSK                                                             0x700
#define HWIO_MSS_CC_Q6_CFG_RCGR_SRC_SEL_SHFT                                                                 8
#define HWIO_MSS_CC_Q6_CFG_RCGR_SRC_DIV_BMSK                                                              0x1f
#define HWIO_MSS_CC_Q6_CFG_RCGR_SRC_DIV_SHFT                                                                 0

#define HWIO_MSS_CC_SILVER_CMD_RCGR_ADDR                                                            (MSS_CC_MSS_CC_REG_REG_BASE            + 0x578)
#define HWIO_MSS_CC_SILVER_CMD_RCGR_RMSK                                                            0x80000013
#define HWIO_MSS_CC_SILVER_CMD_RCGR_IN                    \
                in_dword(HWIO_MSS_CC_SILVER_CMD_RCGR_ADDR)
#define HWIO_MSS_CC_SILVER_CMD_RCGR_INM(m)            \
                in_dword_masked(HWIO_MSS_CC_SILVER_CMD_RCGR_ADDR, m)
#define HWIO_MSS_CC_SILVER_CMD_RCGR_OUT(v)            \
                out_dword(HWIO_MSS_CC_SILVER_CMD_RCGR_ADDR,v)
#define HWIO_MSS_CC_SILVER_CMD_RCGR_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_CC_SILVER_CMD_RCGR_ADDR,m,v,HWIO_MSS_CC_SILVER_CMD_RCGR_IN)
#define HWIO_MSS_CC_SILVER_CMD_RCGR_ROOT_OFF_BMSK                                                   0x80000000
#define HWIO_MSS_CC_SILVER_CMD_RCGR_ROOT_OFF_SHFT                                                           31
#define HWIO_MSS_CC_SILVER_CMD_RCGR_DIRTY_CFG_RCGR_BMSK                                                   0x10
#define HWIO_MSS_CC_SILVER_CMD_RCGR_DIRTY_CFG_RCGR_SHFT                                                      4
#define HWIO_MSS_CC_SILVER_CMD_RCGR_ROOT_EN_BMSK                                                           0x2
#define HWIO_MSS_CC_SILVER_CMD_RCGR_ROOT_EN_SHFT                                                             1
#define HWIO_MSS_CC_SILVER_CMD_RCGR_UPDATE_BMSK                                                            0x1
#define HWIO_MSS_CC_SILVER_CMD_RCGR_UPDATE_SHFT                                                              0

#define HWIO_MSS_CC_SILVER_CFG_RCGR_ADDR                                                            (MSS_CC_MSS_CC_REG_REG_BASE            + 0x57c)
#define HWIO_MSS_CC_SILVER_CFG_RCGR_RMSK                                                              0x11071f
#define HWIO_MSS_CC_SILVER_CFG_RCGR_IN                    \
                in_dword(HWIO_MSS_CC_SILVER_CFG_RCGR_ADDR)
#define HWIO_MSS_CC_SILVER_CFG_RCGR_INM(m)            \
                in_dword_masked(HWIO_MSS_CC_SILVER_CFG_RCGR_ADDR, m)
#define HWIO_MSS_CC_SILVER_CFG_RCGR_OUT(v)            \
                out_dword(HWIO_MSS_CC_SILVER_CFG_RCGR_ADDR,v)
#define HWIO_MSS_CC_SILVER_CFG_RCGR_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_CC_SILVER_CFG_RCGR_ADDR,m,v,HWIO_MSS_CC_SILVER_CFG_RCGR_IN)
#define HWIO_MSS_CC_SILVER_CFG_RCGR_HW_CLK_CONTROL_BMSK                                               0x100000
#define HWIO_MSS_CC_SILVER_CFG_RCGR_HW_CLK_CONTROL_SHFT                                                     20
#define HWIO_MSS_CC_SILVER_CFG_RCGR_RCGLITE_DISABLE_BMSK                                               0x10000
#define HWIO_MSS_CC_SILVER_CFG_RCGR_RCGLITE_DISABLE_SHFT                                                    16
#define HWIO_MSS_CC_SILVER_CFG_RCGR_SRC_SEL_BMSK                                                         0x700
#define HWIO_MSS_CC_SILVER_CFG_RCGR_SRC_SEL_SHFT                                                             8
#define HWIO_MSS_CC_SILVER_CFG_RCGR_SRC_DIV_BMSK                                                          0x1f
#define HWIO_MSS_CC_SILVER_CFG_RCGR_SRC_DIV_SHFT                                                             0

#define HWIO_MSS_CC_Q6_BUS_CBCR_ADDR                                                                (MSS_CC_MSS_CC_REG_REG_BASE            + 0x58c)
#define HWIO_MSS_CC_Q6_BUS_CBCR_RMSK                                                                0x81c00005
#define HWIO_MSS_CC_Q6_BUS_CBCR_IN                    \
                in_dword(HWIO_MSS_CC_Q6_BUS_CBCR_ADDR)
#define HWIO_MSS_CC_Q6_BUS_CBCR_INM(m)            \
                in_dword_masked(HWIO_MSS_CC_Q6_BUS_CBCR_ADDR, m)
#define HWIO_MSS_CC_Q6_BUS_CBCR_OUT(v)            \
                out_dword(HWIO_MSS_CC_Q6_BUS_CBCR_ADDR,v)
#define HWIO_MSS_CC_Q6_BUS_CBCR_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_CC_Q6_BUS_CBCR_ADDR,m,v,HWIO_MSS_CC_Q6_BUS_CBCR_IN)
#define HWIO_MSS_CC_Q6_BUS_CBCR_CLK_OFF_BMSK                                                        0x80000000
#define HWIO_MSS_CC_Q6_BUS_CBCR_CLK_OFF_SHFT                                                                31
#define HWIO_MSS_CC_Q6_BUS_CBCR_IGNORE_ALL_ARES_BMSK                                                 0x1000000
#define HWIO_MSS_CC_Q6_BUS_CBCR_IGNORE_ALL_ARES_SHFT                                                        24
#define HWIO_MSS_CC_Q6_BUS_CBCR_IGNORE_ALL_CLK_DIS_BMSK                                               0x800000
#define HWIO_MSS_CC_Q6_BUS_CBCR_IGNORE_ALL_CLK_DIS_SHFT                                                     23
#define HWIO_MSS_CC_Q6_BUS_CBCR_CLK_DIS_BMSK                                                          0x400000
#define HWIO_MSS_CC_Q6_BUS_CBCR_CLK_DIS_SHFT                                                                22
#define HWIO_MSS_CC_Q6_BUS_CBCR_CLK_ARES_BMSK                                                              0x4
#define HWIO_MSS_CC_Q6_BUS_CBCR_CLK_ARES_SHFT                                                                2
#define HWIO_MSS_CC_Q6_BUS_CBCR_CLK_ENABLE_BMSK                                                            0x1
#define HWIO_MSS_CC_Q6_BUS_CBCR_CLK_ENABLE_SHFT                                                              0

#define HWIO_MSS_CC_UIM_DIV_CDIVR_ADDR                                                              (MSS_CC_MSS_CC_REG_REG_BASE            + 0x654)
#define HWIO_MSS_CC_UIM_DIV_CDIVR_RMSK                                                                     0xf
#define HWIO_MSS_CC_UIM_DIV_CDIVR_IN                    \
                in_dword(HWIO_MSS_CC_UIM_DIV_CDIVR_ADDR)
#define HWIO_MSS_CC_UIM_DIV_CDIVR_INM(m)            \
                in_dword_masked(HWIO_MSS_CC_UIM_DIV_CDIVR_ADDR, m)
#define HWIO_MSS_CC_UIM_DIV_CDIVR_OUT(v)            \
                out_dword(HWIO_MSS_CC_UIM_DIV_CDIVR_ADDR,v)
#define HWIO_MSS_CC_UIM_DIV_CDIVR_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_CC_UIM_DIV_CDIVR_ADDR,m,v,HWIO_MSS_CC_UIM_DIV_CDIVR_IN)
#define HWIO_MSS_CC_UIM_DIV_CDIVR_CLK_DIV_BMSK                                                             0xf
#define HWIO_MSS_CC_UIM_DIV_CDIVR_CLK_DIV_SHFT                                                               0

#define HWIO_MSS_CC_UIM0_MND_CMD_RCGR_ADDR                                                          (MSS_CC_MSS_CC_REG_REG_BASE            + 0x67c)
#define HWIO_MSS_CC_UIM0_MND_CMD_RCGR_RMSK                                                          0x800000f3
#define HWIO_MSS_CC_UIM0_MND_CMD_RCGR_IN                    \
                in_dword(HWIO_MSS_CC_UIM0_MND_CMD_RCGR_ADDR)
#define HWIO_MSS_CC_UIM0_MND_CMD_RCGR_INM(m)            \
                in_dword_masked(HWIO_MSS_CC_UIM0_MND_CMD_RCGR_ADDR, m)
#define HWIO_MSS_CC_UIM0_MND_CMD_RCGR_OUT(v)            \
                out_dword(HWIO_MSS_CC_UIM0_MND_CMD_RCGR_ADDR,v)
#define HWIO_MSS_CC_UIM0_MND_CMD_RCGR_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_CC_UIM0_MND_CMD_RCGR_ADDR,m,v,HWIO_MSS_CC_UIM0_MND_CMD_RCGR_IN)
#define HWIO_MSS_CC_UIM0_MND_CMD_RCGR_ROOT_OFF_BMSK                                                 0x80000000
#define HWIO_MSS_CC_UIM0_MND_CMD_RCGR_ROOT_OFF_SHFT                                                         31
#define HWIO_MSS_CC_UIM0_MND_CMD_RCGR_DIRTY_D_BMSK                                                        0x80
#define HWIO_MSS_CC_UIM0_MND_CMD_RCGR_DIRTY_D_SHFT                                                           7
#define HWIO_MSS_CC_UIM0_MND_CMD_RCGR_DIRTY_N_BMSK                                                        0x40
#define HWIO_MSS_CC_UIM0_MND_CMD_RCGR_DIRTY_N_SHFT                                                           6
#define HWIO_MSS_CC_UIM0_MND_CMD_RCGR_DIRTY_M_BMSK                                                        0x20
#define HWIO_MSS_CC_UIM0_MND_CMD_RCGR_DIRTY_M_SHFT                                                           5
#define HWIO_MSS_CC_UIM0_MND_CMD_RCGR_DIRTY_CFG_RCGR_BMSK                                                 0x10
#define HWIO_MSS_CC_UIM0_MND_CMD_RCGR_DIRTY_CFG_RCGR_SHFT                                                    4
#define HWIO_MSS_CC_UIM0_MND_CMD_RCGR_ROOT_EN_BMSK                                                         0x2
#define HWIO_MSS_CC_UIM0_MND_CMD_RCGR_ROOT_EN_SHFT                                                           1
#define HWIO_MSS_CC_UIM0_MND_CMD_RCGR_UPDATE_BMSK                                                          0x1
#define HWIO_MSS_CC_UIM0_MND_CMD_RCGR_UPDATE_SHFT                                                            0

#define HWIO_MSS_CC_UIM0_MND_CFG_RCGR_ADDR                                                          (MSS_CC_MSS_CC_REG_REG_BASE            + 0x680)
#define HWIO_MSS_CC_UIM0_MND_CFG_RCGR_RMSK                                                            0x10371f
#define HWIO_MSS_CC_UIM0_MND_CFG_RCGR_IN                    \
                in_dword(HWIO_MSS_CC_UIM0_MND_CFG_RCGR_ADDR)
#define HWIO_MSS_CC_UIM0_MND_CFG_RCGR_INM(m)            \
                in_dword_masked(HWIO_MSS_CC_UIM0_MND_CFG_RCGR_ADDR, m)
#define HWIO_MSS_CC_UIM0_MND_CFG_RCGR_OUT(v)            \
                out_dword(HWIO_MSS_CC_UIM0_MND_CFG_RCGR_ADDR,v)
#define HWIO_MSS_CC_UIM0_MND_CFG_RCGR_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_CC_UIM0_MND_CFG_RCGR_ADDR,m,v,HWIO_MSS_CC_UIM0_MND_CFG_RCGR_IN)
#define HWIO_MSS_CC_UIM0_MND_CFG_RCGR_HW_CLK_CONTROL_BMSK                                             0x100000
#define HWIO_MSS_CC_UIM0_MND_CFG_RCGR_HW_CLK_CONTROL_SHFT                                                   20
#define HWIO_MSS_CC_UIM0_MND_CFG_RCGR_MODE_BMSK                                                         0x3000
#define HWIO_MSS_CC_UIM0_MND_CFG_RCGR_MODE_SHFT                                                             12
#define HWIO_MSS_CC_UIM0_MND_CFG_RCGR_SRC_SEL_BMSK                                                       0x700
#define HWIO_MSS_CC_UIM0_MND_CFG_RCGR_SRC_SEL_SHFT                                                           8
#define HWIO_MSS_CC_UIM0_MND_CFG_RCGR_SRC_DIV_BMSK                                                        0x1f
#define HWIO_MSS_CC_UIM0_MND_CFG_RCGR_SRC_DIV_SHFT                                                           0

#define HWIO_MSS_CC_UIM0_MND_M_ADDR                                                                 (MSS_CC_MSS_CC_REG_REG_BASE            + 0x684)
#define HWIO_MSS_CC_UIM0_MND_M_RMSK                                                                     0xffff
#define HWIO_MSS_CC_UIM0_MND_M_IN                    \
                in_dword(HWIO_MSS_CC_UIM0_MND_M_ADDR)
#define HWIO_MSS_CC_UIM0_MND_M_INM(m)            \
                in_dword_masked(HWIO_MSS_CC_UIM0_MND_M_ADDR, m)
#define HWIO_MSS_CC_UIM0_MND_M_OUT(v)            \
                out_dword(HWIO_MSS_CC_UIM0_MND_M_ADDR,v)
#define HWIO_MSS_CC_UIM0_MND_M_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_CC_UIM0_MND_M_ADDR,m,v,HWIO_MSS_CC_UIM0_MND_M_IN)
#define HWIO_MSS_CC_UIM0_MND_M_M_BMSK                                                                   0xffff
#define HWIO_MSS_CC_UIM0_MND_M_M_SHFT                                                                        0

#define HWIO_MSS_CC_UIM0_MND_N_ADDR                                                                 (MSS_CC_MSS_CC_REG_REG_BASE            + 0x688)
#define HWIO_MSS_CC_UIM0_MND_N_RMSK                                                                     0xffff
#define HWIO_MSS_CC_UIM0_MND_N_IN                    \
                in_dword(HWIO_MSS_CC_UIM0_MND_N_ADDR)
#define HWIO_MSS_CC_UIM0_MND_N_INM(m)            \
                in_dword_masked(HWIO_MSS_CC_UIM0_MND_N_ADDR, m)
#define HWIO_MSS_CC_UIM0_MND_N_OUT(v)            \
                out_dword(HWIO_MSS_CC_UIM0_MND_N_ADDR,v)
#define HWIO_MSS_CC_UIM0_MND_N_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_CC_UIM0_MND_N_ADDR,m,v,HWIO_MSS_CC_UIM0_MND_N_IN)
#define HWIO_MSS_CC_UIM0_MND_N_NOT_N_MINUS_M_BMSK                                                       0xffff
#define HWIO_MSS_CC_UIM0_MND_N_NOT_N_MINUS_M_SHFT                                                            0

#define HWIO_MSS_CC_UIM0_MND_D_ADDR                                                                 (MSS_CC_MSS_CC_REG_REG_BASE            + 0x68c)
#define HWIO_MSS_CC_UIM0_MND_D_RMSK                                                                     0xffff
#define HWIO_MSS_CC_UIM0_MND_D_IN                    \
                in_dword(HWIO_MSS_CC_UIM0_MND_D_ADDR)
#define HWIO_MSS_CC_UIM0_MND_D_INM(m)            \
                in_dword_masked(HWIO_MSS_CC_UIM0_MND_D_ADDR, m)
#define HWIO_MSS_CC_UIM0_MND_D_OUT(v)            \
                out_dword(HWIO_MSS_CC_UIM0_MND_D_ADDR,v)
#define HWIO_MSS_CC_UIM0_MND_D_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_CC_UIM0_MND_D_ADDR,m,v,HWIO_MSS_CC_UIM0_MND_D_IN)
#define HWIO_MSS_CC_UIM0_MND_D_NOT_2D_BMSK                                                              0xffff
#define HWIO_MSS_CC_UIM0_MND_D_NOT_2D_SHFT                                                                   0

#define HWIO_MSS_CC_UIM1_MND_CMD_RCGR_ADDR                                                          (MSS_CC_MSS_CC_REG_REG_BASE            + 0x690)
#define HWIO_MSS_CC_UIM1_MND_CMD_RCGR_RMSK                                                          0x800000f3
#define HWIO_MSS_CC_UIM1_MND_CMD_RCGR_IN                    \
                in_dword(HWIO_MSS_CC_UIM1_MND_CMD_RCGR_ADDR)
#define HWIO_MSS_CC_UIM1_MND_CMD_RCGR_INM(m)            \
                in_dword_masked(HWIO_MSS_CC_UIM1_MND_CMD_RCGR_ADDR, m)
#define HWIO_MSS_CC_UIM1_MND_CMD_RCGR_OUT(v)            \
                out_dword(HWIO_MSS_CC_UIM1_MND_CMD_RCGR_ADDR,v)
#define HWIO_MSS_CC_UIM1_MND_CMD_RCGR_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_CC_UIM1_MND_CMD_RCGR_ADDR,m,v,HWIO_MSS_CC_UIM1_MND_CMD_RCGR_IN)
#define HWIO_MSS_CC_UIM1_MND_CMD_RCGR_ROOT_OFF_BMSK                                                 0x80000000
#define HWIO_MSS_CC_UIM1_MND_CMD_RCGR_ROOT_OFF_SHFT                                                         31
#define HWIO_MSS_CC_UIM1_MND_CMD_RCGR_DIRTY_D_BMSK                                                        0x80
#define HWIO_MSS_CC_UIM1_MND_CMD_RCGR_DIRTY_D_SHFT                                                           7
#define HWIO_MSS_CC_UIM1_MND_CMD_RCGR_DIRTY_N_BMSK                                                        0x40
#define HWIO_MSS_CC_UIM1_MND_CMD_RCGR_DIRTY_N_SHFT                                                           6
#define HWIO_MSS_CC_UIM1_MND_CMD_RCGR_DIRTY_M_BMSK                                                        0x20
#define HWIO_MSS_CC_UIM1_MND_CMD_RCGR_DIRTY_M_SHFT                                                           5
#define HWIO_MSS_CC_UIM1_MND_CMD_RCGR_DIRTY_CFG_RCGR_BMSK                                                 0x10
#define HWIO_MSS_CC_UIM1_MND_CMD_RCGR_DIRTY_CFG_RCGR_SHFT                                                    4
#define HWIO_MSS_CC_UIM1_MND_CMD_RCGR_ROOT_EN_BMSK                                                         0x2
#define HWIO_MSS_CC_UIM1_MND_CMD_RCGR_ROOT_EN_SHFT                                                           1
#define HWIO_MSS_CC_UIM1_MND_CMD_RCGR_UPDATE_BMSK                                                          0x1
#define HWIO_MSS_CC_UIM1_MND_CMD_RCGR_UPDATE_SHFT                                                            0

#define HWIO_MSS_CC_UIM1_MND_CFG_RCGR_ADDR                                                          (MSS_CC_MSS_CC_REG_REG_BASE            + 0x694)
#define HWIO_MSS_CC_UIM1_MND_CFG_RCGR_RMSK                                                            0x10371f
#define HWIO_MSS_CC_UIM1_MND_CFG_RCGR_IN                    \
                in_dword(HWIO_MSS_CC_UIM1_MND_CFG_RCGR_ADDR)
#define HWIO_MSS_CC_UIM1_MND_CFG_RCGR_INM(m)            \
                in_dword_masked(HWIO_MSS_CC_UIM1_MND_CFG_RCGR_ADDR, m)
#define HWIO_MSS_CC_UIM1_MND_CFG_RCGR_OUT(v)            \
                out_dword(HWIO_MSS_CC_UIM1_MND_CFG_RCGR_ADDR,v)
#define HWIO_MSS_CC_UIM1_MND_CFG_RCGR_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_CC_UIM1_MND_CFG_RCGR_ADDR,m,v,HWIO_MSS_CC_UIM1_MND_CFG_RCGR_IN)
#define HWIO_MSS_CC_UIM1_MND_CFG_RCGR_HW_CLK_CONTROL_BMSK                                             0x100000
#define HWIO_MSS_CC_UIM1_MND_CFG_RCGR_HW_CLK_CONTROL_SHFT                                                   20
#define HWIO_MSS_CC_UIM1_MND_CFG_RCGR_MODE_BMSK                                                         0x3000
#define HWIO_MSS_CC_UIM1_MND_CFG_RCGR_MODE_SHFT                                                             12
#define HWIO_MSS_CC_UIM1_MND_CFG_RCGR_SRC_SEL_BMSK                                                       0x700
#define HWIO_MSS_CC_UIM1_MND_CFG_RCGR_SRC_SEL_SHFT                                                           8
#define HWIO_MSS_CC_UIM1_MND_CFG_RCGR_SRC_DIV_BMSK                                                        0x1f
#define HWIO_MSS_CC_UIM1_MND_CFG_RCGR_SRC_DIV_SHFT                                                           0

#define HWIO_MSS_CC_UIM1_MND_M_ADDR                                                                 (MSS_CC_MSS_CC_REG_REG_BASE            + 0x698)
#define HWIO_MSS_CC_UIM1_MND_M_RMSK                                                                     0xffff
#define HWIO_MSS_CC_UIM1_MND_M_IN                    \
                in_dword(HWIO_MSS_CC_UIM1_MND_M_ADDR)
#define HWIO_MSS_CC_UIM1_MND_M_INM(m)            \
                in_dword_masked(HWIO_MSS_CC_UIM1_MND_M_ADDR, m)
#define HWIO_MSS_CC_UIM1_MND_M_OUT(v)            \
                out_dword(HWIO_MSS_CC_UIM1_MND_M_ADDR,v)
#define HWIO_MSS_CC_UIM1_MND_M_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_CC_UIM1_MND_M_ADDR,m,v,HWIO_MSS_CC_UIM1_MND_M_IN)
#define HWIO_MSS_CC_UIM1_MND_M_M_BMSK                                                                   0xffff
#define HWIO_MSS_CC_UIM1_MND_M_M_SHFT                                                                        0

#define HWIO_MSS_CC_UIM1_MND_N_ADDR                                                                 (MSS_CC_MSS_CC_REG_REG_BASE            + 0x69c)
#define HWIO_MSS_CC_UIM1_MND_N_RMSK                                                                     0xffff
#define HWIO_MSS_CC_UIM1_MND_N_IN                    \
                in_dword(HWIO_MSS_CC_UIM1_MND_N_ADDR)
#define HWIO_MSS_CC_UIM1_MND_N_INM(m)            \
                in_dword_masked(HWIO_MSS_CC_UIM1_MND_N_ADDR, m)
#define HWIO_MSS_CC_UIM1_MND_N_OUT(v)            \
                out_dword(HWIO_MSS_CC_UIM1_MND_N_ADDR,v)
#define HWIO_MSS_CC_UIM1_MND_N_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_CC_UIM1_MND_N_ADDR,m,v,HWIO_MSS_CC_UIM1_MND_N_IN)
#define HWIO_MSS_CC_UIM1_MND_N_NOT_N_MINUS_M_BMSK                                                       0xffff
#define HWIO_MSS_CC_UIM1_MND_N_NOT_N_MINUS_M_SHFT                                                            0

#define HWIO_MSS_CC_UIM1_MND_D_ADDR                                                                 (MSS_CC_MSS_CC_REG_REG_BASE            + 0x6a0)
#define HWIO_MSS_CC_UIM1_MND_D_RMSK                                                                     0xffff
#define HWIO_MSS_CC_UIM1_MND_D_IN                    \
                in_dword(HWIO_MSS_CC_UIM1_MND_D_ADDR)
#define HWIO_MSS_CC_UIM1_MND_D_INM(m)            \
                in_dword_masked(HWIO_MSS_CC_UIM1_MND_D_ADDR, m)
#define HWIO_MSS_CC_UIM1_MND_D_OUT(v)            \
                out_dword(HWIO_MSS_CC_UIM1_MND_D_ADDR,v)
#define HWIO_MSS_CC_UIM1_MND_D_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_CC_UIM1_MND_D_ADDR,m,v,HWIO_MSS_CC_UIM1_MND_D_IN)
#define HWIO_MSS_CC_UIM1_MND_D_NOT_2D_BMSK                                                              0xffff
#define HWIO_MSS_CC_UIM1_MND_D_NOT_2D_SHFT                                                                   0

#define HWIO_MSS_CC_UIM0_BCR_ADDR                                                                   (MSS_CC_MSS_CC_REG_REG_BASE            + 0x6b8)
#define HWIO_MSS_CC_UIM0_BCR_RMSK                                                                          0x1
#define HWIO_MSS_CC_UIM0_BCR_IN                    \
                in_dword(HWIO_MSS_CC_UIM0_BCR_ADDR)
#define HWIO_MSS_CC_UIM0_BCR_INM(m)            \
                in_dword_masked(HWIO_MSS_CC_UIM0_BCR_ADDR, m)
#define HWIO_MSS_CC_UIM0_BCR_OUT(v)            \
                out_dword(HWIO_MSS_CC_UIM0_BCR_ADDR,v)
#define HWIO_MSS_CC_UIM0_BCR_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_CC_UIM0_BCR_ADDR,m,v,HWIO_MSS_CC_UIM0_BCR_IN)
#define HWIO_MSS_CC_UIM0_BCR_BLK_ARES_BMSK                                                                 0x1
#define HWIO_MSS_CC_UIM0_BCR_BLK_ARES_SHFT                                                                   0

#define HWIO_MSS_CC_UIM0_CBCR_ADDR                                                                  (MSS_CC_MSS_CC_REG_REG_BASE            + 0x6cc)
#define HWIO_MSS_CC_UIM0_CBCR_RMSK                                                                  0x81c00005
#define HWIO_MSS_CC_UIM0_CBCR_IN                    \
                in_dword(HWIO_MSS_CC_UIM0_CBCR_ADDR)
#define HWIO_MSS_CC_UIM0_CBCR_INM(m)            \
                in_dword_masked(HWIO_MSS_CC_UIM0_CBCR_ADDR, m)
#define HWIO_MSS_CC_UIM0_CBCR_OUT(v)            \
                out_dword(HWIO_MSS_CC_UIM0_CBCR_ADDR,v)
#define HWIO_MSS_CC_UIM0_CBCR_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_CC_UIM0_CBCR_ADDR,m,v,HWIO_MSS_CC_UIM0_CBCR_IN)
#define HWIO_MSS_CC_UIM0_CBCR_CLK_OFF_BMSK                                                          0x80000000
#define HWIO_MSS_CC_UIM0_CBCR_CLK_OFF_SHFT                                                                  31
#define HWIO_MSS_CC_UIM0_CBCR_IGNORE_ALL_ARES_BMSK                                                   0x1000000
#define HWIO_MSS_CC_UIM0_CBCR_IGNORE_ALL_ARES_SHFT                                                          24
#define HWIO_MSS_CC_UIM0_CBCR_IGNORE_ALL_CLK_DIS_BMSK                                                 0x800000
#define HWIO_MSS_CC_UIM0_CBCR_IGNORE_ALL_CLK_DIS_SHFT                                                       23
#define HWIO_MSS_CC_UIM0_CBCR_CLK_DIS_BMSK                                                            0x400000
#define HWIO_MSS_CC_UIM0_CBCR_CLK_DIS_SHFT                                                                  22
#define HWIO_MSS_CC_UIM0_CBCR_CLK_ARES_BMSK                                                                0x4
#define HWIO_MSS_CC_UIM0_CBCR_CLK_ARES_SHFT                                                                  2
#define HWIO_MSS_CC_UIM0_CBCR_CLK_ENABLE_BMSK                                                              0x1
#define HWIO_MSS_CC_UIM0_CBCR_CLK_ENABLE_SHFT                                                                0

#define HWIO_MSS_CC_UIM0_XO_CBCR_ADDR                                                               (MSS_CC_MSS_CC_REG_REG_BASE            + 0x6e0)
#define HWIO_MSS_CC_UIM0_XO_CBCR_RMSK                                                               0x81c00005
#define HWIO_MSS_CC_UIM0_XO_CBCR_IN                    \
                in_dword(HWIO_MSS_CC_UIM0_XO_CBCR_ADDR)
#define HWIO_MSS_CC_UIM0_XO_CBCR_INM(m)            \
                in_dword_masked(HWIO_MSS_CC_UIM0_XO_CBCR_ADDR, m)
#define HWIO_MSS_CC_UIM0_XO_CBCR_OUT(v)            \
                out_dword(HWIO_MSS_CC_UIM0_XO_CBCR_ADDR,v)
#define HWIO_MSS_CC_UIM0_XO_CBCR_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_CC_UIM0_XO_CBCR_ADDR,m,v,HWIO_MSS_CC_UIM0_XO_CBCR_IN)
#define HWIO_MSS_CC_UIM0_XO_CBCR_CLK_OFF_BMSK                                                       0x80000000
#define HWIO_MSS_CC_UIM0_XO_CBCR_CLK_OFF_SHFT                                                               31
#define HWIO_MSS_CC_UIM0_XO_CBCR_IGNORE_ALL_ARES_BMSK                                                0x1000000
#define HWIO_MSS_CC_UIM0_XO_CBCR_IGNORE_ALL_ARES_SHFT                                                       24
#define HWIO_MSS_CC_UIM0_XO_CBCR_IGNORE_ALL_CLK_DIS_BMSK                                              0x800000
#define HWIO_MSS_CC_UIM0_XO_CBCR_IGNORE_ALL_CLK_DIS_SHFT                                                    23
#define HWIO_MSS_CC_UIM0_XO_CBCR_CLK_DIS_BMSK                                                         0x400000
#define HWIO_MSS_CC_UIM0_XO_CBCR_CLK_DIS_SHFT                                                               22
#define HWIO_MSS_CC_UIM0_XO_CBCR_CLK_ARES_BMSK                                                             0x4
#define HWIO_MSS_CC_UIM0_XO_CBCR_CLK_ARES_SHFT                                                               2
#define HWIO_MSS_CC_UIM0_XO_CBCR_CLK_ENABLE_BMSK                                                           0x1
#define HWIO_MSS_CC_UIM0_XO_CBCR_CLK_ENABLE_SHFT                                                             0

#define HWIO_MSS_CC_UIM1_BCR_ADDR                                                                   (MSS_CC_MSS_CC_REG_REG_BASE            + 0x6f4)
#define HWIO_MSS_CC_UIM1_BCR_RMSK                                                                          0x1
#define HWIO_MSS_CC_UIM1_BCR_IN                    \
                in_dword(HWIO_MSS_CC_UIM1_BCR_ADDR)
#define HWIO_MSS_CC_UIM1_BCR_INM(m)            \
                in_dword_masked(HWIO_MSS_CC_UIM1_BCR_ADDR, m)
#define HWIO_MSS_CC_UIM1_BCR_OUT(v)            \
                out_dword(HWIO_MSS_CC_UIM1_BCR_ADDR,v)
#define HWIO_MSS_CC_UIM1_BCR_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_CC_UIM1_BCR_ADDR,m,v,HWIO_MSS_CC_UIM1_BCR_IN)
#define HWIO_MSS_CC_UIM1_BCR_BLK_ARES_BMSK                                                                 0x1
#define HWIO_MSS_CC_UIM1_BCR_BLK_ARES_SHFT                                                                   0

#define HWIO_MSS_CC_UIM1_CBCR_ADDR                                                                  (MSS_CC_MSS_CC_REG_REG_BASE            + 0x708)
#define HWIO_MSS_CC_UIM1_CBCR_RMSK                                                                  0x81c00005
#define HWIO_MSS_CC_UIM1_CBCR_IN                    \
                in_dword(HWIO_MSS_CC_UIM1_CBCR_ADDR)
#define HWIO_MSS_CC_UIM1_CBCR_INM(m)            \
                in_dword_masked(HWIO_MSS_CC_UIM1_CBCR_ADDR, m)
#define HWIO_MSS_CC_UIM1_CBCR_OUT(v)            \
                out_dword(HWIO_MSS_CC_UIM1_CBCR_ADDR,v)
#define HWIO_MSS_CC_UIM1_CBCR_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_CC_UIM1_CBCR_ADDR,m,v,HWIO_MSS_CC_UIM1_CBCR_IN)
#define HWIO_MSS_CC_UIM1_CBCR_CLK_OFF_BMSK                                                          0x80000000
#define HWIO_MSS_CC_UIM1_CBCR_CLK_OFF_SHFT                                                                  31
#define HWIO_MSS_CC_UIM1_CBCR_IGNORE_ALL_ARES_BMSK                                                   0x1000000
#define HWIO_MSS_CC_UIM1_CBCR_IGNORE_ALL_ARES_SHFT                                                          24
#define HWIO_MSS_CC_UIM1_CBCR_IGNORE_ALL_CLK_DIS_BMSK                                                 0x800000
#define HWIO_MSS_CC_UIM1_CBCR_IGNORE_ALL_CLK_DIS_SHFT                                                       23
#define HWIO_MSS_CC_UIM1_CBCR_CLK_DIS_BMSK                                                            0x400000
#define HWIO_MSS_CC_UIM1_CBCR_CLK_DIS_SHFT                                                                  22
#define HWIO_MSS_CC_UIM1_CBCR_CLK_ARES_BMSK                                                                0x4
#define HWIO_MSS_CC_UIM1_CBCR_CLK_ARES_SHFT                                                                  2
#define HWIO_MSS_CC_UIM1_CBCR_CLK_ENABLE_BMSK                                                              0x1
#define HWIO_MSS_CC_UIM1_CBCR_CLK_ENABLE_SHFT                                                                0

#define HWIO_MSS_CC_UIM1_XO_CBCR_ADDR                                                               (MSS_CC_MSS_CC_REG_REG_BASE            + 0x71c)
#define HWIO_MSS_CC_UIM1_XO_CBCR_RMSK                                                               0x81c00005
#define HWIO_MSS_CC_UIM1_XO_CBCR_IN                    \
                in_dword(HWIO_MSS_CC_UIM1_XO_CBCR_ADDR)
#define HWIO_MSS_CC_UIM1_XO_CBCR_INM(m)            \
                in_dword_masked(HWIO_MSS_CC_UIM1_XO_CBCR_ADDR, m)
#define HWIO_MSS_CC_UIM1_XO_CBCR_OUT(v)            \
                out_dword(HWIO_MSS_CC_UIM1_XO_CBCR_ADDR,v)
#define HWIO_MSS_CC_UIM1_XO_CBCR_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_CC_UIM1_XO_CBCR_ADDR,m,v,HWIO_MSS_CC_UIM1_XO_CBCR_IN)
#define HWIO_MSS_CC_UIM1_XO_CBCR_CLK_OFF_BMSK                                                       0x80000000
#define HWIO_MSS_CC_UIM1_XO_CBCR_CLK_OFF_SHFT                                                               31
#define HWIO_MSS_CC_UIM1_XO_CBCR_IGNORE_ALL_ARES_BMSK                                                0x1000000
#define HWIO_MSS_CC_UIM1_XO_CBCR_IGNORE_ALL_ARES_SHFT                                                       24
#define HWIO_MSS_CC_UIM1_XO_CBCR_IGNORE_ALL_CLK_DIS_BMSK                                              0x800000
#define HWIO_MSS_CC_UIM1_XO_CBCR_IGNORE_ALL_CLK_DIS_SHFT                                                    23
#define HWIO_MSS_CC_UIM1_XO_CBCR_CLK_DIS_BMSK                                                         0x400000
#define HWIO_MSS_CC_UIM1_XO_CBCR_CLK_DIS_SHFT                                                               22
#define HWIO_MSS_CC_UIM1_XO_CBCR_CLK_ARES_BMSK                                                             0x4
#define HWIO_MSS_CC_UIM1_XO_CBCR_CLK_ARES_SHFT                                                               2
#define HWIO_MSS_CC_UIM1_XO_CBCR_CLK_ENABLE_BMSK                                                           0x1
#define HWIO_MSS_CC_UIM1_XO_CBCR_CLK_ENABLE_SHFT                                                             0

#define HWIO_MSS_CC_COXM_BCR_ADDR                                                                   (MSS_CC_MSS_CC_REG_REG_BASE            + 0x76c)
#define HWIO_MSS_CC_COXM_BCR_RMSK                                                                          0x1
#define HWIO_MSS_CC_COXM_BCR_IN                    \
                in_dword(HWIO_MSS_CC_COXM_BCR_ADDR)
#define HWIO_MSS_CC_COXM_BCR_INM(m)            \
                in_dword_masked(HWIO_MSS_CC_COXM_BCR_ADDR, m)
#define HWIO_MSS_CC_COXM_BCR_OUT(v)            \
                out_dword(HWIO_MSS_CC_COXM_BCR_ADDR,v)
#define HWIO_MSS_CC_COXM_BCR_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_CC_COXM_BCR_ADDR,m,v,HWIO_MSS_CC_COXM_BCR_IN)
#define HWIO_MSS_CC_COXM_BCR_BLK_ARES_BMSK                                                                 0x1
#define HWIO_MSS_CC_COXM_BCR_BLK_ARES_SHFT                                                                   0

#define HWIO_MSS_CC_BIT_COXM_CMD_RCGR_ADDR                                                          (MSS_CC_MSS_CC_REG_REG_BASE            + 0x780)
#define HWIO_MSS_CC_BIT_COXM_CMD_RCGR_RMSK                                                          0x800000f3
#define HWIO_MSS_CC_BIT_COXM_CMD_RCGR_IN                    \
                in_dword(HWIO_MSS_CC_BIT_COXM_CMD_RCGR_ADDR)
#define HWIO_MSS_CC_BIT_COXM_CMD_RCGR_INM(m)            \
                in_dword_masked(HWIO_MSS_CC_BIT_COXM_CMD_RCGR_ADDR, m)
#define HWIO_MSS_CC_BIT_COXM_CMD_RCGR_OUT(v)            \
                out_dword(HWIO_MSS_CC_BIT_COXM_CMD_RCGR_ADDR,v)
#define HWIO_MSS_CC_BIT_COXM_CMD_RCGR_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_CC_BIT_COXM_CMD_RCGR_ADDR,m,v,HWIO_MSS_CC_BIT_COXM_CMD_RCGR_IN)
#define HWIO_MSS_CC_BIT_COXM_CMD_RCGR_ROOT_OFF_BMSK                                                 0x80000000
#define HWIO_MSS_CC_BIT_COXM_CMD_RCGR_ROOT_OFF_SHFT                                                         31
#define HWIO_MSS_CC_BIT_COXM_CMD_RCGR_DIRTY_D_BMSK                                                        0x80
#define HWIO_MSS_CC_BIT_COXM_CMD_RCGR_DIRTY_D_SHFT                                                           7
#define HWIO_MSS_CC_BIT_COXM_CMD_RCGR_DIRTY_N_BMSK                                                        0x40
#define HWIO_MSS_CC_BIT_COXM_CMD_RCGR_DIRTY_N_SHFT                                                           6
#define HWIO_MSS_CC_BIT_COXM_CMD_RCGR_DIRTY_M_BMSK                                                        0x20
#define HWIO_MSS_CC_BIT_COXM_CMD_RCGR_DIRTY_M_SHFT                                                           5
#define HWIO_MSS_CC_BIT_COXM_CMD_RCGR_DIRTY_CFG_RCGR_BMSK                                                 0x10
#define HWIO_MSS_CC_BIT_COXM_CMD_RCGR_DIRTY_CFG_RCGR_SHFT                                                    4
#define HWIO_MSS_CC_BIT_COXM_CMD_RCGR_ROOT_EN_BMSK                                                         0x2
#define HWIO_MSS_CC_BIT_COXM_CMD_RCGR_ROOT_EN_SHFT                                                           1
#define HWIO_MSS_CC_BIT_COXM_CMD_RCGR_UPDATE_BMSK                                                          0x1
#define HWIO_MSS_CC_BIT_COXM_CMD_RCGR_UPDATE_SHFT                                                            0

#define HWIO_MSS_CC_BIT_COXM_CFG_RCGR_ADDR                                                          (MSS_CC_MSS_CC_REG_REG_BASE            + 0x784)
#define HWIO_MSS_CC_BIT_COXM_CFG_RCGR_RMSK                                                            0x10371f
#define HWIO_MSS_CC_BIT_COXM_CFG_RCGR_IN                    \
                in_dword(HWIO_MSS_CC_BIT_COXM_CFG_RCGR_ADDR)
#define HWIO_MSS_CC_BIT_COXM_CFG_RCGR_INM(m)            \
                in_dword_masked(HWIO_MSS_CC_BIT_COXM_CFG_RCGR_ADDR, m)
#define HWIO_MSS_CC_BIT_COXM_CFG_RCGR_OUT(v)            \
                out_dword(HWIO_MSS_CC_BIT_COXM_CFG_RCGR_ADDR,v)
#define HWIO_MSS_CC_BIT_COXM_CFG_RCGR_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_CC_BIT_COXM_CFG_RCGR_ADDR,m,v,HWIO_MSS_CC_BIT_COXM_CFG_RCGR_IN)
#define HWIO_MSS_CC_BIT_COXM_CFG_RCGR_HW_CLK_CONTROL_BMSK                                             0x100000
#define HWIO_MSS_CC_BIT_COXM_CFG_RCGR_HW_CLK_CONTROL_SHFT                                                   20
#define HWIO_MSS_CC_BIT_COXM_CFG_RCGR_MODE_BMSK                                                         0x3000
#define HWIO_MSS_CC_BIT_COXM_CFG_RCGR_MODE_SHFT                                                             12
#define HWIO_MSS_CC_BIT_COXM_CFG_RCGR_SRC_SEL_BMSK                                                       0x700
#define HWIO_MSS_CC_BIT_COXM_CFG_RCGR_SRC_SEL_SHFT                                                           8
#define HWIO_MSS_CC_BIT_COXM_CFG_RCGR_SRC_DIV_BMSK                                                        0x1f
#define HWIO_MSS_CC_BIT_COXM_CFG_RCGR_SRC_DIV_SHFT                                                           0

#define HWIO_MSS_CC_BIT_COXM_M_ADDR                                                                 (MSS_CC_MSS_CC_REG_REG_BASE            + 0x788)
#define HWIO_MSS_CC_BIT_COXM_M_RMSK                                                                     0xffff
#define HWIO_MSS_CC_BIT_COXM_M_IN                    \
                in_dword(HWIO_MSS_CC_BIT_COXM_M_ADDR)
#define HWIO_MSS_CC_BIT_COXM_M_INM(m)            \
                in_dword_masked(HWIO_MSS_CC_BIT_COXM_M_ADDR, m)
#define HWIO_MSS_CC_BIT_COXM_M_OUT(v)            \
                out_dword(HWIO_MSS_CC_BIT_COXM_M_ADDR,v)
#define HWIO_MSS_CC_BIT_COXM_M_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_CC_BIT_COXM_M_ADDR,m,v,HWIO_MSS_CC_BIT_COXM_M_IN)
#define HWIO_MSS_CC_BIT_COXM_M_M_BMSK                                                                   0xffff
#define HWIO_MSS_CC_BIT_COXM_M_M_SHFT                                                                        0

#define HWIO_MSS_CC_BIT_COXM_N_ADDR                                                                 (MSS_CC_MSS_CC_REG_REG_BASE            + 0x78c)
#define HWIO_MSS_CC_BIT_COXM_N_RMSK                                                                     0xffff
#define HWIO_MSS_CC_BIT_COXM_N_IN                    \
                in_dword(HWIO_MSS_CC_BIT_COXM_N_ADDR)
#define HWIO_MSS_CC_BIT_COXM_N_INM(m)            \
                in_dword_masked(HWIO_MSS_CC_BIT_COXM_N_ADDR, m)
#define HWIO_MSS_CC_BIT_COXM_N_OUT(v)            \
                out_dword(HWIO_MSS_CC_BIT_COXM_N_ADDR,v)
#define HWIO_MSS_CC_BIT_COXM_N_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_CC_BIT_COXM_N_ADDR,m,v,HWIO_MSS_CC_BIT_COXM_N_IN)
#define HWIO_MSS_CC_BIT_COXM_N_NOT_N_MINUS_M_BMSK                                                       0xffff
#define HWIO_MSS_CC_BIT_COXM_N_NOT_N_MINUS_M_SHFT                                                            0

#define HWIO_MSS_CC_BIT_COXM_D_ADDR                                                                 (MSS_CC_MSS_CC_REG_REG_BASE            + 0x790)
#define HWIO_MSS_CC_BIT_COXM_D_RMSK                                                                     0xffff
#define HWIO_MSS_CC_BIT_COXM_D_IN                    \
                in_dword(HWIO_MSS_CC_BIT_COXM_D_ADDR)
#define HWIO_MSS_CC_BIT_COXM_D_INM(m)            \
                in_dword_masked(HWIO_MSS_CC_BIT_COXM_D_ADDR, m)
#define HWIO_MSS_CC_BIT_COXM_D_OUT(v)            \
                out_dword(HWIO_MSS_CC_BIT_COXM_D_ADDR,v)
#define HWIO_MSS_CC_BIT_COXM_D_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_CC_BIT_COXM_D_ADDR,m,v,HWIO_MSS_CC_BIT_COXM_D_IN)
#define HWIO_MSS_CC_BIT_COXM_D_NOT_2D_BMSK                                                              0xffff
#define HWIO_MSS_CC_BIT_COXM_D_NOT_2D_SHFT                                                                   0

#define HWIO_MSS_CC_BIT_COXM_CBCR_ADDR                                                              (MSS_CC_MSS_CC_REG_REG_BASE            + 0x794)
#define HWIO_MSS_CC_BIT_COXM_CBCR_RMSK                                                              0x81c00005
#define HWIO_MSS_CC_BIT_COXM_CBCR_IN                    \
                in_dword(HWIO_MSS_CC_BIT_COXM_CBCR_ADDR)
#define HWIO_MSS_CC_BIT_COXM_CBCR_INM(m)            \
                in_dword_masked(HWIO_MSS_CC_BIT_COXM_CBCR_ADDR, m)
#define HWIO_MSS_CC_BIT_COXM_CBCR_OUT(v)            \
                out_dword(HWIO_MSS_CC_BIT_COXM_CBCR_ADDR,v)
#define HWIO_MSS_CC_BIT_COXM_CBCR_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_CC_BIT_COXM_CBCR_ADDR,m,v,HWIO_MSS_CC_BIT_COXM_CBCR_IN)
#define HWIO_MSS_CC_BIT_COXM_CBCR_CLK_OFF_BMSK                                                      0x80000000
#define HWIO_MSS_CC_BIT_COXM_CBCR_CLK_OFF_SHFT                                                              31
#define HWIO_MSS_CC_BIT_COXM_CBCR_IGNORE_ALL_ARES_BMSK                                               0x1000000
#define HWIO_MSS_CC_BIT_COXM_CBCR_IGNORE_ALL_ARES_SHFT                                                      24
#define HWIO_MSS_CC_BIT_COXM_CBCR_IGNORE_ALL_CLK_DIS_BMSK                                             0x800000
#define HWIO_MSS_CC_BIT_COXM_CBCR_IGNORE_ALL_CLK_DIS_SHFT                                                   23
#define HWIO_MSS_CC_BIT_COXM_CBCR_CLK_DIS_BMSK                                                        0x400000
#define HWIO_MSS_CC_BIT_COXM_CBCR_CLK_DIS_SHFT                                                              22
#define HWIO_MSS_CC_BIT_COXM_CBCR_CLK_ARES_BMSK                                                            0x4
#define HWIO_MSS_CC_BIT_COXM_CBCR_CLK_ARES_SHFT                                                              2
#define HWIO_MSS_CC_BIT_COXM_CBCR_CLK_ENABLE_BMSK                                                          0x1
#define HWIO_MSS_CC_BIT_COXM_CBCR_CLK_ENABLE_SHFT                                                            0

#define HWIO_MSS_CC_MODEM_TDEC_CORE_BCR_ADDR                                                        (MSS_CC_MSS_CC_REG_REG_BASE            + 0x85c)
#define HWIO_MSS_CC_MODEM_TDEC_CORE_BCR_RMSK                                                               0x1
#define HWIO_MSS_CC_MODEM_TDEC_CORE_BCR_IN                    \
                in_dword(HWIO_MSS_CC_MODEM_TDEC_CORE_BCR_ADDR)
#define HWIO_MSS_CC_MODEM_TDEC_CORE_BCR_INM(m)            \
                in_dword_masked(HWIO_MSS_CC_MODEM_TDEC_CORE_BCR_ADDR, m)
#define HWIO_MSS_CC_MODEM_TDEC_CORE_BCR_OUT(v)            \
                out_dword(HWIO_MSS_CC_MODEM_TDEC_CORE_BCR_ADDR,v)
#define HWIO_MSS_CC_MODEM_TDEC_CORE_BCR_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_CC_MODEM_TDEC_CORE_BCR_ADDR,m,v,HWIO_MSS_CC_MODEM_TDEC_CORE_BCR_IN)
#define HWIO_MSS_CC_MODEM_TDEC_CORE_BCR_BLK_ARES_BMSK                                                      0x1
#define HWIO_MSS_CC_MODEM_TDEC_CORE_BCR_BLK_ARES_SHFT                                                        0

#define HWIO_MSS_CC_MODEM_TDEC_CORE_GDSCR_ADDR                                                      (MSS_CC_MSS_CC_REG_REG_BASE            + 0x860)
#define HWIO_MSS_CC_MODEM_TDEC_CORE_GDSCR_RMSK                                                      0xf8ffffff
#define HWIO_MSS_CC_MODEM_TDEC_CORE_GDSCR_IN                    \
                in_dword(HWIO_MSS_CC_MODEM_TDEC_CORE_GDSCR_ADDR)
#define HWIO_MSS_CC_MODEM_TDEC_CORE_GDSCR_INM(m)            \
                in_dword_masked(HWIO_MSS_CC_MODEM_TDEC_CORE_GDSCR_ADDR, m)
#define HWIO_MSS_CC_MODEM_TDEC_CORE_GDSCR_OUT(v)            \
                out_dword(HWIO_MSS_CC_MODEM_TDEC_CORE_GDSCR_ADDR,v)
#define HWIO_MSS_CC_MODEM_TDEC_CORE_GDSCR_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_CC_MODEM_TDEC_CORE_GDSCR_ADDR,m,v,HWIO_MSS_CC_MODEM_TDEC_CORE_GDSCR_IN)
#define HWIO_MSS_CC_MODEM_TDEC_CORE_GDSCR_PWR_ON_BMSK                                               0x80000000
#define HWIO_MSS_CC_MODEM_TDEC_CORE_GDSCR_PWR_ON_SHFT                                                       31
#define HWIO_MSS_CC_MODEM_TDEC_CORE_GDSCR_GDSC_STATE_BMSK                                           0x78000000
#define HWIO_MSS_CC_MODEM_TDEC_CORE_GDSCR_GDSC_STATE_SHFT                                                   27
#define HWIO_MSS_CC_MODEM_TDEC_CORE_GDSCR_EN_REST_WAIT_BMSK                                           0xf00000
#define HWIO_MSS_CC_MODEM_TDEC_CORE_GDSCR_EN_REST_WAIT_SHFT                                                 20
#define HWIO_MSS_CC_MODEM_TDEC_CORE_GDSCR_EN_FEW_WAIT_BMSK                                             0xf0000
#define HWIO_MSS_CC_MODEM_TDEC_CORE_GDSCR_EN_FEW_WAIT_SHFT                                                  16
#define HWIO_MSS_CC_MODEM_TDEC_CORE_GDSCR_CLK_DIS_WAIT_BMSK                                             0xf000
#define HWIO_MSS_CC_MODEM_TDEC_CORE_GDSCR_CLK_DIS_WAIT_SHFT                                                 12
#define HWIO_MSS_CC_MODEM_TDEC_CORE_GDSCR_RETAIN_FF_ENABLE_BMSK                                          0x800
#define HWIO_MSS_CC_MODEM_TDEC_CORE_GDSCR_RETAIN_FF_ENABLE_SHFT                                             11
#define HWIO_MSS_CC_MODEM_TDEC_CORE_GDSCR_RESTORE_BMSK                                                   0x400
#define HWIO_MSS_CC_MODEM_TDEC_CORE_GDSCR_RESTORE_SHFT                                                      10
#define HWIO_MSS_CC_MODEM_TDEC_CORE_GDSCR_SAVE_BMSK                                                      0x200
#define HWIO_MSS_CC_MODEM_TDEC_CORE_GDSCR_SAVE_SHFT                                                          9
#define HWIO_MSS_CC_MODEM_TDEC_CORE_GDSCR_RETAIN_BMSK                                                    0x100
#define HWIO_MSS_CC_MODEM_TDEC_CORE_GDSCR_RETAIN_SHFT                                                        8
#define HWIO_MSS_CC_MODEM_TDEC_CORE_GDSCR_EN_REST_BMSK                                                    0x80
#define HWIO_MSS_CC_MODEM_TDEC_CORE_GDSCR_EN_REST_SHFT                                                       7
#define HWIO_MSS_CC_MODEM_TDEC_CORE_GDSCR_EN_FEW_BMSK                                                     0x40
#define HWIO_MSS_CC_MODEM_TDEC_CORE_GDSCR_EN_FEW_SHFT                                                        6
#define HWIO_MSS_CC_MODEM_TDEC_CORE_GDSCR_CLAMP_IO_BMSK                                                   0x20
#define HWIO_MSS_CC_MODEM_TDEC_CORE_GDSCR_CLAMP_IO_SHFT                                                      5
#define HWIO_MSS_CC_MODEM_TDEC_CORE_GDSCR_CLK_DISABLE_BMSK                                                0x10
#define HWIO_MSS_CC_MODEM_TDEC_CORE_GDSCR_CLK_DISABLE_SHFT                                                   4
#define HWIO_MSS_CC_MODEM_TDEC_CORE_GDSCR_PD_ARES_BMSK                                                     0x8
#define HWIO_MSS_CC_MODEM_TDEC_CORE_GDSCR_PD_ARES_SHFT                                                       3
#define HWIO_MSS_CC_MODEM_TDEC_CORE_GDSCR_SW_OVERRIDE_BMSK                                                 0x4
#define HWIO_MSS_CC_MODEM_TDEC_CORE_GDSCR_SW_OVERRIDE_SHFT                                                   2
#define HWIO_MSS_CC_MODEM_TDEC_CORE_GDSCR_HW_CONTROL_BMSK                                                  0x2
#define HWIO_MSS_CC_MODEM_TDEC_CORE_GDSCR_HW_CONTROL_SHFT                                                    1
#define HWIO_MSS_CC_MODEM_TDEC_CORE_GDSCR_SW_COLLAPSE_BMSK                                                 0x1
#define HWIO_MSS_CC_MODEM_TDEC_CORE_GDSCR_SW_COLLAPSE_SHFT                                                   0

#define HWIO_MSS_CC_MODEM_TDEC_CORE_CFG_GDSCR_ADDR                                                  (MSS_CC_MSS_CC_REG_REG_BASE            + 0x864)
#define HWIO_MSS_CC_MODEM_TDEC_CORE_CFG_GDSCR_RMSK                                                   0x3ffffff
#define HWIO_MSS_CC_MODEM_TDEC_CORE_CFG_GDSCR_IN                    \
                in_dword(HWIO_MSS_CC_MODEM_TDEC_CORE_CFG_GDSCR_ADDR)
#define HWIO_MSS_CC_MODEM_TDEC_CORE_CFG_GDSCR_INM(m)            \
                in_dword_masked(HWIO_MSS_CC_MODEM_TDEC_CORE_CFG_GDSCR_ADDR, m)
#define HWIO_MSS_CC_MODEM_TDEC_CORE_CFG_GDSCR_OUT(v)            \
                out_dword(HWIO_MSS_CC_MODEM_TDEC_CORE_CFG_GDSCR_ADDR,v)
#define HWIO_MSS_CC_MODEM_TDEC_CORE_CFG_GDSCR_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_CC_MODEM_TDEC_CORE_CFG_GDSCR_ADDR,m,v,HWIO_MSS_CC_MODEM_TDEC_CORE_CFG_GDSCR_IN)
#define HWIO_MSS_CC_MODEM_TDEC_CORE_CFG_GDSCR_GDSC_PWR_DWN_START_BMSK                                0x2000000
#define HWIO_MSS_CC_MODEM_TDEC_CORE_CFG_GDSCR_GDSC_PWR_DWN_START_SHFT                                       25
#define HWIO_MSS_CC_MODEM_TDEC_CORE_CFG_GDSCR_GDSC_PWR_UP_START_BMSK                                 0x1000000
#define HWIO_MSS_CC_MODEM_TDEC_CORE_CFG_GDSCR_GDSC_PWR_UP_START_SHFT                                        24
#define HWIO_MSS_CC_MODEM_TDEC_CORE_CFG_GDSCR_GDSC_CFG_FSM_STATE_STATUS_BMSK                          0xf00000
#define HWIO_MSS_CC_MODEM_TDEC_CORE_CFG_GDSCR_GDSC_CFG_FSM_STATE_STATUS_SHFT                                20
#define HWIO_MSS_CC_MODEM_TDEC_CORE_CFG_GDSCR_GDSC_MEM_PWR_ACK_STATUS_BMSK                             0x80000
#define HWIO_MSS_CC_MODEM_TDEC_CORE_CFG_GDSCR_GDSC_MEM_PWR_ACK_STATUS_SHFT                                  19
#define HWIO_MSS_CC_MODEM_TDEC_CORE_CFG_GDSCR_GDSC_ENR_ACK_STATUS_BMSK                                 0x40000
#define HWIO_MSS_CC_MODEM_TDEC_CORE_CFG_GDSCR_GDSC_ENR_ACK_STATUS_SHFT                                      18
#define HWIO_MSS_CC_MODEM_TDEC_CORE_CFG_GDSCR_GDSC_ENF_ACK_STATUS_BMSK                                 0x20000
#define HWIO_MSS_CC_MODEM_TDEC_CORE_CFG_GDSCR_GDSC_ENF_ACK_STATUS_SHFT                                      17
#define HWIO_MSS_CC_MODEM_TDEC_CORE_CFG_GDSCR_GDSC_POWER_UP_COMPLETE_BMSK                              0x10000
#define HWIO_MSS_CC_MODEM_TDEC_CORE_CFG_GDSCR_GDSC_POWER_UP_COMPLETE_SHFT                                   16
#define HWIO_MSS_CC_MODEM_TDEC_CORE_CFG_GDSCR_GDSC_POWER_DOWN_COMPLETE_BMSK                             0x8000
#define HWIO_MSS_CC_MODEM_TDEC_CORE_CFG_GDSCR_GDSC_POWER_DOWN_COMPLETE_SHFT                                 15
#define HWIO_MSS_CC_MODEM_TDEC_CORE_CFG_GDSCR_SOFTWARE_CONTROL_OVERRIDE_BMSK                            0x7800
#define HWIO_MSS_CC_MODEM_TDEC_CORE_CFG_GDSCR_SOFTWARE_CONTROL_OVERRIDE_SHFT                                11
#define HWIO_MSS_CC_MODEM_TDEC_CORE_CFG_GDSCR_GDSC_HANDSHAKE_DIS_BMSK                                    0x400
#define HWIO_MSS_CC_MODEM_TDEC_CORE_CFG_GDSCR_GDSC_HANDSHAKE_DIS_SHFT                                       10
#define HWIO_MSS_CC_MODEM_TDEC_CORE_CFG_GDSCR_GDSC_MEM_PERI_FORCE_IN_SW_BMSK                             0x200
#define HWIO_MSS_CC_MODEM_TDEC_CORE_CFG_GDSCR_GDSC_MEM_PERI_FORCE_IN_SW_SHFT                                 9
#define HWIO_MSS_CC_MODEM_TDEC_CORE_CFG_GDSCR_GDSC_MEM_CORE_FORCE_IN_SW_BMSK                             0x100
#define HWIO_MSS_CC_MODEM_TDEC_CORE_CFG_GDSCR_GDSC_MEM_CORE_FORCE_IN_SW_SHFT                                 8
#define HWIO_MSS_CC_MODEM_TDEC_CORE_CFG_GDSCR_GDSC_PHASE_RESET_EN_SW_BMSK                                 0x80
#define HWIO_MSS_CC_MODEM_TDEC_CORE_CFG_GDSCR_GDSC_PHASE_RESET_EN_SW_SHFT                                    7
#define HWIO_MSS_CC_MODEM_TDEC_CORE_CFG_GDSCR_GDSC_PHASE_RESET_DELAY_COUNT_SW_BMSK                        0x60
#define HWIO_MSS_CC_MODEM_TDEC_CORE_CFG_GDSCR_GDSC_PHASE_RESET_DELAY_COUNT_SW_SHFT                           5
#define HWIO_MSS_CC_MODEM_TDEC_CORE_CFG_GDSCR_GDSC_PSCBC_PWR_DWN_SW_BMSK                                  0x10
#define HWIO_MSS_CC_MODEM_TDEC_CORE_CFG_GDSCR_GDSC_PSCBC_PWR_DWN_SW_SHFT                                     4
#define HWIO_MSS_CC_MODEM_TDEC_CORE_CFG_GDSCR_UNCLAMP_IO_SOFTWARE_OVERRIDE_BMSK                            0x8
#define HWIO_MSS_CC_MODEM_TDEC_CORE_CFG_GDSCR_UNCLAMP_IO_SOFTWARE_OVERRIDE_SHFT                              3
#define HWIO_MSS_CC_MODEM_TDEC_CORE_CFG_GDSCR_SAVE_RESTORE_SOFTWARE_OVERRIDE_BMSK                          0x4
#define HWIO_MSS_CC_MODEM_TDEC_CORE_CFG_GDSCR_SAVE_RESTORE_SOFTWARE_OVERRIDE_SHFT                            2
#define HWIO_MSS_CC_MODEM_TDEC_CORE_CFG_GDSCR_CLAMP_IO_SOFTWARE_OVERRIDE_BMSK                              0x2
#define HWIO_MSS_CC_MODEM_TDEC_CORE_CFG_GDSCR_CLAMP_IO_SOFTWARE_OVERRIDE_SHFT                                1
#define HWIO_MSS_CC_MODEM_TDEC_CORE_CFG_GDSCR_DISABLE_CLK_SOFTWARE_OVERRIDE_BMSK                           0x1
#define HWIO_MSS_CC_MODEM_TDEC_CORE_CFG_GDSCR_DISABLE_CLK_SOFTWARE_OVERRIDE_SHFT                             0

#define HWIO_MSS_CC_MODEM_TDEC_CORE_CFG2_GDSCR_ADDR                                                 (MSS_CC_MSS_CC_REG_REG_BASE            + 0x868)
#define HWIO_MSS_CC_MODEM_TDEC_CORE_CFG2_GDSCR_RMSK                                                    0x7ffff
#define HWIO_MSS_CC_MODEM_TDEC_CORE_CFG2_GDSCR_IN                    \
                in_dword(HWIO_MSS_CC_MODEM_TDEC_CORE_CFG2_GDSCR_ADDR)
#define HWIO_MSS_CC_MODEM_TDEC_CORE_CFG2_GDSCR_INM(m)            \
                in_dword_masked(HWIO_MSS_CC_MODEM_TDEC_CORE_CFG2_GDSCR_ADDR, m)
#define HWIO_MSS_CC_MODEM_TDEC_CORE_CFG2_GDSCR_OUT(v)            \
                out_dword(HWIO_MSS_CC_MODEM_TDEC_CORE_CFG2_GDSCR_ADDR,v)
#define HWIO_MSS_CC_MODEM_TDEC_CORE_CFG2_GDSCR_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_CC_MODEM_TDEC_CORE_CFG2_GDSCR_ADDR,m,v,HWIO_MSS_CC_MODEM_TDEC_CORE_CFG2_GDSCR_IN)
#define HWIO_MSS_CC_MODEM_TDEC_CORE_CFG2_GDSCR_GDSC_MEM_PWRUP_ACK_OVERRIDE_BMSK                        0x40000
#define HWIO_MSS_CC_MODEM_TDEC_CORE_CFG2_GDSCR_GDSC_MEM_PWRUP_ACK_OVERRIDE_SHFT                             18
#define HWIO_MSS_CC_MODEM_TDEC_CORE_CFG2_GDSCR_GDSC_PWRDWN_ENABLE_ACK_OVERRIDE_BMSK                    0x20000
#define HWIO_MSS_CC_MODEM_TDEC_CORE_CFG2_GDSCR_GDSC_PWRDWN_ENABLE_ACK_OVERRIDE_SHFT                         17
#define HWIO_MSS_CC_MODEM_TDEC_CORE_CFG2_GDSCR_GDSC_CLAMP_MEM_SW_BMSK                                  0x10000
#define HWIO_MSS_CC_MODEM_TDEC_CORE_CFG2_GDSCR_GDSC_CLAMP_MEM_SW_SHFT                                       16
#define HWIO_MSS_CC_MODEM_TDEC_CORE_CFG2_GDSCR_DLY_MEM_PWR_UP_BMSK                                      0xf000
#define HWIO_MSS_CC_MODEM_TDEC_CORE_CFG2_GDSCR_DLY_MEM_PWR_UP_SHFT                                          12
#define HWIO_MSS_CC_MODEM_TDEC_CORE_CFG2_GDSCR_DLY_DEASSERT_CLAMP_MEM_BMSK                               0xf00
#define HWIO_MSS_CC_MODEM_TDEC_CORE_CFG2_GDSCR_DLY_DEASSERT_CLAMP_MEM_SHFT                                   8
#define HWIO_MSS_CC_MODEM_TDEC_CORE_CFG2_GDSCR_DLY_ASSERT_CLAMP_MEM_BMSK                                  0xf0
#define HWIO_MSS_CC_MODEM_TDEC_CORE_CFG2_GDSCR_DLY_ASSERT_CLAMP_MEM_SHFT                                     4
#define HWIO_MSS_CC_MODEM_TDEC_CORE_CFG2_GDSCR_MEM_PWR_DWN_TIMEOUT_BMSK                                    0xf
#define HWIO_MSS_CC_MODEM_TDEC_CORE_CFG2_GDSCR_MEM_PWR_DWN_TIMEOUT_SHFT                                      0

#define HWIO_MSS_CC_MODEM_TDEC_CORE_CFG3_GDSCR_ADDR                                                 (MSS_CC_MSS_CC_REG_REG_BASE            + 0x86c)
#define HWIO_MSS_CC_MODEM_TDEC_CORE_CFG3_GDSCR_RMSK                                                  0x7ffffff
#define HWIO_MSS_CC_MODEM_TDEC_CORE_CFG3_GDSCR_IN                    \
                in_dword(HWIO_MSS_CC_MODEM_TDEC_CORE_CFG3_GDSCR_ADDR)
#define HWIO_MSS_CC_MODEM_TDEC_CORE_CFG3_GDSCR_INM(m)            \
                in_dword_masked(HWIO_MSS_CC_MODEM_TDEC_CORE_CFG3_GDSCR_ADDR, m)
#define HWIO_MSS_CC_MODEM_TDEC_CORE_CFG3_GDSCR_OUT(v)            \
                out_dword(HWIO_MSS_CC_MODEM_TDEC_CORE_CFG3_GDSCR_ADDR,v)
#define HWIO_MSS_CC_MODEM_TDEC_CORE_CFG3_GDSCR_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_CC_MODEM_TDEC_CORE_CFG3_GDSCR_ADDR,m,v,HWIO_MSS_CC_MODEM_TDEC_CORE_CFG3_GDSCR_IN)
#define HWIO_MSS_CC_MODEM_TDEC_CORE_CFG3_GDSCR_GDSC_ACCU_RED_SHIFTER_DONE_STATUS_BMSK                0x4000000
#define HWIO_MSS_CC_MODEM_TDEC_CORE_CFG3_GDSCR_GDSC_ACCU_RED_SHIFTER_DONE_STATUS_SHFT                       26
#define HWIO_MSS_CC_MODEM_TDEC_CORE_CFG3_GDSCR_GDSC_ACCU_RED_ENABLE_BMSK                             0x2000000
#define HWIO_MSS_CC_MODEM_TDEC_CORE_CFG3_GDSCR_GDSC_ACCU_RED_ENABLE_SHFT                                    25
#define HWIO_MSS_CC_MODEM_TDEC_CORE_CFG3_GDSCR_DLY_ACCU_RED_SHIFTER_DONE_BMSK                        0x1e00000
#define HWIO_MSS_CC_MODEM_TDEC_CORE_CFG3_GDSCR_DLY_ACCU_RED_SHIFTER_DONE_SHFT                               21
#define HWIO_MSS_CC_MODEM_TDEC_CORE_CFG3_GDSCR_GDSC_ACCU_RED_TIMER_EN_SW_BMSK                         0x100000
#define HWIO_MSS_CC_MODEM_TDEC_CORE_CFG3_GDSCR_GDSC_ACCU_RED_TIMER_EN_SW_SHFT                               20
#define HWIO_MSS_CC_MODEM_TDEC_CORE_CFG3_GDSCR_GDSC_ACCU_RED_SHIFTER_DONE_OVERRIDE_BMSK                0x80000
#define HWIO_MSS_CC_MODEM_TDEC_CORE_CFG3_GDSCR_GDSC_ACCU_RED_SHIFTER_DONE_OVERRIDE_SHFT                     19
#define HWIO_MSS_CC_MODEM_TDEC_CORE_CFG3_GDSCR_GDSC_ACCU_RED_SHIFTER_CLK_EN_SW_BMSK                    0x40000
#define HWIO_MSS_CC_MODEM_TDEC_CORE_CFG3_GDSCR_GDSC_ACCU_RED_SHIFTER_CLK_EN_SW_SHFT                         18
#define HWIO_MSS_CC_MODEM_TDEC_CORE_CFG3_GDSCR_GDSC_ACCU_RED_SHIFTER_START_SW_BMSK                     0x20000
#define HWIO_MSS_CC_MODEM_TDEC_CORE_CFG3_GDSCR_GDSC_ACCU_RED_SHIFTER_START_SW_SHFT                          17
#define HWIO_MSS_CC_MODEM_TDEC_CORE_CFG3_GDSCR_GDSC_ACCU_RED_SW_OVERRIDE_BMSK                          0x10000
#define HWIO_MSS_CC_MODEM_TDEC_CORE_CFG3_GDSCR_GDSC_ACCU_RED_SW_OVERRIDE_SHFT                               16
#define HWIO_MSS_CC_MODEM_TDEC_CORE_CFG3_GDSCR_GDSC_SPARE_CTRL_IN_BMSK                                  0xff00
#define HWIO_MSS_CC_MODEM_TDEC_CORE_CFG3_GDSCR_GDSC_SPARE_CTRL_IN_SHFT                                       8
#define HWIO_MSS_CC_MODEM_TDEC_CORE_CFG3_GDSCR_GDSC_SPARE_CTRL_OUT_BMSK                                   0xff
#define HWIO_MSS_CC_MODEM_TDEC_CORE_CFG3_GDSCR_GDSC_SPARE_CTRL_OUT_SHFT                                      0

#define HWIO_MSS_CC_MODEM_TDEC_CORE_CFG4_GDSCR_ADDR                                                 (MSS_CC_MSS_CC_REG_REG_BASE            + 0x870)
#define HWIO_MSS_CC_MODEM_TDEC_CORE_CFG4_GDSCR_RMSK                                                   0xffffff
#define HWIO_MSS_CC_MODEM_TDEC_CORE_CFG4_GDSCR_IN                    \
                in_dword(HWIO_MSS_CC_MODEM_TDEC_CORE_CFG4_GDSCR_ADDR)
#define HWIO_MSS_CC_MODEM_TDEC_CORE_CFG4_GDSCR_INM(m)            \
                in_dword_masked(HWIO_MSS_CC_MODEM_TDEC_CORE_CFG4_GDSCR_ADDR, m)
#define HWIO_MSS_CC_MODEM_TDEC_CORE_CFG4_GDSCR_OUT(v)            \
                out_dword(HWIO_MSS_CC_MODEM_TDEC_CORE_CFG4_GDSCR_ADDR,v)
#define HWIO_MSS_CC_MODEM_TDEC_CORE_CFG4_GDSCR_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_CC_MODEM_TDEC_CORE_CFG4_GDSCR_ADDR,m,v,HWIO_MSS_CC_MODEM_TDEC_CORE_CFG4_GDSCR_IN)
#define HWIO_MSS_CC_MODEM_TDEC_CORE_CFG4_GDSCR_DLY_UNCLAMPIO_BMSK                                     0xf00000
#define HWIO_MSS_CC_MODEM_TDEC_CORE_CFG4_GDSCR_DLY_UNCLAMPIO_SHFT                                           20
#define HWIO_MSS_CC_MODEM_TDEC_CORE_CFG4_GDSCR_DLY_RESTOREFF_BMSK                                      0xf0000
#define HWIO_MSS_CC_MODEM_TDEC_CORE_CFG4_GDSCR_DLY_RESTOREFF_SHFT                                           16
#define HWIO_MSS_CC_MODEM_TDEC_CORE_CFG4_GDSCR_DLY_NORETAINFF_BMSK                                      0xf000
#define HWIO_MSS_CC_MODEM_TDEC_CORE_CFG4_GDSCR_DLY_NORETAINFF_SHFT                                          12
#define HWIO_MSS_CC_MODEM_TDEC_CORE_CFG4_GDSCR_DLY_DEASSERTARES_BMSK                                     0xf00
#define HWIO_MSS_CC_MODEM_TDEC_CORE_CFG4_GDSCR_DLY_DEASSERTARES_SHFT                                         8
#define HWIO_MSS_CC_MODEM_TDEC_CORE_CFG4_GDSCR_DLY_CLAMPIO_BMSK                                           0xf0
#define HWIO_MSS_CC_MODEM_TDEC_CORE_CFG4_GDSCR_DLY_CLAMPIO_SHFT                                              4
#define HWIO_MSS_CC_MODEM_TDEC_CORE_CFG4_GDSCR_DLY_RETAINFF_BMSK                                           0xf
#define HWIO_MSS_CC_MODEM_TDEC_CORE_CFG4_GDSCR_DLY_RETAINFF_SHFT                                             0

#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_BCR_ADDR                                                     (MSS_CC_MSS_CC_REG_REG_BASE            + 0x87c)
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_BCR_RMSK                                                            0x1
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_BCR_IN                    \
                in_dword(HWIO_MSS_CC_MODEM_DEMBACK_CORE_BCR_ADDR)
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_BCR_INM(m)            \
                in_dword_masked(HWIO_MSS_CC_MODEM_DEMBACK_CORE_BCR_ADDR, m)
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_BCR_OUT(v)            \
                out_dword(HWIO_MSS_CC_MODEM_DEMBACK_CORE_BCR_ADDR,v)
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_BCR_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_CC_MODEM_DEMBACK_CORE_BCR_ADDR,m,v,HWIO_MSS_CC_MODEM_DEMBACK_CORE_BCR_IN)
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_BCR_BLK_ARES_BMSK                                                   0x1
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_BCR_BLK_ARES_SHFT                                                     0

#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_GDSCR_ADDR                                                   (MSS_CC_MSS_CC_REG_REG_BASE            + 0x880)
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_GDSCR_RMSK                                                   0xf8ffffff
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_GDSCR_IN                    \
                in_dword(HWIO_MSS_CC_MODEM_DEMBACK_CORE_GDSCR_ADDR)
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_GDSCR_INM(m)            \
                in_dword_masked(HWIO_MSS_CC_MODEM_DEMBACK_CORE_GDSCR_ADDR, m)
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_GDSCR_OUT(v)            \
                out_dword(HWIO_MSS_CC_MODEM_DEMBACK_CORE_GDSCR_ADDR,v)
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_GDSCR_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_CC_MODEM_DEMBACK_CORE_GDSCR_ADDR,m,v,HWIO_MSS_CC_MODEM_DEMBACK_CORE_GDSCR_IN)
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_GDSCR_PWR_ON_BMSK                                            0x80000000
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_GDSCR_PWR_ON_SHFT                                                    31
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_GDSCR_GDSC_STATE_BMSK                                        0x78000000
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_GDSCR_GDSC_STATE_SHFT                                                27
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_GDSCR_EN_REST_WAIT_BMSK                                        0xf00000
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_GDSCR_EN_REST_WAIT_SHFT                                              20
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_GDSCR_EN_FEW_WAIT_BMSK                                          0xf0000
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_GDSCR_EN_FEW_WAIT_SHFT                                               16
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_GDSCR_CLK_DIS_WAIT_BMSK                                          0xf000
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_GDSCR_CLK_DIS_WAIT_SHFT                                              12
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_GDSCR_RETAIN_FF_ENABLE_BMSK                                       0x800
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_GDSCR_RETAIN_FF_ENABLE_SHFT                                          11
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_GDSCR_RESTORE_BMSK                                                0x400
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_GDSCR_RESTORE_SHFT                                                   10
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_GDSCR_SAVE_BMSK                                                   0x200
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_GDSCR_SAVE_SHFT                                                       9
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_GDSCR_RETAIN_BMSK                                                 0x100
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_GDSCR_RETAIN_SHFT                                                     8
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_GDSCR_EN_REST_BMSK                                                 0x80
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_GDSCR_EN_REST_SHFT                                                    7
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_GDSCR_EN_FEW_BMSK                                                  0x40
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_GDSCR_EN_FEW_SHFT                                                     6
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_GDSCR_CLAMP_IO_BMSK                                                0x20
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_GDSCR_CLAMP_IO_SHFT                                                   5
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_GDSCR_CLK_DISABLE_BMSK                                             0x10
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_GDSCR_CLK_DISABLE_SHFT                                                4
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_GDSCR_PD_ARES_BMSK                                                  0x8
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_GDSCR_PD_ARES_SHFT                                                    3
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_GDSCR_SW_OVERRIDE_BMSK                                              0x4
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_GDSCR_SW_OVERRIDE_SHFT                                                2
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_GDSCR_HW_CONTROL_BMSK                                               0x2
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_GDSCR_HW_CONTROL_SHFT                                                 1
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_GDSCR_SW_COLLAPSE_BMSK                                              0x1
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_GDSCR_SW_COLLAPSE_SHFT                                                0

#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_CFG_GDSCR_ADDR                                               (MSS_CC_MSS_CC_REG_REG_BASE            + 0x884)
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_CFG_GDSCR_RMSK                                                0x3ffffff
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_CFG_GDSCR_IN                    \
                in_dword(HWIO_MSS_CC_MODEM_DEMBACK_CORE_CFG_GDSCR_ADDR)
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_CFG_GDSCR_INM(m)            \
                in_dword_masked(HWIO_MSS_CC_MODEM_DEMBACK_CORE_CFG_GDSCR_ADDR, m)
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_CFG_GDSCR_OUT(v)            \
                out_dword(HWIO_MSS_CC_MODEM_DEMBACK_CORE_CFG_GDSCR_ADDR,v)
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_CFG_GDSCR_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_CC_MODEM_DEMBACK_CORE_CFG_GDSCR_ADDR,m,v,HWIO_MSS_CC_MODEM_DEMBACK_CORE_CFG_GDSCR_IN)
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_CFG_GDSCR_GDSC_PWR_DWN_START_BMSK                             0x2000000
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_CFG_GDSCR_GDSC_PWR_DWN_START_SHFT                                    25
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_CFG_GDSCR_GDSC_PWR_UP_START_BMSK                              0x1000000
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_CFG_GDSCR_GDSC_PWR_UP_START_SHFT                                     24
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_CFG_GDSCR_GDSC_CFG_FSM_STATE_STATUS_BMSK                       0xf00000
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_CFG_GDSCR_GDSC_CFG_FSM_STATE_STATUS_SHFT                             20
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_CFG_GDSCR_GDSC_MEM_PWR_ACK_STATUS_BMSK                          0x80000
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_CFG_GDSCR_GDSC_MEM_PWR_ACK_STATUS_SHFT                               19
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_CFG_GDSCR_GDSC_ENR_ACK_STATUS_BMSK                              0x40000
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_CFG_GDSCR_GDSC_ENR_ACK_STATUS_SHFT                                   18
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_CFG_GDSCR_GDSC_ENF_ACK_STATUS_BMSK                              0x20000
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_CFG_GDSCR_GDSC_ENF_ACK_STATUS_SHFT                                   17
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_CFG_GDSCR_GDSC_POWER_UP_COMPLETE_BMSK                           0x10000
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_CFG_GDSCR_GDSC_POWER_UP_COMPLETE_SHFT                                16
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_CFG_GDSCR_GDSC_POWER_DOWN_COMPLETE_BMSK                          0x8000
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_CFG_GDSCR_GDSC_POWER_DOWN_COMPLETE_SHFT                              15
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_CFG_GDSCR_SOFTWARE_CONTROL_OVERRIDE_BMSK                         0x7800
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_CFG_GDSCR_SOFTWARE_CONTROL_OVERRIDE_SHFT                             11
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_CFG_GDSCR_GDSC_HANDSHAKE_DIS_BMSK                                 0x400
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_CFG_GDSCR_GDSC_HANDSHAKE_DIS_SHFT                                    10
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_CFG_GDSCR_GDSC_MEM_PERI_FORCE_IN_SW_BMSK                          0x200
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_CFG_GDSCR_GDSC_MEM_PERI_FORCE_IN_SW_SHFT                              9
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_CFG_GDSCR_GDSC_MEM_CORE_FORCE_IN_SW_BMSK                          0x100
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_CFG_GDSCR_GDSC_MEM_CORE_FORCE_IN_SW_SHFT                              8
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_CFG_GDSCR_GDSC_PHASE_RESET_EN_SW_BMSK                              0x80
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_CFG_GDSCR_GDSC_PHASE_RESET_EN_SW_SHFT                                 7
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_CFG_GDSCR_GDSC_PHASE_RESET_DELAY_COUNT_SW_BMSK                     0x60
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_CFG_GDSCR_GDSC_PHASE_RESET_DELAY_COUNT_SW_SHFT                        5
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_CFG_GDSCR_GDSC_PSCBC_PWR_DWN_SW_BMSK                               0x10
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_CFG_GDSCR_GDSC_PSCBC_PWR_DWN_SW_SHFT                                  4
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_CFG_GDSCR_UNCLAMP_IO_SOFTWARE_OVERRIDE_BMSK                         0x8
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_CFG_GDSCR_UNCLAMP_IO_SOFTWARE_OVERRIDE_SHFT                           3
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_CFG_GDSCR_SAVE_RESTORE_SOFTWARE_OVERRIDE_BMSK                       0x4
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_CFG_GDSCR_SAVE_RESTORE_SOFTWARE_OVERRIDE_SHFT                         2
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_CFG_GDSCR_CLAMP_IO_SOFTWARE_OVERRIDE_BMSK                           0x2
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_CFG_GDSCR_CLAMP_IO_SOFTWARE_OVERRIDE_SHFT                             1
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_CFG_GDSCR_DISABLE_CLK_SOFTWARE_OVERRIDE_BMSK                        0x1
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_CFG_GDSCR_DISABLE_CLK_SOFTWARE_OVERRIDE_SHFT                          0

#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_CFG2_GDSCR_ADDR                                              (MSS_CC_MSS_CC_REG_REG_BASE            + 0x888)
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_CFG2_GDSCR_RMSK                                                 0x7ffff
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_CFG2_GDSCR_IN                    \
                in_dword(HWIO_MSS_CC_MODEM_DEMBACK_CORE_CFG2_GDSCR_ADDR)
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_CFG2_GDSCR_INM(m)            \
                in_dword_masked(HWIO_MSS_CC_MODEM_DEMBACK_CORE_CFG2_GDSCR_ADDR, m)
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_CFG2_GDSCR_OUT(v)            \
                out_dword(HWIO_MSS_CC_MODEM_DEMBACK_CORE_CFG2_GDSCR_ADDR,v)
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_CFG2_GDSCR_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_CC_MODEM_DEMBACK_CORE_CFG2_GDSCR_ADDR,m,v,HWIO_MSS_CC_MODEM_DEMBACK_CORE_CFG2_GDSCR_IN)
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_CFG2_GDSCR_GDSC_MEM_PWRUP_ACK_OVERRIDE_BMSK                     0x40000
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_CFG2_GDSCR_GDSC_MEM_PWRUP_ACK_OVERRIDE_SHFT                          18
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_CFG2_GDSCR_GDSC_PWRDWN_ENABLE_ACK_OVERRIDE_BMSK                 0x20000
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_CFG2_GDSCR_GDSC_PWRDWN_ENABLE_ACK_OVERRIDE_SHFT                      17
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_CFG2_GDSCR_GDSC_CLAMP_MEM_SW_BMSK                               0x10000
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_CFG2_GDSCR_GDSC_CLAMP_MEM_SW_SHFT                                    16
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_CFG2_GDSCR_DLY_MEM_PWR_UP_BMSK                                   0xf000
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_CFG2_GDSCR_DLY_MEM_PWR_UP_SHFT                                       12
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_CFG2_GDSCR_DLY_DEASSERT_CLAMP_MEM_BMSK                            0xf00
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_CFG2_GDSCR_DLY_DEASSERT_CLAMP_MEM_SHFT                                8
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_CFG2_GDSCR_DLY_ASSERT_CLAMP_MEM_BMSK                               0xf0
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_CFG2_GDSCR_DLY_ASSERT_CLAMP_MEM_SHFT                                  4
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_CFG2_GDSCR_MEM_PWR_DWN_TIMEOUT_BMSK                                 0xf
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_CFG2_GDSCR_MEM_PWR_DWN_TIMEOUT_SHFT                                   0

#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_CFG3_GDSCR_ADDR                                              (MSS_CC_MSS_CC_REG_REG_BASE            + 0x88c)
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_CFG3_GDSCR_RMSK                                               0x7ffffff
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_CFG3_GDSCR_IN                    \
                in_dword(HWIO_MSS_CC_MODEM_DEMBACK_CORE_CFG3_GDSCR_ADDR)
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_CFG3_GDSCR_INM(m)            \
                in_dword_masked(HWIO_MSS_CC_MODEM_DEMBACK_CORE_CFG3_GDSCR_ADDR, m)
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_CFG3_GDSCR_OUT(v)            \
                out_dword(HWIO_MSS_CC_MODEM_DEMBACK_CORE_CFG3_GDSCR_ADDR,v)
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_CFG3_GDSCR_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_CC_MODEM_DEMBACK_CORE_CFG3_GDSCR_ADDR,m,v,HWIO_MSS_CC_MODEM_DEMBACK_CORE_CFG3_GDSCR_IN)
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_CFG3_GDSCR_GDSC_ACCU_RED_SHIFTER_DONE_STATUS_BMSK             0x4000000
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_CFG3_GDSCR_GDSC_ACCU_RED_SHIFTER_DONE_STATUS_SHFT                    26
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_CFG3_GDSCR_GDSC_ACCU_RED_ENABLE_BMSK                          0x2000000
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_CFG3_GDSCR_GDSC_ACCU_RED_ENABLE_SHFT                                 25
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_CFG3_GDSCR_DLY_ACCU_RED_SHIFTER_DONE_BMSK                     0x1e00000
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_CFG3_GDSCR_DLY_ACCU_RED_SHIFTER_DONE_SHFT                            21
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_CFG3_GDSCR_GDSC_ACCU_RED_TIMER_EN_SW_BMSK                      0x100000
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_CFG3_GDSCR_GDSC_ACCU_RED_TIMER_EN_SW_SHFT                            20
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_CFG3_GDSCR_GDSC_ACCU_RED_SHIFTER_DONE_OVERRIDE_BMSK             0x80000
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_CFG3_GDSCR_GDSC_ACCU_RED_SHIFTER_DONE_OVERRIDE_SHFT                  19
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_CFG3_GDSCR_GDSC_ACCU_RED_SHIFTER_CLK_EN_SW_BMSK                 0x40000
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_CFG3_GDSCR_GDSC_ACCU_RED_SHIFTER_CLK_EN_SW_SHFT                      18
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_CFG3_GDSCR_GDSC_ACCU_RED_SHIFTER_START_SW_BMSK                  0x20000
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_CFG3_GDSCR_GDSC_ACCU_RED_SHIFTER_START_SW_SHFT                       17
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_CFG3_GDSCR_GDSC_ACCU_RED_SW_OVERRIDE_BMSK                       0x10000
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_CFG3_GDSCR_GDSC_ACCU_RED_SW_OVERRIDE_SHFT                            16
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_CFG3_GDSCR_GDSC_SPARE_CTRL_IN_BMSK                               0xff00
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_CFG3_GDSCR_GDSC_SPARE_CTRL_IN_SHFT                                    8
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_CFG3_GDSCR_GDSC_SPARE_CTRL_OUT_BMSK                                0xff
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_CFG3_GDSCR_GDSC_SPARE_CTRL_OUT_SHFT                                   0

#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_CFG4_GDSCR_ADDR                                              (MSS_CC_MSS_CC_REG_REG_BASE            + 0x890)
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_CFG4_GDSCR_RMSK                                                0xffffff
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_CFG4_GDSCR_IN                    \
                in_dword(HWIO_MSS_CC_MODEM_DEMBACK_CORE_CFG4_GDSCR_ADDR)
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_CFG4_GDSCR_INM(m)            \
                in_dword_masked(HWIO_MSS_CC_MODEM_DEMBACK_CORE_CFG4_GDSCR_ADDR, m)
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_CFG4_GDSCR_OUT(v)            \
                out_dword(HWIO_MSS_CC_MODEM_DEMBACK_CORE_CFG4_GDSCR_ADDR,v)
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_CFG4_GDSCR_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_CC_MODEM_DEMBACK_CORE_CFG4_GDSCR_ADDR,m,v,HWIO_MSS_CC_MODEM_DEMBACK_CORE_CFG4_GDSCR_IN)
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_CFG4_GDSCR_DLY_UNCLAMPIO_BMSK                                  0xf00000
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_CFG4_GDSCR_DLY_UNCLAMPIO_SHFT                                        20
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_CFG4_GDSCR_DLY_RESTOREFF_BMSK                                   0xf0000
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_CFG4_GDSCR_DLY_RESTOREFF_SHFT                                        16
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_CFG4_GDSCR_DLY_NORETAINFF_BMSK                                   0xf000
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_CFG4_GDSCR_DLY_NORETAINFF_SHFT                                       12
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_CFG4_GDSCR_DLY_DEASSERTARES_BMSK                                  0xf00
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_CFG4_GDSCR_DLY_DEASSERTARES_SHFT                                      8
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_CFG4_GDSCR_DLY_CLAMPIO_BMSK                                        0xf0
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_CFG4_GDSCR_DLY_CLAMPIO_SHFT                                           4
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_CFG4_GDSCR_DLY_RETAINFF_BMSK                                        0xf
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_CFG4_GDSCR_DLY_RETAINFF_SHFT                                          0

#define HWIO_MSS_CC_MODEM_TX_CORE_BCR_ADDR                                                          (MSS_CC_MSS_CC_REG_REG_BASE            + 0x898)
#define HWIO_MSS_CC_MODEM_TX_CORE_BCR_RMSK                                                                 0x1
#define HWIO_MSS_CC_MODEM_TX_CORE_BCR_IN                    \
                in_dword(HWIO_MSS_CC_MODEM_TX_CORE_BCR_ADDR)
#define HWIO_MSS_CC_MODEM_TX_CORE_BCR_INM(m)            \
                in_dword_masked(HWIO_MSS_CC_MODEM_TX_CORE_BCR_ADDR, m)
#define HWIO_MSS_CC_MODEM_TX_CORE_BCR_OUT(v)            \
                out_dword(HWIO_MSS_CC_MODEM_TX_CORE_BCR_ADDR,v)
#define HWIO_MSS_CC_MODEM_TX_CORE_BCR_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_CC_MODEM_TX_CORE_BCR_ADDR,m,v,HWIO_MSS_CC_MODEM_TX_CORE_BCR_IN)
#define HWIO_MSS_CC_MODEM_TX_CORE_BCR_BLK_ARES_BMSK                                                        0x1
#define HWIO_MSS_CC_MODEM_TX_CORE_BCR_BLK_ARES_SHFT                                                          0

#define HWIO_MSS_CC_MODEM_TX_CORE_GDSCR_ADDR                                                        (MSS_CC_MSS_CC_REG_REG_BASE            + 0x89c)
#define HWIO_MSS_CC_MODEM_TX_CORE_GDSCR_RMSK                                                        0xf8ffffff
#define HWIO_MSS_CC_MODEM_TX_CORE_GDSCR_IN                    \
                in_dword(HWIO_MSS_CC_MODEM_TX_CORE_GDSCR_ADDR)
#define HWIO_MSS_CC_MODEM_TX_CORE_GDSCR_INM(m)            \
                in_dword_masked(HWIO_MSS_CC_MODEM_TX_CORE_GDSCR_ADDR, m)
#define HWIO_MSS_CC_MODEM_TX_CORE_GDSCR_OUT(v)            \
                out_dword(HWIO_MSS_CC_MODEM_TX_CORE_GDSCR_ADDR,v)
#define HWIO_MSS_CC_MODEM_TX_CORE_GDSCR_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_CC_MODEM_TX_CORE_GDSCR_ADDR,m,v,HWIO_MSS_CC_MODEM_TX_CORE_GDSCR_IN)
#define HWIO_MSS_CC_MODEM_TX_CORE_GDSCR_PWR_ON_BMSK                                                 0x80000000
#define HWIO_MSS_CC_MODEM_TX_CORE_GDSCR_PWR_ON_SHFT                                                         31
#define HWIO_MSS_CC_MODEM_TX_CORE_GDSCR_GDSC_STATE_BMSK                                             0x78000000
#define HWIO_MSS_CC_MODEM_TX_CORE_GDSCR_GDSC_STATE_SHFT                                                     27
#define HWIO_MSS_CC_MODEM_TX_CORE_GDSCR_EN_REST_WAIT_BMSK                                             0xf00000
#define HWIO_MSS_CC_MODEM_TX_CORE_GDSCR_EN_REST_WAIT_SHFT                                                   20
#define HWIO_MSS_CC_MODEM_TX_CORE_GDSCR_EN_FEW_WAIT_BMSK                                               0xf0000
#define HWIO_MSS_CC_MODEM_TX_CORE_GDSCR_EN_FEW_WAIT_SHFT                                                    16
#define HWIO_MSS_CC_MODEM_TX_CORE_GDSCR_CLK_DIS_WAIT_BMSK                                               0xf000
#define HWIO_MSS_CC_MODEM_TX_CORE_GDSCR_CLK_DIS_WAIT_SHFT                                                   12
#define HWIO_MSS_CC_MODEM_TX_CORE_GDSCR_RETAIN_FF_ENABLE_BMSK                                            0x800
#define HWIO_MSS_CC_MODEM_TX_CORE_GDSCR_RETAIN_FF_ENABLE_SHFT                                               11
#define HWIO_MSS_CC_MODEM_TX_CORE_GDSCR_RESTORE_BMSK                                                     0x400
#define HWIO_MSS_CC_MODEM_TX_CORE_GDSCR_RESTORE_SHFT                                                        10
#define HWIO_MSS_CC_MODEM_TX_CORE_GDSCR_SAVE_BMSK                                                        0x200
#define HWIO_MSS_CC_MODEM_TX_CORE_GDSCR_SAVE_SHFT                                                            9
#define HWIO_MSS_CC_MODEM_TX_CORE_GDSCR_RETAIN_BMSK                                                      0x100
#define HWIO_MSS_CC_MODEM_TX_CORE_GDSCR_RETAIN_SHFT                                                          8
#define HWIO_MSS_CC_MODEM_TX_CORE_GDSCR_EN_REST_BMSK                                                      0x80
#define HWIO_MSS_CC_MODEM_TX_CORE_GDSCR_EN_REST_SHFT                                                         7
#define HWIO_MSS_CC_MODEM_TX_CORE_GDSCR_EN_FEW_BMSK                                                       0x40
#define HWIO_MSS_CC_MODEM_TX_CORE_GDSCR_EN_FEW_SHFT                                                          6
#define HWIO_MSS_CC_MODEM_TX_CORE_GDSCR_CLAMP_IO_BMSK                                                     0x20
#define HWIO_MSS_CC_MODEM_TX_CORE_GDSCR_CLAMP_IO_SHFT                                                        5
#define HWIO_MSS_CC_MODEM_TX_CORE_GDSCR_CLK_DISABLE_BMSK                                                  0x10
#define HWIO_MSS_CC_MODEM_TX_CORE_GDSCR_CLK_DISABLE_SHFT                                                     4
#define HWIO_MSS_CC_MODEM_TX_CORE_GDSCR_PD_ARES_BMSK                                                       0x8
#define HWIO_MSS_CC_MODEM_TX_CORE_GDSCR_PD_ARES_SHFT                                                         3
#define HWIO_MSS_CC_MODEM_TX_CORE_GDSCR_SW_OVERRIDE_BMSK                                                   0x4
#define HWIO_MSS_CC_MODEM_TX_CORE_GDSCR_SW_OVERRIDE_SHFT                                                     2
#define HWIO_MSS_CC_MODEM_TX_CORE_GDSCR_HW_CONTROL_BMSK                                                    0x2
#define HWIO_MSS_CC_MODEM_TX_CORE_GDSCR_HW_CONTROL_SHFT                                                      1
#define HWIO_MSS_CC_MODEM_TX_CORE_GDSCR_SW_COLLAPSE_BMSK                                                   0x1
#define HWIO_MSS_CC_MODEM_TX_CORE_GDSCR_SW_COLLAPSE_SHFT                                                     0

#define HWIO_MSS_CC_MODEM_TX_CORE_CFG_GDSCR_ADDR                                                    (MSS_CC_MSS_CC_REG_REG_BASE            + 0x8a0)
#define HWIO_MSS_CC_MODEM_TX_CORE_CFG_GDSCR_RMSK                                                     0x3ffffff
#define HWIO_MSS_CC_MODEM_TX_CORE_CFG_GDSCR_IN                    \
                in_dword(HWIO_MSS_CC_MODEM_TX_CORE_CFG_GDSCR_ADDR)
#define HWIO_MSS_CC_MODEM_TX_CORE_CFG_GDSCR_INM(m)            \
                in_dword_masked(HWIO_MSS_CC_MODEM_TX_CORE_CFG_GDSCR_ADDR, m)
#define HWIO_MSS_CC_MODEM_TX_CORE_CFG_GDSCR_OUT(v)            \
                out_dword(HWIO_MSS_CC_MODEM_TX_CORE_CFG_GDSCR_ADDR,v)
#define HWIO_MSS_CC_MODEM_TX_CORE_CFG_GDSCR_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_CC_MODEM_TX_CORE_CFG_GDSCR_ADDR,m,v,HWIO_MSS_CC_MODEM_TX_CORE_CFG_GDSCR_IN)
#define HWIO_MSS_CC_MODEM_TX_CORE_CFG_GDSCR_GDSC_PWR_DWN_START_BMSK                                  0x2000000
#define HWIO_MSS_CC_MODEM_TX_CORE_CFG_GDSCR_GDSC_PWR_DWN_START_SHFT                                         25
#define HWIO_MSS_CC_MODEM_TX_CORE_CFG_GDSCR_GDSC_PWR_UP_START_BMSK                                   0x1000000
#define HWIO_MSS_CC_MODEM_TX_CORE_CFG_GDSCR_GDSC_PWR_UP_START_SHFT                                          24
#define HWIO_MSS_CC_MODEM_TX_CORE_CFG_GDSCR_GDSC_CFG_FSM_STATE_STATUS_BMSK                            0xf00000
#define HWIO_MSS_CC_MODEM_TX_CORE_CFG_GDSCR_GDSC_CFG_FSM_STATE_STATUS_SHFT                                  20
#define HWIO_MSS_CC_MODEM_TX_CORE_CFG_GDSCR_GDSC_MEM_PWR_ACK_STATUS_BMSK                               0x80000
#define HWIO_MSS_CC_MODEM_TX_CORE_CFG_GDSCR_GDSC_MEM_PWR_ACK_STATUS_SHFT                                    19
#define HWIO_MSS_CC_MODEM_TX_CORE_CFG_GDSCR_GDSC_ENR_ACK_STATUS_BMSK                                   0x40000
#define HWIO_MSS_CC_MODEM_TX_CORE_CFG_GDSCR_GDSC_ENR_ACK_STATUS_SHFT                                        18
#define HWIO_MSS_CC_MODEM_TX_CORE_CFG_GDSCR_GDSC_ENF_ACK_STATUS_BMSK                                   0x20000
#define HWIO_MSS_CC_MODEM_TX_CORE_CFG_GDSCR_GDSC_ENF_ACK_STATUS_SHFT                                        17
#define HWIO_MSS_CC_MODEM_TX_CORE_CFG_GDSCR_GDSC_POWER_UP_COMPLETE_BMSK                                0x10000
#define HWIO_MSS_CC_MODEM_TX_CORE_CFG_GDSCR_GDSC_POWER_UP_COMPLETE_SHFT                                     16
#define HWIO_MSS_CC_MODEM_TX_CORE_CFG_GDSCR_GDSC_POWER_DOWN_COMPLETE_BMSK                               0x8000
#define HWIO_MSS_CC_MODEM_TX_CORE_CFG_GDSCR_GDSC_POWER_DOWN_COMPLETE_SHFT                                   15
#define HWIO_MSS_CC_MODEM_TX_CORE_CFG_GDSCR_SOFTWARE_CONTROL_OVERRIDE_BMSK                              0x7800
#define HWIO_MSS_CC_MODEM_TX_CORE_CFG_GDSCR_SOFTWARE_CONTROL_OVERRIDE_SHFT                                  11
#define HWIO_MSS_CC_MODEM_TX_CORE_CFG_GDSCR_GDSC_HANDSHAKE_DIS_BMSK                                      0x400
#define HWIO_MSS_CC_MODEM_TX_CORE_CFG_GDSCR_GDSC_HANDSHAKE_DIS_SHFT                                         10
#define HWIO_MSS_CC_MODEM_TX_CORE_CFG_GDSCR_GDSC_MEM_PERI_FORCE_IN_SW_BMSK                               0x200
#define HWIO_MSS_CC_MODEM_TX_CORE_CFG_GDSCR_GDSC_MEM_PERI_FORCE_IN_SW_SHFT                                   9
#define HWIO_MSS_CC_MODEM_TX_CORE_CFG_GDSCR_GDSC_MEM_CORE_FORCE_IN_SW_BMSK                               0x100
#define HWIO_MSS_CC_MODEM_TX_CORE_CFG_GDSCR_GDSC_MEM_CORE_FORCE_IN_SW_SHFT                                   8
#define HWIO_MSS_CC_MODEM_TX_CORE_CFG_GDSCR_GDSC_PHASE_RESET_EN_SW_BMSK                                   0x80
#define HWIO_MSS_CC_MODEM_TX_CORE_CFG_GDSCR_GDSC_PHASE_RESET_EN_SW_SHFT                                      7
#define HWIO_MSS_CC_MODEM_TX_CORE_CFG_GDSCR_GDSC_PHASE_RESET_DELAY_COUNT_SW_BMSK                          0x60
#define HWIO_MSS_CC_MODEM_TX_CORE_CFG_GDSCR_GDSC_PHASE_RESET_DELAY_COUNT_SW_SHFT                             5
#define HWIO_MSS_CC_MODEM_TX_CORE_CFG_GDSCR_GDSC_PSCBC_PWR_DWN_SW_BMSK                                    0x10
#define HWIO_MSS_CC_MODEM_TX_CORE_CFG_GDSCR_GDSC_PSCBC_PWR_DWN_SW_SHFT                                       4
#define HWIO_MSS_CC_MODEM_TX_CORE_CFG_GDSCR_UNCLAMP_IO_SOFTWARE_OVERRIDE_BMSK                              0x8
#define HWIO_MSS_CC_MODEM_TX_CORE_CFG_GDSCR_UNCLAMP_IO_SOFTWARE_OVERRIDE_SHFT                                3
#define HWIO_MSS_CC_MODEM_TX_CORE_CFG_GDSCR_SAVE_RESTORE_SOFTWARE_OVERRIDE_BMSK                            0x4
#define HWIO_MSS_CC_MODEM_TX_CORE_CFG_GDSCR_SAVE_RESTORE_SOFTWARE_OVERRIDE_SHFT                              2
#define HWIO_MSS_CC_MODEM_TX_CORE_CFG_GDSCR_CLAMP_IO_SOFTWARE_OVERRIDE_BMSK                                0x2
#define HWIO_MSS_CC_MODEM_TX_CORE_CFG_GDSCR_CLAMP_IO_SOFTWARE_OVERRIDE_SHFT                                  1
#define HWIO_MSS_CC_MODEM_TX_CORE_CFG_GDSCR_DISABLE_CLK_SOFTWARE_OVERRIDE_BMSK                             0x1
#define HWIO_MSS_CC_MODEM_TX_CORE_CFG_GDSCR_DISABLE_CLK_SOFTWARE_OVERRIDE_SHFT                               0

#define HWIO_MSS_CC_MODEM_TX_CORE_CFG2_GDSCR_ADDR                                                   (MSS_CC_MSS_CC_REG_REG_BASE            + 0x8a4)
#define HWIO_MSS_CC_MODEM_TX_CORE_CFG2_GDSCR_RMSK                                                      0x7ffff
#define HWIO_MSS_CC_MODEM_TX_CORE_CFG2_GDSCR_IN                    \
                in_dword(HWIO_MSS_CC_MODEM_TX_CORE_CFG2_GDSCR_ADDR)
#define HWIO_MSS_CC_MODEM_TX_CORE_CFG2_GDSCR_INM(m)            \
                in_dword_masked(HWIO_MSS_CC_MODEM_TX_CORE_CFG2_GDSCR_ADDR, m)
#define HWIO_MSS_CC_MODEM_TX_CORE_CFG2_GDSCR_OUT(v)            \
                out_dword(HWIO_MSS_CC_MODEM_TX_CORE_CFG2_GDSCR_ADDR,v)
#define HWIO_MSS_CC_MODEM_TX_CORE_CFG2_GDSCR_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_CC_MODEM_TX_CORE_CFG2_GDSCR_ADDR,m,v,HWIO_MSS_CC_MODEM_TX_CORE_CFG2_GDSCR_IN)
#define HWIO_MSS_CC_MODEM_TX_CORE_CFG2_GDSCR_GDSC_MEM_PWRUP_ACK_OVERRIDE_BMSK                          0x40000
#define HWIO_MSS_CC_MODEM_TX_CORE_CFG2_GDSCR_GDSC_MEM_PWRUP_ACK_OVERRIDE_SHFT                               18
#define HWIO_MSS_CC_MODEM_TX_CORE_CFG2_GDSCR_GDSC_PWRDWN_ENABLE_ACK_OVERRIDE_BMSK                      0x20000
#define HWIO_MSS_CC_MODEM_TX_CORE_CFG2_GDSCR_GDSC_PWRDWN_ENABLE_ACK_OVERRIDE_SHFT                           17
#define HWIO_MSS_CC_MODEM_TX_CORE_CFG2_GDSCR_GDSC_CLAMP_MEM_SW_BMSK                                    0x10000
#define HWIO_MSS_CC_MODEM_TX_CORE_CFG2_GDSCR_GDSC_CLAMP_MEM_SW_SHFT                                         16
#define HWIO_MSS_CC_MODEM_TX_CORE_CFG2_GDSCR_DLY_MEM_PWR_UP_BMSK                                        0xf000
#define HWIO_MSS_CC_MODEM_TX_CORE_CFG2_GDSCR_DLY_MEM_PWR_UP_SHFT                                            12
#define HWIO_MSS_CC_MODEM_TX_CORE_CFG2_GDSCR_DLY_DEASSERT_CLAMP_MEM_BMSK                                 0xf00
#define HWIO_MSS_CC_MODEM_TX_CORE_CFG2_GDSCR_DLY_DEASSERT_CLAMP_MEM_SHFT                                     8
#define HWIO_MSS_CC_MODEM_TX_CORE_CFG2_GDSCR_DLY_ASSERT_CLAMP_MEM_BMSK                                    0xf0
#define HWIO_MSS_CC_MODEM_TX_CORE_CFG2_GDSCR_DLY_ASSERT_CLAMP_MEM_SHFT                                       4
#define HWIO_MSS_CC_MODEM_TX_CORE_CFG2_GDSCR_MEM_PWR_DWN_TIMEOUT_BMSK                                      0xf
#define HWIO_MSS_CC_MODEM_TX_CORE_CFG2_GDSCR_MEM_PWR_DWN_TIMEOUT_SHFT                                        0

#define HWIO_MSS_CC_MODEM_TX_CORE_CFG3_GDSCR_ADDR                                                   (MSS_CC_MSS_CC_REG_REG_BASE            + 0x8a8)
#define HWIO_MSS_CC_MODEM_TX_CORE_CFG3_GDSCR_RMSK                                                    0x7ffffff
#define HWIO_MSS_CC_MODEM_TX_CORE_CFG3_GDSCR_IN                    \
                in_dword(HWIO_MSS_CC_MODEM_TX_CORE_CFG3_GDSCR_ADDR)
#define HWIO_MSS_CC_MODEM_TX_CORE_CFG3_GDSCR_INM(m)            \
                in_dword_masked(HWIO_MSS_CC_MODEM_TX_CORE_CFG3_GDSCR_ADDR, m)
#define HWIO_MSS_CC_MODEM_TX_CORE_CFG3_GDSCR_OUT(v)            \
                out_dword(HWIO_MSS_CC_MODEM_TX_CORE_CFG3_GDSCR_ADDR,v)
#define HWIO_MSS_CC_MODEM_TX_CORE_CFG3_GDSCR_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_CC_MODEM_TX_CORE_CFG3_GDSCR_ADDR,m,v,HWIO_MSS_CC_MODEM_TX_CORE_CFG3_GDSCR_IN)
#define HWIO_MSS_CC_MODEM_TX_CORE_CFG3_GDSCR_GDSC_ACCU_RED_SHIFTER_DONE_STATUS_BMSK                  0x4000000
#define HWIO_MSS_CC_MODEM_TX_CORE_CFG3_GDSCR_GDSC_ACCU_RED_SHIFTER_DONE_STATUS_SHFT                         26
#define HWIO_MSS_CC_MODEM_TX_CORE_CFG3_GDSCR_GDSC_ACCU_RED_ENABLE_BMSK                               0x2000000
#define HWIO_MSS_CC_MODEM_TX_CORE_CFG3_GDSCR_GDSC_ACCU_RED_ENABLE_SHFT                                      25
#define HWIO_MSS_CC_MODEM_TX_CORE_CFG3_GDSCR_DLY_ACCU_RED_SHIFTER_DONE_BMSK                          0x1e00000
#define HWIO_MSS_CC_MODEM_TX_CORE_CFG3_GDSCR_DLY_ACCU_RED_SHIFTER_DONE_SHFT                                 21
#define HWIO_MSS_CC_MODEM_TX_CORE_CFG3_GDSCR_GDSC_ACCU_RED_TIMER_EN_SW_BMSK                           0x100000
#define HWIO_MSS_CC_MODEM_TX_CORE_CFG3_GDSCR_GDSC_ACCU_RED_TIMER_EN_SW_SHFT                                 20
#define HWIO_MSS_CC_MODEM_TX_CORE_CFG3_GDSCR_GDSC_ACCU_RED_SHIFTER_DONE_OVERRIDE_BMSK                  0x80000
#define HWIO_MSS_CC_MODEM_TX_CORE_CFG3_GDSCR_GDSC_ACCU_RED_SHIFTER_DONE_OVERRIDE_SHFT                       19
#define HWIO_MSS_CC_MODEM_TX_CORE_CFG3_GDSCR_GDSC_ACCU_RED_SHIFTER_CLK_EN_SW_BMSK                      0x40000
#define HWIO_MSS_CC_MODEM_TX_CORE_CFG3_GDSCR_GDSC_ACCU_RED_SHIFTER_CLK_EN_SW_SHFT                           18
#define HWIO_MSS_CC_MODEM_TX_CORE_CFG3_GDSCR_GDSC_ACCU_RED_SHIFTER_START_SW_BMSK                       0x20000
#define HWIO_MSS_CC_MODEM_TX_CORE_CFG3_GDSCR_GDSC_ACCU_RED_SHIFTER_START_SW_SHFT                            17
#define HWIO_MSS_CC_MODEM_TX_CORE_CFG3_GDSCR_GDSC_ACCU_RED_SW_OVERRIDE_BMSK                            0x10000
#define HWIO_MSS_CC_MODEM_TX_CORE_CFG3_GDSCR_GDSC_ACCU_RED_SW_OVERRIDE_SHFT                                 16
#define HWIO_MSS_CC_MODEM_TX_CORE_CFG3_GDSCR_GDSC_SPARE_CTRL_IN_BMSK                                    0xff00
#define HWIO_MSS_CC_MODEM_TX_CORE_CFG3_GDSCR_GDSC_SPARE_CTRL_IN_SHFT                                         8
#define HWIO_MSS_CC_MODEM_TX_CORE_CFG3_GDSCR_GDSC_SPARE_CTRL_OUT_BMSK                                     0xff
#define HWIO_MSS_CC_MODEM_TX_CORE_CFG3_GDSCR_GDSC_SPARE_CTRL_OUT_SHFT                                        0

#define HWIO_MSS_CC_MODEM_TX_CORE_CFG4_GDSCR_ADDR                                                   (MSS_CC_MSS_CC_REG_REG_BASE            + 0x8ac)
#define HWIO_MSS_CC_MODEM_TX_CORE_CFG4_GDSCR_RMSK                                                     0xffffff
#define HWIO_MSS_CC_MODEM_TX_CORE_CFG4_GDSCR_IN                    \
                in_dword(HWIO_MSS_CC_MODEM_TX_CORE_CFG4_GDSCR_ADDR)
#define HWIO_MSS_CC_MODEM_TX_CORE_CFG4_GDSCR_INM(m)            \
                in_dword_masked(HWIO_MSS_CC_MODEM_TX_CORE_CFG4_GDSCR_ADDR, m)
#define HWIO_MSS_CC_MODEM_TX_CORE_CFG4_GDSCR_OUT(v)            \
                out_dword(HWIO_MSS_CC_MODEM_TX_CORE_CFG4_GDSCR_ADDR,v)
#define HWIO_MSS_CC_MODEM_TX_CORE_CFG4_GDSCR_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_CC_MODEM_TX_CORE_CFG4_GDSCR_ADDR,m,v,HWIO_MSS_CC_MODEM_TX_CORE_CFG4_GDSCR_IN)
#define HWIO_MSS_CC_MODEM_TX_CORE_CFG4_GDSCR_DLY_UNCLAMPIO_BMSK                                       0xf00000
#define HWIO_MSS_CC_MODEM_TX_CORE_CFG4_GDSCR_DLY_UNCLAMPIO_SHFT                                             20
#define HWIO_MSS_CC_MODEM_TX_CORE_CFG4_GDSCR_DLY_RESTOREFF_BMSK                                        0xf0000
#define HWIO_MSS_CC_MODEM_TX_CORE_CFG4_GDSCR_DLY_RESTOREFF_SHFT                                             16
#define HWIO_MSS_CC_MODEM_TX_CORE_CFG4_GDSCR_DLY_NORETAINFF_BMSK                                        0xf000
#define HWIO_MSS_CC_MODEM_TX_CORE_CFG4_GDSCR_DLY_NORETAINFF_SHFT                                            12
#define HWIO_MSS_CC_MODEM_TX_CORE_CFG4_GDSCR_DLY_DEASSERTARES_BMSK                                       0xf00
#define HWIO_MSS_CC_MODEM_TX_CORE_CFG4_GDSCR_DLY_DEASSERTARES_SHFT                                           8
#define HWIO_MSS_CC_MODEM_TX_CORE_CFG4_GDSCR_DLY_CLAMPIO_BMSK                                             0xf0
#define HWIO_MSS_CC_MODEM_TX_CORE_CFG4_GDSCR_DLY_CLAMPIO_SHFT                                                4
#define HWIO_MSS_CC_MODEM_TX_CORE_CFG4_GDSCR_DLY_RETAINFF_BMSK                                             0xf
#define HWIO_MSS_CC_MODEM_TX_CORE_CFG4_GDSCR_DLY_RETAINFF_SHFT                                               0

#define HWIO_MSS_CC_MODEM_BCR_ADDR                                                                  (MSS_CC_MSS_CC_REG_REG_BASE            + 0x8fc)
#define HWIO_MSS_CC_MODEM_BCR_RMSK                                                                         0x1
#define HWIO_MSS_CC_MODEM_BCR_IN                    \
                in_dword(HWIO_MSS_CC_MODEM_BCR_ADDR)
#define HWIO_MSS_CC_MODEM_BCR_INM(m)            \
                in_dword_masked(HWIO_MSS_CC_MODEM_BCR_ADDR, m)
#define HWIO_MSS_CC_MODEM_BCR_OUT(v)            \
                out_dword(HWIO_MSS_CC_MODEM_BCR_ADDR,v)
#define HWIO_MSS_CC_MODEM_BCR_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_CC_MODEM_BCR_ADDR,m,v,HWIO_MSS_CC_MODEM_BCR_IN)
#define HWIO_MSS_CC_MODEM_BCR_BLK_ARES_BMSK                                                                0x1
#define HWIO_MSS_CC_MODEM_BCR_BLK_ARES_SHFT                                                                  0

#define HWIO_MSS_CC_MODEM_GDSCR_ADDR                                                                (MSS_CC_MSS_CC_REG_REG_BASE            + 0x900)
#define HWIO_MSS_CC_MODEM_GDSCR_RMSK                                                                0xf8ffffff
#define HWIO_MSS_CC_MODEM_GDSCR_IN                    \
                in_dword(HWIO_MSS_CC_MODEM_GDSCR_ADDR)
#define HWIO_MSS_CC_MODEM_GDSCR_INM(m)            \
                in_dword_masked(HWIO_MSS_CC_MODEM_GDSCR_ADDR, m)
#define HWIO_MSS_CC_MODEM_GDSCR_OUT(v)            \
                out_dword(HWIO_MSS_CC_MODEM_GDSCR_ADDR,v)
#define HWIO_MSS_CC_MODEM_GDSCR_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_CC_MODEM_GDSCR_ADDR,m,v,HWIO_MSS_CC_MODEM_GDSCR_IN)
#define HWIO_MSS_CC_MODEM_GDSCR_PWR_ON_BMSK                                                         0x80000000
#define HWIO_MSS_CC_MODEM_GDSCR_PWR_ON_SHFT                                                                 31
#define HWIO_MSS_CC_MODEM_GDSCR_GDSC_STATE_BMSK                                                     0x78000000
#define HWIO_MSS_CC_MODEM_GDSCR_GDSC_STATE_SHFT                                                             27
#define HWIO_MSS_CC_MODEM_GDSCR_EN_REST_WAIT_BMSK                                                     0xf00000
#define HWIO_MSS_CC_MODEM_GDSCR_EN_REST_WAIT_SHFT                                                           20
#define HWIO_MSS_CC_MODEM_GDSCR_EN_FEW_WAIT_BMSK                                                       0xf0000
#define HWIO_MSS_CC_MODEM_GDSCR_EN_FEW_WAIT_SHFT                                                            16
#define HWIO_MSS_CC_MODEM_GDSCR_CLK_DIS_WAIT_BMSK                                                       0xf000
#define HWIO_MSS_CC_MODEM_GDSCR_CLK_DIS_WAIT_SHFT                                                           12
#define HWIO_MSS_CC_MODEM_GDSCR_RETAIN_FF_ENABLE_BMSK                                                    0x800
#define HWIO_MSS_CC_MODEM_GDSCR_RETAIN_FF_ENABLE_SHFT                                                       11
#define HWIO_MSS_CC_MODEM_GDSCR_RESTORE_BMSK                                                             0x400
#define HWIO_MSS_CC_MODEM_GDSCR_RESTORE_SHFT                                                                10
#define HWIO_MSS_CC_MODEM_GDSCR_SAVE_BMSK                                                                0x200
#define HWIO_MSS_CC_MODEM_GDSCR_SAVE_SHFT                                                                    9
#define HWIO_MSS_CC_MODEM_GDSCR_RETAIN_BMSK                                                              0x100
#define HWIO_MSS_CC_MODEM_GDSCR_RETAIN_SHFT                                                                  8
#define HWIO_MSS_CC_MODEM_GDSCR_EN_REST_BMSK                                                              0x80
#define HWIO_MSS_CC_MODEM_GDSCR_EN_REST_SHFT                                                                 7
#define HWIO_MSS_CC_MODEM_GDSCR_EN_FEW_BMSK                                                               0x40
#define HWIO_MSS_CC_MODEM_GDSCR_EN_FEW_SHFT                                                                  6
#define HWIO_MSS_CC_MODEM_GDSCR_CLAMP_IO_BMSK                                                             0x20
#define HWIO_MSS_CC_MODEM_GDSCR_CLAMP_IO_SHFT                                                                5
#define HWIO_MSS_CC_MODEM_GDSCR_CLK_DISABLE_BMSK                                                          0x10
#define HWIO_MSS_CC_MODEM_GDSCR_CLK_DISABLE_SHFT                                                             4
#define HWIO_MSS_CC_MODEM_GDSCR_PD_ARES_BMSK                                                               0x8
#define HWIO_MSS_CC_MODEM_GDSCR_PD_ARES_SHFT                                                                 3
#define HWIO_MSS_CC_MODEM_GDSCR_SW_OVERRIDE_BMSK                                                           0x4
#define HWIO_MSS_CC_MODEM_GDSCR_SW_OVERRIDE_SHFT                                                             2
#define HWIO_MSS_CC_MODEM_GDSCR_HW_CONTROL_BMSK                                                            0x2
#define HWIO_MSS_CC_MODEM_GDSCR_HW_CONTROL_SHFT                                                              1
#define HWIO_MSS_CC_MODEM_GDSCR_SW_COLLAPSE_BMSK                                                           0x1
#define HWIO_MSS_CC_MODEM_GDSCR_SW_COLLAPSE_SHFT                                                             0

#define HWIO_MSS_CC_MODEM_CFG_GDSCR_ADDR                                                            (MSS_CC_MSS_CC_REG_REG_BASE            + 0x904)
#define HWIO_MSS_CC_MODEM_CFG_GDSCR_RMSK                                                             0x3ffffff
#define HWIO_MSS_CC_MODEM_CFG_GDSCR_IN                    \
                in_dword(HWIO_MSS_CC_MODEM_CFG_GDSCR_ADDR)
#define HWIO_MSS_CC_MODEM_CFG_GDSCR_INM(m)            \
                in_dword_masked(HWIO_MSS_CC_MODEM_CFG_GDSCR_ADDR, m)
#define HWIO_MSS_CC_MODEM_CFG_GDSCR_OUT(v)            \
                out_dword(HWIO_MSS_CC_MODEM_CFG_GDSCR_ADDR,v)
#define HWIO_MSS_CC_MODEM_CFG_GDSCR_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_CC_MODEM_CFG_GDSCR_ADDR,m,v,HWIO_MSS_CC_MODEM_CFG_GDSCR_IN)
#define HWIO_MSS_CC_MODEM_CFG_GDSCR_GDSC_PWR_DWN_START_BMSK                                          0x2000000
#define HWIO_MSS_CC_MODEM_CFG_GDSCR_GDSC_PWR_DWN_START_SHFT                                                 25
#define HWIO_MSS_CC_MODEM_CFG_GDSCR_GDSC_PWR_UP_START_BMSK                                           0x1000000
#define HWIO_MSS_CC_MODEM_CFG_GDSCR_GDSC_PWR_UP_START_SHFT                                                  24
#define HWIO_MSS_CC_MODEM_CFG_GDSCR_GDSC_CFG_FSM_STATE_STATUS_BMSK                                    0xf00000
#define HWIO_MSS_CC_MODEM_CFG_GDSCR_GDSC_CFG_FSM_STATE_STATUS_SHFT                                          20
#define HWIO_MSS_CC_MODEM_CFG_GDSCR_GDSC_MEM_PWR_ACK_STATUS_BMSK                                       0x80000
#define HWIO_MSS_CC_MODEM_CFG_GDSCR_GDSC_MEM_PWR_ACK_STATUS_SHFT                                            19
#define HWIO_MSS_CC_MODEM_CFG_GDSCR_GDSC_ENR_ACK_STATUS_BMSK                                           0x40000
#define HWIO_MSS_CC_MODEM_CFG_GDSCR_GDSC_ENR_ACK_STATUS_SHFT                                                18
#define HWIO_MSS_CC_MODEM_CFG_GDSCR_GDSC_ENF_ACK_STATUS_BMSK                                           0x20000
#define HWIO_MSS_CC_MODEM_CFG_GDSCR_GDSC_ENF_ACK_STATUS_SHFT                                                17
#define HWIO_MSS_CC_MODEM_CFG_GDSCR_GDSC_POWER_UP_COMPLETE_BMSK                                        0x10000
#define HWIO_MSS_CC_MODEM_CFG_GDSCR_GDSC_POWER_UP_COMPLETE_SHFT                                             16
#define HWIO_MSS_CC_MODEM_CFG_GDSCR_GDSC_POWER_DOWN_COMPLETE_BMSK                                       0x8000
#define HWIO_MSS_CC_MODEM_CFG_GDSCR_GDSC_POWER_DOWN_COMPLETE_SHFT                                           15
#define HWIO_MSS_CC_MODEM_CFG_GDSCR_SOFTWARE_CONTROL_OVERRIDE_BMSK                                      0x7800
#define HWIO_MSS_CC_MODEM_CFG_GDSCR_SOFTWARE_CONTROL_OVERRIDE_SHFT                                          11
#define HWIO_MSS_CC_MODEM_CFG_GDSCR_GDSC_HANDSHAKE_DIS_BMSK                                              0x400
#define HWIO_MSS_CC_MODEM_CFG_GDSCR_GDSC_HANDSHAKE_DIS_SHFT                                                 10
#define HWIO_MSS_CC_MODEM_CFG_GDSCR_GDSC_MEM_PERI_FORCE_IN_SW_BMSK                                       0x200
#define HWIO_MSS_CC_MODEM_CFG_GDSCR_GDSC_MEM_PERI_FORCE_IN_SW_SHFT                                           9
#define HWIO_MSS_CC_MODEM_CFG_GDSCR_GDSC_MEM_CORE_FORCE_IN_SW_BMSK                                       0x100
#define HWIO_MSS_CC_MODEM_CFG_GDSCR_GDSC_MEM_CORE_FORCE_IN_SW_SHFT                                           8
#define HWIO_MSS_CC_MODEM_CFG_GDSCR_GDSC_PHASE_RESET_EN_SW_BMSK                                           0x80
#define HWIO_MSS_CC_MODEM_CFG_GDSCR_GDSC_PHASE_RESET_EN_SW_SHFT                                              7
#define HWIO_MSS_CC_MODEM_CFG_GDSCR_GDSC_PHASE_RESET_DELAY_COUNT_SW_BMSK                                  0x60
#define HWIO_MSS_CC_MODEM_CFG_GDSCR_GDSC_PHASE_RESET_DELAY_COUNT_SW_SHFT                                     5
#define HWIO_MSS_CC_MODEM_CFG_GDSCR_GDSC_PSCBC_PWR_DWN_SW_BMSK                                            0x10
#define HWIO_MSS_CC_MODEM_CFG_GDSCR_GDSC_PSCBC_PWR_DWN_SW_SHFT                                               4
#define HWIO_MSS_CC_MODEM_CFG_GDSCR_UNCLAMP_IO_SOFTWARE_OVERRIDE_BMSK                                      0x8
#define HWIO_MSS_CC_MODEM_CFG_GDSCR_UNCLAMP_IO_SOFTWARE_OVERRIDE_SHFT                                        3
#define HWIO_MSS_CC_MODEM_CFG_GDSCR_SAVE_RESTORE_SOFTWARE_OVERRIDE_BMSK                                    0x4
#define HWIO_MSS_CC_MODEM_CFG_GDSCR_SAVE_RESTORE_SOFTWARE_OVERRIDE_SHFT                                      2
#define HWIO_MSS_CC_MODEM_CFG_GDSCR_CLAMP_IO_SOFTWARE_OVERRIDE_BMSK                                        0x2
#define HWIO_MSS_CC_MODEM_CFG_GDSCR_CLAMP_IO_SOFTWARE_OVERRIDE_SHFT                                          1
#define HWIO_MSS_CC_MODEM_CFG_GDSCR_DISABLE_CLK_SOFTWARE_OVERRIDE_BMSK                                     0x1
#define HWIO_MSS_CC_MODEM_CFG_GDSCR_DISABLE_CLK_SOFTWARE_OVERRIDE_SHFT                                       0

#define HWIO_MSS_CC_MODEM_CFG2_GDSCR_ADDR                                                           (MSS_CC_MSS_CC_REG_REG_BASE            + 0x908)
#define HWIO_MSS_CC_MODEM_CFG2_GDSCR_RMSK                                                              0x7ffff
#define HWIO_MSS_CC_MODEM_CFG2_GDSCR_IN                    \
                in_dword(HWIO_MSS_CC_MODEM_CFG2_GDSCR_ADDR)
#define HWIO_MSS_CC_MODEM_CFG2_GDSCR_INM(m)            \
                in_dword_masked(HWIO_MSS_CC_MODEM_CFG2_GDSCR_ADDR, m)
#define HWIO_MSS_CC_MODEM_CFG2_GDSCR_OUT(v)            \
                out_dword(HWIO_MSS_CC_MODEM_CFG2_GDSCR_ADDR,v)
#define HWIO_MSS_CC_MODEM_CFG2_GDSCR_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_CC_MODEM_CFG2_GDSCR_ADDR,m,v,HWIO_MSS_CC_MODEM_CFG2_GDSCR_IN)
#define HWIO_MSS_CC_MODEM_CFG2_GDSCR_GDSC_MEM_PWRUP_ACK_OVERRIDE_BMSK                                  0x40000
#define HWIO_MSS_CC_MODEM_CFG2_GDSCR_GDSC_MEM_PWRUP_ACK_OVERRIDE_SHFT                                       18
#define HWIO_MSS_CC_MODEM_CFG2_GDSCR_GDSC_PWRDWN_ENABLE_ACK_OVERRIDE_BMSK                              0x20000
#define HWIO_MSS_CC_MODEM_CFG2_GDSCR_GDSC_PWRDWN_ENABLE_ACK_OVERRIDE_SHFT                                   17
#define HWIO_MSS_CC_MODEM_CFG2_GDSCR_GDSC_CLAMP_MEM_SW_BMSK                                            0x10000
#define HWIO_MSS_CC_MODEM_CFG2_GDSCR_GDSC_CLAMP_MEM_SW_SHFT                                                 16
#define HWIO_MSS_CC_MODEM_CFG2_GDSCR_DLY_MEM_PWR_UP_BMSK                                                0xf000
#define HWIO_MSS_CC_MODEM_CFG2_GDSCR_DLY_MEM_PWR_UP_SHFT                                                    12
#define HWIO_MSS_CC_MODEM_CFG2_GDSCR_DLY_DEASSERT_CLAMP_MEM_BMSK                                         0xf00
#define HWIO_MSS_CC_MODEM_CFG2_GDSCR_DLY_DEASSERT_CLAMP_MEM_SHFT                                             8
#define HWIO_MSS_CC_MODEM_CFG2_GDSCR_DLY_ASSERT_CLAMP_MEM_BMSK                                            0xf0
#define HWIO_MSS_CC_MODEM_CFG2_GDSCR_DLY_ASSERT_CLAMP_MEM_SHFT                                               4
#define HWIO_MSS_CC_MODEM_CFG2_GDSCR_MEM_PWR_DWN_TIMEOUT_BMSK                                              0xf
#define HWIO_MSS_CC_MODEM_CFG2_GDSCR_MEM_PWR_DWN_TIMEOUT_SHFT                                                0

#define HWIO_MSS_CC_MODEM_CFG3_GDSCR_ADDR                                                           (MSS_CC_MSS_CC_REG_REG_BASE            + 0x90c)
#define HWIO_MSS_CC_MODEM_CFG3_GDSCR_RMSK                                                            0x7ffffff
#define HWIO_MSS_CC_MODEM_CFG3_GDSCR_IN                    \
                in_dword(HWIO_MSS_CC_MODEM_CFG3_GDSCR_ADDR)
#define HWIO_MSS_CC_MODEM_CFG3_GDSCR_INM(m)            \
                in_dword_masked(HWIO_MSS_CC_MODEM_CFG3_GDSCR_ADDR, m)
#define HWIO_MSS_CC_MODEM_CFG3_GDSCR_OUT(v)            \
                out_dword(HWIO_MSS_CC_MODEM_CFG3_GDSCR_ADDR,v)
#define HWIO_MSS_CC_MODEM_CFG3_GDSCR_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_CC_MODEM_CFG3_GDSCR_ADDR,m,v,HWIO_MSS_CC_MODEM_CFG3_GDSCR_IN)
#define HWIO_MSS_CC_MODEM_CFG3_GDSCR_GDSC_ACCU_RED_SHIFTER_DONE_STATUS_BMSK                          0x4000000
#define HWIO_MSS_CC_MODEM_CFG3_GDSCR_GDSC_ACCU_RED_SHIFTER_DONE_STATUS_SHFT                                 26
#define HWIO_MSS_CC_MODEM_CFG3_GDSCR_GDSC_ACCU_RED_ENABLE_BMSK                                       0x2000000
#define HWIO_MSS_CC_MODEM_CFG3_GDSCR_GDSC_ACCU_RED_ENABLE_SHFT                                              25
#define HWIO_MSS_CC_MODEM_CFG3_GDSCR_DLY_ACCU_RED_SHIFTER_DONE_BMSK                                  0x1e00000
#define HWIO_MSS_CC_MODEM_CFG3_GDSCR_DLY_ACCU_RED_SHIFTER_DONE_SHFT                                         21
#define HWIO_MSS_CC_MODEM_CFG3_GDSCR_GDSC_ACCU_RED_TIMER_EN_SW_BMSK                                   0x100000
#define HWIO_MSS_CC_MODEM_CFG3_GDSCR_GDSC_ACCU_RED_TIMER_EN_SW_SHFT                                         20
#define HWIO_MSS_CC_MODEM_CFG3_GDSCR_GDSC_ACCU_RED_SHIFTER_DONE_OVERRIDE_BMSK                          0x80000
#define HWIO_MSS_CC_MODEM_CFG3_GDSCR_GDSC_ACCU_RED_SHIFTER_DONE_OVERRIDE_SHFT                               19
#define HWIO_MSS_CC_MODEM_CFG3_GDSCR_GDSC_ACCU_RED_SHIFTER_CLK_EN_SW_BMSK                              0x40000
#define HWIO_MSS_CC_MODEM_CFG3_GDSCR_GDSC_ACCU_RED_SHIFTER_CLK_EN_SW_SHFT                                   18
#define HWIO_MSS_CC_MODEM_CFG3_GDSCR_GDSC_ACCU_RED_SHIFTER_START_SW_BMSK                               0x20000
#define HWIO_MSS_CC_MODEM_CFG3_GDSCR_GDSC_ACCU_RED_SHIFTER_START_SW_SHFT                                    17
#define HWIO_MSS_CC_MODEM_CFG3_GDSCR_GDSC_ACCU_RED_SW_OVERRIDE_BMSK                                    0x10000
#define HWIO_MSS_CC_MODEM_CFG3_GDSCR_GDSC_ACCU_RED_SW_OVERRIDE_SHFT                                         16
#define HWIO_MSS_CC_MODEM_CFG3_GDSCR_GDSC_SPARE_CTRL_IN_BMSK                                            0xff00
#define HWIO_MSS_CC_MODEM_CFG3_GDSCR_GDSC_SPARE_CTRL_IN_SHFT                                                 8
#define HWIO_MSS_CC_MODEM_CFG3_GDSCR_GDSC_SPARE_CTRL_OUT_BMSK                                             0xff
#define HWIO_MSS_CC_MODEM_CFG3_GDSCR_GDSC_SPARE_CTRL_OUT_SHFT                                                0

#define HWIO_MSS_CC_MODEM_CFG4_GDSCR_ADDR                                                           (MSS_CC_MSS_CC_REG_REG_BASE            + 0x910)
#define HWIO_MSS_CC_MODEM_CFG4_GDSCR_RMSK                                                             0xffffff
#define HWIO_MSS_CC_MODEM_CFG4_GDSCR_IN                    \
                in_dword(HWIO_MSS_CC_MODEM_CFG4_GDSCR_ADDR)
#define HWIO_MSS_CC_MODEM_CFG4_GDSCR_INM(m)            \
                in_dword_masked(HWIO_MSS_CC_MODEM_CFG4_GDSCR_ADDR, m)
#define HWIO_MSS_CC_MODEM_CFG4_GDSCR_OUT(v)            \
                out_dword(HWIO_MSS_CC_MODEM_CFG4_GDSCR_ADDR,v)
#define HWIO_MSS_CC_MODEM_CFG4_GDSCR_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_CC_MODEM_CFG4_GDSCR_ADDR,m,v,HWIO_MSS_CC_MODEM_CFG4_GDSCR_IN)
#define HWIO_MSS_CC_MODEM_CFG4_GDSCR_DLY_UNCLAMPIO_BMSK                                               0xf00000
#define HWIO_MSS_CC_MODEM_CFG4_GDSCR_DLY_UNCLAMPIO_SHFT                                                     20
#define HWIO_MSS_CC_MODEM_CFG4_GDSCR_DLY_RESTOREFF_BMSK                                                0xf0000
#define HWIO_MSS_CC_MODEM_CFG4_GDSCR_DLY_RESTOREFF_SHFT                                                     16
#define HWIO_MSS_CC_MODEM_CFG4_GDSCR_DLY_NORETAINFF_BMSK                                                0xf000
#define HWIO_MSS_CC_MODEM_CFG4_GDSCR_DLY_NORETAINFF_SHFT                                                    12
#define HWIO_MSS_CC_MODEM_CFG4_GDSCR_DLY_DEASSERTARES_BMSK                                               0xf00
#define HWIO_MSS_CC_MODEM_CFG4_GDSCR_DLY_DEASSERTARES_SHFT                                                   8
#define HWIO_MSS_CC_MODEM_CFG4_GDSCR_DLY_CLAMPIO_BMSK                                                     0xf0
#define HWIO_MSS_CC_MODEM_CFG4_GDSCR_DLY_CLAMPIO_SHFT                                                        4
#define HWIO_MSS_CC_MODEM_CFG4_GDSCR_DLY_RETAINFF_BMSK                                                     0xf
#define HWIO_MSS_CC_MODEM_CFG4_GDSCR_DLY_RETAINFF_SHFT                                                       0

#define HWIO_MSS_CC_Q6_AXIS_CBCR_ADDR                                                               (MSS_CC_MSS_CC_REG_REG_BASE            + 0x99c)
#define HWIO_MSS_CC_Q6_AXIS_CBCR_RMSK                                                               0x81c0000f
#define HWIO_MSS_CC_Q6_AXIS_CBCR_IN                    \
                in_dword(HWIO_MSS_CC_Q6_AXIS_CBCR_ADDR)
#define HWIO_MSS_CC_Q6_AXIS_CBCR_INM(m)            \
                in_dword_masked(HWIO_MSS_CC_Q6_AXIS_CBCR_ADDR, m)
#define HWIO_MSS_CC_Q6_AXIS_CBCR_OUT(v)            \
                out_dword(HWIO_MSS_CC_Q6_AXIS_CBCR_ADDR,v)
#define HWIO_MSS_CC_Q6_AXIS_CBCR_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_CC_Q6_AXIS_CBCR_ADDR,m,v,HWIO_MSS_CC_Q6_AXIS_CBCR_IN)
#define HWIO_MSS_CC_Q6_AXIS_CBCR_CLK_OFF_BMSK                                                       0x80000000
#define HWIO_MSS_CC_Q6_AXIS_CBCR_CLK_OFF_SHFT                                                               31
#define HWIO_MSS_CC_Q6_AXIS_CBCR_IGNORE_ALL_ARES_BMSK                                                0x1000000
#define HWIO_MSS_CC_Q6_AXIS_CBCR_IGNORE_ALL_ARES_SHFT                                                       24
#define HWIO_MSS_CC_Q6_AXIS_CBCR_IGNORE_ALL_CLK_DIS_BMSK                                              0x800000
#define HWIO_MSS_CC_Q6_AXIS_CBCR_IGNORE_ALL_CLK_DIS_SHFT                                                    23
#define HWIO_MSS_CC_Q6_AXIS_CBCR_CLK_DIS_BMSK                                                         0x400000
#define HWIO_MSS_CC_Q6_AXIS_CBCR_CLK_DIS_SHFT                                                               22
#define HWIO_MSS_CC_Q6_AXIS_CBCR_SW_ONLY_EN_BMSK                                                           0x8
#define HWIO_MSS_CC_Q6_AXIS_CBCR_SW_ONLY_EN_SHFT                                                             3
#define HWIO_MSS_CC_Q6_AXIS_CBCR_CLK_ARES_BMSK                                                             0x4
#define HWIO_MSS_CC_Q6_AXIS_CBCR_CLK_ARES_SHFT                                                               2
#define HWIO_MSS_CC_Q6_AXIS_CBCR_HW_CTL_BMSK                                                               0x2
#define HWIO_MSS_CC_Q6_AXIS_CBCR_HW_CTL_SHFT                                                                 1
#define HWIO_MSS_CC_Q6_AXIS_CBCR_CLK_ENABLE_BMSK                                                           0x1
#define HWIO_MSS_CC_Q6_AXIS_CBCR_CLK_ENABLE_SHFT                                                             0

#define HWIO_MSS_CC_MSS_CLOCK_SPDM_MON_ADDR                                                         (MSS_CC_MSS_CC_REG_REG_BASE            + 0x1388)
#define HWIO_MSS_CC_MSS_CLOCK_SPDM_MON_RMSK                                                                0x3
#define HWIO_MSS_CC_MSS_CLOCK_SPDM_MON_IN                    \
                in_dword(HWIO_MSS_CC_MSS_CLOCK_SPDM_MON_ADDR)
#define HWIO_MSS_CC_MSS_CLOCK_SPDM_MON_INM(m)            \
                in_dword_masked(HWIO_MSS_CC_MSS_CLOCK_SPDM_MON_ADDR, m)
#define HWIO_MSS_CC_MSS_CLOCK_SPDM_MON_OUT(v)            \
                out_dword(HWIO_MSS_CC_MSS_CLOCK_SPDM_MON_ADDR,v)
#define HWIO_MSS_CC_MSS_CLOCK_SPDM_MON_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_CC_MSS_CLOCK_SPDM_MON_ADDR,m,v,HWIO_MSS_CC_MSS_CLOCK_SPDM_MON_IN)
#define HWIO_MSS_CC_MSS_CLOCK_SPDM_MON_Q6_MON_CLKEN_BMSK                                                   0x2
#define HWIO_MSS_CC_MSS_CLOCK_SPDM_MON_Q6_MON_CLKEN_SHFT                                                     1
#define HWIO_MSS_CC_MSS_CLOCK_SPDM_MON_BUS_MON_CLKEN_BMSK                                                  0x1
#define HWIO_MSS_CC_MSS_CLOCK_SPDM_MON_BUS_MON_CLKEN_SHFT                                                    0

#define HWIO_MSS_CC_MSS_BBRX0_MUX_SEL_ADDR                                                          (MSS_CC_MSS_CC_REG_REG_BASE            + 0x138c)
#define HWIO_MSS_CC_MSS_BBRX0_MUX_SEL_RMSK                                                                 0x3
#define HWIO_MSS_CC_MSS_BBRX0_MUX_SEL_IN                    \
                in_dword(HWIO_MSS_CC_MSS_BBRX0_MUX_SEL_ADDR)
#define HWIO_MSS_CC_MSS_BBRX0_MUX_SEL_INM(m)            \
                in_dword_masked(HWIO_MSS_CC_MSS_BBRX0_MUX_SEL_ADDR, m)
#define HWIO_MSS_CC_MSS_BBRX0_MUX_SEL_OUT(v)            \
                out_dword(HWIO_MSS_CC_MSS_BBRX0_MUX_SEL_ADDR,v)
#define HWIO_MSS_CC_MSS_BBRX0_MUX_SEL_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_CC_MSS_BBRX0_MUX_SEL_ADDR,m,v,HWIO_MSS_CC_MSS_BBRX0_MUX_SEL_IN)
#define HWIO_MSS_CC_MSS_BBRX0_MUX_SEL_SECOND_MUX_SEL_BMSK                                                  0x2
#define HWIO_MSS_CC_MSS_BBRX0_MUX_SEL_SECOND_MUX_SEL_SHFT                                                    1
#define HWIO_MSS_CC_MSS_BBRX0_MUX_SEL_FIRST_MUX_SEL_BMSK                                                   0x1
#define HWIO_MSS_CC_MSS_BBRX0_MUX_SEL_FIRST_MUX_SEL_SHFT                                                     0

#define HWIO_MSS_CC_MSS_BBRX1_MUX_SEL_ADDR                                                          (MSS_CC_MSS_CC_REG_REG_BASE            + 0x1390)
#define HWIO_MSS_CC_MSS_BBRX1_MUX_SEL_RMSK                                                                 0x3
#define HWIO_MSS_CC_MSS_BBRX1_MUX_SEL_IN                    \
                in_dword(HWIO_MSS_CC_MSS_BBRX1_MUX_SEL_ADDR)
#define HWIO_MSS_CC_MSS_BBRX1_MUX_SEL_INM(m)            \
                in_dword_masked(HWIO_MSS_CC_MSS_BBRX1_MUX_SEL_ADDR, m)
#define HWIO_MSS_CC_MSS_BBRX1_MUX_SEL_OUT(v)            \
                out_dword(HWIO_MSS_CC_MSS_BBRX1_MUX_SEL_ADDR,v)
#define HWIO_MSS_CC_MSS_BBRX1_MUX_SEL_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_CC_MSS_BBRX1_MUX_SEL_ADDR,m,v,HWIO_MSS_CC_MSS_BBRX1_MUX_SEL_IN)
#define HWIO_MSS_CC_MSS_BBRX1_MUX_SEL_SECOND_MUX_SEL_BMSK                                                  0x2
#define HWIO_MSS_CC_MSS_BBRX1_MUX_SEL_SECOND_MUX_SEL_SHFT                                                    1
#define HWIO_MSS_CC_MSS_BBRX1_MUX_SEL_FIRST_MUX_SEL_BMSK                                                   0x1
#define HWIO_MSS_CC_MSS_BBRX1_MUX_SEL_FIRST_MUX_SEL_SHFT                                                     0

#define HWIO_MSS_CC_MSS_BBRX2_MUX_SEL_ADDR                                                          (MSS_CC_MSS_CC_REG_REG_BASE            + 0x1394)
#define HWIO_MSS_CC_MSS_BBRX2_MUX_SEL_RMSK                                                                 0x3
#define HWIO_MSS_CC_MSS_BBRX2_MUX_SEL_IN                    \
                in_dword(HWIO_MSS_CC_MSS_BBRX2_MUX_SEL_ADDR)
#define HWIO_MSS_CC_MSS_BBRX2_MUX_SEL_INM(m)            \
                in_dword_masked(HWIO_MSS_CC_MSS_BBRX2_MUX_SEL_ADDR, m)
#define HWIO_MSS_CC_MSS_BBRX2_MUX_SEL_OUT(v)            \
                out_dword(HWIO_MSS_CC_MSS_BBRX2_MUX_SEL_ADDR,v)
#define HWIO_MSS_CC_MSS_BBRX2_MUX_SEL_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_CC_MSS_BBRX2_MUX_SEL_ADDR,m,v,HWIO_MSS_CC_MSS_BBRX2_MUX_SEL_IN)
#define HWIO_MSS_CC_MSS_BBRX2_MUX_SEL_SECOND_MUX_SEL_BMSK                                                  0x2
#define HWIO_MSS_CC_MSS_BBRX2_MUX_SEL_SECOND_MUX_SEL_SHFT                                                    1
#define HWIO_MSS_CC_MSS_BBRX2_MUX_SEL_FIRST_MUX_SEL_BMSK                                                   0x1
#define HWIO_MSS_CC_MSS_BBRX2_MUX_SEL_FIRST_MUX_SEL_SHFT                                                     0

#define HWIO_MSS_CC_MSS_BBRX_EXT_CLOCK_MUX_SEL_ADDR                                                 (MSS_CC_MSS_CC_REG_REG_BASE            + 0x13a4)
#define HWIO_MSS_CC_MSS_BBRX_EXT_CLOCK_MUX_SEL_RMSK                                                        0x1
#define HWIO_MSS_CC_MSS_BBRX_EXT_CLOCK_MUX_SEL_IN                    \
                in_dword(HWIO_MSS_CC_MSS_BBRX_EXT_CLOCK_MUX_SEL_ADDR)
#define HWIO_MSS_CC_MSS_BBRX_EXT_CLOCK_MUX_SEL_INM(m)            \
                in_dword_masked(HWIO_MSS_CC_MSS_BBRX_EXT_CLOCK_MUX_SEL_ADDR, m)
#define HWIO_MSS_CC_MSS_BBRX_EXT_CLOCK_MUX_SEL_OUT(v)            \
                out_dword(HWIO_MSS_CC_MSS_BBRX_EXT_CLOCK_MUX_SEL_ADDR,v)
#define HWIO_MSS_CC_MSS_BBRX_EXT_CLOCK_MUX_SEL_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_CC_MSS_BBRX_EXT_CLOCK_MUX_SEL_ADDR,m,v,HWIO_MSS_CC_MSS_BBRX_EXT_CLOCK_MUX_SEL_IN)
#define HWIO_MSS_CC_MSS_BBRX_EXT_CLOCK_MUX_SEL_PLLTEST_BMSK                                                0x1
#define HWIO_MSS_CC_MSS_BBRX_EXT_CLOCK_MUX_SEL_PLLTEST_SHFT                                                  0

#define HWIO_MSS_CC_MSS_MPLL1_EARLY_DIV5_CBCR_ADDR                                                  (MSS_CC_MSS_CC_REG_REG_BASE            + 0x13c8)
#define HWIO_MSS_CC_MSS_MPLL1_EARLY_DIV5_CBCR_RMSK                                                  0x80000001
#define HWIO_MSS_CC_MSS_MPLL1_EARLY_DIV5_CBCR_IN                    \
                in_dword(HWIO_MSS_CC_MSS_MPLL1_EARLY_DIV5_CBCR_ADDR)
#define HWIO_MSS_CC_MSS_MPLL1_EARLY_DIV5_CBCR_INM(m)            \
                in_dword_masked(HWIO_MSS_CC_MSS_MPLL1_EARLY_DIV5_CBCR_ADDR, m)
#define HWIO_MSS_CC_MSS_MPLL1_EARLY_DIV5_CBCR_OUT(v)            \
                out_dword(HWIO_MSS_CC_MSS_MPLL1_EARLY_DIV5_CBCR_ADDR,v)
#define HWIO_MSS_CC_MSS_MPLL1_EARLY_DIV5_CBCR_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_CC_MSS_MPLL1_EARLY_DIV5_CBCR_ADDR,m,v,HWIO_MSS_CC_MSS_MPLL1_EARLY_DIV5_CBCR_IN)
#define HWIO_MSS_CC_MSS_MPLL1_EARLY_DIV5_CBCR_CLKOFF_BMSK                                           0x80000000
#define HWIO_MSS_CC_MSS_MPLL1_EARLY_DIV5_CBCR_CLKOFF_SHFT                                                   31
#define HWIO_MSS_CC_MSS_MPLL1_EARLY_DIV5_CBCR_CLKEN_BMSK                                                   0x1
#define HWIO_MSS_CC_MSS_MPLL1_EARLY_DIV5_CBCR_CLKEN_SHFT                                                     0

#define HWIO_MSS_CC_MSS_MPLL1_EARLY_DIV3_CBCR_ADDR                                                  (MSS_CC_MSS_CC_REG_REG_BASE            + 0x13cc)
#define HWIO_MSS_CC_MSS_MPLL1_EARLY_DIV3_CBCR_RMSK                                                  0x80000001
#define HWIO_MSS_CC_MSS_MPLL1_EARLY_DIV3_CBCR_IN                    \
                in_dword(HWIO_MSS_CC_MSS_MPLL1_EARLY_DIV3_CBCR_ADDR)
#define HWIO_MSS_CC_MSS_MPLL1_EARLY_DIV3_CBCR_INM(m)            \
                in_dword_masked(HWIO_MSS_CC_MSS_MPLL1_EARLY_DIV3_CBCR_ADDR, m)
#define HWIO_MSS_CC_MSS_MPLL1_EARLY_DIV3_CBCR_OUT(v)            \
                out_dword(HWIO_MSS_CC_MSS_MPLL1_EARLY_DIV3_CBCR_ADDR,v)
#define HWIO_MSS_CC_MSS_MPLL1_EARLY_DIV3_CBCR_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_CC_MSS_MPLL1_EARLY_DIV3_CBCR_ADDR,m,v,HWIO_MSS_CC_MSS_MPLL1_EARLY_DIV3_CBCR_IN)
#define HWIO_MSS_CC_MSS_MPLL1_EARLY_DIV3_CBCR_CLKOFF_BMSK                                           0x80000000
#define HWIO_MSS_CC_MSS_MPLL1_EARLY_DIV3_CBCR_CLKOFF_SHFT                                                   31
#define HWIO_MSS_CC_MSS_MPLL1_EARLY_DIV3_CBCR_CLKEN_BMSK                                                   0x1
#define HWIO_MSS_CC_MSS_MPLL1_EARLY_DIV3_CBCR_CLKEN_SHFT                                                     0

#define HWIO_MSS_CC_MSS_MPLL1_OUT_EARLY_DIV3_CBCR_ADDR                                              (MSS_CC_MSS_CC_REG_REG_BASE            + 0x13d0)
#define HWIO_MSS_CC_MSS_MPLL1_OUT_EARLY_DIV3_CBCR_RMSK                                              0x80000001
#define HWIO_MSS_CC_MSS_MPLL1_OUT_EARLY_DIV3_CBCR_IN                    \
                in_dword(HWIO_MSS_CC_MSS_MPLL1_OUT_EARLY_DIV3_CBCR_ADDR)
#define HWIO_MSS_CC_MSS_MPLL1_OUT_EARLY_DIV3_CBCR_INM(m)            \
                in_dword_masked(HWIO_MSS_CC_MSS_MPLL1_OUT_EARLY_DIV3_CBCR_ADDR, m)
#define HWIO_MSS_CC_MSS_MPLL1_OUT_EARLY_DIV3_CBCR_OUT(v)            \
                out_dword(HWIO_MSS_CC_MSS_MPLL1_OUT_EARLY_DIV3_CBCR_ADDR,v)
#define HWIO_MSS_CC_MSS_MPLL1_OUT_EARLY_DIV3_CBCR_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_CC_MSS_MPLL1_OUT_EARLY_DIV3_CBCR_ADDR,m,v,HWIO_MSS_CC_MSS_MPLL1_OUT_EARLY_DIV3_CBCR_IN)
#define HWIO_MSS_CC_MSS_MPLL1_OUT_EARLY_DIV3_CBCR_CLKOFF_BMSK                                       0x80000000
#define HWIO_MSS_CC_MSS_MPLL1_OUT_EARLY_DIV3_CBCR_CLKOFF_SHFT                                               31
#define HWIO_MSS_CC_MSS_MPLL1_OUT_EARLY_DIV3_CBCR_CLKEN_BMSK                                               0x1
#define HWIO_MSS_CC_MSS_MPLL1_OUT_EARLY_DIV3_CBCR_CLKEN_SHFT                                                 0

#define HWIO_MSS_CC_MSS_MPLL1_OUT_EARLY_DIV5_CBCR_ADDR                                              (MSS_CC_MSS_CC_REG_REG_BASE            + 0x13d4)
#define HWIO_MSS_CC_MSS_MPLL1_OUT_EARLY_DIV5_CBCR_RMSK                                              0x80000001
#define HWIO_MSS_CC_MSS_MPLL1_OUT_EARLY_DIV5_CBCR_IN                    \
                in_dword(HWIO_MSS_CC_MSS_MPLL1_OUT_EARLY_DIV5_CBCR_ADDR)
#define HWIO_MSS_CC_MSS_MPLL1_OUT_EARLY_DIV5_CBCR_INM(m)            \
                in_dword_masked(HWIO_MSS_CC_MSS_MPLL1_OUT_EARLY_DIV5_CBCR_ADDR, m)
#define HWIO_MSS_CC_MSS_MPLL1_OUT_EARLY_DIV5_CBCR_OUT(v)            \
                out_dword(HWIO_MSS_CC_MSS_MPLL1_OUT_EARLY_DIV5_CBCR_ADDR,v)
#define HWIO_MSS_CC_MSS_MPLL1_OUT_EARLY_DIV5_CBCR_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_CC_MSS_MPLL1_OUT_EARLY_DIV5_CBCR_ADDR,m,v,HWIO_MSS_CC_MSS_MPLL1_OUT_EARLY_DIV5_CBCR_IN)
#define HWIO_MSS_CC_MSS_MPLL1_OUT_EARLY_DIV5_CBCR_CLKOFF_BMSK                                       0x80000000
#define HWIO_MSS_CC_MSS_MPLL1_OUT_EARLY_DIV5_CBCR_CLKOFF_SHFT                                               31
#define HWIO_MSS_CC_MSS_MPLL1_OUT_EARLY_DIV5_CBCR_CLKEN_BMSK                                               0x1
#define HWIO_MSS_CC_MSS_MPLL1_OUT_EARLY_DIV5_CBCR_CLKEN_SHFT                                                 0

#define HWIO_MSS_CC_MSS_BBRX0_CBCR_ADDR                                                             (MSS_CC_MSS_CC_REG_REG_BASE            + 0x13d8)
#define HWIO_MSS_CC_MSS_BBRX0_CBCR_RMSK                                                             0x80000000
#define HWIO_MSS_CC_MSS_BBRX0_CBCR_IN                    \
                in_dword(HWIO_MSS_CC_MSS_BBRX0_CBCR_ADDR)
#define HWIO_MSS_CC_MSS_BBRX0_CBCR_INM(m)            \
                in_dword_masked(HWIO_MSS_CC_MSS_BBRX0_CBCR_ADDR, m)
#define HWIO_MSS_CC_MSS_BBRX0_CBCR_CLKOFF_BMSK                                                      0x80000000
#define HWIO_MSS_CC_MSS_BBRX0_CBCR_CLKOFF_SHFT                                                              31

#define HWIO_MSS_CC_MSS_BBRX1_CBCR_ADDR                                                             (MSS_CC_MSS_CC_REG_REG_BASE            + 0x13dc)
#define HWIO_MSS_CC_MSS_BBRX1_CBCR_RMSK                                                             0x80000000
#define HWIO_MSS_CC_MSS_BBRX1_CBCR_IN                    \
                in_dword(HWIO_MSS_CC_MSS_BBRX1_CBCR_ADDR)
#define HWIO_MSS_CC_MSS_BBRX1_CBCR_INM(m)            \
                in_dword_masked(HWIO_MSS_CC_MSS_BBRX1_CBCR_ADDR, m)
#define HWIO_MSS_CC_MSS_BBRX1_CBCR_CLKOFF_BMSK                                                      0x80000000
#define HWIO_MSS_CC_MSS_BBRX1_CBCR_CLKOFF_SHFT                                                              31

#define HWIO_MSS_CC_MSS_BBRX2_CBCR_ADDR                                                             (MSS_CC_MSS_CC_REG_REG_BASE            + 0x13e0)
#define HWIO_MSS_CC_MSS_BBRX2_CBCR_RMSK                                                             0x80000000
#define HWIO_MSS_CC_MSS_BBRX2_CBCR_IN                    \
                in_dword(HWIO_MSS_CC_MSS_BBRX2_CBCR_ADDR)
#define HWIO_MSS_CC_MSS_BBRX2_CBCR_INM(m)            \
                in_dword_masked(HWIO_MSS_CC_MSS_BBRX2_CBCR_ADDR, m)
#define HWIO_MSS_CC_MSS_BBRX2_CBCR_CLKOFF_BMSK                                                      0x80000000
#define HWIO_MSS_CC_MSS_BBRX2_CBCR_CLKOFF_SHFT                                                              31

#define HWIO_MSS_CC_MSS_RESERVE_02_ADDR                                                             (MSS_CC_MSS_CC_REG_REG_BASE            + 0x13f0)
#define HWIO_MSS_CC_MSS_RESERVE_02_RMSK                                                             0xffffffff
#define HWIO_MSS_CC_MSS_RESERVE_02_IN                    \
                in_dword(HWIO_MSS_CC_MSS_RESERVE_02_ADDR)
#define HWIO_MSS_CC_MSS_RESERVE_02_INM(m)            \
                in_dword_masked(HWIO_MSS_CC_MSS_RESERVE_02_ADDR, m)
#define HWIO_MSS_CC_MSS_RESERVE_02_OUT(v)            \
                out_dword(HWIO_MSS_CC_MSS_RESERVE_02_ADDR,v)
#define HWIO_MSS_CC_MSS_RESERVE_02_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_CC_MSS_RESERVE_02_ADDR,m,v,HWIO_MSS_CC_MSS_RESERVE_02_IN)
#define HWIO_MSS_CC_MSS_RESERVE_02_MSS_RESERVE_02_BMSK                                              0xfffffffe
#define HWIO_MSS_CC_MSS_RESERVE_02_MSS_RESERVE_02_SHFT                                                       1
#define HWIO_MSS_CC_MSS_RESERVE_02_SWITCH_Q6_AXIS_CLK_BMSK                                                 0x1
#define HWIO_MSS_CC_MSS_RESERVE_02_SWITCH_Q6_AXIS_CLK_SHFT                                                   0

#define HWIO_MSS_CC_MSS_BBRX0_MISC_ADDR                                                             (MSS_CC_MSS_CC_REG_REG_BASE            + 0x13f4)
#define HWIO_MSS_CC_MSS_BBRX0_MISC_RMSK                                                                    0xf
#define HWIO_MSS_CC_MSS_BBRX0_MISC_IN                    \
                in_dword(HWIO_MSS_CC_MSS_BBRX0_MISC_ADDR)
#define HWIO_MSS_CC_MSS_BBRX0_MISC_INM(m)            \
                in_dword_masked(HWIO_MSS_CC_MSS_BBRX0_MISC_ADDR, m)
#define HWIO_MSS_CC_MSS_BBRX0_MISC_OUT(v)            \
                out_dword(HWIO_MSS_CC_MSS_BBRX0_MISC_ADDR,v)
#define HWIO_MSS_CC_MSS_BBRX0_MISC_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_CC_MSS_BBRX0_MISC_ADDR,m,v,HWIO_MSS_CC_MSS_BBRX0_MISC_IN)
#define HWIO_MSS_CC_MSS_BBRX0_MISC_SRC_DIV_BMSK                                                            0xf
#define HWIO_MSS_CC_MSS_BBRX0_MISC_SRC_DIV_SHFT                                                              0

#define HWIO_MSS_CC_MSS_BBRX1_MISC_ADDR                                                             (MSS_CC_MSS_CC_REG_REG_BASE            + 0x13f8)
#define HWIO_MSS_CC_MSS_BBRX1_MISC_RMSK                                                                    0xf
#define HWIO_MSS_CC_MSS_BBRX1_MISC_IN                    \
                in_dword(HWIO_MSS_CC_MSS_BBRX1_MISC_ADDR)
#define HWIO_MSS_CC_MSS_BBRX1_MISC_INM(m)            \
                in_dword_masked(HWIO_MSS_CC_MSS_BBRX1_MISC_ADDR, m)
#define HWIO_MSS_CC_MSS_BBRX1_MISC_OUT(v)            \
                out_dword(HWIO_MSS_CC_MSS_BBRX1_MISC_ADDR,v)
#define HWIO_MSS_CC_MSS_BBRX1_MISC_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_CC_MSS_BBRX1_MISC_ADDR,m,v,HWIO_MSS_CC_MSS_BBRX1_MISC_IN)
#define HWIO_MSS_CC_MSS_BBRX1_MISC_SRC_DIV_BMSK                                                            0xf
#define HWIO_MSS_CC_MSS_BBRX1_MISC_SRC_DIV_SHFT                                                              0

#define HWIO_MSS_CC_MSS_BBRX2_MISC_ADDR                                                             (MSS_CC_MSS_CC_REG_REG_BASE            + 0x13fc)
#define HWIO_MSS_CC_MSS_BBRX2_MISC_RMSK                                                                    0xf
#define HWIO_MSS_CC_MSS_BBRX2_MISC_IN                    \
                in_dword(HWIO_MSS_CC_MSS_BBRX2_MISC_ADDR)
#define HWIO_MSS_CC_MSS_BBRX2_MISC_INM(m)            \
                in_dword_masked(HWIO_MSS_CC_MSS_BBRX2_MISC_ADDR, m)
#define HWIO_MSS_CC_MSS_BBRX2_MISC_OUT(v)            \
                out_dword(HWIO_MSS_CC_MSS_BBRX2_MISC_ADDR,v)
#define HWIO_MSS_CC_MSS_BBRX2_MISC_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_CC_MSS_BBRX2_MISC_ADDR,m,v,HWIO_MSS_CC_MSS_BBRX2_MISC_IN)
#define HWIO_MSS_CC_MSS_BBRX2_MISC_SRC_DIV_BMSK                                                            0xf
#define HWIO_MSS_CC_MSS_BBRX2_MISC_SRC_DIV_SHFT                                                              0

#define HWIO_MSS_CC_MSS_MPLL1_EARLY_DIV2_CBCR_ADDR                                                  (MSS_CC_MSS_CC_REG_REG_BASE            + 0x140c)
#define HWIO_MSS_CC_MSS_MPLL1_EARLY_DIV2_CBCR_RMSK                                                  0x80000001
#define HWIO_MSS_CC_MSS_MPLL1_EARLY_DIV2_CBCR_IN                    \
                in_dword(HWIO_MSS_CC_MSS_MPLL1_EARLY_DIV2_CBCR_ADDR)
#define HWIO_MSS_CC_MSS_MPLL1_EARLY_DIV2_CBCR_INM(m)            \
                in_dword_masked(HWIO_MSS_CC_MSS_MPLL1_EARLY_DIV2_CBCR_ADDR, m)
#define HWIO_MSS_CC_MSS_MPLL1_EARLY_DIV2_CBCR_OUT(v)            \
                out_dword(HWIO_MSS_CC_MSS_MPLL1_EARLY_DIV2_CBCR_ADDR,v)
#define HWIO_MSS_CC_MSS_MPLL1_EARLY_DIV2_CBCR_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_CC_MSS_MPLL1_EARLY_DIV2_CBCR_ADDR,m,v,HWIO_MSS_CC_MSS_MPLL1_EARLY_DIV2_CBCR_IN)
#define HWIO_MSS_CC_MSS_MPLL1_EARLY_DIV2_CBCR_CLKOFF_BMSK                                           0x80000000
#define HWIO_MSS_CC_MSS_MPLL1_EARLY_DIV2_CBCR_CLKOFF_SHFT                                                   31
#define HWIO_MSS_CC_MSS_MPLL1_EARLY_DIV2_CBCR_CLKEN_BMSK                                                   0x1
#define HWIO_MSS_CC_MSS_MPLL1_EARLY_DIV2_CBCR_CLKEN_SHFT                                                     0

#define HWIO_MSS_CC_MSS_MPLL1_OUT_EARLY_DIV2_CBCR_ADDR                                              (MSS_CC_MSS_CC_REG_REG_BASE            + 0x1410)
#define HWIO_MSS_CC_MSS_MPLL1_OUT_EARLY_DIV2_CBCR_RMSK                                              0x80000001
#define HWIO_MSS_CC_MSS_MPLL1_OUT_EARLY_DIV2_CBCR_IN                    \
                in_dword(HWIO_MSS_CC_MSS_MPLL1_OUT_EARLY_DIV2_CBCR_ADDR)
#define HWIO_MSS_CC_MSS_MPLL1_OUT_EARLY_DIV2_CBCR_INM(m)            \
                in_dword_masked(HWIO_MSS_CC_MSS_MPLL1_OUT_EARLY_DIV2_CBCR_ADDR, m)
#define HWIO_MSS_CC_MSS_MPLL1_OUT_EARLY_DIV2_CBCR_OUT(v)            \
                out_dword(HWIO_MSS_CC_MSS_MPLL1_OUT_EARLY_DIV2_CBCR_ADDR,v)
#define HWIO_MSS_CC_MSS_MPLL1_OUT_EARLY_DIV2_CBCR_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_CC_MSS_MPLL1_OUT_EARLY_DIV2_CBCR_ADDR,m,v,HWIO_MSS_CC_MSS_MPLL1_OUT_EARLY_DIV2_CBCR_IN)
#define HWIO_MSS_CC_MSS_MPLL1_OUT_EARLY_DIV2_CBCR_CLKOFF_BMSK                                       0x80000000
#define HWIO_MSS_CC_MSS_MPLL1_OUT_EARLY_DIV2_CBCR_CLKOFF_SHFT                                               31
#define HWIO_MSS_CC_MSS_MPLL1_OUT_EARLY_DIV2_CBCR_CLKEN_BMSK                                               0x1
#define HWIO_MSS_CC_MSS_MPLL1_OUT_EARLY_DIV2_CBCR_CLKEN_SHFT                                                 0

#define HWIO_MSS_CC_DAC_CLK_CFG_ADDR                                                                (MSS_CC_MSS_CC_REG_REG_BASE            + 0x1414)
#define HWIO_MSS_CC_DAC_CLK_CFG_RMSK                                                                  0x15e03d
#define HWIO_MSS_CC_DAC_CLK_CFG_IN                    \
                in_dword(HWIO_MSS_CC_DAC_CLK_CFG_ADDR)
#define HWIO_MSS_CC_DAC_CLK_CFG_INM(m)            \
                in_dword_masked(HWIO_MSS_CC_DAC_CLK_CFG_ADDR, m)
#define HWIO_MSS_CC_DAC_CLK_CFG_OUT(v)            \
                out_dword(HWIO_MSS_CC_DAC_CLK_CFG_ADDR,v)
#define HWIO_MSS_CC_DAC_CLK_CFG_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_CC_DAC_CLK_CFG_ADDR,m,v,HWIO_MSS_CC_DAC_CLK_CFG_IN)
#define HWIO_MSS_CC_DAC_CLK_CFG_FCAL_CLK_EN_BMSK                                                      0x100000
#define HWIO_MSS_CC_DAC_CLK_CFG_FCAL_CLK_EN_SHFT                                                            20
#define HWIO_MSS_CC_DAC_CLK_CFG_DAC0_CLK_EN_BMSK                                                       0x40000
#define HWIO_MSS_CC_DAC_CLK_CFG_DAC0_CLK_EN_SHFT                                                            18
#define HWIO_MSS_CC_DAC_CLK_CFG_EXT_DAC_CLK_EN_BMSK                                                    0x10000
#define HWIO_MSS_CC_DAC_CLK_CFG_EXT_DAC_CLK_EN_SHFT                                                         16
#define HWIO_MSS_CC_DAC_CLK_CFG_EXT_FCAL_EN_BMSK                                                        0x8000
#define HWIO_MSS_CC_DAC_CLK_CFG_EXT_FCAL_EN_SHFT                                                            15
#define HWIO_MSS_CC_DAC_CLK_CFG_XO_FCAL_EN_BMSK                                                         0x4000
#define HWIO_MSS_CC_DAC_CLK_CFG_XO_FCAL_EN_SHFT                                                             14
#define HWIO_MSS_CC_DAC_CLK_CFG_XO_CLK_SEL_BMSK                                                         0x2000
#define HWIO_MSS_CC_DAC_CLK_CFG_XO_CLK_SEL_SHFT                                                             13
#define HWIO_MSS_CC_DAC_CLK_CFG_DAC0_DIV_PGM_BMSK                                                         0x3c
#define HWIO_MSS_CC_DAC_CLK_CFG_DAC0_DIV_PGM_SHFT                                                            2
#define HWIO_MSS_CC_DAC_CLK_CFG_PLL1_EN_BMSK                                                               0x1
#define HWIO_MSS_CC_DAC_CLK_CFG_PLL1_EN_SHFT                                                                 0

#define HWIO_MSS_CC_CLK_XO_GATE_CFG_ADDR                                                            (MSS_CC_MSS_CC_REG_REG_BASE            + 0x1418)
#define HWIO_MSS_CC_CLK_XO_GATE_CFG_RMSK                                                                   0x1
#define HWIO_MSS_CC_CLK_XO_GATE_CFG_IN                    \
                in_dword(HWIO_MSS_CC_CLK_XO_GATE_CFG_ADDR)
#define HWIO_MSS_CC_CLK_XO_GATE_CFG_INM(m)            \
                in_dword_masked(HWIO_MSS_CC_CLK_XO_GATE_CFG_ADDR, m)
#define HWIO_MSS_CC_CLK_XO_GATE_CFG_OUT(v)            \
                out_dword(HWIO_MSS_CC_CLK_XO_GATE_CFG_ADDR,v)
#define HWIO_MSS_CC_CLK_XO_GATE_CFG_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_CC_CLK_XO_GATE_CFG_ADDR,m,v,HWIO_MSS_CC_CLK_XO_GATE_CFG_IN)
#define HWIO_MSS_CC_CLK_XO_GATE_CFG_CLK_XO_GATE_CFG_BMSK                                                   0x1
#define HWIO_MSS_CC_CLK_XO_GATE_CFG_CLK_XO_GATE_CFG_SHFT                                                     0

#define HWIO_MSS_CC_MSS_ALT_RESET_Q6SS_ADDR                                                         (MSS_CC_MSS_CC_REG_REG_BASE            + 0x141c)
#define HWIO_MSS_CC_MSS_ALT_RESET_Q6SS_RMSK                                                                0x1
#define HWIO_MSS_CC_MSS_ALT_RESET_Q6SS_IN                    \
                in_dword(HWIO_MSS_CC_MSS_ALT_RESET_Q6SS_ADDR)
#define HWIO_MSS_CC_MSS_ALT_RESET_Q6SS_INM(m)            \
                in_dword_masked(HWIO_MSS_CC_MSS_ALT_RESET_Q6SS_ADDR, m)
#define HWIO_MSS_CC_MSS_ALT_RESET_Q6SS_OUT(v)            \
                out_dword(HWIO_MSS_CC_MSS_ALT_RESET_Q6SS_ADDR,v)
#define HWIO_MSS_CC_MSS_ALT_RESET_Q6SS_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_CC_MSS_ALT_RESET_Q6SS_ADDR,m,v,HWIO_MSS_CC_MSS_ALT_RESET_Q6SS_IN)
#define HWIO_MSS_CC_MSS_ALT_RESET_Q6SS_EN_BMSK                                                             0x1
#define HWIO_MSS_CC_MSS_ALT_RESET_Q6SS_EN_SHFT                                                               0

#define HWIO_MSS_CC_DEBUG_MUX_MUXR_ADDR                                                             (MSS_CC_MSS_CC_REG_REG_BASE            + 0x1420)
#define HWIO_MSS_CC_DEBUG_MUX_MUXR_RMSK                                                                   0x3f
#define HWIO_MSS_CC_DEBUG_MUX_MUXR_IN                    \
                in_dword(HWIO_MSS_CC_DEBUG_MUX_MUXR_ADDR)
#define HWIO_MSS_CC_DEBUG_MUX_MUXR_INM(m)            \
                in_dword_masked(HWIO_MSS_CC_DEBUG_MUX_MUXR_ADDR, m)
#define HWIO_MSS_CC_DEBUG_MUX_MUXR_OUT(v)            \
                out_dword(HWIO_MSS_CC_DEBUG_MUX_MUXR_ADDR,v)
#define HWIO_MSS_CC_DEBUG_MUX_MUXR_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_CC_DEBUG_MUX_MUXR_ADDR,m,v,HWIO_MSS_CC_DEBUG_MUX_MUXR_IN)
#define HWIO_MSS_CC_DEBUG_MUX_MUXR_MUX_SEL_BMSK                                                           0x3f
#define HWIO_MSS_CC_DEBUG_MUX_MUXR_MUX_SEL_SHFT                                                              0

/*----------------------------------------------------------------------------
 * MODULE: MSS_UIM0_UART_DM
 *--------------------------------------------------------------------------*/

#define MSS_UIM0_UART_DM_REG_BASE                                                                               (MSS_TOP_BASE            + 0x00190000)
#define MSS_UIM0_UART_DM_REG_BASE_SIZE                                                                          0x200
#define MSS_UIM0_UART_DM_REG_BASE_USED                                                                          0x1a0

#define HWIO_MSS_UIM0_UART_DM_MR1_ADDR                                                                          (MSS_UIM0_UART_DM_REG_BASE            + 0x0)
#define HWIO_MSS_UIM0_UART_DM_MR1_RMSK                                                                          0xffffffff
#define HWIO_MSS_UIM0_UART_DM_MR1_IN                    \
                in_dword(HWIO_MSS_UIM0_UART_DM_MR1_ADDR)
#define HWIO_MSS_UIM0_UART_DM_MR1_INM(m)            \
                in_dword_masked(HWIO_MSS_UIM0_UART_DM_MR1_ADDR, m)
#define HWIO_MSS_UIM0_UART_DM_MR1_OUT(v)            \
                out_dword(HWIO_MSS_UIM0_UART_DM_MR1_ADDR,v)
#define HWIO_MSS_UIM0_UART_DM_MR1_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_UIM0_UART_DM_MR1_ADDR,m,v,HWIO_MSS_UIM0_UART_DM_MR1_IN)
#define HWIO_MSS_UIM0_UART_DM_MR1_AUTO_RFR_LEVEL1_BMSK                                                          0xffffff00
#define HWIO_MSS_UIM0_UART_DM_MR1_AUTO_RFR_LEVEL1_SHFT                                                                   8
#define HWIO_MSS_UIM0_UART_DM_MR1_RX_RDY_CTL_BMSK                                                                     0x80
#define HWIO_MSS_UIM0_UART_DM_MR1_RX_RDY_CTL_SHFT                                                                        7
#define HWIO_MSS_UIM0_UART_DM_MR1_CTS_CTL_BMSK                                                                        0x40
#define HWIO_MSS_UIM0_UART_DM_MR1_CTS_CTL_SHFT                                                                           6
#define HWIO_MSS_UIM0_UART_DM_MR1_AUTO_RFR_LEVEL0_BMSK                                                                0x3f
#define HWIO_MSS_UIM0_UART_DM_MR1_AUTO_RFR_LEVEL0_SHFT                                                                   0

#define HWIO_MSS_UIM0_UART_DM_MR2_ADDR                                                                          (MSS_UIM0_UART_DM_REG_BASE            + 0x4)
#define HWIO_MSS_UIM0_UART_DM_MR2_RMSK                                                                               0x7ff
#define HWIO_MSS_UIM0_UART_DM_MR2_IN                    \
                in_dword(HWIO_MSS_UIM0_UART_DM_MR2_ADDR)
#define HWIO_MSS_UIM0_UART_DM_MR2_INM(m)            \
                in_dword_masked(HWIO_MSS_UIM0_UART_DM_MR2_ADDR, m)
#define HWIO_MSS_UIM0_UART_DM_MR2_OUT(v)            \
                out_dword(HWIO_MSS_UIM0_UART_DM_MR2_ADDR,v)
#define HWIO_MSS_UIM0_UART_DM_MR2_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_UIM0_UART_DM_MR2_ADDR,m,v,HWIO_MSS_UIM0_UART_DM_MR2_IN)
#define HWIO_MSS_UIM0_UART_DM_MR2_RFR_CTS_LOOPBACK_BMSK                                                              0x400
#define HWIO_MSS_UIM0_UART_DM_MR2_RFR_CTS_LOOPBACK_SHFT                                                                 10
#define HWIO_MSS_UIM0_UART_DM_MR2_RX_ERROR_CHAR_OFF_BMSK                                                             0x200
#define HWIO_MSS_UIM0_UART_DM_MR2_RX_ERROR_CHAR_OFF_SHFT                                                                 9
#define HWIO_MSS_UIM0_UART_DM_MR2_RX_BREAK_ZERO_CHAR_OFF_BMSK                                                        0x100
#define HWIO_MSS_UIM0_UART_DM_MR2_RX_BREAK_ZERO_CHAR_OFF_SHFT                                                            8
#define HWIO_MSS_UIM0_UART_DM_MR2_LOOPBACK_BMSK                                                                       0x80
#define HWIO_MSS_UIM0_UART_DM_MR2_LOOPBACK_SHFT                                                                          7
#define HWIO_MSS_UIM0_UART_DM_MR2_ERROR_MODE_BMSK                                                                     0x40
#define HWIO_MSS_UIM0_UART_DM_MR2_ERROR_MODE_SHFT                                                                        6
#define HWIO_MSS_UIM0_UART_DM_MR2_BITS_PER_CHAR_BMSK                                                                  0x30
#define HWIO_MSS_UIM0_UART_DM_MR2_BITS_PER_CHAR_SHFT                                                                     4
#define HWIO_MSS_UIM0_UART_DM_MR2_STOP_BIT_LEN_BMSK                                                                    0xc
#define HWIO_MSS_UIM0_UART_DM_MR2_STOP_BIT_LEN_SHFT                                                                      2
#define HWIO_MSS_UIM0_UART_DM_MR2_PARITY_MODE_BMSK                                                                     0x3
#define HWIO_MSS_UIM0_UART_DM_MR2_PARITY_MODE_SHFT                                                                       0

#define HWIO_MSS_UIM0_UART_DM_CSR_SR_DEPRECATED_ADDR                                                            (MSS_UIM0_UART_DM_REG_BASE            + 0x8)
#define HWIO_MSS_UIM0_UART_DM_CSR_SR_DEPRECATED_RMSK                                                            0xffffffff
#define HWIO_MSS_UIM0_UART_DM_CSR_SR_DEPRECATED_IN                    \
                in_dword(HWIO_MSS_UIM0_UART_DM_CSR_SR_DEPRECATED_ADDR)
#define HWIO_MSS_UIM0_UART_DM_CSR_SR_DEPRECATED_INM(m)            \
                in_dword_masked(HWIO_MSS_UIM0_UART_DM_CSR_SR_DEPRECATED_ADDR, m)
#define HWIO_MSS_UIM0_UART_DM_CSR_SR_DEPRECATED_OUT(v)            \
                out_dword(HWIO_MSS_UIM0_UART_DM_CSR_SR_DEPRECATED_ADDR,v)
#define HWIO_MSS_UIM0_UART_DM_CSR_SR_DEPRECATED_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_UIM0_UART_DM_CSR_SR_DEPRECATED_ADDR,m,v,HWIO_MSS_UIM0_UART_DM_CSR_SR_DEPRECATED_IN)
#define HWIO_MSS_UIM0_UART_DM_CSR_SR_DEPRECATED_UART_DM_CSR_SR_DEPRECATED_BMSK                                  0xffffffff
#define HWIO_MSS_UIM0_UART_DM_CSR_SR_DEPRECATED_UART_DM_CSR_SR_DEPRECATED_SHFT                                           0

#define HWIO_MSS_UIM0_UART_DM_CR_MISR_DEPRECATED_ADDR                                                           (MSS_UIM0_UART_DM_REG_BASE            + 0x10)
#define HWIO_MSS_UIM0_UART_DM_CR_MISR_DEPRECATED_RMSK                                                           0xffffffff
#define HWIO_MSS_UIM0_UART_DM_CR_MISR_DEPRECATED_IN                    \
                in_dword(HWIO_MSS_UIM0_UART_DM_CR_MISR_DEPRECATED_ADDR)
#define HWIO_MSS_UIM0_UART_DM_CR_MISR_DEPRECATED_INM(m)            \
                in_dword_masked(HWIO_MSS_UIM0_UART_DM_CR_MISR_DEPRECATED_ADDR, m)
#define HWIO_MSS_UIM0_UART_DM_CR_MISR_DEPRECATED_OUT(v)            \
                out_dword(HWIO_MSS_UIM0_UART_DM_CR_MISR_DEPRECATED_ADDR,v)
#define HWIO_MSS_UIM0_UART_DM_CR_MISR_DEPRECATED_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_UIM0_UART_DM_CR_MISR_DEPRECATED_ADDR,m,v,HWIO_MSS_UIM0_UART_DM_CR_MISR_DEPRECATED_IN)
#define HWIO_MSS_UIM0_UART_DM_CR_MISR_DEPRECATED_UART_DM_CR_MISR_DEPRECATED_BMSK                                0xffffffff
#define HWIO_MSS_UIM0_UART_DM_CR_MISR_DEPRECATED_UART_DM_CR_MISR_DEPRECATED_SHFT                                         0

#define HWIO_MSS_UIM0_UART_DM_IMR_ISR_DEPRECATED_ADDR                                                           (MSS_UIM0_UART_DM_REG_BASE            + 0x14)
#define HWIO_MSS_UIM0_UART_DM_IMR_ISR_DEPRECATED_RMSK                                                           0xffffffff
#define HWIO_MSS_UIM0_UART_DM_IMR_ISR_DEPRECATED_IN                    \
                in_dword(HWIO_MSS_UIM0_UART_DM_IMR_ISR_DEPRECATED_ADDR)
#define HWIO_MSS_UIM0_UART_DM_IMR_ISR_DEPRECATED_INM(m)            \
                in_dword_masked(HWIO_MSS_UIM0_UART_DM_IMR_ISR_DEPRECATED_ADDR, m)
#define HWIO_MSS_UIM0_UART_DM_IMR_ISR_DEPRECATED_OUT(v)            \
                out_dword(HWIO_MSS_UIM0_UART_DM_IMR_ISR_DEPRECATED_ADDR,v)
#define HWIO_MSS_UIM0_UART_DM_IMR_ISR_DEPRECATED_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_UIM0_UART_DM_IMR_ISR_DEPRECATED_ADDR,m,v,HWIO_MSS_UIM0_UART_DM_IMR_ISR_DEPRECATED_IN)
#define HWIO_MSS_UIM0_UART_DM_IMR_ISR_DEPRECATED_UART_DM_IMR_ISR_DEPRECATED_BMSK                                0xffffffff
#define HWIO_MSS_UIM0_UART_DM_IMR_ISR_DEPRECATED_UART_DM_IMR_ISR_DEPRECATED_SHFT                                         0

#define HWIO_MSS_UIM0_UART_DM_IPR_ADDR                                                                          (MSS_UIM0_UART_DM_REG_BASE            + 0x18)
#define HWIO_MSS_UIM0_UART_DM_IPR_RMSK                                                                          0xffffffdf
#define HWIO_MSS_UIM0_UART_DM_IPR_IN                    \
                in_dword(HWIO_MSS_UIM0_UART_DM_IPR_ADDR)
#define HWIO_MSS_UIM0_UART_DM_IPR_INM(m)            \
                in_dword_masked(HWIO_MSS_UIM0_UART_DM_IPR_ADDR, m)
#define HWIO_MSS_UIM0_UART_DM_IPR_OUT(v)            \
                out_dword(HWIO_MSS_UIM0_UART_DM_IPR_ADDR,v)
#define HWIO_MSS_UIM0_UART_DM_IPR_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_UIM0_UART_DM_IPR_ADDR,m,v,HWIO_MSS_UIM0_UART_DM_IPR_IN)
#define HWIO_MSS_UIM0_UART_DM_IPR_STALE_TIMEOUT_MSB_BMSK                                                        0xffffff80
#define HWIO_MSS_UIM0_UART_DM_IPR_STALE_TIMEOUT_MSB_SHFT                                                                 7
#define HWIO_MSS_UIM0_UART_DM_IPR_SAMPLE_DATA_BMSK                                                                    0x40
#define HWIO_MSS_UIM0_UART_DM_IPR_SAMPLE_DATA_SHFT                                                                       6
#define HWIO_MSS_UIM0_UART_DM_IPR_STALE_TIMEOUT_LSB_BMSK                                                              0x1f
#define HWIO_MSS_UIM0_UART_DM_IPR_STALE_TIMEOUT_LSB_SHFT                                                                 0

#define HWIO_MSS_UIM0_UART_DM_TFWR_ADDR                                                                         (MSS_UIM0_UART_DM_REG_BASE            + 0x1c)
#define HWIO_MSS_UIM0_UART_DM_TFWR_RMSK                                                                         0xffffffff
#define HWIO_MSS_UIM0_UART_DM_TFWR_IN                    \
                in_dword(HWIO_MSS_UIM0_UART_DM_TFWR_ADDR)
#define HWIO_MSS_UIM0_UART_DM_TFWR_INM(m)            \
                in_dword_masked(HWIO_MSS_UIM0_UART_DM_TFWR_ADDR, m)
#define HWIO_MSS_UIM0_UART_DM_TFWR_OUT(v)            \
                out_dword(HWIO_MSS_UIM0_UART_DM_TFWR_ADDR,v)
#define HWIO_MSS_UIM0_UART_DM_TFWR_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_UIM0_UART_DM_TFWR_ADDR,m,v,HWIO_MSS_UIM0_UART_DM_TFWR_IN)
#define HWIO_MSS_UIM0_UART_DM_TFWR_TFW_BMSK                                                                     0xffffffff
#define HWIO_MSS_UIM0_UART_DM_TFWR_TFW_SHFT                                                                              0

#define HWIO_MSS_UIM0_UART_DM_RFWR_ADDR                                                                         (MSS_UIM0_UART_DM_REG_BASE            + 0x20)
#define HWIO_MSS_UIM0_UART_DM_RFWR_RMSK                                                                         0xffffffff
#define HWIO_MSS_UIM0_UART_DM_RFWR_IN                    \
                in_dword(HWIO_MSS_UIM0_UART_DM_RFWR_ADDR)
#define HWIO_MSS_UIM0_UART_DM_RFWR_INM(m)            \
                in_dword_masked(HWIO_MSS_UIM0_UART_DM_RFWR_ADDR, m)
#define HWIO_MSS_UIM0_UART_DM_RFWR_OUT(v)            \
                out_dword(HWIO_MSS_UIM0_UART_DM_RFWR_ADDR,v)
#define HWIO_MSS_UIM0_UART_DM_RFWR_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_UIM0_UART_DM_RFWR_ADDR,m,v,HWIO_MSS_UIM0_UART_DM_RFWR_IN)
#define HWIO_MSS_UIM0_UART_DM_RFWR_RFW_BMSK                                                                     0xffffffff
#define HWIO_MSS_UIM0_UART_DM_RFWR_RFW_SHFT                                                                              0

#define HWIO_MSS_UIM0_UART_DM_HCR_ADDR                                                                          (MSS_UIM0_UART_DM_REG_BASE            + 0x24)
#define HWIO_MSS_UIM0_UART_DM_HCR_RMSK                                                                                0xff
#define HWIO_MSS_UIM0_UART_DM_HCR_IN                    \
                in_dword(HWIO_MSS_UIM0_UART_DM_HCR_ADDR)
#define HWIO_MSS_UIM0_UART_DM_HCR_INM(m)            \
                in_dword_masked(HWIO_MSS_UIM0_UART_DM_HCR_ADDR, m)
#define HWIO_MSS_UIM0_UART_DM_HCR_OUT(v)            \
                out_dword(HWIO_MSS_UIM0_UART_DM_HCR_ADDR,v)
#define HWIO_MSS_UIM0_UART_DM_HCR_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_UIM0_UART_DM_HCR_ADDR,m,v,HWIO_MSS_UIM0_UART_DM_HCR_IN)
#define HWIO_MSS_UIM0_UART_DM_HCR_DATA_BMSK                                                                           0xff
#define HWIO_MSS_UIM0_UART_DM_HCR_DATA_SHFT                                                                              0

#define HWIO_MSS_UIM0_UART_DM_DMRX_ADDR                                                                         (MSS_UIM0_UART_DM_REG_BASE            + 0x34)
#define HWIO_MSS_UIM0_UART_DM_DMRX_RMSK                                                                          0x1ffffff
#define HWIO_MSS_UIM0_UART_DM_DMRX_IN                    \
                in_dword(HWIO_MSS_UIM0_UART_DM_DMRX_ADDR)
#define HWIO_MSS_UIM0_UART_DM_DMRX_INM(m)            \
                in_dword_masked(HWIO_MSS_UIM0_UART_DM_DMRX_ADDR, m)
#define HWIO_MSS_UIM0_UART_DM_DMRX_OUT(v)            \
                out_dword(HWIO_MSS_UIM0_UART_DM_DMRX_ADDR,v)
#define HWIO_MSS_UIM0_UART_DM_DMRX_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_UIM0_UART_DM_DMRX_ADDR,m,v,HWIO_MSS_UIM0_UART_DM_DMRX_IN)
#define HWIO_MSS_UIM0_UART_DM_DMRX_RX_DM_CRCI_CHARS_BMSK                                                         0x1ffffff
#define HWIO_MSS_UIM0_UART_DM_DMRX_RX_DM_CRCI_CHARS_SHFT                                                                 0

#define HWIO_MSS_UIM0_UART_DM_IRDA_RX_TOTAL_SNAP_DEPRECATED_ADDR                                                (MSS_UIM0_UART_DM_REG_BASE            + 0x38)
#define HWIO_MSS_UIM0_UART_DM_IRDA_RX_TOTAL_SNAP_DEPRECATED_RMSK                                                0xffffffff
#define HWIO_MSS_UIM0_UART_DM_IRDA_RX_TOTAL_SNAP_DEPRECATED_IN                    \
                in_dword(HWIO_MSS_UIM0_UART_DM_IRDA_RX_TOTAL_SNAP_DEPRECATED_ADDR)
#define HWIO_MSS_UIM0_UART_DM_IRDA_RX_TOTAL_SNAP_DEPRECATED_INM(m)            \
                in_dword_masked(HWIO_MSS_UIM0_UART_DM_IRDA_RX_TOTAL_SNAP_DEPRECATED_ADDR, m)
#define HWIO_MSS_UIM0_UART_DM_IRDA_RX_TOTAL_SNAP_DEPRECATED_OUT(v)            \
                out_dword(HWIO_MSS_UIM0_UART_DM_IRDA_RX_TOTAL_SNAP_DEPRECATED_ADDR,v)
#define HWIO_MSS_UIM0_UART_DM_IRDA_RX_TOTAL_SNAP_DEPRECATED_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_UIM0_UART_DM_IRDA_RX_TOTAL_SNAP_DEPRECATED_ADDR,m,v,HWIO_MSS_UIM0_UART_DM_IRDA_RX_TOTAL_SNAP_DEPRECATED_IN)
#define HWIO_MSS_UIM0_UART_DM_IRDA_RX_TOTAL_SNAP_DEPRECATED_UART_DM_IRDA_RX_TOTAL_SNAP_DEPRECATED_BMSK          0xffffffff
#define HWIO_MSS_UIM0_UART_DM_IRDA_RX_TOTAL_SNAP_DEPRECATED_UART_DM_IRDA_RX_TOTAL_SNAP_DEPRECATED_SHFT                   0

#define HWIO_MSS_UIM0_UART_DM_DMEN_ADDR                                                                         (MSS_UIM0_UART_DM_REG_BASE            + 0x3c)
#define HWIO_MSS_UIM0_UART_DM_DMEN_RMSK                                                                               0x3e
#define HWIO_MSS_UIM0_UART_DM_DMEN_IN                    \
                in_dword(HWIO_MSS_UIM0_UART_DM_DMEN_ADDR)
#define HWIO_MSS_UIM0_UART_DM_DMEN_INM(m)            \
                in_dword_masked(HWIO_MSS_UIM0_UART_DM_DMEN_ADDR, m)
#define HWIO_MSS_UIM0_UART_DM_DMEN_OUT(v)            \
                out_dword(HWIO_MSS_UIM0_UART_DM_DMEN_ADDR,v)
#define HWIO_MSS_UIM0_UART_DM_DMEN_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_UIM0_UART_DM_DMEN_ADDR,m,v,HWIO_MSS_UIM0_UART_DM_DMEN_IN)
#define HWIO_MSS_UIM0_UART_DM_DMEN_RX_SC_ENABLE_BMSK                                                                  0x20
#define HWIO_MSS_UIM0_UART_DM_DMEN_RX_SC_ENABLE_SHFT                                                                     5
#define HWIO_MSS_UIM0_UART_DM_DMEN_TX_SC_ENABLE_BMSK                                                                  0x10
#define HWIO_MSS_UIM0_UART_DM_DMEN_TX_SC_ENABLE_SHFT                                                                     4
#define HWIO_MSS_UIM0_UART_DM_DMEN_RX_BAM_ENABLE_BMSK                                                                  0x8
#define HWIO_MSS_UIM0_UART_DM_DMEN_RX_BAM_ENABLE_SHFT                                                                    3
#define HWIO_MSS_UIM0_UART_DM_DMEN_TX_BAM_ENABLE_BMSK                                                                  0x4
#define HWIO_MSS_UIM0_UART_DM_DMEN_TX_BAM_ENABLE_SHFT                                                                    2
#define HWIO_MSS_UIM0_UART_DM_DMEN_TX_DONE_FIX_CHKN_BMSK                                                               0x2
#define HWIO_MSS_UIM0_UART_DM_DMEN_TX_DONE_FIX_CHKN_SHFT                                                                 1

#define HWIO_MSS_UIM0_UART_DM_NO_CHARS_FOR_TX_ADDR                                                              (MSS_UIM0_UART_DM_REG_BASE            + 0x40)
#define HWIO_MSS_UIM0_UART_DM_NO_CHARS_FOR_TX_RMSK                                                                0xffffff
#define HWIO_MSS_UIM0_UART_DM_NO_CHARS_FOR_TX_IN                    \
                in_dword(HWIO_MSS_UIM0_UART_DM_NO_CHARS_FOR_TX_ADDR)
#define HWIO_MSS_UIM0_UART_DM_NO_CHARS_FOR_TX_INM(m)            \
                in_dword_masked(HWIO_MSS_UIM0_UART_DM_NO_CHARS_FOR_TX_ADDR, m)
#define HWIO_MSS_UIM0_UART_DM_NO_CHARS_FOR_TX_OUT(v)            \
                out_dword(HWIO_MSS_UIM0_UART_DM_NO_CHARS_FOR_TX_ADDR,v)
#define HWIO_MSS_UIM0_UART_DM_NO_CHARS_FOR_TX_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_UIM0_UART_DM_NO_CHARS_FOR_TX_ADDR,m,v,HWIO_MSS_UIM0_UART_DM_NO_CHARS_FOR_TX_IN)
#define HWIO_MSS_UIM0_UART_DM_NO_CHARS_FOR_TX_TX_TOTAL_TRANS_LEN_BMSK                                             0xffffff
#define HWIO_MSS_UIM0_UART_DM_NO_CHARS_FOR_TX_TX_TOTAL_TRANS_LEN_SHFT                                                    0

#define HWIO_MSS_UIM0_UART_DM_BADR_ADDR                                                                         (MSS_UIM0_UART_DM_REG_BASE            + 0x44)
#define HWIO_MSS_UIM0_UART_DM_BADR_RMSK                                                                         0xfffffffc
#define HWIO_MSS_UIM0_UART_DM_BADR_IN                    \
                in_dword(HWIO_MSS_UIM0_UART_DM_BADR_ADDR)
#define HWIO_MSS_UIM0_UART_DM_BADR_INM(m)            \
                in_dword_masked(HWIO_MSS_UIM0_UART_DM_BADR_ADDR, m)
#define HWIO_MSS_UIM0_UART_DM_BADR_OUT(v)            \
                out_dword(HWIO_MSS_UIM0_UART_DM_BADR_ADDR,v)
#define HWIO_MSS_UIM0_UART_DM_BADR_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_UIM0_UART_DM_BADR_ADDR,m,v,HWIO_MSS_UIM0_UART_DM_BADR_IN)
#define HWIO_MSS_UIM0_UART_DM_BADR_RX_BASE_ADDR_BMSK                                                            0xfffffffc
#define HWIO_MSS_UIM0_UART_DM_BADR_RX_BASE_ADDR_SHFT                                                                     2

#define HWIO_MSS_UIM0_UART_DM_TESTSL_ADDR                                                                       (MSS_UIM0_UART_DM_REG_BASE            + 0x48)
#define HWIO_MSS_UIM0_UART_DM_TESTSL_RMSK                                                                             0x1f
#define HWIO_MSS_UIM0_UART_DM_TESTSL_IN                    \
                in_dword(HWIO_MSS_UIM0_UART_DM_TESTSL_ADDR)
#define HWIO_MSS_UIM0_UART_DM_TESTSL_INM(m)            \
                in_dword_masked(HWIO_MSS_UIM0_UART_DM_TESTSL_ADDR, m)
#define HWIO_MSS_UIM0_UART_DM_TESTSL_OUT(v)            \
                out_dword(HWIO_MSS_UIM0_UART_DM_TESTSL_ADDR,v)
#define HWIO_MSS_UIM0_UART_DM_TESTSL_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_UIM0_UART_DM_TESTSL_ADDR,m,v,HWIO_MSS_UIM0_UART_DM_TESTSL_IN)
#define HWIO_MSS_UIM0_UART_DM_TESTSL_TEST_EN_BMSK                                                                     0x10
#define HWIO_MSS_UIM0_UART_DM_TESTSL_TEST_EN_SHFT                                                                        4
#define HWIO_MSS_UIM0_UART_DM_TESTSL_TEST_SEL_BMSK                                                                     0xf
#define HWIO_MSS_UIM0_UART_DM_TESTSL_TEST_SEL_SHFT                                                                       0

#define HWIO_MSS_UIM0_UART_DM_TXFS_ADDR                                                                         (MSS_UIM0_UART_DM_REG_BASE            + 0x4c)
#define HWIO_MSS_UIM0_UART_DM_TXFS_RMSK                                                                         0xffffffff
#define HWIO_MSS_UIM0_UART_DM_TXFS_IN                    \
                in_dword(HWIO_MSS_UIM0_UART_DM_TXFS_ADDR)
#define HWIO_MSS_UIM0_UART_DM_TXFS_INM(m)            \
                in_dword_masked(HWIO_MSS_UIM0_UART_DM_TXFS_ADDR, m)
#define HWIO_MSS_UIM0_UART_DM_TXFS_TX_FIFO_STATE_MSB_BMSK                                                       0xffffc000
#define HWIO_MSS_UIM0_UART_DM_TXFS_TX_FIFO_STATE_MSB_SHFT                                                               14
#define HWIO_MSS_UIM0_UART_DM_TXFS_TX_ASYNC_FIFO_STATE_BMSK                                                         0x3c00
#define HWIO_MSS_UIM0_UART_DM_TXFS_TX_ASYNC_FIFO_STATE_SHFT                                                             10
#define HWIO_MSS_UIM0_UART_DM_TXFS_TX_BUFFER_STATE_BMSK                                                              0x380
#define HWIO_MSS_UIM0_UART_DM_TXFS_TX_BUFFER_STATE_SHFT                                                                  7
#define HWIO_MSS_UIM0_UART_DM_TXFS_TX_FIFO_STATE_LSB_BMSK                                                             0x7f
#define HWIO_MSS_UIM0_UART_DM_TXFS_TX_FIFO_STATE_LSB_SHFT                                                                0

#define HWIO_MSS_UIM0_UART_DM_RXFS_ADDR                                                                         (MSS_UIM0_UART_DM_REG_BASE            + 0x50)
#define HWIO_MSS_UIM0_UART_DM_RXFS_RMSK                                                                         0xffffffff
#define HWIO_MSS_UIM0_UART_DM_RXFS_IN                    \
                in_dword(HWIO_MSS_UIM0_UART_DM_RXFS_ADDR)
#define HWIO_MSS_UIM0_UART_DM_RXFS_INM(m)            \
                in_dword_masked(HWIO_MSS_UIM0_UART_DM_RXFS_ADDR, m)
#define HWIO_MSS_UIM0_UART_DM_RXFS_RX_FIFO_STATE_MSB_BMSK                                                       0xffffc000
#define HWIO_MSS_UIM0_UART_DM_RXFS_RX_FIFO_STATE_MSB_SHFT                                                               14
#define HWIO_MSS_UIM0_UART_DM_RXFS_RX_ASYNC_FIFO_STATE_BMSK                                                         0x3c00
#define HWIO_MSS_UIM0_UART_DM_RXFS_RX_ASYNC_FIFO_STATE_SHFT                                                             10
#define HWIO_MSS_UIM0_UART_DM_RXFS_RX_BUFFER_STATE_BMSK                                                              0x380
#define HWIO_MSS_UIM0_UART_DM_RXFS_RX_BUFFER_STATE_SHFT                                                                  7
#define HWIO_MSS_UIM0_UART_DM_RXFS_RX_FIFO_STATE_LSB_BMSK                                                             0x7f
#define HWIO_MSS_UIM0_UART_DM_RXFS_RX_FIFO_STATE_LSB_SHFT                                                                0

#define HWIO_MSS_UIM0_UART_DM_MISR_MODE_ADDR                                                                    (MSS_UIM0_UART_DM_REG_BASE            + 0x60)
#define HWIO_MSS_UIM0_UART_DM_MISR_MODE_RMSK                                                                           0x3
#define HWIO_MSS_UIM0_UART_DM_MISR_MODE_IN                    \
                in_dword(HWIO_MSS_UIM0_UART_DM_MISR_MODE_ADDR)
#define HWIO_MSS_UIM0_UART_DM_MISR_MODE_INM(m)            \
                in_dword_masked(HWIO_MSS_UIM0_UART_DM_MISR_MODE_ADDR, m)
#define HWIO_MSS_UIM0_UART_DM_MISR_MODE_OUT(v)            \
                out_dword(HWIO_MSS_UIM0_UART_DM_MISR_MODE_ADDR,v)
#define HWIO_MSS_UIM0_UART_DM_MISR_MODE_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_UIM0_UART_DM_MISR_MODE_ADDR,m,v,HWIO_MSS_UIM0_UART_DM_MISR_MODE_IN)
#define HWIO_MSS_UIM0_UART_DM_MISR_MODE_MODE_BMSK                                                                      0x3
#define HWIO_MSS_UIM0_UART_DM_MISR_MODE_MODE_SHFT                                                                        0

#define HWIO_MSS_UIM0_UART_DM_MISR_RESET_ADDR                                                                   (MSS_UIM0_UART_DM_REG_BASE            + 0x64)
#define HWIO_MSS_UIM0_UART_DM_MISR_RESET_RMSK                                                                          0x1
#define HWIO_MSS_UIM0_UART_DM_MISR_RESET_OUT(v)            \
                out_dword(HWIO_MSS_UIM0_UART_DM_MISR_RESET_ADDR,v)
#define HWIO_MSS_UIM0_UART_DM_MISR_RESET_RESET_BMSK                                                                    0x1
#define HWIO_MSS_UIM0_UART_DM_MISR_RESET_RESET_SHFT                                                                      0

#define HWIO_MSS_UIM0_UART_DM_MISR_EXPORT_ADDR                                                                  (MSS_UIM0_UART_DM_REG_BASE            + 0x68)
#define HWIO_MSS_UIM0_UART_DM_MISR_EXPORT_RMSK                                                                         0x1
#define HWIO_MSS_UIM0_UART_DM_MISR_EXPORT_IN                    \
                in_dword(HWIO_MSS_UIM0_UART_DM_MISR_EXPORT_ADDR)
#define HWIO_MSS_UIM0_UART_DM_MISR_EXPORT_INM(m)            \
                in_dword_masked(HWIO_MSS_UIM0_UART_DM_MISR_EXPORT_ADDR, m)
#define HWIO_MSS_UIM0_UART_DM_MISR_EXPORT_OUT(v)            \
                out_dword(HWIO_MSS_UIM0_UART_DM_MISR_EXPORT_ADDR,v)
#define HWIO_MSS_UIM0_UART_DM_MISR_EXPORT_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_UIM0_UART_DM_MISR_EXPORT_ADDR,m,v,HWIO_MSS_UIM0_UART_DM_MISR_EXPORT_IN)
#define HWIO_MSS_UIM0_UART_DM_MISR_EXPORT_EXPORT_BMSK                                                                  0x1
#define HWIO_MSS_UIM0_UART_DM_MISR_EXPORT_EXPORT_SHFT                                                                    0

#define HWIO_MSS_UIM0_UART_DM_MISR_VAL_ADDR                                                                     (MSS_UIM0_UART_DM_REG_BASE            + 0x6c)
#define HWIO_MSS_UIM0_UART_DM_MISR_VAL_RMSK                                                                          0x3ff
#define HWIO_MSS_UIM0_UART_DM_MISR_VAL_IN                    \
                in_dword(HWIO_MSS_UIM0_UART_DM_MISR_VAL_ADDR)
#define HWIO_MSS_UIM0_UART_DM_MISR_VAL_INM(m)            \
                in_dword_masked(HWIO_MSS_UIM0_UART_DM_MISR_VAL_ADDR, m)
#define HWIO_MSS_UIM0_UART_DM_MISR_VAL_VAL_BMSK                                                                      0x3ff
#define HWIO_MSS_UIM0_UART_DM_MISR_VAL_VAL_SHFT                                                                          0

#define HWIO_MSS_UIM0_UART_DM_TF_RF_DEPRECATED_ADDR                                                             (MSS_UIM0_UART_DM_REG_BASE            + 0x70)
#define HWIO_MSS_UIM0_UART_DM_TF_RF_DEPRECATED_RMSK                                                             0xffffffff
#define HWIO_MSS_UIM0_UART_DM_TF_RF_DEPRECATED_IN                    \
                in_dword(HWIO_MSS_UIM0_UART_DM_TF_RF_DEPRECATED_ADDR)
#define HWIO_MSS_UIM0_UART_DM_TF_RF_DEPRECATED_INM(m)            \
                in_dword_masked(HWIO_MSS_UIM0_UART_DM_TF_RF_DEPRECATED_ADDR, m)
#define HWIO_MSS_UIM0_UART_DM_TF_RF_DEPRECATED_OUT(v)            \
                out_dword(HWIO_MSS_UIM0_UART_DM_TF_RF_DEPRECATED_ADDR,v)
#define HWIO_MSS_UIM0_UART_DM_TF_RF_DEPRECATED_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_UIM0_UART_DM_TF_RF_DEPRECATED_ADDR,m,v,HWIO_MSS_UIM0_UART_DM_TF_RF_DEPRECATED_IN)
#define HWIO_MSS_UIM0_UART_DM_TF_RF_DEPRECATED_UART_DM_TF_RF_DEPRECATED_BMSK                                    0xffffffff
#define HWIO_MSS_UIM0_UART_DM_TF_RF_DEPRECATED_UART_DM_TF_RF_DEPRECATED_SHFT                                             0

#define HWIO_MSS_UIM0_UART_DM_TF_RF_2_DEPRECATED_ADDR                                                           (MSS_UIM0_UART_DM_REG_BASE            + 0x74)
#define HWIO_MSS_UIM0_UART_DM_TF_RF_2_DEPRECATED_RMSK                                                           0xffffffff
#define HWIO_MSS_UIM0_UART_DM_TF_RF_2_DEPRECATED_IN                    \
                in_dword(HWIO_MSS_UIM0_UART_DM_TF_RF_2_DEPRECATED_ADDR)
#define HWIO_MSS_UIM0_UART_DM_TF_RF_2_DEPRECATED_INM(m)            \
                in_dword_masked(HWIO_MSS_UIM0_UART_DM_TF_RF_2_DEPRECATED_ADDR, m)
#define HWIO_MSS_UIM0_UART_DM_TF_RF_2_DEPRECATED_OUT(v)            \
                out_dword(HWIO_MSS_UIM0_UART_DM_TF_RF_2_DEPRECATED_ADDR,v)
#define HWIO_MSS_UIM0_UART_DM_TF_RF_2_DEPRECATED_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_UIM0_UART_DM_TF_RF_2_DEPRECATED_ADDR,m,v,HWIO_MSS_UIM0_UART_DM_TF_RF_2_DEPRECATED_IN)
#define HWIO_MSS_UIM0_UART_DM_TF_RF_2_DEPRECATED_UART_DM_TF_RF_2_DEPRECATED_BMSK                                0xffffffff
#define HWIO_MSS_UIM0_UART_DM_TF_RF_2_DEPRECATED_UART_DM_TF_RF_2_DEPRECATED_SHFT                                         0

#define HWIO_MSS_UIM0_UART_DM_TF_RF_3_DEPRECATED_ADDR                                                           (MSS_UIM0_UART_DM_REG_BASE            + 0x78)
#define HWIO_MSS_UIM0_UART_DM_TF_RF_3_DEPRECATED_RMSK                                                           0xffffffff
#define HWIO_MSS_UIM0_UART_DM_TF_RF_3_DEPRECATED_IN                    \
                in_dword(HWIO_MSS_UIM0_UART_DM_TF_RF_3_DEPRECATED_ADDR)
#define HWIO_MSS_UIM0_UART_DM_TF_RF_3_DEPRECATED_INM(m)            \
                in_dword_masked(HWIO_MSS_UIM0_UART_DM_TF_RF_3_DEPRECATED_ADDR, m)
#define HWIO_MSS_UIM0_UART_DM_TF_RF_3_DEPRECATED_OUT(v)            \
                out_dword(HWIO_MSS_UIM0_UART_DM_TF_RF_3_DEPRECATED_ADDR,v)
#define HWIO_MSS_UIM0_UART_DM_TF_RF_3_DEPRECATED_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_UIM0_UART_DM_TF_RF_3_DEPRECATED_ADDR,m,v,HWIO_MSS_UIM0_UART_DM_TF_RF_3_DEPRECATED_IN)
#define HWIO_MSS_UIM0_UART_DM_TF_RF_3_DEPRECATED_UART_DM_TF_RF_3_DEPRECATED_BMSK                                0xffffffff
#define HWIO_MSS_UIM0_UART_DM_TF_RF_3_DEPRECATED_UART_DM_TF_RF_3_DEPRECATED_SHFT                                         0

#define HWIO_MSS_UIM0_UART_DM_TF_RF_4_DEPRECATED_ADDR                                                           (MSS_UIM0_UART_DM_REG_BASE            + 0x7c)
#define HWIO_MSS_UIM0_UART_DM_TF_RF_4_DEPRECATED_RMSK                                                           0xffffffff
#define HWIO_MSS_UIM0_UART_DM_TF_RF_4_DEPRECATED_IN                    \
                in_dword(HWIO_MSS_UIM0_UART_DM_TF_RF_4_DEPRECATED_ADDR)
#define HWIO_MSS_UIM0_UART_DM_TF_RF_4_DEPRECATED_INM(m)            \
                in_dword_masked(HWIO_MSS_UIM0_UART_DM_TF_RF_4_DEPRECATED_ADDR, m)
#define HWIO_MSS_UIM0_UART_DM_TF_RF_4_DEPRECATED_OUT(v)            \
                out_dword(HWIO_MSS_UIM0_UART_DM_TF_RF_4_DEPRECATED_ADDR,v)
#define HWIO_MSS_UIM0_UART_DM_TF_RF_4_DEPRECATED_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_UIM0_UART_DM_TF_RF_4_DEPRECATED_ADDR,m,v,HWIO_MSS_UIM0_UART_DM_TF_RF_4_DEPRECATED_IN)
#define HWIO_MSS_UIM0_UART_DM_TF_RF_4_DEPRECATED_UART_DM_TF_RF_4_DEPRECATED_BMSK                                0xffffffff
#define HWIO_MSS_UIM0_UART_DM_TF_RF_4_DEPRECATED_UART_DM_TF_RF_4_DEPRECATED_SHFT                                         0

#define HWIO_MSS_UIM0_UART_DM_SIM_CFG_ADDR                                                                      (MSS_UIM0_UART_DM_REG_BASE            + 0x80)
#define HWIO_MSS_UIM0_UART_DM_SIM_CFG_RMSK                                                                         0x3ffef
#define HWIO_MSS_UIM0_UART_DM_SIM_CFG_IN                    \
                in_dword(HWIO_MSS_UIM0_UART_DM_SIM_CFG_ADDR)
#define HWIO_MSS_UIM0_UART_DM_SIM_CFG_INM(m)            \
                in_dword_masked(HWIO_MSS_UIM0_UART_DM_SIM_CFG_ADDR, m)
#define HWIO_MSS_UIM0_UART_DM_SIM_CFG_OUT(v)            \
                out_dword(HWIO_MSS_UIM0_UART_DM_SIM_CFG_ADDR,v)
#define HWIO_MSS_UIM0_UART_DM_SIM_CFG_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_UIM0_UART_DM_SIM_CFG_ADDR,m,v,HWIO_MSS_UIM0_UART_DM_SIM_CFG_IN)
#define HWIO_MSS_UIM0_UART_DM_SIM_CFG_UIM_TX_MODE_BMSK                                                             0x20000
#define HWIO_MSS_UIM0_UART_DM_SIM_CFG_UIM_TX_MODE_SHFT                                                                  17
#define HWIO_MSS_UIM0_UART_DM_SIM_CFG_UIM_RX_MODE_BMSK                                                             0x10000
#define HWIO_MSS_UIM0_UART_DM_SIM_CFG_UIM_RX_MODE_SHFT                                                                  16
#define HWIO_MSS_UIM0_UART_DM_SIM_CFG_SIM_STOP_BIT_LEN_BMSK                                                         0xff00
#define HWIO_MSS_UIM0_UART_DM_SIM_CFG_SIM_STOP_BIT_LEN_SHFT                                                              8
#define HWIO_MSS_UIM0_UART_DM_SIM_CFG_SIM_CLK_ON_BMSK                                                                 0x80
#define HWIO_MSS_UIM0_UART_DM_SIM_CFG_SIM_CLK_ON_SHFT                                                                    7
#define HWIO_MSS_UIM0_UART_DM_SIM_CFG_SIM_CLK_TD8_SEL_BMSK                                                            0x40
#define HWIO_MSS_UIM0_UART_DM_SIM_CFG_SIM_CLK_TD8_SEL_SHFT                                                               6
#define HWIO_MSS_UIM0_UART_DM_SIM_CFG_SIM_CLK_STOP_HIGH_BMSK                                                          0x20
#define HWIO_MSS_UIM0_UART_DM_SIM_CFG_SIM_CLK_STOP_HIGH_SHFT                                                             5
#define HWIO_MSS_UIM0_UART_DM_SIM_CFG_MASK_RX_BMSK                                                                     0x8
#define HWIO_MSS_UIM0_UART_DM_SIM_CFG_MASK_RX_SHFT                                                                       3
#define HWIO_MSS_UIM0_UART_DM_SIM_CFG_SWAP_D_BMSK                                                                      0x4
#define HWIO_MSS_UIM0_UART_DM_SIM_CFG_SWAP_D_SHFT                                                                        2
#define HWIO_MSS_UIM0_UART_DM_SIM_CFG_INV_D_BMSK                                                                       0x2
#define HWIO_MSS_UIM0_UART_DM_SIM_CFG_INV_D_SHFT                                                                         1
#define HWIO_MSS_UIM0_UART_DM_SIM_CFG_SIM_SEL_BMSK                                                                     0x1
#define HWIO_MSS_UIM0_UART_DM_SIM_CFG_SIM_SEL_SHFT                                                                       0

#define HWIO_MSS_UIM0_UART_DM_TEST_WR_ADDR_ADDR                                                                 (MSS_UIM0_UART_DM_REG_BASE            + 0x84)
#define HWIO_MSS_UIM0_UART_DM_TEST_WR_ADDR_RMSK                                                                 0xffffffff
#define HWIO_MSS_UIM0_UART_DM_TEST_WR_ADDR_IN                    \
                in_dword(HWIO_MSS_UIM0_UART_DM_TEST_WR_ADDR_ADDR)
#define HWIO_MSS_UIM0_UART_DM_TEST_WR_ADDR_INM(m)            \
                in_dword_masked(HWIO_MSS_UIM0_UART_DM_TEST_WR_ADDR_ADDR, m)
#define HWIO_MSS_UIM0_UART_DM_TEST_WR_ADDR_OUT(v)            \
                out_dword(HWIO_MSS_UIM0_UART_DM_TEST_WR_ADDR_ADDR,v)
#define HWIO_MSS_UIM0_UART_DM_TEST_WR_ADDR_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_UIM0_UART_DM_TEST_WR_ADDR_ADDR,m,v,HWIO_MSS_UIM0_UART_DM_TEST_WR_ADDR_IN)
#define HWIO_MSS_UIM0_UART_DM_TEST_WR_ADDR_TEST_WR_ADDR_BMSK                                                    0xffffffff
#define HWIO_MSS_UIM0_UART_DM_TEST_WR_ADDR_TEST_WR_ADDR_SHFT                                                             0

#define HWIO_MSS_UIM0_UART_DM_TEST_WR_DATA_ADDR                                                                 (MSS_UIM0_UART_DM_REG_BASE            + 0x88)
#define HWIO_MSS_UIM0_UART_DM_TEST_WR_DATA_RMSK                                                                 0xffffffff
#define HWIO_MSS_UIM0_UART_DM_TEST_WR_DATA_OUT(v)            \
                out_dword(HWIO_MSS_UIM0_UART_DM_TEST_WR_DATA_ADDR,v)
#define HWIO_MSS_UIM0_UART_DM_TEST_WR_DATA_TEST_WR_DATA_BMSK                                                    0xffffffff
#define HWIO_MSS_UIM0_UART_DM_TEST_WR_DATA_TEST_WR_DATA_SHFT                                                             0

#define HWIO_MSS_UIM0_UART_DM_TEST_RD_ADDR_ADDR                                                                 (MSS_UIM0_UART_DM_REG_BASE            + 0x8c)
#define HWIO_MSS_UIM0_UART_DM_TEST_RD_ADDR_RMSK                                                                 0xffffffff
#define HWIO_MSS_UIM0_UART_DM_TEST_RD_ADDR_IN                    \
                in_dword(HWIO_MSS_UIM0_UART_DM_TEST_RD_ADDR_ADDR)
#define HWIO_MSS_UIM0_UART_DM_TEST_RD_ADDR_INM(m)            \
                in_dword_masked(HWIO_MSS_UIM0_UART_DM_TEST_RD_ADDR_ADDR, m)
#define HWIO_MSS_UIM0_UART_DM_TEST_RD_ADDR_OUT(v)            \
                out_dword(HWIO_MSS_UIM0_UART_DM_TEST_RD_ADDR_ADDR,v)
#define HWIO_MSS_UIM0_UART_DM_TEST_RD_ADDR_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_UIM0_UART_DM_TEST_RD_ADDR_ADDR,m,v,HWIO_MSS_UIM0_UART_DM_TEST_RD_ADDR_IN)
#define HWIO_MSS_UIM0_UART_DM_TEST_RD_ADDR_TEST_RD_ADDR_BMSK                                                    0xffffffff
#define HWIO_MSS_UIM0_UART_DM_TEST_RD_ADDR_TEST_RD_ADDR_SHFT                                                             0

#define HWIO_MSS_UIM0_UART_DM_TEST_RD_DATA_ADDR                                                                 (MSS_UIM0_UART_DM_REG_BASE            + 0x90)
#define HWIO_MSS_UIM0_UART_DM_TEST_RD_DATA_RMSK                                                                 0xffffffff
#define HWIO_MSS_UIM0_UART_DM_TEST_RD_DATA_IN                    \
                in_dword(HWIO_MSS_UIM0_UART_DM_TEST_RD_DATA_ADDR)
#define HWIO_MSS_UIM0_UART_DM_TEST_RD_DATA_INM(m)            \
                in_dword_masked(HWIO_MSS_UIM0_UART_DM_TEST_RD_DATA_ADDR, m)
#define HWIO_MSS_UIM0_UART_DM_TEST_RD_DATA_TEST_RD_DATA_BMSK                                                    0xffffffff
#define HWIO_MSS_UIM0_UART_DM_TEST_RD_DATA_TEST_RD_DATA_SHFT                                                             0

#define HWIO_MSS_UIM0_UART_DM_CSR_ADDR                                                                          (MSS_UIM0_UART_DM_REG_BASE            + 0xa0)
#define HWIO_MSS_UIM0_UART_DM_CSR_RMSK                                                                                0xff
#define HWIO_MSS_UIM0_UART_DM_CSR_IN                    \
                in_dword(HWIO_MSS_UIM0_UART_DM_CSR_ADDR)
#define HWIO_MSS_UIM0_UART_DM_CSR_INM(m)            \
                in_dword_masked(HWIO_MSS_UIM0_UART_DM_CSR_ADDR, m)
#define HWIO_MSS_UIM0_UART_DM_CSR_OUT(v)            \
                out_dword(HWIO_MSS_UIM0_UART_DM_CSR_ADDR,v)
#define HWIO_MSS_UIM0_UART_DM_CSR_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_UIM0_UART_DM_CSR_ADDR,m,v,HWIO_MSS_UIM0_UART_DM_CSR_IN)
#define HWIO_MSS_UIM0_UART_DM_CSR_UART_RX_CLK_SEL_BMSK                                                                0xf0
#define HWIO_MSS_UIM0_UART_DM_CSR_UART_RX_CLK_SEL_SHFT                                                                   4
#define HWIO_MSS_UIM0_UART_DM_CSR_UART_TX_CLK_SEL_BMSK                                                                 0xf
#define HWIO_MSS_UIM0_UART_DM_CSR_UART_TX_CLK_SEL_SHFT                                                                   0

#define HWIO_MSS_UIM0_UART_DM_SR_ADDR                                                                           (MSS_UIM0_UART_DM_REG_BASE            + 0xa4)
#define HWIO_MSS_UIM0_UART_DM_SR_RMSK                                                                               0x1fff
#define HWIO_MSS_UIM0_UART_DM_SR_IN                    \
                in_dword(HWIO_MSS_UIM0_UART_DM_SR_ADDR)
#define HWIO_MSS_UIM0_UART_DM_SR_INM(m)            \
                in_dword_masked(HWIO_MSS_UIM0_UART_DM_SR_ADDR, m)
#define HWIO_MSS_UIM0_UART_DM_SR_COMMAND_IN_PROGRESS_BMSK                                                           0x1000
#define HWIO_MSS_UIM0_UART_DM_SR_COMMAND_IN_PROGRESS_SHFT                                                               12
#define HWIO_MSS_UIM0_UART_DM_SR_TRANS_END_TRIGGER_BMSK                                                              0xc00
#define HWIO_MSS_UIM0_UART_DM_SR_TRANS_END_TRIGGER_SHFT                                                                 10
#define HWIO_MSS_UIM0_UART_DM_SR_TRANS_ACTIVE_BMSK                                                                   0x200
#define HWIO_MSS_UIM0_UART_DM_SR_TRANS_ACTIVE_SHFT                                                                       9
#define HWIO_MSS_UIM0_UART_DM_SR_RX_BREAK_START_LAST_BMSK                                                            0x100
#define HWIO_MSS_UIM0_UART_DM_SR_RX_BREAK_START_LAST_SHFT                                                                8
#define HWIO_MSS_UIM0_UART_DM_SR_HUNT_CHAR_BMSK                                                                       0x80
#define HWIO_MSS_UIM0_UART_DM_SR_HUNT_CHAR_SHFT                                                                          7
#define HWIO_MSS_UIM0_UART_DM_SR_RX_BREAK_BMSK                                                                        0x40
#define HWIO_MSS_UIM0_UART_DM_SR_RX_BREAK_SHFT                                                                           6
#define HWIO_MSS_UIM0_UART_DM_SR_PAR_FRAME_ERR_BMSK                                                                   0x20
#define HWIO_MSS_UIM0_UART_DM_SR_PAR_FRAME_ERR_SHFT                                                                      5
#define HWIO_MSS_UIM0_UART_DM_SR_UART_OVERRUN_BMSK                                                                    0x10
#define HWIO_MSS_UIM0_UART_DM_SR_UART_OVERRUN_SHFT                                                                       4
#define HWIO_MSS_UIM0_UART_DM_SR_TXEMT_BMSK                                                                            0x8
#define HWIO_MSS_UIM0_UART_DM_SR_TXEMT_SHFT                                                                              3
#define HWIO_MSS_UIM0_UART_DM_SR_TXRDY_BMSK                                                                            0x4
#define HWIO_MSS_UIM0_UART_DM_SR_TXRDY_SHFT                                                                              2
#define HWIO_MSS_UIM0_UART_DM_SR_RXFULL_BMSK                                                                           0x2
#define HWIO_MSS_UIM0_UART_DM_SR_RXFULL_SHFT                                                                             1
#define HWIO_MSS_UIM0_UART_DM_SR_RXRDY_BMSK                                                                            0x1
#define HWIO_MSS_UIM0_UART_DM_SR_RXRDY_SHFT                                                                              0

#define HWIO_MSS_UIM0_UART_DM_CR_ADDR                                                                           (MSS_UIM0_UART_DM_REG_BASE            + 0xa8)
#define HWIO_MSS_UIM0_UART_DM_CR_RMSK                                                                                0xfff
#define HWIO_MSS_UIM0_UART_DM_CR_IN                    \
                in_dword(HWIO_MSS_UIM0_UART_DM_CR_ADDR)
#define HWIO_MSS_UIM0_UART_DM_CR_INM(m)            \
                in_dword_masked(HWIO_MSS_UIM0_UART_DM_CR_ADDR, m)
#define HWIO_MSS_UIM0_UART_DM_CR_OUT(v)            \
                out_dword(HWIO_MSS_UIM0_UART_DM_CR_ADDR,v)
#define HWIO_MSS_UIM0_UART_DM_CR_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_UIM0_UART_DM_CR_ADDR,m,v,HWIO_MSS_UIM0_UART_DM_CR_IN)
#define HWIO_MSS_UIM0_UART_DM_CR_CHANNEL_COMMAND_MSB_BMSK                                                            0x800
#define HWIO_MSS_UIM0_UART_DM_CR_CHANNEL_COMMAND_MSB_SHFT                                                               11
#define HWIO_MSS_UIM0_UART_DM_CR_GENERAL_COMMAND_BMSK                                                                0x700
#define HWIO_MSS_UIM0_UART_DM_CR_GENERAL_COMMAND_SHFT                                                                    8
#define HWIO_MSS_UIM0_UART_DM_CR_CHANNEL_COMMAND_LSB_BMSK                                                             0xf0
#define HWIO_MSS_UIM0_UART_DM_CR_CHANNEL_COMMAND_LSB_SHFT                                                                4
#define HWIO_MSS_UIM0_UART_DM_CR_UART_TX_DISABLE_BMSK                                                                  0x8
#define HWIO_MSS_UIM0_UART_DM_CR_UART_TX_DISABLE_SHFT                                                                    3
#define HWIO_MSS_UIM0_UART_DM_CR_UART_TX_EN_BMSK                                                                       0x4
#define HWIO_MSS_UIM0_UART_DM_CR_UART_TX_EN_SHFT                                                                         2
#define HWIO_MSS_UIM0_UART_DM_CR_UART_RX_DISABLE_BMSK                                                                  0x2
#define HWIO_MSS_UIM0_UART_DM_CR_UART_RX_DISABLE_SHFT                                                                    1
#define HWIO_MSS_UIM0_UART_DM_CR_UART_RX_EN_BMSK                                                                       0x1
#define HWIO_MSS_UIM0_UART_DM_CR_UART_RX_EN_SHFT                                                                         0

#define HWIO_MSS_UIM0_UART_DM_MISR_ADDR                                                                         (MSS_UIM0_UART_DM_REG_BASE            + 0xac)
#define HWIO_MSS_UIM0_UART_DM_MISR_RMSK                                                                            0x3ffff
#define HWIO_MSS_UIM0_UART_DM_MISR_IN                    \
                in_dword(HWIO_MSS_UIM0_UART_DM_MISR_ADDR)
#define HWIO_MSS_UIM0_UART_DM_MISR_INM(m)            \
                in_dword_masked(HWIO_MSS_UIM0_UART_DM_MISR_ADDR, m)
#define HWIO_MSS_UIM0_UART_DM_MISR_UART_MISR_BMSK                                                                  0x3ffff
#define HWIO_MSS_UIM0_UART_DM_MISR_UART_MISR_SHFT                                                                        0

#define HWIO_MSS_UIM0_UART_DM_IMR_ADDR                                                                          (MSS_UIM0_UART_DM_REG_BASE            + 0xb0)
#define HWIO_MSS_UIM0_UART_DM_IMR_RMSK                                                                             0x3ffff
#define HWIO_MSS_UIM0_UART_DM_IMR_IN                    \
                in_dword(HWIO_MSS_UIM0_UART_DM_IMR_ADDR)
#define HWIO_MSS_UIM0_UART_DM_IMR_INM(m)            \
                in_dword_masked(HWIO_MSS_UIM0_UART_DM_IMR_ADDR, m)
#define HWIO_MSS_UIM0_UART_DM_IMR_OUT(v)            \
                out_dword(HWIO_MSS_UIM0_UART_DM_IMR_ADDR,v)
#define HWIO_MSS_UIM0_UART_DM_IMR_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_UIM0_UART_DM_IMR_ADDR,m,v,HWIO_MSS_UIM0_UART_DM_IMR_IN)
#define HWIO_MSS_UIM0_UART_DM_IMR_NO_FINISH_CMD_VIOL_BMSK                                                          0x20000
#define HWIO_MSS_UIM0_UART_DM_IMR_NO_FINISH_CMD_VIOL_SHFT                                                               17
#define HWIO_MSS_UIM0_UART_DM_IMR_WWT_IRQ_BMSK                                                                     0x10000
#define HWIO_MSS_UIM0_UART_DM_IMR_WWT_IRQ_SHFT                                                                          16
#define HWIO_MSS_UIM0_UART_DM_IMR_TXCOMP_IRQ_BMSK                                                                   0x8000
#define HWIO_MSS_UIM0_UART_DM_IMR_TXCOMP_IRQ_SHFT                                                                       15
#define HWIO_MSS_UIM0_UART_DM_IMR_RX_RD_ERROR_IRQ_BMSK                                                              0x4000
#define HWIO_MSS_UIM0_UART_DM_IMR_RX_RD_ERROR_IRQ_SHFT                                                                  14
#define HWIO_MSS_UIM0_UART_DM_IMR_TX_WR_ERROR_IRQ_BMSK                                                              0x2000
#define HWIO_MSS_UIM0_UART_DM_IMR_TX_WR_ERROR_IRQ_SHFT                                                                  13
#define HWIO_MSS_UIM0_UART_DM_IMR_PAR_FRAME_ERR_IRQ_BMSK                                                            0x1000
#define HWIO_MSS_UIM0_UART_DM_IMR_PAR_FRAME_ERR_IRQ_SHFT                                                                12
#define HWIO_MSS_UIM0_UART_DM_IMR_RXBREAK_END_BMSK                                                                   0x800
#define HWIO_MSS_UIM0_UART_DM_IMR_RXBREAK_END_SHFT                                                                      11
#define HWIO_MSS_UIM0_UART_DM_IMR_RXBREAK_START_BMSK                                                                 0x400
#define HWIO_MSS_UIM0_UART_DM_IMR_RXBREAK_START_SHFT                                                                    10
#define HWIO_MSS_UIM0_UART_DM_IMR_TX_DONE_BMSK                                                                       0x200
#define HWIO_MSS_UIM0_UART_DM_IMR_TX_DONE_SHFT                                                                           9
#define HWIO_MSS_UIM0_UART_DM_IMR_TX_ERROR_BMSK                                                                      0x100
#define HWIO_MSS_UIM0_UART_DM_IMR_TX_ERROR_SHFT                                                                          8
#define HWIO_MSS_UIM0_UART_DM_IMR_TX_READY_BMSK                                                                       0x80
#define HWIO_MSS_UIM0_UART_DM_IMR_TX_READY_SHFT                                                                          7
#define HWIO_MSS_UIM0_UART_DM_IMR_CURRENT_CTS_BMSK                                                                    0x40
#define HWIO_MSS_UIM0_UART_DM_IMR_CURRENT_CTS_SHFT                                                                       6
#define HWIO_MSS_UIM0_UART_DM_IMR_DELTA_CTS_BMSK                                                                      0x20
#define HWIO_MSS_UIM0_UART_DM_IMR_DELTA_CTS_SHFT                                                                         5
#define HWIO_MSS_UIM0_UART_DM_IMR_RXLEV_BMSK                                                                          0x10
#define HWIO_MSS_UIM0_UART_DM_IMR_RXLEV_SHFT                                                                             4
#define HWIO_MSS_UIM0_UART_DM_IMR_RXSTALE_BMSK                                                                         0x8
#define HWIO_MSS_UIM0_UART_DM_IMR_RXSTALE_SHFT                                                                           3
#define HWIO_MSS_UIM0_UART_DM_IMR_RXBREAK_CHANGE_BMSK                                                                  0x4
#define HWIO_MSS_UIM0_UART_DM_IMR_RXBREAK_CHANGE_SHFT                                                                    2
#define HWIO_MSS_UIM0_UART_DM_IMR_RXHUNT_BMSK                                                                          0x2
#define HWIO_MSS_UIM0_UART_DM_IMR_RXHUNT_SHFT                                                                            1
#define HWIO_MSS_UIM0_UART_DM_IMR_TXLEV_BMSK                                                                           0x1
#define HWIO_MSS_UIM0_UART_DM_IMR_TXLEV_SHFT                                                                             0

#define HWIO_MSS_UIM0_UART_DM_ISR_ADDR                                                                          (MSS_UIM0_UART_DM_REG_BASE            + 0xb4)
#define HWIO_MSS_UIM0_UART_DM_ISR_RMSK                                                                             0x3ffff
#define HWIO_MSS_UIM0_UART_DM_ISR_IN                    \
                in_dword(HWIO_MSS_UIM0_UART_DM_ISR_ADDR)
#define HWIO_MSS_UIM0_UART_DM_ISR_INM(m)            \
                in_dword_masked(HWIO_MSS_UIM0_UART_DM_ISR_ADDR, m)
#define HWIO_MSS_UIM0_UART_DM_ISR_NO_FINISH_CMD_VIOL_BMSK                                                          0x20000
#define HWIO_MSS_UIM0_UART_DM_ISR_NO_FINISH_CMD_VIOL_SHFT                                                               17
#define HWIO_MSS_UIM0_UART_DM_ISR_WWT_IRQ_BMSK                                                                     0x10000
#define HWIO_MSS_UIM0_UART_DM_ISR_WWT_IRQ_SHFT                                                                          16
#define HWIO_MSS_UIM0_UART_DM_ISR_TXCOMP_IRQ_BMSK                                                                   0x8000
#define HWIO_MSS_UIM0_UART_DM_ISR_TXCOMP_IRQ_SHFT                                                                       15
#define HWIO_MSS_UIM0_UART_DM_ISR_RX_RD_ERROR_IRQ_BMSK                                                              0x4000
#define HWIO_MSS_UIM0_UART_DM_ISR_RX_RD_ERROR_IRQ_SHFT                                                                  14
#define HWIO_MSS_UIM0_UART_DM_ISR_TX_WR_ERROR_IRQ_BMSK                                                              0x2000
#define HWIO_MSS_UIM0_UART_DM_ISR_TX_WR_ERROR_IRQ_SHFT                                                                  13
#define HWIO_MSS_UIM0_UART_DM_ISR_PAR_FRAME_ERR_IRQ_BMSK                                                            0x1000
#define HWIO_MSS_UIM0_UART_DM_ISR_PAR_FRAME_ERR_IRQ_SHFT                                                                12
#define HWIO_MSS_UIM0_UART_DM_ISR_RXBREAK_END_BMSK                                                                   0x800
#define HWIO_MSS_UIM0_UART_DM_ISR_RXBREAK_END_SHFT                                                                      11
#define HWIO_MSS_UIM0_UART_DM_ISR_RXBREAK_START_BMSK                                                                 0x400
#define HWIO_MSS_UIM0_UART_DM_ISR_RXBREAK_START_SHFT                                                                    10
#define HWIO_MSS_UIM0_UART_DM_ISR_TX_DONE_BMSK                                                                       0x200
#define HWIO_MSS_UIM0_UART_DM_ISR_TX_DONE_SHFT                                                                           9
#define HWIO_MSS_UIM0_UART_DM_ISR_TX_ERROR_BMSK                                                                      0x100
#define HWIO_MSS_UIM0_UART_DM_ISR_TX_ERROR_SHFT                                                                          8
#define HWIO_MSS_UIM0_UART_DM_ISR_TX_READY_BMSK                                                                       0x80
#define HWIO_MSS_UIM0_UART_DM_ISR_TX_READY_SHFT                                                                          7
#define HWIO_MSS_UIM0_UART_DM_ISR_CURRENT_CTS_BMSK                                                                    0x40
#define HWIO_MSS_UIM0_UART_DM_ISR_CURRENT_CTS_SHFT                                                                       6
#define HWIO_MSS_UIM0_UART_DM_ISR_DELTA_CTS_BMSK                                                                      0x20
#define HWIO_MSS_UIM0_UART_DM_ISR_DELTA_CTS_SHFT                                                                         5
#define HWIO_MSS_UIM0_UART_DM_ISR_RXLEV_BMSK                                                                          0x10
#define HWIO_MSS_UIM0_UART_DM_ISR_RXLEV_SHFT                                                                             4
#define HWIO_MSS_UIM0_UART_DM_ISR_RXSTALE_BMSK                                                                         0x8
#define HWIO_MSS_UIM0_UART_DM_ISR_RXSTALE_SHFT                                                                           3
#define HWIO_MSS_UIM0_UART_DM_ISR_RXBREAK_CHANGE_BMSK                                                                  0x4
#define HWIO_MSS_UIM0_UART_DM_ISR_RXBREAK_CHANGE_SHFT                                                                    2
#define HWIO_MSS_UIM0_UART_DM_ISR_RXHUNT_BMSK                                                                          0x2
#define HWIO_MSS_UIM0_UART_DM_ISR_RXHUNT_SHFT                                                                            1
#define HWIO_MSS_UIM0_UART_DM_ISR_TXLEV_BMSK                                                                           0x1
#define HWIO_MSS_UIM0_UART_DM_ISR_TXLEV_SHFT                                                                             0

#define HWIO_MSS_UIM0_UART_DM_IRDA_ADDR                                                                         (MSS_UIM0_UART_DM_REG_BASE            + 0xb8)
#define HWIO_MSS_UIM0_UART_DM_IRDA_RMSK                                                                               0x1f
#define HWIO_MSS_UIM0_UART_DM_IRDA_IN                    \
                in_dword(HWIO_MSS_UIM0_UART_DM_IRDA_ADDR)
#define HWIO_MSS_UIM0_UART_DM_IRDA_INM(m)            \
                in_dword_masked(HWIO_MSS_UIM0_UART_DM_IRDA_ADDR, m)
#define HWIO_MSS_UIM0_UART_DM_IRDA_OUT(v)            \
                out_dword(HWIO_MSS_UIM0_UART_DM_IRDA_ADDR,v)
#define HWIO_MSS_UIM0_UART_DM_IRDA_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_UIM0_UART_DM_IRDA_ADDR,m,v,HWIO_MSS_UIM0_UART_DM_IRDA_IN)
#define HWIO_MSS_UIM0_UART_DM_IRDA_MEDIUM_RATE_EN_BMSK                                                                0x10
#define HWIO_MSS_UIM0_UART_DM_IRDA_MEDIUM_RATE_EN_SHFT                                                                   4
#define HWIO_MSS_UIM0_UART_DM_IRDA_IRDA_LOOPBACK_BMSK                                                                  0x8
#define HWIO_MSS_UIM0_UART_DM_IRDA_IRDA_LOOPBACK_SHFT                                                                    3
#define HWIO_MSS_UIM0_UART_DM_IRDA_INVERT_IRDA_TX_BMSK                                                                 0x4
#define HWIO_MSS_UIM0_UART_DM_IRDA_INVERT_IRDA_TX_SHFT                                                                   2
#define HWIO_MSS_UIM0_UART_DM_IRDA_INVERT_IRDA_RX_BMSK                                                                 0x2
#define HWIO_MSS_UIM0_UART_DM_IRDA_INVERT_IRDA_RX_SHFT                                                                   1
#define HWIO_MSS_UIM0_UART_DM_IRDA_IRDA_EN_BMSK                                                                        0x1
#define HWIO_MSS_UIM0_UART_DM_IRDA_IRDA_EN_SHFT                                                                          0

#define HWIO_MSS_UIM0_UART_DM_RX_TOTAL_SNAP_ADDR                                                                (MSS_UIM0_UART_DM_REG_BASE            + 0xbc)
#define HWIO_MSS_UIM0_UART_DM_RX_TOTAL_SNAP_RMSK                                                                  0xffffff
#define HWIO_MSS_UIM0_UART_DM_RX_TOTAL_SNAP_IN                    \
                in_dword(HWIO_MSS_UIM0_UART_DM_RX_TOTAL_SNAP_ADDR)
#define HWIO_MSS_UIM0_UART_DM_RX_TOTAL_SNAP_INM(m)            \
                in_dword_masked(HWIO_MSS_UIM0_UART_DM_RX_TOTAL_SNAP_ADDR, m)
#define HWIO_MSS_UIM0_UART_DM_RX_TOTAL_SNAP_RX_TOTAL_BYTES_BMSK                                                   0xffffff
#define HWIO_MSS_UIM0_UART_DM_RX_TOTAL_SNAP_RX_TOTAL_BYTES_SHFT                                                          0

#define HWIO_MSS_UIM0_UART_DM_WWT_TIMEOUT_ADDR                                                                  (MSS_UIM0_UART_DM_REG_BASE            + 0xc0)
#define HWIO_MSS_UIM0_UART_DM_WWT_TIMEOUT_RMSK                                                                   0x3ffffff
#define HWIO_MSS_UIM0_UART_DM_WWT_TIMEOUT_IN                    \
                in_dword(HWIO_MSS_UIM0_UART_DM_WWT_TIMEOUT_ADDR)
#define HWIO_MSS_UIM0_UART_DM_WWT_TIMEOUT_INM(m)            \
                in_dword_masked(HWIO_MSS_UIM0_UART_DM_WWT_TIMEOUT_ADDR, m)
#define HWIO_MSS_UIM0_UART_DM_WWT_TIMEOUT_OUT(v)            \
                out_dword(HWIO_MSS_UIM0_UART_DM_WWT_TIMEOUT_ADDR,v)
#define HWIO_MSS_UIM0_UART_DM_WWT_TIMEOUT_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_UIM0_UART_DM_WWT_TIMEOUT_ADDR,m,v,HWIO_MSS_UIM0_UART_DM_WWT_TIMEOUT_IN)
#define HWIO_MSS_UIM0_UART_DM_WWT_TIMEOUT_WWT_CYCLE_REENABLE_BMSK                                                0x2000000
#define HWIO_MSS_UIM0_UART_DM_WWT_TIMEOUT_WWT_CYCLE_REENABLE_SHFT                                                       25
#define HWIO_MSS_UIM0_UART_DM_WWT_TIMEOUT_WWT_TIMEOUT_BMSK                                                       0x1ffffff
#define HWIO_MSS_UIM0_UART_DM_WWT_TIMEOUT_WWT_TIMEOUT_SHFT                                                               0

#define HWIO_MSS_UIM0_UART_DM_CLK_CTRL_ADDR                                                                     (MSS_UIM0_UART_DM_REG_BASE            + 0xc4)
#define HWIO_MSS_UIM0_UART_DM_CLK_CTRL_RMSK                                                                       0xf06731
#define HWIO_MSS_UIM0_UART_DM_CLK_CTRL_IN                    \
                in_dword(HWIO_MSS_UIM0_UART_DM_CLK_CTRL_ADDR)
#define HWIO_MSS_UIM0_UART_DM_CLK_CTRL_INM(m)            \
                in_dword_masked(HWIO_MSS_UIM0_UART_DM_CLK_CTRL_ADDR, m)
#define HWIO_MSS_UIM0_UART_DM_CLK_CTRL_OUT(v)            \
                out_dword(HWIO_MSS_UIM0_UART_DM_CLK_CTRL_ADDR,v)
#define HWIO_MSS_UIM0_UART_DM_CLK_CTRL_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_UIM0_UART_DM_CLK_CTRL_ADDR,m,v,HWIO_MSS_UIM0_UART_DM_CLK_CTRL_IN)
#define HWIO_MSS_UIM0_UART_DM_CLK_CTRL_UART_IRDA_CLK_CGC_OPEN_BMSK                                                0x800000
#define HWIO_MSS_UIM0_UART_DM_CLK_CTRL_UART_IRDA_CLK_CGC_OPEN_SHFT                                                      23
#define HWIO_MSS_UIM0_UART_DM_CLK_CTRL_UART_SIM_CLK_CGC_OPEN_BMSK                                                 0x400000
#define HWIO_MSS_UIM0_UART_DM_CLK_CTRL_UART_SIM_CLK_CGC_OPEN_SHFT                                                       22
#define HWIO_MSS_UIM0_UART_DM_CLK_CTRL_UART_RX_CLK_CGC_OPEN_BMSK                                                  0x200000
#define HWIO_MSS_UIM0_UART_DM_CLK_CTRL_UART_RX_CLK_CGC_OPEN_SHFT                                                        21
#define HWIO_MSS_UIM0_UART_DM_CLK_CTRL_UART_TX_CLK_CGC_OPEN_BMSK                                                  0x100000
#define HWIO_MSS_UIM0_UART_DM_CLK_CTRL_UART_TX_CLK_CGC_OPEN_SHFT                                                        20
#define HWIO_MSS_UIM0_UART_DM_CLK_CTRL_AHB_RX_BAM_CLK_CGC_OPEN_BMSK                                                 0x4000
#define HWIO_MSS_UIM0_UART_DM_CLK_CTRL_AHB_RX_BAM_CLK_CGC_OPEN_SHFT                                                     14
#define HWIO_MSS_UIM0_UART_DM_CLK_CTRL_AHB_TX_BAM_CLK_CGC_OPEN_BMSK                                                 0x2000
#define HWIO_MSS_UIM0_UART_DM_CLK_CTRL_AHB_TX_BAM_CLK_CGC_OPEN_SHFT                                                     13
#define HWIO_MSS_UIM0_UART_DM_CLK_CTRL_AHB_RX_CLK_CGC_OPEN_BMSK                                                      0x400
#define HWIO_MSS_UIM0_UART_DM_CLK_CTRL_AHB_RX_CLK_CGC_OPEN_SHFT                                                         10
#define HWIO_MSS_UIM0_UART_DM_CLK_CTRL_AHB_TX_CLK_CGC_OPEN_BMSK                                                      0x200
#define HWIO_MSS_UIM0_UART_DM_CLK_CTRL_AHB_TX_CLK_CGC_OPEN_SHFT                                                          9
#define HWIO_MSS_UIM0_UART_DM_CLK_CTRL_AHB_WR_CLK_CGC_OPEN_BMSK                                                      0x100
#define HWIO_MSS_UIM0_UART_DM_CLK_CTRL_AHB_WR_CLK_CGC_OPEN_SHFT                                                          8
#define HWIO_MSS_UIM0_UART_DM_CLK_CTRL_RX_ENABLE_CGC_OPT_BMSK                                                         0x20
#define HWIO_MSS_UIM0_UART_DM_CLK_CTRL_RX_ENABLE_CGC_OPT_SHFT                                                            5
#define HWIO_MSS_UIM0_UART_DM_CLK_CTRL_TX_ENABLE_CGC_OPT_BMSK                                                         0x10
#define HWIO_MSS_UIM0_UART_DM_CLK_CTRL_TX_ENABLE_CGC_OPT_SHFT                                                            4
#define HWIO_MSS_UIM0_UART_DM_CLK_CTRL_AHB_CLK_CGC_CLOSE_BMSK                                                          0x1
#define HWIO_MSS_UIM0_UART_DM_CLK_CTRL_AHB_CLK_CGC_CLOSE_SHFT                                                            0

#define HWIO_MSS_UIM0_UART_DM_BCR_ADDR                                                                          (MSS_UIM0_UART_DM_REG_BASE            + 0xc8)
#define HWIO_MSS_UIM0_UART_DM_BCR_RMSK                                                                                0x77
#define HWIO_MSS_UIM0_UART_DM_BCR_IN                    \
                in_dword(HWIO_MSS_UIM0_UART_DM_BCR_ADDR)
#define HWIO_MSS_UIM0_UART_DM_BCR_INM(m)            \
                in_dword_masked(HWIO_MSS_UIM0_UART_DM_BCR_ADDR, m)
#define HWIO_MSS_UIM0_UART_DM_BCR_OUT(v)            \
                out_dword(HWIO_MSS_UIM0_UART_DM_BCR_ADDR,v)
#define HWIO_MSS_UIM0_UART_DM_BCR_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_UIM0_UART_DM_BCR_ADDR,m,v,HWIO_MSS_UIM0_UART_DM_BCR_IN)
#define HWIO_MSS_UIM0_UART_DM_BCR_IGNORE_CR_PROT_VIOL_BMSK                                                            0x40
#define HWIO_MSS_UIM0_UART_DM_BCR_IGNORE_CR_PROT_VIOL_SHFT                                                               6
#define HWIO_MSS_UIM0_UART_DM_BCR_RX_DMRX_1BYTE_RES_EN_BMSK                                                           0x20
#define HWIO_MSS_UIM0_UART_DM_BCR_RX_DMRX_1BYTE_RES_EN_SHFT                                                              5
#define HWIO_MSS_UIM0_UART_DM_BCR_RX_STALE_IRQ_DMRX_EQUAL_BMSK                                                        0x10
#define HWIO_MSS_UIM0_UART_DM_BCR_RX_STALE_IRQ_DMRX_EQUAL_SHFT                                                           4
#define HWIO_MSS_UIM0_UART_DM_BCR_RX_DMRX_LOW_EN_BMSK                                                                  0x4
#define HWIO_MSS_UIM0_UART_DM_BCR_RX_DMRX_LOW_EN_SHFT                                                                    2
#define HWIO_MSS_UIM0_UART_DM_BCR_STALE_IRQ_EMPTY_BMSK                                                                 0x2
#define HWIO_MSS_UIM0_UART_DM_BCR_STALE_IRQ_EMPTY_SHFT                                                                   1
#define HWIO_MSS_UIM0_UART_DM_BCR_TX_BREAK_DISABLE_BMSK                                                                0x1
#define HWIO_MSS_UIM0_UART_DM_BCR_TX_BREAK_DISABLE_SHFT                                                                  0

#define HWIO_MSS_UIM0_UART_DM_RX_TRANS_CTRL_ADDR                                                                (MSS_UIM0_UART_DM_REG_BASE            + 0xcc)
#define HWIO_MSS_UIM0_UART_DM_RX_TRANS_CTRL_RMSK                                                                       0x7
#define HWIO_MSS_UIM0_UART_DM_RX_TRANS_CTRL_IN                    \
                in_dword(HWIO_MSS_UIM0_UART_DM_RX_TRANS_CTRL_ADDR)
#define HWIO_MSS_UIM0_UART_DM_RX_TRANS_CTRL_INM(m)            \
                in_dword_masked(HWIO_MSS_UIM0_UART_DM_RX_TRANS_CTRL_ADDR, m)
#define HWIO_MSS_UIM0_UART_DM_RX_TRANS_CTRL_OUT(v)            \
                out_dword(HWIO_MSS_UIM0_UART_DM_RX_TRANS_CTRL_ADDR,v)
#define HWIO_MSS_UIM0_UART_DM_RX_TRANS_CTRL_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_UIM0_UART_DM_RX_TRANS_CTRL_ADDR,m,v,HWIO_MSS_UIM0_UART_DM_RX_TRANS_CTRL_IN)
#define HWIO_MSS_UIM0_UART_DM_RX_TRANS_CTRL_RX_DMRX_CYCLIC_EN_BMSK                                                     0x4
#define HWIO_MSS_UIM0_UART_DM_RX_TRANS_CTRL_RX_DMRX_CYCLIC_EN_SHFT                                                       2
#define HWIO_MSS_UIM0_UART_DM_RX_TRANS_CTRL_RX_TRANS_AUTO_RE_ACTIVATE_BMSK                                             0x2
#define HWIO_MSS_UIM0_UART_DM_RX_TRANS_CTRL_RX_TRANS_AUTO_RE_ACTIVATE_SHFT                                               1
#define HWIO_MSS_UIM0_UART_DM_RX_TRANS_CTRL_RX_STALE_AUTO_RE_EN_BMSK                                                   0x1
#define HWIO_MSS_UIM0_UART_DM_RX_TRANS_CTRL_RX_STALE_AUTO_RE_EN_SHFT                                                     0

#define HWIO_MSS_UIM0_UART_DM_DMRX_DBG_ADDR                                                                     (MSS_UIM0_UART_DM_REG_BASE            + 0xd0)
#define HWIO_MSS_UIM0_UART_DM_DMRX_DBG_RMSK                                                                      0x1ffffff
#define HWIO_MSS_UIM0_UART_DM_DMRX_DBG_IN                    \
                in_dword(HWIO_MSS_UIM0_UART_DM_DMRX_DBG_ADDR)
#define HWIO_MSS_UIM0_UART_DM_DMRX_DBG_INM(m)            \
                in_dword_masked(HWIO_MSS_UIM0_UART_DM_DMRX_DBG_ADDR, m)
#define HWIO_MSS_UIM0_UART_DM_DMRX_DBG_UART_DM_DMRX_VAL_BMSK                                                     0x1ffffff
#define HWIO_MSS_UIM0_UART_DM_DMRX_DBG_UART_DM_DMRX_VAL_SHFT                                                             0

#define HWIO_MSS_UIM0_UART_DM_FSM_STATUS_ADDR                                                                   (MSS_UIM0_UART_DM_REG_BASE            + 0xd4)
#define HWIO_MSS_UIM0_UART_DM_FSM_STATUS_RMSK                                                                   0x37377771
#define HWIO_MSS_UIM0_UART_DM_FSM_STATUS_IN                    \
                in_dword(HWIO_MSS_UIM0_UART_DM_FSM_STATUS_ADDR)
#define HWIO_MSS_UIM0_UART_DM_FSM_STATUS_INM(m)            \
                in_dword_masked(HWIO_MSS_UIM0_UART_DM_FSM_STATUS_ADDR, m)
#define HWIO_MSS_UIM0_UART_DM_FSM_STATUS_TX_COMP_FSM_BMSK                                                       0x30000000
#define HWIO_MSS_UIM0_UART_DM_FSM_STATUS_TX_COMP_FSM_SHFT                                                               28
#define HWIO_MSS_UIM0_UART_DM_FSM_STATUS_RX_PACK_FSM_BMSK                                                        0x7000000
#define HWIO_MSS_UIM0_UART_DM_FSM_STATUS_RX_PACK_FSM_SHFT                                                               24
#define HWIO_MSS_UIM0_UART_DM_FSM_STATUS_RX_TRANS_FSM_BMSK                                                        0x300000
#define HWIO_MSS_UIM0_UART_DM_FSM_STATUS_RX_TRANS_FSM_SHFT                                                              20
#define HWIO_MSS_UIM0_UART_DM_FSM_STATUS_TX_TRANS_FSM_BMSK                                                         0x70000
#define HWIO_MSS_UIM0_UART_DM_FSM_STATUS_TX_TRANS_FSM_SHFT                                                              16
#define HWIO_MSS_UIM0_UART_DM_FSM_STATUS_RX_PRO_TRANS_END_FSM_BMSK                                                  0x7000
#define HWIO_MSS_UIM0_UART_DM_FSM_STATUS_RX_PRO_TRANS_END_FSM_SHFT                                                      12
#define HWIO_MSS_UIM0_UART_DM_FSM_STATUS_RX_PRO_ACTIVE_FSM_BMSK                                                      0x700
#define HWIO_MSS_UIM0_UART_DM_FSM_STATUS_RX_PRO_ACTIVE_FSM_SHFT                                                          8
#define HWIO_MSS_UIM0_UART_DM_FSM_STATUS_TX_CON_TRANS_END_FSM_BMSK                                                    0x70
#define HWIO_MSS_UIM0_UART_DM_FSM_STATUS_TX_CON_TRANS_END_FSM_SHFT                                                       4
#define HWIO_MSS_UIM0_UART_DM_FSM_STATUS_RX_TRANSFER_ACTIVE_BMSK                                                       0x1
#define HWIO_MSS_UIM0_UART_DM_FSM_STATUS_RX_TRANSFER_ACTIVE_SHFT                                                         0

#define HWIO_MSS_UIM0_UART_DM_HW_VERSION_ADDR                                                                   (MSS_UIM0_UART_DM_REG_BASE            + 0xd8)
#define HWIO_MSS_UIM0_UART_DM_HW_VERSION_RMSK                                                                   0xffffffff
#define HWIO_MSS_UIM0_UART_DM_HW_VERSION_IN                    \
                in_dword(HWIO_MSS_UIM0_UART_DM_HW_VERSION_ADDR)
#define HWIO_MSS_UIM0_UART_DM_HW_VERSION_INM(m)            \
                in_dword_masked(HWIO_MSS_UIM0_UART_DM_HW_VERSION_ADDR, m)
#define HWIO_MSS_UIM0_UART_DM_HW_VERSION_HW_VERSION_MAJOR_BMSK                                                  0xf0000000
#define HWIO_MSS_UIM0_UART_DM_HW_VERSION_HW_VERSION_MAJOR_SHFT                                                          28
#define HWIO_MSS_UIM0_UART_DM_HW_VERSION_HW_VERSION_MINOR_BMSK                                                   0xfff0000
#define HWIO_MSS_UIM0_UART_DM_HW_VERSION_HW_VERSION_MINOR_SHFT                                                          16
#define HWIO_MSS_UIM0_UART_DM_HW_VERSION_HW_VERSION_STEP_BMSK                                                       0xffff
#define HWIO_MSS_UIM0_UART_DM_HW_VERSION_HW_VERSION_STEP_SHFT                                                            0

#define HWIO_MSS_UIM0_UART_DM_GENERICS_ADDR                                                                     (MSS_UIM0_UART_DM_REG_BASE            + 0xdc)
#define HWIO_MSS_UIM0_UART_DM_GENERICS_RMSK                                                                           0xff
#define HWIO_MSS_UIM0_UART_DM_GENERICS_IN                    \
                in_dword(HWIO_MSS_UIM0_UART_DM_GENERICS_ADDR)
#define HWIO_MSS_UIM0_UART_DM_GENERICS_INM(m)            \
                in_dword_masked(HWIO_MSS_UIM0_UART_DM_GENERICS_ADDR, m)
#define HWIO_MSS_UIM0_UART_DM_GENERICS_GENERIC_BAM_IFC_BMSK                                                           0x80
#define HWIO_MSS_UIM0_UART_DM_GENERICS_GENERIC_BAM_IFC_SHFT                                                              7
#define HWIO_MSS_UIM0_UART_DM_GENERICS_GENERIC_DM_IFC_BMSK                                                            0x40
#define HWIO_MSS_UIM0_UART_DM_GENERICS_GENERIC_DM_IFC_SHFT                                                               6
#define HWIO_MSS_UIM0_UART_DM_GENERICS_GENERIC_IRDA_IFC_BMSK                                                          0x20
#define HWIO_MSS_UIM0_UART_DM_GENERICS_GENERIC_IRDA_IFC_SHFT                                                             5
#define HWIO_MSS_UIM0_UART_DM_GENERICS_GENERIC_SIM_GLUE_BMSK                                                          0x10
#define HWIO_MSS_UIM0_UART_DM_GENERICS_GENERIC_SIM_GLUE_SHFT                                                             4
#define HWIO_MSS_UIM0_UART_DM_GENERICS_GENERIC_RAM_ADDR_WIDTH_BMSK                                                     0xf
#define HWIO_MSS_UIM0_UART_DM_GENERICS_GENERIC_RAM_ADDR_WIDTH_SHFT                                                       0

#define HWIO_MSS_UIM0_UART_DM_ISR_CLR_ADDR                                                                      (MSS_UIM0_UART_DM_REG_BASE            + 0xe0)
#define HWIO_MSS_UIM0_UART_DM_ISR_CLR_RMSK                                                                         0x20000
#define HWIO_MSS_UIM0_UART_DM_ISR_CLR_OUT(v)            \
                out_dword(HWIO_MSS_UIM0_UART_DM_ISR_CLR_ADDR,v)
#define HWIO_MSS_UIM0_UART_DM_ISR_CLR_NO_FINISH_CMD_VIOL_BMSK                                                      0x20000
#define HWIO_MSS_UIM0_UART_DM_ISR_CLR_NO_FINISH_CMD_VIOL_SHFT                                                           17

#define HWIO_MSS_UIM0_UART_DM_TF_ADDR                                                                           (MSS_UIM0_UART_DM_REG_BASE            + 0x100)
#define HWIO_MSS_UIM0_UART_DM_TF_RMSK                                                                           0xffffffff
#define HWIO_MSS_UIM0_UART_DM_TF_OUT(v)            \
                out_dword(HWIO_MSS_UIM0_UART_DM_TF_ADDR,v)
#define HWIO_MSS_UIM0_UART_DM_TF_UART_TF_BMSK                                                                   0xffffffff
#define HWIO_MSS_UIM0_UART_DM_TF_UART_TF_SHFT                                                                            0

#define HWIO_MSS_UIM0_UART_DM_TF_2_ADDR                                                                         (MSS_UIM0_UART_DM_REG_BASE            + 0x104)
#define HWIO_MSS_UIM0_UART_DM_TF_2_RMSK                                                                         0xffffffff
#define HWIO_MSS_UIM0_UART_DM_TF_2_OUT(v)            \
                out_dword(HWIO_MSS_UIM0_UART_DM_TF_2_ADDR,v)
#define HWIO_MSS_UIM0_UART_DM_TF_2_UART_TF_BMSK                                                                 0xffffffff
#define HWIO_MSS_UIM0_UART_DM_TF_2_UART_TF_SHFT                                                                          0

#define HWIO_MSS_UIM0_UART_DM_TF_3_ADDR                                                                         (MSS_UIM0_UART_DM_REG_BASE            + 0x108)
#define HWIO_MSS_UIM0_UART_DM_TF_3_RMSK                                                                         0xffffffff
#define HWIO_MSS_UIM0_UART_DM_TF_3_OUT(v)            \
                out_dword(HWIO_MSS_UIM0_UART_DM_TF_3_ADDR,v)
#define HWIO_MSS_UIM0_UART_DM_TF_3_UART_TF_BMSK                                                                 0xffffffff
#define HWIO_MSS_UIM0_UART_DM_TF_3_UART_TF_SHFT                                                                          0

#define HWIO_MSS_UIM0_UART_DM_TF_4_ADDR                                                                         (MSS_UIM0_UART_DM_REG_BASE            + 0x10c)
#define HWIO_MSS_UIM0_UART_DM_TF_4_RMSK                                                                         0xffffffff
#define HWIO_MSS_UIM0_UART_DM_TF_4_OUT(v)            \
                out_dword(HWIO_MSS_UIM0_UART_DM_TF_4_ADDR,v)
#define HWIO_MSS_UIM0_UART_DM_TF_4_UART_TF_BMSK                                                                 0xffffffff
#define HWIO_MSS_UIM0_UART_DM_TF_4_UART_TF_SHFT                                                                          0

#define HWIO_MSS_UIM0_UART_DM_TF_5_ADDR                                                                         (MSS_UIM0_UART_DM_REG_BASE            + 0x110)
#define HWIO_MSS_UIM0_UART_DM_TF_5_RMSK                                                                         0xffffffff
#define HWIO_MSS_UIM0_UART_DM_TF_5_OUT(v)            \
                out_dword(HWIO_MSS_UIM0_UART_DM_TF_5_ADDR,v)
#define HWIO_MSS_UIM0_UART_DM_TF_5_UART_TF_BMSK                                                                 0xffffffff
#define HWIO_MSS_UIM0_UART_DM_TF_5_UART_TF_SHFT                                                                          0

#define HWIO_MSS_UIM0_UART_DM_TF_6_ADDR                                                                         (MSS_UIM0_UART_DM_REG_BASE            + 0x114)
#define HWIO_MSS_UIM0_UART_DM_TF_6_RMSK                                                                         0xffffffff
#define HWIO_MSS_UIM0_UART_DM_TF_6_OUT(v)            \
                out_dword(HWIO_MSS_UIM0_UART_DM_TF_6_ADDR,v)
#define HWIO_MSS_UIM0_UART_DM_TF_6_UART_TF_BMSK                                                                 0xffffffff
#define HWIO_MSS_UIM0_UART_DM_TF_6_UART_TF_SHFT                                                                          0

#define HWIO_MSS_UIM0_UART_DM_TF_7_ADDR                                                                         (MSS_UIM0_UART_DM_REG_BASE            + 0x118)
#define HWIO_MSS_UIM0_UART_DM_TF_7_RMSK                                                                         0xffffffff
#define HWIO_MSS_UIM0_UART_DM_TF_7_OUT(v)            \
                out_dword(HWIO_MSS_UIM0_UART_DM_TF_7_ADDR,v)
#define HWIO_MSS_UIM0_UART_DM_TF_7_UART_TF_BMSK                                                                 0xffffffff
#define HWIO_MSS_UIM0_UART_DM_TF_7_UART_TF_SHFT                                                                          0

#define HWIO_MSS_UIM0_UART_DM_TF_8_ADDR                                                                         (MSS_UIM0_UART_DM_REG_BASE            + 0x11c)
#define HWIO_MSS_UIM0_UART_DM_TF_8_RMSK                                                                         0xffffffff
#define HWIO_MSS_UIM0_UART_DM_TF_8_OUT(v)            \
                out_dword(HWIO_MSS_UIM0_UART_DM_TF_8_ADDR,v)
#define HWIO_MSS_UIM0_UART_DM_TF_8_UART_TF_BMSK                                                                 0xffffffff
#define HWIO_MSS_UIM0_UART_DM_TF_8_UART_TF_SHFT                                                                          0

#define HWIO_MSS_UIM0_UART_DM_TF_9_ADDR                                                                         (MSS_UIM0_UART_DM_REG_BASE            + 0x120)
#define HWIO_MSS_UIM0_UART_DM_TF_9_RMSK                                                                         0xffffffff
#define HWIO_MSS_UIM0_UART_DM_TF_9_OUT(v)            \
                out_dword(HWIO_MSS_UIM0_UART_DM_TF_9_ADDR,v)
#define HWIO_MSS_UIM0_UART_DM_TF_9_UART_TF_BMSK                                                                 0xffffffff
#define HWIO_MSS_UIM0_UART_DM_TF_9_UART_TF_SHFT                                                                          0

#define HWIO_MSS_UIM0_UART_DM_TF_10_ADDR                                                                        (MSS_UIM0_UART_DM_REG_BASE            + 0x124)
#define HWIO_MSS_UIM0_UART_DM_TF_10_RMSK                                                                        0xffffffff
#define HWIO_MSS_UIM0_UART_DM_TF_10_OUT(v)            \
                out_dword(HWIO_MSS_UIM0_UART_DM_TF_10_ADDR,v)
#define HWIO_MSS_UIM0_UART_DM_TF_10_UART_TF_BMSK                                                                0xffffffff
#define HWIO_MSS_UIM0_UART_DM_TF_10_UART_TF_SHFT                                                                         0

#define HWIO_MSS_UIM0_UART_DM_TF_11_ADDR                                                                        (MSS_UIM0_UART_DM_REG_BASE            + 0x128)
#define HWIO_MSS_UIM0_UART_DM_TF_11_RMSK                                                                        0xffffffff
#define HWIO_MSS_UIM0_UART_DM_TF_11_OUT(v)            \
                out_dword(HWIO_MSS_UIM0_UART_DM_TF_11_ADDR,v)
#define HWIO_MSS_UIM0_UART_DM_TF_11_UART_TF_BMSK                                                                0xffffffff
#define HWIO_MSS_UIM0_UART_DM_TF_11_UART_TF_SHFT                                                                         0

#define HWIO_MSS_UIM0_UART_DM_TF_12_ADDR                                                                        (MSS_UIM0_UART_DM_REG_BASE            + 0x12c)
#define HWIO_MSS_UIM0_UART_DM_TF_12_RMSK                                                                        0xffffffff
#define HWIO_MSS_UIM0_UART_DM_TF_12_OUT(v)            \
                out_dword(HWIO_MSS_UIM0_UART_DM_TF_12_ADDR,v)
#define HWIO_MSS_UIM0_UART_DM_TF_12_UART_TF_BMSK                                                                0xffffffff
#define HWIO_MSS_UIM0_UART_DM_TF_12_UART_TF_SHFT                                                                         0

#define HWIO_MSS_UIM0_UART_DM_TF_13_ADDR                                                                        (MSS_UIM0_UART_DM_REG_BASE            + 0x130)
#define HWIO_MSS_UIM0_UART_DM_TF_13_RMSK                                                                        0xffffffff
#define HWIO_MSS_UIM0_UART_DM_TF_13_OUT(v)            \
                out_dword(HWIO_MSS_UIM0_UART_DM_TF_13_ADDR,v)
#define HWIO_MSS_UIM0_UART_DM_TF_13_UART_TF_BMSK                                                                0xffffffff
#define HWIO_MSS_UIM0_UART_DM_TF_13_UART_TF_SHFT                                                                         0

#define HWIO_MSS_UIM0_UART_DM_TF_14_ADDR                                                                        (MSS_UIM0_UART_DM_REG_BASE            + 0x134)
#define HWIO_MSS_UIM0_UART_DM_TF_14_RMSK                                                                        0xffffffff
#define HWIO_MSS_UIM0_UART_DM_TF_14_OUT(v)            \
                out_dword(HWIO_MSS_UIM0_UART_DM_TF_14_ADDR,v)
#define HWIO_MSS_UIM0_UART_DM_TF_14_UART_TF_BMSK                                                                0xffffffff
#define HWIO_MSS_UIM0_UART_DM_TF_14_UART_TF_SHFT                                                                         0

#define HWIO_MSS_UIM0_UART_DM_TF_15_ADDR                                                                        (MSS_UIM0_UART_DM_REG_BASE            + 0x138)
#define HWIO_MSS_UIM0_UART_DM_TF_15_RMSK                                                                        0xffffffff
#define HWIO_MSS_UIM0_UART_DM_TF_15_OUT(v)            \
                out_dword(HWIO_MSS_UIM0_UART_DM_TF_15_ADDR,v)
#define HWIO_MSS_UIM0_UART_DM_TF_15_UART_TF_BMSK                                                                0xffffffff
#define HWIO_MSS_UIM0_UART_DM_TF_15_UART_TF_SHFT                                                                         0

#define HWIO_MSS_UIM0_UART_DM_TF_16_ADDR                                                                        (MSS_UIM0_UART_DM_REG_BASE            + 0x13c)
#define HWIO_MSS_UIM0_UART_DM_TF_16_RMSK                                                                        0xffffffff
#define HWIO_MSS_UIM0_UART_DM_TF_16_OUT(v)            \
                out_dword(HWIO_MSS_UIM0_UART_DM_TF_16_ADDR,v)
#define HWIO_MSS_UIM0_UART_DM_TF_16_UART_TF_BMSK                                                                0xffffffff
#define HWIO_MSS_UIM0_UART_DM_TF_16_UART_TF_SHFT                                                                         0

#define HWIO_MSS_UIM0_UART_DM_RF_ADDR                                                                           (MSS_UIM0_UART_DM_REG_BASE            + 0x140)
#define HWIO_MSS_UIM0_UART_DM_RF_RMSK                                                                           0xffffffff
#define HWIO_MSS_UIM0_UART_DM_RF_IN                    \
                in_dword(HWIO_MSS_UIM0_UART_DM_RF_ADDR)
#define HWIO_MSS_UIM0_UART_DM_RF_INM(m)            \
                in_dword_masked(HWIO_MSS_UIM0_UART_DM_RF_ADDR, m)
#define HWIO_MSS_UIM0_UART_DM_RF_UART_RF_BMSK                                                                   0xffffffff
#define HWIO_MSS_UIM0_UART_DM_RF_UART_RF_SHFT                                                                            0

#define HWIO_MSS_UIM0_UART_DM_RF_2_ADDR                                                                         (MSS_UIM0_UART_DM_REG_BASE            + 0x144)
#define HWIO_MSS_UIM0_UART_DM_RF_2_RMSK                                                                         0xffffffff
#define HWIO_MSS_UIM0_UART_DM_RF_2_IN                    \
                in_dword(HWIO_MSS_UIM0_UART_DM_RF_2_ADDR)
#define HWIO_MSS_UIM0_UART_DM_RF_2_INM(m)            \
                in_dword_masked(HWIO_MSS_UIM0_UART_DM_RF_2_ADDR, m)
#define HWIO_MSS_UIM0_UART_DM_RF_2_UART_RF_BMSK                                                                 0xffffffff
#define HWIO_MSS_UIM0_UART_DM_RF_2_UART_RF_SHFT                                                                          0

#define HWIO_MSS_UIM0_UART_DM_RF_3_ADDR                                                                         (MSS_UIM0_UART_DM_REG_BASE            + 0x148)
#define HWIO_MSS_UIM0_UART_DM_RF_3_RMSK                                                                         0xffffffff
#define HWIO_MSS_UIM0_UART_DM_RF_3_IN                    \
                in_dword(HWIO_MSS_UIM0_UART_DM_RF_3_ADDR)
#define HWIO_MSS_UIM0_UART_DM_RF_3_INM(m)            \
                in_dword_masked(HWIO_MSS_UIM0_UART_DM_RF_3_ADDR, m)
#define HWIO_MSS_UIM0_UART_DM_RF_3_UART_RF_BMSK                                                                 0xffffffff
#define HWIO_MSS_UIM0_UART_DM_RF_3_UART_RF_SHFT                                                                          0

#define HWIO_MSS_UIM0_UART_DM_RF_4_ADDR                                                                         (MSS_UIM0_UART_DM_REG_BASE            + 0x14c)
#define HWIO_MSS_UIM0_UART_DM_RF_4_RMSK                                                                         0xffffffff
#define HWIO_MSS_UIM0_UART_DM_RF_4_IN                    \
                in_dword(HWIO_MSS_UIM0_UART_DM_RF_4_ADDR)
#define HWIO_MSS_UIM0_UART_DM_RF_4_INM(m)            \
                in_dword_masked(HWIO_MSS_UIM0_UART_DM_RF_4_ADDR, m)
#define HWIO_MSS_UIM0_UART_DM_RF_4_UART_RF_BMSK                                                                 0xffffffff
#define HWIO_MSS_UIM0_UART_DM_RF_4_UART_RF_SHFT                                                                          0

#define HWIO_MSS_UIM0_UART_DM_RF_5_ADDR                                                                         (MSS_UIM0_UART_DM_REG_BASE            + 0x150)
#define HWIO_MSS_UIM0_UART_DM_RF_5_RMSK                                                                         0xffffffff
#define HWIO_MSS_UIM0_UART_DM_RF_5_IN                    \
                in_dword(HWIO_MSS_UIM0_UART_DM_RF_5_ADDR)
#define HWIO_MSS_UIM0_UART_DM_RF_5_INM(m)            \
                in_dword_masked(HWIO_MSS_UIM0_UART_DM_RF_5_ADDR, m)
#define HWIO_MSS_UIM0_UART_DM_RF_5_UART_RF_BMSK                                                                 0xffffffff
#define HWIO_MSS_UIM0_UART_DM_RF_5_UART_RF_SHFT                                                                          0

#define HWIO_MSS_UIM0_UART_DM_RF_6_ADDR                                                                         (MSS_UIM0_UART_DM_REG_BASE            + 0x154)
#define HWIO_MSS_UIM0_UART_DM_RF_6_RMSK                                                                         0xffffffff
#define HWIO_MSS_UIM0_UART_DM_RF_6_IN                    \
                in_dword(HWIO_MSS_UIM0_UART_DM_RF_6_ADDR)
#define HWIO_MSS_UIM0_UART_DM_RF_6_INM(m)            \
                in_dword_masked(HWIO_MSS_UIM0_UART_DM_RF_6_ADDR, m)
#define HWIO_MSS_UIM0_UART_DM_RF_6_UART_RF_BMSK                                                                 0xffffffff
#define HWIO_MSS_UIM0_UART_DM_RF_6_UART_RF_SHFT                                                                          0

#define HWIO_MSS_UIM0_UART_DM_RF_7_ADDR                                                                         (MSS_UIM0_UART_DM_REG_BASE            + 0x158)
#define HWIO_MSS_UIM0_UART_DM_RF_7_RMSK                                                                         0xffffffff
#define HWIO_MSS_UIM0_UART_DM_RF_7_IN                    \
                in_dword(HWIO_MSS_UIM0_UART_DM_RF_7_ADDR)
#define HWIO_MSS_UIM0_UART_DM_RF_7_INM(m)            \
                in_dword_masked(HWIO_MSS_UIM0_UART_DM_RF_7_ADDR, m)
#define HWIO_MSS_UIM0_UART_DM_RF_7_UART_RF_BMSK                                                                 0xffffffff
#define HWIO_MSS_UIM0_UART_DM_RF_7_UART_RF_SHFT                                                                          0

#define HWIO_MSS_UIM0_UART_DM_RF_8_ADDR                                                                         (MSS_UIM0_UART_DM_REG_BASE            + 0x15c)
#define HWIO_MSS_UIM0_UART_DM_RF_8_RMSK                                                                         0xffffffff
#define HWIO_MSS_UIM0_UART_DM_RF_8_IN                    \
                in_dword(HWIO_MSS_UIM0_UART_DM_RF_8_ADDR)
#define HWIO_MSS_UIM0_UART_DM_RF_8_INM(m)            \
                in_dword_masked(HWIO_MSS_UIM0_UART_DM_RF_8_ADDR, m)
#define HWIO_MSS_UIM0_UART_DM_RF_8_UART_RF_BMSK                                                                 0xffffffff
#define HWIO_MSS_UIM0_UART_DM_RF_8_UART_RF_SHFT                                                                          0

#define HWIO_MSS_UIM0_UART_DM_RF_9_ADDR                                                                         (MSS_UIM0_UART_DM_REG_BASE            + 0x160)
#define HWIO_MSS_UIM0_UART_DM_RF_9_RMSK                                                                         0xffffffff
#define HWIO_MSS_UIM0_UART_DM_RF_9_IN                    \
                in_dword(HWIO_MSS_UIM0_UART_DM_RF_9_ADDR)
#define HWIO_MSS_UIM0_UART_DM_RF_9_INM(m)            \
                in_dword_masked(HWIO_MSS_UIM0_UART_DM_RF_9_ADDR, m)
#define HWIO_MSS_UIM0_UART_DM_RF_9_UART_RF_BMSK                                                                 0xffffffff
#define HWIO_MSS_UIM0_UART_DM_RF_9_UART_RF_SHFT                                                                          0

#define HWIO_MSS_UIM0_UART_DM_RF_10_ADDR                                                                        (MSS_UIM0_UART_DM_REG_BASE            + 0x164)
#define HWIO_MSS_UIM0_UART_DM_RF_10_RMSK                                                                        0xffffffff
#define HWIO_MSS_UIM0_UART_DM_RF_10_IN                    \
                in_dword(HWIO_MSS_UIM0_UART_DM_RF_10_ADDR)
#define HWIO_MSS_UIM0_UART_DM_RF_10_INM(m)            \
                in_dword_masked(HWIO_MSS_UIM0_UART_DM_RF_10_ADDR, m)
#define HWIO_MSS_UIM0_UART_DM_RF_10_UART_RF_BMSK                                                                0xffffffff
#define HWIO_MSS_UIM0_UART_DM_RF_10_UART_RF_SHFT                                                                         0

#define HWIO_MSS_UIM0_UART_DM_RF_11_ADDR                                                                        (MSS_UIM0_UART_DM_REG_BASE            + 0x168)
#define HWIO_MSS_UIM0_UART_DM_RF_11_RMSK                                                                        0xffffffff
#define HWIO_MSS_UIM0_UART_DM_RF_11_IN                    \
                in_dword(HWIO_MSS_UIM0_UART_DM_RF_11_ADDR)
#define HWIO_MSS_UIM0_UART_DM_RF_11_INM(m)            \
                in_dword_masked(HWIO_MSS_UIM0_UART_DM_RF_11_ADDR, m)
#define HWIO_MSS_UIM0_UART_DM_RF_11_UART_RF_BMSK                                                                0xffffffff
#define HWIO_MSS_UIM0_UART_DM_RF_11_UART_RF_SHFT                                                                         0

#define HWIO_MSS_UIM0_UART_DM_RF_12_ADDR                                                                        (MSS_UIM0_UART_DM_REG_BASE            + 0x16c)
#define HWIO_MSS_UIM0_UART_DM_RF_12_RMSK                                                                        0xffffffff
#define HWIO_MSS_UIM0_UART_DM_RF_12_IN                    \
                in_dword(HWIO_MSS_UIM0_UART_DM_RF_12_ADDR)
#define HWIO_MSS_UIM0_UART_DM_RF_12_INM(m)            \
                in_dword_masked(HWIO_MSS_UIM0_UART_DM_RF_12_ADDR, m)
#define HWIO_MSS_UIM0_UART_DM_RF_12_UART_RF_BMSK                                                                0xffffffff
#define HWIO_MSS_UIM0_UART_DM_RF_12_UART_RF_SHFT                                                                         0

#define HWIO_MSS_UIM0_UART_DM_RF_13_ADDR                                                                        (MSS_UIM0_UART_DM_REG_BASE            + 0x170)
#define HWIO_MSS_UIM0_UART_DM_RF_13_RMSK                                                                        0xffffffff
#define HWIO_MSS_UIM0_UART_DM_RF_13_IN                    \
                in_dword(HWIO_MSS_UIM0_UART_DM_RF_13_ADDR)
#define HWIO_MSS_UIM0_UART_DM_RF_13_INM(m)            \
                in_dword_masked(HWIO_MSS_UIM0_UART_DM_RF_13_ADDR, m)
#define HWIO_MSS_UIM0_UART_DM_RF_13_UART_RF_BMSK                                                                0xffffffff
#define HWIO_MSS_UIM0_UART_DM_RF_13_UART_RF_SHFT                                                                         0

#define HWIO_MSS_UIM0_UART_DM_RF_14_ADDR                                                                        (MSS_UIM0_UART_DM_REG_BASE            + 0x174)
#define HWIO_MSS_UIM0_UART_DM_RF_14_RMSK                                                                        0xffffffff
#define HWIO_MSS_UIM0_UART_DM_RF_14_IN                    \
                in_dword(HWIO_MSS_UIM0_UART_DM_RF_14_ADDR)
#define HWIO_MSS_UIM0_UART_DM_RF_14_INM(m)            \
                in_dword_masked(HWIO_MSS_UIM0_UART_DM_RF_14_ADDR, m)
#define HWIO_MSS_UIM0_UART_DM_RF_14_UART_RF_BMSK                                                                0xffffffff
#define HWIO_MSS_UIM0_UART_DM_RF_14_UART_RF_SHFT                                                                         0

#define HWIO_MSS_UIM0_UART_DM_RF_15_ADDR                                                                        (MSS_UIM0_UART_DM_REG_BASE            + 0x178)
#define HWIO_MSS_UIM0_UART_DM_RF_15_RMSK                                                                        0xffffffff
#define HWIO_MSS_UIM0_UART_DM_RF_15_IN                    \
                in_dword(HWIO_MSS_UIM0_UART_DM_RF_15_ADDR)
#define HWIO_MSS_UIM0_UART_DM_RF_15_INM(m)            \
                in_dword_masked(HWIO_MSS_UIM0_UART_DM_RF_15_ADDR, m)
#define HWIO_MSS_UIM0_UART_DM_RF_15_UART_RF_BMSK                                                                0xffffffff
#define HWIO_MSS_UIM0_UART_DM_RF_15_UART_RF_SHFT                                                                         0

#define HWIO_MSS_UIM0_UART_DM_RF_16_ADDR                                                                        (MSS_UIM0_UART_DM_REG_BASE            + 0x17c)
#define HWIO_MSS_UIM0_UART_DM_RF_16_RMSK                                                                        0xffffffff
#define HWIO_MSS_UIM0_UART_DM_RF_16_IN                    \
                in_dword(HWIO_MSS_UIM0_UART_DM_RF_16_ADDR)
#define HWIO_MSS_UIM0_UART_DM_RF_16_INM(m)            \
                in_dword_masked(HWIO_MSS_UIM0_UART_DM_RF_16_ADDR, m)
#define HWIO_MSS_UIM0_UART_DM_RF_16_UART_RF_BMSK                                                                0xffffffff
#define HWIO_MSS_UIM0_UART_DM_RF_16_UART_RF_SHFT                                                                         0

#define HWIO_MSS_UIM0_UART_DM_UIM_CFG_ADDR                                                                      (MSS_UIM0_UART_DM_REG_BASE            + 0x180)
#define HWIO_MSS_UIM0_UART_DM_UIM_CFG_RMSK                                                                          0xf77f
#define HWIO_MSS_UIM0_UART_DM_UIM_CFG_IN                    \
                in_dword(HWIO_MSS_UIM0_UART_DM_UIM_CFG_ADDR)
#define HWIO_MSS_UIM0_UART_DM_UIM_CFG_INM(m)            \
                in_dword_masked(HWIO_MSS_UIM0_UART_DM_UIM_CFG_ADDR, m)
#define HWIO_MSS_UIM0_UART_DM_UIM_CFG_OUT(v)            \
                out_dword(HWIO_MSS_UIM0_UART_DM_UIM_CFG_ADDR,v)
#define HWIO_MSS_UIM0_UART_DM_UIM_CFG_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_UIM0_UART_DM_UIM_CFG_ADDR,m,v,HWIO_MSS_UIM0_UART_DM_UIM_CFG_IN)
#define HWIO_MSS_UIM0_UART_DM_UIM_CFG_BATT_ALARM_QUICK_DROP_EN_BMSK                                                 0x8000
#define HWIO_MSS_UIM0_UART_DM_UIM_CFG_BATT_ALARM_QUICK_DROP_EN_SHFT                                                     15
#define HWIO_MSS_UIM0_UART_DM_UIM_CFG_TESTBUS_EN_BMSK                                                               0x4000
#define HWIO_MSS_UIM0_UART_DM_UIM_CFG_TESTBUS_EN_SHFT                                                                   14
#define HWIO_MSS_UIM0_UART_DM_UIM_CFG_SW_RESET_BMSK                                                                 0x2000
#define HWIO_MSS_UIM0_UART_DM_UIM_CFG_SW_RESET_SHFT                                                                     13
#define HWIO_MSS_UIM0_UART_DM_UIM_CFG_MODE18_BMSK                                                                   0x1000
#define HWIO_MSS_UIM0_UART_DM_UIM_CFG_MODE18_SHFT                                                                       12
#define HWIO_MSS_UIM0_UART_DM_UIM_CFG_PMIC_ALARM_EN_BMSK                                                             0x400
#define HWIO_MSS_UIM0_UART_DM_UIM_CFG_PMIC_ALARM_EN_SHFT                                                                10
#define HWIO_MSS_UIM0_UART_DM_UIM_CFG_BATT_ALARM_TRIGGER_EN_BMSK                                                     0x200
#define HWIO_MSS_UIM0_UART_DM_UIM_CFG_BATT_ALARM_TRIGGER_EN_SHFT                                                         9
#define HWIO_MSS_UIM0_UART_DM_UIM_CFG_UIM_RMV_TRIGGER_EN_BMSK                                                        0x100
#define HWIO_MSS_UIM0_UART_DM_UIM_CFG_UIM_RMV_TRIGGER_EN_SHFT                                                            8
#define HWIO_MSS_UIM0_UART_DM_UIM_CFG_UIM_CARD_EVENTS_ENABLE_BMSK                                                     0x40
#define HWIO_MSS_UIM0_UART_DM_UIM_CFG_UIM_CARD_EVENTS_ENABLE_SHFT                                                        6
#define HWIO_MSS_UIM0_UART_DM_UIM_CFG_UIM_PRESENT_POLARITY_BMSK                                                       0x20
#define HWIO_MSS_UIM0_UART_DM_UIM_CFG_UIM_PRESENT_POLARITY_SHFT                                                          5
#define HWIO_MSS_UIM0_UART_DM_UIM_CFG_EVENT_DEBOUNCE_TIME_BMSK                                                        0x1f
#define HWIO_MSS_UIM0_UART_DM_UIM_CFG_EVENT_DEBOUNCE_TIME_SHFT                                                           0

#define HWIO_MSS_UIM0_UART_DM_UIM_CMD_ADDR                                                                      (MSS_UIM0_UART_DM_REG_BASE            + 0x184)
#define HWIO_MSS_UIM0_UART_DM_UIM_CMD_RMSK                                                                             0x3
#define HWIO_MSS_UIM0_UART_DM_UIM_CMD_OUT(v)            \
                out_dword(HWIO_MSS_UIM0_UART_DM_UIM_CMD_ADDR,v)
#define HWIO_MSS_UIM0_UART_DM_UIM_CMD_RECOVER_FROM_HW_DEACTIVATION_BMSK                                                0x2
#define HWIO_MSS_UIM0_UART_DM_UIM_CMD_RECOVER_FROM_HW_DEACTIVATION_SHFT                                                  1
#define HWIO_MSS_UIM0_UART_DM_UIM_CMD_INITIATE_HW_DEACTIVATION_BMSK                                                    0x1
#define HWIO_MSS_UIM0_UART_DM_UIM_CMD_INITIATE_HW_DEACTIVATION_SHFT                                                      0

#define HWIO_MSS_UIM0_UART_DM_UIM_IO_STATUS_ADDR                                                                (MSS_UIM0_UART_DM_REG_BASE            + 0x188)
#define HWIO_MSS_UIM0_UART_DM_UIM_IO_STATUS_RMSK                                                                       0x7
#define HWIO_MSS_UIM0_UART_DM_UIM_IO_STATUS_IN                    \
                in_dword(HWIO_MSS_UIM0_UART_DM_UIM_IO_STATUS_ADDR)
#define HWIO_MSS_UIM0_UART_DM_UIM_IO_STATUS_INM(m)            \
                in_dword_masked(HWIO_MSS_UIM0_UART_DM_UIM_IO_STATUS_ADDR, m)
#define HWIO_MSS_UIM0_UART_DM_UIM_IO_STATUS_UIM_IO_WRITE_IN_PROGRESS_BMSK                                              0x4
#define HWIO_MSS_UIM0_UART_DM_UIM_IO_STATUS_UIM_IO_WRITE_IN_PROGRESS_SHFT                                                2
#define HWIO_MSS_UIM0_UART_DM_UIM_IO_STATUS_UIM_DEACTIVATION_STATUS_BMSK                                               0x2
#define HWIO_MSS_UIM0_UART_DM_UIM_IO_STATUS_UIM_DEACTIVATION_STATUS_SHFT                                                 1
#define HWIO_MSS_UIM0_UART_DM_UIM_IO_STATUS_CARD_PRESENCE_BMSK                                                         0x1
#define HWIO_MSS_UIM0_UART_DM_UIM_IO_STATUS_CARD_PRESENCE_SHFT                                                           0

#define HWIO_MSS_UIM0_UART_DM_UIM_IRQ_ISR_ADDR                                                                  (MSS_UIM0_UART_DM_REG_BASE            + 0x18c)
#define HWIO_MSS_UIM0_UART_DM_UIM_IRQ_ISR_RMSK                                                                        0x1f
#define HWIO_MSS_UIM0_UART_DM_UIM_IRQ_ISR_IN                    \
                in_dword(HWIO_MSS_UIM0_UART_DM_UIM_IRQ_ISR_ADDR)
#define HWIO_MSS_UIM0_UART_DM_UIM_IRQ_ISR_INM(m)            \
                in_dword_masked(HWIO_MSS_UIM0_UART_DM_UIM_IRQ_ISR_ADDR, m)
#define HWIO_MSS_UIM0_UART_DM_UIM_IRQ_ISR_UIM_IO_WRITE_DONE_BMSK                                                      0x10
#define HWIO_MSS_UIM0_UART_DM_UIM_IRQ_ISR_UIM_IO_WRITE_DONE_SHFT                                                         4
#define HWIO_MSS_UIM0_UART_DM_UIM_IRQ_ISR_HW_SEQUENCE_FINISH_BMSK                                                      0x8
#define HWIO_MSS_UIM0_UART_DM_UIM_IRQ_ISR_HW_SEQUENCE_FINISH_SHFT                                                        3
#define HWIO_MSS_UIM0_UART_DM_UIM_IRQ_ISR_BATT_ALARM_BMSK                                                              0x4
#define HWIO_MSS_UIM0_UART_DM_UIM_IRQ_ISR_BATT_ALARM_SHFT                                                                2
#define HWIO_MSS_UIM0_UART_DM_UIM_IRQ_ISR_UIM_CARD_INSERTION_BMSK                                                      0x2
#define HWIO_MSS_UIM0_UART_DM_UIM_IRQ_ISR_UIM_CARD_INSERTION_SHFT                                                        1
#define HWIO_MSS_UIM0_UART_DM_UIM_IRQ_ISR_UIM_CARD_REMOVAL_BMSK                                                        0x1
#define HWIO_MSS_UIM0_UART_DM_UIM_IRQ_ISR_UIM_CARD_REMOVAL_SHFT                                                          0

#define HWIO_MSS_UIM0_UART_DM_UIM_IRQ_MISR_ADDR                                                                 (MSS_UIM0_UART_DM_REG_BASE            + 0x190)
#define HWIO_MSS_UIM0_UART_DM_UIM_IRQ_MISR_RMSK                                                                       0x1f
#define HWIO_MSS_UIM0_UART_DM_UIM_IRQ_MISR_IN                    \
                in_dword(HWIO_MSS_UIM0_UART_DM_UIM_IRQ_MISR_ADDR)
#define HWIO_MSS_UIM0_UART_DM_UIM_IRQ_MISR_INM(m)            \
                in_dword_masked(HWIO_MSS_UIM0_UART_DM_UIM_IRQ_MISR_ADDR, m)
#define HWIO_MSS_UIM0_UART_DM_UIM_IRQ_MISR_UIM_IO_WRITE_DONE_BMSK                                                     0x10
#define HWIO_MSS_UIM0_UART_DM_UIM_IRQ_MISR_UIM_IO_WRITE_DONE_SHFT                                                        4
#define HWIO_MSS_UIM0_UART_DM_UIM_IRQ_MISR_HW_SEQUENCE_FINISH_BMSK                                                     0x8
#define HWIO_MSS_UIM0_UART_DM_UIM_IRQ_MISR_HW_SEQUENCE_FINISH_SHFT                                                       3
#define HWIO_MSS_UIM0_UART_DM_UIM_IRQ_MISR_BATT_ALARM_BMSK                                                             0x4
#define HWIO_MSS_UIM0_UART_DM_UIM_IRQ_MISR_BATT_ALARM_SHFT                                                               2
#define HWIO_MSS_UIM0_UART_DM_UIM_IRQ_MISR_UIM_CARD_INSERTION_BMSK                                                     0x2
#define HWIO_MSS_UIM0_UART_DM_UIM_IRQ_MISR_UIM_CARD_INSERTION_SHFT                                                       1
#define HWIO_MSS_UIM0_UART_DM_UIM_IRQ_MISR_UIM_CARD_REMOVAL_BMSK                                                       0x1
#define HWIO_MSS_UIM0_UART_DM_UIM_IRQ_MISR_UIM_CARD_REMOVAL_SHFT                                                         0

#define HWIO_MSS_UIM0_UART_DM_UIM_IRQ_CLR_ADDR                                                                  (MSS_UIM0_UART_DM_REG_BASE            + 0x194)
#define HWIO_MSS_UIM0_UART_DM_UIM_IRQ_CLR_RMSK                                                                        0x1f
#define HWIO_MSS_UIM0_UART_DM_UIM_IRQ_CLR_OUT(v)            \
                out_dword(HWIO_MSS_UIM0_UART_DM_UIM_IRQ_CLR_ADDR,v)
#define HWIO_MSS_UIM0_UART_DM_UIM_IRQ_CLR_UIM_IO_WRITE_DONE_BMSK                                                      0x10
#define HWIO_MSS_UIM0_UART_DM_UIM_IRQ_CLR_UIM_IO_WRITE_DONE_SHFT                                                         4
#define HWIO_MSS_UIM0_UART_DM_UIM_IRQ_CLR_HW_SEQUENCE_FINISH_BMSK                                                      0x8
#define HWIO_MSS_UIM0_UART_DM_UIM_IRQ_CLR_HW_SEQUENCE_FINISH_SHFT                                                        3
#define HWIO_MSS_UIM0_UART_DM_UIM_IRQ_CLR_BATT_ALARM_BMSK                                                              0x4
#define HWIO_MSS_UIM0_UART_DM_UIM_IRQ_CLR_BATT_ALARM_SHFT                                                                2
#define HWIO_MSS_UIM0_UART_DM_UIM_IRQ_CLR_UIM_CARD_INSERTION_BMSK                                                      0x2
#define HWIO_MSS_UIM0_UART_DM_UIM_IRQ_CLR_UIM_CARD_INSERTION_SHFT                                                        1
#define HWIO_MSS_UIM0_UART_DM_UIM_IRQ_CLR_UIM_CARD_REMOVAL_BMSK                                                        0x1
#define HWIO_MSS_UIM0_UART_DM_UIM_IRQ_CLR_UIM_CARD_REMOVAL_SHFT                                                          0

#define HWIO_MSS_UIM0_UART_DM_UIM_IRQ_IMR_ADDR                                                                  (MSS_UIM0_UART_DM_REG_BASE            + 0x198)
#define HWIO_MSS_UIM0_UART_DM_UIM_IRQ_IMR_RMSK                                                                        0x1f
#define HWIO_MSS_UIM0_UART_DM_UIM_IRQ_IMR_IN                    \
                in_dword(HWIO_MSS_UIM0_UART_DM_UIM_IRQ_IMR_ADDR)
#define HWIO_MSS_UIM0_UART_DM_UIM_IRQ_IMR_INM(m)            \
                in_dword_masked(HWIO_MSS_UIM0_UART_DM_UIM_IRQ_IMR_ADDR, m)
#define HWIO_MSS_UIM0_UART_DM_UIM_IRQ_IMR_OUT(v)            \
                out_dword(HWIO_MSS_UIM0_UART_DM_UIM_IRQ_IMR_ADDR,v)
#define HWIO_MSS_UIM0_UART_DM_UIM_IRQ_IMR_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_UIM0_UART_DM_UIM_IRQ_IMR_ADDR,m,v,HWIO_MSS_UIM0_UART_DM_UIM_IRQ_IMR_IN)
#define HWIO_MSS_UIM0_UART_DM_UIM_IRQ_IMR_UIM_IO_WRITE_DONE_BMSK                                                      0x10
#define HWIO_MSS_UIM0_UART_DM_UIM_IRQ_IMR_UIM_IO_WRITE_DONE_SHFT                                                         4
#define HWIO_MSS_UIM0_UART_DM_UIM_IRQ_IMR_HW_SEQUENCE_FINISH_BMSK                                                      0x8
#define HWIO_MSS_UIM0_UART_DM_UIM_IRQ_IMR_HW_SEQUENCE_FINISH_SHFT                                                        3
#define HWIO_MSS_UIM0_UART_DM_UIM_IRQ_IMR_BATT_ALARM_BMSK                                                              0x4
#define HWIO_MSS_UIM0_UART_DM_UIM_IRQ_IMR_BATT_ALARM_SHFT                                                                2
#define HWIO_MSS_UIM0_UART_DM_UIM_IRQ_IMR_UIM_CARD_INSERTION_BMSK                                                      0x2
#define HWIO_MSS_UIM0_UART_DM_UIM_IRQ_IMR_UIM_CARD_INSERTION_SHFT                                                        1
#define HWIO_MSS_UIM0_UART_DM_UIM_IRQ_IMR_UIM_CARD_REMOVAL_BMSK                                                        0x1
#define HWIO_MSS_UIM0_UART_DM_UIM_IRQ_IMR_UIM_CARD_REMOVAL_SHFT                                                          0

#define HWIO_MSS_UIM0_UART_DM_UIM_IRQ_IMR_SET_ADDR                                                              (MSS_UIM0_UART_DM_REG_BASE            + 0x19c)
#define HWIO_MSS_UIM0_UART_DM_UIM_IRQ_IMR_SET_RMSK                                                                    0x1f
#define HWIO_MSS_UIM0_UART_DM_UIM_IRQ_IMR_SET_OUT(v)            \
                out_dword(HWIO_MSS_UIM0_UART_DM_UIM_IRQ_IMR_SET_ADDR,v)
#define HWIO_MSS_UIM0_UART_DM_UIM_IRQ_IMR_SET_UIM_IO_WRITE_DONE_BMSK                                                  0x10
#define HWIO_MSS_UIM0_UART_DM_UIM_IRQ_IMR_SET_UIM_IO_WRITE_DONE_SHFT                                                     4
#define HWIO_MSS_UIM0_UART_DM_UIM_IRQ_IMR_SET_HW_SEQUENCE_FINISH_BMSK                                                  0x8
#define HWIO_MSS_UIM0_UART_DM_UIM_IRQ_IMR_SET_HW_SEQUENCE_FINISH_SHFT                                                    3
#define HWIO_MSS_UIM0_UART_DM_UIM_IRQ_IMR_SET_BATT_ALARM_BMSK                                                          0x4
#define HWIO_MSS_UIM0_UART_DM_UIM_IRQ_IMR_SET_BATT_ALARM_SHFT                                                            2
#define HWIO_MSS_UIM0_UART_DM_UIM_IRQ_IMR_SET_UIM_CARD_INSERTION_BMSK                                                  0x2
#define HWIO_MSS_UIM0_UART_DM_UIM_IRQ_IMR_SET_UIM_CARD_INSERTION_SHFT                                                    1
#define HWIO_MSS_UIM0_UART_DM_UIM_IRQ_IMR_SET_UIM_CARD_REMOVAL_BMSK                                                    0x1
#define HWIO_MSS_UIM0_UART_DM_UIM_IRQ_IMR_SET_UIM_CARD_REMOVAL_SHFT                                                      0

#define HWIO_MSS_UIM0_UART_DM_UIM_IRQ_IMR_CLR_ADDR                                                              (MSS_UIM0_UART_DM_REG_BASE            + 0x1a0)
#define HWIO_MSS_UIM0_UART_DM_UIM_IRQ_IMR_CLR_RMSK                                                                    0x1f
#define HWIO_MSS_UIM0_UART_DM_UIM_IRQ_IMR_CLR_OUT(v)            \
                out_dword(HWIO_MSS_UIM0_UART_DM_UIM_IRQ_IMR_CLR_ADDR,v)
#define HWIO_MSS_UIM0_UART_DM_UIM_IRQ_IMR_CLR_UIM_IO_WRITE_DONE_BMSK                                                  0x10
#define HWIO_MSS_UIM0_UART_DM_UIM_IRQ_IMR_CLR_UIM_IO_WRITE_DONE_SHFT                                                     4
#define HWIO_MSS_UIM0_UART_DM_UIM_IRQ_IMR_CLR_HW_SEQUENCE_FINISH_BMSK                                                  0x8
#define HWIO_MSS_UIM0_UART_DM_UIM_IRQ_IMR_CLR_HW_SEQUENCE_FINISH_SHFT                                                    3
#define HWIO_MSS_UIM0_UART_DM_UIM_IRQ_IMR_CLR_BATT_ALARM_BMSK                                                          0x4
#define HWIO_MSS_UIM0_UART_DM_UIM_IRQ_IMR_CLR_BATT_ALARM_SHFT                                                            2
#define HWIO_MSS_UIM0_UART_DM_UIM_IRQ_IMR_CLR_UIM_CARD_INSERTION_BMSK                                                  0x2
#define HWIO_MSS_UIM0_UART_DM_UIM_IRQ_IMR_CLR_UIM_CARD_INSERTION_SHFT                                                    1
#define HWIO_MSS_UIM0_UART_DM_UIM_IRQ_IMR_CLR_UIM_CARD_REMOVAL_BMSK                                                    0x1
#define HWIO_MSS_UIM0_UART_DM_UIM_IRQ_IMR_CLR_UIM_CARD_REMOVAL_SHFT                                                      0

/*----------------------------------------------------------------------------
 * MODULE: MSS_PERPH_MSS_PERPH
 *--------------------------------------------------------------------------*/

#define MSS_PERPH_MSS_PERPH_REG_BASE                                                  (MSS_TOP_BASE            + 0x00181770)
#define MSS_PERPH_MSS_PERPH_REG_BASE_SIZE                                             0xd8b0
#define MSS_PERPH_MSS_PERPH_REG_BASE_USED                                             0xd890

#define HWIO_MSS_PERPH_MSS_ENABLE_ADDR                                                (MSS_PERPH_MSS_PERPH_REG_BASE            + 0x0)
#define HWIO_MSS_PERPH_MSS_ENABLE_RMSK                                                0x80000000
#define HWIO_MSS_PERPH_MSS_ENABLE_IN                    \
                in_dword(HWIO_MSS_PERPH_MSS_ENABLE_ADDR)
#define HWIO_MSS_PERPH_MSS_ENABLE_INM(m)            \
                in_dword_masked(HWIO_MSS_PERPH_MSS_ENABLE_ADDR, m)
#define HWIO_MSS_PERPH_MSS_ENABLE_MODEM_ARES_IN_BMSK                                  0x80000000
#define HWIO_MSS_PERPH_MSS_ENABLE_MODEM_ARES_IN_SHFT                                          31

#define HWIO_MSS_PERPH_MSS_CLAMP_MEM_ADDR                                             (MSS_PERPH_MSS_PERPH_REG_BASE            + 0x4)
#define HWIO_MSS_PERPH_MSS_CLAMP_MEM_RMSK                                                    0x7
#define HWIO_MSS_PERPH_MSS_CLAMP_MEM_IN                    \
                in_dword(HWIO_MSS_PERPH_MSS_CLAMP_MEM_ADDR)
#define HWIO_MSS_PERPH_MSS_CLAMP_MEM_INM(m)            \
                in_dword_masked(HWIO_MSS_PERPH_MSS_CLAMP_MEM_ADDR, m)
#define HWIO_MSS_PERPH_MSS_CLAMP_MEM_OUT(v)            \
                out_dword(HWIO_MSS_PERPH_MSS_CLAMP_MEM_ADDR,v)
#define HWIO_MSS_PERPH_MSS_CLAMP_MEM_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_PERPH_MSS_CLAMP_MEM_ADDR,m,v,HWIO_MSS_PERPH_MSS_CLAMP_MEM_IN)
#define HWIO_MSS_PERPH_MSS_CLAMP_MEM_SPARE_BMSK                                              0x4
#define HWIO_MSS_PERPH_MSS_CLAMP_MEM_SPARE_SHFT                                                2
#define HWIO_MSS_PERPH_MSS_CLAMP_MEM_UNCLAMP_ALL_BMSK                                        0x2
#define HWIO_MSS_PERPH_MSS_CLAMP_MEM_UNCLAMP_ALL_SHFT                                          1
#define HWIO_MSS_PERPH_MSS_CLAMP_MEM_HM_CLAMP_BMSK                                           0x1
#define HWIO_MSS_PERPH_MSS_CLAMP_MEM_HM_CLAMP_SHFT                                             0

#define HWIO_MSS_PERPH_MSS_CLAMP_IO_ADDR                                              (MSS_PERPH_MSS_PERPH_REG_BASE            + 0x8)
#define HWIO_MSS_PERPH_MSS_CLAMP_IO_RMSK                                                    0xd0
#define HWIO_MSS_PERPH_MSS_CLAMP_IO_IN                    \
                in_dword(HWIO_MSS_PERPH_MSS_CLAMP_IO_ADDR)
#define HWIO_MSS_PERPH_MSS_CLAMP_IO_INM(m)            \
                in_dword_masked(HWIO_MSS_PERPH_MSS_CLAMP_IO_ADDR, m)
#define HWIO_MSS_PERPH_MSS_CLAMP_IO_OUT(v)            \
                out_dword(HWIO_MSS_PERPH_MSS_CLAMP_IO_ADDR,v)
#define HWIO_MSS_PERPH_MSS_CLAMP_IO_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_PERPH_MSS_CLAMP_IO_ADDR,m,v,HWIO_MSS_PERPH_MSS_CLAMP_IO_IN)
#define HWIO_MSS_PERPH_MSS_CLAMP_IO_SPARE_7_BMSK                                            0x80
#define HWIO_MSS_PERPH_MSS_CLAMP_IO_SPARE_7_SHFT                                               7
#define HWIO_MSS_PERPH_MSS_CLAMP_IO_UNCLAMP_ALL_BMSK                                        0x40
#define HWIO_MSS_PERPH_MSS_CLAMP_IO_UNCLAMP_ALL_SHFT                                           6
#define HWIO_MSS_PERPH_MSS_CLAMP_IO_BBRX_ADC_BMSK                                           0x10
#define HWIO_MSS_PERPH_MSS_CLAMP_IO_BBRX_ADC_SHFT                                              4

#define HWIO_MSS_PERPH_MSS_BUS_AHB2AHB_CFG_ADDR                                       (MSS_PERPH_MSS_PERPH_REG_BASE            + 0xc)
#define HWIO_MSS_PERPH_MSS_BUS_AHB2AHB_CFG_RMSK                                              0x1
#define HWIO_MSS_PERPH_MSS_BUS_AHB2AHB_CFG_IN                    \
                in_dword(HWIO_MSS_PERPH_MSS_BUS_AHB2AHB_CFG_ADDR)
#define HWIO_MSS_PERPH_MSS_BUS_AHB2AHB_CFG_INM(m)            \
                in_dword_masked(HWIO_MSS_PERPH_MSS_BUS_AHB2AHB_CFG_ADDR, m)
#define HWIO_MSS_PERPH_MSS_BUS_AHB2AHB_CFG_OUT(v)            \
                out_dword(HWIO_MSS_PERPH_MSS_BUS_AHB2AHB_CFG_ADDR,v)
#define HWIO_MSS_PERPH_MSS_BUS_AHB2AHB_CFG_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_PERPH_MSS_BUS_AHB2AHB_CFG_ADDR,m,v,HWIO_MSS_PERPH_MSS_BUS_AHB2AHB_CFG_IN)
#define HWIO_MSS_PERPH_MSS_BUS_AHB2AHB_CFG_POST_EN_AHB2AHB_BMSK                              0x1
#define HWIO_MSS_PERPH_MSS_BUS_AHB2AHB_CFG_POST_EN_AHB2AHB_SHFT                                0

#define HWIO_MSS_PERPH_MSS_CUSTOM_MEM_ARRSTBYN_ADDR                                   (MSS_PERPH_MSS_PERPH_REG_BASE            + 0x14)
#define HWIO_MSS_PERPH_MSS_CUSTOM_MEM_ARRSTBYN_RMSK                                         0xff
#define HWIO_MSS_PERPH_MSS_CUSTOM_MEM_ARRSTBYN_IN                    \
                in_dword(HWIO_MSS_PERPH_MSS_CUSTOM_MEM_ARRSTBYN_ADDR)
#define HWIO_MSS_PERPH_MSS_CUSTOM_MEM_ARRSTBYN_INM(m)            \
                in_dword_masked(HWIO_MSS_PERPH_MSS_CUSTOM_MEM_ARRSTBYN_ADDR, m)
#define HWIO_MSS_PERPH_MSS_CUSTOM_MEM_ARRSTBYN_OUT(v)            \
                out_dword(HWIO_MSS_PERPH_MSS_CUSTOM_MEM_ARRSTBYN_ADDR,v)
#define HWIO_MSS_PERPH_MSS_CUSTOM_MEM_ARRSTBYN_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_PERPH_MSS_CUSTOM_MEM_ARRSTBYN_ADDR,m,v,HWIO_MSS_PERPH_MSS_CUSTOM_MEM_ARRSTBYN_IN)
#define HWIO_MSS_PERPH_MSS_CUSTOM_MEM_ARRSTBYN_CTL_BMSK                                     0xff
#define HWIO_MSS_PERPH_MSS_CUSTOM_MEM_ARRSTBYN_CTL_SHFT                                        0

#define HWIO_MSS_PERPH_MSS_ANALOG_IP_TEST_CTL_ADDR                                    (MSS_PERPH_MSS_PERPH_REG_BASE            + 0x18)
#define HWIO_MSS_PERPH_MSS_ANALOG_IP_TEST_CTL_RMSK                                           0x3
#define HWIO_MSS_PERPH_MSS_ANALOG_IP_TEST_CTL_IN                    \
                in_dword(HWIO_MSS_PERPH_MSS_ANALOG_IP_TEST_CTL_ADDR)
#define HWIO_MSS_PERPH_MSS_ANALOG_IP_TEST_CTL_INM(m)            \
                in_dword_masked(HWIO_MSS_PERPH_MSS_ANALOG_IP_TEST_CTL_ADDR, m)
#define HWIO_MSS_PERPH_MSS_ANALOG_IP_TEST_CTL_OUT(v)            \
                out_dword(HWIO_MSS_PERPH_MSS_ANALOG_IP_TEST_CTL_ADDR,v)
#define HWIO_MSS_PERPH_MSS_ANALOG_IP_TEST_CTL_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_PERPH_MSS_ANALOG_IP_TEST_CTL_ADDR,m,v,HWIO_MSS_PERPH_MSS_ANALOG_IP_TEST_CTL_IN)
#define HWIO_MSS_PERPH_MSS_ANALOG_IP_TEST_CTL_EXTERNAL_IQDATA_EN_BMSK                        0x2
#define HWIO_MSS_PERPH_MSS_ANALOG_IP_TEST_CTL_EXTERNAL_IQDATA_EN_SHFT                          1
#define HWIO_MSS_PERPH_MSS_ANALOG_IP_TEST_CTL_EXTERNAL_Y1Y2_EN_BMSK                          0x1
#define HWIO_MSS_PERPH_MSS_ANALOG_IP_TEST_CTL_EXTERNAL_Y1Y2_EN_SHFT                            0

#define HWIO_MSS_PERPH_MSS_ATB_ID_ADDR                                                (MSS_PERPH_MSS_PERPH_REG_BASE            + 0x1c)
#define HWIO_MSS_PERPH_MSS_ATB_ID_RMSK                                                    0x3fff
#define HWIO_MSS_PERPH_MSS_ATB_ID_IN                    \
                in_dword(HWIO_MSS_PERPH_MSS_ATB_ID_ADDR)
#define HWIO_MSS_PERPH_MSS_ATB_ID_INM(m)            \
                in_dword_masked(HWIO_MSS_PERPH_MSS_ATB_ID_ADDR, m)
#define HWIO_MSS_PERPH_MSS_ATB_ID_OUT(v)            \
                out_dword(HWIO_MSS_PERPH_MSS_ATB_ID_ADDR,v)
#define HWIO_MSS_PERPH_MSS_ATB_ID_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_PERPH_MSS_ATB_ID_ADDR,m,v,HWIO_MSS_PERPH_MSS_ATB_ID_IN)
#define HWIO_MSS_PERPH_MSS_ATB_ID_MODEM_RXFE_ATB_ID_BMSK                                  0x3f80
#define HWIO_MSS_PERPH_MSS_ATB_ID_MODEM_RXFE_ATB_ID_SHFT                                       7
#define HWIO_MSS_PERPH_MSS_ATB_ID_ATB_ID_BMSK                                               0x7f
#define HWIO_MSS_PERPH_MSS_ATB_ID_ATB_ID_SHFT                                                  0

#define HWIO_MSS_PERPH_MSS_DBG_BUS_CTL_ADDR                                           (MSS_PERPH_MSS_PERPH_REG_BASE            + 0x20)
#define HWIO_MSS_PERPH_MSS_DBG_BUS_CTL_RMSK                                                0x7e3
#define HWIO_MSS_PERPH_MSS_DBG_BUS_CTL_IN                    \
                in_dword(HWIO_MSS_PERPH_MSS_DBG_BUS_CTL_ADDR)
#define HWIO_MSS_PERPH_MSS_DBG_BUS_CTL_INM(m)            \
                in_dword_masked(HWIO_MSS_PERPH_MSS_DBG_BUS_CTL_ADDR, m)
#define HWIO_MSS_PERPH_MSS_DBG_BUS_CTL_OUT(v)            \
                out_dword(HWIO_MSS_PERPH_MSS_DBG_BUS_CTL_ADDR,v)
#define HWIO_MSS_PERPH_MSS_DBG_BUS_CTL_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_PERPH_MSS_DBG_BUS_CTL_ADDR,m,v,HWIO_MSS_PERPH_MSS_DBG_BUS_CTL_IN)
#define HWIO_MSS_PERPH_MSS_DBG_BUS_CTL_AHB2AHB_SEL_BMSK                                    0x600
#define HWIO_MSS_PERPH_MSS_DBG_BUS_CTL_AHB2AHB_SEL_SHFT                                        9
#define HWIO_MSS_PERPH_MSS_DBG_BUS_CTL_MSS_DBG_BUS_TOP_SEL_BMSK                            0x1e0
#define HWIO_MSS_PERPH_MSS_DBG_BUS_CTL_MSS_DBG_BUS_TOP_SEL_SHFT                                5
#define HWIO_MSS_PERPH_MSS_DBG_BUS_CTL_MSS_DBG_GPIO_ATB_SEL_BMSK                             0x3
#define HWIO_MSS_PERPH_MSS_DBG_BUS_CTL_MSS_DBG_GPIO_ATB_SEL_SHFT                               0

#define HWIO_MSS_PERPH_MSS_AHB_ACCESS_ERR_IRQ_EN_ADDR                                 (MSS_PERPH_MSS_PERPH_REG_BASE            + 0x24)
#define HWIO_MSS_PERPH_MSS_AHB_ACCESS_ERR_IRQ_EN_RMSK                                        0x1
#define HWIO_MSS_PERPH_MSS_AHB_ACCESS_ERR_IRQ_EN_IN                    \
                in_dword(HWIO_MSS_PERPH_MSS_AHB_ACCESS_ERR_IRQ_EN_ADDR)
#define HWIO_MSS_PERPH_MSS_AHB_ACCESS_ERR_IRQ_EN_INM(m)            \
                in_dword_masked(HWIO_MSS_PERPH_MSS_AHB_ACCESS_ERR_IRQ_EN_ADDR, m)
#define HWIO_MSS_PERPH_MSS_AHB_ACCESS_ERR_IRQ_EN_OUT(v)            \
                out_dword(HWIO_MSS_PERPH_MSS_AHB_ACCESS_ERR_IRQ_EN_ADDR,v)
#define HWIO_MSS_PERPH_MSS_AHB_ACCESS_ERR_IRQ_EN_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_PERPH_MSS_AHB_ACCESS_ERR_IRQ_EN_ADDR,m,v,HWIO_MSS_PERPH_MSS_AHB_ACCESS_ERR_IRQ_EN_IN)
#define HWIO_MSS_PERPH_MSS_AHB_ACCESS_ERR_IRQ_EN_EN_BMSK                                     0x1
#define HWIO_MSS_PERPH_MSS_AHB_ACCESS_ERR_IRQ_EN_EN_SHFT                                       0

#define HWIO_MSS_PERPH_MSS_AHB_ACCESS_ERR_IRQ_STATUS_ADDR                             (MSS_PERPH_MSS_PERPH_REG_BASE            + 0x28)
#define HWIO_MSS_PERPH_MSS_AHB_ACCESS_ERR_IRQ_STATUS_RMSK                                    0x1
#define HWIO_MSS_PERPH_MSS_AHB_ACCESS_ERR_IRQ_STATUS_IN                    \
                in_dword(HWIO_MSS_PERPH_MSS_AHB_ACCESS_ERR_IRQ_STATUS_ADDR)
#define HWIO_MSS_PERPH_MSS_AHB_ACCESS_ERR_IRQ_STATUS_INM(m)            \
                in_dword_masked(HWIO_MSS_PERPH_MSS_AHB_ACCESS_ERR_IRQ_STATUS_ADDR, m)
#define HWIO_MSS_PERPH_MSS_AHB_ACCESS_ERR_IRQ_STATUS_STATUS_BMSK                             0x1
#define HWIO_MSS_PERPH_MSS_AHB_ACCESS_ERR_IRQ_STATUS_STATUS_SHFT                               0

#define HWIO_MSS_PERPH_MSS_AHB_ACCESS_ERR_IRQ_CLR_ADDR                                (MSS_PERPH_MSS_PERPH_REG_BASE            + 0x2c)
#define HWIO_MSS_PERPH_MSS_AHB_ACCESS_ERR_IRQ_CLR_RMSK                                       0x1
#define HWIO_MSS_PERPH_MSS_AHB_ACCESS_ERR_IRQ_CLR_OUT(v)            \
                out_dword(HWIO_MSS_PERPH_MSS_AHB_ACCESS_ERR_IRQ_CLR_ADDR,v)
#define HWIO_MSS_PERPH_MSS_AHB_ACCESS_ERR_IRQ_CLR_CMD_BMSK                                   0x1
#define HWIO_MSS_PERPH_MSS_AHB_ACCESS_ERR_IRQ_CLR_CMD_SHFT                                     0

#define HWIO_MSS_PERPH_MSS_BUS_CTL_CFG_ADDR                                           (MSS_PERPH_MSS_PERPH_REG_BASE            + 0x30)
#define HWIO_MSS_PERPH_MSS_BUS_CTL_CFG_RMSK                                                  0x1
#define HWIO_MSS_PERPH_MSS_BUS_CTL_CFG_IN                    \
                in_dword(HWIO_MSS_PERPH_MSS_BUS_CTL_CFG_ADDR)
#define HWIO_MSS_PERPH_MSS_BUS_CTL_CFG_INM(m)            \
                in_dword_masked(HWIO_MSS_PERPH_MSS_BUS_CTL_CFG_ADDR, m)
#define HWIO_MSS_PERPH_MSS_BUS_CTL_CFG_OUT(v)            \
                out_dword(HWIO_MSS_PERPH_MSS_BUS_CTL_CFG_ADDR,v)
#define HWIO_MSS_PERPH_MSS_BUS_CTL_CFG_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_PERPH_MSS_BUS_CTL_CFG_ADDR,m,v,HWIO_MSS_PERPH_MSS_BUS_CTL_CFG_IN)
#define HWIO_MSS_PERPH_MSS_BUS_CTL_CFG_Q6_FORCE_UNBUFFERED_BMSK                              0x1
#define HWIO_MSS_PERPH_MSS_BUS_CTL_CFG_Q6_FORCE_UNBUFFERED_SHFT                                0

#define HWIO_MSS_PERPH_MSS_MSA_ADDR                                                   (MSS_PERPH_MSS_PERPH_REG_BASE            + 0x34)
#define HWIO_MSS_PERPH_MSS_MSA_RMSK                                                          0x7
#define HWIO_MSS_PERPH_MSS_MSA_IN                    \
                in_dword(HWIO_MSS_PERPH_MSS_MSA_ADDR)
#define HWIO_MSS_PERPH_MSS_MSA_INM(m)            \
                in_dword_masked(HWIO_MSS_PERPH_MSS_MSA_ADDR, m)
#define HWIO_MSS_PERPH_MSS_MSA_OUT(v)            \
                out_dword(HWIO_MSS_PERPH_MSS_MSA_ADDR,v)
#define HWIO_MSS_PERPH_MSS_MSA_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_PERPH_MSS_MSA_ADDR,m,v,HWIO_MSS_PERPH_MSS_MSA_IN)
#define HWIO_MSS_PERPH_MSS_MSA_FORCE_Q6_MSA_BMSK                                             0x4
#define HWIO_MSS_PERPH_MSS_MSA_FORCE_Q6_MSA_SHFT                                               2
#define HWIO_MSS_PERPH_MSS_MSA_MBA_OK_BMSK                                                   0x2
#define HWIO_MSS_PERPH_MSS_MSA_MBA_OK_SHFT                                                     1
#define HWIO_MSS_PERPH_MSS_MSA_CONFIG_LOCK_BMSK                                              0x1
#define HWIO_MSS_PERPH_MSS_MSA_CONFIG_LOCK_SHFT                                                0

#define HWIO_MSS_PERPH_MSS_HW_VERSION_ADDR                                            (MSS_PERPH_MSS_PERPH_REG_BASE            + 0x38)
#define HWIO_MSS_PERPH_MSS_HW_VERSION_RMSK                                            0xffffffff
#define HWIO_MSS_PERPH_MSS_HW_VERSION_IN                    \
                in_dword(HWIO_MSS_PERPH_MSS_HW_VERSION_ADDR)
#define HWIO_MSS_PERPH_MSS_HW_VERSION_INM(m)            \
                in_dword_masked(HWIO_MSS_PERPH_MSS_HW_VERSION_ADDR, m)
#define HWIO_MSS_PERPH_MSS_HW_VERSION_MAJOR_BMSK                                      0xf0000000
#define HWIO_MSS_PERPH_MSS_HW_VERSION_MAJOR_SHFT                                              28
#define HWIO_MSS_PERPH_MSS_HW_VERSION_MINOR_BMSK                                       0xfff0000
#define HWIO_MSS_PERPH_MSS_HW_VERSION_MINOR_SHFT                                              16
#define HWIO_MSS_PERPH_MSS_HW_VERSION_STEP_BMSK                                           0xffff
#define HWIO_MSS_PERPH_MSS_HW_VERSION_STEP_SHFT                                                0

#define HWIO_MSS_PERPH_MSS_BBRX_CTL_ADDR                                              (MSS_PERPH_MSS_PERPH_REG_BASE            + 0x40)
#define HWIO_MSS_PERPH_MSS_BBRX_CTL_RMSK                                                     0x3
#define HWIO_MSS_PERPH_MSS_BBRX_CTL_IN                    \
                in_dword(HWIO_MSS_PERPH_MSS_BBRX_CTL_ADDR)
#define HWIO_MSS_PERPH_MSS_BBRX_CTL_INM(m)            \
                in_dword_masked(HWIO_MSS_PERPH_MSS_BBRX_CTL_ADDR, m)
#define HWIO_MSS_PERPH_MSS_BBRX_CTL_OUT(v)            \
                out_dword(HWIO_MSS_PERPH_MSS_BBRX_CTL_ADDR,v)
#define HWIO_MSS_PERPH_MSS_BBRX_CTL_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_PERPH_MSS_BBRX_CTL_ADDR,m,v,HWIO_MSS_PERPH_MSS_BBRX_CTL_IN)
#define HWIO_MSS_PERPH_MSS_BBRX_CTL_BBRX_HS_TEST_MUX_CTL_BMSK                                0x3
#define HWIO_MSS_PERPH_MSS_BBRX_CTL_BBRX_HS_TEST_MUX_CTL_SHFT                                  0

#define HWIO_MSS_PERPH_MSS_DEBUG_CTL_ADDR                                             (MSS_PERPH_MSS_PERPH_REG_BASE            + 0x44)
#define HWIO_MSS_PERPH_MSS_DEBUG_CTL_RMSK                                                    0x7
#define HWIO_MSS_PERPH_MSS_DEBUG_CTL_IN                    \
                in_dword(HWIO_MSS_PERPH_MSS_DEBUG_CTL_ADDR)
#define HWIO_MSS_PERPH_MSS_DEBUG_CTL_INM(m)            \
                in_dword_masked(HWIO_MSS_PERPH_MSS_DEBUG_CTL_ADDR, m)
#define HWIO_MSS_PERPH_MSS_DEBUG_CTL_OUT(v)            \
                out_dword(HWIO_MSS_PERPH_MSS_DEBUG_CTL_ADDR,v)
#define HWIO_MSS_PERPH_MSS_DEBUG_CTL_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_PERPH_MSS_DEBUG_CTL_ADDR,m,v,HWIO_MSS_PERPH_MSS_DEBUG_CTL_IN)
#define HWIO_MSS_PERPH_MSS_DEBUG_CTL_DAC_DISABLE_ON_Q6_DBG_BMSK                              0x4
#define HWIO_MSS_PERPH_MSS_DEBUG_CTL_DAC_DISABLE_ON_Q6_DBG_SHFT                                2
#define HWIO_MSS_PERPH_MSS_DEBUG_CTL_GRFC_DISABLE_Q6_DBG_BMSK                                0x2
#define HWIO_MSS_PERPH_MSS_DEBUG_CTL_GRFC_DISABLE_Q6_DBG_SHFT                                  1
#define HWIO_MSS_PERPH_MSS_DEBUG_CTL_GRFC_DISABLE_Q6_WDOG_BMSK                               0x1
#define HWIO_MSS_PERPH_MSS_DEBUG_CTL_GRFC_DISABLE_Q6_WDOG_SHFT                                 0

#define HWIO_MSS_PERPH_MSS_MSA_NC_HM_ADDR                                             (MSS_PERPH_MSS_PERPH_REG_BASE            + 0x4c)
#define HWIO_MSS_PERPH_MSS_MSA_NC_HM_RMSK                                             0xfffffeff
#define HWIO_MSS_PERPH_MSS_MSA_NC_HM_IN                    \
                in_dword(HWIO_MSS_PERPH_MSS_MSA_NC_HM_ADDR)
#define HWIO_MSS_PERPH_MSS_MSA_NC_HM_INM(m)            \
                in_dword_masked(HWIO_MSS_PERPH_MSS_MSA_NC_HM_ADDR, m)
#define HWIO_MSS_PERPH_MSS_MSA_NC_HM_OUT(v)            \
                out_dword(HWIO_MSS_PERPH_MSS_MSA_NC_HM_ADDR,v)
#define HWIO_MSS_PERPH_MSS_MSA_NC_HM_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_PERPH_MSS_MSA_NC_HM_ADDR,m,v,HWIO_MSS_PERPH_MSS_MSA_NC_HM_IN)
#define HWIO_MSS_PERPH_MSS_MSA_NC_HM_RESERVE_31_9_BMSK                                0xfffffe00
#define HWIO_MSS_PERPH_MSS_MSA_NC_HM_RESERVE_31_9_SHFT                                         9
#define HWIO_MSS_PERPH_MSS_MSA_NC_HM_RESERVE_7_4_BMSK                                       0xf0
#define HWIO_MSS_PERPH_MSS_MSA_NC_HM_RESERVE_7_4_SHFT                                          4
#define HWIO_MSS_PERPH_MSS_MSA_NC_HM_CRYPTO_BMSK                                             0xf
#define HWIO_MSS_PERPH_MSS_MSA_NC_HM_CRYPTO_SHFT                                               0

#define HWIO_MSS_PERPH_MSS_CXM_ADDR                                                   (MSS_PERPH_MSS_PERPH_REG_BASE            + 0x50)
#define HWIO_MSS_PERPH_MSS_CXM_RMSK                                                   0xffffffff
#define HWIO_MSS_PERPH_MSS_CXM_IN                    \
                in_dword(HWIO_MSS_PERPH_MSS_CXM_ADDR)
#define HWIO_MSS_PERPH_MSS_CXM_INM(m)            \
                in_dword_masked(HWIO_MSS_PERPH_MSS_CXM_ADDR, m)
#define HWIO_MSS_PERPH_MSS_CXM_OUT(v)            \
                out_dword(HWIO_MSS_PERPH_MSS_CXM_ADDR,v)
#define HWIO_MSS_PERPH_MSS_CXM_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_PERPH_MSS_CXM_ADDR,m,v,HWIO_MSS_PERPH_MSS_CXM_IN)
#define HWIO_MSS_PERPH_MSS_CXM_RESERVE_31_12_BMSK                                     0xfffff000
#define HWIO_MSS_PERPH_MSS_CXM_RESERVE_31_12_SHFT                                             12
#define HWIO_MSS_PERPH_MSS_CXM_USE_EXTERNAL_WLAN_BMSK                                      0x800
#define HWIO_MSS_PERPH_MSS_CXM_USE_EXTERNAL_WLAN_SHFT                                         11
#define HWIO_MSS_PERPH_MSS_CXM_BT_TX_ACT_LST_SUBFRM_BMSK                                   0x400
#define HWIO_MSS_PERPH_MSS_CXM_BT_TX_ACT_LST_SUBFRM_SHFT                                      10
#define HWIO_MSS_PERPH_MSS_CXM_WLAN_TX_ACT_LST_SUBFRM_BMSK                                 0x200
#define HWIO_MSS_PERPH_MSS_CXM_WLAN_TX_ACT_LST_SUBFRM_SHFT                                     9
#define HWIO_MSS_PERPH_MSS_CXM_BT_TX_S_WAN_BMSK                                            0x100
#define HWIO_MSS_PERPH_MSS_CXM_BT_TX_S_WAN_SHFT                                                8
#define HWIO_MSS_PERPH_MSS_CXM_WLAN_TX_S_WAN_BMSK                                           0x80
#define HWIO_MSS_PERPH_MSS_CXM_WLAN_TX_S_WAN_SHFT                                              7
#define HWIO_MSS_PERPH_MSS_CXM_BT_TX_REGISTERED_BMSK                                        0x40
#define HWIO_MSS_PERPH_MSS_CXM_BT_TX_REGISTERED_SHFT                                           6
#define HWIO_MSS_PERPH_MSS_CXM_WLAN_TX_REGISTERED_BMSK                                      0x20
#define HWIO_MSS_PERPH_MSS_CXM_WLAN_TX_REGISTERED_SHFT                                         5
#define HWIO_MSS_PERPH_MSS_CXM_WLAN_TX_STICKY_BMSK                                          0x10
#define HWIO_MSS_PERPH_MSS_CXM_WLAN_TX_STICKY_SHFT                                             4
#define HWIO_MSS_PERPH_MSS_CXM_WLAN_TX_STICKY_CLR_BMSK                                       0x8
#define HWIO_MSS_PERPH_MSS_CXM_WLAN_TX_STICKY_CLR_SHFT                                         3
#define HWIO_MSS_PERPH_MSS_CXM_CXM_WAKEUP_CLR_BMSK                                           0x4
#define HWIO_MSS_PERPH_MSS_CXM_CXM_WAKEUP_CLR_SHFT                                             2
#define HWIO_MSS_PERPH_MSS_CXM_BT_TX_ON_BLANKING_EN_BMSK                                     0x2
#define HWIO_MSS_PERPH_MSS_CXM_BT_TX_ON_BLANKING_EN_SHFT                                       1
#define HWIO_MSS_PERPH_MSS_CXM_WLAN_TX_ON_BLANKING_EN_BMSK                                   0x1
#define HWIO_MSS_PERPH_MSS_CXM_WLAN_TX_ON_BLANKING_EN_SHFT                                     0

#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_00_ADDR                                   (MSS_PERPH_MSS_PERPH_REG_BASE            + 0x54)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_00_RMSK                                   0xffffffff
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_00_IN                    \
                in_dword(HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_00_ADDR)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_00_INM(m)            \
                in_dword_masked(HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_00_ADDR, m)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_00_OUT(v)            \
                out_dword(HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_00_ADDR,v)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_00_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_00_ADDR,m,v,HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_00_IN)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_00_RELAY_MSG_SHADOW_DATA_00_BMSK          0xffffffff
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_00_RELAY_MSG_SHADOW_DATA_00_SHFT                   0

#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_01_ADDR                                   (MSS_PERPH_MSS_PERPH_REG_BASE            + 0x58)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_01_RMSK                                   0xffffffff
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_01_IN                    \
                in_dword(HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_01_ADDR)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_01_INM(m)            \
                in_dword_masked(HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_01_ADDR, m)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_01_OUT(v)            \
                out_dword(HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_01_ADDR,v)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_01_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_01_ADDR,m,v,HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_01_IN)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_01_RELAY_MSG_SHADOW_DATA_01_BMSK          0xffffffff
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_01_RELAY_MSG_SHADOW_DATA_01_SHFT                   0

#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_02_ADDR                                   (MSS_PERPH_MSS_PERPH_REG_BASE            + 0x5c)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_02_RMSK                                   0xffffffff
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_02_IN                    \
                in_dword(HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_02_ADDR)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_02_INM(m)            \
                in_dword_masked(HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_02_ADDR, m)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_02_OUT(v)            \
                out_dword(HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_02_ADDR,v)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_02_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_02_ADDR,m,v,HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_02_IN)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_02_RELAY_MSG_SHADOW_DATA_02_BMSK          0xffffffff
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_02_RELAY_MSG_SHADOW_DATA_02_SHFT                   0

#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_03_ADDR                                   (MSS_PERPH_MSS_PERPH_REG_BASE            + 0x60)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_03_RMSK                                   0xffffffff
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_03_IN                    \
                in_dword(HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_03_ADDR)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_03_INM(m)            \
                in_dword_masked(HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_03_ADDR, m)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_03_OUT(v)            \
                out_dword(HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_03_ADDR,v)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_03_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_03_ADDR,m,v,HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_03_IN)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_03_RELAY_MSG_SHADOW_DATA_03_BMSK          0xffffffff
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_03_RELAY_MSG_SHADOW_DATA_03_SHFT                   0

#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_04_ADDR                                   (MSS_PERPH_MSS_PERPH_REG_BASE            + 0x64)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_04_RMSK                                   0xffffffff
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_04_IN                    \
                in_dword(HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_04_ADDR)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_04_INM(m)            \
                in_dword_masked(HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_04_ADDR, m)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_04_OUT(v)            \
                out_dword(HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_04_ADDR,v)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_04_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_04_ADDR,m,v,HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_04_IN)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_04_RELAY_MSG_SHADOW_DATA_04_BMSK          0xffffffff
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_04_RELAY_MSG_SHADOW_DATA_04_SHFT                   0

#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_05_ADDR                                   (MSS_PERPH_MSS_PERPH_REG_BASE            + 0x68)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_05_RMSK                                   0xffffffff
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_05_IN                    \
                in_dword(HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_05_ADDR)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_05_INM(m)            \
                in_dword_masked(HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_05_ADDR, m)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_05_OUT(v)            \
                out_dword(HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_05_ADDR,v)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_05_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_05_ADDR,m,v,HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_05_IN)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_05_RELAY_MSG_SHADOW_DATA_05_BMSK          0xffffffff
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_05_RELAY_MSG_SHADOW_DATA_05_SHFT                   0

#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_06_ADDR                                   (MSS_PERPH_MSS_PERPH_REG_BASE            + 0x6c)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_06_RMSK                                   0xffffffff
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_06_IN                    \
                in_dword(HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_06_ADDR)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_06_INM(m)            \
                in_dword_masked(HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_06_ADDR, m)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_06_OUT(v)            \
                out_dword(HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_06_ADDR,v)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_06_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_06_ADDR,m,v,HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_06_IN)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_06_RELAY_MSG_SHADOW_DATA_06_BMSK          0xffffffff
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_06_RELAY_MSG_SHADOW_DATA_06_SHFT                   0

#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_07_ADDR                                   (MSS_PERPH_MSS_PERPH_REG_BASE            + 0x70)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_07_RMSK                                   0xffffffff
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_07_IN                    \
                in_dword(HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_07_ADDR)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_07_INM(m)            \
                in_dword_masked(HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_07_ADDR, m)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_07_OUT(v)            \
                out_dword(HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_07_ADDR,v)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_07_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_07_ADDR,m,v,HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_07_IN)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_07_RELAY_MSG_SHADOW_DATA_07_BMSK          0xffffffff
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_07_RELAY_MSG_SHADOW_DATA_07_SHFT                   0

#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_08_ADDR                                   (MSS_PERPH_MSS_PERPH_REG_BASE            + 0x74)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_08_RMSK                                   0xffffffff
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_08_IN                    \
                in_dword(HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_08_ADDR)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_08_INM(m)            \
                in_dword_masked(HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_08_ADDR, m)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_08_OUT(v)            \
                out_dword(HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_08_ADDR,v)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_08_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_08_ADDR,m,v,HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_08_IN)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_08_RELAY_MSG_SHADOW_DATA_08_BMSK          0xffffffff
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_08_RELAY_MSG_SHADOW_DATA_08_SHFT                   0

#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_09_ADDR                                   (MSS_PERPH_MSS_PERPH_REG_BASE            + 0x78)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_09_RMSK                                   0xffffffff
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_09_IN                    \
                in_dword(HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_09_ADDR)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_09_INM(m)            \
                in_dword_masked(HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_09_ADDR, m)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_09_OUT(v)            \
                out_dword(HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_09_ADDR,v)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_09_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_09_ADDR,m,v,HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_09_IN)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_09_RELAY_MSG_SHADOW_DATA_09_BMSK          0xffffffff
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_09_RELAY_MSG_SHADOW_DATA_09_SHFT                   0

#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_10_ADDR                                   (MSS_PERPH_MSS_PERPH_REG_BASE            + 0x7c)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_10_RMSK                                   0xffffffff
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_10_IN                    \
                in_dword(HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_10_ADDR)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_10_INM(m)            \
                in_dword_masked(HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_10_ADDR, m)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_10_OUT(v)            \
                out_dword(HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_10_ADDR,v)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_10_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_10_ADDR,m,v,HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_10_IN)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_10_RELAY_MSG_SHADOW_DATA_10_BMSK          0xffffffff
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_10_RELAY_MSG_SHADOW_DATA_10_SHFT                   0

#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_11_ADDR                                   (MSS_PERPH_MSS_PERPH_REG_BASE            + 0x80)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_11_RMSK                                   0xffffffff
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_11_IN                    \
                in_dword(HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_11_ADDR)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_11_INM(m)            \
                in_dword_masked(HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_11_ADDR, m)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_11_OUT(v)            \
                out_dword(HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_11_ADDR,v)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_11_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_11_ADDR,m,v,HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_11_IN)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_11_RELAY_MSG_SHADOW_DATA_11_BMSK          0xffffffff
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_11_RELAY_MSG_SHADOW_DATA_11_SHFT                   0

#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_12_ADDR                                   (MSS_PERPH_MSS_PERPH_REG_BASE            + 0x84)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_12_RMSK                                   0xffffffff
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_12_IN                    \
                in_dword(HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_12_ADDR)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_12_INM(m)            \
                in_dword_masked(HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_12_ADDR, m)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_12_OUT(v)            \
                out_dword(HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_12_ADDR,v)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_12_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_12_ADDR,m,v,HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_12_IN)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_12_RELAY_MSG_SHADOW_DATA_12_BMSK          0xffffffff
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_12_RELAY_MSG_SHADOW_DATA_12_SHFT                   0

#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_13_ADDR                                   (MSS_PERPH_MSS_PERPH_REG_BASE            + 0x88)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_13_RMSK                                   0xffffffff
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_13_IN                    \
                in_dword(HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_13_ADDR)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_13_INM(m)            \
                in_dword_masked(HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_13_ADDR, m)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_13_OUT(v)            \
                out_dword(HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_13_ADDR,v)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_13_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_13_ADDR,m,v,HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_13_IN)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_13_RELAY_MSG_SHADOW_DATA_13_BMSK          0xffffffff
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_13_RELAY_MSG_SHADOW_DATA_13_SHFT                   0

#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_14_ADDR                                   (MSS_PERPH_MSS_PERPH_REG_BASE            + 0x8c)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_14_RMSK                                   0xffffffff
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_14_IN                    \
                in_dword(HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_14_ADDR)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_14_INM(m)            \
                in_dword_masked(HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_14_ADDR, m)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_14_OUT(v)            \
                out_dword(HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_14_ADDR,v)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_14_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_14_ADDR,m,v,HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_14_IN)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_14_RELAY_MSG_SHADOW_DATA_14_BMSK          0xffffffff
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_14_RELAY_MSG_SHADOW_DATA_14_SHFT                   0

#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_15_ADDR                                   (MSS_PERPH_MSS_PERPH_REG_BASE            + 0x90)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_15_RMSK                                   0xffffffff
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_15_IN                    \
                in_dword(HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_15_ADDR)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_15_INM(m)            \
                in_dword_masked(HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_15_ADDR, m)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_15_OUT(v)            \
                out_dword(HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_15_ADDR,v)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_15_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_15_ADDR,m,v,HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_15_IN)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_15_RELAY_MSG_SHADOW_DATA_15_BMSK          0xffffffff
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_15_RELAY_MSG_SHADOW_DATA_15_SHFT                   0

#define HWIO_MSS_PERPH_EXT_GPIO_SEL_CFG_ADDR                                          (MSS_PERPH_MSS_PERPH_REG_BASE            + 0x94)
#define HWIO_MSS_PERPH_EXT_GPIO_SEL_CFG_RMSK                                                0xff
#define HWIO_MSS_PERPH_EXT_GPIO_SEL_CFG_IN                    \
                in_dword(HWIO_MSS_PERPH_EXT_GPIO_SEL_CFG_ADDR)
#define HWIO_MSS_PERPH_EXT_GPIO_SEL_CFG_INM(m)            \
                in_dword_masked(HWIO_MSS_PERPH_EXT_GPIO_SEL_CFG_ADDR, m)
#define HWIO_MSS_PERPH_EXT_GPIO_SEL_CFG_OUT(v)            \
                out_dword(HWIO_MSS_PERPH_EXT_GPIO_SEL_CFG_ADDR,v)
#define HWIO_MSS_PERPH_EXT_GPIO_SEL_CFG_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_PERPH_EXT_GPIO_SEL_CFG_ADDR,m,v,HWIO_MSS_PERPH_EXT_GPIO_SEL_CFG_IN)
#define HWIO_MSS_PERPH_EXT_GPIO_SEL_CFG_EXT_GPIO_SEL_CFG_BMSK                               0xff
#define HWIO_MSS_PERPH_EXT_GPIO_SEL_CFG_EXT_GPIO_SEL_CFG_SHFT                                  0

#define HWIO_MSS_PERPH_MSS_BIMC_THROTTLE_ADDR                                         (MSS_PERPH_MSS_PERPH_REG_BASE            + 0x9c)
#define HWIO_MSS_PERPH_MSS_BIMC_THROTTLE_RMSK                                                0x7
#define HWIO_MSS_PERPH_MSS_BIMC_THROTTLE_IN                    \
                in_dword(HWIO_MSS_PERPH_MSS_BIMC_THROTTLE_ADDR)
#define HWIO_MSS_PERPH_MSS_BIMC_THROTTLE_INM(m)            \
                in_dword_masked(HWIO_MSS_PERPH_MSS_BIMC_THROTTLE_ADDR, m)
#define HWIO_MSS_PERPH_MSS_BIMC_THROTTLE_OUT(v)            \
                out_dword(HWIO_MSS_PERPH_MSS_BIMC_THROTTLE_ADDR,v)
#define HWIO_MSS_PERPH_MSS_BIMC_THROTTLE_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_PERPH_MSS_BIMC_THROTTLE_ADDR,m,v,HWIO_MSS_PERPH_MSS_BIMC_THROTTLE_IN)
#define HWIO_MSS_PERPH_MSS_BIMC_THROTTLE_MSS_DANGER_BMSK                                     0x6
#define HWIO_MSS_PERPH_MSS_BIMC_THROTTLE_MSS_DANGER_SHFT                                       1
#define HWIO_MSS_PERPH_MSS_BIMC_THROTTLE_MSS_SAFE_BMSK                                       0x1
#define HWIO_MSS_PERPH_MSS_BIMC_THROTTLE_MSS_SAFE_SHFT                                         0

#define HWIO_MSS_PERPH_MSS_TCSR_ACC_SEL_ADDR                                          (MSS_PERPH_MSS_PERPH_REG_BASE            + 0xd890)
#define HWIO_MSS_PERPH_MSS_TCSR_ACC_SEL_RMSK                                                 0x3
#define HWIO_MSS_PERPH_MSS_TCSR_ACC_SEL_IN                    \
                in_dword(HWIO_MSS_PERPH_MSS_TCSR_ACC_SEL_ADDR)
#define HWIO_MSS_PERPH_MSS_TCSR_ACC_SEL_INM(m)            \
                in_dword_masked(HWIO_MSS_PERPH_MSS_TCSR_ACC_SEL_ADDR, m)
#define HWIO_MSS_PERPH_MSS_TCSR_ACC_SEL_OUT(v)            \
                out_dword(HWIO_MSS_PERPH_MSS_TCSR_ACC_SEL_ADDR,v)
#define HWIO_MSS_PERPH_MSS_TCSR_ACC_SEL_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_PERPH_MSS_TCSR_ACC_SEL_ADDR,m,v,HWIO_MSS_PERPH_MSS_TCSR_ACC_SEL_IN)
#define HWIO_MSS_PERPH_MSS_TCSR_ACC_SEL_ACC_MEM_SEL_BMSK                                     0x3
#define HWIO_MSS_PERPH_MSS_TCSR_ACC_SEL_ACC_MEM_SEL_SHFT                                       0

/*----------------------------------------------------------------------------
 * MODULE: SSC_TLMM_CSR
 *--------------------------------------------------------------------------*/

#define SSC_TLMM_CSR_REG_BASE                                       (LPASS_BASE            + 0x00c40000)
#define SSC_TLMM_CSR_REG_BASE_SIZE                                  0x20000
#define SSC_TLMM_CSR_REG_BASE_USED                                  0xf004

#define HWIO_SSC_TLMM_LITE_GPIO_CFG_0_ADDR                          (SSC_TLMM_CSR_REG_BASE            + 0x0)
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_0_RMSK                              0xffff
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_0_IN                    \
                in_dword(HWIO_SSC_TLMM_LITE_GPIO_CFG_0_ADDR)
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_0_INM(m)            \
                in_dword_masked(HWIO_SSC_TLMM_LITE_GPIO_CFG_0_ADDR, m)
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_0_OUT(v)            \
                out_dword(HWIO_SSC_TLMM_LITE_GPIO_CFG_0_ADDR,v)
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_0_OUTM(m,v) \
                out_dword_masked_ns(HWIO_SSC_TLMM_LITE_GPIO_CFG_0_ADDR,m,v,HWIO_SSC_TLMM_LITE_GPIO_CFG_0_IN)
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_0_GPIO_MISC_CFG_BMSK                0xf800
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_0_GPIO_MISC_CFG_SHFT                    11
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_0_GPIO_HIHYS_CTL_BMSK                0x400
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_0_GPIO_HIHYS_CTL_SHFT                   10
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_0_GPIO_OE_BMSK                       0x200
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_0_GPIO_OE_SHFT                           9
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_0_GPIO_HDRIVE_BMSK                   0x1c0
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_0_GPIO_HDRIVE_SHFT                       6
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_0_GPIO_FUNC_SEL_BMSK                  0x3c
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_0_GPIO_FUNC_SEL_SHFT                     2
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_0_GPIO_PULL_BMSK                       0x3
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_0_GPIO_PULL_SHFT                         0

#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_0_ADDR                       (SSC_TLMM_CSR_REG_BASE            + 0x4)
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_0_RMSK                              0x3
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_0_IN                    \
                in_dword(HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_0_ADDR)
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_0_INM(m)            \
                in_dword_masked(HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_0_ADDR, m)
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_0_OUT(v)            \
                out_dword(HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_0_ADDR,v)
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_0_OUTM(m,v) \
                out_dword_masked_ns(HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_0_ADDR,m,v,HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_0_IN)
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_0_GPIO_OUT_BMSK                     0x2
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_0_GPIO_OUT_SHFT                       1
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_0_GPIO_IN_BMSK                      0x1
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_0_GPIO_IN_SHFT                        0

#define HWIO_SSC_TLMM_LITE_GPIO_CFG_1_ADDR                          (SSC_TLMM_CSR_REG_BASE            + 0x1000)
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_1_RMSK                              0xffff
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_1_IN                    \
                in_dword(HWIO_SSC_TLMM_LITE_GPIO_CFG_1_ADDR)
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_1_INM(m)            \
                in_dword_masked(HWIO_SSC_TLMM_LITE_GPIO_CFG_1_ADDR, m)
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_1_OUT(v)            \
                out_dword(HWIO_SSC_TLMM_LITE_GPIO_CFG_1_ADDR,v)
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_1_OUTM(m,v) \
                out_dword_masked_ns(HWIO_SSC_TLMM_LITE_GPIO_CFG_1_ADDR,m,v,HWIO_SSC_TLMM_LITE_GPIO_CFG_1_IN)
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_1_GPIO_MISC_CFG_BMSK                0xf800
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_1_GPIO_MISC_CFG_SHFT                    11
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_1_GPIO_HIHYS_CTL_BMSK                0x400
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_1_GPIO_HIHYS_CTL_SHFT                   10
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_1_GPIO_OE_BMSK                       0x200
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_1_GPIO_OE_SHFT                           9
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_1_GPIO_HDRIVE_BMSK                   0x1c0
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_1_GPIO_HDRIVE_SHFT                       6
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_1_GPIO_FUNC_SEL_BMSK                  0x3c
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_1_GPIO_FUNC_SEL_SHFT                     2
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_1_GPIO_PULL_BMSK                       0x3
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_1_GPIO_PULL_SHFT                         0

#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_1_ADDR                       (SSC_TLMM_CSR_REG_BASE            + 0x1004)
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_1_RMSK                              0x3
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_1_IN                    \
                in_dword(HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_1_ADDR)
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_1_INM(m)            \
                in_dword_masked(HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_1_ADDR, m)
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_1_OUT(v)            \
                out_dword(HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_1_ADDR,v)
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_1_OUTM(m,v) \
                out_dword_masked_ns(HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_1_ADDR,m,v,HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_1_IN)
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_1_GPIO_OUT_BMSK                     0x2
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_1_GPIO_OUT_SHFT                       1
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_1_GPIO_IN_BMSK                      0x1
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_1_GPIO_IN_SHFT                        0

#define HWIO_SSC_TLMM_LITE_GPIO_CFG_2_ADDR                          (SSC_TLMM_CSR_REG_BASE            + 0x2000)
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_2_RMSK                              0xffff
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_2_IN                    \
                in_dword(HWIO_SSC_TLMM_LITE_GPIO_CFG_2_ADDR)
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_2_INM(m)            \
                in_dword_masked(HWIO_SSC_TLMM_LITE_GPIO_CFG_2_ADDR, m)
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_2_OUT(v)            \
                out_dword(HWIO_SSC_TLMM_LITE_GPIO_CFG_2_ADDR,v)
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_2_OUTM(m,v) \
                out_dword_masked_ns(HWIO_SSC_TLMM_LITE_GPIO_CFG_2_ADDR,m,v,HWIO_SSC_TLMM_LITE_GPIO_CFG_2_IN)
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_2_GPIO_MISC_CFG_BMSK                0xf800
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_2_GPIO_MISC_CFG_SHFT                    11
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_2_GPIO_HIHYS_CTL_BMSK                0x400
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_2_GPIO_HIHYS_CTL_SHFT                   10
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_2_GPIO_OE_BMSK                       0x200
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_2_GPIO_OE_SHFT                           9
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_2_GPIO_HDRIVE_BMSK                   0x1c0
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_2_GPIO_HDRIVE_SHFT                       6
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_2_GPIO_FUNC_SEL_BMSK                  0x3c
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_2_GPIO_FUNC_SEL_SHFT                     2
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_2_GPIO_PULL_BMSK                       0x3
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_2_GPIO_PULL_SHFT                         0

#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_2_ADDR                       (SSC_TLMM_CSR_REG_BASE            + 0x2004)
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_2_RMSK                              0x3
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_2_IN                    \
                in_dword(HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_2_ADDR)
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_2_INM(m)            \
                in_dword_masked(HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_2_ADDR, m)
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_2_OUT(v)            \
                out_dword(HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_2_ADDR,v)
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_2_OUTM(m,v) \
                out_dword_masked_ns(HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_2_ADDR,m,v,HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_2_IN)
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_2_GPIO_OUT_BMSK                     0x2
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_2_GPIO_OUT_SHFT                       1
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_2_GPIO_IN_BMSK                      0x1
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_2_GPIO_IN_SHFT                        0

#define HWIO_SSC_TLMM_LITE_GPIO_CFG_3_ADDR                          (SSC_TLMM_CSR_REG_BASE            + 0x3000)
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_3_RMSK                              0xffff
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_3_IN                    \
                in_dword(HWIO_SSC_TLMM_LITE_GPIO_CFG_3_ADDR)
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_3_INM(m)            \
                in_dword_masked(HWIO_SSC_TLMM_LITE_GPIO_CFG_3_ADDR, m)
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_3_OUT(v)            \
                out_dword(HWIO_SSC_TLMM_LITE_GPIO_CFG_3_ADDR,v)
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_3_OUTM(m,v) \
                out_dword_masked_ns(HWIO_SSC_TLMM_LITE_GPIO_CFG_3_ADDR,m,v,HWIO_SSC_TLMM_LITE_GPIO_CFG_3_IN)
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_3_GPIO_MISC_CFG_BMSK                0xf800
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_3_GPIO_MISC_CFG_SHFT                    11
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_3_GPIO_HIHYS_CTL_BMSK                0x400
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_3_GPIO_HIHYS_CTL_SHFT                   10
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_3_GPIO_OE_BMSK                       0x200
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_3_GPIO_OE_SHFT                           9
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_3_GPIO_HDRIVE_BMSK                   0x1c0
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_3_GPIO_HDRIVE_SHFT                       6
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_3_GPIO_FUNC_SEL_BMSK                  0x3c
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_3_GPIO_FUNC_SEL_SHFT                     2
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_3_GPIO_PULL_BMSK                       0x3
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_3_GPIO_PULL_SHFT                         0

#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_3_ADDR                       (SSC_TLMM_CSR_REG_BASE            + 0x3004)
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_3_RMSK                              0x3
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_3_IN                    \
                in_dword(HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_3_ADDR)
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_3_INM(m)            \
                in_dword_masked(HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_3_ADDR, m)
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_3_OUT(v)            \
                out_dword(HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_3_ADDR,v)
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_3_OUTM(m,v) \
                out_dword_masked_ns(HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_3_ADDR,m,v,HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_3_IN)
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_3_GPIO_OUT_BMSK                     0x2
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_3_GPIO_OUT_SHFT                       1
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_3_GPIO_IN_BMSK                      0x1
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_3_GPIO_IN_SHFT                        0

#define HWIO_SSC_TLMM_LITE_GPIO_CFG_4_ADDR                          (SSC_TLMM_CSR_REG_BASE            + 0x4000)
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_4_RMSK                              0xffff
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_4_IN                    \
                in_dword(HWIO_SSC_TLMM_LITE_GPIO_CFG_4_ADDR)
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_4_INM(m)            \
                in_dword_masked(HWIO_SSC_TLMM_LITE_GPIO_CFG_4_ADDR, m)
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_4_OUT(v)            \
                out_dword(HWIO_SSC_TLMM_LITE_GPIO_CFG_4_ADDR,v)
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_4_OUTM(m,v) \
                out_dword_masked_ns(HWIO_SSC_TLMM_LITE_GPIO_CFG_4_ADDR,m,v,HWIO_SSC_TLMM_LITE_GPIO_CFG_4_IN)
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_4_GPIO_MISC_CFG_BMSK                0xf800
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_4_GPIO_MISC_CFG_SHFT                    11
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_4_GPIO_HIHYS_CTL_BMSK                0x400
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_4_GPIO_HIHYS_CTL_SHFT                   10
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_4_GPIO_OE_BMSK                       0x200
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_4_GPIO_OE_SHFT                           9
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_4_GPIO_HDRIVE_BMSK                   0x1c0
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_4_GPIO_HDRIVE_SHFT                       6
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_4_GPIO_FUNC_SEL_BMSK                  0x3c
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_4_GPIO_FUNC_SEL_SHFT                     2
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_4_GPIO_PULL_BMSK                       0x3
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_4_GPIO_PULL_SHFT                         0

#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_4_ADDR                       (SSC_TLMM_CSR_REG_BASE            + 0x4004)
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_4_RMSK                              0x3
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_4_IN                    \
                in_dword(HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_4_ADDR)
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_4_INM(m)            \
                in_dword_masked(HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_4_ADDR, m)
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_4_OUT(v)            \
                out_dword(HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_4_ADDR,v)
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_4_OUTM(m,v) \
                out_dword_masked_ns(HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_4_ADDR,m,v,HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_4_IN)
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_4_GPIO_OUT_BMSK                     0x2
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_4_GPIO_OUT_SHFT                       1
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_4_GPIO_IN_BMSK                      0x1
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_4_GPIO_IN_SHFT                        0

#define HWIO_SSC_TLMM_LITE_GPIO_CFG_5_ADDR                          (SSC_TLMM_CSR_REG_BASE            + 0x5000)
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_5_RMSK                              0xffff
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_5_IN                    \
                in_dword(HWIO_SSC_TLMM_LITE_GPIO_CFG_5_ADDR)
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_5_INM(m)            \
                in_dword_masked(HWIO_SSC_TLMM_LITE_GPIO_CFG_5_ADDR, m)
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_5_OUT(v)            \
                out_dword(HWIO_SSC_TLMM_LITE_GPIO_CFG_5_ADDR,v)
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_5_OUTM(m,v) \
                out_dword_masked_ns(HWIO_SSC_TLMM_LITE_GPIO_CFG_5_ADDR,m,v,HWIO_SSC_TLMM_LITE_GPIO_CFG_5_IN)
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_5_GPIO_MISC_CFG_BMSK                0xf800
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_5_GPIO_MISC_CFG_SHFT                    11
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_5_GPIO_HIHYS_CTL_BMSK                0x400
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_5_GPIO_HIHYS_CTL_SHFT                   10
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_5_GPIO_OE_BMSK                       0x200
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_5_GPIO_OE_SHFT                           9
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_5_GPIO_HDRIVE_BMSK                   0x1c0
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_5_GPIO_HDRIVE_SHFT                       6
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_5_GPIO_FUNC_SEL_BMSK                  0x3c
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_5_GPIO_FUNC_SEL_SHFT                     2
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_5_GPIO_PULL_BMSK                       0x3
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_5_GPIO_PULL_SHFT                         0

#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_5_ADDR                       (SSC_TLMM_CSR_REG_BASE            + 0x5004)
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_5_RMSK                              0x3
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_5_IN                    \
                in_dword(HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_5_ADDR)
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_5_INM(m)            \
                in_dword_masked(HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_5_ADDR, m)
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_5_OUT(v)            \
                out_dword(HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_5_ADDR,v)
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_5_OUTM(m,v) \
                out_dword_masked_ns(HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_5_ADDR,m,v,HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_5_IN)
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_5_GPIO_OUT_BMSK                     0x2
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_5_GPIO_OUT_SHFT                       1
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_5_GPIO_IN_BMSK                      0x1
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_5_GPIO_IN_SHFT                        0

#define HWIO_SSC_TLMM_LITE_GPIO_CFG_6_ADDR                          (SSC_TLMM_CSR_REG_BASE            + 0x6000)
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_6_RMSK                              0xffff
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_6_IN                    \
                in_dword(HWIO_SSC_TLMM_LITE_GPIO_CFG_6_ADDR)
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_6_INM(m)            \
                in_dword_masked(HWIO_SSC_TLMM_LITE_GPIO_CFG_6_ADDR, m)
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_6_OUT(v)            \
                out_dword(HWIO_SSC_TLMM_LITE_GPIO_CFG_6_ADDR,v)
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_6_OUTM(m,v) \
                out_dword_masked_ns(HWIO_SSC_TLMM_LITE_GPIO_CFG_6_ADDR,m,v,HWIO_SSC_TLMM_LITE_GPIO_CFG_6_IN)
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_6_GPIO_MISC_CFG_BMSK                0xf800
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_6_GPIO_MISC_CFG_SHFT                    11
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_6_GPIO_HIHYS_CTL_BMSK                0x400
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_6_GPIO_HIHYS_CTL_SHFT                   10
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_6_GPIO_OE_BMSK                       0x200
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_6_GPIO_OE_SHFT                           9
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_6_GPIO_HDRIVE_BMSK                   0x1c0
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_6_GPIO_HDRIVE_SHFT                       6
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_6_GPIO_FUNC_SEL_BMSK                  0x3c
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_6_GPIO_FUNC_SEL_SHFT                     2
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_6_GPIO_PULL_BMSK                       0x3
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_6_GPIO_PULL_SHFT                         0

#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_6_ADDR                       (SSC_TLMM_CSR_REG_BASE            + 0x6004)
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_6_RMSK                              0x3
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_6_IN                    \
                in_dword(HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_6_ADDR)
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_6_INM(m)            \
                in_dword_masked(HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_6_ADDR, m)
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_6_OUT(v)            \
                out_dword(HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_6_ADDR,v)
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_6_OUTM(m,v) \
                out_dword_masked_ns(HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_6_ADDR,m,v,HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_6_IN)
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_6_GPIO_OUT_BMSK                     0x2
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_6_GPIO_OUT_SHFT                       1
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_6_GPIO_IN_BMSK                      0x1
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_6_GPIO_IN_SHFT                        0

#define HWIO_SSC_TLMM_LITE_GPIO_CFG_7_ADDR                          (SSC_TLMM_CSR_REG_BASE            + 0x7000)
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_7_RMSK                              0xffff
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_7_IN                    \
                in_dword(HWIO_SSC_TLMM_LITE_GPIO_CFG_7_ADDR)
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_7_INM(m)            \
                in_dword_masked(HWIO_SSC_TLMM_LITE_GPIO_CFG_7_ADDR, m)
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_7_OUT(v)            \
                out_dword(HWIO_SSC_TLMM_LITE_GPIO_CFG_7_ADDR,v)
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_7_OUTM(m,v) \
                out_dword_masked_ns(HWIO_SSC_TLMM_LITE_GPIO_CFG_7_ADDR,m,v,HWIO_SSC_TLMM_LITE_GPIO_CFG_7_IN)
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_7_GPIO_MISC_CFG_BMSK                0xf800
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_7_GPIO_MISC_CFG_SHFT                    11
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_7_GPIO_HIHYS_CTL_BMSK                0x400
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_7_GPIO_HIHYS_CTL_SHFT                   10
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_7_GPIO_OE_BMSK                       0x200
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_7_GPIO_OE_SHFT                           9
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_7_GPIO_HDRIVE_BMSK                   0x1c0
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_7_GPIO_HDRIVE_SHFT                       6
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_7_GPIO_FUNC_SEL_BMSK                  0x3c
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_7_GPIO_FUNC_SEL_SHFT                     2
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_7_GPIO_PULL_BMSK                       0x3
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_7_GPIO_PULL_SHFT                         0

#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_7_ADDR                       (SSC_TLMM_CSR_REG_BASE            + 0x7004)
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_7_RMSK                              0x3
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_7_IN                    \
                in_dword(HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_7_ADDR)
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_7_INM(m)            \
                in_dword_masked(HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_7_ADDR, m)
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_7_OUT(v)            \
                out_dword(HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_7_ADDR,v)
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_7_OUTM(m,v) \
                out_dword_masked_ns(HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_7_ADDR,m,v,HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_7_IN)
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_7_GPIO_OUT_BMSK                     0x2
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_7_GPIO_OUT_SHFT                       1
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_7_GPIO_IN_BMSK                      0x1
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_7_GPIO_IN_SHFT                        0

#define HWIO_SSC_TLMM_LITE_GPIO_CFG_8_ADDR                          (SSC_TLMM_CSR_REG_BASE            + 0x8000)
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_8_RMSK                              0xffff
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_8_IN                    \
                in_dword(HWIO_SSC_TLMM_LITE_GPIO_CFG_8_ADDR)
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_8_INM(m)            \
                in_dword_masked(HWIO_SSC_TLMM_LITE_GPIO_CFG_8_ADDR, m)
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_8_OUT(v)            \
                out_dword(HWIO_SSC_TLMM_LITE_GPIO_CFG_8_ADDR,v)
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_8_OUTM(m,v) \
                out_dword_masked_ns(HWIO_SSC_TLMM_LITE_GPIO_CFG_8_ADDR,m,v,HWIO_SSC_TLMM_LITE_GPIO_CFG_8_IN)
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_8_GPIO_MISC_CFG_BMSK                0xf800
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_8_GPIO_MISC_CFG_SHFT                    11
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_8_GPIO_HIHYS_CTL_BMSK                0x400
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_8_GPIO_HIHYS_CTL_SHFT                   10
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_8_GPIO_OE_BMSK                       0x200
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_8_GPIO_OE_SHFT                           9
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_8_GPIO_HDRIVE_BMSK                   0x1c0
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_8_GPIO_HDRIVE_SHFT                       6
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_8_GPIO_FUNC_SEL_BMSK                  0x3c
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_8_GPIO_FUNC_SEL_SHFT                     2
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_8_GPIO_PULL_BMSK                       0x3
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_8_GPIO_PULL_SHFT                         0

#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_8_ADDR                       (SSC_TLMM_CSR_REG_BASE            + 0x8004)
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_8_RMSK                              0x3
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_8_IN                    \
                in_dword(HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_8_ADDR)
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_8_INM(m)            \
                in_dword_masked(HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_8_ADDR, m)
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_8_OUT(v)            \
                out_dword(HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_8_ADDR,v)
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_8_OUTM(m,v) \
                out_dword_masked_ns(HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_8_ADDR,m,v,HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_8_IN)
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_8_GPIO_OUT_BMSK                     0x2
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_8_GPIO_OUT_SHFT                       1
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_8_GPIO_IN_BMSK                      0x1
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_8_GPIO_IN_SHFT                        0

#define HWIO_SSC_TLMM_LITE_GPIO_CFG_9_ADDR                          (SSC_TLMM_CSR_REG_BASE            + 0x9000)
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_9_RMSK                              0xffff
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_9_IN                    \
                in_dword(HWIO_SSC_TLMM_LITE_GPIO_CFG_9_ADDR)
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_9_INM(m)            \
                in_dword_masked(HWIO_SSC_TLMM_LITE_GPIO_CFG_9_ADDR, m)
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_9_OUT(v)            \
                out_dword(HWIO_SSC_TLMM_LITE_GPIO_CFG_9_ADDR,v)
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_9_OUTM(m,v) \
                out_dword_masked_ns(HWIO_SSC_TLMM_LITE_GPIO_CFG_9_ADDR,m,v,HWIO_SSC_TLMM_LITE_GPIO_CFG_9_IN)
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_9_GPIO_MISC_CFG_BMSK                0xf800
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_9_GPIO_MISC_CFG_SHFT                    11
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_9_GPIO_HIHYS_CTL_BMSK                0x400
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_9_GPIO_HIHYS_CTL_SHFT                   10
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_9_GPIO_OE_BMSK                       0x200
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_9_GPIO_OE_SHFT                           9
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_9_GPIO_HDRIVE_BMSK                   0x1c0
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_9_GPIO_HDRIVE_SHFT                       6
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_9_GPIO_FUNC_SEL_BMSK                  0x3c
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_9_GPIO_FUNC_SEL_SHFT                     2
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_9_GPIO_PULL_BMSK                       0x3
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_9_GPIO_PULL_SHFT                         0

#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_9_ADDR                       (SSC_TLMM_CSR_REG_BASE            + 0x9004)
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_9_RMSK                              0x3
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_9_IN                    \
                in_dword(HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_9_ADDR)
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_9_INM(m)            \
                in_dword_masked(HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_9_ADDR, m)
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_9_OUT(v)            \
                out_dword(HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_9_ADDR,v)
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_9_OUTM(m,v) \
                out_dword_masked_ns(HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_9_ADDR,m,v,HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_9_IN)
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_9_GPIO_OUT_BMSK                     0x2
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_9_GPIO_OUT_SHFT                       1
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_9_GPIO_IN_BMSK                      0x1
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_9_GPIO_IN_SHFT                        0

#define HWIO_SSC_TLMM_LITE_GPIO_CFG_10_ADDR                         (SSC_TLMM_CSR_REG_BASE            + 0xa000)
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_10_RMSK                             0xffff
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_10_IN                    \
                in_dword(HWIO_SSC_TLMM_LITE_GPIO_CFG_10_ADDR)
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_10_INM(m)            \
                in_dword_masked(HWIO_SSC_TLMM_LITE_GPIO_CFG_10_ADDR, m)
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_10_OUT(v)            \
                out_dword(HWIO_SSC_TLMM_LITE_GPIO_CFG_10_ADDR,v)
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_10_OUTM(m,v) \
                out_dword_masked_ns(HWIO_SSC_TLMM_LITE_GPIO_CFG_10_ADDR,m,v,HWIO_SSC_TLMM_LITE_GPIO_CFG_10_IN)
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_10_GPIO_MISC_CFG_BMSK               0xf800
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_10_GPIO_MISC_CFG_SHFT                   11
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_10_GPIO_HIHYS_CTL_BMSK               0x400
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_10_GPIO_HIHYS_CTL_SHFT                  10
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_10_GPIO_OE_BMSK                      0x200
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_10_GPIO_OE_SHFT                          9
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_10_GPIO_HDRIVE_BMSK                  0x1c0
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_10_GPIO_HDRIVE_SHFT                      6
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_10_GPIO_FUNC_SEL_BMSK                 0x3c
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_10_GPIO_FUNC_SEL_SHFT                    2
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_10_GPIO_PULL_BMSK                      0x3
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_10_GPIO_PULL_SHFT                        0

#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_10_ADDR                      (SSC_TLMM_CSR_REG_BASE            + 0xa004)
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_10_RMSK                             0x3
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_10_IN                    \
                in_dword(HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_10_ADDR)
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_10_INM(m)            \
                in_dword_masked(HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_10_ADDR, m)
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_10_OUT(v)            \
                out_dword(HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_10_ADDR,v)
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_10_OUTM(m,v) \
                out_dword_masked_ns(HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_10_ADDR,m,v,HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_10_IN)
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_10_GPIO_OUT_BMSK                    0x2
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_10_GPIO_OUT_SHFT                      1
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_10_GPIO_IN_BMSK                     0x1
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_10_GPIO_IN_SHFT                       0

#define HWIO_SSC_TLMM_LITE_GPIO_CFG_11_ADDR                         (SSC_TLMM_CSR_REG_BASE            + 0xb000)
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_11_RMSK                             0xffff
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_11_IN                    \
                in_dword(HWIO_SSC_TLMM_LITE_GPIO_CFG_11_ADDR)
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_11_INM(m)            \
                in_dword_masked(HWIO_SSC_TLMM_LITE_GPIO_CFG_11_ADDR, m)
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_11_OUT(v)            \
                out_dword(HWIO_SSC_TLMM_LITE_GPIO_CFG_11_ADDR,v)
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_11_OUTM(m,v) \
                out_dword_masked_ns(HWIO_SSC_TLMM_LITE_GPIO_CFG_11_ADDR,m,v,HWIO_SSC_TLMM_LITE_GPIO_CFG_11_IN)
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_11_GPIO_MISC_CFG_BMSK               0xf800
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_11_GPIO_MISC_CFG_SHFT                   11
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_11_GPIO_HIHYS_CTL_BMSK               0x400
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_11_GPIO_HIHYS_CTL_SHFT                  10
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_11_GPIO_OE_BMSK                      0x200
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_11_GPIO_OE_SHFT                          9
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_11_GPIO_HDRIVE_BMSK                  0x1c0
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_11_GPIO_HDRIVE_SHFT                      6
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_11_GPIO_FUNC_SEL_BMSK                 0x3c
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_11_GPIO_FUNC_SEL_SHFT                    2
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_11_GPIO_PULL_BMSK                      0x3
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_11_GPIO_PULL_SHFT                        0

#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_11_ADDR                      (SSC_TLMM_CSR_REG_BASE            + 0xb004)
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_11_RMSK                             0x3
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_11_IN                    \
                in_dword(HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_11_ADDR)
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_11_INM(m)            \
                in_dword_masked(HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_11_ADDR, m)
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_11_OUT(v)            \
                out_dword(HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_11_ADDR,v)
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_11_OUTM(m,v) \
                out_dword_masked_ns(HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_11_ADDR,m,v,HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_11_IN)
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_11_GPIO_OUT_BMSK                    0x2
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_11_GPIO_OUT_SHFT                      1
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_11_GPIO_IN_BMSK                     0x1
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_11_GPIO_IN_SHFT                       0

#define HWIO_SSC_TLMM_LITE_GPIO_CFG_12_ADDR                         (SSC_TLMM_CSR_REG_BASE            + 0xc000)
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_12_RMSK                             0xffff
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_12_IN                    \
                in_dword(HWIO_SSC_TLMM_LITE_GPIO_CFG_12_ADDR)
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_12_INM(m)            \
                in_dword_masked(HWIO_SSC_TLMM_LITE_GPIO_CFG_12_ADDR, m)
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_12_OUT(v)            \
                out_dword(HWIO_SSC_TLMM_LITE_GPIO_CFG_12_ADDR,v)
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_12_OUTM(m,v) \
                out_dword_masked_ns(HWIO_SSC_TLMM_LITE_GPIO_CFG_12_ADDR,m,v,HWIO_SSC_TLMM_LITE_GPIO_CFG_12_IN)
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_12_GPIO_MISC_CFG_BMSK               0xf800
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_12_GPIO_MISC_CFG_SHFT                   11
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_12_GPIO_HIHYS_CTL_BMSK               0x400
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_12_GPIO_HIHYS_CTL_SHFT                  10
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_12_GPIO_OE_BMSK                      0x200
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_12_GPIO_OE_SHFT                          9
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_12_GPIO_HDRIVE_BMSK                  0x1c0
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_12_GPIO_HDRIVE_SHFT                      6
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_12_GPIO_FUNC_SEL_BMSK                 0x3c
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_12_GPIO_FUNC_SEL_SHFT                    2
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_12_GPIO_PULL_BMSK                      0x3
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_12_GPIO_PULL_SHFT                        0

#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_12_ADDR                      (SSC_TLMM_CSR_REG_BASE            + 0xc004)
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_12_RMSK                             0x3
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_12_IN                    \
                in_dword(HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_12_ADDR)
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_12_INM(m)            \
                in_dword_masked(HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_12_ADDR, m)
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_12_OUT(v)            \
                out_dword(HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_12_ADDR,v)
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_12_OUTM(m,v) \
                out_dword_masked_ns(HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_12_ADDR,m,v,HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_12_IN)
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_12_GPIO_OUT_BMSK                    0x2
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_12_GPIO_OUT_SHFT                      1
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_12_GPIO_IN_BMSK                     0x1
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_12_GPIO_IN_SHFT                       0

#define HWIO_SSC_TLMM_LITE_GPIO_CFG_13_ADDR                         (SSC_TLMM_CSR_REG_BASE            + 0xd000)
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_13_RMSK                             0xffff
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_13_IN                    \
                in_dword(HWIO_SSC_TLMM_LITE_GPIO_CFG_13_ADDR)
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_13_INM(m)            \
                in_dword_masked(HWIO_SSC_TLMM_LITE_GPIO_CFG_13_ADDR, m)
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_13_OUT(v)            \
                out_dword(HWIO_SSC_TLMM_LITE_GPIO_CFG_13_ADDR,v)
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_13_OUTM(m,v) \
                out_dword_masked_ns(HWIO_SSC_TLMM_LITE_GPIO_CFG_13_ADDR,m,v,HWIO_SSC_TLMM_LITE_GPIO_CFG_13_IN)
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_13_GPIO_MISC_CFG_BMSK               0xf800
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_13_GPIO_MISC_CFG_SHFT                   11
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_13_GPIO_HIHYS_CTL_BMSK               0x400
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_13_GPIO_HIHYS_CTL_SHFT                  10
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_13_GPIO_OE_BMSK                      0x200
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_13_GPIO_OE_SHFT                          9
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_13_GPIO_HDRIVE_BMSK                  0x1c0
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_13_GPIO_HDRIVE_SHFT                      6
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_13_GPIO_FUNC_SEL_BMSK                 0x3c
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_13_GPIO_FUNC_SEL_SHFT                    2
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_13_GPIO_PULL_BMSK                      0x3
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_13_GPIO_PULL_SHFT                        0

#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_13_ADDR                      (SSC_TLMM_CSR_REG_BASE            + 0xd004)
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_13_RMSK                             0x3
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_13_IN                    \
                in_dword(HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_13_ADDR)
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_13_INM(m)            \
                in_dword_masked(HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_13_ADDR, m)
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_13_OUT(v)            \
                out_dword(HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_13_ADDR,v)
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_13_OUTM(m,v) \
                out_dword_masked_ns(HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_13_ADDR,m,v,HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_13_IN)
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_13_GPIO_OUT_BMSK                    0x2
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_13_GPIO_OUT_SHFT                      1
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_13_GPIO_IN_BMSK                     0x1
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_13_GPIO_IN_SHFT                       0

#define HWIO_SSC_TLMM_LITE_GPIO_CFG_14_ADDR                         (SSC_TLMM_CSR_REG_BASE            + 0xe000)
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_14_RMSK                             0xffff
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_14_IN                    \
                in_dword(HWIO_SSC_TLMM_LITE_GPIO_CFG_14_ADDR)
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_14_INM(m)            \
                in_dword_masked(HWIO_SSC_TLMM_LITE_GPIO_CFG_14_ADDR, m)
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_14_OUT(v)            \
                out_dword(HWIO_SSC_TLMM_LITE_GPIO_CFG_14_ADDR,v)
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_14_OUTM(m,v) \
                out_dword_masked_ns(HWIO_SSC_TLMM_LITE_GPIO_CFG_14_ADDR,m,v,HWIO_SSC_TLMM_LITE_GPIO_CFG_14_IN)
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_14_GPIO_MISC_CFG_BMSK               0xf800
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_14_GPIO_MISC_CFG_SHFT                   11
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_14_GPIO_HIHYS_CTL_BMSK               0x400
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_14_GPIO_HIHYS_CTL_SHFT                  10
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_14_GPIO_OE_BMSK                      0x200
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_14_GPIO_OE_SHFT                          9
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_14_GPIO_HDRIVE_BMSK                  0x1c0
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_14_GPIO_HDRIVE_SHFT                      6
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_14_GPIO_FUNC_SEL_BMSK                 0x3c
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_14_GPIO_FUNC_SEL_SHFT                    2
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_14_GPIO_PULL_BMSK                      0x3
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_14_GPIO_PULL_SHFT                        0

#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_14_ADDR                      (SSC_TLMM_CSR_REG_BASE            + 0xe004)
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_14_RMSK                             0x3
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_14_IN                    \
                in_dword(HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_14_ADDR)
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_14_INM(m)            \
                in_dword_masked(HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_14_ADDR, m)
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_14_OUT(v)            \
                out_dword(HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_14_ADDR,v)
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_14_OUTM(m,v) \
                out_dword_masked_ns(HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_14_ADDR,m,v,HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_14_IN)
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_14_GPIO_OUT_BMSK                    0x2
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_14_GPIO_OUT_SHFT                      1
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_14_GPIO_IN_BMSK                     0x1
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_14_GPIO_IN_SHFT                       0

#define HWIO_SSC_TLMM_LITE_GPIO_CFG_15_ADDR                         (SSC_TLMM_CSR_REG_BASE            + 0xf000)
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_15_RMSK                             0xffff
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_15_IN                    \
                in_dword(HWIO_SSC_TLMM_LITE_GPIO_CFG_15_ADDR)
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_15_INM(m)            \
                in_dword_masked(HWIO_SSC_TLMM_LITE_GPIO_CFG_15_ADDR, m)
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_15_OUT(v)            \
                out_dword(HWIO_SSC_TLMM_LITE_GPIO_CFG_15_ADDR,v)
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_15_OUTM(m,v) \
                out_dword_masked_ns(HWIO_SSC_TLMM_LITE_GPIO_CFG_15_ADDR,m,v,HWIO_SSC_TLMM_LITE_GPIO_CFG_15_IN)
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_15_GPIO_MISC_CFG_BMSK               0xf800
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_15_GPIO_MISC_CFG_SHFT                   11
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_15_GPIO_HIHYS_CTL_BMSK               0x400
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_15_GPIO_HIHYS_CTL_SHFT                  10
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_15_GPIO_OE_BMSK                      0x200
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_15_GPIO_OE_SHFT                          9
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_15_GPIO_HDRIVE_BMSK                  0x1c0
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_15_GPIO_HDRIVE_SHFT                      6
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_15_GPIO_FUNC_SEL_BMSK                 0x3c
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_15_GPIO_FUNC_SEL_SHFT                    2
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_15_GPIO_PULL_BMSK                      0x3
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_15_GPIO_PULL_SHFT                        0

#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_15_ADDR                      (SSC_TLMM_CSR_REG_BASE            + 0xf004)
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_15_RMSK                             0x3
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_15_IN                    \
                in_dword(HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_15_ADDR)
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_15_INM(m)            \
                in_dword_masked(HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_15_ADDR, m)
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_15_OUT(v)            \
                out_dword(HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_15_ADDR,v)
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_15_OUTM(m,v) \
                out_dword_masked_ns(HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_15_ADDR,m,v,HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_15_IN)
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_15_GPIO_OUT_BMSK                    0x2
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_15_GPIO_OUT_SHFT                      1
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_15_GPIO_IN_BMSK                     0x1
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_15_GPIO_IN_SHFT                       0


#endif /* __UIMATHERTONHWIOMACROS_H__ */
