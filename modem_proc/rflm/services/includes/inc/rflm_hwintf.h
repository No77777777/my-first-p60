/*!
  @file
  rflm_hwintf.h

  @brief
  Hardware register definitions.

*/

/*===========================================================================

  Copyright (c) 2013-2015 Qualcomm Technologies, Inc.  All Rights Reserved.
  Qualcomm Technologies Proprietary and Confidential.

  Export of this technology or software is regulated by the U.S. Government.
  Diversion contrary to U.S. law prohibited.

  All ideas, data and information contained in or disclosed by
  this document are confidential and proprietary information of
  Qualcomm Technologies, Incorporated and all rights therein are expressly reserved.
  By accepting this material the recipient agrees that this material
  and the information contained therein are held in confidence and in
  trust and will not be used, copied, reproduced in whole or in part,
  nor its contents revealed in any manner to others without the express
  written permission of Qualcomm Technologies, Incorporated.

===========================================================================*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/rflm.mpss/2.11/services/includes/inc/rflm_hwintf.h#1 $

when         who     what, where, why
----------   ---     --------------------------------------------------------
05/29/15     dej     Add handling of V2 registers
05/28/14     sar     Removed interim HWIO definitions to fix warnings.
04/22/14     sar     Interim HWIO definition for HWIO_TCSR_SOC_HW_VERSION_ADDR.
2014-04-15   jc      make compatible for use in assembly file
10/24/13     vrb     Interim HWIO definitions for FLAT file name changes 
2013-07-18   JC      Port from modem_fw_hwintf.h

===========================================================================*/

#ifndef RFLM_HWINTF_H
#define RFLM_HWINTF_H

/*===========================================================================

                           INCLUDE FILES

===========================================================================*/

#ifndef __ASSEMBLER__
#include "rflm.h"
#endif  // __ASSEMBLER__

/* The msmhwio files were generated using the AMSS parser.
   See http://qwiki.qualcomm.com/qct-drvsw/HWIO for details */
#if (defined(TEST_FRAMEWORK) && defined(FEATURE_BOLT_MODEM)) 
#error code not present
#endif

#ifndef __ASSEMBLER__
#include "msmhwio.h"
#endif  // __ASSEMBLER__

#if (RFLM_FEATURE_BUILD_MODE==RFLM_FEATURE_BUILD_NORMAL)
#include "mcs_hwio_modem.h"
#include "msmhwiobase.h"

#else
#include "modem_fw_hwioreg.h"
#endif

#if (defined(TEST_FRAMEWORK) && defined(FEATURE_BOLT_MODEM)) 
#error code not present
#endif

//#include "hw_irq.h"
//#include "hw_task_def.h"


/* Target-specific HW definitions and overrides */
//#include "modem_fw_target_hw.h"


/*! @brief Extract a register field from "input". Note the register is NOT
    implicitly read.
    This macro is useful for extracting several fields from a register without
    doing multiple HW reads, as HWIO_INM() would do.
*/
#define HWIO_EXTRACT(input, reg, field) \
  ( ( ((uint32) (input)) & (HWIO_FMSK(reg, field)) ) >> (HWIO_SHFT(reg, field)) ) 


/*! @brief Insert a register field "value" into "input". Note the register
    is NOT implicitly read/written.
    This macro is useful for inserting several fields into a register without
    doing multiple HW reads/writes, as HWIO_OUTM() would do.
*/
/* XXX Using standard expressions instead of intrinsic helps compiler merge
       multiple inserts together. 
       However, it also adds a redundant and() instruction. Check with GCC 4.x.
*/
#define HWIO_INSERT(input, reg, field, value) \
  ( ( ((uint32) (input)) & ~(HWIO_FMSK(reg, field)) ) | \
    ( ( (value) & ( (HWIO_FMSK(reg, field)) >> (HWIO_SHFT(reg, field)) ) ) << (HWIO_SHFT(reg, field)) ) )


/*===========================================================================

                   EXTERNAL DEFINITIONS AND TYPES

===========================================================================*/

#ifndef HWIO_MSS_MODEM_DTR_PLL1_MODE_ADDR

#define HWIO_MSS_MODEM_DTR_PLL1_MODE_ADDR                                  (MODEM_DTR_ANALOG_REG_BASE      + 0x00000000)
#define HWIO_MSS_MODEM_DTR_PLL1_MODE_PHYS                                  (MODEM_DTR_ANALOG_REG_BASE_PHYS + 0x00000000)
#define HWIO_MSS_MODEM_DTR_PLL1_MODE_RMSK                                  0xffffffff
#define HWIO_MSS_MODEM_DTR_PLL1_MODE_IN          \
        in_dword_masked(HWIO_MSS_MODEM_DTR_PLL1_MODE_ADDR, HWIO_MSS_MODEM_DTR_PLL1_MODE_RMSK)
#define HWIO_MSS_MODEM_DTR_PLL1_MODE_INM(m)      \
        in_dword_masked(HWIO_MSS_MODEM_DTR_PLL1_MODE_ADDR, m)
#define HWIO_MSS_MODEM_DTR_PLL1_MODE_OUT(v)      \
        out_dword(HWIO_MSS_MODEM_DTR_PLL1_MODE_ADDR,v)
#define HWIO_MSS_MODEM_DTR_PLL1_MODE_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_MODEM_DTR_PLL1_MODE_ADDR,m,v,HWIO_MSS_MODEM_DTR_PLL1_MODE_IN)
#define HWIO_MSS_MODEM_DTR_PLL1_MODE_LOCK_DET_BMSK                         0x80000000
#define HWIO_MSS_MODEM_DTR_PLL1_MODE_LOCK_DET_SHFT                               0x1f
#define HWIO_MSS_MODEM_DTR_PLL1_MODE_ACTIVE_FLAG_BMSK                      0x40000000
#define HWIO_MSS_MODEM_DTR_PLL1_MODE_ACTIVE_FLAG_SHFT                            0x1e
#define HWIO_MSS_MODEM_DTR_PLL1_MODE_LATCH_ACK_BMSK                        0x20000000
#define HWIO_MSS_MODEM_DTR_PLL1_MODE_LATCH_ACK_SHFT                              0x1d
#define HWIO_MSS_MODEM_DTR_PLL1_MODE_USER_RSRVD2_BMSK                      0x1f000000
#define HWIO_MSS_MODEM_DTR_PLL1_MODE_USER_RSRVD2_SHFT                            0x18
#define HWIO_MSS_MODEM_DTR_PLL1_MODE_HW_UPDATE_LOGIC_BYPASS_BMSK             0x800000
#define HWIO_MSS_MODEM_DTR_PLL1_MODE_HW_UPDATE_LOGIC_BYPASS_SHFT                 0x17
#define HWIO_MSS_MODEM_DTR_PLL1_MODE_UPDATE_BMSK                             0x400000
#define HWIO_MSS_MODEM_DTR_PLL1_MODE_UPDATE_SHFT                                 0x16
#define HWIO_MSS_MODEM_DTR_PLL1_MODE_VOTE_FSM_RESET_BMSK                     0x200000
#define HWIO_MSS_MODEM_DTR_PLL1_MODE_VOTE_FSM_RESET_SHFT                         0x15
#define HWIO_MSS_MODEM_DTR_PLL1_MODE_VOTE_FSM_ENA_BMSK                       0x100000
#define HWIO_MSS_MODEM_DTR_PLL1_MODE_VOTE_FSM_ENA_SHFT                           0x14
#define HWIO_MSS_MODEM_DTR_PLL1_MODE_BIAS_COUNT_BMSK                          0xfc000
#define HWIO_MSS_MODEM_DTR_PLL1_MODE_BIAS_COUNT_SHFT                              0xe
#define HWIO_MSS_MODEM_DTR_PLL1_MODE_LOCK_COUNT_BMSK                           0x3f00
#define HWIO_MSS_MODEM_DTR_PLL1_MODE_LOCK_COUNT_SHFT                              0x8
#define HWIO_MSS_MODEM_DTR_PLL1_MODE_USER_RSRVD1_BMSK                            0xc0
#define HWIO_MSS_MODEM_DTR_PLL1_MODE_USER_RSRVD1_SHFT                             0x6
#define HWIO_MSS_MODEM_DTR_PLL1_MODE_REF_XO_SELECT_BMSK                          0x30
#define HWIO_MSS_MODEM_DTR_PLL1_MODE_REF_XO_SELECT_SHFT                           0x4
#define HWIO_MSS_MODEM_DTR_PLL1_MODE_PLLTEST_BMSK                                 0x8
#define HWIO_MSS_MODEM_DTR_PLL1_MODE_PLLTEST_SHFT                                 0x3
#define HWIO_MSS_MODEM_DTR_PLL1_MODE_RESET_N_BMSK                                 0x4
#define HWIO_MSS_MODEM_DTR_PLL1_MODE_RESET_N_SHFT                                 0x2
#define HWIO_MSS_MODEM_DTR_PLL1_MODE_BYPASSNL_BMSK                                0x2
#define HWIO_MSS_MODEM_DTR_PLL1_MODE_BYPASSNL_SHFT                                0x1
#define HWIO_MSS_MODEM_DTR_PLL1_MODE_OUTCTRL_BMSK                                 0x1
#define HWIO_MSS_MODEM_DTR_PLL1_MODE_OUTCTRL_SHFT                                 0x0

#define HWIO_MSS_MODEM_DTR_PLL1_L_VAL_ADDR                                 (MODEM_DTR_ANALOG_REG_BASE      + 0x00000008)
#define HWIO_MSS_MODEM_DTR_PLL1_L_VAL_PHYS                                 (MODEM_DTR_ANALOG_REG_BASE_PHYS + 0x00000008)
#define HWIO_MSS_MODEM_DTR_PLL1_L_VAL_RMSK                                       0xff
#define HWIO_MSS_MODEM_DTR_PLL1_L_VAL_IN          \
        in_dword_masked(HWIO_MSS_MODEM_DTR_PLL1_L_VAL_ADDR, HWIO_MSS_MODEM_DTR_PLL1_L_VAL_RMSK)
#define HWIO_MSS_MODEM_DTR_PLL1_L_VAL_INM(m)      \
        in_dword_masked(HWIO_MSS_MODEM_DTR_PLL1_L_VAL_ADDR, m)
#define HWIO_MSS_MODEM_DTR_PLL1_L_VAL_OUT(v)      \
        out_dword(HWIO_MSS_MODEM_DTR_PLL1_L_VAL_ADDR,v)
#define HWIO_MSS_MODEM_DTR_PLL1_L_VAL_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_MODEM_DTR_PLL1_L_VAL_ADDR,m,v,HWIO_MSS_MODEM_DTR_PLL1_L_VAL_IN)
#define HWIO_MSS_MODEM_DTR_PLL1_L_VAL_L_LVAL_BMSK                                0xff
#define HWIO_MSS_MODEM_DTR_PLL1_L_VAL_L_LVAL_SHFT                                 0x0

#define HWIO_MSS_MODEM_DTR_PLL1_ALPHA_VAL_L_ADDR                           (MODEM_DTR_ANALOG_REG_BASE      + 0x00000010)
#define HWIO_MSS_MODEM_DTR_PLL1_ALPHA_VAL_L_PHYS                           (MODEM_DTR_ANALOG_REG_BASE_PHYS + 0x00000010)
#define HWIO_MSS_MODEM_DTR_PLL1_ALPHA_VAL_L_RMSK                           0xffffffff
#define HWIO_MSS_MODEM_DTR_PLL1_ALPHA_VAL_L_IN          \
        in_dword_masked(HWIO_MSS_MODEM_DTR_PLL1_ALPHA_VAL_L_ADDR, HWIO_MSS_MODEM_DTR_PLL1_ALPHA_VAL_L_RMSK)
#define HWIO_MSS_MODEM_DTR_PLL1_ALPHA_VAL_L_INM(m)      \
        in_dword_masked(HWIO_MSS_MODEM_DTR_PLL1_ALPHA_VAL_L_ADDR, m)
#define HWIO_MSS_MODEM_DTR_PLL1_ALPHA_VAL_L_OUT(v)      \
        out_dword(HWIO_MSS_MODEM_DTR_PLL1_ALPHA_VAL_L_ADDR,v)
#define HWIO_MSS_MODEM_DTR_PLL1_ALPHA_VAL_L_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_MODEM_DTR_PLL1_ALPHA_VAL_L_ADDR,m,v,HWIO_MSS_MODEM_DTR_PLL1_ALPHA_VAL_L_IN)
#define HWIO_MSS_MODEM_DTR_PLL1_ALPHA_VAL_L_ALPHA_VAL_L_BMSK               0xffffffff
#define HWIO_MSS_MODEM_DTR_PLL1_ALPHA_VAL_L_ALPHA_VAL_L_SHFT                      0x0

#define HWIO_MSS_MODEM_DTR_PLL1_ALPHA_VAL_H_ADDR                           (MODEM_DTR_ANALOG_REG_BASE      + 0x00000018)
#define HWIO_MSS_MODEM_DTR_PLL1_ALPHA_VAL_H_PHYS                           (MODEM_DTR_ANALOG_REG_BASE_PHYS + 0x00000018)
#define HWIO_MSS_MODEM_DTR_PLL1_ALPHA_VAL_H_RMSK                                 0xff
#define HWIO_MSS_MODEM_DTR_PLL1_ALPHA_VAL_H_IN          \
        in_dword_masked(HWIO_MSS_MODEM_DTR_PLL1_ALPHA_VAL_H_ADDR, HWIO_MSS_MODEM_DTR_PLL1_ALPHA_VAL_H_RMSK)
