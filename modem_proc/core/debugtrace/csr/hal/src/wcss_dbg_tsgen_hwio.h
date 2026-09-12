#ifndef __WCSS_DBG_TSGEN_HWIO_H__
#define __WCSS_DBG_TSGEN_HWIO_H__
/*
===========================================================================
*/
/**
  @file wcss_dbg_tsgen_hwio.h
  @brief Auto-generated HWIO interface include file.

  Reference chip release:
    MSM8998 (Nazgul) [nazgul_v1.0_p3q2r28.0]
 
  This file contains HWIO register definitions for the following modules:
    WCSS_HM_A_WCSS_DBG_TSGEN_TSGEN_CXTSGEN

  'Include' filters applied: 
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
*/
#include "HALhwio.h"


/*----------------------------------------------------------------------------
 * MODULE: WCSS_HM_A_WCSS_DBG_TSGEN_TSGEN_CXTSGEN
 *--------------------------------------------------------------------------*/

extern uint32 WCSS_HM_A_WCSS_DBG_TSGEN_TSGEN_CXTSGEN_REG_BASE;

#define HWIO_WCSS_HM_A_WCSS_DBG_TSGEN_CNTCR_ADDR                       (WCSS_HM_A_WCSS_DBG_TSGEN_TSGEN_CXTSGEN_REG_BASE      + 0x00000000)
#define HWIO_WCSS_HM_A_WCSS_DBG_TSGEN_CNTCR_RMSK                              0x3
#define HWIO_WCSS_HM_A_WCSS_DBG_TSGEN_CNTCR_IN          \
        in_dword_masked(HWIO_WCSS_HM_A_WCSS_DBG_TSGEN_CNTCR_ADDR, HWIO_WCSS_HM_A_WCSS_DBG_TSGEN_CNTCR_RMSK)
#define HWIO_WCSS_HM_A_WCSS_DBG_TSGEN_CNTCR_INM(m)      \
        in_dword_masked(HWIO_WCSS_HM_A_WCSS_DBG_TSGEN_CNTCR_ADDR, m)
#define HWIO_WCSS_HM_A_WCSS_DBG_TSGEN_CNTCR_OUT(v)      \
        out_dword(HWIO_WCSS_HM_A_WCSS_DBG_TSGEN_CNTCR_ADDR,v)
#define HWIO_WCSS_HM_A_WCSS_DBG_TSGEN_CNTCR_OUTM(m,v) \
        out_dword_masked_ns(HWIO_WCSS_HM_A_WCSS_DBG_TSGEN_CNTCR_ADDR,m,v,HWIO_WCSS_HM_A_WCSS_DBG_TSGEN_CNTCR_IN)
#define HWIO_WCSS_HM_A_WCSS_DBG_TSGEN_CNTCR_CNTCR_HDBG_BMSK                   0x2
#define HWIO_WCSS_HM_A_WCSS_DBG_TSGEN_CNTCR_CNTCR_HDBG_SHFT                   0x1
#define HWIO_WCSS_HM_A_WCSS_DBG_TSGEN_CNTCR_CNTCR_EN_BMSK                     0x1
#define HWIO_WCSS_HM_A_WCSS_DBG_TSGEN_CNTCR_CNTCR_EN_SHFT                     0x0

#define HWIO_WCSS_HM_A_WCSS_DBG_TSGEN_CNTSR_ADDR                       (WCSS_HM_A_WCSS_DBG_TSGEN_TSGEN_CXTSGEN_REG_BASE      + 0x00000004)
#define HWIO_WCSS_HM_A_WCSS_DBG_TSGEN_CNTSR_RMSK                              0x2
#define HWIO_WCSS_HM_A_WCSS_DBG_TSGEN_CNTSR_IN          \
        in_dword_masked(HWIO_WCSS_HM_A_WCSS_DBG_TSGEN_CNTSR_ADDR, HWIO_WCSS_HM_A_WCSS_DBG_TSGEN_CNTSR_RMSK)
#define HWIO_WCSS_HM_A_WCSS_DBG_TSGEN_CNTSR_INM(m)      \
        in_dword_masked(HWIO_WCSS_HM_A_WCSS_DBG_TSGEN_CNTSR_ADDR, m)
