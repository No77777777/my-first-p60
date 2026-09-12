#ifndef __SPSS_HWIO_H__
#define __SPSS_HWIO_H__
/*
===========================================================================
*/
/**
  @file sp2mpssHWIO.h
  @brief Auto-generated HWIO interface include file.

  Reference chip release:
    SDM845 (Napali) [napali_v1.0_p2q1r33.1.0_F03_ECO]
 
  This file contains HWIO register definitions for the following modules:
    SP_CNOC_SP_SCSR_REGS

  'Include' filters applied: 
  'Exclude' filters applied: RESERVED DUMMY 
*/
/*
  ===========================================================================

  Copyright (c) 2016 Qualcomm Technologies Incorporated.
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

  $Header: //components/rel/core.mpss/10.0/settings/mproc/glink/xport_smem/config/845/spss_hwio.h#1 $
  $DateTime: 2019/04/24 00:03:26 $
  $Author: pwbldsvc $

  ===========================================================================
*/

#include "msmhwiobase.h"

/*----------------------------------------------------------------------------
 * MODULE: SP_CNOC_SP_SCSR_REGS
 *--------------------------------------------------------------------------*/

#define SP_CNOC_SP_SCSR_REGS_REG_BASE                                                     (SP_SCSR_BASE      + 0x00080000)
#define SP_CNOC_SP_SCSR_REGS_REG_BASE_OFFS                                                0x00080000

#define HWIO_SP_CNOC_SP_SCSR_HW_VERSION_ADDR                                              (SP_CNOC_SP_SCSR_REGS_REG_BASE      + 0x00000000)
#define HWIO_SP_CNOC_SP_SCSR_HW_VERSION_OFFS                                              (SP_CNOC_SP_SCSR_REGS_REG_BASE_OFFS + 0x00000000)
#define HWIO_SP_CNOC_SP_SCSR_HW_VERSION_RMSK                                              0xffffffff
#define HWIO_SP_CNOC_SP_SCSR_HW_VERSION_IN          \
        in_dword_masked(HWIO_SP_CNOC_SP_SCSR_HW_VERSION_ADDR, HWIO_SP_CNOC_SP_SCSR_HW_VERSION_RMSK)
#define HWIO_SP_CNOC_SP_SCSR_HW_VERSION_INM(m)      \
        in_dword_masked(HWIO_SP_CNOC_SP_SCSR_HW_VERSION_ADDR, m)
#define HWIO_SP_CNOC_SP_SCSR_HW_VERSION_MAJOR_VERSION_BMSK                                0xff000000
#define HWIO_SP_CNOC_SP_SCSR_HW_VERSION_MAJOR_VERSION_SHFT                                      0x18
#define HWIO_SP_CNOC_SP_SCSR_HW_VERSION_MINOR_VERSION_BMSK                                  0xff0000
#define HWIO_SP_CNOC_SP_SCSR_HW_VERSION_MINOR_VERSION_SHFT                                      0x10
#define HWIO_SP_CNOC_SP_SCSR_HW_VERSION_STEP_VERSION_BMSK                                     0xffff
#define HWIO_SP_CNOC_SP_SCSR_HW_VERSION_STEP_VERSION_SHFT                                        0x0

#define HWIO_SP_CNOC_SP_CORE_HW_VERSION_ADDR                                              (SP_CNOC_SP_SCSR_REGS_REG_BASE      + 0x00000004)
#define HWIO_SP_CNOC_SP_CORE_HW_VERSION_OFFS                                              (SP_CNOC_SP_SCSR_REGS_REG_BASE_OFFS + 0x00000004)
#define HWIO_SP_CNOC_SP_CORE_HW_VERSION_RMSK                                              0xffffffff
#define HWIO_SP_CNOC_SP_CORE_HW_VERSION_IN          \
        in_dword_masked(HWIO_SP_CNOC_SP_CORE_HW_VERSION_ADDR, HWIO_SP_CNOC_SP_CORE_HW_VERSION_RMSK)
#define HWIO_SP_CNOC_SP_CORE_HW_VERSION_INM(m)      \
        in_dword_masked(HWIO_SP_CNOC_SP_CORE_HW_VERSION_ADDR, m)
#define HWIO_SP_CNOC_SP_CORE_HW_VERSION_MAJOR_VERSION_BMSK                                0xff000000
#define HWIO_SP_CNOC_SP_CORE_HW_VERSION_MAJOR_VERSION_SHFT                                      0x18
#define HWIO_SP_CNOC_SP_CORE_HW_VERSION_MINOR_VERSION_BMSK                                  0xff0000
#define HWIO_SP_CNOC_SP_CORE_HW_VERSION_MINOR_VERSION_SHFT                                      0x10
#define HWIO_SP_CNOC_SP_CORE_HW_VERSION_STEP_VERSION_BMSK                                     0xffff
#define HWIO_SP_CNOC_SP_CORE_HW_VERSION_STEP_VERSION_SHFT                                        0x0

#define HWIO_SP_CNOC_SP_SCSR_GP_SCSR2CORE_ADDR                                            (SP_CNOC_SP_SCSR_REGS_REG_BASE      + 0x00000008)
#define HWIO_SP_CNOC_SP_SCSR_GP_SCSR2CORE_OFFS                                            (SP_CNOC_SP_SCSR_REGS_REG_BASE_OFFS + 0x00000008)
#define HWIO_SP_CNOC_SP_SCSR_GP_SCSR2CORE_RMSK                                            0xffffffff
#define HWIO_SP_CNOC_SP_SCSR_GP_SCSR2CORE_IN          \
        in_dword_masked(HWIO_SP_CNOC_SP_SCSR_GP_SCSR2CORE_ADDR, HWIO_SP_CNOC_SP_SCSR_GP_SCSR2CORE_RMSK)
#define HWIO_SP_CNOC_SP_SCSR_GP_SCSR2CORE_INM(m)      \
        in_dword_masked(HWIO_SP_CNOC_SP_SCSR_GP_SCSR2CORE_ADDR, m)
#define HWIO_SP_CNOC_SP_SCSR_GP_SCSR2CORE_OUT(v)      \
        out_dword(HWIO_SP_CNOC_SP_SCSR_GP_SCSR2CORE_ADDR,v)
#define HWIO_SP_CNOC_SP_SCSR_GP_SCSR2CORE_OUTM(m,v) \
        out_dword_masked_ns(HWIO_SP_CNOC_SP_SCSR_GP_SCSR2CORE_ADDR,m,v,HWIO_SP_CNOC_SP_SCSR_GP_SCSR2CORE_IN)
#define HWIO_SP_CNOC_SP_SCSR_GP_SCSR2CORE_GP_BMSK                                         0xffffffff
#define HWIO_SP_CNOC_SP_SCSR_GP_SCSR2CORE_GP_SHFT                                                0x0

#define HWIO_SP_CNOC_SP_SCSR_GP_CORE2SCSR_ADDR                                            (SP_CNOC_SP_SCSR_REGS_REG_BASE      + 0x0000000c)
#define HWIO_SP_CNOC_SP_SCSR_GP_CORE2SCSR_OFFS                                            (SP_CNOC_SP_SCSR_REGS_REG_BASE_OFFS + 0x0000000c)
#define HWIO_SP_CNOC_SP_SCSR_GP_CORE2SCSR_RMSK                                            0xffffffff
#define HWIO_SP_CNOC_SP_SCSR_GP_CORE2SCSR_IN          \
        in_dword_masked(HWIO_SP_CNOC_SP_SCSR_GP_CORE2SCSR_ADDR, HWIO_SP_CNOC_SP_SCSR_GP_CORE2SCSR_RMSK)
#define HWIO_SP_CNOC_SP_SCSR_GP_CORE2SCSR_INM(m)      \
        in_dword_masked(HWIO_SP_CNOC_SP_SCSR_GP_CORE2SCSR_ADDR, m)
#define HWIO_SP_CNOC_SP_SCSR_GP_CORE2SCSR_GP_BMSK                                         0xffffffff
#define HWIO_SP_CNOC_SP_SCSR_GP_CORE2SCSR_GP_SHFT                                                0x0

#define HWIO_SP_CNOC_SP_SCSR_TEST_CODE_ADDR_L_ADDR                                        (SP_CNOC_SP_SCSR_REGS_REG_BASE      + 0x00000010)
#define HWIO_SP_CNOC_SP_SCSR_TEST_CODE_ADDR_L_OFFS                                        (SP_CNOC_SP_SCSR_REGS_REG_BASE_OFFS + 0x00000010)
#define HWIO_SP_CNOC_SP_SCSR_TEST_CODE_ADDR_L_RMSK                                        0xffffffff
#define HWIO_SP_CNOC_SP_SCSR_TEST_CODE_ADDR_L_IN          \
        in_dword_masked(HWIO_SP_CNOC_SP_SCSR_TEST_CODE_ADDR_L_ADDR, HWIO_SP_CNOC_SP_SCSR_TEST_CODE_ADDR_L_RMSK)
#define HWIO_SP_CNOC_SP_SCSR_TEST_CODE_ADDR_L_INM(m)      \
        in_dword_masked(HWIO_SP_CNOC_SP_SCSR_TEST_CODE_ADDR_L_ADDR, m)
#define HWIO_SP_CNOC_SP_SCSR_TEST_CODE_ADDR_L_OUT(v)      \
        out_dword(HWIO_SP_CNOC_SP_SCSR_TEST_CODE_ADDR_L_ADDR,v)
#define HWIO_SP_CNOC_SP_SCSR_TEST_CODE_ADDR_L_OUTM(m,v) \
        out_dword_masked_ns(HWIO_SP_CNOC_SP_SCSR_TEST_CODE_ADDR_L_ADDR,m,v,HWIO_SP_CNOC_SP_SCSR_TEST_CODE_ADDR_L_IN)
#define HWIO_SP_CNOC_SP_SCSR_TEST_CODE_ADDR_L_ADDR_BMSK                                   0xffffffff
#define HWIO_SP_CNOC_SP_SCSR_TEST_CODE_ADDR_L_ADDR_SHFT                                          0x0

#define HWIO_SP_CNOC_SP_SCSR_TEST_CODE_ADDR_U_ADDR                                        (SP_CNOC_SP_SCSR_REGS_REG_BASE      + 0x00000014)
#define HWIO_SP_CNOC_SP_SCSR_TEST_CODE_ADDR_U_OFFS                                        (SP_CNOC_SP_SCSR_REGS_REG_BASE_OFFS + 0x00000014)
#define HWIO_SP_CNOC_SP_SCSR_TEST_CODE_ADDR_U_RMSK                                            0xffff
#define HWIO_SP_CNOC_SP_SCSR_TEST_CODE_ADDR_U_IN          \
        in_dword_masked(HWIO_SP_CNOC_SP_SCSR_TEST_CODE_ADDR_U_ADDR, HWIO_SP_CNOC_SP_SCSR_TEST_CODE_ADDR_U_RMSK)
#define HWIO_SP_CNOC_SP_SCSR_TEST_CODE_ADDR_U_INM(m)      \
        in_dword_masked(HWIO_SP_CNOC_SP_SCSR_TEST_CODE_ADDR_U_ADDR, m)
#define HWIO_SP_CNOC_SP_SCSR_TEST_CODE_ADDR_U_OUT(v)      \
        out_dword(HWIO_SP_CNOC_SP_SCSR_TEST_CODE_ADDR_U_ADDR,v)
#define HWIO_SP_CNOC_SP_SCSR_TEST_CODE_ADDR_U_OUTM(m,v) \
        out_dword_masked_ns(HWIO_SP_CNOC_SP_SCSR_TEST_CODE_ADDR_U_ADDR,m,v,HWIO_SP_CNOC_SP_SCSR_TEST_CODE_ADDR_U_IN)
#define HWIO_SP_CNOC_SP_SCSR_TEST_CODE_ADDR_U_ADDR_BMSK                                       0xffff
#define HWIO_SP_CNOC_SP_SCSR_TEST_CODE_ADDR_U_ADDR_SHFT                                          0x0

#define HWIO_SP_CNOC_SP_SCSR_TEST_CODE_SIZE_ADDR                                          (SP_CNOC_SP_SCSR_REGS_REG_BASE      + 0x00000018)
#define HWIO_SP_CNOC_SP_SCSR_TEST_CODE_SIZE_OFFS                                          (SP_CNOC_SP_SCSR_REGS_REG_BASE_OFFS + 0x00000018)
#define HWIO_SP_CNOC_SP_SCSR_TEST_CODE_SIZE_RMSK                                             0xfffff
#define HWIO_SP_CNOC_SP_SCSR_TEST_CODE_SIZE_IN          \
        in_dword_masked(HWIO_SP_CNOC_SP_SCSR_TEST_CODE_SIZE_ADDR, HWIO_SP_CNOC_SP_SCSR_TEST_CODE_SIZE_RMSK)
#define HWIO_SP_CNOC_SP_SCSR_TEST_CODE_SIZE_INM(m)      \
        in_dword_masked(HWIO_SP_CNOC_SP_SCSR_TEST_CODE_SIZE_ADDR, m)
#define HWIO_SP_CNOC_SP_SCSR_TEST_CODE_SIZE_OUT(v)      \
        out_dword(HWIO_SP_CNOC_SP_SCSR_TEST_CODE_SIZE_ADDR,v)
#define HWIO_SP_CNOC_SP_SCSR_TEST_CODE_SIZE_OUTM(m,v) \
        out_dword_masked_ns(HWIO_SP_CNOC_SP_SCSR_TEST_CODE_SIZE_ADDR,m,v,HWIO_SP_CNOC_SP_SCSR_TEST_CODE_SIZE_IN)
#define HWIO_SP_CNOC_SP_SCSR_TEST_CODE_SIZE_SIZE_BMSK                                        0xfffff
#define HWIO_SP_CNOC_SP_SCSR_TEST_CODE_SIZE_SIZE_SHFT                                            0x0

#define HWIO_SP_CNOC_SP_SCSR_TEST_CODE_ENTRY_OFFSET_ADDR                                  (SP_CNOC_SP_SCSR_REGS_REG_BASE      + 0x0000001c)
#define HWIO_SP_CNOC_SP_SCSR_TEST_CODE_ENTRY_OFFSET_OFFS                                  (SP_CNOC_SP_SCSR_REGS_REG_BASE_OFFS + 0x0000001c)
#define HWIO_SP_CNOC_SP_SCSR_TEST_CODE_ENTRY_OFFSET_RMSK                                     0xfffff
#define HWIO_SP_CNOC_SP_SCSR_TEST_CODE_ENTRY_OFFSET_IN          \
        in_dword_masked(HWIO_SP_CNOC_SP_SCSR_TEST_CODE_ENTRY_OFFSET_ADDR, HWIO_SP_CNOC_SP_SCSR_TEST_CODE_ENTRY_OFFSET_RMSK)
#define HWIO_SP_CNOC_SP_SCSR_TEST_CODE_ENTRY_OFFSET_INM(m)      \
        in_dword_masked(HWIO_SP_CNOC_SP_SCSR_TEST_CODE_ENTRY_OFFSET_ADDR, m)
#define HWIO_SP_CNOC_SP_SCSR_TEST_CODE_ENTRY_OFFSET_OUT(v)      \
        out_dword(HWIO_SP_CNOC_SP_SCSR_TEST_CODE_ENTRY_OFFSET_ADDR,v)
#define HWIO_SP_CNOC_SP_SCSR_TEST_CODE_ENTRY_OFFSET_OUTM(m,v) \
        out_dword_masked_ns(HWIO_SP_CNOC_SP_SCSR_TEST_CODE_ENTRY_OFFSET_ADDR,m,v,HWIO_SP_CNOC_SP_SCSR_TEST_CODE_ENTRY_OFFSET_IN)
#define HWIO_SP_CNOC_SP_SCSR_TEST_CODE_ENTRY_OFFSET_OFFSET_BMSK                              0xfffff
#define HWIO_SP_CNOC_SP_SCSR_TEST_CODE_ENTRY_OFFSET_OFFSET_SHFT                                  0x0

#define HWIO_SP_CNOC_SP_SCSR_OTP_CTRL_ADDR                                                (SP_CNOC_SP_SCSR_REGS_REG_BASE      + 0x00000020)
#define HWIO_SP_CNOC_SP_SCSR_OTP_CTRL_OFFS                                                (SP_CNOC_SP_SCSR_REGS_REG_BASE_OFFS + 0x00000020)
#define HWIO_SP_CNOC_SP_SCSR_OTP_CTRL_RMSK                                                  0xffffff
#define HWIO_SP_CNOC_SP_SCSR_OTP_CTRL_IN          \
        in_dword_masked(HWIO_SP_CNOC_SP_SCSR_OTP_CTRL_ADDR, HWIO_SP_CNOC_SP_SCSR_OTP_CTRL_RMSK)
#define HWIO_SP_CNOC_SP_SCSR_OTP_CTRL_INM(m)      \
        in_dword_masked(HWIO_SP_CNOC_SP_SCSR_OTP_CTRL_ADDR, m)
#define HWIO_SP_CNOC_SP_SCSR_OTP_CTRL_OUT(v)      \
        out_dword(HWIO_SP_CNOC_SP_SCSR_OTP_CTRL_ADDR,v)
#define HWIO_SP_CNOC_SP_SCSR_OTP_CTRL_OUTM(m,v) \
        out_dword_masked_ns(HWIO_SP_CNOC_SP_SCSR_OTP_CTRL_ADDR,m,v,HWIO_SP_CNOC_SP_SCSR_OTP_CTRL_IN)
#define HWIO_SP_CNOC_SP_SCSR_OTP_CTRL_QFPROM_BLOW_TIMER_BMSK                                0xfff000
#define HWIO_SP_CNOC_SP_SCSR_OTP_CTRL_QFPROM_BLOW_TIMER_SHFT                                     0xc
#define HWIO_SP_CNOC_SP_SCSR_OTP_CTRL_QFPROM_ACCEL_GATE_LAST_BMSK                              0x800
#define HWIO_SP_CNOC_SP_SCSR_OTP_CTRL_QFPROM_ACCEL_GATE_LAST_SHFT                                0xb
#define HWIO_SP_CNOC_SP_SCSR_OTP_CTRL_QFPROM_ACCEL_TRIPPT_SEL_BMSK                             0x700
#define HWIO_SP_CNOC_SP_SCSR_OTP_CTRL_QFPROM_ACCEL_TRIPPT_SEL_SHFT                               0x8
#define HWIO_SP_CNOC_SP_SCSR_OTP_CTRL_QFPROM_ACCEL_BMSK                                         0xff
#define HWIO_SP_CNOC_SP_SCSR_OTP_CTRL_QFPROM_ACCEL_SHFT                                          0x0