#define HWIO_MSS_MODEM_DTR_PLL1_ALPHA_VAL_H_INM(m)      \
        in_dword_masked(HWIO_MSS_MODEM_DTR_PLL1_ALPHA_VAL_H_ADDR, m)
#define HWIO_MSS_MODEM_DTR_PLL1_ALPHA_VAL_H_OUT(v)      \
        out_dword(HWIO_MSS_MODEM_DTR_PLL1_ALPHA_VAL_H_ADDR,v)
#define HWIO_MSS_MODEM_DTR_PLL1_ALPHA_VAL_H_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_MODEM_DTR_PLL1_ALPHA_VAL_H_ADDR,m,v,HWIO_MSS_MODEM_DTR_PLL1_ALPHA_VAL_H_IN)
#define HWIO_MSS_MODEM_DTR_PLL1_ALPHA_VAL_H_PLL1_ALPHA_VAL_H_BMSK                0xff
#define HWIO_MSS_MODEM_DTR_PLL1_ALPHA_VAL_H_PLL1_ALPHA_VAL_H_SHFT                 0x0

#define HWIO_MSS_MODEM_DTR_PLL1_USER_CTL_ADDR                              (MODEM_DTR_ANALOG_REG_BASE      + 0x00000020)
#define HWIO_MSS_MODEM_DTR_PLL1_USER_CTL_PHYS                              (MODEM_DTR_ANALOG_REG_BASE_PHYS + 0x00000020)
#define HWIO_MSS_MODEM_DTR_PLL1_USER_CTL_RMSK                              0xffffffff
#define HWIO_MSS_MODEM_DTR_PLL1_USER_CTL_IN          \
        in_dword_masked(HWIO_MSS_MODEM_DTR_PLL1_USER_CTL_ADDR, HWIO_MSS_MODEM_DTR_PLL1_USER_CTL_RMSK)
#define HWIO_MSS_MODEM_DTR_PLL1_USER_CTL_INM(m)      \
        in_dword_masked(HWIO_MSS_MODEM_DTR_PLL1_USER_CTL_ADDR, m)
#define HWIO_MSS_MODEM_DTR_PLL1_USER_CTL_OUT(v)      \
        out_dword(HWIO_MSS_MODEM_DTR_PLL1_USER_CTL_ADDR,v)
#define HWIO_MSS_MODEM_DTR_PLL1_USER_CTL_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_MODEM_DTR_PLL1_USER_CTL_ADDR,m,v,HWIO_MSS_MODEM_DTR_PLL1_USER_CTL_IN)
#define HWIO_MSS_MODEM_DTR_PLL1_USER_CTL_USER_RSVD4_BMSK                   0xfe000000
#define HWIO_MSS_MODEM_DTR_PLL1_USER_CTL_USER_RSVD4_SHFT                         0x19
#define HWIO_MSS_MODEM_DTR_PLL1_USER_CTL_MN_EN_BMSK                         0x1000000
#define HWIO_MSS_MODEM_DTR_PLL1_USER_CTL_MN_EN_SHFT                              0x18
#define HWIO_MSS_MODEM_DTR_PLL1_USER_CTL_USER_RSVD3_BMSK                     0xffc000
#define HWIO_MSS_MODEM_DTR_PLL1_USER_CTL_USER_RSVD3_SHFT                          0xe
#define HWIO_MSS_MODEM_DTR_PLL1_USER_CTL_INCLK_SEL_BMSK                        0x3000
#define HWIO_MSS_MODEM_DTR_PLL1_USER_CTL_INCLK_SEL_SHFT                           0xc
#define HWIO_MSS_MODEM_DTR_PLL1_USER_CTL_USER_RSVD2_BMSK                        0xc00
#define HWIO_MSS_MODEM_DTR_PLL1_USER_CTL_USER_RSVD2_SHFT                          0xa
#define HWIO_MSS_MODEM_DTR_PLL1_USER_CTL_POSTDIV_CTL_BMSK                       0x300
#define HWIO_MSS_MODEM_DTR_PLL1_USER_CTL_POSTDIV_CTL_SHFT                         0x8
#define HWIO_MSS_MODEM_DTR_PLL1_USER_CTL_INV_OUTPUT_BMSK                         0x80
#define HWIO_MSS_MODEM_DTR_PLL1_USER_CTL_INV_OUTPUT_SHFT                          0x7
#define HWIO_MSS_MODEM_DTR_PLL1_USER_CTL_USER_RSVD1_BMSK                         0x60
#define HWIO_MSS_MODEM_DTR_PLL1_USER_CTL_USER_RSVD1_SHFT                          0x5
#define HWIO_MSS_MODEM_DTR_PLL1_USER_CTL_LVTEST_EN_BMSK                          0x10
#define HWIO_MSS_MODEM_DTR_PLL1_USER_CTL_LVTEST_EN_SHFT                           0x4
#define HWIO_MSS_MODEM_DTR_PLL1_USER_CTL_LVEARLY_EN_BMSK                          0x8
#define HWIO_MSS_MODEM_DTR_PLL1_USER_CTL_LVEARLY_EN_SHFT                          0x3
#define HWIO_MSS_MODEM_DTR_PLL1_USER_CTL_LVAUX2_EN_BMSK                           0x4
#define HWIO_MSS_MODEM_DTR_PLL1_USER_CTL_LVAUX2_EN_SHFT                           0x2
#define HWIO_MSS_MODEM_DTR_PLL1_USER_CTL_LVAUX_EN_BMSK                            0x2
#define HWIO_MSS_MODEM_DTR_PLL1_USER_CTL_LVAUX_EN_SHFT                            0x1
#define HWIO_MSS_MODEM_DTR_PLL1_USER_CTL_LVMAIN_EN_BMSK                           0x1
#define HWIO_MSS_MODEM_DTR_PLL1_USER_CTL_LVMAIN_EN_SHFT                           0x0

#define HWIO_MSS_MODEM_DTR_PLL1_CONFIG_CTL_ADDR                            (MODEM_DTR_ANALOG_REG_BASE      + 0x00000028)
#define HWIO_MSS_MODEM_DTR_PLL1_CONFIG_CTL_PHYS                            (MODEM_DTR_ANALOG_REG_BASE_PHYS + 0x00000028)
#define HWIO_MSS_MODEM_DTR_PLL1_CONFIG_CTL_RMSK                            0xffffffff
#define HWIO_MSS_MODEM_DTR_PLL1_CONFIG_CTL_IN          \
        in_dword_masked(HWIO_MSS_MODEM_DTR_PLL1_CONFIG_CTL_ADDR, HWIO_MSS_MODEM_DTR_PLL1_CONFIG_CTL_RMSK)
#define HWIO_MSS_MODEM_DTR_PLL1_CONFIG_CTL_INM(m)      \
        in_dword_masked(HWIO_MSS_MODEM_DTR_PLL1_CONFIG_CTL_ADDR, m)
#define HWIO_MSS_MODEM_DTR_PLL1_CONFIG_CTL_OUT(v)      \
        out_dword(HWIO_MSS_MODEM_DTR_PLL1_CONFIG_CTL_ADDR,v)
#define HWIO_MSS_MODEM_DTR_PLL1_CONFIG_CTL_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_MODEM_DTR_PLL1_CONFIG_CTL_ADDR,m,v,HWIO_MSS_MODEM_DTR_PLL1_CONFIG_CTL_IN)
#define HWIO_MSS_MODEM_DTR_PLL1_CONFIG_CTL_USER_RSVD_BMSK                  0xffffffff
#define HWIO_MSS_MODEM_DTR_PLL1_CONFIG_CTL_USER_RSVD_SHFT                         0x0

#define HWIO_MSS_MODEM_DTR_PLL1_TEST_CTL_ADDR                              (MODEM_DTR_ANALOG_REG_BASE      + 0x00000030)
#define HWIO_MSS_MODEM_DTR_PLL1_TEST_CTL_PHYS                              (MODEM_DTR_ANALOG_REG_BASE_PHYS + 0x00000030)
#define HWIO_MSS_MODEM_DTR_PLL1_TEST_CTL_RMSK                              0xffffffff
#define HWIO_MSS_MODEM_DTR_PLL1_TEST_CTL_IN          \
        in_dword_masked(HWIO_MSS_MODEM_DTR_PLL1_TEST_CTL_ADDR, HWIO_MSS_MODEM_DTR_PLL1_TEST_CTL_RMSK)
#define HWIO_MSS_MODEM_DTR_PLL1_TEST_CTL_INM(m)      \
        in_dword_masked(HWIO_MSS_MODEM_DTR_PLL1_TEST_CTL_ADDR, m)
#define HWIO_MSS_MODEM_DTR_PLL1_TEST_CTL_OUT(v)      \
        out_dword(HWIO_MSS_MODEM_DTR_PLL1_TEST_CTL_ADDR,v)
#define HWIO_MSS_MODEM_DTR_PLL1_TEST_CTL_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_MODEM_DTR_PLL1_TEST_CTL_ADDR,m,v,HWIO_MSS_MODEM_DTR_PLL1_TEST_CTL_IN)
#define HWIO_MSS_MODEM_DTR_PLL1_TEST_CTL_USER_RSVD_BMSK                    0xffffffff
#define HWIO_MSS_MODEM_DTR_PLL1_TEST_CTL_USER_RSVD_SHFT                           0x0

#define HWIO_MSS_MODEM_DTR_PLL1_STATUS_ADDR                                (MODEM_DTR_ANALOG_REG_BASE      + 0x00000038)
#define HWIO_MSS_MODEM_DTR_PLL1_STATUS_PHYS                                (MODEM_DTR_ANALOG_REG_BASE_PHYS + 0x00000038)
#define HWIO_MSS_MODEM_DTR_PLL1_STATUS_RMSK                                    0xffff
#define HWIO_MSS_MODEM_DTR_PLL1_STATUS_IN          \
        in_dword_masked(HWIO_MSS_MODEM_DTR_PLL1_STATUS_ADDR, HWIO_MSS_MODEM_DTR_PLL1_STATUS_RMSK)
#define HWIO_MSS_MODEM_DTR_PLL1_STATUS_INM(m)      \
        in_dword_masked(HWIO_MSS_MODEM_DTR_PLL1_STATUS_ADDR, m)
#define HWIO_MSS_MODEM_DTR_PLL1_STATUS_STATUS_BMSK                             0xffff
#define HWIO_MSS_MODEM_DTR_PLL1_STATUS_STATUS_SHFT                                0x0

#define HWIO_MSS_MODEM_DTR_PLL2_MODE_ADDR                                  (MODEM_DTR_ANALOG_REG_BASE      + 0x00000040)
#define HWIO_MSS_MODEM_DTR_PLL2_MODE_PHYS                                  (MODEM_DTR_ANALOG_REG_BASE_PHYS + 0x00000040)
#define HWIO_MSS_MODEM_DTR_PLL2_MODE_RMSK                                  0xffffffff
#define HWIO_MSS_MODEM_DTR_PLL2_MODE_IN          \
        in_dword_masked(HWIO_MSS_MODEM_DTR_PLL2_MODE_ADDR, HWIO_MSS_MODEM_DTR_PLL2_MODE_RMSK)
#define HWIO_MSS_MODEM_DTR_PLL2_MODE_INM(m)      \
        in_dword_masked(HWIO_MSS_MODEM_DTR_PLL2_MODE_ADDR, m)
#define HWIO_MSS_MODEM_DTR_PLL2_MODE_OUT(v)      \
        out_dword(HWIO_MSS_MODEM_DTR_PLL2_MODE_ADDR,v)
#define HWIO_MSS_MODEM_DTR_PLL2_MODE_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_MODEM_DTR_PLL2_MODE_ADDR,m,v,HWIO_MSS_MODEM_DTR_PLL2_MODE_IN)
#define HWIO_MSS_MODEM_DTR_PLL2_MODE_LOCK_DET_BMSK                         0x80000000
#define HWIO_MSS_MODEM_DTR_PLL2_MODE_LOCK_DET_SHFT                               0x1f
#define HWIO_MSS_MODEM_DTR_PLL2_MODE_ACTIVE_FLAG_BMSK                      0x40000000
#define HWIO_MSS_MODEM_DTR_PLL2_MODE_ACTIVE_FLAG_SHFT                            0x1e
#define HWIO_MSS_MODEM_DTR_PLL2_MODE_LATCH_ACK_BMSK                        0x20000000
#define HWIO_MSS_MODEM_DTR_PLL2_MODE_LATCH_ACK_SHFT                              0x1d
#define HWIO_MSS_MODEM_DTR_PLL2_MODE_USER_RSRVD2_BMSK                      0x1f000000
#define HWIO_MSS_MODEM_DTR_PLL2_MODE_USER_RSRVD2_SHFT                            0x18
#define HWIO_MSS_MODEM_DTR_PLL2_MODE_HW_UPDATE_LOGIC_BYPASS_BMSK             0x800000
#define HWIO_MSS_MODEM_DTR_PLL2_MODE_HW_UPDATE_LOGIC_BYPASS_SHFT                 0x17
#define HWIO_MSS_MODEM_DTR_PLL2_MODE_UPDATE_BMSK                             0x400000
#define HWIO_MSS_MODEM_DTR_PLL2_MODE_UPDATE_SHFT                                 0x16
#define HWIO_MSS_MODEM_DTR_PLL2_MODE_VOTE_FSM_RESET_BMSK                     0x200000
#define HWIO_MSS_MODEM_DTR_PLL2_MODE_VOTE_FSM_RESET_SHFT                         0x15
#define HWIO_MSS_MODEM_DTR_PLL2_MODE_VOTE_FSM_ENA_BMSK                       0x100000
#define HWIO_MSS_MODEM_DTR_PLL2_MODE_VOTE_FSM_ENA_SHFT                           0x14
#define HWIO_MSS_MODEM_DTR_PLL2_MODE_BIAS_COUNT_BMSK                          0xfc000
#define HWIO_MSS_MODEM_DTR_PLL2_MODE_BIAS_COUNT_SHFT                              0xe
#define HWIO_MSS_MODEM_DTR_PLL2_MODE_LOCK_COUNT_BMSK                           0x3f00
#define HWIO_MSS_MODEM_DTR_PLL2_MODE_LOCK_COUNT_SHFT                              0x8
#define HWIO_MSS_MODEM_DTR_PLL2_MODE_USER_RSRVD1_BMSK                            0xc0
#define HWIO_MSS_MODEM_DTR_PLL2_MODE_USER_RSRVD1_SHFT                             0x6
#define HWIO_MSS_MODEM_DTR_PLL2_MODE_REF_XO_SELECT_BMSK                          0x30
#define HWIO_MSS_MODEM_DTR_PLL2_MODE_REF_XO_SELECT_SHFT                           0x4
#define HWIO_MSS_MODEM_DTR_PLL2_MODE_PLLTEST_BMSK                                 0x8
#define HWIO_MSS_MODEM_DTR_PLL2_MODE_PLLTEST_SHFT                                 0x3
#define HWIO_MSS_MODEM_DTR_PLL2_MODE_RESET_N_BMSK                                 0x4
#define HWIO_MSS_MODEM_DTR_PLL2_MODE_RESET_N_SHFT                                 0x2
#define HWIO_MSS_MODEM_DTR_PLL2_MODE_BYPASSNL_BMSK                                0x2
#define HWIO_MSS_MODEM_DTR_PLL2_MODE_BYPASSNL_SHFT                                0x1
#define HWIO_MSS_MODEM_DTR_PLL2_MODE_OUTCTRL_BMSK                                 0x1
#define HWIO_MSS_MODEM_DTR_PLL2_MODE_OUTCTRL_SHFT                                 0x0

