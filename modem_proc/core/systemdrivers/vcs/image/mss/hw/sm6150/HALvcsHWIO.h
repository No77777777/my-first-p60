#ifndef __HALVCSHWIO_H__
#define __HALVCSHWIO_H__
/*
===========================================================================
*/
/**
  @file HALvcsHWIO.h
  @brief Auto-generated HWIO interface include file.

  Reference chip release:
    SM6150 (Talos) [talos_v1.0_p3q3r72_mto]
 
  This file contains HWIO register definitions for the following modules:
    MSS_QDSP6V66SS_PUB

  'Include' filters applied: VERSION[MSS_QDSP6V66SS_PUB] LDO[MSS_QDSP6V66SS_PUB] QMC_SVS_CTL[MSS_QDSP6V66SS_PUB] BHS[MSS_QDSP6V66SS_PUB] 

  Generation parameters: 
  { u'filename': u'HALvcsHWIO.h',
    u'header': u'#include "msmhwiobase.h"',
    u'module-filter-exclude': { },
    u'module-filter-include': { u'MSS_QDSP6V66SS_PUB': [ u'VERSION',
                                                         u'LDO',
                                                         u'QMC_SVS_CTL',
                                                         u'BHS']},
    u'modules': [u'MSS_QDSP6V66SS_PUB'],
    u'output-offsets': True}
*/
/*
  ===========================================================================

  Copyright (c) 2018 Qualcomm Technologies, Inc.
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

  $Header: //components/rel/core.mpss/10.0/systemdrivers/vcs/image/mss/hw/sm6150/HALvcsHWIO.h#1 $
  $DateTime: 2019/04/24 00:03:26 $
  $Author: pwbldsvc $

  ===========================================================================
*/

#include "msmhwiobase.h"

/*----------------------------------------------------------------------------
 * MODULE: MSS_QDSP6V66SS_PUB
 *--------------------------------------------------------------------------*/

#define MSS_QDSP6V66SS_PUB_REG_BASE                                               (MODEM_TOP_BASE      + 0x00080000)
#define MSS_QDSP6V66SS_PUB_REG_BASE_SIZE                                          0x4040
#define MSS_QDSP6V66SS_PUB_REG_BASE_USED                                          0x2048
#define MSS_QDSP6V66SS_PUB_REG_BASE_OFFS                                          0x00080000

#define HWIO_MSS_QDSP6SS_VERSION_ADDR                                             (MSS_QDSP6V66SS_PUB_REG_BASE      + 0x00000000)
#define HWIO_MSS_QDSP6SS_VERSION_OFFS                                             (MSS_QDSP6V66SS_PUB_REG_BASE_OFFS + 0x00000000)
#define HWIO_MSS_QDSP6SS_VERSION_RMSK                                             0xffffffff
#define HWIO_MSS_QDSP6SS_VERSION_IN          \
        in_dword_masked(HWIO_MSS_QDSP6SS_VERSION_ADDR, HWIO_MSS_QDSP6SS_VERSION_RMSK)
#define HWIO_MSS_QDSP6SS_VERSION_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_VERSION_ADDR, m)
#define HWIO_MSS_QDSP6SS_VERSION_MAJOR_BMSK                                       0xf0000000
#define HWIO_MSS_QDSP6SS_VERSION_MAJOR_SHFT                                             0x1c
#define HWIO_MSS_QDSP6SS_VERSION_MINOR_BMSK                                        0xfff0000
#define HWIO_MSS_QDSP6SS_VERSION_MINOR_SHFT                                             0x10
#define HWIO_MSS_QDSP6SS_VERSION_STEP_BMSK                                            0xffff
#define HWIO_MSS_QDSP6SS_VERSION_STEP_SHFT                                               0x0

#define HWIO_MSS_QDSP6SS_CORE_BHS_CTL_ADDR                                        (MSS_QDSP6V66SS_PUB_REG_BASE      + 0x000000c0)
#define HWIO_MSS_QDSP6SS_CORE_BHS_CTL_OFFS                                        (MSS_QDSP6V66SS_PUB_REG_BASE_OFFS + 0x000000c0)
#define HWIO_MSS_QDSP6SS_CORE_BHS_CTL_RMSK                                               0xf
#define HWIO_MSS_QDSP6SS_CORE_BHS_CTL_IN          \
        in_dword_masked(HWIO_MSS_QDSP6SS_CORE_BHS_CTL_ADDR, HWIO_MSS_QDSP6SS_CORE_BHS_CTL_RMSK)
#define HWIO_MSS_QDSP6SS_CORE_BHS_CTL_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_CORE_BHS_CTL_ADDR, m)
#define HWIO_MSS_QDSP6SS_CORE_BHS_CTL_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_CORE_BHS_CTL_ADDR,v)
#define HWIO_MSS_QDSP6SS_CORE_BHS_CTL_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_CORE_BHS_CTL_ADDR,m,v,HWIO_MSS_QDSP6SS_CORE_BHS_CTL_IN)
#define HWIO_MSS_QDSP6SS_CORE_BHS_CTL_DRIVE_SEL_BMSK                                     0xf
#define HWIO_MSS_QDSP6SS_CORE_BHS_CTL_DRIVE_SEL_SHFT                                     0x0

