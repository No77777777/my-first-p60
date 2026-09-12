#ifndef __UIMKAMORTAHWIOMACROS_H__
#define __UIMKAMORTAHWIOMACROS_H__
/*
===========================================================================
*/
/**
  @file uimkamortahwiomacros.h
  @brief Auto-generated HWIO interface include file.

  Reference chip release:
    (Kamorta) [kamorta_v1.0_p3q2r42]

  This file contains HWIO register definitions for the following modules:
    MSS_CC_MSS_CC_REG
    MSS_UIM0_UART_DM
    MSS_UIM1_UART_DM
    MSS_UIM2_UART_DM
    MSS_UIM3_UART_DM
    MSS_PERPH_MSS_PERPH
    SSC_TLMM_CSR


  Generation parameters: 
  { 'filename': 'uimkamortahwiomacros.h',
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

  $Header: //components/rel/uim.mpss/6.1.0/uimdrv/src/uimkamortahwiomacros.h#2 $
  $DateTime: 2019/12/15 22:59:33 $
  $Author: pwbldsvc $

  ===========================================================================
*/

#include "msmhwiobase.h"

/*----------------------------------------------------------------------------
 * MODULE: MSS_CC_MSS_CC_REG
 *--------------------------------------------------------------------------*/

#define MSS_CC_MSS_CC_REG_REG_BASE                                                             (MSS_TOP_BASE      + 0x00180000)
#define MSS_CC_MSS_CC_REG_REG_BASE_SIZE                                                        0x176d
#define MSS_CC_MSS_CC_REG_REG_BASE_USED                                                        0x1420

#define HWIO_MSS_CC_MPLL3_MODE_ADDR                                                            (MSS_CC_MSS_CC_REG_REG_BASE      + 0x000009b0)
#define HWIO_MSS_CC_MPLL3_MODE_RMSK                                                            0xffffffff
#define HWIO_MSS_CC_MPLL3_MODE_IN          \
        in_dword(HWIO_MSS_CC_MPLL3_MODE_ADDR)
#define HWIO_MSS_CC_MPLL3_MODE_INM(m)      \
        in_dword_masked(HWIO_MSS_CC_MPLL3_MODE_ADDR, m)
#define HWIO_MSS_CC_MPLL3_MODE_OUT(v)      \
        out_dword(HWIO_MSS_CC_MPLL3_MODE_ADDR,v)
#define HWIO_MSS_CC_MPLL3_MODE_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_CC_MPLL3_MODE_ADDR,m,v,HWIO_MSS_CC_MPLL3_MODE_IN)
#define HWIO_MSS_CC_MPLL3_MODE_PLL_LOCK_DET_BMSK                                               0x80000000
#define HWIO_MSS_CC_MPLL3_MODE_PLL_LOCK_DET_SHFT                                                     0x1f
#define HWIO_MSS_CC_MPLL3_MODE_PLL_ACTIVE_FLAG_BMSK                                            0x40000000
#define HWIO_MSS_CC_MPLL3_MODE_PLL_ACTIVE_FLAG_SHFT                                                  0x1e
#define HWIO_MSS_CC_MPLL3_MODE_PLL_ACK_LATCH_BMSK                                              0x20000000
#define HWIO_MSS_CC_MPLL3_MODE_PLL_ACK_LATCH_SHFT                                                    0x1d
#define HWIO_MSS_CC_MPLL3_MODE_RESERVE_BITS28_24_BMSK                                          0x1f000000
#define HWIO_MSS_CC_MPLL3_MODE_RESERVE_BITS28_24_SHFT                                                0x18
#define HWIO_MSS_CC_MPLL3_MODE_PLL_HW_UPDATE_LOGIC_BYPASS_BMSK                                   0x800000
#define HWIO_MSS_CC_MPLL3_MODE_PLL_HW_UPDATE_LOGIC_BYPASS_SHFT                                       0x17
#define HWIO_MSS_CC_MPLL3_MODE_PLL_UPDATE_BMSK                                                   0x400000
#define HWIO_MSS_CC_MPLL3_MODE_PLL_UPDATE_SHFT                                                       0x16
#define HWIO_MSS_CC_MPLL3_MODE_PLL_VOTE_FSM_RESET_BMSK                                           0x200000
#define HWIO_MSS_CC_MPLL3_MODE_PLL_VOTE_FSM_RESET_SHFT                                               0x15
#define HWIO_MSS_CC_MPLL3_MODE_PLL_VOTE_FSM_ENA_BMSK                                             0x100000
#define HWIO_MSS_CC_MPLL3_MODE_PLL_VOTE_FSM_ENA_SHFT                                                 0x14
#define HWIO_MSS_CC_MPLL3_MODE_PLL_BIAS_COUNT_BMSK                                                0xfc000
#define HWIO_MSS_CC_MPLL3_MODE_PLL_BIAS_COUNT_SHFT                                                    0xe
#define HWIO_MSS_CC_MPLL3_MODE_PLL_LOCK_COUNT_BMSK                                                 0x3f00
#define HWIO_MSS_CC_MPLL3_MODE_PLL_LOCK_COUNT_SHFT                                                    0x8
#define HWIO_MSS_CC_MPLL3_MODE_RESERVE_BITS7_4_BMSK                                                  0xf0
#define HWIO_MSS_CC_MPLL3_MODE_RESERVE_BITS7_4_SHFT                                                   0x4
#define HWIO_MSS_CC_MPLL3_MODE_PLL_PLLTEST_BMSK                                                       0x8
#define HWIO_MSS_CC_MPLL3_MODE_PLL_PLLTEST_SHFT                                                       0x3
#define HWIO_MSS_CC_MPLL3_MODE_PLL_RESET_N_BMSK                                                       0x4
#define HWIO_MSS_CC_MPLL3_MODE_PLL_RESET_N_SHFT                                                       0x2
#define HWIO_MSS_CC_MPLL3_MODE_PLL_BYPASSNL_BMSK                                                      0x2
#define HWIO_MSS_CC_MPLL3_MODE_PLL_BYPASSNL_SHFT                                                      0x1
#define HWIO_MSS_CC_MPLL3_MODE_PLL_OUTCTRL_BMSK                                                       0x1
#define HWIO_MSS_CC_MPLL3_MODE_PLL_OUTCTRL_SHFT                                                       0x0

#define HWIO_MSS_CC_MPLL3_L_VAL_ADDR                                                           (MSS_CC_MSS_CC_REG_REG_BASE      + 0x000009b4)
#define HWIO_MSS_CC_MPLL3_L_VAL_RMSK                                                               0xffff
#define HWIO_MSS_CC_MPLL3_L_VAL_IN          \
        in_dword(HWIO_MSS_CC_MPLL3_L_VAL_ADDR)
#define HWIO_MSS_CC_MPLL3_L_VAL_INM(m)      \
        in_dword_masked(HWIO_MSS_CC_MPLL3_L_VAL_ADDR, m)
#define HWIO_MSS_CC_MPLL3_L_VAL_OUT(v)      \
        out_dword(HWIO_MSS_CC_MPLL3_L_VAL_ADDR,v)
#define HWIO_MSS_CC_MPLL3_L_VAL_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_CC_MPLL3_L_VAL_ADDR,m,v,HWIO_MSS_CC_MPLL3_L_VAL_IN)
#define HWIO_MSS_CC_MPLL3_L_VAL_PLL_L_BMSK                                                         0xffff
#define HWIO_MSS_CC_MPLL3_L_VAL_PLL_L_SHFT                                                            0x0

#define HWIO_MSS_CC_MPLL3_ALPHA_VAL_ADDR                                                       (MSS_CC_MSS_CC_REG_REG_BASE      + 0x000009b8)
#define HWIO_MSS_CC_MPLL3_ALPHA_VAL_RMSK                                                       0xffffffff
#define HWIO_MSS_CC_MPLL3_ALPHA_VAL_IN          \
        in_dword(HWIO_MSS_CC_MPLL3_ALPHA_VAL_ADDR)
#define HWIO_MSS_CC_MPLL3_ALPHA_VAL_INM(m)      \
        in_dword_masked(HWIO_MSS_CC_MPLL3_ALPHA_VAL_ADDR, m)
#define HWIO_MSS_CC_MPLL3_ALPHA_VAL_OUT(v)      \
        out_dword(HWIO_MSS_CC_MPLL3_ALPHA_VAL_ADDR,v)
#define HWIO_MSS_CC_MPLL3_ALPHA_VAL_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_CC_MPLL3_ALPHA_VAL_ADDR,m,v,HWIO_MSS_CC_MPLL3_ALPHA_VAL_IN)
#define HWIO_MSS_CC_MPLL3_ALPHA_VAL_PLL_ALPHA_31_0_BMSK                                        0xffffffff
#define HWIO_MSS_CC_MPLL3_ALPHA_VAL_PLL_ALPHA_31_0_SHFT                                               0x0

#define HWIO_MSS_CC_MPLL3_ALPHA_VAL_U_ADDR                                                     (MSS_CC_MSS_CC_REG_REG_BASE      + 0x000009bc)
#define HWIO_MSS_CC_MPLL3_ALPHA_VAL_U_RMSK                                                           0xff
#define HWIO_MSS_CC_MPLL3_ALPHA_VAL_U_IN          \
        in_dword(HWIO_MSS_CC_MPLL3_ALPHA_VAL_U_ADDR)
#define HWIO_MSS_CC_MPLL3_ALPHA_VAL_U_INM(m)      \
        in_dword_masked(HWIO_MSS_CC_MPLL3_ALPHA_VAL_U_ADDR, m)
#define HWIO_MSS_CC_MPLL3_ALPHA_VAL_U_OUT(v)      \
        out_dword(HWIO_MSS_CC_MPLL3_ALPHA_VAL_U_ADDR,v)
#define HWIO_MSS_CC_MPLL3_ALPHA_VAL_U_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_CC_MPLL3_ALPHA_VAL_U_ADDR,m,v,HWIO_MSS_CC_MPLL3_ALPHA_VAL_U_IN)
#define HWIO_MSS_CC_MPLL3_ALPHA_VAL_U_PLL_ALPHA_39_32_BMSK                                           0xff
#define HWIO_MSS_CC_MPLL3_ALPHA_VAL_U_PLL_ALPHA_39_32_SHFT                                            0x0

#define HWIO_MSS_CC_MPLL3_TEST_CTL_ADDR                                                        (MSS_CC_MSS_CC_REG_REG_BASE      + 0x000009c0)
#define HWIO_MSS_CC_MPLL3_TEST_CTL_RMSK                                                        0xffffffff
#define HWIO_MSS_CC_MPLL3_TEST_CTL_IN          \
        in_dword(HWIO_MSS_CC_MPLL3_TEST_CTL_ADDR)
#define HWIO_MSS_CC_MPLL3_TEST_CTL_INM(m)      \
        in_dword_masked(HWIO_MSS_CC_MPLL3_TEST_CTL_ADDR, m)
#define HWIO_MSS_CC_MPLL3_TEST_CTL_OUT(v)      \
        out_dword(HWIO_MSS_CC_MPLL3_TEST_CTL_ADDR,v)
#define HWIO_MSS_CC_MPLL3_TEST_CTL_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_CC_MPLL3_TEST_CTL_ADDR,m,v,HWIO_MSS_CC_MPLL3_TEST_CTL_IN)
#define HWIO_MSS_CC_MPLL3_TEST_CTL_BIAS_GEN_TRIM_BMSK                                          0xe0000000
#define HWIO_MSS_CC_MPLL3_TEST_CTL_BIAS_GEN_TRIM_SHFT                                                0x1d
#define HWIO_MSS_CC_MPLL3_TEST_CTL_PROCESS_CALB_BMSK                                           0x1c000000
#define HWIO_MSS_CC_MPLL3_TEST_CTL_PROCESS_CALB_SHFT                                                 0x1a
#define HWIO_MSS_CC_MPLL3_TEST_CTL_OVERRIDE_PROCESS_CALB_BMSK                                   0x2000000
#define HWIO_MSS_CC_MPLL3_TEST_CTL_OVERRIDE_PROCESS_CALB_SHFT                                        0x19
#define HWIO_MSS_CC_MPLL3_TEST_CTL_FINE_FCW_BMSK                                                0x1f00000
#define HWIO_MSS_CC_MPLL3_TEST_CTL_FINE_FCW_SHFT                                                     0x14
#define HWIO_MSS_CC_MPLL3_TEST_CTL_OVERRIDE_FINE_FCW_BMSK                                         0x80000
#define HWIO_MSS_CC_MPLL3_TEST_CTL_OVERRIDE_FINE_FCW_SHFT                                            0x13
#define HWIO_MSS_CC_MPLL3_TEST_CTL_COARSE_FCW_BMSK                                                0x7e000
#define HWIO_MSS_CC_MPLL3_TEST_CTL_COARSE_FCW_SHFT                                                    0xd
#define HWIO_MSS_CC_MPLL3_TEST_CTL_OVERRIDE_COARSE_BMSK                                            0x1000
#define HWIO_MSS_CC_MPLL3_TEST_CTL_OVERRIDE_COARSE_SHFT                                               0xc
#define HWIO_MSS_CC_MPLL3_TEST_CTL_DISABLE_LFSR_BMSK                                                0x800
#define HWIO_MSS_CC_MPLL3_TEST_CTL_DISABLE_LFSR_SHFT                                                  0xb
#define HWIO_MSS_CC_MPLL3_TEST_CTL_DTEST_SEL_BMSK                                                   0x700
#define HWIO_MSS_CC_MPLL3_TEST_CTL_DTEST_SEL_SHFT                                                     0x8
#define HWIO_MSS_CC_MPLL3_TEST_CTL_DTEST_EN_BMSK                                                     0x80
#define HWIO_MSS_CC_MPLL3_TEST_CTL_DTEST_EN_SHFT                                                      0x7
#define HWIO_MSS_CC_MPLL3_TEST_CTL_BYP_TESTAMP_BMSK                                                  0x40
#define HWIO_MSS_CC_MPLL3_TEST_CTL_BYP_TESTAMP_SHFT                                                   0x6
#define HWIO_MSS_CC_MPLL3_TEST_CTL_ATEST1_SEL_BMSK                                                   0x30
#define HWIO_MSS_CC_MPLL3_TEST_CTL_ATEST1_SEL_SHFT                                                    0x4
#define HWIO_MSS_CC_MPLL3_TEST_CTL_ATEST0_SEL_BMSK                                                    0xc
#define HWIO_MSS_CC_MPLL3_TEST_CTL_ATEST0_SEL_SHFT                                                    0x2
#define HWIO_MSS_CC_MPLL3_TEST_CTL_ATEST1_EN_BMSK                                                     0x2
#define HWIO_MSS_CC_MPLL3_TEST_CTL_ATEST1_EN_SHFT                                                     0x1
#define HWIO_MSS_CC_MPLL3_TEST_CTL_ATEST0_EN_BMSK                                                     0x1
#define HWIO_MSS_CC_MPLL3_TEST_CTL_ATEST0_EN_SHFT                                                     0x0

#define HWIO_MSS_CC_MPLL3_TEST_CTL_U_ADDR                                                      (MSS_CC_MSS_CC_REG_REG_BASE      + 0x000009c4)
#define HWIO_MSS_CC_MPLL3_TEST_CTL_U_RMSK                                                      0xfffffeff
#define HWIO_MSS_CC_MPLL3_TEST_CTL_U_IN          \
        in_dword(HWIO_MSS_CC_MPLL3_TEST_CTL_U_ADDR)
#define HWIO_MSS_CC_MPLL3_TEST_CTL_U_INM(m)      \
        in_dword_masked(HWIO_MSS_CC_MPLL3_TEST_CTL_U_ADDR, m)
#define HWIO_MSS_CC_MPLL3_TEST_CTL_U_OUT(v)      \
        out_dword(HWIO_MSS_CC_MPLL3_TEST_CTL_U_ADDR,v)
#define HWIO_MSS_CC_MPLL3_TEST_CTL_U_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_CC_MPLL3_TEST_CTL_U_ADDR,m,v,HWIO_MSS_CC_MPLL3_TEST_CTL_U_IN)
#define HWIO_MSS_CC_MPLL3_TEST_CTL_U_RESERVE_BITS31_22_BMSK                                    0xffc00000
#define HWIO_MSS_CC_MPLL3_TEST_CTL_U_RESERVE_BITS31_22_SHFT                                          0x16
#define HWIO_MSS_CC_MPLL3_TEST_CTL_U_GLITCH_DETECT_COUNT_BMSK                                    0x300000
#define HWIO_MSS_CC_MPLL3_TEST_CTL_U_GLITCH_DETECT_COUNT_SHFT                                        0x14
#define HWIO_MSS_CC_MPLL3_TEST_CTL_U_DISABLE_GLITCH_BMSK                                          0x80000
#define HWIO_MSS_CC_MPLL3_TEST_CTL_U_DISABLE_GLITCH_SHFT                                             0x13
#define HWIO_MSS_CC_MPLL3_TEST_CTL_U_DTEST_MODE_SEL_2_BMSK                                        0x60000
#define HWIO_MSS_CC_MPLL3_TEST_CTL_U_DTEST_MODE_SEL_2_SHFT                                           0x11
#define HWIO_MSS_CC_MPLL3_TEST_CTL_U_DITHER_SEL_BMSK                                              0x18000
#define HWIO_MSS_CC_MPLL3_TEST_CTL_U_DITHER_SEL_SHFT                                                  0xf
#define HWIO_MSS_CC_MPLL3_TEST_CTL_U_PROCESS_CAL_SEL_BMSK                                          0x4000
#define HWIO_MSS_CC_MPLL3_TEST_CTL_U_PROCESS_CAL_SEL_SHFT                                             0xe
#define HWIO_MSS_CC_MPLL3_TEST_CTL_U_OVERRIDE_FINE_FCW_MSB_BMSK                                    0x2000
#define HWIO_MSS_CC_MPLL3_TEST_CTL_U_OVERRIDE_FINE_FCW_MSB_SHFT                                       0xd
#define HWIO_MSS_CC_MPLL3_TEST_CTL_U_DTEST_MODE_SEL_BMSK                                           0x1800
#define HWIO_MSS_CC_MPLL3_TEST_CTL_U_DTEST_MODE_SEL_SHFT                                              0xb
#define HWIO_MSS_CC_MPLL3_TEST_CTL_U_NMO_OSC_SEL_BMSK                                               0x600
#define HWIO_MSS_CC_MPLL3_TEST_CTL_U_NMO_OSC_SEL_SHFT                                                 0x9
#define HWIO_MSS_CC_MPLL3_TEST_CTL_U_NOISE_MAG_BMSK                                                  0xe0
#define HWIO_MSS_CC_MPLL3_TEST_CTL_U_NOISE_MAG_SHFT                                                   0x5
#define HWIO_MSS_CC_MPLL3_TEST_CTL_U_NOISE_GEN_BMSK                                                  0x10
#define HWIO_MSS_CC_MPLL3_TEST_CTL_U_NOISE_GEN_SHFT                                                   0x4
#define HWIO_MSS_CC_MPLL3_TEST_CTL_U_OSC_BIAS_GND_BMSK                                                0x8
#define HWIO_MSS_CC_MPLL3_TEST_CTL_U_OSC_BIAS_GND_SHFT                                                0x3
#define HWIO_MSS_CC_MPLL3_TEST_CTL_U_PLL_TEST_OUT_SEL_BMSK                                            0x6
#define HWIO_MSS_CC_MPLL3_TEST_CTL_U_PLL_TEST_OUT_SEL_SHFT                                            0x1
#define HWIO_MSS_CC_MPLL3_TEST_CTL_U_CAL_CODE_UPDATE_BMSK                                             0x1
#define HWIO_MSS_CC_MPLL3_TEST_CTL_U_CAL_CODE_UPDATE_SHFT                                             0x0

#define HWIO_MSS_CC_MPLL3_USER_CTL_ADDR                                                        (MSS_CC_MSS_CC_REG_REG_BASE      + 0x000009c8)
#define HWIO_MSS_CC_MPLL3_USER_CTL_RMSK                                                        0xffffffff
#define HWIO_MSS_CC_MPLL3_USER_CTL_IN          \
        in_dword(HWIO_MSS_CC_MPLL3_USER_CTL_ADDR)
#define HWIO_MSS_CC_MPLL3_USER_CTL_INM(m)      \
        in_dword_masked(HWIO_MSS_CC_MPLL3_USER_CTL_ADDR, m)
#define HWIO_MSS_CC_MPLL3_USER_CTL_OUT(v)      \
        out_dword(HWIO_MSS_CC_MPLL3_USER_CTL_ADDR,v)
#define HWIO_MSS_CC_MPLL3_USER_CTL_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_CC_MPLL3_USER_CTL_ADDR,m,v,HWIO_MSS_CC_MPLL3_USER_CTL_IN)
#define HWIO_MSS_CC_MPLL3_USER_CTL_RESERVE_BITS31_28_BMSK                                      0xf0000000
#define HWIO_MSS_CC_MPLL3_USER_CTL_RESERVE_BITS31_28_SHFT                                            0x1c
#define HWIO_MSS_CC_MPLL3_USER_CTL_SSC_MODE_CONTROL_BMSK                                        0x8000000
#define HWIO_MSS_CC_MPLL3_USER_CTL_SSC_MODE_CONTROL_SHFT                                             0x1b
#define HWIO_MSS_CC_MPLL3_USER_CTL_RESERVE_BITS26_25_BMSK                                       0x6000000
#define HWIO_MSS_CC_MPLL3_USER_CTL_RESERVE_BITS26_25_SHFT                                            0x19
#define HWIO_MSS_CC_MPLL3_USER_CTL_ALPHA_EN_BMSK                                                0x1000000
#define HWIO_MSS_CC_MPLL3_USER_CTL_ALPHA_EN_SHFT                                                     0x18
#define HWIO_MSS_CC_MPLL3_USER_CTL_RESERVE_BITS23_22_BMSK                                        0xc00000
#define HWIO_MSS_CC_MPLL3_USER_CTL_RESERVE_BITS23_22_SHFT                                            0x16
#define HWIO_MSS_CC_MPLL3_USER_CTL_VCO_SEL_BMSK                                                  0x300000
#define HWIO_MSS_CC_MPLL3_USER_CTL_VCO_SEL_SHFT                                                      0x14
#define HWIO_MSS_CC_MPLL3_USER_CTL_RESERVE_BITS19_18_BMSK                                         0xc0000
#define HWIO_MSS_CC_MPLL3_USER_CTL_RESERVE_BITS19_18_SHFT                                            0x12
#define HWIO_MSS_CC_MPLL3_USER_CTL_AUX_POST_DIV_RATIO_BMSK                                        0x38000
#define HWIO_MSS_CC_MPLL3_USER_CTL_AUX_POST_DIV_RATIO_SHFT                                            0xf
#define HWIO_MSS_CC_MPLL3_USER_CTL_PRE_DIV_RATIO_BMSK                                              0x7000
#define HWIO_MSS_CC_MPLL3_USER_CTL_PRE_DIV_RATIO_SHFT                                                 0xc
#define HWIO_MSS_CC_MPLL3_USER_CTL_POST_DIV_RATIO_BMSK                                              0xf00
#define HWIO_MSS_CC_MPLL3_USER_CTL_POST_DIV_RATIO_SHFT                                                0x8
#define HWIO_MSS_CC_MPLL3_USER_CTL_OUTPUT_INV_BMSK                                                   0x80
#define HWIO_MSS_CC_MPLL3_USER_CTL_OUTPUT_INV_SHFT                                                    0x7
#define HWIO_MSS_CC_MPLL3_USER_CTL_RESERVE_BITS6_5_BMSK                                              0x60
#define HWIO_MSS_CC_MPLL3_USER_CTL_RESERVE_BITS6_5_SHFT                                               0x5
#define HWIO_MSS_CC_MPLL3_USER_CTL_PLLOUT_LV_TEST_BMSK                                               0x10
#define HWIO_MSS_CC_MPLL3_USER_CTL_PLLOUT_LV_TEST_SHFT                                                0x4
#define HWIO_MSS_CC_MPLL3_USER_CTL_PLLOUT_LV_EARLY_BMSK                                               0x8
#define HWIO_MSS_CC_MPLL3_USER_CTL_PLLOUT_LV_EARLY_SHFT                                               0x3
#define HWIO_MSS_CC_MPLL3_USER_CTL_PLLOUT_LV_AUX2_BMSK                                                0x4
#define HWIO_MSS_CC_MPLL3_USER_CTL_PLLOUT_LV_AUX2_SHFT                                                0x2
#define HWIO_MSS_CC_MPLL3_USER_CTL_PLLOUT_LV_AUX_BMSK                                                 0x2
#define HWIO_MSS_CC_MPLL3_USER_CTL_PLLOUT_LV_AUX_SHFT                                                 0x1
#define HWIO_MSS_CC_MPLL3_USER_CTL_PLLOUT_LV_MAIN_BMSK                                                0x1
#define HWIO_MSS_CC_MPLL3_USER_CTL_PLLOUT_LV_MAIN_SHFT                                                0x0

#define HWIO_MSS_CC_MPLL3_USER_CTL_U_ADDR                                                      (MSS_CC_MSS_CC_REG_REG_BASE      + 0x000009cc)
#define HWIO_MSS_CC_MPLL3_USER_CTL_U_RMSK                                                      0xffffffff
#define HWIO_MSS_CC_MPLL3_USER_CTL_U_IN          \
        in_dword(HWIO_MSS_CC_MPLL3_USER_CTL_U_ADDR)
#define HWIO_MSS_CC_MPLL3_USER_CTL_U_INM(m)      \
        in_dword_masked(HWIO_MSS_CC_MPLL3_USER_CTL_U_ADDR, m)
#define HWIO_MSS_CC_MPLL3_USER_CTL_U_OUT(v)      \
        out_dword(HWIO_MSS_CC_MPLL3_USER_CTL_U_ADDR,v)
#define HWIO_MSS_CC_MPLL3_USER_CTL_U_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_CC_MPLL3_USER_CTL_U_ADDR,m,v,HWIO_MSS_CC_MPLL3_USER_CTL_U_IN)
#define HWIO_MSS_CC_MPLL3_USER_CTL_U_RESERVE_BITS31_12_BMSK                                    0xfffff000
#define HWIO_MSS_CC_MPLL3_USER_CTL_U_RESERVE_BITS31_12_SHFT                                           0xc
#define HWIO_MSS_CC_MPLL3_USER_CTL_U_LATCH_INTERFACE_BYPASS_BMSK                                    0x800
#define HWIO_MSS_CC_MPLL3_USER_CTL_U_LATCH_INTERFACE_BYPASS_SHFT                                      0xb
#define HWIO_MSS_CC_MPLL3_USER_CTL_U_STATUS_REGISTER_BMSK                                           0x700
#define HWIO_MSS_CC_MPLL3_USER_CTL_U_STATUS_REGISTER_SHFT                                             0x8
#define HWIO_MSS_CC_MPLL3_USER_CTL_U_DSM_BMSK                                                        0x80
#define HWIO_MSS_CC_MPLL3_USER_CTL_U_DSM_SHFT                                                         0x7
#define HWIO_MSS_CC_MPLL3_USER_CTL_U_WRITE_STATE_BMSK                                                0x40
#define HWIO_MSS_CC_MPLL3_USER_CTL_U_WRITE_STATE_SHFT                                                 0x6
#define HWIO_MSS_CC_MPLL3_USER_CTL_U_TARGET_CTL_BMSK                                                 0x38
#define HWIO_MSS_CC_MPLL3_USER_CTL_U_TARGET_CTL_SHFT                                                  0x3
#define HWIO_MSS_CC_MPLL3_USER_CTL_U_LOCK_DET_BMSK                                                    0x4
#define HWIO_MSS_CC_MPLL3_USER_CTL_U_LOCK_DET_SHFT                                                    0x2
#define HWIO_MSS_CC_MPLL3_USER_CTL_U_FREEZE_PLL_BMSK                                                  0x2
#define HWIO_MSS_CC_MPLL3_USER_CTL_U_FREEZE_PLL_SHFT                                                  0x1
#define HWIO_MSS_CC_MPLL3_USER_CTL_U_TOGGLE_DET_BMSK                                                  0x1
#define HWIO_MSS_CC_MPLL3_USER_CTL_U_TOGGLE_DET_SHFT                                                  0x0

#define HWIO_MSS_CC_MPLL3_CONFIG_CTL_ADDR                                                      (MSS_CC_MSS_CC_REG_REG_BASE      + 0x000009d0)
#define HWIO_MSS_CC_MPLL3_CONFIG_CTL_RMSK                                                      0xffffffff
#define HWIO_MSS_CC_MPLL3_CONFIG_CTL_IN          \
        in_dword(HWIO_MSS_CC_MPLL3_CONFIG_CTL_ADDR)
#define HWIO_MSS_CC_MPLL3_CONFIG_CTL_INM(m)      \
        in_dword_masked(HWIO_MSS_CC_MPLL3_CONFIG_CTL_ADDR, m)
#define HWIO_MSS_CC_MPLL3_CONFIG_CTL_OUT(v)      \
        out_dword(HWIO_MSS_CC_MPLL3_CONFIG_CTL_ADDR,v)
#define HWIO_MSS_CC_MPLL3_CONFIG_CTL_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_CC_MPLL3_CONFIG_CTL_ADDR,m,v,HWIO_MSS_CC_MPLL3_CONFIG_CTL_IN)
#define HWIO_MSS_CC_MPLL3_CONFIG_CTL_SINGLE_DMET_MODE_ENABLE_BMSK                              0x80000000
#define HWIO_MSS_CC_MPLL3_CONFIG_CTL_SINGLE_DMET_MODE_ENABLE_SHFT                                    0x1f
#define HWIO_MSS_CC_MPLL3_CONFIG_CTL_DMET_WINDOW_ENABLE_BMSK                                   0x40000000
#define HWIO_MSS_CC_MPLL3_CONFIG_CTL_DMET_WINDOW_ENABLE_SHFT                                         0x1e
#define HWIO_MSS_CC_MPLL3_CONFIG_CTL_TOGGLE_DET_SAMPLE_INTER_BMSK                              0x3c000000
#define HWIO_MSS_CC_MPLL3_CONFIG_CTL_TOGGLE_DET_SAMPLE_INTER_SHFT                                    0x1a
#define HWIO_MSS_CC_MPLL3_CONFIG_CTL_TOGGLE_DET_THRESHOLD_BMSK                                  0x3800000
#define HWIO_MSS_CC_MPLL3_CONFIG_CTL_TOGGLE_DET_THRESHOLD_SHFT                                       0x17
#define HWIO_MSS_CC_MPLL3_CONFIG_CTL_TOGGLE_DET_SAMPLE_BMSK                                      0x700000
#define HWIO_MSS_CC_MPLL3_CONFIG_CTL_TOGGLE_DET_SAMPLE_SHFT                                          0x14
#define HWIO_MSS_CC_MPLL3_CONFIG_CTL_LOCK_DET_THRESHOLD_BMSK                                      0xff000
#define HWIO_MSS_CC_MPLL3_CONFIG_CTL_LOCK_DET_THRESHOLD_SHFT                                          0xc
#define HWIO_MSS_CC_MPLL3_CONFIG_CTL_LOCK_DET_SAMPLE_SIZE_BMSK                                      0xf00
#define HWIO_MSS_CC_MPLL3_CONFIG_CTL_LOCK_DET_SAMPLE_SIZE_SHFT                                        0x8
#define HWIO_MSS_CC_MPLL3_CONFIG_CTL_MIN_GLITCH_THRESHOLD_BMSK                                       0xc0
#define HWIO_MSS_CC_MPLL3_CONFIG_CTL_MIN_GLITCH_THRESHOLD_SHFT                                        0x6
#define HWIO_MSS_CC_MPLL3_CONFIG_CTL_REF_CYCLE_BMSK                                                  0x30
#define HWIO_MSS_CC_MPLL3_CONFIG_CTL_REF_CYCLE_SHFT                                                   0x4
#define HWIO_MSS_CC_MPLL3_CONFIG_CTL_KFN_BMSK                                                         0xf
#define HWIO_MSS_CC_MPLL3_CONFIG_CTL_KFN_SHFT                                                         0x0

#define HWIO_MSS_CC_MPLL3_STATUS_ADDR                                                          (MSS_CC_MSS_CC_REG_REG_BASE      + 0x000009d4)
#define HWIO_MSS_CC_MPLL3_STATUS_RMSK                                                          0xffffffff
#define HWIO_MSS_CC_MPLL3_STATUS_IN          \
        in_dword(HWIO_MSS_CC_MPLL3_STATUS_ADDR)
#define HWIO_MSS_CC_MPLL3_STATUS_INM(m)      \
        in_dword_masked(HWIO_MSS_CC_MPLL3_STATUS_ADDR, m)
#define HWIO_MSS_CC_MPLL3_STATUS_STATUS_31_0_BMSK                                              0xffffffff
#define HWIO_MSS_CC_MPLL3_STATUS_STATUS_31_0_SHFT                                                     0x0

#define HWIO_MSS_CC_MPLL3_FREQ_CTL_ADDR                                                        (MSS_CC_MSS_CC_REG_REG_BASE      + 0x000009d8)
#define HWIO_MSS_CC_MPLL3_FREQ_CTL_RMSK                                                        0xffffffff
#define HWIO_MSS_CC_MPLL3_FREQ_CTL_IN          \
        in_dword(HWIO_MSS_CC_MPLL3_FREQ_CTL_ADDR)
#define HWIO_MSS_CC_MPLL3_FREQ_CTL_INM(m)      \
        in_dword_masked(HWIO_MSS_CC_MPLL3_FREQ_CTL_ADDR, m)
#define HWIO_MSS_CC_MPLL3_FREQ_CTL_OUT(v)      \
        out_dword(HWIO_MSS_CC_MPLL3_FREQ_CTL_ADDR,v)
#define HWIO_MSS_CC_MPLL3_FREQ_CTL_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_CC_MPLL3_FREQ_CTL_ADDR,m,v,HWIO_MSS_CC_MPLL3_FREQ_CTL_IN)
#define HWIO_MSS_CC_MPLL3_FREQ_CTL_FREQUENCY_CTL_WORD_BMSK                                     0xffffffff
#define HWIO_MSS_CC_MPLL3_FREQ_CTL_FREQUENCY_CTL_WORD_SHFT                                            0x0

#define HWIO_MSS_CC_XO_TO_Q6_MODEM_BCR_ADDR                                                    (MSS_CC_MSS_CC_REG_REG_BASE      + 0x0000021c)
#define HWIO_MSS_CC_XO_TO_Q6_MODEM_BCR_RMSK                                                           0x1
#define HWIO_MSS_CC_XO_TO_Q6_MODEM_BCR_IN          \
        in_dword(HWIO_MSS_CC_XO_TO_Q6_MODEM_BCR_ADDR)
#define HWIO_MSS_CC_XO_TO_Q6_MODEM_BCR_INM(m)      \
        in_dword_masked(HWIO_MSS_CC_XO_TO_Q6_MODEM_BCR_ADDR, m)
#define HWIO_MSS_CC_XO_TO_Q6_MODEM_BCR_OUT(v)      \
        out_dword(HWIO_MSS_CC_XO_TO_Q6_MODEM_BCR_ADDR,v)
#define HWIO_MSS_CC_XO_TO_Q6_MODEM_BCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_CC_XO_TO_Q6_MODEM_BCR_ADDR,m,v,HWIO_MSS_CC_XO_TO_Q6_MODEM_BCR_IN)
#define HWIO_MSS_CC_XO_TO_Q6_MODEM_BCR_BLK_ARES_BMSK                                                  0x1
#define HWIO_MSS_CC_XO_TO_Q6_MODEM_BCR_BLK_ARES_SHFT                                                  0x0

#define HWIO_MSS_CC_XO_CMD_RCGR_ADDR                                                           (MSS_CC_MSS_CC_REG_REG_BASE      + 0x00000230)
#define HWIO_MSS_CC_XO_CMD_RCGR_RMSK                                                           0x80000013
#define HWIO_MSS_CC_XO_CMD_RCGR_IN          \
        in_dword(HWIO_MSS_CC_XO_CMD_RCGR_ADDR)
#define HWIO_MSS_CC_XO_CMD_RCGR_INM(m)      \
        in_dword_masked(HWIO_MSS_CC_XO_CMD_RCGR_ADDR, m)
#define HWIO_MSS_CC_XO_CMD_RCGR_OUT(v)      \
        out_dword(HWIO_MSS_CC_XO_CMD_RCGR_ADDR,v)
#define HWIO_MSS_CC_XO_CMD_RCGR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_CC_XO_CMD_RCGR_ADDR,m,v,HWIO_MSS_CC_XO_CMD_RCGR_IN)
#define HWIO_MSS_CC_XO_CMD_RCGR_ROOT_OFF_BMSK                                                  0x80000000
#define HWIO_MSS_CC_XO_CMD_RCGR_ROOT_OFF_SHFT                                                        0x1f
#define HWIO_MSS_CC_XO_CMD_RCGR_DIRTY_CFG_RCGR_BMSK                                                  0x10
#define HWIO_MSS_CC_XO_CMD_RCGR_DIRTY_CFG_RCGR_SHFT                                                   0x4
#define HWIO_MSS_CC_XO_CMD_RCGR_ROOT_EN_BMSK                                                          0x2
#define HWIO_MSS_CC_XO_CMD_RCGR_ROOT_EN_SHFT                                                          0x1
#define HWIO_MSS_CC_XO_CMD_RCGR_UPDATE_BMSK                                                           0x1
#define HWIO_MSS_CC_XO_CMD_RCGR_UPDATE_SHFT                                                           0x0

#define HWIO_MSS_CC_XO_CFG_RCGR_ADDR                                                           (MSS_CC_MSS_CC_REG_REG_BASE      + 0x00000234)
#define HWIO_MSS_CC_XO_CFG_RCGR_RMSK                                                             0x11071f
#define HWIO_MSS_CC_XO_CFG_RCGR_IN          \
        in_dword(HWIO_MSS_CC_XO_CFG_RCGR_ADDR)
#define HWIO_MSS_CC_XO_CFG_RCGR_INM(m)      \
        in_dword_masked(HWIO_MSS_CC_XO_CFG_RCGR_ADDR, m)
#define HWIO_MSS_CC_XO_CFG_RCGR_OUT(v)      \
        out_dword(HWIO_MSS_CC_XO_CFG_RCGR_ADDR,v)
#define HWIO_MSS_CC_XO_CFG_RCGR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_CC_XO_CFG_RCGR_ADDR,m,v,HWIO_MSS_CC_XO_CFG_RCGR_IN)
#define HWIO_MSS_CC_XO_CFG_RCGR_HW_CLK_CONTROL_BMSK                                              0x100000
#define HWIO_MSS_CC_XO_CFG_RCGR_HW_CLK_CONTROL_SHFT                                                  0x14
#define HWIO_MSS_CC_XO_CFG_RCGR_RCGLITE_DISABLE_BMSK                                              0x10000
#define HWIO_MSS_CC_XO_CFG_RCGR_RCGLITE_DISABLE_SHFT                                                 0x10
#define HWIO_MSS_CC_XO_CFG_RCGR_SRC_SEL_BMSK                                                        0x700
#define HWIO_MSS_CC_XO_CFG_RCGR_SRC_SEL_SHFT                                                          0x8
#define HWIO_MSS_CC_XO_CFG_RCGR_SRC_DIV_BMSK                                                         0x1f
#define HWIO_MSS_CC_XO_CFG_RCGR_SRC_DIV_SHFT                                                          0x0

#define HWIO_MSS_CC_QTIMER_SYNC_XO_CBCR_ADDR                                                   (MSS_CC_MSS_CC_REG_REG_BASE      + 0x00000258)
#define HWIO_MSS_CC_QTIMER_SYNC_XO_CBCR_RMSK                                                   0x80000005
#define HWIO_MSS_CC_QTIMER_SYNC_XO_CBCR_IN          \
        in_dword(HWIO_MSS_CC_QTIMER_SYNC_XO_CBCR_ADDR)
#define HWIO_MSS_CC_QTIMER_SYNC_XO_CBCR_INM(m)      \
        in_dword_masked(HWIO_MSS_CC_QTIMER_SYNC_XO_CBCR_ADDR, m)
#define HWIO_MSS_CC_QTIMER_SYNC_XO_CBCR_OUT(v)      \
        out_dword(HWIO_MSS_CC_QTIMER_SYNC_XO_CBCR_ADDR,v)
#define HWIO_MSS_CC_QTIMER_SYNC_XO_CBCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_CC_QTIMER_SYNC_XO_CBCR_ADDR,m,v,HWIO_MSS_CC_QTIMER_SYNC_XO_CBCR_IN)
#define HWIO_MSS_CC_QTIMER_SYNC_XO_CBCR_CLK_OFF_BMSK                                           0x80000000
#define HWIO_MSS_CC_QTIMER_SYNC_XO_CBCR_CLK_OFF_SHFT                                                 0x1f
#define HWIO_MSS_CC_QTIMER_SYNC_XO_CBCR_CLK_ARES_BMSK                                                 0x4
#define HWIO_MSS_CC_QTIMER_SYNC_XO_CBCR_CLK_ARES_SHFT                                                 0x2
#define HWIO_MSS_CC_QTIMER_SYNC_XO_CBCR_CLK_ENABLE_BMSK                                               0x1
#define HWIO_MSS_CC_QTIMER_SYNC_XO_CBCR_CLK_ENABLE_SHFT                                               0x0

#define HWIO_MSS_CC_BUS_BCR_ADDR                                                               (MSS_CC_MSS_CC_REG_REG_BASE      + 0x000002a8)
#define HWIO_MSS_CC_BUS_BCR_RMSK                                                                      0x1
#define HWIO_MSS_CC_BUS_BCR_IN          \
        in_dword(HWIO_MSS_CC_BUS_BCR_ADDR)
#define HWIO_MSS_CC_BUS_BCR_INM(m)      \
        in_dword_masked(HWIO_MSS_CC_BUS_BCR_ADDR, m)
#define HWIO_MSS_CC_BUS_BCR_OUT(v)      \
        out_dword(HWIO_MSS_CC_BUS_BCR_ADDR,v)
#define HWIO_MSS_CC_BUS_BCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_CC_BUS_BCR_ADDR,m,v,HWIO_MSS_CC_BUS_BCR_IN)
#define HWIO_MSS_CC_BUS_BCR_BLK_ARES_BMSK                                                             0x1
#define HWIO_MSS_CC_BUS_BCR_BLK_ARES_SHFT                                                             0x0

#define HWIO_MSS_CC_BUS_PRE_MUX_CMD_RCGR_ADDR                                                  (MSS_CC_MSS_CC_REG_REG_BASE      + 0x000002bc)
#define HWIO_MSS_CC_BUS_PRE_MUX_CMD_RCGR_RMSK                                                  0x80000013
#define HWIO_MSS_CC_BUS_PRE_MUX_CMD_RCGR_IN          \
        in_dword(HWIO_MSS_CC_BUS_PRE_MUX_CMD_RCGR_ADDR)
#define HWIO_MSS_CC_BUS_PRE_MUX_CMD_RCGR_INM(m)      \
        in_dword_masked(HWIO_MSS_CC_BUS_PRE_MUX_CMD_RCGR_ADDR, m)
#define HWIO_MSS_CC_BUS_PRE_MUX_CMD_RCGR_OUT(v)      \
        out_dword(HWIO_MSS_CC_BUS_PRE_MUX_CMD_RCGR_ADDR,v)
#define HWIO_MSS_CC_BUS_PRE_MUX_CMD_RCGR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_CC_BUS_PRE_MUX_CMD_RCGR_ADDR,m,v,HWIO_MSS_CC_BUS_PRE_MUX_CMD_RCGR_IN)
#define HWIO_MSS_CC_BUS_PRE_MUX_CMD_RCGR_ROOT_OFF_BMSK                                         0x80000000
#define HWIO_MSS_CC_BUS_PRE_MUX_CMD_RCGR_ROOT_OFF_SHFT                                               0x1f
#define HWIO_MSS_CC_BUS_PRE_MUX_CMD_RCGR_DIRTY_CFG_RCGR_BMSK                                         0x10
#define HWIO_MSS_CC_BUS_PRE_MUX_CMD_RCGR_DIRTY_CFG_RCGR_SHFT                                          0x4
#define HWIO_MSS_CC_BUS_PRE_MUX_CMD_RCGR_ROOT_EN_BMSK                                                 0x2
#define HWIO_MSS_CC_BUS_PRE_MUX_CMD_RCGR_ROOT_EN_SHFT                                                 0x1
#define HWIO_MSS_CC_BUS_PRE_MUX_CMD_RCGR_UPDATE_BMSK                                                  0x1
#define HWIO_MSS_CC_BUS_PRE_MUX_CMD_RCGR_UPDATE_SHFT                                                  0x0

#define HWIO_MSS_CC_BUS_PRE_MUX_CFG_RCGR_ADDR                                                  (MSS_CC_MSS_CC_REG_REG_BASE      + 0x000002c0)
#define HWIO_MSS_CC_BUS_PRE_MUX_CFG_RCGR_RMSK                                                    0x11071f
#define HWIO_MSS_CC_BUS_PRE_MUX_CFG_RCGR_IN          \
        in_dword(HWIO_MSS_CC_BUS_PRE_MUX_CFG_RCGR_ADDR)
#define HWIO_MSS_CC_BUS_PRE_MUX_CFG_RCGR_INM(m)      \
        in_dword_masked(HWIO_MSS_CC_BUS_PRE_MUX_CFG_RCGR_ADDR, m)
#define HWIO_MSS_CC_BUS_PRE_MUX_CFG_RCGR_OUT(v)      \
        out_dword(HWIO_MSS_CC_BUS_PRE_MUX_CFG_RCGR_ADDR,v)
#define HWIO_MSS_CC_BUS_PRE_MUX_CFG_RCGR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_CC_BUS_PRE_MUX_CFG_RCGR_ADDR,m,v,HWIO_MSS_CC_BUS_PRE_MUX_CFG_RCGR_IN)
#define HWIO_MSS_CC_BUS_PRE_MUX_CFG_RCGR_HW_CLK_CONTROL_BMSK                                     0x100000
#define HWIO_MSS_CC_BUS_PRE_MUX_CFG_RCGR_HW_CLK_CONTROL_SHFT                                         0x14
#define HWIO_MSS_CC_BUS_PRE_MUX_CFG_RCGR_RCGLITE_DISABLE_BMSK                                     0x10000
#define HWIO_MSS_CC_BUS_PRE_MUX_CFG_RCGR_RCGLITE_DISABLE_SHFT                                        0x10
#define HWIO_MSS_CC_BUS_PRE_MUX_CFG_RCGR_SRC_SEL_BMSK                                               0x700
#define HWIO_MSS_CC_BUS_PRE_MUX_CFG_RCGR_SRC_SEL_SHFT                                                 0x8
#define HWIO_MSS_CC_BUS_PRE_MUX_CFG_RCGR_SRC_DIV_BMSK                                                0x1f
#define HWIO_MSS_CC_BUS_PRE_MUX_CFG_RCGR_SRC_DIV_SHFT                                                 0x0

#define HWIO_MSS_CC_MODEM_BUS_PRE_MUX_CMD_RCGR_ADDR                                            (MSS_CC_MSS_CC_REG_REG_BASE      + 0x000002e4)
#define HWIO_MSS_CC_MODEM_BUS_PRE_MUX_CMD_RCGR_RMSK                                            0x80000013
#define HWIO_MSS_CC_MODEM_BUS_PRE_MUX_CMD_RCGR_IN          \
        in_dword(HWIO_MSS_CC_MODEM_BUS_PRE_MUX_CMD_RCGR_ADDR)
#define HWIO_MSS_CC_MODEM_BUS_PRE_MUX_CMD_RCGR_INM(m)      \
        in_dword_masked(HWIO_MSS_CC_MODEM_BUS_PRE_MUX_CMD_RCGR_ADDR, m)
#define HWIO_MSS_CC_MODEM_BUS_PRE_MUX_CMD_RCGR_OUT(v)      \
        out_dword(HWIO_MSS_CC_MODEM_BUS_PRE_MUX_CMD_RCGR_ADDR,v)
#define HWIO_MSS_CC_MODEM_BUS_PRE_MUX_CMD_RCGR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_CC_MODEM_BUS_PRE_MUX_CMD_RCGR_ADDR,m,v,HWIO_MSS_CC_MODEM_BUS_PRE_MUX_CMD_RCGR_IN)
#define HWIO_MSS_CC_MODEM_BUS_PRE_MUX_CMD_RCGR_ROOT_OFF_BMSK                                   0x80000000
#define HWIO_MSS_CC_MODEM_BUS_PRE_MUX_CMD_RCGR_ROOT_OFF_SHFT                                         0x1f
#define HWIO_MSS_CC_MODEM_BUS_PRE_MUX_CMD_RCGR_DIRTY_CFG_RCGR_BMSK                                   0x10
#define HWIO_MSS_CC_MODEM_BUS_PRE_MUX_CMD_RCGR_DIRTY_CFG_RCGR_SHFT                                    0x4
#define HWIO_MSS_CC_MODEM_BUS_PRE_MUX_CMD_RCGR_ROOT_EN_BMSK                                           0x2
#define HWIO_MSS_CC_MODEM_BUS_PRE_MUX_CMD_RCGR_ROOT_EN_SHFT                                           0x1
#define HWIO_MSS_CC_MODEM_BUS_PRE_MUX_CMD_RCGR_UPDATE_BMSK                                            0x1
#define HWIO_MSS_CC_MODEM_BUS_PRE_MUX_CMD_RCGR_UPDATE_SHFT                                            0x0

#define HWIO_MSS_CC_MODEM_BUS_PRE_MUX_CFG_RCGR_ADDR                                            (MSS_CC_MSS_CC_REG_REG_BASE      + 0x000002e8)
#define HWIO_MSS_CC_MODEM_BUS_PRE_MUX_CFG_RCGR_RMSK                                              0x11071f
#define HWIO_MSS_CC_MODEM_BUS_PRE_MUX_CFG_RCGR_IN          \
        in_dword(HWIO_MSS_CC_MODEM_BUS_PRE_MUX_CFG_RCGR_ADDR)
#define HWIO_MSS_CC_MODEM_BUS_PRE_MUX_CFG_RCGR_INM(m)      \
        in_dword_masked(HWIO_MSS_CC_MODEM_BUS_PRE_MUX_CFG_RCGR_ADDR, m)
#define HWIO_MSS_CC_MODEM_BUS_PRE_MUX_CFG_RCGR_OUT(v)      \
        out_dword(HWIO_MSS_CC_MODEM_BUS_PRE_MUX_CFG_RCGR_ADDR,v)
#define HWIO_MSS_CC_MODEM_BUS_PRE_MUX_CFG_RCGR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_CC_MODEM_BUS_PRE_MUX_CFG_RCGR_ADDR,m,v,HWIO_MSS_CC_MODEM_BUS_PRE_MUX_CFG_RCGR_IN)
#define HWIO_MSS_CC_MODEM_BUS_PRE_MUX_CFG_RCGR_HW_CLK_CONTROL_BMSK                               0x100000
#define HWIO_MSS_CC_MODEM_BUS_PRE_MUX_CFG_RCGR_HW_CLK_CONTROL_SHFT                                   0x14
#define HWIO_MSS_CC_MODEM_BUS_PRE_MUX_CFG_RCGR_RCGLITE_DISABLE_BMSK                               0x10000
#define HWIO_MSS_CC_MODEM_BUS_PRE_MUX_CFG_RCGR_RCGLITE_DISABLE_SHFT                                  0x10
#define HWIO_MSS_CC_MODEM_BUS_PRE_MUX_CFG_RCGR_SRC_SEL_BMSK                                         0x700
#define HWIO_MSS_CC_MODEM_BUS_PRE_MUX_CFG_RCGR_SRC_SEL_SHFT                                           0x8
#define HWIO_MSS_CC_MODEM_BUS_PRE_MUX_CFG_RCGR_SRC_DIV_BMSK                                          0x1f
#define HWIO_MSS_CC_MODEM_BUS_PRE_MUX_CFG_RCGR_SRC_DIV_SHFT                                           0x0

#define HWIO_MSS_CC_COXM_BUS_CBCR_ADDR                                                         (MSS_CC_MSS_CC_REG_REG_BASE      + 0x0000030c)
#define HWIO_MSS_CC_COXM_BUS_CBCR_RMSK                                                         0x80007ff5
#define HWIO_MSS_CC_COXM_BUS_CBCR_IN          \
        in_dword(HWIO_MSS_CC_COXM_BUS_CBCR_ADDR)
#define HWIO_MSS_CC_COXM_BUS_CBCR_INM(m)      \
        in_dword_masked(HWIO_MSS_CC_COXM_BUS_CBCR_ADDR, m)
#define HWIO_MSS_CC_COXM_BUS_CBCR_OUT(v)      \
        out_dword(HWIO_MSS_CC_COXM_BUS_CBCR_ADDR,v)
#define HWIO_MSS_CC_COXM_BUS_CBCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_CC_COXM_BUS_CBCR_ADDR,m,v,HWIO_MSS_CC_COXM_BUS_CBCR_IN)
#define HWIO_MSS_CC_COXM_BUS_CBCR_CLK_OFF_BMSK                                                 0x80000000
#define HWIO_MSS_CC_COXM_BUS_CBCR_CLK_OFF_SHFT                                                       0x1f
#define HWIO_MSS_CC_COXM_BUS_CBCR_FORCE_MEM_CORE_ON_BMSK                                           0x4000
#define HWIO_MSS_CC_COXM_BUS_CBCR_FORCE_MEM_CORE_ON_SHFT                                              0xe
#define HWIO_MSS_CC_COXM_BUS_CBCR_FORCE_MEM_PERIPH_ON_BMSK                                         0x2000
#define HWIO_MSS_CC_COXM_BUS_CBCR_FORCE_MEM_PERIPH_ON_SHFT                                            0xd
#define HWIO_MSS_CC_COXM_BUS_CBCR_FORCE_MEM_PERIPH_OFF_BMSK                                        0x1000
#define HWIO_MSS_CC_COXM_BUS_CBCR_FORCE_MEM_PERIPH_OFF_SHFT                                           0xc
#define HWIO_MSS_CC_COXM_BUS_CBCR_WAKEUP_BMSK                                                       0xf00
#define HWIO_MSS_CC_COXM_BUS_CBCR_WAKEUP_SHFT                                                         0x8
#define HWIO_MSS_CC_COXM_BUS_CBCR_SLEEP_BMSK                                                         0xf0
#define HWIO_MSS_CC_COXM_BUS_CBCR_SLEEP_SHFT                                                          0x4
#define HWIO_MSS_CC_COXM_BUS_CBCR_CLK_ARES_BMSK                                                       0x4
#define HWIO_MSS_CC_COXM_BUS_CBCR_CLK_ARES_SHFT                                                       0x2
#define HWIO_MSS_CC_COXM_BUS_CBCR_CLK_ENABLE_BMSK                                                     0x1
#define HWIO_MSS_CC_COXM_BUS_CBCR_CLK_ENABLE_SHFT                                                     0x0

#define HWIO_MSS_CC_CRYPTO_CORE_CBCR_ADDR                                                      (MSS_CC_MSS_CC_REG_REG_BASE      + 0x00000320)
#define HWIO_MSS_CC_CRYPTO_CORE_CBCR_RMSK                                                      0x80000005
#define HWIO_MSS_CC_CRYPTO_CORE_CBCR_IN          \
        in_dword(HWIO_MSS_CC_CRYPTO_CORE_CBCR_ADDR)
#define HWIO_MSS_CC_CRYPTO_CORE_CBCR_INM(m)      \
        in_dword_masked(HWIO_MSS_CC_CRYPTO_CORE_CBCR_ADDR, m)
#define HWIO_MSS_CC_CRYPTO_CORE_CBCR_OUT(v)      \
        out_dword(HWIO_MSS_CC_CRYPTO_CORE_CBCR_ADDR,v)
#define HWIO_MSS_CC_CRYPTO_CORE_CBCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_CC_CRYPTO_CORE_CBCR_ADDR,m,v,HWIO_MSS_CC_CRYPTO_CORE_CBCR_IN)
#define HWIO_MSS_CC_CRYPTO_CORE_CBCR_CLK_OFF_BMSK                                              0x80000000
#define HWIO_MSS_CC_CRYPTO_CORE_CBCR_CLK_OFF_SHFT                                                    0x1f
#define HWIO_MSS_CC_CRYPTO_CORE_CBCR_CLK_ARES_BMSK                                                    0x4
#define HWIO_MSS_CC_CRYPTO_CORE_CBCR_CLK_ARES_SHFT                                                    0x2
#define HWIO_MSS_CC_CRYPTO_CORE_CBCR_CLK_ENABLE_BMSK                                                  0x1
#define HWIO_MSS_CC_CRYPTO_CORE_CBCR_CLK_ENABLE_SHFT                                                  0x0

#define HWIO_MSS_CC_CRYPTO_BUS_CBCR_ADDR                                                       (MSS_CC_MSS_CC_REG_REG_BASE      + 0x00000334)
#define HWIO_MSS_CC_CRYPTO_BUS_CBCR_RMSK                                                       0x80000005
#define HWIO_MSS_CC_CRYPTO_BUS_CBCR_IN          \
        in_dword(HWIO_MSS_CC_CRYPTO_BUS_CBCR_ADDR)
#define HWIO_MSS_CC_CRYPTO_BUS_CBCR_INM(m)      \
        in_dword_masked(HWIO_MSS_CC_CRYPTO_BUS_CBCR_ADDR, m)
#define HWIO_MSS_CC_CRYPTO_BUS_CBCR_OUT(v)      \
        out_dword(HWIO_MSS_CC_CRYPTO_BUS_CBCR_ADDR,v)
#define HWIO_MSS_CC_CRYPTO_BUS_CBCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_CC_CRYPTO_BUS_CBCR_ADDR,m,v,HWIO_MSS_CC_CRYPTO_BUS_CBCR_IN)
#define HWIO_MSS_CC_CRYPTO_BUS_CBCR_CLK_OFF_BMSK                                               0x80000000
#define HWIO_MSS_CC_CRYPTO_BUS_CBCR_CLK_OFF_SHFT                                                     0x1f
#define HWIO_MSS_CC_CRYPTO_BUS_CBCR_CLK_ARES_BMSK                                                     0x4
#define HWIO_MSS_CC_CRYPTO_BUS_CBCR_CLK_ARES_SHFT                                                     0x2
#define HWIO_MSS_CC_CRYPTO_BUS_CBCR_CLK_ENABLE_BMSK                                                   0x1
#define HWIO_MSS_CC_CRYPTO_BUS_CBCR_CLK_ENABLE_SHFT                                                   0x0

#define HWIO_MSS_CC_UIM0_BUS_CBCR_ADDR                                                         (MSS_CC_MSS_CC_REG_REG_BASE      + 0x00000348)
#define HWIO_MSS_CC_UIM0_BUS_CBCR_RMSK                                                         0x80007ff5
#define HWIO_MSS_CC_UIM0_BUS_CBCR_IN          \
        in_dword(HWIO_MSS_CC_UIM0_BUS_CBCR_ADDR)
#define HWIO_MSS_CC_UIM0_BUS_CBCR_INM(m)      \
        in_dword_masked(HWIO_MSS_CC_UIM0_BUS_CBCR_ADDR, m)
#define HWIO_MSS_CC_UIM0_BUS_CBCR_OUT(v)      \
        out_dword(HWIO_MSS_CC_UIM0_BUS_CBCR_ADDR,v)
#define HWIO_MSS_CC_UIM0_BUS_CBCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_CC_UIM0_BUS_CBCR_ADDR,m,v,HWIO_MSS_CC_UIM0_BUS_CBCR_IN)
#define HWIO_MSS_CC_UIM0_BUS_CBCR_CLK_OFF_BMSK                                                 0x80000000
#define HWIO_MSS_CC_UIM0_BUS_CBCR_CLK_OFF_SHFT                                                       0x1f
#define HWIO_MSS_CC_UIM0_BUS_CBCR_FORCE_MEM_CORE_ON_BMSK                                           0x4000
#define HWIO_MSS_CC_UIM0_BUS_CBCR_FORCE_MEM_CORE_ON_SHFT                                              0xe
#define HWIO_MSS_CC_UIM0_BUS_CBCR_FORCE_MEM_PERIPH_ON_BMSK                                         0x2000
#define HWIO_MSS_CC_UIM0_BUS_CBCR_FORCE_MEM_PERIPH_ON_SHFT                                            0xd
#define HWIO_MSS_CC_UIM0_BUS_CBCR_FORCE_MEM_PERIPH_OFF_BMSK                                        0x1000
#define HWIO_MSS_CC_UIM0_BUS_CBCR_FORCE_MEM_PERIPH_OFF_SHFT                                           0xc
#define HWIO_MSS_CC_UIM0_BUS_CBCR_WAKEUP_BMSK                                                       0xf00
#define HWIO_MSS_CC_UIM0_BUS_CBCR_WAKEUP_SHFT                                                         0x8
#define HWIO_MSS_CC_UIM0_BUS_CBCR_SLEEP_BMSK                                                         0xf0
#define HWIO_MSS_CC_UIM0_BUS_CBCR_SLEEP_SHFT                                                          0x4
#define HWIO_MSS_CC_UIM0_BUS_CBCR_CLK_ARES_BMSK                                                       0x4
#define HWIO_MSS_CC_UIM0_BUS_CBCR_CLK_ARES_SHFT                                                       0x2
#define HWIO_MSS_CC_UIM0_BUS_CBCR_CLK_ENABLE_BMSK                                                     0x1
#define HWIO_MSS_CC_UIM0_BUS_CBCR_CLK_ENABLE_SHFT                                                     0x0

#define HWIO_MSS_CC_UIM1_BUS_CBCR_ADDR                                                         (MSS_CC_MSS_CC_REG_REG_BASE      + 0x0000035c)
#define HWIO_MSS_CC_UIM1_BUS_CBCR_RMSK                                                         0x80007ff5
#define HWIO_MSS_CC_UIM1_BUS_CBCR_IN          \
        in_dword(HWIO_MSS_CC_UIM1_BUS_CBCR_ADDR)
#define HWIO_MSS_CC_UIM1_BUS_CBCR_INM(m)      \
        in_dword_masked(HWIO_MSS_CC_UIM1_BUS_CBCR_ADDR, m)
#define HWIO_MSS_CC_UIM1_BUS_CBCR_OUT(v)      \
        out_dword(HWIO_MSS_CC_UIM1_BUS_CBCR_ADDR,v)
#define HWIO_MSS_CC_UIM1_BUS_CBCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_CC_UIM1_BUS_CBCR_ADDR,m,v,HWIO_MSS_CC_UIM1_BUS_CBCR_IN)
#define HWIO_MSS_CC_UIM1_BUS_CBCR_CLK_OFF_BMSK                                                 0x80000000
#define HWIO_MSS_CC_UIM1_BUS_CBCR_CLK_OFF_SHFT                                                       0x1f
#define HWIO_MSS_CC_UIM1_BUS_CBCR_FORCE_MEM_CORE_ON_BMSK                                           0x4000
#define HWIO_MSS_CC_UIM1_BUS_CBCR_FORCE_MEM_CORE_ON_SHFT                                              0xe
#define HWIO_MSS_CC_UIM1_BUS_CBCR_FORCE_MEM_PERIPH_ON_BMSK                                         0x2000
#define HWIO_MSS_CC_UIM1_BUS_CBCR_FORCE_MEM_PERIPH_ON_SHFT                                            0xd
#define HWIO_MSS_CC_UIM1_BUS_CBCR_FORCE_MEM_PERIPH_OFF_BMSK                                        0x1000
#define HWIO_MSS_CC_UIM1_BUS_CBCR_FORCE_MEM_PERIPH_OFF_SHFT                                           0xc
#define HWIO_MSS_CC_UIM1_BUS_CBCR_WAKEUP_BMSK                                                       0xf00
#define HWIO_MSS_CC_UIM1_BUS_CBCR_WAKEUP_SHFT                                                         0x8
#define HWIO_MSS_CC_UIM1_BUS_CBCR_SLEEP_BMSK                                                         0xf0
#define HWIO_MSS_CC_UIM1_BUS_CBCR_SLEEP_SHFT                                                          0x4
#define HWIO_MSS_CC_UIM1_BUS_CBCR_CLK_ARES_BMSK                                                       0x4
#define HWIO_MSS_CC_UIM1_BUS_CBCR_CLK_ARES_SHFT                                                       0x2
#define HWIO_MSS_CC_UIM1_BUS_CBCR_CLK_ENABLE_BMSK                                                     0x1
#define HWIO_MSS_CC_UIM1_BUS_CBCR_CLK_ENABLE_SHFT                                                     0x0

#define HWIO_MSS_CC_CSR_BUS_CBCR_ADDR                                                          (MSS_CC_MSS_CC_REG_REG_BASE      + 0x00000384)
#define HWIO_MSS_CC_CSR_BUS_CBCR_RMSK                                                          0x80000005
#define HWIO_MSS_CC_CSR_BUS_CBCR_IN          \
        in_dword(HWIO_MSS_CC_CSR_BUS_CBCR_ADDR)
#define HWIO_MSS_CC_CSR_BUS_CBCR_INM(m)      \
        in_dword_masked(HWIO_MSS_CC_CSR_BUS_CBCR_ADDR, m)
#define HWIO_MSS_CC_CSR_BUS_CBCR_OUT(v)      \
        out_dword(HWIO_MSS_CC_CSR_BUS_CBCR_ADDR,v)
#define HWIO_MSS_CC_CSR_BUS_CBCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_CC_CSR_BUS_CBCR_ADDR,m,v,HWIO_MSS_CC_CSR_BUS_CBCR_IN)
#define HWIO_MSS_CC_CSR_BUS_CBCR_CLK_OFF_BMSK                                                  0x80000000
#define HWIO_MSS_CC_CSR_BUS_CBCR_CLK_OFF_SHFT                                                        0x1f
#define HWIO_MSS_CC_CSR_BUS_CBCR_CLK_ARES_BMSK                                                        0x4
#define HWIO_MSS_CC_CSR_BUS_CBCR_CLK_ARES_SHFT                                                        0x2
#define HWIO_MSS_CC_CSR_BUS_CBCR_CLK_ENABLE_BMSK                                                      0x1
#define HWIO_MSS_CC_CSR_BUS_CBCR_CLK_ENABLE_SHFT                                                      0x0

#define HWIO_MSS_CC_ATB_BUS_CBCR_ADDR                                                          (MSS_CC_MSS_CC_REG_REG_BASE      + 0x00000398)
#define HWIO_MSS_CC_ATB_BUS_CBCR_RMSK                                                          0x80000005
#define HWIO_MSS_CC_ATB_BUS_CBCR_IN          \
        in_dword(HWIO_MSS_CC_ATB_BUS_CBCR_ADDR)
#define HWIO_MSS_CC_ATB_BUS_CBCR_INM(m)      \
        in_dword_masked(HWIO_MSS_CC_ATB_BUS_CBCR_ADDR, m)
#define HWIO_MSS_CC_ATB_BUS_CBCR_OUT(v)      \
        out_dword(HWIO_MSS_CC_ATB_BUS_CBCR_ADDR,v)
#define HWIO_MSS_CC_ATB_BUS_CBCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_CC_ATB_BUS_CBCR_ADDR,m,v,HWIO_MSS_CC_ATB_BUS_CBCR_IN)
#define HWIO_MSS_CC_ATB_BUS_CBCR_CLK_OFF_BMSK                                                  0x80000000
#define HWIO_MSS_CC_ATB_BUS_CBCR_CLK_OFF_SHFT                                                        0x1f
#define HWIO_MSS_CC_ATB_BUS_CBCR_CLK_ARES_BMSK                                                        0x4
#define HWIO_MSS_CC_ATB_BUS_CBCR_CLK_ARES_SHFT                                                        0x2
#define HWIO_MSS_CC_ATB_BUS_CBCR_CLK_ENABLE_BMSK                                                      0x1
#define HWIO_MSS_CC_ATB_BUS_CBCR_CLK_ENABLE_SHFT                                                      0x0

#define HWIO_MSS_CC_BRIDGE_BUS_CBCR_ADDR                                                       (MSS_CC_MSS_CC_REG_REG_BASE      + 0x000003ac)
#define HWIO_MSS_CC_BRIDGE_BUS_CBCR_RMSK                                                       0x80000005
#define HWIO_MSS_CC_BRIDGE_BUS_CBCR_IN          \
        in_dword(HWIO_MSS_CC_BRIDGE_BUS_CBCR_ADDR)
#define HWIO_MSS_CC_BRIDGE_BUS_CBCR_INM(m)      \
        in_dword_masked(HWIO_MSS_CC_BRIDGE_BUS_CBCR_ADDR, m)
#define HWIO_MSS_CC_BRIDGE_BUS_CBCR_OUT(v)      \
        out_dword(HWIO_MSS_CC_BRIDGE_BUS_CBCR_ADDR,v)
#define HWIO_MSS_CC_BRIDGE_BUS_CBCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_CC_BRIDGE_BUS_CBCR_ADDR,m,v,HWIO_MSS_CC_BRIDGE_BUS_CBCR_IN)
#define HWIO_MSS_CC_BRIDGE_BUS_CBCR_CLK_OFF_BMSK                                               0x80000000
#define HWIO_MSS_CC_BRIDGE_BUS_CBCR_CLK_OFF_SHFT                                                     0x1f
#define HWIO_MSS_CC_BRIDGE_BUS_CBCR_CLK_ARES_BMSK                                                     0x4
#define HWIO_MSS_CC_BRIDGE_BUS_CBCR_CLK_ARES_SHFT                                                     0x2
#define HWIO_MSS_CC_BRIDGE_BUS_CBCR_CLK_ENABLE_BMSK                                                   0x1
#define HWIO_MSS_CC_BRIDGE_BUS_CBCR_CLK_ENABLE_SHFT                                                   0x0

#define HWIO_MSS_CC_Q6_ALT_CBCR_ADDR                                                           (MSS_CC_MSS_CC_REG_REG_BASE      + 0x000003c0)
#define HWIO_MSS_CC_Q6_ALT_CBCR_RMSK                                                           0x80000005
#define HWIO_MSS_CC_Q6_ALT_CBCR_IN          \
        in_dword(HWIO_MSS_CC_Q6_ALT_CBCR_ADDR)
#define HWIO_MSS_CC_Q6_ALT_CBCR_INM(m)      \
        in_dword_masked(HWIO_MSS_CC_Q6_ALT_CBCR_ADDR, m)
#define HWIO_MSS_CC_Q6_ALT_CBCR_OUT(v)      \
        out_dword(HWIO_MSS_CC_Q6_ALT_CBCR_ADDR,v)
#define HWIO_MSS_CC_Q6_ALT_CBCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_CC_Q6_ALT_CBCR_ADDR,m,v,HWIO_MSS_CC_Q6_ALT_CBCR_IN)
#define HWIO_MSS_CC_Q6_ALT_CBCR_CLK_OFF_BMSK                                                   0x80000000
#define HWIO_MSS_CC_Q6_ALT_CBCR_CLK_OFF_SHFT                                                         0x1f
#define HWIO_MSS_CC_Q6_ALT_CBCR_CLK_ARES_BMSK                                                         0x4
#define HWIO_MSS_CC_Q6_ALT_CBCR_CLK_ARES_SHFT                                                         0x2
#define HWIO_MSS_CC_Q6_ALT_CBCR_CLK_ENABLE_BMSK                                                       0x1
#define HWIO_MSS_CC_Q6_ALT_CBCR_CLK_ENABLE_SHFT                                                       0x0

#define HWIO_MSS_CC_SLAVE_TIMEOUT_BUS_CBCR_ADDR                                                (MSS_CC_MSS_CC_REG_REG_BASE      + 0x000003d4)
#define HWIO_MSS_CC_SLAVE_TIMEOUT_BUS_CBCR_RMSK                                                0x80000005
#define HWIO_MSS_CC_SLAVE_TIMEOUT_BUS_CBCR_IN          \
        in_dword(HWIO_MSS_CC_SLAVE_TIMEOUT_BUS_CBCR_ADDR)
#define HWIO_MSS_CC_SLAVE_TIMEOUT_BUS_CBCR_INM(m)      \
        in_dword_masked(HWIO_MSS_CC_SLAVE_TIMEOUT_BUS_CBCR_ADDR, m)
#define HWIO_MSS_CC_SLAVE_TIMEOUT_BUS_CBCR_OUT(v)      \
        out_dword(HWIO_MSS_CC_SLAVE_TIMEOUT_BUS_CBCR_ADDR,v)
#define HWIO_MSS_CC_SLAVE_TIMEOUT_BUS_CBCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_CC_SLAVE_TIMEOUT_BUS_CBCR_ADDR,m,v,HWIO_MSS_CC_SLAVE_TIMEOUT_BUS_CBCR_IN)
#define HWIO_MSS_CC_SLAVE_TIMEOUT_BUS_CBCR_CLK_OFF_BMSK                                        0x80000000
#define HWIO_MSS_CC_SLAVE_TIMEOUT_BUS_CBCR_CLK_OFF_SHFT                                              0x1f
#define HWIO_MSS_CC_SLAVE_TIMEOUT_BUS_CBCR_CLK_ARES_BMSK                                              0x4
#define HWIO_MSS_CC_SLAVE_TIMEOUT_BUS_CBCR_CLK_ARES_SHFT                                              0x2
#define HWIO_MSS_CC_SLAVE_TIMEOUT_BUS_CBCR_CLK_ENABLE_BMSK                                            0x1
#define HWIO_MSS_CC_SLAVE_TIMEOUT_BUS_CBCR_CLK_ENABLE_SHFT                                            0x0

#define HWIO_MSS_CC_NAV_BUS_CX_TO_LS_CBCR_ADDR                                                 (MSS_CC_MSS_CC_REG_REG_BASE      + 0x000003fc)
#define HWIO_MSS_CC_NAV_BUS_CX_TO_LS_CBCR_RMSK                                                 0x80000005
#define HWIO_MSS_CC_NAV_BUS_CX_TO_LS_CBCR_IN          \
        in_dword(HWIO_MSS_CC_NAV_BUS_CX_TO_LS_CBCR_ADDR)
#define HWIO_MSS_CC_NAV_BUS_CX_TO_LS_CBCR_INM(m)      \
        in_dword_masked(HWIO_MSS_CC_NAV_BUS_CX_TO_LS_CBCR_ADDR, m)
#define HWIO_MSS_CC_NAV_BUS_CX_TO_LS_CBCR_OUT(v)      \
        out_dword(HWIO_MSS_CC_NAV_BUS_CX_TO_LS_CBCR_ADDR,v)
#define HWIO_MSS_CC_NAV_BUS_CX_TO_LS_CBCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_CC_NAV_BUS_CX_TO_LS_CBCR_ADDR,m,v,HWIO_MSS_CC_NAV_BUS_CX_TO_LS_CBCR_IN)
#define HWIO_MSS_CC_NAV_BUS_CX_TO_LS_CBCR_CLK_OFF_BMSK                                         0x80000000
#define HWIO_MSS_CC_NAV_BUS_CX_TO_LS_CBCR_CLK_OFF_SHFT                                               0x1f
#define HWIO_MSS_CC_NAV_BUS_CX_TO_LS_CBCR_CLK_ARES_BMSK                                               0x4
#define HWIO_MSS_CC_NAV_BUS_CX_TO_LS_CBCR_CLK_ARES_SHFT                                               0x2
#define HWIO_MSS_CC_NAV_BUS_CX_TO_LS_CBCR_CLK_ENABLE_BMSK                                             0x1
#define HWIO_MSS_CC_NAV_BUS_CX_TO_LS_CBCR_CLK_ENABLE_SHFT                                             0x0

#define HWIO_MSS_CC_MGPI_BUS_CBCR_ADDR                                                         (MSS_CC_MSS_CC_REG_REG_BASE      + 0x00000410)
#define HWIO_MSS_CC_MGPI_BUS_CBCR_RMSK                                                         0x80000005
#define HWIO_MSS_CC_MGPI_BUS_CBCR_IN          \
        in_dword(HWIO_MSS_CC_MGPI_BUS_CBCR_ADDR)
#define HWIO_MSS_CC_MGPI_BUS_CBCR_INM(m)      \
        in_dword_masked(HWIO_MSS_CC_MGPI_BUS_CBCR_ADDR, m)
#define HWIO_MSS_CC_MGPI_BUS_CBCR_OUT(v)      \
        out_dword(HWIO_MSS_CC_MGPI_BUS_CBCR_ADDR,v)
#define HWIO_MSS_CC_MGPI_BUS_CBCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_CC_MGPI_BUS_CBCR_ADDR,m,v,HWIO_MSS_CC_MGPI_BUS_CBCR_IN)
#define HWIO_MSS_CC_MGPI_BUS_CBCR_CLK_OFF_BMSK                                                 0x80000000
#define HWIO_MSS_CC_MGPI_BUS_CBCR_CLK_OFF_SHFT                                                       0x1f
#define HWIO_MSS_CC_MGPI_BUS_CBCR_CLK_ARES_BMSK                                                       0x4
#define HWIO_MSS_CC_MGPI_BUS_CBCR_CLK_ARES_SHFT                                                       0x2
#define HWIO_MSS_CC_MGPI_BUS_CBCR_CLK_ENABLE_BMSK                                                     0x1
#define HWIO_MSS_CC_MGPI_BUS_CBCR_CLK_ENABLE_SHFT                                                     0x0

#define HWIO_MSS_CC_MODEM_BCR_ADDR                                                             (MSS_CC_MSS_CC_REG_REG_BASE      + 0x00000424)
#define HWIO_MSS_CC_MODEM_BCR_RMSK                                                                    0x1
#define HWIO_MSS_CC_MODEM_BCR_IN          \
        in_dword(HWIO_MSS_CC_MODEM_BCR_ADDR)
#define HWIO_MSS_CC_MODEM_BCR_INM(m)      \
        in_dword_masked(HWIO_MSS_CC_MODEM_BCR_ADDR, m)
#define HWIO_MSS_CC_MODEM_BCR_OUT(v)      \
        out_dword(HWIO_MSS_CC_MODEM_BCR_ADDR,v)
#define HWIO_MSS_CC_MODEM_BCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_CC_MODEM_BCR_ADDR,m,v,HWIO_MSS_CC_MODEM_BCR_IN)
#define HWIO_MSS_CC_MODEM_BCR_BLK_ARES_BMSK                                                           0x1
#define HWIO_MSS_CC_MODEM_BCR_BLK_ARES_SHFT                                                           0x0

#define HWIO_MSS_CC_MODEM_GDSCR_ADDR                                                           (MSS_CC_MSS_CC_REG_REG_BASE      + 0x00000428)
#define HWIO_MSS_CC_MODEM_GDSCR_RMSK                                                           0xf8ffffff
#define HWIO_MSS_CC_MODEM_GDSCR_IN          \
        in_dword(HWIO_MSS_CC_MODEM_GDSCR_ADDR)
#define HWIO_MSS_CC_MODEM_GDSCR_INM(m)      \
        in_dword_masked(HWIO_MSS_CC_MODEM_GDSCR_ADDR, m)
#define HWIO_MSS_CC_MODEM_GDSCR_OUT(v)      \
        out_dword(HWIO_MSS_CC_MODEM_GDSCR_ADDR,v)
#define HWIO_MSS_CC_MODEM_GDSCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_CC_MODEM_GDSCR_ADDR,m,v,HWIO_MSS_CC_MODEM_GDSCR_IN)
#define HWIO_MSS_CC_MODEM_GDSCR_PWR_ON_BMSK                                                    0x80000000
#define HWIO_MSS_CC_MODEM_GDSCR_PWR_ON_SHFT                                                          0x1f
#define HWIO_MSS_CC_MODEM_GDSCR_GDSC_STATE_BMSK                                                0x78000000
#define HWIO_MSS_CC_MODEM_GDSCR_GDSC_STATE_SHFT                                                      0x1b
#define HWIO_MSS_CC_MODEM_GDSCR_EN_REST_WAIT_BMSK                                                0xf00000
#define HWIO_MSS_CC_MODEM_GDSCR_EN_REST_WAIT_SHFT                                                    0x14
#define HWIO_MSS_CC_MODEM_GDSCR_EN_FEW_WAIT_BMSK                                                  0xf0000
#define HWIO_MSS_CC_MODEM_GDSCR_EN_FEW_WAIT_SHFT                                                     0x10
#define HWIO_MSS_CC_MODEM_GDSCR_CLK_DIS_WAIT_BMSK                                                  0xf000
#define HWIO_MSS_CC_MODEM_GDSCR_CLK_DIS_WAIT_SHFT                                                     0xc
#define HWIO_MSS_CC_MODEM_GDSCR_RETAIN_FF_ENABLE_BMSK                                               0x800
#define HWIO_MSS_CC_MODEM_GDSCR_RETAIN_FF_ENABLE_SHFT                                                 0xb
#define HWIO_MSS_CC_MODEM_GDSCR_RESTORE_BMSK                                                        0x400
#define HWIO_MSS_CC_MODEM_GDSCR_RESTORE_SHFT                                                          0xa
#define HWIO_MSS_CC_MODEM_GDSCR_SAVE_BMSK                                                           0x200
#define HWIO_MSS_CC_MODEM_GDSCR_SAVE_SHFT                                                             0x9
#define HWIO_MSS_CC_MODEM_GDSCR_RETAIN_BMSK                                                         0x100
#define HWIO_MSS_CC_MODEM_GDSCR_RETAIN_SHFT                                                           0x8
#define HWIO_MSS_CC_MODEM_GDSCR_EN_REST_BMSK                                                         0x80
#define HWIO_MSS_CC_MODEM_GDSCR_EN_REST_SHFT                                                          0x7
#define HWIO_MSS_CC_MODEM_GDSCR_EN_FEW_BMSK                                                          0x40
#define HWIO_MSS_CC_MODEM_GDSCR_EN_FEW_SHFT                                                           0x6
#define HWIO_MSS_CC_MODEM_GDSCR_CLAMP_IO_BMSK                                                        0x20
#define HWIO_MSS_CC_MODEM_GDSCR_CLAMP_IO_SHFT                                                         0x5
#define HWIO_MSS_CC_MODEM_GDSCR_CLK_DISABLE_BMSK                                                     0x10
#define HWIO_MSS_CC_MODEM_GDSCR_CLK_DISABLE_SHFT                                                      0x4
#define HWIO_MSS_CC_MODEM_GDSCR_PD_ARES_BMSK                                                          0x8
#define HWIO_MSS_CC_MODEM_GDSCR_PD_ARES_SHFT                                                          0x3
#define HWIO_MSS_CC_MODEM_GDSCR_SW_OVERRIDE_BMSK                                                      0x4
#define HWIO_MSS_CC_MODEM_GDSCR_SW_OVERRIDE_SHFT                                                      0x2
#define HWIO_MSS_CC_MODEM_GDSCR_HW_CONTROL_BMSK                                                       0x2
#define HWIO_MSS_CC_MODEM_GDSCR_HW_CONTROL_SHFT                                                       0x1
#define HWIO_MSS_CC_MODEM_GDSCR_SW_COLLAPSE_BMSK                                                      0x1
#define HWIO_MSS_CC_MODEM_GDSCR_SW_COLLAPSE_SHFT                                                      0x0

#define HWIO_MSS_CC_MODEM_CFG_GDSCR_ADDR                                                       (MSS_CC_MSS_CC_REG_REG_BASE      + 0x0000042c)
#define HWIO_MSS_CC_MODEM_CFG_GDSCR_RMSK                                                       0xffffffff
#define HWIO_MSS_CC_MODEM_CFG_GDSCR_IN          \
        in_dword(HWIO_MSS_CC_MODEM_CFG_GDSCR_ADDR)
#define HWIO_MSS_CC_MODEM_CFG_GDSCR_INM(m)      \
        in_dword_masked(HWIO_MSS_CC_MODEM_CFG_GDSCR_ADDR, m)
#define HWIO_MSS_CC_MODEM_CFG_GDSCR_OUT(v)      \
        out_dword(HWIO_MSS_CC_MODEM_CFG_GDSCR_ADDR,v)
#define HWIO_MSS_CC_MODEM_CFG_GDSCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_CC_MODEM_CFG_GDSCR_ADDR,m,v,HWIO_MSS_CC_MODEM_CFG_GDSCR_IN)
#define HWIO_MSS_CC_MODEM_CFG_GDSCR_GDSC_SPARE_CTRL_IN_BMSK                                    0xf0000000
#define HWIO_MSS_CC_MODEM_CFG_GDSCR_GDSC_SPARE_CTRL_IN_SHFT                                          0x1c
#define HWIO_MSS_CC_MODEM_CFG_GDSCR_GDSC_SPARE_CTRL_OUT_BMSK                                    0xc000000
#define HWIO_MSS_CC_MODEM_CFG_GDSCR_GDSC_SPARE_CTRL_OUT_SHFT                                         0x1a
#define HWIO_MSS_CC_MODEM_CFG_GDSCR_GDSC_PWR_DWN_START_BMSK                                     0x2000000
#define HWIO_MSS_CC_MODEM_CFG_GDSCR_GDSC_PWR_DWN_START_SHFT                                          0x19
#define HWIO_MSS_CC_MODEM_CFG_GDSCR_GDSC_PWR_UP_START_BMSK                                      0x1000000
#define HWIO_MSS_CC_MODEM_CFG_GDSCR_GDSC_PWR_UP_START_SHFT                                           0x18
#define HWIO_MSS_CC_MODEM_CFG_GDSCR_GDSC_CFG_FSM_STATE_STATUS_BMSK                               0xf00000
#define HWIO_MSS_CC_MODEM_CFG_GDSCR_GDSC_CFG_FSM_STATE_STATUS_SHFT                                   0x14
#define HWIO_MSS_CC_MODEM_CFG_GDSCR_GDSC_MEM_PWR_ACK_STATUS_BMSK                                  0x80000
#define HWIO_MSS_CC_MODEM_CFG_GDSCR_GDSC_MEM_PWR_ACK_STATUS_SHFT                                     0x13
#define HWIO_MSS_CC_MODEM_CFG_GDSCR_GDSC_ENR_ACK_STATUS_BMSK                                      0x40000
#define HWIO_MSS_CC_MODEM_CFG_GDSCR_GDSC_ENR_ACK_STATUS_SHFT                                         0x12
#define HWIO_MSS_CC_MODEM_CFG_GDSCR_GDSC_ENF_ACK_STATUS_BMSK                                      0x20000
#define HWIO_MSS_CC_MODEM_CFG_GDSCR_GDSC_ENF_ACK_STATUS_SHFT                                         0x11
#define HWIO_MSS_CC_MODEM_CFG_GDSCR_GDSC_POWER_UP_COMPLETE_BMSK                                   0x10000
#define HWIO_MSS_CC_MODEM_CFG_GDSCR_GDSC_POWER_UP_COMPLETE_SHFT                                      0x10
#define HWIO_MSS_CC_MODEM_CFG_GDSCR_GDSC_POWER_DOWN_COMPLETE_BMSK                                  0x8000
#define HWIO_MSS_CC_MODEM_CFG_GDSCR_GDSC_POWER_DOWN_COMPLETE_SHFT                                     0xf
#define HWIO_MSS_CC_MODEM_CFG_GDSCR_SOFTWARE_CONTROL_OVERRIDE_BMSK                                 0x7800
#define HWIO_MSS_CC_MODEM_CFG_GDSCR_SOFTWARE_CONTROL_OVERRIDE_SHFT                                    0xb
#define HWIO_MSS_CC_MODEM_CFG_GDSCR_GDSC_HANDSHAKE_DIS_BMSK                                         0x400
#define HWIO_MSS_CC_MODEM_CFG_GDSCR_GDSC_HANDSHAKE_DIS_SHFT                                           0xa
#define HWIO_MSS_CC_MODEM_CFG_GDSCR_GDSC_MEM_PERI_FORCE_IN_SW_BMSK                                  0x200
#define HWIO_MSS_CC_MODEM_CFG_GDSCR_GDSC_MEM_PERI_FORCE_IN_SW_SHFT                                    0x9
#define HWIO_MSS_CC_MODEM_CFG_GDSCR_GDSC_MEM_CORE_FORCE_IN_SW_BMSK                                  0x100
#define HWIO_MSS_CC_MODEM_CFG_GDSCR_GDSC_MEM_CORE_FORCE_IN_SW_SHFT                                    0x8
#define HWIO_MSS_CC_MODEM_CFG_GDSCR_GDSC_PHASE_RESET_EN_SW_BMSK                                      0x80
#define HWIO_MSS_CC_MODEM_CFG_GDSCR_GDSC_PHASE_RESET_EN_SW_SHFT                                       0x7
#define HWIO_MSS_CC_MODEM_CFG_GDSCR_GDSC_PHASE_RESET_DELAY_COUNT_SW_BMSK                             0x60
#define HWIO_MSS_CC_MODEM_CFG_GDSCR_GDSC_PHASE_RESET_DELAY_COUNT_SW_SHFT                              0x5
#define HWIO_MSS_CC_MODEM_CFG_GDSCR_GDSC_PSCBC_PWR_DWN_SW_BMSK                                       0x10
#define HWIO_MSS_CC_MODEM_CFG_GDSCR_GDSC_PSCBC_PWR_DWN_SW_SHFT                                        0x4
#define HWIO_MSS_CC_MODEM_CFG_GDSCR_UNCLAMP_IO_SOFTWARE_OVERRIDE_BMSK                                 0x8
#define HWIO_MSS_CC_MODEM_CFG_GDSCR_UNCLAMP_IO_SOFTWARE_OVERRIDE_SHFT                                 0x3
#define HWIO_MSS_CC_MODEM_CFG_GDSCR_SAVE_RESTORE_SOFTWARE_OVERRIDE_BMSK                               0x4
#define HWIO_MSS_CC_MODEM_CFG_GDSCR_SAVE_RESTORE_SOFTWARE_OVERRIDE_SHFT                               0x2
#define HWIO_MSS_CC_MODEM_CFG_GDSCR_CLAMP_IO_SOFTWARE_OVERRIDE_BMSK                                   0x2
#define HWIO_MSS_CC_MODEM_CFG_GDSCR_CLAMP_IO_SOFTWARE_OVERRIDE_SHFT                                   0x1
#define HWIO_MSS_CC_MODEM_CFG_GDSCR_DISABLE_CLK_SOFTWARE_OVERRIDE_BMSK                                0x1
#define HWIO_MSS_CC_MODEM_CFG_GDSCR_DISABLE_CLK_SOFTWARE_OVERRIDE_SHFT                                0x0

#define HWIO_MSS_CC_MODEM_CFG2_GDSCR_ADDR                                                      (MSS_CC_MSS_CC_REG_REG_BASE      + 0x00000430)
#define HWIO_MSS_CC_MODEM_CFG2_GDSCR_RMSK                                                         0x1ffff
#define HWIO_MSS_CC_MODEM_CFG2_GDSCR_IN          \
        in_dword(HWIO_MSS_CC_MODEM_CFG2_GDSCR_ADDR)
#define HWIO_MSS_CC_MODEM_CFG2_GDSCR_INM(m)      \
        in_dword_masked(HWIO_MSS_CC_MODEM_CFG2_GDSCR_ADDR, m)
#define HWIO_MSS_CC_MODEM_CFG2_GDSCR_OUT(v)      \
        out_dword(HWIO_MSS_CC_MODEM_CFG2_GDSCR_ADDR,v)
#define HWIO_MSS_CC_MODEM_CFG2_GDSCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_CC_MODEM_CFG2_GDSCR_ADDR,m,v,HWIO_MSS_CC_MODEM_CFG2_GDSCR_IN)
#define HWIO_MSS_CC_MODEM_CFG2_GDSCR_GDSC_CLAMP_MEM_SW_BMSK                                       0x10000
#define HWIO_MSS_CC_MODEM_CFG2_GDSCR_GDSC_CLAMP_MEM_SW_SHFT                                          0x10
#define HWIO_MSS_CC_MODEM_CFG2_GDSCR_DLY_MEM_PWR_UP_BMSK                                           0xf000
#define HWIO_MSS_CC_MODEM_CFG2_GDSCR_DLY_MEM_PWR_UP_SHFT                                              0xc
#define HWIO_MSS_CC_MODEM_CFG2_GDSCR_DLY_DEASSERT_CLAMP_MEM_BMSK                                    0xf00
#define HWIO_MSS_CC_MODEM_CFG2_GDSCR_DLY_DEASSERT_CLAMP_MEM_SHFT                                      0x8
#define HWIO_MSS_CC_MODEM_CFG2_GDSCR_DLY_ASSERT_CLAMP_MEM_BMSK                                       0xf0
#define HWIO_MSS_CC_MODEM_CFG2_GDSCR_DLY_ASSERT_CLAMP_MEM_SHFT                                        0x4
#define HWIO_MSS_CC_MODEM_CFG2_GDSCR_MEM_PWR_DWN_TIMEOUT_BMSK                                         0xf
#define HWIO_MSS_CC_MODEM_CFG2_GDSCR_MEM_PWR_DWN_TIMEOUT_SHFT                                         0x0

#define HWIO_MSS_CC_MODEM_XO_CX_TO_LS_CBCR_ADDR                                                (MSS_CC_MSS_CC_REG_REG_BASE      + 0x00000438)
#define HWIO_MSS_CC_MODEM_XO_CX_TO_LS_CBCR_RMSK                                                0x80000005
#define HWIO_MSS_CC_MODEM_XO_CX_TO_LS_CBCR_IN          \
        in_dword(HWIO_MSS_CC_MODEM_XO_CX_TO_LS_CBCR_ADDR)
#define HWIO_MSS_CC_MODEM_XO_CX_TO_LS_CBCR_INM(m)      \
        in_dword_masked(HWIO_MSS_CC_MODEM_XO_CX_TO_LS_CBCR_ADDR, m)
#define HWIO_MSS_CC_MODEM_XO_CX_TO_LS_CBCR_OUT(v)      \
        out_dword(HWIO_MSS_CC_MODEM_XO_CX_TO_LS_CBCR_ADDR,v)
#define HWIO_MSS_CC_MODEM_XO_CX_TO_LS_CBCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_CC_MODEM_XO_CX_TO_LS_CBCR_ADDR,m,v,HWIO_MSS_CC_MODEM_XO_CX_TO_LS_CBCR_IN)
#define HWIO_MSS_CC_MODEM_XO_CX_TO_LS_CBCR_CLK_OFF_BMSK                                        0x80000000
#define HWIO_MSS_CC_MODEM_XO_CX_TO_LS_CBCR_CLK_OFF_SHFT                                              0x1f
#define HWIO_MSS_CC_MODEM_XO_CX_TO_LS_CBCR_CLK_ARES_BMSK                                              0x4
#define HWIO_MSS_CC_MODEM_XO_CX_TO_LS_CBCR_CLK_ARES_SHFT                                              0x2
#define HWIO_MSS_CC_MODEM_XO_CX_TO_LS_CBCR_CLK_ENABLE_BMSK                                            0x1
#define HWIO_MSS_CC_MODEM_XO_CX_TO_LS_CBCR_CLK_ENABLE_SHFT                                            0x0

#define HWIO_MSS_CC_MODEM_SNOC_AXI_CBCR_ADDR                                                   (MSS_CC_MSS_CC_REG_REG_BASE      + 0x0000044c)
#define HWIO_MSS_CC_MODEM_SNOC_AXI_CBCR_RMSK                                                   0x80000005
#define HWIO_MSS_CC_MODEM_SNOC_AXI_CBCR_IN          \
        in_dword(HWIO_MSS_CC_MODEM_SNOC_AXI_CBCR_ADDR)
#define HWIO_MSS_CC_MODEM_SNOC_AXI_CBCR_INM(m)      \
        in_dword_masked(HWIO_MSS_CC_MODEM_SNOC_AXI_CBCR_ADDR, m)
#define HWIO_MSS_CC_MODEM_SNOC_AXI_CBCR_OUT(v)      \
        out_dword(HWIO_MSS_CC_MODEM_SNOC_AXI_CBCR_ADDR,v)
#define HWIO_MSS_CC_MODEM_SNOC_AXI_CBCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_CC_MODEM_SNOC_AXI_CBCR_ADDR,m,v,HWIO_MSS_CC_MODEM_SNOC_AXI_CBCR_IN)
#define HWIO_MSS_CC_MODEM_SNOC_AXI_CBCR_CLK_OFF_BMSK                                           0x80000000
#define HWIO_MSS_CC_MODEM_SNOC_AXI_CBCR_CLK_OFF_SHFT                                                 0x1f
#define HWIO_MSS_CC_MODEM_SNOC_AXI_CBCR_CLK_ARES_BMSK                                                 0x4
#define HWIO_MSS_CC_MODEM_SNOC_AXI_CBCR_CLK_ARES_SHFT                                                 0x2
#define HWIO_MSS_CC_MODEM_SNOC_AXI_CBCR_CLK_ENABLE_BMSK                                               0x1
#define HWIO_MSS_CC_MODEM_SNOC_AXI_CBCR_CLK_ENABLE_SHFT                                               0x0

#define HWIO_MSS_CC_MODEM_BRIDGE_CX_BUS_CBCR_ADDR                                              (MSS_CC_MSS_CC_REG_REG_BASE      + 0x00000460)
#define HWIO_MSS_CC_MODEM_BRIDGE_CX_BUS_CBCR_RMSK                                              0x80000005
#define HWIO_MSS_CC_MODEM_BRIDGE_CX_BUS_CBCR_IN          \
        in_dword(HWIO_MSS_CC_MODEM_BRIDGE_CX_BUS_CBCR_ADDR)
#define HWIO_MSS_CC_MODEM_BRIDGE_CX_BUS_CBCR_INM(m)      \
        in_dword_masked(HWIO_MSS_CC_MODEM_BRIDGE_CX_BUS_CBCR_ADDR, m)
#define HWIO_MSS_CC_MODEM_BRIDGE_CX_BUS_CBCR_OUT(v)      \
        out_dword(HWIO_MSS_CC_MODEM_BRIDGE_CX_BUS_CBCR_ADDR,v)
#define HWIO_MSS_CC_MODEM_BRIDGE_CX_BUS_CBCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_CC_MODEM_BRIDGE_CX_BUS_CBCR_ADDR,m,v,HWIO_MSS_CC_MODEM_BRIDGE_CX_BUS_CBCR_IN)
#define HWIO_MSS_CC_MODEM_BRIDGE_CX_BUS_CBCR_CLK_OFF_BMSK                                      0x80000000
#define HWIO_MSS_CC_MODEM_BRIDGE_CX_BUS_CBCR_CLK_OFF_SHFT                                            0x1f
#define HWIO_MSS_CC_MODEM_BRIDGE_CX_BUS_CBCR_CLK_ARES_BMSK                                            0x4
#define HWIO_MSS_CC_MODEM_BRIDGE_CX_BUS_CBCR_CLK_ARES_SHFT                                            0x2
#define HWIO_MSS_CC_MODEM_BRIDGE_CX_BUS_CBCR_CLK_ENABLE_BMSK                                          0x1
#define HWIO_MSS_CC_MODEM_BRIDGE_CX_BUS_CBCR_CLK_ENABLE_SHFT                                          0x0

#define HWIO_MSS_CC_MODEM_ATB_DEBUG_CBCR_ADDR                                                  (MSS_CC_MSS_CC_REG_REG_BASE      + 0x00000474)
#define HWIO_MSS_CC_MODEM_ATB_DEBUG_CBCR_RMSK                                                  0x80000005
#define HWIO_MSS_CC_MODEM_ATB_DEBUG_CBCR_IN          \
        in_dword(HWIO_MSS_CC_MODEM_ATB_DEBUG_CBCR_ADDR)
#define HWIO_MSS_CC_MODEM_ATB_DEBUG_CBCR_INM(m)      \
        in_dword_masked(HWIO_MSS_CC_MODEM_ATB_DEBUG_CBCR_ADDR, m)
#define HWIO_MSS_CC_MODEM_ATB_DEBUG_CBCR_OUT(v)      \
        out_dword(HWIO_MSS_CC_MODEM_ATB_DEBUG_CBCR_ADDR,v)
#define HWIO_MSS_CC_MODEM_ATB_DEBUG_CBCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_CC_MODEM_ATB_DEBUG_CBCR_ADDR,m,v,HWIO_MSS_CC_MODEM_ATB_DEBUG_CBCR_IN)
#define HWIO_MSS_CC_MODEM_ATB_DEBUG_CBCR_CLK_OFF_BMSK                                          0x80000000
#define HWIO_MSS_CC_MODEM_ATB_DEBUG_CBCR_CLK_OFF_SHFT                                                0x1f
#define HWIO_MSS_CC_MODEM_ATB_DEBUG_CBCR_CLK_ARES_BMSK                                                0x4
#define HWIO_MSS_CC_MODEM_ATB_DEBUG_CBCR_CLK_ARES_SHFT                                                0x2
#define HWIO_MSS_CC_MODEM_ATB_DEBUG_CBCR_CLK_ENABLE_BMSK                                              0x1
#define HWIO_MSS_CC_MODEM_ATB_DEBUG_CBCR_CLK_ENABLE_SHFT                                              0x0

#define HWIO_MSS_CC_MODEM_RFFE_TWO_XO_CBCR_ADDR                                                (MSS_CC_MSS_CC_REG_REG_BASE      + 0x00000488)
#define HWIO_MSS_CC_MODEM_RFFE_TWO_XO_CBCR_RMSK                                                0x80000005
#define HWIO_MSS_CC_MODEM_RFFE_TWO_XO_CBCR_IN          \
        in_dword(HWIO_MSS_CC_MODEM_RFFE_TWO_XO_CBCR_ADDR)
#define HWIO_MSS_CC_MODEM_RFFE_TWO_XO_CBCR_INM(m)      \
        in_dword_masked(HWIO_MSS_CC_MODEM_RFFE_TWO_XO_CBCR_ADDR, m)
#define HWIO_MSS_CC_MODEM_RFFE_TWO_XO_CBCR_OUT(v)      \
        out_dword(HWIO_MSS_CC_MODEM_RFFE_TWO_XO_CBCR_ADDR,v)
#define HWIO_MSS_CC_MODEM_RFFE_TWO_XO_CBCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_CC_MODEM_RFFE_TWO_XO_CBCR_ADDR,m,v,HWIO_MSS_CC_MODEM_RFFE_TWO_XO_CBCR_IN)
#define HWIO_MSS_CC_MODEM_RFFE_TWO_XO_CBCR_CLK_OFF_BMSK                                        0x80000000
#define HWIO_MSS_CC_MODEM_RFFE_TWO_XO_CBCR_CLK_OFF_SHFT                                              0x1f
#define HWIO_MSS_CC_MODEM_RFFE_TWO_XO_CBCR_CLK_ARES_BMSK                                              0x4
#define HWIO_MSS_CC_MODEM_RFFE_TWO_XO_CBCR_CLK_ARES_SHFT                                              0x2
#define HWIO_MSS_CC_MODEM_RFFE_TWO_XO_CBCR_CLK_ENABLE_BMSK                                            0x1
#define HWIO_MSS_CC_MODEM_RFFE_TWO_XO_CBCR_CLK_ENABLE_SHFT                                            0x0

#define HWIO_MSS_CC_MODEM_CORE_CBCR_ADDR                                                       (MSS_CC_MSS_CC_REG_REG_BASE      + 0x0000049c)
#define HWIO_MSS_CC_MODEM_CORE_CBCR_RMSK                                                       0x80007ff5
#define HWIO_MSS_CC_MODEM_CORE_CBCR_IN          \
        in_dword(HWIO_MSS_CC_MODEM_CORE_CBCR_ADDR)
#define HWIO_MSS_CC_MODEM_CORE_CBCR_INM(m)      \
        in_dword_masked(HWIO_MSS_CC_MODEM_CORE_CBCR_ADDR, m)
#define HWIO_MSS_CC_MODEM_CORE_CBCR_OUT(v)      \
        out_dword(HWIO_MSS_CC_MODEM_CORE_CBCR_ADDR,v)
#define HWIO_MSS_CC_MODEM_CORE_CBCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_CC_MODEM_CORE_CBCR_ADDR,m,v,HWIO_MSS_CC_MODEM_CORE_CBCR_IN)
#define HWIO_MSS_CC_MODEM_CORE_CBCR_CLK_OFF_BMSK                                               0x80000000
#define HWIO_MSS_CC_MODEM_CORE_CBCR_CLK_OFF_SHFT                                                     0x1f
#define HWIO_MSS_CC_MODEM_CORE_CBCR_FORCE_MEM_CORE_ON_BMSK                                         0x4000
#define HWIO_MSS_CC_MODEM_CORE_CBCR_FORCE_MEM_CORE_ON_SHFT                                            0xe
#define HWIO_MSS_CC_MODEM_CORE_CBCR_FORCE_MEM_PERIPH_ON_BMSK                                       0x2000
#define HWIO_MSS_CC_MODEM_CORE_CBCR_FORCE_MEM_PERIPH_ON_SHFT                                          0xd
#define HWIO_MSS_CC_MODEM_CORE_CBCR_FORCE_MEM_PERIPH_OFF_BMSK                                      0x1000
#define HWIO_MSS_CC_MODEM_CORE_CBCR_FORCE_MEM_PERIPH_OFF_SHFT                                         0xc
#define HWIO_MSS_CC_MODEM_CORE_CBCR_WAKEUP_BMSK                                                     0xf00
#define HWIO_MSS_CC_MODEM_CORE_CBCR_WAKEUP_SHFT                                                       0x8
#define HWIO_MSS_CC_MODEM_CORE_CBCR_SLEEP_BMSK                                                       0xf0
#define HWIO_MSS_CC_MODEM_CORE_CBCR_SLEEP_SHFT                                                        0x4
#define HWIO_MSS_CC_MODEM_CORE_CBCR_CLK_ARES_BMSK                                                     0x4
#define HWIO_MSS_CC_MODEM_CORE_CBCR_CLK_ARES_SHFT                                                     0x2
#define HWIO_MSS_CC_MODEM_CORE_CBCR_CLK_ENABLE_BMSK                                                   0x1
#define HWIO_MSS_CC_MODEM_CORE_CBCR_CLK_ENABLE_SHFT                                                   0x0

#define HWIO_MSS_CC_MODEM_CCS_CBCR_ADDR                                                        (MSS_CC_MSS_CC_REG_REG_BASE      + 0x000004b0)
#define HWIO_MSS_CC_MODEM_CCS_CBCR_RMSK                                                        0x80007ff5
#define HWIO_MSS_CC_MODEM_CCS_CBCR_IN          \
        in_dword(HWIO_MSS_CC_MODEM_CCS_CBCR_ADDR)
#define HWIO_MSS_CC_MODEM_CCS_CBCR_INM(m)      \
        in_dword_masked(HWIO_MSS_CC_MODEM_CCS_CBCR_ADDR, m)
#define HWIO_MSS_CC_MODEM_CCS_CBCR_OUT(v)      \
        out_dword(HWIO_MSS_CC_MODEM_CCS_CBCR_ADDR,v)
#define HWIO_MSS_CC_MODEM_CCS_CBCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_CC_MODEM_CCS_CBCR_ADDR,m,v,HWIO_MSS_CC_MODEM_CCS_CBCR_IN)
#define HWIO_MSS_CC_MODEM_CCS_CBCR_CLK_OFF_BMSK                                                0x80000000
#define HWIO_MSS_CC_MODEM_CCS_CBCR_CLK_OFF_SHFT                                                      0x1f
#define HWIO_MSS_CC_MODEM_CCS_CBCR_FORCE_MEM_CORE_ON_BMSK                                          0x4000
#define HWIO_MSS_CC_MODEM_CCS_CBCR_FORCE_MEM_CORE_ON_SHFT                                             0xe
#define HWIO_MSS_CC_MODEM_CCS_CBCR_FORCE_MEM_PERIPH_ON_BMSK                                        0x2000
#define HWIO_MSS_CC_MODEM_CCS_CBCR_FORCE_MEM_PERIPH_ON_SHFT                                           0xd
#define HWIO_MSS_CC_MODEM_CCS_CBCR_FORCE_MEM_PERIPH_OFF_BMSK                                       0x1000
#define HWIO_MSS_CC_MODEM_CCS_CBCR_FORCE_MEM_PERIPH_OFF_SHFT                                          0xc
#define HWIO_MSS_CC_MODEM_CCS_CBCR_WAKEUP_BMSK                                                      0xf00
#define HWIO_MSS_CC_MODEM_CCS_CBCR_WAKEUP_SHFT                                                        0x8
#define HWIO_MSS_CC_MODEM_CCS_CBCR_SLEEP_BMSK                                                        0xf0
#define HWIO_MSS_CC_MODEM_CCS_CBCR_SLEEP_SHFT                                                         0x4
#define HWIO_MSS_CC_MODEM_CCS_CBCR_CLK_ARES_BMSK                                                      0x4
#define HWIO_MSS_CC_MODEM_CCS_CBCR_CLK_ARES_SHFT                                                      0x2
#define HWIO_MSS_CC_MODEM_CCS_CBCR_CLK_ENABLE_BMSK                                                    0x1
#define HWIO_MSS_CC_MODEM_CCS_CBCR_CLK_ENABLE_SHFT                                                    0x0

#define HWIO_MSS_CC_MODEM_BUS_CBCR_ADDR                                                        (MSS_CC_MSS_CC_REG_REG_BASE      + 0x000004c4)
#define HWIO_MSS_CC_MODEM_BUS_CBCR_RMSK                                                        0x80000007
#define HWIO_MSS_CC_MODEM_BUS_CBCR_IN          \
        in_dword(HWIO_MSS_CC_MODEM_BUS_CBCR_ADDR)
#define HWIO_MSS_CC_MODEM_BUS_CBCR_INM(m)      \
        in_dword_masked(HWIO_MSS_CC_MODEM_BUS_CBCR_ADDR, m)
#define HWIO_MSS_CC_MODEM_BUS_CBCR_OUT(v)      \
        out_dword(HWIO_MSS_CC_MODEM_BUS_CBCR_ADDR,v)
#define HWIO_MSS_CC_MODEM_BUS_CBCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_CC_MODEM_BUS_CBCR_ADDR,m,v,HWIO_MSS_CC_MODEM_BUS_CBCR_IN)
#define HWIO_MSS_CC_MODEM_BUS_CBCR_CLK_OFF_BMSK                                                0x80000000
#define HWIO_MSS_CC_MODEM_BUS_CBCR_CLK_OFF_SHFT                                                      0x1f
#define HWIO_MSS_CC_MODEM_BUS_CBCR_CLK_ARES_BMSK                                                      0x4
#define HWIO_MSS_CC_MODEM_BUS_CBCR_CLK_ARES_SHFT                                                      0x2
#define HWIO_MSS_CC_MODEM_BUS_CBCR_HW_CTL_BMSK                                                        0x2
#define HWIO_MSS_CC_MODEM_BUS_CBCR_HW_CTL_SHFT                                                        0x1
#define HWIO_MSS_CC_MODEM_BUS_CBCR_CLK_ENABLE_BMSK                                                    0x1
#define HWIO_MSS_CC_MODEM_BUS_CBCR_CLK_ENABLE_SHFT                                                    0x0

#define HWIO_MSS_CC_MODEM_TX_CORE_BCR_ADDR                                                     (MSS_CC_MSS_CC_REG_REG_BASE      + 0x000004d8)
#define HWIO_MSS_CC_MODEM_TX_CORE_BCR_RMSK                                                            0x1
#define HWIO_MSS_CC_MODEM_TX_CORE_BCR_IN          \
        in_dword(HWIO_MSS_CC_MODEM_TX_CORE_BCR_ADDR)
#define HWIO_MSS_CC_MODEM_TX_CORE_BCR_INM(m)      \
        in_dword_masked(HWIO_MSS_CC_MODEM_TX_CORE_BCR_ADDR, m)
#define HWIO_MSS_CC_MODEM_TX_CORE_BCR_OUT(v)      \
        out_dword(HWIO_MSS_CC_MODEM_TX_CORE_BCR_ADDR,v)
#define HWIO_MSS_CC_MODEM_TX_CORE_BCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_CC_MODEM_TX_CORE_BCR_ADDR,m,v,HWIO_MSS_CC_MODEM_TX_CORE_BCR_IN)
#define HWIO_MSS_CC_MODEM_TX_CORE_BCR_BLK_ARES_BMSK                                                   0x1
#define HWIO_MSS_CC_MODEM_TX_CORE_BCR_BLK_ARES_SHFT                                                   0x0

#define HWIO_MSS_CC_MODEM_TX_CORE_GDSCR_ADDR                                                   (MSS_CC_MSS_CC_REG_REG_BASE      + 0x000004dc)
#define HWIO_MSS_CC_MODEM_TX_CORE_GDSCR_RMSK                                                   0xf8ffffff
#define HWIO_MSS_CC_MODEM_TX_CORE_GDSCR_IN          \
        in_dword(HWIO_MSS_CC_MODEM_TX_CORE_GDSCR_ADDR)
#define HWIO_MSS_CC_MODEM_TX_CORE_GDSCR_INM(m)      \
        in_dword_masked(HWIO_MSS_CC_MODEM_TX_CORE_GDSCR_ADDR, m)
#define HWIO_MSS_CC_MODEM_TX_CORE_GDSCR_OUT(v)      \
        out_dword(HWIO_MSS_CC_MODEM_TX_CORE_GDSCR_ADDR,v)
#define HWIO_MSS_CC_MODEM_TX_CORE_GDSCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_CC_MODEM_TX_CORE_GDSCR_ADDR,m,v,HWIO_MSS_CC_MODEM_TX_CORE_GDSCR_IN)
#define HWIO_MSS_CC_MODEM_TX_CORE_GDSCR_PWR_ON_BMSK                                            0x80000000
#define HWIO_MSS_CC_MODEM_TX_CORE_GDSCR_PWR_ON_SHFT                                                  0x1f
#define HWIO_MSS_CC_MODEM_TX_CORE_GDSCR_GDSC_STATE_BMSK                                        0x78000000
#define HWIO_MSS_CC_MODEM_TX_CORE_GDSCR_GDSC_STATE_SHFT                                              0x1b
#define HWIO_MSS_CC_MODEM_TX_CORE_GDSCR_EN_REST_WAIT_BMSK                                        0xf00000
#define HWIO_MSS_CC_MODEM_TX_CORE_GDSCR_EN_REST_WAIT_SHFT                                            0x14
#define HWIO_MSS_CC_MODEM_TX_CORE_GDSCR_EN_FEW_WAIT_BMSK                                          0xf0000
#define HWIO_MSS_CC_MODEM_TX_CORE_GDSCR_EN_FEW_WAIT_SHFT                                             0x10
#define HWIO_MSS_CC_MODEM_TX_CORE_GDSCR_CLK_DIS_WAIT_BMSK                                          0xf000
#define HWIO_MSS_CC_MODEM_TX_CORE_GDSCR_CLK_DIS_WAIT_SHFT                                             0xc
#define HWIO_MSS_CC_MODEM_TX_CORE_GDSCR_RETAIN_FF_ENABLE_BMSK                                       0x800
#define HWIO_MSS_CC_MODEM_TX_CORE_GDSCR_RETAIN_FF_ENABLE_SHFT                                         0xb
#define HWIO_MSS_CC_MODEM_TX_CORE_GDSCR_RESTORE_BMSK                                                0x400
#define HWIO_MSS_CC_MODEM_TX_CORE_GDSCR_RESTORE_SHFT                                                  0xa
#define HWIO_MSS_CC_MODEM_TX_CORE_GDSCR_SAVE_BMSK                                                   0x200
#define HWIO_MSS_CC_MODEM_TX_CORE_GDSCR_SAVE_SHFT                                                     0x9
#define HWIO_MSS_CC_MODEM_TX_CORE_GDSCR_RETAIN_BMSK                                                 0x100
#define HWIO_MSS_CC_MODEM_TX_CORE_GDSCR_RETAIN_SHFT                                                   0x8
#define HWIO_MSS_CC_MODEM_TX_CORE_GDSCR_EN_REST_BMSK                                                 0x80
#define HWIO_MSS_CC_MODEM_TX_CORE_GDSCR_EN_REST_SHFT                                                  0x7
#define HWIO_MSS_CC_MODEM_TX_CORE_GDSCR_EN_FEW_BMSK                                                  0x40
#define HWIO_MSS_CC_MODEM_TX_CORE_GDSCR_EN_FEW_SHFT                                                   0x6
#define HWIO_MSS_CC_MODEM_TX_CORE_GDSCR_CLAMP_IO_BMSK                                                0x20
#define HWIO_MSS_CC_MODEM_TX_CORE_GDSCR_CLAMP_IO_SHFT                                                 0x5
#define HWIO_MSS_CC_MODEM_TX_CORE_GDSCR_CLK_DISABLE_BMSK                                             0x10
#define HWIO_MSS_CC_MODEM_TX_CORE_GDSCR_CLK_DISABLE_SHFT                                              0x4
#define HWIO_MSS_CC_MODEM_TX_CORE_GDSCR_PD_ARES_BMSK                                                  0x8
#define HWIO_MSS_CC_MODEM_TX_CORE_GDSCR_PD_ARES_SHFT                                                  0x3
#define HWIO_MSS_CC_MODEM_TX_CORE_GDSCR_SW_OVERRIDE_BMSK                                              0x4
#define HWIO_MSS_CC_MODEM_TX_CORE_GDSCR_SW_OVERRIDE_SHFT                                              0x2
#define HWIO_MSS_CC_MODEM_TX_CORE_GDSCR_HW_CONTROL_BMSK                                               0x2
#define HWIO_MSS_CC_MODEM_TX_CORE_GDSCR_HW_CONTROL_SHFT                                               0x1
#define HWIO_MSS_CC_MODEM_TX_CORE_GDSCR_SW_COLLAPSE_BMSK                                              0x1
#define HWIO_MSS_CC_MODEM_TX_CORE_GDSCR_SW_COLLAPSE_SHFT                                              0x0

#define HWIO_MSS_CC_MODEM_TX_CORE_CFG_GDSCR_ADDR                                               (MSS_CC_MSS_CC_REG_REG_BASE      + 0x000004e0)
#define HWIO_MSS_CC_MODEM_TX_CORE_CFG_GDSCR_RMSK                                               0xffffffff
#define HWIO_MSS_CC_MODEM_TX_CORE_CFG_GDSCR_IN          \
        in_dword(HWIO_MSS_CC_MODEM_TX_CORE_CFG_GDSCR_ADDR)
#define HWIO_MSS_CC_MODEM_TX_CORE_CFG_GDSCR_INM(m)      \
        in_dword_masked(HWIO_MSS_CC_MODEM_TX_CORE_CFG_GDSCR_ADDR, m)
#define HWIO_MSS_CC_MODEM_TX_CORE_CFG_GDSCR_OUT(v)      \
        out_dword(HWIO_MSS_CC_MODEM_TX_CORE_CFG_GDSCR_ADDR,v)
#define HWIO_MSS_CC_MODEM_TX_CORE_CFG_GDSCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_CC_MODEM_TX_CORE_CFG_GDSCR_ADDR,m,v,HWIO_MSS_CC_MODEM_TX_CORE_CFG_GDSCR_IN)
#define HWIO_MSS_CC_MODEM_TX_CORE_CFG_GDSCR_GDSC_SPARE_CTRL_IN_BMSK                            0xf0000000
#define HWIO_MSS_CC_MODEM_TX_CORE_CFG_GDSCR_GDSC_SPARE_CTRL_IN_SHFT                                  0x1c
#define HWIO_MSS_CC_MODEM_TX_CORE_CFG_GDSCR_GDSC_SPARE_CTRL_OUT_BMSK                            0xc000000
#define HWIO_MSS_CC_MODEM_TX_CORE_CFG_GDSCR_GDSC_SPARE_CTRL_OUT_SHFT                                 0x1a
#define HWIO_MSS_CC_MODEM_TX_CORE_CFG_GDSCR_GDSC_PWR_DWN_START_BMSK                             0x2000000
#define HWIO_MSS_CC_MODEM_TX_CORE_CFG_GDSCR_GDSC_PWR_DWN_START_SHFT                                  0x19
#define HWIO_MSS_CC_MODEM_TX_CORE_CFG_GDSCR_GDSC_PWR_UP_START_BMSK                              0x1000000
#define HWIO_MSS_CC_MODEM_TX_CORE_CFG_GDSCR_GDSC_PWR_UP_START_SHFT                                   0x18
#define HWIO_MSS_CC_MODEM_TX_CORE_CFG_GDSCR_GDSC_CFG_FSM_STATE_STATUS_BMSK                       0xf00000
#define HWIO_MSS_CC_MODEM_TX_CORE_CFG_GDSCR_GDSC_CFG_FSM_STATE_STATUS_SHFT                           0x14
#define HWIO_MSS_CC_MODEM_TX_CORE_CFG_GDSCR_GDSC_MEM_PWR_ACK_STATUS_BMSK                          0x80000
#define HWIO_MSS_CC_MODEM_TX_CORE_CFG_GDSCR_GDSC_MEM_PWR_ACK_STATUS_SHFT                             0x13
#define HWIO_MSS_CC_MODEM_TX_CORE_CFG_GDSCR_GDSC_ENR_ACK_STATUS_BMSK                              0x40000
#define HWIO_MSS_CC_MODEM_TX_CORE_CFG_GDSCR_GDSC_ENR_ACK_STATUS_SHFT                                 0x12
#define HWIO_MSS_CC_MODEM_TX_CORE_CFG_GDSCR_GDSC_ENF_ACK_STATUS_BMSK                              0x20000
#define HWIO_MSS_CC_MODEM_TX_CORE_CFG_GDSCR_GDSC_ENF_ACK_STATUS_SHFT                                 0x11
#define HWIO_MSS_CC_MODEM_TX_CORE_CFG_GDSCR_GDSC_POWER_UP_COMPLETE_BMSK                           0x10000
#define HWIO_MSS_CC_MODEM_TX_CORE_CFG_GDSCR_GDSC_POWER_UP_COMPLETE_SHFT                              0x10
#define HWIO_MSS_CC_MODEM_TX_CORE_CFG_GDSCR_GDSC_POWER_DOWN_COMPLETE_BMSK                          0x8000
#define HWIO_MSS_CC_MODEM_TX_CORE_CFG_GDSCR_GDSC_POWER_DOWN_COMPLETE_SHFT                             0xf
#define HWIO_MSS_CC_MODEM_TX_CORE_CFG_GDSCR_SOFTWARE_CONTROL_OVERRIDE_BMSK                         0x7800
#define HWIO_MSS_CC_MODEM_TX_CORE_CFG_GDSCR_SOFTWARE_CONTROL_OVERRIDE_SHFT                            0xb
#define HWIO_MSS_CC_MODEM_TX_CORE_CFG_GDSCR_GDSC_HANDSHAKE_DIS_BMSK                                 0x400
#define HWIO_MSS_CC_MODEM_TX_CORE_CFG_GDSCR_GDSC_HANDSHAKE_DIS_SHFT                                   0xa
#define HWIO_MSS_CC_MODEM_TX_CORE_CFG_GDSCR_GDSC_MEM_PERI_FORCE_IN_SW_BMSK                          0x200
#define HWIO_MSS_CC_MODEM_TX_CORE_CFG_GDSCR_GDSC_MEM_PERI_FORCE_IN_SW_SHFT                            0x9
#define HWIO_MSS_CC_MODEM_TX_CORE_CFG_GDSCR_GDSC_MEM_CORE_FORCE_IN_SW_BMSK                          0x100
#define HWIO_MSS_CC_MODEM_TX_CORE_CFG_GDSCR_GDSC_MEM_CORE_FORCE_IN_SW_SHFT                            0x8
#define HWIO_MSS_CC_MODEM_TX_CORE_CFG_GDSCR_GDSC_PHASE_RESET_EN_SW_BMSK                              0x80
#define HWIO_MSS_CC_MODEM_TX_CORE_CFG_GDSCR_GDSC_PHASE_RESET_EN_SW_SHFT                               0x7
#define HWIO_MSS_CC_MODEM_TX_CORE_CFG_GDSCR_GDSC_PHASE_RESET_DELAY_COUNT_SW_BMSK                     0x60
#define HWIO_MSS_CC_MODEM_TX_CORE_CFG_GDSCR_GDSC_PHASE_RESET_DELAY_COUNT_SW_SHFT                      0x5
#define HWIO_MSS_CC_MODEM_TX_CORE_CFG_GDSCR_GDSC_PSCBC_PWR_DWN_SW_BMSK                               0x10
#define HWIO_MSS_CC_MODEM_TX_CORE_CFG_GDSCR_GDSC_PSCBC_PWR_DWN_SW_SHFT                                0x4
#define HWIO_MSS_CC_MODEM_TX_CORE_CFG_GDSCR_UNCLAMP_IO_SOFTWARE_OVERRIDE_BMSK                         0x8
#define HWIO_MSS_CC_MODEM_TX_CORE_CFG_GDSCR_UNCLAMP_IO_SOFTWARE_OVERRIDE_SHFT                         0x3
#define HWIO_MSS_CC_MODEM_TX_CORE_CFG_GDSCR_SAVE_RESTORE_SOFTWARE_OVERRIDE_BMSK                       0x4
#define HWIO_MSS_CC_MODEM_TX_CORE_CFG_GDSCR_SAVE_RESTORE_SOFTWARE_OVERRIDE_SHFT                       0x2
#define HWIO_MSS_CC_MODEM_TX_CORE_CFG_GDSCR_CLAMP_IO_SOFTWARE_OVERRIDE_BMSK                           0x2
#define HWIO_MSS_CC_MODEM_TX_CORE_CFG_GDSCR_CLAMP_IO_SOFTWARE_OVERRIDE_SHFT                           0x1
#define HWIO_MSS_CC_MODEM_TX_CORE_CFG_GDSCR_DISABLE_CLK_SOFTWARE_OVERRIDE_BMSK                        0x1
#define HWIO_MSS_CC_MODEM_TX_CORE_CFG_GDSCR_DISABLE_CLK_SOFTWARE_OVERRIDE_SHFT                        0x0

#define HWIO_MSS_CC_MODEM_TX_CORE_CFG2_GDSCR_ADDR                                              (MSS_CC_MSS_CC_REG_REG_BASE      + 0x000004e4)
#define HWIO_MSS_CC_MODEM_TX_CORE_CFG2_GDSCR_RMSK                                                 0x1ffff
#define HWIO_MSS_CC_MODEM_TX_CORE_CFG2_GDSCR_IN          \
        in_dword(HWIO_MSS_CC_MODEM_TX_CORE_CFG2_GDSCR_ADDR)
#define HWIO_MSS_CC_MODEM_TX_CORE_CFG2_GDSCR_INM(m)      \
        in_dword_masked(HWIO_MSS_CC_MODEM_TX_CORE_CFG2_GDSCR_ADDR, m)
#define HWIO_MSS_CC_MODEM_TX_CORE_CFG2_GDSCR_OUT(v)      \
        out_dword(HWIO_MSS_CC_MODEM_TX_CORE_CFG2_GDSCR_ADDR,v)
#define HWIO_MSS_CC_MODEM_TX_CORE_CFG2_GDSCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_CC_MODEM_TX_CORE_CFG2_GDSCR_ADDR,m,v,HWIO_MSS_CC_MODEM_TX_CORE_CFG2_GDSCR_IN)
#define HWIO_MSS_CC_MODEM_TX_CORE_CFG2_GDSCR_GDSC_CLAMP_MEM_SW_BMSK                               0x10000
#define HWIO_MSS_CC_MODEM_TX_CORE_CFG2_GDSCR_GDSC_CLAMP_MEM_SW_SHFT                                  0x10
#define HWIO_MSS_CC_MODEM_TX_CORE_CFG2_GDSCR_DLY_MEM_PWR_UP_BMSK                                   0xf000
#define HWIO_MSS_CC_MODEM_TX_CORE_CFG2_GDSCR_DLY_MEM_PWR_UP_SHFT                                      0xc
#define HWIO_MSS_CC_MODEM_TX_CORE_CFG2_GDSCR_DLY_DEASSERT_CLAMP_MEM_BMSK                            0xf00
#define HWIO_MSS_CC_MODEM_TX_CORE_CFG2_GDSCR_DLY_DEASSERT_CLAMP_MEM_SHFT                              0x8
#define HWIO_MSS_CC_MODEM_TX_CORE_CFG2_GDSCR_DLY_ASSERT_CLAMP_MEM_BMSK                               0xf0
#define HWIO_MSS_CC_MODEM_TX_CORE_CFG2_GDSCR_DLY_ASSERT_CLAMP_MEM_SHFT                                0x4
#define HWIO_MSS_CC_MODEM_TX_CORE_CFG2_GDSCR_MEM_PWR_DWN_TIMEOUT_BMSK                                 0xf
#define HWIO_MSS_CC_MODEM_TX_CORE_CFG2_GDSCR_MEM_PWR_DWN_TIMEOUT_SHFT                                 0x0

#define HWIO_MSS_CC_MODEM_TX_CORE_CBCR_ADDR                                                    (MSS_CC_MSS_CC_REG_REG_BASE      + 0x000004ec)
#define HWIO_MSS_CC_MODEM_TX_CORE_CBCR_RMSK                                                    0x80007ff5
#define HWIO_MSS_CC_MODEM_TX_CORE_CBCR_IN          \
        in_dword(HWIO_MSS_CC_MODEM_TX_CORE_CBCR_ADDR)
#define HWIO_MSS_CC_MODEM_TX_CORE_CBCR_INM(m)      \
        in_dword_masked(HWIO_MSS_CC_MODEM_TX_CORE_CBCR_ADDR, m)
#define HWIO_MSS_CC_MODEM_TX_CORE_CBCR_OUT(v)      \
        out_dword(HWIO_MSS_CC_MODEM_TX_CORE_CBCR_ADDR,v)
#define HWIO_MSS_CC_MODEM_TX_CORE_CBCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_CC_MODEM_TX_CORE_CBCR_ADDR,m,v,HWIO_MSS_CC_MODEM_TX_CORE_CBCR_IN)
#define HWIO_MSS_CC_MODEM_TX_CORE_CBCR_CLK_OFF_BMSK                                            0x80000000
#define HWIO_MSS_CC_MODEM_TX_CORE_CBCR_CLK_OFF_SHFT                                                  0x1f
#define HWIO_MSS_CC_MODEM_TX_CORE_CBCR_FORCE_MEM_CORE_ON_BMSK                                      0x4000
#define HWIO_MSS_CC_MODEM_TX_CORE_CBCR_FORCE_MEM_CORE_ON_SHFT                                         0xe
#define HWIO_MSS_CC_MODEM_TX_CORE_CBCR_FORCE_MEM_PERIPH_ON_BMSK                                    0x2000
#define HWIO_MSS_CC_MODEM_TX_CORE_CBCR_FORCE_MEM_PERIPH_ON_SHFT                                       0xd
#define HWIO_MSS_CC_MODEM_TX_CORE_CBCR_FORCE_MEM_PERIPH_OFF_BMSK                                   0x1000
#define HWIO_MSS_CC_MODEM_TX_CORE_CBCR_FORCE_MEM_PERIPH_OFF_SHFT                                      0xc
#define HWIO_MSS_CC_MODEM_TX_CORE_CBCR_WAKEUP_BMSK                                                  0xf00
#define HWIO_MSS_CC_MODEM_TX_CORE_CBCR_WAKEUP_SHFT                                                    0x8
#define HWIO_MSS_CC_MODEM_TX_CORE_CBCR_SLEEP_BMSK                                                    0xf0
#define HWIO_MSS_CC_MODEM_TX_CORE_CBCR_SLEEP_SHFT                                                     0x4
#define HWIO_MSS_CC_MODEM_TX_CORE_CBCR_CLK_ARES_BMSK                                                  0x4
#define HWIO_MSS_CC_MODEM_TX_CORE_CBCR_CLK_ARES_SHFT                                                  0x2
#define HWIO_MSS_CC_MODEM_TX_CORE_CBCR_CLK_ENABLE_BMSK                                                0x1
#define HWIO_MSS_CC_MODEM_TX_CORE_CBCR_CLK_ENABLE_SHFT                                                0x0

#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_BCR_ADDR                                                (MSS_CC_MSS_CC_REG_REG_BASE      + 0x00000500)
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_BCR_RMSK                                                       0x1
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_BCR_IN          \
        in_dword(HWIO_MSS_CC_MODEM_DEMBACK_CORE_BCR_ADDR)
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_BCR_INM(m)      \
        in_dword_masked(HWIO_MSS_CC_MODEM_DEMBACK_CORE_BCR_ADDR, m)
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_BCR_OUT(v)      \
        out_dword(HWIO_MSS_CC_MODEM_DEMBACK_CORE_BCR_ADDR,v)
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_BCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_CC_MODEM_DEMBACK_CORE_BCR_ADDR,m,v,HWIO_MSS_CC_MODEM_DEMBACK_CORE_BCR_IN)
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_BCR_BLK_ARES_BMSK                                              0x1
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_BCR_BLK_ARES_SHFT                                              0x0

#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_GDSCR_ADDR                                              (MSS_CC_MSS_CC_REG_REG_BASE      + 0x00000504)
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_GDSCR_RMSK                                              0xf8ffffff
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_GDSCR_IN          \
        in_dword(HWIO_MSS_CC_MODEM_DEMBACK_CORE_GDSCR_ADDR)
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_GDSCR_INM(m)      \
        in_dword_masked(HWIO_MSS_CC_MODEM_DEMBACK_CORE_GDSCR_ADDR, m)
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_GDSCR_OUT(v)      \
        out_dword(HWIO_MSS_CC_MODEM_DEMBACK_CORE_GDSCR_ADDR,v)
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_GDSCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_CC_MODEM_DEMBACK_CORE_GDSCR_ADDR,m,v,HWIO_MSS_CC_MODEM_DEMBACK_CORE_GDSCR_IN)
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_GDSCR_PWR_ON_BMSK                                       0x80000000
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_GDSCR_PWR_ON_SHFT                                             0x1f
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_GDSCR_GDSC_STATE_BMSK                                   0x78000000
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_GDSCR_GDSC_STATE_SHFT                                         0x1b
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_GDSCR_EN_REST_WAIT_BMSK                                   0xf00000
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_GDSCR_EN_REST_WAIT_SHFT                                       0x14
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_GDSCR_EN_FEW_WAIT_BMSK                                     0xf0000
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_GDSCR_EN_FEW_WAIT_SHFT                                        0x10
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_GDSCR_CLK_DIS_WAIT_BMSK                                     0xf000
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_GDSCR_CLK_DIS_WAIT_SHFT                                        0xc
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_GDSCR_RETAIN_FF_ENABLE_BMSK                                  0x800
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_GDSCR_RETAIN_FF_ENABLE_SHFT                                    0xb
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_GDSCR_RESTORE_BMSK                                           0x400
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_GDSCR_RESTORE_SHFT                                             0xa
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_GDSCR_SAVE_BMSK                                              0x200
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_GDSCR_SAVE_SHFT                                                0x9
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_GDSCR_RETAIN_BMSK                                            0x100
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_GDSCR_RETAIN_SHFT                                              0x8
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_GDSCR_EN_REST_BMSK                                            0x80
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_GDSCR_EN_REST_SHFT                                             0x7
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_GDSCR_EN_FEW_BMSK                                             0x40
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_GDSCR_EN_FEW_SHFT                                              0x6
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_GDSCR_CLAMP_IO_BMSK                                           0x20
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_GDSCR_CLAMP_IO_SHFT                                            0x5
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_GDSCR_CLK_DISABLE_BMSK                                        0x10
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_GDSCR_CLK_DISABLE_SHFT                                         0x4
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_GDSCR_PD_ARES_BMSK                                             0x8
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_GDSCR_PD_ARES_SHFT                                             0x3
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_GDSCR_SW_OVERRIDE_BMSK                                         0x4
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_GDSCR_SW_OVERRIDE_SHFT                                         0x2
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_GDSCR_HW_CONTROL_BMSK                                          0x2
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_GDSCR_HW_CONTROL_SHFT                                          0x1
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_GDSCR_SW_COLLAPSE_BMSK                                         0x1
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_GDSCR_SW_COLLAPSE_SHFT                                         0x0

#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_CFG_GDSCR_ADDR                                          (MSS_CC_MSS_CC_REG_REG_BASE      + 0x00000508)
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_CFG_GDSCR_RMSK                                          0xffffffff
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_CFG_GDSCR_IN          \
        in_dword(HWIO_MSS_CC_MODEM_DEMBACK_CORE_CFG_GDSCR_ADDR)
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_CFG_GDSCR_INM(m)      \
        in_dword_masked(HWIO_MSS_CC_MODEM_DEMBACK_CORE_CFG_GDSCR_ADDR, m)
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_CFG_GDSCR_OUT(v)      \
        out_dword(HWIO_MSS_CC_MODEM_DEMBACK_CORE_CFG_GDSCR_ADDR,v)
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_CFG_GDSCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_CC_MODEM_DEMBACK_CORE_CFG_GDSCR_ADDR,m,v,HWIO_MSS_CC_MODEM_DEMBACK_CORE_CFG_GDSCR_IN)
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_CFG_GDSCR_GDSC_SPARE_CTRL_IN_BMSK                       0xf0000000
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_CFG_GDSCR_GDSC_SPARE_CTRL_IN_SHFT                             0x1c
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_CFG_GDSCR_GDSC_SPARE_CTRL_OUT_BMSK                       0xc000000
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_CFG_GDSCR_GDSC_SPARE_CTRL_OUT_SHFT                            0x1a
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_CFG_GDSCR_GDSC_PWR_DWN_START_BMSK                        0x2000000
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_CFG_GDSCR_GDSC_PWR_DWN_START_SHFT                             0x19
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_CFG_GDSCR_GDSC_PWR_UP_START_BMSK                         0x1000000
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_CFG_GDSCR_GDSC_PWR_UP_START_SHFT                              0x18
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_CFG_GDSCR_GDSC_CFG_FSM_STATE_STATUS_BMSK                  0xf00000
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_CFG_GDSCR_GDSC_CFG_FSM_STATE_STATUS_SHFT                      0x14
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_CFG_GDSCR_GDSC_MEM_PWR_ACK_STATUS_BMSK                     0x80000
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_CFG_GDSCR_GDSC_MEM_PWR_ACK_STATUS_SHFT                        0x13
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_CFG_GDSCR_GDSC_ENR_ACK_STATUS_BMSK                         0x40000
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_CFG_GDSCR_GDSC_ENR_ACK_STATUS_SHFT                            0x12
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_CFG_GDSCR_GDSC_ENF_ACK_STATUS_BMSK                         0x20000
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_CFG_GDSCR_GDSC_ENF_ACK_STATUS_SHFT                            0x11
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_CFG_GDSCR_GDSC_POWER_UP_COMPLETE_BMSK                      0x10000
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_CFG_GDSCR_GDSC_POWER_UP_COMPLETE_SHFT                         0x10
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_CFG_GDSCR_GDSC_POWER_DOWN_COMPLETE_BMSK                     0x8000
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_CFG_GDSCR_GDSC_POWER_DOWN_COMPLETE_SHFT                        0xf
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_CFG_GDSCR_SOFTWARE_CONTROL_OVERRIDE_BMSK                    0x7800
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_CFG_GDSCR_SOFTWARE_CONTROL_OVERRIDE_SHFT                       0xb
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_CFG_GDSCR_GDSC_HANDSHAKE_DIS_BMSK                            0x400
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_CFG_GDSCR_GDSC_HANDSHAKE_DIS_SHFT                              0xa
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_CFG_GDSCR_GDSC_MEM_PERI_FORCE_IN_SW_BMSK                     0x200
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_CFG_GDSCR_GDSC_MEM_PERI_FORCE_IN_SW_SHFT                       0x9
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_CFG_GDSCR_GDSC_MEM_CORE_FORCE_IN_SW_BMSK                     0x100
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_CFG_GDSCR_GDSC_MEM_CORE_FORCE_IN_SW_SHFT                       0x8
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_CFG_GDSCR_GDSC_PHASE_RESET_EN_SW_BMSK                         0x80
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_CFG_GDSCR_GDSC_PHASE_RESET_EN_SW_SHFT                          0x7
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_CFG_GDSCR_GDSC_PHASE_RESET_DELAY_COUNT_SW_BMSK                0x60
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_CFG_GDSCR_GDSC_PHASE_RESET_DELAY_COUNT_SW_SHFT                 0x5
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_CFG_GDSCR_GDSC_PSCBC_PWR_DWN_SW_BMSK                          0x10
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_CFG_GDSCR_GDSC_PSCBC_PWR_DWN_SW_SHFT                           0x4
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_CFG_GDSCR_UNCLAMP_IO_SOFTWARE_OVERRIDE_BMSK                    0x8
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_CFG_GDSCR_UNCLAMP_IO_SOFTWARE_OVERRIDE_SHFT                    0x3
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_CFG_GDSCR_SAVE_RESTORE_SOFTWARE_OVERRIDE_BMSK                  0x4
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_CFG_GDSCR_SAVE_RESTORE_SOFTWARE_OVERRIDE_SHFT                  0x2
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_CFG_GDSCR_CLAMP_IO_SOFTWARE_OVERRIDE_BMSK                      0x2
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_CFG_GDSCR_CLAMP_IO_SOFTWARE_OVERRIDE_SHFT                      0x1
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_CFG_GDSCR_DISABLE_CLK_SOFTWARE_OVERRIDE_BMSK                   0x1
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_CFG_GDSCR_DISABLE_CLK_SOFTWARE_OVERRIDE_SHFT                   0x0

#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_CFG2_GDSCR_ADDR                                         (MSS_CC_MSS_CC_REG_REG_BASE      + 0x0000050c)
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_CFG2_GDSCR_RMSK                                            0x1ffff
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_CFG2_GDSCR_IN          \
        in_dword(HWIO_MSS_CC_MODEM_DEMBACK_CORE_CFG2_GDSCR_ADDR)
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_CFG2_GDSCR_INM(m)      \
        in_dword_masked(HWIO_MSS_CC_MODEM_DEMBACK_CORE_CFG2_GDSCR_ADDR, m)
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_CFG2_GDSCR_OUT(v)      \
        out_dword(HWIO_MSS_CC_MODEM_DEMBACK_CORE_CFG2_GDSCR_ADDR,v)
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_CFG2_GDSCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_CC_MODEM_DEMBACK_CORE_CFG2_GDSCR_ADDR,m,v,HWIO_MSS_CC_MODEM_DEMBACK_CORE_CFG2_GDSCR_IN)
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_CFG2_GDSCR_GDSC_CLAMP_MEM_SW_BMSK                          0x10000
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_CFG2_GDSCR_GDSC_CLAMP_MEM_SW_SHFT                             0x10
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_CFG2_GDSCR_DLY_MEM_PWR_UP_BMSK                              0xf000
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_CFG2_GDSCR_DLY_MEM_PWR_UP_SHFT                                 0xc
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_CFG2_GDSCR_DLY_DEASSERT_CLAMP_MEM_BMSK                       0xf00
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_CFG2_GDSCR_DLY_DEASSERT_CLAMP_MEM_SHFT                         0x8
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_CFG2_GDSCR_DLY_ASSERT_CLAMP_MEM_BMSK                          0xf0
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_CFG2_GDSCR_DLY_ASSERT_CLAMP_MEM_SHFT                           0x4
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_CFG2_GDSCR_MEM_PWR_DWN_TIMEOUT_BMSK                            0xf
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_CFG2_GDSCR_MEM_PWR_DWN_TIMEOUT_SHFT                            0x0

#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_CBCR_ADDR                                               (MSS_CC_MSS_CC_REG_REG_BASE      + 0x00000514)
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_CBCR_RMSK                                               0x80007ff5
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_CBCR_IN          \
        in_dword(HWIO_MSS_CC_MODEM_DEMBACK_CORE_CBCR_ADDR)
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_CBCR_INM(m)      \
        in_dword_masked(HWIO_MSS_CC_MODEM_DEMBACK_CORE_CBCR_ADDR, m)
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_CBCR_OUT(v)      \
        out_dword(HWIO_MSS_CC_MODEM_DEMBACK_CORE_CBCR_ADDR,v)
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_CBCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_CC_MODEM_DEMBACK_CORE_CBCR_ADDR,m,v,HWIO_MSS_CC_MODEM_DEMBACK_CORE_CBCR_IN)
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_CBCR_CLK_OFF_BMSK                                       0x80000000
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_CBCR_CLK_OFF_SHFT                                             0x1f
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_CBCR_FORCE_MEM_CORE_ON_BMSK                                 0x4000
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_CBCR_FORCE_MEM_CORE_ON_SHFT                                    0xe
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_CBCR_FORCE_MEM_PERIPH_ON_BMSK                               0x2000
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_CBCR_FORCE_MEM_PERIPH_ON_SHFT                                  0xd
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_CBCR_FORCE_MEM_PERIPH_OFF_BMSK                              0x1000
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_CBCR_FORCE_MEM_PERIPH_OFF_SHFT                                 0xc
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_CBCR_WAKEUP_BMSK                                             0xf00
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_CBCR_WAKEUP_SHFT                                               0x8
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_CBCR_SLEEP_BMSK                                               0xf0
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_CBCR_SLEEP_SHFT                                                0x4
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_CBCR_CLK_ARES_BMSK                                             0x4
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_CBCR_CLK_ARES_SHFT                                             0x2
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_CBCR_CLK_ENABLE_BMSK                                           0x1
#define HWIO_MSS_CC_MODEM_DEMBACK_CORE_CBCR_CLK_ENABLE_SHFT                                           0x0

#define HWIO_MSS_CC_MODEM_TDEC_CORE_BCR_ADDR                                                   (MSS_CC_MSS_CC_REG_REG_BASE      + 0x00000528)
#define HWIO_MSS_CC_MODEM_TDEC_CORE_BCR_RMSK                                                          0x1
#define HWIO_MSS_CC_MODEM_TDEC_CORE_BCR_IN          \
        in_dword(HWIO_MSS_CC_MODEM_TDEC_CORE_BCR_ADDR)
#define HWIO_MSS_CC_MODEM_TDEC_CORE_BCR_INM(m)      \
        in_dword_masked(HWIO_MSS_CC_MODEM_TDEC_CORE_BCR_ADDR, m)
#define HWIO_MSS_CC_MODEM_TDEC_CORE_BCR_OUT(v)      \
        out_dword(HWIO_MSS_CC_MODEM_TDEC_CORE_BCR_ADDR,v)
#define HWIO_MSS_CC_MODEM_TDEC_CORE_BCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_CC_MODEM_TDEC_CORE_BCR_ADDR,m,v,HWIO_MSS_CC_MODEM_TDEC_CORE_BCR_IN)
#define HWIO_MSS_CC_MODEM_TDEC_CORE_BCR_BLK_ARES_BMSK                                                 0x1
#define HWIO_MSS_CC_MODEM_TDEC_CORE_BCR_BLK_ARES_SHFT                                                 0x0

#define HWIO_MSS_CC_MODEM_TDEC_CORE_GDSCR_ADDR                                                 (MSS_CC_MSS_CC_REG_REG_BASE      + 0x0000052c)
#define HWIO_MSS_CC_MODEM_TDEC_CORE_GDSCR_RMSK                                                 0xf8ffffff
#define HWIO_MSS_CC_MODEM_TDEC_CORE_GDSCR_IN          \
        in_dword(HWIO_MSS_CC_MODEM_TDEC_CORE_GDSCR_ADDR)
#define HWIO_MSS_CC_MODEM_TDEC_CORE_GDSCR_INM(m)      \
        in_dword_masked(HWIO_MSS_CC_MODEM_TDEC_CORE_GDSCR_ADDR, m)
#define HWIO_MSS_CC_MODEM_TDEC_CORE_GDSCR_OUT(v)      \
        out_dword(HWIO_MSS_CC_MODEM_TDEC_CORE_GDSCR_ADDR,v)
#define HWIO_MSS_CC_MODEM_TDEC_CORE_GDSCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_CC_MODEM_TDEC_CORE_GDSCR_ADDR,m,v,HWIO_MSS_CC_MODEM_TDEC_CORE_GDSCR_IN)
#define HWIO_MSS_CC_MODEM_TDEC_CORE_GDSCR_PWR_ON_BMSK                                          0x80000000
#define HWIO_MSS_CC_MODEM_TDEC_CORE_GDSCR_PWR_ON_SHFT                                                0x1f
#define HWIO_MSS_CC_MODEM_TDEC_CORE_GDSCR_GDSC_STATE_BMSK                                      0x78000000
#define HWIO_MSS_CC_MODEM_TDEC_CORE_GDSCR_GDSC_STATE_SHFT                                            0x1b
#define HWIO_MSS_CC_MODEM_TDEC_CORE_GDSCR_EN_REST_WAIT_BMSK                                      0xf00000
#define HWIO_MSS_CC_MODEM_TDEC_CORE_GDSCR_EN_REST_WAIT_SHFT                                          0x14
#define HWIO_MSS_CC_MODEM_TDEC_CORE_GDSCR_EN_FEW_WAIT_BMSK                                        0xf0000
#define HWIO_MSS_CC_MODEM_TDEC_CORE_GDSCR_EN_FEW_WAIT_SHFT                                           0x10
#define HWIO_MSS_CC_MODEM_TDEC_CORE_GDSCR_CLK_DIS_WAIT_BMSK                                        0xf000
#define HWIO_MSS_CC_MODEM_TDEC_CORE_GDSCR_CLK_DIS_WAIT_SHFT                                           0xc
#define HWIO_MSS_CC_MODEM_TDEC_CORE_GDSCR_RETAIN_FF_ENABLE_BMSK                                     0x800
#define HWIO_MSS_CC_MODEM_TDEC_CORE_GDSCR_RETAIN_FF_ENABLE_SHFT                                       0xb
#define HWIO_MSS_CC_MODEM_TDEC_CORE_GDSCR_RESTORE_BMSK                                              0x400
#define HWIO_MSS_CC_MODEM_TDEC_CORE_GDSCR_RESTORE_SHFT                                                0xa
#define HWIO_MSS_CC_MODEM_TDEC_CORE_GDSCR_SAVE_BMSK                                                 0x200
#define HWIO_MSS_CC_MODEM_TDEC_CORE_GDSCR_SAVE_SHFT                                                   0x9
#define HWIO_MSS_CC_MODEM_TDEC_CORE_GDSCR_RETAIN_BMSK                                               0x100
#define HWIO_MSS_CC_MODEM_TDEC_CORE_GDSCR_RETAIN_SHFT                                                 0x8
#define HWIO_MSS_CC_MODEM_TDEC_CORE_GDSCR_EN_REST_BMSK                                               0x80
#define HWIO_MSS_CC_MODEM_TDEC_CORE_GDSCR_EN_REST_SHFT                                                0x7
#define HWIO_MSS_CC_MODEM_TDEC_CORE_GDSCR_EN_FEW_BMSK                                                0x40
#define HWIO_MSS_CC_MODEM_TDEC_CORE_GDSCR_EN_FEW_SHFT                                                 0x6
#define HWIO_MSS_CC_MODEM_TDEC_CORE_GDSCR_CLAMP_IO_BMSK                                              0x20
#define HWIO_MSS_CC_MODEM_TDEC_CORE_GDSCR_CLAMP_IO_SHFT                                               0x5
#define HWIO_MSS_CC_MODEM_TDEC_CORE_GDSCR_CLK_DISABLE_BMSK                                           0x10
#define HWIO_MSS_CC_MODEM_TDEC_CORE_GDSCR_CLK_DISABLE_SHFT                                            0x4
#define HWIO_MSS_CC_MODEM_TDEC_CORE_GDSCR_PD_ARES_BMSK                                                0x8
#define HWIO_MSS_CC_MODEM_TDEC_CORE_GDSCR_PD_ARES_SHFT                                                0x3
#define HWIO_MSS_CC_MODEM_TDEC_CORE_GDSCR_SW_OVERRIDE_BMSK                                            0x4
#define HWIO_MSS_CC_MODEM_TDEC_CORE_GDSCR_SW_OVERRIDE_SHFT                                            0x2
#define HWIO_MSS_CC_MODEM_TDEC_CORE_GDSCR_HW_CONTROL_BMSK                                             0x2
#define HWIO_MSS_CC_MODEM_TDEC_CORE_GDSCR_HW_CONTROL_SHFT                                             0x1
#define HWIO_MSS_CC_MODEM_TDEC_CORE_GDSCR_SW_COLLAPSE_BMSK                                            0x1
#define HWIO_MSS_CC_MODEM_TDEC_CORE_GDSCR_SW_COLLAPSE_SHFT                                            0x0

#define HWIO_MSS_CC_MODEM_TDEC_CORE_CFG_GDSCR_ADDR                                             (MSS_CC_MSS_CC_REG_REG_BASE      + 0x00000530)
#define HWIO_MSS_CC_MODEM_TDEC_CORE_CFG_GDSCR_RMSK                                             0xffffffff
#define HWIO_MSS_CC_MODEM_TDEC_CORE_CFG_GDSCR_IN          \
        in_dword(HWIO_MSS_CC_MODEM_TDEC_CORE_CFG_GDSCR_ADDR)
#define HWIO_MSS_CC_MODEM_TDEC_CORE_CFG_GDSCR_INM(m)      \
        in_dword_masked(HWIO_MSS_CC_MODEM_TDEC_CORE_CFG_GDSCR_ADDR, m)
#define HWIO_MSS_CC_MODEM_TDEC_CORE_CFG_GDSCR_OUT(v)      \
        out_dword(HWIO_MSS_CC_MODEM_TDEC_CORE_CFG_GDSCR_ADDR,v)
#define HWIO_MSS_CC_MODEM_TDEC_CORE_CFG_GDSCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_CC_MODEM_TDEC_CORE_CFG_GDSCR_ADDR,m,v,HWIO_MSS_CC_MODEM_TDEC_CORE_CFG_GDSCR_IN)
#define HWIO_MSS_CC_MODEM_TDEC_CORE_CFG_GDSCR_GDSC_SPARE_CTRL_IN_BMSK                          0xf0000000
#define HWIO_MSS_CC_MODEM_TDEC_CORE_CFG_GDSCR_GDSC_SPARE_CTRL_IN_SHFT                                0x1c
#define HWIO_MSS_CC_MODEM_TDEC_CORE_CFG_GDSCR_GDSC_SPARE_CTRL_OUT_BMSK                          0xc000000
#define HWIO_MSS_CC_MODEM_TDEC_CORE_CFG_GDSCR_GDSC_SPARE_CTRL_OUT_SHFT                               0x1a
#define HWIO_MSS_CC_MODEM_TDEC_CORE_CFG_GDSCR_GDSC_PWR_DWN_START_BMSK                           0x2000000
#define HWIO_MSS_CC_MODEM_TDEC_CORE_CFG_GDSCR_GDSC_PWR_DWN_START_SHFT                                0x19
#define HWIO_MSS_CC_MODEM_TDEC_CORE_CFG_GDSCR_GDSC_PWR_UP_START_BMSK                            0x1000000
#define HWIO_MSS_CC_MODEM_TDEC_CORE_CFG_GDSCR_GDSC_PWR_UP_START_SHFT                                 0x18
#define HWIO_MSS_CC_MODEM_TDEC_CORE_CFG_GDSCR_GDSC_CFG_FSM_STATE_STATUS_BMSK                     0xf00000
#define HWIO_MSS_CC_MODEM_TDEC_CORE_CFG_GDSCR_GDSC_CFG_FSM_STATE_STATUS_SHFT                         0x14
#define HWIO_MSS_CC_MODEM_TDEC_CORE_CFG_GDSCR_GDSC_MEM_PWR_ACK_STATUS_BMSK                        0x80000
#define HWIO_MSS_CC_MODEM_TDEC_CORE_CFG_GDSCR_GDSC_MEM_PWR_ACK_STATUS_SHFT                           0x13
#define HWIO_MSS_CC_MODEM_TDEC_CORE_CFG_GDSCR_GDSC_ENR_ACK_STATUS_BMSK                            0x40000
#define HWIO_MSS_CC_MODEM_TDEC_CORE_CFG_GDSCR_GDSC_ENR_ACK_STATUS_SHFT                               0x12
#define HWIO_MSS_CC_MODEM_TDEC_CORE_CFG_GDSCR_GDSC_ENF_ACK_STATUS_BMSK                            0x20000
#define HWIO_MSS_CC_MODEM_TDEC_CORE_CFG_GDSCR_GDSC_ENF_ACK_STATUS_SHFT                               0x11
#define HWIO_MSS_CC_MODEM_TDEC_CORE_CFG_GDSCR_GDSC_POWER_UP_COMPLETE_BMSK                         0x10000
#define HWIO_MSS_CC_MODEM_TDEC_CORE_CFG_GDSCR_GDSC_POWER_UP_COMPLETE_SHFT                            0x10
#define HWIO_MSS_CC_MODEM_TDEC_CORE_CFG_GDSCR_GDSC_POWER_DOWN_COMPLETE_BMSK                        0x8000
#define HWIO_MSS_CC_MODEM_TDEC_CORE_CFG_GDSCR_GDSC_POWER_DOWN_COMPLETE_SHFT                           0xf
#define HWIO_MSS_CC_MODEM_TDEC_CORE_CFG_GDSCR_SOFTWARE_CONTROL_OVERRIDE_BMSK                       0x7800
#define HWIO_MSS_CC_MODEM_TDEC_CORE_CFG_GDSCR_SOFTWARE_CONTROL_OVERRIDE_SHFT                          0xb
#define HWIO_MSS_CC_MODEM_TDEC_CORE_CFG_GDSCR_GDSC_HANDSHAKE_DIS_BMSK                               0x400
#define HWIO_MSS_CC_MODEM_TDEC_CORE_CFG_GDSCR_GDSC_HANDSHAKE_DIS_SHFT                                 0xa
#define HWIO_MSS_CC_MODEM_TDEC_CORE_CFG_GDSCR_GDSC_MEM_PERI_FORCE_IN_SW_BMSK                        0x200
#define HWIO_MSS_CC_MODEM_TDEC_CORE_CFG_GDSCR_GDSC_MEM_PERI_FORCE_IN_SW_SHFT                          0x9
#define HWIO_MSS_CC_MODEM_TDEC_CORE_CFG_GDSCR_GDSC_MEM_CORE_FORCE_IN_SW_BMSK                        0x100
#define HWIO_MSS_CC_MODEM_TDEC_CORE_CFG_GDSCR_GDSC_MEM_CORE_FORCE_IN_SW_SHFT                          0x8
#define HWIO_MSS_CC_MODEM_TDEC_CORE_CFG_GDSCR_GDSC_PHASE_RESET_EN_SW_BMSK                            0x80
#define HWIO_MSS_CC_MODEM_TDEC_CORE_CFG_GDSCR_GDSC_PHASE_RESET_EN_SW_SHFT                             0x7
#define HWIO_MSS_CC_MODEM_TDEC_CORE_CFG_GDSCR_GDSC_PHASE_RESET_DELAY_COUNT_SW_BMSK                   0x60
#define HWIO_MSS_CC_MODEM_TDEC_CORE_CFG_GDSCR_GDSC_PHASE_RESET_DELAY_COUNT_SW_SHFT                    0x5
#define HWIO_MSS_CC_MODEM_TDEC_CORE_CFG_GDSCR_GDSC_PSCBC_PWR_DWN_SW_BMSK                             0x10
#define HWIO_MSS_CC_MODEM_TDEC_CORE_CFG_GDSCR_GDSC_PSCBC_PWR_DWN_SW_SHFT                              0x4
#define HWIO_MSS_CC_MODEM_TDEC_CORE_CFG_GDSCR_UNCLAMP_IO_SOFTWARE_OVERRIDE_BMSK                       0x8
#define HWIO_MSS_CC_MODEM_TDEC_CORE_CFG_GDSCR_UNCLAMP_IO_SOFTWARE_OVERRIDE_SHFT                       0x3
#define HWIO_MSS_CC_MODEM_TDEC_CORE_CFG_GDSCR_SAVE_RESTORE_SOFTWARE_OVERRIDE_BMSK                     0x4
#define HWIO_MSS_CC_MODEM_TDEC_CORE_CFG_GDSCR_SAVE_RESTORE_SOFTWARE_OVERRIDE_SHFT                     0x2
#define HWIO_MSS_CC_MODEM_TDEC_CORE_CFG_GDSCR_CLAMP_IO_SOFTWARE_OVERRIDE_BMSK                         0x2
#define HWIO_MSS_CC_MODEM_TDEC_CORE_CFG_GDSCR_CLAMP_IO_SOFTWARE_OVERRIDE_SHFT                         0x1
#define HWIO_MSS_CC_MODEM_TDEC_CORE_CFG_GDSCR_DISABLE_CLK_SOFTWARE_OVERRIDE_BMSK                      0x1
#define HWIO_MSS_CC_MODEM_TDEC_CORE_CFG_GDSCR_DISABLE_CLK_SOFTWARE_OVERRIDE_SHFT                      0x0

#define HWIO_MSS_CC_MODEM_TDEC_CORE_CFG2_GDSCR_ADDR                                            (MSS_CC_MSS_CC_REG_REG_BASE      + 0x00000534)
#define HWIO_MSS_CC_MODEM_TDEC_CORE_CFG2_GDSCR_RMSK                                               0x1ffff
#define HWIO_MSS_CC_MODEM_TDEC_CORE_CFG2_GDSCR_IN          \
        in_dword(HWIO_MSS_CC_MODEM_TDEC_CORE_CFG2_GDSCR_ADDR)
#define HWIO_MSS_CC_MODEM_TDEC_CORE_CFG2_GDSCR_INM(m)      \
        in_dword_masked(HWIO_MSS_CC_MODEM_TDEC_CORE_CFG2_GDSCR_ADDR, m)
#define HWIO_MSS_CC_MODEM_TDEC_CORE_CFG2_GDSCR_OUT(v)      \
        out_dword(HWIO_MSS_CC_MODEM_TDEC_CORE_CFG2_GDSCR_ADDR,v)
#define HWIO_MSS_CC_MODEM_TDEC_CORE_CFG2_GDSCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_CC_MODEM_TDEC_CORE_CFG2_GDSCR_ADDR,m,v,HWIO_MSS_CC_MODEM_TDEC_CORE_CFG2_GDSCR_IN)
#define HWIO_MSS_CC_MODEM_TDEC_CORE_CFG2_GDSCR_GDSC_CLAMP_MEM_SW_BMSK                             0x10000
#define HWIO_MSS_CC_MODEM_TDEC_CORE_CFG2_GDSCR_GDSC_CLAMP_MEM_SW_SHFT                                0x10
#define HWIO_MSS_CC_MODEM_TDEC_CORE_CFG2_GDSCR_DLY_MEM_PWR_UP_BMSK                                 0xf000
#define HWIO_MSS_CC_MODEM_TDEC_CORE_CFG2_GDSCR_DLY_MEM_PWR_UP_SHFT                                    0xc
#define HWIO_MSS_CC_MODEM_TDEC_CORE_CFG2_GDSCR_DLY_DEASSERT_CLAMP_MEM_BMSK                          0xf00
#define HWIO_MSS_CC_MODEM_TDEC_CORE_CFG2_GDSCR_DLY_DEASSERT_CLAMP_MEM_SHFT                            0x8
#define HWIO_MSS_CC_MODEM_TDEC_CORE_CFG2_GDSCR_DLY_ASSERT_CLAMP_MEM_BMSK                             0xf0
#define HWIO_MSS_CC_MODEM_TDEC_CORE_CFG2_GDSCR_DLY_ASSERT_CLAMP_MEM_SHFT                              0x4
#define HWIO_MSS_CC_MODEM_TDEC_CORE_CFG2_GDSCR_MEM_PWR_DWN_TIMEOUT_BMSK                               0xf
#define HWIO_MSS_CC_MODEM_TDEC_CORE_CFG2_GDSCR_MEM_PWR_DWN_TIMEOUT_SHFT                               0x0

#define HWIO_MSS_CC_MODEM_TDEC_CORE_CBCR_ADDR                                                  (MSS_CC_MSS_CC_REG_REG_BASE      + 0x0000053c)
#define HWIO_MSS_CC_MODEM_TDEC_CORE_CBCR_RMSK                                                  0x80007ff5
#define HWIO_MSS_CC_MODEM_TDEC_CORE_CBCR_IN          \
        in_dword(HWIO_MSS_CC_MODEM_TDEC_CORE_CBCR_ADDR)
#define HWIO_MSS_CC_MODEM_TDEC_CORE_CBCR_INM(m)      \
        in_dword_masked(HWIO_MSS_CC_MODEM_TDEC_CORE_CBCR_ADDR, m)
#define HWIO_MSS_CC_MODEM_TDEC_CORE_CBCR_OUT(v)      \
        out_dword(HWIO_MSS_CC_MODEM_TDEC_CORE_CBCR_ADDR,v)
#define HWIO_MSS_CC_MODEM_TDEC_CORE_CBCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_CC_MODEM_TDEC_CORE_CBCR_ADDR,m,v,HWIO_MSS_CC_MODEM_TDEC_CORE_CBCR_IN)
#define HWIO_MSS_CC_MODEM_TDEC_CORE_CBCR_CLK_OFF_BMSK                                          0x80000000
#define HWIO_MSS_CC_MODEM_TDEC_CORE_CBCR_CLK_OFF_SHFT                                                0x1f
#define HWIO_MSS_CC_MODEM_TDEC_CORE_CBCR_FORCE_MEM_CORE_ON_BMSK                                    0x4000
#define HWIO_MSS_CC_MODEM_TDEC_CORE_CBCR_FORCE_MEM_CORE_ON_SHFT                                       0xe
#define HWIO_MSS_CC_MODEM_TDEC_CORE_CBCR_FORCE_MEM_PERIPH_ON_BMSK                                  0x2000
#define HWIO_MSS_CC_MODEM_TDEC_CORE_CBCR_FORCE_MEM_PERIPH_ON_SHFT                                     0xd
#define HWIO_MSS_CC_MODEM_TDEC_CORE_CBCR_FORCE_MEM_PERIPH_OFF_BMSK                                 0x1000
#define HWIO_MSS_CC_MODEM_TDEC_CORE_CBCR_FORCE_MEM_PERIPH_OFF_SHFT                                    0xc
#define HWIO_MSS_CC_MODEM_TDEC_CORE_CBCR_WAKEUP_BMSK                                                0xf00
#define HWIO_MSS_CC_MODEM_TDEC_CORE_CBCR_WAKEUP_SHFT                                                  0x8
#define HWIO_MSS_CC_MODEM_TDEC_CORE_CBCR_SLEEP_BMSK                                                  0xf0
#define HWIO_MSS_CC_MODEM_TDEC_CORE_CBCR_SLEEP_SHFT                                                   0x4
#define HWIO_MSS_CC_MODEM_TDEC_CORE_CBCR_CLK_ARES_BMSK                                                0x4
#define HWIO_MSS_CC_MODEM_TDEC_CORE_CBCR_CLK_ARES_SHFT                                                0x2
#define HWIO_MSS_CC_MODEM_TDEC_CORE_CBCR_CLK_ENABLE_BMSK                                              0x1
#define HWIO_MSS_CC_MODEM_TDEC_CORE_CBCR_CLK_ENABLE_SHFT                                              0x0

#define HWIO_MSS_CC_Q6_BCR_ADDR                                                                (MSS_CC_MSS_CC_REG_REG_BASE      + 0x00000550)
#define HWIO_MSS_CC_Q6_BCR_RMSK                                                                       0x1
#define HWIO_MSS_CC_Q6_BCR_IN          \
        in_dword(HWIO_MSS_CC_Q6_BCR_ADDR)
#define HWIO_MSS_CC_Q6_BCR_INM(m)      \
        in_dword_masked(HWIO_MSS_CC_Q6_BCR_ADDR, m)
#define HWIO_MSS_CC_Q6_BCR_OUT(v)      \
        out_dword(HWIO_MSS_CC_Q6_BCR_ADDR,v)
#define HWIO_MSS_CC_Q6_BCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_CC_Q6_BCR_ADDR,m,v,HWIO_MSS_CC_Q6_BCR_IN)
#define HWIO_MSS_CC_Q6_BCR_BLK_ARES_BMSK                                                              0x1
#define HWIO_MSS_CC_Q6_BCR_BLK_ARES_SHFT                                                              0x0

#define HWIO_MSS_CC_Q6_CMD_RCGR_ADDR                                                           (MSS_CC_MSS_CC_REG_REG_BASE      + 0x00000564)
#define HWIO_MSS_CC_Q6_CMD_RCGR_RMSK                                                           0x80000013
#define HWIO_MSS_CC_Q6_CMD_RCGR_IN          \
        in_dword(HWIO_MSS_CC_Q6_CMD_RCGR_ADDR)
#define HWIO_MSS_CC_Q6_CMD_RCGR_INM(m)      \
        in_dword_masked(HWIO_MSS_CC_Q6_CMD_RCGR_ADDR, m)
#define HWIO_MSS_CC_Q6_CMD_RCGR_OUT(v)      \
        out_dword(HWIO_MSS_CC_Q6_CMD_RCGR_ADDR,v)
#define HWIO_MSS_CC_Q6_CMD_RCGR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_CC_Q6_CMD_RCGR_ADDR,m,v,HWIO_MSS_CC_Q6_CMD_RCGR_IN)
#define HWIO_MSS_CC_Q6_CMD_RCGR_ROOT_OFF_BMSK                                                  0x80000000
#define HWIO_MSS_CC_Q6_CMD_RCGR_ROOT_OFF_SHFT                                                        0x1f
#define HWIO_MSS_CC_Q6_CMD_RCGR_DIRTY_CFG_RCGR_BMSK                                                  0x10
#define HWIO_MSS_CC_Q6_CMD_RCGR_DIRTY_CFG_RCGR_SHFT                                                   0x4
#define HWIO_MSS_CC_Q6_CMD_RCGR_ROOT_EN_BMSK                                                          0x2
#define HWIO_MSS_CC_Q6_CMD_RCGR_ROOT_EN_SHFT                                                          0x1
#define HWIO_MSS_CC_Q6_CMD_RCGR_UPDATE_BMSK                                                           0x1
#define HWIO_MSS_CC_Q6_CMD_RCGR_UPDATE_SHFT                                                           0x0

#define HWIO_MSS_CC_Q6_CFG_RCGR_ADDR                                                           (MSS_CC_MSS_CC_REG_REG_BASE      + 0x00000568)
#define HWIO_MSS_CC_Q6_CFG_RCGR_RMSK                                                             0x11071f
#define HWIO_MSS_CC_Q6_CFG_RCGR_IN          \
        in_dword(HWIO_MSS_CC_Q6_CFG_RCGR_ADDR)
#define HWIO_MSS_CC_Q6_CFG_RCGR_INM(m)      \
        in_dword_masked(HWIO_MSS_CC_Q6_CFG_RCGR_ADDR, m)
#define HWIO_MSS_CC_Q6_CFG_RCGR_OUT(v)      \
        out_dword(HWIO_MSS_CC_Q6_CFG_RCGR_ADDR,v)
#define HWIO_MSS_CC_Q6_CFG_RCGR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_CC_Q6_CFG_RCGR_ADDR,m,v,HWIO_MSS_CC_Q6_CFG_RCGR_IN)
#define HWIO_MSS_CC_Q6_CFG_RCGR_HW_CLK_CONTROL_BMSK                                              0x100000
#define HWIO_MSS_CC_Q6_CFG_RCGR_HW_CLK_CONTROL_SHFT                                                  0x14
#define HWIO_MSS_CC_Q6_CFG_RCGR_RCGLITE_DISABLE_BMSK                                              0x10000
#define HWIO_MSS_CC_Q6_CFG_RCGR_RCGLITE_DISABLE_SHFT                                                 0x10
#define HWIO_MSS_CC_Q6_CFG_RCGR_SRC_SEL_BMSK                                                        0x700
#define HWIO_MSS_CC_Q6_CFG_RCGR_SRC_SEL_SHFT                                                          0x8
#define HWIO_MSS_CC_Q6_CFG_RCGR_SRC_DIV_BMSK                                                         0x1f
#define HWIO_MSS_CC_Q6_CFG_RCGR_SRC_DIV_SHFT                                                          0x0

#define HWIO_MSS_CC_SILVER_CMD_RCGR_ADDR                                                       (MSS_CC_MSS_CC_REG_REG_BASE      + 0x00000578)
#define HWIO_MSS_CC_SILVER_CMD_RCGR_RMSK                                                       0x80000013
#define HWIO_MSS_CC_SILVER_CMD_RCGR_IN          \
        in_dword(HWIO_MSS_CC_SILVER_CMD_RCGR_ADDR)
#define HWIO_MSS_CC_SILVER_CMD_RCGR_INM(m)      \
        in_dword_masked(HWIO_MSS_CC_SILVER_CMD_RCGR_ADDR, m)
#define HWIO_MSS_CC_SILVER_CMD_RCGR_OUT(v)      \
        out_dword(HWIO_MSS_CC_SILVER_CMD_RCGR_ADDR,v)
#define HWIO_MSS_CC_SILVER_CMD_RCGR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_CC_SILVER_CMD_RCGR_ADDR,m,v,HWIO_MSS_CC_SILVER_CMD_RCGR_IN)
#define HWIO_MSS_CC_SILVER_CMD_RCGR_ROOT_OFF_BMSK                                              0x80000000
#define HWIO_MSS_CC_SILVER_CMD_RCGR_ROOT_OFF_SHFT                                                    0x1f
#define HWIO_MSS_CC_SILVER_CMD_RCGR_DIRTY_CFG_RCGR_BMSK                                              0x10
#define HWIO_MSS_CC_SILVER_CMD_RCGR_DIRTY_CFG_RCGR_SHFT                                               0x4
#define HWIO_MSS_CC_SILVER_CMD_RCGR_ROOT_EN_BMSK                                                      0x2
#define HWIO_MSS_CC_SILVER_CMD_RCGR_ROOT_EN_SHFT                                                      0x1
#define HWIO_MSS_CC_SILVER_CMD_RCGR_UPDATE_BMSK                                                       0x1
#define HWIO_MSS_CC_SILVER_CMD_RCGR_UPDATE_SHFT                                                       0x0

#define HWIO_MSS_CC_SILVER_CFG_RCGR_ADDR                                                       (MSS_CC_MSS_CC_REG_REG_BASE      + 0x0000057c)
#define HWIO_MSS_CC_SILVER_CFG_RCGR_RMSK                                                         0x11071f
#define HWIO_MSS_CC_SILVER_CFG_RCGR_IN          \
        in_dword(HWIO_MSS_CC_SILVER_CFG_RCGR_ADDR)
#define HWIO_MSS_CC_SILVER_CFG_RCGR_INM(m)      \
        in_dword_masked(HWIO_MSS_CC_SILVER_CFG_RCGR_ADDR, m)
#define HWIO_MSS_CC_SILVER_CFG_RCGR_OUT(v)      \
        out_dword(HWIO_MSS_CC_SILVER_CFG_RCGR_ADDR,v)
#define HWIO_MSS_CC_SILVER_CFG_RCGR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_CC_SILVER_CFG_RCGR_ADDR,m,v,HWIO_MSS_CC_SILVER_CFG_RCGR_IN)
#define HWIO_MSS_CC_SILVER_CFG_RCGR_HW_CLK_CONTROL_BMSK                                          0x100000
#define HWIO_MSS_CC_SILVER_CFG_RCGR_HW_CLK_CONTROL_SHFT                                              0x14
#define HWIO_MSS_CC_SILVER_CFG_RCGR_RCGLITE_DISABLE_BMSK                                          0x10000
#define HWIO_MSS_CC_SILVER_CFG_RCGR_RCGLITE_DISABLE_SHFT                                             0x10
#define HWIO_MSS_CC_SILVER_CFG_RCGR_SRC_SEL_BMSK                                                    0x700
#define HWIO_MSS_CC_SILVER_CFG_RCGR_SRC_SEL_SHFT                                                      0x8
#define HWIO_MSS_CC_SILVER_CFG_RCGR_SRC_DIV_BMSK                                                     0x1f
#define HWIO_MSS_CC_SILVER_CFG_RCGR_SRC_DIV_SHFT                                                      0x0

#define HWIO_MSS_CC_Q6_BUS_CBCR_ADDR                                                           (MSS_CC_MSS_CC_REG_REG_BASE      + 0x0000058c)
#define HWIO_MSS_CC_Q6_BUS_CBCR_RMSK                                                           0x80000005
#define HWIO_MSS_CC_Q6_BUS_CBCR_IN          \
        in_dword(HWIO_MSS_CC_Q6_BUS_CBCR_ADDR)
#define HWIO_MSS_CC_Q6_BUS_CBCR_INM(m)      \
        in_dword_masked(HWIO_MSS_CC_Q6_BUS_CBCR_ADDR, m)
#define HWIO_MSS_CC_Q6_BUS_CBCR_OUT(v)      \
        out_dword(HWIO_MSS_CC_Q6_BUS_CBCR_ADDR,v)
#define HWIO_MSS_CC_Q6_BUS_CBCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_CC_Q6_BUS_CBCR_ADDR,m,v,HWIO_MSS_CC_Q6_BUS_CBCR_IN)
#define HWIO_MSS_CC_Q6_BUS_CBCR_CLK_OFF_BMSK                                                   0x80000000
#define HWIO_MSS_CC_Q6_BUS_CBCR_CLK_OFF_SHFT                                                         0x1f
#define HWIO_MSS_CC_Q6_BUS_CBCR_CLK_ARES_BMSK                                                         0x4
#define HWIO_MSS_CC_Q6_BUS_CBCR_CLK_ARES_SHFT                                                         0x2
#define HWIO_MSS_CC_Q6_BUS_CBCR_CLK_ENABLE_BMSK                                                       0x1
#define HWIO_MSS_CC_Q6_BUS_CBCR_CLK_ENABLE_SHFT                                                       0x0

#define HWIO_MSS_CC_Q6_AXIS_CBCR_ADDR                                                          (MSS_CC_MSS_CC_REG_REG_BASE      + 0x0000099c)
#define HWIO_MSS_CC_Q6_AXIS_CBCR_RMSK                                                          0x80000007
#define HWIO_MSS_CC_Q6_AXIS_CBCR_IN          \
        in_dword(HWIO_MSS_CC_Q6_AXIS_CBCR_ADDR)
#define HWIO_MSS_CC_Q6_AXIS_CBCR_INM(m)      \
        in_dword_masked(HWIO_MSS_CC_Q6_AXIS_CBCR_ADDR, m)
#define HWIO_MSS_CC_Q6_AXIS_CBCR_OUT(v)      \
        out_dword(HWIO_MSS_CC_Q6_AXIS_CBCR_ADDR,v)
#define HWIO_MSS_CC_Q6_AXIS_CBCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_CC_Q6_AXIS_CBCR_ADDR,m,v,HWIO_MSS_CC_Q6_AXIS_CBCR_IN)
#define HWIO_MSS_CC_Q6_AXIS_CBCR_CLK_OFF_BMSK                                                  0x80000000
#define HWIO_MSS_CC_Q6_AXIS_CBCR_CLK_OFF_SHFT                                                        0x1f
#define HWIO_MSS_CC_Q6_AXIS_CBCR_CLK_ARES_BMSK                                                        0x4
#define HWIO_MSS_CC_Q6_AXIS_CBCR_CLK_ARES_SHFT                                                        0x2
#define HWIO_MSS_CC_Q6_AXIS_CBCR_HW_CTL_BMSK                                                          0x2
#define HWIO_MSS_CC_Q6_AXIS_CBCR_HW_CTL_SHFT                                                          0x1
#define HWIO_MSS_CC_Q6_AXIS_CBCR_CLK_ENABLE_BMSK                                                      0x1
#define HWIO_MSS_CC_Q6_AXIS_CBCR_CLK_ENABLE_SHFT                                                      0x0

#define HWIO_MSS_CC_UIM_DIV_CDIVR_ADDR                                                         (MSS_CC_MSS_CC_REG_REG_BASE      + 0x00000654)
#define HWIO_MSS_CC_UIM_DIV_CDIVR_RMSK                                                                0xf
#define HWIO_MSS_CC_UIM_DIV_CDIVR_IN          \
        in_dword(HWIO_MSS_CC_UIM_DIV_CDIVR_ADDR)
#define HWIO_MSS_CC_UIM_DIV_CDIVR_INM(m)      \
        in_dword_masked(HWIO_MSS_CC_UIM_DIV_CDIVR_ADDR, m)
#define HWIO_MSS_CC_UIM_DIV_CDIVR_OUT(v)      \
        out_dword(HWIO_MSS_CC_UIM_DIV_CDIVR_ADDR,v)
#define HWIO_MSS_CC_UIM_DIV_CDIVR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_CC_UIM_DIV_CDIVR_ADDR,m,v,HWIO_MSS_CC_UIM_DIV_CDIVR_IN)
#define HWIO_MSS_CC_UIM_DIV_CDIVR_CLK_DIV_BMSK                                                        0xf
#define HWIO_MSS_CC_UIM_DIV_CDIVR_CLK_DIV_SHFT                                                        0x0

#define HWIO_MSS_CC_UIM0_MND_CMD_RCGR_ADDR                                                     (MSS_CC_MSS_CC_REG_REG_BASE      + 0x0000067c)
#define HWIO_MSS_CC_UIM0_MND_CMD_RCGR_RMSK                                                     0x800000f3
#define HWIO_MSS_CC_UIM0_MND_CMD_RCGR_IN          \
        in_dword(HWIO_MSS_CC_UIM0_MND_CMD_RCGR_ADDR)
#define HWIO_MSS_CC_UIM0_MND_CMD_RCGR_INM(m)      \
        in_dword_masked(HWIO_MSS_CC_UIM0_MND_CMD_RCGR_ADDR, m)
#define HWIO_MSS_CC_UIM0_MND_CMD_RCGR_OUT(v)      \
        out_dword(HWIO_MSS_CC_UIM0_MND_CMD_RCGR_ADDR,v)
#define HWIO_MSS_CC_UIM0_MND_CMD_RCGR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_CC_UIM0_MND_CMD_RCGR_ADDR,m,v,HWIO_MSS_CC_UIM0_MND_CMD_RCGR_IN)
#define HWIO_MSS_CC_UIM0_MND_CMD_RCGR_ROOT_OFF_BMSK                                            0x80000000
#define HWIO_MSS_CC_UIM0_MND_CMD_RCGR_ROOT_OFF_SHFT                                                  0x1f
#define HWIO_MSS_CC_UIM0_MND_CMD_RCGR_DIRTY_D_BMSK                                                   0x80
#define HWIO_MSS_CC_UIM0_MND_CMD_RCGR_DIRTY_D_SHFT                                                    0x7
#define HWIO_MSS_CC_UIM0_MND_CMD_RCGR_DIRTY_N_BMSK                                                   0x40
#define HWIO_MSS_CC_UIM0_MND_CMD_RCGR_DIRTY_N_SHFT                                                    0x6
#define HWIO_MSS_CC_UIM0_MND_CMD_RCGR_DIRTY_M_BMSK                                                   0x20
#define HWIO_MSS_CC_UIM0_MND_CMD_RCGR_DIRTY_M_SHFT                                                    0x5
#define HWIO_MSS_CC_UIM0_MND_CMD_RCGR_DIRTY_CFG_RCGR_BMSK                                            0x10
#define HWIO_MSS_CC_UIM0_MND_CMD_RCGR_DIRTY_CFG_RCGR_SHFT                                             0x4
#define HWIO_MSS_CC_UIM0_MND_CMD_RCGR_ROOT_EN_BMSK                                                    0x2
#define HWIO_MSS_CC_UIM0_MND_CMD_RCGR_ROOT_EN_SHFT                                                    0x1
#define HWIO_MSS_CC_UIM0_MND_CMD_RCGR_UPDATE_BMSK                                                     0x1
#define HWIO_MSS_CC_UIM0_MND_CMD_RCGR_UPDATE_SHFT                                                     0x0

#define HWIO_MSS_CC_UIM0_MND_CFG_RCGR_ADDR                                                     (MSS_CC_MSS_CC_REG_REG_BASE      + 0x00000680)
#define HWIO_MSS_CC_UIM0_MND_CFG_RCGR_RMSK                                                       0x10371f
#define HWIO_MSS_CC_UIM0_MND_CFG_RCGR_IN          \
        in_dword(HWIO_MSS_CC_UIM0_MND_CFG_RCGR_ADDR)
#define HWIO_MSS_CC_UIM0_MND_CFG_RCGR_INM(m)      \
        in_dword_masked(HWIO_MSS_CC_UIM0_MND_CFG_RCGR_ADDR, m)
#define HWIO_MSS_CC_UIM0_MND_CFG_RCGR_OUT(v)      \
        out_dword(HWIO_MSS_CC_UIM0_MND_CFG_RCGR_ADDR,v)
#define HWIO_MSS_CC_UIM0_MND_CFG_RCGR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_CC_UIM0_MND_CFG_RCGR_ADDR,m,v,HWIO_MSS_CC_UIM0_MND_CFG_RCGR_IN)
#define HWIO_MSS_CC_UIM0_MND_CFG_RCGR_HW_CLK_CONTROL_BMSK                                        0x100000
#define HWIO_MSS_CC_UIM0_MND_CFG_RCGR_HW_CLK_CONTROL_SHFT                                            0x14
#define HWIO_MSS_CC_UIM0_MND_CFG_RCGR_MODE_BMSK                                                    0x3000
#define HWIO_MSS_CC_UIM0_MND_CFG_RCGR_MODE_SHFT                                                       0xc
#define HWIO_MSS_CC_UIM0_MND_CFG_RCGR_SRC_SEL_BMSK                                                  0x700
#define HWIO_MSS_CC_UIM0_MND_CFG_RCGR_SRC_SEL_SHFT                                                    0x8
#define HWIO_MSS_CC_UIM0_MND_CFG_RCGR_SRC_DIV_BMSK                                                   0x1f
#define HWIO_MSS_CC_UIM0_MND_CFG_RCGR_SRC_DIV_SHFT                                                    0x0

#define HWIO_MSS_CC_UIM0_MND_M_ADDR                                                            (MSS_CC_MSS_CC_REG_REG_BASE      + 0x00000684)
#define HWIO_MSS_CC_UIM0_MND_M_RMSK                                                                0xffff
#define HWIO_MSS_CC_UIM0_MND_M_IN          \
        in_dword(HWIO_MSS_CC_UIM0_MND_M_ADDR)
#define HWIO_MSS_CC_UIM0_MND_M_INM(m)      \
        in_dword_masked(HWIO_MSS_CC_UIM0_MND_M_ADDR, m)
#define HWIO_MSS_CC_UIM0_MND_M_OUT(v)      \
        out_dword(HWIO_MSS_CC_UIM0_MND_M_ADDR,v)
#define HWIO_MSS_CC_UIM0_MND_M_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_CC_UIM0_MND_M_ADDR,m,v,HWIO_MSS_CC_UIM0_MND_M_IN)
#define HWIO_MSS_CC_UIM0_MND_M_M_BMSK                                                              0xffff
#define HWIO_MSS_CC_UIM0_MND_M_M_SHFT                                                                 0x0

#define HWIO_MSS_CC_UIM0_MND_N_ADDR                                                            (MSS_CC_MSS_CC_REG_REG_BASE      + 0x00000688)
#define HWIO_MSS_CC_UIM0_MND_N_RMSK                                                                0xffff
#define HWIO_MSS_CC_UIM0_MND_N_IN          \
        in_dword(HWIO_MSS_CC_UIM0_MND_N_ADDR)
#define HWIO_MSS_CC_UIM0_MND_N_INM(m)      \
        in_dword_masked(HWIO_MSS_CC_UIM0_MND_N_ADDR, m)
#define HWIO_MSS_CC_UIM0_MND_N_OUT(v)      \
        out_dword(HWIO_MSS_CC_UIM0_MND_N_ADDR,v)
#define HWIO_MSS_CC_UIM0_MND_N_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_CC_UIM0_MND_N_ADDR,m,v,HWIO_MSS_CC_UIM0_MND_N_IN)
#define HWIO_MSS_CC_UIM0_MND_N_NOT_N_MINUS_M_BMSK                                                  0xffff
#define HWIO_MSS_CC_UIM0_MND_N_NOT_N_MINUS_M_SHFT                                                     0x0

#define HWIO_MSS_CC_UIM0_MND_D_ADDR                                                            (MSS_CC_MSS_CC_REG_REG_BASE      + 0x0000068c)
#define HWIO_MSS_CC_UIM0_MND_D_RMSK                                                                0xffff
#define HWIO_MSS_CC_UIM0_MND_D_IN          \
        in_dword(HWIO_MSS_CC_UIM0_MND_D_ADDR)
#define HWIO_MSS_CC_UIM0_MND_D_INM(m)      \
        in_dword_masked(HWIO_MSS_CC_UIM0_MND_D_ADDR, m)
#define HWIO_MSS_CC_UIM0_MND_D_OUT(v)      \
        out_dword(HWIO_MSS_CC_UIM0_MND_D_ADDR,v)
#define HWIO_MSS_CC_UIM0_MND_D_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_CC_UIM0_MND_D_ADDR,m,v,HWIO_MSS_CC_UIM0_MND_D_IN)
#define HWIO_MSS_CC_UIM0_MND_D_NOT_2D_BMSK                                                         0xffff
#define HWIO_MSS_CC_UIM0_MND_D_NOT_2D_SHFT                                                            0x0

#define HWIO_MSS_CC_UIM1_MND_CMD_RCGR_ADDR                                                     (MSS_CC_MSS_CC_REG_REG_BASE      + 0x00000690)
#define HWIO_MSS_CC_UIM1_MND_CMD_RCGR_RMSK                                                     0x800000f3
#define HWIO_MSS_CC_UIM1_MND_CMD_RCGR_IN          \
        in_dword(HWIO_MSS_CC_UIM1_MND_CMD_RCGR_ADDR)
#define HWIO_MSS_CC_UIM1_MND_CMD_RCGR_INM(m)      \
        in_dword_masked(HWIO_MSS_CC_UIM1_MND_CMD_RCGR_ADDR, m)
#define HWIO_MSS_CC_UIM1_MND_CMD_RCGR_OUT(v)      \
        out_dword(HWIO_MSS_CC_UIM1_MND_CMD_RCGR_ADDR,v)
#define HWIO_MSS_CC_UIM1_MND_CMD_RCGR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_CC_UIM1_MND_CMD_RCGR_ADDR,m,v,HWIO_MSS_CC_UIM1_MND_CMD_RCGR_IN)
#define HWIO_MSS_CC_UIM1_MND_CMD_RCGR_ROOT_OFF_BMSK                                            0x80000000
#define HWIO_MSS_CC_UIM1_MND_CMD_RCGR_ROOT_OFF_SHFT                                                  0x1f
#define HWIO_MSS_CC_UIM1_MND_CMD_RCGR_DIRTY_D_BMSK                                                   0x80
#define HWIO_MSS_CC_UIM1_MND_CMD_RCGR_DIRTY_D_SHFT                                                    0x7
#define HWIO_MSS_CC_UIM1_MND_CMD_RCGR_DIRTY_N_BMSK                                                   0x40
#define HWIO_MSS_CC_UIM1_MND_CMD_RCGR_DIRTY_N_SHFT                                                    0x6
#define HWIO_MSS_CC_UIM1_MND_CMD_RCGR_DIRTY_M_BMSK                                                   0x20
#define HWIO_MSS_CC_UIM1_MND_CMD_RCGR_DIRTY_M_SHFT                                                    0x5
#define HWIO_MSS_CC_UIM1_MND_CMD_RCGR_DIRTY_CFG_RCGR_BMSK                                            0x10
#define HWIO_MSS_CC_UIM1_MND_CMD_RCGR_DIRTY_CFG_RCGR_SHFT                                             0x4
#define HWIO_MSS_CC_UIM1_MND_CMD_RCGR_ROOT_EN_BMSK                                                    0x2
#define HWIO_MSS_CC_UIM1_MND_CMD_RCGR_ROOT_EN_SHFT                                                    0x1
#define HWIO_MSS_CC_UIM1_MND_CMD_RCGR_UPDATE_BMSK                                                     0x1
#define HWIO_MSS_CC_UIM1_MND_CMD_RCGR_UPDATE_SHFT                                                     0x0

#define HWIO_MSS_CC_UIM1_MND_CFG_RCGR_ADDR                                                     (MSS_CC_MSS_CC_REG_REG_BASE      + 0x00000694)
#define HWIO_MSS_CC_UIM1_MND_CFG_RCGR_RMSK                                                       0x10371f
#define HWIO_MSS_CC_UIM1_MND_CFG_RCGR_IN          \
        in_dword(HWIO_MSS_CC_UIM1_MND_CFG_RCGR_ADDR)
#define HWIO_MSS_CC_UIM1_MND_CFG_RCGR_INM(m)      \
        in_dword_masked(HWIO_MSS_CC_UIM1_MND_CFG_RCGR_ADDR, m)
#define HWIO_MSS_CC_UIM1_MND_CFG_RCGR_OUT(v)      \
        out_dword(HWIO_MSS_CC_UIM1_MND_CFG_RCGR_ADDR,v)
#define HWIO_MSS_CC_UIM1_MND_CFG_RCGR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_CC_UIM1_MND_CFG_RCGR_ADDR,m,v,HWIO_MSS_CC_UIM1_MND_CFG_RCGR_IN)
#define HWIO_MSS_CC_UIM1_MND_CFG_RCGR_HW_CLK_CONTROL_BMSK                                        0x100000
#define HWIO_MSS_CC_UIM1_MND_CFG_RCGR_HW_CLK_CONTROL_SHFT                                            0x14
#define HWIO_MSS_CC_UIM1_MND_CFG_RCGR_MODE_BMSK                                                    0x3000
#define HWIO_MSS_CC_UIM1_MND_CFG_RCGR_MODE_SHFT                                                       0xc
#define HWIO_MSS_CC_UIM1_MND_CFG_RCGR_SRC_SEL_BMSK                                                  0x700
#define HWIO_MSS_CC_UIM1_MND_CFG_RCGR_SRC_SEL_SHFT                                                    0x8
#define HWIO_MSS_CC_UIM1_MND_CFG_RCGR_SRC_DIV_BMSK                                                   0x1f
#define HWIO_MSS_CC_UIM1_MND_CFG_RCGR_SRC_DIV_SHFT                                                    0x0

#define HWIO_MSS_CC_UIM1_MND_M_ADDR                                                            (MSS_CC_MSS_CC_REG_REG_BASE      + 0x00000698)
#define HWIO_MSS_CC_UIM1_MND_M_RMSK                                                                0xffff
#define HWIO_MSS_CC_UIM1_MND_M_IN          \
        in_dword(HWIO_MSS_CC_UIM1_MND_M_ADDR)
#define HWIO_MSS_CC_UIM1_MND_M_INM(m)      \
        in_dword_masked(HWIO_MSS_CC_UIM1_MND_M_ADDR, m)
#define HWIO_MSS_CC_UIM1_MND_M_OUT(v)      \
        out_dword(HWIO_MSS_CC_UIM1_MND_M_ADDR,v)
#define HWIO_MSS_CC_UIM1_MND_M_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_CC_UIM1_MND_M_ADDR,m,v,HWIO_MSS_CC_UIM1_MND_M_IN)
#define HWIO_MSS_CC_UIM1_MND_M_M_BMSK                                                              0xffff
#define HWIO_MSS_CC_UIM1_MND_M_M_SHFT                                                                 0x0

#define HWIO_MSS_CC_UIM1_MND_N_ADDR                                                            (MSS_CC_MSS_CC_REG_REG_BASE      + 0x0000069c)
#define HWIO_MSS_CC_UIM1_MND_N_RMSK                                                                0xffff
#define HWIO_MSS_CC_UIM1_MND_N_IN          \
        in_dword(HWIO_MSS_CC_UIM1_MND_N_ADDR)
#define HWIO_MSS_CC_UIM1_MND_N_INM(m)      \
        in_dword_masked(HWIO_MSS_CC_UIM1_MND_N_ADDR, m)
#define HWIO_MSS_CC_UIM1_MND_N_OUT(v)      \
        out_dword(HWIO_MSS_CC_UIM1_MND_N_ADDR,v)
#define HWIO_MSS_CC_UIM1_MND_N_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_CC_UIM1_MND_N_ADDR,m,v,HWIO_MSS_CC_UIM1_MND_N_IN)
#define HWIO_MSS_CC_UIM1_MND_N_NOT_N_MINUS_M_BMSK                                                  0xffff
#define HWIO_MSS_CC_UIM1_MND_N_NOT_N_MINUS_M_SHFT                                                     0x0

#define HWIO_MSS_CC_UIM1_MND_D_ADDR                                                            (MSS_CC_MSS_CC_REG_REG_BASE      + 0x000006a0)
#define HWIO_MSS_CC_UIM1_MND_D_RMSK                                                                0xffff
#define HWIO_MSS_CC_UIM1_MND_D_IN          \
        in_dword(HWIO_MSS_CC_UIM1_MND_D_ADDR)
#define HWIO_MSS_CC_UIM1_MND_D_INM(m)      \
        in_dword_masked(HWIO_MSS_CC_UIM1_MND_D_ADDR, m)
#define HWIO_MSS_CC_UIM1_MND_D_OUT(v)      \
        out_dword(HWIO_MSS_CC_UIM1_MND_D_ADDR,v)
#define HWIO_MSS_CC_UIM1_MND_D_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_CC_UIM1_MND_D_ADDR,m,v,HWIO_MSS_CC_UIM1_MND_D_IN)
#define HWIO_MSS_CC_UIM1_MND_D_NOT_2D_BMSK                                                         0xffff
#define HWIO_MSS_CC_UIM1_MND_D_NOT_2D_SHFT                                                            0x0

#define HWIO_MSS_CC_UIM0_BCR_ADDR                                                              (MSS_CC_MSS_CC_REG_REG_BASE      + 0x000006b8)
#define HWIO_MSS_CC_UIM0_BCR_RMSK                                                                     0x1
#define HWIO_MSS_CC_UIM0_BCR_IN          \
        in_dword(HWIO_MSS_CC_UIM0_BCR_ADDR)
#define HWIO_MSS_CC_UIM0_BCR_INM(m)      \
        in_dword_masked(HWIO_MSS_CC_UIM0_BCR_ADDR, m)
#define HWIO_MSS_CC_UIM0_BCR_OUT(v)      \
        out_dword(HWIO_MSS_CC_UIM0_BCR_ADDR,v)
#define HWIO_MSS_CC_UIM0_BCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_CC_UIM0_BCR_ADDR,m,v,HWIO_MSS_CC_UIM0_BCR_IN)
#define HWIO_MSS_CC_UIM0_BCR_BLK_ARES_BMSK                                                            0x1
#define HWIO_MSS_CC_UIM0_BCR_BLK_ARES_SHFT                                                            0x0

#define HWIO_MSS_CC_UIM0_CBCR_ADDR                                                             (MSS_CC_MSS_CC_REG_REG_BASE      + 0x000006cc)
#define HWIO_MSS_CC_UIM0_CBCR_RMSK                                                             0x80000005
#define HWIO_MSS_CC_UIM0_CBCR_IN          \
        in_dword(HWIO_MSS_CC_UIM0_CBCR_ADDR)
#define HWIO_MSS_CC_UIM0_CBCR_INM(m)      \
        in_dword_masked(HWIO_MSS_CC_UIM0_CBCR_ADDR, m)
#define HWIO_MSS_CC_UIM0_CBCR_OUT(v)      \
        out_dword(HWIO_MSS_CC_UIM0_CBCR_ADDR,v)
#define HWIO_MSS_CC_UIM0_CBCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_CC_UIM0_CBCR_ADDR,m,v,HWIO_MSS_CC_UIM0_CBCR_IN)
#define HWIO_MSS_CC_UIM0_CBCR_CLK_OFF_BMSK                                                     0x80000000
#define HWIO_MSS_CC_UIM0_CBCR_CLK_OFF_SHFT                                                           0x1f
#define HWIO_MSS_CC_UIM0_CBCR_CLK_ARES_BMSK                                                           0x4
#define HWIO_MSS_CC_UIM0_CBCR_CLK_ARES_SHFT                                                           0x2
#define HWIO_MSS_CC_UIM0_CBCR_CLK_ENABLE_BMSK                                                         0x1
#define HWIO_MSS_CC_UIM0_CBCR_CLK_ENABLE_SHFT                                                         0x0

#define HWIO_MSS_CC_UIM0_XO_CBCR_ADDR                                                          (MSS_CC_MSS_CC_REG_REG_BASE      + 0x000006e0)
#define HWIO_MSS_CC_UIM0_XO_CBCR_RMSK                                                          0x80000005
#define HWIO_MSS_CC_UIM0_XO_CBCR_IN          \
        in_dword(HWIO_MSS_CC_UIM0_XO_CBCR_ADDR)
#define HWIO_MSS_CC_UIM0_XO_CBCR_INM(m)      \
        in_dword_masked(HWIO_MSS_CC_UIM0_XO_CBCR_ADDR, m)
#define HWIO_MSS_CC_UIM0_XO_CBCR_OUT(v)      \
        out_dword(HWIO_MSS_CC_UIM0_XO_CBCR_ADDR,v)
#define HWIO_MSS_CC_UIM0_XO_CBCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_CC_UIM0_XO_CBCR_ADDR,m,v,HWIO_MSS_CC_UIM0_XO_CBCR_IN)
#define HWIO_MSS_CC_UIM0_XO_CBCR_CLK_OFF_BMSK                                                  0x80000000
#define HWIO_MSS_CC_UIM0_XO_CBCR_CLK_OFF_SHFT                                                        0x1f
#define HWIO_MSS_CC_UIM0_XO_CBCR_CLK_ARES_BMSK                                                        0x4
#define HWIO_MSS_CC_UIM0_XO_CBCR_CLK_ARES_SHFT                                                        0x2
#define HWIO_MSS_CC_UIM0_XO_CBCR_CLK_ENABLE_BMSK                                                      0x1
#define HWIO_MSS_CC_UIM0_XO_CBCR_CLK_ENABLE_SHFT                                                      0x0

#define HWIO_MSS_CC_UIM1_BCR_ADDR                                                              (MSS_CC_MSS_CC_REG_REG_BASE      + 0x000006f4)
#define HWIO_MSS_CC_UIM1_BCR_RMSK                                                                     0x1
#define HWIO_MSS_CC_UIM1_BCR_IN          \
        in_dword(HWIO_MSS_CC_UIM1_BCR_ADDR)
#define HWIO_MSS_CC_UIM1_BCR_INM(m)      \
        in_dword_masked(HWIO_MSS_CC_UIM1_BCR_ADDR, m)
#define HWIO_MSS_CC_UIM1_BCR_OUT(v)      \
        out_dword(HWIO_MSS_CC_UIM1_BCR_ADDR,v)
#define HWIO_MSS_CC_UIM1_BCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_CC_UIM1_BCR_ADDR,m,v,HWIO_MSS_CC_UIM1_BCR_IN)
#define HWIO_MSS_CC_UIM1_BCR_BLK_ARES_BMSK                                                            0x1
#define HWIO_MSS_CC_UIM1_BCR_BLK_ARES_SHFT                                                            0x0

#define HWIO_MSS_CC_UIM1_CBCR_ADDR                                                             (MSS_CC_MSS_CC_REG_REG_BASE      + 0x00000708)
#define HWIO_MSS_CC_UIM1_CBCR_RMSK                                                             0x80000005
#define HWIO_MSS_CC_UIM1_CBCR_IN          \
        in_dword(HWIO_MSS_CC_UIM1_CBCR_ADDR)
#define HWIO_MSS_CC_UIM1_CBCR_INM(m)      \
        in_dword_masked(HWIO_MSS_CC_UIM1_CBCR_ADDR, m)
#define HWIO_MSS_CC_UIM1_CBCR_OUT(v)      \
        out_dword(HWIO_MSS_CC_UIM1_CBCR_ADDR,v)
#define HWIO_MSS_CC_UIM1_CBCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_CC_UIM1_CBCR_ADDR,m,v,HWIO_MSS_CC_UIM1_CBCR_IN)
#define HWIO_MSS_CC_UIM1_CBCR_CLK_OFF_BMSK                                                     0x80000000
#define HWIO_MSS_CC_UIM1_CBCR_CLK_OFF_SHFT                                                           0x1f
#define HWIO_MSS_CC_UIM1_CBCR_CLK_ARES_BMSK                                                           0x4
#define HWIO_MSS_CC_UIM1_CBCR_CLK_ARES_SHFT                                                           0x2
#define HWIO_MSS_CC_UIM1_CBCR_CLK_ENABLE_BMSK                                                         0x1
#define HWIO_MSS_CC_UIM1_CBCR_CLK_ENABLE_SHFT                                                         0x0

#define HWIO_MSS_CC_UIM1_XO_CBCR_ADDR                                                          (MSS_CC_MSS_CC_REG_REG_BASE      + 0x0000071c)
#define HWIO_MSS_CC_UIM1_XO_CBCR_RMSK                                                          0x80000005
#define HWIO_MSS_CC_UIM1_XO_CBCR_IN          \
        in_dword(HWIO_MSS_CC_UIM1_XO_CBCR_ADDR)
#define HWIO_MSS_CC_UIM1_XO_CBCR_INM(m)      \
        in_dword_masked(HWIO_MSS_CC_UIM1_XO_CBCR_ADDR, m)
#define HWIO_MSS_CC_UIM1_XO_CBCR_OUT(v)      \
        out_dword(HWIO_MSS_CC_UIM1_XO_CBCR_ADDR,v)
#define HWIO_MSS_CC_UIM1_XO_CBCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_CC_UIM1_XO_CBCR_ADDR,m,v,HWIO_MSS_CC_UIM1_XO_CBCR_IN)
#define HWIO_MSS_CC_UIM1_XO_CBCR_CLK_OFF_BMSK                                                  0x80000000
#define HWIO_MSS_CC_UIM1_XO_CBCR_CLK_OFF_SHFT                                                        0x1f
#define HWIO_MSS_CC_UIM1_XO_CBCR_CLK_ARES_BMSK                                                        0x4
#define HWIO_MSS_CC_UIM1_XO_CBCR_CLK_ARES_SHFT                                                        0x2
#define HWIO_MSS_CC_UIM1_XO_CBCR_CLK_ENABLE_BMSK                                                      0x1
#define HWIO_MSS_CC_UIM1_XO_CBCR_CLK_ENABLE_SHFT                                                      0x0

#define HWIO_MSS_CC_COXM_BCR_ADDR                                                              (MSS_CC_MSS_CC_REG_REG_BASE      + 0x0000076c)
#define HWIO_MSS_CC_COXM_BCR_RMSK                                                                     0x1
#define HWIO_MSS_CC_COXM_BCR_IN          \
        in_dword(HWIO_MSS_CC_COXM_BCR_ADDR)
#define HWIO_MSS_CC_COXM_BCR_INM(m)      \
        in_dword_masked(HWIO_MSS_CC_COXM_BCR_ADDR, m)
#define HWIO_MSS_CC_COXM_BCR_OUT(v)      \
        out_dword(HWIO_MSS_CC_COXM_BCR_ADDR,v)
#define HWIO_MSS_CC_COXM_BCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_CC_COXM_BCR_ADDR,m,v,HWIO_MSS_CC_COXM_BCR_IN)
#define HWIO_MSS_CC_COXM_BCR_BLK_ARES_BMSK                                                            0x1
#define HWIO_MSS_CC_COXM_BCR_BLK_ARES_SHFT                                                            0x0

#define HWIO_MSS_CC_BIT_COXM_CMD_RCGR_ADDR                                                     (MSS_CC_MSS_CC_REG_REG_BASE      + 0x00000780)
#define HWIO_MSS_CC_BIT_COXM_CMD_RCGR_RMSK                                                     0x800000f3
#define HWIO_MSS_CC_BIT_COXM_CMD_RCGR_IN          \
        in_dword(HWIO_MSS_CC_BIT_COXM_CMD_RCGR_ADDR)
#define HWIO_MSS_CC_BIT_COXM_CMD_RCGR_INM(m)      \
        in_dword_masked(HWIO_MSS_CC_BIT_COXM_CMD_RCGR_ADDR, m)
#define HWIO_MSS_CC_BIT_COXM_CMD_RCGR_OUT(v)      \
        out_dword(HWIO_MSS_CC_BIT_COXM_CMD_RCGR_ADDR,v)
#define HWIO_MSS_CC_BIT_COXM_CMD_RCGR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_CC_BIT_COXM_CMD_RCGR_ADDR,m,v,HWIO_MSS_CC_BIT_COXM_CMD_RCGR_IN)
#define HWIO_MSS_CC_BIT_COXM_CMD_RCGR_ROOT_OFF_BMSK                                            0x80000000
#define HWIO_MSS_CC_BIT_COXM_CMD_RCGR_ROOT_OFF_SHFT                                                  0x1f
#define HWIO_MSS_CC_BIT_COXM_CMD_RCGR_DIRTY_D_BMSK                                                   0x80
#define HWIO_MSS_CC_BIT_COXM_CMD_RCGR_DIRTY_D_SHFT                                                    0x7
#define HWIO_MSS_CC_BIT_COXM_CMD_RCGR_DIRTY_N_BMSK                                                   0x40
#define HWIO_MSS_CC_BIT_COXM_CMD_RCGR_DIRTY_N_SHFT                                                    0x6
#define HWIO_MSS_CC_BIT_COXM_CMD_RCGR_DIRTY_M_BMSK                                                   0x20
#define HWIO_MSS_CC_BIT_COXM_CMD_RCGR_DIRTY_M_SHFT                                                    0x5
#define HWIO_MSS_CC_BIT_COXM_CMD_RCGR_DIRTY_CFG_RCGR_BMSK                                            0x10
#define HWIO_MSS_CC_BIT_COXM_CMD_RCGR_DIRTY_CFG_RCGR_SHFT                                             0x4
#define HWIO_MSS_CC_BIT_COXM_CMD_RCGR_ROOT_EN_BMSK                                                    0x2
#define HWIO_MSS_CC_BIT_COXM_CMD_RCGR_ROOT_EN_SHFT                                                    0x1
#define HWIO_MSS_CC_BIT_COXM_CMD_RCGR_UPDATE_BMSK                                                     0x1
#define HWIO_MSS_CC_BIT_COXM_CMD_RCGR_UPDATE_SHFT                                                     0x0

#define HWIO_MSS_CC_BIT_COXM_CFG_RCGR_ADDR                                                     (MSS_CC_MSS_CC_REG_REG_BASE      + 0x00000784)
#define HWIO_MSS_CC_BIT_COXM_CFG_RCGR_RMSK                                                       0x10371f
#define HWIO_MSS_CC_BIT_COXM_CFG_RCGR_IN          \
        in_dword(HWIO_MSS_CC_BIT_COXM_CFG_RCGR_ADDR)
#define HWIO_MSS_CC_BIT_COXM_CFG_RCGR_INM(m)      \
        in_dword_masked(HWIO_MSS_CC_BIT_COXM_CFG_RCGR_ADDR, m)
#define HWIO_MSS_CC_BIT_COXM_CFG_RCGR_OUT(v)      \
        out_dword(HWIO_MSS_CC_BIT_COXM_CFG_RCGR_ADDR,v)
#define HWIO_MSS_CC_BIT_COXM_CFG_RCGR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_CC_BIT_COXM_CFG_RCGR_ADDR,m,v,HWIO_MSS_CC_BIT_COXM_CFG_RCGR_IN)
#define HWIO_MSS_CC_BIT_COXM_CFG_RCGR_HW_CLK_CONTROL_BMSK                                        0x100000
#define HWIO_MSS_CC_BIT_COXM_CFG_RCGR_HW_CLK_CONTROL_SHFT                                            0x14
#define HWIO_MSS_CC_BIT_COXM_CFG_RCGR_MODE_BMSK                                                    0x3000
#define HWIO_MSS_CC_BIT_COXM_CFG_RCGR_MODE_SHFT                                                       0xc
#define HWIO_MSS_CC_BIT_COXM_CFG_RCGR_SRC_SEL_BMSK                                                  0x700
#define HWIO_MSS_CC_BIT_COXM_CFG_RCGR_SRC_SEL_SHFT                                                    0x8
#define HWIO_MSS_CC_BIT_COXM_CFG_RCGR_SRC_DIV_BMSK                                                   0x1f
#define HWIO_MSS_CC_BIT_COXM_CFG_RCGR_SRC_DIV_SHFT                                                    0x0

#define HWIO_MSS_CC_BIT_COXM_M_ADDR                                                            (MSS_CC_MSS_CC_REG_REG_BASE      + 0x00000788)
#define HWIO_MSS_CC_BIT_COXM_M_RMSK                                                                0xffff
#define HWIO_MSS_CC_BIT_COXM_M_IN          \
        in_dword(HWIO_MSS_CC_BIT_COXM_M_ADDR)
#define HWIO_MSS_CC_BIT_COXM_M_INM(m)      \
        in_dword_masked(HWIO_MSS_CC_BIT_COXM_M_ADDR, m)
#define HWIO_MSS_CC_BIT_COXM_M_OUT(v)      \
        out_dword(HWIO_MSS_CC_BIT_COXM_M_ADDR,v)
#define HWIO_MSS_CC_BIT_COXM_M_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_CC_BIT_COXM_M_ADDR,m,v,HWIO_MSS_CC_BIT_COXM_M_IN)
#define HWIO_MSS_CC_BIT_COXM_M_M_BMSK                                                              0xffff
#define HWIO_MSS_CC_BIT_COXM_M_M_SHFT                                                                 0x0

#define HWIO_MSS_CC_BIT_COXM_N_ADDR                                                            (MSS_CC_MSS_CC_REG_REG_BASE      + 0x0000078c)
#define HWIO_MSS_CC_BIT_COXM_N_RMSK                                                                0xffff
#define HWIO_MSS_CC_BIT_COXM_N_IN          \
        in_dword(HWIO_MSS_CC_BIT_COXM_N_ADDR)
#define HWIO_MSS_CC_BIT_COXM_N_INM(m)      \
        in_dword_masked(HWIO_MSS_CC_BIT_COXM_N_ADDR, m)
#define HWIO_MSS_CC_BIT_COXM_N_OUT(v)      \
        out_dword(HWIO_MSS_CC_BIT_COXM_N_ADDR,v)
#define HWIO_MSS_CC_BIT_COXM_N_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_CC_BIT_COXM_N_ADDR,m,v,HWIO_MSS_CC_BIT_COXM_N_IN)
#define HWIO_MSS_CC_BIT_COXM_N_NOT_N_MINUS_M_BMSK                                                  0xffff
#define HWIO_MSS_CC_BIT_COXM_N_NOT_N_MINUS_M_SHFT                                                     0x0

#define HWIO_MSS_CC_BIT_COXM_D_ADDR                                                            (MSS_CC_MSS_CC_REG_REG_BASE      + 0x00000790)
#define HWIO_MSS_CC_BIT_COXM_D_RMSK                                                                0xffff
#define HWIO_MSS_CC_BIT_COXM_D_IN          \
        in_dword(HWIO_MSS_CC_BIT_COXM_D_ADDR)
#define HWIO_MSS_CC_BIT_COXM_D_INM(m)      \
        in_dword_masked(HWIO_MSS_CC_BIT_COXM_D_ADDR, m)
#define HWIO_MSS_CC_BIT_COXM_D_OUT(v)      \
        out_dword(HWIO_MSS_CC_BIT_COXM_D_ADDR,v)
#define HWIO_MSS_CC_BIT_COXM_D_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_CC_BIT_COXM_D_ADDR,m,v,HWIO_MSS_CC_BIT_COXM_D_IN)
#define HWIO_MSS_CC_BIT_COXM_D_NOT_2D_BMSK                                                         0xffff
#define HWIO_MSS_CC_BIT_COXM_D_NOT_2D_SHFT                                                            0x0

#define HWIO_MSS_CC_BIT_COXM_CBCR_ADDR                                                         (MSS_CC_MSS_CC_REG_REG_BASE      + 0x00000794)
#define HWIO_MSS_CC_BIT_COXM_CBCR_RMSK                                                         0x80000005
#define HWIO_MSS_CC_BIT_COXM_CBCR_IN          \
        in_dword(HWIO_MSS_CC_BIT_COXM_CBCR_ADDR)
#define HWIO_MSS_CC_BIT_COXM_CBCR_INM(m)      \
        in_dword_masked(HWIO_MSS_CC_BIT_COXM_CBCR_ADDR, m)
#define HWIO_MSS_CC_BIT_COXM_CBCR_OUT(v)      \
        out_dword(HWIO_MSS_CC_BIT_COXM_CBCR_ADDR,v)
#define HWIO_MSS_CC_BIT_COXM_CBCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_CC_BIT_COXM_CBCR_ADDR,m,v,HWIO_MSS_CC_BIT_COXM_CBCR_IN)
#define HWIO_MSS_CC_BIT_COXM_CBCR_CLK_OFF_BMSK                                                 0x80000000
#define HWIO_MSS_CC_BIT_COXM_CBCR_CLK_OFF_SHFT                                                       0x1f
#define HWIO_MSS_CC_BIT_COXM_CBCR_CLK_ARES_BMSK                                                       0x4
#define HWIO_MSS_CC_BIT_COXM_CBCR_CLK_ARES_SHFT                                                       0x2
#define HWIO_MSS_CC_BIT_COXM_CBCR_CLK_ENABLE_BMSK                                                     0x1
#define HWIO_MSS_CC_BIT_COXM_CBCR_CLK_ENABLE_SHFT                                                     0x0

#define HWIO_MSS_CC_MSS_CLOCK_SPDM_MON_ADDR                                                    (MSS_CC_MSS_CC_REG_REG_BASE      + 0x00001388)
#define HWIO_MSS_CC_MSS_CLOCK_SPDM_MON_RMSK                                                           0x3
#define HWIO_MSS_CC_MSS_CLOCK_SPDM_MON_IN          \
        in_dword(HWIO_MSS_CC_MSS_CLOCK_SPDM_MON_ADDR)
#define HWIO_MSS_CC_MSS_CLOCK_SPDM_MON_INM(m)      \
        in_dword_masked(HWIO_MSS_CC_MSS_CLOCK_SPDM_MON_ADDR, m)
#define HWIO_MSS_CC_MSS_CLOCK_SPDM_MON_OUT(v)      \
        out_dword(HWIO_MSS_CC_MSS_CLOCK_SPDM_MON_ADDR,v)
#define HWIO_MSS_CC_MSS_CLOCK_SPDM_MON_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_CC_MSS_CLOCK_SPDM_MON_ADDR,m,v,HWIO_MSS_CC_MSS_CLOCK_SPDM_MON_IN)
#define HWIO_MSS_CC_MSS_CLOCK_SPDM_MON_Q6_MON_CLKEN_BMSK                                              0x2
#define HWIO_MSS_CC_MSS_CLOCK_SPDM_MON_Q6_MON_CLKEN_SHFT                                              0x1
#define HWIO_MSS_CC_MSS_CLOCK_SPDM_MON_BUS_MON_CLKEN_BMSK                                             0x1
#define HWIO_MSS_CC_MSS_CLOCK_SPDM_MON_BUS_MON_CLKEN_SHFT                                             0x0

#define HWIO_MSS_CC_MSS_BBRX0_MUX_SEL_ADDR                                                     (MSS_CC_MSS_CC_REG_REG_BASE      + 0x0000138c)
#define HWIO_MSS_CC_MSS_BBRX0_MUX_SEL_RMSK                                                            0x3
#define HWIO_MSS_CC_MSS_BBRX0_MUX_SEL_IN          \
        in_dword(HWIO_MSS_CC_MSS_BBRX0_MUX_SEL_ADDR)
#define HWIO_MSS_CC_MSS_BBRX0_MUX_SEL_INM(m)      \
        in_dword_masked(HWIO_MSS_CC_MSS_BBRX0_MUX_SEL_ADDR, m)
#define HWIO_MSS_CC_MSS_BBRX0_MUX_SEL_OUT(v)      \
        out_dword(HWIO_MSS_CC_MSS_BBRX0_MUX_SEL_ADDR,v)
#define HWIO_MSS_CC_MSS_BBRX0_MUX_SEL_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_CC_MSS_BBRX0_MUX_SEL_ADDR,m,v,HWIO_MSS_CC_MSS_BBRX0_MUX_SEL_IN)
#define HWIO_MSS_CC_MSS_BBRX0_MUX_SEL_SECOND_MUX_SEL_BMSK                                             0x2
#define HWIO_MSS_CC_MSS_BBRX0_MUX_SEL_SECOND_MUX_SEL_SHFT                                             0x1
#define HWIO_MSS_CC_MSS_BBRX0_MUX_SEL_FIRST_MUX_SEL_BMSK                                              0x1
#define HWIO_MSS_CC_MSS_BBRX0_MUX_SEL_FIRST_MUX_SEL_SHFT                                              0x0

#define HWIO_MSS_CC_MSS_BBRX1_MUX_SEL_ADDR                                                     (MSS_CC_MSS_CC_REG_REG_BASE      + 0x00001390)
#define HWIO_MSS_CC_MSS_BBRX1_MUX_SEL_RMSK                                                            0x3
#define HWIO_MSS_CC_MSS_BBRX1_MUX_SEL_IN          \
        in_dword(HWIO_MSS_CC_MSS_BBRX1_MUX_SEL_ADDR)
#define HWIO_MSS_CC_MSS_BBRX1_MUX_SEL_INM(m)      \
        in_dword_masked(HWIO_MSS_CC_MSS_BBRX1_MUX_SEL_ADDR, m)
#define HWIO_MSS_CC_MSS_BBRX1_MUX_SEL_OUT(v)      \
        out_dword(HWIO_MSS_CC_MSS_BBRX1_MUX_SEL_ADDR,v)
#define HWIO_MSS_CC_MSS_BBRX1_MUX_SEL_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_CC_MSS_BBRX1_MUX_SEL_ADDR,m,v,HWIO_MSS_CC_MSS_BBRX1_MUX_SEL_IN)
#define HWIO_MSS_CC_MSS_BBRX1_MUX_SEL_SECOND_MUX_SEL_BMSK                                             0x2
#define HWIO_MSS_CC_MSS_BBRX1_MUX_SEL_SECOND_MUX_SEL_SHFT                                             0x1
#define HWIO_MSS_CC_MSS_BBRX1_MUX_SEL_FIRST_MUX_SEL_BMSK                                              0x1
#define HWIO_MSS_CC_MSS_BBRX1_MUX_SEL_FIRST_MUX_SEL_SHFT                                              0x0

#define HWIO_MSS_CC_MSS_BBRX2_MUX_SEL_ADDR                                                     (MSS_CC_MSS_CC_REG_REG_BASE      + 0x00001394)
#define HWIO_MSS_CC_MSS_BBRX2_MUX_SEL_RMSK                                                            0x3
#define HWIO_MSS_CC_MSS_BBRX2_MUX_SEL_IN          \
        in_dword(HWIO_MSS_CC_MSS_BBRX2_MUX_SEL_ADDR)
#define HWIO_MSS_CC_MSS_BBRX2_MUX_SEL_INM(m)      \
        in_dword_masked(HWIO_MSS_CC_MSS_BBRX2_MUX_SEL_ADDR, m)
#define HWIO_MSS_CC_MSS_BBRX2_MUX_SEL_OUT(v)      \
        out_dword(HWIO_MSS_CC_MSS_BBRX2_MUX_SEL_ADDR,v)
#define HWIO_MSS_CC_MSS_BBRX2_MUX_SEL_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_CC_MSS_BBRX2_MUX_SEL_ADDR,m,v,HWIO_MSS_CC_MSS_BBRX2_MUX_SEL_IN)
#define HWIO_MSS_CC_MSS_BBRX2_MUX_SEL_SECOND_MUX_SEL_BMSK                                             0x2
#define HWIO_MSS_CC_MSS_BBRX2_MUX_SEL_SECOND_MUX_SEL_SHFT                                             0x1
#define HWIO_MSS_CC_MSS_BBRX2_MUX_SEL_FIRST_MUX_SEL_BMSK                                              0x1
#define HWIO_MSS_CC_MSS_BBRX2_MUX_SEL_FIRST_MUX_SEL_SHFT                                              0x0

#define HWIO_MSS_CC_MSS_BBRX3_MUX_SEL_ADDR                                                     (MSS_CC_MSS_CC_REG_REG_BASE      + 0x00001398)
#define HWIO_MSS_CC_MSS_BBRX3_MUX_SEL_RMSK                                                            0x3
#define HWIO_MSS_CC_MSS_BBRX3_MUX_SEL_IN          \
        in_dword(HWIO_MSS_CC_MSS_BBRX3_MUX_SEL_ADDR)
#define HWIO_MSS_CC_MSS_BBRX3_MUX_SEL_INM(m)      \
        in_dword_masked(HWIO_MSS_CC_MSS_BBRX3_MUX_SEL_ADDR, m)
#define HWIO_MSS_CC_MSS_BBRX3_MUX_SEL_OUT(v)      \
        out_dword(HWIO_MSS_CC_MSS_BBRX3_MUX_SEL_ADDR,v)
#define HWIO_MSS_CC_MSS_BBRX3_MUX_SEL_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_CC_MSS_BBRX3_MUX_SEL_ADDR,m,v,HWIO_MSS_CC_MSS_BBRX3_MUX_SEL_IN)
#define HWIO_MSS_CC_MSS_BBRX3_MUX_SEL_SECOND_MUX_SEL_BMSK                                             0x2
#define HWIO_MSS_CC_MSS_BBRX3_MUX_SEL_SECOND_MUX_SEL_SHFT                                             0x1
#define HWIO_MSS_CC_MSS_BBRX3_MUX_SEL_FIRST_MUX_SEL_BMSK                                              0x1
#define HWIO_MSS_CC_MSS_BBRX3_MUX_SEL_FIRST_MUX_SEL_SHFT                                              0x0

#define HWIO_MSS_CC_MSS_BBRX_EXT_CLOCK_MUX_SEL_ADDR                                            (MSS_CC_MSS_CC_REG_REG_BASE      + 0x000013a4)
#define HWIO_MSS_CC_MSS_BBRX_EXT_CLOCK_MUX_SEL_RMSK                                                   0x1
#define HWIO_MSS_CC_MSS_BBRX_EXT_CLOCK_MUX_SEL_IN          \
        in_dword(HWIO_MSS_CC_MSS_BBRX_EXT_CLOCK_MUX_SEL_ADDR)
#define HWIO_MSS_CC_MSS_BBRX_EXT_CLOCK_MUX_SEL_INM(m)      \
        in_dword_masked(HWIO_MSS_CC_MSS_BBRX_EXT_CLOCK_MUX_SEL_ADDR, m)
#define HWIO_MSS_CC_MSS_BBRX_EXT_CLOCK_MUX_SEL_OUT(v)      \
        out_dword(HWIO_MSS_CC_MSS_BBRX_EXT_CLOCK_MUX_SEL_ADDR,v)
#define HWIO_MSS_CC_MSS_BBRX_EXT_CLOCK_MUX_SEL_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_CC_MSS_BBRX_EXT_CLOCK_MUX_SEL_ADDR,m,v,HWIO_MSS_CC_MSS_BBRX_EXT_CLOCK_MUX_SEL_IN)
#define HWIO_MSS_CC_MSS_BBRX_EXT_CLOCK_MUX_SEL_PLLTEST_BMSK                                           0x1
#define HWIO_MSS_CC_MSS_BBRX_EXT_CLOCK_MUX_SEL_PLLTEST_SHFT                                           0x0

#define HWIO_MSS_CC_MSS_MPLL1_MODE_ADDR                                                        (MSS_CC_MSS_CC_REG_REG_BASE      + 0x000013a8)
#define HWIO_MSS_CC_MSS_MPLL1_MODE_RMSK                                                          0x3fffff
#define HWIO_MSS_CC_MSS_MPLL1_MODE_IN          \
        in_dword(HWIO_MSS_CC_MSS_MPLL1_MODE_ADDR)
#define HWIO_MSS_CC_MSS_MPLL1_MODE_INM(m)      \
        in_dword_masked(HWIO_MSS_CC_MSS_MPLL1_MODE_ADDR, m)
#define HWIO_MSS_CC_MSS_MPLL1_MODE_OUT(v)      \
        out_dword(HWIO_MSS_CC_MSS_MPLL1_MODE_ADDR,v)
#define HWIO_MSS_CC_MSS_MPLL1_MODE_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_CC_MSS_MPLL1_MODE_ADDR,m,v,HWIO_MSS_CC_MSS_MPLL1_MODE_IN)
#define HWIO_MSS_CC_MSS_MPLL1_MODE_RESERVE_21_4_BMSK                                             0x3ffff0
#define HWIO_MSS_CC_MSS_MPLL1_MODE_RESERVE_21_4_SHFT                                                  0x4
#define HWIO_MSS_CC_MSS_MPLL1_MODE_PLL_PLLTEST_BMSK                                                   0x8
#define HWIO_MSS_CC_MSS_MPLL1_MODE_PLL_PLLTEST_SHFT                                                   0x3
#define HWIO_MSS_CC_MSS_MPLL1_MODE_PLL_RESET_N_BMSK                                                   0x4
#define HWIO_MSS_CC_MSS_MPLL1_MODE_PLL_RESET_N_SHFT                                                   0x2
#define HWIO_MSS_CC_MSS_MPLL1_MODE_PLL_BYPASSNL_BMSK                                                  0x2
#define HWIO_MSS_CC_MSS_MPLL1_MODE_PLL_BYPASSNL_SHFT                                                  0x1
#define HWIO_MSS_CC_MSS_MPLL1_MODE_PLL_OUTCTRL_BMSK                                                   0x1
#define HWIO_MSS_CC_MSS_MPLL1_MODE_PLL_OUTCTRL_SHFT                                                   0x0

#define HWIO_MSS_CC_MSS_MPLL1_L_VAL_ADDR                                                       (MSS_CC_MSS_CC_REG_REG_BASE      + 0x000013ac)
#define HWIO_MSS_CC_MSS_MPLL1_L_VAL_RMSK                                                             0x7f
#define HWIO_MSS_CC_MSS_MPLL1_L_VAL_IN          \
        in_dword(HWIO_MSS_CC_MSS_MPLL1_L_VAL_ADDR)
#define HWIO_MSS_CC_MSS_MPLL1_L_VAL_INM(m)      \
        in_dword_masked(HWIO_MSS_CC_MSS_MPLL1_L_VAL_ADDR, m)
#define HWIO_MSS_CC_MSS_MPLL1_L_VAL_OUT(v)      \
        out_dword(HWIO_MSS_CC_MSS_MPLL1_L_VAL_ADDR,v)
#define HWIO_MSS_CC_MSS_MPLL1_L_VAL_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_CC_MSS_MPLL1_L_VAL_ADDR,m,v,HWIO_MSS_CC_MSS_MPLL1_L_VAL_IN)
#define HWIO_MSS_CC_MSS_MPLL1_L_VAL_PLL_L_BMSK                                                       0x7f
#define HWIO_MSS_CC_MSS_MPLL1_L_VAL_PLL_L_SHFT                                                        0x0

#define HWIO_MSS_CC_MSS_MPLL1_ALPHA_VAL_LOWER_ADDR                                             (MSS_CC_MSS_CC_REG_REG_BASE      + 0x000013b0)
#define HWIO_MSS_CC_MSS_MPLL1_ALPHA_VAL_LOWER_RMSK                                             0xffffffff
#define HWIO_MSS_CC_MSS_MPLL1_ALPHA_VAL_LOWER_IN          \
        in_dword(HWIO_MSS_CC_MSS_MPLL1_ALPHA_VAL_LOWER_ADDR)
#define HWIO_MSS_CC_MSS_MPLL1_ALPHA_VAL_LOWER_INM(m)      \
        in_dword_masked(HWIO_MSS_CC_MSS_MPLL1_ALPHA_VAL_LOWER_ADDR, m)
#define HWIO_MSS_CC_MSS_MPLL1_ALPHA_VAL_LOWER_OUT(v)      \
        out_dword(HWIO_MSS_CC_MSS_MPLL1_ALPHA_VAL_LOWER_ADDR,v)
#define HWIO_MSS_CC_MSS_MPLL1_ALPHA_VAL_LOWER_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_CC_MSS_MPLL1_ALPHA_VAL_LOWER_ADDR,m,v,HWIO_MSS_CC_MSS_MPLL1_ALPHA_VAL_LOWER_IN)
#define HWIO_MSS_CC_MSS_MPLL1_ALPHA_VAL_LOWER_PLL_ALPHA_LOWER_BMSK                             0xffffffff
#define HWIO_MSS_CC_MSS_MPLL1_ALPHA_VAL_LOWER_PLL_ALPHA_LOWER_SHFT                                    0x0

#define HWIO_MSS_CC_MSS_MPLL1_ALPHA_VAL_UPPER_ADDR                                             (MSS_CC_MSS_CC_REG_REG_BASE      + 0x000013b4)
#define HWIO_MSS_CC_MSS_MPLL1_ALPHA_VAL_UPPER_RMSK                                                   0xff
#define HWIO_MSS_CC_MSS_MPLL1_ALPHA_VAL_UPPER_IN          \
        in_dword(HWIO_MSS_CC_MSS_MPLL1_ALPHA_VAL_UPPER_ADDR)
#define HWIO_MSS_CC_MSS_MPLL1_ALPHA_VAL_UPPER_INM(m)      \
        in_dword_masked(HWIO_MSS_CC_MSS_MPLL1_ALPHA_VAL_UPPER_ADDR, m)
#define HWIO_MSS_CC_MSS_MPLL1_ALPHA_VAL_UPPER_OUT(v)      \
        out_dword(HWIO_MSS_CC_MSS_MPLL1_ALPHA_VAL_UPPER_ADDR,v)
#define HWIO_MSS_CC_MSS_MPLL1_ALPHA_VAL_UPPER_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_CC_MSS_MPLL1_ALPHA_VAL_UPPER_ADDR,m,v,HWIO_MSS_CC_MSS_MPLL1_ALPHA_VAL_UPPER_IN)
#define HWIO_MSS_CC_MSS_MPLL1_ALPHA_VAL_UPPER_PLL_ALPHA_UPPER_BMSK                                   0xff
#define HWIO_MSS_CC_MSS_MPLL1_ALPHA_VAL_UPPER_PLL_ALPHA_UPPER_SHFT                                    0x0

#define HWIO_MSS_CC_MSS_MPLL1_USER_CTL_ADDR                                                    (MSS_CC_MSS_CC_REG_REG_BASE      + 0x000013b8)
#define HWIO_MSS_CC_MSS_MPLL1_USER_CTL_RMSK                                                    0xffffffff
#define HWIO_MSS_CC_MSS_MPLL1_USER_CTL_IN          \
        in_dword(HWIO_MSS_CC_MSS_MPLL1_USER_CTL_ADDR)
#define HWIO_MSS_CC_MSS_MPLL1_USER_CTL_INM(m)      \
        in_dword_masked(HWIO_MSS_CC_MSS_MPLL1_USER_CTL_ADDR, m)
#define HWIO_MSS_CC_MSS_MPLL1_USER_CTL_OUT(v)      \
        out_dword(HWIO_MSS_CC_MSS_MPLL1_USER_CTL_ADDR,v)
#define HWIO_MSS_CC_MSS_MPLL1_USER_CTL_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_CC_MSS_MPLL1_USER_CTL_ADDR,m,v,HWIO_MSS_CC_MSS_MPLL1_USER_CTL_IN)
#define HWIO_MSS_CC_MSS_MPLL1_USER_CTL_RESERVE_BITS_31_30_BMSK                                 0xc0000000
#define HWIO_MSS_CC_MSS_MPLL1_USER_CTL_RESERVE_BITS_31_30_SHFT                                       0x1e
#define HWIO_MSS_CC_MSS_MPLL1_USER_CTL_RESERVE_BITS_29_28_BMSK                                 0x30000000
#define HWIO_MSS_CC_MSS_MPLL1_USER_CTL_RESERVE_BITS_29_28_SHFT                                       0x1c
#define HWIO_MSS_CC_MSS_MPLL1_USER_CTL_RESERVE_BITS_27_25_BMSK                                  0xe000000
#define HWIO_MSS_CC_MSS_MPLL1_USER_CTL_RESERVE_BITS_27_25_SHFT                                       0x19
#define HWIO_MSS_CC_MSS_MPLL1_USER_CTL_MN_EN_BMSK                                               0x1000000
#define HWIO_MSS_CC_MSS_MPLL1_USER_CTL_MN_EN_SHFT                                                    0x18
#define HWIO_MSS_CC_MSS_MPLL1_USER_CTL_RESERVE_BITS_23_14_BMSK                                   0xffc000
#define HWIO_MSS_CC_MSS_MPLL1_USER_CTL_RESERVE_BITS_23_14_SHFT                                        0xe
#define HWIO_MSS_CC_MSS_MPLL1_USER_CTL_PREDIV_CTL_BMSK                                             0x3000
#define HWIO_MSS_CC_MSS_MPLL1_USER_CTL_PREDIV_CTL_SHFT                                                0xc
#define HWIO_MSS_CC_MSS_MPLL1_USER_CTL_RESERVE_BITS_11_10_BMSK                                      0xc00
#define HWIO_MSS_CC_MSS_MPLL1_USER_CTL_RESERVE_BITS_11_10_SHFT                                        0xa
#define HWIO_MSS_CC_MSS_MPLL1_USER_CTL_POSTDIV_CTL_BMSK                                             0x300
#define HWIO_MSS_CC_MSS_MPLL1_USER_CTL_POSTDIV_CTL_SHFT                                               0x8
#define HWIO_MSS_CC_MSS_MPLL1_USER_CTL_INV_OUTPUT_BMSK                                               0x80
#define HWIO_MSS_CC_MSS_MPLL1_USER_CTL_INV_OUTPUT_SHFT                                                0x7
#define HWIO_MSS_CC_MSS_MPLL1_USER_CTL_RESERVE_BIT_6_5_BMSK                                          0x60
#define HWIO_MSS_CC_MSS_MPLL1_USER_CTL_RESERVE_BIT_6_5_SHFT                                           0x5
#define HWIO_MSS_CC_MSS_MPLL1_USER_CTL_LVTEST_EN_BMSK                                                0x10
#define HWIO_MSS_CC_MSS_MPLL1_USER_CTL_LVTEST_EN_SHFT                                                 0x4
#define HWIO_MSS_CC_MSS_MPLL1_USER_CTL_LVEARLY_EN_BMSK                                                0x8
#define HWIO_MSS_CC_MSS_MPLL1_USER_CTL_LVEARLY_EN_SHFT                                                0x3
#define HWIO_MSS_CC_MSS_MPLL1_USER_CTL_LVBIST_EN_BMSK                                                 0x4
#define HWIO_MSS_CC_MSS_MPLL1_USER_CTL_LVBIST_EN_SHFT                                                 0x2
#define HWIO_MSS_CC_MSS_MPLL1_USER_CTL_LVAUX_EN_BMSK                                                  0x2
#define HWIO_MSS_CC_MSS_MPLL1_USER_CTL_LVAUX_EN_SHFT                                                  0x1
#define HWIO_MSS_CC_MSS_MPLL1_USER_CTL_LVMAIN_EN_BMSK                                                 0x1
#define HWIO_MSS_CC_MSS_MPLL1_USER_CTL_LVMAIN_EN_SHFT                                                 0x0

#define HWIO_MSS_CC_MSS_MPLL1_CONFIG_CTL_ADDR                                                  (MSS_CC_MSS_CC_REG_REG_BASE      + 0x000013bc)
#define HWIO_MSS_CC_MSS_MPLL1_CONFIG_CTL_RMSK                                                  0xffffffff
#define HWIO_MSS_CC_MSS_MPLL1_CONFIG_CTL_IN          \
        in_dword(HWIO_MSS_CC_MSS_MPLL1_CONFIG_CTL_ADDR)
#define HWIO_MSS_CC_MSS_MPLL1_CONFIG_CTL_INM(m)      \
        in_dword_masked(HWIO_MSS_CC_MSS_MPLL1_CONFIG_CTL_ADDR, m)
#define HWIO_MSS_CC_MSS_MPLL1_CONFIG_CTL_OUT(v)      \
        out_dword(HWIO_MSS_CC_MSS_MPLL1_CONFIG_CTL_ADDR,v)
#define HWIO_MSS_CC_MSS_MPLL1_CONFIG_CTL_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_CC_MSS_MPLL1_CONFIG_CTL_ADDR,m,v,HWIO_MSS_CC_MSS_MPLL1_CONFIG_CTL_IN)
#define HWIO_MSS_CC_MSS_MPLL1_CONFIG_CTL_PLLBW_CNT_BMSK                                        0xff000000
#define HWIO_MSS_CC_MSS_MPLL1_CONFIG_CTL_PLLBW_CNT_SHFT                                              0x18
#define HWIO_MSS_CC_MSS_MPLL1_CONFIG_CTL_RESERVE_23_BMSK                                         0x800000
#define HWIO_MSS_CC_MSS_MPLL1_CONFIG_CTL_RESERVE_23_SHFT                                             0x17
#define HWIO_MSS_CC_MSS_MPLL1_CONFIG_CTL_FILT_RESAMPLE_EN_BMSK                                   0x400000
#define HWIO_MSS_CC_MSS_MPLL1_CONFIG_CTL_FILT_RESAMPLE_EN_SHFT                                       0x16
#define HWIO_MSS_CC_MSS_MPLL1_CONFIG_CTL_CHGPUMP_OFFSET_EN_BMSK                                  0x200000
#define HWIO_MSS_CC_MSS_MPLL1_CONFIG_CTL_CHGPUMP_OFFSET_EN_SHFT                                      0x15
#define HWIO_MSS_CC_MSS_MPLL1_CONFIG_CTL_PLLBW_EN_BMSK                                           0x100000
#define HWIO_MSS_CC_MSS_MPLL1_CONFIG_CTL_PLLBW_EN_SHFT                                               0x14
#define HWIO_MSS_CC_MSS_MPLL1_CONFIG_CTL_PLLBW_UPDATE_BMSK                                        0x80000
#define HWIO_MSS_CC_MSS_MPLL1_CONFIG_CTL_PLLBW_UPDATE_SHFT                                           0x13
#define HWIO_MSS_CC_MSS_MPLL1_CONFIG_CTL_FORCE_ISEED_BMSK                                         0x40000
#define HWIO_MSS_CC_MSS_MPLL1_CONFIG_CTL_FORCE_ISEED_SHFT                                            0x12
#define HWIO_MSS_CC_MSS_MPLL1_CONFIG_CTL_DITHER_SEL_BMSK                                          0x30000
#define HWIO_MSS_CC_MSS_MPLL1_CONFIG_CTL_DITHER_SEL_SHFT                                             0x10
#define HWIO_MSS_CC_MSS_MPLL1_CONFIG_CTL_CORR_EN_BMSK                                              0x8000
#define HWIO_MSS_CC_MSS_MPLL1_CONFIG_CTL_CORR_EN_SHFT                                                 0xf
#define HWIO_MSS_CC_MSS_MPLL1_CONFIG_CTL_RVSIG_DEL_BMSK                                            0x6000
#define HWIO_MSS_CC_MSS_MPLL1_CONFIG_CTL_RVSIG_DEL_SHFT                                               0xd
#define HWIO_MSS_CC_MSS_MPLL1_CONFIG_CTL_PFD_DZSEL_BMSK                                            0x1800
#define HWIO_MSS_CC_MSS_MPLL1_CONFIG_CTL_PFD_DZSEL_SHFT                                               0xb
#define HWIO_MSS_CC_MSS_MPLL1_CONFIG_CTL_PLLBW_ICPC_1_0_PLLBW_ICPF_2_0_BMSK                         0x7c0
#define HWIO_MSS_CC_MSS_MPLL1_CONFIG_CTL_PLLBW_ICPC_1_0_PLLBW_ICPF_2_0_SHFT                           0x6
#define HWIO_MSS_CC_MSS_MPLL1_CONFIG_CTL_FILT_BS_CNTL_BMSK                                           0x30
#define HWIO_MSS_CC_MSS_MPLL1_CONFIG_CTL_FILT_BS_CNTL_SHFT                                            0x4
#define HWIO_MSS_CC_MSS_MPLL1_CONFIG_CTL_SEL_IREG_OSC_BMSK                                            0xc
#define HWIO_MSS_CC_MSS_MPLL1_CONFIG_CTL_SEL_IREG_OSC_SHFT                                            0x2
#define HWIO_MSS_CC_MSS_MPLL1_CONFIG_CTL_LCKDET_CNFG_BMSK                                             0x3
#define HWIO_MSS_CC_MSS_MPLL1_CONFIG_CTL_LCKDET_CNFG_SHFT                                             0x0

#define HWIO_MSS_CC_MSS_MPLL1_TEST_CTL_ADDR                                                    (MSS_CC_MSS_CC_REG_REG_BASE      + 0x000013c0)
#define HWIO_MSS_CC_MSS_MPLL1_TEST_CTL_RMSK                                                    0xffffffff
#define HWIO_MSS_CC_MSS_MPLL1_TEST_CTL_IN          \
        in_dword(HWIO_MSS_CC_MSS_MPLL1_TEST_CTL_ADDR)
#define HWIO_MSS_CC_MSS_MPLL1_TEST_CTL_INM(m)      \
        in_dword_masked(HWIO_MSS_CC_MSS_MPLL1_TEST_CTL_ADDR, m)
#define HWIO_MSS_CC_MSS_MPLL1_TEST_CTL_OUT(v)      \
        out_dword(HWIO_MSS_CC_MSS_MPLL1_TEST_CTL_ADDR,v)
#define HWIO_MSS_CC_MSS_MPLL1_TEST_CTL_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_CC_MSS_MPLL1_TEST_CTL_ADDR,m,v,HWIO_MSS_CC_MSS_MPLL1_TEST_CTL_IN)
#define HWIO_MSS_CC_MSS_MPLL1_TEST_CTL_RESERVE_31_30_BMSK                                      0xc0000000
#define HWIO_MSS_CC_MSS_MPLL1_TEST_CTL_RESERVE_31_30_SHFT                                            0x1e
#define HWIO_MSS_CC_MSS_MPLL1_TEST_CTL_PLLBW_DELTAL_BMSK                                       0x38000000
#define HWIO_MSS_CC_MSS_MPLL1_TEST_CTL_PLLBW_DELTAL_SHFT                                             0x1b
#define HWIO_MSS_CC_MSS_MPLL1_TEST_CTL_PLLBW_ERR_BMSK                                           0x7800000
#define HWIO_MSS_CC_MSS_MPLL1_TEST_CTL_PLLBW_ERR_SHFT                                                0x17
#define HWIO_MSS_CC_MSS_MPLL1_TEST_CTL_EN_BMSK                                                   0x700000
#define HWIO_MSS_CC_MSS_MPLL1_TEST_CTL_EN_SHFT                                                       0x14
#define HWIO_MSS_CC_MSS_MPLL1_TEST_CTL_PUP_BMSK                                                   0x80000
#define HWIO_MSS_CC_MSS_MPLL1_TEST_CTL_PUP_SHFT                                                      0x13
#define HWIO_MSS_CC_MSS_MPLL1_TEST_CTL_CFG_BMSK                                                   0x60000
#define HWIO_MSS_CC_MSS_MPLL1_TEST_CTL_CFG_SHFT                                                      0x11
#define HWIO_MSS_CC_MSS_MPLL1_TEST_CTL_TEST_OUT_SEL_BMSK                                          0x10000
#define HWIO_MSS_CC_MSS_MPLL1_TEST_CTL_TEST_OUT_SEL_SHFT                                             0x10
#define HWIO_MSS_CC_MSS_MPLL1_TEST_CTL_PLLBW_WAIT_BMSK                                             0xc000
#define HWIO_MSS_CC_MSS_MPLL1_TEST_CTL_PLLBW_WAIT_SHFT                                                0xe
#define HWIO_MSS_CC_MSS_MPLL1_TEST_CTL_CNT_MSB_SEL_BMSK                                            0x2000
#define HWIO_MSS_CC_MSS_MPLL1_TEST_CTL_CNT_MSB_SEL_SHFT                                               0xd
#define HWIO_MSS_CC_MSS_MPLL1_TEST_CTL_DCC_EN_BMSK                                                 0x1000
#define HWIO_MSS_CC_MSS_MPLL1_TEST_CTL_DCC_EN_SHFT                                                    0xc
#define HWIO_MSS_CC_MSS_MPLL1_TEST_CTL_RESERVE_11_BMSK                                              0x800
#define HWIO_MSS_CC_MSS_MPLL1_TEST_CTL_RESERVE_11_SHFT                                                0xb
#define HWIO_MSS_CC_MSS_MPLL1_TEST_CTL_PRBS_EN_BMSK                                                 0x400
#define HWIO_MSS_CC_MSS_MPLL1_TEST_CTL_PRBS_EN_SHFT                                                   0xa
#define HWIO_MSS_CC_MSS_MPLL1_TEST_CTL_SEL_IEXT_BMSK                                                0x200
#define HWIO_MSS_CC_MSS_MPLL1_TEST_CTL_SEL_IEXT_SHFT                                                  0x9
#define HWIO_MSS_CC_MSS_MPLL1_TEST_CTL_DTEST_SEL_BMSK                                               0x180
#define HWIO_MSS_CC_MSS_MPLL1_TEST_CTL_DTEST_SEL_SHFT                                                 0x7
#define HWIO_MSS_CC_MSS_MPLL1_TEST_CTL_BYP_TESTAMP_BMSK                                              0x40
#define HWIO_MSS_CC_MSS_MPLL1_TEST_CTL_BYP_TESTAMP_SHFT                                               0x6
#define HWIO_MSS_CC_MSS_MPLL1_TEST_CTL_ATEST1_SEL_BMSK                                               0x30
#define HWIO_MSS_CC_MSS_MPLL1_TEST_CTL_ATEST1_SEL_SHFT                                                0x4
#define HWIO_MSS_CC_MSS_MPLL1_TEST_CTL_ATEST0_SEL_BMSK                                                0xc
#define HWIO_MSS_CC_MSS_MPLL1_TEST_CTL_ATEST0_SEL_SHFT                                                0x2
#define HWIO_MSS_CC_MSS_MPLL1_TEST_CTL_ATEST1_EN_BMSK                                                 0x2
#define HWIO_MSS_CC_MSS_MPLL1_TEST_CTL_ATEST1_EN_SHFT                                                 0x1
#define HWIO_MSS_CC_MSS_MPLL1_TEST_CTL_ATEST0_EN_BMSK                                                 0x1
#define HWIO_MSS_CC_MSS_MPLL1_TEST_CTL_ATEST0_EN_SHFT                                                 0x0

#define HWIO_MSS_CC_MSS_MPLL1_STATUS_ADDR                                                      (MSS_CC_MSS_CC_REG_REG_BASE      + 0x000013c4)
#define HWIO_MSS_CC_MSS_MPLL1_STATUS_RMSK                                                         0x3ffff
#define HWIO_MSS_CC_MSS_MPLL1_STATUS_IN          \
        in_dword(HWIO_MSS_CC_MSS_MPLL1_STATUS_ADDR)
#define HWIO_MSS_CC_MSS_MPLL1_STATUS_INM(m)      \
        in_dword_masked(HWIO_MSS_CC_MSS_MPLL1_STATUS_ADDR, m)
#define HWIO_MSS_CC_MSS_MPLL1_STATUS_PLL_ACTIVE_FLAG_BMSK                                         0x20000
#define HWIO_MSS_CC_MSS_MPLL1_STATUS_PLL_ACTIVE_FLAG_SHFT                                            0x11
#define HWIO_MSS_CC_MSS_MPLL1_STATUS_PLL_LOCK_DET_BMSK                                            0x10000
#define HWIO_MSS_CC_MSS_MPLL1_STATUS_PLL_LOCK_DET_SHFT                                               0x10
#define HWIO_MSS_CC_MSS_MPLL1_STATUS_PLL_D_BMSK                                                    0xffff
#define HWIO_MSS_CC_MSS_MPLL1_STATUS_PLL_D_SHFT                                                       0x0

#define HWIO_MSS_CC_MSS_MPLL1_EARLY_DIV5_CBCR_ADDR                                             (MSS_CC_MSS_CC_REG_REG_BASE      + 0x000013c8)
#define HWIO_MSS_CC_MSS_MPLL1_EARLY_DIV5_CBCR_RMSK                                             0x80000001
#define HWIO_MSS_CC_MSS_MPLL1_EARLY_DIV5_CBCR_IN          \
        in_dword(HWIO_MSS_CC_MSS_MPLL1_EARLY_DIV5_CBCR_ADDR)
#define HWIO_MSS_CC_MSS_MPLL1_EARLY_DIV5_CBCR_INM(m)      \
        in_dword_masked(HWIO_MSS_CC_MSS_MPLL1_EARLY_DIV5_CBCR_ADDR, m)
#define HWIO_MSS_CC_MSS_MPLL1_EARLY_DIV5_CBCR_OUT(v)      \
        out_dword(HWIO_MSS_CC_MSS_MPLL1_EARLY_DIV5_CBCR_ADDR,v)
#define HWIO_MSS_CC_MSS_MPLL1_EARLY_DIV5_CBCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_CC_MSS_MPLL1_EARLY_DIV5_CBCR_ADDR,m,v,HWIO_MSS_CC_MSS_MPLL1_EARLY_DIV5_CBCR_IN)
#define HWIO_MSS_CC_MSS_MPLL1_EARLY_DIV5_CBCR_CLKOFF_BMSK                                      0x80000000
#define HWIO_MSS_CC_MSS_MPLL1_EARLY_DIV5_CBCR_CLKOFF_SHFT                                            0x1f
#define HWIO_MSS_CC_MSS_MPLL1_EARLY_DIV5_CBCR_CLKEN_BMSK                                              0x1
#define HWIO_MSS_CC_MSS_MPLL1_EARLY_DIV5_CBCR_CLKEN_SHFT                                              0x0

#define HWIO_MSS_CC_MSS_MPLL1_EARLY_DIV3_CBCR_ADDR                                             (MSS_CC_MSS_CC_REG_REG_BASE      + 0x000013cc)
#define HWIO_MSS_CC_MSS_MPLL1_EARLY_DIV3_CBCR_RMSK                                             0x80000001
#define HWIO_MSS_CC_MSS_MPLL1_EARLY_DIV3_CBCR_IN          \
        in_dword(HWIO_MSS_CC_MSS_MPLL1_EARLY_DIV3_CBCR_ADDR)
#define HWIO_MSS_CC_MSS_MPLL1_EARLY_DIV3_CBCR_INM(m)      \
        in_dword_masked(HWIO_MSS_CC_MSS_MPLL1_EARLY_DIV3_CBCR_ADDR, m)
#define HWIO_MSS_CC_MSS_MPLL1_EARLY_DIV3_CBCR_OUT(v)      \
        out_dword(HWIO_MSS_CC_MSS_MPLL1_EARLY_DIV3_CBCR_ADDR,v)
#define HWIO_MSS_CC_MSS_MPLL1_EARLY_DIV3_CBCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_CC_MSS_MPLL1_EARLY_DIV3_CBCR_ADDR,m,v,HWIO_MSS_CC_MSS_MPLL1_EARLY_DIV3_CBCR_IN)
#define HWIO_MSS_CC_MSS_MPLL1_EARLY_DIV3_CBCR_CLKOFF_BMSK                                      0x80000000
#define HWIO_MSS_CC_MSS_MPLL1_EARLY_DIV3_CBCR_CLKOFF_SHFT                                            0x1f
#define HWIO_MSS_CC_MSS_MPLL1_EARLY_DIV3_CBCR_CLKEN_BMSK                                              0x1
#define HWIO_MSS_CC_MSS_MPLL1_EARLY_DIV3_CBCR_CLKEN_SHFT                                              0x0

#define HWIO_MSS_CC_MSS_MPLL1_OUT_EARLY_DIV3_CBCR_ADDR                                         (MSS_CC_MSS_CC_REG_REG_BASE      + 0x000013d0)
#define HWIO_MSS_CC_MSS_MPLL1_OUT_EARLY_DIV3_CBCR_RMSK                                         0x80000001
#define HWIO_MSS_CC_MSS_MPLL1_OUT_EARLY_DIV3_CBCR_IN          \
        in_dword(HWIO_MSS_CC_MSS_MPLL1_OUT_EARLY_DIV3_CBCR_ADDR)
#define HWIO_MSS_CC_MSS_MPLL1_OUT_EARLY_DIV3_CBCR_INM(m)      \
        in_dword_masked(HWIO_MSS_CC_MSS_MPLL1_OUT_EARLY_DIV3_CBCR_ADDR, m)
#define HWIO_MSS_CC_MSS_MPLL1_OUT_EARLY_DIV3_CBCR_OUT(v)      \
        out_dword(HWIO_MSS_CC_MSS_MPLL1_OUT_EARLY_DIV3_CBCR_ADDR,v)
#define HWIO_MSS_CC_MSS_MPLL1_OUT_EARLY_DIV3_CBCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_CC_MSS_MPLL1_OUT_EARLY_DIV3_CBCR_ADDR,m,v,HWIO_MSS_CC_MSS_MPLL1_OUT_EARLY_DIV3_CBCR_IN)
#define HWIO_MSS_CC_MSS_MPLL1_OUT_EARLY_DIV3_CBCR_CLKOFF_BMSK                                  0x80000000
#define HWIO_MSS_CC_MSS_MPLL1_OUT_EARLY_DIV3_CBCR_CLKOFF_SHFT                                        0x1f
#define HWIO_MSS_CC_MSS_MPLL1_OUT_EARLY_DIV3_CBCR_CLKEN_BMSK                                          0x1
#define HWIO_MSS_CC_MSS_MPLL1_OUT_EARLY_DIV3_CBCR_CLKEN_SHFT                                          0x0

#define HWIO_MSS_CC_MSS_MPLL1_OUT_EARLY_DIV5_CBCR_ADDR                                         (MSS_CC_MSS_CC_REG_REG_BASE      + 0x000013d4)
#define HWIO_MSS_CC_MSS_MPLL1_OUT_EARLY_DIV5_CBCR_RMSK                                         0x80000001
#define HWIO_MSS_CC_MSS_MPLL1_OUT_EARLY_DIV5_CBCR_IN          \
        in_dword(HWIO_MSS_CC_MSS_MPLL1_OUT_EARLY_DIV5_CBCR_ADDR)
#define HWIO_MSS_CC_MSS_MPLL1_OUT_EARLY_DIV5_CBCR_INM(m)      \
        in_dword_masked(HWIO_MSS_CC_MSS_MPLL1_OUT_EARLY_DIV5_CBCR_ADDR, m)
#define HWIO_MSS_CC_MSS_MPLL1_OUT_EARLY_DIV5_CBCR_OUT(v)      \
        out_dword(HWIO_MSS_CC_MSS_MPLL1_OUT_EARLY_DIV5_CBCR_ADDR,v)
#define HWIO_MSS_CC_MSS_MPLL1_OUT_EARLY_DIV5_CBCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_CC_MSS_MPLL1_OUT_EARLY_DIV5_CBCR_ADDR,m,v,HWIO_MSS_CC_MSS_MPLL1_OUT_EARLY_DIV5_CBCR_IN)
#define HWIO_MSS_CC_MSS_MPLL1_OUT_EARLY_DIV5_CBCR_CLKOFF_BMSK                                  0x80000000
#define HWIO_MSS_CC_MSS_MPLL1_OUT_EARLY_DIV5_CBCR_CLKOFF_SHFT                                        0x1f
#define HWIO_MSS_CC_MSS_MPLL1_OUT_EARLY_DIV5_CBCR_CLKEN_BMSK                                          0x1
#define HWIO_MSS_CC_MSS_MPLL1_OUT_EARLY_DIV5_CBCR_CLKEN_SHFT                                          0x0

#define HWIO_MSS_CC_MSS_BBRX0_CBCR_ADDR                                                        (MSS_CC_MSS_CC_REG_REG_BASE      + 0x000013d8)
#define HWIO_MSS_CC_MSS_BBRX0_CBCR_RMSK                                                        0x80000000
#define HWIO_MSS_CC_MSS_BBRX0_CBCR_IN          \
        in_dword(HWIO_MSS_CC_MSS_BBRX0_CBCR_ADDR)
#define HWIO_MSS_CC_MSS_BBRX0_CBCR_INM(m)      \
        in_dword_masked(HWIO_MSS_CC_MSS_BBRX0_CBCR_ADDR, m)
#define HWIO_MSS_CC_MSS_BBRX0_CBCR_CLKOFF_BMSK                                                 0x80000000
#define HWIO_MSS_CC_MSS_BBRX0_CBCR_CLKOFF_SHFT                                                       0x1f

#define HWIO_MSS_CC_MSS_BBRX1_CBCR_ADDR                                                        (MSS_CC_MSS_CC_REG_REG_BASE      + 0x000013dc)
#define HWIO_MSS_CC_MSS_BBRX1_CBCR_RMSK                                                        0x80000000
#define HWIO_MSS_CC_MSS_BBRX1_CBCR_IN          \
        in_dword(HWIO_MSS_CC_MSS_BBRX1_CBCR_ADDR)
#define HWIO_MSS_CC_MSS_BBRX1_CBCR_INM(m)      \
        in_dword_masked(HWIO_MSS_CC_MSS_BBRX1_CBCR_ADDR, m)
#define HWIO_MSS_CC_MSS_BBRX1_CBCR_CLKOFF_BMSK                                                 0x80000000
#define HWIO_MSS_CC_MSS_BBRX1_CBCR_CLKOFF_SHFT                                                       0x1f

#define HWIO_MSS_CC_MSS_BBRX2_CBCR_ADDR                                                        (MSS_CC_MSS_CC_REG_REG_BASE      + 0x000013e0)
#define HWIO_MSS_CC_MSS_BBRX2_CBCR_RMSK                                                        0x80000000
#define HWIO_MSS_CC_MSS_BBRX2_CBCR_IN          \
        in_dword(HWIO_MSS_CC_MSS_BBRX2_CBCR_ADDR)
#define HWIO_MSS_CC_MSS_BBRX2_CBCR_INM(m)      \
        in_dword_masked(HWIO_MSS_CC_MSS_BBRX2_CBCR_ADDR, m)
#define HWIO_MSS_CC_MSS_BBRX2_CBCR_CLKOFF_BMSK                                                 0x80000000
#define HWIO_MSS_CC_MSS_BBRX2_CBCR_CLKOFF_SHFT                                                       0x1f

#define HWIO_MSS_CC_MSS_BBRX3_CBCR_ADDR                                                        (MSS_CC_MSS_CC_REG_REG_BASE      + 0x000013e4)
#define HWIO_MSS_CC_MSS_BBRX3_CBCR_RMSK                                                        0x80000000
#define HWIO_MSS_CC_MSS_BBRX3_CBCR_IN          \
        in_dword(HWIO_MSS_CC_MSS_BBRX3_CBCR_ADDR)
#define HWIO_MSS_CC_MSS_BBRX3_CBCR_INM(m)      \
        in_dword_masked(HWIO_MSS_CC_MSS_BBRX3_CBCR_ADDR, m)
#define HWIO_MSS_CC_MSS_BBRX3_CBCR_CLKOFF_BMSK                                                 0x80000000
#define HWIO_MSS_CC_MSS_BBRX3_CBCR_CLKOFF_SHFT                                                       0x1f

#define HWIO_MSS_CC_MSS_RESERVE_02_ADDR                                                        (MSS_CC_MSS_CC_REG_REG_BASE      + 0x000013f0)
#define HWIO_MSS_CC_MSS_RESERVE_02_RMSK                                                        0xffffffff
#define HWIO_MSS_CC_MSS_RESERVE_02_IN          \
        in_dword(HWIO_MSS_CC_MSS_RESERVE_02_ADDR)
#define HWIO_MSS_CC_MSS_RESERVE_02_INM(m)      \
        in_dword_masked(HWIO_MSS_CC_MSS_RESERVE_02_ADDR, m)
#define HWIO_MSS_CC_MSS_RESERVE_02_OUT(v)      \
        out_dword(HWIO_MSS_CC_MSS_RESERVE_02_ADDR,v)
#define HWIO_MSS_CC_MSS_RESERVE_02_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_CC_MSS_RESERVE_02_ADDR,m,v,HWIO_MSS_CC_MSS_RESERVE_02_IN)
#define HWIO_MSS_CC_MSS_RESERVE_02_MSS_RESERVE_02_BMSK                                         0xfffffffe
#define HWIO_MSS_CC_MSS_RESERVE_02_MSS_RESERVE_02_SHFT                                                0x1
#define HWIO_MSS_CC_MSS_RESERVE_02_SWITCH_Q6_AXIS_CLK_BMSK                                            0x1
#define HWIO_MSS_CC_MSS_RESERVE_02_SWITCH_Q6_AXIS_CLK_SHFT                                            0x0

#define HWIO_MSS_CC_MSS_BBRX0_MISC_ADDR                                                        (MSS_CC_MSS_CC_REG_REG_BASE      + 0x000013f4)
#define HWIO_MSS_CC_MSS_BBRX0_MISC_RMSK                                                               0xf
#define HWIO_MSS_CC_MSS_BBRX0_MISC_IN          \
        in_dword(HWIO_MSS_CC_MSS_BBRX0_MISC_ADDR)
#define HWIO_MSS_CC_MSS_BBRX0_MISC_INM(m)      \
        in_dword_masked(HWIO_MSS_CC_MSS_BBRX0_MISC_ADDR, m)
#define HWIO_MSS_CC_MSS_BBRX0_MISC_OUT(v)      \
        out_dword(HWIO_MSS_CC_MSS_BBRX0_MISC_ADDR,v)
#define HWIO_MSS_CC_MSS_BBRX0_MISC_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_CC_MSS_BBRX0_MISC_ADDR,m,v,HWIO_MSS_CC_MSS_BBRX0_MISC_IN)
#define HWIO_MSS_CC_MSS_BBRX0_MISC_SRC_DIV_BMSK                                                       0xf
#define HWIO_MSS_CC_MSS_BBRX0_MISC_SRC_DIV_SHFT                                                       0x0

#define HWIO_MSS_CC_MSS_BBRX1_MISC_ADDR                                                        (MSS_CC_MSS_CC_REG_REG_BASE      + 0x000013f8)
#define HWIO_MSS_CC_MSS_BBRX1_MISC_RMSK                                                               0xf
#define HWIO_MSS_CC_MSS_BBRX1_MISC_IN          \
        in_dword(HWIO_MSS_CC_MSS_BBRX1_MISC_ADDR)
#define HWIO_MSS_CC_MSS_BBRX1_MISC_INM(m)      \
        in_dword_masked(HWIO_MSS_CC_MSS_BBRX1_MISC_ADDR, m)
#define HWIO_MSS_CC_MSS_BBRX1_MISC_OUT(v)      \
        out_dword(HWIO_MSS_CC_MSS_BBRX1_MISC_ADDR,v)
#define HWIO_MSS_CC_MSS_BBRX1_MISC_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_CC_MSS_BBRX1_MISC_ADDR,m,v,HWIO_MSS_CC_MSS_BBRX1_MISC_IN)
#define HWIO_MSS_CC_MSS_BBRX1_MISC_SRC_DIV_BMSK                                                       0xf
#define HWIO_MSS_CC_MSS_BBRX1_MISC_SRC_DIV_SHFT                                                       0x0

#define HWIO_MSS_CC_MSS_BBRX2_MISC_ADDR                                                        (MSS_CC_MSS_CC_REG_REG_BASE      + 0x000013fc)
#define HWIO_MSS_CC_MSS_BBRX2_MISC_RMSK                                                               0xf
#define HWIO_MSS_CC_MSS_BBRX2_MISC_IN          \
        in_dword(HWIO_MSS_CC_MSS_BBRX2_MISC_ADDR)
#define HWIO_MSS_CC_MSS_BBRX2_MISC_INM(m)      \
        in_dword_masked(HWIO_MSS_CC_MSS_BBRX2_MISC_ADDR, m)
#define HWIO_MSS_CC_MSS_BBRX2_MISC_OUT(v)      \
        out_dword(HWIO_MSS_CC_MSS_BBRX2_MISC_ADDR,v)
#define HWIO_MSS_CC_MSS_BBRX2_MISC_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_CC_MSS_BBRX2_MISC_ADDR,m,v,HWIO_MSS_CC_MSS_BBRX2_MISC_IN)
#define HWIO_MSS_CC_MSS_BBRX2_MISC_SRC_DIV_BMSK                                                       0xf
#define HWIO_MSS_CC_MSS_BBRX2_MISC_SRC_DIV_SHFT                                                       0x0

#define HWIO_MSS_CC_MSS_BBRX3_MISC_ADDR                                                        (MSS_CC_MSS_CC_REG_REG_BASE      + 0x00001400)
#define HWIO_MSS_CC_MSS_BBRX3_MISC_RMSK                                                               0xf
#define HWIO_MSS_CC_MSS_BBRX3_MISC_IN          \
        in_dword(HWIO_MSS_CC_MSS_BBRX3_MISC_ADDR)
#define HWIO_MSS_CC_MSS_BBRX3_MISC_INM(m)      \
        in_dword_masked(HWIO_MSS_CC_MSS_BBRX3_MISC_ADDR, m)
#define HWIO_MSS_CC_MSS_BBRX3_MISC_OUT(v)      \
        out_dword(HWIO_MSS_CC_MSS_BBRX3_MISC_ADDR,v)
#define HWIO_MSS_CC_MSS_BBRX3_MISC_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_CC_MSS_BBRX3_MISC_ADDR,m,v,HWIO_MSS_CC_MSS_BBRX3_MISC_IN)
#define HWIO_MSS_CC_MSS_BBRX3_MISC_SRC_DIV_BMSK                                                       0xf
#define HWIO_MSS_CC_MSS_BBRX3_MISC_SRC_DIV_SHFT                                                       0x0

#define HWIO_MSS_CC_MSS_MPLL1_CMD_ADDR                                                         (MSS_CC_MSS_CC_REG_REG_BASE      + 0x00001408)
#define HWIO_MSS_CC_MSS_MPLL1_CMD_RMSK                                                         0x80000001
#define HWIO_MSS_CC_MSS_MPLL1_CMD_IN          \
        in_dword(HWIO_MSS_CC_MSS_MPLL1_CMD_ADDR)
#define HWIO_MSS_CC_MSS_MPLL1_CMD_INM(m)      \
        in_dword_masked(HWIO_MSS_CC_MSS_MPLL1_CMD_ADDR, m)
#define HWIO_MSS_CC_MSS_MPLL1_CMD_OUT(v)      \
        out_dword(HWIO_MSS_CC_MSS_MPLL1_CMD_ADDR,v)
#define HWIO_MSS_CC_MSS_MPLL1_CMD_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_CC_MSS_MPLL1_CMD_ADDR,m,v,HWIO_MSS_CC_MSS_MPLL1_CMD_IN)
#define HWIO_MSS_CC_MSS_MPLL1_CMD_ROOT_OFF_BMSK                                                0x80000000
#define HWIO_MSS_CC_MSS_MPLL1_CMD_ROOT_OFF_SHFT                                                      0x1f
#define HWIO_MSS_CC_MSS_MPLL1_CMD_UPDATE_BMSK                                                         0x1
#define HWIO_MSS_CC_MSS_MPLL1_CMD_UPDATE_SHFT                                                         0x0

#define HWIO_MSS_CC_MSS_MPLL1_EARLY_DIV2_CBCR_ADDR                                             (MSS_CC_MSS_CC_REG_REG_BASE      + 0x0000140c)
#define HWIO_MSS_CC_MSS_MPLL1_EARLY_DIV2_CBCR_RMSK                                             0x80000001
#define HWIO_MSS_CC_MSS_MPLL1_EARLY_DIV2_CBCR_IN          \
        in_dword(HWIO_MSS_CC_MSS_MPLL1_EARLY_DIV2_CBCR_ADDR)
#define HWIO_MSS_CC_MSS_MPLL1_EARLY_DIV2_CBCR_INM(m)      \
        in_dword_masked(HWIO_MSS_CC_MSS_MPLL1_EARLY_DIV2_CBCR_ADDR, m)
#define HWIO_MSS_CC_MSS_MPLL1_EARLY_DIV2_CBCR_OUT(v)      \
        out_dword(HWIO_MSS_CC_MSS_MPLL1_EARLY_DIV2_CBCR_ADDR,v)
#define HWIO_MSS_CC_MSS_MPLL1_EARLY_DIV2_CBCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_CC_MSS_MPLL1_EARLY_DIV2_CBCR_ADDR,m,v,HWIO_MSS_CC_MSS_MPLL1_EARLY_DIV2_CBCR_IN)
#define HWIO_MSS_CC_MSS_MPLL1_EARLY_DIV2_CBCR_CLKOFF_BMSK                                      0x80000000
#define HWIO_MSS_CC_MSS_MPLL1_EARLY_DIV2_CBCR_CLKOFF_SHFT                                            0x1f
#define HWIO_MSS_CC_MSS_MPLL1_EARLY_DIV2_CBCR_CLKEN_BMSK                                              0x1
#define HWIO_MSS_CC_MSS_MPLL1_EARLY_DIV2_CBCR_CLKEN_SHFT                                              0x0

#define HWIO_MSS_CC_MSS_MPLL1_OUT_EARLY_DIV2_CBCR_ADDR                                         (MSS_CC_MSS_CC_REG_REG_BASE      + 0x00001410)
#define HWIO_MSS_CC_MSS_MPLL1_OUT_EARLY_DIV2_CBCR_RMSK                                         0x80000001
#define HWIO_MSS_CC_MSS_MPLL1_OUT_EARLY_DIV2_CBCR_IN          \
        in_dword(HWIO_MSS_CC_MSS_MPLL1_OUT_EARLY_DIV2_CBCR_ADDR)
#define HWIO_MSS_CC_MSS_MPLL1_OUT_EARLY_DIV2_CBCR_INM(m)      \
        in_dword_masked(HWIO_MSS_CC_MSS_MPLL1_OUT_EARLY_DIV2_CBCR_ADDR, m)
#define HWIO_MSS_CC_MSS_MPLL1_OUT_EARLY_DIV2_CBCR_OUT(v)      \
        out_dword(HWIO_MSS_CC_MSS_MPLL1_OUT_EARLY_DIV2_CBCR_ADDR,v)
#define HWIO_MSS_CC_MSS_MPLL1_OUT_EARLY_DIV2_CBCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_CC_MSS_MPLL1_OUT_EARLY_DIV2_CBCR_ADDR,m,v,HWIO_MSS_CC_MSS_MPLL1_OUT_EARLY_DIV2_CBCR_IN)
#define HWIO_MSS_CC_MSS_MPLL1_OUT_EARLY_DIV2_CBCR_CLKOFF_BMSK                                  0x80000000
#define HWIO_MSS_CC_MSS_MPLL1_OUT_EARLY_DIV2_CBCR_CLKOFF_SHFT                                        0x1f
#define HWIO_MSS_CC_MSS_MPLL1_OUT_EARLY_DIV2_CBCR_CLKEN_BMSK                                          0x1
#define HWIO_MSS_CC_MSS_MPLL1_OUT_EARLY_DIV2_CBCR_CLKEN_SHFT                                          0x0

#define HWIO_MSS_CC_DAC_CLK_CFG_ADDR                                                           (MSS_CC_MSS_CC_REG_REG_BASE      + 0x00001414)
#define HWIO_MSS_CC_DAC_CLK_CFG_RMSK                                                             0x15e03d
#define HWIO_MSS_CC_DAC_CLK_CFG_IN          \
        in_dword(HWIO_MSS_CC_DAC_CLK_CFG_ADDR)
#define HWIO_MSS_CC_DAC_CLK_CFG_INM(m)      \
        in_dword_masked(HWIO_MSS_CC_DAC_CLK_CFG_ADDR, m)
#define HWIO_MSS_CC_DAC_CLK_CFG_OUT(v)      \
        out_dword(HWIO_MSS_CC_DAC_CLK_CFG_ADDR,v)
#define HWIO_MSS_CC_DAC_CLK_CFG_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_CC_DAC_CLK_CFG_ADDR,m,v,HWIO_MSS_CC_DAC_CLK_CFG_IN)
#define HWIO_MSS_CC_DAC_CLK_CFG_FCAL_CLK_EN_BMSK                                                 0x100000
#define HWIO_MSS_CC_DAC_CLK_CFG_FCAL_CLK_EN_SHFT                                                     0x14
#define HWIO_MSS_CC_DAC_CLK_CFG_DAC0_CLK_EN_BMSK                                                  0x40000
#define HWIO_MSS_CC_DAC_CLK_CFG_DAC0_CLK_EN_SHFT                                                     0x12
#define HWIO_MSS_CC_DAC_CLK_CFG_EXT_DAC_CLK_EN_BMSK                                               0x10000
#define HWIO_MSS_CC_DAC_CLK_CFG_EXT_DAC_CLK_EN_SHFT                                                  0x10
#define HWIO_MSS_CC_DAC_CLK_CFG_EXT_FCAL_EN_BMSK                                                   0x8000
#define HWIO_MSS_CC_DAC_CLK_CFG_EXT_FCAL_EN_SHFT                                                      0xf
#define HWIO_MSS_CC_DAC_CLK_CFG_XO_FCAL_EN_BMSK                                                    0x4000
#define HWIO_MSS_CC_DAC_CLK_CFG_XO_FCAL_EN_SHFT                                                       0xe
#define HWIO_MSS_CC_DAC_CLK_CFG_XO_CLK_SEL_BMSK                                                    0x2000
#define HWIO_MSS_CC_DAC_CLK_CFG_XO_CLK_SEL_SHFT                                                       0xd
#define HWIO_MSS_CC_DAC_CLK_CFG_DAC0_DIV_PGM_BMSK                                                    0x3c
#define HWIO_MSS_CC_DAC_CLK_CFG_DAC0_DIV_PGM_SHFT                                                     0x2
#define HWIO_MSS_CC_DAC_CLK_CFG_PLL1_EN_BMSK                                                          0x1
#define HWIO_MSS_CC_DAC_CLK_CFG_PLL1_EN_SHFT                                                          0x0

#define HWIO_MSS_CC_CLK_XO_GATE_CFG_ADDR                                                       (MSS_CC_MSS_CC_REG_REG_BASE      + 0x00001418)
#define HWIO_MSS_CC_CLK_XO_GATE_CFG_RMSK                                                              0x1
#define HWIO_MSS_CC_CLK_XO_GATE_CFG_IN          \
        in_dword(HWIO_MSS_CC_CLK_XO_GATE_CFG_ADDR)
#define HWIO_MSS_CC_CLK_XO_GATE_CFG_INM(m)      \
        in_dword_masked(HWIO_MSS_CC_CLK_XO_GATE_CFG_ADDR, m)
#define HWIO_MSS_CC_CLK_XO_GATE_CFG_OUT(v)      \
        out_dword(HWIO_MSS_CC_CLK_XO_GATE_CFG_ADDR,v)
#define HWIO_MSS_CC_CLK_XO_GATE_CFG_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_CC_CLK_XO_GATE_CFG_ADDR,m,v,HWIO_MSS_CC_CLK_XO_GATE_CFG_IN)
#define HWIO_MSS_CC_CLK_XO_GATE_CFG_CLK_XO_GATE_CFG_BMSK                                              0x1
#define HWIO_MSS_CC_CLK_XO_GATE_CFG_CLK_XO_GATE_CFG_SHFT                                              0x0

#define HWIO_MSS_CC_MSS_ALT_RESET_Q6SS_ADDR                                                    (MSS_CC_MSS_CC_REG_REG_BASE      + 0x0000141c)
#define HWIO_MSS_CC_MSS_ALT_RESET_Q6SS_RMSK                                                           0x1
#define HWIO_MSS_CC_MSS_ALT_RESET_Q6SS_IN          \
        in_dword(HWIO_MSS_CC_MSS_ALT_RESET_Q6SS_ADDR)
#define HWIO_MSS_CC_MSS_ALT_RESET_Q6SS_INM(m)      \
        in_dword_masked(HWIO_MSS_CC_MSS_ALT_RESET_Q6SS_ADDR, m)
#define HWIO_MSS_CC_MSS_ALT_RESET_Q6SS_OUT(v)      \
        out_dword(HWIO_MSS_CC_MSS_ALT_RESET_Q6SS_ADDR,v)
#define HWIO_MSS_CC_MSS_ALT_RESET_Q6SS_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_CC_MSS_ALT_RESET_Q6SS_ADDR,m,v,HWIO_MSS_CC_MSS_ALT_RESET_Q6SS_IN)
#define HWIO_MSS_CC_MSS_ALT_RESET_Q6SS_EN_BMSK                                                        0x1
#define HWIO_MSS_CC_MSS_ALT_RESET_Q6SS_EN_SHFT                                                        0x0

#define HWIO_MSS_CC_DEBUG_MUX_MUXR_ADDR                                                        (MSS_CC_MSS_CC_REG_REG_BASE      + 0x00001420)
#define HWIO_MSS_CC_DEBUG_MUX_MUXR_RMSK                                                              0x3f
#define HWIO_MSS_CC_DEBUG_MUX_MUXR_IN          \
        in_dword(HWIO_MSS_CC_DEBUG_MUX_MUXR_ADDR)
#define HWIO_MSS_CC_DEBUG_MUX_MUXR_INM(m)      \
        in_dword_masked(HWIO_MSS_CC_DEBUG_MUX_MUXR_ADDR, m)
#define HWIO_MSS_CC_DEBUG_MUX_MUXR_OUT(v)      \
        out_dword(HWIO_MSS_CC_DEBUG_MUX_MUXR_ADDR,v)
#define HWIO_MSS_CC_DEBUG_MUX_MUXR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_CC_DEBUG_MUX_MUXR_ADDR,m,v,HWIO_MSS_CC_DEBUG_MUX_MUXR_IN)
#define HWIO_MSS_CC_DEBUG_MUX_MUXR_MUX_SEL_BMSK                                                      0x3f
#define HWIO_MSS_CC_DEBUG_MUX_MUXR_MUX_SEL_SHFT                                                       0x0

/*----------------------------------------------------------------------------
 * MODULE: MSS_UIM0_UART_DM
 *--------------------------------------------------------------------------*/

#define MSS_UIM0_UART_DM_REG_BASE                                                                               (MSS_TOP_BASE      + 0x00190000)
#define MSS_UIM0_UART_DM_REG_BASE_SIZE                                                                          0x200
#define MSS_UIM0_UART_DM_REG_BASE_USED                                                                          0x1a0

#define HWIO_MSS_UIM0_UART_DM_MR1_ADDR                                                                          (MSS_UIM0_UART_DM_REG_BASE      + 0x00000000)
#define HWIO_MSS_UIM0_UART_DM_MR1_RMSK                                                                          0xffffffff
#define HWIO_MSS_UIM0_UART_DM_MR1_IN          \
        in_dword(HWIO_MSS_UIM0_UART_DM_MR1_ADDR)
#define HWIO_MSS_UIM0_UART_DM_MR1_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM0_UART_DM_MR1_ADDR, m)
#define HWIO_MSS_UIM0_UART_DM_MR1_OUT(v)      \
        out_dword(HWIO_MSS_UIM0_UART_DM_MR1_ADDR,v)
#define HWIO_MSS_UIM0_UART_DM_MR1_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_UIM0_UART_DM_MR1_ADDR,m,v,HWIO_MSS_UIM0_UART_DM_MR1_IN)
#define HWIO_MSS_UIM0_UART_DM_MR1_AUTO_RFR_LEVEL1_BMSK                                                          0xffffff00
#define HWIO_MSS_UIM0_UART_DM_MR1_AUTO_RFR_LEVEL1_SHFT                                                                 0x8
#define HWIO_MSS_UIM0_UART_DM_MR1_RX_RDY_CTL_BMSK                                                                     0x80
#define HWIO_MSS_UIM0_UART_DM_MR1_RX_RDY_CTL_SHFT                                                                      0x7
#define HWIO_MSS_UIM0_UART_DM_MR1_CTS_CTL_BMSK                                                                        0x40
#define HWIO_MSS_UIM0_UART_DM_MR1_CTS_CTL_SHFT                                                                         0x6
#define HWIO_MSS_UIM0_UART_DM_MR1_AUTO_RFR_LEVEL0_BMSK                                                                0x3f
#define HWIO_MSS_UIM0_UART_DM_MR1_AUTO_RFR_LEVEL0_SHFT                                                                 0x0

#define HWIO_MSS_UIM0_UART_DM_MR2_ADDR                                                                          (MSS_UIM0_UART_DM_REG_BASE      + 0x00000004)
#define HWIO_MSS_UIM0_UART_DM_MR2_RMSK                                                                               0x7ff
#define HWIO_MSS_UIM0_UART_DM_MR2_IN          \
        in_dword(HWIO_MSS_UIM0_UART_DM_MR2_ADDR)
#define HWIO_MSS_UIM0_UART_DM_MR2_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM0_UART_DM_MR2_ADDR, m)
#define HWIO_MSS_UIM0_UART_DM_MR2_OUT(v)      \
        out_dword(HWIO_MSS_UIM0_UART_DM_MR2_ADDR,v)
#define HWIO_MSS_UIM0_UART_DM_MR2_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_UIM0_UART_DM_MR2_ADDR,m,v,HWIO_MSS_UIM0_UART_DM_MR2_IN)
#define HWIO_MSS_UIM0_UART_DM_MR2_RFR_CTS_LOOPBACK_BMSK                                                              0x400
#define HWIO_MSS_UIM0_UART_DM_MR2_RFR_CTS_LOOPBACK_SHFT                                                                0xa
#define HWIO_MSS_UIM0_UART_DM_MR2_RX_ERROR_CHAR_OFF_BMSK                                                             0x200
#define HWIO_MSS_UIM0_UART_DM_MR2_RX_ERROR_CHAR_OFF_SHFT                                                               0x9
#define HWIO_MSS_UIM0_UART_DM_MR2_RX_BREAK_ZERO_CHAR_OFF_BMSK                                                        0x100
#define HWIO_MSS_UIM0_UART_DM_MR2_RX_BREAK_ZERO_CHAR_OFF_SHFT                                                          0x8
#define HWIO_MSS_UIM0_UART_DM_MR2_LOOPBACK_BMSK                                                                       0x80
#define HWIO_MSS_UIM0_UART_DM_MR2_LOOPBACK_SHFT                                                                        0x7
#define HWIO_MSS_UIM0_UART_DM_MR2_ERROR_MODE_BMSK                                                                     0x40
#define HWIO_MSS_UIM0_UART_DM_MR2_ERROR_MODE_SHFT                                                                      0x6
#define HWIO_MSS_UIM0_UART_DM_MR2_BITS_PER_CHAR_BMSK                                                                  0x30
#define HWIO_MSS_UIM0_UART_DM_MR2_BITS_PER_CHAR_SHFT                                                                   0x4
#define HWIO_MSS_UIM0_UART_DM_MR2_STOP_BIT_LEN_BMSK                                                                    0xc
#define HWIO_MSS_UIM0_UART_DM_MR2_STOP_BIT_LEN_SHFT                                                                    0x2
#define HWIO_MSS_UIM0_UART_DM_MR2_PARITY_MODE_BMSK                                                                     0x3
#define HWIO_MSS_UIM0_UART_DM_MR2_PARITY_MODE_SHFT                                                                     0x0

#define HWIO_MSS_UIM0_UART_DM_CSR_SR_DEPRECATED_ADDR                                                            (MSS_UIM0_UART_DM_REG_BASE      + 0x00000008)
#define HWIO_MSS_UIM0_UART_DM_CSR_SR_DEPRECATED_RMSK                                                            0xffffffff
#define HWIO_MSS_UIM0_UART_DM_CSR_SR_DEPRECATED_IN          \
        in_dword(HWIO_MSS_UIM0_UART_DM_CSR_SR_DEPRECATED_ADDR)
#define HWIO_MSS_UIM0_UART_DM_CSR_SR_DEPRECATED_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM0_UART_DM_CSR_SR_DEPRECATED_ADDR, m)
#define HWIO_MSS_UIM0_UART_DM_CSR_SR_DEPRECATED_OUT(v)      \
        out_dword(HWIO_MSS_UIM0_UART_DM_CSR_SR_DEPRECATED_ADDR,v)
#define HWIO_MSS_UIM0_UART_DM_CSR_SR_DEPRECATED_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_UIM0_UART_DM_CSR_SR_DEPRECATED_ADDR,m,v,HWIO_MSS_UIM0_UART_DM_CSR_SR_DEPRECATED_IN)
#define HWIO_MSS_UIM0_UART_DM_CSR_SR_DEPRECATED_UART_DM_CSR_SR_DEPRECATED_BMSK                                  0xffffffff
#define HWIO_MSS_UIM0_UART_DM_CSR_SR_DEPRECATED_UART_DM_CSR_SR_DEPRECATED_SHFT                                         0x0

#define HWIO_MSS_UIM0_UART_DM_CR_MISR_DEPRECATED_ADDR                                                           (MSS_UIM0_UART_DM_REG_BASE      + 0x00000010)
#define HWIO_MSS_UIM0_UART_DM_CR_MISR_DEPRECATED_RMSK                                                           0xffffffff
#define HWIO_MSS_UIM0_UART_DM_CR_MISR_DEPRECATED_IN          \
        in_dword(HWIO_MSS_UIM0_UART_DM_CR_MISR_DEPRECATED_ADDR)
#define HWIO_MSS_UIM0_UART_DM_CR_MISR_DEPRECATED_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM0_UART_DM_CR_MISR_DEPRECATED_ADDR, m)
#define HWIO_MSS_UIM0_UART_DM_CR_MISR_DEPRECATED_OUT(v)      \
        out_dword(HWIO_MSS_UIM0_UART_DM_CR_MISR_DEPRECATED_ADDR,v)
#define HWIO_MSS_UIM0_UART_DM_CR_MISR_DEPRECATED_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_UIM0_UART_DM_CR_MISR_DEPRECATED_ADDR,m,v,HWIO_MSS_UIM0_UART_DM_CR_MISR_DEPRECATED_IN)
#define HWIO_MSS_UIM0_UART_DM_CR_MISR_DEPRECATED_UART_DM_CR_MISR_DEPRECATED_BMSK                                0xffffffff
#define HWIO_MSS_UIM0_UART_DM_CR_MISR_DEPRECATED_UART_DM_CR_MISR_DEPRECATED_SHFT                                       0x0

#define HWIO_MSS_UIM0_UART_DM_IMR_ISR_DEPRECATED_ADDR                                                           (MSS_UIM0_UART_DM_REG_BASE      + 0x00000014)
#define HWIO_MSS_UIM0_UART_DM_IMR_ISR_DEPRECATED_RMSK                                                           0xffffffff
#define HWIO_MSS_UIM0_UART_DM_IMR_ISR_DEPRECATED_IN          \
        in_dword(HWIO_MSS_UIM0_UART_DM_IMR_ISR_DEPRECATED_ADDR)
#define HWIO_MSS_UIM0_UART_DM_IMR_ISR_DEPRECATED_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM0_UART_DM_IMR_ISR_DEPRECATED_ADDR, m)
#define HWIO_MSS_UIM0_UART_DM_IMR_ISR_DEPRECATED_OUT(v)      \
        out_dword(HWIO_MSS_UIM0_UART_DM_IMR_ISR_DEPRECATED_ADDR,v)
#define HWIO_MSS_UIM0_UART_DM_IMR_ISR_DEPRECATED_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_UIM0_UART_DM_IMR_ISR_DEPRECATED_ADDR,m,v,HWIO_MSS_UIM0_UART_DM_IMR_ISR_DEPRECATED_IN)
#define HWIO_MSS_UIM0_UART_DM_IMR_ISR_DEPRECATED_UART_DM_IMR_ISR_DEPRECATED_BMSK                                0xffffffff
#define HWIO_MSS_UIM0_UART_DM_IMR_ISR_DEPRECATED_UART_DM_IMR_ISR_DEPRECATED_SHFT                                       0x0

#define HWIO_MSS_UIM0_UART_DM_IPR_ADDR                                                                          (MSS_UIM0_UART_DM_REG_BASE      + 0x00000018)
#define HWIO_MSS_UIM0_UART_DM_IPR_RMSK                                                                          0xffffffdf
#define HWIO_MSS_UIM0_UART_DM_IPR_IN          \
        in_dword(HWIO_MSS_UIM0_UART_DM_IPR_ADDR)
#define HWIO_MSS_UIM0_UART_DM_IPR_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM0_UART_DM_IPR_ADDR, m)
#define HWIO_MSS_UIM0_UART_DM_IPR_OUT(v)      \
        out_dword(HWIO_MSS_UIM0_UART_DM_IPR_ADDR,v)
#define HWIO_MSS_UIM0_UART_DM_IPR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_UIM0_UART_DM_IPR_ADDR,m,v,HWIO_MSS_UIM0_UART_DM_IPR_IN)
#define HWIO_MSS_UIM0_UART_DM_IPR_STALE_TIMEOUT_MSB_BMSK                                                        0xffffff80
#define HWIO_MSS_UIM0_UART_DM_IPR_STALE_TIMEOUT_MSB_SHFT                                                               0x7
#define HWIO_MSS_UIM0_UART_DM_IPR_SAMPLE_DATA_BMSK                                                                    0x40
#define HWIO_MSS_UIM0_UART_DM_IPR_SAMPLE_DATA_SHFT                                                                     0x6
#define HWIO_MSS_UIM0_UART_DM_IPR_STALE_TIMEOUT_LSB_BMSK                                                              0x1f
#define HWIO_MSS_UIM0_UART_DM_IPR_STALE_TIMEOUT_LSB_SHFT                                                               0x0

#define HWIO_MSS_UIM0_UART_DM_TFWR_ADDR                                                                         (MSS_UIM0_UART_DM_REG_BASE      + 0x0000001c)
#define HWIO_MSS_UIM0_UART_DM_TFWR_RMSK                                                                         0xffffffff
#define HWIO_MSS_UIM0_UART_DM_TFWR_IN          \
        in_dword(HWIO_MSS_UIM0_UART_DM_TFWR_ADDR)
#define HWIO_MSS_UIM0_UART_DM_TFWR_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM0_UART_DM_TFWR_ADDR, m)
#define HWIO_MSS_UIM0_UART_DM_TFWR_OUT(v)      \
        out_dword(HWIO_MSS_UIM0_UART_DM_TFWR_ADDR,v)
#define HWIO_MSS_UIM0_UART_DM_TFWR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_UIM0_UART_DM_TFWR_ADDR,m,v,HWIO_MSS_UIM0_UART_DM_TFWR_IN)
#define HWIO_MSS_UIM0_UART_DM_TFWR_TFW_BMSK                                                                     0xffffffff
#define HWIO_MSS_UIM0_UART_DM_TFWR_TFW_SHFT                                                                            0x0

#define HWIO_MSS_UIM0_UART_DM_RFWR_ADDR                                                                         (MSS_UIM0_UART_DM_REG_BASE      + 0x00000020)
#define HWIO_MSS_UIM0_UART_DM_RFWR_RMSK                                                                         0xffffffff
#define HWIO_MSS_UIM0_UART_DM_RFWR_IN          \
        in_dword(HWIO_MSS_UIM0_UART_DM_RFWR_ADDR)
#define HWIO_MSS_UIM0_UART_DM_RFWR_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM0_UART_DM_RFWR_ADDR, m)
#define HWIO_MSS_UIM0_UART_DM_RFWR_OUT(v)      \
        out_dword(HWIO_MSS_UIM0_UART_DM_RFWR_ADDR,v)
#define HWIO_MSS_UIM0_UART_DM_RFWR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_UIM0_UART_DM_RFWR_ADDR,m,v,HWIO_MSS_UIM0_UART_DM_RFWR_IN)
#define HWIO_MSS_UIM0_UART_DM_RFWR_RFW_BMSK                                                                     0xffffffff
#define HWIO_MSS_UIM0_UART_DM_RFWR_RFW_SHFT                                                                            0x0

#define HWIO_MSS_UIM0_UART_DM_HCR_ADDR                                                                          (MSS_UIM0_UART_DM_REG_BASE      + 0x00000024)
#define HWIO_MSS_UIM0_UART_DM_HCR_RMSK                                                                                0xff
#define HWIO_MSS_UIM0_UART_DM_HCR_IN          \
        in_dword(HWIO_MSS_UIM0_UART_DM_HCR_ADDR)
#define HWIO_MSS_UIM0_UART_DM_HCR_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM0_UART_DM_HCR_ADDR, m)
#define HWIO_MSS_UIM0_UART_DM_HCR_OUT(v)      \
        out_dword(HWIO_MSS_UIM0_UART_DM_HCR_ADDR,v)
#define HWIO_MSS_UIM0_UART_DM_HCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_UIM0_UART_DM_HCR_ADDR,m,v,HWIO_MSS_UIM0_UART_DM_HCR_IN)
#define HWIO_MSS_UIM0_UART_DM_HCR_DATA_BMSK                                                                           0xff
#define HWIO_MSS_UIM0_UART_DM_HCR_DATA_SHFT                                                                            0x0

#define HWIO_MSS_UIM0_UART_DM_DMRX_ADDR                                                                         (MSS_UIM0_UART_DM_REG_BASE      + 0x00000034)
#define HWIO_MSS_UIM0_UART_DM_DMRX_RMSK                                                                          0x1ffffff
#define HWIO_MSS_UIM0_UART_DM_DMRX_IN          \
        in_dword(HWIO_MSS_UIM0_UART_DM_DMRX_ADDR)
#define HWIO_MSS_UIM0_UART_DM_DMRX_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM0_UART_DM_DMRX_ADDR, m)
#define HWIO_MSS_UIM0_UART_DM_DMRX_OUT(v)      \
        out_dword(HWIO_MSS_UIM0_UART_DM_DMRX_ADDR,v)
#define HWIO_MSS_UIM0_UART_DM_DMRX_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_UIM0_UART_DM_DMRX_ADDR,m,v,HWIO_MSS_UIM0_UART_DM_DMRX_IN)
#define HWIO_MSS_UIM0_UART_DM_DMRX_RX_DM_CRCI_CHARS_BMSK                                                         0x1ffffff
#define HWIO_MSS_UIM0_UART_DM_DMRX_RX_DM_CRCI_CHARS_SHFT                                                               0x0

#define HWIO_MSS_UIM0_UART_DM_IRDA_RX_TOTAL_SNAP_DEPRECATED_ADDR                                                (MSS_UIM0_UART_DM_REG_BASE      + 0x00000038)
#define HWIO_MSS_UIM0_UART_DM_IRDA_RX_TOTAL_SNAP_DEPRECATED_RMSK                                                0xffffffff
#define HWIO_MSS_UIM0_UART_DM_IRDA_RX_TOTAL_SNAP_DEPRECATED_IN          \
        in_dword(HWIO_MSS_UIM0_UART_DM_IRDA_RX_TOTAL_SNAP_DEPRECATED_ADDR)
#define HWIO_MSS_UIM0_UART_DM_IRDA_RX_TOTAL_SNAP_DEPRECATED_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM0_UART_DM_IRDA_RX_TOTAL_SNAP_DEPRECATED_ADDR, m)
#define HWIO_MSS_UIM0_UART_DM_IRDA_RX_TOTAL_SNAP_DEPRECATED_OUT(v)      \
        out_dword(HWIO_MSS_UIM0_UART_DM_IRDA_RX_TOTAL_SNAP_DEPRECATED_ADDR,v)
#define HWIO_MSS_UIM0_UART_DM_IRDA_RX_TOTAL_SNAP_DEPRECATED_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_UIM0_UART_DM_IRDA_RX_TOTAL_SNAP_DEPRECATED_ADDR,m,v,HWIO_MSS_UIM0_UART_DM_IRDA_RX_TOTAL_SNAP_DEPRECATED_IN)
#define HWIO_MSS_UIM0_UART_DM_IRDA_RX_TOTAL_SNAP_DEPRECATED_UART_DM_IRDA_RX_TOTAL_SNAP_DEPRECATED_BMSK          0xffffffff
#define HWIO_MSS_UIM0_UART_DM_IRDA_RX_TOTAL_SNAP_DEPRECATED_UART_DM_IRDA_RX_TOTAL_SNAP_DEPRECATED_SHFT                 0x0

#define HWIO_MSS_UIM0_UART_DM_DMEN_ADDR                                                                         (MSS_UIM0_UART_DM_REG_BASE      + 0x0000003c)
#define HWIO_MSS_UIM0_UART_DM_DMEN_RMSK                                                                               0x3c
#define HWIO_MSS_UIM0_UART_DM_DMEN_IN          \
        in_dword(HWIO_MSS_UIM0_UART_DM_DMEN_ADDR)
#define HWIO_MSS_UIM0_UART_DM_DMEN_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM0_UART_DM_DMEN_ADDR, m)
#define HWIO_MSS_UIM0_UART_DM_DMEN_OUT(v)      \
        out_dword(HWIO_MSS_UIM0_UART_DM_DMEN_ADDR,v)
#define HWIO_MSS_UIM0_UART_DM_DMEN_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_UIM0_UART_DM_DMEN_ADDR,m,v,HWIO_MSS_UIM0_UART_DM_DMEN_IN)
#define HWIO_MSS_UIM0_UART_DM_DMEN_RX_SC_ENABLE_BMSK                                                                  0x20
#define HWIO_MSS_UIM0_UART_DM_DMEN_RX_SC_ENABLE_SHFT                                                                   0x5
#define HWIO_MSS_UIM0_UART_DM_DMEN_TX_SC_ENABLE_BMSK                                                                  0x10
#define HWIO_MSS_UIM0_UART_DM_DMEN_TX_SC_ENABLE_SHFT                                                                   0x4
#define HWIO_MSS_UIM0_UART_DM_DMEN_RX_BAM_ENABLE_BMSK                                                                  0x8
#define HWIO_MSS_UIM0_UART_DM_DMEN_RX_BAM_ENABLE_SHFT                                                                  0x3
#define HWIO_MSS_UIM0_UART_DM_DMEN_TX_BAM_ENABLE_BMSK                                                                  0x4
#define HWIO_MSS_UIM0_UART_DM_DMEN_TX_BAM_ENABLE_SHFT                                                                  0x2

#define HWIO_MSS_UIM0_UART_DM_NO_CHARS_FOR_TX_ADDR                                                              (MSS_UIM0_UART_DM_REG_BASE      + 0x00000040)
#define HWIO_MSS_UIM0_UART_DM_NO_CHARS_FOR_TX_RMSK                                                                0xffffff
#define HWIO_MSS_UIM0_UART_DM_NO_CHARS_FOR_TX_IN          \
        in_dword(HWIO_MSS_UIM0_UART_DM_NO_CHARS_FOR_TX_ADDR)
#define HWIO_MSS_UIM0_UART_DM_NO_CHARS_FOR_TX_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM0_UART_DM_NO_CHARS_FOR_TX_ADDR, m)
#define HWIO_MSS_UIM0_UART_DM_NO_CHARS_FOR_TX_OUT(v)      \
        out_dword(HWIO_MSS_UIM0_UART_DM_NO_CHARS_FOR_TX_ADDR,v)
#define HWIO_MSS_UIM0_UART_DM_NO_CHARS_FOR_TX_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_UIM0_UART_DM_NO_CHARS_FOR_TX_ADDR,m,v,HWIO_MSS_UIM0_UART_DM_NO_CHARS_FOR_TX_IN)
#define HWIO_MSS_UIM0_UART_DM_NO_CHARS_FOR_TX_TX_TOTAL_TRANS_LEN_BMSK                                             0xffffff
#define HWIO_MSS_UIM0_UART_DM_NO_CHARS_FOR_TX_TX_TOTAL_TRANS_LEN_SHFT                                                  0x0

#define HWIO_MSS_UIM0_UART_DM_BADR_ADDR                                                                         (MSS_UIM0_UART_DM_REG_BASE      + 0x00000044)
#define HWIO_MSS_UIM0_UART_DM_BADR_RMSK                                                                         0xfffffffc
#define HWIO_MSS_UIM0_UART_DM_BADR_IN          \
        in_dword(HWIO_MSS_UIM0_UART_DM_BADR_ADDR)
#define HWIO_MSS_UIM0_UART_DM_BADR_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM0_UART_DM_BADR_ADDR, m)
#define HWIO_MSS_UIM0_UART_DM_BADR_OUT(v)      \
        out_dword(HWIO_MSS_UIM0_UART_DM_BADR_ADDR,v)
#define HWIO_MSS_UIM0_UART_DM_BADR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_UIM0_UART_DM_BADR_ADDR,m,v,HWIO_MSS_UIM0_UART_DM_BADR_IN)
#define HWIO_MSS_UIM0_UART_DM_BADR_RX_BASE_ADDR_BMSK                                                            0xfffffffc
#define HWIO_MSS_UIM0_UART_DM_BADR_RX_BASE_ADDR_SHFT                                                                   0x2

#define HWIO_MSS_UIM0_UART_DM_TESTSL_ADDR                                                                       (MSS_UIM0_UART_DM_REG_BASE      + 0x00000048)
#define HWIO_MSS_UIM0_UART_DM_TESTSL_RMSK                                                                             0x1f
#define HWIO_MSS_UIM0_UART_DM_TESTSL_IN          \
        in_dword(HWIO_MSS_UIM0_UART_DM_TESTSL_ADDR)
#define HWIO_MSS_UIM0_UART_DM_TESTSL_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM0_UART_DM_TESTSL_ADDR, m)
#define HWIO_MSS_UIM0_UART_DM_TESTSL_OUT(v)      \
        out_dword(HWIO_MSS_UIM0_UART_DM_TESTSL_ADDR,v)
#define HWIO_MSS_UIM0_UART_DM_TESTSL_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_UIM0_UART_DM_TESTSL_ADDR,m,v,HWIO_MSS_UIM0_UART_DM_TESTSL_IN)
#define HWIO_MSS_UIM0_UART_DM_TESTSL_TEST_EN_BMSK                                                                     0x10
#define HWIO_MSS_UIM0_UART_DM_TESTSL_TEST_EN_SHFT                                                                      0x4
#define HWIO_MSS_UIM0_UART_DM_TESTSL_TEST_SEL_BMSK                                                                     0xf
#define HWIO_MSS_UIM0_UART_DM_TESTSL_TEST_SEL_SHFT                                                                     0x0

#define HWIO_MSS_UIM0_UART_DM_TXFS_ADDR                                                                         (MSS_UIM0_UART_DM_REG_BASE      + 0x0000004c)
#define HWIO_MSS_UIM0_UART_DM_TXFS_RMSK                                                                         0xffffffff
#define HWIO_MSS_UIM0_UART_DM_TXFS_IN          \
        in_dword(HWIO_MSS_UIM0_UART_DM_TXFS_ADDR)
#define HWIO_MSS_UIM0_UART_DM_TXFS_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM0_UART_DM_TXFS_ADDR, m)
#define HWIO_MSS_UIM0_UART_DM_TXFS_TX_FIFO_STATE_MSB_BMSK                                                       0xffffc000
#define HWIO_MSS_UIM0_UART_DM_TXFS_TX_FIFO_STATE_MSB_SHFT                                                              0xe
#define HWIO_MSS_UIM0_UART_DM_TXFS_TX_ASYNC_FIFO_STATE_BMSK                                                         0x3c00
#define HWIO_MSS_UIM0_UART_DM_TXFS_TX_ASYNC_FIFO_STATE_SHFT                                                            0xa
#define HWIO_MSS_UIM0_UART_DM_TXFS_TX_BUFFER_STATE_BMSK                                                              0x380
#define HWIO_MSS_UIM0_UART_DM_TXFS_TX_BUFFER_STATE_SHFT                                                                0x7
#define HWIO_MSS_UIM0_UART_DM_TXFS_TX_FIFO_STATE_LSB_BMSK                                                             0x7f
#define HWIO_MSS_UIM0_UART_DM_TXFS_TX_FIFO_STATE_LSB_SHFT                                                              0x0

#define HWIO_MSS_UIM0_UART_DM_RXFS_ADDR                                                                         (MSS_UIM0_UART_DM_REG_BASE      + 0x00000050)
#define HWIO_MSS_UIM0_UART_DM_RXFS_RMSK                                                                         0xffffffff
#define HWIO_MSS_UIM0_UART_DM_RXFS_IN          \
        in_dword(HWIO_MSS_UIM0_UART_DM_RXFS_ADDR)
#define HWIO_MSS_UIM0_UART_DM_RXFS_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM0_UART_DM_RXFS_ADDR, m)
#define HWIO_MSS_UIM0_UART_DM_RXFS_RX_FIFO_STATE_MSB_BMSK                                                       0xffffc000
#define HWIO_MSS_UIM0_UART_DM_RXFS_RX_FIFO_STATE_MSB_SHFT                                                              0xe
#define HWIO_MSS_UIM0_UART_DM_RXFS_RX_ASYNC_FIFO_STATE_BMSK                                                         0x3c00
#define HWIO_MSS_UIM0_UART_DM_RXFS_RX_ASYNC_FIFO_STATE_SHFT                                                            0xa
#define HWIO_MSS_UIM0_UART_DM_RXFS_RX_BUFFER_STATE_BMSK                                                              0x380
#define HWIO_MSS_UIM0_UART_DM_RXFS_RX_BUFFER_STATE_SHFT                                                                0x7
#define HWIO_MSS_UIM0_UART_DM_RXFS_RX_FIFO_STATE_LSB_BMSK                                                             0x7f
#define HWIO_MSS_UIM0_UART_DM_RXFS_RX_FIFO_STATE_LSB_SHFT                                                              0x0

#define HWIO_MSS_UIM0_UART_DM_MISR_MODE_ADDR                                                                    (MSS_UIM0_UART_DM_REG_BASE      + 0x00000060)
#define HWIO_MSS_UIM0_UART_DM_MISR_MODE_RMSK                                                                           0x3
#define HWIO_MSS_UIM0_UART_DM_MISR_MODE_IN          \
        in_dword(HWIO_MSS_UIM0_UART_DM_MISR_MODE_ADDR)
#define HWIO_MSS_UIM0_UART_DM_MISR_MODE_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM0_UART_DM_MISR_MODE_ADDR, m)
#define HWIO_MSS_UIM0_UART_DM_MISR_MODE_OUT(v)      \
        out_dword(HWIO_MSS_UIM0_UART_DM_MISR_MODE_ADDR,v)
#define HWIO_MSS_UIM0_UART_DM_MISR_MODE_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_UIM0_UART_DM_MISR_MODE_ADDR,m,v,HWIO_MSS_UIM0_UART_DM_MISR_MODE_IN)
#define HWIO_MSS_UIM0_UART_DM_MISR_MODE_MODE_BMSK                                                                      0x3
#define HWIO_MSS_UIM0_UART_DM_MISR_MODE_MODE_SHFT                                                                      0x0

#define HWIO_MSS_UIM0_UART_DM_MISR_RESET_ADDR                                                                   (MSS_UIM0_UART_DM_REG_BASE      + 0x00000064)
#define HWIO_MSS_UIM0_UART_DM_MISR_RESET_RMSK                                                                          0x1
#define HWIO_MSS_UIM0_UART_DM_MISR_RESET_OUT(v)      \
        out_dword(HWIO_MSS_UIM0_UART_DM_MISR_RESET_ADDR,v)
#define HWIO_MSS_UIM0_UART_DM_MISR_RESET_RESET_BMSK                                                                    0x1
#define HWIO_MSS_UIM0_UART_DM_MISR_RESET_RESET_SHFT                                                                    0x0

#define HWIO_MSS_UIM0_UART_DM_MISR_EXPORT_ADDR                                                                  (MSS_UIM0_UART_DM_REG_BASE      + 0x00000068)
#define HWIO_MSS_UIM0_UART_DM_MISR_EXPORT_RMSK                                                                         0x1
#define HWIO_MSS_UIM0_UART_DM_MISR_EXPORT_IN          \
        in_dword(HWIO_MSS_UIM0_UART_DM_MISR_EXPORT_ADDR)
#define HWIO_MSS_UIM0_UART_DM_MISR_EXPORT_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM0_UART_DM_MISR_EXPORT_ADDR, m)
#define HWIO_MSS_UIM0_UART_DM_MISR_EXPORT_OUT(v)      \
        out_dword(HWIO_MSS_UIM0_UART_DM_MISR_EXPORT_ADDR,v)
#define HWIO_MSS_UIM0_UART_DM_MISR_EXPORT_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_UIM0_UART_DM_MISR_EXPORT_ADDR,m,v,HWIO_MSS_UIM0_UART_DM_MISR_EXPORT_IN)
#define HWIO_MSS_UIM0_UART_DM_MISR_EXPORT_EXPORT_BMSK                                                                  0x1
#define HWIO_MSS_UIM0_UART_DM_MISR_EXPORT_EXPORT_SHFT                                                                  0x0

#define HWIO_MSS_UIM0_UART_DM_MISR_VAL_ADDR                                                                     (MSS_UIM0_UART_DM_REG_BASE      + 0x0000006c)
#define HWIO_MSS_UIM0_UART_DM_MISR_VAL_RMSK                                                                          0x3ff
#define HWIO_MSS_UIM0_UART_DM_MISR_VAL_IN          \
        in_dword(HWIO_MSS_UIM0_UART_DM_MISR_VAL_ADDR)
#define HWIO_MSS_UIM0_UART_DM_MISR_VAL_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM0_UART_DM_MISR_VAL_ADDR, m)
#define HWIO_MSS_UIM0_UART_DM_MISR_VAL_VAL_BMSK                                                                      0x3ff
#define HWIO_MSS_UIM0_UART_DM_MISR_VAL_VAL_SHFT                                                                        0x0

#define HWIO_MSS_UIM0_UART_DM_TF_RF_DEPRECATED_ADDR                                                             (MSS_UIM0_UART_DM_REG_BASE      + 0x00000070)
#define HWIO_MSS_UIM0_UART_DM_TF_RF_DEPRECATED_RMSK                                                             0xffffffff
#define HWIO_MSS_UIM0_UART_DM_TF_RF_DEPRECATED_IN          \
        in_dword(HWIO_MSS_UIM0_UART_DM_TF_RF_DEPRECATED_ADDR)
#define HWIO_MSS_UIM0_UART_DM_TF_RF_DEPRECATED_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM0_UART_DM_TF_RF_DEPRECATED_ADDR, m)
#define HWIO_MSS_UIM0_UART_DM_TF_RF_DEPRECATED_OUT(v)      \
        out_dword(HWIO_MSS_UIM0_UART_DM_TF_RF_DEPRECATED_ADDR,v)
#define HWIO_MSS_UIM0_UART_DM_TF_RF_DEPRECATED_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_UIM0_UART_DM_TF_RF_DEPRECATED_ADDR,m,v,HWIO_MSS_UIM0_UART_DM_TF_RF_DEPRECATED_IN)
#define HWIO_MSS_UIM0_UART_DM_TF_RF_DEPRECATED_UART_DM_TF_RF_DEPRECATED_BMSK                                    0xffffffff
#define HWIO_MSS_UIM0_UART_DM_TF_RF_DEPRECATED_UART_DM_TF_RF_DEPRECATED_SHFT                                           0x0

#define HWIO_MSS_UIM0_UART_DM_TF_RF_2_DEPRECATED_ADDR                                                           (MSS_UIM0_UART_DM_REG_BASE      + 0x00000074)
#define HWIO_MSS_UIM0_UART_DM_TF_RF_2_DEPRECATED_RMSK                                                           0xffffffff
#define HWIO_MSS_UIM0_UART_DM_TF_RF_2_DEPRECATED_IN          \
        in_dword(HWIO_MSS_UIM0_UART_DM_TF_RF_2_DEPRECATED_ADDR)
#define HWIO_MSS_UIM0_UART_DM_TF_RF_2_DEPRECATED_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM0_UART_DM_TF_RF_2_DEPRECATED_ADDR, m)
#define HWIO_MSS_UIM0_UART_DM_TF_RF_2_DEPRECATED_OUT(v)      \
        out_dword(HWIO_MSS_UIM0_UART_DM_TF_RF_2_DEPRECATED_ADDR,v)
#define HWIO_MSS_UIM0_UART_DM_TF_RF_2_DEPRECATED_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_UIM0_UART_DM_TF_RF_2_DEPRECATED_ADDR,m,v,HWIO_MSS_UIM0_UART_DM_TF_RF_2_DEPRECATED_IN)
#define HWIO_MSS_UIM0_UART_DM_TF_RF_2_DEPRECATED_UART_DM_TF_RF_2_DEPRECATED_BMSK                                0xffffffff
#define HWIO_MSS_UIM0_UART_DM_TF_RF_2_DEPRECATED_UART_DM_TF_RF_2_DEPRECATED_SHFT                                       0x0

#define HWIO_MSS_UIM0_UART_DM_TF_RF_3_DEPRECATED_ADDR                                                           (MSS_UIM0_UART_DM_REG_BASE      + 0x00000078)
#define HWIO_MSS_UIM0_UART_DM_TF_RF_3_DEPRECATED_RMSK                                                           0xffffffff
#define HWIO_MSS_UIM0_UART_DM_TF_RF_3_DEPRECATED_IN          \
        in_dword(HWIO_MSS_UIM0_UART_DM_TF_RF_3_DEPRECATED_ADDR)
#define HWIO_MSS_UIM0_UART_DM_TF_RF_3_DEPRECATED_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM0_UART_DM_TF_RF_3_DEPRECATED_ADDR, m)
#define HWIO_MSS_UIM0_UART_DM_TF_RF_3_DEPRECATED_OUT(v)      \
        out_dword(HWIO_MSS_UIM0_UART_DM_TF_RF_3_DEPRECATED_ADDR,v)
#define HWIO_MSS_UIM0_UART_DM_TF_RF_3_DEPRECATED_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_UIM0_UART_DM_TF_RF_3_DEPRECATED_ADDR,m,v,HWIO_MSS_UIM0_UART_DM_TF_RF_3_DEPRECATED_IN)
#define HWIO_MSS_UIM0_UART_DM_TF_RF_3_DEPRECATED_UART_DM_TF_RF_3_DEPRECATED_BMSK                                0xffffffff
#define HWIO_MSS_UIM0_UART_DM_TF_RF_3_DEPRECATED_UART_DM_TF_RF_3_DEPRECATED_SHFT                                       0x0

#define HWIO_MSS_UIM0_UART_DM_TF_RF_4_DEPRECATED_ADDR                                                           (MSS_UIM0_UART_DM_REG_BASE      + 0x0000007c)
#define HWIO_MSS_UIM0_UART_DM_TF_RF_4_DEPRECATED_RMSK                                                           0xffffffff
#define HWIO_MSS_UIM0_UART_DM_TF_RF_4_DEPRECATED_IN          \
        in_dword(HWIO_MSS_UIM0_UART_DM_TF_RF_4_DEPRECATED_ADDR)
#define HWIO_MSS_UIM0_UART_DM_TF_RF_4_DEPRECATED_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM0_UART_DM_TF_RF_4_DEPRECATED_ADDR, m)
#define HWIO_MSS_UIM0_UART_DM_TF_RF_4_DEPRECATED_OUT(v)      \
        out_dword(HWIO_MSS_UIM0_UART_DM_TF_RF_4_DEPRECATED_ADDR,v)
#define HWIO_MSS_UIM0_UART_DM_TF_RF_4_DEPRECATED_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_UIM0_UART_DM_TF_RF_4_DEPRECATED_ADDR,m,v,HWIO_MSS_UIM0_UART_DM_TF_RF_4_DEPRECATED_IN)
#define HWIO_MSS_UIM0_UART_DM_TF_RF_4_DEPRECATED_UART_DM_TF_RF_4_DEPRECATED_BMSK                                0xffffffff
#define HWIO_MSS_UIM0_UART_DM_TF_RF_4_DEPRECATED_UART_DM_TF_RF_4_DEPRECATED_SHFT                                       0x0

#define HWIO_MSS_UIM0_UART_DM_SIM_CFG_ADDR                                                                      (MSS_UIM0_UART_DM_REG_BASE      + 0x00000080)
#define HWIO_MSS_UIM0_UART_DM_SIM_CFG_RMSK                                                                         0x3ffef
#define HWIO_MSS_UIM0_UART_DM_SIM_CFG_IN          \
        in_dword(HWIO_MSS_UIM0_UART_DM_SIM_CFG_ADDR)
#define HWIO_MSS_UIM0_UART_DM_SIM_CFG_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM0_UART_DM_SIM_CFG_ADDR, m)
#define HWIO_MSS_UIM0_UART_DM_SIM_CFG_OUT(v)      \
        out_dword(HWIO_MSS_UIM0_UART_DM_SIM_CFG_ADDR,v)
#define HWIO_MSS_UIM0_UART_DM_SIM_CFG_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_UIM0_UART_DM_SIM_CFG_ADDR,m,v,HWIO_MSS_UIM0_UART_DM_SIM_CFG_IN)
#define HWIO_MSS_UIM0_UART_DM_SIM_CFG_UIM_TX_MODE_BMSK                                                             0x20000
#define HWIO_MSS_UIM0_UART_DM_SIM_CFG_UIM_TX_MODE_SHFT                                                                0x11
#define HWIO_MSS_UIM0_UART_DM_SIM_CFG_UIM_RX_MODE_BMSK                                                             0x10000
#define HWIO_MSS_UIM0_UART_DM_SIM_CFG_UIM_RX_MODE_SHFT                                                                0x10
#define HWIO_MSS_UIM0_UART_DM_SIM_CFG_SIM_STOP_BIT_LEN_BMSK                                                         0xff00
#define HWIO_MSS_UIM0_UART_DM_SIM_CFG_SIM_STOP_BIT_LEN_SHFT                                                            0x8
#define HWIO_MSS_UIM0_UART_DM_SIM_CFG_SIM_CLK_ON_BMSK                                                                 0x80
#define HWIO_MSS_UIM0_UART_DM_SIM_CFG_SIM_CLK_ON_SHFT                                                                  0x7
#define HWIO_MSS_UIM0_UART_DM_SIM_CFG_SIM_CLK_TD8_SEL_BMSK                                                            0x40
#define HWIO_MSS_UIM0_UART_DM_SIM_CFG_SIM_CLK_TD8_SEL_SHFT                                                             0x6
#define HWIO_MSS_UIM0_UART_DM_SIM_CFG_SIM_CLK_STOP_HIGH_BMSK                                                          0x20
#define HWIO_MSS_UIM0_UART_DM_SIM_CFG_SIM_CLK_STOP_HIGH_SHFT                                                           0x5
#define HWIO_MSS_UIM0_UART_DM_SIM_CFG_MASK_RX_BMSK                                                                     0x8
#define HWIO_MSS_UIM0_UART_DM_SIM_CFG_MASK_RX_SHFT                                                                     0x3
#define HWIO_MSS_UIM0_UART_DM_SIM_CFG_SWAP_D_BMSK                                                                      0x4
#define HWIO_MSS_UIM0_UART_DM_SIM_CFG_SWAP_D_SHFT                                                                      0x2
#define HWIO_MSS_UIM0_UART_DM_SIM_CFG_INV_D_BMSK                                                                       0x2
#define HWIO_MSS_UIM0_UART_DM_SIM_CFG_INV_D_SHFT                                                                       0x1
#define HWIO_MSS_UIM0_UART_DM_SIM_CFG_SIM_SEL_BMSK                                                                     0x1
#define HWIO_MSS_UIM0_UART_DM_SIM_CFG_SIM_SEL_SHFT                                                                     0x0

#define HWIO_MSS_UIM0_UART_DM_TEST_WR_ADDR_ADDR                                                                 (MSS_UIM0_UART_DM_REG_BASE      + 0x00000084)
#define HWIO_MSS_UIM0_UART_DM_TEST_WR_ADDR_RMSK                                                                 0xffffffff
#define HWIO_MSS_UIM0_UART_DM_TEST_WR_ADDR_IN          \
        in_dword(HWIO_MSS_UIM0_UART_DM_TEST_WR_ADDR_ADDR)
#define HWIO_MSS_UIM0_UART_DM_TEST_WR_ADDR_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM0_UART_DM_TEST_WR_ADDR_ADDR, m)
#define HWIO_MSS_UIM0_UART_DM_TEST_WR_ADDR_OUT(v)      \
        out_dword(HWIO_MSS_UIM0_UART_DM_TEST_WR_ADDR_ADDR,v)
#define HWIO_MSS_UIM0_UART_DM_TEST_WR_ADDR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_UIM0_UART_DM_TEST_WR_ADDR_ADDR,m,v,HWIO_MSS_UIM0_UART_DM_TEST_WR_ADDR_IN)
#define HWIO_MSS_UIM0_UART_DM_TEST_WR_ADDR_TEST_WR_ADDR_BMSK                                                    0xffffffff
#define HWIO_MSS_UIM0_UART_DM_TEST_WR_ADDR_TEST_WR_ADDR_SHFT                                                           0x0

#define HWIO_MSS_UIM0_UART_DM_TEST_WR_DATA_ADDR                                                                 (MSS_UIM0_UART_DM_REG_BASE      + 0x00000088)
#define HWIO_MSS_UIM0_UART_DM_TEST_WR_DATA_RMSK                                                                 0xffffffff
#define HWIO_MSS_UIM0_UART_DM_TEST_WR_DATA_OUT(v)      \
        out_dword(HWIO_MSS_UIM0_UART_DM_TEST_WR_DATA_ADDR,v)
#define HWIO_MSS_UIM0_UART_DM_TEST_WR_DATA_TEST_WR_DATA_BMSK                                                    0xffffffff
#define HWIO_MSS_UIM0_UART_DM_TEST_WR_DATA_TEST_WR_DATA_SHFT                                                           0x0

#define HWIO_MSS_UIM0_UART_DM_TEST_RD_ADDR_ADDR                                                                 (MSS_UIM0_UART_DM_REG_BASE      + 0x0000008c)
#define HWIO_MSS_UIM0_UART_DM_TEST_RD_ADDR_RMSK                                                                 0xffffffff
#define HWIO_MSS_UIM0_UART_DM_TEST_RD_ADDR_IN          \
        in_dword(HWIO_MSS_UIM0_UART_DM_TEST_RD_ADDR_ADDR)
#define HWIO_MSS_UIM0_UART_DM_TEST_RD_ADDR_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM0_UART_DM_TEST_RD_ADDR_ADDR, m)
#define HWIO_MSS_UIM0_UART_DM_TEST_RD_ADDR_OUT(v)      \
        out_dword(HWIO_MSS_UIM0_UART_DM_TEST_RD_ADDR_ADDR,v)
#define HWIO_MSS_UIM0_UART_DM_TEST_RD_ADDR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_UIM0_UART_DM_TEST_RD_ADDR_ADDR,m,v,HWIO_MSS_UIM0_UART_DM_TEST_RD_ADDR_IN)
#define HWIO_MSS_UIM0_UART_DM_TEST_RD_ADDR_TEST_RD_ADDR_BMSK                                                    0xffffffff
#define HWIO_MSS_UIM0_UART_DM_TEST_RD_ADDR_TEST_RD_ADDR_SHFT                                                           0x0

#define HWIO_MSS_UIM0_UART_DM_TEST_RD_DATA_ADDR                                                                 (MSS_UIM0_UART_DM_REG_BASE      + 0x00000090)
#define HWIO_MSS_UIM0_UART_DM_TEST_RD_DATA_RMSK                                                                 0xffffffff
#define HWIO_MSS_UIM0_UART_DM_TEST_RD_DATA_IN          \
        in_dword(HWIO_MSS_UIM0_UART_DM_TEST_RD_DATA_ADDR)
#define HWIO_MSS_UIM0_UART_DM_TEST_RD_DATA_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM0_UART_DM_TEST_RD_DATA_ADDR, m)
#define HWIO_MSS_UIM0_UART_DM_TEST_RD_DATA_TEST_RD_DATA_BMSK                                                    0xffffffff
#define HWIO_MSS_UIM0_UART_DM_TEST_RD_DATA_TEST_RD_DATA_SHFT                                                           0x0

#define HWIO_MSS_UIM0_UART_DM_CSR_ADDR                                                                          (MSS_UIM0_UART_DM_REG_BASE      + 0x000000a0)
#define HWIO_MSS_UIM0_UART_DM_CSR_RMSK                                                                                0xff
#define HWIO_MSS_UIM0_UART_DM_CSR_IN          \
        in_dword(HWIO_MSS_UIM0_UART_DM_CSR_ADDR)
#define HWIO_MSS_UIM0_UART_DM_CSR_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM0_UART_DM_CSR_ADDR, m)
#define HWIO_MSS_UIM0_UART_DM_CSR_OUT(v)      \
        out_dword(HWIO_MSS_UIM0_UART_DM_CSR_ADDR,v)
#define HWIO_MSS_UIM0_UART_DM_CSR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_UIM0_UART_DM_CSR_ADDR,m,v,HWIO_MSS_UIM0_UART_DM_CSR_IN)
#define HWIO_MSS_UIM0_UART_DM_CSR_UART_RX_CLK_SEL_BMSK                                                                0xf0
#define HWIO_MSS_UIM0_UART_DM_CSR_UART_RX_CLK_SEL_SHFT                                                                 0x4
#define HWIO_MSS_UIM0_UART_DM_CSR_UART_TX_CLK_SEL_BMSK                                                                 0xf
#define HWIO_MSS_UIM0_UART_DM_CSR_UART_TX_CLK_SEL_SHFT                                                                 0x0

#define HWIO_MSS_UIM0_UART_DM_SR_ADDR                                                                           (MSS_UIM0_UART_DM_REG_BASE      + 0x000000a4)
#define HWIO_MSS_UIM0_UART_DM_SR_RMSK                                                                               0x1fff
#define HWIO_MSS_UIM0_UART_DM_SR_IN          \
        in_dword(HWIO_MSS_UIM0_UART_DM_SR_ADDR)
#define HWIO_MSS_UIM0_UART_DM_SR_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM0_UART_DM_SR_ADDR, m)
#define HWIO_MSS_UIM0_UART_DM_SR_COMMAND_IN_PROGRESS_BMSK                                                           0x1000
#define HWIO_MSS_UIM0_UART_DM_SR_COMMAND_IN_PROGRESS_SHFT                                                              0xc
#define HWIO_MSS_UIM0_UART_DM_SR_TRANS_END_TRIGGER_BMSK                                                              0xc00
#define HWIO_MSS_UIM0_UART_DM_SR_TRANS_END_TRIGGER_SHFT                                                                0xa
#define HWIO_MSS_UIM0_UART_DM_SR_TRANS_ACTIVE_BMSK                                                                   0x200
#define HWIO_MSS_UIM0_UART_DM_SR_TRANS_ACTIVE_SHFT                                                                     0x9
#define HWIO_MSS_UIM0_UART_DM_SR_RX_BREAK_START_LAST_BMSK                                                            0x100
#define HWIO_MSS_UIM0_UART_DM_SR_RX_BREAK_START_LAST_SHFT                                                              0x8
#define HWIO_MSS_UIM0_UART_DM_SR_HUNT_CHAR_BMSK                                                                       0x80
#define HWIO_MSS_UIM0_UART_DM_SR_HUNT_CHAR_SHFT                                                                        0x7
#define HWIO_MSS_UIM0_UART_DM_SR_RX_BREAK_BMSK                                                                        0x40
#define HWIO_MSS_UIM0_UART_DM_SR_RX_BREAK_SHFT                                                                         0x6
#define HWIO_MSS_UIM0_UART_DM_SR_PAR_FRAME_ERR_BMSK                                                                   0x20
#define HWIO_MSS_UIM0_UART_DM_SR_PAR_FRAME_ERR_SHFT                                                                    0x5
#define HWIO_MSS_UIM0_UART_DM_SR_UART_OVERRUN_BMSK                                                                    0x10
#define HWIO_MSS_UIM0_UART_DM_SR_UART_OVERRUN_SHFT                                                                     0x4
#define HWIO_MSS_UIM0_UART_DM_SR_TXEMT_BMSK                                                                            0x8
#define HWIO_MSS_UIM0_UART_DM_SR_TXEMT_SHFT                                                                            0x3
#define HWIO_MSS_UIM0_UART_DM_SR_TXRDY_BMSK                                                                            0x4
#define HWIO_MSS_UIM0_UART_DM_SR_TXRDY_SHFT                                                                            0x2
#define HWIO_MSS_UIM0_UART_DM_SR_RXFULL_BMSK                                                                           0x2
#define HWIO_MSS_UIM0_UART_DM_SR_RXFULL_SHFT                                                                           0x1
#define HWIO_MSS_UIM0_UART_DM_SR_RXRDY_BMSK                                                                            0x1
#define HWIO_MSS_UIM0_UART_DM_SR_RXRDY_SHFT                                                                            0x0

#define HWIO_MSS_UIM0_UART_DM_CR_ADDR                                                                           (MSS_UIM0_UART_DM_REG_BASE      + 0x000000a8)
#define HWIO_MSS_UIM0_UART_DM_CR_RMSK                                                                                0xfff
#define HWIO_MSS_UIM0_UART_DM_CR_IN          \
        in_dword(HWIO_MSS_UIM0_UART_DM_CR_ADDR)
#define HWIO_MSS_UIM0_UART_DM_CR_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM0_UART_DM_CR_ADDR, m)
#define HWIO_MSS_UIM0_UART_DM_CR_OUT(v)      \
        out_dword(HWIO_MSS_UIM0_UART_DM_CR_ADDR,v)
#define HWIO_MSS_UIM0_UART_DM_CR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_UIM0_UART_DM_CR_ADDR,m,v,HWIO_MSS_UIM0_UART_DM_CR_IN)
#define HWIO_MSS_UIM0_UART_DM_CR_CHANNEL_COMMAND_MSB_BMSK                                                            0x800
#define HWIO_MSS_UIM0_UART_DM_CR_CHANNEL_COMMAND_MSB_SHFT                                                              0xb
#define HWIO_MSS_UIM0_UART_DM_CR_GENERAL_COMMAND_BMSK                                                                0x700
#define HWIO_MSS_UIM0_UART_DM_CR_GENERAL_COMMAND_SHFT                                                                  0x8
#define HWIO_MSS_UIM0_UART_DM_CR_CHANNEL_COMMAND_LSB_BMSK                                                             0xf0
#define HWIO_MSS_UIM0_UART_DM_CR_CHANNEL_COMMAND_LSB_SHFT                                                              0x4
#define HWIO_MSS_UIM0_UART_DM_CR_UART_TX_DISABLE_BMSK                                                                  0x8
#define HWIO_MSS_UIM0_UART_DM_CR_UART_TX_DISABLE_SHFT                                                                  0x3
#define HWIO_MSS_UIM0_UART_DM_CR_UART_TX_EN_BMSK                                                                       0x4
#define HWIO_MSS_UIM0_UART_DM_CR_UART_TX_EN_SHFT                                                                       0x2
#define HWIO_MSS_UIM0_UART_DM_CR_UART_RX_DISABLE_BMSK                                                                  0x2
#define HWIO_MSS_UIM0_UART_DM_CR_UART_RX_DISABLE_SHFT                                                                  0x1
#define HWIO_MSS_UIM0_UART_DM_CR_UART_RX_EN_BMSK                                                                       0x1
#define HWIO_MSS_UIM0_UART_DM_CR_UART_RX_EN_SHFT                                                                       0x0

#define HWIO_MSS_UIM0_UART_DM_MISR_ADDR                                                                         (MSS_UIM0_UART_DM_REG_BASE      + 0x000000ac)
#define HWIO_MSS_UIM0_UART_DM_MISR_RMSK                                                                            0x3ffff
#define HWIO_MSS_UIM0_UART_DM_MISR_IN          \
        in_dword(HWIO_MSS_UIM0_UART_DM_MISR_ADDR)
#define HWIO_MSS_UIM0_UART_DM_MISR_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM0_UART_DM_MISR_ADDR, m)
#define HWIO_MSS_UIM0_UART_DM_MISR_UART_MISR_BMSK                                                                  0x3ffff
#define HWIO_MSS_UIM0_UART_DM_MISR_UART_MISR_SHFT                                                                      0x0

#define HWIO_MSS_UIM0_UART_DM_IMR_ADDR                                                                          (MSS_UIM0_UART_DM_REG_BASE      + 0x000000b0)
#define HWIO_MSS_UIM0_UART_DM_IMR_RMSK                                                                             0x3ffff
#define HWIO_MSS_UIM0_UART_DM_IMR_IN          \
        in_dword(HWIO_MSS_UIM0_UART_DM_IMR_ADDR)
#define HWIO_MSS_UIM0_UART_DM_IMR_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM0_UART_DM_IMR_ADDR, m)
#define HWIO_MSS_UIM0_UART_DM_IMR_OUT(v)      \
        out_dword(HWIO_MSS_UIM0_UART_DM_IMR_ADDR,v)
#define HWIO_MSS_UIM0_UART_DM_IMR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_UIM0_UART_DM_IMR_ADDR,m,v,HWIO_MSS_UIM0_UART_DM_IMR_IN)
#define HWIO_MSS_UIM0_UART_DM_IMR_NO_FINISH_CMD_VIOL_BMSK                                                          0x20000
#define HWIO_MSS_UIM0_UART_DM_IMR_NO_FINISH_CMD_VIOL_SHFT                                                             0x11
#define HWIO_MSS_UIM0_UART_DM_IMR_WWT_IRQ_BMSK                                                                     0x10000
#define HWIO_MSS_UIM0_UART_DM_IMR_WWT_IRQ_SHFT                                                                        0x10
#define HWIO_MSS_UIM0_UART_DM_IMR_TXCOMP_IRQ_BMSK                                                                   0x8000
#define HWIO_MSS_UIM0_UART_DM_IMR_TXCOMP_IRQ_SHFT                                                                      0xf
#define HWIO_MSS_UIM0_UART_DM_IMR_RX_RD_ERROR_IRQ_BMSK                                                              0x4000
#define HWIO_MSS_UIM0_UART_DM_IMR_RX_RD_ERROR_IRQ_SHFT                                                                 0xe
#define HWIO_MSS_UIM0_UART_DM_IMR_TX_WR_ERROR_IRQ_BMSK                                                              0x2000
#define HWIO_MSS_UIM0_UART_DM_IMR_TX_WR_ERROR_IRQ_SHFT                                                                 0xd
#define HWIO_MSS_UIM0_UART_DM_IMR_PAR_FRAME_ERR_IRQ_BMSK                                                            0x1000
#define HWIO_MSS_UIM0_UART_DM_IMR_PAR_FRAME_ERR_IRQ_SHFT                                                               0xc
#define HWIO_MSS_UIM0_UART_DM_IMR_RXBREAK_END_BMSK                                                                   0x800
#define HWIO_MSS_UIM0_UART_DM_IMR_RXBREAK_END_SHFT                                                                     0xb
#define HWIO_MSS_UIM0_UART_DM_IMR_RXBREAK_START_BMSK                                                                 0x400
#define HWIO_MSS_UIM0_UART_DM_IMR_RXBREAK_START_SHFT                                                                   0xa
#define HWIO_MSS_UIM0_UART_DM_IMR_TX_DONE_BMSK                                                                       0x200
#define HWIO_MSS_UIM0_UART_DM_IMR_TX_DONE_SHFT                                                                         0x9
#define HWIO_MSS_UIM0_UART_DM_IMR_TX_ERROR_BMSK                                                                      0x100
#define HWIO_MSS_UIM0_UART_DM_IMR_TX_ERROR_SHFT                                                                        0x8
#define HWIO_MSS_UIM0_UART_DM_IMR_TX_READY_BMSK                                                                       0x80
#define HWIO_MSS_UIM0_UART_DM_IMR_TX_READY_SHFT                                                                        0x7
#define HWIO_MSS_UIM0_UART_DM_IMR_CURRENT_CTS_BMSK                                                                    0x40
#define HWIO_MSS_UIM0_UART_DM_IMR_CURRENT_CTS_SHFT                                                                     0x6
#define HWIO_MSS_UIM0_UART_DM_IMR_DELTA_CTS_BMSK                                                                      0x20
#define HWIO_MSS_UIM0_UART_DM_IMR_DELTA_CTS_SHFT                                                                       0x5
#define HWIO_MSS_UIM0_UART_DM_IMR_RXLEV_BMSK                                                                          0x10
#define HWIO_MSS_UIM0_UART_DM_IMR_RXLEV_SHFT                                                                           0x4
#define HWIO_MSS_UIM0_UART_DM_IMR_RXSTALE_BMSK                                                                         0x8
#define HWIO_MSS_UIM0_UART_DM_IMR_RXSTALE_SHFT                                                                         0x3
#define HWIO_MSS_UIM0_UART_DM_IMR_RXBREAK_CHANGE_BMSK                                                                  0x4
#define HWIO_MSS_UIM0_UART_DM_IMR_RXBREAK_CHANGE_SHFT                                                                  0x2
#define HWIO_MSS_UIM0_UART_DM_IMR_RXHUNT_BMSK                                                                          0x2
#define HWIO_MSS_UIM0_UART_DM_IMR_RXHUNT_SHFT                                                                          0x1
#define HWIO_MSS_UIM0_UART_DM_IMR_TXLEV_BMSK                                                                           0x1
#define HWIO_MSS_UIM0_UART_DM_IMR_TXLEV_SHFT                                                                           0x0

#define HWIO_MSS_UIM0_UART_DM_ISR_ADDR                                                                          (MSS_UIM0_UART_DM_REG_BASE      + 0x000000b4)
#define HWIO_MSS_UIM0_UART_DM_ISR_RMSK                                                                             0x3ffff
#define HWIO_MSS_UIM0_UART_DM_ISR_IN          \
        in_dword(HWIO_MSS_UIM0_UART_DM_ISR_ADDR)
#define HWIO_MSS_UIM0_UART_DM_ISR_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM0_UART_DM_ISR_ADDR, m)
#define HWIO_MSS_UIM0_UART_DM_ISR_NO_FINISH_CMD_VIOL_BMSK                                                          0x20000
#define HWIO_MSS_UIM0_UART_DM_ISR_NO_FINISH_CMD_VIOL_SHFT                                                             0x11
#define HWIO_MSS_UIM0_UART_DM_ISR_WWT_IRQ_BMSK                                                                     0x10000
#define HWIO_MSS_UIM0_UART_DM_ISR_WWT_IRQ_SHFT                                                                        0x10
#define HWIO_MSS_UIM0_UART_DM_ISR_TXCOMP_IRQ_BMSK                                                                   0x8000
#define HWIO_MSS_UIM0_UART_DM_ISR_TXCOMP_IRQ_SHFT                                                                      0xf
#define HWIO_MSS_UIM0_UART_DM_ISR_RX_RD_ERROR_IRQ_BMSK                                                              0x4000
#define HWIO_MSS_UIM0_UART_DM_ISR_RX_RD_ERROR_IRQ_SHFT                                                                 0xe
#define HWIO_MSS_UIM0_UART_DM_ISR_TX_WR_ERROR_IRQ_BMSK                                                              0x2000
#define HWIO_MSS_UIM0_UART_DM_ISR_TX_WR_ERROR_IRQ_SHFT                                                                 0xd
#define HWIO_MSS_UIM0_UART_DM_ISR_PAR_FRAME_ERR_IRQ_BMSK                                                            0x1000
#define HWIO_MSS_UIM0_UART_DM_ISR_PAR_FRAME_ERR_IRQ_SHFT                                                               0xc
#define HWIO_MSS_UIM0_UART_DM_ISR_RXBREAK_END_BMSK                                                                   0x800
#define HWIO_MSS_UIM0_UART_DM_ISR_RXBREAK_END_SHFT                                                                     0xb
#define HWIO_MSS_UIM0_UART_DM_ISR_RXBREAK_START_BMSK                                                                 0x400
#define HWIO_MSS_UIM0_UART_DM_ISR_RXBREAK_START_SHFT                                                                   0xa
#define HWIO_MSS_UIM0_UART_DM_ISR_TX_DONE_BMSK                                                                       0x200
#define HWIO_MSS_UIM0_UART_DM_ISR_TX_DONE_SHFT                                                                         0x9
#define HWIO_MSS_UIM0_UART_DM_ISR_TX_ERROR_BMSK                                                                      0x100
#define HWIO_MSS_UIM0_UART_DM_ISR_TX_ERROR_SHFT                                                                        0x8
#define HWIO_MSS_UIM0_UART_DM_ISR_TX_READY_BMSK                                                                       0x80
#define HWIO_MSS_UIM0_UART_DM_ISR_TX_READY_SHFT                                                                        0x7
#define HWIO_MSS_UIM0_UART_DM_ISR_CURRENT_CTS_BMSK                                                                    0x40
#define HWIO_MSS_UIM0_UART_DM_ISR_CURRENT_CTS_SHFT                                                                     0x6
#define HWIO_MSS_UIM0_UART_DM_ISR_DELTA_CTS_BMSK                                                                      0x20
#define HWIO_MSS_UIM0_UART_DM_ISR_DELTA_CTS_SHFT                                                                       0x5
#define HWIO_MSS_UIM0_UART_DM_ISR_RXLEV_BMSK                                                                          0x10
#define HWIO_MSS_UIM0_UART_DM_ISR_RXLEV_SHFT                                                                           0x4
#define HWIO_MSS_UIM0_UART_DM_ISR_RXSTALE_BMSK                                                                         0x8
#define HWIO_MSS_UIM0_UART_DM_ISR_RXSTALE_SHFT                                                                         0x3
#define HWIO_MSS_UIM0_UART_DM_ISR_RXBREAK_CHANGE_BMSK                                                                  0x4
#define HWIO_MSS_UIM0_UART_DM_ISR_RXBREAK_CHANGE_SHFT                                                                  0x2
#define HWIO_MSS_UIM0_UART_DM_ISR_RXHUNT_BMSK                                                                          0x2
#define HWIO_MSS_UIM0_UART_DM_ISR_RXHUNT_SHFT                                                                          0x1
#define HWIO_MSS_UIM0_UART_DM_ISR_TXLEV_BMSK                                                                           0x1
#define HWIO_MSS_UIM0_UART_DM_ISR_TXLEV_SHFT                                                                           0x0

#define HWIO_MSS_UIM0_UART_DM_IRDA_ADDR                                                                         (MSS_UIM0_UART_DM_REG_BASE      + 0x000000b8)
#define HWIO_MSS_UIM0_UART_DM_IRDA_RMSK                                                                               0x1f
#define HWIO_MSS_UIM0_UART_DM_IRDA_IN          \
        in_dword(HWIO_MSS_UIM0_UART_DM_IRDA_ADDR)
#define HWIO_MSS_UIM0_UART_DM_IRDA_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM0_UART_DM_IRDA_ADDR, m)
#define HWIO_MSS_UIM0_UART_DM_IRDA_OUT(v)      \
        out_dword(HWIO_MSS_UIM0_UART_DM_IRDA_ADDR,v)
#define HWIO_MSS_UIM0_UART_DM_IRDA_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_UIM0_UART_DM_IRDA_ADDR,m,v,HWIO_MSS_UIM0_UART_DM_IRDA_IN)
#define HWIO_MSS_UIM0_UART_DM_IRDA_MEDIUM_RATE_EN_BMSK                                                                0x10
#define HWIO_MSS_UIM0_UART_DM_IRDA_MEDIUM_RATE_EN_SHFT                                                                 0x4
#define HWIO_MSS_UIM0_UART_DM_IRDA_IRDA_LOOPBACK_BMSK                                                                  0x8
#define HWIO_MSS_UIM0_UART_DM_IRDA_IRDA_LOOPBACK_SHFT                                                                  0x3
#define HWIO_MSS_UIM0_UART_DM_IRDA_INVERT_IRDA_TX_BMSK                                                                 0x4
#define HWIO_MSS_UIM0_UART_DM_IRDA_INVERT_IRDA_TX_SHFT                                                                 0x2
#define HWIO_MSS_UIM0_UART_DM_IRDA_INVERT_IRDA_RX_BMSK                                                                 0x2
#define HWIO_MSS_UIM0_UART_DM_IRDA_INVERT_IRDA_RX_SHFT                                                                 0x1
#define HWIO_MSS_UIM0_UART_DM_IRDA_IRDA_EN_BMSK                                                                        0x1
#define HWIO_MSS_UIM0_UART_DM_IRDA_IRDA_EN_SHFT                                                                        0x0

#define HWIO_MSS_UIM0_UART_DM_RX_TOTAL_SNAP_ADDR                                                                (MSS_UIM0_UART_DM_REG_BASE      + 0x000000bc)
#define HWIO_MSS_UIM0_UART_DM_RX_TOTAL_SNAP_RMSK                                                                  0xffffff
#define HWIO_MSS_UIM0_UART_DM_RX_TOTAL_SNAP_IN          \
        in_dword(HWIO_MSS_UIM0_UART_DM_RX_TOTAL_SNAP_ADDR)
#define HWIO_MSS_UIM0_UART_DM_RX_TOTAL_SNAP_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM0_UART_DM_RX_TOTAL_SNAP_ADDR, m)
#define HWIO_MSS_UIM0_UART_DM_RX_TOTAL_SNAP_RX_TOTAL_BYTES_BMSK                                                   0xffffff
#define HWIO_MSS_UIM0_UART_DM_RX_TOTAL_SNAP_RX_TOTAL_BYTES_SHFT                                                        0x0

#define HWIO_MSS_UIM0_UART_DM_WWT_TIMEOUT_ADDR                                                                  (MSS_UIM0_UART_DM_REG_BASE      + 0x000000c0)
#define HWIO_MSS_UIM0_UART_DM_WWT_TIMEOUT_RMSK                                                                   0x3ffffff
#define HWIO_MSS_UIM0_UART_DM_WWT_TIMEOUT_IN          \
        in_dword(HWIO_MSS_UIM0_UART_DM_WWT_TIMEOUT_ADDR)
#define HWIO_MSS_UIM0_UART_DM_WWT_TIMEOUT_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM0_UART_DM_WWT_TIMEOUT_ADDR, m)
#define HWIO_MSS_UIM0_UART_DM_WWT_TIMEOUT_OUT(v)      \
        out_dword(HWIO_MSS_UIM0_UART_DM_WWT_TIMEOUT_ADDR,v)
#define HWIO_MSS_UIM0_UART_DM_WWT_TIMEOUT_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_UIM0_UART_DM_WWT_TIMEOUT_ADDR,m,v,HWIO_MSS_UIM0_UART_DM_WWT_TIMEOUT_IN)
#define HWIO_MSS_UIM0_UART_DM_WWT_TIMEOUT_WWT_CYCLE_REENABLE_BMSK                                                0x2000000
#define HWIO_MSS_UIM0_UART_DM_WWT_TIMEOUT_WWT_CYCLE_REENABLE_SHFT                                                     0x19
#define HWIO_MSS_UIM0_UART_DM_WWT_TIMEOUT_WWT_TIMEOUT_BMSK                                                       0x1ffffff
#define HWIO_MSS_UIM0_UART_DM_WWT_TIMEOUT_WWT_TIMEOUT_SHFT                                                             0x0

#define HWIO_MSS_UIM0_UART_DM_CLK_CTRL_ADDR                                                                     (MSS_UIM0_UART_DM_REG_BASE      + 0x000000c4)
#define HWIO_MSS_UIM0_UART_DM_CLK_CTRL_RMSK                                                                       0xf06731
#define HWIO_MSS_UIM0_UART_DM_CLK_CTRL_IN          \
        in_dword(HWIO_MSS_UIM0_UART_DM_CLK_CTRL_ADDR)
#define HWIO_MSS_UIM0_UART_DM_CLK_CTRL_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM0_UART_DM_CLK_CTRL_ADDR, m)
#define HWIO_MSS_UIM0_UART_DM_CLK_CTRL_OUT(v)      \
        out_dword(HWIO_MSS_UIM0_UART_DM_CLK_CTRL_ADDR,v)
#define HWIO_MSS_UIM0_UART_DM_CLK_CTRL_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_UIM0_UART_DM_CLK_CTRL_ADDR,m,v,HWIO_MSS_UIM0_UART_DM_CLK_CTRL_IN)
#define HWIO_MSS_UIM0_UART_DM_CLK_CTRL_UART_IRDA_CLK_CGC_OPEN_BMSK                                                0x800000
#define HWIO_MSS_UIM0_UART_DM_CLK_CTRL_UART_IRDA_CLK_CGC_OPEN_SHFT                                                    0x17
#define HWIO_MSS_UIM0_UART_DM_CLK_CTRL_UART_SIM_CLK_CGC_OPEN_BMSK                                                 0x400000
#define HWIO_MSS_UIM0_UART_DM_CLK_CTRL_UART_SIM_CLK_CGC_OPEN_SHFT                                                     0x16
#define HWIO_MSS_UIM0_UART_DM_CLK_CTRL_UART_RX_CLK_CGC_OPEN_BMSK                                                  0x200000
#define HWIO_MSS_UIM0_UART_DM_CLK_CTRL_UART_RX_CLK_CGC_OPEN_SHFT                                                      0x15
#define HWIO_MSS_UIM0_UART_DM_CLK_CTRL_UART_TX_CLK_CGC_OPEN_BMSK                                                  0x100000
#define HWIO_MSS_UIM0_UART_DM_CLK_CTRL_UART_TX_CLK_CGC_OPEN_SHFT                                                      0x14
#define HWIO_MSS_UIM0_UART_DM_CLK_CTRL_AHB_RX_BAM_CLK_CGC_OPEN_BMSK                                                 0x4000
#define HWIO_MSS_UIM0_UART_DM_CLK_CTRL_AHB_RX_BAM_CLK_CGC_OPEN_SHFT                                                    0xe
#define HWIO_MSS_UIM0_UART_DM_CLK_CTRL_AHB_TX_BAM_CLK_CGC_OPEN_BMSK                                                 0x2000
#define HWIO_MSS_UIM0_UART_DM_CLK_CTRL_AHB_TX_BAM_CLK_CGC_OPEN_SHFT                                                    0xd
#define HWIO_MSS_UIM0_UART_DM_CLK_CTRL_AHB_RX_CLK_CGC_OPEN_BMSK                                                      0x400
#define HWIO_MSS_UIM0_UART_DM_CLK_CTRL_AHB_RX_CLK_CGC_OPEN_SHFT                                                        0xa
#define HWIO_MSS_UIM0_UART_DM_CLK_CTRL_AHB_TX_CLK_CGC_OPEN_BMSK                                                      0x200
#define HWIO_MSS_UIM0_UART_DM_CLK_CTRL_AHB_TX_CLK_CGC_OPEN_SHFT                                                        0x9
#define HWIO_MSS_UIM0_UART_DM_CLK_CTRL_AHB_WR_CLK_CGC_OPEN_BMSK                                                      0x100
#define HWIO_MSS_UIM0_UART_DM_CLK_CTRL_AHB_WR_CLK_CGC_OPEN_SHFT                                                        0x8
#define HWIO_MSS_UIM0_UART_DM_CLK_CTRL_RX_ENABLE_CGC_OPT_BMSK                                                         0x20
#define HWIO_MSS_UIM0_UART_DM_CLK_CTRL_RX_ENABLE_CGC_OPT_SHFT                                                          0x5
#define HWIO_MSS_UIM0_UART_DM_CLK_CTRL_TX_ENABLE_CGC_OPT_BMSK                                                         0x10
#define HWIO_MSS_UIM0_UART_DM_CLK_CTRL_TX_ENABLE_CGC_OPT_SHFT                                                          0x4
#define HWIO_MSS_UIM0_UART_DM_CLK_CTRL_AHB_CLK_CGC_CLOSE_BMSK                                                          0x1
#define HWIO_MSS_UIM0_UART_DM_CLK_CTRL_AHB_CLK_CGC_CLOSE_SHFT                                                          0x0

#define HWIO_MSS_UIM0_UART_DM_BCR_ADDR                                                                          (MSS_UIM0_UART_DM_REG_BASE      + 0x000000c8)
#define HWIO_MSS_UIM0_UART_DM_BCR_RMSK                                                                                0x77
#define HWIO_MSS_UIM0_UART_DM_BCR_IN          \
        in_dword(HWIO_MSS_UIM0_UART_DM_BCR_ADDR)
#define HWIO_MSS_UIM0_UART_DM_BCR_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM0_UART_DM_BCR_ADDR, m)
#define HWIO_MSS_UIM0_UART_DM_BCR_OUT(v)      \
        out_dword(HWIO_MSS_UIM0_UART_DM_BCR_ADDR,v)
#define HWIO_MSS_UIM0_UART_DM_BCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_UIM0_UART_DM_BCR_ADDR,m,v,HWIO_MSS_UIM0_UART_DM_BCR_IN)
#define HWIO_MSS_UIM0_UART_DM_BCR_IGNORE_CR_PROT_VIOL_BMSK                                                            0x40
#define HWIO_MSS_UIM0_UART_DM_BCR_IGNORE_CR_PROT_VIOL_SHFT                                                             0x6
#define HWIO_MSS_UIM0_UART_DM_BCR_RX_DMRX_1BYTE_RES_EN_BMSK                                                           0x20
#define HWIO_MSS_UIM0_UART_DM_BCR_RX_DMRX_1BYTE_RES_EN_SHFT                                                            0x5
#define HWIO_MSS_UIM0_UART_DM_BCR_RX_STALE_IRQ_DMRX_EQUAL_BMSK                                                        0x10
#define HWIO_MSS_UIM0_UART_DM_BCR_RX_STALE_IRQ_DMRX_EQUAL_SHFT                                                         0x4
#define HWIO_MSS_UIM0_UART_DM_BCR_RX_DMRX_LOW_EN_BMSK                                                                  0x4
#define HWIO_MSS_UIM0_UART_DM_BCR_RX_DMRX_LOW_EN_SHFT                                                                  0x2
#define HWIO_MSS_UIM0_UART_DM_BCR_STALE_IRQ_EMPTY_BMSK                                                                 0x2
#define HWIO_MSS_UIM0_UART_DM_BCR_STALE_IRQ_EMPTY_SHFT                                                                 0x1
#define HWIO_MSS_UIM0_UART_DM_BCR_TX_BREAK_DISABLE_BMSK                                                                0x1
#define HWIO_MSS_UIM0_UART_DM_BCR_TX_BREAK_DISABLE_SHFT                                                                0x0

#define HWIO_MSS_UIM0_UART_DM_RX_TRANS_CTRL_ADDR                                                                (MSS_UIM0_UART_DM_REG_BASE      + 0x000000cc)
#define HWIO_MSS_UIM0_UART_DM_RX_TRANS_CTRL_RMSK                                                                       0x7
#define HWIO_MSS_UIM0_UART_DM_RX_TRANS_CTRL_IN          \
        in_dword(HWIO_MSS_UIM0_UART_DM_RX_TRANS_CTRL_ADDR)
#define HWIO_MSS_UIM0_UART_DM_RX_TRANS_CTRL_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM0_UART_DM_RX_TRANS_CTRL_ADDR, m)
#define HWIO_MSS_UIM0_UART_DM_RX_TRANS_CTRL_OUT(v)      \
        out_dword(HWIO_MSS_UIM0_UART_DM_RX_TRANS_CTRL_ADDR,v)
#define HWIO_MSS_UIM0_UART_DM_RX_TRANS_CTRL_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_UIM0_UART_DM_RX_TRANS_CTRL_ADDR,m,v,HWIO_MSS_UIM0_UART_DM_RX_TRANS_CTRL_IN)
#define HWIO_MSS_UIM0_UART_DM_RX_TRANS_CTRL_RX_DMRX_CYCLIC_EN_BMSK                                                     0x4
#define HWIO_MSS_UIM0_UART_DM_RX_TRANS_CTRL_RX_DMRX_CYCLIC_EN_SHFT                                                     0x2
#define HWIO_MSS_UIM0_UART_DM_RX_TRANS_CTRL_RX_TRANS_AUTO_RE_ACTIVATE_BMSK                                             0x2
#define HWIO_MSS_UIM0_UART_DM_RX_TRANS_CTRL_RX_TRANS_AUTO_RE_ACTIVATE_SHFT                                             0x1
#define HWIO_MSS_UIM0_UART_DM_RX_TRANS_CTRL_RX_STALE_AUTO_RE_EN_BMSK                                                   0x1
#define HWIO_MSS_UIM0_UART_DM_RX_TRANS_CTRL_RX_STALE_AUTO_RE_EN_SHFT                                                   0x0

#define HWIO_MSS_UIM0_UART_DM_DMRX_DBG_ADDR                                                                     (MSS_UIM0_UART_DM_REG_BASE      + 0x000000d0)
#define HWIO_MSS_UIM0_UART_DM_DMRX_DBG_RMSK                                                                      0x1ffffff
#define HWIO_MSS_UIM0_UART_DM_DMRX_DBG_IN          \
        in_dword(HWIO_MSS_UIM0_UART_DM_DMRX_DBG_ADDR)
#define HWIO_MSS_UIM0_UART_DM_DMRX_DBG_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM0_UART_DM_DMRX_DBG_ADDR, m)
#define HWIO_MSS_UIM0_UART_DM_DMRX_DBG_UART_DM_DMRX_VAL_BMSK                                                     0x1ffffff
#define HWIO_MSS_UIM0_UART_DM_DMRX_DBG_UART_DM_DMRX_VAL_SHFT                                                           0x0

#define HWIO_MSS_UIM0_UART_DM_FSM_STATUS_ADDR                                                                   (MSS_UIM0_UART_DM_REG_BASE      + 0x000000d4)
#define HWIO_MSS_UIM0_UART_DM_FSM_STATUS_RMSK                                                                   0x37377771
#define HWIO_MSS_UIM0_UART_DM_FSM_STATUS_IN          \
        in_dword(HWIO_MSS_UIM0_UART_DM_FSM_STATUS_ADDR)
#define HWIO_MSS_UIM0_UART_DM_FSM_STATUS_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM0_UART_DM_FSM_STATUS_ADDR, m)
#define HWIO_MSS_UIM0_UART_DM_FSM_STATUS_TX_COMP_FSM_BMSK                                                       0x30000000
#define HWIO_MSS_UIM0_UART_DM_FSM_STATUS_TX_COMP_FSM_SHFT                                                             0x1c
#define HWIO_MSS_UIM0_UART_DM_FSM_STATUS_RX_PACK_FSM_BMSK                                                        0x7000000
#define HWIO_MSS_UIM0_UART_DM_FSM_STATUS_RX_PACK_FSM_SHFT                                                             0x18
#define HWIO_MSS_UIM0_UART_DM_FSM_STATUS_RX_TRANS_FSM_BMSK                                                        0x300000
#define HWIO_MSS_UIM0_UART_DM_FSM_STATUS_RX_TRANS_FSM_SHFT                                                            0x14
#define HWIO_MSS_UIM0_UART_DM_FSM_STATUS_TX_TRANS_FSM_BMSK                                                         0x70000
#define HWIO_MSS_UIM0_UART_DM_FSM_STATUS_TX_TRANS_FSM_SHFT                                                            0x10
#define HWIO_MSS_UIM0_UART_DM_FSM_STATUS_RX_PRO_TRANS_END_FSM_BMSK                                                  0x7000
#define HWIO_MSS_UIM0_UART_DM_FSM_STATUS_RX_PRO_TRANS_END_FSM_SHFT                                                     0xc
#define HWIO_MSS_UIM0_UART_DM_FSM_STATUS_RX_PRO_ACTIVE_FSM_BMSK                                                      0x700
#define HWIO_MSS_UIM0_UART_DM_FSM_STATUS_RX_PRO_ACTIVE_FSM_SHFT                                                        0x8
#define HWIO_MSS_UIM0_UART_DM_FSM_STATUS_TX_CON_TRANS_END_FSM_BMSK                                                    0x70
#define HWIO_MSS_UIM0_UART_DM_FSM_STATUS_TX_CON_TRANS_END_FSM_SHFT                                                     0x4
#define HWIO_MSS_UIM0_UART_DM_FSM_STATUS_RX_TRANSFER_ACTIVE_BMSK                                                       0x1
#define HWIO_MSS_UIM0_UART_DM_FSM_STATUS_RX_TRANSFER_ACTIVE_SHFT                                                       0x0

#define HWIO_MSS_UIM0_UART_DM_HW_VERSION_ADDR                                                                   (MSS_UIM0_UART_DM_REG_BASE      + 0x000000d8)
#define HWIO_MSS_UIM0_UART_DM_HW_VERSION_RMSK                                                                   0xffffffff
#define HWIO_MSS_UIM0_UART_DM_HW_VERSION_IN          \
        in_dword(HWIO_MSS_UIM0_UART_DM_HW_VERSION_ADDR)
#define HWIO_MSS_UIM0_UART_DM_HW_VERSION_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM0_UART_DM_HW_VERSION_ADDR, m)
#define HWIO_MSS_UIM0_UART_DM_HW_VERSION_HW_VERSION_MAJOR_BMSK                                                  0xf0000000
#define HWIO_MSS_UIM0_UART_DM_HW_VERSION_HW_VERSION_MAJOR_SHFT                                                        0x1c
#define HWIO_MSS_UIM0_UART_DM_HW_VERSION_HW_VERSION_MINOR_BMSK                                                   0xfff0000
#define HWIO_MSS_UIM0_UART_DM_HW_VERSION_HW_VERSION_MINOR_SHFT                                                        0x10
#define HWIO_MSS_UIM0_UART_DM_HW_VERSION_HW_VERSION_STEP_BMSK                                                       0xffff
#define HWIO_MSS_UIM0_UART_DM_HW_VERSION_HW_VERSION_STEP_SHFT                                                          0x0

#define HWIO_MSS_UIM0_UART_DM_GENERICS_ADDR                                                                     (MSS_UIM0_UART_DM_REG_BASE      + 0x000000dc)
#define HWIO_MSS_UIM0_UART_DM_GENERICS_RMSK                                                                           0xff
#define HWIO_MSS_UIM0_UART_DM_GENERICS_IN          \
        in_dword(HWIO_MSS_UIM0_UART_DM_GENERICS_ADDR)
#define HWIO_MSS_UIM0_UART_DM_GENERICS_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM0_UART_DM_GENERICS_ADDR, m)
#define HWIO_MSS_UIM0_UART_DM_GENERICS_GENERIC_BAM_IFC_BMSK                                                           0x80
#define HWIO_MSS_UIM0_UART_DM_GENERICS_GENERIC_BAM_IFC_SHFT                                                            0x7
#define HWIO_MSS_UIM0_UART_DM_GENERICS_GENERIC_DM_IFC_BMSK                                                            0x40
#define HWIO_MSS_UIM0_UART_DM_GENERICS_GENERIC_DM_IFC_SHFT                                                             0x6
#define HWIO_MSS_UIM0_UART_DM_GENERICS_GENERIC_IRDA_IFC_BMSK                                                          0x20
#define HWIO_MSS_UIM0_UART_DM_GENERICS_GENERIC_IRDA_IFC_SHFT                                                           0x5
#define HWIO_MSS_UIM0_UART_DM_GENERICS_GENERIC_SIM_GLUE_BMSK                                                          0x10
#define HWIO_MSS_UIM0_UART_DM_GENERICS_GENERIC_SIM_GLUE_SHFT                                                           0x4
#define HWIO_MSS_UIM0_UART_DM_GENERICS_GENERIC_RAM_ADDR_WIDTH_BMSK                                                     0xf
#define HWIO_MSS_UIM0_UART_DM_GENERICS_GENERIC_RAM_ADDR_WIDTH_SHFT                                                     0x0

#define HWIO_MSS_UIM0_UART_DM_ISR_CLR_ADDR                                                                      (MSS_UIM0_UART_DM_REG_BASE      + 0x000000e0)
#define HWIO_MSS_UIM0_UART_DM_ISR_CLR_RMSK                                                                         0x20000
#define HWIO_MSS_UIM0_UART_DM_ISR_CLR_OUT(v)      \
        out_dword(HWIO_MSS_UIM0_UART_DM_ISR_CLR_ADDR,v)
#define HWIO_MSS_UIM0_UART_DM_ISR_CLR_NO_FINISH_CMD_VIOL_BMSK                                                      0x20000
#define HWIO_MSS_UIM0_UART_DM_ISR_CLR_NO_FINISH_CMD_VIOL_SHFT                                                         0x11

#define HWIO_MSS_UIM0_UART_DM_TF_ADDR                                                                           (MSS_UIM0_UART_DM_REG_BASE      + 0x00000100)
#define HWIO_MSS_UIM0_UART_DM_TF_RMSK                                                                           0xffffffff
#define HWIO_MSS_UIM0_UART_DM_TF_OUT(v)      \
        out_dword(HWIO_MSS_UIM0_UART_DM_TF_ADDR,v)
#define HWIO_MSS_UIM0_UART_DM_TF_UART_TF_BMSK                                                                   0xffffffff
#define HWIO_MSS_UIM0_UART_DM_TF_UART_TF_SHFT                                                                          0x0

#define HWIO_MSS_UIM0_UART_DM_TF_2_ADDR                                                                         (MSS_UIM0_UART_DM_REG_BASE      + 0x00000104)
#define HWIO_MSS_UIM0_UART_DM_TF_2_RMSK                                                                         0xffffffff
#define HWIO_MSS_UIM0_UART_DM_TF_2_OUT(v)      \
        out_dword(HWIO_MSS_UIM0_UART_DM_TF_2_ADDR,v)
#define HWIO_MSS_UIM0_UART_DM_TF_2_UART_TF_BMSK                                                                 0xffffffff
#define HWIO_MSS_UIM0_UART_DM_TF_2_UART_TF_SHFT                                                                        0x0

#define HWIO_MSS_UIM0_UART_DM_TF_3_ADDR                                                                         (MSS_UIM0_UART_DM_REG_BASE      + 0x00000108)
#define HWIO_MSS_UIM0_UART_DM_TF_3_RMSK                                                                         0xffffffff
#define HWIO_MSS_UIM0_UART_DM_TF_3_OUT(v)      \
        out_dword(HWIO_MSS_UIM0_UART_DM_TF_3_ADDR,v)
#define HWIO_MSS_UIM0_UART_DM_TF_3_UART_TF_BMSK                                                                 0xffffffff
#define HWIO_MSS_UIM0_UART_DM_TF_3_UART_TF_SHFT                                                                        0x0

#define HWIO_MSS_UIM0_UART_DM_TF_4_ADDR                                                                         (MSS_UIM0_UART_DM_REG_BASE      + 0x0000010c)
#define HWIO_MSS_UIM0_UART_DM_TF_4_RMSK                                                                         0xffffffff
#define HWIO_MSS_UIM0_UART_DM_TF_4_OUT(v)      \
        out_dword(HWIO_MSS_UIM0_UART_DM_TF_4_ADDR,v)
#define HWIO_MSS_UIM0_UART_DM_TF_4_UART_TF_BMSK                                                                 0xffffffff
#define HWIO_MSS_UIM0_UART_DM_TF_4_UART_TF_SHFT                                                                        0x0

#define HWIO_MSS_UIM0_UART_DM_TF_5_ADDR                                                                         (MSS_UIM0_UART_DM_REG_BASE      + 0x00000110)
#define HWIO_MSS_UIM0_UART_DM_TF_5_RMSK                                                                         0xffffffff
#define HWIO_MSS_UIM0_UART_DM_TF_5_OUT(v)      \
        out_dword(HWIO_MSS_UIM0_UART_DM_TF_5_ADDR,v)
#define HWIO_MSS_UIM0_UART_DM_TF_5_UART_TF_BMSK                                                                 0xffffffff
#define HWIO_MSS_UIM0_UART_DM_TF_5_UART_TF_SHFT                                                                        0x0

#define HWIO_MSS_UIM0_UART_DM_TF_6_ADDR                                                                         (MSS_UIM0_UART_DM_REG_BASE      + 0x00000114)
#define HWIO_MSS_UIM0_UART_DM_TF_6_RMSK                                                                         0xffffffff
#define HWIO_MSS_UIM0_UART_DM_TF_6_OUT(v)      \
        out_dword(HWIO_MSS_UIM0_UART_DM_TF_6_ADDR,v)
#define HWIO_MSS_UIM0_UART_DM_TF_6_UART_TF_BMSK                                                                 0xffffffff
#define HWIO_MSS_UIM0_UART_DM_TF_6_UART_TF_SHFT                                                                        0x0

#define HWIO_MSS_UIM0_UART_DM_TF_7_ADDR                                                                         (MSS_UIM0_UART_DM_REG_BASE      + 0x00000118)
#define HWIO_MSS_UIM0_UART_DM_TF_7_RMSK                                                                         0xffffffff
#define HWIO_MSS_UIM0_UART_DM_TF_7_OUT(v)      \
        out_dword(HWIO_MSS_UIM0_UART_DM_TF_7_ADDR,v)
#define HWIO_MSS_UIM0_UART_DM_TF_7_UART_TF_BMSK                                                                 0xffffffff
#define HWIO_MSS_UIM0_UART_DM_TF_7_UART_TF_SHFT                                                                        0x0

#define HWIO_MSS_UIM0_UART_DM_TF_8_ADDR                                                                         (MSS_UIM0_UART_DM_REG_BASE      + 0x0000011c)
#define HWIO_MSS_UIM0_UART_DM_TF_8_RMSK                                                                         0xffffffff
#define HWIO_MSS_UIM0_UART_DM_TF_8_OUT(v)      \
        out_dword(HWIO_MSS_UIM0_UART_DM_TF_8_ADDR,v)
#define HWIO_MSS_UIM0_UART_DM_TF_8_UART_TF_BMSK                                                                 0xffffffff
#define HWIO_MSS_UIM0_UART_DM_TF_8_UART_TF_SHFT                                                                        0x0

#define HWIO_MSS_UIM0_UART_DM_TF_9_ADDR                                                                         (MSS_UIM0_UART_DM_REG_BASE      + 0x00000120)
#define HWIO_MSS_UIM0_UART_DM_TF_9_RMSK                                                                         0xffffffff
#define HWIO_MSS_UIM0_UART_DM_TF_9_OUT(v)      \
        out_dword(HWIO_MSS_UIM0_UART_DM_TF_9_ADDR,v)
#define HWIO_MSS_UIM0_UART_DM_TF_9_UART_TF_BMSK                                                                 0xffffffff
#define HWIO_MSS_UIM0_UART_DM_TF_9_UART_TF_SHFT                                                                        0x0

#define HWIO_MSS_UIM0_UART_DM_TF_10_ADDR                                                                        (MSS_UIM0_UART_DM_REG_BASE      + 0x00000124)
#define HWIO_MSS_UIM0_UART_DM_TF_10_RMSK                                                                        0xffffffff
#define HWIO_MSS_UIM0_UART_DM_TF_10_OUT(v)      \
        out_dword(HWIO_MSS_UIM0_UART_DM_TF_10_ADDR,v)
#define HWIO_MSS_UIM0_UART_DM_TF_10_UART_TF_BMSK                                                                0xffffffff
#define HWIO_MSS_UIM0_UART_DM_TF_10_UART_TF_SHFT                                                                       0x0

#define HWIO_MSS_UIM0_UART_DM_TF_11_ADDR                                                                        (MSS_UIM0_UART_DM_REG_BASE      + 0x00000128)
#define HWIO_MSS_UIM0_UART_DM_TF_11_RMSK                                                                        0xffffffff
#define HWIO_MSS_UIM0_UART_DM_TF_11_OUT(v)      \
        out_dword(HWIO_MSS_UIM0_UART_DM_TF_11_ADDR,v)
#define HWIO_MSS_UIM0_UART_DM_TF_11_UART_TF_BMSK                                                                0xffffffff
#define HWIO_MSS_UIM0_UART_DM_TF_11_UART_TF_SHFT                                                                       0x0

#define HWIO_MSS_UIM0_UART_DM_TF_12_ADDR                                                                        (MSS_UIM0_UART_DM_REG_BASE      + 0x0000012c)
#define HWIO_MSS_UIM0_UART_DM_TF_12_RMSK                                                                        0xffffffff
#define HWIO_MSS_UIM0_UART_DM_TF_12_OUT(v)      \
        out_dword(HWIO_MSS_UIM0_UART_DM_TF_12_ADDR,v)
#define HWIO_MSS_UIM0_UART_DM_TF_12_UART_TF_BMSK                                                                0xffffffff
#define HWIO_MSS_UIM0_UART_DM_TF_12_UART_TF_SHFT                                                                       0x0

#define HWIO_MSS_UIM0_UART_DM_TF_13_ADDR                                                                        (MSS_UIM0_UART_DM_REG_BASE      + 0x00000130)
#define HWIO_MSS_UIM0_UART_DM_TF_13_RMSK                                                                        0xffffffff
#define HWIO_MSS_UIM0_UART_DM_TF_13_OUT(v)      \
        out_dword(HWIO_MSS_UIM0_UART_DM_TF_13_ADDR,v)
#define HWIO_MSS_UIM0_UART_DM_TF_13_UART_TF_BMSK                                                                0xffffffff
#define HWIO_MSS_UIM0_UART_DM_TF_13_UART_TF_SHFT                                                                       0x0

#define HWIO_MSS_UIM0_UART_DM_TF_14_ADDR                                                                        (MSS_UIM0_UART_DM_REG_BASE      + 0x00000134)
#define HWIO_MSS_UIM0_UART_DM_TF_14_RMSK                                                                        0xffffffff
#define HWIO_MSS_UIM0_UART_DM_TF_14_OUT(v)      \
        out_dword(HWIO_MSS_UIM0_UART_DM_TF_14_ADDR,v)
#define HWIO_MSS_UIM0_UART_DM_TF_14_UART_TF_BMSK                                                                0xffffffff
#define HWIO_MSS_UIM0_UART_DM_TF_14_UART_TF_SHFT                                                                       0x0

#define HWIO_MSS_UIM0_UART_DM_TF_15_ADDR                                                                        (MSS_UIM0_UART_DM_REG_BASE      + 0x00000138)
#define HWIO_MSS_UIM0_UART_DM_TF_15_RMSK                                                                        0xffffffff
#define HWIO_MSS_UIM0_UART_DM_TF_15_OUT(v)      \
        out_dword(HWIO_MSS_UIM0_UART_DM_TF_15_ADDR,v)
#define HWIO_MSS_UIM0_UART_DM_TF_15_UART_TF_BMSK                                                                0xffffffff
#define HWIO_MSS_UIM0_UART_DM_TF_15_UART_TF_SHFT                                                                       0x0

#define HWIO_MSS_UIM0_UART_DM_TF_16_ADDR                                                                        (MSS_UIM0_UART_DM_REG_BASE      + 0x0000013c)
#define HWIO_MSS_UIM0_UART_DM_TF_16_RMSK                                                                        0xffffffff
#define HWIO_MSS_UIM0_UART_DM_TF_16_OUT(v)      \
        out_dword(HWIO_MSS_UIM0_UART_DM_TF_16_ADDR,v)
#define HWIO_MSS_UIM0_UART_DM_TF_16_UART_TF_BMSK                                                                0xffffffff
#define HWIO_MSS_UIM0_UART_DM_TF_16_UART_TF_SHFT                                                                       0x0

#define HWIO_MSS_UIM0_UART_DM_RF_ADDR                                                                           (MSS_UIM0_UART_DM_REG_BASE      + 0x00000140)
#define HWIO_MSS_UIM0_UART_DM_RF_RMSK                                                                           0xffffffff
#define HWIO_MSS_UIM0_UART_DM_RF_IN          \
        in_dword(HWIO_MSS_UIM0_UART_DM_RF_ADDR)
#define HWIO_MSS_UIM0_UART_DM_RF_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM0_UART_DM_RF_ADDR, m)
#define HWIO_MSS_UIM0_UART_DM_RF_UART_RF_BMSK                                                                   0xffffffff
#define HWIO_MSS_UIM0_UART_DM_RF_UART_RF_SHFT                                                                          0x0

#define HWIO_MSS_UIM0_UART_DM_RF_2_ADDR                                                                         (MSS_UIM0_UART_DM_REG_BASE      + 0x00000144)
#define HWIO_MSS_UIM0_UART_DM_RF_2_RMSK                                                                         0xffffffff
#define HWIO_MSS_UIM0_UART_DM_RF_2_IN          \
        in_dword(HWIO_MSS_UIM0_UART_DM_RF_2_ADDR)
#define HWIO_MSS_UIM0_UART_DM_RF_2_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM0_UART_DM_RF_2_ADDR, m)
#define HWIO_MSS_UIM0_UART_DM_RF_2_UART_RF_BMSK                                                                 0xffffffff
#define HWIO_MSS_UIM0_UART_DM_RF_2_UART_RF_SHFT                                                                        0x0

#define HWIO_MSS_UIM0_UART_DM_RF_3_ADDR                                                                         (MSS_UIM0_UART_DM_REG_BASE      + 0x00000148)
#define HWIO_MSS_UIM0_UART_DM_RF_3_RMSK                                                                         0xffffffff
#define HWIO_MSS_UIM0_UART_DM_RF_3_IN          \
        in_dword(HWIO_MSS_UIM0_UART_DM_RF_3_ADDR)
#define HWIO_MSS_UIM0_UART_DM_RF_3_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM0_UART_DM_RF_3_ADDR, m)
#define HWIO_MSS_UIM0_UART_DM_RF_3_UART_RF_BMSK                                                                 0xffffffff
#define HWIO_MSS_UIM0_UART_DM_RF_3_UART_RF_SHFT                                                                        0x0

#define HWIO_MSS_UIM0_UART_DM_RF_4_ADDR                                                                         (MSS_UIM0_UART_DM_REG_BASE      + 0x0000014c)
#define HWIO_MSS_UIM0_UART_DM_RF_4_RMSK                                                                         0xffffffff
#define HWIO_MSS_UIM0_UART_DM_RF_4_IN          \
        in_dword(HWIO_MSS_UIM0_UART_DM_RF_4_ADDR)
#define HWIO_MSS_UIM0_UART_DM_RF_4_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM0_UART_DM_RF_4_ADDR, m)
#define HWIO_MSS_UIM0_UART_DM_RF_4_UART_RF_BMSK                                                                 0xffffffff
#define HWIO_MSS_UIM0_UART_DM_RF_4_UART_RF_SHFT                                                                        0x0

#define HWIO_MSS_UIM0_UART_DM_RF_5_ADDR                                                                         (MSS_UIM0_UART_DM_REG_BASE      + 0x00000150)
#define HWIO_MSS_UIM0_UART_DM_RF_5_RMSK                                                                         0xffffffff
#define HWIO_MSS_UIM0_UART_DM_RF_5_IN          \
        in_dword(HWIO_MSS_UIM0_UART_DM_RF_5_ADDR)
#define HWIO_MSS_UIM0_UART_DM_RF_5_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM0_UART_DM_RF_5_ADDR, m)
#define HWIO_MSS_UIM0_UART_DM_RF_5_UART_RF_BMSK                                                                 0xffffffff
#define HWIO_MSS_UIM0_UART_DM_RF_5_UART_RF_SHFT                                                                        0x0

#define HWIO_MSS_UIM0_UART_DM_RF_6_ADDR                                                                         (MSS_UIM0_UART_DM_REG_BASE      + 0x00000154)
#define HWIO_MSS_UIM0_UART_DM_RF_6_RMSK                                                                         0xffffffff
#define HWIO_MSS_UIM0_UART_DM_RF_6_IN          \
        in_dword(HWIO_MSS_UIM0_UART_DM_RF_6_ADDR)
#define HWIO_MSS_UIM0_UART_DM_RF_6_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM0_UART_DM_RF_6_ADDR, m)
#define HWIO_MSS_UIM0_UART_DM_RF_6_UART_RF_BMSK                                                                 0xffffffff
#define HWIO_MSS_UIM0_UART_DM_RF_6_UART_RF_SHFT                                                                        0x0

#define HWIO_MSS_UIM0_UART_DM_RF_7_ADDR                                                                         (MSS_UIM0_UART_DM_REG_BASE      + 0x00000158)
#define HWIO_MSS_UIM0_UART_DM_RF_7_RMSK                                                                         0xffffffff
#define HWIO_MSS_UIM0_UART_DM_RF_7_IN          \
        in_dword(HWIO_MSS_UIM0_UART_DM_RF_7_ADDR)
#define HWIO_MSS_UIM0_UART_DM_RF_7_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM0_UART_DM_RF_7_ADDR, m)
#define HWIO_MSS_UIM0_UART_DM_RF_7_UART_RF_BMSK                                                                 0xffffffff
#define HWIO_MSS_UIM0_UART_DM_RF_7_UART_RF_SHFT                                                                        0x0

#define HWIO_MSS_UIM0_UART_DM_RF_8_ADDR                                                                         (MSS_UIM0_UART_DM_REG_BASE      + 0x0000015c)
#define HWIO_MSS_UIM0_UART_DM_RF_8_RMSK                                                                         0xffffffff
#define HWIO_MSS_UIM0_UART_DM_RF_8_IN          \
        in_dword(HWIO_MSS_UIM0_UART_DM_RF_8_ADDR)
#define HWIO_MSS_UIM0_UART_DM_RF_8_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM0_UART_DM_RF_8_ADDR, m)
#define HWIO_MSS_UIM0_UART_DM_RF_8_UART_RF_BMSK                                                                 0xffffffff
#define HWIO_MSS_UIM0_UART_DM_RF_8_UART_RF_SHFT                                                                        0x0

#define HWIO_MSS_UIM0_UART_DM_RF_9_ADDR                                                                         (MSS_UIM0_UART_DM_REG_BASE      + 0x00000160)
#define HWIO_MSS_UIM0_UART_DM_RF_9_RMSK                                                                         0xffffffff
#define HWIO_MSS_UIM0_UART_DM_RF_9_IN          \
        in_dword(HWIO_MSS_UIM0_UART_DM_RF_9_ADDR)
#define HWIO_MSS_UIM0_UART_DM_RF_9_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM0_UART_DM_RF_9_ADDR, m)
#define HWIO_MSS_UIM0_UART_DM_RF_9_UART_RF_BMSK                                                                 0xffffffff
#define HWIO_MSS_UIM0_UART_DM_RF_9_UART_RF_SHFT                                                                        0x0

#define HWIO_MSS_UIM0_UART_DM_RF_10_ADDR                                                                        (MSS_UIM0_UART_DM_REG_BASE      + 0x00000164)
#define HWIO_MSS_UIM0_UART_DM_RF_10_RMSK                                                                        0xffffffff
#define HWIO_MSS_UIM0_UART_DM_RF_10_IN          \
        in_dword(HWIO_MSS_UIM0_UART_DM_RF_10_ADDR)
#define HWIO_MSS_UIM0_UART_DM_RF_10_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM0_UART_DM_RF_10_ADDR, m)
#define HWIO_MSS_UIM0_UART_DM_RF_10_UART_RF_BMSK                                                                0xffffffff
#define HWIO_MSS_UIM0_UART_DM_RF_10_UART_RF_SHFT                                                                       0x0

#define HWIO_MSS_UIM0_UART_DM_RF_11_ADDR                                                                        (MSS_UIM0_UART_DM_REG_BASE      + 0x00000168)
#define HWIO_MSS_UIM0_UART_DM_RF_11_RMSK                                                                        0xffffffff
#define HWIO_MSS_UIM0_UART_DM_RF_11_IN          \
        in_dword(HWIO_MSS_UIM0_UART_DM_RF_11_ADDR)
#define HWIO_MSS_UIM0_UART_DM_RF_11_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM0_UART_DM_RF_11_ADDR, m)
#define HWIO_MSS_UIM0_UART_DM_RF_11_UART_RF_BMSK                                                                0xffffffff
#define HWIO_MSS_UIM0_UART_DM_RF_11_UART_RF_SHFT                                                                       0x0

#define HWIO_MSS_UIM0_UART_DM_RF_12_ADDR                                                                        (MSS_UIM0_UART_DM_REG_BASE      + 0x0000016c)
#define HWIO_MSS_UIM0_UART_DM_RF_12_RMSK                                                                        0xffffffff
#define HWIO_MSS_UIM0_UART_DM_RF_12_IN          \
        in_dword(HWIO_MSS_UIM0_UART_DM_RF_12_ADDR)
#define HWIO_MSS_UIM0_UART_DM_RF_12_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM0_UART_DM_RF_12_ADDR, m)
#define HWIO_MSS_UIM0_UART_DM_RF_12_UART_RF_BMSK                                                                0xffffffff
#define HWIO_MSS_UIM0_UART_DM_RF_12_UART_RF_SHFT                                                                       0x0

#define HWIO_MSS_UIM0_UART_DM_RF_13_ADDR                                                                        (MSS_UIM0_UART_DM_REG_BASE      + 0x00000170)
#define HWIO_MSS_UIM0_UART_DM_RF_13_RMSK                                                                        0xffffffff
#define HWIO_MSS_UIM0_UART_DM_RF_13_IN          \
        in_dword(HWIO_MSS_UIM0_UART_DM_RF_13_ADDR)
#define HWIO_MSS_UIM0_UART_DM_RF_13_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM0_UART_DM_RF_13_ADDR, m)
#define HWIO_MSS_UIM0_UART_DM_RF_13_UART_RF_BMSK                                                                0xffffffff
#define HWIO_MSS_UIM0_UART_DM_RF_13_UART_RF_SHFT                                                                       0x0

#define HWIO_MSS_UIM0_UART_DM_RF_14_ADDR                                                                        (MSS_UIM0_UART_DM_REG_BASE      + 0x00000174)
#define HWIO_MSS_UIM0_UART_DM_RF_14_RMSK                                                                        0xffffffff
#define HWIO_MSS_UIM0_UART_DM_RF_14_IN          \
        in_dword(HWIO_MSS_UIM0_UART_DM_RF_14_ADDR)
#define HWIO_MSS_UIM0_UART_DM_RF_14_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM0_UART_DM_RF_14_ADDR, m)
#define HWIO_MSS_UIM0_UART_DM_RF_14_UART_RF_BMSK                                                                0xffffffff
#define HWIO_MSS_UIM0_UART_DM_RF_14_UART_RF_SHFT                                                                       0x0

#define HWIO_MSS_UIM0_UART_DM_RF_15_ADDR                                                                        (MSS_UIM0_UART_DM_REG_BASE      + 0x00000178)
#define HWIO_MSS_UIM0_UART_DM_RF_15_RMSK                                                                        0xffffffff
#define HWIO_MSS_UIM0_UART_DM_RF_15_IN          \
        in_dword(HWIO_MSS_UIM0_UART_DM_RF_15_ADDR)
#define HWIO_MSS_UIM0_UART_DM_RF_15_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM0_UART_DM_RF_15_ADDR, m)
#define HWIO_MSS_UIM0_UART_DM_RF_15_UART_RF_BMSK                                                                0xffffffff
#define HWIO_MSS_UIM0_UART_DM_RF_15_UART_RF_SHFT                                                                       0x0

#define HWIO_MSS_UIM0_UART_DM_RF_16_ADDR                                                                        (MSS_UIM0_UART_DM_REG_BASE      + 0x0000017c)
#define HWIO_MSS_UIM0_UART_DM_RF_16_RMSK                                                                        0xffffffff
#define HWIO_MSS_UIM0_UART_DM_RF_16_IN          \
        in_dword(HWIO_MSS_UIM0_UART_DM_RF_16_ADDR)
#define HWIO_MSS_UIM0_UART_DM_RF_16_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM0_UART_DM_RF_16_ADDR, m)
#define HWIO_MSS_UIM0_UART_DM_RF_16_UART_RF_BMSK                                                                0xffffffff
#define HWIO_MSS_UIM0_UART_DM_RF_16_UART_RF_SHFT                                                                       0x0

#define HWIO_MSS_UIM0_UART_DM_UIM_CFG_ADDR                                                                      (MSS_UIM0_UART_DM_REG_BASE      + 0x00000180)
#define HWIO_MSS_UIM0_UART_DM_UIM_CFG_RMSK                                                                          0xf77f
#define HWIO_MSS_UIM0_UART_DM_UIM_CFG_IN          \
        in_dword(HWIO_MSS_UIM0_UART_DM_UIM_CFG_ADDR)
#define HWIO_MSS_UIM0_UART_DM_UIM_CFG_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM0_UART_DM_UIM_CFG_ADDR, m)
#define HWIO_MSS_UIM0_UART_DM_UIM_CFG_OUT(v)      \
        out_dword(HWIO_MSS_UIM0_UART_DM_UIM_CFG_ADDR,v)
#define HWIO_MSS_UIM0_UART_DM_UIM_CFG_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_UIM0_UART_DM_UIM_CFG_ADDR,m,v,HWIO_MSS_UIM0_UART_DM_UIM_CFG_IN)
#define HWIO_MSS_UIM0_UART_DM_UIM_CFG_BATT_ALARM_QUICK_DROP_EN_BMSK                                                 0x8000
#define HWIO_MSS_UIM0_UART_DM_UIM_CFG_BATT_ALARM_QUICK_DROP_EN_SHFT                                                    0xf
#define HWIO_MSS_UIM0_UART_DM_UIM_CFG_TESTBUS_EN_BMSK                                                               0x4000
#define HWIO_MSS_UIM0_UART_DM_UIM_CFG_TESTBUS_EN_SHFT                                                                  0xe
#define HWIO_MSS_UIM0_UART_DM_UIM_CFG_SW_RESET_BMSK                                                                 0x2000
#define HWIO_MSS_UIM0_UART_DM_UIM_CFG_SW_RESET_SHFT                                                                    0xd
#define HWIO_MSS_UIM0_UART_DM_UIM_CFG_MODE18_BMSK                                                                   0x1000
#define HWIO_MSS_UIM0_UART_DM_UIM_CFG_MODE18_SHFT                                                                      0xc
#define HWIO_MSS_UIM0_UART_DM_UIM_CFG_PMIC_ALARM_EN_BMSK                                                             0x400
#define HWIO_MSS_UIM0_UART_DM_UIM_CFG_PMIC_ALARM_EN_SHFT                                                               0xa
#define HWIO_MSS_UIM0_UART_DM_UIM_CFG_BATT_ALARM_TRIGGER_EN_BMSK                                                     0x200
#define HWIO_MSS_UIM0_UART_DM_UIM_CFG_BATT_ALARM_TRIGGER_EN_SHFT                                                       0x9
#define HWIO_MSS_UIM0_UART_DM_UIM_CFG_UIM_RMV_TRIGGER_EN_BMSK                                                        0x100
#define HWIO_MSS_UIM0_UART_DM_UIM_CFG_UIM_RMV_TRIGGER_EN_SHFT                                                          0x8
#define HWIO_MSS_UIM0_UART_DM_UIM_CFG_UIM_CARD_EVENTS_ENABLE_BMSK                                                     0x40
#define HWIO_MSS_UIM0_UART_DM_UIM_CFG_UIM_CARD_EVENTS_ENABLE_SHFT                                                      0x6
#define HWIO_MSS_UIM0_UART_DM_UIM_CFG_UIM_PRESENT_POLARITY_BMSK                                                       0x20
#define HWIO_MSS_UIM0_UART_DM_UIM_CFG_UIM_PRESENT_POLARITY_SHFT                                                        0x5
#define HWIO_MSS_UIM0_UART_DM_UIM_CFG_EVENT_DEBOUNCE_TIME_BMSK                                                        0x1f
#define HWIO_MSS_UIM0_UART_DM_UIM_CFG_EVENT_DEBOUNCE_TIME_SHFT                                                         0x0

#define HWIO_MSS_UIM0_UART_DM_UIM_CMD_ADDR                                                                      (MSS_UIM0_UART_DM_REG_BASE      + 0x00000184)
#define HWIO_MSS_UIM0_UART_DM_UIM_CMD_RMSK                                                                             0x3
#define HWIO_MSS_UIM0_UART_DM_UIM_CMD_OUT(v)      \
        out_dword(HWIO_MSS_UIM0_UART_DM_UIM_CMD_ADDR,v)
#define HWIO_MSS_UIM0_UART_DM_UIM_CMD_RECOVER_FROM_HW_DEACTIVATION_BMSK                                                0x2
#define HWIO_MSS_UIM0_UART_DM_UIM_CMD_RECOVER_FROM_HW_DEACTIVATION_SHFT                                                0x1
#define HWIO_MSS_UIM0_UART_DM_UIM_CMD_INITIATE_HW_DEACTIVATION_BMSK                                                    0x1
#define HWIO_MSS_UIM0_UART_DM_UIM_CMD_INITIATE_HW_DEACTIVATION_SHFT                                                    0x0

#define HWIO_MSS_UIM0_UART_DM_UIM_IO_STATUS_ADDR                                                                (MSS_UIM0_UART_DM_REG_BASE      + 0x00000188)
#define HWIO_MSS_UIM0_UART_DM_UIM_IO_STATUS_RMSK                                                                       0x7
#define HWIO_MSS_UIM0_UART_DM_UIM_IO_STATUS_IN          \
        in_dword(HWIO_MSS_UIM0_UART_DM_UIM_IO_STATUS_ADDR)
#define HWIO_MSS_UIM0_UART_DM_UIM_IO_STATUS_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM0_UART_DM_UIM_IO_STATUS_ADDR, m)
#define HWIO_MSS_UIM0_UART_DM_UIM_IO_STATUS_UIM_IO_WRITE_IN_PROGRESS_BMSK                                              0x4
#define HWIO_MSS_UIM0_UART_DM_UIM_IO_STATUS_UIM_IO_WRITE_IN_PROGRESS_SHFT                                              0x2
#define HWIO_MSS_UIM0_UART_DM_UIM_IO_STATUS_UIM_DEACTIVATION_STATUS_BMSK                                               0x2
#define HWIO_MSS_UIM0_UART_DM_UIM_IO_STATUS_UIM_DEACTIVATION_STATUS_SHFT                                               0x1
#define HWIO_MSS_UIM0_UART_DM_UIM_IO_STATUS_CARD_PRESENCE_BMSK                                                         0x1
#define HWIO_MSS_UIM0_UART_DM_UIM_IO_STATUS_CARD_PRESENCE_SHFT                                                         0x0

#define HWIO_MSS_UIM0_UART_DM_UIM_IRQ_ISR_ADDR                                                                  (MSS_UIM0_UART_DM_REG_BASE      + 0x0000018c)
#define HWIO_MSS_UIM0_UART_DM_UIM_IRQ_ISR_RMSK                                                                        0x1f
#define HWIO_MSS_UIM0_UART_DM_UIM_IRQ_ISR_IN          \
        in_dword(HWIO_MSS_UIM0_UART_DM_UIM_IRQ_ISR_ADDR)
#define HWIO_MSS_UIM0_UART_DM_UIM_IRQ_ISR_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM0_UART_DM_UIM_IRQ_ISR_ADDR, m)
#define HWIO_MSS_UIM0_UART_DM_UIM_IRQ_ISR_UIM_IO_WRITE_DONE_BMSK                                                      0x10
#define HWIO_MSS_UIM0_UART_DM_UIM_IRQ_ISR_UIM_IO_WRITE_DONE_SHFT                                                       0x4
#define HWIO_MSS_UIM0_UART_DM_UIM_IRQ_ISR_HW_SEQUENCE_FINISH_BMSK                                                      0x8
#define HWIO_MSS_UIM0_UART_DM_UIM_IRQ_ISR_HW_SEQUENCE_FINISH_SHFT                                                      0x3
#define HWIO_MSS_UIM0_UART_DM_UIM_IRQ_ISR_BATT_ALARM_BMSK                                                              0x4
#define HWIO_MSS_UIM0_UART_DM_UIM_IRQ_ISR_BATT_ALARM_SHFT                                                              0x2
#define HWIO_MSS_UIM0_UART_DM_UIM_IRQ_ISR_UIM_CARD_INSERTION_BMSK                                                      0x2
#define HWIO_MSS_UIM0_UART_DM_UIM_IRQ_ISR_UIM_CARD_INSERTION_SHFT                                                      0x1
#define HWIO_MSS_UIM0_UART_DM_UIM_IRQ_ISR_UIM_CARD_REMOVAL_BMSK                                                        0x1
#define HWIO_MSS_UIM0_UART_DM_UIM_IRQ_ISR_UIM_CARD_REMOVAL_SHFT                                                        0x0

#define HWIO_MSS_UIM0_UART_DM_UIM_IRQ_MISR_ADDR                                                                 (MSS_UIM0_UART_DM_REG_BASE      + 0x00000190)
#define HWIO_MSS_UIM0_UART_DM_UIM_IRQ_MISR_RMSK                                                                       0x1f
#define HWIO_MSS_UIM0_UART_DM_UIM_IRQ_MISR_IN          \
        in_dword(HWIO_MSS_UIM0_UART_DM_UIM_IRQ_MISR_ADDR)
#define HWIO_MSS_UIM0_UART_DM_UIM_IRQ_MISR_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM0_UART_DM_UIM_IRQ_MISR_ADDR, m)
#define HWIO_MSS_UIM0_UART_DM_UIM_IRQ_MISR_UIM_IO_WRITE_DONE_BMSK                                                     0x10
#define HWIO_MSS_UIM0_UART_DM_UIM_IRQ_MISR_UIM_IO_WRITE_DONE_SHFT                                                      0x4
#define HWIO_MSS_UIM0_UART_DM_UIM_IRQ_MISR_HW_SEQUENCE_FINISH_BMSK                                                     0x8
#define HWIO_MSS_UIM0_UART_DM_UIM_IRQ_MISR_HW_SEQUENCE_FINISH_SHFT                                                     0x3
#define HWIO_MSS_UIM0_UART_DM_UIM_IRQ_MISR_BATT_ALARM_BMSK                                                             0x4
#define HWIO_MSS_UIM0_UART_DM_UIM_IRQ_MISR_BATT_ALARM_SHFT                                                             0x2
#define HWIO_MSS_UIM0_UART_DM_UIM_IRQ_MISR_UIM_CARD_INSERTION_BMSK                                                     0x2
#define HWIO_MSS_UIM0_UART_DM_UIM_IRQ_MISR_UIM_CARD_INSERTION_SHFT                                                     0x1
#define HWIO_MSS_UIM0_UART_DM_UIM_IRQ_MISR_UIM_CARD_REMOVAL_BMSK                                                       0x1
#define HWIO_MSS_UIM0_UART_DM_UIM_IRQ_MISR_UIM_CARD_REMOVAL_SHFT                                                       0x0

#define HWIO_MSS_UIM0_UART_DM_UIM_IRQ_CLR_ADDR                                                                  (MSS_UIM0_UART_DM_REG_BASE      + 0x00000194)
#define HWIO_MSS_UIM0_UART_DM_UIM_IRQ_CLR_RMSK                                                                        0x1f
#define HWIO_MSS_UIM0_UART_DM_UIM_IRQ_CLR_OUT(v)      \
        out_dword(HWIO_MSS_UIM0_UART_DM_UIM_IRQ_CLR_ADDR,v)
#define HWIO_MSS_UIM0_UART_DM_UIM_IRQ_CLR_UIM_IO_WRITE_DONE_BMSK                                                      0x10
#define HWIO_MSS_UIM0_UART_DM_UIM_IRQ_CLR_UIM_IO_WRITE_DONE_SHFT                                                       0x4
#define HWIO_MSS_UIM0_UART_DM_UIM_IRQ_CLR_HW_SEQUENCE_FINISH_BMSK                                                      0x8
#define HWIO_MSS_UIM0_UART_DM_UIM_IRQ_CLR_HW_SEQUENCE_FINISH_SHFT                                                      0x3
#define HWIO_MSS_UIM0_UART_DM_UIM_IRQ_CLR_BATT_ALARM_BMSK                                                              0x4
#define HWIO_MSS_UIM0_UART_DM_UIM_IRQ_CLR_BATT_ALARM_SHFT                                                              0x2
#define HWIO_MSS_UIM0_UART_DM_UIM_IRQ_CLR_UIM_CARD_INSERTION_BMSK                                                      0x2
#define HWIO_MSS_UIM0_UART_DM_UIM_IRQ_CLR_UIM_CARD_INSERTION_SHFT                                                      0x1
#define HWIO_MSS_UIM0_UART_DM_UIM_IRQ_CLR_UIM_CARD_REMOVAL_BMSK                                                        0x1
#define HWIO_MSS_UIM0_UART_DM_UIM_IRQ_CLR_UIM_CARD_REMOVAL_SHFT                                                        0x0

#define HWIO_MSS_UIM0_UART_DM_UIM_IRQ_IMR_ADDR                                                                  (MSS_UIM0_UART_DM_REG_BASE      + 0x00000198)
#define HWIO_MSS_UIM0_UART_DM_UIM_IRQ_IMR_RMSK                                                                        0x1f
#define HWIO_MSS_UIM0_UART_DM_UIM_IRQ_IMR_IN          \
        in_dword(HWIO_MSS_UIM0_UART_DM_UIM_IRQ_IMR_ADDR)
#define HWIO_MSS_UIM0_UART_DM_UIM_IRQ_IMR_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM0_UART_DM_UIM_IRQ_IMR_ADDR, m)
#define HWIO_MSS_UIM0_UART_DM_UIM_IRQ_IMR_OUT(v)      \
        out_dword(HWIO_MSS_UIM0_UART_DM_UIM_IRQ_IMR_ADDR,v)
#define HWIO_MSS_UIM0_UART_DM_UIM_IRQ_IMR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_UIM0_UART_DM_UIM_IRQ_IMR_ADDR,m,v,HWIO_MSS_UIM0_UART_DM_UIM_IRQ_IMR_IN)
#define HWIO_MSS_UIM0_UART_DM_UIM_IRQ_IMR_UIM_IO_WRITE_DONE_BMSK                                                      0x10
#define HWIO_MSS_UIM0_UART_DM_UIM_IRQ_IMR_UIM_IO_WRITE_DONE_SHFT                                                       0x4
#define HWIO_MSS_UIM0_UART_DM_UIM_IRQ_IMR_HW_SEQUENCE_FINISH_BMSK                                                      0x8
#define HWIO_MSS_UIM0_UART_DM_UIM_IRQ_IMR_HW_SEQUENCE_FINISH_SHFT                                                      0x3
#define HWIO_MSS_UIM0_UART_DM_UIM_IRQ_IMR_BATT_ALARM_BMSK                                                              0x4
#define HWIO_MSS_UIM0_UART_DM_UIM_IRQ_IMR_BATT_ALARM_SHFT                                                              0x2
#define HWIO_MSS_UIM0_UART_DM_UIM_IRQ_IMR_UIM_CARD_INSERTION_BMSK                                                      0x2
#define HWIO_MSS_UIM0_UART_DM_UIM_IRQ_IMR_UIM_CARD_INSERTION_SHFT                                                      0x1
#define HWIO_MSS_UIM0_UART_DM_UIM_IRQ_IMR_UIM_CARD_REMOVAL_BMSK                                                        0x1
#define HWIO_MSS_UIM0_UART_DM_UIM_IRQ_IMR_UIM_CARD_REMOVAL_SHFT                                                        0x0

#define HWIO_MSS_UIM0_UART_DM_UIM_IRQ_IMR_SET_ADDR                                                              (MSS_UIM0_UART_DM_REG_BASE      + 0x0000019c)
#define HWIO_MSS_UIM0_UART_DM_UIM_IRQ_IMR_SET_RMSK                                                                    0x1f
#define HWIO_MSS_UIM0_UART_DM_UIM_IRQ_IMR_SET_OUT(v)      \
        out_dword(HWIO_MSS_UIM0_UART_DM_UIM_IRQ_IMR_SET_ADDR,v)
#define HWIO_MSS_UIM0_UART_DM_UIM_IRQ_IMR_SET_UIM_IO_WRITE_DONE_BMSK                                                  0x10
#define HWIO_MSS_UIM0_UART_DM_UIM_IRQ_IMR_SET_UIM_IO_WRITE_DONE_SHFT                                                   0x4
#define HWIO_MSS_UIM0_UART_DM_UIM_IRQ_IMR_SET_HW_SEQUENCE_FINISH_BMSK                                                  0x8
#define HWIO_MSS_UIM0_UART_DM_UIM_IRQ_IMR_SET_HW_SEQUENCE_FINISH_SHFT                                                  0x3
#define HWIO_MSS_UIM0_UART_DM_UIM_IRQ_IMR_SET_BATT_ALARM_BMSK                                                          0x4
#define HWIO_MSS_UIM0_UART_DM_UIM_IRQ_IMR_SET_BATT_ALARM_SHFT                                                          0x2
#define HWIO_MSS_UIM0_UART_DM_UIM_IRQ_IMR_SET_UIM_CARD_INSERTION_BMSK                                                  0x2
#define HWIO_MSS_UIM0_UART_DM_UIM_IRQ_IMR_SET_UIM_CARD_INSERTION_SHFT                                                  0x1
#define HWIO_MSS_UIM0_UART_DM_UIM_IRQ_IMR_SET_UIM_CARD_REMOVAL_BMSK                                                    0x1
#define HWIO_MSS_UIM0_UART_DM_UIM_IRQ_IMR_SET_UIM_CARD_REMOVAL_SHFT                                                    0x0

#define HWIO_MSS_UIM0_UART_DM_UIM_IRQ_IMR_CLR_ADDR                                                              (MSS_UIM0_UART_DM_REG_BASE      + 0x000001a0)
#define HWIO_MSS_UIM0_UART_DM_UIM_IRQ_IMR_CLR_RMSK                                                                    0x1f
#define HWIO_MSS_UIM0_UART_DM_UIM_IRQ_IMR_CLR_OUT(v)      \
        out_dword(HWIO_MSS_UIM0_UART_DM_UIM_IRQ_IMR_CLR_ADDR,v)
#define HWIO_MSS_UIM0_UART_DM_UIM_IRQ_IMR_CLR_UIM_IO_WRITE_DONE_BMSK                                                  0x10
#define HWIO_MSS_UIM0_UART_DM_UIM_IRQ_IMR_CLR_UIM_IO_WRITE_DONE_SHFT                                                   0x4
#define HWIO_MSS_UIM0_UART_DM_UIM_IRQ_IMR_CLR_HW_SEQUENCE_FINISH_BMSK                                                  0x8
#define HWIO_MSS_UIM0_UART_DM_UIM_IRQ_IMR_CLR_HW_SEQUENCE_FINISH_SHFT                                                  0x3
#define HWIO_MSS_UIM0_UART_DM_UIM_IRQ_IMR_CLR_BATT_ALARM_BMSK                                                          0x4
#define HWIO_MSS_UIM0_UART_DM_UIM_IRQ_IMR_CLR_BATT_ALARM_SHFT                                                          0x2
#define HWIO_MSS_UIM0_UART_DM_UIM_IRQ_IMR_CLR_UIM_CARD_INSERTION_BMSK                                                  0x2
#define HWIO_MSS_UIM0_UART_DM_UIM_IRQ_IMR_CLR_UIM_CARD_INSERTION_SHFT                                                  0x1
#define HWIO_MSS_UIM0_UART_DM_UIM_IRQ_IMR_CLR_UIM_CARD_REMOVAL_BMSK                                                    0x1
#define HWIO_MSS_UIM0_UART_DM_UIM_IRQ_IMR_CLR_UIM_CARD_REMOVAL_SHFT                                                    0x0

/*----------------------------------------------------------------------------
 * MODULE: MSS_UIM1_UART_DM
 *--------------------------------------------------------------------------*/

#define MSS_UIM1_UART_DM_REG_BASE                                                                               (MSS_TOP_BASE      + 0x00198000)
#define MSS_UIM1_UART_DM_REG_BASE_SIZE                                                                          0x200
#define MSS_UIM1_UART_DM_REG_BASE_USED                                                                          0x1a0

#define HWIO_MSS_UIM1_UART_DM_MR1_ADDR                                                                          (MSS_UIM1_UART_DM_REG_BASE      + 0x00000000)
#define HWIO_MSS_UIM1_UART_DM_MR1_RMSK                                                                          0xffffffff
#define HWIO_MSS_UIM1_UART_DM_MR1_IN          \
        in_dword(HWIO_MSS_UIM1_UART_DM_MR1_ADDR)
#define HWIO_MSS_UIM1_UART_DM_MR1_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM1_UART_DM_MR1_ADDR, m)
#define HWIO_MSS_UIM1_UART_DM_MR1_OUT(v)      \
        out_dword(HWIO_MSS_UIM1_UART_DM_MR1_ADDR,v)
#define HWIO_MSS_UIM1_UART_DM_MR1_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_UIM1_UART_DM_MR1_ADDR,m,v,HWIO_MSS_UIM1_UART_DM_MR1_IN)
#define HWIO_MSS_UIM1_UART_DM_MR1_AUTO_RFR_LEVEL1_BMSK                                                          0xffffff00
#define HWIO_MSS_UIM1_UART_DM_MR1_AUTO_RFR_LEVEL1_SHFT                                                                 0x8
#define HWIO_MSS_UIM1_UART_DM_MR1_RX_RDY_CTL_BMSK                                                                     0x80
#define HWIO_MSS_UIM1_UART_DM_MR1_RX_RDY_CTL_SHFT                                                                      0x7
#define HWIO_MSS_UIM1_UART_DM_MR1_CTS_CTL_BMSK                                                                        0x40
#define HWIO_MSS_UIM1_UART_DM_MR1_CTS_CTL_SHFT                                                                         0x6
#define HWIO_MSS_UIM1_UART_DM_MR1_AUTO_RFR_LEVEL0_BMSK                                                                0x3f
#define HWIO_MSS_UIM1_UART_DM_MR1_AUTO_RFR_LEVEL0_SHFT                                                                 0x0

#define HWIO_MSS_UIM1_UART_DM_MR2_ADDR                                                                          (MSS_UIM1_UART_DM_REG_BASE      + 0x00000004)
#define HWIO_MSS_UIM1_UART_DM_MR2_RMSK                                                                               0x7ff
#define HWIO_MSS_UIM1_UART_DM_MR2_IN          \
        in_dword(HWIO_MSS_UIM1_UART_DM_MR2_ADDR)
#define HWIO_MSS_UIM1_UART_DM_MR2_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM1_UART_DM_MR2_ADDR, m)
#define HWIO_MSS_UIM1_UART_DM_MR2_OUT(v)      \
        out_dword(HWIO_MSS_UIM1_UART_DM_MR2_ADDR,v)
#define HWIO_MSS_UIM1_UART_DM_MR2_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_UIM1_UART_DM_MR2_ADDR,m,v,HWIO_MSS_UIM1_UART_DM_MR2_IN)
#define HWIO_MSS_UIM1_UART_DM_MR2_RFR_CTS_LOOPBACK_BMSK                                                              0x400
#define HWIO_MSS_UIM1_UART_DM_MR2_RFR_CTS_LOOPBACK_SHFT                                                                0xa
#define HWIO_MSS_UIM1_UART_DM_MR2_RX_ERROR_CHAR_OFF_BMSK                                                             0x200
#define HWIO_MSS_UIM1_UART_DM_MR2_RX_ERROR_CHAR_OFF_SHFT                                                               0x9
#define HWIO_MSS_UIM1_UART_DM_MR2_RX_BREAK_ZERO_CHAR_OFF_BMSK                                                        0x100
#define HWIO_MSS_UIM1_UART_DM_MR2_RX_BREAK_ZERO_CHAR_OFF_SHFT                                                          0x8
#define HWIO_MSS_UIM1_UART_DM_MR2_LOOPBACK_BMSK                                                                       0x80
#define HWIO_MSS_UIM1_UART_DM_MR2_LOOPBACK_SHFT                                                                        0x7
#define HWIO_MSS_UIM1_UART_DM_MR2_ERROR_MODE_BMSK                                                                     0x40
#define HWIO_MSS_UIM1_UART_DM_MR2_ERROR_MODE_SHFT                                                                      0x6
#define HWIO_MSS_UIM1_UART_DM_MR2_BITS_PER_CHAR_BMSK                                                                  0x30
#define HWIO_MSS_UIM1_UART_DM_MR2_BITS_PER_CHAR_SHFT                                                                   0x4
#define HWIO_MSS_UIM1_UART_DM_MR2_STOP_BIT_LEN_BMSK                                                                    0xc
#define HWIO_MSS_UIM1_UART_DM_MR2_STOP_BIT_LEN_SHFT                                                                    0x2
#define HWIO_MSS_UIM1_UART_DM_MR2_PARITY_MODE_BMSK                                                                     0x3
#define HWIO_MSS_UIM1_UART_DM_MR2_PARITY_MODE_SHFT                                                                     0x0

#define HWIO_MSS_UIM1_UART_DM_CSR_SR_DEPRECATED_ADDR                                                            (MSS_UIM1_UART_DM_REG_BASE      + 0x00000008)
#define HWIO_MSS_UIM1_UART_DM_CSR_SR_DEPRECATED_RMSK                                                            0xffffffff
#define HWIO_MSS_UIM1_UART_DM_CSR_SR_DEPRECATED_IN          \
        in_dword(HWIO_MSS_UIM1_UART_DM_CSR_SR_DEPRECATED_ADDR)
#define HWIO_MSS_UIM1_UART_DM_CSR_SR_DEPRECATED_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM1_UART_DM_CSR_SR_DEPRECATED_ADDR, m)
#define HWIO_MSS_UIM1_UART_DM_CSR_SR_DEPRECATED_OUT(v)      \
        out_dword(HWIO_MSS_UIM1_UART_DM_CSR_SR_DEPRECATED_ADDR,v)
#define HWIO_MSS_UIM1_UART_DM_CSR_SR_DEPRECATED_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_UIM1_UART_DM_CSR_SR_DEPRECATED_ADDR,m,v,HWIO_MSS_UIM1_UART_DM_CSR_SR_DEPRECATED_IN)
#define HWIO_MSS_UIM1_UART_DM_CSR_SR_DEPRECATED_UART_DM_CSR_SR_DEPRECATED_BMSK                                  0xffffffff
#define HWIO_MSS_UIM1_UART_DM_CSR_SR_DEPRECATED_UART_DM_CSR_SR_DEPRECATED_SHFT                                         0x0

#define HWIO_MSS_UIM1_UART_DM_CR_MISR_DEPRECATED_ADDR                                                           (MSS_UIM1_UART_DM_REG_BASE      + 0x00000010)
#define HWIO_MSS_UIM1_UART_DM_CR_MISR_DEPRECATED_RMSK                                                           0xffffffff
#define HWIO_MSS_UIM1_UART_DM_CR_MISR_DEPRECATED_IN          \
        in_dword(HWIO_MSS_UIM1_UART_DM_CR_MISR_DEPRECATED_ADDR)
#define HWIO_MSS_UIM1_UART_DM_CR_MISR_DEPRECATED_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM1_UART_DM_CR_MISR_DEPRECATED_ADDR, m)
#define HWIO_MSS_UIM1_UART_DM_CR_MISR_DEPRECATED_OUT(v)      \
        out_dword(HWIO_MSS_UIM1_UART_DM_CR_MISR_DEPRECATED_ADDR,v)
#define HWIO_MSS_UIM1_UART_DM_CR_MISR_DEPRECATED_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_UIM1_UART_DM_CR_MISR_DEPRECATED_ADDR,m,v,HWIO_MSS_UIM1_UART_DM_CR_MISR_DEPRECATED_IN)
#define HWIO_MSS_UIM1_UART_DM_CR_MISR_DEPRECATED_UART_DM_CR_MISR_DEPRECATED_BMSK                                0xffffffff
#define HWIO_MSS_UIM1_UART_DM_CR_MISR_DEPRECATED_UART_DM_CR_MISR_DEPRECATED_SHFT                                       0x0

#define HWIO_MSS_UIM1_UART_DM_IMR_ISR_DEPRECATED_ADDR                                                           (MSS_UIM1_UART_DM_REG_BASE      + 0x00000014)
#define HWIO_MSS_UIM1_UART_DM_IMR_ISR_DEPRECATED_RMSK                                                           0xffffffff
#define HWIO_MSS_UIM1_UART_DM_IMR_ISR_DEPRECATED_IN          \
        in_dword(HWIO_MSS_UIM1_UART_DM_IMR_ISR_DEPRECATED_ADDR)
#define HWIO_MSS_UIM1_UART_DM_IMR_ISR_DEPRECATED_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM1_UART_DM_IMR_ISR_DEPRECATED_ADDR, m)
#define HWIO_MSS_UIM1_UART_DM_IMR_ISR_DEPRECATED_OUT(v)      \
        out_dword(HWIO_MSS_UIM1_UART_DM_IMR_ISR_DEPRECATED_ADDR,v)
#define HWIO_MSS_UIM1_UART_DM_IMR_ISR_DEPRECATED_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_UIM1_UART_DM_IMR_ISR_DEPRECATED_ADDR,m,v,HWIO_MSS_UIM1_UART_DM_IMR_ISR_DEPRECATED_IN)
#define HWIO_MSS_UIM1_UART_DM_IMR_ISR_DEPRECATED_UART_DM_IMR_ISR_DEPRECATED_BMSK                                0xffffffff
#define HWIO_MSS_UIM1_UART_DM_IMR_ISR_DEPRECATED_UART_DM_IMR_ISR_DEPRECATED_SHFT                                       0x0

#define HWIO_MSS_UIM1_UART_DM_IPR_ADDR                                                                          (MSS_UIM1_UART_DM_REG_BASE      + 0x00000018)
#define HWIO_MSS_UIM1_UART_DM_IPR_RMSK                                                                          0xffffffdf
#define HWIO_MSS_UIM1_UART_DM_IPR_IN          \
        in_dword(HWIO_MSS_UIM1_UART_DM_IPR_ADDR)
#define HWIO_MSS_UIM1_UART_DM_IPR_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM1_UART_DM_IPR_ADDR, m)
#define HWIO_MSS_UIM1_UART_DM_IPR_OUT(v)      \
        out_dword(HWIO_MSS_UIM1_UART_DM_IPR_ADDR,v)
#define HWIO_MSS_UIM1_UART_DM_IPR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_UIM1_UART_DM_IPR_ADDR,m,v,HWIO_MSS_UIM1_UART_DM_IPR_IN)
#define HWIO_MSS_UIM1_UART_DM_IPR_STALE_TIMEOUT_MSB_BMSK                                                        0xffffff80
#define HWIO_MSS_UIM1_UART_DM_IPR_STALE_TIMEOUT_MSB_SHFT                                                               0x7
#define HWIO_MSS_UIM1_UART_DM_IPR_SAMPLE_DATA_BMSK                                                                    0x40
#define HWIO_MSS_UIM1_UART_DM_IPR_SAMPLE_DATA_SHFT                                                                     0x6
#define HWIO_MSS_UIM1_UART_DM_IPR_STALE_TIMEOUT_LSB_BMSK                                                              0x1f
#define HWIO_MSS_UIM1_UART_DM_IPR_STALE_TIMEOUT_LSB_SHFT                                                               0x0

#define HWIO_MSS_UIM1_UART_DM_TFWR_ADDR                                                                         (MSS_UIM1_UART_DM_REG_BASE      + 0x0000001c)
#define HWIO_MSS_UIM1_UART_DM_TFWR_RMSK                                                                         0xffffffff
#define HWIO_MSS_UIM1_UART_DM_TFWR_IN          \
        in_dword(HWIO_MSS_UIM1_UART_DM_TFWR_ADDR)
#define HWIO_MSS_UIM1_UART_DM_TFWR_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM1_UART_DM_TFWR_ADDR, m)
#define HWIO_MSS_UIM1_UART_DM_TFWR_OUT(v)      \
        out_dword(HWIO_MSS_UIM1_UART_DM_TFWR_ADDR,v)
#define HWIO_MSS_UIM1_UART_DM_TFWR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_UIM1_UART_DM_TFWR_ADDR,m,v,HWIO_MSS_UIM1_UART_DM_TFWR_IN)
#define HWIO_MSS_UIM1_UART_DM_TFWR_TFW_BMSK                                                                     0xffffffff
#define HWIO_MSS_UIM1_UART_DM_TFWR_TFW_SHFT                                                                            0x0

#define HWIO_MSS_UIM1_UART_DM_RFWR_ADDR                                                                         (MSS_UIM1_UART_DM_REG_BASE      + 0x00000020)
#define HWIO_MSS_UIM1_UART_DM_RFWR_RMSK                                                                         0xffffffff
#define HWIO_MSS_UIM1_UART_DM_RFWR_IN          \
        in_dword(HWIO_MSS_UIM1_UART_DM_RFWR_ADDR)
#define HWIO_MSS_UIM1_UART_DM_RFWR_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM1_UART_DM_RFWR_ADDR, m)
#define HWIO_MSS_UIM1_UART_DM_RFWR_OUT(v)      \
        out_dword(HWIO_MSS_UIM1_UART_DM_RFWR_ADDR,v)
#define HWIO_MSS_UIM1_UART_DM_RFWR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_UIM1_UART_DM_RFWR_ADDR,m,v,HWIO_MSS_UIM1_UART_DM_RFWR_IN)
#define HWIO_MSS_UIM1_UART_DM_RFWR_RFW_BMSK                                                                     0xffffffff
#define HWIO_MSS_UIM1_UART_DM_RFWR_RFW_SHFT                                                                            0x0

#define HWIO_MSS_UIM1_UART_DM_HCR_ADDR                                                                          (MSS_UIM1_UART_DM_REG_BASE      + 0x00000024)
#define HWIO_MSS_UIM1_UART_DM_HCR_RMSK                                                                                0xff
#define HWIO_MSS_UIM1_UART_DM_HCR_IN          \
        in_dword(HWIO_MSS_UIM1_UART_DM_HCR_ADDR)
#define HWIO_MSS_UIM1_UART_DM_HCR_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM1_UART_DM_HCR_ADDR, m)
#define HWIO_MSS_UIM1_UART_DM_HCR_OUT(v)      \
        out_dword(HWIO_MSS_UIM1_UART_DM_HCR_ADDR,v)
#define HWIO_MSS_UIM1_UART_DM_HCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_UIM1_UART_DM_HCR_ADDR,m,v,HWIO_MSS_UIM1_UART_DM_HCR_IN)
#define HWIO_MSS_UIM1_UART_DM_HCR_DATA_BMSK                                                                           0xff
#define HWIO_MSS_UIM1_UART_DM_HCR_DATA_SHFT                                                                            0x0

#define HWIO_MSS_UIM1_UART_DM_DMRX_ADDR                                                                         (MSS_UIM1_UART_DM_REG_BASE      + 0x00000034)
#define HWIO_MSS_UIM1_UART_DM_DMRX_RMSK                                                                          0x1ffffff
#define HWIO_MSS_UIM1_UART_DM_DMRX_IN          \
        in_dword(HWIO_MSS_UIM1_UART_DM_DMRX_ADDR)
#define HWIO_MSS_UIM1_UART_DM_DMRX_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM1_UART_DM_DMRX_ADDR, m)
#define HWIO_MSS_UIM1_UART_DM_DMRX_OUT(v)      \
        out_dword(HWIO_MSS_UIM1_UART_DM_DMRX_ADDR,v)
#define HWIO_MSS_UIM1_UART_DM_DMRX_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_UIM1_UART_DM_DMRX_ADDR,m,v,HWIO_MSS_UIM1_UART_DM_DMRX_IN)
#define HWIO_MSS_UIM1_UART_DM_DMRX_RX_DM_CRCI_CHARS_BMSK                                                         0x1ffffff
#define HWIO_MSS_UIM1_UART_DM_DMRX_RX_DM_CRCI_CHARS_SHFT                                                               0x0

#define HWIO_MSS_UIM1_UART_DM_IRDA_RX_TOTAL_SNAP_DEPRECATED_ADDR                                                (MSS_UIM1_UART_DM_REG_BASE      + 0x00000038)
#define HWIO_MSS_UIM1_UART_DM_IRDA_RX_TOTAL_SNAP_DEPRECATED_RMSK                                                0xffffffff
#define HWIO_MSS_UIM1_UART_DM_IRDA_RX_TOTAL_SNAP_DEPRECATED_IN          \
        in_dword(HWIO_MSS_UIM1_UART_DM_IRDA_RX_TOTAL_SNAP_DEPRECATED_ADDR)
#define HWIO_MSS_UIM1_UART_DM_IRDA_RX_TOTAL_SNAP_DEPRECATED_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM1_UART_DM_IRDA_RX_TOTAL_SNAP_DEPRECATED_ADDR, m)
#define HWIO_MSS_UIM1_UART_DM_IRDA_RX_TOTAL_SNAP_DEPRECATED_OUT(v)      \
        out_dword(HWIO_MSS_UIM1_UART_DM_IRDA_RX_TOTAL_SNAP_DEPRECATED_ADDR,v)
#define HWIO_MSS_UIM1_UART_DM_IRDA_RX_TOTAL_SNAP_DEPRECATED_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_UIM1_UART_DM_IRDA_RX_TOTAL_SNAP_DEPRECATED_ADDR,m,v,HWIO_MSS_UIM1_UART_DM_IRDA_RX_TOTAL_SNAP_DEPRECATED_IN)
#define HWIO_MSS_UIM1_UART_DM_IRDA_RX_TOTAL_SNAP_DEPRECATED_UART_DM_IRDA_RX_TOTAL_SNAP_DEPRECATED_BMSK          0xffffffff
#define HWIO_MSS_UIM1_UART_DM_IRDA_RX_TOTAL_SNAP_DEPRECATED_UART_DM_IRDA_RX_TOTAL_SNAP_DEPRECATED_SHFT                 0x0

#define HWIO_MSS_UIM1_UART_DM_DMEN_ADDR                                                                         (MSS_UIM1_UART_DM_REG_BASE      + 0x0000003c)
#define HWIO_MSS_UIM1_UART_DM_DMEN_RMSK                                                                               0x3c
#define HWIO_MSS_UIM1_UART_DM_DMEN_IN          \
        in_dword(HWIO_MSS_UIM1_UART_DM_DMEN_ADDR)
#define HWIO_MSS_UIM1_UART_DM_DMEN_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM1_UART_DM_DMEN_ADDR, m)
#define HWIO_MSS_UIM1_UART_DM_DMEN_OUT(v)      \
        out_dword(HWIO_MSS_UIM1_UART_DM_DMEN_ADDR,v)
#define HWIO_MSS_UIM1_UART_DM_DMEN_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_UIM1_UART_DM_DMEN_ADDR,m,v,HWIO_MSS_UIM1_UART_DM_DMEN_IN)
#define HWIO_MSS_UIM1_UART_DM_DMEN_RX_SC_ENABLE_BMSK                                                                  0x20
#define HWIO_MSS_UIM1_UART_DM_DMEN_RX_SC_ENABLE_SHFT                                                                   0x5
#define HWIO_MSS_UIM1_UART_DM_DMEN_TX_SC_ENABLE_BMSK                                                                  0x10
#define HWIO_MSS_UIM1_UART_DM_DMEN_TX_SC_ENABLE_SHFT                                                                   0x4
#define HWIO_MSS_UIM1_UART_DM_DMEN_RX_BAM_ENABLE_BMSK                                                                  0x8
#define HWIO_MSS_UIM1_UART_DM_DMEN_RX_BAM_ENABLE_SHFT                                                                  0x3
#define HWIO_MSS_UIM1_UART_DM_DMEN_TX_BAM_ENABLE_BMSK                                                                  0x4
#define HWIO_MSS_UIM1_UART_DM_DMEN_TX_BAM_ENABLE_SHFT                                                                  0x2

#define HWIO_MSS_UIM1_UART_DM_NO_CHARS_FOR_TX_ADDR                                                              (MSS_UIM1_UART_DM_REG_BASE      + 0x00000040)
#define HWIO_MSS_UIM1_UART_DM_NO_CHARS_FOR_TX_RMSK                                                                0xffffff
#define HWIO_MSS_UIM1_UART_DM_NO_CHARS_FOR_TX_IN          \
        in_dword(HWIO_MSS_UIM1_UART_DM_NO_CHARS_FOR_TX_ADDR)
#define HWIO_MSS_UIM1_UART_DM_NO_CHARS_FOR_TX_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM1_UART_DM_NO_CHARS_FOR_TX_ADDR, m)
#define HWIO_MSS_UIM1_UART_DM_NO_CHARS_FOR_TX_OUT(v)      \
        out_dword(HWIO_MSS_UIM1_UART_DM_NO_CHARS_FOR_TX_ADDR,v)
#define HWIO_MSS_UIM1_UART_DM_NO_CHARS_FOR_TX_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_UIM1_UART_DM_NO_CHARS_FOR_TX_ADDR,m,v,HWIO_MSS_UIM1_UART_DM_NO_CHARS_FOR_TX_IN)
#define HWIO_MSS_UIM1_UART_DM_NO_CHARS_FOR_TX_TX_TOTAL_TRANS_LEN_BMSK                                             0xffffff
#define HWIO_MSS_UIM1_UART_DM_NO_CHARS_FOR_TX_TX_TOTAL_TRANS_LEN_SHFT                                                  0x0

#define HWIO_MSS_UIM1_UART_DM_BADR_ADDR                                                                         (MSS_UIM1_UART_DM_REG_BASE      + 0x00000044)
#define HWIO_MSS_UIM1_UART_DM_BADR_RMSK                                                                         0xfffffffc
#define HWIO_MSS_UIM1_UART_DM_BADR_IN          \
        in_dword(HWIO_MSS_UIM1_UART_DM_BADR_ADDR)
#define HWIO_MSS_UIM1_UART_DM_BADR_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM1_UART_DM_BADR_ADDR, m)
#define HWIO_MSS_UIM1_UART_DM_BADR_OUT(v)      \
        out_dword(HWIO_MSS_UIM1_UART_DM_BADR_ADDR,v)
#define HWIO_MSS_UIM1_UART_DM_BADR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_UIM1_UART_DM_BADR_ADDR,m,v,HWIO_MSS_UIM1_UART_DM_BADR_IN)
#define HWIO_MSS_UIM1_UART_DM_BADR_RX_BASE_ADDR_BMSK                                                            0xfffffffc
#define HWIO_MSS_UIM1_UART_DM_BADR_RX_BASE_ADDR_SHFT                                                                   0x2

#define HWIO_MSS_UIM1_UART_DM_TESTSL_ADDR                                                                       (MSS_UIM1_UART_DM_REG_BASE      + 0x00000048)
#define HWIO_MSS_UIM1_UART_DM_TESTSL_RMSK                                                                             0x1f
#define HWIO_MSS_UIM1_UART_DM_TESTSL_IN          \
        in_dword(HWIO_MSS_UIM1_UART_DM_TESTSL_ADDR)
#define HWIO_MSS_UIM1_UART_DM_TESTSL_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM1_UART_DM_TESTSL_ADDR, m)
#define HWIO_MSS_UIM1_UART_DM_TESTSL_OUT(v)      \
        out_dword(HWIO_MSS_UIM1_UART_DM_TESTSL_ADDR,v)
#define HWIO_MSS_UIM1_UART_DM_TESTSL_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_UIM1_UART_DM_TESTSL_ADDR,m,v,HWIO_MSS_UIM1_UART_DM_TESTSL_IN)
#define HWIO_MSS_UIM1_UART_DM_TESTSL_TEST_EN_BMSK                                                                     0x10
#define HWIO_MSS_UIM1_UART_DM_TESTSL_TEST_EN_SHFT                                                                      0x4
#define HWIO_MSS_UIM1_UART_DM_TESTSL_TEST_SEL_BMSK                                                                     0xf
#define HWIO_MSS_UIM1_UART_DM_TESTSL_TEST_SEL_SHFT                                                                     0x0

#define HWIO_MSS_UIM1_UART_DM_TXFS_ADDR                                                                         (MSS_UIM1_UART_DM_REG_BASE      + 0x0000004c)
#define HWIO_MSS_UIM1_UART_DM_TXFS_RMSK                                                                         0xffffffff
#define HWIO_MSS_UIM1_UART_DM_TXFS_IN          \
        in_dword(HWIO_MSS_UIM1_UART_DM_TXFS_ADDR)
#define HWIO_MSS_UIM1_UART_DM_TXFS_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM1_UART_DM_TXFS_ADDR, m)
#define HWIO_MSS_UIM1_UART_DM_TXFS_TX_FIFO_STATE_MSB_BMSK                                                       0xffffc000
#define HWIO_MSS_UIM1_UART_DM_TXFS_TX_FIFO_STATE_MSB_SHFT                                                              0xe
#define HWIO_MSS_UIM1_UART_DM_TXFS_TX_ASYNC_FIFO_STATE_BMSK                                                         0x3c00
#define HWIO_MSS_UIM1_UART_DM_TXFS_TX_ASYNC_FIFO_STATE_SHFT                                                            0xa
#define HWIO_MSS_UIM1_UART_DM_TXFS_TX_BUFFER_STATE_BMSK                                                              0x380
#define HWIO_MSS_UIM1_UART_DM_TXFS_TX_BUFFER_STATE_SHFT                                                                0x7
#define HWIO_MSS_UIM1_UART_DM_TXFS_TX_FIFO_STATE_LSB_BMSK                                                             0x7f
#define HWIO_MSS_UIM1_UART_DM_TXFS_TX_FIFO_STATE_LSB_SHFT                                                              0x0

#define HWIO_MSS_UIM1_UART_DM_RXFS_ADDR                                                                         (MSS_UIM1_UART_DM_REG_BASE      + 0x00000050)
#define HWIO_MSS_UIM1_UART_DM_RXFS_RMSK                                                                         0xffffffff
#define HWIO_MSS_UIM1_UART_DM_RXFS_IN          \
        in_dword(HWIO_MSS_UIM1_UART_DM_RXFS_ADDR)
#define HWIO_MSS_UIM1_UART_DM_RXFS_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM1_UART_DM_RXFS_ADDR, m)
#define HWIO_MSS_UIM1_UART_DM_RXFS_RX_FIFO_STATE_MSB_BMSK                                                       0xffffc000
#define HWIO_MSS_UIM1_UART_DM_RXFS_RX_FIFO_STATE_MSB_SHFT                                                              0xe
#define HWIO_MSS_UIM1_UART_DM_RXFS_RX_ASYNC_FIFO_STATE_BMSK                                                         0x3c00
#define HWIO_MSS_UIM1_UART_DM_RXFS_RX_ASYNC_FIFO_STATE_SHFT                                                            0xa
#define HWIO_MSS_UIM1_UART_DM_RXFS_RX_BUFFER_STATE_BMSK                                                              0x380
#define HWIO_MSS_UIM1_UART_DM_RXFS_RX_BUFFER_STATE_SHFT                                                                0x7
#define HWIO_MSS_UIM1_UART_DM_RXFS_RX_FIFO_STATE_LSB_BMSK                                                             0x7f
#define HWIO_MSS_UIM1_UART_DM_RXFS_RX_FIFO_STATE_LSB_SHFT                                                              0x0

#define HWIO_MSS_UIM1_UART_DM_MISR_MODE_ADDR                                                                    (MSS_UIM1_UART_DM_REG_BASE      + 0x00000060)
#define HWIO_MSS_UIM1_UART_DM_MISR_MODE_RMSK                                                                           0x3
#define HWIO_MSS_UIM1_UART_DM_MISR_MODE_IN          \
        in_dword(HWIO_MSS_UIM1_UART_DM_MISR_MODE_ADDR)
#define HWIO_MSS_UIM1_UART_DM_MISR_MODE_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM1_UART_DM_MISR_MODE_ADDR, m)
#define HWIO_MSS_UIM1_UART_DM_MISR_MODE_OUT(v)      \
        out_dword(HWIO_MSS_UIM1_UART_DM_MISR_MODE_ADDR,v)
#define HWIO_MSS_UIM1_UART_DM_MISR_MODE_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_UIM1_UART_DM_MISR_MODE_ADDR,m,v,HWIO_MSS_UIM1_UART_DM_MISR_MODE_IN)
#define HWIO_MSS_UIM1_UART_DM_MISR_MODE_MODE_BMSK                                                                      0x3
#define HWIO_MSS_UIM1_UART_DM_MISR_MODE_MODE_SHFT                                                                      0x0

#define HWIO_MSS_UIM1_UART_DM_MISR_RESET_ADDR                                                                   (MSS_UIM1_UART_DM_REG_BASE      + 0x00000064)
#define HWIO_MSS_UIM1_UART_DM_MISR_RESET_RMSK                                                                          0x1
#define HWIO_MSS_UIM1_UART_DM_MISR_RESET_OUT(v)      \
        out_dword(HWIO_MSS_UIM1_UART_DM_MISR_RESET_ADDR,v)
#define HWIO_MSS_UIM1_UART_DM_MISR_RESET_RESET_BMSK                                                                    0x1
#define HWIO_MSS_UIM1_UART_DM_MISR_RESET_RESET_SHFT                                                                    0x0

#define HWIO_MSS_UIM1_UART_DM_MISR_EXPORT_ADDR                                                                  (MSS_UIM1_UART_DM_REG_BASE      + 0x00000068)
#define HWIO_MSS_UIM1_UART_DM_MISR_EXPORT_RMSK                                                                         0x1
#define HWIO_MSS_UIM1_UART_DM_MISR_EXPORT_IN          \
        in_dword(HWIO_MSS_UIM1_UART_DM_MISR_EXPORT_ADDR)
#define HWIO_MSS_UIM1_UART_DM_MISR_EXPORT_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM1_UART_DM_MISR_EXPORT_ADDR, m)
#define HWIO_MSS_UIM1_UART_DM_MISR_EXPORT_OUT(v)      \
        out_dword(HWIO_MSS_UIM1_UART_DM_MISR_EXPORT_ADDR,v)
#define HWIO_MSS_UIM1_UART_DM_MISR_EXPORT_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_UIM1_UART_DM_MISR_EXPORT_ADDR,m,v,HWIO_MSS_UIM1_UART_DM_MISR_EXPORT_IN)
#define HWIO_MSS_UIM1_UART_DM_MISR_EXPORT_EXPORT_BMSK                                                                  0x1
#define HWIO_MSS_UIM1_UART_DM_MISR_EXPORT_EXPORT_SHFT                                                                  0x0

#define HWIO_MSS_UIM1_UART_DM_MISR_VAL_ADDR                                                                     (MSS_UIM1_UART_DM_REG_BASE      + 0x0000006c)
#define HWIO_MSS_UIM1_UART_DM_MISR_VAL_RMSK                                                                          0x3ff
#define HWIO_MSS_UIM1_UART_DM_MISR_VAL_IN          \
        in_dword(HWIO_MSS_UIM1_UART_DM_MISR_VAL_ADDR)
#define HWIO_MSS_UIM1_UART_DM_MISR_VAL_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM1_UART_DM_MISR_VAL_ADDR, m)
#define HWIO_MSS_UIM1_UART_DM_MISR_VAL_VAL_BMSK                                                                      0x3ff
#define HWIO_MSS_UIM1_UART_DM_MISR_VAL_VAL_SHFT                                                                        0x0

#define HWIO_MSS_UIM1_UART_DM_TF_RF_DEPRECATED_ADDR                                                             (MSS_UIM1_UART_DM_REG_BASE      + 0x00000070)
#define HWIO_MSS_UIM1_UART_DM_TF_RF_DEPRECATED_RMSK                                                             0xffffffff
#define HWIO_MSS_UIM1_UART_DM_TF_RF_DEPRECATED_IN          \
        in_dword(HWIO_MSS_UIM1_UART_DM_TF_RF_DEPRECATED_ADDR)
#define HWIO_MSS_UIM1_UART_DM_TF_RF_DEPRECATED_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM1_UART_DM_TF_RF_DEPRECATED_ADDR, m)
#define HWIO_MSS_UIM1_UART_DM_TF_RF_DEPRECATED_OUT(v)      \
        out_dword(HWIO_MSS_UIM1_UART_DM_TF_RF_DEPRECATED_ADDR,v)
#define HWIO_MSS_UIM1_UART_DM_TF_RF_DEPRECATED_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_UIM1_UART_DM_TF_RF_DEPRECATED_ADDR,m,v,HWIO_MSS_UIM1_UART_DM_TF_RF_DEPRECATED_IN)
#define HWIO_MSS_UIM1_UART_DM_TF_RF_DEPRECATED_UART_DM_TF_RF_DEPRECATED_BMSK                                    0xffffffff
#define HWIO_MSS_UIM1_UART_DM_TF_RF_DEPRECATED_UART_DM_TF_RF_DEPRECATED_SHFT                                           0x0

#define HWIO_MSS_UIM1_UART_DM_TF_RF_2_DEPRECATED_ADDR                                                           (MSS_UIM1_UART_DM_REG_BASE      + 0x00000074)
#define HWIO_MSS_UIM1_UART_DM_TF_RF_2_DEPRECATED_RMSK                                                           0xffffffff
#define HWIO_MSS_UIM1_UART_DM_TF_RF_2_DEPRECATED_IN          \
        in_dword(HWIO_MSS_UIM1_UART_DM_TF_RF_2_DEPRECATED_ADDR)
#define HWIO_MSS_UIM1_UART_DM_TF_RF_2_DEPRECATED_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM1_UART_DM_TF_RF_2_DEPRECATED_ADDR, m)
#define HWIO_MSS_UIM1_UART_DM_TF_RF_2_DEPRECATED_OUT(v)      \
        out_dword(HWIO_MSS_UIM1_UART_DM_TF_RF_2_DEPRECATED_ADDR,v)
#define HWIO_MSS_UIM1_UART_DM_TF_RF_2_DEPRECATED_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_UIM1_UART_DM_TF_RF_2_DEPRECATED_ADDR,m,v,HWIO_MSS_UIM1_UART_DM_TF_RF_2_DEPRECATED_IN)
#define HWIO_MSS_UIM1_UART_DM_TF_RF_2_DEPRECATED_UART_DM_TF_RF_2_DEPRECATED_BMSK                                0xffffffff
#define HWIO_MSS_UIM1_UART_DM_TF_RF_2_DEPRECATED_UART_DM_TF_RF_2_DEPRECATED_SHFT                                       0x0

#define HWIO_MSS_UIM1_UART_DM_TF_RF_3_DEPRECATED_ADDR                                                           (MSS_UIM1_UART_DM_REG_BASE      + 0x00000078)
#define HWIO_MSS_UIM1_UART_DM_TF_RF_3_DEPRECATED_RMSK                                                           0xffffffff
#define HWIO_MSS_UIM1_UART_DM_TF_RF_3_DEPRECATED_IN          \
        in_dword(HWIO_MSS_UIM1_UART_DM_TF_RF_3_DEPRECATED_ADDR)
#define HWIO_MSS_UIM1_UART_DM_TF_RF_3_DEPRECATED_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM1_UART_DM_TF_RF_3_DEPRECATED_ADDR, m)
#define HWIO_MSS_UIM1_UART_DM_TF_RF_3_DEPRECATED_OUT(v)      \
        out_dword(HWIO_MSS_UIM1_UART_DM_TF_RF_3_DEPRECATED_ADDR,v)
#define HWIO_MSS_UIM1_UART_DM_TF_RF_3_DEPRECATED_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_UIM1_UART_DM_TF_RF_3_DEPRECATED_ADDR,m,v,HWIO_MSS_UIM1_UART_DM_TF_RF_3_DEPRECATED_IN)
#define HWIO_MSS_UIM1_UART_DM_TF_RF_3_DEPRECATED_UART_DM_TF_RF_3_DEPRECATED_BMSK                                0xffffffff
#define HWIO_MSS_UIM1_UART_DM_TF_RF_3_DEPRECATED_UART_DM_TF_RF_3_DEPRECATED_SHFT                                       0x0

#define HWIO_MSS_UIM1_UART_DM_TF_RF_4_DEPRECATED_ADDR                                                           (MSS_UIM1_UART_DM_REG_BASE      + 0x0000007c)
#define HWIO_MSS_UIM1_UART_DM_TF_RF_4_DEPRECATED_RMSK                                                           0xffffffff
#define HWIO_MSS_UIM1_UART_DM_TF_RF_4_DEPRECATED_IN          \
        in_dword(HWIO_MSS_UIM1_UART_DM_TF_RF_4_DEPRECATED_ADDR)
#define HWIO_MSS_UIM1_UART_DM_TF_RF_4_DEPRECATED_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM1_UART_DM_TF_RF_4_DEPRECATED_ADDR, m)
#define HWIO_MSS_UIM1_UART_DM_TF_RF_4_DEPRECATED_OUT(v)      \
        out_dword(HWIO_MSS_UIM1_UART_DM_TF_RF_4_DEPRECATED_ADDR,v)
#define HWIO_MSS_UIM1_UART_DM_TF_RF_4_DEPRECATED_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_UIM1_UART_DM_TF_RF_4_DEPRECATED_ADDR,m,v,HWIO_MSS_UIM1_UART_DM_TF_RF_4_DEPRECATED_IN)
#define HWIO_MSS_UIM1_UART_DM_TF_RF_4_DEPRECATED_UART_DM_TF_RF_4_DEPRECATED_BMSK                                0xffffffff
#define HWIO_MSS_UIM1_UART_DM_TF_RF_4_DEPRECATED_UART_DM_TF_RF_4_DEPRECATED_SHFT                                       0x0

#define HWIO_MSS_UIM1_UART_DM_SIM_CFG_ADDR                                                                      (MSS_UIM1_UART_DM_REG_BASE      + 0x00000080)
#define HWIO_MSS_UIM1_UART_DM_SIM_CFG_RMSK                                                                         0x3ffef
#define HWIO_MSS_UIM1_UART_DM_SIM_CFG_IN          \
        in_dword(HWIO_MSS_UIM1_UART_DM_SIM_CFG_ADDR)
#define HWIO_MSS_UIM1_UART_DM_SIM_CFG_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM1_UART_DM_SIM_CFG_ADDR, m)
#define HWIO_MSS_UIM1_UART_DM_SIM_CFG_OUT(v)      \
        out_dword(HWIO_MSS_UIM1_UART_DM_SIM_CFG_ADDR,v)
#define HWIO_MSS_UIM1_UART_DM_SIM_CFG_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_UIM1_UART_DM_SIM_CFG_ADDR,m,v,HWIO_MSS_UIM1_UART_DM_SIM_CFG_IN)
#define HWIO_MSS_UIM1_UART_DM_SIM_CFG_UIM_TX_MODE_BMSK                                                             0x20000
#define HWIO_MSS_UIM1_UART_DM_SIM_CFG_UIM_TX_MODE_SHFT                                                                0x11
#define HWIO_MSS_UIM1_UART_DM_SIM_CFG_UIM_RX_MODE_BMSK                                                             0x10000
#define HWIO_MSS_UIM1_UART_DM_SIM_CFG_UIM_RX_MODE_SHFT                                                                0x10
#define HWIO_MSS_UIM1_UART_DM_SIM_CFG_SIM_STOP_BIT_LEN_BMSK                                                         0xff00
#define HWIO_MSS_UIM1_UART_DM_SIM_CFG_SIM_STOP_BIT_LEN_SHFT                                                            0x8
#define HWIO_MSS_UIM1_UART_DM_SIM_CFG_SIM_CLK_ON_BMSK                                                                 0x80
#define HWIO_MSS_UIM1_UART_DM_SIM_CFG_SIM_CLK_ON_SHFT                                                                  0x7
#define HWIO_MSS_UIM1_UART_DM_SIM_CFG_SIM_CLK_TD8_SEL_BMSK                                                            0x40
#define HWIO_MSS_UIM1_UART_DM_SIM_CFG_SIM_CLK_TD8_SEL_SHFT                                                             0x6
#define HWIO_MSS_UIM1_UART_DM_SIM_CFG_SIM_CLK_STOP_HIGH_BMSK                                                          0x20
#define HWIO_MSS_UIM1_UART_DM_SIM_CFG_SIM_CLK_STOP_HIGH_SHFT                                                           0x5
#define HWIO_MSS_UIM1_UART_DM_SIM_CFG_MASK_RX_BMSK                                                                     0x8
#define HWIO_MSS_UIM1_UART_DM_SIM_CFG_MASK_RX_SHFT                                                                     0x3
#define HWIO_MSS_UIM1_UART_DM_SIM_CFG_SWAP_D_BMSK                                                                      0x4
#define HWIO_MSS_UIM1_UART_DM_SIM_CFG_SWAP_D_SHFT                                                                      0x2
#define HWIO_MSS_UIM1_UART_DM_SIM_CFG_INV_D_BMSK                                                                       0x2
#define HWIO_MSS_UIM1_UART_DM_SIM_CFG_INV_D_SHFT                                                                       0x1
#define HWIO_MSS_UIM1_UART_DM_SIM_CFG_SIM_SEL_BMSK                                                                     0x1
#define HWIO_MSS_UIM1_UART_DM_SIM_CFG_SIM_SEL_SHFT                                                                     0x0

#define HWIO_MSS_UIM1_UART_DM_TEST_WR_ADDR_ADDR                                                                 (MSS_UIM1_UART_DM_REG_BASE      + 0x00000084)
#define HWIO_MSS_UIM1_UART_DM_TEST_WR_ADDR_RMSK                                                                 0xffffffff
#define HWIO_MSS_UIM1_UART_DM_TEST_WR_ADDR_IN          \
        in_dword(HWIO_MSS_UIM1_UART_DM_TEST_WR_ADDR_ADDR)
#define HWIO_MSS_UIM1_UART_DM_TEST_WR_ADDR_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM1_UART_DM_TEST_WR_ADDR_ADDR, m)
#define HWIO_MSS_UIM1_UART_DM_TEST_WR_ADDR_OUT(v)      \
        out_dword(HWIO_MSS_UIM1_UART_DM_TEST_WR_ADDR_ADDR,v)
#define HWIO_MSS_UIM1_UART_DM_TEST_WR_ADDR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_UIM1_UART_DM_TEST_WR_ADDR_ADDR,m,v,HWIO_MSS_UIM1_UART_DM_TEST_WR_ADDR_IN)
#define HWIO_MSS_UIM1_UART_DM_TEST_WR_ADDR_TEST_WR_ADDR_BMSK                                                    0xffffffff
#define HWIO_MSS_UIM1_UART_DM_TEST_WR_ADDR_TEST_WR_ADDR_SHFT                                                           0x0

#define HWIO_MSS_UIM1_UART_DM_TEST_WR_DATA_ADDR                                                                 (MSS_UIM1_UART_DM_REG_BASE      + 0x00000088)
#define HWIO_MSS_UIM1_UART_DM_TEST_WR_DATA_RMSK                                                                 0xffffffff
#define HWIO_MSS_UIM1_UART_DM_TEST_WR_DATA_OUT(v)      \
        out_dword(HWIO_MSS_UIM1_UART_DM_TEST_WR_DATA_ADDR,v)
#define HWIO_MSS_UIM1_UART_DM_TEST_WR_DATA_TEST_WR_DATA_BMSK                                                    0xffffffff
#define HWIO_MSS_UIM1_UART_DM_TEST_WR_DATA_TEST_WR_DATA_SHFT                                                           0x0

#define HWIO_MSS_UIM1_UART_DM_TEST_RD_ADDR_ADDR                                                                 (MSS_UIM1_UART_DM_REG_BASE      + 0x0000008c)
#define HWIO_MSS_UIM1_UART_DM_TEST_RD_ADDR_RMSK                                                                 0xffffffff
#define HWIO_MSS_UIM1_UART_DM_TEST_RD_ADDR_IN          \
        in_dword(HWIO_MSS_UIM1_UART_DM_TEST_RD_ADDR_ADDR)
#define HWIO_MSS_UIM1_UART_DM_TEST_RD_ADDR_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM1_UART_DM_TEST_RD_ADDR_ADDR, m)
#define HWIO_MSS_UIM1_UART_DM_TEST_RD_ADDR_OUT(v)      \
        out_dword(HWIO_MSS_UIM1_UART_DM_TEST_RD_ADDR_ADDR,v)
#define HWIO_MSS_UIM1_UART_DM_TEST_RD_ADDR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_UIM1_UART_DM_TEST_RD_ADDR_ADDR,m,v,HWIO_MSS_UIM1_UART_DM_TEST_RD_ADDR_IN)
#define HWIO_MSS_UIM1_UART_DM_TEST_RD_ADDR_TEST_RD_ADDR_BMSK                                                    0xffffffff
#define HWIO_MSS_UIM1_UART_DM_TEST_RD_ADDR_TEST_RD_ADDR_SHFT                                                           0x0

#define HWIO_MSS_UIM1_UART_DM_TEST_RD_DATA_ADDR                                                                 (MSS_UIM1_UART_DM_REG_BASE      + 0x00000090)
#define HWIO_MSS_UIM1_UART_DM_TEST_RD_DATA_RMSK                                                                 0xffffffff
#define HWIO_MSS_UIM1_UART_DM_TEST_RD_DATA_IN          \
        in_dword(HWIO_MSS_UIM1_UART_DM_TEST_RD_DATA_ADDR)
#define HWIO_MSS_UIM1_UART_DM_TEST_RD_DATA_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM1_UART_DM_TEST_RD_DATA_ADDR, m)
#define HWIO_MSS_UIM1_UART_DM_TEST_RD_DATA_TEST_RD_DATA_BMSK                                                    0xffffffff
#define HWIO_MSS_UIM1_UART_DM_TEST_RD_DATA_TEST_RD_DATA_SHFT                                                           0x0

#define HWIO_MSS_UIM1_UART_DM_CSR_ADDR                                                                          (MSS_UIM1_UART_DM_REG_BASE      + 0x000000a0)
#define HWIO_MSS_UIM1_UART_DM_CSR_RMSK                                                                                0xff
#define HWIO_MSS_UIM1_UART_DM_CSR_IN          \
        in_dword(HWIO_MSS_UIM1_UART_DM_CSR_ADDR)
#define HWIO_MSS_UIM1_UART_DM_CSR_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM1_UART_DM_CSR_ADDR, m)
#define HWIO_MSS_UIM1_UART_DM_CSR_OUT(v)      \
        out_dword(HWIO_MSS_UIM1_UART_DM_CSR_ADDR,v)
#define HWIO_MSS_UIM1_UART_DM_CSR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_UIM1_UART_DM_CSR_ADDR,m,v,HWIO_MSS_UIM1_UART_DM_CSR_IN)
#define HWIO_MSS_UIM1_UART_DM_CSR_UART_RX_CLK_SEL_BMSK                                                                0xf0
#define HWIO_MSS_UIM1_UART_DM_CSR_UART_RX_CLK_SEL_SHFT                                                                 0x4
#define HWIO_MSS_UIM1_UART_DM_CSR_UART_TX_CLK_SEL_BMSK                                                                 0xf
#define HWIO_MSS_UIM1_UART_DM_CSR_UART_TX_CLK_SEL_SHFT                                                                 0x0

#define HWIO_MSS_UIM1_UART_DM_SR_ADDR                                                                           (MSS_UIM1_UART_DM_REG_BASE      + 0x000000a4)
#define HWIO_MSS_UIM1_UART_DM_SR_RMSK                                                                               0x1fff
#define HWIO_MSS_UIM1_UART_DM_SR_IN          \
        in_dword(HWIO_MSS_UIM1_UART_DM_SR_ADDR)
#define HWIO_MSS_UIM1_UART_DM_SR_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM1_UART_DM_SR_ADDR, m)
#define HWIO_MSS_UIM1_UART_DM_SR_COMMAND_IN_PROGRESS_BMSK                                                           0x1000
#define HWIO_MSS_UIM1_UART_DM_SR_COMMAND_IN_PROGRESS_SHFT                                                              0xc
#define HWIO_MSS_UIM1_UART_DM_SR_TRANS_END_TRIGGER_BMSK                                                              0xc00
#define HWIO_MSS_UIM1_UART_DM_SR_TRANS_END_TRIGGER_SHFT                                                                0xa
#define HWIO_MSS_UIM1_UART_DM_SR_TRANS_ACTIVE_BMSK                                                                   0x200
#define HWIO_MSS_UIM1_UART_DM_SR_TRANS_ACTIVE_SHFT                                                                     0x9
#define HWIO_MSS_UIM1_UART_DM_SR_RX_BREAK_START_LAST_BMSK                                                            0x100
#define HWIO_MSS_UIM1_UART_DM_SR_RX_BREAK_START_LAST_SHFT                                                              0x8
#define HWIO_MSS_UIM1_UART_DM_SR_HUNT_CHAR_BMSK                                                                       0x80
#define HWIO_MSS_UIM1_UART_DM_SR_HUNT_CHAR_SHFT                                                                        0x7
#define HWIO_MSS_UIM1_UART_DM_SR_RX_BREAK_BMSK                                                                        0x40
#define HWIO_MSS_UIM1_UART_DM_SR_RX_BREAK_SHFT                                                                         0x6
#define HWIO_MSS_UIM1_UART_DM_SR_PAR_FRAME_ERR_BMSK                                                                   0x20
#define HWIO_MSS_UIM1_UART_DM_SR_PAR_FRAME_ERR_SHFT                                                                    0x5
#define HWIO_MSS_UIM1_UART_DM_SR_UART_OVERRUN_BMSK                                                                    0x10
#define HWIO_MSS_UIM1_UART_DM_SR_UART_OVERRUN_SHFT                                                                     0x4
#define HWIO_MSS_UIM1_UART_DM_SR_TXEMT_BMSK                                                                            0x8
#define HWIO_MSS_UIM1_UART_DM_SR_TXEMT_SHFT                                                                            0x3
#define HWIO_MSS_UIM1_UART_DM_SR_TXRDY_BMSK                                                                            0x4
#define HWIO_MSS_UIM1_UART_DM_SR_TXRDY_SHFT                                                                            0x2
#define HWIO_MSS_UIM1_UART_DM_SR_RXFULL_BMSK                                                                           0x2
#define HWIO_MSS_UIM1_UART_DM_SR_RXFULL_SHFT                                                                           0x1
#define HWIO_MSS_UIM1_UART_DM_SR_RXRDY_BMSK                                                                            0x1
#define HWIO_MSS_UIM1_UART_DM_SR_RXRDY_SHFT                                                                            0x0

#define HWIO_MSS_UIM1_UART_DM_CR_ADDR                                                                           (MSS_UIM1_UART_DM_REG_BASE      + 0x000000a8)
#define HWIO_MSS_UIM1_UART_DM_CR_RMSK                                                                                0xfff
#define HWIO_MSS_UIM1_UART_DM_CR_IN          \
        in_dword(HWIO_MSS_UIM1_UART_DM_CR_ADDR)
#define HWIO_MSS_UIM1_UART_DM_CR_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM1_UART_DM_CR_ADDR, m)
#define HWIO_MSS_UIM1_UART_DM_CR_OUT(v)      \
        out_dword(HWIO_MSS_UIM1_UART_DM_CR_ADDR,v)
#define HWIO_MSS_UIM1_UART_DM_CR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_UIM1_UART_DM_CR_ADDR,m,v,HWIO_MSS_UIM1_UART_DM_CR_IN)
#define HWIO_MSS_UIM1_UART_DM_CR_CHANNEL_COMMAND_MSB_BMSK                                                            0x800
#define HWIO_MSS_UIM1_UART_DM_CR_CHANNEL_COMMAND_MSB_SHFT                                                              0xb
#define HWIO_MSS_UIM1_UART_DM_CR_GENERAL_COMMAND_BMSK                                                                0x700
#define HWIO_MSS_UIM1_UART_DM_CR_GENERAL_COMMAND_SHFT                                                                  0x8
#define HWIO_MSS_UIM1_UART_DM_CR_CHANNEL_COMMAND_LSB_BMSK                                                             0xf0
#define HWIO_MSS_UIM1_UART_DM_CR_CHANNEL_COMMAND_LSB_SHFT                                                              0x4
#define HWIO_MSS_UIM1_UART_DM_CR_UART_TX_DISABLE_BMSK                                                                  0x8
#define HWIO_MSS_UIM1_UART_DM_CR_UART_TX_DISABLE_SHFT                                                                  0x3
#define HWIO_MSS_UIM1_UART_DM_CR_UART_TX_EN_BMSK                                                                       0x4
#define HWIO_MSS_UIM1_UART_DM_CR_UART_TX_EN_SHFT                                                                       0x2
#define HWIO_MSS_UIM1_UART_DM_CR_UART_RX_DISABLE_BMSK                                                                  0x2
#define HWIO_MSS_UIM1_UART_DM_CR_UART_RX_DISABLE_SHFT                                                                  0x1
#define HWIO_MSS_UIM1_UART_DM_CR_UART_RX_EN_BMSK                                                                       0x1
#define HWIO_MSS_UIM1_UART_DM_CR_UART_RX_EN_SHFT                                                                       0x0

#define HWIO_MSS_UIM1_UART_DM_MISR_ADDR                                                                         (MSS_UIM1_UART_DM_REG_BASE      + 0x000000ac)
#define HWIO_MSS_UIM1_UART_DM_MISR_RMSK                                                                            0x3ffff
#define HWIO_MSS_UIM1_UART_DM_MISR_IN          \
        in_dword(HWIO_MSS_UIM1_UART_DM_MISR_ADDR)
#define HWIO_MSS_UIM1_UART_DM_MISR_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM1_UART_DM_MISR_ADDR, m)
#define HWIO_MSS_UIM1_UART_DM_MISR_UART_MISR_BMSK                                                                  0x3ffff
#define HWIO_MSS_UIM1_UART_DM_MISR_UART_MISR_SHFT                                                                      0x0

#define HWIO_MSS_UIM1_UART_DM_IMR_ADDR                                                                          (MSS_UIM1_UART_DM_REG_BASE      + 0x000000b0)
#define HWIO_MSS_UIM1_UART_DM_IMR_RMSK                                                                             0x3ffff
#define HWIO_MSS_UIM1_UART_DM_IMR_IN          \
        in_dword(HWIO_MSS_UIM1_UART_DM_IMR_ADDR)
#define HWIO_MSS_UIM1_UART_DM_IMR_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM1_UART_DM_IMR_ADDR, m)
#define HWIO_MSS_UIM1_UART_DM_IMR_OUT(v)      \
        out_dword(HWIO_MSS_UIM1_UART_DM_IMR_ADDR,v)
#define HWIO_MSS_UIM1_UART_DM_IMR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_UIM1_UART_DM_IMR_ADDR,m,v,HWIO_MSS_UIM1_UART_DM_IMR_IN)
#define HWIO_MSS_UIM1_UART_DM_IMR_NO_FINISH_CMD_VIOL_BMSK                                                          0x20000
#define HWIO_MSS_UIM1_UART_DM_IMR_NO_FINISH_CMD_VIOL_SHFT                                                             0x11
#define HWIO_MSS_UIM1_UART_DM_IMR_WWT_IRQ_BMSK                                                                     0x10000
#define HWIO_MSS_UIM1_UART_DM_IMR_WWT_IRQ_SHFT                                                                        0x10
#define HWIO_MSS_UIM1_UART_DM_IMR_TXCOMP_IRQ_BMSK                                                                   0x8000
#define HWIO_MSS_UIM1_UART_DM_IMR_TXCOMP_IRQ_SHFT                                                                      0xf
#define HWIO_MSS_UIM1_UART_DM_IMR_RX_RD_ERROR_IRQ_BMSK                                                              0x4000
#define HWIO_MSS_UIM1_UART_DM_IMR_RX_RD_ERROR_IRQ_SHFT                                                                 0xe
#define HWIO_MSS_UIM1_UART_DM_IMR_TX_WR_ERROR_IRQ_BMSK                                                              0x2000
#define HWIO_MSS_UIM1_UART_DM_IMR_TX_WR_ERROR_IRQ_SHFT                                                                 0xd
#define HWIO_MSS_UIM1_UART_DM_IMR_PAR_FRAME_ERR_IRQ_BMSK                                                            0x1000
#define HWIO_MSS_UIM1_UART_DM_IMR_PAR_FRAME_ERR_IRQ_SHFT                                                               0xc
#define HWIO_MSS_UIM1_UART_DM_IMR_RXBREAK_END_BMSK                                                                   0x800
#define HWIO_MSS_UIM1_UART_DM_IMR_RXBREAK_END_SHFT                                                                     0xb
#define HWIO_MSS_UIM1_UART_DM_IMR_RXBREAK_START_BMSK                                                                 0x400
#define HWIO_MSS_UIM1_UART_DM_IMR_RXBREAK_START_SHFT                                                                   0xa
#define HWIO_MSS_UIM1_UART_DM_IMR_TX_DONE_BMSK                                                                       0x200
#define HWIO_MSS_UIM1_UART_DM_IMR_TX_DONE_SHFT                                                                         0x9
#define HWIO_MSS_UIM1_UART_DM_IMR_TX_ERROR_BMSK                                                                      0x100
#define HWIO_MSS_UIM1_UART_DM_IMR_TX_ERROR_SHFT                                                                        0x8
#define HWIO_MSS_UIM1_UART_DM_IMR_TX_READY_BMSK                                                                       0x80
#define HWIO_MSS_UIM1_UART_DM_IMR_TX_READY_SHFT                                                                        0x7
#define HWIO_MSS_UIM1_UART_DM_IMR_CURRENT_CTS_BMSK                                                                    0x40
#define HWIO_MSS_UIM1_UART_DM_IMR_CURRENT_CTS_SHFT                                                                     0x6
#define HWIO_MSS_UIM1_UART_DM_IMR_DELTA_CTS_BMSK                                                                      0x20
#define HWIO_MSS_UIM1_UART_DM_IMR_DELTA_CTS_SHFT                                                                       0x5
#define HWIO_MSS_UIM1_UART_DM_IMR_RXLEV_BMSK                                                                          0x10
#define HWIO_MSS_UIM1_UART_DM_IMR_RXLEV_SHFT                                                                           0x4
#define HWIO_MSS_UIM1_UART_DM_IMR_RXSTALE_BMSK                                                                         0x8
#define HWIO_MSS_UIM1_UART_DM_IMR_RXSTALE_SHFT                                                                         0x3
#define HWIO_MSS_UIM1_UART_DM_IMR_RXBREAK_CHANGE_BMSK                                                                  0x4
#define HWIO_MSS_UIM1_UART_DM_IMR_RXBREAK_CHANGE_SHFT                                                                  0x2
#define HWIO_MSS_UIM1_UART_DM_IMR_RXHUNT_BMSK                                                                          0x2
#define HWIO_MSS_UIM1_UART_DM_IMR_RXHUNT_SHFT                                                                          0x1
#define HWIO_MSS_UIM1_UART_DM_IMR_TXLEV_BMSK                                                                           0x1
#define HWIO_MSS_UIM1_UART_DM_IMR_TXLEV_SHFT                                                                           0x0

#define HWIO_MSS_UIM1_UART_DM_ISR_ADDR                                                                          (MSS_UIM1_UART_DM_REG_BASE      + 0x000000b4)
#define HWIO_MSS_UIM1_UART_DM_ISR_RMSK                                                                             0x3ffff
#define HWIO_MSS_UIM1_UART_DM_ISR_IN          \
        in_dword(HWIO_MSS_UIM1_UART_DM_ISR_ADDR)
#define HWIO_MSS_UIM1_UART_DM_ISR_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM1_UART_DM_ISR_ADDR, m)
#define HWIO_MSS_UIM1_UART_DM_ISR_NO_FINISH_CMD_VIOL_BMSK                                                          0x20000
#define HWIO_MSS_UIM1_UART_DM_ISR_NO_FINISH_CMD_VIOL_SHFT                                                             0x11
#define HWIO_MSS_UIM1_UART_DM_ISR_WWT_IRQ_BMSK                                                                     0x10000
#define HWIO_MSS_UIM1_UART_DM_ISR_WWT_IRQ_SHFT                                                                        0x10
#define HWIO_MSS_UIM1_UART_DM_ISR_TXCOMP_IRQ_BMSK                                                                   0x8000
#define HWIO_MSS_UIM1_UART_DM_ISR_TXCOMP_IRQ_SHFT                                                                      0xf
#define HWIO_MSS_UIM1_UART_DM_ISR_RX_RD_ERROR_IRQ_BMSK                                                              0x4000
#define HWIO_MSS_UIM1_UART_DM_ISR_RX_RD_ERROR_IRQ_SHFT                                                                 0xe
#define HWIO_MSS_UIM1_UART_DM_ISR_TX_WR_ERROR_IRQ_BMSK                                                              0x2000
#define HWIO_MSS_UIM1_UART_DM_ISR_TX_WR_ERROR_IRQ_SHFT                                                                 0xd
#define HWIO_MSS_UIM1_UART_DM_ISR_PAR_FRAME_ERR_IRQ_BMSK                                                            0x1000
#define HWIO_MSS_UIM1_UART_DM_ISR_PAR_FRAME_ERR_IRQ_SHFT                                                               0xc
#define HWIO_MSS_UIM1_UART_DM_ISR_RXBREAK_END_BMSK                                                                   0x800
#define HWIO_MSS_UIM1_UART_DM_ISR_RXBREAK_END_SHFT                                                                     0xb
#define HWIO_MSS_UIM1_UART_DM_ISR_RXBREAK_START_BMSK                                                                 0x400
#define HWIO_MSS_UIM1_UART_DM_ISR_RXBREAK_START_SHFT                                                                   0xa
#define HWIO_MSS_UIM1_UART_DM_ISR_TX_DONE_BMSK                                                                       0x200
#define HWIO_MSS_UIM1_UART_DM_ISR_TX_DONE_SHFT                                                                         0x9
#define HWIO_MSS_UIM1_UART_DM_ISR_TX_ERROR_BMSK                                                                      0x100
#define HWIO_MSS_UIM1_UART_DM_ISR_TX_ERROR_SHFT                                                                        0x8
#define HWIO_MSS_UIM1_UART_DM_ISR_TX_READY_BMSK                                                                       0x80
#define HWIO_MSS_UIM1_UART_DM_ISR_TX_READY_SHFT                                                                        0x7
#define HWIO_MSS_UIM1_UART_DM_ISR_CURRENT_CTS_BMSK                                                                    0x40
#define HWIO_MSS_UIM1_UART_DM_ISR_CURRENT_CTS_SHFT                                                                     0x6
#define HWIO_MSS_UIM1_UART_DM_ISR_DELTA_CTS_BMSK                                                                      0x20
#define HWIO_MSS_UIM1_UART_DM_ISR_DELTA_CTS_SHFT                                                                       0x5
#define HWIO_MSS_UIM1_UART_DM_ISR_RXLEV_BMSK                                                                          0x10
#define HWIO_MSS_UIM1_UART_DM_ISR_RXLEV_SHFT                                                                           0x4
#define HWIO_MSS_UIM1_UART_DM_ISR_RXSTALE_BMSK                                                                         0x8
#define HWIO_MSS_UIM1_UART_DM_ISR_RXSTALE_SHFT                                                                         0x3
#define HWIO_MSS_UIM1_UART_DM_ISR_RXBREAK_CHANGE_BMSK                                                                  0x4
#define HWIO_MSS_UIM1_UART_DM_ISR_RXBREAK_CHANGE_SHFT                                                                  0x2
#define HWIO_MSS_UIM1_UART_DM_ISR_RXHUNT_BMSK                                                                          0x2
#define HWIO_MSS_UIM1_UART_DM_ISR_RXHUNT_SHFT                                                                          0x1
#define HWIO_MSS_UIM1_UART_DM_ISR_TXLEV_BMSK                                                                           0x1
#define HWIO_MSS_UIM1_UART_DM_ISR_TXLEV_SHFT                                                                           0x0

#define HWIO_MSS_UIM1_UART_DM_IRDA_ADDR                                                                         (MSS_UIM1_UART_DM_REG_BASE      + 0x000000b8)
#define HWIO_MSS_UIM1_UART_DM_IRDA_RMSK                                                                               0x1f
#define HWIO_MSS_UIM1_UART_DM_IRDA_IN          \
        in_dword(HWIO_MSS_UIM1_UART_DM_IRDA_ADDR)
#define HWIO_MSS_UIM1_UART_DM_IRDA_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM1_UART_DM_IRDA_ADDR, m)
#define HWIO_MSS_UIM1_UART_DM_IRDA_OUT(v)      \
        out_dword(HWIO_MSS_UIM1_UART_DM_IRDA_ADDR,v)
#define HWIO_MSS_UIM1_UART_DM_IRDA_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_UIM1_UART_DM_IRDA_ADDR,m,v,HWIO_MSS_UIM1_UART_DM_IRDA_IN)
#define HWIO_MSS_UIM1_UART_DM_IRDA_MEDIUM_RATE_EN_BMSK                                                                0x10
#define HWIO_MSS_UIM1_UART_DM_IRDA_MEDIUM_RATE_EN_SHFT                                                                 0x4
#define HWIO_MSS_UIM1_UART_DM_IRDA_IRDA_LOOPBACK_BMSK                                                                  0x8
#define HWIO_MSS_UIM1_UART_DM_IRDA_IRDA_LOOPBACK_SHFT                                                                  0x3
#define HWIO_MSS_UIM1_UART_DM_IRDA_INVERT_IRDA_TX_BMSK                                                                 0x4
#define HWIO_MSS_UIM1_UART_DM_IRDA_INVERT_IRDA_TX_SHFT                                                                 0x2
#define HWIO_MSS_UIM1_UART_DM_IRDA_INVERT_IRDA_RX_BMSK                                                                 0x2
#define HWIO_MSS_UIM1_UART_DM_IRDA_INVERT_IRDA_RX_SHFT                                                                 0x1
#define HWIO_MSS_UIM1_UART_DM_IRDA_IRDA_EN_BMSK                                                                        0x1
#define HWIO_MSS_UIM1_UART_DM_IRDA_IRDA_EN_SHFT                                                                        0x0

#define HWIO_MSS_UIM1_UART_DM_RX_TOTAL_SNAP_ADDR                                                                (MSS_UIM1_UART_DM_REG_BASE      + 0x000000bc)
#define HWIO_MSS_UIM1_UART_DM_RX_TOTAL_SNAP_RMSK                                                                  0xffffff
#define HWIO_MSS_UIM1_UART_DM_RX_TOTAL_SNAP_IN          \
        in_dword(HWIO_MSS_UIM1_UART_DM_RX_TOTAL_SNAP_ADDR)
#define HWIO_MSS_UIM1_UART_DM_RX_TOTAL_SNAP_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM1_UART_DM_RX_TOTAL_SNAP_ADDR, m)
#define HWIO_MSS_UIM1_UART_DM_RX_TOTAL_SNAP_RX_TOTAL_BYTES_BMSK                                                   0xffffff
#define HWIO_MSS_UIM1_UART_DM_RX_TOTAL_SNAP_RX_TOTAL_BYTES_SHFT                                                        0x0

#define HWIO_MSS_UIM1_UART_DM_WWT_TIMEOUT_ADDR                                                                  (MSS_UIM1_UART_DM_REG_BASE      + 0x000000c0)
#define HWIO_MSS_UIM1_UART_DM_WWT_TIMEOUT_RMSK                                                                   0x3ffffff
#define HWIO_MSS_UIM1_UART_DM_WWT_TIMEOUT_IN          \
        in_dword(HWIO_MSS_UIM1_UART_DM_WWT_TIMEOUT_ADDR)
#define HWIO_MSS_UIM1_UART_DM_WWT_TIMEOUT_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM1_UART_DM_WWT_TIMEOUT_ADDR, m)
#define HWIO_MSS_UIM1_UART_DM_WWT_TIMEOUT_OUT(v)      \
        out_dword(HWIO_MSS_UIM1_UART_DM_WWT_TIMEOUT_ADDR,v)
#define HWIO_MSS_UIM1_UART_DM_WWT_TIMEOUT_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_UIM1_UART_DM_WWT_TIMEOUT_ADDR,m,v,HWIO_MSS_UIM1_UART_DM_WWT_TIMEOUT_IN)
#define HWIO_MSS_UIM1_UART_DM_WWT_TIMEOUT_WWT_CYCLE_REENABLE_BMSK                                                0x2000000
#define HWIO_MSS_UIM1_UART_DM_WWT_TIMEOUT_WWT_CYCLE_REENABLE_SHFT                                                     0x19
#define HWIO_MSS_UIM1_UART_DM_WWT_TIMEOUT_WWT_TIMEOUT_BMSK                                                       0x1ffffff
#define HWIO_MSS_UIM1_UART_DM_WWT_TIMEOUT_WWT_TIMEOUT_SHFT                                                             0x0

#define HWIO_MSS_UIM1_UART_DM_CLK_CTRL_ADDR                                                                     (MSS_UIM1_UART_DM_REG_BASE      + 0x000000c4)
#define HWIO_MSS_UIM1_UART_DM_CLK_CTRL_RMSK                                                                       0xf06731
#define HWIO_MSS_UIM1_UART_DM_CLK_CTRL_IN          \
        in_dword(HWIO_MSS_UIM1_UART_DM_CLK_CTRL_ADDR)
#define HWIO_MSS_UIM1_UART_DM_CLK_CTRL_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM1_UART_DM_CLK_CTRL_ADDR, m)
#define HWIO_MSS_UIM1_UART_DM_CLK_CTRL_OUT(v)      \
        out_dword(HWIO_MSS_UIM1_UART_DM_CLK_CTRL_ADDR,v)
#define HWIO_MSS_UIM1_UART_DM_CLK_CTRL_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_UIM1_UART_DM_CLK_CTRL_ADDR,m,v,HWIO_MSS_UIM1_UART_DM_CLK_CTRL_IN)
#define HWIO_MSS_UIM1_UART_DM_CLK_CTRL_UART_IRDA_CLK_CGC_OPEN_BMSK                                                0x800000
#define HWIO_MSS_UIM1_UART_DM_CLK_CTRL_UART_IRDA_CLK_CGC_OPEN_SHFT                                                    0x17
#define HWIO_MSS_UIM1_UART_DM_CLK_CTRL_UART_SIM_CLK_CGC_OPEN_BMSK                                                 0x400000
#define HWIO_MSS_UIM1_UART_DM_CLK_CTRL_UART_SIM_CLK_CGC_OPEN_SHFT                                                     0x16
#define HWIO_MSS_UIM1_UART_DM_CLK_CTRL_UART_RX_CLK_CGC_OPEN_BMSK                                                  0x200000
#define HWIO_MSS_UIM1_UART_DM_CLK_CTRL_UART_RX_CLK_CGC_OPEN_SHFT                                                      0x15
#define HWIO_MSS_UIM1_UART_DM_CLK_CTRL_UART_TX_CLK_CGC_OPEN_BMSK                                                  0x100000
#define HWIO_MSS_UIM1_UART_DM_CLK_CTRL_UART_TX_CLK_CGC_OPEN_SHFT                                                      0x14
#define HWIO_MSS_UIM1_UART_DM_CLK_CTRL_AHB_RX_BAM_CLK_CGC_OPEN_BMSK                                                 0x4000
#define HWIO_MSS_UIM1_UART_DM_CLK_CTRL_AHB_RX_BAM_CLK_CGC_OPEN_SHFT                                                    0xe
#define HWIO_MSS_UIM1_UART_DM_CLK_CTRL_AHB_TX_BAM_CLK_CGC_OPEN_BMSK                                                 0x2000
#define HWIO_MSS_UIM1_UART_DM_CLK_CTRL_AHB_TX_BAM_CLK_CGC_OPEN_SHFT                                                    0xd
#define HWIO_MSS_UIM1_UART_DM_CLK_CTRL_AHB_RX_CLK_CGC_OPEN_BMSK                                                      0x400
#define HWIO_MSS_UIM1_UART_DM_CLK_CTRL_AHB_RX_CLK_CGC_OPEN_SHFT                                                        0xa
#define HWIO_MSS_UIM1_UART_DM_CLK_CTRL_AHB_TX_CLK_CGC_OPEN_BMSK                                                      0x200
#define HWIO_MSS_UIM1_UART_DM_CLK_CTRL_AHB_TX_CLK_CGC_OPEN_SHFT                                                        0x9
#define HWIO_MSS_UIM1_UART_DM_CLK_CTRL_AHB_WR_CLK_CGC_OPEN_BMSK                                                      0x100
#define HWIO_MSS_UIM1_UART_DM_CLK_CTRL_AHB_WR_CLK_CGC_OPEN_SHFT                                                        0x8
#define HWIO_MSS_UIM1_UART_DM_CLK_CTRL_RX_ENABLE_CGC_OPT_BMSK                                                         0x20
#define HWIO_MSS_UIM1_UART_DM_CLK_CTRL_RX_ENABLE_CGC_OPT_SHFT                                                          0x5
#define HWIO_MSS_UIM1_UART_DM_CLK_CTRL_TX_ENABLE_CGC_OPT_BMSK                                                         0x10
#define HWIO_MSS_UIM1_UART_DM_CLK_CTRL_TX_ENABLE_CGC_OPT_SHFT                                                          0x4
#define HWIO_MSS_UIM1_UART_DM_CLK_CTRL_AHB_CLK_CGC_CLOSE_BMSK                                                          0x1
#define HWIO_MSS_UIM1_UART_DM_CLK_CTRL_AHB_CLK_CGC_CLOSE_SHFT                                                          0x0

#define HWIO_MSS_UIM1_UART_DM_BCR_ADDR                                                                          (MSS_UIM1_UART_DM_REG_BASE      + 0x000000c8)
#define HWIO_MSS_UIM1_UART_DM_BCR_RMSK                                                                                0x77
#define HWIO_MSS_UIM1_UART_DM_BCR_IN          \
        in_dword(HWIO_MSS_UIM1_UART_DM_BCR_ADDR)
#define HWIO_MSS_UIM1_UART_DM_BCR_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM1_UART_DM_BCR_ADDR, m)
#define HWIO_MSS_UIM1_UART_DM_BCR_OUT(v)      \
        out_dword(HWIO_MSS_UIM1_UART_DM_BCR_ADDR,v)
#define HWIO_MSS_UIM1_UART_DM_BCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_UIM1_UART_DM_BCR_ADDR,m,v,HWIO_MSS_UIM1_UART_DM_BCR_IN)
#define HWIO_MSS_UIM1_UART_DM_BCR_IGNORE_CR_PROT_VIOL_BMSK                                                            0x40
#define HWIO_MSS_UIM1_UART_DM_BCR_IGNORE_CR_PROT_VIOL_SHFT                                                             0x6
#define HWIO_MSS_UIM1_UART_DM_BCR_RX_DMRX_1BYTE_RES_EN_BMSK                                                           0x20
#define HWIO_MSS_UIM1_UART_DM_BCR_RX_DMRX_1BYTE_RES_EN_SHFT                                                            0x5
#define HWIO_MSS_UIM1_UART_DM_BCR_RX_STALE_IRQ_DMRX_EQUAL_BMSK                                                        0x10
#define HWIO_MSS_UIM1_UART_DM_BCR_RX_STALE_IRQ_DMRX_EQUAL_SHFT                                                         0x4
#define HWIO_MSS_UIM1_UART_DM_BCR_RX_DMRX_LOW_EN_BMSK                                                                  0x4
#define HWIO_MSS_UIM1_UART_DM_BCR_RX_DMRX_LOW_EN_SHFT                                                                  0x2
#define HWIO_MSS_UIM1_UART_DM_BCR_STALE_IRQ_EMPTY_BMSK                                                                 0x2
#define HWIO_MSS_UIM1_UART_DM_BCR_STALE_IRQ_EMPTY_SHFT                                                                 0x1
#define HWIO_MSS_UIM1_UART_DM_BCR_TX_BREAK_DISABLE_BMSK                                                                0x1
#define HWIO_MSS_UIM1_UART_DM_BCR_TX_BREAK_DISABLE_SHFT                                                                0x0

#define HWIO_MSS_UIM1_UART_DM_RX_TRANS_CTRL_ADDR                                                                (MSS_UIM1_UART_DM_REG_BASE      + 0x000000cc)
#define HWIO_MSS_UIM1_UART_DM_RX_TRANS_CTRL_RMSK                                                                       0x7
#define HWIO_MSS_UIM1_UART_DM_RX_TRANS_CTRL_IN          \
        in_dword(HWIO_MSS_UIM1_UART_DM_RX_TRANS_CTRL_ADDR)
#define HWIO_MSS_UIM1_UART_DM_RX_TRANS_CTRL_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM1_UART_DM_RX_TRANS_CTRL_ADDR, m)
#define HWIO_MSS_UIM1_UART_DM_RX_TRANS_CTRL_OUT(v)      \
        out_dword(HWIO_MSS_UIM1_UART_DM_RX_TRANS_CTRL_ADDR,v)
#define HWIO_MSS_UIM1_UART_DM_RX_TRANS_CTRL_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_UIM1_UART_DM_RX_TRANS_CTRL_ADDR,m,v,HWIO_MSS_UIM1_UART_DM_RX_TRANS_CTRL_IN)
#define HWIO_MSS_UIM1_UART_DM_RX_TRANS_CTRL_RX_DMRX_CYCLIC_EN_BMSK                                                     0x4
#define HWIO_MSS_UIM1_UART_DM_RX_TRANS_CTRL_RX_DMRX_CYCLIC_EN_SHFT                                                     0x2
#define HWIO_MSS_UIM1_UART_DM_RX_TRANS_CTRL_RX_TRANS_AUTO_RE_ACTIVATE_BMSK                                             0x2
#define HWIO_MSS_UIM1_UART_DM_RX_TRANS_CTRL_RX_TRANS_AUTO_RE_ACTIVATE_SHFT                                             0x1
#define HWIO_MSS_UIM1_UART_DM_RX_TRANS_CTRL_RX_STALE_AUTO_RE_EN_BMSK                                                   0x1
#define HWIO_MSS_UIM1_UART_DM_RX_TRANS_CTRL_RX_STALE_AUTO_RE_EN_SHFT                                                   0x0

#define HWIO_MSS_UIM1_UART_DM_DMRX_DBG_ADDR                                                                     (MSS_UIM1_UART_DM_REG_BASE      + 0x000000d0)
#define HWIO_MSS_UIM1_UART_DM_DMRX_DBG_RMSK                                                                      0x1ffffff
#define HWIO_MSS_UIM1_UART_DM_DMRX_DBG_IN          \
        in_dword(HWIO_MSS_UIM1_UART_DM_DMRX_DBG_ADDR)
#define HWIO_MSS_UIM1_UART_DM_DMRX_DBG_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM1_UART_DM_DMRX_DBG_ADDR, m)
#define HWIO_MSS_UIM1_UART_DM_DMRX_DBG_UART_DM_DMRX_VAL_BMSK                                                     0x1ffffff
#define HWIO_MSS_UIM1_UART_DM_DMRX_DBG_UART_DM_DMRX_VAL_SHFT                                                           0x0

#define HWIO_MSS_UIM1_UART_DM_FSM_STATUS_ADDR                                                                   (MSS_UIM1_UART_DM_REG_BASE      + 0x000000d4)
#define HWIO_MSS_UIM1_UART_DM_FSM_STATUS_RMSK                                                                   0x37377771
#define HWIO_MSS_UIM1_UART_DM_FSM_STATUS_IN          \
        in_dword(HWIO_MSS_UIM1_UART_DM_FSM_STATUS_ADDR)
#define HWIO_MSS_UIM1_UART_DM_FSM_STATUS_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM1_UART_DM_FSM_STATUS_ADDR, m)
#define HWIO_MSS_UIM1_UART_DM_FSM_STATUS_TX_COMP_FSM_BMSK                                                       0x30000000
#define HWIO_MSS_UIM1_UART_DM_FSM_STATUS_TX_COMP_FSM_SHFT                                                             0x1c
#define HWIO_MSS_UIM1_UART_DM_FSM_STATUS_RX_PACK_FSM_BMSK                                                        0x7000000
#define HWIO_MSS_UIM1_UART_DM_FSM_STATUS_RX_PACK_FSM_SHFT                                                             0x18
#define HWIO_MSS_UIM1_UART_DM_FSM_STATUS_RX_TRANS_FSM_BMSK                                                        0x300000
#define HWIO_MSS_UIM1_UART_DM_FSM_STATUS_RX_TRANS_FSM_SHFT                                                            0x14
#define HWIO_MSS_UIM1_UART_DM_FSM_STATUS_TX_TRANS_FSM_BMSK                                                         0x70000
#define HWIO_MSS_UIM1_UART_DM_FSM_STATUS_TX_TRANS_FSM_SHFT                                                            0x10
#define HWIO_MSS_UIM1_UART_DM_FSM_STATUS_RX_PRO_TRANS_END_FSM_BMSK                                                  0x7000
#define HWIO_MSS_UIM1_UART_DM_FSM_STATUS_RX_PRO_TRANS_END_FSM_SHFT                                                     0xc
#define HWIO_MSS_UIM1_UART_DM_FSM_STATUS_RX_PRO_ACTIVE_FSM_BMSK                                                      0x700
#define HWIO_MSS_UIM1_UART_DM_FSM_STATUS_RX_PRO_ACTIVE_FSM_SHFT                                                        0x8
#define HWIO_MSS_UIM1_UART_DM_FSM_STATUS_TX_CON_TRANS_END_FSM_BMSK                                                    0x70
#define HWIO_MSS_UIM1_UART_DM_FSM_STATUS_TX_CON_TRANS_END_FSM_SHFT                                                     0x4
#define HWIO_MSS_UIM1_UART_DM_FSM_STATUS_RX_TRANSFER_ACTIVE_BMSK                                                       0x1
#define HWIO_MSS_UIM1_UART_DM_FSM_STATUS_RX_TRANSFER_ACTIVE_SHFT                                                       0x0

#define HWIO_MSS_UIM1_UART_DM_HW_VERSION_ADDR                                                                   (MSS_UIM1_UART_DM_REG_BASE      + 0x000000d8)
#define HWIO_MSS_UIM1_UART_DM_HW_VERSION_RMSK                                                                   0xffffffff
#define HWIO_MSS_UIM1_UART_DM_HW_VERSION_IN          \
        in_dword(HWIO_MSS_UIM1_UART_DM_HW_VERSION_ADDR)
#define HWIO_MSS_UIM1_UART_DM_HW_VERSION_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM1_UART_DM_HW_VERSION_ADDR, m)
#define HWIO_MSS_UIM1_UART_DM_HW_VERSION_HW_VERSION_MAJOR_BMSK                                                  0xf0000000
#define HWIO_MSS_UIM1_UART_DM_HW_VERSION_HW_VERSION_MAJOR_SHFT                                                        0x1c
#define HWIO_MSS_UIM1_UART_DM_HW_VERSION_HW_VERSION_MINOR_BMSK                                                   0xfff0000
#define HWIO_MSS_UIM1_UART_DM_HW_VERSION_HW_VERSION_MINOR_SHFT                                                        0x10
#define HWIO_MSS_UIM1_UART_DM_HW_VERSION_HW_VERSION_STEP_BMSK                                                       0xffff
#define HWIO_MSS_UIM1_UART_DM_HW_VERSION_HW_VERSION_STEP_SHFT                                                          0x0

#define HWIO_MSS_UIM1_UART_DM_GENERICS_ADDR                                                                     (MSS_UIM1_UART_DM_REG_BASE      + 0x000000dc)
#define HWIO_MSS_UIM1_UART_DM_GENERICS_RMSK                                                                           0xff
#define HWIO_MSS_UIM1_UART_DM_GENERICS_IN          \
        in_dword(HWIO_MSS_UIM1_UART_DM_GENERICS_ADDR)
#define HWIO_MSS_UIM1_UART_DM_GENERICS_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM1_UART_DM_GENERICS_ADDR, m)
#define HWIO_MSS_UIM1_UART_DM_GENERICS_GENERIC_BAM_IFC_BMSK                                                           0x80
#define HWIO_MSS_UIM1_UART_DM_GENERICS_GENERIC_BAM_IFC_SHFT                                                            0x7
#define HWIO_MSS_UIM1_UART_DM_GENERICS_GENERIC_DM_IFC_BMSK                                                            0x40
#define HWIO_MSS_UIM1_UART_DM_GENERICS_GENERIC_DM_IFC_SHFT                                                             0x6
#define HWIO_MSS_UIM1_UART_DM_GENERICS_GENERIC_IRDA_IFC_BMSK                                                          0x20
#define HWIO_MSS_UIM1_UART_DM_GENERICS_GENERIC_IRDA_IFC_SHFT                                                           0x5
#define HWIO_MSS_UIM1_UART_DM_GENERICS_GENERIC_SIM_GLUE_BMSK                                                          0x10
#define HWIO_MSS_UIM1_UART_DM_GENERICS_GENERIC_SIM_GLUE_SHFT                                                           0x4
#define HWIO_MSS_UIM1_UART_DM_GENERICS_GENERIC_RAM_ADDR_WIDTH_BMSK                                                     0xf
#define HWIO_MSS_UIM1_UART_DM_GENERICS_GENERIC_RAM_ADDR_WIDTH_SHFT                                                     0x0

#define HWIO_MSS_UIM1_UART_DM_ISR_CLR_ADDR                                                                      (MSS_UIM1_UART_DM_REG_BASE      + 0x000000e0)
#define HWIO_MSS_UIM1_UART_DM_ISR_CLR_RMSK                                                                         0x20000
#define HWIO_MSS_UIM1_UART_DM_ISR_CLR_OUT(v)      \
        out_dword(HWIO_MSS_UIM1_UART_DM_ISR_CLR_ADDR,v)
#define HWIO_MSS_UIM1_UART_DM_ISR_CLR_NO_FINISH_CMD_VIOL_BMSK                                                      0x20000
#define HWIO_MSS_UIM1_UART_DM_ISR_CLR_NO_FINISH_CMD_VIOL_SHFT                                                         0x11

#define HWIO_MSS_UIM1_UART_DM_TF_ADDR                                                                           (MSS_UIM1_UART_DM_REG_BASE      + 0x00000100)
#define HWIO_MSS_UIM1_UART_DM_TF_RMSK                                                                           0xffffffff
#define HWIO_MSS_UIM1_UART_DM_TF_OUT(v)      \
        out_dword(HWIO_MSS_UIM1_UART_DM_TF_ADDR,v)
#define HWIO_MSS_UIM1_UART_DM_TF_UART_TF_BMSK                                                                   0xffffffff
#define HWIO_MSS_UIM1_UART_DM_TF_UART_TF_SHFT                                                                          0x0

#define HWIO_MSS_UIM1_UART_DM_TF_2_ADDR                                                                         (MSS_UIM1_UART_DM_REG_BASE      + 0x00000104)
#define HWIO_MSS_UIM1_UART_DM_TF_2_RMSK                                                                         0xffffffff
#define HWIO_MSS_UIM1_UART_DM_TF_2_OUT(v)      \
        out_dword(HWIO_MSS_UIM1_UART_DM_TF_2_ADDR,v)
#define HWIO_MSS_UIM1_UART_DM_TF_2_UART_TF_BMSK                                                                 0xffffffff
#define HWIO_MSS_UIM1_UART_DM_TF_2_UART_TF_SHFT                                                                        0x0

#define HWIO_MSS_UIM1_UART_DM_TF_3_ADDR                                                                         (MSS_UIM1_UART_DM_REG_BASE      + 0x00000108)
#define HWIO_MSS_UIM1_UART_DM_TF_3_RMSK                                                                         0xffffffff
#define HWIO_MSS_UIM1_UART_DM_TF_3_OUT(v)      \
        out_dword(HWIO_MSS_UIM1_UART_DM_TF_3_ADDR,v)
#define HWIO_MSS_UIM1_UART_DM_TF_3_UART_TF_BMSK                                                                 0xffffffff
#define HWIO_MSS_UIM1_UART_DM_TF_3_UART_TF_SHFT                                                                        0x0

#define HWIO_MSS_UIM1_UART_DM_TF_4_ADDR                                                                         (MSS_UIM1_UART_DM_REG_BASE      + 0x0000010c)
#define HWIO_MSS_UIM1_UART_DM_TF_4_RMSK                                                                         0xffffffff
#define HWIO_MSS_UIM1_UART_DM_TF_4_OUT(v)      \
        out_dword(HWIO_MSS_UIM1_UART_DM_TF_4_ADDR,v)
#define HWIO_MSS_UIM1_UART_DM_TF_4_UART_TF_BMSK                                                                 0xffffffff
#define HWIO_MSS_UIM1_UART_DM_TF_4_UART_TF_SHFT                                                                        0x0

#define HWIO_MSS_UIM1_UART_DM_TF_5_ADDR                                                                         (MSS_UIM1_UART_DM_REG_BASE      + 0x00000110)
#define HWIO_MSS_UIM1_UART_DM_TF_5_RMSK                                                                         0xffffffff
#define HWIO_MSS_UIM1_UART_DM_TF_5_OUT(v)      \
        out_dword(HWIO_MSS_UIM1_UART_DM_TF_5_ADDR,v)
#define HWIO_MSS_UIM1_UART_DM_TF_5_UART_TF_BMSK                                                                 0xffffffff
#define HWIO_MSS_UIM1_UART_DM_TF_5_UART_TF_SHFT                                                                        0x0

#define HWIO_MSS_UIM1_UART_DM_TF_6_ADDR                                                                         (MSS_UIM1_UART_DM_REG_BASE      + 0x00000114)
#define HWIO_MSS_UIM1_UART_DM_TF_6_RMSK                                                                         0xffffffff
#define HWIO_MSS_UIM1_UART_DM_TF_6_OUT(v)      \
        out_dword(HWIO_MSS_UIM1_UART_DM_TF_6_ADDR,v)
#define HWIO_MSS_UIM1_UART_DM_TF_6_UART_TF_BMSK                                                                 0xffffffff
#define HWIO_MSS_UIM1_UART_DM_TF_6_UART_TF_SHFT                                                                        0x0

#define HWIO_MSS_UIM1_UART_DM_TF_7_ADDR                                                                         (MSS_UIM1_UART_DM_REG_BASE      + 0x00000118)
#define HWIO_MSS_UIM1_UART_DM_TF_7_RMSK                                                                         0xffffffff
#define HWIO_MSS_UIM1_UART_DM_TF_7_OUT(v)      \
        out_dword(HWIO_MSS_UIM1_UART_DM_TF_7_ADDR,v)
#define HWIO_MSS_UIM1_UART_DM_TF_7_UART_TF_BMSK                                                                 0xffffffff
#define HWIO_MSS_UIM1_UART_DM_TF_7_UART_TF_SHFT                                                                        0x0

#define HWIO_MSS_UIM1_UART_DM_TF_8_ADDR                                                                         (MSS_UIM1_UART_DM_REG_BASE      + 0x0000011c)
#define HWIO_MSS_UIM1_UART_DM_TF_8_RMSK                                                                         0xffffffff
#define HWIO_MSS_UIM1_UART_DM_TF_8_OUT(v)      \
        out_dword(HWIO_MSS_UIM1_UART_DM_TF_8_ADDR,v)
#define HWIO_MSS_UIM1_UART_DM_TF_8_UART_TF_BMSK                                                                 0xffffffff
#define HWIO_MSS_UIM1_UART_DM_TF_8_UART_TF_SHFT                                                                        0x0

#define HWIO_MSS_UIM1_UART_DM_TF_9_ADDR                                                                         (MSS_UIM1_UART_DM_REG_BASE      + 0x00000120)
#define HWIO_MSS_UIM1_UART_DM_TF_9_RMSK                                                                         0xffffffff
#define HWIO_MSS_UIM1_UART_DM_TF_9_OUT(v)      \
        out_dword(HWIO_MSS_UIM1_UART_DM_TF_9_ADDR,v)
#define HWIO_MSS_UIM1_UART_DM_TF_9_UART_TF_BMSK                                                                 0xffffffff
#define HWIO_MSS_UIM1_UART_DM_TF_9_UART_TF_SHFT                                                                        0x0

#define HWIO_MSS_UIM1_UART_DM_TF_10_ADDR                                                                        (MSS_UIM1_UART_DM_REG_BASE      + 0x00000124)
#define HWIO_MSS_UIM1_UART_DM_TF_10_RMSK                                                                        0xffffffff
#define HWIO_MSS_UIM1_UART_DM_TF_10_OUT(v)      \
        out_dword(HWIO_MSS_UIM1_UART_DM_TF_10_ADDR,v)
#define HWIO_MSS_UIM1_UART_DM_TF_10_UART_TF_BMSK                                                                0xffffffff
#define HWIO_MSS_UIM1_UART_DM_TF_10_UART_TF_SHFT                                                                       0x0

#define HWIO_MSS_UIM1_UART_DM_TF_11_ADDR                                                                        (MSS_UIM1_UART_DM_REG_BASE      + 0x00000128)
#define HWIO_MSS_UIM1_UART_DM_TF_11_RMSK                                                                        0xffffffff
#define HWIO_MSS_UIM1_UART_DM_TF_11_OUT(v)      \
        out_dword(HWIO_MSS_UIM1_UART_DM_TF_11_ADDR,v)
#define HWIO_MSS_UIM1_UART_DM_TF_11_UART_TF_BMSK                                                                0xffffffff
#define HWIO_MSS_UIM1_UART_DM_TF_11_UART_TF_SHFT                                                                       0x0

#define HWIO_MSS_UIM1_UART_DM_TF_12_ADDR                                                                        (MSS_UIM1_UART_DM_REG_BASE      + 0x0000012c)
#define HWIO_MSS_UIM1_UART_DM_TF_12_RMSK                                                                        0xffffffff
#define HWIO_MSS_UIM1_UART_DM_TF_12_OUT(v)      \
        out_dword(HWIO_MSS_UIM1_UART_DM_TF_12_ADDR,v)
#define HWIO_MSS_UIM1_UART_DM_TF_12_UART_TF_BMSK                                                                0xffffffff
#define HWIO_MSS_UIM1_UART_DM_TF_12_UART_TF_SHFT                                                                       0x0

#define HWIO_MSS_UIM1_UART_DM_TF_13_ADDR                                                                        (MSS_UIM1_UART_DM_REG_BASE      + 0x00000130)
#define HWIO_MSS_UIM1_UART_DM_TF_13_RMSK                                                                        0xffffffff
#define HWIO_MSS_UIM1_UART_DM_TF_13_OUT(v)      \
        out_dword(HWIO_MSS_UIM1_UART_DM_TF_13_ADDR,v)
#define HWIO_MSS_UIM1_UART_DM_TF_13_UART_TF_BMSK                                                                0xffffffff
#define HWIO_MSS_UIM1_UART_DM_TF_13_UART_TF_SHFT                                                                       0x0

#define HWIO_MSS_UIM1_UART_DM_TF_14_ADDR                                                                        (MSS_UIM1_UART_DM_REG_BASE      + 0x00000134)
#define HWIO_MSS_UIM1_UART_DM_TF_14_RMSK                                                                        0xffffffff
#define HWIO_MSS_UIM1_UART_DM_TF_14_OUT(v)      \
        out_dword(HWIO_MSS_UIM1_UART_DM_TF_14_ADDR,v)
#define HWIO_MSS_UIM1_UART_DM_TF_14_UART_TF_BMSK                                                                0xffffffff
#define HWIO_MSS_UIM1_UART_DM_TF_14_UART_TF_SHFT                                                                       0x0

#define HWIO_MSS_UIM1_UART_DM_TF_15_ADDR                                                                        (MSS_UIM1_UART_DM_REG_BASE      + 0x00000138)
#define HWIO_MSS_UIM1_UART_DM_TF_15_RMSK                                                                        0xffffffff
#define HWIO_MSS_UIM1_UART_DM_TF_15_OUT(v)      \
        out_dword(HWIO_MSS_UIM1_UART_DM_TF_15_ADDR,v)
#define HWIO_MSS_UIM1_UART_DM_TF_15_UART_TF_BMSK                                                                0xffffffff
#define HWIO_MSS_UIM1_UART_DM_TF_15_UART_TF_SHFT                                                                       0x0

#define HWIO_MSS_UIM1_UART_DM_TF_16_ADDR                                                                        (MSS_UIM1_UART_DM_REG_BASE      + 0x0000013c)
#define HWIO_MSS_UIM1_UART_DM_TF_16_RMSK                                                                        0xffffffff
#define HWIO_MSS_UIM1_UART_DM_TF_16_OUT(v)      \
        out_dword(HWIO_MSS_UIM1_UART_DM_TF_16_ADDR,v)
#define HWIO_MSS_UIM1_UART_DM_TF_16_UART_TF_BMSK                                                                0xffffffff
#define HWIO_MSS_UIM1_UART_DM_TF_16_UART_TF_SHFT                                                                       0x0

#define HWIO_MSS_UIM1_UART_DM_RF_ADDR                                                                           (MSS_UIM1_UART_DM_REG_BASE      + 0x00000140)
#define HWIO_MSS_UIM1_UART_DM_RF_RMSK                                                                           0xffffffff
#define HWIO_MSS_UIM1_UART_DM_RF_IN          \
        in_dword(HWIO_MSS_UIM1_UART_DM_RF_ADDR)
#define HWIO_MSS_UIM1_UART_DM_RF_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM1_UART_DM_RF_ADDR, m)
#define HWIO_MSS_UIM1_UART_DM_RF_UART_RF_BMSK                                                                   0xffffffff
#define HWIO_MSS_UIM1_UART_DM_RF_UART_RF_SHFT                                                                          0x0

#define HWIO_MSS_UIM1_UART_DM_RF_2_ADDR                                                                         (MSS_UIM1_UART_DM_REG_BASE      + 0x00000144)
#define HWIO_MSS_UIM1_UART_DM_RF_2_RMSK                                                                         0xffffffff
#define HWIO_MSS_UIM1_UART_DM_RF_2_IN          \
        in_dword(HWIO_MSS_UIM1_UART_DM_RF_2_ADDR)
#define HWIO_MSS_UIM1_UART_DM_RF_2_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM1_UART_DM_RF_2_ADDR, m)
#define HWIO_MSS_UIM1_UART_DM_RF_2_UART_RF_BMSK                                                                 0xffffffff
#define HWIO_MSS_UIM1_UART_DM_RF_2_UART_RF_SHFT                                                                        0x0

#define HWIO_MSS_UIM1_UART_DM_RF_3_ADDR                                                                         (MSS_UIM1_UART_DM_REG_BASE      + 0x00000148)
#define HWIO_MSS_UIM1_UART_DM_RF_3_RMSK                                                                         0xffffffff
#define HWIO_MSS_UIM1_UART_DM_RF_3_IN          \
        in_dword(HWIO_MSS_UIM1_UART_DM_RF_3_ADDR)
#define HWIO_MSS_UIM1_UART_DM_RF_3_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM1_UART_DM_RF_3_ADDR, m)
#define HWIO_MSS_UIM1_UART_DM_RF_3_UART_RF_BMSK                                                                 0xffffffff
#define HWIO_MSS_UIM1_UART_DM_RF_3_UART_RF_SHFT                                                                        0x0

#define HWIO_MSS_UIM1_UART_DM_RF_4_ADDR                                                                         (MSS_UIM1_UART_DM_REG_BASE      + 0x0000014c)
#define HWIO_MSS_UIM1_UART_DM_RF_4_RMSK                                                                         0xffffffff
#define HWIO_MSS_UIM1_UART_DM_RF_4_IN          \
        in_dword(HWIO_MSS_UIM1_UART_DM_RF_4_ADDR)
#define HWIO_MSS_UIM1_UART_DM_RF_4_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM1_UART_DM_RF_4_ADDR, m)
#define HWIO_MSS_UIM1_UART_DM_RF_4_UART_RF_BMSK                                                                 0xffffffff
#define HWIO_MSS_UIM1_UART_DM_RF_4_UART_RF_SHFT                                                                        0x0

#define HWIO_MSS_UIM1_UART_DM_RF_5_ADDR                                                                         (MSS_UIM1_UART_DM_REG_BASE      + 0x00000150)
#define HWIO_MSS_UIM1_UART_DM_RF_5_RMSK                                                                         0xffffffff
#define HWIO_MSS_UIM1_UART_DM_RF_5_IN          \
        in_dword(HWIO_MSS_UIM1_UART_DM_RF_5_ADDR)
#define HWIO_MSS_UIM1_UART_DM_RF_5_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM1_UART_DM_RF_5_ADDR, m)
#define HWIO_MSS_UIM1_UART_DM_RF_5_UART_RF_BMSK                                                                 0xffffffff
#define HWIO_MSS_UIM1_UART_DM_RF_5_UART_RF_SHFT                                                                        0x0

#define HWIO_MSS_UIM1_UART_DM_RF_6_ADDR                                                                         (MSS_UIM1_UART_DM_REG_BASE      + 0x00000154)
#define HWIO_MSS_UIM1_UART_DM_RF_6_RMSK                                                                         0xffffffff
#define HWIO_MSS_UIM1_UART_DM_RF_6_IN          \
        in_dword(HWIO_MSS_UIM1_UART_DM_RF_6_ADDR)
#define HWIO_MSS_UIM1_UART_DM_RF_6_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM1_UART_DM_RF_6_ADDR, m)
#define HWIO_MSS_UIM1_UART_DM_RF_6_UART_RF_BMSK                                                                 0xffffffff
#define HWIO_MSS_UIM1_UART_DM_RF_6_UART_RF_SHFT                                                                        0x0

#define HWIO_MSS_UIM1_UART_DM_RF_7_ADDR                                                                         (MSS_UIM1_UART_DM_REG_BASE      + 0x00000158)
#define HWIO_MSS_UIM1_UART_DM_RF_7_RMSK                                                                         0xffffffff
#define HWIO_MSS_UIM1_UART_DM_RF_7_IN          \
        in_dword(HWIO_MSS_UIM1_UART_DM_RF_7_ADDR)
#define HWIO_MSS_UIM1_UART_DM_RF_7_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM1_UART_DM_RF_7_ADDR, m)
#define HWIO_MSS_UIM1_UART_DM_RF_7_UART_RF_BMSK                                                                 0xffffffff
#define HWIO_MSS_UIM1_UART_DM_RF_7_UART_RF_SHFT                                                                        0x0

#define HWIO_MSS_UIM1_UART_DM_RF_8_ADDR                                                                         (MSS_UIM1_UART_DM_REG_BASE      + 0x0000015c)
#define HWIO_MSS_UIM1_UART_DM_RF_8_RMSK                                                                         0xffffffff
#define HWIO_MSS_UIM1_UART_DM_RF_8_IN          \
        in_dword(HWIO_MSS_UIM1_UART_DM_RF_8_ADDR)
#define HWIO_MSS_UIM1_UART_DM_RF_8_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM1_UART_DM_RF_8_ADDR, m)
#define HWIO_MSS_UIM1_UART_DM_RF_8_UART_RF_BMSK                                                                 0xffffffff
#define HWIO_MSS_UIM1_UART_DM_RF_8_UART_RF_SHFT                                                                        0x0

#define HWIO_MSS_UIM1_UART_DM_RF_9_ADDR                                                                         (MSS_UIM1_UART_DM_REG_BASE      + 0x00000160)
#define HWIO_MSS_UIM1_UART_DM_RF_9_RMSK                                                                         0xffffffff
#define HWIO_MSS_UIM1_UART_DM_RF_9_IN          \
        in_dword(HWIO_MSS_UIM1_UART_DM_RF_9_ADDR)
#define HWIO_MSS_UIM1_UART_DM_RF_9_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM1_UART_DM_RF_9_ADDR, m)
#define HWIO_MSS_UIM1_UART_DM_RF_9_UART_RF_BMSK                                                                 0xffffffff
#define HWIO_MSS_UIM1_UART_DM_RF_9_UART_RF_SHFT                                                                        0x0

#define HWIO_MSS_UIM1_UART_DM_RF_10_ADDR                                                                        (MSS_UIM1_UART_DM_REG_BASE      + 0x00000164)
#define HWIO_MSS_UIM1_UART_DM_RF_10_RMSK                                                                        0xffffffff
#define HWIO_MSS_UIM1_UART_DM_RF_10_IN          \
        in_dword(HWIO_MSS_UIM1_UART_DM_RF_10_ADDR)
#define HWIO_MSS_UIM1_UART_DM_RF_10_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM1_UART_DM_RF_10_ADDR, m)
#define HWIO_MSS_UIM1_UART_DM_RF_10_UART_RF_BMSK                                                                0xffffffff
#define HWIO_MSS_UIM1_UART_DM_RF_10_UART_RF_SHFT                                                                       0x0

#define HWIO_MSS_UIM1_UART_DM_RF_11_ADDR                                                                        (MSS_UIM1_UART_DM_REG_BASE      + 0x00000168)
#define HWIO_MSS_UIM1_UART_DM_RF_11_RMSK                                                                        0xffffffff
#define HWIO_MSS_UIM1_UART_DM_RF_11_IN          \
        in_dword(HWIO_MSS_UIM1_UART_DM_RF_11_ADDR)
#define HWIO_MSS_UIM1_UART_DM_RF_11_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM1_UART_DM_RF_11_ADDR, m)
#define HWIO_MSS_UIM1_UART_DM_RF_11_UART_RF_BMSK                                                                0xffffffff
#define HWIO_MSS_UIM1_UART_DM_RF_11_UART_RF_SHFT                                                                       0x0

#define HWIO_MSS_UIM1_UART_DM_RF_12_ADDR                                                                        (MSS_UIM1_UART_DM_REG_BASE      + 0x0000016c)
#define HWIO_MSS_UIM1_UART_DM_RF_12_RMSK                                                                        0xffffffff
#define HWIO_MSS_UIM1_UART_DM_RF_12_IN          \
        in_dword(HWIO_MSS_UIM1_UART_DM_RF_12_ADDR)
#define HWIO_MSS_UIM1_UART_DM_RF_12_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM1_UART_DM_RF_12_ADDR, m)
#define HWIO_MSS_UIM1_UART_DM_RF_12_UART_RF_BMSK                                                                0xffffffff
#define HWIO_MSS_UIM1_UART_DM_RF_12_UART_RF_SHFT                                                                       0x0

#define HWIO_MSS_UIM1_UART_DM_RF_13_ADDR                                                                        (MSS_UIM1_UART_DM_REG_BASE      + 0x00000170)
#define HWIO_MSS_UIM1_UART_DM_RF_13_RMSK                                                                        0xffffffff
#define HWIO_MSS_UIM1_UART_DM_RF_13_IN          \
        in_dword(HWIO_MSS_UIM1_UART_DM_RF_13_ADDR)
#define HWIO_MSS_UIM1_UART_DM_RF_13_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM1_UART_DM_RF_13_ADDR, m)
#define HWIO_MSS_UIM1_UART_DM_RF_13_UART_RF_BMSK                                                                0xffffffff
#define HWIO_MSS_UIM1_UART_DM_RF_13_UART_RF_SHFT                                                                       0x0

#define HWIO_MSS_UIM1_UART_DM_RF_14_ADDR                                                                        (MSS_UIM1_UART_DM_REG_BASE      + 0x00000174)
#define HWIO_MSS_UIM1_UART_DM_RF_14_RMSK                                                                        0xffffffff
#define HWIO_MSS_UIM1_UART_DM_RF_14_IN          \
        in_dword(HWIO_MSS_UIM1_UART_DM_RF_14_ADDR)
#define HWIO_MSS_UIM1_UART_DM_RF_14_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM1_UART_DM_RF_14_ADDR, m)
#define HWIO_MSS_UIM1_UART_DM_RF_14_UART_RF_BMSK                                                                0xffffffff
#define HWIO_MSS_UIM1_UART_DM_RF_14_UART_RF_SHFT                                                                       0x0

#define HWIO_MSS_UIM1_UART_DM_RF_15_ADDR                                                                        (MSS_UIM1_UART_DM_REG_BASE      + 0x00000178)
#define HWIO_MSS_UIM1_UART_DM_RF_15_RMSK                                                                        0xffffffff
#define HWIO_MSS_UIM1_UART_DM_RF_15_IN          \
        in_dword(HWIO_MSS_UIM1_UART_DM_RF_15_ADDR)
#define HWIO_MSS_UIM1_UART_DM_RF_15_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM1_UART_DM_RF_15_ADDR, m)
#define HWIO_MSS_UIM1_UART_DM_RF_15_UART_RF_BMSK                                                                0xffffffff
#define HWIO_MSS_UIM1_UART_DM_RF_15_UART_RF_SHFT                                                                       0x0

#define HWIO_MSS_UIM1_UART_DM_RF_16_ADDR                                                                        (MSS_UIM1_UART_DM_REG_BASE      + 0x0000017c)
#define HWIO_MSS_UIM1_UART_DM_RF_16_RMSK                                                                        0xffffffff
#define HWIO_MSS_UIM1_UART_DM_RF_16_IN          \
        in_dword(HWIO_MSS_UIM1_UART_DM_RF_16_ADDR)
#define HWIO_MSS_UIM1_UART_DM_RF_16_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM1_UART_DM_RF_16_ADDR, m)
#define HWIO_MSS_UIM1_UART_DM_RF_16_UART_RF_BMSK                                                                0xffffffff
#define HWIO_MSS_UIM1_UART_DM_RF_16_UART_RF_SHFT                                                                       0x0

#define HWIO_MSS_UIM1_UART_DM_UIM_CFG_ADDR                                                                      (MSS_UIM1_UART_DM_REG_BASE      + 0x00000180)
#define HWIO_MSS_UIM1_UART_DM_UIM_CFG_RMSK                                                                          0xf77f
#define HWIO_MSS_UIM1_UART_DM_UIM_CFG_IN          \
        in_dword(HWIO_MSS_UIM1_UART_DM_UIM_CFG_ADDR)
#define HWIO_MSS_UIM1_UART_DM_UIM_CFG_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM1_UART_DM_UIM_CFG_ADDR, m)
#define HWIO_MSS_UIM1_UART_DM_UIM_CFG_OUT(v)      \
        out_dword(HWIO_MSS_UIM1_UART_DM_UIM_CFG_ADDR,v)
#define HWIO_MSS_UIM1_UART_DM_UIM_CFG_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_UIM1_UART_DM_UIM_CFG_ADDR,m,v,HWIO_MSS_UIM1_UART_DM_UIM_CFG_IN)
#define HWIO_MSS_UIM1_UART_DM_UIM_CFG_BATT_ALARM_QUICK_DROP_EN_BMSK                                                 0x8000
#define HWIO_MSS_UIM1_UART_DM_UIM_CFG_BATT_ALARM_QUICK_DROP_EN_SHFT                                                    0xf
#define HWIO_MSS_UIM1_UART_DM_UIM_CFG_TESTBUS_EN_BMSK                                                               0x4000
#define HWIO_MSS_UIM1_UART_DM_UIM_CFG_TESTBUS_EN_SHFT                                                                  0xe
#define HWIO_MSS_UIM1_UART_DM_UIM_CFG_SW_RESET_BMSK                                                                 0x2000
#define HWIO_MSS_UIM1_UART_DM_UIM_CFG_SW_RESET_SHFT                                                                    0xd
#define HWIO_MSS_UIM1_UART_DM_UIM_CFG_MODE18_BMSK                                                                   0x1000
#define HWIO_MSS_UIM1_UART_DM_UIM_CFG_MODE18_SHFT                                                                      0xc
#define HWIO_MSS_UIM1_UART_DM_UIM_CFG_PMIC_ALARM_EN_BMSK                                                             0x400
#define HWIO_MSS_UIM1_UART_DM_UIM_CFG_PMIC_ALARM_EN_SHFT                                                               0xa
#define HWIO_MSS_UIM1_UART_DM_UIM_CFG_BATT_ALARM_TRIGGER_EN_BMSK                                                     0x200
#define HWIO_MSS_UIM1_UART_DM_UIM_CFG_BATT_ALARM_TRIGGER_EN_SHFT                                                       0x9
#define HWIO_MSS_UIM1_UART_DM_UIM_CFG_UIM_RMV_TRIGGER_EN_BMSK                                                        0x100
#define HWIO_MSS_UIM1_UART_DM_UIM_CFG_UIM_RMV_TRIGGER_EN_SHFT                                                          0x8
#define HWIO_MSS_UIM1_UART_DM_UIM_CFG_UIM_CARD_EVENTS_ENABLE_BMSK                                                     0x40
#define HWIO_MSS_UIM1_UART_DM_UIM_CFG_UIM_CARD_EVENTS_ENABLE_SHFT                                                      0x6
#define HWIO_MSS_UIM1_UART_DM_UIM_CFG_UIM_PRESENT_POLARITY_BMSK                                                       0x20
#define HWIO_MSS_UIM1_UART_DM_UIM_CFG_UIM_PRESENT_POLARITY_SHFT                                                        0x5
#define HWIO_MSS_UIM1_UART_DM_UIM_CFG_EVENT_DEBOUNCE_TIME_BMSK                                                        0x1f
#define HWIO_MSS_UIM1_UART_DM_UIM_CFG_EVENT_DEBOUNCE_TIME_SHFT                                                         0x0

#define HWIO_MSS_UIM1_UART_DM_UIM_CMD_ADDR                                                                      (MSS_UIM1_UART_DM_REG_BASE      + 0x00000184)
#define HWIO_MSS_UIM1_UART_DM_UIM_CMD_RMSK                                                                             0x3
#define HWIO_MSS_UIM1_UART_DM_UIM_CMD_OUT(v)      \
        out_dword(HWIO_MSS_UIM1_UART_DM_UIM_CMD_ADDR,v)
#define HWIO_MSS_UIM1_UART_DM_UIM_CMD_RECOVER_FROM_HW_DEACTIVATION_BMSK                                                0x2
#define HWIO_MSS_UIM1_UART_DM_UIM_CMD_RECOVER_FROM_HW_DEACTIVATION_SHFT                                                0x1
#define HWIO_MSS_UIM1_UART_DM_UIM_CMD_INITIATE_HW_DEACTIVATION_BMSK                                                    0x1
#define HWIO_MSS_UIM1_UART_DM_UIM_CMD_INITIATE_HW_DEACTIVATION_SHFT                                                    0x0

#define HWIO_MSS_UIM1_UART_DM_UIM_IO_STATUS_ADDR                                                                (MSS_UIM1_UART_DM_REG_BASE      + 0x00000188)
#define HWIO_MSS_UIM1_UART_DM_UIM_IO_STATUS_RMSK                                                                       0x7
#define HWIO_MSS_UIM1_UART_DM_UIM_IO_STATUS_IN          \
        in_dword(HWIO_MSS_UIM1_UART_DM_UIM_IO_STATUS_ADDR)
#define HWIO_MSS_UIM1_UART_DM_UIM_IO_STATUS_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM1_UART_DM_UIM_IO_STATUS_ADDR, m)
#define HWIO_MSS_UIM1_UART_DM_UIM_IO_STATUS_UIM_IO_WRITE_IN_PROGRESS_BMSK                                              0x4
#define HWIO_MSS_UIM1_UART_DM_UIM_IO_STATUS_UIM_IO_WRITE_IN_PROGRESS_SHFT                                              0x2
#define HWIO_MSS_UIM1_UART_DM_UIM_IO_STATUS_UIM_DEACTIVATION_STATUS_BMSK                                               0x2
#define HWIO_MSS_UIM1_UART_DM_UIM_IO_STATUS_UIM_DEACTIVATION_STATUS_SHFT                                               0x1
#define HWIO_MSS_UIM1_UART_DM_UIM_IO_STATUS_CARD_PRESENCE_BMSK                                                         0x1
#define HWIO_MSS_UIM1_UART_DM_UIM_IO_STATUS_CARD_PRESENCE_SHFT                                                         0x0

#define HWIO_MSS_UIM1_UART_DM_UIM_IRQ_ISR_ADDR                                                                  (MSS_UIM1_UART_DM_REG_BASE      + 0x0000018c)
#define HWIO_MSS_UIM1_UART_DM_UIM_IRQ_ISR_RMSK                                                                        0x1f
#define HWIO_MSS_UIM1_UART_DM_UIM_IRQ_ISR_IN          \
        in_dword(HWIO_MSS_UIM1_UART_DM_UIM_IRQ_ISR_ADDR)
#define HWIO_MSS_UIM1_UART_DM_UIM_IRQ_ISR_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM1_UART_DM_UIM_IRQ_ISR_ADDR, m)
#define HWIO_MSS_UIM1_UART_DM_UIM_IRQ_ISR_UIM_IO_WRITE_DONE_BMSK                                                      0x10
#define HWIO_MSS_UIM1_UART_DM_UIM_IRQ_ISR_UIM_IO_WRITE_DONE_SHFT                                                       0x4
#define HWIO_MSS_UIM1_UART_DM_UIM_IRQ_ISR_HW_SEQUENCE_FINISH_BMSK                                                      0x8
#define HWIO_MSS_UIM1_UART_DM_UIM_IRQ_ISR_HW_SEQUENCE_FINISH_SHFT                                                      0x3
#define HWIO_MSS_UIM1_UART_DM_UIM_IRQ_ISR_BATT_ALARM_BMSK                                                              0x4
#define HWIO_MSS_UIM1_UART_DM_UIM_IRQ_ISR_BATT_ALARM_SHFT                                                              0x2
#define HWIO_MSS_UIM1_UART_DM_UIM_IRQ_ISR_UIM_CARD_INSERTION_BMSK                                                      0x2
#define HWIO_MSS_UIM1_UART_DM_UIM_IRQ_ISR_UIM_CARD_INSERTION_SHFT                                                      0x1
#define HWIO_MSS_UIM1_UART_DM_UIM_IRQ_ISR_UIM_CARD_REMOVAL_BMSK                                                        0x1
#define HWIO_MSS_UIM1_UART_DM_UIM_IRQ_ISR_UIM_CARD_REMOVAL_SHFT                                                        0x0

#define HWIO_MSS_UIM1_UART_DM_UIM_IRQ_MISR_ADDR                                                                 (MSS_UIM1_UART_DM_REG_BASE      + 0x00000190)
#define HWIO_MSS_UIM1_UART_DM_UIM_IRQ_MISR_RMSK                                                                       0x1f
#define HWIO_MSS_UIM1_UART_DM_UIM_IRQ_MISR_IN          \
        in_dword(HWIO_MSS_UIM1_UART_DM_UIM_IRQ_MISR_ADDR)
#define HWIO_MSS_UIM1_UART_DM_UIM_IRQ_MISR_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM1_UART_DM_UIM_IRQ_MISR_ADDR, m)
#define HWIO_MSS_UIM1_UART_DM_UIM_IRQ_MISR_UIM_IO_WRITE_DONE_BMSK                                                     0x10
#define HWIO_MSS_UIM1_UART_DM_UIM_IRQ_MISR_UIM_IO_WRITE_DONE_SHFT                                                      0x4
#define HWIO_MSS_UIM1_UART_DM_UIM_IRQ_MISR_HW_SEQUENCE_FINISH_BMSK                                                     0x8
#define HWIO_MSS_UIM1_UART_DM_UIM_IRQ_MISR_HW_SEQUENCE_FINISH_SHFT                                                     0x3
#define HWIO_MSS_UIM1_UART_DM_UIM_IRQ_MISR_BATT_ALARM_BMSK                                                             0x4
#define HWIO_MSS_UIM1_UART_DM_UIM_IRQ_MISR_BATT_ALARM_SHFT                                                             0x2
#define HWIO_MSS_UIM1_UART_DM_UIM_IRQ_MISR_UIM_CARD_INSERTION_BMSK                                                     0x2
#define HWIO_MSS_UIM1_UART_DM_UIM_IRQ_MISR_UIM_CARD_INSERTION_SHFT                                                     0x1
#define HWIO_MSS_UIM1_UART_DM_UIM_IRQ_MISR_UIM_CARD_REMOVAL_BMSK                                                       0x1
#define HWIO_MSS_UIM1_UART_DM_UIM_IRQ_MISR_UIM_CARD_REMOVAL_SHFT                                                       0x0

#define HWIO_MSS_UIM1_UART_DM_UIM_IRQ_CLR_ADDR                                                                  (MSS_UIM1_UART_DM_REG_BASE      + 0x00000194)
#define HWIO_MSS_UIM1_UART_DM_UIM_IRQ_CLR_RMSK                                                                        0x1f
#define HWIO_MSS_UIM1_UART_DM_UIM_IRQ_CLR_OUT(v)      \
        out_dword(HWIO_MSS_UIM1_UART_DM_UIM_IRQ_CLR_ADDR,v)
#define HWIO_MSS_UIM1_UART_DM_UIM_IRQ_CLR_UIM_IO_WRITE_DONE_BMSK                                                      0x10
#define HWIO_MSS_UIM1_UART_DM_UIM_IRQ_CLR_UIM_IO_WRITE_DONE_SHFT                                                       0x4
#define HWIO_MSS_UIM1_UART_DM_UIM_IRQ_CLR_HW_SEQUENCE_FINISH_BMSK                                                      0x8
#define HWIO_MSS_UIM1_UART_DM_UIM_IRQ_CLR_HW_SEQUENCE_FINISH_SHFT                                                      0x3
#define HWIO_MSS_UIM1_UART_DM_UIM_IRQ_CLR_BATT_ALARM_BMSK                                                              0x4
#define HWIO_MSS_UIM1_UART_DM_UIM_IRQ_CLR_BATT_ALARM_SHFT                                                              0x2
#define HWIO_MSS_UIM1_UART_DM_UIM_IRQ_CLR_UIM_CARD_INSERTION_BMSK                                                      0x2
#define HWIO_MSS_UIM1_UART_DM_UIM_IRQ_CLR_UIM_CARD_INSERTION_SHFT                                                      0x1
#define HWIO_MSS_UIM1_UART_DM_UIM_IRQ_CLR_UIM_CARD_REMOVAL_BMSK                                                        0x1
#define HWIO_MSS_UIM1_UART_DM_UIM_IRQ_CLR_UIM_CARD_REMOVAL_SHFT                                                        0x0

#define HWIO_MSS_UIM1_UART_DM_UIM_IRQ_IMR_ADDR                                                                  (MSS_UIM1_UART_DM_REG_BASE      + 0x00000198)
#define HWIO_MSS_UIM1_UART_DM_UIM_IRQ_IMR_RMSK                                                                        0x1f
#define HWIO_MSS_UIM1_UART_DM_UIM_IRQ_IMR_IN          \
        in_dword(HWIO_MSS_UIM1_UART_DM_UIM_IRQ_IMR_ADDR)
#define HWIO_MSS_UIM1_UART_DM_UIM_IRQ_IMR_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM1_UART_DM_UIM_IRQ_IMR_ADDR, m)
#define HWIO_MSS_UIM1_UART_DM_UIM_IRQ_IMR_OUT(v)      \
        out_dword(HWIO_MSS_UIM1_UART_DM_UIM_IRQ_IMR_ADDR,v)
#define HWIO_MSS_UIM1_UART_DM_UIM_IRQ_IMR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_UIM1_UART_DM_UIM_IRQ_IMR_ADDR,m,v,HWIO_MSS_UIM1_UART_DM_UIM_IRQ_IMR_IN)
#define HWIO_MSS_UIM1_UART_DM_UIM_IRQ_IMR_UIM_IO_WRITE_DONE_BMSK                                                      0x10
#define HWIO_MSS_UIM1_UART_DM_UIM_IRQ_IMR_UIM_IO_WRITE_DONE_SHFT                                                       0x4
#define HWIO_MSS_UIM1_UART_DM_UIM_IRQ_IMR_HW_SEQUENCE_FINISH_BMSK                                                      0x8
#define HWIO_MSS_UIM1_UART_DM_UIM_IRQ_IMR_HW_SEQUENCE_FINISH_SHFT                                                      0x3
#define HWIO_MSS_UIM1_UART_DM_UIM_IRQ_IMR_BATT_ALARM_BMSK                                                              0x4
#define HWIO_MSS_UIM1_UART_DM_UIM_IRQ_IMR_BATT_ALARM_SHFT                                                              0x2
#define HWIO_MSS_UIM1_UART_DM_UIM_IRQ_IMR_UIM_CARD_INSERTION_BMSK                                                      0x2
#define HWIO_MSS_UIM1_UART_DM_UIM_IRQ_IMR_UIM_CARD_INSERTION_SHFT                                                      0x1
#define HWIO_MSS_UIM1_UART_DM_UIM_IRQ_IMR_UIM_CARD_REMOVAL_BMSK                                                        0x1
#define HWIO_MSS_UIM1_UART_DM_UIM_IRQ_IMR_UIM_CARD_REMOVAL_SHFT                                                        0x0

#define HWIO_MSS_UIM1_UART_DM_UIM_IRQ_IMR_SET_ADDR                                                              (MSS_UIM1_UART_DM_REG_BASE      + 0x0000019c)
#define HWIO_MSS_UIM1_UART_DM_UIM_IRQ_IMR_SET_RMSK                                                                    0x1f
#define HWIO_MSS_UIM1_UART_DM_UIM_IRQ_IMR_SET_OUT(v)      \
        out_dword(HWIO_MSS_UIM1_UART_DM_UIM_IRQ_IMR_SET_ADDR,v)
#define HWIO_MSS_UIM1_UART_DM_UIM_IRQ_IMR_SET_UIM_IO_WRITE_DONE_BMSK                                                  0x10
#define HWIO_MSS_UIM1_UART_DM_UIM_IRQ_IMR_SET_UIM_IO_WRITE_DONE_SHFT                                                   0x4
#define HWIO_MSS_UIM1_UART_DM_UIM_IRQ_IMR_SET_HW_SEQUENCE_FINISH_BMSK                                                  0x8
#define HWIO_MSS_UIM1_UART_DM_UIM_IRQ_IMR_SET_HW_SEQUENCE_FINISH_SHFT                                                  0x3
#define HWIO_MSS_UIM1_UART_DM_UIM_IRQ_IMR_SET_BATT_ALARM_BMSK                                                          0x4
#define HWIO_MSS_UIM1_UART_DM_UIM_IRQ_IMR_SET_BATT_ALARM_SHFT                                                          0x2
#define HWIO_MSS_UIM1_UART_DM_UIM_IRQ_IMR_SET_UIM_CARD_INSERTION_BMSK                                                  0x2
#define HWIO_MSS_UIM1_UART_DM_UIM_IRQ_IMR_SET_UIM_CARD_INSERTION_SHFT                                                  0x1
#define HWIO_MSS_UIM1_UART_DM_UIM_IRQ_IMR_SET_UIM_CARD_REMOVAL_BMSK                                                    0x1
#define HWIO_MSS_UIM1_UART_DM_UIM_IRQ_IMR_SET_UIM_CARD_REMOVAL_SHFT                                                    0x0

#define HWIO_MSS_UIM1_UART_DM_UIM_IRQ_IMR_CLR_ADDR                                                              (MSS_UIM1_UART_DM_REG_BASE      + 0x000001a0)
#define HWIO_MSS_UIM1_UART_DM_UIM_IRQ_IMR_CLR_RMSK                                                                    0x1f
#define HWIO_MSS_UIM1_UART_DM_UIM_IRQ_IMR_CLR_OUT(v)      \
        out_dword(HWIO_MSS_UIM1_UART_DM_UIM_IRQ_IMR_CLR_ADDR,v)
#define HWIO_MSS_UIM1_UART_DM_UIM_IRQ_IMR_CLR_UIM_IO_WRITE_DONE_BMSK                                                  0x10
#define HWIO_MSS_UIM1_UART_DM_UIM_IRQ_IMR_CLR_UIM_IO_WRITE_DONE_SHFT                                                   0x4
#define HWIO_MSS_UIM1_UART_DM_UIM_IRQ_IMR_CLR_HW_SEQUENCE_FINISH_BMSK                                                  0x8
#define HWIO_MSS_UIM1_UART_DM_UIM_IRQ_IMR_CLR_HW_SEQUENCE_FINISH_SHFT                                                  0x3
#define HWIO_MSS_UIM1_UART_DM_UIM_IRQ_IMR_CLR_BATT_ALARM_BMSK                                                          0x4
#define HWIO_MSS_UIM1_UART_DM_UIM_IRQ_IMR_CLR_BATT_ALARM_SHFT                                                          0x2
#define HWIO_MSS_UIM1_UART_DM_UIM_IRQ_IMR_CLR_UIM_CARD_INSERTION_BMSK                                                  0x2
#define HWIO_MSS_UIM1_UART_DM_UIM_IRQ_IMR_CLR_UIM_CARD_INSERTION_SHFT                                                  0x1
#define HWIO_MSS_UIM1_UART_DM_UIM_IRQ_IMR_CLR_UIM_CARD_REMOVAL_BMSK                                                    0x1
#define HWIO_MSS_UIM1_UART_DM_UIM_IRQ_IMR_CLR_UIM_CARD_REMOVAL_SHFT                                                    0x0

/*----------------------------------------------------------------------------
 * MODULE: MSS_PERPH_MSS_PERPH
 *--------------------------------------------------------------------------*/

#define MSS_PERPH_MSS_PERPH_REG_BASE                                                  (MSS_TOP_BASE      + 0x00181770)
#define MSS_PERPH_MSS_PERPH_REG_BASE_SIZE                                             0xd8b0
#define MSS_PERPH_MSS_PERPH_REG_BASE_USED                                             0xd890

#define HWIO_MSS_PERPH_MSS_ENABLE_ADDR                                                (MSS_PERPH_MSS_PERPH_REG_BASE      + 0x00000000)
#define HWIO_MSS_PERPH_MSS_ENABLE_RMSK                                                0x80000000
#define HWIO_MSS_PERPH_MSS_ENABLE_IN          \
        in_dword(HWIO_MSS_PERPH_MSS_ENABLE_ADDR)
#define HWIO_MSS_PERPH_MSS_ENABLE_INM(m)      \
        in_dword_masked(HWIO_MSS_PERPH_MSS_ENABLE_ADDR, m)
#define HWIO_MSS_PERPH_MSS_ENABLE_MODEM_ARES_IN_BMSK                                  0x80000000
#define HWIO_MSS_PERPH_MSS_ENABLE_MODEM_ARES_IN_SHFT                                        0x1f

#define HWIO_MSS_PERPH_MSS_CLAMP_MEM_ADDR                                             (MSS_PERPH_MSS_PERPH_REG_BASE      + 0x00000004)
#define HWIO_MSS_PERPH_MSS_CLAMP_MEM_RMSK                                                    0x7
#define HWIO_MSS_PERPH_MSS_CLAMP_MEM_IN          \
        in_dword(HWIO_MSS_PERPH_MSS_CLAMP_MEM_ADDR)
#define HWIO_MSS_PERPH_MSS_CLAMP_MEM_INM(m)      \
        in_dword_masked(HWIO_MSS_PERPH_MSS_CLAMP_MEM_ADDR, m)
#define HWIO_MSS_PERPH_MSS_CLAMP_MEM_OUT(v)      \
        out_dword(HWIO_MSS_PERPH_MSS_CLAMP_MEM_ADDR,v)
#define HWIO_MSS_PERPH_MSS_CLAMP_MEM_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_PERPH_MSS_CLAMP_MEM_ADDR,m,v,HWIO_MSS_PERPH_MSS_CLAMP_MEM_IN)
#define HWIO_MSS_PERPH_MSS_CLAMP_MEM_SPARE_BMSK                                              0x4
#define HWIO_MSS_PERPH_MSS_CLAMP_MEM_SPARE_SHFT                                              0x2
#define HWIO_MSS_PERPH_MSS_CLAMP_MEM_UNCLAMP_ALL_BMSK                                        0x2
#define HWIO_MSS_PERPH_MSS_CLAMP_MEM_UNCLAMP_ALL_SHFT                                        0x1
#define HWIO_MSS_PERPH_MSS_CLAMP_MEM_HM_CLAMP_BMSK                                           0x1
#define HWIO_MSS_PERPH_MSS_CLAMP_MEM_HM_CLAMP_SHFT                                           0x0

#define HWIO_MSS_PERPH_MSS_CLAMP_IO_ADDR                                              (MSS_PERPH_MSS_PERPH_REG_BASE      + 0x00000008)
#define HWIO_MSS_PERPH_MSS_CLAMP_IO_RMSK                                                    0xd0
#define HWIO_MSS_PERPH_MSS_CLAMP_IO_IN          \
        in_dword(HWIO_MSS_PERPH_MSS_CLAMP_IO_ADDR)
#define HWIO_MSS_PERPH_MSS_CLAMP_IO_INM(m)      \
        in_dword_masked(HWIO_MSS_PERPH_MSS_CLAMP_IO_ADDR, m)
#define HWIO_MSS_PERPH_MSS_CLAMP_IO_OUT(v)      \
        out_dword(HWIO_MSS_PERPH_MSS_CLAMP_IO_ADDR,v)
#define HWIO_MSS_PERPH_MSS_CLAMP_IO_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_PERPH_MSS_CLAMP_IO_ADDR,m,v,HWIO_MSS_PERPH_MSS_CLAMP_IO_IN)
#define HWIO_MSS_PERPH_MSS_CLAMP_IO_SPARE_7_BMSK                                            0x80
#define HWIO_MSS_PERPH_MSS_CLAMP_IO_SPARE_7_SHFT                                             0x7
#define HWIO_MSS_PERPH_MSS_CLAMP_IO_UNCLAMP_ALL_BMSK                                        0x40
#define HWIO_MSS_PERPH_MSS_CLAMP_IO_UNCLAMP_ALL_SHFT                                         0x6
#define HWIO_MSS_PERPH_MSS_CLAMP_IO_BBRX_ADC_BMSK                                           0x10
#define HWIO_MSS_PERPH_MSS_CLAMP_IO_BBRX_ADC_SHFT                                            0x4

#define HWIO_MSS_PERPH_MSS_BUS_AHB2AHB_CFG_ADDR                                       (MSS_PERPH_MSS_PERPH_REG_BASE      + 0x0000000c)
#define HWIO_MSS_PERPH_MSS_BUS_AHB2AHB_CFG_RMSK                                              0x1
#define HWIO_MSS_PERPH_MSS_BUS_AHB2AHB_CFG_IN          \
        in_dword(HWIO_MSS_PERPH_MSS_BUS_AHB2AHB_CFG_ADDR)
#define HWIO_MSS_PERPH_MSS_BUS_AHB2AHB_CFG_INM(m)      \
        in_dword_masked(HWIO_MSS_PERPH_MSS_BUS_AHB2AHB_CFG_ADDR, m)
#define HWIO_MSS_PERPH_MSS_BUS_AHB2AHB_CFG_OUT(v)      \
        out_dword(HWIO_MSS_PERPH_MSS_BUS_AHB2AHB_CFG_ADDR,v)
#define HWIO_MSS_PERPH_MSS_BUS_AHB2AHB_CFG_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_PERPH_MSS_BUS_AHB2AHB_CFG_ADDR,m,v,HWIO_MSS_PERPH_MSS_BUS_AHB2AHB_CFG_IN)
#define HWIO_MSS_PERPH_MSS_BUS_AHB2AHB_CFG_POST_EN_AHB2AHB_BMSK                              0x1
#define HWIO_MSS_PERPH_MSS_BUS_AHB2AHB_CFG_POST_EN_AHB2AHB_SHFT                              0x0

#define HWIO_MSS_PERPH_MSS_CUSTOM_MEM_ARRSTBYN_ADDR                                   (MSS_PERPH_MSS_PERPH_REG_BASE      + 0x00000014)
#define HWIO_MSS_PERPH_MSS_CUSTOM_MEM_ARRSTBYN_RMSK                                         0xff
#define HWIO_MSS_PERPH_MSS_CUSTOM_MEM_ARRSTBYN_IN          \
        in_dword(HWIO_MSS_PERPH_MSS_CUSTOM_MEM_ARRSTBYN_ADDR)
#define HWIO_MSS_PERPH_MSS_CUSTOM_MEM_ARRSTBYN_INM(m)      \
        in_dword_masked(HWIO_MSS_PERPH_MSS_CUSTOM_MEM_ARRSTBYN_ADDR, m)
#define HWIO_MSS_PERPH_MSS_CUSTOM_MEM_ARRSTBYN_OUT(v)      \
        out_dword(HWIO_MSS_PERPH_MSS_CUSTOM_MEM_ARRSTBYN_ADDR,v)
#define HWIO_MSS_PERPH_MSS_CUSTOM_MEM_ARRSTBYN_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_PERPH_MSS_CUSTOM_MEM_ARRSTBYN_ADDR,m,v,HWIO_MSS_PERPH_MSS_CUSTOM_MEM_ARRSTBYN_IN)
#define HWIO_MSS_PERPH_MSS_CUSTOM_MEM_ARRSTBYN_CTL_BMSK                                     0xff
#define HWIO_MSS_PERPH_MSS_CUSTOM_MEM_ARRSTBYN_CTL_SHFT                                      0x0

#define HWIO_MSS_PERPH_MSS_ANALOG_IP_TEST_CTL_ADDR                                    (MSS_PERPH_MSS_PERPH_REG_BASE      + 0x00000018)
#define HWIO_MSS_PERPH_MSS_ANALOG_IP_TEST_CTL_RMSK                                           0x3
#define HWIO_MSS_PERPH_MSS_ANALOG_IP_TEST_CTL_IN          \
        in_dword(HWIO_MSS_PERPH_MSS_ANALOG_IP_TEST_CTL_ADDR)
#define HWIO_MSS_PERPH_MSS_ANALOG_IP_TEST_CTL_INM(m)      \
        in_dword_masked(HWIO_MSS_PERPH_MSS_ANALOG_IP_TEST_CTL_ADDR, m)
#define HWIO_MSS_PERPH_MSS_ANALOG_IP_TEST_CTL_OUT(v)      \
        out_dword(HWIO_MSS_PERPH_MSS_ANALOG_IP_TEST_CTL_ADDR,v)
#define HWIO_MSS_PERPH_MSS_ANALOG_IP_TEST_CTL_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_PERPH_MSS_ANALOG_IP_TEST_CTL_ADDR,m,v,HWIO_MSS_PERPH_MSS_ANALOG_IP_TEST_CTL_IN)
#define HWIO_MSS_PERPH_MSS_ANALOG_IP_TEST_CTL_EXTERNAL_IQDATA_EN_BMSK                        0x2
#define HWIO_MSS_PERPH_MSS_ANALOG_IP_TEST_CTL_EXTERNAL_IQDATA_EN_SHFT                        0x1
#define HWIO_MSS_PERPH_MSS_ANALOG_IP_TEST_CTL_EXTERNAL_Y1Y2_EN_BMSK                          0x1
#define HWIO_MSS_PERPH_MSS_ANALOG_IP_TEST_CTL_EXTERNAL_Y1Y2_EN_SHFT                          0x0

#define HWIO_MSS_PERPH_MSS_ATB_ID_ADDR                                                (MSS_PERPH_MSS_PERPH_REG_BASE      + 0x0000001c)
#define HWIO_MSS_PERPH_MSS_ATB_ID_RMSK                                                    0x3fff
#define HWIO_MSS_PERPH_MSS_ATB_ID_IN          \
        in_dword(HWIO_MSS_PERPH_MSS_ATB_ID_ADDR)
#define HWIO_MSS_PERPH_MSS_ATB_ID_INM(m)      \
        in_dword_masked(HWIO_MSS_PERPH_MSS_ATB_ID_ADDR, m)
#define HWIO_MSS_PERPH_MSS_ATB_ID_OUT(v)      \
        out_dword(HWIO_MSS_PERPH_MSS_ATB_ID_ADDR,v)
#define HWIO_MSS_PERPH_MSS_ATB_ID_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_PERPH_MSS_ATB_ID_ADDR,m,v,HWIO_MSS_PERPH_MSS_ATB_ID_IN)
#define HWIO_MSS_PERPH_MSS_ATB_ID_MODEM_RXFE_ATB_ID_BMSK                                  0x3f80
#define HWIO_MSS_PERPH_MSS_ATB_ID_MODEM_RXFE_ATB_ID_SHFT                                     0x7
#define HWIO_MSS_PERPH_MSS_ATB_ID_ATB_ID_BMSK                                               0x7f
#define HWIO_MSS_PERPH_MSS_ATB_ID_ATB_ID_SHFT                                                0x0

#define HWIO_MSS_PERPH_MSS_DBG_BUS_CTL_ADDR                                           (MSS_PERPH_MSS_PERPH_REG_BASE      + 0x00000020)
#define HWIO_MSS_PERPH_MSS_DBG_BUS_CTL_RMSK                                                0x7e3
#define HWIO_MSS_PERPH_MSS_DBG_BUS_CTL_IN          \
        in_dword(HWIO_MSS_PERPH_MSS_DBG_BUS_CTL_ADDR)
#define HWIO_MSS_PERPH_MSS_DBG_BUS_CTL_INM(m)      \
        in_dword_masked(HWIO_MSS_PERPH_MSS_DBG_BUS_CTL_ADDR, m)
#define HWIO_MSS_PERPH_MSS_DBG_BUS_CTL_OUT(v)      \
        out_dword(HWIO_MSS_PERPH_MSS_DBG_BUS_CTL_ADDR,v)
#define HWIO_MSS_PERPH_MSS_DBG_BUS_CTL_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_PERPH_MSS_DBG_BUS_CTL_ADDR,m,v,HWIO_MSS_PERPH_MSS_DBG_BUS_CTL_IN)
#define HWIO_MSS_PERPH_MSS_DBG_BUS_CTL_AHB2AHB_SEL_BMSK                                    0x600
#define HWIO_MSS_PERPH_MSS_DBG_BUS_CTL_AHB2AHB_SEL_SHFT                                      0x9
#define HWIO_MSS_PERPH_MSS_DBG_BUS_CTL_MSS_DBG_BUS_TOP_SEL_BMSK                            0x1e0
#define HWIO_MSS_PERPH_MSS_DBG_BUS_CTL_MSS_DBG_BUS_TOP_SEL_SHFT                              0x5
#define HWIO_MSS_PERPH_MSS_DBG_BUS_CTL_MSS_DBG_GPIO_ATB_SEL_BMSK                             0x3
#define HWIO_MSS_PERPH_MSS_DBG_BUS_CTL_MSS_DBG_GPIO_ATB_SEL_SHFT                             0x0

#define HWIO_MSS_PERPH_MSS_AHB_ACCESS_ERR_IRQ_EN_ADDR                                 (MSS_PERPH_MSS_PERPH_REG_BASE      + 0x00000024)
#define HWIO_MSS_PERPH_MSS_AHB_ACCESS_ERR_IRQ_EN_RMSK                                        0x1
#define HWIO_MSS_PERPH_MSS_AHB_ACCESS_ERR_IRQ_EN_IN          \
        in_dword(HWIO_MSS_PERPH_MSS_AHB_ACCESS_ERR_IRQ_EN_ADDR)
#define HWIO_MSS_PERPH_MSS_AHB_ACCESS_ERR_IRQ_EN_INM(m)      \
        in_dword_masked(HWIO_MSS_PERPH_MSS_AHB_ACCESS_ERR_IRQ_EN_ADDR, m)
#define HWIO_MSS_PERPH_MSS_AHB_ACCESS_ERR_IRQ_EN_OUT(v)      \
        out_dword(HWIO_MSS_PERPH_MSS_AHB_ACCESS_ERR_IRQ_EN_ADDR,v)
#define HWIO_MSS_PERPH_MSS_AHB_ACCESS_ERR_IRQ_EN_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_PERPH_MSS_AHB_ACCESS_ERR_IRQ_EN_ADDR,m,v,HWIO_MSS_PERPH_MSS_AHB_ACCESS_ERR_IRQ_EN_IN)
#define HWIO_MSS_PERPH_MSS_AHB_ACCESS_ERR_IRQ_EN_EN_BMSK                                     0x1
#define HWIO_MSS_PERPH_MSS_AHB_ACCESS_ERR_IRQ_EN_EN_SHFT                                     0x0

#define HWIO_MSS_PERPH_MSS_AHB_ACCESS_ERR_IRQ_STATUS_ADDR                             (MSS_PERPH_MSS_PERPH_REG_BASE      + 0x00000028)
#define HWIO_MSS_PERPH_MSS_AHB_ACCESS_ERR_IRQ_STATUS_RMSK                                    0x1
#define HWIO_MSS_PERPH_MSS_AHB_ACCESS_ERR_IRQ_STATUS_IN          \
        in_dword(HWIO_MSS_PERPH_MSS_AHB_ACCESS_ERR_IRQ_STATUS_ADDR)
#define HWIO_MSS_PERPH_MSS_AHB_ACCESS_ERR_IRQ_STATUS_INM(m)      \
        in_dword_masked(HWIO_MSS_PERPH_MSS_AHB_ACCESS_ERR_IRQ_STATUS_ADDR, m)
#define HWIO_MSS_PERPH_MSS_AHB_ACCESS_ERR_IRQ_STATUS_STATUS_BMSK                             0x1
#define HWIO_MSS_PERPH_MSS_AHB_ACCESS_ERR_IRQ_STATUS_STATUS_SHFT                             0x0

#define HWIO_MSS_PERPH_MSS_AHB_ACCESS_ERR_IRQ_CLR_ADDR                                (MSS_PERPH_MSS_PERPH_REG_BASE      + 0x0000002c)
#define HWIO_MSS_PERPH_MSS_AHB_ACCESS_ERR_IRQ_CLR_RMSK                                       0x1
#define HWIO_MSS_PERPH_MSS_AHB_ACCESS_ERR_IRQ_CLR_OUT(v)      \
        out_dword(HWIO_MSS_PERPH_MSS_AHB_ACCESS_ERR_IRQ_CLR_ADDR,v)
#define HWIO_MSS_PERPH_MSS_AHB_ACCESS_ERR_IRQ_CLR_CMD_BMSK                                   0x1
#define HWIO_MSS_PERPH_MSS_AHB_ACCESS_ERR_IRQ_CLR_CMD_SHFT                                   0x0

#define HWIO_MSS_PERPH_MSS_BUS_CTL_CFG_ADDR                                           (MSS_PERPH_MSS_PERPH_REG_BASE      + 0x00000030)
#define HWIO_MSS_PERPH_MSS_BUS_CTL_CFG_RMSK                                                  0x1
#define HWIO_MSS_PERPH_MSS_BUS_CTL_CFG_IN          \
        in_dword(HWIO_MSS_PERPH_MSS_BUS_CTL_CFG_ADDR)
#define HWIO_MSS_PERPH_MSS_BUS_CTL_CFG_INM(m)      \
        in_dword_masked(HWIO_MSS_PERPH_MSS_BUS_CTL_CFG_ADDR, m)
#define HWIO_MSS_PERPH_MSS_BUS_CTL_CFG_OUT(v)      \
        out_dword(HWIO_MSS_PERPH_MSS_BUS_CTL_CFG_ADDR,v)
#define HWIO_MSS_PERPH_MSS_BUS_CTL_CFG_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_PERPH_MSS_BUS_CTL_CFG_ADDR,m,v,HWIO_MSS_PERPH_MSS_BUS_CTL_CFG_IN)
#define HWIO_MSS_PERPH_MSS_BUS_CTL_CFG_Q6_FORCE_UNBUFFERED_BMSK                              0x1
#define HWIO_MSS_PERPH_MSS_BUS_CTL_CFG_Q6_FORCE_UNBUFFERED_SHFT                              0x0

#define HWIO_MSS_PERPH_MSS_MSA_ADDR                                                   (MSS_PERPH_MSS_PERPH_REG_BASE      + 0x00000034)
#define HWIO_MSS_PERPH_MSS_MSA_RMSK                                                          0x7
#define HWIO_MSS_PERPH_MSS_MSA_IN          \
        in_dword(HWIO_MSS_PERPH_MSS_MSA_ADDR)
#define HWIO_MSS_PERPH_MSS_MSA_INM(m)      \
        in_dword_masked(HWIO_MSS_PERPH_MSS_MSA_ADDR, m)
#define HWIO_MSS_PERPH_MSS_MSA_OUT(v)      \
        out_dword(HWIO_MSS_PERPH_MSS_MSA_ADDR,v)
#define HWIO_MSS_PERPH_MSS_MSA_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_PERPH_MSS_MSA_ADDR,m,v,HWIO_MSS_PERPH_MSS_MSA_IN)
#define HWIO_MSS_PERPH_MSS_MSA_FORCE_Q6_MSA_BMSK                                             0x4
#define HWIO_MSS_PERPH_MSS_MSA_FORCE_Q6_MSA_SHFT                                             0x2
#define HWIO_MSS_PERPH_MSS_MSA_MBA_OK_BMSK                                                   0x2
#define HWIO_MSS_PERPH_MSS_MSA_MBA_OK_SHFT                                                   0x1
#define HWIO_MSS_PERPH_MSS_MSA_CONFIG_LOCK_BMSK                                              0x1
#define HWIO_MSS_PERPH_MSS_MSA_CONFIG_LOCK_SHFT                                              0x0

#define HWIO_MSS_PERPH_MSS_HW_VERSION_ADDR                                            (MSS_PERPH_MSS_PERPH_REG_BASE      + 0x00000038)
#define HWIO_MSS_PERPH_MSS_HW_VERSION_RMSK                                            0xffffffff
#define HWIO_MSS_PERPH_MSS_HW_VERSION_IN          \
        in_dword(HWIO_MSS_PERPH_MSS_HW_VERSION_ADDR)
#define HWIO_MSS_PERPH_MSS_HW_VERSION_INM(m)      \
        in_dword_masked(HWIO_MSS_PERPH_MSS_HW_VERSION_ADDR, m)
#define HWIO_MSS_PERPH_MSS_HW_VERSION_MAJOR_BMSK                                      0xf0000000
#define HWIO_MSS_PERPH_MSS_HW_VERSION_MAJOR_SHFT                                            0x1c
#define HWIO_MSS_PERPH_MSS_HW_VERSION_MINOR_BMSK                                       0xfff0000
#define HWIO_MSS_PERPH_MSS_HW_VERSION_MINOR_SHFT                                            0x10
#define HWIO_MSS_PERPH_MSS_HW_VERSION_STEP_BMSK                                           0xffff
#define HWIO_MSS_PERPH_MSS_HW_VERSION_STEP_SHFT                                              0x0

#define HWIO_MSS_PERPH_MSS_BBRX_CTL_ADDR                                              (MSS_PERPH_MSS_PERPH_REG_BASE      + 0x00000040)
#define HWIO_MSS_PERPH_MSS_BBRX_CTL_RMSK                                                     0x3
#define HWIO_MSS_PERPH_MSS_BBRX_CTL_IN          \
        in_dword(HWIO_MSS_PERPH_MSS_BBRX_CTL_ADDR)
#define HWIO_MSS_PERPH_MSS_BBRX_CTL_INM(m)      \
        in_dword_masked(HWIO_MSS_PERPH_MSS_BBRX_CTL_ADDR, m)
#define HWIO_MSS_PERPH_MSS_BBRX_CTL_OUT(v)      \
        out_dword(HWIO_MSS_PERPH_MSS_BBRX_CTL_ADDR,v)
#define HWIO_MSS_PERPH_MSS_BBRX_CTL_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_PERPH_MSS_BBRX_CTL_ADDR,m,v,HWIO_MSS_PERPH_MSS_BBRX_CTL_IN)
#define HWIO_MSS_PERPH_MSS_BBRX_CTL_BBRX_HS_TEST_MUX_CTL_BMSK                                0x3
#define HWIO_MSS_PERPH_MSS_BBRX_CTL_BBRX_HS_TEST_MUX_CTL_SHFT                                0x0

#define HWIO_MSS_PERPH_MSS_DEBUG_CTL_ADDR                                             (MSS_PERPH_MSS_PERPH_REG_BASE      + 0x00000044)
#define HWIO_MSS_PERPH_MSS_DEBUG_CTL_RMSK                                                    0x7
#define HWIO_MSS_PERPH_MSS_DEBUG_CTL_IN          \
        in_dword(HWIO_MSS_PERPH_MSS_DEBUG_CTL_ADDR)
#define HWIO_MSS_PERPH_MSS_DEBUG_CTL_INM(m)      \
        in_dword_masked(HWIO_MSS_PERPH_MSS_DEBUG_CTL_ADDR, m)
#define HWIO_MSS_PERPH_MSS_DEBUG_CTL_OUT(v)      \
        out_dword(HWIO_MSS_PERPH_MSS_DEBUG_CTL_ADDR,v)
#define HWIO_MSS_PERPH_MSS_DEBUG_CTL_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_PERPH_MSS_DEBUG_CTL_ADDR,m,v,HWIO_MSS_PERPH_MSS_DEBUG_CTL_IN)
#define HWIO_MSS_PERPH_MSS_DEBUG_CTL_DAC_DISABLE_ON_Q6_DBG_BMSK                              0x4
#define HWIO_MSS_PERPH_MSS_DEBUG_CTL_DAC_DISABLE_ON_Q6_DBG_SHFT                              0x2
#define HWIO_MSS_PERPH_MSS_DEBUG_CTL_GRFC_DISABLE_Q6_DBG_BMSK                                0x2
#define HWIO_MSS_PERPH_MSS_DEBUG_CTL_GRFC_DISABLE_Q6_DBG_SHFT                                0x1
#define HWIO_MSS_PERPH_MSS_DEBUG_CTL_GRFC_DISABLE_Q6_WDOG_BMSK                               0x1
#define HWIO_MSS_PERPH_MSS_DEBUG_CTL_GRFC_DISABLE_Q6_WDOG_SHFT                               0x0

#define HWIO_MSS_PERPH_MSS_MSA_NC_HM_ADDR                                             (MSS_PERPH_MSS_PERPH_REG_BASE      + 0x0000004c)
#define HWIO_MSS_PERPH_MSS_MSA_NC_HM_RMSK                                             0xfffffeff
#define HWIO_MSS_PERPH_MSS_MSA_NC_HM_IN          \
        in_dword(HWIO_MSS_PERPH_MSS_MSA_NC_HM_ADDR)
#define HWIO_MSS_PERPH_MSS_MSA_NC_HM_INM(m)      \
        in_dword_masked(HWIO_MSS_PERPH_MSS_MSA_NC_HM_ADDR, m)
#define HWIO_MSS_PERPH_MSS_MSA_NC_HM_OUT(v)      \
        out_dword(HWIO_MSS_PERPH_MSS_MSA_NC_HM_ADDR,v)
#define HWIO_MSS_PERPH_MSS_MSA_NC_HM_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_PERPH_MSS_MSA_NC_HM_ADDR,m,v,HWIO_MSS_PERPH_MSS_MSA_NC_HM_IN)
#define HWIO_MSS_PERPH_MSS_MSA_NC_HM_RESERVE_31_9_BMSK                                0xfffffe00
#define HWIO_MSS_PERPH_MSS_MSA_NC_HM_RESERVE_31_9_SHFT                                       0x9
#define HWIO_MSS_PERPH_MSS_MSA_NC_HM_RESERVE_7_4_BMSK                                       0xf0
#define HWIO_MSS_PERPH_MSS_MSA_NC_HM_RESERVE_7_4_SHFT                                        0x4
#define HWIO_MSS_PERPH_MSS_MSA_NC_HM_CRYPTO_BMSK                                             0xf
#define HWIO_MSS_PERPH_MSS_MSA_NC_HM_CRYPTO_SHFT                                             0x0

#define HWIO_MSS_PERPH_MSS_CXM_ADDR                                                   (MSS_PERPH_MSS_PERPH_REG_BASE      + 0x00000050)
#define HWIO_MSS_PERPH_MSS_CXM_RMSK                                                   0xffffffff
#define HWIO_MSS_PERPH_MSS_CXM_IN          \
        in_dword(HWIO_MSS_PERPH_MSS_CXM_ADDR)
#define HWIO_MSS_PERPH_MSS_CXM_INM(m)      \
        in_dword_masked(HWIO_MSS_PERPH_MSS_CXM_ADDR, m)
#define HWIO_MSS_PERPH_MSS_CXM_OUT(v)      \
        out_dword(HWIO_MSS_PERPH_MSS_CXM_ADDR,v)
#define HWIO_MSS_PERPH_MSS_CXM_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_PERPH_MSS_CXM_ADDR,m,v,HWIO_MSS_PERPH_MSS_CXM_IN)
#define HWIO_MSS_PERPH_MSS_CXM_RESERVE_31_12_BMSK                                     0xfffff000
#define HWIO_MSS_PERPH_MSS_CXM_RESERVE_31_12_SHFT                                            0xc
#define HWIO_MSS_PERPH_MSS_CXM_USE_EXTERNAL_WLAN_BMSK                                      0x800
#define HWIO_MSS_PERPH_MSS_CXM_USE_EXTERNAL_WLAN_SHFT                                        0xb
#define HWIO_MSS_PERPH_MSS_CXM_BT_TX_ACT_LST_SUBFRM_BMSK                                   0x400
#define HWIO_MSS_PERPH_MSS_CXM_BT_TX_ACT_LST_SUBFRM_SHFT                                     0xa
#define HWIO_MSS_PERPH_MSS_CXM_WLAN_TX_ACT_LST_SUBFRM_BMSK                                 0x200
#define HWIO_MSS_PERPH_MSS_CXM_WLAN_TX_ACT_LST_SUBFRM_SHFT                                   0x9
#define HWIO_MSS_PERPH_MSS_CXM_BT_TX_S_WAN_BMSK                                            0x100
#define HWIO_MSS_PERPH_MSS_CXM_BT_TX_S_WAN_SHFT                                              0x8
#define HWIO_MSS_PERPH_MSS_CXM_WLAN_TX_S_WAN_BMSK                                           0x80
#define HWIO_MSS_PERPH_MSS_CXM_WLAN_TX_S_WAN_SHFT                                            0x7
#define HWIO_MSS_PERPH_MSS_CXM_BT_TX_REGISTERED_BMSK                                        0x40
#define HWIO_MSS_PERPH_MSS_CXM_BT_TX_REGISTERED_SHFT                                         0x6
#define HWIO_MSS_PERPH_MSS_CXM_WLAN_TX_REGISTERED_BMSK                                      0x20
#define HWIO_MSS_PERPH_MSS_CXM_WLAN_TX_REGISTERED_SHFT                                       0x5
#define HWIO_MSS_PERPH_MSS_CXM_WLAN_TX_STICKY_BMSK                                          0x10
#define HWIO_MSS_PERPH_MSS_CXM_WLAN_TX_STICKY_SHFT                                           0x4
#define HWIO_MSS_PERPH_MSS_CXM_WLAN_TX_STICKY_CLR_BMSK                                       0x8
#define HWIO_MSS_PERPH_MSS_CXM_WLAN_TX_STICKY_CLR_SHFT                                       0x3
#define HWIO_MSS_PERPH_MSS_CXM_CXM_WAKEUP_CLR_BMSK                                           0x4
#define HWIO_MSS_PERPH_MSS_CXM_CXM_WAKEUP_CLR_SHFT                                           0x2
#define HWIO_MSS_PERPH_MSS_CXM_BT_TX_ON_BLANKING_EN_BMSK                                     0x2
#define HWIO_MSS_PERPH_MSS_CXM_BT_TX_ON_BLANKING_EN_SHFT                                     0x1
#define HWIO_MSS_PERPH_MSS_CXM_WLAN_TX_ON_BLANKING_EN_BMSK                                   0x1
#define HWIO_MSS_PERPH_MSS_CXM_WLAN_TX_ON_BLANKING_EN_SHFT                                   0x0

#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_00_ADDR                                   (MSS_PERPH_MSS_PERPH_REG_BASE      + 0x00000054)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_00_RMSK                                   0xffffffff
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_00_IN          \
        in_dword(HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_00_ADDR)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_00_INM(m)      \
        in_dword_masked(HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_00_ADDR, m)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_00_OUT(v)      \
        out_dword(HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_00_ADDR,v)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_00_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_00_ADDR,m,v,HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_00_IN)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_00_RELAY_MSG_SHADOW_DATA_00_BMSK          0xffffffff
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_00_RELAY_MSG_SHADOW_DATA_00_SHFT                 0x0

#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_01_ADDR                                   (MSS_PERPH_MSS_PERPH_REG_BASE      + 0x00000058)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_01_RMSK                                   0xffffffff
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_01_IN          \
        in_dword(HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_01_ADDR)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_01_INM(m)      \
        in_dword_masked(HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_01_ADDR, m)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_01_OUT(v)      \
        out_dword(HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_01_ADDR,v)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_01_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_01_ADDR,m,v,HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_01_IN)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_01_RELAY_MSG_SHADOW_DATA_01_BMSK          0xffffffff
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_01_RELAY_MSG_SHADOW_DATA_01_SHFT                 0x0

#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_02_ADDR                                   (MSS_PERPH_MSS_PERPH_REG_BASE      + 0x0000005c)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_02_RMSK                                   0xffffffff
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_02_IN          \
        in_dword(HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_02_ADDR)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_02_INM(m)      \
        in_dword_masked(HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_02_ADDR, m)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_02_OUT(v)      \
        out_dword(HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_02_ADDR,v)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_02_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_02_ADDR,m,v,HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_02_IN)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_02_RELAY_MSG_SHADOW_DATA_02_BMSK          0xffffffff
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_02_RELAY_MSG_SHADOW_DATA_02_SHFT                 0x0

#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_03_ADDR                                   (MSS_PERPH_MSS_PERPH_REG_BASE      + 0x00000060)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_03_RMSK                                   0xffffffff
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_03_IN          \
        in_dword(HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_03_ADDR)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_03_INM(m)      \
        in_dword_masked(HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_03_ADDR, m)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_03_OUT(v)      \
        out_dword(HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_03_ADDR,v)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_03_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_03_ADDR,m,v,HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_03_IN)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_03_RELAY_MSG_SHADOW_DATA_03_BMSK          0xffffffff
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_03_RELAY_MSG_SHADOW_DATA_03_SHFT                 0x0

#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_04_ADDR                                   (MSS_PERPH_MSS_PERPH_REG_BASE      + 0x00000064)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_04_RMSK                                   0xffffffff
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_04_IN          \
        in_dword(HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_04_ADDR)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_04_INM(m)      \
        in_dword_masked(HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_04_ADDR, m)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_04_OUT(v)      \
        out_dword(HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_04_ADDR,v)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_04_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_04_ADDR,m,v,HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_04_IN)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_04_RELAY_MSG_SHADOW_DATA_04_BMSK          0xffffffff
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_04_RELAY_MSG_SHADOW_DATA_04_SHFT                 0x0

#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_05_ADDR                                   (MSS_PERPH_MSS_PERPH_REG_BASE      + 0x00000068)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_05_RMSK                                   0xffffffff
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_05_IN          \
        in_dword(HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_05_ADDR)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_05_INM(m)      \
        in_dword_masked(HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_05_ADDR, m)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_05_OUT(v)      \
        out_dword(HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_05_ADDR,v)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_05_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_05_ADDR,m,v,HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_05_IN)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_05_RELAY_MSG_SHADOW_DATA_05_BMSK          0xffffffff
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_05_RELAY_MSG_SHADOW_DATA_05_SHFT                 0x0

#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_06_ADDR                                   (MSS_PERPH_MSS_PERPH_REG_BASE      + 0x0000006c)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_06_RMSK                                   0xffffffff
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_06_IN          \
        in_dword(HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_06_ADDR)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_06_INM(m)      \
        in_dword_masked(HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_06_ADDR, m)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_06_OUT(v)      \
        out_dword(HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_06_ADDR,v)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_06_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_06_ADDR,m,v,HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_06_IN)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_06_RELAY_MSG_SHADOW_DATA_06_BMSK          0xffffffff
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_06_RELAY_MSG_SHADOW_DATA_06_SHFT                 0x0

#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_07_ADDR                                   (MSS_PERPH_MSS_PERPH_REG_BASE      + 0x00000070)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_07_RMSK                                   0xffffffff
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_07_IN          \
        in_dword(HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_07_ADDR)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_07_INM(m)      \
        in_dword_masked(HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_07_ADDR, m)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_07_OUT(v)      \
        out_dword(HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_07_ADDR,v)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_07_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_07_ADDR,m,v,HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_07_IN)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_07_RELAY_MSG_SHADOW_DATA_07_BMSK          0xffffffff
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_07_RELAY_MSG_SHADOW_DATA_07_SHFT                 0x0

#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_08_ADDR                                   (MSS_PERPH_MSS_PERPH_REG_BASE      + 0x00000074)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_08_RMSK                                   0xffffffff
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_08_IN          \
        in_dword(HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_08_ADDR)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_08_INM(m)      \
        in_dword_masked(HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_08_ADDR, m)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_08_OUT(v)      \
        out_dword(HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_08_ADDR,v)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_08_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_08_ADDR,m,v,HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_08_IN)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_08_RELAY_MSG_SHADOW_DATA_08_BMSK          0xffffffff
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_08_RELAY_MSG_SHADOW_DATA_08_SHFT                 0x0

#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_09_ADDR                                   (MSS_PERPH_MSS_PERPH_REG_BASE      + 0x00000078)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_09_RMSK                                   0xffffffff
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_09_IN          \
        in_dword(HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_09_ADDR)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_09_INM(m)      \
        in_dword_masked(HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_09_ADDR, m)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_09_OUT(v)      \
        out_dword(HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_09_ADDR,v)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_09_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_09_ADDR,m,v,HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_09_IN)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_09_RELAY_MSG_SHADOW_DATA_09_BMSK          0xffffffff
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_09_RELAY_MSG_SHADOW_DATA_09_SHFT                 0x0

#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_10_ADDR                                   (MSS_PERPH_MSS_PERPH_REG_BASE      + 0x0000007c)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_10_RMSK                                   0xffffffff
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_10_IN          \
        in_dword(HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_10_ADDR)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_10_INM(m)      \
        in_dword_masked(HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_10_ADDR, m)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_10_OUT(v)      \
        out_dword(HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_10_ADDR,v)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_10_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_10_ADDR,m,v,HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_10_IN)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_10_RELAY_MSG_SHADOW_DATA_10_BMSK          0xffffffff
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_10_RELAY_MSG_SHADOW_DATA_10_SHFT                 0x0

#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_11_ADDR                                   (MSS_PERPH_MSS_PERPH_REG_BASE      + 0x00000080)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_11_RMSK                                   0xffffffff
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_11_IN          \
        in_dword(HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_11_ADDR)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_11_INM(m)      \
        in_dword_masked(HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_11_ADDR, m)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_11_OUT(v)      \
        out_dword(HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_11_ADDR,v)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_11_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_11_ADDR,m,v,HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_11_IN)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_11_RELAY_MSG_SHADOW_DATA_11_BMSK          0xffffffff
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_11_RELAY_MSG_SHADOW_DATA_11_SHFT                 0x0

#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_12_ADDR                                   (MSS_PERPH_MSS_PERPH_REG_BASE      + 0x00000084)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_12_RMSK                                   0xffffffff
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_12_IN          \
        in_dword(HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_12_ADDR)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_12_INM(m)      \
        in_dword_masked(HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_12_ADDR, m)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_12_OUT(v)      \
        out_dword(HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_12_ADDR,v)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_12_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_12_ADDR,m,v,HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_12_IN)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_12_RELAY_MSG_SHADOW_DATA_12_BMSK          0xffffffff
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_12_RELAY_MSG_SHADOW_DATA_12_SHFT                 0x0

#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_13_ADDR                                   (MSS_PERPH_MSS_PERPH_REG_BASE      + 0x00000088)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_13_RMSK                                   0xffffffff
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_13_IN          \
        in_dword(HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_13_ADDR)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_13_INM(m)      \
        in_dword_masked(HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_13_ADDR, m)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_13_OUT(v)      \
        out_dword(HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_13_ADDR,v)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_13_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_13_ADDR,m,v,HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_13_IN)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_13_RELAY_MSG_SHADOW_DATA_13_BMSK          0xffffffff
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_13_RELAY_MSG_SHADOW_DATA_13_SHFT                 0x0

#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_14_ADDR                                   (MSS_PERPH_MSS_PERPH_REG_BASE      + 0x0000008c)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_14_RMSK                                   0xffffffff
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_14_IN          \
        in_dword(HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_14_ADDR)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_14_INM(m)      \
        in_dword_masked(HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_14_ADDR, m)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_14_OUT(v)      \
        out_dword(HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_14_ADDR,v)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_14_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_14_ADDR,m,v,HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_14_IN)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_14_RELAY_MSG_SHADOW_DATA_14_BMSK          0xffffffff
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_14_RELAY_MSG_SHADOW_DATA_14_SHFT                 0x0

#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_15_ADDR                                   (MSS_PERPH_MSS_PERPH_REG_BASE      + 0x00000090)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_15_RMSK                                   0xffffffff
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_15_IN          \
        in_dword(HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_15_ADDR)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_15_INM(m)      \
        in_dword_masked(HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_15_ADDR, m)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_15_OUT(v)      \
        out_dword(HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_15_ADDR,v)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_15_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_15_ADDR,m,v,HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_15_IN)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_15_RELAY_MSG_SHADOW_DATA_15_BMSK          0xffffffff
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_15_RELAY_MSG_SHADOW_DATA_15_SHFT                 0x0

#define HWIO_MSS_PERPH_EXT_GPIO_SEL_CFG_ADDR                                          (MSS_PERPH_MSS_PERPH_REG_BASE      + 0x00000094)
#define HWIO_MSS_PERPH_EXT_GPIO_SEL_CFG_RMSK                                                0xff
#define HWIO_MSS_PERPH_EXT_GPIO_SEL_CFG_IN          \
        in_dword(HWIO_MSS_PERPH_EXT_GPIO_SEL_CFG_ADDR)
#define HWIO_MSS_PERPH_EXT_GPIO_SEL_CFG_INM(m)      \
        in_dword_masked(HWIO_MSS_PERPH_EXT_GPIO_SEL_CFG_ADDR, m)
#define HWIO_MSS_PERPH_EXT_GPIO_SEL_CFG_OUT(v)      \
        out_dword(HWIO_MSS_PERPH_EXT_GPIO_SEL_CFG_ADDR,v)
#define HWIO_MSS_PERPH_EXT_GPIO_SEL_CFG_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_PERPH_EXT_GPIO_SEL_CFG_ADDR,m,v,HWIO_MSS_PERPH_EXT_GPIO_SEL_CFG_IN)
#define HWIO_MSS_PERPH_EXT_GPIO_SEL_CFG_EXT_GPIO_SEL_CFG_BMSK                               0xff
#define HWIO_MSS_PERPH_EXT_GPIO_SEL_CFG_EXT_GPIO_SEL_CFG_SHFT                                0x0

#define HWIO_MSS_PERPH_MSS_TCSR_ACC_SEL_ADDR                                          (MSS_PERPH_MSS_PERPH_REG_BASE      + 0x0000d890)
#define HWIO_MSS_PERPH_MSS_TCSR_ACC_SEL_RMSK                                                 0x3
#define HWIO_MSS_PERPH_MSS_TCSR_ACC_SEL_IN          \
        in_dword(HWIO_MSS_PERPH_MSS_TCSR_ACC_SEL_ADDR)
#define HWIO_MSS_PERPH_MSS_TCSR_ACC_SEL_INM(m)      \
        in_dword_masked(HWIO_MSS_PERPH_MSS_TCSR_ACC_SEL_ADDR, m)
#define HWIO_MSS_PERPH_MSS_TCSR_ACC_SEL_OUT(v)      \
        out_dword(HWIO_MSS_PERPH_MSS_TCSR_ACC_SEL_ADDR,v)
#define HWIO_MSS_PERPH_MSS_TCSR_ACC_SEL_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_PERPH_MSS_TCSR_ACC_SEL_ADDR,m,v,HWIO_MSS_PERPH_MSS_TCSR_ACC_SEL_IN)
#define HWIO_MSS_PERPH_MSS_TCSR_ACC_SEL_ACC_MEM_SEL_BMSK                                     0x3
#define HWIO_MSS_PERPH_MSS_TCSR_ACC_SEL_ACC_MEM_SEL_SHFT                                     0x0

#define HWIO_MSS_PERPH_MSS_BIMC_THROTTLE_ADDR                                         (MSS_PERPH_MSS_PERPH_REG_BASE      + 0x0000009c)
#define HWIO_MSS_PERPH_MSS_BIMC_THROTTLE_RMSK                                                0x7
#define HWIO_MSS_PERPH_MSS_BIMC_THROTTLE_IN          \
        in_dword(HWIO_MSS_PERPH_MSS_BIMC_THROTTLE_ADDR)
#define HWIO_MSS_PERPH_MSS_BIMC_THROTTLE_INM(m)      \
        in_dword_masked(HWIO_MSS_PERPH_MSS_BIMC_THROTTLE_ADDR, m)
#define HWIO_MSS_PERPH_MSS_BIMC_THROTTLE_OUT(v)      \
        out_dword(HWIO_MSS_PERPH_MSS_BIMC_THROTTLE_ADDR,v)
#define HWIO_MSS_PERPH_MSS_BIMC_THROTTLE_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_PERPH_MSS_BIMC_THROTTLE_ADDR,m,v,HWIO_MSS_PERPH_MSS_BIMC_THROTTLE_IN)
#define HWIO_MSS_PERPH_MSS_BIMC_THROTTLE_MSS_DANGER_BMSK                                     0x6
#define HWIO_MSS_PERPH_MSS_BIMC_THROTTLE_MSS_DANGER_SHFT                                     0x1
#define HWIO_MSS_PERPH_MSS_BIMC_THROTTLE_MSS_SAFE_BMSK                                       0x1
#define HWIO_MSS_PERPH_MSS_BIMC_THROTTLE_MSS_SAFE_SHFT                                       0x0

/*----------------------------------------------------------------------------
 * MODULE: SSC_TLMM_CSR
 *--------------------------------------------------------------------------*/

#define SSC_TLMM_CSR_REG_BASE                                       (LPASS_BASE      + 0x00c40000)
#define SSC_TLMM_CSR_REG_BASE_SIZE                                  0x20000
#define SSC_TLMM_CSR_REG_BASE_USED                                  0xf004

#define HWIO_SSC_TLMM_LITE_GPIO_CFG_0_ADDR                          (SSC_TLMM_CSR_REG_BASE      + 0x00000000)
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_0_RMSK                              0xffff
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_0_IN          \
        in_dword(HWIO_SSC_TLMM_LITE_GPIO_CFG_0_ADDR)
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_0_INM(m)      \
        in_dword_masked(HWIO_SSC_TLMM_LITE_GPIO_CFG_0_ADDR, m)
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_0_OUT(v)      \
        out_dword(HWIO_SSC_TLMM_LITE_GPIO_CFG_0_ADDR,v)
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_0_OUTM(m,v) \
        out_dword_masked_ns(HWIO_SSC_TLMM_LITE_GPIO_CFG_0_ADDR,m,v,HWIO_SSC_TLMM_LITE_GPIO_CFG_0_IN)
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_0_GPIO_MISC_CFG_BMSK                0xf800
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_0_GPIO_MISC_CFG_SHFT                   0xb
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_0_GPIO_HIHYS_CTL_BMSK                0x400
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_0_GPIO_HIHYS_CTL_SHFT                  0xa
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_0_GPIO_OE_BMSK                       0x200
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_0_GPIO_OE_SHFT                         0x9
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_0_GPIO_HDRIVE_BMSK                   0x1c0
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_0_GPIO_HDRIVE_SHFT                     0x6
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_0_GPIO_FUNC_SEL_BMSK                  0x3c
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_0_GPIO_FUNC_SEL_SHFT                   0x2
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_0_GPIO_PULL_BMSK                       0x3
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_0_GPIO_PULL_SHFT                       0x0

#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_0_ADDR                       (SSC_TLMM_CSR_REG_BASE      + 0x00000004)
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_0_RMSK                              0x3
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_0_IN          \
        in_dword(HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_0_ADDR)
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_0_INM(m)      \
        in_dword_masked(HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_0_ADDR, m)
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_0_OUT(v)      \
        out_dword(HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_0_ADDR,v)
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_0_OUTM(m,v) \
        out_dword_masked_ns(HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_0_ADDR,m,v,HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_0_IN)
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_0_GPIO_OUT_BMSK                     0x2
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_0_GPIO_OUT_SHFT                     0x1
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_0_GPIO_IN_BMSK                      0x1
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_0_GPIO_IN_SHFT                      0x0

#define HWIO_SSC_TLMM_LITE_GPIO_CFG_1_ADDR                          (SSC_TLMM_CSR_REG_BASE      + 0x00001000)
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_1_RMSK                              0xffff
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_1_IN          \
        in_dword(HWIO_SSC_TLMM_LITE_GPIO_CFG_1_ADDR)
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_1_INM(m)      \
        in_dword_masked(HWIO_SSC_TLMM_LITE_GPIO_CFG_1_ADDR, m)
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_1_OUT(v)      \
        out_dword(HWIO_SSC_TLMM_LITE_GPIO_CFG_1_ADDR,v)
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_1_OUTM(m,v) \
        out_dword_masked_ns(HWIO_SSC_TLMM_LITE_GPIO_CFG_1_ADDR,m,v,HWIO_SSC_TLMM_LITE_GPIO_CFG_1_IN)
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_1_GPIO_MISC_CFG_BMSK                0xf800
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_1_GPIO_MISC_CFG_SHFT                   0xb
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_1_GPIO_HIHYS_CTL_BMSK                0x400
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_1_GPIO_HIHYS_CTL_SHFT                  0xa
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_1_GPIO_OE_BMSK                       0x200
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_1_GPIO_OE_SHFT                         0x9
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_1_GPIO_HDRIVE_BMSK                   0x1c0
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_1_GPIO_HDRIVE_SHFT                     0x6
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_1_GPIO_FUNC_SEL_BMSK                  0x3c
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_1_GPIO_FUNC_SEL_SHFT                   0x2
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_1_GPIO_PULL_BMSK                       0x3
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_1_GPIO_PULL_SHFT                       0x0

#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_1_ADDR                       (SSC_TLMM_CSR_REG_BASE      + 0x00001004)
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_1_RMSK                              0x3
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_1_IN          \
        in_dword(HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_1_ADDR)
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_1_INM(m)      \
        in_dword_masked(HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_1_ADDR, m)
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_1_OUT(v)      \
        out_dword(HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_1_ADDR,v)
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_1_OUTM(m,v) \
        out_dword_masked_ns(HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_1_ADDR,m,v,HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_1_IN)
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_1_GPIO_OUT_BMSK                     0x2
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_1_GPIO_OUT_SHFT                     0x1
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_1_GPIO_IN_BMSK                      0x1
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_1_GPIO_IN_SHFT                      0x0

#define HWIO_SSC_TLMM_LITE_GPIO_CFG_2_ADDR                          (SSC_TLMM_CSR_REG_BASE      + 0x00002000)
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_2_RMSK                              0xffff
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_2_IN          \
        in_dword(HWIO_SSC_TLMM_LITE_GPIO_CFG_2_ADDR)
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_2_INM(m)      \
        in_dword_masked(HWIO_SSC_TLMM_LITE_GPIO_CFG_2_ADDR, m)
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_2_OUT(v)      \
        out_dword(HWIO_SSC_TLMM_LITE_GPIO_CFG_2_ADDR,v)
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_2_OUTM(m,v) \
        out_dword_masked_ns(HWIO_SSC_TLMM_LITE_GPIO_CFG_2_ADDR,m,v,HWIO_SSC_TLMM_LITE_GPIO_CFG_2_IN)
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_2_GPIO_MISC_CFG_BMSK                0xf800
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_2_GPIO_MISC_CFG_SHFT                   0xb
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_2_GPIO_HIHYS_CTL_BMSK                0x400
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_2_GPIO_HIHYS_CTL_SHFT                  0xa
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_2_GPIO_OE_BMSK                       0x200
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_2_GPIO_OE_SHFT                         0x9
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_2_GPIO_HDRIVE_BMSK                   0x1c0
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_2_GPIO_HDRIVE_SHFT                     0x6
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_2_GPIO_FUNC_SEL_BMSK                  0x3c
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_2_GPIO_FUNC_SEL_SHFT                   0x2
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_2_GPIO_PULL_BMSK                       0x3
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_2_GPIO_PULL_SHFT                       0x0

#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_2_ADDR                       (SSC_TLMM_CSR_REG_BASE      + 0x00002004)
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_2_RMSK                              0x3
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_2_IN          \
        in_dword(HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_2_ADDR)
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_2_INM(m)      \
        in_dword_masked(HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_2_ADDR, m)
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_2_OUT(v)      \
        out_dword(HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_2_ADDR,v)
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_2_OUTM(m,v) \
        out_dword_masked_ns(HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_2_ADDR,m,v,HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_2_IN)
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_2_GPIO_OUT_BMSK                     0x2
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_2_GPIO_OUT_SHFT                     0x1
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_2_GPIO_IN_BMSK                      0x1
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_2_GPIO_IN_SHFT                      0x0

#define HWIO_SSC_TLMM_LITE_GPIO_CFG_3_ADDR                          (SSC_TLMM_CSR_REG_BASE      + 0x00003000)
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_3_RMSK                              0xffff
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_3_IN          \
        in_dword(HWIO_SSC_TLMM_LITE_GPIO_CFG_3_ADDR)
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_3_INM(m)      \
        in_dword_masked(HWIO_SSC_TLMM_LITE_GPIO_CFG_3_ADDR, m)
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_3_OUT(v)      \
        out_dword(HWIO_SSC_TLMM_LITE_GPIO_CFG_3_ADDR,v)
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_3_OUTM(m,v) \
        out_dword_masked_ns(HWIO_SSC_TLMM_LITE_GPIO_CFG_3_ADDR,m,v,HWIO_SSC_TLMM_LITE_GPIO_CFG_3_IN)
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_3_GPIO_MISC_CFG_BMSK                0xf800
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_3_GPIO_MISC_CFG_SHFT                   0xb
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_3_GPIO_HIHYS_CTL_BMSK                0x400
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_3_GPIO_HIHYS_CTL_SHFT                  0xa
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_3_GPIO_OE_BMSK                       0x200
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_3_GPIO_OE_SHFT                         0x9
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_3_GPIO_HDRIVE_BMSK                   0x1c0
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_3_GPIO_HDRIVE_SHFT                     0x6
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_3_GPIO_FUNC_SEL_BMSK                  0x3c
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_3_GPIO_FUNC_SEL_SHFT                   0x2
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_3_GPIO_PULL_BMSK                       0x3
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_3_GPIO_PULL_SHFT                       0x0

#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_3_ADDR                       (SSC_TLMM_CSR_REG_BASE      + 0x00003004)
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_3_RMSK                              0x3
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_3_IN          \
        in_dword(HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_3_ADDR)
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_3_INM(m)      \
        in_dword_masked(HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_3_ADDR, m)
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_3_OUT(v)      \
        out_dword(HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_3_ADDR,v)
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_3_OUTM(m,v) \
        out_dword_masked_ns(HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_3_ADDR,m,v,HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_3_IN)
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_3_GPIO_OUT_BMSK                     0x2
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_3_GPIO_OUT_SHFT                     0x1
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_3_GPIO_IN_BMSK                      0x1
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_3_GPIO_IN_SHFT                      0x0

#define HWIO_SSC_TLMM_LITE_GPIO_CFG_4_ADDR                          (SSC_TLMM_CSR_REG_BASE      + 0x00004000)
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_4_RMSK                              0xffff
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_4_IN          \
        in_dword(HWIO_SSC_TLMM_LITE_GPIO_CFG_4_ADDR)
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_4_INM(m)      \
        in_dword_masked(HWIO_SSC_TLMM_LITE_GPIO_CFG_4_ADDR, m)
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_4_OUT(v)      \
        out_dword(HWIO_SSC_TLMM_LITE_GPIO_CFG_4_ADDR,v)
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_4_OUTM(m,v) \
        out_dword_masked_ns(HWIO_SSC_TLMM_LITE_GPIO_CFG_4_ADDR,m,v,HWIO_SSC_TLMM_LITE_GPIO_CFG_4_IN)
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_4_GPIO_MISC_CFG_BMSK                0xf800
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_4_GPIO_MISC_CFG_SHFT                   0xb
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_4_GPIO_HIHYS_CTL_BMSK                0x400
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_4_GPIO_HIHYS_CTL_SHFT                  0xa
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_4_GPIO_OE_BMSK                       0x200
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_4_GPIO_OE_SHFT                         0x9
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_4_GPIO_HDRIVE_BMSK                   0x1c0
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_4_GPIO_HDRIVE_SHFT                     0x6
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_4_GPIO_FUNC_SEL_BMSK                  0x3c
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_4_GPIO_FUNC_SEL_SHFT                   0x2
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_4_GPIO_PULL_BMSK                       0x3
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_4_GPIO_PULL_SHFT                       0x0

#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_4_ADDR                       (SSC_TLMM_CSR_REG_BASE      + 0x00004004)
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_4_RMSK                              0x3
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_4_IN          \
        in_dword(HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_4_ADDR)
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_4_INM(m)      \
        in_dword_masked(HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_4_ADDR, m)
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_4_OUT(v)      \
        out_dword(HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_4_ADDR,v)
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_4_OUTM(m,v) \
        out_dword_masked_ns(HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_4_ADDR,m,v,HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_4_IN)
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_4_GPIO_OUT_BMSK                     0x2
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_4_GPIO_OUT_SHFT                     0x1
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_4_GPIO_IN_BMSK                      0x1
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_4_GPIO_IN_SHFT                      0x0

#define HWIO_SSC_TLMM_LITE_GPIO_CFG_5_ADDR                          (SSC_TLMM_CSR_REG_BASE      + 0x00005000)
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_5_RMSK                              0xffff
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_5_IN          \
        in_dword(HWIO_SSC_TLMM_LITE_GPIO_CFG_5_ADDR)
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_5_INM(m)      \
        in_dword_masked(HWIO_SSC_TLMM_LITE_GPIO_CFG_5_ADDR, m)
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_5_OUT(v)      \
        out_dword(HWIO_SSC_TLMM_LITE_GPIO_CFG_5_ADDR,v)
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_5_OUTM(m,v) \
        out_dword_masked_ns(HWIO_SSC_TLMM_LITE_GPIO_CFG_5_ADDR,m,v,HWIO_SSC_TLMM_LITE_GPIO_CFG_5_IN)
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_5_GPIO_MISC_CFG_BMSK                0xf800
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_5_GPIO_MISC_CFG_SHFT                   0xb
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_5_GPIO_HIHYS_CTL_BMSK                0x400
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_5_GPIO_HIHYS_CTL_SHFT                  0xa
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_5_GPIO_OE_BMSK                       0x200
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_5_GPIO_OE_SHFT                         0x9
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_5_GPIO_HDRIVE_BMSK                   0x1c0
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_5_GPIO_HDRIVE_SHFT                     0x6
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_5_GPIO_FUNC_SEL_BMSK                  0x3c
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_5_GPIO_FUNC_SEL_SHFT                   0x2
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_5_GPIO_PULL_BMSK                       0x3
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_5_GPIO_PULL_SHFT                       0x0

#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_5_ADDR                       (SSC_TLMM_CSR_REG_BASE      + 0x00005004)
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_5_RMSK                              0x3
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_5_IN          \
        in_dword(HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_5_ADDR)
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_5_INM(m)      \
        in_dword_masked(HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_5_ADDR, m)
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_5_OUT(v)      \
        out_dword(HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_5_ADDR,v)
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_5_OUTM(m,v) \
        out_dword_masked_ns(HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_5_ADDR,m,v,HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_5_IN)
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_5_GPIO_OUT_BMSK                     0x2
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_5_GPIO_OUT_SHFT                     0x1
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_5_GPIO_IN_BMSK                      0x1
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_5_GPIO_IN_SHFT                      0x0

#define HWIO_SSC_TLMM_LITE_GPIO_CFG_6_ADDR                          (SSC_TLMM_CSR_REG_BASE      + 0x00006000)
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_6_RMSK                              0xffff
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_6_IN          \
        in_dword(HWIO_SSC_TLMM_LITE_GPIO_CFG_6_ADDR)
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_6_INM(m)      \
        in_dword_masked(HWIO_SSC_TLMM_LITE_GPIO_CFG_6_ADDR, m)
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_6_OUT(v)      \
        out_dword(HWIO_SSC_TLMM_LITE_GPIO_CFG_6_ADDR,v)
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_6_OUTM(m,v) \
        out_dword_masked_ns(HWIO_SSC_TLMM_LITE_GPIO_CFG_6_ADDR,m,v,HWIO_SSC_TLMM_LITE_GPIO_CFG_6_IN)
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_6_GPIO_MISC_CFG_BMSK                0xf800
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_6_GPIO_MISC_CFG_SHFT                   0xb
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_6_GPIO_HIHYS_CTL_BMSK                0x400
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_6_GPIO_HIHYS_CTL_SHFT                  0xa
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_6_GPIO_OE_BMSK                       0x200
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_6_GPIO_OE_SHFT                         0x9
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_6_GPIO_HDRIVE_BMSK                   0x1c0
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_6_GPIO_HDRIVE_SHFT                     0x6
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_6_GPIO_FUNC_SEL_BMSK                  0x3c
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_6_GPIO_FUNC_SEL_SHFT                   0x2
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_6_GPIO_PULL_BMSK                       0x3
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_6_GPIO_PULL_SHFT                       0x0

#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_6_ADDR                       (SSC_TLMM_CSR_REG_BASE      + 0x00006004)
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_6_RMSK                              0x3
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_6_IN          \
        in_dword(HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_6_ADDR)
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_6_INM(m)      \
        in_dword_masked(HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_6_ADDR, m)
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_6_OUT(v)      \
        out_dword(HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_6_ADDR,v)
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_6_OUTM(m,v) \
        out_dword_masked_ns(HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_6_ADDR,m,v,HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_6_IN)
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_6_GPIO_OUT_BMSK                     0x2
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_6_GPIO_OUT_SHFT                     0x1
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_6_GPIO_IN_BMSK                      0x1
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_6_GPIO_IN_SHFT                      0x0

#define HWIO_SSC_TLMM_LITE_GPIO_CFG_7_ADDR                          (SSC_TLMM_CSR_REG_BASE      + 0x00007000)
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_7_RMSK                              0xffff
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_7_IN          \
        in_dword(HWIO_SSC_TLMM_LITE_GPIO_CFG_7_ADDR)
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_7_INM(m)      \
        in_dword_masked(HWIO_SSC_TLMM_LITE_GPIO_CFG_7_ADDR, m)
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_7_OUT(v)      \
        out_dword(HWIO_SSC_TLMM_LITE_GPIO_CFG_7_ADDR,v)
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_7_OUTM(m,v) \
        out_dword_masked_ns(HWIO_SSC_TLMM_LITE_GPIO_CFG_7_ADDR,m,v,HWIO_SSC_TLMM_LITE_GPIO_CFG_7_IN)
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_7_GPIO_MISC_CFG_BMSK                0xf800
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_7_GPIO_MISC_CFG_SHFT                   0xb
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_7_GPIO_HIHYS_CTL_BMSK                0x400
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_7_GPIO_HIHYS_CTL_SHFT                  0xa
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_7_GPIO_OE_BMSK                       0x200
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_7_GPIO_OE_SHFT                         0x9
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_7_GPIO_HDRIVE_BMSK                   0x1c0
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_7_GPIO_HDRIVE_SHFT                     0x6
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_7_GPIO_FUNC_SEL_BMSK                  0x3c
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_7_GPIO_FUNC_SEL_SHFT                   0x2
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_7_GPIO_PULL_BMSK                       0x3
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_7_GPIO_PULL_SHFT                       0x0

#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_7_ADDR                       (SSC_TLMM_CSR_REG_BASE      + 0x00007004)
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_7_RMSK                              0x3
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_7_IN          \
        in_dword(HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_7_ADDR)
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_7_INM(m)      \
        in_dword_masked(HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_7_ADDR, m)
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_7_OUT(v)      \
        out_dword(HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_7_ADDR,v)
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_7_OUTM(m,v) \
        out_dword_masked_ns(HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_7_ADDR,m,v,HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_7_IN)
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_7_GPIO_OUT_BMSK                     0x2
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_7_GPIO_OUT_SHFT                     0x1
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_7_GPIO_IN_BMSK                      0x1
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_7_GPIO_IN_SHFT                      0x0

#define HWIO_SSC_TLMM_LITE_GPIO_CFG_8_ADDR                          (SSC_TLMM_CSR_REG_BASE      + 0x00008000)
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_8_RMSK                              0xffff
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_8_IN          \
        in_dword(HWIO_SSC_TLMM_LITE_GPIO_CFG_8_ADDR)
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_8_INM(m)      \
        in_dword_masked(HWIO_SSC_TLMM_LITE_GPIO_CFG_8_ADDR, m)
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_8_OUT(v)      \
        out_dword(HWIO_SSC_TLMM_LITE_GPIO_CFG_8_ADDR,v)
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_8_OUTM(m,v) \
        out_dword_masked_ns(HWIO_SSC_TLMM_LITE_GPIO_CFG_8_ADDR,m,v,HWIO_SSC_TLMM_LITE_GPIO_CFG_8_IN)
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_8_GPIO_MISC_CFG_BMSK                0xf800
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_8_GPIO_MISC_CFG_SHFT                   0xb
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_8_GPIO_HIHYS_CTL_BMSK                0x400
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_8_GPIO_HIHYS_CTL_SHFT                  0xa
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_8_GPIO_OE_BMSK                       0x200
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_8_GPIO_OE_SHFT                         0x9
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_8_GPIO_HDRIVE_BMSK                   0x1c0
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_8_GPIO_HDRIVE_SHFT                     0x6
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_8_GPIO_FUNC_SEL_BMSK                  0x3c
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_8_GPIO_FUNC_SEL_SHFT                   0x2
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_8_GPIO_PULL_BMSK                       0x3
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_8_GPIO_PULL_SHFT                       0x0

#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_8_ADDR                       (SSC_TLMM_CSR_REG_BASE      + 0x00008004)
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_8_RMSK                              0x3
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_8_IN          \
        in_dword(HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_8_ADDR)
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_8_INM(m)      \
        in_dword_masked(HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_8_ADDR, m)
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_8_OUT(v)      \
        out_dword(HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_8_ADDR,v)
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_8_OUTM(m,v) \
        out_dword_masked_ns(HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_8_ADDR,m,v,HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_8_IN)
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_8_GPIO_OUT_BMSK                     0x2
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_8_GPIO_OUT_SHFT                     0x1
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_8_GPIO_IN_BMSK                      0x1
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_8_GPIO_IN_SHFT                      0x0

#define HWIO_SSC_TLMM_LITE_GPIO_CFG_9_ADDR                          (SSC_TLMM_CSR_REG_BASE      + 0x00009000)
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_9_RMSK                              0xffff
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_9_IN          \
        in_dword(HWIO_SSC_TLMM_LITE_GPIO_CFG_9_ADDR)
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_9_INM(m)      \
        in_dword_masked(HWIO_SSC_TLMM_LITE_GPIO_CFG_9_ADDR, m)
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_9_OUT(v)      \
        out_dword(HWIO_SSC_TLMM_LITE_GPIO_CFG_9_ADDR,v)
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_9_OUTM(m,v) \
        out_dword_masked_ns(HWIO_SSC_TLMM_LITE_GPIO_CFG_9_ADDR,m,v,HWIO_SSC_TLMM_LITE_GPIO_CFG_9_IN)
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_9_GPIO_MISC_CFG_BMSK                0xf800
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_9_GPIO_MISC_CFG_SHFT                   0xb
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_9_GPIO_HIHYS_CTL_BMSK                0x400
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_9_GPIO_HIHYS_CTL_SHFT                  0xa
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_9_GPIO_OE_BMSK                       0x200
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_9_GPIO_OE_SHFT                         0x9
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_9_GPIO_HDRIVE_BMSK                   0x1c0
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_9_GPIO_HDRIVE_SHFT                     0x6
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_9_GPIO_FUNC_SEL_BMSK                  0x3c
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_9_GPIO_FUNC_SEL_SHFT                   0x2
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_9_GPIO_PULL_BMSK                       0x3
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_9_GPIO_PULL_SHFT                       0x0

#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_9_ADDR                       (SSC_TLMM_CSR_REG_BASE      + 0x00009004)
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_9_RMSK                              0x3
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_9_IN          \
        in_dword(HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_9_ADDR)
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_9_INM(m)      \
        in_dword_masked(HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_9_ADDR, m)
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_9_OUT(v)      \
        out_dword(HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_9_ADDR,v)
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_9_OUTM(m,v) \
        out_dword_masked_ns(HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_9_ADDR,m,v,HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_9_IN)
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_9_GPIO_OUT_BMSK                     0x2
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_9_GPIO_OUT_SHFT                     0x1
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_9_GPIO_IN_BMSK                      0x1
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_9_GPIO_IN_SHFT                      0x0

#define HWIO_SSC_TLMM_LITE_GPIO_CFG_10_ADDR                         (SSC_TLMM_CSR_REG_BASE      + 0x0000a000)
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_10_RMSK                             0xffff
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_10_IN          \
        in_dword(HWIO_SSC_TLMM_LITE_GPIO_CFG_10_ADDR)
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_10_INM(m)      \
        in_dword_masked(HWIO_SSC_TLMM_LITE_GPIO_CFG_10_ADDR, m)
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_10_OUT(v)      \
        out_dword(HWIO_SSC_TLMM_LITE_GPIO_CFG_10_ADDR,v)
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_10_OUTM(m,v) \
        out_dword_masked_ns(HWIO_SSC_TLMM_LITE_GPIO_CFG_10_ADDR,m,v,HWIO_SSC_TLMM_LITE_GPIO_CFG_10_IN)
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_10_GPIO_MISC_CFG_BMSK               0xf800
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_10_GPIO_MISC_CFG_SHFT                  0xb
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_10_GPIO_HIHYS_CTL_BMSK               0x400
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_10_GPIO_HIHYS_CTL_SHFT                 0xa
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_10_GPIO_OE_BMSK                      0x200
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_10_GPIO_OE_SHFT                        0x9
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_10_GPIO_HDRIVE_BMSK                  0x1c0
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_10_GPIO_HDRIVE_SHFT                    0x6
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_10_GPIO_FUNC_SEL_BMSK                 0x3c
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_10_GPIO_FUNC_SEL_SHFT                  0x2
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_10_GPIO_PULL_BMSK                      0x3
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_10_GPIO_PULL_SHFT                      0x0

#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_10_ADDR                      (SSC_TLMM_CSR_REG_BASE      + 0x0000a004)
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_10_RMSK                             0x3
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_10_IN          \
        in_dword(HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_10_ADDR)
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_10_INM(m)      \
        in_dword_masked(HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_10_ADDR, m)
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_10_OUT(v)      \
        out_dword(HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_10_ADDR,v)
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_10_OUTM(m,v) \
        out_dword_masked_ns(HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_10_ADDR,m,v,HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_10_IN)
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_10_GPIO_OUT_BMSK                    0x2
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_10_GPIO_OUT_SHFT                    0x1
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_10_GPIO_IN_BMSK                     0x1
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_10_GPIO_IN_SHFT                     0x0

#define HWIO_SSC_TLMM_LITE_GPIO_CFG_11_ADDR                         (SSC_TLMM_CSR_REG_BASE      + 0x0000b000)
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_11_RMSK                             0xffff
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_11_IN          \
        in_dword(HWIO_SSC_TLMM_LITE_GPIO_CFG_11_ADDR)
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_11_INM(m)      \
        in_dword_masked(HWIO_SSC_TLMM_LITE_GPIO_CFG_11_ADDR, m)
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_11_OUT(v)      \
        out_dword(HWIO_SSC_TLMM_LITE_GPIO_CFG_11_ADDR,v)
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_11_OUTM(m,v) \
        out_dword_masked_ns(HWIO_SSC_TLMM_LITE_GPIO_CFG_11_ADDR,m,v,HWIO_SSC_TLMM_LITE_GPIO_CFG_11_IN)
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_11_GPIO_MISC_CFG_BMSK               0xf800
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_11_GPIO_MISC_CFG_SHFT                  0xb
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_11_GPIO_HIHYS_CTL_BMSK               0x400
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_11_GPIO_HIHYS_CTL_SHFT                 0xa
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_11_GPIO_OE_BMSK                      0x200
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_11_GPIO_OE_SHFT                        0x9
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_11_GPIO_HDRIVE_BMSK                  0x1c0
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_11_GPIO_HDRIVE_SHFT                    0x6
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_11_GPIO_FUNC_SEL_BMSK                 0x3c
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_11_GPIO_FUNC_SEL_SHFT                  0x2
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_11_GPIO_PULL_BMSK                      0x3
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_11_GPIO_PULL_SHFT                      0x0

#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_11_ADDR                      (SSC_TLMM_CSR_REG_BASE      + 0x0000b004)
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_11_RMSK                             0x3
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_11_IN          \
        in_dword(HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_11_ADDR)
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_11_INM(m)      \
        in_dword_masked(HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_11_ADDR, m)
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_11_OUT(v)      \
        out_dword(HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_11_ADDR,v)
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_11_OUTM(m,v) \
        out_dword_masked_ns(HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_11_ADDR,m,v,HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_11_IN)
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_11_GPIO_OUT_BMSK                    0x2
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_11_GPIO_OUT_SHFT                    0x1
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_11_GPIO_IN_BMSK                     0x1
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_11_GPIO_IN_SHFT                     0x0

#define HWIO_SSC_TLMM_LITE_GPIO_CFG_12_ADDR                         (SSC_TLMM_CSR_REG_BASE      + 0x0000c000)
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_12_RMSK                             0xffff
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_12_IN          \
        in_dword(HWIO_SSC_TLMM_LITE_GPIO_CFG_12_ADDR)
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_12_INM(m)      \
        in_dword_masked(HWIO_SSC_TLMM_LITE_GPIO_CFG_12_ADDR, m)
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_12_OUT(v)      \
        out_dword(HWIO_SSC_TLMM_LITE_GPIO_CFG_12_ADDR,v)
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_12_OUTM(m,v) \
        out_dword_masked_ns(HWIO_SSC_TLMM_LITE_GPIO_CFG_12_ADDR,m,v,HWIO_SSC_TLMM_LITE_GPIO_CFG_12_IN)
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_12_GPIO_MISC_CFG_BMSK               0xf800
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_12_GPIO_MISC_CFG_SHFT                  0xb
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_12_GPIO_HIHYS_CTL_BMSK               0x400
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_12_GPIO_HIHYS_CTL_SHFT                 0xa
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_12_GPIO_OE_BMSK                      0x200
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_12_GPIO_OE_SHFT                        0x9
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_12_GPIO_HDRIVE_BMSK                  0x1c0
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_12_GPIO_HDRIVE_SHFT                    0x6
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_12_GPIO_FUNC_SEL_BMSK                 0x3c
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_12_GPIO_FUNC_SEL_SHFT                  0x2
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_12_GPIO_PULL_BMSK                      0x3
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_12_GPIO_PULL_SHFT                      0x0

#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_12_ADDR                      (SSC_TLMM_CSR_REG_BASE      + 0x0000c004)
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_12_RMSK                             0x3
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_12_IN          \
        in_dword(HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_12_ADDR)
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_12_INM(m)      \
        in_dword_masked(HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_12_ADDR, m)
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_12_OUT(v)      \
        out_dword(HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_12_ADDR,v)
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_12_OUTM(m,v) \
        out_dword_masked_ns(HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_12_ADDR,m,v,HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_12_IN)
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_12_GPIO_OUT_BMSK                    0x2
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_12_GPIO_OUT_SHFT                    0x1
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_12_GPIO_IN_BMSK                     0x1
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_12_GPIO_IN_SHFT                     0x0

#define HWIO_SSC_TLMM_LITE_GPIO_CFG_13_ADDR                         (SSC_TLMM_CSR_REG_BASE      + 0x0000d000)
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_13_RMSK                             0xffff
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_13_IN          \
        in_dword(HWIO_SSC_TLMM_LITE_GPIO_CFG_13_ADDR)
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_13_INM(m)      \
        in_dword_masked(HWIO_SSC_TLMM_LITE_GPIO_CFG_13_ADDR, m)
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_13_OUT(v)      \
        out_dword(HWIO_SSC_TLMM_LITE_GPIO_CFG_13_ADDR,v)
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_13_OUTM(m,v) \
        out_dword_masked_ns(HWIO_SSC_TLMM_LITE_GPIO_CFG_13_ADDR,m,v,HWIO_SSC_TLMM_LITE_GPIO_CFG_13_IN)
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_13_GPIO_MISC_CFG_BMSK               0xf800
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_13_GPIO_MISC_CFG_SHFT                  0xb
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_13_GPIO_HIHYS_CTL_BMSK               0x400
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_13_GPIO_HIHYS_CTL_SHFT                 0xa
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_13_GPIO_OE_BMSK                      0x200
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_13_GPIO_OE_SHFT                        0x9
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_13_GPIO_HDRIVE_BMSK                  0x1c0
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_13_GPIO_HDRIVE_SHFT                    0x6
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_13_GPIO_FUNC_SEL_BMSK                 0x3c
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_13_GPIO_FUNC_SEL_SHFT                  0x2
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_13_GPIO_PULL_BMSK                      0x3
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_13_GPIO_PULL_SHFT                      0x0

#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_13_ADDR                      (SSC_TLMM_CSR_REG_BASE      + 0x0000d004)
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_13_RMSK                             0x3
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_13_IN          \
        in_dword(HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_13_ADDR)
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_13_INM(m)      \
        in_dword_masked(HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_13_ADDR, m)
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_13_OUT(v)      \
        out_dword(HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_13_ADDR,v)
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_13_OUTM(m,v) \
        out_dword_masked_ns(HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_13_ADDR,m,v,HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_13_IN)
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_13_GPIO_OUT_BMSK                    0x2
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_13_GPIO_OUT_SHFT                    0x1
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_13_GPIO_IN_BMSK                     0x1
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_13_GPIO_IN_SHFT                     0x0

#define HWIO_SSC_TLMM_LITE_GPIO_CFG_14_ADDR                         (SSC_TLMM_CSR_REG_BASE      + 0x0000e000)
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_14_RMSK                             0xffff
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_14_IN          \
        in_dword(HWIO_SSC_TLMM_LITE_GPIO_CFG_14_ADDR)
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_14_INM(m)      \
        in_dword_masked(HWIO_SSC_TLMM_LITE_GPIO_CFG_14_ADDR, m)
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_14_OUT(v)      \
        out_dword(HWIO_SSC_TLMM_LITE_GPIO_CFG_14_ADDR,v)
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_14_OUTM(m,v) \
        out_dword_masked_ns(HWIO_SSC_TLMM_LITE_GPIO_CFG_14_ADDR,m,v,HWIO_SSC_TLMM_LITE_GPIO_CFG_14_IN)
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_14_GPIO_MISC_CFG_BMSK               0xf800
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_14_GPIO_MISC_CFG_SHFT                  0xb
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_14_GPIO_HIHYS_CTL_BMSK               0x400
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_14_GPIO_HIHYS_CTL_SHFT                 0xa
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_14_GPIO_OE_BMSK                      0x200
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_14_GPIO_OE_SHFT                        0x9
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_14_GPIO_HDRIVE_BMSK                  0x1c0
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_14_GPIO_HDRIVE_SHFT                    0x6
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_14_GPIO_FUNC_SEL_BMSK                 0x3c
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_14_GPIO_FUNC_SEL_SHFT                  0x2
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_14_GPIO_PULL_BMSK                      0x3
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_14_GPIO_PULL_SHFT                      0x0

#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_14_ADDR                      (SSC_TLMM_CSR_REG_BASE      + 0x0000e004)
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_14_RMSK                             0x3
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_14_IN          \
        in_dword(HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_14_ADDR)
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_14_INM(m)      \
        in_dword_masked(HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_14_ADDR, m)
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_14_OUT(v)      \
        out_dword(HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_14_ADDR,v)
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_14_OUTM(m,v) \
        out_dword_masked_ns(HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_14_ADDR,m,v,HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_14_IN)
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_14_GPIO_OUT_BMSK                    0x2
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_14_GPIO_OUT_SHFT                    0x1
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_14_GPIO_IN_BMSK                     0x1
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_14_GPIO_IN_SHFT                     0x0

#define HWIO_SSC_TLMM_LITE_GPIO_CFG_15_ADDR                         (SSC_TLMM_CSR_REG_BASE      + 0x0000f000)
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_15_RMSK                             0xffff
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_15_IN          \
        in_dword(HWIO_SSC_TLMM_LITE_GPIO_CFG_15_ADDR)
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_15_INM(m)      \
        in_dword_masked(HWIO_SSC_TLMM_LITE_GPIO_CFG_15_ADDR, m)
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_15_OUT(v)      \
        out_dword(HWIO_SSC_TLMM_LITE_GPIO_CFG_15_ADDR,v)
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_15_OUTM(m,v) \
        out_dword_masked_ns(HWIO_SSC_TLMM_LITE_GPIO_CFG_15_ADDR,m,v,HWIO_SSC_TLMM_LITE_GPIO_CFG_15_IN)
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_15_GPIO_MISC_CFG_BMSK               0xf800
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_15_GPIO_MISC_CFG_SHFT                  0xb
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_15_GPIO_HIHYS_CTL_BMSK               0x400
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_15_GPIO_HIHYS_CTL_SHFT                 0xa
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_15_GPIO_OE_BMSK                      0x200
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_15_GPIO_OE_SHFT                        0x9
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_15_GPIO_HDRIVE_BMSK                  0x1c0
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_15_GPIO_HDRIVE_SHFT                    0x6
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_15_GPIO_FUNC_SEL_BMSK                 0x3c
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_15_GPIO_FUNC_SEL_SHFT                  0x2
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_15_GPIO_PULL_BMSK                      0x3
#define HWIO_SSC_TLMM_LITE_GPIO_CFG_15_GPIO_PULL_SHFT                      0x0

#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_15_ADDR                      (SSC_TLMM_CSR_REG_BASE      + 0x0000f004)
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_15_RMSK                             0x3
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_15_IN          \
        in_dword(HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_15_ADDR)
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_15_INM(m)      \
        in_dword_masked(HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_15_ADDR, m)
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_15_OUT(v)      \
        out_dword(HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_15_ADDR,v)
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_15_OUTM(m,v) \
        out_dword_masked_ns(HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_15_ADDR,m,v,HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_15_IN)
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_15_GPIO_OUT_BMSK                    0x2
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_15_GPIO_OUT_SHFT                    0x1
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_15_GPIO_IN_BMSK                     0x1
#define HWIO_SSC_TLMM_LITE_GPIO_IN_OUT_15_GPIO_IN_SHFT                     0x0


#endif /* __UIMKAMORTAHWIOMACROS_H__ */