#define HWIO_MSS_MODEM_DTR_PLL2_L_VAL_ADDR                                 (MODEM_DTR_ANALOG_REG_BASE      + 0x00000048)
#define HWIO_MSS_MODEM_DTR_PLL2_L_VAL_PHYS                                 (MODEM_DTR_ANALOG_REG_BASE_PHYS + 0x00000048)
#define HWIO_MSS_MODEM_DTR_PLL2_L_VAL_RMSK                                       0xff
#define HWIO_MSS_MODEM_DTR_PLL2_L_VAL_IN          \
        in_dword_masked(HWIO_MSS_MODEM_DTR_PLL2_L_VAL_ADDR, HWIO_MSS_MODEM_DTR_PLL2_L_VAL_RMSK)
#define HWIO_MSS_MODEM_DTR_PLL2_L_VAL_INM(m)      \
        in_dword_masked(HWIO_MSS_MODEM_DTR_PLL2_L_VAL_ADDR, m)
#define HWIO_MSS_MODEM_DTR_PLL2_L_VAL_OUT(v)      \
        out_dword(HWIO_MSS_MODEM_DTR_PLL2_L_VAL_ADDR,v)
#define HWIO_MSS_MODEM_DTR_PLL2_L_VAL_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_MODEM_DTR_PLL2_L_VAL_ADDR,m,v,HWIO_MSS_MODEM_DTR_PLL2_L_VAL_IN)
#define HWIO_MSS_MODEM_DTR_PLL2_L_VAL_L_LVAL_BMSK                                0xff
#define HWIO_MSS_MODEM_DTR_PLL2_L_VAL_L_LVAL_SHFT                                 0x0

#define HWIO_MSS_MODEM_DTR_PLL2_ALPHA_VAL_L_ADDR                           (MODEM_DTR_ANALOG_REG_BASE      + 0x00000050)
#define HWIO_MSS_MODEM_DTR_PLL2_ALPHA_VAL_L_PHYS                           (MODEM_DTR_ANALOG_REG_BASE_PHYS + 0x00000050)
#define HWIO_MSS_MODEM_DTR_PLL2_ALPHA_VAL_L_RMSK                           0xffffffff
#define HWIO_MSS_MODEM_DTR_PLL2_ALPHA_VAL_L_IN          \
        in_dword_masked(HWIO_MSS_MODEM_DTR_PLL2_ALPHA_VAL_L_ADDR, HWIO_MSS_MODEM_DTR_PLL2_ALPHA_VAL_L_RMSK)
#define HWIO_MSS_MODEM_DTR_PLL2_ALPHA_VAL_L_INM(m)      \
        in_dword_masked(HWIO_MSS_MODEM_DTR_PLL2_ALPHA_VAL_L_ADDR, m)
#define HWIO_MSS_MODEM_DTR_PLL2_ALPHA_VAL_L_OUT(v)      \
        out_dword(HWIO_MSS_MODEM_DTR_PLL2_ALPHA_VAL_L_ADDR,v)
#define HWIO_MSS_MODEM_DTR_PLL2_ALPHA_VAL_L_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_MODEM_DTR_PLL2_ALPHA_VAL_L_ADDR,m,v,HWIO_MSS_MODEM_DTR_PLL2_ALPHA_VAL_L_IN)
#define HWIO_MSS_MODEM_DTR_PLL2_ALPHA_VAL_L_ALPHA_VAL_L_BMSK               0xffffffff
#define HWIO_MSS_MODEM_DTR_PLL2_ALPHA_VAL_L_ALPHA_VAL_L_SHFT                      0x0

#define HWIO_MSS_MODEM_DTR_PLL2_ALPHA_VAL_H_ADDR                           (MODEM_DTR_ANALOG_REG_BASE      + 0x00000058)
#define HWIO_MSS_MODEM_DTR_PLL2_ALPHA_VAL_H_PHYS                           (MODEM_DTR_ANALOG_REG_BASE_PHYS + 0x00000058)
#define HWIO_MSS_MODEM_DTR_PLL2_ALPHA_VAL_H_RMSK                                 0xff
#define HWIO_MSS_MODEM_DTR_PLL2_ALPHA_VAL_H_IN          \
        in_dword_masked(HWIO_MSS_MODEM_DTR_PLL2_ALPHA_VAL_H_ADDR, HWIO_MSS_MODEM_DTR_PLL2_ALPHA_VAL_H_RMSK)
#define HWIO_MSS_MODEM_DTR_PLL2_ALPHA_VAL_H_INM(m)      \
        in_dword_masked(HWIO_MSS_MODEM_DTR_PLL2_ALPHA_VAL_H_ADDR, m)
#define HWIO_MSS_MODEM_DTR_PLL2_ALPHA_VAL_H_OUT(v)      \
        out_dword(HWIO_MSS_MODEM_DTR_PLL2_ALPHA_VAL_H_ADDR,v)
#define HWIO_MSS_MODEM_DTR_PLL2_ALPHA_VAL_H_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_MODEM_DTR_PLL2_ALPHA_VAL_H_ADDR,m,v,HWIO_MSS_MODEM_DTR_PLL2_ALPHA_VAL_H_IN)
#define HWIO_MSS_MODEM_DTR_PLL2_ALPHA_VAL_H_PLL2_ALPHA_VAL_H_BMSK                0xff
#define HWIO_MSS_MODEM_DTR_PLL2_ALPHA_VAL_H_PLL2_ALPHA_VAL_H_SHFT                 0x0

#define HWIO_MSS_MODEM_DTR_PLL2_USER_CTL_ADDR                              (MODEM_DTR_ANALOG_REG_BASE      + 0x00000060)
#define HWIO_MSS_MODEM_DTR_PLL2_USER_CTL_PHYS                              (MODEM_DTR_ANALOG_REG_BASE_PHYS + 0x00000060)
#define HWIO_MSS_MODEM_DTR_PLL2_USER_CTL_RMSK                              0xffffffff
#define HWIO_MSS_MODEM_DTR_PLL2_USER_CTL_IN          \
        in_dword_masked(HWIO_MSS_MODEM_DTR_PLL2_USER_CTL_ADDR, HWIO_MSS_MODEM_DTR_PLL2_USER_CTL_RMSK)
#define HWIO_MSS_MODEM_DTR_PLL2_USER_CTL_INM(m)      \
        in_dword_masked(HWIO_MSS_MODEM_DTR_PLL2_USER_CTL_ADDR, m)
#define HWIO_MSS_MODEM_DTR_PLL2_USER_CTL_OUT(v)      \
        out_dword(HWIO_MSS_MODEM_DTR_PLL2_USER_CTL_ADDR,v)
#define HWIO_MSS_MODEM_DTR_PLL2_USER_CTL_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_MODEM_DTR_PLL2_USER_CTL_ADDR,m,v,HWIO_MSS_MODEM_DTR_PLL2_USER_CTL_IN)
#define HWIO_MSS_MODEM_DTR_PLL2_USER_CTL_USER_RSVD4_BMSK                   0xfe000000
#define HWIO_MSS_MODEM_DTR_PLL2_USER_CTL_USER_RSVD4_SHFT                         0x19
#define HWIO_MSS_MODEM_DTR_PLL2_USER_CTL_MN_EN_BMSK                         0x1000000
#define HWIO_MSS_MODEM_DTR_PLL2_USER_CTL_MN_EN_SHFT                              0x18
#define HWIO_MSS_MODEM_DTR_PLL2_USER_CTL_USER_RSVD3_BMSK                     0xffc000
#define HWIO_MSS_MODEM_DTR_PLL2_USER_CTL_USER_RSVD3_SHFT                          0xe
#define HWIO_MSS_MODEM_DTR_PLL2_USER_CTL_INCLK_SEL_BMSK                        0x3000
#define HWIO_MSS_MODEM_DTR_PLL2_USER_CTL_INCLK_SEL_SHFT                           0xc
#define HWIO_MSS_MODEM_DTR_PLL2_USER_CTL_USER_RSVD2_BMSK                        0xc00
#define HWIO_MSS_MODEM_DTR_PLL2_USER_CTL_USER_RSVD2_SHFT                          0xa
#define HWIO_MSS_MODEM_DTR_PLL2_USER_CTL_POSTDIV_CTL_BMSK                       0x300
#define HWIO_MSS_MODEM_DTR_PLL2_USER_CTL_POSTDIV_CTL_SHFT                         0x8
#define HWIO_MSS_MODEM_DTR_PLL2_USER_CTL_INV_OUTPUT_BMSK                         0x80
#define HWIO_MSS_MODEM_DTR_PLL2_USER_CTL_INV_OUTPUT_SHFT                          0x7
#define HWIO_MSS_MODEM_DTR_PLL2_USER_CTL_USER_RSVD1_BMSK                         0x60
#define HWIO_MSS_MODEM_DTR_PLL2_USER_CTL_USER_RSVD1_SHFT                          0x5
#define HWIO_MSS_MODEM_DTR_PLL2_USER_CTL_LVTEST_EN_BMSK                          0x10
#define HWIO_MSS_MODEM_DTR_PLL2_USER_CTL_LVTEST_EN_SHFT                           0x4
#define HWIO_MSS_MODEM_DTR_PLL2_USER_CTL_LVEARLY_EN_BMSK                          0x8
#define HWIO_MSS_MODEM_DTR_PLL2_USER_CTL_LVEARLY_EN_SHFT                          0x3
#define HWIO_MSS_MODEM_DTR_PLL2_USER_CTL_LVAUX2_EN_BMSK                           0x4
#define HWIO_MSS_MODEM_DTR_PLL2_USER_CTL_LVAUX2_EN_SHFT                           0x2
#define HWIO_MSS_MODEM_DTR_PLL2_USER_CTL_LVAUX_EN_BMSK                            0x2
#define HWIO_MSS_MODEM_DTR_PLL2_USER_CTL_LVAUX_EN_SHFT                            0x1
#define HWIO_MSS_MODEM_DTR_PLL2_USER_CTL_LVMAIN_EN_BMSK                           0x1
#define HWIO_MSS_MODEM_DTR_PLL2_USER_CTL_LVMAIN_EN_SHFT                           0x0

#define HWIO_MSS_MODEM_DTR_PLL2_CONFIG_CTL_ADDR                            (MODEM_DTR_ANALOG_REG_BASE      + 0x00000068)
#define HWIO_MSS_MODEM_DTR_PLL2_CONFIG_CTL_PHYS                            (MODEM_DTR_ANALOG_REG_BASE_PHYS + 0x00000068)
#define HWIO_MSS_MODEM_DTR_PLL2_CONFIG_CTL_RMSK                            0xffffffff
#define HWIO_MSS_MODEM_DTR_PLL2_CONFIG_CTL_IN          \
        in_dword_masked(HWIO_MSS_MODEM_DTR_PLL2_CONFIG_CTL_ADDR, HWIO_MSS_MODEM_DTR_PLL2_CONFIG_CTL_RMSK)
#define HWIO_MSS_MODEM_DTR_PLL2_CONFIG_CTL_INM(m)      \
        in_dword_masked(HWIO_MSS_MODEM_DTR_PLL2_CONFIG_CTL_ADDR, m)
#define HWIO_MSS_MODEM_DTR_PLL2_CONFIG_CTL_OUT(v)      \
        out_dword(HWIO_MSS_MODEM_DTR_PLL2_CONFIG_CTL_ADDR,v)
#define HWIO_MSS_MODEM_DTR_PLL2_CONFIG_CTL_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_MODEM_DTR_PLL2_CONFIG_CTL_ADDR,m,v,HWIO_MSS_MODEM_DTR_PLL2_CONFIG_CTL_IN)
#define HWIO_MSS_MODEM_DTR_PLL2_CONFIG_CTL_USER_RSVD_BMSK                  0xffffffff
#define HWIO_MSS_MODEM_DTR_PLL2_CONFIG_CTL_USER_RSVD_SHFT                         0x0

