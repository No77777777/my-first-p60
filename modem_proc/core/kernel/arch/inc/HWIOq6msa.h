#ifndef __HWIOQ6MSA_H__
#define __HWIOQ6MSA_H__
/*
===========================================================================
*/
/**
  @file HWIOq6msa.h
  @brief Auto-generated HWIO interface include file.

  This file contains HWIO register definitions for the following modules:
    MPSS_PERPH

  'Include' filters applied: MSS_MSA[MPSS_PERPH] 
  'Exclude' filters applied: RESERVED DUMMY 
*/
/*
  ===========================================================================

  Copyright (c) 2014 Qualcomm Technologies Incorporated.
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

  $Header: //components/rel/core.mpss/10.0/kernel/arch/inc/HWIOq6msa.h#2 $
  $DateTime: 2019/05/20 08:07:41 $
  $Author: pwbldsvc $

  ===========================================================================
*/
#include "msmhwiobase.h"
/*----------------------------------------------------------------------------
 * MODULE: MPSS_PERPH
 *--------------------------------------------------------------------------*/

#define MPSS_PERPH_REG_BASE                                                   (MSS_TOP_BASE      + 0x001a8000)

#define HWIO_MSS_MSA_CRYPTO_ADDR                                              (MPSS_PERPH_REG_BASE      + 0x0000000c)
#define HWIO_MSS_MSA_CRYPTO_RMSK                                                     0xf
#define HWIO_MSS_MSA_CRYPTO_POR                                               0x0000000f
#define HWIO_MSS_MSA_CRYPTO_IN          \
        in_dword_masked(HWIO_MSS_MSA_CRYPTO_ADDR, HWIO_MSS_MSA_CRYPTO_RMSK)
#define HWIO_MSS_MSA_CRYPTO_INM(m)      \
        in_dword_masked(HWIO_MSS_MSA_CRYPTO_ADDR, m)
#define HWIO_MSS_MSA_CRYPTO_OUT(v)      \
        out_dword(HWIO_MSS_MSA_CRYPTO_ADDR,v)
#define HWIO_MSS_MSA_CRYPTO_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_MSA_CRYPTO_ADDR,m,v,HWIO_MSS_MSA_CRYPTO_IN)
#define HWIO_MSS_MSA_CRYPTO_MSA_CRYPTO_BMSK                                          0xf
#define HWIO_MSS_MSA_CRYPTO_MSA_CRYPTO_SHFT                                          0x0

#define HWIO_MSS_MSA_ADDR                                                     (MPSS_PERPH_REG_BASE      + 0x00000010)
#define HWIO_MSS_MSA_RMSK                                                            0xf
#define HWIO_MSS_MSA_POR                                                      0x00000004
#define HWIO_MSS_MSA_IN          \
        in_dword_masked(HWIO_MSS_MSA_ADDR, HWIO_MSS_MSA_RMSK)
#define HWIO_MSS_MSA_INM(m)      \
        in_dword_masked(HWIO_MSS_MSA_ADDR, m)
#define HWIO_MSS_MSA_OUT(v)      \
        out_dword(HWIO_MSS_MSA_ADDR,v)
#define HWIO_MSS_MSA_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_MSA_ADDR,m,v,HWIO_MSS_MSA_IN)
#define HWIO_MSS_MSA_CONFIG_UNLOCK2_BMSK                                             0x8
#define HWIO_MSS_MSA_CONFIG_UNLOCK2_SHFT                                             0x3
#define HWIO_MSS_MSA_CONFIG_UNLOCK2_CNOC_NON_MSA_MASTERS_CANNOT_ACCESS_PARTITION3_OF_MSS_ADDRESS_SPACE_FVAL        0x0
#define HWIO_MSS_MSA_CONFIG_UNLOCK2_CNOC_NON_MSA_MASTERS_CAN_ACCESS_PARTITION3_OF_MSS_ADDRESS_SPACE_FVAL        0x1
#define HWIO_MSS_MSA_FORCE_Q6_MSA_BMSK                                               0x4
#define HWIO_MSS_MSA_FORCE_Q6_MSA_SHFT                                               0x2
#define HWIO_MSS_MSA_FORCE_Q6_MSA_FORCE_Q6_MSA_NO_FVAL                               0x0
#define HWIO_MSS_MSA_FORCE_Q6_MSA_FORCE_Q6_MSA_YES_FVAL                              0x1
#define HWIO_MSS_MSA_MBA_OK_BMSK                                                     0x2
#define HWIO_MSS_MSA_MBA_OK_SHFT                                                     0x1
#define HWIO_MSS_MSA_MBA_OK_MBA_OK_NO_FVAL                                           0x0
#define HWIO_MSS_MSA_MBA_OK_MBA_OK_YES_FVAL                                          0x1
#define HWIO_MSS_MSA_CONFIG_LOCK_BMSK                                                0x1
#define HWIO_MSS_MSA_CONFIG_LOCK_SHFT                                                0x0
#define HWIO_MSS_MSA_CONFIG_LOCK_CONFIG_NOT_LOCK_FVAL                                0x0
#define HWIO_MSS_MSA_CONFIG_LOCK_CONFIG_LOCK_FVAL                                    0x1


#endif /* __HWIOQ6MSA_H__ */
