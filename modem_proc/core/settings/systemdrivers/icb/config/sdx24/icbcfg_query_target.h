#ifndef __ICBCFG_QUERY_TARGET_H__
#define __ICBCFG_QUERY_TARGET_H__
/*
===========================================================================
*/
/**
  @file icbcfg_query_target.h
  @brief Auto-generated HWIO interface include file.

  Reference chip release:
    SDM845 (Napali) [napali_v1.0_p1q0r25.1.2_F0_ECO]
 
  This file contains HWIO register definitions for the following modules:
    MEMNOC_MEM_NOC

  'Include' filters applied: LLCC0_ADDR_TRANSLATOR_SWID_LOW[MEMNOC_MEM_NOC] 
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

  $Header: //components/rel/core.mpss/10.0/settings/systemdrivers/icb/config/sdx24/icbcfg_query_target.h#1 $
  $DateTime: 2019/04/24 00:03:26 $
  $Author: pwbldsvc $

  ===========================================================================
*/

#include "msmhwiobase.h"

/*----------------------------------------------------------------------------
 * MODULE: MEMNOC_MEM_NOC
 *--------------------------------------------------------------------------*/

#define MEMNOC_MEM_NOC_REG_BASE                                                                             (DDR_SS_BASE      + 0x00280000)

#define HWIO_MEMNOC_LLCC0_ADDR_TRANSLATOR_SWID_LOW_ADDR                                                     (MEMNOC_MEM_NOC_REG_BASE      + 0x00023000)
#define HWIO_MEMNOC_LLCC0_ADDR_TRANSLATOR_SWID_LOW_RMSK                                                       0xffffff
#define HWIO_MEMNOC_LLCC0_ADDR_TRANSLATOR_SWID_LOW_IN          \
        in_dword_masked(HWIO_MEMNOC_LLCC0_ADDR_TRANSLATOR_SWID_LOW_ADDR, HWIO_MEMNOC_LLCC0_ADDR_TRANSLATOR_SWID_LOW_RMSK)
#define HWIO_MEMNOC_LLCC0_ADDR_TRANSLATOR_SWID_LOW_INM(m)      \
        in_dword_masked(HWIO_MEMNOC_LLCC0_ADDR_TRANSLATOR_SWID_LOW_ADDR, m)
#define HWIO_MEMNOC_LLCC0_ADDR_TRANSLATOR_SWID_LOW_UNITTYPEID_BMSK                                            0xff0000
#define HWIO_MEMNOC_LLCC0_ADDR_TRANSLATOR_SWID_LOW_UNITTYPEID_SHFT                                                0x10
#define HWIO_MEMNOC_LLCC0_ADDR_TRANSLATOR_SWID_LOW_UNITCONFID_BMSK                                              0xffff
#define HWIO_MEMNOC_LLCC0_ADDR_TRANSLATOR_SWID_LOW_UNITCONFID_SHFT                                                 0x0

#define HWIO_MEMNOC_LLCC1_ADDR_TRANSLATOR_SWID_LOW_ADDR                                                     (MEMNOC_MEM_NOC_REG_BASE      + 0x00023080)
#define HWIO_MEMNOC_LLCC1_ADDR_TRANSLATOR_SWID_LOW_PHYS                                                     (MEMNOC_MEM_NOC_REG_BASE_PHYS + 0x00023080)
#define HWIO_MEMNOC_LLCC1_ADDR_TRANSLATOR_SWID_LOW_RMSK                                                       0xffffff
#define HWIO_MEMNOC_LLCC1_ADDR_TRANSLATOR_SWID_LOW_IN          \
        in_dword_masked(HWIO_MEMNOC_LLCC1_ADDR_TRANSLATOR_SWID_LOW_ADDR, HWIO_MEMNOC_LLCC1_ADDR_TRANSLATOR_SWID_LOW_RMSK)
#define HWIO_MEMNOC_LLCC1_ADDR_TRANSLATOR_SWID_LOW_INM(m)      \
        in_dword_masked(HWIO_MEMNOC_LLCC1_ADDR_TRANSLATOR_SWID_LOW_ADDR, m)