#define HWIO_MSS_MODEM_DTR_PLL2_TEST_CTL_ADDR                              (MODEM_DTR_ANALOG_REG_BASE      + 0x00000070)
#define HWIO_MSS_MODEM_DTR_PLL2_TEST_CTL_PHYS                              (MODEM_DTR_ANALOG_REG_BASE_PHYS + 0x00000070)
#define HWIO_MSS_MODEM_DTR_PLL2_TEST_CTL_RMSK                              0xffffffff
#define HWIO_MSS_MODEM_DTR_PLL2_TEST_CTL_IN          \
        in_dword_masked(HWIO_MSS_MODEM_DTR_PLL2_TEST_CTL_ADDR, HWIO_MSS_MODEM_DTR_PLL2_TEST_CTL_RMSK)
#define HWIO_MSS_MODEM_DTR_PLL2_TEST_CTL_INM(m)      \
        in_dword_masked(HWIO_MSS_MODEM_DTR_PLL2_TEST_CTL_ADDR, m)
#define HWIO_MSS_MODEM_DTR_PLL2_TEST_CTL_OUT(v)      \
        out_dword(HWIO_MSS_MODEM_DTR_PLL2_TEST_CTL_ADDR,v)
#define HWIO_MSS_MODEM_DTR_PLL2_TEST_CTL_OUTM(m,v) \
        out_dword_masked_ns(HWIO_MSS_MODEM_DTR_PLL2_TEST_CTL_ADDR,m,v,HWIO_MSS_MODEM_DTR_PLL2_TEST_CTL_IN)
#define HWIO_MSS_MODEM_DTR_PLL2_TEST_CTL_USER_RSVD_BMSK                    0xffffffff
#define HWIO_MSS_MODEM_DTR_PLL2_TEST_CTL_USER_RSVD_SHFT                           0x0

#define HWIO_MSS_MODEM_DTR_PLL2_STATUS_ADDR                                (MODEM_DTR_ANALOG_REG_BASE      + 0x00000078)
#define HWIO_MSS_MODEM_DTR_PLL2_STATUS_PHYS                                (MODEM_DTR_ANALOG_REG_BASE_PHYS + 0x00000078)
#define HWIO_MSS_MODEM_DTR_PLL2_STATUS_RMSK                                    0xffff
#define HWIO_MSS_MODEM_DTR_PLL2_STATUS_IN          \
        in_dword_masked(HWIO_MSS_MODEM_DTR_PLL2_STATUS_ADDR, HWIO_MSS_MODEM_DTR_PLL2_STATUS_RMSK)
#define HWIO_MSS_MODEM_DTR_PLL2_STATUS_INM(m)      \
        in_dword_masked(HWIO_MSS_MODEM_DTR_PLL2_STATUS_ADDR, m)
#define HWIO_MSS_MODEM_DTR_PLL2_STATUS_USER_RSVD_BMSK                          0xffff
#define HWIO_MSS_MODEM_DTR_PLL2_STATUS_USER_RSVD_SHFT                             0x0

#endif //HWIO_MSS_MODEM_DTR_PLL1_MODE_ADDR: INTERIM HWIO definitions for FLAT file name changes

#ifndef SECURITY_CONTROL_BASE
#define SECURITY_CONTROL_BASE                                       0xe0fb8000
#endif

#ifndef SECURITY_CONTROL_CORE_REG_BASE
#define SECURITY_CONTROL_CORE_REG_BASE                                                                             (SECURITY_CONTROL_BASE      + 0x00000000)
#endif

#ifndef SECURITY_CONTROL_CORE_REG_BASE_PHYS
#define SECURITY_CONTROL_CORE_REG_BASE_PHYS                                                                        (SECURITY_CONTROL_BASE_PHYS + 0x00000000)
#endif

#ifndef HWIO_QFPROM_CORR_CALIB_ROW0_MSB_ADDR
#define HWIO_QFPROM_CORR_CALIB_ROW0_MSB_ADDR                                                                       (SECURITY_CONTROL_CORE_REG_BASE      + 0x000041c4)
#define HWIO_QFPROM_CORR_CALIB_ROW0_MSB_PHYS                                                                       (SECURITY_CONTROL_CORE_REG_BASE_PHYS + 0x000041c4)
#define HWIO_QFPROM_CORR_CALIB_ROW0_MSB_RMSK                                                                       0xffffffff
#define HWIO_QFPROM_CORR_CALIB_ROW0_MSB_IN          \
        in_dword_masked(HWIO_QFPROM_CORR_CALIB_ROW0_MSB_ADDR, HWIO_QFPROM_CORR_CALIB_ROW0_MSB_RMSK)
#define HWIO_QFPROM_CORR_CALIB_ROW0_MSB_INM(m)      \
        in_dword_masked(HWIO_QFPROM_CORR_CALIB_ROW0_MSB_ADDR, m)
#define HWIO_QFPROM_CORR_CALIB_ROW0_MSB_MODEM_TXDAC1_CALIB_BMSK                                                    0xff000000
#define HWIO_QFPROM_CORR_CALIB_ROW0_MSB_MODEM_TXDAC1_CALIB_SHFT                                                          0x18
#define HWIO_QFPROM_CORR_CALIB_ROW0_MSB_MODEM_TXDAC0_CALIB_BMSK                                                      0xff0000
#define HWIO_QFPROM_CORR_CALIB_ROW0_MSB_MODEM_TXDAC0_CALIB_SHFT                                                          0x10
#define HWIO_QFPROM_CORR_CALIB_ROW0_MSB_GNSS_ADC_CH0_VCM_BMSK                                                          0xc000
#define HWIO_QFPROM_CORR_CALIB_ROW0_MSB_GNSS_ADC_CH0_VCM_SHFT                                                             0xe
#define HWIO_QFPROM_CORR_CALIB_ROW0_MSB_GNSS_ADC_CH0_LDO_BMSK                                                          0x3000
#define HWIO_QFPROM_CORR_CALIB_ROW0_MSB_GNSS_ADC_CH0_LDO_SHFT                                                             0xc
#define HWIO_QFPROM_CORR_CALIB_ROW0_MSB_GNSS_ADC_CH0_VREF_BMSK                                                          0xc00
#define HWIO_QFPROM_CORR_CALIB_ROW0_MSB_GNSS_ADC_CH0_VREF_SHFT                                                            0xa
#define HWIO_QFPROM_CORR_CALIB_ROW0_MSB_SPARE0_BMSK                                                                     0x380
#define HWIO_QFPROM_CORR_CALIB_ROW0_MSB_SPARE0_SHFT                                                                       0x7
#define HWIO_QFPROM_CORR_CALIB_ROW0_MSB_MODEM_TXDAC1_CALIB_AVG_ERROR_BMSK                                                0x40
#define HWIO_QFPROM_CORR_CALIB_ROW0_MSB_MODEM_TXDAC1_CALIB_AVG_ERROR_SHFT                                                 0x6
#define HWIO_QFPROM_CORR_CALIB_ROW0_MSB_MODEM_TXDAC0_CALIB_AVG_ERROR_BMSK                                                0x20
#define HWIO_QFPROM_CORR_CALIB_ROW0_MSB_MODEM_TXDAC0_CALIB_AVG_ERROR_SHFT                                                 0x5
#define HWIO_QFPROM_CORR_CALIB_ROW0_MSB_MODEM_TXDAC_0_1_FUSEFLAG_BMSK                                                    0x10
#define HWIO_QFPROM_CORR_CALIB_ROW0_MSB_MODEM_TXDAC_0_1_FUSEFLAG_SHFT                                                     0x4
#define HWIO_QFPROM_CORR_CALIB_ROW0_MSB_MODEM_TXDAC1_CALIB_OVERFLOW_BMSK                                                  0x8
#define HWIO_QFPROM_CORR_CALIB_ROW0_MSB_MODEM_TXDAC1_CALIB_OVERFLOW_SHFT                                                  0x3
#define HWIO_QFPROM_CORR_CALIB_ROW0_MSB_MODEM_TXDAC0_CALIB_OVERFLOW_BMSK                                                  0x4
#define HWIO_QFPROM_CORR_CALIB_ROW0_MSB_MODEM_TXDAC0_CALIB_OVERFLOW_SHFT                                                  0x2
#define HWIO_QFPROM_CORR_CALIB_ROW0_MSB_SAR_LDO_ERR4_BMSK                                                                 0x3
#define HWIO_QFPROM_CORR_CALIB_ROW0_MSB_SAR_LDO_ERR4_SHFT                                                                 0x0


#endif

/*----------------------------------------------------------------------------
  Interim Definition:
* MODULE: TCSR_TCSR_REGS

*--------------------------------------------------------------------------*/
#ifndef HWIO_TCSR_SOC_HW_VERSION_ADDR 
#define TCSR_TCSR_REGS_REG_BASE (CORE_TOP_CSR_BASE + 0x00020000)
#define TCSR_TCSR_REGS_REG_BASE_PHYS (CORE_TOP_CSR_BASE_PHYS + 0x00020000)
#define HWIO_TCSR_SOC_HW_VERSION_ADDR (TCSR_TCSR_REGS_REG_BASE + 0x00008000)
#define HWIO_TCSR_SOC_HW_VERSION_PHYS (TCSR_TCSR_REGS_REG_BASE_PHYS + 0x00008000)
#define HWIO_TCSR_SOC_HW_VERSION_RMSK 0xffffffff
#define HWIO_TCSR_SOC_HW_VERSION_IN          \
        in_dword_masked(HWIO_TCSR_SOC_HW_VERSION_ADDR, HWIO_TCSR_SOC_HW_VERSION_RMSK)
#define HWIO_TCSR_SOC_HW_VERSION_INM(m)      \
        in_dword_masked(HWIO_TCSR_SOC_HW_VERSION_ADDR, m)
#endif /* !HWIO_TCSR_SOC_HW_VERSION_ADDR */




#ifndef SECURITY_CONTROL_V2_BASE
#define SECURITY_CONTROL_V2_BASE                                    0xe04a0000
#define SECURITY_CONTROL_V2_BASE_SIZE                               0x00010000
#define SECURITY_CONTROL_V2_BASE_PHYS                               0x000a0000
#endif

#ifndef SECURITY_CONTROL_CORE_V2_REG_BASE
#define SECURITY_CONTROL_CORE_V2_REG_BASE                                                                             (SECURITY_CONTROL_V2_BASE      + 0x00000000)
#endif

#ifndef SECURITY_CONTROL_CORE_V2_REG_BASE_PHYS
#define SECURITY_CONTROL_CORE_V2_REG_BASE_PHYS                                                                        (SECURITY_CONTROL_V2_BASE_PHYS + 0x00000000)
#endif

#ifndef HWIO_QFPROM_RAW_CALIB_ROW4_LSB_V2_ADDR
#define HWIO_QFPROM_RAW_CALIB_ROW4_LSB_V2_ADDR                                                    (SECURITY_CONTROL_CORE_V2_REG_BASE      + 0x000000f0)
#define HWIO_QFPROM_RAW_CALIB_ROW4_LSB_V2_PHYS                                                    (SECURITY_CONTROL_CORE_V2_REG_BASE_PHYS + 0x000000f0)
#define HWIO_QFPROM_RAW_CALIB_ROW4_LSB_V2_RMSK                                                    0xffffffff
#define HWIO_QFPROM_RAW_CALIB_ROW4_LSB_V2_IN          \
        in_dword_masked(HWIO_QFPROM_RAW_CALIB_ROW4_LSB_V2_ADDR, HWIO_QFPROM_RAW_CALIB_ROW4_LSB_V2_RMSK)

#define HWIO_QFPROM_RAW_CALIB_ROW4_LSB_V2_INM(m)      \
        in_dword_masked(HWIO_QFPROM_RAW_CALIB_ROW4_LSB_V2_ADDR, m)

#define HWIO_QFPROM_RAW_CALIB_ROW4_LSB_V2_OUT(v)      \
        out_dword(HWIO_QFPROM_RAW_CALIB_ROW4_LSB_V2_ADDR,v)

#define HWIO_QFPROM_RAW_CALIB_ROW4_LSB_V2_OUTM(m,v) \
        out_dword_masked_ns(HWIO_QFPROM_RAW_CALIB_ROW4_LSB_V2_ADDR,m,v,HWIO_QFPROM_RAW_CALIB_ROW4_LSB_V2_IN)