#define HWIO_SP_CNOC_SP_SCSR_OTP_STATUS_ADDR                                              (SP_CNOC_SP_SCSR_REGS_REG_BASE      + 0x00000024)
#define HWIO_SP_CNOC_SP_SCSR_OTP_STATUS_OFFS                                              (SP_CNOC_SP_SCSR_REGS_REG_BASE_OFFS + 0x00000024)
#define HWIO_SP_CNOC_SP_SCSR_OTP_STATUS_RMSK                                                 0x300ff
#define HWIO_SP_CNOC_SP_SCSR_OTP_STATUS_IN          \
        in_dword_masked(HWIO_SP_CNOC_SP_SCSR_OTP_STATUS_ADDR, HWIO_SP_CNOC_SP_SCSR_OTP_STATUS_RMSK)
#define HWIO_SP_CNOC_SP_SCSR_OTP_STATUS_INM(m)      \
        in_dword_masked(HWIO_SP_CNOC_SP_SCSR_OTP_STATUS_ADDR, m)
#define HWIO_SP_CNOC_SP_SCSR_OTP_STATUS_QFPROM_BLOW_STATUS_WR_ERR_BMSK                       0x20000
#define HWIO_SP_CNOC_SP_SCSR_OTP_STATUS_QFPROM_BLOW_STATUS_WR_ERR_SHFT                          0x11
#define HWIO_SP_CNOC_SP_SCSR_OTP_STATUS_QFPROM_BLOW_STATUS_BUSY_BMSK                         0x10000
#define HWIO_SP_CNOC_SP_SCSR_OTP_STATUS_QFPROM_BLOW_STATUS_BUSY_SHFT                            0x10
#define HWIO_SP_CNOC_SP_SCSR_OTP_STATUS_QFPROM_HW_KEY_BLOW_STATUS_BMSK                          0xff
#define HWIO_SP_CNOC_SP_SCSR_OTP_STATUS_QFPROM_HW_KEY_BLOW_STATUS_SHFT                           0x0

#define HWIO_SP_CNOC_SP_SCSR_OTP_ZEROIZE_EN_ADDR                                          (SP_CNOC_SP_SCSR_REGS_REG_BASE      + 0x00000028)
#define HWIO_SP_CNOC_SP_SCSR_OTP_ZEROIZE_EN_OFFS                                          (SP_CNOC_SP_SCSR_REGS_REG_BASE_OFFS + 0x00000028)
#define HWIO_SP_CNOC_SP_SCSR_OTP_ZEROIZE_EN_RMSK                                                 0x1
#define HWIO_SP_CNOC_SP_SCSR_OTP_ZEROIZE_EN_IN          \
        in_dword_masked(HWIO_SP_CNOC_SP_SCSR_OTP_ZEROIZE_EN_ADDR, HWIO_SP_CNOC_SP_SCSR_OTP_ZEROIZE_EN_RMSK)
#define HWIO_SP_CNOC_SP_SCSR_OTP_ZEROIZE_EN_INM(m)      \
        in_dword_masked(HWIO_SP_CNOC_SP_SCSR_OTP_ZEROIZE_EN_ADDR, m)
#define HWIO_SP_CNOC_SP_SCSR_OTP_ZEROIZE_EN_OUT(v)      \
        out_dword(HWIO_SP_CNOC_SP_SCSR_OTP_ZEROIZE_EN_ADDR,v)
#define HWIO_SP_CNOC_SP_SCSR_OTP_ZEROIZE_EN_OUTM(m,v) \
        out_dword_masked_ns(HWIO_SP_CNOC_SP_SCSR_OTP_ZEROIZE_EN_ADDR,m,v,HWIO_SP_CNOC_SP_SCSR_OTP_ZEROIZE_EN_IN)
#define HWIO_SP_CNOC_SP_SCSR_OTP_ZEROIZE_EN_EN_BMSK                                              0x1
#define HWIO_SP_CNOC_SP_SCSR_OTP_ZEROIZE_EN_EN_SHFT                                              0x0

#define HWIO_SP_CNOC_SP_SCSR_RMB_CODE_ADDR_L_ADDR                                         (SP_CNOC_SP_SCSR_REGS_REG_BASE      + 0x00001000)
#define HWIO_SP_CNOC_SP_SCSR_RMB_CODE_ADDR_L_OFFS                                         (SP_CNOC_SP_SCSR_REGS_REG_BASE_OFFS + 0x00001000)
#define HWIO_SP_CNOC_SP_SCSR_RMB_CODE_ADDR_L_RMSK                                         0xffffffff
#define HWIO_SP_CNOC_SP_SCSR_RMB_CODE_ADDR_L_IN          \
        in_dword_masked(HWIO_SP_CNOC_SP_SCSR_RMB_CODE_ADDR_L_ADDR, HWIO_SP_CNOC_SP_SCSR_RMB_CODE_ADDR_L_RMSK)
#define HWIO_SP_CNOC_SP_SCSR_RMB_CODE_ADDR_L_INM(m)      \
        in_dword_masked(HWIO_SP_CNOC_SP_SCSR_RMB_CODE_ADDR_L_ADDR, m)
#define HWIO_SP_CNOC_SP_SCSR_RMB_CODE_ADDR_L_OUT(v)      \
        out_dword(HWIO_SP_CNOC_SP_SCSR_RMB_CODE_ADDR_L_ADDR,v)
#define HWIO_SP_CNOC_SP_SCSR_RMB_CODE_ADDR_L_OUTM(m,v) \
        out_dword_masked_ns(HWIO_SP_CNOC_SP_SCSR_RMB_CODE_ADDR_L_ADDR,m,v,HWIO_SP_CNOC_SP_SCSR_RMB_CODE_ADDR_L_IN)
#define HWIO_SP_CNOC_SP_SCSR_RMB_CODE_ADDR_L_ADDR_BMSK                                    0xffffffff
#define HWIO_SP_CNOC_SP_SCSR_RMB_CODE_ADDR_L_ADDR_SHFT                                           0x0

#define HWIO_SP_CNOC_SP_SCSR_RMB_CODE_ADDR_U_ADDR                                         (SP_CNOC_SP_SCSR_REGS_REG_BASE      + 0x00001004)
#define HWIO_SP_CNOC_SP_SCSR_RMB_CODE_ADDR_U_OFFS                                         (SP_CNOC_SP_SCSR_REGS_REG_BASE_OFFS + 0x00001004)
#define HWIO_SP_CNOC_SP_SCSR_RMB_CODE_ADDR_U_RMSK                                             0xffff
#define HWIO_SP_CNOC_SP_SCSR_RMB_CODE_ADDR_U_IN          \
        in_dword_masked(HWIO_SP_CNOC_SP_SCSR_RMB_CODE_ADDR_U_ADDR, HWIO_SP_CNOC_SP_SCSR_RMB_CODE_ADDR_U_RMSK)
#define HWIO_SP_CNOC_SP_SCSR_RMB_CODE_ADDR_U_INM(m)      \
        in_dword_masked(HWIO_SP_CNOC_SP_SCSR_RMB_CODE_ADDR_U_ADDR, m)
#define HWIO_SP_CNOC_SP_SCSR_RMB_CODE_ADDR_U_OUT(v)      \
        out_dword(HWIO_SP_CNOC_SP_SCSR_RMB_CODE_ADDR_U_ADDR,v)
#define HWIO_SP_CNOC_SP_SCSR_RMB_CODE_ADDR_U_OUTM(m,v) \
        out_dword_masked_ns(HWIO_SP_CNOC_SP_SCSR_RMB_CODE_ADDR_U_ADDR,m,v,HWIO_SP_CNOC_SP_SCSR_RMB_CODE_ADDR_U_IN)
#define HWIO_SP_CNOC_SP_SCSR_RMB_CODE_ADDR_U_ADDR_BMSK                                        0xffff
#define HWIO_SP_CNOC_SP_SCSR_RMB_CODE_ADDR_U_ADDR_SHFT                                           0x0

#define HWIO_SP_CNOC_SP_SCSR_RMB_CODE_SIZE_ADDR                                           (SP_CNOC_SP_SCSR_REGS_REG_BASE      + 0x00001008)
#define HWIO_SP_CNOC_SP_SCSR_RMB_CODE_SIZE_OFFS                                           (SP_CNOC_SP_SCSR_REGS_REG_BASE_OFFS + 0x00001008)
#define HWIO_SP_CNOC_SP_SCSR_RMB_CODE_SIZE_RMSK                                              0xfffff
#define HWIO_SP_CNOC_SP_SCSR_RMB_CODE_SIZE_IN          \
        in_dword_masked(HWIO_SP_CNOC_SP_SCSR_RMB_CODE_SIZE_ADDR, HWIO_SP_CNOC_SP_SCSR_RMB_CODE_SIZE_RMSK)
#define HWIO_SP_CNOC_SP_SCSR_RMB_CODE_SIZE_INM(m)      \
        in_dword_masked(HWIO_SP_CNOC_SP_SCSR_RMB_CODE_SIZE_ADDR, m)
#define HWIO_SP_CNOC_SP_SCSR_RMB_CODE_SIZE_OUT(v)      \
        out_dword(HWIO_SP_CNOC_SP_SCSR_RMB_CODE_SIZE_ADDR,v)
#define HWIO_SP_CNOC_SP_SCSR_RMB_CODE_SIZE_OUTM(m,v) \
        out_dword_masked_ns(HWIO_SP_CNOC_SP_SCSR_RMB_CODE_SIZE_ADDR,m,v,HWIO_SP_CNOC_SP_SCSR_RMB_CODE_SIZE_IN)
#define HWIO_SP_CNOC_SP_SCSR_RMB_CODE_SIZE_SIZE_BMSK                                         0xfffff
#define HWIO_SP_CNOC_SP_SCSR_RMB_CODE_SIZE_SIZE_SHFT                                             0x0

#define HWIO_SP_CNOC_SP_SCSR_RMB_RAM_DUMP_ADDR_L_ADDR                                     (SP_CNOC_SP_SCSR_REGS_REG_BASE      + 0x0000100c)
#define HWIO_SP_CNOC_SP_SCSR_RMB_RAM_DUMP_ADDR_L_OFFS                                     (SP_CNOC_SP_SCSR_REGS_REG_BASE_OFFS + 0x0000100c)
#define HWIO_SP_CNOC_SP_SCSR_RMB_RAM_DUMP_ADDR_L_RMSK                                     0xffffffff
#define HWIO_SP_CNOC_SP_SCSR_RMB_RAM_DUMP_ADDR_L_IN          \
        in_dword_masked(HWIO_SP_CNOC_SP_SCSR_RMB_RAM_DUMP_ADDR_L_ADDR, HWIO_SP_CNOC_SP_SCSR_RMB_RAM_DUMP_ADDR_L_RMSK)
#define HWIO_SP_CNOC_SP_SCSR_RMB_RAM_DUMP_ADDR_L_INM(m)      \
        in_dword_masked(HWIO_SP_CNOC_SP_SCSR_RMB_RAM_DUMP_ADDR_L_ADDR, m)
#define HWIO_SP_CNOC_SP_SCSR_RMB_RAM_DUMP_ADDR_L_OUT(v)      \
        out_dword(HWIO_SP_CNOC_SP_SCSR_RMB_RAM_DUMP_ADDR_L_ADDR,v)
#define HWIO_SP_CNOC_SP_SCSR_RMB_RAM_DUMP_ADDR_L_OUTM(m,v) \
        out_dword_masked_ns(HWIO_SP_CNOC_SP_SCSR_RMB_RAM_DUMP_ADDR_L_ADDR,m,v,HWIO_SP_CNOC_SP_SCSR_RMB_RAM_DUMP_ADDR_L_IN)
#define HWIO_SP_CNOC_SP_SCSR_RMB_RAM_DUMP_ADDR_L_ADDR_BMSK                                0xffffffff
#define HWIO_SP_CNOC_SP_SCSR_RMB_RAM_DUMP_ADDR_L_ADDR_SHFT                                       0x0

#define HWIO_SP_CNOC_SP_SCSR_RMB_RAM_DUMP_ADDR_U_ADDR                                     (SP_CNOC_SP_SCSR_REGS_REG_BASE      + 0x00001010)
#define HWIO_SP_CNOC_SP_SCSR_RMB_RAM_DUMP_ADDR_U_OFFS                                     (SP_CNOC_SP_SCSR_REGS_REG_BASE_OFFS + 0x00001010)
#define HWIO_SP_CNOC_SP_SCSR_RMB_RAM_DUMP_ADDR_U_RMSK                                         0xffff
#define HWIO_SP_CNOC_SP_SCSR_RMB_RAM_DUMP_ADDR_U_IN          \
        in_dword_masked(HWIO_SP_CNOC_SP_SCSR_RMB_RAM_DUMP_ADDR_U_ADDR, HWIO_SP_CNOC_SP_SCSR_RMB_RAM_DUMP_ADDR_U_RMSK)
#define HWIO_SP_CNOC_SP_SCSR_RMB_RAM_DUMP_ADDR_U_INM(m)      \
        in_dword_masked(HWIO_SP_CNOC_SP_SCSR_RMB_RAM_DUMP_ADDR_U_ADDR, m)
#define HWIO_SP_CNOC_SP_SCSR_RMB_RAM_DUMP_ADDR_U_OUT(v)      \
        out_dword(HWIO_SP_CNOC_SP_SCSR_RMB_RAM_DUMP_ADDR_U_ADDR,v)
#define HWIO_SP_CNOC_SP_SCSR_RMB_RAM_DUMP_ADDR_U_OUTM(m,v) \
        out_dword_masked_ns(HWIO_SP_CNOC_SP_SCSR_RMB_RAM_DUMP_ADDR_U_ADDR,m,v,HWIO_SP_CNOC_SP_SCSR_RMB_RAM_DUMP_ADDR_U_IN)
#define HWIO_SP_CNOC_SP_SCSR_RMB_RAM_DUMP_ADDR_U_ADDR_BMSK                                    0xffff
#define HWIO_SP_CNOC_SP_SCSR_RMB_RAM_DUMP_ADDR_U_ADDR_SHFT                                       0x0

#define HWIO_SP_CNOC_SP_SCSR_RMB_RAM_DUMP_SIZE_ADDR                                       (SP_CNOC_SP_SCSR_REGS_REG_BASE      + 0x00001014)
#define HWIO_SP_CNOC_SP_SCSR_RMB_RAM_DUMP_SIZE_OFFS                                       (SP_CNOC_SP_SCSR_REGS_REG_BASE_OFFS + 0x00001014)
#define HWIO_SP_CNOC_SP_SCSR_RMB_RAM_DUMP_SIZE_RMSK                                          0xfffff
#define HWIO_SP_CNOC_SP_SCSR_RMB_RAM_DUMP_SIZE_IN          \
        in_dword_masked(HWIO_SP_CNOC_SP_SCSR_RMB_RAM_DUMP_SIZE_ADDR, HWIO_SP_CNOC_SP_SCSR_RMB_RAM_DUMP_SIZE_RMSK)
#define HWIO_SP_CNOC_SP_SCSR_RMB_RAM_DUMP_SIZE_INM(m)      \
        in_dword_masked(HWIO_SP_CNOC_SP_SCSR_RMB_RAM_DUMP_SIZE_ADDR, m)
#define HWIO_SP_CNOC_SP_SCSR_RMB_RAM_DUMP_SIZE_OUT(v)      \
        out_dword(HWIO_SP_CNOC_SP_SCSR_RMB_RAM_DUMP_SIZE_ADDR,v)
#define HWIO_SP_CNOC_SP_SCSR_RMB_RAM_DUMP_SIZE_OUTM(m,v) \
        out_dword_masked_ns(HWIO_SP_CNOC_SP_SCSR_RMB_RAM_DUMP_SIZE_ADDR,m,v,HWIO_SP_CNOC_SP_SCSR_RMB_RAM_DUMP_SIZE_IN)
#define HWIO_SP_CNOC_SP_SCSR_RMB_RAM_DUMP_SIZE_SIZE_BMSK                                     0xfffff
#define HWIO_SP_CNOC_SP_SCSR_RMB_RAM_DUMP_SIZE_SIZE_SHFT                                         0x0

#define HWIO_SP_CNOC_SP_SCSR_RMB_BOOT_CTRL_ADDR                                           (SP_CNOC_SP_SCSR_REGS_REG_BASE      + 0x00001018)
#define HWIO_SP_CNOC_SP_SCSR_RMB_BOOT_CTRL_OFFS                                           (SP_CNOC_SP_SCSR_REGS_REG_BASE_OFFS + 0x00001018)
#define HWIO_SP_CNOC_SP_SCSR_RMB_BOOT_CTRL_RMSK                                                  0xf
#define HWIO_SP_CNOC_SP_SCSR_RMB_BOOT_CTRL_IN          \
        in_dword_masked(HWIO_SP_CNOC_SP_SCSR_RMB_BOOT_CTRL_ADDR, HWIO_SP_CNOC_SP_SCSR_RMB_BOOT_CTRL_RMSK)
#define HWIO_SP_CNOC_SP_SCSR_RMB_BOOT_CTRL_INM(m)      \
        in_dword_masked(HWIO_SP_CNOC_SP_SCSR_RMB_BOOT_CTRL_ADDR, m)
#define HWIO_SP_CNOC_SP_SCSR_RMB_BOOT_CTRL_OUT(v)      \
        out_dword(HWIO_SP_CNOC_SP_SCSR_RMB_BOOT_CTRL_ADDR,v)
#define HWIO_SP_CNOC_SP_SCSR_RMB_BOOT_CTRL_OUTM(m,v) \
        out_dword_masked_ns(HWIO_SP_CNOC_SP_SCSR_RMB_BOOT_CTRL_ADDR,m,v,HWIO_SP_CNOC_SP_SCSR_RMB_BOOT_CTRL_IN)