#define HWIO_WCSS_HM_A_WCSS_DBG_TSGEN_CNTSR_CNTSR_DBGH_BMSK                   0x2
#define HWIO_WCSS_HM_A_WCSS_DBG_TSGEN_CNTSR_CNTSR_DBGH_SHFT                   0x1

#define HWIO_WCSS_HM_A_WCSS_DBG_TSGEN_CNTCVL_ADDR                      (WCSS_HM_A_WCSS_DBG_TSGEN_TSGEN_CXTSGEN_REG_BASE      + 0x00000008)
#define HWIO_WCSS_HM_A_WCSS_DBG_TSGEN_CNTCVL_RMSK                      0xffffffff
#define HWIO_WCSS_HM_A_WCSS_DBG_TSGEN_CNTCVL_IN          \
        in_dword_masked(HWIO_WCSS_HM_A_WCSS_DBG_TSGEN_CNTCVL_ADDR, HWIO_WCSS_HM_A_WCSS_DBG_TSGEN_CNTCVL_RMSK)
#define HWIO_WCSS_HM_A_WCSS_DBG_TSGEN_CNTCVL_INM(m)      \
        in_dword_masked(HWIO_WCSS_HM_A_WCSS_DBG_TSGEN_CNTCVL_ADDR, m)
#define HWIO_WCSS_HM_A_WCSS_DBG_TSGEN_CNTCVL_OUT(v)      \
        out_dword(HWIO_WCSS_HM_A_WCSS_DBG_TSGEN_CNTCVL_ADDR,v)
#define HWIO_WCSS_HM_A_WCSS_DBG_TSGEN_CNTCVL_OUTM(m,v) \
        out_dword_masked_ns(HWIO_WCSS_HM_A_WCSS_DBG_TSGEN_CNTCVL_ADDR,m,v,HWIO_WCSS_HM_A_WCSS_DBG_TSGEN_CNTCVL_IN)
#define HWIO_WCSS_HM_A_WCSS_DBG_TSGEN_CNTCVL_CNTCVL_L_32_BMSK          0xffffffff
#define HWIO_WCSS_HM_A_WCSS_DBG_TSGEN_CNTCVL_CNTCVL_L_32_SHFT                 0x0

#define HWIO_WCSS_HM_A_WCSS_DBG_TSGEN_CNTCVU_ADDR                      (WCSS_HM_A_WCSS_DBG_TSGEN_TSGEN_CXTSGEN_REG_BASE      + 0x0000000c)
#define HWIO_WCSS_HM_A_WCSS_DBG_TSGEN_CNTCVU_RMSK                      0xffffffff
#define HWIO_WCSS_HM_A_WCSS_DBG_TSGEN_CNTCVU_IN          \
        in_dword_masked(HWIO_WCSS_HM_A_WCSS_DBG_TSGEN_CNTCVU_ADDR, HWIO_WCSS_HM_A_WCSS_DBG_TSGEN_CNTCVU_RMSK)
#define HWIO_WCSS_HM_A_WCSS_DBG_TSGEN_CNTCVU_INM(m)      \
        in_dword_masked(HWIO_WCSS_HM_A_WCSS_DBG_TSGEN_CNTCVU_ADDR, m)
#define HWIO_WCSS_HM_A_WCSS_DBG_TSGEN_CNTCVU_OUT(v)      \
        out_dword(HWIO_WCSS_HM_A_WCSS_DBG_TSGEN_CNTCVU_ADDR,v)
#define HWIO_WCSS_HM_A_WCSS_DBG_TSGEN_CNTCVU_OUTM(m,v) \
        out_dword_masked_ns(HWIO_WCSS_HM_A_WCSS_DBG_TSGEN_CNTCVU_ADDR,m,v,HWIO_WCSS_HM_A_WCSS_DBG_TSGEN_CNTCVU_IN)
#define HWIO_WCSS_HM_A_WCSS_DBG_TSGEN_CNTCVU_CNTCVU_U_32_BMSK          0xffffffff
#define HWIO_WCSS_HM_A_WCSS_DBG_TSGEN_CNTCVU_CNTCVU_U_32_SHFT                 0x0