#define HWIO_QFPROM_RAW_CALIB_ROW4_LSB_V2_PH_B1M2_BMSK                                            0xc0000000
#define HWIO_QFPROM_RAW_CALIB_ROW4_LSB_V2_PH_B1M2_SHFT                                                  0x1e
#define HWIO_QFPROM_RAW_CALIB_ROW4_LSB_V2_PH_B1M1_BMSK                                            0x38000000
#define HWIO_QFPROM_RAW_CALIB_ROW4_LSB_V2_PH_B1M1_SHFT                                                  0x1b
#define HWIO_QFPROM_RAW_CALIB_ROW4_LSB_V2_PH_B1M0_BMSK                                             0x7000000
#define HWIO_QFPROM_RAW_CALIB_ROW4_LSB_V2_PH_B1M0_SHFT                                                  0x18
#define HWIO_QFPROM_RAW_CALIB_ROW4_LSB_V2_Q6SS0_LDO_VREF_TRIM_BMSK                                  0xf80000
#define HWIO_QFPROM_RAW_CALIB_ROW4_LSB_V2_Q6SS0_LDO_VREF_TRIM_SHFT                                      0x13
#define HWIO_QFPROM_RAW_CALIB_ROW4_LSB_V2_CLK_LDO_ERR0_BMSK                                          0x60000
#define HWIO_QFPROM_RAW_CALIB_ROW4_LSB_V2_CLK_LDO_ERR0_SHFT                                             0x11
#define HWIO_QFPROM_RAW_CALIB_ROW4_LSB_V2_VREF_ERR_B0_BMSK                                           0x18000
#define HWIO_QFPROM_RAW_CALIB_ROW4_LSB_V2_VREF_ERR_B0_SHFT                                               0xf
#define HWIO_QFPROM_RAW_CALIB_ROW4_LSB_V2_PH_B0M3_BMSK                                                0x7000
#define HWIO_QFPROM_RAW_CALIB_ROW4_LSB_V2_PH_B0M3_SHFT                                                   0xc
#define HWIO_QFPROM_RAW_CALIB_ROW4_LSB_V2_PH_B0M2_BMSK                                                 0xe00
#define HWIO_QFPROM_RAW_CALIB_ROW4_LSB_V2_PH_B0M2_SHFT                                                   0x9
#define HWIO_QFPROM_RAW_CALIB_ROW4_LSB_V2_PH_B0M1_BMSK                                                 0x1c0
#define HWIO_QFPROM_RAW_CALIB_ROW4_LSB_V2_PH_B0M1_SHFT                                                   0x6
#define HWIO_QFPROM_RAW_CALIB_ROW4_LSB_V2_PH_B0M0_BMSK                                                  0x38
#define HWIO_QFPROM_RAW_CALIB_ROW4_LSB_V2_PH_B0M0_SHFT                                                   0x3
#define HWIO_QFPROM_RAW_CALIB_ROW4_LSB_V2_G_B0_BMSK                                                      0x7
#define HWIO_QFPROM_RAW_CALIB_ROW4_LSB_V2_G_B0_SHFT                                                      0x0
#endif

#ifndef HWIO_QFPROM_RAW_CALIB_ROW4_MSB_V2_ADDR
#define HWIO_QFPROM_RAW_CALIB_ROW4_MSB_V2_ADDR                                                    (SECURITY_CONTROL_CORE_V2_REG_BASE      + 0x000000f4)
#define HWIO_QFPROM_RAW_CALIB_ROW4_MSB_V2_PHYS                                                    (SECURITY_CONTROL_CORE_V2_REG_BASE_PHYS + 0x000000f4)
#define HWIO_QFPROM_RAW_CALIB_ROW4_MSB_V2_RMSK                                                    0xffffffff
#define HWIO_QFPROM_RAW_CALIB_ROW4_MSB_V2_IN          \
        in_dword_masked(HWIO_QFPROM_RAW_CALIB_ROW4_MSB_V2_ADDR, HWIO_QFPROM_RAW_CALIB_ROW4_MSB_V2_RMSK)

#define HWIO_QFPROM_RAW_CALIB_ROW4_MSB_V2_INM(m)      \
        in_dword_masked(HWIO_QFPROM_RAW_CALIB_ROW4_MSB_V2_ADDR, m)

#define HWIO_QFPROM_RAW_CALIB_ROW4_MSB_V2_OUT(v)      \
        out_dword(HWIO_QFPROM_RAW_CALIB_ROW4_MSB_V2_ADDR,v)

#define HWIO_QFPROM_RAW_CALIB_ROW4_MSB_V2_OUTM(m,v) \
        out_dword_masked_ns(HWIO_QFPROM_RAW_CALIB_ROW4_MSB_V2_ADDR,m,v,HWIO_QFPROM_RAW_CALIB_ROW4_MSB_V2_IN)

#define HWIO_QFPROM_RAW_CALIB_ROW4_MSB_V2_FEC_BMSK                                                0xff000000
#define HWIO_QFPROM_RAW_CALIB_ROW4_MSB_V2_FEC_SHFT                                                      0x18
#define HWIO_QFPROM_RAW_CALIB_ROW4_MSB_V2_PH_B2M3_BMSK                                              0xc00000
#define HWIO_QFPROM_RAW_CALIB_ROW4_MSB_V2_PH_B2M3_SHFT                                                  0x16
#define HWIO_QFPROM_RAW_CALIB_ROW4_MSB_V2_PH_B2M2_BMSK                                              0x380000
#define HWIO_QFPROM_RAW_CALIB_ROW4_MSB_V2_PH_B2M2_SHFT                                                  0x13
#define HWIO_QFPROM_RAW_CALIB_ROW4_MSB_V2_PH_B2M1_BMSK                                               0x70000
#define HWIO_QFPROM_RAW_CALIB_ROW4_MSB_V2_PH_B2M1_SHFT                                                  0x10
#define HWIO_QFPROM_RAW_CALIB_ROW4_MSB_V2_PH_B2M0_BMSK                                                0xe000
#define HWIO_QFPROM_RAW_CALIB_ROW4_MSB_V2_PH_B2M0_SHFT                                                   0xd
#define HWIO_QFPROM_RAW_CALIB_ROW4_MSB_V2_G_B2_BMSK                                                   0x1c00
#define HWIO_QFPROM_RAW_CALIB_ROW4_MSB_V2_G_B2_SHFT                                                      0xa
#define HWIO_QFPROM_RAW_CALIB_ROW4_MSB_V2_SAR_LDO_ERR1_BMSK                                            0x300
#define HWIO_QFPROM_RAW_CALIB_ROW4_MSB_V2_SAR_LDO_ERR1_SHFT                                              0x8
#define HWIO_QFPROM_RAW_CALIB_ROW4_MSB_V2_CLK_LDO_ERR1_BMSK                                             0xc0
#define HWIO_QFPROM_RAW_CALIB_ROW4_MSB_V2_CLK_LDO_ERR1_SHFT                                              0x6
#define HWIO_QFPROM_RAW_CALIB_ROW4_MSB_V2_VREF_ERR_B1_BMSK                                              0x30
#define HWIO_QFPROM_RAW_CALIB_ROW4_MSB_V2_VREF_ERR_B1_SHFT                                               0x4
#define HWIO_QFPROM_RAW_CALIB_ROW4_MSB_V2_PH_B1M3_BMSK                                                   0xe
#define HWIO_QFPROM_RAW_CALIB_ROW4_MSB_V2_PH_B1M3_SHFT                                                   0x1
#define HWIO_QFPROM_RAW_CALIB_ROW4_MSB_V2_PH_B1M2_BMSK                                                   0x1
#define HWIO_QFPROM_RAW_CALIB_ROW4_MSB_V2_PH_B1M2_SHFT                                                   0x0
#endif

#ifndef HWIO_QFPROM_RAW_CALIB_ROW5_LSB_V2_ADDR
#define HWIO_QFPROM_RAW_CALIB_ROW5_LSB_V2_ADDR                                                    (SECURITY_CONTROL_CORE_V2_REG_BASE      + 0x000000f8)
#define HWIO_QFPROM_RAW_CALIB_ROW5_LSB_V2_PHYS                                                    (SECURITY_CONTROL_CORE_V2_REG_BASE_PHYS + 0x000000f8)
#define HWIO_QFPROM_RAW_CALIB_ROW5_LSB_V2_RMSK                                                    0xffffffff
#define HWIO_QFPROM_RAW_CALIB_ROW5_LSB_V2_IN          \
        in_dword_masked(HWIO_QFPROM_RAW_CALIB_ROW5_LSB_V2_ADDR, HWIO_QFPROM_RAW_CALIB_ROW5_LSB_V2_RMSK)

#define HWIO_QFPROM_RAW_CALIB_ROW5_LSB_V2_INM(m)      \
        in_dword_masked(HWIO_QFPROM_RAW_CALIB_ROW5_LSB_V2_ADDR, m)

#define HWIO_QFPROM_RAW_CALIB_ROW5_LSB_V2_OUT(v)      \
        out_dword(HWIO_QFPROM_RAW_CALIB_ROW5_LSB_V2_ADDR,v)

#define HWIO_QFPROM_RAW_CALIB_ROW5_LSB_V2_OUTM(m,v) \
        out_dword_masked_ns(HWIO_QFPROM_RAW_CALIB_ROW5_LSB_V2_ADDR,m,v,HWIO_QFPROM_RAW_CALIB_ROW5_LSB_V2_IN)

#define HWIO_QFPROM_RAW_CALIB_ROW5_LSB_V2_PH_B4M0_BMSK                                            0x80000000
#define HWIO_QFPROM_RAW_CALIB_ROW5_LSB_V2_PH_B4M0_SHFT                                                  0x1f
#define HWIO_QFPROM_RAW_CALIB_ROW5_LSB_V2_G_B4_BMSK                                               0x70000000
#define HWIO_QFPROM_RAW_CALIB_ROW5_LSB_V2_G_B4_SHFT                                                     0x1c
#define HWIO_QFPROM_RAW_CALIB_ROW5_LSB_V2_SAR_LDO_ERR3_BMSK                                        0xc000000
#define HWIO_QFPROM_RAW_CALIB_ROW5_LSB_V2_SAR_LDO_ERR3_SHFT                                             0x1a
#define HWIO_QFPROM_RAW_CALIB_ROW5_LSB_V2_CLK_LDO_ERR3_BMSK                                        0x3000000
#define HWIO_QFPROM_RAW_CALIB_ROW5_LSB_V2_CLK_LDO_ERR3_SHFT                                             0x18
#define HWIO_QFPROM_RAW_CALIB_ROW5_LSB_V2_VREF_ERR_B3_BMSK                                          0xc00000
#define HWIO_QFPROM_RAW_CALIB_ROW5_LSB_V2_VREF_ERR_B3_SHFT                                              0x16
#define HWIO_QFPROM_RAW_CALIB_ROW5_LSB_V2_PH_B3M3_BMSK                                              0x380000
#define HWIO_QFPROM_RAW_CALIB_ROW5_LSB_V2_PH_B3M3_SHFT                                                  0x13
#define HWIO_QFPROM_RAW_CALIB_ROW5_LSB_V2_PH_B3M2_BMSK                                               0x70000
#define HWIO_QFPROM_RAW_CALIB_ROW5_LSB_V2_PH_B3M2_SHFT                                                  0x10
#define HWIO_QFPROM_RAW_CALIB_ROW5_LSB_V2_PH_B3M1_BMSK                                                0xe000
#define HWIO_QFPROM_RAW_CALIB_ROW5_LSB_V2_PH_B3M1_SHFT                                                   0xd
#define HWIO_QFPROM_RAW_CALIB_ROW5_LSB_V2_PH_B3M0_BMSK                                                0x1c00
#define HWIO_QFPROM_RAW_CALIB_ROW5_LSB_V2_PH_B3M0_SHFT                                                   0xa
#define HWIO_QFPROM_RAW_CALIB_ROW5_LSB_V2_G_B3_BMSK                                                    0x380
#define HWIO_QFPROM_RAW_CALIB_ROW5_LSB_V2_G_B3_SHFT                                                      0x7
#define HWIO_QFPROM_RAW_CALIB_ROW5_LSB_V2_SAR_LDO_ERR2_BMSK                                             0x60
#define HWIO_QFPROM_RAW_CALIB_ROW5_LSB_V2_SAR_LDO_ERR2_SHFT                                              0x5
#define HWIO_QFPROM_RAW_CALIB_ROW5_LSB_V2_CLK_LDO_ERR2_BMSK                                             0x18
#define HWIO_QFPROM_RAW_CALIB_ROW5_LSB_V2_CLK_LDO_ERR2_SHFT                                              0x3
#define HWIO_QFPROM_RAW_CALIB_ROW5_LSB_V2_VREF_ERR_B2_BMSK                                               0x6
#define HWIO_QFPROM_RAW_CALIB_ROW5_LSB_V2_VREF_ERR_B2_SHFT                                               0x1
#define HWIO_QFPROM_RAW_CALIB_ROW5_LSB_V2_PH_B2M3_BMSK                                                   0x1
#define HWIO_QFPROM_RAW_CALIB_ROW5_LSB_V2_PH_B2M3_SHFT                                                   0x0
#endif

#ifndef HWIO_QFPROM_RAW_CALIB_ROW5_MSB_V2_ADDR
#define HWIO_QFPROM_RAW_CALIB_ROW5_MSB_V2_ADDR                                                    (SECURITY_CONTROL_CORE_V2_REG_BASE      + 0x000000fc)
#define HWIO_QFPROM_RAW_CALIB_ROW5_MSB_V2_PHYS                                                    (SECURITY_CONTROL_CORE_V2_REG_BASE_PHYS + 0x000000fc)
#define HWIO_QFPROM_RAW_CALIB_ROW5_MSB_V2_RMSK                                                    0xffffffff
#define HWIO_QFPROM_RAW_CALIB_ROW5_MSB_V2_IN          \
        in_dword_masked(HWIO_QFPROM_RAW_CALIB_ROW5_MSB_V2_ADDR, HWIO_QFPROM_RAW_CALIB_ROW5_MSB_V2_RMSK)

#define HWIO_QFPROM_RAW_CALIB_ROW5_MSB_V2_INM(m)      \
        in_dword_masked(HWIO_QFPROM_RAW_CALIB_ROW5_MSB_V2_ADDR, m)

#define HWIO_QFPROM_RAW_CALIB_ROW5_MSB_V2_OUT(v)      \
        out_dword(HWIO_QFPROM_RAW_CALIB_ROW5_MSB_V2_ADDR,v)

#define HWIO_QFPROM_RAW_CALIB_ROW5_MSB_V2_OUTM(m,v) \
        out_dword_masked_ns(HWIO_QFPROM_RAW_CALIB_ROW5_MSB_V2_ADDR,m,v,HWIO_QFPROM_RAW_CALIB_ROW5_MSB_V2_IN)

