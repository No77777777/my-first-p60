#ifndef __HAL_UIMAGE_RPM_HWIO_H__
#define __HAL_UIMAGE_RPM_HWIO_H__
/*
===========================================================================
*/
/**
  @file HAL_uimage_rpm_hwio.h
  @brief Auto-generated HWIO interface include file.

  Reference chip release:
    Nicobar [nicobar_v1.0_p3q3r104_BTO]
 
  This file contains HWIO register definitions for the following modules:
    MSS_QDSP6V66SS_CSR

  'Include' filters applied: MSS_QDSP6SS_IPC[MSS_QDSP6V66SS_CSR] 
  'Exclude' filters applied: RESERVED DUMMY[MSS_QDSP6V66SS_CSR] 

  Generation parameters: 
  { u'filename': u'HAL_uimage_rpm_hwio.h',
    u'header': u'#include "msmhwiobase.h"',
    u'module-filter-exclude': { u'MSS_QDSP6V66SS_CSR': [u'RESERVED DUMMY']},
    u'module-filter-include': { u'MSS_QDSP6V66SS_CSR': [u'MSS_QDSP6SS_IPC']},
    u'modules': [u'MSS_QDSP6V66SS_CSR']}
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

  $Header: //components/rel/core.mpss/10.0/power/uimage_rpm/src/client_glink/divar/HAL_uimage_rpm_hwio.h#1 $
  $DateTime: 2021/04/06 16:32:41 $
  $Author: pwbldsvc $

  ===========================================================================
*/

#include "msmhwiobase.h"

/*----------------------------------------------------------------------------
 * MODULE: MSS_QDSP6V66SS_CSR
 *--------------------------------------------------------------------------*/

#define MSS_QDSP6V66SS_CSR_REG_BASE                                                      (MODEM_TOP_BASE      + 0x00100000)
#define MSS_QDSP6V66SS_CSR_REG_BASE_SIZE                                                 0xd000
#define MSS_QDSP6V66SS_CSR_REG_BASE_USED                                                 0xc248

#define HWIO_MSS_QDSP6SS_IPC_ADDR                                                        (MSS_QDSP6V66SS_CSR_REG_BASE      + 0x00008000)
#define HWIO_MSS_QDSP6SS_IPC_RMSK                                                        0xffffffff
#define HWIO_MSS_QDSP6SS_IPC_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_IPC_ADDR,v)
#define HWIO_MSS_QDSP6SS_IPC_INTR_BMSK                                                   0xffffffff
#define HWIO_MSS_QDSP6SS_IPC_INTR_SHFT                                                          0x0

#define HWIO_MSS_QDSP6SS_IPC1_ADDR                                                       (MSS_QDSP6V66SS_CSR_REG_BASE      + 0x00008004)
#define HWIO_MSS_QDSP6SS_IPC1_RMSK                                                       0xffffffff
#define HWIO_MSS_QDSP6SS_IPC1_OUT(v)      \
        out_dword(HWIO_MSS_QDSP6SS_IPC1_ADDR,v)
#define HWIO_MSS_QDSP6SS_IPC1_INTR_BMSK                                                  0xffffffff
#define HWIO_MSS_QDSP6SS_IPC1_INTR_SHFT                                                         0x0


#endif /* __HAL_UIMAGE_RPM_HWIO_H__ */
