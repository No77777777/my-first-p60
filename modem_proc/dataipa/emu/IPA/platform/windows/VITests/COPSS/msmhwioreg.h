#ifndef __MSMHWIOREG_H__
#define __MSMHWIOREG_H__
/*
===========================================================================
*/
/**
  @file msmhwioreg.h
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

  $Header: //components/rel/dataipa.mpss/3.2/emu/IPA/platform/windows/VITests/COPSS/msmhwioreg.h#1 $
  $DateTime: 2019/07/05 03:48:36 $
  $Author: pwbldsvc $

  ===========================================================================
*/

#include "msmhwioreg_gen_emu.h"
#include "msmhwioreg_dut.h"

#define GSIH_SYM(hwiosym)        GSI_TOP_##hwiosym

/* Define the gsi hal to use local ipa msmhwioreg.h */
#define GSI_MSMHWIOREG_H         "msmhwioreg_ipa.h"

#define IPAH_SYM(hwiosym)        IPA_##hwiosym

/*
 * @brief   Memory range for PCIe performance testing.
*/
#define PCIE_RANGE_START_OFFSET            0x010000
#define PCIE_RANGE_SIZE                    0xff0000
#define PCIE_RANGE_START_ADDRESS           (DUT1_BASE_PTR + PCIE_RANGE_START_OFFSET)
#define PCIE_RANGE_START_ADDRESS_PHYS      (DUT1_BASE_PHYS + PCIE_RANGE_START_OFFSET)
#define PCIE_RANGE_END_ADDRESS             (PCIE_RANGE_START_ADDRESS + PCIE_RANGE_SIZE - 1)

#endif /* __MSMHWIOREG_H__ */
