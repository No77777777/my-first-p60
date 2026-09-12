#ifndef __ABTIMEOUT_HWIO_WCSS_H__
#define __ABTIMEOUT_HWIO_WCSS_H__
/*
===========================================================================
*/
/**
  @file ABTimeout_HWIO_WCSS.h
  @brief Auto-generated HWIO interface include file.

  This file contains HWIO register definitions for the following modules:
    WCSS_ECAHB_TSLV
    WCSS_HM_A_WCSS_WAHB_AHB_TSLV_WFSS_WAHB_AHB_TSLV
    WCSS_HM_A_WIFI_APB_1_A_WCSS_WAHB_APB_TSLV_WFSS_WAHB_APB_TSLV
    WCSS_HM_A_WIFI_APB_3_A_WCMN_MAC_WCMN_REG
    WCSS_SR
    TCSR_TCSR_REGS
    WCSS_HM_A_NOC_CFG_WIFI_NOC

  'Include' filters applied: BASE[WCSS_HM_A_WCSS_WAHB_AHB_TSLV_WFSS_WAHB_AHB_TSLV] WCSS_HM_A_NOC_CFG_WNOC_OBS_ID_COREID[WCSS_HM_A_NOC_CFG_WIFI_NOC] WCSS_HM_A_NOC_CFG_WNOC_SBM_ID_COREID[WCSS_HM_A_NOC_CFG_WIFI_NOC] WCSS_SR_SR_INTERRUPT_ENABLE[WCSS_SR] WCSS_SR_SR_INTERRUPT_STATUS[WCSS_SR] BASE[WCSS_HM_A_WIFI_APB_1_A_WCSS_WAHB_APB_TSLV_WFSS_WAHB_APB_TSLV] BASE[WCSS_ECAHB_TSLV] QDSP_ERROR_INTR_ENABLES_SET[WCSS_HM_A_WIFI_APB_3_A_WCMN_MAC_WCMN_REG] QDSP_ERROR_INTR_MASKED_STATUS[WCSS_HM_A_WIFI_APB_3_A_WCMN_MAC_WCMN_REG] WAKEUP_INTR_ENABLES_SET[WCSS_HM_A_WIFI_APB_3_A_WCMN_MAC_WCMN_REG] TIMEOUT_SLAVE_GLB_EN[TCSR_TCSR_REGS] 
  'Exclude' filters applied: RESERVED DUMMY 
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

  $Header: //components/rel/core.mpss/10.0/settings/systemdrivers/icb/config/sdm845/wcss/ABTimeout_HWIO_WCSS.h#1 $
  $DateTime: 2019/04/24 00:03:26 $
  $Author: pwbldsvc $

  ===========================================================================
*/

#include "msmhwiobase.h"


/*----------------------------------------------------------------------------
 * MODULE: TCSR_TCSR_REGS
 *--------------------------------------------------------------------------*/

#define TCSR_TCSR_REGS_REG_BASE                                                                                      (CORE_TOP_CSR_BASE      + 0x000c0000)

#define HWIO_TCSR_TIMEOUT_SLAVE_GLB_EN_ADDR                                                                          (TCSR_TCSR_REGS_REG_BASE      + 0x00000800)
#define HWIO_TCSR_TIMEOUT_SLAVE_GLB_EN_RMSK                                                                                 0x1
#define HWIO_TCSR_TIMEOUT_SLAVE_GLB_EN_IN          \
        in_dword_masked(HWIO_TCSR_TIMEOUT_SLAVE_GLB_EN_ADDR, HWIO_TCSR_TIMEOUT_SLAVE_GLB_EN_RMSK)
#define HWIO_TCSR_TIMEOUT_SLAVE_GLB_EN_INM(m)      \
        in_dword_masked(HWIO_TCSR_TIMEOUT_SLAVE_GLB_EN_ADDR, m)
#define HWIO_TCSR_TIMEOUT_SLAVE_GLB_EN_OUT(v)      \
        out_dword(HWIO_TCSR_TIMEOUT_SLAVE_GLB_EN_ADDR,v)
#define HWIO_TCSR_TIMEOUT_SLAVE_GLB_EN_OUTM(m,v) \
        out_dword_masked_ns(HWIO_TCSR_TIMEOUT_SLAVE_GLB_EN_ADDR,m,v,HWIO_TCSR_TIMEOUT_SLAVE_GLB_EN_IN)
#define HWIO_TCSR_TIMEOUT_SLAVE_GLB_EN_TIMEOUT_SLAVE_GLB_EN_BMSK                                                            0x1
#define HWIO_TCSR_TIMEOUT_SLAVE_GLB_EN_TIMEOUT_SLAVE_GLB_EN_SHFT                                                            0x0

/*----------------------------------------------------------------------------
 * MODULE: WCSS_ECAHB_TSLV
 *--------------------------------------------------------------------------*/

#define WCSS_ECAHB_TSLV_REG_BASE                                      (WCSS_WRAPPER_BASE      + 0x00010000)

/*----------------------------------------------------------------------------
 * MODULE: WCSS_SR
 *--------------------------------------------------------------------------*/

#define WCSS_SR_REG_BASE                                                                        (WCSS_WRAPPER_BASE      + 0x00020000)

#define HWIO_WCSS_SR_SR_INTERRUPT_STATUS_ADDR                                                   (WCSS_SR_REG_BASE      + 0x0000000c)
#define HWIO_WCSS_SR_SR_INTERRUPT_STATUS_RMSK                                                        0x7ff
#define HWIO_WCSS_SR_SR_INTERRUPT_STATUS_IN          \
        in_dword_masked(HWIO_WCSS_SR_SR_INTERRUPT_STATUS_ADDR, HWIO_WCSS_SR_SR_INTERRUPT_STATUS_RMSK)
#define HWIO_WCSS_SR_SR_INTERRUPT_STATUS_INM(m)      \
        in_dword_masked(HWIO_WCSS_SR_SR_INTERRUPT_STATUS_ADDR, m)