#define HWIO_MEMNOC_LLCC1_ADDR_TRANSLATOR_SWID_LOW_UNITTYPEID_BMSK                                            0xff0000
#define HWIO_MEMNOC_LLCC1_ADDR_TRANSLATOR_SWID_LOW_UNITTYPEID_SHFT                                                0x10
#define HWIO_MEMNOC_LLCC1_ADDR_TRANSLATOR_SWID_LOW_UNITCONFID_BMSK                                              0xffff
#define HWIO_MEMNOC_LLCC1_ADDR_TRANSLATOR_SWID_LOW_UNITCONFID_SHFT                                                 0x0

#define HWIO_MEMNOC_LLCC2_ADDR_TRANSLATOR_SWID_LOW_ADDR                                                     (MEMNOC_MEM_NOC_REG_BASE      + 0x00023100)
#define HWIO_MEMNOC_LLCC2_ADDR_TRANSLATOR_SWID_LOW_PHYS                                                     (MEMNOC_MEM_NOC_REG_BASE_PHYS + 0x00023100)
#define HWIO_MEMNOC_LLCC2_ADDR_TRANSLATOR_SWID_LOW_RMSK                                                       0xffffff
#define HWIO_MEMNOC_LLCC2_ADDR_TRANSLATOR_SWID_LOW_IN          \
        in_dword_masked(HWIO_MEMNOC_LLCC2_ADDR_TRANSLATOR_SWID_LOW_ADDR, HWIO_MEMNOC_LLCC2_ADDR_TRANSLATOR_SWID_LOW_RMSK)
#define HWIO_MEMNOC_LLCC2_ADDR_TRANSLATOR_SWID_LOW_INM(m)      \
        in_dword_masked(HWIO_MEMNOC_LLCC2_ADDR_TRANSLATOR_SWID_LOW_ADDR, m)
#define HWIO_MEMNOC_LLCC2_ADDR_TRANSLATOR_SWID_LOW_UNITTYPEID_BMSK                                            0xff0000
#define HWIO_MEMNOC_LLCC2_ADDR_TRANSLATOR_SWID_LOW_UNITTYPEID_SHFT                                                0x10
#define HWIO_MEMNOC_LLCC2_ADDR_TRANSLATOR_SWID_LOW_UNITCONFID_BMSK                                              0xffff
#define HWIO_MEMNOC_LLCC2_ADDR_TRANSLATOR_SWID_LOW_UNITCONFID_SHFT                                                 0x0

#define HWIO_MEMNOC_LLCC3_ADDR_TRANSLATOR_SWID_LOW_ADDR                                                     (MEMNOC_MEM_NOC_REG_BASE      + 0x00023180)
#define HWIO_MEMNOC_LLCC3_ADDR_TRANSLATOR_SWID_LOW_PHYS                                                     (MEMNOC_MEM_NOC_REG_BASE_PHYS + 0x00023180)
#define HWIO_MEMNOC_LLCC3_ADDR_TRANSLATOR_SWID_LOW_RMSK                                                       0xffffff
#define HWIO_MEMNOC_LLCC3_ADDR_TRANSLATOR_SWID_LOW_IN          \
        in_dword_masked(HWIO_MEMNOC_LLCC3_ADDR_TRANSLATOR_SWID_LOW_ADDR, HWIO_MEMNOC_LLCC3_ADDR_TRANSLATOR_SWID_LOW_RMSK)
#define HWIO_MEMNOC_LLCC3_ADDR_TRANSLATOR_SWID_LOW_INM(m)      \
        in_dword_masked(HWIO_MEMNOC_LLCC3_ADDR_TRANSLATOR_SWID_LOW_ADDR, m)
#define HWIO_MEMNOC_LLCC3_ADDR_TRANSLATOR_SWID_LOW_UNITTYPEID_BMSK                                            0xff0000
#define HWIO_MEMNOC_LLCC3_ADDR_TRANSLATOR_SWID_LOW_UNITTYPEID_SHFT                                                0x10
#define HWIO_MEMNOC_LLCC3_ADDR_TRANSLATOR_SWID_LOW_UNITCONFID_BMSK                                              0xffff
#define HWIO_MEMNOC_LLCC3_ADDR_TRANSLATOR_SWID_LOW_UNITCONFID_SHFT                                                 0x0


#endif /* __ICBCFG_QUERY_TARGET_H__ */