#define HWIO_SP_CNOC_SP_SCSR_RMB_BOOT_CTRL_CPU_BOOT_EN_BMSK                                      0x8
#define HWIO_SP_CNOC_SP_SCSR_RMB_BOOT_CTRL_CPU_BOOT_EN_SHFT                                      0x3
#define HWIO_SP_CNOC_SP_SCSR_RMB_BOOT_CTRL_RAM_WIPE_EN_BMSK                                      0x4
#define HWIO_SP_CNOC_SP_SCSR_RMB_BOOT_CTRL_RAM_WIPE_EN_SHFT                                      0x2
#define HWIO_SP_CNOC_SP_SCSR_RMB_BOOT_CTRL_GDSC_BRINGUP_EN_BMSK                                  0x2
#define HWIO_SP_CNOC_SP_SCSR_RMB_BOOT_CTRL_GDSC_BRINGUP_EN_SHFT                                  0x1
#define HWIO_SP_CNOC_SP_SCSR_RMB_BOOT_CTRL_COLD_BOOT_EN_BMSK                                     0x1
#define HWIO_SP_CNOC_SP_SCSR_RMB_BOOT_CTRL_COLD_BOOT_EN_SHFT                                     0x0

#define HWIO_SP_CNOC_SP_SCSR_RMB_SP2SOC_IRQ_STATUS_ADDR                                   (SP_CNOC_SP_SCSR_REGS_REG_BASE      + 0x0000101c)
#define HWIO_SP_CNOC_SP_SCSR_RMB_SP2SOC_IRQ_STATUS_OFFS                                   (SP_CNOC_SP_SCSR_REGS_REG_BASE_OFFS + 0x0000101c)
#define HWIO_SP_CNOC_SP_SCSR_RMB_SP2SOC_IRQ_STATUS_RMSK                                    0x303000f
#define HWIO_SP_CNOC_SP_SCSR_RMB_SP2SOC_IRQ_STATUS_IN          \
        in_dword_masked(HWIO_SP_CNOC_SP_SCSR_RMB_SP2SOC_IRQ_STATUS_ADDR, HWIO_SP_CNOC_SP_SCSR_RMB_SP2SOC_IRQ_STATUS_RMSK)
#define HWIO_SP_CNOC_SP_SCSR_RMB_SP2SOC_IRQ_STATUS_INM(m)      \
        in_dword_masked(HWIO_SP_CNOC_SP_SCSR_RMB_SP2SOC_IRQ_STATUS_ADDR, m)
#define HWIO_SP_CNOC_SP_SCSR_RMB_SP2SOC_IRQ_STATUS_PBL_DONE_BMSK                           0x2000000
#define HWIO_SP_CNOC_SP_SCSR_RMB_SP2SOC_IRQ_STATUS_PBL_DONE_SHFT                                0x19
#define HWIO_SP_CNOC_SP_SCSR_RMB_SP2SOC_IRQ_STATUS_SW_INIT_DONE_BMSK                       0x1000000
#define HWIO_SP_CNOC_SP_SCSR_RMB_SP2SOC_IRQ_STATUS_SW_INIT_DONE_SHFT                            0x18
#define HWIO_SP_CNOC_SP_SCSR_RMB_SP2SOC_IRQ_STATUS_SPSS_FIRST_PASS_RESET_BMSK                0x20000
#define HWIO_SP_CNOC_SP_SCSR_RMB_SP2SOC_IRQ_STATUS_SPSS_FIRST_PASS_RESET_SHFT                   0x11
#define HWIO_SP_CNOC_SP_SCSR_RMB_SP2SOC_IRQ_STATUS_SPSS_INT_ALT_RESET_BMSK                   0x10000
#define HWIO_SP_CNOC_SP_SCSR_RMB_SP2SOC_IRQ_STATUS_SPSS_INT_ALT_RESET_SHFT                      0x10
#define HWIO_SP_CNOC_SP_SCSR_RMB_SP2SOC_IRQ_STATUS_CPU_BOOT_DONE_BMSK                            0x8
#define HWIO_SP_CNOC_SP_SCSR_RMB_SP2SOC_IRQ_STATUS_CPU_BOOT_DONE_SHFT                            0x3
#define HWIO_SP_CNOC_SP_SCSR_RMB_SP2SOC_IRQ_STATUS_RAM_WIPE_DONE_BMSK                            0x4
#define HWIO_SP_CNOC_SP_SCSR_RMB_SP2SOC_IRQ_STATUS_RAM_WIPE_DONE_SHFT                            0x2
#define HWIO_SP_CNOC_SP_SCSR_RMB_SP2SOC_IRQ_STATUS_GDSC_BRINGUP_DONE_BMSK                        0x2
#define HWIO_SP_CNOC_SP_SCSR_RMB_SP2SOC_IRQ_STATUS_GDSC_BRINGUP_DONE_SHFT                        0x1
#define HWIO_SP_CNOC_SP_SCSR_RMB_SP2SOC_IRQ_STATUS_COLD_BOOT_DONE_BMSK                           0x1
#define HWIO_SP_CNOC_SP_SCSR_RMB_SP2SOC_IRQ_STATUS_COLD_BOOT_DONE_SHFT                           0x0

#define HWIO_SP_CNOC_SP_SCSR_RMB_SP2SOC_IRQ_SET_ADDR                                      (SP_CNOC_SP_SCSR_REGS_REG_BASE      + 0x00001020)
#define HWIO_SP_CNOC_SP_SCSR_RMB_SP2SOC_IRQ_SET_OFFS                                      (SP_CNOC_SP_SCSR_REGS_REG_BASE_OFFS + 0x00001020)
#define HWIO_SP_CNOC_SP_SCSR_RMB_SP2SOC_IRQ_SET_RMSK                                       0x303000f
#define HWIO_SP_CNOC_SP_SCSR_RMB_SP2SOC_IRQ_SET_OUT(v)      \
        out_dword(HWIO_SP_CNOC_SP_SCSR_RMB_SP2SOC_IRQ_SET_ADDR,v)
#define HWIO_SP_CNOC_SP_SCSR_RMB_SP2SOC_IRQ_SET_PBL_DONE_BMSK                              0x2000000
#define HWIO_SP_CNOC_SP_SCSR_RMB_SP2SOC_IRQ_SET_PBL_DONE_SHFT                                   0x19
#define HWIO_SP_CNOC_SP_SCSR_RMB_SP2SOC_IRQ_SET_SW_INIT_DONE_BMSK                          0x1000000
#define HWIO_SP_CNOC_SP_SCSR_RMB_SP2SOC_IRQ_SET_SW_INIT_DONE_SHFT                               0x18
#define HWIO_SP_CNOC_SP_SCSR_RMB_SP2SOC_IRQ_SET_SPSS_FIRST_PASS_RESET_BMSK                   0x20000
#define HWIO_SP_CNOC_SP_SCSR_RMB_SP2SOC_IRQ_SET_SPSS_FIRST_PASS_RESET_SHFT                      0x11
#define HWIO_SP_CNOC_SP_SCSR_RMB_SP2SOC_IRQ_SET_SPSS_INT_ALT_RESET_BMSK                      0x10000
#define HWIO_SP_CNOC_SP_SCSR_RMB_SP2SOC_IRQ_SET_SPSS_INT_ALT_RESET_SHFT                         0x10
#define HWIO_SP_CNOC_SP_SCSR_RMB_SP2SOC_IRQ_SET_CPU_BOOT_DONE_BMSK                               0x8
#define HWIO_SP_CNOC_SP_SCSR_RMB_SP2SOC_IRQ_SET_CPU_BOOT_DONE_SHFT                               0x3
#define HWIO_SP_CNOC_SP_SCSR_RMB_SP2SOC_IRQ_SET_RAM_WIPE_DONE_BMSK                               0x4
#define HWIO_SP_CNOC_SP_SCSR_RMB_SP2SOC_IRQ_SET_RAM_WIPE_DONE_SHFT                               0x2
#define HWIO_SP_CNOC_SP_SCSR_RMB_SP2SOC_IRQ_SET_GDSC_BRINGUP_DONE_BMSK                           0x2
#define HWIO_SP_CNOC_SP_SCSR_RMB_SP2SOC_IRQ_SET_GDSC_BRINGUP_DONE_SHFT                           0x1
#define HWIO_SP_CNOC_SP_SCSR_RMB_SP2SOC_IRQ_SET_COLD_BOOT_DONE_BMSK                              0x1
#define HWIO_SP_CNOC_SP_SCSR_RMB_SP2SOC_IRQ_SET_COLD_BOOT_DONE_SHFT                              0x0

#define HWIO_SP_CNOC_SP_SCSR_RMB_SP2SOC_IRQ_CLR_ADDR                                      (SP_CNOC_SP_SCSR_REGS_REG_BASE      + 0x00001024)
#define HWIO_SP_CNOC_SP_SCSR_RMB_SP2SOC_IRQ_CLR_OFFS                                      (SP_CNOC_SP_SCSR_REGS_REG_BASE_OFFS + 0x00001024)
#define HWIO_SP_CNOC_SP_SCSR_RMB_SP2SOC_IRQ_CLR_RMSK                                       0x303000f
#define HWIO_SP_CNOC_SP_SCSR_RMB_SP2SOC_IRQ_CLR_OUT(v)      \
        out_dword(HWIO_SP_CNOC_SP_SCSR_RMB_SP2SOC_IRQ_CLR_ADDR,v)
#define HWIO_SP_CNOC_SP_SCSR_RMB_SP2SOC_IRQ_CLR_PBL_DONE_BMSK                              0x2000000
#define HWIO_SP_CNOC_SP_SCSR_RMB_SP2SOC_IRQ_CLR_PBL_DONE_SHFT                                   0x19
#define HWIO_SP_CNOC_SP_SCSR_RMB_SP2SOC_IRQ_CLR_SW_INIT_DONE_BMSK                          0x1000000
#define HWIO_SP_CNOC_SP_SCSR_RMB_SP2SOC_IRQ_CLR_SW_INIT_DONE_SHFT                               0x18
#define HWIO_SP_CNOC_SP_SCSR_RMB_SP2SOC_IRQ_CLR_SPSS_FIRST_PASS_RESET_BMSK                   0x20000
#define HWIO_SP_CNOC_SP_SCSR_RMB_SP2SOC_IRQ_CLR_SPSS_FIRST_PASS_RESET_SHFT                      0x11
#define HWIO_SP_CNOC_SP_SCSR_RMB_SP2SOC_IRQ_CLR_SPSS_INT_ALT_RESET_BMSK                      0x10000
#define HWIO_SP_CNOC_SP_SCSR_RMB_SP2SOC_IRQ_CLR_SPSS_INT_ALT_RESET_SHFT                         0x10
#define HWIO_SP_CNOC_SP_SCSR_RMB_SP2SOC_IRQ_CLR_CPU_BOOT_DONE_BMSK                               0x8
#define HWIO_SP_CNOC_SP_SCSR_RMB_SP2SOC_IRQ_CLR_CPU_BOOT_DONE_SHFT                               0x3
#define HWIO_SP_CNOC_SP_SCSR_RMB_SP2SOC_IRQ_CLR_RAM_WIPE_DONE_BMSK                               0x4
#define HWIO_SP_CNOC_SP_SCSR_RMB_SP2SOC_IRQ_CLR_RAM_WIPE_DONE_SHFT                               0x2
#define HWIO_SP_CNOC_SP_SCSR_RMB_SP2SOC_IRQ_CLR_GDSC_BRINGUP_DONE_BMSK                           0x2
#define HWIO_SP_CNOC_SP_SCSR_RMB_SP2SOC_IRQ_CLR_GDSC_BRINGUP_DONE_SHFT                           0x1
#define HWIO_SP_CNOC_SP_SCSR_RMB_SP2SOC_IRQ_CLR_COLD_BOOT_DONE_BMSK                              0x1
#define HWIO_SP_CNOC_SP_SCSR_RMB_SP2SOC_IRQ_CLR_COLD_BOOT_DONE_SHFT                              0x0

#define HWIO_SP_CNOC_SP_SCSR_RMB_SP2SOC_IRQ_MASK_ADDR                                     (SP_CNOC_SP_SCSR_REGS_REG_BASE      + 0x00001028)
#define HWIO_SP_CNOC_SP_SCSR_RMB_SP2SOC_IRQ_MASK_OFFS                                     (SP_CNOC_SP_SCSR_REGS_REG_BASE_OFFS + 0x00001028)
#define HWIO_SP_CNOC_SP_SCSR_RMB_SP2SOC_IRQ_MASK_RMSK                                      0x303000f
#define HWIO_SP_CNOC_SP_SCSR_RMB_SP2SOC_IRQ_MASK_IN          \
        in_dword_masked(HWIO_SP_CNOC_SP_SCSR_RMB_SP2SOC_IRQ_MASK_ADDR, HWIO_SP_CNOC_SP_SCSR_RMB_SP2SOC_IRQ_MASK_RMSK)
#define HWIO_SP_CNOC_SP_SCSR_RMB_SP2SOC_IRQ_MASK_INM(m)      \
        in_dword_masked(HWIO_SP_CNOC_SP_SCSR_RMB_SP2SOC_IRQ_MASK_ADDR, m)
#define HWIO_SP_CNOC_SP_SCSR_RMB_SP2SOC_IRQ_MASK_OUT(v)      \
        out_dword(HWIO_SP_CNOC_SP_SCSR_RMB_SP2SOC_IRQ_MASK_ADDR,v)
#define HWIO_SP_CNOC_SP_SCSR_RMB_SP2SOC_IRQ_MASK_OUTM(m,v) \
        out_dword_masked_ns(HWIO_SP_CNOC_SP_SCSR_RMB_SP2SOC_IRQ_MASK_ADDR,m,v,HWIO_SP_CNOC_SP_SCSR_RMB_SP2SOC_IRQ_MASK_IN)
#define HWIO_SP_CNOC_SP_SCSR_RMB_SP2SOC_IRQ_MASK_PBL_DONE_BMSK                             0x2000000
#define HWIO_SP_CNOC_SP_SCSR_RMB_SP2SOC_IRQ_MASK_PBL_DONE_SHFT                                  0x19
#define HWIO_SP_CNOC_SP_SCSR_RMB_SP2SOC_IRQ_MASK_SW_INIT_DONE_BMSK                         0x1000000
#define HWIO_SP_CNOC_SP_SCSR_RMB_SP2SOC_IRQ_MASK_SW_INIT_DONE_SHFT                              0x18
#define HWIO_SP_CNOC_SP_SCSR_RMB_SP2SOC_IRQ_MASK_SPSS_FIRST_PASS_RESET_BMSK                  0x20000
#define HWIO_SP_CNOC_SP_SCSR_RMB_SP2SOC_IRQ_MASK_SPSS_FIRST_PASS_RESET_SHFT                     0x11
#define HWIO_SP_CNOC_SP_SCSR_RMB_SP2SOC_IRQ_MASK_SPSS_INT_ALT_RESET_BMSK                     0x10000
#define HWIO_SP_CNOC_SP_SCSR_RMB_SP2SOC_IRQ_MASK_SPSS_INT_ALT_RESET_SHFT                        0x10
#define HWIO_SP_CNOC_SP_SCSR_RMB_SP2SOC_IRQ_MASK_CPU_BOOT_DONE_BMSK                              0x8
#define HWIO_SP_CNOC_SP_SCSR_RMB_SP2SOC_IRQ_MASK_CPU_BOOT_DONE_SHFT                              0x3
#define HWIO_SP_CNOC_SP_SCSR_RMB_SP2SOC_IRQ_MASK_RAM_WIPE_DONE_BMSK                              0x4
#define HWIO_SP_CNOC_SP_SCSR_RMB_SP2SOC_IRQ_MASK_RAM_WIPE_DONE_SHFT                              0x2
#define HWIO_SP_CNOC_SP_SCSR_RMB_SP2SOC_IRQ_MASK_GDSC_BRINGUP_DONE_BMSK                          0x2
#define HWIO_SP_CNOC_SP_SCSR_RMB_SP2SOC_IRQ_MASK_GDSC_BRINGUP_DONE_SHFT                          0x1
#define HWIO_SP_CNOC_SP_SCSR_RMB_SP2SOC_IRQ_MASK_COLD_BOOT_DONE_BMSK                             0x1
#define HWIO_SP_CNOC_SP_SCSR_RMB_SP2SOC_IRQ_MASK_COLD_BOOT_DONE_SHFT                             0x0

#define HWIO_SP_CNOC_SP_SCSR_RMB_SOC2SP_IRQ_STATUS_ADDR                                   (SP_CNOC_SP_SCSR_REGS_REG_BASE      + 0x0000102c)
#define HWIO_SP_CNOC_SP_SCSR_RMB_SOC2SP_IRQ_STATUS_OFFS                                   (SP_CNOC_SP_SCSR_REGS_REG_BASE_OFFS + 0x0000102c)
#define HWIO_SP_CNOC_SP_SCSR_RMB_SOC2SP_IRQ_STATUS_RMSK                                          0xf
#define HWIO_SP_CNOC_SP_SCSR_RMB_SOC2SP_IRQ_STATUS_IN          \
        in_dword_masked(HWIO_SP_CNOC_SP_SCSR_RMB_SOC2SP_IRQ_STATUS_ADDR, HWIO_SP_CNOC_SP_SCSR_RMB_SOC2SP_IRQ_STATUS_RMSK)
#define HWIO_SP_CNOC_SP_SCSR_RMB_SOC2SP_IRQ_STATUS_INM(m)      \
        in_dword_masked(HWIO_SP_CNOC_SP_SCSR_RMB_SOC2SP_IRQ_STATUS_ADDR, m)
#define HWIO_SP_CNOC_SP_SCSR_RMB_SOC2SP_IRQ_STATUS_IRQ_BMSK                                      0xf
#define HWIO_SP_CNOC_SP_SCSR_RMB_SOC2SP_IRQ_STATUS_IRQ_SHFT                                      0x0