#define HWIO_WCSS_SR_SR_INTERRUPT_STATUS_ECAHB_TSLV_INTR_BMSK                                        0x400
#define HWIO_WCSS_SR_SR_INTERRUPT_STATUS_ECAHB_TSLV_INTR_SHFT                                          0xa
#define HWIO_WCSS_SR_SR_INTERRUPT_STATUS_PMM_SR_XO_SETTLE_TIMEOUT_BMSK                               0x200
#define HWIO_WCSS_SR_SR_INTERRUPT_STATUS_PMM_SR_XO_SETTLE_TIMEOUT_SHFT                                 0x9
#define HWIO_WCSS_SR_SR_INTERRUPT_STATUS_PMM_WCSS_WAKEUP_IRQ_ACK_BMSK                                0x100
#define HWIO_WCSS_SR_SR_INTERRUPT_STATUS_PMM_WCSS_WAKEUP_IRQ_ACK_SHFT                                  0x8
#define HWIO_WCSS_SR_SR_INTERRUPT_STATUS_WLAN2_HW2SW_GRANT_BMSK                                       0x80
#define HWIO_WCSS_SR_SR_INTERRUPT_STATUS_WLAN2_HW2SW_GRANT_SHFT                                        0x7
#define HWIO_WCSS_SR_SR_INTERRUPT_STATUS_WLAN1_HW2SW_GRANT_BMSK                                       0x40
#define HWIO_WCSS_SR_SR_INTERRUPT_STATUS_WLAN1_HW2SW_GRANT_SHFT                                        0x6
#define HWIO_WCSS_SR_SR_INTERRUPT_STATUS_VALUE_REG_UPDATED_WITH_INVALID_ADDR_BMSK                     0x20
#define HWIO_WCSS_SR_SR_INTERRUPT_STATUS_VALUE_REG_UPDATED_WITH_INVALID_ADDR_SHFT                      0x5
#define HWIO_WCSS_SR_SR_INTERRUPT_STATUS_ECAHB_TIMEOUT_BMSK                                           0x10
#define HWIO_WCSS_SR_SR_INTERRUPT_STATUS_ECAHB_TIMEOUT_SHFT                                            0x4
#define HWIO_WCSS_SR_SR_INTERRUPT_STATUS_INVALID_ADDR_BMSK                                             0x8
#define HWIO_WCSS_SR_SR_INTERRUPT_STATUS_INVALID_ADDR_SHFT                                             0x3
#define HWIO_WCSS_SR_SR_INTERRUPT_STATUS_ERR_RESP_BMSK                                                 0x4
#define HWIO_WCSS_SR_SR_INTERRUPT_STATUS_ERR_RESP_SHFT                                                 0x2
#define HWIO_WCSS_SR_SR_INTERRUPT_STATUS_FORCE_WAKE_BMSK                                               0x2
#define HWIO_WCSS_SR_SR_INTERRUPT_STATUS_FORCE_WAKE_SHFT                                               0x1
#define HWIO_WCSS_SR_SR_INTERRUPT_STATUS_DIRTY_BIT_SET_BMSK                                            0x1
#define HWIO_WCSS_SR_SR_INTERRUPT_STATUS_DIRTY_BIT_SET_SHFT                                            0x0

#define HWIO_WCSS_SR_SR_INTERRUPT_ENABLE_ADDR                                                   (WCSS_SR_REG_BASE      + 0x00000010)
#define HWIO_WCSS_SR_SR_INTERRUPT_ENABLE_RMSK                                                        0x7ff
#define HWIO_WCSS_SR_SR_INTERRUPT_ENABLE_IN          \
        in_dword_masked(HWIO_WCSS_SR_SR_INTERRUPT_ENABLE_ADDR, HWIO_WCSS_SR_SR_INTERRUPT_ENABLE_RMSK)
#define HWIO_WCSS_SR_SR_INTERRUPT_ENABLE_INM(m)      \
        in_dword_masked(HWIO_WCSS_SR_SR_INTERRUPT_ENABLE_ADDR, m)
#define HWIO_WCSS_SR_SR_INTERRUPT_ENABLE_OUT(v)      \
        out_dword(HWIO_WCSS_SR_SR_INTERRUPT_ENABLE_ADDR,v)
#define HWIO_WCSS_SR_SR_INTERRUPT_ENABLE_OUTM(m,v) \
        out_dword_masked_ns(HWIO_WCSS_SR_SR_INTERRUPT_ENABLE_ADDR,m,v,HWIO_WCSS_SR_SR_INTERRUPT_ENABLE_IN)
#define HWIO_WCSS_SR_SR_INTERRUPT_ENABLE_ECAHB_TSLV_INTR_BMSK                                        0x400
#define HWIO_WCSS_SR_SR_INTERRUPT_ENABLE_ECAHB_TSLV_INTR_SHFT                                          0xa
#define HWIO_WCSS_SR_SR_INTERRUPT_ENABLE_PMM_SR_XO_SETTLE_TIMEOUT_BMSK                               0x200
#define HWIO_WCSS_SR_SR_INTERRUPT_ENABLE_PMM_SR_XO_SETTLE_TIMEOUT_SHFT                                 0x9
#define HWIO_WCSS_SR_SR_INTERRUPT_ENABLE_PMM_WCSS_WAKEUP_IRQ_ACK_BMSK                                0x100
#define HWIO_WCSS_SR_SR_INTERRUPT_ENABLE_PMM_WCSS_WAKEUP_IRQ_ACK_SHFT                                  0x8
#define HWIO_WCSS_SR_SR_INTERRUPT_ENABLE_WLAN2_HW2SW_GRANT_BMSK                                       0x80
#define HWIO_WCSS_SR_SR_INTERRUPT_ENABLE_WLAN2_HW2SW_GRANT_SHFT                                        0x7
#define HWIO_WCSS_SR_SR_INTERRUPT_ENABLE_WLAN1_HW2SW_GRANT_BMSK                                       0x40
#define HWIO_WCSS_SR_SR_INTERRUPT_ENABLE_WLAN1_HW2SW_GRANT_SHFT                                        0x6
#define HWIO_WCSS_SR_SR_INTERRUPT_ENABLE_VALUE_REG_UPDATED_WITH_INVALID_ADDR_BMSK                     0x20
#define HWIO_WCSS_SR_SR_INTERRUPT_ENABLE_VALUE_REG_UPDATED_WITH_INVALID_ADDR_SHFT                      0x5
#define HWIO_WCSS_SR_SR_INTERRUPT_ENABLE_ECAHB_TIMEOUT_BMSK                                           0x10
#define HWIO_WCSS_SR_SR_INTERRUPT_ENABLE_ECAHB_TIMEOUT_SHFT                                            0x4
#define HWIO_WCSS_SR_SR_INTERRUPT_ENABLE_INVALID_ADDR_BMSK                                             0x8
#define HWIO_WCSS_SR_SR_INTERRUPT_ENABLE_INVALID_ADDR_SHFT                                             0x3
#define HWIO_WCSS_SR_SR_INTERRUPT_ENABLE_ERR_RESP_ENABLE_BMSK                                          0x4
#define HWIO_WCSS_SR_SR_INTERRUPT_ENABLE_ERR_RESP_ENABLE_SHFT                                          0x2
#define HWIO_WCSS_SR_SR_INTERRUPT_ENABLE_FORCE_WAKE_ENABLE_BMSK                                        0x2
#define HWIO_WCSS_SR_SR_INTERRUPT_ENABLE_FORCE_WAKE_ENABLE_SHFT                                        0x1
#define HWIO_WCSS_SR_SR_INTERRUPT_ENABLE_DIRTY_BIT_SET_ENABLE_BMSK                                     0x1
#define HWIO_WCSS_SR_SR_INTERRUPT_ENABLE_DIRTY_BIT_SET_ENABLE_SHFT                                     0x0

