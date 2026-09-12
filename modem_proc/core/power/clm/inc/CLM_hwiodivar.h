#ifndef __CLM_HWIODIVAR_H__
#define __CLM_HWIODIVAR_H__
/*
===========================================================================
*/
/**
    @file CLM_hwiodivar.h
    @brief Auto-generated HWIO interface include file.

    Reference chip release:
        SM6225 (Divar) [divar_v1.0_p3q3r44.1_MTO]
 
    This file contains HWIO register definitions for the following modules:
        MSS_QDSP6V671SS_PUB

    'Include' filters applied: MSS_QDSP6SS_CGC_OVERRIDE[MSS_QDSP6V671SS_PUB] 

    Generation parameters: 
    { 'filename': 'CLM_hwiodivar.h',
      'header': '#define CLK_Q6_NAME "clk_q6"',
      'module-filter-include': { 'MSS_QDSP6V671SS_PUB': [ 'MSS_QDSP6SS_CGC_OVERRIDE']},
      'modules': ['MSS_QDSP6V671SS_PUB']}
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

    $Header: //components/rel/core.mpss/10.0/power/clm/inc/CLM_hwiodivar.h#2 $
    $DateTime: 2021/07/15 04:21:31 $
    $Author: pwbldsvc $

    ===========================================================================
*/

#define CLK_Q6_NAME "clk_q6"

/*----------------------------------------------------------------------------
 * MODULE: MSS_QDSP6V671SS_PUB
 *--------------------------------------------------------------------------*/

#define MSS_QDSP6V671SS_PUB_REG_BASE                                              (MSS_TOP_BASE            + 0x00080000)
#define MSS_QDSP6V671SS_PUB_REG_BASE_SIZE                                         0x10000
#define MSS_QDSP6V671SS_PUB_REG_BASE_USED                                         0x204c

#define HWIO_MSS_QDSP6SS_CGC_OVERRIDE_ADDR                                        (MSS_QDSP6V671SS_PUB_REG_BASE            + 0xb0)
#define HWIO_MSS_QDSP6SS_CGC_OVERRIDE_RMSK                                            0xffff
#define HWIO_MSS_QDSP6SS_CGC_OVERRIDE_IN                    \
                in_dword(HWIO_MSS_QDSP6SS_CGC_OVERRIDE_ADDR)
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


#endif /* __CLM_HWIODIVAR_H__ */