#define HWIO_WCSS_HM_A_WCSS_DBG_TSGEN_CNTFID0_ADDR                     (WCSS_HM_A_WCSS_DBG_TSGEN_TSGEN_CXTSGEN_REG_BASE      + 0x00000020)
#define HWIO_WCSS_HM_A_WCSS_DBG_TSGEN_CNTFID0_RMSK                     0xffffffff
#define HWIO_WCSS_HM_A_WCSS_DBG_TSGEN_CNTFID0_IN          \
        in_dword_masked(HWIO_WCSS_HM_A_WCSS_DBG_TSGEN_CNTFID0_ADDR, HWIO_WCSS_HM_A_WCSS_DBG_TSGEN_CNTFID0_RMSK)
#define HWIO_WCSS_HM_A_WCSS_DBG_TSGEN_CNTFID0_INM(m)      \
        in_dword_masked(HWIO_WCSS_HM_A_WCSS_DBG_TSGEN_CNTFID0_ADDR, m)
#define HWIO_WCSS_HM_A_WCSS_DBG_TSGEN_CNTFID0_OUT(v)      \
        out_dword(HWIO_WCSS_HM_A_WCSS_DBG_TSGEN_CNTFID0_ADDR,v)
#define HWIO_WCSS_HM_A_WCSS_DBG_TSGEN_CNTFID0_OUTM(m,v) \
        out_dword_masked_ns(HWIO_WCSS_HM_A_WCSS_DBG_TSGEN_CNTFID0_ADDR,m,v,HWIO_WCSS_HM_A_WCSS_DBG_TSGEN_CNTFID0_IN)
#define HWIO_WCSS_HM_A_WCSS_DBG_TSGEN_CNTFID0_CNTFID0_BMSK             0xffffffff
#define HWIO_WCSS_HM_A_WCSS_DBG_TSGEN_CNTFID0_CNTFID0_SHFT                    0x0

#define HWIO_WCSS_HM_A_WCSS_DBG_TSGEN_PIDR0_ADDR                       (WCSS_HM_A_WCSS_DBG_TSGEN_TSGEN_CXTSGEN_REG_BASE      + 0x00000fe0)
#define HWIO_WCSS_HM_A_WCSS_DBG_TSGEN_PIDR0_RMSK                             0xff
#define HWIO_WCSS_HM_A_WCSS_DBG_TSGEN_PIDR0_IN          \
        in_dword_masked(HWIO_WCSS_HM_A_WCSS_DBG_TSGEN_PIDR0_ADDR, HWIO_WCSS_HM_A_WCSS_DBG_TSGEN_PIDR0_RMSK)
#define HWIO_WCSS_HM_A_WCSS_DBG_TSGEN_PIDR0_INM(m)      \
        in_dword_masked(HWIO_WCSS_HM_A_WCSS_DBG_TSGEN_PIDR0_ADDR, m)
#define HWIO_WCSS_HM_A_WCSS_DBG_TSGEN_PIDR0_PART_0_BMSK                      0xff
#define HWIO_WCSS_HM_A_WCSS_DBG_TSGEN_PIDR0_PART_0_SHFT                       0x0

#define HWIO_WCSS_HM_A_WCSS_DBG_TSGEN_PIDR1_ADDR                       (WCSS_HM_A_WCSS_DBG_TSGEN_TSGEN_CXTSGEN_REG_BASE      + 0x00000fe4)
#define HWIO_WCSS_HM_A_WCSS_DBG_TSGEN_PIDR1_RMSK                             0xff
#define HWIO_WCSS_HM_A_WCSS_DBG_TSGEN_PIDR1_IN          \
        in_dword_masked(HWIO_WCSS_HM_A_WCSS_DBG_TSGEN_PIDR1_ADDR, HWIO_WCSS_HM_A_WCSS_DBG_TSGEN_PIDR1_RMSK)