#define HWIO_SP_CNOC_SP_SCSR_RMB_SOC2SP_IRQ_SET_ADDR                                      (SP_CNOC_SP_SCSR_REGS_REG_BASE      + 0x00001030)
#define HWIO_SP_CNOC_SP_SCSR_RMB_SOC2SP_IRQ_SET_OFFS                                      (SP_CNOC_SP_SCSR_REGS_REG_BASE_OFFS + 0x00001030)
#define HWIO_SP_CNOC_SP_SCSR_RMB_SOC2SP_IRQ_SET_RMSK                                             0xf
#define HWIO_SP_CNOC_SP_SCSR_RMB_SOC2SP_IRQ_SET_OUT(v)      \
        out_dword(HWIO_SP_CNOC_SP_SCSR_RMB_SOC2SP_IRQ_SET_ADDR,v)
#define HWIO_SP_CNOC_SP_SCSR_RMB_SOC2SP_IRQ_SET_IRQ_BMSK                                         0xf
#define HWIO_SP_CNOC_SP_SCSR_RMB_SOC2SP_IRQ_SET_IRQ_SHFT                                         0x0

#define HWIO_SP_CNOC_SP_SCSR_RMB_SOC2SP_IRQ_CLR_ADDR                                      (SP_CNOC_SP_SCSR_REGS_REG_BASE      + 0x00001034)
#define HWIO_SP_CNOC_SP_SCSR_RMB_SOC2SP_IRQ_CLR_OFFS                                      (SP_CNOC_SP_SCSR_REGS_REG_BASE_OFFS + 0x00001034)
#define HWIO_SP_CNOC_SP_SCSR_RMB_SOC2SP_IRQ_CLR_RMSK                                             0xf
#define HWIO_SP_CNOC_SP_SCSR_RMB_SOC2SP_IRQ_CLR_OUT(v)      \
        out_dword(HWIO_SP_CNOC_SP_SCSR_RMB_SOC2SP_IRQ_CLR_ADDR,v)
#define HWIO_SP_CNOC_SP_SCSR_RMB_SOC2SP_IRQ_CLR_IRQ_BMSK                                         0xf
#define HWIO_SP_CNOC_SP_SCSR_RMB_SOC2SP_IRQ_CLR_IRQ_SHFT                                         0x0

#define HWIO_SP_CNOC_SP_SCSR_RMB_SOC2SP_IRQ_MASK_ADDR                                     (SP_CNOC_SP_SCSR_REGS_REG_BASE      + 0x00001038)
#define HWIO_SP_CNOC_SP_SCSR_RMB_SOC2SP_IRQ_MASK_OFFS                                     (SP_CNOC_SP_SCSR_REGS_REG_BASE_OFFS + 0x00001038)
#define HWIO_SP_CNOC_SP_SCSR_RMB_SOC2SP_IRQ_MASK_RMSK                                            0xf
#define HWIO_SP_CNOC_SP_SCSR_RMB_SOC2SP_IRQ_MASK_IN          \
        in_dword_masked(HWIO_SP_CNOC_SP_SCSR_RMB_SOC2SP_IRQ_MASK_ADDR, HWIO_SP_CNOC_SP_SCSR_RMB_SOC2SP_IRQ_MASK_RMSK)
#define HWIO_SP_CNOC_SP_SCSR_RMB_SOC2SP_IRQ_MASK_INM(m)      \
        in_dword_masked(HWIO_SP_CNOC_SP_SCSR_RMB_SOC2SP_IRQ_MASK_ADDR, m)
#define HWIO_SP_CNOC_SP_SCSR_RMB_SOC2SP_IRQ_MASK_OUT(v)      \
        out_dword(HWIO_SP_CNOC_SP_SCSR_RMB_SOC2SP_IRQ_MASK_ADDR,v)
#define HWIO_SP_CNOC_SP_SCSR_RMB_SOC2SP_IRQ_MASK_OUTM(m,v) \
        out_dword_masked_ns(HWIO_SP_CNOC_SP_SCSR_RMB_SOC2SP_IRQ_MASK_ADDR,m,v,HWIO_SP_CNOC_SP_SCSR_RMB_SOC2SP_IRQ_MASK_IN)
#define HWIO_SP_CNOC_SP_SCSR_RMB_SOC2SP_IRQ_MASK_IRQ_BMSK                                        0xf
#define HWIO_SP_CNOC_SP_SCSR_RMB_SOC2SP_IRQ_MASK_IRQ_SHFT                                        0x0

#define HWIO_SP_CNOC_SP_SCSR_RMB_ERR_STATUS_ADDR                                          (SP_CNOC_SP_SCSR_REGS_REG_BASE      + 0x0000103c)
#define HWIO_SP_CNOC_SP_SCSR_RMB_ERR_STATUS_OFFS                                          (SP_CNOC_SP_SCSR_REGS_REG_BASE_OFFS + 0x0000103c)
#define HWIO_SP_CNOC_SP_SCSR_RMB_ERR_STATUS_RMSK                                          0xffffffff
#define HWIO_SP_CNOC_SP_SCSR_RMB_ERR_STATUS_IN          \
        in_dword_masked(HWIO_SP_CNOC_SP_SCSR_RMB_ERR_STATUS_ADDR, HWIO_SP_CNOC_SP_SCSR_RMB_ERR_STATUS_RMSK)
#define HWIO_SP_CNOC_SP_SCSR_RMB_ERR_STATUS_INM(m)      \
        in_dword_masked(HWIO_SP_CNOC_SP_SCSR_RMB_ERR_STATUS_ADDR, m)
#define HWIO_SP_CNOC_SP_SCSR_RMB_ERR_STATUS_OUT(v)      \
        out_dword(HWIO_SP_CNOC_SP_SCSR_RMB_ERR_STATUS_ADDR,v)
#define HWIO_SP_CNOC_SP_SCSR_RMB_ERR_STATUS_OUTM(m,v) \
        out_dword_masked_ns(HWIO_SP_CNOC_SP_SCSR_RMB_ERR_STATUS_ADDR,m,v,HWIO_SP_CNOC_SP_SCSR_RMB_ERR_STATUS_IN)
#define HWIO_SP_CNOC_SP_SCSR_RMB_ERR_STATUS_STATUS_BMSK                                   0xffffffff
#define HWIO_SP_CNOC_SP_SCSR_RMB_ERR_STATUS_STATUS_SHFT                                          0x0

#define HWIO_SP_CNOC_SP_SCSR_RMB_GPm_ADDR(m)                                              (SP_CNOC_SP_SCSR_REGS_REG_BASE      + 0x00001100 + 0x4 * (m))
#define HWIO_SP_CNOC_SP_SCSR_RMB_GPm_OFFS(m)                                              (SP_CNOC_SP_SCSR_REGS_REG_BASE_OFFS + 0x00001100 + 0x4 * (m))
#define HWIO_SP_CNOC_SP_SCSR_RMB_GPm_RMSK                                                     0xffff
#define HWIO_SP_CNOC_SP_SCSR_RMB_GPm_MAXm                                                          1
#define HWIO_SP_CNOC_SP_SCSR_RMB_GPm_INI(m)        \
        in_dword_masked(HWIO_SP_CNOC_SP_SCSR_RMB_GPm_ADDR(m), HWIO_SP_CNOC_SP_SCSR_RMB_GPm_RMSK)
#define HWIO_SP_CNOC_SP_SCSR_RMB_GPm_INMI(m,mask)    \
        in_dword_masked(HWIO_SP_CNOC_SP_SCSR_RMB_GPm_ADDR(m), mask)
#define HWIO_SP_CNOC_SP_SCSR_RMB_GPm_OUTI(m,val)    \
        out_dword(HWIO_SP_CNOC_SP_SCSR_RMB_GPm_ADDR(m),val)
#define HWIO_SP_CNOC_SP_SCSR_RMB_GPm_OUTMI(m,mask,val) \
        out_dword_masked_ns(HWIO_SP_CNOC_SP_SCSR_RMB_GPm_ADDR(m),mask,val,HWIO_SP_CNOC_SP_SCSR_RMB_GPm_INI(m))
#define HWIO_SP_CNOC_SP_SCSR_RMB_GPm_GP_BMSK                                                  0xffff
#define HWIO_SP_CNOC_SP_SCSR_RMB_GPm_GP_SHFT                                                     0x0

#define HWIO_SP_CNOC_SP_SCSR_SOC_HW_VERSION_ADDR                                          (SP_CNOC_SP_SCSR_REGS_REG_BASE      + 0x00001200)
#define HWIO_SP_CNOC_SP_SCSR_SOC_HW_VERSION_OFFS                                          (SP_CNOC_SP_SCSR_REGS_REG_BASE_OFFS + 0x00001200)
#define HWIO_SP_CNOC_SP_SCSR_SOC_HW_VERSION_RMSK                                          0xffffffff
#define HWIO_SP_CNOC_SP_SCSR_SOC_HW_VERSION_IN          \
        in_dword_masked(HWIO_SP_CNOC_SP_SCSR_SOC_HW_VERSION_ADDR, HWIO_SP_CNOC_SP_SCSR_SOC_HW_VERSION_RMSK)
#define HWIO_SP_CNOC_SP_SCSR_SOC_HW_VERSION_INM(m)      \
        in_dword_masked(HWIO_SP_CNOC_SP_SCSR_SOC_HW_VERSION_ADDR, m)
#define HWIO_SP_CNOC_SP_SCSR_SOC_HW_VERSION_FAMILY_NUMBER_BMSK                            0xf0000000
#define HWIO_SP_CNOC_SP_SCSR_SOC_HW_VERSION_FAMILY_NUMBER_SHFT                                  0x1c
#define HWIO_SP_CNOC_SP_SCSR_SOC_HW_VERSION_DEVICE_NUMBER_BMSK                             0xfff0000
#define HWIO_SP_CNOC_SP_SCSR_SOC_HW_VERSION_DEVICE_NUMBER_SHFT                                  0x10
#define HWIO_SP_CNOC_SP_SCSR_SOC_HW_VERSION_MAJOR_VERSION_BMSK                                0xff00
#define HWIO_SP_CNOC_SP_SCSR_SOC_HW_VERSION_MAJOR_VERSION_SHFT                                   0x8
#define HWIO_SP_CNOC_SP_SCSR_SOC_HW_VERSION_MINOR_VERSION_BMSK                                  0xff
#define HWIO_SP_CNOC_SP_SCSR_SOC_HW_VERSION_MINOR_VERSION_SHFT                                   0x0

#define HWIO_SP_CNOC_SP_SCSR_JTAG_ID_ADDR                                                 (SP_CNOC_SP_SCSR_REGS_REG_BASE      + 0x00001204)
#define HWIO_SP_CNOC_SP_SCSR_JTAG_ID_OFFS                                                 (SP_CNOC_SP_SCSR_REGS_REG_BASE_OFFS + 0x00001204)
#define HWIO_SP_CNOC_SP_SCSR_JTAG_ID_RMSK                                                 0xffffffff
#define HWIO_SP_CNOC_SP_SCSR_JTAG_ID_IN          \
        in_dword_masked(HWIO_SP_CNOC_SP_SCSR_JTAG_ID_ADDR, HWIO_SP_CNOC_SP_SCSR_JTAG_ID_RMSK)
#define HWIO_SP_CNOC_SP_SCSR_JTAG_ID_INM(m)      \
        in_dword_masked(HWIO_SP_CNOC_SP_SCSR_JTAG_ID_ADDR, m)
#define HWIO_SP_CNOC_SP_SCSR_JTAG_ID_JTAG_ID_BMSK                                         0xffffffff
#define HWIO_SP_CNOC_SP_SCSR_JTAG_ID_JTAG_ID_SHFT                                                0x0

#define HWIO_SP_CNOC_SP_SCSR_OEM_ID_ADDR                                                  (SP_CNOC_SP_SCSR_REGS_REG_BASE      + 0x00001208)
#define HWIO_SP_CNOC_SP_SCSR_OEM_ID_OFFS                                                  (SP_CNOC_SP_SCSR_REGS_REG_BASE_OFFS + 0x00001208)
#define HWIO_SP_CNOC_SP_SCSR_OEM_ID_RMSK                                                      0xffff
#define HWIO_SP_CNOC_SP_SCSR_OEM_ID_IN          \
        in_dword_masked(HWIO_SP_CNOC_SP_SCSR_OEM_ID_ADDR, HWIO_SP_CNOC_SP_SCSR_OEM_ID_RMSK)
#define HWIO_SP_CNOC_SP_SCSR_OEM_ID_INM(m)      \
        in_dword_masked(HWIO_SP_CNOC_SP_SCSR_OEM_ID_ADDR, m)
#define HWIO_SP_CNOC_SP_SCSR_OEM_ID_OEM_ID_BMSK                                               0xffff
#define HWIO_SP_CNOC_SP_SCSR_OEM_ID_OEM_ID_SHFT                                                  0x0

#define HWIO_SP_CNOC_SP_SCSR_SERIAL_NUM_ADDR                                              (SP_CNOC_SP_SCSR_REGS_REG_BASE      + 0x0000120c)
#define HWIO_SP_CNOC_SP_SCSR_SERIAL_NUM_OFFS                                              (SP_CNOC_SP_SCSR_REGS_REG_BASE_OFFS + 0x0000120c)
#define HWIO_SP_CNOC_SP_SCSR_SERIAL_NUM_RMSK                                              0xffffffff
#define HWIO_SP_CNOC_SP_SCSR_SERIAL_NUM_IN          \
        in_dword_masked(HWIO_SP_CNOC_SP_SCSR_SERIAL_NUM_ADDR, HWIO_SP_CNOC_SP_SCSR_SERIAL_NUM_RMSK)
#define HWIO_SP_CNOC_SP_SCSR_SERIAL_NUM_INM(m)      \
        in_dword_masked(HWIO_SP_CNOC_SP_SCSR_SERIAL_NUM_ADDR, m)
#define HWIO_SP_CNOC_SP_SCSR_SERIAL_NUM_SERIAL_NUM_BMSK                                   0xffffffff
#define HWIO_SP_CNOC_SP_SCSR_SERIAL_NUM_SERIAL_NUM_SHFT                                          0x0

#define HWIO_SP_CNOC_SP_SCSR_DBG_STATUS_ADDR                                              (SP_CNOC_SP_SCSR_REGS_REG_BASE      + 0x00002000)
#define HWIO_SP_CNOC_SP_SCSR_DBG_STATUS_OFFS                                              (SP_CNOC_SP_SCSR_REGS_REG_BASE_OFFS + 0x00002000)
#define HWIO_SP_CNOC_SP_SCSR_DBG_STATUS_RMSK                                                    0x7f
#define HWIO_SP_CNOC_SP_SCSR_DBG_STATUS_IN          \
        in_dword_masked(HWIO_SP_CNOC_SP_SCSR_DBG_STATUS_ADDR, HWIO_SP_CNOC_SP_SCSR_DBG_STATUS_RMSK)
#define HWIO_SP_CNOC_SP_SCSR_DBG_STATUS_INM(m)      \
        in_dword_masked(HWIO_SP_CNOC_SP_SCSR_DBG_STATUS_ADDR, m)
#define HWIO_SP_CNOC_SP_SCSR_DBG_STATUS_DBGEN_EN_BMSK                                           0x40
#define HWIO_SP_CNOC_SP_SCSR_DBG_STATUS_DBGEN_EN_SHFT                                            0x6
#define HWIO_SP_CNOC_SP_SCSR_DBG_STATUS_DAPEN_EN_BMSK                                           0x20
#define HWIO_SP_CNOC_SP_SCSR_DBG_STATUS_DAPEN_EN_SHFT                                            0x5
#define HWIO_SP_CNOC_SP_SCSR_DBG_STATUS_NIDEN_EN_BMSK                                           0x10
#define HWIO_SP_CNOC_SP_SCSR_DBG_STATUS_NIDEN_EN_SHFT                                            0x4
#define HWIO_SP_CNOC_SP_SCSR_DBG_STATUS_TEST_EN_BMSK                                             0x8
#define HWIO_SP_CNOC_SP_SCSR_DBG_STATUS_TEST_EN_SHFT                                             0x3
#define HWIO_SP_CNOC_SP_SCSR_DBG_STATUS_SCAN_OUT_EN_BMSK                                         0x4
#define HWIO_SP_CNOC_SP_SCSR_DBG_STATUS_SCAN_OUT_EN_SHFT                                         0x2
#define HWIO_SP_CNOC_SP_SCSR_DBG_STATUS_BOOT_FROM_ROM_N_EN_BMSK                                  0x2
#define HWIO_SP_CNOC_SP_SCSR_DBG_STATUS_BOOT_FROM_ROM_N_EN_SHFT                                  0x1
#define HWIO_SP_CNOC_SP_SCSR_DBG_STATUS_DUMP_MODE_EN_BMSK                                        0x1
#define HWIO_SP_CNOC_SP_SCSR_DBG_STATUS_DUMP_MODE_EN_SHFT                                        0x0

#define HWIO_SP_CNOC_SP_SCSR_COLD_BOOT_FSM_STATE_ADDR                                     (SP_CNOC_SP_SCSR_REGS_REG_BASE      + 0x00002004)
#define HWIO_SP_CNOC_SP_SCSR_COLD_BOOT_FSM_STATE_OFFS                                     (SP_CNOC_SP_SCSR_REGS_REG_BASE_OFFS + 0x00002004)
#define HWIO_SP_CNOC_SP_SCSR_COLD_BOOT_FSM_STATE_RMSK                                           0x7f
#define HWIO_SP_CNOC_SP_SCSR_COLD_BOOT_FSM_STATE_IN          \
        in_dword_masked(HWIO_SP_CNOC_SP_SCSR_COLD_BOOT_FSM_STATE_ADDR, HWIO_SP_CNOC_SP_SCSR_COLD_BOOT_FSM_STATE_RMSK)
#define HWIO_SP_CNOC_SP_SCSR_COLD_BOOT_FSM_STATE_INM(m)      \
        in_dword_masked(HWIO_SP_CNOC_SP_SCSR_COLD_BOOT_FSM_STATE_ADDR, m)