/*----------------------------------------------------------------------------
 * MODULE: WCSS_HM_A_WCSS_WAHB_AHB_TSLV_WFSS_WAHB_AHB_TSLV
 *--------------------------------------------------------------------------*/

#define WCSS_HM_A_WCSS_WAHB_AHB_TSLV_WFSS_WAHB_AHB_TSLV_REG_BASE                   (WCSS_WRAPPER_BASE      + 0x00101000)

/*----------------------------------------------------------------------------
 * MODULE: WCSS_HM_A_NOC_CFG_WIFI_NOC
 *--------------------------------------------------------------------------*/

#define WCSS_HM_A_NOC_CFG_WIFI_NOC_REG_BASE                                                                   (WCSS_WRAPPER_BASE      + 0x00180000)

#define HWIO_WCSS_HM_A_NOC_CFG_WNOC_OBS_ID_COREID_ADDR                                                        (WCSS_HM_A_NOC_CFG_WIFI_NOC_REG_BASE      + 0x00000000)
#define HWIO_WCSS_HM_A_NOC_CFG_WNOC_OBS_ID_COREID_RMSK                                                        0xffffffff
#define HWIO_WCSS_HM_A_NOC_CFG_WNOC_OBS_ID_COREID_IN          \
        in_dword_masked(HWIO_WCSS_HM_A_NOC_CFG_WNOC_OBS_ID_COREID_ADDR, HWIO_WCSS_HM_A_NOC_CFG_WNOC_OBS_ID_COREID_RMSK)
#define HWIO_WCSS_HM_A_NOC_CFG_WNOC_OBS_ID_COREID_INM(m)      \
        in_dword_masked(HWIO_WCSS_HM_A_NOC_CFG_WNOC_OBS_ID_COREID_ADDR, m)
#define HWIO_WCSS_HM_A_NOC_CFG_WNOC_OBS_ID_COREID_CORECHECKSUM_BMSK                                           0xffffff00
#define HWIO_WCSS_HM_A_NOC_CFG_WNOC_OBS_ID_COREID_CORECHECKSUM_SHFT                                                  0x8
#define HWIO_WCSS_HM_A_NOC_CFG_WNOC_OBS_ID_COREID_CORETYPEID_BMSK                                                   0xff
#define HWIO_WCSS_HM_A_NOC_CFG_WNOC_OBS_ID_COREID_CORETYPEID_SHFT                                                    0x0

#define HWIO_WCSS_HM_A_NOC_CFG_WNOC_SBM_ID_COREID_ADDR                                                        (WCSS_HM_A_NOC_CFG_WIFI_NOC_REG_BASE      + 0x00000200)
#define HWIO_WCSS_HM_A_NOC_CFG_WNOC_SBM_ID_COREID_RMSK                                                        0xffffffff
#define HWIO_WCSS_HM_A_NOC_CFG_WNOC_SBM_ID_COREID_IN          \
        in_dword_masked(HWIO_WCSS_HM_A_NOC_CFG_WNOC_SBM_ID_COREID_ADDR, HWIO_WCSS_HM_A_NOC_CFG_WNOC_SBM_ID_COREID_RMSK)
#define HWIO_WCSS_HM_A_NOC_CFG_WNOC_SBM_ID_COREID_INM(m)      \
        in_dword_masked(HWIO_WCSS_HM_A_NOC_CFG_WNOC_SBM_ID_COREID_ADDR, m)
#define HWIO_WCSS_HM_A_NOC_CFG_WNOC_SBM_ID_COREID_CORECHECKSUM_BMSK                                           0xffffff00
#define HWIO_WCSS_HM_A_NOC_CFG_WNOC_SBM_ID_COREID_CORECHECKSUM_SHFT                                                  0x8
#define HWIO_WCSS_HM_A_NOC_CFG_WNOC_SBM_ID_COREID_CORETYPEID_BMSK                                                   0xff
#define HWIO_WCSS_HM_A_NOC_CFG_WNOC_SBM_ID_COREID_CORETYPEID_SHFT                                                    0x0

/*----------------------------------------------------------------------------
 * MODULE: WCSS_HM_A_WIFI_APB_1_A_WCSS_WAHB_APB_TSLV_WFSS_WAHB_APB_TSLV
 *--------------------------------------------------------------------------*/

#define WCSS_HM_A_WIFI_APB_1_A_WCSS_WAHB_APB_TSLV_WFSS_WAHB_APB_TSLV_REG_BASE                   (WCSS_WRAPPER_BASE      + 0x00272000)

/*----------------------------------------------------------------------------
 * MODULE: WCSS_HM_A_WIFI_APB_3_A_WCMN_MAC_WCMN_REG
 *--------------------------------------------------------------------------*/

#define WCSS_HM_A_WIFI_APB_3_A_WCMN_MAC_WCMN_REG_REG_BASE                                                              (WCSS_WRAPPER_BASE      + 0x002f0000)

#define HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_QDSP_ERROR_INTR_ENABLES_SET_ADDR                                              (WCSS_HM_A_WIFI_APB_3_A_WCMN_MAC_WCMN_REG_REG_BASE      + 0x00000088)
#define HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_QDSP_ERROR_INTR_ENABLES_SET_RMSK                                              0xffffffff
#define HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_QDSP_ERROR_INTR_ENABLES_SET_IN          \
        in_dword_masked(HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_QDSP_ERROR_INTR_ENABLES_SET_ADDR, HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_QDSP_ERROR_INTR_ENABLES_SET_RMSK)