#define HWIO_WCSS_HM_A_WCSS_DBG_TSGEN_PIDR1_INM(m)      \
        in_dword_masked(HWIO_WCSS_HM_A_WCSS_DBG_TSGEN_PIDR1_ADDR, m)
#define HWIO_WCSS_HM_A_WCSS_DBG_TSGEN_PIDR1_DES_0_BMSK                       0xf0
#define HWIO_WCSS_HM_A_WCSS_DBG_TSGEN_PIDR1_DES_0_SHFT                        0x4
#define HWIO_WCSS_HM_A_WCSS_DBG_TSGEN_PIDR1_PART_1_BMSK                       0xf
#define HWIO_WCSS_HM_A_WCSS_DBG_TSGEN_PIDR1_PART_1_SHFT                       0x0

#define HWIO_WCSS_HM_A_WCSS_DBG_TSGEN_PIDR2_ADDR                       (WCSS_HM_A_WCSS_DBG_TSGEN_TSGEN_CXTSGEN_REG_BASE      + 0x00000fe8)
#define HWIO_WCSS_HM_A_WCSS_DBG_TSGEN_PIDR2_RMSK                             0xff
#define HWIO_WCSS_HM_A_WCSS_DBG_TSGEN_PIDR2_IN          \
        in_dword_masked(HWIO_WCSS_HM_A_WCSS_DBG_TSGEN_PIDR2_ADDR, HWIO_WCSS_HM_A_WCSS_DBG_TSGEN_PIDR2_RMSK)
#define HWIO_WCSS_HM_A_WCSS_DBG_TSGEN_PIDR2_INM(m)      \
        in_dword_masked(HWIO_WCSS_HM_A_WCSS_DBG_TSGEN_PIDR2_ADDR, m)
#define HWIO_WCSS_HM_A_WCSS_DBG_TSGEN_PIDR2_REVISION_BMSK                    0xf0
#define HWIO_WCSS_HM_A_WCSS_DBG_TSGEN_PIDR2_REVISION_SHFT                     0x4
#define HWIO_WCSS_HM_A_WCSS_DBG_TSGEN_PIDR2_JEDEC_BMSK                        0x8
#define HWIO_WCSS_HM_A_WCSS_DBG_TSGEN_PIDR2_JEDEC_SHFT                        0x3
#define HWIO_WCSS_HM_A_WCSS_DBG_TSGEN_PIDR2_DES_1_BMSK                        0x7
#define HWIO_WCSS_HM_A_WCSS_DBG_TSGEN_PIDR2_DES_1_SHFT                        0x0

#define HWIO_WCSS_HM_A_WCSS_DBG_TSGEN_PIDR3_ADDR                       (WCSS_HM_A_WCSS_DBG_TSGEN_TSGEN_CXTSGEN_REG_BASE      + 0x00000fec)
#define HWIO_WCSS_HM_A_WCSS_DBG_TSGEN_PIDR3_RMSK                             0xff
#define HWIO_WCSS_HM_A_WCSS_DBG_TSGEN_PIDR3_IN          \
        in_dword_masked(HWIO_WCSS_HM_A_WCSS_DBG_TSGEN_PIDR3_ADDR, HWIO_WCSS_HM_A_WCSS_DBG_TSGEN_PIDR3_RMSK)
#define HWIO_WCSS_HM_A_WCSS_DBG_TSGEN_PIDR3_INM(m)      \
        in_dword_masked(HWIO_WCSS_HM_A_WCSS_DBG_TSGEN_PIDR3_ADDR, m)
#define HWIO_WCSS_HM_A_WCSS_DBG_TSGEN_PIDR3_REVAND_BMSK                      0xf0
#define HWIO_WCSS_HM_A_WCSS_DBG_TSGEN_PIDR3_REVAND_SHFT                       0x4
#define HWIO_WCSS_HM_A_WCSS_DBG_TSGEN_PIDR3_CMOD_BMSK                         0xf
#define HWIO_WCSS_HM_A_WCSS_DBG_TSGEN_PIDR3_CMOD_SHFT                         0x0