#define HWIO_SP_CNOC_SP_SCSR_COLD_BOOT_FSM_STATE_CPU_BOOT_BMSK                                  0x40
#define HWIO_SP_CNOC_SP_SCSR_COLD_BOOT_FSM_STATE_CPU_BOOT_SHFT                                   0x6
#define HWIO_SP_CNOC_SP_SCSR_COLD_BOOT_FSM_STATE_FUSE_UNMASK_BMSK                               0x20
#define HWIO_SP_CNOC_SP_SCSR_COLD_BOOT_FSM_STATE_FUSE_UNMASK_SHFT                                0x5
#define HWIO_SP_CNOC_SP_SCSR_COLD_BOOT_FSM_STATE_RAM_WIPE_WAIT_BMSK                             0x10
#define HWIO_SP_CNOC_SP_SCSR_COLD_BOOT_FSM_STATE_RAM_WIPE_WAIT_SHFT                              0x4
#define HWIO_SP_CNOC_SP_SCSR_COLD_BOOT_FSM_STATE_RAM_WIPE_START_BMSK                             0x8
#define HWIO_SP_CNOC_SP_SCSR_COLD_BOOT_FSM_STATE_RAM_WIPE_START_SHFT                             0x3
#define HWIO_SP_CNOC_SP_SCSR_COLD_BOOT_FSM_STATE_GDSC_BRINGUP_BMSK                               0x4
#define HWIO_SP_CNOC_SP_SCSR_COLD_BOOT_FSM_STATE_GDSC_BRINGUP_SHFT                               0x2
#define HWIO_SP_CNOC_SP_SCSR_COLD_BOOT_FSM_STATE_FUSE_SENSE_BMSK                                 0x2
#define HWIO_SP_CNOC_SP_SCSR_COLD_BOOT_FSM_STATE_FUSE_SENSE_SHFT                                 0x1
#define HWIO_SP_CNOC_SP_SCSR_COLD_BOOT_FSM_STATE_IDLE_BMSK                                       0x1
#define HWIO_SP_CNOC_SP_SCSR_COLD_BOOT_FSM_STATE_IDLE_SHFT                                       0x0

#define HWIO_SP_CNOC_SP_SCSR_DCD_CGC_CTRL_ADDR                                            (SP_CNOC_SP_SCSR_REGS_REG_BASE      + 0x00002008)
#define HWIO_SP_CNOC_SP_SCSR_DCD_CGC_CTRL_OFFS                                            (SP_CNOC_SP_SCSR_REGS_REG_BASE_OFFS + 0x00002008)
#define HWIO_SP_CNOC_SP_SCSR_DCD_CGC_CTRL_RMSK                                                   0xf
#define HWIO_SP_CNOC_SP_SCSR_DCD_CGC_CTRL_IN          \
        in_dword_masked(HWIO_SP_CNOC_SP_SCSR_DCD_CGC_CTRL_ADDR, HWIO_SP_CNOC_SP_SCSR_DCD_CGC_CTRL_RMSK)
#define HWIO_SP_CNOC_SP_SCSR_DCD_CGC_CTRL_INM(m)      \
        in_dword_masked(HWIO_SP_CNOC_SP_SCSR_DCD_CGC_CTRL_ADDR, m)
#define HWIO_SP_CNOC_SP_SCSR_DCD_CGC_CTRL_OUT(v)      \
        out_dword(HWIO_SP_CNOC_SP_SCSR_DCD_CGC_CTRL_ADDR,v)
#define HWIO_SP_CNOC_SP_SCSR_DCD_CGC_CTRL_OUTM(m,v) \
        out_dword_masked_ns(HWIO_SP_CNOC_SP_SCSR_DCD_CGC_CTRL_ADDR,m,v,HWIO_SP_CNOC_SP_SCSR_DCD_CGC_CTRL_IN)
#define HWIO_SP_CNOC_SP_SCSR_DCD_CGC_CTRL_CNOC_DCD_VOTE_SW_DIS_BMSK                              0x8
#define HWIO_SP_CNOC_SP_SCSR_DCD_CGC_CTRL_CNOC_DCD_VOTE_SW_DIS_SHFT                              0x3
#define HWIO_SP_CNOC_SP_SCSR_DCD_CGC_CTRL_CNOC_CGC_SW_EN_BMSK                                    0x4
#define HWIO_SP_CNOC_SP_SCSR_DCD_CGC_CTRL_CNOC_CGC_SW_EN_SHFT                                    0x2
#define HWIO_SP_CNOC_SP_SCSR_DCD_CGC_CTRL_SCSR_CGC_SW_EN_BMSK                                    0x2
#define HWIO_SP_CNOC_SP_SCSR_DCD_CGC_CTRL_SCSR_CGC_SW_EN_SHFT                                    0x1
#define HWIO_SP_CNOC_SP_SCSR_DCD_CGC_CTRL_GPKT_CGC_SW_EN_BMSK                                    0x1
#define HWIO_SP_CNOC_SP_SCSR_DCD_CGC_CTRL_GPKT_CGC_SW_EN_SHFT                                    0x0

#define HWIO_SP_CNOC_SP_SCSR_RMB_ERR_STATUS_SPAREm_ADDR(m)                                (SP_CNOC_SP_SCSR_REGS_REG_BASE      + 0x0000200c + 0x4 * (m))
#define HWIO_SP_CNOC_SP_SCSR_RMB_ERR_STATUS_SPAREm_OFFS(m)                                (SP_CNOC_SP_SCSR_REGS_REG_BASE_OFFS + 0x0000200c + 0x4 * (m))
#define HWIO_SP_CNOC_SP_SCSR_RMB_ERR_STATUS_SPAREm_RMSK                                   0xffffffff
#define HWIO_SP_CNOC_SP_SCSR_RMB_ERR_STATUS_SPAREm_MAXm                                            2
#define HWIO_SP_CNOC_SP_SCSR_RMB_ERR_STATUS_SPAREm_INI(m)        \
        in_dword_masked(HWIO_SP_CNOC_SP_SCSR_RMB_ERR_STATUS_SPAREm_ADDR(m), HWIO_SP_CNOC_SP_SCSR_RMB_ERR_STATUS_SPAREm_RMSK)
#define HWIO_SP_CNOC_SP_SCSR_RMB_ERR_STATUS_SPAREm_INMI(m,mask)    \
        in_dword_masked(HWIO_SP_CNOC_SP_SCSR_RMB_ERR_STATUS_SPAREm_ADDR(m), mask)
#define HWIO_SP_CNOC_SP_SCSR_RMB_ERR_STATUS_SPAREm_OUTI(m,val)    \
        out_dword(HWIO_SP_CNOC_SP_SCSR_RMB_ERR_STATUS_SPAREm_ADDR(m),val)
#define HWIO_SP_CNOC_SP_SCSR_RMB_ERR_STATUS_SPAREm_OUTMI(m,mask,val) \
        out_dword_masked_ns(HWIO_SP_CNOC_SP_SCSR_RMB_ERR_STATUS_SPAREm_ADDR(m),mask,val,HWIO_SP_CNOC_SP_SCSR_RMB_ERR_STATUS_SPAREm_INI(m))
#define HWIO_SP_CNOC_SP_SCSR_RMB_ERR_STATUS_SPAREm_STATUS_BMSK                            0xffffffff
#define HWIO_SP_CNOC_SP_SCSR_RMB_ERR_STATUS_SPAREm_STATUS_SHFT                                   0x0

#define HWIO_SP_CNOC_SP_SCSR_RST_CTRL_ADDR                                                (SP_CNOC_SP_SCSR_REGS_REG_BASE      + 0x0000201c)
#define HWIO_SP_CNOC_SP_SCSR_RST_CTRL_OFFS                                                (SP_CNOC_SP_SCSR_REGS_REG_BASE_OFFS + 0x0000201c)
#define HWIO_SP_CNOC_SP_SCSR_RST_CTRL_RMSK                                                       0x1
#define HWIO_SP_CNOC_SP_SCSR_RST_CTRL_IN          \
        in_dword_masked(HWIO_SP_CNOC_SP_SCSR_RST_CTRL_ADDR, HWIO_SP_CNOC_SP_SCSR_RST_CTRL_RMSK)
#define HWIO_SP_CNOC_SP_SCSR_RST_CTRL_INM(m)      \
        in_dword_masked(HWIO_SP_CNOC_SP_SCSR_RST_CTRL_ADDR, m)
#define HWIO_SP_CNOC_SP_SCSR_RST_CTRL_OUT(v)      \
        out_dword(HWIO_SP_CNOC_SP_SCSR_RST_CTRL_ADDR,v)
#define HWIO_SP_CNOC_SP_SCSR_RST_CTRL_OUTM(m,v) \
        out_dword_masked_ns(HWIO_SP_CNOC_SP_SCSR_RST_CTRL_ADDR,m,v,HWIO_SP_CNOC_SP_SCSR_RST_CTRL_IN)
#define HWIO_SP_CNOC_SP_SCSR_RST_CTRL_FULL_SSR_EN_BMSK                                           0x1
#define HWIO_SP_CNOC_SP_SCSR_RST_CTRL_FULL_SSR_EN_SHFT                                           0x0

#define HWIO_SP_CNOC_SP_SCSR_RST_EVENT_STATUS_ADDR                                        (SP_CNOC_SP_SCSR_REGS_REG_BASE      + 0x00002020)
#define HWIO_SP_CNOC_SP_SCSR_RST_EVENT_STATUS_OFFS                                        (SP_CNOC_SP_SCSR_REGS_REG_BASE_OFFS + 0x00002020)
#define HWIO_SP_CNOC_SP_SCSR_RST_EVENT_STATUS_RMSK                                               0xf
#define HWIO_SP_CNOC_SP_SCSR_RST_EVENT_STATUS_IN          \
        in_dword_masked(HWIO_SP_CNOC_SP_SCSR_RST_EVENT_STATUS_ADDR, HWIO_SP_CNOC_SP_SCSR_RST_EVENT_STATUS_RMSK)
#define HWIO_SP_CNOC_SP_SCSR_RST_EVENT_STATUS_INM(m)      \
        in_dword_masked(HWIO_SP_CNOC_SP_SCSR_RST_EVENT_STATUS_ADDR, m)
#define HWIO_SP_CNOC_SP_SCSR_RST_EVENT_STATUS_SPSS_FUNC_RESET_BMSK                               0x8
#define HWIO_SP_CNOC_SP_SCSR_RST_EVENT_STATUS_SPSS_FUNC_RESET_SHFT                               0x3
#define HWIO_SP_CNOC_SP_SCSR_RST_EVENT_STATUS_WDOG_RESET_REQUEST_BMSK                            0x4
#define HWIO_SP_CNOC_SP_SCSR_RST_EVENT_STATUS_WDOG_RESET_REQUEST_SHFT                            0x2
#define HWIO_SP_CNOC_SP_SCSR_RST_EVENT_STATUS_COLD_RESET_REQUEST_BMSK                            0x2
#define HWIO_SP_CNOC_SP_SCSR_RST_EVENT_STATUS_COLD_RESET_REQUEST_SHFT                            0x1
#define HWIO_SP_CNOC_SP_SCSR_RST_EVENT_STATUS_GDSC_RESET_BMSK                                    0x1
#define HWIO_SP_CNOC_SP_SCSR_RST_EVENT_STATUS_GDSC_RESET_SHFT                                    0x0

#define HWIO_SP_CNOC_SP_SCSR_RST_EVENT_CLR_ADDR                                           (SP_CNOC_SP_SCSR_REGS_REG_BASE      + 0x00002024)
#define HWIO_SP_CNOC_SP_SCSR_RST_EVENT_CLR_OFFS                                           (SP_CNOC_SP_SCSR_REGS_REG_BASE_OFFS + 0x00002024)
#define HWIO_SP_CNOC_SP_SCSR_RST_EVENT_CLR_RMSK                                                  0xf
#define HWIO_SP_CNOC_SP_SCSR_RST_EVENT_CLR_OUT(v)      \
        out_dword(HWIO_SP_CNOC_SP_SCSR_RST_EVENT_CLR_ADDR,v)
#define HWIO_SP_CNOC_SP_SCSR_RST_EVENT_CLR_SPSS_FUNC_RESET_BMSK                                  0x8
#define HWIO_SP_CNOC_SP_SCSR_RST_EVENT_CLR_SPSS_FUNC_RESET_SHFT                                  0x3
#define HWIO_SP_CNOC_SP_SCSR_RST_EVENT_CLR_WDOG_RESET_REQUEST_BMSK                               0x4
#define HWIO_SP_CNOC_SP_SCSR_RST_EVENT_CLR_WDOG_RESET_REQUEST_SHFT                               0x2
#define HWIO_SP_CNOC_SP_SCSR_RST_EVENT_CLR_COLD_RESET_REQUEST_BMSK                               0x2
#define HWIO_SP_CNOC_SP_SCSR_RST_EVENT_CLR_COLD_RESET_REQUEST_SHFT                               0x1
#define HWIO_SP_CNOC_SP_SCSR_RST_EVENT_CLR_GDSC_RESET_BMSK                                       0x1
#define HWIO_SP_CNOC_SP_SCSR_RST_EVENT_CLR_GDSC_RESET_SHFT                                       0x0

#define HWIO_SP_CNOC_SP_SCSR_RST_STATE_ADDR                                               (SP_CNOC_SP_SCSR_REGS_REG_BASE      + 0x00002028)
#define HWIO_SP_CNOC_SP_SCSR_RST_STATE_OFFS                                               (SP_CNOC_SP_SCSR_REGS_REG_BASE_OFFS + 0x00002028)
#define HWIO_SP_CNOC_SP_SCSR_RST_STATE_RMSK                                                  0x301ff
#define HWIO_SP_CNOC_SP_SCSR_RST_STATE_IN          \
        in_dword_masked(HWIO_SP_CNOC_SP_SCSR_RST_STATE_ADDR, HWIO_SP_CNOC_SP_SCSR_RST_STATE_RMSK)
#define HWIO_SP_CNOC_SP_SCSR_RST_STATE_INM(m)      \
        in_dword_masked(HWIO_SP_CNOC_SP_SCSR_RST_STATE_ADDR, m)
#define HWIO_SP_CNOC_SP_SCSR_RST_STATE_SPSS_FIRST_PASS_RESET_BMSK                            0x20000
#define HWIO_SP_CNOC_SP_SCSR_RST_STATE_SPSS_FIRST_PASS_RESET_SHFT                               0x11
#define HWIO_SP_CNOC_SP_SCSR_RST_STATE_SPSS_INT_ALT_RESET_BMSK                               0x10000
#define HWIO_SP_CNOC_SP_SCSR_RST_STATE_SPSS_INT_ALT_RESET_SHFT                                  0x10
#define HWIO_SP_CNOC_SP_SCSR_RST_STATE_PREVENT_SSR_BMSK                                        0x100
#define HWIO_SP_CNOC_SP_SCSR_RST_STATE_PREVENT_SSR_SHFT                                          0x8
#define HWIO_SP_CNOC_SP_SCSR_RST_STATE_WDOG_FUNC_RESET_BMSK                                     0x80
#define HWIO_SP_CNOC_SP_SCSR_RST_STATE_WDOG_FUNC_RESET_SHFT                                      0x7
#define HWIO_SP_CNOC_SP_SCSR_RST_STATE_WDOG_ALT_RESET_BMSK                                      0x40
#define HWIO_SP_CNOC_SP_SCSR_RST_STATE_WDOG_ALT_RESET_SHFT                                       0x6
#define HWIO_SP_CNOC_SP_SCSR_RST_STATE_WDOG_CLK_DISABLE_BMSK                                    0x20
#define HWIO_SP_CNOC_SP_SCSR_RST_STATE_WDOG_CLK_DISABLE_SHFT                                     0x5
#define HWIO_SP_CNOC_SP_SCSR_RST_STATE_SPSS_INT_CLK_DISABLE_BMSK                                0x10
#define HWIO_SP_CNOC_SP_SCSR_RST_STATE_SPSS_INT_CLK_DISABLE_SHFT                                 0x4
#define HWIO_SP_CNOC_SP_SCSR_RST_STATE_SPSS_FUNC_RESET_BMSK                                      0x8
#define HWIO_SP_CNOC_SP_SCSR_RST_STATE_SPSS_FUNC_RESET_SHFT                                      0x3
#define HWIO_SP_CNOC_SP_SCSR_RST_STATE_WDOG_RESET_REQUEST_BMSK                                   0x4
#define HWIO_SP_CNOC_SP_SCSR_RST_STATE_WDOG_RESET_REQUEST_SHFT                                   0x2
#define HWIO_SP_CNOC_SP_SCSR_RST_STATE_COLD_RESET_REQUEST_BMSK                                   0x2
#define HWIO_SP_CNOC_SP_SCSR_RST_STATE_COLD_RESET_REQUEST_SHFT                                   0x1
#define HWIO_SP_CNOC_SP_SCSR_RST_STATE_GDSC_RESET_BMSK                                           0x1
#define HWIO_SP_CNOC_SP_SCSR_RST_STATE_GDSC_RESET_SHFT                                           0x0

#define HWIO_SP_CNOC_SP_SCSR_WDOG_FSM_STATE_ADDR                                          (SP_CNOC_SP_SCSR_REGS_REG_BASE      + 0x0000202c)
#define HWIO_SP_CNOC_SP_SCSR_WDOG_FSM_STATE_OFFS                                          (SP_CNOC_SP_SCSR_REGS_REG_BASE_OFFS + 0x0000202c)
#define HWIO_SP_CNOC_SP_SCSR_WDOG_FSM_STATE_RMSK                                                0xff
#define HWIO_SP_CNOC_SP_SCSR_WDOG_FSM_STATE_IN          \
        in_dword_masked(HWIO_SP_CNOC_SP_SCSR_WDOG_FSM_STATE_ADDR, HWIO_SP_CNOC_SP_SCSR_WDOG_FSM_STATE_RMSK)
#define HWIO_SP_CNOC_SP_SCSR_WDOG_FSM_STATE_INM(m)      \
        in_dword_masked(HWIO_SP_CNOC_SP_SCSR_WDOG_FSM_STATE_ADDR, m)