#define HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_QDSP_ERROR_INTR_ENABLES_SET_INM(m)      \
        in_dword_masked(HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_QDSP_ERROR_INTR_ENABLES_SET_ADDR, m)
#define HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_QDSP_ERROR_INTR_ENABLES_SET_OUT(v)      \
        out_dword(HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_QDSP_ERROR_INTR_ENABLES_SET_ADDR,v)
#define HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_QDSP_ERROR_INTR_ENABLES_SET_OUTM(m,v) \
        out_dword_masked_ns(HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_QDSP_ERROR_INTR_ENABLES_SET_ADDR,m,v,HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_QDSP_ERROR_INTR_ENABLES_SET_IN)
#define HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_QDSP_ERROR_INTR_ENABLES_SET_SPARE_3_BMSK                                      0x80000000
#define HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_QDSP_ERROR_INTR_ENABLES_SET_SPARE_3_SHFT                                            0x1f
#define HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_QDSP_ERROR_INTR_ENABLES_SET_SPARE_2_BMSK                                      0x40000000
#define HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_QDSP_ERROR_INTR_ENABLES_SET_SPARE_2_SHFT                                            0x1e
#define HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_QDSP_ERROR_INTR_ENABLES_SET_SPARE_1_BMSK                                      0x20000000
#define HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_QDSP_ERROR_INTR_ENABLES_SET_SPARE_1_SHFT                                            0x1d
#define HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_QDSP_ERROR_INTR_ENABLES_SET_SPARE_0_BMSK                                      0x10000000
#define HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_QDSP_ERROR_INTR_ENABLES_SET_SPARE_0_SHFT                                            0x1c
#define HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_QDSP_ERROR_INTR_ENABLES_SET_EXTERNAL_INTR_BMSK                                 0xffc0000
#define HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_QDSP_ERROR_INTR_ENABLES_SET_EXTERNAL_INTR_SHFT                                      0x12
#define HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_QDSP_ERROR_INTR_ENABLES_SET_WFSS_DBG_INTR_BMSK                                   0x20000
#define HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_QDSP_ERROR_INTR_ENABLES_SET_WFSS_DBG_INTR_SHFT                                      0x11
#define HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_QDSP_ERROR_INTR_ENABLES_SET_SW_SLP_TMR_INTR_BMSK                                 0x10000
#define HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_QDSP_ERROR_INTR_ENABLES_SET_SW_SLP_TMR_INTR_SHFT                                    0x10
#define HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_QDSP_ERROR_INTR_ENABLES_SET_WLAN2_SLP_TMR_INTR_BMSK                               0x8000
#define HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_QDSP_ERROR_INTR_ENABLES_SET_WLAN2_SLP_TMR_INTR_SHFT                                  0xf
#define HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_QDSP_ERROR_INTR_ENABLES_SET_WLAN1_SLP_TMR_INTR_BMSK                               0x4000
#define HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_QDSP_ERROR_INTR_ENABLES_SET_WLAN1_SLP_TMR_INTR_SHFT                                  0xe
#define HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_QDSP_ERROR_INTR_ENABLES_SET_WAHB_APB_TSLV_INTR_BMSK                               0x2000
#define HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_QDSP_ERROR_INTR_ENABLES_SET_WAHB_APB_TSLV_INTR_SHFT                                  0xd
#define HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_QDSP_ERROR_INTR_ENABLES_SET_NOC_WCMN_INTR_BMSK                                    0x1000
#define HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_QDSP_ERROR_INTR_ENABLES_SET_NOC_WCMN_INTR_SHFT                                       0xc
#define HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_QDSP_ERROR_INTR_ENABLES_SET_INVALID_BB_2_INTR_BMSK                                 0x800
#define HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_QDSP_ERROR_INTR_ENABLES_SET_INVALID_BB_2_INTR_SHFT                                   0xb
#define HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_QDSP_ERROR_INTR_ENABLES_SET_INVALID_BB_1_INTR_BMSK                                 0x400
#define HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_QDSP_ERROR_INTR_ENABLES_SET_INVALID_BB_1_INTR_SHFT                                   0xa
#define HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_QDSP_ERROR_INTR_ENABLES_SET_WAHB_AHB_TSLV_INTR_BMSK                                0x200
#define HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_QDSP_ERROR_INTR_ENABLES_SET_WAHB_AHB_TSLV_INTR_SHFT                                  0x9
#define HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_QDSP_ERROR_INTR_ENABLES_SET_CE_INTR_TIMEOUT_P_BMSK                                 0x100
#define HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_QDSP_ERROR_INTR_ENABLES_SET_CE_INTR_TIMEOUT_P_SHFT                                   0x8
#define HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_QDSP_ERROR_INTR_ENABLES_SET_CE_INTR_MISC_P_BMSK                                     0x80
#define HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_QDSP_ERROR_INTR_ENABLES_SET_CE_INTR_MISC_P_SHFT                                      0x7
#define HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_QDSP_ERROR_INTR_ENABLES_SET_SMH_INT_BMSK                                            0x40
#define HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_QDSP_ERROR_INTR_ENABLES_SET_SMH_INT_SHFT                                             0x6
#define HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_QDSP_ERROR_INTR_ENABLES_SET_PMH_INT_BMSK                                            0x20
#define HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_QDSP_ERROR_INTR_ENABLES_SET_PMH_INT_SHFT                                             0x5
#define HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_QDSP_ERROR_INTR_ENABLES_SET_MCIM_INT_BMSK                                           0x10
#define HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_QDSP_ERROR_INTR_ENABLES_SET_MCIM_INT_SHFT                                            0x4
#define HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_QDSP_ERROR_INTR_ENABLES_SET_LMH_INT_BMSK                                             0x8
#define HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_QDSP_ERROR_INTR_ENABLES_SET_LMH_INT_SHFT                                             0x3
#define HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_QDSP_ERROR_INTR_ENABLES_SET_LCMH_WCI2_INTERRUPT_BMSK                                 0x4
#define HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_QDSP_ERROR_INTR_ENABLES_SET_LCMH_WCI2_INTERRUPT_SHFT                                 0x2
#define HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_QDSP_ERROR_INTR_ENABLES_SET_LCMH_STROBE_INTERRUPT_BMSK                               0x2
#define HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_QDSP_ERROR_INTR_ENABLES_SET_LCMH_STROBE_INTERRUPT_SHFT                               0x1
#define HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_QDSP_ERROR_INTR_ENABLES_SET_BMH_INT_BMSK                                             0x1
#define HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_QDSP_ERROR_INTR_ENABLES_SET_BMH_INT_SHFT                                             0x0