#define HWIO_WCSS_HM_A_WCSS_DBG_TSGEN_PIDR4_ADDR                       (WCSS_HM_A_WCSS_DBG_TSGEN_TSGEN_CXTSGEN_REG_BASE      + 0x00000fd0)
#define HWIO_WCSS_HM_A_WCSS_DBG_TSGEN_PIDR4_RMSK                             0xff
#define HWIO_WCSS_HM_A_WCSS_DBG_TSGEN_PIDR4_IN          \
        in_dword_masked(HWIO_WCSS_HM_A_WCSS_DBG_TSGEN_PIDR4_ADDR, HWIO_WCSS_HM_A_WCSS_DBG_TSGEN_PIDR4_RMSK)
#define HWIO_WCSS_HM_A_WCSS_DBG_TSGEN_PIDR4_INM(m)      \
        in_dword_masked(HWIO_WCSS_HM_A_WCSS_DBG_TSGEN_PIDR4_ADDR, m)
#define HWIO_WCSS_HM_A_WCSS_DBG_TSGEN_PIDR4_SIZE_BMSK                        0xf0
#define HWIO_WCSS_HM_A_WCSS_DBG_TSGEN_PIDR4_SIZE_SHFT                         0x4
#define HWIO_WCSS_HM_A_WCSS_DBG_TSGEN_PIDR4_DES_2_BMSK                        0xf
#define HWIO_WCSS_HM_A_WCSS_DBG_TSGEN_PIDR4_DES_2_SHFT                        0x0

#define HWIO_WCSS_HM_A_WCSS_DBG_TSGEN_PIDR5_ADDR                       (WCSS_HM_A_WCSS_DBG_TSGEN_TSGEN_CXTSGEN_REG_BASE      + 0x00000fd4)
#define HWIO_WCSS_HM_A_WCSS_DBG_TSGEN_PIDR5_RMSK                       0xffffffff
#define HWIO_WCSS_HM_A_WCSS_DBG_TSGEN_PIDR5_IN          \
        in_dword_masked(HWIO_WCSS_HM_A_WCSS_DBG_TSGEN_PIDR5_ADDR, HWIO_WCSS_HM_A_WCSS_DBG_TSGEN_PIDR5_RMSK)
#define HWIO_WCSS_HM_A_WCSS_DBG_TSGEN_PIDR5_INM(m)      \
        in_dword_masked(HWIO_WCSS_HM_A_WCSS_DBG_TSGEN_PIDR5_ADDR, m)
#define HWIO_WCSS_HM_A_WCSS_DBG_TSGEN_PIDR5_OUT(v)      \
        out_dword(HWIO_WCSS_HM_A_WCSS_DBG_TSGEN_PIDR5_ADDR,v)
#define HWIO_WCSS_HM_A_WCSS_DBG_TSGEN_PIDR5_OUTM(m,v) \
        out_dword_masked_ns(HWIO_WCSS_HM_A_WCSS_DBG_TSGEN_PIDR5_ADDR,m,v,HWIO_WCSS_HM_A_WCSS_DBG_TSGEN_PIDR5_IN)
#define HWIO_WCSS_HM_A_WCSS_DBG_TSGEN_PIDR5_PIDR5_BMSK                 0xffffffff
#define HWIO_WCSS_HM_A_WCSS_DBG_TSGEN_PIDR5_PIDR5_SHFT                        0x0

#define HWIO_WCSS_HM_A_WCSS_DBG_TSGEN_PIDR6_ADDR                       (WCSS_HM_A_WCSS_DBG_TSGEN_TSGEN_CXTSGEN_REG_BASE      + 0x00000fd8)
#define HWIO_WCSS_HM_A_WCSS_DBG_TSGEN_PIDR6_RMSK                       0xffffffff
#define HWIO_WCSS_HM_A_WCSS_DBG_TSGEN_PIDR6_IN          \
        in_dword_masked(HWIO_WCSS_HM_A_WCSS_DBG_TSGEN_PIDR6_ADDR, HWIO_WCSS_HM_A_WCSS_DBG_TSGEN_PIDR6_RMSK)
#define HWIO_WCSS_HM_A_WCSS_DBG_TSGEN_PIDR6_INM(m)      \
        in_dword_masked(HWIO_WCSS_HM_A_WCSS_DBG_TSGEN_PIDR6_ADDR, m)