#define HWIO_QFPROM_RAW_CALIB_ROW5_MSB_V2_FEC_BMSK                                                0xff000000
#define HWIO_QFPROM_RAW_CALIB_ROW5_MSB_V2_FEC_SHFT                                                      0x18
#define HWIO_QFPROM_RAW_CALIB_ROW5_MSB_V2_PH_B5M1_BMSK                                              0x800000
#define HWIO_QFPROM_RAW_CALIB_ROW5_MSB_V2_PH_B5M1_SHFT                                                  0x17
#define HWIO_QFPROM_RAW_CALIB_ROW5_MSB_V2_PH_B5M0_BMSK                                              0x700000
#define HWIO_QFPROM_RAW_CALIB_ROW5_MSB_V2_PH_B5M0_SHFT                                                  0x14
#define HWIO_QFPROM_RAW_CALIB_ROW5_MSB_V2_G_B5_BMSK                                                  0xe0000
#define HWIO_QFPROM_RAW_CALIB_ROW5_MSB_V2_G_B5_SHFT                                                     0x11
#define HWIO_QFPROM_RAW_CALIB_ROW5_MSB_V2_SAR_LDO_ERR4_BMSK                                          0x18000
#define HWIO_QFPROM_RAW_CALIB_ROW5_MSB_V2_SAR_LDO_ERR4_SHFT                                              0xf
#define HWIO_QFPROM_RAW_CALIB_ROW5_MSB_V2_CLK_LDO_ERR4_BMSK                                           0x6000
#define HWIO_QFPROM_RAW_CALIB_ROW5_MSB_V2_CLK_LDO_ERR4_SHFT                                              0xd
#define HWIO_QFPROM_RAW_CALIB_ROW5_MSB_V2_VREF_ERR_B4_BMSK                                            0x1800
#define HWIO_QFPROM_RAW_CALIB_ROW5_MSB_V2_VREF_ERR_B4_SHFT                                               0xb
#define HWIO_QFPROM_RAW_CALIB_ROW5_MSB_V2_PH_B4M3_BMSK                                                 0x700
#define HWIO_QFPROM_RAW_CALIB_ROW5_MSB_V2_PH_B4M3_SHFT                                                   0x8
#define HWIO_QFPROM_RAW_CALIB_ROW5_MSB_V2_PH_B4M2_BMSK                                                  0xe0
#define HWIO_QFPROM_RAW_CALIB_ROW5_MSB_V2_PH_B4M2_SHFT                                                   0x5
#define HWIO_QFPROM_RAW_CALIB_ROW5_MSB_V2_PH_B4M1_BMSK                                                  0x1c
#define HWIO_QFPROM_RAW_CALIB_ROW5_MSB_V2_PH_B4M1_SHFT                                                   0x2
#define HWIO_QFPROM_RAW_CALIB_ROW5_MSB_V2_PH_B4M0_BMSK                                                   0x3
#define HWIO_QFPROM_RAW_CALIB_ROW5_MSB_V2_PH_B4M0_SHFT                                                   0x0
#endif

#ifndef HWIO_QFPROM_RAW_CALIB_ROW6_LSB_V2_ADDR
#define HWIO_QFPROM_RAW_CALIB_ROW6_LSB_V2_ADDR                                                    (SECURITY_CONTROL_CORE_V2_REG_BASE      + 0x00000100)
#define HWIO_QFPROM_RAW_CALIB_ROW6_LSB_V2_PHYS                                                    (SECURITY_CONTROL_CORE_V2_REG_BASE_PHYS + 0x00000100)
#define HWIO_QFPROM_RAW_CALIB_ROW6_LSB_V2_RMSK                                                    0xffffffff
#define HWIO_QFPROM_RAW_CALIB_ROW6_LSB_V2_IN          \
        in_dword_masked(HWIO_QFPROM_RAW_CALIB_ROW6_LSB_V2_ADDR, HWIO_QFPROM_RAW_CALIB_ROW6_LSB_V2_RMSK)

#define HWIO_QFPROM_RAW_CALIB_ROW6_LSB_V2_INM(m)      \
        in_dword_masked(HWIO_QFPROM_RAW_CALIB_ROW6_LSB_V2_ADDR, m)

#define HWIO_QFPROM_RAW_CALIB_ROW6_LSB_V2_OUT(v)      \
        out_dword(HWIO_QFPROM_RAW_CALIB_ROW6_LSB_V2_ADDR,v)

#define HWIO_QFPROM_RAW_CALIB_ROW6_LSB_V2_OUTM(m,v) \
        out_dword_masked_ns(HWIO_QFPROM_RAW_CALIB_ROW6_LSB_V2_ADDR,m,v,HWIO_QFPROM_RAW_CALIB_ROW6_LSB_V2_IN)

#define HWIO_QFPROM_RAW_CALIB_ROW6_LSB_V2_CLK_LDO_ERR6_BMSK                                       0x80000000
#define HWIO_QFPROM_RAW_CALIB_ROW6_LSB_V2_CLK_LDO_ERR6_SHFT                                             0x1f
#define HWIO_QFPROM_RAW_CALIB_ROW6_LSB_V2_VREF_ERR_B6_BMSK                                        0x60000000
#define HWIO_QFPROM_RAW_CALIB_ROW6_LSB_V2_VREF_ERR_B6_SHFT                                              0x1d
#define HWIO_QFPROM_RAW_CALIB_ROW6_LSB_V2_PH_B6M3_BMSK                                            0x1c000000
#define HWIO_QFPROM_RAW_CALIB_ROW6_LSB_V2_PH_B6M3_SHFT                                                  0x1a
#define HWIO_QFPROM_RAW_CALIB_ROW6_LSB_V2_PH_B6M2_BMSK                                             0x3800000
#define HWIO_QFPROM_RAW_CALIB_ROW6_LSB_V2_PH_B6M2_SHFT                                                  0x17
#define HWIO_QFPROM_RAW_CALIB_ROW6_LSB_V2_PH_B6M1_BMSK                                              0x700000
#define HWIO_QFPROM_RAW_CALIB_ROW6_LSB_V2_PH_B6M1_SHFT                                                  0x14
#define HWIO_QFPROM_RAW_CALIB_ROW6_LSB_V2_PH_B6M0_BMSK                                               0xe0000
#define HWIO_QFPROM_RAW_CALIB_ROW6_LSB_V2_PH_B6M0_SHFT                                                  0x11
#define HWIO_QFPROM_RAW_CALIB_ROW6_LSB_V2_G_B6_BMSK                                                  0x1c000
#define HWIO_QFPROM_RAW_CALIB_ROW6_LSB_V2_G_B6_SHFT                                                      0xe
#define HWIO_QFPROM_RAW_CALIB_ROW6_LSB_V2_SAR_LDO_ERR5_BMSK                                           0x3000
#define HWIO_QFPROM_RAW_CALIB_ROW6_LSB_V2_SAR_LDO_ERR5_SHFT                                              0xc
#define HWIO_QFPROM_RAW_CALIB_ROW6_LSB_V2_CLK_LDO_ERR5_BMSK                                            0xc00
#define HWIO_QFPROM_RAW_CALIB_ROW6_LSB_V2_CLK_LDO_ERR5_SHFT                                              0xa
#define HWIO_QFPROM_RAW_CALIB_ROW6_LSB_V2_VREF_ERR_B5_BMSK                                             0x300
#define HWIO_QFPROM_RAW_CALIB_ROW6_LSB_V2_VREF_ERR_B5_SHFT                                               0x8
#define HWIO_QFPROM_RAW_CALIB_ROW6_LSB_V2_PH_B5M3_BMSK                                                  0xe0
#define HWIO_QFPROM_RAW_CALIB_ROW6_LSB_V2_PH_B5M3_SHFT                                                   0x5
#define HWIO_QFPROM_RAW_CALIB_ROW6_LSB_V2_PH_B5M2_BMSK                                                  0x1c
#define HWIO_QFPROM_RAW_CALIB_ROW6_LSB_V2_PH_B5M2_SHFT                                                   0x2
#define HWIO_QFPROM_RAW_CALIB_ROW6_LSB_V2_PH_B5M1_BMSK                                                   0x3
#define HWIO_QFPROM_RAW_CALIB_ROW6_LSB_V2_PH_B5M1_SHFT                                                   0x0
#endif

#ifndef HWIO_QFPROM_RAW_CALIB_ROW4_LSB_V2_ADDR
#define HWIO_QFPROM_RAW_CALIB_ROW4_LSB_V2_ADDR                                                    (SECURITY_CONTROL_CORE_V2_REG_BASE      + 0x000000f0)
#define HWIO_QFPROM_RAW_CALIB_ROW4_LSB_V2_PHYS                                                    (SECURITY_CONTROL_CORE_V2_REG_BASE_PHYS + 0x000000f0)
#define HWIO_QFPROM_RAW_CALIB_ROW4_LSB_V2_RMSK                                                    0xffffffff
#define HWIO_QFPROM_RAW_CALIB_ROW4_LSB_V2_IN          \
        in_dword_masked(HWIO_QFPROM_RAW_CALIB_ROW4_LSB_V2_ADDR, HWIO_QFPROM_RAW_CALIB_ROW4_LSB_V2_RMSK)

#define HWIO_QFPROM_RAW_CALIB_ROW4_LSB_V2_INM(m)      \
        in_dword_masked(HWIO_QFPROM_RAW_CALIB_ROW4_LSB_V2_ADDR, m)

#define HWIO_QFPROM_RAW_CALIB_ROW4_LSB_V2_OUT(v)      \
        out_dword(HWIO_QFPROM_RAW_CALIB_ROW4_LSB_V2_ADDR,v)

#define HWIO_QFPROM_RAW_CALIB_ROW4_LSB_V2_OUTM(m,v) \
        out_dword_masked_ns(HWIO_QFPROM_RAW_CALIB_ROW4_LSB_V2_ADDR,m,v,HWIO_QFPROM_RAW_CALIB_ROW4_LSB_V2_IN)

#define HWIO_QFPROM_RAW_CALIB_ROW4_LSB_V2_PH_B1M2_BMSK                                            0xc0000000
#define HWIO_QFPROM_RAW_CALIB_ROW4_LSB_V2_PH_B1M2_SHFT                                                  0x1e
#define HWIO_QFPROM_RAW_CALIB_ROW4_LSB_V2_PH_B1M1_BMSK                                            0x38000000
#define HWIO_QFPROM_RAW_CALIB_ROW4_LSB_V2_PH_B1M1_SHFT                                                  0x1b
#define HWIO_QFPROM_RAW_CALIB_ROW4_LSB_V2_PH_B1M0_BMSK                                             0x7000000
#define HWIO_QFPROM_RAW_CALIB_ROW4_LSB_V2_PH_B1M0_SHFT                                                  0x18
#define HWIO_QFPROM_RAW_CALIB_ROW4_LSB_V2_Q6SS0_LDO_VREF_TRIM_BMSK                                  0xf80000
#define HWIO_QFPROM_RAW_CALIB_ROW4_LSB_V2_Q6SS0_LDO_VREF_TRIM_SHFT                                      0x13
#define HWIO_QFPROM_RAW_CALIB_ROW4_LSB_V2_CLK_LDO_ERR0_BMSK                                          0x60000
#define HWIO_QFPROM_RAW_CALIB_ROW4_LSB_V2_CLK_LDO_ERR0_SHFT                                             0x11
#define HWIO_QFPROM_RAW_CALIB_ROW4_LSB_V2_VREF_ERR_B0_BMSK                                           0x18000
#define HWIO_QFPROM_RAW_CALIB_ROW4_LSB_V2_VREF_ERR_B0_SHFT                                               0xf
#define HWIO_QFPROM_RAW_CALIB_ROW4_LSB_V2_PH_B0M3_BMSK                                                0x7000
#define HWIO_QFPROM_RAW_CALIB_ROW4_LSB_V2_PH_B0M3_SHFT                                                   0xc
#define HWIO_QFPROM_RAW_CALIB_ROW4_LSB_V2_PH_B0M2_BMSK                                                 0xe00
#define HWIO_QFPROM_RAW_CALIB_ROW4_LSB_V2_PH_B0M2_SHFT                                                   0x9
#define HWIO_QFPROM_RAW_CALIB_ROW4_LSB_V2_PH_B0M1_BMSK                                                 0x1c0
#define HWIO_QFPROM_RAW_CALIB_ROW4_LSB_V2_PH_B0M1_SHFT                                                   0x6
#define HWIO_QFPROM_RAW_CALIB_ROW4_LSB_V2_PH_B0M0_BMSK                                                  0x38
#define HWIO_QFPROM_RAW_CALIB_ROW4_LSB_V2_PH_B0M0_SHFT                                                   0x3
#define HWIO_QFPROM_RAW_CALIB_ROW4_LSB_V2_G_B0_BMSK                                                      0x7
#define HWIO_QFPROM_RAW_CALIB_ROW4_LSB_V2_G_B0_SHFT                                                      0x0
#endif

#ifndef HWIO_QFPROM_RAW_CALIB_ROW4_MSB_V2_ADDR
#define HWIO_QFPROM_RAW_CALIB_ROW4_MSB_V2_ADDR                                                    (SECURITY_CONTROL_CORE_V2_REG_BASE      + 0x000000f4)
#define HWIO_QFPROM_RAW_CALIB_ROW4_MSB_V2_PHYS                                                    (SECURITY_CONTROL_CORE_V2_REG_BASE_PHYS + 0x000000f4)
#define HWIO_QFPROM_RAW_CALIB_ROW4_MSB_V2_RMSK                                                    0xffffffff
#define HWIO_QFPROM_RAW_CALIB_ROW4_MSB_V2_IN          \
        in_dword_masked(HWIO_QFPROM_RAW_CALIB_ROW4_MSB_V2_ADDR, HWIO_QFPROM_RAW_CALIB_ROW4_MSB_V2_RMSK)