#define HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_QDSP_ERROR_INTR_ENABLES_SET_2_ADDR                                            (WCSS_HM_A_WIFI_APB_3_A_WCMN_MAC_WCMN_REG_REG_BASE      + 0x0000008c)
#define HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_QDSP_ERROR_INTR_ENABLES_SET_2_RMSK                                                  0x7f
#define HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_QDSP_ERROR_INTR_ENABLES_SET_2_IN          \
        in_dword_masked(HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_QDSP_ERROR_INTR_ENABLES_SET_2_ADDR, HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_QDSP_ERROR_INTR_ENABLES_SET_2_RMSK)
#define HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_QDSP_ERROR_INTR_ENABLES_SET_2_INM(m)      \
        in_dword_masked(HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_QDSP_ERROR_INTR_ENABLES_SET_2_ADDR, m)
#define HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_QDSP_ERROR_INTR_ENABLES_SET_2_OUT(v)      \
        out_dword(HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_QDSP_ERROR_INTR_ENABLES_SET_2_ADDR,v)
#define HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_QDSP_ERROR_INTR_ENABLES_SET_2_OUTM(m,v) \
        out_dword_masked_ns(HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_QDSP_ERROR_INTR_ENABLES_SET_2_ADDR,m,v,HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_QDSP_ERROR_INTR_ENABLES_SET_2_IN)
#define HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_QDSP_ERROR_INTR_ENABLES_SET_2_WAHB_PWR_DWN_ACC_ERR_INT_BMSK                         0x40
#define HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_QDSP_ERROR_INTR_ENABLES_SET_2_WAHB_PWR_DWN_ACC_ERR_INT_SHFT                          0x6
#define HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_QDSP_ERROR_INTR_ENABLES_SET_2_RFACTRL_WCMN_RFACTRL1_ERROR_INT_BMSK                  0x20
#define HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_QDSP_ERROR_INTR_ENABLES_SET_2_RFACTRL_WCMN_RFACTRL1_ERROR_INT_SHFT                   0x5
#define HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_QDSP_ERROR_INTR_ENABLES_SET_2_RFACTRL_WCMN_RFACTRL0_ERROR_INT_BMSK                  0x10
#define HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_QDSP_ERROR_INTR_ENABLES_SET_2_RFACTRL_WCMN_RFACTRL0_ERROR_INT_SHFT                   0x4
#define HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_QDSP_ERROR_INTR_ENABLES_SET_2_PMM_WCMN_WMAC2_IDLE_WAIT_TO_INT_BMSK                   0x8
#define HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_QDSP_ERROR_INTR_ENABLES_SET_2_PMM_WCMN_WMAC2_IDLE_WAIT_TO_INT_SHFT                   0x3
#define HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_QDSP_ERROR_INTR_ENABLES_SET_2_PMM_WCMN_WMAC1_IDLE_WAIT_TO_INT_BMSK                   0x4
#define HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_QDSP_ERROR_INTR_ENABLES_SET_2_PMM_WCMN_WMAC1_IDLE_WAIT_TO_INT_SHFT                   0x2
#define HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_QDSP_ERROR_INTR_ENABLES_SET_2_PMM_WCMN_PHY2_IDLE_WAIT_TO_INT_BMSK                    0x2
#define HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_QDSP_ERROR_INTR_ENABLES_SET_2_PMM_WCMN_PHY2_IDLE_WAIT_TO_INT_SHFT                    0x1
#define HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_QDSP_ERROR_INTR_ENABLES_SET_2_PMM_WCMN_PHY1_IDLE_WAIT_TO_INT_BMSK                    0x1
#define HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_QDSP_ERROR_INTR_ENABLES_SET_2_PMM_WCMN_PHY1_IDLE_WAIT_TO_INT_SHFT                    0x0

#define HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_QDSP_ERROR_INTR_MASKED_STATUS_ADDR                                            (WCSS_HM_A_WIFI_APB_3_A_WCMN_MAC_WCMN_REG_REG_BASE      + 0x000000a0)
#define HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_QDSP_ERROR_INTR_MASKED_STATUS_RMSK                                            0xffffffff
#define HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_QDSP_ERROR_INTR_MASKED_STATUS_IN          \
        in_dword_masked(HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_QDSP_ERROR_INTR_MASKED_STATUS_ADDR, HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_QDSP_ERROR_INTR_MASKED_STATUS_RMSK)
#define HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_QDSP_ERROR_INTR_MASKED_STATUS_INM(m)      \
        in_dword_masked(HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_QDSP_ERROR_INTR_MASKED_STATUS_ADDR, m)