#define HWIO_SP_CNOC_SP_SCSR_WDOG_FSM_STATE_RSVD_BMSK                                           0x80
#define HWIO_SP_CNOC_SP_SCSR_WDOG_FSM_STATE_RSVD_SHFT                                            0x7
#define HWIO_SP_CNOC_SP_SCSR_WDOG_FSM_STATE_SECOND_PASS_RESET_BMSK                              0x40
#define HWIO_SP_CNOC_SP_SCSR_WDOG_FSM_STATE_SECOND_PASS_RESET_SHFT                               0x6
#define HWIO_SP_CNOC_SP_SCSR_WDOG_FSM_STATE_FIRST_PASS_DELAY_BMSK                               0x20
#define HWIO_SP_CNOC_SP_SCSR_WDOG_FSM_STATE_FIRST_PASS_DELAY_SHFT                                0x5
#define HWIO_SP_CNOC_SP_SCSR_WDOG_FSM_STATE_FIRST_PASS_RESET_EXTEND_BMSK                        0x10
#define HWIO_SP_CNOC_SP_SCSR_WDOG_FSM_STATE_FIRST_PASS_RESET_EXTEND_SHFT                         0x4
#define HWIO_SP_CNOC_SP_SCSR_WDOG_FSM_STATE_FIRST_PASS_RESET_BMSK                                0x8
#define HWIO_SP_CNOC_SP_SCSR_WDOG_FSM_STATE_FIRST_PASS_RESET_SHFT                                0x3
#define HWIO_SP_CNOC_SP_SCSR_WDOG_FSM_STATE_CONTEXT_DELAY_BMSK                                   0x4
#define HWIO_SP_CNOC_SP_SCSR_WDOG_FSM_STATE_CONTEXT_DELAY_SHFT                                   0x2
#define HWIO_SP_CNOC_SP_SCSR_WDOG_FSM_STATE_CONTEXT_IRQ_BMSK                                     0x2
#define HWIO_SP_CNOC_SP_SCSR_WDOG_FSM_STATE_CONTEXT_IRQ_SHFT                                     0x1
#define HWIO_SP_CNOC_SP_SCSR_WDOG_FSM_STATE_IDLE_BMSK                                            0x1
#define HWIO_SP_CNOC_SP_SCSR_WDOG_FSM_STATE_IDLE_SHFT                                            0x0

#define HWIO_SP_CNOC_SP_SCSR_PLL_STATUS_ADDR                                              (SP_CNOC_SP_SCSR_REGS_REG_BASE      + 0x00002030)
#define HWIO_SP_CNOC_SP_SCSR_PLL_STATUS_OFFS                                              (SP_CNOC_SP_SCSR_REGS_REG_BASE_OFFS + 0x00002030)
#define HWIO_SP_CNOC_SP_SCSR_PLL_STATUS_RMSK                                                 0xf000f
#define HWIO_SP_CNOC_SP_SCSR_PLL_STATUS_IN          \
        in_dword_masked(HWIO_SP_CNOC_SP_SCSR_PLL_STATUS_ADDR, HWIO_SP_CNOC_SP_SCSR_PLL_STATUS_RMSK)
#define HWIO_SP_CNOC_SP_SCSR_PLL_STATUS_INM(m)      \
        in_dword_masked(HWIO_SP_CNOC_SP_SCSR_PLL_STATUS_ADDR, m)
#define HWIO_SP_CNOC_SP_SCSR_PLL_STATUS_PLL1_STATE_BMSK                                      0xe0000
#define HWIO_SP_CNOC_SP_SCSR_PLL_STATUS_PLL1_STATE_SHFT                                         0x11
#define HWIO_SP_CNOC_SP_SCSR_PLL_STATUS_PLL1_LOCK_DET_BMSK                                   0x10000
#define HWIO_SP_CNOC_SP_SCSR_PLL_STATUS_PLL1_LOCK_DET_SHFT                                      0x10
#define HWIO_SP_CNOC_SP_SCSR_PLL_STATUS_PLL0_STATE_BMSK                                          0xe
#define HWIO_SP_CNOC_SP_SCSR_PLL_STATUS_PLL0_STATE_SHFT                                          0x1
#define HWIO_SP_CNOC_SP_SCSR_PLL_STATUS_PLL0_LOCK_DET_BMSK                                       0x1
#define HWIO_SP_CNOC_SP_SCSR_PLL_STATUS_PLL0_LOCK_DET_SHFT                                       0x0

#define HWIO_SP_CNOC_SP_SCSR_GDSC_STATE_ADDR                                              (SP_CNOC_SP_SCSR_REGS_REG_BASE      + 0x00002034)
#define HWIO_SP_CNOC_SP_SCSR_GDSC_STATE_OFFS                                              (SP_CNOC_SP_SCSR_REGS_REG_BASE_OFFS + 0x00002034)
#define HWIO_SP_CNOC_SP_SCSR_GDSC_STATE_RMSK                                                    0xff
#define HWIO_SP_CNOC_SP_SCSR_GDSC_STATE_IN          \
        in_dword_masked(HWIO_SP_CNOC_SP_SCSR_GDSC_STATE_ADDR, HWIO_SP_CNOC_SP_SCSR_GDSC_STATE_RMSK)
#define HWIO_SP_CNOC_SP_SCSR_GDSC_STATE_INM(m)      \
        in_dword_masked(HWIO_SP_CNOC_SP_SCSR_GDSC_STATE_ADDR, m)
#define HWIO_SP_CNOC_SP_SCSR_GDSC_STATE_STATE_BMSK                                              0xff
#define HWIO_SP_CNOC_SP_SCSR_GDSC_STATE_STATE_SHFT                                               0x0

#define HWIO_SP_CNOC_SP_SCSR_RSCC_CPU_SYNC_FSM_STATE_ADDR                                 (SP_CNOC_SP_SCSR_REGS_REG_BASE      + 0x00002038)
#define HWIO_SP_CNOC_SP_SCSR_RSCC_CPU_SYNC_FSM_STATE_OFFS                                 (SP_CNOC_SP_SCSR_REGS_REG_BASE_OFFS + 0x00002038)
#define HWIO_SP_CNOC_SP_SCSR_RSCC_CPU_SYNC_FSM_STATE_RMSK                                       0x7f
#define HWIO_SP_CNOC_SP_SCSR_RSCC_CPU_SYNC_FSM_STATE_IN          \
        in_dword_masked(HWIO_SP_CNOC_SP_SCSR_RSCC_CPU_SYNC_FSM_STATE_ADDR, HWIO_SP_CNOC_SP_SCSR_RSCC_CPU_SYNC_FSM_STATE_RMSK)
#define HWIO_SP_CNOC_SP_SCSR_RSCC_CPU_SYNC_FSM_STATE_INM(m)      \
        in_dword_masked(HWIO_SP_CNOC_SP_SCSR_RSCC_CPU_SYNC_FSM_STATE_ADDR, m)
#define HWIO_SP_CNOC_SP_SCSR_RSCC_CPU_SYNC_FSM_STATE_GEN_DBG_ACK_BMSK                           0x40
#define HWIO_SP_CNOC_SP_SCSR_RSCC_CPU_SYNC_FSM_STATE_GEN_DBG_ACK_SHFT                            0x6
#define HWIO_SP_CNOC_SP_SCSR_RSCC_CPU_SYNC_FSM_STATE_REM_SLP_HOLD_REQ_BMSK                      0x20
#define HWIO_SP_CNOC_SP_SCSR_RSCC_CPU_SYNC_FSM_STATE_REM_SLP_HOLD_REQ_SHFT                       0x5
#define HWIO_SP_CNOC_SP_SCSR_RSCC_CPU_SYNC_FSM_STATE_GEN_BRINGUP_REQ_BMSK                       0x10
#define HWIO_SP_CNOC_SP_SCSR_RSCC_CPU_SYNC_FSM_STATE_GEN_BRINGUP_REQ_SHFT                        0x4
#define HWIO_SP_CNOC_SP_SCSR_RSCC_CPU_SYNC_FSM_STATE_WAIT_FOR_WAKEUP_BMSK                        0x8
#define HWIO_SP_CNOC_SP_SCSR_RSCC_CPU_SYNC_FSM_STATE_WAIT_FOR_WAKEUP_SHFT                        0x3
#define HWIO_SP_CNOC_SP_SCSR_RSCC_CPU_SYNC_FSM_STATE_GEN_SHUTDOWN_REQ_BMSK                       0x4
#define HWIO_SP_CNOC_SP_SCSR_RSCC_CPU_SYNC_FSM_STATE_GEN_SHUTDOWN_REQ_SHFT                       0x2
#define HWIO_SP_CNOC_SP_SCSR_RSCC_CPU_SYNC_FSM_STATE_GEN_SLP_HOLD_REQ_BMSK                       0x2
#define HWIO_SP_CNOC_SP_SCSR_RSCC_CPU_SYNC_FSM_STATE_GEN_SLP_HOLD_REQ_SHFT                       0x1
#define HWIO_SP_CNOC_SP_SCSR_RSCC_CPU_SYNC_FSM_STATE_ACTIVE_BMSK                                 0x1
#define HWIO_SP_CNOC_SP_SCSR_RSCC_CPU_SYNC_FSM_STATE_ACTIVE_SHFT                                 0x0

#define HWIO_SP_CNOC_SP_SCSR_OTP_ERROR_STATUS_ADDR                                        (SP_CNOC_SP_SCSR_REGS_REG_BASE      + 0x0000203c)
#define HWIO_SP_CNOC_SP_SCSR_OTP_ERROR_STATUS_OFFS                                        (SP_CNOC_SP_SCSR_REGS_REG_BASE_OFFS + 0x0000203c)
#define HWIO_SP_CNOC_SP_SCSR_OTP_ERROR_STATUS_RMSK                                              0x1f
#define HWIO_SP_CNOC_SP_SCSR_OTP_ERROR_STATUS_IN          \
        in_dword_masked(HWIO_SP_CNOC_SP_SCSR_OTP_ERROR_STATUS_ADDR, HWIO_SP_CNOC_SP_SCSR_OTP_ERROR_STATUS_RMSK)
#define HWIO_SP_CNOC_SP_SCSR_OTP_ERROR_STATUS_INM(m)      \
        in_dword_masked(HWIO_SP_CNOC_SP_SCSR_OTP_ERROR_STATUS_ADDR, m)
#define HWIO_SP_CNOC_SP_SCSR_OTP_ERROR_STATUS_COLD_BOOT_FUSECTL_SENSE_ERROR_BMSK                0x10
#define HWIO_SP_CNOC_SP_SCSR_OTP_ERROR_STATUS_COLD_BOOT_FUSECTL_SENSE_ERROR_SHFT                 0x4
#define HWIO_SP_CNOC_SP_SCSR_OTP_ERROR_STATUS_QFPROM_SP_DISABLE_BMSK                             0x8
#define HWIO_SP_CNOC_SP_SCSR_OTP_ERROR_STATUS_QFPROM_SP_DISABLE_SHFT                             0x3
#define HWIO_SP_CNOC_SP_SCSR_OTP_ERROR_STATUS_QFPROM_FUSE_SENSE_FEC_ERROR_BMSK                   0x4
#define HWIO_SP_CNOC_SP_SCSR_OTP_ERROR_STATUS_QFPROM_FUSE_SENSE_FEC_ERROR_SHFT                   0x2
#define HWIO_SP_CNOC_SP_SCSR_OTP_ERROR_STATUS_QFPROM_FUSE_SENSE_ROM_ERROR_BMSK                   0x2
#define HWIO_SP_CNOC_SP_SCSR_OTP_ERROR_STATUS_QFPROM_FUSE_SENSE_ROM_ERROR_SHFT                   0x1
#define HWIO_SP_CNOC_SP_SCSR_OTP_ERROR_STATUS_QFPROM_KEK_ZEROIZED_BMSK                           0x1
#define HWIO_SP_CNOC_SP_SCSR_OTP_ERROR_STATUS_QFPROM_KEK_ZEROIZED_SHFT                           0x0

#define HWIO_SP_CNOC_SP_SCSR_CNOC2SP_INTF_DBG_CTRL_ADDR                                   (SP_CNOC_SP_SCSR_REGS_REG_BASE      + 0x00002040)
#define HWIO_SP_CNOC_SP_SCSR_CNOC2SP_INTF_DBG_CTRL_OFFS                                   (SP_CNOC_SP_SCSR_REGS_REG_BASE_OFFS + 0x00002040)
#define HWIO_SP_CNOC_SP_SCSR_CNOC2SP_INTF_DBG_CTRL_RMSK                                          0x1
#define HWIO_SP_CNOC_SP_SCSR_CNOC2SP_INTF_DBG_CTRL_IN          \
        in_dword_masked(HWIO_SP_CNOC_SP_SCSR_CNOC2SP_INTF_DBG_CTRL_ADDR, HWIO_SP_CNOC_SP_SCSR_CNOC2SP_INTF_DBG_CTRL_RMSK)
#define HWIO_SP_CNOC_SP_SCSR_CNOC2SP_INTF_DBG_CTRL_INM(m)      \
        in_dword_masked(HWIO_SP_CNOC_SP_SCSR_CNOC2SP_INTF_DBG_CTRL_ADDR, m)
#define HWIO_SP_CNOC_SP_SCSR_CNOC2SP_INTF_DBG_CTRL_OUT(v)      \
        out_dword(HWIO_SP_CNOC_SP_SCSR_CNOC2SP_INTF_DBG_CTRL_ADDR,v)
#define HWIO_SP_CNOC_SP_SCSR_CNOC2SP_INTF_DBG_CTRL_OUTM(m,v) \
        out_dword_masked_ns(HWIO_SP_CNOC_SP_SCSR_CNOC2SP_INTF_DBG_CTRL_ADDR,m,v,HWIO_SP_CNOC_SP_SCSR_CNOC2SP_INTF_DBG_CTRL_IN)
#define HWIO_SP_CNOC_SP_SCSR_CNOC2SP_INTF_DBG_CTRL_CX_PERIPH_CLK_FORCE_ON_BMSK                   0x1
#define HWIO_SP_CNOC_SP_SCSR_CNOC2SP_INTF_DBG_CTRL_CX_PERIPH_CLK_FORCE_ON_SHFT                   0x0

#define HWIO_SP_CNOC_SP_SCSR_CNOC2SP_INTF_DBG_STATUS_ADDR                                 (SP_CNOC_SP_SCSR_REGS_REG_BASE      + 0x00002044)
#define HWIO_SP_CNOC_SP_SCSR_CNOC2SP_INTF_DBG_STATUS_OFFS                                 (SP_CNOC_SP_SCSR_REGS_REG_BASE_OFFS + 0x00002044)
#define HWIO_SP_CNOC_SP_SCSR_CNOC2SP_INTF_DBG_STATUS_RMSK                                        0x3
#define HWIO_SP_CNOC_SP_SCSR_CNOC2SP_INTF_DBG_STATUS_IN          \
        in_dword_masked(HWIO_SP_CNOC_SP_SCSR_CNOC2SP_INTF_DBG_STATUS_ADDR, HWIO_SP_CNOC_SP_SCSR_CNOC2SP_INTF_DBG_STATUS_RMSK)
#define HWIO_SP_CNOC_SP_SCSR_CNOC2SP_INTF_DBG_STATUS_INM(m)      \
        in_dword_masked(HWIO_SP_CNOC_SP_SCSR_CNOC2SP_INTF_DBG_STATUS_ADDR, m)
#define HWIO_SP_CNOC_SP_SCSR_CNOC2SP_INTF_DBG_STATUS_CX_PERIPH_CLK_OFF_BMSK                      0x2
#define HWIO_SP_CNOC_SP_SCSR_CNOC2SP_INTF_DBG_STATUS_CX_PERIPH_CLK_OFF_SHFT                      0x1
#define HWIO_SP_CNOC_SP_SCSR_CNOC2SP_INTF_DBG_STATUS_CNOC2SP_INTF_DISABLE_BMSK                   0x1
#define HWIO_SP_CNOC_SP_SCSR_CNOC2SP_INTF_DBG_STATUS_CNOC2SP_INTF_DISABLE_SHFT                   0x0

#define HWIO_SP_CNOC_SP_SCSR_PRI_KEY1_SEL_ADDR                                            (SP_CNOC_SP_SCSR_REGS_REG_BASE      + 0x00003000)
#define HWIO_SP_CNOC_SP_SCSR_PRI_KEY1_SEL_OFFS                                            (SP_CNOC_SP_SCSR_REGS_REG_BASE_OFFS + 0x00003000)
#define HWIO_SP_CNOC_SP_SCSR_PRI_KEY1_SEL_RMSK                                                   0x1
#define HWIO_SP_CNOC_SP_SCSR_PRI_KEY1_SEL_IN          \
        in_dword_masked(HWIO_SP_CNOC_SP_SCSR_PRI_KEY1_SEL_ADDR, HWIO_SP_CNOC_SP_SCSR_PRI_KEY1_SEL_RMSK)
#define HWIO_SP_CNOC_SP_SCSR_PRI_KEY1_SEL_INM(m)      \
        in_dword_masked(HWIO_SP_CNOC_SP_SCSR_PRI_KEY1_SEL_ADDR, m)
#define HWIO_SP_CNOC_SP_SCSR_PRI_KEY1_SEL_OUT(v)      \
        out_dword(HWIO_SP_CNOC_SP_SCSR_PRI_KEY1_SEL_ADDR,v)
#define HWIO_SP_CNOC_SP_SCSR_PRI_KEY1_SEL_OUTM(m,v) \
        out_dword_masked_ns(HWIO_SP_CNOC_SP_SCSR_PRI_KEY1_SEL_ADDR,m,v,HWIO_SP_CNOC_SP_SCSR_PRI_KEY1_SEL_IN)
#define HWIO_SP_CNOC_SP_SCSR_PRI_KEY1_SEL_SEL_BMSK                                               0x1
#define HWIO_SP_CNOC_SP_SCSR_PRI_KEY1_SEL_SEL_SHFT                                               0x0