#define HWIO_MSS_QDSP6SS_QMC_SVS_CTL_ADDR                                         (MSS_QDSP6V66SS_PUB_REG_BASE      + 0x00000130)
#define HWIO_MSS_QDSP6SS_QMC_SVS_CTL_OFFS                                         (MSS_QDSP6V66SS_PUB_REG_BASE_OFFS + 0x00000130)
#define HWIO_MSS_QDSP6SS_QMC_SVS_CTL_RMSK                                                0x3
#define HWIO_MSS_QDSP6SS_QMC_SVS_CTL_IN          \
        in_dword_masked(HWIO_MSS_QDSP6SS_QMC_SVS_CTL_ADDR, HWIO_MSS_QDSP6SS_QMC_SVS_CTL_RMSK)
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

#define HWIO_MSS_QDSP6SS_CORE_BHS_CFG_ADDR                                        (MSS_QDSP6V66SS_PUB_REG_BASE      + 0x00000410)
#define HWIO_MSS_QDSP6SS_CORE_BHS_CFG_OFFS                                        (MSS_QDSP6V66SS_PUB_REG_BASE_OFFS + 0x00000410)
#define HWIO_MSS_QDSP6SS_CORE_BHS_CFG_RMSK                                               0x1
#define HWIO_MSS_QDSP6SS_CORE_BHS_CFG_IN          \
        in_dword_masked(HWIO_MSS_QDSP6SS_CORE_BHS_CFG_ADDR, HWIO_MSS_QDSP6SS_CORE_BHS_CFG_RMSK)
#define HWIO_MSS_QDSP6SS_CORE_BHS_CFG_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_CORE_BHS_CFG_ADDR, m)
#define HWIO_MSS_QDSP6SS_CORE_BHS_CFG_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_CORE_BHS_CFG_ADDR,v)
#define HWIO_MSS_QDSP6SS_CORE_BHS_CFG_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_CORE_BHS_CFG_ADDR,m,v,HWIO_MSS_QDSP6SS_CORE_BHS_CFG_IN)
#define HWIO_MSS_QDSP6SS_CORE_BHS_CFG_BHS_ON_BMSK                                        0x1
#define HWIO_MSS_QDSP6SS_CORE_BHS_CFG_BHS_ON_SHFT                                        0x0

#define HWIO_MSS_QDSP6SS_CORE_BHS_CMD_ADDR                                        (MSS_QDSP6V66SS_PUB_REG_BASE      + 0x00000414)
#define HWIO_MSS_QDSP6SS_CORE_BHS_CMD_OFFS                                        (MSS_QDSP6V66SS_PUB_REG_BASE_OFFS + 0x00000414)
#define HWIO_MSS_QDSP6SS_CORE_BHS_CMD_RMSK                                               0x1
#define HWIO_MSS_QDSP6SS_CORE_BHS_CMD_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_CORE_BHS_CMD_ADDR,v)
#define HWIO_MSS_QDSP6SS_CORE_BHS_CMD_UPDATE_BMSK                                        0x1
#define HWIO_MSS_QDSP6SS_CORE_BHS_CMD_UPDATE_SHFT                                        0x0

#define HWIO_MSS_QDSP6SS_CORE_BHS_STATUS_ADDR                                     (MSS_QDSP6V66SS_PUB_REG_BASE      + 0x00000418)
#define HWIO_MSS_QDSP6SS_CORE_BHS_STATUS_OFFS                                     (MSS_QDSP6V66SS_PUB_REG_BASE_OFFS + 0x00000418)
#define HWIO_MSS_QDSP6SS_CORE_BHS_STATUS_RMSK                                            0x1
#define HWIO_MSS_QDSP6SS_CORE_BHS_STATUS_IN          \
        in_dword_masked(HWIO_MSS_QDSP6SS_CORE_BHS_STATUS_ADDR, HWIO_MSS_QDSP6SS_CORE_BHS_STATUS_RMSK)
#define HWIO_MSS_QDSP6SS_CORE_BHS_STATUS_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_CORE_BHS_STATUS_ADDR, m)
#define HWIO_MSS_QDSP6SS_CORE_BHS_STATUS_BHS_ON_BMSK                                     0x1
#define HWIO_MSS_QDSP6SS_CORE_BHS_STATUS_BHS_ON_SHFT                                     0x0

#define HWIO_MSS_QDSP6SS_LDO_CTL_CFG_ADDR                                         (MSS_QDSP6V66SS_PUB_REG_BASE      + 0x00000420)
#define HWIO_MSS_QDSP6SS_LDO_CTL_CFG_OFFS                                         (MSS_QDSP6V66SS_PUB_REG_BASE_OFFS + 0x00000420)
#define HWIO_MSS_QDSP6SS_LDO_CTL_CFG_RMSK                                                0x7
#define HWIO_MSS_QDSP6SS_LDO_CTL_CFG_IN          \
        in_dword_masked(HWIO_MSS_QDSP6SS_LDO_CTL_CFG_ADDR, HWIO_MSS_QDSP6SS_LDO_CTL_CFG_RMSK)
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