#define HWIO_QFPROM_RAW_CALIB_ROW4_MSB_V2_INM(m)      \
        in_dword_masked(HWIO_QFPROM_RAW_CALIB_ROW4_MSB_V2_ADDR, m)

#define HWIO_QFPROM_RAW_CALIB_ROW4_MSB_V2_OUT(v)      \
        out_dword(HWIO_QFPROM_RAW_CALIB_ROW4_MSB_V2_ADDR,v)

#define HWIO_QFPROM_RAW_CALIB_ROW4_MSB_V2_OUTM(m,v) \
        out_dword_masked_ns(HWIO_QFPROM_RAW_CALIB_ROW4_MSB_V2_ADDR,m,v,HWIO_QFPROM_RAW_CALIB_ROW4_MSB_V2_IN)

#define HWIO_QFPROM_RAW_CALIB_ROW4_MSB_V2_FEC_BMSK                                                0xff000000
#define HWIO_QFPROM_RAW_CALIB_ROW4_MSB_V2_FEC_SHFT                                                      0x18
#define HWIO_QFPROM_RAW_CALIB_ROW4_MSB_V2_PH_B2M3_BMSK                                              0xc00000
#define HWIO_QFPROM_RAW_CALIB_ROW4_MSB_V2_PH_B2M3_SHFT                                                  0x16
#define HWIO_QFPROM_RAW_CALIB_ROW4_MSB_V2_PH_B2M2_BMSK                                              0x380000
#define HWIO_QFPROM_RAW_CALIB_ROW4_MSB_V2_PH_B2M2_SHFT                                                  0x13
#define HWIO_QFPROM_RAW_CALIB_ROW4_MSB_V2_PH_B2M1_BMSK                                               0x70000
#define HWIO_QFPROM_RAW_CALIB_ROW4_MSB_V2_PH_B2M1_SHFT                                                  0x10
#define HWIO_QFPROM_RAW_CALIB_ROW4_MSB_V2_PH_B2M0_BMSK                                                0xe000
#define HWIO_QFPROM_RAW_CALIB_ROW4_MSB_V2_PH_B2M0_SHFT                                                   0xd
#define HWIO_QFPROM_RAW_CALIB_ROW4_MSB_V2_G_B2_BMSK                                                   0x1c00
#define HWIO_QFPROM_RAW_CALIB_ROW4_MSB_V2_G_B2_SHFT                                                      0xa
#define HWIO_QFPROM_RAW_CALIB_ROW4_MSB_V2_SAR_LDO_ERR1_BMSK                                            0x300
#define HWIO_QFPROM_RAW_CALIB_ROW4_MSB_V2_SAR_LDO_ERR1_SHFT                                              0x8
#define HWIO_QFPROM_RAW_CALIB_ROW4_MSB_V2_CLK_LDO_ERR1_BMSK                                             0xc0
#define HWIO_QFPROM_RAW_CALIB_ROW4_MSB_V2_CLK_LDO_ERR1_SHFT                                              0x6
#define HWIO_QFPROM_RAW_CALIB_ROW4_MSB_V2_VREF_ERR_B1_BMSK                                              0x30
#define HWIO_QFPROM_RAW_CALIB_ROW4_MSB_V2_VREF_ERR_B1_SHFT                                               0x4
#define HWIO_QFPROM_RAW_CALIB_ROW4_MSB_V2_PH_B1M3_BMSK                                                   0xe
#define HWIO_QFPROM_RAW_CALIB_ROW4_MSB_V2_PH_B1M3_SHFT                                                   0x1
#define HWIO_QFPROM_RAW_CALIB_ROW4_MSB_V2_PH_B1M2_BMSK                                                   0x1
#define HWIO_QFPROM_RAW_CALIB_ROW4_MSB_V2_PH_B1M2_SHFT                                                   0x0
#endif

#ifndef HWIO_QFPROM_RAW_CALIB_ROW5_LSB_V2_ADDR
#define HWIO_QFPROM_RAW_CALIB_ROW5_LSB_V2_ADDR                                                    (SECURITY_CONTROL_CORE_V2_REG_BASE      + 0x000000f8)
#define HWIO_QFPROM_RAW_CALIB_ROW5_LSB_V2_PHYS                                                    (SECURITY_CONTROL_CORE_V2_REG_BASE_PHYS + 0x000000f8)
#define HWIO_QFPROM_RAW_CALIB_ROW5_LSB_V2_RMSK                                                    0xffffffff
#define HWIO_QFPROM_RAW_CALIB_ROW5_LSB_V2_IN          \
        in_dword_masked(HWIO_QFPROM_RAW_CALIB_ROW5_LSB_V2_ADDR, HWIO_QFPROM_RAW_CALIB_ROW5_LSB_V2_RMSK)

#define HWIO_QFPROM_RAW_CALIB_ROW5_LSB_V2_INM(m)      \
        in_dword_masked(HWIO_QFPROM_RAW_CALIB_ROW5_LSB_V2_ADDR, m)

#define HWIO_QFPROM_RAW_CALIB_ROW5_LSB_V2_OUT(v)      \
        out_dword(HWIO_QFPROM_RAW_CALIB_ROW5_LSB_V2_ADDR,v)

#define HWIO_QFPROM_RAW_CALIB_ROW5_LSB_V2_OUTM(m,v) \
        out_dword_masked_ns(HWIO_QFPROM_RAW_CALIB_ROW5_LSB_V2_ADDR,m,v,HWIO_QFPROM_RAW_CALIB_ROW5_LSB_V2_IN)

#define HWIO_QFPROM_RAW_CALIB_ROW5_LSB_V2_PH_B4M0_BMSK                                            0x80000000
#define HWIO_QFPROM_RAW_CALIB_ROW5_LSB_V2_PH_B4M0_SHFT                                                  0x1f
#define HWIO_QFPROM_RAW_CALIB_ROW5_LSB_V2_G_B4_BMSK                                               0x70000000
#define HWIO_QFPROM_RAW_CALIB_ROW5_LSB_V2_G_B4_SHFT                                                     0x1c
#define HWIO_QFPROM_RAW_CALIB_ROW5_LSB_V2_SAR_LDO_ERR3_BMSK                                        0xc000000
#define HWIO_QFPROM_RAW_CALIB_ROW5_LSB_V2_SAR_LDO_ERR3_SHFT                                             0x1a
#define HWIO_QFPROM_RAW_CALIB_ROW5_LSB_V2_CLK_LDO_ERR3_BMSK                                        0x3000000
#define HWIO_QFPROM_RAW_CALIB_ROW5_LSB_V2_CLK_LDO_ERR3_SHFT                                             0x18
#define HWIO_QFPROM_RAW_CALIB_ROW5_LSB_V2_VREF_ERR_B3_BMSK                                          0xc00000
#define HWIO_QFPROM_RAW_CALIB_ROW5_LSB_V2_VREF_ERR_B3_SHFT                                              0x16
#define HWIO_QFPROM_RAW_CALIB_ROW5_LSB_V2_PH_B3M3_BMSK                                              0x380000
#define HWIO_QFPROM_RAW_CALIB_ROW5_LSB_V2_PH_B3M3_SHFT                                                  0x13
#define HWIO_QFPROM_RAW_CALIB_ROW5_LSB_V2_PH_B3M2_BMSK                                               0x70000
#define HWIO_QFPROM_RAW_CALIB_ROW5_LSB_V2_PH_B3M2_SHFT                                                  0x10
#define HWIO_QFPROM_RAW_CALIB_ROW5_LSB_V2_PH_B3M1_BMSK                                                0xe000
#define HWIO_QFPROM_RAW_CALIB_ROW5_LSB_V2_PH_B3M1_SHFT                                                   0xd
#define HWIO_QFPROM_RAW_CALIB_ROW5_LSB_V2_PH_B3M0_BMSK                                                0x1c00
#define HWIO_QFPROM_RAW_CALIB_ROW5_LSB_V2_PH_B3M0_SHFT                                                   0xa
#define HWIO_QFPROM_RAW_CALIB_ROW5_LSB_V2_G_B3_BMSK                                                    0x380
#define HWIO_QFPROM_RAW_CALIB_ROW5_LSB_V2_G_B3_SHFT                                                      0x7
#define HWIO_QFPROM_RAW_CALIB_ROW5_LSB_V2_SAR_LDO_ERR2_BMSK                                             0x60
#define HWIO_QFPROM_RAW_CALIB_ROW5_LSB_V2_SAR_LDO_ERR2_SHFT                                              0x5
#define HWIO_QFPROM_RAW_CALIB_ROW5_LSB_V2_CLK_LDO_ERR2_BMSK                                             0x18
#define HWIO_QFPROM_RAW_CALIB_ROW5_LSB_V2_CLK_LDO_ERR2_SHFT                                              0x3
#define HWIO_QFPROM_RAW_CALIB_ROW5_LSB_V2_VREF_ERR_B2_BMSK                                               0x6
#define HWIO_QFPROM_RAW_CALIB_ROW5_LSB_V2_VREF_ERR_B2_SHFT                                               0x1
#define HWIO_QFPROM_RAW_CALIB_ROW5_LSB_V2_PH_B2M3_BMSK                                                   0x1
#define HWIO_QFPROM_RAW_CALIB_ROW5_LSB_V2_PH_B2M3_SHFT                                                   0x0
#endif

#ifndef HWIO_QFPROM_RAW_CALIB_ROW5_MSB_V2_ADDR
#define HWIO_QFPROM_RAW_CALIB_ROW5_MSB_V2_ADDR                                                    (SECURITY_CONTROL_CORE_V2_REG_BASE      + 0x000000fc)
#define HWIO_QFPROM_RAW_CALIB_ROW5_MSB_V2_PHYS                                                    (SECURITY_CONTROL_CORE_V2_REG_BASE_PHYS + 0x000000fc)
#define HWIO_QFPROM_RAW_CALIB_ROW5_MSB_V2_RMSK                                                    0xffffffff
#define HWIO_QFPROM_RAW_CALIB_ROW5_MSB_V2_IN          \
        in_dword_masked(HWIO_QFPROM_RAW_CALIB_ROW5_MSB_V2_ADDR, HWIO_QFPROM_RAW_CALIB_ROW5_MSB_V2_RMSK)

#define HWIO_QFPROM_RAW_CALIB_ROW5_MSB_V2_INM(m)      \
        in_dword_masked(HWIO_QFPROM_RAW_CALIB_ROW5_MSB_V2_ADDR, m)

#define HWIO_QFPROM_RAW_CALIB_ROW5_MSB_V2_OUT(v)      \
        out_dword(HWIO_QFPROM_RAW_CALIB_ROW5_MSB_V2_ADDR,v)

#define HWIO_QFPROM_RAW_CALIB_ROW5_MSB_V2_OUTM(m,v) \
        out_dword_masked_ns(HWIO_QFPROM_RAW_CALIB_ROW5_MSB_V2_ADDR,m,v,HWIO_QFPROM_RAW_CALIB_ROW5_MSB_V2_IN)

#define HWIO_QFPROM_RAW_CALIB_ROW5_MSB_V2_FEC_BMSK                                                0xff000000
#define HWIO_QFPROM_RAW_CALIB_ROW5_MSB_V2_FEC_SHFT                                                      0x18
#define HWIO_QFPROM_RAW_CALIB_ROW5_MSB_V2_PH_B5M1_BMSK                                              0x800000
#define HWIO_QFPROM_RAW_CALIB_ROW5_MSB_V2_PH_B5M1_SHFT                                                  0x17
#define HWIO_QFPROM_RAW_CALIB_ROW5_MSB_V2_PH_B5M0_BMSK                                              0x700000
#define HWIO_QFPROM_RAW_CALIB_ROW5_MSB_V2_PH_B5M0_SHFT                                                  0x14
#define HWIO_QFPROM_RAW_CALIB_ROW5_MSB_V2_G_B5_BMSK                                                  0xe0000
#define HWIO_QFPROM_RAW_CALIB_ROW5_MSB_V2_G_B5_SHFT                                                     0x11
#define HWIO_QFPROM_RAW_CALIB_ROW5_MSB_V2_SAR_LDO_ERR4_BMSK                                          0x18000
#define HWIO_QFPROM_RAW_CALIB_ROW5_MSB_V2_SAR_LDO_ERR4_SHFT                                              0xf
#define HWIO_QFPROM_RAW_CALIB_ROW5_MSB_V2_CLK_LDO_ERR4_BMSK                                           0x6000
#define HWIO_QFPROM_RAW_CALIB_ROW5_MSB_V2_CLK_LDO_ERR4_SHFT                                              0xd
#define HWIO_QFPROM_RAW_CALIB_ROW5_MSB_V2_VREF_ERR_B4_BMSK                                            0x1800
#define HWIO_QFPROM_RAW_CALIB_ROW5_MSB_V2_VREF_ERR_B4_SHFT                                               0xb
#define HWIO_QFPROM_RAW_CALIB_ROW5_MSB_V2_PH_B4M3_BMSK                                                 0x700
#define HWIO_QFPROM_RAW_CALIB_ROW5_MSB_V2_PH_B4M3_SHFT                                                   0x8
#define HWIO_QFPROM_RAW_CALIB_ROW5_MSB_V2_PH_B4M2_BMSK                                                  0xe0
#define HWIO_QFPROM_RAW_CALIB_ROW5_MSB_V2_PH_B4M2_SHFT                                                   0x5
#define HWIO_QFPROM_RAW_CALIB_ROW5_MSB_V2_PH_B4M1_BMSK                                                  0x1c
#define HWIO_QFPROM_RAW_CALIB_ROW5_MSB_V2_PH_B4M1_SHFT                                                   0x2
#define HWIO_QFPROM_RAW_CALIB_ROW5_MSB_V2_PH_B4M0_BMSK                                                   0x3
#define HWIO_QFPROM_RAW_CALIB_ROW5_MSB_V2_PH_B4M0_SHFT                                                   0x0
#endif