#define HWIO_SP_CNOC_SP_SCSR_SCSA_KEY_SEL_ADDR                                            (SP_CNOC_SP_SCSR_REGS_REG_BASE      + 0x00004000)
#define HWIO_SP_CNOC_SP_SCSR_SCSA_KEY_SEL_OFFS                                            (SP_CNOC_SP_SCSR_REGS_REG_BASE_OFFS + 0x00004000)
#define HWIO_SP_CNOC_SP_SCSR_SCSA_KEY_SEL_RMSK                                                   0x3
#define HWIO_SP_CNOC_SP_SCSR_SCSA_KEY_SEL_IN          \
        in_dword_masked(HWIO_SP_CNOC_SP_SCSR_SCSA_KEY_SEL_ADDR, HWIO_SP_CNOC_SP_SCSR_SCSA_KEY_SEL_RMSK)
#define HWIO_SP_CNOC_SP_SCSR_SCSA_KEY_SEL_INM(m)      \
        in_dword_masked(HWIO_SP_CNOC_SP_SCSR_SCSA_KEY_SEL_ADDR, m)
#define HWIO_SP_CNOC_SP_SCSR_SCSA_KEY_SEL_OUT(v)      \
        out_dword(HWIO_SP_CNOC_SP_SCSR_SCSA_KEY_SEL_ADDR,v)
#define HWIO_SP_CNOC_SP_SCSR_SCSA_KEY_SEL_OUTM(m,v) \
        out_dword_masked_ns(HWIO_SP_CNOC_SP_SCSR_SCSA_KEY_SEL_ADDR,m,v,HWIO_SP_CNOC_SP_SCSR_SCSA_KEY_SEL_IN)
#define HWIO_SP_CNOC_SP_SCSR_SCSA_KEY_SEL_SEL_BMSK                                               0x3
#define HWIO_SP_CNOC_SP_SCSR_SCSA_KEY_SEL_SEL_SHFT                                               0x0

#define HWIO_SP_CNOC_SP_SCSR_MBn_TZ_CTRL_ADDR(n)                                          (SP_CNOC_SP_SCSR_REGS_REG_BASE      + 0x00005000 + 0x2000 * (n))
#define HWIO_SP_CNOC_SP_SCSR_MBn_TZ_CTRL_OFFS(n)                                          (SP_CNOC_SP_SCSR_REGS_REG_BASE_OFFS + 0x00005000 + 0x2000 * (n))
#define HWIO_SP_CNOC_SP_SCSR_MBn_TZ_CTRL_RMSK                                                    0x1
#define HWIO_SP_CNOC_SP_SCSR_MBn_TZ_CTRL_MAXn                                                      3
#define HWIO_SP_CNOC_SP_SCSR_MBn_TZ_CTRL_INI(n)        \
        in_dword_masked(HWIO_SP_CNOC_SP_SCSR_MBn_TZ_CTRL_ADDR(n), HWIO_SP_CNOC_SP_SCSR_MBn_TZ_CTRL_RMSK)
#define HWIO_SP_CNOC_SP_SCSR_MBn_TZ_CTRL_INMI(n,mask)    \
        in_dword_masked(HWIO_SP_CNOC_SP_SCSR_MBn_TZ_CTRL_ADDR(n), mask)
#define HWIO_SP_CNOC_SP_SCSR_MBn_TZ_CTRL_OUTI(n,val)    \
        out_dword(HWIO_SP_CNOC_SP_SCSR_MBn_TZ_CTRL_ADDR(n),val)
#define HWIO_SP_CNOC_SP_SCSR_MBn_TZ_CTRL_OUTMI(n,mask,val) \
        out_dword_masked_ns(HWIO_SP_CNOC_SP_SCSR_MBn_TZ_CTRL_ADDR(n),mask,val,HWIO_SP_CNOC_SP_SCSR_MBn_TZ_CTRL_INI(n))
#define HWIO_SP_CNOC_SP_SCSR_MBn_TZ_CTRL_LOCK_MB_BMSK                                            0x1
#define HWIO_SP_CNOC_SP_SCSR_MBn_TZ_CTRL_LOCK_MB_SHFT                                            0x0

#define HWIO_SP_CNOC_SP_SCSR_MBn_MSA_CTRL_ADDR(n)                                         (SP_CNOC_SP_SCSR_REGS_REG_BASE      + 0x00005004 + 0x2000 * (n))
#define HWIO_SP_CNOC_SP_SCSR_MBn_MSA_CTRL_OFFS(n)                                         (SP_CNOC_SP_SCSR_REGS_REG_BASE_OFFS + 0x00005004 + 0x2000 * (n))
#define HWIO_SP_CNOC_SP_SCSR_MBn_MSA_CTRL_RMSK                                                   0x1
#define HWIO_SP_CNOC_SP_SCSR_MBn_MSA_CTRL_MAXn                                                     3
#define HWIO_SP_CNOC_SP_SCSR_MBn_MSA_CTRL_INI(n)        \
        in_dword_masked(HWIO_SP_CNOC_SP_SCSR_MBn_MSA_CTRL_ADDR(n), HWIO_SP_CNOC_SP_SCSR_MBn_MSA_CTRL_RMSK)
#define HWIO_SP_CNOC_SP_SCSR_MBn_MSA_CTRL_INMI(n,mask)    \
        in_dword_masked(HWIO_SP_CNOC_SP_SCSR_MBn_MSA_CTRL_ADDR(n), mask)
#define HWIO_SP_CNOC_SP_SCSR_MBn_MSA_CTRL_OUTI(n,val)    \
        out_dword(HWIO_SP_CNOC_SP_SCSR_MBn_MSA_CTRL_ADDR(n),val)
#define HWIO_SP_CNOC_SP_SCSR_MBn_MSA_CTRL_OUTMI(n,mask,val) \
        out_dword_masked_ns(HWIO_SP_CNOC_SP_SCSR_MBn_MSA_CTRL_ADDR(n),mask,val,HWIO_SP_CNOC_SP_SCSR_MBn_MSA_CTRL_INI(n))
#define HWIO_SP_CNOC_SP_SCSR_MBn_MSA_CTRL_LOCK_MB_BMSK                                           0x1
#define HWIO_SP_CNOC_SP_SCSR_MBn_MSA_CTRL_LOCK_MB_SHFT                                           0x0

#define HWIO_SP_CNOC_SP_SCSR_MBn_SMEM_ADDR_L_ADDR(n)                                      (SP_CNOC_SP_SCSR_REGS_REG_BASE      + 0x00005008 + 0x2000 * (n))
#define HWIO_SP_CNOC_SP_SCSR_MBn_SMEM_ADDR_L_OFFS(n)                                      (SP_CNOC_SP_SCSR_REGS_REG_BASE_OFFS + 0x00005008 + 0x2000 * (n))
#define HWIO_SP_CNOC_SP_SCSR_MBn_SMEM_ADDR_L_RMSK                                         0xffffffff
#define HWIO_SP_CNOC_SP_SCSR_MBn_SMEM_ADDR_L_MAXn                                                  3
#define HWIO_SP_CNOC_SP_SCSR_MBn_SMEM_ADDR_L_INI(n)        \
        in_dword_masked(HWIO_SP_CNOC_SP_SCSR_MBn_SMEM_ADDR_L_ADDR(n), HWIO_SP_CNOC_SP_SCSR_MBn_SMEM_ADDR_L_RMSK)
#define HWIO_SP_CNOC_SP_SCSR_MBn_SMEM_ADDR_L_INMI(n,mask)    \
        in_dword_masked(HWIO_SP_CNOC_SP_SCSR_MBn_SMEM_ADDR_L_ADDR(n), mask)
#define HWIO_SP_CNOC_SP_SCSR_MBn_SMEM_ADDR_L_OUTI(n,val)    \
        out_dword(HWIO_SP_CNOC_SP_SCSR_MBn_SMEM_ADDR_L_ADDR(n),val)
#define HWIO_SP_CNOC_SP_SCSR_MBn_SMEM_ADDR_L_OUTMI(n,mask,val) \
        out_dword_masked_ns(HWIO_SP_CNOC_SP_SCSR_MBn_SMEM_ADDR_L_ADDR(n),mask,val,HWIO_SP_CNOC_SP_SCSR_MBn_SMEM_ADDR_L_INI(n))
#define HWIO_SP_CNOC_SP_SCSR_MBn_SMEM_ADDR_L_ADDR_BMSK                                    0xffffffff
#define HWIO_SP_CNOC_SP_SCSR_MBn_SMEM_ADDR_L_ADDR_SHFT                                           0x0

#define HWIO_SP_CNOC_SP_SCSR_MBn_SMEM_ADDR_U_ADDR(n)                                      (SP_CNOC_SP_SCSR_REGS_REG_BASE      + 0x0000500c + 0x2000 * (n))
#define HWIO_SP_CNOC_SP_SCSR_MBn_SMEM_ADDR_U_OFFS(n)                                      (SP_CNOC_SP_SCSR_REGS_REG_BASE_OFFS + 0x0000500c + 0x2000 * (n))
#define HWIO_SP_CNOC_SP_SCSR_MBn_SMEM_ADDR_U_RMSK                                             0xffff
#define HWIO_SP_CNOC_SP_SCSR_MBn_SMEM_ADDR_U_MAXn                                                  3
#define HWIO_SP_CNOC_SP_SCSR_MBn_SMEM_ADDR_U_INI(n)        \
        in_dword_masked(HWIO_SP_CNOC_SP_SCSR_MBn_SMEM_ADDR_U_ADDR(n), HWIO_SP_CNOC_SP_SCSR_MBn_SMEM_ADDR_U_RMSK)
#define HWIO_SP_CNOC_SP_SCSR_MBn_SMEM_ADDR_U_INMI(n,mask)    \
        in_dword_masked(HWIO_SP_CNOC_SP_SCSR_MBn_SMEM_ADDR_U_ADDR(n), mask)
#define HWIO_SP_CNOC_SP_SCSR_MBn_SMEM_ADDR_U_OUTI(n,val)    \
        out_dword(HWIO_SP_CNOC_SP_SCSR_MBn_SMEM_ADDR_U_ADDR(n),val)
#define HWIO_SP_CNOC_SP_SCSR_MBn_SMEM_ADDR_U_OUTMI(n,mask,val) \
        out_dword_masked_ns(HWIO_SP_CNOC_SP_SCSR_MBn_SMEM_ADDR_U_ADDR(n),mask,val,HWIO_SP_CNOC_SP_SCSR_MBn_SMEM_ADDR_U_INI(n))
#define HWIO_SP_CNOC_SP_SCSR_MBn_SMEM_ADDR_U_ADDR_BMSK                                        0xffff
#define HWIO_SP_CNOC_SP_SCSR_MBn_SMEM_ADDR_U_ADDR_SHFT                                           0x0

#define HWIO_SP_CNOC_SP_SCSR_MBn_SMEM_SIZE_ADDR(n)                                        (SP_CNOC_SP_SCSR_REGS_REG_BASE      + 0x00005010 + 0x2000 * (n))
#define HWIO_SP_CNOC_SP_SCSR_MBn_SMEM_SIZE_OFFS(n)                                        (SP_CNOC_SP_SCSR_REGS_REG_BASE_OFFS + 0x00005010 + 0x2000 * (n))
#define HWIO_SP_CNOC_SP_SCSR_MBn_SMEM_SIZE_RMSK                                              0xfffff
#define HWIO_SP_CNOC_SP_SCSR_MBn_SMEM_SIZE_MAXn                                                    3
#define HWIO_SP_CNOC_SP_SCSR_MBn_SMEM_SIZE_INI(n)        \
        in_dword_masked(HWIO_SP_CNOC_SP_SCSR_MBn_SMEM_SIZE_ADDR(n), HWIO_SP_CNOC_SP_SCSR_MBn_SMEM_SIZE_RMSK)
#define HWIO_SP_CNOC_SP_SCSR_MBn_SMEM_SIZE_INMI(n,mask)    \
        in_dword_masked(HWIO_SP_CNOC_SP_SCSR_MBn_SMEM_SIZE_ADDR(n), mask)
#define HWIO_SP_CNOC_SP_SCSR_MBn_SMEM_SIZE_OUTI(n,val)    \
        out_dword(HWIO_SP_CNOC_SP_SCSR_MBn_SMEM_SIZE_ADDR(n),val)
#define HWIO_SP_CNOC_SP_SCSR_MBn_SMEM_SIZE_OUTMI(n,mask,val) \
        out_dword_masked_ns(HWIO_SP_CNOC_SP_SCSR_MBn_SMEM_SIZE_ADDR(n),mask,val,HWIO_SP_CNOC_SP_SCSR_MBn_SMEM_SIZE_INI(n))
#define HWIO_SP_CNOC_SP_SCSR_MBn_SMEM_SIZE_SIZE_BMSK                                         0xfffff
#define HWIO_SP_CNOC_SP_SCSR_MBn_SMEM_SIZE_SIZE_SHFT                                             0x0

#define HWIO_SP_CNOC_SP_SCSR_MB0_SMEM_ATTR_ADDR                                           (SP_CNOC_SP_SCSR_REGS_REG_BASE      + 0x00005014)
#define HWIO_SP_CNOC_SP_SCSR_MB0_SMEM_ATTR_OFFS                                           (SP_CNOC_SP_SCSR_REGS_REG_BASE_OFFS + 0x00005014)
#define HWIO_SP_CNOC_SP_SCSR_MB0_SMEM_ATTR_RMSK                                                  0x3
#define HWIO_SP_CNOC_SP_SCSR_MB0_SMEM_ATTR_IN          \
        in_dword_masked(HWIO_SP_CNOC_SP_SCSR_MB0_SMEM_ATTR_ADDR, HWIO_SP_CNOC_SP_SCSR_MB0_SMEM_ATTR_RMSK)
#define HWIO_SP_CNOC_SP_SCSR_MB0_SMEM_ATTR_INM(m)      \
        in_dword_masked(HWIO_SP_CNOC_SP_SCSR_MB0_SMEM_ATTR_ADDR, m)
#define HWIO_SP_CNOC_SP_SCSR_MB0_SMEM_ATTR_CLIENT_ID_BMSK                                        0x3
#define HWIO_SP_CNOC_SP_SCSR_MB0_SMEM_ATTR_CLIENT_ID_SHFT                                        0x0

#define HWIO_SP_CNOC_SP_SCSR_MB1_SMEM_ATTR_ADDR                                           (SP_CNOC_SP_SCSR_REGS_REG_BASE      + 0x00007014)
#define HWIO_SP_CNOC_SP_SCSR_MB1_SMEM_ATTR_OFFS                                           (SP_CNOC_SP_SCSR_REGS_REG_BASE_OFFS + 0x00007014)
#define HWIO_SP_CNOC_SP_SCSR_MB1_SMEM_ATTR_RMSK                                                  0x3
#define HWIO_SP_CNOC_SP_SCSR_MB1_SMEM_ATTR_IN          \
        in_dword_masked(HWIO_SP_CNOC_SP_SCSR_MB1_SMEM_ATTR_ADDR, HWIO_SP_CNOC_SP_SCSR_MB1_SMEM_ATTR_RMSK)
#define HWIO_SP_CNOC_SP_SCSR_MB1_SMEM_ATTR_INM(m)      \
        in_dword_masked(HWIO_SP_CNOC_SP_SCSR_MB1_SMEM_ATTR_ADDR, m)
#define HWIO_SP_CNOC_SP_SCSR_MB1_SMEM_ATTR_CLIENT_ID_BMSK                                        0x3
#define HWIO_SP_CNOC_SP_SCSR_MB1_SMEM_ATTR_CLIENT_ID_SHFT                                        0x0

#define HWIO_SP_CNOC_SP_SCSR_MB2_SMEM_ATTR_ADDR                                           (SP_CNOC_SP_SCSR_REGS_REG_BASE      + 0x00009014)
#define HWIO_SP_CNOC_SP_SCSR_MB2_SMEM_ATTR_OFFS                                           (SP_CNOC_SP_SCSR_REGS_REG_BASE_OFFS + 0x00009014)
#define HWIO_SP_CNOC_SP_SCSR_MB2_SMEM_ATTR_RMSK                                                  0x3
#define HWIO_SP_CNOC_SP_SCSR_MB2_SMEM_ATTR_IN          \
        in_dword_masked(HWIO_SP_CNOC_SP_SCSR_MB2_SMEM_ATTR_ADDR, HWIO_SP_CNOC_SP_SCSR_MB2_SMEM_ATTR_RMSK)
#define HWIO_SP_CNOC_SP_SCSR_MB2_SMEM_ATTR_INM(m)      \
        in_dword_masked(HWIO_SP_CNOC_SP_SCSR_MB2_SMEM_ATTR_ADDR, m)
#define HWIO_SP_CNOC_SP_SCSR_MB2_SMEM_ATTR_CLIENT_ID_BMSK                                        0x3
#define HWIO_SP_CNOC_SP_SCSR_MB2_SMEM_ATTR_CLIENT_ID_SHFT                                        0x0

#define HWIO_SP_CNOC_SP_SCSR_MB3_SMEM_ATTR_ADDR                                           (SP_CNOC_SP_SCSR_REGS_REG_BASE      + 0x0000b014)
#define HWIO_SP_CNOC_SP_SCSR_MB3_SMEM_ATTR_OFFS                                           (SP_CNOC_SP_SCSR_REGS_REG_BASE_OFFS + 0x0000b014)
#define HWIO_SP_CNOC_SP_SCSR_MB3_SMEM_ATTR_RMSK                                                  0x3
#define HWIO_SP_CNOC_SP_SCSR_MB3_SMEM_ATTR_IN          \
        in_dword_masked(HWIO_SP_CNOC_SP_SCSR_MB3_SMEM_ATTR_ADDR, HWIO_SP_CNOC_SP_SCSR_MB3_SMEM_ATTR_RMSK)
#define HWIO_SP_CNOC_SP_SCSR_MB3_SMEM_ATTR_INM(m)      \
        in_dword_masked(HWIO_SP_CNOC_SP_SCSR_MB3_SMEM_ATTR_ADDR, m)
#define HWIO_SP_CNOC_SP_SCSR_MB3_SMEM_ATTR_CLIENT_ID_BMSK                                        0x3
#define HWIO_SP_CNOC_SP_SCSR_MB3_SMEM_ATTR_CLIENT_ID_SHFT                                        0x0