#define HWIO_MSS_QDSP6SS_LDO_CTL_CMD_ADDR                                         (MSS_QDSP6V66SS_PUB_REG_BASE      + 0x00000424)
#define HWIO_MSS_QDSP6SS_LDO_CTL_CMD_OFFS                                         (MSS_QDSP6V66SS_PUB_REG_BASE_OFFS + 0x00000424)
#define HWIO_MSS_QDSP6SS_LDO_CTL_CMD_RMSK                                                0x7
#define HWIO_MSS_QDSP6SS_LDO_CTL_CMD_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_LDO_CTL_CMD_ADDR,v)
#define HWIO_MSS_QDSP6SS_LDO_CTL_CMD_LDO_PWR_UP_BG_UPDATE_BMSK                           0x4
#define HWIO_MSS_QDSP6SS_LDO_CTL_CMD_LDO_PWR_UP_BG_UPDATE_SHFT                           0x2
#define HWIO_MSS_QDSP6SS_LDO_CTL_CMD_LDO_PWR_UP_UPDATE_BMSK                              0x2
#define HWIO_MSS_QDSP6SS_LDO_CTL_CMD_LDO_PWR_UP_UPDATE_SHFT                              0x1
#define HWIO_MSS_QDSP6SS_LDO_CTL_CMD_LDO_BYP_UPDATE_BMSK                                 0x1
#define HWIO_MSS_QDSP6SS_LDO_CTL_CMD_LDO_BYP_UPDATE_SHFT                                 0x0

#define HWIO_MSS_QDSP6SS_LDO_CTL_STATUS_ADDR                                      (MSS_QDSP6V66SS_PUB_REG_BASE      + 0x00000428)
#define HWIO_MSS_QDSP6SS_LDO_CTL_STATUS_OFFS                                      (MSS_QDSP6V66SS_PUB_REG_BASE_OFFS + 0x00000428)
#define HWIO_MSS_QDSP6SS_LDO_CTL_STATUS_RMSK                                             0x7
#define HWIO_MSS_QDSP6SS_LDO_CTL_STATUS_IN          \
        in_dword_masked(HWIO_MSS_QDSP6SS_LDO_CTL_STATUS_ADDR, HWIO_MSS_QDSP6SS_LDO_CTL_STATUS_RMSK)
#define HWIO_MSS_QDSP6SS_LDO_CTL_STATUS_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_LDO_CTL_STATUS_ADDR, m)
#define HWIO_MSS_QDSP6SS_LDO_CTL_STATUS_LDO_PWR_UP_BG_BMSK                               0x4
#define HWIO_MSS_QDSP6SS_LDO_CTL_STATUS_LDO_PWR_UP_BG_SHFT                               0x2
#define HWIO_MSS_QDSP6SS_LDO_CTL_STATUS_LDO_PWR_UP_BMSK                                  0x2
#define HWIO_MSS_QDSP6SS_LDO_CTL_STATUS_LDO_PWR_UP_SHFT                                  0x1
#define HWIO_MSS_QDSP6SS_LDO_CTL_STATUS_LDO_BYP_BMSK                                     0x1
#define HWIO_MSS_QDSP6SS_LDO_CTL_STATUS_LDO_BYP_SHFT                                     0x0

#define HWIO_MSS_QDSP6SS_BHS_TEST_ADDR                                            (MSS_QDSP6V66SS_PUB_REG_BASE      + 0x00002014)
#define HWIO_MSS_QDSP6SS_BHS_TEST_OFFS                                            (MSS_QDSP6V66SS_PUB_REG_BASE_OFFS + 0x00002014)
#define HWIO_MSS_QDSP6SS_BHS_TEST_RMSK                                                   0x1
#define HWIO_MSS_QDSP6SS_BHS_TEST_IN          \
        in_dword_masked(HWIO_MSS_QDSP6SS_BHS_TEST_ADDR, HWIO_MSS_QDSP6SS_BHS_TEST_RMSK)
#define HWIO_MSS_QDSP6SS_BHS_TEST_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_BHS_TEST_ADDR, m)
#define HWIO_MSS_QDSP6SS_BHS_TEST_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_BHS_TEST_ADDR,v)
#define HWIO_MSS_QDSP6SS_BHS_TEST_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_QDSP6SS_BHS_TEST_ADDR,m,v,HWIO_MSS_QDSP6SS_BHS_TEST_IN)
#define HWIO_MSS_QDSP6SS_BHS_TEST_INSTANT_ON_BMSK                                        0x1
#define HWIO_MSS_QDSP6SS_BHS_TEST_INSTANT_ON_SHFT                                        0x0


#endif /* __HALVCSHWIO_H__ */