#define HWIO_WCSS_HM_A_WCSS_DBG_TSGEN_PIDR6_OUT(v)      \
        out_dword(HWIO_WCSS_HM_A_WCSS_DBG_TSGEN_PIDR6_ADDR,v)
#define HWIO_WCSS_HM_A_WCSS_DBG_TSGEN_PIDR6_OUTM(m,v) \
        out_dword_masked_ns(HWIO_WCSS_HM_A_WCSS_DBG_TSGEN_PIDR6_ADDR,m,v,HWIO_WCSS_HM_A_WCSS_DBG_TSGEN_PIDR6_IN)
#define HWIO_WCSS_HM_A_WCSS_DBG_TSGEN_PIDR6_PIDR6_BMSK                 0xffffffff
#define HWIO_WCSS_HM_A_WCSS_DBG_TSGEN_PIDR6_PIDR6_SHFT                        0x0

#define HWIO_WCSS_HM_A_WCSS_DBG_TSGEN_PIDR7_ADDR                       (WCSS_HM_A_WCSS_DBG_TSGEN_TSGEN_CXTSGEN_REG_BASE      + 0x00000fdc)
#define HWIO_WCSS_HM_A_WCSS_DBG_TSGEN_PIDR7_RMSK                       0xffffffff
#define HWIO_WCSS_HM_A_WCSS_DBG_TSGEN_PIDR7_IN          \
        in_dword_masked(HWIO_WCSS_HM_A_WCSS_DBG_TSGEN_PIDR7_ADDR, HWIO_WCSS_HM_A_WCSS_DBG_TSGEN_PIDR7_RMSK)
#define HWIO_WCSS_HM_A_WCSS_DBG_TSGEN_PIDR7_INM(m)      \
        in_dword_masked(HWIO_WCSS_HM_A_WCSS_DBG_TSGEN_PIDR7_ADDR, m)
#define HWIO_WCSS_HM_A_WCSS_DBG_TSGEN_PIDR7_OUT(v)      \
        out_dword(HWIO_WCSS_HM_A_WCSS_DBG_TSGEN_PIDR7_ADDR,v)
#define HWIO_WCSS_HM_A_WCSS_DBG_TSGEN_PIDR7_OUTM(m,v) \
        out_dword_masked_ns(HWIO_WCSS_HM_A_WCSS_DBG_TSGEN_PIDR7_ADDR,m,v,HWIO_WCSS_HM_A_WCSS_DBG_TSGEN_PIDR7_IN)
#define HWIO_WCSS_HM_A_WCSS_DBG_TSGEN_PIDR7_PIDR7_BMSK                 0xffffffff
#define HWIO_WCSS_HM_A_WCSS_DBG_TSGEN_PIDR7_PIDR7_SHFT                        0x0

#define HWIO_WCSS_HM_A_WCSS_DBG_TSGEN_CIDR0_ADDR                       (WCSS_HM_A_WCSS_DBG_TSGEN_TSGEN_CXTSGEN_REG_BASE      + 0x00000ff0)
#define HWIO_WCSS_HM_A_WCSS_DBG_TSGEN_CIDR0_RMSK                             0xff
#define HWIO_WCSS_HM_A_WCSS_DBG_TSGEN_CIDR0_IN          \
        in_dword_masked(HWIO_WCSS_HM_A_WCSS_DBG_TSGEN_CIDR0_ADDR, HWIO_WCSS_HM_A_WCSS_DBG_TSGEN_CIDR0_RMSK)
#define HWIO_WCSS_HM_A_WCSS_DBG_TSGEN_CIDR0_INM(m)      \
        in_dword_masked(HWIO_WCSS_HM_A_WCSS_DBG_TSGEN_CIDR0_ADDR, m)
#define HWIO_WCSS_HM_A_WCSS_DBG_TSGEN_CIDR0_PRMBL_0_BMSK                     0xff
#define HWIO_WCSS_HM_A_WCSS_DBG_TSGEN_CIDR0_PRMBL_0_SHFT                      0x0