#ifndef HWIO_QFPROM_RAW_CALIB_ROW6_LSB_V2_ADDR
#define HWIO_QFPROM_RAW_CALIB_ROW6_LSB_V2_ADDR                                                    (SECURITY_CONTROL_CORE_V2_REG_BASE      + 0x00000100)
#define HWIO_QFPROM_RAW_CALIB_ROW6_LSB_V2_PHYS                                                    (SECURITY_CONTROL_CORE_V2_REG_BASE_PHYS + 0x00000100)
#define HWIO_QFPROM_RAW_CALIB_ROW6_LSB_V2_RMSK                                                    0xffffffff
#define HWIO_QFPROM_RAW_CALIB_ROW6_LSB_V2_IN          \
        in_dword_masked(HWIO_QFPROM_RAW_CALIB_ROW6_LSB_V2_ADDR, HWIO_QFPROM_RAW_CALIB_ROW6_LSB_V2_RMSK)

#define HWIO_QFPROM_RAW_CALIB_ROW6_LSB_V2_INM(m)      \
        in_dword_masked(HWIO_QFPROM_RAW_CALIB_ROW6_LSB_V2_ADDR, m)

#define HWIO_QFPROM_RAW_CALIB_ROW6_LSB_V2_OUT(v)      \
        out_dword(HWIO_QFPROM_RAW_CALIB_ROW6_LSB_V2_ADDR,v)

#define HWIO_QFPROM_RAW_CALIB_ROW6_LSB_V2_OUTM(m,v) \
        out_dword_masked_ns(HWIO_QFPROM_RAW_CALIB_ROW6_LSB_V2_ADDR,m,v,HWIO_QFPROM_RAW_CALIB_ROW6_LSB_V2_IN)

#define HWIO_QFPROM_RAW_CALIB_ROW6_LSB_V2_CLK_LDO_ERR6_BMSK                                       0x80000000
#define HWIO_QFPROM_RAW_CALIB_ROW6_LSB_V2_CLK_LDO_ERR6_SHFT                                             0x1f
#define HWIO_QFPROM_RAW_CALIB_ROW6_LSB_V2_VREF_ERR_B6_BMSK                                        0x60000000
#define HWIO_QFPROM_RAW_CALIB_ROW6_LSB_V2_VREF_ERR_B6_SHFT                                              0x1d
#define HWIO_QFPROM_RAW_CALIB_ROW6_LSB_V2_PH_B6M3_BMSK                                            0x1c000000
#define HWIO_QFPROM_RAW_CALIB_ROW6_LSB_V2_PH_B6M3_SHFT                                                  0x1a
#define HWIO_QFPROM_RAW_CALIB_ROW6_LSB_V2_PH_B6M2_BMSK                                             0x3800000
#define HWIO_QFPROM_RAW_CALIB_ROW6_LSB_V2_PH_B6M2_SHFT                                                  0x17
#define HWIO_QFPROM_RAW_CALIB_ROW6_LSB_V2_PH_B6M1_BMSK                                              0x700000
#define HWIO_QFPROM_RAW_CALIB_ROW6_LSB_V2_PH_B6M1_SHFT                                                  0x14
#define HWIO_QFPROM_RAW_CALIB_ROW6_LSB_V2_PH_B6M0_BMSK                                               0xe0000
#define HWIO_QFPROM_RAW_CALIB_ROW6_LSB_V2_PH_B6M0_SHFT                                                  0x11
#define HWIO_QFPROM_RAW_CALIB_ROW6_LSB_V2_G_B6_BMSK                                                  0x1c000
#define HWIO_QFPROM_RAW_CALIB_ROW6_LSB_V2_G_B6_SHFT                                                      0xe
#define HWIO_QFPROM_RAW_CALIB_ROW6_LSB_V2_SAR_LDO_ERR5_BMSK                                           0x3000
#define HWIO_QFPROM_RAW_CALIB_ROW6_LSB_V2_SAR_LDO_ERR5_SHFT                                              0xc
#define HWIO_QFPROM_RAW_CALIB_ROW6_LSB_V2_CLK_LDO_ERR5_BMSK                                            0xc00
#define HWIO_QFPROM_RAW_CALIB_ROW6_LSB_V2_CLK_LDO_ERR5_SHFT                                              0xa
#define HWIO_QFPROM_RAW_CALIB_ROW6_LSB_V2_VREF_ERR_B5_BMSK                                             0x300
#define HWIO_QFPROM_RAW_CALIB_ROW6_LSB_V2_VREF_ERR_B5_SHFT                                               0x8
#define HWIO_QFPROM_RAW_CALIB_ROW6_LSB_V2_PH_B5M3_BMSK                                                  0xe0
#define HWIO_QFPROM_RAW_CALIB_ROW6_LSB_V2_PH_B5M3_SHFT                                                   0x5
#define HWIO_QFPROM_RAW_CALIB_ROW6_LSB_V2_PH_B5M2_BMSK                                                  0x1c
#define HWIO_QFPROM_RAW_CALIB_ROW6_LSB_V2_PH_B5M2_SHFT                                                   0x2
#define HWIO_QFPROM_RAW_CALIB_ROW6_LSB_V2_PH_B5M1_BMSK                                                   0x3
#define HWIO_QFPROM_RAW_CALIB_ROW6_LSB_V2_PH_B5M1_SHFT                                                   0x0
#endif

#ifndef HWIO_QFPROM_CORR_PTE2_V2_ADDR
#define HWIO_QFPROM_CORR_PTE2_V2_ADDR                                                             (SECURITY_CONTROL_CORE_V2_REG_BASE      + 0x0000400c)
#define HWIO_QFPROM_CORR_PTE2_V2_PHYS                                                             (SECURITY_CONTROL_CORE_V2_REG_BASE_PHYS + 0x0000400c)
#define HWIO_QFPROM_CORR_PTE2_V2_RMSK                                                             0xffffffff
#define HWIO_QFPROM_CORR_PTE2_V2_IN          \
        in_dword_masked(HWIO_QFPROM_CORR_PTE2_V2_ADDR, HWIO_QFPROM_CORR_PTE2_V2_RMSK)

#define HWIO_QFPROM_CORR_PTE2_V2_INM(m)      \
        in_dword_masked(HWIO_QFPROM_CORR_PTE2_V2_ADDR, m)

#define HWIO_QFPROM_CORR_PTE2_V2_PTE_IDDQ_SPARE_BMSK                                              0xfc000000
#define HWIO_QFPROM_CORR_PTE2_V2_PTE_IDDQ_SPARE_SHFT                                                    0x1a
#define HWIO_QFPROM_CORR_PTE2_V2_IDDQ_MSS_OFF_BMSK                                                 0x3f80000
#define HWIO_QFPROM_CORR_PTE2_V2_IDDQ_MSS_OFF_SHFT                                                      0x13
#define HWIO_QFPROM_CORR_PTE2_V2_IDDQ_MSS_ON_BMSK                                                    0x7f800
#define HWIO_QFPROM_CORR_PTE2_V2_IDDQ_MSS_ON_SHFT                                                        0xb
#define HWIO_QFPROM_CORR_PTE2_V2_FOUNDRY_ID_BMSK                                                       0x700
#define HWIO_QFPROM_CORR_PTE2_V2_FOUNDRY_ID_SHFT                                                         0x8
#define HWIO_QFPROM_CORR_PTE2_V2_LOGIC_RETENTION_BMSK                                                   0xe0
#define HWIO_QFPROM_CORR_PTE2_V2_LOGIC_RETENTION_SHFT                                                    0x5
#define HWIO_QFPROM_CORR_PTE2_V2_SPEED_BIN_BMSK                                                         0x1c
#define HWIO_QFPROM_CORR_PTE2_V2_SPEED_BIN_SHFT                                                          0x2
#define HWIO_QFPROM_CORR_PTE2_V2_MX_RET_BIN_BMSK                                                         0x3
#define HWIO_QFPROM_CORR_PTE2_V2_MX_RET_BIN_SHFT                                                         0x0
#endif

#ifndef HWIO_QFPROM_CORR_CALIB_ROW6_MSB_V2_ADDR
#define HWIO_QFPROM_CORR_CALIB_ROW6_MSB_V2_ADDR                                                   (SECURITY_CONTROL_CORE_V2_REG_BASE      + 0x00004104)
#define HWIO_QFPROM_CORR_CALIB_ROW6_MSB_V2_PHYS                                                   (SECURITY_CONTROL_CORE_V2_REG_BASE_PHYS + 0x00004104)
#define HWIO_QFPROM_CORR_CALIB_ROW6_MSB_V2_RMSK                                                   0xffffffff
#define HWIO_QFPROM_CORR_CALIB_ROW6_MSB_V2_IN          \
        in_dword_masked(HWIO_QFPROM_CORR_CALIB_ROW6_MSB_V2_ADDR, HWIO_QFPROM_CORR_CALIB_ROW6_MSB_V2_RMSK)

#define HWIO_QFPROM_CORR_CALIB_ROW6_MSB_V2_INM(m)      \
        in_dword_masked(HWIO_QFPROM_CORR_CALIB_ROW6_MSB_V2_ADDR, m)

#define HWIO_QFPROM_CORR_CALIB_ROW6_MSB_V2_FEC_BMSK                                               0xff000000
#define HWIO_QFPROM_CORR_CALIB_ROW6_MSB_V2_FEC_SHFT                                                     0x18
#define HWIO_QFPROM_CORR_CALIB_ROW6_MSB_V2_MODEM_TXDAC1_CALIB_BMSK                                  0xff0000
#define HWIO_QFPROM_CORR_CALIB_ROW6_MSB_V2_MODEM_TXDAC1_CALIB_BMSK                                  0xff0000
#define HWIO_QFPROM_CORR_CALIB_ROW6_MSB_V2_MODEM_TXDAC1_CALIB_SHFT                                      0x10
#define HWIO_QFPROM_CORR_CALIB_ROW6_MSB_V2_MODEM_TXDAC0_CALIB_BMSK                                    0xff00
#define HWIO_QFPROM_CORR_CALIB_ROW6_MSB_V2_MODEM_TXDAC0_CALIB_BMSK                                    0xff00
#define HWIO_QFPROM_CORR_CALIB_ROW6_MSB_V2_MODEM_TXDAC0_CALIB_SHFT                                       0x8
#define HWIO_QFPROM_CORR_CALIB_ROW6_MSB_V2_MODEM_TXDAC1_CALIB_AVG_ERROR_BMSK                            0x80
#define HWIO_QFPROM_CORR_CALIB_ROW6_MSB_V2_MODEM_TXDAC1_CALIB_AVG_ERROR_BMSK                            0x80
#define HWIO_QFPROM_CORR_CALIB_ROW6_MSB_V2_MODEM_TXDAC1_CALIB_AVG_ERROR_SHFT                             0x7
#define HWIO_QFPROM_CORR_CALIB_ROW6_MSB_V2_MODEM_TXDAC0_CALIB_AVG_ERROR_BMSK                            0x40
#define HWIO_QFPROM_CORR_CALIB_ROW6_MSB_V2_MODEM_TXDAC0_CALIB_AVG_ERROR_BMSK                            0x40
#define HWIO_QFPROM_CORR_CALIB_ROW6_MSB_V2_MODEM_TXDAC0_CALIB_AVG_ERROR_SHFT                             0x6
#define HWIO_QFPROM_CORR_CALIB_ROW6_MSB_V2_MODEM_TXDAC_0_1_FUSEFLAG_BMSK                                0x20
#define HWIO_QFPROM_CORR_CALIB_ROW6_MSB_V2_MODEM_TXDAC_0_1_FUSEFLAG_BMSK                                0x20
#define HWIO_QFPROM_CORR_CALIB_ROW6_MSB_V2_MODEM_TXDAC_0_1_FUSEFLAG_SHFT                                 0x5
#define HWIO_QFPROM_CORR_CALIB_ROW6_MSB_V2_MODEM_TXDAC1_CALIB_OVERFLOW_BMSK                             0x10
#define HWIO_QFPROM_CORR_CALIB_ROW6_MSB_V2_MODEM_TXDAC1_CALIB_OVERFLOW_BMSK                             0x10
#define HWIO_QFPROM_CORR_CALIB_ROW6_MSB_V2_MODEM_TXDAC1_CALIB_OVERFLOW_SHFT                              0x4
#define HWIO_QFPROM_CORR_CALIB_ROW6_MSB_V2_MODEM_TXDAC0_CALIB_OVERFLOW_BMSK                              0x8
#define HWIO_QFPROM_CORR_CALIB_ROW6_MSB_V2_MODEM_TXDAC0_CALIB_OVERFLOW_BMSK                              0x8
#define HWIO_QFPROM_CORR_CALIB_ROW6_MSB_V2_MODEM_TXDAC0_CALIB_OVERFLOW_SHFT                              0x3
#define HWIO_QFPROM_CORR_CALIB_ROW6_MSB_V2_SAR_LDO_ERR6_BMSK                                             0x6
#define HWIO_QFPROM_CORR_CALIB_ROW6_MSB_V2_SAR_LDO_ERR6_SHFT                                             0x1
#define HWIO_QFPROM_CORR_CALIB_ROW6_MSB_V2_CLK_LDO_ERR6_BMSK                                             0x1
#define HWIO_QFPROM_CORR_CALIB_ROW6_MSB_V2_CLK_LDO_ERR6_SHFT                                             0x0
#endif

#endif /* RFLM_HWINTF_H */
