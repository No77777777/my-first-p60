#ifndef __MSMHWIOREG_GEN_EMU_H__
#define __MSMHWIOREG_GEN_EMU_H__
/*
===========================================================================
*/
/**
  @file msmhwioreg_gen_emu.h
  @brief Auto-generated HWIO interface include file.

  This file contains HWIO register definitions for the following bases:
    .*

  'Include' filters applied: <none>
  'Exclude' filters applied: RESERVED 

  Attribute definitions for the HWIO_*_ATTR macros are as follows:
    0x0: Command register
    0x1: Read-Only
    0x2: Write-Only
    0x3: Read/Write
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

  $Header: //components/rel/dataipa.mpss/3.2/emu/IPA/platform/windows/VITests/COPSS/msmhwioreg_gen_emu.h#1 $
  $DateTime: 2019/07/05 03:48:36 $
  $Author: pwbldsvc $

  ===========================================================================
*/

/*----------------------------------------------------------------------------
 * MODULE: GE_PCIE_PCIE20_DBI_4
 *--------------------------------------------------------------------------*/

#define GE_PCIE_PCIE20_DBI_4_REG_BASE                                                                      (GE_PCIE_PCIE_REGS_BASE      + 0x00000000)
#define GE_PCIE_PCIE20_DBI_4_REG_BASE_PHYS                                                                 (GE_PCIE_PCIE_REGS_BASE_PHYS + 0x00000000)
#define GE_PCIE_PCIE20_DBI_4_REG_BASE_OFFS                                                                 0x00000000

#define HWIO_GE_PCIE_TYPE1_DEV_ID_VEND_ID_REG_ADDR                                                         (GE_PCIE_PCIE20_DBI_4_REG_BASE      + 0x00000000)
#define HWIO_GE_PCIE_TYPE1_DEV_ID_VEND_ID_REG_PHYS                                                         (GE_PCIE_PCIE20_DBI_4_REG_BASE_PHYS + 0x00000000)
#define HWIO_GE_PCIE_TYPE1_DEV_ID_VEND_ID_REG_OFFS                                                         (GE_PCIE_PCIE20_DBI_4_REG_BASE_OFFS + 0x00000000)
#define HWIO_GE_PCIE_TYPE1_DEV_ID_VEND_ID_REG_RMSK                                                         0xffffffff
#define HWIO_GE_PCIE_TYPE1_DEV_ID_VEND_ID_REG_POR                                                          0x00000000
#define HWIO_GE_PCIE_TYPE1_DEV_ID_VEND_ID_REG_POR_RMSK                                                     0xffffffff
#define HWIO_GE_PCIE_TYPE1_DEV_ID_VEND_ID_REG_ATTR                                                                0x1
#define HWIO_GE_PCIE_TYPE1_DEV_ID_VEND_ID_REG_IN          \
        in_dword_masked(HWIO_GE_PCIE_TYPE1_DEV_ID_VEND_ID_REG_ADDR, HWIO_GE_PCIE_TYPE1_DEV_ID_VEND_ID_REG_RMSK)
#define HWIO_GE_PCIE_TYPE1_DEV_ID_VEND_ID_REG_INM(m)      \
        in_dword_masked(HWIO_GE_PCIE_TYPE1_DEV_ID_VEND_ID_REG_ADDR, m)
#define HWIO_GE_PCIE_TYPE1_DEV_ID_VEND_ID_REG_DEVICE_ID_BMSK                                               0xffff0000
#define HWIO_GE_PCIE_TYPE1_DEV_ID_VEND_ID_REG_DEVICE_ID_SHFT                                                     0x10
#define HWIO_GE_PCIE_TYPE1_DEV_ID_VEND_ID_REG_VENDOR_ID_BMSK                                                   0xffff
#define HWIO_GE_PCIE_TYPE1_DEV_ID_VEND_ID_REG_VENDOR_ID_SHFT                                                      0x0

#define HWIO_GE_PCIE_TYPE1_STATUS_COMMAND_REG_ADDR                                                         (GE_PCIE_PCIE20_DBI_4_REG_BASE      + 0x00000004)
#define HWIO_GE_PCIE_TYPE1_STATUS_COMMAND_REG_PHYS                                                         (GE_PCIE_PCIE20_DBI_4_REG_BASE_PHYS + 0x00000004)
#define HWIO_GE_PCIE_TYPE1_STATUS_COMMAND_REG_OFFS                                                         (GE_PCIE_PCIE20_DBI_4_REG_BASE_OFFS + 0x00000004)
#define HWIO_GE_PCIE_TYPE1_STATUS_COMMAND_REG_RMSK                                                         0xffffffff
#define HWIO_GE_PCIE_TYPE1_STATUS_COMMAND_REG_POR                                                          0x00000000
#define HWIO_GE_PCIE_TYPE1_STATUS_COMMAND_REG_POR_RMSK                                                     0xffffffff
#define HWIO_GE_PCIE_TYPE1_STATUS_COMMAND_REG_ATTR                                                                0x3
#define HWIO_GE_PCIE_TYPE1_STATUS_COMMAND_REG_IN          \
        in_dword_masked(HWIO_GE_PCIE_TYPE1_STATUS_COMMAND_REG_ADDR, HWIO_GE_PCIE_TYPE1_STATUS_COMMAND_REG_RMSK)
#define HWIO_GE_PCIE_TYPE1_STATUS_COMMAND_REG_INM(m)      \
        in_dword_masked(HWIO_GE_PCIE_TYPE1_STATUS_COMMAND_REG_ADDR, m)
#define HWIO_GE_PCIE_TYPE1_STATUS_COMMAND_REG_OUT(v)      \
        out_dword(HWIO_GE_PCIE_TYPE1_STATUS_COMMAND_REG_ADDR,v)
#define HWIO_GE_PCIE_TYPE1_STATUS_COMMAND_REG_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GE_PCIE_TYPE1_STATUS_COMMAND_REG_ADDR,m,v,HWIO_GE_PCIE_TYPE1_STATUS_COMMAND_REG_IN)
#define HWIO_GE_PCIE_TYPE1_STATUS_COMMAND_REG_DETECTED_PARITY_ERROR_BMSK                                   0x80000000
#define HWIO_GE_PCIE_TYPE1_STATUS_COMMAND_REG_DETECTED_PARITY_ERROR_SHFT                                         0x1f
#define HWIO_GE_PCIE_TYPE1_STATUS_COMMAND_REG_SIGNALED_SYS_ERROR_BMSK                                      0x40000000
#define HWIO_GE_PCIE_TYPE1_STATUS_COMMAND_REG_SIGNALED_SYS_ERROR_SHFT                                            0x1e
#define HWIO_GE_PCIE_TYPE1_STATUS_COMMAND_REG_RCVD_MASTER_ABORT_BMSK                                       0x20000000
#define HWIO_GE_PCIE_TYPE1_STATUS_COMMAND_REG_RCVD_MASTER_ABORT_SHFT                                             0x1d
#define HWIO_GE_PCIE_TYPE1_STATUS_COMMAND_REG_RCVD_TARGET_ABORT_BMSK                                       0x10000000
#define HWIO_GE_PCIE_TYPE1_STATUS_COMMAND_REG_RCVD_TARGET_ABORT_SHFT                                             0x1c
#define HWIO_GE_PCIE_TYPE1_STATUS_COMMAND_REG_SIGNALED_TARGET_ABORT_BMSK                                    0x8000000
#define HWIO_GE_PCIE_TYPE1_STATUS_COMMAND_REG_SIGNALED_TARGET_ABORT_SHFT                                         0x1b
#define HWIO_GE_PCIE_TYPE1_STATUS_COMMAND_REG_DEV_SEL_TIMING_BMSK                                           0x6000000
#define HWIO_GE_PCIE_TYPE1_STATUS_COMMAND_REG_DEV_SEL_TIMING_SHFT                                                0x19
#define HWIO_GE_PCIE_TYPE1_STATUS_COMMAND_REG_MASTER_DPE_BMSK                                               0x1000000
#define HWIO_GE_PCIE_TYPE1_STATUS_COMMAND_REG_MASTER_DPE_SHFT                                                    0x18
#define HWIO_GE_PCIE_TYPE1_STATUS_COMMAND_REG_FAST_B2B_CAP_BMSK                                              0x800000
#define HWIO_GE_PCIE_TYPE1_STATUS_COMMAND_REG_FAST_B2B_CAP_SHFT                                                  0x17
#define HWIO_GE_PCIE_TYPE1_STATUS_COMMAND_REG_RSVDP_22_BMSK                                                  0x400000
#define HWIO_GE_PCIE_TYPE1_STATUS_COMMAND_REG_RSVDP_22_SHFT                                                      0x16
#define HWIO_GE_PCIE_TYPE1_STATUS_COMMAND_REG_FAST_66MHZ_CAP_BMSK                                            0x200000
#define HWIO_GE_PCIE_TYPE1_STATUS_COMMAND_REG_FAST_66MHZ_CAP_SHFT                                                0x15
#define HWIO_GE_PCIE_TYPE1_STATUS_COMMAND_REG_CAP_LIST_BMSK                                                  0x100000
#define HWIO_GE_PCIE_TYPE1_STATUS_COMMAND_REG_CAP_LIST_SHFT                                                      0x14
#define HWIO_GE_PCIE_TYPE1_STATUS_COMMAND_REG_INT_STATUS_BMSK                                                 0x80000
#define HWIO_GE_PCIE_TYPE1_STATUS_COMMAND_REG_INT_STATUS_SHFT                                                    0x13
#define HWIO_GE_PCIE_TYPE1_STATUS_COMMAND_REG_RSVDP_17_BMSK                                                   0x60000
#define HWIO_GE_PCIE_TYPE1_STATUS_COMMAND_REG_RSVDP_17_SHFT                                                      0x11
#define HWIO_GE_PCIE_TYPE1_STATUS_COMMAND_REG_RESERV_BMSK                                                     0x1f800
#define HWIO_GE_PCIE_TYPE1_STATUS_COMMAND_REG_RESERV_SHFT                                                         0xb
#define HWIO_GE_PCIE_TYPE1_STATUS_COMMAND_REG_INT_EN_BMSK                                                       0x400
#define HWIO_GE_PCIE_TYPE1_STATUS_COMMAND_REG_INT_EN_SHFT                                                         0xa
#define HWIO_GE_PCIE_TYPE1_STATUS_COMMAND_REG_RSVDP_9_BMSK                                                      0x200
#define HWIO_GE_PCIE_TYPE1_STATUS_COMMAND_REG_RSVDP_9_SHFT                                                        0x9
#define HWIO_GE_PCIE_TYPE1_STATUS_COMMAND_REG_SERREN_BMSK                                                       0x100
#define HWIO_GE_PCIE_TYPE1_STATUS_COMMAND_REG_SERREN_SHFT                                                         0x8
#define HWIO_GE_PCIE_TYPE1_STATUS_COMMAND_REG_IDSEL_BMSK                                                         0x80
#define HWIO_GE_PCIE_TYPE1_STATUS_COMMAND_REG_IDSEL_SHFT                                                          0x7
#define HWIO_GE_PCIE_TYPE1_STATUS_COMMAND_REG_PERREN_BMSK                                                        0x40
#define HWIO_GE_PCIE_TYPE1_STATUS_COMMAND_REG_PERREN_SHFT                                                         0x6
#define HWIO_GE_PCIE_TYPE1_STATUS_COMMAND_REG_VGAPS_BMSK                                                         0x20
#define HWIO_GE_PCIE_TYPE1_STATUS_COMMAND_REG_VGAPS_SHFT                                                          0x5
#define HWIO_GE_PCIE_TYPE1_STATUS_COMMAND_REG_MWI_EN_BMSK                                                        0x10
#define HWIO_GE_PCIE_TYPE1_STATUS_COMMAND_REG_MWI_EN_SHFT                                                         0x4
#define HWIO_GE_PCIE_TYPE1_STATUS_COMMAND_REG_SCO_BMSK                                                            0x8
#define HWIO_GE_PCIE_TYPE1_STATUS_COMMAND_REG_SCO_SHFT                                                            0x3
#define HWIO_GE_PCIE_TYPE1_STATUS_COMMAND_REG_BME_BMSK                                                            0x4
#define HWIO_GE_PCIE_TYPE1_STATUS_COMMAND_REG_BME_SHFT                                                            0x2
#define HWIO_GE_PCIE_TYPE1_STATUS_COMMAND_REG_MSE_BMSK                                                            0x2
#define HWIO_GE_PCIE_TYPE1_STATUS_COMMAND_REG_MSE_SHFT                                                            0x1
#define HWIO_GE_PCIE_TYPE1_STATUS_COMMAND_REG_IO_EN_BMSK                                                          0x1
#define HWIO_GE_PCIE_TYPE1_STATUS_COMMAND_REG_IO_EN_SHFT                                                          0x0

#define HWIO_GE_PCIE_TYPE1_CLASS_CODE_REV_ID_REG_ADDR                                                      (GE_PCIE_PCIE20_DBI_4_REG_BASE      + 0x00000008)
#define HWIO_GE_PCIE_TYPE1_CLASS_CODE_REV_ID_REG_PHYS                                                      (GE_PCIE_PCIE20_DBI_4_REG_BASE_PHYS + 0x00000008)
#define HWIO_GE_PCIE_TYPE1_CLASS_CODE_REV_ID_REG_OFFS                                                      (GE_PCIE_PCIE20_DBI_4_REG_BASE_OFFS + 0x00000008)
#define HWIO_GE_PCIE_TYPE1_CLASS_CODE_REV_ID_REG_RMSK                                                      0xffffffff
#define HWIO_GE_PCIE_TYPE1_CLASS_CODE_REV_ID_REG_POR                                                       0x00000000
#define HWIO_GE_PCIE_TYPE1_CLASS_CODE_REV_ID_REG_POR_RMSK                                                  0xffffffff
#define HWIO_GE_PCIE_TYPE1_CLASS_CODE_REV_ID_REG_ATTR                                                             0x1
#define HWIO_GE_PCIE_TYPE1_CLASS_CODE_REV_ID_REG_IN          \
        in_dword_masked(HWIO_GE_PCIE_TYPE1_CLASS_CODE_REV_ID_REG_ADDR, HWIO_GE_PCIE_TYPE1_CLASS_CODE_REV_ID_REG_RMSK)
#define HWIO_GE_PCIE_TYPE1_CLASS_CODE_REV_ID_REG_INM(m)      \
        in_dword_masked(HWIO_GE_PCIE_TYPE1_CLASS_CODE_REV_ID_REG_ADDR, m)
#define HWIO_GE_PCIE_TYPE1_CLASS_CODE_REV_ID_REG_BASE_CLASS_CODE_BMSK                                      0xff000000
#define HWIO_GE_PCIE_TYPE1_CLASS_CODE_REV_ID_REG_BASE_CLASS_CODE_SHFT                                            0x18
#define HWIO_GE_PCIE_TYPE1_CLASS_CODE_REV_ID_REG_SUBCLASS_CODE_BMSK                                          0xff0000
#define HWIO_GE_PCIE_TYPE1_CLASS_CODE_REV_ID_REG_SUBCLASS_CODE_SHFT                                              0x10
#define HWIO_GE_PCIE_TYPE1_CLASS_CODE_REV_ID_REG_PROGRAM_INTERFACE_BMSK                                        0xff00
#define HWIO_GE_PCIE_TYPE1_CLASS_CODE_REV_ID_REG_PROGRAM_INTERFACE_SHFT                                           0x8
#define HWIO_GE_PCIE_TYPE1_CLASS_CODE_REV_ID_REG_REVISION_ID_BMSK                                                0xff
#define HWIO_GE_PCIE_TYPE1_CLASS_CODE_REV_ID_REG_REVISION_ID_SHFT                                                 0x0

#define HWIO_GE_PCIE_TYPE1_BIST_HDR_TYPE_LAT_CACHE_LINE_SIZE_REG_ADDR                                      (GE_PCIE_PCIE20_DBI_4_REG_BASE      + 0x0000000c)
#define HWIO_GE_PCIE_TYPE1_BIST_HDR_TYPE_LAT_CACHE_LINE_SIZE_REG_PHYS                                      (GE_PCIE_PCIE20_DBI_4_REG_BASE_PHYS + 0x0000000c)
#define HWIO_GE_PCIE_TYPE1_BIST_HDR_TYPE_LAT_CACHE_LINE_SIZE_REG_OFFS                                      (GE_PCIE_PCIE20_DBI_4_REG_BASE_OFFS + 0x0000000c)
#define HWIO_GE_PCIE_TYPE1_BIST_HDR_TYPE_LAT_CACHE_LINE_SIZE_REG_RMSK                                      0xffffffff
#define HWIO_GE_PCIE_TYPE1_BIST_HDR_TYPE_LAT_CACHE_LINE_SIZE_REG_POR                                       0x00000000
#define HWIO_GE_PCIE_TYPE1_BIST_HDR_TYPE_LAT_CACHE_LINE_SIZE_REG_POR_RMSK                                  0xffffffff
#define HWIO_GE_PCIE_TYPE1_BIST_HDR_TYPE_LAT_CACHE_LINE_SIZE_REG_ATTR                                             0x3
#define HWIO_GE_PCIE_TYPE1_BIST_HDR_TYPE_LAT_CACHE_LINE_SIZE_REG_IN          \
        in_dword_masked(HWIO_GE_PCIE_TYPE1_BIST_HDR_TYPE_LAT_CACHE_LINE_SIZE_REG_ADDR, HWIO_GE_PCIE_TYPE1_BIST_HDR_TYPE_LAT_CACHE_LINE_SIZE_REG_RMSK)
#define HWIO_GE_PCIE_TYPE1_BIST_HDR_TYPE_LAT_CACHE_LINE_SIZE_REG_INM(m)      \
        in_dword_masked(HWIO_GE_PCIE_TYPE1_BIST_HDR_TYPE_LAT_CACHE_LINE_SIZE_REG_ADDR, m)
#define HWIO_GE_PCIE_TYPE1_BIST_HDR_TYPE_LAT_CACHE_LINE_SIZE_REG_OUT(v)      \
        out_dword(HWIO_GE_PCIE_TYPE1_BIST_HDR_TYPE_LAT_CACHE_LINE_SIZE_REG_ADDR,v)
#define HWIO_GE_PCIE_TYPE1_BIST_HDR_TYPE_LAT_CACHE_LINE_SIZE_REG_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GE_PCIE_TYPE1_BIST_HDR_TYPE_LAT_CACHE_LINE_SIZE_REG_ADDR,m,v,HWIO_GE_PCIE_TYPE1_BIST_HDR_TYPE_LAT_CACHE_LINE_SIZE_REG_IN)
#define HWIO_GE_PCIE_TYPE1_BIST_HDR_TYPE_LAT_CACHE_LINE_SIZE_REG_BIST_BMSK                                 0xff000000
#define HWIO_GE_PCIE_TYPE1_BIST_HDR_TYPE_LAT_CACHE_LINE_SIZE_REG_BIST_SHFT                                       0x18
#define HWIO_GE_PCIE_TYPE1_BIST_HDR_TYPE_LAT_CACHE_LINE_SIZE_REG_MULTI_FUNC_BMSK                             0x800000
#define HWIO_GE_PCIE_TYPE1_BIST_HDR_TYPE_LAT_CACHE_LINE_SIZE_REG_MULTI_FUNC_SHFT                                 0x17
#define HWIO_GE_PCIE_TYPE1_BIST_HDR_TYPE_LAT_CACHE_LINE_SIZE_REG_HEADER_TYPE_BMSK                            0x7f0000
#define HWIO_GE_PCIE_TYPE1_BIST_HDR_TYPE_LAT_CACHE_LINE_SIZE_REG_HEADER_TYPE_SHFT                                0x10
#define HWIO_GE_PCIE_TYPE1_BIST_HDR_TYPE_LAT_CACHE_LINE_SIZE_REG_LATENCY_MASTER_TIMER_BMSK                     0xff00
#define HWIO_GE_PCIE_TYPE1_BIST_HDR_TYPE_LAT_CACHE_LINE_SIZE_REG_LATENCY_MASTER_TIMER_SHFT                        0x8
#define HWIO_GE_PCIE_TYPE1_BIST_HDR_TYPE_LAT_CACHE_LINE_SIZE_REG_CACHE_LINE_SIZE_BMSK                            0xff
#define HWIO_GE_PCIE_TYPE1_BIST_HDR_TYPE_LAT_CACHE_LINE_SIZE_REG_CACHE_LINE_SIZE_SHFT                             0x0

#define HWIO_GE_PCIE_SEC_LAT_TIMER_SUB_BUS_SEC_BUS_PRI_BUS_REG_ADDR                                        (GE_PCIE_PCIE20_DBI_4_REG_BASE      + 0x00000018)
#define HWIO_GE_PCIE_SEC_LAT_TIMER_SUB_BUS_SEC_BUS_PRI_BUS_REG_PHYS                                        (GE_PCIE_PCIE20_DBI_4_REG_BASE_PHYS + 0x00000018)
#define HWIO_GE_PCIE_SEC_LAT_TIMER_SUB_BUS_SEC_BUS_PRI_BUS_REG_OFFS                                        (GE_PCIE_PCIE20_DBI_4_REG_BASE_OFFS + 0x00000018)
#define HWIO_GE_PCIE_SEC_LAT_TIMER_SUB_BUS_SEC_BUS_PRI_BUS_REG_RMSK                                        0xffffffff
#define HWIO_GE_PCIE_SEC_LAT_TIMER_SUB_BUS_SEC_BUS_PRI_BUS_REG_POR                                         0x00000000
#define HWIO_GE_PCIE_SEC_LAT_TIMER_SUB_BUS_SEC_BUS_PRI_BUS_REG_POR_RMSK                                    0xffffffff
#define HWIO_GE_PCIE_SEC_LAT_TIMER_SUB_BUS_SEC_BUS_PRI_BUS_REG_ATTR                                               0x3
#define HWIO_GE_PCIE_SEC_LAT_TIMER_SUB_BUS_SEC_BUS_PRI_BUS_REG_IN          \
        in_dword_masked(HWIO_GE_PCIE_SEC_LAT_TIMER_SUB_BUS_SEC_BUS_PRI_BUS_REG_ADDR, HWIO_GE_PCIE_SEC_LAT_TIMER_SUB_BUS_SEC_BUS_PRI_BUS_REG_RMSK)
#define HWIO_GE_PCIE_SEC_LAT_TIMER_SUB_BUS_SEC_BUS_PRI_BUS_REG_INM(m)      \
        in_dword_masked(HWIO_GE_PCIE_SEC_LAT_TIMER_SUB_BUS_SEC_BUS_PRI_BUS_REG_ADDR, m)
#define HWIO_GE_PCIE_SEC_LAT_TIMER_SUB_BUS_SEC_BUS_PRI_BUS_REG_OUT(v)      \
        out_dword(HWIO_GE_PCIE_SEC_LAT_TIMER_SUB_BUS_SEC_BUS_PRI_BUS_REG_ADDR,v)
#define HWIO_GE_PCIE_SEC_LAT_TIMER_SUB_BUS_SEC_BUS_PRI_BUS_REG_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GE_PCIE_SEC_LAT_TIMER_SUB_BUS_SEC_BUS_PRI_BUS_REG_ADDR,m,v,HWIO_GE_PCIE_SEC_LAT_TIMER_SUB_BUS_SEC_BUS_PRI_BUS_REG_IN)
#define HWIO_GE_PCIE_SEC_LAT_TIMER_SUB_BUS_SEC_BUS_PRI_BUS_REG_SEC_LAT_TIMER_BMSK                          0xff000000
#define HWIO_GE_PCIE_SEC_LAT_TIMER_SUB_BUS_SEC_BUS_PRI_BUS_REG_SEC_LAT_TIMER_SHFT                                0x18
#define HWIO_GE_PCIE_SEC_LAT_TIMER_SUB_BUS_SEC_BUS_PRI_BUS_REG_SUB_BUS_BMSK                                  0xff0000
#define HWIO_GE_PCIE_SEC_LAT_TIMER_SUB_BUS_SEC_BUS_PRI_BUS_REG_SUB_BUS_SHFT                                      0x10
#define HWIO_GE_PCIE_SEC_LAT_TIMER_SUB_BUS_SEC_BUS_PRI_BUS_REG_SEC_BUS_BMSK                                    0xff00
#define HWIO_GE_PCIE_SEC_LAT_TIMER_SUB_BUS_SEC_BUS_PRI_BUS_REG_SEC_BUS_SHFT                                       0x8
#define HWIO_GE_PCIE_SEC_LAT_TIMER_SUB_BUS_SEC_BUS_PRI_BUS_REG_PRIM_BUS_BMSK                                     0xff
#define HWIO_GE_PCIE_SEC_LAT_TIMER_SUB_BUS_SEC_BUS_PRI_BUS_REG_PRIM_BUS_SHFT                                      0x0

#define HWIO_GE_PCIE_SEC_STAT_IO_LIMIT_IO_BASE_REG_ADDR                                                    (GE_PCIE_PCIE20_DBI_4_REG_BASE      + 0x0000001c)
#define HWIO_GE_PCIE_SEC_STAT_IO_LIMIT_IO_BASE_REG_PHYS                                                    (GE_PCIE_PCIE20_DBI_4_REG_BASE_PHYS + 0x0000001c)
#define HWIO_GE_PCIE_SEC_STAT_IO_LIMIT_IO_BASE_REG_OFFS                                                    (GE_PCIE_PCIE20_DBI_4_REG_BASE_OFFS + 0x0000001c)
#define HWIO_GE_PCIE_SEC_STAT_IO_LIMIT_IO_BASE_REG_RMSK                                                    0xffffffff
#define HWIO_GE_PCIE_SEC_STAT_IO_LIMIT_IO_BASE_REG_POR                                                     0x00000000
#define HWIO_GE_PCIE_SEC_STAT_IO_LIMIT_IO_BASE_REG_POR_RMSK                                                0xffffffff
#define HWIO_GE_PCIE_SEC_STAT_IO_LIMIT_IO_BASE_REG_ATTR                                                           0x3
#define HWIO_GE_PCIE_SEC_STAT_IO_LIMIT_IO_BASE_REG_IN          \
        in_dword_masked(HWIO_GE_PCIE_SEC_STAT_IO_LIMIT_IO_BASE_REG_ADDR, HWIO_GE_PCIE_SEC_STAT_IO_LIMIT_IO_BASE_REG_RMSK)
#define HWIO_GE_PCIE_SEC_STAT_IO_LIMIT_IO_BASE_REG_INM(m)      \
        in_dword_masked(HWIO_GE_PCIE_SEC_STAT_IO_LIMIT_IO_BASE_REG_ADDR, m)
#define HWIO_GE_PCIE_SEC_STAT_IO_LIMIT_IO_BASE_REG_OUT(v)      \
        out_dword(HWIO_GE_PCIE_SEC_STAT_IO_LIMIT_IO_BASE_REG_ADDR,v)
#define HWIO_GE_PCIE_SEC_STAT_IO_LIMIT_IO_BASE_REG_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GE_PCIE_SEC_STAT_IO_LIMIT_IO_BASE_REG_ADDR,m,v,HWIO_GE_PCIE_SEC_STAT_IO_LIMIT_IO_BASE_REG_IN)
#define HWIO_GE_PCIE_SEC_STAT_IO_LIMIT_IO_BASE_REG_SEC_STAT_DPE_BMSK                                       0x80000000
#define HWIO_GE_PCIE_SEC_STAT_IO_LIMIT_IO_BASE_REG_SEC_STAT_DPE_SHFT                                             0x1f
#define HWIO_GE_PCIE_SEC_STAT_IO_LIMIT_IO_BASE_REG_SEC_STAT_RCVD_SYS_ERR_BMSK                              0x40000000
#define HWIO_GE_PCIE_SEC_STAT_IO_LIMIT_IO_BASE_REG_SEC_STAT_RCVD_SYS_ERR_SHFT                                    0x1e
#define HWIO_GE_PCIE_SEC_STAT_IO_LIMIT_IO_BASE_REG_SEC_STAT_RCVD_MSTR_ABRT_BMSK                            0x20000000
#define HWIO_GE_PCIE_SEC_STAT_IO_LIMIT_IO_BASE_REG_SEC_STAT_RCVD_MSTR_ABRT_SHFT                                  0x1d
#define HWIO_GE_PCIE_SEC_STAT_IO_LIMIT_IO_BASE_REG_SEC_STAT_RCVD_TRGT_ABRT_BMSK                            0x10000000
#define HWIO_GE_PCIE_SEC_STAT_IO_LIMIT_IO_BASE_REG_SEC_STAT_RCVD_TRGT_ABRT_SHFT                                  0x1c
#define HWIO_GE_PCIE_SEC_STAT_IO_LIMIT_IO_BASE_REG_SEC_STAT_SIG_TRGT_ABRT_BMSK                              0x8000000
#define HWIO_GE_PCIE_SEC_STAT_IO_LIMIT_IO_BASE_REG_SEC_STAT_SIG_TRGT_ABRT_SHFT                                   0x1b
#define HWIO_GE_PCIE_SEC_STAT_IO_LIMIT_IO_BASE_REG_RSVDP_25_BMSK                                            0x6000000
#define HWIO_GE_PCIE_SEC_STAT_IO_LIMIT_IO_BASE_REG_RSVDP_25_SHFT                                                 0x19
#define HWIO_GE_PCIE_SEC_STAT_IO_LIMIT_IO_BASE_REG_SEC_STAT_MDPE_BMSK                                       0x1000000
#define HWIO_GE_PCIE_SEC_STAT_IO_LIMIT_IO_BASE_REG_SEC_STAT_MDPE_SHFT                                            0x18
#define HWIO_GE_PCIE_SEC_STAT_IO_LIMIT_IO_BASE_REG_RSVDP_23_BMSK                                             0x800000
#define HWIO_GE_PCIE_SEC_STAT_IO_LIMIT_IO_BASE_REG_RSVDP_23_SHFT                                                 0x17
#define HWIO_GE_PCIE_SEC_STAT_IO_LIMIT_IO_BASE_REG_SEC_STAT_RESERV_BMSK                                      0x7f0000
#define HWIO_GE_PCIE_SEC_STAT_IO_LIMIT_IO_BASE_REG_SEC_STAT_RESERV_SHFT                                          0x10
#define HWIO_GE_PCIE_SEC_STAT_IO_LIMIT_IO_BASE_REG_IO_LIMIT_BMSK                                               0xf000
#define HWIO_GE_PCIE_SEC_STAT_IO_LIMIT_IO_BASE_REG_IO_LIMIT_SHFT                                                  0xc
#define HWIO_GE_PCIE_SEC_STAT_IO_LIMIT_IO_BASE_REG_IO_RESERV1_BMSK                                              0xe00
#define HWIO_GE_PCIE_SEC_STAT_IO_LIMIT_IO_BASE_REG_IO_RESERV1_SHFT                                                0x9
#define HWIO_GE_PCIE_SEC_STAT_IO_LIMIT_IO_BASE_REG_IO_DECODE_BIT8_BMSK                                          0x100
#define HWIO_GE_PCIE_SEC_STAT_IO_LIMIT_IO_BASE_REG_IO_DECODE_BIT8_SHFT                                            0x8
#define HWIO_GE_PCIE_SEC_STAT_IO_LIMIT_IO_BASE_REG_IO_BASE_BMSK                                                  0xf0
#define HWIO_GE_PCIE_SEC_STAT_IO_LIMIT_IO_BASE_REG_IO_BASE_SHFT                                                   0x4
#define HWIO_GE_PCIE_SEC_STAT_IO_LIMIT_IO_BASE_REG_IO_RESERV_BMSK                                                 0xe
#define HWIO_GE_PCIE_SEC_STAT_IO_LIMIT_IO_BASE_REG_IO_RESERV_SHFT                                                 0x1
#define HWIO_GE_PCIE_SEC_STAT_IO_LIMIT_IO_BASE_REG_IO_DECODE_BMSK                                                 0x1
#define HWIO_GE_PCIE_SEC_STAT_IO_LIMIT_IO_BASE_REG_IO_DECODE_SHFT                                                 0x0

#define HWIO_GE_PCIE_MEM_LIMIT_MEM_BASE_REG_ADDR                                                           (GE_PCIE_PCIE20_DBI_4_REG_BASE      + 0x00000020)
#define HWIO_GE_PCIE_MEM_LIMIT_MEM_BASE_REG_PHYS                                                           (GE_PCIE_PCIE20_DBI_4_REG_BASE_PHYS + 0x00000020)
#define HWIO_GE_PCIE_MEM_LIMIT_MEM_BASE_REG_OFFS                                                           (GE_PCIE_PCIE20_DBI_4_REG_BASE_OFFS + 0x00000020)
#define HWIO_GE_PCIE_MEM_LIMIT_MEM_BASE_REG_RMSK                                                           0xffffffff
#define HWIO_GE_PCIE_MEM_LIMIT_MEM_BASE_REG_POR                                                            0x00000000
#define HWIO_GE_PCIE_MEM_LIMIT_MEM_BASE_REG_POR_RMSK                                                       0xffffffff
#define HWIO_GE_PCIE_MEM_LIMIT_MEM_BASE_REG_ATTR                                                                  0x3
#define HWIO_GE_PCIE_MEM_LIMIT_MEM_BASE_REG_IN          \
        in_dword_masked(HWIO_GE_PCIE_MEM_LIMIT_MEM_BASE_REG_ADDR, HWIO_GE_PCIE_MEM_LIMIT_MEM_BASE_REG_RMSK)
#define HWIO_GE_PCIE_MEM_LIMIT_MEM_BASE_REG_INM(m)      \
        in_dword_masked(HWIO_GE_PCIE_MEM_LIMIT_MEM_BASE_REG_ADDR, m)
#define HWIO_GE_PCIE_MEM_LIMIT_MEM_BASE_REG_OUT(v)      \
        out_dword(HWIO_GE_PCIE_MEM_LIMIT_MEM_BASE_REG_ADDR,v)
#define HWIO_GE_PCIE_MEM_LIMIT_MEM_BASE_REG_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GE_PCIE_MEM_LIMIT_MEM_BASE_REG_ADDR,m,v,HWIO_GE_PCIE_MEM_LIMIT_MEM_BASE_REG_IN)
#define HWIO_GE_PCIE_MEM_LIMIT_MEM_BASE_REG_MEM_LIMIT_BMSK                                                 0xfff00000
#define HWIO_GE_PCIE_MEM_LIMIT_MEM_BASE_REG_MEM_LIMIT_SHFT                                                       0x14
#define HWIO_GE_PCIE_MEM_LIMIT_MEM_BASE_REG_MEM_LIMIT_RESERV_BMSK                                             0xf0000
#define HWIO_GE_PCIE_MEM_LIMIT_MEM_BASE_REG_MEM_LIMIT_RESERV_SHFT                                                0x10
#define HWIO_GE_PCIE_MEM_LIMIT_MEM_BASE_REG_MEM_BASE_BMSK                                                      0xfff0
#define HWIO_GE_PCIE_MEM_LIMIT_MEM_BASE_REG_MEM_BASE_SHFT                                                         0x4
#define HWIO_GE_PCIE_MEM_LIMIT_MEM_BASE_REG_MEM_BASE_RESERV_BMSK                                                  0xf
#define HWIO_GE_PCIE_MEM_LIMIT_MEM_BASE_REG_MEM_BASE_RESERV_SHFT                                                  0x0

#define HWIO_GE_PCIE_PREF_MEM_LIMIT_PREF_MEM_BASE_REG_ADDR                                                 (GE_PCIE_PCIE20_DBI_4_REG_BASE      + 0x00000024)
#define HWIO_GE_PCIE_PREF_MEM_LIMIT_PREF_MEM_BASE_REG_PHYS                                                 (GE_PCIE_PCIE20_DBI_4_REG_BASE_PHYS + 0x00000024)
#define HWIO_GE_PCIE_PREF_MEM_LIMIT_PREF_MEM_BASE_REG_OFFS                                                 (GE_PCIE_PCIE20_DBI_4_REG_BASE_OFFS + 0x00000024)
#define HWIO_GE_PCIE_PREF_MEM_LIMIT_PREF_MEM_BASE_REG_RMSK                                                 0xffffffff
#define HWIO_GE_PCIE_PREF_MEM_LIMIT_PREF_MEM_BASE_REG_POR                                                  0x00000000
#define HWIO_GE_PCIE_PREF_MEM_LIMIT_PREF_MEM_BASE_REG_POR_RMSK                                             0xffffffff
#define HWIO_GE_PCIE_PREF_MEM_LIMIT_PREF_MEM_BASE_REG_ATTR                                                        0x3
#define HWIO_GE_PCIE_PREF_MEM_LIMIT_PREF_MEM_BASE_REG_IN          \
        in_dword_masked(HWIO_GE_PCIE_PREF_MEM_LIMIT_PREF_MEM_BASE_REG_ADDR, HWIO_GE_PCIE_PREF_MEM_LIMIT_PREF_MEM_BASE_REG_RMSK)
#define HWIO_GE_PCIE_PREF_MEM_LIMIT_PREF_MEM_BASE_REG_INM(m)      \
        in_dword_masked(HWIO_GE_PCIE_PREF_MEM_LIMIT_PREF_MEM_BASE_REG_ADDR, m)
#define HWIO_GE_PCIE_PREF_MEM_LIMIT_PREF_MEM_BASE_REG_OUT(v)      \
        out_dword(HWIO_GE_PCIE_PREF_MEM_LIMIT_PREF_MEM_BASE_REG_ADDR,v)
#define HWIO_GE_PCIE_PREF_MEM_LIMIT_PREF_MEM_BASE_REG_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GE_PCIE_PREF_MEM_LIMIT_PREF_MEM_BASE_REG_ADDR,m,v,HWIO_GE_PCIE_PREF_MEM_LIMIT_PREF_MEM_BASE_REG_IN)
#define HWIO_GE_PCIE_PREF_MEM_LIMIT_PREF_MEM_BASE_REG_PREF_MEM_LIMIT_BMSK                                  0xfff00000
#define HWIO_GE_PCIE_PREF_MEM_LIMIT_PREF_MEM_BASE_REG_PREF_MEM_LIMIT_SHFT                                        0x14
#define HWIO_GE_PCIE_PREF_MEM_LIMIT_PREF_MEM_BASE_REG_PREF_RESERV1_BMSK                                       0xe0000
#define HWIO_GE_PCIE_PREF_MEM_LIMIT_PREF_MEM_BASE_REG_PREF_RESERV1_SHFT                                          0x11
#define HWIO_GE_PCIE_PREF_MEM_LIMIT_PREF_MEM_BASE_REG_PREF_MEM_LIMIT_DECODE_BMSK                              0x10000
#define HWIO_GE_PCIE_PREF_MEM_LIMIT_PREF_MEM_BASE_REG_PREF_MEM_LIMIT_DECODE_SHFT                                 0x10
#define HWIO_GE_PCIE_PREF_MEM_LIMIT_PREF_MEM_BASE_REG_PREF_MEM_BASE_BMSK                                       0xfff0
#define HWIO_GE_PCIE_PREF_MEM_LIMIT_PREF_MEM_BASE_REG_PREF_MEM_BASE_SHFT                                          0x4
#define HWIO_GE_PCIE_PREF_MEM_LIMIT_PREF_MEM_BASE_REG_PREF_RESERV_BMSK                                            0xe
#define HWIO_GE_PCIE_PREF_MEM_LIMIT_PREF_MEM_BASE_REG_PREF_RESERV_SHFT                                            0x1
#define HWIO_GE_PCIE_PREF_MEM_LIMIT_PREF_MEM_BASE_REG_PREF_MEM_DECODE_BMSK                                        0x1
#define HWIO_GE_PCIE_PREF_MEM_LIMIT_PREF_MEM_BASE_REG_PREF_MEM_DECODE_SHFT                                        0x0

#define HWIO_GE_PCIE_PREF_BASE_UPPER_REG_ADDR                                                              (GE_PCIE_PCIE20_DBI_4_REG_BASE      + 0x00000028)
#define HWIO_GE_PCIE_PREF_BASE_UPPER_REG_PHYS                                                              (GE_PCIE_PCIE20_DBI_4_REG_BASE_PHYS + 0x00000028)
#define HWIO_GE_PCIE_PREF_BASE_UPPER_REG_OFFS                                                              (GE_PCIE_PCIE20_DBI_4_REG_BASE_OFFS + 0x00000028)
#define HWIO_GE_PCIE_PREF_BASE_UPPER_REG_RMSK                                                              0xffffffff
#define HWIO_GE_PCIE_PREF_BASE_UPPER_REG_POR                                                               0x00000000
#define HWIO_GE_PCIE_PREF_BASE_UPPER_REG_POR_RMSK                                                          0xffffffff
#define HWIO_GE_PCIE_PREF_BASE_UPPER_REG_ATTR                                                                     0x3
#define HWIO_GE_PCIE_PREF_BASE_UPPER_REG_IN          \
        in_dword_masked(HWIO_GE_PCIE_PREF_BASE_UPPER_REG_ADDR, HWIO_GE_PCIE_PREF_BASE_UPPER_REG_RMSK)
#define HWIO_GE_PCIE_PREF_BASE_UPPER_REG_INM(m)      \
        in_dword_masked(HWIO_GE_PCIE_PREF_BASE_UPPER_REG_ADDR, m)
#define HWIO_GE_PCIE_PREF_BASE_UPPER_REG_OUT(v)      \
        out_dword(HWIO_GE_PCIE_PREF_BASE_UPPER_REG_ADDR,v)
#define HWIO_GE_PCIE_PREF_BASE_UPPER_REG_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GE_PCIE_PREF_BASE_UPPER_REG_ADDR,m,v,HWIO_GE_PCIE_PREF_BASE_UPPER_REG_IN)
#define HWIO_GE_PCIE_PREF_BASE_UPPER_REG_PREF_MEM_BASE_UPPER_BMSK                                          0xffffffff
#define HWIO_GE_PCIE_PREF_BASE_UPPER_REG_PREF_MEM_BASE_UPPER_SHFT                                                 0x0

#define HWIO_GE_PCIE_PREF_LIMIT_UPPER_REG_ADDR                                                             (GE_PCIE_PCIE20_DBI_4_REG_BASE      + 0x0000002c)
#define HWIO_GE_PCIE_PREF_LIMIT_UPPER_REG_PHYS                                                             (GE_PCIE_PCIE20_DBI_4_REG_BASE_PHYS + 0x0000002c)
#define HWIO_GE_PCIE_PREF_LIMIT_UPPER_REG_OFFS                                                             (GE_PCIE_PCIE20_DBI_4_REG_BASE_OFFS + 0x0000002c)
#define HWIO_GE_PCIE_PREF_LIMIT_UPPER_REG_RMSK                                                             0xffffffff
#define HWIO_GE_PCIE_PREF_LIMIT_UPPER_REG_POR                                                              0x00000000
#define HWIO_GE_PCIE_PREF_LIMIT_UPPER_REG_POR_RMSK                                                         0xffffffff
#define HWIO_GE_PCIE_PREF_LIMIT_UPPER_REG_ATTR                                                                    0x3
#define HWIO_GE_PCIE_PREF_LIMIT_UPPER_REG_IN          \
        in_dword_masked(HWIO_GE_PCIE_PREF_LIMIT_UPPER_REG_ADDR, HWIO_GE_PCIE_PREF_LIMIT_UPPER_REG_RMSK)
#define HWIO_GE_PCIE_PREF_LIMIT_UPPER_REG_INM(m)      \
        in_dword_masked(HWIO_GE_PCIE_PREF_LIMIT_UPPER_REG_ADDR, m)
#define HWIO_GE_PCIE_PREF_LIMIT_UPPER_REG_OUT(v)      \
        out_dword(HWIO_GE_PCIE_PREF_LIMIT_UPPER_REG_ADDR,v)
#define HWIO_GE_PCIE_PREF_LIMIT_UPPER_REG_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GE_PCIE_PREF_LIMIT_UPPER_REG_ADDR,m,v,HWIO_GE_PCIE_PREF_LIMIT_UPPER_REG_IN)
#define HWIO_GE_PCIE_PREF_LIMIT_UPPER_REG_PREF_MEM_LIMIT_UPPER_BMSK                                        0xffffffff
#define HWIO_GE_PCIE_PREF_LIMIT_UPPER_REG_PREF_MEM_LIMIT_UPPER_SHFT                                               0x0

#define HWIO_GE_PCIE_IO_LIMIT_UPPER_IO_BASE_UPPER_REG_ADDR                                                 (GE_PCIE_PCIE20_DBI_4_REG_BASE      + 0x00000030)
#define HWIO_GE_PCIE_IO_LIMIT_UPPER_IO_BASE_UPPER_REG_PHYS                                                 (GE_PCIE_PCIE20_DBI_4_REG_BASE_PHYS + 0x00000030)
#define HWIO_GE_PCIE_IO_LIMIT_UPPER_IO_BASE_UPPER_REG_OFFS                                                 (GE_PCIE_PCIE20_DBI_4_REG_BASE_OFFS + 0x00000030)
#define HWIO_GE_PCIE_IO_LIMIT_UPPER_IO_BASE_UPPER_REG_RMSK                                                 0xffffffff
#define HWIO_GE_PCIE_IO_LIMIT_UPPER_IO_BASE_UPPER_REG_POR                                                  0x00000000
#define HWIO_GE_PCIE_IO_LIMIT_UPPER_IO_BASE_UPPER_REG_POR_RMSK                                             0xffffffff
#define HWIO_GE_PCIE_IO_LIMIT_UPPER_IO_BASE_UPPER_REG_ATTR                                                        0x1
#define HWIO_GE_PCIE_IO_LIMIT_UPPER_IO_BASE_UPPER_REG_IN          \
        in_dword_masked(HWIO_GE_PCIE_IO_LIMIT_UPPER_IO_BASE_UPPER_REG_ADDR, HWIO_GE_PCIE_IO_LIMIT_UPPER_IO_BASE_UPPER_REG_RMSK)
#define HWIO_GE_PCIE_IO_LIMIT_UPPER_IO_BASE_UPPER_REG_INM(m)      \
        in_dword_masked(HWIO_GE_PCIE_IO_LIMIT_UPPER_IO_BASE_UPPER_REG_ADDR, m)
#define HWIO_GE_PCIE_IO_LIMIT_UPPER_IO_BASE_UPPER_REG_IO_LIMIT_UPPER_BMSK                                  0xffff0000
#define HWIO_GE_PCIE_IO_LIMIT_UPPER_IO_BASE_UPPER_REG_IO_LIMIT_UPPER_SHFT                                        0x10
#define HWIO_GE_PCIE_IO_LIMIT_UPPER_IO_BASE_UPPER_REG_IO_BASE_UPPER_BMSK                                       0xffff
#define HWIO_GE_PCIE_IO_LIMIT_UPPER_IO_BASE_UPPER_REG_IO_BASE_UPPER_SHFT                                          0x0

#define HWIO_GE_PCIE_TYPE1_CAP_PTR_REG_ADDR                                                                (GE_PCIE_PCIE20_DBI_4_REG_BASE      + 0x00000034)
#define HWIO_GE_PCIE_TYPE1_CAP_PTR_REG_PHYS                                                                (GE_PCIE_PCIE20_DBI_4_REG_BASE_PHYS + 0x00000034)
#define HWIO_GE_PCIE_TYPE1_CAP_PTR_REG_OFFS                                                                (GE_PCIE_PCIE20_DBI_4_REG_BASE_OFFS + 0x00000034)
#define HWIO_GE_PCIE_TYPE1_CAP_PTR_REG_RMSK                                                                0xffffffff
#define HWIO_GE_PCIE_TYPE1_CAP_PTR_REG_POR                                                                 0x00000000
#define HWIO_GE_PCIE_TYPE1_CAP_PTR_REG_POR_RMSK                                                            0xffffffff
#define HWIO_GE_PCIE_TYPE1_CAP_PTR_REG_ATTR                                                                       0x1
#define HWIO_GE_PCIE_TYPE1_CAP_PTR_REG_IN          \
        in_dword_masked(HWIO_GE_PCIE_TYPE1_CAP_PTR_REG_ADDR, HWIO_GE_PCIE_TYPE1_CAP_PTR_REG_RMSK)
#define HWIO_GE_PCIE_TYPE1_CAP_PTR_REG_INM(m)      \
        in_dword_masked(HWIO_GE_PCIE_TYPE1_CAP_PTR_REG_ADDR, m)
#define HWIO_GE_PCIE_TYPE1_CAP_PTR_REG_RSVDP_8_BMSK                                                        0xffffff00
#define HWIO_GE_PCIE_TYPE1_CAP_PTR_REG_RSVDP_8_SHFT                                                               0x8
#define HWIO_GE_PCIE_TYPE1_CAP_PTR_REG_CAP_POINTER_BMSK                                                          0xff
#define HWIO_GE_PCIE_TYPE1_CAP_PTR_REG_CAP_POINTER_SHFT                                                           0x0

#define HWIO_GE_PCIE_TYPE1_EXP_ROM_BASE_REG_ADDR                                                           (GE_PCIE_PCIE20_DBI_4_REG_BASE      + 0x00000038)
#define HWIO_GE_PCIE_TYPE1_EXP_ROM_BASE_REG_PHYS                                                           (GE_PCIE_PCIE20_DBI_4_REG_BASE_PHYS + 0x00000038)
#define HWIO_GE_PCIE_TYPE1_EXP_ROM_BASE_REG_OFFS                                                           (GE_PCIE_PCIE20_DBI_4_REG_BASE_OFFS + 0x00000038)
#define HWIO_GE_PCIE_TYPE1_EXP_ROM_BASE_REG_RMSK                                                           0xffffffff
#define HWIO_GE_PCIE_TYPE1_EXP_ROM_BASE_REG_POR                                                            0x00000000
#define HWIO_GE_PCIE_TYPE1_EXP_ROM_BASE_REG_POR_RMSK                                                       0xffffffff
#define HWIO_GE_PCIE_TYPE1_EXP_ROM_BASE_REG_ATTR                                                                  0x3
#define HWIO_GE_PCIE_TYPE1_EXP_ROM_BASE_REG_IN          \
        in_dword_masked(HWIO_GE_PCIE_TYPE1_EXP_ROM_BASE_REG_ADDR, HWIO_GE_PCIE_TYPE1_EXP_ROM_BASE_REG_RMSK)
#define HWIO_GE_PCIE_TYPE1_EXP_ROM_BASE_REG_INM(m)      \
        in_dword_masked(HWIO_GE_PCIE_TYPE1_EXP_ROM_BASE_REG_ADDR, m)
#define HWIO_GE_PCIE_TYPE1_EXP_ROM_BASE_REG_OUT(v)      \
        out_dword(HWIO_GE_PCIE_TYPE1_EXP_ROM_BASE_REG_ADDR,v)
#define HWIO_GE_PCIE_TYPE1_EXP_ROM_BASE_REG_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GE_PCIE_TYPE1_EXP_ROM_BASE_REG_ADDR,m,v,HWIO_GE_PCIE_TYPE1_EXP_ROM_BASE_REG_IN)
#define HWIO_GE_PCIE_TYPE1_EXP_ROM_BASE_REG_EXP_ROM_BASE_ADDRESS_BMSK                                      0xfffff800
#define HWIO_GE_PCIE_TYPE1_EXP_ROM_BASE_REG_EXP_ROM_BASE_ADDRESS_SHFT                                             0xb
#define HWIO_GE_PCIE_TYPE1_EXP_ROM_BASE_REG_RSVDP_1_BMSK                                                        0x7fe
#define HWIO_GE_PCIE_TYPE1_EXP_ROM_BASE_REG_RSVDP_1_SHFT                                                          0x1
#define HWIO_GE_PCIE_TYPE1_EXP_ROM_BASE_REG_ROM_BAR_ENABLE_BMSK                                                   0x1
#define HWIO_GE_PCIE_TYPE1_EXP_ROM_BASE_REG_ROM_BAR_ENABLE_SHFT                                                   0x0

#define HWIO_GE_PCIE_BRIDGE_CTRL_INT_PIN_INT_LINE_REG_ADDR                                                 (GE_PCIE_PCIE20_DBI_4_REG_BASE      + 0x0000003c)
#define HWIO_GE_PCIE_BRIDGE_CTRL_INT_PIN_INT_LINE_REG_PHYS                                                 (GE_PCIE_PCIE20_DBI_4_REG_BASE_PHYS + 0x0000003c)
#define HWIO_GE_PCIE_BRIDGE_CTRL_INT_PIN_INT_LINE_REG_OFFS                                                 (GE_PCIE_PCIE20_DBI_4_REG_BASE_OFFS + 0x0000003c)
#define HWIO_GE_PCIE_BRIDGE_CTRL_INT_PIN_INT_LINE_REG_RMSK                                                 0xffffffff
#define HWIO_GE_PCIE_BRIDGE_CTRL_INT_PIN_INT_LINE_REG_POR                                                  0x00000000
#define HWIO_GE_PCIE_BRIDGE_CTRL_INT_PIN_INT_LINE_REG_POR_RMSK                                             0xffffffff
#define HWIO_GE_PCIE_BRIDGE_CTRL_INT_PIN_INT_LINE_REG_ATTR                                                        0x3
#define HWIO_GE_PCIE_BRIDGE_CTRL_INT_PIN_INT_LINE_REG_IN          \
        in_dword_masked(HWIO_GE_PCIE_BRIDGE_CTRL_INT_PIN_INT_LINE_REG_ADDR, HWIO_GE_PCIE_BRIDGE_CTRL_INT_PIN_INT_LINE_REG_RMSK)
#define HWIO_GE_PCIE_BRIDGE_CTRL_INT_PIN_INT_LINE_REG_INM(m)      \
        in_dword_masked(HWIO_GE_PCIE_BRIDGE_CTRL_INT_PIN_INT_LINE_REG_ADDR, m)
#define HWIO_GE_PCIE_BRIDGE_CTRL_INT_PIN_INT_LINE_REG_OUT(v)      \
        out_dword(HWIO_GE_PCIE_BRIDGE_CTRL_INT_PIN_INT_LINE_REG_ADDR,v)
#define HWIO_GE_PCIE_BRIDGE_CTRL_INT_PIN_INT_LINE_REG_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GE_PCIE_BRIDGE_CTRL_INT_PIN_INT_LINE_REG_ADDR,m,v,HWIO_GE_PCIE_BRIDGE_CTRL_INT_PIN_INT_LINE_REG_IN)
#define HWIO_GE_PCIE_BRIDGE_CTRL_INT_PIN_INT_LINE_REG_BRIDGE_CTRL_RESERV_BMSK                              0xff800000
#define HWIO_GE_PCIE_BRIDGE_CTRL_INT_PIN_INT_LINE_REG_BRIDGE_CTRL_RESERV_SHFT                                    0x17
#define HWIO_GE_PCIE_BRIDGE_CTRL_INT_PIN_INT_LINE_REG_SBR_BMSK                                               0x400000
#define HWIO_GE_PCIE_BRIDGE_CTRL_INT_PIN_INT_LINE_REG_SBR_SHFT                                                   0x16
#define HWIO_GE_PCIE_BRIDGE_CTRL_INT_PIN_INT_LINE_REG_MSTR_ABORT_MODE_BMSK                                   0x200000
#define HWIO_GE_PCIE_BRIDGE_CTRL_INT_PIN_INT_LINE_REG_MSTR_ABORT_MODE_SHFT                                       0x15
#define HWIO_GE_PCIE_BRIDGE_CTRL_INT_PIN_INT_LINE_REG_VGA_16B_DEC_BMSK                                       0x100000
#define HWIO_GE_PCIE_BRIDGE_CTRL_INT_PIN_INT_LINE_REG_VGA_16B_DEC_SHFT                                           0x14
#define HWIO_GE_PCIE_BRIDGE_CTRL_INT_PIN_INT_LINE_REG_VGA_EN_BMSK                                             0x80000
#define HWIO_GE_PCIE_BRIDGE_CTRL_INT_PIN_INT_LINE_REG_VGA_EN_SHFT                                                0x13
#define HWIO_GE_PCIE_BRIDGE_CTRL_INT_PIN_INT_LINE_REG_ISA_EN_BMSK                                             0x40000
#define HWIO_GE_PCIE_BRIDGE_CTRL_INT_PIN_INT_LINE_REG_ISA_EN_SHFT                                                0x12
#define HWIO_GE_PCIE_BRIDGE_CTRL_INT_PIN_INT_LINE_REG_SERR_EN_BMSK                                            0x20000
#define HWIO_GE_PCIE_BRIDGE_CTRL_INT_PIN_INT_LINE_REG_SERR_EN_SHFT                                               0x11
#define HWIO_GE_PCIE_BRIDGE_CTRL_INT_PIN_INT_LINE_REG_PERE_BMSK                                               0x10000
#define HWIO_GE_PCIE_BRIDGE_CTRL_INT_PIN_INT_LINE_REG_PERE_SHFT                                                  0x10
#define HWIO_GE_PCIE_BRIDGE_CTRL_INT_PIN_INT_LINE_REG_INT_PIN_BMSK                                             0xff00
#define HWIO_GE_PCIE_BRIDGE_CTRL_INT_PIN_INT_LINE_REG_INT_PIN_SHFT                                                0x8
#define HWIO_GE_PCIE_BRIDGE_CTRL_INT_PIN_INT_LINE_REG_INT_LINE_BMSK                                              0xff
#define HWIO_GE_PCIE_BRIDGE_CTRL_INT_PIN_INT_LINE_REG_INT_LINE_SHFT                                               0x0

#define HWIO_GE_PCIE_CAP_ID_NXT_PTR_REG_ADDR                                                               (GE_PCIE_PCIE20_DBI_4_REG_BASE      + 0x00000040)
#define HWIO_GE_PCIE_CAP_ID_NXT_PTR_REG_PHYS                                                               (GE_PCIE_PCIE20_DBI_4_REG_BASE_PHYS + 0x00000040)
#define HWIO_GE_PCIE_CAP_ID_NXT_PTR_REG_OFFS                                                               (GE_PCIE_PCIE20_DBI_4_REG_BASE_OFFS + 0x00000040)
#define HWIO_GE_PCIE_CAP_ID_NXT_PTR_REG_RMSK                                                               0xffffffff
#define HWIO_GE_PCIE_CAP_ID_NXT_PTR_REG_POR                                                                0x00000000
#define HWIO_GE_PCIE_CAP_ID_NXT_PTR_REG_POR_RMSK                                                           0xffffffff
#define HWIO_GE_PCIE_CAP_ID_NXT_PTR_REG_ATTR                                                                      0x3
#define HWIO_GE_PCIE_CAP_ID_NXT_PTR_REG_IN          \
        in_dword_masked(HWIO_GE_PCIE_CAP_ID_NXT_PTR_REG_ADDR, HWIO_GE_PCIE_CAP_ID_NXT_PTR_REG_RMSK)
#define HWIO_GE_PCIE_CAP_ID_NXT_PTR_REG_INM(m)      \
        in_dword_masked(HWIO_GE_PCIE_CAP_ID_NXT_PTR_REG_ADDR, m)
#define HWIO_GE_PCIE_CAP_ID_NXT_PTR_REG_OUT(v)      \
        out_dword(HWIO_GE_PCIE_CAP_ID_NXT_PTR_REG_ADDR,v)
#define HWIO_GE_PCIE_CAP_ID_NXT_PTR_REG_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GE_PCIE_CAP_ID_NXT_PTR_REG_ADDR,m,v,HWIO_GE_PCIE_CAP_ID_NXT_PTR_REG_IN)
#define HWIO_GE_PCIE_CAP_ID_NXT_PTR_REG_PME_SUPPORT_BMSK                                                   0xf8000000
#define HWIO_GE_PCIE_CAP_ID_NXT_PTR_REG_PME_SUPPORT_SHFT                                                         0x1b
#define HWIO_GE_PCIE_CAP_ID_NXT_PTR_REG_D2_SUPPORT_BMSK                                                     0x4000000
#define HWIO_GE_PCIE_CAP_ID_NXT_PTR_REG_D2_SUPPORT_SHFT                                                          0x1a
#define HWIO_GE_PCIE_CAP_ID_NXT_PTR_REG_D1_SUPPORT_BMSK                                                     0x2000000
#define HWIO_GE_PCIE_CAP_ID_NXT_PTR_REG_D1_SUPPORT_SHFT                                                          0x19
#define HWIO_GE_PCIE_CAP_ID_NXT_PTR_REG_AUX_CURR_BMSK                                                       0x1c00000
#define HWIO_GE_PCIE_CAP_ID_NXT_PTR_REG_AUX_CURR_SHFT                                                            0x16
#define HWIO_GE_PCIE_CAP_ID_NXT_PTR_REG_DSI_BMSK                                                             0x200000
#define HWIO_GE_PCIE_CAP_ID_NXT_PTR_REG_DSI_SHFT                                                                 0x15
#define HWIO_GE_PCIE_CAP_ID_NXT_PTR_REG_RSVDP_20_BMSK                                                        0x100000
#define HWIO_GE_PCIE_CAP_ID_NXT_PTR_REG_RSVDP_20_SHFT                                                            0x14
#define HWIO_GE_PCIE_CAP_ID_NXT_PTR_REG_PME_CLK_BMSK                                                          0x80000
#define HWIO_GE_PCIE_CAP_ID_NXT_PTR_REG_PME_CLK_SHFT                                                             0x13
#define HWIO_GE_PCIE_CAP_ID_NXT_PTR_REG_PM_SPEC_VER_BMSK                                                      0x70000
#define HWIO_GE_PCIE_CAP_ID_NXT_PTR_REG_PM_SPEC_VER_SHFT                                                         0x10
#define HWIO_GE_PCIE_CAP_ID_NXT_PTR_REG_PM_NEXT_POINTER_BMSK                                                   0xff00
#define HWIO_GE_PCIE_CAP_ID_NXT_PTR_REG_PM_NEXT_POINTER_SHFT                                                      0x8
#define HWIO_GE_PCIE_CAP_ID_NXT_PTR_REG_PM_CAP_ID_BMSK                                                           0xff
#define HWIO_GE_PCIE_CAP_ID_NXT_PTR_REG_PM_CAP_ID_SHFT                                                            0x0

#define HWIO_GE_PCIE_CON_STATUS_REG_ADDR                                                                   (GE_PCIE_PCIE20_DBI_4_REG_BASE      + 0x00000044)
#define HWIO_GE_PCIE_CON_STATUS_REG_PHYS                                                                   (GE_PCIE_PCIE20_DBI_4_REG_BASE_PHYS + 0x00000044)
#define HWIO_GE_PCIE_CON_STATUS_REG_OFFS                                                                   (GE_PCIE_PCIE20_DBI_4_REG_BASE_OFFS + 0x00000044)
#define HWIO_GE_PCIE_CON_STATUS_REG_RMSK                                                                   0xffffffff
#define HWIO_GE_PCIE_CON_STATUS_REG_POR                                                                    0x00000000
#define HWIO_GE_PCIE_CON_STATUS_REG_POR_RMSK                                                               0xffffffff
#define HWIO_GE_PCIE_CON_STATUS_REG_ATTR                                                                          0x3
#define HWIO_GE_PCIE_CON_STATUS_REG_IN          \
        in_dword_masked(HWIO_GE_PCIE_CON_STATUS_REG_ADDR, HWIO_GE_PCIE_CON_STATUS_REG_RMSK)
#define HWIO_GE_PCIE_CON_STATUS_REG_INM(m)      \
        in_dword_masked(HWIO_GE_PCIE_CON_STATUS_REG_ADDR, m)
#define HWIO_GE_PCIE_CON_STATUS_REG_OUT(v)      \
        out_dword(HWIO_GE_PCIE_CON_STATUS_REG_ADDR,v)
#define HWIO_GE_PCIE_CON_STATUS_REG_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GE_PCIE_CON_STATUS_REG_ADDR,m,v,HWIO_GE_PCIE_CON_STATUS_REG_IN)
#define HWIO_GE_PCIE_CON_STATUS_REG_DATA_REG_ADD_INFO_BMSK                                                 0xff000000
#define HWIO_GE_PCIE_CON_STATUS_REG_DATA_REG_ADD_INFO_SHFT                                                       0x18
#define HWIO_GE_PCIE_CON_STATUS_REG_BUS_PWR_CLK_CON_EN_BMSK                                                  0x800000
#define HWIO_GE_PCIE_CON_STATUS_REG_BUS_PWR_CLK_CON_EN_SHFT                                                      0x17
#define HWIO_GE_PCIE_CON_STATUS_REG_B2_B3_SUPPORT_BMSK                                                       0x400000
#define HWIO_GE_PCIE_CON_STATUS_REG_B2_B3_SUPPORT_SHFT                                                           0x16
#define HWIO_GE_PCIE_CON_STATUS_REG_RSVDP_16_BMSK                                                            0x3f0000
#define HWIO_GE_PCIE_CON_STATUS_REG_RSVDP_16_SHFT                                                                0x10
#define HWIO_GE_PCIE_CON_STATUS_REG_PME_STATUS_BMSK                                                            0x8000
#define HWIO_GE_PCIE_CON_STATUS_REG_PME_STATUS_SHFT                                                               0xf
#define HWIO_GE_PCIE_CON_STATUS_REG_DATA_SCALE_BMSK                                                            0x6000
#define HWIO_GE_PCIE_CON_STATUS_REG_DATA_SCALE_SHFT                                                               0xd
#define HWIO_GE_PCIE_CON_STATUS_REG_DATA_SELECT_BMSK                                                           0x1e00
#define HWIO_GE_PCIE_CON_STATUS_REG_DATA_SELECT_SHFT                                                              0x9
#define HWIO_GE_PCIE_CON_STATUS_REG_PME_ENABLE_BMSK                                                             0x100
#define HWIO_GE_PCIE_CON_STATUS_REG_PME_ENABLE_SHFT                                                               0x8
#define HWIO_GE_PCIE_CON_STATUS_REG_RSVDP_4_BMSK                                                                 0xf0
#define HWIO_GE_PCIE_CON_STATUS_REG_RSVDP_4_SHFT                                                                  0x4
#define HWIO_GE_PCIE_CON_STATUS_REG_NO_SOFT_RST_BMSK                                                              0x8
#define HWIO_GE_PCIE_CON_STATUS_REG_NO_SOFT_RST_SHFT                                                              0x3
#define HWIO_GE_PCIE_CON_STATUS_REG_RSVDP_2_BMSK                                                                  0x4
#define HWIO_GE_PCIE_CON_STATUS_REG_RSVDP_2_SHFT                                                                  0x2
#define HWIO_GE_PCIE_CON_STATUS_REG_POWER_STATE_BMSK                                                              0x3
#define HWIO_GE_PCIE_CON_STATUS_REG_POWER_STATE_SHFT                                                              0x0

#define HWIO_GE_PCIE_PCIE_CAP_ID_PCIE_NEXT_CAP_PTR_PCIE_CAP_REG_ADDR                                       (GE_PCIE_PCIE20_DBI_4_REG_BASE      + 0x00000070)
#define HWIO_GE_PCIE_PCIE_CAP_ID_PCIE_NEXT_CAP_PTR_PCIE_CAP_REG_PHYS                                       (GE_PCIE_PCIE20_DBI_4_REG_BASE_PHYS + 0x00000070)
#define HWIO_GE_PCIE_PCIE_CAP_ID_PCIE_NEXT_CAP_PTR_PCIE_CAP_REG_OFFS                                       (GE_PCIE_PCIE20_DBI_4_REG_BASE_OFFS + 0x00000070)
#define HWIO_GE_PCIE_PCIE_CAP_ID_PCIE_NEXT_CAP_PTR_PCIE_CAP_REG_RMSK                                       0xffffffff
#define HWIO_GE_PCIE_PCIE_CAP_ID_PCIE_NEXT_CAP_PTR_PCIE_CAP_REG_POR                                        0x00000000
#define HWIO_GE_PCIE_PCIE_CAP_ID_PCIE_NEXT_CAP_PTR_PCIE_CAP_REG_POR_RMSK                                   0xffffffff
#define HWIO_GE_PCIE_PCIE_CAP_ID_PCIE_NEXT_CAP_PTR_PCIE_CAP_REG_ATTR                                              0x1
#define HWIO_GE_PCIE_PCIE_CAP_ID_PCIE_NEXT_CAP_PTR_PCIE_CAP_REG_IN          \
        in_dword_masked(HWIO_GE_PCIE_PCIE_CAP_ID_PCIE_NEXT_CAP_PTR_PCIE_CAP_REG_ADDR, HWIO_GE_PCIE_PCIE_CAP_ID_PCIE_NEXT_CAP_PTR_PCIE_CAP_REG_RMSK)
#define HWIO_GE_PCIE_PCIE_CAP_ID_PCIE_NEXT_CAP_PTR_PCIE_CAP_REG_INM(m)      \
        in_dword_masked(HWIO_GE_PCIE_PCIE_CAP_ID_PCIE_NEXT_CAP_PTR_PCIE_CAP_REG_ADDR, m)
#define HWIO_GE_PCIE_PCIE_CAP_ID_PCIE_NEXT_CAP_PTR_PCIE_CAP_REG_RSVDP_31_BMSK                              0x80000000
#define HWIO_GE_PCIE_PCIE_CAP_ID_PCIE_NEXT_CAP_PTR_PCIE_CAP_REG_RSVDP_31_SHFT                                    0x1f
#define HWIO_GE_PCIE_PCIE_CAP_ID_PCIE_NEXT_CAP_PTR_PCIE_CAP_REG_RSVD_BMSK                                  0x40000000
#define HWIO_GE_PCIE_PCIE_CAP_ID_PCIE_NEXT_CAP_PTR_PCIE_CAP_REG_RSVD_SHFT                                        0x1e
#define HWIO_GE_PCIE_PCIE_CAP_ID_PCIE_NEXT_CAP_PTR_PCIE_CAP_REG_PCIE_INT_MSG_NUM_BMSK                      0x3e000000
#define HWIO_GE_PCIE_PCIE_CAP_ID_PCIE_NEXT_CAP_PTR_PCIE_CAP_REG_PCIE_INT_MSG_NUM_SHFT                            0x19
#define HWIO_GE_PCIE_PCIE_CAP_ID_PCIE_NEXT_CAP_PTR_PCIE_CAP_REG_PCIE_SLOT_IMP_BMSK                          0x1000000
#define HWIO_GE_PCIE_PCIE_CAP_ID_PCIE_NEXT_CAP_PTR_PCIE_CAP_REG_PCIE_SLOT_IMP_SHFT                               0x18
#define HWIO_GE_PCIE_PCIE_CAP_ID_PCIE_NEXT_CAP_PTR_PCIE_CAP_REG_PCIE_DEV_PORT_TYPE_BMSK                      0xf00000
#define HWIO_GE_PCIE_PCIE_CAP_ID_PCIE_NEXT_CAP_PTR_PCIE_CAP_REG_PCIE_DEV_PORT_TYPE_SHFT                          0x14
#define HWIO_GE_PCIE_PCIE_CAP_ID_PCIE_NEXT_CAP_PTR_PCIE_CAP_REG_PCIE_CAP_REG_BMSK                             0xf0000
#define HWIO_GE_PCIE_PCIE_CAP_ID_PCIE_NEXT_CAP_PTR_PCIE_CAP_REG_PCIE_CAP_REG_SHFT                                0x10
#define HWIO_GE_PCIE_PCIE_CAP_ID_PCIE_NEXT_CAP_PTR_PCIE_CAP_REG_PCIE_CAP_NEXT_PTR_BMSK                         0xff00
#define HWIO_GE_PCIE_PCIE_CAP_ID_PCIE_NEXT_CAP_PTR_PCIE_CAP_REG_PCIE_CAP_NEXT_PTR_SHFT                            0x8
#define HWIO_GE_PCIE_PCIE_CAP_ID_PCIE_NEXT_CAP_PTR_PCIE_CAP_REG_PCIE_CAP_ID_BMSK                                 0xff
#define HWIO_GE_PCIE_PCIE_CAP_ID_PCIE_NEXT_CAP_PTR_PCIE_CAP_REG_PCIE_CAP_ID_SHFT                                  0x0

#define HWIO_GE_PCIE_DEVICE_CAPABILITIES_REG_ADDR                                                          (GE_PCIE_PCIE20_DBI_4_REG_BASE      + 0x00000074)
#define HWIO_GE_PCIE_DEVICE_CAPABILITIES_REG_PHYS                                                          (GE_PCIE_PCIE20_DBI_4_REG_BASE_PHYS + 0x00000074)
#define HWIO_GE_PCIE_DEVICE_CAPABILITIES_REG_OFFS                                                          (GE_PCIE_PCIE20_DBI_4_REG_BASE_OFFS + 0x00000074)
#define HWIO_GE_PCIE_DEVICE_CAPABILITIES_REG_RMSK                                                          0xffffffff
#define HWIO_GE_PCIE_DEVICE_CAPABILITIES_REG_POR                                                           0x00000000
#define HWIO_GE_PCIE_DEVICE_CAPABILITIES_REG_POR_RMSK                                                      0xffffffff
#define HWIO_GE_PCIE_DEVICE_CAPABILITIES_REG_ATTR                                                                 0x1
#define HWIO_GE_PCIE_DEVICE_CAPABILITIES_REG_IN          \
        in_dword_masked(HWIO_GE_PCIE_DEVICE_CAPABILITIES_REG_ADDR, HWIO_GE_PCIE_DEVICE_CAPABILITIES_REG_RMSK)
#define HWIO_GE_PCIE_DEVICE_CAPABILITIES_REG_INM(m)      \
        in_dword_masked(HWIO_GE_PCIE_DEVICE_CAPABILITIES_REG_ADDR, m)
#define HWIO_GE_PCIE_DEVICE_CAPABILITIES_REG_RSVDP_16_BMSK                                                 0xffff0000
#define HWIO_GE_PCIE_DEVICE_CAPABILITIES_REG_RSVDP_16_SHFT                                                       0x10
#define HWIO_GE_PCIE_DEVICE_CAPABILITIES_REG_PCIE_CAP_ROLE_BASED_ERR_REPORT_BMSK                               0x8000
#define HWIO_GE_PCIE_DEVICE_CAPABILITIES_REG_PCIE_CAP_ROLE_BASED_ERR_REPORT_SHFT                                  0xf
#define HWIO_GE_PCIE_DEVICE_CAPABILITIES_REG_RSVDP_6_BMSK                                                      0x7fc0
#define HWIO_GE_PCIE_DEVICE_CAPABILITIES_REG_RSVDP_6_SHFT                                                         0x6
#define HWIO_GE_PCIE_DEVICE_CAPABILITIES_REG_PCIE_CAP_EXT_TAG_SUPP_BMSK                                          0x20
#define HWIO_GE_PCIE_DEVICE_CAPABILITIES_REG_PCIE_CAP_EXT_TAG_SUPP_SHFT                                           0x5
#define HWIO_GE_PCIE_DEVICE_CAPABILITIES_REG_PCIE_CAP_PHANTOM_FUNC_SUPPORT_BMSK                                  0x18
#define HWIO_GE_PCIE_DEVICE_CAPABILITIES_REG_PCIE_CAP_PHANTOM_FUNC_SUPPORT_SHFT                                   0x3
#define HWIO_GE_PCIE_DEVICE_CAPABILITIES_REG_PCIE_CAP_MAX_PAYLOAD_SIZE_BMSK                                       0x7
#define HWIO_GE_PCIE_DEVICE_CAPABILITIES_REG_PCIE_CAP_MAX_PAYLOAD_SIZE_SHFT                                       0x0

#define HWIO_GE_PCIE_DEVICE_CONTROL_DEVICE_STATUS_ADDR                                                     (GE_PCIE_PCIE20_DBI_4_REG_BASE      + 0x00000078)
#define HWIO_GE_PCIE_DEVICE_CONTROL_DEVICE_STATUS_PHYS                                                     (GE_PCIE_PCIE20_DBI_4_REG_BASE_PHYS + 0x00000078)
#define HWIO_GE_PCIE_DEVICE_CONTROL_DEVICE_STATUS_OFFS                                                     (GE_PCIE_PCIE20_DBI_4_REG_BASE_OFFS + 0x00000078)
#define HWIO_GE_PCIE_DEVICE_CONTROL_DEVICE_STATUS_RMSK                                                     0xffffffff
#define HWIO_GE_PCIE_DEVICE_CONTROL_DEVICE_STATUS_POR                                                      0x00000000
#define HWIO_GE_PCIE_DEVICE_CONTROL_DEVICE_STATUS_POR_RMSK                                                 0xffffffff
#define HWIO_GE_PCIE_DEVICE_CONTROL_DEVICE_STATUS_ATTR                                                            0x3
#define HWIO_GE_PCIE_DEVICE_CONTROL_DEVICE_STATUS_IN          \
        in_dword_masked(HWIO_GE_PCIE_DEVICE_CONTROL_DEVICE_STATUS_ADDR, HWIO_GE_PCIE_DEVICE_CONTROL_DEVICE_STATUS_RMSK)
#define HWIO_GE_PCIE_DEVICE_CONTROL_DEVICE_STATUS_INM(m)      \
        in_dword_masked(HWIO_GE_PCIE_DEVICE_CONTROL_DEVICE_STATUS_ADDR, m)
#define HWIO_GE_PCIE_DEVICE_CONTROL_DEVICE_STATUS_OUT(v)      \
        out_dword(HWIO_GE_PCIE_DEVICE_CONTROL_DEVICE_STATUS_ADDR,v)
#define HWIO_GE_PCIE_DEVICE_CONTROL_DEVICE_STATUS_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GE_PCIE_DEVICE_CONTROL_DEVICE_STATUS_ADDR,m,v,HWIO_GE_PCIE_DEVICE_CONTROL_DEVICE_STATUS_IN)
#define HWIO_GE_PCIE_DEVICE_CONTROL_DEVICE_STATUS_RSVDP_22_BMSK                                            0xffc00000
#define HWIO_GE_PCIE_DEVICE_CONTROL_DEVICE_STATUS_RSVDP_22_SHFT                                                  0x16
#define HWIO_GE_PCIE_DEVICE_CONTROL_DEVICE_STATUS_PCIE_CAP_TRANS_PENDING_BMSK                                0x200000
#define HWIO_GE_PCIE_DEVICE_CONTROL_DEVICE_STATUS_PCIE_CAP_TRANS_PENDING_SHFT                                    0x15
#define HWIO_GE_PCIE_DEVICE_CONTROL_DEVICE_STATUS_PCIE_CAP_AUX_POWER_DETECTED_BMSK                           0x100000
#define HWIO_GE_PCIE_DEVICE_CONTROL_DEVICE_STATUS_PCIE_CAP_AUX_POWER_DETECTED_SHFT                               0x14
#define HWIO_GE_PCIE_DEVICE_CONTROL_DEVICE_STATUS_PCIE_CAP_UNSUPPORTED_REQ_DETECTED_BMSK                      0x80000
#define HWIO_GE_PCIE_DEVICE_CONTROL_DEVICE_STATUS_PCIE_CAP_UNSUPPORTED_REQ_DETECTED_SHFT                         0x13
#define HWIO_GE_PCIE_DEVICE_CONTROL_DEVICE_STATUS_PCIE_CAP_FATAL_ERR_DETECTED_BMSK                            0x40000
#define HWIO_GE_PCIE_DEVICE_CONTROL_DEVICE_STATUS_PCIE_CAP_FATAL_ERR_DETECTED_SHFT                               0x12
#define HWIO_GE_PCIE_DEVICE_CONTROL_DEVICE_STATUS_PCIE_CAP_NON_FATAL_ERR_DETECTED_BMSK                        0x20000
#define HWIO_GE_PCIE_DEVICE_CONTROL_DEVICE_STATUS_PCIE_CAP_NON_FATAL_ERR_DETECTED_SHFT                           0x11
#define HWIO_GE_PCIE_DEVICE_CONTROL_DEVICE_STATUS_PCIE_CAP_CORR_ERR_DETECTED_BMSK                             0x10000
#define HWIO_GE_PCIE_DEVICE_CONTROL_DEVICE_STATUS_PCIE_CAP_CORR_ERR_DETECTED_SHFT                                0x10
#define HWIO_GE_PCIE_DEVICE_CONTROL_DEVICE_STATUS_PCIE_CAP_INITIATE_FLR_BMSK                                   0x8000
#define HWIO_GE_PCIE_DEVICE_CONTROL_DEVICE_STATUS_PCIE_CAP_INITIATE_FLR_SHFT                                      0xf
#define HWIO_GE_PCIE_DEVICE_CONTROL_DEVICE_STATUS_PCIE_CAP_MAX_READ_REQ_SIZE_BMSK                              0x7000
#define HWIO_GE_PCIE_DEVICE_CONTROL_DEVICE_STATUS_PCIE_CAP_MAX_READ_REQ_SIZE_SHFT                                 0xc
#define HWIO_GE_PCIE_DEVICE_CONTROL_DEVICE_STATUS_PCIE_CAP_EN_NO_SNOOP_BMSK                                     0x800
#define HWIO_GE_PCIE_DEVICE_CONTROL_DEVICE_STATUS_PCIE_CAP_EN_NO_SNOOP_SHFT                                       0xb
#define HWIO_GE_PCIE_DEVICE_CONTROL_DEVICE_STATUS_PCIE_CAP_AUX_POWER_PM_EN_BMSK                                 0x400
#define HWIO_GE_PCIE_DEVICE_CONTROL_DEVICE_STATUS_PCIE_CAP_AUX_POWER_PM_EN_SHFT                                   0xa
#define HWIO_GE_PCIE_DEVICE_CONTROL_DEVICE_STATUS_PCIE_CAP_PHANTOM_FUNC_EN_BMSK                                 0x200
#define HWIO_GE_PCIE_DEVICE_CONTROL_DEVICE_STATUS_PCIE_CAP_PHANTOM_FUNC_EN_SHFT                                   0x9
#define HWIO_GE_PCIE_DEVICE_CONTROL_DEVICE_STATUS_PCIE_CAP_EXT_TAG_EN_BMSK                                      0x100
#define HWIO_GE_PCIE_DEVICE_CONTROL_DEVICE_STATUS_PCIE_CAP_EXT_TAG_EN_SHFT                                        0x8
#define HWIO_GE_PCIE_DEVICE_CONTROL_DEVICE_STATUS_PCIE_CAP_MAX_PAYLOAD_SIZE_CS_BMSK                              0xe0
#define HWIO_GE_PCIE_DEVICE_CONTROL_DEVICE_STATUS_PCIE_CAP_MAX_PAYLOAD_SIZE_CS_SHFT                               0x5
#define HWIO_GE_PCIE_DEVICE_CONTROL_DEVICE_STATUS_PCIE_CAP_EN_REL_ORDER_BMSK                                     0x10
#define HWIO_GE_PCIE_DEVICE_CONTROL_DEVICE_STATUS_PCIE_CAP_EN_REL_ORDER_SHFT                                      0x4
#define HWIO_GE_PCIE_DEVICE_CONTROL_DEVICE_STATUS_PCIE_CAP_UNSUPPORT_REQ_REP_EN_BMSK                              0x8
#define HWIO_GE_PCIE_DEVICE_CONTROL_DEVICE_STATUS_PCIE_CAP_UNSUPPORT_REQ_REP_EN_SHFT                              0x3
#define HWIO_GE_PCIE_DEVICE_CONTROL_DEVICE_STATUS_PCIE_CAP_FATAL_ERR_REPORT_EN_BMSK                               0x4
#define HWIO_GE_PCIE_DEVICE_CONTROL_DEVICE_STATUS_PCIE_CAP_FATAL_ERR_REPORT_EN_SHFT                               0x2
#define HWIO_GE_PCIE_DEVICE_CONTROL_DEVICE_STATUS_PCIE_CAP_NON_FATAL_ERR_REPORT_EN_BMSK                           0x2
#define HWIO_GE_PCIE_DEVICE_CONTROL_DEVICE_STATUS_PCIE_CAP_NON_FATAL_ERR_REPORT_EN_SHFT                           0x1
#define HWIO_GE_PCIE_DEVICE_CONTROL_DEVICE_STATUS_PCIE_CAP_CORR_ERR_REPORT_EN_BMSK                                0x1
#define HWIO_GE_PCIE_DEVICE_CONTROL_DEVICE_STATUS_PCIE_CAP_CORR_ERR_REPORT_EN_SHFT                                0x0

#define HWIO_GE_PCIE_LINK_CAPABILITIES_REG_ADDR                                                            (GE_PCIE_PCIE20_DBI_4_REG_BASE      + 0x0000007c)
#define HWIO_GE_PCIE_LINK_CAPABILITIES_REG_PHYS                                                            (GE_PCIE_PCIE20_DBI_4_REG_BASE_PHYS + 0x0000007c)
#define HWIO_GE_PCIE_LINK_CAPABILITIES_REG_OFFS                                                            (GE_PCIE_PCIE20_DBI_4_REG_BASE_OFFS + 0x0000007c)
#define HWIO_GE_PCIE_LINK_CAPABILITIES_REG_RMSK                                                            0xffffffff
#define HWIO_GE_PCIE_LINK_CAPABILITIES_REG_POR                                                             0x00000000
#define HWIO_GE_PCIE_LINK_CAPABILITIES_REG_POR_RMSK                                                        0xffffffff
#define HWIO_GE_PCIE_LINK_CAPABILITIES_REG_ATTR                                                                   0x3
#define HWIO_GE_PCIE_LINK_CAPABILITIES_REG_IN          \
        in_dword_masked(HWIO_GE_PCIE_LINK_CAPABILITIES_REG_ADDR, HWIO_GE_PCIE_LINK_CAPABILITIES_REG_RMSK)
#define HWIO_GE_PCIE_LINK_CAPABILITIES_REG_INM(m)      \
        in_dword_masked(HWIO_GE_PCIE_LINK_CAPABILITIES_REG_ADDR, m)
#define HWIO_GE_PCIE_LINK_CAPABILITIES_REG_OUT(v)      \
        out_dword(HWIO_GE_PCIE_LINK_CAPABILITIES_REG_ADDR,v)
#define HWIO_GE_PCIE_LINK_CAPABILITIES_REG_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GE_PCIE_LINK_CAPABILITIES_REG_ADDR,m,v,HWIO_GE_PCIE_LINK_CAPABILITIES_REG_IN)
#define HWIO_GE_PCIE_LINK_CAPABILITIES_REG_PCIE_CAP_PORT_NUM_BMSK                                          0xff000000
#define HWIO_GE_PCIE_LINK_CAPABILITIES_REG_PCIE_CAP_PORT_NUM_SHFT                                                0x18
#define HWIO_GE_PCIE_LINK_CAPABILITIES_REG_RSVDP_23_BMSK                                                     0x800000
#define HWIO_GE_PCIE_LINK_CAPABILITIES_REG_RSVDP_23_SHFT                                                         0x17
#define HWIO_GE_PCIE_LINK_CAPABILITIES_REG_PCIE_CAP_ASPM_OPT_COMPLIANCE_BMSK                                 0x400000
#define HWIO_GE_PCIE_LINK_CAPABILITIES_REG_PCIE_CAP_ASPM_OPT_COMPLIANCE_SHFT                                     0x16
#define HWIO_GE_PCIE_LINK_CAPABILITIES_REG_PCIE_CAP_LINK_BW_NOT_CAP_BMSK                                     0x200000
#define HWIO_GE_PCIE_LINK_CAPABILITIES_REG_PCIE_CAP_LINK_BW_NOT_CAP_SHFT                                         0x15
#define HWIO_GE_PCIE_LINK_CAPABILITIES_REG_PCIE_CAP_DLL_ACTIVE_REP_CAP_BMSK                                  0x100000
#define HWIO_GE_PCIE_LINK_CAPABILITIES_REG_PCIE_CAP_DLL_ACTIVE_REP_CAP_SHFT                                      0x14
#define HWIO_GE_PCIE_LINK_CAPABILITIES_REG_PCIE_CAP_SURPRISE_DOWN_ERR_REP_CAP_BMSK                            0x80000
#define HWIO_GE_PCIE_LINK_CAPABILITIES_REG_PCIE_CAP_SURPRISE_DOWN_ERR_REP_CAP_SHFT                               0x13
#define HWIO_GE_PCIE_LINK_CAPABILITIES_REG_PCIE_CAP_CLOCK_POWER_MAN_BMSK                                      0x40000
#define HWIO_GE_PCIE_LINK_CAPABILITIES_REG_PCIE_CAP_CLOCK_POWER_MAN_SHFT                                         0x12
#define HWIO_GE_PCIE_LINK_CAPABILITIES_REG_PCIE_CAP_L1_EXIT_LATENCY_BMSK                                      0x38000
#define HWIO_GE_PCIE_LINK_CAPABILITIES_REG_PCIE_CAP_L1_EXIT_LATENCY_SHFT                                          0xf
#define HWIO_GE_PCIE_LINK_CAPABILITIES_REG_PCIE_CAP_L0S_EXIT_LATENCY_BMSK                                      0x7000
#define HWIO_GE_PCIE_LINK_CAPABILITIES_REG_PCIE_CAP_L0S_EXIT_LATENCY_SHFT                                         0xc
#define HWIO_GE_PCIE_LINK_CAPABILITIES_REG_PCIE_CAP_ACTIVE_STATE_LINK_PM_SUPPORT_BMSK                           0xc00
#define HWIO_GE_PCIE_LINK_CAPABILITIES_REG_PCIE_CAP_ACTIVE_STATE_LINK_PM_SUPPORT_SHFT                             0xa
#define HWIO_GE_PCIE_LINK_CAPABILITIES_REG_PCIE_CAP_MAX_LINK_WIDTH_BMSK                                         0x3f0
#define HWIO_GE_PCIE_LINK_CAPABILITIES_REG_PCIE_CAP_MAX_LINK_WIDTH_SHFT                                           0x4
#define HWIO_GE_PCIE_LINK_CAPABILITIES_REG_PCIE_CAP_MAX_LINK_SPEED_BMSK                                           0xf
#define HWIO_GE_PCIE_LINK_CAPABILITIES_REG_PCIE_CAP_MAX_LINK_SPEED_SHFT                                           0x0

#define HWIO_GE_PCIE_LINK_CONTROL_LINK_STATUS_REG_ADDR                                                     (GE_PCIE_PCIE20_DBI_4_REG_BASE      + 0x00000080)
#define HWIO_GE_PCIE_LINK_CONTROL_LINK_STATUS_REG_PHYS                                                     (GE_PCIE_PCIE20_DBI_4_REG_BASE_PHYS + 0x00000080)
#define HWIO_GE_PCIE_LINK_CONTROL_LINK_STATUS_REG_OFFS                                                     (GE_PCIE_PCIE20_DBI_4_REG_BASE_OFFS + 0x00000080)
#define HWIO_GE_PCIE_LINK_CONTROL_LINK_STATUS_REG_RMSK                                                     0xffffffff
#define HWIO_GE_PCIE_LINK_CONTROL_LINK_STATUS_REG_POR                                                      0x00000000
#define HWIO_GE_PCIE_LINK_CONTROL_LINK_STATUS_REG_POR_RMSK                                                 0xffffffff
#define HWIO_GE_PCIE_LINK_CONTROL_LINK_STATUS_REG_ATTR                                                            0x3
#define HWIO_GE_PCIE_LINK_CONTROL_LINK_STATUS_REG_IN          \
        in_dword_masked(HWIO_GE_PCIE_LINK_CONTROL_LINK_STATUS_REG_ADDR, HWIO_GE_PCIE_LINK_CONTROL_LINK_STATUS_REG_RMSK)
#define HWIO_GE_PCIE_LINK_CONTROL_LINK_STATUS_REG_INM(m)      \
        in_dword_masked(HWIO_GE_PCIE_LINK_CONTROL_LINK_STATUS_REG_ADDR, m)
#define HWIO_GE_PCIE_LINK_CONTROL_LINK_STATUS_REG_OUT(v)      \
        out_dword(HWIO_GE_PCIE_LINK_CONTROL_LINK_STATUS_REG_ADDR,v)
#define HWIO_GE_PCIE_LINK_CONTROL_LINK_STATUS_REG_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GE_PCIE_LINK_CONTROL_LINK_STATUS_REG_ADDR,m,v,HWIO_GE_PCIE_LINK_CONTROL_LINK_STATUS_REG_IN)
#define HWIO_GE_PCIE_LINK_CONTROL_LINK_STATUS_REG_PCIE_CAP_LINK_AUTO_BW_STATUS_BMSK                        0x80000000
#define HWIO_GE_PCIE_LINK_CONTROL_LINK_STATUS_REG_PCIE_CAP_LINK_AUTO_BW_STATUS_SHFT                              0x1f
#define HWIO_GE_PCIE_LINK_CONTROL_LINK_STATUS_REG_PCIE_CAP_LINK_BW_MAN_STATUS_BMSK                         0x40000000
#define HWIO_GE_PCIE_LINK_CONTROL_LINK_STATUS_REG_PCIE_CAP_LINK_BW_MAN_STATUS_SHFT                               0x1e
#define HWIO_GE_PCIE_LINK_CONTROL_LINK_STATUS_REG_PCIE_CAP_DLL_ACTIVE_BMSK                                 0x20000000
#define HWIO_GE_PCIE_LINK_CONTROL_LINK_STATUS_REG_PCIE_CAP_DLL_ACTIVE_SHFT                                       0x1d
#define HWIO_GE_PCIE_LINK_CONTROL_LINK_STATUS_REG_PCIE_CAP_SLOT_CLK_CONFIG_BMSK                            0x10000000
#define HWIO_GE_PCIE_LINK_CONTROL_LINK_STATUS_REG_PCIE_CAP_SLOT_CLK_CONFIG_SHFT                                  0x1c
#define HWIO_GE_PCIE_LINK_CONTROL_LINK_STATUS_REG_PCIE_CAP_LINK_TRAINING_BMSK                               0x8000000
#define HWIO_GE_PCIE_LINK_CONTROL_LINK_STATUS_REG_PCIE_CAP_LINK_TRAINING_SHFT                                    0x1b
#define HWIO_GE_PCIE_LINK_CONTROL_LINK_STATUS_REG_RSVDP_25_BMSK                                             0x6000000
#define HWIO_GE_PCIE_LINK_CONTROL_LINK_STATUS_REG_RSVDP_25_SHFT                                                  0x19
#define HWIO_GE_PCIE_LINK_CONTROL_LINK_STATUS_REG_PCIE_CAP_NEGO_LINK_WIDTH_BMSK                             0x1f00000
#define HWIO_GE_PCIE_LINK_CONTROL_LINK_STATUS_REG_PCIE_CAP_NEGO_LINK_WIDTH_SHFT                                  0x14
#define HWIO_GE_PCIE_LINK_CONTROL_LINK_STATUS_REG_PCIE_CAP_LINK_SPEED_BMSK                                    0xf0000
#define HWIO_GE_PCIE_LINK_CONTROL_LINK_STATUS_REG_PCIE_CAP_LINK_SPEED_SHFT                                       0x10
#define HWIO_GE_PCIE_LINK_CONTROL_LINK_STATUS_REG_RSVDP_12_BMSK                                                0xf000
#define HWIO_GE_PCIE_LINK_CONTROL_LINK_STATUS_REG_RSVDP_12_SHFT                                                   0xc
#define HWIO_GE_PCIE_LINK_CONTROL_LINK_STATUS_REG_PCIE_CAP_LINK_AUTO_BW_INT_EN_BMSK                             0x800
#define HWIO_GE_PCIE_LINK_CONTROL_LINK_STATUS_REG_PCIE_CAP_LINK_AUTO_BW_INT_EN_SHFT                               0xb
#define HWIO_GE_PCIE_LINK_CONTROL_LINK_STATUS_REG_PCIE_CAP_LINK_BW_MAN_INT_EN_BMSK                              0x400
#define HWIO_GE_PCIE_LINK_CONTROL_LINK_STATUS_REG_PCIE_CAP_LINK_BW_MAN_INT_EN_SHFT                                0xa
#define HWIO_GE_PCIE_LINK_CONTROL_LINK_STATUS_REG_PCIE_CAP_HW_AUTO_WIDTH_DISABLE_BMSK                           0x200
#define HWIO_GE_PCIE_LINK_CONTROL_LINK_STATUS_REG_PCIE_CAP_HW_AUTO_WIDTH_DISABLE_SHFT                             0x9
#define HWIO_GE_PCIE_LINK_CONTROL_LINK_STATUS_REG_PCIE_CAP_EN_CLK_POWER_MAN_BMSK                                0x100
#define HWIO_GE_PCIE_LINK_CONTROL_LINK_STATUS_REG_PCIE_CAP_EN_CLK_POWER_MAN_SHFT                                  0x8
#define HWIO_GE_PCIE_LINK_CONTROL_LINK_STATUS_REG_PCIE_CAP_EXTENDED_SYNCH_BMSK                                   0x80
#define HWIO_GE_PCIE_LINK_CONTROL_LINK_STATUS_REG_PCIE_CAP_EXTENDED_SYNCH_SHFT                                    0x7
#define HWIO_GE_PCIE_LINK_CONTROL_LINK_STATUS_REG_PCIE_CAP_COMMON_CLK_CONFIG_BMSK                                0x40
#define HWIO_GE_PCIE_LINK_CONTROL_LINK_STATUS_REG_PCIE_CAP_COMMON_CLK_CONFIG_SHFT                                 0x6
#define HWIO_GE_PCIE_LINK_CONTROL_LINK_STATUS_REG_PCIE_CAP_RETRAIN_LINK_BMSK                                     0x20
#define HWIO_GE_PCIE_LINK_CONTROL_LINK_STATUS_REG_PCIE_CAP_RETRAIN_LINK_SHFT                                      0x5
#define HWIO_GE_PCIE_LINK_CONTROL_LINK_STATUS_REG_PCIE_CAP_LINK_DISABLE_BMSK                                     0x10
#define HWIO_GE_PCIE_LINK_CONTROL_LINK_STATUS_REG_PCIE_CAP_LINK_DISABLE_SHFT                                      0x4
#define HWIO_GE_PCIE_LINK_CONTROL_LINK_STATUS_REG_PCIE_CAP_RCB_BMSK                                               0x8
#define HWIO_GE_PCIE_LINK_CONTROL_LINK_STATUS_REG_PCIE_CAP_RCB_SHFT                                               0x3
#define HWIO_GE_PCIE_LINK_CONTROL_LINK_STATUS_REG_RSVDP_2_BMSK                                                    0x4
#define HWIO_GE_PCIE_LINK_CONTROL_LINK_STATUS_REG_RSVDP_2_SHFT                                                    0x2
#define HWIO_GE_PCIE_LINK_CONTROL_LINK_STATUS_REG_PCIE_CAP_ACTIVE_STATE_LINK_PM_CONTROL_BMSK                      0x3
#define HWIO_GE_PCIE_LINK_CONTROL_LINK_STATUS_REG_PCIE_CAP_ACTIVE_STATE_LINK_PM_CONTROL_SHFT                      0x0

#define HWIO_GE_PCIE_SLOT_CAPABILITIES_REG_ADDR                                                            (GE_PCIE_PCIE20_DBI_4_REG_BASE      + 0x00000084)
#define HWIO_GE_PCIE_SLOT_CAPABILITIES_REG_PHYS                                                            (GE_PCIE_PCIE20_DBI_4_REG_BASE_PHYS + 0x00000084)
#define HWIO_GE_PCIE_SLOT_CAPABILITIES_REG_OFFS                                                            (GE_PCIE_PCIE20_DBI_4_REG_BASE_OFFS + 0x00000084)
#define HWIO_GE_PCIE_SLOT_CAPABILITIES_REG_RMSK                                                            0xffffffff
#define HWIO_GE_PCIE_SLOT_CAPABILITIES_REG_POR                                                             0x00000000
#define HWIO_GE_PCIE_SLOT_CAPABILITIES_REG_POR_RMSK                                                        0xffffffff
#define HWIO_GE_PCIE_SLOT_CAPABILITIES_REG_ATTR                                                                   0x1
#define HWIO_GE_PCIE_SLOT_CAPABILITIES_REG_IN          \
        in_dword_masked(HWIO_GE_PCIE_SLOT_CAPABILITIES_REG_ADDR, HWIO_GE_PCIE_SLOT_CAPABILITIES_REG_RMSK)
#define HWIO_GE_PCIE_SLOT_CAPABILITIES_REG_INM(m)      \
        in_dword_masked(HWIO_GE_PCIE_SLOT_CAPABILITIES_REG_ADDR, m)
#define HWIO_GE_PCIE_SLOT_CAPABILITIES_REG_PCIE_CAP_PHY_SLOT_NUM_BMSK                                      0xfff80000
#define HWIO_GE_PCIE_SLOT_CAPABILITIES_REG_PCIE_CAP_PHY_SLOT_NUM_SHFT                                            0x13
#define HWIO_GE_PCIE_SLOT_CAPABILITIES_REG_PCIE_CAP_NO_CMD_CPL_SUPPORT_BMSK                                   0x40000
#define HWIO_GE_PCIE_SLOT_CAPABILITIES_REG_PCIE_CAP_NO_CMD_CPL_SUPPORT_SHFT                                      0x12
#define HWIO_GE_PCIE_SLOT_CAPABILITIES_REG_PCIE_CAP_ELECTROMECH_INTERLOCK_BMSK                                0x20000
#define HWIO_GE_PCIE_SLOT_CAPABILITIES_REG_PCIE_CAP_ELECTROMECH_INTERLOCK_SHFT                                   0x11
#define HWIO_GE_PCIE_SLOT_CAPABILITIES_REG_PCIE_CAP_SLOT_POWER_LIMIT_SCALE_BMSK                               0x18000
#define HWIO_GE_PCIE_SLOT_CAPABILITIES_REG_PCIE_CAP_SLOT_POWER_LIMIT_SCALE_SHFT                                   0xf
#define HWIO_GE_PCIE_SLOT_CAPABILITIES_REG_PCIE_CAP_SLOT_POWER_LIMIT_VALUE_BMSK                                0x7f80
#define HWIO_GE_PCIE_SLOT_CAPABILITIES_REG_PCIE_CAP_SLOT_POWER_LIMIT_VALUE_SHFT                                   0x7
#define HWIO_GE_PCIE_SLOT_CAPABILITIES_REG_PCIE_CAP_HOT_PLUG_CAPABLE_BMSK                                        0x40
#define HWIO_GE_PCIE_SLOT_CAPABILITIES_REG_PCIE_CAP_HOT_PLUG_CAPABLE_SHFT                                         0x6
#define HWIO_GE_PCIE_SLOT_CAPABILITIES_REG_PCIE_CAP_HOT_PLUG_SURPRISE_BMSK                                       0x20
#define HWIO_GE_PCIE_SLOT_CAPABILITIES_REG_PCIE_CAP_HOT_PLUG_SURPRISE_SHFT                                        0x5
#define HWIO_GE_PCIE_SLOT_CAPABILITIES_REG_PCIE_CAP_POWER_INDICATOR_BMSK                                         0x10
#define HWIO_GE_PCIE_SLOT_CAPABILITIES_REG_PCIE_CAP_POWER_INDICATOR_SHFT                                          0x4
#define HWIO_GE_PCIE_SLOT_CAPABILITIES_REG_PCIE_CAP_ATTENTION_INDICATOR_BMSK                                      0x8
#define HWIO_GE_PCIE_SLOT_CAPABILITIES_REG_PCIE_CAP_ATTENTION_INDICATOR_SHFT                                      0x3
#define HWIO_GE_PCIE_SLOT_CAPABILITIES_REG_PCIE_CAP_MRL_SENSOR_BMSK                                               0x4
#define HWIO_GE_PCIE_SLOT_CAPABILITIES_REG_PCIE_CAP_MRL_SENSOR_SHFT                                               0x2
#define HWIO_GE_PCIE_SLOT_CAPABILITIES_REG_PCIE_CAP_POWER_CONTROLLER_BMSK                                         0x2
#define HWIO_GE_PCIE_SLOT_CAPABILITIES_REG_PCIE_CAP_POWER_CONTROLLER_SHFT                                         0x1
#define HWIO_GE_PCIE_SLOT_CAPABILITIES_REG_PCIE_CAP_ATTENTION_INDICATOR_BUTTON_BMSK                               0x1
#define HWIO_GE_PCIE_SLOT_CAPABILITIES_REG_PCIE_CAP_ATTENTION_INDICATOR_BUTTON_SHFT                               0x0

#define HWIO_GE_PCIE_SLOT_CONTROL_SLOT_STATUS_ADDR                                                         (GE_PCIE_PCIE20_DBI_4_REG_BASE      + 0x00000088)
#define HWIO_GE_PCIE_SLOT_CONTROL_SLOT_STATUS_PHYS                                                         (GE_PCIE_PCIE20_DBI_4_REG_BASE_PHYS + 0x00000088)
#define HWIO_GE_PCIE_SLOT_CONTROL_SLOT_STATUS_OFFS                                                         (GE_PCIE_PCIE20_DBI_4_REG_BASE_OFFS + 0x00000088)
#define HWIO_GE_PCIE_SLOT_CONTROL_SLOT_STATUS_RMSK                                                         0xffffffff
#define HWIO_GE_PCIE_SLOT_CONTROL_SLOT_STATUS_POR                                                          0x00000000
#define HWIO_GE_PCIE_SLOT_CONTROL_SLOT_STATUS_POR_RMSK                                                     0xffffffff
#define HWIO_GE_PCIE_SLOT_CONTROL_SLOT_STATUS_ATTR                                                                0x3
#define HWIO_GE_PCIE_SLOT_CONTROL_SLOT_STATUS_IN          \
        in_dword_masked(HWIO_GE_PCIE_SLOT_CONTROL_SLOT_STATUS_ADDR, HWIO_GE_PCIE_SLOT_CONTROL_SLOT_STATUS_RMSK)
#define HWIO_GE_PCIE_SLOT_CONTROL_SLOT_STATUS_INM(m)      \
        in_dword_masked(HWIO_GE_PCIE_SLOT_CONTROL_SLOT_STATUS_ADDR, m)
#define HWIO_GE_PCIE_SLOT_CONTROL_SLOT_STATUS_OUT(v)      \
        out_dword(HWIO_GE_PCIE_SLOT_CONTROL_SLOT_STATUS_ADDR,v)
#define HWIO_GE_PCIE_SLOT_CONTROL_SLOT_STATUS_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GE_PCIE_SLOT_CONTROL_SLOT_STATUS_ADDR,m,v,HWIO_GE_PCIE_SLOT_CONTROL_SLOT_STATUS_IN)
#define HWIO_GE_PCIE_SLOT_CONTROL_SLOT_STATUS_RSVDP_25_BMSK                                                0xfe000000
#define HWIO_GE_PCIE_SLOT_CONTROL_SLOT_STATUS_RSVDP_25_SHFT                                                      0x19
#define HWIO_GE_PCIE_SLOT_CONTROL_SLOT_STATUS_PCIE_CAP_DLL_STATE_CHANGED_BMSK                               0x1000000
#define HWIO_GE_PCIE_SLOT_CONTROL_SLOT_STATUS_PCIE_CAP_DLL_STATE_CHANGED_SHFT                                    0x18
#define HWIO_GE_PCIE_SLOT_CONTROL_SLOT_STATUS_PCIE_CAP_ELECTROMECH_INTERLOCK_STATUS_BMSK                     0x800000
#define HWIO_GE_PCIE_SLOT_CONTROL_SLOT_STATUS_PCIE_CAP_ELECTROMECH_INTERLOCK_STATUS_SHFT                         0x17
#define HWIO_GE_PCIE_SLOT_CONTROL_SLOT_STATUS_PCIE_CAP_PRESENCE_DETECT_STATE_BMSK                            0x400000
#define HWIO_GE_PCIE_SLOT_CONTROL_SLOT_STATUS_PCIE_CAP_PRESENCE_DETECT_STATE_SHFT                                0x16
#define HWIO_GE_PCIE_SLOT_CONTROL_SLOT_STATUS_PCIE_CAP_MRL_SENSOR_STATE_BMSK                                 0x200000
#define HWIO_GE_PCIE_SLOT_CONTROL_SLOT_STATUS_PCIE_CAP_MRL_SENSOR_STATE_SHFT                                     0x15
#define HWIO_GE_PCIE_SLOT_CONTROL_SLOT_STATUS_PCIE_CAP_CMD_CPLD_BMSK                                         0x100000
#define HWIO_GE_PCIE_SLOT_CONTROL_SLOT_STATUS_PCIE_CAP_CMD_CPLD_SHFT                                             0x14
#define HWIO_GE_PCIE_SLOT_CONTROL_SLOT_STATUS_PCIE_CAP_PRESENCE_DETECTED_CHANGED_BMSK                         0x80000
#define HWIO_GE_PCIE_SLOT_CONTROL_SLOT_STATUS_PCIE_CAP_PRESENCE_DETECTED_CHANGED_SHFT                            0x13
#define HWIO_GE_PCIE_SLOT_CONTROL_SLOT_STATUS_PCIE_CAP_MRL_SENSOR_CHANGED_BMSK                                0x40000
#define HWIO_GE_PCIE_SLOT_CONTROL_SLOT_STATUS_PCIE_CAP_MRL_SENSOR_CHANGED_SHFT                                   0x12
#define HWIO_GE_PCIE_SLOT_CONTROL_SLOT_STATUS_PCIE_CAP_POWER_FAULT_DETECTED_BMSK                              0x20000
#define HWIO_GE_PCIE_SLOT_CONTROL_SLOT_STATUS_PCIE_CAP_POWER_FAULT_DETECTED_SHFT                                 0x11
#define HWIO_GE_PCIE_SLOT_CONTROL_SLOT_STATUS_PCIE_CAP_ATTENTION_BUTTON_PRESSED_BMSK                          0x10000
#define HWIO_GE_PCIE_SLOT_CONTROL_SLOT_STATUS_PCIE_CAP_ATTENTION_BUTTON_PRESSED_SHFT                             0x10
#define HWIO_GE_PCIE_SLOT_CONTROL_SLOT_STATUS_RSVDP_13_BMSK                                                    0xe000
#define HWIO_GE_PCIE_SLOT_CONTROL_SLOT_STATUS_RSVDP_13_SHFT                                                       0xd
#define HWIO_GE_PCIE_SLOT_CONTROL_SLOT_STATUS_PCIE_CAP_DLL_STATE_CHANGED_EN_BMSK                               0x1000
#define HWIO_GE_PCIE_SLOT_CONTROL_SLOT_STATUS_PCIE_CAP_DLL_STATE_CHANGED_EN_SHFT                                  0xc
#define HWIO_GE_PCIE_SLOT_CONTROL_SLOT_STATUS_PCIE_CAP_ELECTROMECH_INTERLOCK_CTRL_BMSK                          0x800
#define HWIO_GE_PCIE_SLOT_CONTROL_SLOT_STATUS_PCIE_CAP_ELECTROMECH_INTERLOCK_CTRL_SHFT                            0xb
#define HWIO_GE_PCIE_SLOT_CONTROL_SLOT_STATUS_PCIE_CAP_POWER_CONTROLLER_CTRL_BMSK                               0x400
#define HWIO_GE_PCIE_SLOT_CONTROL_SLOT_STATUS_PCIE_CAP_POWER_CONTROLLER_CTRL_SHFT                                 0xa
#define HWIO_GE_PCIE_SLOT_CONTROL_SLOT_STATUS_PCIE_CAP_POWER_INDICATOR_CTRL_BMSK                                0x300
#define HWIO_GE_PCIE_SLOT_CONTROL_SLOT_STATUS_PCIE_CAP_POWER_INDICATOR_CTRL_SHFT                                  0x8
#define HWIO_GE_PCIE_SLOT_CONTROL_SLOT_STATUS_PCIE_CAP_ATTENTION_INDICATOR_CTRL_BMSK                             0xc0
#define HWIO_GE_PCIE_SLOT_CONTROL_SLOT_STATUS_PCIE_CAP_ATTENTION_INDICATOR_CTRL_SHFT                              0x6
#define HWIO_GE_PCIE_SLOT_CONTROL_SLOT_STATUS_PCIE_CAP_HOT_PLUG_INT_EN_BMSK                                      0x20
#define HWIO_GE_PCIE_SLOT_CONTROL_SLOT_STATUS_PCIE_CAP_HOT_PLUG_INT_EN_SHFT                                       0x5
#define HWIO_GE_PCIE_SLOT_CONTROL_SLOT_STATUS_PCIE_CAP_CMD_CPL_INT_EN_BMSK                                       0x10
#define HWIO_GE_PCIE_SLOT_CONTROL_SLOT_STATUS_PCIE_CAP_CMD_CPL_INT_EN_SHFT                                        0x4
#define HWIO_GE_PCIE_SLOT_CONTROL_SLOT_STATUS_PCIE_CAP_PRESENCE_DETECT_CHANGE_EN_BMSK                             0x8
#define HWIO_GE_PCIE_SLOT_CONTROL_SLOT_STATUS_PCIE_CAP_PRESENCE_DETECT_CHANGE_EN_SHFT                             0x3
#define HWIO_GE_PCIE_SLOT_CONTROL_SLOT_STATUS_PCIE_CAP_MRL_SENSOR_CHANGED_EN_BMSK                                 0x4
#define HWIO_GE_PCIE_SLOT_CONTROL_SLOT_STATUS_PCIE_CAP_MRL_SENSOR_CHANGED_EN_SHFT                                 0x2
#define HWIO_GE_PCIE_SLOT_CONTROL_SLOT_STATUS_PCIE_CAP_POWER_FAULT_DETECTED_EN_BMSK                               0x2
#define HWIO_GE_PCIE_SLOT_CONTROL_SLOT_STATUS_PCIE_CAP_POWER_FAULT_DETECTED_EN_SHFT                               0x1
#define HWIO_GE_PCIE_SLOT_CONTROL_SLOT_STATUS_PCIE_CAP_ATTENTION_BUTTON_PRESSED_EN_BMSK                           0x1
#define HWIO_GE_PCIE_SLOT_CONTROL_SLOT_STATUS_PCIE_CAP_ATTENTION_BUTTON_PRESSED_EN_SHFT                           0x0

#define HWIO_GE_PCIE_ROOT_CONTROL_ROOT_CAPABILITIES_REG_ADDR                                               (GE_PCIE_PCIE20_DBI_4_REG_BASE      + 0x0000008c)
#define HWIO_GE_PCIE_ROOT_CONTROL_ROOT_CAPABILITIES_REG_PHYS                                               (GE_PCIE_PCIE20_DBI_4_REG_BASE_PHYS + 0x0000008c)
#define HWIO_GE_PCIE_ROOT_CONTROL_ROOT_CAPABILITIES_REG_OFFS                                               (GE_PCIE_PCIE20_DBI_4_REG_BASE_OFFS + 0x0000008c)
#define HWIO_GE_PCIE_ROOT_CONTROL_ROOT_CAPABILITIES_REG_RMSK                                               0xffffffff
#define HWIO_GE_PCIE_ROOT_CONTROL_ROOT_CAPABILITIES_REG_POR                                                0x00000000
#define HWIO_GE_PCIE_ROOT_CONTROL_ROOT_CAPABILITIES_REG_POR_RMSK                                           0xffffffff
#define HWIO_GE_PCIE_ROOT_CONTROL_ROOT_CAPABILITIES_REG_ATTR                                                      0x3
#define HWIO_GE_PCIE_ROOT_CONTROL_ROOT_CAPABILITIES_REG_IN          \
        in_dword_masked(HWIO_GE_PCIE_ROOT_CONTROL_ROOT_CAPABILITIES_REG_ADDR, HWIO_GE_PCIE_ROOT_CONTROL_ROOT_CAPABILITIES_REG_RMSK)
#define HWIO_GE_PCIE_ROOT_CONTROL_ROOT_CAPABILITIES_REG_INM(m)      \
        in_dword_masked(HWIO_GE_PCIE_ROOT_CONTROL_ROOT_CAPABILITIES_REG_ADDR, m)
#define HWIO_GE_PCIE_ROOT_CONTROL_ROOT_CAPABILITIES_REG_OUT(v)      \
        out_dword(HWIO_GE_PCIE_ROOT_CONTROL_ROOT_CAPABILITIES_REG_ADDR,v)
#define HWIO_GE_PCIE_ROOT_CONTROL_ROOT_CAPABILITIES_REG_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GE_PCIE_ROOT_CONTROL_ROOT_CAPABILITIES_REG_ADDR,m,v,HWIO_GE_PCIE_ROOT_CONTROL_ROOT_CAPABILITIES_REG_IN)
#define HWIO_GE_PCIE_ROOT_CONTROL_ROOT_CAPABILITIES_REG_RSVDP_17_BMSK                                      0xfffe0000
#define HWIO_GE_PCIE_ROOT_CONTROL_ROOT_CAPABILITIES_REG_RSVDP_17_SHFT                                            0x11
#define HWIO_GE_PCIE_ROOT_CONTROL_ROOT_CAPABILITIES_REG_PCIE_CAP_CRS_SW_VISIBILITY_BMSK                       0x10000
#define HWIO_GE_PCIE_ROOT_CONTROL_ROOT_CAPABILITIES_REG_PCIE_CAP_CRS_SW_VISIBILITY_SHFT                          0x10
#define HWIO_GE_PCIE_ROOT_CONTROL_ROOT_CAPABILITIES_REG_RSVDP_5_BMSK                                           0xffe0
#define HWIO_GE_PCIE_ROOT_CONTROL_ROOT_CAPABILITIES_REG_RSVDP_5_SHFT                                              0x5
#define HWIO_GE_PCIE_ROOT_CONTROL_ROOT_CAPABILITIES_REG_PCIE_CAP_CRS_SW_VISIBILITY_EN_BMSK                       0x10
#define HWIO_GE_PCIE_ROOT_CONTROL_ROOT_CAPABILITIES_REG_PCIE_CAP_CRS_SW_VISIBILITY_EN_SHFT                        0x4
#define HWIO_GE_PCIE_ROOT_CONTROL_ROOT_CAPABILITIES_REG_PCIE_CAP_PME_INT_EN_BMSK                                  0x8
#define HWIO_GE_PCIE_ROOT_CONTROL_ROOT_CAPABILITIES_REG_PCIE_CAP_PME_INT_EN_SHFT                                  0x3
#define HWIO_GE_PCIE_ROOT_CONTROL_ROOT_CAPABILITIES_REG_PCIE_CAP_SYS_ERR_ON_FATAL_ERR_EN_BMSK                     0x4
#define HWIO_GE_PCIE_ROOT_CONTROL_ROOT_CAPABILITIES_REG_PCIE_CAP_SYS_ERR_ON_FATAL_ERR_EN_SHFT                     0x2
#define HWIO_GE_PCIE_ROOT_CONTROL_ROOT_CAPABILITIES_REG_PCIE_CAP_SYS_ERR_ON_NON_FATAL_ERR_EN_BMSK                 0x2
#define HWIO_GE_PCIE_ROOT_CONTROL_ROOT_CAPABILITIES_REG_PCIE_CAP_SYS_ERR_ON_NON_FATAL_ERR_EN_SHFT                 0x1
#define HWIO_GE_PCIE_ROOT_CONTROL_ROOT_CAPABILITIES_REG_PCIE_CAP_SYS_ERR_ON_CORR_ERR_EN_BMSK                      0x1
#define HWIO_GE_PCIE_ROOT_CONTROL_ROOT_CAPABILITIES_REG_PCIE_CAP_SYS_ERR_ON_CORR_ERR_EN_SHFT                      0x0

#define HWIO_GE_PCIE_ROOT_STATUS_REG_ADDR                                                                  (GE_PCIE_PCIE20_DBI_4_REG_BASE      + 0x00000090)
#define HWIO_GE_PCIE_ROOT_STATUS_REG_PHYS                                                                  (GE_PCIE_PCIE20_DBI_4_REG_BASE_PHYS + 0x00000090)
#define HWIO_GE_PCIE_ROOT_STATUS_REG_OFFS                                                                  (GE_PCIE_PCIE20_DBI_4_REG_BASE_OFFS + 0x00000090)
#define HWIO_GE_PCIE_ROOT_STATUS_REG_RMSK                                                                  0xffffffff
#define HWIO_GE_PCIE_ROOT_STATUS_REG_POR                                                                   0x00000000
#define HWIO_GE_PCIE_ROOT_STATUS_REG_POR_RMSK                                                              0xffffffff
#define HWIO_GE_PCIE_ROOT_STATUS_REG_ATTR                                                                         0x3
#define HWIO_GE_PCIE_ROOT_STATUS_REG_IN          \
        in_dword_masked(HWIO_GE_PCIE_ROOT_STATUS_REG_ADDR, HWIO_GE_PCIE_ROOT_STATUS_REG_RMSK)
#define HWIO_GE_PCIE_ROOT_STATUS_REG_INM(m)      \
        in_dword_masked(HWIO_GE_PCIE_ROOT_STATUS_REG_ADDR, m)
#define HWIO_GE_PCIE_ROOT_STATUS_REG_OUT(v)      \
        out_dword(HWIO_GE_PCIE_ROOT_STATUS_REG_ADDR,v)
#define HWIO_GE_PCIE_ROOT_STATUS_REG_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GE_PCIE_ROOT_STATUS_REG_ADDR,m,v,HWIO_GE_PCIE_ROOT_STATUS_REG_IN)
#define HWIO_GE_PCIE_ROOT_STATUS_REG_RSVDP_18_BMSK                                                         0xfffc0000
#define HWIO_GE_PCIE_ROOT_STATUS_REG_RSVDP_18_SHFT                                                               0x12
#define HWIO_GE_PCIE_ROOT_STATUS_REG_PCIE_CAP_PME_PENDING_BMSK                                                0x20000
#define HWIO_GE_PCIE_ROOT_STATUS_REG_PCIE_CAP_PME_PENDING_SHFT                                                   0x11
#define HWIO_GE_PCIE_ROOT_STATUS_REG_PCIE_CAP_PME_STATUS_BMSK                                                 0x10000
#define HWIO_GE_PCIE_ROOT_STATUS_REG_PCIE_CAP_PME_STATUS_SHFT                                                    0x10
#define HWIO_GE_PCIE_ROOT_STATUS_REG_PCIE_CAP_PME_REQ_ID_BMSK                                                  0xffff
#define HWIO_GE_PCIE_ROOT_STATUS_REG_PCIE_CAP_PME_REQ_ID_SHFT                                                     0x0

#define HWIO_GE_PCIE_DEVICE_CAPABILITIES2_REG_ADDR                                                         (GE_PCIE_PCIE20_DBI_4_REG_BASE      + 0x00000094)
#define HWIO_GE_PCIE_DEVICE_CAPABILITIES2_REG_PHYS                                                         (GE_PCIE_PCIE20_DBI_4_REG_BASE_PHYS + 0x00000094)
#define HWIO_GE_PCIE_DEVICE_CAPABILITIES2_REG_OFFS                                                         (GE_PCIE_PCIE20_DBI_4_REG_BASE_OFFS + 0x00000094)
#define HWIO_GE_PCIE_DEVICE_CAPABILITIES2_REG_RMSK                                                         0xff0fffff
#define HWIO_GE_PCIE_DEVICE_CAPABILITIES2_REG_POR                                                          0x00000c1f
#define HWIO_GE_PCIE_DEVICE_CAPABILITIES2_REG_POR_RMSK                                                     0xffffffff
#define HWIO_GE_PCIE_DEVICE_CAPABILITIES2_REG_ATTR                                                                0x1
#define HWIO_GE_PCIE_DEVICE_CAPABILITIES2_REG_IN          \
        in_dword_masked(HWIO_GE_PCIE_DEVICE_CAPABILITIES2_REG_ADDR, HWIO_GE_PCIE_DEVICE_CAPABILITIES2_REG_RMSK)
#define HWIO_GE_PCIE_DEVICE_CAPABILITIES2_REG_INM(m)      \
        in_dword_masked(HWIO_GE_PCIE_DEVICE_CAPABILITIES2_REG_ADDR, m)
#define HWIO_GE_PCIE_DEVICE_CAPABILITIES2_REG_RSVDP_24_BMSK                                                0xff000000
#define HWIO_GE_PCIE_DEVICE_CAPABILITIES2_REG_RSVDP_24_SHFT                                                      0x18
#define HWIO_GE_PCIE_DEVICE_CAPABILITIES2_REG_PCIE_CAP_OBFF_SUPPORT_BMSK                                      0xc0000
#define HWIO_GE_PCIE_DEVICE_CAPABILITIES2_REG_PCIE_CAP_OBFF_SUPPORT_SHFT                                         0x12
#define HWIO_GE_PCIE_DEVICE_CAPABILITIES2_REG_RSVDP_14_BMSK                                                   0x3c000
#define HWIO_GE_PCIE_DEVICE_CAPABILITIES2_REG_RSVDP_14_SHFT                                                       0xe
#define HWIO_GE_PCIE_DEVICE_CAPABILITIES2_REG_PCIE_CAP_TPH_CMPLT_SUPPORT_1_BMSK                                0x2000
#define HWIO_GE_PCIE_DEVICE_CAPABILITIES2_REG_PCIE_CAP_TPH_CMPLT_SUPPORT_1_SHFT                                   0xd
#define HWIO_GE_PCIE_DEVICE_CAPABILITIES2_REG_PCIE_CAP_TPH_CMPLT_SUPPORT_0_BMSK                                0x1000
#define HWIO_GE_PCIE_DEVICE_CAPABILITIES2_REG_PCIE_CAP_TPH_CMPLT_SUPPORT_0_SHFT                                   0xc
#define HWIO_GE_PCIE_DEVICE_CAPABILITIES2_REG_PCIE_CAP_LTR_SUPP_BMSK                                            0x800
#define HWIO_GE_PCIE_DEVICE_CAPABILITIES2_REG_PCIE_CAP_LTR_SUPP_SHFT                                              0xb
#define HWIO_GE_PCIE_DEVICE_CAPABILITIES2_REG_PCIE_CAP_NO_RO_EN_PR2PR_PAR_BMSK                                  0x400
#define HWIO_GE_PCIE_DEVICE_CAPABILITIES2_REG_PCIE_CAP_NO_RO_EN_PR2PR_PAR_SHFT                                    0xa
#define HWIO_GE_PCIE_DEVICE_CAPABILITIES2_REG_PCIE_CAP_128_CAS_CPL_SUPP_BMSK                                    0x200
#define HWIO_GE_PCIE_DEVICE_CAPABILITIES2_REG_PCIE_CAP_128_CAS_CPL_SUPP_SHFT                                      0x9
#define HWIO_GE_PCIE_DEVICE_CAPABILITIES2_REG_PCIE_CAP_64_ATOMIC_CPL_SUPP_BMSK                                  0x100
#define HWIO_GE_PCIE_DEVICE_CAPABILITIES2_REG_PCIE_CAP_64_ATOMIC_CPL_SUPP_SHFT                                    0x8
#define HWIO_GE_PCIE_DEVICE_CAPABILITIES2_REG_PCIE_CAP_32_ATOMIC_CPL_SUPP_BMSK                                   0x80
#define HWIO_GE_PCIE_DEVICE_CAPABILITIES2_REG_PCIE_CAP_32_ATOMIC_CPL_SUPP_SHFT                                    0x7
#define HWIO_GE_PCIE_DEVICE_CAPABILITIES2_REG_PCIE_CAP_ATOMIC_ROUTING_SUPP_BMSK                                  0x40
#define HWIO_GE_PCIE_DEVICE_CAPABILITIES2_REG_PCIE_CAP_ATOMIC_ROUTING_SUPP_SHFT                                   0x6
#define HWIO_GE_PCIE_DEVICE_CAPABILITIES2_REG_PCIE_CAP_ARI_FORWARD_SUPPORT_BMSK                                  0x20
#define HWIO_GE_PCIE_DEVICE_CAPABILITIES2_REG_PCIE_CAP_ARI_FORWARD_SUPPORT_SHFT                                   0x5
#define HWIO_GE_PCIE_DEVICE_CAPABILITIES2_REG_PCIE_CAP_CPL_TIMEOUT_DISABLE_SUPPORT_BMSK                          0x10
#define HWIO_GE_PCIE_DEVICE_CAPABILITIES2_REG_PCIE_CAP_CPL_TIMEOUT_DISABLE_SUPPORT_SHFT                           0x4
#define HWIO_GE_PCIE_DEVICE_CAPABILITIES2_REG_PCIE_CAP_CPL_TIMEOUT_RANGE_BMSK                                     0xf
#define HWIO_GE_PCIE_DEVICE_CAPABILITIES2_REG_PCIE_CAP_CPL_TIMEOUT_RANGE_SHFT                                     0x0

#define HWIO_GE_PCIE_DEVICE_CONTROL2_DEVICE_STATUS2_REG_ADDR                                               (GE_PCIE_PCIE20_DBI_4_REG_BASE      + 0x00000098)
#define HWIO_GE_PCIE_DEVICE_CONTROL2_DEVICE_STATUS2_REG_PHYS                                               (GE_PCIE_PCIE20_DBI_4_REG_BASE_PHYS + 0x00000098)
#define HWIO_GE_PCIE_DEVICE_CONTROL2_DEVICE_STATUS2_REG_OFFS                                               (GE_PCIE_PCIE20_DBI_4_REG_BASE_OFFS + 0x00000098)
#define HWIO_GE_PCIE_DEVICE_CONTROL2_DEVICE_STATUS2_REG_RMSK                                                    0x43f
#define HWIO_GE_PCIE_DEVICE_CONTROL2_DEVICE_STATUS2_REG_POR                                                0x00000000
#define HWIO_GE_PCIE_DEVICE_CONTROL2_DEVICE_STATUS2_REG_POR_RMSK                                           0xffffffff
#define HWIO_GE_PCIE_DEVICE_CONTROL2_DEVICE_STATUS2_REG_ATTR                                                      0x3
#define HWIO_GE_PCIE_DEVICE_CONTROL2_DEVICE_STATUS2_REG_IN          \
        in_dword_masked(HWIO_GE_PCIE_DEVICE_CONTROL2_DEVICE_STATUS2_REG_ADDR, HWIO_GE_PCIE_DEVICE_CONTROL2_DEVICE_STATUS2_REG_RMSK)
#define HWIO_GE_PCIE_DEVICE_CONTROL2_DEVICE_STATUS2_REG_INM(m)      \
        in_dword_masked(HWIO_GE_PCIE_DEVICE_CONTROL2_DEVICE_STATUS2_REG_ADDR, m)
#define HWIO_GE_PCIE_DEVICE_CONTROL2_DEVICE_STATUS2_REG_OUT(v)      \
        out_dword(HWIO_GE_PCIE_DEVICE_CONTROL2_DEVICE_STATUS2_REG_ADDR,v)
#define HWIO_GE_PCIE_DEVICE_CONTROL2_DEVICE_STATUS2_REG_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GE_PCIE_DEVICE_CONTROL2_DEVICE_STATUS2_REG_ADDR,m,v,HWIO_GE_PCIE_DEVICE_CONTROL2_DEVICE_STATUS2_REG_IN)
#define HWIO_GE_PCIE_DEVICE_CONTROL2_DEVICE_STATUS2_REG_PCIE_CAP_LTR_EN_BMSK                                    0x400
#define HWIO_GE_PCIE_DEVICE_CONTROL2_DEVICE_STATUS2_REG_PCIE_CAP_LTR_EN_SHFT                                      0xa
#define HWIO_GE_PCIE_DEVICE_CONTROL2_DEVICE_STATUS2_REG_PCIE_CAP_ARI_FORWARD_SUPPORT_CS_BMSK                     0x20
#define HWIO_GE_PCIE_DEVICE_CONTROL2_DEVICE_STATUS2_REG_PCIE_CAP_ARI_FORWARD_SUPPORT_CS_SHFT                      0x5
#define HWIO_GE_PCIE_DEVICE_CONTROL2_DEVICE_STATUS2_REG_PCIE_CAP_CPL_TIMEOUT_DISABLE_BMSK                        0x10
#define HWIO_GE_PCIE_DEVICE_CONTROL2_DEVICE_STATUS2_REG_PCIE_CAP_CPL_TIMEOUT_DISABLE_SHFT                         0x4
#define HWIO_GE_PCIE_DEVICE_CONTROL2_DEVICE_STATUS2_REG_PCIE_CAP_CPL_TIMEOUT_VALUE_BMSK                           0xf
#define HWIO_GE_PCIE_DEVICE_CONTROL2_DEVICE_STATUS2_REG_PCIE_CAP_CPL_TIMEOUT_VALUE_SHFT                           0x0

#define HWIO_GE_PCIE_LINK_CAPABILITIES2_REG_ADDR                                                           (GE_PCIE_PCIE20_DBI_4_REG_BASE      + 0x0000009c)
#define HWIO_GE_PCIE_LINK_CAPABILITIES2_REG_PHYS                                                           (GE_PCIE_PCIE20_DBI_4_REG_BASE_PHYS + 0x0000009c)
#define HWIO_GE_PCIE_LINK_CAPABILITIES2_REG_OFFS                                                           (GE_PCIE_PCIE20_DBI_4_REG_BASE_OFFS + 0x0000009c)
#define HWIO_GE_PCIE_LINK_CAPABILITIES2_REG_RMSK                                                           0xffffffff
#define HWIO_GE_PCIE_LINK_CAPABILITIES2_REG_POR                                                            0x00000000
#define HWIO_GE_PCIE_LINK_CAPABILITIES2_REG_POR_RMSK                                                       0xffffffff
#define HWIO_GE_PCIE_LINK_CAPABILITIES2_REG_ATTR                                                                  0x1
#define HWIO_GE_PCIE_LINK_CAPABILITIES2_REG_IN          \
        in_dword_masked(HWIO_GE_PCIE_LINK_CAPABILITIES2_REG_ADDR, HWIO_GE_PCIE_LINK_CAPABILITIES2_REG_RMSK)
#define HWIO_GE_PCIE_LINK_CAPABILITIES2_REG_INM(m)      \
        in_dword_masked(HWIO_GE_PCIE_LINK_CAPABILITIES2_REG_ADDR, m)
#define HWIO_GE_PCIE_LINK_CAPABILITIES2_REG_RSVDP_9_BMSK                                                   0xfffffe00
#define HWIO_GE_PCIE_LINK_CAPABILITIES2_REG_RSVDP_9_SHFT                                                          0x9
#define HWIO_GE_PCIE_LINK_CAPABILITIES2_REG_PCIE_CAP_CROSS_LINK_SUPPORT_BMSK                                    0x100
#define HWIO_GE_PCIE_LINK_CAPABILITIES2_REG_PCIE_CAP_CROSS_LINK_SUPPORT_SHFT                                      0x8
#define HWIO_GE_PCIE_LINK_CAPABILITIES2_REG_PCIE_CAP_SUPPORT_LINK_SPEED_VECTOR_BMSK                              0xfe
#define HWIO_GE_PCIE_LINK_CAPABILITIES2_REG_PCIE_CAP_SUPPORT_LINK_SPEED_VECTOR_SHFT                               0x1
#define HWIO_GE_PCIE_LINK_CAPABILITIES2_REG_RSVDP_0_BMSK                                                          0x1
#define HWIO_GE_PCIE_LINK_CAPABILITIES2_REG_RSVDP_0_SHFT                                                          0x0

#define HWIO_GE_PCIE_LINK_CONTROL2_LINK_STATUS2_REG_ADDR                                                   (GE_PCIE_PCIE20_DBI_4_REG_BASE      + 0x000000a0)
#define HWIO_GE_PCIE_LINK_CONTROL2_LINK_STATUS2_REG_PHYS                                                   (GE_PCIE_PCIE20_DBI_4_REG_BASE_PHYS + 0x000000a0)
#define HWIO_GE_PCIE_LINK_CONTROL2_LINK_STATUS2_REG_OFFS                                                   (GE_PCIE_PCIE20_DBI_4_REG_BASE_OFFS + 0x000000a0)
#define HWIO_GE_PCIE_LINK_CONTROL2_LINK_STATUS2_REG_RMSK                                                   0xf801ffff
#define HWIO_GE_PCIE_LINK_CONTROL2_LINK_STATUS2_REG_POR                                                    0x00000000
#define HWIO_GE_PCIE_LINK_CONTROL2_LINK_STATUS2_REG_POR_RMSK                                               0xffffffff
#define HWIO_GE_PCIE_LINK_CONTROL2_LINK_STATUS2_REG_ATTR                                                          0x3
#define HWIO_GE_PCIE_LINK_CONTROL2_LINK_STATUS2_REG_IN          \
        in_dword_masked(HWIO_GE_PCIE_LINK_CONTROL2_LINK_STATUS2_REG_ADDR, HWIO_GE_PCIE_LINK_CONTROL2_LINK_STATUS2_REG_RMSK)
#define HWIO_GE_PCIE_LINK_CONTROL2_LINK_STATUS2_REG_INM(m)      \
        in_dword_masked(HWIO_GE_PCIE_LINK_CONTROL2_LINK_STATUS2_REG_ADDR, m)
#define HWIO_GE_PCIE_LINK_CONTROL2_LINK_STATUS2_REG_OUT(v)      \
        out_dword(HWIO_GE_PCIE_LINK_CONTROL2_LINK_STATUS2_REG_ADDR,v)
#define HWIO_GE_PCIE_LINK_CONTROL2_LINK_STATUS2_REG_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GE_PCIE_LINK_CONTROL2_LINK_STATUS2_REG_ADDR,m,v,HWIO_GE_PCIE_LINK_CONTROL2_LINK_STATUS2_REG_IN)
#define HWIO_GE_PCIE_LINK_CONTROL2_LINK_STATUS2_REG_RSVDP_27_BMSK                                          0xf8000000
#define HWIO_GE_PCIE_LINK_CONTROL2_LINK_STATUS2_REG_RSVDP_27_SHFT                                                0x1b
#define HWIO_GE_PCIE_LINK_CONTROL2_LINK_STATUS2_REG_PCIE_CAP_CURR_DEEMPHASIS_BMSK                             0x10000
#define HWIO_GE_PCIE_LINK_CONTROL2_LINK_STATUS2_REG_PCIE_CAP_CURR_DEEMPHASIS_SHFT                                0x10
#define HWIO_GE_PCIE_LINK_CONTROL2_LINK_STATUS2_REG_PCIE_CAP_COMPLIANCE_PRESET_BMSK                            0xf000
#define HWIO_GE_PCIE_LINK_CONTROL2_LINK_STATUS2_REG_PCIE_CAP_COMPLIANCE_PRESET_SHFT                               0xc
#define HWIO_GE_PCIE_LINK_CONTROL2_LINK_STATUS2_REG_PCIE_CAP_COMPLIANCE_SOS_BMSK                                0x800
#define HWIO_GE_PCIE_LINK_CONTROL2_LINK_STATUS2_REG_PCIE_CAP_COMPLIANCE_SOS_SHFT                                  0xb
#define HWIO_GE_PCIE_LINK_CONTROL2_LINK_STATUS2_REG_PCIE_CAP_ENTER_MODIFIED_COMPLIANCE_BMSK                     0x400
#define HWIO_GE_PCIE_LINK_CONTROL2_LINK_STATUS2_REG_PCIE_CAP_ENTER_MODIFIED_COMPLIANCE_SHFT                       0xa
#define HWIO_GE_PCIE_LINK_CONTROL2_LINK_STATUS2_REG_PCIE_CAP_TX_MARGIN_BMSK                                     0x380
#define HWIO_GE_PCIE_LINK_CONTROL2_LINK_STATUS2_REG_PCIE_CAP_TX_MARGIN_SHFT                                       0x7
#define HWIO_GE_PCIE_LINK_CONTROL2_LINK_STATUS2_REG_PCIE_CAP_SEL_DEEMPHASIS_BMSK                                 0x40
#define HWIO_GE_PCIE_LINK_CONTROL2_LINK_STATUS2_REG_PCIE_CAP_SEL_DEEMPHASIS_SHFT                                  0x6
#define HWIO_GE_PCIE_LINK_CONTROL2_LINK_STATUS2_REG_PCIE_CAP_HW_AUTO_SPEED_DISABLE_BMSK                          0x20
#define HWIO_GE_PCIE_LINK_CONTROL2_LINK_STATUS2_REG_PCIE_CAP_HW_AUTO_SPEED_DISABLE_SHFT                           0x5
#define HWIO_GE_PCIE_LINK_CONTROL2_LINK_STATUS2_REG_PCIE_CAP_ENTER_COMPLIANCE_BMSK                               0x10
#define HWIO_GE_PCIE_LINK_CONTROL2_LINK_STATUS2_REG_PCIE_CAP_ENTER_COMPLIANCE_SHFT                                0x4
#define HWIO_GE_PCIE_LINK_CONTROL2_LINK_STATUS2_REG_PCIE_CAP_TARGET_LINK_SPEED_BMSK                               0xf
#define HWIO_GE_PCIE_LINK_CONTROL2_LINK_STATUS2_REG_PCIE_CAP_TARGET_LINK_SPEED_SHFT                               0x0

#define HWIO_GE_PCIE_AER_EXT_CAP_HDR_OFF_ADDR                                                              (GE_PCIE_PCIE20_DBI_4_REG_BASE      + 0x00000100)
#define HWIO_GE_PCIE_AER_EXT_CAP_HDR_OFF_PHYS                                                              (GE_PCIE_PCIE20_DBI_4_REG_BASE_PHYS + 0x00000100)
#define HWIO_GE_PCIE_AER_EXT_CAP_HDR_OFF_OFFS                                                              (GE_PCIE_PCIE20_DBI_4_REG_BASE_OFFS + 0x00000100)
#define HWIO_GE_PCIE_AER_EXT_CAP_HDR_OFF_RMSK                                                              0xffffffff
#define HWIO_GE_PCIE_AER_EXT_CAP_HDR_OFF_POR                                                               0x00000000
#define HWIO_GE_PCIE_AER_EXT_CAP_HDR_OFF_POR_RMSK                                                          0xffffffff
#define HWIO_GE_PCIE_AER_EXT_CAP_HDR_OFF_ATTR                                                                     0x1
#define HWIO_GE_PCIE_AER_EXT_CAP_HDR_OFF_IN          \
        in_dword_masked(HWIO_GE_PCIE_AER_EXT_CAP_HDR_OFF_ADDR, HWIO_GE_PCIE_AER_EXT_CAP_HDR_OFF_RMSK)
#define HWIO_GE_PCIE_AER_EXT_CAP_HDR_OFF_INM(m)      \
        in_dword_masked(HWIO_GE_PCIE_AER_EXT_CAP_HDR_OFF_ADDR, m)
#define HWIO_GE_PCIE_AER_EXT_CAP_HDR_OFF_NEXT_OFFSET_BMSK                                                  0xfff00000
#define HWIO_GE_PCIE_AER_EXT_CAP_HDR_OFF_NEXT_OFFSET_SHFT                                                        0x14
#define HWIO_GE_PCIE_AER_EXT_CAP_HDR_OFF_CAP_VERSION_BMSK                                                     0xf0000
#define HWIO_GE_PCIE_AER_EXT_CAP_HDR_OFF_CAP_VERSION_SHFT                                                        0x10
#define HWIO_GE_PCIE_AER_EXT_CAP_HDR_OFF_CAP_ID_BMSK                                                           0xffff
#define HWIO_GE_PCIE_AER_EXT_CAP_HDR_OFF_CAP_ID_SHFT                                                              0x0

#define HWIO_GE_PCIE_UNCORR_ERR_STATUS_OFF_ADDR                                                            (GE_PCIE_PCIE20_DBI_4_REG_BASE      + 0x00000104)
#define HWIO_GE_PCIE_UNCORR_ERR_STATUS_OFF_PHYS                                                            (GE_PCIE_PCIE20_DBI_4_REG_BASE_PHYS + 0x00000104)
#define HWIO_GE_PCIE_UNCORR_ERR_STATUS_OFF_OFFS                                                            (GE_PCIE_PCIE20_DBI_4_REG_BASE_OFFS + 0x00000104)
#define HWIO_GE_PCIE_UNCORR_ERR_STATUS_OFF_RMSK                                                            0xffffffff
#define HWIO_GE_PCIE_UNCORR_ERR_STATUS_OFF_POR                                                             0x00000000
#define HWIO_GE_PCIE_UNCORR_ERR_STATUS_OFF_POR_RMSK                                                        0xffffffff
#define HWIO_GE_PCIE_UNCORR_ERR_STATUS_OFF_ATTR                                                                   0x3
#define HWIO_GE_PCIE_UNCORR_ERR_STATUS_OFF_IN          \
        in_dword_masked(HWIO_GE_PCIE_UNCORR_ERR_STATUS_OFF_ADDR, HWIO_GE_PCIE_UNCORR_ERR_STATUS_OFF_RMSK)
#define HWIO_GE_PCIE_UNCORR_ERR_STATUS_OFF_INM(m)      \
        in_dword_masked(HWIO_GE_PCIE_UNCORR_ERR_STATUS_OFF_ADDR, m)
#define HWIO_GE_PCIE_UNCORR_ERR_STATUS_OFF_OUT(v)      \
        out_dword(HWIO_GE_PCIE_UNCORR_ERR_STATUS_OFF_ADDR,v)
#define HWIO_GE_PCIE_UNCORR_ERR_STATUS_OFF_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GE_PCIE_UNCORR_ERR_STATUS_OFF_ADDR,m,v,HWIO_GE_PCIE_UNCORR_ERR_STATUS_OFF_IN)
#define HWIO_GE_PCIE_UNCORR_ERR_STATUS_OFF_RSVDP_26_BMSK                                                   0xfc000000
#define HWIO_GE_PCIE_UNCORR_ERR_STATUS_OFF_RSVDP_26_SHFT                                                         0x1a
#define HWIO_GE_PCIE_UNCORR_ERR_STATUS_OFF_TLP_PRFX_BLOCKED_ERR_STATUS_BMSK                                 0x2000000
#define HWIO_GE_PCIE_UNCORR_ERR_STATUS_OFF_TLP_PRFX_BLOCKED_ERR_STATUS_SHFT                                      0x19
#define HWIO_GE_PCIE_UNCORR_ERR_STATUS_OFF_RSVDP_23_BMSK                                                    0x1800000
#define HWIO_GE_PCIE_UNCORR_ERR_STATUS_OFF_RSVDP_23_SHFT                                                         0x17
#define HWIO_GE_PCIE_UNCORR_ERR_STATUS_OFF_INTERNAL_ERR_STATUS_BMSK                                          0x400000
#define HWIO_GE_PCIE_UNCORR_ERR_STATUS_OFF_INTERNAL_ERR_STATUS_SHFT                                              0x16
#define HWIO_GE_PCIE_UNCORR_ERR_STATUS_OFF_RSVDP_21_BMSK                                                     0x200000
#define HWIO_GE_PCIE_UNCORR_ERR_STATUS_OFF_RSVDP_21_SHFT                                                         0x15
#define HWIO_GE_PCIE_UNCORR_ERR_STATUS_OFF_UNSUPPORTED_REQ_ERR_STATUS_BMSK                                   0x100000
#define HWIO_GE_PCIE_UNCORR_ERR_STATUS_OFF_UNSUPPORTED_REQ_ERR_STATUS_SHFT                                       0x14
#define HWIO_GE_PCIE_UNCORR_ERR_STATUS_OFF_ECRC_ERR_STATUS_BMSK                                               0x80000
#define HWIO_GE_PCIE_UNCORR_ERR_STATUS_OFF_ECRC_ERR_STATUS_SHFT                                                  0x13
#define HWIO_GE_PCIE_UNCORR_ERR_STATUS_OFF_MALF_TLP_ERR_STATUS_BMSK                                           0x40000
#define HWIO_GE_PCIE_UNCORR_ERR_STATUS_OFF_MALF_TLP_ERR_STATUS_SHFT                                              0x12
#define HWIO_GE_PCIE_UNCORR_ERR_STATUS_OFF_REC_OVERFLOW_ERR_STATUS_BMSK                                       0x20000
#define HWIO_GE_PCIE_UNCORR_ERR_STATUS_OFF_REC_OVERFLOW_ERR_STATUS_SHFT                                          0x11
#define HWIO_GE_PCIE_UNCORR_ERR_STATUS_OFF_UNEXP_CMPLT_ERR_STATUS_BMSK                                        0x10000
#define HWIO_GE_PCIE_UNCORR_ERR_STATUS_OFF_UNEXP_CMPLT_ERR_STATUS_SHFT                                           0x10
#define HWIO_GE_PCIE_UNCORR_ERR_STATUS_OFF_CMPLT_ABORT_ERR_STATUS_BMSK                                         0x8000
#define HWIO_GE_PCIE_UNCORR_ERR_STATUS_OFF_CMPLT_ABORT_ERR_STATUS_SHFT                                            0xf
#define HWIO_GE_PCIE_UNCORR_ERR_STATUS_OFF_CMPLT_TIMEOUT_ERR_STATUS_BMSK                                       0x4000
#define HWIO_GE_PCIE_UNCORR_ERR_STATUS_OFF_CMPLT_TIMEOUT_ERR_STATUS_SHFT                                          0xe
#define HWIO_GE_PCIE_UNCORR_ERR_STATUS_OFF_FC_PROTOCOL_ERR_STATUS_BMSK                                         0x2000
#define HWIO_GE_PCIE_UNCORR_ERR_STATUS_OFF_FC_PROTOCOL_ERR_STATUS_SHFT                                            0xd
#define HWIO_GE_PCIE_UNCORR_ERR_STATUS_OFF_POIS_TLP_ERR_STATUS_BMSK                                            0x1000
#define HWIO_GE_PCIE_UNCORR_ERR_STATUS_OFF_POIS_TLP_ERR_STATUS_SHFT                                               0xc
#define HWIO_GE_PCIE_UNCORR_ERR_STATUS_OFF_RSVDP_6_BMSK                                                         0xfc0
#define HWIO_GE_PCIE_UNCORR_ERR_STATUS_OFF_RSVDP_6_SHFT                                                           0x6
#define HWIO_GE_PCIE_UNCORR_ERR_STATUS_OFF_SUR_DWN_ERR_STATUS_BMSK                                               0x20
#define HWIO_GE_PCIE_UNCORR_ERR_STATUS_OFF_SUR_DWN_ERR_STATUS_SHFT                                                0x5
#define HWIO_GE_PCIE_UNCORR_ERR_STATUS_OFF_DL_PROTOCOL_ERR_STATUS_BMSK                                           0x10
#define HWIO_GE_PCIE_UNCORR_ERR_STATUS_OFF_DL_PROTOCOL_ERR_STATUS_SHFT                                            0x4
#define HWIO_GE_PCIE_UNCORR_ERR_STATUS_OFF_RSVDP_0_BMSK                                                           0xf
#define HWIO_GE_PCIE_UNCORR_ERR_STATUS_OFF_RSVDP_0_SHFT                                                           0x0

#define HWIO_GE_PCIE_UNCORR_ERR_MASK_OFF_ADDR                                                              (GE_PCIE_PCIE20_DBI_4_REG_BASE      + 0x00000108)
#define HWIO_GE_PCIE_UNCORR_ERR_MASK_OFF_PHYS                                                              (GE_PCIE_PCIE20_DBI_4_REG_BASE_PHYS + 0x00000108)
#define HWIO_GE_PCIE_UNCORR_ERR_MASK_OFF_OFFS                                                              (GE_PCIE_PCIE20_DBI_4_REG_BASE_OFFS + 0x00000108)
#define HWIO_GE_PCIE_UNCORR_ERR_MASK_OFF_RMSK                                                              0xffffffff
#define HWIO_GE_PCIE_UNCORR_ERR_MASK_OFF_POR                                                               0x00000000
#define HWIO_GE_PCIE_UNCORR_ERR_MASK_OFF_POR_RMSK                                                          0xffffffff
#define HWIO_GE_PCIE_UNCORR_ERR_MASK_OFF_ATTR                                                                     0x3
#define HWIO_GE_PCIE_UNCORR_ERR_MASK_OFF_IN          \
        in_dword_masked(HWIO_GE_PCIE_UNCORR_ERR_MASK_OFF_ADDR, HWIO_GE_PCIE_UNCORR_ERR_MASK_OFF_RMSK)
#define HWIO_GE_PCIE_UNCORR_ERR_MASK_OFF_INM(m)      \
        in_dword_masked(HWIO_GE_PCIE_UNCORR_ERR_MASK_OFF_ADDR, m)
#define HWIO_GE_PCIE_UNCORR_ERR_MASK_OFF_OUT(v)      \
        out_dword(HWIO_GE_PCIE_UNCORR_ERR_MASK_OFF_ADDR,v)
#define HWIO_GE_PCIE_UNCORR_ERR_MASK_OFF_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GE_PCIE_UNCORR_ERR_MASK_OFF_ADDR,m,v,HWIO_GE_PCIE_UNCORR_ERR_MASK_OFF_IN)
#define HWIO_GE_PCIE_UNCORR_ERR_MASK_OFF_RSVDP_26_BMSK                                                     0xfc000000
#define HWIO_GE_PCIE_UNCORR_ERR_MASK_OFF_RSVDP_26_SHFT                                                           0x1a
#define HWIO_GE_PCIE_UNCORR_ERR_MASK_OFF_TLP_PRFX_BLOCKED_ERR_MASK_BMSK                                     0x2000000
#define HWIO_GE_PCIE_UNCORR_ERR_MASK_OFF_TLP_PRFX_BLOCKED_ERR_MASK_SHFT                                          0x19
#define HWIO_GE_PCIE_UNCORR_ERR_MASK_OFF_ATOMIC_EGRESS_BLOCKED_ERR_MASK_BMSK                                0x1000000
#define HWIO_GE_PCIE_UNCORR_ERR_MASK_OFF_ATOMIC_EGRESS_BLOCKED_ERR_MASK_SHFT                                     0x18
#define HWIO_GE_PCIE_UNCORR_ERR_MASK_OFF_RSVDP_23_BMSK                                                       0x800000
#define HWIO_GE_PCIE_UNCORR_ERR_MASK_OFF_RSVDP_23_SHFT                                                           0x17
#define HWIO_GE_PCIE_UNCORR_ERR_MASK_OFF_INTERNAL_ERR_MASK_BMSK                                              0x400000
#define HWIO_GE_PCIE_UNCORR_ERR_MASK_OFF_INTERNAL_ERR_MASK_SHFT                                                  0x16
#define HWIO_GE_PCIE_UNCORR_ERR_MASK_OFF_RSVDP_21_BMSK                                                       0x200000
#define HWIO_GE_PCIE_UNCORR_ERR_MASK_OFF_RSVDP_21_SHFT                                                           0x15
#define HWIO_GE_PCIE_UNCORR_ERR_MASK_OFF_UNSUPPORTED_REQ_ERR_MASK_BMSK                                       0x100000
#define HWIO_GE_PCIE_UNCORR_ERR_MASK_OFF_UNSUPPORTED_REQ_ERR_MASK_SHFT                                           0x14
#define HWIO_GE_PCIE_UNCORR_ERR_MASK_OFF_ECRC_ERR_MASK_BMSK                                                   0x80000
#define HWIO_GE_PCIE_UNCORR_ERR_MASK_OFF_ECRC_ERR_MASK_SHFT                                                      0x13
#define HWIO_GE_PCIE_UNCORR_ERR_MASK_OFF_MALF_TLP_ERR_MASK_BMSK                                               0x40000
#define HWIO_GE_PCIE_UNCORR_ERR_MASK_OFF_MALF_TLP_ERR_MASK_SHFT                                                  0x12
#define HWIO_GE_PCIE_UNCORR_ERR_MASK_OFF_REC_OVERFLOW_ERR_MASK_BMSK                                           0x20000
#define HWIO_GE_PCIE_UNCORR_ERR_MASK_OFF_REC_OVERFLOW_ERR_MASK_SHFT                                              0x11
#define HWIO_GE_PCIE_UNCORR_ERR_MASK_OFF_UNEXP_CMPLT_ERR_MASK_BMSK                                            0x10000
#define HWIO_GE_PCIE_UNCORR_ERR_MASK_OFF_UNEXP_CMPLT_ERR_MASK_SHFT                                               0x10
#define HWIO_GE_PCIE_UNCORR_ERR_MASK_OFF_CMPLT_ABORT_ERR_MASK_BMSK                                             0x8000
#define HWIO_GE_PCIE_UNCORR_ERR_MASK_OFF_CMPLT_ABORT_ERR_MASK_SHFT                                                0xf
#define HWIO_GE_PCIE_UNCORR_ERR_MASK_OFF_CMPLT_TIMEOUT_ERR_MASK_BMSK                                           0x4000
#define HWIO_GE_PCIE_UNCORR_ERR_MASK_OFF_CMPLT_TIMEOUT_ERR_MASK_SHFT                                              0xe
#define HWIO_GE_PCIE_UNCORR_ERR_MASK_OFF_FC_PROTOCOL_ERR_MASK_BMSK                                             0x2000
#define HWIO_GE_PCIE_UNCORR_ERR_MASK_OFF_FC_PROTOCOL_ERR_MASK_SHFT                                                0xd
#define HWIO_GE_PCIE_UNCORR_ERR_MASK_OFF_POIS_TLP_ERR_MASK_BMSK                                                0x1000
#define HWIO_GE_PCIE_UNCORR_ERR_MASK_OFF_POIS_TLP_ERR_MASK_SHFT                                                   0xc
#define HWIO_GE_PCIE_UNCORR_ERR_MASK_OFF_RSVDP_6_BMSK                                                           0xfc0
#define HWIO_GE_PCIE_UNCORR_ERR_MASK_OFF_RSVDP_6_SHFT                                                             0x6
#define HWIO_GE_PCIE_UNCORR_ERR_MASK_OFF_SUR_DWN_ERR_MASK_BMSK                                                   0x20
#define HWIO_GE_PCIE_UNCORR_ERR_MASK_OFF_SUR_DWN_ERR_MASK_SHFT                                                    0x5
#define HWIO_GE_PCIE_UNCORR_ERR_MASK_OFF_DL_PROTOCOL_ERR_MASK_BMSK                                               0x10
#define HWIO_GE_PCIE_UNCORR_ERR_MASK_OFF_DL_PROTOCOL_ERR_MASK_SHFT                                                0x4
#define HWIO_GE_PCIE_UNCORR_ERR_MASK_OFF_RSVDP_0_BMSK                                                             0xf
#define HWIO_GE_PCIE_UNCORR_ERR_MASK_OFF_RSVDP_0_SHFT                                                             0x0

#define HWIO_GE_PCIE_UNCORR_ERR_SEV_OFF_ADDR                                                               (GE_PCIE_PCIE20_DBI_4_REG_BASE      + 0x0000010c)
#define HWIO_GE_PCIE_UNCORR_ERR_SEV_OFF_PHYS                                                               (GE_PCIE_PCIE20_DBI_4_REG_BASE_PHYS + 0x0000010c)
#define HWIO_GE_PCIE_UNCORR_ERR_SEV_OFF_OFFS                                                               (GE_PCIE_PCIE20_DBI_4_REG_BASE_OFFS + 0x0000010c)
#define HWIO_GE_PCIE_UNCORR_ERR_SEV_OFF_RMSK                                                               0xffffffff
#define HWIO_GE_PCIE_UNCORR_ERR_SEV_OFF_POR                                                                0x00000000
#define HWIO_GE_PCIE_UNCORR_ERR_SEV_OFF_POR_RMSK                                                           0xffffffff
#define HWIO_GE_PCIE_UNCORR_ERR_SEV_OFF_ATTR                                                                      0x3
#define HWIO_GE_PCIE_UNCORR_ERR_SEV_OFF_IN          \
        in_dword_masked(HWIO_GE_PCIE_UNCORR_ERR_SEV_OFF_ADDR, HWIO_GE_PCIE_UNCORR_ERR_SEV_OFF_RMSK)
#define HWIO_GE_PCIE_UNCORR_ERR_SEV_OFF_INM(m)      \
        in_dword_masked(HWIO_GE_PCIE_UNCORR_ERR_SEV_OFF_ADDR, m)
#define HWIO_GE_PCIE_UNCORR_ERR_SEV_OFF_OUT(v)      \
        out_dword(HWIO_GE_PCIE_UNCORR_ERR_SEV_OFF_ADDR,v)
#define HWIO_GE_PCIE_UNCORR_ERR_SEV_OFF_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GE_PCIE_UNCORR_ERR_SEV_OFF_ADDR,m,v,HWIO_GE_PCIE_UNCORR_ERR_SEV_OFF_IN)
#define HWIO_GE_PCIE_UNCORR_ERR_SEV_OFF_RSVDP_26_BMSK                                                      0xfc000000
#define HWIO_GE_PCIE_UNCORR_ERR_SEV_OFF_RSVDP_26_SHFT                                                            0x1a
#define HWIO_GE_PCIE_UNCORR_ERR_SEV_OFF_TLP_PRFX_BLOCKED_ERR_SEVERITY_BMSK                                  0x2000000
#define HWIO_GE_PCIE_UNCORR_ERR_SEV_OFF_TLP_PRFX_BLOCKED_ERR_SEVERITY_SHFT                                       0x19
#define HWIO_GE_PCIE_UNCORR_ERR_SEV_OFF_ATOMIC_EGRESS_BLOCKED_ERR_SEVERITY_BMSK                             0x1000000
#define HWIO_GE_PCIE_UNCORR_ERR_SEV_OFF_ATOMIC_EGRESS_BLOCKED_ERR_SEVERITY_SHFT                                  0x18
#define HWIO_GE_PCIE_UNCORR_ERR_SEV_OFF_RSVDP_23_BMSK                                                        0x800000
#define HWIO_GE_PCIE_UNCORR_ERR_SEV_OFF_RSVDP_23_SHFT                                                            0x17
#define HWIO_GE_PCIE_UNCORR_ERR_SEV_OFF_INTERNAL_ERR_SEVERITY_BMSK                                           0x400000
#define HWIO_GE_PCIE_UNCORR_ERR_SEV_OFF_INTERNAL_ERR_SEVERITY_SHFT                                               0x16
#define HWIO_GE_PCIE_UNCORR_ERR_SEV_OFF_RSVDP_21_BMSK                                                        0x200000
#define HWIO_GE_PCIE_UNCORR_ERR_SEV_OFF_RSVDP_21_SHFT                                                            0x15
#define HWIO_GE_PCIE_UNCORR_ERR_SEV_OFF_UNSUPPORTED_REQ_ERR_SEVERITY_BMSK                                    0x100000
#define HWIO_GE_PCIE_UNCORR_ERR_SEV_OFF_UNSUPPORTED_REQ_ERR_SEVERITY_SHFT                                        0x14
#define HWIO_GE_PCIE_UNCORR_ERR_SEV_OFF_ECRC_ERR_SEVERITY_BMSK                                                0x80000
#define HWIO_GE_PCIE_UNCORR_ERR_SEV_OFF_ECRC_ERR_SEVERITY_SHFT                                                   0x13
#define HWIO_GE_PCIE_UNCORR_ERR_SEV_OFF_MALF_TLP_ERR_SEVERITY_BMSK                                            0x40000
#define HWIO_GE_PCIE_UNCORR_ERR_SEV_OFF_MALF_TLP_ERR_SEVERITY_SHFT                                               0x12
#define HWIO_GE_PCIE_UNCORR_ERR_SEV_OFF_REC_OVERFLOW_ERR_SEVERITY_BMSK                                        0x20000
#define HWIO_GE_PCIE_UNCORR_ERR_SEV_OFF_REC_OVERFLOW_ERR_SEVERITY_SHFT                                           0x11
#define HWIO_GE_PCIE_UNCORR_ERR_SEV_OFF_UNEXP_CMPLT_ERR_SEVERITY_BMSK                                         0x10000
#define HWIO_GE_PCIE_UNCORR_ERR_SEV_OFF_UNEXP_CMPLT_ERR_SEVERITY_SHFT                                            0x10
#define HWIO_GE_PCIE_UNCORR_ERR_SEV_OFF_CMPLT_ABORT_ERR_SEVERITY_BMSK                                          0x8000
#define HWIO_GE_PCIE_UNCORR_ERR_SEV_OFF_CMPLT_ABORT_ERR_SEVERITY_SHFT                                             0xf
#define HWIO_GE_PCIE_UNCORR_ERR_SEV_OFF_CMPLT_TIMEOUT_ERR_SEVERITY_BMSK                                        0x4000
#define HWIO_GE_PCIE_UNCORR_ERR_SEV_OFF_CMPLT_TIMEOUT_ERR_SEVERITY_SHFT                                           0xe
#define HWIO_GE_PCIE_UNCORR_ERR_SEV_OFF_FC_PROTOCOL_ERR_SEVERITY_BMSK                                          0x2000
#define HWIO_GE_PCIE_UNCORR_ERR_SEV_OFF_FC_PROTOCOL_ERR_SEVERITY_SHFT                                             0xd
#define HWIO_GE_PCIE_UNCORR_ERR_SEV_OFF_POIS_TLP_ERR_SEVERITY_BMSK                                             0x1000
#define HWIO_GE_PCIE_UNCORR_ERR_SEV_OFF_POIS_TLP_ERR_SEVERITY_SHFT                                                0xc
#define HWIO_GE_PCIE_UNCORR_ERR_SEV_OFF_RSVDP_6_BMSK                                                            0xfc0
#define HWIO_GE_PCIE_UNCORR_ERR_SEV_OFF_RSVDP_6_SHFT                                                              0x6
#define HWIO_GE_PCIE_UNCORR_ERR_SEV_OFF_SUR_DWN_ERR_SEVERITY_BMSK                                                0x20
#define HWIO_GE_PCIE_UNCORR_ERR_SEV_OFF_SUR_DWN_ERR_SEVERITY_SHFT                                                 0x5
#define HWIO_GE_PCIE_UNCORR_ERR_SEV_OFF_DL_PROTOCOL_ERR_SEVERITY_BMSK                                            0x10
#define HWIO_GE_PCIE_UNCORR_ERR_SEV_OFF_DL_PROTOCOL_ERR_SEVERITY_SHFT                                             0x4
#define HWIO_GE_PCIE_UNCORR_ERR_SEV_OFF_RSVDP_0_BMSK                                                              0xf
#define HWIO_GE_PCIE_UNCORR_ERR_SEV_OFF_RSVDP_0_SHFT                                                              0x0

#define HWIO_GE_PCIE_CORR_ERR_STATUS_OFF_ADDR                                                              (GE_PCIE_PCIE20_DBI_4_REG_BASE      + 0x00000110)
#define HWIO_GE_PCIE_CORR_ERR_STATUS_OFF_PHYS                                                              (GE_PCIE_PCIE20_DBI_4_REG_BASE_PHYS + 0x00000110)
#define HWIO_GE_PCIE_CORR_ERR_STATUS_OFF_OFFS                                                              (GE_PCIE_PCIE20_DBI_4_REG_BASE_OFFS + 0x00000110)
#define HWIO_GE_PCIE_CORR_ERR_STATUS_OFF_RMSK                                                              0xffffffff
#define HWIO_GE_PCIE_CORR_ERR_STATUS_OFF_POR                                                               0x00000000
#define HWIO_GE_PCIE_CORR_ERR_STATUS_OFF_POR_RMSK                                                          0xffffffff
#define HWIO_GE_PCIE_CORR_ERR_STATUS_OFF_ATTR                                                                     0x3
#define HWIO_GE_PCIE_CORR_ERR_STATUS_OFF_IN          \
        in_dword_masked(HWIO_GE_PCIE_CORR_ERR_STATUS_OFF_ADDR, HWIO_GE_PCIE_CORR_ERR_STATUS_OFF_RMSK)
#define HWIO_GE_PCIE_CORR_ERR_STATUS_OFF_INM(m)      \
        in_dword_masked(HWIO_GE_PCIE_CORR_ERR_STATUS_OFF_ADDR, m)
#define HWIO_GE_PCIE_CORR_ERR_STATUS_OFF_OUT(v)      \
        out_dword(HWIO_GE_PCIE_CORR_ERR_STATUS_OFF_ADDR,v)
#define HWIO_GE_PCIE_CORR_ERR_STATUS_OFF_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GE_PCIE_CORR_ERR_STATUS_OFF_ADDR,m,v,HWIO_GE_PCIE_CORR_ERR_STATUS_OFF_IN)
#define HWIO_GE_PCIE_CORR_ERR_STATUS_OFF_RSVDP_15_BMSK                                                     0xffff8000
#define HWIO_GE_PCIE_CORR_ERR_STATUS_OFF_RSVDP_15_SHFT                                                            0xf
#define HWIO_GE_PCIE_CORR_ERR_STATUS_OFF_CORRECTED_INT_ERR_STATUS_BMSK                                         0x4000
#define HWIO_GE_PCIE_CORR_ERR_STATUS_OFF_CORRECTED_INT_ERR_STATUS_SHFT                                            0xe
#define HWIO_GE_PCIE_CORR_ERR_STATUS_OFF_ADVISORY_NON_FATAL_ERR_STATUS_BMSK                                    0x2000
#define HWIO_GE_PCIE_CORR_ERR_STATUS_OFF_ADVISORY_NON_FATAL_ERR_STATUS_SHFT                                       0xd
#define HWIO_GE_PCIE_CORR_ERR_STATUS_OFF_RPL_TIMER_TIMEOUT_STATUS_BMSK                                         0x1000
#define HWIO_GE_PCIE_CORR_ERR_STATUS_OFF_RPL_TIMER_TIMEOUT_STATUS_SHFT                                            0xc
#define HWIO_GE_PCIE_CORR_ERR_STATUS_OFF_RSVDP_9_BMSK                                                           0xe00
#define HWIO_GE_PCIE_CORR_ERR_STATUS_OFF_RSVDP_9_SHFT                                                             0x9
#define HWIO_GE_PCIE_CORR_ERR_STATUS_OFF_REPLAY_NO_ROLEOVER_STATUS_BMSK                                         0x100
#define HWIO_GE_PCIE_CORR_ERR_STATUS_OFF_REPLAY_NO_ROLEOVER_STATUS_SHFT                                           0x8
#define HWIO_GE_PCIE_CORR_ERR_STATUS_OFF_BAD_DLLP_STATUS_BMSK                                                    0x80
#define HWIO_GE_PCIE_CORR_ERR_STATUS_OFF_BAD_DLLP_STATUS_SHFT                                                     0x7
#define HWIO_GE_PCIE_CORR_ERR_STATUS_OFF_BAD_TLP_STATUS_BMSK                                                     0x40
#define HWIO_GE_PCIE_CORR_ERR_STATUS_OFF_BAD_TLP_STATUS_SHFT                                                      0x6
#define HWIO_GE_PCIE_CORR_ERR_STATUS_OFF_RSVDP_1_BMSK                                                            0x3e
#define HWIO_GE_PCIE_CORR_ERR_STATUS_OFF_RSVDP_1_SHFT                                                             0x1
#define HWIO_GE_PCIE_CORR_ERR_STATUS_OFF_RX_ERR_STATUS_BMSK                                                       0x1
#define HWIO_GE_PCIE_CORR_ERR_STATUS_OFF_RX_ERR_STATUS_SHFT                                                       0x0

#define HWIO_GE_PCIE_CORR_ERR_MASK_OFF_ADDR                                                                (GE_PCIE_PCIE20_DBI_4_REG_BASE      + 0x00000114)
#define HWIO_GE_PCIE_CORR_ERR_MASK_OFF_PHYS                                                                (GE_PCIE_PCIE20_DBI_4_REG_BASE_PHYS + 0x00000114)
#define HWIO_GE_PCIE_CORR_ERR_MASK_OFF_OFFS                                                                (GE_PCIE_PCIE20_DBI_4_REG_BASE_OFFS + 0x00000114)
#define HWIO_GE_PCIE_CORR_ERR_MASK_OFF_RMSK                                                                0xffffffff
#define HWIO_GE_PCIE_CORR_ERR_MASK_OFF_POR                                                                 0x00000000
#define HWIO_GE_PCIE_CORR_ERR_MASK_OFF_POR_RMSK                                                            0xffffffff
#define HWIO_GE_PCIE_CORR_ERR_MASK_OFF_ATTR                                                                       0x3
#define HWIO_GE_PCIE_CORR_ERR_MASK_OFF_IN          \
        in_dword_masked(HWIO_GE_PCIE_CORR_ERR_MASK_OFF_ADDR, HWIO_GE_PCIE_CORR_ERR_MASK_OFF_RMSK)
#define HWIO_GE_PCIE_CORR_ERR_MASK_OFF_INM(m)      \
        in_dword_masked(HWIO_GE_PCIE_CORR_ERR_MASK_OFF_ADDR, m)
#define HWIO_GE_PCIE_CORR_ERR_MASK_OFF_OUT(v)      \
        out_dword(HWIO_GE_PCIE_CORR_ERR_MASK_OFF_ADDR,v)
#define HWIO_GE_PCIE_CORR_ERR_MASK_OFF_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GE_PCIE_CORR_ERR_MASK_OFF_ADDR,m,v,HWIO_GE_PCIE_CORR_ERR_MASK_OFF_IN)
#define HWIO_GE_PCIE_CORR_ERR_MASK_OFF_RSVDP_15_BMSK                                                       0xffff8000
#define HWIO_GE_PCIE_CORR_ERR_MASK_OFF_RSVDP_15_SHFT                                                              0xf
#define HWIO_GE_PCIE_CORR_ERR_MASK_OFF_CORRECTED_INT_ERR_MASK_BMSK                                             0x4000
#define HWIO_GE_PCIE_CORR_ERR_MASK_OFF_CORRECTED_INT_ERR_MASK_SHFT                                                0xe
#define HWIO_GE_PCIE_CORR_ERR_MASK_OFF_ADVISORY_NON_FATAL_ERR_MASK_BMSK                                        0x2000
#define HWIO_GE_PCIE_CORR_ERR_MASK_OFF_ADVISORY_NON_FATAL_ERR_MASK_SHFT                                           0xd
#define HWIO_GE_PCIE_CORR_ERR_MASK_OFF_RPL_TIMER_TIMEOUT_MASK_BMSK                                             0x1000
#define HWIO_GE_PCIE_CORR_ERR_MASK_OFF_RPL_TIMER_TIMEOUT_MASK_SHFT                                                0xc
#define HWIO_GE_PCIE_CORR_ERR_MASK_OFF_RSVDP_9_BMSK                                                             0xe00
#define HWIO_GE_PCIE_CORR_ERR_MASK_OFF_RSVDP_9_SHFT                                                               0x9
#define HWIO_GE_PCIE_CORR_ERR_MASK_OFF_REPLAY_NO_ROLEOVER_MASK_BMSK                                             0x100
#define HWIO_GE_PCIE_CORR_ERR_MASK_OFF_REPLAY_NO_ROLEOVER_MASK_SHFT                                               0x8
#define HWIO_GE_PCIE_CORR_ERR_MASK_OFF_BAD_DLLP_MASK_BMSK                                                        0x80
#define HWIO_GE_PCIE_CORR_ERR_MASK_OFF_BAD_DLLP_MASK_SHFT                                                         0x7
#define HWIO_GE_PCIE_CORR_ERR_MASK_OFF_BAD_TLP_MASK_BMSK                                                         0x40
#define HWIO_GE_PCIE_CORR_ERR_MASK_OFF_BAD_TLP_MASK_SHFT                                                          0x6
#define HWIO_GE_PCIE_CORR_ERR_MASK_OFF_RSVDP_1_BMSK                                                              0x3e
#define HWIO_GE_PCIE_CORR_ERR_MASK_OFF_RSVDP_1_SHFT                                                               0x1
#define HWIO_GE_PCIE_CORR_ERR_MASK_OFF_RX_ERR_MASK_BMSK                                                           0x1
#define HWIO_GE_PCIE_CORR_ERR_MASK_OFF_RX_ERR_MASK_SHFT                                                           0x0

#define HWIO_GE_PCIE_ADV_ERR_CAP_CTRL_OFF_ADDR                                                             (GE_PCIE_PCIE20_DBI_4_REG_BASE      + 0x00000118)
#define HWIO_GE_PCIE_ADV_ERR_CAP_CTRL_OFF_PHYS                                                             (GE_PCIE_PCIE20_DBI_4_REG_BASE_PHYS + 0x00000118)
#define HWIO_GE_PCIE_ADV_ERR_CAP_CTRL_OFF_OFFS                                                             (GE_PCIE_PCIE20_DBI_4_REG_BASE_OFFS + 0x00000118)
#define HWIO_GE_PCIE_ADV_ERR_CAP_CTRL_OFF_RMSK                                                             0xfffff7ff
#define HWIO_GE_PCIE_ADV_ERR_CAP_CTRL_OFF_POR                                                              0x000000a0
#define HWIO_GE_PCIE_ADV_ERR_CAP_CTRL_OFF_POR_RMSK                                                         0xffffffff
#define HWIO_GE_PCIE_ADV_ERR_CAP_CTRL_OFF_ATTR                                                                    0x3
#define HWIO_GE_PCIE_ADV_ERR_CAP_CTRL_OFF_IN          \
        in_dword_masked(HWIO_GE_PCIE_ADV_ERR_CAP_CTRL_OFF_ADDR, HWIO_GE_PCIE_ADV_ERR_CAP_CTRL_OFF_RMSK)
#define HWIO_GE_PCIE_ADV_ERR_CAP_CTRL_OFF_INM(m)      \
        in_dword_masked(HWIO_GE_PCIE_ADV_ERR_CAP_CTRL_OFF_ADDR, m)
#define HWIO_GE_PCIE_ADV_ERR_CAP_CTRL_OFF_OUT(v)      \
        out_dword(HWIO_GE_PCIE_ADV_ERR_CAP_CTRL_OFF_ADDR,v)
#define HWIO_GE_PCIE_ADV_ERR_CAP_CTRL_OFF_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GE_PCIE_ADV_ERR_CAP_CTRL_OFF_ADDR,m,v,HWIO_GE_PCIE_ADV_ERR_CAP_CTRL_OFF_IN)
#define HWIO_GE_PCIE_ADV_ERR_CAP_CTRL_OFF_RSVDP_12_BMSK                                                    0xfffff000
#define HWIO_GE_PCIE_ADV_ERR_CAP_CTRL_OFF_RSVDP_12_SHFT                                                           0xc
#define HWIO_GE_PCIE_ADV_ERR_CAP_CTRL_OFF_RSVDP_9_BMSK                                                          0x600
#define HWIO_GE_PCIE_ADV_ERR_CAP_CTRL_OFF_RSVDP_9_SHFT                                                            0x9
#define HWIO_GE_PCIE_ADV_ERR_CAP_CTRL_OFF_ECRC_CHECK_EN_BMSK                                                    0x100
#define HWIO_GE_PCIE_ADV_ERR_CAP_CTRL_OFF_ECRC_CHECK_EN_SHFT                                                      0x8
#define HWIO_GE_PCIE_ADV_ERR_CAP_CTRL_OFF_ECRC_CHECK_CAP_BMSK                                                    0x80
#define HWIO_GE_PCIE_ADV_ERR_CAP_CTRL_OFF_ECRC_CHECK_CAP_SHFT                                                     0x7
#define HWIO_GE_PCIE_ADV_ERR_CAP_CTRL_OFF_ECRC_GEN_EN_BMSK                                                       0x40
#define HWIO_GE_PCIE_ADV_ERR_CAP_CTRL_OFF_ECRC_GEN_EN_SHFT                                                        0x6
#define HWIO_GE_PCIE_ADV_ERR_CAP_CTRL_OFF_ECRC_GEN_CAP_BMSK                                                      0x20
#define HWIO_GE_PCIE_ADV_ERR_CAP_CTRL_OFF_ECRC_GEN_CAP_SHFT                                                       0x5
#define HWIO_GE_PCIE_ADV_ERR_CAP_CTRL_OFF_FIRST_ERR_POINTER_BMSK                                                 0x1f
#define HWIO_GE_PCIE_ADV_ERR_CAP_CTRL_OFF_FIRST_ERR_POINTER_SHFT                                                  0x0

#define HWIO_GE_PCIE_HDR_LOG_0_OFF_ADDR                                                                    (GE_PCIE_PCIE20_DBI_4_REG_BASE      + 0x0000011c)
#define HWIO_GE_PCIE_HDR_LOG_0_OFF_PHYS                                                                    (GE_PCIE_PCIE20_DBI_4_REG_BASE_PHYS + 0x0000011c)
#define HWIO_GE_PCIE_HDR_LOG_0_OFF_OFFS                                                                    (GE_PCIE_PCIE20_DBI_4_REG_BASE_OFFS + 0x0000011c)
#define HWIO_GE_PCIE_HDR_LOG_0_OFF_RMSK                                                                    0xffffffff
#define HWIO_GE_PCIE_HDR_LOG_0_OFF_POR                                                                     0x00000000
#define HWIO_GE_PCIE_HDR_LOG_0_OFF_POR_RMSK                                                                0xffffffff
#define HWIO_GE_PCIE_HDR_LOG_0_OFF_ATTR                                                                           0x1
#define HWIO_GE_PCIE_HDR_LOG_0_OFF_IN          \
        in_dword_masked(HWIO_GE_PCIE_HDR_LOG_0_OFF_ADDR, HWIO_GE_PCIE_HDR_LOG_0_OFF_RMSK)
#define HWIO_GE_PCIE_HDR_LOG_0_OFF_INM(m)      \
        in_dword_masked(HWIO_GE_PCIE_HDR_LOG_0_OFF_ADDR, m)
#define HWIO_GE_PCIE_HDR_LOG_0_OFF_FIRST_DWORD_BMSK                                                        0xffffffff
#define HWIO_GE_PCIE_HDR_LOG_0_OFF_FIRST_DWORD_SHFT                                                               0x0

#define HWIO_GE_PCIE_HDR_LOG_1_OFF_ADDR                                                                    (GE_PCIE_PCIE20_DBI_4_REG_BASE      + 0x00000120)
#define HWIO_GE_PCIE_HDR_LOG_1_OFF_PHYS                                                                    (GE_PCIE_PCIE20_DBI_4_REG_BASE_PHYS + 0x00000120)
#define HWIO_GE_PCIE_HDR_LOG_1_OFF_OFFS                                                                    (GE_PCIE_PCIE20_DBI_4_REG_BASE_OFFS + 0x00000120)
#define HWIO_GE_PCIE_HDR_LOG_1_OFF_RMSK                                                                    0xffffffff
#define HWIO_GE_PCIE_HDR_LOG_1_OFF_POR                                                                     0x00000000
#define HWIO_GE_PCIE_HDR_LOG_1_OFF_POR_RMSK                                                                0xffffffff
#define HWIO_GE_PCIE_HDR_LOG_1_OFF_ATTR                                                                           0x1
#define HWIO_GE_PCIE_HDR_LOG_1_OFF_IN          \
        in_dword_masked(HWIO_GE_PCIE_HDR_LOG_1_OFF_ADDR, HWIO_GE_PCIE_HDR_LOG_1_OFF_RMSK)
#define HWIO_GE_PCIE_HDR_LOG_1_OFF_INM(m)      \
        in_dword_masked(HWIO_GE_PCIE_HDR_LOG_1_OFF_ADDR, m)
#define HWIO_GE_PCIE_HDR_LOG_1_OFF_SECOND_DWORD_BMSK                                                       0xffffffff
#define HWIO_GE_PCIE_HDR_LOG_1_OFF_SECOND_DWORD_SHFT                                                              0x0

#define HWIO_GE_PCIE_HDR_LOG_2_OFF_ADDR                                                                    (GE_PCIE_PCIE20_DBI_4_REG_BASE      + 0x00000124)
#define HWIO_GE_PCIE_HDR_LOG_2_OFF_PHYS                                                                    (GE_PCIE_PCIE20_DBI_4_REG_BASE_PHYS + 0x00000124)
#define HWIO_GE_PCIE_HDR_LOG_2_OFF_OFFS                                                                    (GE_PCIE_PCIE20_DBI_4_REG_BASE_OFFS + 0x00000124)
#define HWIO_GE_PCIE_HDR_LOG_2_OFF_RMSK                                                                    0xffffffff
#define HWIO_GE_PCIE_HDR_LOG_2_OFF_POR                                                                     0x00000000
#define HWIO_GE_PCIE_HDR_LOG_2_OFF_POR_RMSK                                                                0xffffffff
#define HWIO_GE_PCIE_HDR_LOG_2_OFF_ATTR                                                                           0x1
#define HWIO_GE_PCIE_HDR_LOG_2_OFF_IN          \
        in_dword_masked(HWIO_GE_PCIE_HDR_LOG_2_OFF_ADDR, HWIO_GE_PCIE_HDR_LOG_2_OFF_RMSK)
#define HWIO_GE_PCIE_HDR_LOG_2_OFF_INM(m)      \
        in_dword_masked(HWIO_GE_PCIE_HDR_LOG_2_OFF_ADDR, m)
#define HWIO_GE_PCIE_HDR_LOG_2_OFF_THIRD_DWORD_BMSK                                                        0xffffffff
#define HWIO_GE_PCIE_HDR_LOG_2_OFF_THIRD_DWORD_SHFT                                                               0x0

#define HWIO_GE_PCIE_HDR_LOG_3_OFF_ADDR                                                                    (GE_PCIE_PCIE20_DBI_4_REG_BASE      + 0x00000128)
#define HWIO_GE_PCIE_HDR_LOG_3_OFF_PHYS                                                                    (GE_PCIE_PCIE20_DBI_4_REG_BASE_PHYS + 0x00000128)
#define HWIO_GE_PCIE_HDR_LOG_3_OFF_OFFS                                                                    (GE_PCIE_PCIE20_DBI_4_REG_BASE_OFFS + 0x00000128)
#define HWIO_GE_PCIE_HDR_LOG_3_OFF_RMSK                                                                    0xffffffff
#define HWIO_GE_PCIE_HDR_LOG_3_OFF_POR                                                                     0x00000000
#define HWIO_GE_PCIE_HDR_LOG_3_OFF_POR_RMSK                                                                0xffffffff
#define HWIO_GE_PCIE_HDR_LOG_3_OFF_ATTR                                                                           0x1
#define HWIO_GE_PCIE_HDR_LOG_3_OFF_IN          \
        in_dword_masked(HWIO_GE_PCIE_HDR_LOG_3_OFF_ADDR, HWIO_GE_PCIE_HDR_LOG_3_OFF_RMSK)
#define HWIO_GE_PCIE_HDR_LOG_3_OFF_INM(m)      \
        in_dword_masked(HWIO_GE_PCIE_HDR_LOG_3_OFF_ADDR, m)
#define HWIO_GE_PCIE_HDR_LOG_3_OFF_FOURTH_DWORD_BMSK                                                       0xffffffff
#define HWIO_GE_PCIE_HDR_LOG_3_OFF_FOURTH_DWORD_SHFT                                                              0x0

#define HWIO_GE_PCIE_ROOT_ERR_CMD_OFF_ADDR                                                                 (GE_PCIE_PCIE20_DBI_4_REG_BASE      + 0x0000012c)
#define HWIO_GE_PCIE_ROOT_ERR_CMD_OFF_PHYS                                                                 (GE_PCIE_PCIE20_DBI_4_REG_BASE_PHYS + 0x0000012c)
#define HWIO_GE_PCIE_ROOT_ERR_CMD_OFF_OFFS                                                                 (GE_PCIE_PCIE20_DBI_4_REG_BASE_OFFS + 0x0000012c)
#define HWIO_GE_PCIE_ROOT_ERR_CMD_OFF_RMSK                                                                 0xffffffff
#define HWIO_GE_PCIE_ROOT_ERR_CMD_OFF_POR                                                                  0x00000000
#define HWIO_GE_PCIE_ROOT_ERR_CMD_OFF_POR_RMSK                                                             0xffffffff
#define HWIO_GE_PCIE_ROOT_ERR_CMD_OFF_ATTR                                                                        0x3
#define HWIO_GE_PCIE_ROOT_ERR_CMD_OFF_IN          \
        in_dword_masked(HWIO_GE_PCIE_ROOT_ERR_CMD_OFF_ADDR, HWIO_GE_PCIE_ROOT_ERR_CMD_OFF_RMSK)
#define HWIO_GE_PCIE_ROOT_ERR_CMD_OFF_INM(m)      \
        in_dword_masked(HWIO_GE_PCIE_ROOT_ERR_CMD_OFF_ADDR, m)
#define HWIO_GE_PCIE_ROOT_ERR_CMD_OFF_OUT(v)      \
        out_dword(HWIO_GE_PCIE_ROOT_ERR_CMD_OFF_ADDR,v)
#define HWIO_GE_PCIE_ROOT_ERR_CMD_OFF_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GE_PCIE_ROOT_ERR_CMD_OFF_ADDR,m,v,HWIO_GE_PCIE_ROOT_ERR_CMD_OFF_IN)
#define HWIO_GE_PCIE_ROOT_ERR_CMD_OFF_RSVDP_3_BMSK                                                         0xfffffff8
#define HWIO_GE_PCIE_ROOT_ERR_CMD_OFF_RSVDP_3_SHFT                                                                0x3
#define HWIO_GE_PCIE_ROOT_ERR_CMD_OFF_FATAL_ERR_REPORTING_EN_BMSK                                                 0x4
#define HWIO_GE_PCIE_ROOT_ERR_CMD_OFF_FATAL_ERR_REPORTING_EN_SHFT                                                 0x2
#define HWIO_GE_PCIE_ROOT_ERR_CMD_OFF_NON_FATAL_ERR_REPORTING_EN_BMSK                                             0x2
#define HWIO_GE_PCIE_ROOT_ERR_CMD_OFF_NON_FATAL_ERR_REPORTING_EN_SHFT                                             0x1
#define HWIO_GE_PCIE_ROOT_ERR_CMD_OFF_CORR_ERR_REPORTING_EN_BMSK                                                  0x1
#define HWIO_GE_PCIE_ROOT_ERR_CMD_OFF_CORR_ERR_REPORTING_EN_SHFT                                                  0x0

#define HWIO_GE_PCIE_ROOT_ERR_STATUS_OFF_ADDR                                                              (GE_PCIE_PCIE20_DBI_4_REG_BASE      + 0x00000130)
#define HWIO_GE_PCIE_ROOT_ERR_STATUS_OFF_PHYS                                                              (GE_PCIE_PCIE20_DBI_4_REG_BASE_PHYS + 0x00000130)
#define HWIO_GE_PCIE_ROOT_ERR_STATUS_OFF_OFFS                                                              (GE_PCIE_PCIE20_DBI_4_REG_BASE_OFFS + 0x00000130)
#define HWIO_GE_PCIE_ROOT_ERR_STATUS_OFF_RMSK                                                              0xffffffff
#define HWIO_GE_PCIE_ROOT_ERR_STATUS_OFF_POR                                                               0x00000000
#define HWIO_GE_PCIE_ROOT_ERR_STATUS_OFF_POR_RMSK                                                          0xffffffff
#define HWIO_GE_PCIE_ROOT_ERR_STATUS_OFF_ATTR                                                                     0x3
#define HWIO_GE_PCIE_ROOT_ERR_STATUS_OFF_IN          \
        in_dword_masked(HWIO_GE_PCIE_ROOT_ERR_STATUS_OFF_ADDR, HWIO_GE_PCIE_ROOT_ERR_STATUS_OFF_RMSK)
#define HWIO_GE_PCIE_ROOT_ERR_STATUS_OFF_INM(m)      \
        in_dword_masked(HWIO_GE_PCIE_ROOT_ERR_STATUS_OFF_ADDR, m)
#define HWIO_GE_PCIE_ROOT_ERR_STATUS_OFF_OUT(v)      \
        out_dword(HWIO_GE_PCIE_ROOT_ERR_STATUS_OFF_ADDR,v)
#define HWIO_GE_PCIE_ROOT_ERR_STATUS_OFF_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GE_PCIE_ROOT_ERR_STATUS_OFF_ADDR,m,v,HWIO_GE_PCIE_ROOT_ERR_STATUS_OFF_IN)
#define HWIO_GE_PCIE_ROOT_ERR_STATUS_OFF_ADV_ERR_INT_MSG_NUM_BMSK                                          0xf8000000
#define HWIO_GE_PCIE_ROOT_ERR_STATUS_OFF_ADV_ERR_INT_MSG_NUM_SHFT                                                0x1b
#define HWIO_GE_PCIE_ROOT_ERR_STATUS_OFF_RSVDP_7_BMSK                                                       0x7ffff80
#define HWIO_GE_PCIE_ROOT_ERR_STATUS_OFF_RSVDP_7_SHFT                                                             0x7
#define HWIO_GE_PCIE_ROOT_ERR_STATUS_OFF_FATAL_ERR_MSG_RX_BMSK                                                   0x40
#define HWIO_GE_PCIE_ROOT_ERR_STATUS_OFF_FATAL_ERR_MSG_RX_SHFT                                                    0x6
#define HWIO_GE_PCIE_ROOT_ERR_STATUS_OFF_NON_FATAL_ERR_MSG_RX_BMSK                                               0x20
#define HWIO_GE_PCIE_ROOT_ERR_STATUS_OFF_NON_FATAL_ERR_MSG_RX_SHFT                                                0x5
#define HWIO_GE_PCIE_ROOT_ERR_STATUS_OFF_FIRST_UNCORR_FATAL_BMSK                                                 0x10
#define HWIO_GE_PCIE_ROOT_ERR_STATUS_OFF_FIRST_UNCORR_FATAL_SHFT                                                  0x4
#define HWIO_GE_PCIE_ROOT_ERR_STATUS_OFF_MUL_ERR_FATAL_NON_FATAL_RX_BMSK                                          0x8
#define HWIO_GE_PCIE_ROOT_ERR_STATUS_OFF_MUL_ERR_FATAL_NON_FATAL_RX_SHFT                                          0x3
#define HWIO_GE_PCIE_ROOT_ERR_STATUS_OFF_ERR_FATAL_NON_FATAL_RX_BMSK                                              0x4
#define HWIO_GE_PCIE_ROOT_ERR_STATUS_OFF_ERR_FATAL_NON_FATAL_RX_SHFT                                              0x2
#define HWIO_GE_PCIE_ROOT_ERR_STATUS_OFF_MUL_ERR_COR_RX_BMSK                                                      0x2
#define HWIO_GE_PCIE_ROOT_ERR_STATUS_OFF_MUL_ERR_COR_RX_SHFT                                                      0x1
#define HWIO_GE_PCIE_ROOT_ERR_STATUS_OFF_ERR_COR_RX_BMSK                                                          0x1
#define HWIO_GE_PCIE_ROOT_ERR_STATUS_OFF_ERR_COR_RX_SHFT                                                          0x0

#define HWIO_GE_PCIE_ERR_SRC_ID_OFF_ADDR                                                                   (GE_PCIE_PCIE20_DBI_4_REG_BASE      + 0x00000134)
#define HWIO_GE_PCIE_ERR_SRC_ID_OFF_PHYS                                                                   (GE_PCIE_PCIE20_DBI_4_REG_BASE_PHYS + 0x00000134)
#define HWIO_GE_PCIE_ERR_SRC_ID_OFF_OFFS                                                                   (GE_PCIE_PCIE20_DBI_4_REG_BASE_OFFS + 0x00000134)
#define HWIO_GE_PCIE_ERR_SRC_ID_OFF_RMSK                                                                   0xffffffff
#define HWIO_GE_PCIE_ERR_SRC_ID_OFF_POR                                                                    0x00000000
#define HWIO_GE_PCIE_ERR_SRC_ID_OFF_POR_RMSK                                                               0xffffffff
#define HWIO_GE_PCIE_ERR_SRC_ID_OFF_ATTR                                                                          0x1
#define HWIO_GE_PCIE_ERR_SRC_ID_OFF_IN          \
        in_dword_masked(HWIO_GE_PCIE_ERR_SRC_ID_OFF_ADDR, HWIO_GE_PCIE_ERR_SRC_ID_OFF_RMSK)
#define HWIO_GE_PCIE_ERR_SRC_ID_OFF_INM(m)      \
        in_dword_masked(HWIO_GE_PCIE_ERR_SRC_ID_OFF_ADDR, m)
#define HWIO_GE_PCIE_ERR_SRC_ID_OFF_ERR_FATAL_NON_FATAL_SOURCE_ID_BMSK                                     0xffff0000
#define HWIO_GE_PCIE_ERR_SRC_ID_OFF_ERR_FATAL_NON_FATAL_SOURCE_ID_SHFT                                           0x10
#define HWIO_GE_PCIE_ERR_SRC_ID_OFF_ERR_COR_SOURCE_ID_BMSK                                                     0xffff
#define HWIO_GE_PCIE_ERR_SRC_ID_OFF_ERR_COR_SOURCE_ID_SHFT                                                        0x0

#define HWIO_GE_PCIE_TLP_PREFIX_LOG_1_OFF_ADDR                                                             (GE_PCIE_PCIE20_DBI_4_REG_BASE      + 0x00000138)
#define HWIO_GE_PCIE_TLP_PREFIX_LOG_1_OFF_PHYS                                                             (GE_PCIE_PCIE20_DBI_4_REG_BASE_PHYS + 0x00000138)
#define HWIO_GE_PCIE_TLP_PREFIX_LOG_1_OFF_OFFS                                                             (GE_PCIE_PCIE20_DBI_4_REG_BASE_OFFS + 0x00000138)
#define HWIO_GE_PCIE_TLP_PREFIX_LOG_1_OFF_RMSK                                                             0xffffffff
#define HWIO_GE_PCIE_TLP_PREFIX_LOG_1_OFF_POR                                                              0x00000000
#define HWIO_GE_PCIE_TLP_PREFIX_LOG_1_OFF_POR_RMSK                                                         0xffffffff
#define HWIO_GE_PCIE_TLP_PREFIX_LOG_1_OFF_ATTR                                                                    0x1
#define HWIO_GE_PCIE_TLP_PREFIX_LOG_1_OFF_IN          \
        in_dword_masked(HWIO_GE_PCIE_TLP_PREFIX_LOG_1_OFF_ADDR, HWIO_GE_PCIE_TLP_PREFIX_LOG_1_OFF_RMSK)
#define HWIO_GE_PCIE_TLP_PREFIX_LOG_1_OFF_INM(m)      \
        in_dword_masked(HWIO_GE_PCIE_TLP_PREFIX_LOG_1_OFF_ADDR, m)
#define HWIO_GE_PCIE_TLP_PREFIX_LOG_1_OFF_CFG_TLP_PFX_LOG_1_BMSK                                           0xffffffff
#define HWIO_GE_PCIE_TLP_PREFIX_LOG_1_OFF_CFG_TLP_PFX_LOG_1_SHFT                                                  0x0

#define HWIO_GE_PCIE_TLP_PREFIX_LOG_2_OFF_ADDR                                                             (GE_PCIE_PCIE20_DBI_4_REG_BASE      + 0x0000013c)
#define HWIO_GE_PCIE_TLP_PREFIX_LOG_2_OFF_PHYS                                                             (GE_PCIE_PCIE20_DBI_4_REG_BASE_PHYS + 0x0000013c)
#define HWIO_GE_PCIE_TLP_PREFIX_LOG_2_OFF_OFFS                                                             (GE_PCIE_PCIE20_DBI_4_REG_BASE_OFFS + 0x0000013c)
#define HWIO_GE_PCIE_TLP_PREFIX_LOG_2_OFF_RMSK                                                             0xffffffff
#define HWIO_GE_PCIE_TLP_PREFIX_LOG_2_OFF_POR                                                              0x00000000
#define HWIO_GE_PCIE_TLP_PREFIX_LOG_2_OFF_POR_RMSK                                                         0xffffffff
#define HWIO_GE_PCIE_TLP_PREFIX_LOG_2_OFF_ATTR                                                                    0x1
#define HWIO_GE_PCIE_TLP_PREFIX_LOG_2_OFF_IN          \
        in_dword_masked(HWIO_GE_PCIE_TLP_PREFIX_LOG_2_OFF_ADDR, HWIO_GE_PCIE_TLP_PREFIX_LOG_2_OFF_RMSK)
#define HWIO_GE_PCIE_TLP_PREFIX_LOG_2_OFF_INM(m)      \
        in_dword_masked(HWIO_GE_PCIE_TLP_PREFIX_LOG_2_OFF_ADDR, m)
#define HWIO_GE_PCIE_TLP_PREFIX_LOG_2_OFF_CFG_TLP_PFX_LOG_2_BMSK                                           0xffffffff
#define HWIO_GE_PCIE_TLP_PREFIX_LOG_2_OFF_CFG_TLP_PFX_LOG_2_SHFT                                                  0x0

#define HWIO_GE_PCIE_TLP_PREFIX_LOG_3_OFF_ADDR                                                             (GE_PCIE_PCIE20_DBI_4_REG_BASE      + 0x00000140)
#define HWIO_GE_PCIE_TLP_PREFIX_LOG_3_OFF_PHYS                                                             (GE_PCIE_PCIE20_DBI_4_REG_BASE_PHYS + 0x00000140)
#define HWIO_GE_PCIE_TLP_PREFIX_LOG_3_OFF_OFFS                                                             (GE_PCIE_PCIE20_DBI_4_REG_BASE_OFFS + 0x00000140)
#define HWIO_GE_PCIE_TLP_PREFIX_LOG_3_OFF_RMSK                                                             0xffffffff
#define HWIO_GE_PCIE_TLP_PREFIX_LOG_3_OFF_POR                                                              0x00000000
#define HWIO_GE_PCIE_TLP_PREFIX_LOG_3_OFF_POR_RMSK                                                         0xffffffff
#define HWIO_GE_PCIE_TLP_PREFIX_LOG_3_OFF_ATTR                                                                    0x1
#define HWIO_GE_PCIE_TLP_PREFIX_LOG_3_OFF_IN          \
        in_dword_masked(HWIO_GE_PCIE_TLP_PREFIX_LOG_3_OFF_ADDR, HWIO_GE_PCIE_TLP_PREFIX_LOG_3_OFF_RMSK)
#define HWIO_GE_PCIE_TLP_PREFIX_LOG_3_OFF_INM(m)      \
        in_dword_masked(HWIO_GE_PCIE_TLP_PREFIX_LOG_3_OFF_ADDR, m)
#define HWIO_GE_PCIE_TLP_PREFIX_LOG_3_OFF_CFG_TLP_PFX_LOG_3_BMSK                                           0xffffffff
#define HWIO_GE_PCIE_TLP_PREFIX_LOG_3_OFF_CFG_TLP_PFX_LOG_3_SHFT                                                  0x0

#define HWIO_GE_PCIE_TLP_PREFIX_LOG_4_OFF_ADDR                                                             (GE_PCIE_PCIE20_DBI_4_REG_BASE      + 0x00000144)
#define HWIO_GE_PCIE_TLP_PREFIX_LOG_4_OFF_PHYS                                                             (GE_PCIE_PCIE20_DBI_4_REG_BASE_PHYS + 0x00000144)
#define HWIO_GE_PCIE_TLP_PREFIX_LOG_4_OFF_OFFS                                                             (GE_PCIE_PCIE20_DBI_4_REG_BASE_OFFS + 0x00000144)
#define HWIO_GE_PCIE_TLP_PREFIX_LOG_4_OFF_RMSK                                                             0xffffffff
#define HWIO_GE_PCIE_TLP_PREFIX_LOG_4_OFF_POR                                                              0x00000000
#define HWIO_GE_PCIE_TLP_PREFIX_LOG_4_OFF_POR_RMSK                                                         0xffffffff
#define HWIO_GE_PCIE_TLP_PREFIX_LOG_4_OFF_ATTR                                                                    0x1
#define HWIO_GE_PCIE_TLP_PREFIX_LOG_4_OFF_IN          \
        in_dword_masked(HWIO_GE_PCIE_TLP_PREFIX_LOG_4_OFF_ADDR, HWIO_GE_PCIE_TLP_PREFIX_LOG_4_OFF_RMSK)
#define HWIO_GE_PCIE_TLP_PREFIX_LOG_4_OFF_INM(m)      \
        in_dword_masked(HWIO_GE_PCIE_TLP_PREFIX_LOG_4_OFF_ADDR, m)
#define HWIO_GE_PCIE_TLP_PREFIX_LOG_4_OFF_CFG_TLP_PFX_LOG_4_BMSK                                           0xffffffff
#define HWIO_GE_PCIE_TLP_PREFIX_LOG_4_OFF_CFG_TLP_PFX_LOG_4_SHFT                                                  0x0

#define HWIO_GE_PCIE_RBAR_EXT_CAP_HEADER_REG_ADDR                                                          (GE_PCIE_PCIE20_DBI_4_REG_BASE      + 0x00000150)
#define HWIO_GE_PCIE_RBAR_EXT_CAP_HEADER_REG_PHYS                                                          (GE_PCIE_PCIE20_DBI_4_REG_BASE_PHYS + 0x00000150)
#define HWIO_GE_PCIE_RBAR_EXT_CAP_HEADER_REG_OFFS                                                          (GE_PCIE_PCIE20_DBI_4_REG_BASE_OFFS + 0x00000150)
#define HWIO_GE_PCIE_RBAR_EXT_CAP_HEADER_REG_RMSK                                                          0xffffffff
#define HWIO_GE_PCIE_RBAR_EXT_CAP_HEADER_REG_POR                                                           0x00000000
#define HWIO_GE_PCIE_RBAR_EXT_CAP_HEADER_REG_POR_RMSK                                                      0xffffffff
#define HWIO_GE_PCIE_RBAR_EXT_CAP_HEADER_REG_ATTR                                                                 0x1
#define HWIO_GE_PCIE_RBAR_EXT_CAP_HEADER_REG_IN          \
        in_dword_masked(HWIO_GE_PCIE_RBAR_EXT_CAP_HEADER_REG_ADDR, HWIO_GE_PCIE_RBAR_EXT_CAP_HEADER_REG_RMSK)
#define HWIO_GE_PCIE_RBAR_EXT_CAP_HEADER_REG_INM(m)      \
        in_dword_masked(HWIO_GE_PCIE_RBAR_EXT_CAP_HEADER_REG_ADDR, m)
#define HWIO_GE_PCIE_RBAR_EXT_CAP_HEADER_REG_NEXT_OFFSET_BMSK                                              0xfff00000
#define HWIO_GE_PCIE_RBAR_EXT_CAP_HEADER_REG_NEXT_OFFSET_SHFT                                                    0x14
#define HWIO_GE_PCIE_RBAR_EXT_CAP_HEADER_REG_CAP_VERSION_BMSK                                                 0xf0000
#define HWIO_GE_PCIE_RBAR_EXT_CAP_HEADER_REG_CAP_VERSION_SHFT                                                    0x10
#define HWIO_GE_PCIE_RBAR_EXT_CAP_HEADER_REG_CAP_ID_BMSK                                                       0xffff
#define HWIO_GE_PCIE_RBAR_EXT_CAP_HEADER_REG_CAP_ID_SHFT                                                          0x0

#define HWIO_GE_PCIE_RBAR_CAPABILITY_REG_ADDR                                                              (GE_PCIE_PCIE20_DBI_4_REG_BASE      + 0x00000154)
#define HWIO_GE_PCIE_RBAR_CAPABILITY_REG_PHYS                                                              (GE_PCIE_PCIE20_DBI_4_REG_BASE_PHYS + 0x00000154)
#define HWIO_GE_PCIE_RBAR_CAPABILITY_REG_OFFS                                                              (GE_PCIE_PCIE20_DBI_4_REG_BASE_OFFS + 0x00000154)
#define HWIO_GE_PCIE_RBAR_CAPABILITY_REG_RMSK                                                                0xfffff0
#define HWIO_GE_PCIE_RBAR_CAPABILITY_REG_POR                                                               0x00001040
#define HWIO_GE_PCIE_RBAR_CAPABILITY_REG_POR_RMSK                                                          0xffffffff
#define HWIO_GE_PCIE_RBAR_CAPABILITY_REG_ATTR                                                                     0x3
#define HWIO_GE_PCIE_RBAR_CAPABILITY_REG_IN          \
        in_dword_masked(HWIO_GE_PCIE_RBAR_CAPABILITY_REG_ADDR, HWIO_GE_PCIE_RBAR_CAPABILITY_REG_RMSK)
#define HWIO_GE_PCIE_RBAR_CAPABILITY_REG_INM(m)      \
        in_dword_masked(HWIO_GE_PCIE_RBAR_CAPABILITY_REG_ADDR, m)
#define HWIO_GE_PCIE_RBAR_CAPABILITY_REG_OUT(v)      \
        out_dword(HWIO_GE_PCIE_RBAR_CAPABILITY_REG_ADDR,v)
#define HWIO_GE_PCIE_RBAR_CAPABILITY_REG_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GE_PCIE_RBAR_CAPABILITY_REG_ADDR,m,v,HWIO_GE_PCIE_RBAR_CAPABILITY_REG_IN)
#define HWIO_GE_PCIE_RBAR_CAPABILITY_REG_BAR_512GB_ALLOW_BMSK                                                0x800000
#define HWIO_GE_PCIE_RBAR_CAPABILITY_REG_BAR_512GB_ALLOW_SHFT                                                    0x17
#define HWIO_GE_PCIE_RBAR_CAPABILITY_REG_BAR_256GB_ALLOW_BMSK                                                0x400000
#define HWIO_GE_PCIE_RBAR_CAPABILITY_REG_BAR_256GB_ALLOW_SHFT                                                    0x16
#define HWIO_GE_PCIE_RBAR_CAPABILITY_REG_BAR_128GB_ALLOW_BMSK                                                0x200000
#define HWIO_GE_PCIE_RBAR_CAPABILITY_REG_BAR_128GB_ALLOW_SHFT                                                    0x15
#define HWIO_GE_PCIE_RBAR_CAPABILITY_REG_BAR_64GB_ALLOW_BMSK                                                 0x100000
#define HWIO_GE_PCIE_RBAR_CAPABILITY_REG_BAR_64GB_ALLOW_SHFT                                                     0x14
#define HWIO_GE_PCIE_RBAR_CAPABILITY_REG_BAR_32GB_ALLOW_BMSK                                                  0x80000
#define HWIO_GE_PCIE_RBAR_CAPABILITY_REG_BAR_32GB_ALLOW_SHFT                                                     0x13
#define HWIO_GE_PCIE_RBAR_CAPABILITY_REG_BAR_16GB_ALLOW_BMSK                                                  0x40000
#define HWIO_GE_PCIE_RBAR_CAPABILITY_REG_BAR_16GB_ALLOW_SHFT                                                     0x12
#define HWIO_GE_PCIE_RBAR_CAPABILITY_REG_BAR_8GB_ALLOW_BMSK                                                   0x20000
#define HWIO_GE_PCIE_RBAR_CAPABILITY_REG_BAR_8GB_ALLOW_SHFT                                                      0x11
#define HWIO_GE_PCIE_RBAR_CAPABILITY_REG_BAR_4GB_ALLOW_BMSK                                                   0x10000
#define HWIO_GE_PCIE_RBAR_CAPABILITY_REG_BAR_4GB_ALLOW_SHFT                                                      0x10
#define HWIO_GE_PCIE_RBAR_CAPABILITY_REG_BAR_2GB_ALLOW_BMSK                                                    0x8000
#define HWIO_GE_PCIE_RBAR_CAPABILITY_REG_BAR_2GB_ALLOW_SHFT                                                       0xf
#define HWIO_GE_PCIE_RBAR_CAPABILITY_REG_BAR_1GB_ALLOW_BMSK                                                    0x4000
#define HWIO_GE_PCIE_RBAR_CAPABILITY_REG_BAR_1GB_ALLOW_SHFT                                                       0xe
#define HWIO_GE_PCIE_RBAR_CAPABILITY_REG_BAR_512MB_ALLOW_BMSK                                                  0x2000
#define HWIO_GE_PCIE_RBAR_CAPABILITY_REG_BAR_512MB_ALLOW_SHFT                                                     0xd
#define HWIO_GE_PCIE_RBAR_CAPABILITY_REG_BAR_256MB_ALLOW_BMSK                                                  0x1000
#define HWIO_GE_PCIE_RBAR_CAPABILITY_REG_BAR_256MB_ALLOW_SHFT                                                     0xc
#define HWIO_GE_PCIE_RBAR_CAPABILITY_REG_BAR_128MB_ALLOW_BMSK                                                   0x800
#define HWIO_GE_PCIE_RBAR_CAPABILITY_REG_BAR_128MB_ALLOW_SHFT                                                     0xb
#define HWIO_GE_PCIE_RBAR_CAPABILITY_REG_BAR_64MB_ALLOW_BMSK                                                    0x400
#define HWIO_GE_PCIE_RBAR_CAPABILITY_REG_BAR_64MB_ALLOW_SHFT                                                      0xa
#define HWIO_GE_PCIE_RBAR_CAPABILITY_REG_BAR_32MB_ALLOW_BMSK                                                    0x200
#define HWIO_GE_PCIE_RBAR_CAPABILITY_REG_BAR_32MB_ALLOW_SHFT                                                      0x9
#define HWIO_GE_PCIE_RBAR_CAPABILITY_REG_BAR_16MB_ALLOW_BMSK                                                    0x100
#define HWIO_GE_PCIE_RBAR_CAPABILITY_REG_BAR_16MB_ALLOW_SHFT                                                      0x8
#define HWIO_GE_PCIE_RBAR_CAPABILITY_REG_BAR_8MB_ALLOW_BMSK                                                      0x80
#define HWIO_GE_PCIE_RBAR_CAPABILITY_REG_BAR_8MB_ALLOW_SHFT                                                       0x7
#define HWIO_GE_PCIE_RBAR_CAPABILITY_REG_BAR_4MB_ALLOW_BMSK                                                      0x40
#define HWIO_GE_PCIE_RBAR_CAPABILITY_REG_BAR_4MB_ALLOW_SHFT                                                       0x6
#define HWIO_GE_PCIE_RBAR_CAPABILITY_REG_BAR_2MB_ALLOW_BMSK                                                      0x20
#define HWIO_GE_PCIE_RBAR_CAPABILITY_REG_BAR_2MB_ALLOW_SHFT                                                       0x5
#define HWIO_GE_PCIE_RBAR_CAPABILITY_REG_BAR_1MB_ALLOW_BMSK                                                      0x10
#define HWIO_GE_PCIE_RBAR_CAPABILITY_REG_BAR_1MB_ALLOW_SHFT                                                       0x4

#define HWIO_GE_PCIE_RBAR_CONTROL_REG_ADDR                                                                 (GE_PCIE_PCIE20_DBI_4_REG_BASE      + 0x00000158)
#define HWIO_GE_PCIE_RBAR_CONTROL_REG_PHYS                                                                 (GE_PCIE_PCIE20_DBI_4_REG_BASE_PHYS + 0x00000158)
#define HWIO_GE_PCIE_RBAR_CONTROL_REG_OFFS                                                                 (GE_PCIE_PCIE20_DBI_4_REG_BASE_OFFS + 0x00000158)
#define HWIO_GE_PCIE_RBAR_CONTROL_REG_RMSK                                                                     0x1fe7
#define HWIO_GE_PCIE_RBAR_CONTROL_REG_POR                                                                  0x00000000
#define HWIO_GE_PCIE_RBAR_CONTROL_REG_POR_RMSK                                                             0xffffffff
#define HWIO_GE_PCIE_RBAR_CONTROL_REG_ATTR                                                                        0x3
#define HWIO_GE_PCIE_RBAR_CONTROL_REG_IN          \
        in_dword_masked(HWIO_GE_PCIE_RBAR_CONTROL_REG_ADDR, HWIO_GE_PCIE_RBAR_CONTROL_REG_RMSK)
#define HWIO_GE_PCIE_RBAR_CONTROL_REG_INM(m)      \
        in_dword_masked(HWIO_GE_PCIE_RBAR_CONTROL_REG_ADDR, m)
#define HWIO_GE_PCIE_RBAR_CONTROL_REG_OUT(v)      \
        out_dword(HWIO_GE_PCIE_RBAR_CONTROL_REG_ADDR,v)
#define HWIO_GE_PCIE_RBAR_CONTROL_REG_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GE_PCIE_RBAR_CONTROL_REG_ADDR,m,v,HWIO_GE_PCIE_RBAR_CONTROL_REG_IN)
#define HWIO_GE_PCIE_RBAR_CONTROL_REG_RBAR_SIZE_BMSK                                                           0x1f00
#define HWIO_GE_PCIE_RBAR_CONTROL_REG_RBAR_SIZE_SHFT                                                              0x8
#define HWIO_GE_PCIE_RBAR_CONTROL_REG_TOTAL_RBARS_BMSK                                                           0xe0
#define HWIO_GE_PCIE_RBAR_CONTROL_REG_TOTAL_RBARS_SHFT                                                            0x5
#define HWIO_GE_PCIE_RBAR_CONTROL_REG_BAR_INDEX_BMSK                                                              0x7
#define HWIO_GE_PCIE_RBAR_CONTROL_REG_BAR_INDEX_SHFT                                                              0x0

#define HWIO_GE_PCIE_PCI_MSI_CAP_ID_NEXT_CTRL_REG_ADDR                                                     (GE_PCIE_PCIE20_DBI_4_REG_BASE      + 0x00000050)
#define HWIO_GE_PCIE_PCI_MSI_CAP_ID_NEXT_CTRL_REG_PHYS                                                     (GE_PCIE_PCIE20_DBI_4_REG_BASE_PHYS + 0x00000050)
#define HWIO_GE_PCIE_PCI_MSI_CAP_ID_NEXT_CTRL_REG_OFFS                                                     (GE_PCIE_PCIE20_DBI_4_REG_BASE_OFFS + 0x00000050)
#define HWIO_GE_PCIE_PCI_MSI_CAP_ID_NEXT_CTRL_REG_RMSK                                                     0xffffffff
#define HWIO_GE_PCIE_PCI_MSI_CAP_ID_NEXT_CTRL_REG_POR                                                      0x00000000
#define HWIO_GE_PCIE_PCI_MSI_CAP_ID_NEXT_CTRL_REG_POR_RMSK                                                 0xffffffff
#define HWIO_GE_PCIE_PCI_MSI_CAP_ID_NEXT_CTRL_REG_ATTR                                                            0x3
#define HWIO_GE_PCIE_PCI_MSI_CAP_ID_NEXT_CTRL_REG_IN          \
        in_dword_masked(HWIO_GE_PCIE_PCI_MSI_CAP_ID_NEXT_CTRL_REG_ADDR, HWIO_GE_PCIE_PCI_MSI_CAP_ID_NEXT_CTRL_REG_RMSK)
#define HWIO_GE_PCIE_PCI_MSI_CAP_ID_NEXT_CTRL_REG_INM(m)      \
        in_dword_masked(HWIO_GE_PCIE_PCI_MSI_CAP_ID_NEXT_CTRL_REG_ADDR, m)
#define HWIO_GE_PCIE_PCI_MSI_CAP_ID_NEXT_CTRL_REG_OUT(v)      \
        out_dword(HWIO_GE_PCIE_PCI_MSI_CAP_ID_NEXT_CTRL_REG_ADDR,v)
#define HWIO_GE_PCIE_PCI_MSI_CAP_ID_NEXT_CTRL_REG_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GE_PCIE_PCI_MSI_CAP_ID_NEXT_CTRL_REG_ADDR,m,v,HWIO_GE_PCIE_PCI_MSI_CAP_ID_NEXT_CTRL_REG_IN)
#define HWIO_GE_PCIE_PCI_MSI_CAP_ID_NEXT_CTRL_REG_RSVDP_25_BMSK                                            0xfe000000
#define HWIO_GE_PCIE_PCI_MSI_CAP_ID_NEXT_CTRL_REG_RSVDP_25_SHFT                                                  0x19
#define HWIO_GE_PCIE_PCI_MSI_CAP_ID_NEXT_CTRL_REG_PCI_PVM_SUPPORT_BMSK                                      0x1000000
#define HWIO_GE_PCIE_PCI_MSI_CAP_ID_NEXT_CTRL_REG_PCI_PVM_SUPPORT_SHFT                                           0x18
#define HWIO_GE_PCIE_PCI_MSI_CAP_ID_NEXT_CTRL_REG_PCI_MSI_64_BIT_ADDR_CAP_BMSK                               0x800000
#define HWIO_GE_PCIE_PCI_MSI_CAP_ID_NEXT_CTRL_REG_PCI_MSI_64_BIT_ADDR_CAP_SHFT                                   0x17
#define HWIO_GE_PCIE_PCI_MSI_CAP_ID_NEXT_CTRL_REG_PCI_MSI_MULTIPLE_MSG_EN_BMSK                               0x700000
#define HWIO_GE_PCIE_PCI_MSI_CAP_ID_NEXT_CTRL_REG_PCI_MSI_MULTIPLE_MSG_EN_SHFT                                   0x14
#define HWIO_GE_PCIE_PCI_MSI_CAP_ID_NEXT_CTRL_REG_PCI_MSI_MULTIPLE_MSG_CAP_BMSK                               0xe0000
#define HWIO_GE_PCIE_PCI_MSI_CAP_ID_NEXT_CTRL_REG_PCI_MSI_MULTIPLE_MSG_CAP_SHFT                                  0x11
#define HWIO_GE_PCIE_PCI_MSI_CAP_ID_NEXT_CTRL_REG_PCI_MSI_ENABLE_BMSK                                         0x10000
#define HWIO_GE_PCIE_PCI_MSI_CAP_ID_NEXT_CTRL_REG_PCI_MSI_ENABLE_SHFT                                            0x10
#define HWIO_GE_PCIE_PCI_MSI_CAP_ID_NEXT_CTRL_REG_PCI_MSI_CAP_NEXT_OFFSET_BMSK                                 0xff00
#define HWIO_GE_PCIE_PCI_MSI_CAP_ID_NEXT_CTRL_REG_PCI_MSI_CAP_NEXT_OFFSET_SHFT                                    0x8
#define HWIO_GE_PCIE_PCI_MSI_CAP_ID_NEXT_CTRL_REG_PCI_MSI_CAP_ID_BMSK                                            0xff
#define HWIO_GE_PCIE_PCI_MSI_CAP_ID_NEXT_CTRL_REG_PCI_MSI_CAP_ID_SHFT                                             0x0

#define HWIO_GE_PCIE_MSI_CAP_OFF_04H_REG_ADDR                                                              (GE_PCIE_PCIE20_DBI_4_REG_BASE      + 0x00000054)
#define HWIO_GE_PCIE_MSI_CAP_OFF_04H_REG_PHYS                                                              (GE_PCIE_PCIE20_DBI_4_REG_BASE_PHYS + 0x00000054)
#define HWIO_GE_PCIE_MSI_CAP_OFF_04H_REG_OFFS                                                              (GE_PCIE_PCIE20_DBI_4_REG_BASE_OFFS + 0x00000054)
#define HWIO_GE_PCIE_MSI_CAP_OFF_04H_REG_RMSK                                                              0xffffffff
#define HWIO_GE_PCIE_MSI_CAP_OFF_04H_REG_POR                                                               0x00000000
#define HWIO_GE_PCIE_MSI_CAP_OFF_04H_REG_POR_RMSK                                                          0xffffffff
#define HWIO_GE_PCIE_MSI_CAP_OFF_04H_REG_ATTR                                                                     0x3
#define HWIO_GE_PCIE_MSI_CAP_OFF_04H_REG_IN          \
        in_dword_masked(HWIO_GE_PCIE_MSI_CAP_OFF_04H_REG_ADDR, HWIO_GE_PCIE_MSI_CAP_OFF_04H_REG_RMSK)
#define HWIO_GE_PCIE_MSI_CAP_OFF_04H_REG_INM(m)      \
        in_dword_masked(HWIO_GE_PCIE_MSI_CAP_OFF_04H_REG_ADDR, m)
#define HWIO_GE_PCIE_MSI_CAP_OFF_04H_REG_OUT(v)      \
        out_dword(HWIO_GE_PCIE_MSI_CAP_OFF_04H_REG_ADDR,v)
#define HWIO_GE_PCIE_MSI_CAP_OFF_04H_REG_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GE_PCIE_MSI_CAP_OFF_04H_REG_ADDR,m,v,HWIO_GE_PCIE_MSI_CAP_OFF_04H_REG_IN)
#define HWIO_GE_PCIE_MSI_CAP_OFF_04H_REG_PCI_MSI_CAP_OFF_04H_BMSK                                          0xfffffffc
#define HWIO_GE_PCIE_MSI_CAP_OFF_04H_REG_PCI_MSI_CAP_OFF_04H_SHFT                                                 0x2
#define HWIO_GE_PCIE_MSI_CAP_OFF_04H_REG_RSVDP_0_BMSK                                                             0x3
#define HWIO_GE_PCIE_MSI_CAP_OFF_04H_REG_RSVDP_0_SHFT                                                             0x0

#define HWIO_GE_PCIE_MSI_CAP_OFF_08H_REG_ADDR                                                              (GE_PCIE_PCIE20_DBI_4_REG_BASE      + 0x00000058)
#define HWIO_GE_PCIE_MSI_CAP_OFF_08H_REG_PHYS                                                              (GE_PCIE_PCIE20_DBI_4_REG_BASE_PHYS + 0x00000058)
#define HWIO_GE_PCIE_MSI_CAP_OFF_08H_REG_OFFS                                                              (GE_PCIE_PCIE20_DBI_4_REG_BASE_OFFS + 0x00000058)
#define HWIO_GE_PCIE_MSI_CAP_OFF_08H_REG_RMSK                                                              0xffffffff
#define HWIO_GE_PCIE_MSI_CAP_OFF_08H_REG_POR                                                               0x00000000
#define HWIO_GE_PCIE_MSI_CAP_OFF_08H_REG_POR_RMSK                                                          0xffffffff
#define HWIO_GE_PCIE_MSI_CAP_OFF_08H_REG_ATTR                                                                     0x3
#define HWIO_GE_PCIE_MSI_CAP_OFF_08H_REG_IN          \
        in_dword_masked(HWIO_GE_PCIE_MSI_CAP_OFF_08H_REG_ADDR, HWIO_GE_PCIE_MSI_CAP_OFF_08H_REG_RMSK)
#define HWIO_GE_PCIE_MSI_CAP_OFF_08H_REG_INM(m)      \
        in_dword_masked(HWIO_GE_PCIE_MSI_CAP_OFF_08H_REG_ADDR, m)
#define HWIO_GE_PCIE_MSI_CAP_OFF_08H_REG_OUT(v)      \
        out_dword(HWIO_GE_PCIE_MSI_CAP_OFF_08H_REG_ADDR,v)
#define HWIO_GE_PCIE_MSI_CAP_OFF_08H_REG_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GE_PCIE_MSI_CAP_OFF_08H_REG_ADDR,m,v,HWIO_GE_PCIE_MSI_CAP_OFF_08H_REG_IN)
#define HWIO_GE_PCIE_MSI_CAP_OFF_08H_REG_PCI_MSI_CAP_OFF_0AH_BMSK                                          0xffff0000
#define HWIO_GE_PCIE_MSI_CAP_OFF_08H_REG_PCI_MSI_CAP_OFF_0AH_SHFT                                                0x10
#define HWIO_GE_PCIE_MSI_CAP_OFF_08H_REG_PCI_MSI_CAP_OFF_08H_BMSK                                              0xffff
#define HWIO_GE_PCIE_MSI_CAP_OFF_08H_REG_PCI_MSI_CAP_OFF_08H_SHFT                                                 0x0

#define HWIO_GE_PCIE_MSI_CAP_OFF_0CH_REG_ADDR                                                              (GE_PCIE_PCIE20_DBI_4_REG_BASE      + 0x0000005c)
#define HWIO_GE_PCIE_MSI_CAP_OFF_0CH_REG_PHYS                                                              (GE_PCIE_PCIE20_DBI_4_REG_BASE_PHYS + 0x0000005c)
#define HWIO_GE_PCIE_MSI_CAP_OFF_0CH_REG_OFFS                                                              (GE_PCIE_PCIE20_DBI_4_REG_BASE_OFFS + 0x0000005c)
#define HWIO_GE_PCIE_MSI_CAP_OFF_0CH_REG_RMSK                                                              0xffffffff
#define HWIO_GE_PCIE_MSI_CAP_OFF_0CH_REG_POR                                                               0x00000000
#define HWIO_GE_PCIE_MSI_CAP_OFF_0CH_REG_POR_RMSK                                                          0xffffffff
#define HWIO_GE_PCIE_MSI_CAP_OFF_0CH_REG_ATTR                                                                     0x3
#define HWIO_GE_PCIE_MSI_CAP_OFF_0CH_REG_IN          \
        in_dword_masked(HWIO_GE_PCIE_MSI_CAP_OFF_0CH_REG_ADDR, HWIO_GE_PCIE_MSI_CAP_OFF_0CH_REG_RMSK)
#define HWIO_GE_PCIE_MSI_CAP_OFF_0CH_REG_INM(m)      \
        in_dword_masked(HWIO_GE_PCIE_MSI_CAP_OFF_0CH_REG_ADDR, m)
#define HWIO_GE_PCIE_MSI_CAP_OFF_0CH_REG_OUT(v)      \
        out_dword(HWIO_GE_PCIE_MSI_CAP_OFF_0CH_REG_ADDR,v)
#define HWIO_GE_PCIE_MSI_CAP_OFF_0CH_REG_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GE_PCIE_MSI_CAP_OFF_0CH_REG_ADDR,m,v,HWIO_GE_PCIE_MSI_CAP_OFF_0CH_REG_IN)
#define HWIO_GE_PCIE_MSI_CAP_OFF_0CH_REG_PCI_MSI_CAP_OFF_0EH_BMSK                                          0xffff0000
#define HWIO_GE_PCIE_MSI_CAP_OFF_0CH_REG_PCI_MSI_CAP_OFF_0EH_SHFT                                                0x10
#define HWIO_GE_PCIE_MSI_CAP_OFF_0CH_REG_PCI_MSI_CAP_OFF_0CH_BMSK                                              0xffff
#define HWIO_GE_PCIE_MSI_CAP_OFF_0CH_REG_PCI_MSI_CAP_OFF_0CH_SHFT                                                 0x0

#define HWIO_GE_PCIE_MSI_CAP_OFF_10H_REG_ADDR                                                              (GE_PCIE_PCIE20_DBI_4_REG_BASE      + 0x00000060)
#define HWIO_GE_PCIE_MSI_CAP_OFF_10H_REG_PHYS                                                              (GE_PCIE_PCIE20_DBI_4_REG_BASE_PHYS + 0x00000060)
#define HWIO_GE_PCIE_MSI_CAP_OFF_10H_REG_OFFS                                                              (GE_PCIE_PCIE20_DBI_4_REG_BASE_OFFS + 0x00000060)
#define HWIO_GE_PCIE_MSI_CAP_OFF_10H_REG_RMSK                                                              0xffffffff
#define HWIO_GE_PCIE_MSI_CAP_OFF_10H_REG_POR                                                               0x00000000
#define HWIO_GE_PCIE_MSI_CAP_OFF_10H_REG_POR_RMSK                                                          0xffffffff
#define HWIO_GE_PCIE_MSI_CAP_OFF_10H_REG_ATTR                                                                     0x3
#define HWIO_GE_PCIE_MSI_CAP_OFF_10H_REG_IN          \
        in_dword_masked(HWIO_GE_PCIE_MSI_CAP_OFF_10H_REG_ADDR, HWIO_GE_PCIE_MSI_CAP_OFF_10H_REG_RMSK)
#define HWIO_GE_PCIE_MSI_CAP_OFF_10H_REG_INM(m)      \
        in_dword_masked(HWIO_GE_PCIE_MSI_CAP_OFF_10H_REG_ADDR, m)
#define HWIO_GE_PCIE_MSI_CAP_OFF_10H_REG_OUT(v)      \
        out_dword(HWIO_GE_PCIE_MSI_CAP_OFF_10H_REG_ADDR,v)
#define HWIO_GE_PCIE_MSI_CAP_OFF_10H_REG_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GE_PCIE_MSI_CAP_OFF_10H_REG_ADDR,m,v,HWIO_GE_PCIE_MSI_CAP_OFF_10H_REG_IN)
#define HWIO_GE_PCIE_MSI_CAP_OFF_10H_REG_PCI_MSI_CAP_OFF_10H_BMSK                                          0xffffffff
#define HWIO_GE_PCIE_MSI_CAP_OFF_10H_REG_PCI_MSI_CAP_OFF_10H_SHFT                                                 0x0

#define HWIO_GE_PCIE_MSI_CAP_OFF_14H_REG_ADDR                                                              (GE_PCIE_PCIE20_DBI_4_REG_BASE      + 0x00000064)
#define HWIO_GE_PCIE_MSI_CAP_OFF_14H_REG_PHYS                                                              (GE_PCIE_PCIE20_DBI_4_REG_BASE_PHYS + 0x00000064)
#define HWIO_GE_PCIE_MSI_CAP_OFF_14H_REG_OFFS                                                              (GE_PCIE_PCIE20_DBI_4_REG_BASE_OFFS + 0x00000064)
#define HWIO_GE_PCIE_MSI_CAP_OFF_14H_REG_RMSK                                                              0xffffffff
#define HWIO_GE_PCIE_MSI_CAP_OFF_14H_REG_POR                                                               0x00000000
#define HWIO_GE_PCIE_MSI_CAP_OFF_14H_REG_POR_RMSK                                                          0xffffffff
#define HWIO_GE_PCIE_MSI_CAP_OFF_14H_REG_ATTR                                                                     0x1
#define HWIO_GE_PCIE_MSI_CAP_OFF_14H_REG_IN          \
        in_dword_masked(HWIO_GE_PCIE_MSI_CAP_OFF_14H_REG_ADDR, HWIO_GE_PCIE_MSI_CAP_OFF_14H_REG_RMSK)
#define HWIO_GE_PCIE_MSI_CAP_OFF_14H_REG_INM(m)      \
        in_dword_masked(HWIO_GE_PCIE_MSI_CAP_OFF_14H_REG_ADDR, m)
#define HWIO_GE_PCIE_MSI_CAP_OFF_14H_REG_PCI_MSI_CAP_OFF_14H_BMSK                                          0xffffffff
#define HWIO_GE_PCIE_MSI_CAP_OFF_14H_REG_PCI_MSI_CAP_OFF_14H_SHFT                                                 0x0

#define HWIO_GE_PCIE_ACK_LATENCY_TIMER_OFF_ADDR                                                            (GE_PCIE_PCIE20_DBI_4_REG_BASE      + 0x00000700)
#define HWIO_GE_PCIE_ACK_LATENCY_TIMER_OFF_PHYS                                                            (GE_PCIE_PCIE20_DBI_4_REG_BASE_PHYS + 0x00000700)
#define HWIO_GE_PCIE_ACK_LATENCY_TIMER_OFF_OFFS                                                            (GE_PCIE_PCIE20_DBI_4_REG_BASE_OFFS + 0x00000700)
#define HWIO_GE_PCIE_ACK_LATENCY_TIMER_OFF_RMSK                                                            0xffffffff
#define HWIO_GE_PCIE_ACK_LATENCY_TIMER_OFF_POR                                                             0x00000000
#define HWIO_GE_PCIE_ACK_LATENCY_TIMER_OFF_POR_RMSK                                                        0xffffffff
#define HWIO_GE_PCIE_ACK_LATENCY_TIMER_OFF_ATTR                                                                   0x3
#define HWIO_GE_PCIE_ACK_LATENCY_TIMER_OFF_IN          \
        in_dword_masked(HWIO_GE_PCIE_ACK_LATENCY_TIMER_OFF_ADDR, HWIO_GE_PCIE_ACK_LATENCY_TIMER_OFF_RMSK)
#define HWIO_GE_PCIE_ACK_LATENCY_TIMER_OFF_INM(m)      \
        in_dword_masked(HWIO_GE_PCIE_ACK_LATENCY_TIMER_OFF_ADDR, m)
#define HWIO_GE_PCIE_ACK_LATENCY_TIMER_OFF_OUT(v)      \
        out_dword(HWIO_GE_PCIE_ACK_LATENCY_TIMER_OFF_ADDR,v)
#define HWIO_GE_PCIE_ACK_LATENCY_TIMER_OFF_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GE_PCIE_ACK_LATENCY_TIMER_OFF_ADDR,m,v,HWIO_GE_PCIE_ACK_LATENCY_TIMER_OFF_IN)
#define HWIO_GE_PCIE_ACK_LATENCY_TIMER_OFF_REPLAY_TIME_LIMIT_BMSK                                          0xffff0000
#define HWIO_GE_PCIE_ACK_LATENCY_TIMER_OFF_REPLAY_TIME_LIMIT_SHFT                                                0x10
#define HWIO_GE_PCIE_ACK_LATENCY_TIMER_OFF_ROUND_TRIP_LATENCY_TIME_LIMIT_BMSK                                  0xffff
#define HWIO_GE_PCIE_ACK_LATENCY_TIMER_OFF_ROUND_TRIP_LATENCY_TIME_LIMIT_SHFT                                     0x0

#define HWIO_GE_PCIE_VENDOR_SPEC_DLLP_OFF_ADDR                                                             (GE_PCIE_PCIE20_DBI_4_REG_BASE      + 0x00000704)
#define HWIO_GE_PCIE_VENDOR_SPEC_DLLP_OFF_PHYS                                                             (GE_PCIE_PCIE20_DBI_4_REG_BASE_PHYS + 0x00000704)
#define HWIO_GE_PCIE_VENDOR_SPEC_DLLP_OFF_OFFS                                                             (GE_PCIE_PCIE20_DBI_4_REG_BASE_OFFS + 0x00000704)
#define HWIO_GE_PCIE_VENDOR_SPEC_DLLP_OFF_RMSK                                                             0xffffffff
#define HWIO_GE_PCIE_VENDOR_SPEC_DLLP_OFF_POR                                                              0x00000000
#define HWIO_GE_PCIE_VENDOR_SPEC_DLLP_OFF_POR_RMSK                                                         0xffffffff
#define HWIO_GE_PCIE_VENDOR_SPEC_DLLP_OFF_ATTR                                                                    0x3
#define HWIO_GE_PCIE_VENDOR_SPEC_DLLP_OFF_IN          \
        in_dword_masked(HWIO_GE_PCIE_VENDOR_SPEC_DLLP_OFF_ADDR, HWIO_GE_PCIE_VENDOR_SPEC_DLLP_OFF_RMSK)
#define HWIO_GE_PCIE_VENDOR_SPEC_DLLP_OFF_INM(m)      \
        in_dword_masked(HWIO_GE_PCIE_VENDOR_SPEC_DLLP_OFF_ADDR, m)
#define HWIO_GE_PCIE_VENDOR_SPEC_DLLP_OFF_OUT(v)      \
        out_dword(HWIO_GE_PCIE_VENDOR_SPEC_DLLP_OFF_ADDR,v)
#define HWIO_GE_PCIE_VENDOR_SPEC_DLLP_OFF_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GE_PCIE_VENDOR_SPEC_DLLP_OFF_ADDR,m,v,HWIO_GE_PCIE_VENDOR_SPEC_DLLP_OFF_IN)
#define HWIO_GE_PCIE_VENDOR_SPEC_DLLP_OFF_VENDOR_SPEC_DLLP_BMSK                                            0xffffffff
#define HWIO_GE_PCIE_VENDOR_SPEC_DLLP_OFF_VENDOR_SPEC_DLLP_SHFT                                                   0x0

#define HWIO_GE_PCIE_PORT_FORCE_OFF_ADDR                                                                   (GE_PCIE_PCIE20_DBI_4_REG_BASE      + 0x00000708)
#define HWIO_GE_PCIE_PORT_FORCE_OFF_PHYS                                                                   (GE_PCIE_PCIE20_DBI_4_REG_BASE_PHYS + 0x00000708)
#define HWIO_GE_PCIE_PORT_FORCE_OFF_OFFS                                                                   (GE_PCIE_PCIE20_DBI_4_REG_BASE_OFFS + 0x00000708)
#define HWIO_GE_PCIE_PORT_FORCE_OFF_RMSK                                                                   0xffffffff
#define HWIO_GE_PCIE_PORT_FORCE_OFF_POR                                                                    0x00000000
#define HWIO_GE_PCIE_PORT_FORCE_OFF_POR_RMSK                                                               0xffffffff
#define HWIO_GE_PCIE_PORT_FORCE_OFF_ATTR                                                                          0x3
#define HWIO_GE_PCIE_PORT_FORCE_OFF_IN          \
        in_dword_masked(HWIO_GE_PCIE_PORT_FORCE_OFF_ADDR, HWIO_GE_PCIE_PORT_FORCE_OFF_RMSK)
#define HWIO_GE_PCIE_PORT_FORCE_OFF_INM(m)      \
        in_dword_masked(HWIO_GE_PCIE_PORT_FORCE_OFF_ADDR, m)
#define HWIO_GE_PCIE_PORT_FORCE_OFF_OUT(v)      \
        out_dword(HWIO_GE_PCIE_PORT_FORCE_OFF_ADDR,v)
#define HWIO_GE_PCIE_PORT_FORCE_OFF_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GE_PCIE_PORT_FORCE_OFF_ADDR,m,v,HWIO_GE_PCIE_PORT_FORCE_OFF_IN)
#define HWIO_GE_PCIE_PORT_FORCE_OFF_CPL_SENT_COUNT_BMSK                                                    0xff000000
#define HWIO_GE_PCIE_PORT_FORCE_OFF_CPL_SENT_COUNT_SHFT                                                          0x18
#define HWIO_GE_PCIE_PORT_FORCE_OFF_RSVDP_22_BMSK                                                            0xc00000
#define HWIO_GE_PCIE_PORT_FORCE_OFF_RSVDP_22_SHFT                                                                0x16
#define HWIO_GE_PCIE_PORT_FORCE_OFF_LINK_STATE_BMSK                                                          0x3f0000
#define HWIO_GE_PCIE_PORT_FORCE_OFF_LINK_STATE_SHFT                                                              0x10
#define HWIO_GE_PCIE_PORT_FORCE_OFF_FORCE_EN_BMSK                                                              0x8000
#define HWIO_GE_PCIE_PORT_FORCE_OFF_FORCE_EN_SHFT                                                                 0xf
#define HWIO_GE_PCIE_PORT_FORCE_OFF_RSVDP_12_BMSK                                                              0x7000
#define HWIO_GE_PCIE_PORT_FORCE_OFF_RSVDP_12_SHFT                                                                 0xc
#define HWIO_GE_PCIE_PORT_FORCE_OFF_FORCED_LTSSM_BMSK                                                           0xf00
#define HWIO_GE_PCIE_PORT_FORCE_OFF_FORCED_LTSSM_SHFT                                                             0x8
#define HWIO_GE_PCIE_PORT_FORCE_OFF_LINK_NUM_BMSK                                                                0xff
#define HWIO_GE_PCIE_PORT_FORCE_OFF_LINK_NUM_SHFT                                                                 0x0

#define HWIO_GE_PCIE_ACK_F_ASPM_CTRL_OFF_ADDR                                                              (GE_PCIE_PCIE20_DBI_4_REG_BASE      + 0x0000070c)
#define HWIO_GE_PCIE_ACK_F_ASPM_CTRL_OFF_PHYS                                                              (GE_PCIE_PCIE20_DBI_4_REG_BASE_PHYS + 0x0000070c)
#define HWIO_GE_PCIE_ACK_F_ASPM_CTRL_OFF_OFFS                                                              (GE_PCIE_PCIE20_DBI_4_REG_BASE_OFFS + 0x0000070c)
#define HWIO_GE_PCIE_ACK_F_ASPM_CTRL_OFF_RMSK                                                              0xffffffff
#define HWIO_GE_PCIE_ACK_F_ASPM_CTRL_OFF_POR                                                               0x00000000
#define HWIO_GE_PCIE_ACK_F_ASPM_CTRL_OFF_POR_RMSK                                                          0xffffffff
#define HWIO_GE_PCIE_ACK_F_ASPM_CTRL_OFF_ATTR                                                                     0x3
#define HWIO_GE_PCIE_ACK_F_ASPM_CTRL_OFF_IN          \
        in_dword_masked(HWIO_GE_PCIE_ACK_F_ASPM_CTRL_OFF_ADDR, HWIO_GE_PCIE_ACK_F_ASPM_CTRL_OFF_RMSK)
#define HWIO_GE_PCIE_ACK_F_ASPM_CTRL_OFF_INM(m)      \
        in_dword_masked(HWIO_GE_PCIE_ACK_F_ASPM_CTRL_OFF_ADDR, m)
#define HWIO_GE_PCIE_ACK_F_ASPM_CTRL_OFF_OUT(v)      \
        out_dword(HWIO_GE_PCIE_ACK_F_ASPM_CTRL_OFF_ADDR,v)
#define HWIO_GE_PCIE_ACK_F_ASPM_CTRL_OFF_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GE_PCIE_ACK_F_ASPM_CTRL_OFF_ADDR,m,v,HWIO_GE_PCIE_ACK_F_ASPM_CTRL_OFF_IN)
#define HWIO_GE_PCIE_ACK_F_ASPM_CTRL_OFF_RSVDP_31_BMSK                                                     0x80000000
#define HWIO_GE_PCIE_ACK_F_ASPM_CTRL_OFF_RSVDP_31_SHFT                                                           0x1f
#define HWIO_GE_PCIE_ACK_F_ASPM_CTRL_OFF_ENTER_ASPM_BMSK                                                   0x40000000
#define HWIO_GE_PCIE_ACK_F_ASPM_CTRL_OFF_ENTER_ASPM_SHFT                                                         0x1e
#define HWIO_GE_PCIE_ACK_F_ASPM_CTRL_OFF_L1_ENTRANCE_LATENCY_BMSK                                          0x38000000
#define HWIO_GE_PCIE_ACK_F_ASPM_CTRL_OFF_L1_ENTRANCE_LATENCY_SHFT                                                0x1b
#define HWIO_GE_PCIE_ACK_F_ASPM_CTRL_OFF_L0S_ENTRANCE_LATENCY_BMSK                                          0x7000000
#define HWIO_GE_PCIE_ACK_F_ASPM_CTRL_OFF_L0S_ENTRANCE_LATENCY_SHFT                                               0x18
#define HWIO_GE_PCIE_ACK_F_ASPM_CTRL_OFF_COMMON_CLK_N_FTS_BMSK                                               0xff0000
#define HWIO_GE_PCIE_ACK_F_ASPM_CTRL_OFF_COMMON_CLK_N_FTS_SHFT                                                   0x10
#define HWIO_GE_PCIE_ACK_F_ASPM_CTRL_OFF_ACK_N_FTS_BMSK                                                        0xff00
#define HWIO_GE_PCIE_ACK_F_ASPM_CTRL_OFF_ACK_N_FTS_SHFT                                                           0x8
#define HWIO_GE_PCIE_ACK_F_ASPM_CTRL_OFF_ACK_FREQ_BMSK                                                           0xff
#define HWIO_GE_PCIE_ACK_F_ASPM_CTRL_OFF_ACK_FREQ_SHFT                                                            0x0

#define HWIO_GE_PCIE_PORT_LINK_CTRL_OFF_ADDR                                                               (GE_PCIE_PCIE20_DBI_4_REG_BASE      + 0x00000710)
#define HWIO_GE_PCIE_PORT_LINK_CTRL_OFF_PHYS                                                               (GE_PCIE_PCIE20_DBI_4_REG_BASE_PHYS + 0x00000710)
#define HWIO_GE_PCIE_PORT_LINK_CTRL_OFF_OFFS                                                               (GE_PCIE_PCIE20_DBI_4_REG_BASE_OFFS + 0x00000710)
#define HWIO_GE_PCIE_PORT_LINK_CTRL_OFF_RMSK                                                               0xff3fffff
#define HWIO_GE_PCIE_PORT_LINK_CTRL_OFF_POR                                                                0x00000000
#define HWIO_GE_PCIE_PORT_LINK_CTRL_OFF_POR_RMSK                                                           0xffffffff
#define HWIO_GE_PCIE_PORT_LINK_CTRL_OFF_ATTR                                                                      0x3
#define HWIO_GE_PCIE_PORT_LINK_CTRL_OFF_IN          \
        in_dword_masked(HWIO_GE_PCIE_PORT_LINK_CTRL_OFF_ADDR, HWIO_GE_PCIE_PORT_LINK_CTRL_OFF_RMSK)
#define HWIO_GE_PCIE_PORT_LINK_CTRL_OFF_INM(m)      \
        in_dword_masked(HWIO_GE_PCIE_PORT_LINK_CTRL_OFF_ADDR, m)
#define HWIO_GE_PCIE_PORT_LINK_CTRL_OFF_OUT(v)      \
        out_dword(HWIO_GE_PCIE_PORT_LINK_CTRL_OFF_ADDR,v)
#define HWIO_GE_PCIE_PORT_LINK_CTRL_OFF_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GE_PCIE_PORT_LINK_CTRL_OFF_ADDR,m,v,HWIO_GE_PCIE_PORT_LINK_CTRL_OFF_IN)
#define HWIO_GE_PCIE_PORT_LINK_CTRL_OFF_RSVDP_28_BMSK                                                      0xf0000000
#define HWIO_GE_PCIE_PORT_LINK_CTRL_OFF_RSVDP_28_SHFT                                                            0x1c
#define HWIO_GE_PCIE_PORT_LINK_CTRL_OFF_TRANSMIT_LANE_REVERSALE_ENABLE_BMSK                                 0x8000000
#define HWIO_GE_PCIE_PORT_LINK_CTRL_OFF_TRANSMIT_LANE_REVERSALE_ENABLE_SHFT                                      0x1b
#define HWIO_GE_PCIE_PORT_LINK_CTRL_OFF_EXTENDED_SYNCH_BMSK                                                 0x4000000
#define HWIO_GE_PCIE_PORT_LINK_CTRL_OFF_EXTENDED_SYNCH_SHFT                                                      0x1a
#define HWIO_GE_PCIE_PORT_LINK_CTRL_OFF_CORRUPT_LCRC_ENABLE_BMSK                                            0x2000000
#define HWIO_GE_PCIE_PORT_LINK_CTRL_OFF_CORRUPT_LCRC_ENABLE_SHFT                                                 0x19
#define HWIO_GE_PCIE_PORT_LINK_CTRL_OFF_BEACON_ENABLE_BMSK                                                  0x1000000
#define HWIO_GE_PCIE_PORT_LINK_CTRL_OFF_BEACON_ENABLE_SHFT                                                       0x18
#define HWIO_GE_PCIE_PORT_LINK_CTRL_OFF_LINK_CAPABLE_BMSK                                                    0x3f0000
#define HWIO_GE_PCIE_PORT_LINK_CTRL_OFF_LINK_CAPABLE_SHFT                                                        0x10
#define HWIO_GE_PCIE_PORT_LINK_CTRL_OFF_RSVDP_12_BMSK                                                          0xf000
#define HWIO_GE_PCIE_PORT_LINK_CTRL_OFF_RSVDP_12_SHFT                                                             0xc
#define HWIO_GE_PCIE_PORT_LINK_CTRL_OFF_LINK_RATE_BMSK                                                          0xf00
#define HWIO_GE_PCIE_PORT_LINK_CTRL_OFF_LINK_RATE_SHFT                                                            0x8
#define HWIO_GE_PCIE_PORT_LINK_CTRL_OFF_FAST_LINK_MODE_BMSK                                                      0x80
#define HWIO_GE_PCIE_PORT_LINK_CTRL_OFF_FAST_LINK_MODE_SHFT                                                       0x7
#define HWIO_GE_PCIE_PORT_LINK_CTRL_OFF_LINK_DISABLE_BMSK                                                        0x40
#define HWIO_GE_PCIE_PORT_LINK_CTRL_OFF_LINK_DISABLE_SHFT                                                         0x6
#define HWIO_GE_PCIE_PORT_LINK_CTRL_OFF_DLL_LINK_EN_BMSK                                                         0x20
#define HWIO_GE_PCIE_PORT_LINK_CTRL_OFF_DLL_LINK_EN_SHFT                                                          0x5
#define HWIO_GE_PCIE_PORT_LINK_CTRL_OFF_RSVDP_4_BMSK                                                             0x10
#define HWIO_GE_PCIE_PORT_LINK_CTRL_OFF_RSVDP_4_SHFT                                                              0x4
#define HWIO_GE_PCIE_PORT_LINK_CTRL_OFF_RESET_ASSERT_BMSK                                                         0x8
#define HWIO_GE_PCIE_PORT_LINK_CTRL_OFF_RESET_ASSERT_SHFT                                                         0x3
#define HWIO_GE_PCIE_PORT_LINK_CTRL_OFF_LOOPBACK_ENABLE_BMSK                                                      0x4
#define HWIO_GE_PCIE_PORT_LINK_CTRL_OFF_LOOPBACK_ENABLE_SHFT                                                      0x2
#define HWIO_GE_PCIE_PORT_LINK_CTRL_OFF_SCRAMBLE_DISABLE_BMSK                                                     0x2
#define HWIO_GE_PCIE_PORT_LINK_CTRL_OFF_SCRAMBLE_DISABLE_SHFT                                                     0x1
#define HWIO_GE_PCIE_PORT_LINK_CTRL_OFF_VENDOR_SPECIFIC_DLLP_REQ_BMSK                                             0x1
#define HWIO_GE_PCIE_PORT_LINK_CTRL_OFF_VENDOR_SPECIFIC_DLLP_REQ_SHFT                                             0x0

#define HWIO_GE_PCIE_LANE_SKEW_OFF_ADDR                                                                    (GE_PCIE_PCIE20_DBI_4_REG_BASE      + 0x00000714)
#define HWIO_GE_PCIE_LANE_SKEW_OFF_PHYS                                                                    (GE_PCIE_PCIE20_DBI_4_REG_BASE_PHYS + 0x00000714)
#define HWIO_GE_PCIE_LANE_SKEW_OFF_OFFS                                                                    (GE_PCIE_PCIE20_DBI_4_REG_BASE_OFFS + 0x00000714)
#define HWIO_GE_PCIE_LANE_SKEW_OFF_RMSK                                                                    0xffffffff
#define HWIO_GE_PCIE_LANE_SKEW_OFF_POR                                                                     0x00000000
#define HWIO_GE_PCIE_LANE_SKEW_OFF_POR_RMSK                                                                0xffffffff
#define HWIO_GE_PCIE_LANE_SKEW_OFF_ATTR                                                                           0x3
#define HWIO_GE_PCIE_LANE_SKEW_OFF_IN          \
        in_dword_masked(HWIO_GE_PCIE_LANE_SKEW_OFF_ADDR, HWIO_GE_PCIE_LANE_SKEW_OFF_RMSK)
#define HWIO_GE_PCIE_LANE_SKEW_OFF_INM(m)      \
        in_dword_masked(HWIO_GE_PCIE_LANE_SKEW_OFF_ADDR, m)
#define HWIO_GE_PCIE_LANE_SKEW_OFF_OUT(v)      \
        out_dword(HWIO_GE_PCIE_LANE_SKEW_OFF_ADDR,v)
#define HWIO_GE_PCIE_LANE_SKEW_OFF_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GE_PCIE_LANE_SKEW_OFF_ADDR,m,v,HWIO_GE_PCIE_LANE_SKEW_OFF_IN)
#define HWIO_GE_PCIE_LANE_SKEW_OFF_DISABLE_LANE_TO_LANE_DESKEW_BMSK                                        0x80000000
#define HWIO_GE_PCIE_LANE_SKEW_OFF_DISABLE_LANE_TO_LANE_DESKEW_SHFT                                              0x1f
#define HWIO_GE_PCIE_LANE_SKEW_OFF_RSVDP_26_BMSK                                                           0x7c000000
#define HWIO_GE_PCIE_LANE_SKEW_OFF_RSVDP_26_SHFT                                                                 0x1a
#define HWIO_GE_PCIE_LANE_SKEW_OFF_ACK_NAK_DISABLE_BMSK                                                     0x2000000
#define HWIO_GE_PCIE_LANE_SKEW_OFF_ACK_NAK_DISABLE_SHFT                                                          0x19
#define HWIO_GE_PCIE_LANE_SKEW_OFF_FLOW_CTRL_DISABLE_BMSK                                                   0x1000000
#define HWIO_GE_PCIE_LANE_SKEW_OFF_FLOW_CTRL_DISABLE_SHFT                                                        0x18
#define HWIO_GE_PCIE_LANE_SKEW_OFF_INSERT_LANE_SKEW_BMSK                                                     0xffffff
#define HWIO_GE_PCIE_LANE_SKEW_OFF_INSERT_LANE_SKEW_SHFT                                                          0x0

#define HWIO_GE_PCIE_TIMER_CTRL_MAX_FUNC_NUM_OFF_ADDR                                                      (GE_PCIE_PCIE20_DBI_4_REG_BASE      + 0x00000718)
#define HWIO_GE_PCIE_TIMER_CTRL_MAX_FUNC_NUM_OFF_PHYS                                                      (GE_PCIE_PCIE20_DBI_4_REG_BASE_PHYS + 0x00000718)
#define HWIO_GE_PCIE_TIMER_CTRL_MAX_FUNC_NUM_OFF_OFFS                                                      (GE_PCIE_PCIE20_DBI_4_REG_BASE_OFFS + 0x00000718)
#define HWIO_GE_PCIE_TIMER_CTRL_MAX_FUNC_NUM_OFF_RMSK                                                      0xffffffff
#define HWIO_GE_PCIE_TIMER_CTRL_MAX_FUNC_NUM_OFF_POR                                                       0x00000000
#define HWIO_GE_PCIE_TIMER_CTRL_MAX_FUNC_NUM_OFF_POR_RMSK                                                  0xffffffff
#define HWIO_GE_PCIE_TIMER_CTRL_MAX_FUNC_NUM_OFF_ATTR                                                             0x3
#define HWIO_GE_PCIE_TIMER_CTRL_MAX_FUNC_NUM_OFF_IN          \
        in_dword_masked(HWIO_GE_PCIE_TIMER_CTRL_MAX_FUNC_NUM_OFF_ADDR, HWIO_GE_PCIE_TIMER_CTRL_MAX_FUNC_NUM_OFF_RMSK)
#define HWIO_GE_PCIE_TIMER_CTRL_MAX_FUNC_NUM_OFF_INM(m)      \
        in_dword_masked(HWIO_GE_PCIE_TIMER_CTRL_MAX_FUNC_NUM_OFF_ADDR, m)
#define HWIO_GE_PCIE_TIMER_CTRL_MAX_FUNC_NUM_OFF_OUT(v)      \
        out_dword(HWIO_GE_PCIE_TIMER_CTRL_MAX_FUNC_NUM_OFF_ADDR,v)
#define HWIO_GE_PCIE_TIMER_CTRL_MAX_FUNC_NUM_OFF_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GE_PCIE_TIMER_CTRL_MAX_FUNC_NUM_OFF_ADDR,m,v,HWIO_GE_PCIE_TIMER_CTRL_MAX_FUNC_NUM_OFF_IN)
#define HWIO_GE_PCIE_TIMER_CTRL_MAX_FUNC_NUM_OFF_RSVDP_29_BMSK                                             0xe0000000
#define HWIO_GE_PCIE_TIMER_CTRL_MAX_FUNC_NUM_OFF_RSVDP_29_SHFT                                                   0x1d
#define HWIO_GE_PCIE_TIMER_CTRL_MAX_FUNC_NUM_OFF_UPDATE_FREQ_TIMER_BMSK                                    0x1f000000
#define HWIO_GE_PCIE_TIMER_CTRL_MAX_FUNC_NUM_OFF_UPDATE_FREQ_TIMER_SHFT                                          0x18
#define HWIO_GE_PCIE_TIMER_CTRL_MAX_FUNC_NUM_OFF_TIMER_MOD_ACK_NAK_BMSK                                      0xf80000
#define HWIO_GE_PCIE_TIMER_CTRL_MAX_FUNC_NUM_OFF_TIMER_MOD_ACK_NAK_SHFT                                          0x13
#define HWIO_GE_PCIE_TIMER_CTRL_MAX_FUNC_NUM_OFF_TIMER_MOD_REPLAY_TIMER_BMSK                                  0x7c000
#define HWIO_GE_PCIE_TIMER_CTRL_MAX_FUNC_NUM_OFF_TIMER_MOD_REPLAY_TIMER_SHFT                                      0xe
#define HWIO_GE_PCIE_TIMER_CTRL_MAX_FUNC_NUM_OFF_RSVDP_8_BMSK                                                  0x3f00
#define HWIO_GE_PCIE_TIMER_CTRL_MAX_FUNC_NUM_OFF_RSVDP_8_SHFT                                                     0x8
#define HWIO_GE_PCIE_TIMER_CTRL_MAX_FUNC_NUM_OFF_MAX_FUNC_NUM_BMSK                                               0xff
#define HWIO_GE_PCIE_TIMER_CTRL_MAX_FUNC_NUM_OFF_MAX_FUNC_NUM_SHFT                                                0x0

#define HWIO_GE_PCIE_SYMBOL_TIMER_FILTER_1_OFF_ADDR                                                        (GE_PCIE_PCIE20_DBI_4_REG_BASE      + 0x0000071c)
#define HWIO_GE_PCIE_SYMBOL_TIMER_FILTER_1_OFF_PHYS                                                        (GE_PCIE_PCIE20_DBI_4_REG_BASE_PHYS + 0x0000071c)
#define HWIO_GE_PCIE_SYMBOL_TIMER_FILTER_1_OFF_OFFS                                                        (GE_PCIE_PCIE20_DBI_4_REG_BASE_OFFS + 0x0000071c)
#define HWIO_GE_PCIE_SYMBOL_TIMER_FILTER_1_OFF_RMSK                                                        0xffffffff
#define HWIO_GE_PCIE_SYMBOL_TIMER_FILTER_1_OFF_POR                                                         0x00000000
#define HWIO_GE_PCIE_SYMBOL_TIMER_FILTER_1_OFF_POR_RMSK                                                    0xffffffff
#define HWIO_GE_PCIE_SYMBOL_TIMER_FILTER_1_OFF_ATTR                                                               0x3
#define HWIO_GE_PCIE_SYMBOL_TIMER_FILTER_1_OFF_IN          \
        in_dword_masked(HWIO_GE_PCIE_SYMBOL_TIMER_FILTER_1_OFF_ADDR, HWIO_GE_PCIE_SYMBOL_TIMER_FILTER_1_OFF_RMSK)
#define HWIO_GE_PCIE_SYMBOL_TIMER_FILTER_1_OFF_INM(m)      \
        in_dword_masked(HWIO_GE_PCIE_SYMBOL_TIMER_FILTER_1_OFF_ADDR, m)
#define HWIO_GE_PCIE_SYMBOL_TIMER_FILTER_1_OFF_OUT(v)      \
        out_dword(HWIO_GE_PCIE_SYMBOL_TIMER_FILTER_1_OFF_ADDR,v)
#define HWIO_GE_PCIE_SYMBOL_TIMER_FILTER_1_OFF_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GE_PCIE_SYMBOL_TIMER_FILTER_1_OFF_ADDR,m,v,HWIO_GE_PCIE_SYMBOL_TIMER_FILTER_1_OFF_IN)
#define HWIO_GE_PCIE_SYMBOL_TIMER_FILTER_1_OFF_MASK_RADM_1_BMSK                                            0xffff0000
#define HWIO_GE_PCIE_SYMBOL_TIMER_FILTER_1_OFF_MASK_RADM_1_SHFT                                                  0x10
#define HWIO_GE_PCIE_SYMBOL_TIMER_FILTER_1_OFF_DISABLE_FC_WD_TIMER_BMSK                                        0x8000
#define HWIO_GE_PCIE_SYMBOL_TIMER_FILTER_1_OFF_DISABLE_FC_WD_TIMER_SHFT                                           0xf
#define HWIO_GE_PCIE_SYMBOL_TIMER_FILTER_1_OFF_EIDLE_TIMER_BMSK                                                0x7800
#define HWIO_GE_PCIE_SYMBOL_TIMER_FILTER_1_OFF_EIDLE_TIMER_SHFT                                                   0xb
#define HWIO_GE_PCIE_SYMBOL_TIMER_FILTER_1_OFF_SKP_INT_VAL_BMSK                                                 0x7ff
#define HWIO_GE_PCIE_SYMBOL_TIMER_FILTER_1_OFF_SKP_INT_VAL_SHFT                                                   0x0

#define HWIO_GE_PCIE_FILTER_MASK_2_OFF_ADDR                                                                (GE_PCIE_PCIE20_DBI_4_REG_BASE      + 0x00000720)
#define HWIO_GE_PCIE_FILTER_MASK_2_OFF_PHYS                                                                (GE_PCIE_PCIE20_DBI_4_REG_BASE_PHYS + 0x00000720)
#define HWIO_GE_PCIE_FILTER_MASK_2_OFF_OFFS                                                                (GE_PCIE_PCIE20_DBI_4_REG_BASE_OFFS + 0x00000720)
#define HWIO_GE_PCIE_FILTER_MASK_2_OFF_RMSK                                                                0xffffffff
#define HWIO_GE_PCIE_FILTER_MASK_2_OFF_POR                                                                 0x00000000
#define HWIO_GE_PCIE_FILTER_MASK_2_OFF_POR_RMSK                                                            0xffffffff
#define HWIO_GE_PCIE_FILTER_MASK_2_OFF_ATTR                                                                       0x3
#define HWIO_GE_PCIE_FILTER_MASK_2_OFF_IN          \
        in_dword_masked(HWIO_GE_PCIE_FILTER_MASK_2_OFF_ADDR, HWIO_GE_PCIE_FILTER_MASK_2_OFF_RMSK)
#define HWIO_GE_PCIE_FILTER_MASK_2_OFF_INM(m)      \
        in_dword_masked(HWIO_GE_PCIE_FILTER_MASK_2_OFF_ADDR, m)
#define HWIO_GE_PCIE_FILTER_MASK_2_OFF_OUT(v)      \
        out_dword(HWIO_GE_PCIE_FILTER_MASK_2_OFF_ADDR,v)
#define HWIO_GE_PCIE_FILTER_MASK_2_OFF_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GE_PCIE_FILTER_MASK_2_OFF_ADDR,m,v,HWIO_GE_PCIE_FILTER_MASK_2_OFF_IN)
#define HWIO_GE_PCIE_FILTER_MASK_2_OFF_MASK_RADM_2_BMSK                                                    0xffffffff
#define HWIO_GE_PCIE_FILTER_MASK_2_OFF_MASK_RADM_2_SHFT                                                           0x0

#define HWIO_GE_PCIE_AMBA_MUL_OB_DECOMP_NP_SUB_REQ_CTRL_OFF_ADDR                                           (GE_PCIE_PCIE20_DBI_4_REG_BASE      + 0x00000724)
#define HWIO_GE_PCIE_AMBA_MUL_OB_DECOMP_NP_SUB_REQ_CTRL_OFF_PHYS                                           (GE_PCIE_PCIE20_DBI_4_REG_BASE_PHYS + 0x00000724)
#define HWIO_GE_PCIE_AMBA_MUL_OB_DECOMP_NP_SUB_REQ_CTRL_OFF_OFFS                                           (GE_PCIE_PCIE20_DBI_4_REG_BASE_OFFS + 0x00000724)
#define HWIO_GE_PCIE_AMBA_MUL_OB_DECOMP_NP_SUB_REQ_CTRL_OFF_RMSK                                           0xffffffff
#define HWIO_GE_PCIE_AMBA_MUL_OB_DECOMP_NP_SUB_REQ_CTRL_OFF_POR                                            0x00000000
#define HWIO_GE_PCIE_AMBA_MUL_OB_DECOMP_NP_SUB_REQ_CTRL_OFF_POR_RMSK                                       0xffffffff
#define HWIO_GE_PCIE_AMBA_MUL_OB_DECOMP_NP_SUB_REQ_CTRL_OFF_ATTR                                                  0x3
#define HWIO_GE_PCIE_AMBA_MUL_OB_DECOMP_NP_SUB_REQ_CTRL_OFF_IN          \
        in_dword_masked(HWIO_GE_PCIE_AMBA_MUL_OB_DECOMP_NP_SUB_REQ_CTRL_OFF_ADDR, HWIO_GE_PCIE_AMBA_MUL_OB_DECOMP_NP_SUB_REQ_CTRL_OFF_RMSK)
#define HWIO_GE_PCIE_AMBA_MUL_OB_DECOMP_NP_SUB_REQ_CTRL_OFF_INM(m)      \
        in_dword_masked(HWIO_GE_PCIE_AMBA_MUL_OB_DECOMP_NP_SUB_REQ_CTRL_OFF_ADDR, m)
#define HWIO_GE_PCIE_AMBA_MUL_OB_DECOMP_NP_SUB_REQ_CTRL_OFF_OUT(v)      \
        out_dword(HWIO_GE_PCIE_AMBA_MUL_OB_DECOMP_NP_SUB_REQ_CTRL_OFF_ADDR,v)
#define HWIO_GE_PCIE_AMBA_MUL_OB_DECOMP_NP_SUB_REQ_CTRL_OFF_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GE_PCIE_AMBA_MUL_OB_DECOMP_NP_SUB_REQ_CTRL_OFF_ADDR,m,v,HWIO_GE_PCIE_AMBA_MUL_OB_DECOMP_NP_SUB_REQ_CTRL_OFF_IN)
#define HWIO_GE_PCIE_AMBA_MUL_OB_DECOMP_NP_SUB_REQ_CTRL_OFF_RSVDP_1_BMSK                                   0xfffffffe
#define HWIO_GE_PCIE_AMBA_MUL_OB_DECOMP_NP_SUB_REQ_CTRL_OFF_RSVDP_1_SHFT                                          0x1
#define HWIO_GE_PCIE_AMBA_MUL_OB_DECOMP_NP_SUB_REQ_CTRL_OFF_OB_RD_SPLIT_BURST_EN_BMSK                             0x1
#define HWIO_GE_PCIE_AMBA_MUL_OB_DECOMP_NP_SUB_REQ_CTRL_OFF_OB_RD_SPLIT_BURST_EN_SHFT                             0x0

#define HWIO_GE_PCIE_PL_DEBUG0_OFF_ADDR                                                                    (GE_PCIE_PCIE20_DBI_4_REG_BASE      + 0x00000728)
#define HWIO_GE_PCIE_PL_DEBUG0_OFF_PHYS                                                                    (GE_PCIE_PCIE20_DBI_4_REG_BASE_PHYS + 0x00000728)
#define HWIO_GE_PCIE_PL_DEBUG0_OFF_OFFS                                                                    (GE_PCIE_PCIE20_DBI_4_REG_BASE_OFFS + 0x00000728)
#define HWIO_GE_PCIE_PL_DEBUG0_OFF_RMSK                                                                    0xffffffff
#define HWIO_GE_PCIE_PL_DEBUG0_OFF_POR                                                                     0x00000000
#define HWIO_GE_PCIE_PL_DEBUG0_OFF_POR_RMSK                                                                0xffffffff
#define HWIO_GE_PCIE_PL_DEBUG0_OFF_ATTR                                                                           0x1
#define HWIO_GE_PCIE_PL_DEBUG0_OFF_IN          \
        in_dword_masked(HWIO_GE_PCIE_PL_DEBUG0_OFF_ADDR, HWIO_GE_PCIE_PL_DEBUG0_OFF_RMSK)
#define HWIO_GE_PCIE_PL_DEBUG0_OFF_INM(m)      \
        in_dword_masked(HWIO_GE_PCIE_PL_DEBUG0_OFF_ADDR, m)
#define HWIO_GE_PCIE_PL_DEBUG0_OFF_DEB_REG_0_BMSK                                                          0xffffffff
#define HWIO_GE_PCIE_PL_DEBUG0_OFF_DEB_REG_0_SHFT                                                                 0x0

#define HWIO_GE_PCIE_PL_DEBUG1_OFF_ADDR                                                                    (GE_PCIE_PCIE20_DBI_4_REG_BASE      + 0x0000072c)
#define HWIO_GE_PCIE_PL_DEBUG1_OFF_PHYS                                                                    (GE_PCIE_PCIE20_DBI_4_REG_BASE_PHYS + 0x0000072c)
#define HWIO_GE_PCIE_PL_DEBUG1_OFF_OFFS                                                                    (GE_PCIE_PCIE20_DBI_4_REG_BASE_OFFS + 0x0000072c)
#define HWIO_GE_PCIE_PL_DEBUG1_OFF_RMSK                                                                    0xffffffff
#define HWIO_GE_PCIE_PL_DEBUG1_OFF_POR                                                                     0x00000000
#define HWIO_GE_PCIE_PL_DEBUG1_OFF_POR_RMSK                                                                0xffffffff
#define HWIO_GE_PCIE_PL_DEBUG1_OFF_ATTR                                                                           0x1
#define HWIO_GE_PCIE_PL_DEBUG1_OFF_IN          \
        in_dword_masked(HWIO_GE_PCIE_PL_DEBUG1_OFF_ADDR, HWIO_GE_PCIE_PL_DEBUG1_OFF_RMSK)
#define HWIO_GE_PCIE_PL_DEBUG1_OFF_INM(m)      \
        in_dword_masked(HWIO_GE_PCIE_PL_DEBUG1_OFF_ADDR, m)
#define HWIO_GE_PCIE_PL_DEBUG1_OFF_DEB_REG_1_BMSK                                                          0xffffffff
#define HWIO_GE_PCIE_PL_DEBUG1_OFF_DEB_REG_1_SHFT                                                                 0x0

#define HWIO_GE_PCIE_TX_P_FC_CREDIT_STATUS_OFF_ADDR                                                        (GE_PCIE_PCIE20_DBI_4_REG_BASE      + 0x00000730)
#define HWIO_GE_PCIE_TX_P_FC_CREDIT_STATUS_OFF_PHYS                                                        (GE_PCIE_PCIE20_DBI_4_REG_BASE_PHYS + 0x00000730)
#define HWIO_GE_PCIE_TX_P_FC_CREDIT_STATUS_OFF_OFFS                                                        (GE_PCIE_PCIE20_DBI_4_REG_BASE_OFFS + 0x00000730)
#define HWIO_GE_PCIE_TX_P_FC_CREDIT_STATUS_OFF_RMSK                                                        0xffffffff
#define HWIO_GE_PCIE_TX_P_FC_CREDIT_STATUS_OFF_POR                                                         0x00000000
#define HWIO_GE_PCIE_TX_P_FC_CREDIT_STATUS_OFF_POR_RMSK                                                    0xffffffff
#define HWIO_GE_PCIE_TX_P_FC_CREDIT_STATUS_OFF_ATTR                                                               0x1
#define HWIO_GE_PCIE_TX_P_FC_CREDIT_STATUS_OFF_IN          \
        in_dword_masked(HWIO_GE_PCIE_TX_P_FC_CREDIT_STATUS_OFF_ADDR, HWIO_GE_PCIE_TX_P_FC_CREDIT_STATUS_OFF_RMSK)
#define HWIO_GE_PCIE_TX_P_FC_CREDIT_STATUS_OFF_INM(m)      \
        in_dword_masked(HWIO_GE_PCIE_TX_P_FC_CREDIT_STATUS_OFF_ADDR, m)
#define HWIO_GE_PCIE_TX_P_FC_CREDIT_STATUS_OFF_RSVDP_20_BMSK                                               0xfff00000
#define HWIO_GE_PCIE_TX_P_FC_CREDIT_STATUS_OFF_RSVDP_20_SHFT                                                     0x14
#define HWIO_GE_PCIE_TX_P_FC_CREDIT_STATUS_OFF_TX_P_HEADER_FC_CREDIT_BMSK                                     0xff000
#define HWIO_GE_PCIE_TX_P_FC_CREDIT_STATUS_OFF_TX_P_HEADER_FC_CREDIT_SHFT                                         0xc
#define HWIO_GE_PCIE_TX_P_FC_CREDIT_STATUS_OFF_TX_P_DATA_FC_CREDIT_BMSK                                         0xfff
#define HWIO_GE_PCIE_TX_P_FC_CREDIT_STATUS_OFF_TX_P_DATA_FC_CREDIT_SHFT                                           0x0

#define HWIO_GE_PCIE_TX_NP_FC_CREDIT_STATUS_OFF_ADDR                                                       (GE_PCIE_PCIE20_DBI_4_REG_BASE      + 0x00000734)
#define HWIO_GE_PCIE_TX_NP_FC_CREDIT_STATUS_OFF_PHYS                                                       (GE_PCIE_PCIE20_DBI_4_REG_BASE_PHYS + 0x00000734)
#define HWIO_GE_PCIE_TX_NP_FC_CREDIT_STATUS_OFF_OFFS                                                       (GE_PCIE_PCIE20_DBI_4_REG_BASE_OFFS + 0x00000734)
#define HWIO_GE_PCIE_TX_NP_FC_CREDIT_STATUS_OFF_RMSK                                                       0xffffffff
#define HWIO_GE_PCIE_TX_NP_FC_CREDIT_STATUS_OFF_POR                                                        0x00000000
#define HWIO_GE_PCIE_TX_NP_FC_CREDIT_STATUS_OFF_POR_RMSK                                                   0xffffffff
#define HWIO_GE_PCIE_TX_NP_FC_CREDIT_STATUS_OFF_ATTR                                                              0x1
#define HWIO_GE_PCIE_TX_NP_FC_CREDIT_STATUS_OFF_IN          \
        in_dword_masked(HWIO_GE_PCIE_TX_NP_FC_CREDIT_STATUS_OFF_ADDR, HWIO_GE_PCIE_TX_NP_FC_CREDIT_STATUS_OFF_RMSK)
#define HWIO_GE_PCIE_TX_NP_FC_CREDIT_STATUS_OFF_INM(m)      \
        in_dword_masked(HWIO_GE_PCIE_TX_NP_FC_CREDIT_STATUS_OFF_ADDR, m)
#define HWIO_GE_PCIE_TX_NP_FC_CREDIT_STATUS_OFF_RSVDP_20_BMSK                                              0xfff00000
#define HWIO_GE_PCIE_TX_NP_FC_CREDIT_STATUS_OFF_RSVDP_20_SHFT                                                    0x14
#define HWIO_GE_PCIE_TX_NP_FC_CREDIT_STATUS_OFF_TX_NP_HEADER_FC_CREDIT_BMSK                                   0xff000
#define HWIO_GE_PCIE_TX_NP_FC_CREDIT_STATUS_OFF_TX_NP_HEADER_FC_CREDIT_SHFT                                       0xc
#define HWIO_GE_PCIE_TX_NP_FC_CREDIT_STATUS_OFF_TX_NP_DATA_FC_CREDIT_BMSK                                       0xfff
#define HWIO_GE_PCIE_TX_NP_FC_CREDIT_STATUS_OFF_TX_NP_DATA_FC_CREDIT_SHFT                                         0x0

#define HWIO_GE_PCIE_TX_CPL_FC_CREDIT_STATUS_OFF_ADDR                                                      (GE_PCIE_PCIE20_DBI_4_REG_BASE      + 0x00000738)
#define HWIO_GE_PCIE_TX_CPL_FC_CREDIT_STATUS_OFF_PHYS                                                      (GE_PCIE_PCIE20_DBI_4_REG_BASE_PHYS + 0x00000738)
#define HWIO_GE_PCIE_TX_CPL_FC_CREDIT_STATUS_OFF_OFFS                                                      (GE_PCIE_PCIE20_DBI_4_REG_BASE_OFFS + 0x00000738)
#define HWIO_GE_PCIE_TX_CPL_FC_CREDIT_STATUS_OFF_RMSK                                                      0xffffffff
#define HWIO_GE_PCIE_TX_CPL_FC_CREDIT_STATUS_OFF_POR                                                       0x00000000
#define HWIO_GE_PCIE_TX_CPL_FC_CREDIT_STATUS_OFF_POR_RMSK                                                  0xffffffff
#define HWIO_GE_PCIE_TX_CPL_FC_CREDIT_STATUS_OFF_ATTR                                                             0x1
#define HWIO_GE_PCIE_TX_CPL_FC_CREDIT_STATUS_OFF_IN          \
        in_dword_masked(HWIO_GE_PCIE_TX_CPL_FC_CREDIT_STATUS_OFF_ADDR, HWIO_GE_PCIE_TX_CPL_FC_CREDIT_STATUS_OFF_RMSK)
#define HWIO_GE_PCIE_TX_CPL_FC_CREDIT_STATUS_OFF_INM(m)      \
        in_dword_masked(HWIO_GE_PCIE_TX_CPL_FC_CREDIT_STATUS_OFF_ADDR, m)
#define HWIO_GE_PCIE_TX_CPL_FC_CREDIT_STATUS_OFF_RSVDP_20_BMSK                                             0xfff00000
#define HWIO_GE_PCIE_TX_CPL_FC_CREDIT_STATUS_OFF_RSVDP_20_SHFT                                                   0x14
#define HWIO_GE_PCIE_TX_CPL_FC_CREDIT_STATUS_OFF_TX_CPL_HEADER_FC_CREDIT_BMSK                                 0xff000
#define HWIO_GE_PCIE_TX_CPL_FC_CREDIT_STATUS_OFF_TX_CPL_HEADER_FC_CREDIT_SHFT                                     0xc
#define HWIO_GE_PCIE_TX_CPL_FC_CREDIT_STATUS_OFF_TX_CPL_DATA_FC_CREDIT_BMSK                                     0xfff
#define HWIO_GE_PCIE_TX_CPL_FC_CREDIT_STATUS_OFF_TX_CPL_DATA_FC_CREDIT_SHFT                                       0x0

#define HWIO_GE_PCIE_QUEUE_STATUS_OFF_ADDR                                                                 (GE_PCIE_PCIE20_DBI_4_REG_BASE      + 0x0000073c)
#define HWIO_GE_PCIE_QUEUE_STATUS_OFF_PHYS                                                                 (GE_PCIE_PCIE20_DBI_4_REG_BASE_PHYS + 0x0000073c)
#define HWIO_GE_PCIE_QUEUE_STATUS_OFF_OFFS                                                                 (GE_PCIE_PCIE20_DBI_4_REG_BASE_OFFS + 0x0000073c)
#define HWIO_GE_PCIE_QUEUE_STATUS_OFF_RMSK                                                                 0xffffffff
#define HWIO_GE_PCIE_QUEUE_STATUS_OFF_POR                                                                  0x00000000
#define HWIO_GE_PCIE_QUEUE_STATUS_OFF_POR_RMSK                                                             0xffffffff
#define HWIO_GE_PCIE_QUEUE_STATUS_OFF_ATTR                                                                        0x3
#define HWIO_GE_PCIE_QUEUE_STATUS_OFF_IN          \
        in_dword_masked(HWIO_GE_PCIE_QUEUE_STATUS_OFF_ADDR, HWIO_GE_PCIE_QUEUE_STATUS_OFF_RMSK)
#define HWIO_GE_PCIE_QUEUE_STATUS_OFF_INM(m)      \
        in_dword_masked(HWIO_GE_PCIE_QUEUE_STATUS_OFF_ADDR, m)
#define HWIO_GE_PCIE_QUEUE_STATUS_OFF_OUT(v)      \
        out_dword(HWIO_GE_PCIE_QUEUE_STATUS_OFF_ADDR,v)
#define HWIO_GE_PCIE_QUEUE_STATUS_OFF_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GE_PCIE_QUEUE_STATUS_OFF_ADDR,m,v,HWIO_GE_PCIE_QUEUE_STATUS_OFF_IN)
#define HWIO_GE_PCIE_QUEUE_STATUS_OFF_TIMER_MOD_FLOW_CONTROL_EN_BMSK                                       0x80000000
#define HWIO_GE_PCIE_QUEUE_STATUS_OFF_TIMER_MOD_FLOW_CONTROL_EN_SHFT                                             0x1f
#define HWIO_GE_PCIE_QUEUE_STATUS_OFF_RSVDP_29_BMSK                                                        0x60000000
#define HWIO_GE_PCIE_QUEUE_STATUS_OFF_RSVDP_29_SHFT                                                              0x1d
#define HWIO_GE_PCIE_QUEUE_STATUS_OFF_TIMER_MOD_FLOW_CONTROL_BMSK                                          0x1fff0000
#define HWIO_GE_PCIE_QUEUE_STATUS_OFF_TIMER_MOD_FLOW_CONTROL_SHFT                                                0x10
#define HWIO_GE_PCIE_QUEUE_STATUS_OFF_RSVDP_3_BMSK                                                             0xfff8
#define HWIO_GE_PCIE_QUEUE_STATUS_OFF_RSVDP_3_SHFT                                                                0x3
#define HWIO_GE_PCIE_QUEUE_STATUS_OFF_RX_QUEUE_NON_EMPTY_BMSK                                                     0x4
#define HWIO_GE_PCIE_QUEUE_STATUS_OFF_RX_QUEUE_NON_EMPTY_SHFT                                                     0x2
#define HWIO_GE_PCIE_QUEUE_STATUS_OFF_TX_RETRY_BUFFER_NE_BMSK                                                     0x2
#define HWIO_GE_PCIE_QUEUE_STATUS_OFF_TX_RETRY_BUFFER_NE_SHFT                                                     0x1
#define HWIO_GE_PCIE_QUEUE_STATUS_OFF_RX_TLP_FC_CREDIT_NON_RETURN_BMSK                                            0x1
#define HWIO_GE_PCIE_QUEUE_STATUS_OFF_RX_TLP_FC_CREDIT_NON_RETURN_SHFT                                            0x0

#define HWIO_GE_PCIE_VC_TX_ARBI_1_OFF_ADDR                                                                 (GE_PCIE_PCIE20_DBI_4_REG_BASE      + 0x00000740)
#define HWIO_GE_PCIE_VC_TX_ARBI_1_OFF_PHYS                                                                 (GE_PCIE_PCIE20_DBI_4_REG_BASE_PHYS + 0x00000740)
#define HWIO_GE_PCIE_VC_TX_ARBI_1_OFF_OFFS                                                                 (GE_PCIE_PCIE20_DBI_4_REG_BASE_OFFS + 0x00000740)
#define HWIO_GE_PCIE_VC_TX_ARBI_1_OFF_RMSK                                                                 0xffffffff
#define HWIO_GE_PCIE_VC_TX_ARBI_1_OFF_POR                                                                  0x00000000
#define HWIO_GE_PCIE_VC_TX_ARBI_1_OFF_POR_RMSK                                                             0xffffffff
#define HWIO_GE_PCIE_VC_TX_ARBI_1_OFF_ATTR                                                                        0x1
#define HWIO_GE_PCIE_VC_TX_ARBI_1_OFF_IN          \
        in_dword_masked(HWIO_GE_PCIE_VC_TX_ARBI_1_OFF_ADDR, HWIO_GE_PCIE_VC_TX_ARBI_1_OFF_RMSK)
#define HWIO_GE_PCIE_VC_TX_ARBI_1_OFF_INM(m)      \
        in_dword_masked(HWIO_GE_PCIE_VC_TX_ARBI_1_OFF_ADDR, m)
#define HWIO_GE_PCIE_VC_TX_ARBI_1_OFF_WRR_WEIGHT_VC_3_BMSK                                                 0xff000000
#define HWIO_GE_PCIE_VC_TX_ARBI_1_OFF_WRR_WEIGHT_VC_3_SHFT                                                       0x18
#define HWIO_GE_PCIE_VC_TX_ARBI_1_OFF_WRR_WEIGHT_VC_2_BMSK                                                   0xff0000
#define HWIO_GE_PCIE_VC_TX_ARBI_1_OFF_WRR_WEIGHT_VC_2_SHFT                                                       0x10
#define HWIO_GE_PCIE_VC_TX_ARBI_1_OFF_WRR_WEIGHT_VC_1_BMSK                                                     0xff00
#define HWIO_GE_PCIE_VC_TX_ARBI_1_OFF_WRR_WEIGHT_VC_1_SHFT                                                        0x8
#define HWIO_GE_PCIE_VC_TX_ARBI_1_OFF_WRR_WEIGHT_VC_0_BMSK                                                       0xff
#define HWIO_GE_PCIE_VC_TX_ARBI_1_OFF_WRR_WEIGHT_VC_0_SHFT                                                        0x0

#define HWIO_GE_PCIE_VC_TX_ARBI_2_OFF_ADDR                                                                 (GE_PCIE_PCIE20_DBI_4_REG_BASE      + 0x00000744)
#define HWIO_GE_PCIE_VC_TX_ARBI_2_OFF_PHYS                                                                 (GE_PCIE_PCIE20_DBI_4_REG_BASE_PHYS + 0x00000744)
#define HWIO_GE_PCIE_VC_TX_ARBI_2_OFF_OFFS                                                                 (GE_PCIE_PCIE20_DBI_4_REG_BASE_OFFS + 0x00000744)
#define HWIO_GE_PCIE_VC_TX_ARBI_2_OFF_RMSK                                                                 0xffffffff
#define HWIO_GE_PCIE_VC_TX_ARBI_2_OFF_POR                                                                  0x00000000
#define HWIO_GE_PCIE_VC_TX_ARBI_2_OFF_POR_RMSK                                                             0xffffffff
#define HWIO_GE_PCIE_VC_TX_ARBI_2_OFF_ATTR                                                                        0x1
#define HWIO_GE_PCIE_VC_TX_ARBI_2_OFF_IN          \
        in_dword_masked(HWIO_GE_PCIE_VC_TX_ARBI_2_OFF_ADDR, HWIO_GE_PCIE_VC_TX_ARBI_2_OFF_RMSK)
#define HWIO_GE_PCIE_VC_TX_ARBI_2_OFF_INM(m)      \
        in_dword_masked(HWIO_GE_PCIE_VC_TX_ARBI_2_OFF_ADDR, m)
#define HWIO_GE_PCIE_VC_TX_ARBI_2_OFF_WRR_WEIGHT_VC_7_BMSK                                                 0xff000000
#define HWIO_GE_PCIE_VC_TX_ARBI_2_OFF_WRR_WEIGHT_VC_7_SHFT                                                       0x18
#define HWIO_GE_PCIE_VC_TX_ARBI_2_OFF_WRR_WEIGHT_VC_6_BMSK                                                   0xff0000
#define HWIO_GE_PCIE_VC_TX_ARBI_2_OFF_WRR_WEIGHT_VC_6_SHFT                                                       0x10
#define HWIO_GE_PCIE_VC_TX_ARBI_2_OFF_WRR_WEIGHT_VC_5_BMSK                                                     0xff00
#define HWIO_GE_PCIE_VC_TX_ARBI_2_OFF_WRR_WEIGHT_VC_5_SHFT                                                        0x8
#define HWIO_GE_PCIE_VC_TX_ARBI_2_OFF_WRR_WEIGHT_VC_4_BMSK                                                       0xff
#define HWIO_GE_PCIE_VC_TX_ARBI_2_OFF_WRR_WEIGHT_VC_4_SHFT                                                        0x0

#define HWIO_GE_PCIE_VC0_P_RX_Q_CTRL_OFF_ADDR                                                              (GE_PCIE_PCIE20_DBI_4_REG_BASE      + 0x00000748)
#define HWIO_GE_PCIE_VC0_P_RX_Q_CTRL_OFF_PHYS                                                              (GE_PCIE_PCIE20_DBI_4_REG_BASE_PHYS + 0x00000748)
#define HWIO_GE_PCIE_VC0_P_RX_Q_CTRL_OFF_OFFS                                                              (GE_PCIE_PCIE20_DBI_4_REG_BASE_OFFS + 0x00000748)
#define HWIO_GE_PCIE_VC0_P_RX_Q_CTRL_OFF_RMSK                                                              0xc0efffff
#define HWIO_GE_PCIE_VC0_P_RX_Q_CTRL_OFF_POR                                                               0x00000000
#define HWIO_GE_PCIE_VC0_P_RX_Q_CTRL_OFF_POR_RMSK                                                          0xffffffff
#define HWIO_GE_PCIE_VC0_P_RX_Q_CTRL_OFF_ATTR                                                                     0x3
#define HWIO_GE_PCIE_VC0_P_RX_Q_CTRL_OFF_IN          \
        in_dword_masked(HWIO_GE_PCIE_VC0_P_RX_Q_CTRL_OFF_ADDR, HWIO_GE_PCIE_VC0_P_RX_Q_CTRL_OFF_RMSK)
#define HWIO_GE_PCIE_VC0_P_RX_Q_CTRL_OFF_INM(m)      \
        in_dword_masked(HWIO_GE_PCIE_VC0_P_RX_Q_CTRL_OFF_ADDR, m)
#define HWIO_GE_PCIE_VC0_P_RX_Q_CTRL_OFF_OUT(v)      \
        out_dword(HWIO_GE_PCIE_VC0_P_RX_Q_CTRL_OFF_ADDR,v)
#define HWIO_GE_PCIE_VC0_P_RX_Q_CTRL_OFF_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GE_PCIE_VC0_P_RX_Q_CTRL_OFF_ADDR,m,v,HWIO_GE_PCIE_VC0_P_RX_Q_CTRL_OFF_IN)
#define HWIO_GE_PCIE_VC0_P_RX_Q_CTRL_OFF_VC_ORDERING_RX_Q_BMSK                                             0x80000000
#define HWIO_GE_PCIE_VC0_P_RX_Q_CTRL_OFF_VC_ORDERING_RX_Q_SHFT                                                   0x1f
#define HWIO_GE_PCIE_VC0_P_RX_Q_CTRL_OFF_TLP_TYPE_ORDERING_VC0_BMSK                                        0x40000000
#define HWIO_GE_PCIE_VC0_P_RX_Q_CTRL_OFF_TLP_TYPE_ORDERING_VC0_SHFT                                              0x1e
#define HWIO_GE_PCIE_VC0_P_RX_Q_CTRL_OFF_VC0_P_TLP_Q_MODE_BMSK                                               0xe00000
#define HWIO_GE_PCIE_VC0_P_RX_Q_CTRL_OFF_VC0_P_TLP_Q_MODE_SHFT                                                   0x15
#define HWIO_GE_PCIE_VC0_P_RX_Q_CTRL_OFF_VC0_P_HEADER_CREDIT_BMSK                                             0xff000
#define HWIO_GE_PCIE_VC0_P_RX_Q_CTRL_OFF_VC0_P_HEADER_CREDIT_SHFT                                                 0xc
#define HWIO_GE_PCIE_VC0_P_RX_Q_CTRL_OFF_VC0_P_DATA_CREDIT_BMSK                                                 0xfff
#define HWIO_GE_PCIE_VC0_P_RX_Q_CTRL_OFF_VC0_P_DATA_CREDIT_SHFT                                                   0x0

#define HWIO_GE_PCIE_VC0_NP_RX_Q_CTRL_OFF_ADDR                                                             (GE_PCIE_PCIE20_DBI_4_REG_BASE      + 0x0000074c)
#define HWIO_GE_PCIE_VC0_NP_RX_Q_CTRL_OFF_PHYS                                                             (GE_PCIE_PCIE20_DBI_4_REG_BASE_PHYS + 0x0000074c)
#define HWIO_GE_PCIE_VC0_NP_RX_Q_CTRL_OFF_OFFS                                                             (GE_PCIE_PCIE20_DBI_4_REG_BASE_OFFS + 0x0000074c)
#define HWIO_GE_PCIE_VC0_NP_RX_Q_CTRL_OFF_RMSK                                                               0xefffff
#define HWIO_GE_PCIE_VC0_NP_RX_Q_CTRL_OFF_POR                                                              0x00000000
#define HWIO_GE_PCIE_VC0_NP_RX_Q_CTRL_OFF_POR_RMSK                                                         0xffffffff
#define HWIO_GE_PCIE_VC0_NP_RX_Q_CTRL_OFF_ATTR                                                                    0x3
#define HWIO_GE_PCIE_VC0_NP_RX_Q_CTRL_OFF_IN          \
        in_dword_masked(HWIO_GE_PCIE_VC0_NP_RX_Q_CTRL_OFF_ADDR, HWIO_GE_PCIE_VC0_NP_RX_Q_CTRL_OFF_RMSK)
#define HWIO_GE_PCIE_VC0_NP_RX_Q_CTRL_OFF_INM(m)      \
        in_dword_masked(HWIO_GE_PCIE_VC0_NP_RX_Q_CTRL_OFF_ADDR, m)
#define HWIO_GE_PCIE_VC0_NP_RX_Q_CTRL_OFF_OUT(v)      \
        out_dword(HWIO_GE_PCIE_VC0_NP_RX_Q_CTRL_OFF_ADDR,v)
#define HWIO_GE_PCIE_VC0_NP_RX_Q_CTRL_OFF_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GE_PCIE_VC0_NP_RX_Q_CTRL_OFF_ADDR,m,v,HWIO_GE_PCIE_VC0_NP_RX_Q_CTRL_OFF_IN)
#define HWIO_GE_PCIE_VC0_NP_RX_Q_CTRL_OFF_VC0_NP_TLP_Q_MODE_BMSK                                             0xe00000
#define HWIO_GE_PCIE_VC0_NP_RX_Q_CTRL_OFF_VC0_NP_TLP_Q_MODE_SHFT                                                 0x15
#define HWIO_GE_PCIE_VC0_NP_RX_Q_CTRL_OFF_VC0_NP_HEADER_CREDIT_BMSK                                           0xff000
#define HWIO_GE_PCIE_VC0_NP_RX_Q_CTRL_OFF_VC0_NP_HEADER_CREDIT_SHFT                                               0xc
#define HWIO_GE_PCIE_VC0_NP_RX_Q_CTRL_OFF_VC0_NP_DATA_CREDIT_BMSK                                               0xfff
#define HWIO_GE_PCIE_VC0_NP_RX_Q_CTRL_OFF_VC0_NP_DATA_CREDIT_SHFT                                                 0x0

#define HWIO_GE_PCIE_VC0_CPL_RX_Q_CTRL_OFF_ADDR                                                            (GE_PCIE_PCIE20_DBI_4_REG_BASE      + 0x00000750)
#define HWIO_GE_PCIE_VC0_CPL_RX_Q_CTRL_OFF_PHYS                                                            (GE_PCIE_PCIE20_DBI_4_REG_BASE_PHYS + 0x00000750)
#define HWIO_GE_PCIE_VC0_CPL_RX_Q_CTRL_OFF_OFFS                                                            (GE_PCIE_PCIE20_DBI_4_REG_BASE_OFFS + 0x00000750)
#define HWIO_GE_PCIE_VC0_CPL_RX_Q_CTRL_OFF_RMSK                                                              0xefffff
#define HWIO_GE_PCIE_VC0_CPL_RX_Q_CTRL_OFF_POR                                                             0x00000000
#define HWIO_GE_PCIE_VC0_CPL_RX_Q_CTRL_OFF_POR_RMSK                                                        0xffffffff
#define HWIO_GE_PCIE_VC0_CPL_RX_Q_CTRL_OFF_ATTR                                                                   0x3
#define HWIO_GE_PCIE_VC0_CPL_RX_Q_CTRL_OFF_IN          \
        in_dword_masked(HWIO_GE_PCIE_VC0_CPL_RX_Q_CTRL_OFF_ADDR, HWIO_GE_PCIE_VC0_CPL_RX_Q_CTRL_OFF_RMSK)
#define HWIO_GE_PCIE_VC0_CPL_RX_Q_CTRL_OFF_INM(m)      \
        in_dword_masked(HWIO_GE_PCIE_VC0_CPL_RX_Q_CTRL_OFF_ADDR, m)
#define HWIO_GE_PCIE_VC0_CPL_RX_Q_CTRL_OFF_OUT(v)      \
        out_dword(HWIO_GE_PCIE_VC0_CPL_RX_Q_CTRL_OFF_ADDR,v)
#define HWIO_GE_PCIE_VC0_CPL_RX_Q_CTRL_OFF_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GE_PCIE_VC0_CPL_RX_Q_CTRL_OFF_ADDR,m,v,HWIO_GE_PCIE_VC0_CPL_RX_Q_CTRL_OFF_IN)
#define HWIO_GE_PCIE_VC0_CPL_RX_Q_CTRL_OFF_VC0_CPL_TLP_Q_MODE_BMSK                                           0xe00000
#define HWIO_GE_PCIE_VC0_CPL_RX_Q_CTRL_OFF_VC0_CPL_TLP_Q_MODE_SHFT                                               0x15
#define HWIO_GE_PCIE_VC0_CPL_RX_Q_CTRL_OFF_VC0_CPL_HEADER_CREDIT_BMSK                                         0xff000
#define HWIO_GE_PCIE_VC0_CPL_RX_Q_CTRL_OFF_VC0_CPL_HEADER_CREDIT_SHFT                                             0xc
#define HWIO_GE_PCIE_VC0_CPL_RX_Q_CTRL_OFF_VC0_CPL_DATA_CREDIT_BMSK                                             0xfff
#define HWIO_GE_PCIE_VC0_CPL_RX_Q_CTRL_OFF_VC0_CPL_DATA_CREDIT_SHFT                                               0x0

#define HWIO_GE_PCIE_GEN2_CTRL_OFF_ADDR                                                                    (GE_PCIE_PCIE20_DBI_4_REG_BASE      + 0x0000080c)
#define HWIO_GE_PCIE_GEN2_CTRL_OFF_PHYS                                                                    (GE_PCIE_PCIE20_DBI_4_REG_BASE_PHYS + 0x0000080c)
#define HWIO_GE_PCIE_GEN2_CTRL_OFF_OFFS                                                                    (GE_PCIE_PCIE20_DBI_4_REG_BASE_OFFS + 0x0000080c)
#define HWIO_GE_PCIE_GEN2_CTRL_OFF_RMSK                                                                    0xffffffff
#define HWIO_GE_PCIE_GEN2_CTRL_OFF_POR                                                                     0x00000000
#define HWIO_GE_PCIE_GEN2_CTRL_OFF_POR_RMSK                                                                0xffffffff
#define HWIO_GE_PCIE_GEN2_CTRL_OFF_ATTR                                                                           0x3
#define HWIO_GE_PCIE_GEN2_CTRL_OFF_IN          \
        in_dword_masked(HWIO_GE_PCIE_GEN2_CTRL_OFF_ADDR, HWIO_GE_PCIE_GEN2_CTRL_OFF_RMSK)
#define HWIO_GE_PCIE_GEN2_CTRL_OFF_INM(m)      \
        in_dword_masked(HWIO_GE_PCIE_GEN2_CTRL_OFF_ADDR, m)
#define HWIO_GE_PCIE_GEN2_CTRL_OFF_OUT(v)      \
        out_dword(HWIO_GE_PCIE_GEN2_CTRL_OFF_ADDR,v)
#define HWIO_GE_PCIE_GEN2_CTRL_OFF_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GE_PCIE_GEN2_CTRL_OFF_ADDR,m,v,HWIO_GE_PCIE_GEN2_CTRL_OFF_IN)
#define HWIO_GE_PCIE_GEN2_CTRL_OFF_RSVDP_22_BMSK                                                           0xffc00000
#define HWIO_GE_PCIE_GEN2_CTRL_OFF_RSVDP_22_SHFT                                                                 0x16
#define HWIO_GE_PCIE_GEN2_CTRL_OFF_GEN1_EI_INFERENCE_BMSK                                                    0x200000
#define HWIO_GE_PCIE_GEN2_CTRL_OFF_GEN1_EI_INFERENCE_SHFT                                                        0x15
#define HWIO_GE_PCIE_GEN2_CTRL_OFF_SEL_DEEMPHASIS_BMSK                                                       0x100000
#define HWIO_GE_PCIE_GEN2_CTRL_OFF_SEL_DEEMPHASIS_SHFT                                                           0x14
#define HWIO_GE_PCIE_GEN2_CTRL_OFF_CONFIG_TX_COMP_RX_BMSK                                                     0x80000
#define HWIO_GE_PCIE_GEN2_CTRL_OFF_CONFIG_TX_COMP_RX_SHFT                                                        0x13
#define HWIO_GE_PCIE_GEN2_CTRL_OFF_CONFIG_PHY_TX_CHANGE_BMSK                                                  0x40000
#define HWIO_GE_PCIE_GEN2_CTRL_OFF_CONFIG_PHY_TX_CHANGE_SHFT                                                     0x12
#define HWIO_GE_PCIE_GEN2_CTRL_OFF_DIRECT_SPEED_CHANGE_BMSK                                                   0x20000
#define HWIO_GE_PCIE_GEN2_CTRL_OFF_DIRECT_SPEED_CHANGE_SHFT                                                      0x11
#define HWIO_GE_PCIE_GEN2_CTRL_OFF_AUTO_LANE_FLIP_CTRL_EN_BMSK                                                0x10000
#define HWIO_GE_PCIE_GEN2_CTRL_OFF_AUTO_LANE_FLIP_CTRL_EN_SHFT                                                   0x10
#define HWIO_GE_PCIE_GEN2_CTRL_OFF_PRE_DET_LANE_BMSK                                                           0xe000
#define HWIO_GE_PCIE_GEN2_CTRL_OFF_PRE_DET_LANE_SHFT                                                              0xd
#define HWIO_GE_PCIE_GEN2_CTRL_OFF_NUM_OF_LANES_BMSK                                                           0x1f00
#define HWIO_GE_PCIE_GEN2_CTRL_OFF_NUM_OF_LANES_SHFT                                                              0x8
#define HWIO_GE_PCIE_GEN2_CTRL_OFF_FAST_TRAINING_SEQ_BMSK                                                        0xff
#define HWIO_GE_PCIE_GEN2_CTRL_OFF_FAST_TRAINING_SEQ_SHFT                                                         0x0

#define HWIO_GE_PCIE_PHY_STATUS_OFF_ADDR                                                                   (GE_PCIE_PCIE20_DBI_4_REG_BASE      + 0x00000810)
#define HWIO_GE_PCIE_PHY_STATUS_OFF_PHYS                                                                   (GE_PCIE_PCIE20_DBI_4_REG_BASE_PHYS + 0x00000810)
#define HWIO_GE_PCIE_PHY_STATUS_OFF_OFFS                                                                   (GE_PCIE_PCIE20_DBI_4_REG_BASE_OFFS + 0x00000810)
#define HWIO_GE_PCIE_PHY_STATUS_OFF_RMSK                                                                   0xffffffff
#define HWIO_GE_PCIE_PHY_STATUS_OFF_POR                                                                    0x00000000
#define HWIO_GE_PCIE_PHY_STATUS_OFF_POR_RMSK                                                               0xffffffff
#define HWIO_GE_PCIE_PHY_STATUS_OFF_ATTR                                                                          0x1
#define HWIO_GE_PCIE_PHY_STATUS_OFF_IN          \
        in_dword_masked(HWIO_GE_PCIE_PHY_STATUS_OFF_ADDR, HWIO_GE_PCIE_PHY_STATUS_OFF_RMSK)
#define HWIO_GE_PCIE_PHY_STATUS_OFF_INM(m)      \
        in_dword_masked(HWIO_GE_PCIE_PHY_STATUS_OFF_ADDR, m)
#define HWIO_GE_PCIE_PHY_STATUS_OFF_PHY_STATUS_BMSK                                                        0xffffffff
#define HWIO_GE_PCIE_PHY_STATUS_OFF_PHY_STATUS_SHFT                                                               0x0

#define HWIO_GE_PCIE_PHY_CONTROL_OFF_ADDR                                                                  (GE_PCIE_PCIE20_DBI_4_REG_BASE      + 0x00000814)
#define HWIO_GE_PCIE_PHY_CONTROL_OFF_PHYS                                                                  (GE_PCIE_PCIE20_DBI_4_REG_BASE_PHYS + 0x00000814)
#define HWIO_GE_PCIE_PHY_CONTROL_OFF_OFFS                                                                  (GE_PCIE_PCIE20_DBI_4_REG_BASE_OFFS + 0x00000814)
#define HWIO_GE_PCIE_PHY_CONTROL_OFF_RMSK                                                                  0xffffffff
#define HWIO_GE_PCIE_PHY_CONTROL_OFF_POR                                                                   0x00000000
#define HWIO_GE_PCIE_PHY_CONTROL_OFF_POR_RMSK                                                              0xffffffff
#define HWIO_GE_PCIE_PHY_CONTROL_OFF_ATTR                                                                         0x3
#define HWIO_GE_PCIE_PHY_CONTROL_OFF_IN          \
        in_dword_masked(HWIO_GE_PCIE_PHY_CONTROL_OFF_ADDR, HWIO_GE_PCIE_PHY_CONTROL_OFF_RMSK)
#define HWIO_GE_PCIE_PHY_CONTROL_OFF_INM(m)      \
        in_dword_masked(HWIO_GE_PCIE_PHY_CONTROL_OFF_ADDR, m)
#define HWIO_GE_PCIE_PHY_CONTROL_OFF_OUT(v)      \
        out_dword(HWIO_GE_PCIE_PHY_CONTROL_OFF_ADDR,v)
#define HWIO_GE_PCIE_PHY_CONTROL_OFF_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GE_PCIE_PHY_CONTROL_OFF_ADDR,m,v,HWIO_GE_PCIE_PHY_CONTROL_OFF_IN)
#define HWIO_GE_PCIE_PHY_CONTROL_OFF_PHY_CONTROL_BMSK                                                      0xffffffff
#define HWIO_GE_PCIE_PHY_CONTROL_OFF_PHY_CONTROL_SHFT                                                             0x0

#define HWIO_GE_PCIE_MSI_CTRL_ADDR_OFF_ADDR                                                                (GE_PCIE_PCIE20_DBI_4_REG_BASE      + 0x00000820)
#define HWIO_GE_PCIE_MSI_CTRL_ADDR_OFF_PHYS                                                                (GE_PCIE_PCIE20_DBI_4_REG_BASE_PHYS + 0x00000820)
#define HWIO_GE_PCIE_MSI_CTRL_ADDR_OFF_OFFS                                                                (GE_PCIE_PCIE20_DBI_4_REG_BASE_OFFS + 0x00000820)
#define HWIO_GE_PCIE_MSI_CTRL_ADDR_OFF_RMSK                                                                0xffffffff
#define HWIO_GE_PCIE_MSI_CTRL_ADDR_OFF_POR                                                                 0x00000000
#define HWIO_GE_PCIE_MSI_CTRL_ADDR_OFF_POR_RMSK                                                            0xffffffff
#define HWIO_GE_PCIE_MSI_CTRL_ADDR_OFF_ATTR                                                                       0x3
#define HWIO_GE_PCIE_MSI_CTRL_ADDR_OFF_IN          \
        in_dword_masked(HWIO_GE_PCIE_MSI_CTRL_ADDR_OFF_ADDR, HWIO_GE_PCIE_MSI_CTRL_ADDR_OFF_RMSK)
#define HWIO_GE_PCIE_MSI_CTRL_ADDR_OFF_INM(m)      \
        in_dword_masked(HWIO_GE_PCIE_MSI_CTRL_ADDR_OFF_ADDR, m)
#define HWIO_GE_PCIE_MSI_CTRL_ADDR_OFF_OUT(v)      \
        out_dword(HWIO_GE_PCIE_MSI_CTRL_ADDR_OFF_ADDR,v)
#define HWIO_GE_PCIE_MSI_CTRL_ADDR_OFF_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GE_PCIE_MSI_CTRL_ADDR_OFF_ADDR,m,v,HWIO_GE_PCIE_MSI_CTRL_ADDR_OFF_IN)
#define HWIO_GE_PCIE_MSI_CTRL_ADDR_OFF_MSI_CTRL_ADDR_BMSK                                                  0xffffffff
#define HWIO_GE_PCIE_MSI_CTRL_ADDR_OFF_MSI_CTRL_ADDR_SHFT                                                         0x0

#define HWIO_GE_PCIE_MSI_CTRL_UPPER_ADDR_OFF_ADDR                                                          (GE_PCIE_PCIE20_DBI_4_REG_BASE      + 0x00000824)
#define HWIO_GE_PCIE_MSI_CTRL_UPPER_ADDR_OFF_PHYS                                                          (GE_PCIE_PCIE20_DBI_4_REG_BASE_PHYS + 0x00000824)
#define HWIO_GE_PCIE_MSI_CTRL_UPPER_ADDR_OFF_OFFS                                                          (GE_PCIE_PCIE20_DBI_4_REG_BASE_OFFS + 0x00000824)
#define HWIO_GE_PCIE_MSI_CTRL_UPPER_ADDR_OFF_RMSK                                                          0xffffffff
#define HWIO_GE_PCIE_MSI_CTRL_UPPER_ADDR_OFF_POR                                                           0x00000000
#define HWIO_GE_PCIE_MSI_CTRL_UPPER_ADDR_OFF_POR_RMSK                                                      0xffffffff
#define HWIO_GE_PCIE_MSI_CTRL_UPPER_ADDR_OFF_ATTR                                                                 0x3
#define HWIO_GE_PCIE_MSI_CTRL_UPPER_ADDR_OFF_IN          \
        in_dword_masked(HWIO_GE_PCIE_MSI_CTRL_UPPER_ADDR_OFF_ADDR, HWIO_GE_PCIE_MSI_CTRL_UPPER_ADDR_OFF_RMSK)
#define HWIO_GE_PCIE_MSI_CTRL_UPPER_ADDR_OFF_INM(m)      \
        in_dword_masked(HWIO_GE_PCIE_MSI_CTRL_UPPER_ADDR_OFF_ADDR, m)
#define HWIO_GE_PCIE_MSI_CTRL_UPPER_ADDR_OFF_OUT(v)      \
        out_dword(HWIO_GE_PCIE_MSI_CTRL_UPPER_ADDR_OFF_ADDR,v)
#define HWIO_GE_PCIE_MSI_CTRL_UPPER_ADDR_OFF_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GE_PCIE_MSI_CTRL_UPPER_ADDR_OFF_ADDR,m,v,HWIO_GE_PCIE_MSI_CTRL_UPPER_ADDR_OFF_IN)
#define HWIO_GE_PCIE_MSI_CTRL_UPPER_ADDR_OFF_MSI_CTRL_UPPER_ADDR_BMSK                                      0xffffffff
#define HWIO_GE_PCIE_MSI_CTRL_UPPER_ADDR_OFF_MSI_CTRL_UPPER_ADDR_SHFT                                             0x0

#define HWIO_GE_PCIE_MSI_CTRL_INT_0_EN_OFF_ADDR                                                            (GE_PCIE_PCIE20_DBI_4_REG_BASE      + 0x00000828)
#define HWIO_GE_PCIE_MSI_CTRL_INT_0_EN_OFF_PHYS                                                            (GE_PCIE_PCIE20_DBI_4_REG_BASE_PHYS + 0x00000828)
#define HWIO_GE_PCIE_MSI_CTRL_INT_0_EN_OFF_OFFS                                                            (GE_PCIE_PCIE20_DBI_4_REG_BASE_OFFS + 0x00000828)
#define HWIO_GE_PCIE_MSI_CTRL_INT_0_EN_OFF_RMSK                                                            0xffffffff
#define HWIO_GE_PCIE_MSI_CTRL_INT_0_EN_OFF_POR                                                             0x00000000
#define HWIO_GE_PCIE_MSI_CTRL_INT_0_EN_OFF_POR_RMSK                                                        0xffffffff
#define HWIO_GE_PCIE_MSI_CTRL_INT_0_EN_OFF_ATTR                                                                   0x3
#define HWIO_GE_PCIE_MSI_CTRL_INT_0_EN_OFF_IN          \
        in_dword_masked(HWIO_GE_PCIE_MSI_CTRL_INT_0_EN_OFF_ADDR, HWIO_GE_PCIE_MSI_CTRL_INT_0_EN_OFF_RMSK)
#define HWIO_GE_PCIE_MSI_CTRL_INT_0_EN_OFF_INM(m)      \
        in_dword_masked(HWIO_GE_PCIE_MSI_CTRL_INT_0_EN_OFF_ADDR, m)
#define HWIO_GE_PCIE_MSI_CTRL_INT_0_EN_OFF_OUT(v)      \
        out_dword(HWIO_GE_PCIE_MSI_CTRL_INT_0_EN_OFF_ADDR,v)
#define HWIO_GE_PCIE_MSI_CTRL_INT_0_EN_OFF_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GE_PCIE_MSI_CTRL_INT_0_EN_OFF_ADDR,m,v,HWIO_GE_PCIE_MSI_CTRL_INT_0_EN_OFF_IN)
#define HWIO_GE_PCIE_MSI_CTRL_INT_0_EN_OFF_MSI_CTRL_INT_0_EN_BMSK                                          0xffffffff
#define HWIO_GE_PCIE_MSI_CTRL_INT_0_EN_OFF_MSI_CTRL_INT_0_EN_SHFT                                                 0x0

#define HWIO_GE_PCIE_MSI_CTRL_INT_0_MASK_OFF_ADDR                                                          (GE_PCIE_PCIE20_DBI_4_REG_BASE      + 0x0000082c)
#define HWIO_GE_PCIE_MSI_CTRL_INT_0_MASK_OFF_PHYS                                                          (GE_PCIE_PCIE20_DBI_4_REG_BASE_PHYS + 0x0000082c)
#define HWIO_GE_PCIE_MSI_CTRL_INT_0_MASK_OFF_OFFS                                                          (GE_PCIE_PCIE20_DBI_4_REG_BASE_OFFS + 0x0000082c)
#define HWIO_GE_PCIE_MSI_CTRL_INT_0_MASK_OFF_RMSK                                                          0xffffffff
#define HWIO_GE_PCIE_MSI_CTRL_INT_0_MASK_OFF_POR                                                           0x00000000
#define HWIO_GE_PCIE_MSI_CTRL_INT_0_MASK_OFF_POR_RMSK                                                      0xffffffff
#define HWIO_GE_PCIE_MSI_CTRL_INT_0_MASK_OFF_ATTR                                                                 0x3
#define HWIO_GE_PCIE_MSI_CTRL_INT_0_MASK_OFF_IN          \
        in_dword_masked(HWIO_GE_PCIE_MSI_CTRL_INT_0_MASK_OFF_ADDR, HWIO_GE_PCIE_MSI_CTRL_INT_0_MASK_OFF_RMSK)
#define HWIO_GE_PCIE_MSI_CTRL_INT_0_MASK_OFF_INM(m)      \
        in_dword_masked(HWIO_GE_PCIE_MSI_CTRL_INT_0_MASK_OFF_ADDR, m)
#define HWIO_GE_PCIE_MSI_CTRL_INT_0_MASK_OFF_OUT(v)      \
        out_dword(HWIO_GE_PCIE_MSI_CTRL_INT_0_MASK_OFF_ADDR,v)
#define HWIO_GE_PCIE_MSI_CTRL_INT_0_MASK_OFF_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GE_PCIE_MSI_CTRL_INT_0_MASK_OFF_ADDR,m,v,HWIO_GE_PCIE_MSI_CTRL_INT_0_MASK_OFF_IN)
#define HWIO_GE_PCIE_MSI_CTRL_INT_0_MASK_OFF_MSI_CTRL_INT_0_MASK_BMSK                                      0xffffffff
#define HWIO_GE_PCIE_MSI_CTRL_INT_0_MASK_OFF_MSI_CTRL_INT_0_MASK_SHFT                                             0x0

#define HWIO_GE_PCIE_MSI_CTRL_INT_0_STATUS_OFF_ADDR                                                        (GE_PCIE_PCIE20_DBI_4_REG_BASE      + 0x00000830)
#define HWIO_GE_PCIE_MSI_CTRL_INT_0_STATUS_OFF_PHYS                                                        (GE_PCIE_PCIE20_DBI_4_REG_BASE_PHYS + 0x00000830)
#define HWIO_GE_PCIE_MSI_CTRL_INT_0_STATUS_OFF_OFFS                                                        (GE_PCIE_PCIE20_DBI_4_REG_BASE_OFFS + 0x00000830)
#define HWIO_GE_PCIE_MSI_CTRL_INT_0_STATUS_OFF_RMSK                                                        0xffffffff
#define HWIO_GE_PCIE_MSI_CTRL_INT_0_STATUS_OFF_POR                                                         0x00000000
#define HWIO_GE_PCIE_MSI_CTRL_INT_0_STATUS_OFF_POR_RMSK                                                    0xffffffff
#define HWIO_GE_PCIE_MSI_CTRL_INT_0_STATUS_OFF_ATTR                                                               0x3
#define HWIO_GE_PCIE_MSI_CTRL_INT_0_STATUS_OFF_IN          \
        in_dword_masked(HWIO_GE_PCIE_MSI_CTRL_INT_0_STATUS_OFF_ADDR, HWIO_GE_PCIE_MSI_CTRL_INT_0_STATUS_OFF_RMSK)
#define HWIO_GE_PCIE_MSI_CTRL_INT_0_STATUS_OFF_INM(m)      \
        in_dword_masked(HWIO_GE_PCIE_MSI_CTRL_INT_0_STATUS_OFF_ADDR, m)
#define HWIO_GE_PCIE_MSI_CTRL_INT_0_STATUS_OFF_OUT(v)      \
        out_dword(HWIO_GE_PCIE_MSI_CTRL_INT_0_STATUS_OFF_ADDR,v)
#define HWIO_GE_PCIE_MSI_CTRL_INT_0_STATUS_OFF_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GE_PCIE_MSI_CTRL_INT_0_STATUS_OFF_ADDR,m,v,HWIO_GE_PCIE_MSI_CTRL_INT_0_STATUS_OFF_IN)
#define HWIO_GE_PCIE_MSI_CTRL_INT_0_STATUS_OFF_MSI_CTRL_INT_0_STATUS_BMSK                                  0xffffffff
#define HWIO_GE_PCIE_MSI_CTRL_INT_0_STATUS_OFF_MSI_CTRL_INT_0_STATUS_SHFT                                         0x0

#define HWIO_GE_PCIE_MSI_CTRL_INT_1_EN_OFF_ADDR                                                            (GE_PCIE_PCIE20_DBI_4_REG_BASE      + 0x00000834)
#define HWIO_GE_PCIE_MSI_CTRL_INT_1_EN_OFF_PHYS                                                            (GE_PCIE_PCIE20_DBI_4_REG_BASE_PHYS + 0x00000834)
#define HWIO_GE_PCIE_MSI_CTRL_INT_1_EN_OFF_OFFS                                                            (GE_PCIE_PCIE20_DBI_4_REG_BASE_OFFS + 0x00000834)
#define HWIO_GE_PCIE_MSI_CTRL_INT_1_EN_OFF_RMSK                                                            0xffffffff
#define HWIO_GE_PCIE_MSI_CTRL_INT_1_EN_OFF_POR                                                             0x00000000
#define HWIO_GE_PCIE_MSI_CTRL_INT_1_EN_OFF_POR_RMSK                                                        0xffffffff
#define HWIO_GE_PCIE_MSI_CTRL_INT_1_EN_OFF_ATTR                                                                   0x3
#define HWIO_GE_PCIE_MSI_CTRL_INT_1_EN_OFF_IN          \
        in_dword_masked(HWIO_GE_PCIE_MSI_CTRL_INT_1_EN_OFF_ADDR, HWIO_GE_PCIE_MSI_CTRL_INT_1_EN_OFF_RMSK)
#define HWIO_GE_PCIE_MSI_CTRL_INT_1_EN_OFF_INM(m)      \
        in_dword_masked(HWIO_GE_PCIE_MSI_CTRL_INT_1_EN_OFF_ADDR, m)
#define HWIO_GE_PCIE_MSI_CTRL_INT_1_EN_OFF_OUT(v)      \
        out_dword(HWIO_GE_PCIE_MSI_CTRL_INT_1_EN_OFF_ADDR,v)
#define HWIO_GE_PCIE_MSI_CTRL_INT_1_EN_OFF_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GE_PCIE_MSI_CTRL_INT_1_EN_OFF_ADDR,m,v,HWIO_GE_PCIE_MSI_CTRL_INT_1_EN_OFF_IN)
#define HWIO_GE_PCIE_MSI_CTRL_INT_1_EN_OFF_MSI_CTRL_INT_1_EN_BMSK                                          0xffffffff
#define HWIO_GE_PCIE_MSI_CTRL_INT_1_EN_OFF_MSI_CTRL_INT_1_EN_SHFT                                                 0x0

#define HWIO_GE_PCIE_MSI_CTRL_INT_1_MASK_OFF_ADDR                                                          (GE_PCIE_PCIE20_DBI_4_REG_BASE      + 0x00000838)
#define HWIO_GE_PCIE_MSI_CTRL_INT_1_MASK_OFF_PHYS                                                          (GE_PCIE_PCIE20_DBI_4_REG_BASE_PHYS + 0x00000838)
#define HWIO_GE_PCIE_MSI_CTRL_INT_1_MASK_OFF_OFFS                                                          (GE_PCIE_PCIE20_DBI_4_REG_BASE_OFFS + 0x00000838)
#define HWIO_GE_PCIE_MSI_CTRL_INT_1_MASK_OFF_RMSK                                                          0xffffffff
#define HWIO_GE_PCIE_MSI_CTRL_INT_1_MASK_OFF_POR                                                           0x00000000
#define HWIO_GE_PCIE_MSI_CTRL_INT_1_MASK_OFF_POR_RMSK                                                      0xffffffff
#define HWIO_GE_PCIE_MSI_CTRL_INT_1_MASK_OFF_ATTR                                                                 0x3
#define HWIO_GE_PCIE_MSI_CTRL_INT_1_MASK_OFF_IN          \
        in_dword_masked(HWIO_GE_PCIE_MSI_CTRL_INT_1_MASK_OFF_ADDR, HWIO_GE_PCIE_MSI_CTRL_INT_1_MASK_OFF_RMSK)
#define HWIO_GE_PCIE_MSI_CTRL_INT_1_MASK_OFF_INM(m)      \
        in_dword_masked(HWIO_GE_PCIE_MSI_CTRL_INT_1_MASK_OFF_ADDR, m)
#define HWIO_GE_PCIE_MSI_CTRL_INT_1_MASK_OFF_OUT(v)      \
        out_dword(HWIO_GE_PCIE_MSI_CTRL_INT_1_MASK_OFF_ADDR,v)
#define HWIO_GE_PCIE_MSI_CTRL_INT_1_MASK_OFF_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GE_PCIE_MSI_CTRL_INT_1_MASK_OFF_ADDR,m,v,HWIO_GE_PCIE_MSI_CTRL_INT_1_MASK_OFF_IN)
#define HWIO_GE_PCIE_MSI_CTRL_INT_1_MASK_OFF_MSI_CTRL_INT_1_MASK_BMSK                                      0xffffffff
#define HWIO_GE_PCIE_MSI_CTRL_INT_1_MASK_OFF_MSI_CTRL_INT_1_MASK_SHFT                                             0x0

#define HWIO_GE_PCIE_MSI_CTRL_INT_1_STATUS_OFF_ADDR                                                        (GE_PCIE_PCIE20_DBI_4_REG_BASE      + 0x0000083c)
#define HWIO_GE_PCIE_MSI_CTRL_INT_1_STATUS_OFF_PHYS                                                        (GE_PCIE_PCIE20_DBI_4_REG_BASE_PHYS + 0x0000083c)
#define HWIO_GE_PCIE_MSI_CTRL_INT_1_STATUS_OFF_OFFS                                                        (GE_PCIE_PCIE20_DBI_4_REG_BASE_OFFS + 0x0000083c)
#define HWIO_GE_PCIE_MSI_CTRL_INT_1_STATUS_OFF_RMSK                                                        0xffffffff
#define HWIO_GE_PCIE_MSI_CTRL_INT_1_STATUS_OFF_POR                                                         0x00000000
#define HWIO_GE_PCIE_MSI_CTRL_INT_1_STATUS_OFF_POR_RMSK                                                    0xffffffff
#define HWIO_GE_PCIE_MSI_CTRL_INT_1_STATUS_OFF_ATTR                                                               0x3
#define HWIO_GE_PCIE_MSI_CTRL_INT_1_STATUS_OFF_IN          \
        in_dword_masked(HWIO_GE_PCIE_MSI_CTRL_INT_1_STATUS_OFF_ADDR, HWIO_GE_PCIE_MSI_CTRL_INT_1_STATUS_OFF_RMSK)
#define HWIO_GE_PCIE_MSI_CTRL_INT_1_STATUS_OFF_INM(m)      \
        in_dword_masked(HWIO_GE_PCIE_MSI_CTRL_INT_1_STATUS_OFF_ADDR, m)
#define HWIO_GE_PCIE_MSI_CTRL_INT_1_STATUS_OFF_OUT(v)      \
        out_dword(HWIO_GE_PCIE_MSI_CTRL_INT_1_STATUS_OFF_ADDR,v)
#define HWIO_GE_PCIE_MSI_CTRL_INT_1_STATUS_OFF_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GE_PCIE_MSI_CTRL_INT_1_STATUS_OFF_ADDR,m,v,HWIO_GE_PCIE_MSI_CTRL_INT_1_STATUS_OFF_IN)
#define HWIO_GE_PCIE_MSI_CTRL_INT_1_STATUS_OFF_MSI_CTRL_INT_1_STATUS_BMSK                                  0xffffffff
#define HWIO_GE_PCIE_MSI_CTRL_INT_1_STATUS_OFF_MSI_CTRL_INT_1_STATUS_SHFT                                         0x0

#define HWIO_GE_PCIE_MSI_CTRL_INT_2_EN_OFF_ADDR                                                            (GE_PCIE_PCIE20_DBI_4_REG_BASE      + 0x00000840)
#define HWIO_GE_PCIE_MSI_CTRL_INT_2_EN_OFF_PHYS                                                            (GE_PCIE_PCIE20_DBI_4_REG_BASE_PHYS + 0x00000840)
#define HWIO_GE_PCIE_MSI_CTRL_INT_2_EN_OFF_OFFS                                                            (GE_PCIE_PCIE20_DBI_4_REG_BASE_OFFS + 0x00000840)
#define HWIO_GE_PCIE_MSI_CTRL_INT_2_EN_OFF_RMSK                                                            0xffffffff
#define HWIO_GE_PCIE_MSI_CTRL_INT_2_EN_OFF_POR                                                             0x00000000
#define HWIO_GE_PCIE_MSI_CTRL_INT_2_EN_OFF_POR_RMSK                                                        0xffffffff
#define HWIO_GE_PCIE_MSI_CTRL_INT_2_EN_OFF_ATTR                                                                   0x3
#define HWIO_GE_PCIE_MSI_CTRL_INT_2_EN_OFF_IN          \
        in_dword_masked(HWIO_GE_PCIE_MSI_CTRL_INT_2_EN_OFF_ADDR, HWIO_GE_PCIE_MSI_CTRL_INT_2_EN_OFF_RMSK)
#define HWIO_GE_PCIE_MSI_CTRL_INT_2_EN_OFF_INM(m)      \
        in_dword_masked(HWIO_GE_PCIE_MSI_CTRL_INT_2_EN_OFF_ADDR, m)
#define HWIO_GE_PCIE_MSI_CTRL_INT_2_EN_OFF_OUT(v)      \
        out_dword(HWIO_GE_PCIE_MSI_CTRL_INT_2_EN_OFF_ADDR,v)
#define HWIO_GE_PCIE_MSI_CTRL_INT_2_EN_OFF_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GE_PCIE_MSI_CTRL_INT_2_EN_OFF_ADDR,m,v,HWIO_GE_PCIE_MSI_CTRL_INT_2_EN_OFF_IN)
#define HWIO_GE_PCIE_MSI_CTRL_INT_2_EN_OFF_MSI_CTRL_INT_2_EN_BMSK                                          0xffffffff
#define HWIO_GE_PCIE_MSI_CTRL_INT_2_EN_OFF_MSI_CTRL_INT_2_EN_SHFT                                                 0x0

#define HWIO_GE_PCIE_MSI_CTRL_INT_2_MASK_OFF_ADDR                                                          (GE_PCIE_PCIE20_DBI_4_REG_BASE      + 0x00000844)
#define HWIO_GE_PCIE_MSI_CTRL_INT_2_MASK_OFF_PHYS                                                          (GE_PCIE_PCIE20_DBI_4_REG_BASE_PHYS + 0x00000844)
#define HWIO_GE_PCIE_MSI_CTRL_INT_2_MASK_OFF_OFFS                                                          (GE_PCIE_PCIE20_DBI_4_REG_BASE_OFFS + 0x00000844)
#define HWIO_GE_PCIE_MSI_CTRL_INT_2_MASK_OFF_RMSK                                                          0xffffffff
#define HWIO_GE_PCIE_MSI_CTRL_INT_2_MASK_OFF_POR                                                           0x00000000
#define HWIO_GE_PCIE_MSI_CTRL_INT_2_MASK_OFF_POR_RMSK                                                      0xffffffff
#define HWIO_GE_PCIE_MSI_CTRL_INT_2_MASK_OFF_ATTR                                                                 0x3
#define HWIO_GE_PCIE_MSI_CTRL_INT_2_MASK_OFF_IN          \
        in_dword_masked(HWIO_GE_PCIE_MSI_CTRL_INT_2_MASK_OFF_ADDR, HWIO_GE_PCIE_MSI_CTRL_INT_2_MASK_OFF_RMSK)
#define HWIO_GE_PCIE_MSI_CTRL_INT_2_MASK_OFF_INM(m)      \
        in_dword_masked(HWIO_GE_PCIE_MSI_CTRL_INT_2_MASK_OFF_ADDR, m)
#define HWIO_GE_PCIE_MSI_CTRL_INT_2_MASK_OFF_OUT(v)      \
        out_dword(HWIO_GE_PCIE_MSI_CTRL_INT_2_MASK_OFF_ADDR,v)
#define HWIO_GE_PCIE_MSI_CTRL_INT_2_MASK_OFF_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GE_PCIE_MSI_CTRL_INT_2_MASK_OFF_ADDR,m,v,HWIO_GE_PCIE_MSI_CTRL_INT_2_MASK_OFF_IN)
#define HWIO_GE_PCIE_MSI_CTRL_INT_2_MASK_OFF_MSI_CTRL_INT_2_MASK_BMSK                                      0xffffffff
#define HWIO_GE_PCIE_MSI_CTRL_INT_2_MASK_OFF_MSI_CTRL_INT_2_MASK_SHFT                                             0x0

#define HWIO_GE_PCIE_MSI_CTRL_INT_2_STATUS_OFF_ADDR                                                        (GE_PCIE_PCIE20_DBI_4_REG_BASE      + 0x00000848)
#define HWIO_GE_PCIE_MSI_CTRL_INT_2_STATUS_OFF_PHYS                                                        (GE_PCIE_PCIE20_DBI_4_REG_BASE_PHYS + 0x00000848)
#define HWIO_GE_PCIE_MSI_CTRL_INT_2_STATUS_OFF_OFFS                                                        (GE_PCIE_PCIE20_DBI_4_REG_BASE_OFFS + 0x00000848)
#define HWIO_GE_PCIE_MSI_CTRL_INT_2_STATUS_OFF_RMSK                                                        0xffffffff
#define HWIO_GE_PCIE_MSI_CTRL_INT_2_STATUS_OFF_POR                                                         0x00000000
#define HWIO_GE_PCIE_MSI_CTRL_INT_2_STATUS_OFF_POR_RMSK                                                    0xffffffff
#define HWIO_GE_PCIE_MSI_CTRL_INT_2_STATUS_OFF_ATTR                                                               0x3
#define HWIO_GE_PCIE_MSI_CTRL_INT_2_STATUS_OFF_IN          \
        in_dword_masked(HWIO_GE_PCIE_MSI_CTRL_INT_2_STATUS_OFF_ADDR, HWIO_GE_PCIE_MSI_CTRL_INT_2_STATUS_OFF_RMSK)
#define HWIO_GE_PCIE_MSI_CTRL_INT_2_STATUS_OFF_INM(m)      \
        in_dword_masked(HWIO_GE_PCIE_MSI_CTRL_INT_2_STATUS_OFF_ADDR, m)
#define HWIO_GE_PCIE_MSI_CTRL_INT_2_STATUS_OFF_OUT(v)      \
        out_dword(HWIO_GE_PCIE_MSI_CTRL_INT_2_STATUS_OFF_ADDR,v)
#define HWIO_GE_PCIE_MSI_CTRL_INT_2_STATUS_OFF_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GE_PCIE_MSI_CTRL_INT_2_STATUS_OFF_ADDR,m,v,HWIO_GE_PCIE_MSI_CTRL_INT_2_STATUS_OFF_IN)
#define HWIO_GE_PCIE_MSI_CTRL_INT_2_STATUS_OFF_MSI_CTRL_INT_2_STATUS_BMSK                                  0xffffffff
#define HWIO_GE_PCIE_MSI_CTRL_INT_2_STATUS_OFF_MSI_CTRL_INT_2_STATUS_SHFT                                         0x0

#define HWIO_GE_PCIE_MSI_CTRL_INT_3_EN_OFF_ADDR                                                            (GE_PCIE_PCIE20_DBI_4_REG_BASE      + 0x0000084c)
#define HWIO_GE_PCIE_MSI_CTRL_INT_3_EN_OFF_PHYS                                                            (GE_PCIE_PCIE20_DBI_4_REG_BASE_PHYS + 0x0000084c)
#define HWIO_GE_PCIE_MSI_CTRL_INT_3_EN_OFF_OFFS                                                            (GE_PCIE_PCIE20_DBI_4_REG_BASE_OFFS + 0x0000084c)
#define HWIO_GE_PCIE_MSI_CTRL_INT_3_EN_OFF_RMSK                                                            0xffffffff
#define HWIO_GE_PCIE_MSI_CTRL_INT_3_EN_OFF_POR                                                             0x00000000
#define HWIO_GE_PCIE_MSI_CTRL_INT_3_EN_OFF_POR_RMSK                                                        0xffffffff
#define HWIO_GE_PCIE_MSI_CTRL_INT_3_EN_OFF_ATTR                                                                   0x3
#define HWIO_GE_PCIE_MSI_CTRL_INT_3_EN_OFF_IN          \
        in_dword_masked(HWIO_GE_PCIE_MSI_CTRL_INT_3_EN_OFF_ADDR, HWIO_GE_PCIE_MSI_CTRL_INT_3_EN_OFF_RMSK)
#define HWIO_GE_PCIE_MSI_CTRL_INT_3_EN_OFF_INM(m)      \
        in_dword_masked(HWIO_GE_PCIE_MSI_CTRL_INT_3_EN_OFF_ADDR, m)
#define HWIO_GE_PCIE_MSI_CTRL_INT_3_EN_OFF_OUT(v)      \
        out_dword(HWIO_GE_PCIE_MSI_CTRL_INT_3_EN_OFF_ADDR,v)
#define HWIO_GE_PCIE_MSI_CTRL_INT_3_EN_OFF_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GE_PCIE_MSI_CTRL_INT_3_EN_OFF_ADDR,m,v,HWIO_GE_PCIE_MSI_CTRL_INT_3_EN_OFF_IN)
#define HWIO_GE_PCIE_MSI_CTRL_INT_3_EN_OFF_MSI_CTRL_INT_3_EN_BMSK                                          0xffffffff
#define HWIO_GE_PCIE_MSI_CTRL_INT_3_EN_OFF_MSI_CTRL_INT_3_EN_SHFT                                                 0x0

#define HWIO_GE_PCIE_MSI_CTRL_INT_3_MASK_OFF_ADDR                                                          (GE_PCIE_PCIE20_DBI_4_REG_BASE      + 0x00000850)
#define HWIO_GE_PCIE_MSI_CTRL_INT_3_MASK_OFF_PHYS                                                          (GE_PCIE_PCIE20_DBI_4_REG_BASE_PHYS + 0x00000850)
#define HWIO_GE_PCIE_MSI_CTRL_INT_3_MASK_OFF_OFFS                                                          (GE_PCIE_PCIE20_DBI_4_REG_BASE_OFFS + 0x00000850)
#define HWIO_GE_PCIE_MSI_CTRL_INT_3_MASK_OFF_RMSK                                                          0xffffffff
#define HWIO_GE_PCIE_MSI_CTRL_INT_3_MASK_OFF_POR                                                           0x00000000
#define HWIO_GE_PCIE_MSI_CTRL_INT_3_MASK_OFF_POR_RMSK                                                      0xffffffff
#define HWIO_GE_PCIE_MSI_CTRL_INT_3_MASK_OFF_ATTR                                                                 0x3
#define HWIO_GE_PCIE_MSI_CTRL_INT_3_MASK_OFF_IN          \
        in_dword_masked(HWIO_GE_PCIE_MSI_CTRL_INT_3_MASK_OFF_ADDR, HWIO_GE_PCIE_MSI_CTRL_INT_3_MASK_OFF_RMSK)
#define HWIO_GE_PCIE_MSI_CTRL_INT_3_MASK_OFF_INM(m)      \
        in_dword_masked(HWIO_GE_PCIE_MSI_CTRL_INT_3_MASK_OFF_ADDR, m)
#define HWIO_GE_PCIE_MSI_CTRL_INT_3_MASK_OFF_OUT(v)      \
        out_dword(HWIO_GE_PCIE_MSI_CTRL_INT_3_MASK_OFF_ADDR,v)
#define HWIO_GE_PCIE_MSI_CTRL_INT_3_MASK_OFF_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GE_PCIE_MSI_CTRL_INT_3_MASK_OFF_ADDR,m,v,HWIO_GE_PCIE_MSI_CTRL_INT_3_MASK_OFF_IN)
#define HWIO_GE_PCIE_MSI_CTRL_INT_3_MASK_OFF_MSI_CTRL_INT_3_MASK_BMSK                                      0xffffffff
#define HWIO_GE_PCIE_MSI_CTRL_INT_3_MASK_OFF_MSI_CTRL_INT_3_MASK_SHFT                                             0x0

#define HWIO_GE_PCIE_MSI_CTRL_INT_3_STATUS_OFF_ADDR                                                        (GE_PCIE_PCIE20_DBI_4_REG_BASE      + 0x00000854)
#define HWIO_GE_PCIE_MSI_CTRL_INT_3_STATUS_OFF_PHYS                                                        (GE_PCIE_PCIE20_DBI_4_REG_BASE_PHYS + 0x00000854)
#define HWIO_GE_PCIE_MSI_CTRL_INT_3_STATUS_OFF_OFFS                                                        (GE_PCIE_PCIE20_DBI_4_REG_BASE_OFFS + 0x00000854)
#define HWIO_GE_PCIE_MSI_CTRL_INT_3_STATUS_OFF_RMSK                                                        0xffffffff
#define HWIO_GE_PCIE_MSI_CTRL_INT_3_STATUS_OFF_POR                                                         0x00000000
#define HWIO_GE_PCIE_MSI_CTRL_INT_3_STATUS_OFF_POR_RMSK                                                    0xffffffff
#define HWIO_GE_PCIE_MSI_CTRL_INT_3_STATUS_OFF_ATTR                                                               0x3
#define HWIO_GE_PCIE_MSI_CTRL_INT_3_STATUS_OFF_IN          \
        in_dword_masked(HWIO_GE_PCIE_MSI_CTRL_INT_3_STATUS_OFF_ADDR, HWIO_GE_PCIE_MSI_CTRL_INT_3_STATUS_OFF_RMSK)
#define HWIO_GE_PCIE_MSI_CTRL_INT_3_STATUS_OFF_INM(m)      \
        in_dword_masked(HWIO_GE_PCIE_MSI_CTRL_INT_3_STATUS_OFF_ADDR, m)
#define HWIO_GE_PCIE_MSI_CTRL_INT_3_STATUS_OFF_OUT(v)      \
        out_dword(HWIO_GE_PCIE_MSI_CTRL_INT_3_STATUS_OFF_ADDR,v)
#define HWIO_GE_PCIE_MSI_CTRL_INT_3_STATUS_OFF_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GE_PCIE_MSI_CTRL_INT_3_STATUS_OFF_ADDR,m,v,HWIO_GE_PCIE_MSI_CTRL_INT_3_STATUS_OFF_IN)
#define HWIO_GE_PCIE_MSI_CTRL_INT_3_STATUS_OFF_MSI_CTRL_INT_3_STATUS_BMSK                                  0xffffffff
#define HWIO_GE_PCIE_MSI_CTRL_INT_3_STATUS_OFF_MSI_CTRL_INT_3_STATUS_SHFT                                         0x0

#define HWIO_GE_PCIE_MSI_CTRL_INT_4_EN_OFF_ADDR                                                            (GE_PCIE_PCIE20_DBI_4_REG_BASE      + 0x00000858)
#define HWIO_GE_PCIE_MSI_CTRL_INT_4_EN_OFF_PHYS                                                            (GE_PCIE_PCIE20_DBI_4_REG_BASE_PHYS + 0x00000858)
#define HWIO_GE_PCIE_MSI_CTRL_INT_4_EN_OFF_OFFS                                                            (GE_PCIE_PCIE20_DBI_4_REG_BASE_OFFS + 0x00000858)
#define HWIO_GE_PCIE_MSI_CTRL_INT_4_EN_OFF_RMSK                                                            0xffffffff
#define HWIO_GE_PCIE_MSI_CTRL_INT_4_EN_OFF_POR                                                             0x00000000
#define HWIO_GE_PCIE_MSI_CTRL_INT_4_EN_OFF_POR_RMSK                                                        0xffffffff
#define HWIO_GE_PCIE_MSI_CTRL_INT_4_EN_OFF_ATTR                                                                   0x3
#define HWIO_GE_PCIE_MSI_CTRL_INT_4_EN_OFF_IN          \
        in_dword_masked(HWIO_GE_PCIE_MSI_CTRL_INT_4_EN_OFF_ADDR, HWIO_GE_PCIE_MSI_CTRL_INT_4_EN_OFF_RMSK)
#define HWIO_GE_PCIE_MSI_CTRL_INT_4_EN_OFF_INM(m)      \
        in_dword_masked(HWIO_GE_PCIE_MSI_CTRL_INT_4_EN_OFF_ADDR, m)
#define HWIO_GE_PCIE_MSI_CTRL_INT_4_EN_OFF_OUT(v)      \
        out_dword(HWIO_GE_PCIE_MSI_CTRL_INT_4_EN_OFF_ADDR,v)
#define HWIO_GE_PCIE_MSI_CTRL_INT_4_EN_OFF_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GE_PCIE_MSI_CTRL_INT_4_EN_OFF_ADDR,m,v,HWIO_GE_PCIE_MSI_CTRL_INT_4_EN_OFF_IN)
#define HWIO_GE_PCIE_MSI_CTRL_INT_4_EN_OFF_MSI_CTRL_INT_4_EN_BMSK                                          0xffffffff
#define HWIO_GE_PCIE_MSI_CTRL_INT_4_EN_OFF_MSI_CTRL_INT_4_EN_SHFT                                                 0x0

#define HWIO_GE_PCIE_MSI_CTRL_INT_4_MASK_OFF_ADDR                                                          (GE_PCIE_PCIE20_DBI_4_REG_BASE      + 0x0000085c)
#define HWIO_GE_PCIE_MSI_CTRL_INT_4_MASK_OFF_PHYS                                                          (GE_PCIE_PCIE20_DBI_4_REG_BASE_PHYS + 0x0000085c)
#define HWIO_GE_PCIE_MSI_CTRL_INT_4_MASK_OFF_OFFS                                                          (GE_PCIE_PCIE20_DBI_4_REG_BASE_OFFS + 0x0000085c)
#define HWIO_GE_PCIE_MSI_CTRL_INT_4_MASK_OFF_RMSK                                                          0xffffffff
#define HWIO_GE_PCIE_MSI_CTRL_INT_4_MASK_OFF_POR                                                           0x00000000
#define HWIO_GE_PCIE_MSI_CTRL_INT_4_MASK_OFF_POR_RMSK                                                      0xffffffff
#define HWIO_GE_PCIE_MSI_CTRL_INT_4_MASK_OFF_ATTR                                                                 0x3
#define HWIO_GE_PCIE_MSI_CTRL_INT_4_MASK_OFF_IN          \
        in_dword_masked(HWIO_GE_PCIE_MSI_CTRL_INT_4_MASK_OFF_ADDR, HWIO_GE_PCIE_MSI_CTRL_INT_4_MASK_OFF_RMSK)
#define HWIO_GE_PCIE_MSI_CTRL_INT_4_MASK_OFF_INM(m)      \
        in_dword_masked(HWIO_GE_PCIE_MSI_CTRL_INT_4_MASK_OFF_ADDR, m)
#define HWIO_GE_PCIE_MSI_CTRL_INT_4_MASK_OFF_OUT(v)      \
        out_dword(HWIO_GE_PCIE_MSI_CTRL_INT_4_MASK_OFF_ADDR,v)
#define HWIO_GE_PCIE_MSI_CTRL_INT_4_MASK_OFF_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GE_PCIE_MSI_CTRL_INT_4_MASK_OFF_ADDR,m,v,HWIO_GE_PCIE_MSI_CTRL_INT_4_MASK_OFF_IN)
#define HWIO_GE_PCIE_MSI_CTRL_INT_4_MASK_OFF_MSI_CTRL_INT_4_MASK_BMSK                                      0xffffffff
#define HWIO_GE_PCIE_MSI_CTRL_INT_4_MASK_OFF_MSI_CTRL_INT_4_MASK_SHFT                                             0x0

#define HWIO_GE_PCIE_MSI_CTRL_INT_4_STATUS_OFF_ADDR                                                        (GE_PCIE_PCIE20_DBI_4_REG_BASE      + 0x00000860)
#define HWIO_GE_PCIE_MSI_CTRL_INT_4_STATUS_OFF_PHYS                                                        (GE_PCIE_PCIE20_DBI_4_REG_BASE_PHYS + 0x00000860)
#define HWIO_GE_PCIE_MSI_CTRL_INT_4_STATUS_OFF_OFFS                                                        (GE_PCIE_PCIE20_DBI_4_REG_BASE_OFFS + 0x00000860)
#define HWIO_GE_PCIE_MSI_CTRL_INT_4_STATUS_OFF_RMSK                                                        0xffffffff
#define HWIO_GE_PCIE_MSI_CTRL_INT_4_STATUS_OFF_POR                                                         0x00000000
#define HWIO_GE_PCIE_MSI_CTRL_INT_4_STATUS_OFF_POR_RMSK                                                    0xffffffff
#define HWIO_GE_PCIE_MSI_CTRL_INT_4_STATUS_OFF_ATTR                                                               0x3
#define HWIO_GE_PCIE_MSI_CTRL_INT_4_STATUS_OFF_IN          \
        in_dword_masked(HWIO_GE_PCIE_MSI_CTRL_INT_4_STATUS_OFF_ADDR, HWIO_GE_PCIE_MSI_CTRL_INT_4_STATUS_OFF_RMSK)
#define HWIO_GE_PCIE_MSI_CTRL_INT_4_STATUS_OFF_INM(m)      \
        in_dword_masked(HWIO_GE_PCIE_MSI_CTRL_INT_4_STATUS_OFF_ADDR, m)
#define HWIO_GE_PCIE_MSI_CTRL_INT_4_STATUS_OFF_OUT(v)      \
        out_dword(HWIO_GE_PCIE_MSI_CTRL_INT_4_STATUS_OFF_ADDR,v)
#define HWIO_GE_PCIE_MSI_CTRL_INT_4_STATUS_OFF_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GE_PCIE_MSI_CTRL_INT_4_STATUS_OFF_ADDR,m,v,HWIO_GE_PCIE_MSI_CTRL_INT_4_STATUS_OFF_IN)
#define HWIO_GE_PCIE_MSI_CTRL_INT_4_STATUS_OFF_MSI_CTRL_INT_4_STATUS_BMSK                                  0xffffffff
#define HWIO_GE_PCIE_MSI_CTRL_INT_4_STATUS_OFF_MSI_CTRL_INT_4_STATUS_SHFT                                         0x0

#define HWIO_GE_PCIE_MSI_CTRL_INT_5_EN_OFF_ADDR                                                            (GE_PCIE_PCIE20_DBI_4_REG_BASE      + 0x00000864)
#define HWIO_GE_PCIE_MSI_CTRL_INT_5_EN_OFF_PHYS                                                            (GE_PCIE_PCIE20_DBI_4_REG_BASE_PHYS + 0x00000864)
#define HWIO_GE_PCIE_MSI_CTRL_INT_5_EN_OFF_OFFS                                                            (GE_PCIE_PCIE20_DBI_4_REG_BASE_OFFS + 0x00000864)
#define HWIO_GE_PCIE_MSI_CTRL_INT_5_EN_OFF_RMSK                                                            0xffffffff
#define HWIO_GE_PCIE_MSI_CTRL_INT_5_EN_OFF_POR                                                             0x00000000
#define HWIO_GE_PCIE_MSI_CTRL_INT_5_EN_OFF_POR_RMSK                                                        0xffffffff
#define HWIO_GE_PCIE_MSI_CTRL_INT_5_EN_OFF_ATTR                                                                   0x3
#define HWIO_GE_PCIE_MSI_CTRL_INT_5_EN_OFF_IN          \
        in_dword_masked(HWIO_GE_PCIE_MSI_CTRL_INT_5_EN_OFF_ADDR, HWIO_GE_PCIE_MSI_CTRL_INT_5_EN_OFF_RMSK)
#define HWIO_GE_PCIE_MSI_CTRL_INT_5_EN_OFF_INM(m)      \
        in_dword_masked(HWIO_GE_PCIE_MSI_CTRL_INT_5_EN_OFF_ADDR, m)
#define HWIO_GE_PCIE_MSI_CTRL_INT_5_EN_OFF_OUT(v)      \
        out_dword(HWIO_GE_PCIE_MSI_CTRL_INT_5_EN_OFF_ADDR,v)
#define HWIO_GE_PCIE_MSI_CTRL_INT_5_EN_OFF_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GE_PCIE_MSI_CTRL_INT_5_EN_OFF_ADDR,m,v,HWIO_GE_PCIE_MSI_CTRL_INT_5_EN_OFF_IN)
#define HWIO_GE_PCIE_MSI_CTRL_INT_5_EN_OFF_MSI_CTRL_INT_5_EN_BMSK                                          0xffffffff
#define HWIO_GE_PCIE_MSI_CTRL_INT_5_EN_OFF_MSI_CTRL_INT_5_EN_SHFT                                                 0x0

#define HWIO_GE_PCIE_MSI_CTRL_INT_5_MASK_OFF_ADDR                                                          (GE_PCIE_PCIE20_DBI_4_REG_BASE      + 0x00000868)
#define HWIO_GE_PCIE_MSI_CTRL_INT_5_MASK_OFF_PHYS                                                          (GE_PCIE_PCIE20_DBI_4_REG_BASE_PHYS + 0x00000868)
#define HWIO_GE_PCIE_MSI_CTRL_INT_5_MASK_OFF_OFFS                                                          (GE_PCIE_PCIE20_DBI_4_REG_BASE_OFFS + 0x00000868)
#define HWIO_GE_PCIE_MSI_CTRL_INT_5_MASK_OFF_RMSK                                                          0xffffffff
#define HWIO_GE_PCIE_MSI_CTRL_INT_5_MASK_OFF_POR                                                           0x00000000
#define HWIO_GE_PCIE_MSI_CTRL_INT_5_MASK_OFF_POR_RMSK                                                      0xffffffff
#define HWIO_GE_PCIE_MSI_CTRL_INT_5_MASK_OFF_ATTR                                                                 0x3
#define HWIO_GE_PCIE_MSI_CTRL_INT_5_MASK_OFF_IN          \
        in_dword_masked(HWIO_GE_PCIE_MSI_CTRL_INT_5_MASK_OFF_ADDR, HWIO_GE_PCIE_MSI_CTRL_INT_5_MASK_OFF_RMSK)
#define HWIO_GE_PCIE_MSI_CTRL_INT_5_MASK_OFF_INM(m)      \
        in_dword_masked(HWIO_GE_PCIE_MSI_CTRL_INT_5_MASK_OFF_ADDR, m)
#define HWIO_GE_PCIE_MSI_CTRL_INT_5_MASK_OFF_OUT(v)      \
        out_dword(HWIO_GE_PCIE_MSI_CTRL_INT_5_MASK_OFF_ADDR,v)
#define HWIO_GE_PCIE_MSI_CTRL_INT_5_MASK_OFF_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GE_PCIE_MSI_CTRL_INT_5_MASK_OFF_ADDR,m,v,HWIO_GE_PCIE_MSI_CTRL_INT_5_MASK_OFF_IN)
#define HWIO_GE_PCIE_MSI_CTRL_INT_5_MASK_OFF_MSI_CTRL_INT_5_MASK_BMSK                                      0xffffffff
#define HWIO_GE_PCIE_MSI_CTRL_INT_5_MASK_OFF_MSI_CTRL_INT_5_MASK_SHFT                                             0x0

#define HWIO_GE_PCIE_MSI_CTRL_INT_5_STATUS_OFF_ADDR                                                        (GE_PCIE_PCIE20_DBI_4_REG_BASE      + 0x0000086c)
#define HWIO_GE_PCIE_MSI_CTRL_INT_5_STATUS_OFF_PHYS                                                        (GE_PCIE_PCIE20_DBI_4_REG_BASE_PHYS + 0x0000086c)
#define HWIO_GE_PCIE_MSI_CTRL_INT_5_STATUS_OFF_OFFS                                                        (GE_PCIE_PCIE20_DBI_4_REG_BASE_OFFS + 0x0000086c)
#define HWIO_GE_PCIE_MSI_CTRL_INT_5_STATUS_OFF_RMSK                                                        0xffffffff
#define HWIO_GE_PCIE_MSI_CTRL_INT_5_STATUS_OFF_POR                                                         0x00000000
#define HWIO_GE_PCIE_MSI_CTRL_INT_5_STATUS_OFF_POR_RMSK                                                    0xffffffff
#define HWIO_GE_PCIE_MSI_CTRL_INT_5_STATUS_OFF_ATTR                                                               0x3
#define HWIO_GE_PCIE_MSI_CTRL_INT_5_STATUS_OFF_IN          \
        in_dword_masked(HWIO_GE_PCIE_MSI_CTRL_INT_5_STATUS_OFF_ADDR, HWIO_GE_PCIE_MSI_CTRL_INT_5_STATUS_OFF_RMSK)
#define HWIO_GE_PCIE_MSI_CTRL_INT_5_STATUS_OFF_INM(m)      \
        in_dword_masked(HWIO_GE_PCIE_MSI_CTRL_INT_5_STATUS_OFF_ADDR, m)
#define HWIO_GE_PCIE_MSI_CTRL_INT_5_STATUS_OFF_OUT(v)      \
        out_dword(HWIO_GE_PCIE_MSI_CTRL_INT_5_STATUS_OFF_ADDR,v)
#define HWIO_GE_PCIE_MSI_CTRL_INT_5_STATUS_OFF_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GE_PCIE_MSI_CTRL_INT_5_STATUS_OFF_ADDR,m,v,HWIO_GE_PCIE_MSI_CTRL_INT_5_STATUS_OFF_IN)
#define HWIO_GE_PCIE_MSI_CTRL_INT_5_STATUS_OFF_MSI_CTRL_INT_5_STATUS_BMSK                                  0xffffffff
#define HWIO_GE_PCIE_MSI_CTRL_INT_5_STATUS_OFF_MSI_CTRL_INT_5_STATUS_SHFT                                         0x0

#define HWIO_GE_PCIE_MSI_CTRL_INT_6_EN_OFF_ADDR                                                            (GE_PCIE_PCIE20_DBI_4_REG_BASE      + 0x00000870)
#define HWIO_GE_PCIE_MSI_CTRL_INT_6_EN_OFF_PHYS                                                            (GE_PCIE_PCIE20_DBI_4_REG_BASE_PHYS + 0x00000870)
#define HWIO_GE_PCIE_MSI_CTRL_INT_6_EN_OFF_OFFS                                                            (GE_PCIE_PCIE20_DBI_4_REG_BASE_OFFS + 0x00000870)
#define HWIO_GE_PCIE_MSI_CTRL_INT_6_EN_OFF_RMSK                                                            0xffffffff
#define HWIO_GE_PCIE_MSI_CTRL_INT_6_EN_OFF_POR                                                             0x00000000
#define HWIO_GE_PCIE_MSI_CTRL_INT_6_EN_OFF_POR_RMSK                                                        0xffffffff
#define HWIO_GE_PCIE_MSI_CTRL_INT_6_EN_OFF_ATTR                                                                   0x3
#define HWIO_GE_PCIE_MSI_CTRL_INT_6_EN_OFF_IN          \
        in_dword_masked(HWIO_GE_PCIE_MSI_CTRL_INT_6_EN_OFF_ADDR, HWIO_GE_PCIE_MSI_CTRL_INT_6_EN_OFF_RMSK)
#define HWIO_GE_PCIE_MSI_CTRL_INT_6_EN_OFF_INM(m)      \
        in_dword_masked(HWIO_GE_PCIE_MSI_CTRL_INT_6_EN_OFF_ADDR, m)
#define HWIO_GE_PCIE_MSI_CTRL_INT_6_EN_OFF_OUT(v)      \
        out_dword(HWIO_GE_PCIE_MSI_CTRL_INT_6_EN_OFF_ADDR,v)
#define HWIO_GE_PCIE_MSI_CTRL_INT_6_EN_OFF_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GE_PCIE_MSI_CTRL_INT_6_EN_OFF_ADDR,m,v,HWIO_GE_PCIE_MSI_CTRL_INT_6_EN_OFF_IN)
#define HWIO_GE_PCIE_MSI_CTRL_INT_6_EN_OFF_MSI_CTRL_INT_6_EN_BMSK                                          0xffffffff
#define HWIO_GE_PCIE_MSI_CTRL_INT_6_EN_OFF_MSI_CTRL_INT_6_EN_SHFT                                                 0x0

#define HWIO_GE_PCIE_MSI_CTRL_INT_6_MASK_OFF_ADDR                                                          (GE_PCIE_PCIE20_DBI_4_REG_BASE      + 0x00000874)
#define HWIO_GE_PCIE_MSI_CTRL_INT_6_MASK_OFF_PHYS                                                          (GE_PCIE_PCIE20_DBI_4_REG_BASE_PHYS + 0x00000874)
#define HWIO_GE_PCIE_MSI_CTRL_INT_6_MASK_OFF_OFFS                                                          (GE_PCIE_PCIE20_DBI_4_REG_BASE_OFFS + 0x00000874)
#define HWIO_GE_PCIE_MSI_CTRL_INT_6_MASK_OFF_RMSK                                                          0xffffffff
#define HWIO_GE_PCIE_MSI_CTRL_INT_6_MASK_OFF_POR                                                           0x00000000
#define HWIO_GE_PCIE_MSI_CTRL_INT_6_MASK_OFF_POR_RMSK                                                      0xffffffff
#define HWIO_GE_PCIE_MSI_CTRL_INT_6_MASK_OFF_ATTR                                                                 0x3
#define HWIO_GE_PCIE_MSI_CTRL_INT_6_MASK_OFF_IN          \
        in_dword_masked(HWIO_GE_PCIE_MSI_CTRL_INT_6_MASK_OFF_ADDR, HWIO_GE_PCIE_MSI_CTRL_INT_6_MASK_OFF_RMSK)
#define HWIO_GE_PCIE_MSI_CTRL_INT_6_MASK_OFF_INM(m)      \
        in_dword_masked(HWIO_GE_PCIE_MSI_CTRL_INT_6_MASK_OFF_ADDR, m)
#define HWIO_GE_PCIE_MSI_CTRL_INT_6_MASK_OFF_OUT(v)      \
        out_dword(HWIO_GE_PCIE_MSI_CTRL_INT_6_MASK_OFF_ADDR,v)
#define HWIO_GE_PCIE_MSI_CTRL_INT_6_MASK_OFF_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GE_PCIE_MSI_CTRL_INT_6_MASK_OFF_ADDR,m,v,HWIO_GE_PCIE_MSI_CTRL_INT_6_MASK_OFF_IN)
#define HWIO_GE_PCIE_MSI_CTRL_INT_6_MASK_OFF_MSI_CTRL_INT_6_MASK_BMSK                                      0xffffffff
#define HWIO_GE_PCIE_MSI_CTRL_INT_6_MASK_OFF_MSI_CTRL_INT_6_MASK_SHFT                                             0x0

#define HWIO_GE_PCIE_MSI_CTRL_INT_6_STATUS_OFF_ADDR                                                        (GE_PCIE_PCIE20_DBI_4_REG_BASE      + 0x00000878)
#define HWIO_GE_PCIE_MSI_CTRL_INT_6_STATUS_OFF_PHYS                                                        (GE_PCIE_PCIE20_DBI_4_REG_BASE_PHYS + 0x00000878)
#define HWIO_GE_PCIE_MSI_CTRL_INT_6_STATUS_OFF_OFFS                                                        (GE_PCIE_PCIE20_DBI_4_REG_BASE_OFFS + 0x00000878)
#define HWIO_GE_PCIE_MSI_CTRL_INT_6_STATUS_OFF_RMSK                                                        0xffffffff
#define HWIO_GE_PCIE_MSI_CTRL_INT_6_STATUS_OFF_POR                                                         0x00000000
#define HWIO_GE_PCIE_MSI_CTRL_INT_6_STATUS_OFF_POR_RMSK                                                    0xffffffff
#define HWIO_GE_PCIE_MSI_CTRL_INT_6_STATUS_OFF_ATTR                                                               0x3
#define HWIO_GE_PCIE_MSI_CTRL_INT_6_STATUS_OFF_IN          \
        in_dword_masked(HWIO_GE_PCIE_MSI_CTRL_INT_6_STATUS_OFF_ADDR, HWIO_GE_PCIE_MSI_CTRL_INT_6_STATUS_OFF_RMSK)
#define HWIO_GE_PCIE_MSI_CTRL_INT_6_STATUS_OFF_INM(m)      \
        in_dword_masked(HWIO_GE_PCIE_MSI_CTRL_INT_6_STATUS_OFF_ADDR, m)
#define HWIO_GE_PCIE_MSI_CTRL_INT_6_STATUS_OFF_OUT(v)      \
        out_dword(HWIO_GE_PCIE_MSI_CTRL_INT_6_STATUS_OFF_ADDR,v)
#define HWIO_GE_PCIE_MSI_CTRL_INT_6_STATUS_OFF_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GE_PCIE_MSI_CTRL_INT_6_STATUS_OFF_ADDR,m,v,HWIO_GE_PCIE_MSI_CTRL_INT_6_STATUS_OFF_IN)
#define HWIO_GE_PCIE_MSI_CTRL_INT_6_STATUS_OFF_MSI_CTRL_INT_6_STATUS_BMSK                                  0xffffffff
#define HWIO_GE_PCIE_MSI_CTRL_INT_6_STATUS_OFF_MSI_CTRL_INT_6_STATUS_SHFT                                         0x0

#define HWIO_GE_PCIE_MSI_CTRL_INT_7_EN_OFF_ADDR                                                            (GE_PCIE_PCIE20_DBI_4_REG_BASE      + 0x0000087c)
#define HWIO_GE_PCIE_MSI_CTRL_INT_7_EN_OFF_PHYS                                                            (GE_PCIE_PCIE20_DBI_4_REG_BASE_PHYS + 0x0000087c)
#define HWIO_GE_PCIE_MSI_CTRL_INT_7_EN_OFF_OFFS                                                            (GE_PCIE_PCIE20_DBI_4_REG_BASE_OFFS + 0x0000087c)
#define HWIO_GE_PCIE_MSI_CTRL_INT_7_EN_OFF_RMSK                                                            0xffffffff
#define HWIO_GE_PCIE_MSI_CTRL_INT_7_EN_OFF_POR                                                             0x00000000
#define HWIO_GE_PCIE_MSI_CTRL_INT_7_EN_OFF_POR_RMSK                                                        0xffffffff
#define HWIO_GE_PCIE_MSI_CTRL_INT_7_EN_OFF_ATTR                                                                   0x3
#define HWIO_GE_PCIE_MSI_CTRL_INT_7_EN_OFF_IN          \
        in_dword_masked(HWIO_GE_PCIE_MSI_CTRL_INT_7_EN_OFF_ADDR, HWIO_GE_PCIE_MSI_CTRL_INT_7_EN_OFF_RMSK)
#define HWIO_GE_PCIE_MSI_CTRL_INT_7_EN_OFF_INM(m)      \
        in_dword_masked(HWIO_GE_PCIE_MSI_CTRL_INT_7_EN_OFF_ADDR, m)
#define HWIO_GE_PCIE_MSI_CTRL_INT_7_EN_OFF_OUT(v)      \
        out_dword(HWIO_GE_PCIE_MSI_CTRL_INT_7_EN_OFF_ADDR,v)
#define HWIO_GE_PCIE_MSI_CTRL_INT_7_EN_OFF_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GE_PCIE_MSI_CTRL_INT_7_EN_OFF_ADDR,m,v,HWIO_GE_PCIE_MSI_CTRL_INT_7_EN_OFF_IN)
#define HWIO_GE_PCIE_MSI_CTRL_INT_7_EN_OFF_MSI_CTRL_INT_7_EN_BMSK                                          0xffffffff
#define HWIO_GE_PCIE_MSI_CTRL_INT_7_EN_OFF_MSI_CTRL_INT_7_EN_SHFT                                                 0x0

#define HWIO_GE_PCIE_MSI_CTRL_INT_7_MASK_OFF_ADDR                                                          (GE_PCIE_PCIE20_DBI_4_REG_BASE      + 0x00000880)
#define HWIO_GE_PCIE_MSI_CTRL_INT_7_MASK_OFF_PHYS                                                          (GE_PCIE_PCIE20_DBI_4_REG_BASE_PHYS + 0x00000880)
#define HWIO_GE_PCIE_MSI_CTRL_INT_7_MASK_OFF_OFFS                                                          (GE_PCIE_PCIE20_DBI_4_REG_BASE_OFFS + 0x00000880)
#define HWIO_GE_PCIE_MSI_CTRL_INT_7_MASK_OFF_RMSK                                                          0xffffffff
#define HWIO_GE_PCIE_MSI_CTRL_INT_7_MASK_OFF_POR                                                           0x00000000
#define HWIO_GE_PCIE_MSI_CTRL_INT_7_MASK_OFF_POR_RMSK                                                      0xffffffff
#define HWIO_GE_PCIE_MSI_CTRL_INT_7_MASK_OFF_ATTR                                                                 0x3
#define HWIO_GE_PCIE_MSI_CTRL_INT_7_MASK_OFF_IN          \
        in_dword_masked(HWIO_GE_PCIE_MSI_CTRL_INT_7_MASK_OFF_ADDR, HWIO_GE_PCIE_MSI_CTRL_INT_7_MASK_OFF_RMSK)
#define HWIO_GE_PCIE_MSI_CTRL_INT_7_MASK_OFF_INM(m)      \
        in_dword_masked(HWIO_GE_PCIE_MSI_CTRL_INT_7_MASK_OFF_ADDR, m)
#define HWIO_GE_PCIE_MSI_CTRL_INT_7_MASK_OFF_OUT(v)      \
        out_dword(HWIO_GE_PCIE_MSI_CTRL_INT_7_MASK_OFF_ADDR,v)
#define HWIO_GE_PCIE_MSI_CTRL_INT_7_MASK_OFF_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GE_PCIE_MSI_CTRL_INT_7_MASK_OFF_ADDR,m,v,HWIO_GE_PCIE_MSI_CTRL_INT_7_MASK_OFF_IN)
#define HWIO_GE_PCIE_MSI_CTRL_INT_7_MASK_OFF_MSI_CTRL_INT_7_MASK_BMSK                                      0xffffffff
#define HWIO_GE_PCIE_MSI_CTRL_INT_7_MASK_OFF_MSI_CTRL_INT_7_MASK_SHFT                                             0x0

#define HWIO_GE_PCIE_MSI_CTRL_INT_7_STATUS_OFF_ADDR                                                        (GE_PCIE_PCIE20_DBI_4_REG_BASE      + 0x00000884)
#define HWIO_GE_PCIE_MSI_CTRL_INT_7_STATUS_OFF_PHYS                                                        (GE_PCIE_PCIE20_DBI_4_REG_BASE_PHYS + 0x00000884)
#define HWIO_GE_PCIE_MSI_CTRL_INT_7_STATUS_OFF_OFFS                                                        (GE_PCIE_PCIE20_DBI_4_REG_BASE_OFFS + 0x00000884)
#define HWIO_GE_PCIE_MSI_CTRL_INT_7_STATUS_OFF_RMSK                                                        0xffffffff
#define HWIO_GE_PCIE_MSI_CTRL_INT_7_STATUS_OFF_POR                                                         0x00000000
#define HWIO_GE_PCIE_MSI_CTRL_INT_7_STATUS_OFF_POR_RMSK                                                    0xffffffff
#define HWIO_GE_PCIE_MSI_CTRL_INT_7_STATUS_OFF_ATTR                                                               0x3
#define HWIO_GE_PCIE_MSI_CTRL_INT_7_STATUS_OFF_IN          \
        in_dword_masked(HWIO_GE_PCIE_MSI_CTRL_INT_7_STATUS_OFF_ADDR, HWIO_GE_PCIE_MSI_CTRL_INT_7_STATUS_OFF_RMSK)
#define HWIO_GE_PCIE_MSI_CTRL_INT_7_STATUS_OFF_INM(m)      \
        in_dword_masked(HWIO_GE_PCIE_MSI_CTRL_INT_7_STATUS_OFF_ADDR, m)
#define HWIO_GE_PCIE_MSI_CTRL_INT_7_STATUS_OFF_OUT(v)      \
        out_dword(HWIO_GE_PCIE_MSI_CTRL_INT_7_STATUS_OFF_ADDR,v)
#define HWIO_GE_PCIE_MSI_CTRL_INT_7_STATUS_OFF_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GE_PCIE_MSI_CTRL_INT_7_STATUS_OFF_ADDR,m,v,HWIO_GE_PCIE_MSI_CTRL_INT_7_STATUS_OFF_IN)
#define HWIO_GE_PCIE_MSI_CTRL_INT_7_STATUS_OFF_MSI_CTRL_INT_7_STATUS_BMSK                                  0xffffffff
#define HWIO_GE_PCIE_MSI_CTRL_INT_7_STATUS_OFF_MSI_CTRL_INT_7_STATUS_SHFT                                         0x0

#define HWIO_GE_PCIE_MSI_GPIO_IO_OFF_ADDR                                                                  (GE_PCIE_PCIE20_DBI_4_REG_BASE      + 0x00000888)
#define HWIO_GE_PCIE_MSI_GPIO_IO_OFF_PHYS                                                                  (GE_PCIE_PCIE20_DBI_4_REG_BASE_PHYS + 0x00000888)
#define HWIO_GE_PCIE_MSI_GPIO_IO_OFF_OFFS                                                                  (GE_PCIE_PCIE20_DBI_4_REG_BASE_OFFS + 0x00000888)
#define HWIO_GE_PCIE_MSI_GPIO_IO_OFF_RMSK                                                                  0xffffffff
#define HWIO_GE_PCIE_MSI_GPIO_IO_OFF_POR                                                                   0x00000000
#define HWIO_GE_PCIE_MSI_GPIO_IO_OFF_POR_RMSK                                                              0xffffffff
#define HWIO_GE_PCIE_MSI_GPIO_IO_OFF_ATTR                                                                         0x3
#define HWIO_GE_PCIE_MSI_GPIO_IO_OFF_IN          \
        in_dword_masked(HWIO_GE_PCIE_MSI_GPIO_IO_OFF_ADDR, HWIO_GE_PCIE_MSI_GPIO_IO_OFF_RMSK)
#define HWIO_GE_PCIE_MSI_GPIO_IO_OFF_INM(m)      \
        in_dword_masked(HWIO_GE_PCIE_MSI_GPIO_IO_OFF_ADDR, m)
#define HWIO_GE_PCIE_MSI_GPIO_IO_OFF_OUT(v)      \
        out_dword(HWIO_GE_PCIE_MSI_GPIO_IO_OFF_ADDR,v)
#define HWIO_GE_PCIE_MSI_GPIO_IO_OFF_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GE_PCIE_MSI_GPIO_IO_OFF_ADDR,m,v,HWIO_GE_PCIE_MSI_GPIO_IO_OFF_IN)
#define HWIO_GE_PCIE_MSI_GPIO_IO_OFF_MSI_GPIO_REG_BMSK                                                     0xffffffff
#define HWIO_GE_PCIE_MSI_GPIO_IO_OFF_MSI_GPIO_REG_SHFT                                                            0x0

#define HWIO_GE_PCIE_PIPE_LOOPBACK_CONTROL_OFF_ADDR                                                        (GE_PCIE_PCIE20_DBI_4_REG_BASE      + 0x000008b8)
#define HWIO_GE_PCIE_PIPE_LOOPBACK_CONTROL_OFF_PHYS                                                        (GE_PCIE_PCIE20_DBI_4_REG_BASE_PHYS + 0x000008b8)
#define HWIO_GE_PCIE_PIPE_LOOPBACK_CONTROL_OFF_OFFS                                                        (GE_PCIE_PCIE20_DBI_4_REG_BASE_OFFS + 0x000008b8)
#define HWIO_GE_PCIE_PIPE_LOOPBACK_CONTROL_OFF_RMSK                                                        0xffffffff
#define HWIO_GE_PCIE_PIPE_LOOPBACK_CONTROL_OFF_POR                                                         0x00000000
#define HWIO_GE_PCIE_PIPE_LOOPBACK_CONTROL_OFF_POR_RMSK                                                    0xffffffff
#define HWIO_GE_PCIE_PIPE_LOOPBACK_CONTROL_OFF_ATTR                                                               0x3
#define HWIO_GE_PCIE_PIPE_LOOPBACK_CONTROL_OFF_IN          \
        in_dword_masked(HWIO_GE_PCIE_PIPE_LOOPBACK_CONTROL_OFF_ADDR, HWIO_GE_PCIE_PIPE_LOOPBACK_CONTROL_OFF_RMSK)
#define HWIO_GE_PCIE_PIPE_LOOPBACK_CONTROL_OFF_INM(m)      \
        in_dword_masked(HWIO_GE_PCIE_PIPE_LOOPBACK_CONTROL_OFF_ADDR, m)
#define HWIO_GE_PCIE_PIPE_LOOPBACK_CONTROL_OFF_OUT(v)      \
        out_dword(HWIO_GE_PCIE_PIPE_LOOPBACK_CONTROL_OFF_ADDR,v)
#define HWIO_GE_PCIE_PIPE_LOOPBACK_CONTROL_OFF_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GE_PCIE_PIPE_LOOPBACK_CONTROL_OFF_ADDR,m,v,HWIO_GE_PCIE_PIPE_LOOPBACK_CONTROL_OFF_IN)
#define HWIO_GE_PCIE_PIPE_LOOPBACK_CONTROL_OFF_PIPE_LOOPBACK_BMSK                                          0x80000000
#define HWIO_GE_PCIE_PIPE_LOOPBACK_CONTROL_OFF_PIPE_LOOPBACK_SHFT                                                0x1f
#define HWIO_GE_PCIE_PIPE_LOOPBACK_CONTROL_OFF_RSVDP_27_BMSK                                               0x78000000
#define HWIO_GE_PCIE_PIPE_LOOPBACK_CONTROL_OFF_RSVDP_27_SHFT                                                     0x1b
#define HWIO_GE_PCIE_PIPE_LOOPBACK_CONTROL_OFF_RXSTATUS_VALUE_BMSK                                          0x7000000
#define HWIO_GE_PCIE_PIPE_LOOPBACK_CONTROL_OFF_RXSTATUS_VALUE_SHFT                                               0x18
#define HWIO_GE_PCIE_PIPE_LOOPBACK_CONTROL_OFF_RSVDP_22_BMSK                                                 0xc00000
#define HWIO_GE_PCIE_PIPE_LOOPBACK_CONTROL_OFF_RSVDP_22_SHFT                                                     0x16
#define HWIO_GE_PCIE_PIPE_LOOPBACK_CONTROL_OFF_RXSTATUS_LANE_BMSK                                            0x3f0000
#define HWIO_GE_PCIE_PIPE_LOOPBACK_CONTROL_OFF_RXSTATUS_LANE_SHFT                                                0x10
#define HWIO_GE_PCIE_PIPE_LOOPBACK_CONTROL_OFF_LPBK_RXVALID_BMSK                                               0xffff
#define HWIO_GE_PCIE_PIPE_LOOPBACK_CONTROL_OFF_LPBK_RXVALID_SHFT                                                  0x0

#define HWIO_GE_PCIE_MISC_CONTROL_1_OFF_ADDR                                                               (GE_PCIE_PCIE20_DBI_4_REG_BASE      + 0x000008bc)
#define HWIO_GE_PCIE_MISC_CONTROL_1_OFF_PHYS                                                               (GE_PCIE_PCIE20_DBI_4_REG_BASE_PHYS + 0x000008bc)
#define HWIO_GE_PCIE_MISC_CONTROL_1_OFF_OFFS                                                               (GE_PCIE_PCIE20_DBI_4_REG_BASE_OFFS + 0x000008bc)
#define HWIO_GE_PCIE_MISC_CONTROL_1_OFF_RMSK                                                               0xffffffff
#define HWIO_GE_PCIE_MISC_CONTROL_1_OFF_POR                                                                0x00000000
#define HWIO_GE_PCIE_MISC_CONTROL_1_OFF_POR_RMSK                                                           0xffffffff
#define HWIO_GE_PCIE_MISC_CONTROL_1_OFF_ATTR                                                                      0x3
#define HWIO_GE_PCIE_MISC_CONTROL_1_OFF_IN          \
        in_dword_masked(HWIO_GE_PCIE_MISC_CONTROL_1_OFF_ADDR, HWIO_GE_PCIE_MISC_CONTROL_1_OFF_RMSK)
#define HWIO_GE_PCIE_MISC_CONTROL_1_OFF_INM(m)      \
        in_dword_masked(HWIO_GE_PCIE_MISC_CONTROL_1_OFF_ADDR, m)
#define HWIO_GE_PCIE_MISC_CONTROL_1_OFF_OUT(v)      \
        out_dword(HWIO_GE_PCIE_MISC_CONTROL_1_OFF_ADDR,v)
#define HWIO_GE_PCIE_MISC_CONTROL_1_OFF_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GE_PCIE_MISC_CONTROL_1_OFF_ADDR,m,v,HWIO_GE_PCIE_MISC_CONTROL_1_OFF_IN)
#define HWIO_GE_PCIE_MISC_CONTROL_1_OFF_RSVDP_1_BMSK                                                       0xfffffffe
#define HWIO_GE_PCIE_MISC_CONTROL_1_OFF_RSVDP_1_SHFT                                                              0x1
#define HWIO_GE_PCIE_MISC_CONTROL_1_OFF_DBI_RO_WR_EN_BMSK                                                         0x1
#define HWIO_GE_PCIE_MISC_CONTROL_1_OFF_DBI_RO_WR_EN_SHFT                                                         0x0

#define HWIO_GE_PCIE_LINK_FLUSH_CONTROL_OFF_ADDR                                                           (GE_PCIE_PCIE20_DBI_4_REG_BASE      + 0x000008cc)
#define HWIO_GE_PCIE_LINK_FLUSH_CONTROL_OFF_PHYS                                                           (GE_PCIE_PCIE20_DBI_4_REG_BASE_PHYS + 0x000008cc)
#define HWIO_GE_PCIE_LINK_FLUSH_CONTROL_OFF_OFFS                                                           (GE_PCIE_PCIE20_DBI_4_REG_BASE_OFFS + 0x000008cc)
#define HWIO_GE_PCIE_LINK_FLUSH_CONTROL_OFF_RMSK                                                           0xffffffff
#define HWIO_GE_PCIE_LINK_FLUSH_CONTROL_OFF_POR                                                            0x00000000
#define HWIO_GE_PCIE_LINK_FLUSH_CONTROL_OFF_POR_RMSK                                                       0xffffffff
#define HWIO_GE_PCIE_LINK_FLUSH_CONTROL_OFF_ATTR                                                                  0x3
#define HWIO_GE_PCIE_LINK_FLUSH_CONTROL_OFF_IN          \
        in_dword_masked(HWIO_GE_PCIE_LINK_FLUSH_CONTROL_OFF_ADDR, HWIO_GE_PCIE_LINK_FLUSH_CONTROL_OFF_RMSK)
#define HWIO_GE_PCIE_LINK_FLUSH_CONTROL_OFF_INM(m)      \
        in_dword_masked(HWIO_GE_PCIE_LINK_FLUSH_CONTROL_OFF_ADDR, m)
#define HWIO_GE_PCIE_LINK_FLUSH_CONTROL_OFF_OUT(v)      \
        out_dword(HWIO_GE_PCIE_LINK_FLUSH_CONTROL_OFF_ADDR,v)
#define HWIO_GE_PCIE_LINK_FLUSH_CONTROL_OFF_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GE_PCIE_LINK_FLUSH_CONTROL_OFF_ADDR,m,v,HWIO_GE_PCIE_LINK_FLUSH_CONTROL_OFF_IN)
#define HWIO_GE_PCIE_LINK_FLUSH_CONTROL_OFF_RSVD_I_8_BMSK                                                  0xff000000
#define HWIO_GE_PCIE_LINK_FLUSH_CONTROL_OFF_RSVD_I_8_SHFT                                                        0x18
#define HWIO_GE_PCIE_LINK_FLUSH_CONTROL_OFF_RSVDP_1_BMSK                                                     0xfffffe
#define HWIO_GE_PCIE_LINK_FLUSH_CONTROL_OFF_RSVDP_1_SHFT                                                          0x1
#define HWIO_GE_PCIE_LINK_FLUSH_CONTROL_OFF_AUTO_FLUSH_EN_BMSK                                                    0x1
#define HWIO_GE_PCIE_LINK_FLUSH_CONTROL_OFF_AUTO_FLUSH_EN_SHFT                                                    0x0

#define HWIO_GE_PCIE_AMBA_ERROR_RESPONSE_DEFAULT_OFF_ADDR                                                  (GE_PCIE_PCIE20_DBI_4_REG_BASE      + 0x000008d0)
#define HWIO_GE_PCIE_AMBA_ERROR_RESPONSE_DEFAULT_OFF_PHYS                                                  (GE_PCIE_PCIE20_DBI_4_REG_BASE_PHYS + 0x000008d0)
#define HWIO_GE_PCIE_AMBA_ERROR_RESPONSE_DEFAULT_OFF_OFFS                                                  (GE_PCIE_PCIE20_DBI_4_REG_BASE_OFFS + 0x000008d0)
#define HWIO_GE_PCIE_AMBA_ERROR_RESPONSE_DEFAULT_OFF_RMSK                                                  0xffffffff
#define HWIO_GE_PCIE_AMBA_ERROR_RESPONSE_DEFAULT_OFF_POR                                                   0x00000000
#define HWIO_GE_PCIE_AMBA_ERROR_RESPONSE_DEFAULT_OFF_POR_RMSK                                              0xffffffff
#define HWIO_GE_PCIE_AMBA_ERROR_RESPONSE_DEFAULT_OFF_ATTR                                                         0x3
#define HWIO_GE_PCIE_AMBA_ERROR_RESPONSE_DEFAULT_OFF_IN          \
        in_dword_masked(HWIO_GE_PCIE_AMBA_ERROR_RESPONSE_DEFAULT_OFF_ADDR, HWIO_GE_PCIE_AMBA_ERROR_RESPONSE_DEFAULT_OFF_RMSK)
#define HWIO_GE_PCIE_AMBA_ERROR_RESPONSE_DEFAULT_OFF_INM(m)      \
        in_dword_masked(HWIO_GE_PCIE_AMBA_ERROR_RESPONSE_DEFAULT_OFF_ADDR, m)
#define HWIO_GE_PCIE_AMBA_ERROR_RESPONSE_DEFAULT_OFF_OUT(v)      \
        out_dword(HWIO_GE_PCIE_AMBA_ERROR_RESPONSE_DEFAULT_OFF_ADDR,v)
#define HWIO_GE_PCIE_AMBA_ERROR_RESPONSE_DEFAULT_OFF_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GE_PCIE_AMBA_ERROR_RESPONSE_DEFAULT_OFF_ADDR,m,v,HWIO_GE_PCIE_AMBA_ERROR_RESPONSE_DEFAULT_OFF_IN)
#define HWIO_GE_PCIE_AMBA_ERROR_RESPONSE_DEFAULT_OFF_RSVDP_16_BMSK                                         0xffff0000
#define HWIO_GE_PCIE_AMBA_ERROR_RESPONSE_DEFAULT_OFF_RSVDP_16_SHFT                                               0x10
#define HWIO_GE_PCIE_AMBA_ERROR_RESPONSE_DEFAULT_OFF_AMBA_ERROR_RESPONSE_MAP_BMSK                              0xfc00
#define HWIO_GE_PCIE_AMBA_ERROR_RESPONSE_DEFAULT_OFF_AMBA_ERROR_RESPONSE_MAP_SHFT                                 0xa
#define HWIO_GE_PCIE_AMBA_ERROR_RESPONSE_DEFAULT_OFF_RSVDP_3_BMSK                                               0x3f8
#define HWIO_GE_PCIE_AMBA_ERROR_RESPONSE_DEFAULT_OFF_RSVDP_3_SHFT                                                 0x3
#define HWIO_GE_PCIE_AMBA_ERROR_RESPONSE_DEFAULT_OFF_AMBA_ERROR_RESPONSE_VENDORID_BMSK                            0x4
#define HWIO_GE_PCIE_AMBA_ERROR_RESPONSE_DEFAULT_OFF_AMBA_ERROR_RESPONSE_VENDORID_SHFT                            0x2
#define HWIO_GE_PCIE_AMBA_ERROR_RESPONSE_DEFAULT_OFF_RSVDP_1_BMSK                                                 0x2
#define HWIO_GE_PCIE_AMBA_ERROR_RESPONSE_DEFAULT_OFF_RSVDP_1_SHFT                                                 0x1
#define HWIO_GE_PCIE_AMBA_ERROR_RESPONSE_DEFAULT_OFF_AMBA_ERROR_RESPONSE_GLOBAL_BMSK                              0x1
#define HWIO_GE_PCIE_AMBA_ERROR_RESPONSE_DEFAULT_OFF_AMBA_ERROR_RESPONSE_GLOBAL_SHFT                              0x0

#define HWIO_GE_PCIE_AMBA_LINK_TIMEOUT_OFF_ADDR                                                            (GE_PCIE_PCIE20_DBI_4_REG_BASE      + 0x000008d4)
#define HWIO_GE_PCIE_AMBA_LINK_TIMEOUT_OFF_PHYS                                                            (GE_PCIE_PCIE20_DBI_4_REG_BASE_PHYS + 0x000008d4)
#define HWIO_GE_PCIE_AMBA_LINK_TIMEOUT_OFF_OFFS                                                            (GE_PCIE_PCIE20_DBI_4_REG_BASE_OFFS + 0x000008d4)
#define HWIO_GE_PCIE_AMBA_LINK_TIMEOUT_OFF_RMSK                                                            0xffffffff
#define HWIO_GE_PCIE_AMBA_LINK_TIMEOUT_OFF_POR                                                             0x00000000
#define HWIO_GE_PCIE_AMBA_LINK_TIMEOUT_OFF_POR_RMSK                                                        0xffffffff
#define HWIO_GE_PCIE_AMBA_LINK_TIMEOUT_OFF_ATTR                                                                   0x3
#define HWIO_GE_PCIE_AMBA_LINK_TIMEOUT_OFF_IN          \
        in_dword_masked(HWIO_GE_PCIE_AMBA_LINK_TIMEOUT_OFF_ADDR, HWIO_GE_PCIE_AMBA_LINK_TIMEOUT_OFF_RMSK)
#define HWIO_GE_PCIE_AMBA_LINK_TIMEOUT_OFF_INM(m)      \
        in_dword_masked(HWIO_GE_PCIE_AMBA_LINK_TIMEOUT_OFF_ADDR, m)
#define HWIO_GE_PCIE_AMBA_LINK_TIMEOUT_OFF_OUT(v)      \
        out_dword(HWIO_GE_PCIE_AMBA_LINK_TIMEOUT_OFF_ADDR,v)
#define HWIO_GE_PCIE_AMBA_LINK_TIMEOUT_OFF_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GE_PCIE_AMBA_LINK_TIMEOUT_OFF_ADDR,m,v,HWIO_GE_PCIE_AMBA_LINK_TIMEOUT_OFF_IN)
#define HWIO_GE_PCIE_AMBA_LINK_TIMEOUT_OFF_RSVDP_9_BMSK                                                    0xfffffe00
#define HWIO_GE_PCIE_AMBA_LINK_TIMEOUT_OFF_RSVDP_9_SHFT                                                           0x9
#define HWIO_GE_PCIE_AMBA_LINK_TIMEOUT_OFF_LINK_TIMEOUT_ENABLE_DEFAULT_BMSK                                     0x100
#define HWIO_GE_PCIE_AMBA_LINK_TIMEOUT_OFF_LINK_TIMEOUT_ENABLE_DEFAULT_SHFT                                       0x8
#define HWIO_GE_PCIE_AMBA_LINK_TIMEOUT_OFF_LINK_TIMEOUT_PERIOD_DEFAULT_BMSK                                      0xff
#define HWIO_GE_PCIE_AMBA_LINK_TIMEOUT_OFF_LINK_TIMEOUT_PERIOD_DEFAULT_SHFT                                       0x0

#define HWIO_GE_PCIE_AMBA_ORDERING_CTRL_OFF_ADDR                                                           (GE_PCIE_PCIE20_DBI_4_REG_BASE      + 0x000008d8)
#define HWIO_GE_PCIE_AMBA_ORDERING_CTRL_OFF_PHYS                                                           (GE_PCIE_PCIE20_DBI_4_REG_BASE_PHYS + 0x000008d8)
#define HWIO_GE_PCIE_AMBA_ORDERING_CTRL_OFF_OFFS                                                           (GE_PCIE_PCIE20_DBI_4_REG_BASE_OFFS + 0x000008d8)
#define HWIO_GE_PCIE_AMBA_ORDERING_CTRL_OFF_RMSK                                                           0xffffffff
#define HWIO_GE_PCIE_AMBA_ORDERING_CTRL_OFF_POR                                                            0x00000000
#define HWIO_GE_PCIE_AMBA_ORDERING_CTRL_OFF_POR_RMSK                                                       0xffffffff
#define HWIO_GE_PCIE_AMBA_ORDERING_CTRL_OFF_ATTR                                                                  0x3
#define HWIO_GE_PCIE_AMBA_ORDERING_CTRL_OFF_IN          \
        in_dword_masked(HWIO_GE_PCIE_AMBA_ORDERING_CTRL_OFF_ADDR, HWIO_GE_PCIE_AMBA_ORDERING_CTRL_OFF_RMSK)
#define HWIO_GE_PCIE_AMBA_ORDERING_CTRL_OFF_INM(m)      \
        in_dword_masked(HWIO_GE_PCIE_AMBA_ORDERING_CTRL_OFF_ADDR, m)
#define HWIO_GE_PCIE_AMBA_ORDERING_CTRL_OFF_OUT(v)      \
        out_dword(HWIO_GE_PCIE_AMBA_ORDERING_CTRL_OFF_ADDR,v)
#define HWIO_GE_PCIE_AMBA_ORDERING_CTRL_OFF_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GE_PCIE_AMBA_ORDERING_CTRL_OFF_ADDR,m,v,HWIO_GE_PCIE_AMBA_ORDERING_CTRL_OFF_IN)
#define HWIO_GE_PCIE_AMBA_ORDERING_CTRL_OFF_RSVDP_2_BMSK                                                   0xfffffffc
#define HWIO_GE_PCIE_AMBA_ORDERING_CTRL_OFF_RSVDP_2_SHFT                                                          0x2
#define HWIO_GE_PCIE_AMBA_ORDERING_CTRL_OFF_AX_SNP_EN_BMSK                                                        0x2
#define HWIO_GE_PCIE_AMBA_ORDERING_CTRL_OFF_AX_SNP_EN_SHFT                                                        0x1
#define HWIO_GE_PCIE_AMBA_ORDERING_CTRL_OFF_AX_MSTR_NP_PASS_P_BMSK                                                0x1
#define HWIO_GE_PCIE_AMBA_ORDERING_CTRL_OFF_AX_MSTR_NP_PASS_P_SHFT                                                0x0

#define HWIO_GE_PCIE_COHERENCY_CONTROL_1_OFF_ADDR                                                          (GE_PCIE_PCIE20_DBI_4_REG_BASE      + 0x000008e0)
#define HWIO_GE_PCIE_COHERENCY_CONTROL_1_OFF_PHYS                                                          (GE_PCIE_PCIE20_DBI_4_REG_BASE_PHYS + 0x000008e0)
#define HWIO_GE_PCIE_COHERENCY_CONTROL_1_OFF_OFFS                                                          (GE_PCIE_PCIE20_DBI_4_REG_BASE_OFFS + 0x000008e0)
#define HWIO_GE_PCIE_COHERENCY_CONTROL_1_OFF_RMSK                                                          0xffffffff
#define HWIO_GE_PCIE_COHERENCY_CONTROL_1_OFF_POR                                                           0x00000000
#define HWIO_GE_PCIE_COHERENCY_CONTROL_1_OFF_POR_RMSK                                                      0xffffffff
#define HWIO_GE_PCIE_COHERENCY_CONTROL_1_OFF_ATTR                                                                 0x3
#define HWIO_GE_PCIE_COHERENCY_CONTROL_1_OFF_IN          \
        in_dword_masked(HWIO_GE_PCIE_COHERENCY_CONTROL_1_OFF_ADDR, HWIO_GE_PCIE_COHERENCY_CONTROL_1_OFF_RMSK)
#define HWIO_GE_PCIE_COHERENCY_CONTROL_1_OFF_INM(m)      \
        in_dword_masked(HWIO_GE_PCIE_COHERENCY_CONTROL_1_OFF_ADDR, m)
#define HWIO_GE_PCIE_COHERENCY_CONTROL_1_OFF_OUT(v)      \
        out_dword(HWIO_GE_PCIE_COHERENCY_CONTROL_1_OFF_ADDR,v)
#define HWIO_GE_PCIE_COHERENCY_CONTROL_1_OFF_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GE_PCIE_COHERENCY_CONTROL_1_OFF_ADDR,m,v,HWIO_GE_PCIE_COHERENCY_CONTROL_1_OFF_IN)
#define HWIO_GE_PCIE_COHERENCY_CONTROL_1_OFF_CFG_MEMTYPE_BOUNDARY_LOW_ADDR_BMSK                            0xfffffffc
#define HWIO_GE_PCIE_COHERENCY_CONTROL_1_OFF_CFG_MEMTYPE_BOUNDARY_LOW_ADDR_SHFT                                   0x2
#define HWIO_GE_PCIE_COHERENCY_CONTROL_1_OFF_RSVDP_1_BMSK                                                         0x2
#define HWIO_GE_PCIE_COHERENCY_CONTROL_1_OFF_RSVDP_1_SHFT                                                         0x1
#define HWIO_GE_PCIE_COHERENCY_CONTROL_1_OFF_CFG_MEMTYPE_VALUE_BMSK                                               0x1
#define HWIO_GE_PCIE_COHERENCY_CONTROL_1_OFF_CFG_MEMTYPE_VALUE_SHFT                                               0x0

#define HWIO_GE_PCIE_COHERENCY_CONTROL_2_OFF_ADDR                                                          (GE_PCIE_PCIE20_DBI_4_REG_BASE      + 0x000008e4)
#define HWIO_GE_PCIE_COHERENCY_CONTROL_2_OFF_PHYS                                                          (GE_PCIE_PCIE20_DBI_4_REG_BASE_PHYS + 0x000008e4)
#define HWIO_GE_PCIE_COHERENCY_CONTROL_2_OFF_OFFS                                                          (GE_PCIE_PCIE20_DBI_4_REG_BASE_OFFS + 0x000008e4)
#define HWIO_GE_PCIE_COHERENCY_CONTROL_2_OFF_RMSK                                                          0xffffffff
#define HWIO_GE_PCIE_COHERENCY_CONTROL_2_OFF_POR                                                           0x00000000
#define HWIO_GE_PCIE_COHERENCY_CONTROL_2_OFF_POR_RMSK                                                      0xffffffff
#define HWIO_GE_PCIE_COHERENCY_CONTROL_2_OFF_ATTR                                                                 0x3
#define HWIO_GE_PCIE_COHERENCY_CONTROL_2_OFF_IN          \
        in_dword_masked(HWIO_GE_PCIE_COHERENCY_CONTROL_2_OFF_ADDR, HWIO_GE_PCIE_COHERENCY_CONTROL_2_OFF_RMSK)
#define HWIO_GE_PCIE_COHERENCY_CONTROL_2_OFF_INM(m)      \
        in_dword_masked(HWIO_GE_PCIE_COHERENCY_CONTROL_2_OFF_ADDR, m)
#define HWIO_GE_PCIE_COHERENCY_CONTROL_2_OFF_OUT(v)      \
        out_dword(HWIO_GE_PCIE_COHERENCY_CONTROL_2_OFF_ADDR,v)
#define HWIO_GE_PCIE_COHERENCY_CONTROL_2_OFF_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GE_PCIE_COHERENCY_CONTROL_2_OFF_ADDR,m,v,HWIO_GE_PCIE_COHERENCY_CONTROL_2_OFF_IN)
#define HWIO_GE_PCIE_COHERENCY_CONTROL_2_OFF_CFG_MEMTYPE_BOUNDARY_HIGH_ADDR_BMSK                           0xffffffff
#define HWIO_GE_PCIE_COHERENCY_CONTROL_2_OFF_CFG_MEMTYPE_BOUNDARY_HIGH_ADDR_SHFT                                  0x0

#define HWIO_GE_PCIE_COHERENCY_CONTROL_3_OFF_ADDR                                                          (GE_PCIE_PCIE20_DBI_4_REG_BASE      + 0x000008e8)
#define HWIO_GE_PCIE_COHERENCY_CONTROL_3_OFF_PHYS                                                          (GE_PCIE_PCIE20_DBI_4_REG_BASE_PHYS + 0x000008e8)
#define HWIO_GE_PCIE_COHERENCY_CONTROL_3_OFF_OFFS                                                          (GE_PCIE_PCIE20_DBI_4_REG_BASE_OFFS + 0x000008e8)
#define HWIO_GE_PCIE_COHERENCY_CONTROL_3_OFF_RMSK                                                          0x78787878
#define HWIO_GE_PCIE_COHERENCY_CONTROL_3_OFF_POR                                                           0x00000000
#define HWIO_GE_PCIE_COHERENCY_CONTROL_3_OFF_POR_RMSK                                                      0xffffffff
#define HWIO_GE_PCIE_COHERENCY_CONTROL_3_OFF_ATTR                                                                 0x3
#define HWIO_GE_PCIE_COHERENCY_CONTROL_3_OFF_IN          \
        in_dword_masked(HWIO_GE_PCIE_COHERENCY_CONTROL_3_OFF_ADDR, HWIO_GE_PCIE_COHERENCY_CONTROL_3_OFF_RMSK)
#define HWIO_GE_PCIE_COHERENCY_CONTROL_3_OFF_INM(m)      \
        in_dword_masked(HWIO_GE_PCIE_COHERENCY_CONTROL_3_OFF_ADDR, m)
#define HWIO_GE_PCIE_COHERENCY_CONTROL_3_OFF_OUT(v)      \
        out_dword(HWIO_GE_PCIE_COHERENCY_CONTROL_3_OFF_ADDR,v)
#define HWIO_GE_PCIE_COHERENCY_CONTROL_3_OFF_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GE_PCIE_COHERENCY_CONTROL_3_OFF_ADDR,m,v,HWIO_GE_PCIE_COHERENCY_CONTROL_3_OFF_IN)
#define HWIO_GE_PCIE_COHERENCY_CONTROL_3_OFF_CFG_MSTR_AWCACHE_VALUE_BMSK                                   0x78000000
#define HWIO_GE_PCIE_COHERENCY_CONTROL_3_OFF_CFG_MSTR_AWCACHE_VALUE_SHFT                                         0x1b
#define HWIO_GE_PCIE_COHERENCY_CONTROL_3_OFF_CFG_MSTR_ARCACHE_VALUE_BMSK                                     0x780000
#define HWIO_GE_PCIE_COHERENCY_CONTROL_3_OFF_CFG_MSTR_ARCACHE_VALUE_SHFT                                         0x13
#define HWIO_GE_PCIE_COHERENCY_CONTROL_3_OFF_CFG_MSTR_AWCACHE_MODE_BMSK                                        0x7800
#define HWIO_GE_PCIE_COHERENCY_CONTROL_3_OFF_CFG_MSTR_AWCACHE_MODE_SHFT                                           0xb
#define HWIO_GE_PCIE_COHERENCY_CONTROL_3_OFF_CFG_MSTR_ARCACHE_MODE_BMSK                                          0x78
#define HWIO_GE_PCIE_COHERENCY_CONTROL_3_OFF_CFG_MSTR_ARCACHE_MODE_SHFT                                           0x3

#define HWIO_GE_PCIE_PL_LAST_OFF_ADDR                                                                      (GE_PCIE_PCIE20_DBI_4_REG_BASE      + 0x000008fc)
#define HWIO_GE_PCIE_PL_LAST_OFF_PHYS                                                                      (GE_PCIE_PCIE20_DBI_4_REG_BASE_PHYS + 0x000008fc)
#define HWIO_GE_PCIE_PL_LAST_OFF_OFFS                                                                      (GE_PCIE_PCIE20_DBI_4_REG_BASE_OFFS + 0x000008fc)
#define HWIO_GE_PCIE_PL_LAST_OFF_RMSK                                                                      0xffffffff
#define HWIO_GE_PCIE_PL_LAST_OFF_POR                                                                       0x00000000
#define HWIO_GE_PCIE_PL_LAST_OFF_POR_RMSK                                                                  0xffffffff
#define HWIO_GE_PCIE_PL_LAST_OFF_ATTR                                                                             0x1
#define HWIO_GE_PCIE_PL_LAST_OFF_IN          \
        in_dword_masked(HWIO_GE_PCIE_PL_LAST_OFF_ADDR, HWIO_GE_PCIE_PL_LAST_OFF_RMSK)
#define HWIO_GE_PCIE_PL_LAST_OFF_INM(m)      \
        in_dword_masked(HWIO_GE_PCIE_PL_LAST_OFF_ADDR, m)
#define HWIO_GE_PCIE_PL_LAST_OFF_RSVDP_1_BMSK                                                              0xfffffffe
#define HWIO_GE_PCIE_PL_LAST_OFF_RSVDP_1_SHFT                                                                     0x1
#define HWIO_GE_PCIE_PL_LAST_OFF_PL_LAST_BMSK                                                                     0x1
#define HWIO_GE_PCIE_PL_LAST_OFF_PL_LAST_SHFT                                                                     0x0

#define HWIO_GE_PCIE_IATU_VIEWPORT_OFF_ADDR                                                                (GE_PCIE_PCIE20_DBI_4_REG_BASE      + 0x00000900)
#define HWIO_GE_PCIE_IATU_VIEWPORT_OFF_PHYS                                                                (GE_PCIE_PCIE20_DBI_4_REG_BASE_PHYS + 0x00000900)
#define HWIO_GE_PCIE_IATU_VIEWPORT_OFF_OFFS                                                                (GE_PCIE_PCIE20_DBI_4_REG_BASE_OFFS + 0x00000900)
#define HWIO_GE_PCIE_IATU_VIEWPORT_OFF_RMSK                                                                0x8000000f
#define HWIO_GE_PCIE_IATU_VIEWPORT_OFF_POR                                                                 0x00000000
#define HWIO_GE_PCIE_IATU_VIEWPORT_OFF_POR_RMSK                                                            0xffffffff
#define HWIO_GE_PCIE_IATU_VIEWPORT_OFF_ATTR                                                                       0x3
#define HWIO_GE_PCIE_IATU_VIEWPORT_OFF_IN          \
        in_dword_masked(HWIO_GE_PCIE_IATU_VIEWPORT_OFF_ADDR, HWIO_GE_PCIE_IATU_VIEWPORT_OFF_RMSK)
#define HWIO_GE_PCIE_IATU_VIEWPORT_OFF_INM(m)      \
        in_dword_masked(HWIO_GE_PCIE_IATU_VIEWPORT_OFF_ADDR, m)
#define HWIO_GE_PCIE_IATU_VIEWPORT_OFF_OUT(v)      \
        out_dword(HWIO_GE_PCIE_IATU_VIEWPORT_OFF_ADDR,v)
#define HWIO_GE_PCIE_IATU_VIEWPORT_OFF_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GE_PCIE_IATU_VIEWPORT_OFF_ADDR,m,v,HWIO_GE_PCIE_IATU_VIEWPORT_OFF_IN)
#define HWIO_GE_PCIE_IATU_VIEWPORT_OFF_REGION_DIR_BMSK                                                     0x80000000
#define HWIO_GE_PCIE_IATU_VIEWPORT_OFF_REGION_DIR_SHFT                                                           0x1f
#define HWIO_GE_PCIE_IATU_VIEWPORT_OFF_REGION_INDEX_BMSK                                                          0xf
#define HWIO_GE_PCIE_IATU_VIEWPORT_OFF_REGION_INDEX_SHFT                                                          0x0

#define HWIO_GE_PCIE_IATU_REGION_CTRL_1_OFF_ADDR                                                           (GE_PCIE_PCIE20_DBI_4_REG_BASE      + 0x00000904)
#define HWIO_GE_PCIE_IATU_REGION_CTRL_1_OFF_PHYS                                                           (GE_PCIE_PCIE20_DBI_4_REG_BASE_PHYS + 0x00000904)
#define HWIO_GE_PCIE_IATU_REGION_CTRL_1_OFF_OFFS                                                           (GE_PCIE_PCIE20_DBI_4_REG_BASE_OFFS + 0x00000904)
#define HWIO_GE_PCIE_IATU_REGION_CTRL_1_OFF_RMSK                                                            0x1f307ff
#define HWIO_GE_PCIE_IATU_REGION_CTRL_1_OFF_POR                                                            0x00000000
#define HWIO_GE_PCIE_IATU_REGION_CTRL_1_OFF_POR_RMSK                                                       0xffffffff
#define HWIO_GE_PCIE_IATU_REGION_CTRL_1_OFF_ATTR                                                                  0x3
#define HWIO_GE_PCIE_IATU_REGION_CTRL_1_OFF_IN          \
        in_dword_masked(HWIO_GE_PCIE_IATU_REGION_CTRL_1_OFF_ADDR, HWIO_GE_PCIE_IATU_REGION_CTRL_1_OFF_RMSK)
#define HWIO_GE_PCIE_IATU_REGION_CTRL_1_OFF_INM(m)      \
        in_dword_masked(HWIO_GE_PCIE_IATU_REGION_CTRL_1_OFF_ADDR, m)
#define HWIO_GE_PCIE_IATU_REGION_CTRL_1_OFF_OUT(v)      \
        out_dword(HWIO_GE_PCIE_IATU_REGION_CTRL_1_OFF_ADDR,v)
#define HWIO_GE_PCIE_IATU_REGION_CTRL_1_OFF_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GE_PCIE_IATU_REGION_CTRL_1_OFF_ADDR,m,v,HWIO_GE_PCIE_IATU_REGION_CTRL_1_OFF_IN)
#define HWIO_GE_PCIE_IATU_REGION_CTRL_1_OFF_FUNC_NUM_BMSK                                                   0x1f00000
#define HWIO_GE_PCIE_IATU_REGION_CTRL_1_OFF_FUNC_NUM_SHFT                                                        0x14
#define HWIO_GE_PCIE_IATU_REGION_CTRL_1_OFF_AT_BMSK                                                           0x30000
#define HWIO_GE_PCIE_IATU_REGION_CTRL_1_OFF_AT_SHFT                                                              0x10
#define HWIO_GE_PCIE_IATU_REGION_CTRL_1_OFF_ATTR_BMSK                                                           0x600
#define HWIO_GE_PCIE_IATU_REGION_CTRL_1_OFF_ATTR_SHFT                                                             0x9
#define HWIO_GE_PCIE_IATU_REGION_CTRL_1_OFF_TD_BMSK                                                             0x100
#define HWIO_GE_PCIE_IATU_REGION_CTRL_1_OFF_TD_SHFT                                                               0x8
#define HWIO_GE_PCIE_IATU_REGION_CTRL_1_OFF_TC_BMSK                                                              0xe0
#define HWIO_GE_PCIE_IATU_REGION_CTRL_1_OFF_TC_SHFT                                                               0x5
#define HWIO_GE_PCIE_IATU_REGION_CTRL_1_OFF_TYPE_BMSK                                                            0x1f
#define HWIO_GE_PCIE_IATU_REGION_CTRL_1_OFF_TYPE_SHFT                                                             0x0

#define HWIO_GE_PCIE_IATU_REGION_CTRL_2_OFF_ADDR                                                           (GE_PCIE_PCIE20_DBI_4_REG_BASE      + 0x00000908)
#define HWIO_GE_PCIE_IATU_REGION_CTRL_2_OFF_PHYS                                                           (GE_PCIE_PCIE20_DBI_4_REG_BASE_PHYS + 0x00000908)
#define HWIO_GE_PCIE_IATU_REGION_CTRL_2_OFF_OFFS                                                           (GE_PCIE_PCIE20_DBI_4_REG_BASE_OFFS + 0x00000908)
#define HWIO_GE_PCIE_IATU_REGION_CTRL_2_OFF_RMSK                                                           0xfb3dc7ff
#define HWIO_GE_PCIE_IATU_REGION_CTRL_2_OFF_POR                                                            0x00000000
#define HWIO_GE_PCIE_IATU_REGION_CTRL_2_OFF_POR_RMSK                                                       0xffffffff
#define HWIO_GE_PCIE_IATU_REGION_CTRL_2_OFF_ATTR                                                                  0x3
#define HWIO_GE_PCIE_IATU_REGION_CTRL_2_OFF_IN          \
        in_dword_masked(HWIO_GE_PCIE_IATU_REGION_CTRL_2_OFF_ADDR, HWIO_GE_PCIE_IATU_REGION_CTRL_2_OFF_RMSK)
#define HWIO_GE_PCIE_IATU_REGION_CTRL_2_OFF_INM(m)      \
        in_dword_masked(HWIO_GE_PCIE_IATU_REGION_CTRL_2_OFF_ADDR, m)
#define HWIO_GE_PCIE_IATU_REGION_CTRL_2_OFF_OUT(v)      \
        out_dword(HWIO_GE_PCIE_IATU_REGION_CTRL_2_OFF_ADDR,v)
#define HWIO_GE_PCIE_IATU_REGION_CTRL_2_OFF_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GE_PCIE_IATU_REGION_CTRL_2_OFF_ADDR,m,v,HWIO_GE_PCIE_IATU_REGION_CTRL_2_OFF_IN)
#define HWIO_GE_PCIE_IATU_REGION_CTRL_2_OFF_REGION_EN_BMSK                                                 0x80000000
#define HWIO_GE_PCIE_IATU_REGION_CTRL_2_OFF_REGION_EN_SHFT                                                       0x1f
#define HWIO_GE_PCIE_IATU_REGION_CTRL_2_OFF_MATCH_MODE_BMSK                                                0x40000000
#define HWIO_GE_PCIE_IATU_REGION_CTRL_2_OFF_MATCH_MODE_SHFT                                                      0x1e
#define HWIO_GE_PCIE_IATU_REGION_CTRL_2_OFF_INVERT_MODE_BMSK                                               0x20000000
#define HWIO_GE_PCIE_IATU_REGION_CTRL_2_OFF_INVERT_MODE_SHFT                                                     0x1d
#define HWIO_GE_PCIE_IATU_REGION_CTRL_2_OFF_CFG_SHIFT_MODE_BMSK                                            0x10000000
#define HWIO_GE_PCIE_IATU_REGION_CTRL_2_OFF_CFG_SHIFT_MODE_SHFT                                                  0x1c
#define HWIO_GE_PCIE_IATU_REGION_CTRL_2_OFF_DMA_BYPASSS_MODE_BMSK                                           0x8000000
#define HWIO_GE_PCIE_IATU_REGION_CTRL_2_OFF_DMA_BYPASSS_MODE_SHFT                                                0x1b
#define HWIO_GE_PCIE_IATU_REGION_CTRL_2_OFF_RESP_CODE_BMSK                                                  0x3000000
#define HWIO_GE_PCIE_IATU_REGION_CTRL_2_OFF_RESP_CODE_SHFT                                                       0x18
#define HWIO_GE_PCIE_IATU_REGION_CTRL_2_OFF_MC_MATCH_EN_BMSK                                                 0x200000
#define HWIO_GE_PCIE_IATU_REGION_CTRL_2_OFF_MC_MATCH_EN_SHFT                                                     0x15
#define HWIO_GE_PCIE_IATU_REGION_CTRL_2_OFF_VF_MATCH_EN_BMSK                                                 0x100000
#define HWIO_GE_PCIE_IATU_REGION_CTRL_2_OFF_VF_MATCH_EN_SHFT                                                     0x14
#define HWIO_GE_PCIE_IATU_REGION_CTRL_2_OFF_FUNC_BYPASS_BMSK                                                  0x80000
#define HWIO_GE_PCIE_IATU_REGION_CTRL_2_OFF_FUNC_BYPASS_SHFT                                                     0x13
#define HWIO_GE_PCIE_IATU_REGION_CTRL_2_OFF_AT_MATCH_EN_BMSK                                                  0x40000
#define HWIO_GE_PCIE_IATU_REGION_CTRL_2_OFF_AT_MATCH_EN_SHFT                                                     0x12
#define HWIO_GE_PCIE_IATU_REGION_CTRL_2_OFF_ATTR_MATCH_EN_BMSK                                                0x10000
#define HWIO_GE_PCIE_IATU_REGION_CTRL_2_OFF_ATTR_MATCH_EN_SHFT                                                   0x10
#define HWIO_GE_PCIE_IATU_REGION_CTRL_2_OFF_TD_MATCH_EN_BMSK                                                   0x8000
#define HWIO_GE_PCIE_IATU_REGION_CTRL_2_OFF_TD_MATCH_EN_SHFT                                                      0xf
#define HWIO_GE_PCIE_IATU_REGION_CTRL_2_OFF_TC_MATCH_EN_BMSK                                                   0x4000
#define HWIO_GE_PCIE_IATU_REGION_CTRL_2_OFF_TC_MATCH_EN_SHFT                                                      0xe
#define HWIO_GE_PCIE_IATU_REGION_CTRL_2_OFF_BAR_NUMBER_BMSK                                                     0x700
#define HWIO_GE_PCIE_IATU_REGION_CTRL_2_OFF_BAR_NUMBER_SHFT                                                       0x8
#define HWIO_GE_PCIE_IATU_REGION_CTRL_2_OFF_MSG_CODE_BMSK                                                        0xff
#define HWIO_GE_PCIE_IATU_REGION_CTRL_2_OFF_MSG_CODE_SHFT                                                         0x0

#define HWIO_GE_PCIE_IATU_LWR_BASE_ADDR_OFF_ADDR                                                           (GE_PCIE_PCIE20_DBI_4_REG_BASE      + 0x0000090c)
#define HWIO_GE_PCIE_IATU_LWR_BASE_ADDR_OFF_PHYS                                                           (GE_PCIE_PCIE20_DBI_4_REG_BASE_PHYS + 0x0000090c)
#define HWIO_GE_PCIE_IATU_LWR_BASE_ADDR_OFF_OFFS                                                           (GE_PCIE_PCIE20_DBI_4_REG_BASE_OFFS + 0x0000090c)
#define HWIO_GE_PCIE_IATU_LWR_BASE_ADDR_OFF_RMSK                                                           0xffffffff
#define HWIO_GE_PCIE_IATU_LWR_BASE_ADDR_OFF_POR                                                            0x00000000
#define HWIO_GE_PCIE_IATU_LWR_BASE_ADDR_OFF_POR_RMSK                                                       0xffffffff
#define HWIO_GE_PCIE_IATU_LWR_BASE_ADDR_OFF_ATTR                                                                  0x3
#define HWIO_GE_PCIE_IATU_LWR_BASE_ADDR_OFF_IN          \
        in_dword_masked(HWIO_GE_PCIE_IATU_LWR_BASE_ADDR_OFF_ADDR, HWIO_GE_PCIE_IATU_LWR_BASE_ADDR_OFF_RMSK)
#define HWIO_GE_PCIE_IATU_LWR_BASE_ADDR_OFF_INM(m)      \
        in_dword_masked(HWIO_GE_PCIE_IATU_LWR_BASE_ADDR_OFF_ADDR, m)
#define HWIO_GE_PCIE_IATU_LWR_BASE_ADDR_OFF_OUT(v)      \
        out_dword(HWIO_GE_PCIE_IATU_LWR_BASE_ADDR_OFF_ADDR,v)
#define HWIO_GE_PCIE_IATU_LWR_BASE_ADDR_OFF_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GE_PCIE_IATU_LWR_BASE_ADDR_OFF_ADDR,m,v,HWIO_GE_PCIE_IATU_LWR_BASE_ADDR_OFF_IN)
#define HWIO_GE_PCIE_IATU_LWR_BASE_ADDR_OFF_LWR_BASE_RW_BMSK                                               0xffff0000
#define HWIO_GE_PCIE_IATU_LWR_BASE_ADDR_OFF_LWR_BASE_RW_SHFT                                                     0x10
#define HWIO_GE_PCIE_IATU_LWR_BASE_ADDR_OFF_LWR_BASE_HW_BMSK                                                   0xffff
#define HWIO_GE_PCIE_IATU_LWR_BASE_ADDR_OFF_LWR_BASE_HW_SHFT                                                      0x0

#define HWIO_GE_PCIE_IATU_UPPER_BASE_ADDR_OFF_ADDR                                                         (GE_PCIE_PCIE20_DBI_4_REG_BASE      + 0x00000910)
#define HWIO_GE_PCIE_IATU_UPPER_BASE_ADDR_OFF_PHYS                                                         (GE_PCIE_PCIE20_DBI_4_REG_BASE_PHYS + 0x00000910)
#define HWIO_GE_PCIE_IATU_UPPER_BASE_ADDR_OFF_OFFS                                                         (GE_PCIE_PCIE20_DBI_4_REG_BASE_OFFS + 0x00000910)
#define HWIO_GE_PCIE_IATU_UPPER_BASE_ADDR_OFF_RMSK                                                         0xffffffff
#define HWIO_GE_PCIE_IATU_UPPER_BASE_ADDR_OFF_POR                                                          0x00000000
#define HWIO_GE_PCIE_IATU_UPPER_BASE_ADDR_OFF_POR_RMSK                                                     0xffffffff
#define HWIO_GE_PCIE_IATU_UPPER_BASE_ADDR_OFF_ATTR                                                                0x3
#define HWIO_GE_PCIE_IATU_UPPER_BASE_ADDR_OFF_IN          \
        in_dword_masked(HWIO_GE_PCIE_IATU_UPPER_BASE_ADDR_OFF_ADDR, HWIO_GE_PCIE_IATU_UPPER_BASE_ADDR_OFF_RMSK)
#define HWIO_GE_PCIE_IATU_UPPER_BASE_ADDR_OFF_INM(m)      \
        in_dword_masked(HWIO_GE_PCIE_IATU_UPPER_BASE_ADDR_OFF_ADDR, m)
#define HWIO_GE_PCIE_IATU_UPPER_BASE_ADDR_OFF_OUT(v)      \
        out_dword(HWIO_GE_PCIE_IATU_UPPER_BASE_ADDR_OFF_ADDR,v)
#define HWIO_GE_PCIE_IATU_UPPER_BASE_ADDR_OFF_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GE_PCIE_IATU_UPPER_BASE_ADDR_OFF_ADDR,m,v,HWIO_GE_PCIE_IATU_UPPER_BASE_ADDR_OFF_IN)
#define HWIO_GE_PCIE_IATU_UPPER_BASE_ADDR_OFF_UPPER_BASE_RW_BMSK                                           0xffffffff
#define HWIO_GE_PCIE_IATU_UPPER_BASE_ADDR_OFF_UPPER_BASE_RW_SHFT                                                  0x0

#define HWIO_GE_PCIE_IATU_LIMIT_ADDR_OFF_ADDR                                                              (GE_PCIE_PCIE20_DBI_4_REG_BASE      + 0x00000914)
#define HWIO_GE_PCIE_IATU_LIMIT_ADDR_OFF_PHYS                                                              (GE_PCIE_PCIE20_DBI_4_REG_BASE_PHYS + 0x00000914)
#define HWIO_GE_PCIE_IATU_LIMIT_ADDR_OFF_OFFS                                                              (GE_PCIE_PCIE20_DBI_4_REG_BASE_OFFS + 0x00000914)
#define HWIO_GE_PCIE_IATU_LIMIT_ADDR_OFF_RMSK                                                              0xffffffff
#define HWIO_GE_PCIE_IATU_LIMIT_ADDR_OFF_POR                                                               0x00000000
#define HWIO_GE_PCIE_IATU_LIMIT_ADDR_OFF_POR_RMSK                                                          0xffffffff
#define HWIO_GE_PCIE_IATU_LIMIT_ADDR_OFF_ATTR                                                                     0x3
#define HWIO_GE_PCIE_IATU_LIMIT_ADDR_OFF_IN          \
        in_dword_masked(HWIO_GE_PCIE_IATU_LIMIT_ADDR_OFF_ADDR, HWIO_GE_PCIE_IATU_LIMIT_ADDR_OFF_RMSK)
#define HWIO_GE_PCIE_IATU_LIMIT_ADDR_OFF_INM(m)      \
        in_dword_masked(HWIO_GE_PCIE_IATU_LIMIT_ADDR_OFF_ADDR, m)
#define HWIO_GE_PCIE_IATU_LIMIT_ADDR_OFF_OUT(v)      \
        out_dword(HWIO_GE_PCIE_IATU_LIMIT_ADDR_OFF_ADDR,v)
#define HWIO_GE_PCIE_IATU_LIMIT_ADDR_OFF_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GE_PCIE_IATU_LIMIT_ADDR_OFF_ADDR,m,v,HWIO_GE_PCIE_IATU_LIMIT_ADDR_OFF_IN)
#define HWIO_GE_PCIE_IATU_LIMIT_ADDR_OFF_LIMIT_ADDR_RW_BMSK                                                0xffff0000
#define HWIO_GE_PCIE_IATU_LIMIT_ADDR_OFF_LIMIT_ADDR_RW_SHFT                                                      0x10
#define HWIO_GE_PCIE_IATU_LIMIT_ADDR_OFF_LIMIT_ADDR_HW_BMSK                                                    0xffff
#define HWIO_GE_PCIE_IATU_LIMIT_ADDR_OFF_LIMIT_ADDR_HW_SHFT                                                       0x0

#define HWIO_GE_PCIE_IATU_LWR_TARGET_ADDR_OFF_ADDR                                                         (GE_PCIE_PCIE20_DBI_4_REG_BASE      + 0x00000918)
#define HWIO_GE_PCIE_IATU_LWR_TARGET_ADDR_OFF_PHYS                                                         (GE_PCIE_PCIE20_DBI_4_REG_BASE_PHYS + 0x00000918)
#define HWIO_GE_PCIE_IATU_LWR_TARGET_ADDR_OFF_OFFS                                                         (GE_PCIE_PCIE20_DBI_4_REG_BASE_OFFS + 0x00000918)
#define HWIO_GE_PCIE_IATU_LWR_TARGET_ADDR_OFF_RMSK                                                         0xffffffff
#define HWIO_GE_PCIE_IATU_LWR_TARGET_ADDR_OFF_POR                                                          0x00000000
#define HWIO_GE_PCIE_IATU_LWR_TARGET_ADDR_OFF_POR_RMSK                                                     0xffffffff
#define HWIO_GE_PCIE_IATU_LWR_TARGET_ADDR_OFF_ATTR                                                                0x3
#define HWIO_GE_PCIE_IATU_LWR_TARGET_ADDR_OFF_IN          \
        in_dword_masked(HWIO_GE_PCIE_IATU_LWR_TARGET_ADDR_OFF_ADDR, HWIO_GE_PCIE_IATU_LWR_TARGET_ADDR_OFF_RMSK)
#define HWIO_GE_PCIE_IATU_LWR_TARGET_ADDR_OFF_INM(m)      \
        in_dword_masked(HWIO_GE_PCIE_IATU_LWR_TARGET_ADDR_OFF_ADDR, m)
#define HWIO_GE_PCIE_IATU_LWR_TARGET_ADDR_OFF_OUT(v)      \
        out_dword(HWIO_GE_PCIE_IATU_LWR_TARGET_ADDR_OFF_ADDR,v)
#define HWIO_GE_PCIE_IATU_LWR_TARGET_ADDR_OFF_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GE_PCIE_IATU_LWR_TARGET_ADDR_OFF_ADDR,m,v,HWIO_GE_PCIE_IATU_LWR_TARGET_ADDR_OFF_IN)
#define HWIO_GE_PCIE_IATU_LWR_TARGET_ADDR_OFF_LWR_TARGET_RW_BMSK                                           0xffff0000
#define HWIO_GE_PCIE_IATU_LWR_TARGET_ADDR_OFF_LWR_TARGET_RW_SHFT                                                 0x10
#define HWIO_GE_PCIE_IATU_LWR_TARGET_ADDR_OFF_LWR_TARGET_HW_BMSK                                               0xffff
#define HWIO_GE_PCIE_IATU_LWR_TARGET_ADDR_OFF_LWR_TARGET_HW_SHFT                                                  0x0

#define HWIO_GE_PCIE_IATU_UPPER_TARGET_ADDR_OFF_ADDR                                                       (GE_PCIE_PCIE20_DBI_4_REG_BASE      + 0x0000091c)
#define HWIO_GE_PCIE_IATU_UPPER_TARGET_ADDR_OFF_PHYS                                                       (GE_PCIE_PCIE20_DBI_4_REG_BASE_PHYS + 0x0000091c)
#define HWIO_GE_PCIE_IATU_UPPER_TARGET_ADDR_OFF_OFFS                                                       (GE_PCIE_PCIE20_DBI_4_REG_BASE_OFFS + 0x0000091c)
#define HWIO_GE_PCIE_IATU_UPPER_TARGET_ADDR_OFF_RMSK                                                       0xffffffff
#define HWIO_GE_PCIE_IATU_UPPER_TARGET_ADDR_OFF_POR                                                        0x00000000
#define HWIO_GE_PCIE_IATU_UPPER_TARGET_ADDR_OFF_POR_RMSK                                                   0xffffffff
#define HWIO_GE_PCIE_IATU_UPPER_TARGET_ADDR_OFF_ATTR                                                              0x3
#define HWIO_GE_PCIE_IATU_UPPER_TARGET_ADDR_OFF_IN          \
        in_dword_masked(HWIO_GE_PCIE_IATU_UPPER_TARGET_ADDR_OFF_ADDR, HWIO_GE_PCIE_IATU_UPPER_TARGET_ADDR_OFF_RMSK)
#define HWIO_GE_PCIE_IATU_UPPER_TARGET_ADDR_OFF_INM(m)      \
        in_dword_masked(HWIO_GE_PCIE_IATU_UPPER_TARGET_ADDR_OFF_ADDR, m)
#define HWIO_GE_PCIE_IATU_UPPER_TARGET_ADDR_OFF_OUT(v)      \
        out_dword(HWIO_GE_PCIE_IATU_UPPER_TARGET_ADDR_OFF_ADDR,v)
#define HWIO_GE_PCIE_IATU_UPPER_TARGET_ADDR_OFF_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GE_PCIE_IATU_UPPER_TARGET_ADDR_OFF_ADDR,m,v,HWIO_GE_PCIE_IATU_UPPER_TARGET_ADDR_OFF_IN)
#define HWIO_GE_PCIE_IATU_UPPER_TARGET_ADDR_OFF_UPPER_TARGET_RW_BMSK                                       0xffffffff
#define HWIO_GE_PCIE_IATU_UPPER_TARGET_ADDR_OFF_UPPER_TARGET_RW_SHFT                                              0x0

#define HWIO_GE_PCIE_PL_LTR_LATENCY_OFF_ADDR                                                               (GE_PCIE_PCIE20_DBI_4_REG_BASE      + 0x00000b30)
#define HWIO_GE_PCIE_PL_LTR_LATENCY_OFF_PHYS                                                               (GE_PCIE_PCIE20_DBI_4_REG_BASE_PHYS + 0x00000b30)
#define HWIO_GE_PCIE_PL_LTR_LATENCY_OFF_OFFS                                                               (GE_PCIE_PCIE20_DBI_4_REG_BASE_OFFS + 0x00000b30)
#define HWIO_GE_PCIE_PL_LTR_LATENCY_OFF_RMSK                                                               0xffffffff
#define HWIO_GE_PCIE_PL_LTR_LATENCY_OFF_POR                                                                0x00000000
#define HWIO_GE_PCIE_PL_LTR_LATENCY_OFF_POR_RMSK                                                           0xffffffff
#define HWIO_GE_PCIE_PL_LTR_LATENCY_OFF_ATTR                                                                      0x3
#define HWIO_GE_PCIE_PL_LTR_LATENCY_OFF_IN          \
        in_dword_masked(HWIO_GE_PCIE_PL_LTR_LATENCY_OFF_ADDR, HWIO_GE_PCIE_PL_LTR_LATENCY_OFF_RMSK)
#define HWIO_GE_PCIE_PL_LTR_LATENCY_OFF_INM(m)      \
        in_dword_masked(HWIO_GE_PCIE_PL_LTR_LATENCY_OFF_ADDR, m)
#define HWIO_GE_PCIE_PL_LTR_LATENCY_OFF_OUT(v)      \
        out_dword(HWIO_GE_PCIE_PL_LTR_LATENCY_OFF_ADDR,v)
#define HWIO_GE_PCIE_PL_LTR_LATENCY_OFF_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GE_PCIE_PL_LTR_LATENCY_OFF_ADDR,m,v,HWIO_GE_PCIE_PL_LTR_LATENCY_OFF_IN)
#define HWIO_GE_PCIE_PL_LTR_LATENCY_OFF_NO_SNOOP_LATENCY_REQUIRE_BMSK                                      0x80000000
#define HWIO_GE_PCIE_PL_LTR_LATENCY_OFF_NO_SNOOP_LATENCY_REQUIRE_SHFT                                            0x1f
#define HWIO_GE_PCIE_PL_LTR_LATENCY_OFF_RSVDP_29_BMSK                                                      0x60000000
#define HWIO_GE_PCIE_PL_LTR_LATENCY_OFF_RSVDP_29_SHFT                                                            0x1d
#define HWIO_GE_PCIE_PL_LTR_LATENCY_OFF_NO_SNOOP_LATENCY_SCALE_BMSK                                        0x1c000000
#define HWIO_GE_PCIE_PL_LTR_LATENCY_OFF_NO_SNOOP_LATENCY_SCALE_SHFT                                              0x1a
#define HWIO_GE_PCIE_PL_LTR_LATENCY_OFF_NO_SNOOP_LATENCY_VALUE_BMSK                                         0x3ff0000
#define HWIO_GE_PCIE_PL_LTR_LATENCY_OFF_NO_SNOOP_LATENCY_VALUE_SHFT                                              0x10
#define HWIO_GE_PCIE_PL_LTR_LATENCY_OFF_SNOOP_LATENCY_REQUIRE_BMSK                                             0x8000
#define HWIO_GE_PCIE_PL_LTR_LATENCY_OFF_SNOOP_LATENCY_REQUIRE_SHFT                                                0xf
#define HWIO_GE_PCIE_PL_LTR_LATENCY_OFF_RSVDP_13_BMSK                                                          0x6000
#define HWIO_GE_PCIE_PL_LTR_LATENCY_OFF_RSVDP_13_SHFT                                                             0xd
#define HWIO_GE_PCIE_PL_LTR_LATENCY_OFF_SNOOP_LATENCY_SCALE_BMSK                                               0x1c00
#define HWIO_GE_PCIE_PL_LTR_LATENCY_OFF_SNOOP_LATENCY_SCALE_SHFT                                                  0xa
#define HWIO_GE_PCIE_PL_LTR_LATENCY_OFF_SNOOP_LATENCY_VALUE_BMSK                                                0x3ff
#define HWIO_GE_PCIE_PL_LTR_LATENCY_OFF_SNOOP_LATENCY_VALUE_SHFT                                                  0x0

#define HWIO_GE_PCIE_AUX_CLK_FREQ_OFF_ADDR                                                                 (GE_PCIE_PCIE20_DBI_4_REG_BASE      + 0x00000b40)
#define HWIO_GE_PCIE_AUX_CLK_FREQ_OFF_PHYS                                                                 (GE_PCIE_PCIE20_DBI_4_REG_BASE_PHYS + 0x00000b40)
#define HWIO_GE_PCIE_AUX_CLK_FREQ_OFF_OFFS                                                                 (GE_PCIE_PCIE20_DBI_4_REG_BASE_OFFS + 0x00000b40)
#define HWIO_GE_PCIE_AUX_CLK_FREQ_OFF_RMSK                                                                 0xffffffff
#define HWIO_GE_PCIE_AUX_CLK_FREQ_OFF_POR                                                                  0x00000000
#define HWIO_GE_PCIE_AUX_CLK_FREQ_OFF_POR_RMSK                                                             0xffffffff
#define HWIO_GE_PCIE_AUX_CLK_FREQ_OFF_ATTR                                                                        0x3
#define HWIO_GE_PCIE_AUX_CLK_FREQ_OFF_IN          \
        in_dword_masked(HWIO_GE_PCIE_AUX_CLK_FREQ_OFF_ADDR, HWIO_GE_PCIE_AUX_CLK_FREQ_OFF_RMSK)
#define HWIO_GE_PCIE_AUX_CLK_FREQ_OFF_INM(m)      \
        in_dword_masked(HWIO_GE_PCIE_AUX_CLK_FREQ_OFF_ADDR, m)
#define HWIO_GE_PCIE_AUX_CLK_FREQ_OFF_OUT(v)      \
        out_dword(HWIO_GE_PCIE_AUX_CLK_FREQ_OFF_ADDR,v)
#define HWIO_GE_PCIE_AUX_CLK_FREQ_OFF_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GE_PCIE_AUX_CLK_FREQ_OFF_ADDR,m,v,HWIO_GE_PCIE_AUX_CLK_FREQ_OFF_IN)
#define HWIO_GE_PCIE_AUX_CLK_FREQ_OFF_RSVDP_10_BMSK                                                        0xfffffc00
#define HWIO_GE_PCIE_AUX_CLK_FREQ_OFF_RSVDP_10_SHFT                                                               0xa
#define HWIO_GE_PCIE_AUX_CLK_FREQ_OFF_AUX_CLK_FREQ_BMSK                                                         0x3ff
#define HWIO_GE_PCIE_AUX_CLK_FREQ_OFF_AUX_CLK_FREQ_SHFT                                                           0x0

#define HWIO_GE_PCIE_L1_SUBSTATES_OFF_ADDR                                                                 (GE_PCIE_PCIE20_DBI_4_REG_BASE      + 0x00000b44)
#define HWIO_GE_PCIE_L1_SUBSTATES_OFF_PHYS                                                                 (GE_PCIE_PCIE20_DBI_4_REG_BASE_PHYS + 0x00000b44)
#define HWIO_GE_PCIE_L1_SUBSTATES_OFF_OFFS                                                                 (GE_PCIE_PCIE20_DBI_4_REG_BASE_OFFS + 0x00000b44)
#define HWIO_GE_PCIE_L1_SUBSTATES_OFF_RMSK                                                                 0xffffffff
#define HWIO_GE_PCIE_L1_SUBSTATES_OFF_POR                                                                  0x00000000
#define HWIO_GE_PCIE_L1_SUBSTATES_OFF_POR_RMSK                                                             0xffffffff
#define HWIO_GE_PCIE_L1_SUBSTATES_OFF_ATTR                                                                        0x3
#define HWIO_GE_PCIE_L1_SUBSTATES_OFF_IN          \
        in_dword_masked(HWIO_GE_PCIE_L1_SUBSTATES_OFF_ADDR, HWIO_GE_PCIE_L1_SUBSTATES_OFF_RMSK)
#define HWIO_GE_PCIE_L1_SUBSTATES_OFF_INM(m)      \
        in_dword_masked(HWIO_GE_PCIE_L1_SUBSTATES_OFF_ADDR, m)
#define HWIO_GE_PCIE_L1_SUBSTATES_OFF_OUT(v)      \
        out_dword(HWIO_GE_PCIE_L1_SUBSTATES_OFF_ADDR,v)
#define HWIO_GE_PCIE_L1_SUBSTATES_OFF_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GE_PCIE_L1_SUBSTATES_OFF_ADDR,m,v,HWIO_GE_PCIE_L1_SUBSTATES_OFF_IN)
#define HWIO_GE_PCIE_L1_SUBSTATES_OFF_RSVDP_8_BMSK                                                         0xffffff00
#define HWIO_GE_PCIE_L1_SUBSTATES_OFF_RSVDP_8_SHFT                                                                0x8
#define HWIO_GE_PCIE_L1_SUBSTATES_OFF_L1SUB_T_PCLKACK_BMSK                                                       0xc0
#define HWIO_GE_PCIE_L1_SUBSTATES_OFF_L1SUB_T_PCLKACK_SHFT                                                        0x6
#define HWIO_GE_PCIE_L1_SUBSTATES_OFF_L1SUB_T_L1_2_BMSK                                                          0x3c
#define HWIO_GE_PCIE_L1_SUBSTATES_OFF_L1SUB_T_L1_2_SHFT                                                           0x2
#define HWIO_GE_PCIE_L1_SUBSTATES_OFF_L1SUB_T_POWER_OFF_BMSK                                                      0x3
#define HWIO_GE_PCIE_L1_SUBSTATES_OFF_L1SUB_T_POWER_OFF_SHFT                                                      0x0

/*----------------------------------------------------------------------------
 * MODULE: GE_PCIE_PCIE_REGS_ELBI
 *--------------------------------------------------------------------------*/

#define GE_PCIE_PCIE_REGS_ELBI_REG_BASE                               (GE_PCIE_PCIE_REGS_BASE      + 0x00003000)
#define GE_PCIE_PCIE_REGS_ELBI_REG_BASE_PHYS                          (GE_PCIE_PCIE_REGS_BASE_PHYS + 0x00003000)
#define GE_PCIE_PCIE_REGS_ELBI_REG_BASE_OFFS                          0x00003000

#define HWIO_GE_PCIE_ELBI_ID_ADDR                                     (GE_PCIE_PCIE_REGS_ELBI_REG_BASE      + 0x00000000)
#define HWIO_GE_PCIE_ELBI_ID_PHYS                                     (GE_PCIE_PCIE_REGS_ELBI_REG_BASE_PHYS + 0x00000000)
#define HWIO_GE_PCIE_ELBI_ID_OFFS                                     (GE_PCIE_PCIE_REGS_ELBI_REG_BASE_OFFS + 0x00000000)
#define HWIO_GE_PCIE_ELBI_ID_RMSK                                     0xffffffff
#define HWIO_GE_PCIE_ELBI_ID_POR                                      0xacce55ed
#define HWIO_GE_PCIE_ELBI_ID_POR_RMSK                                 0xffffffff
#define HWIO_GE_PCIE_ELBI_ID_ATTR                                            0x1
#define HWIO_GE_PCIE_ELBI_ID_IN          \
        in_dword_masked(HWIO_GE_PCIE_ELBI_ID_ADDR, HWIO_GE_PCIE_ELBI_ID_RMSK)
#define HWIO_GE_PCIE_ELBI_ID_INM(m)      \
        in_dword_masked(HWIO_GE_PCIE_ELBI_ID_ADDR, m)
#define HWIO_GE_PCIE_ELBI_ID_SYNOPBUSH_ID_BMSK                        0xffffffff
#define HWIO_GE_PCIE_ELBI_ID_SYNOPBUSH_ID_SHFT                               0x0

#define HWIO_GE_PCIE_ELBI_SYS_CTRL_ADDR                               (GE_PCIE_PCIE_REGS_ELBI_REG_BASE      + 0x00000004)
#define HWIO_GE_PCIE_ELBI_SYS_CTRL_PHYS                               (GE_PCIE_PCIE_REGS_ELBI_REG_BASE_PHYS + 0x00000004)
#define HWIO_GE_PCIE_ELBI_SYS_CTRL_OFFS                               (GE_PCIE_PCIE_REGS_ELBI_REG_BASE_OFFS + 0x00000004)
#define HWIO_GE_PCIE_ELBI_SYS_CTRL_RMSK                                      0x1
#define HWIO_GE_PCIE_ELBI_SYS_CTRL_POR                                0x00000000
#define HWIO_GE_PCIE_ELBI_SYS_CTRL_POR_RMSK                           0xffffffff
#define HWIO_GE_PCIE_ELBI_SYS_CTRL_ATTR                                      0x3
#define HWIO_GE_PCIE_ELBI_SYS_CTRL_IN          \
        in_dword_masked(HWIO_GE_PCIE_ELBI_SYS_CTRL_ADDR, HWIO_GE_PCIE_ELBI_SYS_CTRL_RMSK)
#define HWIO_GE_PCIE_ELBI_SYS_CTRL_INM(m)      \
        in_dword_masked(HWIO_GE_PCIE_ELBI_SYS_CTRL_ADDR, m)
#define HWIO_GE_PCIE_ELBI_SYS_CTRL_OUT(v)      \
        out_dword(HWIO_GE_PCIE_ELBI_SYS_CTRL_ADDR,v)
#define HWIO_GE_PCIE_ELBI_SYS_CTRL_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GE_PCIE_ELBI_SYS_CTRL_ADDR,m,v,HWIO_GE_PCIE_ELBI_SYS_CTRL_IN)
#define HWIO_GE_PCIE_ELBI_SYS_CTRL_SYS_RESET_REG_BMSK                        0x1
#define HWIO_GE_PCIE_ELBI_SYS_CTRL_SYS_RESET_REG_SHFT                        0x0

#define HWIO_GE_PCIE_ELBI_DUT_CONNECT_REG_ADDR                        (GE_PCIE_PCIE_REGS_ELBI_REG_BASE      + 0x00000008)
#define HWIO_GE_PCIE_ELBI_DUT_CONNECT_REG_PHYS                        (GE_PCIE_PCIE_REGS_ELBI_REG_BASE_PHYS + 0x00000008)
#define HWIO_GE_PCIE_ELBI_DUT_CONNECT_REG_OFFS                        (GE_PCIE_PCIE_REGS_ELBI_REG_BASE_OFFS + 0x00000008)
#define HWIO_GE_PCIE_ELBI_DUT_CONNECT_REG_RMSK                          0xf7777f
#define HWIO_GE_PCIE_ELBI_DUT_CONNECT_REG_POR                         0x00f32101
#define HWIO_GE_PCIE_ELBI_DUT_CONNECT_REG_POR_RMSK                    0xffffffff
#define HWIO_GE_PCIE_ELBI_DUT_CONNECT_REG_ATTR                               0x3
#define HWIO_GE_PCIE_ELBI_DUT_CONNECT_REG_IN          \
        in_dword_masked(HWIO_GE_PCIE_ELBI_DUT_CONNECT_REG_ADDR, HWIO_GE_PCIE_ELBI_DUT_CONNECT_REG_RMSK)
#define HWIO_GE_PCIE_ELBI_DUT_CONNECT_REG_INM(m)      \
        in_dword_masked(HWIO_GE_PCIE_ELBI_DUT_CONNECT_REG_ADDR, m)
#define HWIO_GE_PCIE_ELBI_DUT_CONNECT_REG_OUT(v)      \
        out_dword(HWIO_GE_PCIE_ELBI_DUT_CONNECT_REG_ADDR,v)
#define HWIO_GE_PCIE_ELBI_DUT_CONNECT_REG_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GE_PCIE_ELBI_DUT_CONNECT_REG_ADDR,m,v,HWIO_GE_PCIE_ELBI_DUT_CONNECT_REG_IN)
#define HWIO_GE_PCIE_ELBI_DUT_CONNECT_REG_JTAG_SEL_BMSK                 0xf00000
#define HWIO_GE_PCIE_ELBI_DUT_CONNECT_REG_JTAG_SEL_SHFT                     0x14
#define HWIO_GE_PCIE_ELBI_DUT_CONNECT_REG_DUT3_FPGA_SEL_BMSK             0x70000
#define HWIO_GE_PCIE_ELBI_DUT_CONNECT_REG_DUT3_FPGA_SEL_SHFT                0x10
#define HWIO_GE_PCIE_ELBI_DUT_CONNECT_REG_DUT2_FPGA_SEL_BMSK              0x7000
#define HWIO_GE_PCIE_ELBI_DUT_CONNECT_REG_DUT2_FPGA_SEL_SHFT                 0xc
#define HWIO_GE_PCIE_ELBI_DUT_CONNECT_REG_DUT1_FPGA_SEL_BMSK               0x700
#define HWIO_GE_PCIE_ELBI_DUT_CONNECT_REG_DUT1_FPGA_SEL_SHFT                 0x8
#define HWIO_GE_PCIE_ELBI_DUT_CONNECT_REG_DUT0_FPGA_SEL_BMSK                0x70
#define HWIO_GE_PCIE_ELBI_DUT_CONNECT_REG_DUT0_FPGA_SEL_SHFT                 0x4
#define HWIO_GE_PCIE_ELBI_DUT_CONNECT_REG_DUT3_ON_BMSK                       0x8
#define HWIO_GE_PCIE_ELBI_DUT_CONNECT_REG_DUT3_ON_SHFT                       0x3
#define HWIO_GE_PCIE_ELBI_DUT_CONNECT_REG_DUT2_ON_BMSK                       0x4
#define HWIO_GE_PCIE_ELBI_DUT_CONNECT_REG_DUT2_ON_SHFT                       0x2
#define HWIO_GE_PCIE_ELBI_DUT_CONNECT_REG_DUT1_ON_BMSK                       0x2
#define HWIO_GE_PCIE_ELBI_DUT_CONNECT_REG_DUT1_ON_SHFT                       0x1
#define HWIO_GE_PCIE_ELBI_DUT_CONNECT_REG_DUT0_ON_BMSK                       0x1
#define HWIO_GE_PCIE_ELBI_DUT_CONNECT_REG_DUT0_ON_SHFT                       0x0

#define HWIO_GE_PCIE_ELBI_SYS_STTS_ADDR                               (GE_PCIE_PCIE_REGS_ELBI_REG_BASE      + 0x00000030)
#define HWIO_GE_PCIE_ELBI_SYS_STTS_PHYS                               (GE_PCIE_PCIE_REGS_ELBI_REG_BASE_PHYS + 0x00000030)
#define HWIO_GE_PCIE_ELBI_SYS_STTS_OFFS                               (GE_PCIE_PCIE_REGS_ELBI_REG_BASE_OFFS + 0x00000030)
#define HWIO_GE_PCIE_ELBI_SYS_STTS_RMSK                                   0x303f
#define HWIO_GE_PCIE_ELBI_SYS_STTS_POR                                0x00000000
#define HWIO_GE_PCIE_ELBI_SYS_STTS_POR_RMSK                           0xffffffff
#define HWIO_GE_PCIE_ELBI_SYS_STTS_ATTR                                      0x1
#define HWIO_GE_PCIE_ELBI_SYS_STTS_IN          \
        in_dword_masked(HWIO_GE_PCIE_ELBI_SYS_STTS_ADDR, HWIO_GE_PCIE_ELBI_SYS_STTS_RMSK)
#define HWIO_GE_PCIE_ELBI_SYS_STTS_INM(m)      \
        in_dword_masked(HWIO_GE_PCIE_ELBI_SYS_STTS_ADDR, m)
#define HWIO_GE_PCIE_ELBI_SYS_STTS_RESET_WARNING_BMSK                     0x2000
#define HWIO_GE_PCIE_ELBI_SYS_STTS_RESET_WARNING_SHFT                        0xd
#define HWIO_GE_PCIE_ELBI_SYS_STTS_SAFEGUARD_ACTIVE_BMSK                  0x1000
#define HWIO_GE_PCIE_ELBI_SYS_STTS_SAFEGUARD_ACTIVE_SHFT                     0xc
#define HWIO_GE_PCIE_ELBI_SYS_STTS_LTSSM_STATE_BMSK                         0x3f
#define HWIO_GE_PCIE_ELBI_SYS_STTS_LTSSM_STATE_SHFT                          0x0

#define HWIO_GE_PCIE_ELBI_SNOWBUSH_STTS_ADDR                          (GE_PCIE_PCIE_REGS_ELBI_REG_BASE      + 0x00000034)
#define HWIO_GE_PCIE_ELBI_SNOWBUSH_STTS_PHYS                          (GE_PCIE_PCIE_REGS_ELBI_REG_BASE_PHYS + 0x00000034)
#define HWIO_GE_PCIE_ELBI_SNOWBUSH_STTS_OFFS                          (GE_PCIE_PCIE_REGS_ELBI_REG_BASE_OFFS + 0x00000034)
#define HWIO_GE_PCIE_ELBI_SNOWBUSH_STTS_RMSK                               0x107
#define HWIO_GE_PCIE_ELBI_SNOWBUSH_STTS_POR                           0x00000000
#define HWIO_GE_PCIE_ELBI_SNOWBUSH_STTS_POR_RMSK                      0xffffffff
#define HWIO_GE_PCIE_ELBI_SNOWBUSH_STTS_ATTR                                 0x1
#define HWIO_GE_PCIE_ELBI_SNOWBUSH_STTS_IN          \
        in_dword_masked(HWIO_GE_PCIE_ELBI_SNOWBUSH_STTS_ADDR, HWIO_GE_PCIE_ELBI_SNOWBUSH_STTS_RMSK)
#define HWIO_GE_PCIE_ELBI_SNOWBUSH_STTS_INM(m)      \
        in_dword_masked(HWIO_GE_PCIE_ELBI_SNOWBUSH_STTS_ADDR, m)
#define HWIO_GE_PCIE_ELBI_SNOWBUSH_STTS_GEN2_SPEED_BMSK                    0x100
#define HWIO_GE_PCIE_ELBI_SNOWBUSH_STTS_GEN2_SPEED_SHFT                      0x8
#define HWIO_GE_PCIE_ELBI_SNOWBUSH_STTS_CONFIGURED_BMSK                      0x4
#define HWIO_GE_PCIE_ELBI_SNOWBUSH_STTS_CONFIGURED_SHFT                      0x2
#define HWIO_GE_PCIE_ELBI_SNOWBUSH_STTS_DATA_UP_BMSK                         0x2
#define HWIO_GE_PCIE_ELBI_SNOWBUSH_STTS_DATA_UP_SHFT                         0x1
#define HWIO_GE_PCIE_ELBI_SNOWBUSH_STTS_LINKED_UP_BMSK                       0x1
#define HWIO_GE_PCIE_ELBI_SNOWBUSH_STTS_LINKED_UP_SHFT                       0x0

/*----------------------------------------------------------------------------
 * MODULE: GE_QSB_IMEM
 *--------------------------------------------------------------------------*/

#define GE_QSB_IMEM_REG_BASE (GE_GEN_EMU_BASE      + 0x00000000)
#define GE_QSB_IMEM_REG_BASE_PHYS (GE_GEN_EMU_BASE_PHYS + 0x00000000)
#define GE_QSB_IMEM_REG_BASE_OFFS 0x00000000

/*----------------------------------------------------------------------------
 * MODULE: GE_INT_CTL
 *--------------------------------------------------------------------------*/

#define GE_INT_CTL_REG_BASE                                (GE_GEN_EMU_BASE      + 0x001c0000)
#define GE_INT_CTL_REG_BASE_PHYS                           (GE_GEN_EMU_BASE_PHYS + 0x001c0000)
#define GE_INT_CTL_REG_BASE_OFFS                           0x001c0000

#define HWIO_GE_INT_CTL_VER_CNT_ADDR                       (GE_INT_CTL_REG_BASE      + 0x00000000)
#define HWIO_GE_INT_CTL_VER_CNT_PHYS                       (GE_INT_CTL_REG_BASE_PHYS + 0x00000000)
#define HWIO_GE_INT_CTL_VER_CNT_OFFS                       (GE_INT_CTL_REG_BASE_OFFS + 0x00000000)
#define HWIO_GE_INT_CTL_VER_CNT_RMSK                       0xffffffff
#define HWIO_GE_INT_CTL_VER_CNT_POR                        0x01020100
#define HWIO_GE_INT_CTL_VER_CNT_POR_RMSK                   0xffffffff
#define HWIO_GE_INT_CTL_VER_CNT_ATTR                              0x1
#define HWIO_GE_INT_CTL_VER_CNT_IN          \
        in_dword_masked(HWIO_GE_INT_CTL_VER_CNT_ADDR, HWIO_GE_INT_CTL_VER_CNT_RMSK)
#define HWIO_GE_INT_CTL_VER_CNT_INM(m)      \
        in_dword_masked(HWIO_GE_INT_CTL_VER_CNT_ADDR, m)
#define HWIO_GE_INT_CTL_VER_CNT_MAJOR_BMSK                 0xff000000
#define HWIO_GE_INT_CTL_VER_CNT_MAJOR_SHFT                       0x18
#define HWIO_GE_INT_CTL_VER_CNT_MINOR_BMSK                   0xff0000
#define HWIO_GE_INT_CTL_VER_CNT_MINOR_SHFT                       0x10
#define HWIO_GE_INT_CTL_VER_CNT_COUNT_BMSK                     0xffff
#define HWIO_GE_INT_CTL_VER_CNT_COUNT_SHFT                        0x0

#define HWIO_GE_INT_OUT_ENABLE_ADDR                        (GE_INT_CTL_REG_BASE      + 0x00000004)
#define HWIO_GE_INT_OUT_ENABLE_PHYS                        (GE_INT_CTL_REG_BASE_PHYS + 0x00000004)
#define HWIO_GE_INT_OUT_ENABLE_OFFS                        (GE_INT_CTL_REG_BASE_OFFS + 0x00000004)
#define HWIO_GE_INT_OUT_ENABLE_RMSK                               0x1
#define HWIO_GE_INT_OUT_ENABLE_POR                         0x00000001
#define HWIO_GE_INT_OUT_ENABLE_POR_RMSK                    0xffffffff
#define HWIO_GE_INT_OUT_ENABLE_ATTR                               0x3
#define HWIO_GE_INT_OUT_ENABLE_IN          \
        in_dword_masked(HWIO_GE_INT_OUT_ENABLE_ADDR, HWIO_GE_INT_OUT_ENABLE_RMSK)
#define HWIO_GE_INT_OUT_ENABLE_INM(m)      \
        in_dword_masked(HWIO_GE_INT_OUT_ENABLE_ADDR, m)
#define HWIO_GE_INT_OUT_ENABLE_OUT(v)      \
        out_dword(HWIO_GE_INT_OUT_ENABLE_ADDR,v)
#define HWIO_GE_INT_OUT_ENABLE_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GE_INT_OUT_ENABLE_ADDR,m,v,HWIO_GE_INT_OUT_ENABLE_IN)
#define HWIO_GE_INT_OUT_ENABLE_OUT_ENABLE_BMSK                    0x1
#define HWIO_GE_INT_OUT_ENABLE_OUT_ENABLE_SHFT                    0x0

#define HWIO_GE_INT_MASTER_ENABLE_ADDR                     (GE_INT_CTL_REG_BASE      + 0x00000008)
#define HWIO_GE_INT_MASTER_ENABLE_PHYS                     (GE_INT_CTL_REG_BASE_PHYS + 0x00000008)
#define HWIO_GE_INT_MASTER_ENABLE_OFFS                     (GE_INT_CTL_REG_BASE_OFFS + 0x00000008)
#define HWIO_GE_INT_MASTER_ENABLE_RMSK                            0x1
#define HWIO_GE_INT_MASTER_ENABLE_POR                      0x00000001
#define HWIO_GE_INT_MASTER_ENABLE_POR_RMSK                 0xffffffff
#define HWIO_GE_INT_MASTER_ENABLE_ATTR                            0x3
#define HWIO_GE_INT_MASTER_ENABLE_IN          \
        in_dword_masked(HWIO_GE_INT_MASTER_ENABLE_ADDR, HWIO_GE_INT_MASTER_ENABLE_RMSK)
#define HWIO_GE_INT_MASTER_ENABLE_INM(m)      \
        in_dword_masked(HWIO_GE_INT_MASTER_ENABLE_ADDR, m)
#define HWIO_GE_INT_MASTER_ENABLE_OUT(v)      \
        out_dword(HWIO_GE_INT_MASTER_ENABLE_ADDR,v)
#define HWIO_GE_INT_MASTER_ENABLE_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GE_INT_MASTER_ENABLE_ADDR,m,v,HWIO_GE_INT_MASTER_ENABLE_IN)
#define HWIO_GE_INT_MASTER_ENABLE_IRQ_ENABLE_BMSK                 0x1
#define HWIO_GE_INT_MASTER_ENABLE_IRQ_ENABLE_SHFT                 0x0

#define HWIO_GE_INT_MASTER_STATUS_ADDR                     (GE_INT_CTL_REG_BASE      + 0x0000000c)
#define HWIO_GE_INT_MASTER_STATUS_PHYS                     (GE_INT_CTL_REG_BASE_PHYS + 0x0000000c)
#define HWIO_GE_INT_MASTER_STATUS_OFFS                     (GE_INT_CTL_REG_BASE_OFFS + 0x0000000c)
#define HWIO_GE_INT_MASTER_STATUS_RMSK                            0x1
#define HWIO_GE_INT_MASTER_STATUS_POR                      0x00000000
#define HWIO_GE_INT_MASTER_STATUS_POR_RMSK                 0xffffffff
#define HWIO_GE_INT_MASTER_STATUS_ATTR                            0x1
#define HWIO_GE_INT_MASTER_STATUS_IN          \
        in_dword_masked(HWIO_GE_INT_MASTER_STATUS_ADDR, HWIO_GE_INT_MASTER_STATUS_RMSK)
#define HWIO_GE_INT_MASTER_STATUS_INM(m)      \
        in_dword_masked(HWIO_GE_INT_MASTER_STATUS_ADDR, m)
#define HWIO_GE_INT_MASTER_STATUS_IRQ_OUT_BMSK                    0x1
#define HWIO_GE_INT_MASTER_STATUS_IRQ_OUT_SHFT                    0x0

#define HWIO_GE_INT_ENABLE_n_ADDR(n)                       (GE_INT_CTL_REG_BASE      + 0x00000010 + 0x20 * (n))
#define HWIO_GE_INT_ENABLE_n_PHYS(n)                       (GE_INT_CTL_REG_BASE_PHYS + 0x00000010 + 0x20 * (n))
#define HWIO_GE_INT_ENABLE_n_OFFS(n)                       (GE_INT_CTL_REG_BASE_OFFS + 0x00000010 + 0x20 * (n))
#define HWIO_GE_INT_ENABLE_n_RMSK                          0xffffffff
#define HWIO_GE_INT_ENABLE_n_MAXn                                   7
#define HWIO_GE_INT_ENABLE_n_POR                           0x00000000
#define HWIO_GE_INT_ENABLE_n_POR_RMSK                      0xffffffff
#define HWIO_GE_INT_ENABLE_n_ATTR                                 0x3
#define HWIO_GE_INT_ENABLE_n_INI(n)        \
        in_dword_masked(HWIO_GE_INT_ENABLE_n_ADDR(n), HWIO_GE_INT_ENABLE_n_RMSK)
#define HWIO_GE_INT_ENABLE_n_INMI(n,mask)    \
        in_dword_masked(HWIO_GE_INT_ENABLE_n_ADDR(n), mask)
#define HWIO_GE_INT_ENABLE_n_OUTI(n,val)    \
        out_dword(HWIO_GE_INT_ENABLE_n_ADDR(n),val)
#define HWIO_GE_INT_ENABLE_n_OUTMI(n,mask,val) \
        out_dword_masked_ns(HWIO_GE_INT_ENABLE_n_ADDR(n),mask,val,HWIO_GE_INT_ENABLE_n_INI(n))
#define HWIO_GE_INT_ENABLE_n_ENABLE_BMSK                   0xffffffff
#define HWIO_GE_INT_ENABLE_n_ENABLE_SHFT                          0x0

#define HWIO_GE_INT_ENABLE_CLEAR_n_ADDR(n)                 (GE_INT_CTL_REG_BASE      + 0x00000014 + 0x20 * (n))
#define HWIO_GE_INT_ENABLE_CLEAR_n_PHYS(n)                 (GE_INT_CTL_REG_BASE_PHYS + 0x00000014 + 0x20 * (n))
#define HWIO_GE_INT_ENABLE_CLEAR_n_OFFS(n)                 (GE_INT_CTL_REG_BASE_OFFS + 0x00000014 + 0x20 * (n))
#define HWIO_GE_INT_ENABLE_CLEAR_n_RMSK                    0xffffffff
#define HWIO_GE_INT_ENABLE_CLEAR_n_MAXn                             7
#define HWIO_GE_INT_ENABLE_CLEAR_n_POR                     0x00000000
#define HWIO_GE_INT_ENABLE_CLEAR_n_POR_RMSK                0xffffffff
#define HWIO_GE_INT_ENABLE_CLEAR_n_ATTR                           0x2
#define HWIO_GE_INT_ENABLE_CLEAR_n_OUTI(n,val)    \
        out_dword(HWIO_GE_INT_ENABLE_CLEAR_n_ADDR(n),val)
#define HWIO_GE_INT_ENABLE_CLEAR_n_CLEAR_BMSK              0xffffffff
#define HWIO_GE_INT_ENABLE_CLEAR_n_CLEAR_SHFT                     0x0

#define HWIO_GE_INT_ENABLE_SET_n_ADDR(n)                   (GE_INT_CTL_REG_BASE      + 0x00000018 + 0x20 * (n))
#define HWIO_GE_INT_ENABLE_SET_n_PHYS(n)                   (GE_INT_CTL_REG_BASE_PHYS + 0x00000018 + 0x20 * (n))
#define HWIO_GE_INT_ENABLE_SET_n_OFFS(n)                   (GE_INT_CTL_REG_BASE_OFFS + 0x00000018 + 0x20 * (n))
#define HWIO_GE_INT_ENABLE_SET_n_RMSK                      0xffffffff
#define HWIO_GE_INT_ENABLE_SET_n_MAXn                               7
#define HWIO_GE_INT_ENABLE_SET_n_POR                       0x00000000
#define HWIO_GE_INT_ENABLE_SET_n_POR_RMSK                  0xffffffff
#define HWIO_GE_INT_ENABLE_SET_n_ATTR                             0x2
#define HWIO_GE_INT_ENABLE_SET_n_OUTI(n,val)    \
        out_dword(HWIO_GE_INT_ENABLE_SET_n_ADDR(n),val)
#define HWIO_GE_INT_ENABLE_SET_n_SET_BMSK                  0xffffffff
#define HWIO_GE_INT_ENABLE_SET_n_SET_SHFT                         0x0

#define HWIO_GE_INT_TYPE_n_ADDR(n)                         (GE_INT_CTL_REG_BASE      + 0x0000001c + 0x20 * (n))
#define HWIO_GE_INT_TYPE_n_PHYS(n)                         (GE_INT_CTL_REG_BASE_PHYS + 0x0000001c + 0x20 * (n))
#define HWIO_GE_INT_TYPE_n_OFFS(n)                         (GE_INT_CTL_REG_BASE_OFFS + 0x0000001c + 0x20 * (n))
#define HWIO_GE_INT_TYPE_n_RMSK                            0xffffffff
#define HWIO_GE_INT_TYPE_n_MAXn                                     7
#define HWIO_GE_INT_TYPE_n_POR                             0x00000000
#define HWIO_GE_INT_TYPE_n_POR_RMSK                        0xffffffff
#define HWIO_GE_INT_TYPE_n_ATTR                                   0x3
#define HWIO_GE_INT_TYPE_n_INI(n)        \
        in_dword_masked(HWIO_GE_INT_TYPE_n_ADDR(n), HWIO_GE_INT_TYPE_n_RMSK)
#define HWIO_GE_INT_TYPE_n_INMI(n,mask)    \
        in_dword_masked(HWIO_GE_INT_TYPE_n_ADDR(n), mask)
#define HWIO_GE_INT_TYPE_n_OUTI(n,val)    \
        out_dword(HWIO_GE_INT_TYPE_n_ADDR(n),val)
#define HWIO_GE_INT_TYPE_n_OUTMI(n,mask,val) \
        out_dword_masked_ns(HWIO_GE_INT_TYPE_n_ADDR(n),mask,val,HWIO_GE_INT_TYPE_n_INI(n))
#define HWIO_GE_INT_TYPE_n_SRC_TYPE_BMSK                   0xffffffff
#define HWIO_GE_INT_TYPE_n_SRC_TYPE_SHFT                          0x0

#define HWIO_GE_IRQ_STATUS_n_ADDR(n)                       (GE_INT_CTL_REG_BASE      + 0x00000020 + 0x20 * (n))
#define HWIO_GE_IRQ_STATUS_n_PHYS(n)                       (GE_INT_CTL_REG_BASE_PHYS + 0x00000020 + 0x20 * (n))
#define HWIO_GE_IRQ_STATUS_n_OFFS(n)                       (GE_INT_CTL_REG_BASE_OFFS + 0x00000020 + 0x20 * (n))
#define HWIO_GE_IRQ_STATUS_n_RMSK                          0xffffffff
#define HWIO_GE_IRQ_STATUS_n_MAXn                                   7
#define HWIO_GE_IRQ_STATUS_n_POR                           0x00000000
#define HWIO_GE_IRQ_STATUS_n_POR_RMSK                      0xffffffff
#define HWIO_GE_IRQ_STATUS_n_ATTR                                 0x1
#define HWIO_GE_IRQ_STATUS_n_INI(n)        \
        in_dword_masked(HWIO_GE_IRQ_STATUS_n_ADDR(n), HWIO_GE_IRQ_STATUS_n_RMSK)
#define HWIO_GE_IRQ_STATUS_n_INMI(n,mask)    \
        in_dword_masked(HWIO_GE_IRQ_STATUS_n_ADDR(n), mask)
#define HWIO_GE_IRQ_STATUS_n_INT_SRC_BMSK                  0xffffffff
#define HWIO_GE_IRQ_STATUS_n_INT_SRC_SHFT                         0x0

#define HWIO_GE_RAW_STATUS_n_ADDR(n)                       (GE_INT_CTL_REG_BASE      + 0x00000024 + 0x20 * (n))
#define HWIO_GE_RAW_STATUS_n_PHYS(n)                       (GE_INT_CTL_REG_BASE_PHYS + 0x00000024 + 0x20 * (n))
#define HWIO_GE_RAW_STATUS_n_OFFS(n)                       (GE_INT_CTL_REG_BASE_OFFS + 0x00000024 + 0x20 * (n))
#define HWIO_GE_RAW_STATUS_n_RMSK                          0xffffffff
#define HWIO_GE_RAW_STATUS_n_MAXn                                   7
#define HWIO_GE_RAW_STATUS_n_POR                           0x00000000
#define HWIO_GE_RAW_STATUS_n_POR_RMSK                      0xffffffff
#define HWIO_GE_RAW_STATUS_n_ATTR                                 0x1
#define HWIO_GE_RAW_STATUS_n_INI(n)        \
        in_dword_masked(HWIO_GE_RAW_STATUS_n_ADDR(n), HWIO_GE_RAW_STATUS_n_RMSK)
#define HWIO_GE_RAW_STATUS_n_INMI(n,mask)    \
        in_dword_masked(HWIO_GE_RAW_STATUS_n_ADDR(n), mask)
#define HWIO_GE_RAW_STATUS_n_INT_SRC_BMSK                  0xffffffff
#define HWIO_GE_RAW_STATUS_n_INT_SRC_SHFT                         0x0

#define HWIO_GE_INT_CLEAR_n_ADDR(n)                        (GE_INT_CTL_REG_BASE      + 0x00000028 + 0x20 * (n))
#define HWIO_GE_INT_CLEAR_n_PHYS(n)                        (GE_INT_CTL_REG_BASE_PHYS + 0x00000028 + 0x20 * (n))
#define HWIO_GE_INT_CLEAR_n_OFFS(n)                        (GE_INT_CTL_REG_BASE_OFFS + 0x00000028 + 0x20 * (n))
#define HWIO_GE_INT_CLEAR_n_RMSK                           0xffffffff
#define HWIO_GE_INT_CLEAR_n_MAXn                                    7
#define HWIO_GE_INT_CLEAR_n_POR                            0x00000000
#define HWIO_GE_INT_CLEAR_n_POR_RMSK                       0xffffffff
#define HWIO_GE_INT_CLEAR_n_ATTR                                  0x2
#define HWIO_GE_INT_CLEAR_n_OUTI(n,val)    \
        out_dword(HWIO_GE_INT_CLEAR_n_ADDR(n),val)
#define HWIO_GE_INT_CLEAR_n_CLEAR_BMSK                     0xffffffff
#define HWIO_GE_INT_CLEAR_n_CLEAR_SHFT                            0x0

#define HWIO_GE_SOFT_INT_n_ADDR(n)                         (GE_INT_CTL_REG_BASE      + 0x0000002c + 0x20 * (n))
#define HWIO_GE_SOFT_INT_n_PHYS(n)                         (GE_INT_CTL_REG_BASE_PHYS + 0x0000002c + 0x20 * (n))
#define HWIO_GE_SOFT_INT_n_OFFS(n)                         (GE_INT_CTL_REG_BASE_OFFS + 0x0000002c + 0x20 * (n))
#define HWIO_GE_SOFT_INT_n_RMSK                            0xffffffff
#define HWIO_GE_SOFT_INT_n_MAXn                                     7
#define HWIO_GE_SOFT_INT_n_POR                             0x00000000
#define HWIO_GE_SOFT_INT_n_POR_RMSK                        0xffffffff
#define HWIO_GE_SOFT_INT_n_ATTR                                   0x2
#define HWIO_GE_SOFT_INT_n_OUTI(n,val)    \
        out_dword(HWIO_GE_SOFT_INT_n_ADDR(n),val)
#define HWIO_GE_SOFT_INT_n_SW_INT_BMSK                     0xffffffff
#define HWIO_GE_SOFT_INT_n_SW_INT_SHFT                            0x0

/*----------------------------------------------------------------------------
 * MODULE: GE_GEN_EMU_REGS_TOP
 *--------------------------------------------------------------------------*/

#define GE_GEN_EMU_REGS_TOP_REG_BASE (GE_GEN_EMU_BASE      + 0x001e0000)
#define GE_GEN_EMU_REGS_TOP_REG_BASE_PHYS (GE_GEN_EMU_BASE_PHYS + 0x001e0000)
#define GE_GEN_EMU_REGS_TOP_REG_BASE_OFFS 0x001e0000

/*----------------------------------------------------------------------------
 * MODULE: GE_GEN_EMU_REGS
 *--------------------------------------------------------------------------*/

#define GE_GEN_EMU_REGS_REG_BASE                                            (GE_GEN_EMU_BASE      + 0x001e0000)
#define GE_GEN_EMU_REGS_REG_BASE_PHYS                                       (GE_GEN_EMU_BASE_PHYS + 0x001e0000)
#define GE_GEN_EMU_REGS_REG_BASE_OFFS                                       0x001e0000

#define HWIO_GE_GEN_EMU_VERSION_ADDR                                        (GE_GEN_EMU_REGS_REG_BASE      + 0x00000000)
#define HWIO_GE_GEN_EMU_VERSION_PHYS                                        (GE_GEN_EMU_REGS_REG_BASE_PHYS + 0x00000000)
#define HWIO_GE_GEN_EMU_VERSION_OFFS                                        (GE_GEN_EMU_REGS_REG_BASE_OFFS + 0x00000000)
#define HWIO_GE_GEN_EMU_VERSION_RMSK                                        0xffffffff
#define HWIO_GE_GEN_EMU_VERSION_POR                                         0x00000000
#define HWIO_GE_GEN_EMU_VERSION_POR_RMSK                                    0xffffffff
#define HWIO_GE_GEN_EMU_VERSION_ATTR                                               0x1
#define HWIO_GE_GEN_EMU_VERSION_IN          \
        in_dword_masked(HWIO_GE_GEN_EMU_VERSION_ADDR, HWIO_GE_GEN_EMU_VERSION_RMSK)
#define HWIO_GE_GEN_EMU_VERSION_INM(m)      \
        in_dword_masked(HWIO_GE_GEN_EMU_VERSION_ADDR, m)
#define HWIO_GE_GEN_EMU_VERSION_SETUP_BMSK                                  0xff000000
#define HWIO_GE_GEN_EMU_VERSION_SETUP_SHFT                                        0x18
#define HWIO_GE_GEN_EMU_VERSION_MAJOR_BMSK                                    0xff0000
#define HWIO_GE_GEN_EMU_VERSION_MAJOR_SHFT                                        0x10
#define HWIO_GE_GEN_EMU_VERSION_MINOR_BMSK                                      0xfff0
#define HWIO_GE_GEN_EMU_VERSION_MINOR_SHFT                                         0x4
#define HWIO_GE_GEN_EMU_VERSION_STEP_BMSK                                          0xf
#define HWIO_GE_GEN_EMU_VERSION_STEP_SHFT                                          0x0

#define HWIO_GE_GEN_EMU_RESET_REG_ADDR                                      (GE_GEN_EMU_REGS_REG_BASE      + 0x00000004)
#define HWIO_GE_GEN_EMU_RESET_REG_PHYS                                      (GE_GEN_EMU_REGS_REG_BASE_PHYS + 0x00000004)
#define HWIO_GE_GEN_EMU_RESET_REG_OFFS                                      (GE_GEN_EMU_REGS_REG_BASE_OFFS + 0x00000004)
#define HWIO_GE_GEN_EMU_RESET_REG_RMSK                                      0x8000001f
#define HWIO_GE_GEN_EMU_RESET_REG_POR                                       0x00000002
#define HWIO_GE_GEN_EMU_RESET_REG_POR_RMSK                                  0xffffffff
#define HWIO_GE_GEN_EMU_RESET_REG_ATTR                                             0x3
#define HWIO_GE_GEN_EMU_RESET_REG_IN          \
        in_dword_masked(HWIO_GE_GEN_EMU_RESET_REG_ADDR, HWIO_GE_GEN_EMU_RESET_REG_RMSK)
#define HWIO_GE_GEN_EMU_RESET_REG_INM(m)      \
        in_dword_masked(HWIO_GE_GEN_EMU_RESET_REG_ADDR, m)
#define HWIO_GE_GEN_EMU_RESET_REG_OUT(v)      \
        out_dword(HWIO_GE_GEN_EMU_RESET_REG_ADDR,v)
#define HWIO_GE_GEN_EMU_RESET_REG_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GE_GEN_EMU_RESET_REG_ADDR,m,v,HWIO_GE_GEN_EMU_RESET_REG_IN)
#define HWIO_GE_GEN_EMU_RESET_REG_RESET_WARNING_BMSK                        0x80000000
#define HWIO_GE_GEN_EMU_RESET_REG_RESET_WARNING_SHFT                              0x1f
#define HWIO_GE_GEN_EMU_RESET_REG_SNIF_RESET_BMSK                                 0x10
#define HWIO_GE_GEN_EMU_RESET_REG_SNIF_RESET_SHFT                                  0x4
#define HWIO_GE_GEN_EMU_RESET_REG_UART_RESET_BMSK                                  0x8
#define HWIO_GE_GEN_EMU_RESET_REG_UART_RESET_SHFT                                  0x3
#define HWIO_GE_GEN_EMU_RESET_REG_DDR_RESET_BMSK                                   0x4
#define HWIO_GE_GEN_EMU_RESET_REG_DDR_RESET_SHFT                                   0x2
#define HWIO_GE_GEN_EMU_RESET_REG_CPU_RESET_BMSK                                   0x2
#define HWIO_GE_GEN_EMU_RESET_REG_CPU_RESET_SHFT                                   0x1
#define HWIO_GE_GEN_EMU_RESET_REG_SYS_RESET_BMSK                                   0x1
#define HWIO_GE_GEN_EMU_RESET_REG_SYS_RESET_SHFT                                   0x0

#define HWIO_GE_GEN_EMU_DUT_STATUS_REG_ADDR                                 (GE_GEN_EMU_REGS_REG_BASE      + 0x00000008)
#define HWIO_GE_GEN_EMU_DUT_STATUS_REG_PHYS                                 (GE_GEN_EMU_REGS_REG_BASE_PHYS + 0x00000008)
#define HWIO_GE_GEN_EMU_DUT_STATUS_REG_OFFS                                 (GE_GEN_EMU_REGS_REG_BASE_OFFS + 0x00000008)
#define HWIO_GE_GEN_EMU_DUT_STATUS_REG_RMSK                                 0x8003ffff
#define HWIO_GE_GEN_EMU_DUT_STATUS_REG_POR                                  0x00000000
#define HWIO_GE_GEN_EMU_DUT_STATUS_REG_POR_RMSK                             0xffffffff
#define HWIO_GE_GEN_EMU_DUT_STATUS_REG_ATTR                                        0x1
#define HWIO_GE_GEN_EMU_DUT_STATUS_REG_IN          \
        in_dword_masked(HWIO_GE_GEN_EMU_DUT_STATUS_REG_ADDR, HWIO_GE_GEN_EMU_DUT_STATUS_REG_RMSK)
#define HWIO_GE_GEN_EMU_DUT_STATUS_REG_INM(m)      \
        in_dword_masked(HWIO_GE_GEN_EMU_DUT_STATUS_REG_ADDR, m)
#define HWIO_GE_GEN_EMU_DUT_STATUS_REG_BACKPLANE_DETECTED_BMSK              0x80000000
#define HWIO_GE_GEN_EMU_DUT_STATUS_REG_BACKPLANE_DETECTED_SHFT                    0x1f
#define HWIO_GE_GEN_EMU_DUT_STATUS_REG_DDR_INITIALIZED_BMSK                    0x20000
#define HWIO_GE_GEN_EMU_DUT_STATUS_REG_DDR_INITIALIZED_SHFT                       0x11
#define HWIO_GE_GEN_EMU_DUT_STATUS_REG_DDR_FPGA_LOADED_BMSK                    0x10000
#define HWIO_GE_GEN_EMU_DUT_STATUS_REG_DDR_FPGA_LOADED_SHFT                       0x10
#define HWIO_GE_GEN_EMU_DUT_STATUS_REG_DUT_FPGA_READY_BMSK                      0xff00
#define HWIO_GE_GEN_EMU_DUT_STATUS_REG_DUT_FPGA_READY_SHFT                         0x8
#define HWIO_GE_GEN_EMU_DUT_STATUS_REG_DUT_FPGA_LOADED_BMSK                       0xff
#define HWIO_GE_GEN_EMU_DUT_STATUS_REG_DUT_FPGA_LOADED_SHFT                        0x0

#define HWIO_GE_GEN_EMU_DUT_CONNECT_REG_ADDR                                (GE_GEN_EMU_REGS_REG_BASE      + 0x0000000c)
#define HWIO_GE_GEN_EMU_DUT_CONNECT_REG_PHYS                                (GE_GEN_EMU_REGS_REG_BASE_PHYS + 0x0000000c)
#define HWIO_GE_GEN_EMU_DUT_CONNECT_REG_OFFS                                (GE_GEN_EMU_REGS_REG_BASE_OFFS + 0x0000000c)
#define HWIO_GE_GEN_EMU_DUT_CONNECT_REG_RMSK                                  0xf7777f
#define HWIO_GE_GEN_EMU_DUT_CONNECT_REG_POR                                 0x00f32101
#define HWIO_GE_GEN_EMU_DUT_CONNECT_REG_POR_RMSK                            0xffffffff
#define HWIO_GE_GEN_EMU_DUT_CONNECT_REG_ATTR                                       0x3
#define HWIO_GE_GEN_EMU_DUT_CONNECT_REG_IN          \
        in_dword_masked(HWIO_GE_GEN_EMU_DUT_CONNECT_REG_ADDR, HWIO_GE_GEN_EMU_DUT_CONNECT_REG_RMSK)
#define HWIO_GE_GEN_EMU_DUT_CONNECT_REG_INM(m)      \
        in_dword_masked(HWIO_GE_GEN_EMU_DUT_CONNECT_REG_ADDR, m)
#define HWIO_GE_GEN_EMU_DUT_CONNECT_REG_OUT(v)      \
        out_dword(HWIO_GE_GEN_EMU_DUT_CONNECT_REG_ADDR,v)
#define HWIO_GE_GEN_EMU_DUT_CONNECT_REG_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GE_GEN_EMU_DUT_CONNECT_REG_ADDR,m,v,HWIO_GE_GEN_EMU_DUT_CONNECT_REG_IN)
#define HWIO_GE_GEN_EMU_DUT_CONNECT_REG_JTAG_SEL_BMSK                         0xf00000
#define HWIO_GE_GEN_EMU_DUT_CONNECT_REG_JTAG_SEL_SHFT                             0x14
#define HWIO_GE_GEN_EMU_DUT_CONNECT_REG_DUT3_FPGA_SEL_BMSK                     0x70000
#define HWIO_GE_GEN_EMU_DUT_CONNECT_REG_DUT3_FPGA_SEL_SHFT                        0x10
#define HWIO_GE_GEN_EMU_DUT_CONNECT_REG_DUT2_FPGA_SEL_BMSK                      0x7000
#define HWIO_GE_GEN_EMU_DUT_CONNECT_REG_DUT2_FPGA_SEL_SHFT                         0xc
#define HWIO_GE_GEN_EMU_DUT_CONNECT_REG_DUT1_FPGA_SEL_BMSK                       0x700
#define HWIO_GE_GEN_EMU_DUT_CONNECT_REG_DUT1_FPGA_SEL_SHFT                         0x8
#define HWIO_GE_GEN_EMU_DUT_CONNECT_REG_DUT0_FPGA_SEL_BMSK                        0x70
#define HWIO_GE_GEN_EMU_DUT_CONNECT_REG_DUT0_FPGA_SEL_SHFT                         0x4
#define HWIO_GE_GEN_EMU_DUT_CONNECT_REG_DUT3_ON_BMSK                               0x8
#define HWIO_GE_GEN_EMU_DUT_CONNECT_REG_DUT3_ON_SHFT                               0x3
#define HWIO_GE_GEN_EMU_DUT_CONNECT_REG_DUT2_ON_BMSK                               0x4
#define HWIO_GE_GEN_EMU_DUT_CONNECT_REG_DUT2_ON_SHFT                               0x2
#define HWIO_GE_GEN_EMU_DUT_CONNECT_REG_DUT1_ON_BMSK                               0x2
#define HWIO_GE_GEN_EMU_DUT_CONNECT_REG_DUT1_ON_SHFT                               0x1
#define HWIO_GE_GEN_EMU_DUT_CONNECT_REG_DUT0_ON_BMSK                               0x1
#define HWIO_GE_GEN_EMU_DUT_CONNECT_REG_DUT0_ON_SHFT                               0x0

#define HWIO_GE_GEN_EMU_CLK_STATUS_REG_ADDR                                 (GE_GEN_EMU_REGS_REG_BASE      + 0x00000010)
#define HWIO_GE_GEN_EMU_CLK_STATUS_REG_PHYS                                 (GE_GEN_EMU_REGS_REG_BASE_PHYS + 0x00000010)
#define HWIO_GE_GEN_EMU_CLK_STATUS_REG_OFFS                                 (GE_GEN_EMU_REGS_REG_BASE_OFFS + 0x00000010)
#define HWIO_GE_GEN_EMU_CLK_STATUS_REG_RMSK                                    0xfffff
#define HWIO_GE_GEN_EMU_CLK_STATUS_REG_POR                                  0x00000000
#define HWIO_GE_GEN_EMU_CLK_STATUS_REG_POR_RMSK                             0xffffffff
#define HWIO_GE_GEN_EMU_CLK_STATUS_REG_ATTR                                        0x1
#define HWIO_GE_GEN_EMU_CLK_STATUS_REG_IN          \
        in_dword_masked(HWIO_GE_GEN_EMU_CLK_STATUS_REG_ADDR, HWIO_GE_GEN_EMU_CLK_STATUS_REG_RMSK)
#define HWIO_GE_GEN_EMU_CLK_STATUS_REG_INM(m)      \
        in_dword_masked(HWIO_GE_GEN_EMU_CLK_STATUS_REG_ADDR, m)
#define HWIO_GE_GEN_EMU_CLK_STATUS_REG_FREQUENCY_IN_KHZ_BMSK                   0xfffff
#define HWIO_GE_GEN_EMU_CLK_STATUS_REG_FREQUENCY_IN_KHZ_SHFT                       0x0

#define HWIO_GE_GEN_EMU_CLK_STATUS_SEL_REG_ADDR                             (GE_GEN_EMU_REGS_REG_BASE      + 0x00000014)
#define HWIO_GE_GEN_EMU_CLK_STATUS_SEL_REG_PHYS                             (GE_GEN_EMU_REGS_REG_BASE_PHYS + 0x00000014)
#define HWIO_GE_GEN_EMU_CLK_STATUS_SEL_REG_OFFS                             (GE_GEN_EMU_REGS_REG_BASE_OFFS + 0x00000014)
#define HWIO_GE_GEN_EMU_CLK_STATUS_SEL_REG_RMSK                                    0xf
#define HWIO_GE_GEN_EMU_CLK_STATUS_SEL_REG_POR                              0x00000000
#define HWIO_GE_GEN_EMU_CLK_STATUS_SEL_REG_POR_RMSK                         0xffffffff
#define HWIO_GE_GEN_EMU_CLK_STATUS_SEL_REG_ATTR                                    0x3
#define HWIO_GE_GEN_EMU_CLK_STATUS_SEL_REG_IN          \
        in_dword_masked(HWIO_GE_GEN_EMU_CLK_STATUS_SEL_REG_ADDR, HWIO_GE_GEN_EMU_CLK_STATUS_SEL_REG_RMSK)
#define HWIO_GE_GEN_EMU_CLK_STATUS_SEL_REG_INM(m)      \
        in_dword_masked(HWIO_GE_GEN_EMU_CLK_STATUS_SEL_REG_ADDR, m)
#define HWIO_GE_GEN_EMU_CLK_STATUS_SEL_REG_OUT(v)      \
        out_dword(HWIO_GE_GEN_EMU_CLK_STATUS_SEL_REG_ADDR,v)
#define HWIO_GE_GEN_EMU_CLK_STATUS_SEL_REG_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GE_GEN_EMU_CLK_STATUS_SEL_REG_ADDR,m,v,HWIO_GE_GEN_EMU_CLK_STATUS_SEL_REG_IN)
#define HWIO_GE_GEN_EMU_CLK_STATUS_SEL_REG_CLK_NUM_BMSK                            0xf
#define HWIO_GE_GEN_EMU_CLK_STATUS_SEL_REG_CLK_NUM_SHFT                            0x0

#define HWIO_GE_GEN_EMU_SE_PLL_CTRL_REG_ADDR                                (GE_GEN_EMU_REGS_REG_BASE      + 0x00000018)
#define HWIO_GE_GEN_EMU_SE_PLL_CTRL_REG_PHYS                                (GE_GEN_EMU_REGS_REG_BASE_PHYS + 0x00000018)
#define HWIO_GE_GEN_EMU_SE_PLL_CTRL_REG_OFFS                                (GE_GEN_EMU_REGS_REG_BASE_OFFS + 0x00000018)
#define HWIO_GE_GEN_EMU_SE_PLL_CTRL_REG_RMSK                                0x13ffffff
#define HWIO_GE_GEN_EMU_SE_PLL_CTRL_REG_POR                                 0x00000000
#define HWIO_GE_GEN_EMU_SE_PLL_CTRL_REG_POR_RMSK                            0xffffffff
#define HWIO_GE_GEN_EMU_SE_PLL_CTRL_REG_ATTR                                       0x3
#define HWIO_GE_GEN_EMU_SE_PLL_CTRL_REG_IN          \
        in_dword_masked(HWIO_GE_GEN_EMU_SE_PLL_CTRL_REG_ADDR, HWIO_GE_GEN_EMU_SE_PLL_CTRL_REG_RMSK)
#define HWIO_GE_GEN_EMU_SE_PLL_CTRL_REG_INM(m)      \
        in_dword_masked(HWIO_GE_GEN_EMU_SE_PLL_CTRL_REG_ADDR, m)
#define HWIO_GE_GEN_EMU_SE_PLL_CTRL_REG_OUT(v)      \
        out_dword(HWIO_GE_GEN_EMU_SE_PLL_CTRL_REG_ADDR,v)
#define HWIO_GE_GEN_EMU_SE_PLL_CTRL_REG_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GE_GEN_EMU_SE_PLL_CTRL_REG_ADDR,m,v,HWIO_GE_GEN_EMU_SE_PLL_CTRL_REG_IN)
#define HWIO_GE_GEN_EMU_SE_PLL_CTRL_REG_EXECUTE_BMSK                        0x10000000
#define HWIO_GE_GEN_EMU_SE_PLL_CTRL_REG_EXECUTE_SHFT                              0x1c
#define HWIO_GE_GEN_EMU_SE_PLL_CTRL_REG_PLL_SELECT_BMSK                      0x3000000
#define HWIO_GE_GEN_EMU_SE_PLL_CTRL_REG_PLL_SELECT_SHFT                           0x18
#define HWIO_GE_GEN_EMU_SE_PLL_CTRL_REG_INT_LOAD_CAP_BMSK                     0xc00000
#define HWIO_GE_GEN_EMU_SE_PLL_CTRL_REG_INT_LOAD_CAP_SHFT                         0x16
#define HWIO_GE_GEN_EMU_SE_PLL_CTRL_REG_TTL_DUTY_CYCLE_BMSK                   0x200000
#define HWIO_GE_GEN_EMU_SE_PLL_CTRL_REG_TTL_DUTY_CYCLE_SHFT                       0x15
#define HWIO_GE_GEN_EMU_SE_PLL_CTRL_REG_CLK2_FUNC_BMSK                        0x180000
#define HWIO_GE_GEN_EMU_SE_PLL_CTRL_REG_CLK2_FUNC_SHFT                            0x13
#define HWIO_GE_GEN_EMU_SE_PLL_CTRL_REG_OUT_DIV_SEL_BMSK                       0x70000
#define HWIO_GE_GEN_EMU_SE_PLL_CTRL_REG_OUT_DIV_SEL_SHFT                          0x10
#define HWIO_GE_GEN_EMU_SE_PLL_CTRL_REG_VCO_DIV_WORD_BMSK                       0xff80
#define HWIO_GE_GEN_EMU_SE_PLL_CTRL_REG_VCO_DIV_WORD_SHFT                          0x7
#define HWIO_GE_GEN_EMU_SE_PLL_CTRL_REG_REF_DIV_WORD_BMSK                         0x7f
#define HWIO_GE_GEN_EMU_SE_PLL_CTRL_REG_REF_DIV_WORD_SHFT                          0x0

#define HWIO_GE_GEN_EMU_HS_PLL_CTRL_REG_ADDR                                (GE_GEN_EMU_REGS_REG_BASE      + 0x0000001c)
#define HWIO_GE_GEN_EMU_HS_PLL_CTRL_REG_PHYS                                (GE_GEN_EMU_REGS_REG_BASE_PHYS + 0x0000001c)
#define HWIO_GE_GEN_EMU_HS_PLL_CTRL_REG_OFFS                                (GE_GEN_EMU_REGS_REG_BASE_OFFS + 0x0000001c)
#define HWIO_GE_GEN_EMU_HS_PLL_CTRL_REG_RMSK                                 0x11137ff
#define HWIO_GE_GEN_EMU_HS_PLL_CTRL_REG_POR                                 0x00000000
#define HWIO_GE_GEN_EMU_HS_PLL_CTRL_REG_POR_RMSK                            0xffffffff
#define HWIO_GE_GEN_EMU_HS_PLL_CTRL_REG_ATTR                                       0x3
#define HWIO_GE_GEN_EMU_HS_PLL_CTRL_REG_IN          \
        in_dword_masked(HWIO_GE_GEN_EMU_HS_PLL_CTRL_REG_ADDR, HWIO_GE_GEN_EMU_HS_PLL_CTRL_REG_RMSK)
#define HWIO_GE_GEN_EMU_HS_PLL_CTRL_REG_INM(m)      \
        in_dword_masked(HWIO_GE_GEN_EMU_HS_PLL_CTRL_REG_ADDR, m)
#define HWIO_GE_GEN_EMU_HS_PLL_CTRL_REG_OUT(v)      \
        out_dword(HWIO_GE_GEN_EMU_HS_PLL_CTRL_REG_ADDR,v)
#define HWIO_GE_GEN_EMU_HS_PLL_CTRL_REG_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GE_GEN_EMU_HS_PLL_CTRL_REG_ADDR,m,v,HWIO_GE_GEN_EMU_HS_PLL_CTRL_REG_IN)
#define HWIO_GE_GEN_EMU_HS_PLL_CTRL_REG_VCO_SEL_N_BMSK                       0x1000000
#define HWIO_GE_GEN_EMU_HS_PLL_CTRL_REG_VCO_SEL_N_SHFT                            0x18
#define HWIO_GE_GEN_EMU_HS_PLL_CTRL_REG_EXECUTE_BMSK                          0x100000
#define HWIO_GE_GEN_EMU_HS_PLL_CTRL_REG_EXECUTE_SHFT                              0x14
#define HWIO_GE_GEN_EMU_HS_PLL_CTRL_REG_PLL_SELECT_BMSK                        0x10000
#define HWIO_GE_GEN_EMU_HS_PLL_CTRL_REG_PLL_SELECT_SHFT                           0x10
#define HWIO_GE_GEN_EMU_HS_PLL_CTRL_REG_TEST_OUT_SEL_BMSK                       0x3000
#define HWIO_GE_GEN_EMU_HS_PLL_CTRL_REG_TEST_OUT_SEL_SHFT                          0xc
#define HWIO_GE_GEN_EMU_HS_PLL_CTRL_REG_OUT_DIV_N_BMSK                           0x600
#define HWIO_GE_GEN_EMU_HS_PLL_CTRL_REG_OUT_DIV_N_SHFT                             0x9
#define HWIO_GE_GEN_EMU_HS_PLL_CTRL_REG_VCO_DIV_M_BMSK                           0x1ff
#define HWIO_GE_GEN_EMU_HS_PLL_CTRL_REG_VCO_DIV_M_SHFT                             0x0

#define HWIO_GE_GEN_EMU_DMA_SRC_ADDR_LSB_REG_ADDR                           (GE_GEN_EMU_REGS_REG_BASE      + 0x00000020)
#define HWIO_GE_GEN_EMU_DMA_SRC_ADDR_LSB_REG_PHYS                           (GE_GEN_EMU_REGS_REG_BASE_PHYS + 0x00000020)
#define HWIO_GE_GEN_EMU_DMA_SRC_ADDR_LSB_REG_OFFS                           (GE_GEN_EMU_REGS_REG_BASE_OFFS + 0x00000020)
#define HWIO_GE_GEN_EMU_DMA_SRC_ADDR_LSB_REG_RMSK                           0xffffffff
#define HWIO_GE_GEN_EMU_DMA_SRC_ADDR_LSB_REG_POR                            0x00000000
#define HWIO_GE_GEN_EMU_DMA_SRC_ADDR_LSB_REG_POR_RMSK                       0xffffffff
#define HWIO_GE_GEN_EMU_DMA_SRC_ADDR_LSB_REG_ATTR                                  0x3
#define HWIO_GE_GEN_EMU_DMA_SRC_ADDR_LSB_REG_IN          \
        in_dword_masked(HWIO_GE_GEN_EMU_DMA_SRC_ADDR_LSB_REG_ADDR, HWIO_GE_GEN_EMU_DMA_SRC_ADDR_LSB_REG_RMSK)
#define HWIO_GE_GEN_EMU_DMA_SRC_ADDR_LSB_REG_INM(m)      \
        in_dword_masked(HWIO_GE_GEN_EMU_DMA_SRC_ADDR_LSB_REG_ADDR, m)
#define HWIO_GE_GEN_EMU_DMA_SRC_ADDR_LSB_REG_OUT(v)      \
        out_dword(HWIO_GE_GEN_EMU_DMA_SRC_ADDR_LSB_REG_ADDR,v)
#define HWIO_GE_GEN_EMU_DMA_SRC_ADDR_LSB_REG_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GE_GEN_EMU_DMA_SRC_ADDR_LSB_REG_ADDR,m,v,HWIO_GE_GEN_EMU_DMA_SRC_ADDR_LSB_REG_IN)
#define HWIO_GE_GEN_EMU_DMA_SRC_ADDR_LSB_REG_LSB_BMSK                       0xffffffff
#define HWIO_GE_GEN_EMU_DMA_SRC_ADDR_LSB_REG_LSB_SHFT                              0x0

#define HWIO_GE_GEN_EMU_DMA_SRC_ADDR_MSB_REG_ADDR                           (GE_GEN_EMU_REGS_REG_BASE      + 0x00000024)
#define HWIO_GE_GEN_EMU_DMA_SRC_ADDR_MSB_REG_PHYS                           (GE_GEN_EMU_REGS_REG_BASE_PHYS + 0x00000024)
#define HWIO_GE_GEN_EMU_DMA_SRC_ADDR_MSB_REG_OFFS                           (GE_GEN_EMU_REGS_REG_BASE_OFFS + 0x00000024)
#define HWIO_GE_GEN_EMU_DMA_SRC_ADDR_MSB_REG_RMSK                           0xffffffff
#define HWIO_GE_GEN_EMU_DMA_SRC_ADDR_MSB_REG_POR                            0x00000000
#define HWIO_GE_GEN_EMU_DMA_SRC_ADDR_MSB_REG_POR_RMSK                       0xffffffff
#define HWIO_GE_GEN_EMU_DMA_SRC_ADDR_MSB_REG_ATTR                                  0x3
#define HWIO_GE_GEN_EMU_DMA_SRC_ADDR_MSB_REG_IN          \
        in_dword_masked(HWIO_GE_GEN_EMU_DMA_SRC_ADDR_MSB_REG_ADDR, HWIO_GE_GEN_EMU_DMA_SRC_ADDR_MSB_REG_RMSK)
#define HWIO_GE_GEN_EMU_DMA_SRC_ADDR_MSB_REG_INM(m)      \
        in_dword_masked(HWIO_GE_GEN_EMU_DMA_SRC_ADDR_MSB_REG_ADDR, m)
#define HWIO_GE_GEN_EMU_DMA_SRC_ADDR_MSB_REG_OUT(v)      \
        out_dword(HWIO_GE_GEN_EMU_DMA_SRC_ADDR_MSB_REG_ADDR,v)
#define HWIO_GE_GEN_EMU_DMA_SRC_ADDR_MSB_REG_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GE_GEN_EMU_DMA_SRC_ADDR_MSB_REG_ADDR,m,v,HWIO_GE_GEN_EMU_DMA_SRC_ADDR_MSB_REG_IN)
#define HWIO_GE_GEN_EMU_DMA_SRC_ADDR_MSB_REG_MSB_BMSK                       0xffffffff
#define HWIO_GE_GEN_EMU_DMA_SRC_ADDR_MSB_REG_MSB_SHFT                              0x0

#define HWIO_GE_GEN_EMU_DMA_DST_ADDR_LSB_REG_ADDR                           (GE_GEN_EMU_REGS_REG_BASE      + 0x00000028)
#define HWIO_GE_GEN_EMU_DMA_DST_ADDR_LSB_REG_PHYS                           (GE_GEN_EMU_REGS_REG_BASE_PHYS + 0x00000028)
#define HWIO_GE_GEN_EMU_DMA_DST_ADDR_LSB_REG_OFFS                           (GE_GEN_EMU_REGS_REG_BASE_OFFS + 0x00000028)
#define HWIO_GE_GEN_EMU_DMA_DST_ADDR_LSB_REG_RMSK                           0xffffffff
#define HWIO_GE_GEN_EMU_DMA_DST_ADDR_LSB_REG_POR                            0x00000000
#define HWIO_GE_GEN_EMU_DMA_DST_ADDR_LSB_REG_POR_RMSK                       0xffffffff
#define HWIO_GE_GEN_EMU_DMA_DST_ADDR_LSB_REG_ATTR                                  0x3
#define HWIO_GE_GEN_EMU_DMA_DST_ADDR_LSB_REG_IN          \
        in_dword_masked(HWIO_GE_GEN_EMU_DMA_DST_ADDR_LSB_REG_ADDR, HWIO_GE_GEN_EMU_DMA_DST_ADDR_LSB_REG_RMSK)
#define HWIO_GE_GEN_EMU_DMA_DST_ADDR_LSB_REG_INM(m)      \
        in_dword_masked(HWIO_GE_GEN_EMU_DMA_DST_ADDR_LSB_REG_ADDR, m)
#define HWIO_GE_GEN_EMU_DMA_DST_ADDR_LSB_REG_OUT(v)      \
        out_dword(HWIO_GE_GEN_EMU_DMA_DST_ADDR_LSB_REG_ADDR,v)
#define HWIO_GE_GEN_EMU_DMA_DST_ADDR_LSB_REG_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GE_GEN_EMU_DMA_DST_ADDR_LSB_REG_ADDR,m,v,HWIO_GE_GEN_EMU_DMA_DST_ADDR_LSB_REG_IN)
#define HWIO_GE_GEN_EMU_DMA_DST_ADDR_LSB_REG_LSB_BMSK                       0xffffffff
#define HWIO_GE_GEN_EMU_DMA_DST_ADDR_LSB_REG_LSB_SHFT                              0x0

#define HWIO_GE_GEN_EMU_DMA_DST_ADDR_MSB_REG_ADDR                           (GE_GEN_EMU_REGS_REG_BASE      + 0x0000002c)
#define HWIO_GE_GEN_EMU_DMA_DST_ADDR_MSB_REG_PHYS                           (GE_GEN_EMU_REGS_REG_BASE_PHYS + 0x0000002c)
#define HWIO_GE_GEN_EMU_DMA_DST_ADDR_MSB_REG_OFFS                           (GE_GEN_EMU_REGS_REG_BASE_OFFS + 0x0000002c)
#define HWIO_GE_GEN_EMU_DMA_DST_ADDR_MSB_REG_RMSK                           0xffffffff
#define HWIO_GE_GEN_EMU_DMA_DST_ADDR_MSB_REG_POR                            0x00000000
#define HWIO_GE_GEN_EMU_DMA_DST_ADDR_MSB_REG_POR_RMSK                       0xffffffff
#define HWIO_GE_GEN_EMU_DMA_DST_ADDR_MSB_REG_ATTR                                  0x3
#define HWIO_GE_GEN_EMU_DMA_DST_ADDR_MSB_REG_IN          \
        in_dword_masked(HWIO_GE_GEN_EMU_DMA_DST_ADDR_MSB_REG_ADDR, HWIO_GE_GEN_EMU_DMA_DST_ADDR_MSB_REG_RMSK)
#define HWIO_GE_GEN_EMU_DMA_DST_ADDR_MSB_REG_INM(m)      \
        in_dword_masked(HWIO_GE_GEN_EMU_DMA_DST_ADDR_MSB_REG_ADDR, m)
#define HWIO_GE_GEN_EMU_DMA_DST_ADDR_MSB_REG_OUT(v)      \
        out_dword(HWIO_GE_GEN_EMU_DMA_DST_ADDR_MSB_REG_ADDR,v)
#define HWIO_GE_GEN_EMU_DMA_DST_ADDR_MSB_REG_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GE_GEN_EMU_DMA_DST_ADDR_MSB_REG_ADDR,m,v,HWIO_GE_GEN_EMU_DMA_DST_ADDR_MSB_REG_IN)
#define HWIO_GE_GEN_EMU_DMA_DST_ADDR_MSB_REG_MSB_BMSK                       0xffffffff
#define HWIO_GE_GEN_EMU_DMA_DST_ADDR_MSB_REG_MSB_SHFT                              0x0

#define HWIO_GE_GEN_EMU_DMA_LEN_LSB_REG_ADDR                                (GE_GEN_EMU_REGS_REG_BASE      + 0x00000030)
#define HWIO_GE_GEN_EMU_DMA_LEN_LSB_REG_PHYS                                (GE_GEN_EMU_REGS_REG_BASE_PHYS + 0x00000030)
#define HWIO_GE_GEN_EMU_DMA_LEN_LSB_REG_OFFS                                (GE_GEN_EMU_REGS_REG_BASE_OFFS + 0x00000030)
#define HWIO_GE_GEN_EMU_DMA_LEN_LSB_REG_RMSK                                0xffffffff
#define HWIO_GE_GEN_EMU_DMA_LEN_LSB_REG_POR                                 0x00000000
#define HWIO_GE_GEN_EMU_DMA_LEN_LSB_REG_POR_RMSK                            0xffffffff
#define HWIO_GE_GEN_EMU_DMA_LEN_LSB_REG_ATTR                                       0x3
#define HWIO_GE_GEN_EMU_DMA_LEN_LSB_REG_IN          \
        in_dword_masked(HWIO_GE_GEN_EMU_DMA_LEN_LSB_REG_ADDR, HWIO_GE_GEN_EMU_DMA_LEN_LSB_REG_RMSK)
#define HWIO_GE_GEN_EMU_DMA_LEN_LSB_REG_INM(m)      \
        in_dword_masked(HWIO_GE_GEN_EMU_DMA_LEN_LSB_REG_ADDR, m)
#define HWIO_GE_GEN_EMU_DMA_LEN_LSB_REG_OUT(v)      \
        out_dword(HWIO_GE_GEN_EMU_DMA_LEN_LSB_REG_ADDR,v)
#define HWIO_GE_GEN_EMU_DMA_LEN_LSB_REG_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GE_GEN_EMU_DMA_LEN_LSB_REG_ADDR,m,v,HWIO_GE_GEN_EMU_DMA_LEN_LSB_REG_IN)
#define HWIO_GE_GEN_EMU_DMA_LEN_LSB_REG_LSB_BMSK                            0xffffffff
#define HWIO_GE_GEN_EMU_DMA_LEN_LSB_REG_LSB_SHFT                                   0x0

#define HWIO_GE_GEN_EMU_DMA_LEN_MSB_REG_ADDR                                (GE_GEN_EMU_REGS_REG_BASE      + 0x00000034)
#define HWIO_GE_GEN_EMU_DMA_LEN_MSB_REG_PHYS                                (GE_GEN_EMU_REGS_REG_BASE_PHYS + 0x00000034)
#define HWIO_GE_GEN_EMU_DMA_LEN_MSB_REG_OFFS                                (GE_GEN_EMU_REGS_REG_BASE_OFFS + 0x00000034)
#define HWIO_GE_GEN_EMU_DMA_LEN_MSB_REG_RMSK                                0xffffffff
#define HWIO_GE_GEN_EMU_DMA_LEN_MSB_REG_POR                                 0x00000000
#define HWIO_GE_GEN_EMU_DMA_LEN_MSB_REG_POR_RMSK                            0xffffffff
#define HWIO_GE_GEN_EMU_DMA_LEN_MSB_REG_ATTR                                       0x3
#define HWIO_GE_GEN_EMU_DMA_LEN_MSB_REG_IN          \
        in_dword_masked(HWIO_GE_GEN_EMU_DMA_LEN_MSB_REG_ADDR, HWIO_GE_GEN_EMU_DMA_LEN_MSB_REG_RMSK)
#define HWIO_GE_GEN_EMU_DMA_LEN_MSB_REG_INM(m)      \
        in_dword_masked(HWIO_GE_GEN_EMU_DMA_LEN_MSB_REG_ADDR, m)
#define HWIO_GE_GEN_EMU_DMA_LEN_MSB_REG_OUT(v)      \
        out_dword(HWIO_GE_GEN_EMU_DMA_LEN_MSB_REG_ADDR,v)
#define HWIO_GE_GEN_EMU_DMA_LEN_MSB_REG_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GE_GEN_EMU_DMA_LEN_MSB_REG_ADDR,m,v,HWIO_GE_GEN_EMU_DMA_LEN_MSB_REG_IN)
#define HWIO_GE_GEN_EMU_DMA_LEN_MSB_REG_MSB_BMSK                            0xffffffff
#define HWIO_GE_GEN_EMU_DMA_LEN_MSB_REG_MSB_SHFT                                   0x0

#define HWIO_GE_GEN_EMU_DMA_CTL_STS_REG_ADDR                                (GE_GEN_EMU_REGS_REG_BASE      + 0x00000038)
#define HWIO_GE_GEN_EMU_DMA_CTL_STS_REG_PHYS                                (GE_GEN_EMU_REGS_REG_BASE_PHYS + 0x00000038)
#define HWIO_GE_GEN_EMU_DMA_CTL_STS_REG_OFFS                                (GE_GEN_EMU_REGS_REG_BASE_OFFS + 0x00000038)
#define HWIO_GE_GEN_EMU_DMA_CTL_STS_REG_RMSK                                 0x1f1f0f3
#define HWIO_GE_GEN_EMU_DMA_CTL_STS_REG_POR                                 0x00000000
#define HWIO_GE_GEN_EMU_DMA_CTL_STS_REG_POR_RMSK                            0xffffffff
#define HWIO_GE_GEN_EMU_DMA_CTL_STS_REG_ATTR                                       0x3
#define HWIO_GE_GEN_EMU_DMA_CTL_STS_REG_IN          \
        in_dword_masked(HWIO_GE_GEN_EMU_DMA_CTL_STS_REG_ADDR, HWIO_GE_GEN_EMU_DMA_CTL_STS_REG_RMSK)
#define HWIO_GE_GEN_EMU_DMA_CTL_STS_REG_INM(m)      \
        in_dword_masked(HWIO_GE_GEN_EMU_DMA_CTL_STS_REG_ADDR, m)
#define HWIO_GE_GEN_EMU_DMA_CTL_STS_REG_OUT(v)      \
        out_dword(HWIO_GE_GEN_EMU_DMA_CTL_STS_REG_ADDR,v)
#define HWIO_GE_GEN_EMU_DMA_CTL_STS_REG_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GE_GEN_EMU_DMA_CTL_STS_REG_ADDR,m,v,HWIO_GE_GEN_EMU_DMA_CTL_STS_REG_IN)
#define HWIO_GE_GEN_EMU_DMA_CTL_STS_REG_OUTSTANDING_WRITES_BMSK              0x1f00000
#define HWIO_GE_GEN_EMU_DMA_CTL_STS_REG_OUTSTANDING_WRITES_SHFT                   0x14
#define HWIO_GE_GEN_EMU_DMA_CTL_STS_REG_OUTSTANDING_READS_BMSK                 0x1f000
#define HWIO_GE_GEN_EMU_DMA_CTL_STS_REG_OUTSTANDING_READS_SHFT                     0xc
#define HWIO_GE_GEN_EMU_DMA_CTL_STS_REG_ACTIVE_DOORBELLS_BMSK                     0xf0
#define HWIO_GE_GEN_EMU_DMA_CTL_STS_REG_ACTIVE_DOORBELLS_SHFT                      0x4
#define HWIO_GE_GEN_EMU_DMA_CTL_STS_REG_ERROR_BMSK                                 0x2
#define HWIO_GE_GEN_EMU_DMA_CTL_STS_REG_ERROR_SHFT                                 0x1
#define HWIO_GE_GEN_EMU_DMA_CTL_STS_REG_GO_BMSK                                    0x1
#define HWIO_GE_GEN_EMU_DMA_CTL_STS_REG_GO_SHFT                                    0x0

#define HWIO_GE_GEN_EMU_MASTER_SEC_CTRL_REG_ADDR                            (GE_GEN_EMU_REGS_REG_BASE      + 0x00000040)
#define HWIO_GE_GEN_EMU_MASTER_SEC_CTRL_REG_PHYS                            (GE_GEN_EMU_REGS_REG_BASE_PHYS + 0x00000040)
#define HWIO_GE_GEN_EMU_MASTER_SEC_CTRL_REG_OFFS                            (GE_GEN_EMU_REGS_REG_BASE_OFFS + 0x00000040)
#define HWIO_GE_GEN_EMU_MASTER_SEC_CTRL_REG_RMSK                                0xffff
#define HWIO_GE_GEN_EMU_MASTER_SEC_CTRL_REG_POR                             0x00000000
#define HWIO_GE_GEN_EMU_MASTER_SEC_CTRL_REG_POR_RMSK                        0xffffffff
#define HWIO_GE_GEN_EMU_MASTER_SEC_CTRL_REG_ATTR                                   0x3
#define HWIO_GE_GEN_EMU_MASTER_SEC_CTRL_REG_IN          \
        in_dword_masked(HWIO_GE_GEN_EMU_MASTER_SEC_CTRL_REG_ADDR, HWIO_GE_GEN_EMU_MASTER_SEC_CTRL_REG_RMSK)
#define HWIO_GE_GEN_EMU_MASTER_SEC_CTRL_REG_INM(m)      \
        in_dword_masked(HWIO_GE_GEN_EMU_MASTER_SEC_CTRL_REG_ADDR, m)
#define HWIO_GE_GEN_EMU_MASTER_SEC_CTRL_REG_OUT(v)      \
        out_dword(HWIO_GE_GEN_EMU_MASTER_SEC_CTRL_REG_ADDR,v)
#define HWIO_GE_GEN_EMU_MASTER_SEC_CTRL_REG_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GE_GEN_EMU_MASTER_SEC_CTRL_REG_ADDR,m,v,HWIO_GE_GEN_EMU_MASTER_SEC_CTRL_REG_IN)
#define HWIO_GE_GEN_EMU_MASTER_SEC_CTRL_REG_CPU_SECUREPROC_BMSK                 0x8000
#define HWIO_GE_GEN_EMU_MASTER_SEC_CTRL_REG_CPU_SECUREPROC_SHFT                    0xf
#define HWIO_GE_GEN_EMU_MASTER_SEC_CTRL_REG_CPU_MSSSELFAUTH_BMSK                0x4000
#define HWIO_GE_GEN_EMU_MASTER_SEC_CTRL_REG_CPU_MSSSELFAUTH_SHFT                   0xe
#define HWIO_GE_GEN_EMU_MASTER_SEC_CTRL_REG_CPU_PROTNS_BMSK                     0x2000
#define HWIO_GE_GEN_EMU_MASTER_SEC_CTRL_REG_CPU_PROTNS_SHFT                        0xd
#define HWIO_GE_GEN_EMU_MASTER_SEC_CTRL_REG_CPU_VMID_BMSK                       0x1f00
#define HWIO_GE_GEN_EMU_MASTER_SEC_CTRL_REG_CPU_VMID_SHFT                          0x8
#define HWIO_GE_GEN_EMU_MASTER_SEC_CTRL_REG_PCIE_SECUREPROC_BMSK                  0x80
#define HWIO_GE_GEN_EMU_MASTER_SEC_CTRL_REG_PCIE_SECUREPROC_SHFT                   0x7
#define HWIO_GE_GEN_EMU_MASTER_SEC_CTRL_REG_PCIE_MSSSELFAUTH_BMSK                 0x40
#define HWIO_GE_GEN_EMU_MASTER_SEC_CTRL_REG_PCIE_MSSSELFAUTH_SHFT                  0x6
#define HWIO_GE_GEN_EMU_MASTER_SEC_CTRL_REG_PCIE_PROTNS_BMSK                      0x20
#define HWIO_GE_GEN_EMU_MASTER_SEC_CTRL_REG_PCIE_PROTNS_SHFT                       0x5
#define HWIO_GE_GEN_EMU_MASTER_SEC_CTRL_REG_PCIE_VMID_BMSK                        0x1f
#define HWIO_GE_GEN_EMU_MASTER_SEC_CTRL_REG_PCIE_VMID_SHFT                         0x0

#define HWIO_GE_GEN_EMU_PCIE_CTL_STS_REG_ADDR                               (GE_GEN_EMU_REGS_REG_BASE      + 0x00000044)
#define HWIO_GE_GEN_EMU_PCIE_CTL_STS_REG_PHYS                               (GE_GEN_EMU_REGS_REG_BASE_PHYS + 0x00000044)
#define HWIO_GE_GEN_EMU_PCIE_CTL_STS_REG_OFFS                               (GE_GEN_EMU_REGS_REG_BASE_OFFS + 0x00000044)
#define HWIO_GE_GEN_EMU_PCIE_CTL_STS_REG_RMSK                                    0x777
#define HWIO_GE_GEN_EMU_PCIE_CTL_STS_REG_POR                                0x00000000
#define HWIO_GE_GEN_EMU_PCIE_CTL_STS_REG_POR_RMSK                           0xffffffff
#define HWIO_GE_GEN_EMU_PCIE_CTL_STS_REG_ATTR                                      0x3
#define HWIO_GE_GEN_EMU_PCIE_CTL_STS_REG_IN          \
        in_dword_masked(HWIO_GE_GEN_EMU_PCIE_CTL_STS_REG_ADDR, HWIO_GE_GEN_EMU_PCIE_CTL_STS_REG_RMSK)
#define HWIO_GE_GEN_EMU_PCIE_CTL_STS_REG_INM(m)      \
        in_dword_masked(HWIO_GE_GEN_EMU_PCIE_CTL_STS_REG_ADDR, m)
#define HWIO_GE_GEN_EMU_PCIE_CTL_STS_REG_OUT(v)      \
        out_dword(HWIO_GE_GEN_EMU_PCIE_CTL_STS_REG_ADDR,v)
#define HWIO_GE_GEN_EMU_PCIE_CTL_STS_REG_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GE_GEN_EMU_PCIE_CTL_STS_REG_ADDR,m,v,HWIO_GE_GEN_EMU_PCIE_CTL_STS_REG_IN)
#define HWIO_GE_GEN_EMU_PCIE_CTL_STS_REG_CLKREQ_N_OE_BMSK                        0x400
#define HWIO_GE_GEN_EMU_PCIE_CTL_STS_REG_CLKREQ_N_OE_SHFT                          0xa
#define HWIO_GE_GEN_EMU_PCIE_CTL_STS_REG_CLKREQ_N_OUT_BMSK                       0x200
#define HWIO_GE_GEN_EMU_PCIE_CTL_STS_REG_CLKREQ_N_OUT_SHFT                         0x9
#define HWIO_GE_GEN_EMU_PCIE_CTL_STS_REG_CLKREQ_N_IN_BMSK                        0x100
#define HWIO_GE_GEN_EMU_PCIE_CTL_STS_REG_CLKREQ_N_IN_SHFT                          0x8
#define HWIO_GE_GEN_EMU_PCIE_CTL_STS_REG_WAKE_N_OE_BMSK                           0x40
#define HWIO_GE_GEN_EMU_PCIE_CTL_STS_REG_WAKE_N_OE_SHFT                            0x6
#define HWIO_GE_GEN_EMU_PCIE_CTL_STS_REG_WAKE_N_OUT_BMSK                          0x20
#define HWIO_GE_GEN_EMU_PCIE_CTL_STS_REG_WAKE_N_OUT_SHFT                           0x5
#define HWIO_GE_GEN_EMU_PCIE_CTL_STS_REG_WAKE_N_IN_BMSK                           0x10
#define HWIO_GE_GEN_EMU_PCIE_CTL_STS_REG_WAKE_N_IN_SHFT                            0x4
#define HWIO_GE_GEN_EMU_PCIE_CTL_STS_REG_PERST_N_OE_BMSK                           0x4
#define HWIO_GE_GEN_EMU_PCIE_CTL_STS_REG_PERST_N_OE_SHFT                           0x2
#define HWIO_GE_GEN_EMU_PCIE_CTL_STS_REG_PERST_N_OUT_BMSK                          0x2
#define HWIO_GE_GEN_EMU_PCIE_CTL_STS_REG_PERST_N_OUT_SHFT                          0x1
#define HWIO_GE_GEN_EMU_PCIE_CTL_STS_REG_PERST_N_IN_BMSK                           0x1
#define HWIO_GE_GEN_EMU_PCIE_CTL_STS_REG_PERST_N_IN_SHFT                           0x0

#define HWIO_GE_GEN_EMU_BUS_DELAY_CTRL_REG_ADDR                             (GE_GEN_EMU_REGS_REG_BASE      + 0x00000048)
#define HWIO_GE_GEN_EMU_BUS_DELAY_CTRL_REG_PHYS                             (GE_GEN_EMU_REGS_REG_BASE_PHYS + 0x00000048)
#define HWIO_GE_GEN_EMU_BUS_DELAY_CTRL_REG_OFFS                             (GE_GEN_EMU_REGS_REG_BASE_OFFS + 0x00000048)
#define HWIO_GE_GEN_EMU_BUS_DELAY_CTRL_REG_RMSK                                    0x7
#define HWIO_GE_GEN_EMU_BUS_DELAY_CTRL_REG_POR                              0x00000000
#define HWIO_GE_GEN_EMU_BUS_DELAY_CTRL_REG_POR_RMSK                         0xffffffff
#define HWIO_GE_GEN_EMU_BUS_DELAY_CTRL_REG_ATTR                                    0x3
#define HWIO_GE_GEN_EMU_BUS_DELAY_CTRL_REG_IN          \
        in_dword_masked(HWIO_GE_GEN_EMU_BUS_DELAY_CTRL_REG_ADDR, HWIO_GE_GEN_EMU_BUS_DELAY_CTRL_REG_RMSK)
#define HWIO_GE_GEN_EMU_BUS_DELAY_CTRL_REG_INM(m)      \
        in_dword_masked(HWIO_GE_GEN_EMU_BUS_DELAY_CTRL_REG_ADDR, m)
#define HWIO_GE_GEN_EMU_BUS_DELAY_CTRL_REG_OUT(v)      \
        out_dword(HWIO_GE_GEN_EMU_BUS_DELAY_CTRL_REG_ADDR,v)
#define HWIO_GE_GEN_EMU_BUS_DELAY_CTRL_REG_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GE_GEN_EMU_BUS_DELAY_CTRL_REG_ADDR,m,v,HWIO_GE_GEN_EMU_BUS_DELAY_CTRL_REG_IN)
#define HWIO_GE_GEN_EMU_BUS_DELAY_CTRL_REG_DUT_FPGA_SEL_BMSK                       0x7
#define HWIO_GE_GEN_EMU_BUS_DELAY_CTRL_REG_DUT_FPGA_SEL_SHFT                       0x0

#define HWIO_GE_GEN_EMU_BUS_DELAY_CTL_STS_REG_ADDR                          (GE_GEN_EMU_REGS_REG_BASE      + 0x0000004c)
#define HWIO_GE_GEN_EMU_BUS_DELAY_CTL_STS_REG_PHYS                          (GE_GEN_EMU_REGS_REG_BASE_PHYS + 0x0000004c)
#define HWIO_GE_GEN_EMU_BUS_DELAY_CTL_STS_REG_OFFS                          (GE_GEN_EMU_REGS_REG_BASE_OFFS + 0x0000004c)
#define HWIO_GE_GEN_EMU_BUS_DELAY_CTL_STS_REG_RMSK                          0x1ffffff3
#define HWIO_GE_GEN_EMU_BUS_DELAY_CTL_STS_REG_POR                           0x00000000
#define HWIO_GE_GEN_EMU_BUS_DELAY_CTL_STS_REG_POR_RMSK                      0xffffffff
#define HWIO_GE_GEN_EMU_BUS_DELAY_CTL_STS_REG_ATTR                                 0x3
#define HWIO_GE_GEN_EMU_BUS_DELAY_CTL_STS_REG_IN          \
        in_dword_masked(HWIO_GE_GEN_EMU_BUS_DELAY_CTL_STS_REG_ADDR, HWIO_GE_GEN_EMU_BUS_DELAY_CTL_STS_REG_RMSK)
#define HWIO_GE_GEN_EMU_BUS_DELAY_CTL_STS_REG_INM(m)      \
        in_dword_masked(HWIO_GE_GEN_EMU_BUS_DELAY_CTL_STS_REG_ADDR, m)
#define HWIO_GE_GEN_EMU_BUS_DELAY_CTL_STS_REG_OUT(v)      \
        out_dword(HWIO_GE_GEN_EMU_BUS_DELAY_CTL_STS_REG_ADDR,v)
#define HWIO_GE_GEN_EMU_BUS_DELAY_CTL_STS_REG_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GE_GEN_EMU_BUS_DELAY_CTL_STS_REG_ADDR,m,v,HWIO_GE_GEN_EMU_BUS_DELAY_CTL_STS_REG_IN)
#define HWIO_GE_GEN_EMU_BUS_DELAY_CTL_STS_REG_DELAY_RAND_VAL_BMSK           0x1f000000
#define HWIO_GE_GEN_EMU_BUS_DELAY_CTL_STS_REG_DELAY_RAND_VAL_SHFT                 0x18
#define HWIO_GE_GEN_EMU_BUS_DELAY_CTL_STS_REG_DELAY_CONST_VAL_BMSK            0xfffff0
#define HWIO_GE_GEN_EMU_BUS_DELAY_CTL_STS_REG_DELAY_CONST_VAL_SHFT                 0x4
#define HWIO_GE_GEN_EMU_BUS_DELAY_CTL_STS_REG_DELAY_LFSR_EN_BMSK                   0x2
#define HWIO_GE_GEN_EMU_BUS_DELAY_CTL_STS_REG_DELAY_LFSR_EN_SHFT                   0x1
#define HWIO_GE_GEN_EMU_BUS_DELAY_CTL_STS_REG_DELAY_CONST_EN_BMSK                  0x1
#define HWIO_GE_GEN_EMU_BUS_DELAY_CTL_STS_REG_DELAY_CONST_EN_SHFT                  0x0

#define HWIO_GE_GEN_EMU_QTIMER_LSB_REG_ADDR                                 (GE_GEN_EMU_REGS_REG_BASE      + 0x00000050)
#define HWIO_GE_GEN_EMU_QTIMER_LSB_REG_PHYS                                 (GE_GEN_EMU_REGS_REG_BASE_PHYS + 0x00000050)
#define HWIO_GE_GEN_EMU_QTIMER_LSB_REG_OFFS                                 (GE_GEN_EMU_REGS_REG_BASE_OFFS + 0x00000050)
#define HWIO_GE_GEN_EMU_QTIMER_LSB_REG_RMSK                                 0xffffffff
#define HWIO_GE_GEN_EMU_QTIMER_LSB_REG_POR                                  0x00000000
#define HWIO_GE_GEN_EMU_QTIMER_LSB_REG_POR_RMSK                             0xffffffff
#define HWIO_GE_GEN_EMU_QTIMER_LSB_REG_ATTR                                        0x1
#define HWIO_GE_GEN_EMU_QTIMER_LSB_REG_IN          \
        in_dword_masked(HWIO_GE_GEN_EMU_QTIMER_LSB_REG_ADDR, HWIO_GE_GEN_EMU_QTIMER_LSB_REG_RMSK)
#define HWIO_GE_GEN_EMU_QTIMER_LSB_REG_INM(m)      \
        in_dword_masked(HWIO_GE_GEN_EMU_QTIMER_LSB_REG_ADDR, m)
#define HWIO_GE_GEN_EMU_QTIMER_LSB_REG_GRAY_LSB_BMSK                        0xffffffff
#define HWIO_GE_GEN_EMU_QTIMER_LSB_REG_GRAY_LSB_SHFT                               0x0

#define HWIO_GE_GEN_EMU_QTIMER_MSB_REG_ADDR                                 (GE_GEN_EMU_REGS_REG_BASE      + 0x00000054)
#define HWIO_GE_GEN_EMU_QTIMER_MSB_REG_PHYS                                 (GE_GEN_EMU_REGS_REG_BASE_PHYS + 0x00000054)
#define HWIO_GE_GEN_EMU_QTIMER_MSB_REG_OFFS                                 (GE_GEN_EMU_REGS_REG_BASE_OFFS + 0x00000054)
#define HWIO_GE_GEN_EMU_QTIMER_MSB_REG_RMSK                                   0xffffff
#define HWIO_GE_GEN_EMU_QTIMER_MSB_REG_POR                                  0x00000000
#define HWIO_GE_GEN_EMU_QTIMER_MSB_REG_POR_RMSK                             0xffffffff
#define HWIO_GE_GEN_EMU_QTIMER_MSB_REG_ATTR                                        0x1
#define HWIO_GE_GEN_EMU_QTIMER_MSB_REG_IN          \
        in_dword_masked(HWIO_GE_GEN_EMU_QTIMER_MSB_REG_ADDR, HWIO_GE_GEN_EMU_QTIMER_MSB_REG_RMSK)
#define HWIO_GE_GEN_EMU_QTIMER_MSB_REG_INM(m)      \
        in_dword_masked(HWIO_GE_GEN_EMU_QTIMER_MSB_REG_ADDR, m)
#define HWIO_GE_GEN_EMU_QTIMER_MSB_REG_GRAY_MSB_BMSK                          0xffffff
#define HWIO_GE_GEN_EMU_QTIMER_MSB_REG_GRAY_MSB_SHFT                               0x0

#define HWIO_GE_GEN_EMU_BTIMER_LSB_REG_ADDR                                 (GE_GEN_EMU_REGS_REG_BASE      + 0x00000058)
#define HWIO_GE_GEN_EMU_BTIMER_LSB_REG_PHYS                                 (GE_GEN_EMU_REGS_REG_BASE_PHYS + 0x00000058)
#define HWIO_GE_GEN_EMU_BTIMER_LSB_REG_OFFS                                 (GE_GEN_EMU_REGS_REG_BASE_OFFS + 0x00000058)
#define HWIO_GE_GEN_EMU_BTIMER_LSB_REG_RMSK                                 0xffffffff
#define HWIO_GE_GEN_EMU_BTIMER_LSB_REG_POR                                  0x00000000
#define HWIO_GE_GEN_EMU_BTIMER_LSB_REG_POR_RMSK                             0xffffffff
#define HWIO_GE_GEN_EMU_BTIMER_LSB_REG_ATTR                                        0x1
#define HWIO_GE_GEN_EMU_BTIMER_LSB_REG_IN          \
        in_dword_masked(HWIO_GE_GEN_EMU_BTIMER_LSB_REG_ADDR, HWIO_GE_GEN_EMU_BTIMER_LSB_REG_RMSK)
#define HWIO_GE_GEN_EMU_BTIMER_LSB_REG_INM(m)      \
        in_dword_masked(HWIO_GE_GEN_EMU_BTIMER_LSB_REG_ADDR, m)
#define HWIO_GE_GEN_EMU_BTIMER_LSB_REG_BINARY_LSB_BMSK                      0xffffffff
#define HWIO_GE_GEN_EMU_BTIMER_LSB_REG_BINARY_LSB_SHFT                             0x0

#define HWIO_GE_GEN_EMU_BTIMER_MSB_REG_ADDR                                 (GE_GEN_EMU_REGS_REG_BASE      + 0x0000005c)
#define HWIO_GE_GEN_EMU_BTIMER_MSB_REG_PHYS                                 (GE_GEN_EMU_REGS_REG_BASE_PHYS + 0x0000005c)
#define HWIO_GE_GEN_EMU_BTIMER_MSB_REG_OFFS                                 (GE_GEN_EMU_REGS_REG_BASE_OFFS + 0x0000005c)
#define HWIO_GE_GEN_EMU_BTIMER_MSB_REG_RMSK                                   0xffffff
#define HWIO_GE_GEN_EMU_BTIMER_MSB_REG_POR                                  0x00000000
#define HWIO_GE_GEN_EMU_BTIMER_MSB_REG_POR_RMSK                             0xffffffff
#define HWIO_GE_GEN_EMU_BTIMER_MSB_REG_ATTR                                        0x1
#define HWIO_GE_GEN_EMU_BTIMER_MSB_REG_IN          \
        in_dword_masked(HWIO_GE_GEN_EMU_BTIMER_MSB_REG_ADDR, HWIO_GE_GEN_EMU_BTIMER_MSB_REG_RMSK)
#define HWIO_GE_GEN_EMU_BTIMER_MSB_REG_INM(m)      \
        in_dword_masked(HWIO_GE_GEN_EMU_BTIMER_MSB_REG_ADDR, m)
#define HWIO_GE_GEN_EMU_BTIMER_MSB_REG_BINARY_MSB_BMSK                        0xffffff
#define HWIO_GE_GEN_EMU_BTIMER_MSB_REG_BINARY_MSB_SHFT                             0x0

#define HWIO_GE_GEN_EMU_UART_CTL_STS_REG_ADDR                               (GE_GEN_EMU_REGS_REG_BASE      + 0x00000060)
#define HWIO_GE_GEN_EMU_UART_CTL_STS_REG_PHYS                               (GE_GEN_EMU_REGS_REG_BASE_PHYS + 0x00000060)
#define HWIO_GE_GEN_EMU_UART_CTL_STS_REG_OFFS                               (GE_GEN_EMU_REGS_REG_BASE_OFFS + 0x00000060)
#define HWIO_GE_GEN_EMU_UART_CTL_STS_REG_RMSK                                   0x11ff
#define HWIO_GE_GEN_EMU_UART_CTL_STS_REG_POR                                0x00000108
#define HWIO_GE_GEN_EMU_UART_CTL_STS_REG_POR_RMSK                           0xffffffff
#define HWIO_GE_GEN_EMU_UART_CTL_STS_REG_ATTR                                      0x3
#define HWIO_GE_GEN_EMU_UART_CTL_STS_REG_IN          \
        in_dword_masked(HWIO_GE_GEN_EMU_UART_CTL_STS_REG_ADDR, HWIO_GE_GEN_EMU_UART_CTL_STS_REG_RMSK)
#define HWIO_GE_GEN_EMU_UART_CTL_STS_REG_INM(m)      \
        in_dword_masked(HWIO_GE_GEN_EMU_UART_CTL_STS_REG_ADDR, m)
#define HWIO_GE_GEN_EMU_UART_CTL_STS_REG_OUT(v)      \
        out_dword(HWIO_GE_GEN_EMU_UART_CTL_STS_REG_ADDR,v)
#define HWIO_GE_GEN_EMU_UART_CTL_STS_REG_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GE_GEN_EMU_UART_CTL_STS_REG_ADDR,m,v,HWIO_GE_GEN_EMU_UART_CTL_STS_REG_IN)
#define HWIO_GE_GEN_EMU_UART_CTL_STS_REG_RX_ERROR_BMSK                          0x1000
#define HWIO_GE_GEN_EMU_UART_CTL_STS_REG_RX_ERROR_SHFT                             0xc
#define HWIO_GE_GEN_EMU_UART_CTL_STS_REG_ECHO_EN_BMSK                            0x100
#define HWIO_GE_GEN_EMU_UART_CTL_STS_REG_ECHO_EN_SHFT                              0x8
#define HWIO_GE_GEN_EMU_UART_CTL_STS_REG_BAUD_DIV_BMSK                            0xff
#define HWIO_GE_GEN_EMU_UART_CTL_STS_REG_BAUD_DIV_SHFT                             0x0

#define HWIO_GE_GEN_EMU_SAFEGUARD_CTL_REG_ADDR                              (GE_GEN_EMU_REGS_REG_BASE      + 0x00000064)
#define HWIO_GE_GEN_EMU_SAFEGUARD_CTL_REG_PHYS                              (GE_GEN_EMU_REGS_REG_BASE_PHYS + 0x00000064)
#define HWIO_GE_GEN_EMU_SAFEGUARD_CTL_REG_OFFS                              (GE_GEN_EMU_REGS_REG_BASE_OFFS + 0x00000064)
#define HWIO_GE_GEN_EMU_SAFEGUARD_CTL_REG_RMSK                               0x1ffffff
#define HWIO_GE_GEN_EMU_SAFEGUARD_CTL_REG_POR                               0x000ff0ff
#define HWIO_GE_GEN_EMU_SAFEGUARD_CTL_REG_POR_RMSK                          0xffffffff
#define HWIO_GE_GEN_EMU_SAFEGUARD_CTL_REG_ATTR                                     0x3
#define HWIO_GE_GEN_EMU_SAFEGUARD_CTL_REG_IN          \
        in_dword_masked(HWIO_GE_GEN_EMU_SAFEGUARD_CTL_REG_ADDR, HWIO_GE_GEN_EMU_SAFEGUARD_CTL_REG_RMSK)
#define HWIO_GE_GEN_EMU_SAFEGUARD_CTL_REG_INM(m)      \
        in_dword_masked(HWIO_GE_GEN_EMU_SAFEGUARD_CTL_REG_ADDR, m)
#define HWIO_GE_GEN_EMU_SAFEGUARD_CTL_REG_OUT(v)      \
        out_dword(HWIO_GE_GEN_EMU_SAFEGUARD_CTL_REG_ADDR,v)
#define HWIO_GE_GEN_EMU_SAFEGUARD_CTL_REG_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GE_GEN_EMU_SAFEGUARD_CTL_REG_ADDR,m,v,HWIO_GE_GEN_EMU_SAFEGUARD_CTL_REG_IN)
#define HWIO_GE_GEN_EMU_SAFEGUARD_CTL_REG_AUTOEXIT_EN_BMSK                   0x1000000
#define HWIO_GE_GEN_EMU_SAFEGUARD_CTL_REG_AUTOEXIT_EN_SHFT                        0x18
#define HWIO_GE_GEN_EMU_SAFEGUARD_CTL_REG_AUTOEXIT_CYCLES_BMSK                0xfff000
#define HWIO_GE_GEN_EMU_SAFEGUARD_CTL_REG_AUTOEXIT_CYCLES_SHFT                     0xc
#define HWIO_GE_GEN_EMU_SAFEGUARD_CTL_REG_ENTRY_CYCLES_BMSK                      0xfff
#define HWIO_GE_GEN_EMU_SAFEGUARD_CTL_REG_ENTRY_CYCLES_SHFT                        0x0

#define HWIO_GE_GEN_EMU_SNIFFER_START_ADDR_LSB_REG_ADDR                     (GE_GEN_EMU_REGS_REG_BASE      + 0x00000070)
#define HWIO_GE_GEN_EMU_SNIFFER_START_ADDR_LSB_REG_PHYS                     (GE_GEN_EMU_REGS_REG_BASE_PHYS + 0x00000070)
#define HWIO_GE_GEN_EMU_SNIFFER_START_ADDR_LSB_REG_OFFS                     (GE_GEN_EMU_REGS_REG_BASE_OFFS + 0x00000070)
#define HWIO_GE_GEN_EMU_SNIFFER_START_ADDR_LSB_REG_RMSK                     0xffffffff
#define HWIO_GE_GEN_EMU_SNIFFER_START_ADDR_LSB_REG_POR                      0x00000000
#define HWIO_GE_GEN_EMU_SNIFFER_START_ADDR_LSB_REG_POR_RMSK                 0xffffffff
#define HWIO_GE_GEN_EMU_SNIFFER_START_ADDR_LSB_REG_ATTR                            0x3
#define HWIO_GE_GEN_EMU_SNIFFER_START_ADDR_LSB_REG_IN          \
        in_dword_masked(HWIO_GE_GEN_EMU_SNIFFER_START_ADDR_LSB_REG_ADDR, HWIO_GE_GEN_EMU_SNIFFER_START_ADDR_LSB_REG_RMSK)
#define HWIO_GE_GEN_EMU_SNIFFER_START_ADDR_LSB_REG_INM(m)      \
        in_dword_masked(HWIO_GE_GEN_EMU_SNIFFER_START_ADDR_LSB_REG_ADDR, m)
#define HWIO_GE_GEN_EMU_SNIFFER_START_ADDR_LSB_REG_OUT(v)      \
        out_dword(HWIO_GE_GEN_EMU_SNIFFER_START_ADDR_LSB_REG_ADDR,v)
#define HWIO_GE_GEN_EMU_SNIFFER_START_ADDR_LSB_REG_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GE_GEN_EMU_SNIFFER_START_ADDR_LSB_REG_ADDR,m,v,HWIO_GE_GEN_EMU_SNIFFER_START_ADDR_LSB_REG_IN)
#define HWIO_GE_GEN_EMU_SNIFFER_START_ADDR_LSB_REG_LSB_BMSK                 0xffffffff
#define HWIO_GE_GEN_EMU_SNIFFER_START_ADDR_LSB_REG_LSB_SHFT                        0x0

#define HWIO_GE_GEN_EMU_SNIFFER_START_ADDR_MSB_REG_ADDR                     (GE_GEN_EMU_REGS_REG_BASE      + 0x00000074)
#define HWIO_GE_GEN_EMU_SNIFFER_START_ADDR_MSB_REG_PHYS                     (GE_GEN_EMU_REGS_REG_BASE_PHYS + 0x00000074)
#define HWIO_GE_GEN_EMU_SNIFFER_START_ADDR_MSB_REG_OFFS                     (GE_GEN_EMU_REGS_REG_BASE_OFFS + 0x00000074)
#define HWIO_GE_GEN_EMU_SNIFFER_START_ADDR_MSB_REG_RMSK                            0xf
#define HWIO_GE_GEN_EMU_SNIFFER_START_ADDR_MSB_REG_POR                      0x00000000
#define HWIO_GE_GEN_EMU_SNIFFER_START_ADDR_MSB_REG_POR_RMSK                 0xffffffff
#define HWIO_GE_GEN_EMU_SNIFFER_START_ADDR_MSB_REG_ATTR                            0x3
#define HWIO_GE_GEN_EMU_SNIFFER_START_ADDR_MSB_REG_IN          \
        in_dword_masked(HWIO_GE_GEN_EMU_SNIFFER_START_ADDR_MSB_REG_ADDR, HWIO_GE_GEN_EMU_SNIFFER_START_ADDR_MSB_REG_RMSK)
#define HWIO_GE_GEN_EMU_SNIFFER_START_ADDR_MSB_REG_INM(m)      \
        in_dword_masked(HWIO_GE_GEN_EMU_SNIFFER_START_ADDR_MSB_REG_ADDR, m)
#define HWIO_GE_GEN_EMU_SNIFFER_START_ADDR_MSB_REG_OUT(v)      \
        out_dword(HWIO_GE_GEN_EMU_SNIFFER_START_ADDR_MSB_REG_ADDR,v)
#define HWIO_GE_GEN_EMU_SNIFFER_START_ADDR_MSB_REG_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GE_GEN_EMU_SNIFFER_START_ADDR_MSB_REG_ADDR,m,v,HWIO_GE_GEN_EMU_SNIFFER_START_ADDR_MSB_REG_IN)
#define HWIO_GE_GEN_EMU_SNIFFER_START_ADDR_MSB_REG_MSB_BMSK                        0xf
#define HWIO_GE_GEN_EMU_SNIFFER_START_ADDR_MSB_REG_MSB_SHFT                        0x0

#define HWIO_GE_GEN_EMU_SNIFFER_END_ADDR_LSB_REG_ADDR                       (GE_GEN_EMU_REGS_REG_BASE      + 0x00000078)
#define HWIO_GE_GEN_EMU_SNIFFER_END_ADDR_LSB_REG_PHYS                       (GE_GEN_EMU_REGS_REG_BASE_PHYS + 0x00000078)
#define HWIO_GE_GEN_EMU_SNIFFER_END_ADDR_LSB_REG_OFFS                       (GE_GEN_EMU_REGS_REG_BASE_OFFS + 0x00000078)
#define HWIO_GE_GEN_EMU_SNIFFER_END_ADDR_LSB_REG_RMSK                       0xffffffff
#define HWIO_GE_GEN_EMU_SNIFFER_END_ADDR_LSB_REG_POR                        0x00000000
#define HWIO_GE_GEN_EMU_SNIFFER_END_ADDR_LSB_REG_POR_RMSK                   0xffffffff
#define HWIO_GE_GEN_EMU_SNIFFER_END_ADDR_LSB_REG_ATTR                              0x3
#define HWIO_GE_GEN_EMU_SNIFFER_END_ADDR_LSB_REG_IN          \
        in_dword_masked(HWIO_GE_GEN_EMU_SNIFFER_END_ADDR_LSB_REG_ADDR, HWIO_GE_GEN_EMU_SNIFFER_END_ADDR_LSB_REG_RMSK)
#define HWIO_GE_GEN_EMU_SNIFFER_END_ADDR_LSB_REG_INM(m)      \
        in_dword_masked(HWIO_GE_GEN_EMU_SNIFFER_END_ADDR_LSB_REG_ADDR, m)
#define HWIO_GE_GEN_EMU_SNIFFER_END_ADDR_LSB_REG_OUT(v)      \
        out_dword(HWIO_GE_GEN_EMU_SNIFFER_END_ADDR_LSB_REG_ADDR,v)
#define HWIO_GE_GEN_EMU_SNIFFER_END_ADDR_LSB_REG_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GE_GEN_EMU_SNIFFER_END_ADDR_LSB_REG_ADDR,m,v,HWIO_GE_GEN_EMU_SNIFFER_END_ADDR_LSB_REG_IN)
#define HWIO_GE_GEN_EMU_SNIFFER_END_ADDR_LSB_REG_LSB_BMSK                   0xffffffff
#define HWIO_GE_GEN_EMU_SNIFFER_END_ADDR_LSB_REG_LSB_SHFT                          0x0

#define HWIO_GE_GEN_EMU_SNIFFER_END_ADDR_MSB_REG_ADDR                       (GE_GEN_EMU_REGS_REG_BASE      + 0x0000007c)
#define HWIO_GE_GEN_EMU_SNIFFER_END_ADDR_MSB_REG_PHYS                       (GE_GEN_EMU_REGS_REG_BASE_PHYS + 0x0000007c)
#define HWIO_GE_GEN_EMU_SNIFFER_END_ADDR_MSB_REG_OFFS                       (GE_GEN_EMU_REGS_REG_BASE_OFFS + 0x0000007c)
#define HWIO_GE_GEN_EMU_SNIFFER_END_ADDR_MSB_REG_RMSK                              0xf
#define HWIO_GE_GEN_EMU_SNIFFER_END_ADDR_MSB_REG_POR                        0x00000000
#define HWIO_GE_GEN_EMU_SNIFFER_END_ADDR_MSB_REG_POR_RMSK                   0xffffffff
#define HWIO_GE_GEN_EMU_SNIFFER_END_ADDR_MSB_REG_ATTR                              0x3
#define HWIO_GE_GEN_EMU_SNIFFER_END_ADDR_MSB_REG_IN          \
        in_dword_masked(HWIO_GE_GEN_EMU_SNIFFER_END_ADDR_MSB_REG_ADDR, HWIO_GE_GEN_EMU_SNIFFER_END_ADDR_MSB_REG_RMSK)
#define HWIO_GE_GEN_EMU_SNIFFER_END_ADDR_MSB_REG_INM(m)      \
        in_dword_masked(HWIO_GE_GEN_EMU_SNIFFER_END_ADDR_MSB_REG_ADDR, m)
#define HWIO_GE_GEN_EMU_SNIFFER_END_ADDR_MSB_REG_OUT(v)      \
        out_dword(HWIO_GE_GEN_EMU_SNIFFER_END_ADDR_MSB_REG_ADDR,v)
#define HWIO_GE_GEN_EMU_SNIFFER_END_ADDR_MSB_REG_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GE_GEN_EMU_SNIFFER_END_ADDR_MSB_REG_ADDR,m,v,HWIO_GE_GEN_EMU_SNIFFER_END_ADDR_MSB_REG_IN)
#define HWIO_GE_GEN_EMU_SNIFFER_END_ADDR_MSB_REG_MSB_BMSK                          0xf
#define HWIO_GE_GEN_EMU_SNIFFER_END_ADDR_MSB_REG_MSB_SHFT                          0x0

#define HWIO_GE_GEN_EMU_SNIFFER_CTRL_REG_ADDR                               (GE_GEN_EMU_REGS_REG_BASE      + 0x00000080)
#define HWIO_GE_GEN_EMU_SNIFFER_CTRL_REG_PHYS                               (GE_GEN_EMU_REGS_REG_BASE_PHYS + 0x00000080)
#define HWIO_GE_GEN_EMU_SNIFFER_CTRL_REG_OFFS                               (GE_GEN_EMU_REGS_REG_BASE_OFFS + 0x00000080)
#define HWIO_GE_GEN_EMU_SNIFFER_CTRL_REG_RMSK                               0x7dfffff1
#define HWIO_GE_GEN_EMU_SNIFFER_CTRL_REG_POR                                0x00000800
#define HWIO_GE_GEN_EMU_SNIFFER_CTRL_REG_POR_RMSK                           0xffffffff
#define HWIO_GE_GEN_EMU_SNIFFER_CTRL_REG_ATTR                                      0x3
#define HWIO_GE_GEN_EMU_SNIFFER_CTRL_REG_IN          \
        in_dword_masked(HWIO_GE_GEN_EMU_SNIFFER_CTRL_REG_ADDR, HWIO_GE_GEN_EMU_SNIFFER_CTRL_REG_RMSK)
#define HWIO_GE_GEN_EMU_SNIFFER_CTRL_REG_INM(m)      \
        in_dword_masked(HWIO_GE_GEN_EMU_SNIFFER_CTRL_REG_ADDR, m)
#define HWIO_GE_GEN_EMU_SNIFFER_CTRL_REG_OUT(v)      \
        out_dword(HWIO_GE_GEN_EMU_SNIFFER_CTRL_REG_ADDR,v)
#define HWIO_GE_GEN_EMU_SNIFFER_CTRL_REG_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GE_GEN_EMU_SNIFFER_CTRL_REG_ADDR,m,v,HWIO_GE_GEN_EMU_SNIFFER_CTRL_REG_IN)
#define HWIO_GE_GEN_EMU_SNIFFER_CTRL_REG_PCIE_OP_MODE_BMSK                  0x7c000000
#define HWIO_GE_GEN_EMU_SNIFFER_CTRL_REG_PCIE_OP_MODE_SHFT                        0x1a
#define HWIO_GE_GEN_EMU_SNIFFER_CTRL_REG_CPU_OP_MODE_BMSK                    0x1f00000
#define HWIO_GE_GEN_EMU_SNIFFER_CTRL_REG_CPU_OP_MODE_SHFT                         0x14
#define HWIO_GE_GEN_EMU_SNIFFER_CTRL_REG_AGG0_OP_MODE_BMSK                     0xf0000
#define HWIO_GE_GEN_EMU_SNIFFER_CTRL_REG_AGG0_OP_MODE_SHFT                        0x10
#define HWIO_GE_GEN_EMU_SNIFFER_CTRL_REG_AGG1_OP_MODE_BMSK                      0xf000
#define HWIO_GE_GEN_EMU_SNIFFER_CTRL_REG_AGG1_OP_MODE_SHFT                         0xc
#define HWIO_GE_GEN_EMU_SNIFFER_CTRL_REG_MAX_TIMER_VAL_BMSK                      0xff0
#define HWIO_GE_GEN_EMU_SNIFFER_CTRL_REG_MAX_TIMER_VAL_SHFT                        0x4
#define HWIO_GE_GEN_EMU_SNIFFER_CTRL_REG_MEM_WRAP_BMSK                             0x1
#define HWIO_GE_GEN_EMU_SNIFFER_CTRL_REG_MEM_WRAP_SHFT                             0x0

#define HWIO_GE_GEN_EMU_SNIFFER_STATUS_REG_ADDR                             (GE_GEN_EMU_REGS_REG_BASE      + 0x00000084)
#define HWIO_GE_GEN_EMU_SNIFFER_STATUS_REG_PHYS                             (GE_GEN_EMU_REGS_REG_BASE_PHYS + 0x00000084)
#define HWIO_GE_GEN_EMU_SNIFFER_STATUS_REG_OFFS                             (GE_GEN_EMU_REGS_REG_BASE_OFFS + 0x00000084)
#define HWIO_GE_GEN_EMU_SNIFFER_STATUS_REG_RMSK                                    0xf
#define HWIO_GE_GEN_EMU_SNIFFER_STATUS_REG_POR                              0x00000000
#define HWIO_GE_GEN_EMU_SNIFFER_STATUS_REG_POR_RMSK                         0xffffffff
#define HWIO_GE_GEN_EMU_SNIFFER_STATUS_REG_ATTR                                    0x1
#define HWIO_GE_GEN_EMU_SNIFFER_STATUS_REG_IN          \
        in_dword_masked(HWIO_GE_GEN_EMU_SNIFFER_STATUS_REG_ADDR, HWIO_GE_GEN_EMU_SNIFFER_STATUS_REG_RMSK)
#define HWIO_GE_GEN_EMU_SNIFFER_STATUS_REG_INM(m)      \
        in_dword_masked(HWIO_GE_GEN_EMU_SNIFFER_STATUS_REG_ADDR, m)
#define HWIO_GE_GEN_EMU_SNIFFER_STATUS_REG_MEM_WR_ERROR_BMSK                       0x8
#define HWIO_GE_GEN_EMU_SNIFFER_STATUS_REG_MEM_WR_ERROR_SHFT                       0x3
#define HWIO_GE_GEN_EMU_SNIFFER_STATUS_REG_MEM_REACHED_END_BMSK                    0x4
#define HWIO_GE_GEN_EMU_SNIFFER_STATUS_REG_MEM_REACHED_END_SHFT                    0x2
#define HWIO_GE_GEN_EMU_SNIFFER_STATUS_REG_MEM_WRAPPED_BMSK                        0x2
#define HWIO_GE_GEN_EMU_SNIFFER_STATUS_REG_MEM_WRAPPED_SHFT                        0x1
#define HWIO_GE_GEN_EMU_SNIFFER_STATUS_REG_SNIF_FULL_BMSK                          0x1
#define HWIO_GE_GEN_EMU_SNIFFER_STATUS_REG_SNIF_FULL_SHFT                          0x0

#define HWIO_GE_GEN_EMU_SNIFFER_CURR_ADDR_LSB_REG_ADDR                      (GE_GEN_EMU_REGS_REG_BASE      + 0x00000088)
#define HWIO_GE_GEN_EMU_SNIFFER_CURR_ADDR_LSB_REG_PHYS                      (GE_GEN_EMU_REGS_REG_BASE_PHYS + 0x00000088)
#define HWIO_GE_GEN_EMU_SNIFFER_CURR_ADDR_LSB_REG_OFFS                      (GE_GEN_EMU_REGS_REG_BASE_OFFS + 0x00000088)
#define HWIO_GE_GEN_EMU_SNIFFER_CURR_ADDR_LSB_REG_RMSK                      0xffffffff
#define HWIO_GE_GEN_EMU_SNIFFER_CURR_ADDR_LSB_REG_POR                       0x00000000
#define HWIO_GE_GEN_EMU_SNIFFER_CURR_ADDR_LSB_REG_POR_RMSK                  0xffffffff
#define HWIO_GE_GEN_EMU_SNIFFER_CURR_ADDR_LSB_REG_ATTR                             0x1
#define HWIO_GE_GEN_EMU_SNIFFER_CURR_ADDR_LSB_REG_IN          \
        in_dword_masked(HWIO_GE_GEN_EMU_SNIFFER_CURR_ADDR_LSB_REG_ADDR, HWIO_GE_GEN_EMU_SNIFFER_CURR_ADDR_LSB_REG_RMSK)
#define HWIO_GE_GEN_EMU_SNIFFER_CURR_ADDR_LSB_REG_INM(m)      \
        in_dword_masked(HWIO_GE_GEN_EMU_SNIFFER_CURR_ADDR_LSB_REG_ADDR, m)
#define HWIO_GE_GEN_EMU_SNIFFER_CURR_ADDR_LSB_REG_LSB_BMSK                  0xffffffff
#define HWIO_GE_GEN_EMU_SNIFFER_CURR_ADDR_LSB_REG_LSB_SHFT                         0x0

#define HWIO_GE_GEN_EMU_SNIFFER_CURR_ADDR_MSB_REG_ADDR                      (GE_GEN_EMU_REGS_REG_BASE      + 0x0000008c)
#define HWIO_GE_GEN_EMU_SNIFFER_CURR_ADDR_MSB_REG_PHYS                      (GE_GEN_EMU_REGS_REG_BASE_PHYS + 0x0000008c)
#define HWIO_GE_GEN_EMU_SNIFFER_CURR_ADDR_MSB_REG_OFFS                      (GE_GEN_EMU_REGS_REG_BASE_OFFS + 0x0000008c)
#define HWIO_GE_GEN_EMU_SNIFFER_CURR_ADDR_MSB_REG_RMSK                             0xf
#define HWIO_GE_GEN_EMU_SNIFFER_CURR_ADDR_MSB_REG_POR                       0x00000000
#define HWIO_GE_GEN_EMU_SNIFFER_CURR_ADDR_MSB_REG_POR_RMSK                  0xffffffff
#define HWIO_GE_GEN_EMU_SNIFFER_CURR_ADDR_MSB_REG_ATTR                             0x1
#define HWIO_GE_GEN_EMU_SNIFFER_CURR_ADDR_MSB_REG_IN          \
        in_dword_masked(HWIO_GE_GEN_EMU_SNIFFER_CURR_ADDR_MSB_REG_ADDR, HWIO_GE_GEN_EMU_SNIFFER_CURR_ADDR_MSB_REG_RMSK)
#define HWIO_GE_GEN_EMU_SNIFFER_CURR_ADDR_MSB_REG_INM(m)      \
        in_dword_masked(HWIO_GE_GEN_EMU_SNIFFER_CURR_ADDR_MSB_REG_ADDR, m)
#define HWIO_GE_GEN_EMU_SNIFFER_CURR_ADDR_MSB_REG_MSB_BMSK                         0xf
#define HWIO_GE_GEN_EMU_SNIFFER_CURR_ADDR_MSB_REG_MSB_SHFT                         0x0

#define HWIO_GE_GEN_EMU_NODE_TIMING_REG_ADDR                                (GE_GEN_EMU_REGS_REG_BASE      + 0x00000090)
#define HWIO_GE_GEN_EMU_NODE_TIMING_REG_PHYS                                (GE_GEN_EMU_REGS_REG_BASE_PHYS + 0x00000090)
#define HWIO_GE_GEN_EMU_NODE_TIMING_REG_OFFS                                (GE_GEN_EMU_REGS_REG_BASE_OFFS + 0x00000090)
#define HWIO_GE_GEN_EMU_NODE_TIMING_REG_RMSK                                      0x9f
#define HWIO_GE_GEN_EMU_NODE_TIMING_REG_POR                                 0x00000090
#define HWIO_GE_GEN_EMU_NODE_TIMING_REG_POR_RMSK                            0xffffffff
#define HWIO_GE_GEN_EMU_NODE_TIMING_REG_ATTR                                       0x3
#define HWIO_GE_GEN_EMU_NODE_TIMING_REG_IN          \
        in_dword_masked(HWIO_GE_GEN_EMU_NODE_TIMING_REG_ADDR, HWIO_GE_GEN_EMU_NODE_TIMING_REG_RMSK)
#define HWIO_GE_GEN_EMU_NODE_TIMING_REG_INM(m)      \
        in_dword_masked(HWIO_GE_GEN_EMU_NODE_TIMING_REG_ADDR, m)
#define HWIO_GE_GEN_EMU_NODE_TIMING_REG_OUT(v)      \
        out_dword(HWIO_GE_GEN_EMU_NODE_TIMING_REG_ADDR,v)
#define HWIO_GE_GEN_EMU_NODE_TIMING_REG_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GE_GEN_EMU_NODE_TIMING_REG_ADDR,m,v,HWIO_GE_GEN_EMU_NODE_TIMING_REG_IN)
#define HWIO_GE_GEN_EMU_NODE_TIMING_REG_RX_FLIP_BMSK                              0x80
#define HWIO_GE_GEN_EMU_NODE_TIMING_REG_RX_FLIP_SHFT                               0x7
#define HWIO_GE_GEN_EMU_NODE_TIMING_REG_RX_DELAY_BMSK                             0x1f
#define HWIO_GE_GEN_EMU_NODE_TIMING_REG_RX_DELAY_SHFT                              0x0

/*----------------------------------------------------------------------------
 * MODULE: GE_GEN_EMU_ROM
 *--------------------------------------------------------------------------*/

#define GE_GEN_EMU_ROM_REG_BASE                              (GE_GEN_EMU_BASE      + 0x001e8000)
#define GE_GEN_EMU_ROM_REG_BASE_PHYS                         (GE_GEN_EMU_BASE_PHYS + 0x001e8000)
#define GE_GEN_EMU_ROM_REG_BASE_OFFS                         0x001e8000

#define HWIO_GE_GEN_EMU_ROM_STRING_SIZE_ADDR                 (GE_GEN_EMU_ROM_REG_BASE      + 0x00000004)
#define HWIO_GE_GEN_EMU_ROM_STRING_SIZE_PHYS                 (GE_GEN_EMU_ROM_REG_BASE_PHYS + 0x00000004)
#define HWIO_GE_GEN_EMU_ROM_STRING_SIZE_OFFS                 (GE_GEN_EMU_ROM_REG_BASE_OFFS + 0x00000004)
#define HWIO_GE_GEN_EMU_ROM_STRING_SIZE_RMSK                 0xffffffff
#define HWIO_GE_GEN_EMU_ROM_STRING_SIZE_POR                  0x00000000
#define HWIO_GE_GEN_EMU_ROM_STRING_SIZE_POR_RMSK             0xffffffff
#define HWIO_GE_GEN_EMU_ROM_STRING_SIZE_ATTR                        0x1
#define HWIO_GE_GEN_EMU_ROM_STRING_SIZE_IN          \
        in_dword_masked(HWIO_GE_GEN_EMU_ROM_STRING_SIZE_ADDR, HWIO_GE_GEN_EMU_ROM_STRING_SIZE_RMSK)
#define HWIO_GE_GEN_EMU_ROM_STRING_SIZE_INM(m)      \
        in_dword_masked(HWIO_GE_GEN_EMU_ROM_STRING_SIZE_ADDR, m)
#define HWIO_GE_GEN_EMU_ROM_STRING_SIZE_LENGTH_BMSK          0xffffffff
#define HWIO_GE_GEN_EMU_ROM_STRING_SIZE_LENGTH_SHFT                 0x0

#define HWIO_GE_GEN_EMU_ROM_STRING_START_ADDR                (GE_GEN_EMU_ROM_REG_BASE      + 0x00000008)
#define HWIO_GE_GEN_EMU_ROM_STRING_START_PHYS                (GE_GEN_EMU_ROM_REG_BASE_PHYS + 0x00000008)
#define HWIO_GE_GEN_EMU_ROM_STRING_START_OFFS                (GE_GEN_EMU_ROM_REG_BASE_OFFS + 0x00000008)
#define HWIO_GE_GEN_EMU_ROM_STRING_START_RMSK                0xffffffff
#define HWIO_GE_GEN_EMU_ROM_STRING_START_POR                 0x00000000
#define HWIO_GE_GEN_EMU_ROM_STRING_START_POR_RMSK            0xffffffff
#define HWIO_GE_GEN_EMU_ROM_STRING_START_ATTR                       0x1
#define HWIO_GE_GEN_EMU_ROM_STRING_START_IN          \
        in_dword_masked(HWIO_GE_GEN_EMU_ROM_STRING_START_ADDR, HWIO_GE_GEN_EMU_ROM_STRING_START_RMSK)
#define HWIO_GE_GEN_EMU_ROM_STRING_START_INM(m)      \
        in_dword_masked(HWIO_GE_GEN_EMU_ROM_STRING_START_ADDR, m)
#define HWIO_GE_GEN_EMU_ROM_STRING_START_START_BMSK          0xffffffff
#define HWIO_GE_GEN_EMU_ROM_STRING_START_START_SHFT                 0x0

/*----------------------------------------------------------------------------
 * MODULE: GE_NODE_TOP
 *--------------------------------------------------------------------------*/

#define GE_NODE_TOP_REG_BASE (GE_GEN_EMU_BASE      + 0x001f0000)
#define GE_NODE_TOP_REG_BASE_PHYS (GE_GEN_EMU_BASE_PHYS + 0x001f0000)
#define GE_NODE_TOP_REG_BASE_OFFS 0x001f0000

/*----------------------------------------------------------------------------
 * MODULE: GE_NODE_REGS
 *--------------------------------------------------------------------------*/

#define GE_NODE_REGS_REG_BASE                                             (GE_GEN_EMU_BASE      + 0x001f0000)
#define GE_NODE_REGS_REG_BASE_PHYS                                        (GE_GEN_EMU_BASE_PHYS + 0x001f0000)
#define GE_NODE_REGS_REG_BASE_OFFS                                        0x001f0000

#define HWIO_GE_NODE_VERSION_ADDR                                         (GE_NODE_REGS_REG_BASE      + 0x00000000)
#define HWIO_GE_NODE_VERSION_PHYS                                         (GE_NODE_REGS_REG_BASE_PHYS + 0x00000000)
#define HWIO_GE_NODE_VERSION_OFFS                                         (GE_NODE_REGS_REG_BASE_OFFS + 0x00000000)
#define HWIO_GE_NODE_VERSION_RMSK                                         0xffffffff
#define HWIO_GE_NODE_VERSION_POR                                          0x00000000
#define HWIO_GE_NODE_VERSION_POR_RMSK                                     0xffffffff
#define HWIO_GE_NODE_VERSION_ATTR                                                0x1
#define HWIO_GE_NODE_VERSION_IN          \
        in_dword_masked(HWIO_GE_NODE_VERSION_ADDR, HWIO_GE_NODE_VERSION_RMSK)
#define HWIO_GE_NODE_VERSION_INM(m)      \
        in_dword_masked(HWIO_GE_NODE_VERSION_ADDR, m)
#define HWIO_GE_NODE_VERSION_SETUP_BMSK                                   0xff000000
#define HWIO_GE_NODE_VERSION_SETUP_SHFT                                         0x18
#define HWIO_GE_NODE_VERSION_MAJOR_BMSK                                     0xff0000
#define HWIO_GE_NODE_VERSION_MAJOR_SHFT                                         0x10
#define HWIO_GE_NODE_VERSION_MINOR_BMSK                                       0xfff0
#define HWIO_GE_NODE_VERSION_MINOR_SHFT                                          0x4
#define HWIO_GE_NODE_VERSION_STEP_BMSK                                           0xf
#define HWIO_GE_NODE_VERSION_STEP_SHFT                                           0x0

#define HWIO_GE_NODE_RESET_REG_ADDR                                       (GE_NODE_REGS_REG_BASE      + 0x00000004)
#define HWIO_GE_NODE_RESET_REG_PHYS                                       (GE_NODE_REGS_REG_BASE_PHYS + 0x00000004)
#define HWIO_GE_NODE_RESET_REG_OFFS                                       (GE_NODE_REGS_REG_BASE_OFFS + 0x00000004)
#define HWIO_GE_NODE_RESET_REG_RMSK                                              0x1
#define HWIO_GE_NODE_RESET_REG_POR                                        0x00000001
#define HWIO_GE_NODE_RESET_REG_POR_RMSK                                   0xffffffff
#define HWIO_GE_NODE_RESET_REG_ATTR                                              0x3
#define HWIO_GE_NODE_RESET_REG_IN          \
        in_dword_masked(HWIO_GE_NODE_RESET_REG_ADDR, HWIO_GE_NODE_RESET_REG_RMSK)
#define HWIO_GE_NODE_RESET_REG_INM(m)      \
        in_dword_masked(HWIO_GE_NODE_RESET_REG_ADDR, m)
#define HWIO_GE_NODE_RESET_REG_OUT(v)      \
        out_dword(HWIO_GE_NODE_RESET_REG_ADDR,v)
#define HWIO_GE_NODE_RESET_REG_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GE_NODE_RESET_REG_ADDR,m,v,HWIO_GE_NODE_RESET_REG_IN)
#define HWIO_GE_NODE_RESET_REG_DBD_RESET_BMSK                                    0x1
#define HWIO_GE_NODE_RESET_REG_DBD_RESET_SHFT                                    0x0

#define HWIO_GE_NODE_CLK_STATUS_REG_ADDR                                  (GE_NODE_REGS_REG_BASE      + 0x00000010)
#define HWIO_GE_NODE_CLK_STATUS_REG_PHYS                                  (GE_NODE_REGS_REG_BASE_PHYS + 0x00000010)
#define HWIO_GE_NODE_CLK_STATUS_REG_OFFS                                  (GE_NODE_REGS_REG_BASE_OFFS + 0x00000010)
#define HWIO_GE_NODE_CLK_STATUS_REG_RMSK                                     0xfffff
#define HWIO_GE_NODE_CLK_STATUS_REG_POR                                   0x00000000
#define HWIO_GE_NODE_CLK_STATUS_REG_POR_RMSK                              0xffffffff
#define HWIO_GE_NODE_CLK_STATUS_REG_ATTR                                         0x1
#define HWIO_GE_NODE_CLK_STATUS_REG_IN          \
        in_dword_masked(HWIO_GE_NODE_CLK_STATUS_REG_ADDR, HWIO_GE_NODE_CLK_STATUS_REG_RMSK)
#define HWIO_GE_NODE_CLK_STATUS_REG_INM(m)      \
        in_dword_masked(HWIO_GE_NODE_CLK_STATUS_REG_ADDR, m)
#define HWIO_GE_NODE_CLK_STATUS_REG_FREQUENCY_IN_KHZ_BMSK                    0xfffff
#define HWIO_GE_NODE_CLK_STATUS_REG_FREQUENCY_IN_KHZ_SHFT                        0x0

#define HWIO_GE_NODE_CLK_STATUS_SEL_REG_ADDR                              (GE_NODE_REGS_REG_BASE      + 0x00000014)
#define HWIO_GE_NODE_CLK_STATUS_SEL_REG_PHYS                              (GE_NODE_REGS_REG_BASE_PHYS + 0x00000014)
#define HWIO_GE_NODE_CLK_STATUS_SEL_REG_OFFS                              (GE_NODE_REGS_REG_BASE_OFFS + 0x00000014)
#define HWIO_GE_NODE_CLK_STATUS_SEL_REG_RMSK                                     0xf
#define HWIO_GE_NODE_CLK_STATUS_SEL_REG_POR                               0x00000000
#define HWIO_GE_NODE_CLK_STATUS_SEL_REG_POR_RMSK                          0xffffffff
#define HWIO_GE_NODE_CLK_STATUS_SEL_REG_ATTR                                     0x3
#define HWIO_GE_NODE_CLK_STATUS_SEL_REG_IN          \
        in_dword_masked(HWIO_GE_NODE_CLK_STATUS_SEL_REG_ADDR, HWIO_GE_NODE_CLK_STATUS_SEL_REG_RMSK)
#define HWIO_GE_NODE_CLK_STATUS_SEL_REG_INM(m)      \
        in_dword_masked(HWIO_GE_NODE_CLK_STATUS_SEL_REG_ADDR, m)
#define HWIO_GE_NODE_CLK_STATUS_SEL_REG_OUT(v)      \
        out_dword(HWIO_GE_NODE_CLK_STATUS_SEL_REG_ADDR,v)
#define HWIO_GE_NODE_CLK_STATUS_SEL_REG_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GE_NODE_CLK_STATUS_SEL_REG_ADDR,m,v,HWIO_GE_NODE_CLK_STATUS_SEL_REG_IN)
#define HWIO_GE_NODE_CLK_STATUS_SEL_REG_CLK_NUM_BMSK                             0xf
#define HWIO_GE_NODE_CLK_STATUS_SEL_REG_CLK_NUM_SHFT                             0x0

#define HWIO_GE_NODE_SE_PLL_CTRL_REG_ADDR                                 (GE_NODE_REGS_REG_BASE      + 0x00000018)
#define HWIO_GE_NODE_SE_PLL_CTRL_REG_PHYS                                 (GE_NODE_REGS_REG_BASE_PHYS + 0x00000018)
#define HWIO_GE_NODE_SE_PLL_CTRL_REG_OFFS                                 (GE_NODE_REGS_REG_BASE_OFFS + 0x00000018)
#define HWIO_GE_NODE_SE_PLL_CTRL_REG_RMSK                                 0x33ffffff
#define HWIO_GE_NODE_SE_PLL_CTRL_REG_POR                                  0x00000000
#define HWIO_GE_NODE_SE_PLL_CTRL_REG_POR_RMSK                             0xffffffff
#define HWIO_GE_NODE_SE_PLL_CTRL_REG_ATTR                                        0x3
#define HWIO_GE_NODE_SE_PLL_CTRL_REG_IN          \
        in_dword_masked(HWIO_GE_NODE_SE_PLL_CTRL_REG_ADDR, HWIO_GE_NODE_SE_PLL_CTRL_REG_RMSK)
#define HWIO_GE_NODE_SE_PLL_CTRL_REG_INM(m)      \
        in_dword_masked(HWIO_GE_NODE_SE_PLL_CTRL_REG_ADDR, m)
#define HWIO_GE_NODE_SE_PLL_CTRL_REG_OUT(v)      \
        out_dword(HWIO_GE_NODE_SE_PLL_CTRL_REG_ADDR,v)
#define HWIO_GE_NODE_SE_PLL_CTRL_REG_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GE_NODE_SE_PLL_CTRL_REG_ADDR,m,v,HWIO_GE_NODE_SE_PLL_CTRL_REG_IN)
#define HWIO_GE_NODE_SE_PLL_CTRL_REG_DRIVE_GEN_EMU_BMSK                   0x20000000
#define HWIO_GE_NODE_SE_PLL_CTRL_REG_DRIVE_GEN_EMU_SHFT                         0x1d
#define HWIO_GE_NODE_SE_PLL_CTRL_REG_EXECUTE_BMSK                         0x10000000
#define HWIO_GE_NODE_SE_PLL_CTRL_REG_EXECUTE_SHFT                               0x1c
#define HWIO_GE_NODE_SE_PLL_CTRL_REG_PLL_SELECT_BMSK                       0x3000000
#define HWIO_GE_NODE_SE_PLL_CTRL_REG_PLL_SELECT_SHFT                            0x18
#define HWIO_GE_NODE_SE_PLL_CTRL_REG_INT_LOAD_CAP_BMSK                      0xc00000
#define HWIO_GE_NODE_SE_PLL_CTRL_REG_INT_LOAD_CAP_SHFT                          0x16
#define HWIO_GE_NODE_SE_PLL_CTRL_REG_TTL_DUTY_CYCLE_BMSK                    0x200000
#define HWIO_GE_NODE_SE_PLL_CTRL_REG_TTL_DUTY_CYCLE_SHFT                        0x15
#define HWIO_GE_NODE_SE_PLL_CTRL_REG_CLK2_FUNC_BMSK                         0x180000
#define HWIO_GE_NODE_SE_PLL_CTRL_REG_CLK2_FUNC_SHFT                             0x13
#define HWIO_GE_NODE_SE_PLL_CTRL_REG_OUT_DIV_SEL_BMSK                        0x70000
#define HWIO_GE_NODE_SE_PLL_CTRL_REG_OUT_DIV_SEL_SHFT                           0x10
#define HWIO_GE_NODE_SE_PLL_CTRL_REG_VCO_DIV_WORD_BMSK                        0xff80
#define HWIO_GE_NODE_SE_PLL_CTRL_REG_VCO_DIV_WORD_SHFT                           0x7
#define HWIO_GE_NODE_SE_PLL_CTRL_REG_REF_DIV_WORD_BMSK                          0x7f
#define HWIO_GE_NODE_SE_PLL_CTRL_REG_REF_DIV_WORD_SHFT                           0x0

#define HWIO_GE_NODE_HS_PLL_CTRL_REG_ADDR                                 (GE_NODE_REGS_REG_BASE      + 0x0000001c)
#define HWIO_GE_NODE_HS_PLL_CTRL_REG_PHYS                                 (GE_NODE_REGS_REG_BASE_PHYS + 0x0000001c)
#define HWIO_GE_NODE_HS_PLL_CTRL_REG_OFFS                                 (GE_NODE_REGS_REG_BASE_OFFS + 0x0000001c)
#define HWIO_GE_NODE_HS_PLL_CTRL_REG_RMSK                                  0x11137ff
#define HWIO_GE_NODE_HS_PLL_CTRL_REG_POR                                  0x00000000
#define HWIO_GE_NODE_HS_PLL_CTRL_REG_POR_RMSK                             0xffffffff
#define HWIO_GE_NODE_HS_PLL_CTRL_REG_ATTR                                        0x3
#define HWIO_GE_NODE_HS_PLL_CTRL_REG_IN          \
        in_dword_masked(HWIO_GE_NODE_HS_PLL_CTRL_REG_ADDR, HWIO_GE_NODE_HS_PLL_CTRL_REG_RMSK)
#define HWIO_GE_NODE_HS_PLL_CTRL_REG_INM(m)      \
        in_dword_masked(HWIO_GE_NODE_HS_PLL_CTRL_REG_ADDR, m)
#define HWIO_GE_NODE_HS_PLL_CTRL_REG_OUT(v)      \
        out_dword(HWIO_GE_NODE_HS_PLL_CTRL_REG_ADDR,v)
#define HWIO_GE_NODE_HS_PLL_CTRL_REG_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GE_NODE_HS_PLL_CTRL_REG_ADDR,m,v,HWIO_GE_NODE_HS_PLL_CTRL_REG_IN)
#define HWIO_GE_NODE_HS_PLL_CTRL_REG_VCO_SEL_N_BMSK                        0x1000000
#define HWIO_GE_NODE_HS_PLL_CTRL_REG_VCO_SEL_N_SHFT                             0x18
#define HWIO_GE_NODE_HS_PLL_CTRL_REG_EXECUTE_BMSK                           0x100000
#define HWIO_GE_NODE_HS_PLL_CTRL_REG_EXECUTE_SHFT                               0x14
#define HWIO_GE_NODE_HS_PLL_CTRL_REG_PLL_SELECT_BMSK                         0x10000
#define HWIO_GE_NODE_HS_PLL_CTRL_REG_PLL_SELECT_SHFT                            0x10
#define HWIO_GE_NODE_HS_PLL_CTRL_REG_TEST_OUT_SEL_BMSK                        0x3000
#define HWIO_GE_NODE_HS_PLL_CTRL_REG_TEST_OUT_SEL_SHFT                           0xc
#define HWIO_GE_NODE_HS_PLL_CTRL_REG_OUT_DIV_N_BMSK                            0x600
#define HWIO_GE_NODE_HS_PLL_CTRL_REG_OUT_DIV_N_SHFT                              0x9
#define HWIO_GE_NODE_HS_PLL_CTRL_REG_VCO_DIV_M_BMSK                            0x1ff
#define HWIO_GE_NODE_HS_PLL_CTRL_REG_VCO_DIV_M_SHFT                              0x0

#define HWIO_GE_NODE_PCIE_CTL_STS_REG_ADDR                                (GE_NODE_REGS_REG_BASE      + 0x00000044)
#define HWIO_GE_NODE_PCIE_CTL_STS_REG_PHYS                                (GE_NODE_REGS_REG_BASE_PHYS + 0x00000044)
#define HWIO_GE_NODE_PCIE_CTL_STS_REG_OFFS                                (GE_NODE_REGS_REG_BASE_OFFS + 0x00000044)
#define HWIO_GE_NODE_PCIE_CTL_STS_REG_RMSK                                      0x77
#define HWIO_GE_NODE_PCIE_CTL_STS_REG_POR                                 0x00000000
#define HWIO_GE_NODE_PCIE_CTL_STS_REG_POR_RMSK                            0xffffffff
#define HWIO_GE_NODE_PCIE_CTL_STS_REG_ATTR                                       0x3
#define HWIO_GE_NODE_PCIE_CTL_STS_REG_IN          \
        in_dword_masked(HWIO_GE_NODE_PCIE_CTL_STS_REG_ADDR, HWIO_GE_NODE_PCIE_CTL_STS_REG_RMSK)
#define HWIO_GE_NODE_PCIE_CTL_STS_REG_INM(m)      \
        in_dword_masked(HWIO_GE_NODE_PCIE_CTL_STS_REG_ADDR, m)
#define HWIO_GE_NODE_PCIE_CTL_STS_REG_OUT(v)      \
        out_dword(HWIO_GE_NODE_PCIE_CTL_STS_REG_ADDR,v)
#define HWIO_GE_NODE_PCIE_CTL_STS_REG_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GE_NODE_PCIE_CTL_STS_REG_ADDR,m,v,HWIO_GE_NODE_PCIE_CTL_STS_REG_IN)
#define HWIO_GE_NODE_PCIE_CTL_STS_REG_WAKE_N_OE_BMSK                            0x40
#define HWIO_GE_NODE_PCIE_CTL_STS_REG_WAKE_N_OE_SHFT                             0x6
#define HWIO_GE_NODE_PCIE_CTL_STS_REG_WAKE_N_OUT_BMSK                           0x20
#define HWIO_GE_NODE_PCIE_CTL_STS_REG_WAKE_N_OUT_SHFT                            0x5
#define HWIO_GE_NODE_PCIE_CTL_STS_REG_WAKE_N_IN_BMSK                            0x10
#define HWIO_GE_NODE_PCIE_CTL_STS_REG_WAKE_N_IN_SHFT                             0x4
#define HWIO_GE_NODE_PCIE_CTL_STS_REG_PERST_N_OE_BMSK                            0x4
#define HWIO_GE_NODE_PCIE_CTL_STS_REG_PERST_N_OE_SHFT                            0x2
#define HWIO_GE_NODE_PCIE_CTL_STS_REG_PERST_N_OUT_BMSK                           0x2
#define HWIO_GE_NODE_PCIE_CTL_STS_REG_PERST_N_OUT_SHFT                           0x1
#define HWIO_GE_NODE_PCIE_CTL_STS_REG_PERST_N_IN_BMSK                            0x1
#define HWIO_GE_NODE_PCIE_CTL_STS_REG_PERST_N_IN_SHFT                            0x0

#define HWIO_GE_NODE_DBD_START_ADDR_LSB_REG_ADDR                          (GE_NODE_REGS_REG_BASE      + 0x00000050)
#define HWIO_GE_NODE_DBD_START_ADDR_LSB_REG_PHYS                          (GE_NODE_REGS_REG_BASE_PHYS + 0x00000050)
#define HWIO_GE_NODE_DBD_START_ADDR_LSB_REG_OFFS                          (GE_NODE_REGS_REG_BASE_OFFS + 0x00000050)
#define HWIO_GE_NODE_DBD_START_ADDR_LSB_REG_RMSK                          0xffffffff
#define HWIO_GE_NODE_DBD_START_ADDR_LSB_REG_POR                           0x00000000
#define HWIO_GE_NODE_DBD_START_ADDR_LSB_REG_POR_RMSK                      0xffffffff
#define HWIO_GE_NODE_DBD_START_ADDR_LSB_REG_ATTR                                 0x3
#define HWIO_GE_NODE_DBD_START_ADDR_LSB_REG_IN          \
        in_dword_masked(HWIO_GE_NODE_DBD_START_ADDR_LSB_REG_ADDR, HWIO_GE_NODE_DBD_START_ADDR_LSB_REG_RMSK)
#define HWIO_GE_NODE_DBD_START_ADDR_LSB_REG_INM(m)      \
        in_dword_masked(HWIO_GE_NODE_DBD_START_ADDR_LSB_REG_ADDR, m)
#define HWIO_GE_NODE_DBD_START_ADDR_LSB_REG_OUT(v)      \
        out_dword(HWIO_GE_NODE_DBD_START_ADDR_LSB_REG_ADDR,v)
#define HWIO_GE_NODE_DBD_START_ADDR_LSB_REG_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GE_NODE_DBD_START_ADDR_LSB_REG_ADDR,m,v,HWIO_GE_NODE_DBD_START_ADDR_LSB_REG_IN)
#define HWIO_GE_NODE_DBD_START_ADDR_LSB_REG_LSB_BMSK                      0xffffffff
#define HWIO_GE_NODE_DBD_START_ADDR_LSB_REG_LSB_SHFT                             0x0

#define HWIO_GE_NODE_DBD_START_ADDR_MSB_REG_ADDR                          (GE_NODE_REGS_REG_BASE      + 0x00000054)
#define HWIO_GE_NODE_DBD_START_ADDR_MSB_REG_PHYS                          (GE_NODE_REGS_REG_BASE_PHYS + 0x00000054)
#define HWIO_GE_NODE_DBD_START_ADDR_MSB_REG_OFFS                          (GE_NODE_REGS_REG_BASE_OFFS + 0x00000054)
#define HWIO_GE_NODE_DBD_START_ADDR_MSB_REG_RMSK                                 0xf
#define HWIO_GE_NODE_DBD_START_ADDR_MSB_REG_POR                           0x00000000
#define HWIO_GE_NODE_DBD_START_ADDR_MSB_REG_POR_RMSK                      0xffffffff
#define HWIO_GE_NODE_DBD_START_ADDR_MSB_REG_ATTR                                 0x3
#define HWIO_GE_NODE_DBD_START_ADDR_MSB_REG_IN          \
        in_dword_masked(HWIO_GE_NODE_DBD_START_ADDR_MSB_REG_ADDR, HWIO_GE_NODE_DBD_START_ADDR_MSB_REG_RMSK)
#define HWIO_GE_NODE_DBD_START_ADDR_MSB_REG_INM(m)      \
        in_dword_masked(HWIO_GE_NODE_DBD_START_ADDR_MSB_REG_ADDR, m)
#define HWIO_GE_NODE_DBD_START_ADDR_MSB_REG_OUT(v)      \
        out_dword(HWIO_GE_NODE_DBD_START_ADDR_MSB_REG_ADDR,v)
#define HWIO_GE_NODE_DBD_START_ADDR_MSB_REG_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GE_NODE_DBD_START_ADDR_MSB_REG_ADDR,m,v,HWIO_GE_NODE_DBD_START_ADDR_MSB_REG_IN)
#define HWIO_GE_NODE_DBD_START_ADDR_MSB_REG_MSB_BMSK                             0xf
#define HWIO_GE_NODE_DBD_START_ADDR_MSB_REG_MSB_SHFT                             0x0

#define HWIO_GE_NODE_DBD_END_ADDR_LSB_REG_ADDR                            (GE_NODE_REGS_REG_BASE      + 0x00000058)
#define HWIO_GE_NODE_DBD_END_ADDR_LSB_REG_PHYS                            (GE_NODE_REGS_REG_BASE_PHYS + 0x00000058)
#define HWIO_GE_NODE_DBD_END_ADDR_LSB_REG_OFFS                            (GE_NODE_REGS_REG_BASE_OFFS + 0x00000058)
#define HWIO_GE_NODE_DBD_END_ADDR_LSB_REG_RMSK                            0xffffffff
#define HWIO_GE_NODE_DBD_END_ADDR_LSB_REG_POR                             0x00000000
#define HWIO_GE_NODE_DBD_END_ADDR_LSB_REG_POR_RMSK                        0xffffffff
#define HWIO_GE_NODE_DBD_END_ADDR_LSB_REG_ATTR                                   0x3
#define HWIO_GE_NODE_DBD_END_ADDR_LSB_REG_IN          \
        in_dword_masked(HWIO_GE_NODE_DBD_END_ADDR_LSB_REG_ADDR, HWIO_GE_NODE_DBD_END_ADDR_LSB_REG_RMSK)
#define HWIO_GE_NODE_DBD_END_ADDR_LSB_REG_INM(m)      \
        in_dword_masked(HWIO_GE_NODE_DBD_END_ADDR_LSB_REG_ADDR, m)
#define HWIO_GE_NODE_DBD_END_ADDR_LSB_REG_OUT(v)      \
        out_dword(HWIO_GE_NODE_DBD_END_ADDR_LSB_REG_ADDR,v)
#define HWIO_GE_NODE_DBD_END_ADDR_LSB_REG_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GE_NODE_DBD_END_ADDR_LSB_REG_ADDR,m,v,HWIO_GE_NODE_DBD_END_ADDR_LSB_REG_IN)
#define HWIO_GE_NODE_DBD_END_ADDR_LSB_REG_LSB_BMSK                        0xffffffff
#define HWIO_GE_NODE_DBD_END_ADDR_LSB_REG_LSB_SHFT                               0x0

#define HWIO_GE_NODE_DBD_END_ADDR_MSB_REG_ADDR                            (GE_NODE_REGS_REG_BASE      + 0x0000005c)
#define HWIO_GE_NODE_DBD_END_ADDR_MSB_REG_PHYS                            (GE_NODE_REGS_REG_BASE_PHYS + 0x0000005c)
#define HWIO_GE_NODE_DBD_END_ADDR_MSB_REG_OFFS                            (GE_NODE_REGS_REG_BASE_OFFS + 0x0000005c)
#define HWIO_GE_NODE_DBD_END_ADDR_MSB_REG_RMSK                                   0xf
#define HWIO_GE_NODE_DBD_END_ADDR_MSB_REG_POR                             0x00000000
#define HWIO_GE_NODE_DBD_END_ADDR_MSB_REG_POR_RMSK                        0xffffffff
#define HWIO_GE_NODE_DBD_END_ADDR_MSB_REG_ATTR                                   0x3
#define HWIO_GE_NODE_DBD_END_ADDR_MSB_REG_IN          \
        in_dword_masked(HWIO_GE_NODE_DBD_END_ADDR_MSB_REG_ADDR, HWIO_GE_NODE_DBD_END_ADDR_MSB_REG_RMSK)
#define HWIO_GE_NODE_DBD_END_ADDR_MSB_REG_INM(m)      \
        in_dword_masked(HWIO_GE_NODE_DBD_END_ADDR_MSB_REG_ADDR, m)
#define HWIO_GE_NODE_DBD_END_ADDR_MSB_REG_OUT(v)      \
        out_dword(HWIO_GE_NODE_DBD_END_ADDR_MSB_REG_ADDR,v)
#define HWIO_GE_NODE_DBD_END_ADDR_MSB_REG_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GE_NODE_DBD_END_ADDR_MSB_REG_ADDR,m,v,HWIO_GE_NODE_DBD_END_ADDR_MSB_REG_IN)
#define HWIO_GE_NODE_DBD_END_ADDR_MSB_REG_MSB_BMSK                               0xf
#define HWIO_GE_NODE_DBD_END_ADDR_MSB_REG_MSB_SHFT                               0x0

#define HWIO_GE_NODE_DBD_CTRL_REG_ADDR                                    (GE_NODE_REGS_REG_BASE      + 0x00000060)
#define HWIO_GE_NODE_DBD_CTRL_REG_PHYS                                    (GE_NODE_REGS_REG_BASE_PHYS + 0x00000060)
#define HWIO_GE_NODE_DBD_CTRL_REG_OFFS                                    (GE_NODE_REGS_REG_BASE_OFFS + 0x00000060)
#define HWIO_GE_NODE_DBD_CTRL_REG_RMSK                                    0xf111ffff
#define HWIO_GE_NODE_DBD_CTRL_REG_POR                                     0x0000ff80
#define HWIO_GE_NODE_DBD_CTRL_REG_POR_RMSK                                0xffffffff
#define HWIO_GE_NODE_DBD_CTRL_REG_ATTR                                           0x3
#define HWIO_GE_NODE_DBD_CTRL_REG_IN          \
        in_dword_masked(HWIO_GE_NODE_DBD_CTRL_REG_ADDR, HWIO_GE_NODE_DBD_CTRL_REG_RMSK)
#define HWIO_GE_NODE_DBD_CTRL_REG_INM(m)      \
        in_dword_masked(HWIO_GE_NODE_DBD_CTRL_REG_ADDR, m)
#define HWIO_GE_NODE_DBD_CTRL_REG_OUT(v)      \
        out_dword(HWIO_GE_NODE_DBD_CTRL_REG_ADDR,v)
#define HWIO_GE_NODE_DBD_CTRL_REG_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GE_NODE_DBD_CTRL_REG_ADDR,m,v,HWIO_GE_NODE_DBD_CTRL_REG_IN)
#define HWIO_GE_NODE_DBD_CTRL_REG_DEBUG_CLK_SEL_BMSK                      0x80000000
#define HWIO_GE_NODE_DBD_CTRL_REG_DEBUG_CLK_SEL_SHFT                            0x1f
#define HWIO_GE_NODE_DBD_CTRL_REG_FPGA_SEL_BMSK                           0x70000000
#define HWIO_GE_NODE_DBD_CTRL_REG_FPGA_SEL_SHFT                                 0x1c
#define HWIO_GE_NODE_DBD_CTRL_REG_EDGE_SEL_BMSK                            0x1000000
#define HWIO_GE_NODE_DBD_CTRL_REG_EDGE_SEL_SHFT                                 0x18
#define HWIO_GE_NODE_DBD_CTRL_REG_TRIG_MODE_BMSK                            0x100000
#define HWIO_GE_NODE_DBD_CTRL_REG_TRIG_MODE_SHFT                                0x14
#define HWIO_GE_NODE_DBD_CTRL_REG_DUMPER_STOP_BMSK                           0x10000
#define HWIO_GE_NODE_DBD_CTRL_REG_DUMPER_STOP_SHFT                              0x10
#define HWIO_GE_NODE_DBD_CTRL_REG_POST_TRIG_PERC_BMSK                         0xff00
#define HWIO_GE_NODE_DBD_CTRL_REG_POST_TRIG_PERC_SHFT                            0x8
#define HWIO_GE_NODE_DBD_CTRL_REG_MAX_TIMER_VAL_BMSK                            0xff
#define HWIO_GE_NODE_DBD_CTRL_REG_MAX_TIMER_VAL_SHFT                             0x0

#define HWIO_GE_NODE_DBD_STATUS_REG_ADDR                                  (GE_NODE_REGS_REG_BASE      + 0x00000064)
#define HWIO_GE_NODE_DBD_STATUS_REG_PHYS                                  (GE_NODE_REGS_REG_BASE_PHYS + 0x00000064)
#define HWIO_GE_NODE_DBD_STATUS_REG_OFFS                                  (GE_NODE_REGS_REG_BASE_OFFS + 0x00000064)
#define HWIO_GE_NODE_DBD_STATUS_REG_RMSK                                  0xf9111111
#define HWIO_GE_NODE_DBD_STATUS_REG_POR                                   0x00000000
#define HWIO_GE_NODE_DBD_STATUS_REG_POR_RMSK                              0xffffffff
#define HWIO_GE_NODE_DBD_STATUS_REG_ATTR                                         0x1
#define HWIO_GE_NODE_DBD_STATUS_REG_IN          \
        in_dword_masked(HWIO_GE_NODE_DBD_STATUS_REG_ADDR, HWIO_GE_NODE_DBD_STATUS_REG_RMSK)
#define HWIO_GE_NODE_DBD_STATUS_REG_INM(m)      \
        in_dword_masked(HWIO_GE_NODE_DBD_STATUS_REG_ADDR, m)
#define HWIO_GE_NODE_DBD_STATUS_REG_OUTSTND_CNTR_BMSK                     0xf8000000
#define HWIO_GE_NODE_DBD_STATUS_REG_OUTSTND_CNTR_SHFT                           0x1b
#define HWIO_GE_NODE_DBD_STATUS_REG_TRIG_FIRED_BMSK                        0x1000000
#define HWIO_GE_NODE_DBD_STATUS_REG_TRIG_FIRED_SHFT                             0x18
#define HWIO_GE_NODE_DBD_STATUS_REG_TRIG_DONE_BMSK                          0x100000
#define HWIO_GE_NODE_DBD_STATUS_REG_TRIG_DONE_SHFT                              0x14
#define HWIO_GE_NODE_DBD_STATUS_REG_TRIG_ODD_BMSK                            0x10000
#define HWIO_GE_NODE_DBD_STATUS_REG_TRIG_ODD_SHFT                               0x10
#define HWIO_GE_NODE_DBD_STATUS_REG_MEM_WR_ERROR_BMSK                         0x1000
#define HWIO_GE_NODE_DBD_STATUS_REG_MEM_WR_ERROR_SHFT                            0xc
#define HWIO_GE_NODE_DBD_STATUS_REG_MEM_WRAPPED_BMSK                           0x100
#define HWIO_GE_NODE_DBD_STATUS_REG_MEM_WRAPPED_SHFT                             0x8
#define HWIO_GE_NODE_DBD_STATUS_REG_DUMPER_FULL_BMSK                            0x10
#define HWIO_GE_NODE_DBD_STATUS_REG_DUMPER_FULL_SHFT                             0x4
#define HWIO_GE_NODE_DBD_STATUS_REG_DUMPER_IDLE_BMSK                             0x1
#define HWIO_GE_NODE_DBD_STATUS_REG_DUMPER_IDLE_SHFT                             0x0

#define HWIO_GE_NODE_DBD_TRIG_MASK_REG_ADDR                               (GE_NODE_REGS_REG_BASE      + 0x00000068)
#define HWIO_GE_NODE_DBD_TRIG_MASK_REG_PHYS                               (GE_NODE_REGS_REG_BASE_PHYS + 0x00000068)
#define HWIO_GE_NODE_DBD_TRIG_MASK_REG_OFFS                               (GE_NODE_REGS_REG_BASE_OFFS + 0x00000068)
#define HWIO_GE_NODE_DBD_TRIG_MASK_REG_RMSK                               0xffffffff
#define HWIO_GE_NODE_DBD_TRIG_MASK_REG_POR                                0x00000000
#define HWIO_GE_NODE_DBD_TRIG_MASK_REG_POR_RMSK                           0xffffffff
#define HWIO_GE_NODE_DBD_TRIG_MASK_REG_ATTR                                      0x3
#define HWIO_GE_NODE_DBD_TRIG_MASK_REG_IN          \
        in_dword_masked(HWIO_GE_NODE_DBD_TRIG_MASK_REG_ADDR, HWIO_GE_NODE_DBD_TRIG_MASK_REG_RMSK)
#define HWIO_GE_NODE_DBD_TRIG_MASK_REG_INM(m)      \
        in_dword_masked(HWIO_GE_NODE_DBD_TRIG_MASK_REG_ADDR, m)
#define HWIO_GE_NODE_DBD_TRIG_MASK_REG_OUT(v)      \
        out_dword(HWIO_GE_NODE_DBD_TRIG_MASK_REG_ADDR,v)
#define HWIO_GE_NODE_DBD_TRIG_MASK_REG_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GE_NODE_DBD_TRIG_MASK_REG_ADDR,m,v,HWIO_GE_NODE_DBD_TRIG_MASK_REG_IN)
#define HWIO_GE_NODE_DBD_TRIG_MASK_REG_TRIG_MASK_BMSK                     0xffffffff
#define HWIO_GE_NODE_DBD_TRIG_MASK_REG_TRIG_MASK_SHFT                            0x0

#define HWIO_GE_NODE_DBD_TRIG_VAL_REG_ADDR                                (GE_NODE_REGS_REG_BASE      + 0x0000006c)
#define HWIO_GE_NODE_DBD_TRIG_VAL_REG_PHYS                                (GE_NODE_REGS_REG_BASE_PHYS + 0x0000006c)
#define HWIO_GE_NODE_DBD_TRIG_VAL_REG_OFFS                                (GE_NODE_REGS_REG_BASE_OFFS + 0x0000006c)
#define HWIO_GE_NODE_DBD_TRIG_VAL_REG_RMSK                                0xffffffff
#define HWIO_GE_NODE_DBD_TRIG_VAL_REG_POR                                 0x00000000
#define HWIO_GE_NODE_DBD_TRIG_VAL_REG_POR_RMSK                            0xffffffff
#define HWIO_GE_NODE_DBD_TRIG_VAL_REG_ATTR                                       0x3
#define HWIO_GE_NODE_DBD_TRIG_VAL_REG_IN          \
        in_dword_masked(HWIO_GE_NODE_DBD_TRIG_VAL_REG_ADDR, HWIO_GE_NODE_DBD_TRIG_VAL_REG_RMSK)
#define HWIO_GE_NODE_DBD_TRIG_VAL_REG_INM(m)      \
        in_dword_masked(HWIO_GE_NODE_DBD_TRIG_VAL_REG_ADDR, m)
#define HWIO_GE_NODE_DBD_TRIG_VAL_REG_OUT(v)      \
        out_dword(HWIO_GE_NODE_DBD_TRIG_VAL_REG_ADDR,v)
#define HWIO_GE_NODE_DBD_TRIG_VAL_REG_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GE_NODE_DBD_TRIG_VAL_REG_ADDR,m,v,HWIO_GE_NODE_DBD_TRIG_VAL_REG_IN)
#define HWIO_GE_NODE_DBD_TRIG_VAL_REG_TRIG_VAL_BMSK                       0xffffffff
#define HWIO_GE_NODE_DBD_TRIG_VAL_REG_TRIG_VAL_SHFT                              0x0

#define HWIO_GE_NODE_DBD_TRIG_CTRL_REG_ADDR                               (GE_NODE_REGS_REG_BASE      + 0x00000070)
#define HWIO_GE_NODE_DBD_TRIG_CTRL_REG_PHYS                               (GE_NODE_REGS_REG_BASE_PHYS + 0x00000070)
#define HWIO_GE_NODE_DBD_TRIG_CTRL_REG_OFFS                               (GE_NODE_REGS_REG_BASE_OFFS + 0x00000070)
#define HWIO_GE_NODE_DBD_TRIG_CTRL_REG_RMSK                                0x70fff13
#define HWIO_GE_NODE_DBD_TRIG_CTRL_REG_POR                                0x00000000
#define HWIO_GE_NODE_DBD_TRIG_CTRL_REG_POR_RMSK                           0xffffffff
#define HWIO_GE_NODE_DBD_TRIG_CTRL_REG_ATTR                                      0x3
#define HWIO_GE_NODE_DBD_TRIG_CTRL_REG_IN          \
        in_dword_masked(HWIO_GE_NODE_DBD_TRIG_CTRL_REG_ADDR, HWIO_GE_NODE_DBD_TRIG_CTRL_REG_RMSK)
#define HWIO_GE_NODE_DBD_TRIG_CTRL_REG_INM(m)      \
        in_dword_masked(HWIO_GE_NODE_DBD_TRIG_CTRL_REG_ADDR, m)
#define HWIO_GE_NODE_DBD_TRIG_CTRL_REG_OUT(v)      \
        out_dword(HWIO_GE_NODE_DBD_TRIG_CTRL_REG_ADDR,v)
#define HWIO_GE_NODE_DBD_TRIG_CTRL_REG_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GE_NODE_DBD_TRIG_CTRL_REG_ADDR,m,v,HWIO_GE_NODE_DBD_TRIG_CTRL_REG_IN)
#define HWIO_GE_NODE_DBD_TRIG_CTRL_REG_TRIG_COND_BMSK                      0x7000000
#define HWIO_GE_NODE_DBD_TRIG_CTRL_REG_TRIG_COND_SHFT                           0x18
#define HWIO_GE_NODE_DBD_TRIG_CTRL_REG_TRIG_CNT_BMSK                         0xfff00
#define HWIO_GE_NODE_DBD_TRIG_CTRL_REG_TRIG_CNT_SHFT                             0x8
#define HWIO_GE_NODE_DBD_TRIG_CTRL_REG_TRIG_CONSEC_BMSK                         0x10
#define HWIO_GE_NODE_DBD_TRIG_CTRL_REG_TRIG_CONSEC_SHFT                          0x4
#define HWIO_GE_NODE_DBD_TRIG_CTRL_REG_TRIG_NEXT_BMSK                            0x3
#define HWIO_GE_NODE_DBD_TRIG_CTRL_REG_TRIG_NEXT_SHFT                            0x0

#define HWIO_GE_NODE_DBD_TRIG_OFFSET_LSB_REG_ADDR                         (GE_NODE_REGS_REG_BASE      + 0x00000080)
#define HWIO_GE_NODE_DBD_TRIG_OFFSET_LSB_REG_PHYS                         (GE_NODE_REGS_REG_BASE_PHYS + 0x00000080)
#define HWIO_GE_NODE_DBD_TRIG_OFFSET_LSB_REG_OFFS                         (GE_NODE_REGS_REG_BASE_OFFS + 0x00000080)
#define HWIO_GE_NODE_DBD_TRIG_OFFSET_LSB_REG_RMSK                         0xffffffff
#define HWIO_GE_NODE_DBD_TRIG_OFFSET_LSB_REG_POR                          0x00000000
#define HWIO_GE_NODE_DBD_TRIG_OFFSET_LSB_REG_POR_RMSK                     0xffffffff
#define HWIO_GE_NODE_DBD_TRIG_OFFSET_LSB_REG_ATTR                                0x1
#define HWIO_GE_NODE_DBD_TRIG_OFFSET_LSB_REG_IN          \
        in_dword_masked(HWIO_GE_NODE_DBD_TRIG_OFFSET_LSB_REG_ADDR, HWIO_GE_NODE_DBD_TRIG_OFFSET_LSB_REG_RMSK)
#define HWIO_GE_NODE_DBD_TRIG_OFFSET_LSB_REG_INM(m)      \
        in_dword_masked(HWIO_GE_NODE_DBD_TRIG_OFFSET_LSB_REG_ADDR, m)
#define HWIO_GE_NODE_DBD_TRIG_OFFSET_LSB_REG_LSB_BMSK                     0xffffffff
#define HWIO_GE_NODE_DBD_TRIG_OFFSET_LSB_REG_LSB_SHFT                            0x0

#define HWIO_GE_NODE_DBD_TRIG_OFFSET_MSB_REG_ADDR                         (GE_NODE_REGS_REG_BASE      + 0x00000084)
#define HWIO_GE_NODE_DBD_TRIG_OFFSET_MSB_REG_PHYS                         (GE_NODE_REGS_REG_BASE_PHYS + 0x00000084)
#define HWIO_GE_NODE_DBD_TRIG_OFFSET_MSB_REG_OFFS                         (GE_NODE_REGS_REG_BASE_OFFS + 0x00000084)
#define HWIO_GE_NODE_DBD_TRIG_OFFSET_MSB_REG_RMSK                                0x1
#define HWIO_GE_NODE_DBD_TRIG_OFFSET_MSB_REG_POR                          0x00000000
#define HWIO_GE_NODE_DBD_TRIG_OFFSET_MSB_REG_POR_RMSK                     0xffffffff
#define HWIO_GE_NODE_DBD_TRIG_OFFSET_MSB_REG_ATTR                                0x1
#define HWIO_GE_NODE_DBD_TRIG_OFFSET_MSB_REG_IN          \
        in_dword_masked(HWIO_GE_NODE_DBD_TRIG_OFFSET_MSB_REG_ADDR, HWIO_GE_NODE_DBD_TRIG_OFFSET_MSB_REG_RMSK)
#define HWIO_GE_NODE_DBD_TRIG_OFFSET_MSB_REG_INM(m)      \
        in_dword_masked(HWIO_GE_NODE_DBD_TRIG_OFFSET_MSB_REG_ADDR, m)
#define HWIO_GE_NODE_DBD_TRIG_OFFSET_MSB_REG_MSB_BMSK                            0x1
#define HWIO_GE_NODE_DBD_TRIG_OFFSET_MSB_REG_MSB_SHFT                            0x0

#define HWIO_GE_NODE_DBD_CURR_ADDR_LSB_REG_ADDR                           (GE_NODE_REGS_REG_BASE      + 0x00000088)
#define HWIO_GE_NODE_DBD_CURR_ADDR_LSB_REG_PHYS                           (GE_NODE_REGS_REG_BASE_PHYS + 0x00000088)
#define HWIO_GE_NODE_DBD_CURR_ADDR_LSB_REG_OFFS                           (GE_NODE_REGS_REG_BASE_OFFS + 0x00000088)
#define HWIO_GE_NODE_DBD_CURR_ADDR_LSB_REG_RMSK                           0xffffffff
#define HWIO_GE_NODE_DBD_CURR_ADDR_LSB_REG_POR                            0x00000000
#define HWIO_GE_NODE_DBD_CURR_ADDR_LSB_REG_POR_RMSK                       0xffffffff
#define HWIO_GE_NODE_DBD_CURR_ADDR_LSB_REG_ATTR                                  0x1
#define HWIO_GE_NODE_DBD_CURR_ADDR_LSB_REG_IN          \
        in_dword_masked(HWIO_GE_NODE_DBD_CURR_ADDR_LSB_REG_ADDR, HWIO_GE_NODE_DBD_CURR_ADDR_LSB_REG_RMSK)
#define HWIO_GE_NODE_DBD_CURR_ADDR_LSB_REG_INM(m)      \
        in_dword_masked(HWIO_GE_NODE_DBD_CURR_ADDR_LSB_REG_ADDR, m)
#define HWIO_GE_NODE_DBD_CURR_ADDR_LSB_REG_LSB_BMSK                       0xffffffff
#define HWIO_GE_NODE_DBD_CURR_ADDR_LSB_REG_LSB_SHFT                              0x0

#define HWIO_GE_NODE_DBD_CURR_ADDR_MSB_REG_ADDR                           (GE_NODE_REGS_REG_BASE      + 0x0000008c)
#define HWIO_GE_NODE_DBD_CURR_ADDR_MSB_REG_PHYS                           (GE_NODE_REGS_REG_BASE_PHYS + 0x0000008c)
#define HWIO_GE_NODE_DBD_CURR_ADDR_MSB_REG_OFFS                           (GE_NODE_REGS_REG_BASE_OFFS + 0x0000008c)
#define HWIO_GE_NODE_DBD_CURR_ADDR_MSB_REG_RMSK                                  0xf
#define HWIO_GE_NODE_DBD_CURR_ADDR_MSB_REG_POR                            0x00000000
#define HWIO_GE_NODE_DBD_CURR_ADDR_MSB_REG_POR_RMSK                       0xffffffff
#define HWIO_GE_NODE_DBD_CURR_ADDR_MSB_REG_ATTR                                  0x1
#define HWIO_GE_NODE_DBD_CURR_ADDR_MSB_REG_IN          \
        in_dword_masked(HWIO_GE_NODE_DBD_CURR_ADDR_MSB_REG_ADDR, HWIO_GE_NODE_DBD_CURR_ADDR_MSB_REG_RMSK)
#define HWIO_GE_NODE_DBD_CURR_ADDR_MSB_REG_INM(m)      \
        in_dword_masked(HWIO_GE_NODE_DBD_CURR_ADDR_MSB_REG_ADDR, m)
#define HWIO_GE_NODE_DBD_CURR_ADDR_MSB_REG_MSB_BMSK                              0xf
#define HWIO_GE_NODE_DBD_CURR_ADDR_MSB_REG_MSB_SHFT                              0x0

#define HWIO_GE_NODE_DBD_TIMER_REG_ADDR                                   (GE_NODE_REGS_REG_BASE      + 0x00000090)
#define HWIO_GE_NODE_DBD_TIMER_REG_PHYS                                   (GE_NODE_REGS_REG_BASE_PHYS + 0x00000090)
#define HWIO_GE_NODE_DBD_TIMER_REG_OFFS                                   (GE_NODE_REGS_REG_BASE_OFFS + 0x00000090)
#define HWIO_GE_NODE_DBD_TIMER_REG_RMSK                                   0xffffffff
#define HWIO_GE_NODE_DBD_TIMER_REG_POR                                    0x00000000
#define HWIO_GE_NODE_DBD_TIMER_REG_POR_RMSK                               0xffffffff
#define HWIO_GE_NODE_DBD_TIMER_REG_ATTR                                          0x1
#define HWIO_GE_NODE_DBD_TIMER_REG_IN          \
        in_dword_masked(HWIO_GE_NODE_DBD_TIMER_REG_ADDR, HWIO_GE_NODE_DBD_TIMER_REG_RMSK)
#define HWIO_GE_NODE_DBD_TIMER_REG_INM(m)      \
        in_dword_masked(HWIO_GE_NODE_DBD_TIMER_REG_ADDR, m)
#define HWIO_GE_NODE_DBD_TIMER_REG_DBD_TIMER_BMSK                         0xffffffff
#define HWIO_GE_NODE_DBD_TIMER_REG_DBD_TIMER_SHFT                                0x0

#define HWIO_GE_NODE_DBD_TRIG_TIMESTAMP_REG_ADDR                          (GE_NODE_REGS_REG_BASE      + 0x00000094)
#define HWIO_GE_NODE_DBD_TRIG_TIMESTAMP_REG_PHYS                          (GE_NODE_REGS_REG_BASE_PHYS + 0x00000094)
#define HWIO_GE_NODE_DBD_TRIG_TIMESTAMP_REG_OFFS                          (GE_NODE_REGS_REG_BASE_OFFS + 0x00000094)
#define HWIO_GE_NODE_DBD_TRIG_TIMESTAMP_REG_RMSK                          0xffffffff
#define HWIO_GE_NODE_DBD_TRIG_TIMESTAMP_REG_POR                           0x00000000
#define HWIO_GE_NODE_DBD_TRIG_TIMESTAMP_REG_POR_RMSK                      0xffffffff
#define HWIO_GE_NODE_DBD_TRIG_TIMESTAMP_REG_ATTR                                 0x1
#define HWIO_GE_NODE_DBD_TRIG_TIMESTAMP_REG_IN          \
        in_dword_masked(HWIO_GE_NODE_DBD_TRIG_TIMESTAMP_REG_ADDR, HWIO_GE_NODE_DBD_TRIG_TIMESTAMP_REG_RMSK)
#define HWIO_GE_NODE_DBD_TRIG_TIMESTAMP_REG_INM(m)      \
        in_dword_masked(HWIO_GE_NODE_DBD_TRIG_TIMESTAMP_REG_ADDR, m)
#define HWIO_GE_NODE_DBD_TRIG_TIMESTAMP_REG_DBD_TRIG_TSTAMP_BMSK          0xffffffff
#define HWIO_GE_NODE_DBD_TRIG_TIMESTAMP_REG_DBD_TRIG_TSTAMP_SHFT                 0x0

#define HWIO_GE_NODE_DBD_DATA_MASK_REG_ADDR                               (GE_NODE_REGS_REG_BASE      + 0x00000098)
#define HWIO_GE_NODE_DBD_DATA_MASK_REG_PHYS                               (GE_NODE_REGS_REG_BASE_PHYS + 0x00000098)
#define HWIO_GE_NODE_DBD_DATA_MASK_REG_OFFS                               (GE_NODE_REGS_REG_BASE_OFFS + 0x00000098)
#define HWIO_GE_NODE_DBD_DATA_MASK_REG_RMSK                               0xffffffff
#define HWIO_GE_NODE_DBD_DATA_MASK_REG_POR                                0xffffffff
#define HWIO_GE_NODE_DBD_DATA_MASK_REG_POR_RMSK                           0xffffffff
#define HWIO_GE_NODE_DBD_DATA_MASK_REG_ATTR                                      0x3
#define HWIO_GE_NODE_DBD_DATA_MASK_REG_IN          \
        in_dword_masked(HWIO_GE_NODE_DBD_DATA_MASK_REG_ADDR, HWIO_GE_NODE_DBD_DATA_MASK_REG_RMSK)
#define HWIO_GE_NODE_DBD_DATA_MASK_REG_INM(m)      \
        in_dword_masked(HWIO_GE_NODE_DBD_DATA_MASK_REG_ADDR, m)
#define HWIO_GE_NODE_DBD_DATA_MASK_REG_OUT(v)      \
        out_dword(HWIO_GE_NODE_DBD_DATA_MASK_REG_ADDR,v)
#define HWIO_GE_NODE_DBD_DATA_MASK_REG_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GE_NODE_DBD_DATA_MASK_REG_ADDR,m,v,HWIO_GE_NODE_DBD_DATA_MASK_REG_IN)
#define HWIO_GE_NODE_DBD_DATA_MASK_REG_DATA_MASK_BMSK                     0xffffffff
#define HWIO_GE_NODE_DBD_DATA_MASK_REG_DATA_MASK_SHFT                            0x0

#define HWIO_GE_NODE_DBD_SW_MASK_REG_ADDR                                 (GE_NODE_REGS_REG_BASE      + 0x0000009c)
#define HWIO_GE_NODE_DBD_SW_MASK_REG_PHYS                                 (GE_NODE_REGS_REG_BASE_PHYS + 0x0000009c)
#define HWIO_GE_NODE_DBD_SW_MASK_REG_OFFS                                 (GE_NODE_REGS_REG_BASE_OFFS + 0x0000009c)
#define HWIO_GE_NODE_DBD_SW_MASK_REG_RMSK                                 0xffffffff
#define HWIO_GE_NODE_DBD_SW_MASK_REG_POR                                  0x00000000
#define HWIO_GE_NODE_DBD_SW_MASK_REG_POR_RMSK                             0xffffffff
#define HWIO_GE_NODE_DBD_SW_MASK_REG_ATTR                                        0x3
#define HWIO_GE_NODE_DBD_SW_MASK_REG_IN          \
        in_dword_masked(HWIO_GE_NODE_DBD_SW_MASK_REG_ADDR, HWIO_GE_NODE_DBD_SW_MASK_REG_RMSK)
#define HWIO_GE_NODE_DBD_SW_MASK_REG_INM(m)      \
        in_dword_masked(HWIO_GE_NODE_DBD_SW_MASK_REG_ADDR, m)
#define HWIO_GE_NODE_DBD_SW_MASK_REG_OUT(v)      \
        out_dword(HWIO_GE_NODE_DBD_SW_MASK_REG_ADDR,v)
#define HWIO_GE_NODE_DBD_SW_MASK_REG_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GE_NODE_DBD_SW_MASK_REG_ADDR,m,v,HWIO_GE_NODE_DBD_SW_MASK_REG_IN)
#define HWIO_GE_NODE_DBD_SW_MASK_REG_SW_MASK_BMSK                         0xffffffff
#define HWIO_GE_NODE_DBD_SW_MASK_REG_SW_MASK_SHFT                                0x0

#define HWIO_GE_NODE_DBD_SW_VAL_REG_ADDR                                  (GE_NODE_REGS_REG_BASE      + 0x000000a0)
#define HWIO_GE_NODE_DBD_SW_VAL_REG_PHYS                                  (GE_NODE_REGS_REG_BASE_PHYS + 0x000000a0)
#define HWIO_GE_NODE_DBD_SW_VAL_REG_OFFS                                  (GE_NODE_REGS_REG_BASE_OFFS + 0x000000a0)
#define HWIO_GE_NODE_DBD_SW_VAL_REG_RMSK                                  0xffffffff
#define HWIO_GE_NODE_DBD_SW_VAL_REG_POR                                   0x00000000
#define HWIO_GE_NODE_DBD_SW_VAL_REG_POR_RMSK                              0xffffffff
#define HWIO_GE_NODE_DBD_SW_VAL_REG_ATTR                                         0x3
#define HWIO_GE_NODE_DBD_SW_VAL_REG_IN          \
        in_dword_masked(HWIO_GE_NODE_DBD_SW_VAL_REG_ADDR, HWIO_GE_NODE_DBD_SW_VAL_REG_RMSK)
#define HWIO_GE_NODE_DBD_SW_VAL_REG_INM(m)      \
        in_dword_masked(HWIO_GE_NODE_DBD_SW_VAL_REG_ADDR, m)
#define HWIO_GE_NODE_DBD_SW_VAL_REG_OUT(v)      \
        out_dword(HWIO_GE_NODE_DBD_SW_VAL_REG_ADDR,v)
#define HWIO_GE_NODE_DBD_SW_VAL_REG_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GE_NODE_DBD_SW_VAL_REG_ADDR,m,v,HWIO_GE_NODE_DBD_SW_VAL_REG_IN)
#define HWIO_GE_NODE_DBD_SW_VAL_REG_SW_VAL_BMSK                           0xffffffff
#define HWIO_GE_NODE_DBD_SW_VAL_REG_SW_VAL_SHFT                                  0x0

/*----------------------------------------------------------------------------
 * MODULE: GE_NODE_ROM
 *--------------------------------------------------------------------------*/

#define GE_NODE_ROM_REG_BASE                              (GE_GEN_EMU_BASE      + 0x001f8000)
#define GE_NODE_ROM_REG_BASE_PHYS                         (GE_GEN_EMU_BASE_PHYS + 0x001f8000)
#define GE_NODE_ROM_REG_BASE_OFFS                         0x001f8000

#define HWIO_GE_NODE_ROM_STRING_SIZE_ADDR                 (GE_NODE_ROM_REG_BASE      + 0x00000004)
#define HWIO_GE_NODE_ROM_STRING_SIZE_PHYS                 (GE_NODE_ROM_REG_BASE_PHYS + 0x00000004)
#define HWIO_GE_NODE_ROM_STRING_SIZE_OFFS                 (GE_NODE_ROM_REG_BASE_OFFS + 0x00000004)
#define HWIO_GE_NODE_ROM_STRING_SIZE_RMSK                 0xffffffff
#define HWIO_GE_NODE_ROM_STRING_SIZE_POR                  0x00000000
#define HWIO_GE_NODE_ROM_STRING_SIZE_POR_RMSK             0xffffffff
#define HWIO_GE_NODE_ROM_STRING_SIZE_ATTR                        0x1
#define HWIO_GE_NODE_ROM_STRING_SIZE_IN          \
        in_dword_masked(HWIO_GE_NODE_ROM_STRING_SIZE_ADDR, HWIO_GE_NODE_ROM_STRING_SIZE_RMSK)
#define HWIO_GE_NODE_ROM_STRING_SIZE_INM(m)      \
        in_dword_masked(HWIO_GE_NODE_ROM_STRING_SIZE_ADDR, m)
#define HWIO_GE_NODE_ROM_STRING_SIZE_LENGTH_BMSK          0xffffffff
#define HWIO_GE_NODE_ROM_STRING_SIZE_LENGTH_SHFT                 0x0

/*----------------------------------------------------------------------------
 * MODULE: GE_PCIE_DMA
 *--------------------------------------------------------------------------*/

#define GE_PCIE_DMA_REG_BASE (GE_PCIE_DMA_BASE      + 0x00000000)
#define GE_PCIE_DMA_REG_BASE_PHYS (GE_PCIE_DMA_BASE_PHYS + 0x00000000)
#define GE_PCIE_DMA_REG_BASE_OFFS 0x00000000

/*----------------------------------------------------------------------------
 * MODULE: GE_CM3_MMU
 *--------------------------------------------------------------------------*/

#define GE_CM3_MMU_REG_BASE                                             (GE_CM3_MMU_BASE      + 0x00000000)
#define GE_CM3_MMU_REG_BASE_PHYS                                        (GE_CM3_MMU_BASE_PHYS + 0x00000000)
#define GE_CM3_MMU_REG_BASE_OFFS                                        0x00000000

#define HWIO_GE_CM3_MMU_IBUS_DBUS_MMU_BASE_ADDR                         (GE_CM3_MMU_REG_BASE      + 0x00000000)
#define HWIO_GE_CM3_MMU_IBUS_DBUS_MMU_BASE_PHYS                         (GE_CM3_MMU_REG_BASE_PHYS + 0x00000000)
#define HWIO_GE_CM3_MMU_IBUS_DBUS_MMU_BASE_OFFS                         (GE_CM3_MMU_REG_BASE_OFFS + 0x00000000)
#define HWIO_GE_CM3_MMU_IBUS_DBUS_MMU_BASE_RMSK                            0xfffff
#define HWIO_GE_CM3_MMU_IBUS_DBUS_MMU_BASE_POR                          0x00000000
#define HWIO_GE_CM3_MMU_IBUS_DBUS_MMU_BASE_POR_RMSK                     0xffffffff
#define HWIO_GE_CM3_MMU_IBUS_DBUS_MMU_BASE_ATTR                                0x3
#define HWIO_GE_CM3_MMU_IBUS_DBUS_MMU_BASE_IN          \
        in_dword_masked(HWIO_GE_CM3_MMU_IBUS_DBUS_MMU_BASE_ADDR, HWIO_GE_CM3_MMU_IBUS_DBUS_MMU_BASE_RMSK)
#define HWIO_GE_CM3_MMU_IBUS_DBUS_MMU_BASE_INM(m)      \
        in_dword_masked(HWIO_GE_CM3_MMU_IBUS_DBUS_MMU_BASE_ADDR, m)
#define HWIO_GE_CM3_MMU_IBUS_DBUS_MMU_BASE_OUT(v)      \
        out_dword(HWIO_GE_CM3_MMU_IBUS_DBUS_MMU_BASE_ADDR,v)
#define HWIO_GE_CM3_MMU_IBUS_DBUS_MMU_BASE_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GE_CM3_MMU_IBUS_DBUS_MMU_BASE_ADDR,m,v,HWIO_GE_CM3_MMU_IBUS_DBUS_MMU_BASE_IN)
#define HWIO_GE_CM3_MMU_IBUS_DBUS_MMU_BASE_BASE_MSB_20BIT_BMSK             0xfffff
#define HWIO_GE_CM3_MMU_IBUS_DBUS_MMU_BASE_BASE_MSB_20BIT_SHFT                 0x0

#define HWIO_GE_CM3_MMU_IBUS_DBUS_MMU_MASK_ADDR                         (GE_CM3_MMU_REG_BASE      + 0x00000004)
#define HWIO_GE_CM3_MMU_IBUS_DBUS_MMU_MASK_PHYS                         (GE_CM3_MMU_REG_BASE_PHYS + 0x00000004)
#define HWIO_GE_CM3_MMU_IBUS_DBUS_MMU_MASK_OFFS                         (GE_CM3_MMU_REG_BASE_OFFS + 0x00000004)
#define HWIO_GE_CM3_MMU_IBUS_DBUS_MMU_MASK_RMSK                             0xffff
#define HWIO_GE_CM3_MMU_IBUS_DBUS_MMU_MASK_POR                          0x00000000
#define HWIO_GE_CM3_MMU_IBUS_DBUS_MMU_MASK_POR_RMSK                     0xffffffff
#define HWIO_GE_CM3_MMU_IBUS_DBUS_MMU_MASK_ATTR                                0x3
#define HWIO_GE_CM3_MMU_IBUS_DBUS_MMU_MASK_IN          \
        in_dword_masked(HWIO_GE_CM3_MMU_IBUS_DBUS_MMU_MASK_ADDR, HWIO_GE_CM3_MMU_IBUS_DBUS_MMU_MASK_RMSK)
#define HWIO_GE_CM3_MMU_IBUS_DBUS_MMU_MASK_INM(m)      \
        in_dword_masked(HWIO_GE_CM3_MMU_IBUS_DBUS_MMU_MASK_ADDR, m)
#define HWIO_GE_CM3_MMU_IBUS_DBUS_MMU_MASK_OUT(v)      \
        out_dword(HWIO_GE_CM3_MMU_IBUS_DBUS_MMU_MASK_ADDR,v)
#define HWIO_GE_CM3_MMU_IBUS_DBUS_MMU_MASK_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GE_CM3_MMU_IBUS_DBUS_MMU_MASK_ADDR,m,v,HWIO_GE_CM3_MMU_IBUS_DBUS_MMU_MASK_IN)
#define HWIO_GE_CM3_MMU_IBUS_DBUS_MMU_MASK_MASK_MSB_16BIT_BMSK              0xffff
#define HWIO_GE_CM3_MMU_IBUS_DBUS_MMU_MASK_MASK_MSB_16BIT_SHFT                 0x0

#define HWIO_GE_CM3_MMU_SBUS_MMU_BASE_ADDR                              (GE_CM3_MMU_REG_BASE      + 0x00000008)
#define HWIO_GE_CM3_MMU_SBUS_MMU_BASE_PHYS                              (GE_CM3_MMU_REG_BASE_PHYS + 0x00000008)
#define HWIO_GE_CM3_MMU_SBUS_MMU_BASE_OFFS                              (GE_CM3_MMU_REG_BASE_OFFS + 0x00000008)
#define HWIO_GE_CM3_MMU_SBUS_MMU_BASE_RMSK                                 0xfffff
#define HWIO_GE_CM3_MMU_SBUS_MMU_BASE_POR                               0x00000000
#define HWIO_GE_CM3_MMU_SBUS_MMU_BASE_POR_RMSK                          0xffffffff
#define HWIO_GE_CM3_MMU_SBUS_MMU_BASE_ATTR                                     0x3
#define HWIO_GE_CM3_MMU_SBUS_MMU_BASE_IN          \
        in_dword_masked(HWIO_GE_CM3_MMU_SBUS_MMU_BASE_ADDR, HWIO_GE_CM3_MMU_SBUS_MMU_BASE_RMSK)
#define HWIO_GE_CM3_MMU_SBUS_MMU_BASE_INM(m)      \
        in_dword_masked(HWIO_GE_CM3_MMU_SBUS_MMU_BASE_ADDR, m)
#define HWIO_GE_CM3_MMU_SBUS_MMU_BASE_OUT(v)      \
        out_dword(HWIO_GE_CM3_MMU_SBUS_MMU_BASE_ADDR,v)
#define HWIO_GE_CM3_MMU_SBUS_MMU_BASE_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GE_CM3_MMU_SBUS_MMU_BASE_ADDR,m,v,HWIO_GE_CM3_MMU_SBUS_MMU_BASE_IN)
#define HWIO_GE_CM3_MMU_SBUS_MMU_BASE_BASE_MSB_20BIT_BMSK                  0xfffff
#define HWIO_GE_CM3_MMU_SBUS_MMU_BASE_BASE_MSB_20BIT_SHFT                      0x0

#define HWIO_GE_CM3_MMU_SBUS_MMU_MASK_ADDR                              (GE_CM3_MMU_REG_BASE      + 0x0000000c)
#define HWIO_GE_CM3_MMU_SBUS_MMU_MASK_PHYS                              (GE_CM3_MMU_REG_BASE_PHYS + 0x0000000c)
#define HWIO_GE_CM3_MMU_SBUS_MMU_MASK_OFFS                              (GE_CM3_MMU_REG_BASE_OFFS + 0x0000000c)
#define HWIO_GE_CM3_MMU_SBUS_MMU_MASK_RMSK                                  0xffff
#define HWIO_GE_CM3_MMU_SBUS_MMU_MASK_POR                               0x00000000
#define HWIO_GE_CM3_MMU_SBUS_MMU_MASK_POR_RMSK                          0xffffffff
#define HWIO_GE_CM3_MMU_SBUS_MMU_MASK_ATTR                                     0x3
#define HWIO_GE_CM3_MMU_SBUS_MMU_MASK_IN          \
        in_dword_masked(HWIO_GE_CM3_MMU_SBUS_MMU_MASK_ADDR, HWIO_GE_CM3_MMU_SBUS_MMU_MASK_RMSK)
#define HWIO_GE_CM3_MMU_SBUS_MMU_MASK_INM(m)      \
        in_dword_masked(HWIO_GE_CM3_MMU_SBUS_MMU_MASK_ADDR, m)
#define HWIO_GE_CM3_MMU_SBUS_MMU_MASK_OUT(v)      \
        out_dword(HWIO_GE_CM3_MMU_SBUS_MMU_MASK_ADDR,v)
#define HWIO_GE_CM3_MMU_SBUS_MMU_MASK_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GE_CM3_MMU_SBUS_MMU_MASK_ADDR,m,v,HWIO_GE_CM3_MMU_SBUS_MMU_MASK_IN)
#define HWIO_GE_CM3_MMU_SBUS_MMU_MASK_MASK_MSB_16BIT_BMSK                   0xffff
#define HWIO_GE_CM3_MMU_SBUS_MMU_MASK_MASK_MSB_16BIT_SHFT                      0x0

#define HWIO_GE_CM3_MMU_BUS_CTRL_REG_ADDR                               (GE_CM3_MMU_REG_BASE      + 0x00000010)
#define HWIO_GE_CM3_MMU_BUS_CTRL_REG_PHYS                               (GE_CM3_MMU_REG_BASE_PHYS + 0x00000010)
#define HWIO_GE_CM3_MMU_BUS_CTRL_REG_OFFS                               (GE_CM3_MMU_REG_BASE_OFFS + 0x00000010)
#define HWIO_GE_CM3_MMU_BUS_CTRL_REG_RMSK                                      0x1
#define HWIO_GE_CM3_MMU_BUS_CTRL_REG_POR                                0x00000000
#define HWIO_GE_CM3_MMU_BUS_CTRL_REG_POR_RMSK                           0xffffffff
#define HWIO_GE_CM3_MMU_BUS_CTRL_REG_ATTR                                      0x3
#define HWIO_GE_CM3_MMU_BUS_CTRL_REG_IN          \
        in_dword_masked(HWIO_GE_CM3_MMU_BUS_CTRL_REG_ADDR, HWIO_GE_CM3_MMU_BUS_CTRL_REG_RMSK)
#define HWIO_GE_CM3_MMU_BUS_CTRL_REG_INM(m)      \
        in_dword_masked(HWIO_GE_CM3_MMU_BUS_CTRL_REG_ADDR, m)
#define HWIO_GE_CM3_MMU_BUS_CTRL_REG_OUT(v)      \
        out_dword(HWIO_GE_CM3_MMU_BUS_CTRL_REG_ADDR,v)
#define HWIO_GE_CM3_MMU_BUS_CTRL_REG_OUTM(m,v) \
        out_dword_masked_ns(HWIO_GE_CM3_MMU_BUS_CTRL_REG_ADDR,m,v,HWIO_GE_CM3_MMU_BUS_CTRL_REG_IN)
#define HWIO_GE_CM3_MMU_BUS_CTRL_REG_DBUS_IS_SBUS_BMSK                         0x1
#define HWIO_GE_CM3_MMU_BUS_CTRL_REG_DBUS_IS_SBUS_SHFT                         0x0

#define HWIO_GE_CM3_MMU_DUMMY_REG_ADDR                                  (GE_CM3_MMU_REG_BASE      + 0x00000014)
#define HWIO_GE_CM3_MMU_DUMMY_REG_PHYS                                  (GE_CM3_MMU_REG_BASE_PHYS + 0x00000014)
#define HWIO_GE_CM3_MMU_DUMMY_REG_OFFS                                  (GE_CM3_MMU_REG_BASE_OFFS + 0x00000014)
#define HWIO_GE_CM3_MMU_DUMMY_REG_RMSK                                  0xffffffff
#define HWIO_GE_CM3_MMU_DUMMY_REG_POR                                   0x0badc0de
#define HWIO_GE_CM3_MMU_DUMMY_REG_POR_RMSK                              0xffffffff
#define HWIO_GE_CM3_MMU_DUMMY_REG_ATTR                                         0x1
#define HWIO_GE_CM3_MMU_DUMMY_REG_IN          \
        in_dword_masked(HWIO_GE_CM3_MMU_DUMMY_REG_ADDR, HWIO_GE_CM3_MMU_DUMMY_REG_RMSK)
#define HWIO_GE_CM3_MMU_DUMMY_REG_INM(m)      \
        in_dword_masked(HWIO_GE_CM3_MMU_DUMMY_REG_ADDR, m)
#define HWIO_GE_CM3_MMU_DUMMY_REG_BADCODE_BMSK                          0xffffffff
#define HWIO_GE_CM3_MMU_DUMMY_REG_BADCODE_SHFT                                 0x0

/*----------------------------------------------------------------------------
 * MODULE: GEN_EMU
 *--------------------------------------------------------------------------*/

#define GEN_EMU_REG_BASE     (GEN_EMU_BASE      + 0x00000000)
#define GEN_EMU_REG_BASE_PHYS (GEN_EMU_BASE_PHYS + 0x00000000)
#define GEN_EMU_REG_BASE_OFFS 0x00000000

/*----------------------------------------------------------------------------
 * MODULE: GE_NODE_DDR_WINDOW
 *--------------------------------------------------------------------------*/

#define GE_NODE_DDR_WINDOW_REG_BASE (GE_NODE_DDR_WINDOW_BASE      + 0x00000000)
#define GE_NODE_DDR_WINDOW_REG_BASE_PHYS (GE_NODE_DDR_WINDOW_BASE_PHYS + 0x00000000)
#define GE_NODE_DDR_WINDOW_REG_BASE_OFFS 0x00000000

/*----------------------------------------------------------------------------
 * MODULE: GE_NODE_DDR
 *--------------------------------------------------------------------------*/

#define GE_NODE_DDR_REG_BASE (GE_NODE_DDR_BASE      + 0x00000000)
#define GE_NODE_DDR_REG_BASE_PHYS (GE_NODE_DDR_BASE_PHYS + 0x00000000)
#define GE_NODE_DDR_REG_BASE_OFFS 0x00000000


#endif /* __MSMHWIOREG_GEN_EMU_H__ */