#define HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_QDSP_ERROR_INTR_MASKED_STATUS_SPARE_3_BMSK                                    0x80000000
#define HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_QDSP_ERROR_INTR_MASKED_STATUS_SPARE_3_SHFT                                          0x1f
#define HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_QDSP_ERROR_INTR_MASKED_STATUS_SPARE_2_BMSK                                    0x40000000
#define HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_QDSP_ERROR_INTR_MASKED_STATUS_SPARE_2_SHFT                                          0x1e
#define HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_QDSP_ERROR_INTR_MASKED_STATUS_SPARE_1_BMSK                                    0x20000000
#define HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_QDSP_ERROR_INTR_MASKED_STATUS_SPARE_1_SHFT                                          0x1d
#define HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_QDSP_ERROR_INTR_MASKED_STATUS_SPARE_0_BMSK                                    0x10000000
#define HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_QDSP_ERROR_INTR_MASKED_STATUS_SPARE_0_SHFT                                          0x1c
#define HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_QDSP_ERROR_INTR_MASKED_STATUS_EXTERNAL_INTR_BMSK                               0xffc0000
#define HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_QDSP_ERROR_INTR_MASKED_STATUS_EXTERNAL_INTR_SHFT                                    0x12
#define HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_QDSP_ERROR_INTR_MASKED_STATUS_WFSS_DBG_INTR_BMSK                                 0x20000
#define HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_QDSP_ERROR_INTR_MASKED_STATUS_WFSS_DBG_INTR_SHFT                                    0x11
#define HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_QDSP_ERROR_INTR_MASKED_STATUS_SW_SLP_TMR_INTR_BMSK                               0x10000
#define HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_QDSP_ERROR_INTR_MASKED_STATUS_SW_SLP_TMR_INTR_SHFT                                  0x10
#define HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_QDSP_ERROR_INTR_MASKED_STATUS_WLAN2_SLP_TMR_INTR_BMSK                             0x8000
#define HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_QDSP_ERROR_INTR_MASKED_STATUS_WLAN2_SLP_TMR_INTR_SHFT                                0xf
#define HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_QDSP_ERROR_INTR_MASKED_STATUS_WLAN1_SLP_TMR_INTR_BMSK                             0x4000
#define HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_QDSP_ERROR_INTR_MASKED_STATUS_WLAN1_SLP_TMR_INTR_SHFT                                0xe
#define HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_QDSP_ERROR_INTR_MASKED_STATUS_WAHB_APB_TSLV_INTR_BMSK                             0x2000
#define HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_QDSP_ERROR_INTR_MASKED_STATUS_WAHB_APB_TSLV_INTR_SHFT                                0xd
#define HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_QDSP_ERROR_INTR_MASKED_STATUS_NOC_WCMN_INTR_BMSK                                  0x1000
#define HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_QDSP_ERROR_INTR_MASKED_STATUS_NOC_WCMN_INTR_SHFT                                     0xc
#define HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_QDSP_ERROR_INTR_MASKED_STATUS_INVALID_BB_2_INTR_BMSK                               0x800
#define HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_QDSP_ERROR_INTR_MASKED_STATUS_INVALID_BB_2_INTR_SHFT                                 0xb
#define HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_QDSP_ERROR_INTR_MASKED_STATUS_INVALID_BB_1_INTR_BMSK                               0x400
#define HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_QDSP_ERROR_INTR_MASKED_STATUS_INVALID_BB_1_INTR_SHFT                                 0xa
#define HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_QDSP_ERROR_INTR_MASKED_STATUS_WAHB_AHB_TSLV_INTR_BMSK                              0x200
#define HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_QDSP_ERROR_INTR_MASKED_STATUS_WAHB_AHB_TSLV_INTR_SHFT                                0x9
#define HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_QDSP_ERROR_INTR_MASKED_STATUS_CE_INTR_TIMEOUT_P_BMSK                               0x100
#define HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_QDSP_ERROR_INTR_MASKED_STATUS_CE_INTR_TIMEOUT_P_SHFT                                 0x8
#define HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_QDSP_ERROR_INTR_MASKED_STATUS_CE_INTR_MISC_P_BMSK                                   0x80
#define HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_QDSP_ERROR_INTR_MASKED_STATUS_CE_INTR_MISC_P_SHFT                                    0x7
#define HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_QDSP_ERROR_INTR_MASKED_STATUS_SMH_INT_BMSK                                          0x40
#define HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_QDSP_ERROR_INTR_MASKED_STATUS_SMH_INT_SHFT                                           0x6
#define HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_QDSP_ERROR_INTR_MASKED_STATUS_PMH_INT_BMSK                                          0x20
#define HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_QDSP_ERROR_INTR_MASKED_STATUS_PMH_INT_SHFT                                           0x5
#define HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_QDSP_ERROR_INTR_MASKED_STATUS_MCIM_INT_BMSK                                         0x10
#define HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_QDSP_ERROR_INTR_MASKED_STATUS_MCIM_INT_SHFT                                          0x4
#define HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_QDSP_ERROR_INTR_MASKED_STATUS_LMH_INT_BMSK                                           0x8
#define HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_QDSP_ERROR_INTR_MASKED_STATUS_LMH_INT_SHFT                                           0x3
#define HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_QDSP_ERROR_INTR_MASKED_STATUS_LCMH_WCI2_INTERRUPT_BMSK                               0x4
#define HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_QDSP_ERROR_INTR_MASKED_STATUS_LCMH_WCI2_INTERRUPT_SHFT                               0x2
#define HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_QDSP_ERROR_INTR_MASKED_STATUS_LCMH_STROBE_INTERRUPT_BMSK                             0x2
#define HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_QDSP_ERROR_INTR_MASKED_STATUS_LCMH_STROBE_INTERRUPT_SHFT                             0x1
#define HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_QDSP_ERROR_INTR_MASKED_STATUS_BMH_INT_BMSK                                           0x1
#define HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_QDSP_ERROR_INTR_MASKED_STATUS_BMH_INT_SHFT                                           0x0

#define HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_QDSP_ERROR_INTR_MASKED_STATUS_2_ADDR                                          (WCSS_HM_A_WIFI_APB_3_A_WCMN_MAC_WCMN_REG_REG_BASE      + 0x000000a4)
#define HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_QDSP_ERROR_INTR_MASKED_STATUS_2_RMSK                                                0x7f
#define HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_QDSP_ERROR_INTR_MASKED_STATUS_2_IN          \
        in_dword_masked(HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_QDSP_ERROR_INTR_MASKED_STATUS_2_ADDR, HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_QDSP_ERROR_INTR_MASKED_STATUS_2_RMSK)
#define HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_QDSP_ERROR_INTR_MASKED_STATUS_2_INM(m)      \
        in_dword_masked(HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_QDSP_ERROR_INTR_MASKED_STATUS_2_ADDR, m)
