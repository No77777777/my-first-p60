#ifndef __HALCLKHWIOASM_H__
#define __HALCLKHWIOASM_H__
/*
===========================================================================
*/
/**
  @file HALclkHWIOASM.h
  @brief Auto-generated HWIO interface include file.

  This file contains HWIO register definitions for the following modules:
    MPSS_PERPH
    MSS_QDSP6V66SS_PUB
    MSS_QDSP6SS_QDSP6SS_QTMR_AC
    MSS_QDSP6V66SS_QTMR_F0_0
    MSS_QDSP6V66SS_QTMR_F0_1
    MSS_QDSP6SS_QTMR_F1_1
    MSS_QDSP6V66SS_QTMR_F2_2
    MSS_QDSP6V66SS_CSR
    MSS_MSS_CC_REG
    TCSR_TCSR_REGS

  'Include' filters applied: <none>
  'Exclude' filters applied: TSENS ACR RPU RESERVED RESERVE DUMMY 
*/
/*
  ===========================================================================

  Copyright (c) 2017 Qualcomm Technologies Incorporated.
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

  $Header: //components/rel/core.mpss/10.0/systemdrivers/clock/image/mss/hw/sdx24/HALclkHWIOASM.h#1 $
  $DateTime: 2019/04/24 00:03:26 $
  $Author: pwbldsvc $

  ===========================================================================
*/

#define MODEM_TOP_BASE_PHYS      0x04000000
#define MODEM_TOP_BASE           MODEM_TOP_BASE_PHYS

#define CORE_TOP_CSR_BASE_PHYS   0x01F00000
#define CORE_TOP_CSR_BASE        CORE_TOP_CSR_BASE_PHYS



/*----------------------------------------------------------------------------
 * MODULE: TCSR_TCSR_REGS
 *--------------------------------------------------------------------------*/

#define TCSR_TCSR_REGS_REG_BASE                                                                                       (CORE_TOP_CSR_BASE      + 0x000c0000)
#define TCSR_TCSR_REGS_REG_BASE_OFFS                                                                                  0x000c0000

#define HWIO_TCSR_TIMEOUT_SLAVE_GLB_EN_ADDR                                                                           (TCSR_TCSR_REGS_REG_BASE      + 0x00000800)
#define HWIO_TCSR_TIMEOUT_SLAVE_GLB_EN_OFFS                                                                           (TCSR_TCSR_REGS_REG_BASE_OFFS + 0x00000800)
#define HWIO_TCSR_TIMEOUT_SLAVE_GLB_EN_RMSK                                                                                  0x1
#define HWIO_TCSR_TIMEOUT_SLAVE_GLB_EN_IN          \
        in_dword_masked(HWIO_TCSR_TIMEOUT_SLAVE_GLB_EN_ADDR, HWIO_TCSR_TIMEOUT_SLAVE_GLB_EN_RMSK)
#define HWIO_TCSR_TIMEOUT_SLAVE_GLB_EN_INM(m)      \
        in_dword_masked(HWIO_TCSR_TIMEOUT_SLAVE_GLB_EN_ADDR, m)
#define HWIO_TCSR_TIMEOUT_SLAVE_GLB_EN_OUT(v)      \
        out_dword(HWIO_TCSR_TIMEOUT_SLAVE_GLB_EN_ADDR,v)
#define HWIO_TCSR_TIMEOUT_SLAVE_GLB_EN_OUTM(m,v) \
        out_dword_masked_ns(HWIO_TCSR_TIMEOUT_SLAVE_GLB_EN_ADDR,m,v,HWIO_TCSR_TIMEOUT_SLAVE_GLB_EN_IN)
#define HWIO_TCSR_TIMEOUT_SLAVE_GLB_EN_TIMEOUT_SLAVE_GLB_EN_BMSK                                                             0x1
#define HWIO_TCSR_TIMEOUT_SLAVE_GLB_EN_TIMEOUT_SLAVE_GLB_EN_SHFT                                                             0x0

#define HWIO_TCSR_XPU_NSEN_STATUS_ADDR                                                                                (TCSR_TCSR_REGS_REG_BASE      + 0x00000820)
#define HWIO_TCSR_XPU_NSEN_STATUS_OFFS                                                                                (TCSR_TCSR_REGS_REG_BASE_OFFS + 0x00000820)
#define HWIO_TCSR_XPU_NSEN_STATUS_RMSK                                                                                       0x3
#define HWIO_TCSR_XPU_NSEN_STATUS_IN          \
        in_dword_masked(HWIO_TCSR_XPU_NSEN_STATUS_ADDR, HWIO_TCSR_XPU_NSEN_STATUS_RMSK)
#define HWIO_TCSR_XPU_NSEN_STATUS_INM(m)      \
        in_dword_masked(HWIO_TCSR_XPU_NSEN_STATUS_ADDR, m)
#define HWIO_TCSR_XPU_NSEN_STATUS_REGS_XPU_NSEN_STATUS_BMSK                                                                  0x2
#define HWIO_TCSR_XPU_NSEN_STATUS_REGS_XPU_NSEN_STATUS_SHFT                                                                  0x1
#define HWIO_TCSR_XPU_NSEN_STATUS_MUTEX_XPU_NSEN_STATUS_BMSK                                                                 0x1
#define HWIO_TCSR_XPU_NSEN_STATUS_MUTEX_XPU_NSEN_STATUS_SHFT                                                                 0x0

#define HWIO_TCSR_XPU_VMIDEN_STATUS_ADDR                                                                              (TCSR_TCSR_REGS_REG_BASE      + 0x00000824)
#define HWIO_TCSR_XPU_VMIDEN_STATUS_OFFS                                                                              (TCSR_TCSR_REGS_REG_BASE_OFFS + 0x00000824)
#define HWIO_TCSR_XPU_VMIDEN_STATUS_RMSK                                                                                     0x3
#define HWIO_TCSR_XPU_VMIDEN_STATUS_IN          \
        in_dword_masked(HWIO_TCSR_XPU_VMIDEN_STATUS_ADDR, HWIO_TCSR_XPU_VMIDEN_STATUS_RMSK)
#define HWIO_TCSR_XPU_VMIDEN_STATUS_INM(m)      \
        in_dword_masked(HWIO_TCSR_XPU_VMIDEN_STATUS_ADDR, m)
#define HWIO_TCSR_XPU_VMIDEN_STATUS_REGS_XPU_VMIDEN_STATUS_BMSK                                                              0x2
#define HWIO_TCSR_XPU_VMIDEN_STATUS_REGS_XPU_VMIDEN_STATUS_SHFT                                                              0x1
#define HWIO_TCSR_XPU_VMIDEN_STATUS_MUTEX_XPU_VMIDEN_STATUS_BMSK                                                             0x1
#define HWIO_TCSR_XPU_VMIDEN_STATUS_MUTEX_XPU_VMIDEN_STATUS_SHFT                                                             0x0

#define HWIO_TCSR_XPU_MSAEN_STATUS_ADDR                                                                               (TCSR_TCSR_REGS_REG_BASE      + 0x00000828)
#define HWIO_TCSR_XPU_MSAEN_STATUS_OFFS                                                                               (TCSR_TCSR_REGS_REG_BASE_OFFS + 0x00000828)
#define HWIO_TCSR_XPU_MSAEN_STATUS_RMSK                                                                                      0x3
#define HWIO_TCSR_XPU_MSAEN_STATUS_IN          \
        in_dword_masked(HWIO_TCSR_XPU_MSAEN_STATUS_ADDR, HWIO_TCSR_XPU_MSAEN_STATUS_RMSK)
#define HWIO_TCSR_XPU_MSAEN_STATUS_INM(m)      \
        in_dword_masked(HWIO_TCSR_XPU_MSAEN_STATUS_ADDR, m)
#define HWIO_TCSR_XPU_MSAEN_STATUS_REGS_XPU_MSAEN_STATUS_BMSK                                                                0x2
#define HWIO_TCSR_XPU_MSAEN_STATUS_REGS_XPU_MSAEN_STATUS_SHFT                                                                0x1
#define HWIO_TCSR_XPU_MSAEN_STATUS_MUTEX_XPU_MSAEN_STATUS_BMSK                                                               0x1
#define HWIO_TCSR_XPU_MSAEN_STATUS_MUTEX_XPU_MSAEN_STATUS_SHFT                                                               0x0

#define HWIO_TCSR_USB_IRQ_EN_ADDR                                                                                     (TCSR_TCSR_REGS_REG_BASE      + 0x0000083c)
#define HWIO_TCSR_USB_IRQ_EN_OFFS                                                                                     (TCSR_TCSR_REGS_REG_BASE_OFFS + 0x0000083c)
#define HWIO_TCSR_USB_IRQ_EN_RMSK                                                                                            0x1
#define HWIO_TCSR_USB_IRQ_EN_IN          \
        in_dword_masked(HWIO_TCSR_USB_IRQ_EN_ADDR, HWIO_TCSR_USB_IRQ_EN_RMSK)
#define HWIO_TCSR_USB_IRQ_EN_INM(m)      \
        in_dword_masked(HWIO_TCSR_USB_IRQ_EN_ADDR, m)
#define HWIO_TCSR_USB_IRQ_EN_OUT(v)      \
        out_dword(HWIO_TCSR_USB_IRQ_EN_ADDR,v)
#define HWIO_TCSR_USB_IRQ_EN_OUTM(m,v) \
        out_dword_masked_ns(HWIO_TCSR_USB_IRQ_EN_ADDR,m,v,HWIO_TCSR_USB_IRQ_EN_IN)
#define HWIO_TCSR_USB_IRQ_EN_USB_IRQ_EN_BMSK                                                                                 0x1
#define HWIO_TCSR_USB_IRQ_EN_USB_IRQ_EN_SHFT                                                                                 0x0

#define HWIO_TCSR_QPDI_DISABLE_CFG_ADDR                                                                               (TCSR_TCSR_REGS_REG_BASE      + 0x00001000)
#define HWIO_TCSR_QPDI_DISABLE_CFG_OFFS                                                                               (TCSR_TCSR_REGS_REG_BASE_OFFS + 0x00001000)
#define HWIO_TCSR_QPDI_DISABLE_CFG_RMSK                                                                                    0x303
#define HWIO_TCSR_QPDI_DISABLE_CFG_IN          \
        in_dword_masked(HWIO_TCSR_QPDI_DISABLE_CFG_ADDR, HWIO_TCSR_QPDI_DISABLE_CFG_RMSK)
#define HWIO_TCSR_QPDI_DISABLE_CFG_INM(m)      \
        in_dword_masked(HWIO_TCSR_QPDI_DISABLE_CFG_ADDR, m)
#define HWIO_TCSR_QPDI_DISABLE_CFG_OUT(v)      \
        out_dword(HWIO_TCSR_QPDI_DISABLE_CFG_ADDR,v)
#define HWIO_TCSR_QPDI_DISABLE_CFG_OUTM(m,v) \
        out_dword_masked_ns(HWIO_TCSR_QPDI_DISABLE_CFG_ADDR,m,v,HWIO_TCSR_QPDI_DISABLE_CFG_IN)
#define HWIO_TCSR_QPDI_DISABLE_CFG_QPDI_SPMI_DBG_ACK_BMSK                                                                  0x200
#define HWIO_TCSR_QPDI_DISABLE_CFG_QPDI_SPMI_DBG_ACK_SHFT                                                                    0x9
#define HWIO_TCSR_QPDI_DISABLE_CFG_QPDI_SPMI_DBG_REQ_BMSK                                                                  0x100
#define HWIO_TCSR_QPDI_DISABLE_CFG_QPDI_SPMI_DBG_REQ_SHFT                                                                    0x8
#define HWIO_TCSR_QPDI_DISABLE_CFG_SPMI_HANDSHAKE_DISABLE_BMSK                                                               0x2
#define HWIO_TCSR_QPDI_DISABLE_CFG_SPMI_HANDSHAKE_DISABLE_SHFT                                                               0x1
#define HWIO_TCSR_QPDI_DISABLE_CFG_QPDI_DISABLE_CFG_BMSK                                                                     0x1
#define HWIO_TCSR_QPDI_DISABLE_CFG_QPDI_DISABLE_CFG_SHFT                                                                     0x0

#define HWIO_TCSR_SS_XPU_NON_SEC_INTR0_ADDR                                                                           (TCSR_TCSR_REGS_REG_BASE      + 0x00002000)
#define HWIO_TCSR_SS_XPU_NON_SEC_INTR0_OFFS                                                                           (TCSR_TCSR_REGS_REG_BASE_OFFS + 0x00002000)
#define HWIO_TCSR_SS_XPU_NON_SEC_INTR0_RMSK                                                                           0xffffffff
#define HWIO_TCSR_SS_XPU_NON_SEC_INTR0_IN          \
        in_dword_masked(HWIO_TCSR_SS_XPU_NON_SEC_INTR0_ADDR, HWIO_TCSR_SS_XPU_NON_SEC_INTR0_RMSK)
#define HWIO_TCSR_SS_XPU_NON_SEC_INTR0_INM(m)      \
        in_dword_masked(HWIO_TCSR_SS_XPU_NON_SEC_INTR0_ADDR, m)
#define HWIO_TCSR_SS_XPU_NON_SEC_INTR0_QM_XPU3_NON_SEC_INTR0_BMSK                                                     0x40000000
#define HWIO_TCSR_SS_XPU_NON_SEC_INTR0_QM_XPU3_NON_SEC_INTR0_SHFT                                                           0x1e
#define HWIO_TCSR_SS_XPU_NON_SEC_INTR0_MEMNOC_MCDMA_XPU3_NON_SEC_INTR0_BMSK                                           0x20000000
#define HWIO_TCSR_SS_XPU_NON_SEC_INTR0_MEMNOC_MCDMA_XPU3_NON_SEC_INTR0_SHFT                                                 0x1d
#define HWIO_TCSR_SS_XPU_NON_SEC_INTR0_SPMI_BAM_APU_NON_SEC_INTR0_BMSK                                                0x10000000
#define HWIO_TCSR_SS_XPU_NON_SEC_INTR0_SPMI_BAM_APU_NON_SEC_INTR0_SHFT                                                      0x1c
#define HWIO_TCSR_SS_XPU_NON_SEC_INTR0_IPA_APU_NON_SEC_INTR0_BMSK                                                      0x8000000
#define HWIO_TCSR_SS_XPU_NON_SEC_INTR0_IPA_APU_NON_SEC_INTR0_SHFT                                                           0x1b
#define HWIO_TCSR_SS_XPU_NON_SEC_INTR0_BLSP_BAM_APU_NON_SEC_INTR0_BMSK                                                 0x4000000
#define HWIO_TCSR_SS_XPU_NON_SEC_INTR0_BLSP_BAM_APU_NON_SEC_INTR0_SHFT                                                      0x1a
#define HWIO_TCSR_SS_XPU_NON_SEC_INTR0_PKA_XPU_NON_SEC_INTR0_BMSK                                                      0x2000000
#define HWIO_TCSR_SS_XPU_NON_SEC_INTR0_PKA_XPU_NON_SEC_INTR0_SHFT                                                           0x19
#define HWIO_TCSR_SS_XPU_NON_SEC_INTR0_QPIC_XPU2_NON_SEC_INTR0_BMSK                                                    0x1000000
#define HWIO_TCSR_SS_XPU_NON_SEC_INTR0_QPIC_XPU2_NON_SEC_INTR0_SHFT                                                         0x18
#define HWIO_TCSR_SS_XPU_NON_SEC_INTR0_QXS_Q6_TCM_SS_MPU_XPU3_NON_SEC_INTR0_BMSK                                        0x400000
#define HWIO_TCSR_SS_XPU_NON_SEC_INTR0_QXS_Q6_TCM_SS_MPU_XPU3_NON_SEC_INTR0_SHFT                                            0x16
#define HWIO_TCSR_SS_XPU_NON_SEC_INTR0_QXM_MSS_NAV_CE_MS_MPU_XPU3_NON_SEC_INTR0_BMSK                                    0x200000
#define HWIO_TCSR_SS_XPU_NON_SEC_INTR0_QXM_MSS_NAV_CE_MS_MPU_XPU3_NON_SEC_INTR0_SHFT                                        0x15
#define HWIO_TCSR_SS_XPU_NON_SEC_INTR0_QNM_MEMNOC_MS_MPU_XPU3_NON_SEC_INTR0_BMSK                                        0x100000
#define HWIO_TCSR_SS_XPU_NON_SEC_INTR0_QNM_MEMNOC_MS_MPU_XPU3_NON_SEC_INTR0_SHFT                                            0x14
#define HWIO_TCSR_SS_XPU_NON_SEC_INTR0_QNM_AGGRE_NOC_IPA_MS_MPU_XPU3_NON_SEC_INTR0_BMSK                                  0x80000
#define HWIO_TCSR_SS_XPU_NON_SEC_INTR0_QNM_AGGRE_NOC_IPA_MS_MPU_XPU3_NON_SEC_INTR0_SHFT                                     0x13
#define HWIO_TCSR_SS_XPU_NON_SEC_INTR0_QHS_SNOC_CFG_SS_MPU_XPU3_NON_SEC_INTR0_BMSK                                       0x40000
#define HWIO_TCSR_SS_XPU_NON_SEC_INTR0_QHS_SNOC_CFG_SS_MPU_XPU3_NON_SEC_INTR0_SHFT                                          0x12
#define HWIO_TCSR_SS_XPU_NON_SEC_INTR0_TLMM_CENTRAL_XPU_NON_SEC_INTR0_BMSK                                               0x20000
#define HWIO_TCSR_SS_XPU_NON_SEC_INTR0_TLMM_CENTRAL_XPU_NON_SEC_INTR0_SHFT                                                  0x11
#define HWIO_TCSR_SS_XPU_NON_SEC_INTR0_SPDM_WRAPPER_XPU_NON_SEC_INTR0_BMSK                                               0x10000
#define HWIO_TCSR_SS_XPU_NON_SEC_INTR0_SPDM_WRAPPER_XPU_NON_SEC_INTR0_SHFT                                                  0x10
#define HWIO_TCSR_SS_XPU_NON_SEC_INTR0_OCIMEM_MPU_NON_SEC_INTR0_BMSK                                                      0x8000
#define HWIO_TCSR_SS_XPU_NON_SEC_INTR0_OCIMEM_MPU_NON_SEC_INTR0_SHFT                                                         0xf
#define HWIO_TCSR_SS_XPU_NON_SEC_INTR0_DTP_QHS_NON_BROADCAST_MPU_MAINO_XPU3_NON_SEC_INTR0_BMSK                            0x4000
#define HWIO_TCSR_SS_XPU_NON_SEC_INTR0_DTP_QHS_NON_BROADCAST_MPU_MAINO_XPU3_NON_SEC_INTR0_SHFT                               0xe
#define HWIO_TCSR_SS_XPU_NON_SEC_INTR0_BOOT_ROM_NON_SEC_INTR0_BMSK                                                        0x2000
#define HWIO_TCSR_SS_XPU_NON_SEC_INTR0_BOOT_ROM_NON_SEC_INTR0_SHFT                                                           0xd
#define HWIO_TCSR_SS_XPU_NON_SEC_INTR0_DTP_QHS_BROADCAST_MPU_MAINO_XPU3_NON_SEC_INTR0_BMSK                                0x1000
#define HWIO_TCSR_SS_XPU_NON_SEC_INTR0_DTP_QHS_BROADCAST_MPU_MAINO_XPU3_NON_SEC_INTR0_SHFT                                   0xc
#define HWIO_TCSR_SS_XPU_NON_SEC_INTR0_LLCC0_XPU3_NON_SEC_INTR0_BMSK                                                       0x800
#define HWIO_TCSR_SS_XPU_NON_SEC_INTR0_LLCC0_XPU3_NON_SEC_INTR0_SHFT                                                         0xb
#define HWIO_TCSR_SS_XPU_NON_SEC_INTR0_GCC_XPU_NON_SEC_INTR0_BMSK                                                          0x400
#define HWIO_TCSR_SS_XPU_NON_SEC_INTR0_GCC_XPU_NON_SEC_INTR0_SHFT                                                            0xa
#define HWIO_TCSR_SS_XPU_NON_SEC_INTR0_MEMNOC_XPU3_NON_SEC_INTR0_BMSK                                                      0x200
#define HWIO_TCSR_SS_XPU_NON_SEC_INTR0_MEMNOC_XPU3_NON_SEC_INTR0_SHFT                                                        0x9
#define HWIO_TCSR_SS_XPU_NON_SEC_INTR0_QHM_AOSS_MS_MPU_XPU3_NON_SEC_INTR0_BMSK                                             0x100
#define HWIO_TCSR_SS_XPU_NON_SEC_INTR0_QHM_AOSS_MS_MPU_XPU3_NON_SEC_INTR0_SHFT                                               0x8
#define HWIO_TCSR_SS_XPU_NON_SEC_INTR0_SEC_CTRL_XPU3_NON_SEC_INTR0_BMSK                                                     0x80
#define HWIO_TCSR_SS_XPU_NON_SEC_INTR0_SEC_CTRL_XPU3_NON_SEC_INTR0_SHFT                                                      0x7
#define HWIO_TCSR_SS_XPU_NON_SEC_INTR0_DCC_XPU_NON_SEC_INTR0_BMSK                                                           0x40
#define HWIO_TCSR_SS_XPU_NON_SEC_INTR0_DCC_XPU_NON_SEC_INTR0_SHFT                                                            0x6
#define HWIO_TCSR_SS_XPU_NON_SEC_INTR0_CRYPTO_BAM_APU_NON_SEC_INTR0_BMSK                                                    0x10
#define HWIO_TCSR_SS_XPU_NON_SEC_INTR0_CRYPTO_BAM_APU_NON_SEC_INTR0_SHFT                                                     0x4
#define HWIO_TCSR_SS_XPU_NON_SEC_INTR0_TCSR_MUTEX_XPU_NON_SEC_INTR0_BMSK                                                     0x8
#define HWIO_TCSR_SS_XPU_NON_SEC_INTR0_TCSR_MUTEX_XPU_NON_SEC_INTR0_SHFT                                                     0x3
#define HWIO_TCSR_SS_XPU_NON_SEC_INTR0_TCSR_REGS_XPU_NON_SEC_INTR0_BMSK                                                      0x4
#define HWIO_TCSR_SS_XPU_NON_SEC_INTR0_TCSR_REGS_XPU_NON_SEC_INTR0_SHFT                                                      0x2
#define HWIO_TCSR_SS_XPU_NON_SEC_INTR0_AOSS_MPU_NON_SEC_INTR0_BMSK                                                           0x2
#define HWIO_TCSR_SS_XPU_NON_SEC_INTR0_AOSS_MPU_NON_SEC_INTR0_SHFT                                                           0x1
#define HWIO_TCSR_SS_XPU_NON_SEC_INTR0_PMIC_ARB_MPU_NON_SEC_INTR0_BMSK                                                       0x1
#define HWIO_TCSR_SS_XPU_NON_SEC_INTR0_PMIC_ARB_MPU_NON_SEC_INTR0_SHFT                                                       0x0

#define HWIO_TCSR_SS_XPU_NON_SEC_INTR1_ADDR                                                                           (TCSR_TCSR_REGS_REG_BASE      + 0x00002004)
#define HWIO_TCSR_SS_XPU_NON_SEC_INTR1_OFFS                                                                           (TCSR_TCSR_REGS_REG_BASE_OFFS + 0x00002004)
#define HWIO_TCSR_SS_XPU_NON_SEC_INTR1_RMSK                                                                              0x7ffff
#define HWIO_TCSR_SS_XPU_NON_SEC_INTR1_IN          \
        in_dword_masked(HWIO_TCSR_SS_XPU_NON_SEC_INTR1_ADDR, HWIO_TCSR_SS_XPU_NON_SEC_INTR1_RMSK)
#define HWIO_TCSR_SS_XPU_NON_SEC_INTR1_INM(m)      \
        in_dword_masked(HWIO_TCSR_SS_XPU_NON_SEC_INTR1_ADDR, m)

#define HWIO_TCSR_SS_VMIDMT_CLIENT_NON_SEC_INTR_ADDR                                                                  (TCSR_TCSR_REGS_REG_BASE      + 0x00002010)
#define HWIO_TCSR_SS_VMIDMT_CLIENT_NON_SEC_INTR_OFFS                                                                  (TCSR_TCSR_REGS_REG_BASE_OFFS + 0x00002010)
#define HWIO_TCSR_SS_VMIDMT_CLIENT_NON_SEC_INTR_RMSK                                                                     0x3ffff
#define HWIO_TCSR_SS_VMIDMT_CLIENT_NON_SEC_INTR_IN          \
        in_dword_masked(HWIO_TCSR_SS_VMIDMT_CLIENT_NON_SEC_INTR_ADDR, HWIO_TCSR_SS_VMIDMT_CLIENT_NON_SEC_INTR_RMSK)
#define HWIO_TCSR_SS_VMIDMT_CLIENT_NON_SEC_INTR_INM(m)      \
        in_dword_masked(HWIO_TCSR_SS_VMIDMT_CLIENT_NON_SEC_INTR_ADDR, m)
#define HWIO_TCSR_SS_VMIDMT_CLIENT_NON_SEC_INTR_QDSS_DAP_VMIDMT_NSGIRPT_BMSK                                                0x80
#define HWIO_TCSR_SS_VMIDMT_CLIENT_NON_SEC_INTR_QDSS_DAP_VMIDMT_NSGIRPT_SHFT                                                 0x7
#define HWIO_TCSR_SS_VMIDMT_CLIENT_NON_SEC_INTR_QDSS_TRACE_VMIDMT_NSGIRPT_BMSK                                              0x40
#define HWIO_TCSR_SS_VMIDMT_CLIENT_NON_SEC_INTR_QDSS_TRACE_VMIDMT_NSGIRPT_SHFT                                               0x6
#define HWIO_TCSR_SS_VMIDMT_CLIENT_NON_SEC_INTR_IPA_VMIDMT_NSGIRPT_BMSK                                                     0x20
#define HWIO_TCSR_SS_VMIDMT_CLIENT_NON_SEC_INTR_IPA_VMIDMT_NSGIRPT_SHFT                                                      0x5
#define HWIO_TCSR_SS_VMIDMT_CLIENT_NON_SEC_INTR_SPMI_BAM_VMIDMT_NSGIRPT_BMSK                                                0x10
#define HWIO_TCSR_SS_VMIDMT_CLIENT_NON_SEC_INTR_SPMI_BAM_VMIDMT_NSGIRPT_SHFT                                                 0x4
#define HWIO_TCSR_SS_VMIDMT_CLIENT_NON_SEC_INTR_BAM_EDML_VMIDMT_NSGIRPT_BMSK                                                 0x8
#define HWIO_TCSR_SS_VMIDMT_CLIENT_NON_SEC_INTR_BAM_EDML_VMIDMT_NSGIRPT_SHFT                                                 0x3
#define HWIO_TCSR_SS_VMIDMT_CLIENT_NON_SEC_INTR_QPIC_BAM_VMIDMT_NSGIRPT_BMSK                                                 0x4
#define HWIO_TCSR_SS_VMIDMT_CLIENT_NON_SEC_INTR_QPIC_BAM_VMIDMT_NSGIRPT_SHFT                                                 0x2
#define HWIO_TCSR_SS_VMIDMT_CLIENT_NON_SEC_INTR_AOP_VMIDMT_NSGIRPT_BMSK                                                      0x2
#define HWIO_TCSR_SS_VMIDMT_CLIENT_NON_SEC_INTR_AOP_VMIDMT_NSGIRPT_SHFT                                                      0x1
#define HWIO_TCSR_SS_VMIDMT_CLIENT_NON_SEC_INTR_CRYPTO_VMIDMT_NSGIRPT_BMSK                                                   0x1
#define HWIO_TCSR_SS_VMIDMT_CLIENT_NON_SEC_INTR_CRYPTO_VMIDMT_NSGIRPT_SHFT                                                   0x0

#define HWIO_TCSR_SS_MMU_CLIENT_NON_SEC_INTR_ADDR                                                                     (TCSR_TCSR_REGS_REG_BASE      + 0x00002014)
#define HWIO_TCSR_SS_MMU_CLIENT_NON_SEC_INTR_OFFS                                                                     (TCSR_TCSR_REGS_REG_BASE_OFFS + 0x00002014)
#define HWIO_TCSR_SS_MMU_CLIENT_NON_SEC_INTR_RMSK                                                                         0x1e42
#define HWIO_TCSR_SS_MMU_CLIENT_NON_SEC_INTR_IN          \
        in_dword_masked(HWIO_TCSR_SS_MMU_CLIENT_NON_SEC_INTR_ADDR, HWIO_TCSR_SS_MMU_CLIENT_NON_SEC_INTR_RMSK)
#define HWIO_TCSR_SS_MMU_CLIENT_NON_SEC_INTR_INM(m)      \
        in_dword_masked(HWIO_TCSR_SS_MMU_CLIENT_NON_SEC_INTR_ADDR, m)
#define HWIO_TCSR_SS_MMU_CLIENT_NON_SEC_INTR_LPASS_5_IRQ_OUT_SECURITY_BMSK                                                0x1000
#define HWIO_TCSR_SS_MMU_CLIENT_NON_SEC_INTR_LPASS_5_IRQ_OUT_SECURITY_SHFT                                                   0xc
#define HWIO_TCSR_SS_MMU_CLIENT_NON_SEC_INTR_LPASS_13_IRQ_OUT_SECURITY_BMSK                                                0x800
#define HWIO_TCSR_SS_MMU_CLIENT_NON_SEC_INTR_LPASS_13_IRQ_OUT_SECURITY_SHFT                                                  0xb
#define HWIO_TCSR_SS_MMU_CLIENT_NON_SEC_INTR_AGGR2_NOC_SMMU_MMU_NSGIRPT_BMSK                                               0x400
#define HWIO_TCSR_SS_MMU_CLIENT_NON_SEC_INTR_AGGR2_NOC_SMMU_MMU_NSGIRPT_SHFT                                                 0xa
#define HWIO_TCSR_SS_MMU_CLIENT_NON_SEC_INTR_AGGR1_NOC_SMMU_MMU_NSGIRPT_BMSK                                               0x200
#define HWIO_TCSR_SS_MMU_CLIENT_NON_SEC_INTR_AGGR1_NOC_SMMU_MMU_NSGIRPT_SHFT                                                 0x9
#define HWIO_TCSR_SS_MMU_CLIENT_NON_SEC_INTR_OGPU_MMU_NSGIRPT_BMSK                                                          0x40
#define HWIO_TCSR_SS_MMU_CLIENT_NON_SEC_INTR_OGPU_MMU_NSGIRPT_SHFT                                                           0x6
#define HWIO_TCSR_SS_MMU_CLIENT_NON_SEC_INTR_MMSS_BIMC_SMMU_NSGIRPT_BMSK                                                     0x2
#define HWIO_TCSR_SS_MMU_CLIENT_NON_SEC_INTR_MMSS_BIMC_SMMU_NSGIRPT_SHFT                                                     0x1

#define HWIO_TCSR_SS_XPU_NON_SEC_INTR0_ENABLE_ADDR                                                                    (TCSR_TCSR_REGS_REG_BASE      + 0x00002040)
#define HWIO_TCSR_SS_XPU_NON_SEC_INTR0_ENABLE_OFFS                                                                    (TCSR_TCSR_REGS_REG_BASE_OFFS + 0x00002040)
#define HWIO_TCSR_SS_XPU_NON_SEC_INTR0_ENABLE_RMSK                                                                    0xffffffff
#define HWIO_TCSR_SS_XPU_NON_SEC_INTR0_ENABLE_IN          \
        in_dword_masked(HWIO_TCSR_SS_XPU_NON_SEC_INTR0_ENABLE_ADDR, HWIO_TCSR_SS_XPU_NON_SEC_INTR0_ENABLE_RMSK)
#define HWIO_TCSR_SS_XPU_NON_SEC_INTR0_ENABLE_INM(m)      \
        in_dword_masked(HWIO_TCSR_SS_XPU_NON_SEC_INTR0_ENABLE_ADDR, m)
#define HWIO_TCSR_SS_XPU_NON_SEC_INTR0_ENABLE_OUT(v)      \
        out_dword(HWIO_TCSR_SS_XPU_NON_SEC_INTR0_ENABLE_ADDR,v)
#define HWIO_TCSR_SS_XPU_NON_SEC_INTR0_ENABLE_OUTM(m,v) \
        out_dword_masked_ns(HWIO_TCSR_SS_XPU_NON_SEC_INTR0_ENABLE_ADDR,m,v,HWIO_TCSR_SS_XPU_NON_SEC_INTR0_ENABLE_IN)
#define HWIO_TCSR_SS_XPU_NON_SEC_INTR0_ENABLE_QM_XPU3_NON_SEC_INTR0_ENABLE_BMSK                                       0x40000000
#define HWIO_TCSR_SS_XPU_NON_SEC_INTR0_ENABLE_QM_XPU3_NON_SEC_INTR0_ENABLE_SHFT                                             0x1e
#define HWIO_TCSR_SS_XPU_NON_SEC_INTR0_ENABLE_MEMNOC_MCDMA_XPU3_NON_SEC_INTR0_ENABLE_BMSK                             0x20000000
#define HWIO_TCSR_SS_XPU_NON_SEC_INTR0_ENABLE_MEMNOC_MCDMA_XPU3_NON_SEC_INTR0_ENABLE_SHFT                                   0x1d
#define HWIO_TCSR_SS_XPU_NON_SEC_INTR0_ENABLE_SPMI_BAM_APU_NON_SEC_INTR0_ENABLE_BMSK                                  0x10000000
#define HWIO_TCSR_SS_XPU_NON_SEC_INTR0_ENABLE_SPMI_BAM_APU_NON_SEC_INTR0_ENABLE_SHFT                                        0x1c
#define HWIO_TCSR_SS_XPU_NON_SEC_INTR0_ENABLE_IPA_APU_NON_SEC_INTR0_ENABLE_BMSK                                        0x8000000
#define HWIO_TCSR_SS_XPU_NON_SEC_INTR0_ENABLE_IPA_APU_NON_SEC_INTR0_ENABLE_SHFT                                             0x1b
#define HWIO_TCSR_SS_XPU_NON_SEC_INTR0_ENABLE_BLSP_BAM_APU_NON_SEC_INTR0_ENABLE_BMSK                                   0x4000000
#define HWIO_TCSR_SS_XPU_NON_SEC_INTR0_ENABLE_BLSP_BAM_APU_NON_SEC_INTR0_ENABLE_SHFT                                        0x1a
#define HWIO_TCSR_SS_XPU_NON_SEC_INTR0_ENABLE_PKA_XPU_NON_SEC_INTR0_ENABLE_BMSK                                        0x2000000
#define HWIO_TCSR_SS_XPU_NON_SEC_INTR0_ENABLE_PKA_XPU_NON_SEC_INTR0_ENABLE_SHFT                                             0x19
#define HWIO_TCSR_SS_XPU_NON_SEC_INTR0_ENABLE_QPIC_XPU2_NON_SEC_INTR0_ENABLE_BMSK                                      0x1000000
#define HWIO_TCSR_SS_XPU_NON_SEC_INTR0_ENABLE_QPIC_XPU2_NON_SEC_INTR0_ENABLE_SHFT                                           0x18
#define HWIO_TCSR_SS_XPU_NON_SEC_INTR0_ENABLE_QXS_Q6_TCM_SS_MPU_XPU3_NON_SEC_INTR0_ENABLE_BMSK                          0x400000
#define HWIO_TCSR_SS_XPU_NON_SEC_INTR0_ENABLE_QXS_Q6_TCM_SS_MPU_XPU3_NON_SEC_INTR0_ENABLE_SHFT                              0x16
#define HWIO_TCSR_SS_XPU_NON_SEC_INTR0_ENABLE_QXM_MSS_NAV_CE_MS_MPU_XPU3_NON_SEC_INTR0_ENABLE_BMSK                      0x200000
#define HWIO_TCSR_SS_XPU_NON_SEC_INTR0_ENABLE_QXM_MSS_NAV_CE_MS_MPU_XPU3_NON_SEC_INTR0_ENABLE_SHFT                          0x15
#define HWIO_TCSR_SS_XPU_NON_SEC_INTR0_ENABLE_QNM_MEMNOC_MS_MPU_XPU3_NON_SEC_INTR0_ENABLE_BMSK                          0x100000
#define HWIO_TCSR_SS_XPU_NON_SEC_INTR0_ENABLE_QNM_MEMNOC_MS_MPU_XPU3_NON_SEC_INTR0_ENABLE_SHFT                              0x14
#define HWIO_TCSR_SS_XPU_NON_SEC_INTR0_ENABLE_QNM_AGGRE_NOC_IPA_MS_MPU_XPU3_NON_SEC_INTR0_ENABLE_BMSK                    0x80000
#define HWIO_TCSR_SS_XPU_NON_SEC_INTR0_ENABLE_QNM_AGGRE_NOC_IPA_MS_MPU_XPU3_NON_SEC_INTR0_ENABLE_SHFT                       0x13
#define HWIO_TCSR_SS_XPU_NON_SEC_INTR0_ENABLE_QHS_SNOC_CFG_SS_MPU_XPU3_NON_SEC_INTR0_ENABLE_BMSK                         0x40000
#define HWIO_TCSR_SS_XPU_NON_SEC_INTR0_ENABLE_QHS_SNOC_CFG_SS_MPU_XPU3_NON_SEC_INTR0_ENABLE_SHFT                            0x12
#define HWIO_TCSR_SS_XPU_NON_SEC_INTR0_ENABLE_TLMM_CENTRAL_XPU_NON_SEC_INTR0_ENABLE_BMSK                                 0x20000
#define HWIO_TCSR_SS_XPU_NON_SEC_INTR0_ENABLE_TLMM_CENTRAL_XPU_NON_SEC_INTR0_ENABLE_SHFT                                    0x11
#define HWIO_TCSR_SS_XPU_NON_SEC_INTR0_ENABLE_SPDM_WRAPPER_XPU_NON_SEC_INTR0_ENABLE_BMSK                                 0x10000
#define HWIO_TCSR_SS_XPU_NON_SEC_INTR0_ENABLE_SPDM_WRAPPER_XPU_NON_SEC_INTR0_ENABLE_SHFT                                    0x10
#define HWIO_TCSR_SS_XPU_NON_SEC_INTR0_ENABLE_OCIMEM_MPU_NON_SEC_INTR0_ENABLE_BMSK                                        0x8000
#define HWIO_TCSR_SS_XPU_NON_SEC_INTR0_ENABLE_OCIMEM_MPU_NON_SEC_INTR0_ENABLE_SHFT                                           0xf
#define HWIO_TCSR_SS_XPU_NON_SEC_INTR0_ENABLE_DTP_QHS_NON_BROADCAST_MPU_MAINO_XPU3_NON_SEC_INTR0_ENABLE_BMSK              0x4000
#define HWIO_TCSR_SS_XPU_NON_SEC_INTR0_ENABLE_DTP_QHS_NON_BROADCAST_MPU_MAINO_XPU3_NON_SEC_INTR0_ENABLE_SHFT                 0xe
#define HWIO_TCSR_SS_XPU_NON_SEC_INTR0_ENABLE_BOOT_ROM_NON_SEC_INTR0_ENABLE_BMSK                                          0x2000
#define HWIO_TCSR_SS_XPU_NON_SEC_INTR0_ENABLE_BOOT_ROM_NON_SEC_INTR0_ENABLE_SHFT                                             0xd
#define HWIO_TCSR_SS_XPU_NON_SEC_INTR0_ENABLE_DTP_QHS_BROADCAST_MPU_MAINO_XPU3_NON_SEC_INTR0_ENABLE_BMSK                  0x1000
#define HWIO_TCSR_SS_XPU_NON_SEC_INTR0_ENABLE_DTP_QHS_BROADCAST_MPU_MAINO_XPU3_NON_SEC_INTR0_ENABLE_SHFT                     0xc
#define HWIO_TCSR_SS_XPU_NON_SEC_INTR0_ENABLE_LLCC0_XPU3_NON_SEC_INTR0_ENABLE_BMSK                                         0x800
#define HWIO_TCSR_SS_XPU_NON_SEC_INTR0_ENABLE_LLCC0_XPU3_NON_SEC_INTR0_ENABLE_SHFT                                           0xb
#define HWIO_TCSR_SS_XPU_NON_SEC_INTR0_ENABLE_GCC_XPU_NON_SEC_INTR0_ENABLE_BMSK                                            0x400
#define HWIO_TCSR_SS_XPU_NON_SEC_INTR0_ENABLE_GCC_XPU_NON_SEC_INTR0_ENABLE_SHFT                                              0xa
#define HWIO_TCSR_SS_XPU_NON_SEC_INTR0_ENABLE_MEMNOC_XPU3_NON_SEC_INTR0_ENABLE_BMSK                                        0x200
#define HWIO_TCSR_SS_XPU_NON_SEC_INTR0_ENABLE_MEMNOC_XPU3_NON_SEC_INTR0_ENABLE_SHFT                                          0x9
#define HWIO_TCSR_SS_XPU_NON_SEC_INTR0_ENABLE_QHM_AOSS_MS_MPU_XPU3_NON_SEC_INTR0_ENABLE_BMSK                               0x100
#define HWIO_TCSR_SS_XPU_NON_SEC_INTR0_ENABLE_QHM_AOSS_MS_MPU_XPU3_NON_SEC_INTR0_ENABLE_SHFT                                 0x8
#define HWIO_TCSR_SS_XPU_NON_SEC_INTR0_ENABLE_SEC_CTRL_XPU3_NON_SEC_INTR0_ENABLE_BMSK                                       0x80
#define HWIO_TCSR_SS_XPU_NON_SEC_INTR0_ENABLE_SEC_CTRL_XPU3_NON_SEC_INTR0_ENABLE_SHFT                                        0x7
#define HWIO_TCSR_SS_XPU_NON_SEC_INTR0_ENABLE_DCC_XPU_NON_SEC_INTR0_ENABLE_BMSK                                             0x40
#define HWIO_TCSR_SS_XPU_NON_SEC_INTR0_ENABLE_DCC_XPU_NON_SEC_INTR0_ENABLE_SHFT                                              0x6
#define HWIO_TCSR_SS_XPU_NON_SEC_INTR0_ENABLE_CRYPTO_BAM_APU_NON_SEC_INTR0_ENABLE_BMSK                                      0x10
#define HWIO_TCSR_SS_XPU_NON_SEC_INTR0_ENABLE_CRYPTO_BAM_APU_NON_SEC_INTR0_ENABLE_SHFT                                       0x4
#define HWIO_TCSR_SS_XPU_NON_SEC_INTR0_ENABLE_TCSR_MUTEX_XPU_NON_SEC_INTR0_ENABLE_BMSK                                       0x8
#define HWIO_TCSR_SS_XPU_NON_SEC_INTR0_ENABLE_TCSR_MUTEX_XPU_NON_SEC_INTR0_ENABLE_SHFT                                       0x3
#define HWIO_TCSR_SS_XPU_NON_SEC_INTR0_ENABLE_TCSR_REGS_XPU_NON_SEC_INTR0_ENABLE_BMSK                                        0x4
#define HWIO_TCSR_SS_XPU_NON_SEC_INTR0_ENABLE_TCSR_REGS_XPU_NON_SEC_INTR0_ENABLE_SHFT                                        0x2
#define HWIO_TCSR_SS_XPU_NON_SEC_INTR0_ENABLE_AOSS_MPU_NON_SEC_INTR0_ENABLE_BMSK                                             0x2
#define HWIO_TCSR_SS_XPU_NON_SEC_INTR0_ENABLE_AOSS_MPU_NON_SEC_INTR0_ENABLE_SHFT                                             0x1
#define HWIO_TCSR_SS_XPU_NON_SEC_INTR0_ENABLE_PMIC_ARB_MPU_NON_SEC_INTR0_ENABLE_BMSK                                         0x1
#define HWIO_TCSR_SS_XPU_NON_SEC_INTR0_ENABLE_PMIC_ARB_MPU_NON_SEC_INTR0_ENABLE_SHFT                                         0x0

#define HWIO_TCSR_SS_XPU_NON_SEC_INTR1_ENABLE_ADDR                                                                    (TCSR_TCSR_REGS_REG_BASE      + 0x00002044)
#define HWIO_TCSR_SS_XPU_NON_SEC_INTR1_ENABLE_OFFS                                                                    (TCSR_TCSR_REGS_REG_BASE_OFFS + 0x00002044)
#define HWIO_TCSR_SS_XPU_NON_SEC_INTR1_ENABLE_RMSK                                                                       0x7ffff
#define HWIO_TCSR_SS_XPU_NON_SEC_INTR1_ENABLE_IN          \
        in_dword_masked(HWIO_TCSR_SS_XPU_NON_SEC_INTR1_ENABLE_ADDR, HWIO_TCSR_SS_XPU_NON_SEC_INTR1_ENABLE_RMSK)
#define HWIO_TCSR_SS_XPU_NON_SEC_INTR1_ENABLE_INM(m)      \
        in_dword_masked(HWIO_TCSR_SS_XPU_NON_SEC_INTR1_ENABLE_ADDR, m)
#define HWIO_TCSR_SS_XPU_NON_SEC_INTR1_ENABLE_OUT(v)      \
        out_dword(HWIO_TCSR_SS_XPU_NON_SEC_INTR1_ENABLE_ADDR,v)
#define HWIO_TCSR_SS_XPU_NON_SEC_INTR1_ENABLE_OUTM(m,v) \
        out_dword_masked_ns(HWIO_TCSR_SS_XPU_NON_SEC_INTR1_ENABLE_ADDR,m,v,HWIO_TCSR_SS_XPU_NON_SEC_INTR1_ENABLE_IN)

#define HWIO_TCSR_SS_VMIDMT_CLIENT_NON_SEC_INTR_ENABLE_ADDR                                                           (TCSR_TCSR_REGS_REG_BASE      + 0x00002050)
#define HWIO_TCSR_SS_VMIDMT_CLIENT_NON_SEC_INTR_ENABLE_OFFS                                                           (TCSR_TCSR_REGS_REG_BASE_OFFS + 0x00002050)
#define HWIO_TCSR_SS_VMIDMT_CLIENT_NON_SEC_INTR_ENABLE_RMSK                                                              0x3ffff
#define HWIO_TCSR_SS_VMIDMT_CLIENT_NON_SEC_INTR_ENABLE_IN          \
        in_dword_masked(HWIO_TCSR_SS_VMIDMT_CLIENT_NON_SEC_INTR_ENABLE_ADDR, HWIO_TCSR_SS_VMIDMT_CLIENT_NON_SEC_INTR_ENABLE_RMSK)
#define HWIO_TCSR_SS_VMIDMT_CLIENT_NON_SEC_INTR_ENABLE_INM(m)      \
        in_dword_masked(HWIO_TCSR_SS_VMIDMT_CLIENT_NON_SEC_INTR_ENABLE_ADDR, m)
#define HWIO_TCSR_SS_VMIDMT_CLIENT_NON_SEC_INTR_ENABLE_OUT(v)      \
        out_dword(HWIO_TCSR_SS_VMIDMT_CLIENT_NON_SEC_INTR_ENABLE_ADDR,v)
#define HWIO_TCSR_SS_VMIDMT_CLIENT_NON_SEC_INTR_ENABLE_OUTM(m,v) \
        out_dword_masked_ns(HWIO_TCSR_SS_VMIDMT_CLIENT_NON_SEC_INTR_ENABLE_ADDR,m,v,HWIO_TCSR_SS_VMIDMT_CLIENT_NON_SEC_INTR_ENABLE_IN)
#define HWIO_TCSR_SS_VMIDMT_CLIENT_NON_SEC_INTR_ENABLE_QDSS_DAP_VMIDMT_NSGIRPT_ENABLE_BMSK                                  0x80
#define HWIO_TCSR_SS_VMIDMT_CLIENT_NON_SEC_INTR_ENABLE_QDSS_DAP_VMIDMT_NSGIRPT_ENABLE_SHFT                                   0x7
#define HWIO_TCSR_SS_VMIDMT_CLIENT_NON_SEC_INTR_ENABLE_QDSS_TRACE_VMIDMT_NSGIRPT_ENABLE_BMSK                                0x40
#define HWIO_TCSR_SS_VMIDMT_CLIENT_NON_SEC_INTR_ENABLE_QDSS_TRACE_VMIDMT_NSGIRPT_ENABLE_SHFT                                 0x6
#define HWIO_TCSR_SS_VMIDMT_CLIENT_NON_SEC_INTR_ENABLE_IPA_VMIDMT_NSGIRPT_ENABLE_BMSK                                       0x20
#define HWIO_TCSR_SS_VMIDMT_CLIENT_NON_SEC_INTR_ENABLE_IPA_VMIDMT_NSGIRPT_ENABLE_SHFT                                        0x5
#define HWIO_TCSR_SS_VMIDMT_CLIENT_NON_SEC_INTR_ENABLE_SPMI_BAM_VMIDMT_NSGIRPT_ENABLE_BMSK                                  0x10
#define HWIO_TCSR_SS_VMIDMT_CLIENT_NON_SEC_INTR_ENABLE_SPMI_BAM_VMIDMT_NSGIRPT_ENABLE_SHFT                                   0x4
#define HWIO_TCSR_SS_VMIDMT_CLIENT_NON_SEC_INTR_ENABLE_BAM_EDML_VMIDMT_NSGIRPT_ENABLE_BMSK                                   0x8
#define HWIO_TCSR_SS_VMIDMT_CLIENT_NON_SEC_INTR_ENABLE_BAM_EDML_VMIDMT_NSGIRPT_ENABLE_SHFT                                   0x3
#define HWIO_TCSR_SS_VMIDMT_CLIENT_NON_SEC_INTR_ENABLE_QPIC_BAM_VMIDMT_NSGIRPT_ENABLE_BMSK                                   0x4
#define HWIO_TCSR_SS_VMIDMT_CLIENT_NON_SEC_INTR_ENABLE_QPIC_BAM_VMIDMT_NSGIRPT_ENABLE_SHFT                                   0x2
#define HWIO_TCSR_SS_VMIDMT_CLIENT_NON_SEC_INTR_ENABLE_AOP_VMIDMT_NSGIRPT_ENABLE_BMSK                                        0x2
#define HWIO_TCSR_SS_VMIDMT_CLIENT_NON_SEC_INTR_ENABLE_AOP_VMIDMT_NSGIRPT_ENABLE_SHFT                                        0x1
#define HWIO_TCSR_SS_VMIDMT_CLIENT_NON_SEC_INTR_ENABLE_CRYPTO_VMIDMT_NSGIRPT_ENABLE_BMSK                                     0x1
#define HWIO_TCSR_SS_VMIDMT_CLIENT_NON_SEC_INTR_ENABLE_CRYPTO_VMIDMT_NSGIRPT_ENABLE_SHFT                                     0x0

#define HWIO_TCSR_SS_MMU_CLIENT_NON_SEC_INTR_ENABLE_ADDR                                                              (TCSR_TCSR_REGS_REG_BASE      + 0x00002054)
#define HWIO_TCSR_SS_MMU_CLIENT_NON_SEC_INTR_ENABLE_OFFS                                                              (TCSR_TCSR_REGS_REG_BASE_OFFS + 0x00002054)
#define HWIO_TCSR_SS_MMU_CLIENT_NON_SEC_INTR_ENABLE_RMSK                                                                  0x1e42
#define HWIO_TCSR_SS_MMU_CLIENT_NON_SEC_INTR_ENABLE_IN          \
        in_dword_masked(HWIO_TCSR_SS_MMU_CLIENT_NON_SEC_INTR_ENABLE_ADDR, HWIO_TCSR_SS_MMU_CLIENT_NON_SEC_INTR_ENABLE_RMSK)
#define HWIO_TCSR_SS_MMU_CLIENT_NON_SEC_INTR_ENABLE_INM(m)      \
        in_dword_masked(HWIO_TCSR_SS_MMU_CLIENT_NON_SEC_INTR_ENABLE_ADDR, m)
#define HWIO_TCSR_SS_MMU_CLIENT_NON_SEC_INTR_ENABLE_OUT(v)      \
        out_dword(HWIO_TCSR_SS_MMU_CLIENT_NON_SEC_INTR_ENABLE_ADDR,v)
#define HWIO_TCSR_SS_MMU_CLIENT_NON_SEC_INTR_ENABLE_OUTM(m,v) \
        out_dword_masked_ns(HWIO_TCSR_SS_MMU_CLIENT_NON_SEC_INTR_ENABLE_ADDR,m,v,HWIO_TCSR_SS_MMU_CLIENT_NON_SEC_INTR_ENABLE_IN)
#define HWIO_TCSR_SS_MMU_CLIENT_NON_SEC_INTR_ENABLE_LPASS_5_IRQ_OUT_SECURITY_INTR_ENABLE_BMSK                             0x1000
#define HWIO_TCSR_SS_MMU_CLIENT_NON_SEC_INTR_ENABLE_LPASS_5_IRQ_OUT_SECURITY_INTR_ENABLE_SHFT                                0xc
#define HWIO_TCSR_SS_MMU_CLIENT_NON_SEC_INTR_ENABLE_LPASS_13_IRQ_OUT_SECURITY_INTR_ENABLE_BMSK                             0x800
#define HWIO_TCSR_SS_MMU_CLIENT_NON_SEC_INTR_ENABLE_LPASS_13_IRQ_OUT_SECURITY_INTR_ENABLE_SHFT                               0xb
#define HWIO_TCSR_SS_MMU_CLIENT_NON_SEC_INTR_ENABLE_AGGR2_NOC_SMMU_MMU_NSGIRPT_ENABLE_BMSK                                 0x400
#define HWIO_TCSR_SS_MMU_CLIENT_NON_SEC_INTR_ENABLE_AGGR2_NOC_SMMU_MMU_NSGIRPT_ENABLE_SHFT                                   0xa
#define HWIO_TCSR_SS_MMU_CLIENT_NON_SEC_INTR_ENABLE_AGGR1_NOC_SMMU_MMU_NSGIRPT_ENABLE_BMSK                                 0x200
#define HWIO_TCSR_SS_MMU_CLIENT_NON_SEC_INTR_ENABLE_AGGR1_NOC_SMMU_MMU_NSGIRPT_ENABLE_SHFT                                   0x9
#define HWIO_TCSR_SS_MMU_CLIENT_NON_SEC_INTR_ENABLE_OGPU_MMU_NSGIRPT_ENABLE_BMSK                                            0x40
#define HWIO_TCSR_SS_MMU_CLIENT_NON_SEC_INTR_ENABLE_OGPU_MMU_NSGIRPT_ENABLE_SHFT                                             0x6
#define HWIO_TCSR_SS_MMU_CLIENT_NON_SEC_INTR_ENABLE_MMSS_BIMC_SMMU_NSGIRPT_ENABLE_BMSK                                       0x2
#define HWIO_TCSR_SS_MMU_CLIENT_NON_SEC_INTR_ENABLE_MMSS_BIMC_SMMU_NSGIRPT_ENABLE_SHFT                                       0x1

#define HWIO_TCSR_SS_VMIDMT_CFG_NON_SEC_INTR_ADDR                                                                     (TCSR_TCSR_REGS_REG_BASE      + 0x00003000)
#define HWIO_TCSR_SS_VMIDMT_CFG_NON_SEC_INTR_OFFS                                                                     (TCSR_TCSR_REGS_REG_BASE_OFFS + 0x00003000)
#define HWIO_TCSR_SS_VMIDMT_CFG_NON_SEC_INTR_RMSK                                                                        0x3ffff
#define HWIO_TCSR_SS_VMIDMT_CFG_NON_SEC_INTR_IN          \
        in_dword_masked(HWIO_TCSR_SS_VMIDMT_CFG_NON_SEC_INTR_ADDR, HWIO_TCSR_SS_VMIDMT_CFG_NON_SEC_INTR_RMSK)
#define HWIO_TCSR_SS_VMIDMT_CFG_NON_SEC_INTR_INM(m)      \
        in_dword_masked(HWIO_TCSR_SS_VMIDMT_CFG_NON_SEC_INTR_ADDR, m)
#define HWIO_TCSR_SS_VMIDMT_CFG_NON_SEC_INTR_QDSS_DAP_VMIDMT_NSGCFGIRPT_BMSK                                                0x80
#define HWIO_TCSR_SS_VMIDMT_CFG_NON_SEC_INTR_QDSS_DAP_VMIDMT_NSGCFGIRPT_SHFT                                                 0x7
#define HWIO_TCSR_SS_VMIDMT_CFG_NON_SEC_INTR_QDSS_TRACE_VMIDMT_NSGCFGIRPT_BMSK                                              0x40
#define HWIO_TCSR_SS_VMIDMT_CFG_NON_SEC_INTR_QDSS_TRACE_VMIDMT_NSGCFGIRPT_SHFT                                               0x6
#define HWIO_TCSR_SS_VMIDMT_CFG_NON_SEC_INTR_IPA_VMIDMT_NSGCFGIRPT_BMSK                                                     0x20
#define HWIO_TCSR_SS_VMIDMT_CFG_NON_SEC_INTR_IPA_VMIDMT_NSGCFGIRPT_SHFT                                                      0x5
#define HWIO_TCSR_SS_VMIDMT_CFG_NON_SEC_INTR_SPMI_BAM_VMIDMT_NSGCFGIRPT_BMSK                                                0x10
#define HWIO_TCSR_SS_VMIDMT_CFG_NON_SEC_INTR_SPMI_BAM_VMIDMT_NSGCFGIRPT_SHFT                                                 0x4
#define HWIO_TCSR_SS_VMIDMT_CFG_NON_SEC_INTR_BAM_EDML_VMIDMT_NSGCFGIRPT_BMSK                                                 0x8
#define HWIO_TCSR_SS_VMIDMT_CFG_NON_SEC_INTR_BAM_EDML_VMIDMT_NSGCFGIRPT_SHFT                                                 0x3
#define HWIO_TCSR_SS_VMIDMT_CFG_NON_SEC_INTR_QPIC_BAM_VMIDMT_NSGCFGIRPT_BMSK                                                 0x4
#define HWIO_TCSR_SS_VMIDMT_CFG_NON_SEC_INTR_QPIC_BAM_VMIDMT_NSGCFGIRPT_SHFT                                                 0x2
#define HWIO_TCSR_SS_VMIDMT_CFG_NON_SEC_INTR_AOP_VMIDMT_NSGCFGIRPT_BMSK                                                      0x2
#define HWIO_TCSR_SS_VMIDMT_CFG_NON_SEC_INTR_AOP_VMIDMT_NSGCFGIRPT_SHFT                                                      0x1
#define HWIO_TCSR_SS_VMIDMT_CFG_NON_SEC_INTR_CRYPTO_VMIDMT_NSGCFGIRPT_BMSK                                                   0x1
#define HWIO_TCSR_SS_VMIDMT_CFG_NON_SEC_INTR_CRYPTO_VMIDMT_NSGCFGIRPT_SHFT                                                   0x0

#define HWIO_TCSR_SS_MMU_CFG_NON_SEC_INTR_ADDR                                                                        (TCSR_TCSR_REGS_REG_BASE      + 0x00003004)
#define HWIO_TCSR_SS_MMU_CFG_NON_SEC_INTR_OFFS                                                                        (TCSR_TCSR_REGS_REG_BASE_OFFS + 0x00003004)
#define HWIO_TCSR_SS_MMU_CFG_NON_SEC_INTR_RMSK                                                                            0x1e42
#define HWIO_TCSR_SS_MMU_CFG_NON_SEC_INTR_IN          \
        in_dword_masked(HWIO_TCSR_SS_MMU_CFG_NON_SEC_INTR_ADDR, HWIO_TCSR_SS_MMU_CFG_NON_SEC_INTR_RMSK)
#define HWIO_TCSR_SS_MMU_CFG_NON_SEC_INTR_INM(m)      \
        in_dword_masked(HWIO_TCSR_SS_MMU_CFG_NON_SEC_INTR_ADDR, m)
#define HWIO_TCSR_SS_MMU_CFG_NON_SEC_INTR_LPASS_4_IRQ_OUT_SECURITY_BMSK                                                   0x1000
#define HWIO_TCSR_SS_MMU_CFG_NON_SEC_INTR_LPASS_4_IRQ_OUT_SECURITY_SHFT                                                      0xc
#define HWIO_TCSR_SS_MMU_CFG_NON_SEC_INTR_LPASS_12_IRQ_OUT_SECURITY_BMSK                                                   0x800
#define HWIO_TCSR_SS_MMU_CFG_NON_SEC_INTR_LPASS_12_IRQ_OUT_SECURITY_SHFT                                                     0xb
#define HWIO_TCSR_SS_MMU_CFG_NON_SEC_INTR_AGGR2_NOC_SMMU_MMU_NSGCFGIRPT_BMSK                                               0x400
#define HWIO_TCSR_SS_MMU_CFG_NON_SEC_INTR_AGGR2_NOC_SMMU_MMU_NSGCFGIRPT_SHFT                                                 0xa
#define HWIO_TCSR_SS_MMU_CFG_NON_SEC_INTR_AGGR1_NOC_SMMU_MMU_NSGCFGIRPT_BMSK                                               0x200
#define HWIO_TCSR_SS_MMU_CFG_NON_SEC_INTR_AGGR1_NOC_SMMU_MMU_NSGCFGIRPT_SHFT                                                 0x9
#define HWIO_TCSR_SS_MMU_CFG_NON_SEC_INTR_OGPU_MMU_NSGCFGIRPT_BMSK                                                          0x40
#define HWIO_TCSR_SS_MMU_CFG_NON_SEC_INTR_OGPU_MMU_NSGCFGIRPT_SHFT                                                           0x6
#define HWIO_TCSR_SS_MMU_CFG_NON_SEC_INTR_MMSS_BIMC_SMMU_NSGCFGIRPT_BMSK                                                     0x2
#define HWIO_TCSR_SS_MMU_CFG_NON_SEC_INTR_MMSS_BIMC_SMMU_NSGCFGIRPT_SHFT                                                     0x1

#define HWIO_TCSR_SS_VMIDMT_CFG_NON_SEC_INTR_ENABLE_ADDR                                                              (TCSR_TCSR_REGS_REG_BASE      + 0x00003040)
#define HWIO_TCSR_SS_VMIDMT_CFG_NON_SEC_INTR_ENABLE_OFFS                                                              (TCSR_TCSR_REGS_REG_BASE_OFFS + 0x00003040)
#define HWIO_TCSR_SS_VMIDMT_CFG_NON_SEC_INTR_ENABLE_RMSK                                                                 0x3ffff
#define HWIO_TCSR_SS_VMIDMT_CFG_NON_SEC_INTR_ENABLE_IN          \
        in_dword_masked(HWIO_TCSR_SS_VMIDMT_CFG_NON_SEC_INTR_ENABLE_ADDR, HWIO_TCSR_SS_VMIDMT_CFG_NON_SEC_INTR_ENABLE_RMSK)
#define HWIO_TCSR_SS_VMIDMT_CFG_NON_SEC_INTR_ENABLE_INM(m)      \
        in_dword_masked(HWIO_TCSR_SS_VMIDMT_CFG_NON_SEC_INTR_ENABLE_ADDR, m)
#define HWIO_TCSR_SS_VMIDMT_CFG_NON_SEC_INTR_ENABLE_OUT(v)      \
        out_dword(HWIO_TCSR_SS_VMIDMT_CFG_NON_SEC_INTR_ENABLE_ADDR,v)
#define HWIO_TCSR_SS_VMIDMT_CFG_NON_SEC_INTR_ENABLE_OUTM(m,v) \
        out_dword_masked_ns(HWIO_TCSR_SS_VMIDMT_CFG_NON_SEC_INTR_ENABLE_ADDR,m,v,HWIO_TCSR_SS_VMIDMT_CFG_NON_SEC_INTR_ENABLE_IN)
#define HWIO_TCSR_SS_VMIDMT_CFG_NON_SEC_INTR_ENABLE_QDSS_DAP_VMIDMT_NSGCFGIRPT_ENABLE_BMSK                                  0x80
#define HWIO_TCSR_SS_VMIDMT_CFG_NON_SEC_INTR_ENABLE_QDSS_DAP_VMIDMT_NSGCFGIRPT_ENABLE_SHFT                                   0x7
#define HWIO_TCSR_SS_VMIDMT_CFG_NON_SEC_INTR_ENABLE_QDSS_TRACE_VMIDMT_NSGCFGIRPT_ENABLE_BMSK                                0x40
#define HWIO_TCSR_SS_VMIDMT_CFG_NON_SEC_INTR_ENABLE_QDSS_TRACE_VMIDMT_NSGCFGIRPT_ENABLE_SHFT                                 0x6
#define HWIO_TCSR_SS_VMIDMT_CFG_NON_SEC_INTR_ENABLE_IPA_VMIDMT_NSGCFGIRPT_ENABLE_BMSK                                       0x20
#define HWIO_TCSR_SS_VMIDMT_CFG_NON_SEC_INTR_ENABLE_IPA_VMIDMT_NSGCFGIRPT_ENABLE_SHFT                                        0x5
#define HWIO_TCSR_SS_VMIDMT_CFG_NON_SEC_INTR_ENABLE_SPMI_BAM_VMIDMT_NSGCFGIRPT_ENABLE_BMSK                                  0x10
#define HWIO_TCSR_SS_VMIDMT_CFG_NON_SEC_INTR_ENABLE_SPMI_BAM_VMIDMT_NSGCFGIRPT_ENABLE_SHFT                                   0x4
#define HWIO_TCSR_SS_VMIDMT_CFG_NON_SEC_INTR_ENABLE_BAM_EDML_VMIDMT_NSGCFGIRPT_ENABLE_BMSK                                   0x8
#define HWIO_TCSR_SS_VMIDMT_CFG_NON_SEC_INTR_ENABLE_BAM_EDML_VMIDMT_NSGCFGIRPT_ENABLE_SHFT                                   0x3
#define HWIO_TCSR_SS_VMIDMT_CFG_NON_SEC_INTR_ENABLE_QPIC_BAM_VMIDMT_NSGCFGIRPT_ENABLE_BMSK                                   0x4
#define HWIO_TCSR_SS_VMIDMT_CFG_NON_SEC_INTR_ENABLE_QPIC_BAM_VMIDMT_NSGCFGIRPT_ENABLE_SHFT                                   0x2
#define HWIO_TCSR_SS_VMIDMT_CFG_NON_SEC_INTR_ENABLE_AOP_VMIDMT_NSGCFGIRPT_ENABLE_BMSK                                        0x2
#define HWIO_TCSR_SS_VMIDMT_CFG_NON_SEC_INTR_ENABLE_AOP_VMIDMT_NSGCFGIRPT_ENABLE_SHFT                                        0x1
#define HWIO_TCSR_SS_VMIDMT_CFG_NON_SEC_INTR_ENABLE_CRYPTO_VMIDMT_NSGCFGIRPT_ENABLE_BMSK                                     0x1
#define HWIO_TCSR_SS_VMIDMT_CFG_NON_SEC_INTR_ENABLE_CRYPTO_VMIDMT_NSGCFGIRPT_ENABLE_SHFT                                     0x0

#define HWIO_TCSR_SS_MMU_CFG_NON_SEC_INTR_ENABLE_ADDR                                                                 (TCSR_TCSR_REGS_REG_BASE      + 0x00003044)
#define HWIO_TCSR_SS_MMU_CFG_NON_SEC_INTR_ENABLE_OFFS                                                                 (TCSR_TCSR_REGS_REG_BASE_OFFS + 0x00003044)
#define HWIO_TCSR_SS_MMU_CFG_NON_SEC_INTR_ENABLE_RMSK                                                                     0x1e42
#define HWIO_TCSR_SS_MMU_CFG_NON_SEC_INTR_ENABLE_IN          \
        in_dword_masked(HWIO_TCSR_SS_MMU_CFG_NON_SEC_INTR_ENABLE_ADDR, HWIO_TCSR_SS_MMU_CFG_NON_SEC_INTR_ENABLE_RMSK)
#define HWIO_TCSR_SS_MMU_CFG_NON_SEC_INTR_ENABLE_INM(m)      \
        in_dword_masked(HWIO_TCSR_SS_MMU_CFG_NON_SEC_INTR_ENABLE_ADDR, m)
#define HWIO_TCSR_SS_MMU_CFG_NON_SEC_INTR_ENABLE_OUT(v)      \
        out_dword(HWIO_TCSR_SS_MMU_CFG_NON_SEC_INTR_ENABLE_ADDR,v)
#define HWIO_TCSR_SS_MMU_CFG_NON_SEC_INTR_ENABLE_OUTM(m,v) \
        out_dword_masked_ns(HWIO_TCSR_SS_MMU_CFG_NON_SEC_INTR_ENABLE_ADDR,m,v,HWIO_TCSR_SS_MMU_CFG_NON_SEC_INTR_ENABLE_IN)
#define HWIO_TCSR_SS_MMU_CFG_NON_SEC_INTR_ENABLE_LPASS_4_IRQ_OUT_SECURITY_INTR_ENABLE_BMSK                                0x1000
#define HWIO_TCSR_SS_MMU_CFG_NON_SEC_INTR_ENABLE_LPASS_4_IRQ_OUT_SECURITY_INTR_ENABLE_SHFT                                   0xc
#define HWIO_TCSR_SS_MMU_CFG_NON_SEC_INTR_ENABLE_LPASS_12_IRQ_OUT_SECURITY_INTR_ENABLE_BMSK                                0x800
#define HWIO_TCSR_SS_MMU_CFG_NON_SEC_INTR_ENABLE_LPASS_12_IRQ_OUT_SECURITY_INTR_ENABLE_SHFT                                  0xb
#define HWIO_TCSR_SS_MMU_CFG_NON_SEC_INTR_ENABLE_AGGR2_NOC_SMMU_MMU_NSGCFGIRPT_ENABLE_BMSK                                 0x400
#define HWIO_TCSR_SS_MMU_CFG_NON_SEC_INTR_ENABLE_AGGR2_NOC_SMMU_MMU_NSGCFGIRPT_ENABLE_SHFT                                   0xa
#define HWIO_TCSR_SS_MMU_CFG_NON_SEC_INTR_ENABLE_AGGR1_NOC_SMMU_MMU_NSGCFGIRPT_ENABLE_BMSK                                 0x200
#define HWIO_TCSR_SS_MMU_CFG_NON_SEC_INTR_ENABLE_AGGR1_NOC_SMMU_MMU_NSGCFGIRPT_ENABLE_SHFT                                   0x9
#define HWIO_TCSR_SS_MMU_CFG_NON_SEC_INTR_ENABLE_OGPU_MMU_NSGCFGIRPT_ENABLE_BMSK                                            0x40
#define HWIO_TCSR_SS_MMU_CFG_NON_SEC_INTR_ENABLE_OGPU_MMU_NSGCFGIRPT_ENABLE_SHFT                                             0x6
#define HWIO_TCSR_SS_MMU_CFG_NON_SEC_INTR_ENABLE_MMSS_BIMC_SMMU_NSGCFGIRPT_ENABLE_BMSK                                       0x2
#define HWIO_TCSR_SS_MMU_CFG_NON_SEC_INTR_ENABLE_MMSS_BIMC_SMMU_NSGCFGIRPT_ENABLE_SHFT                                       0x1

#define HWIO_TCSR_SS_XPU_SEC_INTR0_ADDR                                                                               (TCSR_TCSR_REGS_REG_BASE      + 0x00004000)
#define HWIO_TCSR_SS_XPU_SEC_INTR0_OFFS                                                                               (TCSR_TCSR_REGS_REG_BASE_OFFS + 0x00004000)
#define HWIO_TCSR_SS_XPU_SEC_INTR0_RMSK                                                                               0xffffffff
#define HWIO_TCSR_SS_XPU_SEC_INTR0_IN          \
        in_dword_masked(HWIO_TCSR_SS_XPU_SEC_INTR0_ADDR, HWIO_TCSR_SS_XPU_SEC_INTR0_RMSK)
#define HWIO_TCSR_SS_XPU_SEC_INTR0_INM(m)      \
        in_dword_masked(HWIO_TCSR_SS_XPU_SEC_INTR0_ADDR, m)
#define HWIO_TCSR_SS_XPU_SEC_INTR0_QM_XPU3_SEC_INTR0_BMSK                                                             0x40000000
#define HWIO_TCSR_SS_XPU_SEC_INTR0_QM_XPU3_SEC_INTR0_SHFT                                                                   0x1e
#define HWIO_TCSR_SS_XPU_SEC_INTR0_MEMNOC_MCDMA_XPU3_SEC_INTR0_BMSK                                                   0x20000000
#define HWIO_TCSR_SS_XPU_SEC_INTR0_MEMNOC_MCDMA_XPU3_SEC_INTR0_SHFT                                                         0x1d
#define HWIO_TCSR_SS_XPU_SEC_INTR0_SPMI_BAM_APU_SEC_INTR0_BMSK                                                        0x10000000
#define HWIO_TCSR_SS_XPU_SEC_INTR0_SPMI_BAM_APU_SEC_INTR0_SHFT                                                              0x1c
#define HWIO_TCSR_SS_XPU_SEC_INTR0_IPA_APU_SEC_INTR0_BMSK                                                              0x8000000
#define HWIO_TCSR_SS_XPU_SEC_INTR0_IPA_APU_SEC_INTR0_SHFT                                                                   0x1b
#define HWIO_TCSR_SS_XPU_SEC_INTR0_BLSP_BAM_APU_SEC_INTR0_BMSK                                                         0x4000000
#define HWIO_TCSR_SS_XPU_SEC_INTR0_BLSP_BAM_APU_SEC_INTR0_SHFT                                                              0x1a
#define HWIO_TCSR_SS_XPU_SEC_INTR0_PKA_XPU_SEC_INTR0_BMSK                                                              0x2000000
#define HWIO_TCSR_SS_XPU_SEC_INTR0_PKA_XPU_SEC_INTR0_SHFT                                                                   0x19
#define HWIO_TCSR_SS_XPU_SEC_INTR0_QPIC_XPU2_SEC_INTR0_BMSK                                                            0x1000000
#define HWIO_TCSR_SS_XPU_SEC_INTR0_QPIC_XPU2_SEC_INTR0_SHFT                                                                 0x18
#define HWIO_TCSR_SS_XPU_SEC_INTR0_QXS_Q6_TCM_SS_MPU_XPU3_SEC_INTR0_BMSK                                                0x400000
#define HWIO_TCSR_SS_XPU_SEC_INTR0_QXS_Q6_TCM_SS_MPU_XPU3_SEC_INTR0_SHFT                                                    0x16
#define HWIO_TCSR_SS_XPU_SEC_INTR0_QXM_MSS_NAV_CE_MS_MPU_XPU3_SEC_INTR0_BMSK                                            0x200000
#define HWIO_TCSR_SS_XPU_SEC_INTR0_QXM_MSS_NAV_CE_MS_MPU_XPU3_SEC_INTR0_SHFT                                                0x15
#define HWIO_TCSR_SS_XPU_SEC_INTR0_QNM_MEMNOC_MS_MPU_XPU3_SEC_INTR0_BMSK                                                0x100000
#define HWIO_TCSR_SS_XPU_SEC_INTR0_QNM_MEMNOC_MS_MPU_XPU3_SEC_INTR0_SHFT                                                    0x14
#define HWIO_TCSR_SS_XPU_SEC_INTR0_QNM_AGGRE_NOC_IPA_MS_MPU_XPU3_SEC_INTR0_BMSK                                          0x80000
#define HWIO_TCSR_SS_XPU_SEC_INTR0_QNM_AGGRE_NOC_IPA_MS_MPU_XPU3_SEC_INTR0_SHFT                                             0x13
#define HWIO_TCSR_SS_XPU_SEC_INTR0_QHS_SNOC_CFG_SS_MPU_XPU3_SEC_INTR0_BMSK                                               0x40000
#define HWIO_TCSR_SS_XPU_SEC_INTR0_QHS_SNOC_CFG_SS_MPU_XPU3_SEC_INTR0_SHFT                                                  0x12
#define HWIO_TCSR_SS_XPU_SEC_INTR0_TLMM_CENTRAL_XPU_SEC_INTR0_BMSK                                                       0x20000
#define HWIO_TCSR_SS_XPU_SEC_INTR0_TLMM_CENTRAL_XPU_SEC_INTR0_SHFT                                                          0x11
#define HWIO_TCSR_SS_XPU_SEC_INTR0_SPDM_WRAPPER_XPU_SEC_INTR0_BMSK                                                       0x10000
#define HWIO_TCSR_SS_XPU_SEC_INTR0_SPDM_WRAPPER_XPU_SEC_INTR0_SHFT                                                          0x10
#define HWIO_TCSR_SS_XPU_SEC_INTR0_OCIMEM_MPU_SEC_INTR0_BMSK                                                              0x8000
#define HWIO_TCSR_SS_XPU_SEC_INTR0_OCIMEM_MPU_SEC_INTR0_SHFT                                                                 0xf
#define HWIO_TCSR_SS_XPU_SEC_INTR0_DTP_QHS_NON_BROADCAST_MPU_MAINO_XPU3_SEC_INTR0_BMSK                                    0x4000
#define HWIO_TCSR_SS_XPU_SEC_INTR0_DTP_QHS_NON_BROADCAST_MPU_MAINO_XPU3_SEC_INTR0_SHFT                                       0xe
#define HWIO_TCSR_SS_XPU_SEC_INTR0_BOOT_ROM_SEC_INTR0_BMSK                                                                0x2000
#define HWIO_TCSR_SS_XPU_SEC_INTR0_BOOT_ROM_SEC_INTR0_SHFT                                                                   0xd
#define HWIO_TCSR_SS_XPU_SEC_INTR0_DTP_QHS_BROADCAST_MPU_MAINO_XPU3_SEC_INTR0_BMSK                                        0x1000
#define HWIO_TCSR_SS_XPU_SEC_INTR0_DTP_QHS_BROADCAST_MPU_MAINO_XPU3_SEC_INTR0_SHFT                                           0xc
#define HWIO_TCSR_SS_XPU_SEC_INTR0_LLCC0_XPU3_SEC_INTR0_BMSK                                                               0x800
#define HWIO_TCSR_SS_XPU_SEC_INTR0_LLCC0_XPU3_SEC_INTR0_SHFT                                                                 0xb
#define HWIO_TCSR_SS_XPU_SEC_INTR0_GCC_XPU_SEC_INTR0_BMSK                                                                  0x400
#define HWIO_TCSR_SS_XPU_SEC_INTR0_GCC_XPU_SEC_INTR0_SHFT                                                                    0xa
#define HWIO_TCSR_SS_XPU_SEC_INTR0_MEMNOC_XPU3_SEC_INTR0_BMSK                                                              0x200
#define HWIO_TCSR_SS_XPU_SEC_INTR0_MEMNOC_XPU3_SEC_INTR0_SHFT                                                                0x9
#define HWIO_TCSR_SS_XPU_SEC_INTR0_QHM_AOSS_MS_MPU_XPU3_SEC_INTR0_BMSK                                                     0x100
#define HWIO_TCSR_SS_XPU_SEC_INTR0_QHM_AOSS_MS_MPU_XPU3_SEC_INTR0_SHFT                                                       0x8
#define HWIO_TCSR_SS_XPU_SEC_INTR0_SEC_CTRL_XPU3_SEC_INTR0_BMSK                                                             0x80
#define HWIO_TCSR_SS_XPU_SEC_INTR0_SEC_CTRL_XPU3_SEC_INTR0_SHFT                                                              0x7
#define HWIO_TCSR_SS_XPU_SEC_INTR0_DCC_XPU_SEC_INTR0_BMSK                                                                   0x40
#define HWIO_TCSR_SS_XPU_SEC_INTR0_DCC_XPU_SEC_INTR0_SHFT                                                                    0x6
#define HWIO_TCSR_SS_XPU_SEC_INTR0_CRYPTO_BAM_APU_SEC_INTR0_BMSK                                                            0x10
#define HWIO_TCSR_SS_XPU_SEC_INTR0_CRYPTO_BAM_APU_SEC_INTR0_SHFT                                                             0x4
#define HWIO_TCSR_SS_XPU_SEC_INTR0_TCSR_MUTEX_XPU_SEC_INTR0_BMSK                                                             0x8
#define HWIO_TCSR_SS_XPU_SEC_INTR0_TCSR_MUTEX_XPU_SEC_INTR0_SHFT                                                             0x3
#define HWIO_TCSR_SS_XPU_SEC_INTR0_TCSR_REGS_XPU_SEC_INTR0_BMSK                                                              0x4
#define HWIO_TCSR_SS_XPU_SEC_INTR0_TCSR_REGS_XPU_SEC_INTR0_SHFT                                                              0x2
#define HWIO_TCSR_SS_XPU_SEC_INTR0_AOSS_MPU_SEC_INTR0_BMSK                                                                   0x2
#define HWIO_TCSR_SS_XPU_SEC_INTR0_AOSS_MPU_SEC_INTR0_SHFT                                                                   0x1
#define HWIO_TCSR_SS_XPU_SEC_INTR0_PMIC_ARB_MPU_SEC_INTR0_BMSK                                                               0x1
#define HWIO_TCSR_SS_XPU_SEC_INTR0_PMIC_ARB_MPU_SEC_INTR0_SHFT                                                               0x0

#define HWIO_TCSR_SS_XPU_SEC_INTR1_ADDR                                                                               (TCSR_TCSR_REGS_REG_BASE      + 0x00004004)
#define HWIO_TCSR_SS_XPU_SEC_INTR1_OFFS                                                                               (TCSR_TCSR_REGS_REG_BASE_OFFS + 0x00004004)
#define HWIO_TCSR_SS_XPU_SEC_INTR1_RMSK                                                                                  0x7ffff
#define HWIO_TCSR_SS_XPU_SEC_INTR1_IN          \
        in_dword_masked(HWIO_TCSR_SS_XPU_SEC_INTR1_ADDR, HWIO_TCSR_SS_XPU_SEC_INTR1_RMSK)
#define HWIO_TCSR_SS_XPU_SEC_INTR1_INM(m)      \
        in_dword_masked(HWIO_TCSR_SS_XPU_SEC_INTR1_ADDR, m)

#define HWIO_TCSR_SS_VMIDMT_CLIENT_SEC_INTR_ADDR                                                                      (TCSR_TCSR_REGS_REG_BASE      + 0x00004010)
#define HWIO_TCSR_SS_VMIDMT_CLIENT_SEC_INTR_OFFS                                                                      (TCSR_TCSR_REGS_REG_BASE_OFFS + 0x00004010)
#define HWIO_TCSR_SS_VMIDMT_CLIENT_SEC_INTR_RMSK                                                                         0x3ffff
#define HWIO_TCSR_SS_VMIDMT_CLIENT_SEC_INTR_IN          \
        in_dword_masked(HWIO_TCSR_SS_VMIDMT_CLIENT_SEC_INTR_ADDR, HWIO_TCSR_SS_VMIDMT_CLIENT_SEC_INTR_RMSK)
#define HWIO_TCSR_SS_VMIDMT_CLIENT_SEC_INTR_INM(m)      \
        in_dword_masked(HWIO_TCSR_SS_VMIDMT_CLIENT_SEC_INTR_ADDR, m)
#define HWIO_TCSR_SS_VMIDMT_CLIENT_SEC_INTR_QDSS_DAP_VMIDMT_GIRPT_BMSK                                                      0x80
#define HWIO_TCSR_SS_VMIDMT_CLIENT_SEC_INTR_QDSS_DAP_VMIDMT_GIRPT_SHFT                                                       0x7
#define HWIO_TCSR_SS_VMIDMT_CLIENT_SEC_INTR_QDSS_TRACE_VMIDMT_GIRPT_BMSK                                                    0x40
#define HWIO_TCSR_SS_VMIDMT_CLIENT_SEC_INTR_QDSS_TRACE_VMIDMT_GIRPT_SHFT                                                     0x6
#define HWIO_TCSR_SS_VMIDMT_CLIENT_SEC_INTR_IPA_VMIDMT_GIRPT_BMSK                                                           0x20
#define HWIO_TCSR_SS_VMIDMT_CLIENT_SEC_INTR_IPA_VMIDMT_GIRPT_SHFT                                                            0x5
#define HWIO_TCSR_SS_VMIDMT_CLIENT_SEC_INTR_SPMI_BAM_VMIDMT_GIRPT_BMSK                                                      0x10
#define HWIO_TCSR_SS_VMIDMT_CLIENT_SEC_INTR_SPMI_BAM_VMIDMT_GIRPT_SHFT                                                       0x4
#define HWIO_TCSR_SS_VMIDMT_CLIENT_SEC_INTR_BAM_EDML_VMIDMT_GIRPT_BMSK                                                       0x8
#define HWIO_TCSR_SS_VMIDMT_CLIENT_SEC_INTR_BAM_EDML_VMIDMT_GIRPT_SHFT                                                       0x3
#define HWIO_TCSR_SS_VMIDMT_CLIENT_SEC_INTR_QPIC_BAM_VMIDMT_GIRPT_BMSK                                                       0x4
#define HWIO_TCSR_SS_VMIDMT_CLIENT_SEC_INTR_QPIC_BAM_VMIDMT_GIRPT_SHFT                                                       0x2
#define HWIO_TCSR_SS_VMIDMT_CLIENT_SEC_INTR_AOP_VMIDMT_GIRPT_BMSK                                                            0x2
#define HWIO_TCSR_SS_VMIDMT_CLIENT_SEC_INTR_AOP_VMIDMT_GIRPT_SHFT                                                            0x1
#define HWIO_TCSR_SS_VMIDMT_CLIENT_SEC_INTR_CRYPTO_VMIDMT_GIRPT_BMSK                                                         0x1
#define HWIO_TCSR_SS_VMIDMT_CLIENT_SEC_INTR_CRYPTO_VMIDMT_GIRPT_SHFT                                                         0x0

#define HWIO_TCSR_SS_MMU_CLIENT_SEC_INTR_ADDR                                                                         (TCSR_TCSR_REGS_REG_BASE      + 0x00004014)
#define HWIO_TCSR_SS_MMU_CLIENT_SEC_INTR_OFFS                                                                         (TCSR_TCSR_REGS_REG_BASE_OFFS + 0x00004014)
#define HWIO_TCSR_SS_MMU_CLIENT_SEC_INTR_RMSK                                                                             0x1e42
#define HWIO_TCSR_SS_MMU_CLIENT_SEC_INTR_IN          \
        in_dword_masked(HWIO_TCSR_SS_MMU_CLIENT_SEC_INTR_ADDR, HWIO_TCSR_SS_MMU_CLIENT_SEC_INTR_RMSK)
#define HWIO_TCSR_SS_MMU_CLIENT_SEC_INTR_INM(m)      \
        in_dword_masked(HWIO_TCSR_SS_MMU_CLIENT_SEC_INTR_ADDR, m)
#define HWIO_TCSR_SS_MMU_CLIENT_SEC_INTR_LPASS_7_IRQ_OUT_SECURITY_BMSK                                                    0x1000
#define HWIO_TCSR_SS_MMU_CLIENT_SEC_INTR_LPASS_7_IRQ_OUT_SECURITY_SHFT                                                       0xc
#define HWIO_TCSR_SS_MMU_CLIENT_SEC_INTR_LPASS_15_IRQ_OUT_SECURITY_BMSK                                                    0x800
#define HWIO_TCSR_SS_MMU_CLIENT_SEC_INTR_LPASS_15_IRQ_OUT_SECURITY_SHFT                                                      0xb
#define HWIO_TCSR_SS_MMU_CLIENT_SEC_INTR_AGGR2_NOC_SMMU_MMU_GIRPT_BMSK                                                     0x400
#define HWIO_TCSR_SS_MMU_CLIENT_SEC_INTR_AGGR2_NOC_SMMU_MMU_GIRPT_SHFT                                                       0xa
#define HWIO_TCSR_SS_MMU_CLIENT_SEC_INTR_AGGR1_NOC_SMMU_MMU_GIRPT_BMSK                                                     0x200
#define HWIO_TCSR_SS_MMU_CLIENT_SEC_INTR_AGGR1_NOC_SMMU_MMU_GIRPT_SHFT                                                       0x9
#define HWIO_TCSR_SS_MMU_CLIENT_SEC_INTR_OGPU_MMU_GIRPT_BMSK                                                                0x40
#define HWIO_TCSR_SS_MMU_CLIENT_SEC_INTR_OGPU_MMU_GIRPT_SHFT                                                                 0x6
#define HWIO_TCSR_SS_MMU_CLIENT_SEC_INTR_MMSS_BIMC_SMMU_GIRPT_BMSK                                                           0x2
#define HWIO_TCSR_SS_MMU_CLIENT_SEC_INTR_MMSS_BIMC_SMMU_GIRPT_SHFT                                                           0x1

#define HWIO_TCSR_SS_XPU_SEC_INTR0_ENABLE_ADDR                                                                        (TCSR_TCSR_REGS_REG_BASE      + 0x00004040)
#define HWIO_TCSR_SS_XPU_SEC_INTR0_ENABLE_OFFS                                                                        (TCSR_TCSR_REGS_REG_BASE_OFFS + 0x00004040)
#define HWIO_TCSR_SS_XPU_SEC_INTR0_ENABLE_RMSK                                                                        0xffffffff
#define HWIO_TCSR_SS_XPU_SEC_INTR0_ENABLE_IN          \
        in_dword_masked(HWIO_TCSR_SS_XPU_SEC_INTR0_ENABLE_ADDR, HWIO_TCSR_SS_XPU_SEC_INTR0_ENABLE_RMSK)
#define HWIO_TCSR_SS_XPU_SEC_INTR0_ENABLE_INM(m)      \
        in_dword_masked(HWIO_TCSR_SS_XPU_SEC_INTR0_ENABLE_ADDR, m)
#define HWIO_TCSR_SS_XPU_SEC_INTR0_ENABLE_OUT(v)      \
        out_dword(HWIO_TCSR_SS_XPU_SEC_INTR0_ENABLE_ADDR,v)
#define HWIO_TCSR_SS_XPU_SEC_INTR0_ENABLE_OUTM(m,v) \
        out_dword_masked_ns(HWIO_TCSR_SS_XPU_SEC_INTR0_ENABLE_ADDR,m,v,HWIO_TCSR_SS_XPU_SEC_INTR0_ENABLE_IN)
#define HWIO_TCSR_SS_XPU_SEC_INTR0_ENABLE_QM_XPU3_SEC_INTR0_ENABLE_BMSK                                               0x40000000
#define HWIO_TCSR_SS_XPU_SEC_INTR0_ENABLE_QM_XPU3_SEC_INTR0_ENABLE_SHFT                                                     0x1e
#define HWIO_TCSR_SS_XPU_SEC_INTR0_ENABLE_MEMNOC_MCDMA_XPU3_SEC_INTR0_ENABLE_BMSK                                     0x20000000
#define HWIO_TCSR_SS_XPU_SEC_INTR0_ENABLE_MEMNOC_MCDMA_XPU3_SEC_INTR0_ENABLE_SHFT                                           0x1d
#define HWIO_TCSR_SS_XPU_SEC_INTR0_ENABLE_SPMI_BAM_APU_SEC_INTR0_ENABLE_BMSK                                          0x10000000
#define HWIO_TCSR_SS_XPU_SEC_INTR0_ENABLE_SPMI_BAM_APU_SEC_INTR0_ENABLE_SHFT                                                0x1c
#define HWIO_TCSR_SS_XPU_SEC_INTR0_ENABLE_IPA_APU_SEC_INTR0_ENABLE_BMSK                                                0x8000000
#define HWIO_TCSR_SS_XPU_SEC_INTR0_ENABLE_IPA_APU_SEC_INTR0_ENABLE_SHFT                                                     0x1b
#define HWIO_TCSR_SS_XPU_SEC_INTR0_ENABLE_BLSP_BAM_APU_SEC_INTR0_ENABLE_BMSK                                           0x4000000
#define HWIO_TCSR_SS_XPU_SEC_INTR0_ENABLE_BLSP_BAM_APU_SEC_INTR0_ENABLE_SHFT                                                0x1a
#define HWIO_TCSR_SS_XPU_SEC_INTR0_ENABLE_PKA_XPU_SEC_INTR0_ENABLE_BMSK                                                0x2000000
#define HWIO_TCSR_SS_XPU_SEC_INTR0_ENABLE_PKA_XPU_SEC_INTR0_ENABLE_SHFT                                                     0x19
#define HWIO_TCSR_SS_XPU_SEC_INTR0_ENABLE_QPIC_XPU2_SEC_INTR0_ENABLE_BMSK                                              0x1000000
#define HWIO_TCSR_SS_XPU_SEC_INTR0_ENABLE_QPIC_XPU2_SEC_INTR0_ENABLE_SHFT                                                   0x18
#define HWIO_TCSR_SS_XPU_SEC_INTR0_ENABLE_QXS_Q6_TCM_SS_MPU_XPU3_SEC_INTR0_ENABLE_BMSK                                  0x400000
#define HWIO_TCSR_SS_XPU_SEC_INTR0_ENABLE_QXS_Q6_TCM_SS_MPU_XPU3_SEC_INTR0_ENABLE_SHFT                                      0x16
#define HWIO_TCSR_SS_XPU_SEC_INTR0_ENABLE_QXM_MSS_NAV_CE_MS_MPU_XPU3_SEC_INTR0_ENABLE_BMSK                              0x200000
#define HWIO_TCSR_SS_XPU_SEC_INTR0_ENABLE_QXM_MSS_NAV_CE_MS_MPU_XPU3_SEC_INTR0_ENABLE_SHFT                                  0x15
#define HWIO_TCSR_SS_XPU_SEC_INTR0_ENABLE_QNM_MEMNOC_MS_MPU_XPU3_SEC_INTR0_ENABLE_BMSK                                  0x100000
#define HWIO_TCSR_SS_XPU_SEC_INTR0_ENABLE_QNM_MEMNOC_MS_MPU_XPU3_SEC_INTR0_ENABLE_SHFT                                      0x14
#define HWIO_TCSR_SS_XPU_SEC_INTR0_ENABLE_QNM_AGGRE_NOC_IPA_MS_MPU_XPU3_SEC_INTR0_ENABLE_BMSK                            0x80000
#define HWIO_TCSR_SS_XPU_SEC_INTR0_ENABLE_QNM_AGGRE_NOC_IPA_MS_MPU_XPU3_SEC_INTR0_ENABLE_SHFT                               0x13
#define HWIO_TCSR_SS_XPU_SEC_INTR0_ENABLE_QHS_SNOC_CFG_SS_MPU_XPU3_SEC_INTR0_ENABLE_BMSK                                 0x40000
#define HWIO_TCSR_SS_XPU_SEC_INTR0_ENABLE_QHS_SNOC_CFG_SS_MPU_XPU3_SEC_INTR0_ENABLE_SHFT                                    0x12
#define HWIO_TCSR_SS_XPU_SEC_INTR0_ENABLE_TLMM_CENTRAL_XPU_SEC_INTR0_ENABLE_BMSK                                         0x20000
#define HWIO_TCSR_SS_XPU_SEC_INTR0_ENABLE_TLMM_CENTRAL_XPU_SEC_INTR0_ENABLE_SHFT                                            0x11
#define HWIO_TCSR_SS_XPU_SEC_INTR0_ENABLE_SPDM_WRAPPER_XPU_SEC_INTR0_ENABLE_BMSK                                         0x10000
#define HWIO_TCSR_SS_XPU_SEC_INTR0_ENABLE_SPDM_WRAPPER_XPU_SEC_INTR0_ENABLE_SHFT                                            0x10
#define HWIO_TCSR_SS_XPU_SEC_INTR0_ENABLE_OCIMEM_MPU_SEC_INTR0_ENABLE_BMSK                                                0x8000
#define HWIO_TCSR_SS_XPU_SEC_INTR0_ENABLE_OCIMEM_MPU_SEC_INTR0_ENABLE_SHFT                                                   0xf
#define HWIO_TCSR_SS_XPU_SEC_INTR0_ENABLE_DTP_QHS_NON_BROADCAST_MPU_MAINO_XPU3_SEC_INTR0_ENABLE_BMSK                      0x4000
#define HWIO_TCSR_SS_XPU_SEC_INTR0_ENABLE_DTP_QHS_NON_BROADCAST_MPU_MAINO_XPU3_SEC_INTR0_ENABLE_SHFT                         0xe
#define HWIO_TCSR_SS_XPU_SEC_INTR0_ENABLE_BOOT_ROM_SEC_INTR0_ENABLE_BMSK                                                  0x2000
#define HWIO_TCSR_SS_XPU_SEC_INTR0_ENABLE_BOOT_ROM_SEC_INTR0_ENABLE_SHFT                                                     0xd
#define HWIO_TCSR_SS_XPU_SEC_INTR0_ENABLE_DTP_QHS_BROADCAST_MPU_MAINO_XPU3_SEC_INTR0_ENABLE_BMSK                          0x1000
#define HWIO_TCSR_SS_XPU_SEC_INTR0_ENABLE_DTP_QHS_BROADCAST_MPU_MAINO_XPU3_SEC_INTR0_ENABLE_SHFT                             0xc
#define HWIO_TCSR_SS_XPU_SEC_INTR0_ENABLE_LLCC0_XPU3_SEC_INTR0_ENABLE_BMSK                                                 0x800
#define HWIO_TCSR_SS_XPU_SEC_INTR0_ENABLE_LLCC0_XPU3_SEC_INTR0_ENABLE_SHFT                                                   0xb
#define HWIO_TCSR_SS_XPU_SEC_INTR0_ENABLE_GCC_XPU_SEC_INTR0_ENABLE_BMSK                                                    0x400
#define HWIO_TCSR_SS_XPU_SEC_INTR0_ENABLE_GCC_XPU_SEC_INTR0_ENABLE_SHFT                                                      0xa
#define HWIO_TCSR_SS_XPU_SEC_INTR0_ENABLE_MEMNOC_XPU3_SEC_INTR0_ENABLE_BMSK                                                0x200
#define HWIO_TCSR_SS_XPU_SEC_INTR0_ENABLE_MEMNOC_XPU3_SEC_INTR0_ENABLE_SHFT                                                  0x9
#define HWIO_TCSR_SS_XPU_SEC_INTR0_ENABLE_QHM_AOSS_MS_MPU_XPU3_SEC_INTR0_ENABLE_BMSK                                       0x100
#define HWIO_TCSR_SS_XPU_SEC_INTR0_ENABLE_QHM_AOSS_MS_MPU_XPU3_SEC_INTR0_ENABLE_SHFT                                         0x8
#define HWIO_TCSR_SS_XPU_SEC_INTR0_ENABLE_SEC_CTRL_XPU3_SEC_INTR0_ENABLE_BMSK                                               0x80
#define HWIO_TCSR_SS_XPU_SEC_INTR0_ENABLE_SEC_CTRL_XPU3_SEC_INTR0_ENABLE_SHFT                                                0x7
#define HWIO_TCSR_SS_XPU_SEC_INTR0_ENABLE_DCC_XPU_SEC_INTR0_ENABLE_BMSK                                                     0x40
#define HWIO_TCSR_SS_XPU_SEC_INTR0_ENABLE_DCC_XPU_SEC_INTR0_ENABLE_SHFT                                                      0x6
#define HWIO_TCSR_SS_XPU_SEC_INTR0_ENABLE_CRYPTO_BAM_APU_SEC_INTR0_ENABLE_BMSK                                              0x10
#define HWIO_TCSR_SS_XPU_SEC_INTR0_ENABLE_CRYPTO_BAM_APU_SEC_INTR0_ENABLE_SHFT                                               0x4
#define HWIO_TCSR_SS_XPU_SEC_INTR0_ENABLE_TCSR_MUTEX_XPU_SEC_INTR0_ENABLE_BMSK                                               0x8
#define HWIO_TCSR_SS_XPU_SEC_INTR0_ENABLE_TCSR_MUTEX_XPU_SEC_INTR0_ENABLE_SHFT                                               0x3
#define HWIO_TCSR_SS_XPU_SEC_INTR0_ENABLE_TCSR_REGS_XPU_SEC_INTR0_ENABLE_BMSK                                                0x4
#define HWIO_TCSR_SS_XPU_SEC_INTR0_ENABLE_TCSR_REGS_XPU_SEC_INTR0_ENABLE_SHFT                                                0x2
#define HWIO_TCSR_SS_XPU_SEC_INTR0_ENABLE_AOSS_MPU_SEC_INTR0_ENABLE_BMSK                                                     0x2
#define HWIO_TCSR_SS_XPU_SEC_INTR0_ENABLE_AOSS_MPU_SEC_INTR0_ENABLE_SHFT                                                     0x1
#define HWIO_TCSR_SS_XPU_SEC_INTR0_ENABLE_PMIC_ARB_MPU_SEC_INTR0_ENABLE_BMSK                                                 0x1
#define HWIO_TCSR_SS_XPU_SEC_INTR0_ENABLE_PMIC_ARB_MPU_SEC_INTR0_ENABLE_SHFT                                                 0x0

#define HWIO_TCSR_SS_XPU_SEC_INTR1_ENABLE_ADDR                                                                        (TCSR_TCSR_REGS_REG_BASE      + 0x00004044)
#define HWIO_TCSR_SS_XPU_SEC_INTR1_ENABLE_OFFS                                                                        (TCSR_TCSR_REGS_REG_BASE_OFFS + 0x00004044)
#define HWIO_TCSR_SS_XPU_SEC_INTR1_ENABLE_RMSK                                                                           0x7ffff
#define HWIO_TCSR_SS_XPU_SEC_INTR1_ENABLE_IN          \
        in_dword_masked(HWIO_TCSR_SS_XPU_SEC_INTR1_ENABLE_ADDR, HWIO_TCSR_SS_XPU_SEC_INTR1_ENABLE_RMSK)
#define HWIO_TCSR_SS_XPU_SEC_INTR1_ENABLE_INM(m)      \
        in_dword_masked(HWIO_TCSR_SS_XPU_SEC_INTR1_ENABLE_ADDR, m)
#define HWIO_TCSR_SS_XPU_SEC_INTR1_ENABLE_OUT(v)      \
        out_dword(HWIO_TCSR_SS_XPU_SEC_INTR1_ENABLE_ADDR,v)
#define HWIO_TCSR_SS_XPU_SEC_INTR1_ENABLE_OUTM(m,v) \
        out_dword_masked_ns(HWIO_TCSR_SS_XPU_SEC_INTR1_ENABLE_ADDR,m,v,HWIO_TCSR_SS_XPU_SEC_INTR1_ENABLE_IN)

#define HWIO_TCSR_SS_VMIDMT_CLIENT_SEC_INTR_ENABLE_ADDR                                                               (TCSR_TCSR_REGS_REG_BASE      + 0x00004050)
#define HWIO_TCSR_SS_VMIDMT_CLIENT_SEC_INTR_ENABLE_OFFS                                                               (TCSR_TCSR_REGS_REG_BASE_OFFS + 0x00004050)
#define HWIO_TCSR_SS_VMIDMT_CLIENT_SEC_INTR_ENABLE_RMSK                                                                  0x3ffff
#define HWIO_TCSR_SS_VMIDMT_CLIENT_SEC_INTR_ENABLE_IN          \
        in_dword_masked(HWIO_TCSR_SS_VMIDMT_CLIENT_SEC_INTR_ENABLE_ADDR, HWIO_TCSR_SS_VMIDMT_CLIENT_SEC_INTR_ENABLE_RMSK)
#define HWIO_TCSR_SS_VMIDMT_CLIENT_SEC_INTR_ENABLE_INM(m)      \
        in_dword_masked(HWIO_TCSR_SS_VMIDMT_CLIENT_SEC_INTR_ENABLE_ADDR, m)
#define HWIO_TCSR_SS_VMIDMT_CLIENT_SEC_INTR_ENABLE_OUT(v)      \
        out_dword(HWIO_TCSR_SS_VMIDMT_CLIENT_SEC_INTR_ENABLE_ADDR,v)
#define HWIO_TCSR_SS_VMIDMT_CLIENT_SEC_INTR_ENABLE_OUTM(m,v) \
        out_dword_masked_ns(HWIO_TCSR_SS_VMIDMT_CLIENT_SEC_INTR_ENABLE_ADDR,m,v,HWIO_TCSR_SS_VMIDMT_CLIENT_SEC_INTR_ENABLE_IN)
#define HWIO_TCSR_SS_VMIDMT_CLIENT_SEC_INTR_ENABLE_QDSS_DAP_VMIDMT_GIRPT_ENABLE_BMSK                                        0x80
#define HWIO_TCSR_SS_VMIDMT_CLIENT_SEC_INTR_ENABLE_QDSS_DAP_VMIDMT_GIRPT_ENABLE_SHFT                                         0x7
#define HWIO_TCSR_SS_VMIDMT_CLIENT_SEC_INTR_ENABLE_QDSS_TRACE_VMIDMT_GIRPT_ENABLE_BMSK                                      0x40
#define HWIO_TCSR_SS_VMIDMT_CLIENT_SEC_INTR_ENABLE_QDSS_TRACE_VMIDMT_GIRPT_ENABLE_SHFT                                       0x6
#define HWIO_TCSR_SS_VMIDMT_CLIENT_SEC_INTR_ENABLE_IPA_VMIDMT_GIRPT_ENABLE_BMSK                                             0x20
#define HWIO_TCSR_SS_VMIDMT_CLIENT_SEC_INTR_ENABLE_IPA_VMIDMT_GIRPT_ENABLE_SHFT                                              0x5
#define HWIO_TCSR_SS_VMIDMT_CLIENT_SEC_INTR_ENABLE_SPMI_BAM_VMIDMT_GIRPT_ENABLE_BMSK                                        0x10
#define HWIO_TCSR_SS_VMIDMT_CLIENT_SEC_INTR_ENABLE_SPMI_BAM_VMIDMT_GIRPT_ENABLE_SHFT                                         0x4
#define HWIO_TCSR_SS_VMIDMT_CLIENT_SEC_INTR_ENABLE_BAM_EDML_VMIDMT_GIRPT_ENABLE_BMSK                                         0x8
#define HWIO_TCSR_SS_VMIDMT_CLIENT_SEC_INTR_ENABLE_BAM_EDML_VMIDMT_GIRPT_ENABLE_SHFT                                         0x3
#define HWIO_TCSR_SS_VMIDMT_CLIENT_SEC_INTR_ENABLE_QPIC_BAM_VMIDMT_GIRPT_ENABLE_BMSK                                         0x4
#define HWIO_TCSR_SS_VMIDMT_CLIENT_SEC_INTR_ENABLE_QPIC_BAM_VMIDMT_GIRPT_ENABLE_SHFT                                         0x2
#define HWIO_TCSR_SS_VMIDMT_CLIENT_SEC_INTR_ENABLE_AOP_VMIDMT_GIRPT_ENABLE_BMSK                                              0x2
#define HWIO_TCSR_SS_VMIDMT_CLIENT_SEC_INTR_ENABLE_AOP_VMIDMT_GIRPT_ENABLE_SHFT                                              0x1
#define HWIO_TCSR_SS_VMIDMT_CLIENT_SEC_INTR_ENABLE_CRYPTO_VMIDMT_GIRPT_ENABLE_BMSK                                           0x1
#define HWIO_TCSR_SS_VMIDMT_CLIENT_SEC_INTR_ENABLE_CRYPTO_VMIDMT_GIRPT_ENABLE_SHFT                                           0x0

#define HWIO_TCSR_SS_MMU_CLIENT_SEC_INTR_ENABLE_ADDR                                                                  (TCSR_TCSR_REGS_REG_BASE      + 0x00004054)
#define HWIO_TCSR_SS_MMU_CLIENT_SEC_INTR_ENABLE_OFFS                                                                  (TCSR_TCSR_REGS_REG_BASE_OFFS + 0x00004054)
#define HWIO_TCSR_SS_MMU_CLIENT_SEC_INTR_ENABLE_RMSK                                                                      0x1e42
#define HWIO_TCSR_SS_MMU_CLIENT_SEC_INTR_ENABLE_IN          \
        in_dword_masked(HWIO_TCSR_SS_MMU_CLIENT_SEC_INTR_ENABLE_ADDR, HWIO_TCSR_SS_MMU_CLIENT_SEC_INTR_ENABLE_RMSK)
#define HWIO_TCSR_SS_MMU_CLIENT_SEC_INTR_ENABLE_INM(m)      \
        in_dword_masked(HWIO_TCSR_SS_MMU_CLIENT_SEC_INTR_ENABLE_ADDR, m)
#define HWIO_TCSR_SS_MMU_CLIENT_SEC_INTR_ENABLE_OUT(v)      \
        out_dword(HWIO_TCSR_SS_MMU_CLIENT_SEC_INTR_ENABLE_ADDR,v)
#define HWIO_TCSR_SS_MMU_CLIENT_SEC_INTR_ENABLE_OUTM(m,v) \
        out_dword_masked_ns(HWIO_TCSR_SS_MMU_CLIENT_SEC_INTR_ENABLE_ADDR,m,v,HWIO_TCSR_SS_MMU_CLIENT_SEC_INTR_ENABLE_IN)
#define HWIO_TCSR_SS_MMU_CLIENT_SEC_INTR_ENABLE_LPASS_7_IRQ_OUT_SECURITY_INTR_ENABLE_BMSK                                 0x1000
#define HWIO_TCSR_SS_MMU_CLIENT_SEC_INTR_ENABLE_LPASS_7_IRQ_OUT_SECURITY_INTR_ENABLE_SHFT                                    0xc
#define HWIO_TCSR_SS_MMU_CLIENT_SEC_INTR_ENABLE_LPASS_15_IRQ_OUT_SECURITY_INTR_ENABLE_BMSK                                 0x800
#define HWIO_TCSR_SS_MMU_CLIENT_SEC_INTR_ENABLE_LPASS_15_IRQ_OUT_SECURITY_INTR_ENABLE_SHFT                                   0xb
#define HWIO_TCSR_SS_MMU_CLIENT_SEC_INTR_ENABLE_AGGR2_NOC_SMMU_MMU_GIRPT_ENABLE_BMSK                                       0x400
#define HWIO_TCSR_SS_MMU_CLIENT_SEC_INTR_ENABLE_AGGR2_NOC_SMMU_MMU_GIRPT_ENABLE_SHFT                                         0xa
#define HWIO_TCSR_SS_MMU_CLIENT_SEC_INTR_ENABLE_AGGR1_NOC_SMMU_MMU_GIRPT_ENABLE_BMSK                                       0x200
#define HWIO_TCSR_SS_MMU_CLIENT_SEC_INTR_ENABLE_AGGR1_NOC_SMMU_MMU_GIRPT_ENABLE_SHFT                                         0x9
#define HWIO_TCSR_SS_MMU_CLIENT_SEC_INTR_ENABLE_OGPU_MMU_GIRPT_ENABLE_BMSK                                                  0x40
#define HWIO_TCSR_SS_MMU_CLIENT_SEC_INTR_ENABLE_OGPU_MMU_GIRPT_ENABLE_SHFT                                                   0x6
#define HWIO_TCSR_SS_MMU_CLIENT_SEC_INTR_ENABLE_MMSS_BIMC_SMMU_GIRPT_ENABLE_BMSK                                             0x2
#define HWIO_TCSR_SS_MMU_CLIENT_SEC_INTR_ENABLE_MMSS_BIMC_SMMU_GIRPT_ENABLE_SHFT                                             0x1

#define HWIO_TCSR_SS_VMIDMT_CFG_SEC_INTR_ADDR                                                                         (TCSR_TCSR_REGS_REG_BASE      + 0x00005000)
#define HWIO_TCSR_SS_VMIDMT_CFG_SEC_INTR_OFFS                                                                         (TCSR_TCSR_REGS_REG_BASE_OFFS + 0x00005000)
#define HWIO_TCSR_SS_VMIDMT_CFG_SEC_INTR_RMSK                                                                            0x3ffff
#define HWIO_TCSR_SS_VMIDMT_CFG_SEC_INTR_IN          \
        in_dword_masked(HWIO_TCSR_SS_VMIDMT_CFG_SEC_INTR_ADDR, HWIO_TCSR_SS_VMIDMT_CFG_SEC_INTR_RMSK)
#define HWIO_TCSR_SS_VMIDMT_CFG_SEC_INTR_INM(m)      \
        in_dword_masked(HWIO_TCSR_SS_VMIDMT_CFG_SEC_INTR_ADDR, m)
#define HWIO_TCSR_SS_VMIDMT_CFG_SEC_INTR_QDSS_DAP_VMIDMT_GCFGIRPT_BMSK                                                      0x80
#define HWIO_TCSR_SS_VMIDMT_CFG_SEC_INTR_QDSS_DAP_VMIDMT_GCFGIRPT_SHFT                                                       0x7
#define HWIO_TCSR_SS_VMIDMT_CFG_SEC_INTR_QDSS_TRACE_VMIDMT_GCFGIRPT_BMSK                                                    0x40
#define HWIO_TCSR_SS_VMIDMT_CFG_SEC_INTR_QDSS_TRACE_VMIDMT_GCFGIRPT_SHFT                                                     0x6
#define HWIO_TCSR_SS_VMIDMT_CFG_SEC_INTR_IPA_VMIDMT_GCFGIRPT_BMSK                                                           0x20
#define HWIO_TCSR_SS_VMIDMT_CFG_SEC_INTR_IPA_VMIDMT_GCFGIRPT_SHFT                                                            0x5
#define HWIO_TCSR_SS_VMIDMT_CFG_SEC_INTR_SPMI_BAM_VMIDMT_GCFGIRPT_BMSK                                                      0x10
#define HWIO_TCSR_SS_VMIDMT_CFG_SEC_INTR_SPMI_BAM_VMIDMT_GCFGIRPT_SHFT                                                       0x4
#define HWIO_TCSR_SS_VMIDMT_CFG_SEC_INTR_BAM_EDML_VMIDMT_GCFGIRPT_BMSK                                                       0x8
#define HWIO_TCSR_SS_VMIDMT_CFG_SEC_INTR_BAM_EDML_VMIDMT_GCFGIRPT_SHFT                                                       0x3
#define HWIO_TCSR_SS_VMIDMT_CFG_SEC_INTR_QPIC_BAM_VMIDMT_GCFGIRPT_BMSK                                                       0x4
#define HWIO_TCSR_SS_VMIDMT_CFG_SEC_INTR_QPIC_BAM_VMIDMT_GCFGIRPT_SHFT                                                       0x2
#define HWIO_TCSR_SS_VMIDMT_CFG_SEC_INTR_AOP_VMIDMT_GCFGIRPT_BMSK                                                            0x2
#define HWIO_TCSR_SS_VMIDMT_CFG_SEC_INTR_AOP_VMIDMT_GCFGIRPT_SHFT                                                            0x1
#define HWIO_TCSR_SS_VMIDMT_CFG_SEC_INTR_CRYPTO_VMIDMT_GCFGIRPT_BMSK                                                         0x1
#define HWIO_TCSR_SS_VMIDMT_CFG_SEC_INTR_CRYPTO_VMIDMT_GCFGIRPT_SHFT                                                         0x0

#define HWIO_TCSR_SS_MMU_CFG_SEC_INTR_ADDR                                                                            (TCSR_TCSR_REGS_REG_BASE      + 0x00005004)
#define HWIO_TCSR_SS_MMU_CFG_SEC_INTR_OFFS                                                                            (TCSR_TCSR_REGS_REG_BASE_OFFS + 0x00005004)
#define HWIO_TCSR_SS_MMU_CFG_SEC_INTR_RMSK                                                                                0x1e42
#define HWIO_TCSR_SS_MMU_CFG_SEC_INTR_IN          \
        in_dword_masked(HWIO_TCSR_SS_MMU_CFG_SEC_INTR_ADDR, HWIO_TCSR_SS_MMU_CFG_SEC_INTR_RMSK)
#define HWIO_TCSR_SS_MMU_CFG_SEC_INTR_INM(m)      \
        in_dword_masked(HWIO_TCSR_SS_MMU_CFG_SEC_INTR_ADDR, m)
#define HWIO_TCSR_SS_MMU_CFG_SEC_INTR_LPASS_6_IRQ_OUT_SECURITY_BMSK                                                       0x1000
#define HWIO_TCSR_SS_MMU_CFG_SEC_INTR_LPASS_6_IRQ_OUT_SECURITY_SHFT                                                          0xc
#define HWIO_TCSR_SS_MMU_CFG_SEC_INTR_LPASS_14_IRQ_OUT_SECURITY_BMSK                                                       0x800
#define HWIO_TCSR_SS_MMU_CFG_SEC_INTR_LPASS_14_IRQ_OUT_SECURITY_SHFT                                                         0xb
#define HWIO_TCSR_SS_MMU_CFG_SEC_INTR_AGGR2_NOC_SMMU_MMU_GCFGIRPT_BMSK                                                     0x400
#define HWIO_TCSR_SS_MMU_CFG_SEC_INTR_AGGR2_NOC_SMMU_MMU_GCFGIRPT_SHFT                                                       0xa
#define HWIO_TCSR_SS_MMU_CFG_SEC_INTR_AGGR1_NOC_SMMU_MMU_GCFGIRPT_BMSK                                                     0x200
#define HWIO_TCSR_SS_MMU_CFG_SEC_INTR_AGGR1_NOC_SMMU_MMU_GCFGIRPT_SHFT                                                       0x9
#define HWIO_TCSR_SS_MMU_CFG_SEC_INTR_OGPU_MMU_GCFGIRPT_BMSK                                                                0x40
#define HWIO_TCSR_SS_MMU_CFG_SEC_INTR_OGPU_MMU_GCFGIRPT_SHFT                                                                 0x6
#define HWIO_TCSR_SS_MMU_CFG_SEC_INTR_MMSS_BIMC_SMMU_GCFGIRPT_BMSK                                                           0x2
#define HWIO_TCSR_SS_MMU_CFG_SEC_INTR_MMSS_BIMC_SMMU_GCFGIRPT_SHFT                                                           0x1

#define HWIO_TCSR_SS_VMIDMT_CFG_SEC_INTR_ENABLE_ADDR                                                                  (TCSR_TCSR_REGS_REG_BASE      + 0x00005040)
#define HWIO_TCSR_SS_VMIDMT_CFG_SEC_INTR_ENABLE_OFFS                                                                  (TCSR_TCSR_REGS_REG_BASE_OFFS + 0x00005040)
#define HWIO_TCSR_SS_VMIDMT_CFG_SEC_INTR_ENABLE_RMSK                                                                     0x3ffff
#define HWIO_TCSR_SS_VMIDMT_CFG_SEC_INTR_ENABLE_IN          \
        in_dword_masked(HWIO_TCSR_SS_VMIDMT_CFG_SEC_INTR_ENABLE_ADDR, HWIO_TCSR_SS_VMIDMT_CFG_SEC_INTR_ENABLE_RMSK)
#define HWIO_TCSR_SS_VMIDMT_CFG_SEC_INTR_ENABLE_INM(m)      \
        in_dword_masked(HWIO_TCSR_SS_VMIDMT_CFG_SEC_INTR_ENABLE_ADDR, m)
#define HWIO_TCSR_SS_VMIDMT_CFG_SEC_INTR_ENABLE_OUT(v)      \
        out_dword(HWIO_TCSR_SS_VMIDMT_CFG_SEC_INTR_ENABLE_ADDR,v)
#define HWIO_TCSR_SS_VMIDMT_CFG_SEC_INTR_ENABLE_OUTM(m,v) \
        out_dword_masked_ns(HWIO_TCSR_SS_VMIDMT_CFG_SEC_INTR_ENABLE_ADDR,m,v,HWIO_TCSR_SS_VMIDMT_CFG_SEC_INTR_ENABLE_IN)
#define HWIO_TCSR_SS_VMIDMT_CFG_SEC_INTR_ENABLE_QDSS_DAP_VMIDMT_GCFGIRPT_ENABLE_BMSK                                        0x80
#define HWIO_TCSR_SS_VMIDMT_CFG_SEC_INTR_ENABLE_QDSS_DAP_VMIDMT_GCFGIRPT_ENABLE_SHFT                                         0x7
#define HWIO_TCSR_SS_VMIDMT_CFG_SEC_INTR_ENABLE_QDSS_TRACE_VMIDMT_GCFGIRPT_ENABLE_BMSK                                      0x40
#define HWIO_TCSR_SS_VMIDMT_CFG_SEC_INTR_ENABLE_QDSS_TRACE_VMIDMT_GCFGIRPT_ENABLE_SHFT                                       0x6
#define HWIO_TCSR_SS_VMIDMT_CFG_SEC_INTR_ENABLE_IPA_VMIDMT_GCFGIRPT_ENABLE_BMSK                                             0x20
#define HWIO_TCSR_SS_VMIDMT_CFG_SEC_INTR_ENABLE_IPA_VMIDMT_GCFGIRPT_ENABLE_SHFT                                              0x5
#define HWIO_TCSR_SS_VMIDMT_CFG_SEC_INTR_ENABLE_SPMI_BAM_VMIDMT_GCFGIRPT_ENABLE_BMSK                                        0x10
#define HWIO_TCSR_SS_VMIDMT_CFG_SEC_INTR_ENABLE_SPMI_BAM_VMIDMT_GCFGIRPT_ENABLE_SHFT                                         0x4
#define HWIO_TCSR_SS_VMIDMT_CFG_SEC_INTR_ENABLE_BAM_EDML_VMIDMT_GCFGIRPT_ENABLE_BMSK                                         0x8
#define HWIO_TCSR_SS_VMIDMT_CFG_SEC_INTR_ENABLE_BAM_EDML_VMIDMT_GCFGIRPT_ENABLE_SHFT                                         0x3
#define HWIO_TCSR_SS_VMIDMT_CFG_SEC_INTR_ENABLE_QPIC_BAM_VMIDMT_GCFGIRPT_ENABLE_BMSK                                         0x4
#define HWIO_TCSR_SS_VMIDMT_CFG_SEC_INTR_ENABLE_QPIC_BAM_VMIDMT_GCFGIRPT_ENABLE_SHFT                                         0x2
#define HWIO_TCSR_SS_VMIDMT_CFG_SEC_INTR_ENABLE_AOP_VMIDMT_GCFGIRPT_ENABLE_BMSK                                              0x2
#define HWIO_TCSR_SS_VMIDMT_CFG_SEC_INTR_ENABLE_AOP_VMIDMT_GCFGIRPT_ENABLE_SHFT                                              0x1
#define HWIO_TCSR_SS_VMIDMT_CFG_SEC_INTR_ENABLE_CRYPTO_VMIDMT_GCFGIRPT_ENABLE_BMSK                                           0x1
#define HWIO_TCSR_SS_VMIDMT_CFG_SEC_INTR_ENABLE_CRYPTO_VMIDMT_GCFGIRPT_ENABLE_SHFT                                           0x0

#define HWIO_TCSR_SS_MMU_CFG_SEC_INTR_ENABLE_ADDR                                                                     (TCSR_TCSR_REGS_REG_BASE      + 0x00005044)
#define HWIO_TCSR_SS_MMU_CFG_SEC_INTR_ENABLE_OFFS                                                                     (TCSR_TCSR_REGS_REG_BASE_OFFS + 0x00005044)
#define HWIO_TCSR_SS_MMU_CFG_SEC_INTR_ENABLE_RMSK                                                                         0x1e42
#define HWIO_TCSR_SS_MMU_CFG_SEC_INTR_ENABLE_IN          \
        in_dword_masked(HWIO_TCSR_SS_MMU_CFG_SEC_INTR_ENABLE_ADDR, HWIO_TCSR_SS_MMU_CFG_SEC_INTR_ENABLE_RMSK)
#define HWIO_TCSR_SS_MMU_CFG_SEC_INTR_ENABLE_INM(m)      \
        in_dword_masked(HWIO_TCSR_SS_MMU_CFG_SEC_INTR_ENABLE_ADDR, m)
#define HWIO_TCSR_SS_MMU_CFG_SEC_INTR_ENABLE_OUT(v)      \
        out_dword(HWIO_TCSR_SS_MMU_CFG_SEC_INTR_ENABLE_ADDR,v)
#define HWIO_TCSR_SS_MMU_CFG_SEC_INTR_ENABLE_OUTM(m,v) \
        out_dword_masked_ns(HWIO_TCSR_SS_MMU_CFG_SEC_INTR_ENABLE_ADDR,m,v,HWIO_TCSR_SS_MMU_CFG_SEC_INTR_ENABLE_IN)
#define HWIO_TCSR_SS_MMU_CFG_SEC_INTR_ENABLE_LPASS_6_IRQ_OUT_SECURITY_INTR_ENABLE_BMSK                                    0x1000
#define HWIO_TCSR_SS_MMU_CFG_SEC_INTR_ENABLE_LPASS_6_IRQ_OUT_SECURITY_INTR_ENABLE_SHFT                                       0xc
#define HWIO_TCSR_SS_MMU_CFG_SEC_INTR_ENABLE_LPASS_14_IRQ_OUT_SECURITY_INTR_ENABLE_BMSK                                    0x800
#define HWIO_TCSR_SS_MMU_CFG_SEC_INTR_ENABLE_LPASS_14_IRQ_OUT_SECURITY_INTR_ENABLE_SHFT                                      0xb
#define HWIO_TCSR_SS_MMU_CFG_SEC_INTR_ENABLE_AGGR2_NOC_SMMU_MMU_GCFGIRPT_ENABLE_BMSK                                       0x400
#define HWIO_TCSR_SS_MMU_CFG_SEC_INTR_ENABLE_AGGR2_NOC_SMMU_MMU_GCFGIRPT_ENABLE_SHFT                                         0xa
#define HWIO_TCSR_SS_MMU_CFG_SEC_INTR_ENABLE_AGGR1_NOC_SMMU_MMU_GCFGIRPT_ENABLE_BMSK                                       0x200
#define HWIO_TCSR_SS_MMU_CFG_SEC_INTR_ENABLE_AGGR1_NOC_SMMU_MMU_GCFGIRPT_ENABLE_SHFT                                         0x9
#define HWIO_TCSR_SS_MMU_CFG_SEC_INTR_ENABLE_OGPU_MMU_GCFGIRPT_ENABLE_BMSK                                                  0x40
#define HWIO_TCSR_SS_MMU_CFG_SEC_INTR_ENABLE_OGPU_MMU_GCFGIRPT_ENABLE_SHFT                                                   0x6
#define HWIO_TCSR_SS_MMU_CFG_SEC_INTR_ENABLE_MMSS_BIMC_SMMU_GCFGIRPT_ENABLE_BMSK                                             0x2
#define HWIO_TCSR_SS_MMU_CFG_SEC_INTR_ENABLE_MMSS_BIMC_SMMU_GCFGIRPT_ENABLE_SHFT                                             0x1

#define HWIO_TCSR_SS_XPU_MSA_INTR0_ADDR                                                                               (TCSR_TCSR_REGS_REG_BASE      + 0x00006000)
#define HWIO_TCSR_SS_XPU_MSA_INTR0_OFFS                                                                               (TCSR_TCSR_REGS_REG_BASE_OFFS + 0x00006000)
#define HWIO_TCSR_SS_XPU_MSA_INTR0_RMSK                                                                               0xffffffff
#define HWIO_TCSR_SS_XPU_MSA_INTR0_IN          \
        in_dword_masked(HWIO_TCSR_SS_XPU_MSA_INTR0_ADDR, HWIO_TCSR_SS_XPU_MSA_INTR0_RMSK)
#define HWIO_TCSR_SS_XPU_MSA_INTR0_INM(m)      \
        in_dword_masked(HWIO_TCSR_SS_XPU_MSA_INTR0_ADDR, m)
#define HWIO_TCSR_SS_XPU_MSA_INTR0_QM_XPU3_MSA_INTR0_BMSK                                                             0x40000000
#define HWIO_TCSR_SS_XPU_MSA_INTR0_QM_XPU3_MSA_INTR0_SHFT                                                                   0x1e
#define HWIO_TCSR_SS_XPU_MSA_INTR0_MEMNOC_MCDMA_XPU3_MSA_INTR0_BMSK                                                   0x20000000
#define HWIO_TCSR_SS_XPU_MSA_INTR0_MEMNOC_MCDMA_XPU3_MSA_INTR0_SHFT                                                         0x1d
#define HWIO_TCSR_SS_XPU_MSA_INTR0_SPMI_BAM_APU_MSA_INTR0_BMSK                                                        0x10000000
#define HWIO_TCSR_SS_XPU_MSA_INTR0_SPMI_BAM_APU_MSA_INTR0_SHFT                                                              0x1c
#define HWIO_TCSR_SS_XPU_MSA_INTR0_IPA_APU_MSA_INTR0_BMSK                                                              0x8000000
#define HWIO_TCSR_SS_XPU_MSA_INTR0_IPA_APU_MSA_INTR0_SHFT                                                                   0x1b
#define HWIO_TCSR_SS_XPU_MSA_INTR0_BLSP_BAM_APU_MSA_INTR0_BMSK                                                         0x4000000
#define HWIO_TCSR_SS_XPU_MSA_INTR0_BLSP_BAM_APU_MSA_INTR0_SHFT                                                              0x1a
#define HWIO_TCSR_SS_XPU_MSA_INTR0_PKA_XPU_MSA_INTR0_BMSK                                                              0x2000000
#define HWIO_TCSR_SS_XPU_MSA_INTR0_PKA_XPU_MSA_INTR0_SHFT                                                                   0x19
#define HWIO_TCSR_SS_XPU_MSA_INTR0_QPIC_XPU2_MSA_INTR0_BMSK                                                            0x1000000
#define HWIO_TCSR_SS_XPU_MSA_INTR0_QPIC_XPU2_MSA_INTR0_SHFT                                                                 0x18
#define HWIO_TCSR_SS_XPU_MSA_INTR0_QXS_Q6_TCM_SS_MPU_XPU3_MSA_INTR0_BMSK                                                0x400000
#define HWIO_TCSR_SS_XPU_MSA_INTR0_QXS_Q6_TCM_SS_MPU_XPU3_MSA_INTR0_SHFT                                                    0x16
#define HWIO_TCSR_SS_XPU_MSA_INTR0_QXM_MSS_NAV_CE_MS_MPU_XPU3_MSA_INTR0_BMSK                                            0x200000
#define HWIO_TCSR_SS_XPU_MSA_INTR0_QXM_MSS_NAV_CE_MS_MPU_XPU3_MSA_INTR0_SHFT                                                0x15
#define HWIO_TCSR_SS_XPU_MSA_INTR0_QNM_MEMNOC_MS_MPU_XPU3_MSA_INTR0_BMSK                                                0x100000
#define HWIO_TCSR_SS_XPU_MSA_INTR0_QNM_MEMNOC_MS_MPU_XPU3_MSA_INTR0_SHFT                                                    0x14
#define HWIO_TCSR_SS_XPU_MSA_INTR0_QNM_AGGRE_NOC_IPA_MS_MPU_XPU3_MSA_INTR0_BMSK                                          0x80000
#define HWIO_TCSR_SS_XPU_MSA_INTR0_QNM_AGGRE_NOC_IPA_MS_MPU_XPU3_MSA_INTR0_SHFT                                             0x13
#define HWIO_TCSR_SS_XPU_MSA_INTR0_QHS_SNOC_CFG_SS_MPU_XPU3_MSA_INTR0_BMSK                                               0x40000
#define HWIO_TCSR_SS_XPU_MSA_INTR0_QHS_SNOC_CFG_SS_MPU_XPU3_MSA_INTR0_SHFT                                                  0x12
#define HWIO_TCSR_SS_XPU_MSA_INTR0_TLMM_CENTRAL_XPU_MSA_INTR0_BMSK                                                       0x20000
#define HWIO_TCSR_SS_XPU_MSA_INTR0_TLMM_CENTRAL_XPU_MSA_INTR0_SHFT                                                          0x11
#define HWIO_TCSR_SS_XPU_MSA_INTR0_SPDM_WRAPPER_XPU_MSA_INTR0_BMSK                                                       0x10000
#define HWIO_TCSR_SS_XPU_MSA_INTR0_SPDM_WRAPPER_XPU_MSA_INTR0_SHFT                                                          0x10
#define HWIO_TCSR_SS_XPU_MSA_INTR0_OCIMEM_MPU_MSA_INTR0_BMSK                                                              0x8000
#define HWIO_TCSR_SS_XPU_MSA_INTR0_OCIMEM_MPU_MSA_INTR0_SHFT                                                                 0xf
#define HWIO_TCSR_SS_XPU_MSA_INTR0_DTP_QHS_NON_BROADCAST_MPU_MAINO_XPU3_MSA_INTR0_BMSK                                    0x4000
#define HWIO_TCSR_SS_XPU_MSA_INTR0_DTP_QHS_NON_BROADCAST_MPU_MAINO_XPU3_MSA_INTR0_SHFT                                       0xe
#define HWIO_TCSR_SS_XPU_MSA_INTR0_BOOT_ROM_MSA_INTR0_BMSK                                                                0x2000
#define HWIO_TCSR_SS_XPU_MSA_INTR0_BOOT_ROM_MSA_INTR0_SHFT                                                                   0xd
#define HWIO_TCSR_SS_XPU_MSA_INTR0_DTP_QHS_BROADCAST_MPU_MAINO_XPU3_MSA_INTR0_BMSK                                        0x1000
#define HWIO_TCSR_SS_XPU_MSA_INTR0_DTP_QHS_BROADCAST_MPU_MAINO_XPU3_MSA_INTR0_SHFT                                           0xc
#define HWIO_TCSR_SS_XPU_MSA_INTR0_LLCC0_XPU3_MSA_INTR0_BMSK                                                               0x800
#define HWIO_TCSR_SS_XPU_MSA_INTR0_LLCC0_XPU3_MSA_INTR0_SHFT                                                                 0xb
#define HWIO_TCSR_SS_XPU_MSA_INTR0_GCC_XPU_MSA_INTR0_BMSK                                                                  0x400
#define HWIO_TCSR_SS_XPU_MSA_INTR0_GCC_XPU_MSA_INTR0_SHFT                                                                    0xa
#define HWIO_TCSR_SS_XPU_MSA_INTR0_MEMNOC_XPU3_MSA_INTR0_BMSK                                                              0x200
#define HWIO_TCSR_SS_XPU_MSA_INTR0_MEMNOC_XPU3_MSA_INTR0_SHFT                                                                0x9
#define HWIO_TCSR_SS_XPU_MSA_INTR0_QHM_AOSS_MS_MPU_XPU3_MSA_INTR0_BMSK                                                     0x100
#define HWIO_TCSR_SS_XPU_MSA_INTR0_QHM_AOSS_MS_MPU_XPU3_MSA_INTR0_SHFT                                                       0x8
#define HWIO_TCSR_SS_XPU_MSA_INTR0_SEC_CTRL_XPU3_MSA_INTR0_BMSK                                                             0x80
#define HWIO_TCSR_SS_XPU_MSA_INTR0_SEC_CTRL_XPU3_MSA_INTR0_SHFT                                                              0x7
#define HWIO_TCSR_SS_XPU_MSA_INTR0_DCC_XPU_MSA_INTR0_BMSK                                                                   0x40
#define HWIO_TCSR_SS_XPU_MSA_INTR0_DCC_XPU_MSA_INTR0_SHFT                                                                    0x6
#define HWIO_TCSR_SS_XPU_MSA_INTR0_CRYPTO_BAM_APU_MSA_INTR0_BMSK                                                            0x10
#define HWIO_TCSR_SS_XPU_MSA_INTR0_CRYPTO_BAM_APU_MSA_INTR0_SHFT                                                             0x4
#define HWIO_TCSR_SS_XPU_MSA_INTR0_TCSR_MUTEX_XPU_MSA_INTR0_BMSK                                                             0x8
#define HWIO_TCSR_SS_XPU_MSA_INTR0_TCSR_MUTEX_XPU_MSA_INTR0_SHFT                                                             0x3
#define HWIO_TCSR_SS_XPU_MSA_INTR0_TCSR_REGS_XPU_MSA_INTR0_BMSK                                                              0x4
#define HWIO_TCSR_SS_XPU_MSA_INTR0_TCSR_REGS_XPU_MSA_INTR0_SHFT                                                              0x2
#define HWIO_TCSR_SS_XPU_MSA_INTR0_AOSS_MPU_MSA_INTR0_BMSK                                                                   0x2
#define HWIO_TCSR_SS_XPU_MSA_INTR0_AOSS_MPU_MSA_INTR0_SHFT                                                                   0x1
#define HWIO_TCSR_SS_XPU_MSA_INTR0_PMIC_ARB_MPU_MSA_INTR0_BMSK                                                               0x1
#define HWIO_TCSR_SS_XPU_MSA_INTR0_PMIC_ARB_MPU_MSA_INTR0_SHFT                                                               0x0

#define HWIO_TCSR_SS_XPU_MSA_INTR1_ADDR                                                                               (TCSR_TCSR_REGS_REG_BASE      + 0x00006004)
#define HWIO_TCSR_SS_XPU_MSA_INTR1_OFFS                                                                               (TCSR_TCSR_REGS_REG_BASE_OFFS + 0x00006004)
#define HWIO_TCSR_SS_XPU_MSA_INTR1_RMSK                                                                                  0x7ffff
#define HWIO_TCSR_SS_XPU_MSA_INTR1_IN          \
        in_dword_masked(HWIO_TCSR_SS_XPU_MSA_INTR1_ADDR, HWIO_TCSR_SS_XPU_MSA_INTR1_RMSK)
#define HWIO_TCSR_SS_XPU_MSA_INTR1_INM(m)      \
        in_dword_masked(HWIO_TCSR_SS_XPU_MSA_INTR1_ADDR, m)

#define HWIO_TCSR_SS_XPU_MSA_INTR0_ENABLE_ADDR                                                                        (TCSR_TCSR_REGS_REG_BASE      + 0x00006040)
#define HWIO_TCSR_SS_XPU_MSA_INTR0_ENABLE_OFFS                                                                        (TCSR_TCSR_REGS_REG_BASE_OFFS + 0x00006040)
#define HWIO_TCSR_SS_XPU_MSA_INTR0_ENABLE_RMSK                                                                        0xffffffff
#define HWIO_TCSR_SS_XPU_MSA_INTR0_ENABLE_IN          \
        in_dword_masked(HWIO_TCSR_SS_XPU_MSA_INTR0_ENABLE_ADDR, HWIO_TCSR_SS_XPU_MSA_INTR0_ENABLE_RMSK)
#define HWIO_TCSR_SS_XPU_MSA_INTR0_ENABLE_INM(m)      \
        in_dword_masked(HWIO_TCSR_SS_XPU_MSA_INTR0_ENABLE_ADDR, m)
#define HWIO_TCSR_SS_XPU_MSA_INTR0_ENABLE_OUT(v)      \
        out_dword(HWIO_TCSR_SS_XPU_MSA_INTR0_ENABLE_ADDR,v)
#define HWIO_TCSR_SS_XPU_MSA_INTR0_ENABLE_OUTM(m,v) \
        out_dword_masked_ns(HWIO_TCSR_SS_XPU_MSA_INTR0_ENABLE_ADDR,m,v,HWIO_TCSR_SS_XPU_MSA_INTR0_ENABLE_IN)
#define HWIO_TCSR_SS_XPU_MSA_INTR0_ENABLE_QM_XPU3_MSA_INTR0_ENABLE_BMSK                                               0x40000000
#define HWIO_TCSR_SS_XPU_MSA_INTR0_ENABLE_QM_XPU3_MSA_INTR0_ENABLE_SHFT                                                     0x1e
#define HWIO_TCSR_SS_XPU_MSA_INTR0_ENABLE_MEMNOC_MCDMA_XPU3_MSA_INTR0_ENABLE_BMSK                                     0x20000000
#define HWIO_TCSR_SS_XPU_MSA_INTR0_ENABLE_MEMNOC_MCDMA_XPU3_MSA_INTR0_ENABLE_SHFT                                           0x1d
#define HWIO_TCSR_SS_XPU_MSA_INTR0_ENABLE_SPMI_BAM_APU_MSA_INTR0_ENABLE_BMSK                                          0x10000000
#define HWIO_TCSR_SS_XPU_MSA_INTR0_ENABLE_SPMI_BAM_APU_MSA_INTR0_ENABLE_SHFT                                                0x1c
#define HWIO_TCSR_SS_XPU_MSA_INTR0_ENABLE_IPA_APU_MSA_INTR0_ENABLE_BMSK                                                0x8000000
#define HWIO_TCSR_SS_XPU_MSA_INTR0_ENABLE_IPA_APU_MSA_INTR0_ENABLE_SHFT                                                     0x1b
#define HWIO_TCSR_SS_XPU_MSA_INTR0_ENABLE_BLSP_BAM_APU_MSA_INTR0_ENABLE_BMSK                                           0x4000000
#define HWIO_TCSR_SS_XPU_MSA_INTR0_ENABLE_BLSP_BAM_APU_MSA_INTR0_ENABLE_SHFT                                                0x1a
#define HWIO_TCSR_SS_XPU_MSA_INTR0_ENABLE_PKA_XPU_MSA_INTR0_ENABLE_BMSK                                                0x2000000
#define HWIO_TCSR_SS_XPU_MSA_INTR0_ENABLE_PKA_XPU_MSA_INTR0_ENABLE_SHFT                                                     0x19
#define HWIO_TCSR_SS_XPU_MSA_INTR0_ENABLE_QPIC_XPU2_MSA_INTR0_ENABLE_BMSK                                              0x1000000
#define HWIO_TCSR_SS_XPU_MSA_INTR0_ENABLE_QPIC_XPU2_MSA_INTR0_ENABLE_SHFT                                                   0x18
#define HWIO_TCSR_SS_XPU_MSA_INTR0_ENABLE_QXS_Q6_TCM_SS_MPU_XPU3_MSA_INTR0_ENABLE_BMSK                                  0x400000
#define HWIO_TCSR_SS_XPU_MSA_INTR0_ENABLE_QXS_Q6_TCM_SS_MPU_XPU3_MSA_INTR0_ENABLE_SHFT                                      0x16
#define HWIO_TCSR_SS_XPU_MSA_INTR0_ENABLE_QXM_MSS_NAV_CE_MS_MPU_XPU3_MSA_INTR0_ENABLE_BMSK                              0x200000
#define HWIO_TCSR_SS_XPU_MSA_INTR0_ENABLE_QXM_MSS_NAV_CE_MS_MPU_XPU3_MSA_INTR0_ENABLE_SHFT                                  0x15
#define HWIO_TCSR_SS_XPU_MSA_INTR0_ENABLE_QNM_MEMNOC_MS_MPU_XPU3_MSA_INTR0_ENABLE_BMSK                                  0x100000
#define HWIO_TCSR_SS_XPU_MSA_INTR0_ENABLE_QNM_MEMNOC_MS_MPU_XPU3_MSA_INTR0_ENABLE_SHFT                                      0x14
#define HWIO_TCSR_SS_XPU_MSA_INTR0_ENABLE_QNM_AGGRE_NOC_IPA_MS_MPU_XPU3_MSA_INTR0_ENABLE_BMSK                            0x80000
#define HWIO_TCSR_SS_XPU_MSA_INTR0_ENABLE_QNM_AGGRE_NOC_IPA_MS_MPU_XPU3_MSA_INTR0_ENABLE_SHFT                               0x13
#define HWIO_TCSR_SS_XPU_MSA_INTR0_ENABLE_QHS_SNOC_CFG_SS_MPU_XPU3_MSA_INTR0_ENABLE_BMSK                                 0x40000
#define HWIO_TCSR_SS_XPU_MSA_INTR0_ENABLE_QHS_SNOC_CFG_SS_MPU_XPU3_MSA_INTR0_ENABLE_SHFT                                    0x12
#define HWIO_TCSR_SS_XPU_MSA_INTR0_ENABLE_TLMM_CENTRAL_XPU_MSA_INTR0_ENABLE_BMSK                                         0x20000
#define HWIO_TCSR_SS_XPU_MSA_INTR0_ENABLE_TLMM_CENTRAL_XPU_MSA_INTR0_ENABLE_SHFT                                            0x11
#define HWIO_TCSR_SS_XPU_MSA_INTR0_ENABLE_SPDM_WRAPPER_XPU_MSA_INTR0_ENABLE_BMSK                                         0x10000
#define HWIO_TCSR_SS_XPU_MSA_INTR0_ENABLE_SPDM_WRAPPER_XPU_MSA_INTR0_ENABLE_SHFT                                            0x10
#define HWIO_TCSR_SS_XPU_MSA_INTR0_ENABLE_OCIMEM_MPU_MSA_INTR0_ENABLE_BMSK                                                0x8000
#define HWIO_TCSR_SS_XPU_MSA_INTR0_ENABLE_OCIMEM_MPU_MSA_INTR0_ENABLE_SHFT                                                   0xf
#define HWIO_TCSR_SS_XPU_MSA_INTR0_ENABLE_DTP_QHS_NON_BROADCAST_MPU_MAINO_XPU3_MSA_INTR0_ENABLE_BMSK                      0x4000
#define HWIO_TCSR_SS_XPU_MSA_INTR0_ENABLE_DTP_QHS_NON_BROADCAST_MPU_MAINO_XPU3_MSA_INTR0_ENABLE_SHFT                         0xe
#define HWIO_TCSR_SS_XPU_MSA_INTR0_ENABLE_BOOT_ROM_MSA_INTR0_ENABLE_BMSK                                                  0x2000
#define HWIO_TCSR_SS_XPU_MSA_INTR0_ENABLE_BOOT_ROM_MSA_INTR0_ENABLE_SHFT                                                     0xd
#define HWIO_TCSR_SS_XPU_MSA_INTR0_ENABLE_DTP_QHS_BROADCAST_MPU_MAINO_XPU3_MSA_INTR0_ENABLE_BMSK                          0x1000
#define HWIO_TCSR_SS_XPU_MSA_INTR0_ENABLE_DTP_QHS_BROADCAST_MPU_MAINO_XPU3_MSA_INTR0_ENABLE_SHFT                             0xc
#define HWIO_TCSR_SS_XPU_MSA_INTR0_ENABLE_LLCC0_XPU3_MSA_INTR0_ENABLE_BMSK                                                 0x800
#define HWIO_TCSR_SS_XPU_MSA_INTR0_ENABLE_LLCC0_XPU3_MSA_INTR0_ENABLE_SHFT                                                   0xb
#define HWIO_TCSR_SS_XPU_MSA_INTR0_ENABLE_GCC_XPU_MSA_INTR0_ENABLE_BMSK                                                    0x400
#define HWIO_TCSR_SS_XPU_MSA_INTR0_ENABLE_GCC_XPU_MSA_INTR0_ENABLE_SHFT                                                      0xa
#define HWIO_TCSR_SS_XPU_MSA_INTR0_ENABLE_MEMNOC_XPU3_MSA_INTR0_ENABLE_BMSK                                                0x200
#define HWIO_TCSR_SS_XPU_MSA_INTR0_ENABLE_MEMNOC_XPU3_MSA_INTR0_ENABLE_SHFT                                                  0x9
#define HWIO_TCSR_SS_XPU_MSA_INTR0_ENABLE_QHM_AOSS_MS_MPU_XPU3_MSA_INTR0_ENABLE_BMSK                                       0x100
#define HWIO_TCSR_SS_XPU_MSA_INTR0_ENABLE_QHM_AOSS_MS_MPU_XPU3_MSA_INTR0_ENABLE_SHFT                                         0x8
#define HWIO_TCSR_SS_XPU_MSA_INTR0_ENABLE_SEC_CTRL_XPU3_MSA_INTR0_ENABLE_BMSK                                               0x80
#define HWIO_TCSR_SS_XPU_MSA_INTR0_ENABLE_SEC_CTRL_XPU3_MSA_INTR0_ENABLE_SHFT                                                0x7
#define HWIO_TCSR_SS_XPU_MSA_INTR0_ENABLE_DCC_XPU_MSA_INTR0_ENABLE_BMSK                                                     0x40
#define HWIO_TCSR_SS_XPU_MSA_INTR0_ENABLE_DCC_XPU_MSA_INTR0_ENABLE_SHFT                                                      0x6
#define HWIO_TCSR_SS_XPU_MSA_INTR0_ENABLE_CRYPTO_BAM_APU_MSA_INTR0_ENABLE_BMSK                                              0x10
#define HWIO_TCSR_SS_XPU_MSA_INTR0_ENABLE_CRYPTO_BAM_APU_MSA_INTR0_ENABLE_SHFT                                               0x4
#define HWIO_TCSR_SS_XPU_MSA_INTR0_ENABLE_TCSR_MUTEX_XPU_MSA_INTR0_ENABLE_BMSK                                               0x8
#define HWIO_TCSR_SS_XPU_MSA_INTR0_ENABLE_TCSR_MUTEX_XPU_MSA_INTR0_ENABLE_SHFT                                               0x3
#define HWIO_TCSR_SS_XPU_MSA_INTR0_ENABLE_TCSR_REGS_XPU_MSA_INTR0_ENABLE_BMSK                                                0x4
#define HWIO_TCSR_SS_XPU_MSA_INTR0_ENABLE_TCSR_REGS_XPU_MSA_INTR0_ENABLE_SHFT                                                0x2
#define HWIO_TCSR_SS_XPU_MSA_INTR0_ENABLE_AOSS_MPU_MSA_INTR0_ENABLE_BMSK                                                     0x2
#define HWIO_TCSR_SS_XPU_MSA_INTR0_ENABLE_AOSS_MPU_MSA_INTR0_ENABLE_SHFT                                                     0x1
#define HWIO_TCSR_SS_XPU_MSA_INTR0_ENABLE_PMIC_ARB_MPU_MSA_INTR0_ENABLE_BMSK                                                 0x1
#define HWIO_TCSR_SS_XPU_MSA_INTR0_ENABLE_PMIC_ARB_MPU_MSA_INTR0_ENABLE_SHFT                                                 0x0

#define HWIO_TCSR_SS_XPU_MSA_INTR1_ENABLE_ADDR                                                                        (TCSR_TCSR_REGS_REG_BASE      + 0x00006044)
#define HWIO_TCSR_SS_XPU_MSA_INTR1_ENABLE_OFFS                                                                        (TCSR_TCSR_REGS_REG_BASE_OFFS + 0x00006044)
#define HWIO_TCSR_SS_XPU_MSA_INTR1_ENABLE_RMSK                                                                           0x7ffff
#define HWIO_TCSR_SS_XPU_MSA_INTR1_ENABLE_IN          \
        in_dword_masked(HWIO_TCSR_SS_XPU_MSA_INTR1_ENABLE_ADDR, HWIO_TCSR_SS_XPU_MSA_INTR1_ENABLE_RMSK)
#define HWIO_TCSR_SS_XPU_MSA_INTR1_ENABLE_INM(m)      \
        in_dword_masked(HWIO_TCSR_SS_XPU_MSA_INTR1_ENABLE_ADDR, m)
#define HWIO_TCSR_SS_XPU_MSA_INTR1_ENABLE_OUT(v)      \
        out_dword(HWIO_TCSR_SS_XPU_MSA_INTR1_ENABLE_ADDR,v)
#define HWIO_TCSR_SS_XPU_MSA_INTR1_ENABLE_OUTM(m,v) \
        out_dword_masked_ns(HWIO_TCSR_SS_XPU_MSA_INTR1_ENABLE_ADDR,m,v,HWIO_TCSR_SS_XPU_MSA_INTR1_ENABLE_IN)

#define HWIO_TCSR_SPDM_CNT_CLK_CTRL_ADDR                                                                              (TCSR_TCSR_REGS_REG_BASE      + 0x00007000)
#define HWIO_TCSR_SPDM_CNT_CLK_CTRL_OFFS                                                                              (TCSR_TCSR_REGS_REG_BASE_OFFS + 0x00007000)
#define HWIO_TCSR_SPDM_CNT_CLK_CTRL_RMSK                                                                                  0xffff
#define HWIO_TCSR_SPDM_CNT_CLK_CTRL_IN          \
        in_dword_masked(HWIO_TCSR_SPDM_CNT_CLK_CTRL_ADDR, HWIO_TCSR_SPDM_CNT_CLK_CTRL_RMSK)
#define HWIO_TCSR_SPDM_CNT_CLK_CTRL_INM(m)      \
        in_dword_masked(HWIO_TCSR_SPDM_CNT_CLK_CTRL_ADDR, m)
#define HWIO_TCSR_SPDM_CNT_CLK_CTRL_OUT(v)      \
        out_dword(HWIO_TCSR_SPDM_CNT_CLK_CTRL_ADDR,v)
#define HWIO_TCSR_SPDM_CNT_CLK_CTRL_OUTM(m,v) \
        out_dword_masked_ns(HWIO_TCSR_SPDM_CNT_CLK_CTRL_ADDR,m,v,HWIO_TCSR_SPDM_CNT_CLK_CTRL_IN)
#define HWIO_TCSR_SPDM_CNT_CLK_CTRL_SPDM_CNT_CLK_MUX_SEL_BMSK                                                             0xffff
#define HWIO_TCSR_SPDM_CNT_CLK_CTRL_SPDM_CNT_CLK_MUX_SEL_SHFT                                                                0x0

#define HWIO_TCSR_SPDM_DLY_FIFO_EN_ADDR                                                                               (TCSR_TCSR_REGS_REG_BASE      + 0x00007004)
#define HWIO_TCSR_SPDM_DLY_FIFO_EN_OFFS                                                                               (TCSR_TCSR_REGS_REG_BASE_OFFS + 0x00007004)
#define HWIO_TCSR_SPDM_DLY_FIFO_EN_RMSK                                                                               0xffffffff
#define HWIO_TCSR_SPDM_DLY_FIFO_EN_IN          \
        in_dword_masked(HWIO_TCSR_SPDM_DLY_FIFO_EN_ADDR, HWIO_TCSR_SPDM_DLY_FIFO_EN_RMSK)
#define HWIO_TCSR_SPDM_DLY_FIFO_EN_INM(m)      \
        in_dword_masked(HWIO_TCSR_SPDM_DLY_FIFO_EN_ADDR, m)
#define HWIO_TCSR_SPDM_DLY_FIFO_EN_OUT(v)      \
        out_dword(HWIO_TCSR_SPDM_DLY_FIFO_EN_ADDR,v)
#define HWIO_TCSR_SPDM_DLY_FIFO_EN_OUTM(m,v) \
        out_dword_masked_ns(HWIO_TCSR_SPDM_DLY_FIFO_EN_ADDR,m,v,HWIO_TCSR_SPDM_DLY_FIFO_EN_IN)
#define HWIO_TCSR_SPDM_DLY_FIFO_EN_SPDM_DLY_FIFO_EN_BMSK                                                              0xffffffff
#define HWIO_TCSR_SPDM_DLY_FIFO_EN_SPDM_DLY_FIFO_EN_SHFT                                                                     0x0

#define HWIO_TCSR_SPDM_WRP_CTI_CTL0_ADDR                                                                              (TCSR_TCSR_REGS_REG_BASE      + 0x0000705c)
#define HWIO_TCSR_SPDM_WRP_CTI_CTL0_OFFS                                                                              (TCSR_TCSR_REGS_REG_BASE_OFFS + 0x0000705c)
#define HWIO_TCSR_SPDM_WRP_CTI_CTL0_RMSK                                                                              0xffffffff
#define HWIO_TCSR_SPDM_WRP_CTI_CTL0_IN          \
        in_dword_masked(HWIO_TCSR_SPDM_WRP_CTI_CTL0_ADDR, HWIO_TCSR_SPDM_WRP_CTI_CTL0_RMSK)
#define HWIO_TCSR_SPDM_WRP_CTI_CTL0_INM(m)      \
        in_dword_masked(HWIO_TCSR_SPDM_WRP_CTI_CTL0_ADDR, m)
#define HWIO_TCSR_SPDM_WRP_CTI_CTL0_OUT(v)      \
        out_dword(HWIO_TCSR_SPDM_WRP_CTI_CTL0_ADDR,v)
#define HWIO_TCSR_SPDM_WRP_CTI_CTL0_OUTM(m,v) \
        out_dword_masked_ns(HWIO_TCSR_SPDM_WRP_CTI_CTL0_ADDR,m,v,HWIO_TCSR_SPDM_WRP_CTI_CTL0_IN)
#define HWIO_TCSR_SPDM_WRP_CTI_CTL0_SPDM_WRP_CTI_CTL0_BMSK                                                            0xffffffff
#define HWIO_TCSR_SPDM_WRP_CTI_CTL0_SPDM_WRP_CTI_CTL0_SHFT                                                                   0x0

#define HWIO_TCSR_SPDM_WRP_CTI_CTL1_ADDR                                                                              (TCSR_TCSR_REGS_REG_BASE      + 0x00007060)
#define HWIO_TCSR_SPDM_WRP_CTI_CTL1_OFFS                                                                              (TCSR_TCSR_REGS_REG_BASE_OFFS + 0x00007060)
#define HWIO_TCSR_SPDM_WRP_CTI_CTL1_RMSK                                                                              0xffffffff
#define HWIO_TCSR_SPDM_WRP_CTI_CTL1_IN          \
        in_dword_masked(HWIO_TCSR_SPDM_WRP_CTI_CTL1_ADDR, HWIO_TCSR_SPDM_WRP_CTI_CTL1_RMSK)
#define HWIO_TCSR_SPDM_WRP_CTI_CTL1_INM(m)      \
        in_dword_masked(HWIO_TCSR_SPDM_WRP_CTI_CTL1_ADDR, m)
#define HWIO_TCSR_SPDM_WRP_CTI_CTL1_OUT(v)      \
        out_dword(HWIO_TCSR_SPDM_WRP_CTI_CTL1_ADDR,v)
#define HWIO_TCSR_SPDM_WRP_CTI_CTL1_OUTM(m,v) \
        out_dword_masked_ns(HWIO_TCSR_SPDM_WRP_CTI_CTL1_ADDR,m,v,HWIO_TCSR_SPDM_WRP_CTI_CTL1_IN)
#define HWIO_TCSR_SPDM_WRP_CTI_CTL1_SPDM_WRP_CTI_CTL1_BMSK                                                            0xffffffff
#define HWIO_TCSR_SPDM_WRP_CTI_CTL1_SPDM_WRP_CTI_CTL1_SHFT                                                                   0x0

#define HWIO_TCSR_SPDM_WRP_CTI_CTL2_ADDR                                                                              (TCSR_TCSR_REGS_REG_BASE      + 0x00007064)
#define HWIO_TCSR_SPDM_WRP_CTI_CTL2_OFFS                                                                              (TCSR_TCSR_REGS_REG_BASE_OFFS + 0x00007064)
#define HWIO_TCSR_SPDM_WRP_CTI_CTL2_RMSK                                                                              0xffffffff
#define HWIO_TCSR_SPDM_WRP_CTI_CTL2_IN          \
        in_dword_masked(HWIO_TCSR_SPDM_WRP_CTI_CTL2_ADDR, HWIO_TCSR_SPDM_WRP_CTI_CTL2_RMSK)
#define HWIO_TCSR_SPDM_WRP_CTI_CTL2_INM(m)      \
        in_dword_masked(HWIO_TCSR_SPDM_WRP_CTI_CTL2_ADDR, m)
#define HWIO_TCSR_SPDM_WRP_CTI_CTL2_OUT(v)      \
        out_dword(HWIO_TCSR_SPDM_WRP_CTI_CTL2_ADDR,v)
#define HWIO_TCSR_SPDM_WRP_CTI_CTL2_OUTM(m,v) \
        out_dword_masked_ns(HWIO_TCSR_SPDM_WRP_CTI_CTL2_ADDR,m,v,HWIO_TCSR_SPDM_WRP_CTI_CTL2_IN)
#define HWIO_TCSR_SPDM_WRP_CTI_CTL2_SPDM_WRP_CTI_CTL2_BMSK                                                            0xffffffff
#define HWIO_TCSR_SPDM_WRP_CTI_CTL2_SPDM_WRP_CTI_CTL2_SHFT                                                                   0x0

#define HWIO_TCSR_SPDM_WRP_CTI_CTL3_ADDR                                                                              (TCSR_TCSR_REGS_REG_BASE      + 0x00007068)
#define HWIO_TCSR_SPDM_WRP_CTI_CTL3_OFFS                                                                              (TCSR_TCSR_REGS_REG_BASE_OFFS + 0x00007068)
#define HWIO_TCSR_SPDM_WRP_CTI_CTL3_RMSK                                                                              0xffffffff
#define HWIO_TCSR_SPDM_WRP_CTI_CTL3_IN          \
        in_dword_masked(HWIO_TCSR_SPDM_WRP_CTI_CTL3_ADDR, HWIO_TCSR_SPDM_WRP_CTI_CTL3_RMSK)
#define HWIO_TCSR_SPDM_WRP_CTI_CTL3_INM(m)      \
        in_dword_masked(HWIO_TCSR_SPDM_WRP_CTI_CTL3_ADDR, m)
#define HWIO_TCSR_SPDM_WRP_CTI_CTL3_OUT(v)      \
        out_dword(HWIO_TCSR_SPDM_WRP_CTI_CTL3_ADDR,v)
#define HWIO_TCSR_SPDM_WRP_CTI_CTL3_OUTM(m,v) \
        out_dword_masked_ns(HWIO_TCSR_SPDM_WRP_CTI_CTL3_ADDR,m,v,HWIO_TCSR_SPDM_WRP_CTI_CTL3_IN)
#define HWIO_TCSR_SPDM_WRP_CTI_CTL3_SPDM_WRP_CTI_CTL3_BMSK                                                            0xffffffff
#define HWIO_TCSR_SPDM_WRP_CTI_CTL3_SPDM_WRP_CTI_CTL3_SHFT                                                                   0x0

#define HWIO_TCSR_SPDM_WRP_CTI_CTL_ADDR                                                                               (TCSR_TCSR_REGS_REG_BASE      + 0x00007070)
#define HWIO_TCSR_SPDM_WRP_CTI_CTL_OFFS                                                                               (TCSR_TCSR_REGS_REG_BASE_OFFS + 0x00007070)
#define HWIO_TCSR_SPDM_WRP_CTI_CTL_RMSK                                                                                      0x7
#define HWIO_TCSR_SPDM_WRP_CTI_CTL_IN          \
        in_dword_masked(HWIO_TCSR_SPDM_WRP_CTI_CTL_ADDR, HWIO_TCSR_SPDM_WRP_CTI_CTL_RMSK)
#define HWIO_TCSR_SPDM_WRP_CTI_CTL_INM(m)      \
        in_dword_masked(HWIO_TCSR_SPDM_WRP_CTI_CTL_ADDR, m)
#define HWIO_TCSR_SPDM_WRP_CTI_CTL_OUT(v)      \
        out_dword(HWIO_TCSR_SPDM_WRP_CTI_CTL_ADDR,v)
#define HWIO_TCSR_SPDM_WRP_CTI_CTL_OUTM(m,v) \
        out_dword_masked_ns(HWIO_TCSR_SPDM_WRP_CTI_CTL_ADDR,m,v,HWIO_TCSR_SPDM_WRP_CTI_CTL_IN)
#define HWIO_TCSR_SPDM_WRP_CTI_CTL_SPDM_WRP_CTI_CTL_BMSK                                                                     0x7
#define HWIO_TCSR_SPDM_WRP_CTI_CTL_SPDM_WRP_CTI_CTL_SHFT                                                                     0x0

#define HWIO_TCSR_SOC_HW_VERSION_ADDR                                                                                 (TCSR_TCSR_REGS_REG_BASE      + 0x00008000)
#define HWIO_TCSR_SOC_HW_VERSION_OFFS                                                                                 (TCSR_TCSR_REGS_REG_BASE_OFFS + 0x00008000)
#define HWIO_TCSR_SOC_HW_VERSION_RMSK                                                                                 0xffffffff
#define HWIO_TCSR_SOC_HW_VERSION_IN          \
        in_dword_masked(HWIO_TCSR_SOC_HW_VERSION_ADDR, HWIO_TCSR_SOC_HW_VERSION_RMSK)
#define HWIO_TCSR_SOC_HW_VERSION_INM(m)      \
        in_dword_masked(HWIO_TCSR_SOC_HW_VERSION_ADDR, m)
#define HWIO_TCSR_SOC_HW_VERSION_FAMILY_NUMBER_BMSK                                                                   0xf0000000
#define HWIO_TCSR_SOC_HW_VERSION_FAMILY_NUMBER_SHFT                                                                         0x1c
#define HWIO_TCSR_SOC_HW_VERSION_DEVICE_NUMBER_BMSK                                                                    0xfff0000
#define HWIO_TCSR_SOC_HW_VERSION_DEVICE_NUMBER_SHFT                                                                         0x10
#define HWIO_TCSR_SOC_HW_VERSION_MAJOR_VERSION_BMSK                                                                       0xff00
#define HWIO_TCSR_SOC_HW_VERSION_MAJOR_VERSION_SHFT                                                                          0x8
#define HWIO_TCSR_SOC_HW_VERSION_MINOR_VERSION_BMSK                                                                         0xff
#define HWIO_TCSR_SOC_HW_VERSION_MINOR_VERSION_SHFT                                                                          0x0

#define HWIO_TCSR_SOC_EMULATION_TYPE_ADDR                                                                             (TCSR_TCSR_REGS_REG_BASE      + 0x00008004)
#define HWIO_TCSR_SOC_EMULATION_TYPE_OFFS                                                                             (TCSR_TCSR_REGS_REG_BASE_OFFS + 0x00008004)
#define HWIO_TCSR_SOC_EMULATION_TYPE_RMSK                                                                                    0x3
#define HWIO_TCSR_SOC_EMULATION_TYPE_IN          \
        in_dword_masked(HWIO_TCSR_SOC_EMULATION_TYPE_ADDR, HWIO_TCSR_SOC_EMULATION_TYPE_RMSK)
#define HWIO_TCSR_SOC_EMULATION_TYPE_INM(m)      \
        in_dword_masked(HWIO_TCSR_SOC_EMULATION_TYPE_ADDR, m)
#define HWIO_TCSR_SOC_EMULATION_TYPE_SOC_EMULATION_TYPE_BMSK                                                                 0x3
#define HWIO_TCSR_SOC_EMULATION_TYPE_SOC_EMULATION_TYPE_SHFT                                                                 0x0

#define HWIO_TCSR_TIMEOUT_INTR_STATUS_ADDR                                                                            (TCSR_TCSR_REGS_REG_BASE      + 0x00008020)
#define HWIO_TCSR_TIMEOUT_INTR_STATUS_OFFS                                                                            (TCSR_TCSR_REGS_REG_BASE_OFFS + 0x00008020)
#define HWIO_TCSR_TIMEOUT_INTR_STATUS_RMSK                                                                            0xffffffff
#define HWIO_TCSR_TIMEOUT_INTR_STATUS_IN          \
        in_dword_masked(HWIO_TCSR_TIMEOUT_INTR_STATUS_ADDR, HWIO_TCSR_TIMEOUT_INTR_STATUS_RMSK)
#define HWIO_TCSR_TIMEOUT_INTR_STATUS_INM(m)      \
        in_dword_masked(HWIO_TCSR_TIMEOUT_INTR_STATUS_ADDR, m)
#define HWIO_TCSR_TIMEOUT_INTR_STATUS_ULTAUDIO_AHBI_TIMEOUT_SLAVE_IRQ_BMSK                                                 0x800
#define HWIO_TCSR_TIMEOUT_INTR_STATUS_ULTAUDIO_AHBI_TIMEOUT_SLAVE_IRQ_SHFT                                                   0xb
#define HWIO_TCSR_TIMEOUT_INTR_STATUS_MSS_CONFIG_TIMEOUT_SLAVE_IRQ_BMSK                                                    0x400
#define HWIO_TCSR_TIMEOUT_INTR_STATUS_MSS_CONFIG_TIMEOUT_SLAVE_IRQ_SHFT                                                      0xa

#define HWIO_TCSR_SRAM_REDUNDANCY_CRC_ERR_ADDR                                                                        (TCSR_TCSR_REGS_REG_BASE      + 0x00008070)
#define HWIO_TCSR_SRAM_REDUNDANCY_CRC_ERR_OFFS                                                                        (TCSR_TCSR_REGS_REG_BASE_OFFS + 0x00008070)
#define HWIO_TCSR_SRAM_REDUNDANCY_CRC_ERR_RMSK                                                                               0x1
#define HWIO_TCSR_SRAM_REDUNDANCY_CRC_ERR_IN          \
        in_dword_masked(HWIO_TCSR_SRAM_REDUNDANCY_CRC_ERR_ADDR, HWIO_TCSR_SRAM_REDUNDANCY_CRC_ERR_RMSK)
#define HWIO_TCSR_SRAM_REDUNDANCY_CRC_ERR_INM(m)      \
        in_dword_masked(HWIO_TCSR_SRAM_REDUNDANCY_CRC_ERR_ADDR, m)
#define HWIO_TCSR_SRAM_REDUNDANCY_CRC_ERR_SRAM_REDUNDANCY_CRC_ERR_BMSK                                                       0x1
#define HWIO_TCSR_SRAM_REDUNDANCY_CRC_ERR_SRAM_REDUNDANCY_CRC_ERR_SHFT                                                       0x0

#define HWIO_TCSR_TCSR_CLK_EN_ADDR                                                                                    (TCSR_TCSR_REGS_REG_BASE      + 0x0000907c)
#define HWIO_TCSR_TCSR_CLK_EN_OFFS                                                                                    (TCSR_TCSR_REGS_REG_BASE_OFFS + 0x0000907c)
#define HWIO_TCSR_TCSR_CLK_EN_RMSK                                                                                           0x1
#define HWIO_TCSR_TCSR_CLK_EN_IN          \
        in_dword_masked(HWIO_TCSR_TCSR_CLK_EN_ADDR, HWIO_TCSR_TCSR_CLK_EN_RMSK)
#define HWIO_TCSR_TCSR_CLK_EN_INM(m)      \
        in_dword_masked(HWIO_TCSR_TCSR_CLK_EN_ADDR, m)
#define HWIO_TCSR_TCSR_CLK_EN_OUT(v)      \
        out_dword(HWIO_TCSR_TCSR_CLK_EN_ADDR,v)
#define HWIO_TCSR_TCSR_CLK_EN_OUTM(m,v) \
        out_dword_masked_ns(HWIO_TCSR_TCSR_CLK_EN_ADDR,m,v,HWIO_TCSR_TCSR_CLK_EN_IN)
#define HWIO_TCSR_TCSR_CLK_EN_TCSR_CLK_EN_BMSK                                                                               0x1
#define HWIO_TCSR_TCSR_CLK_EN_TCSR_CLK_EN_SHFT                                                                               0x0

#define HWIO_TCSR_SYS_POWER_CTRL_ADDR                                                                                 (TCSR_TCSR_REGS_REG_BASE      + 0x0000a000)
#define HWIO_TCSR_SYS_POWER_CTRL_OFFS                                                                                 (TCSR_TCSR_REGS_REG_BASE_OFFS + 0x0000a000)
#define HWIO_TCSR_SYS_POWER_CTRL_RMSK                                                                                     0xffff
#define HWIO_TCSR_SYS_POWER_CTRL_IN          \
        in_dword_masked(HWIO_TCSR_SYS_POWER_CTRL_ADDR, HWIO_TCSR_SYS_POWER_CTRL_RMSK)
#define HWIO_TCSR_SYS_POWER_CTRL_INM(m)      \
        in_dword_masked(HWIO_TCSR_SYS_POWER_CTRL_ADDR, m)
#define HWIO_TCSR_SYS_POWER_CTRL_OUT(v)      \
        out_dword(HWIO_TCSR_SYS_POWER_CTRL_ADDR,v)
#define HWIO_TCSR_SYS_POWER_CTRL_OUTM(m,v) \
        out_dword_masked_ns(HWIO_TCSR_SYS_POWER_CTRL_ADDR,m,v,HWIO_TCSR_SYS_POWER_CTRL_IN)
#define HWIO_TCSR_SYS_POWER_CTRL_SYS_POWER_CTRL_BMSK                                                                      0xffff
#define HWIO_TCSR_SYS_POWER_CTRL_SYS_POWER_CTRL_SHFT                                                                         0x0

#define HWIO_TCSR_USB_CORE_ID_ADDR                                                                                    (TCSR_TCSR_REGS_REG_BASE      + 0x0000a004)
#define HWIO_TCSR_USB_CORE_ID_OFFS                                                                                    (TCSR_TCSR_REGS_REG_BASE_OFFS + 0x0000a004)
#define HWIO_TCSR_USB_CORE_ID_RMSK                                                                                           0x3
#define HWIO_TCSR_USB_CORE_ID_IN          \
        in_dword_masked(HWIO_TCSR_USB_CORE_ID_ADDR, HWIO_TCSR_USB_CORE_ID_RMSK)
#define HWIO_TCSR_USB_CORE_ID_INM(m)      \
        in_dword_masked(HWIO_TCSR_USB_CORE_ID_ADDR, m)
#define HWIO_TCSR_USB_CORE_ID_OUT(v)      \
        out_dword(HWIO_TCSR_USB_CORE_ID_ADDR,v)
#define HWIO_TCSR_USB_CORE_ID_OUTM(m,v) \
        out_dword_masked_ns(HWIO_TCSR_USB_CORE_ID_ADDR,m,v,HWIO_TCSR_USB_CORE_ID_IN)
#define HWIO_TCSR_USB_CORE_ID_USB_CORE_ID_BMSK                                                                               0x3
#define HWIO_TCSR_USB_CORE_ID_USB_CORE_ID_SHFT                                                                               0x0

#define HWIO_TCSR_SPARE_REG0_ADDR                                                                                     (TCSR_TCSR_REGS_REG_BASE      + 0x0000a044)
#define HWIO_TCSR_SPARE_REG0_OFFS                                                                                     (TCSR_TCSR_REGS_REG_BASE_OFFS + 0x0000a044)
#define HWIO_TCSR_SPARE_REG0_RMSK                                                                                     0xffffffff
#define HWIO_TCSR_SPARE_REG0_IN          \
        in_dword_masked(HWIO_TCSR_SPARE_REG0_ADDR, HWIO_TCSR_SPARE_REG0_RMSK)
#define HWIO_TCSR_SPARE_REG0_INM(m)      \
        in_dword_masked(HWIO_TCSR_SPARE_REG0_ADDR, m)
#define HWIO_TCSR_SPARE_REG0_OUT(v)      \
        out_dword(HWIO_TCSR_SPARE_REG0_ADDR,v)
#define HWIO_TCSR_SPARE_REG0_OUTM(m,v) \
        out_dword_masked_ns(HWIO_TCSR_SPARE_REG0_ADDR,m,v,HWIO_TCSR_SPARE_REG0_IN)
#define HWIO_TCSR_SPARE_REG0_SPARE_REG0_BMSK                                                                          0xffffffff
#define HWIO_TCSR_SPARE_REG0_SPARE_REG0_SHFT                                                                                 0x0

#define HWIO_TCSR_SPARE_REG1_ADDR                                                                                     (TCSR_TCSR_REGS_REG_BASE      + 0x0000a048)
#define HWIO_TCSR_SPARE_REG1_OFFS                                                                                     (TCSR_TCSR_REGS_REG_BASE_OFFS + 0x0000a048)
#define HWIO_TCSR_SPARE_REG1_RMSK                                                                                     0xffffffff
#define HWIO_TCSR_SPARE_REG1_IN          \
        in_dword_masked(HWIO_TCSR_SPARE_REG1_ADDR, HWIO_TCSR_SPARE_REG1_RMSK)
#define HWIO_TCSR_SPARE_REG1_INM(m)      \
        in_dword_masked(HWIO_TCSR_SPARE_REG1_ADDR, m)
#define HWIO_TCSR_SPARE_REG1_OUT(v)      \
        out_dword(HWIO_TCSR_SPARE_REG1_ADDR,v)
#define HWIO_TCSR_SPARE_REG1_OUTM(m,v) \
        out_dword_masked_ns(HWIO_TCSR_SPARE_REG1_ADDR,m,v,HWIO_TCSR_SPARE_REG1_IN)
#define HWIO_TCSR_SPARE_REG1_SPARE_REG1_BMSK                                                                          0xffffffff
#define HWIO_TCSR_SPARE_REG1_SPARE_REG1_SHFT                                                                                 0x0

#define HWIO_TCSR_SPARE_REG2_ADDR                                                                                     (TCSR_TCSR_REGS_REG_BASE      + 0x0000a04c)
#define HWIO_TCSR_SPARE_REG2_OFFS                                                                                     (TCSR_TCSR_REGS_REG_BASE_OFFS + 0x0000a04c)
#define HWIO_TCSR_SPARE_REG2_RMSK                                                                                           0xff
#define HWIO_TCSR_SPARE_REG2_IN          \
        in_dword_masked(HWIO_TCSR_SPARE_REG2_ADDR, HWIO_TCSR_SPARE_REG2_RMSK)
#define HWIO_TCSR_SPARE_REG2_INM(m)      \
        in_dword_masked(HWIO_TCSR_SPARE_REG2_ADDR, m)
#define HWIO_TCSR_SPARE_REG2_OUT(v)      \
        out_dword(HWIO_TCSR_SPARE_REG2_ADDR,v)
#define HWIO_TCSR_SPARE_REG2_OUTM(m,v) \
        out_dword_masked_ns(HWIO_TCSR_SPARE_REG2_ADDR,m,v,HWIO_TCSR_SPARE_REG2_IN)
#define HWIO_TCSR_SPARE_REG2_SPARE_REG2_BMSK                                                                                0xff
#define HWIO_TCSR_SPARE_REG2_SPARE_REG2_SHFT                                                                                 0x0

#define HWIO_TCSR_SPARE_REG3_ADDR                                                                                     (TCSR_TCSR_REGS_REG_BASE      + 0x0000a050)
#define HWIO_TCSR_SPARE_REG3_OFFS                                                                                     (TCSR_TCSR_REGS_REG_BASE_OFFS + 0x0000a050)
#define HWIO_TCSR_SPARE_REG3_RMSK                                                                                           0xff
#define HWIO_TCSR_SPARE_REG3_IN          \
        in_dword_masked(HWIO_TCSR_SPARE_REG3_ADDR, HWIO_TCSR_SPARE_REG3_RMSK)
#define HWIO_TCSR_SPARE_REG3_INM(m)      \
        in_dword_masked(HWIO_TCSR_SPARE_REG3_ADDR, m)
#define HWIO_TCSR_SPARE_REG3_OUT(v)      \
        out_dword(HWIO_TCSR_SPARE_REG3_ADDR,v)
#define HWIO_TCSR_SPARE_REG3_OUTM(m,v) \
        out_dword_masked_ns(HWIO_TCSR_SPARE_REG3_ADDR,m,v,HWIO_TCSR_SPARE_REG3_IN)
#define HWIO_TCSR_SPARE_REG3_SPARE_REG3_BMSK                                                                                0xff
#define HWIO_TCSR_SPARE_REG3_SPARE_REG3_SHFT                                                                                 0x0

#define HWIO_TCSR_SPARE_REG4_ADDR                                                                                     (TCSR_TCSR_REGS_REG_BASE      + 0x0000a054)
#define HWIO_TCSR_SPARE_REG4_OFFS                                                                                     (TCSR_TCSR_REGS_REG_BASE_OFFS + 0x0000a054)
#define HWIO_TCSR_SPARE_REG4_RMSK                                                                                           0xff
#define HWIO_TCSR_SPARE_REG4_IN          \
        in_dword_masked(HWIO_TCSR_SPARE_REG4_ADDR, HWIO_TCSR_SPARE_REG4_RMSK)
#define HWIO_TCSR_SPARE_REG4_INM(m)      \
        in_dword_masked(HWIO_TCSR_SPARE_REG4_ADDR, m)
#define HWIO_TCSR_SPARE_REG4_OUT(v)      \
        out_dword(HWIO_TCSR_SPARE_REG4_ADDR,v)
#define HWIO_TCSR_SPARE_REG4_OUTM(m,v) \
        out_dword_masked_ns(HWIO_TCSR_SPARE_REG4_ADDR,m,v,HWIO_TCSR_SPARE_REG4_IN)
#define HWIO_TCSR_SPARE_REG4_SPARE_REG4_BMSK                                                                                0xff
#define HWIO_TCSR_SPARE_REG4_SPARE_REG4_SHFT                                                                                 0x0

#define HWIO_TCSR_SPARE_REG5_ADDR                                                                                     (TCSR_TCSR_REGS_REG_BASE      + 0x0000a058)
#define HWIO_TCSR_SPARE_REG5_OFFS                                                                                     (TCSR_TCSR_REGS_REG_BASE_OFFS + 0x0000a058)
#define HWIO_TCSR_SPARE_REG5_RMSK                                                                                           0xff
#define HWIO_TCSR_SPARE_REG5_IN          \
        in_dword_masked(HWIO_TCSR_SPARE_REG5_ADDR, HWIO_TCSR_SPARE_REG5_RMSK)
#define HWIO_TCSR_SPARE_REG5_INM(m)      \
        in_dword_masked(HWIO_TCSR_SPARE_REG5_ADDR, m)
#define HWIO_TCSR_SPARE_REG5_OUT(v)      \
        out_dword(HWIO_TCSR_SPARE_REG5_ADDR,v)
#define HWIO_TCSR_SPARE_REG5_OUTM(m,v) \
        out_dword_masked_ns(HWIO_TCSR_SPARE_REG5_ADDR,m,v,HWIO_TCSR_SPARE_REG5_IN)
#define HWIO_TCSR_SPARE_REG5_SPARE_REG5_BMSK                                                                                0xff
#define HWIO_TCSR_SPARE_REG5_SPARE_REG5_SHFT                                                                                 0x0

#define HWIO_TCSR_SPARE_QGIC_INTERRUPTS_ADDR                                                                          (TCSR_TCSR_REGS_REG_BASE      + 0x0000a05c)
#define HWIO_TCSR_SPARE_QGIC_INTERRUPTS_OFFS                                                                          (TCSR_TCSR_REGS_REG_BASE_OFFS + 0x0000a05c)
#define HWIO_TCSR_SPARE_QGIC_INTERRUPTS_RMSK                                                                          0xffffffff
#define HWIO_TCSR_SPARE_QGIC_INTERRUPTS_IN          \
        in_dword_masked(HWIO_TCSR_SPARE_QGIC_INTERRUPTS_ADDR, HWIO_TCSR_SPARE_QGIC_INTERRUPTS_RMSK)
#define HWIO_TCSR_SPARE_QGIC_INTERRUPTS_INM(m)      \
        in_dword_masked(HWIO_TCSR_SPARE_QGIC_INTERRUPTS_ADDR, m)
#define HWIO_TCSR_SPARE_QGIC_INTERRUPTS_OUT(v)      \
        out_dword(HWIO_TCSR_SPARE_QGIC_INTERRUPTS_ADDR,v)
#define HWIO_TCSR_SPARE_QGIC_INTERRUPTS_OUTM(m,v) \
        out_dword_masked_ns(HWIO_TCSR_SPARE_QGIC_INTERRUPTS_ADDR,m,v,HWIO_TCSR_SPARE_QGIC_INTERRUPTS_IN)
#define HWIO_TCSR_SPARE_QGIC_INTERRUPTS_SPARE_QGIC_INTERRUPTS_BMSK                                                    0xffffffff
#define HWIO_TCSR_SPARE_QGIC_INTERRUPTS_SPARE_QGIC_INTERRUPTS_SHFT                                                           0x0

#define HWIO_TCSR_UFS_SCM_FAULT_IRQ_ADDR                                                                              (TCSR_TCSR_REGS_REG_BASE      + 0x0000a070)
#define HWIO_TCSR_UFS_SCM_FAULT_IRQ_OFFS                                                                              (TCSR_TCSR_REGS_REG_BASE_OFFS + 0x0000a070)
#define HWIO_TCSR_UFS_SCM_FAULT_IRQ_RMSK                                                                                     0x1
#define HWIO_TCSR_UFS_SCM_FAULT_IRQ_IN          \
        in_dword_masked(HWIO_TCSR_UFS_SCM_FAULT_IRQ_ADDR, HWIO_TCSR_UFS_SCM_FAULT_IRQ_RMSK)
#define HWIO_TCSR_UFS_SCM_FAULT_IRQ_INM(m)      \
        in_dword_masked(HWIO_TCSR_UFS_SCM_FAULT_IRQ_ADDR, m)
#define HWIO_TCSR_UFS_SCM_FAULT_IRQ_UFS_SCM_FAULT_IRQ_BMSK                                                                   0x1
#define HWIO_TCSR_UFS_SCM_FAULT_IRQ_UFS_SCM_FAULT_IRQ_SHFT                                                                   0x0

#define HWIO_TCSR_SDCC5_SCM_FAULT_IRQ_ADDR                                                                            (TCSR_TCSR_REGS_REG_BASE      + 0x0000a074)
#define HWIO_TCSR_SDCC5_SCM_FAULT_IRQ_OFFS                                                                            (TCSR_TCSR_REGS_REG_BASE_OFFS + 0x0000a074)
#define HWIO_TCSR_SDCC5_SCM_FAULT_IRQ_RMSK                                                                                   0x1
#define HWIO_TCSR_SDCC5_SCM_FAULT_IRQ_IN          \
        in_dword_masked(HWIO_TCSR_SDCC5_SCM_FAULT_IRQ_ADDR, HWIO_TCSR_SDCC5_SCM_FAULT_IRQ_RMSK)
#define HWIO_TCSR_SDCC5_SCM_FAULT_IRQ_INM(m)      \
        in_dword_masked(HWIO_TCSR_SDCC5_SCM_FAULT_IRQ_ADDR, m)
#define HWIO_TCSR_SDCC5_SCM_FAULT_IRQ_SDCC5_SCM_FAULT_IRQ_BMSK                                                               0x1
#define HWIO_TCSR_SDCC5_SCM_FAULT_IRQ_SDCC5_SCM_FAULT_IRQ_SHFT                                                               0x0

#define HWIO_TCSR_SDCC1_SCM_FAULT_IRQ_ADDR                                                                            (TCSR_TCSR_REGS_REG_BASE      + 0x0000a078)
#define HWIO_TCSR_SDCC1_SCM_FAULT_IRQ_OFFS                                                                            (TCSR_TCSR_REGS_REG_BASE_OFFS + 0x0000a078)
#define HWIO_TCSR_SDCC1_SCM_FAULT_IRQ_RMSK                                                                                   0x1
#define HWIO_TCSR_SDCC1_SCM_FAULT_IRQ_IN          \
        in_dword_masked(HWIO_TCSR_SDCC1_SCM_FAULT_IRQ_ADDR, HWIO_TCSR_SDCC1_SCM_FAULT_IRQ_RMSK)
#define HWIO_TCSR_SDCC1_SCM_FAULT_IRQ_INM(m)      \
        in_dword_masked(HWIO_TCSR_SDCC1_SCM_FAULT_IRQ_ADDR, m)
#define HWIO_TCSR_SDCC1_SCM_FAULT_IRQ_SDCC1_SCM_FAULT_IRQ_BMSK                                                               0x1
#define HWIO_TCSR_SDCC1_SCM_FAULT_IRQ_SDCC1_SCM_FAULT_IRQ_SHFT                                                               0x0

#define HWIO_TCSR_TCSR_IPA_WARMBOOT_EN_ADDR                                                                           (TCSR_TCSR_REGS_REG_BASE      + 0x0000a100)
#define HWIO_TCSR_TCSR_IPA_WARMBOOT_EN_OFFS                                                                           (TCSR_TCSR_REGS_REG_BASE_OFFS + 0x0000a100)
#define HWIO_TCSR_TCSR_IPA_WARMBOOT_EN_RMSK                                                                                  0x1
#define HWIO_TCSR_TCSR_IPA_WARMBOOT_EN_IN          \
        in_dword_masked(HWIO_TCSR_TCSR_IPA_WARMBOOT_EN_ADDR, HWIO_TCSR_TCSR_IPA_WARMBOOT_EN_RMSK)
#define HWIO_TCSR_TCSR_IPA_WARMBOOT_EN_INM(m)      \
        in_dword_masked(HWIO_TCSR_TCSR_IPA_WARMBOOT_EN_ADDR, m)
#define HWIO_TCSR_TCSR_IPA_WARMBOOT_EN_OUT(v)      \
        out_dword(HWIO_TCSR_TCSR_IPA_WARMBOOT_EN_ADDR,v)
#define HWIO_TCSR_TCSR_IPA_WARMBOOT_EN_OUTM(m,v) \
        out_dword_masked_ns(HWIO_TCSR_TCSR_IPA_WARMBOOT_EN_ADDR,m,v,HWIO_TCSR_TCSR_IPA_WARMBOOT_EN_IN)
#define HWIO_TCSR_TCSR_IPA_WARMBOOT_EN_TCSR_IPA_WARMBOOT_EN_BMSK                                                             0x1
#define HWIO_TCSR_TCSR_IPA_WARMBOOT_EN_TCSR_IPA_WARMBOOT_EN_SHFT                                                             0x0

#define HWIO_TCSR_PHSS_UART_MSS_INT_SEL_0_ADDR                                                                        (TCSR_TCSR_REGS_REG_BASE      + 0x0000b00c)
#define HWIO_TCSR_PHSS_UART_MSS_INT_SEL_0_OFFS                                                                        (TCSR_TCSR_REGS_REG_BASE_OFFS + 0x0000b00c)
#define HWIO_TCSR_PHSS_UART_MSS_INT_SEL_0_RMSK                                                                               0xf
#define HWIO_TCSR_PHSS_UART_MSS_INT_SEL_0_IN          \
        in_dword_masked(HWIO_TCSR_PHSS_UART_MSS_INT_SEL_0_ADDR, HWIO_TCSR_PHSS_UART_MSS_INT_SEL_0_RMSK)
#define HWIO_TCSR_PHSS_UART_MSS_INT_SEL_0_INM(m)      \
        in_dword_masked(HWIO_TCSR_PHSS_UART_MSS_INT_SEL_0_ADDR, m)
#define HWIO_TCSR_PHSS_UART_MSS_INT_SEL_0_OUT(v)      \
        out_dword(HWIO_TCSR_PHSS_UART_MSS_INT_SEL_0_ADDR,v)
#define HWIO_TCSR_PHSS_UART_MSS_INT_SEL_0_OUTM(m,v) \
        out_dword_masked_ns(HWIO_TCSR_PHSS_UART_MSS_INT_SEL_0_ADDR,m,v,HWIO_TCSR_PHSS_UART_MSS_INT_SEL_0_IN)
#define HWIO_TCSR_PHSS_UART_MSS_INT_SEL_0_MSS_BLSP1_UART_4_IRQ_ENABLE_BMSK                                                   0x8
#define HWIO_TCSR_PHSS_UART_MSS_INT_SEL_0_MSS_BLSP1_UART_4_IRQ_ENABLE_SHFT                                                   0x3
#define HWIO_TCSR_PHSS_UART_MSS_INT_SEL_0_MSS_BLSP1_UART_3_IRQ_ENABLE_BMSK                                                   0x4
#define HWIO_TCSR_PHSS_UART_MSS_INT_SEL_0_MSS_BLSP1_UART_3_IRQ_ENABLE_SHFT                                                   0x2
#define HWIO_TCSR_PHSS_UART_MSS_INT_SEL_0_MSS_BLSP1_UART_2_IRQ_ENABLE_BMSK                                                   0x2
#define HWIO_TCSR_PHSS_UART_MSS_INT_SEL_0_MSS_BLSP1_UART_2_IRQ_ENABLE_SHFT                                                   0x1
#define HWIO_TCSR_PHSS_UART_MSS_INT_SEL_0_MSS_BLSP1_UART_1_IRQ_ENABLE_BMSK                                                   0x1
#define HWIO_TCSR_PHSS_UART_MSS_INT_SEL_0_MSS_BLSP1_UART_1_IRQ_ENABLE_SHFT                                                   0x0

#define HWIO_TCSR_PHSS_UART_MSS_INT_SEL_1_ADDR                                                                        (TCSR_TCSR_REGS_REG_BASE      + 0x0000b010)
#define HWIO_TCSR_PHSS_UART_MSS_INT_SEL_1_OFFS                                                                        (TCSR_TCSR_REGS_REG_BASE_OFFS + 0x0000b010)
#define HWIO_TCSR_PHSS_UART_MSS_INT_SEL_1_RMSK                                                                               0xf
#define HWIO_TCSR_PHSS_UART_MSS_INT_SEL_1_IN          \
        in_dword_masked(HWIO_TCSR_PHSS_UART_MSS_INT_SEL_1_ADDR, HWIO_TCSR_PHSS_UART_MSS_INT_SEL_1_RMSK)
#define HWIO_TCSR_PHSS_UART_MSS_INT_SEL_1_INM(m)      \
        in_dword_masked(HWIO_TCSR_PHSS_UART_MSS_INT_SEL_1_ADDR, m)
#define HWIO_TCSR_PHSS_UART_MSS_INT_SEL_1_OUT(v)      \
        out_dword(HWIO_TCSR_PHSS_UART_MSS_INT_SEL_1_ADDR,v)
#define HWIO_TCSR_PHSS_UART_MSS_INT_SEL_1_OUTM(m,v) \
        out_dword_masked_ns(HWIO_TCSR_PHSS_UART_MSS_INT_SEL_1_ADDR,m,v,HWIO_TCSR_PHSS_UART_MSS_INT_SEL_1_IN)
#define HWIO_TCSR_PHSS_UART_MSS_INT_SEL_1_MSS_BLSP1_UART_4_IRQ_ENABLE_BMSK                                                   0x8
#define HWIO_TCSR_PHSS_UART_MSS_INT_SEL_1_MSS_BLSP1_UART_4_IRQ_ENABLE_SHFT                                                   0x3
#define HWIO_TCSR_PHSS_UART_MSS_INT_SEL_1_MSS_BLSP1_UART_3_IRQ_ENABLE_BMSK                                                   0x4
#define HWIO_TCSR_PHSS_UART_MSS_INT_SEL_1_MSS_BLSP1_UART_3_IRQ_ENABLE_SHFT                                                   0x2
#define HWIO_TCSR_PHSS_UART_MSS_INT_SEL_1_MSS_BLSP1_UART_2_IRQ_ENABLE_BMSK                                                   0x2
#define HWIO_TCSR_PHSS_UART_MSS_INT_SEL_1_MSS_BLSP1_UART_2_IRQ_ENABLE_SHFT                                                   0x1
#define HWIO_TCSR_PHSS_UART_MSS_INT_SEL_1_MSS_BLSP1_UART_1_IRQ_ENABLE_BMSK                                                   0x1
#define HWIO_TCSR_PHSS_UART_MSS_INT_SEL_1_MSS_BLSP1_UART_1_IRQ_ENABLE_SHFT                                                   0x0

#define HWIO_TCSR_PHSS_QUP_MSS_INT_SEL_0_ADDR                                                                         (TCSR_TCSR_REGS_REG_BASE      + 0x0000b014)
#define HWIO_TCSR_PHSS_QUP_MSS_INT_SEL_0_OFFS                                                                         (TCSR_TCSR_REGS_REG_BASE_OFFS + 0x0000b014)
#define HWIO_TCSR_PHSS_QUP_MSS_INT_SEL_0_RMSK                                                                                0xf
#define HWIO_TCSR_PHSS_QUP_MSS_INT_SEL_0_IN          \
        in_dword_masked(HWIO_TCSR_PHSS_QUP_MSS_INT_SEL_0_ADDR, HWIO_TCSR_PHSS_QUP_MSS_INT_SEL_0_RMSK)
#define HWIO_TCSR_PHSS_QUP_MSS_INT_SEL_0_INM(m)      \
        in_dword_masked(HWIO_TCSR_PHSS_QUP_MSS_INT_SEL_0_ADDR, m)
#define HWIO_TCSR_PHSS_QUP_MSS_INT_SEL_0_OUT(v)      \
        out_dword(HWIO_TCSR_PHSS_QUP_MSS_INT_SEL_0_ADDR,v)
#define HWIO_TCSR_PHSS_QUP_MSS_INT_SEL_0_OUTM(m,v) \
        out_dword_masked_ns(HWIO_TCSR_PHSS_QUP_MSS_INT_SEL_0_ADDR,m,v,HWIO_TCSR_PHSS_QUP_MSS_INT_SEL_0_IN)
#define HWIO_TCSR_PHSS_QUP_MSS_INT_SEL_0_MSS_BLSP1_QUP_4_IRQ_ENABLE_BMSK                                                     0x8
#define HWIO_TCSR_PHSS_QUP_MSS_INT_SEL_0_MSS_BLSP1_QUP_4_IRQ_ENABLE_SHFT                                                     0x3
#define HWIO_TCSR_PHSS_QUP_MSS_INT_SEL_0_MSS_BLSP1_QUP_3_IRQ_ENABLE_BMSK                                                     0x4
#define HWIO_TCSR_PHSS_QUP_MSS_INT_SEL_0_MSS_BLSP1_QUP_3_IRQ_ENABLE_SHFT                                                     0x2
#define HWIO_TCSR_PHSS_QUP_MSS_INT_SEL_0_MSS_BLSP1_QUP_2_IRQ_ENABLE_BMSK                                                     0x2
#define HWIO_TCSR_PHSS_QUP_MSS_INT_SEL_0_MSS_BLSP1_QUP_2_IRQ_ENABLE_SHFT                                                     0x1
#define HWIO_TCSR_PHSS_QUP_MSS_INT_SEL_0_MSS_BLSP1_QUP_1_IRQ_ENABLE_BMSK                                                     0x1
#define HWIO_TCSR_PHSS_QUP_MSS_INT_SEL_0_MSS_BLSP1_QUP_1_IRQ_ENABLE_SHFT                                                     0x0

#define HWIO_TCSR_PHSS_QUP_MSS_INT_SEL_1_ADDR                                                                         (TCSR_TCSR_REGS_REG_BASE      + 0x0000b018)
#define HWIO_TCSR_PHSS_QUP_MSS_INT_SEL_1_OFFS                                                                         (TCSR_TCSR_REGS_REG_BASE_OFFS + 0x0000b018)
#define HWIO_TCSR_PHSS_QUP_MSS_INT_SEL_1_RMSK                                                                                0xf
#define HWIO_TCSR_PHSS_QUP_MSS_INT_SEL_1_IN          \
        in_dword_masked(HWIO_TCSR_PHSS_QUP_MSS_INT_SEL_1_ADDR, HWIO_TCSR_PHSS_QUP_MSS_INT_SEL_1_RMSK)
#define HWIO_TCSR_PHSS_QUP_MSS_INT_SEL_1_INM(m)      \
        in_dword_masked(HWIO_TCSR_PHSS_QUP_MSS_INT_SEL_1_ADDR, m)
#define HWIO_TCSR_PHSS_QUP_MSS_INT_SEL_1_OUT(v)      \
        out_dword(HWIO_TCSR_PHSS_QUP_MSS_INT_SEL_1_ADDR,v)
#define HWIO_TCSR_PHSS_QUP_MSS_INT_SEL_1_OUTM(m,v) \
        out_dword_masked_ns(HWIO_TCSR_PHSS_QUP_MSS_INT_SEL_1_ADDR,m,v,HWIO_TCSR_PHSS_QUP_MSS_INT_SEL_1_IN)
#define HWIO_TCSR_PHSS_QUP_MSS_INT_SEL_1_MSS_BLSP1_QUP_4_IRQ_ENABLE_BMSK                                                     0x8
#define HWIO_TCSR_PHSS_QUP_MSS_INT_SEL_1_MSS_BLSP1_QUP_4_IRQ_ENABLE_SHFT                                                     0x3
#define HWIO_TCSR_PHSS_QUP_MSS_INT_SEL_1_MSS_BLSP1_QUP_3_IRQ_ENABLE_BMSK                                                     0x4
#define HWIO_TCSR_PHSS_QUP_MSS_INT_SEL_1_MSS_BLSP1_QUP_3_IRQ_ENABLE_SHFT                                                     0x2
#define HWIO_TCSR_PHSS_QUP_MSS_INT_SEL_1_MSS_BLSP1_QUP_2_IRQ_ENABLE_BMSK                                                     0x2
#define HWIO_TCSR_PHSS_QUP_MSS_INT_SEL_1_MSS_BLSP1_QUP_2_IRQ_ENABLE_SHFT                                                     0x1
#define HWIO_TCSR_PHSS_QUP_MSS_INT_SEL_1_MSS_BLSP1_QUP_1_IRQ_ENABLE_BMSK                                                     0x1
#define HWIO_TCSR_PHSS_QUP_MSS_INT_SEL_1_MSS_BLSP1_QUP_1_IRQ_ENABLE_SHFT                                                     0x0

#define HWIO_TCSR_TOP_QUPV3_0_MSS_INT_SEL_OUT_0_ADDR                                                                  (TCSR_TCSR_REGS_REG_BASE      + 0x0000b050)
#define HWIO_TCSR_TOP_QUPV3_0_MSS_INT_SEL_OUT_0_OFFS                                                                  (TCSR_TCSR_REGS_REG_BASE_OFFS + 0x0000b050)
#define HWIO_TCSR_TOP_QUPV3_0_MSS_INT_SEL_OUT_0_RMSK                                                                    0x1fffff
#define HWIO_TCSR_TOP_QUPV3_0_MSS_INT_SEL_OUT_0_IN          \
        in_dword_masked(HWIO_TCSR_TOP_QUPV3_0_MSS_INT_SEL_OUT_0_ADDR, HWIO_TCSR_TOP_QUPV3_0_MSS_INT_SEL_OUT_0_RMSK)
#define HWIO_TCSR_TOP_QUPV3_0_MSS_INT_SEL_OUT_0_INM(m)      \
        in_dword_masked(HWIO_TCSR_TOP_QUPV3_0_MSS_INT_SEL_OUT_0_ADDR, m)
#define HWIO_TCSR_TOP_QUPV3_0_MSS_INT_SEL_OUT_0_OUT(v)      \
        out_dword(HWIO_TCSR_TOP_QUPV3_0_MSS_INT_SEL_OUT_0_ADDR,v)
#define HWIO_TCSR_TOP_QUPV3_0_MSS_INT_SEL_OUT_0_OUTM(m,v) \
        out_dword_masked_ns(HWIO_TCSR_TOP_QUPV3_0_MSS_INT_SEL_OUT_0_ADDR,m,v,HWIO_TCSR_TOP_QUPV3_0_MSS_INT_SEL_OUT_0_IN)
#define HWIO_TCSR_TOP_QUPV3_0_MSS_INT_SEL_OUT_0_TOP_QUPV3_0_GSI_MSS_INT_SEL_OUT_0_BMSK                                  0x1fff00
#define HWIO_TCSR_TOP_QUPV3_0_MSS_INT_SEL_OUT_0_TOP_QUPV3_0_GSI_MSS_INT_SEL_OUT_0_SHFT                                       0x8
#define HWIO_TCSR_TOP_QUPV3_0_MSS_INT_SEL_OUT_0_TOP_QUPV3_0_SE_MSS_INT_SEL_OUT_0_BMSK                                       0xff
#define HWIO_TCSR_TOP_QUPV3_0_MSS_INT_SEL_OUT_0_TOP_QUPV3_0_SE_MSS_INT_SEL_OUT_0_SHFT                                        0x0

#define HWIO_TCSR_TOP_QUPV3_1_MSS_INT_SEL_OUT_0_ADDR                                                                  (TCSR_TCSR_REGS_REG_BASE      + 0x0000b054)
#define HWIO_TCSR_TOP_QUPV3_1_MSS_INT_SEL_OUT_0_OFFS                                                                  (TCSR_TCSR_REGS_REG_BASE_OFFS + 0x0000b054)
#define HWIO_TCSR_TOP_QUPV3_1_MSS_INT_SEL_OUT_0_RMSK                                                                    0x1fffff
#define HWIO_TCSR_TOP_QUPV3_1_MSS_INT_SEL_OUT_0_IN          \
        in_dword_masked(HWIO_TCSR_TOP_QUPV3_1_MSS_INT_SEL_OUT_0_ADDR, HWIO_TCSR_TOP_QUPV3_1_MSS_INT_SEL_OUT_0_RMSK)
#define HWIO_TCSR_TOP_QUPV3_1_MSS_INT_SEL_OUT_0_INM(m)      \
        in_dword_masked(HWIO_TCSR_TOP_QUPV3_1_MSS_INT_SEL_OUT_0_ADDR, m)
#define HWIO_TCSR_TOP_QUPV3_1_MSS_INT_SEL_OUT_0_OUT(v)      \
        out_dword(HWIO_TCSR_TOP_QUPV3_1_MSS_INT_SEL_OUT_0_ADDR,v)
#define HWIO_TCSR_TOP_QUPV3_1_MSS_INT_SEL_OUT_0_OUTM(m,v) \
        out_dword_masked_ns(HWIO_TCSR_TOP_QUPV3_1_MSS_INT_SEL_OUT_0_ADDR,m,v,HWIO_TCSR_TOP_QUPV3_1_MSS_INT_SEL_OUT_0_IN)
#define HWIO_TCSR_TOP_QUPV3_1_MSS_INT_SEL_OUT_0_TOP_QUPV3_1_GSI_MSS_INT_SEL_OUT_0_BMSK                                  0x1fff00
#define HWIO_TCSR_TOP_QUPV3_1_MSS_INT_SEL_OUT_0_TOP_QUPV3_1_GSI_MSS_INT_SEL_OUT_0_SHFT                                       0x8
#define HWIO_TCSR_TOP_QUPV3_1_MSS_INT_SEL_OUT_0_TOP_QUPV3_1_SE_MSS_INT_SEL_OUT_0_BMSK                                       0xff
#define HWIO_TCSR_TOP_QUPV3_1_MSS_INT_SEL_OUT_0_TOP_QUPV3_1_SE_MSS_INT_SEL_OUT_0_SHFT                                        0x0

#define HWIO_TCSR_TOP_QUPV3_0_MSS_INT_SEL_OUT_1_ADDR                                                                  (TCSR_TCSR_REGS_REG_BASE      + 0x0000b058)
#define HWIO_TCSR_TOP_QUPV3_0_MSS_INT_SEL_OUT_1_OFFS                                                                  (TCSR_TCSR_REGS_REG_BASE_OFFS + 0x0000b058)
#define HWIO_TCSR_TOP_QUPV3_0_MSS_INT_SEL_OUT_1_RMSK                                                                    0x1fffff
#define HWIO_TCSR_TOP_QUPV3_0_MSS_INT_SEL_OUT_1_IN          \
        in_dword_masked(HWIO_TCSR_TOP_QUPV3_0_MSS_INT_SEL_OUT_1_ADDR, HWIO_TCSR_TOP_QUPV3_0_MSS_INT_SEL_OUT_1_RMSK)
#define HWIO_TCSR_TOP_QUPV3_0_MSS_INT_SEL_OUT_1_INM(m)      \
        in_dword_masked(HWIO_TCSR_TOP_QUPV3_0_MSS_INT_SEL_OUT_1_ADDR, m)
#define HWIO_TCSR_TOP_QUPV3_0_MSS_INT_SEL_OUT_1_OUT(v)      \
        out_dword(HWIO_TCSR_TOP_QUPV3_0_MSS_INT_SEL_OUT_1_ADDR,v)
#define HWIO_TCSR_TOP_QUPV3_0_MSS_INT_SEL_OUT_1_OUTM(m,v) \
        out_dword_masked_ns(HWIO_TCSR_TOP_QUPV3_0_MSS_INT_SEL_OUT_1_ADDR,m,v,HWIO_TCSR_TOP_QUPV3_0_MSS_INT_SEL_OUT_1_IN)
#define HWIO_TCSR_TOP_QUPV3_0_MSS_INT_SEL_OUT_1_TOP_QUPV3_0_GSI_MSS_INT_SEL_OUT_1_BMSK                                  0x1fff00
#define HWIO_TCSR_TOP_QUPV3_0_MSS_INT_SEL_OUT_1_TOP_QUPV3_0_GSI_MSS_INT_SEL_OUT_1_SHFT                                       0x8
#define HWIO_TCSR_TOP_QUPV3_0_MSS_INT_SEL_OUT_1_TOP_QUPV3_0_SE_MSS_INT_SEL_OUT_1_BMSK                                       0xff
#define HWIO_TCSR_TOP_QUPV3_0_MSS_INT_SEL_OUT_1_TOP_QUPV3_0_SE_MSS_INT_SEL_OUT_1_SHFT                                        0x0

#define HWIO_TCSR_TOP_QUPV3_1_MSS_INT_SEL_OUT_1_ADDR                                                                  (TCSR_TCSR_REGS_REG_BASE      + 0x0000b05c)
#define HWIO_TCSR_TOP_QUPV3_1_MSS_INT_SEL_OUT_1_OFFS                                                                  (TCSR_TCSR_REGS_REG_BASE_OFFS + 0x0000b05c)
#define HWIO_TCSR_TOP_QUPV3_1_MSS_INT_SEL_OUT_1_RMSK                                                                    0x1fffff
#define HWIO_TCSR_TOP_QUPV3_1_MSS_INT_SEL_OUT_1_IN          \
        in_dword_masked(HWIO_TCSR_TOP_QUPV3_1_MSS_INT_SEL_OUT_1_ADDR, HWIO_TCSR_TOP_QUPV3_1_MSS_INT_SEL_OUT_1_RMSK)
#define HWIO_TCSR_TOP_QUPV3_1_MSS_INT_SEL_OUT_1_INM(m)      \
        in_dword_masked(HWIO_TCSR_TOP_QUPV3_1_MSS_INT_SEL_OUT_1_ADDR, m)
#define HWIO_TCSR_TOP_QUPV3_1_MSS_INT_SEL_OUT_1_OUT(v)      \
        out_dword(HWIO_TCSR_TOP_QUPV3_1_MSS_INT_SEL_OUT_1_ADDR,v)
#define HWIO_TCSR_TOP_QUPV3_1_MSS_INT_SEL_OUT_1_OUTM(m,v) \
        out_dword_masked_ns(HWIO_TCSR_TOP_QUPV3_1_MSS_INT_SEL_OUT_1_ADDR,m,v,HWIO_TCSR_TOP_QUPV3_1_MSS_INT_SEL_OUT_1_IN)
#define HWIO_TCSR_TOP_QUPV3_1_MSS_INT_SEL_OUT_1_TOP_QUPV3_1_GSI_MSS_INT_SEL_OUT_1_BMSK                                  0x1fff00
#define HWIO_TCSR_TOP_QUPV3_1_MSS_INT_SEL_OUT_1_TOP_QUPV3_1_GSI_MSS_INT_SEL_OUT_1_SHFT                                       0x8
#define HWIO_TCSR_TOP_QUPV3_1_MSS_INT_SEL_OUT_1_TOP_QUPV3_1_SE_MSS_INT_SEL_OUT_1_BMSK                                       0xff
#define HWIO_TCSR_TOP_QUPV3_1_MSS_INT_SEL_OUT_1_TOP_QUPV3_1_SE_MSS_INT_SEL_OUT_1_SHFT                                        0x0

#define HWIO_TCSR_COPSS_USB_CONTROL_WITH_JDR_ADDR                                                                     (TCSR_TCSR_REGS_REG_BASE      + 0x0000b204)
#define HWIO_TCSR_COPSS_USB_CONTROL_WITH_JDR_OFFS                                                                     (TCSR_TCSR_REGS_REG_BASE_OFFS + 0x0000b204)
#define HWIO_TCSR_COPSS_USB_CONTROL_WITH_JDR_RMSK                                                                     0xffffffff
#define HWIO_TCSR_COPSS_USB_CONTROL_WITH_JDR_IN          \
        in_dword_masked(HWIO_TCSR_COPSS_USB_CONTROL_WITH_JDR_ADDR, HWIO_TCSR_COPSS_USB_CONTROL_WITH_JDR_RMSK)
#define HWIO_TCSR_COPSS_USB_CONTROL_WITH_JDR_INM(m)      \
        in_dword_masked(HWIO_TCSR_COPSS_USB_CONTROL_WITH_JDR_ADDR, m)
#define HWIO_TCSR_COPSS_USB_CONTROL_WITH_JDR_OUT(v)      \
        out_dword(HWIO_TCSR_COPSS_USB_CONTROL_WITH_JDR_ADDR,v)
#define HWIO_TCSR_COPSS_USB_CONTROL_WITH_JDR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_TCSR_COPSS_USB_CONTROL_WITH_JDR_ADDR,m,v,HWIO_TCSR_COPSS_USB_CONTROL_WITH_JDR_IN)
#define HWIO_TCSR_COPSS_USB_CONTROL_WITH_JDR_COPSS_USB_CONTROL_WITH_JDR_BMSK                                          0xffffffff
#define HWIO_TCSR_COPSS_USB_CONTROL_WITH_JDR_COPSS_USB_CONTROL_WITH_JDR_SHFT                                                 0x0

#define HWIO_TCSR_UFS_SATA_CONTROL_WITH_JDR_ADDR                                                                      (TCSR_TCSR_REGS_REG_BASE      + 0x0000b20c)
#define HWIO_TCSR_UFS_SATA_CONTROL_WITH_JDR_OFFS                                                                      (TCSR_TCSR_REGS_REG_BASE_OFFS + 0x0000b20c)
#define HWIO_TCSR_UFS_SATA_CONTROL_WITH_JDR_RMSK                                                                             0x1
#define HWIO_TCSR_UFS_SATA_CONTROL_WITH_JDR_IN          \
        in_dword_masked(HWIO_TCSR_UFS_SATA_CONTROL_WITH_JDR_ADDR, HWIO_TCSR_UFS_SATA_CONTROL_WITH_JDR_RMSK)
#define HWIO_TCSR_UFS_SATA_CONTROL_WITH_JDR_INM(m)      \
        in_dword_masked(HWIO_TCSR_UFS_SATA_CONTROL_WITH_JDR_ADDR, m)
#define HWIO_TCSR_UFS_SATA_CONTROL_WITH_JDR_OUT(v)      \
        out_dword(HWIO_TCSR_UFS_SATA_CONTROL_WITH_JDR_ADDR,v)
#define HWIO_TCSR_UFS_SATA_CONTROL_WITH_JDR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_TCSR_UFS_SATA_CONTROL_WITH_JDR_ADDR,m,v,HWIO_TCSR_UFS_SATA_CONTROL_WITH_JDR_IN)
#define HWIO_TCSR_UFS_SATA_CONTROL_WITH_JDR_UFS_SATA_CTRL_SEL_BMSK                                                           0x1
#define HWIO_TCSR_UFS_SATA_CONTROL_WITH_JDR_UFS_SATA_CTRL_SEL_SHFT                                                           0x0

#define HWIO_TCSR_DDR_SS_DEBUG_BUS_SEL_ADDR                                                                           (TCSR_TCSR_REGS_REG_BASE      + 0x0000b220)
#define HWIO_TCSR_DDR_SS_DEBUG_BUS_SEL_OFFS                                                                           (TCSR_TCSR_REGS_REG_BASE_OFFS + 0x0000b220)
#define HWIO_TCSR_DDR_SS_DEBUG_BUS_SEL_RMSK                                                                                0x3ff
#define HWIO_TCSR_DDR_SS_DEBUG_BUS_SEL_IN          \
        in_dword_masked(HWIO_TCSR_DDR_SS_DEBUG_BUS_SEL_ADDR, HWIO_TCSR_DDR_SS_DEBUG_BUS_SEL_RMSK)
#define HWIO_TCSR_DDR_SS_DEBUG_BUS_SEL_INM(m)      \
        in_dword_masked(HWIO_TCSR_DDR_SS_DEBUG_BUS_SEL_ADDR, m)
#define HWIO_TCSR_DDR_SS_DEBUG_BUS_SEL_OUT(v)      \
        out_dword(HWIO_TCSR_DDR_SS_DEBUG_BUS_SEL_ADDR,v)
#define HWIO_TCSR_DDR_SS_DEBUG_BUS_SEL_OUTM(m,v) \
        out_dword_masked_ns(HWIO_TCSR_DDR_SS_DEBUG_BUS_SEL_ADDR,m,v,HWIO_TCSR_DDR_SS_DEBUG_BUS_SEL_IN)
#define HWIO_TCSR_DDR_SS_DEBUG_BUS_SEL_DDR_SS_DEBUG_BUS_SEL_BMSK                                                           0x3ff
#define HWIO_TCSR_DDR_SS_DEBUG_BUS_SEL_DDR_SS_DEBUG_BUS_SEL_SHFT                                                             0x0

#define HWIO_TCSR_TCSR_USB_PHY_VLS_CLAMP_ADDR                                                                         (TCSR_TCSR_REGS_REG_BASE      + 0x0000b244)
#define HWIO_TCSR_TCSR_USB_PHY_VLS_CLAMP_OFFS                                                                         (TCSR_TCSR_REGS_REG_BASE_OFFS + 0x0000b244)
#define HWIO_TCSR_TCSR_USB_PHY_VLS_CLAMP_RMSK                                                                         0xffffffff
#define HWIO_TCSR_TCSR_USB_PHY_VLS_CLAMP_IN          \
        in_dword_masked(HWIO_TCSR_TCSR_USB_PHY_VLS_CLAMP_ADDR, HWIO_TCSR_TCSR_USB_PHY_VLS_CLAMP_RMSK)
#define HWIO_TCSR_TCSR_USB_PHY_VLS_CLAMP_INM(m)      \
        in_dword_masked(HWIO_TCSR_TCSR_USB_PHY_VLS_CLAMP_ADDR, m)
#define HWIO_TCSR_TCSR_USB_PHY_VLS_CLAMP_OUT(v)      \
        out_dword(HWIO_TCSR_TCSR_USB_PHY_VLS_CLAMP_ADDR,v)
#define HWIO_TCSR_TCSR_USB_PHY_VLS_CLAMP_OUTM(m,v) \
        out_dword_masked_ns(HWIO_TCSR_TCSR_USB_PHY_VLS_CLAMP_ADDR,m,v,HWIO_TCSR_TCSR_USB_PHY_VLS_CLAMP_IN)
#define HWIO_TCSR_TCSR_USB_PHY_VLS_CLAMP_TCSR_USB_PHY_VLS_CLAMP_BMSK                                                  0xffffffff
#define HWIO_TCSR_TCSR_USB_PHY_VLS_CLAMP_TCSR_USB_PHY_VLS_CLAMP_SHFT                                                         0x0

#define HWIO_TCSR_PCIE_SLAVE_NOC_TIMEOUT_EN_ADDR                                                                      (TCSR_TCSR_REGS_REG_BASE      + 0x0000b250)
#define HWIO_TCSR_PCIE_SLAVE_NOC_TIMEOUT_EN_OFFS                                                                      (TCSR_TCSR_REGS_REG_BASE_OFFS + 0x0000b250)
#define HWIO_TCSR_PCIE_SLAVE_NOC_TIMEOUT_EN_RMSK                                                                             0x1
#define HWIO_TCSR_PCIE_SLAVE_NOC_TIMEOUT_EN_IN          \
        in_dword_masked(HWIO_TCSR_PCIE_SLAVE_NOC_TIMEOUT_EN_ADDR, HWIO_TCSR_PCIE_SLAVE_NOC_TIMEOUT_EN_RMSK)
#define HWIO_TCSR_PCIE_SLAVE_NOC_TIMEOUT_EN_INM(m)      \
        in_dword_masked(HWIO_TCSR_PCIE_SLAVE_NOC_TIMEOUT_EN_ADDR, m)
#define HWIO_TCSR_PCIE_SLAVE_NOC_TIMEOUT_EN_OUT(v)      \
        out_dword(HWIO_TCSR_PCIE_SLAVE_NOC_TIMEOUT_EN_ADDR,v)
#define HWIO_TCSR_PCIE_SLAVE_NOC_TIMEOUT_EN_OUTM(m,v) \
        out_dword_masked_ns(HWIO_TCSR_PCIE_SLAVE_NOC_TIMEOUT_EN_ADDR,m,v,HWIO_TCSR_PCIE_SLAVE_NOC_TIMEOUT_EN_IN)
#define HWIO_TCSR_PCIE_SLAVE_NOC_TIMEOUT_EN_PCIE_SLAVE_NOC_TIMEOUT_EN_BMSK                                                   0x1
#define HWIO_TCSR_PCIE_SLAVE_NOC_TIMEOUT_EN_PCIE_SLAVE_NOC_TIMEOUT_EN_SHFT                                                   0x0

#define HWIO_TCSR_PCIE_SLAVE_NOC_TIMEOUT_CLK_DIV_ADDR                                                                 (TCSR_TCSR_REGS_REG_BASE      + 0x0000b254)
#define HWIO_TCSR_PCIE_SLAVE_NOC_TIMEOUT_CLK_DIV_OFFS                                                                 (TCSR_TCSR_REGS_REG_BASE_OFFS + 0x0000b254)
#define HWIO_TCSR_PCIE_SLAVE_NOC_TIMEOUT_CLK_DIV_RMSK                                                                        0x7
#define HWIO_TCSR_PCIE_SLAVE_NOC_TIMEOUT_CLK_DIV_IN          \
        in_dword_masked(HWIO_TCSR_PCIE_SLAVE_NOC_TIMEOUT_CLK_DIV_ADDR, HWIO_TCSR_PCIE_SLAVE_NOC_TIMEOUT_CLK_DIV_RMSK)
#define HWIO_TCSR_PCIE_SLAVE_NOC_TIMEOUT_CLK_DIV_INM(m)      \
        in_dword_masked(HWIO_TCSR_PCIE_SLAVE_NOC_TIMEOUT_CLK_DIV_ADDR, m)
#define HWIO_TCSR_PCIE_SLAVE_NOC_TIMEOUT_CLK_DIV_OUT(v)      \
        out_dword(HWIO_TCSR_PCIE_SLAVE_NOC_TIMEOUT_CLK_DIV_ADDR,v)
#define HWIO_TCSR_PCIE_SLAVE_NOC_TIMEOUT_CLK_DIV_OUTM(m,v) \
        out_dword_masked_ns(HWIO_TCSR_PCIE_SLAVE_NOC_TIMEOUT_CLK_DIV_ADDR,m,v,HWIO_TCSR_PCIE_SLAVE_NOC_TIMEOUT_CLK_DIV_IN)
#define HWIO_TCSR_PCIE_SLAVE_NOC_TIMEOUT_CLK_DIV_PCIE_SLAVE_NOC_TIMEOUT_CLK_DIV_BMSK                                         0x7
#define HWIO_TCSR_PCIE_SLAVE_NOC_TIMEOUT_CLK_DIV_PCIE_SLAVE_NOC_TIMEOUT_CLK_DIV_SHFT                                         0x0

#define HWIO_TCSR_PCIE_PERST_EN_ADDR                                                                                  (TCSR_TCSR_REGS_REG_BASE      + 0x0000b258)
#define HWIO_TCSR_PCIE_PERST_EN_OFFS                                                                                  (TCSR_TCSR_REGS_REG_BASE_OFFS + 0x0000b258)
#define HWIO_TCSR_PCIE_PERST_EN_RMSK                                                                                         0x1
#define HWIO_TCSR_PCIE_PERST_EN_IN          \
        in_dword_masked(HWIO_TCSR_PCIE_PERST_EN_ADDR, HWIO_TCSR_PCIE_PERST_EN_RMSK)
#define HWIO_TCSR_PCIE_PERST_EN_INM(m)      \
        in_dword_masked(HWIO_TCSR_PCIE_PERST_EN_ADDR, m)
#define HWIO_TCSR_PCIE_PERST_EN_OUT(v)      \
        out_dword(HWIO_TCSR_PCIE_PERST_EN_ADDR,v)
#define HWIO_TCSR_PCIE_PERST_EN_OUTM(m,v) \
        out_dword_masked_ns(HWIO_TCSR_PCIE_PERST_EN_ADDR,m,v,HWIO_TCSR_PCIE_PERST_EN_IN)
#define HWIO_TCSR_PCIE_PERST_EN_PCIE_PERST_EN_BMSK                                                                           0x1
#define HWIO_TCSR_PCIE_PERST_EN_PCIE_PERST_EN_SHFT                                                                           0x0

#define HWIO_TCSR_PCIE_IRQ_EN_ADDR                                                                                    (TCSR_TCSR_REGS_REG_BASE      + 0x0000b25c)
#define HWIO_TCSR_PCIE_IRQ_EN_OFFS                                                                                    (TCSR_TCSR_REGS_REG_BASE_OFFS + 0x0000b25c)
#define HWIO_TCSR_PCIE_IRQ_EN_RMSK                                                                                           0x1
#define HWIO_TCSR_PCIE_IRQ_EN_IN          \
        in_dword_masked(HWIO_TCSR_PCIE_IRQ_EN_ADDR, HWIO_TCSR_PCIE_IRQ_EN_RMSK)
#define HWIO_TCSR_PCIE_IRQ_EN_INM(m)      \
        in_dword_masked(HWIO_TCSR_PCIE_IRQ_EN_ADDR, m)
#define HWIO_TCSR_PCIE_IRQ_EN_OUT(v)      \
        out_dword(HWIO_TCSR_PCIE_IRQ_EN_ADDR,v)
#define HWIO_TCSR_PCIE_IRQ_EN_OUTM(m,v) \
        out_dword_masked_ns(HWIO_TCSR_PCIE_IRQ_EN_ADDR,m,v,HWIO_TCSR_PCIE_IRQ_EN_IN)
#define HWIO_TCSR_PCIE_IRQ_EN_PCIE_IRQ_EN_BMSK                                                                               0x1
#define HWIO_TCSR_PCIE_IRQ_EN_PCIE_IRQ_EN_SHFT                                                                               0x0

#define HWIO_TCSR_TBU_BYPASS_ENABLE_ADDR                                                                              (TCSR_TCSR_REGS_REG_BASE      + 0x0000b260)
#define HWIO_TCSR_TBU_BYPASS_ENABLE_OFFS                                                                              (TCSR_TCSR_REGS_REG_BASE_OFFS + 0x0000b260)
#define HWIO_TCSR_TBU_BYPASS_ENABLE_RMSK                                                                                    0xff
#define HWIO_TCSR_TBU_BYPASS_ENABLE_IN          \
        in_dword_masked(HWIO_TCSR_TBU_BYPASS_ENABLE_ADDR, HWIO_TCSR_TBU_BYPASS_ENABLE_RMSK)
#define HWIO_TCSR_TBU_BYPASS_ENABLE_INM(m)      \
        in_dword_masked(HWIO_TCSR_TBU_BYPASS_ENABLE_ADDR, m)
#define HWIO_TCSR_TBU_BYPASS_ENABLE_OUT(v)      \
        out_dword(HWIO_TCSR_TBU_BYPASS_ENABLE_ADDR,v)
#define HWIO_TCSR_TBU_BYPASS_ENABLE_OUTM(m,v) \
        out_dword_masked_ns(HWIO_TCSR_TBU_BYPASS_ENABLE_ADDR,m,v,HWIO_TCSR_TBU_BYPASS_ENABLE_IN)
#define HWIO_TCSR_TBU_BYPASS_ENABLE_TBU_BYPASS_ENABLE_BMSK                                                                  0xff
#define HWIO_TCSR_TBU_BYPASS_ENABLE_TBU_BYPASS_ENABLE_SHFT                                                                   0x0

#define HWIO_TCSR_QUSB2PHY_CX2PX_LVL_SHIFT_KEEPER_ADDR                                                                (TCSR_TCSR_REGS_REG_BASE      + 0x0000b264)
#define HWIO_TCSR_QUSB2PHY_CX2PX_LVL_SHIFT_KEEPER_OFFS                                                                (TCSR_TCSR_REGS_REG_BASE_OFFS + 0x0000b264)
#define HWIO_TCSR_QUSB2PHY_CX2PX_LVL_SHIFT_KEEPER_RMSK                                                                       0x1
#define HWIO_TCSR_QUSB2PHY_CX2PX_LVL_SHIFT_KEEPER_IN          \
        in_dword_masked(HWIO_TCSR_QUSB2PHY_CX2PX_LVL_SHIFT_KEEPER_ADDR, HWIO_TCSR_QUSB2PHY_CX2PX_LVL_SHIFT_KEEPER_RMSK)
#define HWIO_TCSR_QUSB2PHY_CX2PX_LVL_SHIFT_KEEPER_INM(m)      \
        in_dword_masked(HWIO_TCSR_QUSB2PHY_CX2PX_LVL_SHIFT_KEEPER_ADDR, m)
#define HWIO_TCSR_QUSB2PHY_CX2PX_LVL_SHIFT_KEEPER_OUT(v)      \
        out_dword(HWIO_TCSR_QUSB2PHY_CX2PX_LVL_SHIFT_KEEPER_ADDR,v)
#define HWIO_TCSR_QUSB2PHY_CX2PX_LVL_SHIFT_KEEPER_OUTM(m,v) \
        out_dword_masked_ns(HWIO_TCSR_QUSB2PHY_CX2PX_LVL_SHIFT_KEEPER_ADDR,m,v,HWIO_TCSR_QUSB2PHY_CX2PX_LVL_SHIFT_KEEPER_IN)
#define HWIO_TCSR_QUSB2PHY_CX2PX_LVL_SHIFT_KEEPER_QUSB2PHY_CX2PX_LVL_SHIFT_KEEPER_BMSK                                       0x1
#define HWIO_TCSR_QUSB2PHY_CX2PX_LVL_SHIFT_KEEPER_QUSB2PHY_CX2PX_LVL_SHIFT_KEEPER_SHFT                                       0x0

#define HWIO_TCSR_REFGEN_QLINK_ENABLE_ADDR                                                                            (TCSR_TCSR_REGS_REG_BASE      + 0x0000b268)
#define HWIO_TCSR_REFGEN_QLINK_ENABLE_OFFS                                                                            (TCSR_TCSR_REGS_REG_BASE_OFFS + 0x0000b268)
#define HWIO_TCSR_REFGEN_QLINK_ENABLE_RMSK                                                                                   0x1
#define HWIO_TCSR_REFGEN_QLINK_ENABLE_IN          \
        in_dword_masked(HWIO_TCSR_REFGEN_QLINK_ENABLE_ADDR, HWIO_TCSR_REFGEN_QLINK_ENABLE_RMSK)
#define HWIO_TCSR_REFGEN_QLINK_ENABLE_INM(m)      \
        in_dword_masked(HWIO_TCSR_REFGEN_QLINK_ENABLE_ADDR, m)
#define HWIO_TCSR_REFGEN_QLINK_ENABLE_OUT(v)      \
        out_dword(HWIO_TCSR_REFGEN_QLINK_ENABLE_ADDR,v)
#define HWIO_TCSR_REFGEN_QLINK_ENABLE_OUTM(m,v) \
        out_dword_masked_ns(HWIO_TCSR_REFGEN_QLINK_ENABLE_ADDR,m,v,HWIO_TCSR_REFGEN_QLINK_ENABLE_IN)
#define HWIO_TCSR_REFGEN_QLINK_ENABLE_REFGEN_QLINK_ENABLE_BMSK                                                               0x1
#define HWIO_TCSR_REFGEN_QLINK_ENABLE_REFGEN_QLINK_ENABLE_SHFT                                                               0x0

#define HWIO_TCSR_TIC_CNOC_NS_ADDR                                                                                    (TCSR_TCSR_REGS_REG_BASE      + 0x0000b3e0)
#define HWIO_TCSR_TIC_CNOC_NS_OFFS                                                                                    (TCSR_TCSR_REGS_REG_BASE_OFFS + 0x0000b3e0)
#define HWIO_TCSR_TIC_CNOC_NS_RMSK                                                                                           0x1
#define HWIO_TCSR_TIC_CNOC_NS_IN          \
        in_dword_masked(HWIO_TCSR_TIC_CNOC_NS_ADDR, HWIO_TCSR_TIC_CNOC_NS_RMSK)
#define HWIO_TCSR_TIC_CNOC_NS_INM(m)      \
        in_dword_masked(HWIO_TCSR_TIC_CNOC_NS_ADDR, m)
#define HWIO_TCSR_TIC_CNOC_NS_OUT(v)      \
        out_dword(HWIO_TCSR_TIC_CNOC_NS_ADDR,v)
#define HWIO_TCSR_TIC_CNOC_NS_OUTM(m,v) \
        out_dword_masked_ns(HWIO_TCSR_TIC_CNOC_NS_ADDR,m,v,HWIO_TCSR_TIC_CNOC_NS_IN)
#define HWIO_TCSR_TIC_CNOC_NS_TIC_CNOC_NS_BMSK                                                                               0x1
#define HWIO_TCSR_TIC_CNOC_NS_TIC_CNOC_NS_SHFT                                                                               0x0

#define HWIO_TCSR_CONN_BOX_SPARE_0_ADDR                                                                               (TCSR_TCSR_REGS_REG_BASE      + 0x0000b3e4)
#define HWIO_TCSR_CONN_BOX_SPARE_0_OFFS                                                                               (TCSR_TCSR_REGS_REG_BASE_OFFS + 0x0000b3e4)
#define HWIO_TCSR_CONN_BOX_SPARE_0_RMSK                                                                               0xffffffff
#define HWIO_TCSR_CONN_BOX_SPARE_0_IN          \
        in_dword_masked(HWIO_TCSR_CONN_BOX_SPARE_0_ADDR, HWIO_TCSR_CONN_BOX_SPARE_0_RMSK)
#define HWIO_TCSR_CONN_BOX_SPARE_0_INM(m)      \
        in_dword_masked(HWIO_TCSR_CONN_BOX_SPARE_0_ADDR, m)
#define HWIO_TCSR_CONN_BOX_SPARE_0_OUT(v)      \
        out_dword(HWIO_TCSR_CONN_BOX_SPARE_0_ADDR,v)
#define HWIO_TCSR_CONN_BOX_SPARE_0_OUTM(m,v) \
        out_dword_masked_ns(HWIO_TCSR_CONN_BOX_SPARE_0_ADDR,m,v,HWIO_TCSR_CONN_BOX_SPARE_0_IN)
#define HWIO_TCSR_CONN_BOX_SPARE_0_CONN_BOX_SPARE_0_BMSK                                                              0xffffffff
#define HWIO_TCSR_CONN_BOX_SPARE_0_CONN_BOX_SPARE_0_SHFT                                                                     0x0

#define HWIO_TCSR_CONN_BOX_SPARE_1_ADDR                                                                               (TCSR_TCSR_REGS_REG_BASE      + 0x0000b3e8)
#define HWIO_TCSR_CONN_BOX_SPARE_1_OFFS                                                                               (TCSR_TCSR_REGS_REG_BASE_OFFS + 0x0000b3e8)
#define HWIO_TCSR_CONN_BOX_SPARE_1_RMSK                                                                               0xffffffff
#define HWIO_TCSR_CONN_BOX_SPARE_1_IN          \
        in_dword_masked(HWIO_TCSR_CONN_BOX_SPARE_1_ADDR, HWIO_TCSR_CONN_BOX_SPARE_1_RMSK)
#define HWIO_TCSR_CONN_BOX_SPARE_1_INM(m)      \
        in_dword_masked(HWIO_TCSR_CONN_BOX_SPARE_1_ADDR, m)
#define HWIO_TCSR_CONN_BOX_SPARE_1_OUT(v)      \
        out_dword(HWIO_TCSR_CONN_BOX_SPARE_1_ADDR,v)
#define HWIO_TCSR_CONN_BOX_SPARE_1_OUTM(m,v) \
        out_dword_masked_ns(HWIO_TCSR_CONN_BOX_SPARE_1_ADDR,m,v,HWIO_TCSR_CONN_BOX_SPARE_1_IN)
#define HWIO_TCSR_CONN_BOX_SPARE_1_CONN_BOX_SPARE_1_BMSK                                                              0xffffffff
#define HWIO_TCSR_CONN_BOX_SPARE_1_CONN_BOX_SPARE_1_SHFT                                                                     0x0

#define HWIO_TCSR_CONN_BOX_SPARE_2_ADDR                                                                               (TCSR_TCSR_REGS_REG_BASE      + 0x0000b3ec)
#define HWIO_TCSR_CONN_BOX_SPARE_2_OFFS                                                                               (TCSR_TCSR_REGS_REG_BASE_OFFS + 0x0000b3ec)
#define HWIO_TCSR_CONN_BOX_SPARE_2_RMSK                                                                               0xffffffff
#define HWIO_TCSR_CONN_BOX_SPARE_2_IN          \
        in_dword_masked(HWIO_TCSR_CONN_BOX_SPARE_2_ADDR, HWIO_TCSR_CONN_BOX_SPARE_2_RMSK)
#define HWIO_TCSR_CONN_BOX_SPARE_2_INM(m)      \
        in_dword_masked(HWIO_TCSR_CONN_BOX_SPARE_2_ADDR, m)
#define HWIO_TCSR_CONN_BOX_SPARE_2_OUT(v)      \
        out_dword(HWIO_TCSR_CONN_BOX_SPARE_2_ADDR,v)
#define HWIO_TCSR_CONN_BOX_SPARE_2_OUTM(m,v) \
        out_dword_masked_ns(HWIO_TCSR_CONN_BOX_SPARE_2_ADDR,m,v,HWIO_TCSR_CONN_BOX_SPARE_2_IN)
#define HWIO_TCSR_CONN_BOX_SPARE_2_CONN_BOX_SPARE_2_BMSK                                                              0xffffffff
#define HWIO_TCSR_CONN_BOX_SPARE_2_CONN_BOX_SPARE_2_SHFT                                                                     0x0

#define HWIO_TCSR_Q6SS_COREPWR_ON_ADDR                                                                                (TCSR_TCSR_REGS_REG_BASE      + 0x0000b3f0)
#define HWIO_TCSR_Q6SS_COREPWR_ON_OFFS                                                                                (TCSR_TCSR_REGS_REG_BASE_OFFS + 0x0000b3f0)
#define HWIO_TCSR_Q6SS_COREPWR_ON_RMSK                                                                                       0x1
#define HWIO_TCSR_Q6SS_COREPWR_ON_IN          \
        in_dword_masked(HWIO_TCSR_Q6SS_COREPWR_ON_ADDR, HWIO_TCSR_Q6SS_COREPWR_ON_RMSK)
#define HWIO_TCSR_Q6SS_COREPWR_ON_INM(m)      \
        in_dword_masked(HWIO_TCSR_Q6SS_COREPWR_ON_ADDR, m)
#define HWIO_TCSR_Q6SS_COREPWR_ON_Q6SS_COREPWR_ON_BMSK                                                                       0x1
#define HWIO_TCSR_Q6SS_COREPWR_ON_Q6SS_COREPWR_ON_SHFT                                                                       0x0

#define HWIO_TCSR_VQ6SS_COREPWR_ON_ADDR                                                                               (TCSR_TCSR_REGS_REG_BASE      + 0x0000b3f4)
#define HWIO_TCSR_VQ6SS_COREPWR_ON_OFFS                                                                               (TCSR_TCSR_REGS_REG_BASE_OFFS + 0x0000b3f4)
#define HWIO_TCSR_VQ6SS_COREPWR_ON_RMSK                                                                                      0x1
#define HWIO_TCSR_VQ6SS_COREPWR_ON_IN          \
        in_dword_masked(HWIO_TCSR_VQ6SS_COREPWR_ON_ADDR, HWIO_TCSR_VQ6SS_COREPWR_ON_RMSK)
#define HWIO_TCSR_VQ6SS_COREPWR_ON_INM(m)      \
        in_dword_masked(HWIO_TCSR_VQ6SS_COREPWR_ON_ADDR, m)
#define HWIO_TCSR_VQ6SS_COREPWR_ON_VQ6SS_COREPWR_ON_BMSK                                                                     0x1
#define HWIO_TCSR_VQ6SS_COREPWR_ON_VQ6SS_COREPWR_ON_SHFT                                                                     0x0

#define HWIO_TCSR_TCSR_USB_PHY_SEC_VLS_CLAMP_ADDR                                                                     (TCSR_TCSR_REGS_REG_BASE      + 0x0000bff0)
#define HWIO_TCSR_TCSR_USB_PHY_SEC_VLS_CLAMP_OFFS                                                                     (TCSR_TCSR_REGS_REG_BASE_OFFS + 0x0000bff0)
#define HWIO_TCSR_TCSR_USB_PHY_SEC_VLS_CLAMP_RMSK                                                                     0xffffffff
#define HWIO_TCSR_TCSR_USB_PHY_SEC_VLS_CLAMP_IN          \
        in_dword_masked(HWIO_TCSR_TCSR_USB_PHY_SEC_VLS_CLAMP_ADDR, HWIO_TCSR_TCSR_USB_PHY_SEC_VLS_CLAMP_RMSK)
#define HWIO_TCSR_TCSR_USB_PHY_SEC_VLS_CLAMP_INM(m)      \
        in_dword_masked(HWIO_TCSR_TCSR_USB_PHY_SEC_VLS_CLAMP_ADDR, m)
#define HWIO_TCSR_TCSR_USB_PHY_SEC_VLS_CLAMP_OUT(v)      \
        out_dword(HWIO_TCSR_TCSR_USB_PHY_SEC_VLS_CLAMP_ADDR,v)
#define HWIO_TCSR_TCSR_USB_PHY_SEC_VLS_CLAMP_OUTM(m,v) \
        out_dword_masked_ns(HWIO_TCSR_TCSR_USB_PHY_SEC_VLS_CLAMP_ADDR,m,v,HWIO_TCSR_TCSR_USB_PHY_SEC_VLS_CLAMP_IN)
#define HWIO_TCSR_TCSR_USB_PHY_SEC_VLS_CLAMP_TCSR_USB_PHY_SEC_VLS_CLAMP_BMSK                                          0xffffffff
#define HWIO_TCSR_TCSR_USB_PHY_SEC_VLS_CLAMP_TCSR_USB_PHY_SEC_VLS_CLAMP_SHFT                                                 0x0

#define HWIO_TCSR_COMPILER_VDDDDRA_ACC_0_ADDR                                                                         (TCSR_TCSR_REGS_REG_BASE      + 0x0000d010)
#define HWIO_TCSR_COMPILER_VDDDDRA_ACC_0_OFFS                                                                         (TCSR_TCSR_REGS_REG_BASE_OFFS + 0x0000d010)
#define HWIO_TCSR_COMPILER_VDDDDRA_ACC_0_RMSK                                                                         0xffffffff
#define HWIO_TCSR_COMPILER_VDDDDRA_ACC_0_IN          \
        in_dword_masked(HWIO_TCSR_COMPILER_VDDDDRA_ACC_0_ADDR, HWIO_TCSR_COMPILER_VDDDDRA_ACC_0_RMSK)
#define HWIO_TCSR_COMPILER_VDDDDRA_ACC_0_INM(m)      \
        in_dword_masked(HWIO_TCSR_COMPILER_VDDDDRA_ACC_0_ADDR, m)
#define HWIO_TCSR_COMPILER_VDDDDRA_ACC_0_COMPILER_VDDDDRA_ACC_0_BMSK                                                  0xffffffff
#define HWIO_TCSR_COMPILER_VDDDDRA_ACC_0_COMPILER_VDDDDRA_ACC_0_SHFT                                                         0x0

#define HWIO_TCSR_COMPILER_VDDDDRA_ACC_1_ADDR                                                                         (TCSR_TCSR_REGS_REG_BASE      + 0x0000d014)
#define HWIO_TCSR_COMPILER_VDDDDRA_ACC_1_OFFS                                                                         (TCSR_TCSR_REGS_REG_BASE_OFFS + 0x0000d014)
#define HWIO_TCSR_COMPILER_VDDDDRA_ACC_1_RMSK                                                                         0xffffffff
#define HWIO_TCSR_COMPILER_VDDDDRA_ACC_1_IN          \
        in_dword_masked(HWIO_TCSR_COMPILER_VDDDDRA_ACC_1_ADDR, HWIO_TCSR_COMPILER_VDDDDRA_ACC_1_RMSK)
#define HWIO_TCSR_COMPILER_VDDDDRA_ACC_1_INM(m)      \
        in_dword_masked(HWIO_TCSR_COMPILER_VDDDDRA_ACC_1_ADDR, m)
#define HWIO_TCSR_COMPILER_VDDDDRA_ACC_1_COMPILER_VDDDDRA_ACC_1_BMSK                                                  0xffffffff
#define HWIO_TCSR_COMPILER_VDDDDRA_ACC_1_COMPILER_VDDDDRA_ACC_1_SHFT                                                         0x0

#define HWIO_TCSR_COMPILER_VDDCX_ACC_0_ADDR                                                                           (TCSR_TCSR_REGS_REG_BASE      + 0x0000d080)
#define HWIO_TCSR_COMPILER_VDDCX_ACC_0_OFFS                                                                           (TCSR_TCSR_REGS_REG_BASE_OFFS + 0x0000d080)
#define HWIO_TCSR_COMPILER_VDDCX_ACC_0_RMSK                                                                           0xffffffff
#define HWIO_TCSR_COMPILER_VDDCX_ACC_0_IN          \
        in_dword_masked(HWIO_TCSR_COMPILER_VDDCX_ACC_0_ADDR, HWIO_TCSR_COMPILER_VDDCX_ACC_0_RMSK)
#define HWIO_TCSR_COMPILER_VDDCX_ACC_0_INM(m)      \
        in_dword_masked(HWIO_TCSR_COMPILER_VDDCX_ACC_0_ADDR, m)
#define HWIO_TCSR_COMPILER_VDDCX_ACC_0_COMPILER_VDDCX_ACC_0_BMSK                                                      0xffffffff
#define HWIO_TCSR_COMPILER_VDDCX_ACC_0_COMPILER_VDDCX_ACC_0_SHFT                                                             0x0

#define HWIO_TCSR_COMPILER_VDDCX_ACC_1_ADDR                                                                           (TCSR_TCSR_REGS_REG_BASE      + 0x0000d084)
#define HWIO_TCSR_COMPILER_VDDCX_ACC_1_OFFS                                                                           (TCSR_TCSR_REGS_REG_BASE_OFFS + 0x0000d084)
#define HWIO_TCSR_COMPILER_VDDCX_ACC_1_RMSK                                                                           0xffffffff
#define HWIO_TCSR_COMPILER_VDDCX_ACC_1_IN          \
        in_dword_masked(HWIO_TCSR_COMPILER_VDDCX_ACC_1_ADDR, HWIO_TCSR_COMPILER_VDDCX_ACC_1_RMSK)
#define HWIO_TCSR_COMPILER_VDDCX_ACC_1_INM(m)      \
        in_dword_masked(HWIO_TCSR_COMPILER_VDDCX_ACC_1_ADDR, m)
#define HWIO_TCSR_COMPILER_VDDCX_ACC_1_COMPILER_VDDCX_ACC_1_BMSK                                                      0xffffffff
#define HWIO_TCSR_COMPILER_VDDCX_ACC_1_COMPILER_VDDCX_ACC_1_SHFT                                                             0x0

#define HWIO_TCSR_COMPILER_VDDMSS_ACC_0_ADDR                                                                          (TCSR_TCSR_REGS_REG_BASE      + 0x0000d1c0)
#define HWIO_TCSR_COMPILER_VDDMSS_ACC_0_OFFS                                                                          (TCSR_TCSR_REGS_REG_BASE_OFFS + 0x0000d1c0)
#define HWIO_TCSR_COMPILER_VDDMSS_ACC_0_RMSK                                                                          0xffffffff
#define HWIO_TCSR_COMPILER_VDDMSS_ACC_0_IN          \
        in_dword_masked(HWIO_TCSR_COMPILER_VDDMSS_ACC_0_ADDR, HWIO_TCSR_COMPILER_VDDMSS_ACC_0_RMSK)
#define HWIO_TCSR_COMPILER_VDDMSS_ACC_0_INM(m)      \
        in_dword_masked(HWIO_TCSR_COMPILER_VDDMSS_ACC_0_ADDR, m)
#define HWIO_TCSR_COMPILER_VDDMSS_ACC_0_COMPILER_VDDMSS_ACC_0_BMSK                                                    0xffffffff
#define HWIO_TCSR_COMPILER_VDDMSS_ACC_0_COMPILER_VDDMSS_ACC_0_SHFT                                                           0x0

#define HWIO_TCSR_COMPILER_VDDMSS_ACC_1_ADDR                                                                          (TCSR_TCSR_REGS_REG_BASE      + 0x0000d1c4)
#define HWIO_TCSR_COMPILER_VDDMSS_ACC_1_OFFS                                                                          (TCSR_TCSR_REGS_REG_BASE_OFFS + 0x0000d1c4)
#define HWIO_TCSR_COMPILER_VDDMSS_ACC_1_RMSK                                                                          0xffffffff
#define HWIO_TCSR_COMPILER_VDDMSS_ACC_1_IN          \
        in_dword_masked(HWIO_TCSR_COMPILER_VDDMSS_ACC_1_ADDR, HWIO_TCSR_COMPILER_VDDMSS_ACC_1_RMSK)
#define HWIO_TCSR_COMPILER_VDDMSS_ACC_1_INM(m)      \
        in_dword_masked(HWIO_TCSR_COMPILER_VDDMSS_ACC_1_ADDR, m)
#define HWIO_TCSR_COMPILER_VDDMSS_ACC_1_COMPILER_VDDMSS_ACC_1_BMSK                                                    0xffffffff
#define HWIO_TCSR_COMPILER_VDDMSS_ACC_1_COMPILER_VDDMSS_ACC_1_SHFT                                                           0x0

#define HWIO_TCSR_TIMEOUT_INTR_RPM_ENABLE_ADDR                                                                        (TCSR_TCSR_REGS_REG_BASE      + 0x00027000)
#define HWIO_TCSR_TIMEOUT_INTR_RPM_ENABLE_OFFS                                                                        (TCSR_TCSR_REGS_REG_BASE_OFFS + 0x00027000)
#define HWIO_TCSR_TIMEOUT_INTR_RPM_ENABLE_RMSK                                                                        0xffffffff
#define HWIO_TCSR_TIMEOUT_INTR_RPM_ENABLE_IN          \
        in_dword_masked(HWIO_TCSR_TIMEOUT_INTR_RPM_ENABLE_ADDR, HWIO_TCSR_TIMEOUT_INTR_RPM_ENABLE_RMSK)
#define HWIO_TCSR_TIMEOUT_INTR_RPM_ENABLE_INM(m)      \
        in_dword_masked(HWIO_TCSR_TIMEOUT_INTR_RPM_ENABLE_ADDR, m)
#define HWIO_TCSR_TIMEOUT_INTR_RPM_ENABLE_OUT(v)      \
        out_dword(HWIO_TCSR_TIMEOUT_INTR_RPM_ENABLE_ADDR,v)
#define HWIO_TCSR_TIMEOUT_INTR_RPM_ENABLE_OUTM(m,v) \
        out_dword_masked_ns(HWIO_TCSR_TIMEOUT_INTR_RPM_ENABLE_ADDR,m,v,HWIO_TCSR_TIMEOUT_INTR_RPM_ENABLE_IN)
#define HWIO_TCSR_TIMEOUT_INTR_RPM_ENABLE_ULTAUDIO_AHBI_TIMEOUT_SLAVE_IRQ_ENABLE_BMSK                                      0x800
#define HWIO_TCSR_TIMEOUT_INTR_RPM_ENABLE_ULTAUDIO_AHBI_TIMEOUT_SLAVE_IRQ_ENABLE_SHFT                                        0xb
#define HWIO_TCSR_TIMEOUT_INTR_RPM_ENABLE_MSS_CONFIG_TIMEOUT_SLAVE_IRQ_ENABLE_BMSK                                         0x400
#define HWIO_TCSR_TIMEOUT_INTR_RPM_ENABLE_MSS_CONFIG_TIMEOUT_SLAVE_IRQ_ENABLE_SHFT                                           0xa

#define HWIO_TCSR_TIMEOUT_INTR_HMSS_ENABLE_ADDR                                                                       (TCSR_TCSR_REGS_REG_BASE      + 0x00028000)
#define HWIO_TCSR_TIMEOUT_INTR_HMSS_ENABLE_OFFS                                                                       (TCSR_TCSR_REGS_REG_BASE_OFFS + 0x00028000)
#define HWIO_TCSR_TIMEOUT_INTR_HMSS_ENABLE_RMSK                                                                       0xffffffff
#define HWIO_TCSR_TIMEOUT_INTR_HMSS_ENABLE_IN          \
        in_dword_masked(HWIO_TCSR_TIMEOUT_INTR_HMSS_ENABLE_ADDR, HWIO_TCSR_TIMEOUT_INTR_HMSS_ENABLE_RMSK)
#define HWIO_TCSR_TIMEOUT_INTR_HMSS_ENABLE_INM(m)      \
        in_dword_masked(HWIO_TCSR_TIMEOUT_INTR_HMSS_ENABLE_ADDR, m)
#define HWIO_TCSR_TIMEOUT_INTR_HMSS_ENABLE_OUT(v)      \
        out_dword(HWIO_TCSR_TIMEOUT_INTR_HMSS_ENABLE_ADDR,v)
#define HWIO_TCSR_TIMEOUT_INTR_HMSS_ENABLE_OUTM(m,v) \
        out_dword_masked_ns(HWIO_TCSR_TIMEOUT_INTR_HMSS_ENABLE_ADDR,m,v,HWIO_TCSR_TIMEOUT_INTR_HMSS_ENABLE_IN)
#define HWIO_TCSR_TIMEOUT_INTR_HMSS_ENABLE_ULTAUDIO_AHBI_TIMEOUT_SLAVE_IRQ_ENABLE_BMSK                                     0x800
#define HWIO_TCSR_TIMEOUT_INTR_HMSS_ENABLE_ULTAUDIO_AHBI_TIMEOUT_SLAVE_IRQ_ENABLE_SHFT                                       0xb
#define HWIO_TCSR_TIMEOUT_INTR_HMSS_ENABLE_MSS_CONFIG_TIMEOUT_SLAVE_IRQ_ENABLE_BMSK                                        0x400
#define HWIO_TCSR_TIMEOUT_INTR_HMSS_ENABLE_MSS_CONFIG_TIMEOUT_SLAVE_IRQ_ENABLE_SHFT                                          0xa

#define HWIO_TCSR_TIMEOUT_INTR_LPASS_ENABLE_ADDR                                                                      (TCSR_TCSR_REGS_REG_BASE      + 0x00029000)
#define HWIO_TCSR_TIMEOUT_INTR_LPASS_ENABLE_OFFS                                                                      (TCSR_TCSR_REGS_REG_BASE_OFFS + 0x00029000)
#define HWIO_TCSR_TIMEOUT_INTR_LPASS_ENABLE_RMSK                                                                      0xffffffff
#define HWIO_TCSR_TIMEOUT_INTR_LPASS_ENABLE_IN          \
        in_dword_masked(HWIO_TCSR_TIMEOUT_INTR_LPASS_ENABLE_ADDR, HWIO_TCSR_TIMEOUT_INTR_LPASS_ENABLE_RMSK)
#define HWIO_TCSR_TIMEOUT_INTR_LPASS_ENABLE_INM(m)      \
        in_dword_masked(HWIO_TCSR_TIMEOUT_INTR_LPASS_ENABLE_ADDR, m)
#define HWIO_TCSR_TIMEOUT_INTR_LPASS_ENABLE_OUT(v)      \
        out_dword(HWIO_TCSR_TIMEOUT_INTR_LPASS_ENABLE_ADDR,v)
#define HWIO_TCSR_TIMEOUT_INTR_LPASS_ENABLE_OUTM(m,v) \
        out_dword_masked_ns(HWIO_TCSR_TIMEOUT_INTR_LPASS_ENABLE_ADDR,m,v,HWIO_TCSR_TIMEOUT_INTR_LPASS_ENABLE_IN)
#define HWIO_TCSR_TIMEOUT_INTR_LPASS_ENABLE_WCSS_ECAHB_TSLV_INTR_ENABLE_BMSK                                           0x1000000
#define HWIO_TCSR_TIMEOUT_INTR_LPASS_ENABLE_WCSS_ECAHB_TSLV_INTR_ENABLE_SHFT                                                0x18
#define HWIO_TCSR_TIMEOUT_INTR_LPASS_ENABLE_LPASS_IRQ_OUT_AHB_TIMEOUT2_ENABLE_BMSK                                      0x800000
#define HWIO_TCSR_TIMEOUT_INTR_LPASS_ENABLE_LPASS_IRQ_OUT_AHB_TIMEOUT2_ENABLE_SHFT                                          0x17
#define HWIO_TCSR_TIMEOUT_INTR_LPASS_ENABLE_LPASS_IRQ_OUT_AHB_TIMEOUT1_ENABLE_BMSK                                      0x400000
#define HWIO_TCSR_TIMEOUT_INTR_LPASS_ENABLE_LPASS_IRQ_OUT_AHB_TIMEOUT1_ENABLE_SHFT                                          0x16
#define HWIO_TCSR_TIMEOUT_INTR_LPASS_ENABLE_LPASS_IRQ_OUT_AHB_TIMEOUT0_ENABLE_BMSK                                      0x200000
#define HWIO_TCSR_TIMEOUT_INTR_LPASS_ENABLE_LPASS_IRQ_OUT_AHB_TIMEOUT0_ENABLE_SHFT                                          0x15
#define HWIO_TCSR_TIMEOUT_INTR_LPASS_ENABLE_WCSS_TCSR_WAHB_TSLV_INTR_ENABLE_BMSK                                        0x100000
#define HWIO_TCSR_TIMEOUT_INTR_LPASS_ENABLE_WCSS_TCSR_WAHB_TSLV_INTR_ENABLE_SHFT                                            0x14
#define HWIO_TCSR_TIMEOUT_INTR_LPASS_ENABLE_TURING_IRQ_OUT_AHB_TIMEOUT0_ENABLE_BMSK                                      0x40000
#define HWIO_TCSR_TIMEOUT_INTR_LPASS_ENABLE_TURING_IRQ_OUT_AHB_TIMEOUT0_ENABLE_SHFT                                         0x12
#define HWIO_TCSR_TIMEOUT_INTR_LPASS_ENABLE_WCSS_TCSR_WAPB_TSLV_IRQ_ENABLE_BMSK                                            0x800
#define HWIO_TCSR_TIMEOUT_INTR_LPASS_ENABLE_WCSS_TCSR_WAPB_TSLV_IRQ_ENABLE_SHFT                                              0xb
#define HWIO_TCSR_TIMEOUT_INTR_LPASS_ENABLE_MSS_CONFIG_TIMEOUT_SLAVE_IRQ_ENABLE_BMSK                                       0x400
#define HWIO_TCSR_TIMEOUT_INTR_LPASS_ENABLE_MSS_CONFIG_TIMEOUT_SLAVE_IRQ_ENABLE_SHFT                                         0xa
#define HWIO_TCSR_TIMEOUT_INTR_LPASS_ENABLE_WCSS_TCSR_NOC_INTR_ENABLE_BMSK                                                  0x40
#define HWIO_TCSR_TIMEOUT_INTR_LPASS_ENABLE_WCSS_TCSR_NOC_INTR_ENABLE_SHFT                                                   0x6

#define HWIO_TCSR_TIMEOUT_INTR_MSS_ENABLE_ADDR                                                                        (TCSR_TCSR_REGS_REG_BASE      + 0x0002a000)
#define HWIO_TCSR_TIMEOUT_INTR_MSS_ENABLE_OFFS                                                                        (TCSR_TCSR_REGS_REG_BASE_OFFS + 0x0002a000)
#define HWIO_TCSR_TIMEOUT_INTR_MSS_ENABLE_RMSK                                                                        0xffffffff
#define HWIO_TCSR_TIMEOUT_INTR_MSS_ENABLE_IN          \
        in_dword_masked(HWIO_TCSR_TIMEOUT_INTR_MSS_ENABLE_ADDR, HWIO_TCSR_TIMEOUT_INTR_MSS_ENABLE_RMSK)
#define HWIO_TCSR_TIMEOUT_INTR_MSS_ENABLE_INM(m)      \
        in_dword_masked(HWIO_TCSR_TIMEOUT_INTR_MSS_ENABLE_ADDR, m)
#define HWIO_TCSR_TIMEOUT_INTR_MSS_ENABLE_OUT(v)      \
        out_dword(HWIO_TCSR_TIMEOUT_INTR_MSS_ENABLE_ADDR,v)
#define HWIO_TCSR_TIMEOUT_INTR_MSS_ENABLE_OUTM(m,v) \
        out_dword_masked_ns(HWIO_TCSR_TIMEOUT_INTR_MSS_ENABLE_ADDR,m,v,HWIO_TCSR_TIMEOUT_INTR_MSS_ENABLE_IN)
#define HWIO_TCSR_TIMEOUT_INTR_MSS_ENABLE_ULTAUDIO_AHBI_TIMEOUT_SLAVE_IRQ_ENABLE_BMSK                                      0x800
#define HWIO_TCSR_TIMEOUT_INTR_MSS_ENABLE_ULTAUDIO_AHBI_TIMEOUT_SLAVE_IRQ_ENABLE_SHFT                                        0xb
#define HWIO_TCSR_TIMEOUT_INTR_MSS_ENABLE_MSS_CONFIG_TIMEOUT_SLAVE_IRQ_ENABLE_BMSK                                         0x400
#define HWIO_TCSR_TIMEOUT_INTR_MSS_ENABLE_MSS_CONFIG_TIMEOUT_SLAVE_IRQ_ENABLE_SHFT                                           0xa

#define HWIO_TCSR_TIMEOUT_INTR_TURING_ENABLE_ADDR                                                                     (TCSR_TCSR_REGS_REG_BASE      + 0x0002b000)
#define HWIO_TCSR_TIMEOUT_INTR_TURING_ENABLE_OFFS                                                                     (TCSR_TCSR_REGS_REG_BASE_OFFS + 0x0002b000)
#define HWIO_TCSR_TIMEOUT_INTR_TURING_ENABLE_RMSK                                                                     0xffffffff
#define HWIO_TCSR_TIMEOUT_INTR_TURING_ENABLE_IN          \
        in_dword_masked(HWIO_TCSR_TIMEOUT_INTR_TURING_ENABLE_ADDR, HWIO_TCSR_TIMEOUT_INTR_TURING_ENABLE_RMSK)
#define HWIO_TCSR_TIMEOUT_INTR_TURING_ENABLE_INM(m)      \
        in_dword_masked(HWIO_TCSR_TIMEOUT_INTR_TURING_ENABLE_ADDR, m)
#define HWIO_TCSR_TIMEOUT_INTR_TURING_ENABLE_OUT(v)      \
        out_dword(HWIO_TCSR_TIMEOUT_INTR_TURING_ENABLE_ADDR,v)
#define HWIO_TCSR_TIMEOUT_INTR_TURING_ENABLE_OUTM(m,v) \
        out_dword_masked_ns(HWIO_TCSR_TIMEOUT_INTR_TURING_ENABLE_ADDR,m,v,HWIO_TCSR_TIMEOUT_INTR_TURING_ENABLE_IN)
#define HWIO_TCSR_TIMEOUT_INTR_TURING_ENABLE_WCSS_ECAHB_TSLV_INTR_ENABLE_BMSK                                          0x1000000
#define HWIO_TCSR_TIMEOUT_INTR_TURING_ENABLE_WCSS_ECAHB_TSLV_INTR_ENABLE_SHFT                                               0x18
#define HWIO_TCSR_TIMEOUT_INTR_TURING_ENABLE_LPASS_IRQ_OUT_AHB_TIMEOUT2_ENABLE_BMSK                                     0x800000
#define HWIO_TCSR_TIMEOUT_INTR_TURING_ENABLE_LPASS_IRQ_OUT_AHB_TIMEOUT2_ENABLE_SHFT                                         0x17
#define HWIO_TCSR_TIMEOUT_INTR_TURING_ENABLE_LPASS_IRQ_OUT_AHB_TIMEOUT1_ENABLE_BMSK                                     0x400000
#define HWIO_TCSR_TIMEOUT_INTR_TURING_ENABLE_LPASS_IRQ_OUT_AHB_TIMEOUT1_ENABLE_SHFT                                         0x16
#define HWIO_TCSR_TIMEOUT_INTR_TURING_ENABLE_LPASS_IRQ_OUT_AHB_TIMEOUT0_ENABLE_BMSK                                     0x200000
#define HWIO_TCSR_TIMEOUT_INTR_TURING_ENABLE_LPASS_IRQ_OUT_AHB_TIMEOUT0_ENABLE_SHFT                                         0x15
#define HWIO_TCSR_TIMEOUT_INTR_TURING_ENABLE_WCSS_TCSR_WAHB_TSLV_INTR_ENABLE_BMSK                                       0x100000
#define HWIO_TCSR_TIMEOUT_INTR_TURING_ENABLE_WCSS_TCSR_WAHB_TSLV_INTR_ENABLE_SHFT                                           0x14
#define HWIO_TCSR_TIMEOUT_INTR_TURING_ENABLE_TURING_IRQ_OUT_AHB_TIMEOUT0_ENABLE_BMSK                                     0x40000
#define HWIO_TCSR_TIMEOUT_INTR_TURING_ENABLE_TURING_IRQ_OUT_AHB_TIMEOUT0_ENABLE_SHFT                                        0x12
#define HWIO_TCSR_TIMEOUT_INTR_TURING_ENABLE_WCSS_TCSR_WAPB_TSLV_IRQ_ENABLE_BMSK                                           0x800
#define HWIO_TCSR_TIMEOUT_INTR_TURING_ENABLE_WCSS_TCSR_WAPB_TSLV_IRQ_ENABLE_SHFT                                             0xb
#define HWIO_TCSR_TIMEOUT_INTR_TURING_ENABLE_MSS_CONFIG_TIMEOUT_SLAVE_IRQ_ENABLE_BMSK                                      0x400
#define HWIO_TCSR_TIMEOUT_INTR_TURING_ENABLE_MSS_CONFIG_TIMEOUT_SLAVE_IRQ_ENABLE_SHFT                                        0xa
#define HWIO_TCSR_TIMEOUT_INTR_TURING_ENABLE_WCSS_TCSR_NOC_INTR_ENABLE_BMSK                                                 0x40
#define HWIO_TCSR_TIMEOUT_INTR_TURING_ENABLE_WCSS_TCSR_NOC_INTR_ENABLE_SHFT                                                  0x6

#define HWIO_TCSR_DIFFERENTIAL_TEST_CLOCK_1_ADDR                                                                      (TCSR_TCSR_REGS_REG_BASE      + 0x00016000)
#define HWIO_TCSR_DIFFERENTIAL_TEST_CLOCK_1_OFFS                                                                      (TCSR_TCSR_REGS_REG_BASE_OFFS + 0x00016000)
#define HWIO_TCSR_DIFFERENTIAL_TEST_CLOCK_1_RMSK                                                                             0x3
#define HWIO_TCSR_DIFFERENTIAL_TEST_CLOCK_1_IN          \
        in_dword_masked(HWIO_TCSR_DIFFERENTIAL_TEST_CLOCK_1_ADDR, HWIO_TCSR_DIFFERENTIAL_TEST_CLOCK_1_RMSK)
#define HWIO_TCSR_DIFFERENTIAL_TEST_CLOCK_1_INM(m)      \
        in_dword_masked(HWIO_TCSR_DIFFERENTIAL_TEST_CLOCK_1_ADDR, m)
#define HWIO_TCSR_DIFFERENTIAL_TEST_CLOCK_1_OUT(v)      \
        out_dword(HWIO_TCSR_DIFFERENTIAL_TEST_CLOCK_1_ADDR,v)
#define HWIO_TCSR_DIFFERENTIAL_TEST_CLOCK_1_OUTM(m,v) \
        out_dword_masked_ns(HWIO_TCSR_DIFFERENTIAL_TEST_CLOCK_1_ADDR,m,v,HWIO_TCSR_DIFFERENTIAL_TEST_CLOCK_1_IN)
#define HWIO_TCSR_DIFFERENTIAL_TEST_CLOCK_1_TEST_EN_1_BMSK                                                                   0x2
#define HWIO_TCSR_DIFFERENTIAL_TEST_CLOCK_1_TEST_EN_1_SHFT                                                                   0x1
#define HWIO_TCSR_DIFFERENTIAL_TEST_CLOCK_1_TEST_DATA_1_BMSK                                                                 0x1
#define HWIO_TCSR_DIFFERENTIAL_TEST_CLOCK_1_TEST_DATA_1_SHFT                                                                 0x0

#define HWIO_TCSR_DIFFERENTIAL_TEST_CLOCK_0_ADDR                                                                      (TCSR_TCSR_REGS_REG_BASE      + 0x00016004)
#define HWIO_TCSR_DIFFERENTIAL_TEST_CLOCK_0_OFFS                                                                      (TCSR_TCSR_REGS_REG_BASE_OFFS + 0x00016004)
#define HWIO_TCSR_DIFFERENTIAL_TEST_CLOCK_0_RMSK                                                                             0x3
#define HWIO_TCSR_DIFFERENTIAL_TEST_CLOCK_0_IN          \
        in_dword_masked(HWIO_TCSR_DIFFERENTIAL_TEST_CLOCK_0_ADDR, HWIO_TCSR_DIFFERENTIAL_TEST_CLOCK_0_RMSK)
#define HWIO_TCSR_DIFFERENTIAL_TEST_CLOCK_0_INM(m)      \
        in_dword_masked(HWIO_TCSR_DIFFERENTIAL_TEST_CLOCK_0_ADDR, m)
#define HWIO_TCSR_DIFFERENTIAL_TEST_CLOCK_0_OUT(v)      \
        out_dword(HWIO_TCSR_DIFFERENTIAL_TEST_CLOCK_0_ADDR,v)
#define HWIO_TCSR_DIFFERENTIAL_TEST_CLOCK_0_OUTM(m,v) \
        out_dword_masked_ns(HWIO_TCSR_DIFFERENTIAL_TEST_CLOCK_0_ADDR,m,v,HWIO_TCSR_DIFFERENTIAL_TEST_CLOCK_0_IN)
#define HWIO_TCSR_DIFFERENTIAL_TEST_CLOCK_0_TEST_EN_0_BMSK                                                                   0x2
#define HWIO_TCSR_DIFFERENTIAL_TEST_CLOCK_0_TEST_EN_0_SHFT                                                                   0x1
#define HWIO_TCSR_DIFFERENTIAL_TEST_CLOCK_0_TEST_DATA_0_BMSK                                                                 0x1
#define HWIO_TCSR_DIFFERENTIAL_TEST_CLOCK_0_TEST_DATA_0_SHFT                                                                 0x0

#define HWIO_TCSR_CUSTOM_A7SS_ACC_TYPE9_ADDR                                                                          (TCSR_TCSR_REGS_REG_BASE      + 0x0000d224)
#define HWIO_TCSR_CUSTOM_A7SS_ACC_TYPE9_OFFS                                                                          (TCSR_TCSR_REGS_REG_BASE_OFFS + 0x0000d224)
#define HWIO_TCSR_CUSTOM_A7SS_ACC_TYPE9_RMSK                                                                                0xff
#define HWIO_TCSR_CUSTOM_A7SS_ACC_TYPE9_IN          \
        in_dword_masked(HWIO_TCSR_CUSTOM_A7SS_ACC_TYPE9_ADDR, HWIO_TCSR_CUSTOM_A7SS_ACC_TYPE9_RMSK)
#define HWIO_TCSR_CUSTOM_A7SS_ACC_TYPE9_INM(m)      \
        in_dword_masked(HWIO_TCSR_CUSTOM_A7SS_ACC_TYPE9_ADDR, m)
#define HWIO_TCSR_CUSTOM_A7SS_ACC_TYPE9_CUSTOM_A7SS_ACC_TYPE9_BMSK                                                          0xff
#define HWIO_TCSR_CUSTOM_A7SS_ACC_TYPE9_CUSTOM_A7SS_ACC_TYPE9_SHFT                                                           0x0

#define HWIO_TCSR_CUSTOM_A7SS_ACC_TYPE10_ADDR                                                                         (TCSR_TCSR_REGS_REG_BASE      + 0x0000d228)
#define HWIO_TCSR_CUSTOM_A7SS_ACC_TYPE10_OFFS                                                                         (TCSR_TCSR_REGS_REG_BASE_OFFS + 0x0000d228)
#define HWIO_TCSR_CUSTOM_A7SS_ACC_TYPE10_RMSK                                                                               0xff
#define HWIO_TCSR_CUSTOM_A7SS_ACC_TYPE10_IN          \
        in_dword_masked(HWIO_TCSR_CUSTOM_A7SS_ACC_TYPE10_ADDR, HWIO_TCSR_CUSTOM_A7SS_ACC_TYPE10_RMSK)
#define HWIO_TCSR_CUSTOM_A7SS_ACC_TYPE10_INM(m)      \
        in_dword_masked(HWIO_TCSR_CUSTOM_A7SS_ACC_TYPE10_ADDR, m)
#define HWIO_TCSR_CUSTOM_A7SS_ACC_TYPE10_CUSTOM_A7SS_ACC_TYPE10_BMSK                                                        0xff
#define HWIO_TCSR_CUSTOM_A7SS_ACC_TYPE10_CUSTOM_A7SS_ACC_TYPE10_SHFT                                                         0x0

#define HWIO_TCSR_CUSTOM_A7SS_ACC_TYPE11_ADDR                                                                         (TCSR_TCSR_REGS_REG_BASE      + 0x0000d22c)
#define HWIO_TCSR_CUSTOM_A7SS_ACC_TYPE11_OFFS                                                                         (TCSR_TCSR_REGS_REG_BASE_OFFS + 0x0000d22c)
#define HWIO_TCSR_CUSTOM_A7SS_ACC_TYPE11_RMSK                                                                               0xff
#define HWIO_TCSR_CUSTOM_A7SS_ACC_TYPE11_IN          \
        in_dword_masked(HWIO_TCSR_CUSTOM_A7SS_ACC_TYPE11_ADDR, HWIO_TCSR_CUSTOM_A7SS_ACC_TYPE11_RMSK)
#define HWIO_TCSR_CUSTOM_A7SS_ACC_TYPE11_INM(m)      \
        in_dword_masked(HWIO_TCSR_CUSTOM_A7SS_ACC_TYPE11_ADDR, m)
#define HWIO_TCSR_CUSTOM_A7SS_ACC_TYPE11_CUSTOM_A7SS_ACC_TYPE11_BMSK                                                        0xff
#define HWIO_TCSR_CUSTOM_A7SS_ACC_TYPE11_CUSTOM_A7SS_ACC_TYPE11_SHFT                                                         0x0

#define HWIO_TCSR_CUSTOM_A7SS_ACC_TYPE12_ADDR                                                                         (TCSR_TCSR_REGS_REG_BASE      + 0x0000d230)
#define HWIO_TCSR_CUSTOM_A7SS_ACC_TYPE12_OFFS                                                                         (TCSR_TCSR_REGS_REG_BASE_OFFS + 0x0000d230)
#define HWIO_TCSR_CUSTOM_A7SS_ACC_TYPE12_RMSK                                                                               0xff
#define HWIO_TCSR_CUSTOM_A7SS_ACC_TYPE12_IN          \
        in_dword_masked(HWIO_TCSR_CUSTOM_A7SS_ACC_TYPE12_ADDR, HWIO_TCSR_CUSTOM_A7SS_ACC_TYPE12_RMSK)
#define HWIO_TCSR_CUSTOM_A7SS_ACC_TYPE12_INM(m)      \
        in_dword_masked(HWIO_TCSR_CUSTOM_A7SS_ACC_TYPE12_ADDR, m)
#define HWIO_TCSR_CUSTOM_A7SS_ACC_TYPE12_CUSTOM_A7SS_ACC_TYPE12_BMSK                                                        0xff
#define HWIO_TCSR_CUSTOM_A7SS_ACC_TYPE12_CUSTOM_A7SS_ACC_TYPE12_SHFT                                                         0x0

#define HWIO_TCSR_CUSTOM_A7SS_ACC_TYPE13_ADDR                                                                         (TCSR_TCSR_REGS_REG_BASE      + 0x0000d234)
#define HWIO_TCSR_CUSTOM_A7SS_ACC_TYPE13_OFFS                                                                         (TCSR_TCSR_REGS_REG_BASE_OFFS + 0x0000d234)
#define HWIO_TCSR_CUSTOM_A7SS_ACC_TYPE13_RMSK                                                                               0xff
#define HWIO_TCSR_CUSTOM_A7SS_ACC_TYPE13_IN          \
        in_dword_masked(HWIO_TCSR_CUSTOM_A7SS_ACC_TYPE13_ADDR, HWIO_TCSR_CUSTOM_A7SS_ACC_TYPE13_RMSK)
#define HWIO_TCSR_CUSTOM_A7SS_ACC_TYPE13_INM(m)      \
        in_dword_masked(HWIO_TCSR_CUSTOM_A7SS_ACC_TYPE13_ADDR, m)
#define HWIO_TCSR_CUSTOM_A7SS_ACC_TYPE13_CUSTOM_A7SS_ACC_TYPE13_BMSK                                                        0xff
#define HWIO_TCSR_CUSTOM_A7SS_ACC_TYPE13_CUSTOM_A7SS_ACC_TYPE13_SHFT                                                         0x0

#define HWIO_TCSR_CUSTOM_A7SS_ACC_TYPE14_ADDR                                                                         (TCSR_TCSR_REGS_REG_BASE      + 0x0000d238)
#define HWIO_TCSR_CUSTOM_A7SS_ACC_TYPE14_OFFS                                                                         (TCSR_TCSR_REGS_REG_BASE_OFFS + 0x0000d238)
#define HWIO_TCSR_CUSTOM_A7SS_ACC_TYPE14_RMSK                                                                               0xff
#define HWIO_TCSR_CUSTOM_A7SS_ACC_TYPE14_IN          \
        in_dword_masked(HWIO_TCSR_CUSTOM_A7SS_ACC_TYPE14_ADDR, HWIO_TCSR_CUSTOM_A7SS_ACC_TYPE14_RMSK)
#define HWIO_TCSR_CUSTOM_A7SS_ACC_TYPE14_INM(m)      \
        in_dword_masked(HWIO_TCSR_CUSTOM_A7SS_ACC_TYPE14_ADDR, m)
#define HWIO_TCSR_CUSTOM_A7SS_ACC_TYPE14_CUSTOM_A7SS_ACC_TYPE14_BMSK                                                        0xff
#define HWIO_TCSR_CUSTOM_A7SS_ACC_TYPE14_CUSTOM_A7SS_ACC_TYPE14_SHFT                                                         0x0

#define HWIO_TCSR_CUSTOM_VDDCX_ACC_TYPE1_ADDR                                                                         (TCSR_TCSR_REGS_REG_BASE      + 0x0000d24c)
#define HWIO_TCSR_CUSTOM_VDDCX_ACC_TYPE1_OFFS                                                                         (TCSR_TCSR_REGS_REG_BASE_OFFS + 0x0000d24c)
#define HWIO_TCSR_CUSTOM_VDDCX_ACC_TYPE1_RMSK                                                                               0xff
#define HWIO_TCSR_CUSTOM_VDDCX_ACC_TYPE1_IN          \
        in_dword_masked(HWIO_TCSR_CUSTOM_VDDCX_ACC_TYPE1_ADDR, HWIO_TCSR_CUSTOM_VDDCX_ACC_TYPE1_RMSK)
#define HWIO_TCSR_CUSTOM_VDDCX_ACC_TYPE1_INM(m)      \
        in_dword_masked(HWIO_TCSR_CUSTOM_VDDCX_ACC_TYPE1_ADDR, m)
#define HWIO_TCSR_CUSTOM_VDDCX_ACC_TYPE1_CUSTOM_VDDCX_ACC_TYPE1_BMSK                                                        0xff
#define HWIO_TCSR_CUSTOM_VDDCX_ACC_TYPE1_CUSTOM_VDDCX_ACC_TYPE1_SHFT                                                         0x0

#define HWIO_TCSR_CUSTOM_VDDCX_ACC_TYPE2_ADDR                                                                         (TCSR_TCSR_REGS_REG_BASE      + 0x0000d250)
#define HWIO_TCSR_CUSTOM_VDDCX_ACC_TYPE2_OFFS                                                                         (TCSR_TCSR_REGS_REG_BASE_OFFS + 0x0000d250)
#define HWIO_TCSR_CUSTOM_VDDCX_ACC_TYPE2_RMSK                                                                               0xff
#define HWIO_TCSR_CUSTOM_VDDCX_ACC_TYPE2_IN          \
        in_dword_masked(HWIO_TCSR_CUSTOM_VDDCX_ACC_TYPE2_ADDR, HWIO_TCSR_CUSTOM_VDDCX_ACC_TYPE2_RMSK)
#define HWIO_TCSR_CUSTOM_VDDCX_ACC_TYPE2_INM(m)      \
        in_dword_masked(HWIO_TCSR_CUSTOM_VDDCX_ACC_TYPE2_ADDR, m)
#define HWIO_TCSR_CUSTOM_VDDCX_ACC_TYPE2_CUSTOM_VDDCX_ACC_TYPE2_BMSK                                                        0xff
#define HWIO_TCSR_CUSTOM_VDDCX_ACC_TYPE2_CUSTOM_VDDCX_ACC_TYPE2_SHFT                                                         0x0

#define HWIO_TCSR_CUSTOM_VDDCX_ACC_TYPE3_ADDR                                                                         (TCSR_TCSR_REGS_REG_BASE      + 0x0000d254)
#define HWIO_TCSR_CUSTOM_VDDCX_ACC_TYPE3_OFFS                                                                         (TCSR_TCSR_REGS_REG_BASE_OFFS + 0x0000d254)
#define HWIO_TCSR_CUSTOM_VDDCX_ACC_TYPE3_RMSK                                                                               0xff
#define HWIO_TCSR_CUSTOM_VDDCX_ACC_TYPE3_IN          \
        in_dword_masked(HWIO_TCSR_CUSTOM_VDDCX_ACC_TYPE3_ADDR, HWIO_TCSR_CUSTOM_VDDCX_ACC_TYPE3_RMSK)
#define HWIO_TCSR_CUSTOM_VDDCX_ACC_TYPE3_INM(m)      \
        in_dword_masked(HWIO_TCSR_CUSTOM_VDDCX_ACC_TYPE3_ADDR, m)
#define HWIO_TCSR_CUSTOM_VDDCX_ACC_TYPE3_CUSTOM_VDDCX_ACC_TYPE3_BMSK                                                        0xff
#define HWIO_TCSR_CUSTOM_VDDCX_ACC_TYPE3_CUSTOM_VDDCX_ACC_TYPE3_SHFT                                                         0x0

#define HWIO_TCSR_CUSTOM_VDDCX_ACC_TYPE4_ADDR                                                                         (TCSR_TCSR_REGS_REG_BASE      + 0x0000d258)
#define HWIO_TCSR_CUSTOM_VDDCX_ACC_TYPE4_OFFS                                                                         (TCSR_TCSR_REGS_REG_BASE_OFFS + 0x0000d258)
#define HWIO_TCSR_CUSTOM_VDDCX_ACC_TYPE4_RMSK                                                                               0xff
#define HWIO_TCSR_CUSTOM_VDDCX_ACC_TYPE4_IN          \
        in_dword_masked(HWIO_TCSR_CUSTOM_VDDCX_ACC_TYPE4_ADDR, HWIO_TCSR_CUSTOM_VDDCX_ACC_TYPE4_RMSK)
#define HWIO_TCSR_CUSTOM_VDDCX_ACC_TYPE4_INM(m)      \
        in_dword_masked(HWIO_TCSR_CUSTOM_VDDCX_ACC_TYPE4_ADDR, m)
#define HWIO_TCSR_CUSTOM_VDDCX_ACC_TYPE4_CUSTOM_VDDCX_ACC_TYPE4_BMSK                                                        0xff
#define HWIO_TCSR_CUSTOM_VDDCX_ACC_TYPE4_CUSTOM_VDDCX_ACC_TYPE4_SHFT                                                         0x0

#define HWIO_TCSR_CUSTOM_VDDCX_ACC_TYPE5_ADDR                                                                         (TCSR_TCSR_REGS_REG_BASE      + 0x0000d25c)
#define HWIO_TCSR_CUSTOM_VDDCX_ACC_TYPE5_OFFS                                                                         (TCSR_TCSR_REGS_REG_BASE_OFFS + 0x0000d25c)
#define HWIO_TCSR_CUSTOM_VDDCX_ACC_TYPE5_RMSK                                                                               0xff
#define HWIO_TCSR_CUSTOM_VDDCX_ACC_TYPE5_IN          \
        in_dword_masked(HWIO_TCSR_CUSTOM_VDDCX_ACC_TYPE5_ADDR, HWIO_TCSR_CUSTOM_VDDCX_ACC_TYPE5_RMSK)
#define HWIO_TCSR_CUSTOM_VDDCX_ACC_TYPE5_INM(m)      \
        in_dword_masked(HWIO_TCSR_CUSTOM_VDDCX_ACC_TYPE5_ADDR, m)
#define HWIO_TCSR_CUSTOM_VDDCX_ACC_TYPE5_CUSTOM_VDDCX_ACC_TYPE5_BMSK                                                        0xff
#define HWIO_TCSR_CUSTOM_VDDCX_ACC_TYPE5_CUSTOM_VDDCX_ACC_TYPE5_SHFT                                                         0x0

#define HWIO_TCSR_CUSTOM_VDDCX_ACC_TYPE6_ADDR                                                                         (TCSR_TCSR_REGS_REG_BASE      + 0x0000d260)
#define HWIO_TCSR_CUSTOM_VDDCX_ACC_TYPE6_OFFS                                                                         (TCSR_TCSR_REGS_REG_BASE_OFFS + 0x0000d260)
#define HWIO_TCSR_CUSTOM_VDDCX_ACC_TYPE6_RMSK                                                                               0xff
#define HWIO_TCSR_CUSTOM_VDDCX_ACC_TYPE6_IN          \
        in_dword_masked(HWIO_TCSR_CUSTOM_VDDCX_ACC_TYPE6_ADDR, HWIO_TCSR_CUSTOM_VDDCX_ACC_TYPE6_RMSK)
#define HWIO_TCSR_CUSTOM_VDDCX_ACC_TYPE6_INM(m)      \
        in_dword_masked(HWIO_TCSR_CUSTOM_VDDCX_ACC_TYPE6_ADDR, m)
#define HWIO_TCSR_CUSTOM_VDDCX_ACC_TYPE6_CUSTOM_VDDCX_ACC_TYPE6_BMSK                                                        0xff
#define HWIO_TCSR_CUSTOM_VDDCX_ACC_TYPE6_CUSTOM_VDDCX_ACC_TYPE6_SHFT                                                         0x0

#define HWIO_TCSR_CUSTOM_VDDCX_ACC_TYPE7_ADDR                                                                         (TCSR_TCSR_REGS_REG_BASE      + 0x0000d264)
#define HWIO_TCSR_CUSTOM_VDDCX_ACC_TYPE7_OFFS                                                                         (TCSR_TCSR_REGS_REG_BASE_OFFS + 0x0000d264)
#define HWIO_TCSR_CUSTOM_VDDCX_ACC_TYPE7_RMSK                                                                               0xff
#define HWIO_TCSR_CUSTOM_VDDCX_ACC_TYPE7_IN          \
        in_dword_masked(HWIO_TCSR_CUSTOM_VDDCX_ACC_TYPE7_ADDR, HWIO_TCSR_CUSTOM_VDDCX_ACC_TYPE7_RMSK)
#define HWIO_TCSR_CUSTOM_VDDCX_ACC_TYPE7_INM(m)      \
        in_dword_masked(HWIO_TCSR_CUSTOM_VDDCX_ACC_TYPE7_ADDR, m)
#define HWIO_TCSR_CUSTOM_VDDCX_ACC_TYPE7_CUSTOM_VDDCX_ACC_TYPE7_BMSK                                                        0xff
#define HWIO_TCSR_CUSTOM_VDDCX_ACC_TYPE7_CUSTOM_VDDCX_ACC_TYPE7_SHFT                                                         0x0

#define HWIO_TCSR_CUSTOM_VDDCX_ACC_TYPE8_ADDR                                                                         (TCSR_TCSR_REGS_REG_BASE      + 0x0000d268)
#define HWIO_TCSR_CUSTOM_VDDCX_ACC_TYPE8_OFFS                                                                         (TCSR_TCSR_REGS_REG_BASE_OFFS + 0x0000d268)
#define HWIO_TCSR_CUSTOM_VDDCX_ACC_TYPE8_RMSK                                                                               0xff
#define HWIO_TCSR_CUSTOM_VDDCX_ACC_TYPE8_IN          \
        in_dword_masked(HWIO_TCSR_CUSTOM_VDDCX_ACC_TYPE8_ADDR, HWIO_TCSR_CUSTOM_VDDCX_ACC_TYPE8_RMSK)
#define HWIO_TCSR_CUSTOM_VDDCX_ACC_TYPE8_INM(m)      \
        in_dword_masked(HWIO_TCSR_CUSTOM_VDDCX_ACC_TYPE8_ADDR, m)
#define HWIO_TCSR_CUSTOM_VDDCX_ACC_TYPE8_CUSTOM_VDDCX_ACC_TYPE8_BMSK                                                        0xff
#define HWIO_TCSR_CUSTOM_VDDCX_ACC_TYPE8_CUSTOM_VDDCX_ACC_TYPE8_SHFT                                                         0x0

#define HWIO_TCSR_CUSTOM_VDDCX_ACC_TYPE15_ADDR                                                                        (TCSR_TCSR_REGS_REG_BASE      + 0x0000d26c)
#define HWIO_TCSR_CUSTOM_VDDCX_ACC_TYPE15_OFFS                                                                        (TCSR_TCSR_REGS_REG_BASE_OFFS + 0x0000d26c)
#define HWIO_TCSR_CUSTOM_VDDCX_ACC_TYPE15_RMSK                                                                              0xff
#define HWIO_TCSR_CUSTOM_VDDCX_ACC_TYPE15_IN          \
        in_dword_masked(HWIO_TCSR_CUSTOM_VDDCX_ACC_TYPE15_ADDR, HWIO_TCSR_CUSTOM_VDDCX_ACC_TYPE15_RMSK)
#define HWIO_TCSR_CUSTOM_VDDCX_ACC_TYPE15_INM(m)      \
        in_dword_masked(HWIO_TCSR_CUSTOM_VDDCX_ACC_TYPE15_ADDR, m)
#define HWIO_TCSR_CUSTOM_VDDCX_ACC_TYPE15_CUSTOM_VDDCX_ACC_TYPE15_BMSK                                                      0xff
#define HWIO_TCSR_CUSTOM_VDDCX_ACC_TYPE15_CUSTOM_VDDCX_ACC_TYPE15_SHFT                                                       0x0

#define HWIO_TCSR_CUSTOM_VDDMSS_ACC_TYPE1_ADDR                                                                        (TCSR_TCSR_REGS_REG_BASE      + 0x0000d280)
#define HWIO_TCSR_CUSTOM_VDDMSS_ACC_TYPE1_OFFS                                                                        (TCSR_TCSR_REGS_REG_BASE_OFFS + 0x0000d280)
#define HWIO_TCSR_CUSTOM_VDDMSS_ACC_TYPE1_RMSK                                                                              0xff
#define HWIO_TCSR_CUSTOM_VDDMSS_ACC_TYPE1_IN          \
        in_dword_masked(HWIO_TCSR_CUSTOM_VDDMSS_ACC_TYPE1_ADDR, HWIO_TCSR_CUSTOM_VDDMSS_ACC_TYPE1_RMSK)
#define HWIO_TCSR_CUSTOM_VDDMSS_ACC_TYPE1_INM(m)      \
        in_dword_masked(HWIO_TCSR_CUSTOM_VDDMSS_ACC_TYPE1_ADDR, m)
#define HWIO_TCSR_CUSTOM_VDDMSS_ACC_TYPE1_CUSTOM_VDDMSS_ACC_TYPE1_BMSK                                                      0xff
#define HWIO_TCSR_CUSTOM_VDDMSS_ACC_TYPE1_CUSTOM_VDDMSS_ACC_TYPE1_SHFT                                                       0x0

#define HWIO_TCSR_CUSTOM_VDDMSS_ACC_TYPE2_ADDR                                                                        (TCSR_TCSR_REGS_REG_BASE      + 0x0000d284)
#define HWIO_TCSR_CUSTOM_VDDMSS_ACC_TYPE2_OFFS                                                                        (TCSR_TCSR_REGS_REG_BASE_OFFS + 0x0000d284)
#define HWIO_TCSR_CUSTOM_VDDMSS_ACC_TYPE2_RMSK                                                                              0xff
#define HWIO_TCSR_CUSTOM_VDDMSS_ACC_TYPE2_IN          \
        in_dword_masked(HWIO_TCSR_CUSTOM_VDDMSS_ACC_TYPE2_ADDR, HWIO_TCSR_CUSTOM_VDDMSS_ACC_TYPE2_RMSK)
#define HWIO_TCSR_CUSTOM_VDDMSS_ACC_TYPE2_INM(m)      \
        in_dword_masked(HWIO_TCSR_CUSTOM_VDDMSS_ACC_TYPE2_ADDR, m)
#define HWIO_TCSR_CUSTOM_VDDMSS_ACC_TYPE2_CUSTOM_VDDMSS_ACC_TYPE2_BMSK                                                      0xff
#define HWIO_TCSR_CUSTOM_VDDMSS_ACC_TYPE2_CUSTOM_VDDMSS_ACC_TYPE2_SHFT                                                       0x0

#define HWIO_TCSR_CUSTOM_VDDMSS_ACC_TYPE3_ADDR                                                                        (TCSR_TCSR_REGS_REG_BASE      + 0x0000d288)
#define HWIO_TCSR_CUSTOM_VDDMSS_ACC_TYPE3_OFFS                                                                        (TCSR_TCSR_REGS_REG_BASE_OFFS + 0x0000d288)
#define HWIO_TCSR_CUSTOM_VDDMSS_ACC_TYPE3_RMSK                                                                              0xff
#define HWIO_TCSR_CUSTOM_VDDMSS_ACC_TYPE3_IN          \
        in_dword_masked(HWIO_TCSR_CUSTOM_VDDMSS_ACC_TYPE3_ADDR, HWIO_TCSR_CUSTOM_VDDMSS_ACC_TYPE3_RMSK)
#define HWIO_TCSR_CUSTOM_VDDMSS_ACC_TYPE3_INM(m)      \
        in_dword_masked(HWIO_TCSR_CUSTOM_VDDMSS_ACC_TYPE3_ADDR, m)
#define HWIO_TCSR_CUSTOM_VDDMSS_ACC_TYPE3_CUSTOM_VDDMSS_ACC_TYPE3_BMSK                                                      0xff
#define HWIO_TCSR_CUSTOM_VDDMSS_ACC_TYPE3_CUSTOM_VDDMSS_ACC_TYPE3_SHFT                                                       0x0

#define HWIO_TCSR_CUSTOM_VDDMSS_ACC_TYPE4_ADDR                                                                        (TCSR_TCSR_REGS_REG_BASE      + 0x0000d28c)
#define HWIO_TCSR_CUSTOM_VDDMSS_ACC_TYPE4_OFFS                                                                        (TCSR_TCSR_REGS_REG_BASE_OFFS + 0x0000d28c)
#define HWIO_TCSR_CUSTOM_VDDMSS_ACC_TYPE4_RMSK                                                                              0xff
#define HWIO_TCSR_CUSTOM_VDDMSS_ACC_TYPE4_IN          \
        in_dword_masked(HWIO_TCSR_CUSTOM_VDDMSS_ACC_TYPE4_ADDR, HWIO_TCSR_CUSTOM_VDDMSS_ACC_TYPE4_RMSK)
#define HWIO_TCSR_CUSTOM_VDDMSS_ACC_TYPE4_INM(m)      \
        in_dword_masked(HWIO_TCSR_CUSTOM_VDDMSS_ACC_TYPE4_ADDR, m)
#define HWIO_TCSR_CUSTOM_VDDMSS_ACC_TYPE4_CUSTOM_VDDMSS_ACC_TYPE4_BMSK                                                      0xff
#define HWIO_TCSR_CUSTOM_VDDMSS_ACC_TYPE4_CUSTOM_VDDMSS_ACC_TYPE4_SHFT                                                       0x0

#define HWIO_TCSR_CUSTOM_VDDMSS_ACC_TYPE5_ADDR                                                                        (TCSR_TCSR_REGS_REG_BASE      + 0x0000d290)
#define HWIO_TCSR_CUSTOM_VDDMSS_ACC_TYPE5_OFFS                                                                        (TCSR_TCSR_REGS_REG_BASE_OFFS + 0x0000d290)
#define HWIO_TCSR_CUSTOM_VDDMSS_ACC_TYPE5_RMSK                                                                              0xff
#define HWIO_TCSR_CUSTOM_VDDMSS_ACC_TYPE5_IN          \
        in_dword_masked(HWIO_TCSR_CUSTOM_VDDMSS_ACC_TYPE5_ADDR, HWIO_TCSR_CUSTOM_VDDMSS_ACC_TYPE5_RMSK)
#define HWIO_TCSR_CUSTOM_VDDMSS_ACC_TYPE5_INM(m)      \
        in_dword_masked(HWIO_TCSR_CUSTOM_VDDMSS_ACC_TYPE5_ADDR, m)
#define HWIO_TCSR_CUSTOM_VDDMSS_ACC_TYPE5_CUSTOM_VDDMSS_ACC_TYPE5_BMSK                                                      0xff
#define HWIO_TCSR_CUSTOM_VDDMSS_ACC_TYPE5_CUSTOM_VDDMSS_ACC_TYPE5_SHFT                                                       0x0

#define HWIO_TCSR_CUSTOM_VDDMSS_ACC_TYPE6_ADDR                                                                        (TCSR_TCSR_REGS_REG_BASE      + 0x0000d294)
#define HWIO_TCSR_CUSTOM_VDDMSS_ACC_TYPE6_OFFS                                                                        (TCSR_TCSR_REGS_REG_BASE_OFFS + 0x0000d294)
#define HWIO_TCSR_CUSTOM_VDDMSS_ACC_TYPE6_RMSK                                                                              0xff
#define HWIO_TCSR_CUSTOM_VDDMSS_ACC_TYPE6_IN          \
        in_dword_masked(HWIO_TCSR_CUSTOM_VDDMSS_ACC_TYPE6_ADDR, HWIO_TCSR_CUSTOM_VDDMSS_ACC_TYPE6_RMSK)
#define HWIO_TCSR_CUSTOM_VDDMSS_ACC_TYPE6_INM(m)      \
        in_dword_masked(HWIO_TCSR_CUSTOM_VDDMSS_ACC_TYPE6_ADDR, m)
#define HWIO_TCSR_CUSTOM_VDDMSS_ACC_TYPE6_CUSTOM_VDDMSS_ACC_TYPE6_BMSK                                                      0xff
#define HWIO_TCSR_CUSTOM_VDDMSS_ACC_TYPE6_CUSTOM_VDDMSS_ACC_TYPE6_SHFT                                                       0x0

#define HWIO_TCSR_CUSTOM_VDDMSS_ACC_TYPE7_ADDR                                                                        (TCSR_TCSR_REGS_REG_BASE      + 0x0000d298)
#define HWIO_TCSR_CUSTOM_VDDMSS_ACC_TYPE7_OFFS                                                                        (TCSR_TCSR_REGS_REG_BASE_OFFS + 0x0000d298)
#define HWIO_TCSR_CUSTOM_VDDMSS_ACC_TYPE7_RMSK                                                                              0xff
#define HWIO_TCSR_CUSTOM_VDDMSS_ACC_TYPE7_IN          \
        in_dword_masked(HWIO_TCSR_CUSTOM_VDDMSS_ACC_TYPE7_ADDR, HWIO_TCSR_CUSTOM_VDDMSS_ACC_TYPE7_RMSK)
#define HWIO_TCSR_CUSTOM_VDDMSS_ACC_TYPE7_INM(m)      \
        in_dword_masked(HWIO_TCSR_CUSTOM_VDDMSS_ACC_TYPE7_ADDR, m)
#define HWIO_TCSR_CUSTOM_VDDMSS_ACC_TYPE7_CUSTOM_VDDMSS_ACC_TYPE7_BMSK                                                      0xff
#define HWIO_TCSR_CUSTOM_VDDMSS_ACC_TYPE7_CUSTOM_VDDMSS_ACC_TYPE7_SHFT                                                       0x0

#define HWIO_TCSR_CUSTOM_VDDMSS_ACC_TYPE8_ADDR                                                                        (TCSR_TCSR_REGS_REG_BASE      + 0x0000d29c)
#define HWIO_TCSR_CUSTOM_VDDMSS_ACC_TYPE8_OFFS                                                                        (TCSR_TCSR_REGS_REG_BASE_OFFS + 0x0000d29c)
#define HWIO_TCSR_CUSTOM_VDDMSS_ACC_TYPE8_RMSK                                                                              0xff
#define HWIO_TCSR_CUSTOM_VDDMSS_ACC_TYPE8_IN          \
        in_dword_masked(HWIO_TCSR_CUSTOM_VDDMSS_ACC_TYPE8_ADDR, HWIO_TCSR_CUSTOM_VDDMSS_ACC_TYPE8_RMSK)
#define HWIO_TCSR_CUSTOM_VDDMSS_ACC_TYPE8_INM(m)      \
        in_dword_masked(HWIO_TCSR_CUSTOM_VDDMSS_ACC_TYPE8_ADDR, m)
#define HWIO_TCSR_CUSTOM_VDDMSS_ACC_TYPE8_CUSTOM_VDDMSS_ACC_TYPE8_BMSK                                                      0xff
#define HWIO_TCSR_CUSTOM_VDDMSS_ACC_TYPE8_CUSTOM_VDDMSS_ACC_TYPE8_SHFT                                                       0x0

#define HWIO_TCSR_CUSTOM_VDDMSS_ACC_TYPE15_ADDR                                                                       (TCSR_TCSR_REGS_REG_BASE      + 0x0000d2a0)
#define HWIO_TCSR_CUSTOM_VDDMSS_ACC_TYPE15_OFFS                                                                       (TCSR_TCSR_REGS_REG_BASE_OFFS + 0x0000d2a0)
#define HWIO_TCSR_CUSTOM_VDDMSS_ACC_TYPE15_RMSK                                                                             0xff
#define HWIO_TCSR_CUSTOM_VDDMSS_ACC_TYPE15_IN          \
        in_dword_masked(HWIO_TCSR_CUSTOM_VDDMSS_ACC_TYPE15_ADDR, HWIO_TCSR_CUSTOM_VDDMSS_ACC_TYPE15_RMSK)
#define HWIO_TCSR_CUSTOM_VDDMSS_ACC_TYPE15_INM(m)      \
        in_dword_masked(HWIO_TCSR_CUSTOM_VDDMSS_ACC_TYPE15_ADDR, m)
#define HWIO_TCSR_CUSTOM_VDDMSS_ACC_TYPE15_CUSTOM_VDDMSS_ACC_TYPE15_BMSK                                                    0xff
#define HWIO_TCSR_CUSTOM_VDDMSS_ACC_TYPE15_CUSTOM_VDDMSS_ACC_TYPE15_SHFT                                                     0x0

#define HWIO_TCSR_CUSTOM_VDDDDRA_ACC_TYPE1_ADDR                                                                       (TCSR_TCSR_REGS_REG_BASE      + 0x0000d2b4)
#define HWIO_TCSR_CUSTOM_VDDDDRA_ACC_TYPE1_OFFS                                                                       (TCSR_TCSR_REGS_REG_BASE_OFFS + 0x0000d2b4)
#define HWIO_TCSR_CUSTOM_VDDDDRA_ACC_TYPE1_RMSK                                                                             0xff
#define HWIO_TCSR_CUSTOM_VDDDDRA_ACC_TYPE1_IN          \
        in_dword_masked(HWIO_TCSR_CUSTOM_VDDDDRA_ACC_TYPE1_ADDR, HWIO_TCSR_CUSTOM_VDDDDRA_ACC_TYPE1_RMSK)
#define HWIO_TCSR_CUSTOM_VDDDDRA_ACC_TYPE1_INM(m)      \
        in_dword_masked(HWIO_TCSR_CUSTOM_VDDDDRA_ACC_TYPE1_ADDR, m)
#define HWIO_TCSR_CUSTOM_VDDDDRA_ACC_TYPE1_CUSTOM_VDDDDRA_ACC_TYPE1_BMSK                                                    0xff
#define HWIO_TCSR_CUSTOM_VDDDDRA_ACC_TYPE1_CUSTOM_VDDDDRA_ACC_TYPE1_SHFT                                                     0x0

#define HWIO_TCSR_CUSTOM_VDDDDRA_ACC_TYPE2_ADDR                                                                       (TCSR_TCSR_REGS_REG_BASE      + 0x0000d2b8)
#define HWIO_TCSR_CUSTOM_VDDDDRA_ACC_TYPE2_OFFS                                                                       (TCSR_TCSR_REGS_REG_BASE_OFFS + 0x0000d2b8)
#define HWIO_TCSR_CUSTOM_VDDDDRA_ACC_TYPE2_RMSK                                                                             0xff
#define HWIO_TCSR_CUSTOM_VDDDDRA_ACC_TYPE2_IN          \
        in_dword_masked(HWIO_TCSR_CUSTOM_VDDDDRA_ACC_TYPE2_ADDR, HWIO_TCSR_CUSTOM_VDDDDRA_ACC_TYPE2_RMSK)
#define HWIO_TCSR_CUSTOM_VDDDDRA_ACC_TYPE2_INM(m)      \
        in_dword_masked(HWIO_TCSR_CUSTOM_VDDDDRA_ACC_TYPE2_ADDR, m)
#define HWIO_TCSR_CUSTOM_VDDDDRA_ACC_TYPE2_CUSTOM_VDDDDRA_ACC_TYPE2_BMSK                                                    0xff
#define HWIO_TCSR_CUSTOM_VDDDDRA_ACC_TYPE2_CUSTOM_VDDDDRA_ACC_TYPE2_SHFT                                                     0x0

#define HWIO_TCSR_CUSTOM_VDDDDRA_ACC_TYPE3_ADDR                                                                       (TCSR_TCSR_REGS_REG_BASE      + 0x0000d2bc)
#define HWIO_TCSR_CUSTOM_VDDDDRA_ACC_TYPE3_OFFS                                                                       (TCSR_TCSR_REGS_REG_BASE_OFFS + 0x0000d2bc)
#define HWIO_TCSR_CUSTOM_VDDDDRA_ACC_TYPE3_RMSK                                                                             0xff
#define HWIO_TCSR_CUSTOM_VDDDDRA_ACC_TYPE3_IN          \
        in_dword_masked(HWIO_TCSR_CUSTOM_VDDDDRA_ACC_TYPE3_ADDR, HWIO_TCSR_CUSTOM_VDDDDRA_ACC_TYPE3_RMSK)
#define HWIO_TCSR_CUSTOM_VDDDDRA_ACC_TYPE3_INM(m)      \
        in_dword_masked(HWIO_TCSR_CUSTOM_VDDDDRA_ACC_TYPE3_ADDR, m)
#define HWIO_TCSR_CUSTOM_VDDDDRA_ACC_TYPE3_CUSTOM_VDDDDRA_ACC_TYPE3_BMSK                                                    0xff
#define HWIO_TCSR_CUSTOM_VDDDDRA_ACC_TYPE3_CUSTOM_VDDDDRA_ACC_TYPE3_SHFT                                                     0x0

#define HWIO_TCSR_CUSTOM_VDDDDRA_ACC_TYPE4_ADDR                                                                       (TCSR_TCSR_REGS_REG_BASE      + 0x0000d2c0)
#define HWIO_TCSR_CUSTOM_VDDDDRA_ACC_TYPE4_OFFS                                                                       (TCSR_TCSR_REGS_REG_BASE_OFFS + 0x0000d2c0)
#define HWIO_TCSR_CUSTOM_VDDDDRA_ACC_TYPE4_RMSK                                                                             0xff
#define HWIO_TCSR_CUSTOM_VDDDDRA_ACC_TYPE4_IN          \
        in_dword_masked(HWIO_TCSR_CUSTOM_VDDDDRA_ACC_TYPE4_ADDR, HWIO_TCSR_CUSTOM_VDDDDRA_ACC_TYPE4_RMSK)
#define HWIO_TCSR_CUSTOM_VDDDDRA_ACC_TYPE4_INM(m)      \
        in_dword_masked(HWIO_TCSR_CUSTOM_VDDDDRA_ACC_TYPE4_ADDR, m)
#define HWIO_TCSR_CUSTOM_VDDDDRA_ACC_TYPE4_CUSTOM_VDDDDRA_ACC_TYPE4_BMSK                                                    0xff
#define HWIO_TCSR_CUSTOM_VDDDDRA_ACC_TYPE4_CUSTOM_VDDDDRA_ACC_TYPE4_SHFT                                                     0x0

#define HWIO_TCSR_CUSTOM_VDDDDRA_ACC_TYPE5_ADDR                                                                       (TCSR_TCSR_REGS_REG_BASE      + 0x0000d2c4)
#define HWIO_TCSR_CUSTOM_VDDDDRA_ACC_TYPE5_OFFS                                                                       (TCSR_TCSR_REGS_REG_BASE_OFFS + 0x0000d2c4)
#define HWIO_TCSR_CUSTOM_VDDDDRA_ACC_TYPE5_RMSK                                                                             0xff
#define HWIO_TCSR_CUSTOM_VDDDDRA_ACC_TYPE5_IN          \
        in_dword_masked(HWIO_TCSR_CUSTOM_VDDDDRA_ACC_TYPE5_ADDR, HWIO_TCSR_CUSTOM_VDDDDRA_ACC_TYPE5_RMSK)
#define HWIO_TCSR_CUSTOM_VDDDDRA_ACC_TYPE5_INM(m)      \
        in_dword_masked(HWIO_TCSR_CUSTOM_VDDDDRA_ACC_TYPE5_ADDR, m)
#define HWIO_TCSR_CUSTOM_VDDDDRA_ACC_TYPE5_CUSTOM_VDDDDRA_ACC_TYPE5_BMSK                                                    0xff
#define HWIO_TCSR_CUSTOM_VDDDDRA_ACC_TYPE5_CUSTOM_VDDDDRA_ACC_TYPE5_SHFT                                                     0x0

#define HWIO_TCSR_CUSTOM_VDDDDRA_ACC_TYPE6_ADDR                                                                       (TCSR_TCSR_REGS_REG_BASE      + 0x0000d2c8)
#define HWIO_TCSR_CUSTOM_VDDDDRA_ACC_TYPE6_OFFS                                                                       (TCSR_TCSR_REGS_REG_BASE_OFFS + 0x0000d2c8)
#define HWIO_TCSR_CUSTOM_VDDDDRA_ACC_TYPE6_RMSK                                                                             0xff
#define HWIO_TCSR_CUSTOM_VDDDDRA_ACC_TYPE6_IN          \
        in_dword_masked(HWIO_TCSR_CUSTOM_VDDDDRA_ACC_TYPE6_ADDR, HWIO_TCSR_CUSTOM_VDDDDRA_ACC_TYPE6_RMSK)
#define HWIO_TCSR_CUSTOM_VDDDDRA_ACC_TYPE6_INM(m)      \
        in_dword_masked(HWIO_TCSR_CUSTOM_VDDDDRA_ACC_TYPE6_ADDR, m)
#define HWIO_TCSR_CUSTOM_VDDDDRA_ACC_TYPE6_CUSTOM_VDDDDRA_ACC_TYPE6_BMSK                                                    0xff
#define HWIO_TCSR_CUSTOM_VDDDDRA_ACC_TYPE6_CUSTOM_VDDDDRA_ACC_TYPE6_SHFT                                                     0x0

#define HWIO_TCSR_CUSTOM_VDDDDRA_ACC_TYPE7_ADDR                                                                       (TCSR_TCSR_REGS_REG_BASE      + 0x0000d2cc)
#define HWIO_TCSR_CUSTOM_VDDDDRA_ACC_TYPE7_OFFS                                                                       (TCSR_TCSR_REGS_REG_BASE_OFFS + 0x0000d2cc)
#define HWIO_TCSR_CUSTOM_VDDDDRA_ACC_TYPE7_RMSK                                                                             0xff
#define HWIO_TCSR_CUSTOM_VDDDDRA_ACC_TYPE7_IN          \
        in_dword_masked(HWIO_TCSR_CUSTOM_VDDDDRA_ACC_TYPE7_ADDR, HWIO_TCSR_CUSTOM_VDDDDRA_ACC_TYPE7_RMSK)
#define HWIO_TCSR_CUSTOM_VDDDDRA_ACC_TYPE7_INM(m)      \
        in_dword_masked(HWIO_TCSR_CUSTOM_VDDDDRA_ACC_TYPE7_ADDR, m)
#define HWIO_TCSR_CUSTOM_VDDDDRA_ACC_TYPE7_CUSTOM_VDDDDRA_ACC_TYPE7_BMSK                                                    0xff
#define HWIO_TCSR_CUSTOM_VDDDDRA_ACC_TYPE7_CUSTOM_VDDDDRA_ACC_TYPE7_SHFT                                                     0x0

#define HWIO_TCSR_CUSTOM_VDDDDRA_ACC_TYPE8_ADDR                                                                       (TCSR_TCSR_REGS_REG_BASE      + 0x0000d2d0)
#define HWIO_TCSR_CUSTOM_VDDDDRA_ACC_TYPE8_OFFS                                                                       (TCSR_TCSR_REGS_REG_BASE_OFFS + 0x0000d2d0)
#define HWIO_TCSR_CUSTOM_VDDDDRA_ACC_TYPE8_RMSK                                                                             0xff
#define HWIO_TCSR_CUSTOM_VDDDDRA_ACC_TYPE8_IN          \
        in_dword_masked(HWIO_TCSR_CUSTOM_VDDDDRA_ACC_TYPE8_ADDR, HWIO_TCSR_CUSTOM_VDDDDRA_ACC_TYPE8_RMSK)
#define HWIO_TCSR_CUSTOM_VDDDDRA_ACC_TYPE8_INM(m)      \
        in_dword_masked(HWIO_TCSR_CUSTOM_VDDDDRA_ACC_TYPE8_ADDR, m)
#define HWIO_TCSR_CUSTOM_VDDDDRA_ACC_TYPE8_CUSTOM_VDDDDRA_ACC_TYPE8_BMSK                                                    0xff
#define HWIO_TCSR_CUSTOM_VDDDDRA_ACC_TYPE8_CUSTOM_VDDDDRA_ACC_TYPE8_SHFT                                                     0x0

#define HWIO_TCSR_CUSTOM_VDDDDRA_ACC_TYPE15_ADDR                                                                      (TCSR_TCSR_REGS_REG_BASE      + 0x0000d2d4)
#define HWIO_TCSR_CUSTOM_VDDDDRA_ACC_TYPE15_OFFS                                                                      (TCSR_TCSR_REGS_REG_BASE_OFFS + 0x0000d2d4)
#define HWIO_TCSR_CUSTOM_VDDDDRA_ACC_TYPE15_RMSK                                                                            0xff
#define HWIO_TCSR_CUSTOM_VDDDDRA_ACC_TYPE15_IN          \
        in_dword_masked(HWIO_TCSR_CUSTOM_VDDDDRA_ACC_TYPE15_ADDR, HWIO_TCSR_CUSTOM_VDDDDRA_ACC_TYPE15_RMSK)
#define HWIO_TCSR_CUSTOM_VDDDDRA_ACC_TYPE15_INM(m)      \
        in_dword_masked(HWIO_TCSR_CUSTOM_VDDDDRA_ACC_TYPE15_ADDR, m)
#define HWIO_TCSR_CUSTOM_VDDDDRA_ACC_TYPE15_CUSTOM_VDDDDRA_ACC_TYPE15_BMSK                                                  0xff
#define HWIO_TCSR_CUSTOM_VDDDDRA_ACC_TYPE15_CUSTOM_VDDDDRA_ACC_TYPE15_SHFT                                                   0x0

#define HWIO_TCSR_MEM_ARRY_STBY_ADDR                                                                                  (TCSR_TCSR_REGS_REG_BASE      + 0x0000d180)
#define HWIO_TCSR_MEM_ARRY_STBY_OFFS                                                                                  (TCSR_TCSR_REGS_REG_BASE_OFFS + 0x0000d180)
#define HWIO_TCSR_MEM_ARRY_STBY_RMSK                                                                                         0x1
#define HWIO_TCSR_MEM_ARRY_STBY_IN          \
        in_dword_masked(HWIO_TCSR_MEM_ARRY_STBY_ADDR, HWIO_TCSR_MEM_ARRY_STBY_RMSK)
#define HWIO_TCSR_MEM_ARRY_STBY_INM(m)      \
        in_dword_masked(HWIO_TCSR_MEM_ARRY_STBY_ADDR, m)
#define HWIO_TCSR_MEM_ARRY_STBY_OUT(v)      \
        out_dword(HWIO_TCSR_MEM_ARRY_STBY_ADDR,v)
#define HWIO_TCSR_MEM_ARRY_STBY_OUTM(m,v) \
        out_dword_masked_ns(HWIO_TCSR_MEM_ARRY_STBY_ADDR,m,v,HWIO_TCSR_MEM_ARRY_STBY_IN)
#define HWIO_TCSR_MEM_ARRY_STBY_MEM_ARRY_STBY_N_BMSK                                                                         0x1
#define HWIO_TCSR_MEM_ARRY_STBY_MEM_ARRY_STBY_N_SHFT                                                                         0x0

#define HWIO_TCSR_PMU_CTILE_CONFIG_ADDR                                                                               (TCSR_TCSR_REGS_REG_BASE      + 0x0000e000)
#define HWIO_TCSR_PMU_CTILE_CONFIG_OFFS                                                                               (TCSR_TCSR_REGS_REG_BASE_OFFS + 0x0000e000)
#define HWIO_TCSR_PMU_CTILE_CONFIG_RMSK                                                                                  0x1ffff
#define HWIO_TCSR_PMU_CTILE_CONFIG_IN          \
        in_dword_masked(HWIO_TCSR_PMU_CTILE_CONFIG_ADDR, HWIO_TCSR_PMU_CTILE_CONFIG_RMSK)
#define HWIO_TCSR_PMU_CTILE_CONFIG_INM(m)      \
        in_dword_masked(HWIO_TCSR_PMU_CTILE_CONFIG_ADDR, m)
#define HWIO_TCSR_PMU_CTILE_CONFIG_OUT(v)      \
        out_dword(HWIO_TCSR_PMU_CTILE_CONFIG_ADDR,v)
#define HWIO_TCSR_PMU_CTILE_CONFIG_OUTM(m,v) \
        out_dword_masked_ns(HWIO_TCSR_PMU_CTILE_CONFIG_ADDR,m,v,HWIO_TCSR_PMU_CTILE_CONFIG_IN)
#define HWIO_TCSR_PMU_CTILE_CONFIG_DISABLE_PMU_CLAMP_MEM_CX_BMSK                                                         0x10000
#define HWIO_TCSR_PMU_CTILE_CONFIG_DISABLE_PMU_CLAMP_MEM_CX_SHFT                                                            0x10
#define HWIO_TCSR_PMU_CTILE_CONFIG_PMU_DBG_BUS_EN_BMSK                                                                    0x8000
#define HWIO_TCSR_PMU_CTILE_CONFIG_PMU_DBG_BUS_EN_SHFT                                                                       0xf
#define HWIO_TCSR_PMU_CTILE_CONFIG_DISABLE_PMU_FREEZEIO_BMSK                                                              0x4000
#define HWIO_TCSR_PMU_CTILE_CONFIG_DISABLE_PMU_FREEZEIO_SHFT                                                                 0xe
#define HWIO_TCSR_PMU_CTILE_CONFIG_DISABLE_PMU_PREFREEZEIO_BMSK                                                           0x2000
#define HWIO_TCSR_PMU_CTILE_CONFIG_DISABLE_PMU_PREFREEZEIO_SHFT                                                              0xd
#define HWIO_TCSR_PMU_CTILE_CONFIG_SW_OVERRIDE_SLEEP_CLK_DISABLE_BMSK                                                     0x1000
#define HWIO_TCSR_PMU_CTILE_CONFIG_SW_OVERRIDE_SLEEP_CLK_DISABLE_SHFT                                                        0xc
#define HWIO_TCSR_PMU_CTILE_CONFIG_SW_OVERRIDE_CXO_CLK_DISABLE_BMSK                                                        0x800
#define HWIO_TCSR_PMU_CTILE_CONFIG_SW_OVERRIDE_CXO_CLK_DISABLE_SHFT                                                          0xb
#define HWIO_TCSR_PMU_CTILE_CONFIG_MOCK_SHUTDOWN_BMSK                                                                      0x400
#define HWIO_TCSR_PMU_CTILE_CONFIG_MOCK_SHUTDOWN_SHFT                                                                        0xa
#define HWIO_TCSR_PMU_CTILE_CONFIG_DLY_PWRUP_RES_BMSK                                                                      0x3c0
#define HWIO_TCSR_PMU_CTILE_CONFIG_DLY_PWRUP_RES_SHFT                                                                        0x6
#define HWIO_TCSR_PMU_CTILE_CONFIG_DLY_PWRUP_FEW_BMSK                                                                       0x3c
#define HWIO_TCSR_PMU_CTILE_CONFIG_DLY_PWRUP_FEW_SHFT                                                                        0x2
#define HWIO_TCSR_PMU_CTILE_CONFIG_MPM_GATING_DIS_BMSK                                                                       0x2
#define HWIO_TCSR_PMU_CTILE_CONFIG_MPM_GATING_DIS_SHFT                                                                       0x1
#define HWIO_TCSR_PMU_CTILE_CONFIG_VDDCX_PC_EN_BMSK                                                                          0x1
#define HWIO_TCSR_PMU_CTILE_CONFIG_VDDCX_PC_EN_SHFT                                                                          0x0

#define HWIO_TCSR_PMU_CTILE_COLLAPSE_INDICATOR_ADDR                                                                   (TCSR_TCSR_REGS_REG_BASE      + 0x0000e004)
#define HWIO_TCSR_PMU_CTILE_COLLAPSE_INDICATOR_OFFS                                                                   (TCSR_TCSR_REGS_REG_BASE_OFFS + 0x0000e004)
#define HWIO_TCSR_PMU_CTILE_COLLAPSE_INDICATOR_RMSK                                                                          0x7
#define HWIO_TCSR_PMU_CTILE_COLLAPSE_INDICATOR_IN          \
        in_dword_masked(HWIO_TCSR_PMU_CTILE_COLLAPSE_INDICATOR_ADDR, HWIO_TCSR_PMU_CTILE_COLLAPSE_INDICATOR_RMSK)
#define HWIO_TCSR_PMU_CTILE_COLLAPSE_INDICATOR_INM(m)      \
        in_dword_masked(HWIO_TCSR_PMU_CTILE_COLLAPSE_INDICATOR_ADDR, m)
#define HWIO_TCSR_PMU_CTILE_COLLAPSE_INDICATOR_OUT(v)      \
        out_dword(HWIO_TCSR_PMU_CTILE_COLLAPSE_INDICATOR_ADDR,v)
#define HWIO_TCSR_PMU_CTILE_COLLAPSE_INDICATOR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_TCSR_PMU_CTILE_COLLAPSE_INDICATOR_ADDR,m,v,HWIO_TCSR_PMU_CTILE_COLLAPSE_INDICATOR_IN)
#define HWIO_TCSR_PMU_CTILE_COLLAPSE_INDICATOR_CTILE_COLLAPSE_IND_HW_STAT_BMSK                                               0x4
#define HWIO_TCSR_PMU_CTILE_COLLAPSE_INDICATOR_CTILE_COLLAPSE_IND_HW_STAT_SHFT                                               0x2
#define HWIO_TCSR_PMU_CTILE_COLLAPSE_INDICATOR_CTILE_PWR_UP_IND_HW_STAT_BMSK                                                 0x2
#define HWIO_TCSR_PMU_CTILE_COLLAPSE_INDICATOR_CTILE_PWR_UP_IND_HW_STAT_SHFT                                                 0x1
#define HWIO_TCSR_PMU_CTILE_COLLAPSE_INDICATOR_CTILE_PWR_STATE_IND_SW_CLR_BMSK                                               0x1
#define HWIO_TCSR_PMU_CTILE_COLLAPSE_INDICATOR_CTILE_PWR_STATE_IND_SW_CLR_SHFT                                               0x0

#define HWIO_TCSR_PHY_CLK_SCHEME_SEL_ADDR                                                                             (TCSR_TCSR_REGS_REG_BASE      + 0x0000e008)
#define HWIO_TCSR_PHY_CLK_SCHEME_SEL_OFFS                                                                             (TCSR_TCSR_REGS_REG_BASE_OFFS + 0x0000e008)
#define HWIO_TCSR_PHY_CLK_SCHEME_SEL_RMSK                                                                                    0x3
#define HWIO_TCSR_PHY_CLK_SCHEME_SEL_IN          \
        in_dword_masked(HWIO_TCSR_PHY_CLK_SCHEME_SEL_ADDR, HWIO_TCSR_PHY_CLK_SCHEME_SEL_RMSK)
#define HWIO_TCSR_PHY_CLK_SCHEME_SEL_INM(m)      \
        in_dword_masked(HWIO_TCSR_PHY_CLK_SCHEME_SEL_ADDR, m)
#define HWIO_TCSR_PHY_CLK_SCHEME_SEL_OUT(v)      \
        out_dword(HWIO_TCSR_PHY_CLK_SCHEME_SEL_ADDR,v)
#define HWIO_TCSR_PHY_CLK_SCHEME_SEL_OUTM(m,v) \
        out_dword_masked_ns(HWIO_TCSR_PHY_CLK_SCHEME_SEL_ADDR,m,v,HWIO_TCSR_PHY_CLK_SCHEME_SEL_IN)
#define HWIO_TCSR_PHY_CLK_SCHEME_SEL_ENABLE_SE_CLOCK_BMSK                                                                    0x2
#define HWIO_TCSR_PHY_CLK_SCHEME_SEL_ENABLE_SE_CLOCK_SHFT                                                                    0x1
#define HWIO_TCSR_PHY_CLK_SCHEME_SEL_PHY_CLK_SCHEME_SEL_BMSK                                                                 0x1
#define HWIO_TCSR_PHY_CLK_SCHEME_SEL_PHY_CLK_SCHEME_SEL_SHFT                                                                 0x0

#define HWIO_TCSR_VSENSE_CONTROLLER_ENABLE_REGISTER_ADDR                                                              (TCSR_TCSR_REGS_REG_BASE      + 0x00011000)
#define HWIO_TCSR_VSENSE_CONTROLLER_ENABLE_REGISTER_OFFS                                                              (TCSR_TCSR_REGS_REG_BASE_OFFS + 0x00011000)
#define HWIO_TCSR_VSENSE_CONTROLLER_ENABLE_REGISTER_RMSK                                                                     0x1
#define HWIO_TCSR_VSENSE_CONTROLLER_ENABLE_REGISTER_IN          \
        in_dword_masked(HWIO_TCSR_VSENSE_CONTROLLER_ENABLE_REGISTER_ADDR, HWIO_TCSR_VSENSE_CONTROLLER_ENABLE_REGISTER_RMSK)
#define HWIO_TCSR_VSENSE_CONTROLLER_ENABLE_REGISTER_INM(m)      \
        in_dword_masked(HWIO_TCSR_VSENSE_CONTROLLER_ENABLE_REGISTER_ADDR, m)
#define HWIO_TCSR_VSENSE_CONTROLLER_ENABLE_REGISTER_OUT(v)      \
        out_dword(HWIO_TCSR_VSENSE_CONTROLLER_ENABLE_REGISTER_ADDR,v)
#define HWIO_TCSR_VSENSE_CONTROLLER_ENABLE_REGISTER_OUTM(m,v) \
        out_dword_masked_ns(HWIO_TCSR_VSENSE_CONTROLLER_ENABLE_REGISTER_ADDR,m,v,HWIO_TCSR_VSENSE_CONTROLLER_ENABLE_REGISTER_IN)
#define HWIO_TCSR_VSENSE_CONTROLLER_ENABLE_REGISTER_VSENSE_CONTROLLER_ENABLE_REGISTER_BMSK                                   0x1
#define HWIO_TCSR_VSENSE_CONTROLLER_ENABLE_REGISTER_VSENSE_CONTROLLER_ENABLE_REGISTER_SHFT                                   0x0

#define HWIO_TCSR_TCSR_RESET_DEBUG_SW_ENTRY_ADDR                                                                      (TCSR_TCSR_REGS_REG_BASE      + 0x00012000)
#define HWIO_TCSR_TCSR_RESET_DEBUG_SW_ENTRY_OFFS                                                                      (TCSR_TCSR_REGS_REG_BASE_OFFS + 0x00012000)
#define HWIO_TCSR_TCSR_RESET_DEBUG_SW_ENTRY_RMSK                                                                      0xffffffff
#define HWIO_TCSR_TCSR_RESET_DEBUG_SW_ENTRY_IN          \
        in_dword_masked(HWIO_TCSR_TCSR_RESET_DEBUG_SW_ENTRY_ADDR, HWIO_TCSR_TCSR_RESET_DEBUG_SW_ENTRY_RMSK)
#define HWIO_TCSR_TCSR_RESET_DEBUG_SW_ENTRY_INM(m)      \
        in_dword_masked(HWIO_TCSR_TCSR_RESET_DEBUG_SW_ENTRY_ADDR, m)
#define HWIO_TCSR_TCSR_RESET_DEBUG_SW_ENTRY_OUT(v)      \
        out_dword(HWIO_TCSR_TCSR_RESET_DEBUG_SW_ENTRY_ADDR,v)
#define HWIO_TCSR_TCSR_RESET_DEBUG_SW_ENTRY_OUTM(m,v) \
        out_dword_masked_ns(HWIO_TCSR_TCSR_RESET_DEBUG_SW_ENTRY_ADDR,m,v,HWIO_TCSR_TCSR_RESET_DEBUG_SW_ENTRY_IN)
#define HWIO_TCSR_TCSR_RESET_DEBUG_SW_ENTRY_TCSR_RESET_DEBUG_SW_ENTRY_BMSK                                            0xffffffff
#define HWIO_TCSR_TCSR_RESET_DEBUG_SW_ENTRY_TCSR_RESET_DEBUG_SW_ENTRY_SHFT                                                   0x0

#define HWIO_TCSR_TCSR_BOOT_MISC_DETECT_ADDR                                                                          (TCSR_TCSR_REGS_REG_BASE      + 0x00013000)
#define HWIO_TCSR_TCSR_BOOT_MISC_DETECT_OFFS                                                                          (TCSR_TCSR_REGS_REG_BASE_OFFS + 0x00013000)
#define HWIO_TCSR_TCSR_BOOT_MISC_DETECT_RMSK                                                                          0xffffffff
#define HWIO_TCSR_TCSR_BOOT_MISC_DETECT_IN          \
        in_dword_masked(HWIO_TCSR_TCSR_BOOT_MISC_DETECT_ADDR, HWIO_TCSR_TCSR_BOOT_MISC_DETECT_RMSK)
#define HWIO_TCSR_TCSR_BOOT_MISC_DETECT_INM(m)      \
        in_dword_masked(HWIO_TCSR_TCSR_BOOT_MISC_DETECT_ADDR, m)
#define HWIO_TCSR_TCSR_BOOT_MISC_DETECT_OUT(v)      \
        out_dword(HWIO_TCSR_TCSR_BOOT_MISC_DETECT_ADDR,v)
#define HWIO_TCSR_TCSR_BOOT_MISC_DETECT_OUTM(m,v) \
        out_dword_masked_ns(HWIO_TCSR_TCSR_BOOT_MISC_DETECT_ADDR,m,v,HWIO_TCSR_TCSR_BOOT_MISC_DETECT_IN)
#define HWIO_TCSR_TCSR_BOOT_MISC_DETECT_TCSR_BOOT_MISC_DETECT_BMSK                                                    0xffffffff
#define HWIO_TCSR_TCSR_BOOT_MISC_DETECT_TCSR_BOOT_MISC_DETECT_SHFT                                                           0x0

#define HWIO_TCSR_TZ_WONCE_n_ADDR(n)                                                                                  (TCSR_TCSR_REGS_REG_BASE      + 0x00014000 + 0x4 * (n))
#define HWIO_TCSR_TZ_WONCE_n_OFFS(n)                                                                                  (TCSR_TCSR_REGS_REG_BASE_OFFS + 0x00014000 + 0x4 * (n))
#define HWIO_TCSR_TZ_WONCE_n_RMSK                                                                                     0xffffffff
#define HWIO_TCSR_TZ_WONCE_n_MAXn                                                                                             15
#define HWIO_TCSR_TZ_WONCE_n_INI(n)        \
        in_dword_masked(HWIO_TCSR_TZ_WONCE_n_ADDR(n), HWIO_TCSR_TZ_WONCE_n_RMSK)
#define HWIO_TCSR_TZ_WONCE_n_INMI(n,mask)    \
        in_dword_masked(HWIO_TCSR_TZ_WONCE_n_ADDR(n), mask)
#define HWIO_TCSR_TZ_WONCE_n_OUTI(n,val)    \
        out_dword(HWIO_TCSR_TZ_WONCE_n_ADDR(n),val)
#define HWIO_TCSR_TZ_WONCE_n_OUTMI(n,mask,val) \
        out_dword_masked_ns(HWIO_TCSR_TZ_WONCE_n_ADDR(n),mask,val,HWIO_TCSR_TZ_WONCE_n_INI(n))
#define HWIO_TCSR_TZ_WONCE_n_TZ_WONCE_ADDRESS_BMSK                                                                    0xffffffff
#define HWIO_TCSR_TZ_WONCE_n_TZ_WONCE_ADDRESS_SHFT                                                                           0x0

#define HWIO_TCSR_QREFS_TXVBG_CONFIG_ADDR                                                                             (TCSR_TCSR_REGS_REG_BASE      + 0x00015004)
#define HWIO_TCSR_QREFS_TXVBG_CONFIG_OFFS                                                                             (TCSR_TCSR_REGS_REG_BASE_OFFS + 0x00015004)
#define HWIO_TCSR_QREFS_TXVBG_CONFIG_RMSK                                                                                 0xffff
#define HWIO_TCSR_QREFS_TXVBG_CONFIG_IN          \
        in_dword_masked(HWIO_TCSR_QREFS_TXVBG_CONFIG_ADDR, HWIO_TCSR_QREFS_TXVBG_CONFIG_RMSK)
#define HWIO_TCSR_QREFS_TXVBG_CONFIG_INM(m)      \
        in_dword_masked(HWIO_TCSR_QREFS_TXVBG_CONFIG_ADDR, m)
#define HWIO_TCSR_QREFS_TXVBG_CONFIG_OUT(v)      \
        out_dword(HWIO_TCSR_QREFS_TXVBG_CONFIG_ADDR,v)
#define HWIO_TCSR_QREFS_TXVBG_CONFIG_OUTM(m,v) \
        out_dword_masked_ns(HWIO_TCSR_QREFS_TXVBG_CONFIG_ADDR,m,v,HWIO_TCSR_QREFS_TXVBG_CONFIG_IN)
#define HWIO_TCSR_QREFS_TXVBG_CONFIG_QREFS_TXVBG_CONFIG_BMSK                                                              0xffff
#define HWIO_TCSR_QREFS_TXVBG_CONFIG_QREFS_TXVBG_CONFIG_SHFT                                                                 0x0

#define HWIO_TCSR_QZIP_CTL_ST_ADDR                                                                                    (TCSR_TCSR_REGS_REG_BASE      + 0x00015028)
#define HWIO_TCSR_QZIP_CTL_ST_OFFS                                                                                    (TCSR_TCSR_REGS_REG_BASE_OFFS + 0x00015028)
#define HWIO_TCSR_QZIP_CTL_ST_RMSK                                                                                          0xff
#define HWIO_TCSR_QZIP_CTL_ST_IN          \
        in_dword_masked(HWIO_TCSR_QZIP_CTL_ST_ADDR, HWIO_TCSR_QZIP_CTL_ST_RMSK)
#define HWIO_TCSR_QZIP_CTL_ST_INM(m)      \
        in_dword_masked(HWIO_TCSR_QZIP_CTL_ST_ADDR, m)
#define HWIO_TCSR_QZIP_CTL_ST_OUT(v)      \
        out_dword(HWIO_TCSR_QZIP_CTL_ST_ADDR,v)
#define HWIO_TCSR_QZIP_CTL_ST_OUTM(m,v) \
        out_dword_masked_ns(HWIO_TCSR_QZIP_CTL_ST_ADDR,m,v,HWIO_TCSR_QZIP_CTL_ST_IN)
#define HWIO_TCSR_QZIP_CTL_ST_QZIP_CTL_ST_BMSK                                                                              0xff
#define HWIO_TCSR_QZIP_CTL_ST_QZIP_CTL_ST_SHFT                                                                               0x0

#define HWIO_TCSR_MASTER_CFG_COMPUTE_SIGNALING_ADDR                                                                   (TCSR_TCSR_REGS_REG_BASE      + 0x00018000)
#define HWIO_TCSR_MASTER_CFG_COMPUTE_SIGNALING_OFFS                                                                   (TCSR_TCSR_REGS_REG_BASE_OFFS + 0x00018000)
#define HWIO_TCSR_MASTER_CFG_COMPUTE_SIGNALING_RMSK                                                                   0xffffffff
#define HWIO_TCSR_MASTER_CFG_COMPUTE_SIGNALING_IN          \
        in_dword_masked(HWIO_TCSR_MASTER_CFG_COMPUTE_SIGNALING_ADDR, HWIO_TCSR_MASTER_CFG_COMPUTE_SIGNALING_RMSK)
#define HWIO_TCSR_MASTER_CFG_COMPUTE_SIGNALING_INM(m)      \
        in_dword_masked(HWIO_TCSR_MASTER_CFG_COMPUTE_SIGNALING_ADDR, m)
#define HWIO_TCSR_MASTER_CFG_COMPUTE_SIGNALING_OUT(v)      \
        out_dword(HWIO_TCSR_MASTER_CFG_COMPUTE_SIGNALING_ADDR,v)
#define HWIO_TCSR_MASTER_CFG_COMPUTE_SIGNALING_OUTM(m,v) \
        out_dword_masked_ns(HWIO_TCSR_MASTER_CFG_COMPUTE_SIGNALING_ADDR,m,v,HWIO_TCSR_MASTER_CFG_COMPUTE_SIGNALING_IN)
#define HWIO_TCSR_MASTER_CFG_COMPUTE_SIGNALING_CFG_BITS_SHFT_BMSK                                                     0xffffffff
#define HWIO_TCSR_MASTER_CFG_COMPUTE_SIGNALING_CFG_BITS_SHFT_SHFT                                                            0x0

#define HWIO_TCSR_MASTER_COMPUTE_SIGNALING_USER_0_ADDR                                                                (TCSR_TCSR_REGS_REG_BASE      + 0x00019000)
#define HWIO_TCSR_MASTER_COMPUTE_SIGNALING_USER_0_OFFS                                                                (TCSR_TCSR_REGS_REG_BASE_OFFS + 0x00019000)
#define HWIO_TCSR_MASTER_COMPUTE_SIGNALING_USER_0_RMSK                                                                0xffffffff
#define HWIO_TCSR_MASTER_COMPUTE_SIGNALING_USER_0_IN          \
        in_dword_masked(HWIO_TCSR_MASTER_COMPUTE_SIGNALING_USER_0_ADDR, HWIO_TCSR_MASTER_COMPUTE_SIGNALING_USER_0_RMSK)
#define HWIO_TCSR_MASTER_COMPUTE_SIGNALING_USER_0_INM(m)      \
        in_dword_masked(HWIO_TCSR_MASTER_COMPUTE_SIGNALING_USER_0_ADDR, m)
#define HWIO_TCSR_MASTER_COMPUTE_SIGNALING_USER_0_OUT(v)      \
        out_dword(HWIO_TCSR_MASTER_COMPUTE_SIGNALING_USER_0_ADDR,v)
#define HWIO_TCSR_MASTER_COMPUTE_SIGNALING_USER_0_OUTM(m,v) \
        out_dword_masked_ns(HWIO_TCSR_MASTER_COMPUTE_SIGNALING_USER_0_ADDR,m,v,HWIO_TCSR_MASTER_COMPUTE_SIGNALING_USER_0_IN)
#define HWIO_TCSR_MASTER_COMPUTE_SIGNALING_USER_0_USER_COMPUTE_SIGNALING_WRITE_STATUS_BMSK                            0xffffffff
#define HWIO_TCSR_MASTER_COMPUTE_SIGNALING_USER_0_USER_COMPUTE_SIGNALING_WRITE_STATUS_SHFT                                   0x0

#define HWIO_TCSR_MASTER_COMPUTE_SIGNALING_USER_1_ADDR                                                                (TCSR_TCSR_REGS_REG_BASE      + 0x00019004)
#define HWIO_TCSR_MASTER_COMPUTE_SIGNALING_USER_1_OFFS                                                                (TCSR_TCSR_REGS_REG_BASE_OFFS + 0x00019004)
#define HWIO_TCSR_MASTER_COMPUTE_SIGNALING_USER_1_RMSK                                                                0xffffffff
#define HWIO_TCSR_MASTER_COMPUTE_SIGNALING_USER_1_IN          \
        in_dword_masked(HWIO_TCSR_MASTER_COMPUTE_SIGNALING_USER_1_ADDR, HWIO_TCSR_MASTER_COMPUTE_SIGNALING_USER_1_RMSK)
#define HWIO_TCSR_MASTER_COMPUTE_SIGNALING_USER_1_INM(m)      \
        in_dword_masked(HWIO_TCSR_MASTER_COMPUTE_SIGNALING_USER_1_ADDR, m)
#define HWIO_TCSR_MASTER_COMPUTE_SIGNALING_USER_1_OUT(v)      \
        out_dword(HWIO_TCSR_MASTER_COMPUTE_SIGNALING_USER_1_ADDR,v)
#define HWIO_TCSR_MASTER_COMPUTE_SIGNALING_USER_1_OUTM(m,v) \
        out_dword_masked_ns(HWIO_TCSR_MASTER_COMPUTE_SIGNALING_USER_1_ADDR,m,v,HWIO_TCSR_MASTER_COMPUTE_SIGNALING_USER_1_IN)
#define HWIO_TCSR_MASTER_COMPUTE_SIGNALING_USER_1_USER_COMPUTE_SIGNALING_WRITE_STATUS_BMSK                            0xffffffff
#define HWIO_TCSR_MASTER_COMPUTE_SIGNALING_USER_1_USER_COMPUTE_SIGNALING_WRITE_STATUS_SHFT                                   0x0

#define HWIO_TCSR_PROCESSOR_0_COMPUTE_SIGNAL_STATUS_USER_ADDR                                                         (TCSR_TCSR_REGS_REG_BASE      + 0x0001a000)
#define HWIO_TCSR_PROCESSOR_0_COMPUTE_SIGNAL_STATUS_USER_OFFS                                                         (TCSR_TCSR_REGS_REG_BASE_OFFS + 0x0001a000)
#define HWIO_TCSR_PROCESSOR_0_COMPUTE_SIGNAL_STATUS_USER_RMSK                                                         0xffffffff
#define HWIO_TCSR_PROCESSOR_0_COMPUTE_SIGNAL_STATUS_USER_IN          \
        in_dword_masked(HWIO_TCSR_PROCESSOR_0_COMPUTE_SIGNAL_STATUS_USER_ADDR, HWIO_TCSR_PROCESSOR_0_COMPUTE_SIGNAL_STATUS_USER_RMSK)
#define HWIO_TCSR_PROCESSOR_0_COMPUTE_SIGNAL_STATUS_USER_INM(m)      \
        in_dword_masked(HWIO_TCSR_PROCESSOR_0_COMPUTE_SIGNAL_STATUS_USER_ADDR, m)
#define HWIO_TCSR_PROCESSOR_0_COMPUTE_SIGNAL_STATUS_USER_PROCESSOR_0_STATUS_USER_BMSK                                 0xffffffff
#define HWIO_TCSR_PROCESSOR_0_COMPUTE_SIGNAL_STATUS_USER_PROCESSOR_0_STATUS_USER_SHFT                                        0x0

#define HWIO_TCSR_PROCESSOR_0_COMPUTE_SIGNAL_CLEAR_USER_ADDR                                                          (TCSR_TCSR_REGS_REG_BASE      + 0x0001a400)
#define HWIO_TCSR_PROCESSOR_0_COMPUTE_SIGNAL_CLEAR_USER_OFFS                                                          (TCSR_TCSR_REGS_REG_BASE_OFFS + 0x0001a400)
#define HWIO_TCSR_PROCESSOR_0_COMPUTE_SIGNAL_CLEAR_USER_RMSK                                                          0xffffffff
#define HWIO_TCSR_PROCESSOR_0_COMPUTE_SIGNAL_CLEAR_USER_OUT(v)      \
        out_dword(HWIO_TCSR_PROCESSOR_0_COMPUTE_SIGNAL_CLEAR_USER_ADDR,v)
#define HWIO_TCSR_PROCESSOR_0_COMPUTE_SIGNAL_CLEAR_USER_PROCESSOR_0_CLEAR_USER_BMSK                                   0xffffffff
#define HWIO_TCSR_PROCESSOR_0_COMPUTE_SIGNAL_CLEAR_USER_PROCESSOR_0_CLEAR_USER_SHFT                                          0x0

#define HWIO_TCSR_PROCESSOR_0_COMPUTE_SIGNAL_MASK_USER_ADDR                                                           (TCSR_TCSR_REGS_REG_BASE      + 0x0001a800)
#define HWIO_TCSR_PROCESSOR_0_COMPUTE_SIGNAL_MASK_USER_OFFS                                                           (TCSR_TCSR_REGS_REG_BASE_OFFS + 0x0001a800)
#define HWIO_TCSR_PROCESSOR_0_COMPUTE_SIGNAL_MASK_USER_RMSK                                                           0xffffffff
#define HWIO_TCSR_PROCESSOR_0_COMPUTE_SIGNAL_MASK_USER_IN          \
        in_dword_masked(HWIO_TCSR_PROCESSOR_0_COMPUTE_SIGNAL_MASK_USER_ADDR, HWIO_TCSR_PROCESSOR_0_COMPUTE_SIGNAL_MASK_USER_RMSK)
#define HWIO_TCSR_PROCESSOR_0_COMPUTE_SIGNAL_MASK_USER_INM(m)      \
        in_dword_masked(HWIO_TCSR_PROCESSOR_0_COMPUTE_SIGNAL_MASK_USER_ADDR, m)
#define HWIO_TCSR_PROCESSOR_0_COMPUTE_SIGNAL_MASK_USER_OUT(v)      \
        out_dword(HWIO_TCSR_PROCESSOR_0_COMPUTE_SIGNAL_MASK_USER_ADDR,v)
#define HWIO_TCSR_PROCESSOR_0_COMPUTE_SIGNAL_MASK_USER_OUTM(m,v) \
        out_dword_masked_ns(HWIO_TCSR_PROCESSOR_0_COMPUTE_SIGNAL_MASK_USER_ADDR,m,v,HWIO_TCSR_PROCESSOR_0_COMPUTE_SIGNAL_MASK_USER_IN)
#define HWIO_TCSR_PROCESSOR_0_COMPUTE_SIGNAL_MASK_USER_PROCESSOR_0_MASK_USER_BMSK                                     0xffffffff
#define HWIO_TCSR_PROCESSOR_0_COMPUTE_SIGNAL_MASK_USER_PROCESSOR_0_MASK_USER_SHFT                                            0x0

#define HWIO_TCSR_PROCESSOR_1_COMPUTE_SIGNAL_STATUS_USER_ADDR                                                         (TCSR_TCSR_REGS_REG_BASE      + 0x0001b000)
#define HWIO_TCSR_PROCESSOR_1_COMPUTE_SIGNAL_STATUS_USER_OFFS                                                         (TCSR_TCSR_REGS_REG_BASE_OFFS + 0x0001b000)
#define HWIO_TCSR_PROCESSOR_1_COMPUTE_SIGNAL_STATUS_USER_RMSK                                                         0xffffffff
#define HWIO_TCSR_PROCESSOR_1_COMPUTE_SIGNAL_STATUS_USER_IN          \
        in_dword_masked(HWIO_TCSR_PROCESSOR_1_COMPUTE_SIGNAL_STATUS_USER_ADDR, HWIO_TCSR_PROCESSOR_1_COMPUTE_SIGNAL_STATUS_USER_RMSK)
#define HWIO_TCSR_PROCESSOR_1_COMPUTE_SIGNAL_STATUS_USER_INM(m)      \
        in_dword_masked(HWIO_TCSR_PROCESSOR_1_COMPUTE_SIGNAL_STATUS_USER_ADDR, m)
#define HWIO_TCSR_PROCESSOR_1_COMPUTE_SIGNAL_STATUS_USER_PROCESSOR_1_STATUS_USER_BMSK                                 0xffffffff
#define HWIO_TCSR_PROCESSOR_1_COMPUTE_SIGNAL_STATUS_USER_PROCESSOR_1_STATUS_USER_SHFT                                        0x0

#define HWIO_TCSR_PROCESSOR_1_COMPUTE_SIGNAL_CLEAR_USER_ADDR                                                          (TCSR_TCSR_REGS_REG_BASE      + 0x0001b400)
#define HWIO_TCSR_PROCESSOR_1_COMPUTE_SIGNAL_CLEAR_USER_OFFS                                                          (TCSR_TCSR_REGS_REG_BASE_OFFS + 0x0001b400)
#define HWIO_TCSR_PROCESSOR_1_COMPUTE_SIGNAL_CLEAR_USER_RMSK                                                          0xffffffff
#define HWIO_TCSR_PROCESSOR_1_COMPUTE_SIGNAL_CLEAR_USER_OUT(v)      \
        out_dword(HWIO_TCSR_PROCESSOR_1_COMPUTE_SIGNAL_CLEAR_USER_ADDR,v)
#define HWIO_TCSR_PROCESSOR_1_COMPUTE_SIGNAL_CLEAR_USER_PROCESSOR_1_CLEAR_USER_BMSK                                   0xffffffff
#define HWIO_TCSR_PROCESSOR_1_COMPUTE_SIGNAL_CLEAR_USER_PROCESSOR_1_CLEAR_USER_SHFT                                          0x0

#define HWIO_TCSR_PROCESSOR_1_COMPUTE_SIGNAL_MASK_USER_ADDR                                                           (TCSR_TCSR_REGS_REG_BASE      + 0x0001b800)
#define HWIO_TCSR_PROCESSOR_1_COMPUTE_SIGNAL_MASK_USER_OFFS                                                           (TCSR_TCSR_REGS_REG_BASE_OFFS + 0x0001b800)
#define HWIO_TCSR_PROCESSOR_1_COMPUTE_SIGNAL_MASK_USER_RMSK                                                           0xffffffff
#define HWIO_TCSR_PROCESSOR_1_COMPUTE_SIGNAL_MASK_USER_IN          \
        in_dword_masked(HWIO_TCSR_PROCESSOR_1_COMPUTE_SIGNAL_MASK_USER_ADDR, HWIO_TCSR_PROCESSOR_1_COMPUTE_SIGNAL_MASK_USER_RMSK)
#define HWIO_TCSR_PROCESSOR_1_COMPUTE_SIGNAL_MASK_USER_INM(m)      \
        in_dword_masked(HWIO_TCSR_PROCESSOR_1_COMPUTE_SIGNAL_MASK_USER_ADDR, m)
#define HWIO_TCSR_PROCESSOR_1_COMPUTE_SIGNAL_MASK_USER_OUT(v)      \
        out_dword(HWIO_TCSR_PROCESSOR_1_COMPUTE_SIGNAL_MASK_USER_ADDR,v)
#define HWIO_TCSR_PROCESSOR_1_COMPUTE_SIGNAL_MASK_USER_OUTM(m,v) \
        out_dword_masked_ns(HWIO_TCSR_PROCESSOR_1_COMPUTE_SIGNAL_MASK_USER_ADDR,m,v,HWIO_TCSR_PROCESSOR_1_COMPUTE_SIGNAL_MASK_USER_IN)
#define HWIO_TCSR_PROCESSOR_1_COMPUTE_SIGNAL_MASK_USER_PROCESSOR_1_MASK_USER_BMSK                                     0xffffffff
#define HWIO_TCSR_PROCESSOR_1_COMPUTE_SIGNAL_MASK_USER_PROCESSOR_1_MASK_USER_SHFT                                            0x0

#define HWIO_TCSR_PROCESSOR_2_COMPUTE_SIGNAL_STATUS_USER_ADDR                                                         (TCSR_TCSR_REGS_REG_BASE      + 0x0001c000)
#define HWIO_TCSR_PROCESSOR_2_COMPUTE_SIGNAL_STATUS_USER_OFFS                                                         (TCSR_TCSR_REGS_REG_BASE_OFFS + 0x0001c000)
#define HWIO_TCSR_PROCESSOR_2_COMPUTE_SIGNAL_STATUS_USER_RMSK                                                         0xffffffff
#define HWIO_TCSR_PROCESSOR_2_COMPUTE_SIGNAL_STATUS_USER_IN          \
        in_dword_masked(HWIO_TCSR_PROCESSOR_2_COMPUTE_SIGNAL_STATUS_USER_ADDR, HWIO_TCSR_PROCESSOR_2_COMPUTE_SIGNAL_STATUS_USER_RMSK)
#define HWIO_TCSR_PROCESSOR_2_COMPUTE_SIGNAL_STATUS_USER_INM(m)      \
        in_dword_masked(HWIO_TCSR_PROCESSOR_2_COMPUTE_SIGNAL_STATUS_USER_ADDR, m)
#define HWIO_TCSR_PROCESSOR_2_COMPUTE_SIGNAL_STATUS_USER_PROCESSOR_2_STATUS_USER_BMSK                                 0xffffffff
#define HWIO_TCSR_PROCESSOR_2_COMPUTE_SIGNAL_STATUS_USER_PROCESSOR_2_STATUS_USER_SHFT                                        0x0

#define HWIO_TCSR_PROCESSOR_2_COMPUTE_SIGNAL_CLEAR_USER_ADDR                                                          (TCSR_TCSR_REGS_REG_BASE      + 0x0001c400)
#define HWIO_TCSR_PROCESSOR_2_COMPUTE_SIGNAL_CLEAR_USER_OFFS                                                          (TCSR_TCSR_REGS_REG_BASE_OFFS + 0x0001c400)
#define HWIO_TCSR_PROCESSOR_2_COMPUTE_SIGNAL_CLEAR_USER_RMSK                                                          0xffffffff
#define HWIO_TCSR_PROCESSOR_2_COMPUTE_SIGNAL_CLEAR_USER_OUT(v)      \
        out_dword(HWIO_TCSR_PROCESSOR_2_COMPUTE_SIGNAL_CLEAR_USER_ADDR,v)
#define HWIO_TCSR_PROCESSOR_2_COMPUTE_SIGNAL_CLEAR_USER_PROCESSOR_2_CLEAR_USER_BMSK                                   0xffffffff
#define HWIO_TCSR_PROCESSOR_2_COMPUTE_SIGNAL_CLEAR_USER_PROCESSOR_2_CLEAR_USER_SHFT                                          0x0

#define HWIO_TCSR_PROCESSOR_2_COMPUTE_SIGNAL_MASK_USER_ADDR                                                           (TCSR_TCSR_REGS_REG_BASE      + 0x0001c800)
#define HWIO_TCSR_PROCESSOR_2_COMPUTE_SIGNAL_MASK_USER_OFFS                                                           (TCSR_TCSR_REGS_REG_BASE_OFFS + 0x0001c800)
#define HWIO_TCSR_PROCESSOR_2_COMPUTE_SIGNAL_MASK_USER_RMSK                                                           0xffffffff
#define HWIO_TCSR_PROCESSOR_2_COMPUTE_SIGNAL_MASK_USER_IN          \
        in_dword_masked(HWIO_TCSR_PROCESSOR_2_COMPUTE_SIGNAL_MASK_USER_ADDR, HWIO_TCSR_PROCESSOR_2_COMPUTE_SIGNAL_MASK_USER_RMSK)
#define HWIO_TCSR_PROCESSOR_2_COMPUTE_SIGNAL_MASK_USER_INM(m)      \
        in_dword_masked(HWIO_TCSR_PROCESSOR_2_COMPUTE_SIGNAL_MASK_USER_ADDR, m)
#define HWIO_TCSR_PROCESSOR_2_COMPUTE_SIGNAL_MASK_USER_OUT(v)      \
        out_dword(HWIO_TCSR_PROCESSOR_2_COMPUTE_SIGNAL_MASK_USER_ADDR,v)
#define HWIO_TCSR_PROCESSOR_2_COMPUTE_SIGNAL_MASK_USER_OUTM(m,v) \
        out_dword_masked_ns(HWIO_TCSR_PROCESSOR_2_COMPUTE_SIGNAL_MASK_USER_ADDR,m,v,HWIO_TCSR_PROCESSOR_2_COMPUTE_SIGNAL_MASK_USER_IN)
#define HWIO_TCSR_PROCESSOR_2_COMPUTE_SIGNAL_MASK_USER_PROCESSOR_2_MASK_USER_BMSK                                     0xffffffff
#define HWIO_TCSR_PROCESSOR_2_COMPUTE_SIGNAL_MASK_USER_PROCESSOR_2_MASK_USER_SHFT                                            0x0

#define HWIO_TCSR_MASTER_COMPUTE_SIGNALING_KERNEL_0_ADDR                                                              (TCSR_TCSR_REGS_REG_BASE      + 0x0001d000)
#define HWIO_TCSR_MASTER_COMPUTE_SIGNALING_KERNEL_0_OFFS                                                              (TCSR_TCSR_REGS_REG_BASE_OFFS + 0x0001d000)
#define HWIO_TCSR_MASTER_COMPUTE_SIGNALING_KERNEL_0_RMSK                                                              0xffffffff
#define HWIO_TCSR_MASTER_COMPUTE_SIGNALING_KERNEL_0_IN          \
        in_dword_masked(HWIO_TCSR_MASTER_COMPUTE_SIGNALING_KERNEL_0_ADDR, HWIO_TCSR_MASTER_COMPUTE_SIGNALING_KERNEL_0_RMSK)
#define HWIO_TCSR_MASTER_COMPUTE_SIGNALING_KERNEL_0_INM(m)      \
        in_dword_masked(HWIO_TCSR_MASTER_COMPUTE_SIGNALING_KERNEL_0_ADDR, m)
#define HWIO_TCSR_MASTER_COMPUTE_SIGNALING_KERNEL_0_OUT(v)      \
        out_dword(HWIO_TCSR_MASTER_COMPUTE_SIGNALING_KERNEL_0_ADDR,v)
#define HWIO_TCSR_MASTER_COMPUTE_SIGNALING_KERNEL_0_OUTM(m,v) \
        out_dword_masked_ns(HWIO_TCSR_MASTER_COMPUTE_SIGNALING_KERNEL_0_ADDR,m,v,HWIO_TCSR_MASTER_COMPUTE_SIGNALING_KERNEL_0_IN)
#define HWIO_TCSR_MASTER_COMPUTE_SIGNALING_KERNEL_0_KERNEL_COMPUTE_SIGNALING_WRITE_STATUS_BMSK                        0xffffffff
#define HWIO_TCSR_MASTER_COMPUTE_SIGNALING_KERNEL_0_KERNEL_COMPUTE_SIGNALING_WRITE_STATUS_SHFT                               0x0

#define HWIO_TCSR_MASTER_COMPUTE_SIGNALING_KERNEL_1_ADDR                                                              (TCSR_TCSR_REGS_REG_BASE      + 0x0001d004)
#define HWIO_TCSR_MASTER_COMPUTE_SIGNALING_KERNEL_1_OFFS                                                              (TCSR_TCSR_REGS_REG_BASE_OFFS + 0x0001d004)
#define HWIO_TCSR_MASTER_COMPUTE_SIGNALING_KERNEL_1_RMSK                                                              0xffffffff
#define HWIO_TCSR_MASTER_COMPUTE_SIGNALING_KERNEL_1_IN          \
        in_dword_masked(HWIO_TCSR_MASTER_COMPUTE_SIGNALING_KERNEL_1_ADDR, HWIO_TCSR_MASTER_COMPUTE_SIGNALING_KERNEL_1_RMSK)
#define HWIO_TCSR_MASTER_COMPUTE_SIGNALING_KERNEL_1_INM(m)      \
        in_dword_masked(HWIO_TCSR_MASTER_COMPUTE_SIGNALING_KERNEL_1_ADDR, m)
#define HWIO_TCSR_MASTER_COMPUTE_SIGNALING_KERNEL_1_OUT(v)      \
        out_dword(HWIO_TCSR_MASTER_COMPUTE_SIGNALING_KERNEL_1_ADDR,v)
#define HWIO_TCSR_MASTER_COMPUTE_SIGNALING_KERNEL_1_OUTM(m,v) \
        out_dword_masked_ns(HWIO_TCSR_MASTER_COMPUTE_SIGNALING_KERNEL_1_ADDR,m,v,HWIO_TCSR_MASTER_COMPUTE_SIGNALING_KERNEL_1_IN)
#define HWIO_TCSR_MASTER_COMPUTE_SIGNALING_KERNEL_1_KERNEL_COMPUTE_SIGNALING_WRITE_STATUS_BMSK                        0xffffffff
#define HWIO_TCSR_MASTER_COMPUTE_SIGNALING_KERNEL_1_KERNEL_COMPUTE_SIGNALING_WRITE_STATUS_SHFT                               0x0

#define HWIO_TCSR_PROCESSOR_0_COMPUTE_SIGNAL_STATUS_KERNEL_ADDR                                                       (TCSR_TCSR_REGS_REG_BASE      + 0x0001e000)
#define HWIO_TCSR_PROCESSOR_0_COMPUTE_SIGNAL_STATUS_KERNEL_OFFS                                                       (TCSR_TCSR_REGS_REG_BASE_OFFS + 0x0001e000)
#define HWIO_TCSR_PROCESSOR_0_COMPUTE_SIGNAL_STATUS_KERNEL_RMSK                                                       0xffffffff
#define HWIO_TCSR_PROCESSOR_0_COMPUTE_SIGNAL_STATUS_KERNEL_IN          \
        in_dword_masked(HWIO_TCSR_PROCESSOR_0_COMPUTE_SIGNAL_STATUS_KERNEL_ADDR, HWIO_TCSR_PROCESSOR_0_COMPUTE_SIGNAL_STATUS_KERNEL_RMSK)
#define HWIO_TCSR_PROCESSOR_0_COMPUTE_SIGNAL_STATUS_KERNEL_INM(m)      \
        in_dword_masked(HWIO_TCSR_PROCESSOR_0_COMPUTE_SIGNAL_STATUS_KERNEL_ADDR, m)
#define HWIO_TCSR_PROCESSOR_0_COMPUTE_SIGNAL_STATUS_KERNEL_PROCESSOR_0_STATUS_KERNEL_BMSK                             0xffffffff
#define HWIO_TCSR_PROCESSOR_0_COMPUTE_SIGNAL_STATUS_KERNEL_PROCESSOR_0_STATUS_KERNEL_SHFT                                    0x0

#define HWIO_TCSR_PROCESSOR_0_COMPUTE_SIGNAL_CLEAR_KERNEL_ADDR                                                        (TCSR_TCSR_REGS_REG_BASE      + 0x0001e400)
#define HWIO_TCSR_PROCESSOR_0_COMPUTE_SIGNAL_CLEAR_KERNEL_OFFS                                                        (TCSR_TCSR_REGS_REG_BASE_OFFS + 0x0001e400)
#define HWIO_TCSR_PROCESSOR_0_COMPUTE_SIGNAL_CLEAR_KERNEL_RMSK                                                        0xffffffff
#define HWIO_TCSR_PROCESSOR_0_COMPUTE_SIGNAL_CLEAR_KERNEL_OUT(v)      \
        out_dword(HWIO_TCSR_PROCESSOR_0_COMPUTE_SIGNAL_CLEAR_KERNEL_ADDR,v)
#define HWIO_TCSR_PROCESSOR_0_COMPUTE_SIGNAL_CLEAR_KERNEL_PROCESSOR_0_CLEAR_KERNEL_BMSK                               0xffffffff
#define HWIO_TCSR_PROCESSOR_0_COMPUTE_SIGNAL_CLEAR_KERNEL_PROCESSOR_0_CLEAR_KERNEL_SHFT                                      0x0

#define HWIO_TCSR_PROCESSOR_0_COMPUTE_SIGNAL_MASK_KERNEL_ADDR                                                         (TCSR_TCSR_REGS_REG_BASE      + 0x0001e800)
#define HWIO_TCSR_PROCESSOR_0_COMPUTE_SIGNAL_MASK_KERNEL_OFFS                                                         (TCSR_TCSR_REGS_REG_BASE_OFFS + 0x0001e800)
#define HWIO_TCSR_PROCESSOR_0_COMPUTE_SIGNAL_MASK_KERNEL_RMSK                                                         0xffffffff
#define HWIO_TCSR_PROCESSOR_0_COMPUTE_SIGNAL_MASK_KERNEL_IN          \
        in_dword_masked(HWIO_TCSR_PROCESSOR_0_COMPUTE_SIGNAL_MASK_KERNEL_ADDR, HWIO_TCSR_PROCESSOR_0_COMPUTE_SIGNAL_MASK_KERNEL_RMSK)
#define HWIO_TCSR_PROCESSOR_0_COMPUTE_SIGNAL_MASK_KERNEL_INM(m)      \
        in_dword_masked(HWIO_TCSR_PROCESSOR_0_COMPUTE_SIGNAL_MASK_KERNEL_ADDR, m)
#define HWIO_TCSR_PROCESSOR_0_COMPUTE_SIGNAL_MASK_KERNEL_OUT(v)      \
        out_dword(HWIO_TCSR_PROCESSOR_0_COMPUTE_SIGNAL_MASK_KERNEL_ADDR,v)
#define HWIO_TCSR_PROCESSOR_0_COMPUTE_SIGNAL_MASK_KERNEL_OUTM(m,v) \
        out_dword_masked_ns(HWIO_TCSR_PROCESSOR_0_COMPUTE_SIGNAL_MASK_KERNEL_ADDR,m,v,HWIO_TCSR_PROCESSOR_0_COMPUTE_SIGNAL_MASK_KERNEL_IN)
#define HWIO_TCSR_PROCESSOR_0_COMPUTE_SIGNAL_MASK_KERNEL_PROCESSOR_0_MASK_KERNEL_BMSK                                 0xffffffff
#define HWIO_TCSR_PROCESSOR_0_COMPUTE_SIGNAL_MASK_KERNEL_PROCESSOR_0_MASK_KERNEL_SHFT                                        0x0

#define HWIO_TCSR_PROCESSOR_1_COMPUTE_SIGNAL_STATUS_KERNEL_ADDR                                                       (TCSR_TCSR_REGS_REG_BASE      + 0x0001f000)
#define HWIO_TCSR_PROCESSOR_1_COMPUTE_SIGNAL_STATUS_KERNEL_OFFS                                                       (TCSR_TCSR_REGS_REG_BASE_OFFS + 0x0001f000)
#define HWIO_TCSR_PROCESSOR_1_COMPUTE_SIGNAL_STATUS_KERNEL_RMSK                                                       0xffffffff
#define HWIO_TCSR_PROCESSOR_1_COMPUTE_SIGNAL_STATUS_KERNEL_IN          \
        in_dword_masked(HWIO_TCSR_PROCESSOR_1_COMPUTE_SIGNAL_STATUS_KERNEL_ADDR, HWIO_TCSR_PROCESSOR_1_COMPUTE_SIGNAL_STATUS_KERNEL_RMSK)
#define HWIO_TCSR_PROCESSOR_1_COMPUTE_SIGNAL_STATUS_KERNEL_INM(m)      \
        in_dword_masked(HWIO_TCSR_PROCESSOR_1_COMPUTE_SIGNAL_STATUS_KERNEL_ADDR, m)
#define HWIO_TCSR_PROCESSOR_1_COMPUTE_SIGNAL_STATUS_KERNEL_PROCESSOR_1_STATUS_KERNEL_BMSK                             0xffffffff
#define HWIO_TCSR_PROCESSOR_1_COMPUTE_SIGNAL_STATUS_KERNEL_PROCESSOR_1_STATUS_KERNEL_SHFT                                    0x0

#define HWIO_TCSR_PROCESSOR_1_COMPUTE_SIGNAL_CLEAR_KERNEL_ADDR                                                        (TCSR_TCSR_REGS_REG_BASE      + 0x0001f400)
#define HWIO_TCSR_PROCESSOR_1_COMPUTE_SIGNAL_CLEAR_KERNEL_OFFS                                                        (TCSR_TCSR_REGS_REG_BASE_OFFS + 0x0001f400)
#define HWIO_TCSR_PROCESSOR_1_COMPUTE_SIGNAL_CLEAR_KERNEL_RMSK                                                        0xffffffff
#define HWIO_TCSR_PROCESSOR_1_COMPUTE_SIGNAL_CLEAR_KERNEL_OUT(v)      \
        out_dword(HWIO_TCSR_PROCESSOR_1_COMPUTE_SIGNAL_CLEAR_KERNEL_ADDR,v)
#define HWIO_TCSR_PROCESSOR_1_COMPUTE_SIGNAL_CLEAR_KERNEL_PROCESSOR_1_CLEAR_KERNEL_BMSK                               0xffffffff
#define HWIO_TCSR_PROCESSOR_1_COMPUTE_SIGNAL_CLEAR_KERNEL_PROCESSOR_1_CLEAR_KERNEL_SHFT                                      0x0

#define HWIO_TCSR_PROCESSOR_1_COMPUTE_SIGNAL_MASK_KERNEL_ADDR                                                         (TCSR_TCSR_REGS_REG_BASE      + 0x0001f800)
#define HWIO_TCSR_PROCESSOR_1_COMPUTE_SIGNAL_MASK_KERNEL_OFFS                                                         (TCSR_TCSR_REGS_REG_BASE_OFFS + 0x0001f800)
#define HWIO_TCSR_PROCESSOR_1_COMPUTE_SIGNAL_MASK_KERNEL_RMSK                                                         0xffffffff
#define HWIO_TCSR_PROCESSOR_1_COMPUTE_SIGNAL_MASK_KERNEL_IN          \
        in_dword_masked(HWIO_TCSR_PROCESSOR_1_COMPUTE_SIGNAL_MASK_KERNEL_ADDR, HWIO_TCSR_PROCESSOR_1_COMPUTE_SIGNAL_MASK_KERNEL_RMSK)
#define HWIO_TCSR_PROCESSOR_1_COMPUTE_SIGNAL_MASK_KERNEL_INM(m)      \
        in_dword_masked(HWIO_TCSR_PROCESSOR_1_COMPUTE_SIGNAL_MASK_KERNEL_ADDR, m)
#define HWIO_TCSR_PROCESSOR_1_COMPUTE_SIGNAL_MASK_KERNEL_OUT(v)      \
        out_dword(HWIO_TCSR_PROCESSOR_1_COMPUTE_SIGNAL_MASK_KERNEL_ADDR,v)
#define HWIO_TCSR_PROCESSOR_1_COMPUTE_SIGNAL_MASK_KERNEL_OUTM(m,v) \
        out_dword_masked_ns(HWIO_TCSR_PROCESSOR_1_COMPUTE_SIGNAL_MASK_KERNEL_ADDR,m,v,HWIO_TCSR_PROCESSOR_1_COMPUTE_SIGNAL_MASK_KERNEL_IN)
#define HWIO_TCSR_PROCESSOR_1_COMPUTE_SIGNAL_MASK_KERNEL_PROCESSOR_1_MASK_KERNEL_BMSK                                 0xffffffff
#define HWIO_TCSR_PROCESSOR_1_COMPUTE_SIGNAL_MASK_KERNEL_PROCESSOR_1_MASK_KERNEL_SHFT                                        0x0

#define HWIO_TCSR_PROCESSOR_2_COMPUTE_SIGNAL_STATUS_KERNEL_ADDR                                                       (TCSR_TCSR_REGS_REG_BASE      + 0x00020000)
#define HWIO_TCSR_PROCESSOR_2_COMPUTE_SIGNAL_STATUS_KERNEL_OFFS                                                       (TCSR_TCSR_REGS_REG_BASE_OFFS + 0x00020000)
#define HWIO_TCSR_PROCESSOR_2_COMPUTE_SIGNAL_STATUS_KERNEL_RMSK                                                       0xffffffff
#define HWIO_TCSR_PROCESSOR_2_COMPUTE_SIGNAL_STATUS_KERNEL_IN          \
        in_dword_masked(HWIO_TCSR_PROCESSOR_2_COMPUTE_SIGNAL_STATUS_KERNEL_ADDR, HWIO_TCSR_PROCESSOR_2_COMPUTE_SIGNAL_STATUS_KERNEL_RMSK)
#define HWIO_TCSR_PROCESSOR_2_COMPUTE_SIGNAL_STATUS_KERNEL_INM(m)      \
        in_dword_masked(HWIO_TCSR_PROCESSOR_2_COMPUTE_SIGNAL_STATUS_KERNEL_ADDR, m)
#define HWIO_TCSR_PROCESSOR_2_COMPUTE_SIGNAL_STATUS_KERNEL_PROCESSOR_2_STATUS_KERNEL_BMSK                             0xffffffff
#define HWIO_TCSR_PROCESSOR_2_COMPUTE_SIGNAL_STATUS_KERNEL_PROCESSOR_2_STATUS_KERNEL_SHFT                                    0x0

#define HWIO_TCSR_PROCESSOR_2_COMPUTE_SIGNAL_CLEAR_KERNEL_ADDR                                                        (TCSR_TCSR_REGS_REG_BASE      + 0x00020400)
#define HWIO_TCSR_PROCESSOR_2_COMPUTE_SIGNAL_CLEAR_KERNEL_OFFS                                                        (TCSR_TCSR_REGS_REG_BASE_OFFS + 0x00020400)
#define HWIO_TCSR_PROCESSOR_2_COMPUTE_SIGNAL_CLEAR_KERNEL_RMSK                                                        0xffffffff
#define HWIO_TCSR_PROCESSOR_2_COMPUTE_SIGNAL_CLEAR_KERNEL_OUT(v)      \
        out_dword(HWIO_TCSR_PROCESSOR_2_COMPUTE_SIGNAL_CLEAR_KERNEL_ADDR,v)
#define HWIO_TCSR_PROCESSOR_2_COMPUTE_SIGNAL_CLEAR_KERNEL_PROCESSOR_2_CLEAR_KERNEL_BMSK                               0xffffffff
#define HWIO_TCSR_PROCESSOR_2_COMPUTE_SIGNAL_CLEAR_KERNEL_PROCESSOR_2_CLEAR_KERNEL_SHFT                                      0x0

#define HWIO_TCSR_PROCESSOR_2_COMPUTE_SIGNAL_MASK_KERNEL_ADDR                                                         (TCSR_TCSR_REGS_REG_BASE      + 0x00020800)
#define HWIO_TCSR_PROCESSOR_2_COMPUTE_SIGNAL_MASK_KERNEL_OFFS                                                         (TCSR_TCSR_REGS_REG_BASE_OFFS + 0x00020800)
#define HWIO_TCSR_PROCESSOR_2_COMPUTE_SIGNAL_MASK_KERNEL_RMSK                                                         0xffffffff
#define HWIO_TCSR_PROCESSOR_2_COMPUTE_SIGNAL_MASK_KERNEL_IN          \
        in_dword_masked(HWIO_TCSR_PROCESSOR_2_COMPUTE_SIGNAL_MASK_KERNEL_ADDR, HWIO_TCSR_PROCESSOR_2_COMPUTE_SIGNAL_MASK_KERNEL_RMSK)
#define HWIO_TCSR_PROCESSOR_2_COMPUTE_SIGNAL_MASK_KERNEL_INM(m)      \
        in_dword_masked(HWIO_TCSR_PROCESSOR_2_COMPUTE_SIGNAL_MASK_KERNEL_ADDR, m)
#define HWIO_TCSR_PROCESSOR_2_COMPUTE_SIGNAL_MASK_KERNEL_OUT(v)      \
        out_dword(HWIO_TCSR_PROCESSOR_2_COMPUTE_SIGNAL_MASK_KERNEL_ADDR,v)
#define HWIO_TCSR_PROCESSOR_2_COMPUTE_SIGNAL_MASK_KERNEL_OUTM(m,v) \
        out_dword_masked_ns(HWIO_TCSR_PROCESSOR_2_COMPUTE_SIGNAL_MASK_KERNEL_ADDR,m,v,HWIO_TCSR_PROCESSOR_2_COMPUTE_SIGNAL_MASK_KERNEL_IN)
#define HWIO_TCSR_PROCESSOR_2_COMPUTE_SIGNAL_MASK_KERNEL_PROCESSOR_2_MASK_KERNEL_BMSK                                 0xffffffff
#define HWIO_TCSR_PROCESSOR_2_COMPUTE_SIGNAL_MASK_KERNEL_PROCESSOR_2_MASK_KERNEL_SHFT                                        0x0

#define HWIO_TCSR_SS_XPU_SP_INTR0_ADDR                                                                                (TCSR_TCSR_REGS_REG_BASE      + 0x00021000)
#define HWIO_TCSR_SS_XPU_SP_INTR0_OFFS                                                                                (TCSR_TCSR_REGS_REG_BASE_OFFS + 0x00021000)
#define HWIO_TCSR_SS_XPU_SP_INTR0_RMSK                                                                                0xffffffff
#define HWIO_TCSR_SS_XPU_SP_INTR0_IN          \
        in_dword_masked(HWIO_TCSR_SS_XPU_SP_INTR0_ADDR, HWIO_TCSR_SS_XPU_SP_INTR0_RMSK)
#define HWIO_TCSR_SS_XPU_SP_INTR0_INM(m)      \
        in_dword_masked(HWIO_TCSR_SS_XPU_SP_INTR0_ADDR, m)
#define HWIO_TCSR_SS_XPU_SP_INTR0_QM_XPU3_SP_INTR0_BMSK                                                               0x40000000
#define HWIO_TCSR_SS_XPU_SP_INTR0_QM_XPU3_SP_INTR0_SHFT                                                                     0x1e
#define HWIO_TCSR_SS_XPU_SP_INTR0_MEMNOC_MCDMA_XPU3_SP_INTR0_BMSK                                                     0x20000000
#define HWIO_TCSR_SS_XPU_SP_INTR0_MEMNOC_MCDMA_XPU3_SP_INTR0_SHFT                                                           0x1d
#define HWIO_TCSR_SS_XPU_SP_INTR0_SPMI_BAM_APU_SP_INTR0_BMSK                                                          0x10000000
#define HWIO_TCSR_SS_XPU_SP_INTR0_SPMI_BAM_APU_SP_INTR0_SHFT                                                                0x1c
#define HWIO_TCSR_SS_XPU_SP_INTR0_IPA_APU_SP_INTR0_BMSK                                                                0x8000000
#define HWIO_TCSR_SS_XPU_SP_INTR0_IPA_APU_SP_INTR0_SHFT                                                                     0x1b
#define HWIO_TCSR_SS_XPU_SP_INTR0_BLSP_BAM_APU_SP_INTR0_BMSK                                                           0x4000000
#define HWIO_TCSR_SS_XPU_SP_INTR0_BLSP_BAM_APU_SP_INTR0_SHFT                                                                0x1a
#define HWIO_TCSR_SS_XPU_SP_INTR0_PKA_XPU_SP_INTR0_BMSK                                                                0x2000000
#define HWIO_TCSR_SS_XPU_SP_INTR0_PKA_XPU_SP_INTR0_SHFT                                                                     0x19
#define HWIO_TCSR_SS_XPU_SP_INTR0_QPIC_XPU2_SP_INTR0_BMSK                                                              0x1000000
#define HWIO_TCSR_SS_XPU_SP_INTR0_QPIC_XPU2_SP_INTR0_SHFT                                                                   0x18
#define HWIO_TCSR_SS_XPU_SP_INTR0_QXS_Q6_TCM_SS_MPU_XPU3_SP_INTR0_BMSK                                                  0x400000
#define HWIO_TCSR_SS_XPU_SP_INTR0_QXS_Q6_TCM_SS_MPU_XPU3_SP_INTR0_SHFT                                                      0x16
#define HWIO_TCSR_SS_XPU_SP_INTR0_QXM_MSS_NAV_CE_MS_MPU_XPU3_SP_INTR0_BMSK                                              0x200000
#define HWIO_TCSR_SS_XPU_SP_INTR0_QXM_MSS_NAV_CE_MS_MPU_XPU3_SP_INTR0_SHFT                                                  0x15
#define HWIO_TCSR_SS_XPU_SP_INTR0_QNM_MEMNOC_MS_MPU_XPU3_SP_INTR0_BMSK                                                  0x100000
#define HWIO_TCSR_SS_XPU_SP_INTR0_QNM_MEMNOC_MS_MPU_XPU3_SP_INTR0_SHFT                                                      0x14
#define HWIO_TCSR_SS_XPU_SP_INTR0_QNM_AGGRE_NOC_IPA_MS_MPU_XPU3_SP_INTR0_BMSK                                            0x80000
#define HWIO_TCSR_SS_XPU_SP_INTR0_QNM_AGGRE_NOC_IPA_MS_MPU_XPU3_SP_INTR0_SHFT                                               0x13
#define HWIO_TCSR_SS_XPU_SP_INTR0_QHS_SNOC_CFG_SS_MPU_XPU3_SP_INTR0_BMSK                                                 0x40000
#define HWIO_TCSR_SS_XPU_SP_INTR0_QHS_SNOC_CFG_SS_MPU_XPU3_SP_INTR0_SHFT                                                    0x12
#define HWIO_TCSR_SS_XPU_SP_INTR0_TLMM_CENTRAL_XPU_SP_INTR0_BMSK                                                         0x20000
#define HWIO_TCSR_SS_XPU_SP_INTR0_TLMM_CENTRAL_XPU_SP_INTR0_SHFT                                                            0x11
#define HWIO_TCSR_SS_XPU_SP_INTR0_SPDM_WRAPPER_XPU_SP_INTR0_BMSK                                                         0x10000
#define HWIO_TCSR_SS_XPU_SP_INTR0_SPDM_WRAPPER_XPU_SP_INTR0_SHFT                                                            0x10
#define HWIO_TCSR_SS_XPU_SP_INTR0_OCIMEM_MPU_SP_INTR0_BMSK                                                                0x8000
#define HWIO_TCSR_SS_XPU_SP_INTR0_OCIMEM_MPU_SP_INTR0_SHFT                                                                   0xf
#define HWIO_TCSR_SS_XPU_SP_INTR0_DTP_QHS_NON_BROADCAST_MPU_MAINO_XPU3_SP_INTR0_BMSK                                      0x4000
#define HWIO_TCSR_SS_XPU_SP_INTR0_DTP_QHS_NON_BROADCAST_MPU_MAINO_XPU3_SP_INTR0_SHFT                                         0xe
#define HWIO_TCSR_SS_XPU_SP_INTR0_BOOT_ROM_SP_INTR0_BMSK                                                                  0x2000
#define HWIO_TCSR_SS_XPU_SP_INTR0_BOOT_ROM_SP_INTR0_SHFT                                                                     0xd
#define HWIO_TCSR_SS_XPU_SP_INTR0_DTP_QHS_BROADCAST_MPU_MAINO_XPU3_SP_INTR0_BMSK                                          0x1000
#define HWIO_TCSR_SS_XPU_SP_INTR0_DTP_QHS_BROADCAST_MPU_MAINO_XPU3_SP_INTR0_SHFT                                             0xc
#define HWIO_TCSR_SS_XPU_SP_INTR0_LLCC0_XPU3_SP_INTR0_BMSK                                                                 0x800
#define HWIO_TCSR_SS_XPU_SP_INTR0_LLCC0_XPU3_SP_INTR0_SHFT                                                                   0xb
#define HWIO_TCSR_SS_XPU_SP_INTR0_GCC_XPU_SP_INTR0_BMSK                                                                    0x400
#define HWIO_TCSR_SS_XPU_SP_INTR0_GCC_XPU_SP_INTR0_SHFT                                                                      0xa
#define HWIO_TCSR_SS_XPU_SP_INTR0_MEMNOC_XPU3_SP_INTR0_BMSK                                                                0x200
#define HWIO_TCSR_SS_XPU_SP_INTR0_MEMNOC_XPU3_SP_INTR0_SHFT                                                                  0x9
#define HWIO_TCSR_SS_XPU_SP_INTR0_QHM_AOSS_MS_MPU_XPU3_SP_INTR0_BMSK                                                       0x100
#define HWIO_TCSR_SS_XPU_SP_INTR0_QHM_AOSS_MS_MPU_XPU3_SP_INTR0_SHFT                                                         0x8
#define HWIO_TCSR_SS_XPU_SP_INTR0_SEC_CTRL_XPU3_SP_INTR0_BMSK                                                               0x80
#define HWIO_TCSR_SS_XPU_SP_INTR0_SEC_CTRL_XPU3_SP_INTR0_SHFT                                                                0x7
#define HWIO_TCSR_SS_XPU_SP_INTR0_DCC_XPU_SP_INTR0_BMSK                                                                     0x40
#define HWIO_TCSR_SS_XPU_SP_INTR0_DCC_XPU_SP_INTR0_SHFT                                                                      0x6
#define HWIO_TCSR_SS_XPU_SP_INTR0_CRYPTO_BAM_APU_SP_INTR0_BMSK                                                              0x10
#define HWIO_TCSR_SS_XPU_SP_INTR0_CRYPTO_BAM_APU_SP_INTR0_SHFT                                                               0x4
#define HWIO_TCSR_SS_XPU_SP_INTR0_TCSR_MUTEX_XPU_SP_INTR0_BMSK                                                               0x8
#define HWIO_TCSR_SS_XPU_SP_INTR0_TCSR_MUTEX_XPU_SP_INTR0_SHFT                                                               0x3
#define HWIO_TCSR_SS_XPU_SP_INTR0_TCSR_REGS_XPU_SP_INTR0_BMSK                                                                0x4
#define HWIO_TCSR_SS_XPU_SP_INTR0_TCSR_REGS_XPU_SP_INTR0_SHFT                                                                0x2
#define HWIO_TCSR_SS_XPU_SP_INTR0_AOSS_MPU_SP_INTR0_BMSK                                                                     0x2
#define HWIO_TCSR_SS_XPU_SP_INTR0_AOSS_MPU_SP_INTR0_SHFT                                                                     0x1
#define HWIO_TCSR_SS_XPU_SP_INTR0_PMIC_ARB_MPU_SP_INTR0_BMSK                                                                 0x1
#define HWIO_TCSR_SS_XPU_SP_INTR0_PMIC_ARB_MPU_SP_INTR0_SHFT                                                                 0x0

#define HWIO_TCSR_SS_XPU_SP_INTR1_ADDR                                                                                (TCSR_TCSR_REGS_REG_BASE      + 0x00021004)
#define HWIO_TCSR_SS_XPU_SP_INTR1_OFFS                                                                                (TCSR_TCSR_REGS_REG_BASE_OFFS + 0x00021004)
#define HWIO_TCSR_SS_XPU_SP_INTR1_RMSK                                                                                   0x7ffff
#define HWIO_TCSR_SS_XPU_SP_INTR1_IN          \
        in_dword_masked(HWIO_TCSR_SS_XPU_SP_INTR1_ADDR, HWIO_TCSR_SS_XPU_SP_INTR1_RMSK)
#define HWIO_TCSR_SS_XPU_SP_INTR1_INM(m)      \
        in_dword_masked(HWIO_TCSR_SS_XPU_SP_INTR1_ADDR, m)

#define HWIO_TCSR_SS_XPU_SP_INTR0_ENABLE_ADDR                                                                         (TCSR_TCSR_REGS_REG_BASE      + 0x00021008)
#define HWIO_TCSR_SS_XPU_SP_INTR0_ENABLE_OFFS                                                                         (TCSR_TCSR_REGS_REG_BASE_OFFS + 0x00021008)
#define HWIO_TCSR_SS_XPU_SP_INTR0_ENABLE_RMSK                                                                         0xffffffff
#define HWIO_TCSR_SS_XPU_SP_INTR0_ENABLE_IN          \
        in_dword_masked(HWIO_TCSR_SS_XPU_SP_INTR0_ENABLE_ADDR, HWIO_TCSR_SS_XPU_SP_INTR0_ENABLE_RMSK)
#define HWIO_TCSR_SS_XPU_SP_INTR0_ENABLE_INM(m)      \
        in_dword_masked(HWIO_TCSR_SS_XPU_SP_INTR0_ENABLE_ADDR, m)
#define HWIO_TCSR_SS_XPU_SP_INTR0_ENABLE_OUT(v)      \
        out_dword(HWIO_TCSR_SS_XPU_SP_INTR0_ENABLE_ADDR,v)
#define HWIO_TCSR_SS_XPU_SP_INTR0_ENABLE_OUTM(m,v) \
        out_dword_masked_ns(HWIO_TCSR_SS_XPU_SP_INTR0_ENABLE_ADDR,m,v,HWIO_TCSR_SS_XPU_SP_INTR0_ENABLE_IN)
#define HWIO_TCSR_SS_XPU_SP_INTR0_ENABLE_QM_XPU3_SP_INTR0_ENABLE_BMSK                                                 0x40000000
#define HWIO_TCSR_SS_XPU_SP_INTR0_ENABLE_QM_XPU3_SP_INTR0_ENABLE_SHFT                                                       0x1e
#define HWIO_TCSR_SS_XPU_SP_INTR0_ENABLE_MEMNOC_MCDMA_XPU3_SP_INTR0_ENABLE_BMSK                                       0x20000000
#define HWIO_TCSR_SS_XPU_SP_INTR0_ENABLE_MEMNOC_MCDMA_XPU3_SP_INTR0_ENABLE_SHFT                                             0x1d
#define HWIO_TCSR_SS_XPU_SP_INTR0_ENABLE_SPMI_BAM_APU_SP_INTR0_ENABLE_BMSK                                            0x10000000
#define HWIO_TCSR_SS_XPU_SP_INTR0_ENABLE_SPMI_BAM_APU_SP_INTR0_ENABLE_SHFT                                                  0x1c
#define HWIO_TCSR_SS_XPU_SP_INTR0_ENABLE_IPA_APU_SP_INTR0_ENABLE_BMSK                                                  0x8000000
#define HWIO_TCSR_SS_XPU_SP_INTR0_ENABLE_IPA_APU_SP_INTR0_ENABLE_SHFT                                                       0x1b
#define HWIO_TCSR_SS_XPU_SP_INTR0_ENABLE_BLSP_BAM_APU_SP_INTR0_ENABLE_BMSK                                             0x4000000
#define HWIO_TCSR_SS_XPU_SP_INTR0_ENABLE_BLSP_BAM_APU_SP_INTR0_ENABLE_SHFT                                                  0x1a
#define HWIO_TCSR_SS_XPU_SP_INTR0_ENABLE_PKA_XPU_SP_INTR0_ENABLE_BMSK                                                  0x2000000
#define HWIO_TCSR_SS_XPU_SP_INTR0_ENABLE_PKA_XPU_SP_INTR0_ENABLE_SHFT                                                       0x19
#define HWIO_TCSR_SS_XPU_SP_INTR0_ENABLE_QPIC_XPU2_SP_INTR0_ENABLE_BMSK                                                0x1000000
#define HWIO_TCSR_SS_XPU_SP_INTR0_ENABLE_QPIC_XPU2_SP_INTR0_ENABLE_SHFT                                                     0x18
#define HWIO_TCSR_SS_XPU_SP_INTR0_ENABLE_QXS_Q6_TCM_SS_MPU_XPU3_SP_INTR0_ENABLE_BMSK                                    0x400000
#define HWIO_TCSR_SS_XPU_SP_INTR0_ENABLE_QXS_Q6_TCM_SS_MPU_XPU3_SP_INTR0_ENABLE_SHFT                                        0x16
#define HWIO_TCSR_SS_XPU_SP_INTR0_ENABLE_QXM_MSS_NAV_CE_MS_MPU_XPU3_SP_INTR0_ENABLE_BMSK                                0x200000
#define HWIO_TCSR_SS_XPU_SP_INTR0_ENABLE_QXM_MSS_NAV_CE_MS_MPU_XPU3_SP_INTR0_ENABLE_SHFT                                    0x15
#define HWIO_TCSR_SS_XPU_SP_INTR0_ENABLE_QNM_MEMNOC_MS_MPU_XPU3_SP_INTR0_ENABLE_BMSK                                    0x100000
#define HWIO_TCSR_SS_XPU_SP_INTR0_ENABLE_QNM_MEMNOC_MS_MPU_XPU3_SP_INTR0_ENABLE_SHFT                                        0x14
#define HWIO_TCSR_SS_XPU_SP_INTR0_ENABLE_QNM_AGGRE_NOC_IPA_MS_MPU_XPU3_SP_INTR0_ENABLE_BMSK                              0x80000
#define HWIO_TCSR_SS_XPU_SP_INTR0_ENABLE_QNM_AGGRE_NOC_IPA_MS_MPU_XPU3_SP_INTR0_ENABLE_SHFT                                 0x13
#define HWIO_TCSR_SS_XPU_SP_INTR0_ENABLE_QHS_SNOC_CFG_SS_MPU_XPU3_SP_INTR0_ENABLE_BMSK                                   0x40000
#define HWIO_TCSR_SS_XPU_SP_INTR0_ENABLE_QHS_SNOC_CFG_SS_MPU_XPU3_SP_INTR0_ENABLE_SHFT                                      0x12
#define HWIO_TCSR_SS_XPU_SP_INTR0_ENABLE_TLMM_CENTRAL_XPU_SP_INTR0_ENABLE_BMSK                                           0x20000
#define HWIO_TCSR_SS_XPU_SP_INTR0_ENABLE_TLMM_CENTRAL_XPU_SP_INTR0_ENABLE_SHFT                                              0x11
#define HWIO_TCSR_SS_XPU_SP_INTR0_ENABLE_SPDM_WRAPPER_XPU_SP_INTR0_ENABLE_BMSK                                           0x10000
#define HWIO_TCSR_SS_XPU_SP_INTR0_ENABLE_SPDM_WRAPPER_XPU_SP_INTR0_ENABLE_SHFT                                              0x10
#define HWIO_TCSR_SS_XPU_SP_INTR0_ENABLE_OCIMEM_MPU_SP_INTR0_ENABLE_BMSK                                                  0x8000
#define HWIO_TCSR_SS_XPU_SP_INTR0_ENABLE_OCIMEM_MPU_SP_INTR0_ENABLE_SHFT                                                     0xf
#define HWIO_TCSR_SS_XPU_SP_INTR0_ENABLE_DTP_QHS_NON_BROADCAST_MPU_MAINO_XPU3_SP_INTR0_ENABLE_BMSK                        0x4000
#define HWIO_TCSR_SS_XPU_SP_INTR0_ENABLE_DTP_QHS_NON_BROADCAST_MPU_MAINO_XPU3_SP_INTR0_ENABLE_SHFT                           0xe
#define HWIO_TCSR_SS_XPU_SP_INTR0_ENABLE_BOOT_ROM_SP_INTR0_ENABLE_BMSK                                                    0x2000
#define HWIO_TCSR_SS_XPU_SP_INTR0_ENABLE_BOOT_ROM_SP_INTR0_ENABLE_SHFT                                                       0xd
#define HWIO_TCSR_SS_XPU_SP_INTR0_ENABLE_DTP_QHS_BROADCAST_MPU_MAINO_XPU3_SP_INTR0_ENABLE_BMSK                            0x1000
#define HWIO_TCSR_SS_XPU_SP_INTR0_ENABLE_DTP_QHS_BROADCAST_MPU_MAINO_XPU3_SP_INTR0_ENABLE_SHFT                               0xc
#define HWIO_TCSR_SS_XPU_SP_INTR0_ENABLE_LLCC0_XPU3_SP_INTR0_ENABLE_BMSK                                                   0x800
#define HWIO_TCSR_SS_XPU_SP_INTR0_ENABLE_LLCC0_XPU3_SP_INTR0_ENABLE_SHFT                                                     0xb
#define HWIO_TCSR_SS_XPU_SP_INTR0_ENABLE_GCC_XPU_SP_INTR0_ENABLE_BMSK                                                      0x400
#define HWIO_TCSR_SS_XPU_SP_INTR0_ENABLE_GCC_XPU_SP_INTR0_ENABLE_SHFT                                                        0xa
#define HWIO_TCSR_SS_XPU_SP_INTR0_ENABLE_MEMNOC_XPU3_SP_INTR0_ENABLE_BMSK                                                  0x200
#define HWIO_TCSR_SS_XPU_SP_INTR0_ENABLE_MEMNOC_XPU3_SP_INTR0_ENABLE_SHFT                                                    0x9
#define HWIO_TCSR_SS_XPU_SP_INTR0_ENABLE_QHM_AOSS_MS_MPU_XPU3_SP_INTR0_ENABLE_BMSK                                         0x100
#define HWIO_TCSR_SS_XPU_SP_INTR0_ENABLE_QHM_AOSS_MS_MPU_XPU3_SP_INTR0_ENABLE_SHFT                                           0x8
#define HWIO_TCSR_SS_XPU_SP_INTR0_ENABLE_SEC_CTRL_XPU3_SP_INTR0_ENABLE_BMSK                                                 0x80
#define HWIO_TCSR_SS_XPU_SP_INTR0_ENABLE_SEC_CTRL_XPU3_SP_INTR0_ENABLE_SHFT                                                  0x7
#define HWIO_TCSR_SS_XPU_SP_INTR0_ENABLE_DCC_XPU_SP_INTR0_ENABLE_BMSK                                                       0x40
#define HWIO_TCSR_SS_XPU_SP_INTR0_ENABLE_DCC_XPU_SP_INTR0_ENABLE_SHFT                                                        0x6
#define HWIO_TCSR_SS_XPU_SP_INTR0_ENABLE_CRYPTO_BAM_APU_SP_INTR0_ENABLE_BMSK                                                0x10
#define HWIO_TCSR_SS_XPU_SP_INTR0_ENABLE_CRYPTO_BAM_APU_SP_INTR0_ENABLE_SHFT                                                 0x4
#define HWIO_TCSR_SS_XPU_SP_INTR0_ENABLE_TCSR_MUTEX_XPU_SP_INTR0_ENABLE_BMSK                                                 0x8
#define HWIO_TCSR_SS_XPU_SP_INTR0_ENABLE_TCSR_MUTEX_XPU_SP_INTR0_ENABLE_SHFT                                                 0x3
#define HWIO_TCSR_SS_XPU_SP_INTR0_ENABLE_TCSR_REGS_XPU_SP_INTR0_ENABLE_BMSK                                                  0x4
#define HWIO_TCSR_SS_XPU_SP_INTR0_ENABLE_TCSR_REGS_XPU_SP_INTR0_ENABLE_SHFT                                                  0x2
#define HWIO_TCSR_SS_XPU_SP_INTR0_ENABLE_AOSS_MPU_SP_INTR0_ENABLE_BMSK                                                       0x2
#define HWIO_TCSR_SS_XPU_SP_INTR0_ENABLE_AOSS_MPU_SP_INTR0_ENABLE_SHFT                                                       0x1
#define HWIO_TCSR_SS_XPU_SP_INTR0_ENABLE_PMIC_ARB_MPU_SP_INTR0_ENABLE_BMSK                                                   0x1
#define HWIO_TCSR_SS_XPU_SP_INTR0_ENABLE_PMIC_ARB_MPU_SP_INTR0_ENABLE_SHFT                                                   0x0

#define HWIO_TCSR_SS_XPU_SP_INTR1_ENABLE_ADDR                                                                         (TCSR_TCSR_REGS_REG_BASE      + 0x0002100c)
#define HWIO_TCSR_SS_XPU_SP_INTR1_ENABLE_OFFS                                                                         (TCSR_TCSR_REGS_REG_BASE_OFFS + 0x0002100c)
#define HWIO_TCSR_SS_XPU_SP_INTR1_ENABLE_RMSK                                                                            0x7ffff
#define HWIO_TCSR_SS_XPU_SP_INTR1_ENABLE_IN          \
        in_dword_masked(HWIO_TCSR_SS_XPU_SP_INTR1_ENABLE_ADDR, HWIO_TCSR_SS_XPU_SP_INTR1_ENABLE_RMSK)
#define HWIO_TCSR_SS_XPU_SP_INTR1_ENABLE_INM(m)      \
        in_dword_masked(HWIO_TCSR_SS_XPU_SP_INTR1_ENABLE_ADDR, m)
#define HWIO_TCSR_SS_XPU_SP_INTR1_ENABLE_OUT(v)      \
        out_dword(HWIO_TCSR_SS_XPU_SP_INTR1_ENABLE_ADDR,v)
#define HWIO_TCSR_SS_XPU_SP_INTR1_ENABLE_OUTM(m,v) \
        out_dword_masked_ns(HWIO_TCSR_SS_XPU_SP_INTR1_ENABLE_ADDR,m,v,HWIO_TCSR_SS_XPU_SP_INTR1_ENABLE_IN)

#define HWIO_TCSR_DBGOVR_COMPILER_MEM_ACC_SEL_ADDR                                                                    (TCSR_TCSR_REGS_REG_BASE      + 0x00022000)
#define HWIO_TCSR_DBGOVR_COMPILER_MEM_ACC_SEL_OFFS                                                                    (TCSR_TCSR_REGS_REG_BASE_OFFS + 0x00022000)
#define HWIO_TCSR_DBGOVR_COMPILER_MEM_ACC_SEL_RMSK                                                                           0x1
#define HWIO_TCSR_DBGOVR_COMPILER_MEM_ACC_SEL_IN          \
        in_dword_masked(HWIO_TCSR_DBGOVR_COMPILER_MEM_ACC_SEL_ADDR, HWIO_TCSR_DBGOVR_COMPILER_MEM_ACC_SEL_RMSK)
#define HWIO_TCSR_DBGOVR_COMPILER_MEM_ACC_SEL_INM(m)      \
        in_dword_masked(HWIO_TCSR_DBGOVR_COMPILER_MEM_ACC_SEL_ADDR, m)
#define HWIO_TCSR_DBGOVR_COMPILER_MEM_ACC_SEL_OUT(v)      \
        out_dword(HWIO_TCSR_DBGOVR_COMPILER_MEM_ACC_SEL_ADDR,v)
#define HWIO_TCSR_DBGOVR_COMPILER_MEM_ACC_SEL_OUTM(m,v) \
        out_dword_masked_ns(HWIO_TCSR_DBGOVR_COMPILER_MEM_ACC_SEL_ADDR,m,v,HWIO_TCSR_DBGOVR_COMPILER_MEM_ACC_SEL_IN)
#define HWIO_TCSR_DBGOVR_COMPILER_MEM_ACC_SEL_DBGOVR_COMPILER_MEM_ACC_SEL_BMSK                                               0x1
#define HWIO_TCSR_DBGOVR_COMPILER_MEM_ACC_SEL_DBGOVR_COMPILER_MEM_ACC_SEL_SHFT                                               0x0

#define HWIO_TCSR_DBGOVR_CUSTOM_MEM_ACC_SEL_ADDR                                                                      (TCSR_TCSR_REGS_REG_BASE      + 0x00022004)
#define HWIO_TCSR_DBGOVR_CUSTOM_MEM_ACC_SEL_OFFS                                                                      (TCSR_TCSR_REGS_REG_BASE_OFFS + 0x00022004)
#define HWIO_TCSR_DBGOVR_CUSTOM_MEM_ACC_SEL_RMSK                                                                             0x1
#define HWIO_TCSR_DBGOVR_CUSTOM_MEM_ACC_SEL_IN          \
        in_dword_masked(HWIO_TCSR_DBGOVR_CUSTOM_MEM_ACC_SEL_ADDR, HWIO_TCSR_DBGOVR_CUSTOM_MEM_ACC_SEL_RMSK)
#define HWIO_TCSR_DBGOVR_CUSTOM_MEM_ACC_SEL_INM(m)      \
        in_dword_masked(HWIO_TCSR_DBGOVR_CUSTOM_MEM_ACC_SEL_ADDR, m)
#define HWIO_TCSR_DBGOVR_CUSTOM_MEM_ACC_SEL_OUT(v)      \
        out_dword(HWIO_TCSR_DBGOVR_CUSTOM_MEM_ACC_SEL_ADDR,v)
#define HWIO_TCSR_DBGOVR_CUSTOM_MEM_ACC_SEL_OUTM(m,v) \
        out_dword_masked_ns(HWIO_TCSR_DBGOVR_CUSTOM_MEM_ACC_SEL_ADDR,m,v,HWIO_TCSR_DBGOVR_CUSTOM_MEM_ACC_SEL_IN)
#define HWIO_TCSR_DBGOVR_CUSTOM_MEM_ACC_SEL_DBGOVR_CUSTOM_MEM_ACC_SEL_BMSK                                                   0x1
#define HWIO_TCSR_DBGOVR_CUSTOM_MEM_ACC_SEL_DBGOVR_CUSTOM_MEM_ACC_SEL_SHFT                                                   0x0

#define HWIO_TCSR_DBGOVR_VALUE_COMPILER_MEM_ACC_0_ADDR                                                                (TCSR_TCSR_REGS_REG_BASE      + 0x00022010)
#define HWIO_TCSR_DBGOVR_VALUE_COMPILER_MEM_ACC_0_OFFS                                                                (TCSR_TCSR_REGS_REG_BASE_OFFS + 0x00022010)
#define HWIO_TCSR_DBGOVR_VALUE_COMPILER_MEM_ACC_0_RMSK                                                                      0xff
#define HWIO_TCSR_DBGOVR_VALUE_COMPILER_MEM_ACC_0_IN          \
        in_dword_masked(HWIO_TCSR_DBGOVR_VALUE_COMPILER_MEM_ACC_0_ADDR, HWIO_TCSR_DBGOVR_VALUE_COMPILER_MEM_ACC_0_RMSK)
#define HWIO_TCSR_DBGOVR_VALUE_COMPILER_MEM_ACC_0_INM(m)      \
        in_dword_masked(HWIO_TCSR_DBGOVR_VALUE_COMPILER_MEM_ACC_0_ADDR, m)
#define HWIO_TCSR_DBGOVR_VALUE_COMPILER_MEM_ACC_0_OUT(v)      \
        out_dword(HWIO_TCSR_DBGOVR_VALUE_COMPILER_MEM_ACC_0_ADDR,v)
#define HWIO_TCSR_DBGOVR_VALUE_COMPILER_MEM_ACC_0_OUTM(m,v) \
        out_dword_masked_ns(HWIO_TCSR_DBGOVR_VALUE_COMPILER_MEM_ACC_0_ADDR,m,v,HWIO_TCSR_DBGOVR_VALUE_COMPILER_MEM_ACC_0_IN)
#define HWIO_TCSR_DBGOVR_VALUE_COMPILER_MEM_ACC_0_DBGOVR_VALUE_COMPILER_MEM_ACC_0_BMSK                                      0xff
#define HWIO_TCSR_DBGOVR_VALUE_COMPILER_MEM_ACC_0_DBGOVR_VALUE_COMPILER_MEM_ACC_0_SHFT                                       0x0

#define HWIO_TCSR_DBGOVR_VALUE_COMPILER_MEM_ACC_1_ADDR                                                                (TCSR_TCSR_REGS_REG_BASE      + 0x00022014)
#define HWIO_TCSR_DBGOVR_VALUE_COMPILER_MEM_ACC_1_OFFS                                                                (TCSR_TCSR_REGS_REG_BASE_OFFS + 0x00022014)
#define HWIO_TCSR_DBGOVR_VALUE_COMPILER_MEM_ACC_1_RMSK                                                                      0xff
#define HWIO_TCSR_DBGOVR_VALUE_COMPILER_MEM_ACC_1_IN          \
        in_dword_masked(HWIO_TCSR_DBGOVR_VALUE_COMPILER_MEM_ACC_1_ADDR, HWIO_TCSR_DBGOVR_VALUE_COMPILER_MEM_ACC_1_RMSK)
#define HWIO_TCSR_DBGOVR_VALUE_COMPILER_MEM_ACC_1_INM(m)      \
        in_dword_masked(HWIO_TCSR_DBGOVR_VALUE_COMPILER_MEM_ACC_1_ADDR, m)
#define HWIO_TCSR_DBGOVR_VALUE_COMPILER_MEM_ACC_1_OUT(v)      \
        out_dword(HWIO_TCSR_DBGOVR_VALUE_COMPILER_MEM_ACC_1_ADDR,v)
#define HWIO_TCSR_DBGOVR_VALUE_COMPILER_MEM_ACC_1_OUTM(m,v) \
        out_dword_masked_ns(HWIO_TCSR_DBGOVR_VALUE_COMPILER_MEM_ACC_1_ADDR,m,v,HWIO_TCSR_DBGOVR_VALUE_COMPILER_MEM_ACC_1_IN)
#define HWIO_TCSR_DBGOVR_VALUE_COMPILER_MEM_ACC_1_DBGOVR_VALUE_COMPILER_MEM_ACC_1_BMSK                                      0xff
#define HWIO_TCSR_DBGOVR_VALUE_COMPILER_MEM_ACC_1_DBGOVR_VALUE_COMPILER_MEM_ACC_1_SHFT                                       0x0

#define HWIO_TCSR_DBGOVR_VALUE_COMPILER_MEM_ACC_2_ADDR                                                                (TCSR_TCSR_REGS_REG_BASE      + 0x00022018)
#define HWIO_TCSR_DBGOVR_VALUE_COMPILER_MEM_ACC_2_OFFS                                                                (TCSR_TCSR_REGS_REG_BASE_OFFS + 0x00022018)
#define HWIO_TCSR_DBGOVR_VALUE_COMPILER_MEM_ACC_2_RMSK                                                                      0xff
#define HWIO_TCSR_DBGOVR_VALUE_COMPILER_MEM_ACC_2_IN          \
        in_dword_masked(HWIO_TCSR_DBGOVR_VALUE_COMPILER_MEM_ACC_2_ADDR, HWIO_TCSR_DBGOVR_VALUE_COMPILER_MEM_ACC_2_RMSK)
#define HWIO_TCSR_DBGOVR_VALUE_COMPILER_MEM_ACC_2_INM(m)      \
        in_dword_masked(HWIO_TCSR_DBGOVR_VALUE_COMPILER_MEM_ACC_2_ADDR, m)
#define HWIO_TCSR_DBGOVR_VALUE_COMPILER_MEM_ACC_2_OUT(v)      \
        out_dword(HWIO_TCSR_DBGOVR_VALUE_COMPILER_MEM_ACC_2_ADDR,v)
#define HWIO_TCSR_DBGOVR_VALUE_COMPILER_MEM_ACC_2_OUTM(m,v) \
        out_dword_masked_ns(HWIO_TCSR_DBGOVR_VALUE_COMPILER_MEM_ACC_2_ADDR,m,v,HWIO_TCSR_DBGOVR_VALUE_COMPILER_MEM_ACC_2_IN)
#define HWIO_TCSR_DBGOVR_VALUE_COMPILER_MEM_ACC_2_DBGOVR_VALUE_COMPILER_MEM_ACC_2_BMSK                                      0xff
#define HWIO_TCSR_DBGOVR_VALUE_COMPILER_MEM_ACC_2_DBGOVR_VALUE_COMPILER_MEM_ACC_2_SHFT                                       0x0

#define HWIO_TCSR_DBGOVR_VALUE_COMPILER_MEM_ACC_3_ADDR                                                                (TCSR_TCSR_REGS_REG_BASE      + 0x0002201c)
#define HWIO_TCSR_DBGOVR_VALUE_COMPILER_MEM_ACC_3_OFFS                                                                (TCSR_TCSR_REGS_REG_BASE_OFFS + 0x0002201c)
#define HWIO_TCSR_DBGOVR_VALUE_COMPILER_MEM_ACC_3_RMSK                                                                      0xff
#define HWIO_TCSR_DBGOVR_VALUE_COMPILER_MEM_ACC_3_IN          \
        in_dword_masked(HWIO_TCSR_DBGOVR_VALUE_COMPILER_MEM_ACC_3_ADDR, HWIO_TCSR_DBGOVR_VALUE_COMPILER_MEM_ACC_3_RMSK)
#define HWIO_TCSR_DBGOVR_VALUE_COMPILER_MEM_ACC_3_INM(m)      \
        in_dword_masked(HWIO_TCSR_DBGOVR_VALUE_COMPILER_MEM_ACC_3_ADDR, m)
#define HWIO_TCSR_DBGOVR_VALUE_COMPILER_MEM_ACC_3_OUT(v)      \
        out_dword(HWIO_TCSR_DBGOVR_VALUE_COMPILER_MEM_ACC_3_ADDR,v)
#define HWIO_TCSR_DBGOVR_VALUE_COMPILER_MEM_ACC_3_OUTM(m,v) \
        out_dword_masked_ns(HWIO_TCSR_DBGOVR_VALUE_COMPILER_MEM_ACC_3_ADDR,m,v,HWIO_TCSR_DBGOVR_VALUE_COMPILER_MEM_ACC_3_IN)
#define HWIO_TCSR_DBGOVR_VALUE_COMPILER_MEM_ACC_3_DBGOVR_VALUE_COMPILER_MEM_ACC_3_BMSK                                      0xff
#define HWIO_TCSR_DBGOVR_VALUE_COMPILER_MEM_ACC_3_DBGOVR_VALUE_COMPILER_MEM_ACC_3_SHFT                                       0x0

#define HWIO_TCSR_DBGOVR_VALUE_COMPILER_MEM_ACC_4_ADDR                                                                (TCSR_TCSR_REGS_REG_BASE      + 0x00022020)
#define HWIO_TCSR_DBGOVR_VALUE_COMPILER_MEM_ACC_4_OFFS                                                                (TCSR_TCSR_REGS_REG_BASE_OFFS + 0x00022020)
#define HWIO_TCSR_DBGOVR_VALUE_COMPILER_MEM_ACC_4_RMSK                                                                      0xff
#define HWIO_TCSR_DBGOVR_VALUE_COMPILER_MEM_ACC_4_IN          \
        in_dword_masked(HWIO_TCSR_DBGOVR_VALUE_COMPILER_MEM_ACC_4_ADDR, HWIO_TCSR_DBGOVR_VALUE_COMPILER_MEM_ACC_4_RMSK)
#define HWIO_TCSR_DBGOVR_VALUE_COMPILER_MEM_ACC_4_INM(m)      \
        in_dword_masked(HWIO_TCSR_DBGOVR_VALUE_COMPILER_MEM_ACC_4_ADDR, m)
#define HWIO_TCSR_DBGOVR_VALUE_COMPILER_MEM_ACC_4_OUT(v)      \
        out_dword(HWIO_TCSR_DBGOVR_VALUE_COMPILER_MEM_ACC_4_ADDR,v)
#define HWIO_TCSR_DBGOVR_VALUE_COMPILER_MEM_ACC_4_OUTM(m,v) \
        out_dword_masked_ns(HWIO_TCSR_DBGOVR_VALUE_COMPILER_MEM_ACC_4_ADDR,m,v,HWIO_TCSR_DBGOVR_VALUE_COMPILER_MEM_ACC_4_IN)
#define HWIO_TCSR_DBGOVR_VALUE_COMPILER_MEM_ACC_4_DBGOVR_VALUE_COMPILER_MEM_ACC_4_BMSK                                      0xff
#define HWIO_TCSR_DBGOVR_VALUE_COMPILER_MEM_ACC_4_DBGOVR_VALUE_COMPILER_MEM_ACC_4_SHFT                                       0x0

#define HWIO_TCSR_DBGOVR_VALUE_COMPILER_MEM_ACC_5_ADDR                                                                (TCSR_TCSR_REGS_REG_BASE      + 0x00022024)
#define HWIO_TCSR_DBGOVR_VALUE_COMPILER_MEM_ACC_5_OFFS                                                                (TCSR_TCSR_REGS_REG_BASE_OFFS + 0x00022024)
#define HWIO_TCSR_DBGOVR_VALUE_COMPILER_MEM_ACC_5_RMSK                                                                      0xff
#define HWIO_TCSR_DBGOVR_VALUE_COMPILER_MEM_ACC_5_IN          \
        in_dword_masked(HWIO_TCSR_DBGOVR_VALUE_COMPILER_MEM_ACC_5_ADDR, HWIO_TCSR_DBGOVR_VALUE_COMPILER_MEM_ACC_5_RMSK)
#define HWIO_TCSR_DBGOVR_VALUE_COMPILER_MEM_ACC_5_INM(m)      \
        in_dword_masked(HWIO_TCSR_DBGOVR_VALUE_COMPILER_MEM_ACC_5_ADDR, m)
#define HWIO_TCSR_DBGOVR_VALUE_COMPILER_MEM_ACC_5_OUT(v)      \
        out_dword(HWIO_TCSR_DBGOVR_VALUE_COMPILER_MEM_ACC_5_ADDR,v)
#define HWIO_TCSR_DBGOVR_VALUE_COMPILER_MEM_ACC_5_OUTM(m,v) \
        out_dword_masked_ns(HWIO_TCSR_DBGOVR_VALUE_COMPILER_MEM_ACC_5_ADDR,m,v,HWIO_TCSR_DBGOVR_VALUE_COMPILER_MEM_ACC_5_IN)
#define HWIO_TCSR_DBGOVR_VALUE_COMPILER_MEM_ACC_5_DBGOVR_VALUE_COMPILER_MEM_ACC_5_BMSK                                      0xff
#define HWIO_TCSR_DBGOVR_VALUE_COMPILER_MEM_ACC_5_DBGOVR_VALUE_COMPILER_MEM_ACC_5_SHFT                                       0x0

#define HWIO_TCSR_DBGOVR_VALUE_COMPILER_MEM_ACC_6_ADDR                                                                (TCSR_TCSR_REGS_REG_BASE      + 0x00022028)
#define HWIO_TCSR_DBGOVR_VALUE_COMPILER_MEM_ACC_6_OFFS                                                                (TCSR_TCSR_REGS_REG_BASE_OFFS + 0x00022028)
#define HWIO_TCSR_DBGOVR_VALUE_COMPILER_MEM_ACC_6_RMSK                                                                      0xff
#define HWIO_TCSR_DBGOVR_VALUE_COMPILER_MEM_ACC_6_IN          \
        in_dword_masked(HWIO_TCSR_DBGOVR_VALUE_COMPILER_MEM_ACC_6_ADDR, HWIO_TCSR_DBGOVR_VALUE_COMPILER_MEM_ACC_6_RMSK)
#define HWIO_TCSR_DBGOVR_VALUE_COMPILER_MEM_ACC_6_INM(m)      \
        in_dword_masked(HWIO_TCSR_DBGOVR_VALUE_COMPILER_MEM_ACC_6_ADDR, m)
#define HWIO_TCSR_DBGOVR_VALUE_COMPILER_MEM_ACC_6_OUT(v)      \
        out_dword(HWIO_TCSR_DBGOVR_VALUE_COMPILER_MEM_ACC_6_ADDR,v)
#define HWIO_TCSR_DBGOVR_VALUE_COMPILER_MEM_ACC_6_OUTM(m,v) \
        out_dword_masked_ns(HWIO_TCSR_DBGOVR_VALUE_COMPILER_MEM_ACC_6_ADDR,m,v,HWIO_TCSR_DBGOVR_VALUE_COMPILER_MEM_ACC_6_IN)
#define HWIO_TCSR_DBGOVR_VALUE_COMPILER_MEM_ACC_6_DBGOVR_VALUE_COMPILER_MEM_ACC_6_BMSK                                      0xff
#define HWIO_TCSR_DBGOVR_VALUE_COMPILER_MEM_ACC_6_DBGOVR_VALUE_COMPILER_MEM_ACC_6_SHFT                                       0x0

#define HWIO_TCSR_DBGOVR_VALUE_COMPILER_MEM_ACC_7_ADDR                                                                (TCSR_TCSR_REGS_REG_BASE      + 0x0002202c)
#define HWIO_TCSR_DBGOVR_VALUE_COMPILER_MEM_ACC_7_OFFS                                                                (TCSR_TCSR_REGS_REG_BASE_OFFS + 0x0002202c)
#define HWIO_TCSR_DBGOVR_VALUE_COMPILER_MEM_ACC_7_RMSK                                                                      0xff
#define HWIO_TCSR_DBGOVR_VALUE_COMPILER_MEM_ACC_7_IN          \
        in_dword_masked(HWIO_TCSR_DBGOVR_VALUE_COMPILER_MEM_ACC_7_ADDR, HWIO_TCSR_DBGOVR_VALUE_COMPILER_MEM_ACC_7_RMSK)
#define HWIO_TCSR_DBGOVR_VALUE_COMPILER_MEM_ACC_7_INM(m)      \
        in_dword_masked(HWIO_TCSR_DBGOVR_VALUE_COMPILER_MEM_ACC_7_ADDR, m)
#define HWIO_TCSR_DBGOVR_VALUE_COMPILER_MEM_ACC_7_OUT(v)      \
        out_dword(HWIO_TCSR_DBGOVR_VALUE_COMPILER_MEM_ACC_7_ADDR,v)
#define HWIO_TCSR_DBGOVR_VALUE_COMPILER_MEM_ACC_7_OUTM(m,v) \
        out_dword_masked_ns(HWIO_TCSR_DBGOVR_VALUE_COMPILER_MEM_ACC_7_ADDR,m,v,HWIO_TCSR_DBGOVR_VALUE_COMPILER_MEM_ACC_7_IN)
#define HWIO_TCSR_DBGOVR_VALUE_COMPILER_MEM_ACC_7_DBGOVR_VALUE_COMPILER_MEM_ACC_7_BMSK                                      0xff
#define HWIO_TCSR_DBGOVR_VALUE_COMPILER_MEM_ACC_7_DBGOVR_VALUE_COMPILER_MEM_ACC_7_SHFT                                       0x0

#define HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE1_ADDR                                                              (TCSR_TCSR_REGS_REG_BASE      + 0x00022030)
#define HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE1_OFFS                                                              (TCSR_TCSR_REGS_REG_BASE_OFFS + 0x00022030)
#define HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE1_RMSK                                                                    0xff
#define HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE1_IN          \
        in_dword_masked(HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE1_ADDR, HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE1_RMSK)
#define HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE1_INM(m)      \
        in_dword_masked(HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE1_ADDR, m)
#define HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE1_OUT(v)      \
        out_dword(HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE1_ADDR,v)
#define HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE1_OUTM(m,v) \
        out_dword_masked_ns(HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE1_ADDR,m,v,HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE1_IN)
#define HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE1_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE1_BMSK                                  0xff
#define HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE1_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE1_SHFT                                   0x0

#define HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE2_ADDR                                                              (TCSR_TCSR_REGS_REG_BASE      + 0x00022034)
#define HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE2_OFFS                                                              (TCSR_TCSR_REGS_REG_BASE_OFFS + 0x00022034)
#define HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE2_RMSK                                                                    0xff
#define HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE2_IN          \
        in_dword_masked(HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE2_ADDR, HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE2_RMSK)
#define HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE2_INM(m)      \
        in_dword_masked(HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE2_ADDR, m)
#define HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE2_OUT(v)      \
        out_dword(HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE2_ADDR,v)
#define HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE2_OUTM(m,v) \
        out_dword_masked_ns(HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE2_ADDR,m,v,HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE2_IN)
#define HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE2_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE2_BMSK                                  0xff
#define HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE2_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE2_SHFT                                   0x0

#define HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE3_ADDR                                                              (TCSR_TCSR_REGS_REG_BASE      + 0x00022038)
#define HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE3_OFFS                                                              (TCSR_TCSR_REGS_REG_BASE_OFFS + 0x00022038)
#define HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE3_RMSK                                                                    0xff
#define HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE3_IN          \
        in_dword_masked(HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE3_ADDR, HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE3_RMSK)
#define HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE3_INM(m)      \
        in_dword_masked(HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE3_ADDR, m)
#define HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE3_OUT(v)      \
        out_dword(HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE3_ADDR,v)
#define HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE3_OUTM(m,v) \
        out_dword_masked_ns(HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE3_ADDR,m,v,HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE3_IN)
#define HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE3_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE3_BMSK                                  0xff
#define HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE3_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE3_SHFT                                   0x0

#define HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE4_ADDR                                                              (TCSR_TCSR_REGS_REG_BASE      + 0x0002203c)
#define HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE4_OFFS                                                              (TCSR_TCSR_REGS_REG_BASE_OFFS + 0x0002203c)
#define HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE4_RMSK                                                                    0xff
#define HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE4_IN          \
        in_dword_masked(HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE4_ADDR, HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE4_RMSK)
#define HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE4_INM(m)      \
        in_dword_masked(HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE4_ADDR, m)
#define HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE4_OUT(v)      \
        out_dword(HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE4_ADDR,v)
#define HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE4_OUTM(m,v) \
        out_dword_masked_ns(HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE4_ADDR,m,v,HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE4_IN)
#define HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE4_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE4_BMSK                                  0xff
#define HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE4_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE4_SHFT                                   0x0

#define HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE5_ADDR                                                              (TCSR_TCSR_REGS_REG_BASE      + 0x00022040)
#define HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE5_OFFS                                                              (TCSR_TCSR_REGS_REG_BASE_OFFS + 0x00022040)
#define HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE5_RMSK                                                                    0xff
#define HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE5_IN          \
        in_dword_masked(HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE5_ADDR, HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE5_RMSK)
#define HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE5_INM(m)      \
        in_dword_masked(HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE5_ADDR, m)
#define HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE5_OUT(v)      \
        out_dword(HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE5_ADDR,v)
#define HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE5_OUTM(m,v) \
        out_dword_masked_ns(HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE5_ADDR,m,v,HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE5_IN)
#define HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE5_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE5_BMSK                                  0xff
#define HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE5_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE5_SHFT                                   0x0

#define HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE6_ADDR                                                              (TCSR_TCSR_REGS_REG_BASE      + 0x00022044)
#define HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE6_OFFS                                                              (TCSR_TCSR_REGS_REG_BASE_OFFS + 0x00022044)
#define HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE6_RMSK                                                                    0xff
#define HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE6_IN          \
        in_dword_masked(HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE6_ADDR, HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE6_RMSK)
#define HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE6_INM(m)      \
        in_dword_masked(HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE6_ADDR, m)
#define HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE6_OUT(v)      \
        out_dword(HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE6_ADDR,v)
#define HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE6_OUTM(m,v) \
        out_dword_masked_ns(HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE6_ADDR,m,v,HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE6_IN)
#define HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE6_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE6_BMSK                                  0xff
#define HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE6_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE6_SHFT                                   0x0

#define HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE7_ADDR                                                              (TCSR_TCSR_REGS_REG_BASE      + 0x00022048)
#define HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE7_OFFS                                                              (TCSR_TCSR_REGS_REG_BASE_OFFS + 0x00022048)
#define HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE7_RMSK                                                                    0xff
#define HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE7_IN          \
        in_dword_masked(HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE7_ADDR, HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE7_RMSK)
#define HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE7_INM(m)      \
        in_dword_masked(HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE7_ADDR, m)
#define HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE7_OUT(v)      \
        out_dword(HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE7_ADDR,v)
#define HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE7_OUTM(m,v) \
        out_dword_masked_ns(HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE7_ADDR,m,v,HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE7_IN)
#define HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE7_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE7_BMSK                                  0xff
#define HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE7_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE7_SHFT                                   0x0

#define HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE8_ADDR                                                              (TCSR_TCSR_REGS_REG_BASE      + 0x0002204c)
#define HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE8_OFFS                                                              (TCSR_TCSR_REGS_REG_BASE_OFFS + 0x0002204c)
#define HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE8_RMSK                                                                    0xff
#define HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE8_IN          \
        in_dword_masked(HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE8_ADDR, HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE8_RMSK)
#define HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE8_INM(m)      \
        in_dword_masked(HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE8_ADDR, m)
#define HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE8_OUT(v)      \
        out_dword(HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE8_ADDR,v)
#define HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE8_OUTM(m,v) \
        out_dword_masked_ns(HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE8_ADDR,m,v,HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE8_IN)
#define HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE8_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE8_BMSK                                  0xff
#define HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE8_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE8_SHFT                                   0x0

#define HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE9_ADDR                                                              (TCSR_TCSR_REGS_REG_BASE      + 0x00022050)
#define HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE9_OFFS                                                              (TCSR_TCSR_REGS_REG_BASE_OFFS + 0x00022050)
#define HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE9_RMSK                                                                    0xff
#define HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE9_IN          \
        in_dword_masked(HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE9_ADDR, HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE9_RMSK)
#define HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE9_INM(m)      \
        in_dword_masked(HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE9_ADDR, m)
#define HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE9_OUT(v)      \
        out_dword(HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE9_ADDR,v)
#define HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE9_OUTM(m,v) \
        out_dword_masked_ns(HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE9_ADDR,m,v,HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE9_IN)
#define HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE9_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE9_BMSK                                  0xff
#define HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE9_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE9_SHFT                                   0x0

#define HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE10_ADDR                                                             (TCSR_TCSR_REGS_REG_BASE      + 0x00022054)
#define HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE10_OFFS                                                             (TCSR_TCSR_REGS_REG_BASE_OFFS + 0x00022054)
#define HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE10_RMSK                                                                   0xff
#define HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE10_IN          \
        in_dword_masked(HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE10_ADDR, HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE10_RMSK)
#define HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE10_INM(m)      \
        in_dword_masked(HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE10_ADDR, m)
#define HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE10_OUT(v)      \
        out_dword(HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE10_ADDR,v)
#define HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE10_OUTM(m,v) \
        out_dword_masked_ns(HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE10_ADDR,m,v,HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE10_IN)
#define HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE10_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE10_BMSK                                0xff
#define HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE10_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE10_SHFT                                 0x0

#define HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE11_ADDR                                                             (TCSR_TCSR_REGS_REG_BASE      + 0x00022058)
#define HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE11_OFFS                                                             (TCSR_TCSR_REGS_REG_BASE_OFFS + 0x00022058)
#define HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE11_RMSK                                                                   0xff
#define HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE11_IN          \
        in_dword_masked(HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE11_ADDR, HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE11_RMSK)
#define HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE11_INM(m)      \
        in_dword_masked(HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE11_ADDR, m)
#define HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE11_OUT(v)      \
        out_dword(HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE11_ADDR,v)
#define HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE11_OUTM(m,v) \
        out_dword_masked_ns(HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE11_ADDR,m,v,HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE11_IN)
#define HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE11_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE11_BMSK                                0xff
#define HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE11_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE11_SHFT                                 0x0

#define HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE12_ADDR                                                             (TCSR_TCSR_REGS_REG_BASE      + 0x0002205c)
#define HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE12_OFFS                                                             (TCSR_TCSR_REGS_REG_BASE_OFFS + 0x0002205c)
#define HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE12_RMSK                                                                   0xff
#define HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE12_IN          \
        in_dword_masked(HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE12_ADDR, HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE12_RMSK)
#define HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE12_INM(m)      \
        in_dword_masked(HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE12_ADDR, m)
#define HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE12_OUT(v)      \
        out_dword(HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE12_ADDR,v)
#define HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE12_OUTM(m,v) \
        out_dword_masked_ns(HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE12_ADDR,m,v,HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE12_IN)
#define HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE12_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE12_BMSK                                0xff
#define HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE12_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE12_SHFT                                 0x0

#define HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE13_ADDR                                                             (TCSR_TCSR_REGS_REG_BASE      + 0x00022060)
#define HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE13_OFFS                                                             (TCSR_TCSR_REGS_REG_BASE_OFFS + 0x00022060)
#define HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE13_RMSK                                                                   0xff
#define HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE13_IN          \
        in_dword_masked(HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE13_ADDR, HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE13_RMSK)
#define HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE13_INM(m)      \
        in_dword_masked(HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE13_ADDR, m)
#define HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE13_OUT(v)      \
        out_dword(HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE13_ADDR,v)
#define HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE13_OUTM(m,v) \
        out_dword_masked_ns(HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE13_ADDR,m,v,HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE13_IN)
#define HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE13_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE13_BMSK                                0xff
#define HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE13_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE13_SHFT                                 0x0

#define HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE14_ADDR                                                             (TCSR_TCSR_REGS_REG_BASE      + 0x00022064)
#define HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE14_OFFS                                                             (TCSR_TCSR_REGS_REG_BASE_OFFS + 0x00022064)
#define HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE14_RMSK                                                                   0xff
#define HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE14_IN          \
        in_dword_masked(HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE14_ADDR, HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE14_RMSK)
#define HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE14_INM(m)      \
        in_dword_masked(HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE14_ADDR, m)
#define HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE14_OUT(v)      \
        out_dword(HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE14_ADDR,v)
#define HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE14_OUTM(m,v) \
        out_dword_masked_ns(HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE14_ADDR,m,v,HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE14_IN)
#define HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE14_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE14_BMSK                                0xff
#define HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE14_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE14_SHFT                                 0x0

#define HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE15_ADDR                                                             (TCSR_TCSR_REGS_REG_BASE      + 0x00022068)
#define HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE15_OFFS                                                             (TCSR_TCSR_REGS_REG_BASE_OFFS + 0x00022068)
#define HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE15_RMSK                                                                   0xff
#define HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE15_IN          \
        in_dword_masked(HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE15_ADDR, HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE15_RMSK)
#define HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE15_INM(m)      \
        in_dword_masked(HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE15_ADDR, m)
#define HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE15_OUT(v)      \
        out_dword(HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE15_ADDR,v)
#define HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE15_OUTM(m,v) \
        out_dword_masked_ns(HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE15_ADDR,m,v,HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE15_IN)
#define HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE15_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE15_BMSK                                0xff
#define HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE15_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE15_SHFT                                 0x0

#define HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE16_ADDR                                                             (TCSR_TCSR_REGS_REG_BASE      + 0x0002206c)
#define HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE16_OFFS                                                             (TCSR_TCSR_REGS_REG_BASE_OFFS + 0x0002206c)
#define HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE16_RMSK                                                                   0xff
#define HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE16_IN          \
        in_dword_masked(HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE16_ADDR, HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE16_RMSK)
#define HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE16_INM(m)      \
        in_dword_masked(HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE16_ADDR, m)
#define HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE16_OUT(v)      \
        out_dword(HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE16_ADDR,v)
#define HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE16_OUTM(m,v) \
        out_dword_masked_ns(HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE16_ADDR,m,v,HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE16_IN)
#define HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE16_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE16_BMSK                                0xff
#define HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE16_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE16_SHFT                                 0x0

#define HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE17_ADDR                                                             (TCSR_TCSR_REGS_REG_BASE      + 0x00022070)
#define HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE17_OFFS                                                             (TCSR_TCSR_REGS_REG_BASE_OFFS + 0x00022070)
#define HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE17_RMSK                                                                   0xff
#define HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE17_IN          \
        in_dword_masked(HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE17_ADDR, HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE17_RMSK)
#define HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE17_INM(m)      \
        in_dword_masked(HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE17_ADDR, m)
#define HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE17_OUT(v)      \
        out_dword(HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE17_ADDR,v)
#define HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE17_OUTM(m,v) \
        out_dword_masked_ns(HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE17_ADDR,m,v,HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE17_IN)
#define HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE17_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE17_BMSK                                0xff
#define HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE17_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE17_SHFT                                 0x0

#define HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE18_ADDR                                                             (TCSR_TCSR_REGS_REG_BASE      + 0x00022074)
#define HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE18_OFFS                                                             (TCSR_TCSR_REGS_REG_BASE_OFFS + 0x00022074)
#define HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE18_RMSK                                                                   0xff
#define HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE18_IN          \
        in_dword_masked(HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE18_ADDR, HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE18_RMSK)
#define HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE18_INM(m)      \
        in_dword_masked(HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE18_ADDR, m)
#define HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE18_OUT(v)      \
        out_dword(HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE18_ADDR,v)
#define HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE18_OUTM(m,v) \
        out_dword_masked_ns(HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE18_ADDR,m,v,HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE18_IN)
#define HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE18_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE18_BMSK                                0xff
#define HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE18_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE18_SHFT                                 0x0

#define HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE19_ADDR                                                             (TCSR_TCSR_REGS_REG_BASE      + 0x00022078)
#define HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE19_OFFS                                                             (TCSR_TCSR_REGS_REG_BASE_OFFS + 0x00022078)
#define HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE19_RMSK                                                                   0xff
#define HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE19_IN          \
        in_dword_masked(HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE19_ADDR, HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE19_RMSK)
#define HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE19_INM(m)      \
        in_dword_masked(HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE19_ADDR, m)
#define HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE19_OUT(v)      \
        out_dword(HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE19_ADDR,v)
#define HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE19_OUTM(m,v) \
        out_dword_masked_ns(HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE19_ADDR,m,v,HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE19_IN)
#define HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE19_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE19_BMSK                                0xff
#define HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE19_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE19_SHFT                                 0x0

#define HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE20_ADDR                                                             (TCSR_TCSR_REGS_REG_BASE      + 0x0002207c)
#define HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE20_OFFS                                                             (TCSR_TCSR_REGS_REG_BASE_OFFS + 0x0002207c)
#define HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE20_RMSK                                                                   0xff
#define HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE20_IN          \
        in_dword_masked(HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE20_ADDR, HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE20_RMSK)
#define HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE20_INM(m)      \
        in_dword_masked(HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE20_ADDR, m)
#define HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE20_OUT(v)      \
        out_dword(HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE20_ADDR,v)
#define HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE20_OUTM(m,v) \
        out_dword_masked_ns(HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE20_ADDR,m,v,HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE20_IN)
#define HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE20_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE20_BMSK                                0xff
#define HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE20_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE20_SHFT                                 0x0

#define HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE21_ADDR                                                             (TCSR_TCSR_REGS_REG_BASE      + 0x00022080)
#define HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE21_OFFS                                                             (TCSR_TCSR_REGS_REG_BASE_OFFS + 0x00022080)
#define HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE21_RMSK                                                                   0xff
#define HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE21_IN          \
        in_dword_masked(HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE21_ADDR, HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE21_RMSK)
#define HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE21_INM(m)      \
        in_dword_masked(HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE21_ADDR, m)
#define HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE21_OUT(v)      \
        out_dword(HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE21_ADDR,v)
#define HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE21_OUTM(m,v) \
        out_dword_masked_ns(HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE21_ADDR,m,v,HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE21_IN)
#define HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE21_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE21_BMSK                                0xff
#define HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE21_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE21_SHFT                                 0x0

#define HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE22_ADDR                                                             (TCSR_TCSR_REGS_REG_BASE      + 0x00022084)
#define HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE22_OFFS                                                             (TCSR_TCSR_REGS_REG_BASE_OFFS + 0x00022084)
#define HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE22_RMSK                                                                   0xff
#define HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE22_IN          \
        in_dword_masked(HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE22_ADDR, HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE22_RMSK)
#define HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE22_INM(m)      \
        in_dword_masked(HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE22_ADDR, m)
#define HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE22_OUT(v)      \
        out_dword(HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE22_ADDR,v)
#define HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE22_OUTM(m,v) \
        out_dword_masked_ns(HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE22_ADDR,m,v,HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE22_IN)
#define HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE22_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE22_BMSK                                0xff
#define HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE22_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE22_SHFT                                 0x0

#define HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE23_ADDR                                                             (TCSR_TCSR_REGS_REG_BASE      + 0x00022088)
#define HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE23_OFFS                                                             (TCSR_TCSR_REGS_REG_BASE_OFFS + 0x00022088)
#define HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE23_RMSK                                                                   0xff
#define HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE23_IN          \
        in_dword_masked(HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE23_ADDR, HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE23_RMSK)
#define HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE23_INM(m)      \
        in_dword_masked(HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE23_ADDR, m)
#define HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE23_OUT(v)      \
        out_dword(HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE23_ADDR,v)
#define HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE23_OUTM(m,v) \
        out_dword_masked_ns(HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE23_ADDR,m,v,HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE23_IN)
#define HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE23_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE23_BMSK                                0xff
#define HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE23_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE23_SHFT                                 0x0

#define HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE24_ADDR                                                             (TCSR_TCSR_REGS_REG_BASE      + 0x0002208c)
#define HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE24_OFFS                                                             (TCSR_TCSR_REGS_REG_BASE_OFFS + 0x0002208c)
#define HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE24_RMSK                                                                   0xff
#define HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE24_IN          \
        in_dword_masked(HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE24_ADDR, HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE24_RMSK)
#define HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE24_INM(m)      \
        in_dword_masked(HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE24_ADDR, m)
#define HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE24_OUT(v)      \
        out_dword(HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE24_ADDR,v)
#define HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE24_OUTM(m,v) \
        out_dword_masked_ns(HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE24_ADDR,m,v,HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE24_IN)
#define HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE24_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE24_BMSK                                0xff
#define HWIO_TCSR_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE24_DBGOVR_VALUE_CUSTOM_MEM_ACC_TYPE24_SHFT                                 0x0

#define HWIO_TCSR_A1NOC_BYPASS_CHECKTYPE_ADDR                                                                         (TCSR_TCSR_REGS_REG_BASE      + 0x00023000)
#define HWIO_TCSR_A1NOC_BYPASS_CHECKTYPE_OFFS                                                                         (TCSR_TCSR_REGS_REG_BASE_OFFS + 0x00023000)
#define HWIO_TCSR_A1NOC_BYPASS_CHECKTYPE_RMSK                                                                                0x3
#define HWIO_TCSR_A1NOC_BYPASS_CHECKTYPE_IN          \
        in_dword_masked(HWIO_TCSR_A1NOC_BYPASS_CHECKTYPE_ADDR, HWIO_TCSR_A1NOC_BYPASS_CHECKTYPE_RMSK)
#define HWIO_TCSR_A1NOC_BYPASS_CHECKTYPE_INM(m)      \
        in_dword_masked(HWIO_TCSR_A1NOC_BYPASS_CHECKTYPE_ADDR, m)
#define HWIO_TCSR_A1NOC_BYPASS_CHECKTYPE_OUT(v)      \
        out_dword(HWIO_TCSR_A1NOC_BYPASS_CHECKTYPE_ADDR,v)
#define HWIO_TCSR_A1NOC_BYPASS_CHECKTYPE_OUTM(m,v) \
        out_dword_masked_ns(HWIO_TCSR_A1NOC_BYPASS_CHECKTYPE_ADDR,m,v,HWIO_TCSR_A1NOC_BYPASS_CHECKTYPE_IN)
#define HWIO_TCSR_A1NOC_BYPASS_CHECKTYPE_A1NOC_SP_BYPASS_CHECK_TYPE_BMSK                                                     0x2
#define HWIO_TCSR_A1NOC_BYPASS_CHECKTYPE_A1NOC_SP_BYPASS_CHECK_TYPE_SHFT                                                     0x1
#define HWIO_TCSR_A1NOC_BYPASS_CHECKTYPE_A1NOC_MSA_BYPASS_CHECK_TYPE_BMSK                                                    0x1
#define HWIO_TCSR_A1NOC_BYPASS_CHECKTYPE_A1NOC_MSA_BYPASS_CHECK_TYPE_SHFT                                                    0x0

#define HWIO_TCSR_A2NOC_BYPASS_CHECKTYPE_ADDR                                                                         (TCSR_TCSR_REGS_REG_BASE      + 0x00023004)
#define HWIO_TCSR_A2NOC_BYPASS_CHECKTYPE_OFFS                                                                         (TCSR_TCSR_REGS_REG_BASE_OFFS + 0x00023004)
#define HWIO_TCSR_A2NOC_BYPASS_CHECKTYPE_RMSK                                                                                0x3
#define HWIO_TCSR_A2NOC_BYPASS_CHECKTYPE_IN          \
        in_dword_masked(HWIO_TCSR_A2NOC_BYPASS_CHECKTYPE_ADDR, HWIO_TCSR_A2NOC_BYPASS_CHECKTYPE_RMSK)
#define HWIO_TCSR_A2NOC_BYPASS_CHECKTYPE_INM(m)      \
        in_dword_masked(HWIO_TCSR_A2NOC_BYPASS_CHECKTYPE_ADDR, m)
#define HWIO_TCSR_A2NOC_BYPASS_CHECKTYPE_OUT(v)      \
        out_dword(HWIO_TCSR_A2NOC_BYPASS_CHECKTYPE_ADDR,v)
#define HWIO_TCSR_A2NOC_BYPASS_CHECKTYPE_OUTM(m,v) \
        out_dword_masked_ns(HWIO_TCSR_A2NOC_BYPASS_CHECKTYPE_ADDR,m,v,HWIO_TCSR_A2NOC_BYPASS_CHECKTYPE_IN)
#define HWIO_TCSR_A2NOC_BYPASS_CHECKTYPE_A2NOC_SP_BYPASS_CHECK_TYPE_BMSK                                                     0x2
#define HWIO_TCSR_A2NOC_BYPASS_CHECKTYPE_A2NOC_SP_BYPASS_CHECK_TYPE_SHFT                                                     0x1
#define HWIO_TCSR_A2NOC_BYPASS_CHECKTYPE_A2NOC_MSA_BYPASS_CHECK_TYPE_BMSK                                                    0x1
#define HWIO_TCSR_A2NOC_BYPASS_CHECKTYPE_A2NOC_MSA_BYPASS_CHECK_TYPE_SHFT                                                    0x0

#define HWIO_TCSR_WCSS_AHBBM_CTRL_ADDR                                                                                (TCSR_TCSR_REGS_REG_BASE      + 0x00024004)
#define HWIO_TCSR_WCSS_AHBBM_CTRL_OFFS                                                                                (TCSR_TCSR_REGS_REG_BASE_OFFS + 0x00024004)
#define HWIO_TCSR_WCSS_AHBBM_CTRL_RMSK                                                                                       0x7
#define HWIO_TCSR_WCSS_AHBBM_CTRL_IN          \
        in_dword_masked(HWIO_TCSR_WCSS_AHBBM_CTRL_ADDR, HWIO_TCSR_WCSS_AHBBM_CTRL_RMSK)
#define HWIO_TCSR_WCSS_AHBBM_CTRL_INM(m)      \
        in_dword_masked(HWIO_TCSR_WCSS_AHBBM_CTRL_ADDR, m)
#define HWIO_TCSR_WCSS_AHBBM_CTRL_OUT(v)      \
        out_dword(HWIO_TCSR_WCSS_AHBBM_CTRL_ADDR,v)
#define HWIO_TCSR_WCSS_AHBBM_CTRL_OUTM(m,v) \
        out_dword_masked_ns(HWIO_TCSR_WCSS_AHBBM_CTRL_ADDR,m,v,HWIO_TCSR_WCSS_AHBBM_CTRL_IN)
#define HWIO_TCSR_WCSS_AHBBM_CTRL_AHBBM_SOFT_RESET_BMSK                                                                      0x4
#define HWIO_TCSR_WCSS_AHBBM_CTRL_AHBBM_SOFT_RESET_SHFT                                                                      0x2
#define HWIO_TCSR_WCSS_AHBBM_CTRL_AHBBM_CLKGATE_DISABLE_BMSK                                                                 0x2
#define HWIO_TCSR_WCSS_AHBBM_CTRL_AHBBM_CLKGATE_DISABLE_SHFT                                                                 0x1
#define HWIO_TCSR_WCSS_AHBBM_CTRL_AHBBM_ROOT_CLK_ENABLE_BMSK                                                                 0x1
#define HWIO_TCSR_WCSS_AHBBM_CTRL_AHBBM_ROOT_CLK_ENABLE_SHFT                                                                 0x0

#define HWIO_TCSR_A7SS_MUX_SEL_ADDR                                                                                   (TCSR_TCSR_REGS_REG_BASE      + 0x00025008)
#define HWIO_TCSR_A7SS_MUX_SEL_OFFS                                                                                   (TCSR_TCSR_REGS_REG_BASE_OFFS + 0x00025008)
#define HWIO_TCSR_A7SS_MUX_SEL_RMSK                                                                                          0x3
#define HWIO_TCSR_A7SS_MUX_SEL_IN          \
        in_dword_masked(HWIO_TCSR_A7SS_MUX_SEL_ADDR, HWIO_TCSR_A7SS_MUX_SEL_RMSK)
#define HWIO_TCSR_A7SS_MUX_SEL_INM(m)      \
        in_dword_masked(HWIO_TCSR_A7SS_MUX_SEL_ADDR, m)
#define HWIO_TCSR_A7SS_MUX_SEL_OUT(v)      \
        out_dword(HWIO_TCSR_A7SS_MUX_SEL_ADDR,v)
#define HWIO_TCSR_A7SS_MUX_SEL_OUTM(m,v) \
        out_dword_masked_ns(HWIO_TCSR_A7SS_MUX_SEL_ADDR,m,v,HWIO_TCSR_A7SS_MUX_SEL_IN)
#define HWIO_TCSR_A7SS_MUX_SEL_A7SS_MUX_SEL_BMSK                                                                             0x3
#define HWIO_TCSR_A7SS_MUX_SEL_A7SS_MUX_SEL_SHFT                                                                             0x0

#define HWIO_TCSR_A7SS_SPARE_REG0_ADDR                                                                                (TCSR_TCSR_REGS_REG_BASE      + 0x00025014)
#define HWIO_TCSR_A7SS_SPARE_REG0_OFFS                                                                                (TCSR_TCSR_REGS_REG_BASE_OFFS + 0x00025014)
#define HWIO_TCSR_A7SS_SPARE_REG0_RMSK                                                                                0xffffffff
#define HWIO_TCSR_A7SS_SPARE_REG0_IN          \
        in_dword_masked(HWIO_TCSR_A7SS_SPARE_REG0_ADDR, HWIO_TCSR_A7SS_SPARE_REG0_RMSK)
#define HWIO_TCSR_A7SS_SPARE_REG0_INM(m)      \
        in_dword_masked(HWIO_TCSR_A7SS_SPARE_REG0_ADDR, m)
#define HWIO_TCSR_A7SS_SPARE_REG0_OUT(v)      \
        out_dword(HWIO_TCSR_A7SS_SPARE_REG0_ADDR,v)
#define HWIO_TCSR_A7SS_SPARE_REG0_OUTM(m,v) \
        out_dword_masked_ns(HWIO_TCSR_A7SS_SPARE_REG0_ADDR,m,v,HWIO_TCSR_A7SS_SPARE_REG0_IN)
#define HWIO_TCSR_A7SS_SPARE_REG0_SPARE_REG0_BMSK                                                                     0xffffffff
#define HWIO_TCSR_A7SS_SPARE_REG0_SPARE_REG0_SHFT                                                                            0x0

#define HWIO_TCSR_A7SS_SPARE_REG1_ADDR                                                                                (TCSR_TCSR_REGS_REG_BASE      + 0x00025018)
#define HWIO_TCSR_A7SS_SPARE_REG1_OFFS                                                                                (TCSR_TCSR_REGS_REG_BASE_OFFS + 0x00025018)
#define HWIO_TCSR_A7SS_SPARE_REG1_RMSK                                                                                0xffffffff
#define HWIO_TCSR_A7SS_SPARE_REG1_IN          \
        in_dword_masked(HWIO_TCSR_A7SS_SPARE_REG1_ADDR, HWIO_TCSR_A7SS_SPARE_REG1_RMSK)
#define HWIO_TCSR_A7SS_SPARE_REG1_INM(m)      \
        in_dword_masked(HWIO_TCSR_A7SS_SPARE_REG1_ADDR, m)
#define HWIO_TCSR_A7SS_SPARE_REG1_OUT(v)      \
        out_dword(HWIO_TCSR_A7SS_SPARE_REG1_ADDR,v)
#define HWIO_TCSR_A7SS_SPARE_REG1_OUTM(m,v) \
        out_dword_masked_ns(HWIO_TCSR_A7SS_SPARE_REG1_ADDR,m,v,HWIO_TCSR_A7SS_SPARE_REG1_IN)
#define HWIO_TCSR_A7SS_SPARE_REG1_SPARE_REG1_BMSK                                                                     0xffffffff
#define HWIO_TCSR_A7SS_SPARE_REG1_SPARE_REG1_SHFT                                                                            0x0

#define HWIO_TCSR_A7SS_SPARE_REG2_ADDR                                                                                (TCSR_TCSR_REGS_REG_BASE      + 0x0002501c)
#define HWIO_TCSR_A7SS_SPARE_REG2_OFFS                                                                                (TCSR_TCSR_REGS_REG_BASE_OFFS + 0x0002501c)
#define HWIO_TCSR_A7SS_SPARE_REG2_RMSK                                                                                0xffffffff
#define HWIO_TCSR_A7SS_SPARE_REG2_IN          \
        in_dword_masked(HWIO_TCSR_A7SS_SPARE_REG2_ADDR, HWIO_TCSR_A7SS_SPARE_REG2_RMSK)
#define HWIO_TCSR_A7SS_SPARE_REG2_INM(m)      \
        in_dword_masked(HWIO_TCSR_A7SS_SPARE_REG2_ADDR, m)
#define HWIO_TCSR_A7SS_SPARE_REG2_OUT(v)      \
        out_dword(HWIO_TCSR_A7SS_SPARE_REG2_ADDR,v)
#define HWIO_TCSR_A7SS_SPARE_REG2_OUTM(m,v) \
        out_dword_masked_ns(HWIO_TCSR_A7SS_SPARE_REG2_ADDR,m,v,HWIO_TCSR_A7SS_SPARE_REG2_IN)
#define HWIO_TCSR_A7SS_SPARE_REG2_SPARE_REG2_BMSK                                                                     0xffffffff
#define HWIO_TCSR_A7SS_SPARE_REG2_SPARE_REG2_SHFT                                                                            0x0

#define HWIO_TCSR_A7SS_SPARE_REG3_ADDR                                                                                (TCSR_TCSR_REGS_REG_BASE      + 0x00025020)
#define HWIO_TCSR_A7SS_SPARE_REG3_OFFS                                                                                (TCSR_TCSR_REGS_REG_BASE_OFFS + 0x00025020)
#define HWIO_TCSR_A7SS_SPARE_REG3_RMSK                                                                                0xffffffff
#define HWIO_TCSR_A7SS_SPARE_REG3_IN          \
        in_dword_masked(HWIO_TCSR_A7SS_SPARE_REG3_ADDR, HWIO_TCSR_A7SS_SPARE_REG3_RMSK)
#define HWIO_TCSR_A7SS_SPARE_REG3_INM(m)      \
        in_dword_masked(HWIO_TCSR_A7SS_SPARE_REG3_ADDR, m)
#define HWIO_TCSR_A7SS_SPARE_REG3_OUT(v)      \
        out_dword(HWIO_TCSR_A7SS_SPARE_REG3_ADDR,v)
#define HWIO_TCSR_A7SS_SPARE_REG3_OUTM(m,v) \
        out_dword_masked_ns(HWIO_TCSR_A7SS_SPARE_REG3_ADDR,m,v,HWIO_TCSR_A7SS_SPARE_REG3_IN)
#define HWIO_TCSR_A7SS_SPARE_REG3_SPARE_REG3_BMSK                                                                     0xffffffff
#define HWIO_TCSR_A7SS_SPARE_REG3_SPARE_REG3_SHFT                                                                            0x0

#define HWIO_TCSR_A7SS_SPARE_REG_RD_0_ADDR                                                                            (TCSR_TCSR_REGS_REG_BASE      + 0x00025024)
#define HWIO_TCSR_A7SS_SPARE_REG_RD_0_OFFS                                                                            (TCSR_TCSR_REGS_REG_BASE_OFFS + 0x00025024)
#define HWIO_TCSR_A7SS_SPARE_REG_RD_0_RMSK                                                                            0xffffffff
#define HWIO_TCSR_A7SS_SPARE_REG_RD_0_IN          \
        in_dword_masked(HWIO_TCSR_A7SS_SPARE_REG_RD_0_ADDR, HWIO_TCSR_A7SS_SPARE_REG_RD_0_RMSK)
#define HWIO_TCSR_A7SS_SPARE_REG_RD_0_INM(m)      \
        in_dword_masked(HWIO_TCSR_A7SS_SPARE_REG_RD_0_ADDR, m)
#define HWIO_TCSR_A7SS_SPARE_REG_RD_0_SPARE_REG_RD_0_BMSK                                                             0xffffffff
#define HWIO_TCSR_A7SS_SPARE_REG_RD_0_SPARE_REG_RD_0_SHFT                                                                    0x0

#define HWIO_TCSR_A7SS_SPARE_REG_RD_1_ADDR                                                                            (TCSR_TCSR_REGS_REG_BASE      + 0x00002528)
#define HWIO_TCSR_A7SS_SPARE_REG_RD_1_OFFS                                                                            (TCSR_TCSR_REGS_REG_BASE_OFFS + 0x00002528)
#define HWIO_TCSR_A7SS_SPARE_REG_RD_1_RMSK                                                                            0xffffffff
#define HWIO_TCSR_A7SS_SPARE_REG_RD_1_IN          \
        in_dword_masked(HWIO_TCSR_A7SS_SPARE_REG_RD_1_ADDR, HWIO_TCSR_A7SS_SPARE_REG_RD_1_RMSK)
#define HWIO_TCSR_A7SS_SPARE_REG_RD_1_INM(m)      \
        in_dword_masked(HWIO_TCSR_A7SS_SPARE_REG_RD_1_ADDR, m)
#define HWIO_TCSR_A7SS_SPARE_REG_RD_1_SPARE_REG_RD_1_BMSK                                                             0xffffffff
#define HWIO_TCSR_A7SS_SPARE_REG_RD_1_SPARE_REG_RD_1_SHFT                                                                    0x0

#define HWIO_TCSR_A7SS_SPARE_REG_RD_2_ADDR                                                                            (TCSR_TCSR_REGS_REG_BASE      + 0x0000252c)
#define HWIO_TCSR_A7SS_SPARE_REG_RD_2_OFFS                                                                            (TCSR_TCSR_REGS_REG_BASE_OFFS + 0x0000252c)
#define HWIO_TCSR_A7SS_SPARE_REG_RD_2_RMSK                                                                            0xffffffff
#define HWIO_TCSR_A7SS_SPARE_REG_RD_2_IN          \
        in_dword_masked(HWIO_TCSR_A7SS_SPARE_REG_RD_2_ADDR, HWIO_TCSR_A7SS_SPARE_REG_RD_2_RMSK)
#define HWIO_TCSR_A7SS_SPARE_REG_RD_2_INM(m)      \
        in_dword_masked(HWIO_TCSR_A7SS_SPARE_REG_RD_2_ADDR, m)
#define HWIO_TCSR_A7SS_SPARE_REG_RD_2_SPARE_REG_RD_2_BMSK                                                             0xffffffff
#define HWIO_TCSR_A7SS_SPARE_REG_RD_2_SPARE_REG_RD_2_SHFT                                                                    0x0

#define HWIO_TCSR_A7SS_SPARE_REG_RD_3_ADDR                                                                            (TCSR_TCSR_REGS_REG_BASE      + 0x00002530)
#define HWIO_TCSR_A7SS_SPARE_REG_RD_3_OFFS                                                                            (TCSR_TCSR_REGS_REG_BASE_OFFS + 0x00002530)
#define HWIO_TCSR_A7SS_SPARE_REG_RD_3_RMSK                                                                            0xffffffff
#define HWIO_TCSR_A7SS_SPARE_REG_RD_3_IN          \
        in_dword_masked(HWIO_TCSR_A7SS_SPARE_REG_RD_3_ADDR, HWIO_TCSR_A7SS_SPARE_REG_RD_3_RMSK)
#define HWIO_TCSR_A7SS_SPARE_REG_RD_3_INM(m)      \
        in_dword_masked(HWIO_TCSR_A7SS_SPARE_REG_RD_3_ADDR, m)
#define HWIO_TCSR_A7SS_SPARE_REG_RD_3_SPARE_REG_RD_3_BMSK                                                             0xffffffff
#define HWIO_TCSR_A7SS_SPARE_REG_RD_3_SPARE_REG_RD_3_SHFT                                                                    0x0

#define HWIO_TCSR_BOOT_IMEM_START_ADDRESS_ADDR                                                                        (TCSR_TCSR_REGS_REG_BASE      + 0x00013100)
#define HWIO_TCSR_BOOT_IMEM_START_ADDRESS_OFFS                                                                        (TCSR_TCSR_REGS_REG_BASE_OFFS + 0x00013100)
#define HWIO_TCSR_BOOT_IMEM_START_ADDRESS_RMSK                                                                        0xffffffff
#define HWIO_TCSR_BOOT_IMEM_START_ADDRESS_IN          \
        in_dword_masked(HWIO_TCSR_BOOT_IMEM_START_ADDRESS_ADDR, HWIO_TCSR_BOOT_IMEM_START_ADDRESS_RMSK)
#define HWIO_TCSR_BOOT_IMEM_START_ADDRESS_INM(m)      \
        in_dword_masked(HWIO_TCSR_BOOT_IMEM_START_ADDRESS_ADDR, m)
#define HWIO_TCSR_BOOT_IMEM_START_ADDRESS_BOOT_IMEM_START_ADDRESS_BMSK                                                0xffffffff
#define HWIO_TCSR_BOOT_IMEM_START_ADDRESS_BOOT_IMEM_START_ADDRESS_SHFT                                                       0x0

#define HWIO_TCSR_BOOT_IMEM_SIZE_ADDR                                                                                 (TCSR_TCSR_REGS_REG_BASE      + 0x00013200)
#define HWIO_TCSR_BOOT_IMEM_SIZE_OFFS                                                                                 (TCSR_TCSR_REGS_REG_BASE_OFFS + 0x00013200)
#define HWIO_TCSR_BOOT_IMEM_SIZE_RMSK                                                                                 0xffffffff
#define HWIO_TCSR_BOOT_IMEM_SIZE_IN          \
        in_dword_masked(HWIO_TCSR_BOOT_IMEM_SIZE_ADDR, HWIO_TCSR_BOOT_IMEM_SIZE_RMSK)
#define HWIO_TCSR_BOOT_IMEM_SIZE_INM(m)      \
        in_dword_masked(HWIO_TCSR_BOOT_IMEM_SIZE_ADDR, m)
#define HWIO_TCSR_BOOT_IMEM_SIZE_BOOT_IMEM_SIZE_BMSK                                                                  0xffffffff
#define HWIO_TCSR_BOOT_IMEM_SIZE_BOOT_IMEM_SIZE_SHFT                                                                         0x0

#define HWIO_TCSR_BOOT_IMEM_DISABLE_ADDR                                                                              (TCSR_TCSR_REGS_REG_BASE      + 0x00013300)
#define HWIO_TCSR_BOOT_IMEM_DISABLE_OFFS                                                                              (TCSR_TCSR_REGS_REG_BASE_OFFS + 0x00013300)
#define HWIO_TCSR_BOOT_IMEM_DISABLE_RMSK                                                                                     0x1
#define HWIO_TCSR_BOOT_IMEM_DISABLE_IN          \
        in_dword_masked(HWIO_TCSR_BOOT_IMEM_DISABLE_ADDR, HWIO_TCSR_BOOT_IMEM_DISABLE_RMSK)
#define HWIO_TCSR_BOOT_IMEM_DISABLE_INM(m)      \
        in_dword_masked(HWIO_TCSR_BOOT_IMEM_DISABLE_ADDR, m)
#define HWIO_TCSR_BOOT_IMEM_DISABLE_OUT(v)      \
        out_dword(HWIO_TCSR_BOOT_IMEM_DISABLE_ADDR,v)
#define HWIO_TCSR_BOOT_IMEM_DISABLE_OUTM(m,v) \
        out_dword_masked_ns(HWIO_TCSR_BOOT_IMEM_DISABLE_ADDR,m,v,HWIO_TCSR_BOOT_IMEM_DISABLE_IN)
#define HWIO_TCSR_BOOT_IMEM_DISABLE_BOOT_IMEM_DISABLE_BMSK                                                                   0x1
#define HWIO_TCSR_BOOT_IMEM_DISABLE_BOOT_IMEM_DISABLE_SHFT                                                                   0x0

#define HWIO_TCSR_BIAS_REF_LS_EN_ADDR                                                                                 (TCSR_TCSR_REGS_REG_BASE      + 0x00026000)
#define HWIO_TCSR_BIAS_REF_LS_EN_OFFS                                                                                 (TCSR_TCSR_REGS_REG_BASE_OFFS + 0x00026000)
#define HWIO_TCSR_BIAS_REF_LS_EN_RMSK                                                                                      0xfff
#define HWIO_TCSR_BIAS_REF_LS_EN_IN          \
        in_dword_masked(HWIO_TCSR_BIAS_REF_LS_EN_ADDR, HWIO_TCSR_BIAS_REF_LS_EN_RMSK)
#define HWIO_TCSR_BIAS_REF_LS_EN_INM(m)      \
        in_dword_masked(HWIO_TCSR_BIAS_REF_LS_EN_ADDR, m)
#define HWIO_TCSR_BIAS_REF_LS_EN_OUT(v)      \
        out_dword(HWIO_TCSR_BIAS_REF_LS_EN_ADDR,v)
#define HWIO_TCSR_BIAS_REF_LS_EN_OUTM(m,v) \
        out_dword_masked_ns(HWIO_TCSR_BIAS_REF_LS_EN_ADDR,m,v,HWIO_TCSR_BIAS_REF_LS_EN_IN)
#define HWIO_TCSR_BIAS_REF_LS_EN_BIAS_REF_LS_6_AUXBUF_EN_BMSK                                                              0x800
#define HWIO_TCSR_BIAS_REF_LS_EN_BIAS_REF_LS_6_AUXBUF_EN_SHFT                                                                0xb
#define HWIO_TCSR_BIAS_REF_LS_EN_BIAS_REF_LS_6_ATEST_EN_BMSK                                                               0x400
#define HWIO_TCSR_BIAS_REF_LS_EN_BIAS_REF_LS_6_ATEST_EN_SHFT                                                                 0xa
#define HWIO_TCSR_BIAS_REF_LS_EN_BIAS_REF_LS_5_AUXBUF_EN_BMSK                                                              0x200
#define HWIO_TCSR_BIAS_REF_LS_EN_BIAS_REF_LS_5_AUXBUF_EN_SHFT                                                                0x9
#define HWIO_TCSR_BIAS_REF_LS_EN_BIAS_REF_LS_5_ATEST_EN_BMSK                                                               0x100
#define HWIO_TCSR_BIAS_REF_LS_EN_BIAS_REF_LS_5_ATEST_EN_SHFT                                                                 0x8
#define HWIO_TCSR_BIAS_REF_LS_EN_BIAS_REF_LS_4_AUXBUF_EN_BMSK                                                               0x80
#define HWIO_TCSR_BIAS_REF_LS_EN_BIAS_REF_LS_4_AUXBUF_EN_SHFT                                                                0x7
#define HWIO_TCSR_BIAS_REF_LS_EN_BIAS_REF_LS_4_ATEST_EN_BMSK                                                                0x40
#define HWIO_TCSR_BIAS_REF_LS_EN_BIAS_REF_LS_4_ATEST_EN_SHFT                                                                 0x6
#define HWIO_TCSR_BIAS_REF_LS_EN_BIAS_REF_LS_3_AUXBUF_EN_BMSK                                                               0x20
#define HWIO_TCSR_BIAS_REF_LS_EN_BIAS_REF_LS_3_AUXBUF_EN_SHFT                                                                0x5
#define HWIO_TCSR_BIAS_REF_LS_EN_BIAS_REF_LS_3_ATEST_EN_BMSK                                                                0x10
#define HWIO_TCSR_BIAS_REF_LS_EN_BIAS_REF_LS_3_ATEST_EN_SHFT                                                                 0x4
#define HWIO_TCSR_BIAS_REF_LS_EN_BIAS_REF_LS_2_AUXBUF_EN_BMSK                                                                0x8
#define HWIO_TCSR_BIAS_REF_LS_EN_BIAS_REF_LS_2_AUXBUF_EN_SHFT                                                                0x3
#define HWIO_TCSR_BIAS_REF_LS_EN_BIAS_REF_LS_2_ATEST_EN_BMSK                                                                 0x4
#define HWIO_TCSR_BIAS_REF_LS_EN_BIAS_REF_LS_2_ATEST_EN_SHFT                                                                 0x2
#define HWIO_TCSR_BIAS_REF_LS_EN_BIAS_REF_LS_1_AUXBUF_EN_BMSK                                                                0x2
#define HWIO_TCSR_BIAS_REF_LS_EN_BIAS_REF_LS_1_AUXBUF_EN_SHFT                                                                0x1
#define HWIO_TCSR_BIAS_REF_LS_EN_BIAS_REF_LS_1_ATEST_EN_BMSK                                                                 0x1
#define HWIO_TCSR_BIAS_REF_LS_EN_BIAS_REF_LS_1_ATEST_EN_SHFT                                                                 0x0

#define HWIO_TCSR_SPARE_REG_RG15_1_ADDR                                                                               (TCSR_TCSR_REGS_REG_BASE      + 0x0000f000)
#define HWIO_TCSR_SPARE_REG_RG15_1_OFFS                                                                               (TCSR_TCSR_REGS_REG_BASE_OFFS + 0x0000f000)
#define HWIO_TCSR_SPARE_REG_RG15_1_RMSK                                                                               0xffffffff
#define HWIO_TCSR_SPARE_REG_RG15_1_IN          \
        in_dword_masked(HWIO_TCSR_SPARE_REG_RG15_1_ADDR, HWIO_TCSR_SPARE_REG_RG15_1_RMSK)
#define HWIO_TCSR_SPARE_REG_RG15_1_INM(m)      \
        in_dword_masked(HWIO_TCSR_SPARE_REG_RG15_1_ADDR, m)
#define HWIO_TCSR_SPARE_REG_RG15_1_OUT(v)      \
        out_dword(HWIO_TCSR_SPARE_REG_RG15_1_ADDR,v)
#define HWIO_TCSR_SPARE_REG_RG15_1_OUTM(m,v) \
        out_dword_masked_ns(HWIO_TCSR_SPARE_REG_RG15_1_ADDR,m,v,HWIO_TCSR_SPARE_REG_RG15_1_IN)
#define HWIO_TCSR_SPARE_REG_RG15_1_SPARE_REG_RG15_1_BMSK                                                              0xffffffff
#define HWIO_TCSR_SPARE_REG_RG15_1_SPARE_REG_RG15_1_SHFT                                                                     0x0

#define HWIO_TCSR_SPARE_REG_RG15_2_ADDR                                                                               (TCSR_TCSR_REGS_REG_BASE      + 0x0000f004)
#define HWIO_TCSR_SPARE_REG_RG15_2_OFFS                                                                               (TCSR_TCSR_REGS_REG_BASE_OFFS + 0x0000f004)
#define HWIO_TCSR_SPARE_REG_RG15_2_RMSK                                                                               0xffffffff
#define HWIO_TCSR_SPARE_REG_RG15_2_IN          \
        in_dword_masked(HWIO_TCSR_SPARE_REG_RG15_2_ADDR, HWIO_TCSR_SPARE_REG_RG15_2_RMSK)
#define HWIO_TCSR_SPARE_REG_RG15_2_INM(m)      \
        in_dword_masked(HWIO_TCSR_SPARE_REG_RG15_2_ADDR, m)
#define HWIO_TCSR_SPARE_REG_RG15_2_OUT(v)      \
        out_dword(HWIO_TCSR_SPARE_REG_RG15_2_ADDR,v)
#define HWIO_TCSR_SPARE_REG_RG15_2_OUTM(m,v) \
        out_dword_masked_ns(HWIO_TCSR_SPARE_REG_RG15_2_ADDR,m,v,HWIO_TCSR_SPARE_REG_RG15_2_IN)
#define HWIO_TCSR_SPARE_REG_RG15_2_SPARE_REG_RG15_2_BMSK                                                              0xffffffff
#define HWIO_TCSR_SPARE_REG_RG15_2_SPARE_REG_RG15_2_SHFT                                                                     0x0

#define HWIO_TCSR_SPARE_REG_RG15_3_ADDR                                                                               (TCSR_TCSR_REGS_REG_BASE      + 0x0000f008)
#define HWIO_TCSR_SPARE_REG_RG15_3_OFFS                                                                               (TCSR_TCSR_REGS_REG_BASE_OFFS + 0x0000f008)
#define HWIO_TCSR_SPARE_REG_RG15_3_RMSK                                                                               0xffffffff
#define HWIO_TCSR_SPARE_REG_RG15_3_IN          \
        in_dword_masked(HWIO_TCSR_SPARE_REG_RG15_3_ADDR, HWIO_TCSR_SPARE_REG_RG15_3_RMSK)
#define HWIO_TCSR_SPARE_REG_RG15_3_INM(m)      \
        in_dword_masked(HWIO_TCSR_SPARE_REG_RG15_3_ADDR, m)
#define HWIO_TCSR_SPARE_REG_RG15_3_OUT(v)      \
        out_dword(HWIO_TCSR_SPARE_REG_RG15_3_ADDR,v)
#define HWIO_TCSR_SPARE_REG_RG15_3_OUTM(m,v) \
        out_dword_masked_ns(HWIO_TCSR_SPARE_REG_RG15_3_ADDR,m,v,HWIO_TCSR_SPARE_REG_RG15_3_IN)
#define HWIO_TCSR_SPARE_REG_RG15_3_SPARE_REG_RG15_3_BMSK                                                              0xffffffff
#define HWIO_TCSR_SPARE_REG_RG15_3_SPARE_REG_RG15_3_SHFT                                                                     0x0

#define HWIO_TCSR_SPARE_REG_RG15_4_ADDR                                                                               (TCSR_TCSR_REGS_REG_BASE      + 0x0000f00c)
#define HWIO_TCSR_SPARE_REG_RG15_4_OFFS                                                                               (TCSR_TCSR_REGS_REG_BASE_OFFS + 0x0000f00c)
#define HWIO_TCSR_SPARE_REG_RG15_4_RMSK                                                                               0xffffffff
#define HWIO_TCSR_SPARE_REG_RG15_4_IN          \
        in_dword_masked(HWIO_TCSR_SPARE_REG_RG15_4_ADDR, HWIO_TCSR_SPARE_REG_RG15_4_RMSK)
#define HWIO_TCSR_SPARE_REG_RG15_4_INM(m)      \
        in_dword_masked(HWIO_TCSR_SPARE_REG_RG15_4_ADDR, m)
#define HWIO_TCSR_SPARE_REG_RG15_4_OUT(v)      \
        out_dword(HWIO_TCSR_SPARE_REG_RG15_4_ADDR,v)
#define HWIO_TCSR_SPARE_REG_RG15_4_OUTM(m,v) \
        out_dword_masked_ns(HWIO_TCSR_SPARE_REG_RG15_4_ADDR,m,v,HWIO_TCSR_SPARE_REG_RG15_4_IN)
#define HWIO_TCSR_SPARE_REG_RG15_4_SPARE_REG_RG15_4_BMSK                                                              0xffffffff
#define HWIO_TCSR_SPARE_REG_RG15_4_SPARE_REG_RG15_4_SHFT                                                                     0x0

#define HWIO_TCSR_SPARE_REG_RG16_1_ADDR                                                                               (TCSR_TCSR_REGS_REG_BASE      + 0x00010000)
#define HWIO_TCSR_SPARE_REG_RG16_1_OFFS                                                                               (TCSR_TCSR_REGS_REG_BASE_OFFS + 0x00010000)
#define HWIO_TCSR_SPARE_REG_RG16_1_RMSK                                                                               0xffffffff
#define HWIO_TCSR_SPARE_REG_RG16_1_IN          \
        in_dword_masked(HWIO_TCSR_SPARE_REG_RG16_1_ADDR, HWIO_TCSR_SPARE_REG_RG16_1_RMSK)
#define HWIO_TCSR_SPARE_REG_RG16_1_INM(m)      \
        in_dword_masked(HWIO_TCSR_SPARE_REG_RG16_1_ADDR, m)
#define HWIO_TCSR_SPARE_REG_RG16_1_OUT(v)      \
        out_dword(HWIO_TCSR_SPARE_REG_RG16_1_ADDR,v)
#define HWIO_TCSR_SPARE_REG_RG16_1_OUTM(m,v) \
        out_dword_masked_ns(HWIO_TCSR_SPARE_REG_RG16_1_ADDR,m,v,HWIO_TCSR_SPARE_REG_RG16_1_IN)
#define HWIO_TCSR_SPARE_REG_RG16_1_SPARE_REG_RG16_1_BMSK                                                              0xffffffff
#define HWIO_TCSR_SPARE_REG_RG16_1_SPARE_REG_RG16_1_SHFT                                                                     0x0

#define HWIO_TCSR_SPARE_REG_RG16_2_ADDR                                                                               (TCSR_TCSR_REGS_REG_BASE      + 0x00010004)
#define HWIO_TCSR_SPARE_REG_RG16_2_OFFS                                                                               (TCSR_TCSR_REGS_REG_BASE_OFFS + 0x00010004)
#define HWIO_TCSR_SPARE_REG_RG16_2_RMSK                                                                               0xffffffff
#define HWIO_TCSR_SPARE_REG_RG16_2_IN          \
        in_dword_masked(HWIO_TCSR_SPARE_REG_RG16_2_ADDR, HWIO_TCSR_SPARE_REG_RG16_2_RMSK)
#define HWIO_TCSR_SPARE_REG_RG16_2_INM(m)      \
        in_dword_masked(HWIO_TCSR_SPARE_REG_RG16_2_ADDR, m)
#define HWIO_TCSR_SPARE_REG_RG16_2_OUT(v)      \
        out_dword(HWIO_TCSR_SPARE_REG_RG16_2_ADDR,v)
#define HWIO_TCSR_SPARE_REG_RG16_2_OUTM(m,v) \
        out_dword_masked_ns(HWIO_TCSR_SPARE_REG_RG16_2_ADDR,m,v,HWIO_TCSR_SPARE_REG_RG16_2_IN)
#define HWIO_TCSR_SPARE_REG_RG16_2_SPARE_REG_RG16_2_BMSK                                                              0xffffffff
#define HWIO_TCSR_SPARE_REG_RG16_2_SPARE_REG_RG16_2_SHFT                                                                     0x0

#define HWIO_TCSR_SPARE_REG_RG16_3_ADDR                                                                               (TCSR_TCSR_REGS_REG_BASE      + 0x00010008)
#define HWIO_TCSR_SPARE_REG_RG16_3_OFFS                                                                               (TCSR_TCSR_REGS_REG_BASE_OFFS + 0x00010008)
#define HWIO_TCSR_SPARE_REG_RG16_3_RMSK                                                                               0xffffffff
#define HWIO_TCSR_SPARE_REG_RG16_3_IN          \
        in_dword_masked(HWIO_TCSR_SPARE_REG_RG16_3_ADDR, HWIO_TCSR_SPARE_REG_RG16_3_RMSK)
#define HWIO_TCSR_SPARE_REG_RG16_3_INM(m)      \
        in_dword_masked(HWIO_TCSR_SPARE_REG_RG16_3_ADDR, m)
#define HWIO_TCSR_SPARE_REG_RG16_3_OUT(v)      \
        out_dword(HWIO_TCSR_SPARE_REG_RG16_3_ADDR,v)
#define HWIO_TCSR_SPARE_REG_RG16_3_OUTM(m,v) \
        out_dword_masked_ns(HWIO_TCSR_SPARE_REG_RG16_3_ADDR,m,v,HWIO_TCSR_SPARE_REG_RG16_3_IN)
#define HWIO_TCSR_SPARE_REG_RG16_3_SPARE_REG_RG16_3_BMSK                                                              0xffffffff
#define HWIO_TCSR_SPARE_REG_RG16_3_SPARE_REG_RG16_3_SHFT                                                                     0x0

#define HWIO_TCSR_SPARE_REG_RG16_4_ADDR                                                                               (TCSR_TCSR_REGS_REG_BASE      + 0x0001000c)
#define HWIO_TCSR_SPARE_REG_RG16_4_OFFS                                                                               (TCSR_TCSR_REGS_REG_BASE_OFFS + 0x0001000c)
#define HWIO_TCSR_SPARE_REG_RG16_4_RMSK                                                                               0xffffffff
#define HWIO_TCSR_SPARE_REG_RG16_4_IN          \
        in_dword_masked(HWIO_TCSR_SPARE_REG_RG16_4_ADDR, HWIO_TCSR_SPARE_REG_RG16_4_RMSK)
#define HWIO_TCSR_SPARE_REG_RG16_4_INM(m)      \
        in_dword_masked(HWIO_TCSR_SPARE_REG_RG16_4_ADDR, m)
#define HWIO_TCSR_SPARE_REG_RG16_4_OUT(v)      \
        out_dword(HWIO_TCSR_SPARE_REG_RG16_4_ADDR,v)
#define HWIO_TCSR_SPARE_REG_RG16_4_OUTM(m,v) \
        out_dword_masked_ns(HWIO_TCSR_SPARE_REG_RG16_4_ADDR,m,v,HWIO_TCSR_SPARE_REG_RG16_4_IN)
#define HWIO_TCSR_SPARE_REG_RG16_4_SPARE_REG_RG16_4_BMSK                                                              0xffffffff
#define HWIO_TCSR_SPARE_REG_RG16_4_SPARE_REG_RG16_4_SHFT                                                                     0x0

/*----------------------------------------------------------------------------
 * MODULE: MPSS_PERPH
 *--------------------------------------------------------------------------*/

#define MPSS_PERPH_REG_BASE                                                       (MODEM_TOP_BASE      + 0x001a8000)
#define MPSS_PERPH_REG_BASE_OFFS                                                  0x001a8000

#define HWIO_MSS_AHB_ACCESS_ERR_IRQ_EN_ADDR                                       (MPSS_PERPH_REG_BASE      + 0x00000000)
#define HWIO_MSS_AHB_ACCESS_ERR_IRQ_EN_OFFS                                       (MPSS_PERPH_REG_BASE_OFFS + 0x00000000)
#define HWIO_MSS_AHB_ACCESS_ERR_IRQ_EN_RMSK                                              0x1
#define HWIO_MSS_AHB_ACCESS_ERR_IRQ_EN_IN          \
        in_dword_masked(HWIO_MSS_AHB_ACCESS_ERR_IRQ_EN_ADDR, HWIO_MSS_AHB_ACCESS_ERR_IRQ_EN_RMSK)
#define HWIO_MSS_AHB_ACCESS_ERR_IRQ_EN_INM(m)      \
        in_dword_masked(HWIO_MSS_AHB_ACCESS_ERR_IRQ_EN_ADDR, m)
#define HWIO_MSS_AHB_ACCESS_ERR_IRQ_EN_OUT(v)      \
        out_dword(HWIO_MSS_AHB_ACCESS_ERR_IRQ_EN_ADDR,v)
#define HWIO_MSS_AHB_ACCESS_ERR_IRQ_EN_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_AHB_ACCESS_ERR_IRQ_EN_ADDR,m,v,HWIO_MSS_AHB_ACCESS_ERR_IRQ_EN_IN)
#define HWIO_MSS_AHB_ACCESS_ERR_IRQ_EN_EN_BMSK                                           0x1
#define HWIO_MSS_AHB_ACCESS_ERR_IRQ_EN_EN_SHFT                                           0x0

#define HWIO_MSS_AHB_ACCESS_ERR_IRQ_STATUS_ADDR                                   (MPSS_PERPH_REG_BASE      + 0x00000004)
#define HWIO_MSS_AHB_ACCESS_ERR_IRQ_STATUS_OFFS                                   (MPSS_PERPH_REG_BASE_OFFS + 0x00000004)
#define HWIO_MSS_AHB_ACCESS_ERR_IRQ_STATUS_RMSK                                          0x1
#define HWIO_MSS_AHB_ACCESS_ERR_IRQ_STATUS_IN          \
        in_dword_masked(HWIO_MSS_AHB_ACCESS_ERR_IRQ_STATUS_ADDR, HWIO_MSS_AHB_ACCESS_ERR_IRQ_STATUS_RMSK)
#define HWIO_MSS_AHB_ACCESS_ERR_IRQ_STATUS_INM(m)      \
        in_dword_masked(HWIO_MSS_AHB_ACCESS_ERR_IRQ_STATUS_ADDR, m)
#define HWIO_MSS_AHB_ACCESS_ERR_IRQ_STATUS_STATUS_BMSK                                   0x1
#define HWIO_MSS_AHB_ACCESS_ERR_IRQ_STATUS_STATUS_SHFT                                   0x0

#define HWIO_MSS_AHB_ACCESS_ERR_IRQ_CLR_ADDR                                      (MPSS_PERPH_REG_BASE      + 0x00000008)
#define HWIO_MSS_AHB_ACCESS_ERR_IRQ_CLR_OFFS                                      (MPSS_PERPH_REG_BASE_OFFS + 0x00000008)
#define HWIO_MSS_AHB_ACCESS_ERR_IRQ_CLR_RMSK                                             0x1
#define HWIO_MSS_AHB_ACCESS_ERR_IRQ_CLR_OUT(v)      \
        out_dword(HWIO_MSS_AHB_ACCESS_ERR_IRQ_CLR_ADDR,v)
#define HWIO_MSS_AHB_ACCESS_ERR_IRQ_CLR_CMD_BMSK                                         0x1
#define HWIO_MSS_AHB_ACCESS_ERR_IRQ_CLR_CMD_SHFT                                         0x0

#define HWIO_MSS_MSA_CRYPTO_ADDR                                                  (MPSS_PERPH_REG_BASE      + 0x0000000c)
#define HWIO_MSS_MSA_CRYPTO_OFFS                                                  (MPSS_PERPH_REG_BASE_OFFS + 0x0000000c)
#define HWIO_MSS_MSA_CRYPTO_RMSK                                                         0xf
#define HWIO_MSS_MSA_CRYPTO_IN          \
        in_dword_masked(HWIO_MSS_MSA_CRYPTO_ADDR, HWIO_MSS_MSA_CRYPTO_RMSK)
#define HWIO_MSS_MSA_CRYPTO_INM(m)      \
        in_dword_masked(HWIO_MSS_MSA_CRYPTO_ADDR, m)
#define HWIO_MSS_MSA_CRYPTO_OUT(v)      \
        out_dword(HWIO_MSS_MSA_CRYPTO_ADDR,v)
#define HWIO_MSS_MSA_CRYPTO_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_MSA_CRYPTO_ADDR,m,v,HWIO_MSS_MSA_CRYPTO_IN)
#define HWIO_MSS_MSA_CRYPTO_MSA_CRYPTO_BMSK                                              0xf
#define HWIO_MSS_MSA_CRYPTO_MSA_CRYPTO_SHFT                                              0x0

#define HWIO_MSS_MSA_ADDR                                                         (MPSS_PERPH_REG_BASE      + 0x00000010)
#define HWIO_MSS_MSA_OFFS                                                         (MPSS_PERPH_REG_BASE_OFFS + 0x00000010)
#define HWIO_MSS_MSA_RMSK                                                                0x4
#define HWIO_MSS_MSA_IN          \
        in_dword_masked(HWIO_MSS_MSA_ADDR, HWIO_MSS_MSA_RMSK)
#define HWIO_MSS_MSA_INM(m)      \
        in_dword_masked(HWIO_MSS_MSA_ADDR, m)
#define HWIO_MSS_MSA_OUT(v)      \
        out_dword(HWIO_MSS_MSA_ADDR,v)
#define HWIO_MSS_MSA_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_MSA_ADDR,m,v,HWIO_MSS_MSA_IN)
#define HWIO_MSS_MSA_FORCE_Q6_MSA_BMSK                                                   0x4
#define HWIO_MSS_MSA_FORCE_Q6_MSA_SHFT                                                   0x2

#define HWIO_MSS_DEBUG_CLOCK_CTL_ADDR                                             (MPSS_PERPH_REG_BASE      + 0x00000014)
#define HWIO_MSS_DEBUG_CLOCK_CTL_OFFS                                             (MPSS_PERPH_REG_BASE_OFFS + 0x00000014)
#define HWIO_MSS_DEBUG_CLOCK_CTL_RMSK                                             0xff8007ff
#define HWIO_MSS_DEBUG_CLOCK_CTL_IN          \
        in_dword_masked(HWIO_MSS_DEBUG_CLOCK_CTL_ADDR, HWIO_MSS_DEBUG_CLOCK_CTL_RMSK)
#define HWIO_MSS_DEBUG_CLOCK_CTL_INM(m)      \
        in_dword_masked(HWIO_MSS_DEBUG_CLOCK_CTL_ADDR, m)
#define HWIO_MSS_DEBUG_CLOCK_CTL_OUT(v)      \
        out_dword(HWIO_MSS_DEBUG_CLOCK_CTL_ADDR,v)
#define HWIO_MSS_DEBUG_CLOCK_CTL_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_DEBUG_CLOCK_CTL_ADDR,m,v,HWIO_MSS_DEBUG_CLOCK_CTL_IN)
#define HWIO_MSS_DEBUG_CLOCK_CTL_PLL_RESET_SEL_BMSK                               0xe0000000
#define HWIO_MSS_DEBUG_CLOCK_CTL_PLL_RESET_SEL_SHFT                                     0x1d
#define HWIO_MSS_DEBUG_CLOCK_CTL_PLL_BYPASS_SEL_BMSK                              0x1c000000
#define HWIO_MSS_DEBUG_CLOCK_CTL_PLL_BYPASS_SEL_SHFT                                    0x1a
#define HWIO_MSS_DEBUG_CLOCK_CTL_PLLOUT_LV_TEST_SEL_BMSK                           0x3800000
#define HWIO_MSS_DEBUG_CLOCK_CTL_PLLOUT_LV_TEST_SEL_SHFT                                0x17
#define HWIO_MSS_DEBUG_CLOCK_CTL_MODEM_MUX_SEL_BMSK                                    0x7c0
#define HWIO_MSS_DEBUG_CLOCK_CTL_MODEM_MUX_SEL_SHFT                                      0x6
#define HWIO_MSS_DEBUG_CLOCK_CTL_DBG_LEVEL6_MUX_SEL_BMSK                                0x20
#define HWIO_MSS_DEBUG_CLOCK_CTL_DBG_LEVEL6_MUX_SEL_SHFT                                 0x5
#define HWIO_MSS_DEBUG_CLOCK_CTL_DBG_LEVEL5_MUX_SEL_BMSK                                0x10
#define HWIO_MSS_DEBUG_CLOCK_CTL_DBG_LEVEL5_MUX_SEL_SHFT                                 0x4
#define HWIO_MSS_DEBUG_CLOCK_CTL_DBG_LEVEL4_MUX_SEL_BMSK                                 0x8
#define HWIO_MSS_DEBUG_CLOCK_CTL_DBG_LEVEL4_MUX_SEL_SHFT                                 0x3
#define HWIO_MSS_DEBUG_CLOCK_CTL_DBG_LEVEL3_MUX_SEL_BMSK                                 0x4
#define HWIO_MSS_DEBUG_CLOCK_CTL_DBG_LEVEL3_MUX_SEL_SHFT                                 0x2
#define HWIO_MSS_DEBUG_CLOCK_CTL_DBG_LEVEL2_MUX_SEL_BMSK                                 0x2
#define HWIO_MSS_DEBUG_CLOCK_CTL_DBG_LEVEL2_MUX_SEL_SHFT                                 0x1
#define HWIO_MSS_DEBUG_CLOCK_CTL_DBG_LEVEL1_MUX_SEL_BMSK                                 0x1
#define HWIO_MSS_DEBUG_CLOCK_CTL_DBG_LEVEL1_MUX_SEL_SHFT                                 0x0

#define HWIO_MSS_DEBUG_CTL_ADDR                                                   (MPSS_PERPH_REG_BASE      + 0x00000018)
#define HWIO_MSS_DEBUG_CTL_OFFS                                                   (MPSS_PERPH_REG_BASE_OFFS + 0x00000018)
#define HWIO_MSS_DEBUG_CTL_RMSK                                                          0xf
#define HWIO_MSS_DEBUG_CTL_IN          \
        in_dword_masked(HWIO_MSS_DEBUG_CTL_ADDR, HWIO_MSS_DEBUG_CTL_RMSK)
#define HWIO_MSS_DEBUG_CTL_INM(m)      \
        in_dword_masked(HWIO_MSS_DEBUG_CTL_ADDR, m)
#define HWIO_MSS_DEBUG_CTL_OUT(v)      \
        out_dword(HWIO_MSS_DEBUG_CTL_ADDR,v)
#define HWIO_MSS_DEBUG_CTL_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_DEBUG_CTL_ADDR,m,v,HWIO_MSS_DEBUG_CTL_IN)
#define HWIO_MSS_DEBUG_CTL_SMMU_BYPASS_CHECKER_DISABLE_BMSK                              0x8
#define HWIO_MSS_DEBUG_CTL_SMMU_BYPASS_CHECKER_DISABLE_SHFT                              0x3
#define HWIO_MSS_DEBUG_CTL_DAC_DISABLE_ON_Q6_DBG_BMSK                                    0x4
#define HWIO_MSS_DEBUG_CTL_DAC_DISABLE_ON_Q6_DBG_SHFT                                    0x2
#define HWIO_MSS_DEBUG_CTL_GRFC_DISABLE_Q6_DBG_BMSK                                      0x2
#define HWIO_MSS_DEBUG_CTL_GRFC_DISABLE_Q6_DBG_SHFT                                      0x1
#define HWIO_MSS_DEBUG_CTL_GRFC_DISABLE_Q6_WDOG_BMSK                                     0x1
#define HWIO_MSS_DEBUG_CTL_GRFC_DISABLE_Q6_WDOG_SHFT                                     0x0

#define HWIO_MSS_DBG_BUS_CTL_ADDR                                                 (MPSS_PERPH_REG_BASE      + 0x0000001c)
#define HWIO_MSS_DBG_BUS_CTL_OFFS                                                 (MPSS_PERPH_REG_BASE_OFFS + 0x0000001c)
#define HWIO_MSS_DBG_BUS_CTL_RMSK                                                      0xfff
#define HWIO_MSS_DBG_BUS_CTL_IN          \
        in_dword_masked(HWIO_MSS_DBG_BUS_CTL_ADDR, HWIO_MSS_DBG_BUS_CTL_RMSK)
#define HWIO_MSS_DBG_BUS_CTL_INM(m)      \
        in_dword_masked(HWIO_MSS_DBG_BUS_CTL_ADDR, m)
#define HWIO_MSS_DBG_BUS_CTL_OUT(v)      \
        out_dword(HWIO_MSS_DBG_BUS_CTL_ADDR,v)
#define HWIO_MSS_DBG_BUS_CTL_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_DBG_BUS_CTL_ADDR,m,v,HWIO_MSS_DBG_BUS_CTL_IN)
#define HWIO_MSS_DBG_BUS_CTL_AHB2AHB_OFFLINE_SEL_BMSK                                  0xc00
#define HWIO_MSS_DBG_BUS_CTL_AHB2AHB_OFFLINE_SEL_SHFT                                    0xa
#define HWIO_MSS_DBG_BUS_CTL_AHB2AHB_1X1_CNOC_SEL_BMSK                                 0x300
#define HWIO_MSS_DBG_BUS_CTL_AHB2AHB_1X1_CNOC_SEL_SHFT                                   0x8
#define HWIO_MSS_DBG_BUS_CTL_MODEM_DBG_BUS_SEL_BMSK                                     0xf0
#define HWIO_MSS_DBG_BUS_CTL_MODEM_DBG_BUS_SEL_SHFT                                      0x4
#define HWIO_MSS_DBG_BUS_CTL_MPSS_DBG_BUS_SEL_BMSK                                       0xf
#define HWIO_MSS_DBG_BUS_CTL_MPSS_DBG_BUS_SEL_SHFT                                       0x0

#define HWIO_MSS_DL_MCMB_HWE_MUX_SEL_ADDR                                         (MPSS_PERPH_REG_BASE      + 0x00000020)
#define HWIO_MSS_DL_MCMB_HWE_MUX_SEL_OFFS                                         (MPSS_PERPH_REG_BASE_OFFS + 0x00000020)
#define HWIO_MSS_DL_MCMB_HWE_MUX_SEL_RMSK                                                0xf
#define HWIO_MSS_DL_MCMB_HWE_MUX_SEL_IN          \
        in_dword_masked(HWIO_MSS_DL_MCMB_HWE_MUX_SEL_ADDR, HWIO_MSS_DL_MCMB_HWE_MUX_SEL_RMSK)
#define HWIO_MSS_DL_MCMB_HWE_MUX_SEL_INM(m)      \
        in_dword_masked(HWIO_MSS_DL_MCMB_HWE_MUX_SEL_ADDR, m)
#define HWIO_MSS_DL_MCMB_HWE_MUX_SEL_OUT(v)      \
        out_dword(HWIO_MSS_DL_MCMB_HWE_MUX_SEL_ADDR,v)
#define HWIO_MSS_DL_MCMB_HWE_MUX_SEL_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_DL_MCMB_HWE_MUX_SEL_ADDR,m,v,HWIO_MSS_DL_MCMB_HWE_MUX_SEL_IN)
#define HWIO_MSS_DL_MCMB_HWE_MUX_SEL_MUX_SEL_BMSK                                        0xf
#define HWIO_MSS_DL_MCMB_HWE_MUX_SEL_MUX_SEL_SHFT                                        0x0

#define HWIO_MSS_BIMC_DANGER_ADDR                                                 (MPSS_PERPH_REG_BASE      + 0x00000024)
#define HWIO_MSS_BIMC_DANGER_OFFS                                                 (MPSS_PERPH_REG_BASE_OFFS + 0x00000024)
#define HWIO_MSS_BIMC_DANGER_RMSK                                                        0x7
#define HWIO_MSS_BIMC_DANGER_IN          \
        in_dword_masked(HWIO_MSS_BIMC_DANGER_ADDR, HWIO_MSS_BIMC_DANGER_RMSK)
#define HWIO_MSS_BIMC_DANGER_INM(m)      \
        in_dword_masked(HWIO_MSS_BIMC_DANGER_ADDR, m)
#define HWIO_MSS_BIMC_DANGER_OUT(v)      \
        out_dword(HWIO_MSS_BIMC_DANGER_ADDR,v)
#define HWIO_MSS_BIMC_DANGER_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_BIMC_DANGER_ADDR,m,v,HWIO_MSS_BIMC_DANGER_IN)
#define HWIO_MSS_BIMC_DANGER_MSS_SAVE_BMSK                                               0x4
#define HWIO_MSS_BIMC_DANGER_MSS_SAVE_SHFT                                               0x2
#define HWIO_MSS_BIMC_DANGER_MSS_DANGER_BMSK                                             0x3
#define HWIO_MSS_BIMC_DANGER_MSS_DANGER_SHFT                                             0x0

#define HWIO_MSS_HW_VERSION_ADDR                                                  (MPSS_PERPH_REG_BASE      + 0x00000028)
#define HWIO_MSS_HW_VERSION_OFFS                                                  (MPSS_PERPH_REG_BASE_OFFS + 0x00000028)
#define HWIO_MSS_HW_VERSION_RMSK                                                  0xffffffff
#define HWIO_MSS_HW_VERSION_IN          \
        in_dword_masked(HWIO_MSS_HW_VERSION_ADDR, HWIO_MSS_HW_VERSION_RMSK)
#define HWIO_MSS_HW_VERSION_INM(m)      \
        in_dword_masked(HWIO_MSS_HW_VERSION_ADDR, m)
#define HWIO_MSS_HW_VERSION_MAJOR_BMSK                                            0xf0000000
#define HWIO_MSS_HW_VERSION_MAJOR_SHFT                                                  0x1c
#define HWIO_MSS_HW_VERSION_MINOR_BMSK                                             0xfff0000
#define HWIO_MSS_HW_VERSION_MINOR_SHFT                                                  0x10
#define HWIO_MSS_HW_VERSION_STEP_BMSK                                                 0xffff
#define HWIO_MSS_HW_VERSION_STEP_SHFT                                                    0x0

#define HWIO_MSS_STATUS_ADDR                                                      (MPSS_PERPH_REG_BASE      + 0x0000002c)
#define HWIO_MSS_STATUS_OFFS                                                      (MPSS_PERPH_REG_BASE_OFFS + 0x0000002c)
#define HWIO_MSS_STATUS_RMSK                                                            0x3f
#define HWIO_MSS_STATUS_IN          \
        in_dword_masked(HWIO_MSS_STATUS_ADDR, HWIO_MSS_STATUS_RMSK)
#define HWIO_MSS_STATUS_INM(m)      \
        in_dword_masked(HWIO_MSS_STATUS_ADDR, m)
#define HWIO_MSS_STATUS_MSS_OFFLINE_RESET_BMSK                                          0x20
#define HWIO_MSS_STATUS_MSS_OFFLINE_RESET_SHFT                                           0x5
#define HWIO_MSS_STATUS_MSS_RSC_IDLE_BMSK                                               0x10
#define HWIO_MSS_STATUS_MSS_RSC_IDLE_SHFT                                                0x4
#define HWIO_MSS_STATUS_Q6_RSC_IDLE_BMSK                                                 0x8
#define HWIO_MSS_STATUS_Q6_RSC_IDLE_SHFT                                                 0x3
#define HWIO_MSS_STATUS_Q6_CORE_IDLE_BMSK                                                0x4
#define HWIO_MSS_STATUS_Q6_CORE_IDLE_SHFT                                                0x2
#define HWIO_MSS_STATUS_Q6_AHB_ACCESS_BMSK                                               0x2
#define HWIO_MSS_STATUS_Q6_AHB_ACCESS_SHFT                                               0x1
#define HWIO_MSS_STATUS_Q6_AXI_TO_BMIC_ACCESS_BMSK                                       0x1
#define HWIO_MSS_STATUS_Q6_AXI_TO_BMIC_ACCESS_SHFT                                       0x0

#define HWIO_MSS_ALT_RESET_Q6SS_ADDR                                              (MPSS_PERPH_REG_BASE      + 0x00000030)
#define HWIO_MSS_ALT_RESET_Q6SS_OFFS                                              (MPSS_PERPH_REG_BASE_OFFS + 0x00000030)
#define HWIO_MSS_ALT_RESET_Q6SS_RMSK                                                     0x1
#define HWIO_MSS_ALT_RESET_Q6SS_IN          \
        in_dword_masked(HWIO_MSS_ALT_RESET_Q6SS_ADDR, HWIO_MSS_ALT_RESET_Q6SS_RMSK)
#define HWIO_MSS_ALT_RESET_Q6SS_INM(m)      \
        in_dword_masked(HWIO_MSS_ALT_RESET_Q6SS_ADDR, m)
#define HWIO_MSS_ALT_RESET_Q6SS_OUT(v)      \
        out_dword(HWIO_MSS_ALT_RESET_Q6SS_ADDR,v)
#define HWIO_MSS_ALT_RESET_Q6SS_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_ALT_RESET_Q6SS_ADDR,m,v,HWIO_MSS_ALT_RESET_Q6SS_IN)
#define HWIO_MSS_ALT_RESET_Q6SS_EN_BMSK                                                  0x1
#define HWIO_MSS_ALT_RESET_Q6SS_EN_SHFT                                                  0x0

#define HWIO_MSS_OFFLINE_MEM_SLP_CNTL_ADDR                                        (MPSS_PERPH_REG_BASE      + 0x00000034)
#define HWIO_MSS_OFFLINE_MEM_SLP_CNTL_OFFS                                        (MPSS_PERPH_REG_BASE_OFFS + 0x00000034)
#define HWIO_MSS_OFFLINE_MEM_SLP_CNTL_RMSK                                        0xffffffff
#define HWIO_MSS_OFFLINE_MEM_SLP_CNTL_IN          \
        in_dword_masked(HWIO_MSS_OFFLINE_MEM_SLP_CNTL_ADDR, HWIO_MSS_OFFLINE_MEM_SLP_CNTL_RMSK)
#define HWIO_MSS_OFFLINE_MEM_SLP_CNTL_INM(m)      \
        in_dword_masked(HWIO_MSS_OFFLINE_MEM_SLP_CNTL_ADDR, m)
#define HWIO_MSS_OFFLINE_MEM_SLP_CNTL_OUT(v)      \
        out_dword(HWIO_MSS_OFFLINE_MEM_SLP_CNTL_ADDR,v)
#define HWIO_MSS_OFFLINE_MEM_SLP_CNTL_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_OFFLINE_MEM_SLP_CNTL_ADDR,m,v,HWIO_MSS_OFFLINE_MEM_SLP_CNTL_IN)
#define HWIO_MSS_OFFLINE_MEM_SLP_CNTL_OFFLINE_MEM_SLP_CNTL_BMSK                   0xffffffff
#define HWIO_MSS_OFFLINE_MEM_SLP_CNTL_OFFLINE_MEM_SLP_CNTL_SHFT                          0x0

#define HWIO_MSS_OFFLINE_MEM_SLP_CNTL2_ADDR                                       (MPSS_PERPH_REG_BASE      + 0x00000038)
#define HWIO_MSS_OFFLINE_MEM_SLP_CNTL2_OFFS                                       (MPSS_PERPH_REG_BASE_OFFS + 0x00000038)
#define HWIO_MSS_OFFLINE_MEM_SLP_CNTL2_RMSK                                       0xffffffff
#define HWIO_MSS_OFFLINE_MEM_SLP_CNTL2_IN          \
        in_dword_masked(HWIO_MSS_OFFLINE_MEM_SLP_CNTL2_ADDR, HWIO_MSS_OFFLINE_MEM_SLP_CNTL2_RMSK)
#define HWIO_MSS_OFFLINE_MEM_SLP_CNTL2_INM(m)      \
        in_dword_masked(HWIO_MSS_OFFLINE_MEM_SLP_CNTL2_ADDR, m)
#define HWIO_MSS_OFFLINE_MEM_SLP_CNTL2_OUT(v)      \
        out_dword(HWIO_MSS_OFFLINE_MEM_SLP_CNTL2_ADDR,v)
#define HWIO_MSS_OFFLINE_MEM_SLP_CNTL2_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_OFFLINE_MEM_SLP_CNTL2_ADDR,m,v,HWIO_MSS_OFFLINE_MEM_SLP_CNTL2_IN)
#define HWIO_MSS_OFFLINE_MEM_SLP_CNTL2_OFFLINE_MEM_SLP_CNTL_BMSK                  0xffffffff
#define HWIO_MSS_OFFLINE_MEM_SLP_CNTL2_OFFLINE_MEM_SLP_CNTL_SHFT                         0x0

#define HWIO_MSS_OFFLINE_MEM_SLP_CNTL3_ADDR                                       (MPSS_PERPH_REG_BASE      + 0x0000003c)
#define HWIO_MSS_OFFLINE_MEM_SLP_CNTL3_OFFS                                       (MPSS_PERPH_REG_BASE_OFFS + 0x0000003c)
#define HWIO_MSS_OFFLINE_MEM_SLP_CNTL3_RMSK                                       0xffffffff
#define HWIO_MSS_OFFLINE_MEM_SLP_CNTL3_IN          \
        in_dword_masked(HWIO_MSS_OFFLINE_MEM_SLP_CNTL3_ADDR, HWIO_MSS_OFFLINE_MEM_SLP_CNTL3_RMSK)
#define HWIO_MSS_OFFLINE_MEM_SLP_CNTL3_INM(m)      \
        in_dword_masked(HWIO_MSS_OFFLINE_MEM_SLP_CNTL3_ADDR, m)
#define HWIO_MSS_OFFLINE_MEM_SLP_CNTL3_OUT(v)      \
        out_dword(HWIO_MSS_OFFLINE_MEM_SLP_CNTL3_ADDR,v)
#define HWIO_MSS_OFFLINE_MEM_SLP_CNTL3_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_OFFLINE_MEM_SLP_CNTL3_ADDR,m,v,HWIO_MSS_OFFLINE_MEM_SLP_CNTL3_IN)
#define HWIO_MSS_OFFLINE_MEM_SLP_CNTL3_OFFLINE_MEM_SLP_CNTL_BMSK                  0xffffffff
#define HWIO_MSS_OFFLINE_MEM_SLP_CNTL3_OFFLINE_MEM_SLP_CNTL_SHFT                         0x0

#define HWIO_MSS_QLINK_PHY_CNTRL_ADDR                                             (MPSS_PERPH_REG_BASE      + 0x00000040)
#define HWIO_MSS_QLINK_PHY_CNTRL_OFFS                                             (MPSS_PERPH_REG_BASE_OFFS + 0x00000040)
#define HWIO_MSS_QLINK_PHY_CNTRL_RMSK                                                   0x17
#define HWIO_MSS_QLINK_PHY_CNTRL_IN          \
        in_dword_masked(HWIO_MSS_QLINK_PHY_CNTRL_ADDR, HWIO_MSS_QLINK_PHY_CNTRL_RMSK)
#define HWIO_MSS_QLINK_PHY_CNTRL_INM(m)      \
        in_dword_masked(HWIO_MSS_QLINK_PHY_CNTRL_ADDR, m)
#define HWIO_MSS_QLINK_PHY_CNTRL_OUT(v)      \
        out_dword(HWIO_MSS_QLINK_PHY_CNTRL_ADDR,v)
#define HWIO_MSS_QLINK_PHY_CNTRL_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QLINK_PHY_CNTRL_ADDR,m,v,HWIO_MSS_QLINK_PHY_CNTRL_IN)
#define HWIO_MSS_QLINK_PHY_CNTRL_RESET_BMSK                                             0x10
#define HWIO_MSS_QLINK_PHY_CNTRL_RESET_SHFT                                              0x4
#define HWIO_MSS_QLINK_PHY_CNTRL_CLAMP_DFT_BMSK                                          0x4
#define HWIO_MSS_QLINK_PHY_CNTRL_CLAMP_DFT_SHFT                                          0x2
#define HWIO_MSS_QLINK_PHY_CNTRL_CLAMP_CLK_BMSK                                          0x2
#define HWIO_MSS_QLINK_PHY_CNTRL_CLAMP_CLK_SHFT                                          0x1
#define HWIO_MSS_QLINK_PHY_CNTRL_CLAMP_BMSK                                              0x1
#define HWIO_MSS_QLINK_PHY_CNTRL_CLAMP_SHFT                                              0x0

#define HWIO_MSS_SYSTEM_CACHE_CTL_ADDR                                            (MPSS_PERPH_REG_BASE      + 0x00000044)
#define HWIO_MSS_SYSTEM_CACHE_CTL_OFFS                                            (MPSS_PERPH_REG_BASE_OFFS + 0x00000044)
#define HWIO_MSS_SYSTEM_CACHE_CTL_RMSK                                            0xffffffff
#define HWIO_MSS_SYSTEM_CACHE_CTL_IN          \
        in_dword_masked(HWIO_MSS_SYSTEM_CACHE_CTL_ADDR, HWIO_MSS_SYSTEM_CACHE_CTL_RMSK)
#define HWIO_MSS_SYSTEM_CACHE_CTL_INM(m)      \
        in_dword_masked(HWIO_MSS_SYSTEM_CACHE_CTL_ADDR, m)
#define HWIO_MSS_SYSTEM_CACHE_CTL_OUT(v)      \
        out_dword(HWIO_MSS_SYSTEM_CACHE_CTL_ADDR,v)
#define HWIO_MSS_SYSTEM_CACHE_CTL_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_SYSTEM_CACHE_CTL_ADDR,m,v,HWIO_MSS_SYSTEM_CACHE_CTL_IN)
#define HWIO_MSS_SYSTEM_CACHE_CTL_SPARE_31_19_BMSK                                0xfff80000
#define HWIO_MSS_SYSTEM_CACHE_CTL_SPARE_31_19_SHFT                                      0x13
#define HWIO_MSS_SYSTEM_CACHE_CTL_AINNERCACHEABLE_BMSK                               0x40000
#define HWIO_MSS_SYSTEM_CACHE_CTL_AINNERCACHEABLE_SHFT                                  0x12
#define HWIO_MSS_SYSTEM_CACHE_CTL_ACGRANULETRANSACTION_BMSK                          0x20000
#define HWIO_MSS_SYSTEM_CACHE_CTL_ACGRANULETRANSACTION_SHFT                             0x11
#define HWIO_MSS_SYSTEM_CACHE_CTL_ACACHEABLE_BMSK                                    0x10000
#define HWIO_MSS_SYSTEM_CACHE_CTL_ACACHEABLE_SHFT                                       0x10
#define HWIO_MSS_SYSTEM_CACHE_CTL_ACACHEOPTYPE_BMSK                                   0xf000
#define HWIO_MSS_SYSTEM_CACHE_CTL_ACACHEOPTYPE_SHFT                                      0xc
#define HWIO_MSS_SYSTEM_CACHE_CTL_ADEVICETYPE_BMSK                                     0x800
#define HWIO_MSS_SYSTEM_CACHE_CTL_ADEVICETYPE_SHFT                                       0xb
#define HWIO_MSS_SYSTEM_CACHE_CTL_ADEVICE_BMSK                                         0x400
#define HWIO_MSS_SYSTEM_CACHE_CTL_ADEVICE_SHFT                                           0xa
#define HWIO_MSS_SYSTEM_CACHE_CTL_AWRITETHROUGH_BMSK                                   0x200
#define HWIO_MSS_SYSTEM_CACHE_CTL_AWRITETHROUGH_SHFT                                     0x9
#define HWIO_MSS_SYSTEM_CACHE_CTL_AINNERSHARED_BMSK                                    0x100
#define HWIO_MSS_SYSTEM_CACHE_CTL_AINNERSHARED_SHFT                                      0x8
#define HWIO_MSS_SYSTEM_CACHE_CTL_ASHARED_BMSK                                          0x80
#define HWIO_MSS_SYSTEM_CACHE_CTL_ASHARED_SHFT                                           0x7
#define HWIO_MSS_SYSTEM_CACHE_CTL_ANOALLOCATE_BMSK                                      0x40
#define HWIO_MSS_SYSTEM_CACHE_CTL_ANOALLOCATE_SHFT                                       0x6
#define HWIO_MSS_SYSTEM_CACHE_CTL_ATRANSIENT_BMSK                                       0x20
#define HWIO_MSS_SYSTEM_CACHE_CTL_ATRANSIENT_SHFT                                        0x5
#define HWIO_MSS_SYSTEM_CACHE_CTL_ACID_BMSK                                             0x1f
#define HWIO_MSS_SYSTEM_CACHE_CTL_ACID_SHFT                                              0x0

#define HWIO_MSS_SW_FEATURE_FUSES_ADDR                                            (MPSS_PERPH_REG_BASE      + 0x00000048)
#define HWIO_MSS_SW_FEATURE_FUSES_OFFS                                            (MPSS_PERPH_REG_BASE_OFFS + 0x00000048)
#define HWIO_MSS_SW_FEATURE_FUSES_RMSK                                            0xffffffff
#define HWIO_MSS_SW_FEATURE_FUSES_IN          \
        in_dword_masked(HWIO_MSS_SW_FEATURE_FUSES_ADDR, HWIO_MSS_SW_FEATURE_FUSES_RMSK)
#define HWIO_MSS_SW_FEATURE_FUSES_INM(m)      \
        in_dword_masked(HWIO_MSS_SW_FEATURE_FUSES_ADDR, m)
#define HWIO_MSS_SW_FEATURE_FUSES_MSS_SW_FEATURE_FUSES_31_0_BMSK                  0xffffffff
#define HWIO_MSS_SW_FEATURE_FUSES_MSS_SW_FEATURE_FUSES_31_0_SHFT                         0x0

#define HWIO_MSS_SW_FEATURE_FUSES2_ADDR                                           (MPSS_PERPH_REG_BASE      + 0x0000004c)
#define HWIO_MSS_SW_FEATURE_FUSES2_OFFS                                           (MPSS_PERPH_REG_BASE_OFFS + 0x0000004c)
#define HWIO_MSS_SW_FEATURE_FUSES2_RMSK                                           0xffffffff
#define HWIO_MSS_SW_FEATURE_FUSES2_IN          \
        in_dword_masked(HWIO_MSS_SW_FEATURE_FUSES2_ADDR, HWIO_MSS_SW_FEATURE_FUSES2_RMSK)
#define HWIO_MSS_SW_FEATURE_FUSES2_INM(m)      \
        in_dword_masked(HWIO_MSS_SW_FEATURE_FUSES2_ADDR, m)
#define HWIO_MSS_SW_FEATURE_FUSES2_MSS_SW_FEATURE_FUSES2_31_0_BMSK                0xffffffff
#define HWIO_MSS_SW_FEATURE_FUSES2_MSS_SW_FEATURE_FUSES2_31_0_SHFT                       0x0

#define HWIO_MSS_SW_FEATURE_FUSES3_ADDR                                           (MPSS_PERPH_REG_BASE      + 0x00000050)
#define HWIO_MSS_SW_FEATURE_FUSES3_OFFS                                           (MPSS_PERPH_REG_BASE_OFFS + 0x00000050)
#define HWIO_MSS_SW_FEATURE_FUSES3_RMSK                                           0xffffffff
#define HWIO_MSS_SW_FEATURE_FUSES3_IN          \
        in_dword_masked(HWIO_MSS_SW_FEATURE_FUSES3_ADDR, HWIO_MSS_SW_FEATURE_FUSES3_RMSK)
#define HWIO_MSS_SW_FEATURE_FUSES3_INM(m)      \
        in_dword_masked(HWIO_MSS_SW_FEATURE_FUSES3_ADDR, m)
#define HWIO_MSS_SW_FEATURE_FUSES3_MSS_SW_FEATURE_FUSES3_31_0_BMSK                0xffffffff
#define HWIO_MSS_SW_FEATURE_FUSES3_MSS_SW_FEATURE_FUSES3_31_0_SHFT                       0x0

#define HWIO_MSS_OFFLINE_MEM_SLP_CNTL_END_ADDR                                    (MPSS_PERPH_REG_BASE      + 0x0000005c)
#define HWIO_MSS_OFFLINE_MEM_SLP_CNTL_END_OFFS                                    (MPSS_PERPH_REG_BASE_OFFS + 0x0000005c)
#define HWIO_MSS_OFFLINE_MEM_SLP_CNTL_END_RMSK                                    0xffffffff
#define HWIO_MSS_OFFLINE_MEM_SLP_CNTL_END_IN          \
        in_dword_masked(HWIO_MSS_OFFLINE_MEM_SLP_CNTL_END_ADDR, HWIO_MSS_OFFLINE_MEM_SLP_CNTL_END_RMSK)
#define HWIO_MSS_OFFLINE_MEM_SLP_CNTL_END_INM(m)      \
        in_dword_masked(HWIO_MSS_OFFLINE_MEM_SLP_CNTL_END_ADDR, m)
#define HWIO_MSS_OFFLINE_MEM_SLP_CNTL_END_OFFLINE_MEM_SLP_CNTL_END_BMSK           0xffffffff
#define HWIO_MSS_OFFLINE_MEM_SLP_CNTL_END_OFFLINE_MEM_SLP_CNTL_END_SHFT                  0x0

#define HWIO_MSS_OFFLINE_MEM_SLP_CNTL2_END_ADDR                                   (MPSS_PERPH_REG_BASE      + 0x00000060)
#define HWIO_MSS_OFFLINE_MEM_SLP_CNTL2_END_OFFS                                   (MPSS_PERPH_REG_BASE_OFFS + 0x00000060)
#define HWIO_MSS_OFFLINE_MEM_SLP_CNTL2_END_RMSK                                   0xffffffff
#define HWIO_MSS_OFFLINE_MEM_SLP_CNTL2_END_IN          \
        in_dword_masked(HWIO_MSS_OFFLINE_MEM_SLP_CNTL2_END_ADDR, HWIO_MSS_OFFLINE_MEM_SLP_CNTL2_END_RMSK)
#define HWIO_MSS_OFFLINE_MEM_SLP_CNTL2_END_INM(m)      \
        in_dword_masked(HWIO_MSS_OFFLINE_MEM_SLP_CNTL2_END_ADDR, m)
#define HWIO_MSS_OFFLINE_MEM_SLP_CNTL2_END_OFFLINE_MEM_SLP_CNTL_END_BMSK          0xffffffff
#define HWIO_MSS_OFFLINE_MEM_SLP_CNTL2_END_OFFLINE_MEM_SLP_CNTL_END_SHFT                 0x0

#define HWIO_MSS_OFFLINE_MEM_SLP_CNTL3_END_ADDR                                   (MPSS_PERPH_REG_BASE      + 0x00000064)
#define HWIO_MSS_OFFLINE_MEM_SLP_CNTL3_END_OFFS                                   (MPSS_PERPH_REG_BASE_OFFS + 0x00000064)
#define HWIO_MSS_OFFLINE_MEM_SLP_CNTL3_END_RMSK                                   0xffffffff
#define HWIO_MSS_OFFLINE_MEM_SLP_CNTL3_END_IN          \
        in_dword_masked(HWIO_MSS_OFFLINE_MEM_SLP_CNTL3_END_ADDR, HWIO_MSS_OFFLINE_MEM_SLP_CNTL3_END_RMSK)
#define HWIO_MSS_OFFLINE_MEM_SLP_CNTL3_END_INM(m)      \
        in_dword_masked(HWIO_MSS_OFFLINE_MEM_SLP_CNTL3_END_ADDR, m)
#define HWIO_MSS_OFFLINE_MEM_SLP_CNTL3_END_OFFLINE_MEM_SLP_CNTL_END_BMSK          0xffffffff
#define HWIO_MSS_OFFLINE_MEM_SLP_CNTL3_END_OFFLINE_MEM_SLP_CNTL_END_SHFT                 0x0

#define HWIO_MSS_OFFLINE_AXI_TIMEOUT_ADDR                                         (MPSS_PERPH_REG_BASE      + 0x00000068)
#define HWIO_MSS_OFFLINE_AXI_TIMEOUT_OFFS                                         (MPSS_PERPH_REG_BASE_OFFS + 0x00000068)
#define HWIO_MSS_OFFLINE_AXI_TIMEOUT_RMSK                                             0x777f
#define HWIO_MSS_OFFLINE_AXI_TIMEOUT_IN          \
        in_dword_masked(HWIO_MSS_OFFLINE_AXI_TIMEOUT_ADDR, HWIO_MSS_OFFLINE_AXI_TIMEOUT_RMSK)
#define HWIO_MSS_OFFLINE_AXI_TIMEOUT_INM(m)      \
        in_dword_masked(HWIO_MSS_OFFLINE_AXI_TIMEOUT_ADDR, m)
#define HWIO_MSS_OFFLINE_AXI_TIMEOUT_OUT(v)      \
        out_dword(HWIO_MSS_OFFLINE_AXI_TIMEOUT_ADDR,v)
#define HWIO_MSS_OFFLINE_AXI_TIMEOUT_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_OFFLINE_AXI_TIMEOUT_ADDR,m,v,HWIO_MSS_OFFLINE_AXI_TIMEOUT_IN)
#define HWIO_MSS_OFFLINE_AXI_TIMEOUT_TIMEOUT_EN_CX_BMSK                               0x7000
#define HWIO_MSS_OFFLINE_AXI_TIMEOUT_TIMEOUT_EN_CX_SHFT                                  0xc
#define HWIO_MSS_OFFLINE_AXI_TIMEOUT_RESTART_CX_BMSK                                   0x700
#define HWIO_MSS_OFFLINE_AXI_TIMEOUT_RESTART_CX_SHFT                                     0x8
#define HWIO_MSS_OFFLINE_AXI_TIMEOUT_RESTART_MDM_BMSK                                   0x7f
#define HWIO_MSS_OFFLINE_AXI_TIMEOUT_RESTART_MDM_SHFT                                    0x0

#define HWIO_MSS_QLINK_WTR_CNTRL_ADDR                                             (MPSS_PERPH_REG_BASE      + 0x0000006c)
#define HWIO_MSS_QLINK_WTR_CNTRL_OFFS                                             (MPSS_PERPH_REG_BASE_OFFS + 0x0000006c)
#define HWIO_MSS_QLINK_WTR_CNTRL_RMSK                                                    0x1
#define HWIO_MSS_QLINK_WTR_CNTRL_IN          \
        in_dword_masked(HWIO_MSS_QLINK_WTR_CNTRL_ADDR, HWIO_MSS_QLINK_WTR_CNTRL_RMSK)
#define HWIO_MSS_QLINK_WTR_CNTRL_INM(m)      \
        in_dword_masked(HWIO_MSS_QLINK_WTR_CNTRL_ADDR, m)
#define HWIO_MSS_QLINK_WTR_CNTRL_OUT(v)      \
        out_dword(HWIO_MSS_QLINK_WTR_CNTRL_ADDR,v)
#define HWIO_MSS_QLINK_WTR_CNTRL_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QLINK_WTR_CNTRL_ADDR,m,v,HWIO_MSS_QLINK_WTR_CNTRL_IN)
#define HWIO_MSS_QLINK_WTR_CNTRL_RESET_BMSK                                              0x1
#define HWIO_MSS_QLINK_WTR_CNTRL_RESET_SHFT                                              0x0

#define HWIO_MSS_QLINK_BRIDGE_IU_ENABLE_ADDR                                      (MPSS_PERPH_REG_BASE      + 0x00000070)
#define HWIO_MSS_QLINK_BRIDGE_IU_ENABLE_OFFS                                      (MPSS_PERPH_REG_BASE_OFFS + 0x00000070)
#define HWIO_MSS_QLINK_BRIDGE_IU_ENABLE_RMSK                                             0x1
#define HWIO_MSS_QLINK_BRIDGE_IU_ENABLE_IN          \
        in_dword_masked(HWIO_MSS_QLINK_BRIDGE_IU_ENABLE_ADDR, HWIO_MSS_QLINK_BRIDGE_IU_ENABLE_RMSK)
#define HWIO_MSS_QLINK_BRIDGE_IU_ENABLE_INM(m)      \
        in_dword_masked(HWIO_MSS_QLINK_BRIDGE_IU_ENABLE_ADDR, m)
#define HWIO_MSS_QLINK_BRIDGE_IU_ENABLE_OUT(v)      \
        out_dword(HWIO_MSS_QLINK_BRIDGE_IU_ENABLE_ADDR,v)
#define HWIO_MSS_QLINK_BRIDGE_IU_ENABLE_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QLINK_BRIDGE_IU_ENABLE_ADDR,m,v,HWIO_MSS_QLINK_BRIDGE_IU_ENABLE_IN)
#define HWIO_MSS_QLINK_BRIDGE_IU_ENABLE_IU_ENABLE_BMSK                                   0x1
#define HWIO_MSS_QLINK_BRIDGE_IU_ENABLE_IU_ENABLE_SHFT                                   0x0

#define HWIO_MSS_RSCC_ENABLE_ADDR                                                 (MPSS_PERPH_REG_BASE      + 0x00000074)
#define HWIO_MSS_RSCC_ENABLE_OFFS                                                 (MPSS_PERPH_REG_BASE_OFFS + 0x00000074)
#define HWIO_MSS_RSCC_ENABLE_RMSK                                                 0xffffffff
#define HWIO_MSS_RSCC_ENABLE_IN          \
        in_dword_masked(HWIO_MSS_RSCC_ENABLE_ADDR, HWIO_MSS_RSCC_ENABLE_RMSK)
#define HWIO_MSS_RSCC_ENABLE_INM(m)      \
        in_dword_masked(HWIO_MSS_RSCC_ENABLE_ADDR, m)
#define HWIO_MSS_RSCC_ENABLE_OUT(v)      \
        out_dword(HWIO_MSS_RSCC_ENABLE_ADDR,v)
#define HWIO_MSS_RSCC_ENABLE_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_RSCC_ENABLE_ADDR,m,v,HWIO_MSS_RSCC_ENABLE_IN)
#define HWIO_MSS_RSCC_ENABLE_RSCC_ENABLE_BMSK                                     0xffffffff
#define HWIO_MSS_RSCC_ENABLE_RSCC_ENABLE_SHFT                                            0x0

#define HWIO_MSS_RSCC_TOGGLE_MODE_ADDR                                            (MPSS_PERPH_REG_BASE      + 0x00000078)
#define HWIO_MSS_RSCC_TOGGLE_MODE_OFFS                                            (MPSS_PERPH_REG_BASE_OFFS + 0x00000078)
#define HWIO_MSS_RSCC_TOGGLE_MODE_RMSK                                            0xffffffff
#define HWIO_MSS_RSCC_TOGGLE_MODE_IN          \
        in_dword_masked(HWIO_MSS_RSCC_TOGGLE_MODE_ADDR, HWIO_MSS_RSCC_TOGGLE_MODE_RMSK)
#define HWIO_MSS_RSCC_TOGGLE_MODE_INM(m)      \
        in_dword_masked(HWIO_MSS_RSCC_TOGGLE_MODE_ADDR, m)
#define HWIO_MSS_RSCC_TOGGLE_MODE_OUT(v)      \
        out_dword(HWIO_MSS_RSCC_TOGGLE_MODE_ADDR,v)
#define HWIO_MSS_RSCC_TOGGLE_MODE_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_RSCC_TOGGLE_MODE_ADDR,m,v,HWIO_MSS_RSCC_TOGGLE_MODE_IN)
#define HWIO_MSS_RSCC_TOGGLE_MODE_RSCC_TOGGLE_BMSK                                0xffffffff
#define HWIO_MSS_RSCC_TOGGLE_MODE_RSCC_TOGGLE_SHFT                                       0x0

#define HWIO_MSS_RSCC_SHADOW_STATE_ADDR                                           (MPSS_PERPH_REG_BASE      + 0x0000007c)
#define HWIO_MSS_RSCC_SHADOW_STATE_OFFS                                           (MPSS_PERPH_REG_BASE_OFFS + 0x0000007c)
#define HWIO_MSS_RSCC_SHADOW_STATE_RMSK                                           0xffffffff
#define HWIO_MSS_RSCC_SHADOW_STATE_IN          \
        in_dword_masked(HWIO_MSS_RSCC_SHADOW_STATE_ADDR, HWIO_MSS_RSCC_SHADOW_STATE_RMSK)
#define HWIO_MSS_RSCC_SHADOW_STATE_INM(m)      \
        in_dword_masked(HWIO_MSS_RSCC_SHADOW_STATE_ADDR, m)
#define HWIO_MSS_RSCC_SHADOW_STATE_RSCC_SHADOW_STATE_BMSK                         0xffffffff
#define HWIO_MSS_RSCC_SHADOW_STATE_RSCC_SHADOW_STATE_SHFT                                0x0

#define HWIO_MSS_RSCC_EVENT_CONTROL_ADDR                                          (MPSS_PERPH_REG_BASE      + 0x00000080)
#define HWIO_MSS_RSCC_EVENT_CONTROL_OFFS                                          (MPSS_PERPH_REG_BASE_OFFS + 0x00000080)
#define HWIO_MSS_RSCC_EVENT_CONTROL_RMSK                                          0xffffffff
#define HWIO_MSS_RSCC_EVENT_CONTROL_IN          \
        in_dword_masked(HWIO_MSS_RSCC_EVENT_CONTROL_ADDR, HWIO_MSS_RSCC_EVENT_CONTROL_RMSK)
#define HWIO_MSS_RSCC_EVENT_CONTROL_INM(m)      \
        in_dword_masked(HWIO_MSS_RSCC_EVENT_CONTROL_ADDR, m)
#define HWIO_MSS_RSCC_EVENT_CONTROL_OUT(v)      \
        out_dword(HWIO_MSS_RSCC_EVENT_CONTROL_ADDR,v)
#define HWIO_MSS_RSCC_EVENT_CONTROL_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_RSCC_EVENT_CONTROL_ADDR,m,v,HWIO_MSS_RSCC_EVENT_CONTROL_IN)
#define HWIO_MSS_RSCC_EVENT_CONTROL_RSCC_EVENT_MASK_BMSK                          0xffff0000
#define HWIO_MSS_RSCC_EVENT_CONTROL_RSCC_EVENT_MASK_SHFT                                0x10
#define HWIO_MSS_RSCC_EVENT_CONTROL_RSCC_EVENT_VALUE_BMSK                             0xffff
#define HWIO_MSS_RSCC_EVENT_CONTROL_RSCC_EVENT_VALUE_SHFT                                0x0

#define HWIO_MSS_RSCC_MODE_CONTROL_ADDR                                           (MPSS_PERPH_REG_BASE      + 0x00000084)
#define HWIO_MSS_RSCC_MODE_CONTROL_OFFS                                           (MPSS_PERPH_REG_BASE_OFFS + 0x00000084)
#define HWIO_MSS_RSCC_MODE_CONTROL_RMSK                                                  0xf
#define HWIO_MSS_RSCC_MODE_CONTROL_IN          \
        in_dword_masked(HWIO_MSS_RSCC_MODE_CONTROL_ADDR, HWIO_MSS_RSCC_MODE_CONTROL_RMSK)
#define HWIO_MSS_RSCC_MODE_CONTROL_INM(m)      \
        in_dword_masked(HWIO_MSS_RSCC_MODE_CONTROL_ADDR, m)
#define HWIO_MSS_RSCC_MODE_CONTROL_OUT(v)      \
        out_dword(HWIO_MSS_RSCC_MODE_CONTROL_ADDR,v)
#define HWIO_MSS_RSCC_MODE_CONTROL_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_RSCC_MODE_CONTROL_ADDR,m,v,HWIO_MSS_RSCC_MODE_CONTROL_IN)
#define HWIO_MSS_RSCC_MODE_CONTROL_RSCC_MODE_BMSK                                        0xf
#define HWIO_MSS_RSCC_MODE_CONTROL_RSCC_MODE_SHFT                                        0x0

#define HWIO_MSS_RSCC_SYNC_0_ADDR                                                 (MPSS_PERPH_REG_BASE      + 0x00000088)
#define HWIO_MSS_RSCC_SYNC_0_OFFS                                                 (MPSS_PERPH_REG_BASE_OFFS + 0x00000088)
#define HWIO_MSS_RSCC_SYNC_0_RMSK                                                        0x1
#define HWIO_MSS_RSCC_SYNC_0_IN          \
        in_dword_masked(HWIO_MSS_RSCC_SYNC_0_ADDR, HWIO_MSS_RSCC_SYNC_0_RMSK)
#define HWIO_MSS_RSCC_SYNC_0_INM(m)      \
        in_dword_masked(HWIO_MSS_RSCC_SYNC_0_ADDR, m)
#define HWIO_MSS_RSCC_SYNC_0_OUT(v)      \
        out_dword(HWIO_MSS_RSCC_SYNC_0_ADDR,v)
#define HWIO_MSS_RSCC_SYNC_0_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_RSCC_SYNC_0_ADDR,m,v,HWIO_MSS_RSCC_SYNC_0_IN)
#define HWIO_MSS_RSCC_SYNC_0_RSCC_SYNC_BMSK                                              0x1
#define HWIO_MSS_RSCC_SYNC_0_RSCC_SYNC_SHFT                                              0x0

#define HWIO_MSS_RSCC_SYNC_1_ADDR                                                 (MPSS_PERPH_REG_BASE      + 0x0000008c)
#define HWIO_MSS_RSCC_SYNC_1_OFFS                                                 (MPSS_PERPH_REG_BASE_OFFS + 0x0000008c)
#define HWIO_MSS_RSCC_SYNC_1_RMSK                                                        0x1
#define HWIO_MSS_RSCC_SYNC_1_IN          \
        in_dword_masked(HWIO_MSS_RSCC_SYNC_1_ADDR, HWIO_MSS_RSCC_SYNC_1_RMSK)
#define HWIO_MSS_RSCC_SYNC_1_INM(m)      \
        in_dword_masked(HWIO_MSS_RSCC_SYNC_1_ADDR, m)
#define HWIO_MSS_RSCC_SYNC_1_OUT(v)      \
        out_dword(HWIO_MSS_RSCC_SYNC_1_ADDR,v)
#define HWIO_MSS_RSCC_SYNC_1_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_RSCC_SYNC_1_ADDR,m,v,HWIO_MSS_RSCC_SYNC_1_IN)
#define HWIO_MSS_RSCC_SYNC_1_RSCC_SYNC_BMSK                                              0x1
#define HWIO_MSS_RSCC_SYNC_1_RSCC_SYNC_SHFT                                              0x0

#define HWIO_MSS_CLK_TCM_OUT_CX_SWITCH_ADDR                                       (MPSS_PERPH_REG_BASE      + 0x00000090)
#define HWIO_MSS_CLK_TCM_OUT_CX_SWITCH_OFFS                                       (MPSS_PERPH_REG_BASE_OFFS + 0x00000090)
#define HWIO_MSS_CLK_TCM_OUT_CX_SWITCH_RMSK                                              0x1
#define HWIO_MSS_CLK_TCM_OUT_CX_SWITCH_IN          \
        in_dword_masked(HWIO_MSS_CLK_TCM_OUT_CX_SWITCH_ADDR, HWIO_MSS_CLK_TCM_OUT_CX_SWITCH_RMSK)
#define HWIO_MSS_CLK_TCM_OUT_CX_SWITCH_INM(m)      \
        in_dword_masked(HWIO_MSS_CLK_TCM_OUT_CX_SWITCH_ADDR, m)
#define HWIO_MSS_CLK_TCM_OUT_CX_SWITCH_OUT(v)      \
        out_dword(HWIO_MSS_CLK_TCM_OUT_CX_SWITCH_ADDR,v)
#define HWIO_MSS_CLK_TCM_OUT_CX_SWITCH_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_CLK_TCM_OUT_CX_SWITCH_ADDR,m,v,HWIO_MSS_CLK_TCM_OUT_CX_SWITCH_IN)
#define HWIO_MSS_CLK_TCM_OUT_CX_SWITCH_SEL_SRC_CLK_BUS_CONFIG_BMSK                       0x1
#define HWIO_MSS_CLK_TCM_OUT_CX_SWITCH_SEL_SRC_CLK_BUS_CONFIG_SHFT                       0x0

#define HWIO_MSS_RELAY_MSG_SHADOW0_ADDR                                           (MPSS_PERPH_REG_BASE      + 0x00000400)
#define HWIO_MSS_RELAY_MSG_SHADOW0_OFFS                                           (MPSS_PERPH_REG_BASE_OFFS + 0x00000400)
#define HWIO_MSS_RELAY_MSG_SHADOW0_RMSK                                           0xffffffff
#define HWIO_MSS_RELAY_MSG_SHADOW0_IN          \
        in_dword_masked(HWIO_MSS_RELAY_MSG_SHADOW0_ADDR, HWIO_MSS_RELAY_MSG_SHADOW0_RMSK)
#define HWIO_MSS_RELAY_MSG_SHADOW0_INM(m)      \
        in_dword_masked(HWIO_MSS_RELAY_MSG_SHADOW0_ADDR, m)
#define HWIO_MSS_RELAY_MSG_SHADOW0_OUT(v)      \
        out_dword(HWIO_MSS_RELAY_MSG_SHADOW0_ADDR,v)
#define HWIO_MSS_RELAY_MSG_SHADOW0_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_RELAY_MSG_SHADOW0_ADDR,m,v,HWIO_MSS_RELAY_MSG_SHADOW0_IN)
#define HWIO_MSS_RELAY_MSG_SHADOW0_RELAY_MSG_SHADOW0_BMSK                         0xffffffff
#define HWIO_MSS_RELAY_MSG_SHADOW0_RELAY_MSG_SHADOW0_SHFT                                0x0

#define HWIO_MSS_RELAY_MSG_SHADOW1_ADDR                                           (MPSS_PERPH_REG_BASE      + 0x00000404)
#define HWIO_MSS_RELAY_MSG_SHADOW1_OFFS                                           (MPSS_PERPH_REG_BASE_OFFS + 0x00000404)
#define HWIO_MSS_RELAY_MSG_SHADOW1_RMSK                                           0xffffffff
#define HWIO_MSS_RELAY_MSG_SHADOW1_IN          \
        in_dword_masked(HWIO_MSS_RELAY_MSG_SHADOW1_ADDR, HWIO_MSS_RELAY_MSG_SHADOW1_RMSK)
#define HWIO_MSS_RELAY_MSG_SHADOW1_INM(m)      \
        in_dword_masked(HWIO_MSS_RELAY_MSG_SHADOW1_ADDR, m)
#define HWIO_MSS_RELAY_MSG_SHADOW1_OUT(v)      \
        out_dword(HWIO_MSS_RELAY_MSG_SHADOW1_ADDR,v)
#define HWIO_MSS_RELAY_MSG_SHADOW1_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_RELAY_MSG_SHADOW1_ADDR,m,v,HWIO_MSS_RELAY_MSG_SHADOW1_IN)
#define HWIO_MSS_RELAY_MSG_SHADOW1_RELAY_MSG_SHADOW1_BMSK                         0xffffffff
#define HWIO_MSS_RELAY_MSG_SHADOW1_RELAY_MSG_SHADOW1_SHFT                                0x0

#define HWIO_MSS_RELAY_MSG_SHADOW2_ADDR                                           (MPSS_PERPH_REG_BASE      + 0x00000408)
#define HWIO_MSS_RELAY_MSG_SHADOW2_OFFS                                           (MPSS_PERPH_REG_BASE_OFFS + 0x00000408)
#define HWIO_MSS_RELAY_MSG_SHADOW2_RMSK                                           0xffffffff
#define HWIO_MSS_RELAY_MSG_SHADOW2_IN          \
        in_dword_masked(HWIO_MSS_RELAY_MSG_SHADOW2_ADDR, HWIO_MSS_RELAY_MSG_SHADOW2_RMSK)
#define HWIO_MSS_RELAY_MSG_SHADOW2_INM(m)      \
        in_dword_masked(HWIO_MSS_RELAY_MSG_SHADOW2_ADDR, m)
#define HWIO_MSS_RELAY_MSG_SHADOW2_OUT(v)      \
        out_dword(HWIO_MSS_RELAY_MSG_SHADOW2_ADDR,v)
#define HWIO_MSS_RELAY_MSG_SHADOW2_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_RELAY_MSG_SHADOW2_ADDR,m,v,HWIO_MSS_RELAY_MSG_SHADOW2_IN)
#define HWIO_MSS_RELAY_MSG_SHADOW2_RELAY_MSG_SHADOW2_BMSK                         0xffffffff
#define HWIO_MSS_RELAY_MSG_SHADOW2_RELAY_MSG_SHADOW2_SHFT                                0x0

#define HWIO_MSS_RELAY_MSG_SHADOW3_ADDR                                           (MPSS_PERPH_REG_BASE      + 0x0000040c)
#define HWIO_MSS_RELAY_MSG_SHADOW3_OFFS                                           (MPSS_PERPH_REG_BASE_OFFS + 0x0000040c)
#define HWIO_MSS_RELAY_MSG_SHADOW3_RMSK                                           0xffffffff
#define HWIO_MSS_RELAY_MSG_SHADOW3_IN          \
        in_dword_masked(HWIO_MSS_RELAY_MSG_SHADOW3_ADDR, HWIO_MSS_RELAY_MSG_SHADOW3_RMSK)
#define HWIO_MSS_RELAY_MSG_SHADOW3_INM(m)      \
        in_dword_masked(HWIO_MSS_RELAY_MSG_SHADOW3_ADDR, m)
#define HWIO_MSS_RELAY_MSG_SHADOW3_OUT(v)      \
        out_dword(HWIO_MSS_RELAY_MSG_SHADOW3_ADDR,v)
#define HWIO_MSS_RELAY_MSG_SHADOW3_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_RELAY_MSG_SHADOW3_ADDR,m,v,HWIO_MSS_RELAY_MSG_SHADOW3_IN)
#define HWIO_MSS_RELAY_MSG_SHADOW3_RELAY_MSG_SHADOW3_BMSK                         0xffffffff
#define HWIO_MSS_RELAY_MSG_SHADOW3_RELAY_MSG_SHADOW3_SHFT                                0x0

#define HWIO_MSS_RELAY_MSG_SHADOW4_ADDR                                           (MPSS_PERPH_REG_BASE      + 0x00000410)
#define HWIO_MSS_RELAY_MSG_SHADOW4_OFFS                                           (MPSS_PERPH_REG_BASE_OFFS + 0x00000410)
#define HWIO_MSS_RELAY_MSG_SHADOW4_RMSK                                           0xffffffff
#define HWIO_MSS_RELAY_MSG_SHADOW4_IN          \
        in_dword_masked(HWIO_MSS_RELAY_MSG_SHADOW4_ADDR, HWIO_MSS_RELAY_MSG_SHADOW4_RMSK)
#define HWIO_MSS_RELAY_MSG_SHADOW4_INM(m)      \
        in_dword_masked(HWIO_MSS_RELAY_MSG_SHADOW4_ADDR, m)
#define HWIO_MSS_RELAY_MSG_SHADOW4_OUT(v)      \
        out_dword(HWIO_MSS_RELAY_MSG_SHADOW4_ADDR,v)
#define HWIO_MSS_RELAY_MSG_SHADOW4_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_RELAY_MSG_SHADOW4_ADDR,m,v,HWIO_MSS_RELAY_MSG_SHADOW4_IN)
#define HWIO_MSS_RELAY_MSG_SHADOW4_RELAY_MSG_SHADOW4_BMSK                         0xffffffff
#define HWIO_MSS_RELAY_MSG_SHADOW4_RELAY_MSG_SHADOW4_SHFT                                0x0

#define HWIO_MSS_RELAY_MSG_SHADOW5_ADDR                                           (MPSS_PERPH_REG_BASE      + 0x00000414)
#define HWIO_MSS_RELAY_MSG_SHADOW5_OFFS                                           (MPSS_PERPH_REG_BASE_OFFS + 0x00000414)
#define HWIO_MSS_RELAY_MSG_SHADOW5_RMSK                                           0xffffffff
#define HWIO_MSS_RELAY_MSG_SHADOW5_IN          \
        in_dword_masked(HWIO_MSS_RELAY_MSG_SHADOW5_ADDR, HWIO_MSS_RELAY_MSG_SHADOW5_RMSK)
#define HWIO_MSS_RELAY_MSG_SHADOW5_INM(m)      \
        in_dword_masked(HWIO_MSS_RELAY_MSG_SHADOW5_ADDR, m)
#define HWIO_MSS_RELAY_MSG_SHADOW5_OUT(v)      \
        out_dword(HWIO_MSS_RELAY_MSG_SHADOW5_ADDR,v)
#define HWIO_MSS_RELAY_MSG_SHADOW5_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_RELAY_MSG_SHADOW5_ADDR,m,v,HWIO_MSS_RELAY_MSG_SHADOW5_IN)
#define HWIO_MSS_RELAY_MSG_SHADOW5_RELAY_MSG_SHADOW5_BMSK                         0xffffffff
#define HWIO_MSS_RELAY_MSG_SHADOW5_RELAY_MSG_SHADOW5_SHFT                                0x0

#define HWIO_MSS_RELAY_MSG_SHADOW6_ADDR                                           (MPSS_PERPH_REG_BASE      + 0x00000418)
#define HWIO_MSS_RELAY_MSG_SHADOW6_OFFS                                           (MPSS_PERPH_REG_BASE_OFFS + 0x00000418)
#define HWIO_MSS_RELAY_MSG_SHADOW6_RMSK                                           0xffffffff
#define HWIO_MSS_RELAY_MSG_SHADOW6_IN          \
        in_dword_masked(HWIO_MSS_RELAY_MSG_SHADOW6_ADDR, HWIO_MSS_RELAY_MSG_SHADOW6_RMSK)
#define HWIO_MSS_RELAY_MSG_SHADOW6_INM(m)      \
        in_dword_masked(HWIO_MSS_RELAY_MSG_SHADOW6_ADDR, m)
#define HWIO_MSS_RELAY_MSG_SHADOW6_OUT(v)      \
        out_dword(HWIO_MSS_RELAY_MSG_SHADOW6_ADDR,v)
#define HWIO_MSS_RELAY_MSG_SHADOW6_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_RELAY_MSG_SHADOW6_ADDR,m,v,HWIO_MSS_RELAY_MSG_SHADOW6_IN)
#define HWIO_MSS_RELAY_MSG_SHADOW6_RELAY_MSG_SHADOW6_BMSK                         0xffffffff
#define HWIO_MSS_RELAY_MSG_SHADOW6_RELAY_MSG_SHADOW6_SHFT                                0x0

#define HWIO_MSS_RELAY_MSG_SHADOW7_ADDR                                           (MPSS_PERPH_REG_BASE      + 0x0000041c)
#define HWIO_MSS_RELAY_MSG_SHADOW7_OFFS                                           (MPSS_PERPH_REG_BASE_OFFS + 0x0000041c)
#define HWIO_MSS_RELAY_MSG_SHADOW7_RMSK                                           0xffffffff
#define HWIO_MSS_RELAY_MSG_SHADOW7_IN          \
        in_dword_masked(HWIO_MSS_RELAY_MSG_SHADOW7_ADDR, HWIO_MSS_RELAY_MSG_SHADOW7_RMSK)
#define HWIO_MSS_RELAY_MSG_SHADOW7_INM(m)      \
        in_dword_masked(HWIO_MSS_RELAY_MSG_SHADOW7_ADDR, m)
#define HWIO_MSS_RELAY_MSG_SHADOW7_OUT(v)      \
        out_dword(HWIO_MSS_RELAY_MSG_SHADOW7_ADDR,v)
#define HWIO_MSS_RELAY_MSG_SHADOW7_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_RELAY_MSG_SHADOW7_ADDR,m,v,HWIO_MSS_RELAY_MSG_SHADOW7_IN)
#define HWIO_MSS_RELAY_MSG_SHADOW7_RELAY_MSG_SHADOW7_BMSK                         0xffffffff
#define HWIO_MSS_RELAY_MSG_SHADOW7_RELAY_MSG_SHADOW7_SHFT                                0x0

#define HWIO_MSS_RELAY_MSG_SHADOW8_ADDR                                           (MPSS_PERPH_REG_BASE      + 0x00000420)
#define HWIO_MSS_RELAY_MSG_SHADOW8_OFFS                                           (MPSS_PERPH_REG_BASE_OFFS + 0x00000420)
#define HWIO_MSS_RELAY_MSG_SHADOW8_RMSK                                           0xffffffff
#define HWIO_MSS_RELAY_MSG_SHADOW8_IN          \
        in_dword_masked(HWIO_MSS_RELAY_MSG_SHADOW8_ADDR, HWIO_MSS_RELAY_MSG_SHADOW8_RMSK)
#define HWIO_MSS_RELAY_MSG_SHADOW8_INM(m)      \
        in_dword_masked(HWIO_MSS_RELAY_MSG_SHADOW8_ADDR, m)
#define HWIO_MSS_RELAY_MSG_SHADOW8_OUT(v)      \
        out_dword(HWIO_MSS_RELAY_MSG_SHADOW8_ADDR,v)
#define HWIO_MSS_RELAY_MSG_SHADOW8_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_RELAY_MSG_SHADOW8_ADDR,m,v,HWIO_MSS_RELAY_MSG_SHADOW8_IN)
#define HWIO_MSS_RELAY_MSG_SHADOW8_RELAY_MSG_SHADOW8_BMSK                         0xffffffff
#define HWIO_MSS_RELAY_MSG_SHADOW8_RELAY_MSG_SHADOW8_SHFT                                0x0

#define HWIO_MSS_RELAY_MSG_SHADOW9_ADDR                                           (MPSS_PERPH_REG_BASE      + 0x00000424)
#define HWIO_MSS_RELAY_MSG_SHADOW9_OFFS                                           (MPSS_PERPH_REG_BASE_OFFS + 0x00000424)
#define HWIO_MSS_RELAY_MSG_SHADOW9_RMSK                                           0xffffffff
#define HWIO_MSS_RELAY_MSG_SHADOW9_IN          \
        in_dword_masked(HWIO_MSS_RELAY_MSG_SHADOW9_ADDR, HWIO_MSS_RELAY_MSG_SHADOW9_RMSK)
#define HWIO_MSS_RELAY_MSG_SHADOW9_INM(m)      \
        in_dword_masked(HWIO_MSS_RELAY_MSG_SHADOW9_ADDR, m)
#define HWIO_MSS_RELAY_MSG_SHADOW9_OUT(v)      \
        out_dword(HWIO_MSS_RELAY_MSG_SHADOW9_ADDR,v)
#define HWIO_MSS_RELAY_MSG_SHADOW9_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_RELAY_MSG_SHADOW9_ADDR,m,v,HWIO_MSS_RELAY_MSG_SHADOW9_IN)
#define HWIO_MSS_RELAY_MSG_SHADOW9_RELAY_MSG_SHADOW9_BMSK                         0xffffffff
#define HWIO_MSS_RELAY_MSG_SHADOW9_RELAY_MSG_SHADOW9_SHFT                                0x0

#define HWIO_MSS_RELAY_MSG_SHADOW10_ADDR                                          (MPSS_PERPH_REG_BASE      + 0x00000428)
#define HWIO_MSS_RELAY_MSG_SHADOW10_OFFS                                          (MPSS_PERPH_REG_BASE_OFFS + 0x00000428)
#define HWIO_MSS_RELAY_MSG_SHADOW10_RMSK                                          0xffffffff
#define HWIO_MSS_RELAY_MSG_SHADOW10_IN          \
        in_dword_masked(HWIO_MSS_RELAY_MSG_SHADOW10_ADDR, HWIO_MSS_RELAY_MSG_SHADOW10_RMSK)
#define HWIO_MSS_RELAY_MSG_SHADOW10_INM(m)      \
        in_dword_masked(HWIO_MSS_RELAY_MSG_SHADOW10_ADDR, m)
#define HWIO_MSS_RELAY_MSG_SHADOW10_OUT(v)      \
        out_dword(HWIO_MSS_RELAY_MSG_SHADOW10_ADDR,v)
#define HWIO_MSS_RELAY_MSG_SHADOW10_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_RELAY_MSG_SHADOW10_ADDR,m,v,HWIO_MSS_RELAY_MSG_SHADOW10_IN)
#define HWIO_MSS_RELAY_MSG_SHADOW10_RELAY_MSG_SHADOW10_BMSK                       0xffffffff
#define HWIO_MSS_RELAY_MSG_SHADOW10_RELAY_MSG_SHADOW10_SHFT                              0x0

#define HWIO_MSS_RELAY_MSG_SHADOW11_ADDR                                          (MPSS_PERPH_REG_BASE      + 0x0000042c)
#define HWIO_MSS_RELAY_MSG_SHADOW11_OFFS                                          (MPSS_PERPH_REG_BASE_OFFS + 0x0000042c)
#define HWIO_MSS_RELAY_MSG_SHADOW11_RMSK                                          0xffffffff
#define HWIO_MSS_RELAY_MSG_SHADOW11_IN          \
        in_dword_masked(HWIO_MSS_RELAY_MSG_SHADOW11_ADDR, HWIO_MSS_RELAY_MSG_SHADOW11_RMSK)
#define HWIO_MSS_RELAY_MSG_SHADOW11_INM(m)      \
        in_dword_masked(HWIO_MSS_RELAY_MSG_SHADOW11_ADDR, m)
#define HWIO_MSS_RELAY_MSG_SHADOW11_OUT(v)      \
        out_dword(HWIO_MSS_RELAY_MSG_SHADOW11_ADDR,v)
#define HWIO_MSS_RELAY_MSG_SHADOW11_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_RELAY_MSG_SHADOW11_ADDR,m,v,HWIO_MSS_RELAY_MSG_SHADOW11_IN)
#define HWIO_MSS_RELAY_MSG_SHADOW11_RELAY_MSG_SHADOW11_BMSK                       0xffffffff
#define HWIO_MSS_RELAY_MSG_SHADOW11_RELAY_MSG_SHADOW11_SHFT                              0x0

#define HWIO_MSS_RELAY_MSG_SHADOW12_ADDR                                          (MPSS_PERPH_REG_BASE      + 0x00000430)
#define HWIO_MSS_RELAY_MSG_SHADOW12_OFFS                                          (MPSS_PERPH_REG_BASE_OFFS + 0x00000430)
#define HWIO_MSS_RELAY_MSG_SHADOW12_RMSK                                          0xffffffff
#define HWIO_MSS_RELAY_MSG_SHADOW12_IN          \
        in_dword_masked(HWIO_MSS_RELAY_MSG_SHADOW12_ADDR, HWIO_MSS_RELAY_MSG_SHADOW12_RMSK)
#define HWIO_MSS_RELAY_MSG_SHADOW12_INM(m)      \
        in_dword_masked(HWIO_MSS_RELAY_MSG_SHADOW12_ADDR, m)
#define HWIO_MSS_RELAY_MSG_SHADOW12_OUT(v)      \
        out_dword(HWIO_MSS_RELAY_MSG_SHADOW12_ADDR,v)
#define HWIO_MSS_RELAY_MSG_SHADOW12_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_RELAY_MSG_SHADOW12_ADDR,m,v,HWIO_MSS_RELAY_MSG_SHADOW12_IN)
#define HWIO_MSS_RELAY_MSG_SHADOW12_RELAY_MSG_SHADOW12_BMSK                       0xffffffff
#define HWIO_MSS_RELAY_MSG_SHADOW12_RELAY_MSG_SHADOW12_SHFT                              0x0

#define HWIO_MSS_RELAY_MSG_SHADOW13_ADDR                                          (MPSS_PERPH_REG_BASE      + 0x00000434)
#define HWIO_MSS_RELAY_MSG_SHADOW13_OFFS                                          (MPSS_PERPH_REG_BASE_OFFS + 0x00000434)
#define HWIO_MSS_RELAY_MSG_SHADOW13_RMSK                                          0xffffffff
#define HWIO_MSS_RELAY_MSG_SHADOW13_IN          \
        in_dword_masked(HWIO_MSS_RELAY_MSG_SHADOW13_ADDR, HWIO_MSS_RELAY_MSG_SHADOW13_RMSK)
#define HWIO_MSS_RELAY_MSG_SHADOW13_INM(m)      \
        in_dword_masked(HWIO_MSS_RELAY_MSG_SHADOW13_ADDR, m)
#define HWIO_MSS_RELAY_MSG_SHADOW13_OUT(v)      \
        out_dword(HWIO_MSS_RELAY_MSG_SHADOW13_ADDR,v)
#define HWIO_MSS_RELAY_MSG_SHADOW13_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_RELAY_MSG_SHADOW13_ADDR,m,v,HWIO_MSS_RELAY_MSG_SHADOW13_IN)
#define HWIO_MSS_RELAY_MSG_SHADOW13_RELAY_MSG_SHADOW13_BMSK                       0xffffffff
#define HWIO_MSS_RELAY_MSG_SHADOW13_RELAY_MSG_SHADOW13_SHFT                              0x0

#define HWIO_MSS_RELAY_MSG_SHADOW14_ADDR                                          (MPSS_PERPH_REG_BASE      + 0x00000438)
#define HWIO_MSS_RELAY_MSG_SHADOW14_OFFS                                          (MPSS_PERPH_REG_BASE_OFFS + 0x00000438)
#define HWIO_MSS_RELAY_MSG_SHADOW14_RMSK                                          0xffffffff
#define HWIO_MSS_RELAY_MSG_SHADOW14_IN          \
        in_dword_masked(HWIO_MSS_RELAY_MSG_SHADOW14_ADDR, HWIO_MSS_RELAY_MSG_SHADOW14_RMSK)
#define HWIO_MSS_RELAY_MSG_SHADOW14_INM(m)      \
        in_dword_masked(HWIO_MSS_RELAY_MSG_SHADOW14_ADDR, m)
#define HWIO_MSS_RELAY_MSG_SHADOW14_OUT(v)      \
        out_dword(HWIO_MSS_RELAY_MSG_SHADOW14_ADDR,v)
#define HWIO_MSS_RELAY_MSG_SHADOW14_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_RELAY_MSG_SHADOW14_ADDR,m,v,HWIO_MSS_RELAY_MSG_SHADOW14_IN)
#define HWIO_MSS_RELAY_MSG_SHADOW14_RELAY_MSG_SHADOW14_BMSK                       0xffffffff
#define HWIO_MSS_RELAY_MSG_SHADOW14_RELAY_MSG_SHADOW14_SHFT                              0x0

#define HWIO_MSS_RELAY_MSG_SHADOW15_ADDR                                          (MPSS_PERPH_REG_BASE      + 0x0000043c)
#define HWIO_MSS_RELAY_MSG_SHADOW15_OFFS                                          (MPSS_PERPH_REG_BASE_OFFS + 0x0000043c)
#define HWIO_MSS_RELAY_MSG_SHADOW15_RMSK                                          0xffffffff
#define HWIO_MSS_RELAY_MSG_SHADOW15_IN          \
        in_dword_masked(HWIO_MSS_RELAY_MSG_SHADOW15_ADDR, HWIO_MSS_RELAY_MSG_SHADOW15_RMSK)
#define HWIO_MSS_RELAY_MSG_SHADOW15_INM(m)      \
        in_dword_masked(HWIO_MSS_RELAY_MSG_SHADOW15_ADDR, m)
#define HWIO_MSS_RELAY_MSG_SHADOW15_OUT(v)      \
        out_dword(HWIO_MSS_RELAY_MSG_SHADOW15_ADDR,v)
#define HWIO_MSS_RELAY_MSG_SHADOW15_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_RELAY_MSG_SHADOW15_ADDR,m,v,HWIO_MSS_RELAY_MSG_SHADOW15_IN)
#define HWIO_MSS_RELAY_MSG_SHADOW15_RELAY_MSG_SHADOW15_BMSK                       0xffffffff
#define HWIO_MSS_RELAY_MSG_SHADOW15_RELAY_MSG_SHADOW15_SHFT                              0x0

/*----------------------------------------------------------------------------
 * MODULE: MSS_MSS_CC_REG
 *--------------------------------------------------------------------------*/

#define MSS_MSS_CC_REG_REG_BASE                                                     (MODEM_TOP_BASE      + 0x001ae000)
#define MSS_MSS_CC_REG_REG_BASE_OFFS                                                0x001ae000

#define HWIO_MSS_OFFLINE_PLL_MODE_ADDR                                              (MSS_MSS_CC_REG_REG_BASE      + 0x00000000)
#define HWIO_MSS_OFFLINE_PLL_MODE_OFFS                                              (MSS_MSS_CC_REG_REG_BASE_OFFS + 0x00000000)
#define HWIO_MSS_OFFLINE_PLL_MODE_RMSK                                              0xffffffff
#define HWIO_MSS_OFFLINE_PLL_MODE_IN          \
        in_dword_masked(HWIO_MSS_OFFLINE_PLL_MODE_ADDR, HWIO_MSS_OFFLINE_PLL_MODE_RMSK)
#define HWIO_MSS_OFFLINE_PLL_MODE_INM(m)      \
        in_dword_masked(HWIO_MSS_OFFLINE_PLL_MODE_ADDR, m)
#define HWIO_MSS_OFFLINE_PLL_MODE_OUT(v)      \
        out_dword(HWIO_MSS_OFFLINE_PLL_MODE_ADDR,v)
#define HWIO_MSS_OFFLINE_PLL_MODE_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_OFFLINE_PLL_MODE_ADDR,m,v,HWIO_MSS_OFFLINE_PLL_MODE_IN)
#define HWIO_MSS_OFFLINE_PLL_MODE_PLL_LOCK_DET_BMSK                                 0x80000000
#define HWIO_MSS_OFFLINE_PLL_MODE_PLL_LOCK_DET_SHFT                                       0x1f
#define HWIO_MSS_OFFLINE_PLL_MODE_PLL_ACTIVE_FLAG_BMSK                              0x40000000
#define HWIO_MSS_OFFLINE_PLL_MODE_PLL_ACTIVE_FLAG_SHFT                                    0x1e
#define HWIO_MSS_OFFLINE_PLL_MODE_PLL_ACK_LATCH_BMSK                                0x20000000
#define HWIO_MSS_OFFLINE_PLL_MODE_PLL_ACK_LATCH_SHFT                                      0x1d
#define HWIO_MSS_OFFLINE_PLL_MODE_PLL_LOCK_DET_FINE_BMSK                            0x10000000
#define HWIO_MSS_OFFLINE_PLL_MODE_PLL_LOCK_DET_FINE_SHFT                                  0x1c
#define HWIO_MSS_OFFLINE_PLL_MODE_FSM_LEGACY_MODE_BMSK                               0x1000000
#define HWIO_MSS_OFFLINE_PLL_MODE_FSM_LEGACY_MODE_SHFT                                    0x18
#define HWIO_MSS_OFFLINE_PLL_MODE_PLL_HW_UPDATE_LOGIC_BYPASS_BMSK                     0x800000
#define HWIO_MSS_OFFLINE_PLL_MODE_PLL_HW_UPDATE_LOGIC_BYPASS_SHFT                         0x17
#define HWIO_MSS_OFFLINE_PLL_MODE_PLL_UPDATE_BMSK                                     0x400000
#define HWIO_MSS_OFFLINE_PLL_MODE_PLL_UPDATE_SHFT                                         0x16
#define HWIO_MSS_OFFLINE_PLL_MODE_PLL_VOTE_FSM_RESET_BMSK                             0x200000
#define HWIO_MSS_OFFLINE_PLL_MODE_PLL_VOTE_FSM_RESET_SHFT                                 0x15
#define HWIO_MSS_OFFLINE_PLL_MODE_PLL_VOTE_FSM_ENA_BMSK                               0x100000
#define HWIO_MSS_OFFLINE_PLL_MODE_PLL_VOTE_FSM_ENA_SHFT                                   0x14
#define HWIO_MSS_OFFLINE_PLL_MODE_PLL_BIAS_COUNT_BMSK                                  0xfc000
#define HWIO_MSS_OFFLINE_PLL_MODE_PLL_BIAS_COUNT_SHFT                                      0xe
#define HWIO_MSS_OFFLINE_PLL_MODE_PLL_LOCK_COUNT_BMSK                                   0x3f00
#define HWIO_MSS_OFFLINE_PLL_MODE_PLL_LOCK_COUNT_SHFT                                      0x8
#define HWIO_MSS_OFFLINE_PLL_MODE_OUT_TEST_HW_CTL_BMSK                                    0x40
#define HWIO_MSS_OFFLINE_PLL_MODE_OUT_TEST_HW_CTL_SHFT                                     0x6
#define HWIO_MSS_OFFLINE_PLL_MODE_OUT_ODD_HW_CTL_BMSK                                     0x20
#define HWIO_MSS_OFFLINE_PLL_MODE_OUT_ODD_HW_CTL_SHFT                                      0x5
#define HWIO_MSS_OFFLINE_PLL_MODE_OUT_EVEN_HW_CTL_BMSK                                    0x10
#define HWIO_MSS_OFFLINE_PLL_MODE_OUT_EVEN_HW_CTL_SHFT                                     0x4
#define HWIO_MSS_OFFLINE_PLL_MODE_OUT_MAIN_HW_CTL_BMSK                                     0x8
#define HWIO_MSS_OFFLINE_PLL_MODE_OUT_MAIN_HW_CTL_SHFT                                     0x3
#define HWIO_MSS_OFFLINE_PLL_MODE_PLL_RESET_N_BMSK                                         0x4
#define HWIO_MSS_OFFLINE_PLL_MODE_PLL_RESET_N_SHFT                                         0x2
#define HWIO_MSS_OFFLINE_PLL_MODE_PLL_OUTCTRL_BMSK                                         0x1
#define HWIO_MSS_OFFLINE_PLL_MODE_PLL_OUTCTRL_SHFT                                         0x0

#define HWIO_MSS_OFFLINE_PLL_L_VAL_ADDR                                             (MSS_MSS_CC_REG_REG_BASE      + 0x00000004)
#define HWIO_MSS_OFFLINE_PLL_L_VAL_OFFS                                             (MSS_MSS_CC_REG_REG_BASE_OFFS + 0x00000004)
#define HWIO_MSS_OFFLINE_PLL_L_VAL_RMSK                                                 0xffff
#define HWIO_MSS_OFFLINE_PLL_L_VAL_IN          \
        in_dword_masked(HWIO_MSS_OFFLINE_PLL_L_VAL_ADDR, HWIO_MSS_OFFLINE_PLL_L_VAL_RMSK)
#define HWIO_MSS_OFFLINE_PLL_L_VAL_INM(m)      \
        in_dword_masked(HWIO_MSS_OFFLINE_PLL_L_VAL_ADDR, m)
#define HWIO_MSS_OFFLINE_PLL_L_VAL_OUT(v)      \
        out_dword(HWIO_MSS_OFFLINE_PLL_L_VAL_ADDR,v)
#define HWIO_MSS_OFFLINE_PLL_L_VAL_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_OFFLINE_PLL_L_VAL_ADDR,m,v,HWIO_MSS_OFFLINE_PLL_L_VAL_IN)
#define HWIO_MSS_OFFLINE_PLL_L_VAL_PLL_L_BMSK                                           0xffff
#define HWIO_MSS_OFFLINE_PLL_L_VAL_PLL_L_SHFT                                              0x0

#define HWIO_MSS_OFFLINE_PLL_CAL_L_VAL_ADDR                                         (MSS_MSS_CC_REG_REG_BASE      + 0x00000008)
#define HWIO_MSS_OFFLINE_PLL_CAL_L_VAL_OFFS                                         (MSS_MSS_CC_REG_REG_BASE_OFFS + 0x00000008)
#define HWIO_MSS_OFFLINE_PLL_CAL_L_VAL_RMSK                                             0xffff
#define HWIO_MSS_OFFLINE_PLL_CAL_L_VAL_IN          \
        in_dword_masked(HWIO_MSS_OFFLINE_PLL_CAL_L_VAL_ADDR, HWIO_MSS_OFFLINE_PLL_CAL_L_VAL_RMSK)
#define HWIO_MSS_OFFLINE_PLL_CAL_L_VAL_INM(m)      \
        in_dword_masked(HWIO_MSS_OFFLINE_PLL_CAL_L_VAL_ADDR, m)
#define HWIO_MSS_OFFLINE_PLL_CAL_L_VAL_OUT(v)      \
        out_dword(HWIO_MSS_OFFLINE_PLL_CAL_L_VAL_ADDR,v)
#define HWIO_MSS_OFFLINE_PLL_CAL_L_VAL_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_OFFLINE_PLL_CAL_L_VAL_ADDR,m,v,HWIO_MSS_OFFLINE_PLL_CAL_L_VAL_IN)
#define HWIO_MSS_OFFLINE_PLL_CAL_L_VAL_PLL_CAL_L_BMSK                                   0xffff
#define HWIO_MSS_OFFLINE_PLL_CAL_L_VAL_PLL_CAL_L_SHFT                                      0x0

#define HWIO_MSS_OFFLINE_PLL_USER_CTL_ADDR                                          (MSS_MSS_CC_REG_REG_BASE      + 0x0000000c)
#define HWIO_MSS_OFFLINE_PLL_USER_CTL_OFFS                                          (MSS_MSS_CC_REG_REG_BASE_OFFS + 0x0000000c)
#define HWIO_MSS_OFFLINE_PLL_USER_CTL_RMSK                                          0xffffffff
#define HWIO_MSS_OFFLINE_PLL_USER_CTL_IN          \
        in_dword_masked(HWIO_MSS_OFFLINE_PLL_USER_CTL_ADDR, HWIO_MSS_OFFLINE_PLL_USER_CTL_RMSK)
#define HWIO_MSS_OFFLINE_PLL_USER_CTL_INM(m)      \
        in_dword_masked(HWIO_MSS_OFFLINE_PLL_USER_CTL_ADDR, m)
#define HWIO_MSS_OFFLINE_PLL_USER_CTL_OUT(v)      \
        out_dword(HWIO_MSS_OFFLINE_PLL_USER_CTL_ADDR,v)
#define HWIO_MSS_OFFLINE_PLL_USER_CTL_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_OFFLINE_PLL_USER_CTL_ADDR,m,v,HWIO_MSS_OFFLINE_PLL_USER_CTL_IN)
#define HWIO_MSS_OFFLINE_PLL_USER_CTL_PRE_DIV_RATIO_BMSK                               0x70000
#define HWIO_MSS_OFFLINE_PLL_USER_CTL_PRE_DIV_RATIO_SHFT                                  0x10
#define HWIO_MSS_OFFLINE_PLL_USER_CTL_POST_DIV_RATIO_ODD_BMSK                           0xf000
#define HWIO_MSS_OFFLINE_PLL_USER_CTL_POST_DIV_RATIO_ODD_SHFT                              0xc
#define HWIO_MSS_OFFLINE_PLL_USER_CTL_POST_DIV_RATIO_EVEN_BMSK                           0xf00
#define HWIO_MSS_OFFLINE_PLL_USER_CTL_POST_DIV_RATIO_EVEN_SHFT                             0x8
#define HWIO_MSS_OFFLINE_PLL_USER_CTL_OUT_CLK_POLARITY_BMSK                               0x80
#define HWIO_MSS_OFFLINE_PLL_USER_CTL_OUT_CLK_POLARITY_SHFT                                0x7
#define HWIO_MSS_OFFLINE_PLL_USER_CTL_PLLOUT_TEST_BMSK                                    0x10
#define HWIO_MSS_OFFLINE_PLL_USER_CTL_PLLOUT_TEST_SHFT                                     0x4
#define HWIO_MSS_OFFLINE_PLL_USER_CTL_PLLOUT_ODD_BMSK                                      0x4
#define HWIO_MSS_OFFLINE_PLL_USER_CTL_PLLOUT_ODD_SHFT                                      0x2
#define HWIO_MSS_OFFLINE_PLL_USER_CTL_PLLOUT_EVEN_BMSK                                     0x2
#define HWIO_MSS_OFFLINE_PLL_USER_CTL_PLLOUT_EVEN_SHFT                                     0x1
#define HWIO_MSS_OFFLINE_PLL_USER_CTL_PLLOUT_MAIN_BMSK                                     0x1
#define HWIO_MSS_OFFLINE_PLL_USER_CTL_PLLOUT_MAIN_SHFT                                     0x0

#define HWIO_MSS_OFFLINE_PLL_USER_CTL_U_ADDR                                        (MSS_MSS_CC_REG_REG_BASE      + 0x00000010)
#define HWIO_MSS_OFFLINE_PLL_USER_CTL_U_OFFS                                        (MSS_MSS_CC_REG_REG_BASE_OFFS + 0x00000010)
#define HWIO_MSS_OFFLINE_PLL_USER_CTL_U_RMSK                                        0xffffffff
#define HWIO_MSS_OFFLINE_PLL_USER_CTL_U_IN          \
        in_dword_masked(HWIO_MSS_OFFLINE_PLL_USER_CTL_U_ADDR, HWIO_MSS_OFFLINE_PLL_USER_CTL_U_RMSK)
#define HWIO_MSS_OFFLINE_PLL_USER_CTL_U_INM(m)      \
        in_dword_masked(HWIO_MSS_OFFLINE_PLL_USER_CTL_U_ADDR, m)
#define HWIO_MSS_OFFLINE_PLL_USER_CTL_U_OUT(v)      \
        out_dword(HWIO_MSS_OFFLINE_PLL_USER_CTL_U_ADDR,v)
#define HWIO_MSS_OFFLINE_PLL_USER_CTL_U_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_OFFLINE_PLL_USER_CTL_U_ADDR,m,v,HWIO_MSS_OFFLINE_PLL_USER_CTL_U_IN)
#define HWIO_MSS_OFFLINE_PLL_USER_CTL_U_USE_PREDIV_WHEN_CAL_BMSK                       0x40000
#define HWIO_MSS_OFFLINE_PLL_USER_CTL_U_USE_PREDIV_WHEN_CAL_SHFT                          0x12
#define HWIO_MSS_OFFLINE_PLL_USER_CTL_U_USE_EXT_SAVERESTORE_BMSK                       0x20000
#define HWIO_MSS_OFFLINE_PLL_USER_CTL_U_USE_EXT_SAVERESTORE_SHFT                          0x11
#define HWIO_MSS_OFFLINE_PLL_USER_CTL_U_FRAC_FORMAT_SEL_BMSK                            0x8000
#define HWIO_MSS_OFFLINE_PLL_USER_CTL_U_FRAC_FORMAT_SEL_SHFT                               0xf
#define HWIO_MSS_OFFLINE_PLL_USER_CTL_U_BIAS_ON_IN_STANDBY_BMSK                         0x4000
#define HWIO_MSS_OFFLINE_PLL_USER_CTL_U_BIAS_ON_IN_STANDBY_SHFT                            0xe
#define HWIO_MSS_OFFLINE_PLL_USER_CTL_U_REF_CLK_AT_OUT_BMSK                             0x2000
#define HWIO_MSS_OFFLINE_PLL_USER_CTL_U_REF_CLK_AT_OUT_SHFT                                0xd
#define HWIO_MSS_OFFLINE_PLL_USER_CTL_U_SCALE_FREQ_ON_RESTART_BMSK                      0x1800
#define HWIO_MSS_OFFLINE_PLL_USER_CTL_U_SCALE_FREQ_ON_RESTART_SHFT                         0xb
#define HWIO_MSS_OFFLINE_PLL_USER_CTL_U_LATCH_INTERFACE_BYPASS_BMSK                      0x400
#define HWIO_MSS_OFFLINE_PLL_USER_CTL_U_LATCH_INTERFACE_BYPASS_SHFT                        0xa
#define HWIO_MSS_OFFLINE_PLL_USER_CTL_U_STATUS_REGISTER_BMSK                             0x3e0
#define HWIO_MSS_OFFLINE_PLL_USER_CTL_U_STATUS_REGISTER_SHFT                               0x5
#define HWIO_MSS_OFFLINE_PLL_USER_CTL_U_WRITE_STATE_EN_BMSK                               0x10
#define HWIO_MSS_OFFLINE_PLL_USER_CTL_U_WRITE_STATE_EN_SHFT                                0x4
#define HWIO_MSS_OFFLINE_PLL_USER_CTL_U_CALIB_CTRL_BMSK                                    0xe
#define HWIO_MSS_OFFLINE_PLL_USER_CTL_U_CALIB_CTRL_SHFT                                    0x1
#define HWIO_MSS_OFFLINE_PLL_USER_CTL_U_USE_ONLY_FINE_LOCK_DET_BMSK                        0x1
#define HWIO_MSS_OFFLINE_PLL_USER_CTL_U_USE_ONLY_FINE_LOCK_DET_SHFT                        0x0

#define HWIO_MSS_OFFLINE_PLL_USER_CTL_U1_ADDR                                       (MSS_MSS_CC_REG_REG_BASE      + 0x00000014)
#define HWIO_MSS_OFFLINE_PLL_USER_CTL_U1_OFFS                                       (MSS_MSS_CC_REG_REG_BASE_OFFS + 0x00000014)
#define HWIO_MSS_OFFLINE_PLL_USER_CTL_U1_RMSK                                       0xffffffff
#define HWIO_MSS_OFFLINE_PLL_USER_CTL_U1_IN          \
        in_dword_masked(HWIO_MSS_OFFLINE_PLL_USER_CTL_U1_ADDR, HWIO_MSS_OFFLINE_PLL_USER_CTL_U1_RMSK)
#define HWIO_MSS_OFFLINE_PLL_USER_CTL_U1_INM(m)      \
        in_dword_masked(HWIO_MSS_OFFLINE_PLL_USER_CTL_U1_ADDR, m)
#define HWIO_MSS_OFFLINE_PLL_USER_CTL_U1_OUT(v)      \
        out_dword(HWIO_MSS_OFFLINE_PLL_USER_CTL_U1_ADDR,v)
#define HWIO_MSS_OFFLINE_PLL_USER_CTL_U1_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_OFFLINE_PLL_USER_CTL_U1_ADDR,m,v,HWIO_MSS_OFFLINE_PLL_USER_CTL_U1_IN)

#define HWIO_MSS_OFFLINE_PLL_CONFIG_CTL_ADDR                                        (MSS_MSS_CC_REG_REG_BASE      + 0x00000018)
#define HWIO_MSS_OFFLINE_PLL_CONFIG_CTL_OFFS                                        (MSS_MSS_CC_REG_REG_BASE_OFFS + 0x00000018)
#define HWIO_MSS_OFFLINE_PLL_CONFIG_CTL_RMSK                                        0xffffffff
#define HWIO_MSS_OFFLINE_PLL_CONFIG_CTL_IN          \
        in_dword_masked(HWIO_MSS_OFFLINE_PLL_CONFIG_CTL_ADDR, HWIO_MSS_OFFLINE_PLL_CONFIG_CTL_RMSK)
#define HWIO_MSS_OFFLINE_PLL_CONFIG_CTL_INM(m)      \
        in_dword_masked(HWIO_MSS_OFFLINE_PLL_CONFIG_CTL_ADDR, m)
#define HWIO_MSS_OFFLINE_PLL_CONFIG_CTL_OUT(v)      \
        out_dword(HWIO_MSS_OFFLINE_PLL_CONFIG_CTL_ADDR,v)
#define HWIO_MSS_OFFLINE_PLL_CONFIG_CTL_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_OFFLINE_PLL_CONFIG_CTL_ADDR,m,v,HWIO_MSS_OFFLINE_PLL_CONFIG_CTL_IN)
#define HWIO_MSS_OFFLINE_PLL_CONFIG_CTL_FINE_LDC_THRESHOLD_BMSK                     0xfc000000
#define HWIO_MSS_OFFLINE_PLL_CONFIG_CTL_FINE_LDC_THRESHOLD_SHFT                           0x1a
#define HWIO_MSS_OFFLINE_PLL_CONFIG_CTL_COARSE_LDC_THRESHOLD_BMSK                    0x3c00000
#define HWIO_MSS_OFFLINE_PLL_CONFIG_CTL_COARSE_LDC_THRESHOLD_SHFT                         0x16
#define HWIO_MSS_OFFLINE_PLL_CONFIG_CTL_COARSE_LOCK_DET_NEG_THRESHOLD_BMSK            0x3c0000
#define HWIO_MSS_OFFLINE_PLL_CONFIG_CTL_COARSE_LOCK_DET_NEG_THRESHOLD_SHFT                0x12
#define HWIO_MSS_OFFLINE_PLL_CONFIG_CTL_COARSE_LOCK_DET_POS_THRESHOLD_BMSK             0x3c000
#define HWIO_MSS_OFFLINE_PLL_CONFIG_CTL_COARSE_LOCK_DET_POS_THRESHOLD_SHFT                 0xe
#define HWIO_MSS_OFFLINE_PLL_CONFIG_CTL_FINE_LOCK_DET_THRESHOLD_BMSK                    0x3800
#define HWIO_MSS_OFFLINE_PLL_CONFIG_CTL_FINE_LOCK_DET_THRESHOLD_SHFT                       0xb
#define HWIO_MSS_OFFLINE_PLL_CONFIG_CTL_FINE_LOCK_DET_SAMPLE_SIZE_BMSK                   0x700
#define HWIO_MSS_OFFLINE_PLL_CONFIG_CTL_FINE_LOCK_DET_SAMPLE_SIZE_SHFT                     0x8
#define HWIO_MSS_OFFLINE_PLL_CONFIG_CTL_FWD_GAIN_SLEWING_KFN_BMSK                         0xf0
#define HWIO_MSS_OFFLINE_PLL_CONFIG_CTL_FWD_GAIN_SLEWING_KFN_SHFT                          0x4
#define HWIO_MSS_OFFLINE_PLL_CONFIG_CTL_FWD_GAIN_KFN_BMSK                                  0xf
#define HWIO_MSS_OFFLINE_PLL_CONFIG_CTL_FWD_GAIN_KFN_SHFT                                  0x0

#define HWIO_MSS_OFFLINE_PLL_CONFIG_CTL_U_ADDR                                      (MSS_MSS_CC_REG_REG_BASE      + 0x0000001c)
#define HWIO_MSS_OFFLINE_PLL_CONFIG_CTL_U_OFFS                                      (MSS_MSS_CC_REG_REG_BASE_OFFS + 0x0000001c)
#define HWIO_MSS_OFFLINE_PLL_CONFIG_CTL_U_RMSK                                      0xffffffff
#define HWIO_MSS_OFFLINE_PLL_CONFIG_CTL_U_IN          \
        in_dword_masked(HWIO_MSS_OFFLINE_PLL_CONFIG_CTL_U_ADDR, HWIO_MSS_OFFLINE_PLL_CONFIG_CTL_U_RMSK)
#define HWIO_MSS_OFFLINE_PLL_CONFIG_CTL_U_INM(m)      \
        in_dword_masked(HWIO_MSS_OFFLINE_PLL_CONFIG_CTL_U_ADDR, m)
#define HWIO_MSS_OFFLINE_PLL_CONFIG_CTL_U_OUT(v)      \
        out_dword(HWIO_MSS_OFFLINE_PLL_CONFIG_CTL_U_ADDR,v)
#define HWIO_MSS_OFFLINE_PLL_CONFIG_CTL_U_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_OFFLINE_PLL_CONFIG_CTL_U_ADDR,m,v,HWIO_MSS_OFFLINE_PLL_CONFIG_CTL_U_IN)
#define HWIO_MSS_OFFLINE_PLL_CONFIG_CTL_U_BIST_CFG_BMSK                             0xfff00000
#define HWIO_MSS_OFFLINE_PLL_CONFIG_CTL_U_BIST_CFG_SHFT                                   0x14
#define HWIO_MSS_OFFLINE_PLL_CONFIG_CTL_U_FORCE_DAC_ON_BMSK                            0x30000
#define HWIO_MSS_OFFLINE_PLL_CONFIG_CTL_U_FORCE_DAC_ON_SHFT                               0x10
#define HWIO_MSS_OFFLINE_PLL_CONFIG_CTL_U_OSC_SETTLING_TIME_BMSK                        0x3000
#define HWIO_MSS_OFFLINE_PLL_CONFIG_CTL_U_OSC_SETTLING_TIME_SHFT                           0xc
#define HWIO_MSS_OFFLINE_PLL_CONFIG_CTL_U_NUM_STEPS_FAST_LOCK_BMSK                       0xc00
#define HWIO_MSS_OFFLINE_PLL_CONFIG_CTL_U_NUM_STEPS_FAST_LOCK_SHFT                         0xa
#define HWIO_MSS_OFFLINE_PLL_CONFIG_CTL_U_PREDIV_WHEN_CAL_BMSK                           0x380
#define HWIO_MSS_OFFLINE_PLL_CONFIG_CTL_U_PREDIV_WHEN_CAL_SHFT                             0x7
#define HWIO_MSS_OFFLINE_PLL_CONFIG_CTL_U_INC_MIN_GLITCH_THRESHOLD_4X_BMSK                0x40
#define HWIO_MSS_OFFLINE_PLL_CONFIG_CTL_U_INC_MIN_GLITCH_THRESHOLD_4X_SHFT                 0x6
#define HWIO_MSS_OFFLINE_PLL_CONFIG_CTL_U_MIN_GLITCH_THRESHOLD_BMSK                       0x30
#define HWIO_MSS_OFFLINE_PLL_CONFIG_CTL_U_MIN_GLITCH_THRESHOLD_SHFT                        0x4
#define HWIO_MSS_OFFLINE_PLL_CONFIG_CTL_U_USE_BOTH_REF_CLK_EDGE_BMSK                       0x8
#define HWIO_MSS_OFFLINE_PLL_CONFIG_CTL_U_USE_BOTH_REF_CLK_EDGE_SHFT                       0x3
#define HWIO_MSS_OFFLINE_PLL_CONFIG_CTL_U_BIAS_WARMUP_TIME_BMSK                            0x6
#define HWIO_MSS_OFFLINE_PLL_CONFIG_CTL_U_BIAS_WARMUP_TIME_SHFT                            0x1
#define HWIO_MSS_OFFLINE_PLL_CONFIG_CTL_U_DIV_LOCK_DET_THRESHOLDS_BMSK                     0x1
#define HWIO_MSS_OFFLINE_PLL_CONFIG_CTL_U_DIV_LOCK_DET_THRESHOLDS_SHFT                     0x0

#define HWIO_MSS_OFFLINE_PLL_CONFIG_CTL_U1_ADDR                                     (MSS_MSS_CC_REG_REG_BASE      + 0x00000020)
#define HWIO_MSS_OFFLINE_PLL_CONFIG_CTL_U1_OFFS                                     (MSS_MSS_CC_REG_REG_BASE_OFFS + 0x00000020)
#define HWIO_MSS_OFFLINE_PLL_CONFIG_CTL_U1_RMSK                                     0xffffffff
#define HWIO_MSS_OFFLINE_PLL_CONFIG_CTL_U1_IN          \
        in_dword_masked(HWIO_MSS_OFFLINE_PLL_CONFIG_CTL_U1_ADDR, HWIO_MSS_OFFLINE_PLL_CONFIG_CTL_U1_RMSK)
#define HWIO_MSS_OFFLINE_PLL_CONFIG_CTL_U1_INM(m)      \
        in_dword_masked(HWIO_MSS_OFFLINE_PLL_CONFIG_CTL_U1_ADDR, m)
#define HWIO_MSS_OFFLINE_PLL_CONFIG_CTL_U1_OUT(v)      \
        out_dword(HWIO_MSS_OFFLINE_PLL_CONFIG_CTL_U1_ADDR,v)
#define HWIO_MSS_OFFLINE_PLL_CONFIG_CTL_U1_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_OFFLINE_PLL_CONFIG_CTL_U1_ADDR,m,v,HWIO_MSS_OFFLINE_PLL_CONFIG_CTL_U1_IN)

#define HWIO_MSS_OFFLINE_PLL_TEST_CTL_ADDR                                          (MSS_MSS_CC_REG_REG_BASE      + 0x00000024)
#define HWIO_MSS_OFFLINE_PLL_TEST_CTL_OFFS                                          (MSS_MSS_CC_REG_REG_BASE_OFFS + 0x00000024)
#define HWIO_MSS_OFFLINE_PLL_TEST_CTL_RMSK                                          0xffffffff
#define HWIO_MSS_OFFLINE_PLL_TEST_CTL_IN          \
        in_dword_masked(HWIO_MSS_OFFLINE_PLL_TEST_CTL_ADDR, HWIO_MSS_OFFLINE_PLL_TEST_CTL_RMSK)
#define HWIO_MSS_OFFLINE_PLL_TEST_CTL_INM(m)      \
        in_dword_masked(HWIO_MSS_OFFLINE_PLL_TEST_CTL_ADDR, m)
#define HWIO_MSS_OFFLINE_PLL_TEST_CTL_OUT(v)      \
        out_dword(HWIO_MSS_OFFLINE_PLL_TEST_CTL_ADDR,v)
#define HWIO_MSS_OFFLINE_PLL_TEST_CTL_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_OFFLINE_PLL_TEST_CTL_ADDR,m,v,HWIO_MSS_OFFLINE_PLL_TEST_CTL_IN)
#define HWIO_MSS_OFFLINE_PLL_TEST_CTL_ADD_REF_CYC_CALIB_STEPS_BMSK                  0xc0000000
#define HWIO_MSS_OFFLINE_PLL_TEST_CTL_ADD_REF_CYC_CALIB_STEPS_SHFT                        0x1e
#define HWIO_MSS_OFFLINE_PLL_TEST_CTL_GLITCH_DETECTOR_COUNT_LIMIT_BMSK              0x30000000
#define HWIO_MSS_OFFLINE_PLL_TEST_CTL_GLITCH_DETECTOR_COUNT_LIMIT_SHFT                    0x1c
#define HWIO_MSS_OFFLINE_PLL_TEST_CTL_GLITCH_PREVENTION_DIS_BMSK                     0x8000000
#define HWIO_MSS_OFFLINE_PLL_TEST_CTL_GLITCH_PREVENTION_DIS_SHFT                          0x1b
#define HWIO_MSS_OFFLINE_PLL_TEST_CTL_FINE_FCW_BMSK                                  0x7e00000
#define HWIO_MSS_OFFLINE_PLL_TEST_CTL_FINE_FCW_SHFT                                       0x15
#define HWIO_MSS_OFFLINE_PLL_TEST_CTL_OVERRIDE_FINE_FCW_BMSK                          0x100000
#define HWIO_MSS_OFFLINE_PLL_TEST_CTL_OVERRIDE_FINE_FCW_SHFT                              0x14
#define HWIO_MSS_OFFLINE_PLL_TEST_CTL_COARSE_FCW_BMSK                                  0xff000
#define HWIO_MSS_OFFLINE_PLL_TEST_CTL_COARSE_FCW_SHFT                                      0xc
#define HWIO_MSS_OFFLINE_PLL_TEST_CTL_OVERRIDE_COARSE_FCW_BMSK                           0x800
#define HWIO_MSS_OFFLINE_PLL_TEST_CTL_OVERRIDE_COARSE_FCW_SHFT                             0xb
#define HWIO_MSS_OFFLINE_PLL_TEST_CTL_PROCESS_CALIB_WORD_BMSK                            0x700
#define HWIO_MSS_OFFLINE_PLL_TEST_CTL_PROCESS_CALIB_WORD_SHFT                              0x8
#define HWIO_MSS_OFFLINE_PLL_TEST_CTL_OVERRIDE_CALIB_WORD_BMSK                            0x80
#define HWIO_MSS_OFFLINE_PLL_TEST_CTL_OVERRIDE_CALIB_WORD_SHFT                             0x7
#define HWIO_MSS_OFFLINE_PLL_TEST_CTL_DISABLE_LFSR_BMSK                                   0x40
#define HWIO_MSS_OFFLINE_PLL_TEST_CTL_DISABLE_LFSR_SHFT                                    0x6
#define HWIO_MSS_OFFLINE_PLL_TEST_CTL_EN_VREF_TEST_BMSK                                   0x20
#define HWIO_MSS_OFFLINE_PLL_TEST_CTL_EN_VREF_TEST_SHFT                                    0x5
#define HWIO_MSS_OFFLINE_PLL_TEST_CTL_EN_VDCO_TEST_BMSK                                   0x10
#define HWIO_MSS_OFFLINE_PLL_TEST_CTL_EN_VDCO_TEST_SHFT                                    0x4
#define HWIO_MSS_OFFLINE_PLL_TEST_CTL_BYPASS_ATEST_BUFFER_BMSK                             0x8
#define HWIO_MSS_OFFLINE_PLL_TEST_CTL_BYPASS_ATEST_BUFFER_SHFT                             0x3
#define HWIO_MSS_OFFLINE_PLL_TEST_CTL_EN_IVCO_TEST_BMSK                                    0x4
#define HWIO_MSS_OFFLINE_PLL_TEST_CTL_EN_IVCO_TEST_SHFT                                    0x2
#define HWIO_MSS_OFFLINE_PLL_TEST_CTL_ATEST1_EN_BMSK                                       0x2
#define HWIO_MSS_OFFLINE_PLL_TEST_CTL_ATEST1_EN_SHFT                                       0x1
#define HWIO_MSS_OFFLINE_PLL_TEST_CTL_ATEST0_EN_BMSK                                       0x1
#define HWIO_MSS_OFFLINE_PLL_TEST_CTL_ATEST0_EN_SHFT                                       0x0

#define HWIO_MSS_OFFLINE_PLL_TEST_CTL_U_ADDR                                        (MSS_MSS_CC_REG_REG_BASE      + 0x00000028)
#define HWIO_MSS_OFFLINE_PLL_TEST_CTL_U_OFFS                                        (MSS_MSS_CC_REG_REG_BASE_OFFS + 0x00000028)
#define HWIO_MSS_OFFLINE_PLL_TEST_CTL_U_RMSK                                        0xffffffff
#define HWIO_MSS_OFFLINE_PLL_TEST_CTL_U_IN          \
        in_dword_masked(HWIO_MSS_OFFLINE_PLL_TEST_CTL_U_ADDR, HWIO_MSS_OFFLINE_PLL_TEST_CTL_U_RMSK)
#define HWIO_MSS_OFFLINE_PLL_TEST_CTL_U_INM(m)      \
        in_dword_masked(HWIO_MSS_OFFLINE_PLL_TEST_CTL_U_ADDR, m)
#define HWIO_MSS_OFFLINE_PLL_TEST_CTL_U_OUT(v)      \
        out_dword(HWIO_MSS_OFFLINE_PLL_TEST_CTL_U_ADDR,v)
#define HWIO_MSS_OFFLINE_PLL_TEST_CTL_U_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_OFFLINE_PLL_TEST_CTL_U_ADDR,m,v,HWIO_MSS_OFFLINE_PLL_TEST_CTL_U_IN)
#define HWIO_MSS_OFFLINE_PLL_TEST_CTL_U_ENABLE_TRAINING_SEQ_BMSK                    0x80000000
#define HWIO_MSS_OFFLINE_PLL_TEST_CTL_U_ENABLE_TRAINING_SEQ_SHFT                          0x1f
#define HWIO_MSS_OFFLINE_PLL_TEST_CTL_U_BYPASS_LOGIC_DEP_BMSK                       0x40000000
#define HWIO_MSS_OFFLINE_PLL_TEST_CTL_U_BYPASS_LOGIC_DEP_SHFT                             0x1e
#define HWIO_MSS_OFFLINE_PLL_TEST_CTL_U_BYPASS_BIAS_DET_BMSK                        0x20000000
#define HWIO_MSS_OFFLINE_PLL_TEST_CTL_U_BYPASS_BIAS_DET_SHFT                              0x1d
#define HWIO_MSS_OFFLINE_PLL_TEST_CTL_U_MSB_CLOCK_SELECT_BMSK                       0x10000000
#define HWIO_MSS_OFFLINE_PLL_TEST_CTL_U_MSB_CLOCK_SELECT_SHFT                             0x1c
#define HWIO_MSS_OFFLINE_PLL_TEST_CTL_U_BYPASS_FIRST_ORDER_DSM_BMSK                  0x8000000
#define HWIO_MSS_OFFLINE_PLL_TEST_CTL_U_BYPASS_FIRST_ORDER_DSM_SHFT                       0x1b
#define HWIO_MSS_OFFLINE_PLL_TEST_CTL_U_DCO_OFFSET_CURRENT_ADJUST_BMSK               0x6000000
#define HWIO_MSS_OFFLINE_PLL_TEST_CTL_U_DCO_OFFSET_CURRENT_ADJUST_SHFT                    0x19
#define HWIO_MSS_OFFLINE_PLL_TEST_CTL_U_MIRROR_DEVICE_ADJUST_BMSK                    0x1c00000
#define HWIO_MSS_OFFLINE_PLL_TEST_CTL_U_MIRROR_DEVICE_ADJUST_SHFT                         0x16
#define HWIO_MSS_OFFLINE_PLL_TEST_CTL_U_BIAS_STARTUP_CIR_DIS_BMSK                     0x200000
#define HWIO_MSS_OFFLINE_PLL_TEST_CTL_U_BIAS_STARTUP_CIR_DIS_SHFT                         0x15
#define HWIO_MSS_OFFLINE_PLL_TEST_CTL_U_DISABLE_CLAMP_BMSK                            0x100000
#define HWIO_MSS_OFFLINE_PLL_TEST_CTL_U_DISABLE_CLAMP_SHFT                                0x14
#define HWIO_MSS_OFFLINE_PLL_TEST_CTL_U_BYPASS_MODE_OF_BIAS_BMSK                       0x80000
#define HWIO_MSS_OFFLINE_PLL_TEST_CTL_U_BYPASS_MODE_OF_BIAS_SHFT                          0x13
#define HWIO_MSS_OFFLINE_PLL_TEST_CTL_U_BYPASS_MODE_FOR_BIAS_EN_BMSK                   0x40000
#define HWIO_MSS_OFFLINE_PLL_TEST_CTL_U_BYPASS_MODE_FOR_BIAS_EN_SHFT                      0x12
#define HWIO_MSS_OFFLINE_PLL_TEST_CTL_U_BIAS_ADJUST_BMSK                               0x30000
#define HWIO_MSS_OFFLINE_PLL_TEST_CTL_U_BIAS_ADJUST_SHFT                                  0x10
#define HWIO_MSS_OFFLINE_PLL_TEST_CTL_U_DIV2_NMO_EN_BMSK                                0x8000
#define HWIO_MSS_OFFLINE_PLL_TEST_CTL_U_DIV2_NMO_EN_SHFT                                   0xf
#define HWIO_MSS_OFFLINE_PLL_TEST_CTL_U_DIS_AMP_STARTUP_BMSK                            0x4000
#define HWIO_MSS_OFFLINE_PLL_TEST_CTL_U_DIS_AMP_STARTUP_SHFT                               0xe
#define HWIO_MSS_OFFLINE_PLL_TEST_CTL_U_SINGLE_DMET_EN_BMSK                             0x2000
#define HWIO_MSS_OFFLINE_PLL_TEST_CTL_U_SINGLE_DMET_EN_SHFT                                0xd
#define HWIO_MSS_OFFLINE_PLL_TEST_CTL_U_DEMET_WINDOW_DIS_BMSK                           0x1000
#define HWIO_MSS_OFFLINE_PLL_TEST_CTL_U_DEMET_WINDOW_DIS_SHFT                              0xc
#define HWIO_MSS_OFFLINE_PLL_TEST_CTL_U_NMO_OSC_SEL_BMSK                                 0xc00
#define HWIO_MSS_OFFLINE_PLL_TEST_CTL_U_NMO_OSC_SEL_SHFT                                   0xa
#define HWIO_MSS_OFFLINE_PLL_TEST_CTL_U_NOISE_MAG_BMSK                                   0x380
#define HWIO_MSS_OFFLINE_PLL_TEST_CTL_U_NOISE_MAG_SHFT                                     0x7
#define HWIO_MSS_OFFLINE_PLL_TEST_CTL_U_NOISE_GEN_EN_BMSK                                 0x40
#define HWIO_MSS_OFFLINE_PLL_TEST_CTL_U_NOISE_GEN_EN_SHFT                                  0x6
#define HWIO_MSS_OFFLINE_PLL_TEST_CTL_U_OSC_BIAS_GND_BMSK                                 0x20
#define HWIO_MSS_OFFLINE_PLL_TEST_CTL_U_OSC_BIAS_GND_SHFT                                  0x5
#define HWIO_MSS_OFFLINE_PLL_TEST_CTL_U_PLL_TEST_OUT_SEL_BMSK                             0x18
#define HWIO_MSS_OFFLINE_PLL_TEST_CTL_U_PLL_TEST_OUT_SEL_SHFT                              0x3
#define HWIO_MSS_OFFLINE_PLL_TEST_CTL_U_STATUS_REG_EN_BMSK                                 0x4
#define HWIO_MSS_OFFLINE_PLL_TEST_CTL_U_STATUS_REG_EN_SHFT                                 0x2
#define HWIO_MSS_OFFLINE_PLL_TEST_CTL_U_SKIP_FINE_TUNE_BMSK                                0x2
#define HWIO_MSS_OFFLINE_PLL_TEST_CTL_U_SKIP_FINE_TUNE_SHFT                                0x1
#define HWIO_MSS_OFFLINE_PLL_TEST_CTL_U_SKIP_FAST_LOCK_BMSK                                0x1
#define HWIO_MSS_OFFLINE_PLL_TEST_CTL_U_SKIP_FAST_LOCK_SHFT                                0x0

#define HWIO_MSS_OFFLINE_PLL_TEST_CTL_U1_ADDR                                       (MSS_MSS_CC_REG_REG_BASE      + 0x0000002c)
#define HWIO_MSS_OFFLINE_PLL_TEST_CTL_U1_OFFS                                       (MSS_MSS_CC_REG_REG_BASE_OFFS + 0x0000002c)
#define HWIO_MSS_OFFLINE_PLL_TEST_CTL_U1_RMSK                                       0xffffffff
#define HWIO_MSS_OFFLINE_PLL_TEST_CTL_U1_IN          \
        in_dword_masked(HWIO_MSS_OFFLINE_PLL_TEST_CTL_U1_ADDR, HWIO_MSS_OFFLINE_PLL_TEST_CTL_U1_RMSK)
#define HWIO_MSS_OFFLINE_PLL_TEST_CTL_U1_INM(m)      \
        in_dword_masked(HWIO_MSS_OFFLINE_PLL_TEST_CTL_U1_ADDR, m)
#define HWIO_MSS_OFFLINE_PLL_TEST_CTL_U1_OUT(v)      \
        out_dword(HWIO_MSS_OFFLINE_PLL_TEST_CTL_U1_ADDR,v)
#define HWIO_MSS_OFFLINE_PLL_TEST_CTL_U1_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_OFFLINE_PLL_TEST_CTL_U1_ADDR,m,v,HWIO_MSS_OFFLINE_PLL_TEST_CTL_U1_IN)

#define HWIO_MSS_OFFLINE_PLL_STATUS_ADDR                                            (MSS_MSS_CC_REG_REG_BASE      + 0x00000030)
#define HWIO_MSS_OFFLINE_PLL_STATUS_OFFS                                            (MSS_MSS_CC_REG_REG_BASE_OFFS + 0x00000030)
#define HWIO_MSS_OFFLINE_PLL_STATUS_RMSK                                            0xffffffff
#define HWIO_MSS_OFFLINE_PLL_STATUS_IN          \
        in_dword_masked(HWIO_MSS_OFFLINE_PLL_STATUS_ADDR, HWIO_MSS_OFFLINE_PLL_STATUS_RMSK)
#define HWIO_MSS_OFFLINE_PLL_STATUS_INM(m)      \
        in_dword_masked(HWIO_MSS_OFFLINE_PLL_STATUS_ADDR, m)
#define HWIO_MSS_OFFLINE_PLL_STATUS_STATUS_31_0_BMSK                                0xffffffff
#define HWIO_MSS_OFFLINE_PLL_STATUS_STATUS_31_0_SHFT                                       0x0

#define HWIO_MSS_OFFLINE_PLL_FREQ_CTL_ADDR                                          (MSS_MSS_CC_REG_REG_BASE      + 0x00000034)
#define HWIO_MSS_OFFLINE_PLL_FREQ_CTL_OFFS                                          (MSS_MSS_CC_REG_REG_BASE_OFFS + 0x00000034)
#define HWIO_MSS_OFFLINE_PLL_FREQ_CTL_RMSK                                          0xffffffff
#define HWIO_MSS_OFFLINE_PLL_FREQ_CTL_IN          \
        in_dword_masked(HWIO_MSS_OFFLINE_PLL_FREQ_CTL_ADDR, HWIO_MSS_OFFLINE_PLL_FREQ_CTL_RMSK)
#define HWIO_MSS_OFFLINE_PLL_FREQ_CTL_INM(m)      \
        in_dword_masked(HWIO_MSS_OFFLINE_PLL_FREQ_CTL_ADDR, m)
#define HWIO_MSS_OFFLINE_PLL_FREQ_CTL_OUT(v)      \
        out_dword(HWIO_MSS_OFFLINE_PLL_FREQ_CTL_ADDR,v)
#define HWIO_MSS_OFFLINE_PLL_FREQ_CTL_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_OFFLINE_PLL_FREQ_CTL_ADDR,m,v,HWIO_MSS_OFFLINE_PLL_FREQ_CTL_IN)
#define HWIO_MSS_OFFLINE_PLL_FREQ_CTL_FREQUENCY_CTL_WORD_BMSK                       0xffffffff
#define HWIO_MSS_OFFLINE_PLL_FREQ_CTL_FREQUENCY_CTL_WORD_SHFT                              0x0

#define HWIO_MSS_OFFLINE_PLL_OPMODE_ADDR                                            (MSS_MSS_CC_REG_REG_BASE      + 0x00000038)
#define HWIO_MSS_OFFLINE_PLL_OPMODE_OFFS                                            (MSS_MSS_CC_REG_REG_BASE_OFFS + 0x00000038)
#define HWIO_MSS_OFFLINE_PLL_OPMODE_RMSK                                                   0x7
#define HWIO_MSS_OFFLINE_PLL_OPMODE_IN          \
        in_dword_masked(HWIO_MSS_OFFLINE_PLL_OPMODE_ADDR, HWIO_MSS_OFFLINE_PLL_OPMODE_RMSK)
#define HWIO_MSS_OFFLINE_PLL_OPMODE_INM(m)      \
        in_dword_masked(HWIO_MSS_OFFLINE_PLL_OPMODE_ADDR, m)
#define HWIO_MSS_OFFLINE_PLL_OPMODE_OUT(v)      \
        out_dword(HWIO_MSS_OFFLINE_PLL_OPMODE_ADDR,v)
#define HWIO_MSS_OFFLINE_PLL_OPMODE_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_OFFLINE_PLL_OPMODE_ADDR,m,v,HWIO_MSS_OFFLINE_PLL_OPMODE_IN)
#define HWIO_MSS_OFFLINE_PLL_OPMODE_PLL_OPMODE_BMSK                                        0x7
#define HWIO_MSS_OFFLINE_PLL_OPMODE_PLL_OPMODE_SHFT                                        0x0

#define HWIO_MSS_OFFLINE_PLL_STATE_ADDR                                             (MSS_MSS_CC_REG_REG_BASE      + 0x0000003c)
#define HWIO_MSS_OFFLINE_PLL_STATE_OFFS                                             (MSS_MSS_CC_REG_REG_BASE_OFFS + 0x0000003c)
#define HWIO_MSS_OFFLINE_PLL_STATE_RMSK                                                    0x7
#define HWIO_MSS_OFFLINE_PLL_STATE_IN          \
        in_dword_masked(HWIO_MSS_OFFLINE_PLL_STATE_ADDR, HWIO_MSS_OFFLINE_PLL_STATE_RMSK)
#define HWIO_MSS_OFFLINE_PLL_STATE_INM(m)      \
        in_dword_masked(HWIO_MSS_OFFLINE_PLL_STATE_ADDR, m)
#define HWIO_MSS_OFFLINE_PLL_STATE_PLL_STATE_BMSK                                          0x7
#define HWIO_MSS_OFFLINE_PLL_STATE_PLL_STATE_SHFT                                          0x0

#define HWIO_MSS_OFFLINE_PLL_ALPHA_VAL_ADDR                                         (MSS_MSS_CC_REG_REG_BASE      + 0x00000040)
#define HWIO_MSS_OFFLINE_PLL_ALPHA_VAL_OFFS                                         (MSS_MSS_CC_REG_REG_BASE_OFFS + 0x00000040)
#define HWIO_MSS_OFFLINE_PLL_ALPHA_VAL_RMSK                                             0xffff
#define HWIO_MSS_OFFLINE_PLL_ALPHA_VAL_IN          \
        in_dword_masked(HWIO_MSS_OFFLINE_PLL_ALPHA_VAL_ADDR, HWIO_MSS_OFFLINE_PLL_ALPHA_VAL_RMSK)
#define HWIO_MSS_OFFLINE_PLL_ALPHA_VAL_INM(m)      \
        in_dword_masked(HWIO_MSS_OFFLINE_PLL_ALPHA_VAL_ADDR, m)
#define HWIO_MSS_OFFLINE_PLL_ALPHA_VAL_OUT(v)      \
        out_dword(HWIO_MSS_OFFLINE_PLL_ALPHA_VAL_ADDR,v)
#define HWIO_MSS_OFFLINE_PLL_ALPHA_VAL_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_OFFLINE_PLL_ALPHA_VAL_ADDR,m,v,HWIO_MSS_OFFLINE_PLL_ALPHA_VAL_IN)
#define HWIO_MSS_OFFLINE_PLL_ALPHA_VAL_PLL_ALPHA_VAL_BMSK                               0xffff
#define HWIO_MSS_OFFLINE_PLL_ALPHA_VAL_PLL_ALPHA_VAL_SHFT                                  0x0

#define HWIO_MSS_OFFLINE_PLL_SPARE_ADDR                                             (MSS_MSS_CC_REG_REG_BASE      + 0x00000044)
#define HWIO_MSS_OFFLINE_PLL_SPARE_OFFS                                             (MSS_MSS_CC_REG_REG_BASE_OFFS + 0x00000044)
#define HWIO_MSS_OFFLINE_PLL_SPARE_RMSK                                                   0xff
#define HWIO_MSS_OFFLINE_PLL_SPARE_IN          \
        in_dword_masked(HWIO_MSS_OFFLINE_PLL_SPARE_ADDR, HWIO_MSS_OFFLINE_PLL_SPARE_RMSK)
#define HWIO_MSS_OFFLINE_PLL_SPARE_INM(m)      \
        in_dword_masked(HWIO_MSS_OFFLINE_PLL_SPARE_ADDR, m)
#define HWIO_MSS_OFFLINE_PLL_SPARE_OUT(v)      \
        out_dword(HWIO_MSS_OFFLINE_PLL_SPARE_ADDR,v)
#define HWIO_MSS_OFFLINE_PLL_SPARE_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_OFFLINE_PLL_SPARE_ADDR,m,v,HWIO_MSS_OFFLINE_PLL_SPARE_IN)
#define HWIO_MSS_OFFLINE_PLL_SPARE_SPARE_OUTPUTS_BMSK                                     0xf0
#define HWIO_MSS_OFFLINE_PLL_SPARE_SPARE_OUTPUTS_SHFT                                      0x4
#define HWIO_MSS_OFFLINE_PLL_SPARE_SPARE_INPUTS_BMSK                                       0xf
#define HWIO_MSS_OFFLINE_PLL_SPARE_SPARE_INPUTS_SHFT                                       0x0

#define HWIO_MSS_OFFLINE_PLL_SSC_DELTA_ALPHA_ADDR                                   (MSS_MSS_CC_REG_REG_BASE      + 0x00000048)
#define HWIO_MSS_OFFLINE_PLL_SSC_DELTA_ALPHA_OFFS                                   (MSS_MSS_CC_REG_REG_BASE_OFFS + 0x00000048)
#define HWIO_MSS_OFFLINE_PLL_SSC_DELTA_ALPHA_RMSK                                         0xff
#define HWIO_MSS_OFFLINE_PLL_SSC_DELTA_ALPHA_IN          \
        in_dword_masked(HWIO_MSS_OFFLINE_PLL_SSC_DELTA_ALPHA_ADDR, HWIO_MSS_OFFLINE_PLL_SSC_DELTA_ALPHA_RMSK)
#define HWIO_MSS_OFFLINE_PLL_SSC_DELTA_ALPHA_INM(m)      \
        in_dword_masked(HWIO_MSS_OFFLINE_PLL_SSC_DELTA_ALPHA_ADDR, m)
#define HWIO_MSS_OFFLINE_PLL_SSC_DELTA_ALPHA_OUT(v)      \
        out_dword(HWIO_MSS_OFFLINE_PLL_SSC_DELTA_ALPHA_ADDR,v)
#define HWIO_MSS_OFFLINE_PLL_SSC_DELTA_ALPHA_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_OFFLINE_PLL_SSC_DELTA_ALPHA_ADDR,m,v,HWIO_MSS_OFFLINE_PLL_SSC_DELTA_ALPHA_IN)
#define HWIO_MSS_OFFLINE_PLL_SSC_DELTA_ALPHA_SSC_DELTA_ALPHA_BMSK                         0xff
#define HWIO_MSS_OFFLINE_PLL_SSC_DELTA_ALPHA_SSC_DELTA_ALPHA_SHFT                          0x0

#define HWIO_MSS_OFFLINE_PLL_SSC_UPDATE_RATE_ADDR                                   (MSS_MSS_CC_REG_REG_BASE      + 0x0000004c)
#define HWIO_MSS_OFFLINE_PLL_SSC_UPDATE_RATE_OFFS                                   (MSS_MSS_CC_REG_REG_BASE_OFFS + 0x0000004c)
#define HWIO_MSS_OFFLINE_PLL_SSC_UPDATE_RATE_RMSK                                          0xf
#define HWIO_MSS_OFFLINE_PLL_SSC_UPDATE_RATE_IN          \
        in_dword_masked(HWIO_MSS_OFFLINE_PLL_SSC_UPDATE_RATE_ADDR, HWIO_MSS_OFFLINE_PLL_SSC_UPDATE_RATE_RMSK)
#define HWIO_MSS_OFFLINE_PLL_SSC_UPDATE_RATE_INM(m)      \
        in_dword_masked(HWIO_MSS_OFFLINE_PLL_SSC_UPDATE_RATE_ADDR, m)
#define HWIO_MSS_OFFLINE_PLL_SSC_UPDATE_RATE_OUT(v)      \
        out_dword(HWIO_MSS_OFFLINE_PLL_SSC_UPDATE_RATE_ADDR,v)
#define HWIO_MSS_OFFLINE_PLL_SSC_UPDATE_RATE_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_OFFLINE_PLL_SSC_UPDATE_RATE_ADDR,m,v,HWIO_MSS_OFFLINE_PLL_SSC_UPDATE_RATE_IN)
#define HWIO_MSS_OFFLINE_PLL_SSC_UPDATE_RATE_SSC_UPDATE_RATE_BMSK                          0xf
#define HWIO_MSS_OFFLINE_PLL_SSC_UPDATE_RATE_SSC_UPDATE_RATE_SHFT                          0x0

#define HWIO_MSS_OFFLINE_PLL_SSC_NUM_STEPS_ADDR                                     (MSS_MSS_CC_REG_REG_BASE      + 0x00000050)
#define HWIO_MSS_OFFLINE_PLL_SSC_NUM_STEPS_OFFS                                     (MSS_MSS_CC_REG_REG_BASE_OFFS + 0x00000050)
#define HWIO_MSS_OFFLINE_PLL_SSC_NUM_STEPS_RMSK                                            0xf
#define HWIO_MSS_OFFLINE_PLL_SSC_NUM_STEPS_IN          \
        in_dword_masked(HWIO_MSS_OFFLINE_PLL_SSC_NUM_STEPS_ADDR, HWIO_MSS_OFFLINE_PLL_SSC_NUM_STEPS_RMSK)
#define HWIO_MSS_OFFLINE_PLL_SSC_NUM_STEPS_INM(m)      \
        in_dword_masked(HWIO_MSS_OFFLINE_PLL_SSC_NUM_STEPS_ADDR, m)
#define HWIO_MSS_OFFLINE_PLL_SSC_NUM_STEPS_OUT(v)      \
        out_dword(HWIO_MSS_OFFLINE_PLL_SSC_NUM_STEPS_ADDR,v)
#define HWIO_MSS_OFFLINE_PLL_SSC_NUM_STEPS_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_OFFLINE_PLL_SSC_NUM_STEPS_ADDR,m,v,HWIO_MSS_OFFLINE_PLL_SSC_NUM_STEPS_IN)
#define HWIO_MSS_OFFLINE_PLL_SSC_NUM_STEPS_SSC_NUM_STEPS_BMSK                              0xf
#define HWIO_MSS_OFFLINE_PLL_SSC_NUM_STEPS_SSC_NUM_STEPS_SHFT                              0x0

#define HWIO_MSS_MPSS_PLL_MODE_ADDR                                                 (MSS_MSS_CC_REG_REG_BASE      + 0x00000100)
#define HWIO_MSS_MPSS_PLL_MODE_OFFS                                                 (MSS_MSS_CC_REG_REG_BASE_OFFS + 0x00000100)
#define HWIO_MSS_MPSS_PLL_MODE_RMSK                                                 0xffffffff
#define HWIO_MSS_MPSS_PLL_MODE_IN          \
        in_dword_masked(HWIO_MSS_MPSS_PLL_MODE_ADDR, HWIO_MSS_MPSS_PLL_MODE_RMSK)
#define HWIO_MSS_MPSS_PLL_MODE_INM(m)      \
        in_dword_masked(HWIO_MSS_MPSS_PLL_MODE_ADDR, m)
#define HWIO_MSS_MPSS_PLL_MODE_OUT(v)      \
        out_dword(HWIO_MSS_MPSS_PLL_MODE_ADDR,v)
#define HWIO_MSS_MPSS_PLL_MODE_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_MPSS_PLL_MODE_ADDR,m,v,HWIO_MSS_MPSS_PLL_MODE_IN)
#define HWIO_MSS_MPSS_PLL_MODE_PLL_LOCK_DET_BMSK                                    0x80000000
#define HWIO_MSS_MPSS_PLL_MODE_PLL_LOCK_DET_SHFT                                          0x1f
#define HWIO_MSS_MPSS_PLL_MODE_PLL_ACTIVE_FLAG_BMSK                                 0x40000000
#define HWIO_MSS_MPSS_PLL_MODE_PLL_ACTIVE_FLAG_SHFT                                       0x1e
#define HWIO_MSS_MPSS_PLL_MODE_PLL_ACK_LATCH_BMSK                                   0x20000000
#define HWIO_MSS_MPSS_PLL_MODE_PLL_ACK_LATCH_SHFT                                         0x1d
#define HWIO_MSS_MPSS_PLL_MODE_PLL_LOCK_DET_FINE_BMSK                               0x10000000
#define HWIO_MSS_MPSS_PLL_MODE_PLL_LOCK_DET_FINE_SHFT                                     0x1c
#define HWIO_MSS_MPSS_PLL_MODE_FSM_LEGACY_MODE_BMSK                                  0x1000000
#define HWIO_MSS_MPSS_PLL_MODE_FSM_LEGACY_MODE_SHFT                                       0x18
#define HWIO_MSS_MPSS_PLL_MODE_PLL_HW_UPDATE_LOGIC_BYPASS_BMSK                        0x800000
#define HWIO_MSS_MPSS_PLL_MODE_PLL_HW_UPDATE_LOGIC_BYPASS_SHFT                            0x17
#define HWIO_MSS_MPSS_PLL_MODE_PLL_UPDATE_BMSK                                        0x400000
#define HWIO_MSS_MPSS_PLL_MODE_PLL_UPDATE_SHFT                                            0x16
#define HWIO_MSS_MPSS_PLL_MODE_PLL_VOTE_FSM_RESET_BMSK                                0x200000
#define HWIO_MSS_MPSS_PLL_MODE_PLL_VOTE_FSM_RESET_SHFT                                    0x15
#define HWIO_MSS_MPSS_PLL_MODE_PLL_VOTE_FSM_ENA_BMSK                                  0x100000
#define HWIO_MSS_MPSS_PLL_MODE_PLL_VOTE_FSM_ENA_SHFT                                      0x14
#define HWIO_MSS_MPSS_PLL_MODE_PLL_BIAS_COUNT_BMSK                                     0xfc000
#define HWIO_MSS_MPSS_PLL_MODE_PLL_BIAS_COUNT_SHFT                                         0xe
#define HWIO_MSS_MPSS_PLL_MODE_PLL_LOCK_COUNT_BMSK                                      0x3f00
#define HWIO_MSS_MPSS_PLL_MODE_PLL_LOCK_COUNT_SHFT                                         0x8
#define HWIO_MSS_MPSS_PLL_MODE_OUT_TEST_HW_CTL_BMSK                                       0x40
#define HWIO_MSS_MPSS_PLL_MODE_OUT_TEST_HW_CTL_SHFT                                        0x6
#define HWIO_MSS_MPSS_PLL_MODE_OUT_ODD_HW_CTL_BMSK                                        0x20
#define HWIO_MSS_MPSS_PLL_MODE_OUT_ODD_HW_CTL_SHFT                                         0x5
#define HWIO_MSS_MPSS_PLL_MODE_OUT_EVEN_HW_CTL_BMSK                                       0x10
#define HWIO_MSS_MPSS_PLL_MODE_OUT_EVEN_HW_CTL_SHFT                                        0x4
#define HWIO_MSS_MPSS_PLL_MODE_OUT_MAIN_HW_CTL_BMSK                                        0x8
#define HWIO_MSS_MPSS_PLL_MODE_OUT_MAIN_HW_CTL_SHFT                                        0x3
#define HWIO_MSS_MPSS_PLL_MODE_PLL_RESET_N_BMSK                                            0x4
#define HWIO_MSS_MPSS_PLL_MODE_PLL_RESET_N_SHFT                                            0x2
#define HWIO_MSS_MPSS_PLL_MODE_PLL_OUTCTRL_BMSK                                            0x1
#define HWIO_MSS_MPSS_PLL_MODE_PLL_OUTCTRL_SHFT                                            0x0

#define HWIO_MSS_MPSS_PLL_L_VAL_ADDR                                                (MSS_MSS_CC_REG_REG_BASE      + 0x00000104)
#define HWIO_MSS_MPSS_PLL_L_VAL_OFFS                                                (MSS_MSS_CC_REG_REG_BASE_OFFS + 0x00000104)
#define HWIO_MSS_MPSS_PLL_L_VAL_RMSK                                                    0xffff
#define HWIO_MSS_MPSS_PLL_L_VAL_IN          \
        in_dword_masked(HWIO_MSS_MPSS_PLL_L_VAL_ADDR, HWIO_MSS_MPSS_PLL_L_VAL_RMSK)
#define HWIO_MSS_MPSS_PLL_L_VAL_INM(m)      \
        in_dword_masked(HWIO_MSS_MPSS_PLL_L_VAL_ADDR, m)
#define HWIO_MSS_MPSS_PLL_L_VAL_OUT(v)      \
        out_dword(HWIO_MSS_MPSS_PLL_L_VAL_ADDR,v)
#define HWIO_MSS_MPSS_PLL_L_VAL_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_MPSS_PLL_L_VAL_ADDR,m,v,HWIO_MSS_MPSS_PLL_L_VAL_IN)
#define HWIO_MSS_MPSS_PLL_L_VAL_PLL_L_BMSK                                              0xffff
#define HWIO_MSS_MPSS_PLL_L_VAL_PLL_L_SHFT                                                 0x0

#define HWIO_MSS_MPSS_PLL_CAL_L_VAL_ADDR                                            (MSS_MSS_CC_REG_REG_BASE      + 0x00000108)
#define HWIO_MSS_MPSS_PLL_CAL_L_VAL_OFFS                                            (MSS_MSS_CC_REG_REG_BASE_OFFS + 0x00000108)
#define HWIO_MSS_MPSS_PLL_CAL_L_VAL_RMSK                                                0xffff
#define HWIO_MSS_MPSS_PLL_CAL_L_VAL_IN          \
        in_dword_masked(HWIO_MSS_MPSS_PLL_CAL_L_VAL_ADDR, HWIO_MSS_MPSS_PLL_CAL_L_VAL_RMSK)
#define HWIO_MSS_MPSS_PLL_CAL_L_VAL_INM(m)      \
        in_dword_masked(HWIO_MSS_MPSS_PLL_CAL_L_VAL_ADDR, m)
#define HWIO_MSS_MPSS_PLL_CAL_L_VAL_OUT(v)      \
        out_dword(HWIO_MSS_MPSS_PLL_CAL_L_VAL_ADDR,v)
#define HWIO_MSS_MPSS_PLL_CAL_L_VAL_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_MPSS_PLL_CAL_L_VAL_ADDR,m,v,HWIO_MSS_MPSS_PLL_CAL_L_VAL_IN)
#define HWIO_MSS_MPSS_PLL_CAL_L_VAL_PLL_CAL_L_BMSK                                      0xffff
#define HWIO_MSS_MPSS_PLL_CAL_L_VAL_PLL_CAL_L_SHFT                                         0x0

#define HWIO_MSS_MPSS_PLL_USER_CTL_ADDR                                             (MSS_MSS_CC_REG_REG_BASE      + 0x0000010c)
#define HWIO_MSS_MPSS_PLL_USER_CTL_OFFS                                             (MSS_MSS_CC_REG_REG_BASE_OFFS + 0x0000010c)
#define HWIO_MSS_MPSS_PLL_USER_CTL_RMSK                                             0xffffffff
#define HWIO_MSS_MPSS_PLL_USER_CTL_IN          \
        in_dword_masked(HWIO_MSS_MPSS_PLL_USER_CTL_ADDR, HWIO_MSS_MPSS_PLL_USER_CTL_RMSK)
#define HWIO_MSS_MPSS_PLL_USER_CTL_INM(m)      \
        in_dword_masked(HWIO_MSS_MPSS_PLL_USER_CTL_ADDR, m)
#define HWIO_MSS_MPSS_PLL_USER_CTL_OUT(v)      \
        out_dword(HWIO_MSS_MPSS_PLL_USER_CTL_ADDR,v)
#define HWIO_MSS_MPSS_PLL_USER_CTL_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_MPSS_PLL_USER_CTL_ADDR,m,v,HWIO_MSS_MPSS_PLL_USER_CTL_IN)
#define HWIO_MSS_MPSS_PLL_USER_CTL_PRE_DIV_RATIO_BMSK                                  0x70000
#define HWIO_MSS_MPSS_PLL_USER_CTL_PRE_DIV_RATIO_SHFT                                     0x10
#define HWIO_MSS_MPSS_PLL_USER_CTL_POST_DIV_RATIO_ODD_BMSK                              0xf000
#define HWIO_MSS_MPSS_PLL_USER_CTL_POST_DIV_RATIO_ODD_SHFT                                 0xc
#define HWIO_MSS_MPSS_PLL_USER_CTL_POST_DIV_RATIO_EVEN_BMSK                              0xf00
#define HWIO_MSS_MPSS_PLL_USER_CTL_POST_DIV_RATIO_EVEN_SHFT                                0x8
#define HWIO_MSS_MPSS_PLL_USER_CTL_OUT_CLK_POLARITY_BMSK                                  0x80
#define HWIO_MSS_MPSS_PLL_USER_CTL_OUT_CLK_POLARITY_SHFT                                   0x7
#define HWIO_MSS_MPSS_PLL_USER_CTL_PLLOUT_TEST_BMSK                                       0x10
#define HWIO_MSS_MPSS_PLL_USER_CTL_PLLOUT_TEST_SHFT                                        0x4
#define HWIO_MSS_MPSS_PLL_USER_CTL_PLLOUT_ODD_BMSK                                         0x4
#define HWIO_MSS_MPSS_PLL_USER_CTL_PLLOUT_ODD_SHFT                                         0x2
#define HWIO_MSS_MPSS_PLL_USER_CTL_PLLOUT_EVEN_BMSK                                        0x2
#define HWIO_MSS_MPSS_PLL_USER_CTL_PLLOUT_EVEN_SHFT                                        0x1
#define HWIO_MSS_MPSS_PLL_USER_CTL_PLLOUT_MAIN_BMSK                                        0x1
#define HWIO_MSS_MPSS_PLL_USER_CTL_PLLOUT_MAIN_SHFT                                        0x0

#define HWIO_MSS_MPSS_PLL_USER_CTL_U_ADDR                                           (MSS_MSS_CC_REG_REG_BASE      + 0x00000110)
#define HWIO_MSS_MPSS_PLL_USER_CTL_U_OFFS                                           (MSS_MSS_CC_REG_REG_BASE_OFFS + 0x00000110)
#define HWIO_MSS_MPSS_PLL_USER_CTL_U_RMSK                                           0xffffffff
#define HWIO_MSS_MPSS_PLL_USER_CTL_U_IN          \
        in_dword_masked(HWIO_MSS_MPSS_PLL_USER_CTL_U_ADDR, HWIO_MSS_MPSS_PLL_USER_CTL_U_RMSK)
#define HWIO_MSS_MPSS_PLL_USER_CTL_U_INM(m)      \
        in_dword_masked(HWIO_MSS_MPSS_PLL_USER_CTL_U_ADDR, m)
#define HWIO_MSS_MPSS_PLL_USER_CTL_U_OUT(v)      \
        out_dword(HWIO_MSS_MPSS_PLL_USER_CTL_U_ADDR,v)
#define HWIO_MSS_MPSS_PLL_USER_CTL_U_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_MPSS_PLL_USER_CTL_U_ADDR,m,v,HWIO_MSS_MPSS_PLL_USER_CTL_U_IN)
#define HWIO_MSS_MPSS_PLL_USER_CTL_U_USE_PREDIV_WHEN_CAL_BMSK                          0x40000
#define HWIO_MSS_MPSS_PLL_USER_CTL_U_USE_PREDIV_WHEN_CAL_SHFT                             0x12
#define HWIO_MSS_MPSS_PLL_USER_CTL_U_USE_EXT_SAVERESTORE_BMSK                          0x20000
#define HWIO_MSS_MPSS_PLL_USER_CTL_U_USE_EXT_SAVERESTORE_SHFT                             0x11
#define HWIO_MSS_MPSS_PLL_USER_CTL_U_FRAC_FORMAT_SEL_BMSK                               0x8000
#define HWIO_MSS_MPSS_PLL_USER_CTL_U_FRAC_FORMAT_SEL_SHFT                                  0xf
#define HWIO_MSS_MPSS_PLL_USER_CTL_U_BIAS_ON_IN_STANDBY_BMSK                            0x4000
#define HWIO_MSS_MPSS_PLL_USER_CTL_U_BIAS_ON_IN_STANDBY_SHFT                               0xe
#define HWIO_MSS_MPSS_PLL_USER_CTL_U_REF_CLK_AT_OUT_BMSK                                0x2000
#define HWIO_MSS_MPSS_PLL_USER_CTL_U_REF_CLK_AT_OUT_SHFT                                   0xd
#define HWIO_MSS_MPSS_PLL_USER_CTL_U_SCALE_FREQ_ON_RESTART_BMSK                         0x1800
#define HWIO_MSS_MPSS_PLL_USER_CTL_U_SCALE_FREQ_ON_RESTART_SHFT                            0xb
#define HWIO_MSS_MPSS_PLL_USER_CTL_U_LATCH_INTERFACE_BYPASS_BMSK                         0x400
#define HWIO_MSS_MPSS_PLL_USER_CTL_U_LATCH_INTERFACE_BYPASS_SHFT                           0xa
#define HWIO_MSS_MPSS_PLL_USER_CTL_U_STATUS_REGISTER_BMSK                                0x3e0
#define HWIO_MSS_MPSS_PLL_USER_CTL_U_STATUS_REGISTER_SHFT                                  0x5
#define HWIO_MSS_MPSS_PLL_USER_CTL_U_WRITE_STATE_EN_BMSK                                  0x10
#define HWIO_MSS_MPSS_PLL_USER_CTL_U_WRITE_STATE_EN_SHFT                                   0x4
#define HWIO_MSS_MPSS_PLL_USER_CTL_U_CALIB_CTRL_BMSK                                       0xe
#define HWIO_MSS_MPSS_PLL_USER_CTL_U_CALIB_CTRL_SHFT                                       0x1
#define HWIO_MSS_MPSS_PLL_USER_CTL_U_USE_ONLY_FINE_LOCK_DET_BMSK                           0x1
#define HWIO_MSS_MPSS_PLL_USER_CTL_U_USE_ONLY_FINE_LOCK_DET_SHFT                           0x0

#define HWIO_MSS_MPSS_PLL_USER_CTL_U1_ADDR                                          (MSS_MSS_CC_REG_REG_BASE      + 0x00000114)
#define HWIO_MSS_MPSS_PLL_USER_CTL_U1_OFFS                                          (MSS_MSS_CC_REG_REG_BASE_OFFS + 0x00000114)
#define HWIO_MSS_MPSS_PLL_USER_CTL_U1_RMSK                                          0xffffffff
#define HWIO_MSS_MPSS_PLL_USER_CTL_U1_IN          \
        in_dword_masked(HWIO_MSS_MPSS_PLL_USER_CTL_U1_ADDR, HWIO_MSS_MPSS_PLL_USER_CTL_U1_RMSK)
#define HWIO_MSS_MPSS_PLL_USER_CTL_U1_INM(m)      \
        in_dword_masked(HWIO_MSS_MPSS_PLL_USER_CTL_U1_ADDR, m)
#define HWIO_MSS_MPSS_PLL_USER_CTL_U1_OUT(v)      \
        out_dword(HWIO_MSS_MPSS_PLL_USER_CTL_U1_ADDR,v)
#define HWIO_MSS_MPSS_PLL_USER_CTL_U1_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_MPSS_PLL_USER_CTL_U1_ADDR,m,v,HWIO_MSS_MPSS_PLL_USER_CTL_U1_IN)

#define HWIO_MSS_MPSS_PLL_CONFIG_CTL_ADDR                                           (MSS_MSS_CC_REG_REG_BASE      + 0x00000118)
#define HWIO_MSS_MPSS_PLL_CONFIG_CTL_OFFS                                           (MSS_MSS_CC_REG_REG_BASE_OFFS + 0x00000118)
#define HWIO_MSS_MPSS_PLL_CONFIG_CTL_RMSK                                           0xffffffff
#define HWIO_MSS_MPSS_PLL_CONFIG_CTL_IN          \
        in_dword_masked(HWIO_MSS_MPSS_PLL_CONFIG_CTL_ADDR, HWIO_MSS_MPSS_PLL_CONFIG_CTL_RMSK)
#define HWIO_MSS_MPSS_PLL_CONFIG_CTL_INM(m)      \
        in_dword_masked(HWIO_MSS_MPSS_PLL_CONFIG_CTL_ADDR, m)
#define HWIO_MSS_MPSS_PLL_CONFIG_CTL_OUT(v)      \
        out_dword(HWIO_MSS_MPSS_PLL_CONFIG_CTL_ADDR,v)
#define HWIO_MSS_MPSS_PLL_CONFIG_CTL_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_MPSS_PLL_CONFIG_CTL_ADDR,m,v,HWIO_MSS_MPSS_PLL_CONFIG_CTL_IN)
#define HWIO_MSS_MPSS_PLL_CONFIG_CTL_FINE_LDC_THRESHOLD_BMSK                        0xfc000000
#define HWIO_MSS_MPSS_PLL_CONFIG_CTL_FINE_LDC_THRESHOLD_SHFT                              0x1a
#define HWIO_MSS_MPSS_PLL_CONFIG_CTL_COARSE_LDC_THRESHOLD_BMSK                       0x3c00000
#define HWIO_MSS_MPSS_PLL_CONFIG_CTL_COARSE_LDC_THRESHOLD_SHFT                            0x16
#define HWIO_MSS_MPSS_PLL_CONFIG_CTL_COARSE_LOCK_DET_NEG_THRESHOLD_BMSK               0x3c0000
#define HWIO_MSS_MPSS_PLL_CONFIG_CTL_COARSE_LOCK_DET_NEG_THRESHOLD_SHFT                   0x12
#define HWIO_MSS_MPSS_PLL_CONFIG_CTL_COARSE_LOCK_DET_POS_THRESHOLD_BMSK                0x3c000
#define HWIO_MSS_MPSS_PLL_CONFIG_CTL_COARSE_LOCK_DET_POS_THRESHOLD_SHFT                    0xe
#define HWIO_MSS_MPSS_PLL_CONFIG_CTL_FINE_LOCK_DET_THRESHOLD_BMSK                       0x3800
#define HWIO_MSS_MPSS_PLL_CONFIG_CTL_FINE_LOCK_DET_THRESHOLD_SHFT                          0xb
#define HWIO_MSS_MPSS_PLL_CONFIG_CTL_FINE_LOCK_DET_SAMPLE_SIZE_BMSK                      0x700
#define HWIO_MSS_MPSS_PLL_CONFIG_CTL_FINE_LOCK_DET_SAMPLE_SIZE_SHFT                        0x8
#define HWIO_MSS_MPSS_PLL_CONFIG_CTL_FWD_GAIN_SLEWING_KFN_BMSK                            0xf0
#define HWIO_MSS_MPSS_PLL_CONFIG_CTL_FWD_GAIN_SLEWING_KFN_SHFT                             0x4
#define HWIO_MSS_MPSS_PLL_CONFIG_CTL_FWD_GAIN_KFN_BMSK                                     0xf
#define HWIO_MSS_MPSS_PLL_CONFIG_CTL_FWD_GAIN_KFN_SHFT                                     0x0

#define HWIO_MSS_MPSS_PLL_CONFIG_CTL_U_ADDR                                         (MSS_MSS_CC_REG_REG_BASE      + 0x0000011c)
#define HWIO_MSS_MPSS_PLL_CONFIG_CTL_U_OFFS                                         (MSS_MSS_CC_REG_REG_BASE_OFFS + 0x0000011c)
#define HWIO_MSS_MPSS_PLL_CONFIG_CTL_U_RMSK                                         0xffffffff
#define HWIO_MSS_MPSS_PLL_CONFIG_CTL_U_IN          \
        in_dword_masked(HWIO_MSS_MPSS_PLL_CONFIG_CTL_U_ADDR, HWIO_MSS_MPSS_PLL_CONFIG_CTL_U_RMSK)
#define HWIO_MSS_MPSS_PLL_CONFIG_CTL_U_INM(m)      \
        in_dword_masked(HWIO_MSS_MPSS_PLL_CONFIG_CTL_U_ADDR, m)
#define HWIO_MSS_MPSS_PLL_CONFIG_CTL_U_OUT(v)      \
        out_dword(HWIO_MSS_MPSS_PLL_CONFIG_CTL_U_ADDR,v)
#define HWIO_MSS_MPSS_PLL_CONFIG_CTL_U_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_MPSS_PLL_CONFIG_CTL_U_ADDR,m,v,HWIO_MSS_MPSS_PLL_CONFIG_CTL_U_IN)
#define HWIO_MSS_MPSS_PLL_CONFIG_CTL_U_BIST_CFG_BMSK                                0xfff00000
#define HWIO_MSS_MPSS_PLL_CONFIG_CTL_U_BIST_CFG_SHFT                                      0x14
#define HWIO_MSS_MPSS_PLL_CONFIG_CTL_U_FORCE_DAC_ON_BMSK                               0x30000
#define HWIO_MSS_MPSS_PLL_CONFIG_CTL_U_FORCE_DAC_ON_SHFT                                  0x10
#define HWIO_MSS_MPSS_PLL_CONFIG_CTL_U_OSC_SETTLING_TIME_BMSK                           0x3000
#define HWIO_MSS_MPSS_PLL_CONFIG_CTL_U_OSC_SETTLING_TIME_SHFT                              0xc
#define HWIO_MSS_MPSS_PLL_CONFIG_CTL_U_NUM_STEPS_FAST_LOCK_BMSK                          0xc00
#define HWIO_MSS_MPSS_PLL_CONFIG_CTL_U_NUM_STEPS_FAST_LOCK_SHFT                            0xa
#define HWIO_MSS_MPSS_PLL_CONFIG_CTL_U_PREDIV_WHEN_CAL_BMSK                              0x380
#define HWIO_MSS_MPSS_PLL_CONFIG_CTL_U_PREDIV_WHEN_CAL_SHFT                                0x7
#define HWIO_MSS_MPSS_PLL_CONFIG_CTL_U_INC_MIN_GLITCH_THRESHOLD_4X_BMSK                   0x40
#define HWIO_MSS_MPSS_PLL_CONFIG_CTL_U_INC_MIN_GLITCH_THRESHOLD_4X_SHFT                    0x6
#define HWIO_MSS_MPSS_PLL_CONFIG_CTL_U_MIN_GLITCH_THRESHOLD_BMSK                          0x30
#define HWIO_MSS_MPSS_PLL_CONFIG_CTL_U_MIN_GLITCH_THRESHOLD_SHFT                           0x4
#define HWIO_MSS_MPSS_PLL_CONFIG_CTL_U_USE_BOTH_REF_CLK_EDGE_BMSK                          0x8
#define HWIO_MSS_MPSS_PLL_CONFIG_CTL_U_USE_BOTH_REF_CLK_EDGE_SHFT                          0x3
#define HWIO_MSS_MPSS_PLL_CONFIG_CTL_U_BIAS_WARMUP_TIME_BMSK                               0x6
#define HWIO_MSS_MPSS_PLL_CONFIG_CTL_U_BIAS_WARMUP_TIME_SHFT                               0x1
#define HWIO_MSS_MPSS_PLL_CONFIG_CTL_U_DIV_LOCK_DET_THRESHOLDS_BMSK                        0x1
#define HWIO_MSS_MPSS_PLL_CONFIG_CTL_U_DIV_LOCK_DET_THRESHOLDS_SHFT                        0x0

#define HWIO_MSS_MPSS_PLL_CONFIG_CTL_U1_ADDR                                        (MSS_MSS_CC_REG_REG_BASE      + 0x00000120)
#define HWIO_MSS_MPSS_PLL_CONFIG_CTL_U1_OFFS                                        (MSS_MSS_CC_REG_REG_BASE_OFFS + 0x00000120)
#define HWIO_MSS_MPSS_PLL_CONFIG_CTL_U1_RMSK                                        0xffffffff
#define HWIO_MSS_MPSS_PLL_CONFIG_CTL_U1_IN          \
        in_dword_masked(HWIO_MSS_MPSS_PLL_CONFIG_CTL_U1_ADDR, HWIO_MSS_MPSS_PLL_CONFIG_CTL_U1_RMSK)
#define HWIO_MSS_MPSS_PLL_CONFIG_CTL_U1_INM(m)      \
        in_dword_masked(HWIO_MSS_MPSS_PLL_CONFIG_CTL_U1_ADDR, m)
#define HWIO_MSS_MPSS_PLL_CONFIG_CTL_U1_OUT(v)      \
        out_dword(HWIO_MSS_MPSS_PLL_CONFIG_CTL_U1_ADDR,v)
#define HWIO_MSS_MPSS_PLL_CONFIG_CTL_U1_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_MPSS_PLL_CONFIG_CTL_U1_ADDR,m,v,HWIO_MSS_MPSS_PLL_CONFIG_CTL_U1_IN)

#define HWIO_MSS_MPSS_PLL_TEST_CTL_ADDR                                             (MSS_MSS_CC_REG_REG_BASE      + 0x00000124)
#define HWIO_MSS_MPSS_PLL_TEST_CTL_OFFS                                             (MSS_MSS_CC_REG_REG_BASE_OFFS + 0x00000124)
#define HWIO_MSS_MPSS_PLL_TEST_CTL_RMSK                                             0xffffffff
#define HWIO_MSS_MPSS_PLL_TEST_CTL_IN          \
        in_dword_masked(HWIO_MSS_MPSS_PLL_TEST_CTL_ADDR, HWIO_MSS_MPSS_PLL_TEST_CTL_RMSK)
#define HWIO_MSS_MPSS_PLL_TEST_CTL_INM(m)      \
        in_dword_masked(HWIO_MSS_MPSS_PLL_TEST_CTL_ADDR, m)
#define HWIO_MSS_MPSS_PLL_TEST_CTL_OUT(v)      \
        out_dword(HWIO_MSS_MPSS_PLL_TEST_CTL_ADDR,v)
#define HWIO_MSS_MPSS_PLL_TEST_CTL_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_MPSS_PLL_TEST_CTL_ADDR,m,v,HWIO_MSS_MPSS_PLL_TEST_CTL_IN)
#define HWIO_MSS_MPSS_PLL_TEST_CTL_ADD_REF_CYC_CALIB_STEPS_BMSK                     0xc0000000
#define HWIO_MSS_MPSS_PLL_TEST_CTL_ADD_REF_CYC_CALIB_STEPS_SHFT                           0x1e
#define HWIO_MSS_MPSS_PLL_TEST_CTL_GLITCH_DETECTOR_COUNT_LIMIT_BMSK                 0x30000000
#define HWIO_MSS_MPSS_PLL_TEST_CTL_GLITCH_DETECTOR_COUNT_LIMIT_SHFT                       0x1c
#define HWIO_MSS_MPSS_PLL_TEST_CTL_GLITCH_PREVENTION_DIS_BMSK                        0x8000000
#define HWIO_MSS_MPSS_PLL_TEST_CTL_GLITCH_PREVENTION_DIS_SHFT                             0x1b
#define HWIO_MSS_MPSS_PLL_TEST_CTL_FINE_FCW_BMSK                                     0x7e00000
#define HWIO_MSS_MPSS_PLL_TEST_CTL_FINE_FCW_SHFT                                          0x15
#define HWIO_MSS_MPSS_PLL_TEST_CTL_OVERRIDE_FINE_FCW_BMSK                             0x100000
#define HWIO_MSS_MPSS_PLL_TEST_CTL_OVERRIDE_FINE_FCW_SHFT                                 0x14
#define HWIO_MSS_MPSS_PLL_TEST_CTL_COARSE_FCW_BMSK                                     0xff000
#define HWIO_MSS_MPSS_PLL_TEST_CTL_COARSE_FCW_SHFT                                         0xc
#define HWIO_MSS_MPSS_PLL_TEST_CTL_OVERRIDE_COARSE_FCW_BMSK                              0x800
#define HWIO_MSS_MPSS_PLL_TEST_CTL_OVERRIDE_COARSE_FCW_SHFT                                0xb
#define HWIO_MSS_MPSS_PLL_TEST_CTL_PROCESS_CALIB_WORD_BMSK                               0x700
#define HWIO_MSS_MPSS_PLL_TEST_CTL_PROCESS_CALIB_WORD_SHFT                                 0x8
#define HWIO_MSS_MPSS_PLL_TEST_CTL_OVERRIDE_CALIB_WORD_BMSK                               0x80
#define HWIO_MSS_MPSS_PLL_TEST_CTL_OVERRIDE_CALIB_WORD_SHFT                                0x7
#define HWIO_MSS_MPSS_PLL_TEST_CTL_DISABLE_LFSR_BMSK                                      0x40
#define HWIO_MSS_MPSS_PLL_TEST_CTL_DISABLE_LFSR_SHFT                                       0x6
#define HWIO_MSS_MPSS_PLL_TEST_CTL_EN_VREF_TEST_BMSK                                      0x20
#define HWIO_MSS_MPSS_PLL_TEST_CTL_EN_VREF_TEST_SHFT                                       0x5
#define HWIO_MSS_MPSS_PLL_TEST_CTL_EN_VDCO_TEST_BMSK                                      0x10
#define HWIO_MSS_MPSS_PLL_TEST_CTL_EN_VDCO_TEST_SHFT                                       0x4
#define HWIO_MSS_MPSS_PLL_TEST_CTL_BYPASS_ATEST_BUFFER_BMSK                                0x8
#define HWIO_MSS_MPSS_PLL_TEST_CTL_BYPASS_ATEST_BUFFER_SHFT                                0x3
#define HWIO_MSS_MPSS_PLL_TEST_CTL_EN_IVCO_TEST_BMSK                                       0x4
#define HWIO_MSS_MPSS_PLL_TEST_CTL_EN_IVCO_TEST_SHFT                                       0x2
#define HWIO_MSS_MPSS_PLL_TEST_CTL_ATEST1_EN_BMSK                                          0x2
#define HWIO_MSS_MPSS_PLL_TEST_CTL_ATEST1_EN_SHFT                                          0x1
#define HWIO_MSS_MPSS_PLL_TEST_CTL_ATEST0_EN_BMSK                                          0x1
#define HWIO_MSS_MPSS_PLL_TEST_CTL_ATEST0_EN_SHFT                                          0x0

#define HWIO_MSS_MPSS_PLL_TEST_CTL_U_ADDR                                           (MSS_MSS_CC_REG_REG_BASE      + 0x00000128)
#define HWIO_MSS_MPSS_PLL_TEST_CTL_U_OFFS                                           (MSS_MSS_CC_REG_REG_BASE_OFFS + 0x00000128)
#define HWIO_MSS_MPSS_PLL_TEST_CTL_U_RMSK                                           0xffffffff
#define HWIO_MSS_MPSS_PLL_TEST_CTL_U_IN          \
        in_dword_masked(HWIO_MSS_MPSS_PLL_TEST_CTL_U_ADDR, HWIO_MSS_MPSS_PLL_TEST_CTL_U_RMSK)
#define HWIO_MSS_MPSS_PLL_TEST_CTL_U_INM(m)      \
        in_dword_masked(HWIO_MSS_MPSS_PLL_TEST_CTL_U_ADDR, m)
#define HWIO_MSS_MPSS_PLL_TEST_CTL_U_OUT(v)      \
        out_dword(HWIO_MSS_MPSS_PLL_TEST_CTL_U_ADDR,v)
#define HWIO_MSS_MPSS_PLL_TEST_CTL_U_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_MPSS_PLL_TEST_CTL_U_ADDR,m,v,HWIO_MSS_MPSS_PLL_TEST_CTL_U_IN)
#define HWIO_MSS_MPSS_PLL_TEST_CTL_U_ENABLE_TRAINING_SEQ_BMSK                       0x80000000
#define HWIO_MSS_MPSS_PLL_TEST_CTL_U_ENABLE_TRAINING_SEQ_SHFT                             0x1f
#define HWIO_MSS_MPSS_PLL_TEST_CTL_U_BYPASS_LOGIC_DEP_BMSK                          0x40000000
#define HWIO_MSS_MPSS_PLL_TEST_CTL_U_BYPASS_LOGIC_DEP_SHFT                                0x1e
#define HWIO_MSS_MPSS_PLL_TEST_CTL_U_BYPASS_BIAS_DET_BMSK                           0x20000000
#define HWIO_MSS_MPSS_PLL_TEST_CTL_U_BYPASS_BIAS_DET_SHFT                                 0x1d
#define HWIO_MSS_MPSS_PLL_TEST_CTL_U_MSB_CLOCK_SELECT_BMSK                          0x10000000
#define HWIO_MSS_MPSS_PLL_TEST_CTL_U_MSB_CLOCK_SELECT_SHFT                                0x1c
#define HWIO_MSS_MPSS_PLL_TEST_CTL_U_BYPASS_FIRST_ORDER_DSM_BMSK                     0x8000000
#define HWIO_MSS_MPSS_PLL_TEST_CTL_U_BYPASS_FIRST_ORDER_DSM_SHFT                          0x1b
#define HWIO_MSS_MPSS_PLL_TEST_CTL_U_DCO_OFFSET_CURRENT_ADJUST_BMSK                  0x6000000
#define HWIO_MSS_MPSS_PLL_TEST_CTL_U_DCO_OFFSET_CURRENT_ADJUST_SHFT                       0x19
#define HWIO_MSS_MPSS_PLL_TEST_CTL_U_MIRROR_DEVICE_ADJUST_BMSK                       0x1c00000
#define HWIO_MSS_MPSS_PLL_TEST_CTL_U_MIRROR_DEVICE_ADJUST_SHFT                            0x16
#define HWIO_MSS_MPSS_PLL_TEST_CTL_U_BIAS_STARTUP_CIR_DIS_BMSK                        0x200000
#define HWIO_MSS_MPSS_PLL_TEST_CTL_U_BIAS_STARTUP_CIR_DIS_SHFT                            0x15
#define HWIO_MSS_MPSS_PLL_TEST_CTL_U_DISABLE_CLAMP_BMSK                               0x100000
#define HWIO_MSS_MPSS_PLL_TEST_CTL_U_DISABLE_CLAMP_SHFT                                   0x14
#define HWIO_MSS_MPSS_PLL_TEST_CTL_U_BYPASS_MODE_OF_BIAS_BMSK                          0x80000
#define HWIO_MSS_MPSS_PLL_TEST_CTL_U_BYPASS_MODE_OF_BIAS_SHFT                             0x13
#define HWIO_MSS_MPSS_PLL_TEST_CTL_U_BYPASS_MODE_FOR_BIAS_EN_BMSK                      0x40000
#define HWIO_MSS_MPSS_PLL_TEST_CTL_U_BYPASS_MODE_FOR_BIAS_EN_SHFT                         0x12
#define HWIO_MSS_MPSS_PLL_TEST_CTL_U_BIAS_ADJUST_BMSK                                  0x30000
#define HWIO_MSS_MPSS_PLL_TEST_CTL_U_BIAS_ADJUST_SHFT                                     0x10
#define HWIO_MSS_MPSS_PLL_TEST_CTL_U_DIV2_NMO_EN_BMSK                                   0x8000
#define HWIO_MSS_MPSS_PLL_TEST_CTL_U_DIV2_NMO_EN_SHFT                                      0xf
#define HWIO_MSS_MPSS_PLL_TEST_CTL_U_DIS_AMP_STARTUP_BMSK                               0x4000
#define HWIO_MSS_MPSS_PLL_TEST_CTL_U_DIS_AMP_STARTUP_SHFT                                  0xe
#define HWIO_MSS_MPSS_PLL_TEST_CTL_U_SINGLE_DMET_EN_BMSK                                0x2000
#define HWIO_MSS_MPSS_PLL_TEST_CTL_U_SINGLE_DMET_EN_SHFT                                   0xd
#define HWIO_MSS_MPSS_PLL_TEST_CTL_U_DEMET_WINDOW_DIS_BMSK                              0x1000
#define HWIO_MSS_MPSS_PLL_TEST_CTL_U_DEMET_WINDOW_DIS_SHFT                                 0xc
#define HWIO_MSS_MPSS_PLL_TEST_CTL_U_NMO_OSC_SEL_BMSK                                    0xc00
#define HWIO_MSS_MPSS_PLL_TEST_CTL_U_NMO_OSC_SEL_SHFT                                      0xa
#define HWIO_MSS_MPSS_PLL_TEST_CTL_U_NOISE_MAG_BMSK                                      0x380
#define HWIO_MSS_MPSS_PLL_TEST_CTL_U_NOISE_MAG_SHFT                                        0x7
#define HWIO_MSS_MPSS_PLL_TEST_CTL_U_NOISE_GEN_EN_BMSK                                    0x40
#define HWIO_MSS_MPSS_PLL_TEST_CTL_U_NOISE_GEN_EN_SHFT                                     0x6
#define HWIO_MSS_MPSS_PLL_TEST_CTL_U_OSC_BIAS_GND_BMSK                                    0x20
#define HWIO_MSS_MPSS_PLL_TEST_CTL_U_OSC_BIAS_GND_SHFT                                     0x5
#define HWIO_MSS_MPSS_PLL_TEST_CTL_U_PLL_TEST_OUT_SEL_BMSK                                0x18
#define HWIO_MSS_MPSS_PLL_TEST_CTL_U_PLL_TEST_OUT_SEL_SHFT                                 0x3
#define HWIO_MSS_MPSS_PLL_TEST_CTL_U_STATUS_REG_EN_BMSK                                    0x4
#define HWIO_MSS_MPSS_PLL_TEST_CTL_U_STATUS_REG_EN_SHFT                                    0x2
#define HWIO_MSS_MPSS_PLL_TEST_CTL_U_SKIP_FINE_TUNE_BMSK                                   0x2
#define HWIO_MSS_MPSS_PLL_TEST_CTL_U_SKIP_FINE_TUNE_SHFT                                   0x1
#define HWIO_MSS_MPSS_PLL_TEST_CTL_U_SKIP_FAST_LOCK_BMSK                                   0x1
#define HWIO_MSS_MPSS_PLL_TEST_CTL_U_SKIP_FAST_LOCK_SHFT                                   0x0

#define HWIO_MSS_MPSS_PLL_TEST_CTL_U1_ADDR                                          (MSS_MSS_CC_REG_REG_BASE      + 0x0000012c)
#define HWIO_MSS_MPSS_PLL_TEST_CTL_U1_OFFS                                          (MSS_MSS_CC_REG_REG_BASE_OFFS + 0x0000012c)
#define HWIO_MSS_MPSS_PLL_TEST_CTL_U1_RMSK                                          0xffffffff
#define HWIO_MSS_MPSS_PLL_TEST_CTL_U1_IN          \
        in_dword_masked(HWIO_MSS_MPSS_PLL_TEST_CTL_U1_ADDR, HWIO_MSS_MPSS_PLL_TEST_CTL_U1_RMSK)
#define HWIO_MSS_MPSS_PLL_TEST_CTL_U1_INM(m)      \
        in_dword_masked(HWIO_MSS_MPSS_PLL_TEST_CTL_U1_ADDR, m)
#define HWIO_MSS_MPSS_PLL_TEST_CTL_U1_OUT(v)      \
        out_dword(HWIO_MSS_MPSS_PLL_TEST_CTL_U1_ADDR,v)
#define HWIO_MSS_MPSS_PLL_TEST_CTL_U1_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_MPSS_PLL_TEST_CTL_U1_ADDR,m,v,HWIO_MSS_MPSS_PLL_TEST_CTL_U1_IN)

#define HWIO_MSS_MPSS_PLL_STATUS_ADDR                                               (MSS_MSS_CC_REG_REG_BASE      + 0x00000130)
#define HWIO_MSS_MPSS_PLL_STATUS_OFFS                                               (MSS_MSS_CC_REG_REG_BASE_OFFS + 0x00000130)
#define HWIO_MSS_MPSS_PLL_STATUS_RMSK                                               0xffffffff
#define HWIO_MSS_MPSS_PLL_STATUS_IN          \
        in_dword_masked(HWIO_MSS_MPSS_PLL_STATUS_ADDR, HWIO_MSS_MPSS_PLL_STATUS_RMSK)
#define HWIO_MSS_MPSS_PLL_STATUS_INM(m)      \
        in_dword_masked(HWIO_MSS_MPSS_PLL_STATUS_ADDR, m)
#define HWIO_MSS_MPSS_PLL_STATUS_STATUS_31_0_BMSK                                   0xffffffff
#define HWIO_MSS_MPSS_PLL_STATUS_STATUS_31_0_SHFT                                          0x0

#define HWIO_MSS_MPSS_PLL_FREQ_CTL_ADDR                                             (MSS_MSS_CC_REG_REG_BASE      + 0x00000134)
#define HWIO_MSS_MPSS_PLL_FREQ_CTL_OFFS                                             (MSS_MSS_CC_REG_REG_BASE_OFFS + 0x00000134)
#define HWIO_MSS_MPSS_PLL_FREQ_CTL_RMSK                                             0xffffffff
#define HWIO_MSS_MPSS_PLL_FREQ_CTL_IN          \
        in_dword_masked(HWIO_MSS_MPSS_PLL_FREQ_CTL_ADDR, HWIO_MSS_MPSS_PLL_FREQ_CTL_RMSK)
#define HWIO_MSS_MPSS_PLL_FREQ_CTL_INM(m)      \
        in_dword_masked(HWIO_MSS_MPSS_PLL_FREQ_CTL_ADDR, m)
#define HWIO_MSS_MPSS_PLL_FREQ_CTL_OUT(v)      \
        out_dword(HWIO_MSS_MPSS_PLL_FREQ_CTL_ADDR,v)
#define HWIO_MSS_MPSS_PLL_FREQ_CTL_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_MPSS_PLL_FREQ_CTL_ADDR,m,v,HWIO_MSS_MPSS_PLL_FREQ_CTL_IN)
#define HWIO_MSS_MPSS_PLL_FREQ_CTL_FREQUENCY_CTL_WORD_BMSK                          0xffffffff
#define HWIO_MSS_MPSS_PLL_FREQ_CTL_FREQUENCY_CTL_WORD_SHFT                                 0x0

#define HWIO_MSS_MPSS_PLL_OPMODE_ADDR                                               (MSS_MSS_CC_REG_REG_BASE      + 0x00000138)
#define HWIO_MSS_MPSS_PLL_OPMODE_OFFS                                               (MSS_MSS_CC_REG_REG_BASE_OFFS + 0x00000138)
#define HWIO_MSS_MPSS_PLL_OPMODE_RMSK                                                      0x7
#define HWIO_MSS_MPSS_PLL_OPMODE_IN          \
        in_dword_masked(HWIO_MSS_MPSS_PLL_OPMODE_ADDR, HWIO_MSS_MPSS_PLL_OPMODE_RMSK)
#define HWIO_MSS_MPSS_PLL_OPMODE_INM(m)      \
        in_dword_masked(HWIO_MSS_MPSS_PLL_OPMODE_ADDR, m)
#define HWIO_MSS_MPSS_PLL_OPMODE_OUT(v)      \
        out_dword(HWIO_MSS_MPSS_PLL_OPMODE_ADDR,v)
#define HWIO_MSS_MPSS_PLL_OPMODE_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_MPSS_PLL_OPMODE_ADDR,m,v,HWIO_MSS_MPSS_PLL_OPMODE_IN)
#define HWIO_MSS_MPSS_PLL_OPMODE_PLL_OPMODE_BMSK                                           0x7
#define HWIO_MSS_MPSS_PLL_OPMODE_PLL_OPMODE_SHFT                                           0x0

#define HWIO_MSS_MPSS_PLL_STATE_ADDR                                                (MSS_MSS_CC_REG_REG_BASE      + 0x0000013c)
#define HWIO_MSS_MPSS_PLL_STATE_OFFS                                                (MSS_MSS_CC_REG_REG_BASE_OFFS + 0x0000013c)
#define HWIO_MSS_MPSS_PLL_STATE_RMSK                                                       0x7
#define HWIO_MSS_MPSS_PLL_STATE_IN          \
        in_dword_masked(HWIO_MSS_MPSS_PLL_STATE_ADDR, HWIO_MSS_MPSS_PLL_STATE_RMSK)
#define HWIO_MSS_MPSS_PLL_STATE_INM(m)      \
        in_dword_masked(HWIO_MSS_MPSS_PLL_STATE_ADDR, m)
#define HWIO_MSS_MPSS_PLL_STATE_PLL_STATE_BMSK                                             0x7
#define HWIO_MSS_MPSS_PLL_STATE_PLL_STATE_SHFT                                             0x0

#define HWIO_MSS_MPSS_PLL_ALPHA_VAL_ADDR                                            (MSS_MSS_CC_REG_REG_BASE      + 0x00000140)
#define HWIO_MSS_MPSS_PLL_ALPHA_VAL_OFFS                                            (MSS_MSS_CC_REG_REG_BASE_OFFS + 0x00000140)
#define HWIO_MSS_MPSS_PLL_ALPHA_VAL_RMSK                                                0xffff
#define HWIO_MSS_MPSS_PLL_ALPHA_VAL_IN          \
        in_dword_masked(HWIO_MSS_MPSS_PLL_ALPHA_VAL_ADDR, HWIO_MSS_MPSS_PLL_ALPHA_VAL_RMSK)
#define HWIO_MSS_MPSS_PLL_ALPHA_VAL_INM(m)      \
        in_dword_masked(HWIO_MSS_MPSS_PLL_ALPHA_VAL_ADDR, m)
#define HWIO_MSS_MPSS_PLL_ALPHA_VAL_OUT(v)      \
        out_dword(HWIO_MSS_MPSS_PLL_ALPHA_VAL_ADDR,v)
#define HWIO_MSS_MPSS_PLL_ALPHA_VAL_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_MPSS_PLL_ALPHA_VAL_ADDR,m,v,HWIO_MSS_MPSS_PLL_ALPHA_VAL_IN)
#define HWIO_MSS_MPSS_PLL_ALPHA_VAL_PLL_ALPHA_VAL_BMSK                                  0xffff
#define HWIO_MSS_MPSS_PLL_ALPHA_VAL_PLL_ALPHA_VAL_SHFT                                     0x0

#define HWIO_MSS_MPSS_PLL_SPARE_ADDR                                                (MSS_MSS_CC_REG_REG_BASE      + 0x00000144)
#define HWIO_MSS_MPSS_PLL_SPARE_OFFS                                                (MSS_MSS_CC_REG_REG_BASE_OFFS + 0x00000144)
#define HWIO_MSS_MPSS_PLL_SPARE_RMSK                                                      0xff
#define HWIO_MSS_MPSS_PLL_SPARE_IN          \
        in_dword_masked(HWIO_MSS_MPSS_PLL_SPARE_ADDR, HWIO_MSS_MPSS_PLL_SPARE_RMSK)
#define HWIO_MSS_MPSS_PLL_SPARE_INM(m)      \
        in_dword_masked(HWIO_MSS_MPSS_PLL_SPARE_ADDR, m)
#define HWIO_MSS_MPSS_PLL_SPARE_OUT(v)      \
        out_dword(HWIO_MSS_MPSS_PLL_SPARE_ADDR,v)
#define HWIO_MSS_MPSS_PLL_SPARE_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_MPSS_PLL_SPARE_ADDR,m,v,HWIO_MSS_MPSS_PLL_SPARE_IN)
#define HWIO_MSS_MPSS_PLL_SPARE_SPARE_OUTPUTS_BMSK                                        0xf0
#define HWIO_MSS_MPSS_PLL_SPARE_SPARE_OUTPUTS_SHFT                                         0x4
#define HWIO_MSS_MPSS_PLL_SPARE_SPARE_INPUTS_BMSK                                          0xf
#define HWIO_MSS_MPSS_PLL_SPARE_SPARE_INPUTS_SHFT                                          0x0

#define HWIO_MSS_MPSS_PLL_SSC_DELTA_ALPHA_ADDR                                      (MSS_MSS_CC_REG_REG_BASE      + 0x00000148)
#define HWIO_MSS_MPSS_PLL_SSC_DELTA_ALPHA_OFFS                                      (MSS_MSS_CC_REG_REG_BASE_OFFS + 0x00000148)
#define HWIO_MSS_MPSS_PLL_SSC_DELTA_ALPHA_RMSK                                            0xff
#define HWIO_MSS_MPSS_PLL_SSC_DELTA_ALPHA_IN          \
        in_dword_masked(HWIO_MSS_MPSS_PLL_SSC_DELTA_ALPHA_ADDR, HWIO_MSS_MPSS_PLL_SSC_DELTA_ALPHA_RMSK)
#define HWIO_MSS_MPSS_PLL_SSC_DELTA_ALPHA_INM(m)      \
        in_dword_masked(HWIO_MSS_MPSS_PLL_SSC_DELTA_ALPHA_ADDR, m)
#define HWIO_MSS_MPSS_PLL_SSC_DELTA_ALPHA_OUT(v)      \
        out_dword(HWIO_MSS_MPSS_PLL_SSC_DELTA_ALPHA_ADDR,v)
#define HWIO_MSS_MPSS_PLL_SSC_DELTA_ALPHA_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_MPSS_PLL_SSC_DELTA_ALPHA_ADDR,m,v,HWIO_MSS_MPSS_PLL_SSC_DELTA_ALPHA_IN)
#define HWIO_MSS_MPSS_PLL_SSC_DELTA_ALPHA_SSC_DELTA_ALPHA_BMSK                            0xff
#define HWIO_MSS_MPSS_PLL_SSC_DELTA_ALPHA_SSC_DELTA_ALPHA_SHFT                             0x0

#define HWIO_MSS_MPSS_PLL_SSC_UPDATE_RATE_ADDR                                      (MSS_MSS_CC_REG_REG_BASE      + 0x0000014c)
#define HWIO_MSS_MPSS_PLL_SSC_UPDATE_RATE_OFFS                                      (MSS_MSS_CC_REG_REG_BASE_OFFS + 0x0000014c)
#define HWIO_MSS_MPSS_PLL_SSC_UPDATE_RATE_RMSK                                             0xf
#define HWIO_MSS_MPSS_PLL_SSC_UPDATE_RATE_IN          \
        in_dword_masked(HWIO_MSS_MPSS_PLL_SSC_UPDATE_RATE_ADDR, HWIO_MSS_MPSS_PLL_SSC_UPDATE_RATE_RMSK)
#define HWIO_MSS_MPSS_PLL_SSC_UPDATE_RATE_INM(m)      \
        in_dword_masked(HWIO_MSS_MPSS_PLL_SSC_UPDATE_RATE_ADDR, m)
#define HWIO_MSS_MPSS_PLL_SSC_UPDATE_RATE_OUT(v)      \
        out_dword(HWIO_MSS_MPSS_PLL_SSC_UPDATE_RATE_ADDR,v)
#define HWIO_MSS_MPSS_PLL_SSC_UPDATE_RATE_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_MPSS_PLL_SSC_UPDATE_RATE_ADDR,m,v,HWIO_MSS_MPSS_PLL_SSC_UPDATE_RATE_IN)
#define HWIO_MSS_MPSS_PLL_SSC_UPDATE_RATE_SSC_UPDATE_RATE_BMSK                             0xf
#define HWIO_MSS_MPSS_PLL_SSC_UPDATE_RATE_SSC_UPDATE_RATE_SHFT                             0x0

#define HWIO_MSS_MPSS_PLL_SSC_NUM_STEPS_ADDR                                        (MSS_MSS_CC_REG_REG_BASE      + 0x00000150)
#define HWIO_MSS_MPSS_PLL_SSC_NUM_STEPS_OFFS                                        (MSS_MSS_CC_REG_REG_BASE_OFFS + 0x00000150)
#define HWIO_MSS_MPSS_PLL_SSC_NUM_STEPS_RMSK                                               0xf
#define HWIO_MSS_MPSS_PLL_SSC_NUM_STEPS_IN          \
        in_dword_masked(HWIO_MSS_MPSS_PLL_SSC_NUM_STEPS_ADDR, HWIO_MSS_MPSS_PLL_SSC_NUM_STEPS_RMSK)
#define HWIO_MSS_MPSS_PLL_SSC_NUM_STEPS_INM(m)      \
        in_dword_masked(HWIO_MSS_MPSS_PLL_SSC_NUM_STEPS_ADDR, m)
#define HWIO_MSS_MPSS_PLL_SSC_NUM_STEPS_OUT(v)      \
        out_dword(HWIO_MSS_MPSS_PLL_SSC_NUM_STEPS_ADDR,v)
#define HWIO_MSS_MPSS_PLL_SSC_NUM_STEPS_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_MPSS_PLL_SSC_NUM_STEPS_ADDR,m,v,HWIO_MSS_MPSS_PLL_SSC_NUM_STEPS_IN)
#define HWIO_MSS_MPSS_PLL_SSC_NUM_STEPS_SSC_NUM_STEPS_BMSK                                 0xf
#define HWIO_MSS_MPSS_PLL_SSC_NUM_STEPS_SSC_NUM_STEPS_SHFT                                 0x0

#define HWIO_MSS_CLK_XO_CX_CBCR_ADDR                                                (MSS_MSS_CC_REG_REG_BASE      + 0x00000404)
#define HWIO_MSS_CLK_XO_CX_CBCR_OFFS                                                (MSS_MSS_CC_REG_REG_BASE_OFFS + 0x00000404)
#define HWIO_MSS_CLK_XO_CX_CBCR_RMSK                                                0x80000005
#define HWIO_MSS_CLK_XO_CX_CBCR_IN          \
        in_dword_masked(HWIO_MSS_CLK_XO_CX_CBCR_ADDR, HWIO_MSS_CLK_XO_CX_CBCR_RMSK)
#define HWIO_MSS_CLK_XO_CX_CBCR_INM(m)      \
        in_dword_masked(HWIO_MSS_CLK_XO_CX_CBCR_ADDR, m)
#define HWIO_MSS_CLK_XO_CX_CBCR_OUT(v)      \
        out_dword(HWIO_MSS_CLK_XO_CX_CBCR_ADDR,v)
#define HWIO_MSS_CLK_XO_CX_CBCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_CLK_XO_CX_CBCR_ADDR,m,v,HWIO_MSS_CLK_XO_CX_CBCR_IN)
#define HWIO_MSS_CLK_XO_CX_CBCR_CLK_OFF_BMSK                                        0x80000000
#define HWIO_MSS_CLK_XO_CX_CBCR_CLK_OFF_SHFT                                              0x1f
#define HWIO_MSS_CLK_XO_CX_CBCR_CLK_ARES_BMSK                                              0x4
#define HWIO_MSS_CLK_XO_CX_CBCR_CLK_ARES_SHFT                                              0x2
#define HWIO_MSS_CLK_XO_CX_CBCR_CLK_ENABLE_BMSK                                            0x1
#define HWIO_MSS_CLK_XO_CX_CBCR_CLK_ENABLE_SHFT                                            0x0

#define HWIO_MSS_CLK_XO_RSCC_CBCR_ADDR                                              (MSS_MSS_CC_REG_REG_BASE      + 0x00000408)
#define HWIO_MSS_CLK_XO_RSCC_CBCR_OFFS                                              (MSS_MSS_CC_REG_REG_BASE_OFFS + 0x00000408)
#define HWIO_MSS_CLK_XO_RSCC_CBCR_RMSK                                              0x80000005
#define HWIO_MSS_CLK_XO_RSCC_CBCR_IN          \
        in_dword_masked(HWIO_MSS_CLK_XO_RSCC_CBCR_ADDR, HWIO_MSS_CLK_XO_RSCC_CBCR_RMSK)
#define HWIO_MSS_CLK_XO_RSCC_CBCR_INM(m)      \
        in_dword_masked(HWIO_MSS_CLK_XO_RSCC_CBCR_ADDR, m)
#define HWIO_MSS_CLK_XO_RSCC_CBCR_OUT(v)      \
        out_dword(HWIO_MSS_CLK_XO_RSCC_CBCR_ADDR,v)
#define HWIO_MSS_CLK_XO_RSCC_CBCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_CLK_XO_RSCC_CBCR_ADDR,m,v,HWIO_MSS_CLK_XO_RSCC_CBCR_IN)
#define HWIO_MSS_CLK_XO_RSCC_CBCR_CLK_OFF_BMSK                                      0x80000000
#define HWIO_MSS_CLK_XO_RSCC_CBCR_CLK_OFF_SHFT                                            0x1f
#define HWIO_MSS_CLK_XO_RSCC_CBCR_CLK_ARES_BMSK                                            0x4
#define HWIO_MSS_CLK_XO_RSCC_CBCR_CLK_ARES_SHFT                                            0x2
#define HWIO_MSS_CLK_XO_RSCC_CBCR_CLK_ENABLE_BMSK                                          0x1
#define HWIO_MSS_CLK_XO_RSCC_CBCR_CLK_ENABLE_SHFT                                          0x0

#define HWIO_MSS_CLK_AXI_OFFLINE_CBCR_ADDR                                          (MSS_MSS_CC_REG_REG_BASE      + 0x0000040c)
#define HWIO_MSS_CLK_AXI_OFFLINE_CBCR_OFFS                                          (MSS_MSS_CC_REG_REG_BASE_OFFS + 0x0000040c)
#define HWIO_MSS_CLK_AXI_OFFLINE_CBCR_RMSK                                          0x80000005
#define HWIO_MSS_CLK_AXI_OFFLINE_CBCR_IN          \
        in_dword_masked(HWIO_MSS_CLK_AXI_OFFLINE_CBCR_ADDR, HWIO_MSS_CLK_AXI_OFFLINE_CBCR_RMSK)
#define HWIO_MSS_CLK_AXI_OFFLINE_CBCR_INM(m)      \
        in_dword_masked(HWIO_MSS_CLK_AXI_OFFLINE_CBCR_ADDR, m)
#define HWIO_MSS_CLK_AXI_OFFLINE_CBCR_OUT(v)      \
        out_dword(HWIO_MSS_CLK_AXI_OFFLINE_CBCR_ADDR,v)
#define HWIO_MSS_CLK_AXI_OFFLINE_CBCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_CLK_AXI_OFFLINE_CBCR_ADDR,m,v,HWIO_MSS_CLK_AXI_OFFLINE_CBCR_IN)
#define HWIO_MSS_CLK_AXI_OFFLINE_CBCR_CLK_OFF_BMSK                                  0x80000000
#define HWIO_MSS_CLK_AXI_OFFLINE_CBCR_CLK_OFF_SHFT                                        0x1f
#define HWIO_MSS_CLK_AXI_OFFLINE_CBCR_CLK_ARES_BMSK                                        0x4
#define HWIO_MSS_CLK_AXI_OFFLINE_CBCR_CLK_ARES_SHFT                                        0x2
#define HWIO_MSS_CLK_AXI_OFFLINE_CBCR_CLK_ENABLE_BMSK                                      0x1
#define HWIO_MSS_CLK_AXI_OFFLINE_CBCR_CLK_ENABLE_SHFT                                      0x0

#define HWIO_MSS_CLK_AXI_CRYPTO_CBCR_ADDR                                           (MSS_MSS_CC_REG_REG_BASE      + 0x00000410)
#define HWIO_MSS_CLK_AXI_CRYPTO_CBCR_OFFS                                           (MSS_MSS_CC_REG_REG_BASE_OFFS + 0x00000410)
#define HWIO_MSS_CLK_AXI_CRYPTO_CBCR_RMSK                                           0x80000005
#define HWIO_MSS_CLK_AXI_CRYPTO_CBCR_IN          \
        in_dword_masked(HWIO_MSS_CLK_AXI_CRYPTO_CBCR_ADDR, HWIO_MSS_CLK_AXI_CRYPTO_CBCR_RMSK)
#define HWIO_MSS_CLK_AXI_CRYPTO_CBCR_INM(m)      \
        in_dword_masked(HWIO_MSS_CLK_AXI_CRYPTO_CBCR_ADDR, m)
#define HWIO_MSS_CLK_AXI_CRYPTO_CBCR_OUT(v)      \
        out_dword(HWIO_MSS_CLK_AXI_CRYPTO_CBCR_ADDR,v)
#define HWIO_MSS_CLK_AXI_CRYPTO_CBCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_CLK_AXI_CRYPTO_CBCR_ADDR,m,v,HWIO_MSS_CLK_AXI_CRYPTO_CBCR_IN)
#define HWIO_MSS_CLK_AXI_CRYPTO_CBCR_CLK_OFF_BMSK                                   0x80000000
#define HWIO_MSS_CLK_AXI_CRYPTO_CBCR_CLK_OFF_SHFT                                         0x1f
#define HWIO_MSS_CLK_AXI_CRYPTO_CBCR_CLK_ARES_BMSK                                         0x4
#define HWIO_MSS_CLK_AXI_CRYPTO_CBCR_CLK_ARES_SHFT                                         0x2
#define HWIO_MSS_CLK_AXI_CRYPTO_CBCR_CLK_ENABLE_BMSK                                       0x1
#define HWIO_MSS_CLK_AXI_CRYPTO_CBCR_CLK_ENABLE_SHFT                                       0x0

#define HWIO_MSS_CLK_SRC_BUS_MSS_CONFIG_REG_CMD_RCGR_ADDR                           (MSS_MSS_CC_REG_REG_BASE      + 0x00000418)
#define HWIO_MSS_CLK_SRC_BUS_MSS_CONFIG_REG_CMD_RCGR_OFFS                           (MSS_MSS_CC_REG_REG_BASE_OFFS + 0x00000418)
#define HWIO_MSS_CLK_SRC_BUS_MSS_CONFIG_REG_CMD_RCGR_RMSK                           0x80000013
#define HWIO_MSS_CLK_SRC_BUS_MSS_CONFIG_REG_CMD_RCGR_IN          \
        in_dword_masked(HWIO_MSS_CLK_SRC_BUS_MSS_CONFIG_REG_CMD_RCGR_ADDR, HWIO_MSS_CLK_SRC_BUS_MSS_CONFIG_REG_CMD_RCGR_RMSK)
#define HWIO_MSS_CLK_SRC_BUS_MSS_CONFIG_REG_CMD_RCGR_INM(m)      \
        in_dword_masked(HWIO_MSS_CLK_SRC_BUS_MSS_CONFIG_REG_CMD_RCGR_ADDR, m)
#define HWIO_MSS_CLK_SRC_BUS_MSS_CONFIG_REG_CMD_RCGR_OUT(v)      \
        out_dword(HWIO_MSS_CLK_SRC_BUS_MSS_CONFIG_REG_CMD_RCGR_ADDR,v)
#define HWIO_MSS_CLK_SRC_BUS_MSS_CONFIG_REG_CMD_RCGR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_CLK_SRC_BUS_MSS_CONFIG_REG_CMD_RCGR_ADDR,m,v,HWIO_MSS_CLK_SRC_BUS_MSS_CONFIG_REG_CMD_RCGR_IN)
#define HWIO_MSS_CLK_SRC_BUS_MSS_CONFIG_REG_CMD_RCGR_ROOT_OFF_BMSK                  0x80000000
#define HWIO_MSS_CLK_SRC_BUS_MSS_CONFIG_REG_CMD_RCGR_ROOT_OFF_SHFT                        0x1f
#define HWIO_MSS_CLK_SRC_BUS_MSS_CONFIG_REG_CMD_RCGR_DIRTY_CFG_RCGR_BMSK                  0x10
#define HWIO_MSS_CLK_SRC_BUS_MSS_CONFIG_REG_CMD_RCGR_DIRTY_CFG_RCGR_SHFT                   0x4
#define HWIO_MSS_CLK_SRC_BUS_MSS_CONFIG_REG_CMD_RCGR_ROOT_EN_BMSK                          0x2
#define HWIO_MSS_CLK_SRC_BUS_MSS_CONFIG_REG_CMD_RCGR_ROOT_EN_SHFT                          0x1
#define HWIO_MSS_CLK_SRC_BUS_MSS_CONFIG_REG_CMD_RCGR_UPDATE_BMSK                           0x1
#define HWIO_MSS_CLK_SRC_BUS_MSS_CONFIG_REG_CMD_RCGR_UPDATE_SHFT                           0x0

#define HWIO_MSS_CLK_SRC_BUS_MSS_CONFIG_REG_CFG_RCGR_ADDR                           (MSS_MSS_CC_REG_REG_BASE      + 0x0000041c)
#define HWIO_MSS_CLK_SRC_BUS_MSS_CONFIG_REG_CFG_RCGR_OFFS                           (MSS_MSS_CC_REG_REG_BASE_OFFS + 0x0000041c)
#define HWIO_MSS_CLK_SRC_BUS_MSS_CONFIG_REG_CFG_RCGR_RMSK                             0x11071f
#define HWIO_MSS_CLK_SRC_BUS_MSS_CONFIG_REG_CFG_RCGR_IN          \
        in_dword_masked(HWIO_MSS_CLK_SRC_BUS_MSS_CONFIG_REG_CFG_RCGR_ADDR, HWIO_MSS_CLK_SRC_BUS_MSS_CONFIG_REG_CFG_RCGR_RMSK)
#define HWIO_MSS_CLK_SRC_BUS_MSS_CONFIG_REG_CFG_RCGR_INM(m)      \
        in_dword_masked(HWIO_MSS_CLK_SRC_BUS_MSS_CONFIG_REG_CFG_RCGR_ADDR, m)
#define HWIO_MSS_CLK_SRC_BUS_MSS_CONFIG_REG_CFG_RCGR_OUT(v)      \
        out_dword(HWIO_MSS_CLK_SRC_BUS_MSS_CONFIG_REG_CFG_RCGR_ADDR,v)
#define HWIO_MSS_CLK_SRC_BUS_MSS_CONFIG_REG_CFG_RCGR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_CLK_SRC_BUS_MSS_CONFIG_REG_CFG_RCGR_ADDR,m,v,HWIO_MSS_CLK_SRC_BUS_MSS_CONFIG_REG_CFG_RCGR_IN)
#define HWIO_MSS_CLK_SRC_BUS_MSS_CONFIG_REG_CFG_RCGR_HW_CLK_CONTROL_BMSK              0x100000
#define HWIO_MSS_CLK_SRC_BUS_MSS_CONFIG_REG_CFG_RCGR_HW_CLK_CONTROL_SHFT                  0x14
#define HWIO_MSS_CLK_SRC_BUS_MSS_CONFIG_REG_CFG_RCGR_RCGLITE_DISABLE_BMSK              0x10000
#define HWIO_MSS_CLK_SRC_BUS_MSS_CONFIG_REG_CFG_RCGR_RCGLITE_DISABLE_SHFT                 0x10
#define HWIO_MSS_CLK_SRC_BUS_MSS_CONFIG_REG_CFG_RCGR_SRC_SEL_BMSK                        0x700
#define HWIO_MSS_CLK_SRC_BUS_MSS_CONFIG_REG_CFG_RCGR_SRC_SEL_SHFT                          0x8
#define HWIO_MSS_CLK_SRC_BUS_MSS_CONFIG_REG_CFG_RCGR_SRC_DIV_BMSK                         0x1f
#define HWIO_MSS_CLK_SRC_BUS_MSS_CONFIG_REG_CFG_RCGR_SRC_DIV_SHFT                          0x0

#define HWIO_MSS_CLK_BUS_CONFIG_CBCR_ADDR                                           (MSS_MSS_CC_REG_REG_BASE      + 0x00000420)
#define HWIO_MSS_CLK_BUS_CONFIG_CBCR_OFFS                                           (MSS_MSS_CC_REG_REG_BASE_OFFS + 0x00000420)
#define HWIO_MSS_CLK_BUS_CONFIG_CBCR_RMSK                                           0x80000004
#define HWIO_MSS_CLK_BUS_CONFIG_CBCR_IN          \
        in_dword_masked(HWIO_MSS_CLK_BUS_CONFIG_CBCR_ADDR, HWIO_MSS_CLK_BUS_CONFIG_CBCR_RMSK)
#define HWIO_MSS_CLK_BUS_CONFIG_CBCR_INM(m)      \
        in_dword_masked(HWIO_MSS_CLK_BUS_CONFIG_CBCR_ADDR, m)
#define HWIO_MSS_CLK_BUS_CONFIG_CBCR_OUT(v)      \
        out_dword(HWIO_MSS_CLK_BUS_CONFIG_CBCR_ADDR,v)
#define HWIO_MSS_CLK_BUS_CONFIG_CBCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_CLK_BUS_CONFIG_CBCR_ADDR,m,v,HWIO_MSS_CLK_BUS_CONFIG_CBCR_IN)
#define HWIO_MSS_CLK_BUS_CONFIG_CBCR_CLK_OFF_BMSK                                   0x80000000
#define HWIO_MSS_CLK_BUS_CONFIG_CBCR_CLK_OFF_SHFT                                         0x1f
#define HWIO_MSS_CLK_BUS_CONFIG_CBCR_CLK_ARES_BMSK                                         0x4
#define HWIO_MSS_CLK_BUS_CONFIG_CBCR_CLK_ARES_SHFT                                         0x2

#define HWIO_MSS_CLK_BUS_RFC_CBCR_ADDR                                              (MSS_MSS_CC_REG_REG_BASE      + 0x00000424)
#define HWIO_MSS_CLK_BUS_RFC_CBCR_OFFS                                              (MSS_MSS_CC_REG_REG_BASE_OFFS + 0x00000424)
#define HWIO_MSS_CLK_BUS_RFC_CBCR_RMSK                                              0x80007ff5
#define HWIO_MSS_CLK_BUS_RFC_CBCR_IN          \
        in_dword_masked(HWIO_MSS_CLK_BUS_RFC_CBCR_ADDR, HWIO_MSS_CLK_BUS_RFC_CBCR_RMSK)
#define HWIO_MSS_CLK_BUS_RFC_CBCR_INM(m)      \
        in_dword_masked(HWIO_MSS_CLK_BUS_RFC_CBCR_ADDR, m)
#define HWIO_MSS_CLK_BUS_RFC_CBCR_OUT(v)      \
        out_dword(HWIO_MSS_CLK_BUS_RFC_CBCR_ADDR,v)
#define HWIO_MSS_CLK_BUS_RFC_CBCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_CLK_BUS_RFC_CBCR_ADDR,m,v,HWIO_MSS_CLK_BUS_RFC_CBCR_IN)
#define HWIO_MSS_CLK_BUS_RFC_CBCR_CLK_OFF_BMSK                                      0x80000000
#define HWIO_MSS_CLK_BUS_RFC_CBCR_CLK_OFF_SHFT                                            0x1f
#define HWIO_MSS_CLK_BUS_RFC_CBCR_FORCE_MEM_CORE_ON_BMSK                                0x4000
#define HWIO_MSS_CLK_BUS_RFC_CBCR_FORCE_MEM_CORE_ON_SHFT                                   0xe
#define HWIO_MSS_CLK_BUS_RFC_CBCR_FORCE_MEM_PERIPH_ON_BMSK                              0x2000
#define HWIO_MSS_CLK_BUS_RFC_CBCR_FORCE_MEM_PERIPH_ON_SHFT                                 0xd
#define HWIO_MSS_CLK_BUS_RFC_CBCR_FORCE_MEM_PERIPH_OFF_BMSK                             0x1000
#define HWIO_MSS_CLK_BUS_RFC_CBCR_FORCE_MEM_PERIPH_OFF_SHFT                                0xc
#define HWIO_MSS_CLK_BUS_RFC_CBCR_WAKEUP_BMSK                                            0xf00
#define HWIO_MSS_CLK_BUS_RFC_CBCR_WAKEUP_SHFT                                              0x8
#define HWIO_MSS_CLK_BUS_RFC_CBCR_SLEEP_BMSK                                              0xf0
#define HWIO_MSS_CLK_BUS_RFC_CBCR_SLEEP_SHFT                                               0x4
#define HWIO_MSS_CLK_BUS_RFC_CBCR_CLK_ARES_BMSK                                            0x4
#define HWIO_MSS_CLK_BUS_RFC_CBCR_CLK_ARES_SHFT                                            0x2
#define HWIO_MSS_CLK_BUS_RFC_CBCR_CLK_ENABLE_BMSK                                          0x1
#define HWIO_MSS_CLK_BUS_RFC_CBCR_CLK_ENABLE_SHFT                                          0x0

#define HWIO_MSS_CLK_BUS_RFC_TRACE_CBCR_ADDR                                        (MSS_MSS_CC_REG_REG_BASE      + 0x00000428)
#define HWIO_MSS_CLK_BUS_RFC_TRACE_CBCR_OFFS                                        (MSS_MSS_CC_REG_REG_BASE_OFFS + 0x00000428)
#define HWIO_MSS_CLK_BUS_RFC_TRACE_CBCR_RMSK                                        0x80000005
#define HWIO_MSS_CLK_BUS_RFC_TRACE_CBCR_IN          \
        in_dword_masked(HWIO_MSS_CLK_BUS_RFC_TRACE_CBCR_ADDR, HWIO_MSS_CLK_BUS_RFC_TRACE_CBCR_RMSK)
#define HWIO_MSS_CLK_BUS_RFC_TRACE_CBCR_INM(m)      \
        in_dword_masked(HWIO_MSS_CLK_BUS_RFC_TRACE_CBCR_ADDR, m)
#define HWIO_MSS_CLK_BUS_RFC_TRACE_CBCR_OUT(v)      \
        out_dword(HWIO_MSS_CLK_BUS_RFC_TRACE_CBCR_ADDR,v)
#define HWIO_MSS_CLK_BUS_RFC_TRACE_CBCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_CLK_BUS_RFC_TRACE_CBCR_ADDR,m,v,HWIO_MSS_CLK_BUS_RFC_TRACE_CBCR_IN)
#define HWIO_MSS_CLK_BUS_RFC_TRACE_CBCR_CLK_OFF_BMSK                                0x80000000
#define HWIO_MSS_CLK_BUS_RFC_TRACE_CBCR_CLK_OFF_SHFT                                      0x1f
#define HWIO_MSS_CLK_BUS_RFC_TRACE_CBCR_CLK_ARES_BMSK                                      0x4
#define HWIO_MSS_CLK_BUS_RFC_TRACE_CBCR_CLK_ARES_SHFT                                      0x2
#define HWIO_MSS_CLK_BUS_RFC_TRACE_CBCR_CLK_ENABLE_BMSK                                    0x1
#define HWIO_MSS_CLK_BUS_RFC_TRACE_CBCR_CLK_ENABLE_SHFT                                    0x0

#define HWIO_MSS_CLK_BUS_COXM_CBCR_ADDR                                             (MSS_MSS_CC_REG_REG_BASE      + 0x0000042c)
#define HWIO_MSS_CLK_BUS_COXM_CBCR_OFFS                                             (MSS_MSS_CC_REG_REG_BASE_OFFS + 0x0000042c)
#define HWIO_MSS_CLK_BUS_COXM_CBCR_RMSK                                             0x80007ff5
#define HWIO_MSS_CLK_BUS_COXM_CBCR_IN          \
        in_dword_masked(HWIO_MSS_CLK_BUS_COXM_CBCR_ADDR, HWIO_MSS_CLK_BUS_COXM_CBCR_RMSK)
#define HWIO_MSS_CLK_BUS_COXM_CBCR_INM(m)      \
        in_dword_masked(HWIO_MSS_CLK_BUS_COXM_CBCR_ADDR, m)
#define HWIO_MSS_CLK_BUS_COXM_CBCR_OUT(v)      \
        out_dword(HWIO_MSS_CLK_BUS_COXM_CBCR_ADDR,v)
#define HWIO_MSS_CLK_BUS_COXM_CBCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_CLK_BUS_COXM_CBCR_ADDR,m,v,HWIO_MSS_CLK_BUS_COXM_CBCR_IN)
#define HWIO_MSS_CLK_BUS_COXM_CBCR_CLK_OFF_BMSK                                     0x80000000
#define HWIO_MSS_CLK_BUS_COXM_CBCR_CLK_OFF_SHFT                                           0x1f
#define HWIO_MSS_CLK_BUS_COXM_CBCR_FORCE_MEM_CORE_ON_BMSK                               0x4000
#define HWIO_MSS_CLK_BUS_COXM_CBCR_FORCE_MEM_CORE_ON_SHFT                                  0xe
#define HWIO_MSS_CLK_BUS_COXM_CBCR_FORCE_MEM_PERIPH_ON_BMSK                             0x2000
#define HWIO_MSS_CLK_BUS_COXM_CBCR_FORCE_MEM_PERIPH_ON_SHFT                                0xd
#define HWIO_MSS_CLK_BUS_COXM_CBCR_FORCE_MEM_PERIPH_OFF_BMSK                            0x1000
#define HWIO_MSS_CLK_BUS_COXM_CBCR_FORCE_MEM_PERIPH_OFF_SHFT                               0xc
#define HWIO_MSS_CLK_BUS_COXM_CBCR_WAKEUP_BMSK                                           0xf00
#define HWIO_MSS_CLK_BUS_COXM_CBCR_WAKEUP_SHFT                                             0x8
#define HWIO_MSS_CLK_BUS_COXM_CBCR_SLEEP_BMSK                                             0xf0
#define HWIO_MSS_CLK_BUS_COXM_CBCR_SLEEP_SHFT                                              0x4
#define HWIO_MSS_CLK_BUS_COXM_CBCR_CLK_ARES_BMSK                                           0x4
#define HWIO_MSS_CLK_BUS_COXM_CBCR_CLK_ARES_SHFT                                           0x2
#define HWIO_MSS_CLK_BUS_COXM_CBCR_CLK_ENABLE_BMSK                                         0x1
#define HWIO_MSS_CLK_BUS_COXM_CBCR_CLK_ENABLE_SHFT                                         0x0

#define HWIO_MSS_CLK_BUS_UIM0_CBCR_ADDR                                             (MSS_MSS_CC_REG_REG_BASE      + 0x00000430)
#define HWIO_MSS_CLK_BUS_UIM0_CBCR_OFFS                                             (MSS_MSS_CC_REG_REG_BASE_OFFS + 0x00000430)
#define HWIO_MSS_CLK_BUS_UIM0_CBCR_RMSK                                             0x80007ff5
#define HWIO_MSS_CLK_BUS_UIM0_CBCR_IN          \
        in_dword_masked(HWIO_MSS_CLK_BUS_UIM0_CBCR_ADDR, HWIO_MSS_CLK_BUS_UIM0_CBCR_RMSK)
#define HWIO_MSS_CLK_BUS_UIM0_CBCR_INM(m)      \
        in_dword_masked(HWIO_MSS_CLK_BUS_UIM0_CBCR_ADDR, m)
#define HWIO_MSS_CLK_BUS_UIM0_CBCR_OUT(v)      \
        out_dword(HWIO_MSS_CLK_BUS_UIM0_CBCR_ADDR,v)
#define HWIO_MSS_CLK_BUS_UIM0_CBCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_CLK_BUS_UIM0_CBCR_ADDR,m,v,HWIO_MSS_CLK_BUS_UIM0_CBCR_IN)
#define HWIO_MSS_CLK_BUS_UIM0_CBCR_CLK_OFF_BMSK                                     0x80000000
#define HWIO_MSS_CLK_BUS_UIM0_CBCR_CLK_OFF_SHFT                                           0x1f
#define HWIO_MSS_CLK_BUS_UIM0_CBCR_FORCE_MEM_CORE_ON_BMSK                               0x4000
#define HWIO_MSS_CLK_BUS_UIM0_CBCR_FORCE_MEM_CORE_ON_SHFT                                  0xe
#define HWIO_MSS_CLK_BUS_UIM0_CBCR_FORCE_MEM_PERIPH_ON_BMSK                             0x2000
#define HWIO_MSS_CLK_BUS_UIM0_CBCR_FORCE_MEM_PERIPH_ON_SHFT                                0xd
#define HWIO_MSS_CLK_BUS_UIM0_CBCR_FORCE_MEM_PERIPH_OFF_BMSK                            0x1000
#define HWIO_MSS_CLK_BUS_UIM0_CBCR_FORCE_MEM_PERIPH_OFF_SHFT                               0xc
#define HWIO_MSS_CLK_BUS_UIM0_CBCR_WAKEUP_BMSK                                           0xf00
#define HWIO_MSS_CLK_BUS_UIM0_CBCR_WAKEUP_SHFT                                             0x8
#define HWIO_MSS_CLK_BUS_UIM0_CBCR_SLEEP_BMSK                                             0xf0
#define HWIO_MSS_CLK_BUS_UIM0_CBCR_SLEEP_SHFT                                              0x4
#define HWIO_MSS_CLK_BUS_UIM0_CBCR_CLK_ARES_BMSK                                           0x4
#define HWIO_MSS_CLK_BUS_UIM0_CBCR_CLK_ARES_SHFT                                           0x2
#define HWIO_MSS_CLK_BUS_UIM0_CBCR_CLK_ENABLE_BMSK                                         0x1
#define HWIO_MSS_CLK_BUS_UIM0_CBCR_CLK_ENABLE_SHFT                                         0x0

#define HWIO_MSS_CLK_BUS_UIM1_CBCR_ADDR                                             (MSS_MSS_CC_REG_REG_BASE      + 0x00000434)
#define HWIO_MSS_CLK_BUS_UIM1_CBCR_OFFS                                             (MSS_MSS_CC_REG_REG_BASE_OFFS + 0x00000434)
#define HWIO_MSS_CLK_BUS_UIM1_CBCR_RMSK                                             0x80007ff5
#define HWIO_MSS_CLK_BUS_UIM1_CBCR_IN          \
        in_dword_masked(HWIO_MSS_CLK_BUS_UIM1_CBCR_ADDR, HWIO_MSS_CLK_BUS_UIM1_CBCR_RMSK)
#define HWIO_MSS_CLK_BUS_UIM1_CBCR_INM(m)      \
        in_dword_masked(HWIO_MSS_CLK_BUS_UIM1_CBCR_ADDR, m)
#define HWIO_MSS_CLK_BUS_UIM1_CBCR_OUT(v)      \
        out_dword(HWIO_MSS_CLK_BUS_UIM1_CBCR_ADDR,v)
#define HWIO_MSS_CLK_BUS_UIM1_CBCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_CLK_BUS_UIM1_CBCR_ADDR,m,v,HWIO_MSS_CLK_BUS_UIM1_CBCR_IN)
#define HWIO_MSS_CLK_BUS_UIM1_CBCR_CLK_OFF_BMSK                                     0x80000000
#define HWIO_MSS_CLK_BUS_UIM1_CBCR_CLK_OFF_SHFT                                           0x1f
#define HWIO_MSS_CLK_BUS_UIM1_CBCR_FORCE_MEM_CORE_ON_BMSK                               0x4000
#define HWIO_MSS_CLK_BUS_UIM1_CBCR_FORCE_MEM_CORE_ON_SHFT                                  0xe
#define HWIO_MSS_CLK_BUS_UIM1_CBCR_FORCE_MEM_PERIPH_ON_BMSK                             0x2000
#define HWIO_MSS_CLK_BUS_UIM1_CBCR_FORCE_MEM_PERIPH_ON_SHFT                                0xd
#define HWIO_MSS_CLK_BUS_UIM1_CBCR_FORCE_MEM_PERIPH_OFF_BMSK                            0x1000
#define HWIO_MSS_CLK_BUS_UIM1_CBCR_FORCE_MEM_PERIPH_OFF_SHFT                               0xc
#define HWIO_MSS_CLK_BUS_UIM1_CBCR_WAKEUP_BMSK                                           0xf00
#define HWIO_MSS_CLK_BUS_UIM1_CBCR_WAKEUP_SHFT                                             0x8
#define HWIO_MSS_CLK_BUS_UIM1_CBCR_SLEEP_BMSK                                             0xf0
#define HWIO_MSS_CLK_BUS_UIM1_CBCR_SLEEP_SHFT                                              0x4
#define HWIO_MSS_CLK_BUS_UIM1_CBCR_CLK_ARES_BMSK                                           0x4
#define HWIO_MSS_CLK_BUS_UIM1_CBCR_CLK_ARES_SHFT                                           0x2
#define HWIO_MSS_CLK_BUS_UIM1_CBCR_CLK_ENABLE_BMSK                                         0x1
#define HWIO_MSS_CLK_BUS_UIM1_CBCR_CLK_ENABLE_SHFT                                         0x0

#define HWIO_MSS_CLK_BUS_MGPI_CBCR_ADDR                                             (MSS_MSS_CC_REG_REG_BASE      + 0x00000438)
#define HWIO_MSS_CLK_BUS_MGPI_CBCR_OFFS                                             (MSS_MSS_CC_REG_REG_BASE_OFFS + 0x00000438)
#define HWIO_MSS_CLK_BUS_MGPI_CBCR_RMSK                                             0x80000005
#define HWIO_MSS_CLK_BUS_MGPI_CBCR_IN          \
        in_dword_masked(HWIO_MSS_CLK_BUS_MGPI_CBCR_ADDR, HWIO_MSS_CLK_BUS_MGPI_CBCR_RMSK)
#define HWIO_MSS_CLK_BUS_MGPI_CBCR_INM(m)      \
        in_dword_masked(HWIO_MSS_CLK_BUS_MGPI_CBCR_ADDR, m)
#define HWIO_MSS_CLK_BUS_MGPI_CBCR_OUT(v)      \
        out_dword(HWIO_MSS_CLK_BUS_MGPI_CBCR_ADDR,v)
#define HWIO_MSS_CLK_BUS_MGPI_CBCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_CLK_BUS_MGPI_CBCR_ADDR,m,v,HWIO_MSS_CLK_BUS_MGPI_CBCR_IN)
#define HWIO_MSS_CLK_BUS_MGPI_CBCR_CLK_OFF_BMSK                                     0x80000000
#define HWIO_MSS_CLK_BUS_MGPI_CBCR_CLK_OFF_SHFT                                           0x1f
#define HWIO_MSS_CLK_BUS_MGPI_CBCR_CLK_ARES_BMSK                                           0x4
#define HWIO_MSS_CLK_BUS_MGPI_CBCR_CLK_ARES_SHFT                                           0x2
#define HWIO_MSS_CLK_BUS_MGPI_CBCR_CLK_ENABLE_BMSK                                         0x1
#define HWIO_MSS_CLK_BUS_MGPI_CBCR_CLK_ENABLE_SHFT                                         0x0

#define HWIO_MSS_CLK_BUS_Q6_CBCR_ADDR                                               (MSS_MSS_CC_REG_REG_BASE      + 0x0000043c)
#define HWIO_MSS_CLK_BUS_Q6_CBCR_OFFS                                               (MSS_MSS_CC_REG_REG_BASE_OFFS + 0x0000043c)
#define HWIO_MSS_CLK_BUS_Q6_CBCR_RMSK                                               0x80000005
#define HWIO_MSS_CLK_BUS_Q6_CBCR_IN          \
        in_dword_masked(HWIO_MSS_CLK_BUS_Q6_CBCR_ADDR, HWIO_MSS_CLK_BUS_Q6_CBCR_RMSK)
#define HWIO_MSS_CLK_BUS_Q6_CBCR_INM(m)      \
        in_dword_masked(HWIO_MSS_CLK_BUS_Q6_CBCR_ADDR, m)
#define HWIO_MSS_CLK_BUS_Q6_CBCR_OUT(v)      \
        out_dword(HWIO_MSS_CLK_BUS_Q6_CBCR_ADDR,v)
#define HWIO_MSS_CLK_BUS_Q6_CBCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_CLK_BUS_Q6_CBCR_ADDR,m,v,HWIO_MSS_CLK_BUS_Q6_CBCR_IN)
#define HWIO_MSS_CLK_BUS_Q6_CBCR_CLK_OFF_BMSK                                       0x80000000
#define HWIO_MSS_CLK_BUS_Q6_CBCR_CLK_OFF_SHFT                                             0x1f
#define HWIO_MSS_CLK_BUS_Q6_CBCR_CLK_ARES_BMSK                                             0x4
#define HWIO_MSS_CLK_BUS_Q6_CBCR_CLK_ARES_SHFT                                             0x2
#define HWIO_MSS_CLK_BUS_Q6_CBCR_CLK_ENABLE_BMSK                                           0x1
#define HWIO_MSS_CLK_BUS_Q6_CBCR_CLK_ENABLE_SHFT                                           0x0

#define HWIO_MSS_CLK_BUS_TIMEOUT_CBCR_ADDR                                          (MSS_MSS_CC_REG_REG_BASE      + 0x00000440)
#define HWIO_MSS_CLK_BUS_TIMEOUT_CBCR_OFFS                                          (MSS_MSS_CC_REG_REG_BASE_OFFS + 0x00000440)
#define HWIO_MSS_CLK_BUS_TIMEOUT_CBCR_RMSK                                          0x80000005
#define HWIO_MSS_CLK_BUS_TIMEOUT_CBCR_IN          \
        in_dword_masked(HWIO_MSS_CLK_BUS_TIMEOUT_CBCR_ADDR, HWIO_MSS_CLK_BUS_TIMEOUT_CBCR_RMSK)
#define HWIO_MSS_CLK_BUS_TIMEOUT_CBCR_INM(m)      \
        in_dword_masked(HWIO_MSS_CLK_BUS_TIMEOUT_CBCR_ADDR, m)
#define HWIO_MSS_CLK_BUS_TIMEOUT_CBCR_OUT(v)      \
        out_dword(HWIO_MSS_CLK_BUS_TIMEOUT_CBCR_ADDR,v)
#define HWIO_MSS_CLK_BUS_TIMEOUT_CBCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_CLK_BUS_TIMEOUT_CBCR_ADDR,m,v,HWIO_MSS_CLK_BUS_TIMEOUT_CBCR_IN)
#define HWIO_MSS_CLK_BUS_TIMEOUT_CBCR_CLK_OFF_BMSK                                  0x80000000
#define HWIO_MSS_CLK_BUS_TIMEOUT_CBCR_CLK_OFF_SHFT                                        0x1f
#define HWIO_MSS_CLK_BUS_TIMEOUT_CBCR_CLK_ARES_BMSK                                        0x4
#define HWIO_MSS_CLK_BUS_TIMEOUT_CBCR_CLK_ARES_SHFT                                        0x2
#define HWIO_MSS_CLK_BUS_TIMEOUT_CBCR_CLK_ENABLE_BMSK                                      0x1
#define HWIO_MSS_CLK_BUS_TIMEOUT_CBCR_CLK_ENABLE_SHFT                                      0x0

#define HWIO_MSS_CLK_BUS_VQ6SS_CBCR_ADDR                                            (MSS_MSS_CC_REG_REG_BASE      + 0x00000444)
#define HWIO_MSS_CLK_BUS_VQ6SS_CBCR_OFFS                                            (MSS_MSS_CC_REG_REG_BASE_OFFS + 0x00000444)
#define HWIO_MSS_CLK_BUS_VQ6SS_CBCR_RMSK                                            0x80000005
#define HWIO_MSS_CLK_BUS_VQ6SS_CBCR_IN          \
        in_dword_masked(HWIO_MSS_CLK_BUS_VQ6SS_CBCR_ADDR, HWIO_MSS_CLK_BUS_VQ6SS_CBCR_RMSK)
#define HWIO_MSS_CLK_BUS_VQ6SS_CBCR_INM(m)      \
        in_dword_masked(HWIO_MSS_CLK_BUS_VQ6SS_CBCR_ADDR, m)
#define HWIO_MSS_CLK_BUS_VQ6SS_CBCR_OUT(v)      \
        out_dword(HWIO_MSS_CLK_BUS_VQ6SS_CBCR_ADDR,v)
#define HWIO_MSS_CLK_BUS_VQ6SS_CBCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_CLK_BUS_VQ6SS_CBCR_ADDR,m,v,HWIO_MSS_CLK_BUS_VQ6SS_CBCR_IN)
#define HWIO_MSS_CLK_BUS_VQ6SS_CBCR_CLK_OFF_BMSK                                    0x80000000
#define HWIO_MSS_CLK_BUS_VQ6SS_CBCR_CLK_OFF_SHFT                                          0x1f
#define HWIO_MSS_CLK_BUS_VQ6SS_CBCR_CLK_ARES_BMSK                                          0x4
#define HWIO_MSS_CLK_BUS_VQ6SS_CBCR_CLK_ARES_SHFT                                          0x2
#define HWIO_MSS_CLK_BUS_VQ6SS_CBCR_CLK_ENABLE_BMSK                                        0x1
#define HWIO_MSS_CLK_BUS_VQ6SS_CBCR_CLK_ENABLE_SHFT                                        0x0

#define HWIO_MSS_CLK_BUS_OFFLINE_CBCR_ADDR                                          (MSS_MSS_CC_REG_REG_BASE      + 0x00000448)
#define HWIO_MSS_CLK_BUS_OFFLINE_CBCR_OFFS                                          (MSS_MSS_CC_REG_REG_BASE_OFFS + 0x00000448)
#define HWIO_MSS_CLK_BUS_OFFLINE_CBCR_RMSK                                          0x80000005
#define HWIO_MSS_CLK_BUS_OFFLINE_CBCR_IN          \
        in_dword_masked(HWIO_MSS_CLK_BUS_OFFLINE_CBCR_ADDR, HWIO_MSS_CLK_BUS_OFFLINE_CBCR_RMSK)
#define HWIO_MSS_CLK_BUS_OFFLINE_CBCR_INM(m)      \
        in_dword_masked(HWIO_MSS_CLK_BUS_OFFLINE_CBCR_ADDR, m)
#define HWIO_MSS_CLK_BUS_OFFLINE_CBCR_OUT(v)      \
        out_dword(HWIO_MSS_CLK_BUS_OFFLINE_CBCR_ADDR,v)
#define HWIO_MSS_CLK_BUS_OFFLINE_CBCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_CLK_BUS_OFFLINE_CBCR_ADDR,m,v,HWIO_MSS_CLK_BUS_OFFLINE_CBCR_IN)
#define HWIO_MSS_CLK_BUS_OFFLINE_CBCR_CLK_OFF_BMSK                                  0x80000000
#define HWIO_MSS_CLK_BUS_OFFLINE_CBCR_CLK_OFF_SHFT                                        0x1f
#define HWIO_MSS_CLK_BUS_OFFLINE_CBCR_CLK_ARES_BMSK                                        0x4
#define HWIO_MSS_CLK_BUS_OFFLINE_CBCR_CLK_ARES_SHFT                                        0x2
#define HWIO_MSS_CLK_BUS_OFFLINE_CBCR_CLK_ENABLE_BMSK                                      0x1
#define HWIO_MSS_CLK_BUS_OFFLINE_CBCR_CLK_ENABLE_SHFT                                      0x0

#define HWIO_MSS_CLK_BUS_STMR_CBCR_ADDR                                             (MSS_MSS_CC_REG_REG_BASE      + 0x0000044c)
#define HWIO_MSS_CLK_BUS_STMR_CBCR_OFFS                                             (MSS_MSS_CC_REG_REG_BASE_OFFS + 0x0000044c)
#define HWIO_MSS_CLK_BUS_STMR_CBCR_RMSK                                             0x80000005
#define HWIO_MSS_CLK_BUS_STMR_CBCR_IN          \
        in_dword_masked(HWIO_MSS_CLK_BUS_STMR_CBCR_ADDR, HWIO_MSS_CLK_BUS_STMR_CBCR_RMSK)
#define HWIO_MSS_CLK_BUS_STMR_CBCR_INM(m)      \
        in_dword_masked(HWIO_MSS_CLK_BUS_STMR_CBCR_ADDR, m)
#define HWIO_MSS_CLK_BUS_STMR_CBCR_OUT(v)      \
        out_dword(HWIO_MSS_CLK_BUS_STMR_CBCR_ADDR,v)
#define HWIO_MSS_CLK_BUS_STMR_CBCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_CLK_BUS_STMR_CBCR_ADDR,m,v,HWIO_MSS_CLK_BUS_STMR_CBCR_IN)
#define HWIO_MSS_CLK_BUS_STMR_CBCR_CLK_OFF_BMSK                                     0x80000000
#define HWIO_MSS_CLK_BUS_STMR_CBCR_CLK_OFF_SHFT                                           0x1f
#define HWIO_MSS_CLK_BUS_STMR_CBCR_CLK_ARES_BMSK                                           0x4
#define HWIO_MSS_CLK_BUS_STMR_CBCR_CLK_ARES_SHFT                                           0x2
#define HWIO_MSS_CLK_BUS_STMR_CBCR_CLK_ENABLE_BMSK                                         0x1
#define HWIO_MSS_CLK_BUS_STMR_CBCR_CLK_ENABLE_SHFT                                         0x0

#define HWIO_MSS_CLK_BUS_CRYPTO_CBCR_ADDR                                           (MSS_MSS_CC_REG_REG_BASE      + 0x00000450)
#define HWIO_MSS_CLK_BUS_CRYPTO_CBCR_OFFS                                           (MSS_MSS_CC_REG_REG_BASE_OFFS + 0x00000450)
#define HWIO_MSS_CLK_BUS_CRYPTO_CBCR_RMSK                                           0x80000005
#define HWIO_MSS_CLK_BUS_CRYPTO_CBCR_IN          \
        in_dword_masked(HWIO_MSS_CLK_BUS_CRYPTO_CBCR_ADDR, HWIO_MSS_CLK_BUS_CRYPTO_CBCR_RMSK)
#define HWIO_MSS_CLK_BUS_CRYPTO_CBCR_INM(m)      \
        in_dword_masked(HWIO_MSS_CLK_BUS_CRYPTO_CBCR_ADDR, m)
#define HWIO_MSS_CLK_BUS_CRYPTO_CBCR_OUT(v)      \
        out_dword(HWIO_MSS_CLK_BUS_CRYPTO_CBCR_ADDR,v)
#define HWIO_MSS_CLK_BUS_CRYPTO_CBCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_CLK_BUS_CRYPTO_CBCR_ADDR,m,v,HWIO_MSS_CLK_BUS_CRYPTO_CBCR_IN)
#define HWIO_MSS_CLK_BUS_CRYPTO_CBCR_CLK_OFF_BMSK                                   0x80000000
#define HWIO_MSS_CLK_BUS_CRYPTO_CBCR_CLK_OFF_SHFT                                         0x1f
#define HWIO_MSS_CLK_BUS_CRYPTO_CBCR_CLK_ARES_BMSK                                         0x4
#define HWIO_MSS_CLK_BUS_CRYPTO_CBCR_CLK_ARES_SHFT                                         0x2
#define HWIO_MSS_CLK_BUS_CRYPTO_CBCR_CLK_ENABLE_BMSK                                       0x1
#define HWIO_MSS_CLK_BUS_CRYPTO_CBCR_CLK_ENABLE_SHFT                                       0x0

#define HWIO_MSS_CLK_BUS_GERAN_ENCRYPT_ACC_CBCR_ADDR                                (MSS_MSS_CC_REG_REG_BASE      + 0x00000454)
#define HWIO_MSS_CLK_BUS_GERAN_ENCRYPT_ACC_CBCR_OFFS                                (MSS_MSS_CC_REG_REG_BASE_OFFS + 0x00000454)
#define HWIO_MSS_CLK_BUS_GERAN_ENCRYPT_ACC_CBCR_RMSK                                0x80000005
#define HWIO_MSS_CLK_BUS_GERAN_ENCRYPT_ACC_CBCR_IN          \
        in_dword_masked(HWIO_MSS_CLK_BUS_GERAN_ENCRYPT_ACC_CBCR_ADDR, HWIO_MSS_CLK_BUS_GERAN_ENCRYPT_ACC_CBCR_RMSK)
#define HWIO_MSS_CLK_BUS_GERAN_ENCRYPT_ACC_CBCR_INM(m)      \
        in_dword_masked(HWIO_MSS_CLK_BUS_GERAN_ENCRYPT_ACC_CBCR_ADDR, m)
#define HWIO_MSS_CLK_BUS_GERAN_ENCRYPT_ACC_CBCR_OUT(v)      \
        out_dword(HWIO_MSS_CLK_BUS_GERAN_ENCRYPT_ACC_CBCR_ADDR,v)
#define HWIO_MSS_CLK_BUS_GERAN_ENCRYPT_ACC_CBCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_CLK_BUS_GERAN_ENCRYPT_ACC_CBCR_ADDR,m,v,HWIO_MSS_CLK_BUS_GERAN_ENCRYPT_ACC_CBCR_IN)
#define HWIO_MSS_CLK_BUS_GERAN_ENCRYPT_ACC_CBCR_CLK_OFF_BMSK                        0x80000000
#define HWIO_MSS_CLK_BUS_GERAN_ENCRYPT_ACC_CBCR_CLK_OFF_SHFT                              0x1f
#define HWIO_MSS_CLK_BUS_GERAN_ENCRYPT_ACC_CBCR_CLK_ARES_BMSK                              0x4
#define HWIO_MSS_CLK_BUS_GERAN_ENCRYPT_ACC_CBCR_CLK_ARES_SHFT                              0x2
#define HWIO_MSS_CLK_BUS_GERAN_ENCRYPT_ACC_CBCR_CLK_ENABLE_BMSK                            0x1
#define HWIO_MSS_CLK_BUS_GERAN_ENCRYPT_ACC_CBCR_CLK_ENABLE_SHFT                            0x0

#define HWIO_MSS_CLK_BUS_NAV_CBCR_ADDR                                              (MSS_MSS_CC_REG_REG_BASE      + 0x00000458)
#define HWIO_MSS_CLK_BUS_NAV_CBCR_OFFS                                              (MSS_MSS_CC_REG_REG_BASE_OFFS + 0x00000458)
#define HWIO_MSS_CLK_BUS_NAV_CBCR_RMSK                                              0x80000005
#define HWIO_MSS_CLK_BUS_NAV_CBCR_IN          \
        in_dword_masked(HWIO_MSS_CLK_BUS_NAV_CBCR_ADDR, HWIO_MSS_CLK_BUS_NAV_CBCR_RMSK)
#define HWIO_MSS_CLK_BUS_NAV_CBCR_INM(m)      \
        in_dword_masked(HWIO_MSS_CLK_BUS_NAV_CBCR_ADDR, m)
#define HWIO_MSS_CLK_BUS_NAV_CBCR_OUT(v)      \
        out_dword(HWIO_MSS_CLK_BUS_NAV_CBCR_ADDR,v)
#define HWIO_MSS_CLK_BUS_NAV_CBCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_CLK_BUS_NAV_CBCR_ADDR,m,v,HWIO_MSS_CLK_BUS_NAV_CBCR_IN)
#define HWIO_MSS_CLK_BUS_NAV_CBCR_CLK_OFF_BMSK                                      0x80000000
#define HWIO_MSS_CLK_BUS_NAV_CBCR_CLK_OFF_SHFT                                            0x1f
#define HWIO_MSS_CLK_BUS_NAV_CBCR_CLK_ARES_BMSK                                            0x4
#define HWIO_MSS_CLK_BUS_NAV_CBCR_CLK_ARES_SHFT                                            0x2
#define HWIO_MSS_CLK_BUS_NAV_CBCR_CLK_ENABLE_BMSK                                          0x1
#define HWIO_MSS_CLK_BUS_NAV_CBCR_CLK_ENABLE_SHFT                                          0x0

#define HWIO_MSS_CLK_BUS_Q6_ALT_CBCR_ADDR                                           (MSS_MSS_CC_REG_REG_BASE      + 0x0000045c)
#define HWIO_MSS_CLK_BUS_Q6_ALT_CBCR_OFFS                                           (MSS_MSS_CC_REG_REG_BASE_OFFS + 0x0000045c)
#define HWIO_MSS_CLK_BUS_Q6_ALT_CBCR_RMSK                                           0x80000005
#define HWIO_MSS_CLK_BUS_Q6_ALT_CBCR_IN          \
        in_dword_masked(HWIO_MSS_CLK_BUS_Q6_ALT_CBCR_ADDR, HWIO_MSS_CLK_BUS_Q6_ALT_CBCR_RMSK)
#define HWIO_MSS_CLK_BUS_Q6_ALT_CBCR_INM(m)      \
        in_dword_masked(HWIO_MSS_CLK_BUS_Q6_ALT_CBCR_ADDR, m)
#define HWIO_MSS_CLK_BUS_Q6_ALT_CBCR_OUT(v)      \
        out_dword(HWIO_MSS_CLK_BUS_Q6_ALT_CBCR_ADDR,v)
#define HWIO_MSS_CLK_BUS_Q6_ALT_CBCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_CLK_BUS_Q6_ALT_CBCR_ADDR,m,v,HWIO_MSS_CLK_BUS_Q6_ALT_CBCR_IN)
#define HWIO_MSS_CLK_BUS_Q6_ALT_CBCR_CLK_OFF_BMSK                                   0x80000000
#define HWIO_MSS_CLK_BUS_Q6_ALT_CBCR_CLK_OFF_SHFT                                         0x1f
#define HWIO_MSS_CLK_BUS_Q6_ALT_CBCR_CLK_ARES_BMSK                                         0x4
#define HWIO_MSS_CLK_BUS_Q6_ALT_CBCR_CLK_ARES_SHFT                                         0x2
#define HWIO_MSS_CLK_BUS_Q6_ALT_CBCR_CLK_ENABLE_BMSK                                       0x1
#define HWIO_MSS_CLK_BUS_Q6_ALT_CBCR_CLK_ENABLE_SHFT                                       0x0

#define HWIO_MSS_CLK_BUS_RSCC_CBCR_ADDR                                             (MSS_MSS_CC_REG_REG_BASE      + 0x00000460)
#define HWIO_MSS_CLK_BUS_RSCC_CBCR_OFFS                                             (MSS_MSS_CC_REG_REG_BASE_OFFS + 0x00000460)
#define HWIO_MSS_CLK_BUS_RSCC_CBCR_RMSK                                             0x80000005
#define HWIO_MSS_CLK_BUS_RSCC_CBCR_IN          \
        in_dword_masked(HWIO_MSS_CLK_BUS_RSCC_CBCR_ADDR, HWIO_MSS_CLK_BUS_RSCC_CBCR_RMSK)
#define HWIO_MSS_CLK_BUS_RSCC_CBCR_INM(m)      \
        in_dword_masked(HWIO_MSS_CLK_BUS_RSCC_CBCR_ADDR, m)
#define HWIO_MSS_CLK_BUS_RSCC_CBCR_OUT(v)      \
        out_dword(HWIO_MSS_CLK_BUS_RSCC_CBCR_ADDR,v)
#define HWIO_MSS_CLK_BUS_RSCC_CBCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_CLK_BUS_RSCC_CBCR_ADDR,m,v,HWIO_MSS_CLK_BUS_RSCC_CBCR_IN)
#define HWIO_MSS_CLK_BUS_RSCC_CBCR_CLK_OFF_BMSK                                     0x80000000
#define HWIO_MSS_CLK_BUS_RSCC_CBCR_CLK_OFF_SHFT                                           0x1f
#define HWIO_MSS_CLK_BUS_RSCC_CBCR_CLK_ARES_BMSK                                           0x4
#define HWIO_MSS_CLK_BUS_RSCC_CBCR_CLK_ARES_SHFT                                           0x2
#define HWIO_MSS_CLK_BUS_RSCC_CBCR_CLK_ENABLE_BMSK                                         0x1
#define HWIO_MSS_CLK_BUS_RSCC_CBCR_CLK_ENABLE_SHFT                                         0x0

#define HWIO_MSS_CLK_SRC_SERDES_REG_CMD_RCGR_ADDR                                   (MSS_MSS_CC_REG_REG_BASE      + 0x00000464)
#define HWIO_MSS_CLK_SRC_SERDES_REG_CMD_RCGR_OFFS                                   (MSS_MSS_CC_REG_REG_BASE_OFFS + 0x00000464)
#define HWIO_MSS_CLK_SRC_SERDES_REG_CMD_RCGR_RMSK                                   0x80000013
#define HWIO_MSS_CLK_SRC_SERDES_REG_CMD_RCGR_IN          \
        in_dword_masked(HWIO_MSS_CLK_SRC_SERDES_REG_CMD_RCGR_ADDR, HWIO_MSS_CLK_SRC_SERDES_REG_CMD_RCGR_RMSK)
#define HWIO_MSS_CLK_SRC_SERDES_REG_CMD_RCGR_INM(m)      \
        in_dword_masked(HWIO_MSS_CLK_SRC_SERDES_REG_CMD_RCGR_ADDR, m)
#define HWIO_MSS_CLK_SRC_SERDES_REG_CMD_RCGR_OUT(v)      \
        out_dword(HWIO_MSS_CLK_SRC_SERDES_REG_CMD_RCGR_ADDR,v)
#define HWIO_MSS_CLK_SRC_SERDES_REG_CMD_RCGR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_CLK_SRC_SERDES_REG_CMD_RCGR_ADDR,m,v,HWIO_MSS_CLK_SRC_SERDES_REG_CMD_RCGR_IN)
#define HWIO_MSS_CLK_SRC_SERDES_REG_CMD_RCGR_ROOT_OFF_BMSK                          0x80000000
#define HWIO_MSS_CLK_SRC_SERDES_REG_CMD_RCGR_ROOT_OFF_SHFT                                0x1f
#define HWIO_MSS_CLK_SRC_SERDES_REG_CMD_RCGR_DIRTY_CFG_RCGR_BMSK                          0x10
#define HWIO_MSS_CLK_SRC_SERDES_REG_CMD_RCGR_DIRTY_CFG_RCGR_SHFT                           0x4
#define HWIO_MSS_CLK_SRC_SERDES_REG_CMD_RCGR_ROOT_EN_BMSK                                  0x2
#define HWIO_MSS_CLK_SRC_SERDES_REG_CMD_RCGR_ROOT_EN_SHFT                                  0x1
#define HWIO_MSS_CLK_SRC_SERDES_REG_CMD_RCGR_UPDATE_BMSK                                   0x1
#define HWIO_MSS_CLK_SRC_SERDES_REG_CMD_RCGR_UPDATE_SHFT                                   0x0

#define HWIO_MSS_CLK_SRC_SERDES_REG_CFG_RCGR_ADDR                                   (MSS_MSS_CC_REG_REG_BASE      + 0x00000468)
#define HWIO_MSS_CLK_SRC_SERDES_REG_CFG_RCGR_OFFS                                   (MSS_MSS_CC_REG_REG_BASE_OFFS + 0x00000468)
#define HWIO_MSS_CLK_SRC_SERDES_REG_CFG_RCGR_RMSK                                     0x11071f
#define HWIO_MSS_CLK_SRC_SERDES_REG_CFG_RCGR_IN          \
        in_dword_masked(HWIO_MSS_CLK_SRC_SERDES_REG_CFG_RCGR_ADDR, HWIO_MSS_CLK_SRC_SERDES_REG_CFG_RCGR_RMSK)
#define HWIO_MSS_CLK_SRC_SERDES_REG_CFG_RCGR_INM(m)      \
        in_dword_masked(HWIO_MSS_CLK_SRC_SERDES_REG_CFG_RCGR_ADDR, m)
#define HWIO_MSS_CLK_SRC_SERDES_REG_CFG_RCGR_OUT(v)      \
        out_dword(HWIO_MSS_CLK_SRC_SERDES_REG_CFG_RCGR_ADDR,v)
#define HWIO_MSS_CLK_SRC_SERDES_REG_CFG_RCGR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_CLK_SRC_SERDES_REG_CFG_RCGR_ADDR,m,v,HWIO_MSS_CLK_SRC_SERDES_REG_CFG_RCGR_IN)
#define HWIO_MSS_CLK_SRC_SERDES_REG_CFG_RCGR_HW_CLK_CONTROL_BMSK                      0x100000
#define HWIO_MSS_CLK_SRC_SERDES_REG_CFG_RCGR_HW_CLK_CONTROL_SHFT                          0x14
#define HWIO_MSS_CLK_SRC_SERDES_REG_CFG_RCGR_RCGLITE_DISABLE_BMSK                      0x10000
#define HWIO_MSS_CLK_SRC_SERDES_REG_CFG_RCGR_RCGLITE_DISABLE_SHFT                         0x10
#define HWIO_MSS_CLK_SRC_SERDES_REG_CFG_RCGR_SRC_SEL_BMSK                                0x700
#define HWIO_MSS_CLK_SRC_SERDES_REG_CFG_RCGR_SRC_SEL_SHFT                                  0x8
#define HWIO_MSS_CLK_SRC_SERDES_REG_CFG_RCGR_SRC_DIV_BMSK                                 0x1f
#define HWIO_MSS_CLK_SRC_SERDES_REG_CFG_RCGR_SRC_DIV_SHFT                                  0x0

#define HWIO_MSS_CLK_SERDES_CBCR_ADDR                                               (MSS_MSS_CC_REG_REG_BASE      + 0x0000046c)
#define HWIO_MSS_CLK_SERDES_CBCR_OFFS                                               (MSS_MSS_CC_REG_REG_BASE_OFFS + 0x0000046c)
#define HWIO_MSS_CLK_SERDES_CBCR_RMSK                                               0x80000005
#define HWIO_MSS_CLK_SERDES_CBCR_IN          \
        in_dword_masked(HWIO_MSS_CLK_SERDES_CBCR_ADDR, HWIO_MSS_CLK_SERDES_CBCR_RMSK)
#define HWIO_MSS_CLK_SERDES_CBCR_INM(m)      \
        in_dword_masked(HWIO_MSS_CLK_SERDES_CBCR_ADDR, m)
#define HWIO_MSS_CLK_SERDES_CBCR_OUT(v)      \
        out_dword(HWIO_MSS_CLK_SERDES_CBCR_ADDR,v)
#define HWIO_MSS_CLK_SERDES_CBCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_CLK_SERDES_CBCR_ADDR,m,v,HWIO_MSS_CLK_SERDES_CBCR_IN)
#define HWIO_MSS_CLK_SERDES_CBCR_CLK_OFF_BMSK                                       0x80000000
#define HWIO_MSS_CLK_SERDES_CBCR_CLK_OFF_SHFT                                             0x1f
#define HWIO_MSS_CLK_SERDES_CBCR_CLK_ARES_BMSK                                             0x4
#define HWIO_MSS_CLK_SERDES_CBCR_CLK_ARES_SHFT                                             0x2
#define HWIO_MSS_CLK_SERDES_CBCR_CLK_ENABLE_BMSK                                           0x1
#define HWIO_MSS_CLK_SERDES_CBCR_CLK_ENABLE_SHFT                                           0x0

#define HWIO_MSS_CLK_SRC_RFFE_REG_CMD_RCGR_ADDR                                     (MSS_MSS_CC_REG_REG_BASE      + 0x00000470)
#define HWIO_MSS_CLK_SRC_RFFE_REG_CMD_RCGR_OFFS                                     (MSS_MSS_CC_REG_REG_BASE_OFFS + 0x00000470)
#define HWIO_MSS_CLK_SRC_RFFE_REG_CMD_RCGR_RMSK                                     0x80000013
#define HWIO_MSS_CLK_SRC_RFFE_REG_CMD_RCGR_IN          \
        in_dword_masked(HWIO_MSS_CLK_SRC_RFFE_REG_CMD_RCGR_ADDR, HWIO_MSS_CLK_SRC_RFFE_REG_CMD_RCGR_RMSK)
#define HWIO_MSS_CLK_SRC_RFFE_REG_CMD_RCGR_INM(m)      \
        in_dword_masked(HWIO_MSS_CLK_SRC_RFFE_REG_CMD_RCGR_ADDR, m)
#define HWIO_MSS_CLK_SRC_RFFE_REG_CMD_RCGR_OUT(v)      \
        out_dword(HWIO_MSS_CLK_SRC_RFFE_REG_CMD_RCGR_ADDR,v)
#define HWIO_MSS_CLK_SRC_RFFE_REG_CMD_RCGR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_CLK_SRC_RFFE_REG_CMD_RCGR_ADDR,m,v,HWIO_MSS_CLK_SRC_RFFE_REG_CMD_RCGR_IN)
#define HWIO_MSS_CLK_SRC_RFFE_REG_CMD_RCGR_ROOT_OFF_BMSK                            0x80000000
#define HWIO_MSS_CLK_SRC_RFFE_REG_CMD_RCGR_ROOT_OFF_SHFT                                  0x1f
#define HWIO_MSS_CLK_SRC_RFFE_REG_CMD_RCGR_DIRTY_CFG_RCGR_BMSK                            0x10
#define HWIO_MSS_CLK_SRC_RFFE_REG_CMD_RCGR_DIRTY_CFG_RCGR_SHFT                             0x4
#define HWIO_MSS_CLK_SRC_RFFE_REG_CMD_RCGR_ROOT_EN_BMSK                                    0x2
#define HWIO_MSS_CLK_SRC_RFFE_REG_CMD_RCGR_ROOT_EN_SHFT                                    0x1
#define HWIO_MSS_CLK_SRC_RFFE_REG_CMD_RCGR_UPDATE_BMSK                                     0x1
#define HWIO_MSS_CLK_SRC_RFFE_REG_CMD_RCGR_UPDATE_SHFT                                     0x0

#define HWIO_MSS_CLK_SRC_RFFE_REG_CFG_RCGR_ADDR                                     (MSS_MSS_CC_REG_REG_BASE      + 0x00000474)
#define HWIO_MSS_CLK_SRC_RFFE_REG_CFG_RCGR_OFFS                                     (MSS_MSS_CC_REG_REG_BASE_OFFS + 0x00000474)
#define HWIO_MSS_CLK_SRC_RFFE_REG_CFG_RCGR_RMSK                                       0x11071f
#define HWIO_MSS_CLK_SRC_RFFE_REG_CFG_RCGR_IN          \
        in_dword_masked(HWIO_MSS_CLK_SRC_RFFE_REG_CFG_RCGR_ADDR, HWIO_MSS_CLK_SRC_RFFE_REG_CFG_RCGR_RMSK)
#define HWIO_MSS_CLK_SRC_RFFE_REG_CFG_RCGR_INM(m)      \
        in_dword_masked(HWIO_MSS_CLK_SRC_RFFE_REG_CFG_RCGR_ADDR, m)
#define HWIO_MSS_CLK_SRC_RFFE_REG_CFG_RCGR_OUT(v)      \
        out_dword(HWIO_MSS_CLK_SRC_RFFE_REG_CFG_RCGR_ADDR,v)
#define HWIO_MSS_CLK_SRC_RFFE_REG_CFG_RCGR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_CLK_SRC_RFFE_REG_CFG_RCGR_ADDR,m,v,HWIO_MSS_CLK_SRC_RFFE_REG_CFG_RCGR_IN)
#define HWIO_MSS_CLK_SRC_RFFE_REG_CFG_RCGR_HW_CLK_CONTROL_BMSK                        0x100000
#define HWIO_MSS_CLK_SRC_RFFE_REG_CFG_RCGR_HW_CLK_CONTROL_SHFT                            0x14
#define HWIO_MSS_CLK_SRC_RFFE_REG_CFG_RCGR_RCGLITE_DISABLE_BMSK                        0x10000
#define HWIO_MSS_CLK_SRC_RFFE_REG_CFG_RCGR_RCGLITE_DISABLE_SHFT                           0x10
#define HWIO_MSS_CLK_SRC_RFFE_REG_CFG_RCGR_SRC_SEL_BMSK                                  0x700
#define HWIO_MSS_CLK_SRC_RFFE_REG_CFG_RCGR_SRC_SEL_SHFT                                    0x8
#define HWIO_MSS_CLK_SRC_RFFE_REG_CFG_RCGR_SRC_DIV_BMSK                                   0x1f
#define HWIO_MSS_CLK_SRC_RFFE_REG_CFG_RCGR_SRC_DIV_SHFT                                    0x0

#define HWIO_MSS_CLK_4XO_RFFE_CBCR_ADDR                                             (MSS_MSS_CC_REG_REG_BASE      + 0x00000478)
#define HWIO_MSS_CLK_4XO_RFFE_CBCR_OFFS                                             (MSS_MSS_CC_REG_REG_BASE_OFFS + 0x00000478)
#define HWIO_MSS_CLK_4XO_RFFE_CBCR_RMSK                                             0x80000005
#define HWIO_MSS_CLK_4XO_RFFE_CBCR_IN          \
        in_dword_masked(HWIO_MSS_CLK_4XO_RFFE_CBCR_ADDR, HWIO_MSS_CLK_4XO_RFFE_CBCR_RMSK)
#define HWIO_MSS_CLK_4XO_RFFE_CBCR_INM(m)      \
        in_dword_masked(HWIO_MSS_CLK_4XO_RFFE_CBCR_ADDR, m)
#define HWIO_MSS_CLK_4XO_RFFE_CBCR_OUT(v)      \
        out_dword(HWIO_MSS_CLK_4XO_RFFE_CBCR_ADDR,v)
#define HWIO_MSS_CLK_4XO_RFFE_CBCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_CLK_4XO_RFFE_CBCR_ADDR,m,v,HWIO_MSS_CLK_4XO_RFFE_CBCR_IN)
#define HWIO_MSS_CLK_4XO_RFFE_CBCR_CLK_OFF_BMSK                                     0x80000000
#define HWIO_MSS_CLK_4XO_RFFE_CBCR_CLK_OFF_SHFT                                           0x1f
#define HWIO_MSS_CLK_4XO_RFFE_CBCR_CLK_ARES_BMSK                                           0x4
#define HWIO_MSS_CLK_4XO_RFFE_CBCR_CLK_ARES_SHFT                                           0x2
#define HWIO_MSS_CLK_4XO_RFFE_CBCR_CLK_ENABLE_BMSK                                         0x1
#define HWIO_MSS_CLK_4XO_RFFE_CBCR_CLK_ENABLE_SHFT                                         0x0

#define HWIO_MSS_CLK_2XO_RFFE_CBCR_ADDR                                             (MSS_MSS_CC_REG_REG_BASE      + 0x0000047c)
#define HWIO_MSS_CLK_2XO_RFFE_CBCR_OFFS                                             (MSS_MSS_CC_REG_REG_BASE_OFFS + 0x0000047c)
#define HWIO_MSS_CLK_2XO_RFFE_CBCR_RMSK                                             0x80000005
#define HWIO_MSS_CLK_2XO_RFFE_CBCR_IN          \
        in_dword_masked(HWIO_MSS_CLK_2XO_RFFE_CBCR_ADDR, HWIO_MSS_CLK_2XO_RFFE_CBCR_RMSK)
#define HWIO_MSS_CLK_2XO_RFFE_CBCR_INM(m)      \
        in_dword_masked(HWIO_MSS_CLK_2XO_RFFE_CBCR_ADDR, m)
#define HWIO_MSS_CLK_2XO_RFFE_CBCR_OUT(v)      \
        out_dword(HWIO_MSS_CLK_2XO_RFFE_CBCR_ADDR,v)
#define HWIO_MSS_CLK_2XO_RFFE_CBCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_CLK_2XO_RFFE_CBCR_ADDR,m,v,HWIO_MSS_CLK_2XO_RFFE_CBCR_IN)
#define HWIO_MSS_CLK_2XO_RFFE_CBCR_CLK_OFF_BMSK                                     0x80000000
#define HWIO_MSS_CLK_2XO_RFFE_CBCR_CLK_OFF_SHFT                                           0x1f
#define HWIO_MSS_CLK_2XO_RFFE_CBCR_CLK_ARES_BMSK                                           0x4
#define HWIO_MSS_CLK_2XO_RFFE_CBCR_CLK_ARES_SHFT                                           0x2
#define HWIO_MSS_CLK_2XO_RFFE_CBCR_CLK_ENABLE_BMSK                                         0x1
#define HWIO_MSS_CLK_2XO_RFFE_CBCR_CLK_ENABLE_SHFT                                         0x0

#define HWIO_MSS_CLK_XO_RFFE_CBCR_ADDR                                              (MSS_MSS_CC_REG_REG_BASE      + 0x00000480)
#define HWIO_MSS_CLK_XO_RFFE_CBCR_OFFS                                              (MSS_MSS_CC_REG_REG_BASE_OFFS + 0x00000480)
#define HWIO_MSS_CLK_XO_RFFE_CBCR_RMSK                                              0x80000005
#define HWIO_MSS_CLK_XO_RFFE_CBCR_IN          \
        in_dword_masked(HWIO_MSS_CLK_XO_RFFE_CBCR_ADDR, HWIO_MSS_CLK_XO_RFFE_CBCR_RMSK)
#define HWIO_MSS_CLK_XO_RFFE_CBCR_INM(m)      \
        in_dword_masked(HWIO_MSS_CLK_XO_RFFE_CBCR_ADDR, m)
#define HWIO_MSS_CLK_XO_RFFE_CBCR_OUT(v)      \
        out_dword(HWIO_MSS_CLK_XO_RFFE_CBCR_ADDR,v)
#define HWIO_MSS_CLK_XO_RFFE_CBCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_CLK_XO_RFFE_CBCR_ADDR,m,v,HWIO_MSS_CLK_XO_RFFE_CBCR_IN)
#define HWIO_MSS_CLK_XO_RFFE_CBCR_CLK_OFF_BMSK                                      0x80000000
#define HWIO_MSS_CLK_XO_RFFE_CBCR_CLK_OFF_SHFT                                            0x1f
#define HWIO_MSS_CLK_XO_RFFE_CBCR_CLK_ARES_BMSK                                            0x4
#define HWIO_MSS_CLK_XO_RFFE_CBCR_CLK_ARES_SHFT                                            0x2
#define HWIO_MSS_CLK_XO_RFFE_CBCR_CLK_ENABLE_BMSK                                          0x1
#define HWIO_MSS_CLK_XO_RFFE_CBCR_CLK_ENABLE_SHFT                                          0x0

#define HWIO_MSS_CLK_SRC_UIM0_REG_CMD_RCGR_ADDR                                     (MSS_MSS_CC_REG_REG_BASE      + 0x00000484)
#define HWIO_MSS_CLK_SRC_UIM0_REG_CMD_RCGR_OFFS                                     (MSS_MSS_CC_REG_REG_BASE_OFFS + 0x00000484)
#define HWIO_MSS_CLK_SRC_UIM0_REG_CMD_RCGR_RMSK                                     0x80000013
#define HWIO_MSS_CLK_SRC_UIM0_REG_CMD_RCGR_IN          \
        in_dword_masked(HWIO_MSS_CLK_SRC_UIM0_REG_CMD_RCGR_ADDR, HWIO_MSS_CLK_SRC_UIM0_REG_CMD_RCGR_RMSK)
#define HWIO_MSS_CLK_SRC_UIM0_REG_CMD_RCGR_INM(m)      \
        in_dword_masked(HWIO_MSS_CLK_SRC_UIM0_REG_CMD_RCGR_ADDR, m)
#define HWIO_MSS_CLK_SRC_UIM0_REG_CMD_RCGR_OUT(v)      \
        out_dword(HWIO_MSS_CLK_SRC_UIM0_REG_CMD_RCGR_ADDR,v)
#define HWIO_MSS_CLK_SRC_UIM0_REG_CMD_RCGR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_CLK_SRC_UIM0_REG_CMD_RCGR_ADDR,m,v,HWIO_MSS_CLK_SRC_UIM0_REG_CMD_RCGR_IN)
#define HWIO_MSS_CLK_SRC_UIM0_REG_CMD_RCGR_ROOT_OFF_BMSK                            0x80000000
#define HWIO_MSS_CLK_SRC_UIM0_REG_CMD_RCGR_ROOT_OFF_SHFT                                  0x1f
#define HWIO_MSS_CLK_SRC_UIM0_REG_CMD_RCGR_DIRTY_CFG_RCGR_BMSK                            0x10
#define HWIO_MSS_CLK_SRC_UIM0_REG_CMD_RCGR_DIRTY_CFG_RCGR_SHFT                             0x4
#define HWIO_MSS_CLK_SRC_UIM0_REG_CMD_RCGR_ROOT_EN_BMSK                                    0x2
#define HWIO_MSS_CLK_SRC_UIM0_REG_CMD_RCGR_ROOT_EN_SHFT                                    0x1
#define HWIO_MSS_CLK_SRC_UIM0_REG_CMD_RCGR_UPDATE_BMSK                                     0x1
#define HWIO_MSS_CLK_SRC_UIM0_REG_CMD_RCGR_UPDATE_SHFT                                     0x0

#define HWIO_MSS_CLK_SRC_UIM0_REG_CFG_RCGR_ADDR                                     (MSS_MSS_CC_REG_REG_BASE      + 0x00000488)
#define HWIO_MSS_CLK_SRC_UIM0_REG_CFG_RCGR_OFFS                                     (MSS_MSS_CC_REG_REG_BASE_OFFS + 0x00000488)
#define HWIO_MSS_CLK_SRC_UIM0_REG_CFG_RCGR_RMSK                                       0x11071f
#define HWIO_MSS_CLK_SRC_UIM0_REG_CFG_RCGR_IN          \
        in_dword_masked(HWIO_MSS_CLK_SRC_UIM0_REG_CFG_RCGR_ADDR, HWIO_MSS_CLK_SRC_UIM0_REG_CFG_RCGR_RMSK)
#define HWIO_MSS_CLK_SRC_UIM0_REG_CFG_RCGR_INM(m)      \
        in_dword_masked(HWIO_MSS_CLK_SRC_UIM0_REG_CFG_RCGR_ADDR, m)
#define HWIO_MSS_CLK_SRC_UIM0_REG_CFG_RCGR_OUT(v)      \
        out_dword(HWIO_MSS_CLK_SRC_UIM0_REG_CFG_RCGR_ADDR,v)
#define HWIO_MSS_CLK_SRC_UIM0_REG_CFG_RCGR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_CLK_SRC_UIM0_REG_CFG_RCGR_ADDR,m,v,HWIO_MSS_CLK_SRC_UIM0_REG_CFG_RCGR_IN)
#define HWIO_MSS_CLK_SRC_UIM0_REG_CFG_RCGR_HW_CLK_CONTROL_BMSK                        0x100000
#define HWIO_MSS_CLK_SRC_UIM0_REG_CFG_RCGR_HW_CLK_CONTROL_SHFT                            0x14
#define HWIO_MSS_CLK_SRC_UIM0_REG_CFG_RCGR_RCGLITE_DISABLE_BMSK                        0x10000
#define HWIO_MSS_CLK_SRC_UIM0_REG_CFG_RCGR_RCGLITE_DISABLE_SHFT                           0x10
#define HWIO_MSS_CLK_SRC_UIM0_REG_CFG_RCGR_SRC_SEL_BMSK                                  0x700
#define HWIO_MSS_CLK_SRC_UIM0_REG_CFG_RCGR_SRC_SEL_SHFT                                    0x8
#define HWIO_MSS_CLK_SRC_UIM0_REG_CFG_RCGR_SRC_DIV_BMSK                                   0x1f
#define HWIO_MSS_CLK_SRC_UIM0_REG_CFG_RCGR_SRC_DIV_SHFT                                    0x0

#define HWIO_MSS_CLK_SRC_UIM1_REG_CMD_RCGR_ADDR                                     (MSS_MSS_CC_REG_REG_BASE      + 0x0000048c)
#define HWIO_MSS_CLK_SRC_UIM1_REG_CMD_RCGR_OFFS                                     (MSS_MSS_CC_REG_REG_BASE_OFFS + 0x0000048c)
#define HWIO_MSS_CLK_SRC_UIM1_REG_CMD_RCGR_RMSK                                     0x80000013
#define HWIO_MSS_CLK_SRC_UIM1_REG_CMD_RCGR_IN          \
        in_dword_masked(HWIO_MSS_CLK_SRC_UIM1_REG_CMD_RCGR_ADDR, HWIO_MSS_CLK_SRC_UIM1_REG_CMD_RCGR_RMSK)
#define HWIO_MSS_CLK_SRC_UIM1_REG_CMD_RCGR_INM(m)      \
        in_dword_masked(HWIO_MSS_CLK_SRC_UIM1_REG_CMD_RCGR_ADDR, m)
#define HWIO_MSS_CLK_SRC_UIM1_REG_CMD_RCGR_OUT(v)      \
        out_dword(HWIO_MSS_CLK_SRC_UIM1_REG_CMD_RCGR_ADDR,v)
#define HWIO_MSS_CLK_SRC_UIM1_REG_CMD_RCGR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_CLK_SRC_UIM1_REG_CMD_RCGR_ADDR,m,v,HWIO_MSS_CLK_SRC_UIM1_REG_CMD_RCGR_IN)
#define HWIO_MSS_CLK_SRC_UIM1_REG_CMD_RCGR_ROOT_OFF_BMSK                            0x80000000
#define HWIO_MSS_CLK_SRC_UIM1_REG_CMD_RCGR_ROOT_OFF_SHFT                                  0x1f
#define HWIO_MSS_CLK_SRC_UIM1_REG_CMD_RCGR_DIRTY_CFG_RCGR_BMSK                            0x10
#define HWIO_MSS_CLK_SRC_UIM1_REG_CMD_RCGR_DIRTY_CFG_RCGR_SHFT                             0x4
#define HWIO_MSS_CLK_SRC_UIM1_REG_CMD_RCGR_ROOT_EN_BMSK                                    0x2
#define HWIO_MSS_CLK_SRC_UIM1_REG_CMD_RCGR_ROOT_EN_SHFT                                    0x1
#define HWIO_MSS_CLK_SRC_UIM1_REG_CMD_RCGR_UPDATE_BMSK                                     0x1
#define HWIO_MSS_CLK_SRC_UIM1_REG_CMD_RCGR_UPDATE_SHFT                                     0x0

#define HWIO_MSS_CLK_SRC_UIM1_REG_CFG_RCGR_ADDR                                     (MSS_MSS_CC_REG_REG_BASE      + 0x00000490)
#define HWIO_MSS_CLK_SRC_UIM1_REG_CFG_RCGR_OFFS                                     (MSS_MSS_CC_REG_REG_BASE_OFFS + 0x00000490)
#define HWIO_MSS_CLK_SRC_UIM1_REG_CFG_RCGR_RMSK                                       0x11071f
#define HWIO_MSS_CLK_SRC_UIM1_REG_CFG_RCGR_IN          \
        in_dword_masked(HWIO_MSS_CLK_SRC_UIM1_REG_CFG_RCGR_ADDR, HWIO_MSS_CLK_SRC_UIM1_REG_CFG_RCGR_RMSK)
#define HWIO_MSS_CLK_SRC_UIM1_REG_CFG_RCGR_INM(m)      \
        in_dword_masked(HWIO_MSS_CLK_SRC_UIM1_REG_CFG_RCGR_ADDR, m)
#define HWIO_MSS_CLK_SRC_UIM1_REG_CFG_RCGR_OUT(v)      \
        out_dword(HWIO_MSS_CLK_SRC_UIM1_REG_CFG_RCGR_ADDR,v)
#define HWIO_MSS_CLK_SRC_UIM1_REG_CFG_RCGR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_CLK_SRC_UIM1_REG_CFG_RCGR_ADDR,m,v,HWIO_MSS_CLK_SRC_UIM1_REG_CFG_RCGR_IN)
#define HWIO_MSS_CLK_SRC_UIM1_REG_CFG_RCGR_HW_CLK_CONTROL_BMSK                        0x100000
#define HWIO_MSS_CLK_SRC_UIM1_REG_CFG_RCGR_HW_CLK_CONTROL_SHFT                            0x14
#define HWIO_MSS_CLK_SRC_UIM1_REG_CFG_RCGR_RCGLITE_DISABLE_BMSK                        0x10000
#define HWIO_MSS_CLK_SRC_UIM1_REG_CFG_RCGR_RCGLITE_DISABLE_SHFT                           0x10
#define HWIO_MSS_CLK_SRC_UIM1_REG_CFG_RCGR_SRC_SEL_BMSK                                  0x700
#define HWIO_MSS_CLK_SRC_UIM1_REG_CFG_RCGR_SRC_SEL_SHFT                                    0x8
#define HWIO_MSS_CLK_SRC_UIM1_REG_CFG_RCGR_SRC_DIV_BMSK                                   0x1f
#define HWIO_MSS_CLK_SRC_UIM1_REG_CFG_RCGR_SRC_DIV_SHFT                                    0x0

#define HWIO_MSS_CLK_CARD_SRC_UIM0_CBCR_ADDR                                        (MSS_MSS_CC_REG_REG_BASE      + 0x00000494)
#define HWIO_MSS_CLK_CARD_SRC_UIM0_CBCR_OFFS                                        (MSS_MSS_CC_REG_REG_BASE_OFFS + 0x00000494)
#define HWIO_MSS_CLK_CARD_SRC_UIM0_CBCR_RMSK                                        0x80000005
#define HWIO_MSS_CLK_CARD_SRC_UIM0_CBCR_IN          \
        in_dword_masked(HWIO_MSS_CLK_CARD_SRC_UIM0_CBCR_ADDR, HWIO_MSS_CLK_CARD_SRC_UIM0_CBCR_RMSK)
#define HWIO_MSS_CLK_CARD_SRC_UIM0_CBCR_INM(m)      \
        in_dword_masked(HWIO_MSS_CLK_CARD_SRC_UIM0_CBCR_ADDR, m)
#define HWIO_MSS_CLK_CARD_SRC_UIM0_CBCR_OUT(v)      \
        out_dword(HWIO_MSS_CLK_CARD_SRC_UIM0_CBCR_ADDR,v)
#define HWIO_MSS_CLK_CARD_SRC_UIM0_CBCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_CLK_CARD_SRC_UIM0_CBCR_ADDR,m,v,HWIO_MSS_CLK_CARD_SRC_UIM0_CBCR_IN)
#define HWIO_MSS_CLK_CARD_SRC_UIM0_CBCR_CLK_OFF_BMSK                                0x80000000
#define HWIO_MSS_CLK_CARD_SRC_UIM0_CBCR_CLK_OFF_SHFT                                      0x1f
#define HWIO_MSS_CLK_CARD_SRC_UIM0_CBCR_CLK_ARES_BMSK                                      0x4
#define HWIO_MSS_CLK_CARD_SRC_UIM0_CBCR_CLK_ARES_SHFT                                      0x2
#define HWIO_MSS_CLK_CARD_SRC_UIM0_CBCR_CLK_ENABLE_BMSK                                    0x1
#define HWIO_MSS_CLK_CARD_SRC_UIM0_CBCR_CLK_ENABLE_SHFT                                    0x0

#define HWIO_MSS_CLK_CARD_SRC_UIM1_CBCR_ADDR                                        (MSS_MSS_CC_REG_REG_BASE      + 0x00000498)
#define HWIO_MSS_CLK_CARD_SRC_UIM1_CBCR_OFFS                                        (MSS_MSS_CC_REG_REG_BASE_OFFS + 0x00000498)
#define HWIO_MSS_CLK_CARD_SRC_UIM1_CBCR_RMSK                                        0x80000005
#define HWIO_MSS_CLK_CARD_SRC_UIM1_CBCR_IN          \
        in_dword_masked(HWIO_MSS_CLK_CARD_SRC_UIM1_CBCR_ADDR, HWIO_MSS_CLK_CARD_SRC_UIM1_CBCR_RMSK)
#define HWIO_MSS_CLK_CARD_SRC_UIM1_CBCR_INM(m)      \
        in_dword_masked(HWIO_MSS_CLK_CARD_SRC_UIM1_CBCR_ADDR, m)
#define HWIO_MSS_CLK_CARD_SRC_UIM1_CBCR_OUT(v)      \
        out_dword(HWIO_MSS_CLK_CARD_SRC_UIM1_CBCR_ADDR,v)
#define HWIO_MSS_CLK_CARD_SRC_UIM1_CBCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_CLK_CARD_SRC_UIM1_CBCR_ADDR,m,v,HWIO_MSS_CLK_CARD_SRC_UIM1_CBCR_IN)
#define HWIO_MSS_CLK_CARD_SRC_UIM1_CBCR_CLK_OFF_BMSK                                0x80000000
#define HWIO_MSS_CLK_CARD_SRC_UIM1_CBCR_CLK_OFF_SHFT                                      0x1f
#define HWIO_MSS_CLK_CARD_SRC_UIM1_CBCR_CLK_ARES_BMSK                                      0x4
#define HWIO_MSS_CLK_CARD_SRC_UIM1_CBCR_CLK_ARES_SHFT                                      0x2
#define HWIO_MSS_CLK_CARD_SRC_UIM1_CBCR_CLK_ENABLE_BMSK                                    0x1
#define HWIO_MSS_CLK_CARD_SRC_UIM1_CBCR_CLK_ENABLE_SHFT                                    0x0

#define HWIO_MSS_CLK_SRC_UIM0_MND_REG_CMD_RCGR_ADDR                                 (MSS_MSS_CC_REG_REG_BASE      + 0x0000049c)
#define HWIO_MSS_CLK_SRC_UIM0_MND_REG_CMD_RCGR_OFFS                                 (MSS_MSS_CC_REG_REG_BASE_OFFS + 0x0000049c)
#define HWIO_MSS_CLK_SRC_UIM0_MND_REG_CMD_RCGR_RMSK                                 0x800000f3
#define HWIO_MSS_CLK_SRC_UIM0_MND_REG_CMD_RCGR_IN          \
        in_dword_masked(HWIO_MSS_CLK_SRC_UIM0_MND_REG_CMD_RCGR_ADDR, HWIO_MSS_CLK_SRC_UIM0_MND_REG_CMD_RCGR_RMSK)
#define HWIO_MSS_CLK_SRC_UIM0_MND_REG_CMD_RCGR_INM(m)      \
        in_dword_masked(HWIO_MSS_CLK_SRC_UIM0_MND_REG_CMD_RCGR_ADDR, m)
#define HWIO_MSS_CLK_SRC_UIM0_MND_REG_CMD_RCGR_OUT(v)      \
        out_dword(HWIO_MSS_CLK_SRC_UIM0_MND_REG_CMD_RCGR_ADDR,v)
#define HWIO_MSS_CLK_SRC_UIM0_MND_REG_CMD_RCGR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_CLK_SRC_UIM0_MND_REG_CMD_RCGR_ADDR,m,v,HWIO_MSS_CLK_SRC_UIM0_MND_REG_CMD_RCGR_IN)
#define HWIO_MSS_CLK_SRC_UIM0_MND_REG_CMD_RCGR_ROOT_OFF_BMSK                        0x80000000
#define HWIO_MSS_CLK_SRC_UIM0_MND_REG_CMD_RCGR_ROOT_OFF_SHFT                              0x1f
#define HWIO_MSS_CLK_SRC_UIM0_MND_REG_CMD_RCGR_DIRTY_D_BMSK                               0x80
#define HWIO_MSS_CLK_SRC_UIM0_MND_REG_CMD_RCGR_DIRTY_D_SHFT                                0x7
#define HWIO_MSS_CLK_SRC_UIM0_MND_REG_CMD_RCGR_DIRTY_N_BMSK                               0x40
#define HWIO_MSS_CLK_SRC_UIM0_MND_REG_CMD_RCGR_DIRTY_N_SHFT                                0x6
#define HWIO_MSS_CLK_SRC_UIM0_MND_REG_CMD_RCGR_DIRTY_M_BMSK                               0x20
#define HWIO_MSS_CLK_SRC_UIM0_MND_REG_CMD_RCGR_DIRTY_M_SHFT                                0x5
#define HWIO_MSS_CLK_SRC_UIM0_MND_REG_CMD_RCGR_DIRTY_CFG_RCGR_BMSK                        0x10
#define HWIO_MSS_CLK_SRC_UIM0_MND_REG_CMD_RCGR_DIRTY_CFG_RCGR_SHFT                         0x4
#define HWIO_MSS_CLK_SRC_UIM0_MND_REG_CMD_RCGR_ROOT_EN_BMSK                                0x2
#define HWIO_MSS_CLK_SRC_UIM0_MND_REG_CMD_RCGR_ROOT_EN_SHFT                                0x1
#define HWIO_MSS_CLK_SRC_UIM0_MND_REG_CMD_RCGR_UPDATE_BMSK                                 0x1
#define HWIO_MSS_CLK_SRC_UIM0_MND_REG_CMD_RCGR_UPDATE_SHFT                                 0x0

#define HWIO_MSS_CLK_SRC_UIM0_MND_REG_CFG_RCGR_ADDR                                 (MSS_MSS_CC_REG_REG_BASE      + 0x000004a0)
#define HWIO_MSS_CLK_SRC_UIM0_MND_REG_CFG_RCGR_OFFS                                 (MSS_MSS_CC_REG_REG_BASE_OFFS + 0x000004a0)
#define HWIO_MSS_CLK_SRC_UIM0_MND_REG_CFG_RCGR_RMSK                                   0x10371f
#define HWIO_MSS_CLK_SRC_UIM0_MND_REG_CFG_RCGR_IN          \
        in_dword_masked(HWIO_MSS_CLK_SRC_UIM0_MND_REG_CFG_RCGR_ADDR, HWIO_MSS_CLK_SRC_UIM0_MND_REG_CFG_RCGR_RMSK)
#define HWIO_MSS_CLK_SRC_UIM0_MND_REG_CFG_RCGR_INM(m)      \
        in_dword_masked(HWIO_MSS_CLK_SRC_UIM0_MND_REG_CFG_RCGR_ADDR, m)
#define HWIO_MSS_CLK_SRC_UIM0_MND_REG_CFG_RCGR_OUT(v)      \
        out_dword(HWIO_MSS_CLK_SRC_UIM0_MND_REG_CFG_RCGR_ADDR,v)
#define HWIO_MSS_CLK_SRC_UIM0_MND_REG_CFG_RCGR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_CLK_SRC_UIM0_MND_REG_CFG_RCGR_ADDR,m,v,HWIO_MSS_CLK_SRC_UIM0_MND_REG_CFG_RCGR_IN)
#define HWIO_MSS_CLK_SRC_UIM0_MND_REG_CFG_RCGR_HW_CLK_CONTROL_BMSK                    0x100000
#define HWIO_MSS_CLK_SRC_UIM0_MND_REG_CFG_RCGR_HW_CLK_CONTROL_SHFT                        0x14
#define HWIO_MSS_CLK_SRC_UIM0_MND_REG_CFG_RCGR_MODE_BMSK                                0x3000
#define HWIO_MSS_CLK_SRC_UIM0_MND_REG_CFG_RCGR_MODE_SHFT                                   0xc
#define HWIO_MSS_CLK_SRC_UIM0_MND_REG_CFG_RCGR_SRC_SEL_BMSK                              0x700
#define HWIO_MSS_CLK_SRC_UIM0_MND_REG_CFG_RCGR_SRC_SEL_SHFT                                0x8
#define HWIO_MSS_CLK_SRC_UIM0_MND_REG_CFG_RCGR_SRC_DIV_BMSK                               0x1f
#define HWIO_MSS_CLK_SRC_UIM0_MND_REG_CFG_RCGR_SRC_DIV_SHFT                                0x0

#define HWIO_MSS_CLK_SRC_UIM0_MND_REG_M_ADDR                                        (MSS_MSS_CC_REG_REG_BASE      + 0x000004a4)
#define HWIO_MSS_CLK_SRC_UIM0_MND_REG_M_OFFS                                        (MSS_MSS_CC_REG_REG_BASE_OFFS + 0x000004a4)
#define HWIO_MSS_CLK_SRC_UIM0_MND_REG_M_RMSK                                            0xffff
#define HWIO_MSS_CLK_SRC_UIM0_MND_REG_M_IN          \
        in_dword_masked(HWIO_MSS_CLK_SRC_UIM0_MND_REG_M_ADDR, HWIO_MSS_CLK_SRC_UIM0_MND_REG_M_RMSK)
#define HWIO_MSS_CLK_SRC_UIM0_MND_REG_M_INM(m)      \
        in_dword_masked(HWIO_MSS_CLK_SRC_UIM0_MND_REG_M_ADDR, m)
#define HWIO_MSS_CLK_SRC_UIM0_MND_REG_M_OUT(v)      \
        out_dword(HWIO_MSS_CLK_SRC_UIM0_MND_REG_M_ADDR,v)
#define HWIO_MSS_CLK_SRC_UIM0_MND_REG_M_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_CLK_SRC_UIM0_MND_REG_M_ADDR,m,v,HWIO_MSS_CLK_SRC_UIM0_MND_REG_M_IN)
#define HWIO_MSS_CLK_SRC_UIM0_MND_REG_M_M_BMSK                                          0xffff
#define HWIO_MSS_CLK_SRC_UIM0_MND_REG_M_M_SHFT                                             0x0

#define HWIO_MSS_CLK_SRC_UIM0_MND_REG_N_ADDR                                        (MSS_MSS_CC_REG_REG_BASE      + 0x000004a8)
#define HWIO_MSS_CLK_SRC_UIM0_MND_REG_N_OFFS                                        (MSS_MSS_CC_REG_REG_BASE_OFFS + 0x000004a8)
#define HWIO_MSS_CLK_SRC_UIM0_MND_REG_N_RMSK                                            0xffff
#define HWIO_MSS_CLK_SRC_UIM0_MND_REG_N_IN          \
        in_dword_masked(HWIO_MSS_CLK_SRC_UIM0_MND_REG_N_ADDR, HWIO_MSS_CLK_SRC_UIM0_MND_REG_N_RMSK)
#define HWIO_MSS_CLK_SRC_UIM0_MND_REG_N_INM(m)      \
        in_dword_masked(HWIO_MSS_CLK_SRC_UIM0_MND_REG_N_ADDR, m)
#define HWIO_MSS_CLK_SRC_UIM0_MND_REG_N_OUT(v)      \
        out_dword(HWIO_MSS_CLK_SRC_UIM0_MND_REG_N_ADDR,v)
#define HWIO_MSS_CLK_SRC_UIM0_MND_REG_N_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_CLK_SRC_UIM0_MND_REG_N_ADDR,m,v,HWIO_MSS_CLK_SRC_UIM0_MND_REG_N_IN)
#define HWIO_MSS_CLK_SRC_UIM0_MND_REG_N_NOT_N_MINUS_M_BMSK                              0xffff
#define HWIO_MSS_CLK_SRC_UIM0_MND_REG_N_NOT_N_MINUS_M_SHFT                                 0x0

#define HWIO_MSS_CLK_SRC_UIM0_MND_REG_D_ADDR                                        (MSS_MSS_CC_REG_REG_BASE      + 0x000004ac)
#define HWIO_MSS_CLK_SRC_UIM0_MND_REG_D_OFFS                                        (MSS_MSS_CC_REG_REG_BASE_OFFS + 0x000004ac)
#define HWIO_MSS_CLK_SRC_UIM0_MND_REG_D_RMSK                                            0xffff
#define HWIO_MSS_CLK_SRC_UIM0_MND_REG_D_IN          \
        in_dword_masked(HWIO_MSS_CLK_SRC_UIM0_MND_REG_D_ADDR, HWIO_MSS_CLK_SRC_UIM0_MND_REG_D_RMSK)
#define HWIO_MSS_CLK_SRC_UIM0_MND_REG_D_INM(m)      \
        in_dword_masked(HWIO_MSS_CLK_SRC_UIM0_MND_REG_D_ADDR, m)
#define HWIO_MSS_CLK_SRC_UIM0_MND_REG_D_OUT(v)      \
        out_dword(HWIO_MSS_CLK_SRC_UIM0_MND_REG_D_ADDR,v)
#define HWIO_MSS_CLK_SRC_UIM0_MND_REG_D_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_CLK_SRC_UIM0_MND_REG_D_ADDR,m,v,HWIO_MSS_CLK_SRC_UIM0_MND_REG_D_IN)
#define HWIO_MSS_CLK_SRC_UIM0_MND_REG_D_NOT_2D_BMSK                                     0xffff
#define HWIO_MSS_CLK_SRC_UIM0_MND_REG_D_NOT_2D_SHFT                                        0x0

#define HWIO_MSS_CLK_UART_BIT_UIM0_CBCR_ADDR                                        (MSS_MSS_CC_REG_REG_BASE      + 0x000004b0)
#define HWIO_MSS_CLK_UART_BIT_UIM0_CBCR_OFFS                                        (MSS_MSS_CC_REG_REG_BASE_OFFS + 0x000004b0)
#define HWIO_MSS_CLK_UART_BIT_UIM0_CBCR_RMSK                                        0x80000005
#define HWIO_MSS_CLK_UART_BIT_UIM0_CBCR_IN          \
        in_dword_masked(HWIO_MSS_CLK_UART_BIT_UIM0_CBCR_ADDR, HWIO_MSS_CLK_UART_BIT_UIM0_CBCR_RMSK)
#define HWIO_MSS_CLK_UART_BIT_UIM0_CBCR_INM(m)      \
        in_dword_masked(HWIO_MSS_CLK_UART_BIT_UIM0_CBCR_ADDR, m)
#define HWIO_MSS_CLK_UART_BIT_UIM0_CBCR_OUT(v)      \
        out_dword(HWIO_MSS_CLK_UART_BIT_UIM0_CBCR_ADDR,v)
#define HWIO_MSS_CLK_UART_BIT_UIM0_CBCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_CLK_UART_BIT_UIM0_CBCR_ADDR,m,v,HWIO_MSS_CLK_UART_BIT_UIM0_CBCR_IN)
#define HWIO_MSS_CLK_UART_BIT_UIM0_CBCR_CLK_OFF_BMSK                                0x80000000
#define HWIO_MSS_CLK_UART_BIT_UIM0_CBCR_CLK_OFF_SHFT                                      0x1f
#define HWIO_MSS_CLK_UART_BIT_UIM0_CBCR_CLK_ARES_BMSK                                      0x4
#define HWIO_MSS_CLK_UART_BIT_UIM0_CBCR_CLK_ARES_SHFT                                      0x2
#define HWIO_MSS_CLK_UART_BIT_UIM0_CBCR_CLK_ENABLE_BMSK                                    0x1
#define HWIO_MSS_CLK_UART_BIT_UIM0_CBCR_CLK_ENABLE_SHFT                                    0x0

#define HWIO_MSS_CLK_SRC_UIM1_MND_REG_CMD_RCGR_ADDR                                 (MSS_MSS_CC_REG_REG_BASE      + 0x000004b4)
#define HWIO_MSS_CLK_SRC_UIM1_MND_REG_CMD_RCGR_OFFS                                 (MSS_MSS_CC_REG_REG_BASE_OFFS + 0x000004b4)
#define HWIO_MSS_CLK_SRC_UIM1_MND_REG_CMD_RCGR_RMSK                                 0x800000f3
#define HWIO_MSS_CLK_SRC_UIM1_MND_REG_CMD_RCGR_IN          \
        in_dword_masked(HWIO_MSS_CLK_SRC_UIM1_MND_REG_CMD_RCGR_ADDR, HWIO_MSS_CLK_SRC_UIM1_MND_REG_CMD_RCGR_RMSK)
#define HWIO_MSS_CLK_SRC_UIM1_MND_REG_CMD_RCGR_INM(m)      \
        in_dword_masked(HWIO_MSS_CLK_SRC_UIM1_MND_REG_CMD_RCGR_ADDR, m)
#define HWIO_MSS_CLK_SRC_UIM1_MND_REG_CMD_RCGR_OUT(v)      \
        out_dword(HWIO_MSS_CLK_SRC_UIM1_MND_REG_CMD_RCGR_ADDR,v)
#define HWIO_MSS_CLK_SRC_UIM1_MND_REG_CMD_RCGR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_CLK_SRC_UIM1_MND_REG_CMD_RCGR_ADDR,m,v,HWIO_MSS_CLK_SRC_UIM1_MND_REG_CMD_RCGR_IN)
#define HWIO_MSS_CLK_SRC_UIM1_MND_REG_CMD_RCGR_ROOT_OFF_BMSK                        0x80000000
#define HWIO_MSS_CLK_SRC_UIM1_MND_REG_CMD_RCGR_ROOT_OFF_SHFT                              0x1f
#define HWIO_MSS_CLK_SRC_UIM1_MND_REG_CMD_RCGR_DIRTY_D_BMSK                               0x80
#define HWIO_MSS_CLK_SRC_UIM1_MND_REG_CMD_RCGR_DIRTY_D_SHFT                                0x7
#define HWIO_MSS_CLK_SRC_UIM1_MND_REG_CMD_RCGR_DIRTY_N_BMSK                               0x40
#define HWIO_MSS_CLK_SRC_UIM1_MND_REG_CMD_RCGR_DIRTY_N_SHFT                                0x6
#define HWIO_MSS_CLK_SRC_UIM1_MND_REG_CMD_RCGR_DIRTY_M_BMSK                               0x20
#define HWIO_MSS_CLK_SRC_UIM1_MND_REG_CMD_RCGR_DIRTY_M_SHFT                                0x5
#define HWIO_MSS_CLK_SRC_UIM1_MND_REG_CMD_RCGR_DIRTY_CFG_RCGR_BMSK                        0x10
#define HWIO_MSS_CLK_SRC_UIM1_MND_REG_CMD_RCGR_DIRTY_CFG_RCGR_SHFT                         0x4
#define HWIO_MSS_CLK_SRC_UIM1_MND_REG_CMD_RCGR_ROOT_EN_BMSK                                0x2
#define HWIO_MSS_CLK_SRC_UIM1_MND_REG_CMD_RCGR_ROOT_EN_SHFT                                0x1
#define HWIO_MSS_CLK_SRC_UIM1_MND_REG_CMD_RCGR_UPDATE_BMSK                                 0x1
#define HWIO_MSS_CLK_SRC_UIM1_MND_REG_CMD_RCGR_UPDATE_SHFT                                 0x0

#define HWIO_MSS_CLK_SRC_UIM1_MND_REG_CFG_RCGR_ADDR                                 (MSS_MSS_CC_REG_REG_BASE      + 0x000004b8)
#define HWIO_MSS_CLK_SRC_UIM1_MND_REG_CFG_RCGR_OFFS                                 (MSS_MSS_CC_REG_REG_BASE_OFFS + 0x000004b8)
#define HWIO_MSS_CLK_SRC_UIM1_MND_REG_CFG_RCGR_RMSK                                   0x10371f
#define HWIO_MSS_CLK_SRC_UIM1_MND_REG_CFG_RCGR_IN          \
        in_dword_masked(HWIO_MSS_CLK_SRC_UIM1_MND_REG_CFG_RCGR_ADDR, HWIO_MSS_CLK_SRC_UIM1_MND_REG_CFG_RCGR_RMSK)
#define HWIO_MSS_CLK_SRC_UIM1_MND_REG_CFG_RCGR_INM(m)      \
        in_dword_masked(HWIO_MSS_CLK_SRC_UIM1_MND_REG_CFG_RCGR_ADDR, m)
#define HWIO_MSS_CLK_SRC_UIM1_MND_REG_CFG_RCGR_OUT(v)      \
        out_dword(HWIO_MSS_CLK_SRC_UIM1_MND_REG_CFG_RCGR_ADDR,v)
#define HWIO_MSS_CLK_SRC_UIM1_MND_REG_CFG_RCGR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_CLK_SRC_UIM1_MND_REG_CFG_RCGR_ADDR,m,v,HWIO_MSS_CLK_SRC_UIM1_MND_REG_CFG_RCGR_IN)
#define HWIO_MSS_CLK_SRC_UIM1_MND_REG_CFG_RCGR_HW_CLK_CONTROL_BMSK                    0x100000
#define HWIO_MSS_CLK_SRC_UIM1_MND_REG_CFG_RCGR_HW_CLK_CONTROL_SHFT                        0x14
#define HWIO_MSS_CLK_SRC_UIM1_MND_REG_CFG_RCGR_MODE_BMSK                                0x3000
#define HWIO_MSS_CLK_SRC_UIM1_MND_REG_CFG_RCGR_MODE_SHFT                                   0xc
#define HWIO_MSS_CLK_SRC_UIM1_MND_REG_CFG_RCGR_SRC_SEL_BMSK                              0x700
#define HWIO_MSS_CLK_SRC_UIM1_MND_REG_CFG_RCGR_SRC_SEL_SHFT                                0x8
#define HWIO_MSS_CLK_SRC_UIM1_MND_REG_CFG_RCGR_SRC_DIV_BMSK                               0x1f
#define HWIO_MSS_CLK_SRC_UIM1_MND_REG_CFG_RCGR_SRC_DIV_SHFT                                0x0

#define HWIO_MSS_CLK_SRC_UIM1_MND_REG_M_ADDR                                        (MSS_MSS_CC_REG_REG_BASE      + 0x000004bc)
#define HWIO_MSS_CLK_SRC_UIM1_MND_REG_M_OFFS                                        (MSS_MSS_CC_REG_REG_BASE_OFFS + 0x000004bc)
#define HWIO_MSS_CLK_SRC_UIM1_MND_REG_M_RMSK                                            0xffff
#define HWIO_MSS_CLK_SRC_UIM1_MND_REG_M_IN          \
        in_dword_masked(HWIO_MSS_CLK_SRC_UIM1_MND_REG_M_ADDR, HWIO_MSS_CLK_SRC_UIM1_MND_REG_M_RMSK)
#define HWIO_MSS_CLK_SRC_UIM1_MND_REG_M_INM(m)      \
        in_dword_masked(HWIO_MSS_CLK_SRC_UIM1_MND_REG_M_ADDR, m)
#define HWIO_MSS_CLK_SRC_UIM1_MND_REG_M_OUT(v)      \
        out_dword(HWIO_MSS_CLK_SRC_UIM1_MND_REG_M_ADDR,v)
#define HWIO_MSS_CLK_SRC_UIM1_MND_REG_M_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_CLK_SRC_UIM1_MND_REG_M_ADDR,m,v,HWIO_MSS_CLK_SRC_UIM1_MND_REG_M_IN)
#define HWIO_MSS_CLK_SRC_UIM1_MND_REG_M_M_BMSK                                          0xffff
#define HWIO_MSS_CLK_SRC_UIM1_MND_REG_M_M_SHFT                                             0x0

#define HWIO_MSS_CLK_SRC_UIM1_MND_REG_N_ADDR                                        (MSS_MSS_CC_REG_REG_BASE      + 0x000004c0)
#define HWIO_MSS_CLK_SRC_UIM1_MND_REG_N_OFFS                                        (MSS_MSS_CC_REG_REG_BASE_OFFS + 0x000004c0)
#define HWIO_MSS_CLK_SRC_UIM1_MND_REG_N_RMSK                                            0xffff
#define HWIO_MSS_CLK_SRC_UIM1_MND_REG_N_IN          \
        in_dword_masked(HWIO_MSS_CLK_SRC_UIM1_MND_REG_N_ADDR, HWIO_MSS_CLK_SRC_UIM1_MND_REG_N_RMSK)
#define HWIO_MSS_CLK_SRC_UIM1_MND_REG_N_INM(m)      \
        in_dword_masked(HWIO_MSS_CLK_SRC_UIM1_MND_REG_N_ADDR, m)
#define HWIO_MSS_CLK_SRC_UIM1_MND_REG_N_OUT(v)      \
        out_dword(HWIO_MSS_CLK_SRC_UIM1_MND_REG_N_ADDR,v)
#define HWIO_MSS_CLK_SRC_UIM1_MND_REG_N_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_CLK_SRC_UIM1_MND_REG_N_ADDR,m,v,HWIO_MSS_CLK_SRC_UIM1_MND_REG_N_IN)
#define HWIO_MSS_CLK_SRC_UIM1_MND_REG_N_NOT_N_MINUS_M_BMSK                              0xffff
#define HWIO_MSS_CLK_SRC_UIM1_MND_REG_N_NOT_N_MINUS_M_SHFT                                 0x0

#define HWIO_MSS_CLK_SRC_UIM1_MND_REG_D_ADDR                                        (MSS_MSS_CC_REG_REG_BASE      + 0x000004c4)
#define HWIO_MSS_CLK_SRC_UIM1_MND_REG_D_OFFS                                        (MSS_MSS_CC_REG_REG_BASE_OFFS + 0x000004c4)
#define HWIO_MSS_CLK_SRC_UIM1_MND_REG_D_RMSK                                            0xffff
#define HWIO_MSS_CLK_SRC_UIM1_MND_REG_D_IN          \
        in_dword_masked(HWIO_MSS_CLK_SRC_UIM1_MND_REG_D_ADDR, HWIO_MSS_CLK_SRC_UIM1_MND_REG_D_RMSK)
#define HWIO_MSS_CLK_SRC_UIM1_MND_REG_D_INM(m)      \
        in_dword_masked(HWIO_MSS_CLK_SRC_UIM1_MND_REG_D_ADDR, m)
#define HWIO_MSS_CLK_SRC_UIM1_MND_REG_D_OUT(v)      \
        out_dword(HWIO_MSS_CLK_SRC_UIM1_MND_REG_D_ADDR,v)
#define HWIO_MSS_CLK_SRC_UIM1_MND_REG_D_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_CLK_SRC_UIM1_MND_REG_D_ADDR,m,v,HWIO_MSS_CLK_SRC_UIM1_MND_REG_D_IN)
#define HWIO_MSS_CLK_SRC_UIM1_MND_REG_D_NOT_2D_BMSK                                     0xffff
#define HWIO_MSS_CLK_SRC_UIM1_MND_REG_D_NOT_2D_SHFT                                        0x0

#define HWIO_MSS_CLK_UART_BIT_UIM1_CBCR_ADDR                                        (MSS_MSS_CC_REG_REG_BASE      + 0x000004c8)
#define HWIO_MSS_CLK_UART_BIT_UIM1_CBCR_OFFS                                        (MSS_MSS_CC_REG_REG_BASE_OFFS + 0x000004c8)
#define HWIO_MSS_CLK_UART_BIT_UIM1_CBCR_RMSK                                        0x80000005
#define HWIO_MSS_CLK_UART_BIT_UIM1_CBCR_IN          \
        in_dword_masked(HWIO_MSS_CLK_UART_BIT_UIM1_CBCR_ADDR, HWIO_MSS_CLK_UART_BIT_UIM1_CBCR_RMSK)
#define HWIO_MSS_CLK_UART_BIT_UIM1_CBCR_INM(m)      \
        in_dword_masked(HWIO_MSS_CLK_UART_BIT_UIM1_CBCR_ADDR, m)
#define HWIO_MSS_CLK_UART_BIT_UIM1_CBCR_OUT(v)      \
        out_dword(HWIO_MSS_CLK_UART_BIT_UIM1_CBCR_ADDR,v)
#define HWIO_MSS_CLK_UART_BIT_UIM1_CBCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_CLK_UART_BIT_UIM1_CBCR_ADDR,m,v,HWIO_MSS_CLK_UART_BIT_UIM1_CBCR_IN)
#define HWIO_MSS_CLK_UART_BIT_UIM1_CBCR_CLK_OFF_BMSK                                0x80000000
#define HWIO_MSS_CLK_UART_BIT_UIM1_CBCR_CLK_OFF_SHFT                                      0x1f
#define HWIO_MSS_CLK_UART_BIT_UIM1_CBCR_CLK_ARES_BMSK                                      0x4
#define HWIO_MSS_CLK_UART_BIT_UIM1_CBCR_CLK_ARES_SHFT                                      0x2
#define HWIO_MSS_CLK_UART_BIT_UIM1_CBCR_CLK_ENABLE_BMSK                                    0x1
#define HWIO_MSS_CLK_UART_BIT_UIM1_CBCR_CLK_ENABLE_SHFT                                    0x0

#define HWIO_MSS_CLK_SRC_BIT_COXM_MND_REG_CMD_RCGR_ADDR                             (MSS_MSS_CC_REG_REG_BASE      + 0x000004cc)
#define HWIO_MSS_CLK_SRC_BIT_COXM_MND_REG_CMD_RCGR_OFFS                             (MSS_MSS_CC_REG_REG_BASE_OFFS + 0x000004cc)
#define HWIO_MSS_CLK_SRC_BIT_COXM_MND_REG_CMD_RCGR_RMSK                             0x800000f3
#define HWIO_MSS_CLK_SRC_BIT_COXM_MND_REG_CMD_RCGR_IN          \
        in_dword_masked(HWIO_MSS_CLK_SRC_BIT_COXM_MND_REG_CMD_RCGR_ADDR, HWIO_MSS_CLK_SRC_BIT_COXM_MND_REG_CMD_RCGR_RMSK)
#define HWIO_MSS_CLK_SRC_BIT_COXM_MND_REG_CMD_RCGR_INM(m)      \
        in_dword_masked(HWIO_MSS_CLK_SRC_BIT_COXM_MND_REG_CMD_RCGR_ADDR, m)
#define HWIO_MSS_CLK_SRC_BIT_COXM_MND_REG_CMD_RCGR_OUT(v)      \
        out_dword(HWIO_MSS_CLK_SRC_BIT_COXM_MND_REG_CMD_RCGR_ADDR,v)
#define HWIO_MSS_CLK_SRC_BIT_COXM_MND_REG_CMD_RCGR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_CLK_SRC_BIT_COXM_MND_REG_CMD_RCGR_ADDR,m,v,HWIO_MSS_CLK_SRC_BIT_COXM_MND_REG_CMD_RCGR_IN)
#define HWIO_MSS_CLK_SRC_BIT_COXM_MND_REG_CMD_RCGR_ROOT_OFF_BMSK                    0x80000000
#define HWIO_MSS_CLK_SRC_BIT_COXM_MND_REG_CMD_RCGR_ROOT_OFF_SHFT                          0x1f
#define HWIO_MSS_CLK_SRC_BIT_COXM_MND_REG_CMD_RCGR_DIRTY_D_BMSK                           0x80
#define HWIO_MSS_CLK_SRC_BIT_COXM_MND_REG_CMD_RCGR_DIRTY_D_SHFT                            0x7
#define HWIO_MSS_CLK_SRC_BIT_COXM_MND_REG_CMD_RCGR_DIRTY_N_BMSK                           0x40
#define HWIO_MSS_CLK_SRC_BIT_COXM_MND_REG_CMD_RCGR_DIRTY_N_SHFT                            0x6
#define HWIO_MSS_CLK_SRC_BIT_COXM_MND_REG_CMD_RCGR_DIRTY_M_BMSK                           0x20
#define HWIO_MSS_CLK_SRC_BIT_COXM_MND_REG_CMD_RCGR_DIRTY_M_SHFT                            0x5
#define HWIO_MSS_CLK_SRC_BIT_COXM_MND_REG_CMD_RCGR_DIRTY_CFG_RCGR_BMSK                    0x10
#define HWIO_MSS_CLK_SRC_BIT_COXM_MND_REG_CMD_RCGR_DIRTY_CFG_RCGR_SHFT                     0x4
#define HWIO_MSS_CLK_SRC_BIT_COXM_MND_REG_CMD_RCGR_ROOT_EN_BMSK                            0x2
#define HWIO_MSS_CLK_SRC_BIT_COXM_MND_REG_CMD_RCGR_ROOT_EN_SHFT                            0x1
#define HWIO_MSS_CLK_SRC_BIT_COXM_MND_REG_CMD_RCGR_UPDATE_BMSK                             0x1
#define HWIO_MSS_CLK_SRC_BIT_COXM_MND_REG_CMD_RCGR_UPDATE_SHFT                             0x0

#define HWIO_MSS_CLK_SRC_BIT_COXM_MND_REG_CFG_RCGR_ADDR                             (MSS_MSS_CC_REG_REG_BASE      + 0x000004d0)
#define HWIO_MSS_CLK_SRC_BIT_COXM_MND_REG_CFG_RCGR_OFFS                             (MSS_MSS_CC_REG_REG_BASE_OFFS + 0x000004d0)
#define HWIO_MSS_CLK_SRC_BIT_COXM_MND_REG_CFG_RCGR_RMSK                               0x10371f
#define HWIO_MSS_CLK_SRC_BIT_COXM_MND_REG_CFG_RCGR_IN          \
        in_dword_masked(HWIO_MSS_CLK_SRC_BIT_COXM_MND_REG_CFG_RCGR_ADDR, HWIO_MSS_CLK_SRC_BIT_COXM_MND_REG_CFG_RCGR_RMSK)
#define HWIO_MSS_CLK_SRC_BIT_COXM_MND_REG_CFG_RCGR_INM(m)      \
        in_dword_masked(HWIO_MSS_CLK_SRC_BIT_COXM_MND_REG_CFG_RCGR_ADDR, m)
#define HWIO_MSS_CLK_SRC_BIT_COXM_MND_REG_CFG_RCGR_OUT(v)      \
        out_dword(HWIO_MSS_CLK_SRC_BIT_COXM_MND_REG_CFG_RCGR_ADDR,v)
#define HWIO_MSS_CLK_SRC_BIT_COXM_MND_REG_CFG_RCGR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_CLK_SRC_BIT_COXM_MND_REG_CFG_RCGR_ADDR,m,v,HWIO_MSS_CLK_SRC_BIT_COXM_MND_REG_CFG_RCGR_IN)
#define HWIO_MSS_CLK_SRC_BIT_COXM_MND_REG_CFG_RCGR_HW_CLK_CONTROL_BMSK                0x100000
#define HWIO_MSS_CLK_SRC_BIT_COXM_MND_REG_CFG_RCGR_HW_CLK_CONTROL_SHFT                    0x14
#define HWIO_MSS_CLK_SRC_BIT_COXM_MND_REG_CFG_RCGR_MODE_BMSK                            0x3000
#define HWIO_MSS_CLK_SRC_BIT_COXM_MND_REG_CFG_RCGR_MODE_SHFT                               0xc
#define HWIO_MSS_CLK_SRC_BIT_COXM_MND_REG_CFG_RCGR_SRC_SEL_BMSK                          0x700
#define HWIO_MSS_CLK_SRC_BIT_COXM_MND_REG_CFG_RCGR_SRC_SEL_SHFT                            0x8
#define HWIO_MSS_CLK_SRC_BIT_COXM_MND_REG_CFG_RCGR_SRC_DIV_BMSK                           0x1f
#define HWIO_MSS_CLK_SRC_BIT_COXM_MND_REG_CFG_RCGR_SRC_DIV_SHFT                            0x0

#define HWIO_MSS_CLK_SRC_BIT_COXM_MND_REG_M_ADDR                                    (MSS_MSS_CC_REG_REG_BASE      + 0x000004d4)
#define HWIO_MSS_CLK_SRC_BIT_COXM_MND_REG_M_OFFS                                    (MSS_MSS_CC_REG_REG_BASE_OFFS + 0x000004d4)
#define HWIO_MSS_CLK_SRC_BIT_COXM_MND_REG_M_RMSK                                        0xffff
#define HWIO_MSS_CLK_SRC_BIT_COXM_MND_REG_M_IN          \
        in_dword_masked(HWIO_MSS_CLK_SRC_BIT_COXM_MND_REG_M_ADDR, HWIO_MSS_CLK_SRC_BIT_COXM_MND_REG_M_RMSK)
#define HWIO_MSS_CLK_SRC_BIT_COXM_MND_REG_M_INM(m)      \
        in_dword_masked(HWIO_MSS_CLK_SRC_BIT_COXM_MND_REG_M_ADDR, m)
#define HWIO_MSS_CLK_SRC_BIT_COXM_MND_REG_M_OUT(v)      \
        out_dword(HWIO_MSS_CLK_SRC_BIT_COXM_MND_REG_M_ADDR,v)
#define HWIO_MSS_CLK_SRC_BIT_COXM_MND_REG_M_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_CLK_SRC_BIT_COXM_MND_REG_M_ADDR,m,v,HWIO_MSS_CLK_SRC_BIT_COXM_MND_REG_M_IN)
#define HWIO_MSS_CLK_SRC_BIT_COXM_MND_REG_M_M_BMSK                                      0xffff
#define HWIO_MSS_CLK_SRC_BIT_COXM_MND_REG_M_M_SHFT                                         0x0

#define HWIO_MSS_CLK_SRC_BIT_COXM_MND_REG_N_ADDR                                    (MSS_MSS_CC_REG_REG_BASE      + 0x000004d8)
#define HWIO_MSS_CLK_SRC_BIT_COXM_MND_REG_N_OFFS                                    (MSS_MSS_CC_REG_REG_BASE_OFFS + 0x000004d8)
#define HWIO_MSS_CLK_SRC_BIT_COXM_MND_REG_N_RMSK                                        0xffff
#define HWIO_MSS_CLK_SRC_BIT_COXM_MND_REG_N_IN          \
        in_dword_masked(HWIO_MSS_CLK_SRC_BIT_COXM_MND_REG_N_ADDR, HWIO_MSS_CLK_SRC_BIT_COXM_MND_REG_N_RMSK)
#define HWIO_MSS_CLK_SRC_BIT_COXM_MND_REG_N_INM(m)      \
        in_dword_masked(HWIO_MSS_CLK_SRC_BIT_COXM_MND_REG_N_ADDR, m)
#define HWIO_MSS_CLK_SRC_BIT_COXM_MND_REG_N_OUT(v)      \
        out_dword(HWIO_MSS_CLK_SRC_BIT_COXM_MND_REG_N_ADDR,v)
#define HWIO_MSS_CLK_SRC_BIT_COXM_MND_REG_N_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_CLK_SRC_BIT_COXM_MND_REG_N_ADDR,m,v,HWIO_MSS_CLK_SRC_BIT_COXM_MND_REG_N_IN)
#define HWIO_MSS_CLK_SRC_BIT_COXM_MND_REG_N_NOT_N_MINUS_M_BMSK                          0xffff
#define HWIO_MSS_CLK_SRC_BIT_COXM_MND_REG_N_NOT_N_MINUS_M_SHFT                             0x0

#define HWIO_MSS_CLK_SRC_BIT_COXM_MND_REG_D_ADDR                                    (MSS_MSS_CC_REG_REG_BASE      + 0x000004dc)
#define HWIO_MSS_CLK_SRC_BIT_COXM_MND_REG_D_OFFS                                    (MSS_MSS_CC_REG_REG_BASE_OFFS + 0x000004dc)
#define HWIO_MSS_CLK_SRC_BIT_COXM_MND_REG_D_RMSK                                        0xffff
#define HWIO_MSS_CLK_SRC_BIT_COXM_MND_REG_D_IN          \
        in_dword_masked(HWIO_MSS_CLK_SRC_BIT_COXM_MND_REG_D_ADDR, HWIO_MSS_CLK_SRC_BIT_COXM_MND_REG_D_RMSK)
#define HWIO_MSS_CLK_SRC_BIT_COXM_MND_REG_D_INM(m)      \
        in_dword_masked(HWIO_MSS_CLK_SRC_BIT_COXM_MND_REG_D_ADDR, m)
#define HWIO_MSS_CLK_SRC_BIT_COXM_MND_REG_D_OUT(v)      \
        out_dword(HWIO_MSS_CLK_SRC_BIT_COXM_MND_REG_D_ADDR,v)
#define HWIO_MSS_CLK_SRC_BIT_COXM_MND_REG_D_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_CLK_SRC_BIT_COXM_MND_REG_D_ADDR,m,v,HWIO_MSS_CLK_SRC_BIT_COXM_MND_REG_D_IN)
#define HWIO_MSS_CLK_SRC_BIT_COXM_MND_REG_D_NOT_2D_BMSK                                 0xffff
#define HWIO_MSS_CLK_SRC_BIT_COXM_MND_REG_D_NOT_2D_SHFT                                    0x0

#define HWIO_MSS_CLK_BIT_COXM_CBCR_ADDR                                             (MSS_MSS_CC_REG_REG_BASE      + 0x000004e0)
#define HWIO_MSS_CLK_BIT_COXM_CBCR_OFFS                                             (MSS_MSS_CC_REG_REG_BASE_OFFS + 0x000004e0)
#define HWIO_MSS_CLK_BIT_COXM_CBCR_RMSK                                             0x80000005
#define HWIO_MSS_CLK_BIT_COXM_CBCR_IN          \
        in_dword_masked(HWIO_MSS_CLK_BIT_COXM_CBCR_ADDR, HWIO_MSS_CLK_BIT_COXM_CBCR_RMSK)
#define HWIO_MSS_CLK_BIT_COXM_CBCR_INM(m)      \
        in_dword_masked(HWIO_MSS_CLK_BIT_COXM_CBCR_ADDR, m)
#define HWIO_MSS_CLK_BIT_COXM_CBCR_OUT(v)      \
        out_dword(HWIO_MSS_CLK_BIT_COXM_CBCR_ADDR,v)
#define HWIO_MSS_CLK_BIT_COXM_CBCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_CLK_BIT_COXM_CBCR_ADDR,m,v,HWIO_MSS_CLK_BIT_COXM_CBCR_IN)
#define HWIO_MSS_CLK_BIT_COXM_CBCR_CLK_OFF_BMSK                                     0x80000000
#define HWIO_MSS_CLK_BIT_COXM_CBCR_CLK_OFF_SHFT                                           0x1f
#define HWIO_MSS_CLK_BIT_COXM_CBCR_CLK_ARES_BMSK                                           0x4
#define HWIO_MSS_CLK_BIT_COXM_CBCR_CLK_ARES_SHFT                                           0x2
#define HWIO_MSS_CLK_BIT_COXM_CBCR_CLK_ENABLE_BMSK                                         0x1
#define HWIO_MSS_CLK_BIT_COXM_CBCR_CLK_ENABLE_SHFT                                         0x0

/*----------------------------------------------------------------------------
 * MODULE: MSS_QDSP6V66SS_PUB
 *--------------------------------------------------------------------------*/

#define MSS_QDSP6V66SS_PUB_REG_BASE                                                 (MODEM_TOP_BASE      + 0x00080000)
#define MSS_QDSP6V66SS_PUB_REG_BASE_OFFS                                            0x00080000

#define HWIO_MSS_QDSP6SS_VERSION_ADDR                                               (MSS_QDSP6V66SS_PUB_REG_BASE      + 0x00000000)
#define HWIO_MSS_QDSP6SS_VERSION_OFFS                                               (MSS_QDSP6V66SS_PUB_REG_BASE_OFFS + 0x00000000)
#define HWIO_MSS_QDSP6SS_VERSION_RMSK                                               0xffffffff
#define HWIO_MSS_QDSP6SS_VERSION_IN          \
        in_dword_masked(HWIO_MSS_QDSP6SS_VERSION_ADDR, HWIO_MSS_QDSP6SS_VERSION_RMSK)
#define HWIO_MSS_QDSP6SS_VERSION_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_VERSION_ADDR, m)
#define HWIO_MSS_QDSP6SS_VERSION_MAJOR_BMSK                                         0xf0000000
#define HWIO_MSS_QDSP6SS_VERSION_MAJOR_SHFT                                               0x1c
#define HWIO_MSS_QDSP6SS_VERSION_MINOR_BMSK                                          0xfff0000
#define HWIO_MSS_QDSP6SS_VERSION_MINOR_SHFT                                               0x10
#define HWIO_MSS_QDSP6SS_VERSION_STEP_BMSK                                              0xffff
#define HWIO_MSS_QDSP6SS_VERSION_STEP_SHFT                                                 0x0

#define HWIO_MSS_QDSP6SS_RST_EVB_ADDR                                               (MSS_QDSP6V66SS_PUB_REG_BASE      + 0x00000010)
#define HWIO_MSS_QDSP6SS_RST_EVB_OFFS                                               (MSS_QDSP6V66SS_PUB_REG_BASE_OFFS + 0x00000010)
#define HWIO_MSS_QDSP6SS_RST_EVB_RMSK                                                0xffffff0
#define HWIO_MSS_QDSP6SS_RST_EVB_IN          \
        in_dword_masked(HWIO_MSS_QDSP6SS_RST_EVB_ADDR, HWIO_MSS_QDSP6SS_RST_EVB_RMSK)
#define HWIO_MSS_QDSP6SS_RST_EVB_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_RST_EVB_ADDR, m)
#define HWIO_MSS_QDSP6SS_RST_EVB_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_RST_EVB_ADDR,v)
#define HWIO_MSS_QDSP6SS_RST_EVB_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_RST_EVB_ADDR,m,v,HWIO_MSS_QDSP6SS_RST_EVB_IN)
#define HWIO_MSS_QDSP6SS_RST_EVB_EVB_BMSK                                            0xffffff0
#define HWIO_MSS_QDSP6SS_RST_EVB_EVB_SHFT                                                  0x4

#define HWIO_MSS_QDSP6SS_DBG_CFG_ADDR                                               (MSS_QDSP6V66SS_PUB_REG_BASE      + 0x00000018)
#define HWIO_MSS_QDSP6SS_DBG_CFG_OFFS                                               (MSS_QDSP6V66SS_PUB_REG_BASE_OFFS + 0x00000018)
#define HWIO_MSS_QDSP6SS_DBG_CFG_RMSK                                               0xffff0002
#define HWIO_MSS_QDSP6SS_DBG_CFG_IN          \
        in_dword_masked(HWIO_MSS_QDSP6SS_DBG_CFG_ADDR, HWIO_MSS_QDSP6SS_DBG_CFG_RMSK)
#define HWIO_MSS_QDSP6SS_DBG_CFG_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_DBG_CFG_ADDR, m)
#define HWIO_MSS_QDSP6SS_DBG_CFG_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_DBG_CFG_ADDR,v)
#define HWIO_MSS_QDSP6SS_DBG_CFG_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_DBG_CFG_ADDR,m,v,HWIO_MSS_QDSP6SS_DBG_CFG_IN)
#define HWIO_MSS_QDSP6SS_DBG_CFG_DBG_SW_REG_BMSK                                    0xff000000
#define HWIO_MSS_QDSP6SS_DBG_CFG_DBG_SW_REG_SHFT                                          0x18
#define HWIO_MSS_QDSP6SS_DBG_CFG_DBG_SPARE_BMSK                                       0xff0000
#define HWIO_MSS_QDSP6SS_DBG_CFG_DBG_SPARE_SHFT                                           0x10
#define HWIO_MSS_QDSP6SS_DBG_CFG_CTI_TIHS_ENA_BMSK                                         0x2
#define HWIO_MSS_QDSP6SS_DBG_CFG_CTI_TIHS_ENA_SHFT                                         0x1

#define HWIO_MSS_QDSP6SS_RET_CFG_ADDR                                               (MSS_QDSP6V66SS_PUB_REG_BASE      + 0x0000001c)
#define HWIO_MSS_QDSP6SS_RET_CFG_OFFS                                               (MSS_QDSP6V66SS_PUB_REG_BASE_OFFS + 0x0000001c)
#define HWIO_MSS_QDSP6SS_RET_CFG_RMSK                                                      0x1
#define HWIO_MSS_QDSP6SS_RET_CFG_IN          \
        in_dword_masked(HWIO_MSS_QDSP6SS_RET_CFG_ADDR, HWIO_MSS_QDSP6SS_RET_CFG_RMSK)
#define HWIO_MSS_QDSP6SS_RET_CFG_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_RET_CFG_ADDR, m)
#define HWIO_MSS_QDSP6SS_RET_CFG_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_RET_CFG_ADDR,v)
#define HWIO_MSS_QDSP6SS_RET_CFG_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_RET_CFG_ADDR,m,v,HWIO_MSS_QDSP6SS_RET_CFG_IN)
#define HWIO_MSS_QDSP6SS_RET_CFG_RET_ARES_ENA_BMSK                                         0x1
#define HWIO_MSS_QDSP6SS_RET_CFG_RET_ARES_ENA_SHFT                                         0x0

#define HWIO_MSS_QDSP6SS_CORE_CBCR_ADDR                                             (MSS_QDSP6V66SS_PUB_REG_BASE      + 0x00000020)
#define HWIO_MSS_QDSP6SS_CORE_CBCR_OFFS                                             (MSS_QDSP6V66SS_PUB_REG_BASE_OFFS + 0x00000020)
#define HWIO_MSS_QDSP6SS_CORE_CBCR_RMSK                                             0x80000001
#define HWIO_MSS_QDSP6SS_CORE_CBCR_IN          \
        in_dword_masked(HWIO_MSS_QDSP6SS_CORE_CBCR_ADDR, HWIO_MSS_QDSP6SS_CORE_CBCR_RMSK)
#define HWIO_MSS_QDSP6SS_CORE_CBCR_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_CORE_CBCR_ADDR, m)
#define HWIO_MSS_QDSP6SS_CORE_CBCR_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_CORE_CBCR_ADDR,v)
#define HWIO_MSS_QDSP6SS_CORE_CBCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_CORE_CBCR_ADDR,m,v,HWIO_MSS_QDSP6SS_CORE_CBCR_IN)
#define HWIO_MSS_QDSP6SS_CORE_CBCR_CLKOFF_BMSK                                      0x80000000
#define HWIO_MSS_QDSP6SS_CORE_CBCR_CLKOFF_SHFT                                            0x1f
#define HWIO_MSS_QDSP6SS_CORE_CBCR_CLKEN_BMSK                                              0x1
#define HWIO_MSS_QDSP6SS_CORE_CBCR_CLKEN_SHFT                                              0x0

#define HWIO_MSS_QDSP6SS_CORE_CMD_RCGR_ADDR                                         (MSS_QDSP6V66SS_PUB_REG_BASE      + 0x00000028)
#define HWIO_MSS_QDSP6SS_CORE_CMD_RCGR_OFFS                                         (MSS_QDSP6V66SS_PUB_REG_BASE_OFFS + 0x00000028)
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

#define HWIO_MSS_QDSP6SS_CORE_CFG_RCGR_ADDR                                         (MSS_QDSP6V66SS_PUB_REG_BASE      + 0x0000002c)
#define HWIO_MSS_QDSP6SS_CORE_CFG_RCGR_OFFS                                         (MSS_QDSP6V66SS_PUB_REG_BASE_OFFS + 0x0000002c)
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

#define HWIO_MSS_QDSP6SS_CLOCK_SPDM_MON_ADDR                                        (MSS_QDSP6V66SS_PUB_REG_BASE      + 0x00000048)
#define HWIO_MSS_QDSP6SS_CLOCK_SPDM_MON_OFFS                                        (MSS_QDSP6V66SS_PUB_REG_BASE_OFFS + 0x00000048)
#define HWIO_MSS_QDSP6SS_CLOCK_SPDM_MON_RMSK                                               0x1
#define HWIO_MSS_QDSP6SS_CLOCK_SPDM_MON_IN          \
        in_dword_masked(HWIO_MSS_QDSP6SS_CLOCK_SPDM_MON_ADDR, HWIO_MSS_QDSP6SS_CLOCK_SPDM_MON_RMSK)
#define HWIO_MSS_QDSP6SS_CLOCK_SPDM_MON_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_CLOCK_SPDM_MON_ADDR, m)
#define HWIO_MSS_QDSP6SS_CLOCK_SPDM_MON_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_CLOCK_SPDM_MON_ADDR,v)
#define HWIO_MSS_QDSP6SS_CLOCK_SPDM_MON_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_CLOCK_SPDM_MON_ADDR,m,v,HWIO_MSS_QDSP6SS_CLOCK_SPDM_MON_IN)
#define HWIO_MSS_QDSP6SS_CLOCK_SPDM_MON_Q6_MON_CLKEN_BMSK                                  0x1
#define HWIO_MSS_QDSP6SS_CLOCK_SPDM_MON_Q6_MON_CLKEN_SHFT                                  0x0

#define HWIO_MSS_QDSP6SS_XO_CBCR_ADDR                                               (MSS_QDSP6V66SS_PUB_REG_BASE      + 0x00000038)
#define HWIO_MSS_QDSP6SS_XO_CBCR_OFFS                                               (MSS_QDSP6V66SS_PUB_REG_BASE_OFFS + 0x00000038)
#define HWIO_MSS_QDSP6SS_XO_CBCR_RMSK                                               0x80000001
#define HWIO_MSS_QDSP6SS_XO_CBCR_IN          \
        in_dword_masked(HWIO_MSS_QDSP6SS_XO_CBCR_ADDR, HWIO_MSS_QDSP6SS_XO_CBCR_RMSK)
#define HWIO_MSS_QDSP6SS_XO_CBCR_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_XO_CBCR_ADDR, m)
#define HWIO_MSS_QDSP6SS_XO_CBCR_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_XO_CBCR_ADDR,v)
#define HWIO_MSS_QDSP6SS_XO_CBCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_XO_CBCR_ADDR,m,v,HWIO_MSS_QDSP6SS_XO_CBCR_IN)
#define HWIO_MSS_QDSP6SS_XO_CBCR_CLKOFF_BMSK                                        0x80000000
#define HWIO_MSS_QDSP6SS_XO_CBCR_CLKOFF_SHFT                                              0x1f
#define HWIO_MSS_QDSP6SS_XO_CBCR_CLKEN_BMSK                                                0x1
#define HWIO_MSS_QDSP6SS_XO_CBCR_CLKEN_SHFT                                                0x0

#define HWIO_MSS_QDSP6SS_SLEEP_CBCR_ADDR                                            (MSS_QDSP6V66SS_PUB_REG_BASE      + 0x0000003c)
#define HWIO_MSS_QDSP6SS_SLEEP_CBCR_OFFS                                            (MSS_QDSP6V66SS_PUB_REG_BASE_OFFS + 0x0000003c)
#define HWIO_MSS_QDSP6SS_SLEEP_CBCR_RMSK                                            0x80000001
#define HWIO_MSS_QDSP6SS_SLEEP_CBCR_IN          \
        in_dword_masked(HWIO_MSS_QDSP6SS_SLEEP_CBCR_ADDR, HWIO_MSS_QDSP6SS_SLEEP_CBCR_RMSK)
#define HWIO_MSS_QDSP6SS_SLEEP_CBCR_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_SLEEP_CBCR_ADDR, m)
#define HWIO_MSS_QDSP6SS_SLEEP_CBCR_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_SLEEP_CBCR_ADDR,v)
#define HWIO_MSS_QDSP6SS_SLEEP_CBCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_SLEEP_CBCR_ADDR,m,v,HWIO_MSS_QDSP6SS_SLEEP_CBCR_IN)
#define HWIO_MSS_QDSP6SS_SLEEP_CBCR_CLKOFF_BMSK                                     0x80000000
#define HWIO_MSS_QDSP6SS_SLEEP_CBCR_CLKOFF_SHFT                                           0x1f
#define HWIO_MSS_QDSP6SS_SLEEP_CBCR_CLKEN_BMSK                                             0x1
#define HWIO_MSS_QDSP6SS_SLEEP_CBCR_CLKEN_SHFT                                             0x0

#define HWIO_MSS_QDSP6SS_NMI_ADDR                                                   (MSS_QDSP6V66SS_PUB_REG_BASE      + 0x00000040)
#define HWIO_MSS_QDSP6SS_NMI_OFFS                                                   (MSS_QDSP6V66SS_PUB_REG_BASE_OFFS + 0x00000040)
#define HWIO_MSS_QDSP6SS_NMI_RMSK                                                          0x3
#define HWIO_MSS_QDSP6SS_NMI_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_NMI_ADDR,v)
#define HWIO_MSS_QDSP6SS_NMI_CLEAR_STATUS_BMSK                                             0x2
#define HWIO_MSS_QDSP6SS_NMI_CLEAR_STATUS_SHFT                                             0x1
#define HWIO_MSS_QDSP6SS_NMI_SET_NMI_BMSK                                                  0x1
#define HWIO_MSS_QDSP6SS_NMI_SET_NMI_SHFT                                                  0x0

#define HWIO_MSS_QDSP6SS_NMI_STATUS_ADDR                                            (MSS_QDSP6V66SS_PUB_REG_BASE      + 0x00000044)
#define HWIO_MSS_QDSP6SS_NMI_STATUS_OFFS                                            (MSS_QDSP6V66SS_PUB_REG_BASE_OFFS + 0x00000044)
#define HWIO_MSS_QDSP6SS_NMI_STATUS_RMSK                                                   0x7
#define HWIO_MSS_QDSP6SS_NMI_STATUS_IN          \
        in_dword_masked(HWIO_MSS_QDSP6SS_NMI_STATUS_ADDR, HWIO_MSS_QDSP6SS_NMI_STATUS_RMSK)
#define HWIO_MSS_QDSP6SS_NMI_STATUS_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_NMI_STATUS_ADDR, m)
#define HWIO_MSS_QDSP6SS_NMI_STATUS_EXT_DBG_TRIG_BMSK                                      0x4
#define HWIO_MSS_QDSP6SS_NMI_STATUS_EXT_DBG_TRIG_SHFT                                      0x2
#define HWIO_MSS_QDSP6SS_NMI_STATUS_WDOG_TRIG_BMSK                                         0x2
#define HWIO_MSS_QDSP6SS_NMI_STATUS_WDOG_TRIG_SHFT                                         0x1
#define HWIO_MSS_QDSP6SS_NMI_STATUS_PUBCSR_TRIG_BMSK                                       0x1
#define HWIO_MSS_QDSP6SS_NMI_STATUS_PUBCSR_TRIG_SHFT                                       0x0

#define HWIO_MSS_QDSP6SS_INTF_HALTREQ_ADDR                                          (MSS_QDSP6V66SS_PUB_REG_BASE      + 0x00000088)
#define HWIO_MSS_QDSP6SS_INTF_HALTREQ_OFFS                                          (MSS_QDSP6V66SS_PUB_REG_BASE_OFFS + 0x00000088)
#define HWIO_MSS_QDSP6SS_INTF_HALTREQ_RMSK                                                 0x7
#define HWIO_MSS_QDSP6SS_INTF_HALTREQ_IN          \
        in_dword_masked(HWIO_MSS_QDSP6SS_INTF_HALTREQ_ADDR, HWIO_MSS_QDSP6SS_INTF_HALTREQ_RMSK)
#define HWIO_MSS_QDSP6SS_INTF_HALTREQ_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_INTF_HALTREQ_ADDR, m)
#define HWIO_MSS_QDSP6SS_INTF_HALTREQ_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_INTF_HALTREQ_ADDR,v)
#define HWIO_MSS_QDSP6SS_INTF_HALTREQ_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_INTF_HALTREQ_ADDR,m,v,HWIO_MSS_QDSP6SS_INTF_HALTREQ_IN)
#define HWIO_MSS_QDSP6SS_INTF_HALTREQ_AXIM2_BMSK                                           0x4
#define HWIO_MSS_QDSP6SS_INTF_HALTREQ_AXIM2_SHFT                                           0x2
#define HWIO_MSS_QDSP6SS_INTF_HALTREQ_AXIM_BMSK                                            0x2
#define HWIO_MSS_QDSP6SS_INTF_HALTREQ_AXIM_SHFT                                            0x1
#define HWIO_MSS_QDSP6SS_INTF_HALTREQ_ALL_BMSK                                             0x1
#define HWIO_MSS_QDSP6SS_INTF_HALTREQ_ALL_SHFT                                             0x0

#define HWIO_MSS_QDSP6SS_INTF_HALTACK_ADDR                                          (MSS_QDSP6V66SS_PUB_REG_BASE      + 0x0000008c)
#define HWIO_MSS_QDSP6SS_INTF_HALTACK_OFFS                                          (MSS_QDSP6V66SS_PUB_REG_BASE_OFFS + 0x0000008c)
#define HWIO_MSS_QDSP6SS_INTF_HALTACK_RMSK                                                 0x7
#define HWIO_MSS_QDSP6SS_INTF_HALTACK_IN          \
        in_dword_masked(HWIO_MSS_QDSP6SS_INTF_HALTACK_ADDR, HWIO_MSS_QDSP6SS_INTF_HALTACK_RMSK)
#define HWIO_MSS_QDSP6SS_INTF_HALTACK_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_INTF_HALTACK_ADDR, m)
#define HWIO_MSS_QDSP6SS_INTF_HALTACK_AXIM2_BMSK                                           0x4
#define HWIO_MSS_QDSP6SS_INTF_HALTACK_AXIM2_SHFT                                           0x2
#define HWIO_MSS_QDSP6SS_INTF_HALTACK_AXIM_BMSK                                            0x2
#define HWIO_MSS_QDSP6SS_INTF_HALTACK_AXIM_SHFT                                            0x1
#define HWIO_MSS_QDSP6SS_INTF_HALTACK_ALL_BMSK                                             0x1
#define HWIO_MSS_QDSP6SS_INTF_HALTACK_ALL_SHFT                                             0x0

#define HWIO_MSS_QDSP6SS_INTFCLAMP_SET_ADDR                                         (MSS_QDSP6V66SS_PUB_REG_BASE      + 0x00000090)
#define HWIO_MSS_QDSP6SS_INTFCLAMP_SET_OFFS                                         (MSS_QDSP6V66SS_PUB_REG_BASE_OFFS + 0x00000090)
#define HWIO_MSS_QDSP6SS_INTFCLAMP_SET_RMSK                                                0xf
#define HWIO_MSS_QDSP6SS_INTFCLAMP_SET_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_INTFCLAMP_SET_ADDR,v)
#define HWIO_MSS_QDSP6SS_INTFCLAMP_SET_AXIM2_CLAMP_E_BMSK                                  0x8
#define HWIO_MSS_QDSP6SS_INTFCLAMP_SET_AXIM2_CLAMP_E_SHFT                                  0x3
#define HWIO_MSS_QDSP6SS_INTFCLAMP_SET_AXIM2_CLAMP_L_BMSK                                  0x4
#define HWIO_MSS_QDSP6SS_INTFCLAMP_SET_AXIM2_CLAMP_L_SHFT                                  0x2
#define HWIO_MSS_QDSP6SS_INTFCLAMP_SET_AXIM_CLAMP_E_BMSK                                   0x2
#define HWIO_MSS_QDSP6SS_INTFCLAMP_SET_AXIM_CLAMP_E_SHFT                                   0x1
#define HWIO_MSS_QDSP6SS_INTFCLAMP_SET_AXIM_CLAMP_L_BMSK                                   0x1
#define HWIO_MSS_QDSP6SS_INTFCLAMP_SET_AXIM_CLAMP_L_SHFT                                   0x0

#define HWIO_MSS_QDSP6SS_INTFCLAMP_CLEAR_ADDR                                       (MSS_QDSP6V66SS_PUB_REG_BASE      + 0x00000094)
#define HWIO_MSS_QDSP6SS_INTFCLAMP_CLEAR_OFFS                                       (MSS_QDSP6V66SS_PUB_REG_BASE_OFFS + 0x00000094)
#define HWIO_MSS_QDSP6SS_INTFCLAMP_CLEAR_RMSK                                              0xf
#define HWIO_MSS_QDSP6SS_INTFCLAMP_CLEAR_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_INTFCLAMP_CLEAR_ADDR,v)
#define HWIO_MSS_QDSP6SS_INTFCLAMP_CLEAR_AXIM2_CLAMP_E_BMSK                                0x8
#define HWIO_MSS_QDSP6SS_INTFCLAMP_CLEAR_AXIM2_CLAMP_E_SHFT                                0x3
#define HWIO_MSS_QDSP6SS_INTFCLAMP_CLEAR_AXIM2_CLAMP_L_BMSK                                0x4
#define HWIO_MSS_QDSP6SS_INTFCLAMP_CLEAR_AXIM2_CLAMP_L_SHFT                                0x2
#define HWIO_MSS_QDSP6SS_INTFCLAMP_CLEAR_AXIM_CLAMP_E_BMSK                                 0x2
#define HWIO_MSS_QDSP6SS_INTFCLAMP_CLEAR_AXIM_CLAMP_E_SHFT                                 0x1
#define HWIO_MSS_QDSP6SS_INTFCLAMP_CLEAR_AXIM_CLAMP_L_BMSK                                 0x1
#define HWIO_MSS_QDSP6SS_INTFCLAMP_CLEAR_AXIM_CLAMP_L_SHFT                                 0x0

#define HWIO_MSS_QDSP6SS_INTFCLAMP_STATUS_ADDR                                      (MSS_QDSP6V66SS_PUB_REG_BASE      + 0x00000098)
#define HWIO_MSS_QDSP6SS_INTFCLAMP_STATUS_OFFS                                      (MSS_QDSP6V66SS_PUB_REG_BASE_OFFS + 0x00000098)
#define HWIO_MSS_QDSP6SS_INTFCLAMP_STATUS_RMSK                                             0xf
#define HWIO_MSS_QDSP6SS_INTFCLAMP_STATUS_IN          \
        in_dword_masked(HWIO_MSS_QDSP6SS_INTFCLAMP_STATUS_ADDR, HWIO_MSS_QDSP6SS_INTFCLAMP_STATUS_RMSK)
#define HWIO_MSS_QDSP6SS_INTFCLAMP_STATUS_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_INTFCLAMP_STATUS_ADDR, m)
#define HWIO_MSS_QDSP6SS_INTFCLAMP_STATUS_AXIM2_CLAMP_E_BMSK                               0x8
#define HWIO_MSS_QDSP6SS_INTFCLAMP_STATUS_AXIM2_CLAMP_E_SHFT                               0x3
#define HWIO_MSS_QDSP6SS_INTFCLAMP_STATUS_AXIM2_CLAMP_L_BMSK                               0x4
#define HWIO_MSS_QDSP6SS_INTFCLAMP_STATUS_AXIM2_CLAMP_L_SHFT                               0x2
#define HWIO_MSS_QDSP6SS_INTFCLAMP_STATUS_AXIM_CLAMP_E_BMSK                                0x2
#define HWIO_MSS_QDSP6SS_INTFCLAMP_STATUS_AXIM_CLAMP_E_SHFT                                0x1
#define HWIO_MSS_QDSP6SS_INTFCLAMP_STATUS_AXIM_CLAMP_L_BMSK                                0x1
#define HWIO_MSS_QDSP6SS_INTFCLAMP_STATUS_AXIM_CLAMP_L_SHFT                                0x0

#define HWIO_MSS_QDSP6SS_INTF_FIFO_RESET_ADDR                                       (MSS_QDSP6V66SS_PUB_REG_BASE      + 0x0000009c)
#define HWIO_MSS_QDSP6SS_INTF_FIFO_RESET_OFFS                                       (MSS_QDSP6V66SS_PUB_REG_BASE_OFFS + 0x0000009c)
#define HWIO_MSS_QDSP6SS_INTF_FIFO_RESET_RMSK                                              0x3
#define HWIO_MSS_QDSP6SS_INTF_FIFO_RESET_IN          \
        in_dword_masked(HWIO_MSS_QDSP6SS_INTF_FIFO_RESET_ADDR, HWIO_MSS_QDSP6SS_INTF_FIFO_RESET_RMSK)
#define HWIO_MSS_QDSP6SS_INTF_FIFO_RESET_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_INTF_FIFO_RESET_ADDR, m)
#define HWIO_MSS_QDSP6SS_INTF_FIFO_RESET_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_INTF_FIFO_RESET_ADDR,v)
#define HWIO_MSS_QDSP6SS_INTF_FIFO_RESET_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_INTF_FIFO_RESET_ADDR,m,v,HWIO_MSS_QDSP6SS_INTF_FIFO_RESET_IN)
#define HWIO_MSS_QDSP6SS_INTF_FIFO_RESET_AXIM2_BMSK                                        0x2
#define HWIO_MSS_QDSP6SS_INTF_FIFO_RESET_AXIM2_SHFT                                        0x1
#define HWIO_MSS_QDSP6SS_INTF_FIFO_RESET_AXIM_BMSK                                         0x1
#define HWIO_MSS_QDSP6SS_INTF_FIFO_RESET_AXIM_SHFT                                         0x0

#define HWIO_MSS_QDSP6SS_STATERET_CTL_ADDR                                          (MSS_QDSP6V66SS_PUB_REG_BASE      + 0x000000a0)
#define HWIO_MSS_QDSP6SS_STATERET_CTL_OFFS                                          (MSS_QDSP6V66SS_PUB_REG_BASE_OFFS + 0x000000a0)
#define HWIO_MSS_QDSP6SS_STATERET_CTL_RMSK                                                 0xf
#define HWIO_MSS_QDSP6SS_STATERET_CTL_IN          \
        in_dword_masked(HWIO_MSS_QDSP6SS_STATERET_CTL_ADDR, HWIO_MSS_QDSP6SS_STATERET_CTL_RMSK)
#define HWIO_MSS_QDSP6SS_STATERET_CTL_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_STATERET_CTL_ADDR, m)
#define HWIO_MSS_QDSP6SS_STATERET_CTL_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_STATERET_CTL_ADDR,v)
#define HWIO_MSS_QDSP6SS_STATERET_CTL_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_STATERET_CTL_ADDR,m,v,HWIO_MSS_QDSP6SS_STATERET_CTL_IN)
#define HWIO_MSS_QDSP6SS_STATERET_CTL_WAKEUP_IN_BMSK                                       0x8
#define HWIO_MSS_QDSP6SS_STATERET_CTL_WAKEUP_IN_SHFT                                       0x3
#define HWIO_MSS_QDSP6SS_STATERET_CTL_WAKE_IRQ_BMSK                                        0x4
#define HWIO_MSS_QDSP6SS_STATERET_CTL_WAKE_IRQ_SHFT                                        0x2
#define HWIO_MSS_QDSP6SS_STATERET_CTL_RESTORE_BMSK                                         0x2
#define HWIO_MSS_QDSP6SS_STATERET_CTL_RESTORE_SHFT                                         0x1
#define HWIO_MSS_QDSP6SS_STATERET_CTL_SAVE_BMSK                                            0x1
#define HWIO_MSS_QDSP6SS_STATERET_CTL_SAVE_SHFT                                            0x0

#define HWIO_MSS_QDSP6SS_CGC_OVERRIDE_ADDR                                          (MSS_QDSP6V66SS_PUB_REG_BASE      + 0x000000b0)
#define HWIO_MSS_QDSP6SS_CGC_OVERRIDE_OFFS                                          (MSS_QDSP6V66SS_PUB_REG_BASE_OFFS + 0x000000b0)
#define HWIO_MSS_QDSP6SS_CGC_OVERRIDE_RMSK                                              0xffff
#define HWIO_MSS_QDSP6SS_CGC_OVERRIDE_IN          \
        in_dword_masked(HWIO_MSS_QDSP6SS_CGC_OVERRIDE_ADDR, HWIO_MSS_QDSP6SS_CGC_OVERRIDE_RMSK)
#define HWIO_MSS_QDSP6SS_CGC_OVERRIDE_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_CGC_OVERRIDE_ADDR, m)
#define HWIO_MSS_QDSP6SS_CGC_OVERRIDE_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_CGC_OVERRIDE_ADDR,v)
#define HWIO_MSS_QDSP6SS_CGC_OVERRIDE_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_CGC_OVERRIDE_ADDR,m,v,HWIO_MSS_QDSP6SS_CGC_OVERRIDE_IN)
#define HWIO_MSS_QDSP6SS_CGC_OVERRIDE_CP0_CLK_EN_BMSK                                   0x8000
#define HWIO_MSS_QDSP6SS_CGC_OVERRIDE_CP0_CLK_EN_SHFT                                      0xf
#define HWIO_MSS_QDSP6SS_CGC_OVERRIDE_SPARE_BIT14_BMSK                                  0x4000
#define HWIO_MSS_QDSP6SS_CGC_OVERRIDE_SPARE_BIT14_SHFT                                     0xe
#define HWIO_MSS_QDSP6SS_CGC_OVERRIDE_AXIS_CLK_EN_BMSK                                  0x2000
#define HWIO_MSS_QDSP6SS_CGC_OVERRIDE_AXIS_CLK_EN_SHFT                                     0xd
#define HWIO_MSS_QDSP6SS_CGC_OVERRIDE_SPARE_BIT12_BMSK                                  0x1000
#define HWIO_MSS_QDSP6SS_CGC_OVERRIDE_SPARE_BIT12_SHFT                                     0xc
#define HWIO_MSS_QDSP6SS_CGC_OVERRIDE_AHBM_CLK_EN_BMSK                                   0x800
#define HWIO_MSS_QDSP6SS_CGC_OVERRIDE_AHBM_CLK_EN_SHFT                                     0xb
#define HWIO_MSS_QDSP6SS_CGC_OVERRIDE_AHBS_CLK_EN_BMSK                                   0x400
#define HWIO_MSS_QDSP6SS_CGC_OVERRIDE_AHBS_CLK_EN_SHFT                                     0xa
#define HWIO_MSS_QDSP6SS_CGC_OVERRIDE_PRIV_AHBS_CLK_EN_BMSK                              0x300
#define HWIO_MSS_QDSP6SS_CGC_OVERRIDE_PRIV_AHBS_CLK_EN_SHFT                                0x8
#define HWIO_MSS_QDSP6SS_CGC_OVERRIDE_SPARE_BIT7_BMSK                                     0x80
#define HWIO_MSS_QDSP6SS_CGC_OVERRIDE_SPARE_BIT7_SHFT                                      0x7
#define HWIO_MSS_QDSP6SS_CGC_OVERRIDE_L2VIC_AHBS_CLK_EN_BMSK                              0x60
#define HWIO_MSS_QDSP6SS_CGC_OVERRIDE_L2VIC_AHBS_CLK_EN_SHFT                               0x5
#define HWIO_MSS_QDSP6SS_CGC_OVERRIDE_TEST_AHBS_CLK_EN_BMSK                               0x10
#define HWIO_MSS_QDSP6SS_CGC_OVERRIDE_TEST_AHBS_CLK_EN_SHFT                                0x4
#define HWIO_MSS_QDSP6SS_CGC_OVERRIDE_PUB_AHBS_CLK_EN_BMSK                                 0x8
#define HWIO_MSS_QDSP6SS_CGC_OVERRIDE_PUB_AHBS_CLK_EN_SHFT                                 0x3
#define HWIO_MSS_QDSP6SS_CGC_OVERRIDE_CORE_BUS_EN_BMSK                                     0x4
#define HWIO_MSS_QDSP6SS_CGC_OVERRIDE_CORE_BUS_EN_SHFT                                     0x2
#define HWIO_MSS_QDSP6SS_CGC_OVERRIDE_CORE_RCLK_EN_BMSK                                    0x2
#define HWIO_MSS_QDSP6SS_CGC_OVERRIDE_CORE_RCLK_EN_SHFT                                    0x1
#define HWIO_MSS_QDSP6SS_CGC_OVERRIDE_CORE_CLK_EN_BMSK                                     0x1
#define HWIO_MSS_QDSP6SS_CGC_OVERRIDE_CORE_CLK_EN_SHFT                                     0x0

#define HWIO_MSS_QDSP6SS_CORE_BHS_CTL_ADDR                                          (MSS_QDSP6V66SS_PUB_REG_BASE      + 0x000000c0)
#define HWIO_MSS_QDSP6SS_CORE_BHS_CTL_OFFS                                          (MSS_QDSP6V66SS_PUB_REG_BASE_OFFS + 0x000000c0)
#define HWIO_MSS_QDSP6SS_CORE_BHS_CTL_RMSK                                                 0xf
#define HWIO_MSS_QDSP6SS_CORE_BHS_CTL_IN          \
        in_dword_masked(HWIO_MSS_QDSP6SS_CORE_BHS_CTL_ADDR, HWIO_MSS_QDSP6SS_CORE_BHS_CTL_RMSK)
#define HWIO_MSS_QDSP6SS_CORE_BHS_CTL_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_CORE_BHS_CTL_ADDR, m)
#define HWIO_MSS_QDSP6SS_CORE_BHS_CTL_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_CORE_BHS_CTL_ADDR,v)
#define HWIO_MSS_QDSP6SS_CORE_BHS_CTL_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_CORE_BHS_CTL_ADDR,m,v,HWIO_MSS_QDSP6SS_CORE_BHS_CTL_IN)
#define HWIO_MSS_QDSP6SS_CORE_BHS_CTL_DRIVE_SEL_BMSK                                       0xf
#define HWIO_MSS_QDSP6SS_CORE_BHS_CTL_DRIVE_SEL_SHFT                                       0x0

#define HWIO_MSS_QDSP6SS_CP_BHS_CTL_ADDR                                            (MSS_QDSP6V66SS_PUB_REG_BASE      + 0x000000c4)
#define HWIO_MSS_QDSP6SS_CP_BHS_CTL_OFFS                                            (MSS_QDSP6V66SS_PUB_REG_BASE_OFFS + 0x000000c4)
#define HWIO_MSS_QDSP6SS_CP_BHS_CTL_RMSK                                                   0xf
#define HWIO_MSS_QDSP6SS_CP_BHS_CTL_IN          \
        in_dword_masked(HWIO_MSS_QDSP6SS_CP_BHS_CTL_ADDR, HWIO_MSS_QDSP6SS_CP_BHS_CTL_RMSK)
#define HWIO_MSS_QDSP6SS_CP_BHS_CTL_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_CP_BHS_CTL_ADDR, m)
#define HWIO_MSS_QDSP6SS_CP_BHS_CTL_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_CP_BHS_CTL_ADDR,v)
#define HWIO_MSS_QDSP6SS_CP_BHS_CTL_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_CP_BHS_CTL_ADDR,m,v,HWIO_MSS_QDSP6SS_CP_BHS_CTL_IN)
#define HWIO_MSS_QDSP6SS_CP_BHS_CTL_DRIVE_SEL_BMSK                                         0xf
#define HWIO_MSS_QDSP6SS_CP_BHS_CTL_DRIVE_SEL_SHFT                                         0x0

#define HWIO_MSS_QDSP6SS_CPR_OVERRIDE_ADDR                                          (MSS_QDSP6V66SS_PUB_REG_BASE      + 0x000000d0)
#define HWIO_MSS_QDSP6SS_CPR_OVERRIDE_OFFS                                          (MSS_QDSP6V66SS_PUB_REG_BASE_OFFS + 0x000000d0)
#define HWIO_MSS_QDSP6SS_CPR_OVERRIDE_RMSK                                                 0x3
#define HWIO_MSS_QDSP6SS_CPR_OVERRIDE_IN          \
        in_dword_masked(HWIO_MSS_QDSP6SS_CPR_OVERRIDE_ADDR, HWIO_MSS_QDSP6SS_CPR_OVERRIDE_RMSK)
#define HWIO_MSS_QDSP6SS_CPR_OVERRIDE_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_CPR_OVERRIDE_ADDR, m)
#define HWIO_MSS_QDSP6SS_CPR_OVERRIDE_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_CPR_OVERRIDE_ADDR,v)
#define HWIO_MSS_QDSP6SS_CPR_OVERRIDE_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_CPR_OVERRIDE_ADDR,m,v,HWIO_MSS_QDSP6SS_CPR_OVERRIDE_IN)
#define HWIO_MSS_QDSP6SS_CPR_OVERRIDE_EXT_ABYP_DIS_BMSK                                    0x2
#define HWIO_MSS_QDSP6SS_CPR_OVERRIDE_EXT_ABYP_DIS_SHFT                                    0x1
#define HWIO_MSS_QDSP6SS_CPR_OVERRIDE_LOCAL_BYP_DIS_BMSK                                   0x1
#define HWIO_MSS_QDSP6SS_CPR_OVERRIDE_LOCAL_BYP_DIS_SHFT                                   0x0

#define HWIO_MSS_QDSP6SS_STRAP_TCM_ADDR                                             (MSS_QDSP6V66SS_PUB_REG_BASE      + 0x00000100)
#define HWIO_MSS_QDSP6SS_STRAP_TCM_OFFS                                             (MSS_QDSP6V66SS_PUB_REG_BASE_OFFS + 0x00000100)
#define HWIO_MSS_QDSP6SS_STRAP_TCM_RMSK                                             0xfffc0000
#define HWIO_MSS_QDSP6SS_STRAP_TCM_IN          \
        in_dword_masked(HWIO_MSS_QDSP6SS_STRAP_TCM_ADDR, HWIO_MSS_QDSP6SS_STRAP_TCM_RMSK)
#define HWIO_MSS_QDSP6SS_STRAP_TCM_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_STRAP_TCM_ADDR, m)
#define HWIO_MSS_QDSP6SS_STRAP_TCM_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_STRAP_TCM_ADDR,v)
#define HWIO_MSS_QDSP6SS_STRAP_TCM_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_STRAP_TCM_ADDR,m,v,HWIO_MSS_QDSP6SS_STRAP_TCM_IN)
#define HWIO_MSS_QDSP6SS_STRAP_TCM_BASE_ADDR_BMSK                                   0xfffc0000
#define HWIO_MSS_QDSP6SS_STRAP_TCM_BASE_ADDR_SHFT                                         0x12

#define HWIO_MSS_QDSP6SS_STRAP_AHBUPPER_ADDR                                        (MSS_QDSP6V66SS_PUB_REG_BASE      + 0x00000104)
#define HWIO_MSS_QDSP6SS_STRAP_AHBUPPER_OFFS                                        (MSS_QDSP6V66SS_PUB_REG_BASE_OFFS + 0x00000104)
#define HWIO_MSS_QDSP6SS_STRAP_AHBUPPER_RMSK                                        0xffffc000
#define HWIO_MSS_QDSP6SS_STRAP_AHBUPPER_IN          \
        in_dword_masked(HWIO_MSS_QDSP6SS_STRAP_AHBUPPER_ADDR, HWIO_MSS_QDSP6SS_STRAP_AHBUPPER_RMSK)
#define HWIO_MSS_QDSP6SS_STRAP_AHBUPPER_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_STRAP_AHBUPPER_ADDR, m)
#define HWIO_MSS_QDSP6SS_STRAP_AHBUPPER_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_STRAP_AHBUPPER_ADDR,v)
#define HWIO_MSS_QDSP6SS_STRAP_AHBUPPER_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_STRAP_AHBUPPER_ADDR,m,v,HWIO_MSS_QDSP6SS_STRAP_AHBUPPER_IN)
#define HWIO_MSS_QDSP6SS_STRAP_AHBUPPER_UPPER_ADDR_BMSK                             0xffffc000
#define HWIO_MSS_QDSP6SS_STRAP_AHBUPPER_UPPER_ADDR_SHFT                                    0xe

#define HWIO_MSS_QDSP6SS_STRAP_AHBLOWER_ADDR                                        (MSS_QDSP6V66SS_PUB_REG_BASE      + 0x00000108)
#define HWIO_MSS_QDSP6SS_STRAP_AHBLOWER_OFFS                                        (MSS_QDSP6V66SS_PUB_REG_BASE_OFFS + 0x00000108)
#define HWIO_MSS_QDSP6SS_STRAP_AHBLOWER_RMSK                                        0xffffc000
#define HWIO_MSS_QDSP6SS_STRAP_AHBLOWER_IN          \
        in_dword_masked(HWIO_MSS_QDSP6SS_STRAP_AHBLOWER_ADDR, HWIO_MSS_QDSP6SS_STRAP_AHBLOWER_RMSK)
#define HWIO_MSS_QDSP6SS_STRAP_AHBLOWER_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_STRAP_AHBLOWER_ADDR, m)
#define HWIO_MSS_QDSP6SS_STRAP_AHBLOWER_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_STRAP_AHBLOWER_ADDR,v)
#define HWIO_MSS_QDSP6SS_STRAP_AHBLOWER_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_STRAP_AHBLOWER_ADDR,m,v,HWIO_MSS_QDSP6SS_STRAP_AHBLOWER_IN)
#define HWIO_MSS_QDSP6SS_STRAP_AHBLOWER_LOWER_ADDR_BMSK                             0xffffc000
#define HWIO_MSS_QDSP6SS_STRAP_AHBLOWER_LOWER_ADDR_SHFT                                    0xe

#define HWIO_MSS_QDSP6SS_DCC_CTRL_ADDR                                              (MSS_QDSP6V66SS_PUB_REG_BASE      + 0x00000118)
#define HWIO_MSS_QDSP6SS_DCC_CTRL_OFFS                                              (MSS_QDSP6V66SS_PUB_REG_BASE_OFFS + 0x00000118)
#define HWIO_MSS_QDSP6SS_DCC_CTRL_RMSK                                              0xffffffff
#define HWIO_MSS_QDSP6SS_DCC_CTRL_IN          \
        in_dword_masked(HWIO_MSS_QDSP6SS_DCC_CTRL_ADDR, HWIO_MSS_QDSP6SS_DCC_CTRL_RMSK)
#define HWIO_MSS_QDSP6SS_DCC_CTRL_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_DCC_CTRL_ADDR, m)
#define HWIO_MSS_QDSP6SS_DCC_CTRL_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_DCC_CTRL_ADDR,v)
#define HWIO_MSS_QDSP6SS_DCC_CTRL_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_DCC_CTRL_ADDR,m,v,HWIO_MSS_QDSP6SS_DCC_CTRL_IN)
#define HWIO_MSS_QDSP6SS_DCC_CTRL_CTRL_BMSK                                         0xffffffff
#define HWIO_MSS_QDSP6SS_DCC_CTRL_CTRL_SHFT                                                0x0

#define HWIO_MSS_QDSP6SS_STRAP_AXIM2UPPER_ADDR                                      (MSS_QDSP6V66SS_PUB_REG_BASE      + 0x0000011c)
#define HWIO_MSS_QDSP6SS_STRAP_AXIM2UPPER_OFFS                                      (MSS_QDSP6V66SS_PUB_REG_BASE_OFFS + 0x0000011c)
#define HWIO_MSS_QDSP6SS_STRAP_AXIM2UPPER_RMSK                                      0xffffc000
#define HWIO_MSS_QDSP6SS_STRAP_AXIM2UPPER_IN          \
        in_dword_masked(HWIO_MSS_QDSP6SS_STRAP_AXIM2UPPER_ADDR, HWIO_MSS_QDSP6SS_STRAP_AXIM2UPPER_RMSK)
#define HWIO_MSS_QDSP6SS_STRAP_AXIM2UPPER_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_STRAP_AXIM2UPPER_ADDR, m)
#define HWIO_MSS_QDSP6SS_STRAP_AXIM2UPPER_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_STRAP_AXIM2UPPER_ADDR,v)
#define HWIO_MSS_QDSP6SS_STRAP_AXIM2UPPER_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_STRAP_AXIM2UPPER_ADDR,m,v,HWIO_MSS_QDSP6SS_STRAP_AXIM2UPPER_IN)
#define HWIO_MSS_QDSP6SS_STRAP_AXIM2UPPER_UPPER_ADDR_BMSK                           0xffffc000
#define HWIO_MSS_QDSP6SS_STRAP_AXIM2UPPER_UPPER_ADDR_SHFT                                  0xe

#define HWIO_MSS_QDSP6SS_STRAP_AXIM2LOWER_ADDR                                      (MSS_QDSP6V66SS_PUB_REG_BASE      + 0x00000120)
#define HWIO_MSS_QDSP6SS_STRAP_AXIM2LOWER_OFFS                                      (MSS_QDSP6V66SS_PUB_REG_BASE_OFFS + 0x00000120)
#define HWIO_MSS_QDSP6SS_STRAP_AXIM2LOWER_RMSK                                      0xffffc000
#define HWIO_MSS_QDSP6SS_STRAP_AXIM2LOWER_IN          \
        in_dword_masked(HWIO_MSS_QDSP6SS_STRAP_AXIM2LOWER_ADDR, HWIO_MSS_QDSP6SS_STRAP_AXIM2LOWER_RMSK)
#define HWIO_MSS_QDSP6SS_STRAP_AXIM2LOWER_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_STRAP_AXIM2LOWER_ADDR, m)
#define HWIO_MSS_QDSP6SS_STRAP_AXIM2LOWER_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_STRAP_AXIM2LOWER_ADDR,v)
#define HWIO_MSS_QDSP6SS_STRAP_AXIM2LOWER_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_STRAP_AXIM2LOWER_ADDR,m,v,HWIO_MSS_QDSP6SS_STRAP_AXIM2LOWER_IN)
#define HWIO_MSS_QDSP6SS_STRAP_AXIM2LOWER_LOWER_ADDR_BMSK                           0xffffc000
#define HWIO_MSS_QDSP6SS_STRAP_AXIM2LOWER_LOWER_ADDR_SHFT                                  0xe

#define HWIO_MSS_QDSP6SS_STRAP_QMC_ACC_SEL_ADDR                                     (MSS_QDSP6V66SS_PUB_REG_BASE      + 0x00000124)
#define HWIO_MSS_QDSP6SS_STRAP_QMC_ACC_SEL_OFFS                                     (MSS_QDSP6V66SS_PUB_REG_BASE_OFFS + 0x00000124)
#define HWIO_MSS_QDSP6SS_STRAP_QMC_ACC_SEL_RMSK                                            0x1
#define HWIO_MSS_QDSP6SS_STRAP_QMC_ACC_SEL_IN          \
        in_dword_masked(HWIO_MSS_QDSP6SS_STRAP_QMC_ACC_SEL_ADDR, HWIO_MSS_QDSP6SS_STRAP_QMC_ACC_SEL_RMSK)
#define HWIO_MSS_QDSP6SS_STRAP_QMC_ACC_SEL_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_STRAP_QMC_ACC_SEL_ADDR, m)
#define HWIO_MSS_QDSP6SS_STRAP_QMC_ACC_SEL_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_STRAP_QMC_ACC_SEL_ADDR,v)
#define HWIO_MSS_QDSP6SS_STRAP_QMC_ACC_SEL_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_STRAP_QMC_ACC_SEL_ADDR,m,v,HWIO_MSS_QDSP6SS_STRAP_QMC_ACC_SEL_IN)
#define HWIO_MSS_QDSP6SS_STRAP_QMC_ACC_SEL_STRAP_QMC_ACC_SEL_BMSK                          0x1
#define HWIO_MSS_QDSP6SS_STRAP_QMC_ACC_SEL_STRAP_QMC_ACC_SEL_SHFT                          0x0

#define HWIO_MSS_QDSP6SS_STRAP_QMC_ACC_ADDR                                         (MSS_QDSP6V66SS_PUB_REG_BASE      + 0x00000128)
#define HWIO_MSS_QDSP6SS_STRAP_QMC_ACC_OFFS                                         (MSS_QDSP6V66SS_PUB_REG_BASE_OFFS + 0x00000128)
#define HWIO_MSS_QDSP6SS_STRAP_QMC_ACC_RMSK                                         0xffffffff
#define HWIO_MSS_QDSP6SS_STRAP_QMC_ACC_IN          \
        in_dword_masked(HWIO_MSS_QDSP6SS_STRAP_QMC_ACC_ADDR, HWIO_MSS_QDSP6SS_STRAP_QMC_ACC_RMSK)
#define HWIO_MSS_QDSP6SS_STRAP_QMC_ACC_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_STRAP_QMC_ACC_ADDR, m)
#define HWIO_MSS_QDSP6SS_STRAP_QMC_ACC_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_STRAP_QMC_ACC_ADDR,v)
#define HWIO_MSS_QDSP6SS_STRAP_QMC_ACC_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_STRAP_QMC_ACC_ADDR,m,v,HWIO_MSS_QDSP6SS_STRAP_QMC_ACC_IN)
#define HWIO_MSS_QDSP6SS_STRAP_QMC_ACC_STRAP_QMC_ACC_BMSK                           0xffffffff
#define HWIO_MSS_QDSP6SS_STRAP_QMC_ACC_STRAP_QMC_ACC_SHFT                                  0x0

#define HWIO_MSS_QDSP6SS_STRAP_QMC_ACC_U_ADDR                                       (MSS_QDSP6V66SS_PUB_REG_BASE      + 0x0000012c)
#define HWIO_MSS_QDSP6SS_STRAP_QMC_ACC_U_OFFS                                       (MSS_QDSP6V66SS_PUB_REG_BASE_OFFS + 0x0000012c)
#define HWIO_MSS_QDSP6SS_STRAP_QMC_ACC_U_RMSK                                       0xffffffff
#define HWIO_MSS_QDSP6SS_STRAP_QMC_ACC_U_IN          \
        in_dword_masked(HWIO_MSS_QDSP6SS_STRAP_QMC_ACC_U_ADDR, HWIO_MSS_QDSP6SS_STRAP_QMC_ACC_U_RMSK)
#define HWIO_MSS_QDSP6SS_STRAP_QMC_ACC_U_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_STRAP_QMC_ACC_U_ADDR, m)
#define HWIO_MSS_QDSP6SS_STRAP_QMC_ACC_U_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_STRAP_QMC_ACC_U_ADDR,v)
#define HWIO_MSS_QDSP6SS_STRAP_QMC_ACC_U_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_STRAP_QMC_ACC_U_ADDR,m,v,HWIO_MSS_QDSP6SS_STRAP_QMC_ACC_U_IN)
#define HWIO_MSS_QDSP6SS_STRAP_QMC_ACC_U_STRAP_QMC_ACC_U_BMSK                       0xffffffff
#define HWIO_MSS_QDSP6SS_STRAP_QMC_ACC_U_STRAP_QMC_ACC_U_SHFT                              0x0

#define HWIO_MSS_QDSP6SS_QMC_SVS_CTL_ADDR                                           (MSS_QDSP6V66SS_PUB_REG_BASE      + 0x00000130)
#define HWIO_MSS_QDSP6SS_QMC_SVS_CTL_OFFS                                           (MSS_QDSP6V66SS_PUB_REG_BASE_OFFS + 0x00000130)
#define HWIO_MSS_QDSP6SS_QMC_SVS_CTL_RMSK                                                  0x3
#define HWIO_MSS_QDSP6SS_QMC_SVS_CTL_IN          \
        in_dword_masked(HWIO_MSS_QDSP6SS_QMC_SVS_CTL_ADDR, HWIO_MSS_QDSP6SS_QMC_SVS_CTL_RMSK)
#define HWIO_MSS_QDSP6SS_QMC_SVS_CTL_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_QMC_SVS_CTL_ADDR, m)
#define HWIO_MSS_QDSP6SS_QMC_SVS_CTL_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_QMC_SVS_CTL_ADDR,v)
#define HWIO_MSS_QDSP6SS_QMC_SVS_CTL_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_QMC_SVS_CTL_ADDR,m,v,HWIO_MSS_QDSP6SS_QMC_SVS_CTL_IN)
#define HWIO_MSS_QDSP6SS_QMC_SVS_CTL_QMC_MEM_SVS_SEL_BMSK                                  0x2
#define HWIO_MSS_QDSP6SS_QMC_SVS_CTL_QMC_MEM_SVS_SEL_SHFT                                  0x1
#define HWIO_MSS_QDSP6SS_QMC_SVS_CTL_QMC_MEM_SVS_BMSK                                      0x1
#define HWIO_MSS_QDSP6SS_QMC_SVS_CTL_QMC_MEM_SVS_SHFT                                      0x0

#define HWIO_MSS_QDSP6SS_CORE_MEM_STAGGER_CTL_ADDR                                  (MSS_QDSP6V66SS_PUB_REG_BASE      + 0x00000140)
#define HWIO_MSS_QDSP6SS_CORE_MEM_STAGGER_CTL_OFFS                                  (MSS_QDSP6V66SS_PUB_REG_BASE_OFFS + 0x00000140)
#define HWIO_MSS_QDSP6SS_CORE_MEM_STAGGER_CTL_RMSK                                         0xf
#define HWIO_MSS_QDSP6SS_CORE_MEM_STAGGER_CTL_IN          \
        in_dword_masked(HWIO_MSS_QDSP6SS_CORE_MEM_STAGGER_CTL_ADDR, HWIO_MSS_QDSP6SS_CORE_MEM_STAGGER_CTL_RMSK)
#define HWIO_MSS_QDSP6SS_CORE_MEM_STAGGER_CTL_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_CORE_MEM_STAGGER_CTL_ADDR, m)
#define HWIO_MSS_QDSP6SS_CORE_MEM_STAGGER_CTL_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_CORE_MEM_STAGGER_CTL_ADDR,v)
#define HWIO_MSS_QDSP6SS_CORE_MEM_STAGGER_CTL_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_CORE_MEM_STAGGER_CTL_ADDR,m,v,HWIO_MSS_QDSP6SS_CORE_MEM_STAGGER_CTL_IN)
#define HWIO_MSS_QDSP6SS_CORE_MEM_STAGGER_CTL_DIV_BMSK                                     0xf
#define HWIO_MSS_QDSP6SS_CORE_MEM_STAGGER_CTL_DIV_SHFT                                     0x0

#define HWIO_MSS_QDSP6SS_PLL_MODE_ADDR                                              (MSS_QDSP6V66SS_PUB_REG_BASE      + 0x00000200)
#define HWIO_MSS_QDSP6SS_PLL_MODE_OFFS                                              (MSS_QDSP6V66SS_PUB_REG_BASE_OFFS + 0x00000200)
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
#define HWIO_MSS_QDSP6SS_PLL_MODE_FSM_LEGACY_MODE_BMSK                               0x1000000
#define HWIO_MSS_QDSP6SS_PLL_MODE_FSM_LEGACY_MODE_SHFT                                    0x18
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
#define HWIO_MSS_QDSP6SS_PLL_MODE_OUT_TEST_HW_CTL_BMSK                                    0x40
#define HWIO_MSS_QDSP6SS_PLL_MODE_OUT_TEST_HW_CTL_SHFT                                     0x6
#define HWIO_MSS_QDSP6SS_PLL_MODE_OUT_ODD_HW_CTL_BMSK                                     0x20
#define HWIO_MSS_QDSP6SS_PLL_MODE_OUT_ODD_HW_CTL_SHFT                                      0x5
#define HWIO_MSS_QDSP6SS_PLL_MODE_OUT_EVEN_HW_CTL_BMSK                                    0x10
#define HWIO_MSS_QDSP6SS_PLL_MODE_OUT_EVEN_HW_CTL_SHFT                                     0x4
#define HWIO_MSS_QDSP6SS_PLL_MODE_OUT_MAIN_HW_CTL_BMSK                                     0x8
#define HWIO_MSS_QDSP6SS_PLL_MODE_OUT_MAIN_HW_CTL_SHFT                                     0x3
#define HWIO_MSS_QDSP6SS_PLL_MODE_PLL_RESET_N_BMSK                                         0x4
#define HWIO_MSS_QDSP6SS_PLL_MODE_PLL_RESET_N_SHFT                                         0x2
#define HWIO_MSS_QDSP6SS_PLL_MODE_PLL_OUTCTRL_BMSK                                         0x1
#define HWIO_MSS_QDSP6SS_PLL_MODE_PLL_OUTCTRL_SHFT                                         0x0

#define HWIO_MSS_QDSP6SS_PLL_L_VAL_ADDR                                             (MSS_QDSP6V66SS_PUB_REG_BASE      + 0x00000204)
#define HWIO_MSS_QDSP6SS_PLL_L_VAL_OFFS                                             (MSS_QDSP6V66SS_PUB_REG_BASE_OFFS + 0x00000204)
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

#define HWIO_MSS_QDSP6SS_PLL_CAL_L_VAL_ADDR                                         (MSS_QDSP6V66SS_PUB_REG_BASE      + 0x00000208)
#define HWIO_MSS_QDSP6SS_PLL_CAL_L_VAL_OFFS                                         (MSS_QDSP6V66SS_PUB_REG_BASE_OFFS + 0x00000208)
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

#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_ADDR                                          (MSS_QDSP6V66SS_PUB_REG_BASE      + 0x0000020c)
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_OFFS                                          (MSS_QDSP6V66SS_PUB_REG_BASE_OFFS + 0x0000020c)
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_RMSK                                          0xffffffff
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_IN          \
        in_dword_masked(HWIO_MSS_QDSP6SS_PLL_USER_CTL_ADDR, HWIO_MSS_QDSP6SS_PLL_USER_CTL_RMSK)
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_PLL_USER_CTL_ADDR, m)
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_PLL_USER_CTL_ADDR,v)
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_PLL_USER_CTL_ADDR,m,v,HWIO_MSS_QDSP6SS_PLL_USER_CTL_IN)
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_PRE_DIV_RATIO_BMSK                               0x70000
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_PRE_DIV_RATIO_SHFT                                  0x10
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_POST_DIV_RATIO_ODD_BMSK                           0xf000
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_POST_DIV_RATIO_ODD_SHFT                              0xc
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_POST_DIV_RATIO_EVEN_BMSK                           0xf00
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_POST_DIV_RATIO_EVEN_SHFT                             0x8
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_OUT_CLK_POLARITY_BMSK                               0x80
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_OUT_CLK_POLARITY_SHFT                                0x7
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_PLLOUT_TEST_BMSK                                    0x10
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_PLLOUT_TEST_SHFT                                     0x4
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_PLLOUT_ODD_BMSK                                      0x4
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_PLLOUT_ODD_SHFT                                      0x2
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_PLLOUT_EVEN_BMSK                                     0x2
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_PLLOUT_EVEN_SHFT                                     0x1
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_PLLOUT_MAIN_BMSK                                     0x1
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_PLLOUT_MAIN_SHFT                                     0x0

#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_U_ADDR                                        (MSS_QDSP6V66SS_PUB_REG_BASE      + 0x00000210)
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_U_OFFS                                        (MSS_QDSP6V66SS_PUB_REG_BASE_OFFS + 0x00000210)
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_U_RMSK                                        0xffffffff
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_U_IN          \
        in_dword_masked(HWIO_MSS_QDSP6SS_PLL_USER_CTL_U_ADDR, HWIO_MSS_QDSP6SS_PLL_USER_CTL_U_RMSK)
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_U_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_PLL_USER_CTL_U_ADDR, m)
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_U_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_PLL_USER_CTL_U_ADDR,v)
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_U_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_PLL_USER_CTL_U_ADDR,m,v,HWIO_MSS_QDSP6SS_PLL_USER_CTL_U_IN)
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_U_USE_PREDIV_WHEN_CAL_BMSK                       0x40000
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_U_USE_PREDIV_WHEN_CAL_SHFT                          0x12
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_U_USE_EXT_SAVERESTORE_BMSK                       0x20000
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_U_USE_EXT_SAVERESTORE_SHFT                          0x11
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

#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_U1_ADDR                                       (MSS_QDSP6V66SS_PUB_REG_BASE      + 0x00000214)
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_U1_OFFS                                       (MSS_QDSP6V66SS_PUB_REG_BASE_OFFS + 0x00000214)
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_U1_RMSK                                       0xffffffff
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_U1_IN          \
        in_dword_masked(HWIO_MSS_QDSP6SS_PLL_USER_CTL_U1_ADDR, HWIO_MSS_QDSP6SS_PLL_USER_CTL_U1_RMSK)
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_U1_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_PLL_USER_CTL_U1_ADDR, m)
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_U1_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_PLL_USER_CTL_U1_ADDR,v)
#define HWIO_MSS_QDSP6SS_PLL_USER_CTL_U1_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_PLL_USER_CTL_U1_ADDR,m,v,HWIO_MSS_QDSP6SS_PLL_USER_CTL_U1_IN)

#define HWIO_MSS_QDSP6SS_PLL_CONFIG_CTL_ADDR                                        (MSS_QDSP6V66SS_PUB_REG_BASE      + 0x00000218)
#define HWIO_MSS_QDSP6SS_PLL_CONFIG_CTL_OFFS                                        (MSS_QDSP6V66SS_PUB_REG_BASE_OFFS + 0x00000218)
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

#define HWIO_MSS_QDSP6SS_PLL_CONFIG_CTL_U_ADDR                                      (MSS_QDSP6V66SS_PUB_REG_BASE      + 0x0000021c)
#define HWIO_MSS_QDSP6SS_PLL_CONFIG_CTL_U_OFFS                                      (MSS_QDSP6V66SS_PUB_REG_BASE_OFFS + 0x0000021c)
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
#define HWIO_MSS_QDSP6SS_PLL_CONFIG_CTL_U_FORCE_DAC_ON_BMSK                            0x30000
#define HWIO_MSS_QDSP6SS_PLL_CONFIG_CTL_U_FORCE_DAC_ON_SHFT                               0x10
#define HWIO_MSS_QDSP6SS_PLL_CONFIG_CTL_U_OSC_SETTLING_TIME_BMSK                        0x3000
#define HWIO_MSS_QDSP6SS_PLL_CONFIG_CTL_U_OSC_SETTLING_TIME_SHFT                           0xc
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

#define HWIO_MSS_QDSP6SS_PLL_CONFIG_CTL_U1_ADDR                                     (MSS_QDSP6V66SS_PUB_REG_BASE      + 0x00000220)
#define HWIO_MSS_QDSP6SS_PLL_CONFIG_CTL_U1_OFFS                                     (MSS_QDSP6V66SS_PUB_REG_BASE_OFFS + 0x00000220)
#define HWIO_MSS_QDSP6SS_PLL_CONFIG_CTL_U1_RMSK                                     0xffffffff
#define HWIO_MSS_QDSP6SS_PLL_CONFIG_CTL_U1_IN          \
        in_dword_masked(HWIO_MSS_QDSP6SS_PLL_CONFIG_CTL_U1_ADDR, HWIO_MSS_QDSP6SS_PLL_CONFIG_CTL_U1_RMSK)
#define HWIO_MSS_QDSP6SS_PLL_CONFIG_CTL_U1_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_PLL_CONFIG_CTL_U1_ADDR, m)
#define HWIO_MSS_QDSP6SS_PLL_CONFIG_CTL_U1_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_PLL_CONFIG_CTL_U1_ADDR,v)
#define HWIO_MSS_QDSP6SS_PLL_CONFIG_CTL_U1_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_PLL_CONFIG_CTL_U1_ADDR,m,v,HWIO_MSS_QDSP6SS_PLL_CONFIG_CTL_U1_IN)

#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_ADDR                                          (MSS_QDSP6V66SS_PUB_REG_BASE      + 0x00000224)
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_OFFS                                          (MSS_QDSP6V66SS_PUB_REG_BASE_OFFS + 0x00000224)
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
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_EN_VREF_TEST_BMSK                                   0x20
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_EN_VREF_TEST_SHFT                                    0x5
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_EN_VDCO_TEST_BMSK                                   0x10
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_EN_VDCO_TEST_SHFT                                    0x4
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_BYPASS_ATEST_BUFFER_BMSK                             0x8
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_BYPASS_ATEST_BUFFER_SHFT                             0x3
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_EN_IVCO_TEST_BMSK                                    0x4
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_EN_IVCO_TEST_SHFT                                    0x2
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_ATEST1_EN_BMSK                                       0x2
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_ATEST1_EN_SHFT                                       0x1
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_ATEST0_EN_BMSK                                       0x1
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_ATEST0_EN_SHFT                                       0x0

#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_U_ADDR                                        (MSS_QDSP6V66SS_PUB_REG_BASE      + 0x00000228)
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_U_OFFS                                        (MSS_QDSP6V66SS_PUB_REG_BASE_OFFS + 0x00000228)
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
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_U_DIS_AMP_STARTUP_BMSK                            0x4000
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_U_DIS_AMP_STARTUP_SHFT                               0xe
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

#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_U1_ADDR                                       (MSS_QDSP6V66SS_PUB_REG_BASE      + 0x0000022c)
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_U1_OFFS                                       (MSS_QDSP6V66SS_PUB_REG_BASE_OFFS + 0x0000022c)
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_U1_RMSK                                       0xffffffff
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_U1_IN          \
        in_dword_masked(HWIO_MSS_QDSP6SS_PLL_TEST_CTL_U1_ADDR, HWIO_MSS_QDSP6SS_PLL_TEST_CTL_U1_RMSK)
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_U1_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_PLL_TEST_CTL_U1_ADDR, m)
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_U1_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_PLL_TEST_CTL_U1_ADDR,v)
#define HWIO_MSS_QDSP6SS_PLL_TEST_CTL_U1_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_PLL_TEST_CTL_U1_ADDR,m,v,HWIO_MSS_QDSP6SS_PLL_TEST_CTL_U1_IN)

#define HWIO_MSS_QDSP6SS_PLL_STATUS_ADDR                                            (MSS_QDSP6V66SS_PUB_REG_BASE      + 0x00000230)
#define HWIO_MSS_QDSP6SS_PLL_STATUS_OFFS                                            (MSS_QDSP6V66SS_PUB_REG_BASE_OFFS + 0x00000230)
#define HWIO_MSS_QDSP6SS_PLL_STATUS_RMSK                                            0xffffffff
#define HWIO_MSS_QDSP6SS_PLL_STATUS_IN          \
        in_dword_masked(HWIO_MSS_QDSP6SS_PLL_STATUS_ADDR, HWIO_MSS_QDSP6SS_PLL_STATUS_RMSK)
#define HWIO_MSS_QDSP6SS_PLL_STATUS_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_PLL_STATUS_ADDR, m)
#define HWIO_MSS_QDSP6SS_PLL_STATUS_STATUS_31_0_BMSK                                0xffffffff
#define HWIO_MSS_QDSP6SS_PLL_STATUS_STATUS_31_0_SHFT                                       0x0

#define HWIO_MSS_QDSP6SS_PLL_FREQ_CTL_ADDR                                          (MSS_QDSP6V66SS_PUB_REG_BASE      + 0x00000234)
#define HWIO_MSS_QDSP6SS_PLL_FREQ_CTL_OFFS                                          (MSS_QDSP6V66SS_PUB_REG_BASE_OFFS + 0x00000234)
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

#define HWIO_MSS_QDSP6SS_PLL_OPMODE_ADDR                                            (MSS_QDSP6V66SS_PUB_REG_BASE      + 0x00000238)
#define HWIO_MSS_QDSP6SS_PLL_OPMODE_OFFS                                            (MSS_QDSP6V66SS_PUB_REG_BASE_OFFS + 0x00000238)
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

#define HWIO_MSS_QDSP6SS_PLL_STATE_ADDR                                             (MSS_QDSP6V66SS_PUB_REG_BASE      + 0x0000023c)
#define HWIO_MSS_QDSP6SS_PLL_STATE_OFFS                                             (MSS_QDSP6V66SS_PUB_REG_BASE_OFFS + 0x0000023c)
#define HWIO_MSS_QDSP6SS_PLL_STATE_RMSK                                                    0x7
#define HWIO_MSS_QDSP6SS_PLL_STATE_IN          \
        in_dword_masked(HWIO_MSS_QDSP6SS_PLL_STATE_ADDR, HWIO_MSS_QDSP6SS_PLL_STATE_RMSK)
#define HWIO_MSS_QDSP6SS_PLL_STATE_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_PLL_STATE_ADDR, m)
#define HWIO_MSS_QDSP6SS_PLL_STATE_PLL_STATE_BMSK                                          0x7
#define HWIO_MSS_QDSP6SS_PLL_STATE_PLL_STATE_SHFT                                          0x0

#define HWIO_MSS_QDSP6SS_PLL_ALPHA_VAL_ADDR                                         (MSS_QDSP6V66SS_PUB_REG_BASE      + 0x00000240)
#define HWIO_MSS_QDSP6SS_PLL_ALPHA_VAL_OFFS                                         (MSS_QDSP6V66SS_PUB_REG_BASE_OFFS + 0x00000240)
#define HWIO_MSS_QDSP6SS_PLL_ALPHA_VAL_RMSK                                             0xffff
#define HWIO_MSS_QDSP6SS_PLL_ALPHA_VAL_IN          \
        in_dword_masked(HWIO_MSS_QDSP6SS_PLL_ALPHA_VAL_ADDR, HWIO_MSS_QDSP6SS_PLL_ALPHA_VAL_RMSK)
#define HWIO_MSS_QDSP6SS_PLL_ALPHA_VAL_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_PLL_ALPHA_VAL_ADDR, m)
#define HWIO_MSS_QDSP6SS_PLL_ALPHA_VAL_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_PLL_ALPHA_VAL_ADDR,v)
#define HWIO_MSS_QDSP6SS_PLL_ALPHA_VAL_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_PLL_ALPHA_VAL_ADDR,m,v,HWIO_MSS_QDSP6SS_PLL_ALPHA_VAL_IN)
#define HWIO_MSS_QDSP6SS_PLL_ALPHA_VAL_PLL_ALPHA_VAL_BMSK                               0xffff
#define HWIO_MSS_QDSP6SS_PLL_ALPHA_VAL_PLL_ALPHA_VAL_SHFT                                  0x0

#define HWIO_MSS_QDSP6SS_PLL_SPARE_ADDR                                             (MSS_QDSP6V66SS_PUB_REG_BASE      + 0x00000244)
#define HWIO_MSS_QDSP6SS_PLL_SPARE_OFFS                                             (MSS_QDSP6V66SS_PUB_REG_BASE_OFFS + 0x00000244)
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

#define HWIO_MSS_QDSP6SS_PLL_SSC_DELTA_ALPHA_ADDR                                   (MSS_QDSP6V66SS_PUB_REG_BASE      + 0x00000248)
#define HWIO_MSS_QDSP6SS_PLL_SSC_DELTA_ALPHA_OFFS                                   (MSS_QDSP6V66SS_PUB_REG_BASE_OFFS + 0x00000248)
#define HWIO_MSS_QDSP6SS_PLL_SSC_DELTA_ALPHA_RMSK                                         0xff
#define HWIO_MSS_QDSP6SS_PLL_SSC_DELTA_ALPHA_IN          \
        in_dword_masked(HWIO_MSS_QDSP6SS_PLL_SSC_DELTA_ALPHA_ADDR, HWIO_MSS_QDSP6SS_PLL_SSC_DELTA_ALPHA_RMSK)
#define HWIO_MSS_QDSP6SS_PLL_SSC_DELTA_ALPHA_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_PLL_SSC_DELTA_ALPHA_ADDR, m)
#define HWIO_MSS_QDSP6SS_PLL_SSC_DELTA_ALPHA_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_PLL_SSC_DELTA_ALPHA_ADDR,v)
#define HWIO_MSS_QDSP6SS_PLL_SSC_DELTA_ALPHA_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_PLL_SSC_DELTA_ALPHA_ADDR,m,v,HWIO_MSS_QDSP6SS_PLL_SSC_DELTA_ALPHA_IN)
#define HWIO_MSS_QDSP6SS_PLL_SSC_DELTA_ALPHA_SSC_DELTA_ALPHA_BMSK                         0xff
#define HWIO_MSS_QDSP6SS_PLL_SSC_DELTA_ALPHA_SSC_DELTA_ALPHA_SHFT                          0x0

#define HWIO_MSS_QDSP6SS_PLL_SSC_NUM_STEPS_ADDR                                     (MSS_QDSP6V66SS_PUB_REG_BASE      + 0x0000024c)
#define HWIO_MSS_QDSP6SS_PLL_SSC_NUM_STEPS_OFFS                                     (MSS_QDSP6V66SS_PUB_REG_BASE_OFFS + 0x0000024c)
#define HWIO_MSS_QDSP6SS_PLL_SSC_NUM_STEPS_RMSK                                            0xf
#define HWIO_MSS_QDSP6SS_PLL_SSC_NUM_STEPS_IN          \
        in_dword_masked(HWIO_MSS_QDSP6SS_PLL_SSC_NUM_STEPS_ADDR, HWIO_MSS_QDSP6SS_PLL_SSC_NUM_STEPS_RMSK)
#define HWIO_MSS_QDSP6SS_PLL_SSC_NUM_STEPS_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_PLL_SSC_NUM_STEPS_ADDR, m)
#define HWIO_MSS_QDSP6SS_PLL_SSC_NUM_STEPS_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_PLL_SSC_NUM_STEPS_ADDR,v)
#define HWIO_MSS_QDSP6SS_PLL_SSC_NUM_STEPS_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_PLL_SSC_NUM_STEPS_ADDR,m,v,HWIO_MSS_QDSP6SS_PLL_SSC_NUM_STEPS_IN)
#define HWIO_MSS_QDSP6SS_PLL_SSC_NUM_STEPS_SSC_NUM_STEPS_BMSK                              0xf
#define HWIO_MSS_QDSP6SS_PLL_SSC_NUM_STEPS_SSC_NUM_STEPS_SHFT                              0x0

#define HWIO_MSS_QDSP6SS_PLL_SSC_UPDATE_RATE_ADDR                                   (MSS_QDSP6V66SS_PUB_REG_BASE      + 0x00000250)
#define HWIO_MSS_QDSP6SS_PLL_SSC_UPDATE_RATE_OFFS                                   (MSS_QDSP6V66SS_PUB_REG_BASE_OFFS + 0x00000250)
#define HWIO_MSS_QDSP6SS_PLL_SSC_UPDATE_RATE_RMSK                                          0xf
#define HWIO_MSS_QDSP6SS_PLL_SSC_UPDATE_RATE_IN          \
        in_dword_masked(HWIO_MSS_QDSP6SS_PLL_SSC_UPDATE_RATE_ADDR, HWIO_MSS_QDSP6SS_PLL_SSC_UPDATE_RATE_RMSK)
#define HWIO_MSS_QDSP6SS_PLL_SSC_UPDATE_RATE_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_PLL_SSC_UPDATE_RATE_ADDR, m)
#define HWIO_MSS_QDSP6SS_PLL_SSC_UPDATE_RATE_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_PLL_SSC_UPDATE_RATE_ADDR,v)
#define HWIO_MSS_QDSP6SS_PLL_SSC_UPDATE_RATE_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_PLL_SSC_UPDATE_RATE_ADDR,m,v,HWIO_MSS_QDSP6SS_PLL_SSC_UPDATE_RATE_IN)
#define HWIO_MSS_QDSP6SS_PLL_SSC_UPDATE_RATE_SSC_UPDATE_RATE_BMSK                          0xf
#define HWIO_MSS_QDSP6SS_PLL_SSC_UPDATE_RATE_SSC_UPDATE_RATE_SHFT                          0x0

#define HWIO_MSS_QDSP6SS_PLL_RCG_UPDATE_STATUS_ADDR                                 (MSS_QDSP6V66SS_PUB_REG_BASE      + 0x00000260)
#define HWIO_MSS_QDSP6SS_PLL_RCG_UPDATE_STATUS_OFFS                                 (MSS_QDSP6V66SS_PUB_REG_BASE_OFFS + 0x00000260)
#define HWIO_MSS_QDSP6SS_PLL_RCG_UPDATE_STATUS_RMSK                                        0x3
#define HWIO_MSS_QDSP6SS_PLL_RCG_UPDATE_STATUS_IN          \
        in_dword_masked(HWIO_MSS_QDSP6SS_PLL_RCG_UPDATE_STATUS_ADDR, HWIO_MSS_QDSP6SS_PLL_RCG_UPDATE_STATUS_RMSK)
#define HWIO_MSS_QDSP6SS_PLL_RCG_UPDATE_STATUS_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_PLL_RCG_UPDATE_STATUS_ADDR, m)
#define HWIO_MSS_QDSP6SS_PLL_RCG_UPDATE_STATUS_RCG2PLL_UPD_STATUS_BMSK                     0x2
#define HWIO_MSS_QDSP6SS_PLL_RCG_UPDATE_STATUS_RCG2PLL_UPD_STATUS_SHFT                     0x1
#define HWIO_MSS_QDSP6SS_PLL_RCG_UPDATE_STATUS_PLL2RCG_UPD_STATUS_BMSK                     0x1
#define HWIO_MSS_QDSP6SS_PLL_RCG_UPDATE_STATUS_PLL2RCG_UPD_STATUS_SHFT                     0x0

#define HWIO_MSS_QDSP6SS_PLL_RCG_UPDATE_CFG_ADDR                                    (MSS_QDSP6V66SS_PUB_REG_BASE      + 0x00000264)
#define HWIO_MSS_QDSP6SS_PLL_RCG_UPDATE_CFG_OFFS                                    (MSS_QDSP6V66SS_PUB_REG_BASE_OFFS + 0x00000264)
#define HWIO_MSS_QDSP6SS_PLL_RCG_UPDATE_CFG_RMSK                                           0x3
#define HWIO_MSS_QDSP6SS_PLL_RCG_UPDATE_CFG_IN          \
        in_dword_masked(HWIO_MSS_QDSP6SS_PLL_RCG_UPDATE_CFG_ADDR, HWIO_MSS_QDSP6SS_PLL_RCG_UPDATE_CFG_RMSK)
#define HWIO_MSS_QDSP6SS_PLL_RCG_UPDATE_CFG_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_PLL_RCG_UPDATE_CFG_ADDR, m)
#define HWIO_MSS_QDSP6SS_PLL_RCG_UPDATE_CFG_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_PLL_RCG_UPDATE_CFG_ADDR,v)
#define HWIO_MSS_QDSP6SS_PLL_RCG_UPDATE_CFG_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_PLL_RCG_UPDATE_CFG_ADDR,m,v,HWIO_MSS_QDSP6SS_PLL_RCG_UPDATE_CFG_IN)
#define HWIO_MSS_QDSP6SS_PLL_RCG_UPDATE_CFG_RCG2PLL_UPD_EN_BMSK                            0x2
#define HWIO_MSS_QDSP6SS_PLL_RCG_UPDATE_CFG_RCG2PLL_UPD_EN_SHFT                            0x1
#define HWIO_MSS_QDSP6SS_PLL_RCG_UPDATE_CFG_PLL2RCG_UPD_EN_BMSK                            0x1
#define HWIO_MSS_QDSP6SS_PLL_RCG_UPDATE_CFG_PLL2RCG_UPD_EN_SHFT                            0x0

#define HWIO_MSS_QDSP6SS_PLL_RCG_UPDATE_DLYCTL_ADDR                                 (MSS_QDSP6V66SS_PUB_REG_BASE      + 0x00000268)
#define HWIO_MSS_QDSP6SS_PLL_RCG_UPDATE_DLYCTL_OFFS                                 (MSS_QDSP6V66SS_PUB_REG_BASE_OFFS + 0x00000268)
#define HWIO_MSS_QDSP6SS_PLL_RCG_UPDATE_DLYCTL_RMSK                                       0x1f
#define HWIO_MSS_QDSP6SS_PLL_RCG_UPDATE_DLYCTL_IN          \
        in_dword_masked(HWIO_MSS_QDSP6SS_PLL_RCG_UPDATE_DLYCTL_ADDR, HWIO_MSS_QDSP6SS_PLL_RCG_UPDATE_DLYCTL_RMSK)
#define HWIO_MSS_QDSP6SS_PLL_RCG_UPDATE_DLYCTL_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_PLL_RCG_UPDATE_DLYCTL_ADDR, m)
#define HWIO_MSS_QDSP6SS_PLL_RCG_UPDATE_DLYCTL_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_PLL_RCG_UPDATE_DLYCTL_ADDR,v)
#define HWIO_MSS_QDSP6SS_PLL_RCG_UPDATE_DLYCTL_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_PLL_RCG_UPDATE_DLYCTL_ADDR,m,v,HWIO_MSS_QDSP6SS_PLL_RCG_UPDATE_DLYCTL_IN)
#define HWIO_MSS_QDSP6SS_PLL_RCG_UPDATE_DLYCTL_MATCH_VALUE_BMSK                           0x1f
#define HWIO_MSS_QDSP6SS_PLL_RCG_UPDATE_DLYCTL_MATCH_VALUE_SHFT                            0x0

#define HWIO_MSS_QDSP6SS_PLL_RCG_UPDATE_CMD_ADDR                                    (MSS_QDSP6V66SS_PUB_REG_BASE      + 0x0000026c)
#define HWIO_MSS_QDSP6SS_PLL_RCG_UPDATE_CMD_OFFS                                    (MSS_QDSP6V66SS_PUB_REG_BASE_OFFS + 0x0000026c)
#define HWIO_MSS_QDSP6SS_PLL_RCG_UPDATE_CMD_RMSK                                           0x1
#define HWIO_MSS_QDSP6SS_PLL_RCG_UPDATE_CMD_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_PLL_RCG_UPDATE_CMD_ADDR,v)
#define HWIO_MSS_QDSP6SS_PLL_RCG_UPDATE_CMD_CMD_BMSK                                       0x1
#define HWIO_MSS_QDSP6SS_PLL_RCG_UPDATE_CMD_CMD_SHFT                                       0x0

#define HWIO_MSS_QDSP6SS_DBG_NMI_CFG_ADDR                                           (MSS_QDSP6V66SS_PUB_REG_BASE      + 0x00000300)
#define HWIO_MSS_QDSP6SS_DBG_NMI_CFG_OFFS                                           (MSS_QDSP6V66SS_PUB_REG_BASE_OFFS + 0x00000300)
#define HWIO_MSS_QDSP6SS_DBG_NMI_CFG_RMSK                                                  0xf
#define HWIO_MSS_QDSP6SS_DBG_NMI_CFG_IN          \
        in_dword_masked(HWIO_MSS_QDSP6SS_DBG_NMI_CFG_ADDR, HWIO_MSS_QDSP6SS_DBG_NMI_CFG_RMSK)
#define HWIO_MSS_QDSP6SS_DBG_NMI_CFG_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_DBG_NMI_CFG_ADDR, m)
#define HWIO_MSS_QDSP6SS_DBG_NMI_CFG_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_DBG_NMI_CFG_ADDR,v)
#define HWIO_MSS_QDSP6SS_DBG_NMI_CFG_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_DBG_NMI_CFG_ADDR,m,v,HWIO_MSS_QDSP6SS_DBG_NMI_CFG_IN)
#define HWIO_MSS_QDSP6SS_DBG_NMI_CFG_DBG_NMI_EN_IN_APCR_BMSK                               0x8
#define HWIO_MSS_QDSP6SS_DBG_NMI_CFG_DBG_NMI_EN_IN_APCR_SHFT                               0x3
#define HWIO_MSS_QDSP6SS_DBG_NMI_CFG_DBG_NMI_EN_IN_PWR_COLLAPSE_BMSK                       0x4
#define HWIO_MSS_QDSP6SS_DBG_NMI_CFG_DBG_NMI_EN_IN_PWR_COLLAPSE_SHFT                       0x2
#define HWIO_MSS_QDSP6SS_DBG_NMI_CFG_DBG_NMI_DISABLE_BMSK                                  0x2
#define HWIO_MSS_QDSP6SS_DBG_NMI_CFG_DBG_NMI_DISABLE_SHFT                                  0x1
#define HWIO_MSS_QDSP6SS_DBG_NMI_CFG_DBG_NMI_WAKEUP_DISABLE_BMSK                           0x1
#define HWIO_MSS_QDSP6SS_DBG_NMI_CFG_DBG_NMI_WAKEUP_DISABLE_SHFT                           0x0

#define HWIO_MSS_QDSP6SS_DBG_NMI_PWR_STATUS_ADDR                                    (MSS_QDSP6V66SS_PUB_REG_BASE      + 0x00000304)
#define HWIO_MSS_QDSP6SS_DBG_NMI_PWR_STATUS_OFFS                                    (MSS_QDSP6V66SS_PUB_REG_BASE_OFFS + 0x00000304)
#define HWIO_MSS_QDSP6SS_DBG_NMI_PWR_STATUS_RMSK                                           0x3
#define HWIO_MSS_QDSP6SS_DBG_NMI_PWR_STATUS_IN          \
        in_dword_masked(HWIO_MSS_QDSP6SS_DBG_NMI_PWR_STATUS_ADDR, HWIO_MSS_QDSP6SS_DBG_NMI_PWR_STATUS_RMSK)
#define HWIO_MSS_QDSP6SS_DBG_NMI_PWR_STATUS_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_DBG_NMI_PWR_STATUS_ADDR, m)
#define HWIO_MSS_QDSP6SS_DBG_NMI_PWR_STATUS_QDSP6_PWR_COLLAPSE_BMSK                        0x2
#define HWIO_MSS_QDSP6SS_DBG_NMI_PWR_STATUS_QDSP6_PWR_COLLAPSE_SHFT                        0x1
#define HWIO_MSS_QDSP6SS_DBG_NMI_PWR_STATUS_QDSP6_IN_SLEEP_BMSK                            0x1
#define HWIO_MSS_QDSP6SS_DBG_NMI_PWR_STATUS_QDSP6_IN_SLEEP_SHFT                            0x0

#define HWIO_MSS_QDSP6SS_BOOT_CORE_START_ADDR                                       (MSS_QDSP6V66SS_PUB_REG_BASE      + 0x00000400)
#define HWIO_MSS_QDSP6SS_BOOT_CORE_START_OFFS                                       (MSS_QDSP6V66SS_PUB_REG_BASE_OFFS + 0x00000400)
#define HWIO_MSS_QDSP6SS_BOOT_CORE_START_RMSK                                              0x1
#define HWIO_MSS_QDSP6SS_BOOT_CORE_START_IN          \
        in_dword_masked(HWIO_MSS_QDSP6SS_BOOT_CORE_START_ADDR, HWIO_MSS_QDSP6SS_BOOT_CORE_START_RMSK)
#define HWIO_MSS_QDSP6SS_BOOT_CORE_START_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_BOOT_CORE_START_ADDR, m)
#define HWIO_MSS_QDSP6SS_BOOT_CORE_START_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_BOOT_CORE_START_ADDR,v)
#define HWIO_MSS_QDSP6SS_BOOT_CORE_START_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_BOOT_CORE_START_ADDR,m,v,HWIO_MSS_QDSP6SS_BOOT_CORE_START_IN)
#define HWIO_MSS_QDSP6SS_BOOT_CORE_START_START_BMSK                                        0x1
#define HWIO_MSS_QDSP6SS_BOOT_CORE_START_START_SHFT                                        0x0

#define HWIO_MSS_QDSP6SS_BOOT_CMD_ADDR                                              (MSS_QDSP6V66SS_PUB_REG_BASE      + 0x00000404)
#define HWIO_MSS_QDSP6SS_BOOT_CMD_OFFS                                              (MSS_QDSP6V66SS_PUB_REG_BASE_OFFS + 0x00000404)
#define HWIO_MSS_QDSP6SS_BOOT_CMD_RMSK                                                     0x1
#define HWIO_MSS_QDSP6SS_BOOT_CMD_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_BOOT_CMD_ADDR,v)
#define HWIO_MSS_QDSP6SS_BOOT_CMD_CMD_BMSK                                                 0x1
#define HWIO_MSS_QDSP6SS_BOOT_CMD_CMD_SHFT                                                 0x0

#define HWIO_MSS_QDSP6SS_BOOT_STATUS_ADDR                                           (MSS_QDSP6V66SS_PUB_REG_BASE      + 0x00000408)
#define HWIO_MSS_QDSP6SS_BOOT_STATUS_OFFS                                           (MSS_QDSP6V66SS_PUB_REG_BASE_OFFS + 0x00000408)
#define HWIO_MSS_QDSP6SS_BOOT_STATUS_RMSK                                                  0x1
#define HWIO_MSS_QDSP6SS_BOOT_STATUS_IN          \
        in_dword_masked(HWIO_MSS_QDSP6SS_BOOT_STATUS_ADDR, HWIO_MSS_QDSP6SS_BOOT_STATUS_RMSK)
#define HWIO_MSS_QDSP6SS_BOOT_STATUS_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_BOOT_STATUS_ADDR, m)
#define HWIO_MSS_QDSP6SS_BOOT_STATUS_STATUS_BMSK                                           0x1
#define HWIO_MSS_QDSP6SS_BOOT_STATUS_STATUS_SHFT                                           0x0

#define HWIO_MSS_QDSP6SS_CORE_BHS_CFG_ADDR                                          (MSS_QDSP6V66SS_PUB_REG_BASE      + 0x00000410)
#define HWIO_MSS_QDSP6SS_CORE_BHS_CFG_OFFS                                          (MSS_QDSP6V66SS_PUB_REG_BASE_OFFS + 0x00000410)
#define HWIO_MSS_QDSP6SS_CORE_BHS_CFG_RMSK                                                 0x1
#define HWIO_MSS_QDSP6SS_CORE_BHS_CFG_IN          \
        in_dword_masked(HWIO_MSS_QDSP6SS_CORE_BHS_CFG_ADDR, HWIO_MSS_QDSP6SS_CORE_BHS_CFG_RMSK)
#define HWIO_MSS_QDSP6SS_CORE_BHS_CFG_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_CORE_BHS_CFG_ADDR, m)
#define HWIO_MSS_QDSP6SS_CORE_BHS_CFG_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_CORE_BHS_CFG_ADDR,v)
#define HWIO_MSS_QDSP6SS_CORE_BHS_CFG_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_CORE_BHS_CFG_ADDR,m,v,HWIO_MSS_QDSP6SS_CORE_BHS_CFG_IN)
#define HWIO_MSS_QDSP6SS_CORE_BHS_CFG_BHS_ON_BMSK                                          0x1
#define HWIO_MSS_QDSP6SS_CORE_BHS_CFG_BHS_ON_SHFT                                          0x0

#define HWIO_MSS_QDSP6SS_CORE_BHS_CMD_ADDR                                          (MSS_QDSP6V66SS_PUB_REG_BASE      + 0x00000414)
#define HWIO_MSS_QDSP6SS_CORE_BHS_CMD_OFFS                                          (MSS_QDSP6V66SS_PUB_REG_BASE_OFFS + 0x00000414)
#define HWIO_MSS_QDSP6SS_CORE_BHS_CMD_RMSK                                                 0x1
#define HWIO_MSS_QDSP6SS_CORE_BHS_CMD_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_CORE_BHS_CMD_ADDR,v)
#define HWIO_MSS_QDSP6SS_CORE_BHS_CMD_UPDATE_BMSK                                          0x1
#define HWIO_MSS_QDSP6SS_CORE_BHS_CMD_UPDATE_SHFT                                          0x0

#define HWIO_MSS_QDSP6SS_CORE_BHS_STATUS_ADDR                                       (MSS_QDSP6V66SS_PUB_REG_BASE      + 0x00000418)
#define HWIO_MSS_QDSP6SS_CORE_BHS_STATUS_OFFS                                       (MSS_QDSP6V66SS_PUB_REG_BASE_OFFS + 0x00000418)
#define HWIO_MSS_QDSP6SS_CORE_BHS_STATUS_RMSK                                              0x1
#define HWIO_MSS_QDSP6SS_CORE_BHS_STATUS_IN          \
        in_dword_masked(HWIO_MSS_QDSP6SS_CORE_BHS_STATUS_ADDR, HWIO_MSS_QDSP6SS_CORE_BHS_STATUS_RMSK)
#define HWIO_MSS_QDSP6SS_CORE_BHS_STATUS_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_CORE_BHS_STATUS_ADDR, m)
#define HWIO_MSS_QDSP6SS_CORE_BHS_STATUS_BHS_ON_BMSK                                       0x1
#define HWIO_MSS_QDSP6SS_CORE_BHS_STATUS_BHS_ON_SHFT                                       0x0

#define HWIO_MSS_QDSP6SS_LDO_CTL_CFG_ADDR                                           (MSS_QDSP6V66SS_PUB_REG_BASE      + 0x00000420)
#define HWIO_MSS_QDSP6SS_LDO_CTL_CFG_OFFS                                           (MSS_QDSP6V66SS_PUB_REG_BASE_OFFS + 0x00000420)
#define HWIO_MSS_QDSP6SS_LDO_CTL_CFG_RMSK                                                  0x7
#define HWIO_MSS_QDSP6SS_LDO_CTL_CFG_IN          \
        in_dword_masked(HWIO_MSS_QDSP6SS_LDO_CTL_CFG_ADDR, HWIO_MSS_QDSP6SS_LDO_CTL_CFG_RMSK)
#define HWIO_MSS_QDSP6SS_LDO_CTL_CFG_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_LDO_CTL_CFG_ADDR, m)
#define HWIO_MSS_QDSP6SS_LDO_CTL_CFG_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_LDO_CTL_CFG_ADDR,v)
#define HWIO_MSS_QDSP6SS_LDO_CTL_CFG_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_LDO_CTL_CFG_ADDR,m,v,HWIO_MSS_QDSP6SS_LDO_CTL_CFG_IN)
#define HWIO_MSS_QDSP6SS_LDO_CTL_CFG_LDO_PWR_UP_BG_BMSK                                    0x4
#define HWIO_MSS_QDSP6SS_LDO_CTL_CFG_LDO_PWR_UP_BG_SHFT                                    0x2
#define HWIO_MSS_QDSP6SS_LDO_CTL_CFG_LDO_PWR_UP_BMSK                                       0x2
#define HWIO_MSS_QDSP6SS_LDO_CTL_CFG_LDO_PWR_UP_SHFT                                       0x1
#define HWIO_MSS_QDSP6SS_LDO_CTL_CFG_LDO_BYP_BMSK                                          0x1
#define HWIO_MSS_QDSP6SS_LDO_CTL_CFG_LDO_BYP_SHFT                                          0x0

#define HWIO_MSS_QDSP6SS_LDO_CTL_CMD_ADDR                                           (MSS_QDSP6V66SS_PUB_REG_BASE      + 0x00000424)
#define HWIO_MSS_QDSP6SS_LDO_CTL_CMD_OFFS                                           (MSS_QDSP6V66SS_PUB_REG_BASE_OFFS + 0x00000424)
#define HWIO_MSS_QDSP6SS_LDO_CTL_CMD_RMSK                                                  0x7
#define HWIO_MSS_QDSP6SS_LDO_CTL_CMD_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_LDO_CTL_CMD_ADDR,v)
#define HWIO_MSS_QDSP6SS_LDO_CTL_CMD_LDO_PWR_UP_BG_UPDATE_BMSK                             0x4
#define HWIO_MSS_QDSP6SS_LDO_CTL_CMD_LDO_PWR_UP_BG_UPDATE_SHFT                             0x2
#define HWIO_MSS_QDSP6SS_LDO_CTL_CMD_LDO_PWR_UP_UPDATE_BMSK                                0x2
#define HWIO_MSS_QDSP6SS_LDO_CTL_CMD_LDO_PWR_UP_UPDATE_SHFT                                0x1
#define HWIO_MSS_QDSP6SS_LDO_CTL_CMD_LDO_BYP_UPDATE_BMSK                                   0x1
#define HWIO_MSS_QDSP6SS_LDO_CTL_CMD_LDO_BYP_UPDATE_SHFT                                   0x0

#define HWIO_MSS_QDSP6SS_LDO_CTL_STATUS_ADDR                                        (MSS_QDSP6V66SS_PUB_REG_BASE      + 0x00000428)
#define HWIO_MSS_QDSP6SS_LDO_CTL_STATUS_OFFS                                        (MSS_QDSP6V66SS_PUB_REG_BASE_OFFS + 0x00000428)
#define HWIO_MSS_QDSP6SS_LDO_CTL_STATUS_RMSK                                               0x7
#define HWIO_MSS_QDSP6SS_LDO_CTL_STATUS_IN          \
        in_dword_masked(HWIO_MSS_QDSP6SS_LDO_CTL_STATUS_ADDR, HWIO_MSS_QDSP6SS_LDO_CTL_STATUS_RMSK)
#define HWIO_MSS_QDSP6SS_LDO_CTL_STATUS_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_LDO_CTL_STATUS_ADDR, m)
#define HWIO_MSS_QDSP6SS_LDO_CTL_STATUS_LDO_PWR_UP_BG_BMSK                                 0x4
#define HWIO_MSS_QDSP6SS_LDO_CTL_STATUS_LDO_PWR_UP_BG_SHFT                                 0x2
#define HWIO_MSS_QDSP6SS_LDO_CTL_STATUS_LDO_PWR_UP_BMSK                                    0x2
#define HWIO_MSS_QDSP6SS_LDO_CTL_STATUS_LDO_PWR_UP_SHFT                                    0x1
#define HWIO_MSS_QDSP6SS_LDO_CTL_STATUS_LDO_BYP_BMSK                                       0x1
#define HWIO_MSS_QDSP6SS_LDO_CTL_STATUS_LDO_BYP_SHFT                                       0x0

#define HWIO_MSS_QDSP6SS_MEM_CFG_ADDR                                               (MSS_QDSP6V66SS_PUB_REG_BASE      + 0x00000430)
#define HWIO_MSS_QDSP6SS_MEM_CFG_OFFS                                               (MSS_QDSP6V66SS_PUB_REG_BASE_OFFS + 0x00000430)
#define HWIO_MSS_QDSP6SS_MEM_CFG_RMSK                                                 0x1f001f
#define HWIO_MSS_QDSP6SS_MEM_CFG_IN          \
        in_dword_masked(HWIO_MSS_QDSP6SS_MEM_CFG_ADDR, HWIO_MSS_QDSP6SS_MEM_CFG_RMSK)
#define HWIO_MSS_QDSP6SS_MEM_CFG_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_MEM_CFG_ADDR, m)
#define HWIO_MSS_QDSP6SS_MEM_CFG_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_MEM_CFG_ADDR,v)
#define HWIO_MSS_QDSP6SS_MEM_CFG_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_MEM_CFG_ADDR,m,v,HWIO_MSS_QDSP6SS_MEM_CFG_IN)
#define HWIO_MSS_QDSP6SS_MEM_CFG_L2MEM_SLP_RET_N_BMSK                                 0x1e0000
#define HWIO_MSS_QDSP6SS_MEM_CFG_L2MEM_SLP_RET_N_SHFT                                     0x11
#define HWIO_MSS_QDSP6SS_MEM_CFG_MEM_SLP_RET_N_BMSK                                    0x10000
#define HWIO_MSS_QDSP6SS_MEM_CFG_MEM_SLP_RET_N_SHFT                                       0x10
#define HWIO_MSS_QDSP6SS_MEM_CFG_L2MEM_SLP_NRET_N_BMSK                                    0x1e
#define HWIO_MSS_QDSP6SS_MEM_CFG_L2MEM_SLP_NRET_N_SHFT                                     0x1
#define HWIO_MSS_QDSP6SS_MEM_CFG_MEM_SLP_NRET_N_BMSK                                       0x1
#define HWIO_MSS_QDSP6SS_MEM_CFG_MEM_SLP_NRET_N_SHFT                                       0x0

#define HWIO_MSS_QDSP6SS_MEM_CMD_ADDR                                               (MSS_QDSP6V66SS_PUB_REG_BASE      + 0x00000434)
#define HWIO_MSS_QDSP6SS_MEM_CMD_OFFS                                               (MSS_QDSP6V66SS_PUB_REG_BASE_OFFS + 0x00000434)
#define HWIO_MSS_QDSP6SS_MEM_CMD_RMSK                                                      0x3
#define HWIO_MSS_QDSP6SS_MEM_CMD_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_MEM_CMD_ADDR,v)
#define HWIO_MSS_QDSP6SS_MEM_CMD_UPDATE_SLP_RET_N_BMSK                                     0x2
#define HWIO_MSS_QDSP6SS_MEM_CMD_UPDATE_SLP_RET_N_SHFT                                     0x1
#define HWIO_MSS_QDSP6SS_MEM_CMD_UPDATE_SLP_NRET_N_BMSK                                    0x1
#define HWIO_MSS_QDSP6SS_MEM_CMD_UPDATE_SLP_NRET_N_SHFT                                    0x0

#define HWIO_MSS_QDSP6SS_MEM_STATUS_ADDR                                            (MSS_QDSP6V66SS_PUB_REG_BASE      + 0x00000438)
#define HWIO_MSS_QDSP6SS_MEM_STATUS_OFFS                                            (MSS_QDSP6V66SS_PUB_REG_BASE_OFFS + 0x00000438)
#define HWIO_MSS_QDSP6SS_MEM_STATUS_RMSK                                              0x1f001f
#define HWIO_MSS_QDSP6SS_MEM_STATUS_IN          \
        in_dword_masked(HWIO_MSS_QDSP6SS_MEM_STATUS_ADDR, HWIO_MSS_QDSP6SS_MEM_STATUS_RMSK)
#define HWIO_MSS_QDSP6SS_MEM_STATUS_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_MEM_STATUS_ADDR, m)
#define HWIO_MSS_QDSP6SS_MEM_STATUS_L2MEM_SLP_RET_N_BMSK                              0x1e0000
#define HWIO_MSS_QDSP6SS_MEM_STATUS_L2MEM_SLP_RET_N_SHFT                                  0x11
#define HWIO_MSS_QDSP6SS_MEM_STATUS_MEM_SLP_RET_N_BMSK                                 0x10000
#define HWIO_MSS_QDSP6SS_MEM_STATUS_MEM_SLP_RET_N_SHFT                                    0x10
#define HWIO_MSS_QDSP6SS_MEM_STATUS_L2MEM_SLP_NRET_N_BMSK                                 0x1e
#define HWIO_MSS_QDSP6SS_MEM_STATUS_L2MEM_SLP_NRET_N_SHFT                                  0x1
#define HWIO_MSS_QDSP6SS_MEM_STATUS_MEM_SLP_NRET_N_BMSK                                    0x1
#define HWIO_MSS_QDSP6SS_MEM_STATUS_MEM_SLP_NRET_N_SHFT                                    0x0

#define HWIO_MSS_QDSP6SS_RESET_CFG_ADDR                                             (MSS_QDSP6V66SS_PUB_REG_BASE      + 0x00000440)
#define HWIO_MSS_QDSP6SS_RESET_CFG_OFFS                                             (MSS_QDSP6V66SS_PUB_REG_BASE_OFFS + 0x00000440)
#define HWIO_MSS_QDSP6SS_RESET_CFG_RMSK                                                 0x3fff
#define HWIO_MSS_QDSP6SS_RESET_CFG_IN          \
        in_dword_masked(HWIO_MSS_QDSP6SS_RESET_CFG_ADDR, HWIO_MSS_QDSP6SS_RESET_CFG_RMSK)
#define HWIO_MSS_QDSP6SS_RESET_CFG_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_RESET_CFG_ADDR, m)
#define HWIO_MSS_QDSP6SS_RESET_CFG_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_RESET_CFG_ADDR,v)
#define HWIO_MSS_QDSP6SS_RESET_CFG_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_RESET_CFG_ADDR,m,v,HWIO_MSS_QDSP6SS_RESET_CFG_IN)
#define HWIO_MSS_QDSP6SS_RESET_CFG_MNOC_BMSK                                            0x2000
#define HWIO_MSS_QDSP6SS_RESET_CFG_MNOC_SHFT                                               0xd
#define HWIO_MSS_QDSP6SS_RESET_CFG_APB_BMSK                                             0x1000
#define HWIO_MSS_QDSP6SS_RESET_CFG_APB_SHFT                                                0xc
#define HWIO_MSS_QDSP6SS_RESET_CFG_AXIS2_BMSK                                            0x800
#define HWIO_MSS_QDSP6SS_RESET_CFG_AXIS2_SHFT                                              0xb
#define HWIO_MSS_QDSP6SS_RESET_CFG_CSI1_BMSK                                             0x400
#define HWIO_MSS_QDSP6SS_RESET_CFG_CSI1_SHFT                                               0xa
#define HWIO_MSS_QDSP6SS_RESET_CFG_CSI0_BMSK                                             0x200
#define HWIO_MSS_QDSP6SS_RESET_CFG_CSI0_SHFT                                               0x9
#define HWIO_MSS_QDSP6SS_RESET_CFG_ARCH_BMSK                                             0x100
#define HWIO_MSS_QDSP6SS_RESET_CFG_ARCH_SHFT                                               0x8
#define HWIO_MSS_QDSP6SS_RESET_CFG_CORE_BMSK                                              0x80
#define HWIO_MSS_QDSP6SS_RESET_CFG_CORE_SHFT                                               0x7
#define HWIO_MSS_QDSP6SS_RESET_CFG_AXIM2_BMSK                                             0x40
#define HWIO_MSS_QDSP6SS_RESET_CFG_AXIM2_SHFT                                              0x6
#define HWIO_MSS_QDSP6SS_RESET_CFG_AXIS_BMSK                                              0x20
#define HWIO_MSS_QDSP6SS_RESET_CFG_AXIS_SHFT                                               0x5
#define HWIO_MSS_QDSP6SS_RESET_CFG_ATBM_BMSK                                              0x10
#define HWIO_MSS_QDSP6SS_RESET_CFG_ATBM_SHFT                                               0x4
#define HWIO_MSS_QDSP6SS_RESET_CFG_AHBM_BMSK                                               0x8
#define HWIO_MSS_QDSP6SS_RESET_CFG_AHBM_SHFT                                               0x3
#define HWIO_MSS_QDSP6SS_RESET_CFG_AXIM_BMSK                                               0x4
#define HWIO_MSS_QDSP6SS_RESET_CFG_AXIM_SHFT                                               0x2
#define HWIO_MSS_QDSP6SS_RESET_CFG_ISDB_ETM_BMSK                                           0x2
#define HWIO_MSS_QDSP6SS_RESET_CFG_ISDB_ETM_SHFT                                           0x1
#define HWIO_MSS_QDSP6SS_RESET_CFG_RET_BMSK                                                0x1
#define HWIO_MSS_QDSP6SS_RESET_CFG_RET_SHFT                                                0x0

#define HWIO_MSS_QDSP6SS_RESET_CMD_ADDR                                             (MSS_QDSP6V66SS_PUB_REG_BASE      + 0x00000444)
#define HWIO_MSS_QDSP6SS_RESET_CMD_OFFS                                             (MSS_QDSP6V66SS_PUB_REG_BASE_OFFS + 0x00000444)
#define HWIO_MSS_QDSP6SS_RESET_CMD_RMSK                                                    0x1
#define HWIO_MSS_QDSP6SS_RESET_CMD_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_RESET_CMD_ADDR,v)
#define HWIO_MSS_QDSP6SS_RESET_CMD_UPDATE_BMSK                                             0x1
#define HWIO_MSS_QDSP6SS_RESET_CMD_UPDATE_SHFT                                             0x0

#define HWIO_MSS_QDSP6SS_RESET_STATUS_ADDR                                          (MSS_QDSP6V66SS_PUB_REG_BASE      + 0x00000448)
#define HWIO_MSS_QDSP6SS_RESET_STATUS_OFFS                                          (MSS_QDSP6V66SS_PUB_REG_BASE_OFFS + 0x00000448)
#define HWIO_MSS_QDSP6SS_RESET_STATUS_RMSK                                              0x3fff
#define HWIO_MSS_QDSP6SS_RESET_STATUS_IN          \
        in_dword_masked(HWIO_MSS_QDSP6SS_RESET_STATUS_ADDR, HWIO_MSS_QDSP6SS_RESET_STATUS_RMSK)
#define HWIO_MSS_QDSP6SS_RESET_STATUS_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_RESET_STATUS_ADDR, m)
#define HWIO_MSS_QDSP6SS_RESET_STATUS_MNOC_BMSK                                         0x2000
#define HWIO_MSS_QDSP6SS_RESET_STATUS_MNOC_SHFT                                            0xd
#define HWIO_MSS_QDSP6SS_RESET_STATUS_APB_BMSK                                          0x1000
#define HWIO_MSS_QDSP6SS_RESET_STATUS_APB_SHFT                                             0xc
#define HWIO_MSS_QDSP6SS_RESET_STATUS_AXIS2_BMSK                                         0x800
#define HWIO_MSS_QDSP6SS_RESET_STATUS_AXIS2_SHFT                                           0xb
#define HWIO_MSS_QDSP6SS_RESET_STATUS_CSI1_BMSK                                          0x400
#define HWIO_MSS_QDSP6SS_RESET_STATUS_CSI1_SHFT                                            0xa
#define HWIO_MSS_QDSP6SS_RESET_STATUS_CSI0_BMSK                                          0x200
#define HWIO_MSS_QDSP6SS_RESET_STATUS_CSI0_SHFT                                            0x9
#define HWIO_MSS_QDSP6SS_RESET_STATUS_ARCH_BMSK                                          0x100
#define HWIO_MSS_QDSP6SS_RESET_STATUS_ARCH_SHFT                                            0x8
#define HWIO_MSS_QDSP6SS_RESET_STATUS_CORE_BMSK                                           0x80
#define HWIO_MSS_QDSP6SS_RESET_STATUS_CORE_SHFT                                            0x7
#define HWIO_MSS_QDSP6SS_RESET_STATUS_AXIM2_BMSK                                          0x40
#define HWIO_MSS_QDSP6SS_RESET_STATUS_AXIM2_SHFT                                           0x6
#define HWIO_MSS_QDSP6SS_RESET_STATUS_AXIS_BMSK                                           0x20
#define HWIO_MSS_QDSP6SS_RESET_STATUS_AXIS_SHFT                                            0x5
#define HWIO_MSS_QDSP6SS_RESET_STATUS_ATBM_BMSK                                           0x10
#define HWIO_MSS_QDSP6SS_RESET_STATUS_ATBM_SHFT                                            0x4
#define HWIO_MSS_QDSP6SS_RESET_STATUS_AHBM_BMSK                                            0x8
#define HWIO_MSS_QDSP6SS_RESET_STATUS_AHBM_SHFT                                            0x3
#define HWIO_MSS_QDSP6SS_RESET_STATUS_AXIM_BMSK                                            0x4
#define HWIO_MSS_QDSP6SS_RESET_STATUS_AXIM_SHFT                                            0x2
#define HWIO_MSS_QDSP6SS_RESET_STATUS_ISDB_ETM_BMSK                                        0x2
#define HWIO_MSS_QDSP6SS_RESET_STATUS_ISDB_ETM_SHFT                                        0x1
#define HWIO_MSS_QDSP6SS_RESET_STATUS_RET_BMSK                                             0x1
#define HWIO_MSS_QDSP6SS_RESET_STATUS_RET_SHFT                                             0x0

#define HWIO_MSS_QDSP6SS_CLAMP_CFG_ADDR                                             (MSS_QDSP6V66SS_PUB_REG_BASE      + 0x00000450)
#define HWIO_MSS_QDSP6SS_CLAMP_CFG_OFFS                                             (MSS_QDSP6V66SS_PUB_REG_BASE_OFFS + 0x00000450)
#define HWIO_MSS_QDSP6SS_CLAMP_CFG_RMSK                                                    0xd
#define HWIO_MSS_QDSP6SS_CLAMP_CFG_IN          \
        in_dword_masked(HWIO_MSS_QDSP6SS_CLAMP_CFG_ADDR, HWIO_MSS_QDSP6SS_CLAMP_CFG_RMSK)
#define HWIO_MSS_QDSP6SS_CLAMP_CFG_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_CLAMP_CFG_ADDR, m)
#define HWIO_MSS_QDSP6SS_CLAMP_CFG_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_CLAMP_CFG_ADDR,v)
#define HWIO_MSS_QDSP6SS_CLAMP_CFG_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_CLAMP_CFG_ADDR,m,v,HWIO_MSS_QDSP6SS_CLAMP_CFG_IN)
#define HWIO_MSS_QDSP6SS_CLAMP_CFG_CLAMP_MSS2CX_BMSK                                       0x8
#define HWIO_MSS_QDSP6SS_CLAMP_CFG_CLAMP_MSS2CX_SHFT                                       0x3
#define HWIO_MSS_QDSP6SS_CLAMP_CFG_CLAMP_QMC_MEM_BMSK                                      0x4
#define HWIO_MSS_QDSP6SS_CLAMP_CFG_CLAMP_QMC_MEM_SHFT                                      0x2
#define HWIO_MSS_QDSP6SS_CLAMP_CFG_CLAMP_IO_BMSK                                           0x1
#define HWIO_MSS_QDSP6SS_CLAMP_CFG_CLAMP_IO_SHFT                                           0x0

#define HWIO_MSS_QDSP6SS_CLAMP_CMD_ADDR                                             (MSS_QDSP6V66SS_PUB_REG_BASE      + 0x00000454)
#define HWIO_MSS_QDSP6SS_CLAMP_CMD_OFFS                                             (MSS_QDSP6V66SS_PUB_REG_BASE_OFFS + 0x00000454)
#define HWIO_MSS_QDSP6SS_CLAMP_CMD_RMSK                                                    0xd
#define HWIO_MSS_QDSP6SS_CLAMP_CMD_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_CLAMP_CMD_ADDR,v)
#define HWIO_MSS_QDSP6SS_CLAMP_CMD_UPDATE_CLAMP_MSS2CX_BMSK                                0x8
#define HWIO_MSS_QDSP6SS_CLAMP_CMD_UPDATE_CLAMP_MSS2CX_SHFT                                0x3
#define HWIO_MSS_QDSP6SS_CLAMP_CMD_UPDATE_CLAMP_QMC_MEM_BMSK                               0x4
#define HWIO_MSS_QDSP6SS_CLAMP_CMD_UPDATE_CLAMP_QMC_MEM_SHFT                               0x2
#define HWIO_MSS_QDSP6SS_CLAMP_CMD_UPDATE_CLAMP_IO_BMSK                                    0x1
#define HWIO_MSS_QDSP6SS_CLAMP_CMD_UPDATE_CLAMP_IO_SHFT                                    0x0

#define HWIO_MSS_QDSP6SS_CLAMP_STATUS_ADDR                                          (MSS_QDSP6V66SS_PUB_REG_BASE      + 0x00000458)
#define HWIO_MSS_QDSP6SS_CLAMP_STATUS_OFFS                                          (MSS_QDSP6V66SS_PUB_REG_BASE_OFFS + 0x00000458)
#define HWIO_MSS_QDSP6SS_CLAMP_STATUS_RMSK                                                 0xd
#define HWIO_MSS_QDSP6SS_CLAMP_STATUS_IN          \
        in_dword_masked(HWIO_MSS_QDSP6SS_CLAMP_STATUS_ADDR, HWIO_MSS_QDSP6SS_CLAMP_STATUS_RMSK)
#define HWIO_MSS_QDSP6SS_CLAMP_STATUS_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_CLAMP_STATUS_ADDR, m)
#define HWIO_MSS_QDSP6SS_CLAMP_STATUS_CLAMP_MSS2CX_BMSK                                    0x8
#define HWIO_MSS_QDSP6SS_CLAMP_STATUS_CLAMP_MSS2CX_SHFT                                    0x3
#define HWIO_MSS_QDSP6SS_CLAMP_STATUS_CLAMP_QMC_MEM_BMSK                                   0x4
#define HWIO_MSS_QDSP6SS_CLAMP_STATUS_CLAMP_QMC_MEM_SHFT                                   0x2
#define HWIO_MSS_QDSP6SS_CLAMP_STATUS_CLAMP_IO_BMSK                                        0x1
#define HWIO_MSS_QDSP6SS_CLAMP_STATUS_CLAMP_IO_SHFT                                        0x0

#define HWIO_MSS_QDSP6SS_CLK_CFG_ADDR                                               (MSS_QDSP6V66SS_PUB_REG_BASE      + 0x00000460)
#define HWIO_MSS_QDSP6SS_CLK_CFG_OFFS                                               (MSS_QDSP6V66SS_PUB_REG_BASE_OFFS + 0x00000460)
#define HWIO_MSS_QDSP6SS_CLK_CFG_RMSK                                                    0x3ff
#define HWIO_MSS_QDSP6SS_CLK_CFG_IN          \
        in_dword_masked(HWIO_MSS_QDSP6SS_CLK_CFG_ADDR, HWIO_MSS_QDSP6SS_CLK_CFG_RMSK)
#define HWIO_MSS_QDSP6SS_CLK_CFG_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_CLK_CFG_ADDR, m)
#define HWIO_MSS_QDSP6SS_CLK_CFG_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_CLK_CFG_ADDR,v)
#define HWIO_MSS_QDSP6SS_CLK_CFG_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_CLK_CFG_ADDR,m,v,HWIO_MSS_QDSP6SS_CLK_CFG_IN)
#define HWIO_MSS_QDSP6SS_CLK_CFG_AXIS2_BMSK                                              0x200
#define HWIO_MSS_QDSP6SS_CLK_CFG_AXIS2_SHFT                                                0x9
#define HWIO_MSS_QDSP6SS_CLK_CFG_CSI1_BMSK                                               0x100
#define HWIO_MSS_QDSP6SS_CLK_CFG_CSI1_SHFT                                                 0x8
#define HWIO_MSS_QDSP6SS_CLK_CFG_CSI0_BMSK                                                0x80
#define HWIO_MSS_QDSP6SS_CLK_CFG_CSI0_SHFT                                                 0x7
#define HWIO_MSS_QDSP6SS_CLK_CFG_AXIM2_BMSK                                               0x40
#define HWIO_MSS_QDSP6SS_CLK_CFG_AXIM2_SHFT                                                0x6
#define HWIO_MSS_QDSP6SS_CLK_CFG_AXIS_BMSK                                                0x20
#define HWIO_MSS_QDSP6SS_CLK_CFG_AXIS_SHFT                                                 0x5
#define HWIO_MSS_QDSP6SS_CLK_CFG_ATBM_BMSK                                                0x10
#define HWIO_MSS_QDSP6SS_CLK_CFG_ATBM_SHFT                                                 0x4
#define HWIO_MSS_QDSP6SS_CLK_CFG_AHBM_BMSK                                                 0x8
#define HWIO_MSS_QDSP6SS_CLK_CFG_AHBM_SHFT                                                 0x3
#define HWIO_MSS_QDSP6SS_CLK_CFG_AXIM_BMSK                                                 0x4
#define HWIO_MSS_QDSP6SS_CLK_CFG_AXIM_SHFT                                                 0x2
#define HWIO_MSS_QDSP6SS_CLK_CFG_APB_BMSK                                                  0x2
#define HWIO_MSS_QDSP6SS_CLK_CFG_APB_SHFT                                                  0x1
#define HWIO_MSS_QDSP6SS_CLK_CFG_CORE_BMSK                                                 0x1
#define HWIO_MSS_QDSP6SS_CLK_CFG_CORE_SHFT                                                 0x0

#define HWIO_MSS_QDSP6SS_CLK_CMD_ADDR                                               (MSS_QDSP6V66SS_PUB_REG_BASE      + 0x00000464)
#define HWIO_MSS_QDSP6SS_CLK_CMD_OFFS                                               (MSS_QDSP6V66SS_PUB_REG_BASE_OFFS + 0x00000464)
#define HWIO_MSS_QDSP6SS_CLK_CMD_RMSK                                                      0x1
#define HWIO_MSS_QDSP6SS_CLK_CMD_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_CLK_CMD_ADDR,v)
#define HWIO_MSS_QDSP6SS_CLK_CMD_UPDATE_CLK_BMSK                                           0x1
#define HWIO_MSS_QDSP6SS_CLK_CMD_UPDATE_CLK_SHFT                                           0x0

#define HWIO_MSS_QDSP6SS_CLK_STATUS_ADDR                                            (MSS_QDSP6V66SS_PUB_REG_BASE      + 0x00000468)
#define HWIO_MSS_QDSP6SS_CLK_STATUS_OFFS                                            (MSS_QDSP6V66SS_PUB_REG_BASE_OFFS + 0x00000468)
#define HWIO_MSS_QDSP6SS_CLK_STATUS_RMSK                                                 0x3ff
#define HWIO_MSS_QDSP6SS_CLK_STATUS_IN          \
        in_dword_masked(HWIO_MSS_QDSP6SS_CLK_STATUS_ADDR, HWIO_MSS_QDSP6SS_CLK_STATUS_RMSK)
#define HWIO_MSS_QDSP6SS_CLK_STATUS_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_CLK_STATUS_ADDR, m)
#define HWIO_MSS_QDSP6SS_CLK_STATUS_AXIS2_BMSK                                           0x200
#define HWIO_MSS_QDSP6SS_CLK_STATUS_AXIS2_SHFT                                             0x9
#define HWIO_MSS_QDSP6SS_CLK_STATUS_CSI1_BMSK                                            0x100
#define HWIO_MSS_QDSP6SS_CLK_STATUS_CSI1_SHFT                                              0x8
#define HWIO_MSS_QDSP6SS_CLK_STATUS_CSI0_BMSK                                             0x80
#define HWIO_MSS_QDSP6SS_CLK_STATUS_CSI0_SHFT                                              0x7
#define HWIO_MSS_QDSP6SS_CLK_STATUS_AXIM2_BMSK                                            0x40
#define HWIO_MSS_QDSP6SS_CLK_STATUS_AXIM2_SHFT                                             0x6
#define HWIO_MSS_QDSP6SS_CLK_STATUS_AXIS_BMSK                                             0x20
#define HWIO_MSS_QDSP6SS_CLK_STATUS_AXIS_SHFT                                              0x5
#define HWIO_MSS_QDSP6SS_CLK_STATUS_ATBM_BMSK                                             0x10
#define HWIO_MSS_QDSP6SS_CLK_STATUS_ATBM_SHFT                                              0x4
#define HWIO_MSS_QDSP6SS_CLK_STATUS_AHBM_BMSK                                              0x8
#define HWIO_MSS_QDSP6SS_CLK_STATUS_AHBM_SHFT                                              0x3
#define HWIO_MSS_QDSP6SS_CLK_STATUS_AXIM_BMSK                                              0x4
#define HWIO_MSS_QDSP6SS_CLK_STATUS_AXIM_SHFT                                              0x2
#define HWIO_MSS_QDSP6SS_CLK_STATUS_APB_BMSK                                               0x2
#define HWIO_MSS_QDSP6SS_CLK_STATUS_APB_SHFT                                               0x1
#define HWIO_MSS_QDSP6SS_CLK_STATUS_CORE_BMSK                                              0x1
#define HWIO_MSS_QDSP6SS_CLK_STATUS_CORE_SHFT                                              0x0

#define HWIO_MSS_QDSP6SS_MEM_STAGGER_RESET_CFG_ADDR                                 (MSS_QDSP6V66SS_PUB_REG_BASE      + 0x00000470)
#define HWIO_MSS_QDSP6SS_MEM_STAGGER_RESET_CFG_OFFS                                 (MSS_QDSP6V66SS_PUB_REG_BASE_OFFS + 0x00000470)
#define HWIO_MSS_QDSP6SS_MEM_STAGGER_RESET_CFG_RMSK                                        0x3
#define HWIO_MSS_QDSP6SS_MEM_STAGGER_RESET_CFG_IN          \
        in_dword_masked(HWIO_MSS_QDSP6SS_MEM_STAGGER_RESET_CFG_ADDR, HWIO_MSS_QDSP6SS_MEM_STAGGER_RESET_CFG_RMSK)
#define HWIO_MSS_QDSP6SS_MEM_STAGGER_RESET_CFG_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_MEM_STAGGER_RESET_CFG_ADDR, m)
#define HWIO_MSS_QDSP6SS_MEM_STAGGER_RESET_CFG_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_MEM_STAGGER_RESET_CFG_ADDR,v)
#define HWIO_MSS_QDSP6SS_MEM_STAGGER_RESET_CFG_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_MEM_STAGGER_RESET_CFG_ADDR,m,v,HWIO_MSS_QDSP6SS_MEM_STAGGER_RESET_CFG_IN)
#define HWIO_MSS_QDSP6SS_MEM_STAGGER_RESET_CFG_STAGGER_ALT_ARES_BMSK                       0x2
#define HWIO_MSS_QDSP6SS_MEM_STAGGER_RESET_CFG_STAGGER_ALT_ARES_SHFT                       0x1
#define HWIO_MSS_QDSP6SS_MEM_STAGGER_RESET_CFG_STAGGER_FUNC_ARES_BMSK                      0x1
#define HWIO_MSS_QDSP6SS_MEM_STAGGER_RESET_CFG_STAGGER_FUNC_ARES_SHFT                      0x0

#define HWIO_MSS_QDSP6SS_MEM_STAGGER_RESET_CMD_ADDR                                 (MSS_QDSP6V66SS_PUB_REG_BASE      + 0x00000474)
#define HWIO_MSS_QDSP6SS_MEM_STAGGER_RESET_CMD_OFFS                                 (MSS_QDSP6V66SS_PUB_REG_BASE_OFFS + 0x00000474)
#define HWIO_MSS_QDSP6SS_MEM_STAGGER_RESET_CMD_RMSK                                        0x1
#define HWIO_MSS_QDSP6SS_MEM_STAGGER_RESET_CMD_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_MEM_STAGGER_RESET_CMD_ADDR,v)
#define HWIO_MSS_QDSP6SS_MEM_STAGGER_RESET_CMD_UPDATE_BMSK                                 0x1
#define HWIO_MSS_QDSP6SS_MEM_STAGGER_RESET_CMD_UPDATE_SHFT                                 0x0

#define HWIO_MSS_QDSP6SS_MEM_STAGGER_RESET_STATUS_ADDR                              (MSS_QDSP6V66SS_PUB_REG_BASE      + 0x00000478)
#define HWIO_MSS_QDSP6SS_MEM_STAGGER_RESET_STATUS_OFFS                              (MSS_QDSP6V66SS_PUB_REG_BASE_OFFS + 0x00000478)
#define HWIO_MSS_QDSP6SS_MEM_STAGGER_RESET_STATUS_RMSK                                     0x3
#define HWIO_MSS_QDSP6SS_MEM_STAGGER_RESET_STATUS_IN          \
        in_dword_masked(HWIO_MSS_QDSP6SS_MEM_STAGGER_RESET_STATUS_ADDR, HWIO_MSS_QDSP6SS_MEM_STAGGER_RESET_STATUS_RMSK)
#define HWIO_MSS_QDSP6SS_MEM_STAGGER_RESET_STATUS_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_MEM_STAGGER_RESET_STATUS_ADDR, m)
#define HWIO_MSS_QDSP6SS_MEM_STAGGER_RESET_STATUS_STAGGER_ALT_ARES_BMSK                    0x2
#define HWIO_MSS_QDSP6SS_MEM_STAGGER_RESET_STATUS_STAGGER_ALT_ARES_SHFT                    0x1
#define HWIO_MSS_QDSP6SS_MEM_STAGGER_RESET_STATUS_STAGGER_FUNC_ARES_BMSK                   0x1
#define HWIO_MSS_QDSP6SS_MEM_STAGGER_RESET_STATUS_STAGGER_FUNC_ARES_SHFT                   0x0

#define HWIO_MSS_QDSP6SS_MEM_STAGGER_RESTORE_CMD_ADDR                               (MSS_QDSP6V66SS_PUB_REG_BASE      + 0x00000484)
#define HWIO_MSS_QDSP6SS_MEM_STAGGER_RESTORE_CMD_OFFS                               (MSS_QDSP6V66SS_PUB_REG_BASE_OFFS + 0x00000484)
#define HWIO_MSS_QDSP6SS_MEM_STAGGER_RESTORE_CMD_RMSK                                      0x1
#define HWIO_MSS_QDSP6SS_MEM_STAGGER_RESTORE_CMD_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_MEM_STAGGER_RESTORE_CMD_ADDR,v)
#define HWIO_MSS_QDSP6SS_MEM_STAGGER_RESTORE_CMD_RESTORE_CMD_BMSK                          0x1
#define HWIO_MSS_QDSP6SS_MEM_STAGGER_RESTORE_CMD_RESTORE_CMD_SHFT                          0x0

#define HWIO_MSS_QDSP6SS_MEM_STAGGER_RESTORE_STATUS_ADDR                            (MSS_QDSP6V66SS_PUB_REG_BASE      + 0x00000488)
#define HWIO_MSS_QDSP6SS_MEM_STAGGER_RESTORE_STATUS_OFFS                            (MSS_QDSP6V66SS_PUB_REG_BASE_OFFS + 0x00000488)
#define HWIO_MSS_QDSP6SS_MEM_STAGGER_RESTORE_STATUS_RMSK                                   0x1
#define HWIO_MSS_QDSP6SS_MEM_STAGGER_RESTORE_STATUS_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_MEM_STAGGER_RESTORE_STATUS_ADDR,v)
#define HWIO_MSS_QDSP6SS_MEM_STAGGER_RESTORE_STATUS_RESTORE_BUSY_BMSK                      0x1
#define HWIO_MSS_QDSP6SS_MEM_STAGGER_RESTORE_STATUS_RESTORE_BUSY_SHFT                      0x0

#define HWIO_MSS_QDSP6SS_CP_PWR_CTL_ADDR                                            (MSS_QDSP6V66SS_PUB_REG_BASE      + 0x00000500)
#define HWIO_MSS_QDSP6SS_CP_PWR_CTL_OFFS                                            (MSS_QDSP6V66SS_PUB_REG_BASE_OFFS + 0x00000500)
#define HWIO_MSS_QDSP6SS_CP_PWR_CTL_RMSK                                                  0x1d
#define HWIO_MSS_QDSP6SS_CP_PWR_CTL_IN          \
        in_dword_masked(HWIO_MSS_QDSP6SS_CP_PWR_CTL_ADDR, HWIO_MSS_QDSP6SS_CP_PWR_CTL_RMSK)
#define HWIO_MSS_QDSP6SS_CP_PWR_CTL_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_CP_PWR_CTL_ADDR, m)
#define HWIO_MSS_QDSP6SS_CP_PWR_CTL_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_CP_PWR_CTL_ADDR,v)
#define HWIO_MSS_QDSP6SS_CP_PWR_CTL_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_CP_PWR_CTL_ADDR,m,v,HWIO_MSS_QDSP6SS_CP_PWR_CTL_IN)
#define HWIO_MSS_QDSP6SS_CP_PWR_CTL_CLAMP_QMC_MEM_BMSK                                    0x10
#define HWIO_MSS_QDSP6SS_CP_PWR_CTL_CLAMP_QMC_MEM_SHFT                                     0x4
#define HWIO_MSS_QDSP6SS_CP_PWR_CTL_LDO_2PASS_ON_BMSK                                      0x8
#define HWIO_MSS_QDSP6SS_CP_PWR_CTL_LDO_2PASS_ON_SHFT                                      0x3
#define HWIO_MSS_QDSP6SS_CP_PWR_CTL_SHUNT_BHS_ON_BMSK                                      0x4
#define HWIO_MSS_QDSP6SS_CP_PWR_CTL_SHUNT_BHS_ON_SHFT                                      0x2
#define HWIO_MSS_QDSP6SS_CP_PWR_CTL_CLAMP_IO_BMSK                                          0x1
#define HWIO_MSS_QDSP6SS_CP_PWR_CTL_CLAMP_IO_SHFT                                          0x0

#define HWIO_MSS_QDSP6SS_CP_RESET_ADDR                                              (MSS_QDSP6V66SS_PUB_REG_BASE      + 0x00000504)
#define HWIO_MSS_QDSP6SS_CP_RESET_OFFS                                              (MSS_QDSP6V66SS_PUB_REG_BASE_OFFS + 0x00000504)
#define HWIO_MSS_QDSP6SS_CP_RESET_RMSK                                                     0x1
#define HWIO_MSS_QDSP6SS_CP_RESET_IN          \
        in_dword_masked(HWIO_MSS_QDSP6SS_CP_RESET_ADDR, HWIO_MSS_QDSP6SS_CP_RESET_RMSK)
#define HWIO_MSS_QDSP6SS_CP_RESET_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_CP_RESET_ADDR, m)
#define HWIO_MSS_QDSP6SS_CP_RESET_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_CP_RESET_ADDR,v)
#define HWIO_MSS_QDSP6SS_CP_RESET_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_CP_RESET_ADDR,m,v,HWIO_MSS_QDSP6SS_CP_RESET_IN)
#define HWIO_MSS_QDSP6SS_CP_RESET_ARES_BMSK                                                0x1
#define HWIO_MSS_QDSP6SS_CP_RESET_ARES_SHFT                                                0x0

#define HWIO_MSS_QDSP6SS_CP_CLK_CTL_ADDR                                            (MSS_QDSP6V66SS_PUB_REG_BASE      + 0x00000508)
#define HWIO_MSS_QDSP6SS_CP_CLK_CTL_OFFS                                            (MSS_QDSP6V66SS_PUB_REG_BASE_OFFS + 0x00000508)
#define HWIO_MSS_QDSP6SS_CP_CLK_CTL_RMSK                                                   0x1
#define HWIO_MSS_QDSP6SS_CP_CLK_CTL_IN          \
        in_dword_masked(HWIO_MSS_QDSP6SS_CP_CLK_CTL_ADDR, HWIO_MSS_QDSP6SS_CP_CLK_CTL_RMSK)
#define HWIO_MSS_QDSP6SS_CP_CLK_CTL_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_CP_CLK_CTL_ADDR, m)
#define HWIO_MSS_QDSP6SS_CP_CLK_CTL_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_CP_CLK_CTL_ADDR,v)
#define HWIO_MSS_QDSP6SS_CP_CLK_CTL_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_CP_CLK_CTL_ADDR,m,v,HWIO_MSS_QDSP6SS_CP_CLK_CTL_IN)
#define HWIO_MSS_QDSP6SS_CP_CLK_CTL_CLK_ENA_BMSK                                           0x1
#define HWIO_MSS_QDSP6SS_CP_CLK_CTL_CLK_ENA_SHFT                                           0x0

#define HWIO_MSS_QDSP6SS_CP_BHS_STATUS_ADDR                                         (MSS_QDSP6V66SS_PUB_REG_BASE      + 0x00000518)
#define HWIO_MSS_QDSP6SS_CP_BHS_STATUS_OFFS                                         (MSS_QDSP6V66SS_PUB_REG_BASE_OFFS + 0x00000518)
#define HWIO_MSS_QDSP6SS_CP_BHS_STATUS_RMSK                                                0x1
#define HWIO_MSS_QDSP6SS_CP_BHS_STATUS_IN          \
        in_dword_masked(HWIO_MSS_QDSP6SS_CP_BHS_STATUS_ADDR, HWIO_MSS_QDSP6SS_CP_BHS_STATUS_RMSK)
#define HWIO_MSS_QDSP6SS_CP_BHS_STATUS_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_CP_BHS_STATUS_ADDR, m)
#define HWIO_MSS_QDSP6SS_CP_BHS_STATUS_BHS_ON_BMSK                                         0x1
#define HWIO_MSS_QDSP6SS_CP_BHS_STATUS_BHS_ON_SHFT                                         0x0

#define HWIO_MSS_QDSP6SS_CPMEM_CFG_ADDR                                             (MSS_QDSP6V66SS_PUB_REG_BASE      + 0x00000520)
#define HWIO_MSS_QDSP6SS_CPMEM_CFG_OFFS                                             (MSS_QDSP6V66SS_PUB_REG_BASE_OFFS + 0x00000520)
#define HWIO_MSS_QDSP6SS_CPMEM_CFG_RMSK                                                    0x3
#define HWIO_MSS_QDSP6SS_CPMEM_CFG_IN          \
        in_dword_masked(HWIO_MSS_QDSP6SS_CPMEM_CFG_ADDR, HWIO_MSS_QDSP6SS_CPMEM_CFG_RMSK)
#define HWIO_MSS_QDSP6SS_CPMEM_CFG_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_CPMEM_CFG_ADDR, m)
#define HWIO_MSS_QDSP6SS_CPMEM_CFG_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_CPMEM_CFG_ADDR,v)
#define HWIO_MSS_QDSP6SS_CPMEM_CFG_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_CPMEM_CFG_ADDR,m,v,HWIO_MSS_QDSP6SS_CPMEM_CFG_IN)
#define HWIO_MSS_QDSP6SS_CPMEM_CFG_VTCM_SLP_RET_N_BMSK                                     0x2
#define HWIO_MSS_QDSP6SS_CPMEM_CFG_VTCM_SLP_RET_N_SHFT                                     0x1
#define HWIO_MSS_QDSP6SS_CPMEM_CFG_VTCM_SLP_NRET_N_BMSK                                    0x1
#define HWIO_MSS_QDSP6SS_CPMEM_CFG_VTCM_SLP_NRET_N_SHFT                                    0x0

#define HWIO_MSS_QDSP6SS_CPMEM_CMD_ADDR                                             (MSS_QDSP6V66SS_PUB_REG_BASE      + 0x00000524)
#define HWIO_MSS_QDSP6SS_CPMEM_CMD_OFFS                                             (MSS_QDSP6V66SS_PUB_REG_BASE_OFFS + 0x00000524)
#define HWIO_MSS_QDSP6SS_CPMEM_CMD_RMSK                                                    0x3
#define HWIO_MSS_QDSP6SS_CPMEM_CMD_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_CPMEM_CMD_ADDR,v)
#define HWIO_MSS_QDSP6SS_CPMEM_CMD_UPDATE_VTCM_SLP_RET_N_BMSK                              0x2
#define HWIO_MSS_QDSP6SS_CPMEM_CMD_UPDATE_VTCM_SLP_RET_N_SHFT                              0x1
#define HWIO_MSS_QDSP6SS_CPMEM_CMD_UPDATE_VTCM_SLP_NRET_N_BMSK                             0x1
#define HWIO_MSS_QDSP6SS_CPMEM_CMD_UPDATE_VTCM_SLP_NRET_N_SHFT                             0x0

#define HWIO_MSS_QDSP6SS_CPMEM_STATUS_ADDR                                          (MSS_QDSP6V66SS_PUB_REG_BASE      + 0x00000528)
#define HWIO_MSS_QDSP6SS_CPMEM_STATUS_OFFS                                          (MSS_QDSP6V66SS_PUB_REG_BASE_OFFS + 0x00000528)
#define HWIO_MSS_QDSP6SS_CPMEM_STATUS_RMSK                                                 0x3
#define HWIO_MSS_QDSP6SS_CPMEM_STATUS_IN          \
        in_dword_masked(HWIO_MSS_QDSP6SS_CPMEM_STATUS_ADDR, HWIO_MSS_QDSP6SS_CPMEM_STATUS_RMSK)
#define HWIO_MSS_QDSP6SS_CPMEM_STATUS_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_CPMEM_STATUS_ADDR, m)
#define HWIO_MSS_QDSP6SS_CPMEM_STATUS_VTCM_SLP_RET_N_BMSK                                  0x2
#define HWIO_MSS_QDSP6SS_CPMEM_STATUS_VTCM_SLP_RET_N_SHFT                                  0x1
#define HWIO_MSS_QDSP6SS_CPMEM_STATUS_VTCM_SLP_NRET_N_BMSK                                 0x1
#define HWIO_MSS_QDSP6SS_CPMEM_STATUS_VTCM_SLP_NRET_N_SHFT                                 0x0

#define HWIO_MSS_QDSP6SS_L1SMEM_CFG_ADDR                                            (MSS_QDSP6V66SS_PUB_REG_BASE      + 0x00000530)
#define HWIO_MSS_QDSP6SS_L1SMEM_CFG_OFFS                                            (MSS_QDSP6V66SS_PUB_REG_BASE_OFFS + 0x00000530)
#define HWIO_MSS_QDSP6SS_L1SMEM_CFG_RMSK                                              0x3f003f
#define HWIO_MSS_QDSP6SS_L1SMEM_CFG_IN          \
        in_dword_masked(HWIO_MSS_QDSP6SS_L1SMEM_CFG_ADDR, HWIO_MSS_QDSP6SS_L1SMEM_CFG_RMSK)
#define HWIO_MSS_QDSP6SS_L1SMEM_CFG_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_L1SMEM_CFG_ADDR, m)
#define HWIO_MSS_QDSP6SS_L1SMEM_CFG_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_L1SMEM_CFG_ADDR,v)
#define HWIO_MSS_QDSP6SS_L1SMEM_CFG_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_L1SMEM_CFG_ADDR,m,v,HWIO_MSS_QDSP6SS_L1SMEM_CFG_IN)
#define HWIO_MSS_QDSP6SS_L1SMEM_CFG_L1S_SLP_RET_N_BMSK                                0x3f0000
#define HWIO_MSS_QDSP6SS_L1SMEM_CFG_L1S_SLP_RET_N_SHFT                                    0x10
#define HWIO_MSS_QDSP6SS_L1SMEM_CFG_L1S_SLP_NRET_N_BMSK                                   0x3f
#define HWIO_MSS_QDSP6SS_L1SMEM_CFG_L1S_SLP_NRET_N_SHFT                                    0x0

#define HWIO_MSS_QDSP6SS_L1SMEM_CMD_ADDR                                            (MSS_QDSP6V66SS_PUB_REG_BASE      + 0x00000534)
#define HWIO_MSS_QDSP6SS_L1SMEM_CMD_OFFS                                            (MSS_QDSP6V66SS_PUB_REG_BASE_OFFS + 0x00000534)
#define HWIO_MSS_QDSP6SS_L1SMEM_CMD_RMSK                                                   0x3
#define HWIO_MSS_QDSP6SS_L1SMEM_CMD_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_L1SMEM_CMD_ADDR,v)
#define HWIO_MSS_QDSP6SS_L1SMEM_CMD_UPDATE_L1S_SLP_RET_N_BMSK                              0x2
#define HWIO_MSS_QDSP6SS_L1SMEM_CMD_UPDATE_L1S_SLP_RET_N_SHFT                              0x1
#define HWIO_MSS_QDSP6SS_L1SMEM_CMD_UPDATE_L1S_SLP_NRET_N_BMSK                             0x1
#define HWIO_MSS_QDSP6SS_L1SMEM_CMD_UPDATE_L1S_SLP_NRET_N_SHFT                             0x0

#define HWIO_MSS_QDSP6SS_L1SMEM_STATUS_ADDR                                         (MSS_QDSP6V66SS_PUB_REG_BASE      + 0x00000538)
#define HWIO_MSS_QDSP6SS_L1SMEM_STATUS_OFFS                                         (MSS_QDSP6V66SS_PUB_REG_BASE_OFFS + 0x00000538)
#define HWIO_MSS_QDSP6SS_L1SMEM_STATUS_RMSK                                           0x3f003f
#define HWIO_MSS_QDSP6SS_L1SMEM_STATUS_IN          \
        in_dword_masked(HWIO_MSS_QDSP6SS_L1SMEM_STATUS_ADDR, HWIO_MSS_QDSP6SS_L1SMEM_STATUS_RMSK)
#define HWIO_MSS_QDSP6SS_L1SMEM_STATUS_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_L1SMEM_STATUS_ADDR, m)
#define HWIO_MSS_QDSP6SS_L1SMEM_STATUS_L1S_SLP_RET_N_BMSK                             0x3f0000
#define HWIO_MSS_QDSP6SS_L1SMEM_STATUS_L1S_SLP_RET_N_SHFT                                 0x10
#define HWIO_MSS_QDSP6SS_L1SMEM_STATUS_L1S_SLP_NRET_N_BMSK                                0x3f
#define HWIO_MSS_QDSP6SS_L1SMEM_STATUS_L1S_SLP_NRET_N_SHFT                                 0x0

#define HWIO_MSS_QDSP6SS_VU0_PWR_VOTE_SET_ADDR                                      (MSS_QDSP6V66SS_PUB_REG_BASE      + 0x00000600)
#define HWIO_MSS_QDSP6SS_VU0_PWR_VOTE_SET_OFFS                                      (MSS_QDSP6V66SS_PUB_REG_BASE_OFFS + 0x00000600)
#define HWIO_MSS_QDSP6SS_VU0_PWR_VOTE_SET_RMSK                                            0xff
#define HWIO_MSS_QDSP6SS_VU0_PWR_VOTE_SET_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_VU0_PWR_VOTE_SET_ADDR,v)
#define HWIO_MSS_QDSP6SS_VU0_PWR_VOTE_SET_SET_BMSK                                        0xff
#define HWIO_MSS_QDSP6SS_VU0_PWR_VOTE_SET_SET_SHFT                                         0x0

#define HWIO_MSS_QDSP6SS_VU0_PWR_VOTE_CLR_ADDR                                      (MSS_QDSP6V66SS_PUB_REG_BASE      + 0x00000604)
#define HWIO_MSS_QDSP6SS_VU0_PWR_VOTE_CLR_OFFS                                      (MSS_QDSP6V66SS_PUB_REG_BASE_OFFS + 0x00000604)
#define HWIO_MSS_QDSP6SS_VU0_PWR_VOTE_CLR_RMSK                                           0xfff
#define HWIO_MSS_QDSP6SS_VU0_PWR_VOTE_CLR_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_VU0_PWR_VOTE_CLR_ADDR,v)
#define HWIO_MSS_QDSP6SS_VU0_PWR_VOTE_CLR_CLR_BMSK                                       0xfff
#define HWIO_MSS_QDSP6SS_VU0_PWR_VOTE_CLR_CLR_SHFT                                         0x0

#define HWIO_MSS_QDSP6SS_VU0_PWR_VOTE_MASK_ADDR                                     (MSS_QDSP6V66SS_PUB_REG_BASE      + 0x00000608)
#define HWIO_MSS_QDSP6SS_VU0_PWR_VOTE_MASK_OFFS                                     (MSS_QDSP6V66SS_PUB_REG_BASE_OFFS + 0x00000608)
#define HWIO_MSS_QDSP6SS_VU0_PWR_VOTE_MASK_RMSK                                          0xfff
#define HWIO_MSS_QDSP6SS_VU0_PWR_VOTE_MASK_IN          \
        in_dword_masked(HWIO_MSS_QDSP6SS_VU0_PWR_VOTE_MASK_ADDR, HWIO_MSS_QDSP6SS_VU0_PWR_VOTE_MASK_RMSK)
#define HWIO_MSS_QDSP6SS_VU0_PWR_VOTE_MASK_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_VU0_PWR_VOTE_MASK_ADDR, m)
#define HWIO_MSS_QDSP6SS_VU0_PWR_VOTE_MASK_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_VU0_PWR_VOTE_MASK_ADDR,v)
#define HWIO_MSS_QDSP6SS_VU0_PWR_VOTE_MASK_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_VU0_PWR_VOTE_MASK_ADDR,m,v,HWIO_MSS_QDSP6SS_VU0_PWR_VOTE_MASK_IN)
#define HWIO_MSS_QDSP6SS_VU0_PWR_VOTE_MASK_HW_VOTE_EN_BMSK                               0xf00
#define HWIO_MSS_QDSP6SS_VU0_PWR_VOTE_MASK_HW_VOTE_EN_SHFT                                 0x8
#define HWIO_MSS_QDSP6SS_VU0_PWR_VOTE_MASK_VOTE_EN_BMSK                                   0xff
#define HWIO_MSS_QDSP6SS_VU0_PWR_VOTE_MASK_VOTE_EN_SHFT                                    0x0

#define HWIO_MSS_QDSP6SS_VU0_PWR_STATUS_ADDR                                        (MSS_QDSP6V66SS_PUB_REG_BASE      + 0x0000060c)
#define HWIO_MSS_QDSP6SS_VU0_PWR_STATUS_OFFS                                        (MSS_QDSP6V66SS_PUB_REG_BASE_OFFS + 0x0000060c)
#define HWIO_MSS_QDSP6SS_VU0_PWR_STATUS_RMSK                                           0xfff1f
#define HWIO_MSS_QDSP6SS_VU0_PWR_STATUS_IN          \
        in_dword_masked(HWIO_MSS_QDSP6SS_VU0_PWR_STATUS_ADDR, HWIO_MSS_QDSP6SS_VU0_PWR_STATUS_RMSK)
#define HWIO_MSS_QDSP6SS_VU0_PWR_STATUS_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_VU0_PWR_STATUS_ADDR, m)
#define HWIO_MSS_QDSP6SS_VU0_PWR_STATUS_VOTE_STAT_BMSK                                 0xfff00
#define HWIO_MSS_QDSP6SS_VU0_PWR_STATUS_VOTE_STAT_SHFT                                     0x8
#define HWIO_MSS_QDSP6SS_VU0_PWR_STATUS_WAIT_NEXT_BMSK                                    0x10
#define HWIO_MSS_QDSP6SS_VU0_PWR_STATUS_WAIT_NEXT_SHFT                                     0x4
#define HWIO_MSS_QDSP6SS_VU0_PWR_STATUS_WAIT_PREV_BMSK                                     0x8
#define HWIO_MSS_QDSP6SS_VU0_PWR_STATUS_WAIT_PREV_SHFT                                     0x3
#define HWIO_MSS_QDSP6SS_VU0_PWR_STATUS_PWR_DOWN_BMSK                                      0x4
#define HWIO_MSS_QDSP6SS_VU0_PWR_STATUS_PWR_DOWN_SHFT                                      0x2
#define HWIO_MSS_QDSP6SS_VU0_PWR_STATUS_PWR_REQ_BMSK                                       0x2
#define HWIO_MSS_QDSP6SS_VU0_PWR_STATUS_PWR_REQ_SHFT                                       0x1
#define HWIO_MSS_QDSP6SS_VU0_PWR_STATUS_PWR_ON_BMSK                                        0x1
#define HWIO_MSS_QDSP6SS_VU0_PWR_STATUS_PWR_ON_SHFT                                        0x0

#define HWIO_MSS_QDSP6SS_VU0_PWR_DEBUG_ADDR                                         (MSS_QDSP6V66SS_PUB_REG_BASE      + 0x00000610)
#define HWIO_MSS_QDSP6SS_VU0_PWR_DEBUG_OFFS                                         (MSS_QDSP6V66SS_PUB_REG_BASE_OFFS + 0x00000610)
#define HWIO_MSS_QDSP6SS_VU0_PWR_DEBUG_RMSK                                               0xf7
#define HWIO_MSS_QDSP6SS_VU0_PWR_DEBUG_IN          \
        in_dword_masked(HWIO_MSS_QDSP6SS_VU0_PWR_DEBUG_ADDR, HWIO_MSS_QDSP6SS_VU0_PWR_DEBUG_RMSK)
#define HWIO_MSS_QDSP6SS_VU0_PWR_DEBUG_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_VU0_PWR_DEBUG_ADDR, m)
#define HWIO_MSS_QDSP6SS_VU0_PWR_DEBUG_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_VU0_PWR_DEBUG_ADDR,v)
#define HWIO_MSS_QDSP6SS_VU0_PWR_DEBUG_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_VU0_PWR_DEBUG_ADDR,m,v,HWIO_MSS_QDSP6SS_VU0_PWR_DEBUG_IN)
#define HWIO_MSS_QDSP6SS_VU0_PWR_DEBUG_SEQ_DELAY_BMSK                                     0xf0
#define HWIO_MSS_QDSP6SS_VU0_PWR_DEBUG_SEQ_DELAY_SHFT                                      0x4
#define HWIO_MSS_QDSP6SS_VU0_PWR_DEBUG_CLAMP_IO_BMSK                                       0x4
#define HWIO_MSS_QDSP6SS_VU0_PWR_DEBUG_CLAMP_IO_SHFT                                       0x2
#define HWIO_MSS_QDSP6SS_VU0_PWR_DEBUG_CP_ARES_BMSK                                        0x2
#define HWIO_MSS_QDSP6SS_VU0_PWR_DEBUG_CP_ARES_SHFT                                        0x1
#define HWIO_MSS_QDSP6SS_VU0_PWR_DEBUG_CP_CLK_EN_BMSK                                      0x1
#define HWIO_MSS_QDSP6SS_VU0_PWR_DEBUG_CP_CLK_EN_SHFT                                      0x0

#define HWIO_MSS_QDSP6SS_VU1_PWR_VOTE_SET_ADDR                                      (MSS_QDSP6V66SS_PUB_REG_BASE      + 0x00000620)
#define HWIO_MSS_QDSP6SS_VU1_PWR_VOTE_SET_OFFS                                      (MSS_QDSP6V66SS_PUB_REG_BASE_OFFS + 0x00000620)
#define HWIO_MSS_QDSP6SS_VU1_PWR_VOTE_SET_RMSK                                            0xff
#define HWIO_MSS_QDSP6SS_VU1_PWR_VOTE_SET_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_VU1_PWR_VOTE_SET_ADDR,v)
#define HWIO_MSS_QDSP6SS_VU1_PWR_VOTE_SET_SET_BMSK                                        0xff
#define HWIO_MSS_QDSP6SS_VU1_PWR_VOTE_SET_SET_SHFT                                         0x0

#define HWIO_MSS_QDSP6SS_VU1_PWR_VOTE_CLR_ADDR                                      (MSS_QDSP6V66SS_PUB_REG_BASE      + 0x00000624)
#define HWIO_MSS_QDSP6SS_VU1_PWR_VOTE_CLR_OFFS                                      (MSS_QDSP6V66SS_PUB_REG_BASE_OFFS + 0x00000624)
#define HWIO_MSS_QDSP6SS_VU1_PWR_VOTE_CLR_RMSK                                           0xfff
#define HWIO_MSS_QDSP6SS_VU1_PWR_VOTE_CLR_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_VU1_PWR_VOTE_CLR_ADDR,v)
#define HWIO_MSS_QDSP6SS_VU1_PWR_VOTE_CLR_CLR_BMSK                                       0xfff
#define HWIO_MSS_QDSP6SS_VU1_PWR_VOTE_CLR_CLR_SHFT                                         0x0

#define HWIO_MSS_QDSP6SS_VU1_PWR_VOTE_MASK_ADDR                                     (MSS_QDSP6V66SS_PUB_REG_BASE      + 0x00000628)
#define HWIO_MSS_QDSP6SS_VU1_PWR_VOTE_MASK_OFFS                                     (MSS_QDSP6V66SS_PUB_REG_BASE_OFFS + 0x00000628)
#define HWIO_MSS_QDSP6SS_VU1_PWR_VOTE_MASK_RMSK                                          0xfff
#define HWIO_MSS_QDSP6SS_VU1_PWR_VOTE_MASK_IN          \
        in_dword_masked(HWIO_MSS_QDSP6SS_VU1_PWR_VOTE_MASK_ADDR, HWIO_MSS_QDSP6SS_VU1_PWR_VOTE_MASK_RMSK)
#define HWIO_MSS_QDSP6SS_VU1_PWR_VOTE_MASK_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_VU1_PWR_VOTE_MASK_ADDR, m)
#define HWIO_MSS_QDSP6SS_VU1_PWR_VOTE_MASK_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_VU1_PWR_VOTE_MASK_ADDR,v)
#define HWIO_MSS_QDSP6SS_VU1_PWR_VOTE_MASK_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_VU1_PWR_VOTE_MASK_ADDR,m,v,HWIO_MSS_QDSP6SS_VU1_PWR_VOTE_MASK_IN)
#define HWIO_MSS_QDSP6SS_VU1_PWR_VOTE_MASK_HW_VOTE_EN_BMSK                               0xf00
#define HWIO_MSS_QDSP6SS_VU1_PWR_VOTE_MASK_HW_VOTE_EN_SHFT                                 0x8
#define HWIO_MSS_QDSP6SS_VU1_PWR_VOTE_MASK_VOTE_EN_BMSK                                   0xff
#define HWIO_MSS_QDSP6SS_VU1_PWR_VOTE_MASK_VOTE_EN_SHFT                                    0x0

#define HWIO_MSS_QDSP6SS_VU1_PWR_STATUS_ADDR                                        (MSS_QDSP6V66SS_PUB_REG_BASE      + 0x0000062c)
#define HWIO_MSS_QDSP6SS_VU1_PWR_STATUS_OFFS                                        (MSS_QDSP6V66SS_PUB_REG_BASE_OFFS + 0x0000062c)
#define HWIO_MSS_QDSP6SS_VU1_PWR_STATUS_RMSK                                           0xfff1f
#define HWIO_MSS_QDSP6SS_VU1_PWR_STATUS_IN          \
        in_dword_masked(HWIO_MSS_QDSP6SS_VU1_PWR_STATUS_ADDR, HWIO_MSS_QDSP6SS_VU1_PWR_STATUS_RMSK)
#define HWIO_MSS_QDSP6SS_VU1_PWR_STATUS_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_VU1_PWR_STATUS_ADDR, m)
#define HWIO_MSS_QDSP6SS_VU1_PWR_STATUS_VOTE_STAT_BMSK                                 0xfff00
#define HWIO_MSS_QDSP6SS_VU1_PWR_STATUS_VOTE_STAT_SHFT                                     0x8
#define HWIO_MSS_QDSP6SS_VU1_PWR_STATUS_WAIT_NEXT_BMSK                                    0x10
#define HWIO_MSS_QDSP6SS_VU1_PWR_STATUS_WAIT_NEXT_SHFT                                     0x4
#define HWIO_MSS_QDSP6SS_VU1_PWR_STATUS_WAIT_PREV_BMSK                                     0x8
#define HWIO_MSS_QDSP6SS_VU1_PWR_STATUS_WAIT_PREV_SHFT                                     0x3
#define HWIO_MSS_QDSP6SS_VU1_PWR_STATUS_PWR_DOWN_BMSK                                      0x4
#define HWIO_MSS_QDSP6SS_VU1_PWR_STATUS_PWR_DOWN_SHFT                                      0x2
#define HWIO_MSS_QDSP6SS_VU1_PWR_STATUS_PWR_REQ_BMSK                                       0x2
#define HWIO_MSS_QDSP6SS_VU1_PWR_STATUS_PWR_REQ_SHFT                                       0x1
#define HWIO_MSS_QDSP6SS_VU1_PWR_STATUS_PWR_ON_BMSK                                        0x1
#define HWIO_MSS_QDSP6SS_VU1_PWR_STATUS_PWR_ON_SHFT                                        0x0

#define HWIO_MSS_QDSP6SS_VU1_PWR_DEBUG_ADDR                                         (MSS_QDSP6V66SS_PUB_REG_BASE      + 0x00000630)
#define HWIO_MSS_QDSP6SS_VU1_PWR_DEBUG_OFFS                                         (MSS_QDSP6V66SS_PUB_REG_BASE_OFFS + 0x00000630)
#define HWIO_MSS_QDSP6SS_VU1_PWR_DEBUG_RMSK                                               0xf7
#define HWIO_MSS_QDSP6SS_VU1_PWR_DEBUG_IN          \
        in_dword_masked(HWIO_MSS_QDSP6SS_VU1_PWR_DEBUG_ADDR, HWIO_MSS_QDSP6SS_VU1_PWR_DEBUG_RMSK)
#define HWIO_MSS_QDSP6SS_VU1_PWR_DEBUG_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_VU1_PWR_DEBUG_ADDR, m)
#define HWIO_MSS_QDSP6SS_VU1_PWR_DEBUG_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_VU1_PWR_DEBUG_ADDR,v)
#define HWIO_MSS_QDSP6SS_VU1_PWR_DEBUG_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_VU1_PWR_DEBUG_ADDR,m,v,HWIO_MSS_QDSP6SS_VU1_PWR_DEBUG_IN)
#define HWIO_MSS_QDSP6SS_VU1_PWR_DEBUG_SEQ_DELAY_BMSK                                     0xf0
#define HWIO_MSS_QDSP6SS_VU1_PWR_DEBUG_SEQ_DELAY_SHFT                                      0x4
#define HWIO_MSS_QDSP6SS_VU1_PWR_DEBUG_CLAMP_IO_BMSK                                       0x4
#define HWIO_MSS_QDSP6SS_VU1_PWR_DEBUG_CLAMP_IO_SHFT                                       0x2
#define HWIO_MSS_QDSP6SS_VU1_PWR_DEBUG_CP_ARES_BMSK                                        0x2
#define HWIO_MSS_QDSP6SS_VU1_PWR_DEBUG_CP_ARES_SHFT                                        0x1
#define HWIO_MSS_QDSP6SS_VU1_PWR_DEBUG_CP_CLK_EN_BMSK                                      0x1
#define HWIO_MSS_QDSP6SS_VU1_PWR_DEBUG_CP_CLK_EN_SHFT                                      0x0

#define HWIO_MSS_QDSP6SS_VU_QOS_ADDR                                                (MSS_QDSP6V66SS_PUB_REG_BASE      + 0x00000640)
#define HWIO_MSS_QDSP6SS_VU_QOS_OFFS                                                (MSS_QDSP6V66SS_PUB_REG_BASE_OFFS + 0x00000640)
#define HWIO_MSS_QDSP6SS_VU_QOS_RMSK                                                      0x77
#define HWIO_MSS_QDSP6SS_VU_QOS_IN          \
        in_dword_masked(HWIO_MSS_QDSP6SS_VU_QOS_ADDR, HWIO_MSS_QDSP6SS_VU_QOS_RMSK)
#define HWIO_MSS_QDSP6SS_VU_QOS_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_VU_QOS_ADDR, m)
#define HWIO_MSS_QDSP6SS_VU_QOS_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_VU_QOS_ADDR,v)
#define HWIO_MSS_QDSP6SS_VU_QOS_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_VU_QOS_ADDR,m,v,HWIO_MSS_QDSP6SS_VU_QOS_IN)
#define HWIO_MSS_QDSP6SS_VU_QOS_QOS_AXIS_BMSK                                             0x70
#define HWIO_MSS_QDSP6SS_VU_QOS_QOS_AXIS_SHFT                                              0x4
#define HWIO_MSS_QDSP6SS_VU_QOS_QOS_MNOC_BMSK                                              0x7
#define HWIO_MSS_QDSP6SS_VU_QOS_QOS_MNOC_SHFT                                              0x0

#define HWIO_MSS_QDSP6SS_LDO_USER_ADDR                                              (MSS_QDSP6V66SS_PUB_REG_BASE      + 0x00000700)
#define HWIO_MSS_QDSP6SS_LDO_USER_OFFS                                              (MSS_QDSP6V66SS_PUB_REG_BASE_OFFS + 0x00000700)
#define HWIO_MSS_QDSP6SS_LDO_USER_RMSK                                              0xffffffff
#define HWIO_MSS_QDSP6SS_LDO_USER_IN          \
        in_dword_masked(HWIO_MSS_QDSP6SS_LDO_USER_ADDR, HWIO_MSS_QDSP6SS_LDO_USER_RMSK)
#define HWIO_MSS_QDSP6SS_LDO_USER_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_LDO_USER_ADDR, m)
#define HWIO_MSS_QDSP6SS_LDO_USER_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_LDO_USER_ADDR,v)
#define HWIO_MSS_QDSP6SS_LDO_USER_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_LDO_USER_ADDR,m,v,HWIO_MSS_QDSP6SS_LDO_USER_IN)
#define HWIO_MSS_QDSP6SS_LDO_USER_REF_TC_CTL_BMSK                                   0xe0000000
#define HWIO_MSS_QDSP6SS_LDO_USER_REF_TC_CTL_SHFT                                         0x1d
#define HWIO_MSS_QDSP6SS_LDO_USER_EN_VDDMIN_AMP_BMSK                                0x10000000
#define HWIO_MSS_QDSP6SS_LDO_USER_EN_VDDMIN_AMP_SHFT                                      0x1c
#define HWIO_MSS_QDSP6SS_LDO_USER_EN_HDRM_AMP_BMSK                                   0x8000000
#define HWIO_MSS_QDSP6SS_LDO_USER_EN_HDRM_AMP_SHFT                                        0x1b
#define HWIO_MSS_QDSP6SS_LDO_USER_FB_RES3_BMSK                                       0x7000000
#define HWIO_MSS_QDSP6SS_LDO_USER_FB_RES3_SHFT                                            0x18
#define HWIO_MSS_QDSP6SS_LDO_USER_FB_RES2_BMSK                                        0xe00000
#define HWIO_MSS_QDSP6SS_LDO_USER_FB_RES2_SHFT                                            0x15
#define HWIO_MSS_QDSP6SS_LDO_USER_FB_RES1_BMSK                                        0x1c0000
#define HWIO_MSS_QDSP6SS_LDO_USER_FB_RES1_SHFT                                            0x12
#define HWIO_MSS_QDSP6SS_LDO_USER_VAR_RES_SEL_BMSK                                     0x3c000
#define HWIO_MSS_QDSP6SS_LDO_USER_VAR_RES_SEL_SHFT                                         0xe
#define HWIO_MSS_QDSP6SS_LDO_USER_FB_STRNGTH2_BMSK                                      0x3800
#define HWIO_MSS_QDSP6SS_LDO_USER_FB_STRNGTH2_SHFT                                         0xb
#define HWIO_MSS_QDSP6SS_LDO_USER_FB_STRNGTH1_BMSK                                       0x700
#define HWIO_MSS_QDSP6SS_LDO_USER_FB_STRNGTH1_SHFT                                         0x8
#define HWIO_MSS_QDSP6SS_LDO_USER_FB_CAP2_BMSK                                            0xf0
#define HWIO_MSS_QDSP6SS_LDO_USER_FB_CAP2_SHFT                                             0x4
#define HWIO_MSS_QDSP6SS_LDO_USER_FB_CAP1_BMSK                                             0xf
#define HWIO_MSS_QDSP6SS_LDO_USER_FB_CAP1_SHFT                                             0x0

#define HWIO_MSS_QDSP6SS_LDO_VREF_SET_ADDR                                          (MSS_QDSP6V66SS_PUB_REG_BASE      + 0x00000704)
#define HWIO_MSS_QDSP6SS_LDO_VREF_SET_OFFS                                          (MSS_QDSP6V66SS_PUB_REG_BASE_OFFS + 0x00000704)
#define HWIO_MSS_QDSP6SS_LDO_VREF_SET_RMSK                                             0x1007f
#define HWIO_MSS_QDSP6SS_LDO_VREF_SET_IN          \
        in_dword_masked(HWIO_MSS_QDSP6SS_LDO_VREF_SET_ADDR, HWIO_MSS_QDSP6SS_LDO_VREF_SET_RMSK)
#define HWIO_MSS_QDSP6SS_LDO_VREF_SET_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_LDO_VREF_SET_ADDR, m)
#define HWIO_MSS_QDSP6SS_LDO_VREF_SET_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_LDO_VREF_SET_ADDR,v)
#define HWIO_MSS_QDSP6SS_LDO_VREF_SET_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_LDO_VREF_SET_ADDR,m,v,HWIO_MSS_QDSP6SS_LDO_VREF_SET_IN)
#define HWIO_MSS_QDSP6SS_LDO_VREF_SET_LDO_VREF_SEL_RST_BMSK                            0x10000
#define HWIO_MSS_QDSP6SS_LDO_VREF_SET_LDO_VREF_SEL_RST_SHFT                               0x10
#define HWIO_MSS_QDSP6SS_LDO_VREF_SET_VREF_LDO_BMSK                                       0x7f
#define HWIO_MSS_QDSP6SS_LDO_VREF_SET_VREF_LDO_SHFT                                        0x0

#define HWIO_MSS_QDSP6SS_LDO_CFG0_ADDR                                              (MSS_QDSP6V66SS_PUB_REG_BASE      + 0x00000708)
#define HWIO_MSS_QDSP6SS_LDO_CFG0_OFFS                                              (MSS_QDSP6V66SS_PUB_REG_BASE_OFFS + 0x00000708)
#define HWIO_MSS_QDSP6SS_LDO_CFG0_RMSK                                              0xffffffff
#define HWIO_MSS_QDSP6SS_LDO_CFG0_IN          \
        in_dword_masked(HWIO_MSS_QDSP6SS_LDO_CFG0_ADDR, HWIO_MSS_QDSP6SS_LDO_CFG0_RMSK)
#define HWIO_MSS_QDSP6SS_LDO_CFG0_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_LDO_CFG0_ADDR, m)
#define HWIO_MSS_QDSP6SS_LDO_CFG0_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_LDO_CFG0_ADDR,v)
#define HWIO_MSS_QDSP6SS_LDO_CFG0_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_LDO_CFG0_ADDR,m,v,HWIO_MSS_QDSP6SS_LDO_CFG0_IN)
#define HWIO_MSS_QDSP6SS_LDO_CFG0_VSENSE_CTL_BMSK                                   0xffff0000
#define HWIO_MSS_QDSP6SS_LDO_CFG0_VSENSE_CTL_SHFT                                         0x10
#define HWIO_MSS_QDSP6SS_LDO_CFG0_ISENSE_CTL_BMSK                                       0xffff
#define HWIO_MSS_QDSP6SS_LDO_CFG0_ISENSE_CTL_SHFT                                          0x0

#define HWIO_MSS_QDSP6SS_LDO_CFG1_ADDR                                              (MSS_QDSP6V66SS_PUB_REG_BASE      + 0x0000070c)
#define HWIO_MSS_QDSP6SS_LDO_CFG1_OFFS                                              (MSS_QDSP6V66SS_PUB_REG_BASE_OFFS + 0x0000070c)
#define HWIO_MSS_QDSP6SS_LDO_CFG1_RMSK                                              0xffffffff
#define HWIO_MSS_QDSP6SS_LDO_CFG1_IN          \
        in_dword_masked(HWIO_MSS_QDSP6SS_LDO_CFG1_ADDR, HWIO_MSS_QDSP6SS_LDO_CFG1_RMSK)
#define HWIO_MSS_QDSP6SS_LDO_CFG1_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_LDO_CFG1_ADDR, m)
#define HWIO_MSS_QDSP6SS_LDO_CFG1_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_LDO_CFG1_ADDR,v)
#define HWIO_MSS_QDSP6SS_LDO_CFG1_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_LDO_CFG1_ADDR,m,v,HWIO_MSS_QDSP6SS_LDO_CFG1_IN)
#define HWIO_MSS_QDSP6SS_LDO_CFG1_LD_CTL_BMSK                                       0x1ff80000
#define HWIO_MSS_QDSP6SS_LDO_CFG1_LD_CTL_SHFT                                             0x13
#define HWIO_MSS_QDSP6SS_LDO_CFG1_LDO_OFFSET_CAL_CTL_BMSK                              0x40000
#define HWIO_MSS_QDSP6SS_LDO_CFG1_LDO_OFFSET_CAL_CTL_SHFT                                 0x12
#define HWIO_MSS_QDSP6SS_LDO_CFG1_TRIM_REF_SEL_BMSK                                    0x30000
#define HWIO_MSS_QDSP6SS_LDO_CFG1_TRIM_REF_SEL_SHFT                                       0x10

#define HWIO_MSS_QDSP6SS_LDO_CFG2_ADDR                                              (MSS_QDSP6V66SS_PUB_REG_BASE      + 0x00000710)
#define HWIO_MSS_QDSP6SS_LDO_CFG2_OFFS                                              (MSS_QDSP6V66SS_PUB_REG_BASE_OFFS + 0x00000710)
#define HWIO_MSS_QDSP6SS_LDO_CFG2_RMSK                                              0xffffffff
#define HWIO_MSS_QDSP6SS_LDO_CFG2_IN          \
        in_dword_masked(HWIO_MSS_QDSP6SS_LDO_CFG2_ADDR, HWIO_MSS_QDSP6SS_LDO_CFG2_RMSK)
#define HWIO_MSS_QDSP6SS_LDO_CFG2_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_LDO_CFG2_ADDR, m)
#define HWIO_MSS_QDSP6SS_LDO_CFG2_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_LDO_CFG2_ADDR,v)
#define HWIO_MSS_QDSP6SS_LDO_CFG2_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_LDO_CFG2_ADDR,m,v,HWIO_MSS_QDSP6SS_LDO_CFG2_IN)
#define HWIO_MSS_QDSP6SS_LDO_CFG2_PRE_ON_SLEW_BMSK                                   0x3800000
#define HWIO_MSS_QDSP6SS_LDO_CFG2_PRE_ON_SLEW_SHFT                                        0x17
#define HWIO_MSS_QDSP6SS_LDO_CFG2_ABYP_VDDMIN_SEL_BMSK                                0x700000
#define HWIO_MSS_QDSP6SS_LDO_CFG2_ABYP_VDDMIN_SEL_SHFT                                    0x14
#define HWIO_MSS_QDSP6SS_LDO_CFG2_ABYP_HDRM_SEL_BMSK                                   0xe0000
#define HWIO_MSS_QDSP6SS_LDO_CFG2_ABYP_HDRM_SEL_SHFT                                      0x11
#define HWIO_MSS_QDSP6SS_LDO_CFG2_VREF_SETTLE_TIMER_BMSK                               0x18000
#define HWIO_MSS_QDSP6SS_LDO_CFG2_VREF_SETTLE_TIMER_SHFT                                   0xf
#define HWIO_MSS_QDSP6SS_LDO_CFG2_LDO_RDY_TIMER_BMSK                                    0x6000
#define HWIO_MSS_QDSP6SS_LDO_CFG2_LDO_RDY_TIMER_SHFT                                       0xd
#define HWIO_MSS_QDSP6SS_LDO_CFG2_PRE_ON_TIMER_BMSK                                     0x1800
#define HWIO_MSS_QDSP6SS_LDO_CFG2_PRE_ON_TIMER_SHFT                                        0xb
#define HWIO_MSS_QDSP6SS_LDO_CFG2_PRE_ON_EN_BMSK                                         0x400
#define HWIO_MSS_QDSP6SS_LDO_CFG2_PRE_ON_EN_SHFT                                           0xa
#define HWIO_MSS_QDSP6SS_LDO_CFG2_PRE_ON_MODE_BMSK                                       0x200
#define HWIO_MSS_QDSP6SS_LDO_CFG2_PRE_ON_MODE_SHFT                                         0x9
#define HWIO_MSS_QDSP6SS_LDO_CFG2_ABYP_TIMER_EN_BMSK                                     0x100
#define HWIO_MSS_QDSP6SS_LDO_CFG2_ABYP_TIMER_EN_SHFT                                       0x8
#define HWIO_MSS_QDSP6SS_LDO_CFG2_ABYP_TIMER_CTL_BMSK                                     0xc0
#define HWIO_MSS_QDSP6SS_LDO_CFG2_ABYP_TIMER_CTL_SHFT                                      0x6
#define HWIO_MSS_QDSP6SS_LDO_CFG2_TRIM_POL_BMSK                                           0x20
#define HWIO_MSS_QDSP6SS_LDO_CFG2_TRIM_POL_SHFT                                            0x5
#define HWIO_MSS_QDSP6SS_LDO_CFG2_PWR_RAMP_UP_TIME_BMSK                                   0x18
#define HWIO_MSS_QDSP6SS_LDO_CFG2_PWR_RAMP_UP_TIME_SHFT                                    0x3
#define HWIO_MSS_QDSP6SS_LDO_CFG2_LDO_RAMP_EN_BMSK                                         0x4
#define HWIO_MSS_QDSP6SS_LDO_CFG2_LDO_RAMP_EN_SHFT                                         0x2
#define HWIO_MSS_QDSP6SS_LDO_CFG2_CLK_DIV_CTL_BMSK                                         0x3
#define HWIO_MSS_QDSP6SS_LDO_CFG2_CLK_DIV_CTL_SHFT                                         0x0

#define HWIO_MSS_QDSP6SS_LDO_TEST_CTRL_ADDR                                         (MSS_QDSP6V66SS_PUB_REG_BASE      + 0x00000714)
#define HWIO_MSS_QDSP6SS_LDO_TEST_CTRL_OFFS                                         (MSS_QDSP6V66SS_PUB_REG_BASE_OFFS + 0x00000714)
#define HWIO_MSS_QDSP6SS_LDO_TEST_CTRL_RMSK                                             0xffff
#define HWIO_MSS_QDSP6SS_LDO_TEST_CTRL_IN          \
        in_dword_masked(HWIO_MSS_QDSP6SS_LDO_TEST_CTRL_ADDR, HWIO_MSS_QDSP6SS_LDO_TEST_CTRL_RMSK)
#define HWIO_MSS_QDSP6SS_LDO_TEST_CTRL_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_LDO_TEST_CTRL_ADDR, m)
#define HWIO_MSS_QDSP6SS_LDO_TEST_CTRL_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_LDO_TEST_CTRL_ADDR,v)
#define HWIO_MSS_QDSP6SS_LDO_TEST_CTRL_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_LDO_TEST_CTRL_ADDR,m,v,HWIO_MSS_QDSP6SS_LDO_TEST_CTRL_IN)
#define HWIO_MSS_QDSP6SS_LDO_TEST_CTRL_ATEST1_CTL_BMSK                                  0x8000
#define HWIO_MSS_QDSP6SS_LDO_TEST_CTRL_ATEST1_CTL_SHFT                                     0xf
#define HWIO_MSS_QDSP6SS_LDO_TEST_CTRL_ATEST0_CTL_BMSK                                  0x4000
#define HWIO_MSS_QDSP6SS_LDO_TEST_CTRL_ATEST0_CTL_SHFT                                     0xe
#define HWIO_MSS_QDSP6SS_LDO_TEST_CTRL_ATEST_AMP_EN1_BMSK                               0x2000
#define HWIO_MSS_QDSP6SS_LDO_TEST_CTRL_ATEST_AMP_EN1_SHFT                                  0xd
#define HWIO_MSS_QDSP6SS_LDO_TEST_CTRL_ATEST_AMP_EN0_BMSK                               0x1000
#define HWIO_MSS_QDSP6SS_LDO_TEST_CTRL_ATEST_AMP_EN0_SHFT                                  0xc
#define HWIO_MSS_QDSP6SS_LDO_TEST_CTRL_NMO_CTL_BMSK                                      0xe00
#define HWIO_MSS_QDSP6SS_LDO_TEST_CTRL_NMO_CTL_SHFT                                        0x9
#define HWIO_MSS_QDSP6SS_LDO_TEST_CTRL_STATUS_MUX_SEL_BMSK                               0x1c0
#define HWIO_MSS_QDSP6SS_LDO_TEST_CTRL_STATUS_MUX_SEL_SHFT                                 0x6
#define HWIO_MSS_QDSP6SS_LDO_TEST_CTRL_ATEST1_SEL_BMSK                                    0x38
#define HWIO_MSS_QDSP6SS_LDO_TEST_CTRL_ATEST1_SEL_SHFT                                     0x3
#define HWIO_MSS_QDSP6SS_LDO_TEST_CTRL_ATEST0_SEL_BMSK                                     0x7
#define HWIO_MSS_QDSP6SS_LDO_TEST_CTRL_ATEST0_SEL_SHFT                                     0x0

#define HWIO_MSS_QDSP6SS_LDO_TRIM_CTL_ADDR                                          (MSS_QDSP6V66SS_PUB_REG_BASE      + 0x00000718)
#define HWIO_MSS_QDSP6SS_LDO_TRIM_CTL_OFFS                                          (MSS_QDSP6V66SS_PUB_REG_BASE_OFFS + 0x00000718)
#define HWIO_MSS_QDSP6SS_LDO_TRIM_CTL_RMSK                                                0xff
#define HWIO_MSS_QDSP6SS_LDO_TRIM_CTL_IN          \
        in_dword_masked(HWIO_MSS_QDSP6SS_LDO_TRIM_CTL_ADDR, HWIO_MSS_QDSP6SS_LDO_TRIM_CTL_RMSK)
#define HWIO_MSS_QDSP6SS_LDO_TRIM_CTL_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_LDO_TRIM_CTL_ADDR, m)
#define HWIO_MSS_QDSP6SS_LDO_TRIM_CTL_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_LDO_TRIM_CTL_ADDR,v)
#define HWIO_MSS_QDSP6SS_LDO_TRIM_CTL_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_LDO_TRIM_CTL_ADDR,m,v,HWIO_MSS_QDSP6SS_LDO_TRIM_CTL_IN)
#define HWIO_MSS_QDSP6SS_LDO_TRIM_CTL_SELF_TRIM_STEP_BMSK                                 0x80
#define HWIO_MSS_QDSP6SS_LDO_TRIM_CTL_SELF_TRIM_STEP_SHFT                                  0x7
#define HWIO_MSS_QDSP6SS_LDO_TRIM_CTL_SELF_TRIM_STRT_BMSK                                 0x40
#define HWIO_MSS_QDSP6SS_LDO_TRIM_CTL_SELF_TRIM_STRT_SHFT                                  0x6
#define HWIO_MSS_QDSP6SS_LDO_TRIM_CTL_LDO_TRIM_CTL_BMSK                                   0x20
#define HWIO_MSS_QDSP6SS_LDO_TRIM_CTL_LDO_TRIM_CTL_SHFT                                    0x5
#define HWIO_MSS_QDSP6SS_LDO_TRIM_CTL_LDO_TRIM_OVR_BMSK                                   0x1f
#define HWIO_MSS_QDSP6SS_LDO_TRIM_CTL_LDO_TRIM_OVR_SHFT                                    0x0

#define HWIO_MSS_QDSP6SS_LDO_STATUS_ADDR                                            (MSS_QDSP6V66SS_PUB_REG_BASE      + 0x0000071c)
#define HWIO_MSS_QDSP6SS_LDO_STATUS_OFFS                                            (MSS_QDSP6V66SS_PUB_REG_BASE_OFFS + 0x0000071c)
#define HWIO_MSS_QDSP6SS_LDO_STATUS_RMSK                                            0xffffffff
#define HWIO_MSS_QDSP6SS_LDO_STATUS_IN          \
        in_dword_masked(HWIO_MSS_QDSP6SS_LDO_STATUS_ADDR, HWIO_MSS_QDSP6SS_LDO_STATUS_RMSK)
#define HWIO_MSS_QDSP6SS_LDO_STATUS_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_LDO_STATUS_ADDR, m)
#define HWIO_MSS_QDSP6SS_LDO_STATUS_LDO_OPMODE_BMSK                                 0xffffffff
#define HWIO_MSS_QDSP6SS_LDO_STATUS_LDO_OPMODE_SHFT                                        0x0

#define HWIO_MSS_QDSP6SS_LDO_ISENS_OUT_ADDR                                         (MSS_QDSP6V66SS_PUB_REG_BASE      + 0x00000720)
#define HWIO_MSS_QDSP6SS_LDO_ISENS_OUT_OFFS                                         (MSS_QDSP6V66SS_PUB_REG_BASE_OFFS + 0x00000720)
#define HWIO_MSS_QDSP6SS_LDO_ISENS_OUT_RMSK                                              0x3ff
#define HWIO_MSS_QDSP6SS_LDO_ISENS_OUT_IN          \
        in_dword_masked(HWIO_MSS_QDSP6SS_LDO_ISENS_OUT_ADDR, HWIO_MSS_QDSP6SS_LDO_ISENS_OUT_RMSK)
#define HWIO_MSS_QDSP6SS_LDO_ISENS_OUT_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_LDO_ISENS_OUT_ADDR, m)
#define HWIO_MSS_QDSP6SS_LDO_ISENS_OUT_LDO_ISENSE_OUT_BMSK                               0x3ff
#define HWIO_MSS_QDSP6SS_LDO_ISENS_OUT_LDO_ISENSE_OUT_SHFT                                 0x0

#define HWIO_MSS_QDSP6SS_LDO_ISENS_CTL_ADDR                                         (MSS_QDSP6V66SS_PUB_REG_BASE      + 0x00000724)
#define HWIO_MSS_QDSP6SS_LDO_ISENS_CTL_OFFS                                         (MSS_QDSP6V66SS_PUB_REG_BASE_OFFS + 0x00000724)
#define HWIO_MSS_QDSP6SS_LDO_ISENS_CTL_RMSK                                            0x1ffff
#define HWIO_MSS_QDSP6SS_LDO_ISENS_CTL_IN          \
        in_dword_masked(HWIO_MSS_QDSP6SS_LDO_ISENS_CTL_ADDR, HWIO_MSS_QDSP6SS_LDO_ISENS_CTL_RMSK)
#define HWIO_MSS_QDSP6SS_LDO_ISENS_CTL_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_LDO_ISENS_CTL_ADDR, m)
#define HWIO_MSS_QDSP6SS_LDO_ISENS_CTL_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_LDO_ISENS_CTL_ADDR,v)
#define HWIO_MSS_QDSP6SS_LDO_ISENS_CTL_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_LDO_ISENS_CTL_ADDR,m,v,HWIO_MSS_QDSP6SS_LDO_ISENS_CTL_IN)
#define HWIO_MSS_QDSP6SS_LDO_ISENS_CTL_LDO_ISENS_EN_BMSK                               0x10000
#define HWIO_MSS_QDSP6SS_LDO_ISENS_CTL_LDO_ISENS_EN_SHFT                                  0x10
#define HWIO_MSS_QDSP6SS_LDO_ISENS_CTL_LDO_ISENS_TRIM_BMSK                              0xffff
#define HWIO_MSS_QDSP6SS_LDO_ISENS_CTL_LDO_ISENS_TRIM_SHFT                                 0x0

#define HWIO_MSS_QDSP6SS_LDO_VREF_CMD_ADDR                                          (MSS_QDSP6V66SS_PUB_REG_BASE      + 0x00000728)
#define HWIO_MSS_QDSP6SS_LDO_VREF_CMD_OFFS                                          (MSS_QDSP6V66SS_PUB_REG_BASE_OFFS + 0x00000728)
#define HWIO_MSS_QDSP6SS_LDO_VREF_CMD_RMSK                                                 0x1
#define HWIO_MSS_QDSP6SS_LDO_VREF_CMD_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_LDO_VREF_CMD_ADDR,v)
#define HWIO_MSS_QDSP6SS_LDO_VREF_CMD_LDO_VREF_SEL_UPDATE_BMSK                             0x1
#define HWIO_MSS_QDSP6SS_LDO_VREF_CMD_LDO_VREF_SEL_UPDATE_SHFT                             0x0

#define HWIO_MSS_QDSP6SS_LDO_PFET_CFG_ADDR                                          (MSS_QDSP6V66SS_PUB_REG_BASE      + 0x0000072c)
#define HWIO_MSS_QDSP6SS_LDO_PFET_CFG_OFFS                                          (MSS_QDSP6V66SS_PUB_REG_BASE_OFFS + 0x0000072c)
#define HWIO_MSS_QDSP6SS_LDO_PFET_CFG_RMSK                                              0xffff
#define HWIO_MSS_QDSP6SS_LDO_PFET_CFG_IN          \
        in_dword_masked(HWIO_MSS_QDSP6SS_LDO_PFET_CFG_ADDR, HWIO_MSS_QDSP6SS_LDO_PFET_CFG_RMSK)
#define HWIO_MSS_QDSP6SS_LDO_PFET_CFG_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_LDO_PFET_CFG_ADDR, m)
#define HWIO_MSS_QDSP6SS_LDO_PFET_CFG_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_LDO_PFET_CFG_ADDR,v)
#define HWIO_MSS_QDSP6SS_LDO_PFET_CFG_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_LDO_PFET_CFG_ADDR,m,v,HWIO_MSS_QDSP6SS_LDO_PFET_CFG_IN)
#define HWIO_MSS_QDSP6SS_LDO_PFET_CFG_PFET2_CFG_BMSK                                    0xff00
#define HWIO_MSS_QDSP6SS_LDO_PFET_CFG_PFET2_CFG_SHFT                                       0x8
#define HWIO_MSS_QDSP6SS_LDO_PFET_CFG_PFET1_CFG_BMSK                                      0xff
#define HWIO_MSS_QDSP6SS_LDO_PFET_CFG_PFET1_CFG_SHFT                                       0x0

#define HWIO_MSS_QDSP6SS_LDO_PFET_TEST_CTRL_ADDR                                    (MSS_QDSP6V66SS_PUB_REG_BASE      + 0x00000730)
#define HWIO_MSS_QDSP6SS_LDO_PFET_TEST_CTRL_OFFS                                    (MSS_QDSP6V66SS_PUB_REG_BASE_OFFS + 0x00000730)
#define HWIO_MSS_QDSP6SS_LDO_PFET_TEST_CTRL_RMSK                                         0x7ff
#define HWIO_MSS_QDSP6SS_LDO_PFET_TEST_CTRL_IN          \
        in_dword_masked(HWIO_MSS_QDSP6SS_LDO_PFET_TEST_CTRL_ADDR, HWIO_MSS_QDSP6SS_LDO_PFET_TEST_CTRL_RMSK)
#define HWIO_MSS_QDSP6SS_LDO_PFET_TEST_CTRL_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_LDO_PFET_TEST_CTRL_ADDR, m)
#define HWIO_MSS_QDSP6SS_LDO_PFET_TEST_CTRL_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_LDO_PFET_TEST_CTRL_ADDR,v)
#define HWIO_MSS_QDSP6SS_LDO_PFET_TEST_CTRL_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_LDO_PFET_TEST_CTRL_ADDR,m,v,HWIO_MSS_QDSP6SS_LDO_PFET_TEST_CTRL_IN)
#define HWIO_MSS_QDSP6SS_LDO_PFET_TEST_CTRL_PFET_ATEST_SEL_BMSK                          0x700
#define HWIO_MSS_QDSP6SS_LDO_PFET_TEST_CTRL_PFET_ATEST_SEL_SHFT                            0x8
#define HWIO_MSS_QDSP6SS_LDO_PFET_TEST_CTRL_PFET_ATEST_CTRL_BMSK                          0xff
#define HWIO_MSS_QDSP6SS_LDO_PFET_TEST_CTRL_PFET_ATEST_CTRL_SHFT                           0x0

#define HWIO_MSS_QDSP6SS_LDO_EFUSE_STATUS_ADDR                                      (MSS_QDSP6V66SS_PUB_REG_BASE      + 0x00000780)
#define HWIO_MSS_QDSP6SS_LDO_EFUSE_STATUS_OFFS                                      (MSS_QDSP6V66SS_PUB_REG_BASE_OFFS + 0x00000780)
#define HWIO_MSS_QDSP6SS_LDO_EFUSE_STATUS_RMSK                                            0x3f
#define HWIO_MSS_QDSP6SS_LDO_EFUSE_STATUS_IN          \
        in_dword_masked(HWIO_MSS_QDSP6SS_LDO_EFUSE_STATUS_ADDR, HWIO_MSS_QDSP6SS_LDO_EFUSE_STATUS_RMSK)
#define HWIO_MSS_QDSP6SS_LDO_EFUSE_STATUS_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_LDO_EFUSE_STATUS_ADDR, m)
#define HWIO_MSS_QDSP6SS_LDO_EFUSE_STATUS_LDO_VREF_TRIM_BMSK                              0x3e
#define HWIO_MSS_QDSP6SS_LDO_EFUSE_STATUS_LDO_VREF_TRIM_SHFT                               0x1
#define HWIO_MSS_QDSP6SS_LDO_EFUSE_STATUS_LDO_EN_BMSK                                      0x1
#define HWIO_MSS_QDSP6SS_LDO_EFUSE_STATUS_LDO_EN_SHFT                                      0x0

#define HWIO_MSS_QDSP6SS_LDO_PWR_STATUS_ADDR                                        (MSS_QDSP6V66SS_PUB_REG_BASE      + 0x00000784)
#define HWIO_MSS_QDSP6SS_LDO_PWR_STATUS_OFFS                                        (MSS_QDSP6V66SS_PUB_REG_BASE_OFFS + 0x00000784)
#define HWIO_MSS_QDSP6SS_LDO_PWR_STATUS_RMSK                                               0x3
#define HWIO_MSS_QDSP6SS_LDO_PWR_STATUS_IN          \
        in_dword_masked(HWIO_MSS_QDSP6SS_LDO_PWR_STATUS_ADDR, HWIO_MSS_QDSP6SS_LDO_PWR_STATUS_RMSK)
#define HWIO_MSS_QDSP6SS_LDO_PWR_STATUS_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_LDO_PWR_STATUS_ADDR, m)
#define HWIO_MSS_QDSP6SS_LDO_PWR_STATUS_LDO_VREF_SETTLE_BMSK                               0x2
#define HWIO_MSS_QDSP6SS_LDO_PWR_STATUS_LDO_VREF_SETTLE_SHFT                               0x1
#define HWIO_MSS_QDSP6SS_LDO_PWR_STATUS_LDO_READY_BMSK                                     0x1
#define HWIO_MSS_QDSP6SS_LDO_PWR_STATUS_LDO_READY_SHFT                                     0x0

#define HWIO_MSS_QDSP6SS_LDO_PREON_CTL_ADDR                                         (MSS_QDSP6V66SS_PUB_REG_BASE      + 0x00000788)
#define HWIO_MSS_QDSP6SS_LDO_PREON_CTL_OFFS                                         (MSS_QDSP6V66SS_PUB_REG_BASE_OFFS + 0x00000788)
#define HWIO_MSS_QDSP6SS_LDO_PREON_CTL_RMSK                                                0x3
#define HWIO_MSS_QDSP6SS_LDO_PREON_CTL_IN          \
        in_dword_masked(HWIO_MSS_QDSP6SS_LDO_PREON_CTL_ADDR, HWIO_MSS_QDSP6SS_LDO_PREON_CTL_RMSK)
#define HWIO_MSS_QDSP6SS_LDO_PREON_CTL_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_LDO_PREON_CTL_ADDR, m)
#define HWIO_MSS_QDSP6SS_LDO_PREON_CTL_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_LDO_PREON_CTL_ADDR,v)
#define HWIO_MSS_QDSP6SS_LDO_PREON_CTL_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_LDO_PREON_CTL_ADDR,m,v,HWIO_MSS_QDSP6SS_LDO_PREON_CTL_IN)
#define HWIO_MSS_QDSP6SS_LDO_PREON_CTL_CORE_PREON_EN_BMSK                                  0x2
#define HWIO_MSS_QDSP6SS_LDO_PREON_CTL_CORE_PREON_EN_SHFT                                  0x1
#define HWIO_MSS_QDSP6SS_LDO_PREON_CTL_CP_PREON_EN_BMSK                                    0x1
#define HWIO_MSS_QDSP6SS_LDO_PREON_CTL_CP_PREON_EN_SHFT                                    0x0

#define HWIO_MSS_QDSP6SS_LDO_CORE_CLK_CBCR_ADDR                                     (MSS_QDSP6V66SS_PUB_REG_BASE      + 0x000007a0)
#define HWIO_MSS_QDSP6SS_LDO_CORE_CLK_CBCR_OFFS                                     (MSS_QDSP6V66SS_PUB_REG_BASE_OFFS + 0x000007a0)
#define HWIO_MSS_QDSP6SS_LDO_CORE_CLK_CBCR_RMSK                                     0x80000001
#define HWIO_MSS_QDSP6SS_LDO_CORE_CLK_CBCR_IN          \
        in_dword_masked(HWIO_MSS_QDSP6SS_LDO_CORE_CLK_CBCR_ADDR, HWIO_MSS_QDSP6SS_LDO_CORE_CLK_CBCR_RMSK)
#define HWIO_MSS_QDSP6SS_LDO_CORE_CLK_CBCR_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_LDO_CORE_CLK_CBCR_ADDR, m)
#define HWIO_MSS_QDSP6SS_LDO_CORE_CLK_CBCR_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_LDO_CORE_CLK_CBCR_ADDR,v)
#define HWIO_MSS_QDSP6SS_LDO_CORE_CLK_CBCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_LDO_CORE_CLK_CBCR_ADDR,m,v,HWIO_MSS_QDSP6SS_LDO_CORE_CLK_CBCR_IN)
#define HWIO_MSS_QDSP6SS_LDO_CORE_CLK_CBCR_CLKOFF_BMSK                              0x80000000
#define HWIO_MSS_QDSP6SS_LDO_CORE_CLK_CBCR_CLKOFF_SHFT                                    0x1f
#define HWIO_MSS_QDSP6SS_LDO_CORE_CLK_CBCR_CLKEN_BMSK                                      0x1
#define HWIO_MSS_QDSP6SS_LDO_CORE_CLK_CBCR_CLKEN_SHFT                                      0x0

#define HWIO_MSS_QDSP6SS_TEST_BUS_CTL_ADDR                                          (MSS_QDSP6V66SS_PUB_REG_BASE      + 0x00002000)
#define HWIO_MSS_QDSP6SS_TEST_BUS_CTL_OFFS                                          (MSS_QDSP6V66SS_PUB_REG_BASE_OFFS + 0x00002000)
#define HWIO_MSS_QDSP6SS_TEST_BUS_CTL_RMSK                                          0xffffffff
#define HWIO_MSS_QDSP6SS_TEST_BUS_CTL_IN          \
        in_dword_masked(HWIO_MSS_QDSP6SS_TEST_BUS_CTL_ADDR, HWIO_MSS_QDSP6SS_TEST_BUS_CTL_RMSK)
#define HWIO_MSS_QDSP6SS_TEST_BUS_CTL_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_TEST_BUS_CTL_ADDR, m)
#define HWIO_MSS_QDSP6SS_TEST_BUS_CTL_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_TEST_BUS_CTL_ADDR,v)
#define HWIO_MSS_QDSP6SS_TEST_BUS_CTL_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_TEST_BUS_CTL_ADDR,m,v,HWIO_MSS_QDSP6SS_TEST_BUS_CTL_IN)
#define HWIO_MSS_QDSP6SS_TEST_BUS_CTL_SPARE_CFG_BMSK                                0xff000000
#define HWIO_MSS_QDSP6SS_TEST_BUS_CTL_SPARE_CFG_SHFT                                      0x18
#define HWIO_MSS_QDSP6SS_TEST_BUS_CTL_HWE_INV_BMSK                                    0x800000
#define HWIO_MSS_QDSP6SS_TEST_BUS_CTL_HWE_INV_SHFT                                        0x17
#define HWIO_MSS_QDSP6SS_TEST_BUS_CTL_HWE_SEL_BMSK                                    0x7c0000
#define HWIO_MSS_QDSP6SS_TEST_BUS_CTL_HWE_SEL_SHFT                                        0x12
#define HWIO_MSS_QDSP6SS_TEST_BUS_CTL_TESTBUS_EN_BMSK                                  0x20000
#define HWIO_MSS_QDSP6SS_TEST_BUS_CTL_TESTBUS_EN_SHFT                                     0x11
#define HWIO_MSS_QDSP6SS_TEST_BUS_CTL_Q6_SEL_BMSK                                      0x10000
#define HWIO_MSS_QDSP6SS_TEST_BUS_CTL_Q6_SEL_SHFT                                         0x10
#define HWIO_MSS_QDSP6SS_TEST_BUS_CTL_SEL_BMSK                                          0xffff
#define HWIO_MSS_QDSP6SS_TEST_BUS_CTL_SEL_SHFT                                             0x0

#define HWIO_MSS_QDSP6SS_TEST_BUS_VALUE_ADDR                                        (MSS_QDSP6V66SS_PUB_REG_BASE      + 0x00002004)
#define HWIO_MSS_QDSP6SS_TEST_BUS_VALUE_OFFS                                        (MSS_QDSP6V66SS_PUB_REG_BASE_OFFS + 0x00002004)
#define HWIO_MSS_QDSP6SS_TEST_BUS_VALUE_RMSK                                        0xffffffff
#define HWIO_MSS_QDSP6SS_TEST_BUS_VALUE_IN          \
        in_dword_masked(HWIO_MSS_QDSP6SS_TEST_BUS_VALUE_ADDR, HWIO_MSS_QDSP6SS_TEST_BUS_VALUE_RMSK)
#define HWIO_MSS_QDSP6SS_TEST_BUS_VALUE_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_TEST_BUS_VALUE_ADDR, m)
#define HWIO_MSS_QDSP6SS_TEST_BUS_VALUE_VALUE_BMSK                                  0xffffffff
#define HWIO_MSS_QDSP6SS_TEST_BUS_VALUE_VALUE_SHFT                                         0x0

#define HWIO_MSS_QDSP6SS_TEST_BUS_WDATA_ADDR                                        (MSS_QDSP6V66SS_PUB_REG_BASE      + 0x00002008)
#define HWIO_MSS_QDSP6SS_TEST_BUS_WDATA_OFFS                                        (MSS_QDSP6V66SS_PUB_REG_BASE_OFFS + 0x00002008)
#define HWIO_MSS_QDSP6SS_TEST_BUS_WDATA_RMSK                                        0xffffffff
#define HWIO_MSS_QDSP6SS_TEST_BUS_WDATA_IN          \
        in_dword_masked(HWIO_MSS_QDSP6SS_TEST_BUS_WDATA_ADDR, HWIO_MSS_QDSP6SS_TEST_BUS_WDATA_RMSK)
#define HWIO_MSS_QDSP6SS_TEST_BUS_WDATA_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_TEST_BUS_WDATA_ADDR, m)
#define HWIO_MSS_QDSP6SS_TEST_BUS_WDATA_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_TEST_BUS_WDATA_ADDR,v)
#define HWIO_MSS_QDSP6SS_TEST_BUS_WDATA_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_TEST_BUS_WDATA_ADDR,m,v,HWIO_MSS_QDSP6SS_TEST_BUS_WDATA_IN)
#define HWIO_MSS_QDSP6SS_TEST_BUS_WDATA_VALUE_BMSK                                  0xffffffff
#define HWIO_MSS_QDSP6SS_TEST_BUS_WDATA_VALUE_SHFT                                         0x0

#define HWIO_MSS_QDSP6SS_PWRDBG_CTL_ADDR                                            (MSS_QDSP6V66SS_PUB_REG_BASE      + 0x0000200c)
#define HWIO_MSS_QDSP6SS_PWRDBG_CTL_OFFS                                            (MSS_QDSP6V66SS_PUB_REG_BASE_OFFS + 0x0000200c)
#define HWIO_MSS_QDSP6SS_PWRDBG_CTL_RMSK                                                   0x1
#define HWIO_MSS_QDSP6SS_PWRDBG_CTL_IN          \
        in_dword_masked(HWIO_MSS_QDSP6SS_PWRDBG_CTL_ADDR, HWIO_MSS_QDSP6SS_PWRDBG_CTL_RMSK)
#define HWIO_MSS_QDSP6SS_PWRDBG_CTL_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_PWRDBG_CTL_ADDR, m)
#define HWIO_MSS_QDSP6SS_PWRDBG_CTL_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_PWRDBG_CTL_ADDR,v)
#define HWIO_MSS_QDSP6SS_PWRDBG_CTL_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_PWRDBG_CTL_ADDR,m,v,HWIO_MSS_QDSP6SS_PWRDBG_CTL_IN)
#define HWIO_MSS_QDSP6SS_PWRDBG_CTL_ENA_BMSK                                               0x1
#define HWIO_MSS_QDSP6SS_PWRDBG_CTL_ENA_SHFT                                               0x0

#define HWIO_MSS_QDSP6SS_TEST_CLK_CTL_ADDR                                          (MSS_QDSP6V66SS_PUB_REG_BASE      + 0x00002010)
#define HWIO_MSS_QDSP6SS_TEST_CLK_CTL_OFFS                                          (MSS_QDSP6V66SS_PUB_REG_BASE_OFFS + 0x00002010)
#define HWIO_MSS_QDSP6SS_TEST_CLK_CTL_RMSK                                                0x7f
#define HWIO_MSS_QDSP6SS_TEST_CLK_CTL_IN          \
        in_dword_masked(HWIO_MSS_QDSP6SS_TEST_CLK_CTL_ADDR, HWIO_MSS_QDSP6SS_TEST_CLK_CTL_RMSK)
#define HWIO_MSS_QDSP6SS_TEST_CLK_CTL_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_TEST_CLK_CTL_ADDR, m)
#define HWIO_MSS_QDSP6SS_TEST_CLK_CTL_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_TEST_CLK_CTL_ADDR,v)
#define HWIO_MSS_QDSP6SS_TEST_CLK_CTL_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_TEST_CLK_CTL_ADDR,m,v,HWIO_MSS_QDSP6SS_TEST_CLK_CTL_IN)
#define HWIO_MSS_QDSP6SS_TEST_CLK_CTL_Q6_DBG_CLK_EN_BMSK                                  0x40
#define HWIO_MSS_QDSP6SS_TEST_CLK_CTL_Q6_DBG_CLK_EN_SHFT                                   0x6
#define HWIO_MSS_QDSP6SS_TEST_CLK_CTL_Q6_DBG_CLK_INV_BMSK                                 0x20
#define HWIO_MSS_QDSP6SS_TEST_CLK_CTL_Q6_DBG_CLK_INV_SHFT                                  0x5
#define HWIO_MSS_QDSP6SS_TEST_CLK_CTL_DBG_CLK_SEL_BMSK                                    0x1f
#define HWIO_MSS_QDSP6SS_TEST_CLK_CTL_DBG_CLK_SEL_SHFT                                     0x0

#define HWIO_MSS_QDSP6SS_BHS_TEST_ADDR                                              (MSS_QDSP6V66SS_PUB_REG_BASE      + 0x00002014)
#define HWIO_MSS_QDSP6SS_BHS_TEST_OFFS                                              (MSS_QDSP6V66SS_PUB_REG_BASE_OFFS + 0x00002014)
#define HWIO_MSS_QDSP6SS_BHS_TEST_RMSK                                                     0x1
#define HWIO_MSS_QDSP6SS_BHS_TEST_IN          \
        in_dword_masked(HWIO_MSS_QDSP6SS_BHS_TEST_ADDR, HWIO_MSS_QDSP6SS_BHS_TEST_RMSK)
#define HWIO_MSS_QDSP6SS_BHS_TEST_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_BHS_TEST_ADDR, m)
#define HWIO_MSS_QDSP6SS_BHS_TEST_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_BHS_TEST_ADDR,v)
#define HWIO_MSS_QDSP6SS_BHS_TEST_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_BHS_TEST_ADDR,m,v,HWIO_MSS_QDSP6SS_BHS_TEST_IN)
#define HWIO_MSS_QDSP6SS_BHS_TEST_INSTANT_ON_BMSK                                          0x1
#define HWIO_MSS_QDSP6SS_BHS_TEST_INSTANT_ON_SHFT                                          0x0

#define HWIO_MSS_QDSP6SS_ZMEAS_CONFIG_ADDR                                          (MSS_QDSP6V66SS_PUB_REG_BASE      + 0x00002018)
#define HWIO_MSS_QDSP6SS_ZMEAS_CONFIG_OFFS                                          (MSS_QDSP6V66SS_PUB_REG_BASE_OFFS + 0x00002018)
#define HWIO_MSS_QDSP6SS_ZMEAS_CONFIG_RMSK                                            0xffffff
#define HWIO_MSS_QDSP6SS_ZMEAS_CONFIG_IN          \
        in_dword_masked(HWIO_MSS_QDSP6SS_ZMEAS_CONFIG_ADDR, HWIO_MSS_QDSP6SS_ZMEAS_CONFIG_RMSK)
#define HWIO_MSS_QDSP6SS_ZMEAS_CONFIG_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_ZMEAS_CONFIG_ADDR, m)
#define HWIO_MSS_QDSP6SS_ZMEAS_CONFIG_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_ZMEAS_CONFIG_ADDR,v)
#define HWIO_MSS_QDSP6SS_ZMEAS_CONFIG_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_ZMEAS_CONFIG_ADDR,m,v,HWIO_MSS_QDSP6SS_ZMEAS_CONFIG_IN)
#define HWIO_MSS_QDSP6SS_ZMEAS_CONFIG_ZMEAS_COUNT_BMSK                                0xffffe0
#define HWIO_MSS_QDSP6SS_ZMEAS_CONFIG_ZMEAS_COUNT_SHFT                                     0x5
#define HWIO_MSS_QDSP6SS_ZMEAS_CONFIG_MUX_SEL_BMSK                                        0x10
#define HWIO_MSS_QDSP6SS_ZMEAS_CONFIG_MUX_SEL_SHFT                                         0x4
#define HWIO_MSS_QDSP6SS_ZMEAS_CONFIG_ZMEAS_CLK_R_ENA_BMSK                                 0x8
#define HWIO_MSS_QDSP6SS_ZMEAS_CONFIG_ZMEAS_CLK_R_ENA_SHFT                                 0x3
#define HWIO_MSS_QDSP6SS_ZMEAS_CONFIG_ZMEAS_CGC_EN_BMSK                                    0x4
#define HWIO_MSS_QDSP6SS_ZMEAS_CONFIG_ZMEAS_CGC_EN_SHFT                                    0x2
#define HWIO_MSS_QDSP6SS_ZMEAS_CONFIG_ZMEAS_BYPASS_BMSK                                    0x2
#define HWIO_MSS_QDSP6SS_ZMEAS_CONFIG_ZMEAS_BYPASS_SHFT                                    0x1
#define HWIO_MSS_QDSP6SS_ZMEAS_CONFIG_ZMEAS_SW_RESET_BMSK                                  0x1
#define HWIO_MSS_QDSP6SS_ZMEAS_CONFIG_ZMEAS_SW_RESET_SHFT                                  0x0

#define HWIO_MSS_QDSP6SS_CORE_STATUS_ADDR                                           (MSS_QDSP6V66SS_PUB_REG_BASE      + 0x00002028)
#define HWIO_MSS_QDSP6SS_CORE_STATUS_OFFS                                           (MSS_QDSP6V66SS_PUB_REG_BASE_OFFS + 0x00002028)
#define HWIO_MSS_QDSP6SS_CORE_STATUS_RMSK                                                 0x3f
#define HWIO_MSS_QDSP6SS_CORE_STATUS_IN          \
        in_dword_masked(HWIO_MSS_QDSP6SS_CORE_STATUS_ADDR, HWIO_MSS_QDSP6SS_CORE_STATUS_RMSK)
#define HWIO_MSS_QDSP6SS_CORE_STATUS_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_CORE_STATUS_ADDR, m)
#define HWIO_MSS_QDSP6SS_CORE_STATUS_VU1_VX_IDLE_BMSK                                     0x20
#define HWIO_MSS_QDSP6SS_CORE_STATUS_VU1_VX_IDLE_SHFT                                      0x5
#define HWIO_MSS_QDSP6SS_CORE_STATUS_VU0_VX_IDLE_BMSK                                     0x10
#define HWIO_MSS_QDSP6SS_CORE_STATUS_VU0_VX_IDLE_SHFT                                      0x4
#define HWIO_MSS_QDSP6SS_CORE_STATUS_VU_OVERALL_IDLE_BMSK                                  0x8
#define HWIO_MSS_QDSP6SS_CORE_STATUS_VU_OVERALL_IDLE_SHFT                                  0x3
#define HWIO_MSS_QDSP6SS_CORE_STATUS_CP0_IDLE_BMSK                                         0x4
#define HWIO_MSS_QDSP6SS_CORE_STATUS_CP0_IDLE_SHFT                                         0x2
#define HWIO_MSS_QDSP6SS_CORE_STATUS_CORE_IN_SLEEP_BMSK                                    0x2
#define HWIO_MSS_QDSP6SS_CORE_STATUS_CORE_IN_SLEEP_SHFT                                    0x1
#define HWIO_MSS_QDSP6SS_CORE_STATUS_CORE_STATE_BMSK                                       0x1
#define HWIO_MSS_QDSP6SS_CORE_STATUS_CORE_STATE_SHFT                                       0x0

#define HWIO_MSS_QDSP6SS_MEM_DEBUG_ADDR                                             (MSS_QDSP6V66SS_PUB_REG_BASE      + 0x00002030)
#define HWIO_MSS_QDSP6SS_MEM_DEBUG_OFFS                                             (MSS_QDSP6V66SS_PUB_REG_BASE_OFFS + 0x00002030)
#define HWIO_MSS_QDSP6SS_MEM_DEBUG_RMSK                                                 0xffff
#define HWIO_MSS_QDSP6SS_MEM_DEBUG_IN          \
        in_dword_masked(HWIO_MSS_QDSP6SS_MEM_DEBUG_ADDR, HWIO_MSS_QDSP6SS_MEM_DEBUG_RMSK)
#define HWIO_MSS_QDSP6SS_MEM_DEBUG_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_MEM_DEBUG_ADDR, m)
#define HWIO_MSS_QDSP6SS_MEM_DEBUG_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_MEM_DEBUG_ADDR,v)
#define HWIO_MSS_QDSP6SS_MEM_DEBUG_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_MEM_DEBUG_ADDR,m,v,HWIO_MSS_QDSP6SS_MEM_DEBUG_IN)
#define HWIO_MSS_QDSP6SS_MEM_DEBUG_OFF_BMSK                                             0xffff
#define HWIO_MSS_QDSP6SS_MEM_DEBUG_OFF_SHFT                                                0x0

#define HWIO_MSS_QDSP6SS_L2MEM_DEBUG0_ADDR                                          (MSS_QDSP6V66SS_PUB_REG_BASE      + 0x00002034)
#define HWIO_MSS_QDSP6SS_L2MEM_DEBUG0_OFFS                                          (MSS_QDSP6V66SS_PUB_REG_BASE_OFFS + 0x00002034)
#define HWIO_MSS_QDSP6SS_L2MEM_DEBUG0_RMSK                                          0xffffffff
#define HWIO_MSS_QDSP6SS_L2MEM_DEBUG0_IN          \
        in_dword_masked(HWIO_MSS_QDSP6SS_L2MEM_DEBUG0_ADDR, HWIO_MSS_QDSP6SS_L2MEM_DEBUG0_RMSK)
#define HWIO_MSS_QDSP6SS_L2MEM_DEBUG0_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_L2MEM_DEBUG0_ADDR, m)
#define HWIO_MSS_QDSP6SS_L2MEM_DEBUG0_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_L2MEM_DEBUG0_ADDR,v)
#define HWIO_MSS_QDSP6SS_L2MEM_DEBUG0_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_L2MEM_DEBUG0_ADDR,m,v,HWIO_MSS_QDSP6SS_L2MEM_DEBUG0_IN)
#define HWIO_MSS_QDSP6SS_L2MEM_DEBUG0_OFF_BMSK                                      0xffffffff
#define HWIO_MSS_QDSP6SS_L2MEM_DEBUG0_OFF_SHFT                                             0x0

#define HWIO_MSS_QDSP6SS_L2MEM_DEBUG1_ADDR                                          (MSS_QDSP6V66SS_PUB_REG_BASE      + 0x00002038)
#define HWIO_MSS_QDSP6SS_L2MEM_DEBUG1_OFFS                                          (MSS_QDSP6V66SS_PUB_REG_BASE_OFFS + 0x00002038)
#define HWIO_MSS_QDSP6SS_L2MEM_DEBUG1_RMSK                                          0xffffffff
#define HWIO_MSS_QDSP6SS_L2MEM_DEBUG1_IN          \
        in_dword_masked(HWIO_MSS_QDSP6SS_L2MEM_DEBUG1_ADDR, HWIO_MSS_QDSP6SS_L2MEM_DEBUG1_RMSK)
#define HWIO_MSS_QDSP6SS_L2MEM_DEBUG1_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_L2MEM_DEBUG1_ADDR, m)
#define HWIO_MSS_QDSP6SS_L2MEM_DEBUG1_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_L2MEM_DEBUG1_ADDR,v)
#define HWIO_MSS_QDSP6SS_L2MEM_DEBUG1_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_L2MEM_DEBUG1_ADDR,m,v,HWIO_MSS_QDSP6SS_L2MEM_DEBUG1_IN)
#define HWIO_MSS_QDSP6SS_L2MEM_DEBUG1_OFF_BMSK                                      0xffffffff
#define HWIO_MSS_QDSP6SS_L2MEM_DEBUG1_OFF_SHFT                                             0x0

#define HWIO_MSS_QDSP6SS_CPMEM_DEBUG_ADDR                                           (MSS_QDSP6V66SS_PUB_REG_BASE      + 0x0000203c)
#define HWIO_MSS_QDSP6SS_CPMEM_DEBUG_OFFS                                           (MSS_QDSP6V66SS_PUB_REG_BASE_OFFS + 0x0000203c)
#define HWIO_MSS_QDSP6SS_CPMEM_DEBUG_RMSK                                           0xffffffff
#define HWIO_MSS_QDSP6SS_CPMEM_DEBUG_IN          \
        in_dword_masked(HWIO_MSS_QDSP6SS_CPMEM_DEBUG_ADDR, HWIO_MSS_QDSP6SS_CPMEM_DEBUG_RMSK)
#define HWIO_MSS_QDSP6SS_CPMEM_DEBUG_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_CPMEM_DEBUG_ADDR, m)
#define HWIO_MSS_QDSP6SS_CPMEM_DEBUG_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_CPMEM_DEBUG_ADDR,v)
#define HWIO_MSS_QDSP6SS_CPMEM_DEBUG_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_CPMEM_DEBUG_ADDR,m,v,HWIO_MSS_QDSP6SS_CPMEM_DEBUG_IN)
#define HWIO_MSS_QDSP6SS_CPMEM_DEBUG_OFF_BMSK                                       0xffffffff
#define HWIO_MSS_QDSP6SS_CPMEM_DEBUG_OFF_SHFT                                              0x0

#define HWIO_MSS_QDSP6SS_L1SMEM_DEBUG0_ADDR                                         (MSS_QDSP6V66SS_PUB_REG_BASE      + 0x00002040)
#define HWIO_MSS_QDSP6SS_L1SMEM_DEBUG0_OFFS                                         (MSS_QDSP6V66SS_PUB_REG_BASE_OFFS + 0x00002040)
#define HWIO_MSS_QDSP6SS_L1SMEM_DEBUG0_RMSK                                         0xffffffff
#define HWIO_MSS_QDSP6SS_L1SMEM_DEBUG0_IN          \
        in_dword_masked(HWIO_MSS_QDSP6SS_L1SMEM_DEBUG0_ADDR, HWIO_MSS_QDSP6SS_L1SMEM_DEBUG0_RMSK)
#define HWIO_MSS_QDSP6SS_L1SMEM_DEBUG0_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_L1SMEM_DEBUG0_ADDR, m)
#define HWIO_MSS_QDSP6SS_L1SMEM_DEBUG0_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_L1SMEM_DEBUG0_ADDR,v)
#define HWIO_MSS_QDSP6SS_L1SMEM_DEBUG0_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_L1SMEM_DEBUG0_ADDR,m,v,HWIO_MSS_QDSP6SS_L1SMEM_DEBUG0_IN)
#define HWIO_MSS_QDSP6SS_L1SMEM_DEBUG0_OFF_BMSK                                     0xffffffff
#define HWIO_MSS_QDSP6SS_L1SMEM_DEBUG0_OFF_SHFT                                            0x0

#define HWIO_MSS_QDSP6SS_L1SMEM_DEBUG1_ADDR                                         (MSS_QDSP6V66SS_PUB_REG_BASE      + 0x00002044)
#define HWIO_MSS_QDSP6SS_L1SMEM_DEBUG1_OFFS                                         (MSS_QDSP6V66SS_PUB_REG_BASE_OFFS + 0x00002044)
#define HWIO_MSS_QDSP6SS_L1SMEM_DEBUG1_RMSK                                         0xffffffff
#define HWIO_MSS_QDSP6SS_L1SMEM_DEBUG1_IN          \
        in_dword_masked(HWIO_MSS_QDSP6SS_L1SMEM_DEBUG1_ADDR, HWIO_MSS_QDSP6SS_L1SMEM_DEBUG1_RMSK)
#define HWIO_MSS_QDSP6SS_L1SMEM_DEBUG1_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_L1SMEM_DEBUG1_ADDR, m)
#define HWIO_MSS_QDSP6SS_L1SMEM_DEBUG1_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_L1SMEM_DEBUG1_ADDR,v)
#define HWIO_MSS_QDSP6SS_L1SMEM_DEBUG1_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_L1SMEM_DEBUG1_ADDR,m,v,HWIO_MSS_QDSP6SS_L1SMEM_DEBUG1_IN)
#define HWIO_MSS_QDSP6SS_L1SMEM_DEBUG1_OFF_BMSK                                     0xffffffff
#define HWIO_MSS_QDSP6SS_L1SMEM_DEBUG1_OFF_SHFT                                            0x0

#define HWIO_MSS_QDSP6SS_L1SMEM_DEBUG2_ADDR                                         (MSS_QDSP6V66SS_PUB_REG_BASE      + 0x00002048)
#define HWIO_MSS_QDSP6SS_L1SMEM_DEBUG2_OFFS                                         (MSS_QDSP6V66SS_PUB_REG_BASE_OFFS + 0x00002048)
#define HWIO_MSS_QDSP6SS_L1SMEM_DEBUG2_RMSK                                         0xffffffff
#define HWIO_MSS_QDSP6SS_L1SMEM_DEBUG2_IN          \
        in_dword_masked(HWIO_MSS_QDSP6SS_L1SMEM_DEBUG2_ADDR, HWIO_MSS_QDSP6SS_L1SMEM_DEBUG2_RMSK)
#define HWIO_MSS_QDSP6SS_L1SMEM_DEBUG2_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_L1SMEM_DEBUG2_ADDR, m)
#define HWIO_MSS_QDSP6SS_L1SMEM_DEBUG2_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_L1SMEM_DEBUG2_ADDR,v)
#define HWIO_MSS_QDSP6SS_L1SMEM_DEBUG2_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_L1SMEM_DEBUG2_ADDR,m,v,HWIO_MSS_QDSP6SS_L1SMEM_DEBUG2_IN)
#define HWIO_MSS_QDSP6SS_L1SMEM_DEBUG2_OFF_BMSK                                     0xffffffff
#define HWIO_MSS_QDSP6SS_L1SMEM_DEBUG2_OFF_SHFT                                            0x0

/*----------------------------------------------------------------------------
 * MODULE: MSS_QDSP6V66SS_CSR
 *--------------------------------------------------------------------------*/

#define MSS_QDSP6V66SS_CSR_REG_BASE                                                      (MODEM_TOP_BASE      + 0x00100000)
#define MSS_QDSP6V66SS_CSR_REG_BASE_OFFS                                                 0x00100000

#define HWIO_MSS_QDSP6SS_INSTANCE_ID_ADDR                                                (MSS_QDSP6V66SS_CSR_REG_BASE      + 0x00000000)
#define HWIO_MSS_QDSP6SS_INSTANCE_ID_OFFS                                                (MSS_QDSP6V66SS_CSR_REG_BASE_OFFS + 0x00000000)
#define HWIO_MSS_QDSP6SS_INSTANCE_ID_RMSK                                                       0x7
#define HWIO_MSS_QDSP6SS_INSTANCE_ID_IN          \
        in_dword_masked(HWIO_MSS_QDSP6SS_INSTANCE_ID_ADDR, HWIO_MSS_QDSP6SS_INSTANCE_ID_RMSK)
#define HWIO_MSS_QDSP6SS_INSTANCE_ID_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_INSTANCE_ID_ADDR, m)
#define HWIO_MSS_QDSP6SS_INSTANCE_ID_ID_BMSK                                                    0x7
#define HWIO_MSS_QDSP6SS_INSTANCE_ID_ID_SHFT                                                    0x0

#define HWIO_MSS_QDSP6SS_CP_EFUSE_STATUS_ADDR                                            (MSS_QDSP6V66SS_CSR_REG_BASE      + 0x00000100)
#define HWIO_MSS_QDSP6SS_CP_EFUSE_STATUS_OFFS                                            (MSS_QDSP6V66SS_CSR_REG_BASE_OFFS + 0x00000100)
#define HWIO_MSS_QDSP6SS_CP_EFUSE_STATUS_RMSK                                                   0x1
#define HWIO_MSS_QDSP6SS_CP_EFUSE_STATUS_IN          \
        in_dword_masked(HWIO_MSS_QDSP6SS_CP_EFUSE_STATUS_ADDR, HWIO_MSS_QDSP6SS_CP_EFUSE_STATUS_RMSK)
#define HWIO_MSS_QDSP6SS_CP_EFUSE_STATUS_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_CP_EFUSE_STATUS_ADDR, m)
#define HWIO_MSS_QDSP6SS_CP_EFUSE_STATUS_HVX_EN_BMSK                                            0x1
#define HWIO_MSS_QDSP6SS_CP_EFUSE_STATUS_HVX_EN_SHFT                                            0x0

#define HWIO_MSS_QDSP6SS_WDOG_RESET_ADDR                                                 (MSS_QDSP6V66SS_CSR_REG_BASE      + 0x00004000)
#define HWIO_MSS_QDSP6SS_WDOG_RESET_OFFS                                                 (MSS_QDSP6V66SS_CSR_REG_BASE_OFFS + 0x00004000)
#define HWIO_MSS_QDSP6SS_WDOG_RESET_RMSK                                                        0x1
#define HWIO_MSS_QDSP6SS_WDOG_RESET_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_WDOG_RESET_ADDR,v)
#define HWIO_MSS_QDSP6SS_WDOG_RESET_WDOG_RESET_BMSK                                             0x1
#define HWIO_MSS_QDSP6SS_WDOG_RESET_WDOG_RESET_SHFT                                             0x0

#define HWIO_MSS_QDSP6SS_WDOG_CTL_ADDR                                                   (MSS_QDSP6V66SS_CSR_REG_BASE      + 0x00004004)
#define HWIO_MSS_QDSP6SS_WDOG_CTL_OFFS                                                   (MSS_QDSP6V66SS_CSR_REG_BASE_OFFS + 0x00004004)
#define HWIO_MSS_QDSP6SS_WDOG_CTL_RMSK                                                          0x7
#define HWIO_MSS_QDSP6SS_WDOG_CTL_IN          \
        in_dword_masked(HWIO_MSS_QDSP6SS_WDOG_CTL_ADDR, HWIO_MSS_QDSP6SS_WDOG_CTL_RMSK)
#define HWIO_MSS_QDSP6SS_WDOG_CTL_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_WDOG_CTL_ADDR, m)
#define HWIO_MSS_QDSP6SS_WDOG_CTL_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_WDOG_CTL_ADDR,v)
#define HWIO_MSS_QDSP6SS_WDOG_CTL_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_WDOG_CTL_ADDR,m,v,HWIO_MSS_QDSP6SS_WDOG_CTL_IN)
#define HWIO_MSS_QDSP6SS_WDOG_CTL_WDOG_TO_NMI_EN_BMSK                                           0x4
#define HWIO_MSS_QDSP6SS_WDOG_CTL_WDOG_TO_NMI_EN_SHFT                                           0x2
#define HWIO_MSS_QDSP6SS_WDOG_CTL_UNMASKED_INT_EN_BMSK                                          0x2
#define HWIO_MSS_QDSP6SS_WDOG_CTL_UNMASKED_INT_EN_SHFT                                          0x1
#define HWIO_MSS_QDSP6SS_WDOG_CTL_ENABLE_BMSK                                                   0x1
#define HWIO_MSS_QDSP6SS_WDOG_CTL_ENABLE_SHFT                                                   0x0

#define HWIO_MSS_QDSP6SS_WDOG_STATUS_ADDR                                                (MSS_QDSP6V66SS_CSR_REG_BASE      + 0x00004008)
#define HWIO_MSS_QDSP6SS_WDOG_STATUS_OFFS                                                (MSS_QDSP6V66SS_CSR_REG_BASE_OFFS + 0x00004008)
#define HWIO_MSS_QDSP6SS_WDOG_STATUS_RMSK                                                0xf81fffff
#define HWIO_MSS_QDSP6SS_WDOG_STATUS_IN          \
        in_dword_masked(HWIO_MSS_QDSP6SS_WDOG_STATUS_ADDR, HWIO_MSS_QDSP6SS_WDOG_STATUS_RMSK)
#define HWIO_MSS_QDSP6SS_WDOG_STATUS_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_WDOG_STATUS_ADDR, m)
#define HWIO_MSS_QDSP6SS_WDOG_STATUS_RESET_SYNC_STATUS_BMSK                              0x80000000
#define HWIO_MSS_QDSP6SS_WDOG_STATUS_RESET_SYNC_STATUS_SHFT                                    0x1f
#define HWIO_MSS_QDSP6SS_WDOG_STATUS_BITE_TIME_SYNC_STATUS_BMSK                          0x40000000
#define HWIO_MSS_QDSP6SS_WDOG_STATUS_BITE_TIME_SYNC_STATUS_SHFT                                0x1e
#define HWIO_MSS_QDSP6SS_WDOG_STATUS_BARK_TIME_SYNC_STATUS_BMSK                          0x20000000
#define HWIO_MSS_QDSP6SS_WDOG_STATUS_BARK_TIME_SYNC_STATUS_SHFT                                0x1d
#define HWIO_MSS_QDSP6SS_WDOG_STATUS_NMI_TIME_SYNC_STATUS_BMSK                           0x10000000
#define HWIO_MSS_QDSP6SS_WDOG_STATUS_NMI_TIME_SYNC_STATUS_SHFT                                 0x1c
#define HWIO_MSS_QDSP6SS_WDOG_STATUS_TEST_LOAD_SYNC_STATUS_BMSK                           0x8000000
#define HWIO_MSS_QDSP6SS_WDOG_STATUS_TEST_LOAD_SYNC_STATUS_SHFT                                0x1b
#define HWIO_MSS_QDSP6SS_WDOG_STATUS_WDOG_COUNT_BMSK                                       0x1ffffe
#define HWIO_MSS_QDSP6SS_WDOG_STATUS_WDOG_COUNT_SHFT                                            0x1
#define HWIO_MSS_QDSP6SS_WDOG_STATUS_WDOG_EXPIRED_STATUS_BMSK                                   0x1
#define HWIO_MSS_QDSP6SS_WDOG_STATUS_WDOG_EXPIRED_STATUS_SHFT                                   0x0

#define HWIO_MSS_QDSP6SS_WDOG_BARK_TIME_ADDR                                             (MSS_QDSP6V66SS_CSR_REG_BASE      + 0x0000400c)
#define HWIO_MSS_QDSP6SS_WDOG_BARK_TIME_OFFS                                             (MSS_QDSP6V66SS_CSR_REG_BASE_OFFS + 0x0000400c)
#define HWIO_MSS_QDSP6SS_WDOG_BARK_TIME_RMSK                                                0xfffff
#define HWIO_MSS_QDSP6SS_WDOG_BARK_TIME_IN          \
        in_dword_masked(HWIO_MSS_QDSP6SS_WDOG_BARK_TIME_ADDR, HWIO_MSS_QDSP6SS_WDOG_BARK_TIME_RMSK)
#define HWIO_MSS_QDSP6SS_WDOG_BARK_TIME_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_WDOG_BARK_TIME_ADDR, m)
#define HWIO_MSS_QDSP6SS_WDOG_BARK_TIME_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_WDOG_BARK_TIME_ADDR,v)
#define HWIO_MSS_QDSP6SS_WDOG_BARK_TIME_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_WDOG_BARK_TIME_ADDR,m,v,HWIO_MSS_QDSP6SS_WDOG_BARK_TIME_IN)
#define HWIO_MSS_QDSP6SS_WDOG_BARK_TIME_DATA_BMSK                                           0xfffff
#define HWIO_MSS_QDSP6SS_WDOG_BARK_TIME_DATA_SHFT                                               0x0

#define HWIO_MSS_QDSP6SS_WDOG_BITE_TIME_ADDR                                             (MSS_QDSP6V66SS_CSR_REG_BASE      + 0x00004010)
#define HWIO_MSS_QDSP6SS_WDOG_BITE_TIME_OFFS                                             (MSS_QDSP6V66SS_CSR_REG_BASE_OFFS + 0x00004010)
#define HWIO_MSS_QDSP6SS_WDOG_BITE_TIME_RMSK                                                0xfffff
#define HWIO_MSS_QDSP6SS_WDOG_BITE_TIME_IN          \
        in_dword_masked(HWIO_MSS_QDSP6SS_WDOG_BITE_TIME_ADDR, HWIO_MSS_QDSP6SS_WDOG_BITE_TIME_RMSK)
#define HWIO_MSS_QDSP6SS_WDOG_BITE_TIME_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_WDOG_BITE_TIME_ADDR, m)
#define HWIO_MSS_QDSP6SS_WDOG_BITE_TIME_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_WDOG_BITE_TIME_ADDR,v)
#define HWIO_MSS_QDSP6SS_WDOG_BITE_TIME_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_WDOG_BITE_TIME_ADDR,m,v,HWIO_MSS_QDSP6SS_WDOG_BITE_TIME_IN)
#define HWIO_MSS_QDSP6SS_WDOG_BITE_TIME_DATA_BMSK                                           0xfffff
#define HWIO_MSS_QDSP6SS_WDOG_BITE_TIME_DATA_SHFT                                               0x0

#define HWIO_MSS_QDSP6SS_WDOG_NMI_TIME_ADDR                                              (MSS_QDSP6V66SS_CSR_REG_BASE      + 0x00004014)
#define HWIO_MSS_QDSP6SS_WDOG_NMI_TIME_OFFS                                              (MSS_QDSP6V66SS_CSR_REG_BASE_OFFS + 0x00004014)
#define HWIO_MSS_QDSP6SS_WDOG_NMI_TIME_RMSK                                                 0xfffff
#define HWIO_MSS_QDSP6SS_WDOG_NMI_TIME_IN          \
        in_dword_masked(HWIO_MSS_QDSP6SS_WDOG_NMI_TIME_ADDR, HWIO_MSS_QDSP6SS_WDOG_NMI_TIME_RMSK)
#define HWIO_MSS_QDSP6SS_WDOG_NMI_TIME_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_WDOG_NMI_TIME_ADDR, m)
#define HWIO_MSS_QDSP6SS_WDOG_NMI_TIME_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_WDOG_NMI_TIME_ADDR,v)
#define HWIO_MSS_QDSP6SS_WDOG_NMI_TIME_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_WDOG_NMI_TIME_ADDR,m,v,HWIO_MSS_QDSP6SS_WDOG_NMI_TIME_IN)
#define HWIO_MSS_QDSP6SS_WDOG_NMI_TIME_DATA_BMSK                                            0xfffff
#define HWIO_MSS_QDSP6SS_WDOG_NMI_TIME_DATA_SHFT                                                0x0

#define HWIO_MSS_QDSP6SS_WDOG_TEST_LOAD_ADDR                                             (MSS_QDSP6V66SS_CSR_REG_BASE      + 0x00004018)
#define HWIO_MSS_QDSP6SS_WDOG_TEST_LOAD_OFFS                                             (MSS_QDSP6V66SS_CSR_REG_BASE_OFFS + 0x00004018)
#define HWIO_MSS_QDSP6SS_WDOG_TEST_LOAD_RMSK                                                    0x1
#define HWIO_MSS_QDSP6SS_WDOG_TEST_LOAD_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_WDOG_TEST_LOAD_ADDR,v)
#define HWIO_MSS_QDSP6SS_WDOG_TEST_LOAD_LOAD_BMSK                                               0x1
#define HWIO_MSS_QDSP6SS_WDOG_TEST_LOAD_LOAD_SHFT                                               0x0

#define HWIO_MSS_QDSP6SS_WDOG_TEST_ADDR                                                  (MSS_QDSP6V66SS_CSR_REG_BASE      + 0x0000401c)
#define HWIO_MSS_QDSP6SS_WDOG_TEST_OFFS                                                  (MSS_QDSP6V66SS_CSR_REG_BASE_OFFS + 0x0000401c)
#define HWIO_MSS_QDSP6SS_WDOG_TEST_RMSK                                                    0x1fffff
#define HWIO_MSS_QDSP6SS_WDOG_TEST_IN          \
        in_dword_masked(HWIO_MSS_QDSP6SS_WDOG_TEST_ADDR, HWIO_MSS_QDSP6SS_WDOG_TEST_RMSK)
#define HWIO_MSS_QDSP6SS_WDOG_TEST_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_WDOG_TEST_ADDR, m)
#define HWIO_MSS_QDSP6SS_WDOG_TEST_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_WDOG_TEST_ADDR,v)
#define HWIO_MSS_QDSP6SS_WDOG_TEST_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_WDOG_TEST_ADDR,m,v,HWIO_MSS_QDSP6SS_WDOG_TEST_IN)
#define HWIO_MSS_QDSP6SS_WDOG_TEST_SYNC_STATUS_BMSK                                        0x100000
#define HWIO_MSS_QDSP6SS_WDOG_TEST_SYNC_STATUS_SHFT                                            0x14
#define HWIO_MSS_QDSP6SS_WDOG_TEST_LOAD_VALUE_BMSK                                          0xfffff
#define HWIO_MSS_QDSP6SS_WDOG_TEST_LOAD_VALUE_SHFT                                              0x0

#define HWIO_MSS_QDSP6SS_GLBCNT_TRIG_CFG_ADDR                                            (MSS_QDSP6V66SS_CSR_REG_BASE      + 0x00004100)
#define HWIO_MSS_QDSP6SS_GLBCNT_TRIG_CFG_OFFS                                            (MSS_QDSP6V66SS_CSR_REG_BASE_OFFS + 0x00004100)
#define HWIO_MSS_QDSP6SS_GLBCNT_TRIG_CFG_RMSK                                                   0xf
#define HWIO_MSS_QDSP6SS_GLBCNT_TRIG_CFG_IN          \
        in_dword_masked(HWIO_MSS_QDSP6SS_GLBCNT_TRIG_CFG_ADDR, HWIO_MSS_QDSP6SS_GLBCNT_TRIG_CFG_RMSK)
#define HWIO_MSS_QDSP6SS_GLBCNT_TRIG_CFG_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_GLBCNT_TRIG_CFG_ADDR, m)
#define HWIO_MSS_QDSP6SS_GLBCNT_TRIG_CFG_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_GLBCNT_TRIG_CFG_ADDR,v)
#define HWIO_MSS_QDSP6SS_GLBCNT_TRIG_CFG_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_GLBCNT_TRIG_CFG_ADDR,m,v,HWIO_MSS_QDSP6SS_GLBCNT_TRIG_CFG_IN)
#define HWIO_MSS_QDSP6SS_GLBCNT_TRIG_CFG_MAX_COUNT_BMSK                                         0xe
#define HWIO_MSS_QDSP6SS_GLBCNT_TRIG_CFG_MAX_COUNT_SHFT                                         0x1
#define HWIO_MSS_QDSP6SS_GLBCNT_TRIG_CFG_DISABLE_BMSK                                           0x1
#define HWIO_MSS_QDSP6SS_GLBCNT_TRIG_CFG_DISABLE_SHFT                                           0x0

#define HWIO_MSS_QDSP6SS_TS_m_EN_ADDR(m)                                                 (MSS_QDSP6V66SS_CSR_REG_BASE      + 0x00005000 + 0x10 * (m))
#define HWIO_MSS_QDSP6SS_TS_m_EN_OFFS(m)                                                 (MSS_QDSP6V66SS_CSR_REG_BASE_OFFS + 0x00005000 + 0x10 * (m))
#define HWIO_MSS_QDSP6SS_TS_m_EN_RMSK                                                           0x7
#define HWIO_MSS_QDSP6SS_TS_m_EN_MAXm                                                             3
#define HWIO_MSS_QDSP6SS_TS_m_EN_INI(m)        \
        in_dword_masked(HWIO_MSS_QDSP6SS_TS_m_EN_ADDR(m), HWIO_MSS_QDSP6SS_TS_m_EN_RMSK)
#define HWIO_MSS_QDSP6SS_TS_m_EN_INMI(m,mask)    \
        in_dword_masked(HWIO_MSS_QDSP6SS_TS_m_EN_ADDR(m), mask)
#define HWIO_MSS_QDSP6SS_TS_m_EN_OUTI(m,val)    \
        out_dword(HWIO_MSS_QDSP6SS_TS_m_EN_ADDR(m),val)
#define HWIO_MSS_QDSP6SS_TS_m_EN_OUTMI(m,mask,val) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_TS_m_EN_ADDR(m),mask,val,HWIO_MSS_QDSP6SS_TS_m_EN_INI(m))
#define HWIO_MSS_QDSP6SS_TS_m_EN_MODE_BMSK                                                      0x4
#define HWIO_MSS_QDSP6SS_TS_m_EN_MODE_SHFT                                                      0x2
#define HWIO_MSS_QDSP6SS_TS_m_EN_CLEAR_BMSK                                                     0x2
#define HWIO_MSS_QDSP6SS_TS_m_EN_CLEAR_SHFT                                                     0x1
#define HWIO_MSS_QDSP6SS_TS_m_EN_EN_BMSK                                                        0x1
#define HWIO_MSS_QDSP6SS_TS_m_EN_EN_SHFT                                                        0x0

#define HWIO_MSS_QDSP6SS_TS_m_STATUS_ADDR(m)                                             (MSS_QDSP6V66SS_CSR_REG_BASE      + 0x00005004 + 0x10 * (m))
#define HWIO_MSS_QDSP6SS_TS_m_STATUS_OFFS(m)                                             (MSS_QDSP6V66SS_CSR_REG_BASE_OFFS + 0x00005004 + 0x10 * (m))
#define HWIO_MSS_QDSP6SS_TS_m_STATUS_RMSK                                                     0x101
#define HWIO_MSS_QDSP6SS_TS_m_STATUS_MAXm                                                         3
#define HWIO_MSS_QDSP6SS_TS_m_STATUS_INI(m)        \
        in_dword_masked(HWIO_MSS_QDSP6SS_TS_m_STATUS_ADDR(m), HWIO_MSS_QDSP6SS_TS_m_STATUS_RMSK)
#define HWIO_MSS_QDSP6SS_TS_m_STATUS_INMI(m,mask)    \
        in_dword_masked(HWIO_MSS_QDSP6SS_TS_m_STATUS_ADDR(m), mask)
#define HWIO_MSS_QDSP6SS_TS_m_STATUS_OVERFLOW_BMSK                                            0x100
#define HWIO_MSS_QDSP6SS_TS_m_STATUS_OVERFLOW_SHFT                                              0x8
#define HWIO_MSS_QDSP6SS_TS_m_STATUS_VALID_BMSK                                                 0x1
#define HWIO_MSS_QDSP6SS_TS_m_STATUS_VALID_SHFT                                                 0x0

#define HWIO_MSS_QDSP6SS_TS_m_TIMESTAMP_L_ADDR(m)                                        (MSS_QDSP6V66SS_CSR_REG_BASE      + 0x00005008 + 0x10 * (m))
#define HWIO_MSS_QDSP6SS_TS_m_TIMESTAMP_L_OFFS(m)                                        (MSS_QDSP6V66SS_CSR_REG_BASE_OFFS + 0x00005008 + 0x10 * (m))
#define HWIO_MSS_QDSP6SS_TS_m_TIMESTAMP_L_RMSK                                           0xffffffff
#define HWIO_MSS_QDSP6SS_TS_m_TIMESTAMP_L_MAXm                                                    3
#define HWIO_MSS_QDSP6SS_TS_m_TIMESTAMP_L_INI(m)        \
        in_dword_masked(HWIO_MSS_QDSP6SS_TS_m_TIMESTAMP_L_ADDR(m), HWIO_MSS_QDSP6SS_TS_m_TIMESTAMP_L_RMSK)
#define HWIO_MSS_QDSP6SS_TS_m_TIMESTAMP_L_INMI(m,mask)    \
        in_dword_masked(HWIO_MSS_QDSP6SS_TS_m_TIMESTAMP_L_ADDR(m), mask)
#define HWIO_MSS_QDSP6SS_TS_m_TIMESTAMP_L_DATA_L_BMSK                                    0xffffffff
#define HWIO_MSS_QDSP6SS_TS_m_TIMESTAMP_L_DATA_L_SHFT                                           0x0

#define HWIO_MSS_QDSP6SS_TS_m_TIMESTAMP_H_ADDR(m)                                        (MSS_QDSP6V66SS_CSR_REG_BASE      + 0x0000500c + 0x10 * (m))
#define HWIO_MSS_QDSP6SS_TS_m_TIMESTAMP_H_OFFS(m)                                        (MSS_QDSP6V66SS_CSR_REG_BASE_OFFS + 0x0000500c + 0x10 * (m))
#define HWIO_MSS_QDSP6SS_TS_m_TIMESTAMP_H_RMSK                                             0xffffff
#define HWIO_MSS_QDSP6SS_TS_m_TIMESTAMP_H_MAXm                                                    3
#define HWIO_MSS_QDSP6SS_TS_m_TIMESTAMP_H_INI(m)        \
        in_dword_masked(HWIO_MSS_QDSP6SS_TS_m_TIMESTAMP_H_ADDR(m), HWIO_MSS_QDSP6SS_TS_m_TIMESTAMP_H_RMSK)
#define HWIO_MSS_QDSP6SS_TS_m_TIMESTAMP_H_INMI(m,mask)    \
        in_dword_masked(HWIO_MSS_QDSP6SS_TS_m_TIMESTAMP_H_ADDR(m), mask)
#define HWIO_MSS_QDSP6SS_TS_m_TIMESTAMP_H_DATA_H_BMSK                                      0xffffff
#define HWIO_MSS_QDSP6SS_TS_m_TIMESTAMP_H_DATA_H_SHFT                                           0x0

#define HWIO_MSS_QDSP6SS_TS_HWE_SEL_ADDR                                                 (MSS_QDSP6V66SS_CSR_REG_BASE      + 0x00005100)
#define HWIO_MSS_QDSP6SS_TS_HWE_SEL_OFFS                                                 (MSS_QDSP6V66SS_CSR_REG_BASE_OFFS + 0x00005100)
#define HWIO_MSS_QDSP6SS_TS_HWE_SEL_RMSK                                                       0x3f
#define HWIO_MSS_QDSP6SS_TS_HWE_SEL_IN          \
        in_dword_masked(HWIO_MSS_QDSP6SS_TS_HWE_SEL_ADDR, HWIO_MSS_QDSP6SS_TS_HWE_SEL_RMSK)
#define HWIO_MSS_QDSP6SS_TS_HWE_SEL_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_TS_HWE_SEL_ADDR, m)
#define HWIO_MSS_QDSP6SS_TS_HWE_SEL_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_TS_HWE_SEL_ADDR,v)
#define HWIO_MSS_QDSP6SS_TS_HWE_SEL_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_TS_HWE_SEL_ADDR,m,v,HWIO_MSS_QDSP6SS_TS_HWE_SEL_IN)
#define HWIO_MSS_QDSP6SS_TS_HWE_SEL_MUX_SEL_BMSK                                               0x3f
#define HWIO_MSS_QDSP6SS_TS_HWE_SEL_MUX_SEL_SHFT                                                0x0

#define HWIO_MSS_QDSP6SS_IPC_ADDR                                                        (MSS_QDSP6V66SS_CSR_REG_BASE      + 0x00008000)
#define HWIO_MSS_QDSP6SS_IPC_OFFS                                                        (MSS_QDSP6V66SS_CSR_REG_BASE_OFFS + 0x00008000)
#define HWIO_MSS_QDSP6SS_IPC_RMSK                                                        0xffffffff
#define HWIO_MSS_QDSP6SS_IPC_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_IPC_ADDR,v)
#define HWIO_MSS_QDSP6SS_IPC_INTR_BMSK                                                   0xffffffff
#define HWIO_MSS_QDSP6SS_IPC_INTR_SHFT                                                          0x0

#define HWIO_MSS_QDSP6SS_IPC1_ADDR                                                       (MSS_QDSP6V66SS_CSR_REG_BASE      + 0x00008004)
#define HWIO_MSS_QDSP6SS_IPC1_OFFS                                                       (MSS_QDSP6V66SS_CSR_REG_BASE_OFFS + 0x00008004)
#define HWIO_MSS_QDSP6SS_IPC1_RMSK                                                       0xffffffff
#define HWIO_MSS_QDSP6SS_IPC1_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_IPC1_ADDR,v)
#define HWIO_MSS_QDSP6SS_IPC1_INTR_BMSK                                                  0xffffffff
#define HWIO_MSS_QDSP6SS_IPC1_INTR_SHFT                                                         0x0

#define HWIO_MSS_QDSP6SS_BLOCK_INTR_ADDR                                                 (MSS_QDSP6V66SS_CSR_REG_BASE      + 0x0000c008)
#define HWIO_MSS_QDSP6SS_BLOCK_INTR_OFFS                                                 (MSS_QDSP6V66SS_CSR_REG_BASE_OFFS + 0x0000c008)
#define HWIO_MSS_QDSP6SS_BLOCK_INTR_RMSK                                                        0x7
#define HWIO_MSS_QDSP6SS_BLOCK_INTR_IN          \
        in_dword_masked(HWIO_MSS_QDSP6SS_BLOCK_INTR_ADDR, HWIO_MSS_QDSP6SS_BLOCK_INTR_RMSK)
#define HWIO_MSS_QDSP6SS_BLOCK_INTR_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_BLOCK_INTR_ADDR, m)
#define HWIO_MSS_QDSP6SS_BLOCK_INTR_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_BLOCK_INTR_ADDR,v)
#define HWIO_MSS_QDSP6SS_BLOCK_INTR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_BLOCK_INTR_ADDR,m,v,HWIO_MSS_QDSP6SS_BLOCK_INTR_IN)
#define HWIO_MSS_QDSP6SS_BLOCK_INTR_CLEAR_BLOCK_BMSK                                            0x4
#define HWIO_MSS_QDSP6SS_BLOCK_INTR_CLEAR_BLOCK_SHFT                                            0x2
#define HWIO_MSS_QDSP6SS_BLOCK_INTR_BLOCK_STATUS_BMSK                                           0x2
#define HWIO_MSS_QDSP6SS_BLOCK_INTR_BLOCK_STATUS_SHFT                                           0x1
#define HWIO_MSS_QDSP6SS_BLOCK_INTR_BLOCK_INTR_BMSK                                             0x1
#define HWIO_MSS_QDSP6SS_BLOCK_INTR_BLOCK_INTR_SHFT                                             0x0

#define HWIO_MSS_QDSP6SS_SLPC_CFG_ADDR                                                   (MSS_QDSP6V66SS_CSR_REG_BASE      + 0x0000c01c)
#define HWIO_MSS_QDSP6SS_SLPC_CFG_OFFS                                                   (MSS_QDSP6V66SS_CSR_REG_BASE_OFFS + 0x0000c01c)
#define HWIO_MSS_QDSP6SS_SLPC_CFG_RMSK                                                       0x7fa0
#define HWIO_MSS_QDSP6SS_SLPC_CFG_IN          \
        in_dword_masked(HWIO_MSS_QDSP6SS_SLPC_CFG_ADDR, HWIO_MSS_QDSP6SS_SLPC_CFG_RMSK)
#define HWIO_MSS_QDSP6SS_SLPC_CFG_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_SLPC_CFG_ADDR, m)
#define HWIO_MSS_QDSP6SS_SLPC_CFG_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_SLPC_CFG_ADDR,v)
#define HWIO_MSS_QDSP6SS_SLPC_CFG_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_SLPC_CFG_ADDR,m,v,HWIO_MSS_QDSP6SS_SLPC_CFG_IN)
#define HWIO_MSS_QDSP6SS_SLPC_CFG_MEM_FAST_STAGGER_BMSK                                      0x4000
#define HWIO_MSS_QDSP6SS_SLPC_CFG_MEM_FAST_STAGGER_SHFT                                         0xe
#define HWIO_MSS_QDSP6SS_SLPC_CFG_IDLE_CORE_CLK_EN_BMSK                                      0x2000
#define HWIO_MSS_QDSP6SS_SLPC_CFG_IDLE_CORE_CLK_EN_SHFT                                         0xd
#define HWIO_MSS_QDSP6SS_SLPC_CFG_EXTHW_WAKE_EN_BMSK                                         0x1000
#define HWIO_MSS_QDSP6SS_SLPC_CFG_EXTHW_WAKE_EN_SHFT                                            0xc
#define HWIO_MSS_QDSP6SS_SLPC_CFG_MEM_PU_PERI_STAGGER_BMSK                                    0x800
#define HWIO_MSS_QDSP6SS_SLPC_CFG_MEM_PU_PERI_STAGGER_SHFT                                      0xb
#define HWIO_MSS_QDSP6SS_SLPC_CFG_MEM_PD_PERI_STAGGER_BMSK                                    0x400
#define HWIO_MSS_QDSP6SS_SLPC_CFG_MEM_PD_PERI_STAGGER_SHFT                                      0xa
#define HWIO_MSS_QDSP6SS_SLPC_CFG_MEM_PU_ARRY_STAGGER_BMSK                                    0x200
#define HWIO_MSS_QDSP6SS_SLPC_CFG_MEM_PU_ARRY_STAGGER_SHFT                                      0x9
#define HWIO_MSS_QDSP6SS_SLPC_CFG_MEM_PD_ARRY_STAGGER_BMSK                                    0x100
#define HWIO_MSS_QDSP6SS_SLPC_CFG_MEM_PD_ARRY_STAGGER_SHFT                                      0x8
#define HWIO_MSS_QDSP6SS_SLPC_CFG_WAKEUP_IN_EN_BMSK                                            0x80
#define HWIO_MSS_QDSP6SS_SLPC_CFG_WAKEUP_IN_EN_SHFT                                             0x7
#define HWIO_MSS_QDSP6SS_SLPC_CFG_CLK_GATING_MODE_BMSK                                         0x20
#define HWIO_MSS_QDSP6SS_SLPC_CFG_CLK_GATING_MODE_SHFT                                          0x5

#define HWIO_MSS_QDSP6SS_RSCCTL_BR_ADDR                                                  (MSS_QDSP6V66SS_CSR_REG_BASE      + 0x0000c020)
#define HWIO_MSS_QDSP6SS_RSCCTL_BR_OFFS                                                  (MSS_QDSP6V66SS_CSR_REG_BASE_OFFS + 0x0000c020)
#define HWIO_MSS_QDSP6SS_RSCCTL_BR_RMSK                                                         0xe
#define HWIO_MSS_QDSP6SS_RSCCTL_BR_IN          \
        in_dword_masked(HWIO_MSS_QDSP6SS_RSCCTL_BR_ADDR, HWIO_MSS_QDSP6SS_RSCCTL_BR_RMSK)
#define HWIO_MSS_QDSP6SS_RSCCTL_BR_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_RSCCTL_BR_ADDR, m)
#define HWIO_MSS_QDSP6SS_RSCCTL_BR_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_RSCCTL_BR_ADDR,v)
#define HWIO_MSS_QDSP6SS_RSCCTL_BR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_RSCCTL_BR_ADDR,m,v,HWIO_MSS_QDSP6SS_RSCCTL_BR_IN)
#define HWIO_MSS_QDSP6SS_RSCCTL_BR_EVENT_BMSK                                                   0xe
#define HWIO_MSS_QDSP6SS_RSCCTL_BR_EVENT_SHFT                                                   0x1

#define HWIO_MSS_QDSP6SS_WAKEUP_CTL_ADDR                                                 (MSS_QDSP6V66SS_CSR_REG_BASE      + 0x0000c024)
#define HWIO_MSS_QDSP6SS_WAKEUP_CTL_OFFS                                                 (MSS_QDSP6V66SS_CSR_REG_BASE_OFFS + 0x0000c024)
#define HWIO_MSS_QDSP6SS_WAKEUP_CTL_RMSK                                                        0x3
#define HWIO_MSS_QDSP6SS_WAKEUP_CTL_IN          \
        in_dword_masked(HWIO_MSS_QDSP6SS_WAKEUP_CTL_ADDR, HWIO_MSS_QDSP6SS_WAKEUP_CTL_RMSK)
#define HWIO_MSS_QDSP6SS_WAKEUP_CTL_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_WAKEUP_CTL_ADDR, m)
#define HWIO_MSS_QDSP6SS_WAKEUP_CTL_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_WAKEUP_CTL_ADDR,v)
#define HWIO_MSS_QDSP6SS_WAKEUP_CTL_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_WAKEUP_CTL_ADDR,m,v,HWIO_MSS_QDSP6SS_WAKEUP_CTL_IN)
#define HWIO_MSS_QDSP6SS_WAKEUP_CTL_NMI_WAKE_EN_BMSK                                            0x2
#define HWIO_MSS_QDSP6SS_WAKEUP_CTL_NMI_WAKE_EN_SHFT                                            0x1
#define HWIO_MSS_QDSP6SS_WAKEUP_CTL_L2VIC_WAKE_EN_BMSK                                          0x1
#define HWIO_MSS_QDSP6SS_WAKEUP_CTL_L2VIC_WAKE_EN_SHFT                                          0x0

#define HWIO_MSS_QDSP6SS_L2VIC_WAKEUP_EN_ADDR                                            (MSS_QDSP6V66SS_CSR_REG_BASE      + 0x0000c028)
#define HWIO_MSS_QDSP6SS_L2VIC_WAKEUP_EN_OFFS                                            (MSS_QDSP6V66SS_CSR_REG_BASE_OFFS + 0x0000c028)
#define HWIO_MSS_QDSP6SS_L2VIC_WAKEUP_EN_RMSK                                                  0x1c
#define HWIO_MSS_QDSP6SS_L2VIC_WAKEUP_EN_IN          \
        in_dword_masked(HWIO_MSS_QDSP6SS_L2VIC_WAKEUP_EN_ADDR, HWIO_MSS_QDSP6SS_L2VIC_WAKEUP_EN_RMSK)
#define HWIO_MSS_QDSP6SS_L2VIC_WAKEUP_EN_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_L2VIC_WAKEUP_EN_ADDR, m)
#define HWIO_MSS_QDSP6SS_L2VIC_WAKEUP_EN_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_L2VIC_WAKEUP_EN_ADDR,v)
#define HWIO_MSS_QDSP6SS_L2VIC_WAKEUP_EN_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_L2VIC_WAKEUP_EN_ADDR,m,v,HWIO_MSS_QDSP6SS_L2VIC_WAKEUP_EN_IN)
#define HWIO_MSS_QDSP6SS_L2VIC_WAKEUP_EN_WAKEUP_EN_BMSK                                        0x1c
#define HWIO_MSS_QDSP6SS_L2VIC_WAKEUP_EN_WAKEUP_EN_SHFT                                         0x2

#define HWIO_MSS_QDSP6SS_WAKEUP_STATUS_ADDR                                              (MSS_QDSP6V66SS_CSR_REG_BASE      + 0x0000c02c)
#define HWIO_MSS_QDSP6SS_WAKEUP_STATUS_OFFS                                              (MSS_QDSP6V66SS_CSR_REG_BASE_OFFS + 0x0000c02c)
#define HWIO_MSS_QDSP6SS_WAKEUP_STATUS_RMSK                                                    0x1f
#define HWIO_MSS_QDSP6SS_WAKEUP_STATUS_IN          \
        in_dword_masked(HWIO_MSS_QDSP6SS_WAKEUP_STATUS_ADDR, HWIO_MSS_QDSP6SS_WAKEUP_STATUS_RMSK)
#define HWIO_MSS_QDSP6SS_WAKEUP_STATUS_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_WAKEUP_STATUS_ADDR, m)
#define HWIO_MSS_QDSP6SS_WAKEUP_STATUS_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_WAKEUP_STATUS_ADDR,v)
#define HWIO_MSS_QDSP6SS_WAKEUP_STATUS_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_WAKEUP_STATUS_ADDR,m,v,HWIO_MSS_QDSP6SS_WAKEUP_STATUS_IN)
#define HWIO_MSS_QDSP6SS_WAKEUP_STATUS_AXIS_WAKEUP_STICKY_BMSK                                 0x10
#define HWIO_MSS_QDSP6SS_WAKEUP_STATUS_AXIS_WAKEUP_STICKY_SHFT                                  0x4
#define HWIO_MSS_QDSP6SS_WAKEUP_STATUS_AXIS_WAKEUP_BMSK                                         0x8
#define HWIO_MSS_QDSP6SS_WAKEUP_STATUS_AXIS_WAKEUP_SHFT                                         0x3
#define HWIO_MSS_QDSP6SS_WAKEUP_STATUS_DBG_NMI_BMSK                                             0x4
#define HWIO_MSS_QDSP6SS_WAKEUP_STATUS_DBG_NMI_SHFT                                             0x2
#define HWIO_MSS_QDSP6SS_WAKEUP_STATUS_EXT_TRIGGER_BMSK                                         0x2
#define HWIO_MSS_QDSP6SS_WAKEUP_STATUS_EXT_TRIGGER_SHFT                                         0x1
#define HWIO_MSS_QDSP6SS_WAKEUP_STATUS_L2VIC_WAKEUP_BMSK                                        0x1
#define HWIO_MSS_QDSP6SS_WAKEUP_STATUS_L2VIC_WAKEUP_SHFT                                        0x0

#define HWIO_MSS_QDSP6SS_RSC_EVENT_ISOINTF_OVR_ADDR                                      (MSS_QDSP6V66SS_CSR_REG_BASE      + 0x0000c054)
#define HWIO_MSS_QDSP6SS_RSC_EVENT_ISOINTF_OVR_OFFS                                      (MSS_QDSP6V66SS_CSR_REG_BASE_OFFS + 0x0000c054)
#define HWIO_MSS_QDSP6SS_RSC_EVENT_ISOINTF_OVR_RMSK                                             0x7
#define HWIO_MSS_QDSP6SS_RSC_EVENT_ISOINTF_OVR_IN          \
        in_dword_masked(HWIO_MSS_QDSP6SS_RSC_EVENT_ISOINTF_OVR_ADDR, HWIO_MSS_QDSP6SS_RSC_EVENT_ISOINTF_OVR_RMSK)
#define HWIO_MSS_QDSP6SS_RSC_EVENT_ISOINTF_OVR_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_RSC_EVENT_ISOINTF_OVR_ADDR, m)
#define HWIO_MSS_QDSP6SS_RSC_EVENT_ISOINTF_OVR_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_RSC_EVENT_ISOINTF_OVR_ADDR,v)
#define HWIO_MSS_QDSP6SS_RSC_EVENT_ISOINTF_OVR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_RSC_EVENT_ISOINTF_OVR_ADDR,m,v,HWIO_MSS_QDSP6SS_RSC_EVENT_ISOINTF_OVR_IN)
#define HWIO_MSS_QDSP6SS_RSC_EVENT_ISOINTF_OVR_AXIM2_HALTACK_OVRRD_BMSK                         0x4
#define HWIO_MSS_QDSP6SS_RSC_EVENT_ISOINTF_OVR_AXIM2_HALTACK_OVRRD_SHFT                         0x2
#define HWIO_MSS_QDSP6SS_RSC_EVENT_ISOINTF_OVR_AXIM_HALTACK_OVRRD_BMSK                          0x2
#define HWIO_MSS_QDSP6SS_RSC_EVENT_ISOINTF_OVR_AXIM_HALTACK_OVRRD_SHFT                          0x1
#define HWIO_MSS_QDSP6SS_RSC_EVENT_ISOINTF_OVR_ALL_HALTACK_OVRRD_BMSK                           0x1
#define HWIO_MSS_QDSP6SS_RSC_EVENT_ISOINTF_OVR_ALL_HALTACK_OVRRD_SHFT                           0x0

#define HWIO_MSS_QDSP6SS_RSC_EVENT_PRSC_OVR_ADDR                                         (MSS_QDSP6V66SS_CSR_REG_BASE      + 0x0000c058)
#define HWIO_MSS_QDSP6SS_RSC_EVENT_PRSC_OVR_OFFS                                         (MSS_QDSP6V66SS_CSR_REG_BASE_OFFS + 0x0000c058)
#define HWIO_MSS_QDSP6SS_RSC_EVENT_PRSC_OVR_RMSK                                             0x3fff
#define HWIO_MSS_QDSP6SS_RSC_EVENT_PRSC_OVR_IN          \
        in_dword_masked(HWIO_MSS_QDSP6SS_RSC_EVENT_PRSC_OVR_ADDR, HWIO_MSS_QDSP6SS_RSC_EVENT_PRSC_OVR_RMSK)
#define HWIO_MSS_QDSP6SS_RSC_EVENT_PRSC_OVR_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_RSC_EVENT_PRSC_OVR_ADDR, m)
#define HWIO_MSS_QDSP6SS_RSC_EVENT_PRSC_OVR_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_RSC_EVENT_PRSC_OVR_ADDR,v)
#define HWIO_MSS_QDSP6SS_RSC_EVENT_PRSC_OVR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_RSC_EVENT_PRSC_OVR_ADDR,m,v,HWIO_MSS_QDSP6SS_RSC_EVENT_PRSC_OVR_IN)
#define HWIO_MSS_QDSP6SS_RSC_EVENT_PRSC_OVR_SPARE_OVRRD_BMSK                                 0x3c00
#define HWIO_MSS_QDSP6SS_RSC_EVENT_PRSC_OVR_SPARE_OVRRD_SHFT                                    0xa
#define HWIO_MSS_QDSP6SS_RSC_EVENT_PRSC_OVR_TCS_HW_ACK_OVRRD_BMSK                             0x3fc
#define HWIO_MSS_QDSP6SS_RSC_EVENT_PRSC_OVR_TCS_HW_ACK_OVRRD_SHFT                               0x2
#define HWIO_MSS_QDSP6SS_RSC_EVENT_PRSC_OVR_BRINGUP_ACK_OVRRD_BMSK                              0x2
#define HWIO_MSS_QDSP6SS_RSC_EVENT_PRSC_OVR_BRINGUP_ACK_OVRRD_SHFT                              0x1
#define HWIO_MSS_QDSP6SS_RSC_EVENT_PRSC_OVR_SHUTDOWN_ACK_OVRRD_BMSK                             0x1
#define HWIO_MSS_QDSP6SS_RSC_EVENT_PRSC_OVR_SHUTDOWN_ACK_OVRRD_SHFT                             0x0

#define HWIO_MSS_QDSP6SS_RSC_EVENT_PLL_OVR_ADDR                                          (MSS_QDSP6V66SS_CSR_REG_BASE      + 0x0000c05c)
#define HWIO_MSS_QDSP6SS_RSC_EVENT_PLL_OVR_OFFS                                          (MSS_QDSP6V66SS_CSR_REG_BASE_OFFS + 0x0000c05c)
#define HWIO_MSS_QDSP6SS_RSC_EVENT_PLL_OVR_RMSK                                                 0x3
#define HWIO_MSS_QDSP6SS_RSC_EVENT_PLL_OVR_IN          \
        in_dword_masked(HWIO_MSS_QDSP6SS_RSC_EVENT_PLL_OVR_ADDR, HWIO_MSS_QDSP6SS_RSC_EVENT_PLL_OVR_RMSK)
#define HWIO_MSS_QDSP6SS_RSC_EVENT_PLL_OVR_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_RSC_EVENT_PLL_OVR_ADDR, m)
#define HWIO_MSS_QDSP6SS_RSC_EVENT_PLL_OVR_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_RSC_EVENT_PLL_OVR_ADDR,v)
#define HWIO_MSS_QDSP6SS_RSC_EVENT_PLL_OVR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_RSC_EVENT_PLL_OVR_ADDR,m,v,HWIO_MSS_QDSP6SS_RSC_EVENT_PLL_OVR_IN)
#define HWIO_MSS_QDSP6SS_RSC_EVENT_PLL_OVR_PLL_FINE_OVRRD_BMSK                                  0x2
#define HWIO_MSS_QDSP6SS_RSC_EVENT_PLL_OVR_PLL_FINE_OVRRD_SHFT                                  0x1
#define HWIO_MSS_QDSP6SS_RSC_EVENT_PLL_OVR_PLL_OVRRD_BMSK                                       0x1
#define HWIO_MSS_QDSP6SS_RSC_EVENT_PLL_OVR_PLL_OVRRD_SHFT                                       0x0

#define HWIO_MSS_QDSP6SS_RSC_EVENT_MEM_OVR_ADDR                                          (MSS_QDSP6V66SS_CSR_REG_BASE      + 0x0000c060)
#define HWIO_MSS_QDSP6SS_RSC_EVENT_MEM_OVR_OFFS                                          (MSS_QDSP6V66SS_CSR_REG_BASE_OFFS + 0x0000c060)
#define HWIO_MSS_QDSP6SS_RSC_EVENT_MEM_OVR_RMSK                                                0x3f
#define HWIO_MSS_QDSP6SS_RSC_EVENT_MEM_OVR_IN          \
        in_dword_masked(HWIO_MSS_QDSP6SS_RSC_EVENT_MEM_OVR_ADDR, HWIO_MSS_QDSP6SS_RSC_EVENT_MEM_OVR_RMSK)
#define HWIO_MSS_QDSP6SS_RSC_EVENT_MEM_OVR_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_RSC_EVENT_MEM_OVR_ADDR, m)
#define HWIO_MSS_QDSP6SS_RSC_EVENT_MEM_OVR_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_RSC_EVENT_MEM_OVR_ADDR,v)
#define HWIO_MSS_QDSP6SS_RSC_EVENT_MEM_OVR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_RSC_EVENT_MEM_OVR_ADDR,m,v,HWIO_MSS_QDSP6SS_RSC_EVENT_MEM_OVR_IN)
#define HWIO_MSS_QDSP6SS_RSC_EVENT_MEM_OVR_STAGGER_RESTORE_BMSK                                0x20
#define HWIO_MSS_QDSP6SS_RSC_EVENT_MEM_OVR_STAGGER_RESTORE_SHFT                                 0x5
#define HWIO_MSS_QDSP6SS_RSC_EVENT_MEM_OVR_REFRESH_BMSK                                        0x10
#define HWIO_MSS_QDSP6SS_RSC_EVENT_MEM_OVR_REFRESH_SHFT                                         0x4
#define HWIO_MSS_QDSP6SS_RSC_EVENT_MEM_OVR_PERIPH_PD_BMSK                                       0x8
#define HWIO_MSS_QDSP6SS_RSC_EVENT_MEM_OVR_PERIPH_PD_SHFT                                       0x3
#define HWIO_MSS_QDSP6SS_RSC_EVENT_MEM_OVR_PERIPH_READY_BMSK                                    0x4
#define HWIO_MSS_QDSP6SS_RSC_EVENT_MEM_OVR_PERIPH_READY_SHFT                                    0x2
#define HWIO_MSS_QDSP6SS_RSC_EVENT_MEM_OVR_CORE_PD_BMSK                                         0x2
#define HWIO_MSS_QDSP6SS_RSC_EVENT_MEM_OVR_CORE_PD_SHFT                                         0x1
#define HWIO_MSS_QDSP6SS_RSC_EVENT_MEM_OVR_CORE_READY_BMSK                                      0x1
#define HWIO_MSS_QDSP6SS_RSC_EVENT_MEM_OVR_CORE_READY_SHFT                                      0x0

#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_ADDR                                             (MSS_QDSP6V66SS_CSR_REG_BASE      + 0x0000c100)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_OFFS                                             (MSS_QDSP6V66SS_CSR_REG_BASE_OFFS + 0x0000c100)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_RMSK                                                 0x3fff
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_IN          \
        in_dword_masked(HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_ADDR, HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_RMSK)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_ADDR, m)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_ADDR,v)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_ADDR,m,v,HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_IN)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_MNOC_BMSK                                            0x2000
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_MNOC_SHFT                                               0xd
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_APB_BMSK                                             0x1000
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_APB_SHFT                                                0xc
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_AXIS2_BMSK                                            0x800
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_AXIS2_SHFT                                              0xb
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_CSI1_BMSK                                             0x400
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_CSI1_SHFT                                               0xa
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_CSI0_BMSK                                             0x200
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_CSI0_SHFT                                               0x9
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_ARCH_BMSK                                             0x100
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_ARCH_SHFT                                               0x8
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_CORE_BMSK                                              0x80
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_CORE_SHFT                                               0x7
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_AXIM2_BMSK                                             0x40
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_AXIM2_SHFT                                              0x6
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_AXIS_BMSK                                              0x20
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_AXIS_SHFT                                               0x5
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_ATBM_BMSK                                              0x10
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_ATBM_SHFT                                               0x4
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_AHBM_BMSK                                               0x8
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_AHBM_SHFT                                               0x3
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_AXIM_BMSK                                               0x4
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_AXIM_SHFT                                               0x2
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_ISDB_ETM_BMSK                                           0x2
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_ISDB_ETM_SHFT                                           0x1
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_RET_BMSK                                                0x1
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_RET_SHFT                                                0x0

#define HWIO_MSS_QDSP6SS_RSCCTL_EN_CLAMP_ADDR                                            (MSS_QDSP6V66SS_CSR_REG_BASE      + 0x0000c104)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_CLAMP_OFFS                                            (MSS_QDSP6V66SS_CSR_REG_BASE_OFFS + 0x0000c104)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_CLAMP_RMSK                                                   0x3
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_CLAMP_IN          \
        in_dword_masked(HWIO_MSS_QDSP6SS_RSCCTL_EN_CLAMP_ADDR, HWIO_MSS_QDSP6SS_RSCCTL_EN_CLAMP_RMSK)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_CLAMP_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_RSCCTL_EN_CLAMP_ADDR, m)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_CLAMP_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_RSCCTL_EN_CLAMP_ADDR,v)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_CLAMP_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_RSCCTL_EN_CLAMP_ADDR,m,v,HWIO_MSS_QDSP6SS_RSCCTL_EN_CLAMP_IN)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_CLAMP_CLAMP_MSS2CX_BMSK                                      0x2
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_CLAMP_CLAMP_MSS2CX_SHFT                                      0x1
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_CLAMP_CLAMP_IO_BMSK                                          0x1
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_CLAMP_CLAMP_IO_SHFT                                          0x0

#define HWIO_MSS_QDSP6SS_RSCCTL_EN_CLK_ADDR                                              (MSS_QDSP6V66SS_CSR_REG_BASE      + 0x0000c108)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_CLK_OFFS                                              (MSS_QDSP6V66SS_CSR_REG_BASE_OFFS + 0x0000c108)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_CLK_RMSK                                                   0x3ff
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_CLK_IN          \
        in_dword_masked(HWIO_MSS_QDSP6SS_RSCCTL_EN_CLK_ADDR, HWIO_MSS_QDSP6SS_RSCCTL_EN_CLK_RMSK)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_CLK_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_RSCCTL_EN_CLK_ADDR, m)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_CLK_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_RSCCTL_EN_CLK_ADDR,v)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_CLK_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_RSCCTL_EN_CLK_ADDR,m,v,HWIO_MSS_QDSP6SS_RSCCTL_EN_CLK_IN)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_CLK_AXIS2_BMSK                                             0x200
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_CLK_AXIS2_SHFT                                               0x9
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_CLK_CSI1_BMSK                                              0x100
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_CLK_CSI1_SHFT                                                0x8
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_CLK_CSI0_BMSK                                               0x80
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_CLK_CSI0_SHFT                                                0x7
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_CLK_AXIM2_BMSK                                              0x40
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_CLK_AXIM2_SHFT                                               0x6
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_CLK_AXIS_BMSK                                               0x20
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_CLK_AXIS_SHFT                                                0x5
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_CLK_ATBM_BMSK                                               0x10
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_CLK_ATBM_SHFT                                                0x4
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_CLK_AHBM_BMSK                                                0x8
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_CLK_AHBM_SHFT                                                0x3
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_CLK_AXIM_BMSK                                                0x4
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_CLK_AXIM_SHFT                                                0x2
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_CLK_APB_BMSK                                                 0x2
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_CLK_APB_SHFT                                                 0x1
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_CLK_CORE_BMSK                                                0x1
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_CLK_CORE_SHFT                                                0x0

#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_RET_ADDR                                          (MSS_QDSP6V66SS_CSR_REG_BASE      + 0x0000c10c)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_RET_OFFS                                          (MSS_QDSP6V66SS_CSR_REG_BASE_OFFS + 0x0000c10c)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_RET_RMSK                                              0xffff
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_RET_IN          \
        in_dword_masked(HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_RET_ADDR, HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_RET_RMSK)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_RET_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_RET_ADDR, m)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_RET_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_RET_ADDR,v)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_RET_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_RET_ADDR,m,v,HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_RET_IN)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_RET_SPARE_BMSK                                        0xc000
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_RET_SPARE_SHFT                                           0xe
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_RET_L2CLADE_BMSK                                      0x2000
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_RET_L2CLADE_SHFT                                         0xd
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_RET_L2PLRU_BMSK                                       0x1000
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_RET_L2PLRU_SHFT                                          0xc
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_RET_L2TAG_BMSK                                         0x800
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_RET_L2TAG_SHFT                                           0xb
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_RET_L2STBUF_BMSK                                       0x400
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_RET_L2STBUF_SHFT                                         0xa
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_RET_L1DU_TAG_BMSK                                      0x200
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_RET_L1DU_TAG_SHFT                                        0x9
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_RET_L1DU_BMSK                                          0x100
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_RET_L1DU_SHFT                                            0x8
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_RET_JU_TLB64_BMSK                                       0x80
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_RET_JU_TLB64_SHFT                                        0x7
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_RET_JU_TLB128_BMSK                                      0x40
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_RET_JU_TLB128_SHFT                                       0x6
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_RET_JU_CAM_BMSK                                         0x20
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_RET_JU_CAM_SHFT                                          0x5
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_RET_ETB_BMSK                                            0x10
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_RET_ETB_SHFT                                             0x4
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_RET_L1IU_TAG_BMSK                                        0x8
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_RET_L1IU_TAG_SHFT                                        0x3
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_RET_L1IU_PDEC_BMSK                                       0x4
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_RET_L1IU_PDEC_SHFT                                       0x2
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_RET_L1IU_BMSK                                            0x2
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_RET_L1IU_SHFT                                            0x1
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_RET_L1IU_BTB_BMSK                                        0x1
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_RET_L1IU_BTB_SHFT                                        0x0

#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_CTL_ADDR                                          (MSS_QDSP6V66SS_CSR_REG_BASE      + 0x0000c110)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_CTL_OFFS                                          (MSS_QDSP6V66SS_CSR_REG_BASE_OFFS + 0x0000c110)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_CTL_RMSK                                                 0xa
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_CTL_IN          \
        in_dword_masked(HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_CTL_ADDR, HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_CTL_RMSK)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_CTL_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_CTL_ADDR, m)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_CTL_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_CTL_ADDR,v)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_CTL_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_CTL_ADDR,m,v,HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_CTL_IN)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_CTL_CLAMP_QMC_MEM_BMSK                                   0x8
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_CTL_CLAMP_QMC_MEM_SHFT                                   0x3
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_CTL_MEM_PERIPH_BMSK                                      0x2
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_CTL_MEM_PERIPH_SHFT                                      0x1

#define HWIO_MSS_QDSP6SS_RSCCTL_EN_BHS_ADDR                                              (MSS_QDSP6V66SS_CSR_REG_BASE      + 0x0000c114)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_BHS_OFFS                                              (MSS_QDSP6V66SS_CSR_REG_BASE_OFFS + 0x0000c114)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_BHS_RMSK                                                     0x1
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_BHS_IN          \
        in_dword_masked(HWIO_MSS_QDSP6SS_RSCCTL_EN_BHS_ADDR, HWIO_MSS_QDSP6SS_RSCCTL_EN_BHS_RMSK)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_BHS_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_RSCCTL_EN_BHS_ADDR, m)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_BHS_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_RSCCTL_EN_BHS_ADDR,v)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_BHS_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_RSCCTL_EN_BHS_ADDR,m,v,HWIO_MSS_QDSP6SS_RSCCTL_EN_BHS_IN)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_BHS_BHS_BMSK                                                 0x1
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_BHS_BHS_SHFT                                                 0x0

#define HWIO_MSS_QDSP6SS_RSCCTL_EN_LDO_ADDR                                              (MSS_QDSP6V66SS_CSR_REG_BASE      + 0x0000c118)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_LDO_OFFS                                              (MSS_QDSP6V66SS_CSR_REG_BASE_OFFS + 0x0000c118)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_LDO_RMSK                                                    0x7f
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_LDO_IN          \
        in_dword_masked(HWIO_MSS_QDSP6SS_RSCCTL_EN_LDO_ADDR, HWIO_MSS_QDSP6SS_RSCCTL_EN_LDO_RMSK)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_LDO_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_RSCCTL_EN_LDO_ADDR, m)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_LDO_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_RSCCTL_EN_LDO_ADDR,v)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_LDO_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_RSCCTL_EN_LDO_ADDR,m,v,HWIO_MSS_QDSP6SS_RSCCTL_EN_LDO_IN)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_LDO_BG_OFF_BMSK                                             0x40
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_LDO_BG_OFF_SHFT                                              0x6
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_LDO_FORCEOFF_BMSK                                           0x20
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_LDO_FORCEOFF_SHFT                                            0x5
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_LDO_SPARE_BITS4_2_BMSK                                      0x1c
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_LDO_SPARE_BITS4_2_SHFT                                       0x2
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_LDO_VREF_SEL_UPDATE_BMSK                                     0x2
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_LDO_VREF_SEL_UPDATE_SHFT                                     0x1
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_LDO_BYPASS_OPEN_BMSK                                         0x1
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_LDO_BYPASS_OPEN_SHFT                                         0x0

#define HWIO_MSS_QDSP6SS_RSCCTL_EN_PLL_ADDR                                              (MSS_QDSP6V66SS_CSR_REG_BASE      + 0x0000c120)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_PLL_OFFS                                              (MSS_QDSP6V66SS_CSR_REG_BASE_OFFS + 0x0000c120)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_PLL_RMSK                                                    0x17
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_PLL_IN          \
        in_dword_masked(HWIO_MSS_QDSP6SS_RSCCTL_EN_PLL_ADDR, HWIO_MSS_QDSP6SS_RSCCTL_EN_PLL_RMSK)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_PLL_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_RSCCTL_EN_PLL_ADDR, m)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_PLL_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_RSCCTL_EN_PLL_ADDR,v)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_PLL_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_RSCCTL_EN_PLL_ADDR,m,v,HWIO_MSS_QDSP6SS_RSCCTL_EN_PLL_IN)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_PLL_PLL_STBY_BMSK                                           0x10
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_PLL_PLL_STBY_SHFT                                            0x4
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_PLL_PLL_OUTDIS_BMSK                                          0x4
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_PLL_PLL_OUTDIS_SHFT                                          0x2
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_PLL_PLL_OPMODE1_BMSK                                         0x2
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_PLL_PLL_OPMODE1_SHFT                                         0x1
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_PLL_PLL_RESET_BMSK                                           0x1
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_PLL_PLL_RESET_SHFT                                           0x0

#define HWIO_MSS_QDSP6SS_RSCCTL_EN_STATERET_ADDR                                         (MSS_QDSP6V66SS_CSR_REG_BASE      + 0x0000c124)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_STATERET_OFFS                                         (MSS_QDSP6V66SS_CSR_REG_BASE_OFFS + 0x0000c124)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_STATERET_RMSK                                                0x7
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_STATERET_IN          \
        in_dword_masked(HWIO_MSS_QDSP6SS_RSCCTL_EN_STATERET_ADDR, HWIO_MSS_QDSP6SS_RSCCTL_EN_STATERET_RMSK)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_STATERET_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_RSCCTL_EN_STATERET_ADDR, m)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_STATERET_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_RSCCTL_EN_STATERET_ADDR,v)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_STATERET_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_RSCCTL_EN_STATERET_ADDR,m,v,HWIO_MSS_QDSP6SS_RSCCTL_EN_STATERET_IN)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_STATERET_WAKE_IRQ_BMSK                                       0x4
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_STATERET_WAKE_IRQ_SHFT                                       0x2
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_STATERET_RESTORE_BMSK                                        0x2
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_STATERET_RESTORE_SHFT                                        0x1
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_STATERET_SAVE_BMSK                                           0x1
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_STATERET_SAVE_SHFT                                           0x0

#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CTL_ADDR                                      (MSS_QDSP6V66SS_CSR_REG_BASE      + 0x0000c128)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CTL_OFFS                                      (MSS_QDSP6V66SS_CSR_REG_BASE_OFFS + 0x0000c128)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CTL_RMSK                                            0x3d
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CTL_IN          \
        in_dword_masked(HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CTL_ADDR, HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CTL_RMSK)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CTL_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CTL_ADDR, m)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CTL_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CTL_ADDR,v)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CTL_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CTL_ADDR,m,v,HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CTL_IN)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CTL_AXIM2_FIFO_ARESET_BMSK                          0x20
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CTL_AXIM2_FIFO_ARESET_SHFT                           0x5
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CTL_AXIM2_HALTREQ_BMSK                              0x10
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CTL_AXIM2_HALTREQ_SHFT                               0x4
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CTL_AXIM_FIFO_ARESET_BMSK                            0x8
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CTL_AXIM_FIFO_ARESET_SHFT                            0x3
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CTL_AXIM_HALTREQ_BMSK                                0x4
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CTL_AXIM_HALTREQ_SHFT                                0x2
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CTL_BUSM_HALTREQ_BMSK                                0x1
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CTL_BUSM_HALTREQ_SHFT                                0x0

#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CLAMP_ADDR                                    (MSS_QDSP6V66SS_CSR_REG_BASE      + 0x0000c12c)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CLAMP_OFFS                                    (MSS_QDSP6V66SS_CSR_REG_BASE_OFFS + 0x0000c12c)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CLAMP_RMSK                                          0xff
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CLAMP_IN          \
        in_dword_masked(HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CLAMP_ADDR, HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CLAMP_RMSK)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CLAMP_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CLAMP_ADDR, m)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CLAMP_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CLAMP_ADDR,v)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CLAMP_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CLAMP_ADDR,m,v,HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CLAMP_IN)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CLAMP_AXIM2_CLAMP_E_CLEAR_BMSK                      0x80
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CLAMP_AXIM2_CLAMP_E_CLEAR_SHFT                       0x7
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CLAMP_AXIM2_CLAMP_E_SET_BMSK                        0x40
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CLAMP_AXIM2_CLAMP_E_SET_SHFT                         0x6
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CLAMP_AXIM2_CLAMP_L_CLEAR_BMSK                      0x20
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CLAMP_AXIM2_CLAMP_L_CLEAR_SHFT                       0x5
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CLAMP_AXIM2_CLAMP_L_SET_BMSK                        0x10
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CLAMP_AXIM2_CLAMP_L_SET_SHFT                         0x4
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CLAMP_AXIM_CLAMP_E_CLEAR_BMSK                        0x8
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CLAMP_AXIM_CLAMP_E_CLEAR_SHFT                        0x3
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CLAMP_AXIM_CLAMP_E_SET_BMSK                          0x4
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CLAMP_AXIM_CLAMP_E_SET_SHFT                          0x2
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CLAMP_AXIM_CLAMP_L_CLEAR_BMSK                        0x2
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CLAMP_AXIM_CLAMP_L_CLEAR_SHFT                        0x1
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CLAMP_AXIM_CLAMP_L_SET_BMSK                          0x1
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CLAMP_AXIM_CLAMP_L_SET_SHFT                          0x0

#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CLK_ADDR                                      (MSS_QDSP6V66SS_CSR_REG_BASE      + 0x0000c130)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CLK_OFFS                                      (MSS_QDSP6V66SS_CSR_REG_BASE_OFFS + 0x0000c130)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CLK_RMSK                                             0x7
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CLK_IN          \
        in_dword_masked(HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CLK_ADDR, HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CLK_RMSK)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CLK_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CLK_ADDR, m)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CLK_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CLK_ADDR,v)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CLK_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CLK_ADDR,m,v,HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CLK_IN)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CLK_AXIM2_BMSK                                       0x4
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CLK_AXIM2_SHFT                                       0x2
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CLK_AXIM_BMSK                                        0x2
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CLK_AXIM_SHFT                                        0x1
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CLK_CORE_BMSK                                        0x1
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CLK_CORE_SHFT                                        0x0

#define HWIO_MSS_QDSP6SS_RSCCTL_EN_L2MEM_RET_ADDR                                        (MSS_QDSP6V66SS_CSR_REG_BASE      + 0x0000c134)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_L2MEM_RET_OFFS                                        (MSS_QDSP6V66SS_CSR_REG_BASE_OFFS + 0x0000c134)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_L2MEM_RET_RMSK                                               0xf
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_L2MEM_RET_IN          \
        in_dword_masked(HWIO_MSS_QDSP6SS_RSCCTL_EN_L2MEM_RET_ADDR, HWIO_MSS_QDSP6SS_RSCCTL_EN_L2MEM_RET_RMSK)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_L2MEM_RET_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_RSCCTL_EN_L2MEM_RET_ADDR, m)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_L2MEM_RET_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_RSCCTL_EN_L2MEM_RET_ADDR,v)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_L2MEM_RET_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_RSCCTL_EN_L2MEM_RET_ADDR,m,v,HWIO_MSS_QDSP6SS_RSCCTL_EN_L2MEM_RET_IN)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_L2MEM_RET_L2DATA_BMSK                                        0xf
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_L2MEM_RET_L2DATA_SHFT                                        0x0

#define HWIO_MSS_QDSP6SS_RSCCTL_EN_PRSC_ADDR                                             (MSS_QDSP6V66SS_CSR_REG_BASE      + 0x0000c138)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_PRSC_OFFS                                             (MSS_QDSP6V66SS_CSR_REG_BASE_OFFS + 0x0000c138)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_PRSC_RMSK                                                 0x3fff
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_PRSC_IN          \
        in_dword_masked(HWIO_MSS_QDSP6SS_RSCCTL_EN_PRSC_ADDR, HWIO_MSS_QDSP6SS_RSCCTL_EN_PRSC_RMSK)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_PRSC_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_RSCCTL_EN_PRSC_ADDR, m)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_PRSC_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_RSCCTL_EN_PRSC_ADDR,v)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_PRSC_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_RSCCTL_EN_PRSC_ADDR,m,v,HWIO_MSS_QDSP6SS_RSCCTL_EN_PRSC_IN)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_PRSC_SPARE_BMSK                                           0x3c00
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_PRSC_SPARE_SHFT                                              0xa
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_PRSC_TCS_HW_TRIGGER_BMSK                                   0x3fc
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_PRSC_TCS_HW_TRIGGER_SHFT                                     0x2
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_PRSC_BRINGUP_REQ_BMSK                                        0x2
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_PRSC_BRINGUP_REQ_SHFT                                        0x1
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_PRSC_SHUTDOWN_REQ_BMSK                                       0x1
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_PRSC_SHUTDOWN_REQ_SHFT                                       0x0

#define HWIO_MSS_QDSP6SS_RSCCTL_EN_L1SMEM_RET_ADDR                                       (MSS_QDSP6V66SS_CSR_REG_BASE      + 0x0000c140)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_L1SMEM_RET_OFFS                                       (MSS_QDSP6V66SS_CSR_REG_BASE_OFFS + 0x0000c140)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_L1SMEM_RET_RMSK                                             0x3f
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_L1SMEM_RET_IN          \
        in_dword_masked(HWIO_MSS_QDSP6SS_RSCCTL_EN_L1SMEM_RET_ADDR, HWIO_MSS_QDSP6SS_RSCCTL_EN_L1SMEM_RET_RMSK)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_L1SMEM_RET_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_RSCCTL_EN_L1SMEM_RET_ADDR, m)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_L1SMEM_RET_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_RSCCTL_EN_L1SMEM_RET_ADDR,v)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_L1SMEM_RET_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_RSCCTL_EN_L1SMEM_RET_ADDR,m,v,HWIO_MSS_QDSP6SS_RSCCTL_EN_L1SMEM_RET_IN)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_L1SMEM_RET_L1SDATA_BMSK                                     0x3f
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_L1SMEM_RET_L1SDATA_SHFT                                      0x0

#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_STAGGER_ADDR                                      (MSS_QDSP6V66SS_CSR_REG_BASE      + 0x0000c144)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_STAGGER_OFFS                                      (MSS_QDSP6V66SS_CSR_REG_BASE_OFFS + 0x0000c144)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_STAGGER_RMSK                                             0x7
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_STAGGER_IN          \
        in_dword_masked(HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_STAGGER_ADDR, HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_STAGGER_RMSK)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_STAGGER_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_STAGGER_ADDR, m)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_STAGGER_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_STAGGER_ADDR,v)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_STAGGER_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_STAGGER_ADDR,m,v,HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_STAGGER_IN)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_STAGGER_STAGGER_RESTORE_BMSK                             0x4
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_STAGGER_STAGGER_RESTORE_SHFT                             0x2
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_STAGGER_STAGGER_ALT_ARES_BMSK                            0x2
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_STAGGER_STAGGER_ALT_ARES_SHFT                            0x1
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_STAGGER_STAGGER_FUNC_ARES_BMSK                           0x1
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_STAGGER_STAGGER_FUNC_ARES_SHFT                           0x0

#define HWIO_MSS_QDSP6SS_RSCCTL_EN_VTCMEM_RET_ADDR                                       (MSS_QDSP6V66SS_CSR_REG_BASE      + 0x0000c148)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_VTCMEM_RET_OFFS                                       (MSS_QDSP6V66SS_CSR_REG_BASE_OFFS + 0x0000c148)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_VTCMEM_RET_RMSK                                              0x1
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_VTCMEM_RET_IN          \
        in_dword_masked(HWIO_MSS_QDSP6SS_RSCCTL_EN_VTCMEM_RET_ADDR, HWIO_MSS_QDSP6SS_RSCCTL_EN_VTCMEM_RET_RMSK)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_VTCMEM_RET_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_RSCCTL_EN_VTCMEM_RET_ADDR, m)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_VTCMEM_RET_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_RSCCTL_EN_VTCMEM_RET_ADDR,v)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_VTCMEM_RET_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_RSCCTL_EN_VTCMEM_RET_ADDR,m,v,HWIO_MSS_QDSP6SS_RSCCTL_EN_VTCMEM_RET_IN)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_VTCMEM_RET_VTCM_BMSK                                         0x1
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_VTCMEM_RET_VTCM_SHFT                                         0x0

#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_SET_ADDR                                         (MSS_QDSP6V66SS_CSR_REG_BASE      + 0x0000c180)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_SET_OFFS                                         (MSS_QDSP6V66SS_CSR_REG_BASE_OFFS + 0x0000c180)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_SET_RMSK                                             0x3fff
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_SET_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_SET_ADDR,v)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_SET_MNOC_BMSK                                        0x2000
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_SET_MNOC_SHFT                                           0xd
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_SET_APB_BMSK                                         0x1000
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_SET_APB_SHFT                                            0xc
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_SET_AXIS2_BMSK                                        0x800
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_SET_AXIS2_SHFT                                          0xb
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_SET_CSI1_BMSK                                         0x400
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_SET_CSI1_SHFT                                           0xa
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_SET_CSI0_BMSK                                         0x200
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_SET_CSI0_SHFT                                           0x9
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_SET_ARCH_BMSK                                         0x100
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_SET_ARCH_SHFT                                           0x8
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_SET_CORE_BMSK                                          0x80
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_SET_CORE_SHFT                                           0x7
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_SET_AXIM2_BMSK                                         0x40
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_SET_AXIM2_SHFT                                          0x6
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_SET_AXIS_BMSK                                          0x20
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_SET_AXIS_SHFT                                           0x5
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_SET_ATBM_BMSK                                          0x10
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_SET_ATBM_SHFT                                           0x4
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_SET_AHBM_BMSK                                           0x8
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_SET_AHBM_SHFT                                           0x3
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_SET_AXIM_BMSK                                           0x4
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_SET_AXIM_SHFT                                           0x2
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_SET_ISDB_ETM_BMSK                                       0x2
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_SET_ISDB_ETM_SHFT                                       0x1
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_SET_RET_BMSK                                            0x1
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_SET_RET_SHFT                                            0x0

#define HWIO_MSS_QDSP6SS_RSCCTL_EN_CLAMP_SET_ADDR                                        (MSS_QDSP6V66SS_CSR_REG_BASE      + 0x0000c184)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_CLAMP_SET_OFFS                                        (MSS_QDSP6V66SS_CSR_REG_BASE_OFFS + 0x0000c184)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_CLAMP_SET_RMSK                                               0x3
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_CLAMP_SET_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_RSCCTL_EN_CLAMP_SET_ADDR,v)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_CLAMP_SET_CLAMP_MSS2CX_BMSK                                  0x2
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_CLAMP_SET_CLAMP_MSS2CX_SHFT                                  0x1
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_CLAMP_SET_CLAMP_IO_BMSK                                      0x1
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_CLAMP_SET_CLAMP_IO_SHFT                                      0x0

#define HWIO_MSS_QDSP6SS_RSCCTL_EN_CLK_SET_ADDR                                          (MSS_QDSP6V66SS_CSR_REG_BASE      + 0x0000c188)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_CLK_SET_OFFS                                          (MSS_QDSP6V66SS_CSR_REG_BASE_OFFS + 0x0000c188)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_CLK_SET_RMSK                                               0x3ff
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_CLK_SET_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_RSCCTL_EN_CLK_SET_ADDR,v)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_CLK_SET_AXIS2_BMSK                                         0x200
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_CLK_SET_AXIS2_SHFT                                           0x9
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_CLK_SET_CSI1_BMSK                                          0x100
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_CLK_SET_CSI1_SHFT                                            0x8
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_CLK_SET_CSI0_BMSK                                           0x80
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_CLK_SET_CSI0_SHFT                                            0x7
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_CLK_SET_AXIM2_BMSK                                          0x40
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_CLK_SET_AXIM2_SHFT                                           0x6
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_CLK_SET_AXIS_BMSK                                           0x20
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_CLK_SET_AXIS_SHFT                                            0x5
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_CLK_SET_ATBM_BMSK                                           0x10
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_CLK_SET_ATBM_SHFT                                            0x4
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_CLK_SET_AHBM_BMSK                                            0x8
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_CLK_SET_AHBM_SHFT                                            0x3
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_CLK_SET_AXIM_BMSK                                            0x4
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_CLK_SET_AXIM_SHFT                                            0x2
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_CLK_SET_APB_BMSK                                             0x2
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_CLK_SET_APB_SHFT                                             0x1
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_CLK_SET_CORE_BMSK                                            0x1
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_CLK_SET_CORE_SHFT                                            0x0

#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_RET_SET_ADDR                                      (MSS_QDSP6V66SS_CSR_REG_BASE      + 0x0000c18c)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_RET_SET_OFFS                                      (MSS_QDSP6V66SS_CSR_REG_BASE_OFFS + 0x0000c18c)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_RET_SET_RMSK                                          0xffff
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_RET_SET_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_RET_SET_ADDR,v)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_RET_SET_SPARE_BMSK                                    0xc000
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_RET_SET_SPARE_SHFT                                       0xe
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_RET_SET_L2CLADE_BMSK                                  0x2000
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_RET_SET_L2CLADE_SHFT                                     0xd
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_RET_SET_L2PLRU_BMSK                                   0x1000
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_RET_SET_L2PLRU_SHFT                                      0xc
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_RET_SET_L2TAG_BMSK                                     0x800
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_RET_SET_L2TAG_SHFT                                       0xb
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_RET_SET_L2STBUF_BMSK                                   0x400
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_RET_SET_L2STBUF_SHFT                                     0xa
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_RET_SET_L1DU_TAG_BMSK                                  0x200
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_RET_SET_L1DU_TAG_SHFT                                    0x9
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_RET_SET_L1DU_BMSK                                      0x100
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_RET_SET_L1DU_SHFT                                        0x8
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_RET_SET_JU_TLB64_BMSK                                   0x80
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_RET_SET_JU_TLB64_SHFT                                    0x7
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_RET_SET_JU_TLB128_BMSK                                  0x40
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_RET_SET_JU_TLB128_SHFT                                   0x6
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_RET_SET_JU_CAM_BMSK                                     0x20
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_RET_SET_JU_CAM_SHFT                                      0x5
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_RET_SET_ETB_BMSK                                        0x10
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_RET_SET_ETB_SHFT                                         0x4
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_RET_SET_L1IU_TAG_BMSK                                    0x8
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_RET_SET_L1IU_TAG_SHFT                                    0x3
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_RET_SET_L1IU_PDEC_BMSK                                   0x4
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_RET_SET_L1IU_PDEC_SHFT                                   0x2
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_RET_SET_L1IU_BMSK                                        0x2
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_RET_SET_L1IU_SHFT                                        0x1
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_RET_SET_L1IU_BTB_BMSK                                    0x1
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_RET_SET_L1IU_BTB_SHFT                                    0x0

#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_CTL_SET_ADDR                                      (MSS_QDSP6V66SS_CSR_REG_BASE      + 0x0000c190)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_CTL_SET_OFFS                                      (MSS_QDSP6V66SS_CSR_REG_BASE_OFFS + 0x0000c190)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_CTL_SET_RMSK                                             0xa
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_CTL_SET_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_CTL_SET_ADDR,v)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_CTL_SET_CLAMP_QMC_MEM_BMSK                               0x8
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_CTL_SET_CLAMP_QMC_MEM_SHFT                               0x3
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_CTL_SET_MEM_PERIPH_BMSK                                  0x2
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_CTL_SET_MEM_PERIPH_SHFT                                  0x1

#define HWIO_MSS_QDSP6SS_RSCCTL_EN_BHS_SET_ADDR                                          (MSS_QDSP6V66SS_CSR_REG_BASE      + 0x0000c194)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_BHS_SET_OFFS                                          (MSS_QDSP6V66SS_CSR_REG_BASE_OFFS + 0x0000c194)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_BHS_SET_RMSK                                                 0x1
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_BHS_SET_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_RSCCTL_EN_BHS_SET_ADDR,v)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_BHS_SET_BHS_BMSK                                             0x1
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_BHS_SET_BHS_SHFT                                             0x0

#define HWIO_MSS_QDSP6SS_RSCCTL_EN_LDO_SET_ADDR                                          (MSS_QDSP6V66SS_CSR_REG_BASE      + 0x0000c198)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_LDO_SET_OFFS                                          (MSS_QDSP6V66SS_CSR_REG_BASE_OFFS + 0x0000c198)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_LDO_SET_RMSK                                                0x7f
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_LDO_SET_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_RSCCTL_EN_LDO_SET_ADDR,v)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_LDO_SET_BG_OFF_BMSK                                         0x40
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_LDO_SET_BG_OFF_SHFT                                          0x6
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_LDO_SET_FORCEOFF_BMSK                                       0x20
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_LDO_SET_FORCEOFF_SHFT                                        0x5
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_LDO_SET_SPARE_BITS4_2_BMSK                                  0x1c
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_LDO_SET_SPARE_BITS4_2_SHFT                                   0x2
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_LDO_SET_UPDATE_BMSK                                          0x2
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_LDO_SET_UPDATE_SHFT                                          0x1
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_LDO_SET_BYPASS_OPEN_BMSK                                     0x1
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_LDO_SET_BYPASS_OPEN_SHFT                                     0x0

#define HWIO_MSS_QDSP6SS_RSCCTL_EN_PLL_SET_ADDR                                          (MSS_QDSP6V66SS_CSR_REG_BASE      + 0x0000c1a0)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_PLL_SET_OFFS                                          (MSS_QDSP6V66SS_CSR_REG_BASE_OFFS + 0x0000c1a0)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_PLL_SET_RMSK                                                0x17
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_PLL_SET_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_RSCCTL_EN_PLL_SET_ADDR,v)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_PLL_SET_PLL_STBY_BMSK                                       0x10
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_PLL_SET_PLL_STBY_SHFT                                        0x4
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_PLL_SET_PLL_OUTDIS_BMSK                                      0x4
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_PLL_SET_PLL_OUTDIS_SHFT                                      0x2
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_PLL_SET_PLL_OPMODE1_BMSK                                     0x2
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_PLL_SET_PLL_OPMODE1_SHFT                                     0x1
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_PLL_SET_PLL_RESET_BMSK                                       0x1
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_PLL_SET_PLL_RESET_SHFT                                       0x0

#define HWIO_MSS_QDSP6SS_RSCCTL_EN_STATERET_SET_ADDR                                     (MSS_QDSP6V66SS_CSR_REG_BASE      + 0x0000c1a4)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_STATERET_SET_OFFS                                     (MSS_QDSP6V66SS_CSR_REG_BASE_OFFS + 0x0000c1a4)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_STATERET_SET_RMSK                                            0x7
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_STATERET_SET_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_RSCCTL_EN_STATERET_SET_ADDR,v)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_STATERET_SET_WAKE_IRQ_BMSK                                   0x4
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_STATERET_SET_WAKE_IRQ_SHFT                                   0x2
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_STATERET_SET_RESTORE_BMSK                                    0x2
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_STATERET_SET_RESTORE_SHFT                                    0x1
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_STATERET_SET_SAVE_BMSK                                       0x1
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_STATERET_SET_SAVE_SHFT                                       0x0

#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CTL_SET_ADDR                                  (MSS_QDSP6V66SS_CSR_REG_BASE      + 0x0000c1a8)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CTL_SET_OFFS                                  (MSS_QDSP6V66SS_CSR_REG_BASE_OFFS + 0x0000c1a8)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CTL_SET_RMSK                                        0x3d
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CTL_SET_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CTL_SET_ADDR,v)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CTL_SET_AXIM2_FIFO_ARESET_BMSK                      0x20
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CTL_SET_AXIM2_FIFO_ARESET_SHFT                       0x5
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CTL_SET_AXIM2_HALTREQ_BMSK                          0x10
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CTL_SET_AXIM2_HALTREQ_SHFT                           0x4
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CTL_SET_AXIM_FIFO_ARESET_BMSK                        0x8
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CTL_SET_AXIM_FIFO_ARESET_SHFT                        0x3
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CTL_SET_AXIM_HALTREQ_BMSK                            0x4
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CTL_SET_AXIM_HALTREQ_SHFT                            0x2
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CTL_SET_BUSM_HALTREQ_BMSK                            0x1
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CTL_SET_BUSM_HALTREQ_SHFT                            0x0

#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CLAMP_SET_ADDR                                (MSS_QDSP6V66SS_CSR_REG_BASE      + 0x0000c1ac)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CLAMP_SET_OFFS                                (MSS_QDSP6V66SS_CSR_REG_BASE_OFFS + 0x0000c1ac)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CLAMP_SET_RMSK                                      0xff
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CLAMP_SET_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CLAMP_SET_ADDR,v)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CLAMP_SET_AXIM2_CLAMP_E_CLEAR_BMSK                  0x80
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CLAMP_SET_AXIM2_CLAMP_E_CLEAR_SHFT                   0x7
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CLAMP_SET_AXIM2_CLAMP_E_SET_BMSK                    0x40
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CLAMP_SET_AXIM2_CLAMP_E_SET_SHFT                     0x6
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CLAMP_SET_AXIM2_CLAMP_L_CLEAR_BMSK                  0x20
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CLAMP_SET_AXIM2_CLAMP_L_CLEAR_SHFT                   0x5
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CLAMP_SET_AXIM2_CLAMP_L_SET_BMSK                    0x10
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CLAMP_SET_AXIM2_CLAMP_L_SET_SHFT                     0x4
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CLAMP_SET_AXIM_CLAMP_E_CLEAR_BMSK                    0x8
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CLAMP_SET_AXIM_CLAMP_E_CLEAR_SHFT                    0x3
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CLAMP_SET_AXIM_CLAMP_E_SET_BMSK                      0x4
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CLAMP_SET_AXIM_CLAMP_E_SET_SHFT                      0x2
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CLAMP_SET_AXIM_CLAMP_L_CLEAR_BMSK                    0x2
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CLAMP_SET_AXIM_CLAMP_L_CLEAR_SHFT                    0x1
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CLAMP_SET_AXIM_CLAMP_L_SET_BMSK                      0x1
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CLAMP_SET_AXIM_CLAMP_L_SET_SHFT                      0x0

#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CLK_SET_ADDR                                  (MSS_QDSP6V66SS_CSR_REG_BASE      + 0x0000c1b0)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CLK_SET_OFFS                                  (MSS_QDSP6V66SS_CSR_REG_BASE_OFFS + 0x0000c1b0)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CLK_SET_RMSK                                         0x7
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CLK_SET_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CLK_SET_ADDR,v)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CLK_SET_AXIM2_BMSK                                   0x4
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CLK_SET_AXIM2_SHFT                                   0x2
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CLK_SET_AXIM_BMSK                                    0x2
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CLK_SET_AXIM_SHFT                                    0x1
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CLK_SET_CORE_BMSK                                    0x1
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CLK_SET_CORE_SHFT                                    0x0

#define HWIO_MSS_QDSP6SS_RSCCTL_EN_L2MEM_RET_SET_ADDR                                    (MSS_QDSP6V66SS_CSR_REG_BASE      + 0x0000c1b4)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_L2MEM_RET_SET_OFFS                                    (MSS_QDSP6V66SS_CSR_REG_BASE_OFFS + 0x0000c1b4)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_L2MEM_RET_SET_RMSK                                           0xf
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_L2MEM_RET_SET_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_RSCCTL_EN_L2MEM_RET_SET_ADDR,v)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_L2MEM_RET_SET_L2DATA_BMSK                                    0xf
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_L2MEM_RET_SET_L2DATA_SHFT                                    0x0

#define HWIO_MSS_QDSP6SS_RSCCTL_EN_PRSC_SET_ADDR                                         (MSS_QDSP6V66SS_CSR_REG_BASE      + 0x0000c1b8)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_PRSC_SET_OFFS                                         (MSS_QDSP6V66SS_CSR_REG_BASE_OFFS + 0x0000c1b8)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_PRSC_SET_RMSK                                             0x3fff
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_PRSC_SET_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_RSCCTL_EN_PRSC_SET_ADDR,v)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_PRSC_SET_SPARE_BMSK                                       0x3c00
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_PRSC_SET_SPARE_SHFT                                          0xa
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_PRSC_SET_TCS_HW_TRIGGER_BMSK                               0x3fc
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_PRSC_SET_TCS_HW_TRIGGER_SHFT                                 0x2
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_PRSC_SET_BRINGUP_REQ_BMSK                                    0x2
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_PRSC_SET_BRINGUP_REQ_SHFT                                    0x1
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_PRSC_SET_SHUTDOWN_REQ_BMSK                                   0x1
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_PRSC_SET_SHUTDOWN_REQ_SHFT                                   0x0

#define HWIO_MSS_QDSP6SS_RSCCTL_EN_L1SMEM_RET_SET_ADDR                                   (MSS_QDSP6V66SS_CSR_REG_BASE      + 0x0000c1c0)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_L1SMEM_RET_SET_OFFS                                   (MSS_QDSP6V66SS_CSR_REG_BASE_OFFS + 0x0000c1c0)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_L1SMEM_RET_SET_RMSK                                         0x3f
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_L1SMEM_RET_SET_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_RSCCTL_EN_L1SMEM_RET_SET_ADDR,v)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_L1SMEM_RET_SET_L1SDATA_BMSK                                 0x3f
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_L1SMEM_RET_SET_L1SDATA_SHFT                                  0x0

#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_STAGGER_SET_ADDR                                  (MSS_QDSP6V66SS_CSR_REG_BASE      + 0x0000c1c4)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_STAGGER_SET_OFFS                                  (MSS_QDSP6V66SS_CSR_REG_BASE_OFFS + 0x0000c1c4)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_STAGGER_SET_RMSK                                         0x7
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_STAGGER_SET_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_STAGGER_SET_ADDR,v)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_STAGGER_SET_STAGGER_RESTORE_BMSK                         0x4
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_STAGGER_SET_STAGGER_RESTORE_SHFT                         0x2
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_STAGGER_SET_STAGGER_ALT_ARES_BMSK                        0x2
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_STAGGER_SET_STAGGER_ALT_ARES_SHFT                        0x1
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_STAGGER_SET_STAGGER_FUNC_ARES_BMSK                       0x1
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_STAGGER_SET_STAGGER_FUNC_ARES_SHFT                       0x0

#define HWIO_MSS_QDSP6SS_RSCCTL_EN_VTCMEM_RET_SET_ADDR                                   (MSS_QDSP6V66SS_CSR_REG_BASE      + 0x0000c1c8)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_VTCMEM_RET_SET_OFFS                                   (MSS_QDSP6V66SS_CSR_REG_BASE_OFFS + 0x0000c1c8)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_VTCMEM_RET_SET_RMSK                                          0x1
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_VTCMEM_RET_SET_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_RSCCTL_EN_VTCMEM_RET_SET_ADDR,v)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_VTCMEM_RET_SET_VTCM_BMSK                                     0x1
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_VTCMEM_RET_SET_VTCM_SHFT                                     0x0

#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_CLEAR_ADDR                                       (MSS_QDSP6V66SS_CSR_REG_BASE      + 0x0000c200)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_CLEAR_OFFS                                       (MSS_QDSP6V66SS_CSR_REG_BASE_OFFS + 0x0000c200)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_CLEAR_RMSK                                           0x3fff
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_CLEAR_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_CLEAR_ADDR,v)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_CLEAR_MNOC_BMSK                                      0x2000
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_CLEAR_MNOC_SHFT                                         0xd
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_CLEAR_APB_BMSK                                       0x1000
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_CLEAR_APB_SHFT                                          0xc
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_CLEAR_AXIS2_BMSK                                      0x800
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_CLEAR_AXIS2_SHFT                                        0xb
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_CLEAR_CSI1_BMSK                                       0x400
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_CLEAR_CSI1_SHFT                                         0xa
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_CLEAR_CSI0_BMSK                                       0x200
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_CLEAR_CSI0_SHFT                                         0x9
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_CLEAR_ARCH_BMSK                                       0x100
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_CLEAR_ARCH_SHFT                                         0x8
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_CLEAR_CORE_BMSK                                        0x80
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_CLEAR_CORE_SHFT                                         0x7
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_CLEAR_AXIM2_BMSK                                       0x40
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_CLEAR_AXIM2_SHFT                                        0x6
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_CLEAR_AXIS_BMSK                                        0x20
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_CLEAR_AXIS_SHFT                                         0x5
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_CLEAR_ATBM_BMSK                                        0x10
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_CLEAR_ATBM_SHFT                                         0x4
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_CLEAR_AHBM_BMSK                                         0x8
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_CLEAR_AHBM_SHFT                                         0x3
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_CLEAR_AXIM_BMSK                                         0x4
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_CLEAR_AXIM_SHFT                                         0x2
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_CLEAR_ISDB_ETM_BMSK                                     0x2
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_CLEAR_ISDB_ETM_SHFT                                     0x1
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_CLEAR_RET_BMSK                                          0x1
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_CLEAR_RET_SHFT                                          0x0

#define HWIO_MSS_QDSP6SS_RSCCTL_EN_CLAMP_CLEAR_ADDR                                      (MSS_QDSP6V66SS_CSR_REG_BASE      + 0x0000c204)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_CLAMP_CLEAR_OFFS                                      (MSS_QDSP6V66SS_CSR_REG_BASE_OFFS + 0x0000c204)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_CLAMP_CLEAR_RMSK                                             0x3
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_CLAMP_CLEAR_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_RSCCTL_EN_CLAMP_CLEAR_ADDR,v)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_CLAMP_CLEAR_CLAMP_MSS2CX_BMSK                                0x2
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_CLAMP_CLEAR_CLAMP_MSS2CX_SHFT                                0x1
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_CLAMP_CLEAR_CLAMP_IO_BMSK                                    0x1
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_CLAMP_CLEAR_CLAMP_IO_SHFT                                    0x0

#define HWIO_MSS_QDSP6SS_RSCCTL_EN_CLK_CLEAR_ADDR                                        (MSS_QDSP6V66SS_CSR_REG_BASE      + 0x0000c208)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_CLK_CLEAR_OFFS                                        (MSS_QDSP6V66SS_CSR_REG_BASE_OFFS + 0x0000c208)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_CLK_CLEAR_RMSK                                             0x3ff
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_CLK_CLEAR_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_RSCCTL_EN_CLK_CLEAR_ADDR,v)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_CLK_CLEAR_AXIS2_BMSK                                       0x200
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_CLK_CLEAR_AXIS2_SHFT                                         0x9
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_CLK_CLEAR_CSI1_BMSK                                        0x100
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_CLK_CLEAR_CSI1_SHFT                                          0x8
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_CLK_CLEAR_CSI0_BMSK                                         0x80
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_CLK_CLEAR_CSI0_SHFT                                          0x7
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_CLK_CLEAR_AXIM2_BMSK                                        0x40
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_CLK_CLEAR_AXIM2_SHFT                                         0x6
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_CLK_CLEAR_AXIS_BMSK                                         0x20
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_CLK_CLEAR_AXIS_SHFT                                          0x5
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_CLK_CLEAR_ATBM_BMSK                                         0x10
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_CLK_CLEAR_ATBM_SHFT                                          0x4
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_CLK_CLEAR_AHBM_BMSK                                          0x8
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_CLK_CLEAR_AHBM_SHFT                                          0x3
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_CLK_CLEAR_AXIM_BMSK                                          0x4
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_CLK_CLEAR_AXIM_SHFT                                          0x2
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_CLK_CLEAR_APB_BMSK                                           0x2
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_CLK_CLEAR_APB_SHFT                                           0x1
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_CLK_CLEAR_CORE_BMSK                                          0x1
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_CLK_CLEAR_CORE_SHFT                                          0x0

#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_RET_CLEAR_ADDR                                    (MSS_QDSP6V66SS_CSR_REG_BASE      + 0x0000c20c)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_RET_CLEAR_OFFS                                    (MSS_QDSP6V66SS_CSR_REG_BASE_OFFS + 0x0000c20c)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_RET_CLEAR_RMSK                                        0xffff
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_RET_CLEAR_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_RET_CLEAR_ADDR,v)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_RET_CLEAR_SPARE_BMSK                                  0xc000
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_RET_CLEAR_SPARE_SHFT                                     0xe
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_RET_CLEAR_L2CLADE_BMSK                                0x2000
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_RET_CLEAR_L2CLADE_SHFT                                   0xd
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_RET_CLEAR_L2PLRU_BMSK                                 0x1000
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_RET_CLEAR_L2PLRU_SHFT                                    0xc
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_RET_CLEAR_L2TAG_BMSK                                   0x800
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_RET_CLEAR_L2TAG_SHFT                                     0xb
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_RET_CLEAR_L2STBUF_BMSK                                 0x400
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_RET_CLEAR_L2STBUF_SHFT                                   0xa
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_RET_CLEAR_L1DU_TAG_BMSK                                0x200
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_RET_CLEAR_L1DU_TAG_SHFT                                  0x9
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_RET_CLEAR_L1DU_BMSK                                    0x100
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_RET_CLEAR_L1DU_SHFT                                      0x8
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_RET_CLEAR_JU_TLB64_BMSK                                 0x80
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_RET_CLEAR_JU_TLB64_SHFT                                  0x7
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_RET_CLEAR_JU_TLB128_BMSK                                0x40
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_RET_CLEAR_JU_TLB128_SHFT                                 0x6
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_RET_CLEAR_JU_CAM_BMSK                                   0x20
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_RET_CLEAR_JU_CAM_SHFT                                    0x5
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_RET_CLEAR_ETB_BMSK                                      0x10
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_RET_CLEAR_ETB_SHFT                                       0x4
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_RET_CLEAR_L1IU_TAG_BMSK                                  0x8
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_RET_CLEAR_L1IU_TAG_SHFT                                  0x3
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_RET_CLEAR_L1IU_PDEC_BMSK                                 0x4
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_RET_CLEAR_L1IU_PDEC_SHFT                                 0x2
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_RET_CLEAR_L1IU_BMSK                                      0x2
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_RET_CLEAR_L1IU_SHFT                                      0x1
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_RET_CLEAR_L1IU_BTB_BMSK                                  0x1
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_RET_CLEAR_L1IU_BTB_SHFT                                  0x0

#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_CTL_CLEAR_ADDR                                    (MSS_QDSP6V66SS_CSR_REG_BASE      + 0x0000c210)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_CTL_CLEAR_OFFS                                    (MSS_QDSP6V66SS_CSR_REG_BASE_OFFS + 0x0000c210)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_CTL_CLEAR_RMSK                                           0xa
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_CTL_CLEAR_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_CTL_CLEAR_ADDR,v)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_CTL_CLEAR_CLAMP_QMC_MEM_BMSK                             0x8
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_CTL_CLEAR_CLAMP_QMC_MEM_SHFT                             0x3
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_CTL_CLEAR_MEM_PERIPH_BMSK                                0x2
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_CTL_CLEAR_MEM_PERIPH_SHFT                                0x1

#define HWIO_MSS_QDSP6SS_RSCCTL_EN_BHS_CLEAR_ADDR                                        (MSS_QDSP6V66SS_CSR_REG_BASE      + 0x0000c214)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_BHS_CLEAR_OFFS                                        (MSS_QDSP6V66SS_CSR_REG_BASE_OFFS + 0x0000c214)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_BHS_CLEAR_RMSK                                               0x1
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_BHS_CLEAR_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_RSCCTL_EN_BHS_CLEAR_ADDR,v)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_BHS_CLEAR_BHS_BMSK                                           0x1
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_BHS_CLEAR_BHS_SHFT                                           0x0

#define HWIO_MSS_QDSP6SS_RSCCTL_EN_LDO_CLEAR_ADDR                                        (MSS_QDSP6V66SS_CSR_REG_BASE      + 0x0000c218)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_LDO_CLEAR_OFFS                                        (MSS_QDSP6V66SS_CSR_REG_BASE_OFFS + 0x0000c218)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_LDO_CLEAR_RMSK                                              0x7f
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_LDO_CLEAR_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_RSCCTL_EN_LDO_CLEAR_ADDR,v)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_LDO_CLEAR_BG_OFF_BMSK                                       0x40
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_LDO_CLEAR_BG_OFF_SHFT                                        0x6
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_LDO_CLEAR_FORCEOFF_BMSK                                     0x20
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_LDO_CLEAR_FORCEOFF_SHFT                                      0x5
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_LDO_CLEAR_SPARE_BITS4_2_BMSK                                0x1c
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_LDO_CLEAR_SPARE_BITS4_2_SHFT                                 0x2
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_LDO_CLEAR_UPDATE_BMSK                                        0x2
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_LDO_CLEAR_UPDATE_SHFT                                        0x1
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_LDO_CLEAR_BYPASS_OPEN_BMSK                                   0x1
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_LDO_CLEAR_BYPASS_OPEN_SHFT                                   0x0

#define HWIO_MSS_QDSP6SS_RSCCTL_EN_PLL_CLEAR_ADDR                                        (MSS_QDSP6V66SS_CSR_REG_BASE      + 0x0000c220)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_PLL_CLEAR_OFFS                                        (MSS_QDSP6V66SS_CSR_REG_BASE_OFFS + 0x0000c220)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_PLL_CLEAR_RMSK                                              0x17
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_PLL_CLEAR_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_RSCCTL_EN_PLL_CLEAR_ADDR,v)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_PLL_CLEAR_PLL_STBY_BMSK                                     0x10
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_PLL_CLEAR_PLL_STBY_SHFT                                      0x4
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_PLL_CLEAR_PLL_OUTDIS_BMSK                                    0x4
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_PLL_CLEAR_PLL_OUTDIS_SHFT                                    0x2
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_PLL_CLEAR_PLL_OPMODE1_BMSK                                   0x2
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_PLL_CLEAR_PLL_OPMODE1_SHFT                                   0x1
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_PLL_CLEAR_PLL_RESET_BMSK                                     0x1
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_PLL_CLEAR_PLL_RESET_SHFT                                     0x0

#define HWIO_MSS_QDSP6SS_RSCCTL_EN_STATERET_CLEAR_ADDR                                   (MSS_QDSP6V66SS_CSR_REG_BASE      + 0x0000c224)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_STATERET_CLEAR_OFFS                                   (MSS_QDSP6V66SS_CSR_REG_BASE_OFFS + 0x0000c224)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_STATERET_CLEAR_RMSK                                          0x7
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_STATERET_CLEAR_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_RSCCTL_EN_STATERET_CLEAR_ADDR,v)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_STATERET_CLEAR_WAKE_IRQ_BMSK                                 0x4
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_STATERET_CLEAR_WAKE_IRQ_SHFT                                 0x2
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_STATERET_CLEAR_RESTORE_BMSK                                  0x2
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_STATERET_CLEAR_RESTORE_SHFT                                  0x1
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_STATERET_CLEAR_SAVE_BMSK                                     0x1
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_STATERET_CLEAR_SAVE_SHFT                                     0x0

#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CTL_CLEAR_ADDR                                (MSS_QDSP6V66SS_CSR_REG_BASE      + 0x0000c228)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CTL_CLEAR_OFFS                                (MSS_QDSP6V66SS_CSR_REG_BASE_OFFS + 0x0000c228)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CTL_CLEAR_RMSK                                      0x3d
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CTL_CLEAR_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CTL_CLEAR_ADDR,v)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CTL_CLEAR_AXIM2_FIFO_ARESET_BMSK                    0x20
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CTL_CLEAR_AXIM2_FIFO_ARESET_SHFT                     0x5
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CTL_CLEAR_AXIM2_HALTREQ_BMSK                        0x10
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CTL_CLEAR_AXIM2_HALTREQ_SHFT                         0x4
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CTL_CLEAR_AXIM_FIFO_ARESET_BMSK                      0x8
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CTL_CLEAR_AXIM_FIFO_ARESET_SHFT                      0x3
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CTL_CLEAR_AXIM_HALTREQ_BMSK                          0x4
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CTL_CLEAR_AXIM_HALTREQ_SHFT                          0x2
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CTL_CLEAR_BUSM_HALTREQ_BMSK                          0x1
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CTL_CLEAR_BUSM_HALTREQ_SHFT                          0x0

#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CLAMP_CLEAR_ADDR                              (MSS_QDSP6V66SS_CSR_REG_BASE      + 0x0000c22c)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CLAMP_CLEAR_OFFS                              (MSS_QDSP6V66SS_CSR_REG_BASE_OFFS + 0x0000c22c)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CLAMP_CLEAR_RMSK                                    0xff
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CLAMP_CLEAR_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CLAMP_CLEAR_ADDR,v)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CLAMP_CLEAR_AXIM2_CLAMP_E_CLEAR_BMSK                0x80
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CLAMP_CLEAR_AXIM2_CLAMP_E_CLEAR_SHFT                 0x7
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CLAMP_CLEAR_AXIM2_CLAMP_E_SET_BMSK                  0x40
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CLAMP_CLEAR_AXIM2_CLAMP_E_SET_SHFT                   0x6
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CLAMP_CLEAR_AXIM2_CLAMP_L_CLEAR_BMSK                0x20
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CLAMP_CLEAR_AXIM2_CLAMP_L_CLEAR_SHFT                 0x5
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CLAMP_CLEAR_AXIM2_CLAMP_L_SET_BMSK                  0x10
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CLAMP_CLEAR_AXIM2_CLAMP_L_SET_SHFT                   0x4
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CLAMP_CLEAR_AXIM_CLAMP_E_CLEAR_BMSK                  0x8
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CLAMP_CLEAR_AXIM_CLAMP_E_CLEAR_SHFT                  0x3
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CLAMP_CLEAR_AXIM_CLAMP_E_SET_BMSK                    0x4
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CLAMP_CLEAR_AXIM_CLAMP_E_SET_SHFT                    0x2
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CLAMP_CLEAR_AXIM_CLAMP_L_CLEAR_BMSK                  0x2
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CLAMP_CLEAR_AXIM_CLAMP_L_CLEAR_SHFT                  0x1
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CLAMP_CLEAR_AXIM_CLAMP_L_SET_BMSK                    0x1
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CLAMP_CLEAR_AXIM_CLAMP_L_SET_SHFT                    0x0

#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CLK_CLEAR_ADDR                                (MSS_QDSP6V66SS_CSR_REG_BASE      + 0x0000c230)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CLK_CLEAR_OFFS                                (MSS_QDSP6V66SS_CSR_REG_BASE_OFFS + 0x0000c230)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CLK_CLEAR_RMSK                                       0x7
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CLK_CLEAR_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CLK_CLEAR_ADDR,v)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CLK_CLEAR_AXIM2_BMSK                                 0x4
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CLK_CLEAR_AXIM2_SHFT                                 0x2
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CLK_CLEAR_AXIM_BMSK                                  0x2
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CLK_CLEAR_AXIM_SHFT                                  0x1
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CLK_CLEAR_CORE_BMSK                                  0x1
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CLK_CLEAR_CORE_SHFT                                  0x0

#define HWIO_MSS_QDSP6SS_RSCCTL_EN_L2MEM_RET_CLEAR_ADDR                                  (MSS_QDSP6V66SS_CSR_REG_BASE      + 0x0000c234)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_L2MEM_RET_CLEAR_OFFS                                  (MSS_QDSP6V66SS_CSR_REG_BASE_OFFS + 0x0000c234)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_L2MEM_RET_CLEAR_RMSK                                         0xf
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_L2MEM_RET_CLEAR_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_RSCCTL_EN_L2MEM_RET_CLEAR_ADDR,v)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_L2MEM_RET_CLEAR_L2DATA_BMSK                                  0xf
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_L2MEM_RET_CLEAR_L2DATA_SHFT                                  0x0

#define HWIO_MSS_QDSP6SS_RSCCTL_EN_PRSC_CLEAR_ADDR                                       (MSS_QDSP6V66SS_CSR_REG_BASE      + 0x0000c238)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_PRSC_CLEAR_OFFS                                       (MSS_QDSP6V66SS_CSR_REG_BASE_OFFS + 0x0000c238)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_PRSC_CLEAR_RMSK                                           0x3fff
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_PRSC_CLEAR_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_RSCCTL_EN_PRSC_CLEAR_ADDR,v)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_PRSC_CLEAR_SPARE_BMSK                                     0x3c00
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_PRSC_CLEAR_SPARE_SHFT                                        0xa
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_PRSC_CLEAR_TCS_HW_TRIGGER_BMSK                             0x3fc
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_PRSC_CLEAR_TCS_HW_TRIGGER_SHFT                               0x2
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_PRSC_CLEAR_BRINGUP_REQ_BMSK                                  0x2
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_PRSC_CLEAR_BRINGUP_REQ_SHFT                                  0x1
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_PRSC_CLEAR_SHUTDOWN_REQ_BMSK                                 0x1
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_PRSC_CLEAR_SHUTDOWN_REQ_SHFT                                 0x0

#define HWIO_MSS_QDSP6SS_RSCCTL_EN_L1SMEM_RET_CLEAR_ADDR                                 (MSS_QDSP6V66SS_CSR_REG_BASE      + 0x0000c240)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_L1SMEM_RET_CLEAR_OFFS                                 (MSS_QDSP6V66SS_CSR_REG_BASE_OFFS + 0x0000c240)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_L1SMEM_RET_CLEAR_RMSK                                       0x3f
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_L1SMEM_RET_CLEAR_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_RSCCTL_EN_L1SMEM_RET_CLEAR_ADDR,v)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_L1SMEM_RET_CLEAR_L1SDATA_BMSK                               0x3f
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_L1SMEM_RET_CLEAR_L1SDATA_SHFT                                0x0

#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_STAGGER_CLEAR_ADDR                                (MSS_QDSP6V66SS_CSR_REG_BASE      + 0x0000c244)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_STAGGER_CLEAR_OFFS                                (MSS_QDSP6V66SS_CSR_REG_BASE_OFFS + 0x0000c244)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_STAGGER_CLEAR_RMSK                                       0x7
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_STAGGER_CLEAR_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_STAGGER_CLEAR_ADDR,v)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_STAGGER_CLEAR_STAGGER_RESTORE_BMSK                       0x4
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_STAGGER_CLEAR_STAGGER_RESTORE_SHFT                       0x2
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_STAGGER_CLEAR_STAGGER_ALT_ARES_BMSK                      0x2
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_STAGGER_CLEAR_STAGGER_ALT_ARES_SHFT                      0x1
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_STAGGER_CLEAR_STAGGER_FUNC_ARES_BMSK                     0x1
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_STAGGER_CLEAR_STAGGER_FUNC_ARES_SHFT                     0x0

#define HWIO_MSS_QDSP6SS_RSCCTL_EN_VTCMEM_RET_CLEAR_ADDR                                 (MSS_QDSP6V66SS_CSR_REG_BASE      + 0x0000c248)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_VTCMEM_RET_CLEAR_OFFS                                 (MSS_QDSP6V66SS_CSR_REG_BASE_OFFS + 0x0000c248)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_VTCMEM_RET_CLEAR_RMSK                                        0x1
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_VTCMEM_RET_CLEAR_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_RSCCTL_EN_VTCMEM_RET_CLEAR_ADDR,v)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_VTCMEM_RET_CLEAR_VTCM_BMSK                                   0x1
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_VTCMEM_RET_CLEAR_VTCM_SHFT                                   0x0

/*----------------------------------------------------------------------------
 * MODULE: MSS_QDSP6SS_QDSP6SS_QTMR_AC
 *--------------------------------------------------------------------------*/

#define MSS_QDSP6SS_QDSP6SS_QTMR_AC_REG_BASE                                (MODEM_TOP_BASE      + 0x00120000)
#define MSS_QDSP6SS_QDSP6SS_QTMR_AC_REG_BASE_OFFS                           0x00120000

#define HWIO_MSS_QDSP6SS_QTMR_AC_CNTFRQ_ADDR                                (MSS_QDSP6SS_QDSP6SS_QTMR_AC_REG_BASE      + 0x00000000)
#define HWIO_MSS_QDSP6SS_QTMR_AC_CNTFRQ_OFFS                                (MSS_QDSP6SS_QDSP6SS_QTMR_AC_REG_BASE_OFFS + 0x00000000)
#define HWIO_MSS_QDSP6SS_QTMR_AC_CNTFRQ_RMSK                                0xffffffff
#define HWIO_MSS_QDSP6SS_QTMR_AC_CNTFRQ_IN          \
        in_dword_masked(HWIO_MSS_QDSP6SS_QTMR_AC_CNTFRQ_ADDR, HWIO_MSS_QDSP6SS_QTMR_AC_CNTFRQ_RMSK)
#define HWIO_MSS_QDSP6SS_QTMR_AC_CNTFRQ_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_QTMR_AC_CNTFRQ_ADDR, m)
#define HWIO_MSS_QDSP6SS_QTMR_AC_CNTFRQ_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_QTMR_AC_CNTFRQ_ADDR,v)
#define HWIO_MSS_QDSP6SS_QTMR_AC_CNTFRQ_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_QTMR_AC_CNTFRQ_ADDR,m,v,HWIO_MSS_QDSP6SS_QTMR_AC_CNTFRQ_IN)
#define HWIO_MSS_QDSP6SS_QTMR_AC_CNTFRQ_CNTFRQ_BMSK                         0xffffffff
#define HWIO_MSS_QDSP6SS_QTMR_AC_CNTFRQ_CNTFRQ_SHFT                                0x0

#define HWIO_MSS_QDSP6SS_QTMR_AC_CNTNSAR_FG0_ADDR                           (MSS_QDSP6SS_QDSP6SS_QTMR_AC_REG_BASE      + 0x00000004)
#define HWIO_MSS_QDSP6SS_QTMR_AC_CNTNSAR_FG0_OFFS                           (MSS_QDSP6SS_QDSP6SS_QTMR_AC_REG_BASE_OFFS + 0x00000004)
#define HWIO_MSS_QDSP6SS_QTMR_AC_CNTNSAR_FG0_RMSK                                  0x7
#define HWIO_MSS_QDSP6SS_QTMR_AC_CNTNSAR_FG0_IN          \
        in_dword_masked(HWIO_MSS_QDSP6SS_QTMR_AC_CNTNSAR_FG0_ADDR, HWIO_MSS_QDSP6SS_QTMR_AC_CNTNSAR_FG0_RMSK)
#define HWIO_MSS_QDSP6SS_QTMR_AC_CNTNSAR_FG0_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_QTMR_AC_CNTNSAR_FG0_ADDR, m)
#define HWIO_MSS_QDSP6SS_QTMR_AC_CNTNSAR_FG0_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_QTMR_AC_CNTNSAR_FG0_ADDR,v)
#define HWIO_MSS_QDSP6SS_QTMR_AC_CNTNSAR_FG0_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_QTMR_AC_CNTNSAR_FG0_ADDR,m,v,HWIO_MSS_QDSP6SS_QTMR_AC_CNTNSAR_FG0_IN)
#define HWIO_MSS_QDSP6SS_QTMR_AC_CNTNSAR_FG0_NSN_BMSK                              0x7
#define HWIO_MSS_QDSP6SS_QTMR_AC_CNTNSAR_FG0_NSN_SHFT                              0x0

#define HWIO_MSS_QDSP6SS_QTMR_AC_CNTTIDR_FG0_ADDR                           (MSS_QDSP6SS_QDSP6SS_QTMR_AC_REG_BASE      + 0x00000008)
#define HWIO_MSS_QDSP6SS_QTMR_AC_CNTTIDR_FG0_OFFS                           (MSS_QDSP6SS_QDSP6SS_QTMR_AC_REG_BASE_OFFS + 0x00000008)
#define HWIO_MSS_QDSP6SS_QTMR_AC_CNTTIDR_FG0_RMSK                           0xffffffff
#define HWIO_MSS_QDSP6SS_QTMR_AC_CNTTIDR_FG0_IN          \
        in_dword_masked(HWIO_MSS_QDSP6SS_QTMR_AC_CNTTIDR_FG0_ADDR, HWIO_MSS_QDSP6SS_QTMR_AC_CNTTIDR_FG0_RMSK)
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

#define HWIO_MSS_QDSP6SS_QTMR_AC_CNTVOFF_FG0_LO_n_ADDR(n)                   (MSS_QDSP6SS_QDSP6SS_QTMR_AC_REG_BASE      + 0x00000080 + 0x8 * (n))
#define HWIO_MSS_QDSP6SS_QTMR_AC_CNTVOFF_FG0_LO_n_OFFS(n)                   (MSS_QDSP6SS_QDSP6SS_QTMR_AC_REG_BASE_OFFS + 0x00000080 + 0x8 * (n))
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
#define HWIO_MSS_QDSP6SS_QTMR_AC_CNTVOFF_FG0_HI_n_OFFS(n)                   (MSS_QDSP6SS_QDSP6SS_QTMR_AC_REG_BASE_OFFS + 0x00000084 + 0x8 * (n))
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
#define HWIO_MSS_QDSP6SS_QTMR_AC_CFG_OFFS                                   (MSS_QDSP6SS_QDSP6SS_QTMR_AC_REG_BASE_OFFS + 0x00000fc0)
#define HWIO_MSS_QDSP6SS_QTMR_AC_CFG_RMSK                                          0x3
#define HWIO_MSS_QDSP6SS_QTMR_AC_CFG_IN          \
        in_dword_masked(HWIO_MSS_QDSP6SS_QTMR_AC_CFG_ADDR, HWIO_MSS_QDSP6SS_QTMR_AC_CFG_RMSK)
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
#define HWIO_MSS_QDSP6SS_QTMR_AC_VERSION_OFFS                               (MSS_QDSP6SS_QDSP6SS_QTMR_AC_REG_BASE_OFFS + 0x00000fd0)
#define HWIO_MSS_QDSP6SS_QTMR_AC_VERSION_RMSK                               0xffffffff
#define HWIO_MSS_QDSP6SS_QTMR_AC_VERSION_IN          \
        in_dword_masked(HWIO_MSS_QDSP6SS_QTMR_AC_VERSION_ADDR, HWIO_MSS_QDSP6SS_QTMR_AC_VERSION_RMSK)
#define HWIO_MSS_QDSP6SS_QTMR_AC_VERSION_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_QTMR_AC_VERSION_ADDR, m)
#define HWIO_MSS_QDSP6SS_QTMR_AC_VERSION_MAJOR_BMSK                         0xf0000000
#define HWIO_MSS_QDSP6SS_QTMR_AC_VERSION_MAJOR_SHFT                               0x1c
#define HWIO_MSS_QDSP6SS_QTMR_AC_VERSION_MINOR_BMSK                          0xfff0000
#define HWIO_MSS_QDSP6SS_QTMR_AC_VERSION_MINOR_SHFT                               0x10
#define HWIO_MSS_QDSP6SS_QTMR_AC_VERSION_STEP_BMSK                              0xffff
#define HWIO_MSS_QDSP6SS_QTMR_AC_VERSION_STEP_SHFT                                 0x0

#define HWIO_MSS_QDSP6SS_QTMR_AC_HW_FRAME_SEL_1_ADDR                        (MSS_QDSP6SS_QDSP6SS_QTMR_AC_REG_BASE      + 0x00000fe0)
#define HWIO_MSS_QDSP6SS_QTMR_AC_HW_FRAME_SEL_1_OFFS                        (MSS_QDSP6SS_QDSP6SS_QTMR_AC_REG_BASE_OFFS + 0x00000fe0)
#define HWIO_MSS_QDSP6SS_QTMR_AC_HW_FRAME_SEL_1_RMSK                        0xffffffff
#define HWIO_MSS_QDSP6SS_QTMR_AC_HW_FRAME_SEL_1_IN          \
        in_dword_masked(HWIO_MSS_QDSP6SS_QTMR_AC_HW_FRAME_SEL_1_ADDR, HWIO_MSS_QDSP6SS_QTMR_AC_HW_FRAME_SEL_1_RMSK)
#define HWIO_MSS_QDSP6SS_QTMR_AC_HW_FRAME_SEL_1_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_QTMR_AC_HW_FRAME_SEL_1_ADDR, m)
#define HWIO_MSS_QDSP6SS_QTMR_AC_HW_FRAME_SEL_1_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_QTMR_AC_HW_FRAME_SEL_1_ADDR,v)
#define HWIO_MSS_QDSP6SS_QTMR_AC_HW_FRAME_SEL_1_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_QTMR_AC_HW_FRAME_SEL_1_ADDR,m,v,HWIO_MSS_QDSP6SS_QTMR_AC_HW_FRAME_SEL_1_IN)
#define HWIO_MSS_QDSP6SS_QTMR_AC_HW_FRAME_SEL_1_HW_FRAME_SEL1_BMSK          0xffffffff
#define HWIO_MSS_QDSP6SS_QTMR_AC_HW_FRAME_SEL_1_HW_FRAME_SEL1_SHFT                 0x0

#define HWIO_MSS_QDSP6SS_QTMR_AC_HW_FRAME_SEL_2_ADDR                        (MSS_QDSP6SS_QDSP6SS_QTMR_AC_REG_BASE      + 0x00000ff0)
#define HWIO_MSS_QDSP6SS_QTMR_AC_HW_FRAME_SEL_2_OFFS                        (MSS_QDSP6SS_QDSP6SS_QTMR_AC_REG_BASE_OFFS + 0x00000ff0)
#define HWIO_MSS_QDSP6SS_QTMR_AC_HW_FRAME_SEL_2_RMSK                        0xffffffff
#define HWIO_MSS_QDSP6SS_QTMR_AC_HW_FRAME_SEL_2_IN          \
        in_dword_masked(HWIO_MSS_QDSP6SS_QTMR_AC_HW_FRAME_SEL_2_ADDR, HWIO_MSS_QDSP6SS_QTMR_AC_HW_FRAME_SEL_2_RMSK)
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

#define MSS_QDSP6SS_QTMR_F1_1_REG_BASE                                     (MODEM_TOP_BASE      + 0x00122000)
#define MSS_QDSP6SS_QTMR_F1_1_REG_BASE_OFFS                                0x00122000

#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTPCT_LO_1_ADDR                          (MSS_QDSP6SS_QTMR_F1_1_REG_BASE      + 0x00000000)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTPCT_LO_1_OFFS                          (MSS_QDSP6SS_QTMR_F1_1_REG_BASE_OFFS + 0x00000000)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTPCT_LO_1_RMSK                          0xffffffff
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTPCT_LO_1_IN          \
        in_dword_masked(HWIO_MSS_QDSP6SS_QTMR_V1_CNTPCT_LO_1_ADDR, HWIO_MSS_QDSP6SS_QTMR_V1_CNTPCT_LO_1_RMSK)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTPCT_LO_1_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_QTMR_V1_CNTPCT_LO_1_ADDR, m)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTPCT_LO_1_CNTPCT_LO_BMSK                0xffffffff
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTPCT_LO_1_CNTPCT_LO_SHFT                       0x0

#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTPCT_HI_1_ADDR                          (MSS_QDSP6SS_QTMR_F1_1_REG_BASE      + 0x00000004)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTPCT_HI_1_OFFS                          (MSS_QDSP6SS_QTMR_F1_1_REG_BASE_OFFS + 0x00000004)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTPCT_HI_1_RMSK                            0xffffff
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTPCT_HI_1_IN          \
        in_dword_masked(HWIO_MSS_QDSP6SS_QTMR_V1_CNTPCT_HI_1_ADDR, HWIO_MSS_QDSP6SS_QTMR_V1_CNTPCT_HI_1_RMSK)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTPCT_HI_1_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_QTMR_V1_CNTPCT_HI_1_ADDR, m)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTPCT_HI_1_CNTPCT_HI_BMSK                  0xffffff
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTPCT_HI_1_CNTPCT_HI_SHFT                       0x0

#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTVCT_LO_1_ADDR                          (MSS_QDSP6SS_QTMR_F1_1_REG_BASE      + 0x00000008)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTVCT_LO_1_OFFS                          (MSS_QDSP6SS_QTMR_F1_1_REG_BASE_OFFS + 0x00000008)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTVCT_LO_1_RMSK                          0xffffffff
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTVCT_LO_1_IN          \
        in_dword_masked(HWIO_MSS_QDSP6SS_QTMR_V1_CNTVCT_LO_1_ADDR, HWIO_MSS_QDSP6SS_QTMR_V1_CNTVCT_LO_1_RMSK)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTVCT_LO_1_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_QTMR_V1_CNTVCT_LO_1_ADDR, m)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTVCT_LO_1_CNTVCT_LO_BMSK                0xffffffff
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTVCT_LO_1_CNTVCT_LO_SHFT                       0x0

#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTVCT_HI_1_ADDR                          (MSS_QDSP6SS_QTMR_F1_1_REG_BASE      + 0x0000000c)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTVCT_HI_1_OFFS                          (MSS_QDSP6SS_QTMR_F1_1_REG_BASE_OFFS + 0x0000000c)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTVCT_HI_1_RMSK                            0xffffff
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTVCT_HI_1_IN          \
        in_dword_masked(HWIO_MSS_QDSP6SS_QTMR_V1_CNTVCT_HI_1_ADDR, HWIO_MSS_QDSP6SS_QTMR_V1_CNTVCT_HI_1_RMSK)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTVCT_HI_1_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_QTMR_V1_CNTVCT_HI_1_ADDR, m)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTVCT_HI_1_CNTVCT_HI_BMSK                  0xffffff
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTVCT_HI_1_CNTVCT_HI_SHFT                       0x0

#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTFRQ_1_ADDR                             (MSS_QDSP6SS_QTMR_F1_1_REG_BASE      + 0x00000010)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTFRQ_1_OFFS                             (MSS_QDSP6SS_QTMR_F1_1_REG_BASE_OFFS + 0x00000010)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTFRQ_1_RMSK                             0xffffffff
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTFRQ_1_IN          \
        in_dword_masked(HWIO_MSS_QDSP6SS_QTMR_V1_CNTFRQ_1_ADDR, HWIO_MSS_QDSP6SS_QTMR_V1_CNTFRQ_1_RMSK)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTFRQ_1_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_QTMR_V1_CNTFRQ_1_ADDR, m)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTFRQ_1_CNTFRQ_BMSK                      0xffffffff
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTFRQ_1_CNTFRQ_SHFT                             0x0

#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTVOFF_LO_1_ADDR                         (MSS_QDSP6SS_QTMR_F1_1_REG_BASE      + 0x00000018)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTVOFF_LO_1_OFFS                         (MSS_QDSP6SS_QTMR_F1_1_REG_BASE_OFFS + 0x00000018)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTVOFF_LO_1_RMSK                         0xffffffff
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTVOFF_LO_1_IN          \
        in_dword_masked(HWIO_MSS_QDSP6SS_QTMR_V1_CNTVOFF_LO_1_ADDR, HWIO_MSS_QDSP6SS_QTMR_V1_CNTVOFF_LO_1_RMSK)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTVOFF_LO_1_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_QTMR_V1_CNTVOFF_LO_1_ADDR, m)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTVOFF_LO_1_CNTVOFF_L0_BMSK              0xffffffff
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTVOFF_LO_1_CNTVOFF_L0_SHFT                     0x0

#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTVOFF_HI_1_ADDR                         (MSS_QDSP6SS_QTMR_F1_1_REG_BASE      + 0x0000001c)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTVOFF_HI_1_OFFS                         (MSS_QDSP6SS_QTMR_F1_1_REG_BASE_OFFS + 0x0000001c)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTVOFF_HI_1_RMSK                           0xffffff
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTVOFF_HI_1_IN          \
        in_dword_masked(HWIO_MSS_QDSP6SS_QTMR_V1_CNTVOFF_HI_1_ADDR, HWIO_MSS_QDSP6SS_QTMR_V1_CNTVOFF_HI_1_RMSK)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTVOFF_HI_1_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_QTMR_V1_CNTVOFF_HI_1_ADDR, m)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTVOFF_HI_1_CNTVOFF_HI_BMSK                0xffffff
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTVOFF_HI_1_CNTVOFF_HI_SHFT                     0x0

#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_CVAL_LO_1_ADDR                       (MSS_QDSP6SS_QTMR_F1_1_REG_BASE      + 0x00000020)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_CVAL_LO_1_OFFS                       (MSS_QDSP6SS_QTMR_F1_1_REG_BASE_OFFS + 0x00000020)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_CVAL_LO_1_RMSK                       0xffffffff
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_CVAL_LO_1_IN          \
        in_dword_masked(HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_CVAL_LO_1_ADDR, HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_CVAL_LO_1_RMSK)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_CVAL_LO_1_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_CVAL_LO_1_ADDR, m)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_CVAL_LO_1_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_CVAL_LO_1_ADDR,v)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_CVAL_LO_1_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_CVAL_LO_1_ADDR,m,v,HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_CVAL_LO_1_IN)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_CVAL_LO_1_CNTP_CVAL_L0_BMSK          0xffffffff
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_CVAL_LO_1_CNTP_CVAL_L0_SHFT                 0x0

#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_CVAL_HI_1_ADDR                       (MSS_QDSP6SS_QTMR_F1_1_REG_BASE      + 0x00000024)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_CVAL_HI_1_OFFS                       (MSS_QDSP6SS_QTMR_F1_1_REG_BASE_OFFS + 0x00000024)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_CVAL_HI_1_RMSK                         0xffffff
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_CVAL_HI_1_IN          \
        in_dword_masked(HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_CVAL_HI_1_ADDR, HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_CVAL_HI_1_RMSK)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_CVAL_HI_1_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_CVAL_HI_1_ADDR, m)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_CVAL_HI_1_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_CVAL_HI_1_ADDR,v)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_CVAL_HI_1_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_CVAL_HI_1_ADDR,m,v,HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_CVAL_HI_1_IN)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_CVAL_HI_1_CNTP_CVAL_HI_BMSK            0xffffff
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_CVAL_HI_1_CNTP_CVAL_HI_SHFT                 0x0

#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_TVAL_1_ADDR                          (MSS_QDSP6SS_QTMR_F1_1_REG_BASE      + 0x00000028)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_TVAL_1_OFFS                          (MSS_QDSP6SS_QTMR_F1_1_REG_BASE_OFFS + 0x00000028)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_TVAL_1_RMSK                          0xffffffff
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_TVAL_1_IN          \
        in_dword_masked(HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_TVAL_1_ADDR, HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_TVAL_1_RMSK)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_TVAL_1_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_TVAL_1_ADDR, m)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_TVAL_1_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_TVAL_1_ADDR,v)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_TVAL_1_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_TVAL_1_ADDR,m,v,HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_TVAL_1_IN)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_TVAL_1_CNTP_TVAL_BMSK                0xffffffff
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_TVAL_1_CNTP_TVAL_SHFT                       0x0

#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_CTL_1_ADDR                           (MSS_QDSP6SS_QTMR_F1_1_REG_BASE      + 0x0000002c)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_CTL_1_OFFS                           (MSS_QDSP6SS_QTMR_F1_1_REG_BASE_OFFS + 0x0000002c)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_CTL_1_RMSK                                  0x7
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_CTL_1_IN          \
        in_dword_masked(HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_CTL_1_ADDR, HWIO_MSS_QDSP6SS_QTMR_V1_CNTP_CTL_1_RMSK)
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
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_CVAL_LO_1_OFFS                       (MSS_QDSP6SS_QTMR_F1_1_REG_BASE_OFFS + 0x00000030)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_CVAL_LO_1_RMSK                       0xffffffff
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_CVAL_LO_1_IN          \
        in_dword_masked(HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_CVAL_LO_1_ADDR, HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_CVAL_LO_1_RMSK)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_CVAL_LO_1_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_CVAL_LO_1_ADDR, m)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_CVAL_LO_1_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_CVAL_LO_1_ADDR,v)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_CVAL_LO_1_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_CVAL_LO_1_ADDR,m,v,HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_CVAL_LO_1_IN)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_CVAL_LO_1_CNTV_CVAL_L0_BMSK          0xffffffff
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_CVAL_LO_1_CNTV_CVAL_L0_SHFT                 0x0

#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_CVAL_HI_1_ADDR                       (MSS_QDSP6SS_QTMR_F1_1_REG_BASE      + 0x00000034)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_CVAL_HI_1_OFFS                       (MSS_QDSP6SS_QTMR_F1_1_REG_BASE_OFFS + 0x00000034)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_CVAL_HI_1_RMSK                         0xffffff
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_CVAL_HI_1_IN          \
        in_dword_masked(HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_CVAL_HI_1_ADDR, HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_CVAL_HI_1_RMSK)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_CVAL_HI_1_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_CVAL_HI_1_ADDR, m)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_CVAL_HI_1_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_CVAL_HI_1_ADDR,v)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_CVAL_HI_1_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_CVAL_HI_1_ADDR,m,v,HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_CVAL_HI_1_IN)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_CVAL_HI_1_CNTV_CVAL_HI_BMSK            0xffffff
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_CVAL_HI_1_CNTV_CVAL_HI_SHFT                 0x0

#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_TVAL_1_ADDR                          (MSS_QDSP6SS_QTMR_F1_1_REG_BASE      + 0x00000038)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_TVAL_1_OFFS                          (MSS_QDSP6SS_QTMR_F1_1_REG_BASE_OFFS + 0x00000038)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_TVAL_1_RMSK                          0xffffffff
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_TVAL_1_IN          \
        in_dword_masked(HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_TVAL_1_ADDR, HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_TVAL_1_RMSK)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_TVAL_1_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_TVAL_1_ADDR, m)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_TVAL_1_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_TVAL_1_ADDR,v)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_TVAL_1_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_TVAL_1_ADDR,m,v,HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_TVAL_1_IN)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_TVAL_1_CNTV_TVAL_BMSK                0xffffffff
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_TVAL_1_CNTV_TVAL_SHFT                       0x0

#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_CTL_1_ADDR                           (MSS_QDSP6SS_QTMR_F1_1_REG_BASE      + 0x0000003c)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_CTL_1_OFFS                           (MSS_QDSP6SS_QTMR_F1_1_REG_BASE_OFFS + 0x0000003c)
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_CTL_1_RMSK                                  0x7
#define HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_CTL_1_IN          \
        in_dword_masked(HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_CTL_1_ADDR, HWIO_MSS_QDSP6SS_QTMR_V1_CNTV_CTL_1_RMSK)
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
#define HWIO_MSS_QDSP6SS_QTMR_V1_VERSION_1_OFFS                            (MSS_QDSP6SS_QTMR_F1_1_REG_BASE_OFFS + 0x00000fd0)
#define HWIO_MSS_QDSP6SS_QTMR_V1_VERSION_1_RMSK                            0xffffffff
#define HWIO_MSS_QDSP6SS_QTMR_V1_VERSION_1_IN          \
        in_dword_masked(HWIO_MSS_QDSP6SS_QTMR_V1_VERSION_1_ADDR, HWIO_MSS_QDSP6SS_QTMR_V1_VERSION_1_RMSK)
#define HWIO_MSS_QDSP6SS_QTMR_V1_VERSION_1_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_QTMR_V1_VERSION_1_ADDR, m)
#define HWIO_MSS_QDSP6SS_QTMR_V1_VERSION_1_MAJOR_BMSK                      0xf0000000
#define HWIO_MSS_QDSP6SS_QTMR_V1_VERSION_1_MAJOR_SHFT                            0x1c
#define HWIO_MSS_QDSP6SS_QTMR_V1_VERSION_1_MINOR_BMSK                       0xfff0000
#define HWIO_MSS_QDSP6SS_QTMR_V1_VERSION_1_MINOR_SHFT                            0x10
#define HWIO_MSS_QDSP6SS_QTMR_V1_VERSION_1_STEP_BMSK                           0xffff
#define HWIO_MSS_QDSP6SS_QTMR_V1_VERSION_1_STEP_SHFT                              0x0


#endif /* __HALCLKHWIOASM_H__ */
