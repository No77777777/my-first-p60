#ifndef __ICBCFG_QUERY_TARGET_H__
#define __ICBCFG_QUERY_TARGET_H__
/*
===========================================================================
*/
/**
  @file icbcfg_query_target.h
  @brief Auto-generated HWIO interface include file.

  Reference chip release:
    SDM670 (Warlock) [napaliq_v1.0_p3q2r97]
 
  This file contains HWIO register definitions for the following modules:
    LLCC0_BEAC
    LLCC1_BEAC
    LLCC2_BEAC
    LLCC3_BEAC

  'Include' filters applied: TRANSLATOR[LLCC3_BEAC] TRANSLATOR[LLCC0_BEAC] TRANSLATOR[LLCC2_BEAC] TRANSLATOR[LLCC1_BEAC] 
  'Exclude' filters applied: 
*/
/*
  ===========================================================================

  Copyright (c) 2017 Qualcomm Technologies, Inc.
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

  $Header: //components/rel/core.mpss/10.0/settings/systemdrivers/icb/config/sdm670/icbcfg_query_target.h#1 $
  $DateTime: 2019/04/24 00:03:26 $
  $Author: pwbldsvc $

  ===========================================================================
*/

#include "msmhwiobase.h"

/*----------------------------------------------------------------------------
 * MODULE: LLCC0_BEAC
 *--------------------------------------------------------------------------*/

#define LLCC0_BEAC_REG_BASE                                                                   (DDR_SS_BASE      + 0x00048000)

#define HWIO_LLCC0_LLCC_BEAC_ADDR_TRANSLATOR_CFG_ADDR                                         (LLCC0_BEAC_REG_BASE      + 0x00000080)
#define HWIO_LLCC0_LLCC_BEAC_ADDR_TRANSLATOR_CFG_RMSK                                              0x3f1
#define HWIO_LLCC0_LLCC_BEAC_ADDR_TRANSLATOR_CFG_IN          \
        in_dword_masked(HWIO_LLCC0_LLCC_BEAC_ADDR_TRANSLATOR_CFG_ADDR, HWIO_LLCC0_LLCC_BEAC_ADDR_TRANSLATOR_CFG_RMSK)
#define HWIO_LLCC0_LLCC_BEAC_ADDR_TRANSLATOR_CFG_INM(m)      \
        in_dword_masked(HWIO_LLCC0_LLCC_BEAC_ADDR_TRANSLATOR_CFG_ADDR, m)
#define HWIO_LLCC0_LLCC_BEAC_ADDR_TRANSLATOR_CFG_OUT(v)      \
        out_dword(HWIO_LLCC0_LLCC_BEAC_ADDR_TRANSLATOR_CFG_ADDR,v)
#define HWIO_LLCC0_LLCC_BEAC_ADDR_TRANSLATOR_CFG_OUTM(m,v) \
        out_dword_masked_ns(HWIO_LLCC0_LLCC_BEAC_ADDR_TRANSLATOR_CFG_ADDR,m,v,HWIO_LLCC0_LLCC_BEAC_ADDR_TRANSLATOR_CFG_IN)
#define HWIO_LLCC0_LLCC_BEAC_ADDR_TRANSLATOR_CFG_ERROR_BMSK                                        0x3f0
#define HWIO_LLCC0_LLCC_BEAC_ADDR_TRANSLATOR_CFG_ERROR_SHFT                                          0x4
#define HWIO_LLCC0_LLCC_BEAC_ADDR_TRANSLATOR_CFG_ENABLE_BMSK                                         0x1
#define HWIO_LLCC0_LLCC_BEAC_ADDR_TRANSLATOR_CFG_ENABLE_SHFT                                         0x0

/*----------------------------------------------------------------------------
 * MODULE: LLCC1_BEAC
 *--------------------------------------------------------------------------*/

#define LLCC1_BEAC_REG_BASE                                                                   (DDR_SS_BASE      + 0x000c8000)

#define HWIO_LLCC1_LLCC_BEAC_ADDR_TRANSLATOR_CFG_ADDR                                         (LLCC1_BEAC_REG_BASE      + 0x00000080)
#define HWIO_LLCC1_LLCC_BEAC_ADDR_TRANSLATOR_CFG_RMSK                                              0x3f1
#define HWIO_LLCC1_LLCC_BEAC_ADDR_TRANSLATOR_CFG_IN          \
        in_dword_masked(HWIO_LLCC1_LLCC_BEAC_ADDR_TRANSLATOR_CFG_ADDR, HWIO_LLCC1_LLCC_BEAC_ADDR_TRANSLATOR_CFG_RMSK)
#define HWIO_LLCC1_LLCC_BEAC_ADDR_TRANSLATOR_CFG_INM(m)      \
        in_dword_masked(HWIO_LLCC1_LLCC_BEAC_ADDR_TRANSLATOR_CFG_ADDR, m)
#define HWIO_LLCC1_LLCC_BEAC_ADDR_TRANSLATOR_CFG_OUT(v)      \
        out_dword(HWIO_LLCC1_LLCC_BEAC_ADDR_TRANSLATOR_CFG_ADDR,v)
#define HWIO_LLCC1_LLCC_BEAC_ADDR_TRANSLATOR_CFG_OUTM(m,v) \
        out_dword_masked_ns(HWIO_LLCC1_LLCC_BEAC_ADDR_TRANSLATOR_CFG_ADDR,m,v,HWIO_LLCC1_LLCC_BEAC_ADDR_TRANSLATOR_CFG_IN)
#define HWIO_LLCC1_LLCC_BEAC_ADDR_TRANSLATOR_CFG_ERROR_BMSK                                        0x3f0
#define HWIO_LLCC1_LLCC_BEAC_ADDR_TRANSLATOR_CFG_ERROR_SHFT                                          0x4
#define HWIO_LLCC1_LLCC_BEAC_ADDR_TRANSLATOR_CFG_ENABLE_BMSK                                         0x1
#define HWIO_LLCC1_LLCC_BEAC_ADDR_TRANSLATOR_CFG_ENABLE_SHFT                                         0x0


#endif /* __ICBCFG_QUERY_TARGET_H__ */