#define HWIO_SP_CNOC_SP_SCSR_MBn_CL2SP_IRQ_STATUS_ADDR(n)                                 (SP_CNOC_SP_SCSR_REGS_REG_BASE      + 0x00005018 + 0x2000 * (n))
#define HWIO_SP_CNOC_SP_SCSR_MBn_CL2SP_IRQ_STATUS_OFFS(n)                                 (SP_CNOC_SP_SCSR_REGS_REG_BASE_OFFS + 0x00005018 + 0x2000 * (n))
#define HWIO_SP_CNOC_SP_SCSR_MBn_CL2SP_IRQ_STATUS_RMSK                                          0x1f
#define HWIO_SP_CNOC_SP_SCSR_MBn_CL2SP_IRQ_STATUS_MAXn                                             3
#define HWIO_SP_CNOC_SP_SCSR_MBn_CL2SP_IRQ_STATUS_INI(n)        \
        in_dword_masked(HWIO_SP_CNOC_SP_SCSR_MBn_CL2SP_IRQ_STATUS_ADDR(n), HWIO_SP_CNOC_SP_SCSR_MBn_CL2SP_IRQ_STATUS_RMSK)
#define HWIO_SP_CNOC_SP_SCSR_MBn_CL2SP_IRQ_STATUS_INMI(n,mask)    \
        in_dword_masked(HWIO_SP_CNOC_SP_SCSR_MBn_CL2SP_IRQ_STATUS_ADDR(n), mask)
#define HWIO_SP_CNOC_SP_SCSR_MBn_CL2SP_IRQ_STATUS_IRQ_BMSK                                      0x1f
#define HWIO_SP_CNOC_SP_SCSR_MBn_CL2SP_IRQ_STATUS_IRQ_SHFT                                       0x0

#define HWIO_SP_CNOC_SP_SCSR_MBn_CL2SP_IRQ_SET_ADDR(n)                                    (SP_CNOC_SP_SCSR_REGS_REG_BASE      + 0x0000501c + 0x2000 * (n))
#define HWIO_SP_CNOC_SP_SCSR_MBn_CL2SP_IRQ_SET_OFFS(n)                                    (SP_CNOC_SP_SCSR_REGS_REG_BASE_OFFS + 0x0000501c + 0x2000 * (n))
#define HWIO_SP_CNOC_SP_SCSR_MBn_CL2SP_IRQ_SET_RMSK                                             0x1f
#define HWIO_SP_CNOC_SP_SCSR_MBn_CL2SP_IRQ_SET_MAXn                                                3
#define HWIO_SP_CNOC_SP_SCSR_MBn_CL2SP_IRQ_SET_OUTI(n,val)    \
        out_dword(HWIO_SP_CNOC_SP_SCSR_MBn_CL2SP_IRQ_SET_ADDR(n),val)
#define HWIO_SP_CNOC_SP_SCSR_MBn_CL2SP_IRQ_SET_IRQ_BMSK                                         0x1f
#define HWIO_SP_CNOC_SP_SCSR_MBn_CL2SP_IRQ_SET_IRQ_SHFT                                          0x0

#define HWIO_SP_CNOC_SP_SCSR_MBn_CL2SP_IRQ_CLR_ADDR(n)                                    (SP_CNOC_SP_SCSR_REGS_REG_BASE      + 0x00005020 + 0x2000 * (n))
#define HWIO_SP_CNOC_SP_SCSR_MBn_CL2SP_IRQ_CLR_OFFS(n)                                    (SP_CNOC_SP_SCSR_REGS_REG_BASE_OFFS + 0x00005020 + 0x2000 * (n))
#define HWIO_SP_CNOC_SP_SCSR_MBn_CL2SP_IRQ_CLR_RMSK                                             0x1f
#define HWIO_SP_CNOC_SP_SCSR_MBn_CL2SP_IRQ_CLR_MAXn                                                3
#define HWIO_SP_CNOC_SP_SCSR_MBn_CL2SP_IRQ_CLR_OUTI(n,val)    \
        out_dword(HWIO_SP_CNOC_SP_SCSR_MBn_CL2SP_IRQ_CLR_ADDR(n),val)
#define HWIO_SP_CNOC_SP_SCSR_MBn_CL2SP_IRQ_CLR_IRQ_BMSK                                         0x1f
#define HWIO_SP_CNOC_SP_SCSR_MBn_CL2SP_IRQ_CLR_IRQ_SHFT                                          0x0

#define HWIO_SP_CNOC_SP_SCSR_MBn_CL2SP_IRQ_MASK_ADDR(n)                                   (SP_CNOC_SP_SCSR_REGS_REG_BASE      + 0x00005024 + 0x2000 * (n))
#define HWIO_SP_CNOC_SP_SCSR_MBn_CL2SP_IRQ_MASK_OFFS(n)                                   (SP_CNOC_SP_SCSR_REGS_REG_BASE_OFFS + 0x00005024 + 0x2000 * (n))
#define HWIO_SP_CNOC_SP_SCSR_MBn_CL2SP_IRQ_MASK_RMSK                                            0x1f
#define HWIO_SP_CNOC_SP_SCSR_MBn_CL2SP_IRQ_MASK_MAXn                                               3
#define HWIO_SP_CNOC_SP_SCSR_MBn_CL2SP_IRQ_MASK_INI(n)        \
        in_dword_masked(HWIO_SP_CNOC_SP_SCSR_MBn_CL2SP_IRQ_MASK_ADDR(n), HWIO_SP_CNOC_SP_SCSR_MBn_CL2SP_IRQ_MASK_RMSK)
#define HWIO_SP_CNOC_SP_SCSR_MBn_CL2SP_IRQ_MASK_INMI(n,mask)    \
        in_dword_masked(HWIO_SP_CNOC_SP_SCSR_MBn_CL2SP_IRQ_MASK_ADDR(n), mask)
#define HWIO_SP_CNOC_SP_SCSR_MBn_CL2SP_IRQ_MASK_OUTI(n,val)    \
        out_dword(HWIO_SP_CNOC_SP_SCSR_MBn_CL2SP_IRQ_MASK_ADDR(n),val)
#define HWIO_SP_CNOC_SP_SCSR_MBn_CL2SP_IRQ_MASK_OUTMI(n,mask,val) \
        out_dword_masked_ns(HWIO_SP_CNOC_SP_SCSR_MBn_CL2SP_IRQ_MASK_ADDR(n),mask,val,HWIO_SP_CNOC_SP_SCSR_MBn_CL2SP_IRQ_MASK_INI(n))
#define HWIO_SP_CNOC_SP_SCSR_MBn_CL2SP_IRQ_MASK_IRQ_BMSK                                        0x1f
#define HWIO_SP_CNOC_SP_SCSR_MBn_CL2SP_IRQ_MASK_IRQ_SHFT                                         0x0

#define HWIO_SP_CNOC_SP_SCSR_MBn_CL2SP_GPm_ADDR(n,m)                                      (SP_CNOC_SP_SCSR_REGS_REG_BASE      + 0x00005028 + 0x4 * (m) + 0x2000 * (n))
#define HWIO_SP_CNOC_SP_SCSR_MBn_CL2SP_GPm_OFFS(n,m)                                      (SP_CNOC_SP_SCSR_REGS_REG_BASE_OFFS + 0x00005028 + 0x4 * (m) + 0x2000 * (n))
#define HWIO_SP_CNOC_SP_SCSR_MBn_CL2SP_GPm_RMSK                                               0xffff
#define HWIO_SP_CNOC_SP_SCSR_MBn_CL2SP_GPm_MAXn                                                    3
#define HWIO_SP_CNOC_SP_SCSR_MBn_CL2SP_GPm_MAXm                                                    1
#define HWIO_SP_CNOC_SP_SCSR_MBn_CL2SP_GPm_INI2(n,m)        \
        in_dword_masked(HWIO_SP_CNOC_SP_SCSR_MBn_CL2SP_GPm_ADDR(n,m), HWIO_SP_CNOC_SP_SCSR_MBn_CL2SP_GPm_RMSK)
#define HWIO_SP_CNOC_SP_SCSR_MBn_CL2SP_GPm_INMI2(n,m,mask)    \
        in_dword_masked(HWIO_SP_CNOC_SP_SCSR_MBn_CL2SP_GPm_ADDR(n,m), mask)
#define HWIO_SP_CNOC_SP_SCSR_MBn_CL2SP_GPm_OUTI2(n,m,val)    \
        out_dword(HWIO_SP_CNOC_SP_SCSR_MBn_CL2SP_GPm_ADDR(n,m),val)
#define HWIO_SP_CNOC_SP_SCSR_MBn_CL2SP_GPm_OUTMI2(n,m,mask,val) \
        out_dword_masked_ns(HWIO_SP_CNOC_SP_SCSR_MBn_CL2SP_GPm_ADDR(n,m),mask,val,HWIO_SP_CNOC_SP_SCSR_MBn_CL2SP_GPm_INI2(n,m))
#define HWIO_SP_CNOC_SP_SCSR_MBn_CL2SP_GPm_GP_BMSK                                            0xffff
#define HWIO_SP_CNOC_SP_SCSR_MBn_CL2SP_GPm_GP_SHFT                                               0x0

#define HWIO_SP_CNOC_SP_SCSR_MBn_SP2CL_IRQ_STATUS_ADDR(n)                                 (SP_CNOC_SP_SCSR_REGS_REG_BASE      + 0x00006000 + 0x2000 * (n))
#define HWIO_SP_CNOC_SP_SCSR_MBn_SP2CL_IRQ_STATUS_OFFS(n)                                 (SP_CNOC_SP_SCSR_REGS_REG_BASE_OFFS + 0x00006000 + 0x2000 * (n))
#define HWIO_SP_CNOC_SP_SCSR_MBn_SP2CL_IRQ_STATUS_RMSK                                          0x1f
#define HWIO_SP_CNOC_SP_SCSR_MBn_SP2CL_IRQ_STATUS_MAXn                                             3
#define HWIO_SP_CNOC_SP_SCSR_MBn_SP2CL_IRQ_STATUS_INI(n)        \
        in_dword_masked(HWIO_SP_CNOC_SP_SCSR_MBn_SP2CL_IRQ_STATUS_ADDR(n), HWIO_SP_CNOC_SP_SCSR_MBn_SP2CL_IRQ_STATUS_RMSK)
#define HWIO_SP_CNOC_SP_SCSR_MBn_SP2CL_IRQ_STATUS_INMI(n,mask)    \
        in_dword_masked(HWIO_SP_CNOC_SP_SCSR_MBn_SP2CL_IRQ_STATUS_ADDR(n), mask)
#define HWIO_SP_CNOC_SP_SCSR_MBn_SP2CL_IRQ_STATUS_IRQ_BMSK                                      0x1f
#define HWIO_SP_CNOC_SP_SCSR_MBn_SP2CL_IRQ_STATUS_IRQ_SHFT                                       0x0

#define HWIO_SP_CNOC_SP_SCSR_MBn_SP2CL_IRQ_SET_ADDR(n)                                    (SP_CNOC_SP_SCSR_REGS_REG_BASE      + 0x00006004 + 0x2000 * (n))
#define HWIO_SP_CNOC_SP_SCSR_MBn_SP2CL_IRQ_SET_OFFS(n)                                    (SP_CNOC_SP_SCSR_REGS_REG_BASE_OFFS + 0x00006004 + 0x2000 * (n))
#define HWIO_SP_CNOC_SP_SCSR_MBn_SP2CL_IRQ_SET_RMSK                                             0x1f
#define HWIO_SP_CNOC_SP_SCSR_MBn_SP2CL_IRQ_SET_MAXn                                                3
#define HWIO_SP_CNOC_SP_SCSR_MBn_SP2CL_IRQ_SET_OUTI(n,val)    \
        out_dword(HWIO_SP_CNOC_SP_SCSR_MBn_SP2CL_IRQ_SET_ADDR(n),val)
#define HWIO_SP_CNOC_SP_SCSR_MBn_SP2CL_IRQ_SET_IRQ_BMSK                                         0x1f
#define HWIO_SP_CNOC_SP_SCSR_MBn_SP2CL_IRQ_SET_IRQ_SHFT                                          0x0

#define HWIO_SP_CNOC_SP_SCSR_MBn_SP2CL_IRQ_CLR_ADDR(n)                                    (SP_CNOC_SP_SCSR_REGS_REG_BASE      + 0x00006008 + 0x2000 * (n))
#define HWIO_SP_CNOC_SP_SCSR_MBn_SP2CL_IRQ_CLR_OFFS(n)                                    (SP_CNOC_SP_SCSR_REGS_REG_BASE_OFFS + 0x00006008 + 0x2000 * (n))
#define HWIO_SP_CNOC_SP_SCSR_MBn_SP2CL_IRQ_CLR_RMSK                                             0x1f
#define HWIO_SP_CNOC_SP_SCSR_MBn_SP2CL_IRQ_CLR_MAXn                                                3
#define HWIO_SP_CNOC_SP_SCSR_MBn_SP2CL_IRQ_CLR_OUTI(n,val)    \
        out_dword(HWIO_SP_CNOC_SP_SCSR_MBn_SP2CL_IRQ_CLR_ADDR(n),val)
#define HWIO_SP_CNOC_SP_SCSR_MBn_SP2CL_IRQ_CLR_IRQ_BMSK                                         0x1f
#define HWIO_SP_CNOC_SP_SCSR_MBn_SP2CL_IRQ_CLR_IRQ_SHFT                                          0x0

#define HWIO_SP_CNOC_SP_SCSR_MBn_SP2CL_IRQ_MASK_ADDR(n)                                   (SP_CNOC_SP_SCSR_REGS_REG_BASE      + 0x0000601c + 0x2000 * (n))
#define HWIO_SP_CNOC_SP_SCSR_MBn_SP2CL_IRQ_MASK_OFFS(n)                                   (SP_CNOC_SP_SCSR_REGS_REG_BASE_OFFS + 0x0000601c + 0x2000 * (n))
#define HWIO_SP_CNOC_SP_SCSR_MBn_SP2CL_IRQ_MASK_RMSK                                            0x1f
#define HWIO_SP_CNOC_SP_SCSR_MBn_SP2CL_IRQ_MASK_MAXn                                               3
#define HWIO_SP_CNOC_SP_SCSR_MBn_SP2CL_IRQ_MASK_INI(n)        \
        in_dword_masked(HWIO_SP_CNOC_SP_SCSR_MBn_SP2CL_IRQ_MASK_ADDR(n), HWIO_SP_CNOC_SP_SCSR_MBn_SP2CL_IRQ_MASK_RMSK)
#define HWIO_SP_CNOC_SP_SCSR_MBn_SP2CL_IRQ_MASK_INMI(n,mask)    \
        in_dword_masked(HWIO_SP_CNOC_SP_SCSR_MBn_SP2CL_IRQ_MASK_ADDR(n), mask)
#define HWIO_SP_CNOC_SP_SCSR_MBn_SP2CL_IRQ_MASK_OUTI(n,val)    \
        out_dword(HWIO_SP_CNOC_SP_SCSR_MBn_SP2CL_IRQ_MASK_ADDR(n),val)
#define HWIO_SP_CNOC_SP_SCSR_MBn_SP2CL_IRQ_MASK_OUTMI(n,mask,val) \
        out_dword_masked_ns(HWIO_SP_CNOC_SP_SCSR_MBn_SP2CL_IRQ_MASK_ADDR(n),mask,val,HWIO_SP_CNOC_SP_SCSR_MBn_SP2CL_IRQ_MASK_INI(n))
#define HWIO_SP_CNOC_SP_SCSR_MBn_SP2CL_IRQ_MASK_IRQ_BMSK                                        0x1f
#define HWIO_SP_CNOC_SP_SCSR_MBn_SP2CL_IRQ_MASK_IRQ_SHFT                                         0x0

#define HWIO_SP_CNOC_SP_SCSR_MBn_SP2CL_GPm_ADDR(n,m)                                      (SP_CNOC_SP_SCSR_REGS_REG_BASE      + 0x00006020 + 0x4 * (m) + 0x2000 * (n))
#define HWIO_SP_CNOC_SP_SCSR_MBn_SP2CL_GPm_OFFS(n,m)                                      (SP_CNOC_SP_SCSR_REGS_REG_BASE_OFFS + 0x00006020 + 0x4 * (m) + 0x2000 * (n))
#define HWIO_SP_CNOC_SP_SCSR_MBn_SP2CL_GPm_RMSK                                               0xffff
#define HWIO_SP_CNOC_SP_SCSR_MBn_SP2CL_GPm_MAXn                                                    3
#define HWIO_SP_CNOC_SP_SCSR_MBn_SP2CL_GPm_MAXm                                                    1
#define HWIO_SP_CNOC_SP_SCSR_MBn_SP2CL_GPm_INI2(n,m)        \
        in_dword_masked(HWIO_SP_CNOC_SP_SCSR_MBn_SP2CL_GPm_ADDR(n,m), HWIO_SP_CNOC_SP_SCSR_MBn_SP2CL_GPm_RMSK)
#define HWIO_SP_CNOC_SP_SCSR_MBn_SP2CL_GPm_INMI2(n,m,mask)    \
        in_dword_masked(HWIO_SP_CNOC_SP_SCSR_MBn_SP2CL_GPm_ADDR(n,m), mask)
#define HWIO_SP_CNOC_SP_SCSR_MBn_SP2CL_GPm_OUTI2(n,m,val)    \
        out_dword(HWIO_SP_CNOC_SP_SCSR_MBn_SP2CL_GPm_ADDR(n,m),val)
#define HWIO_SP_CNOC_SP_SCSR_MBn_SP2CL_GPm_OUTMI2(n,m,mask,val) \
        out_dword_masked_ns(HWIO_SP_CNOC_SP_SCSR_MBn_SP2CL_GPm_ADDR(n,m),mask,val,HWIO_SP_CNOC_SP_SCSR_MBn_SP2CL_GPm_INI2(n,m))
#define HWIO_SP_CNOC_SP_SCSR_MBn_SP2CL_GPm_GP_BMSK                                            0xffff
#define HWIO_SP_CNOC_SP_SCSR_MBn_SP2CL_GPm_GP_SHFT                                               0x0


#endif /* __SPSS_HWIO_H__ */