#define HWIO_WCSS_HM_A_WCSS_DBG_TSGEN_CIDR1_ADDR                       (WCSS_HM_A_WCSS_DBG_TSGEN_TSGEN_CXTSGEN_REG_BASE      + 0x00000ff4)
#define HWIO_WCSS_HM_A_WCSS_DBG_TSGEN_CIDR1_RMSK                             0xff
#define HWIO_WCSS_HM_A_WCSS_DBG_TSGEN_CIDR1_IN          \
        in_dword_masked(HWIO_WCSS_HM_A_WCSS_DBG_TSGEN_CIDR1_ADDR, HWIO_WCSS_HM_A_WCSS_DBG_TSGEN_CIDR1_RMSK)
#define HWIO_WCSS_HM_A_WCSS_DBG_TSGEN_CIDR1_INM(m)      \
        in_dword_masked(HWIO_WCSS_HM_A_WCSS_DBG_TSGEN_CIDR1_ADDR, m)
#define HWIO_WCSS_HM_A_WCSS_DBG_TSGEN_CIDR1_CLASS_BMSK                       0xf0
#define HWIO_WCSS_HM_A_WCSS_DBG_TSGEN_CIDR1_CLASS_SHFT                        0x4
#define HWIO_WCSS_HM_A_WCSS_DBG_TSGEN_CIDR1_PRMBL_1_BMSK                      0xf
#define HWIO_WCSS_HM_A_WCSS_DBG_TSGEN_CIDR1_PRMBL_1_SHFT                      0x0

#define HWIO_WCSS_HM_A_WCSS_DBG_TSGEN_CIDR2_ADDR                       (WCSS_HM_A_WCSS_DBG_TSGEN_TSGEN_CXTSGEN_REG_BASE      + 0x00000ff8)
#define HWIO_WCSS_HM_A_WCSS_DBG_TSGEN_CIDR2_RMSK                             0xff
#define HWIO_WCSS_HM_A_WCSS_DBG_TSGEN_CIDR2_IN          \
        in_dword_masked(HWIO_WCSS_HM_A_WCSS_DBG_TSGEN_CIDR2_ADDR, HWIO_WCSS_HM_A_WCSS_DBG_TSGEN_CIDR2_RMSK)
#define HWIO_WCSS_HM_A_WCSS_DBG_TSGEN_CIDR2_INM(m)      \
        in_dword_masked(HWIO_WCSS_HM_A_WCSS_DBG_TSGEN_CIDR2_ADDR, m)
#define HWIO_WCSS_HM_A_WCSS_DBG_TSGEN_CIDR2_PRMBL_2_BMSK                     0xff
#define HWIO_WCSS_HM_A_WCSS_DBG_TSGEN_CIDR2_PRMBL_2_SHFT                      0x0

#define HWIO_WCSS_HM_A_WCSS_DBG_TSGEN_CIDR3_ADDR                       (WCSS_HM_A_WCSS_DBG_TSGEN_TSGEN_CXTSGEN_REG_BASE      + 0x00000ffc)
#define HWIO_WCSS_HM_A_WCSS_DBG_TSGEN_CIDR3_RMSK                             0xff
#define HWIO_WCSS_HM_A_WCSS_DBG_TSGEN_CIDR3_IN          \
        in_dword_masked(HWIO_WCSS_HM_A_WCSS_DBG_TSGEN_CIDR3_ADDR, HWIO_WCSS_HM_A_WCSS_DBG_TSGEN_CIDR3_RMSK)
#define HWIO_WCSS_HM_A_WCSS_DBG_TSGEN_CIDR3_INM(m)      \
        in_dword_masked(HWIO_WCSS_HM_A_WCSS_DBG_TSGEN_CIDR3_ADDR, m)
#define HWIO_WCSS_HM_A_WCSS_DBG_TSGEN_CIDR3_PRMBL_3_BMSK                     0xff
#define HWIO_WCSS_HM_A_WCSS_DBG_TSGEN_CIDR3_PRMBL_3_SHFT                      0x0


#endif /* __WCSS_DBG_TSGEN_HWIO_H__ */