#define HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_QDSP_ERROR_INTR_MASKED_STATUS_2_WAHB_PWR_DWN_ACC_ERR_INT_BMSK                       0x40
#define HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_QDSP_ERROR_INTR_MASKED_STATUS_2_WAHB_PWR_DWN_ACC_ERR_INT_SHFT                        0x6
#define HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_QDSP_ERROR_INTR_MASKED_STATUS_2_RFACTRL_WCMN_RFACTRL1_ERROR_INT_BMSK                0x20
#define HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_QDSP_ERROR_INTR_MASKED_STATUS_2_RFACTRL_WCMN_RFACTRL1_ERROR_INT_SHFT                 0x5
#define HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_QDSP_ERROR_INTR_MASKED_STATUS_2_RFACTRL_WCMN_RFACTRL0_ERROR_INT_BMSK                0x10
#define HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_QDSP_ERROR_INTR_MASKED_STATUS_2_RFACTRL_WCMN_RFACTRL0_ERROR_INT_SHFT                 0x4
#define HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_QDSP_ERROR_INTR_MASKED_STATUS_2_PMM_WCMN_WMAC2_IDLE_WAIT_TO_INT_BMSK                 0x8
#define HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_QDSP_ERROR_INTR_MASKED_STATUS_2_PMM_WCMN_WMAC2_IDLE_WAIT_TO_INT_SHFT                 0x3
#define HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_QDSP_ERROR_INTR_MASKED_STATUS_2_PMM_WCMN_WMAC1_IDLE_WAIT_TO_INT_BMSK                 0x4
#define HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_QDSP_ERROR_INTR_MASKED_STATUS_2_PMM_WCMN_WMAC1_IDLE_WAIT_TO_INT_SHFT                 0x2
#define HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_QDSP_ERROR_INTR_MASKED_STATUS_2_PMM_WCMN_PHY2_IDLE_WAIT_TO_INT_BMSK                  0x2
#define HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_QDSP_ERROR_INTR_MASKED_STATUS_2_PMM_WCMN_PHY2_IDLE_WAIT_TO_INT_SHFT                  0x1
#define HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_QDSP_ERROR_INTR_MASKED_STATUS_2_PMM_WCMN_PHY1_IDLE_WAIT_TO_INT_BMSK                  0x1
#define HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_QDSP_ERROR_INTR_MASKED_STATUS_2_PMM_WCMN_PHY1_IDLE_WAIT_TO_INT_SHFT                  0x0

#define HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_WAKEUP_INTR_ENABLES_SET_1_ADDR                                                (WCSS_HM_A_WIFI_APB_3_A_WCMN_MAC_WCMN_REG_REG_BASE      + 0x000000ec)
#define HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_WAKEUP_INTR_ENABLES_SET_1_RMSK                                                0xffffffff
#define HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_WAKEUP_INTR_ENABLES_SET_1_IN          \
        in_dword_masked(HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_WAKEUP_INTR_ENABLES_SET_1_ADDR, HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_WAKEUP_INTR_ENABLES_SET_1_RMSK)
#define HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_WAKEUP_INTR_ENABLES_SET_1_INM(m)      \
        in_dword_masked(HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_WAKEUP_INTR_ENABLES_SET_1_ADDR, m)
#define HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_WAKEUP_INTR_ENABLES_SET_1_OUT(v)      \
        out_dword(HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_WAKEUP_INTR_ENABLES_SET_1_ADDR,v)
#define HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_WAKEUP_INTR_ENABLES_SET_1_OUTM(m,v) \
        out_dword_masked_ns(HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_WAKEUP_INTR_ENABLES_SET_1_ADDR,m,v,HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_WAKEUP_INTR_ENABLES_SET_1_IN)
#define HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_WAKEUP_INTR_ENABLES_SET_1_INVALID_BB_2_INTR_BMSK                              0x80000000
#define HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_WAKEUP_INTR_ENABLES_SET_1_INVALID_BB_2_INTR_SHFT                                    0x1f
#define HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_WAKEUP_INTR_ENABLES_SET_1_INVALID_BB_1_INTR_BMSK                              0x40000000
#define HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_WAKEUP_INTR_ENABLES_SET_1_INVALID_BB_1_INTR_SHFT                                    0x1e
#define HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_WAKEUP_INTR_ENABLES_SET_1_WAHB_AHB_TSLV_INTR_BMSK                             0x20000000
#define HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_WAKEUP_INTR_ENABLES_SET_1_WAHB_AHB_TSLV_INTR_SHFT                                   0x1d
#define HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_WAKEUP_INTR_ENABLES_SET_1_CE_INTR_TIMEOUT_P_BMSK                              0x10000000
#define HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_WAKEUP_INTR_ENABLES_SET_1_CE_INTR_TIMEOUT_P_SHFT                                    0x1c
#define HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_WAKEUP_INTR_ENABLES_SET_1_CE_INTR_MISC_P_BMSK                                  0x8000000
#define HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_WAKEUP_INTR_ENABLES_SET_1_CE_INTR_MISC_P_SHFT                                       0x1b
#define HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_WAKEUP_INTR_ENABLES_SET_1_SMH_INT_BMSK                                         0x4000000
#define HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_WAKEUP_INTR_ENABLES_SET_1_SMH_INT_SHFT                                              0x1a
#define HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_WAKEUP_INTR_ENABLES_SET_1_PMH_INT_BMSK                                         0x2000000
#define HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_WAKEUP_INTR_ENABLES_SET_1_PMH_INT_SHFT                                              0x19
#define HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_WAKEUP_INTR_ENABLES_SET_1_MCIM_INT_BMSK                                        0x1000000
#define HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_WAKEUP_INTR_ENABLES_SET_1_MCIM_INT_SHFT                                             0x18
#define HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_WAKEUP_INTR_ENABLES_SET_1_LMH_INT_BMSK                                          0x800000
#define HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_WAKEUP_INTR_ENABLES_SET_1_LMH_INT_SHFT                                              0x17
#define HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_WAKEUP_INTR_ENABLES_SET_1_LCMH_WCI2_INTERRUPT_BMSK                              0x400000
#define HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_WAKEUP_INTR_ENABLES_SET_1_LCMH_WCI2_INTERRUPT_SHFT                                  0x16
#define HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_WAKEUP_INTR_ENABLES_SET_1_LCMH_STROBE_INTERRUPT_BMSK                            0x200000
#define HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_WAKEUP_INTR_ENABLES_SET_1_LCMH_STROBE_INTERRUPT_SHFT                                0x15
#define HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_WAKEUP_INTR_ENABLES_SET_1_BMH_INT_BMSK                                          0x100000
#define HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_WAKEUP_INTR_ENABLES_SET_1_BMH_INT_SHFT                                              0x14
#define HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_WAKEUP_INTR_ENABLES_SET_1_CE_INTR_TARGET_P_BMSK                                  0xfff00
#define HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_WAKEUP_INTR_ENABLES_SET_1_CE_INTR_TARGET_P_SHFT                                      0x8
#define HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_WAKEUP_INTR_ENABLES_SET_1_WMAC_2_INTR_BMSK                                          0xf0
#define HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_WAKEUP_INTR_ENABLES_SET_1_WMAC_2_INTR_SHFT                                           0x4
#define HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_WAKEUP_INTR_ENABLES_SET_1_WMAC_1_INTR_BMSK                                           0xf
#define HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_WAKEUP_INTR_ENABLES_SET_1_WMAC_1_INTR_SHFT                                           0x0

