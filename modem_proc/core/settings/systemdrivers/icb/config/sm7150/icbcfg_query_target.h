#ifndef __ICBCFG_QUERY_TARGET_H__
#define __ICBCFG_QUERY_TARGET_H__
/*
===========================================================================
*/
/**
  @file icbcfg_query_target.h
  @brief Auto-generated HWIO interface include file.

  Reference chip release:
    SM7150 (Moorea) [moorea_v1.0_p3q2r52]
 
  This file contains HWIO register definitions for the following modules:
    LLCC0_BEAC
    LLCC1_BEAC

  'Include' filters applied: LLCC_BEAC_ADDR_TRANSLATOR_CFG[LLCC0_BEAC] LLCC_BEAC_ADDR_TRANSLATOR_CFG[LLCC1_BEAC] 

  Generation parameters: 
  { u'filename': u'icbcfg_query_target.h',
    u'header': u'#include "msmhwiobase.h"',
    u'module-filter-exclude': { },
    u'module-filter-include': { u'LLCC0_BEAC': [ u'LLCC_BEAC_ADDR_TRANSLATOR_CFG'],
                                u'LLCC1_BEAC': [ u'LLCC_BEAC_ADDR_TRANSLATOR_CFG']},
    u'modules': [u'LLCC0_BEAC', u'LLCC1_BEAC'],
    u'output-fvals': True,
    u'output-offsets': True,
    u'output-phys': True}
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

  $Header: //components/rel/core.mpss/10.0/settings/systemdrivers/icb/config/sm7150/icbcfg_query_target.h#1 $
  $DateTime: 2019/04/24 00:03:26 $
  $Author: pwbldsvc $

  ===========================================================================
*/
#include "msmhwiobase.h"

/*----------------------------------------------------------------------------
 * MODULE: LLCC0_BEAC
 *--------------------------------------------------------------------------*/

#define LLCC0_BEAC_REG_BASE                                                                  (DDR_SS_BASE      + 0x00248000)


#define HWIO_LLCC0_LLCC_BEAC_ADDR_TRANSLATOR_CFG_ADDR                                        (LLCC0_BEAC_REG_BASE      + 0x00000080)


/*----------------------------------------------------------------------------
 * MODULE: LLCC1_BEAC
 *--------------------------------------------------------------------------*/

#define LLCC1_BEAC_REG_BASE                                                                  (DDR_SS_BASE      + 0x002c8000)

#define HWIO_LLCC1_LLCC_BEAC_ADDR_TRANSLATOR_CFG_ADDR                                        (LLCC1_BEAC_REG_BASE      + 0x00000080)


#endif /* __ICBCFG_QUERY_TARGET_H__ */
