#ifndef __UIMJOLOKIAHWIOMACROS_H__
#define __UIMJOLOKIAHWIOMACROS_H__
/*
===========================================================================
*/
/**
  @file uimjolokiahwiomacros.h
  @brief Auto-generated HWIO interface include file.

  Reference chip release:
    MSM8909/08 (Cheel) [cheel_v1.0_p3q3r17.3_MTO]
 
  This file contains HWIO register definitions for the following modules:
    MSS_UIM0_UART_DM
    MSS_UIM1_UART_DM
    MSS_UIM2_UART_DM

  'Include' filters applied: 
  'Exclude' filters applied: RESERVED DUMMY 
*/
/*
  ===========================================================================

  Copyright (c) 2016 QUALCOMM Technologies, Inc (QTI) and its licensors.All Rights Reserved.
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

  $Header: //components/rel/uim.mpss/6.1.0/uimdrv/src/uimjolokiahwiomacros.h#2 $
  $DateTime: 2019/10/30 06:56:05 $
  $Author: pwbldsvc $

  ===========================================================================
*/

/*----------------------------------------------------------------------------
 * MODULE: MSS_UIM0_UART_DM
 *--------------------------------------------------------------------------*/

#define MSS_UIM0_UART_DM_REG_BASE                                                                               (MSS_TOP_BASE      + 0x00190000)
#define MSS_UIM0_UART_DM_REG_BASE_OFFS                                                                          0x00190000

#define HWIO_MSS_UIM0_UART_DM_MR1_ADDR                                                                          (MSS_UIM0_UART_DM_REG_BASE      + 0x00000000)
#define HWIO_MSS_UIM0_UART_DM_MR1_OFFS                                                                          (MSS_UIM0_UART_DM_REG_BASE_OFFS + 0x00000000)
#define HWIO_MSS_UIM0_UART_DM_MR1_RMSK                                                                          0xffffffff
#define HWIO_MSS_UIM0_UART_DM_MR1_IN          \
        in_dword_masked(HWIO_MSS_UIM0_UART_DM_MR1_ADDR, HWIO_MSS_UIM0_UART_DM_MR1_RMSK)
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
#define HWIO_MSS_UIM0_UART_DM_MR2_OFFS                                                                          (MSS_UIM0_UART_DM_REG_BASE_OFFS + 0x00000004)
#define HWIO_MSS_UIM0_UART_DM_MR2_RMSK                                                                               0x7ff
#define HWIO_MSS_UIM0_UART_DM_MR2_IN          \
        in_dword_masked(HWIO_MSS_UIM0_UART_DM_MR2_ADDR, HWIO_MSS_UIM0_UART_DM_MR2_RMSK)
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
#define HWIO_MSS_UIM0_UART_DM_CSR_SR_DEPRECATED_OFFS                                                            (MSS_UIM0_UART_DM_REG_BASE_OFFS + 0x00000008)
#define HWIO_MSS_UIM0_UART_DM_CSR_SR_DEPRECATED_RMSK                                                            0xffffffff
#define HWIO_MSS_UIM0_UART_DM_CSR_SR_DEPRECATED_IN          \
        in_dword_masked(HWIO_MSS_UIM0_UART_DM_CSR_SR_DEPRECATED_ADDR, HWIO_MSS_UIM0_UART_DM_CSR_SR_DEPRECATED_RMSK)
#define HWIO_MSS_UIM0_UART_DM_CSR_SR_DEPRECATED_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM0_UART_DM_CSR_SR_DEPRECATED_ADDR, m)
#define HWIO_MSS_UIM0_UART_DM_CSR_SR_DEPRECATED_OUT(v)      \
        out_dword(HWIO_MSS_UIM0_UART_DM_CSR_SR_DEPRECATED_ADDR,v)
#define HWIO_MSS_UIM0_UART_DM_CSR_SR_DEPRECATED_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_UIM0_UART_DM_CSR_SR_DEPRECATED_ADDR,m,v,HWIO_MSS_UIM0_UART_DM_CSR_SR_DEPRECATED_IN)
#define HWIO_MSS_UIM0_UART_DM_CSR_SR_DEPRECATED_UART_DM_CSR_SR_DEPRECATED_BMSK                                  0xffffffff
#define HWIO_MSS_UIM0_UART_DM_CSR_SR_DEPRECATED_UART_DM_CSR_SR_DEPRECATED_SHFT                                         0x0

#define HWIO_MSS_UIM0_UART_DM_CR_MISR_DEPRECATED_ADDR                                                           (MSS_UIM0_UART_DM_REG_BASE      + 0x00000010)
#define HWIO_MSS_UIM0_UART_DM_CR_MISR_DEPRECATED_OFFS                                                           (MSS_UIM0_UART_DM_REG_BASE_OFFS + 0x00000010)
#define HWIO_MSS_UIM0_UART_DM_CR_MISR_DEPRECATED_RMSK                                                           0xffffffff
#define HWIO_MSS_UIM0_UART_DM_CR_MISR_DEPRECATED_IN          \
        in_dword_masked(HWIO_MSS_UIM0_UART_DM_CR_MISR_DEPRECATED_ADDR, HWIO_MSS_UIM0_UART_DM_CR_MISR_DEPRECATED_RMSK)
#define HWIO_MSS_UIM0_UART_DM_CR_MISR_DEPRECATED_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM0_UART_DM_CR_MISR_DEPRECATED_ADDR, m)
#define HWIO_MSS_UIM0_UART_DM_CR_MISR_DEPRECATED_OUT(v)      \
        out_dword(HWIO_MSS_UIM0_UART_DM_CR_MISR_DEPRECATED_ADDR,v)
#define HWIO_MSS_UIM0_UART_DM_CR_MISR_DEPRECATED_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_UIM0_UART_DM_CR_MISR_DEPRECATED_ADDR,m,v,HWIO_MSS_UIM0_UART_DM_CR_MISR_DEPRECATED_IN)
#define HWIO_MSS_UIM0_UART_DM_CR_MISR_DEPRECATED_UART_DM_CR_MISR_DEPRECATED_BMSK                                0xffffffff
#define HWIO_MSS_UIM0_UART_DM_CR_MISR_DEPRECATED_UART_DM_CR_MISR_DEPRECATED_SHFT                                       0x0

#define HWIO_MSS_UIM0_UART_DM_IMR_ISR_DEPRECATED_ADDR                                                           (MSS_UIM0_UART_DM_REG_BASE      + 0x00000014)
#define HWIO_MSS_UIM0_UART_DM_IMR_ISR_DEPRECATED_OFFS                                                           (MSS_UIM0_UART_DM_REG_BASE_OFFS + 0x00000014)
#define HWIO_MSS_UIM0_UART_DM_IMR_ISR_DEPRECATED_RMSK                                                           0xffffffff
#define HWIO_MSS_UIM0_UART_DM_IMR_ISR_DEPRECATED_IN          \
        in_dword_masked(HWIO_MSS_UIM0_UART_DM_IMR_ISR_DEPRECATED_ADDR, HWIO_MSS_UIM0_UART_DM_IMR_ISR_DEPRECATED_RMSK)
#define HWIO_MSS_UIM0_UART_DM_IMR_ISR_DEPRECATED_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM0_UART_DM_IMR_ISR_DEPRECATED_ADDR, m)
#define HWIO_MSS_UIM0_UART_DM_IMR_ISR_DEPRECATED_OUT(v)      \
        out_dword(HWIO_MSS_UIM0_UART_DM_IMR_ISR_DEPRECATED_ADDR,v)
#define HWIO_MSS_UIM0_UART_DM_IMR_ISR_DEPRECATED_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_UIM0_UART_DM_IMR_ISR_DEPRECATED_ADDR,m,v,HWIO_MSS_UIM0_UART_DM_IMR_ISR_DEPRECATED_IN)
#define HWIO_MSS_UIM0_UART_DM_IMR_ISR_DEPRECATED_UART_DM_IMR_ISR_DEPRECATED_BMSK                                0xffffffff
#define HWIO_MSS_UIM0_UART_DM_IMR_ISR_DEPRECATED_UART_DM_IMR_ISR_DEPRECATED_SHFT                                       0x0

#define HWIO_MSS_UIM0_UART_DM_IPR_ADDR                                                                          (MSS_UIM0_UART_DM_REG_BASE      + 0x00000018)
#define HWIO_MSS_UIM0_UART_DM_IPR_OFFS                                                                          (MSS_UIM0_UART_DM_REG_BASE_OFFS + 0x00000018)
#define HWIO_MSS_UIM0_UART_DM_IPR_RMSK                                                                          0xffffffdf
#define HWIO_MSS_UIM0_UART_DM_IPR_IN          \
        in_dword_masked(HWIO_MSS_UIM0_UART_DM_IPR_ADDR, HWIO_MSS_UIM0_UART_DM_IPR_RMSK)
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
#define HWIO_MSS_UIM0_UART_DM_TFWR_OFFS                                                                         (MSS_UIM0_UART_DM_REG_BASE_OFFS + 0x0000001c)
#define HWIO_MSS_UIM0_UART_DM_TFWR_RMSK                                                                         0xffffffff
#define HWIO_MSS_UIM0_UART_DM_TFWR_IN          \
        in_dword_masked(HWIO_MSS_UIM0_UART_DM_TFWR_ADDR, HWIO_MSS_UIM0_UART_DM_TFWR_RMSK)
#define HWIO_MSS_UIM0_UART_DM_TFWR_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM0_UART_DM_TFWR_ADDR, m)
#define HWIO_MSS_UIM0_UART_DM_TFWR_OUT(v)      \
        out_dword(HWIO_MSS_UIM0_UART_DM_TFWR_ADDR,v)
#define HWIO_MSS_UIM0_UART_DM_TFWR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_UIM0_UART_DM_TFWR_ADDR,m,v,HWIO_MSS_UIM0_UART_DM_TFWR_IN)
#define HWIO_MSS_UIM0_UART_DM_TFWR_TFW_BMSK                                                                     0xffffffff
#define HWIO_MSS_UIM0_UART_DM_TFWR_TFW_SHFT                                                                            0x0

#define HWIO_MSS_UIM0_UART_DM_RFWR_ADDR                                                                         (MSS_UIM0_UART_DM_REG_BASE      + 0x00000020)
#define HWIO_MSS_UIM0_UART_DM_RFWR_OFFS                                                                         (MSS_UIM0_UART_DM_REG_BASE_OFFS + 0x00000020)
#define HWIO_MSS_UIM0_UART_DM_RFWR_RMSK                                                                         0xffffffff
#define HWIO_MSS_UIM0_UART_DM_RFWR_IN          \
        in_dword_masked(HWIO_MSS_UIM0_UART_DM_RFWR_ADDR, HWIO_MSS_UIM0_UART_DM_RFWR_RMSK)
#define HWIO_MSS_UIM0_UART_DM_RFWR_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM0_UART_DM_RFWR_ADDR, m)
#define HWIO_MSS_UIM0_UART_DM_RFWR_OUT(v)      \
        out_dword(HWIO_MSS_UIM0_UART_DM_RFWR_ADDR,v)
#define HWIO_MSS_UIM0_UART_DM_RFWR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_UIM0_UART_DM_RFWR_ADDR,m,v,HWIO_MSS_UIM0_UART_DM_RFWR_IN)
#define HWIO_MSS_UIM0_UART_DM_RFWR_RFW_BMSK                                                                     0xffffffff
#define HWIO_MSS_UIM0_UART_DM_RFWR_RFW_SHFT                                                                            0x0

#define HWIO_MSS_UIM0_UART_DM_HCR_ADDR                                                                          (MSS_UIM0_UART_DM_REG_BASE      + 0x00000024)
#define HWIO_MSS_UIM0_UART_DM_HCR_OFFS                                                                          (MSS_UIM0_UART_DM_REG_BASE_OFFS + 0x00000024)
#define HWIO_MSS_UIM0_UART_DM_HCR_RMSK                                                                                0xff
#define HWIO_MSS_UIM0_UART_DM_HCR_IN          \
        in_dword_masked(HWIO_MSS_UIM0_UART_DM_HCR_ADDR, HWIO_MSS_UIM0_UART_DM_HCR_RMSK)
#define HWIO_MSS_UIM0_UART_DM_HCR_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM0_UART_DM_HCR_ADDR, m)
#define HWIO_MSS_UIM0_UART_DM_HCR_OUT(v)      \
        out_dword(HWIO_MSS_UIM0_UART_DM_HCR_ADDR,v)
#define HWIO_MSS_UIM0_UART_DM_HCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_UIM0_UART_DM_HCR_ADDR,m,v,HWIO_MSS_UIM0_UART_DM_HCR_IN)
#define HWIO_MSS_UIM0_UART_DM_HCR_DATA_BMSK                                                                           0xff
#define HWIO_MSS_UIM0_UART_DM_HCR_DATA_SHFT                                                                            0x0

#define HWIO_MSS_UIM0_UART_DM_DMRX_ADDR                                                                         (MSS_UIM0_UART_DM_REG_BASE      + 0x00000034)
#define HWIO_MSS_UIM0_UART_DM_DMRX_OFFS                                                                         (MSS_UIM0_UART_DM_REG_BASE_OFFS + 0x00000034)
#define HWIO_MSS_UIM0_UART_DM_DMRX_RMSK                                                                          0x1ffffff
#define HWIO_MSS_UIM0_UART_DM_DMRX_IN          \
        in_dword_masked(HWIO_MSS_UIM0_UART_DM_DMRX_ADDR, HWIO_MSS_UIM0_UART_DM_DMRX_RMSK)
#define HWIO_MSS_UIM0_UART_DM_DMRX_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM0_UART_DM_DMRX_ADDR, m)
#define HWIO_MSS_UIM0_UART_DM_DMRX_OUT(v)      \
        out_dword(HWIO_MSS_UIM0_UART_DM_DMRX_ADDR,v)
#define HWIO_MSS_UIM0_UART_DM_DMRX_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_UIM0_UART_DM_DMRX_ADDR,m,v,HWIO_MSS_UIM0_UART_DM_DMRX_IN)
#define HWIO_MSS_UIM0_UART_DM_DMRX_RX_DM_CRCI_CHARS_BMSK                                                         0x1ffffff
#define HWIO_MSS_UIM0_UART_DM_DMRX_RX_DM_CRCI_CHARS_SHFT                                                               0x0

#define HWIO_MSS_UIM0_UART_DM_IRDA_RX_TOTAL_SNAP_DEPRECATED_ADDR                                                (MSS_UIM0_UART_DM_REG_BASE      + 0x00000038)
#define HWIO_MSS_UIM0_UART_DM_IRDA_RX_TOTAL_SNAP_DEPRECATED_OFFS                                                (MSS_UIM0_UART_DM_REG_BASE_OFFS + 0x00000038)
#define HWIO_MSS_UIM0_UART_DM_IRDA_RX_TOTAL_SNAP_DEPRECATED_RMSK                                                0xffffffff
#define HWIO_MSS_UIM0_UART_DM_IRDA_RX_TOTAL_SNAP_DEPRECATED_IN          \
        in_dword_masked(HWIO_MSS_UIM0_UART_DM_IRDA_RX_TOTAL_SNAP_DEPRECATED_ADDR, HWIO_MSS_UIM0_UART_DM_IRDA_RX_TOTAL_SNAP_DEPRECATED_RMSK)
#define HWIO_MSS_UIM0_UART_DM_IRDA_RX_TOTAL_SNAP_DEPRECATED_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM0_UART_DM_IRDA_RX_TOTAL_SNAP_DEPRECATED_ADDR, m)
#define HWIO_MSS_UIM0_UART_DM_IRDA_RX_TOTAL_SNAP_DEPRECATED_OUT(v)      \
        out_dword(HWIO_MSS_UIM0_UART_DM_IRDA_RX_TOTAL_SNAP_DEPRECATED_ADDR,v)
#define HWIO_MSS_UIM0_UART_DM_IRDA_RX_TOTAL_SNAP_DEPRECATED_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_UIM0_UART_DM_IRDA_RX_TOTAL_SNAP_DEPRECATED_ADDR,m,v,HWIO_MSS_UIM0_UART_DM_IRDA_RX_TOTAL_SNAP_DEPRECATED_IN)
#define HWIO_MSS_UIM0_UART_DM_IRDA_RX_TOTAL_SNAP_DEPRECATED_UART_DM_IRDA_RX_TOTAL_SNAP_DEPRECATED_BMSK          0xffffffff
#define HWIO_MSS_UIM0_UART_DM_IRDA_RX_TOTAL_SNAP_DEPRECATED_UART_DM_IRDA_RX_TOTAL_SNAP_DEPRECATED_SHFT                 0x0

#define HWIO_MSS_UIM0_UART_DM_DMEN_ADDR                                                                         (MSS_UIM0_UART_DM_REG_BASE      + 0x0000003c)
#define HWIO_MSS_UIM0_UART_DM_DMEN_OFFS                                                                         (MSS_UIM0_UART_DM_REG_BASE_OFFS + 0x0000003c)
#define HWIO_MSS_UIM0_UART_DM_DMEN_RMSK                                                                               0x3c
#define HWIO_MSS_UIM0_UART_DM_DMEN_IN          \
        in_dword_masked(HWIO_MSS_UIM0_UART_DM_DMEN_ADDR, HWIO_MSS_UIM0_UART_DM_DMEN_RMSK)
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
#define HWIO_MSS_UIM0_UART_DM_NO_CHARS_FOR_TX_OFFS                                                              (MSS_UIM0_UART_DM_REG_BASE_OFFS + 0x00000040)
#define HWIO_MSS_UIM0_UART_DM_NO_CHARS_FOR_TX_RMSK                                                                0xffffff
#define HWIO_MSS_UIM0_UART_DM_NO_CHARS_FOR_TX_IN          \
        in_dword_masked(HWIO_MSS_UIM0_UART_DM_NO_CHARS_FOR_TX_ADDR, HWIO_MSS_UIM0_UART_DM_NO_CHARS_FOR_TX_RMSK)
#define HWIO_MSS_UIM0_UART_DM_NO_CHARS_FOR_TX_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM0_UART_DM_NO_CHARS_FOR_TX_ADDR, m)
#define HWIO_MSS_UIM0_UART_DM_NO_CHARS_FOR_TX_OUT(v)      \
        out_dword(HWIO_MSS_UIM0_UART_DM_NO_CHARS_FOR_TX_ADDR,v)
#define HWIO_MSS_UIM0_UART_DM_NO_CHARS_FOR_TX_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_UIM0_UART_DM_NO_CHARS_FOR_TX_ADDR,m,v,HWIO_MSS_UIM0_UART_DM_NO_CHARS_FOR_TX_IN)
#define HWIO_MSS_UIM0_UART_DM_NO_CHARS_FOR_TX_TX_TOTAL_TRANS_LEN_BMSK                                             0xffffff
#define HWIO_MSS_UIM0_UART_DM_NO_CHARS_FOR_TX_TX_TOTAL_TRANS_LEN_SHFT                                                  0x0

#define HWIO_MSS_UIM0_UART_DM_BADR_ADDR                                                                         (MSS_UIM0_UART_DM_REG_BASE      + 0x00000044)
#define HWIO_MSS_UIM0_UART_DM_BADR_OFFS                                                                         (MSS_UIM0_UART_DM_REG_BASE_OFFS + 0x00000044)
#define HWIO_MSS_UIM0_UART_DM_BADR_RMSK                                                                         0xfffffffc
#define HWIO_MSS_UIM0_UART_DM_BADR_IN          \
        in_dword_masked(HWIO_MSS_UIM0_UART_DM_BADR_ADDR, HWIO_MSS_UIM0_UART_DM_BADR_RMSK)
#define HWIO_MSS_UIM0_UART_DM_BADR_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM0_UART_DM_BADR_ADDR, m)
#define HWIO_MSS_UIM0_UART_DM_BADR_OUT(v)      \
        out_dword(HWIO_MSS_UIM0_UART_DM_BADR_ADDR,v)
#define HWIO_MSS_UIM0_UART_DM_BADR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_UIM0_UART_DM_BADR_ADDR,m,v,HWIO_MSS_UIM0_UART_DM_BADR_IN)
#define HWIO_MSS_UIM0_UART_DM_BADR_RX_BASE_ADDR_BMSK                                                            0xfffffffc
#define HWIO_MSS_UIM0_UART_DM_BADR_RX_BASE_ADDR_SHFT                                                                   0x2

#define HWIO_MSS_UIM0_UART_DM_TESTSL_ADDR                                                                       (MSS_UIM0_UART_DM_REG_BASE      + 0x00000048)
#define HWIO_MSS_UIM0_UART_DM_TESTSL_OFFS                                                                       (MSS_UIM0_UART_DM_REG_BASE_OFFS + 0x00000048)
#define HWIO_MSS_UIM0_UART_DM_TESTSL_RMSK                                                                             0x1f
#define HWIO_MSS_UIM0_UART_DM_TESTSL_IN          \
        in_dword_masked(HWIO_MSS_UIM0_UART_DM_TESTSL_ADDR, HWIO_MSS_UIM0_UART_DM_TESTSL_RMSK)
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
#define HWIO_MSS_UIM0_UART_DM_TXFS_OFFS                                                                         (MSS_UIM0_UART_DM_REG_BASE_OFFS + 0x0000004c)
#define HWIO_MSS_UIM0_UART_DM_TXFS_RMSK                                                                         0xffffffff
#define HWIO_MSS_UIM0_UART_DM_TXFS_IN          \
        in_dword_masked(HWIO_MSS_UIM0_UART_DM_TXFS_ADDR, HWIO_MSS_UIM0_UART_DM_TXFS_RMSK)
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
#define HWIO_MSS_UIM0_UART_DM_RXFS_OFFS                                                                         (MSS_UIM0_UART_DM_REG_BASE_OFFS + 0x00000050)
#define HWIO_MSS_UIM0_UART_DM_RXFS_RMSK                                                                         0xffffffff
#define HWIO_MSS_UIM0_UART_DM_RXFS_IN          \
        in_dword_masked(HWIO_MSS_UIM0_UART_DM_RXFS_ADDR, HWIO_MSS_UIM0_UART_DM_RXFS_RMSK)
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
#define HWIO_MSS_UIM0_UART_DM_MISR_MODE_OFFS                                                                    (MSS_UIM0_UART_DM_REG_BASE_OFFS + 0x00000060)
#define HWIO_MSS_UIM0_UART_DM_MISR_MODE_RMSK                                                                           0x3
#define HWIO_MSS_UIM0_UART_DM_MISR_MODE_IN          \
        in_dword_masked(HWIO_MSS_UIM0_UART_DM_MISR_MODE_ADDR, HWIO_MSS_UIM0_UART_DM_MISR_MODE_RMSK)
#define HWIO_MSS_UIM0_UART_DM_MISR_MODE_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM0_UART_DM_MISR_MODE_ADDR, m)
#define HWIO_MSS_UIM0_UART_DM_MISR_MODE_OUT(v)      \
        out_dword(HWIO_MSS_UIM0_UART_DM_MISR_MODE_ADDR,v)
#define HWIO_MSS_UIM0_UART_DM_MISR_MODE_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_UIM0_UART_DM_MISR_MODE_ADDR,m,v,HWIO_MSS_UIM0_UART_DM_MISR_MODE_IN)
#define HWIO_MSS_UIM0_UART_DM_MISR_MODE_MODE_BMSK                                                                      0x3
#define HWIO_MSS_UIM0_UART_DM_MISR_MODE_MODE_SHFT                                                                      0x0

#define HWIO_MSS_UIM0_UART_DM_MISR_RESET_ADDR                                                                   (MSS_UIM0_UART_DM_REG_BASE      + 0x00000064)
#define HWIO_MSS_UIM0_UART_DM_MISR_RESET_OFFS                                                                   (MSS_UIM0_UART_DM_REG_BASE_OFFS + 0x00000064)
#define HWIO_MSS_UIM0_UART_DM_MISR_RESET_RMSK                                                                          0x1
#define HWIO_MSS_UIM0_UART_DM_MISR_RESET_OUT(v)      \
        out_dword(HWIO_MSS_UIM0_UART_DM_MISR_RESET_ADDR,v)
#define HWIO_MSS_UIM0_UART_DM_MISR_RESET_RESET_BMSK                                                                    0x1
#define HWIO_MSS_UIM0_UART_DM_MISR_RESET_RESET_SHFT                                                                    0x0

#define HWIO_MSS_UIM0_UART_DM_MISR_EXPORT_ADDR                                                                  (MSS_UIM0_UART_DM_REG_BASE      + 0x00000068)
#define HWIO_MSS_UIM0_UART_DM_MISR_EXPORT_OFFS                                                                  (MSS_UIM0_UART_DM_REG_BASE_OFFS + 0x00000068)
#define HWIO_MSS_UIM0_UART_DM_MISR_EXPORT_RMSK                                                                         0x1
#define HWIO_MSS_UIM0_UART_DM_MISR_EXPORT_IN          \
        in_dword_masked(HWIO_MSS_UIM0_UART_DM_MISR_EXPORT_ADDR, HWIO_MSS_UIM0_UART_DM_MISR_EXPORT_RMSK)
#define HWIO_MSS_UIM0_UART_DM_MISR_EXPORT_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM0_UART_DM_MISR_EXPORT_ADDR, m)
#define HWIO_MSS_UIM0_UART_DM_MISR_EXPORT_OUT(v)      \
        out_dword(HWIO_MSS_UIM0_UART_DM_MISR_EXPORT_ADDR,v)
#define HWIO_MSS_UIM0_UART_DM_MISR_EXPORT_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_UIM0_UART_DM_MISR_EXPORT_ADDR,m,v,HWIO_MSS_UIM0_UART_DM_MISR_EXPORT_IN)
#define HWIO_MSS_UIM0_UART_DM_MISR_EXPORT_EXPORT_BMSK                                                                  0x1
#define HWIO_MSS_UIM0_UART_DM_MISR_EXPORT_EXPORT_SHFT                                                                  0x0

#define HWIO_MSS_UIM0_UART_DM_MISR_VAL_ADDR                                                                     (MSS_UIM0_UART_DM_REG_BASE      + 0x0000006c)
#define HWIO_MSS_UIM0_UART_DM_MISR_VAL_OFFS                                                                     (MSS_UIM0_UART_DM_REG_BASE_OFFS + 0x0000006c)
#define HWIO_MSS_UIM0_UART_DM_MISR_VAL_RMSK                                                                          0x3ff
#define HWIO_MSS_UIM0_UART_DM_MISR_VAL_IN          \
        in_dword_masked(HWIO_MSS_UIM0_UART_DM_MISR_VAL_ADDR, HWIO_MSS_UIM0_UART_DM_MISR_VAL_RMSK)
#define HWIO_MSS_UIM0_UART_DM_MISR_VAL_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM0_UART_DM_MISR_VAL_ADDR, m)
#define HWIO_MSS_UIM0_UART_DM_MISR_VAL_VAL_BMSK                                                                      0x3ff
#define HWIO_MSS_UIM0_UART_DM_MISR_VAL_VAL_SHFT                                                                        0x0

#define HWIO_MSS_UIM0_UART_DM_TF_RF_DEPRECATED_ADDR                                                             (MSS_UIM0_UART_DM_REG_BASE      + 0x00000070)
#define HWIO_MSS_UIM0_UART_DM_TF_RF_DEPRECATED_OFFS                                                             (MSS_UIM0_UART_DM_REG_BASE_OFFS + 0x00000070)
#define HWIO_MSS_UIM0_UART_DM_TF_RF_DEPRECATED_RMSK                                                             0xffffffff
#define HWIO_MSS_UIM0_UART_DM_TF_RF_DEPRECATED_IN          \
        in_dword_masked(HWIO_MSS_UIM0_UART_DM_TF_RF_DEPRECATED_ADDR, HWIO_MSS_UIM0_UART_DM_TF_RF_DEPRECATED_RMSK)
#define HWIO_MSS_UIM0_UART_DM_TF_RF_DEPRECATED_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM0_UART_DM_TF_RF_DEPRECATED_ADDR, m)
#define HWIO_MSS_UIM0_UART_DM_TF_RF_DEPRECATED_OUT(v)      \
        out_dword(HWIO_MSS_UIM0_UART_DM_TF_RF_DEPRECATED_ADDR,v)
#define HWIO_MSS_UIM0_UART_DM_TF_RF_DEPRECATED_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_UIM0_UART_DM_TF_RF_DEPRECATED_ADDR,m,v,HWIO_MSS_UIM0_UART_DM_TF_RF_DEPRECATED_IN)
#define HWIO_MSS_UIM0_UART_DM_TF_RF_DEPRECATED_UART_DM_TF_RF_DEPRECATED_BMSK                                    0xffffffff
#define HWIO_MSS_UIM0_UART_DM_TF_RF_DEPRECATED_UART_DM_TF_RF_DEPRECATED_SHFT                                           0x0

#define HWIO_MSS_UIM0_UART_DM_TF_RF_2_DEPRECATED_ADDR                                                           (MSS_UIM0_UART_DM_REG_BASE      + 0x00000074)
#define HWIO_MSS_UIM0_UART_DM_TF_RF_2_DEPRECATED_OFFS                                                           (MSS_UIM0_UART_DM_REG_BASE_OFFS + 0x00000074)
#define HWIO_MSS_UIM0_UART_DM_TF_RF_2_DEPRECATED_RMSK                                                           0xffffffff
#define HWIO_MSS_UIM0_UART_DM_TF_RF_2_DEPRECATED_IN          \
        in_dword_masked(HWIO_MSS_UIM0_UART_DM_TF_RF_2_DEPRECATED_ADDR, HWIO_MSS_UIM0_UART_DM_TF_RF_2_DEPRECATED_RMSK)
#define HWIO_MSS_UIM0_UART_DM_TF_RF_2_DEPRECATED_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM0_UART_DM_TF_RF_2_DEPRECATED_ADDR, m)
#define HWIO_MSS_UIM0_UART_DM_TF_RF_2_DEPRECATED_OUT(v)      \
        out_dword(HWIO_MSS_UIM0_UART_DM_TF_RF_2_DEPRECATED_ADDR,v)
#define HWIO_MSS_UIM0_UART_DM_TF_RF_2_DEPRECATED_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_UIM0_UART_DM_TF_RF_2_DEPRECATED_ADDR,m,v,HWIO_MSS_UIM0_UART_DM_TF_RF_2_DEPRECATED_IN)
#define HWIO_MSS_UIM0_UART_DM_TF_RF_2_DEPRECATED_UART_DM_TF_RF_2_DEPRECATED_BMSK                                0xffffffff
#define HWIO_MSS_UIM0_UART_DM_TF_RF_2_DEPRECATED_UART_DM_TF_RF_2_DEPRECATED_SHFT                                       0x0

#define HWIO_MSS_UIM0_UART_DM_TF_RF_3_DEPRECATED_ADDR                                                           (MSS_UIM0_UART_DM_REG_BASE      + 0x00000078)
#define HWIO_MSS_UIM0_UART_DM_TF_RF_3_DEPRECATED_OFFS                                                           (MSS_UIM0_UART_DM_REG_BASE_OFFS + 0x00000078)
#define HWIO_MSS_UIM0_UART_DM_TF_RF_3_DEPRECATED_RMSK                                                           0xffffffff
#define HWIO_MSS_UIM0_UART_DM_TF_RF_3_DEPRECATED_IN          \
        in_dword_masked(HWIO_MSS_UIM0_UART_DM_TF_RF_3_DEPRECATED_ADDR, HWIO_MSS_UIM0_UART_DM_TF_RF_3_DEPRECATED_RMSK)
#define HWIO_MSS_UIM0_UART_DM_TF_RF_3_DEPRECATED_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM0_UART_DM_TF_RF_3_DEPRECATED_ADDR, m)
#define HWIO_MSS_UIM0_UART_DM_TF_RF_3_DEPRECATED_OUT(v)      \
        out_dword(HWIO_MSS_UIM0_UART_DM_TF_RF_3_DEPRECATED_ADDR,v)
#define HWIO_MSS_UIM0_UART_DM_TF_RF_3_DEPRECATED_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_UIM0_UART_DM_TF_RF_3_DEPRECATED_ADDR,m,v,HWIO_MSS_UIM0_UART_DM_TF_RF_3_DEPRECATED_IN)
#define HWIO_MSS_UIM0_UART_DM_TF_RF_3_DEPRECATED_UART_DM_TF_RF_3_DEPRECATED_BMSK                                0xffffffff
#define HWIO_MSS_UIM0_UART_DM_TF_RF_3_DEPRECATED_UART_DM_TF_RF_3_DEPRECATED_SHFT                                       0x0

#define HWIO_MSS_UIM0_UART_DM_TF_RF_4_DEPRECATED_ADDR                                                           (MSS_UIM0_UART_DM_REG_BASE      + 0x0000007c)
#define HWIO_MSS_UIM0_UART_DM_TF_RF_4_DEPRECATED_OFFS                                                           (MSS_UIM0_UART_DM_REG_BASE_OFFS + 0x0000007c)
#define HWIO_MSS_UIM0_UART_DM_TF_RF_4_DEPRECATED_RMSK                                                           0xffffffff
#define HWIO_MSS_UIM0_UART_DM_TF_RF_4_DEPRECATED_IN          \
        in_dword_masked(HWIO_MSS_UIM0_UART_DM_TF_RF_4_DEPRECATED_ADDR, HWIO_MSS_UIM0_UART_DM_TF_RF_4_DEPRECATED_RMSK)
#define HWIO_MSS_UIM0_UART_DM_TF_RF_4_DEPRECATED_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM0_UART_DM_TF_RF_4_DEPRECATED_ADDR, m)
#define HWIO_MSS_UIM0_UART_DM_TF_RF_4_DEPRECATED_OUT(v)      \
        out_dword(HWIO_MSS_UIM0_UART_DM_TF_RF_4_DEPRECATED_ADDR,v)
#define HWIO_MSS_UIM0_UART_DM_TF_RF_4_DEPRECATED_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_UIM0_UART_DM_TF_RF_4_DEPRECATED_ADDR,m,v,HWIO_MSS_UIM0_UART_DM_TF_RF_4_DEPRECATED_IN)
#define HWIO_MSS_UIM0_UART_DM_TF_RF_4_DEPRECATED_UART_DM_TF_RF_4_DEPRECATED_BMSK                                0xffffffff
#define HWIO_MSS_UIM0_UART_DM_TF_RF_4_DEPRECATED_UART_DM_TF_RF_4_DEPRECATED_SHFT                                       0x0

#define HWIO_MSS_UIM0_UART_DM_SIM_CFG_ADDR                                                                      (MSS_UIM0_UART_DM_REG_BASE      + 0x00000080)
#define HWIO_MSS_UIM0_UART_DM_SIM_CFG_OFFS                                                                      (MSS_UIM0_UART_DM_REG_BASE_OFFS + 0x00000080)
#define HWIO_MSS_UIM0_UART_DM_SIM_CFG_RMSK                                                                         0x3ffef
#define HWIO_MSS_UIM0_UART_DM_SIM_CFG_IN          \
        in_dword_masked(HWIO_MSS_UIM0_UART_DM_SIM_CFG_ADDR, HWIO_MSS_UIM0_UART_DM_SIM_CFG_RMSK)
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
#define HWIO_MSS_UIM0_UART_DM_TEST_WR_ADDR_OFFS                                                                 (MSS_UIM0_UART_DM_REG_BASE_OFFS + 0x00000084)
#define HWIO_MSS_UIM0_UART_DM_TEST_WR_ADDR_RMSK                                                                 0xffffffff
#define HWIO_MSS_UIM0_UART_DM_TEST_WR_ADDR_IN          \
        in_dword_masked(HWIO_MSS_UIM0_UART_DM_TEST_WR_ADDR_ADDR, HWIO_MSS_UIM0_UART_DM_TEST_WR_ADDR_RMSK)
#define HWIO_MSS_UIM0_UART_DM_TEST_WR_ADDR_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM0_UART_DM_TEST_WR_ADDR_ADDR, m)
#define HWIO_MSS_UIM0_UART_DM_TEST_WR_ADDR_OUT(v)      \
        out_dword(HWIO_MSS_UIM0_UART_DM_TEST_WR_ADDR_ADDR,v)
#define HWIO_MSS_UIM0_UART_DM_TEST_WR_ADDR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_UIM0_UART_DM_TEST_WR_ADDR_ADDR,m,v,HWIO_MSS_UIM0_UART_DM_TEST_WR_ADDR_IN)
#define HWIO_MSS_UIM0_UART_DM_TEST_WR_ADDR_TEST_WR_ADDR_BMSK                                                    0xffffffff
#define HWIO_MSS_UIM0_UART_DM_TEST_WR_ADDR_TEST_WR_ADDR_SHFT                                                           0x0

#define HWIO_MSS_UIM0_UART_DM_TEST_WR_DATA_ADDR                                                                 (MSS_UIM0_UART_DM_REG_BASE      + 0x00000088)
#define HWIO_MSS_UIM0_UART_DM_TEST_WR_DATA_OFFS                                                                 (MSS_UIM0_UART_DM_REG_BASE_OFFS + 0x00000088)
#define HWIO_MSS_UIM0_UART_DM_TEST_WR_DATA_RMSK                                                                 0xffffffff
#define HWIO_MSS_UIM0_UART_DM_TEST_WR_DATA_OUT(v)      \
        out_dword(HWIO_MSS_UIM0_UART_DM_TEST_WR_DATA_ADDR,v)
#define HWIO_MSS_UIM0_UART_DM_TEST_WR_DATA_TEST_WR_DATA_BMSK                                                    0xffffffff
#define HWIO_MSS_UIM0_UART_DM_TEST_WR_DATA_TEST_WR_DATA_SHFT                                                           0x0

#define HWIO_MSS_UIM0_UART_DM_TEST_RD_ADDR_ADDR                                                                 (MSS_UIM0_UART_DM_REG_BASE      + 0x0000008c)
#define HWIO_MSS_UIM0_UART_DM_TEST_RD_ADDR_OFFS                                                                 (MSS_UIM0_UART_DM_REG_BASE_OFFS + 0x0000008c)
#define HWIO_MSS_UIM0_UART_DM_TEST_RD_ADDR_RMSK                                                                 0xffffffff
#define HWIO_MSS_UIM0_UART_DM_TEST_RD_ADDR_IN          \
        in_dword_masked(HWIO_MSS_UIM0_UART_DM_TEST_RD_ADDR_ADDR, HWIO_MSS_UIM0_UART_DM_TEST_RD_ADDR_RMSK)
#define HWIO_MSS_UIM0_UART_DM_TEST_RD_ADDR_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM0_UART_DM_TEST_RD_ADDR_ADDR, m)
#define HWIO_MSS_UIM0_UART_DM_TEST_RD_ADDR_OUT(v)      \
        out_dword(HWIO_MSS_UIM0_UART_DM_TEST_RD_ADDR_ADDR,v)
#define HWIO_MSS_UIM0_UART_DM_TEST_RD_ADDR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_UIM0_UART_DM_TEST_RD_ADDR_ADDR,m,v,HWIO_MSS_UIM0_UART_DM_TEST_RD_ADDR_IN)
#define HWIO_MSS_UIM0_UART_DM_TEST_RD_ADDR_TEST_RD_ADDR_BMSK                                                    0xffffffff
#define HWIO_MSS_UIM0_UART_DM_TEST_RD_ADDR_TEST_RD_ADDR_SHFT                                                           0x0

#define HWIO_MSS_UIM0_UART_DM_TEST_RD_DATA_ADDR                                                                 (MSS_UIM0_UART_DM_REG_BASE      + 0x00000090)
#define HWIO_MSS_UIM0_UART_DM_TEST_RD_DATA_OFFS                                                                 (MSS_UIM0_UART_DM_REG_BASE_OFFS + 0x00000090)
#define HWIO_MSS_UIM0_UART_DM_TEST_RD_DATA_RMSK                                                                 0xffffffff
#define HWIO_MSS_UIM0_UART_DM_TEST_RD_DATA_IN          \
        in_dword_masked(HWIO_MSS_UIM0_UART_DM_TEST_RD_DATA_ADDR, HWIO_MSS_UIM0_UART_DM_TEST_RD_DATA_RMSK)
#define HWIO_MSS_UIM0_UART_DM_TEST_RD_DATA_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM0_UART_DM_TEST_RD_DATA_ADDR, m)
#define HWIO_MSS_UIM0_UART_DM_TEST_RD_DATA_TEST_RD_DATA_BMSK                                                    0xffffffff
#define HWIO_MSS_UIM0_UART_DM_TEST_RD_DATA_TEST_RD_DATA_SHFT                                                           0x0

#define HWIO_MSS_UIM0_UART_DM_CSR_ADDR                                                                          (MSS_UIM0_UART_DM_REG_BASE      + 0x000000a0)
#define HWIO_MSS_UIM0_UART_DM_CSR_OFFS                                                                          (MSS_UIM0_UART_DM_REG_BASE_OFFS + 0x000000a0)
#define HWIO_MSS_UIM0_UART_DM_CSR_RMSK                                                                                0xff
#define HWIO_MSS_UIM0_UART_DM_CSR_IN          \
        in_dword_masked(HWIO_MSS_UIM0_UART_DM_CSR_ADDR, HWIO_MSS_UIM0_UART_DM_CSR_RMSK)
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
#define HWIO_MSS_UIM0_UART_DM_SR_OFFS                                                                           (MSS_UIM0_UART_DM_REG_BASE_OFFS + 0x000000a4)
#define HWIO_MSS_UIM0_UART_DM_SR_RMSK                                                                                0xfff
#define HWIO_MSS_UIM0_UART_DM_SR_IN          \
        in_dword_masked(HWIO_MSS_UIM0_UART_DM_SR_ADDR, HWIO_MSS_UIM0_UART_DM_SR_RMSK)
#define HWIO_MSS_UIM0_UART_DM_SR_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM0_UART_DM_SR_ADDR, m)
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
#define HWIO_MSS_UIM0_UART_DM_CR_OFFS                                                                           (MSS_UIM0_UART_DM_REG_BASE_OFFS + 0x000000a8)
#define HWIO_MSS_UIM0_UART_DM_CR_RMSK                                                                                0xfff
#define HWIO_MSS_UIM0_UART_DM_CR_IN          \
        in_dword_masked(HWIO_MSS_UIM0_UART_DM_CR_ADDR, HWIO_MSS_UIM0_UART_DM_CR_RMSK)
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
#define HWIO_MSS_UIM0_UART_DM_MISR_OFFS                                                                         (MSS_UIM0_UART_DM_REG_BASE_OFFS + 0x000000ac)
#define HWIO_MSS_UIM0_UART_DM_MISR_RMSK                                                                            0x1ffff
#define HWIO_MSS_UIM0_UART_DM_MISR_IN          \
        in_dword_masked(HWIO_MSS_UIM0_UART_DM_MISR_ADDR, HWIO_MSS_UIM0_UART_DM_MISR_RMSK)
#define HWIO_MSS_UIM0_UART_DM_MISR_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM0_UART_DM_MISR_ADDR, m)
#define HWIO_MSS_UIM0_UART_DM_MISR_UART_MISR_BMSK                                                                  0x1ffff
#define HWIO_MSS_UIM0_UART_DM_MISR_UART_MISR_SHFT                                                                      0x0

#define HWIO_MSS_UIM0_UART_DM_IMR_ADDR                                                                          (MSS_UIM0_UART_DM_REG_BASE      + 0x000000b0)
#define HWIO_MSS_UIM0_UART_DM_IMR_OFFS                                                                          (MSS_UIM0_UART_DM_REG_BASE_OFFS + 0x000000b0)
#define HWIO_MSS_UIM0_UART_DM_IMR_RMSK                                                                             0x1ffff
#define HWIO_MSS_UIM0_UART_DM_IMR_IN          \
        in_dword_masked(HWIO_MSS_UIM0_UART_DM_IMR_ADDR, HWIO_MSS_UIM0_UART_DM_IMR_RMSK)
#define HWIO_MSS_UIM0_UART_DM_IMR_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM0_UART_DM_IMR_ADDR, m)
#define HWIO_MSS_UIM0_UART_DM_IMR_OUT(v)      \
        out_dword(HWIO_MSS_UIM0_UART_DM_IMR_ADDR,v)
#define HWIO_MSS_UIM0_UART_DM_IMR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_UIM0_UART_DM_IMR_ADDR,m,v,HWIO_MSS_UIM0_UART_DM_IMR_IN)
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
#define HWIO_MSS_UIM0_UART_DM_ISR_OFFS                                                                          (MSS_UIM0_UART_DM_REG_BASE_OFFS + 0x000000b4)
#define HWIO_MSS_UIM0_UART_DM_ISR_RMSK                                                                             0x1ffff
#define HWIO_MSS_UIM0_UART_DM_ISR_IN          \
        in_dword_masked(HWIO_MSS_UIM0_UART_DM_ISR_ADDR, HWIO_MSS_UIM0_UART_DM_ISR_RMSK)
#define HWIO_MSS_UIM0_UART_DM_ISR_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM0_UART_DM_ISR_ADDR, m)
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
#define HWIO_MSS_UIM0_UART_DM_IRDA_OFFS                                                                         (MSS_UIM0_UART_DM_REG_BASE_OFFS + 0x000000b8)
#define HWIO_MSS_UIM0_UART_DM_IRDA_RMSK                                                                               0x1f
#define HWIO_MSS_UIM0_UART_DM_IRDA_IN          \
        in_dword_masked(HWIO_MSS_UIM0_UART_DM_IRDA_ADDR, HWIO_MSS_UIM0_UART_DM_IRDA_RMSK)
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
#define HWIO_MSS_UIM0_UART_DM_RX_TOTAL_SNAP_OFFS                                                                (MSS_UIM0_UART_DM_REG_BASE_OFFS + 0x000000bc)
#define HWIO_MSS_UIM0_UART_DM_RX_TOTAL_SNAP_RMSK                                                                  0xffffff
#define HWIO_MSS_UIM0_UART_DM_RX_TOTAL_SNAP_IN          \
        in_dword_masked(HWIO_MSS_UIM0_UART_DM_RX_TOTAL_SNAP_ADDR, HWIO_MSS_UIM0_UART_DM_RX_TOTAL_SNAP_RMSK)
#define HWIO_MSS_UIM0_UART_DM_RX_TOTAL_SNAP_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM0_UART_DM_RX_TOTAL_SNAP_ADDR, m)
#define HWIO_MSS_UIM0_UART_DM_RX_TOTAL_SNAP_RX_TOTAL_BYTES_BMSK                                                   0xffffff
#define HWIO_MSS_UIM0_UART_DM_RX_TOTAL_SNAP_RX_TOTAL_BYTES_SHFT                                                        0x0

#define HWIO_MSS_UIM0_UART_DM_WWT_TIMEOUT_ADDR                                                                  (MSS_UIM0_UART_DM_REG_BASE      + 0x000000c0)
#define HWIO_MSS_UIM0_UART_DM_WWT_TIMEOUT_OFFS                                                                  (MSS_UIM0_UART_DM_REG_BASE_OFFS + 0x000000c0)
#define HWIO_MSS_UIM0_UART_DM_WWT_TIMEOUT_RMSK                                                                   0x1ffffff
#define HWIO_MSS_UIM0_UART_DM_WWT_TIMEOUT_IN          \
        in_dword_masked(HWIO_MSS_UIM0_UART_DM_WWT_TIMEOUT_ADDR, HWIO_MSS_UIM0_UART_DM_WWT_TIMEOUT_RMSK)
#define HWIO_MSS_UIM0_UART_DM_WWT_TIMEOUT_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM0_UART_DM_WWT_TIMEOUT_ADDR, m)
#define HWIO_MSS_UIM0_UART_DM_WWT_TIMEOUT_OUT(v)      \
        out_dword(HWIO_MSS_UIM0_UART_DM_WWT_TIMEOUT_ADDR,v)
#define HWIO_MSS_UIM0_UART_DM_WWT_TIMEOUT_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_UIM0_UART_DM_WWT_TIMEOUT_ADDR,m,v,HWIO_MSS_UIM0_UART_DM_WWT_TIMEOUT_IN)
#define HWIO_MSS_UIM0_UART_DM_WWT_TIMEOUT_WWT_TIMEOUT_BMSK                                                       0x1ffffff
#define HWIO_MSS_UIM0_UART_DM_WWT_TIMEOUT_WWT_TIMEOUT_SHFT                                                             0x0

#define HWIO_MSS_UIM0_UART_DM_CLK_CTRL_ADDR                                                                     (MSS_UIM0_UART_DM_REG_BASE      + 0x000000c4)
#define HWIO_MSS_UIM0_UART_DM_CLK_CTRL_OFFS                                                                     (MSS_UIM0_UART_DM_REG_BASE_OFFS + 0x000000c4)
#define HWIO_MSS_UIM0_UART_DM_CLK_CTRL_RMSK                                                                       0xf06731
#define HWIO_MSS_UIM0_UART_DM_CLK_CTRL_IN          \
        in_dword_masked(HWIO_MSS_UIM0_UART_DM_CLK_CTRL_ADDR, HWIO_MSS_UIM0_UART_DM_CLK_CTRL_RMSK)
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
#define HWIO_MSS_UIM0_UART_DM_BCR_OFFS                                                                          (MSS_UIM0_UART_DM_REG_BASE_OFFS + 0x000000c8)
#define HWIO_MSS_UIM0_UART_DM_BCR_RMSK                                                                                0x37
#define HWIO_MSS_UIM0_UART_DM_BCR_IN          \
        in_dword_masked(HWIO_MSS_UIM0_UART_DM_BCR_ADDR, HWIO_MSS_UIM0_UART_DM_BCR_RMSK)
#define HWIO_MSS_UIM0_UART_DM_BCR_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM0_UART_DM_BCR_ADDR, m)
#define HWIO_MSS_UIM0_UART_DM_BCR_OUT(v)      \
        out_dword(HWIO_MSS_UIM0_UART_DM_BCR_ADDR,v)
#define HWIO_MSS_UIM0_UART_DM_BCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_UIM0_UART_DM_BCR_ADDR,m,v,HWIO_MSS_UIM0_UART_DM_BCR_IN)
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
#define HWIO_MSS_UIM0_UART_DM_RX_TRANS_CTRL_OFFS                                                                (MSS_UIM0_UART_DM_REG_BASE_OFFS + 0x000000cc)
#define HWIO_MSS_UIM0_UART_DM_RX_TRANS_CTRL_RMSK                                                                       0x7
#define HWIO_MSS_UIM0_UART_DM_RX_TRANS_CTRL_IN          \
        in_dword_masked(HWIO_MSS_UIM0_UART_DM_RX_TRANS_CTRL_ADDR, HWIO_MSS_UIM0_UART_DM_RX_TRANS_CTRL_RMSK)
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
#define HWIO_MSS_UIM0_UART_DM_DMRX_DBG_OFFS                                                                     (MSS_UIM0_UART_DM_REG_BASE_OFFS + 0x000000d0)
#define HWIO_MSS_UIM0_UART_DM_DMRX_DBG_RMSK                                                                      0x1ffffff
#define HWIO_MSS_UIM0_UART_DM_DMRX_DBG_IN          \
        in_dword_masked(HWIO_MSS_UIM0_UART_DM_DMRX_DBG_ADDR, HWIO_MSS_UIM0_UART_DM_DMRX_DBG_RMSK)
#define HWIO_MSS_UIM0_UART_DM_DMRX_DBG_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM0_UART_DM_DMRX_DBG_ADDR, m)
#define HWIO_MSS_UIM0_UART_DM_DMRX_DBG_UART_DM_DMRX_VAL_BMSK                                                     0x1ffffff
#define HWIO_MSS_UIM0_UART_DM_DMRX_DBG_UART_DM_DMRX_VAL_SHFT                                                           0x0

#define HWIO_MSS_UIM0_UART_DM_FSM_STATUS_ADDR                                                                   (MSS_UIM0_UART_DM_REG_BASE      + 0x000000d4)
#define HWIO_MSS_UIM0_UART_DM_FSM_STATUS_OFFS                                                                   (MSS_UIM0_UART_DM_REG_BASE_OFFS + 0x000000d4)
#define HWIO_MSS_UIM0_UART_DM_FSM_STATUS_RMSK                                                                   0x37377771
#define HWIO_MSS_UIM0_UART_DM_FSM_STATUS_IN          \
        in_dword_masked(HWIO_MSS_UIM0_UART_DM_FSM_STATUS_ADDR, HWIO_MSS_UIM0_UART_DM_FSM_STATUS_RMSK)
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
#define HWIO_MSS_UIM0_UART_DM_HW_VERSION_OFFS                                                                   (MSS_UIM0_UART_DM_REG_BASE_OFFS + 0x000000d8)
#define HWIO_MSS_UIM0_UART_DM_HW_VERSION_RMSK                                                                   0xffffffff
#define HWIO_MSS_UIM0_UART_DM_HW_VERSION_IN          \
        in_dword_masked(HWIO_MSS_UIM0_UART_DM_HW_VERSION_ADDR, HWIO_MSS_UIM0_UART_DM_HW_VERSION_RMSK)
#define HWIO_MSS_UIM0_UART_DM_HW_VERSION_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM0_UART_DM_HW_VERSION_ADDR, m)
#define HWIO_MSS_UIM0_UART_DM_HW_VERSION_HW_VERSION_MAJOR_BMSK                                                  0xf0000000
#define HWIO_MSS_UIM0_UART_DM_HW_VERSION_HW_VERSION_MAJOR_SHFT                                                        0x1c
#define HWIO_MSS_UIM0_UART_DM_HW_VERSION_HW_VERSION_MINOR_BMSK                                                   0xfff0000
#define HWIO_MSS_UIM0_UART_DM_HW_VERSION_HW_VERSION_MINOR_SHFT                                                        0x10
#define HWIO_MSS_UIM0_UART_DM_HW_VERSION_HW_VERSION_STEP_BMSK                                                       0xffff
#define HWIO_MSS_UIM0_UART_DM_HW_VERSION_HW_VERSION_STEP_SHFT                                                          0x0

#define HWIO_MSS_UIM0_UART_DM_GENERICS_ADDR                                                                     (MSS_UIM0_UART_DM_REG_BASE      + 0x000000dc)
#define HWIO_MSS_UIM0_UART_DM_GENERICS_OFFS                                                                     (MSS_UIM0_UART_DM_REG_BASE_OFFS + 0x000000dc)
#define HWIO_MSS_UIM0_UART_DM_GENERICS_RMSK                                                                           0xff
#define HWIO_MSS_UIM0_UART_DM_GENERICS_IN          \
        in_dword_masked(HWIO_MSS_UIM0_UART_DM_GENERICS_ADDR, HWIO_MSS_UIM0_UART_DM_GENERICS_RMSK)
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

#define HWIO_MSS_UIM0_UART_DM_TF_ADDR                                                                           (MSS_UIM0_UART_DM_REG_BASE      + 0x00000100)
#define HWIO_MSS_UIM0_UART_DM_TF_OFFS                                                                           (MSS_UIM0_UART_DM_REG_BASE_OFFS + 0x00000100)
#define HWIO_MSS_UIM0_UART_DM_TF_RMSK                                                                           0xffffffff
#define HWIO_MSS_UIM0_UART_DM_TF_OUT(v)      \
        out_dword(HWIO_MSS_UIM0_UART_DM_TF_ADDR,v)
#define HWIO_MSS_UIM0_UART_DM_TF_UART_TF_BMSK                                                                   0xffffffff
#define HWIO_MSS_UIM0_UART_DM_TF_UART_TF_SHFT                                                                          0x0

#define HWIO_MSS_UIM0_UART_DM_TF_2_ADDR                                                                         (MSS_UIM0_UART_DM_REG_BASE      + 0x00000104)
#define HWIO_MSS_UIM0_UART_DM_TF_2_OFFS                                                                         (MSS_UIM0_UART_DM_REG_BASE_OFFS + 0x00000104)
#define HWIO_MSS_UIM0_UART_DM_TF_2_RMSK                                                                         0xffffffff
#define HWIO_MSS_UIM0_UART_DM_TF_2_OUT(v)      \
        out_dword(HWIO_MSS_UIM0_UART_DM_TF_2_ADDR,v)
#define HWIO_MSS_UIM0_UART_DM_TF_2_UART_TF_BMSK                                                                 0xffffffff
#define HWIO_MSS_UIM0_UART_DM_TF_2_UART_TF_SHFT                                                                        0x0

#define HWIO_MSS_UIM0_UART_DM_TF_3_ADDR                                                                         (MSS_UIM0_UART_DM_REG_BASE      + 0x00000108)
#define HWIO_MSS_UIM0_UART_DM_TF_3_OFFS                                                                         (MSS_UIM0_UART_DM_REG_BASE_OFFS + 0x00000108)
#define HWIO_MSS_UIM0_UART_DM_TF_3_RMSK                                                                         0xffffffff
#define HWIO_MSS_UIM0_UART_DM_TF_3_OUT(v)      \
        out_dword(HWIO_MSS_UIM0_UART_DM_TF_3_ADDR,v)
#define HWIO_MSS_UIM0_UART_DM_TF_3_UART_TF_BMSK                                                                 0xffffffff
#define HWIO_MSS_UIM0_UART_DM_TF_3_UART_TF_SHFT                                                                        0x0

#define HWIO_MSS_UIM0_UART_DM_TF_4_ADDR                                                                         (MSS_UIM0_UART_DM_REG_BASE      + 0x0000010c)
#define HWIO_MSS_UIM0_UART_DM_TF_4_OFFS                                                                         (MSS_UIM0_UART_DM_REG_BASE_OFFS + 0x0000010c)
#define HWIO_MSS_UIM0_UART_DM_TF_4_RMSK                                                                         0xffffffff
#define HWIO_MSS_UIM0_UART_DM_TF_4_OUT(v)      \
        out_dword(HWIO_MSS_UIM0_UART_DM_TF_4_ADDR,v)
#define HWIO_MSS_UIM0_UART_DM_TF_4_UART_TF_BMSK                                                                 0xffffffff
#define HWIO_MSS_UIM0_UART_DM_TF_4_UART_TF_SHFT                                                                        0x0

#define HWIO_MSS_UIM0_UART_DM_TF_5_ADDR                                                                         (MSS_UIM0_UART_DM_REG_BASE      + 0x00000110)
#define HWIO_MSS_UIM0_UART_DM_TF_5_OFFS                                                                         (MSS_UIM0_UART_DM_REG_BASE_OFFS + 0x00000110)
#define HWIO_MSS_UIM0_UART_DM_TF_5_RMSK                                                                         0xffffffff
#define HWIO_MSS_UIM0_UART_DM_TF_5_OUT(v)      \
        out_dword(HWIO_MSS_UIM0_UART_DM_TF_5_ADDR,v)
#define HWIO_MSS_UIM0_UART_DM_TF_5_UART_TF_BMSK                                                                 0xffffffff
#define HWIO_MSS_UIM0_UART_DM_TF_5_UART_TF_SHFT                                                                        0x0

#define HWIO_MSS_UIM0_UART_DM_TF_6_ADDR                                                                         (MSS_UIM0_UART_DM_REG_BASE      + 0x00000114)
#define HWIO_MSS_UIM0_UART_DM_TF_6_OFFS                                                                         (MSS_UIM0_UART_DM_REG_BASE_OFFS + 0x00000114)
#define HWIO_MSS_UIM0_UART_DM_TF_6_RMSK                                                                         0xffffffff
#define HWIO_MSS_UIM0_UART_DM_TF_6_OUT(v)      \
        out_dword(HWIO_MSS_UIM0_UART_DM_TF_6_ADDR,v)
#define HWIO_MSS_UIM0_UART_DM_TF_6_UART_TF_BMSK                                                                 0xffffffff
#define HWIO_MSS_UIM0_UART_DM_TF_6_UART_TF_SHFT                                                                        0x0

#define HWIO_MSS_UIM0_UART_DM_TF_7_ADDR                                                                         (MSS_UIM0_UART_DM_REG_BASE      + 0x00000118)
#define HWIO_MSS_UIM0_UART_DM_TF_7_OFFS                                                                         (MSS_UIM0_UART_DM_REG_BASE_OFFS + 0x00000118)
#define HWIO_MSS_UIM0_UART_DM_TF_7_RMSK                                                                         0xffffffff
#define HWIO_MSS_UIM0_UART_DM_TF_7_OUT(v)      \
        out_dword(HWIO_MSS_UIM0_UART_DM_TF_7_ADDR,v)
#define HWIO_MSS_UIM0_UART_DM_TF_7_UART_TF_BMSK                                                                 0xffffffff
#define HWIO_MSS_UIM0_UART_DM_TF_7_UART_TF_SHFT                                                                        0x0

#define HWIO_MSS_UIM0_UART_DM_TF_8_ADDR                                                                         (MSS_UIM0_UART_DM_REG_BASE      + 0x0000011c)
#define HWIO_MSS_UIM0_UART_DM_TF_8_OFFS                                                                         (MSS_UIM0_UART_DM_REG_BASE_OFFS + 0x0000011c)
#define HWIO_MSS_UIM0_UART_DM_TF_8_RMSK                                                                         0xffffffff
#define HWIO_MSS_UIM0_UART_DM_TF_8_OUT(v)      \
        out_dword(HWIO_MSS_UIM0_UART_DM_TF_8_ADDR,v)
#define HWIO_MSS_UIM0_UART_DM_TF_8_UART_TF_BMSK                                                                 0xffffffff
#define HWIO_MSS_UIM0_UART_DM_TF_8_UART_TF_SHFT                                                                        0x0

#define HWIO_MSS_UIM0_UART_DM_TF_9_ADDR                                                                         (MSS_UIM0_UART_DM_REG_BASE      + 0x00000120)
#define HWIO_MSS_UIM0_UART_DM_TF_9_OFFS                                                                         (MSS_UIM0_UART_DM_REG_BASE_OFFS + 0x00000120)
#define HWIO_MSS_UIM0_UART_DM_TF_9_RMSK                                                                         0xffffffff
#define HWIO_MSS_UIM0_UART_DM_TF_9_OUT(v)      \
        out_dword(HWIO_MSS_UIM0_UART_DM_TF_9_ADDR,v)
#define HWIO_MSS_UIM0_UART_DM_TF_9_UART_TF_BMSK                                                                 0xffffffff
#define HWIO_MSS_UIM0_UART_DM_TF_9_UART_TF_SHFT                                                                        0x0

#define HWIO_MSS_UIM0_UART_DM_TF_10_ADDR                                                                        (MSS_UIM0_UART_DM_REG_BASE      + 0x00000124)
#define HWIO_MSS_UIM0_UART_DM_TF_10_OFFS                                                                        (MSS_UIM0_UART_DM_REG_BASE_OFFS + 0x00000124)
#define HWIO_MSS_UIM0_UART_DM_TF_10_RMSK                                                                        0xffffffff
#define HWIO_MSS_UIM0_UART_DM_TF_10_OUT(v)      \
        out_dword(HWIO_MSS_UIM0_UART_DM_TF_10_ADDR,v)
#define HWIO_MSS_UIM0_UART_DM_TF_10_UART_TF_BMSK                                                                0xffffffff
#define HWIO_MSS_UIM0_UART_DM_TF_10_UART_TF_SHFT                                                                       0x0

#define HWIO_MSS_UIM0_UART_DM_TF_11_ADDR                                                                        (MSS_UIM0_UART_DM_REG_BASE      + 0x00000128)
#define HWIO_MSS_UIM0_UART_DM_TF_11_OFFS                                                                        (MSS_UIM0_UART_DM_REG_BASE_OFFS + 0x00000128)
#define HWIO_MSS_UIM0_UART_DM_TF_11_RMSK                                                                        0xffffffff
#define HWIO_MSS_UIM0_UART_DM_TF_11_OUT(v)      \
        out_dword(HWIO_MSS_UIM0_UART_DM_TF_11_ADDR,v)
#define HWIO_MSS_UIM0_UART_DM_TF_11_UART_TF_BMSK                                                                0xffffffff
#define HWIO_MSS_UIM0_UART_DM_TF_11_UART_TF_SHFT                                                                       0x0

#define HWIO_MSS_UIM0_UART_DM_TF_12_ADDR                                                                        (MSS_UIM0_UART_DM_REG_BASE      + 0x0000012c)
#define HWIO_MSS_UIM0_UART_DM_TF_12_OFFS                                                                        (MSS_UIM0_UART_DM_REG_BASE_OFFS + 0x0000012c)
#define HWIO_MSS_UIM0_UART_DM_TF_12_RMSK                                                                        0xffffffff
#define HWIO_MSS_UIM0_UART_DM_TF_12_OUT(v)      \
        out_dword(HWIO_MSS_UIM0_UART_DM_TF_12_ADDR,v)
#define HWIO_MSS_UIM0_UART_DM_TF_12_UART_TF_BMSK                                                                0xffffffff
#define HWIO_MSS_UIM0_UART_DM_TF_12_UART_TF_SHFT                                                                       0x0

#define HWIO_MSS_UIM0_UART_DM_TF_13_ADDR                                                                        (MSS_UIM0_UART_DM_REG_BASE      + 0x00000130)
#define HWIO_MSS_UIM0_UART_DM_TF_13_OFFS                                                                        (MSS_UIM0_UART_DM_REG_BASE_OFFS + 0x00000130)
#define HWIO_MSS_UIM0_UART_DM_TF_13_RMSK                                                                        0xffffffff
#define HWIO_MSS_UIM0_UART_DM_TF_13_OUT(v)      \
        out_dword(HWIO_MSS_UIM0_UART_DM_TF_13_ADDR,v)
#define HWIO_MSS_UIM0_UART_DM_TF_13_UART_TF_BMSK                                                                0xffffffff
#define HWIO_MSS_UIM0_UART_DM_TF_13_UART_TF_SHFT                                                                       0x0

#define HWIO_MSS_UIM0_UART_DM_TF_14_ADDR                                                                        (MSS_UIM0_UART_DM_REG_BASE      + 0x00000134)
#define HWIO_MSS_UIM0_UART_DM_TF_14_OFFS                                                                        (MSS_UIM0_UART_DM_REG_BASE_OFFS + 0x00000134)
#define HWIO_MSS_UIM0_UART_DM_TF_14_RMSK                                                                        0xffffffff
#define HWIO_MSS_UIM0_UART_DM_TF_14_OUT(v)      \
        out_dword(HWIO_MSS_UIM0_UART_DM_TF_14_ADDR,v)
#define HWIO_MSS_UIM0_UART_DM_TF_14_UART_TF_BMSK                                                                0xffffffff
#define HWIO_MSS_UIM0_UART_DM_TF_14_UART_TF_SHFT                                                                       0x0

#define HWIO_MSS_UIM0_UART_DM_TF_15_ADDR                                                                        (MSS_UIM0_UART_DM_REG_BASE      + 0x00000138)
#define HWIO_MSS_UIM0_UART_DM_TF_15_OFFS                                                                        (MSS_UIM0_UART_DM_REG_BASE_OFFS + 0x00000138)
#define HWIO_MSS_UIM0_UART_DM_TF_15_RMSK                                                                        0xffffffff
#define HWIO_MSS_UIM0_UART_DM_TF_15_OUT(v)      \
        out_dword(HWIO_MSS_UIM0_UART_DM_TF_15_ADDR,v)
#define HWIO_MSS_UIM0_UART_DM_TF_15_UART_TF_BMSK                                                                0xffffffff
#define HWIO_MSS_UIM0_UART_DM_TF_15_UART_TF_SHFT                                                                       0x0

#define HWIO_MSS_UIM0_UART_DM_TF_16_ADDR                                                                        (MSS_UIM0_UART_DM_REG_BASE      + 0x0000013c)
#define HWIO_MSS_UIM0_UART_DM_TF_16_OFFS                                                                        (MSS_UIM0_UART_DM_REG_BASE_OFFS + 0x0000013c)
#define HWIO_MSS_UIM0_UART_DM_TF_16_RMSK                                                                        0xffffffff
#define HWIO_MSS_UIM0_UART_DM_TF_16_OUT(v)      \
        out_dword(HWIO_MSS_UIM0_UART_DM_TF_16_ADDR,v)
#define HWIO_MSS_UIM0_UART_DM_TF_16_UART_TF_BMSK                                                                0xffffffff
#define HWIO_MSS_UIM0_UART_DM_TF_16_UART_TF_SHFT                                                                       0x0

#define HWIO_MSS_UIM0_UART_DM_RF_ADDR                                                                           (MSS_UIM0_UART_DM_REG_BASE      + 0x00000140)
#define HWIO_MSS_UIM0_UART_DM_RF_OFFS                                                                           (MSS_UIM0_UART_DM_REG_BASE_OFFS + 0x00000140)
#define HWIO_MSS_UIM0_UART_DM_RF_RMSK                                                                           0xffffffff
#define HWIO_MSS_UIM0_UART_DM_RF_IN          \
        in_dword_masked(HWIO_MSS_UIM0_UART_DM_RF_ADDR, HWIO_MSS_UIM0_UART_DM_RF_RMSK)
#define HWIO_MSS_UIM0_UART_DM_RF_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM0_UART_DM_RF_ADDR, m)
#define HWIO_MSS_UIM0_UART_DM_RF_UART_RF_BMSK                                                                   0xffffffff
#define HWIO_MSS_UIM0_UART_DM_RF_UART_RF_SHFT                                                                          0x0

#define HWIO_MSS_UIM0_UART_DM_RF_2_ADDR                                                                         (MSS_UIM0_UART_DM_REG_BASE      + 0x00000144)
#define HWIO_MSS_UIM0_UART_DM_RF_2_OFFS                                                                         (MSS_UIM0_UART_DM_REG_BASE_OFFS + 0x00000144)
#define HWIO_MSS_UIM0_UART_DM_RF_2_RMSK                                                                         0xffffffff
#define HWIO_MSS_UIM0_UART_DM_RF_2_IN          \
        in_dword_masked(HWIO_MSS_UIM0_UART_DM_RF_2_ADDR, HWIO_MSS_UIM0_UART_DM_RF_2_RMSK)
#define HWIO_MSS_UIM0_UART_DM_RF_2_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM0_UART_DM_RF_2_ADDR, m)
#define HWIO_MSS_UIM0_UART_DM_RF_2_UART_RF_BMSK                                                                 0xffffffff
#define HWIO_MSS_UIM0_UART_DM_RF_2_UART_RF_SHFT                                                                        0x0

#define HWIO_MSS_UIM0_UART_DM_RF_3_ADDR                                                                         (MSS_UIM0_UART_DM_REG_BASE      + 0x00000148)
#define HWIO_MSS_UIM0_UART_DM_RF_3_OFFS                                                                         (MSS_UIM0_UART_DM_REG_BASE_OFFS + 0x00000148)
#define HWIO_MSS_UIM0_UART_DM_RF_3_RMSK                                                                         0xffffffff
#define HWIO_MSS_UIM0_UART_DM_RF_3_IN          \
        in_dword_masked(HWIO_MSS_UIM0_UART_DM_RF_3_ADDR, HWIO_MSS_UIM0_UART_DM_RF_3_RMSK)
#define HWIO_MSS_UIM0_UART_DM_RF_3_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM0_UART_DM_RF_3_ADDR, m)
#define HWIO_MSS_UIM0_UART_DM_RF_3_UART_RF_BMSK                                                                 0xffffffff
#define HWIO_MSS_UIM0_UART_DM_RF_3_UART_RF_SHFT                                                                        0x0

#define HWIO_MSS_UIM0_UART_DM_RF_4_ADDR                                                                         (MSS_UIM0_UART_DM_REG_BASE      + 0x0000014c)
#define HWIO_MSS_UIM0_UART_DM_RF_4_OFFS                                                                         (MSS_UIM0_UART_DM_REG_BASE_OFFS + 0x0000014c)
#define HWIO_MSS_UIM0_UART_DM_RF_4_RMSK                                                                         0xffffffff
#define HWIO_MSS_UIM0_UART_DM_RF_4_IN          \
        in_dword_masked(HWIO_MSS_UIM0_UART_DM_RF_4_ADDR, HWIO_MSS_UIM0_UART_DM_RF_4_RMSK)
#define HWIO_MSS_UIM0_UART_DM_RF_4_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM0_UART_DM_RF_4_ADDR, m)
#define HWIO_MSS_UIM0_UART_DM_RF_4_UART_RF_BMSK                                                                 0xffffffff
#define HWIO_MSS_UIM0_UART_DM_RF_4_UART_RF_SHFT                                                                        0x0

#define HWIO_MSS_UIM0_UART_DM_RF_5_ADDR                                                                         (MSS_UIM0_UART_DM_REG_BASE      + 0x00000150)
#define HWIO_MSS_UIM0_UART_DM_RF_5_OFFS                                                                         (MSS_UIM0_UART_DM_REG_BASE_OFFS + 0x00000150)
#define HWIO_MSS_UIM0_UART_DM_RF_5_RMSK                                                                         0xffffffff
#define HWIO_MSS_UIM0_UART_DM_RF_5_IN          \
        in_dword_masked(HWIO_MSS_UIM0_UART_DM_RF_5_ADDR, HWIO_MSS_UIM0_UART_DM_RF_5_RMSK)
#define HWIO_MSS_UIM0_UART_DM_RF_5_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM0_UART_DM_RF_5_ADDR, m)
#define HWIO_MSS_UIM0_UART_DM_RF_5_UART_RF_BMSK                                                                 0xffffffff
#define HWIO_MSS_UIM0_UART_DM_RF_5_UART_RF_SHFT                                                                        0x0

#define HWIO_MSS_UIM0_UART_DM_RF_6_ADDR                                                                         (MSS_UIM0_UART_DM_REG_BASE      + 0x00000154)
#define HWIO_MSS_UIM0_UART_DM_RF_6_OFFS                                                                         (MSS_UIM0_UART_DM_REG_BASE_OFFS + 0x00000154)
#define HWIO_MSS_UIM0_UART_DM_RF_6_RMSK                                                                         0xffffffff
#define HWIO_MSS_UIM0_UART_DM_RF_6_IN          \
        in_dword_masked(HWIO_MSS_UIM0_UART_DM_RF_6_ADDR, HWIO_MSS_UIM0_UART_DM_RF_6_RMSK)
#define HWIO_MSS_UIM0_UART_DM_RF_6_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM0_UART_DM_RF_6_ADDR, m)
#define HWIO_MSS_UIM0_UART_DM_RF_6_UART_RF_BMSK                                                                 0xffffffff
#define HWIO_MSS_UIM0_UART_DM_RF_6_UART_RF_SHFT                                                                        0x0

#define HWIO_MSS_UIM0_UART_DM_RF_7_ADDR                                                                         (MSS_UIM0_UART_DM_REG_BASE      + 0x00000158)
#define HWIO_MSS_UIM0_UART_DM_RF_7_OFFS                                                                         (MSS_UIM0_UART_DM_REG_BASE_OFFS + 0x00000158)
#define HWIO_MSS_UIM0_UART_DM_RF_7_RMSK                                                                         0xffffffff
#define HWIO_MSS_UIM0_UART_DM_RF_7_IN          \
        in_dword_masked(HWIO_MSS_UIM0_UART_DM_RF_7_ADDR, HWIO_MSS_UIM0_UART_DM_RF_7_RMSK)
#define HWIO_MSS_UIM0_UART_DM_RF_7_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM0_UART_DM_RF_7_ADDR, m)
#define HWIO_MSS_UIM0_UART_DM_RF_7_UART_RF_BMSK                                                                 0xffffffff
#define HWIO_MSS_UIM0_UART_DM_RF_7_UART_RF_SHFT                                                                        0x0

#define HWIO_MSS_UIM0_UART_DM_RF_8_ADDR                                                                         (MSS_UIM0_UART_DM_REG_BASE      + 0x0000015c)
#define HWIO_MSS_UIM0_UART_DM_RF_8_OFFS                                                                         (MSS_UIM0_UART_DM_REG_BASE_OFFS + 0x0000015c)
#define HWIO_MSS_UIM0_UART_DM_RF_8_RMSK                                                                         0xffffffff
#define HWIO_MSS_UIM0_UART_DM_RF_8_IN          \
        in_dword_masked(HWIO_MSS_UIM0_UART_DM_RF_8_ADDR, HWIO_MSS_UIM0_UART_DM_RF_8_RMSK)
#define HWIO_MSS_UIM0_UART_DM_RF_8_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM0_UART_DM_RF_8_ADDR, m)
#define HWIO_MSS_UIM0_UART_DM_RF_8_UART_RF_BMSK                                                                 0xffffffff
#define HWIO_MSS_UIM0_UART_DM_RF_8_UART_RF_SHFT                                                                        0x0

#define HWIO_MSS_UIM0_UART_DM_RF_9_ADDR                                                                         (MSS_UIM0_UART_DM_REG_BASE      + 0x00000160)
#define HWIO_MSS_UIM0_UART_DM_RF_9_OFFS                                                                         (MSS_UIM0_UART_DM_REG_BASE_OFFS + 0x00000160)
#define HWIO_MSS_UIM0_UART_DM_RF_9_RMSK                                                                         0xffffffff
#define HWIO_MSS_UIM0_UART_DM_RF_9_IN          \
        in_dword_masked(HWIO_MSS_UIM0_UART_DM_RF_9_ADDR, HWIO_MSS_UIM0_UART_DM_RF_9_RMSK)
#define HWIO_MSS_UIM0_UART_DM_RF_9_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM0_UART_DM_RF_9_ADDR, m)
#define HWIO_MSS_UIM0_UART_DM_RF_9_UART_RF_BMSK                                                                 0xffffffff
#define HWIO_MSS_UIM0_UART_DM_RF_9_UART_RF_SHFT                                                                        0x0

#define HWIO_MSS_UIM0_UART_DM_RF_10_ADDR                                                                        (MSS_UIM0_UART_DM_REG_BASE      + 0x00000164)
#define HWIO_MSS_UIM0_UART_DM_RF_10_OFFS                                                                        (MSS_UIM0_UART_DM_REG_BASE_OFFS + 0x00000164)
#define HWIO_MSS_UIM0_UART_DM_RF_10_RMSK                                                                        0xffffffff
#define HWIO_MSS_UIM0_UART_DM_RF_10_IN          \
        in_dword_masked(HWIO_MSS_UIM0_UART_DM_RF_10_ADDR, HWIO_MSS_UIM0_UART_DM_RF_10_RMSK)
#define HWIO_MSS_UIM0_UART_DM_RF_10_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM0_UART_DM_RF_10_ADDR, m)
#define HWIO_MSS_UIM0_UART_DM_RF_10_UART_RF_BMSK                                                                0xffffffff
#define HWIO_MSS_UIM0_UART_DM_RF_10_UART_RF_SHFT                                                                       0x0

#define HWIO_MSS_UIM0_UART_DM_RF_11_ADDR                                                                        (MSS_UIM0_UART_DM_REG_BASE      + 0x00000168)
#define HWIO_MSS_UIM0_UART_DM_RF_11_OFFS                                                                        (MSS_UIM0_UART_DM_REG_BASE_OFFS + 0x00000168)
#define HWIO_MSS_UIM0_UART_DM_RF_11_RMSK                                                                        0xffffffff
#define HWIO_MSS_UIM0_UART_DM_RF_11_IN          \
        in_dword_masked(HWIO_MSS_UIM0_UART_DM_RF_11_ADDR, HWIO_MSS_UIM0_UART_DM_RF_11_RMSK)
#define HWIO_MSS_UIM0_UART_DM_RF_11_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM0_UART_DM_RF_11_ADDR, m)
#define HWIO_MSS_UIM0_UART_DM_RF_11_UART_RF_BMSK                                                                0xffffffff
#define HWIO_MSS_UIM0_UART_DM_RF_11_UART_RF_SHFT                                                                       0x0

#define HWIO_MSS_UIM0_UART_DM_RF_12_ADDR                                                                        (MSS_UIM0_UART_DM_REG_BASE      + 0x0000016c)
#define HWIO_MSS_UIM0_UART_DM_RF_12_OFFS                                                                        (MSS_UIM0_UART_DM_REG_BASE_OFFS + 0x0000016c)
#define HWIO_MSS_UIM0_UART_DM_RF_12_RMSK                                                                        0xffffffff
#define HWIO_MSS_UIM0_UART_DM_RF_12_IN          \
        in_dword_masked(HWIO_MSS_UIM0_UART_DM_RF_12_ADDR, HWIO_MSS_UIM0_UART_DM_RF_12_RMSK)
#define HWIO_MSS_UIM0_UART_DM_RF_12_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM0_UART_DM_RF_12_ADDR, m)
#define HWIO_MSS_UIM0_UART_DM_RF_12_UART_RF_BMSK                                                                0xffffffff
#define HWIO_MSS_UIM0_UART_DM_RF_12_UART_RF_SHFT                                                                       0x0

#define HWIO_MSS_UIM0_UART_DM_RF_13_ADDR                                                                        (MSS_UIM0_UART_DM_REG_BASE      + 0x00000170)
#define HWIO_MSS_UIM0_UART_DM_RF_13_OFFS                                                                        (MSS_UIM0_UART_DM_REG_BASE_OFFS + 0x00000170)
#define HWIO_MSS_UIM0_UART_DM_RF_13_RMSK                                                                        0xffffffff
#define HWIO_MSS_UIM0_UART_DM_RF_13_IN          \
        in_dword_masked(HWIO_MSS_UIM0_UART_DM_RF_13_ADDR, HWIO_MSS_UIM0_UART_DM_RF_13_RMSK)
#define HWIO_MSS_UIM0_UART_DM_RF_13_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM0_UART_DM_RF_13_ADDR, m)
#define HWIO_MSS_UIM0_UART_DM_RF_13_UART_RF_BMSK                                                                0xffffffff
#define HWIO_MSS_UIM0_UART_DM_RF_13_UART_RF_SHFT                                                                       0x0

#define HWIO_MSS_UIM0_UART_DM_RF_14_ADDR                                                                        (MSS_UIM0_UART_DM_REG_BASE      + 0x00000174)
#define HWIO_MSS_UIM0_UART_DM_RF_14_OFFS                                                                        (MSS_UIM0_UART_DM_REG_BASE_OFFS + 0x00000174)
#define HWIO_MSS_UIM0_UART_DM_RF_14_RMSK                                                                        0xffffffff
#define HWIO_MSS_UIM0_UART_DM_RF_14_IN          \
        in_dword_masked(HWIO_MSS_UIM0_UART_DM_RF_14_ADDR, HWIO_MSS_UIM0_UART_DM_RF_14_RMSK)
#define HWIO_MSS_UIM0_UART_DM_RF_14_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM0_UART_DM_RF_14_ADDR, m)
#define HWIO_MSS_UIM0_UART_DM_RF_14_UART_RF_BMSK                                                                0xffffffff
#define HWIO_MSS_UIM0_UART_DM_RF_14_UART_RF_SHFT                                                                       0x0

#define HWIO_MSS_UIM0_UART_DM_RF_15_ADDR                                                                        (MSS_UIM0_UART_DM_REG_BASE      + 0x00000178)
#define HWIO_MSS_UIM0_UART_DM_RF_15_OFFS                                                                        (MSS_UIM0_UART_DM_REG_BASE_OFFS + 0x00000178)
#define HWIO_MSS_UIM0_UART_DM_RF_15_RMSK                                                                        0xffffffff
#define HWIO_MSS_UIM0_UART_DM_RF_15_IN          \
        in_dword_masked(HWIO_MSS_UIM0_UART_DM_RF_15_ADDR, HWIO_MSS_UIM0_UART_DM_RF_15_RMSK)
#define HWIO_MSS_UIM0_UART_DM_RF_15_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM0_UART_DM_RF_15_ADDR, m)
#define HWIO_MSS_UIM0_UART_DM_RF_15_UART_RF_BMSK                                                                0xffffffff
#define HWIO_MSS_UIM0_UART_DM_RF_15_UART_RF_SHFT                                                                       0x0

#define HWIO_MSS_UIM0_UART_DM_RF_16_ADDR                                                                        (MSS_UIM0_UART_DM_REG_BASE      + 0x0000017c)
#define HWIO_MSS_UIM0_UART_DM_RF_16_OFFS                                                                        (MSS_UIM0_UART_DM_REG_BASE_OFFS + 0x0000017c)
#define HWIO_MSS_UIM0_UART_DM_RF_16_RMSK                                                                        0xffffffff
#define HWIO_MSS_UIM0_UART_DM_RF_16_IN          \
        in_dword_masked(HWIO_MSS_UIM0_UART_DM_RF_16_ADDR, HWIO_MSS_UIM0_UART_DM_RF_16_RMSK)
#define HWIO_MSS_UIM0_UART_DM_RF_16_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM0_UART_DM_RF_16_ADDR, m)
#define HWIO_MSS_UIM0_UART_DM_RF_16_UART_RF_BMSK                                                                0xffffffff
#define HWIO_MSS_UIM0_UART_DM_RF_16_UART_RF_SHFT                                                                       0x0

#define HWIO_MSS_UIM0_UART_DM_UIM_CFG_ADDR                                                                      (MSS_UIM0_UART_DM_REG_BASE      + 0x00000180)
#define HWIO_MSS_UIM0_UART_DM_UIM_CFG_OFFS                                                                      (MSS_UIM0_UART_DM_REG_BASE_OFFS + 0x00000180)
#define HWIO_MSS_UIM0_UART_DM_UIM_CFG_RMSK                                                                          0xf77f
#define HWIO_MSS_UIM0_UART_DM_UIM_CFG_IN          \
        in_dword_masked(HWIO_MSS_UIM0_UART_DM_UIM_CFG_ADDR, HWIO_MSS_UIM0_UART_DM_UIM_CFG_RMSK)
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
#define HWIO_MSS_UIM0_UART_DM_UIM_CMD_OFFS                                                                      (MSS_UIM0_UART_DM_REG_BASE_OFFS + 0x00000184)
#define HWIO_MSS_UIM0_UART_DM_UIM_CMD_RMSK                                                                             0x3
#define HWIO_MSS_UIM0_UART_DM_UIM_CMD_OUT(v)      \
        out_dword(HWIO_MSS_UIM0_UART_DM_UIM_CMD_ADDR,v)
#define HWIO_MSS_UIM0_UART_DM_UIM_CMD_RECOVER_FROM_HW_DEACTIVATION_BMSK                                                0x2
#define HWIO_MSS_UIM0_UART_DM_UIM_CMD_RECOVER_FROM_HW_DEACTIVATION_SHFT                                                0x1
#define HWIO_MSS_UIM0_UART_DM_UIM_CMD_INITIATE_HW_DEACTIVATION_BMSK                                                    0x1
#define HWIO_MSS_UIM0_UART_DM_UIM_CMD_INITIATE_HW_DEACTIVATION_SHFT                                                    0x0

#define HWIO_MSS_UIM0_UART_DM_UIM_IO_STATUS_ADDR                                                                (MSS_UIM0_UART_DM_REG_BASE      + 0x00000188)
#define HWIO_MSS_UIM0_UART_DM_UIM_IO_STATUS_OFFS                                                                (MSS_UIM0_UART_DM_REG_BASE_OFFS + 0x00000188)
#define HWIO_MSS_UIM0_UART_DM_UIM_IO_STATUS_RMSK                                                                       0x7
#define HWIO_MSS_UIM0_UART_DM_UIM_IO_STATUS_IN          \
        in_dword_masked(HWIO_MSS_UIM0_UART_DM_UIM_IO_STATUS_ADDR, HWIO_MSS_UIM0_UART_DM_UIM_IO_STATUS_RMSK)
#define HWIO_MSS_UIM0_UART_DM_UIM_IO_STATUS_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM0_UART_DM_UIM_IO_STATUS_ADDR, m)
#define HWIO_MSS_UIM0_UART_DM_UIM_IO_STATUS_UIM_IO_WRITE_IN_PROGRESS_BMSK                                              0x4
#define HWIO_MSS_UIM0_UART_DM_UIM_IO_STATUS_UIM_IO_WRITE_IN_PROGRESS_SHFT                                              0x2
#define HWIO_MSS_UIM0_UART_DM_UIM_IO_STATUS_UIM_DEACTIVATION_STATUS_BMSK                                               0x2
#define HWIO_MSS_UIM0_UART_DM_UIM_IO_STATUS_UIM_DEACTIVATION_STATUS_SHFT                                               0x1
#define HWIO_MSS_UIM0_UART_DM_UIM_IO_STATUS_CARD_PRESENCE_BMSK                                                         0x1
#define HWIO_MSS_UIM0_UART_DM_UIM_IO_STATUS_CARD_PRESENCE_SHFT                                                         0x0

#define HWIO_MSS_UIM0_UART_DM_UIM_IRQ_ISR_ADDR                                                                  (MSS_UIM0_UART_DM_REG_BASE      + 0x0000018c)
#define HWIO_MSS_UIM0_UART_DM_UIM_IRQ_ISR_OFFS                                                                  (MSS_UIM0_UART_DM_REG_BASE_OFFS + 0x0000018c)
#define HWIO_MSS_UIM0_UART_DM_UIM_IRQ_ISR_RMSK                                                                        0x1f
#define HWIO_MSS_UIM0_UART_DM_UIM_IRQ_ISR_IN          \
        in_dword_masked(HWIO_MSS_UIM0_UART_DM_UIM_IRQ_ISR_ADDR, HWIO_MSS_UIM0_UART_DM_UIM_IRQ_ISR_RMSK)
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
#define HWIO_MSS_UIM0_UART_DM_UIM_IRQ_MISR_OFFS                                                                 (MSS_UIM0_UART_DM_REG_BASE_OFFS + 0x00000190)
#define HWIO_MSS_UIM0_UART_DM_UIM_IRQ_MISR_RMSK                                                                       0x1f
#define HWIO_MSS_UIM0_UART_DM_UIM_IRQ_MISR_IN          \
        in_dword_masked(HWIO_MSS_UIM0_UART_DM_UIM_IRQ_MISR_ADDR, HWIO_MSS_UIM0_UART_DM_UIM_IRQ_MISR_RMSK)
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
#define HWIO_MSS_UIM0_UART_DM_UIM_IRQ_CLR_OFFS                                                                  (MSS_UIM0_UART_DM_REG_BASE_OFFS + 0x00000194)
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
#define HWIO_MSS_UIM0_UART_DM_UIM_IRQ_IMR_OFFS                                                                  (MSS_UIM0_UART_DM_REG_BASE_OFFS + 0x00000198)
#define HWIO_MSS_UIM0_UART_DM_UIM_IRQ_IMR_RMSK                                                                        0x1f
#define HWIO_MSS_UIM0_UART_DM_UIM_IRQ_IMR_IN          \
        in_dword_masked(HWIO_MSS_UIM0_UART_DM_UIM_IRQ_IMR_ADDR, HWIO_MSS_UIM0_UART_DM_UIM_IRQ_IMR_RMSK)
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
#define HWIO_MSS_UIM0_UART_DM_UIM_IRQ_IMR_SET_OFFS                                                              (MSS_UIM0_UART_DM_REG_BASE_OFFS + 0x0000019c)
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
#define HWIO_MSS_UIM0_UART_DM_UIM_IRQ_IMR_CLR_OFFS                                                              (MSS_UIM0_UART_DM_REG_BASE_OFFS + 0x000001a0)
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
#define MSS_UIM1_UART_DM_REG_BASE_OFFS                                                                          0x00198000

#define HWIO_MSS_UIM1_UART_DM_MR1_ADDR                                                                          (MSS_UIM1_UART_DM_REG_BASE      + 0x00000000)
#define HWIO_MSS_UIM1_UART_DM_MR1_OFFS                                                                          (MSS_UIM1_UART_DM_REG_BASE_OFFS + 0x00000000)
#define HWIO_MSS_UIM1_UART_DM_MR1_RMSK                                                                          0xffffffff
#define HWIO_MSS_UIM1_UART_DM_MR1_IN          \
        in_dword_masked(HWIO_MSS_UIM1_UART_DM_MR1_ADDR, HWIO_MSS_UIM1_UART_DM_MR1_RMSK)
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
#define HWIO_MSS_UIM1_UART_DM_MR2_OFFS                                                                          (MSS_UIM1_UART_DM_REG_BASE_OFFS + 0x00000004)
#define HWIO_MSS_UIM1_UART_DM_MR2_RMSK                                                                               0x7ff
#define HWIO_MSS_UIM1_UART_DM_MR2_IN          \
        in_dword_masked(HWIO_MSS_UIM1_UART_DM_MR2_ADDR, HWIO_MSS_UIM1_UART_DM_MR2_RMSK)
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
#define HWIO_MSS_UIM1_UART_DM_CSR_SR_DEPRECATED_OFFS                                                            (MSS_UIM1_UART_DM_REG_BASE_OFFS + 0x00000008)
#define HWIO_MSS_UIM1_UART_DM_CSR_SR_DEPRECATED_RMSK                                                            0xffffffff
#define HWIO_MSS_UIM1_UART_DM_CSR_SR_DEPRECATED_IN          \
        in_dword_masked(HWIO_MSS_UIM1_UART_DM_CSR_SR_DEPRECATED_ADDR, HWIO_MSS_UIM1_UART_DM_CSR_SR_DEPRECATED_RMSK)
#define HWIO_MSS_UIM1_UART_DM_CSR_SR_DEPRECATED_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM1_UART_DM_CSR_SR_DEPRECATED_ADDR, m)
#define HWIO_MSS_UIM1_UART_DM_CSR_SR_DEPRECATED_OUT(v)      \
        out_dword(HWIO_MSS_UIM1_UART_DM_CSR_SR_DEPRECATED_ADDR,v)
#define HWIO_MSS_UIM1_UART_DM_CSR_SR_DEPRECATED_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_UIM1_UART_DM_CSR_SR_DEPRECATED_ADDR,m,v,HWIO_MSS_UIM1_UART_DM_CSR_SR_DEPRECATED_IN)
#define HWIO_MSS_UIM1_UART_DM_CSR_SR_DEPRECATED_UART_DM_CSR_SR_DEPRECATED_BMSK                                  0xffffffff
#define HWIO_MSS_UIM1_UART_DM_CSR_SR_DEPRECATED_UART_DM_CSR_SR_DEPRECATED_SHFT                                         0x0

#define HWIO_MSS_UIM1_UART_DM_CR_MISR_DEPRECATED_ADDR                                                           (MSS_UIM1_UART_DM_REG_BASE      + 0x00000010)
#define HWIO_MSS_UIM1_UART_DM_CR_MISR_DEPRECATED_OFFS                                                           (MSS_UIM1_UART_DM_REG_BASE_OFFS + 0x00000010)
#define HWIO_MSS_UIM1_UART_DM_CR_MISR_DEPRECATED_RMSK                                                           0xffffffff
#define HWIO_MSS_UIM1_UART_DM_CR_MISR_DEPRECATED_IN          \
        in_dword_masked(HWIO_MSS_UIM1_UART_DM_CR_MISR_DEPRECATED_ADDR, HWIO_MSS_UIM1_UART_DM_CR_MISR_DEPRECATED_RMSK)
#define HWIO_MSS_UIM1_UART_DM_CR_MISR_DEPRECATED_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM1_UART_DM_CR_MISR_DEPRECATED_ADDR, m)
#define HWIO_MSS_UIM1_UART_DM_CR_MISR_DEPRECATED_OUT(v)      \
        out_dword(HWIO_MSS_UIM1_UART_DM_CR_MISR_DEPRECATED_ADDR,v)
#define HWIO_MSS_UIM1_UART_DM_CR_MISR_DEPRECATED_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_UIM1_UART_DM_CR_MISR_DEPRECATED_ADDR,m,v,HWIO_MSS_UIM1_UART_DM_CR_MISR_DEPRECATED_IN)
#define HWIO_MSS_UIM1_UART_DM_CR_MISR_DEPRECATED_UART_DM_CR_MISR_DEPRECATED_BMSK                                0xffffffff
#define HWIO_MSS_UIM1_UART_DM_CR_MISR_DEPRECATED_UART_DM_CR_MISR_DEPRECATED_SHFT                                       0x0

#define HWIO_MSS_UIM1_UART_DM_IMR_ISR_DEPRECATED_ADDR                                                           (MSS_UIM1_UART_DM_REG_BASE      + 0x00000014)
#define HWIO_MSS_UIM1_UART_DM_IMR_ISR_DEPRECATED_OFFS                                                           (MSS_UIM1_UART_DM_REG_BASE_OFFS + 0x00000014)
#define HWIO_MSS_UIM1_UART_DM_IMR_ISR_DEPRECATED_RMSK                                                           0xffffffff
#define HWIO_MSS_UIM1_UART_DM_IMR_ISR_DEPRECATED_IN          \
        in_dword_masked(HWIO_MSS_UIM1_UART_DM_IMR_ISR_DEPRECATED_ADDR, HWIO_MSS_UIM1_UART_DM_IMR_ISR_DEPRECATED_RMSK)
#define HWIO_MSS_UIM1_UART_DM_IMR_ISR_DEPRECATED_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM1_UART_DM_IMR_ISR_DEPRECATED_ADDR, m)
#define HWIO_MSS_UIM1_UART_DM_IMR_ISR_DEPRECATED_OUT(v)      \
        out_dword(HWIO_MSS_UIM1_UART_DM_IMR_ISR_DEPRECATED_ADDR,v)
#define HWIO_MSS_UIM1_UART_DM_IMR_ISR_DEPRECATED_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_UIM1_UART_DM_IMR_ISR_DEPRECATED_ADDR,m,v,HWIO_MSS_UIM1_UART_DM_IMR_ISR_DEPRECATED_IN)
#define HWIO_MSS_UIM1_UART_DM_IMR_ISR_DEPRECATED_UART_DM_IMR_ISR_DEPRECATED_BMSK                                0xffffffff
#define HWIO_MSS_UIM1_UART_DM_IMR_ISR_DEPRECATED_UART_DM_IMR_ISR_DEPRECATED_SHFT                                       0x0

#define HWIO_MSS_UIM1_UART_DM_IPR_ADDR                                                                          (MSS_UIM1_UART_DM_REG_BASE      + 0x00000018)
#define HWIO_MSS_UIM1_UART_DM_IPR_OFFS                                                                          (MSS_UIM1_UART_DM_REG_BASE_OFFS + 0x00000018)
#define HWIO_MSS_UIM1_UART_DM_IPR_RMSK                                                                          0xffffffdf
#define HWIO_MSS_UIM1_UART_DM_IPR_IN          \
        in_dword_masked(HWIO_MSS_UIM1_UART_DM_IPR_ADDR, HWIO_MSS_UIM1_UART_DM_IPR_RMSK)
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
#define HWIO_MSS_UIM1_UART_DM_TFWR_OFFS                                                                         (MSS_UIM1_UART_DM_REG_BASE_OFFS + 0x0000001c)
#define HWIO_MSS_UIM1_UART_DM_TFWR_RMSK                                                                         0xffffffff
#define HWIO_MSS_UIM1_UART_DM_TFWR_IN          \
        in_dword_masked(HWIO_MSS_UIM1_UART_DM_TFWR_ADDR, HWIO_MSS_UIM1_UART_DM_TFWR_RMSK)
#define HWIO_MSS_UIM1_UART_DM_TFWR_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM1_UART_DM_TFWR_ADDR, m)
#define HWIO_MSS_UIM1_UART_DM_TFWR_OUT(v)      \
        out_dword(HWIO_MSS_UIM1_UART_DM_TFWR_ADDR,v)
#define HWIO_MSS_UIM1_UART_DM_TFWR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_UIM1_UART_DM_TFWR_ADDR,m,v,HWIO_MSS_UIM1_UART_DM_TFWR_IN)
#define HWIO_MSS_UIM1_UART_DM_TFWR_TFW_BMSK                                                                     0xffffffff
#define HWIO_MSS_UIM1_UART_DM_TFWR_TFW_SHFT                                                                            0x0

#define HWIO_MSS_UIM1_UART_DM_RFWR_ADDR                                                                         (MSS_UIM1_UART_DM_REG_BASE      + 0x00000020)
#define HWIO_MSS_UIM1_UART_DM_RFWR_OFFS                                                                         (MSS_UIM1_UART_DM_REG_BASE_OFFS + 0x00000020)
#define HWIO_MSS_UIM1_UART_DM_RFWR_RMSK                                                                         0xffffffff
#define HWIO_MSS_UIM1_UART_DM_RFWR_IN          \
        in_dword_masked(HWIO_MSS_UIM1_UART_DM_RFWR_ADDR, HWIO_MSS_UIM1_UART_DM_RFWR_RMSK)
#define HWIO_MSS_UIM1_UART_DM_RFWR_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM1_UART_DM_RFWR_ADDR, m)
#define HWIO_MSS_UIM1_UART_DM_RFWR_OUT(v)      \
        out_dword(HWIO_MSS_UIM1_UART_DM_RFWR_ADDR,v)
#define HWIO_MSS_UIM1_UART_DM_RFWR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_UIM1_UART_DM_RFWR_ADDR,m,v,HWIO_MSS_UIM1_UART_DM_RFWR_IN)
#define HWIO_MSS_UIM1_UART_DM_RFWR_RFW_BMSK                                                                     0xffffffff
#define HWIO_MSS_UIM1_UART_DM_RFWR_RFW_SHFT                                                                            0x0

#define HWIO_MSS_UIM1_UART_DM_HCR_ADDR                                                                          (MSS_UIM1_UART_DM_REG_BASE      + 0x00000024)
#define HWIO_MSS_UIM1_UART_DM_HCR_OFFS                                                                          (MSS_UIM1_UART_DM_REG_BASE_OFFS + 0x00000024)
#define HWIO_MSS_UIM1_UART_DM_HCR_RMSK                                                                                0xff
#define HWIO_MSS_UIM1_UART_DM_HCR_IN          \
        in_dword_masked(HWIO_MSS_UIM1_UART_DM_HCR_ADDR, HWIO_MSS_UIM1_UART_DM_HCR_RMSK)
#define HWIO_MSS_UIM1_UART_DM_HCR_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM1_UART_DM_HCR_ADDR, m)
#define HWIO_MSS_UIM1_UART_DM_HCR_OUT(v)      \
        out_dword(HWIO_MSS_UIM1_UART_DM_HCR_ADDR,v)
#define HWIO_MSS_UIM1_UART_DM_HCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_UIM1_UART_DM_HCR_ADDR,m,v,HWIO_MSS_UIM1_UART_DM_HCR_IN)
#define HWIO_MSS_UIM1_UART_DM_HCR_DATA_BMSK                                                                           0xff
#define HWIO_MSS_UIM1_UART_DM_HCR_DATA_SHFT                                                                            0x0

#define HWIO_MSS_UIM1_UART_DM_DMRX_ADDR                                                                         (MSS_UIM1_UART_DM_REG_BASE      + 0x00000034)
#define HWIO_MSS_UIM1_UART_DM_DMRX_OFFS                                                                         (MSS_UIM1_UART_DM_REG_BASE_OFFS + 0x00000034)
#define HWIO_MSS_UIM1_UART_DM_DMRX_RMSK                                                                          0x1ffffff
#define HWIO_MSS_UIM1_UART_DM_DMRX_IN          \
        in_dword_masked(HWIO_MSS_UIM1_UART_DM_DMRX_ADDR, HWIO_MSS_UIM1_UART_DM_DMRX_RMSK)
#define HWIO_MSS_UIM1_UART_DM_DMRX_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM1_UART_DM_DMRX_ADDR, m)
#define HWIO_MSS_UIM1_UART_DM_DMRX_OUT(v)      \
        out_dword(HWIO_MSS_UIM1_UART_DM_DMRX_ADDR,v)
#define HWIO_MSS_UIM1_UART_DM_DMRX_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_UIM1_UART_DM_DMRX_ADDR,m,v,HWIO_MSS_UIM1_UART_DM_DMRX_IN)
#define HWIO_MSS_UIM1_UART_DM_DMRX_RX_DM_CRCI_CHARS_BMSK                                                         0x1ffffff
#define HWIO_MSS_UIM1_UART_DM_DMRX_RX_DM_CRCI_CHARS_SHFT                                                               0x0

#define HWIO_MSS_UIM1_UART_DM_IRDA_RX_TOTAL_SNAP_DEPRECATED_ADDR                                                (MSS_UIM1_UART_DM_REG_BASE      + 0x00000038)
#define HWIO_MSS_UIM1_UART_DM_IRDA_RX_TOTAL_SNAP_DEPRECATED_OFFS                                                (MSS_UIM1_UART_DM_REG_BASE_OFFS + 0x00000038)
#define HWIO_MSS_UIM1_UART_DM_IRDA_RX_TOTAL_SNAP_DEPRECATED_RMSK                                                0xffffffff
#define HWIO_MSS_UIM1_UART_DM_IRDA_RX_TOTAL_SNAP_DEPRECATED_IN          \
        in_dword_masked(HWIO_MSS_UIM1_UART_DM_IRDA_RX_TOTAL_SNAP_DEPRECATED_ADDR, HWIO_MSS_UIM1_UART_DM_IRDA_RX_TOTAL_SNAP_DEPRECATED_RMSK)
#define HWIO_MSS_UIM1_UART_DM_IRDA_RX_TOTAL_SNAP_DEPRECATED_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM1_UART_DM_IRDA_RX_TOTAL_SNAP_DEPRECATED_ADDR, m)
#define HWIO_MSS_UIM1_UART_DM_IRDA_RX_TOTAL_SNAP_DEPRECATED_OUT(v)      \
        out_dword(HWIO_MSS_UIM1_UART_DM_IRDA_RX_TOTAL_SNAP_DEPRECATED_ADDR,v)
#define HWIO_MSS_UIM1_UART_DM_IRDA_RX_TOTAL_SNAP_DEPRECATED_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_UIM1_UART_DM_IRDA_RX_TOTAL_SNAP_DEPRECATED_ADDR,m,v,HWIO_MSS_UIM1_UART_DM_IRDA_RX_TOTAL_SNAP_DEPRECATED_IN)
#define HWIO_MSS_UIM1_UART_DM_IRDA_RX_TOTAL_SNAP_DEPRECATED_UART_DM_IRDA_RX_TOTAL_SNAP_DEPRECATED_BMSK          0xffffffff
#define HWIO_MSS_UIM1_UART_DM_IRDA_RX_TOTAL_SNAP_DEPRECATED_UART_DM_IRDA_RX_TOTAL_SNAP_DEPRECATED_SHFT                 0x0

#define HWIO_MSS_UIM1_UART_DM_DMEN_ADDR                                                                         (MSS_UIM1_UART_DM_REG_BASE      + 0x0000003c)
#define HWIO_MSS_UIM1_UART_DM_DMEN_OFFS                                                                         (MSS_UIM1_UART_DM_REG_BASE_OFFS + 0x0000003c)
#define HWIO_MSS_UIM1_UART_DM_DMEN_RMSK                                                                               0x3c
#define HWIO_MSS_UIM1_UART_DM_DMEN_IN          \
        in_dword_masked(HWIO_MSS_UIM1_UART_DM_DMEN_ADDR, HWIO_MSS_UIM1_UART_DM_DMEN_RMSK)
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
#define HWIO_MSS_UIM1_UART_DM_NO_CHARS_FOR_TX_OFFS                                                              (MSS_UIM1_UART_DM_REG_BASE_OFFS + 0x00000040)
#define HWIO_MSS_UIM1_UART_DM_NO_CHARS_FOR_TX_RMSK                                                                0xffffff
#define HWIO_MSS_UIM1_UART_DM_NO_CHARS_FOR_TX_IN          \
        in_dword_masked(HWIO_MSS_UIM1_UART_DM_NO_CHARS_FOR_TX_ADDR, HWIO_MSS_UIM1_UART_DM_NO_CHARS_FOR_TX_RMSK)
#define HWIO_MSS_UIM1_UART_DM_NO_CHARS_FOR_TX_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM1_UART_DM_NO_CHARS_FOR_TX_ADDR, m)
#define HWIO_MSS_UIM1_UART_DM_NO_CHARS_FOR_TX_OUT(v)      \
        out_dword(HWIO_MSS_UIM1_UART_DM_NO_CHARS_FOR_TX_ADDR,v)
#define HWIO_MSS_UIM1_UART_DM_NO_CHARS_FOR_TX_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_UIM1_UART_DM_NO_CHARS_FOR_TX_ADDR,m,v,HWIO_MSS_UIM1_UART_DM_NO_CHARS_FOR_TX_IN)
#define HWIO_MSS_UIM1_UART_DM_NO_CHARS_FOR_TX_TX_TOTAL_TRANS_LEN_BMSK                                             0xffffff
#define HWIO_MSS_UIM1_UART_DM_NO_CHARS_FOR_TX_TX_TOTAL_TRANS_LEN_SHFT                                                  0x0

#define HWIO_MSS_UIM1_UART_DM_BADR_ADDR                                                                         (MSS_UIM1_UART_DM_REG_BASE      + 0x00000044)
#define HWIO_MSS_UIM1_UART_DM_BADR_OFFS                                                                         (MSS_UIM1_UART_DM_REG_BASE_OFFS + 0x00000044)
#define HWIO_MSS_UIM1_UART_DM_BADR_RMSK                                                                         0xfffffffc
#define HWIO_MSS_UIM1_UART_DM_BADR_IN          \
        in_dword_masked(HWIO_MSS_UIM1_UART_DM_BADR_ADDR, HWIO_MSS_UIM1_UART_DM_BADR_RMSK)
#define HWIO_MSS_UIM1_UART_DM_BADR_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM1_UART_DM_BADR_ADDR, m)
#define HWIO_MSS_UIM1_UART_DM_BADR_OUT(v)      \
        out_dword(HWIO_MSS_UIM1_UART_DM_BADR_ADDR,v)
#define HWIO_MSS_UIM1_UART_DM_BADR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_UIM1_UART_DM_BADR_ADDR,m,v,HWIO_MSS_UIM1_UART_DM_BADR_IN)
#define HWIO_MSS_UIM1_UART_DM_BADR_RX_BASE_ADDR_BMSK                                                            0xfffffffc
#define HWIO_MSS_UIM1_UART_DM_BADR_RX_BASE_ADDR_SHFT                                                                   0x2

#define HWIO_MSS_UIM1_UART_DM_TESTSL_ADDR                                                                       (MSS_UIM1_UART_DM_REG_BASE      + 0x00000048)
#define HWIO_MSS_UIM1_UART_DM_TESTSL_OFFS                                                                       (MSS_UIM1_UART_DM_REG_BASE_OFFS + 0x00000048)
#define HWIO_MSS_UIM1_UART_DM_TESTSL_RMSK                                                                             0x1f
#define HWIO_MSS_UIM1_UART_DM_TESTSL_IN          \
        in_dword_masked(HWIO_MSS_UIM1_UART_DM_TESTSL_ADDR, HWIO_MSS_UIM1_UART_DM_TESTSL_RMSK)
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
#define HWIO_MSS_UIM1_UART_DM_TXFS_OFFS                                                                         (MSS_UIM1_UART_DM_REG_BASE_OFFS + 0x0000004c)
#define HWIO_MSS_UIM1_UART_DM_TXFS_RMSK                                                                         0xffffffff
#define HWIO_MSS_UIM1_UART_DM_TXFS_IN          \
        in_dword_masked(HWIO_MSS_UIM1_UART_DM_TXFS_ADDR, HWIO_MSS_UIM1_UART_DM_TXFS_RMSK)
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
#define HWIO_MSS_UIM1_UART_DM_RXFS_OFFS                                                                         (MSS_UIM1_UART_DM_REG_BASE_OFFS + 0x00000050)
#define HWIO_MSS_UIM1_UART_DM_RXFS_RMSK                                                                         0xffffffff
#define HWIO_MSS_UIM1_UART_DM_RXFS_IN          \
        in_dword_masked(HWIO_MSS_UIM1_UART_DM_RXFS_ADDR, HWIO_MSS_UIM1_UART_DM_RXFS_RMSK)
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
#define HWIO_MSS_UIM1_UART_DM_MISR_MODE_OFFS                                                                    (MSS_UIM1_UART_DM_REG_BASE_OFFS + 0x00000060)
#define HWIO_MSS_UIM1_UART_DM_MISR_MODE_RMSK                                                                           0x3
#define HWIO_MSS_UIM1_UART_DM_MISR_MODE_IN          \
        in_dword_masked(HWIO_MSS_UIM1_UART_DM_MISR_MODE_ADDR, HWIO_MSS_UIM1_UART_DM_MISR_MODE_RMSK)
#define HWIO_MSS_UIM1_UART_DM_MISR_MODE_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM1_UART_DM_MISR_MODE_ADDR, m)
#define HWIO_MSS_UIM1_UART_DM_MISR_MODE_OUT(v)      \
        out_dword(HWIO_MSS_UIM1_UART_DM_MISR_MODE_ADDR,v)
#define HWIO_MSS_UIM1_UART_DM_MISR_MODE_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_UIM1_UART_DM_MISR_MODE_ADDR,m,v,HWIO_MSS_UIM1_UART_DM_MISR_MODE_IN)
#define HWIO_MSS_UIM1_UART_DM_MISR_MODE_MODE_BMSK                                                                      0x3
#define HWIO_MSS_UIM1_UART_DM_MISR_MODE_MODE_SHFT                                                                      0x0

#define HWIO_MSS_UIM1_UART_DM_MISR_RESET_ADDR                                                                   (MSS_UIM1_UART_DM_REG_BASE      + 0x00000064)
#define HWIO_MSS_UIM1_UART_DM_MISR_RESET_OFFS                                                                   (MSS_UIM1_UART_DM_REG_BASE_OFFS + 0x00000064)
#define HWIO_MSS_UIM1_UART_DM_MISR_RESET_RMSK                                                                          0x1
#define HWIO_MSS_UIM1_UART_DM_MISR_RESET_OUT(v)      \
        out_dword(HWIO_MSS_UIM1_UART_DM_MISR_RESET_ADDR,v)
#define HWIO_MSS_UIM1_UART_DM_MISR_RESET_RESET_BMSK                                                                    0x1
#define HWIO_MSS_UIM1_UART_DM_MISR_RESET_RESET_SHFT                                                                    0x0

#define HWIO_MSS_UIM1_UART_DM_MISR_EXPORT_ADDR                                                                  (MSS_UIM1_UART_DM_REG_BASE      + 0x00000068)
#define HWIO_MSS_UIM1_UART_DM_MISR_EXPORT_OFFS                                                                  (MSS_UIM1_UART_DM_REG_BASE_OFFS + 0x00000068)
#define HWIO_MSS_UIM1_UART_DM_MISR_EXPORT_RMSK                                                                         0x1
#define HWIO_MSS_UIM1_UART_DM_MISR_EXPORT_IN          \
        in_dword_masked(HWIO_MSS_UIM1_UART_DM_MISR_EXPORT_ADDR, HWIO_MSS_UIM1_UART_DM_MISR_EXPORT_RMSK)
#define HWIO_MSS_UIM1_UART_DM_MISR_EXPORT_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM1_UART_DM_MISR_EXPORT_ADDR, m)
#define HWIO_MSS_UIM1_UART_DM_MISR_EXPORT_OUT(v)      \
        out_dword(HWIO_MSS_UIM1_UART_DM_MISR_EXPORT_ADDR,v)
#define HWIO_MSS_UIM1_UART_DM_MISR_EXPORT_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_UIM1_UART_DM_MISR_EXPORT_ADDR,m,v,HWIO_MSS_UIM1_UART_DM_MISR_EXPORT_IN)
#define HWIO_MSS_UIM1_UART_DM_MISR_EXPORT_EXPORT_BMSK                                                                  0x1
#define HWIO_MSS_UIM1_UART_DM_MISR_EXPORT_EXPORT_SHFT                                                                  0x0

#define HWIO_MSS_UIM1_UART_DM_MISR_VAL_ADDR                                                                     (MSS_UIM1_UART_DM_REG_BASE      + 0x0000006c)
#define HWIO_MSS_UIM1_UART_DM_MISR_VAL_OFFS                                                                     (MSS_UIM1_UART_DM_REG_BASE_OFFS + 0x0000006c)
#define HWIO_MSS_UIM1_UART_DM_MISR_VAL_RMSK                                                                          0x3ff
#define HWIO_MSS_UIM1_UART_DM_MISR_VAL_IN          \
        in_dword_masked(HWIO_MSS_UIM1_UART_DM_MISR_VAL_ADDR, HWIO_MSS_UIM1_UART_DM_MISR_VAL_RMSK)
#define HWIO_MSS_UIM1_UART_DM_MISR_VAL_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM1_UART_DM_MISR_VAL_ADDR, m)
#define HWIO_MSS_UIM1_UART_DM_MISR_VAL_VAL_BMSK                                                                      0x3ff
#define HWIO_MSS_UIM1_UART_DM_MISR_VAL_VAL_SHFT                                                                        0x0

#define HWIO_MSS_UIM1_UART_DM_TF_RF_DEPRECATED_ADDR                                                             (MSS_UIM1_UART_DM_REG_BASE      + 0x00000070)
#define HWIO_MSS_UIM1_UART_DM_TF_RF_DEPRECATED_OFFS                                                             (MSS_UIM1_UART_DM_REG_BASE_OFFS + 0x00000070)
#define HWIO_MSS_UIM1_UART_DM_TF_RF_DEPRECATED_RMSK                                                             0xffffffff
#define HWIO_MSS_UIM1_UART_DM_TF_RF_DEPRECATED_IN          \
        in_dword_masked(HWIO_MSS_UIM1_UART_DM_TF_RF_DEPRECATED_ADDR, HWIO_MSS_UIM1_UART_DM_TF_RF_DEPRECATED_RMSK)
#define HWIO_MSS_UIM1_UART_DM_TF_RF_DEPRECATED_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM1_UART_DM_TF_RF_DEPRECATED_ADDR, m)
#define HWIO_MSS_UIM1_UART_DM_TF_RF_DEPRECATED_OUT(v)      \
        out_dword(HWIO_MSS_UIM1_UART_DM_TF_RF_DEPRECATED_ADDR,v)
#define HWIO_MSS_UIM1_UART_DM_TF_RF_DEPRECATED_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_UIM1_UART_DM_TF_RF_DEPRECATED_ADDR,m,v,HWIO_MSS_UIM1_UART_DM_TF_RF_DEPRECATED_IN)
#define HWIO_MSS_UIM1_UART_DM_TF_RF_DEPRECATED_UART_DM_TF_RF_DEPRECATED_BMSK                                    0xffffffff
#define HWIO_MSS_UIM1_UART_DM_TF_RF_DEPRECATED_UART_DM_TF_RF_DEPRECATED_SHFT                                           0x0

#define HWIO_MSS_UIM1_UART_DM_TF_RF_2_DEPRECATED_ADDR                                                           (MSS_UIM1_UART_DM_REG_BASE      + 0x00000074)
#define HWIO_MSS_UIM1_UART_DM_TF_RF_2_DEPRECATED_OFFS                                                           (MSS_UIM1_UART_DM_REG_BASE_OFFS + 0x00000074)
#define HWIO_MSS_UIM1_UART_DM_TF_RF_2_DEPRECATED_RMSK                                                           0xffffffff
#define HWIO_MSS_UIM1_UART_DM_TF_RF_2_DEPRECATED_IN          \
        in_dword_masked(HWIO_MSS_UIM1_UART_DM_TF_RF_2_DEPRECATED_ADDR, HWIO_MSS_UIM1_UART_DM_TF_RF_2_DEPRECATED_RMSK)
#define HWIO_MSS_UIM1_UART_DM_TF_RF_2_DEPRECATED_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM1_UART_DM_TF_RF_2_DEPRECATED_ADDR, m)
#define HWIO_MSS_UIM1_UART_DM_TF_RF_2_DEPRECATED_OUT(v)      \
        out_dword(HWIO_MSS_UIM1_UART_DM_TF_RF_2_DEPRECATED_ADDR,v)
#define HWIO_MSS_UIM1_UART_DM_TF_RF_2_DEPRECATED_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_UIM1_UART_DM_TF_RF_2_DEPRECATED_ADDR,m,v,HWIO_MSS_UIM1_UART_DM_TF_RF_2_DEPRECATED_IN)
#define HWIO_MSS_UIM1_UART_DM_TF_RF_2_DEPRECATED_UART_DM_TF_RF_2_DEPRECATED_BMSK                                0xffffffff
#define HWIO_MSS_UIM1_UART_DM_TF_RF_2_DEPRECATED_UART_DM_TF_RF_2_DEPRECATED_SHFT                                       0x0

#define HWIO_MSS_UIM1_UART_DM_TF_RF_3_DEPRECATED_ADDR                                                           (MSS_UIM1_UART_DM_REG_BASE      + 0x00000078)
#define HWIO_MSS_UIM1_UART_DM_TF_RF_3_DEPRECATED_OFFS                                                           (MSS_UIM1_UART_DM_REG_BASE_OFFS + 0x00000078)
#define HWIO_MSS_UIM1_UART_DM_TF_RF_3_DEPRECATED_RMSK                                                           0xffffffff
#define HWIO_MSS_UIM1_UART_DM_TF_RF_3_DEPRECATED_IN          \
        in_dword_masked(HWIO_MSS_UIM1_UART_DM_TF_RF_3_DEPRECATED_ADDR, HWIO_MSS_UIM1_UART_DM_TF_RF_3_DEPRECATED_RMSK)
#define HWIO_MSS_UIM1_UART_DM_TF_RF_3_DEPRECATED_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM1_UART_DM_TF_RF_3_DEPRECATED_ADDR, m)
#define HWIO_MSS_UIM1_UART_DM_TF_RF_3_DEPRECATED_OUT(v)      \
        out_dword(HWIO_MSS_UIM1_UART_DM_TF_RF_3_DEPRECATED_ADDR,v)
#define HWIO_MSS_UIM1_UART_DM_TF_RF_3_DEPRECATED_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_UIM1_UART_DM_TF_RF_3_DEPRECATED_ADDR,m,v,HWIO_MSS_UIM1_UART_DM_TF_RF_3_DEPRECATED_IN)
#define HWIO_MSS_UIM1_UART_DM_TF_RF_3_DEPRECATED_UART_DM_TF_RF_3_DEPRECATED_BMSK                                0xffffffff
#define HWIO_MSS_UIM1_UART_DM_TF_RF_3_DEPRECATED_UART_DM_TF_RF_3_DEPRECATED_SHFT                                       0x0

#define HWIO_MSS_UIM1_UART_DM_TF_RF_4_DEPRECATED_ADDR                                                           (MSS_UIM1_UART_DM_REG_BASE      + 0x0000007c)
#define HWIO_MSS_UIM1_UART_DM_TF_RF_4_DEPRECATED_OFFS                                                           (MSS_UIM1_UART_DM_REG_BASE_OFFS + 0x0000007c)
#define HWIO_MSS_UIM1_UART_DM_TF_RF_4_DEPRECATED_RMSK                                                           0xffffffff
#define HWIO_MSS_UIM1_UART_DM_TF_RF_4_DEPRECATED_IN          \
        in_dword_masked(HWIO_MSS_UIM1_UART_DM_TF_RF_4_DEPRECATED_ADDR, HWIO_MSS_UIM1_UART_DM_TF_RF_4_DEPRECATED_RMSK)
#define HWIO_MSS_UIM1_UART_DM_TF_RF_4_DEPRECATED_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM1_UART_DM_TF_RF_4_DEPRECATED_ADDR, m)
#define HWIO_MSS_UIM1_UART_DM_TF_RF_4_DEPRECATED_OUT(v)      \
        out_dword(HWIO_MSS_UIM1_UART_DM_TF_RF_4_DEPRECATED_ADDR,v)
#define HWIO_MSS_UIM1_UART_DM_TF_RF_4_DEPRECATED_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_UIM1_UART_DM_TF_RF_4_DEPRECATED_ADDR,m,v,HWIO_MSS_UIM1_UART_DM_TF_RF_4_DEPRECATED_IN)
#define HWIO_MSS_UIM1_UART_DM_TF_RF_4_DEPRECATED_UART_DM_TF_RF_4_DEPRECATED_BMSK                                0xffffffff
#define HWIO_MSS_UIM1_UART_DM_TF_RF_4_DEPRECATED_UART_DM_TF_RF_4_DEPRECATED_SHFT                                       0x0

#define HWIO_MSS_UIM1_UART_DM_SIM_CFG_ADDR                                                                      (MSS_UIM1_UART_DM_REG_BASE      + 0x00000080)
#define HWIO_MSS_UIM1_UART_DM_SIM_CFG_OFFS                                                                      (MSS_UIM1_UART_DM_REG_BASE_OFFS + 0x00000080)
#define HWIO_MSS_UIM1_UART_DM_SIM_CFG_RMSK                                                                         0x3ffef
#define HWIO_MSS_UIM1_UART_DM_SIM_CFG_IN          \
        in_dword_masked(HWIO_MSS_UIM1_UART_DM_SIM_CFG_ADDR, HWIO_MSS_UIM1_UART_DM_SIM_CFG_RMSK)
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
#define HWIO_MSS_UIM1_UART_DM_TEST_WR_ADDR_OFFS                                                                 (MSS_UIM1_UART_DM_REG_BASE_OFFS + 0x00000084)
#define HWIO_MSS_UIM1_UART_DM_TEST_WR_ADDR_RMSK                                                                 0xffffffff
#define HWIO_MSS_UIM1_UART_DM_TEST_WR_ADDR_IN          \
        in_dword_masked(HWIO_MSS_UIM1_UART_DM_TEST_WR_ADDR_ADDR, HWIO_MSS_UIM1_UART_DM_TEST_WR_ADDR_RMSK)
#define HWIO_MSS_UIM1_UART_DM_TEST_WR_ADDR_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM1_UART_DM_TEST_WR_ADDR_ADDR, m)
#define HWIO_MSS_UIM1_UART_DM_TEST_WR_ADDR_OUT(v)      \
        out_dword(HWIO_MSS_UIM1_UART_DM_TEST_WR_ADDR_ADDR,v)
#define HWIO_MSS_UIM1_UART_DM_TEST_WR_ADDR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_UIM1_UART_DM_TEST_WR_ADDR_ADDR,m,v,HWIO_MSS_UIM1_UART_DM_TEST_WR_ADDR_IN)
#define HWIO_MSS_UIM1_UART_DM_TEST_WR_ADDR_TEST_WR_ADDR_BMSK                                                    0xffffffff
#define HWIO_MSS_UIM1_UART_DM_TEST_WR_ADDR_TEST_WR_ADDR_SHFT                                                           0x0

#define HWIO_MSS_UIM1_UART_DM_TEST_WR_DATA_ADDR                                                                 (MSS_UIM1_UART_DM_REG_BASE      + 0x00000088)
#define HWIO_MSS_UIM1_UART_DM_TEST_WR_DATA_OFFS                                                                 (MSS_UIM1_UART_DM_REG_BASE_OFFS + 0x00000088)
#define HWIO_MSS_UIM1_UART_DM_TEST_WR_DATA_RMSK                                                                 0xffffffff
#define HWIO_MSS_UIM1_UART_DM_TEST_WR_DATA_OUT(v)      \
        out_dword(HWIO_MSS_UIM1_UART_DM_TEST_WR_DATA_ADDR,v)
#define HWIO_MSS_UIM1_UART_DM_TEST_WR_DATA_TEST_WR_DATA_BMSK                                                    0xffffffff
#define HWIO_MSS_UIM1_UART_DM_TEST_WR_DATA_TEST_WR_DATA_SHFT                                                           0x0

#define HWIO_MSS_UIM1_UART_DM_TEST_RD_ADDR_ADDR                                                                 (MSS_UIM1_UART_DM_REG_BASE      + 0x0000008c)
#define HWIO_MSS_UIM1_UART_DM_TEST_RD_ADDR_OFFS                                                                 (MSS_UIM1_UART_DM_REG_BASE_OFFS + 0x0000008c)
#define HWIO_MSS_UIM1_UART_DM_TEST_RD_ADDR_RMSK                                                                 0xffffffff
#define HWIO_MSS_UIM1_UART_DM_TEST_RD_ADDR_IN          \
        in_dword_masked(HWIO_MSS_UIM1_UART_DM_TEST_RD_ADDR_ADDR, HWIO_MSS_UIM1_UART_DM_TEST_RD_ADDR_RMSK)
#define HWIO_MSS_UIM1_UART_DM_TEST_RD_ADDR_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM1_UART_DM_TEST_RD_ADDR_ADDR, m)
#define HWIO_MSS_UIM1_UART_DM_TEST_RD_ADDR_OUT(v)      \
        out_dword(HWIO_MSS_UIM1_UART_DM_TEST_RD_ADDR_ADDR,v)
#define HWIO_MSS_UIM1_UART_DM_TEST_RD_ADDR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_UIM1_UART_DM_TEST_RD_ADDR_ADDR,m,v,HWIO_MSS_UIM1_UART_DM_TEST_RD_ADDR_IN)
#define HWIO_MSS_UIM1_UART_DM_TEST_RD_ADDR_TEST_RD_ADDR_BMSK                                                    0xffffffff
#define HWIO_MSS_UIM1_UART_DM_TEST_RD_ADDR_TEST_RD_ADDR_SHFT                                                           0x0

#define HWIO_MSS_UIM1_UART_DM_TEST_RD_DATA_ADDR                                                                 (MSS_UIM1_UART_DM_REG_BASE      + 0x00000090)
#define HWIO_MSS_UIM1_UART_DM_TEST_RD_DATA_OFFS                                                                 (MSS_UIM1_UART_DM_REG_BASE_OFFS + 0x00000090)
#define HWIO_MSS_UIM1_UART_DM_TEST_RD_DATA_RMSK                                                                 0xffffffff
#define HWIO_MSS_UIM1_UART_DM_TEST_RD_DATA_IN          \
        in_dword_masked(HWIO_MSS_UIM1_UART_DM_TEST_RD_DATA_ADDR, HWIO_MSS_UIM1_UART_DM_TEST_RD_DATA_RMSK)
#define HWIO_MSS_UIM1_UART_DM_TEST_RD_DATA_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM1_UART_DM_TEST_RD_DATA_ADDR, m)
#define HWIO_MSS_UIM1_UART_DM_TEST_RD_DATA_TEST_RD_DATA_BMSK                                                    0xffffffff
#define HWIO_MSS_UIM1_UART_DM_TEST_RD_DATA_TEST_RD_DATA_SHFT                                                           0x0

#define HWIO_MSS_UIM1_UART_DM_CSR_ADDR                                                                          (MSS_UIM1_UART_DM_REG_BASE      + 0x000000a0)
#define HWIO_MSS_UIM1_UART_DM_CSR_OFFS                                                                          (MSS_UIM1_UART_DM_REG_BASE_OFFS + 0x000000a0)
#define HWIO_MSS_UIM1_UART_DM_CSR_RMSK                                                                                0xff
#define HWIO_MSS_UIM1_UART_DM_CSR_IN          \
        in_dword_masked(HWIO_MSS_UIM1_UART_DM_CSR_ADDR, HWIO_MSS_UIM1_UART_DM_CSR_RMSK)
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
#define HWIO_MSS_UIM1_UART_DM_SR_OFFS                                                                           (MSS_UIM1_UART_DM_REG_BASE_OFFS + 0x000000a4)
#define HWIO_MSS_UIM1_UART_DM_SR_RMSK                                                                                0xfff
#define HWIO_MSS_UIM1_UART_DM_SR_IN          \
        in_dword_masked(HWIO_MSS_UIM1_UART_DM_SR_ADDR, HWIO_MSS_UIM1_UART_DM_SR_RMSK)
#define HWIO_MSS_UIM1_UART_DM_SR_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM1_UART_DM_SR_ADDR, m)
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
#define HWIO_MSS_UIM1_UART_DM_CR_OFFS                                                                           (MSS_UIM1_UART_DM_REG_BASE_OFFS + 0x000000a8)
#define HWIO_MSS_UIM1_UART_DM_CR_RMSK                                                                                0xfff
#define HWIO_MSS_UIM1_UART_DM_CR_IN          \
        in_dword_masked(HWIO_MSS_UIM1_UART_DM_CR_ADDR, HWIO_MSS_UIM1_UART_DM_CR_RMSK)
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
#define HWIO_MSS_UIM1_UART_DM_MISR_OFFS                                                                         (MSS_UIM1_UART_DM_REG_BASE_OFFS + 0x000000ac)
#define HWIO_MSS_UIM1_UART_DM_MISR_RMSK                                                                            0x1ffff
#define HWIO_MSS_UIM1_UART_DM_MISR_IN          \
        in_dword_masked(HWIO_MSS_UIM1_UART_DM_MISR_ADDR, HWIO_MSS_UIM1_UART_DM_MISR_RMSK)
#define HWIO_MSS_UIM1_UART_DM_MISR_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM1_UART_DM_MISR_ADDR, m)
#define HWIO_MSS_UIM1_UART_DM_MISR_UART_MISR_BMSK                                                                  0x1ffff
#define HWIO_MSS_UIM1_UART_DM_MISR_UART_MISR_SHFT                                                                      0x0

#define HWIO_MSS_UIM1_UART_DM_IMR_ADDR                                                                          (MSS_UIM1_UART_DM_REG_BASE      + 0x000000b0)
#define HWIO_MSS_UIM1_UART_DM_IMR_OFFS                                                                          (MSS_UIM1_UART_DM_REG_BASE_OFFS + 0x000000b0)
#define HWIO_MSS_UIM1_UART_DM_IMR_RMSK                                                                             0x1ffff
#define HWIO_MSS_UIM1_UART_DM_IMR_IN          \
        in_dword_masked(HWIO_MSS_UIM1_UART_DM_IMR_ADDR, HWIO_MSS_UIM1_UART_DM_IMR_RMSK)
#define HWIO_MSS_UIM1_UART_DM_IMR_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM1_UART_DM_IMR_ADDR, m)
#define HWIO_MSS_UIM1_UART_DM_IMR_OUT(v)      \
        out_dword(HWIO_MSS_UIM1_UART_DM_IMR_ADDR,v)
#define HWIO_MSS_UIM1_UART_DM_IMR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_UIM1_UART_DM_IMR_ADDR,m,v,HWIO_MSS_UIM1_UART_DM_IMR_IN)
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
#define HWIO_MSS_UIM1_UART_DM_ISR_OFFS                                                                          (MSS_UIM1_UART_DM_REG_BASE_OFFS + 0x000000b4)
#define HWIO_MSS_UIM1_UART_DM_ISR_RMSK                                                                             0x1ffff
#define HWIO_MSS_UIM1_UART_DM_ISR_IN          \
        in_dword_masked(HWIO_MSS_UIM1_UART_DM_ISR_ADDR, HWIO_MSS_UIM1_UART_DM_ISR_RMSK)
#define HWIO_MSS_UIM1_UART_DM_ISR_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM1_UART_DM_ISR_ADDR, m)
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
#define HWIO_MSS_UIM1_UART_DM_IRDA_OFFS                                                                         (MSS_UIM1_UART_DM_REG_BASE_OFFS + 0x000000b8)
#define HWIO_MSS_UIM1_UART_DM_IRDA_RMSK                                                                               0x1f
#define HWIO_MSS_UIM1_UART_DM_IRDA_IN          \
        in_dword_masked(HWIO_MSS_UIM1_UART_DM_IRDA_ADDR, HWIO_MSS_UIM1_UART_DM_IRDA_RMSK)
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
#define HWIO_MSS_UIM1_UART_DM_RX_TOTAL_SNAP_OFFS                                                                (MSS_UIM1_UART_DM_REG_BASE_OFFS + 0x000000bc)
#define HWIO_MSS_UIM1_UART_DM_RX_TOTAL_SNAP_RMSK                                                                  0xffffff
#define HWIO_MSS_UIM1_UART_DM_RX_TOTAL_SNAP_IN          \
        in_dword_masked(HWIO_MSS_UIM1_UART_DM_RX_TOTAL_SNAP_ADDR, HWIO_MSS_UIM1_UART_DM_RX_TOTAL_SNAP_RMSK)
#define HWIO_MSS_UIM1_UART_DM_RX_TOTAL_SNAP_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM1_UART_DM_RX_TOTAL_SNAP_ADDR, m)
#define HWIO_MSS_UIM1_UART_DM_RX_TOTAL_SNAP_RX_TOTAL_BYTES_BMSK                                                   0xffffff
#define HWIO_MSS_UIM1_UART_DM_RX_TOTAL_SNAP_RX_TOTAL_BYTES_SHFT                                                        0x0

#define HWIO_MSS_UIM1_UART_DM_WWT_TIMEOUT_ADDR                                                                  (MSS_UIM1_UART_DM_REG_BASE      + 0x000000c0)
#define HWIO_MSS_UIM1_UART_DM_WWT_TIMEOUT_OFFS                                                                  (MSS_UIM1_UART_DM_REG_BASE_OFFS + 0x000000c0)
#define HWIO_MSS_UIM1_UART_DM_WWT_TIMEOUT_RMSK                                                                   0x1ffffff
#define HWIO_MSS_UIM1_UART_DM_WWT_TIMEOUT_IN          \
        in_dword_masked(HWIO_MSS_UIM1_UART_DM_WWT_TIMEOUT_ADDR, HWIO_MSS_UIM1_UART_DM_WWT_TIMEOUT_RMSK)
#define HWIO_MSS_UIM1_UART_DM_WWT_TIMEOUT_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM1_UART_DM_WWT_TIMEOUT_ADDR, m)
#define HWIO_MSS_UIM1_UART_DM_WWT_TIMEOUT_OUT(v)      \
        out_dword(HWIO_MSS_UIM1_UART_DM_WWT_TIMEOUT_ADDR,v)
#define HWIO_MSS_UIM1_UART_DM_WWT_TIMEOUT_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_UIM1_UART_DM_WWT_TIMEOUT_ADDR,m,v,HWIO_MSS_UIM1_UART_DM_WWT_TIMEOUT_IN)
#define HWIO_MSS_UIM1_UART_DM_WWT_TIMEOUT_WWT_TIMEOUT_BMSK                                                       0x1ffffff
#define HWIO_MSS_UIM1_UART_DM_WWT_TIMEOUT_WWT_TIMEOUT_SHFT                                                             0x0

#define HWIO_MSS_UIM1_UART_DM_CLK_CTRL_ADDR                                                                     (MSS_UIM1_UART_DM_REG_BASE      + 0x000000c4)
#define HWIO_MSS_UIM1_UART_DM_CLK_CTRL_OFFS                                                                     (MSS_UIM1_UART_DM_REG_BASE_OFFS + 0x000000c4)
#define HWIO_MSS_UIM1_UART_DM_CLK_CTRL_RMSK                                                                       0xf06731
#define HWIO_MSS_UIM1_UART_DM_CLK_CTRL_IN          \
        in_dword_masked(HWIO_MSS_UIM1_UART_DM_CLK_CTRL_ADDR, HWIO_MSS_UIM1_UART_DM_CLK_CTRL_RMSK)
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
#define HWIO_MSS_UIM1_UART_DM_BCR_OFFS                                                                          (MSS_UIM1_UART_DM_REG_BASE_OFFS + 0x000000c8)
#define HWIO_MSS_UIM1_UART_DM_BCR_RMSK                                                                                0x37
#define HWIO_MSS_UIM1_UART_DM_BCR_IN          \
        in_dword_masked(HWIO_MSS_UIM1_UART_DM_BCR_ADDR, HWIO_MSS_UIM1_UART_DM_BCR_RMSK)
#define HWIO_MSS_UIM1_UART_DM_BCR_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM1_UART_DM_BCR_ADDR, m)
#define HWIO_MSS_UIM1_UART_DM_BCR_OUT(v)      \
        out_dword(HWIO_MSS_UIM1_UART_DM_BCR_ADDR,v)
#define HWIO_MSS_UIM1_UART_DM_BCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_UIM1_UART_DM_BCR_ADDR,m,v,HWIO_MSS_UIM1_UART_DM_BCR_IN)
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
#define HWIO_MSS_UIM1_UART_DM_RX_TRANS_CTRL_OFFS                                                                (MSS_UIM1_UART_DM_REG_BASE_OFFS + 0x000000cc)
#define HWIO_MSS_UIM1_UART_DM_RX_TRANS_CTRL_RMSK                                                                       0x7
#define HWIO_MSS_UIM1_UART_DM_RX_TRANS_CTRL_IN          \
        in_dword_masked(HWIO_MSS_UIM1_UART_DM_RX_TRANS_CTRL_ADDR, HWIO_MSS_UIM1_UART_DM_RX_TRANS_CTRL_RMSK)
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
#define HWIO_MSS_UIM1_UART_DM_DMRX_DBG_OFFS                                                                     (MSS_UIM1_UART_DM_REG_BASE_OFFS + 0x000000d0)
#define HWIO_MSS_UIM1_UART_DM_DMRX_DBG_RMSK                                                                      0x1ffffff
#define HWIO_MSS_UIM1_UART_DM_DMRX_DBG_IN          \
        in_dword_masked(HWIO_MSS_UIM1_UART_DM_DMRX_DBG_ADDR, HWIO_MSS_UIM1_UART_DM_DMRX_DBG_RMSK)
#define HWIO_MSS_UIM1_UART_DM_DMRX_DBG_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM1_UART_DM_DMRX_DBG_ADDR, m)
#define HWIO_MSS_UIM1_UART_DM_DMRX_DBG_UART_DM_DMRX_VAL_BMSK                                                     0x1ffffff
#define HWIO_MSS_UIM1_UART_DM_DMRX_DBG_UART_DM_DMRX_VAL_SHFT                                                           0x0

#define HWIO_MSS_UIM1_UART_DM_FSM_STATUS_ADDR                                                                   (MSS_UIM1_UART_DM_REG_BASE      + 0x000000d4)
#define HWIO_MSS_UIM1_UART_DM_FSM_STATUS_OFFS                                                                   (MSS_UIM1_UART_DM_REG_BASE_OFFS + 0x000000d4)
#define HWIO_MSS_UIM1_UART_DM_FSM_STATUS_RMSK                                                                   0x37377771
#define HWIO_MSS_UIM1_UART_DM_FSM_STATUS_IN          \
        in_dword_masked(HWIO_MSS_UIM1_UART_DM_FSM_STATUS_ADDR, HWIO_MSS_UIM1_UART_DM_FSM_STATUS_RMSK)
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
#define HWIO_MSS_UIM1_UART_DM_HW_VERSION_OFFS                                                                   (MSS_UIM1_UART_DM_REG_BASE_OFFS + 0x000000d8)
#define HWIO_MSS_UIM1_UART_DM_HW_VERSION_RMSK                                                                   0xffffffff
#define HWIO_MSS_UIM1_UART_DM_HW_VERSION_IN          \
        in_dword_masked(HWIO_MSS_UIM1_UART_DM_HW_VERSION_ADDR, HWIO_MSS_UIM1_UART_DM_HW_VERSION_RMSK)
#define HWIO_MSS_UIM1_UART_DM_HW_VERSION_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM1_UART_DM_HW_VERSION_ADDR, m)
#define HWIO_MSS_UIM1_UART_DM_HW_VERSION_HW_VERSION_MAJOR_BMSK                                                  0xf0000000
#define HWIO_MSS_UIM1_UART_DM_HW_VERSION_HW_VERSION_MAJOR_SHFT                                                        0x1c
#define HWIO_MSS_UIM1_UART_DM_HW_VERSION_HW_VERSION_MINOR_BMSK                                                   0xfff0000
#define HWIO_MSS_UIM1_UART_DM_HW_VERSION_HW_VERSION_MINOR_SHFT                                                        0x10
#define HWIO_MSS_UIM1_UART_DM_HW_VERSION_HW_VERSION_STEP_BMSK                                                       0xffff
#define HWIO_MSS_UIM1_UART_DM_HW_VERSION_HW_VERSION_STEP_SHFT                                                          0x0

#define HWIO_MSS_UIM1_UART_DM_GENERICS_ADDR                                                                     (MSS_UIM1_UART_DM_REG_BASE      + 0x000000dc)
#define HWIO_MSS_UIM1_UART_DM_GENERICS_OFFS                                                                     (MSS_UIM1_UART_DM_REG_BASE_OFFS + 0x000000dc)
#define HWIO_MSS_UIM1_UART_DM_GENERICS_RMSK                                                                           0xff
#define HWIO_MSS_UIM1_UART_DM_GENERICS_IN          \
        in_dword_masked(HWIO_MSS_UIM1_UART_DM_GENERICS_ADDR, HWIO_MSS_UIM1_UART_DM_GENERICS_RMSK)
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

#define HWIO_MSS_UIM1_UART_DM_TF_ADDR                                                                           (MSS_UIM1_UART_DM_REG_BASE      + 0x00000100)
#define HWIO_MSS_UIM1_UART_DM_TF_OFFS                                                                           (MSS_UIM1_UART_DM_REG_BASE_OFFS + 0x00000100)
#define HWIO_MSS_UIM1_UART_DM_TF_RMSK                                                                           0xffffffff
#define HWIO_MSS_UIM1_UART_DM_TF_OUT(v)      \
        out_dword(HWIO_MSS_UIM1_UART_DM_TF_ADDR,v)
#define HWIO_MSS_UIM1_UART_DM_TF_UART_TF_BMSK                                                                   0xffffffff
#define HWIO_MSS_UIM1_UART_DM_TF_UART_TF_SHFT                                                                          0x0

#define HWIO_MSS_UIM1_UART_DM_TF_2_ADDR                                                                         (MSS_UIM1_UART_DM_REG_BASE      + 0x00000104)
#define HWIO_MSS_UIM1_UART_DM_TF_2_OFFS                                                                         (MSS_UIM1_UART_DM_REG_BASE_OFFS + 0x00000104)
#define HWIO_MSS_UIM1_UART_DM_TF_2_RMSK                                                                         0xffffffff
#define HWIO_MSS_UIM1_UART_DM_TF_2_OUT(v)      \
        out_dword(HWIO_MSS_UIM1_UART_DM_TF_2_ADDR,v)
#define HWIO_MSS_UIM1_UART_DM_TF_2_UART_TF_BMSK                                                                 0xffffffff
#define HWIO_MSS_UIM1_UART_DM_TF_2_UART_TF_SHFT                                                                        0x0

#define HWIO_MSS_UIM1_UART_DM_TF_3_ADDR                                                                         (MSS_UIM1_UART_DM_REG_BASE      + 0x00000108)
#define HWIO_MSS_UIM1_UART_DM_TF_3_OFFS                                                                         (MSS_UIM1_UART_DM_REG_BASE_OFFS + 0x00000108)
#define HWIO_MSS_UIM1_UART_DM_TF_3_RMSK                                                                         0xffffffff
#define HWIO_MSS_UIM1_UART_DM_TF_3_OUT(v)      \
        out_dword(HWIO_MSS_UIM1_UART_DM_TF_3_ADDR,v)
#define HWIO_MSS_UIM1_UART_DM_TF_3_UART_TF_BMSK                                                                 0xffffffff
#define HWIO_MSS_UIM1_UART_DM_TF_3_UART_TF_SHFT                                                                        0x0

#define HWIO_MSS_UIM1_UART_DM_TF_4_ADDR                                                                         (MSS_UIM1_UART_DM_REG_BASE      + 0x0000010c)
#define HWIO_MSS_UIM1_UART_DM_TF_4_OFFS                                                                         (MSS_UIM1_UART_DM_REG_BASE_OFFS + 0x0000010c)
#define HWIO_MSS_UIM1_UART_DM_TF_4_RMSK                                                                         0xffffffff
#define HWIO_MSS_UIM1_UART_DM_TF_4_OUT(v)      \
        out_dword(HWIO_MSS_UIM1_UART_DM_TF_4_ADDR,v)
#define HWIO_MSS_UIM1_UART_DM_TF_4_UART_TF_BMSK                                                                 0xffffffff
#define HWIO_MSS_UIM1_UART_DM_TF_4_UART_TF_SHFT                                                                        0x0

#define HWIO_MSS_UIM1_UART_DM_TF_5_ADDR                                                                         (MSS_UIM1_UART_DM_REG_BASE      + 0x00000110)
#define HWIO_MSS_UIM1_UART_DM_TF_5_OFFS                                                                         (MSS_UIM1_UART_DM_REG_BASE_OFFS + 0x00000110)
#define HWIO_MSS_UIM1_UART_DM_TF_5_RMSK                                                                         0xffffffff
#define HWIO_MSS_UIM1_UART_DM_TF_5_OUT(v)      \
        out_dword(HWIO_MSS_UIM1_UART_DM_TF_5_ADDR,v)
#define HWIO_MSS_UIM1_UART_DM_TF_5_UART_TF_BMSK                                                                 0xffffffff
#define HWIO_MSS_UIM1_UART_DM_TF_5_UART_TF_SHFT                                                                        0x0

#define HWIO_MSS_UIM1_UART_DM_TF_6_ADDR                                                                         (MSS_UIM1_UART_DM_REG_BASE      + 0x00000114)
#define HWIO_MSS_UIM1_UART_DM_TF_6_OFFS                                                                         (MSS_UIM1_UART_DM_REG_BASE_OFFS + 0x00000114)
#define HWIO_MSS_UIM1_UART_DM_TF_6_RMSK                                                                         0xffffffff
#define HWIO_MSS_UIM1_UART_DM_TF_6_OUT(v)      \
        out_dword(HWIO_MSS_UIM1_UART_DM_TF_6_ADDR,v)
#define HWIO_MSS_UIM1_UART_DM_TF_6_UART_TF_BMSK                                                                 0xffffffff
#define HWIO_MSS_UIM1_UART_DM_TF_6_UART_TF_SHFT                                                                        0x0

#define HWIO_MSS_UIM1_UART_DM_TF_7_ADDR                                                                         (MSS_UIM1_UART_DM_REG_BASE      + 0x00000118)
#define HWIO_MSS_UIM1_UART_DM_TF_7_OFFS                                                                         (MSS_UIM1_UART_DM_REG_BASE_OFFS + 0x00000118)
#define HWIO_MSS_UIM1_UART_DM_TF_7_RMSK                                                                         0xffffffff
#define HWIO_MSS_UIM1_UART_DM_TF_7_OUT(v)      \
        out_dword(HWIO_MSS_UIM1_UART_DM_TF_7_ADDR,v)
#define HWIO_MSS_UIM1_UART_DM_TF_7_UART_TF_BMSK                                                                 0xffffffff
#define HWIO_MSS_UIM1_UART_DM_TF_7_UART_TF_SHFT                                                                        0x0

#define HWIO_MSS_UIM1_UART_DM_TF_8_ADDR                                                                         (MSS_UIM1_UART_DM_REG_BASE      + 0x0000011c)
#define HWIO_MSS_UIM1_UART_DM_TF_8_OFFS                                                                         (MSS_UIM1_UART_DM_REG_BASE_OFFS + 0x0000011c)
#define HWIO_MSS_UIM1_UART_DM_TF_8_RMSK                                                                         0xffffffff
#define HWIO_MSS_UIM1_UART_DM_TF_8_OUT(v)      \
        out_dword(HWIO_MSS_UIM1_UART_DM_TF_8_ADDR,v)
#define HWIO_MSS_UIM1_UART_DM_TF_8_UART_TF_BMSK                                                                 0xffffffff
#define HWIO_MSS_UIM1_UART_DM_TF_8_UART_TF_SHFT                                                                        0x0

#define HWIO_MSS_UIM1_UART_DM_TF_9_ADDR                                                                         (MSS_UIM1_UART_DM_REG_BASE      + 0x00000120)
#define HWIO_MSS_UIM1_UART_DM_TF_9_OFFS                                                                         (MSS_UIM1_UART_DM_REG_BASE_OFFS + 0x00000120)
#define HWIO_MSS_UIM1_UART_DM_TF_9_RMSK                                                                         0xffffffff
#define HWIO_MSS_UIM1_UART_DM_TF_9_OUT(v)      \
        out_dword(HWIO_MSS_UIM1_UART_DM_TF_9_ADDR,v)
#define HWIO_MSS_UIM1_UART_DM_TF_9_UART_TF_BMSK                                                                 0xffffffff
#define HWIO_MSS_UIM1_UART_DM_TF_9_UART_TF_SHFT                                                                        0x0

#define HWIO_MSS_UIM1_UART_DM_TF_10_ADDR                                                                        (MSS_UIM1_UART_DM_REG_BASE      + 0x00000124)
#define HWIO_MSS_UIM1_UART_DM_TF_10_OFFS                                                                        (MSS_UIM1_UART_DM_REG_BASE_OFFS + 0x00000124)
#define HWIO_MSS_UIM1_UART_DM_TF_10_RMSK                                                                        0xffffffff
#define HWIO_MSS_UIM1_UART_DM_TF_10_OUT(v)      \
        out_dword(HWIO_MSS_UIM1_UART_DM_TF_10_ADDR,v)
#define HWIO_MSS_UIM1_UART_DM_TF_10_UART_TF_BMSK                                                                0xffffffff
#define HWIO_MSS_UIM1_UART_DM_TF_10_UART_TF_SHFT                                                                       0x0

#define HWIO_MSS_UIM1_UART_DM_TF_11_ADDR                                                                        (MSS_UIM1_UART_DM_REG_BASE      + 0x00000128)
#define HWIO_MSS_UIM1_UART_DM_TF_11_OFFS                                                                        (MSS_UIM1_UART_DM_REG_BASE_OFFS + 0x00000128)
#define HWIO_MSS_UIM1_UART_DM_TF_11_RMSK                                                                        0xffffffff
#define HWIO_MSS_UIM1_UART_DM_TF_11_OUT(v)      \
        out_dword(HWIO_MSS_UIM1_UART_DM_TF_11_ADDR,v)
#define HWIO_MSS_UIM1_UART_DM_TF_11_UART_TF_BMSK                                                                0xffffffff
#define HWIO_MSS_UIM1_UART_DM_TF_11_UART_TF_SHFT                                                                       0x0

#define HWIO_MSS_UIM1_UART_DM_TF_12_ADDR                                                                        (MSS_UIM1_UART_DM_REG_BASE      + 0x0000012c)
#define HWIO_MSS_UIM1_UART_DM_TF_12_OFFS                                                                        (MSS_UIM1_UART_DM_REG_BASE_OFFS + 0x0000012c)
#define HWIO_MSS_UIM1_UART_DM_TF_12_RMSK                                                                        0xffffffff
#define HWIO_MSS_UIM1_UART_DM_TF_12_OUT(v)      \
        out_dword(HWIO_MSS_UIM1_UART_DM_TF_12_ADDR,v)
#define HWIO_MSS_UIM1_UART_DM_TF_12_UART_TF_BMSK                                                                0xffffffff
#define HWIO_MSS_UIM1_UART_DM_TF_12_UART_TF_SHFT                                                                       0x0

#define HWIO_MSS_UIM1_UART_DM_TF_13_ADDR                                                                        (MSS_UIM1_UART_DM_REG_BASE      + 0x00000130)
#define HWIO_MSS_UIM1_UART_DM_TF_13_OFFS                                                                        (MSS_UIM1_UART_DM_REG_BASE_OFFS + 0x00000130)
#define HWIO_MSS_UIM1_UART_DM_TF_13_RMSK                                                                        0xffffffff
#define HWIO_MSS_UIM1_UART_DM_TF_13_OUT(v)      \
        out_dword(HWIO_MSS_UIM1_UART_DM_TF_13_ADDR,v)
#define HWIO_MSS_UIM1_UART_DM_TF_13_UART_TF_BMSK                                                                0xffffffff
#define HWIO_MSS_UIM1_UART_DM_TF_13_UART_TF_SHFT                                                                       0x0

#define HWIO_MSS_UIM1_UART_DM_TF_14_ADDR                                                                        (MSS_UIM1_UART_DM_REG_BASE      + 0x00000134)
#define HWIO_MSS_UIM1_UART_DM_TF_14_OFFS                                                                        (MSS_UIM1_UART_DM_REG_BASE_OFFS + 0x00000134)
#define HWIO_MSS_UIM1_UART_DM_TF_14_RMSK                                                                        0xffffffff
#define HWIO_MSS_UIM1_UART_DM_TF_14_OUT(v)      \
        out_dword(HWIO_MSS_UIM1_UART_DM_TF_14_ADDR,v)
#define HWIO_MSS_UIM1_UART_DM_TF_14_UART_TF_BMSK                                                                0xffffffff
#define HWIO_MSS_UIM1_UART_DM_TF_14_UART_TF_SHFT                                                                       0x0

#define HWIO_MSS_UIM1_UART_DM_TF_15_ADDR                                                                        (MSS_UIM1_UART_DM_REG_BASE      + 0x00000138)
#define HWIO_MSS_UIM1_UART_DM_TF_15_OFFS                                                                        (MSS_UIM1_UART_DM_REG_BASE_OFFS + 0x00000138)
#define HWIO_MSS_UIM1_UART_DM_TF_15_RMSK                                                                        0xffffffff
#define HWIO_MSS_UIM1_UART_DM_TF_15_OUT(v)      \
        out_dword(HWIO_MSS_UIM1_UART_DM_TF_15_ADDR,v)
#define HWIO_MSS_UIM1_UART_DM_TF_15_UART_TF_BMSK                                                                0xffffffff
#define HWIO_MSS_UIM1_UART_DM_TF_15_UART_TF_SHFT                                                                       0x0

#define HWIO_MSS_UIM1_UART_DM_TF_16_ADDR                                                                        (MSS_UIM1_UART_DM_REG_BASE      + 0x0000013c)
#define HWIO_MSS_UIM1_UART_DM_TF_16_OFFS                                                                        (MSS_UIM1_UART_DM_REG_BASE_OFFS + 0x0000013c)
#define HWIO_MSS_UIM1_UART_DM_TF_16_RMSK                                                                        0xffffffff
#define HWIO_MSS_UIM1_UART_DM_TF_16_OUT(v)      \
        out_dword(HWIO_MSS_UIM1_UART_DM_TF_16_ADDR,v)
#define HWIO_MSS_UIM1_UART_DM_TF_16_UART_TF_BMSK                                                                0xffffffff
#define HWIO_MSS_UIM1_UART_DM_TF_16_UART_TF_SHFT                                                                       0x0

#define HWIO_MSS_UIM1_UART_DM_RF_ADDR                                                                           (MSS_UIM1_UART_DM_REG_BASE      + 0x00000140)
#define HWIO_MSS_UIM1_UART_DM_RF_OFFS                                                                           (MSS_UIM1_UART_DM_REG_BASE_OFFS + 0x00000140)
#define HWIO_MSS_UIM1_UART_DM_RF_RMSK                                                                           0xffffffff
#define HWIO_MSS_UIM1_UART_DM_RF_IN          \
        in_dword_masked(HWIO_MSS_UIM1_UART_DM_RF_ADDR, HWIO_MSS_UIM1_UART_DM_RF_RMSK)
#define HWIO_MSS_UIM1_UART_DM_RF_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM1_UART_DM_RF_ADDR, m)
#define HWIO_MSS_UIM1_UART_DM_RF_UART_RF_BMSK                                                                   0xffffffff
#define HWIO_MSS_UIM1_UART_DM_RF_UART_RF_SHFT                                                                          0x0

#define HWIO_MSS_UIM1_UART_DM_RF_2_ADDR                                                                         (MSS_UIM1_UART_DM_REG_BASE      + 0x00000144)
#define HWIO_MSS_UIM1_UART_DM_RF_2_OFFS                                                                         (MSS_UIM1_UART_DM_REG_BASE_OFFS + 0x00000144)
#define HWIO_MSS_UIM1_UART_DM_RF_2_RMSK                                                                         0xffffffff
#define HWIO_MSS_UIM1_UART_DM_RF_2_IN          \
        in_dword_masked(HWIO_MSS_UIM1_UART_DM_RF_2_ADDR, HWIO_MSS_UIM1_UART_DM_RF_2_RMSK)
#define HWIO_MSS_UIM1_UART_DM_RF_2_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM1_UART_DM_RF_2_ADDR, m)
#define HWIO_MSS_UIM1_UART_DM_RF_2_UART_RF_BMSK                                                                 0xffffffff
#define HWIO_MSS_UIM1_UART_DM_RF_2_UART_RF_SHFT                                                                        0x0

#define HWIO_MSS_UIM1_UART_DM_RF_3_ADDR                                                                         (MSS_UIM1_UART_DM_REG_BASE      + 0x00000148)
#define HWIO_MSS_UIM1_UART_DM_RF_3_OFFS                                                                         (MSS_UIM1_UART_DM_REG_BASE_OFFS + 0x00000148)
#define HWIO_MSS_UIM1_UART_DM_RF_3_RMSK                                                                         0xffffffff
#define HWIO_MSS_UIM1_UART_DM_RF_3_IN          \
        in_dword_masked(HWIO_MSS_UIM1_UART_DM_RF_3_ADDR, HWIO_MSS_UIM1_UART_DM_RF_3_RMSK)
#define HWIO_MSS_UIM1_UART_DM_RF_3_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM1_UART_DM_RF_3_ADDR, m)
#define HWIO_MSS_UIM1_UART_DM_RF_3_UART_RF_BMSK                                                                 0xffffffff
#define HWIO_MSS_UIM1_UART_DM_RF_3_UART_RF_SHFT                                                                        0x0

#define HWIO_MSS_UIM1_UART_DM_RF_4_ADDR                                                                         (MSS_UIM1_UART_DM_REG_BASE      + 0x0000014c)
#define HWIO_MSS_UIM1_UART_DM_RF_4_OFFS                                                                         (MSS_UIM1_UART_DM_REG_BASE_OFFS + 0x0000014c)
#define HWIO_MSS_UIM1_UART_DM_RF_4_RMSK                                                                         0xffffffff
#define HWIO_MSS_UIM1_UART_DM_RF_4_IN          \
        in_dword_masked(HWIO_MSS_UIM1_UART_DM_RF_4_ADDR, HWIO_MSS_UIM1_UART_DM_RF_4_RMSK)
#define HWIO_MSS_UIM1_UART_DM_RF_4_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM1_UART_DM_RF_4_ADDR, m)
#define HWIO_MSS_UIM1_UART_DM_RF_4_UART_RF_BMSK                                                                 0xffffffff
#define HWIO_MSS_UIM1_UART_DM_RF_4_UART_RF_SHFT                                                                        0x0

#define HWIO_MSS_UIM1_UART_DM_RF_5_ADDR                                                                         (MSS_UIM1_UART_DM_REG_BASE      + 0x00000150)
#define HWIO_MSS_UIM1_UART_DM_RF_5_OFFS                                                                         (MSS_UIM1_UART_DM_REG_BASE_OFFS + 0x00000150)
#define HWIO_MSS_UIM1_UART_DM_RF_5_RMSK                                                                         0xffffffff
#define HWIO_MSS_UIM1_UART_DM_RF_5_IN          \
        in_dword_masked(HWIO_MSS_UIM1_UART_DM_RF_5_ADDR, HWIO_MSS_UIM1_UART_DM_RF_5_RMSK)
#define HWIO_MSS_UIM1_UART_DM_RF_5_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM1_UART_DM_RF_5_ADDR, m)
#define HWIO_MSS_UIM1_UART_DM_RF_5_UART_RF_BMSK                                                                 0xffffffff
#define HWIO_MSS_UIM1_UART_DM_RF_5_UART_RF_SHFT                                                                        0x0

#define HWIO_MSS_UIM1_UART_DM_RF_6_ADDR                                                                         (MSS_UIM1_UART_DM_REG_BASE      + 0x00000154)
#define HWIO_MSS_UIM1_UART_DM_RF_6_OFFS                                                                         (MSS_UIM1_UART_DM_REG_BASE_OFFS + 0x00000154)
#define HWIO_MSS_UIM1_UART_DM_RF_6_RMSK                                                                         0xffffffff
#define HWIO_MSS_UIM1_UART_DM_RF_6_IN          \
        in_dword_masked(HWIO_MSS_UIM1_UART_DM_RF_6_ADDR, HWIO_MSS_UIM1_UART_DM_RF_6_RMSK)
#define HWIO_MSS_UIM1_UART_DM_RF_6_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM1_UART_DM_RF_6_ADDR, m)
#define HWIO_MSS_UIM1_UART_DM_RF_6_UART_RF_BMSK                                                                 0xffffffff
#define HWIO_MSS_UIM1_UART_DM_RF_6_UART_RF_SHFT                                                                        0x0

#define HWIO_MSS_UIM1_UART_DM_RF_7_ADDR                                                                         (MSS_UIM1_UART_DM_REG_BASE      + 0x00000158)
#define HWIO_MSS_UIM1_UART_DM_RF_7_OFFS                                                                         (MSS_UIM1_UART_DM_REG_BASE_OFFS + 0x00000158)
#define HWIO_MSS_UIM1_UART_DM_RF_7_RMSK                                                                         0xffffffff
#define HWIO_MSS_UIM1_UART_DM_RF_7_IN          \
        in_dword_masked(HWIO_MSS_UIM1_UART_DM_RF_7_ADDR, HWIO_MSS_UIM1_UART_DM_RF_7_RMSK)
#define HWIO_MSS_UIM1_UART_DM_RF_7_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM1_UART_DM_RF_7_ADDR, m)
#define HWIO_MSS_UIM1_UART_DM_RF_7_UART_RF_BMSK                                                                 0xffffffff
#define HWIO_MSS_UIM1_UART_DM_RF_7_UART_RF_SHFT                                                                        0x0

#define HWIO_MSS_UIM1_UART_DM_RF_8_ADDR                                                                         (MSS_UIM1_UART_DM_REG_BASE      + 0x0000015c)
#define HWIO_MSS_UIM1_UART_DM_RF_8_OFFS                                                                         (MSS_UIM1_UART_DM_REG_BASE_OFFS + 0x0000015c)
#define HWIO_MSS_UIM1_UART_DM_RF_8_RMSK                                                                         0xffffffff
#define HWIO_MSS_UIM1_UART_DM_RF_8_IN          \
        in_dword_masked(HWIO_MSS_UIM1_UART_DM_RF_8_ADDR, HWIO_MSS_UIM1_UART_DM_RF_8_RMSK)
#define HWIO_MSS_UIM1_UART_DM_RF_8_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM1_UART_DM_RF_8_ADDR, m)
#define HWIO_MSS_UIM1_UART_DM_RF_8_UART_RF_BMSK                                                                 0xffffffff
#define HWIO_MSS_UIM1_UART_DM_RF_8_UART_RF_SHFT                                                                        0x0

#define HWIO_MSS_UIM1_UART_DM_RF_9_ADDR                                                                         (MSS_UIM1_UART_DM_REG_BASE      + 0x00000160)
#define HWIO_MSS_UIM1_UART_DM_RF_9_OFFS                                                                         (MSS_UIM1_UART_DM_REG_BASE_OFFS + 0x00000160)
#define HWIO_MSS_UIM1_UART_DM_RF_9_RMSK                                                                         0xffffffff
#define HWIO_MSS_UIM1_UART_DM_RF_9_IN          \
        in_dword_masked(HWIO_MSS_UIM1_UART_DM_RF_9_ADDR, HWIO_MSS_UIM1_UART_DM_RF_9_RMSK)
#define HWIO_MSS_UIM1_UART_DM_RF_9_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM1_UART_DM_RF_9_ADDR, m)
#define HWIO_MSS_UIM1_UART_DM_RF_9_UART_RF_BMSK                                                                 0xffffffff
#define HWIO_MSS_UIM1_UART_DM_RF_9_UART_RF_SHFT                                                                        0x0

#define HWIO_MSS_UIM1_UART_DM_RF_10_ADDR                                                                        (MSS_UIM1_UART_DM_REG_BASE      + 0x00000164)
#define HWIO_MSS_UIM1_UART_DM_RF_10_OFFS                                                                        (MSS_UIM1_UART_DM_REG_BASE_OFFS + 0x00000164)
#define HWIO_MSS_UIM1_UART_DM_RF_10_RMSK                                                                        0xffffffff
#define HWIO_MSS_UIM1_UART_DM_RF_10_IN          \
        in_dword_masked(HWIO_MSS_UIM1_UART_DM_RF_10_ADDR, HWIO_MSS_UIM1_UART_DM_RF_10_RMSK)
#define HWIO_MSS_UIM1_UART_DM_RF_10_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM1_UART_DM_RF_10_ADDR, m)
#define HWIO_MSS_UIM1_UART_DM_RF_10_UART_RF_BMSK                                                                0xffffffff
#define HWIO_MSS_UIM1_UART_DM_RF_10_UART_RF_SHFT                                                                       0x0

#define HWIO_MSS_UIM1_UART_DM_RF_11_ADDR                                                                        (MSS_UIM1_UART_DM_REG_BASE      + 0x00000168)
#define HWIO_MSS_UIM1_UART_DM_RF_11_OFFS                                                                        (MSS_UIM1_UART_DM_REG_BASE_OFFS + 0x00000168)
#define HWIO_MSS_UIM1_UART_DM_RF_11_RMSK                                                                        0xffffffff
#define HWIO_MSS_UIM1_UART_DM_RF_11_IN          \
        in_dword_masked(HWIO_MSS_UIM1_UART_DM_RF_11_ADDR, HWIO_MSS_UIM1_UART_DM_RF_11_RMSK)
#define HWIO_MSS_UIM1_UART_DM_RF_11_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM1_UART_DM_RF_11_ADDR, m)
#define HWIO_MSS_UIM1_UART_DM_RF_11_UART_RF_BMSK                                                                0xffffffff
#define HWIO_MSS_UIM1_UART_DM_RF_11_UART_RF_SHFT                                                                       0x0

#define HWIO_MSS_UIM1_UART_DM_RF_12_ADDR                                                                        (MSS_UIM1_UART_DM_REG_BASE      + 0x0000016c)
#define HWIO_MSS_UIM1_UART_DM_RF_12_OFFS                                                                        (MSS_UIM1_UART_DM_REG_BASE_OFFS + 0x0000016c)
#define HWIO_MSS_UIM1_UART_DM_RF_12_RMSK                                                                        0xffffffff
#define HWIO_MSS_UIM1_UART_DM_RF_12_IN          \
        in_dword_masked(HWIO_MSS_UIM1_UART_DM_RF_12_ADDR, HWIO_MSS_UIM1_UART_DM_RF_12_RMSK)
#define HWIO_MSS_UIM1_UART_DM_RF_12_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM1_UART_DM_RF_12_ADDR, m)
#define HWIO_MSS_UIM1_UART_DM_RF_12_UART_RF_BMSK                                                                0xffffffff
#define HWIO_MSS_UIM1_UART_DM_RF_12_UART_RF_SHFT                                                                       0x0

#define HWIO_MSS_UIM1_UART_DM_RF_13_ADDR                                                                        (MSS_UIM1_UART_DM_REG_BASE      + 0x00000170)
#define HWIO_MSS_UIM1_UART_DM_RF_13_OFFS                                                                        (MSS_UIM1_UART_DM_REG_BASE_OFFS + 0x00000170)
#define HWIO_MSS_UIM1_UART_DM_RF_13_RMSK                                                                        0xffffffff
#define HWIO_MSS_UIM1_UART_DM_RF_13_IN          \
        in_dword_masked(HWIO_MSS_UIM1_UART_DM_RF_13_ADDR, HWIO_MSS_UIM1_UART_DM_RF_13_RMSK)
#define HWIO_MSS_UIM1_UART_DM_RF_13_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM1_UART_DM_RF_13_ADDR, m)
#define HWIO_MSS_UIM1_UART_DM_RF_13_UART_RF_BMSK                                                                0xffffffff
#define HWIO_MSS_UIM1_UART_DM_RF_13_UART_RF_SHFT                                                                       0x0

#define HWIO_MSS_UIM1_UART_DM_RF_14_ADDR                                                                        (MSS_UIM1_UART_DM_REG_BASE      + 0x00000174)
#define HWIO_MSS_UIM1_UART_DM_RF_14_OFFS                                                                        (MSS_UIM1_UART_DM_REG_BASE_OFFS + 0x00000174)
#define HWIO_MSS_UIM1_UART_DM_RF_14_RMSK                                                                        0xffffffff
#define HWIO_MSS_UIM1_UART_DM_RF_14_IN          \
        in_dword_masked(HWIO_MSS_UIM1_UART_DM_RF_14_ADDR, HWIO_MSS_UIM1_UART_DM_RF_14_RMSK)
#define HWIO_MSS_UIM1_UART_DM_RF_14_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM1_UART_DM_RF_14_ADDR, m)
#define HWIO_MSS_UIM1_UART_DM_RF_14_UART_RF_BMSK                                                                0xffffffff
#define HWIO_MSS_UIM1_UART_DM_RF_14_UART_RF_SHFT                                                                       0x0

#define HWIO_MSS_UIM1_UART_DM_RF_15_ADDR                                                                        (MSS_UIM1_UART_DM_REG_BASE      + 0x00000178)
#define HWIO_MSS_UIM1_UART_DM_RF_15_OFFS                                                                        (MSS_UIM1_UART_DM_REG_BASE_OFFS + 0x00000178)
#define HWIO_MSS_UIM1_UART_DM_RF_15_RMSK                                                                        0xffffffff
#define HWIO_MSS_UIM1_UART_DM_RF_15_IN          \
        in_dword_masked(HWIO_MSS_UIM1_UART_DM_RF_15_ADDR, HWIO_MSS_UIM1_UART_DM_RF_15_RMSK)
#define HWIO_MSS_UIM1_UART_DM_RF_15_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM1_UART_DM_RF_15_ADDR, m)
#define HWIO_MSS_UIM1_UART_DM_RF_15_UART_RF_BMSK                                                                0xffffffff
#define HWIO_MSS_UIM1_UART_DM_RF_15_UART_RF_SHFT                                                                       0x0

#define HWIO_MSS_UIM1_UART_DM_RF_16_ADDR                                                                        (MSS_UIM1_UART_DM_REG_BASE      + 0x0000017c)
#define HWIO_MSS_UIM1_UART_DM_RF_16_OFFS                                                                        (MSS_UIM1_UART_DM_REG_BASE_OFFS + 0x0000017c)
#define HWIO_MSS_UIM1_UART_DM_RF_16_RMSK                                                                        0xffffffff
#define HWIO_MSS_UIM1_UART_DM_RF_16_IN          \
        in_dword_masked(HWIO_MSS_UIM1_UART_DM_RF_16_ADDR, HWIO_MSS_UIM1_UART_DM_RF_16_RMSK)
#define HWIO_MSS_UIM1_UART_DM_RF_16_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM1_UART_DM_RF_16_ADDR, m)
#define HWIO_MSS_UIM1_UART_DM_RF_16_UART_RF_BMSK                                                                0xffffffff
#define HWIO_MSS_UIM1_UART_DM_RF_16_UART_RF_SHFT                                                                       0x0

#define HWIO_MSS_UIM1_UART_DM_UIM_CFG_ADDR                                                                      (MSS_UIM1_UART_DM_REG_BASE      + 0x00000180)
#define HWIO_MSS_UIM1_UART_DM_UIM_CFG_OFFS                                                                      (MSS_UIM1_UART_DM_REG_BASE_OFFS + 0x00000180)
#define HWIO_MSS_UIM1_UART_DM_UIM_CFG_RMSK                                                                          0xf77f
#define HWIO_MSS_UIM1_UART_DM_UIM_CFG_IN          \
        in_dword_masked(HWIO_MSS_UIM1_UART_DM_UIM_CFG_ADDR, HWIO_MSS_UIM1_UART_DM_UIM_CFG_RMSK)
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
#define HWIO_MSS_UIM1_UART_DM_UIM_CMD_OFFS                                                                      (MSS_UIM1_UART_DM_REG_BASE_OFFS + 0x00000184)
#define HWIO_MSS_UIM1_UART_DM_UIM_CMD_RMSK                                                                             0x3
#define HWIO_MSS_UIM1_UART_DM_UIM_CMD_OUT(v)      \
        out_dword(HWIO_MSS_UIM1_UART_DM_UIM_CMD_ADDR,v)
#define HWIO_MSS_UIM1_UART_DM_UIM_CMD_RECOVER_FROM_HW_DEACTIVATION_BMSK                                                0x2
#define HWIO_MSS_UIM1_UART_DM_UIM_CMD_RECOVER_FROM_HW_DEACTIVATION_SHFT                                                0x1
#define HWIO_MSS_UIM1_UART_DM_UIM_CMD_INITIATE_HW_DEACTIVATION_BMSK                                                    0x1
#define HWIO_MSS_UIM1_UART_DM_UIM_CMD_INITIATE_HW_DEACTIVATION_SHFT                                                    0x0

#define HWIO_MSS_UIM1_UART_DM_UIM_IO_STATUS_ADDR                                                                (MSS_UIM1_UART_DM_REG_BASE      + 0x00000188)
#define HWIO_MSS_UIM1_UART_DM_UIM_IO_STATUS_OFFS                                                                (MSS_UIM1_UART_DM_REG_BASE_OFFS + 0x00000188)
#define HWIO_MSS_UIM1_UART_DM_UIM_IO_STATUS_RMSK                                                                       0x7
#define HWIO_MSS_UIM1_UART_DM_UIM_IO_STATUS_IN          \
        in_dword_masked(HWIO_MSS_UIM1_UART_DM_UIM_IO_STATUS_ADDR, HWIO_MSS_UIM1_UART_DM_UIM_IO_STATUS_RMSK)
#define HWIO_MSS_UIM1_UART_DM_UIM_IO_STATUS_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM1_UART_DM_UIM_IO_STATUS_ADDR, m)
#define HWIO_MSS_UIM1_UART_DM_UIM_IO_STATUS_UIM_IO_WRITE_IN_PROGRESS_BMSK                                              0x4
#define HWIO_MSS_UIM1_UART_DM_UIM_IO_STATUS_UIM_IO_WRITE_IN_PROGRESS_SHFT                                              0x2
#define HWIO_MSS_UIM1_UART_DM_UIM_IO_STATUS_UIM_DEACTIVATION_STATUS_BMSK                                               0x2
#define HWIO_MSS_UIM1_UART_DM_UIM_IO_STATUS_UIM_DEACTIVATION_STATUS_SHFT                                               0x1
#define HWIO_MSS_UIM1_UART_DM_UIM_IO_STATUS_CARD_PRESENCE_BMSK                                                         0x1
#define HWIO_MSS_UIM1_UART_DM_UIM_IO_STATUS_CARD_PRESENCE_SHFT                                                         0x0

#define HWIO_MSS_UIM1_UART_DM_UIM_IRQ_ISR_ADDR                                                                  (MSS_UIM1_UART_DM_REG_BASE      + 0x0000018c)
#define HWIO_MSS_UIM1_UART_DM_UIM_IRQ_ISR_OFFS                                                                  (MSS_UIM1_UART_DM_REG_BASE_OFFS + 0x0000018c)
#define HWIO_MSS_UIM1_UART_DM_UIM_IRQ_ISR_RMSK                                                                        0x1f
#define HWIO_MSS_UIM1_UART_DM_UIM_IRQ_ISR_IN          \
        in_dword_masked(HWIO_MSS_UIM1_UART_DM_UIM_IRQ_ISR_ADDR, HWIO_MSS_UIM1_UART_DM_UIM_IRQ_ISR_RMSK)
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
#define HWIO_MSS_UIM1_UART_DM_UIM_IRQ_MISR_OFFS                                                                 (MSS_UIM1_UART_DM_REG_BASE_OFFS + 0x00000190)
#define HWIO_MSS_UIM1_UART_DM_UIM_IRQ_MISR_RMSK                                                                       0x1f
#define HWIO_MSS_UIM1_UART_DM_UIM_IRQ_MISR_IN          \
        in_dword_masked(HWIO_MSS_UIM1_UART_DM_UIM_IRQ_MISR_ADDR, HWIO_MSS_UIM1_UART_DM_UIM_IRQ_MISR_RMSK)
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
#define HWIO_MSS_UIM1_UART_DM_UIM_IRQ_CLR_OFFS                                                                  (MSS_UIM1_UART_DM_REG_BASE_OFFS + 0x00000194)
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
#define HWIO_MSS_UIM1_UART_DM_UIM_IRQ_IMR_OFFS                                                                  (MSS_UIM1_UART_DM_REG_BASE_OFFS + 0x00000198)
#define HWIO_MSS_UIM1_UART_DM_UIM_IRQ_IMR_RMSK                                                                        0x1f
#define HWIO_MSS_UIM1_UART_DM_UIM_IRQ_IMR_IN          \
        in_dword_masked(HWIO_MSS_UIM1_UART_DM_UIM_IRQ_IMR_ADDR, HWIO_MSS_UIM1_UART_DM_UIM_IRQ_IMR_RMSK)
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
#define HWIO_MSS_UIM1_UART_DM_UIM_IRQ_IMR_SET_OFFS                                                              (MSS_UIM1_UART_DM_REG_BASE_OFFS + 0x0000019c)
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
#define HWIO_MSS_UIM1_UART_DM_UIM_IRQ_IMR_CLR_OFFS                                                              (MSS_UIM1_UART_DM_REG_BASE_OFFS + 0x000001a0)
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
 * MODULE: MSS_UIM2_UART_DM
 *--------------------------------------------------------------------------*/

#define MSS_UIM2_UART_DM_REG_BASE                                                                               (MSS_TOP_BASE      + 0x001a8000)
#define MSS_UIM2_UART_DM_REG_BASE_OFFS                                                                          0x001a8000

#define HWIO_MSS_UIM2_UART_DM_MR1_ADDR                                                                          (MSS_UIM2_UART_DM_REG_BASE      + 0x00000000)
#define HWIO_MSS_UIM2_UART_DM_MR1_OFFS                                                                          (MSS_UIM2_UART_DM_REG_BASE_OFFS + 0x00000000)
#define HWIO_MSS_UIM2_UART_DM_MR1_RMSK                                                                          0xffffffff
#define HWIO_MSS_UIM2_UART_DM_MR1_IN          \
        in_dword_masked(HWIO_MSS_UIM2_UART_DM_MR1_ADDR, HWIO_MSS_UIM2_UART_DM_MR1_RMSK)
#define HWIO_MSS_UIM2_UART_DM_MR1_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM2_UART_DM_MR1_ADDR, m)
#define HWIO_MSS_UIM2_UART_DM_MR1_OUT(v)      \
        out_dword(HWIO_MSS_UIM2_UART_DM_MR1_ADDR,v)
#define HWIO_MSS_UIM2_UART_DM_MR1_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_UIM2_UART_DM_MR1_ADDR,m,v,HWIO_MSS_UIM2_UART_DM_MR1_IN)
#define HWIO_MSS_UIM2_UART_DM_MR1_AUTO_RFR_LEVEL1_BMSK                                                          0xffffff00
#define HWIO_MSS_UIM2_UART_DM_MR1_AUTO_RFR_LEVEL1_SHFT                                                                 0x8
#define HWIO_MSS_UIM2_UART_DM_MR1_RX_RDY_CTL_BMSK                                                                     0x80
#define HWIO_MSS_UIM2_UART_DM_MR1_RX_RDY_CTL_SHFT                                                                      0x7
#define HWIO_MSS_UIM2_UART_DM_MR1_CTS_CTL_BMSK                                                                        0x40
#define HWIO_MSS_UIM2_UART_DM_MR1_CTS_CTL_SHFT                                                                         0x6
#define HWIO_MSS_UIM2_UART_DM_MR1_AUTO_RFR_LEVEL0_BMSK                                                                0x3f
#define HWIO_MSS_UIM2_UART_DM_MR1_AUTO_RFR_LEVEL0_SHFT                                                                 0x0

#define HWIO_MSS_UIM2_UART_DM_MR2_ADDR                                                                          (MSS_UIM2_UART_DM_REG_BASE      + 0x00000004)
#define HWIO_MSS_UIM2_UART_DM_MR2_OFFS                                                                          (MSS_UIM2_UART_DM_REG_BASE_OFFS + 0x00000004)
#define HWIO_MSS_UIM2_UART_DM_MR2_RMSK                                                                               0x7ff
#define HWIO_MSS_UIM2_UART_DM_MR2_IN          \
        in_dword_masked(HWIO_MSS_UIM2_UART_DM_MR2_ADDR, HWIO_MSS_UIM2_UART_DM_MR2_RMSK)
#define HWIO_MSS_UIM2_UART_DM_MR2_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM2_UART_DM_MR2_ADDR, m)
#define HWIO_MSS_UIM2_UART_DM_MR2_OUT(v)      \
        out_dword(HWIO_MSS_UIM2_UART_DM_MR2_ADDR,v)
#define HWIO_MSS_UIM2_UART_DM_MR2_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_UIM2_UART_DM_MR2_ADDR,m,v,HWIO_MSS_UIM2_UART_DM_MR2_IN)
#define HWIO_MSS_UIM2_UART_DM_MR2_RFR_CTS_LOOPBACK_BMSK                                                              0x400
#define HWIO_MSS_UIM2_UART_DM_MR2_RFR_CTS_LOOPBACK_SHFT                                                                0xa
#define HWIO_MSS_UIM2_UART_DM_MR2_RX_ERROR_CHAR_OFF_BMSK                                                             0x200
#define HWIO_MSS_UIM2_UART_DM_MR2_RX_ERROR_CHAR_OFF_SHFT                                                               0x9
#define HWIO_MSS_UIM2_UART_DM_MR2_RX_BREAK_ZERO_CHAR_OFF_BMSK                                                        0x100
#define HWIO_MSS_UIM2_UART_DM_MR2_RX_BREAK_ZERO_CHAR_OFF_SHFT                                                          0x8
#define HWIO_MSS_UIM2_UART_DM_MR2_LOOPBACK_BMSK                                                                       0x80
#define HWIO_MSS_UIM2_UART_DM_MR2_LOOPBACK_SHFT                                                                        0x7
#define HWIO_MSS_UIM2_UART_DM_MR2_ERROR_MODE_BMSK                                                                     0x40
#define HWIO_MSS_UIM2_UART_DM_MR2_ERROR_MODE_SHFT                                                                      0x6
#define HWIO_MSS_UIM2_UART_DM_MR2_BITS_PER_CHAR_BMSK                                                                  0x30
#define HWIO_MSS_UIM2_UART_DM_MR2_BITS_PER_CHAR_SHFT                                                                   0x4
#define HWIO_MSS_UIM2_UART_DM_MR2_STOP_BIT_LEN_BMSK                                                                    0xc
#define HWIO_MSS_UIM2_UART_DM_MR2_STOP_BIT_LEN_SHFT                                                                    0x2
#define HWIO_MSS_UIM2_UART_DM_MR2_PARITY_MODE_BMSK                                                                     0x3
#define HWIO_MSS_UIM2_UART_DM_MR2_PARITY_MODE_SHFT                                                                     0x0

#define HWIO_MSS_UIM2_UART_DM_CSR_SR_DEPRECATED_ADDR                                                            (MSS_UIM2_UART_DM_REG_BASE      + 0x00000008)
#define HWIO_MSS_UIM2_UART_DM_CSR_SR_DEPRECATED_OFFS                                                            (MSS_UIM2_UART_DM_REG_BASE_OFFS + 0x00000008)
#define HWIO_MSS_UIM2_UART_DM_CSR_SR_DEPRECATED_RMSK                                                            0xffffffff
#define HWIO_MSS_UIM2_UART_DM_CSR_SR_DEPRECATED_IN          \
        in_dword_masked(HWIO_MSS_UIM2_UART_DM_CSR_SR_DEPRECATED_ADDR, HWIO_MSS_UIM2_UART_DM_CSR_SR_DEPRECATED_RMSK)
#define HWIO_MSS_UIM2_UART_DM_CSR_SR_DEPRECATED_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM2_UART_DM_CSR_SR_DEPRECATED_ADDR, m)
#define HWIO_MSS_UIM2_UART_DM_CSR_SR_DEPRECATED_OUT(v)      \
        out_dword(HWIO_MSS_UIM2_UART_DM_CSR_SR_DEPRECATED_ADDR,v)
#define HWIO_MSS_UIM2_UART_DM_CSR_SR_DEPRECATED_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_UIM2_UART_DM_CSR_SR_DEPRECATED_ADDR,m,v,HWIO_MSS_UIM2_UART_DM_CSR_SR_DEPRECATED_IN)
#define HWIO_MSS_UIM2_UART_DM_CSR_SR_DEPRECATED_UART_DM_CSR_SR_DEPRECATED_BMSK                                  0xffffffff
#define HWIO_MSS_UIM2_UART_DM_CSR_SR_DEPRECATED_UART_DM_CSR_SR_DEPRECATED_SHFT                                         0x0

#define HWIO_MSS_UIM2_UART_DM_CR_MISR_DEPRECATED_ADDR                                                           (MSS_UIM2_UART_DM_REG_BASE      + 0x00000010)
#define HWIO_MSS_UIM2_UART_DM_CR_MISR_DEPRECATED_OFFS                                                           (MSS_UIM2_UART_DM_REG_BASE_OFFS + 0x00000010)
#define HWIO_MSS_UIM2_UART_DM_CR_MISR_DEPRECATED_RMSK                                                           0xffffffff
#define HWIO_MSS_UIM2_UART_DM_CR_MISR_DEPRECATED_IN          \
        in_dword_masked(HWIO_MSS_UIM2_UART_DM_CR_MISR_DEPRECATED_ADDR, HWIO_MSS_UIM2_UART_DM_CR_MISR_DEPRECATED_RMSK)
#define HWIO_MSS_UIM2_UART_DM_CR_MISR_DEPRECATED_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM2_UART_DM_CR_MISR_DEPRECATED_ADDR, m)
#define HWIO_MSS_UIM2_UART_DM_CR_MISR_DEPRECATED_OUT(v)      \
        out_dword(HWIO_MSS_UIM2_UART_DM_CR_MISR_DEPRECATED_ADDR,v)
#define HWIO_MSS_UIM2_UART_DM_CR_MISR_DEPRECATED_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_UIM2_UART_DM_CR_MISR_DEPRECATED_ADDR,m,v,HWIO_MSS_UIM2_UART_DM_CR_MISR_DEPRECATED_IN)
#define HWIO_MSS_UIM2_UART_DM_CR_MISR_DEPRECATED_UART_DM_CR_MISR_DEPRECATED_BMSK                                0xffffffff
#define HWIO_MSS_UIM2_UART_DM_CR_MISR_DEPRECATED_UART_DM_CR_MISR_DEPRECATED_SHFT                                       0x0

#define HWIO_MSS_UIM2_UART_DM_IMR_ISR_DEPRECATED_ADDR                                                           (MSS_UIM2_UART_DM_REG_BASE      + 0x00000014)
#define HWIO_MSS_UIM2_UART_DM_IMR_ISR_DEPRECATED_OFFS                                                           (MSS_UIM2_UART_DM_REG_BASE_OFFS + 0x00000014)
#define HWIO_MSS_UIM2_UART_DM_IMR_ISR_DEPRECATED_RMSK                                                           0xffffffff
#define HWIO_MSS_UIM2_UART_DM_IMR_ISR_DEPRECATED_IN          \
        in_dword_masked(HWIO_MSS_UIM2_UART_DM_IMR_ISR_DEPRECATED_ADDR, HWIO_MSS_UIM2_UART_DM_IMR_ISR_DEPRECATED_RMSK)
#define HWIO_MSS_UIM2_UART_DM_IMR_ISR_DEPRECATED_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM2_UART_DM_IMR_ISR_DEPRECATED_ADDR, m)
#define HWIO_MSS_UIM2_UART_DM_IMR_ISR_DEPRECATED_OUT(v)      \
        out_dword(HWIO_MSS_UIM2_UART_DM_IMR_ISR_DEPRECATED_ADDR,v)
#define HWIO_MSS_UIM2_UART_DM_IMR_ISR_DEPRECATED_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_UIM2_UART_DM_IMR_ISR_DEPRECATED_ADDR,m,v,HWIO_MSS_UIM2_UART_DM_IMR_ISR_DEPRECATED_IN)
#define HWIO_MSS_UIM2_UART_DM_IMR_ISR_DEPRECATED_UART_DM_IMR_ISR_DEPRECATED_BMSK                                0xffffffff
#define HWIO_MSS_UIM2_UART_DM_IMR_ISR_DEPRECATED_UART_DM_IMR_ISR_DEPRECATED_SHFT                                       0x0

#define HWIO_MSS_UIM2_UART_DM_IPR_ADDR                                                                          (MSS_UIM2_UART_DM_REG_BASE      + 0x00000018)
#define HWIO_MSS_UIM2_UART_DM_IPR_OFFS                                                                          (MSS_UIM2_UART_DM_REG_BASE_OFFS + 0x00000018)
#define HWIO_MSS_UIM2_UART_DM_IPR_RMSK                                                                          0xffffffdf
#define HWIO_MSS_UIM2_UART_DM_IPR_IN          \
        in_dword_masked(HWIO_MSS_UIM2_UART_DM_IPR_ADDR, HWIO_MSS_UIM2_UART_DM_IPR_RMSK)
#define HWIO_MSS_UIM2_UART_DM_IPR_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM2_UART_DM_IPR_ADDR, m)
#define HWIO_MSS_UIM2_UART_DM_IPR_OUT(v)      \
        out_dword(HWIO_MSS_UIM2_UART_DM_IPR_ADDR,v)
#define HWIO_MSS_UIM2_UART_DM_IPR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_UIM2_UART_DM_IPR_ADDR,m,v,HWIO_MSS_UIM2_UART_DM_IPR_IN)
#define HWIO_MSS_UIM2_UART_DM_IPR_STALE_TIMEOUT_MSB_BMSK                                                        0xffffff80
#define HWIO_MSS_UIM2_UART_DM_IPR_STALE_TIMEOUT_MSB_SHFT                                                               0x7
#define HWIO_MSS_UIM2_UART_DM_IPR_SAMPLE_DATA_BMSK                                                                    0x40
#define HWIO_MSS_UIM2_UART_DM_IPR_SAMPLE_DATA_SHFT                                                                     0x6
#define HWIO_MSS_UIM2_UART_DM_IPR_STALE_TIMEOUT_LSB_BMSK                                                              0x1f
#define HWIO_MSS_UIM2_UART_DM_IPR_STALE_TIMEOUT_LSB_SHFT                                                               0x0

#define HWIO_MSS_UIM2_UART_DM_TFWR_ADDR                                                                         (MSS_UIM2_UART_DM_REG_BASE      + 0x0000001c)
#define HWIO_MSS_UIM2_UART_DM_TFWR_OFFS                                                                         (MSS_UIM2_UART_DM_REG_BASE_OFFS + 0x0000001c)
#define HWIO_MSS_UIM2_UART_DM_TFWR_RMSK                                                                         0xffffffff
#define HWIO_MSS_UIM2_UART_DM_TFWR_IN          \
        in_dword_masked(HWIO_MSS_UIM2_UART_DM_TFWR_ADDR, HWIO_MSS_UIM2_UART_DM_TFWR_RMSK)
#define HWIO_MSS_UIM2_UART_DM_TFWR_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM2_UART_DM_TFWR_ADDR, m)
#define HWIO_MSS_UIM2_UART_DM_TFWR_OUT(v)      \
        out_dword(HWIO_MSS_UIM2_UART_DM_TFWR_ADDR,v)
#define HWIO_MSS_UIM2_UART_DM_TFWR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_UIM2_UART_DM_TFWR_ADDR,m,v,HWIO_MSS_UIM2_UART_DM_TFWR_IN)
#define HWIO_MSS_UIM2_UART_DM_TFWR_TFW_BMSK                                                                     0xffffffff
#define HWIO_MSS_UIM2_UART_DM_TFWR_TFW_SHFT                                                                            0x0

#define HWIO_MSS_UIM2_UART_DM_RFWR_ADDR                                                                         (MSS_UIM2_UART_DM_REG_BASE      + 0x00000020)
#define HWIO_MSS_UIM2_UART_DM_RFWR_OFFS                                                                         (MSS_UIM2_UART_DM_REG_BASE_OFFS + 0x00000020)
#define HWIO_MSS_UIM2_UART_DM_RFWR_RMSK                                                                         0xffffffff
#define HWIO_MSS_UIM2_UART_DM_RFWR_IN          \
        in_dword_masked(HWIO_MSS_UIM2_UART_DM_RFWR_ADDR, HWIO_MSS_UIM2_UART_DM_RFWR_RMSK)
#define HWIO_MSS_UIM2_UART_DM_RFWR_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM2_UART_DM_RFWR_ADDR, m)
#define HWIO_MSS_UIM2_UART_DM_RFWR_OUT(v)      \
        out_dword(HWIO_MSS_UIM2_UART_DM_RFWR_ADDR,v)
#define HWIO_MSS_UIM2_UART_DM_RFWR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_UIM2_UART_DM_RFWR_ADDR,m,v,HWIO_MSS_UIM2_UART_DM_RFWR_IN)
#define HWIO_MSS_UIM2_UART_DM_RFWR_RFW_BMSK                                                                     0xffffffff
#define HWIO_MSS_UIM2_UART_DM_RFWR_RFW_SHFT                                                                            0x0

#define HWIO_MSS_UIM2_UART_DM_HCR_ADDR                                                                          (MSS_UIM2_UART_DM_REG_BASE      + 0x00000024)
#define HWIO_MSS_UIM2_UART_DM_HCR_OFFS                                                                          (MSS_UIM2_UART_DM_REG_BASE_OFFS + 0x00000024)
#define HWIO_MSS_UIM2_UART_DM_HCR_RMSK                                                                                0xff
#define HWIO_MSS_UIM2_UART_DM_HCR_IN          \
        in_dword_masked(HWIO_MSS_UIM2_UART_DM_HCR_ADDR, HWIO_MSS_UIM2_UART_DM_HCR_RMSK)
#define HWIO_MSS_UIM2_UART_DM_HCR_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM2_UART_DM_HCR_ADDR, m)
#define HWIO_MSS_UIM2_UART_DM_HCR_OUT(v)      \
        out_dword(HWIO_MSS_UIM2_UART_DM_HCR_ADDR,v)
#define HWIO_MSS_UIM2_UART_DM_HCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_UIM2_UART_DM_HCR_ADDR,m,v,HWIO_MSS_UIM2_UART_DM_HCR_IN)
#define HWIO_MSS_UIM2_UART_DM_HCR_DATA_BMSK                                                                           0xff
#define HWIO_MSS_UIM2_UART_DM_HCR_DATA_SHFT                                                                            0x0

#define HWIO_MSS_UIM2_UART_DM_DMRX_ADDR                                                                         (MSS_UIM2_UART_DM_REG_BASE      + 0x00000034)
#define HWIO_MSS_UIM2_UART_DM_DMRX_OFFS                                                                         (MSS_UIM2_UART_DM_REG_BASE_OFFS + 0x00000034)
#define HWIO_MSS_UIM2_UART_DM_DMRX_RMSK                                                                          0x1ffffff
#define HWIO_MSS_UIM2_UART_DM_DMRX_IN          \
        in_dword_masked(HWIO_MSS_UIM2_UART_DM_DMRX_ADDR, HWIO_MSS_UIM2_UART_DM_DMRX_RMSK)
#define HWIO_MSS_UIM2_UART_DM_DMRX_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM2_UART_DM_DMRX_ADDR, m)
#define HWIO_MSS_UIM2_UART_DM_DMRX_OUT(v)      \
        out_dword(HWIO_MSS_UIM2_UART_DM_DMRX_ADDR,v)
#define HWIO_MSS_UIM2_UART_DM_DMRX_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_UIM2_UART_DM_DMRX_ADDR,m,v,HWIO_MSS_UIM2_UART_DM_DMRX_IN)
#define HWIO_MSS_UIM2_UART_DM_DMRX_RX_DM_CRCI_CHARS_BMSK                                                         0x1ffffff
#define HWIO_MSS_UIM2_UART_DM_DMRX_RX_DM_CRCI_CHARS_SHFT                                                               0x0

#define HWIO_MSS_UIM2_UART_DM_IRDA_RX_TOTAL_SNAP_DEPRECATED_ADDR                                                (MSS_UIM2_UART_DM_REG_BASE      + 0x00000038)
#define HWIO_MSS_UIM2_UART_DM_IRDA_RX_TOTAL_SNAP_DEPRECATED_OFFS                                                (MSS_UIM2_UART_DM_REG_BASE_OFFS + 0x00000038)
#define HWIO_MSS_UIM2_UART_DM_IRDA_RX_TOTAL_SNAP_DEPRECATED_RMSK                                                0xffffffff
#define HWIO_MSS_UIM2_UART_DM_IRDA_RX_TOTAL_SNAP_DEPRECATED_IN          \
        in_dword_masked(HWIO_MSS_UIM2_UART_DM_IRDA_RX_TOTAL_SNAP_DEPRECATED_ADDR, HWIO_MSS_UIM2_UART_DM_IRDA_RX_TOTAL_SNAP_DEPRECATED_RMSK)
#define HWIO_MSS_UIM2_UART_DM_IRDA_RX_TOTAL_SNAP_DEPRECATED_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM2_UART_DM_IRDA_RX_TOTAL_SNAP_DEPRECATED_ADDR, m)
#define HWIO_MSS_UIM2_UART_DM_IRDA_RX_TOTAL_SNAP_DEPRECATED_OUT(v)      \
        out_dword(HWIO_MSS_UIM2_UART_DM_IRDA_RX_TOTAL_SNAP_DEPRECATED_ADDR,v)
#define HWIO_MSS_UIM2_UART_DM_IRDA_RX_TOTAL_SNAP_DEPRECATED_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_UIM2_UART_DM_IRDA_RX_TOTAL_SNAP_DEPRECATED_ADDR,m,v,HWIO_MSS_UIM2_UART_DM_IRDA_RX_TOTAL_SNAP_DEPRECATED_IN)
#define HWIO_MSS_UIM2_UART_DM_IRDA_RX_TOTAL_SNAP_DEPRECATED_UART_DM_IRDA_RX_TOTAL_SNAP_DEPRECATED_BMSK          0xffffffff
#define HWIO_MSS_UIM2_UART_DM_IRDA_RX_TOTAL_SNAP_DEPRECATED_UART_DM_IRDA_RX_TOTAL_SNAP_DEPRECATED_SHFT                 0x0

#define HWIO_MSS_UIM2_UART_DM_DMEN_ADDR                                                                         (MSS_UIM2_UART_DM_REG_BASE      + 0x0000003c)
#define HWIO_MSS_UIM2_UART_DM_DMEN_OFFS                                                                         (MSS_UIM2_UART_DM_REG_BASE_OFFS + 0x0000003c)
#define HWIO_MSS_UIM2_UART_DM_DMEN_RMSK                                                                               0x3c
#define HWIO_MSS_UIM2_UART_DM_DMEN_IN          \
        in_dword_masked(HWIO_MSS_UIM2_UART_DM_DMEN_ADDR, HWIO_MSS_UIM2_UART_DM_DMEN_RMSK)
#define HWIO_MSS_UIM2_UART_DM_DMEN_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM2_UART_DM_DMEN_ADDR, m)
#define HWIO_MSS_UIM2_UART_DM_DMEN_OUT(v)      \
        out_dword(HWIO_MSS_UIM2_UART_DM_DMEN_ADDR,v)
#define HWIO_MSS_UIM2_UART_DM_DMEN_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_UIM2_UART_DM_DMEN_ADDR,m,v,HWIO_MSS_UIM2_UART_DM_DMEN_IN)
#define HWIO_MSS_UIM2_UART_DM_DMEN_RX_SC_ENABLE_BMSK                                                                  0x20
#define HWIO_MSS_UIM2_UART_DM_DMEN_RX_SC_ENABLE_SHFT                                                                   0x5
#define HWIO_MSS_UIM2_UART_DM_DMEN_TX_SC_ENABLE_BMSK                                                                  0x10
#define HWIO_MSS_UIM2_UART_DM_DMEN_TX_SC_ENABLE_SHFT                                                                   0x4
#define HWIO_MSS_UIM2_UART_DM_DMEN_RX_BAM_ENABLE_BMSK                                                                  0x8
#define HWIO_MSS_UIM2_UART_DM_DMEN_RX_BAM_ENABLE_SHFT                                                                  0x3
#define HWIO_MSS_UIM2_UART_DM_DMEN_TX_BAM_ENABLE_BMSK                                                                  0x4
#define HWIO_MSS_UIM2_UART_DM_DMEN_TX_BAM_ENABLE_SHFT                                                                  0x2

#define HWIO_MSS_UIM2_UART_DM_NO_CHARS_FOR_TX_ADDR                                                              (MSS_UIM2_UART_DM_REG_BASE      + 0x00000040)
#define HWIO_MSS_UIM2_UART_DM_NO_CHARS_FOR_TX_OFFS                                                              (MSS_UIM2_UART_DM_REG_BASE_OFFS + 0x00000040)
#define HWIO_MSS_UIM2_UART_DM_NO_CHARS_FOR_TX_RMSK                                                                0xffffff
#define HWIO_MSS_UIM2_UART_DM_NO_CHARS_FOR_TX_IN          \
        in_dword_masked(HWIO_MSS_UIM2_UART_DM_NO_CHARS_FOR_TX_ADDR, HWIO_MSS_UIM2_UART_DM_NO_CHARS_FOR_TX_RMSK)
#define HWIO_MSS_UIM2_UART_DM_NO_CHARS_FOR_TX_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM2_UART_DM_NO_CHARS_FOR_TX_ADDR, m)
#define HWIO_MSS_UIM2_UART_DM_NO_CHARS_FOR_TX_OUT(v)      \
        out_dword(HWIO_MSS_UIM2_UART_DM_NO_CHARS_FOR_TX_ADDR,v)
#define HWIO_MSS_UIM2_UART_DM_NO_CHARS_FOR_TX_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_UIM2_UART_DM_NO_CHARS_FOR_TX_ADDR,m,v,HWIO_MSS_UIM2_UART_DM_NO_CHARS_FOR_TX_IN)
#define HWIO_MSS_UIM2_UART_DM_NO_CHARS_FOR_TX_TX_TOTAL_TRANS_LEN_BMSK                                             0xffffff
#define HWIO_MSS_UIM2_UART_DM_NO_CHARS_FOR_TX_TX_TOTAL_TRANS_LEN_SHFT                                                  0x0

#define HWIO_MSS_UIM2_UART_DM_BADR_ADDR                                                                         (MSS_UIM2_UART_DM_REG_BASE      + 0x00000044)
#define HWIO_MSS_UIM2_UART_DM_BADR_OFFS                                                                         (MSS_UIM2_UART_DM_REG_BASE_OFFS + 0x00000044)
#define HWIO_MSS_UIM2_UART_DM_BADR_RMSK                                                                         0xfffffffc
#define HWIO_MSS_UIM2_UART_DM_BADR_IN          \
        in_dword_masked(HWIO_MSS_UIM2_UART_DM_BADR_ADDR, HWIO_MSS_UIM2_UART_DM_BADR_RMSK)
#define HWIO_MSS_UIM2_UART_DM_BADR_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM2_UART_DM_BADR_ADDR, m)
#define HWIO_MSS_UIM2_UART_DM_BADR_OUT(v)      \
        out_dword(HWIO_MSS_UIM2_UART_DM_BADR_ADDR,v)
#define HWIO_MSS_UIM2_UART_DM_BADR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_UIM2_UART_DM_BADR_ADDR,m,v,HWIO_MSS_UIM2_UART_DM_BADR_IN)
#define HWIO_MSS_UIM2_UART_DM_BADR_RX_BASE_ADDR_BMSK                                                            0xfffffffc
#define HWIO_MSS_UIM2_UART_DM_BADR_RX_BASE_ADDR_SHFT                                                                   0x2

#define HWIO_MSS_UIM2_UART_DM_TESTSL_ADDR                                                                       (MSS_UIM2_UART_DM_REG_BASE      + 0x00000048)
#define HWIO_MSS_UIM2_UART_DM_TESTSL_OFFS                                                                       (MSS_UIM2_UART_DM_REG_BASE_OFFS + 0x00000048)
#define HWIO_MSS_UIM2_UART_DM_TESTSL_RMSK                                                                             0x1f
#define HWIO_MSS_UIM2_UART_DM_TESTSL_IN          \
        in_dword_masked(HWIO_MSS_UIM2_UART_DM_TESTSL_ADDR, HWIO_MSS_UIM2_UART_DM_TESTSL_RMSK)
#define HWIO_MSS_UIM2_UART_DM_TESTSL_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM2_UART_DM_TESTSL_ADDR, m)
#define HWIO_MSS_UIM2_UART_DM_TESTSL_OUT(v)      \
        out_dword(HWIO_MSS_UIM2_UART_DM_TESTSL_ADDR,v)
#define HWIO_MSS_UIM2_UART_DM_TESTSL_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_UIM2_UART_DM_TESTSL_ADDR,m,v,HWIO_MSS_UIM2_UART_DM_TESTSL_IN)
#define HWIO_MSS_UIM2_UART_DM_TESTSL_TEST_EN_BMSK                                                                     0x10
#define HWIO_MSS_UIM2_UART_DM_TESTSL_TEST_EN_SHFT                                                                      0x4
#define HWIO_MSS_UIM2_UART_DM_TESTSL_TEST_SEL_BMSK                                                                     0xf
#define HWIO_MSS_UIM2_UART_DM_TESTSL_TEST_SEL_SHFT                                                                     0x0

#define HWIO_MSS_UIM2_UART_DM_TXFS_ADDR                                                                         (MSS_UIM2_UART_DM_REG_BASE      + 0x0000004c)
#define HWIO_MSS_UIM2_UART_DM_TXFS_OFFS                                                                         (MSS_UIM2_UART_DM_REG_BASE_OFFS + 0x0000004c)
#define HWIO_MSS_UIM2_UART_DM_TXFS_RMSK                                                                         0xffffffff
#define HWIO_MSS_UIM2_UART_DM_TXFS_IN          \
        in_dword_masked(HWIO_MSS_UIM2_UART_DM_TXFS_ADDR, HWIO_MSS_UIM2_UART_DM_TXFS_RMSK)
#define HWIO_MSS_UIM2_UART_DM_TXFS_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM2_UART_DM_TXFS_ADDR, m)
#define HWIO_MSS_UIM2_UART_DM_TXFS_TX_FIFO_STATE_MSB_BMSK                                                       0xffffc000
#define HWIO_MSS_UIM2_UART_DM_TXFS_TX_FIFO_STATE_MSB_SHFT                                                              0xe
#define HWIO_MSS_UIM2_UART_DM_TXFS_TX_ASYNC_FIFO_STATE_BMSK                                                         0x3c00
#define HWIO_MSS_UIM2_UART_DM_TXFS_TX_ASYNC_FIFO_STATE_SHFT                                                            0xa
#define HWIO_MSS_UIM2_UART_DM_TXFS_TX_BUFFER_STATE_BMSK                                                              0x380
#define HWIO_MSS_UIM2_UART_DM_TXFS_TX_BUFFER_STATE_SHFT                                                                0x7
#define HWIO_MSS_UIM2_UART_DM_TXFS_TX_FIFO_STATE_LSB_BMSK                                                             0x7f
#define HWIO_MSS_UIM2_UART_DM_TXFS_TX_FIFO_STATE_LSB_SHFT                                                              0x0

#define HWIO_MSS_UIM2_UART_DM_RXFS_ADDR                                                                         (MSS_UIM2_UART_DM_REG_BASE      + 0x00000050)
#define HWIO_MSS_UIM2_UART_DM_RXFS_OFFS                                                                         (MSS_UIM2_UART_DM_REG_BASE_OFFS + 0x00000050)
#define HWIO_MSS_UIM2_UART_DM_RXFS_RMSK                                                                         0xffffffff
#define HWIO_MSS_UIM2_UART_DM_RXFS_IN          \
        in_dword_masked(HWIO_MSS_UIM2_UART_DM_RXFS_ADDR, HWIO_MSS_UIM2_UART_DM_RXFS_RMSK)
#define HWIO_MSS_UIM2_UART_DM_RXFS_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM2_UART_DM_RXFS_ADDR, m)
#define HWIO_MSS_UIM2_UART_DM_RXFS_RX_FIFO_STATE_MSB_BMSK                                                       0xffffc000
#define HWIO_MSS_UIM2_UART_DM_RXFS_RX_FIFO_STATE_MSB_SHFT                                                              0xe
#define HWIO_MSS_UIM2_UART_DM_RXFS_RX_ASYNC_FIFO_STATE_BMSK                                                         0x3c00
#define HWIO_MSS_UIM2_UART_DM_RXFS_RX_ASYNC_FIFO_STATE_SHFT                                                            0xa
#define HWIO_MSS_UIM2_UART_DM_RXFS_RX_BUFFER_STATE_BMSK                                                              0x380
#define HWIO_MSS_UIM2_UART_DM_RXFS_RX_BUFFER_STATE_SHFT                                                                0x7
#define HWIO_MSS_UIM2_UART_DM_RXFS_RX_FIFO_STATE_LSB_BMSK                                                             0x7f
#define HWIO_MSS_UIM2_UART_DM_RXFS_RX_FIFO_STATE_LSB_SHFT                                                              0x0

#define HWIO_MSS_UIM2_UART_DM_MISR_MODE_ADDR                                                                    (MSS_UIM2_UART_DM_REG_BASE      + 0x00000060)
#define HWIO_MSS_UIM2_UART_DM_MISR_MODE_OFFS                                                                    (MSS_UIM2_UART_DM_REG_BASE_OFFS + 0x00000060)
#define HWIO_MSS_UIM2_UART_DM_MISR_MODE_RMSK                                                                           0x3
#define HWIO_MSS_UIM2_UART_DM_MISR_MODE_IN          \
        in_dword_masked(HWIO_MSS_UIM2_UART_DM_MISR_MODE_ADDR, HWIO_MSS_UIM2_UART_DM_MISR_MODE_RMSK)
#define HWIO_MSS_UIM2_UART_DM_MISR_MODE_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM2_UART_DM_MISR_MODE_ADDR, m)
#define HWIO_MSS_UIM2_UART_DM_MISR_MODE_OUT(v)      \
        out_dword(HWIO_MSS_UIM2_UART_DM_MISR_MODE_ADDR,v)
#define HWIO_MSS_UIM2_UART_DM_MISR_MODE_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_UIM2_UART_DM_MISR_MODE_ADDR,m,v,HWIO_MSS_UIM2_UART_DM_MISR_MODE_IN)
#define HWIO_MSS_UIM2_UART_DM_MISR_MODE_MODE_BMSK                                                                      0x3
#define HWIO_MSS_UIM2_UART_DM_MISR_MODE_MODE_SHFT                                                                      0x0

#define HWIO_MSS_UIM2_UART_DM_MISR_RESET_ADDR                                                                   (MSS_UIM2_UART_DM_REG_BASE      + 0x00000064)
#define HWIO_MSS_UIM2_UART_DM_MISR_RESET_OFFS                                                                   (MSS_UIM2_UART_DM_REG_BASE_OFFS + 0x00000064)
#define HWIO_MSS_UIM2_UART_DM_MISR_RESET_RMSK                                                                          0x1
#define HWIO_MSS_UIM2_UART_DM_MISR_RESET_OUT(v)      \
        out_dword(HWIO_MSS_UIM2_UART_DM_MISR_RESET_ADDR,v)
#define HWIO_MSS_UIM2_UART_DM_MISR_RESET_RESET_BMSK                                                                    0x1
#define HWIO_MSS_UIM2_UART_DM_MISR_RESET_RESET_SHFT                                                                    0x0

#define HWIO_MSS_UIM2_UART_DM_MISR_EXPORT_ADDR                                                                  (MSS_UIM2_UART_DM_REG_BASE      + 0x00000068)
#define HWIO_MSS_UIM2_UART_DM_MISR_EXPORT_OFFS                                                                  (MSS_UIM2_UART_DM_REG_BASE_OFFS + 0x00000068)
#define HWIO_MSS_UIM2_UART_DM_MISR_EXPORT_RMSK                                                                         0x1
#define HWIO_MSS_UIM2_UART_DM_MISR_EXPORT_IN          \
        in_dword_masked(HWIO_MSS_UIM2_UART_DM_MISR_EXPORT_ADDR, HWIO_MSS_UIM2_UART_DM_MISR_EXPORT_RMSK)
#define HWIO_MSS_UIM2_UART_DM_MISR_EXPORT_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM2_UART_DM_MISR_EXPORT_ADDR, m)
#define HWIO_MSS_UIM2_UART_DM_MISR_EXPORT_OUT(v)      \
        out_dword(HWIO_MSS_UIM2_UART_DM_MISR_EXPORT_ADDR,v)
#define HWIO_MSS_UIM2_UART_DM_MISR_EXPORT_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_UIM2_UART_DM_MISR_EXPORT_ADDR,m,v,HWIO_MSS_UIM2_UART_DM_MISR_EXPORT_IN)
#define HWIO_MSS_UIM2_UART_DM_MISR_EXPORT_EXPORT_BMSK                                                                  0x1
#define HWIO_MSS_UIM2_UART_DM_MISR_EXPORT_EXPORT_SHFT                                                                  0x0

#define HWIO_MSS_UIM2_UART_DM_MISR_VAL_ADDR                                                                     (MSS_UIM2_UART_DM_REG_BASE      + 0x0000006c)
#define HWIO_MSS_UIM2_UART_DM_MISR_VAL_OFFS                                                                     (MSS_UIM2_UART_DM_REG_BASE_OFFS + 0x0000006c)
#define HWIO_MSS_UIM2_UART_DM_MISR_VAL_RMSK                                                                          0x3ff
#define HWIO_MSS_UIM2_UART_DM_MISR_VAL_IN          \
        in_dword_masked(HWIO_MSS_UIM2_UART_DM_MISR_VAL_ADDR, HWIO_MSS_UIM2_UART_DM_MISR_VAL_RMSK)
#define HWIO_MSS_UIM2_UART_DM_MISR_VAL_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM2_UART_DM_MISR_VAL_ADDR, m)
#define HWIO_MSS_UIM2_UART_DM_MISR_VAL_VAL_BMSK                                                                      0x3ff
#define HWIO_MSS_UIM2_UART_DM_MISR_VAL_VAL_SHFT                                                                        0x0

#define HWIO_MSS_UIM2_UART_DM_TF_RF_DEPRECATED_ADDR                                                             (MSS_UIM2_UART_DM_REG_BASE      + 0x00000070)
#define HWIO_MSS_UIM2_UART_DM_TF_RF_DEPRECATED_OFFS                                                             (MSS_UIM2_UART_DM_REG_BASE_OFFS + 0x00000070)
#define HWIO_MSS_UIM2_UART_DM_TF_RF_DEPRECATED_RMSK                                                             0xffffffff
#define HWIO_MSS_UIM2_UART_DM_TF_RF_DEPRECATED_IN          \
        in_dword_masked(HWIO_MSS_UIM2_UART_DM_TF_RF_DEPRECATED_ADDR, HWIO_MSS_UIM2_UART_DM_TF_RF_DEPRECATED_RMSK)
#define HWIO_MSS_UIM2_UART_DM_TF_RF_DEPRECATED_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM2_UART_DM_TF_RF_DEPRECATED_ADDR, m)
#define HWIO_MSS_UIM2_UART_DM_TF_RF_DEPRECATED_OUT(v)      \
        out_dword(HWIO_MSS_UIM2_UART_DM_TF_RF_DEPRECATED_ADDR,v)
#define HWIO_MSS_UIM2_UART_DM_TF_RF_DEPRECATED_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_UIM2_UART_DM_TF_RF_DEPRECATED_ADDR,m,v,HWIO_MSS_UIM2_UART_DM_TF_RF_DEPRECATED_IN)
#define HWIO_MSS_UIM2_UART_DM_TF_RF_DEPRECATED_UART_DM_TF_RF_DEPRECATED_BMSK                                    0xffffffff
#define HWIO_MSS_UIM2_UART_DM_TF_RF_DEPRECATED_UART_DM_TF_RF_DEPRECATED_SHFT                                           0x0

#define HWIO_MSS_UIM2_UART_DM_TF_RF_2_DEPRECATED_ADDR                                                           (MSS_UIM2_UART_DM_REG_BASE      + 0x00000074)
#define HWIO_MSS_UIM2_UART_DM_TF_RF_2_DEPRECATED_OFFS                                                           (MSS_UIM2_UART_DM_REG_BASE_OFFS + 0x00000074)
#define HWIO_MSS_UIM2_UART_DM_TF_RF_2_DEPRECATED_RMSK                                                           0xffffffff
#define HWIO_MSS_UIM2_UART_DM_TF_RF_2_DEPRECATED_IN          \
        in_dword_masked(HWIO_MSS_UIM2_UART_DM_TF_RF_2_DEPRECATED_ADDR, HWIO_MSS_UIM2_UART_DM_TF_RF_2_DEPRECATED_RMSK)
#define HWIO_MSS_UIM2_UART_DM_TF_RF_2_DEPRECATED_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM2_UART_DM_TF_RF_2_DEPRECATED_ADDR, m)
#define HWIO_MSS_UIM2_UART_DM_TF_RF_2_DEPRECATED_OUT(v)      \
        out_dword(HWIO_MSS_UIM2_UART_DM_TF_RF_2_DEPRECATED_ADDR,v)
#define HWIO_MSS_UIM2_UART_DM_TF_RF_2_DEPRECATED_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_UIM2_UART_DM_TF_RF_2_DEPRECATED_ADDR,m,v,HWIO_MSS_UIM2_UART_DM_TF_RF_2_DEPRECATED_IN)
#define HWIO_MSS_UIM2_UART_DM_TF_RF_2_DEPRECATED_UART_DM_TF_RF_2_DEPRECATED_BMSK                                0xffffffff
#define HWIO_MSS_UIM2_UART_DM_TF_RF_2_DEPRECATED_UART_DM_TF_RF_2_DEPRECATED_SHFT                                       0x0

#define HWIO_MSS_UIM2_UART_DM_TF_RF_3_DEPRECATED_ADDR                                                           (MSS_UIM2_UART_DM_REG_BASE      + 0x00000078)
#define HWIO_MSS_UIM2_UART_DM_TF_RF_3_DEPRECATED_OFFS                                                           (MSS_UIM2_UART_DM_REG_BASE_OFFS + 0x00000078)
#define HWIO_MSS_UIM2_UART_DM_TF_RF_3_DEPRECATED_RMSK                                                           0xffffffff
#define HWIO_MSS_UIM2_UART_DM_TF_RF_3_DEPRECATED_IN          \
        in_dword_masked(HWIO_MSS_UIM2_UART_DM_TF_RF_3_DEPRECATED_ADDR, HWIO_MSS_UIM2_UART_DM_TF_RF_3_DEPRECATED_RMSK)
#define HWIO_MSS_UIM2_UART_DM_TF_RF_3_DEPRECATED_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM2_UART_DM_TF_RF_3_DEPRECATED_ADDR, m)
#define HWIO_MSS_UIM2_UART_DM_TF_RF_3_DEPRECATED_OUT(v)      \
        out_dword(HWIO_MSS_UIM2_UART_DM_TF_RF_3_DEPRECATED_ADDR,v)
#define HWIO_MSS_UIM2_UART_DM_TF_RF_3_DEPRECATED_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_UIM2_UART_DM_TF_RF_3_DEPRECATED_ADDR,m,v,HWIO_MSS_UIM2_UART_DM_TF_RF_3_DEPRECATED_IN)
#define HWIO_MSS_UIM2_UART_DM_TF_RF_3_DEPRECATED_UART_DM_TF_RF_3_DEPRECATED_BMSK                                0xffffffff
#define HWIO_MSS_UIM2_UART_DM_TF_RF_3_DEPRECATED_UART_DM_TF_RF_3_DEPRECATED_SHFT                                       0x0

#define HWIO_MSS_UIM2_UART_DM_TF_RF_4_DEPRECATED_ADDR                                                           (MSS_UIM2_UART_DM_REG_BASE      + 0x0000007c)
#define HWIO_MSS_UIM2_UART_DM_TF_RF_4_DEPRECATED_OFFS                                                           (MSS_UIM2_UART_DM_REG_BASE_OFFS + 0x0000007c)
#define HWIO_MSS_UIM2_UART_DM_TF_RF_4_DEPRECATED_RMSK                                                           0xffffffff
#define HWIO_MSS_UIM2_UART_DM_TF_RF_4_DEPRECATED_IN          \
        in_dword_masked(HWIO_MSS_UIM2_UART_DM_TF_RF_4_DEPRECATED_ADDR, HWIO_MSS_UIM2_UART_DM_TF_RF_4_DEPRECATED_RMSK)
#define HWIO_MSS_UIM2_UART_DM_TF_RF_4_DEPRECATED_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM2_UART_DM_TF_RF_4_DEPRECATED_ADDR, m)
#define HWIO_MSS_UIM2_UART_DM_TF_RF_4_DEPRECATED_OUT(v)      \
        out_dword(HWIO_MSS_UIM2_UART_DM_TF_RF_4_DEPRECATED_ADDR,v)
#define HWIO_MSS_UIM2_UART_DM_TF_RF_4_DEPRECATED_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_UIM2_UART_DM_TF_RF_4_DEPRECATED_ADDR,m,v,HWIO_MSS_UIM2_UART_DM_TF_RF_4_DEPRECATED_IN)
#define HWIO_MSS_UIM2_UART_DM_TF_RF_4_DEPRECATED_UART_DM_TF_RF_4_DEPRECATED_BMSK                                0xffffffff
#define HWIO_MSS_UIM2_UART_DM_TF_RF_4_DEPRECATED_UART_DM_TF_RF_4_DEPRECATED_SHFT                                       0x0

#define HWIO_MSS_UIM2_UART_DM_SIM_CFG_ADDR                                                                      (MSS_UIM2_UART_DM_REG_BASE      + 0x00000080)
#define HWIO_MSS_UIM2_UART_DM_SIM_CFG_OFFS                                                                      (MSS_UIM2_UART_DM_REG_BASE_OFFS + 0x00000080)
#define HWIO_MSS_UIM2_UART_DM_SIM_CFG_RMSK                                                                         0x3ffef
#define HWIO_MSS_UIM2_UART_DM_SIM_CFG_IN          \
        in_dword_masked(HWIO_MSS_UIM2_UART_DM_SIM_CFG_ADDR, HWIO_MSS_UIM2_UART_DM_SIM_CFG_RMSK)
#define HWIO_MSS_UIM2_UART_DM_SIM_CFG_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM2_UART_DM_SIM_CFG_ADDR, m)
#define HWIO_MSS_UIM2_UART_DM_SIM_CFG_OUT(v)      \
        out_dword(HWIO_MSS_UIM2_UART_DM_SIM_CFG_ADDR,v)
#define HWIO_MSS_UIM2_UART_DM_SIM_CFG_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_UIM2_UART_DM_SIM_CFG_ADDR,m,v,HWIO_MSS_UIM2_UART_DM_SIM_CFG_IN)
#define HWIO_MSS_UIM2_UART_DM_SIM_CFG_UIM_TX_MODE_BMSK                                                             0x20000
#define HWIO_MSS_UIM2_UART_DM_SIM_CFG_UIM_TX_MODE_SHFT                                                                0x11
#define HWIO_MSS_UIM2_UART_DM_SIM_CFG_UIM_RX_MODE_BMSK                                                             0x10000
#define HWIO_MSS_UIM2_UART_DM_SIM_CFG_UIM_RX_MODE_SHFT                                                                0x10
#define HWIO_MSS_UIM2_UART_DM_SIM_CFG_SIM_STOP_BIT_LEN_BMSK                                                         0xff00
#define HWIO_MSS_UIM2_UART_DM_SIM_CFG_SIM_STOP_BIT_LEN_SHFT                                                            0x8
#define HWIO_MSS_UIM2_UART_DM_SIM_CFG_SIM_CLK_ON_BMSK                                                                 0x80
#define HWIO_MSS_UIM2_UART_DM_SIM_CFG_SIM_CLK_ON_SHFT                                                                  0x7
#define HWIO_MSS_UIM2_UART_DM_SIM_CFG_SIM_CLK_TD8_SEL_BMSK                                                            0x40
#define HWIO_MSS_UIM2_UART_DM_SIM_CFG_SIM_CLK_TD8_SEL_SHFT                                                             0x6
#define HWIO_MSS_UIM2_UART_DM_SIM_CFG_SIM_CLK_STOP_HIGH_BMSK                                                          0x20
#define HWIO_MSS_UIM2_UART_DM_SIM_CFG_SIM_CLK_STOP_HIGH_SHFT                                                           0x5
#define HWIO_MSS_UIM2_UART_DM_SIM_CFG_MASK_RX_BMSK                                                                     0x8
#define HWIO_MSS_UIM2_UART_DM_SIM_CFG_MASK_RX_SHFT                                                                     0x3
#define HWIO_MSS_UIM2_UART_DM_SIM_CFG_SWAP_D_BMSK                                                                      0x4
#define HWIO_MSS_UIM2_UART_DM_SIM_CFG_SWAP_D_SHFT                                                                      0x2
#define HWIO_MSS_UIM2_UART_DM_SIM_CFG_INV_D_BMSK                                                                       0x2
#define HWIO_MSS_UIM2_UART_DM_SIM_CFG_INV_D_SHFT                                                                       0x1
#define HWIO_MSS_UIM2_UART_DM_SIM_CFG_SIM_SEL_BMSK                                                                     0x1
#define HWIO_MSS_UIM2_UART_DM_SIM_CFG_SIM_SEL_SHFT                                                                     0x0

#define HWIO_MSS_UIM2_UART_DM_TEST_WR_ADDR_ADDR                                                                 (MSS_UIM2_UART_DM_REG_BASE      + 0x00000084)
#define HWIO_MSS_UIM2_UART_DM_TEST_WR_ADDR_OFFS                                                                 (MSS_UIM2_UART_DM_REG_BASE_OFFS + 0x00000084)
#define HWIO_MSS_UIM2_UART_DM_TEST_WR_ADDR_RMSK                                                                 0xffffffff
#define HWIO_MSS_UIM2_UART_DM_TEST_WR_ADDR_IN          \
        in_dword_masked(HWIO_MSS_UIM2_UART_DM_TEST_WR_ADDR_ADDR, HWIO_MSS_UIM2_UART_DM_TEST_WR_ADDR_RMSK)
#define HWIO_MSS_UIM2_UART_DM_TEST_WR_ADDR_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM2_UART_DM_TEST_WR_ADDR_ADDR, m)
#define HWIO_MSS_UIM2_UART_DM_TEST_WR_ADDR_OUT(v)      \
        out_dword(HWIO_MSS_UIM2_UART_DM_TEST_WR_ADDR_ADDR,v)
#define HWIO_MSS_UIM2_UART_DM_TEST_WR_ADDR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_UIM2_UART_DM_TEST_WR_ADDR_ADDR,m,v,HWIO_MSS_UIM2_UART_DM_TEST_WR_ADDR_IN)
#define HWIO_MSS_UIM2_UART_DM_TEST_WR_ADDR_TEST_WR_ADDR_BMSK                                                    0xffffffff
#define HWIO_MSS_UIM2_UART_DM_TEST_WR_ADDR_TEST_WR_ADDR_SHFT                                                           0x0

#define HWIO_MSS_UIM2_UART_DM_TEST_WR_DATA_ADDR                                                                 (MSS_UIM2_UART_DM_REG_BASE      + 0x00000088)
#define HWIO_MSS_UIM2_UART_DM_TEST_WR_DATA_OFFS                                                                 (MSS_UIM2_UART_DM_REG_BASE_OFFS + 0x00000088)
#define HWIO_MSS_UIM2_UART_DM_TEST_WR_DATA_RMSK                                                                 0xffffffff
#define HWIO_MSS_UIM2_UART_DM_TEST_WR_DATA_OUT(v)      \
        out_dword(HWIO_MSS_UIM2_UART_DM_TEST_WR_DATA_ADDR,v)
#define HWIO_MSS_UIM2_UART_DM_TEST_WR_DATA_TEST_WR_DATA_BMSK                                                    0xffffffff
#define HWIO_MSS_UIM2_UART_DM_TEST_WR_DATA_TEST_WR_DATA_SHFT                                                           0x0

#define HWIO_MSS_UIM2_UART_DM_TEST_RD_ADDR_ADDR                                                                 (MSS_UIM2_UART_DM_REG_BASE      + 0x0000008c)
#define HWIO_MSS_UIM2_UART_DM_TEST_RD_ADDR_OFFS                                                                 (MSS_UIM2_UART_DM_REG_BASE_OFFS + 0x0000008c)
#define HWIO_MSS_UIM2_UART_DM_TEST_RD_ADDR_RMSK                                                                 0xffffffff
#define HWIO_MSS_UIM2_UART_DM_TEST_RD_ADDR_IN          \
        in_dword_masked(HWIO_MSS_UIM2_UART_DM_TEST_RD_ADDR_ADDR, HWIO_MSS_UIM2_UART_DM_TEST_RD_ADDR_RMSK)
#define HWIO_MSS_UIM2_UART_DM_TEST_RD_ADDR_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM2_UART_DM_TEST_RD_ADDR_ADDR, m)
#define HWIO_MSS_UIM2_UART_DM_TEST_RD_ADDR_OUT(v)      \
        out_dword(HWIO_MSS_UIM2_UART_DM_TEST_RD_ADDR_ADDR,v)
#define HWIO_MSS_UIM2_UART_DM_TEST_RD_ADDR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_UIM2_UART_DM_TEST_RD_ADDR_ADDR,m,v,HWIO_MSS_UIM2_UART_DM_TEST_RD_ADDR_IN)
#define HWIO_MSS_UIM2_UART_DM_TEST_RD_ADDR_TEST_RD_ADDR_BMSK                                                    0xffffffff
#define HWIO_MSS_UIM2_UART_DM_TEST_RD_ADDR_TEST_RD_ADDR_SHFT                                                           0x0

#define HWIO_MSS_UIM2_UART_DM_TEST_RD_DATA_ADDR                                                                 (MSS_UIM2_UART_DM_REG_BASE      + 0x00000090)
#define HWIO_MSS_UIM2_UART_DM_TEST_RD_DATA_OFFS                                                                 (MSS_UIM2_UART_DM_REG_BASE_OFFS + 0x00000090)
#define HWIO_MSS_UIM2_UART_DM_TEST_RD_DATA_RMSK                                                                 0xffffffff
#define HWIO_MSS_UIM2_UART_DM_TEST_RD_DATA_IN          \
        in_dword_masked(HWIO_MSS_UIM2_UART_DM_TEST_RD_DATA_ADDR, HWIO_MSS_UIM2_UART_DM_TEST_RD_DATA_RMSK)
#define HWIO_MSS_UIM2_UART_DM_TEST_RD_DATA_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM2_UART_DM_TEST_RD_DATA_ADDR, m)
#define HWIO_MSS_UIM2_UART_DM_TEST_RD_DATA_TEST_RD_DATA_BMSK                                                    0xffffffff
#define HWIO_MSS_UIM2_UART_DM_TEST_RD_DATA_TEST_RD_DATA_SHFT                                                           0x0

#define HWIO_MSS_UIM2_UART_DM_CSR_ADDR                                                                          (MSS_UIM2_UART_DM_REG_BASE      + 0x000000a0)
#define HWIO_MSS_UIM2_UART_DM_CSR_OFFS                                                                          (MSS_UIM2_UART_DM_REG_BASE_OFFS + 0x000000a0)
#define HWIO_MSS_UIM2_UART_DM_CSR_RMSK                                                                                0xff
#define HWIO_MSS_UIM2_UART_DM_CSR_IN          \
        in_dword_masked(HWIO_MSS_UIM2_UART_DM_CSR_ADDR, HWIO_MSS_UIM2_UART_DM_CSR_RMSK)
#define HWIO_MSS_UIM2_UART_DM_CSR_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM2_UART_DM_CSR_ADDR, m)
#define HWIO_MSS_UIM2_UART_DM_CSR_OUT(v)      \
        out_dword(HWIO_MSS_UIM2_UART_DM_CSR_ADDR,v)
#define HWIO_MSS_UIM2_UART_DM_CSR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_UIM2_UART_DM_CSR_ADDR,m,v,HWIO_MSS_UIM2_UART_DM_CSR_IN)
#define HWIO_MSS_UIM2_UART_DM_CSR_UART_RX_CLK_SEL_BMSK                                                                0xf0
#define HWIO_MSS_UIM2_UART_DM_CSR_UART_RX_CLK_SEL_SHFT                                                                 0x4
#define HWIO_MSS_UIM2_UART_DM_CSR_UART_TX_CLK_SEL_BMSK                                                                 0xf
#define HWIO_MSS_UIM2_UART_DM_CSR_UART_TX_CLK_SEL_SHFT                                                                 0x0

#define HWIO_MSS_UIM2_UART_DM_SR_ADDR                                                                           (MSS_UIM2_UART_DM_REG_BASE      + 0x000000a4)
#define HWIO_MSS_UIM2_UART_DM_SR_OFFS                                                                           (MSS_UIM2_UART_DM_REG_BASE_OFFS + 0x000000a4)
#define HWIO_MSS_UIM2_UART_DM_SR_RMSK                                                                                0xfff
#define HWIO_MSS_UIM2_UART_DM_SR_IN          \
        in_dword_masked(HWIO_MSS_UIM2_UART_DM_SR_ADDR, HWIO_MSS_UIM2_UART_DM_SR_RMSK)
#define HWIO_MSS_UIM2_UART_DM_SR_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM2_UART_DM_SR_ADDR, m)
#define HWIO_MSS_UIM2_UART_DM_SR_TRANS_END_TRIGGER_BMSK                                                              0xc00
#define HWIO_MSS_UIM2_UART_DM_SR_TRANS_END_TRIGGER_SHFT                                                                0xa
#define HWIO_MSS_UIM2_UART_DM_SR_TRANS_ACTIVE_BMSK                                                                   0x200
#define HWIO_MSS_UIM2_UART_DM_SR_TRANS_ACTIVE_SHFT                                                                     0x9
#define HWIO_MSS_UIM2_UART_DM_SR_RX_BREAK_START_LAST_BMSK                                                            0x100
#define HWIO_MSS_UIM2_UART_DM_SR_RX_BREAK_START_LAST_SHFT                                                              0x8
#define HWIO_MSS_UIM2_UART_DM_SR_HUNT_CHAR_BMSK                                                                       0x80
#define HWIO_MSS_UIM2_UART_DM_SR_HUNT_CHAR_SHFT                                                                        0x7
#define HWIO_MSS_UIM2_UART_DM_SR_RX_BREAK_BMSK                                                                        0x40
#define HWIO_MSS_UIM2_UART_DM_SR_RX_BREAK_SHFT                                                                         0x6
#define HWIO_MSS_UIM2_UART_DM_SR_PAR_FRAME_ERR_BMSK                                                                   0x20
#define HWIO_MSS_UIM2_UART_DM_SR_PAR_FRAME_ERR_SHFT                                                                    0x5
#define HWIO_MSS_UIM2_UART_DM_SR_UART_OVERRUN_BMSK                                                                    0x10
#define HWIO_MSS_UIM2_UART_DM_SR_UART_OVERRUN_SHFT                                                                     0x4
#define HWIO_MSS_UIM2_UART_DM_SR_TXEMT_BMSK                                                                            0x8
#define HWIO_MSS_UIM2_UART_DM_SR_TXEMT_SHFT                                                                            0x3
#define HWIO_MSS_UIM2_UART_DM_SR_TXRDY_BMSK                                                                            0x4
#define HWIO_MSS_UIM2_UART_DM_SR_TXRDY_SHFT                                                                            0x2
#define HWIO_MSS_UIM2_UART_DM_SR_RXFULL_BMSK                                                                           0x2
#define HWIO_MSS_UIM2_UART_DM_SR_RXFULL_SHFT                                                                           0x1
#define HWIO_MSS_UIM2_UART_DM_SR_RXRDY_BMSK                                                                            0x1
#define HWIO_MSS_UIM2_UART_DM_SR_RXRDY_SHFT                                                                            0x0

#define HWIO_MSS_UIM2_UART_DM_CR_ADDR                                                                           (MSS_UIM2_UART_DM_REG_BASE      + 0x000000a8)
#define HWIO_MSS_UIM2_UART_DM_CR_OFFS                                                                           (MSS_UIM2_UART_DM_REG_BASE_OFFS + 0x000000a8)
#define HWIO_MSS_UIM2_UART_DM_CR_RMSK                                                                                0xfff
#define HWIO_MSS_UIM2_UART_DM_CR_IN          \
        in_dword_masked(HWIO_MSS_UIM2_UART_DM_CR_ADDR, HWIO_MSS_UIM2_UART_DM_CR_RMSK)
#define HWIO_MSS_UIM2_UART_DM_CR_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM2_UART_DM_CR_ADDR, m)
#define HWIO_MSS_UIM2_UART_DM_CR_OUT(v)      \
        out_dword(HWIO_MSS_UIM2_UART_DM_CR_ADDR,v)
#define HWIO_MSS_UIM2_UART_DM_CR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_UIM2_UART_DM_CR_ADDR,m,v,HWIO_MSS_UIM2_UART_DM_CR_IN)
#define HWIO_MSS_UIM2_UART_DM_CR_CHANNEL_COMMAND_MSB_BMSK                                                            0x800
#define HWIO_MSS_UIM2_UART_DM_CR_CHANNEL_COMMAND_MSB_SHFT                                                              0xb
#define HWIO_MSS_UIM2_UART_DM_CR_GENERAL_COMMAND_BMSK                                                                0x700
#define HWIO_MSS_UIM2_UART_DM_CR_GENERAL_COMMAND_SHFT                                                                  0x8
#define HWIO_MSS_UIM2_UART_DM_CR_CHANNEL_COMMAND_LSB_BMSK                                                             0xf0
#define HWIO_MSS_UIM2_UART_DM_CR_CHANNEL_COMMAND_LSB_SHFT                                                              0x4
#define HWIO_MSS_UIM2_UART_DM_CR_UART_TX_DISABLE_BMSK                                                                  0x8
#define HWIO_MSS_UIM2_UART_DM_CR_UART_TX_DISABLE_SHFT                                                                  0x3
#define HWIO_MSS_UIM2_UART_DM_CR_UART_TX_EN_BMSK                                                                       0x4
#define HWIO_MSS_UIM2_UART_DM_CR_UART_TX_EN_SHFT                                                                       0x2
#define HWIO_MSS_UIM2_UART_DM_CR_UART_RX_DISABLE_BMSK                                                                  0x2
#define HWIO_MSS_UIM2_UART_DM_CR_UART_RX_DISABLE_SHFT                                                                  0x1
#define HWIO_MSS_UIM2_UART_DM_CR_UART_RX_EN_BMSK                                                                       0x1
#define HWIO_MSS_UIM2_UART_DM_CR_UART_RX_EN_SHFT                                                                       0x0

#define HWIO_MSS_UIM2_UART_DM_MISR_ADDR                                                                         (MSS_UIM2_UART_DM_REG_BASE      + 0x000000ac)
#define HWIO_MSS_UIM2_UART_DM_MISR_OFFS                                                                         (MSS_UIM2_UART_DM_REG_BASE_OFFS + 0x000000ac)
#define HWIO_MSS_UIM2_UART_DM_MISR_RMSK                                                                            0x1ffff
#define HWIO_MSS_UIM2_UART_DM_MISR_IN          \
        in_dword_masked(HWIO_MSS_UIM2_UART_DM_MISR_ADDR, HWIO_MSS_UIM2_UART_DM_MISR_RMSK)
#define HWIO_MSS_UIM2_UART_DM_MISR_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM2_UART_DM_MISR_ADDR, m)
#define HWIO_MSS_UIM2_UART_DM_MISR_UART_MISR_BMSK                                                                  0x1ffff
#define HWIO_MSS_UIM2_UART_DM_MISR_UART_MISR_SHFT                                                                      0x0

#define HWIO_MSS_UIM2_UART_DM_IMR_ADDR                                                                          (MSS_UIM2_UART_DM_REG_BASE      + 0x000000b0)
#define HWIO_MSS_UIM2_UART_DM_IMR_OFFS                                                                          (MSS_UIM2_UART_DM_REG_BASE_OFFS + 0x000000b0)
#define HWIO_MSS_UIM2_UART_DM_IMR_RMSK                                                                             0x1ffff
#define HWIO_MSS_UIM2_UART_DM_IMR_IN          \
        in_dword_masked(HWIO_MSS_UIM2_UART_DM_IMR_ADDR, HWIO_MSS_UIM2_UART_DM_IMR_RMSK)
#define HWIO_MSS_UIM2_UART_DM_IMR_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM2_UART_DM_IMR_ADDR, m)
#define HWIO_MSS_UIM2_UART_DM_IMR_OUT(v)      \
        out_dword(HWIO_MSS_UIM2_UART_DM_IMR_ADDR,v)
#define HWIO_MSS_UIM2_UART_DM_IMR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_UIM2_UART_DM_IMR_ADDR,m,v,HWIO_MSS_UIM2_UART_DM_IMR_IN)
#define HWIO_MSS_UIM2_UART_DM_IMR_WWT_IRQ_BMSK                                                                     0x10000
#define HWIO_MSS_UIM2_UART_DM_IMR_WWT_IRQ_SHFT                                                                        0x10
#define HWIO_MSS_UIM2_UART_DM_IMR_TXCOMP_IRQ_BMSK                                                                   0x8000
#define HWIO_MSS_UIM2_UART_DM_IMR_TXCOMP_IRQ_SHFT                                                                      0xf
#define HWIO_MSS_UIM2_UART_DM_IMR_RX_RD_ERROR_IRQ_BMSK                                                              0x4000
#define HWIO_MSS_UIM2_UART_DM_IMR_RX_RD_ERROR_IRQ_SHFT                                                                 0xe
#define HWIO_MSS_UIM2_UART_DM_IMR_TX_WR_ERROR_IRQ_BMSK                                                              0x2000
#define HWIO_MSS_UIM2_UART_DM_IMR_TX_WR_ERROR_IRQ_SHFT                                                                 0xd
#define HWIO_MSS_UIM2_UART_DM_IMR_PAR_FRAME_ERR_IRQ_BMSK                                                            0x1000
#define HWIO_MSS_UIM2_UART_DM_IMR_PAR_FRAME_ERR_IRQ_SHFT                                                               0xc
#define HWIO_MSS_UIM2_UART_DM_IMR_RXBREAK_END_BMSK                                                                   0x800
#define HWIO_MSS_UIM2_UART_DM_IMR_RXBREAK_END_SHFT                                                                     0xb
#define HWIO_MSS_UIM2_UART_DM_IMR_RXBREAK_START_BMSK                                                                 0x400
#define HWIO_MSS_UIM2_UART_DM_IMR_RXBREAK_START_SHFT                                                                   0xa
#define HWIO_MSS_UIM2_UART_DM_IMR_TX_DONE_BMSK                                                                       0x200
#define HWIO_MSS_UIM2_UART_DM_IMR_TX_DONE_SHFT                                                                         0x9
#define HWIO_MSS_UIM2_UART_DM_IMR_TX_ERROR_BMSK                                                                      0x100
#define HWIO_MSS_UIM2_UART_DM_IMR_TX_ERROR_SHFT                                                                        0x8
#define HWIO_MSS_UIM2_UART_DM_IMR_TX_READY_BMSK                                                                       0x80
#define HWIO_MSS_UIM2_UART_DM_IMR_TX_READY_SHFT                                                                        0x7
#define HWIO_MSS_UIM2_UART_DM_IMR_CURRENT_CTS_BMSK                                                                    0x40
#define HWIO_MSS_UIM2_UART_DM_IMR_CURRENT_CTS_SHFT                                                                     0x6
#define HWIO_MSS_UIM2_UART_DM_IMR_DELTA_CTS_BMSK                                                                      0x20
#define HWIO_MSS_UIM2_UART_DM_IMR_DELTA_CTS_SHFT                                                                       0x5
#define HWIO_MSS_UIM2_UART_DM_IMR_RXLEV_BMSK                                                                          0x10
#define HWIO_MSS_UIM2_UART_DM_IMR_RXLEV_SHFT                                                                           0x4
#define HWIO_MSS_UIM2_UART_DM_IMR_RXSTALE_BMSK                                                                         0x8
#define HWIO_MSS_UIM2_UART_DM_IMR_RXSTALE_SHFT                                                                         0x3
#define HWIO_MSS_UIM2_UART_DM_IMR_RXBREAK_CHANGE_BMSK                                                                  0x4
#define HWIO_MSS_UIM2_UART_DM_IMR_RXBREAK_CHANGE_SHFT                                                                  0x2
#define HWIO_MSS_UIM2_UART_DM_IMR_RXHUNT_BMSK                                                                          0x2
#define HWIO_MSS_UIM2_UART_DM_IMR_RXHUNT_SHFT                                                                          0x1
#define HWIO_MSS_UIM2_UART_DM_IMR_TXLEV_BMSK                                                                           0x1
#define HWIO_MSS_UIM2_UART_DM_IMR_TXLEV_SHFT                                                                           0x0

#define HWIO_MSS_UIM2_UART_DM_ISR_ADDR                                                                          (MSS_UIM2_UART_DM_REG_BASE      + 0x000000b4)
#define HWIO_MSS_UIM2_UART_DM_ISR_OFFS                                                                          (MSS_UIM2_UART_DM_REG_BASE_OFFS + 0x000000b4)
#define HWIO_MSS_UIM2_UART_DM_ISR_RMSK                                                                             0x1ffff
#define HWIO_MSS_UIM2_UART_DM_ISR_IN          \
        in_dword_masked(HWIO_MSS_UIM2_UART_DM_ISR_ADDR, HWIO_MSS_UIM2_UART_DM_ISR_RMSK)
#define HWIO_MSS_UIM2_UART_DM_ISR_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM2_UART_DM_ISR_ADDR, m)
#define HWIO_MSS_UIM2_UART_DM_ISR_WWT_IRQ_BMSK                                                                     0x10000
#define HWIO_MSS_UIM2_UART_DM_ISR_WWT_IRQ_SHFT                                                                        0x10
#define HWIO_MSS_UIM2_UART_DM_ISR_TXCOMP_IRQ_BMSK                                                                   0x8000
#define HWIO_MSS_UIM2_UART_DM_ISR_TXCOMP_IRQ_SHFT                                                                      0xf
#define HWIO_MSS_UIM2_UART_DM_ISR_RX_RD_ERROR_IRQ_BMSK                                                              0x4000
#define HWIO_MSS_UIM2_UART_DM_ISR_RX_RD_ERROR_IRQ_SHFT                                                                 0xe
#define HWIO_MSS_UIM2_UART_DM_ISR_TX_WR_ERROR_IRQ_BMSK                                                              0x2000
#define HWIO_MSS_UIM2_UART_DM_ISR_TX_WR_ERROR_IRQ_SHFT                                                                 0xd
#define HWIO_MSS_UIM2_UART_DM_ISR_PAR_FRAME_ERR_IRQ_BMSK                                                            0x1000
#define HWIO_MSS_UIM2_UART_DM_ISR_PAR_FRAME_ERR_IRQ_SHFT                                                               0xc
#define HWIO_MSS_UIM2_UART_DM_ISR_RXBREAK_END_BMSK                                                                   0x800
#define HWIO_MSS_UIM2_UART_DM_ISR_RXBREAK_END_SHFT                                                                     0xb
#define HWIO_MSS_UIM2_UART_DM_ISR_RXBREAK_START_BMSK                                                                 0x400
#define HWIO_MSS_UIM2_UART_DM_ISR_RXBREAK_START_SHFT                                                                   0xa
#define HWIO_MSS_UIM2_UART_DM_ISR_TX_DONE_BMSK                                                                       0x200
#define HWIO_MSS_UIM2_UART_DM_ISR_TX_DONE_SHFT                                                                         0x9
#define HWIO_MSS_UIM2_UART_DM_ISR_TX_ERROR_BMSK                                                                      0x100
#define HWIO_MSS_UIM2_UART_DM_ISR_TX_ERROR_SHFT                                                                        0x8
#define HWIO_MSS_UIM2_UART_DM_ISR_TX_READY_BMSK                                                                       0x80
#define HWIO_MSS_UIM2_UART_DM_ISR_TX_READY_SHFT                                                                        0x7
#define HWIO_MSS_UIM2_UART_DM_ISR_CURRENT_CTS_BMSK                                                                    0x40
#define HWIO_MSS_UIM2_UART_DM_ISR_CURRENT_CTS_SHFT                                                                     0x6
#define HWIO_MSS_UIM2_UART_DM_ISR_DELTA_CTS_BMSK                                                                      0x20
#define HWIO_MSS_UIM2_UART_DM_ISR_DELTA_CTS_SHFT                                                                       0x5
#define HWIO_MSS_UIM2_UART_DM_ISR_RXLEV_BMSK                                                                          0x10
#define HWIO_MSS_UIM2_UART_DM_ISR_RXLEV_SHFT                                                                           0x4
#define HWIO_MSS_UIM2_UART_DM_ISR_RXSTALE_BMSK                                                                         0x8
#define HWIO_MSS_UIM2_UART_DM_ISR_RXSTALE_SHFT                                                                         0x3
#define HWIO_MSS_UIM2_UART_DM_ISR_RXBREAK_CHANGE_BMSK                                                                  0x4
#define HWIO_MSS_UIM2_UART_DM_ISR_RXBREAK_CHANGE_SHFT                                                                  0x2
#define HWIO_MSS_UIM2_UART_DM_ISR_RXHUNT_BMSK                                                                          0x2
#define HWIO_MSS_UIM2_UART_DM_ISR_RXHUNT_SHFT                                                                          0x1
#define HWIO_MSS_UIM2_UART_DM_ISR_TXLEV_BMSK                                                                           0x1
#define HWIO_MSS_UIM2_UART_DM_ISR_TXLEV_SHFT                                                                           0x0

#define HWIO_MSS_UIM2_UART_DM_IRDA_ADDR                                                                         (MSS_UIM2_UART_DM_REG_BASE      + 0x000000b8)
#define HWIO_MSS_UIM2_UART_DM_IRDA_OFFS                                                                         (MSS_UIM2_UART_DM_REG_BASE_OFFS + 0x000000b8)
#define HWIO_MSS_UIM2_UART_DM_IRDA_RMSK                                                                               0x1f
#define HWIO_MSS_UIM2_UART_DM_IRDA_IN          \
        in_dword_masked(HWIO_MSS_UIM2_UART_DM_IRDA_ADDR, HWIO_MSS_UIM2_UART_DM_IRDA_RMSK)
#define HWIO_MSS_UIM2_UART_DM_IRDA_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM2_UART_DM_IRDA_ADDR, m)
#define HWIO_MSS_UIM2_UART_DM_IRDA_OUT(v)      \
        out_dword(HWIO_MSS_UIM2_UART_DM_IRDA_ADDR,v)
#define HWIO_MSS_UIM2_UART_DM_IRDA_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_UIM2_UART_DM_IRDA_ADDR,m,v,HWIO_MSS_UIM2_UART_DM_IRDA_IN)
#define HWIO_MSS_UIM2_UART_DM_IRDA_MEDIUM_RATE_EN_BMSK                                                                0x10
#define HWIO_MSS_UIM2_UART_DM_IRDA_MEDIUM_RATE_EN_SHFT                                                                 0x4
#define HWIO_MSS_UIM2_UART_DM_IRDA_IRDA_LOOPBACK_BMSK                                                                  0x8
#define HWIO_MSS_UIM2_UART_DM_IRDA_IRDA_LOOPBACK_SHFT                                                                  0x3
#define HWIO_MSS_UIM2_UART_DM_IRDA_INVERT_IRDA_TX_BMSK                                                                 0x4
#define HWIO_MSS_UIM2_UART_DM_IRDA_INVERT_IRDA_TX_SHFT                                                                 0x2
#define HWIO_MSS_UIM2_UART_DM_IRDA_INVERT_IRDA_RX_BMSK                                                                 0x2
#define HWIO_MSS_UIM2_UART_DM_IRDA_INVERT_IRDA_RX_SHFT                                                                 0x1
#define HWIO_MSS_UIM2_UART_DM_IRDA_IRDA_EN_BMSK                                                                        0x1
#define HWIO_MSS_UIM2_UART_DM_IRDA_IRDA_EN_SHFT                                                                        0x0

#define HWIO_MSS_UIM2_UART_DM_RX_TOTAL_SNAP_ADDR                                                                (MSS_UIM2_UART_DM_REG_BASE      + 0x000000bc)
#define HWIO_MSS_UIM2_UART_DM_RX_TOTAL_SNAP_OFFS                                                                (MSS_UIM2_UART_DM_REG_BASE_OFFS + 0x000000bc)
#define HWIO_MSS_UIM2_UART_DM_RX_TOTAL_SNAP_RMSK                                                                  0xffffff
#define HWIO_MSS_UIM2_UART_DM_RX_TOTAL_SNAP_IN          \
        in_dword_masked(HWIO_MSS_UIM2_UART_DM_RX_TOTAL_SNAP_ADDR, HWIO_MSS_UIM2_UART_DM_RX_TOTAL_SNAP_RMSK)
#define HWIO_MSS_UIM2_UART_DM_RX_TOTAL_SNAP_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM2_UART_DM_RX_TOTAL_SNAP_ADDR, m)
#define HWIO_MSS_UIM2_UART_DM_RX_TOTAL_SNAP_RX_TOTAL_BYTES_BMSK                                                   0xffffff
#define HWIO_MSS_UIM2_UART_DM_RX_TOTAL_SNAP_RX_TOTAL_BYTES_SHFT                                                        0x0

#define HWIO_MSS_UIM2_UART_DM_WWT_TIMEOUT_ADDR                                                                  (MSS_UIM2_UART_DM_REG_BASE      + 0x000000c0)
#define HWIO_MSS_UIM2_UART_DM_WWT_TIMEOUT_OFFS                                                                  (MSS_UIM2_UART_DM_REG_BASE_OFFS + 0x000000c0)
#define HWIO_MSS_UIM2_UART_DM_WWT_TIMEOUT_RMSK                                                                   0x1ffffff
#define HWIO_MSS_UIM2_UART_DM_WWT_TIMEOUT_IN          \
        in_dword_masked(HWIO_MSS_UIM2_UART_DM_WWT_TIMEOUT_ADDR, HWIO_MSS_UIM2_UART_DM_WWT_TIMEOUT_RMSK)
#define HWIO_MSS_UIM2_UART_DM_WWT_TIMEOUT_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM2_UART_DM_WWT_TIMEOUT_ADDR, m)
#define HWIO_MSS_UIM2_UART_DM_WWT_TIMEOUT_OUT(v)      \
        out_dword(HWIO_MSS_UIM2_UART_DM_WWT_TIMEOUT_ADDR,v)
#define HWIO_MSS_UIM2_UART_DM_WWT_TIMEOUT_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_UIM2_UART_DM_WWT_TIMEOUT_ADDR,m,v,HWIO_MSS_UIM2_UART_DM_WWT_TIMEOUT_IN)
#define HWIO_MSS_UIM2_UART_DM_WWT_TIMEOUT_WWT_TIMEOUT_BMSK                                                       0x1ffffff
#define HWIO_MSS_UIM2_UART_DM_WWT_TIMEOUT_WWT_TIMEOUT_SHFT                                                             0x0

#define HWIO_MSS_UIM2_UART_DM_CLK_CTRL_ADDR                                                                     (MSS_UIM2_UART_DM_REG_BASE      + 0x000000c4)
#define HWIO_MSS_UIM2_UART_DM_CLK_CTRL_OFFS                                                                     (MSS_UIM2_UART_DM_REG_BASE_OFFS + 0x000000c4)
#define HWIO_MSS_UIM2_UART_DM_CLK_CTRL_RMSK                                                                       0xf06731
#define HWIO_MSS_UIM2_UART_DM_CLK_CTRL_IN          \
        in_dword_masked(HWIO_MSS_UIM2_UART_DM_CLK_CTRL_ADDR, HWIO_MSS_UIM2_UART_DM_CLK_CTRL_RMSK)
#define HWIO_MSS_UIM2_UART_DM_CLK_CTRL_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM2_UART_DM_CLK_CTRL_ADDR, m)
#define HWIO_MSS_UIM2_UART_DM_CLK_CTRL_OUT(v)      \
        out_dword(HWIO_MSS_UIM2_UART_DM_CLK_CTRL_ADDR,v)
#define HWIO_MSS_UIM2_UART_DM_CLK_CTRL_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_UIM2_UART_DM_CLK_CTRL_ADDR,m,v,HWIO_MSS_UIM2_UART_DM_CLK_CTRL_IN)
#define HWIO_MSS_UIM2_UART_DM_CLK_CTRL_UART_IRDA_CLK_CGC_OPEN_BMSK                                                0x800000
#define HWIO_MSS_UIM2_UART_DM_CLK_CTRL_UART_IRDA_CLK_CGC_OPEN_SHFT                                                    0x17
#define HWIO_MSS_UIM2_UART_DM_CLK_CTRL_UART_SIM_CLK_CGC_OPEN_BMSK                                                 0x400000
#define HWIO_MSS_UIM2_UART_DM_CLK_CTRL_UART_SIM_CLK_CGC_OPEN_SHFT                                                     0x16
#define HWIO_MSS_UIM2_UART_DM_CLK_CTRL_UART_RX_CLK_CGC_OPEN_BMSK                                                  0x200000
#define HWIO_MSS_UIM2_UART_DM_CLK_CTRL_UART_RX_CLK_CGC_OPEN_SHFT                                                      0x15
#define HWIO_MSS_UIM2_UART_DM_CLK_CTRL_UART_TX_CLK_CGC_OPEN_BMSK                                                  0x100000
#define HWIO_MSS_UIM2_UART_DM_CLK_CTRL_UART_TX_CLK_CGC_OPEN_SHFT                                                      0x14
#define HWIO_MSS_UIM2_UART_DM_CLK_CTRL_AHB_RX_BAM_CLK_CGC_OPEN_BMSK                                                 0x4000
#define HWIO_MSS_UIM2_UART_DM_CLK_CTRL_AHB_RX_BAM_CLK_CGC_OPEN_SHFT                                                    0xe
#define HWIO_MSS_UIM2_UART_DM_CLK_CTRL_AHB_TX_BAM_CLK_CGC_OPEN_BMSK                                                 0x2000
#define HWIO_MSS_UIM2_UART_DM_CLK_CTRL_AHB_TX_BAM_CLK_CGC_OPEN_SHFT                                                    0xd
#define HWIO_MSS_UIM2_UART_DM_CLK_CTRL_AHB_RX_CLK_CGC_OPEN_BMSK                                                      0x400
#define HWIO_MSS_UIM2_UART_DM_CLK_CTRL_AHB_RX_CLK_CGC_OPEN_SHFT                                                        0xa
#define HWIO_MSS_UIM2_UART_DM_CLK_CTRL_AHB_TX_CLK_CGC_OPEN_BMSK                                                      0x200
#define HWIO_MSS_UIM2_UART_DM_CLK_CTRL_AHB_TX_CLK_CGC_OPEN_SHFT                                                        0x9
#define HWIO_MSS_UIM2_UART_DM_CLK_CTRL_AHB_WR_CLK_CGC_OPEN_BMSK                                                      0x100
#define HWIO_MSS_UIM2_UART_DM_CLK_CTRL_AHB_WR_CLK_CGC_OPEN_SHFT                                                        0x8
#define HWIO_MSS_UIM2_UART_DM_CLK_CTRL_RX_ENABLE_CGC_OPT_BMSK                                                         0x20
#define HWIO_MSS_UIM2_UART_DM_CLK_CTRL_RX_ENABLE_CGC_OPT_SHFT                                                          0x5
#define HWIO_MSS_UIM2_UART_DM_CLK_CTRL_TX_ENABLE_CGC_OPT_BMSK                                                         0x10
#define HWIO_MSS_UIM2_UART_DM_CLK_CTRL_TX_ENABLE_CGC_OPT_SHFT                                                          0x4
#define HWIO_MSS_UIM2_UART_DM_CLK_CTRL_AHB_CLK_CGC_CLOSE_BMSK                                                          0x1
#define HWIO_MSS_UIM2_UART_DM_CLK_CTRL_AHB_CLK_CGC_CLOSE_SHFT                                                          0x0

#define HWIO_MSS_UIM2_UART_DM_BCR_ADDR                                                                          (MSS_UIM2_UART_DM_REG_BASE      + 0x000000c8)
#define HWIO_MSS_UIM2_UART_DM_BCR_OFFS                                                                          (MSS_UIM2_UART_DM_REG_BASE_OFFS + 0x000000c8)
#define HWIO_MSS_UIM2_UART_DM_BCR_RMSK                                                                                0x37
#define HWIO_MSS_UIM2_UART_DM_BCR_IN          \
        in_dword_masked(HWIO_MSS_UIM2_UART_DM_BCR_ADDR, HWIO_MSS_UIM2_UART_DM_BCR_RMSK)
#define HWIO_MSS_UIM2_UART_DM_BCR_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM2_UART_DM_BCR_ADDR, m)
#define HWIO_MSS_UIM2_UART_DM_BCR_OUT(v)      \
        out_dword(HWIO_MSS_UIM2_UART_DM_BCR_ADDR,v)
#define HWIO_MSS_UIM2_UART_DM_BCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_UIM2_UART_DM_BCR_ADDR,m,v,HWIO_MSS_UIM2_UART_DM_BCR_IN)
#define HWIO_MSS_UIM2_UART_DM_BCR_RX_DMRX_1BYTE_RES_EN_BMSK                                                           0x20
#define HWIO_MSS_UIM2_UART_DM_BCR_RX_DMRX_1BYTE_RES_EN_SHFT                                                            0x5
#define HWIO_MSS_UIM2_UART_DM_BCR_RX_STALE_IRQ_DMRX_EQUAL_BMSK                                                        0x10
#define HWIO_MSS_UIM2_UART_DM_BCR_RX_STALE_IRQ_DMRX_EQUAL_SHFT                                                         0x4
#define HWIO_MSS_UIM2_UART_DM_BCR_RX_DMRX_LOW_EN_BMSK                                                                  0x4
#define HWIO_MSS_UIM2_UART_DM_BCR_RX_DMRX_LOW_EN_SHFT                                                                  0x2
#define HWIO_MSS_UIM2_UART_DM_BCR_STALE_IRQ_EMPTY_BMSK                                                                 0x2
#define HWIO_MSS_UIM2_UART_DM_BCR_STALE_IRQ_EMPTY_SHFT                                                                 0x1
#define HWIO_MSS_UIM2_UART_DM_BCR_TX_BREAK_DISABLE_BMSK                                                                0x1
#define HWIO_MSS_UIM2_UART_DM_BCR_TX_BREAK_DISABLE_SHFT                                                                0x0

#define HWIO_MSS_UIM2_UART_DM_RX_TRANS_CTRL_ADDR                                                                (MSS_UIM2_UART_DM_REG_BASE      + 0x000000cc)
#define HWIO_MSS_UIM2_UART_DM_RX_TRANS_CTRL_OFFS                                                                (MSS_UIM2_UART_DM_REG_BASE_OFFS + 0x000000cc)
#define HWIO_MSS_UIM2_UART_DM_RX_TRANS_CTRL_RMSK                                                                       0x7
#define HWIO_MSS_UIM2_UART_DM_RX_TRANS_CTRL_IN          \
        in_dword_masked(HWIO_MSS_UIM2_UART_DM_RX_TRANS_CTRL_ADDR, HWIO_MSS_UIM2_UART_DM_RX_TRANS_CTRL_RMSK)
#define HWIO_MSS_UIM2_UART_DM_RX_TRANS_CTRL_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM2_UART_DM_RX_TRANS_CTRL_ADDR, m)
#define HWIO_MSS_UIM2_UART_DM_RX_TRANS_CTRL_OUT(v)      \
        out_dword(HWIO_MSS_UIM2_UART_DM_RX_TRANS_CTRL_ADDR,v)
#define HWIO_MSS_UIM2_UART_DM_RX_TRANS_CTRL_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_UIM2_UART_DM_RX_TRANS_CTRL_ADDR,m,v,HWIO_MSS_UIM2_UART_DM_RX_TRANS_CTRL_IN)
#define HWIO_MSS_UIM2_UART_DM_RX_TRANS_CTRL_RX_DMRX_CYCLIC_EN_BMSK                                                     0x4
#define HWIO_MSS_UIM2_UART_DM_RX_TRANS_CTRL_RX_DMRX_CYCLIC_EN_SHFT                                                     0x2
#define HWIO_MSS_UIM2_UART_DM_RX_TRANS_CTRL_RX_TRANS_AUTO_RE_ACTIVATE_BMSK                                             0x2
#define HWIO_MSS_UIM2_UART_DM_RX_TRANS_CTRL_RX_TRANS_AUTO_RE_ACTIVATE_SHFT                                             0x1
#define HWIO_MSS_UIM2_UART_DM_RX_TRANS_CTRL_RX_STALE_AUTO_RE_EN_BMSK                                                   0x1
#define HWIO_MSS_UIM2_UART_DM_RX_TRANS_CTRL_RX_STALE_AUTO_RE_EN_SHFT                                                   0x0

#define HWIO_MSS_UIM2_UART_DM_DMRX_DBG_ADDR                                                                     (MSS_UIM2_UART_DM_REG_BASE      + 0x000000d0)
#define HWIO_MSS_UIM2_UART_DM_DMRX_DBG_OFFS                                                                     (MSS_UIM2_UART_DM_REG_BASE_OFFS + 0x000000d0)
#define HWIO_MSS_UIM2_UART_DM_DMRX_DBG_RMSK                                                                      0x1ffffff
#define HWIO_MSS_UIM2_UART_DM_DMRX_DBG_IN          \
        in_dword_masked(HWIO_MSS_UIM2_UART_DM_DMRX_DBG_ADDR, HWIO_MSS_UIM2_UART_DM_DMRX_DBG_RMSK)
#define HWIO_MSS_UIM2_UART_DM_DMRX_DBG_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM2_UART_DM_DMRX_DBG_ADDR, m)
#define HWIO_MSS_UIM2_UART_DM_DMRX_DBG_UART_DM_DMRX_VAL_BMSK                                                     0x1ffffff
#define HWIO_MSS_UIM2_UART_DM_DMRX_DBG_UART_DM_DMRX_VAL_SHFT                                                           0x0

#define HWIO_MSS_UIM2_UART_DM_FSM_STATUS_ADDR                                                                   (MSS_UIM2_UART_DM_REG_BASE      + 0x000000d4)
#define HWIO_MSS_UIM2_UART_DM_FSM_STATUS_OFFS                                                                   (MSS_UIM2_UART_DM_REG_BASE_OFFS + 0x000000d4)
#define HWIO_MSS_UIM2_UART_DM_FSM_STATUS_RMSK                                                                   0x37377771
#define HWIO_MSS_UIM2_UART_DM_FSM_STATUS_IN          \
        in_dword_masked(HWIO_MSS_UIM2_UART_DM_FSM_STATUS_ADDR, HWIO_MSS_UIM2_UART_DM_FSM_STATUS_RMSK)
#define HWIO_MSS_UIM2_UART_DM_FSM_STATUS_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM2_UART_DM_FSM_STATUS_ADDR, m)
#define HWIO_MSS_UIM2_UART_DM_FSM_STATUS_TX_COMP_FSM_BMSK                                                       0x30000000
#define HWIO_MSS_UIM2_UART_DM_FSM_STATUS_TX_COMP_FSM_SHFT                                                             0x1c
#define HWIO_MSS_UIM2_UART_DM_FSM_STATUS_RX_PACK_FSM_BMSK                                                        0x7000000
#define HWIO_MSS_UIM2_UART_DM_FSM_STATUS_RX_PACK_FSM_SHFT                                                             0x18
#define HWIO_MSS_UIM2_UART_DM_FSM_STATUS_RX_TRANS_FSM_BMSK                                                        0x300000
#define HWIO_MSS_UIM2_UART_DM_FSM_STATUS_RX_TRANS_FSM_SHFT                                                            0x14
#define HWIO_MSS_UIM2_UART_DM_FSM_STATUS_TX_TRANS_FSM_BMSK                                                         0x70000
#define HWIO_MSS_UIM2_UART_DM_FSM_STATUS_TX_TRANS_FSM_SHFT                                                            0x10
#define HWIO_MSS_UIM2_UART_DM_FSM_STATUS_RX_PRO_TRANS_END_FSM_BMSK                                                  0x7000
#define HWIO_MSS_UIM2_UART_DM_FSM_STATUS_RX_PRO_TRANS_END_FSM_SHFT                                                     0xc
#define HWIO_MSS_UIM2_UART_DM_FSM_STATUS_RX_PRO_ACTIVE_FSM_BMSK                                                      0x700
#define HWIO_MSS_UIM2_UART_DM_FSM_STATUS_RX_PRO_ACTIVE_FSM_SHFT                                                        0x8
#define HWIO_MSS_UIM2_UART_DM_FSM_STATUS_TX_CON_TRANS_END_FSM_BMSK                                                    0x70
#define HWIO_MSS_UIM2_UART_DM_FSM_STATUS_TX_CON_TRANS_END_FSM_SHFT                                                     0x4
#define HWIO_MSS_UIM2_UART_DM_FSM_STATUS_RX_TRANSFER_ACTIVE_BMSK                                                       0x1
#define HWIO_MSS_UIM2_UART_DM_FSM_STATUS_RX_TRANSFER_ACTIVE_SHFT                                                       0x0

#define HWIO_MSS_UIM2_UART_DM_HW_VERSION_ADDR                                                                   (MSS_UIM2_UART_DM_REG_BASE      + 0x000000d8)
#define HWIO_MSS_UIM2_UART_DM_HW_VERSION_OFFS                                                                   (MSS_UIM2_UART_DM_REG_BASE_OFFS + 0x000000d8)
#define HWIO_MSS_UIM2_UART_DM_HW_VERSION_RMSK                                                                   0xffffffff
#define HWIO_MSS_UIM2_UART_DM_HW_VERSION_IN          \
        in_dword_masked(HWIO_MSS_UIM2_UART_DM_HW_VERSION_ADDR, HWIO_MSS_UIM2_UART_DM_HW_VERSION_RMSK)
#define HWIO_MSS_UIM2_UART_DM_HW_VERSION_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM2_UART_DM_HW_VERSION_ADDR, m)
#define HWIO_MSS_UIM2_UART_DM_HW_VERSION_HW_VERSION_MAJOR_BMSK                                                  0xf0000000
#define HWIO_MSS_UIM2_UART_DM_HW_VERSION_HW_VERSION_MAJOR_SHFT                                                        0x1c
#define HWIO_MSS_UIM2_UART_DM_HW_VERSION_HW_VERSION_MINOR_BMSK                                                   0xfff0000
#define HWIO_MSS_UIM2_UART_DM_HW_VERSION_HW_VERSION_MINOR_SHFT                                                        0x10
#define HWIO_MSS_UIM2_UART_DM_HW_VERSION_HW_VERSION_STEP_BMSK                                                       0xffff
#define HWIO_MSS_UIM2_UART_DM_HW_VERSION_HW_VERSION_STEP_SHFT                                                          0x0

#define HWIO_MSS_UIM2_UART_DM_GENERICS_ADDR                                                                     (MSS_UIM2_UART_DM_REG_BASE      + 0x000000dc)
#define HWIO_MSS_UIM2_UART_DM_GENERICS_OFFS                                                                     (MSS_UIM2_UART_DM_REG_BASE_OFFS + 0x000000dc)
#define HWIO_MSS_UIM2_UART_DM_GENERICS_RMSK                                                                           0xff
#define HWIO_MSS_UIM2_UART_DM_GENERICS_IN          \
        in_dword_masked(HWIO_MSS_UIM2_UART_DM_GENERICS_ADDR, HWIO_MSS_UIM2_UART_DM_GENERICS_RMSK)
#define HWIO_MSS_UIM2_UART_DM_GENERICS_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM2_UART_DM_GENERICS_ADDR, m)
#define HWIO_MSS_UIM2_UART_DM_GENERICS_GENERIC_BAM_IFC_BMSK                                                           0x80
#define HWIO_MSS_UIM2_UART_DM_GENERICS_GENERIC_BAM_IFC_SHFT                                                            0x7
#define HWIO_MSS_UIM2_UART_DM_GENERICS_GENERIC_DM_IFC_BMSK                                                            0x40
#define HWIO_MSS_UIM2_UART_DM_GENERICS_GENERIC_DM_IFC_SHFT                                                             0x6
#define HWIO_MSS_UIM2_UART_DM_GENERICS_GENERIC_IRDA_IFC_BMSK                                                          0x20
#define HWIO_MSS_UIM2_UART_DM_GENERICS_GENERIC_IRDA_IFC_SHFT                                                           0x5
#define HWIO_MSS_UIM2_UART_DM_GENERICS_GENERIC_SIM_GLUE_BMSK                                                          0x10
#define HWIO_MSS_UIM2_UART_DM_GENERICS_GENERIC_SIM_GLUE_SHFT                                                           0x4
#define HWIO_MSS_UIM2_UART_DM_GENERICS_GENERIC_RAM_ADDR_WIDTH_BMSK                                                     0xf
#define HWIO_MSS_UIM2_UART_DM_GENERICS_GENERIC_RAM_ADDR_WIDTH_SHFT                                                     0x0

#define HWIO_MSS_UIM2_UART_DM_TF_ADDR                                                                           (MSS_UIM2_UART_DM_REG_BASE      + 0x00000100)
#define HWIO_MSS_UIM2_UART_DM_TF_OFFS                                                                           (MSS_UIM2_UART_DM_REG_BASE_OFFS + 0x00000100)
#define HWIO_MSS_UIM2_UART_DM_TF_RMSK                                                                           0xffffffff
#define HWIO_MSS_UIM2_UART_DM_TF_OUT(v)      \
        out_dword(HWIO_MSS_UIM2_UART_DM_TF_ADDR,v)
#define HWIO_MSS_UIM2_UART_DM_TF_UART_TF_BMSK                                                                   0xffffffff
#define HWIO_MSS_UIM2_UART_DM_TF_UART_TF_SHFT                                                                          0x0

#define HWIO_MSS_UIM2_UART_DM_TF_2_ADDR                                                                         (MSS_UIM2_UART_DM_REG_BASE      + 0x00000104)
#define HWIO_MSS_UIM2_UART_DM_TF_2_OFFS                                                                         (MSS_UIM2_UART_DM_REG_BASE_OFFS + 0x00000104)
#define HWIO_MSS_UIM2_UART_DM_TF_2_RMSK                                                                         0xffffffff
#define HWIO_MSS_UIM2_UART_DM_TF_2_OUT(v)      \
        out_dword(HWIO_MSS_UIM2_UART_DM_TF_2_ADDR,v)
#define HWIO_MSS_UIM2_UART_DM_TF_2_UART_TF_BMSK                                                                 0xffffffff
#define HWIO_MSS_UIM2_UART_DM_TF_2_UART_TF_SHFT                                                                        0x0

#define HWIO_MSS_UIM2_UART_DM_TF_3_ADDR                                                                         (MSS_UIM2_UART_DM_REG_BASE      + 0x00000108)
#define HWIO_MSS_UIM2_UART_DM_TF_3_OFFS                                                                         (MSS_UIM2_UART_DM_REG_BASE_OFFS + 0x00000108)
#define HWIO_MSS_UIM2_UART_DM_TF_3_RMSK                                                                         0xffffffff
#define HWIO_MSS_UIM2_UART_DM_TF_3_OUT(v)      \
        out_dword(HWIO_MSS_UIM2_UART_DM_TF_3_ADDR,v)
#define HWIO_MSS_UIM2_UART_DM_TF_3_UART_TF_BMSK                                                                 0xffffffff
#define HWIO_MSS_UIM2_UART_DM_TF_3_UART_TF_SHFT                                                                        0x0

#define HWIO_MSS_UIM2_UART_DM_TF_4_ADDR                                                                         (MSS_UIM2_UART_DM_REG_BASE      + 0x0000010c)
#define HWIO_MSS_UIM2_UART_DM_TF_4_OFFS                                                                         (MSS_UIM2_UART_DM_REG_BASE_OFFS + 0x0000010c)
#define HWIO_MSS_UIM2_UART_DM_TF_4_RMSK                                                                         0xffffffff
#define HWIO_MSS_UIM2_UART_DM_TF_4_OUT(v)      \
        out_dword(HWIO_MSS_UIM2_UART_DM_TF_4_ADDR,v)
#define HWIO_MSS_UIM2_UART_DM_TF_4_UART_TF_BMSK                                                                 0xffffffff
#define HWIO_MSS_UIM2_UART_DM_TF_4_UART_TF_SHFT                                                                        0x0

#define HWIO_MSS_UIM2_UART_DM_TF_5_ADDR                                                                         (MSS_UIM2_UART_DM_REG_BASE      + 0x00000110)
#define HWIO_MSS_UIM2_UART_DM_TF_5_OFFS                                                                         (MSS_UIM2_UART_DM_REG_BASE_OFFS + 0x00000110)
#define HWIO_MSS_UIM2_UART_DM_TF_5_RMSK                                                                         0xffffffff
#define HWIO_MSS_UIM2_UART_DM_TF_5_OUT(v)      \
        out_dword(HWIO_MSS_UIM2_UART_DM_TF_5_ADDR,v)
#define HWIO_MSS_UIM2_UART_DM_TF_5_UART_TF_BMSK                                                                 0xffffffff
#define HWIO_MSS_UIM2_UART_DM_TF_5_UART_TF_SHFT                                                                        0x0

#define HWIO_MSS_UIM2_UART_DM_TF_6_ADDR                                                                         (MSS_UIM2_UART_DM_REG_BASE      + 0x00000114)
#define HWIO_MSS_UIM2_UART_DM_TF_6_OFFS                                                                         (MSS_UIM2_UART_DM_REG_BASE_OFFS + 0x00000114)
#define HWIO_MSS_UIM2_UART_DM_TF_6_RMSK                                                                         0xffffffff
#define HWIO_MSS_UIM2_UART_DM_TF_6_OUT(v)      \
        out_dword(HWIO_MSS_UIM2_UART_DM_TF_6_ADDR,v)
#define HWIO_MSS_UIM2_UART_DM_TF_6_UART_TF_BMSK                                                                 0xffffffff
#define HWIO_MSS_UIM2_UART_DM_TF_6_UART_TF_SHFT                                                                        0x0

#define HWIO_MSS_UIM2_UART_DM_TF_7_ADDR                                                                         (MSS_UIM2_UART_DM_REG_BASE      + 0x00000118)
#define HWIO_MSS_UIM2_UART_DM_TF_7_OFFS                                                                         (MSS_UIM2_UART_DM_REG_BASE_OFFS + 0x00000118)
#define HWIO_MSS_UIM2_UART_DM_TF_7_RMSK                                                                         0xffffffff
#define HWIO_MSS_UIM2_UART_DM_TF_7_OUT(v)      \
        out_dword(HWIO_MSS_UIM2_UART_DM_TF_7_ADDR,v)
#define HWIO_MSS_UIM2_UART_DM_TF_7_UART_TF_BMSK                                                                 0xffffffff
#define HWIO_MSS_UIM2_UART_DM_TF_7_UART_TF_SHFT                                                                        0x0

#define HWIO_MSS_UIM2_UART_DM_TF_8_ADDR                                                                         (MSS_UIM2_UART_DM_REG_BASE      + 0x0000011c)
#define HWIO_MSS_UIM2_UART_DM_TF_8_OFFS                                                                         (MSS_UIM2_UART_DM_REG_BASE_OFFS + 0x0000011c)
#define HWIO_MSS_UIM2_UART_DM_TF_8_RMSK                                                                         0xffffffff
#define HWIO_MSS_UIM2_UART_DM_TF_8_OUT(v)      \
        out_dword(HWIO_MSS_UIM2_UART_DM_TF_8_ADDR,v)
#define HWIO_MSS_UIM2_UART_DM_TF_8_UART_TF_BMSK                                                                 0xffffffff
#define HWIO_MSS_UIM2_UART_DM_TF_8_UART_TF_SHFT                                                                        0x0

#define HWIO_MSS_UIM2_UART_DM_TF_9_ADDR                                                                         (MSS_UIM2_UART_DM_REG_BASE      + 0x00000120)
#define HWIO_MSS_UIM2_UART_DM_TF_9_OFFS                                                                         (MSS_UIM2_UART_DM_REG_BASE_OFFS + 0x00000120)
#define HWIO_MSS_UIM2_UART_DM_TF_9_RMSK                                                                         0xffffffff
#define HWIO_MSS_UIM2_UART_DM_TF_9_OUT(v)      \
        out_dword(HWIO_MSS_UIM2_UART_DM_TF_9_ADDR,v)
#define HWIO_MSS_UIM2_UART_DM_TF_9_UART_TF_BMSK                                                                 0xffffffff
#define HWIO_MSS_UIM2_UART_DM_TF_9_UART_TF_SHFT                                                                        0x0

#define HWIO_MSS_UIM2_UART_DM_TF_10_ADDR                                                                        (MSS_UIM2_UART_DM_REG_BASE      + 0x00000124)
#define HWIO_MSS_UIM2_UART_DM_TF_10_OFFS                                                                        (MSS_UIM2_UART_DM_REG_BASE_OFFS + 0x00000124)
#define HWIO_MSS_UIM2_UART_DM_TF_10_RMSK                                                                        0xffffffff
#define HWIO_MSS_UIM2_UART_DM_TF_10_OUT(v)      \
        out_dword(HWIO_MSS_UIM2_UART_DM_TF_10_ADDR,v)
#define HWIO_MSS_UIM2_UART_DM_TF_10_UART_TF_BMSK                                                                0xffffffff
#define HWIO_MSS_UIM2_UART_DM_TF_10_UART_TF_SHFT                                                                       0x0

#define HWIO_MSS_UIM2_UART_DM_TF_11_ADDR                                                                        (MSS_UIM2_UART_DM_REG_BASE      + 0x00000128)
#define HWIO_MSS_UIM2_UART_DM_TF_11_OFFS                                                                        (MSS_UIM2_UART_DM_REG_BASE_OFFS + 0x00000128)
#define HWIO_MSS_UIM2_UART_DM_TF_11_RMSK                                                                        0xffffffff
#define HWIO_MSS_UIM2_UART_DM_TF_11_OUT(v)      \
        out_dword(HWIO_MSS_UIM2_UART_DM_TF_11_ADDR,v)
#define HWIO_MSS_UIM2_UART_DM_TF_11_UART_TF_BMSK                                                                0xffffffff
#define HWIO_MSS_UIM2_UART_DM_TF_11_UART_TF_SHFT                                                                       0x0

#define HWIO_MSS_UIM2_UART_DM_TF_12_ADDR                                                                        (MSS_UIM2_UART_DM_REG_BASE      + 0x0000012c)
#define HWIO_MSS_UIM2_UART_DM_TF_12_OFFS                                                                        (MSS_UIM2_UART_DM_REG_BASE_OFFS + 0x0000012c)
#define HWIO_MSS_UIM2_UART_DM_TF_12_RMSK                                                                        0xffffffff
#define HWIO_MSS_UIM2_UART_DM_TF_12_OUT(v)      \
        out_dword(HWIO_MSS_UIM2_UART_DM_TF_12_ADDR,v)
#define HWIO_MSS_UIM2_UART_DM_TF_12_UART_TF_BMSK                                                                0xffffffff
#define HWIO_MSS_UIM2_UART_DM_TF_12_UART_TF_SHFT                                                                       0x0

#define HWIO_MSS_UIM2_UART_DM_TF_13_ADDR                                                                        (MSS_UIM2_UART_DM_REG_BASE      + 0x00000130)
#define HWIO_MSS_UIM2_UART_DM_TF_13_OFFS                                                                        (MSS_UIM2_UART_DM_REG_BASE_OFFS + 0x00000130)
#define HWIO_MSS_UIM2_UART_DM_TF_13_RMSK                                                                        0xffffffff
#define HWIO_MSS_UIM2_UART_DM_TF_13_OUT(v)      \
        out_dword(HWIO_MSS_UIM2_UART_DM_TF_13_ADDR,v)
#define HWIO_MSS_UIM2_UART_DM_TF_13_UART_TF_BMSK                                                                0xffffffff
#define HWIO_MSS_UIM2_UART_DM_TF_13_UART_TF_SHFT                                                                       0x0

#define HWIO_MSS_UIM2_UART_DM_TF_14_ADDR                                                                        (MSS_UIM2_UART_DM_REG_BASE      + 0x00000134)
#define HWIO_MSS_UIM2_UART_DM_TF_14_OFFS                                                                        (MSS_UIM2_UART_DM_REG_BASE_OFFS + 0x00000134)
#define HWIO_MSS_UIM2_UART_DM_TF_14_RMSK                                                                        0xffffffff
#define HWIO_MSS_UIM2_UART_DM_TF_14_OUT(v)      \
        out_dword(HWIO_MSS_UIM2_UART_DM_TF_14_ADDR,v)
#define HWIO_MSS_UIM2_UART_DM_TF_14_UART_TF_BMSK                                                                0xffffffff
#define HWIO_MSS_UIM2_UART_DM_TF_14_UART_TF_SHFT                                                                       0x0

#define HWIO_MSS_UIM2_UART_DM_TF_15_ADDR                                                                        (MSS_UIM2_UART_DM_REG_BASE      + 0x00000138)
#define HWIO_MSS_UIM2_UART_DM_TF_15_OFFS                                                                        (MSS_UIM2_UART_DM_REG_BASE_OFFS + 0x00000138)
#define HWIO_MSS_UIM2_UART_DM_TF_15_RMSK                                                                        0xffffffff
#define HWIO_MSS_UIM2_UART_DM_TF_15_OUT(v)      \
        out_dword(HWIO_MSS_UIM2_UART_DM_TF_15_ADDR,v)
#define HWIO_MSS_UIM2_UART_DM_TF_15_UART_TF_BMSK                                                                0xffffffff
#define HWIO_MSS_UIM2_UART_DM_TF_15_UART_TF_SHFT                                                                       0x0

#define HWIO_MSS_UIM2_UART_DM_TF_16_ADDR                                                                        (MSS_UIM2_UART_DM_REG_BASE      + 0x0000013c)
#define HWIO_MSS_UIM2_UART_DM_TF_16_OFFS                                                                        (MSS_UIM2_UART_DM_REG_BASE_OFFS + 0x0000013c)
#define HWIO_MSS_UIM2_UART_DM_TF_16_RMSK                                                                        0xffffffff
#define HWIO_MSS_UIM2_UART_DM_TF_16_OUT(v)      \
        out_dword(HWIO_MSS_UIM2_UART_DM_TF_16_ADDR,v)
#define HWIO_MSS_UIM2_UART_DM_TF_16_UART_TF_BMSK                                                                0xffffffff
#define HWIO_MSS_UIM2_UART_DM_TF_16_UART_TF_SHFT                                                                       0x0

#define HWIO_MSS_UIM2_UART_DM_RF_ADDR                                                                           (MSS_UIM2_UART_DM_REG_BASE      + 0x00000140)
#define HWIO_MSS_UIM2_UART_DM_RF_OFFS                                                                           (MSS_UIM2_UART_DM_REG_BASE_OFFS + 0x00000140)
#define HWIO_MSS_UIM2_UART_DM_RF_RMSK                                                                           0xffffffff
#define HWIO_MSS_UIM2_UART_DM_RF_IN          \
        in_dword_masked(HWIO_MSS_UIM2_UART_DM_RF_ADDR, HWIO_MSS_UIM2_UART_DM_RF_RMSK)
#define HWIO_MSS_UIM2_UART_DM_RF_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM2_UART_DM_RF_ADDR, m)
#define HWIO_MSS_UIM2_UART_DM_RF_UART_RF_BMSK                                                                   0xffffffff
#define HWIO_MSS_UIM2_UART_DM_RF_UART_RF_SHFT                                                                          0x0

#define HWIO_MSS_UIM2_UART_DM_RF_2_ADDR                                                                         (MSS_UIM2_UART_DM_REG_BASE      + 0x00000144)
#define HWIO_MSS_UIM2_UART_DM_RF_2_OFFS                                                                         (MSS_UIM2_UART_DM_REG_BASE_OFFS + 0x00000144)
#define HWIO_MSS_UIM2_UART_DM_RF_2_RMSK                                                                         0xffffffff
#define HWIO_MSS_UIM2_UART_DM_RF_2_IN          \
        in_dword_masked(HWIO_MSS_UIM2_UART_DM_RF_2_ADDR, HWIO_MSS_UIM2_UART_DM_RF_2_RMSK)
#define HWIO_MSS_UIM2_UART_DM_RF_2_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM2_UART_DM_RF_2_ADDR, m)
#define HWIO_MSS_UIM2_UART_DM_RF_2_UART_RF_BMSK                                                                 0xffffffff
#define HWIO_MSS_UIM2_UART_DM_RF_2_UART_RF_SHFT                                                                        0x0

#define HWIO_MSS_UIM2_UART_DM_RF_3_ADDR                                                                         (MSS_UIM2_UART_DM_REG_BASE      + 0x00000148)
#define HWIO_MSS_UIM2_UART_DM_RF_3_OFFS                                                                         (MSS_UIM2_UART_DM_REG_BASE_OFFS + 0x00000148)
#define HWIO_MSS_UIM2_UART_DM_RF_3_RMSK                                                                         0xffffffff
#define HWIO_MSS_UIM2_UART_DM_RF_3_IN          \
        in_dword_masked(HWIO_MSS_UIM2_UART_DM_RF_3_ADDR, HWIO_MSS_UIM2_UART_DM_RF_3_RMSK)
#define HWIO_MSS_UIM2_UART_DM_RF_3_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM2_UART_DM_RF_3_ADDR, m)
#define HWIO_MSS_UIM2_UART_DM_RF_3_UART_RF_BMSK                                                                 0xffffffff
#define HWIO_MSS_UIM2_UART_DM_RF_3_UART_RF_SHFT                                                                        0x0

#define HWIO_MSS_UIM2_UART_DM_RF_4_ADDR                                                                         (MSS_UIM2_UART_DM_REG_BASE      + 0x0000014c)
#define HWIO_MSS_UIM2_UART_DM_RF_4_OFFS                                                                         (MSS_UIM2_UART_DM_REG_BASE_OFFS + 0x0000014c)
#define HWIO_MSS_UIM2_UART_DM_RF_4_RMSK                                                                         0xffffffff
#define HWIO_MSS_UIM2_UART_DM_RF_4_IN          \
        in_dword_masked(HWIO_MSS_UIM2_UART_DM_RF_4_ADDR, HWIO_MSS_UIM2_UART_DM_RF_4_RMSK)
#define HWIO_MSS_UIM2_UART_DM_RF_4_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM2_UART_DM_RF_4_ADDR, m)
#define HWIO_MSS_UIM2_UART_DM_RF_4_UART_RF_BMSK                                                                 0xffffffff
#define HWIO_MSS_UIM2_UART_DM_RF_4_UART_RF_SHFT                                                                        0x0

#define HWIO_MSS_UIM2_UART_DM_RF_5_ADDR                                                                         (MSS_UIM2_UART_DM_REG_BASE      + 0x00000150)
#define HWIO_MSS_UIM2_UART_DM_RF_5_OFFS                                                                         (MSS_UIM2_UART_DM_REG_BASE_OFFS + 0x00000150)
#define HWIO_MSS_UIM2_UART_DM_RF_5_RMSK                                                                         0xffffffff
#define HWIO_MSS_UIM2_UART_DM_RF_5_IN          \
        in_dword_masked(HWIO_MSS_UIM2_UART_DM_RF_5_ADDR, HWIO_MSS_UIM2_UART_DM_RF_5_RMSK)
#define HWIO_MSS_UIM2_UART_DM_RF_5_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM2_UART_DM_RF_5_ADDR, m)
#define HWIO_MSS_UIM2_UART_DM_RF_5_UART_RF_BMSK                                                                 0xffffffff
#define HWIO_MSS_UIM2_UART_DM_RF_5_UART_RF_SHFT                                                                        0x0

#define HWIO_MSS_UIM2_UART_DM_RF_6_ADDR                                                                         (MSS_UIM2_UART_DM_REG_BASE      + 0x00000154)
#define HWIO_MSS_UIM2_UART_DM_RF_6_OFFS                                                                         (MSS_UIM2_UART_DM_REG_BASE_OFFS + 0x00000154)
#define HWIO_MSS_UIM2_UART_DM_RF_6_RMSK                                                                         0xffffffff
#define HWIO_MSS_UIM2_UART_DM_RF_6_IN          \
        in_dword_masked(HWIO_MSS_UIM2_UART_DM_RF_6_ADDR, HWIO_MSS_UIM2_UART_DM_RF_6_RMSK)
#define HWIO_MSS_UIM2_UART_DM_RF_6_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM2_UART_DM_RF_6_ADDR, m)
#define HWIO_MSS_UIM2_UART_DM_RF_6_UART_RF_BMSK                                                                 0xffffffff
#define HWIO_MSS_UIM2_UART_DM_RF_6_UART_RF_SHFT                                                                        0x0

#define HWIO_MSS_UIM2_UART_DM_RF_7_ADDR                                                                         (MSS_UIM2_UART_DM_REG_BASE      + 0x00000158)
#define HWIO_MSS_UIM2_UART_DM_RF_7_OFFS                                                                         (MSS_UIM2_UART_DM_REG_BASE_OFFS + 0x00000158)
#define HWIO_MSS_UIM2_UART_DM_RF_7_RMSK                                                                         0xffffffff
#define HWIO_MSS_UIM2_UART_DM_RF_7_IN          \
        in_dword_masked(HWIO_MSS_UIM2_UART_DM_RF_7_ADDR, HWIO_MSS_UIM2_UART_DM_RF_7_RMSK)
#define HWIO_MSS_UIM2_UART_DM_RF_7_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM2_UART_DM_RF_7_ADDR, m)
#define HWIO_MSS_UIM2_UART_DM_RF_7_UART_RF_BMSK                                                                 0xffffffff
#define HWIO_MSS_UIM2_UART_DM_RF_7_UART_RF_SHFT                                                                        0x0

#define HWIO_MSS_UIM2_UART_DM_RF_8_ADDR                                                                         (MSS_UIM2_UART_DM_REG_BASE      + 0x0000015c)
#define HWIO_MSS_UIM2_UART_DM_RF_8_OFFS                                                                         (MSS_UIM2_UART_DM_REG_BASE_OFFS + 0x0000015c)
#define HWIO_MSS_UIM2_UART_DM_RF_8_RMSK                                                                         0xffffffff
#define HWIO_MSS_UIM2_UART_DM_RF_8_IN          \
        in_dword_masked(HWIO_MSS_UIM2_UART_DM_RF_8_ADDR, HWIO_MSS_UIM2_UART_DM_RF_8_RMSK)
#define HWIO_MSS_UIM2_UART_DM_RF_8_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM2_UART_DM_RF_8_ADDR, m)
#define HWIO_MSS_UIM2_UART_DM_RF_8_UART_RF_BMSK                                                                 0xffffffff
#define HWIO_MSS_UIM2_UART_DM_RF_8_UART_RF_SHFT                                                                        0x0

#define HWIO_MSS_UIM2_UART_DM_RF_9_ADDR                                                                         (MSS_UIM2_UART_DM_REG_BASE      + 0x00000160)
#define HWIO_MSS_UIM2_UART_DM_RF_9_OFFS                                                                         (MSS_UIM2_UART_DM_REG_BASE_OFFS + 0x00000160)
#define HWIO_MSS_UIM2_UART_DM_RF_9_RMSK                                                                         0xffffffff
#define HWIO_MSS_UIM2_UART_DM_RF_9_IN          \
        in_dword_masked(HWIO_MSS_UIM2_UART_DM_RF_9_ADDR, HWIO_MSS_UIM2_UART_DM_RF_9_RMSK)
#define HWIO_MSS_UIM2_UART_DM_RF_9_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM2_UART_DM_RF_9_ADDR, m)
#define HWIO_MSS_UIM2_UART_DM_RF_9_UART_RF_BMSK                                                                 0xffffffff
#define HWIO_MSS_UIM2_UART_DM_RF_9_UART_RF_SHFT                                                                        0x0

#define HWIO_MSS_UIM2_UART_DM_RF_10_ADDR                                                                        (MSS_UIM2_UART_DM_REG_BASE      + 0x00000164)
#define HWIO_MSS_UIM2_UART_DM_RF_10_OFFS                                                                        (MSS_UIM2_UART_DM_REG_BASE_OFFS + 0x00000164)
#define HWIO_MSS_UIM2_UART_DM_RF_10_RMSK                                                                        0xffffffff
#define HWIO_MSS_UIM2_UART_DM_RF_10_IN          \
        in_dword_masked(HWIO_MSS_UIM2_UART_DM_RF_10_ADDR, HWIO_MSS_UIM2_UART_DM_RF_10_RMSK)
#define HWIO_MSS_UIM2_UART_DM_RF_10_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM2_UART_DM_RF_10_ADDR, m)
#define HWIO_MSS_UIM2_UART_DM_RF_10_UART_RF_BMSK                                                                0xffffffff
#define HWIO_MSS_UIM2_UART_DM_RF_10_UART_RF_SHFT                                                                       0x0

#define HWIO_MSS_UIM2_UART_DM_RF_11_ADDR                                                                        (MSS_UIM2_UART_DM_REG_BASE      + 0x00000168)
#define HWIO_MSS_UIM2_UART_DM_RF_11_OFFS                                                                        (MSS_UIM2_UART_DM_REG_BASE_OFFS + 0x00000168)
#define HWIO_MSS_UIM2_UART_DM_RF_11_RMSK                                                                        0xffffffff
#define HWIO_MSS_UIM2_UART_DM_RF_11_IN          \
        in_dword_masked(HWIO_MSS_UIM2_UART_DM_RF_11_ADDR, HWIO_MSS_UIM2_UART_DM_RF_11_RMSK)
#define HWIO_MSS_UIM2_UART_DM_RF_11_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM2_UART_DM_RF_11_ADDR, m)
#define HWIO_MSS_UIM2_UART_DM_RF_11_UART_RF_BMSK                                                                0xffffffff
#define HWIO_MSS_UIM2_UART_DM_RF_11_UART_RF_SHFT                                                                       0x0

#define HWIO_MSS_UIM2_UART_DM_RF_12_ADDR                                                                        (MSS_UIM2_UART_DM_REG_BASE      + 0x0000016c)
#define HWIO_MSS_UIM2_UART_DM_RF_12_OFFS                                                                        (MSS_UIM2_UART_DM_REG_BASE_OFFS + 0x0000016c)
#define HWIO_MSS_UIM2_UART_DM_RF_12_RMSK                                                                        0xffffffff
#define HWIO_MSS_UIM2_UART_DM_RF_12_IN          \
        in_dword_masked(HWIO_MSS_UIM2_UART_DM_RF_12_ADDR, HWIO_MSS_UIM2_UART_DM_RF_12_RMSK)
#define HWIO_MSS_UIM2_UART_DM_RF_12_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM2_UART_DM_RF_12_ADDR, m)
#define HWIO_MSS_UIM2_UART_DM_RF_12_UART_RF_BMSK                                                                0xffffffff
#define HWIO_MSS_UIM2_UART_DM_RF_12_UART_RF_SHFT                                                                       0x0

#define HWIO_MSS_UIM2_UART_DM_RF_13_ADDR                                                                        (MSS_UIM2_UART_DM_REG_BASE      + 0x00000170)
#define HWIO_MSS_UIM2_UART_DM_RF_13_OFFS                                                                        (MSS_UIM2_UART_DM_REG_BASE_OFFS + 0x00000170)
#define HWIO_MSS_UIM2_UART_DM_RF_13_RMSK                                                                        0xffffffff
#define HWIO_MSS_UIM2_UART_DM_RF_13_IN          \
        in_dword_masked(HWIO_MSS_UIM2_UART_DM_RF_13_ADDR, HWIO_MSS_UIM2_UART_DM_RF_13_RMSK)
#define HWIO_MSS_UIM2_UART_DM_RF_13_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM2_UART_DM_RF_13_ADDR, m)
#define HWIO_MSS_UIM2_UART_DM_RF_13_UART_RF_BMSK                                                                0xffffffff
#define HWIO_MSS_UIM2_UART_DM_RF_13_UART_RF_SHFT                                                                       0x0

#define HWIO_MSS_UIM2_UART_DM_RF_14_ADDR                                                                        (MSS_UIM2_UART_DM_REG_BASE      + 0x00000174)
#define HWIO_MSS_UIM2_UART_DM_RF_14_OFFS                                                                        (MSS_UIM2_UART_DM_REG_BASE_OFFS + 0x00000174)
#define HWIO_MSS_UIM2_UART_DM_RF_14_RMSK                                                                        0xffffffff
#define HWIO_MSS_UIM2_UART_DM_RF_14_IN          \
        in_dword_masked(HWIO_MSS_UIM2_UART_DM_RF_14_ADDR, HWIO_MSS_UIM2_UART_DM_RF_14_RMSK)
#define HWIO_MSS_UIM2_UART_DM_RF_14_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM2_UART_DM_RF_14_ADDR, m)
#define HWIO_MSS_UIM2_UART_DM_RF_14_UART_RF_BMSK                                                                0xffffffff
#define HWIO_MSS_UIM2_UART_DM_RF_14_UART_RF_SHFT                                                                       0x0

#define HWIO_MSS_UIM2_UART_DM_RF_15_ADDR                                                                        (MSS_UIM2_UART_DM_REG_BASE      + 0x00000178)
#define HWIO_MSS_UIM2_UART_DM_RF_15_OFFS                                                                        (MSS_UIM2_UART_DM_REG_BASE_OFFS + 0x00000178)
#define HWIO_MSS_UIM2_UART_DM_RF_15_RMSK                                                                        0xffffffff
#define HWIO_MSS_UIM2_UART_DM_RF_15_IN          \
        in_dword_masked(HWIO_MSS_UIM2_UART_DM_RF_15_ADDR, HWIO_MSS_UIM2_UART_DM_RF_15_RMSK)
#define HWIO_MSS_UIM2_UART_DM_RF_15_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM2_UART_DM_RF_15_ADDR, m)
#define HWIO_MSS_UIM2_UART_DM_RF_15_UART_RF_BMSK                                                                0xffffffff
#define HWIO_MSS_UIM2_UART_DM_RF_15_UART_RF_SHFT                                                                       0x0

#define HWIO_MSS_UIM2_UART_DM_RF_16_ADDR                                                                        (MSS_UIM2_UART_DM_REG_BASE      + 0x0000017c)
#define HWIO_MSS_UIM2_UART_DM_RF_16_OFFS                                                                        (MSS_UIM2_UART_DM_REG_BASE_OFFS + 0x0000017c)
#define HWIO_MSS_UIM2_UART_DM_RF_16_RMSK                                                                        0xffffffff
#define HWIO_MSS_UIM2_UART_DM_RF_16_IN          \
        in_dword_masked(HWIO_MSS_UIM2_UART_DM_RF_16_ADDR, HWIO_MSS_UIM2_UART_DM_RF_16_RMSK)
#define HWIO_MSS_UIM2_UART_DM_RF_16_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM2_UART_DM_RF_16_ADDR, m)
#define HWIO_MSS_UIM2_UART_DM_RF_16_UART_RF_BMSK                                                                0xffffffff
#define HWIO_MSS_UIM2_UART_DM_RF_16_UART_RF_SHFT                                                                       0x0

#define HWIO_MSS_UIM2_UART_DM_UIM_CFG_ADDR                                                                      (MSS_UIM2_UART_DM_REG_BASE      + 0x00000180)
#define HWIO_MSS_UIM2_UART_DM_UIM_CFG_OFFS                                                                      (MSS_UIM2_UART_DM_REG_BASE_OFFS + 0x00000180)
#define HWIO_MSS_UIM2_UART_DM_UIM_CFG_RMSK                                                                          0xf77f
#define HWIO_MSS_UIM2_UART_DM_UIM_CFG_IN          \
        in_dword_masked(HWIO_MSS_UIM2_UART_DM_UIM_CFG_ADDR, HWIO_MSS_UIM2_UART_DM_UIM_CFG_RMSK)
#define HWIO_MSS_UIM2_UART_DM_UIM_CFG_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM2_UART_DM_UIM_CFG_ADDR, m)
#define HWIO_MSS_UIM2_UART_DM_UIM_CFG_OUT(v)      \
        out_dword(HWIO_MSS_UIM2_UART_DM_UIM_CFG_ADDR,v)
#define HWIO_MSS_UIM2_UART_DM_UIM_CFG_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_UIM2_UART_DM_UIM_CFG_ADDR,m,v,HWIO_MSS_UIM2_UART_DM_UIM_CFG_IN)
#define HWIO_MSS_UIM2_UART_DM_UIM_CFG_BATT_ALARM_QUICK_DROP_EN_BMSK                                                 0x8000
#define HWIO_MSS_UIM2_UART_DM_UIM_CFG_BATT_ALARM_QUICK_DROP_EN_SHFT                                                    0xf
#define HWIO_MSS_UIM2_UART_DM_UIM_CFG_TESTBUS_EN_BMSK                                                               0x4000
#define HWIO_MSS_UIM2_UART_DM_UIM_CFG_TESTBUS_EN_SHFT                                                                  0xe
#define HWIO_MSS_UIM2_UART_DM_UIM_CFG_SW_RESET_BMSK                                                                 0x2000
#define HWIO_MSS_UIM2_UART_DM_UIM_CFG_SW_RESET_SHFT                                                                    0xd
#define HWIO_MSS_UIM2_UART_DM_UIM_CFG_MODE18_BMSK                                                                   0x1000
#define HWIO_MSS_UIM2_UART_DM_UIM_CFG_MODE18_SHFT                                                                      0xc
#define HWIO_MSS_UIM2_UART_DM_UIM_CFG_PMIC_ALARM_EN_BMSK                                                             0x400
#define HWIO_MSS_UIM2_UART_DM_UIM_CFG_PMIC_ALARM_EN_SHFT                                                               0xa
#define HWIO_MSS_UIM2_UART_DM_UIM_CFG_BATT_ALARM_TRIGGER_EN_BMSK                                                     0x200
#define HWIO_MSS_UIM2_UART_DM_UIM_CFG_BATT_ALARM_TRIGGER_EN_SHFT                                                       0x9
#define HWIO_MSS_UIM2_UART_DM_UIM_CFG_UIM_RMV_TRIGGER_EN_BMSK                                                        0x100
#define HWIO_MSS_UIM2_UART_DM_UIM_CFG_UIM_RMV_TRIGGER_EN_SHFT                                                          0x8
#define HWIO_MSS_UIM2_UART_DM_UIM_CFG_UIM_CARD_EVENTS_ENABLE_BMSK                                                     0x40
#define HWIO_MSS_UIM2_UART_DM_UIM_CFG_UIM_CARD_EVENTS_ENABLE_SHFT                                                      0x6
#define HWIO_MSS_UIM2_UART_DM_UIM_CFG_UIM_PRESENT_POLARITY_BMSK                                                       0x20
#define HWIO_MSS_UIM2_UART_DM_UIM_CFG_UIM_PRESENT_POLARITY_SHFT                                                        0x5
#define HWIO_MSS_UIM2_UART_DM_UIM_CFG_EVENT_DEBOUNCE_TIME_BMSK                                                        0x1f
#define HWIO_MSS_UIM2_UART_DM_UIM_CFG_EVENT_DEBOUNCE_TIME_SHFT                                                         0x0

#define HWIO_MSS_UIM2_UART_DM_UIM_CMD_ADDR                                                                      (MSS_UIM2_UART_DM_REG_BASE      + 0x00000184)
#define HWIO_MSS_UIM2_UART_DM_UIM_CMD_OFFS                                                                      (MSS_UIM2_UART_DM_REG_BASE_OFFS + 0x00000184)
#define HWIO_MSS_UIM2_UART_DM_UIM_CMD_RMSK                                                                             0x3
#define HWIO_MSS_UIM2_UART_DM_UIM_CMD_OUT(v)      \
        out_dword(HWIO_MSS_UIM2_UART_DM_UIM_CMD_ADDR,v)
#define HWIO_MSS_UIM2_UART_DM_UIM_CMD_RECOVER_FROM_HW_DEACTIVATION_BMSK                                                0x2
#define HWIO_MSS_UIM2_UART_DM_UIM_CMD_RECOVER_FROM_HW_DEACTIVATION_SHFT                                                0x1
#define HWIO_MSS_UIM2_UART_DM_UIM_CMD_INITIATE_HW_DEACTIVATION_BMSK                                                    0x1
#define HWIO_MSS_UIM2_UART_DM_UIM_CMD_INITIATE_HW_DEACTIVATION_SHFT                                                    0x0

#define HWIO_MSS_UIM2_UART_DM_UIM_IO_STATUS_ADDR                                                                (MSS_UIM2_UART_DM_REG_BASE      + 0x00000188)
#define HWIO_MSS_UIM2_UART_DM_UIM_IO_STATUS_OFFS                                                                (MSS_UIM2_UART_DM_REG_BASE_OFFS + 0x00000188)
#define HWIO_MSS_UIM2_UART_DM_UIM_IO_STATUS_RMSK                                                                       0x7
#define HWIO_MSS_UIM2_UART_DM_UIM_IO_STATUS_IN          \
        in_dword_masked(HWIO_MSS_UIM2_UART_DM_UIM_IO_STATUS_ADDR, HWIO_MSS_UIM2_UART_DM_UIM_IO_STATUS_RMSK)
#define HWIO_MSS_UIM2_UART_DM_UIM_IO_STATUS_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM2_UART_DM_UIM_IO_STATUS_ADDR, m)
#define HWIO_MSS_UIM2_UART_DM_UIM_IO_STATUS_UIM_IO_WRITE_IN_PROGRESS_BMSK                                              0x4
#define HWIO_MSS_UIM2_UART_DM_UIM_IO_STATUS_UIM_IO_WRITE_IN_PROGRESS_SHFT                                              0x2
#define HWIO_MSS_UIM2_UART_DM_UIM_IO_STATUS_UIM_DEACTIVATION_STATUS_BMSK                                               0x2
#define HWIO_MSS_UIM2_UART_DM_UIM_IO_STATUS_UIM_DEACTIVATION_STATUS_SHFT                                               0x1
#define HWIO_MSS_UIM2_UART_DM_UIM_IO_STATUS_CARD_PRESENCE_BMSK                                                         0x1
#define HWIO_MSS_UIM2_UART_DM_UIM_IO_STATUS_CARD_PRESENCE_SHFT                                                         0x0

#define HWIO_MSS_UIM2_UART_DM_UIM_IRQ_ISR_ADDR                                                                  (MSS_UIM2_UART_DM_REG_BASE      + 0x0000018c)
#define HWIO_MSS_UIM2_UART_DM_UIM_IRQ_ISR_OFFS                                                                  (MSS_UIM2_UART_DM_REG_BASE_OFFS + 0x0000018c)
#define HWIO_MSS_UIM2_UART_DM_UIM_IRQ_ISR_RMSK                                                                        0x1f
#define HWIO_MSS_UIM2_UART_DM_UIM_IRQ_ISR_IN          \
        in_dword_masked(HWIO_MSS_UIM2_UART_DM_UIM_IRQ_ISR_ADDR, HWIO_MSS_UIM2_UART_DM_UIM_IRQ_ISR_RMSK)
#define HWIO_MSS_UIM2_UART_DM_UIM_IRQ_ISR_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM2_UART_DM_UIM_IRQ_ISR_ADDR, m)
#define HWIO_MSS_UIM2_UART_DM_UIM_IRQ_ISR_UIM_IO_WRITE_DONE_BMSK                                                      0x10
#define HWIO_MSS_UIM2_UART_DM_UIM_IRQ_ISR_UIM_IO_WRITE_DONE_SHFT                                                       0x4
#define HWIO_MSS_UIM2_UART_DM_UIM_IRQ_ISR_HW_SEQUENCE_FINISH_BMSK                                                      0x8
#define HWIO_MSS_UIM2_UART_DM_UIM_IRQ_ISR_HW_SEQUENCE_FINISH_SHFT                                                      0x3
#define HWIO_MSS_UIM2_UART_DM_UIM_IRQ_ISR_BATT_ALARM_BMSK                                                              0x4
#define HWIO_MSS_UIM2_UART_DM_UIM_IRQ_ISR_BATT_ALARM_SHFT                                                              0x2
#define HWIO_MSS_UIM2_UART_DM_UIM_IRQ_ISR_UIM_CARD_INSERTION_BMSK                                                      0x2
#define HWIO_MSS_UIM2_UART_DM_UIM_IRQ_ISR_UIM_CARD_INSERTION_SHFT                                                      0x1
#define HWIO_MSS_UIM2_UART_DM_UIM_IRQ_ISR_UIM_CARD_REMOVAL_BMSK                                                        0x1
#define HWIO_MSS_UIM2_UART_DM_UIM_IRQ_ISR_UIM_CARD_REMOVAL_SHFT                                                        0x0

#define HWIO_MSS_UIM2_UART_DM_UIM_IRQ_MISR_ADDR                                                                 (MSS_UIM2_UART_DM_REG_BASE      + 0x00000190)
#define HWIO_MSS_UIM2_UART_DM_UIM_IRQ_MISR_OFFS                                                                 (MSS_UIM2_UART_DM_REG_BASE_OFFS + 0x00000190)
#define HWIO_MSS_UIM2_UART_DM_UIM_IRQ_MISR_RMSK                                                                       0x1f
#define HWIO_MSS_UIM2_UART_DM_UIM_IRQ_MISR_IN          \
        in_dword_masked(HWIO_MSS_UIM2_UART_DM_UIM_IRQ_MISR_ADDR, HWIO_MSS_UIM2_UART_DM_UIM_IRQ_MISR_RMSK)
#define HWIO_MSS_UIM2_UART_DM_UIM_IRQ_MISR_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM2_UART_DM_UIM_IRQ_MISR_ADDR, m)
#define HWIO_MSS_UIM2_UART_DM_UIM_IRQ_MISR_UIM_IO_WRITE_DONE_BMSK                                                     0x10
#define HWIO_MSS_UIM2_UART_DM_UIM_IRQ_MISR_UIM_IO_WRITE_DONE_SHFT                                                      0x4
#define HWIO_MSS_UIM2_UART_DM_UIM_IRQ_MISR_HW_SEQUENCE_FINISH_BMSK                                                     0x8
#define HWIO_MSS_UIM2_UART_DM_UIM_IRQ_MISR_HW_SEQUENCE_FINISH_SHFT                                                     0x3
#define HWIO_MSS_UIM2_UART_DM_UIM_IRQ_MISR_BATT_ALARM_BMSK                                                             0x4
#define HWIO_MSS_UIM2_UART_DM_UIM_IRQ_MISR_BATT_ALARM_SHFT                                                             0x2
#define HWIO_MSS_UIM2_UART_DM_UIM_IRQ_MISR_UIM_CARD_INSERTION_BMSK                                                     0x2
#define HWIO_MSS_UIM2_UART_DM_UIM_IRQ_MISR_UIM_CARD_INSERTION_SHFT                                                     0x1
#define HWIO_MSS_UIM2_UART_DM_UIM_IRQ_MISR_UIM_CARD_REMOVAL_BMSK                                                       0x1
#define HWIO_MSS_UIM2_UART_DM_UIM_IRQ_MISR_UIM_CARD_REMOVAL_SHFT                                                       0x0

#define HWIO_MSS_UIM2_UART_DM_UIM_IRQ_CLR_ADDR                                                                  (MSS_UIM2_UART_DM_REG_BASE      + 0x00000194)
#define HWIO_MSS_UIM2_UART_DM_UIM_IRQ_CLR_OFFS                                                                  (MSS_UIM2_UART_DM_REG_BASE_OFFS + 0x00000194)
#define HWIO_MSS_UIM2_UART_DM_UIM_IRQ_CLR_RMSK                                                                        0x1f
#define HWIO_MSS_UIM2_UART_DM_UIM_IRQ_CLR_OUT(v)      \
        out_dword(HWIO_MSS_UIM2_UART_DM_UIM_IRQ_CLR_ADDR,v)
#define HWIO_MSS_UIM2_UART_DM_UIM_IRQ_CLR_UIM_IO_WRITE_DONE_BMSK                                                      0x10
#define HWIO_MSS_UIM2_UART_DM_UIM_IRQ_CLR_UIM_IO_WRITE_DONE_SHFT                                                       0x4
#define HWIO_MSS_UIM2_UART_DM_UIM_IRQ_CLR_HW_SEQUENCE_FINISH_BMSK                                                      0x8
#define HWIO_MSS_UIM2_UART_DM_UIM_IRQ_CLR_HW_SEQUENCE_FINISH_SHFT                                                      0x3
#define HWIO_MSS_UIM2_UART_DM_UIM_IRQ_CLR_BATT_ALARM_BMSK                                                              0x4
#define HWIO_MSS_UIM2_UART_DM_UIM_IRQ_CLR_BATT_ALARM_SHFT                                                              0x2
#define HWIO_MSS_UIM2_UART_DM_UIM_IRQ_CLR_UIM_CARD_INSERTION_BMSK                                                      0x2
#define HWIO_MSS_UIM2_UART_DM_UIM_IRQ_CLR_UIM_CARD_INSERTION_SHFT                                                      0x1
#define HWIO_MSS_UIM2_UART_DM_UIM_IRQ_CLR_UIM_CARD_REMOVAL_BMSK                                                        0x1
#define HWIO_MSS_UIM2_UART_DM_UIM_IRQ_CLR_UIM_CARD_REMOVAL_SHFT                                                        0x0

#define HWIO_MSS_UIM2_UART_DM_UIM_IRQ_IMR_ADDR                                                                  (MSS_UIM2_UART_DM_REG_BASE      + 0x00000198)
#define HWIO_MSS_UIM2_UART_DM_UIM_IRQ_IMR_OFFS                                                                  (MSS_UIM2_UART_DM_REG_BASE_OFFS + 0x00000198)
#define HWIO_MSS_UIM2_UART_DM_UIM_IRQ_IMR_RMSK                                                                        0x1f
#define HWIO_MSS_UIM2_UART_DM_UIM_IRQ_IMR_IN          \
        in_dword_masked(HWIO_MSS_UIM2_UART_DM_UIM_IRQ_IMR_ADDR, HWIO_MSS_UIM2_UART_DM_UIM_IRQ_IMR_RMSK)
#define HWIO_MSS_UIM2_UART_DM_UIM_IRQ_IMR_INM(m)      \
        in_dword_masked(HWIO_MSS_UIM2_UART_DM_UIM_IRQ_IMR_ADDR, m)
#define HWIO_MSS_UIM2_UART_DM_UIM_IRQ_IMR_OUT(v)      \
        out_dword(HWIO_MSS_UIM2_UART_DM_UIM_IRQ_IMR_ADDR,v)
#define HWIO_MSS_UIM2_UART_DM_UIM_IRQ_IMR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_UIM2_UART_DM_UIM_IRQ_IMR_ADDR,m,v,HWIO_MSS_UIM2_UART_DM_UIM_IRQ_IMR_IN)
#define HWIO_MSS_UIM2_UART_DM_UIM_IRQ_IMR_UIM_IO_WRITE_DONE_BMSK                                                      0x10
#define HWIO_MSS_UIM2_UART_DM_UIM_IRQ_IMR_UIM_IO_WRITE_DONE_SHFT                                                       0x4
#define HWIO_MSS_UIM2_UART_DM_UIM_IRQ_IMR_HW_SEQUENCE_FINISH_BMSK                                                      0x8
#define HWIO_MSS_UIM2_UART_DM_UIM_IRQ_IMR_HW_SEQUENCE_FINISH_SHFT                                                      0x3
#define HWIO_MSS_UIM2_UART_DM_UIM_IRQ_IMR_BATT_ALARM_BMSK                                                              0x4
#define HWIO_MSS_UIM2_UART_DM_UIM_IRQ_IMR_BATT_ALARM_SHFT                                                              0x2
#define HWIO_MSS_UIM2_UART_DM_UIM_IRQ_IMR_UIM_CARD_INSERTION_BMSK                                                      0x2
#define HWIO_MSS_UIM2_UART_DM_UIM_IRQ_IMR_UIM_CARD_INSERTION_SHFT                                                      0x1
#define HWIO_MSS_UIM2_UART_DM_UIM_IRQ_IMR_UIM_CARD_REMOVAL_BMSK                                                        0x1
#define HWIO_MSS_UIM2_UART_DM_UIM_IRQ_IMR_UIM_CARD_REMOVAL_SHFT                                                        0x0

#define HWIO_MSS_UIM2_UART_DM_UIM_IRQ_IMR_SET_ADDR                                                              (MSS_UIM2_UART_DM_REG_BASE      + 0x0000019c)
#define HWIO_MSS_UIM2_UART_DM_UIM_IRQ_IMR_SET_OFFS                                                              (MSS_UIM2_UART_DM_REG_BASE_OFFS + 0x0000019c)
#define HWIO_MSS_UIM2_UART_DM_UIM_IRQ_IMR_SET_RMSK                                                                    0x1f
#define HWIO_MSS_UIM2_UART_DM_UIM_IRQ_IMR_SET_OUT(v)      \
        out_dword(HWIO_MSS_UIM2_UART_DM_UIM_IRQ_IMR_SET_ADDR,v)
#define HWIO_MSS_UIM2_UART_DM_UIM_IRQ_IMR_SET_UIM_IO_WRITE_DONE_BMSK                                                  0x10
#define HWIO_MSS_UIM2_UART_DM_UIM_IRQ_IMR_SET_UIM_IO_WRITE_DONE_SHFT                                                   0x4
#define HWIO_MSS_UIM2_UART_DM_UIM_IRQ_IMR_SET_HW_SEQUENCE_FINISH_BMSK                                                  0x8
#define HWIO_MSS_UIM2_UART_DM_UIM_IRQ_IMR_SET_HW_SEQUENCE_FINISH_SHFT                                                  0x3
#define HWIO_MSS_UIM2_UART_DM_UIM_IRQ_IMR_SET_BATT_ALARM_BMSK                                                          0x4
#define HWIO_MSS_UIM2_UART_DM_UIM_IRQ_IMR_SET_BATT_ALARM_SHFT                                                          0x2
#define HWIO_MSS_UIM2_UART_DM_UIM_IRQ_IMR_SET_UIM_CARD_INSERTION_BMSK                                                  0x2
#define HWIO_MSS_UIM2_UART_DM_UIM_IRQ_IMR_SET_UIM_CARD_INSERTION_SHFT                                                  0x1
#define HWIO_MSS_UIM2_UART_DM_UIM_IRQ_IMR_SET_UIM_CARD_REMOVAL_BMSK                                                    0x1
#define HWIO_MSS_UIM2_UART_DM_UIM_IRQ_IMR_SET_UIM_CARD_REMOVAL_SHFT                                                    0x0

#define HWIO_MSS_UIM2_UART_DM_UIM_IRQ_IMR_CLR_ADDR                                                              (MSS_UIM2_UART_DM_REG_BASE      + 0x000001a0)
#define HWIO_MSS_UIM2_UART_DM_UIM_IRQ_IMR_CLR_OFFS                                                              (MSS_UIM2_UART_DM_REG_BASE_OFFS + 0x000001a0)
#define HWIO_MSS_UIM2_UART_DM_UIM_IRQ_IMR_CLR_RMSK                                                                    0x1f
#define HWIO_MSS_UIM2_UART_DM_UIM_IRQ_IMR_CLR_OUT(v)      \
        out_dword(HWIO_MSS_UIM2_UART_DM_UIM_IRQ_IMR_CLR_ADDR,v)
#define HWIO_MSS_UIM2_UART_DM_UIM_IRQ_IMR_CLR_UIM_IO_WRITE_DONE_BMSK                                                  0x10
#define HWIO_MSS_UIM2_UART_DM_UIM_IRQ_IMR_CLR_UIM_IO_WRITE_DONE_SHFT                                                   0x4
#define HWIO_MSS_UIM2_UART_DM_UIM_IRQ_IMR_CLR_HW_SEQUENCE_FINISH_BMSK                                                  0x8
#define HWIO_MSS_UIM2_UART_DM_UIM_IRQ_IMR_CLR_HW_SEQUENCE_FINISH_SHFT                                                  0x3
#define HWIO_MSS_UIM2_UART_DM_UIM_IRQ_IMR_CLR_BATT_ALARM_BMSK                                                          0x4
#define HWIO_MSS_UIM2_UART_DM_UIM_IRQ_IMR_CLR_BATT_ALARM_SHFT                                                          0x2
#define HWIO_MSS_UIM2_UART_DM_UIM_IRQ_IMR_CLR_UIM_CARD_INSERTION_BMSK                                                  0x2
#define HWIO_MSS_UIM2_UART_DM_UIM_IRQ_IMR_CLR_UIM_CARD_INSERTION_SHFT                                                  0x1
#define HWIO_MSS_UIM2_UART_DM_UIM_IRQ_IMR_CLR_UIM_CARD_REMOVAL_BMSK                                                    0x1
#define HWIO_MSS_UIM2_UART_DM_UIM_IRQ_IMR_CLR_UIM_CARD_REMOVAL_SHFT                                                    0x0


#endif /* __UIMJOLOKIAHWIOMACROS_H__ */