#define HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_WAKEUP_INTR_ENABLES_SET_2_ADDR                                                (WCSS_HM_A_WIFI_APB_3_A_WCMN_MAC_WCMN_REG_REG_BASE      + 0x000000f0)
#define HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_WAKEUP_INTR_ENABLES_SET_2_RMSK                                                 0x7ffffff
#define HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_WAKEUP_INTR_ENABLES_SET_2_IN          \
        in_dword_masked(HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_WAKEUP_INTR_ENABLES_SET_2_ADDR, HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_WAKEUP_INTR_ENABLES_SET_2_RMSK)
#define HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_WAKEUP_INTR_ENABLES_SET_2_INM(m)      \
        in_dword_masked(HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_WAKEUP_INTR_ENABLES_SET_2_ADDR, m)
#define HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_WAKEUP_INTR_ENABLES_SET_2_OUT(v)      \
        out_dword(HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_WAKEUP_INTR_ENABLES_SET_2_ADDR,v)
#define HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_WAKEUP_INTR_ENABLES_SET_2_OUTM(m,v) \
        out_dword_masked_ns(HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_WAKEUP_INTR_ENABLES_SET_2_ADDR,m,v,HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_WAKEUP_INTR_ENABLES_SET_2_IN)
#define HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_WAKEUP_INTR_ENABLES_SET_2_WAHB_PWR_DWN_ACC_ERR_INT_BMSK                        0x4000000
#define HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_WAKEUP_INTR_ENABLES_SET_2_WAHB_PWR_DWN_ACC_ERR_INT_SHFT                             0x1a
#define HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_WAKEUP_INTR_ENABLES_SET_2_SPARE_3_BMSK                                         0x2000000
#define HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_WAKEUP_INTR_ENABLES_SET_2_SPARE_3_SHFT                                              0x19
#define HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_WAKEUP_INTR_ENABLES_SET_2_SPARE_2_BMSK                                         0x1000000
#define HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_WAKEUP_INTR_ENABLES_SET_2_SPARE_2_SHFT                                              0x18
#define HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_WAKEUP_INTR_ENABLES_SET_2_SPARE_1_BMSK                                          0x800000
#define HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_WAKEUP_INTR_ENABLES_SET_2_SPARE_1_SHFT                                              0x17
#define HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_WAKEUP_INTR_ENABLES_SET_2_SPARE_0_BMSK                                          0x400000
#define HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_WAKEUP_INTR_ENABLES_SET_2_SPARE_0_SHFT                                              0x16
#define HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_WAKEUP_INTR_ENABLES_SET_2_RFACTRL_WCMN_RFACTRL1_ERROR_INT_BMSK                  0x200000
#define HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_WAKEUP_INTR_ENABLES_SET_2_RFACTRL_WCMN_RFACTRL1_ERROR_INT_SHFT                      0x15
#define HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_WAKEUP_INTR_ENABLES_SET_2_RFACTRL_WCMN_RFACTRL0_ERROR_INT_BMSK                  0x100000
#define HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_WAKEUP_INTR_ENABLES_SET_2_RFACTRL_WCMN_RFACTRL0_ERROR_INT_SHFT                      0x14
#define HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_WAKEUP_INTR_ENABLES_SET_2_PMM_WCMN_WMAC2_IDLE_WAIT_TO_INT_BMSK                   0x80000
#define HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_WAKEUP_INTR_ENABLES_SET_2_PMM_WCMN_WMAC2_IDLE_WAIT_TO_INT_SHFT                      0x13
#define HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_WAKEUP_INTR_ENABLES_SET_2_PMM_WCMN_WMAC1_IDLE_WAIT_TO_INT_BMSK                   0x40000
#define HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_WAKEUP_INTR_ENABLES_SET_2_PMM_WCMN_WMAC1_IDLE_WAIT_TO_INT_SHFT                      0x12
#define HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_WAKEUP_INTR_ENABLES_SET_2_PMM_WCMN_PHY2_IDLE_WAIT_TO_INT_BMSK                    0x20000
#define HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_WAKEUP_INTR_ENABLES_SET_2_PMM_WCMN_PHY2_IDLE_WAIT_TO_INT_SHFT                       0x11
#define HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_WAKEUP_INTR_ENABLES_SET_2_PMM_WCMN_PHY1_IDLE_WAIT_TO_INT_BMSK                    0x10000
#define HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_WAKEUP_INTR_ENABLES_SET_2_PMM_WCMN_PHY1_IDLE_WAIT_TO_INT_SHFT                       0x10
#define HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_WAKEUP_INTR_ENABLES_SET_2_EXTERNAL_INTR_BMSK                                      0xffc0
#define HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_WAKEUP_INTR_ENABLES_SET_2_EXTERNAL_INTR_SHFT                                         0x6
#define HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_WAKEUP_INTR_ENABLES_SET_2_WFSS_DBG_INTR_BMSK                                        0x20
#define HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_WAKEUP_INTR_ENABLES_SET_2_WFSS_DBG_INTR_SHFT                                         0x5
#define HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_WAKEUP_INTR_ENABLES_SET_2_SW_SLP_TMR_INTR_BMSK                                      0x10
#define HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_WAKEUP_INTR_ENABLES_SET_2_SW_SLP_TMR_INTR_SHFT                                       0x4
#define HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_WAKEUP_INTR_ENABLES_SET_2_WLAN2_SLP_TMR_INTR_BMSK                                    0x8
#define HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_WAKEUP_INTR_ENABLES_SET_2_WLAN2_SLP_TMR_INTR_SHFT                                    0x3
#define HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_WAKEUP_INTR_ENABLES_SET_2_WLAN1_SLP_TMR_INTR_BMSK                                    0x4
#define HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_WAKEUP_INTR_ENABLES_SET_2_WLAN1_SLP_TMR_INTR_SHFT                                    0x2
#define HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_WAKEUP_INTR_ENABLES_SET_2_WAHB_APB_TSLV_INTR_BMSK                                    0x2
#define HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_WAKEUP_INTR_ENABLES_SET_2_WAHB_APB_TSLV_INTR_SHFT                                    0x1
#define HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_WAKEUP_INTR_ENABLES_SET_2_NOC_WCMN_INTR_BMSK                                         0x1
#define HWIO_WCSS_HM_A_WIFI_APB_3_A_WCMN_WAKEUP_INTR_ENABLES_SET_2_NOC_WCMN_INTR_SHFT                                         0x0


#endif /* __ABTIMEOUT_HWIO_WCSS_H__ */
