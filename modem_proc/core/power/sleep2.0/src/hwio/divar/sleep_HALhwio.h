#ifndef __SLEEP_HALHWIO_H__
#define __SLEEP_HALHWIO_H__
/*
===========================================================================
*/
/**
    @file sleep_HALhwio.h
    @brief Auto-generated HWIO interface include file.

    Reference chip release:
        SM6225 (Divar) [divar_v1.0_p3q3r44.1_MTO]
 
    This file contains HWIO register definitions for the following modules:
        MSS_QDSP6V671SS_CSR
        MSS_QDSP6V671SS_PUB
        MSS_PERPH_MSS_PERPH


    Generation parameters: 
    { 'filename': 'sleep_HALhwio.h',
      'header': '#include "msmhwiobase.h"',
      'module-filter-exclude': {},
      'module-filter-include': {},
      'modules': [ 'MSS_QDSP6V671SS_CSR',
                   'MSS_QDSP6V671SS_PUB',
                   'MSS_PERPH_MSS_PERPH']}
*/
/*
    ===========================================================================

    Copyright (c) 2021 Qualcomm Technologies, Inc.
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

    $Header: //components/rel/core.mpss/10.0/power/sleep2.0/src/hwio/divar/sleep_HALhwio.h#2 $
    $DateTime: 2021/05/31 07:21:52 $
    $Author: pwbldsvc $

    ===========================================================================
*/

#include "msmhwiobase.h"

/*----------------------------------------------------------------------------
 * MODULE: MSS_QDSP6V671SS_CSR
 *--------------------------------------------------------------------------*/

#define MSS_QDSP6V671SS_CSR_REG_BASE                                                     (MSS_TOP_BASE            + 0x00100000)
#define MSS_QDSP6V671SS_CSR_REG_BASE_SIZE                                                0xd000
#define MSS_QDSP6V671SS_CSR_REG_BASE_USED                                                0xc24c

#define HWIO_MSS_QDSP6SS_INSTANCE_ID_ADDR                                                (MSS_QDSP6V671SS_CSR_REG_BASE            + 0x0)
#define HWIO_MSS_QDSP6SS_INSTANCE_ID_RMSK                                                       0x7
#define HWIO_MSS_QDSP6SS_INSTANCE_ID_IN                    \
                in_dword(HWIO_MSS_QDSP6SS_INSTANCE_ID_ADDR)
#define HWIO_MSS_QDSP6SS_INSTANCE_ID_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_INSTANCE_ID_ADDR, m)
#define HWIO_MSS_QDSP6SS_INSTANCE_ID_ID_BMSK                                                    0x7
#define HWIO_MSS_QDSP6SS_INSTANCE_ID_ID_SHFT                                                      0

#define HWIO_MSS_QDSP6SS_CP_EFUSE_STATUS_ADDR                                            (MSS_QDSP6V671SS_CSR_REG_BASE            + 0x100)
#define HWIO_MSS_QDSP6SS_CP_EFUSE_STATUS_RMSK                                                   0x1
#define HWIO_MSS_QDSP6SS_CP_EFUSE_STATUS_IN                    \
                in_dword(HWIO_MSS_QDSP6SS_CP_EFUSE_STATUS_ADDR)
#define HWIO_MSS_QDSP6SS_CP_EFUSE_STATUS_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_CP_EFUSE_STATUS_ADDR, m)
#define HWIO_MSS_QDSP6SS_CP_EFUSE_STATUS_HVX_EN_BMSK                                            0x1
#define HWIO_MSS_QDSP6SS_CP_EFUSE_STATUS_HVX_EN_SHFT                                              0

#define HWIO_MSS_QDSP6SS_WDOG_RESET_ADDR                                                 (MSS_QDSP6V671SS_CSR_REG_BASE            + 0x4000)
#define HWIO_MSS_QDSP6SS_WDOG_RESET_RMSK                                                        0x1
#define HWIO_MSS_QDSP6SS_WDOG_RESET_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_WDOG_RESET_ADDR,v)
#define HWIO_MSS_QDSP6SS_WDOG_RESET_WDOG_RESET_BMSK                                             0x1
#define HWIO_MSS_QDSP6SS_WDOG_RESET_WDOG_RESET_SHFT                                               0

#define HWIO_MSS_QDSP6SS_WDOG_CTL_ADDR                                                   (MSS_QDSP6V671SS_CSR_REG_BASE            + 0x4004)
#define HWIO_MSS_QDSP6SS_WDOG_CTL_RMSK                                                          0x7
#define HWIO_MSS_QDSP6SS_WDOG_CTL_IN                    \
                in_dword(HWIO_MSS_QDSP6SS_WDOG_CTL_ADDR)
#define HWIO_MSS_QDSP6SS_WDOG_CTL_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_WDOG_CTL_ADDR, m)
#define HWIO_MSS_QDSP6SS_WDOG_CTL_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_WDOG_CTL_ADDR,v)
#define HWIO_MSS_QDSP6SS_WDOG_CTL_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_QDSP6SS_WDOG_CTL_ADDR,m,v,HWIO_MSS_QDSP6SS_WDOG_CTL_IN)
#define HWIO_MSS_QDSP6SS_WDOG_CTL_WDOG_TO_NMI_EN_BMSK                                           0x4
#define HWIO_MSS_QDSP6SS_WDOG_CTL_WDOG_TO_NMI_EN_SHFT                                             2
#define HWIO_MSS_QDSP6SS_WDOG_CTL_UNMASKED_INT_EN_BMSK                                          0x2
#define HWIO_MSS_QDSP6SS_WDOG_CTL_UNMASKED_INT_EN_SHFT                                            1
#define HWIO_MSS_QDSP6SS_WDOG_CTL_ENABLE_BMSK                                                   0x1
#define HWIO_MSS_QDSP6SS_WDOG_CTL_ENABLE_SHFT                                                     0

#define HWIO_MSS_QDSP6SS_WDOG_STATUS_ADDR                                                (MSS_QDSP6V671SS_CSR_REG_BASE            + 0x4008)
#define HWIO_MSS_QDSP6SS_WDOG_STATUS_RMSK                                                0xf81fffff
#define HWIO_MSS_QDSP6SS_WDOG_STATUS_IN                    \
                in_dword(HWIO_MSS_QDSP6SS_WDOG_STATUS_ADDR)
#define HWIO_MSS_QDSP6SS_WDOG_STATUS_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_WDOG_STATUS_ADDR, m)
#define HWIO_MSS_QDSP6SS_WDOG_STATUS_RESET_SYNC_STATUS_BMSK                              0x80000000
#define HWIO_MSS_QDSP6SS_WDOG_STATUS_RESET_SYNC_STATUS_SHFT                                      31
#define HWIO_MSS_QDSP6SS_WDOG_STATUS_BITE_TIME_SYNC_STATUS_BMSK                          0x40000000
#define HWIO_MSS_QDSP6SS_WDOG_STATUS_BITE_TIME_SYNC_STATUS_SHFT                                  30
#define HWIO_MSS_QDSP6SS_WDOG_STATUS_BARK_TIME_SYNC_STATUS_BMSK                          0x20000000
#define HWIO_MSS_QDSP6SS_WDOG_STATUS_BARK_TIME_SYNC_STATUS_SHFT                                  29
#define HWIO_MSS_QDSP6SS_WDOG_STATUS_NMI_TIME_SYNC_STATUS_BMSK                           0x10000000
#define HWIO_MSS_QDSP6SS_WDOG_STATUS_NMI_TIME_SYNC_STATUS_SHFT                                   28
#define HWIO_MSS_QDSP6SS_WDOG_STATUS_TEST_LOAD_SYNC_STATUS_BMSK                           0x8000000
#define HWIO_MSS_QDSP6SS_WDOG_STATUS_TEST_LOAD_SYNC_STATUS_SHFT                                  27
#define HWIO_MSS_QDSP6SS_WDOG_STATUS_WDOG_COUNT_BMSK                                       0x1ffffe
#define HWIO_MSS_QDSP6SS_WDOG_STATUS_WDOG_COUNT_SHFT                                              1
#define HWIO_MSS_QDSP6SS_WDOG_STATUS_WDOG_EXPIRED_STATUS_BMSK                                   0x1
#define HWIO_MSS_QDSP6SS_WDOG_STATUS_WDOG_EXPIRED_STATUS_SHFT                                     0

#define HWIO_MSS_QDSP6SS_WDOG_BARK_TIME_ADDR                                             (MSS_QDSP6V671SS_CSR_REG_BASE            + 0x400c)
#define HWIO_MSS_QDSP6SS_WDOG_BARK_TIME_RMSK                                                0xfffff
#define HWIO_MSS_QDSP6SS_WDOG_BARK_TIME_IN                    \
                in_dword(HWIO_MSS_QDSP6SS_WDOG_BARK_TIME_ADDR)
#define HWIO_MSS_QDSP6SS_WDOG_BARK_TIME_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_WDOG_BARK_TIME_ADDR, m)
#define HWIO_MSS_QDSP6SS_WDOG_BARK_TIME_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_WDOG_BARK_TIME_ADDR,v)
#define HWIO_MSS_QDSP6SS_WDOG_BARK_TIME_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_QDSP6SS_WDOG_BARK_TIME_ADDR,m,v,HWIO_MSS_QDSP6SS_WDOG_BARK_TIME_IN)
#define HWIO_MSS_QDSP6SS_WDOG_BARK_TIME_DATA_BMSK                                           0xfffff
#define HWIO_MSS_QDSP6SS_WDOG_BARK_TIME_DATA_SHFT                                                 0

#define HWIO_MSS_QDSP6SS_WDOG_BITE_TIME_ADDR                                             (MSS_QDSP6V671SS_CSR_REG_BASE            + 0x4010)
#define HWIO_MSS_QDSP6SS_WDOG_BITE_TIME_RMSK                                                0xfffff
#define HWIO_MSS_QDSP6SS_WDOG_BITE_TIME_IN                    \
                in_dword(HWIO_MSS_QDSP6SS_WDOG_BITE_TIME_ADDR)
#define HWIO_MSS_QDSP6SS_WDOG_BITE_TIME_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_WDOG_BITE_TIME_ADDR, m)
#define HWIO_MSS_QDSP6SS_WDOG_BITE_TIME_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_WDOG_BITE_TIME_ADDR,v)
#define HWIO_MSS_QDSP6SS_WDOG_BITE_TIME_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_QDSP6SS_WDOG_BITE_TIME_ADDR,m,v,HWIO_MSS_QDSP6SS_WDOG_BITE_TIME_IN)
#define HWIO_MSS_QDSP6SS_WDOG_BITE_TIME_DATA_BMSK                                           0xfffff
#define HWIO_MSS_QDSP6SS_WDOG_BITE_TIME_DATA_SHFT                                                 0

#define HWIO_MSS_QDSP6SS_WDOG_NMI_TIME_ADDR                                              (MSS_QDSP6V671SS_CSR_REG_BASE            + 0x4014)
#define HWIO_MSS_QDSP6SS_WDOG_NMI_TIME_RMSK                                                 0xfffff
#define HWIO_MSS_QDSP6SS_WDOG_NMI_TIME_IN                    \
                in_dword(HWIO_MSS_QDSP6SS_WDOG_NMI_TIME_ADDR)
#define HWIO_MSS_QDSP6SS_WDOG_NMI_TIME_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_WDOG_NMI_TIME_ADDR, m)
#define HWIO_MSS_QDSP6SS_WDOG_NMI_TIME_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_WDOG_NMI_TIME_ADDR,v)
#define HWIO_MSS_QDSP6SS_WDOG_NMI_TIME_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_QDSP6SS_WDOG_NMI_TIME_ADDR,m,v,HWIO_MSS_QDSP6SS_WDOG_NMI_TIME_IN)
#define HWIO_MSS_QDSP6SS_WDOG_NMI_TIME_DATA_BMSK                                            0xfffff
#define HWIO_MSS_QDSP6SS_WDOG_NMI_TIME_DATA_SHFT                                                  0

#define HWIO_MSS_QDSP6SS_WDOG_TEST_LOAD_ADDR                                             (MSS_QDSP6V671SS_CSR_REG_BASE            + 0x4018)
#define HWIO_MSS_QDSP6SS_WDOG_TEST_LOAD_RMSK                                                    0x1
#define HWIO_MSS_QDSP6SS_WDOG_TEST_LOAD_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_WDOG_TEST_LOAD_ADDR,v)
#define HWIO_MSS_QDSP6SS_WDOG_TEST_LOAD_LOAD_BMSK                                               0x1
#define HWIO_MSS_QDSP6SS_WDOG_TEST_LOAD_LOAD_SHFT                                                 0

#define HWIO_MSS_QDSP6SS_WDOG_TEST_ADDR                                                  (MSS_QDSP6V671SS_CSR_REG_BASE            + 0x401c)
#define HWIO_MSS_QDSP6SS_WDOG_TEST_RMSK                                                    0x1fffff
#define HWIO_MSS_QDSP6SS_WDOG_TEST_IN                    \
                in_dword(HWIO_MSS_QDSP6SS_WDOG_TEST_ADDR)
#define HWIO_MSS_QDSP6SS_WDOG_TEST_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_WDOG_TEST_ADDR, m)
#define HWIO_MSS_QDSP6SS_WDOG_TEST_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_WDOG_TEST_ADDR,v)
#define HWIO_MSS_QDSP6SS_WDOG_TEST_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_QDSP6SS_WDOG_TEST_ADDR,m,v,HWIO_MSS_QDSP6SS_WDOG_TEST_IN)
#define HWIO_MSS_QDSP6SS_WDOG_TEST_SYNC_STATUS_BMSK                                        0x100000
#define HWIO_MSS_QDSP6SS_WDOG_TEST_SYNC_STATUS_SHFT                                              20
#define HWIO_MSS_QDSP6SS_WDOG_TEST_LOAD_VALUE_BMSK                                          0xfffff
#define HWIO_MSS_QDSP6SS_WDOG_TEST_LOAD_VALUE_SHFT                                                0

#define HWIO_MSS_QDSP6SS_GLBCNT_TRIG_CFG_ADDR                                            (MSS_QDSP6V671SS_CSR_REG_BASE            + 0x4100)
#define HWIO_MSS_QDSP6SS_GLBCNT_TRIG_CFG_RMSK                                                   0xf
#define HWIO_MSS_QDSP6SS_GLBCNT_TRIG_CFG_IN                    \
                in_dword(HWIO_MSS_QDSP6SS_GLBCNT_TRIG_CFG_ADDR)
#define HWIO_MSS_QDSP6SS_GLBCNT_TRIG_CFG_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_GLBCNT_TRIG_CFG_ADDR, m)
#define HWIO_MSS_QDSP6SS_GLBCNT_TRIG_CFG_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_GLBCNT_TRIG_CFG_ADDR,v)
#define HWIO_MSS_QDSP6SS_GLBCNT_TRIG_CFG_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_QDSP6SS_GLBCNT_TRIG_CFG_ADDR,m,v,HWIO_MSS_QDSP6SS_GLBCNT_TRIG_CFG_IN)
#define HWIO_MSS_QDSP6SS_GLBCNT_TRIG_CFG_MAX_COUNT_BMSK                                         0xe
#define HWIO_MSS_QDSP6SS_GLBCNT_TRIG_CFG_MAX_COUNT_SHFT                                           1
#define HWIO_MSS_QDSP6SS_GLBCNT_TRIG_CFG_DISABLE_BMSK                                           0x1
#define HWIO_MSS_QDSP6SS_GLBCNT_TRIG_CFG_DISABLE_SHFT                                             0

#define HWIO_MSS_QDSP6SS_TS_m_EN_ADDR(m)                                                 (MSS_QDSP6V671SS_CSR_REG_BASE            + 0X5000 + (0x10*(m)))
#define HWIO_MSS_QDSP6SS_TS_m_EN_RMSK                                                           0x7
#define HWIO_MSS_QDSP6SS_TS_m_EN_MAXm                                                             3
#define HWIO_MSS_QDSP6SS_TS_m_EN_INI(m)                \
                in_dword_masked(HWIO_MSS_QDSP6SS_TS_m_EN_ADDR(m), HWIO_MSS_QDSP6SS_TS_m_EN_RMSK)
#define HWIO_MSS_QDSP6SS_TS_m_EN_INMI(m,mask)        \
                in_dword_masked(HWIO_MSS_QDSP6SS_TS_m_EN_ADDR(m), mask)
#define HWIO_MSS_QDSP6SS_TS_m_EN_OUTI(m,val)        \
                out_dword(HWIO_MSS_QDSP6SS_TS_m_EN_ADDR(m),val)
#define HWIO_MSS_QDSP6SS_TS_m_EN_OUTMI(m,mask,val) \
                out_dword_masked_ns(HWIO_MSS_QDSP6SS_TS_m_EN_ADDR(m),mask,val,HWIO_MSS_QDSP6SS_TS_m_EN_INI(m))
#define HWIO_MSS_QDSP6SS_TS_m_EN_MODE_BMSK                                                      0x4
#define HWIO_MSS_QDSP6SS_TS_m_EN_MODE_SHFT                                                        2
#define HWIO_MSS_QDSP6SS_TS_m_EN_CLEAR_BMSK                                                     0x2
#define HWIO_MSS_QDSP6SS_TS_m_EN_CLEAR_SHFT                                                       1
#define HWIO_MSS_QDSP6SS_TS_m_EN_EN_BMSK                                                        0x1
#define HWIO_MSS_QDSP6SS_TS_m_EN_EN_SHFT                                                          0

#define HWIO_MSS_QDSP6SS_TS_m_STATUS_ADDR(m)                                             (MSS_QDSP6V671SS_CSR_REG_BASE            + 0X5004 + (0x10*(m)))
#define HWIO_MSS_QDSP6SS_TS_m_STATUS_RMSK                                                     0x101
#define HWIO_MSS_QDSP6SS_TS_m_STATUS_MAXm                                                         3
#define HWIO_MSS_QDSP6SS_TS_m_STATUS_INI(m)                \
                in_dword_masked(HWIO_MSS_QDSP6SS_TS_m_STATUS_ADDR(m), HWIO_MSS_QDSP6SS_TS_m_STATUS_RMSK)
#define HWIO_MSS_QDSP6SS_TS_m_STATUS_INMI(m,mask)        \
                in_dword_masked(HWIO_MSS_QDSP6SS_TS_m_STATUS_ADDR(m), mask)
#define HWIO_MSS_QDSP6SS_TS_m_STATUS_OVERFLOW_BMSK                                            0x100
#define HWIO_MSS_QDSP6SS_TS_m_STATUS_OVERFLOW_SHFT                                                8
#define HWIO_MSS_QDSP6SS_TS_m_STATUS_VALID_BMSK                                                 0x1
#define HWIO_MSS_QDSP6SS_TS_m_STATUS_VALID_SHFT                                                   0

#define HWIO_MSS_QDSP6SS_TS_m_TIMESTAMP_L_ADDR(m)                                        (MSS_QDSP6V671SS_CSR_REG_BASE            + 0X5008 + (0x10*(m)))
#define HWIO_MSS_QDSP6SS_TS_m_TIMESTAMP_L_RMSK                                           0xffffffff
#define HWIO_MSS_QDSP6SS_TS_m_TIMESTAMP_L_MAXm                                                    3
#define HWIO_MSS_QDSP6SS_TS_m_TIMESTAMP_L_INI(m)                \
                in_dword_masked(HWIO_MSS_QDSP6SS_TS_m_TIMESTAMP_L_ADDR(m), HWIO_MSS_QDSP6SS_TS_m_TIMESTAMP_L_RMSK)
#define HWIO_MSS_QDSP6SS_TS_m_TIMESTAMP_L_INMI(m,mask)        \
                in_dword_masked(HWIO_MSS_QDSP6SS_TS_m_TIMESTAMP_L_ADDR(m), mask)
#define HWIO_MSS_QDSP6SS_TS_m_TIMESTAMP_L_DATA_L_BMSK                                    0xffffffff
#define HWIO_MSS_QDSP6SS_TS_m_TIMESTAMP_L_DATA_L_SHFT                                             0

#define HWIO_MSS_QDSP6SS_TS_m_TIMESTAMP_H_ADDR(m)                                        (MSS_QDSP6V671SS_CSR_REG_BASE            + 0X500C + (0x10*(m)))
#define HWIO_MSS_QDSP6SS_TS_m_TIMESTAMP_H_RMSK                                             0xffffff
#define HWIO_MSS_QDSP6SS_TS_m_TIMESTAMP_H_MAXm                                                    3
#define HWIO_MSS_QDSP6SS_TS_m_TIMESTAMP_H_INI(m)                \
                in_dword_masked(HWIO_MSS_QDSP6SS_TS_m_TIMESTAMP_H_ADDR(m), HWIO_MSS_QDSP6SS_TS_m_TIMESTAMP_H_RMSK)
#define HWIO_MSS_QDSP6SS_TS_m_TIMESTAMP_H_INMI(m,mask)        \
                in_dword_masked(HWIO_MSS_QDSP6SS_TS_m_TIMESTAMP_H_ADDR(m), mask)
#define HWIO_MSS_QDSP6SS_TS_m_TIMESTAMP_H_DATA_H_BMSK                                      0xffffff
#define HWIO_MSS_QDSP6SS_TS_m_TIMESTAMP_H_DATA_H_SHFT                                             0

#define HWIO_MSS_QDSP6SS_TS_HWE_SEL_ADDR                                                 (MSS_QDSP6V671SS_CSR_REG_BASE            + 0x5100)
#define HWIO_MSS_QDSP6SS_TS_HWE_SEL_RMSK                                                       0x3f
#define HWIO_MSS_QDSP6SS_TS_HWE_SEL_IN                    \
                in_dword(HWIO_MSS_QDSP6SS_TS_HWE_SEL_ADDR)
#define HWIO_MSS_QDSP6SS_TS_HWE_SEL_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_TS_HWE_SEL_ADDR, m)
#define HWIO_MSS_QDSP6SS_TS_HWE_SEL_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_TS_HWE_SEL_ADDR,v)
#define HWIO_MSS_QDSP6SS_TS_HWE_SEL_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_QDSP6SS_TS_HWE_SEL_ADDR,m,v,HWIO_MSS_QDSP6SS_TS_HWE_SEL_IN)
#define HWIO_MSS_QDSP6SS_TS_HWE_SEL_MUX_SEL_BMSK                                               0x3f
#define HWIO_MSS_QDSP6SS_TS_HWE_SEL_MUX_SEL_SHFT                                                  0

#define HWIO_MSS_QDSP6SS_IPC_ADDR                                                        (MSS_QDSP6V671SS_CSR_REG_BASE            + 0x8000)
#define HWIO_MSS_QDSP6SS_IPC_RMSK                                                        0xffffffff
#define HWIO_MSS_QDSP6SS_IPC_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_IPC_ADDR,v)
#define HWIO_MSS_QDSP6SS_IPC_INTR_BMSK                                                   0xffffffff
#define HWIO_MSS_QDSP6SS_IPC_INTR_SHFT                                                            0

#define HWIO_MSS_QDSP6SS_IPC1_ADDR                                                       (MSS_QDSP6V671SS_CSR_REG_BASE            + 0x8004)
#define HWIO_MSS_QDSP6SS_IPC1_RMSK                                                       0xffffffff
#define HWIO_MSS_QDSP6SS_IPC1_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_IPC1_ADDR,v)
#define HWIO_MSS_QDSP6SS_IPC1_INTR_BMSK                                                  0xffffffff
#define HWIO_MSS_QDSP6SS_IPC1_INTR_SHFT                                                           0

#define HWIO_MSS_QDSP6SS_BLOCK_INTR_ADDR                                                 (MSS_QDSP6V671SS_CSR_REG_BASE            + 0xc008)
#define HWIO_MSS_QDSP6SS_BLOCK_INTR_RMSK                                                        0x7
#define HWIO_MSS_QDSP6SS_BLOCK_INTR_IN                    \
                in_dword(HWIO_MSS_QDSP6SS_BLOCK_INTR_ADDR)
#define HWIO_MSS_QDSP6SS_BLOCK_INTR_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_BLOCK_INTR_ADDR, m)
#define HWIO_MSS_QDSP6SS_BLOCK_INTR_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_BLOCK_INTR_ADDR,v)
#define HWIO_MSS_QDSP6SS_BLOCK_INTR_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_QDSP6SS_BLOCK_INTR_ADDR,m,v,HWIO_MSS_QDSP6SS_BLOCK_INTR_IN)
#define HWIO_MSS_QDSP6SS_BLOCK_INTR_CLEAR_BLOCK_BMSK                                            0x4
#define HWIO_MSS_QDSP6SS_BLOCK_INTR_CLEAR_BLOCK_SHFT                                              2
#define HWIO_MSS_QDSP6SS_BLOCK_INTR_BLOCK_STATUS_BMSK                                           0x2
#define HWIO_MSS_QDSP6SS_BLOCK_INTR_BLOCK_STATUS_SHFT                                             1
#define HWIO_MSS_QDSP6SS_BLOCK_INTR_BLOCK_INTR_BMSK                                             0x1
#define HWIO_MSS_QDSP6SS_BLOCK_INTR_BLOCK_INTR_SHFT                                               0

#define HWIO_MSS_QDSP6SS_SLPC_CFG_ADDR                                                   (MSS_QDSP6V671SS_CSR_REG_BASE            + 0xc01c)
#define HWIO_MSS_QDSP6SS_SLPC_CFG_RMSK                                                       0x7fa0
#define HWIO_MSS_QDSP6SS_SLPC_CFG_IN                    \
                in_dword(HWIO_MSS_QDSP6SS_SLPC_CFG_ADDR)
#define HWIO_MSS_QDSP6SS_SLPC_CFG_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_SLPC_CFG_ADDR, m)
#define HWIO_MSS_QDSP6SS_SLPC_CFG_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_SLPC_CFG_ADDR,v)
#define HWIO_MSS_QDSP6SS_SLPC_CFG_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_QDSP6SS_SLPC_CFG_ADDR,m,v,HWIO_MSS_QDSP6SS_SLPC_CFG_IN)
#define HWIO_MSS_QDSP6SS_SLPC_CFG_MEM_FAST_STAGGER_BMSK                                      0x4000
#define HWIO_MSS_QDSP6SS_SLPC_CFG_MEM_FAST_STAGGER_SHFT                                          14
#define HWIO_MSS_QDSP6SS_SLPC_CFG_IDLE_CORE_CLK_EN_BMSK                                      0x2000
#define HWIO_MSS_QDSP6SS_SLPC_CFG_IDLE_CORE_CLK_EN_SHFT                                          13
#define HWIO_MSS_QDSP6SS_SLPC_CFG_EXTHW_WAKE_EN_BMSK                                         0x1000
#define HWIO_MSS_QDSP6SS_SLPC_CFG_EXTHW_WAKE_EN_SHFT                                             12
#define HWIO_MSS_QDSP6SS_SLPC_CFG_MEM_PU_PERI_STAGGER_BMSK                                    0x800
#define HWIO_MSS_QDSP6SS_SLPC_CFG_MEM_PU_PERI_STAGGER_SHFT                                       11
#define HWIO_MSS_QDSP6SS_SLPC_CFG_MEM_PD_PERI_STAGGER_BMSK                                    0x400
#define HWIO_MSS_QDSP6SS_SLPC_CFG_MEM_PD_PERI_STAGGER_SHFT                                       10
#define HWIO_MSS_QDSP6SS_SLPC_CFG_MEM_PU_ARRY_STAGGER_BMSK                                    0x200
#define HWIO_MSS_QDSP6SS_SLPC_CFG_MEM_PU_ARRY_STAGGER_SHFT                                        9
#define HWIO_MSS_QDSP6SS_SLPC_CFG_MEM_PD_ARRY_STAGGER_BMSK                                    0x100
#define HWIO_MSS_QDSP6SS_SLPC_CFG_MEM_PD_ARRY_STAGGER_SHFT                                        8
#define HWIO_MSS_QDSP6SS_SLPC_CFG_WAKEUP_IN_EN_BMSK                                            0x80
#define HWIO_MSS_QDSP6SS_SLPC_CFG_WAKEUP_IN_EN_SHFT                                               7
#define HWIO_MSS_QDSP6SS_SLPC_CFG_CLK_GATING_MODE_BMSK                                         0x20
#define HWIO_MSS_QDSP6SS_SLPC_CFG_CLK_GATING_MODE_SHFT                                            5

#define HWIO_MSS_QDSP6SS_RSCCTL_BR_ADDR                                                  (MSS_QDSP6V671SS_CSR_REG_BASE            + 0xc020)
#define HWIO_MSS_QDSP6SS_RSCCTL_BR_RMSK                                                         0xe
#define HWIO_MSS_QDSP6SS_RSCCTL_BR_IN                    \
                in_dword(HWIO_MSS_QDSP6SS_RSCCTL_BR_ADDR)
#define HWIO_MSS_QDSP6SS_RSCCTL_BR_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_RSCCTL_BR_ADDR, m)
#define HWIO_MSS_QDSP6SS_RSCCTL_BR_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_RSCCTL_BR_ADDR,v)
#define HWIO_MSS_QDSP6SS_RSCCTL_BR_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_QDSP6SS_RSCCTL_BR_ADDR,m,v,HWIO_MSS_QDSP6SS_RSCCTL_BR_IN)
#define HWIO_MSS_QDSP6SS_RSCCTL_BR_EVENT_BMSK                                                   0xe
#define HWIO_MSS_QDSP6SS_RSCCTL_BR_EVENT_SHFT                                                     1

#define HWIO_MSS_QDSP6SS_WAKEUP_CTL_ADDR                                                 (MSS_QDSP6V671SS_CSR_REG_BASE            + 0xc024)
#define HWIO_MSS_QDSP6SS_WAKEUP_CTL_RMSK                                                        0x3
#define HWIO_MSS_QDSP6SS_WAKEUP_CTL_IN                    \
                in_dword(HWIO_MSS_QDSP6SS_WAKEUP_CTL_ADDR)
#define HWIO_MSS_QDSP6SS_WAKEUP_CTL_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_WAKEUP_CTL_ADDR, m)
#define HWIO_MSS_QDSP6SS_WAKEUP_CTL_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_WAKEUP_CTL_ADDR,v)
#define HWIO_MSS_QDSP6SS_WAKEUP_CTL_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_QDSP6SS_WAKEUP_CTL_ADDR,m,v,HWIO_MSS_QDSP6SS_WAKEUP_CTL_IN)
#define HWIO_MSS_QDSP6SS_WAKEUP_CTL_NMI_WAKE_EN_BMSK                                            0x2
#define HWIO_MSS_QDSP6SS_WAKEUP_CTL_NMI_WAKE_EN_SHFT                                              1
#define HWIO_MSS_QDSP6SS_WAKEUP_CTL_L2VIC_WAKE_EN_BMSK                                          0x1
#define HWIO_MSS_QDSP6SS_WAKEUP_CTL_L2VIC_WAKE_EN_SHFT                                            0

#define HWIO_MSS_QDSP6SS_L2VIC_WAKEUP_EN_ADDR                                            (MSS_QDSP6V671SS_CSR_REG_BASE            + 0xc028)
#define HWIO_MSS_QDSP6SS_L2VIC_WAKEUP_EN_RMSK                                                  0x1c
#define HWIO_MSS_QDSP6SS_L2VIC_WAKEUP_EN_IN                    \
                in_dword(HWIO_MSS_QDSP6SS_L2VIC_WAKEUP_EN_ADDR)
#define HWIO_MSS_QDSP6SS_L2VIC_WAKEUP_EN_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_L2VIC_WAKEUP_EN_ADDR, m)
#define HWIO_MSS_QDSP6SS_L2VIC_WAKEUP_EN_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_L2VIC_WAKEUP_EN_ADDR,v)
#define HWIO_MSS_QDSP6SS_L2VIC_WAKEUP_EN_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_QDSP6SS_L2VIC_WAKEUP_EN_ADDR,m,v,HWIO_MSS_QDSP6SS_L2VIC_WAKEUP_EN_IN)
#define HWIO_MSS_QDSP6SS_L2VIC_WAKEUP_EN_WAKEUP_EN_BMSK                                        0x1c
#define HWIO_MSS_QDSP6SS_L2VIC_WAKEUP_EN_WAKEUP_EN_SHFT                                           2

#define HWIO_MSS_QDSP6SS_WAKEUP_STATUS_ADDR                                              (MSS_QDSP6V671SS_CSR_REG_BASE            + 0xc02c)
#define HWIO_MSS_QDSP6SS_WAKEUP_STATUS_RMSK                                                    0x1f
#define HWIO_MSS_QDSP6SS_WAKEUP_STATUS_IN                    \
                in_dword(HWIO_MSS_QDSP6SS_WAKEUP_STATUS_ADDR)
#define HWIO_MSS_QDSP6SS_WAKEUP_STATUS_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_WAKEUP_STATUS_ADDR, m)
#define HWIO_MSS_QDSP6SS_WAKEUP_STATUS_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_WAKEUP_STATUS_ADDR,v)
#define HWIO_MSS_QDSP6SS_WAKEUP_STATUS_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_QDSP6SS_WAKEUP_STATUS_ADDR,m,v,HWIO_MSS_QDSP6SS_WAKEUP_STATUS_IN)
#define HWIO_MSS_QDSP6SS_WAKEUP_STATUS_AXIS_WAKEUP_STICKY_BMSK                                 0x10
#define HWIO_MSS_QDSP6SS_WAKEUP_STATUS_AXIS_WAKEUP_STICKY_SHFT                                    4
#define HWIO_MSS_QDSP6SS_WAKEUP_STATUS_AXIS_WAKEUP_BMSK                                         0x8
#define HWIO_MSS_QDSP6SS_WAKEUP_STATUS_AXIS_WAKEUP_SHFT                                           3
#define HWIO_MSS_QDSP6SS_WAKEUP_STATUS_DBG_NMI_BMSK                                             0x4
#define HWIO_MSS_QDSP6SS_WAKEUP_STATUS_DBG_NMI_SHFT                                               2
#define HWIO_MSS_QDSP6SS_WAKEUP_STATUS_EXT_TRIGGER_BMSK                                         0x2
#define HWIO_MSS_QDSP6SS_WAKEUP_STATUS_EXT_TRIGGER_SHFT                                           1
#define HWIO_MSS_QDSP6SS_WAKEUP_STATUS_L2VIC_WAKEUP_BMSK                                        0x1
#define HWIO_MSS_QDSP6SS_WAKEUP_STATUS_L2VIC_WAKEUP_SHFT                                          0

#define HWIO_MSS_QDSP6SS_RSC_EVENT_EXTPLL_OVR_ADDR                                       (MSS_QDSP6V671SS_CSR_REG_BASE            + 0xc050)
#define HWIO_MSS_QDSP6SS_RSC_EVENT_EXTPLL_OVR_RMSK                                              0x1
#define HWIO_MSS_QDSP6SS_RSC_EVENT_EXTPLL_OVR_IN                    \
                in_dword(HWIO_MSS_QDSP6SS_RSC_EVENT_EXTPLL_OVR_ADDR)
#define HWIO_MSS_QDSP6SS_RSC_EVENT_EXTPLL_OVR_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_RSC_EVENT_EXTPLL_OVR_ADDR, m)
#define HWIO_MSS_QDSP6SS_RSC_EVENT_EXTPLL_OVR_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_RSC_EVENT_EXTPLL_OVR_ADDR,v)
#define HWIO_MSS_QDSP6SS_RSC_EVENT_EXTPLL_OVR_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_QDSP6SS_RSC_EVENT_EXTPLL_OVR_ADDR,m,v,HWIO_MSS_QDSP6SS_RSC_EVENT_EXTPLL_OVR_IN)
#define HWIO_MSS_QDSP6SS_RSC_EVENT_EXTPLL_OVR_EXT_PLL_OVRRD_BMSK                                0x1
#define HWIO_MSS_QDSP6SS_RSC_EVENT_EXTPLL_OVR_EXT_PLL_OVRRD_SHFT                                  0

#define HWIO_MSS_QDSP6SS_RSC_EVENT_ISOINTF_OVR_ADDR                                      (MSS_QDSP6V671SS_CSR_REG_BASE            + 0xc054)
#define HWIO_MSS_QDSP6SS_RSC_EVENT_ISOINTF_OVR_RMSK                                             0x7
#define HWIO_MSS_QDSP6SS_RSC_EVENT_ISOINTF_OVR_IN                    \
                in_dword(HWIO_MSS_QDSP6SS_RSC_EVENT_ISOINTF_OVR_ADDR)
#define HWIO_MSS_QDSP6SS_RSC_EVENT_ISOINTF_OVR_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_RSC_EVENT_ISOINTF_OVR_ADDR, m)
#define HWIO_MSS_QDSP6SS_RSC_EVENT_ISOINTF_OVR_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_RSC_EVENT_ISOINTF_OVR_ADDR,v)
#define HWIO_MSS_QDSP6SS_RSC_EVENT_ISOINTF_OVR_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_QDSP6SS_RSC_EVENT_ISOINTF_OVR_ADDR,m,v,HWIO_MSS_QDSP6SS_RSC_EVENT_ISOINTF_OVR_IN)
#define HWIO_MSS_QDSP6SS_RSC_EVENT_ISOINTF_OVR_AXIM2_HALTACK_OVRRD_BMSK                         0x4
#define HWIO_MSS_QDSP6SS_RSC_EVENT_ISOINTF_OVR_AXIM2_HALTACK_OVRRD_SHFT                           2
#define HWIO_MSS_QDSP6SS_RSC_EVENT_ISOINTF_OVR_AXIM_HALTACK_OVRRD_BMSK                          0x2
#define HWIO_MSS_QDSP6SS_RSC_EVENT_ISOINTF_OVR_AXIM_HALTACK_OVRRD_SHFT                            1
#define HWIO_MSS_QDSP6SS_RSC_EVENT_ISOINTF_OVR_ALL_HALTACK_OVRRD_BMSK                           0x1
#define HWIO_MSS_QDSP6SS_RSC_EVENT_ISOINTF_OVR_ALL_HALTACK_OVRRD_SHFT                             0

#define HWIO_MSS_QDSP6SS_RSC_EVENT_PRSC_OVR_ADDR                                         (MSS_QDSP6V671SS_CSR_REG_BASE            + 0xc058)
#define HWIO_MSS_QDSP6SS_RSC_EVENT_PRSC_OVR_RMSK                                             0x3fff
#define HWIO_MSS_QDSP6SS_RSC_EVENT_PRSC_OVR_IN                    \
                in_dword(HWIO_MSS_QDSP6SS_RSC_EVENT_PRSC_OVR_ADDR)
#define HWIO_MSS_QDSP6SS_RSC_EVENT_PRSC_OVR_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_RSC_EVENT_PRSC_OVR_ADDR, m)
#define HWIO_MSS_QDSP6SS_RSC_EVENT_PRSC_OVR_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_RSC_EVENT_PRSC_OVR_ADDR,v)
#define HWIO_MSS_QDSP6SS_RSC_EVENT_PRSC_OVR_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_QDSP6SS_RSC_EVENT_PRSC_OVR_ADDR,m,v,HWIO_MSS_QDSP6SS_RSC_EVENT_PRSC_OVR_IN)
#define HWIO_MSS_QDSP6SS_RSC_EVENT_PRSC_OVR_SPARE_OVRRD_BMSK                                 0x3c00
#define HWIO_MSS_QDSP6SS_RSC_EVENT_PRSC_OVR_SPARE_OVRRD_SHFT                                     10
#define HWIO_MSS_QDSP6SS_RSC_EVENT_PRSC_OVR_TCS_HW_ACK_OVRRD_BMSK                             0x3fc
#define HWIO_MSS_QDSP6SS_RSC_EVENT_PRSC_OVR_TCS_HW_ACK_OVRRD_SHFT                                 2
#define HWIO_MSS_QDSP6SS_RSC_EVENT_PRSC_OVR_BRINGUP_ACK_OVRRD_BMSK                              0x2
#define HWIO_MSS_QDSP6SS_RSC_EVENT_PRSC_OVR_BRINGUP_ACK_OVRRD_SHFT                                1
#define HWIO_MSS_QDSP6SS_RSC_EVENT_PRSC_OVR_SHUTDOWN_ACK_OVRRD_BMSK                             0x1
#define HWIO_MSS_QDSP6SS_RSC_EVENT_PRSC_OVR_SHUTDOWN_ACK_OVRRD_SHFT                               0

#define HWIO_MSS_QDSP6SS_RSC_EVENT_PLL_OVR_ADDR                                          (MSS_QDSP6V671SS_CSR_REG_BASE            + 0xc05c)
#define HWIO_MSS_QDSP6SS_RSC_EVENT_PLL_OVR_RMSK                                                 0x7
#define HWIO_MSS_QDSP6SS_RSC_EVENT_PLL_OVR_IN                    \
                in_dword(HWIO_MSS_QDSP6SS_RSC_EVENT_PLL_OVR_ADDR)
#define HWIO_MSS_QDSP6SS_RSC_EVENT_PLL_OVR_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_RSC_EVENT_PLL_OVR_ADDR, m)
#define HWIO_MSS_QDSP6SS_RSC_EVENT_PLL_OVR_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_RSC_EVENT_PLL_OVR_ADDR,v)
#define HWIO_MSS_QDSP6SS_RSC_EVENT_PLL_OVR_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_QDSP6SS_RSC_EVENT_PLL_OVR_ADDR,m,v,HWIO_MSS_QDSP6SS_RSC_EVENT_PLL_OVR_IN)
#define HWIO_MSS_QDSP6SS_RSC_EVENT_PLL_OVR_PLL_DYN_OVRRD_BMSK                                   0x4
#define HWIO_MSS_QDSP6SS_RSC_EVENT_PLL_OVR_PLL_DYN_OVRRD_SHFT                                     2
#define HWIO_MSS_QDSP6SS_RSC_EVENT_PLL_OVR_PLL_FINE_OVRRD_BMSK                                  0x2
#define HWIO_MSS_QDSP6SS_RSC_EVENT_PLL_OVR_PLL_FINE_OVRRD_SHFT                                    1
#define HWIO_MSS_QDSP6SS_RSC_EVENT_PLL_OVR_PLL_OVRRD_BMSK                                       0x1
#define HWIO_MSS_QDSP6SS_RSC_EVENT_PLL_OVR_PLL_OVRRD_SHFT                                         0

#define HWIO_MSS_QDSP6SS_RSC_EVENT_MEM_OVR_ADDR                                          (MSS_QDSP6V671SS_CSR_REG_BASE            + 0xc060)
#define HWIO_MSS_QDSP6SS_RSC_EVENT_MEM_OVR_RMSK                                                0x3f
#define HWIO_MSS_QDSP6SS_RSC_EVENT_MEM_OVR_IN                    \
                in_dword(HWIO_MSS_QDSP6SS_RSC_EVENT_MEM_OVR_ADDR)
#define HWIO_MSS_QDSP6SS_RSC_EVENT_MEM_OVR_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_RSC_EVENT_MEM_OVR_ADDR, m)
#define HWIO_MSS_QDSP6SS_RSC_EVENT_MEM_OVR_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_RSC_EVENT_MEM_OVR_ADDR,v)
#define HWIO_MSS_QDSP6SS_RSC_EVENT_MEM_OVR_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_QDSP6SS_RSC_EVENT_MEM_OVR_ADDR,m,v,HWIO_MSS_QDSP6SS_RSC_EVENT_MEM_OVR_IN)
#define HWIO_MSS_QDSP6SS_RSC_EVENT_MEM_OVR_STAGGER_RESTORE_BMSK                                0x20
#define HWIO_MSS_QDSP6SS_RSC_EVENT_MEM_OVR_STAGGER_RESTORE_SHFT                                   5
#define HWIO_MSS_QDSP6SS_RSC_EVENT_MEM_OVR_REFRESH_BMSK                                        0x10
#define HWIO_MSS_QDSP6SS_RSC_EVENT_MEM_OVR_REFRESH_SHFT                                           4
#define HWIO_MSS_QDSP6SS_RSC_EVENT_MEM_OVR_PERIPH_PD_BMSK                                       0x8
#define HWIO_MSS_QDSP6SS_RSC_EVENT_MEM_OVR_PERIPH_PD_SHFT                                         3
#define HWIO_MSS_QDSP6SS_RSC_EVENT_MEM_OVR_PERIPH_READY_BMSK                                    0x4
#define HWIO_MSS_QDSP6SS_RSC_EVENT_MEM_OVR_PERIPH_READY_SHFT                                      2
#define HWIO_MSS_QDSP6SS_RSC_EVENT_MEM_OVR_CORE_PD_BMSK                                         0x2
#define HWIO_MSS_QDSP6SS_RSC_EVENT_MEM_OVR_CORE_PD_SHFT                                           1
#define HWIO_MSS_QDSP6SS_RSC_EVENT_MEM_OVR_CORE_READY_BMSK                                      0x1
#define HWIO_MSS_QDSP6SS_RSC_EVENT_MEM_OVR_CORE_READY_SHFT                                        0

#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_ADDR                                             (MSS_QDSP6V671SS_CSR_REG_BASE            + 0xc100)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_RMSK                                                0x1ffff
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_IN                    \
                in_dword(HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_ADDR)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_ADDR, m)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_ADDR,v)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_ADDR,m,v,HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_IN)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_CLADE2_BMSK                                         0x10000
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_CLADE2_SHFT                                              16
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_VPE_VU_BMSK                                          0x8000
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_VPE_VU_SHFT                                              15
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_NRET_BMSK                                            0x4000
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_NRET_SHFT                                                14
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_MNOC_BMSK                                            0x2000
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_MNOC_SHFT                                                13
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_APB_BMSK                                             0x1000
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_APB_SHFT                                                 12
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_AXIS2_BMSK                                            0x800
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_AXIS2_SHFT                                               11
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_CSI1_BMSK                                             0x400
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_CSI1_SHFT                                                10
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_CSI0_BMSK                                             0x200
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_CSI0_SHFT                                                 9
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_ARCH_BMSK                                             0x100
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_ARCH_SHFT                                                 8
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_CORE_BMSK                                              0x80
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_CORE_SHFT                                                 7
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_AXIM2_BMSK                                             0x40
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_AXIM2_SHFT                                                6
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_AXIS_BMSK                                              0x20
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_AXIS_SHFT                                                 5
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_ATBM_BMSK                                              0x10
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_ATBM_SHFT                                                 4
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_AHBM_BMSK                                               0x8
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_AHBM_SHFT                                                 3
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_AXIM_BMSK                                               0x4
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_AXIM_SHFT                                                 2
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_ISDB_ETM_BMSK                                           0x2
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_ISDB_ETM_SHFT                                             1
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_RET_BMSK                                                0x1
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_RET_SHFT                                                  0

#define HWIO_MSS_QDSP6SS_RSCCTL_EN_CLAMP_ADDR                                            (MSS_QDSP6V671SS_CSR_REG_BASE            + 0xc104)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_CLAMP_RMSK                                                   0x3
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_CLAMP_IN                    \
                in_dword(HWIO_MSS_QDSP6SS_RSCCTL_EN_CLAMP_ADDR)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_CLAMP_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_RSCCTL_EN_CLAMP_ADDR, m)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_CLAMP_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_RSCCTL_EN_CLAMP_ADDR,v)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_CLAMP_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_QDSP6SS_RSCCTL_EN_CLAMP_ADDR,m,v,HWIO_MSS_QDSP6SS_RSCCTL_EN_CLAMP_IN)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_CLAMP_CLAMP_MSS2CX_BMSK                                      0x2
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_CLAMP_CLAMP_MSS2CX_SHFT                                        1
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_CLAMP_CLAMP_IO_BMSK                                          0x1
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_CLAMP_CLAMP_IO_SHFT                                            0

#define HWIO_MSS_QDSP6SS_RSCCTL_EN_CLK_ADDR                                              (MSS_QDSP6V671SS_CSR_REG_BASE            + 0xc108)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_CLK_RMSK                                                   0x7ff
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_CLK_IN                    \
                in_dword(HWIO_MSS_QDSP6SS_RSCCTL_EN_CLK_ADDR)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_CLK_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_RSCCTL_EN_CLK_ADDR, m)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_CLK_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_RSCCTL_EN_CLK_ADDR,v)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_CLK_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_QDSP6SS_RSCCTL_EN_CLK_ADDR,m,v,HWIO_MSS_QDSP6SS_RSCCTL_EN_CLK_IN)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_CLK_VPE_VU_BMSK                                            0x400
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_CLK_VPE_VU_SHFT                                               10
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_CLK_AXIS2_BMSK                                             0x200
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_CLK_AXIS2_SHFT                                                 9
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_CLK_CSI1_BMSK                                              0x100
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_CLK_CSI1_SHFT                                                  8
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_CLK_CSI0_BMSK                                               0x80
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_CLK_CSI0_SHFT                                                  7
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_CLK_AXIM2_BMSK                                              0x40
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_CLK_AXIM2_SHFT                                                 6
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_CLK_AXIS_BMSK                                               0x20
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_CLK_AXIS_SHFT                                                  5
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_CLK_ATBM_BMSK                                               0x10
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_CLK_ATBM_SHFT                                                  4
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_CLK_AHBM_BMSK                                                0x8
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_CLK_AHBM_SHFT                                                  3
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_CLK_AXIM_BMSK                                                0x4
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_CLK_AXIM_SHFT                                                  2
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_CLK_APB_BMSK                                                 0x2
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_CLK_APB_SHFT                                                   1
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_CLK_CORE_BMSK                                                0x1
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_CLK_CORE_SHFT                                                  0

#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_RET_ADDR                                          (MSS_QDSP6V671SS_CSR_REG_BASE            + 0xc10c)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_RET_RMSK                                              0xffff
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_RET_IN                    \
                in_dword(HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_RET_ADDR)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_RET_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_RET_ADDR, m)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_RET_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_RET_ADDR,v)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_RET_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_RET_ADDR,m,v,HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_RET_IN)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_RET_SPARE_BMSK                                        0xc000
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_RET_SPARE_SHFT                                            14
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_RET_L2CLADE_BMSK                                      0x2000
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_RET_L2CLADE_SHFT                                          13
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_RET_L2PLRU_BMSK                                       0x1000
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_RET_L2PLRU_SHFT                                           12
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_RET_L2TAG_BMSK                                         0x800
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_RET_L2TAG_SHFT                                            11
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_RET_L2STBUF_BMSK                                       0x400
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_RET_L2STBUF_SHFT                                          10
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_RET_L1DU_TAG_BMSK                                      0x200
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_RET_L1DU_TAG_SHFT                                          9
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_RET_L1DU_BMSK                                          0x100
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_RET_L1DU_SHFT                                              8
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_RET_JU_TLB64_BMSK                                       0x80
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_RET_JU_TLB64_SHFT                                          7
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_RET_JU_TLB128_BMSK                                      0x40
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_RET_JU_TLB128_SHFT                                         6
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_RET_JU_CAM_BMSK                                         0x20
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_RET_JU_CAM_SHFT                                            5
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_RET_ETB_BMSK                                            0x10
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_RET_ETB_SHFT                                               4
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_RET_L1IU_TAG_BMSK                                        0x8
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_RET_L1IU_TAG_SHFT                                          3
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_RET_L1IU_PDEC_BMSK                                       0x4
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_RET_L1IU_PDEC_SHFT                                         2
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_RET_L1IU_BMSK                                            0x2
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_RET_L1IU_SHFT                                              1
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_RET_L1IU_BTB_BMSK                                        0x1
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_RET_L1IU_BTB_SHFT                                          0

#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_CTL_ADDR                                          (MSS_QDSP6V671SS_CSR_REG_BASE            + 0xc110)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_CTL_RMSK                                                 0xa
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_CTL_IN                    \
                in_dword(HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_CTL_ADDR)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_CTL_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_CTL_ADDR, m)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_CTL_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_CTL_ADDR,v)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_CTL_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_CTL_ADDR,m,v,HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_CTL_IN)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_CTL_CLAMP_QMC_MEM_BMSK                                   0x8
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_CTL_CLAMP_QMC_MEM_SHFT                                     3
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_CTL_MEM_PERIPH_BMSK                                      0x2
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_CTL_MEM_PERIPH_SHFT                                        1

#define HWIO_MSS_QDSP6SS_RSCCTL_EN_BHS_ADDR                                              (MSS_QDSP6V671SS_CSR_REG_BASE            + 0xc114)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_BHS_RMSK                                                     0x1
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_BHS_IN                    \
                in_dword(HWIO_MSS_QDSP6SS_RSCCTL_EN_BHS_ADDR)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_BHS_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_RSCCTL_EN_BHS_ADDR, m)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_BHS_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_RSCCTL_EN_BHS_ADDR,v)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_BHS_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_QDSP6SS_RSCCTL_EN_BHS_ADDR,m,v,HWIO_MSS_QDSP6SS_RSCCTL_EN_BHS_IN)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_BHS_BHS_BMSK                                                 0x1
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_BHS_BHS_SHFT                                                   0

#define HWIO_MSS_QDSP6SS_RSCCTL_EN_LDO_ADDR                                              (MSS_QDSP6V671SS_CSR_REG_BASE            + 0xc118)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_LDO_RMSK                                                    0x7f
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_LDO_IN                    \
                in_dword(HWIO_MSS_QDSP6SS_RSCCTL_EN_LDO_ADDR)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_LDO_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_RSCCTL_EN_LDO_ADDR, m)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_LDO_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_RSCCTL_EN_LDO_ADDR,v)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_LDO_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_QDSP6SS_RSCCTL_EN_LDO_ADDR,m,v,HWIO_MSS_QDSP6SS_RSCCTL_EN_LDO_IN)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_LDO_BG_OFF_BMSK                                             0x40
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_LDO_BG_OFF_SHFT                                                6
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_LDO_FORCEOFF_BMSK                                           0x20
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_LDO_FORCEOFF_SHFT                                              5
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_LDO_SPARE_BITS4_2_BMSK                                      0x1c
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_LDO_SPARE_BITS4_2_SHFT                                         2
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_LDO_VREF_SEL_UPDATE_BMSK                                     0x2
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_LDO_VREF_SEL_UPDATE_SHFT                                       1
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_LDO_BYPASS_OPEN_BMSK                                         0x1
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_LDO_BYPASS_OPEN_SHFT                                           0

#define HWIO_MSS_QDSP6SS_RSCCTL_EN_EXT_ADDR                                              (MSS_QDSP6V671SS_CSR_REG_BASE            + 0xc11c)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_EXT_RMSK                                                    0x1f
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_EXT_IN                    \
                in_dword(HWIO_MSS_QDSP6SS_RSCCTL_EN_EXT_ADDR)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_EXT_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_RSCCTL_EN_EXT_ADDR, m)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_EXT_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_RSCCTL_EN_EXT_ADDR,v)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_EXT_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_QDSP6SS_RSCCTL_EN_EXT_ADDR,m,v,HWIO_MSS_QDSP6SS_RSCCTL_EN_EXT_IN)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_EXT_PLL_STBY_BMSK                                           0x10
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_EXT_PLL_STBY_SHFT                                              4
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_EXT_PLL_OUTDIS_BMSK                                          0x8
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_EXT_PLL_OUTDIS_SHFT                                            3
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_EXT_PLL_OPMODE1_BMSK                                         0x4
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_EXT_PLL_OPMODE1_SHFT                                           2
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_EXT_PLL_RESET_BMSK                                           0x2
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_EXT_PLL_RESET_SHFT                                             1
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_EXT_XO_ENABLE_BMSK                                           0x1
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_EXT_XO_ENABLE_SHFT                                             0

#define HWIO_MSS_QDSP6SS_RSCCTL_EN_PLL_ADDR                                              (MSS_QDSP6V671SS_CSR_REG_BASE            + 0xc120)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_PLL_RMSK                                                    0x17
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_PLL_IN                    \
                in_dword(HWIO_MSS_QDSP6SS_RSCCTL_EN_PLL_ADDR)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_PLL_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_RSCCTL_EN_PLL_ADDR, m)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_PLL_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_RSCCTL_EN_PLL_ADDR,v)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_PLL_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_QDSP6SS_RSCCTL_EN_PLL_ADDR,m,v,HWIO_MSS_QDSP6SS_RSCCTL_EN_PLL_IN)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_PLL_PLL_STBY_BMSK                                           0x10
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_PLL_PLL_STBY_SHFT                                              4
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_PLL_PLL_OUTDIS_BMSK                                          0x4
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_PLL_PLL_OUTDIS_SHFT                                            2
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_PLL_PLL_OPMODE1_BMSK                                         0x2
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_PLL_PLL_OPMODE1_SHFT                                           1
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_PLL_PLL_RESET_BMSK                                           0x1
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_PLL_PLL_RESET_SHFT                                             0

#define HWIO_MSS_QDSP6SS_RSCCTL_EN_STATERET_ADDR                                         (MSS_QDSP6V671SS_CSR_REG_BASE            + 0xc124)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_STATERET_RMSK                                                0xf
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_STATERET_IN                    \
                in_dword(HWIO_MSS_QDSP6SS_RSCCTL_EN_STATERET_ADDR)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_STATERET_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_RSCCTL_EN_STATERET_ADDR, m)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_STATERET_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_RSCCTL_EN_STATERET_ADDR,v)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_STATERET_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_QDSP6SS_RSCCTL_EN_STATERET_ADDR,m,v,HWIO_MSS_QDSP6SS_RSCCTL_EN_STATERET_IN)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_STATERET_PROF_IRQ_BMSK                                       0x8
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_STATERET_PROF_IRQ_SHFT                                         3
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_STATERET_WAKE_IRQ_BMSK                                       0x4
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_STATERET_WAKE_IRQ_SHFT                                         2
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_STATERET_RESTORE_BMSK                                        0x2
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_STATERET_RESTORE_SHFT                                          1
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_STATERET_SAVE_BMSK                                           0x1
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_STATERET_SAVE_SHFT                                             0

#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CTL_ADDR                                      (MSS_QDSP6V671SS_CSR_REG_BASE            + 0xc128)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CTL_RMSK                                            0x3d
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CTL_IN                    \
                in_dword(HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CTL_ADDR)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CTL_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CTL_ADDR, m)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CTL_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CTL_ADDR,v)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CTL_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CTL_ADDR,m,v,HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CTL_IN)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CTL_AXIM2_FIFO_ARESET_BMSK                          0x20
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CTL_AXIM2_FIFO_ARESET_SHFT                             5
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CTL_AXIM2_HALTREQ_BMSK                              0x10
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CTL_AXIM2_HALTREQ_SHFT                                 4
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CTL_AXIM_FIFO_ARESET_BMSK                            0x8
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CTL_AXIM_FIFO_ARESET_SHFT                              3
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CTL_AXIM_HALTREQ_BMSK                                0x4
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CTL_AXIM_HALTREQ_SHFT                                  2
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CTL_BUSM_HALTREQ_BMSK                                0x1
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CTL_BUSM_HALTREQ_SHFT                                  0

#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CLAMP_ADDR                                    (MSS_QDSP6V671SS_CSR_REG_BASE            + 0xc12c)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CLAMP_RMSK                                          0xff
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CLAMP_IN                    \
                in_dword(HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CLAMP_ADDR)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CLAMP_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CLAMP_ADDR, m)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CLAMP_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CLAMP_ADDR,v)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CLAMP_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CLAMP_ADDR,m,v,HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CLAMP_IN)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CLAMP_AXIM2_CLAMP_E_CLEAR_BMSK                      0x80
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CLAMP_AXIM2_CLAMP_E_CLEAR_SHFT                         7
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CLAMP_AXIM2_CLAMP_E_SET_BMSK                        0x40
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CLAMP_AXIM2_CLAMP_E_SET_SHFT                           6
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CLAMP_AXIM2_CLAMP_L_CLEAR_BMSK                      0x20
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CLAMP_AXIM2_CLAMP_L_CLEAR_SHFT                         5
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CLAMP_AXIM2_CLAMP_L_SET_BMSK                        0x10
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CLAMP_AXIM2_CLAMP_L_SET_SHFT                           4
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CLAMP_AXIM_CLAMP_E_CLEAR_BMSK                        0x8
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CLAMP_AXIM_CLAMP_E_CLEAR_SHFT                          3
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CLAMP_AXIM_CLAMP_E_SET_BMSK                          0x4
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CLAMP_AXIM_CLAMP_E_SET_SHFT                            2
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CLAMP_AXIM_CLAMP_L_CLEAR_BMSK                        0x2
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CLAMP_AXIM_CLAMP_L_CLEAR_SHFT                          1
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CLAMP_AXIM_CLAMP_L_SET_BMSK                          0x1
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CLAMP_AXIM_CLAMP_L_SET_SHFT                            0

#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CLK_ADDR                                      (MSS_QDSP6V671SS_CSR_REG_BASE            + 0xc130)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CLK_RMSK                                             0x7
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CLK_IN                    \
                in_dword(HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CLK_ADDR)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CLK_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CLK_ADDR, m)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CLK_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CLK_ADDR,v)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CLK_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CLK_ADDR,m,v,HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CLK_IN)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CLK_AXIM2_BMSK                                       0x4
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CLK_AXIM2_SHFT                                         2
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CLK_AXIM_BMSK                                        0x2
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CLK_AXIM_SHFT                                          1
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CLK_CORE_BMSK                                        0x1
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CLK_CORE_SHFT                                          0

#define HWIO_MSS_QDSP6SS_RSCCTL_EN_L2MEM_RET_ADDR                                        (MSS_QDSP6V671SS_CSR_REG_BASE            + 0xc134)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_L2MEM_RET_RMSK                                               0xf
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_L2MEM_RET_IN                    \
                in_dword(HWIO_MSS_QDSP6SS_RSCCTL_EN_L2MEM_RET_ADDR)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_L2MEM_RET_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_RSCCTL_EN_L2MEM_RET_ADDR, m)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_L2MEM_RET_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_RSCCTL_EN_L2MEM_RET_ADDR,v)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_L2MEM_RET_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_QDSP6SS_RSCCTL_EN_L2MEM_RET_ADDR,m,v,HWIO_MSS_QDSP6SS_RSCCTL_EN_L2MEM_RET_IN)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_L2MEM_RET_L2DATA_BMSK                                        0xf
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_L2MEM_RET_L2DATA_SHFT                                          0

#define HWIO_MSS_QDSP6SS_RSCCTL_EN_PRSC_ADDR                                             (MSS_QDSP6V671SS_CSR_REG_BASE            + 0xc138)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_PRSC_RMSK                                                 0x7fff
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_PRSC_IN                    \
                in_dword(HWIO_MSS_QDSP6SS_RSCCTL_EN_PRSC_ADDR)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_PRSC_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_RSCCTL_EN_PRSC_ADDR, m)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_PRSC_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_RSCCTL_EN_PRSC_ADDR,v)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_PRSC_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_QDSP6SS_RSCCTL_EN_PRSC_ADDR,m,v,HWIO_MSS_QDSP6SS_RSCCTL_EN_PRSC_IN)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_PRSC_FAL10_VETO_BMSK                                      0x4000
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_PRSC_FAL10_VETO_SHFT                                          14
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_PRSC_SPARE_BMSK                                           0x3c00
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_PRSC_SPARE_SHFT                                               10
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_PRSC_TCS_HW_TRIGGER_BMSK                                   0x3fc
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_PRSC_TCS_HW_TRIGGER_SHFT                                       2
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_PRSC_BRINGUP_REQ_BMSK                                        0x2
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_PRSC_BRINGUP_REQ_SHFT                                          1
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_PRSC_SHUTDOWN_REQ_BMSK                                       0x1
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_PRSC_SHUTDOWN_REQ_SHFT                                         0

#define HWIO_MSS_QDSP6SS_RSCCTL_EN_L1SMEM_RET_ADDR                                       (MSS_QDSP6V671SS_CSR_REG_BASE            + 0xc140)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_L1SMEM_RET_RMSK                                           0x3f1f
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_L1SMEM_RET_IN                    \
                in_dword(HWIO_MSS_QDSP6SS_RSCCTL_EN_L1SMEM_RET_ADDR)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_L1SMEM_RET_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_RSCCTL_EN_L1SMEM_RET_ADDR, m)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_L1SMEM_RET_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_RSCCTL_EN_L1SMEM_RET_ADDR,v)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_L1SMEM_RET_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_QDSP6SS_RSCCTL_EN_L1SMEM_RET_ADDR,m,v,HWIO_MSS_QDSP6SS_RSCCTL_EN_L1SMEM_RET_IN)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_L1SMEM_RET_L1S2DATA_BMSK                                  0x3f00
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_L1SMEM_RET_L1S2DATA_SHFT                                       8
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_L1SMEM_RET_L1SDATA_BMSK                                     0x1f
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_L1SMEM_RET_L1SDATA_SHFT                                        0

#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_STAGGER_ADDR                                      (MSS_QDSP6V671SS_CSR_REG_BASE            + 0xc144)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_STAGGER_RMSK                                             0x7
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_STAGGER_IN                    \
                in_dword(HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_STAGGER_ADDR)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_STAGGER_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_STAGGER_ADDR, m)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_STAGGER_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_STAGGER_ADDR,v)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_STAGGER_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_STAGGER_ADDR,m,v,HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_STAGGER_IN)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_STAGGER_STAGGER_RESTORE_BMSK                             0x4
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_STAGGER_STAGGER_RESTORE_SHFT                               2
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_STAGGER_STAGGER_ALT_ARES_BMSK                            0x2
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_STAGGER_STAGGER_ALT_ARES_SHFT                              1
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_STAGGER_STAGGER_FUNC_ARES_BMSK                           0x1
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_STAGGER_STAGGER_FUNC_ARES_SHFT                             0

#define HWIO_MSS_QDSP6SS_RSCCTL_EN_VTCMEM_RET_ADDR                                       (MSS_QDSP6V671SS_CSR_REG_BASE            + 0xc148)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_VTCMEM_RET_RMSK                                              0x1
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_VTCMEM_RET_IN                    \
                in_dword(HWIO_MSS_QDSP6SS_RSCCTL_EN_VTCMEM_RET_ADDR)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_VTCMEM_RET_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_RSCCTL_EN_VTCMEM_RET_ADDR, m)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_VTCMEM_RET_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_RSCCTL_EN_VTCMEM_RET_ADDR,v)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_VTCMEM_RET_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_QDSP6SS_RSCCTL_EN_VTCMEM_RET_ADDR,m,v,HWIO_MSS_QDSP6SS_RSCCTL_EN_VTCMEM_RET_IN)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_VTCMEM_RET_VTCM_BMSK                                         0x1
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_VTCMEM_RET_VTCM_SHFT                                           0

#define HWIO_MSS_QDSP6SS_RSCCTL_EN_L2ITCM_RET_ADDR                                       (MSS_QDSP6V671SS_CSR_REG_BASE            + 0xc14c)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_L2ITCM_RET_RMSK                                              0x3
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_L2ITCM_RET_IN                    \
                in_dword(HWIO_MSS_QDSP6SS_RSCCTL_EN_L2ITCM_RET_ADDR)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_L2ITCM_RET_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_RSCCTL_EN_L2ITCM_RET_ADDR, m)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_L2ITCM_RET_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_RSCCTL_EN_L2ITCM_RET_ADDR,v)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_L2ITCM_RET_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_QDSP6SS_RSCCTL_EN_L2ITCM_RET_ADDR,m,v,HWIO_MSS_QDSP6SS_RSCCTL_EN_L2ITCM_RET_IN)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_L2ITCM_RET_L2ITCM_BMSK                                       0x3
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_L2ITCM_RET_L2ITCM_SHFT                                         0

#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_SET_ADDR                                         (MSS_QDSP6V671SS_CSR_REG_BASE            + 0xc180)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_SET_RMSK                                            0x1ffff
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_SET_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_SET_ADDR,v)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_SET_CLADE2_BMSK                                     0x10000
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_SET_CLADE2_SHFT                                          16
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_SET_VPE_VU_BMSK                                      0x8000
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_SET_VPE_VU_SHFT                                          15
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_SET_NRET_BMSK                                        0x4000
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_SET_NRET_SHFT                                            14
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_SET_MNOC_BMSK                                        0x2000
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_SET_MNOC_SHFT                                            13
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_SET_APB_BMSK                                         0x1000
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_SET_APB_SHFT                                             12
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_SET_AXIS2_BMSK                                        0x800
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_SET_AXIS2_SHFT                                           11
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_SET_CSI1_BMSK                                         0x400
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_SET_CSI1_SHFT                                            10
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_SET_CSI0_BMSK                                         0x200
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_SET_CSI0_SHFT                                             9
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_SET_ARCH_BMSK                                         0x100
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_SET_ARCH_SHFT                                             8
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_SET_CORE_BMSK                                          0x80
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_SET_CORE_SHFT                                             7
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_SET_AXIM2_BMSK                                         0x40
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_SET_AXIM2_SHFT                                            6
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_SET_AXIS_BMSK                                          0x20
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_SET_AXIS_SHFT                                             5
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_SET_ATBM_BMSK                                          0x10
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_SET_ATBM_SHFT                                             4
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_SET_AHBM_BMSK                                           0x8
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_SET_AHBM_SHFT                                             3
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_SET_AXIM_BMSK                                           0x4
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_SET_AXIM_SHFT                                             2
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_SET_ISDB_ETM_BMSK                                       0x2
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_SET_ISDB_ETM_SHFT                                         1
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_SET_RET_BMSK                                            0x1
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_SET_RET_SHFT                                              0

#define HWIO_MSS_QDSP6SS_RSCCTL_EN_CLAMP_SET_ADDR                                        (MSS_QDSP6V671SS_CSR_REG_BASE            + 0xc184)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_CLAMP_SET_RMSK                                               0x3
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_CLAMP_SET_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_RSCCTL_EN_CLAMP_SET_ADDR,v)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_CLAMP_SET_CLAMP_MSS2CX_BMSK                                  0x2
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_CLAMP_SET_CLAMP_MSS2CX_SHFT                                    1
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_CLAMP_SET_CLAMP_IO_BMSK                                      0x1
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_CLAMP_SET_CLAMP_IO_SHFT                                        0

#define HWIO_MSS_QDSP6SS_RSCCTL_EN_CLK_SET_ADDR                                          (MSS_QDSP6V671SS_CSR_REG_BASE            + 0xc188)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_CLK_SET_RMSK                                               0x7ff
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_CLK_SET_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_RSCCTL_EN_CLK_SET_ADDR,v)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_CLK_SET_VPE_VU_BMSK                                        0x400
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_CLK_SET_VPE_VU_SHFT                                           10
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_CLK_SET_AXIS2_BMSK                                         0x200
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_CLK_SET_AXIS2_SHFT                                             9
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_CLK_SET_CSI1_BMSK                                          0x100
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_CLK_SET_CSI1_SHFT                                              8
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_CLK_SET_CSI0_BMSK                                           0x80
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_CLK_SET_CSI0_SHFT                                              7
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_CLK_SET_AXIM2_BMSK                                          0x40
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_CLK_SET_AXIM2_SHFT                                             6
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_CLK_SET_AXIS_BMSK                                           0x20
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_CLK_SET_AXIS_SHFT                                              5
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_CLK_SET_ATBM_BMSK                                           0x10
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_CLK_SET_ATBM_SHFT                                              4
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_CLK_SET_AHBM_BMSK                                            0x8
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_CLK_SET_AHBM_SHFT                                              3
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_CLK_SET_AXIM_BMSK                                            0x4
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_CLK_SET_AXIM_SHFT                                              2
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_CLK_SET_APB_BMSK                                             0x2
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_CLK_SET_APB_SHFT                                               1
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_CLK_SET_CORE_BMSK                                            0x1
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_CLK_SET_CORE_SHFT                                              0

#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_RET_SET_ADDR                                      (MSS_QDSP6V671SS_CSR_REG_BASE            + 0xc18c)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_RET_SET_RMSK                                          0xffff
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_RET_SET_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_RET_SET_ADDR,v)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_RET_SET_SPARE_BMSK                                    0xc000
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_RET_SET_SPARE_SHFT                                        14
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_RET_SET_L2CLADE_BMSK                                  0x2000
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_RET_SET_L2CLADE_SHFT                                      13
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_RET_SET_L2PLRU_BMSK                                   0x1000
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_RET_SET_L2PLRU_SHFT                                       12
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_RET_SET_L2TAG_BMSK                                     0x800
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_RET_SET_L2TAG_SHFT                                        11
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_RET_SET_L2STBUF_BMSK                                   0x400
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_RET_SET_L2STBUF_SHFT                                      10
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_RET_SET_L1DU_TAG_BMSK                                  0x200
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_RET_SET_L1DU_TAG_SHFT                                      9
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_RET_SET_L1DU_BMSK                                      0x100
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_RET_SET_L1DU_SHFT                                          8
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_RET_SET_JU_TLB64_BMSK                                   0x80
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_RET_SET_JU_TLB64_SHFT                                      7
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_RET_SET_JU_TLB128_BMSK                                  0x40
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_RET_SET_JU_TLB128_SHFT                                     6
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_RET_SET_JU_CAM_BMSK                                     0x20
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_RET_SET_JU_CAM_SHFT                                        5
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_RET_SET_ETB_BMSK                                        0x10
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_RET_SET_ETB_SHFT                                           4
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_RET_SET_L1IU_TAG_BMSK                                    0x8
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_RET_SET_L1IU_TAG_SHFT                                      3
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_RET_SET_L1IU_PDEC_BMSK                                   0x4
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_RET_SET_L1IU_PDEC_SHFT                                     2
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_RET_SET_L1IU_BMSK                                        0x2
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_RET_SET_L1IU_SHFT                                          1
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_RET_SET_L1IU_BTB_BMSK                                    0x1
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_RET_SET_L1IU_BTB_SHFT                                      0

#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_CTL_SET_ADDR                                      (MSS_QDSP6V671SS_CSR_REG_BASE            + 0xc190)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_CTL_SET_RMSK                                             0xa
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_CTL_SET_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_CTL_SET_ADDR,v)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_CTL_SET_CLAMP_QMC_MEM_BMSK                               0x8
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_CTL_SET_CLAMP_QMC_MEM_SHFT                                 3
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_CTL_SET_MEM_PERIPH_BMSK                                  0x2
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_CTL_SET_MEM_PERIPH_SHFT                                    1

#define HWIO_MSS_QDSP6SS_RSCCTL_EN_BHS_SET_ADDR                                          (MSS_QDSP6V671SS_CSR_REG_BASE            + 0xc194)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_BHS_SET_RMSK                                                 0x1
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_BHS_SET_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_RSCCTL_EN_BHS_SET_ADDR,v)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_BHS_SET_BHS_BMSK                                             0x1
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_BHS_SET_BHS_SHFT                                               0

#define HWIO_MSS_QDSP6SS_RSCCTL_EN_LDO_SET_ADDR                                          (MSS_QDSP6V671SS_CSR_REG_BASE            + 0xc198)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_LDO_SET_RMSK                                                0x7f
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_LDO_SET_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_RSCCTL_EN_LDO_SET_ADDR,v)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_LDO_SET_BG_OFF_BMSK                                         0x40
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_LDO_SET_BG_OFF_SHFT                                            6
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_LDO_SET_FORCEOFF_BMSK                                       0x20
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_LDO_SET_FORCEOFF_SHFT                                          5
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_LDO_SET_SPARE_BITS4_2_BMSK                                  0x1c
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_LDO_SET_SPARE_BITS4_2_SHFT                                     2
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_LDO_SET_UPDATE_BMSK                                          0x2
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_LDO_SET_UPDATE_SHFT                                            1
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_LDO_SET_BYPASS_OPEN_BMSK                                     0x1
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_LDO_SET_BYPASS_OPEN_SHFT                                       0

#define HWIO_MSS_QDSP6SS_RSCCTL_EN_EXT_SET_ADDR                                          (MSS_QDSP6V671SS_CSR_REG_BASE            + 0xc19c)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_EXT_SET_RMSK                                                0x1f
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_EXT_SET_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_RSCCTL_EN_EXT_SET_ADDR,v)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_EXT_SET_PLL_STBY_BMSK                                       0x10
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_EXT_SET_PLL_STBY_SHFT                                          4
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_EXT_SET_PLL_OUTDIS_BMSK                                      0x8
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_EXT_SET_PLL_OUTDIS_SHFT                                        3
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_EXT_SET_PLL_OPMODE1_BMSK                                     0x4
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_EXT_SET_PLL_OPMODE1_SHFT                                       2
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_EXT_SET_PLL_RESET_BMSK                                       0x2
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_EXT_SET_PLL_RESET_SHFT                                         1
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_EXT_SET_XO_ENABLE_BMSK                                       0x1
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_EXT_SET_XO_ENABLE_SHFT                                         0

#define HWIO_MSS_QDSP6SS_RSCCTL_EN_PLL_SET_ADDR                                          (MSS_QDSP6V671SS_CSR_REG_BASE            + 0xc1a0)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_PLL_SET_RMSK                                                0x17
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_PLL_SET_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_RSCCTL_EN_PLL_SET_ADDR,v)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_PLL_SET_PLL_STBY_BMSK                                       0x10
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_PLL_SET_PLL_STBY_SHFT                                          4
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_PLL_SET_PLL_OUTDIS_BMSK                                      0x4
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_PLL_SET_PLL_OUTDIS_SHFT                                        2
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_PLL_SET_PLL_OPMODE1_BMSK                                     0x2
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_PLL_SET_PLL_OPMODE1_SHFT                                       1
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_PLL_SET_PLL_RESET_BMSK                                       0x1
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_PLL_SET_PLL_RESET_SHFT                                         0

#define HWIO_MSS_QDSP6SS_RSCCTL_EN_STATERET_SET_ADDR                                     (MSS_QDSP6V671SS_CSR_REG_BASE            + 0xc1a4)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_STATERET_SET_RMSK                                            0xf
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_STATERET_SET_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_RSCCTL_EN_STATERET_SET_ADDR,v)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_STATERET_SET_PROF_IRQ_BMSK                                   0x8
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_STATERET_SET_PROF_IRQ_SHFT                                     3
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_STATERET_SET_WAKE_IRQ_BMSK                                   0x4
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_STATERET_SET_WAKE_IRQ_SHFT                                     2
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_STATERET_SET_RESTORE_BMSK                                    0x2
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_STATERET_SET_RESTORE_SHFT                                      1
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_STATERET_SET_SAVE_BMSK                                       0x1
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_STATERET_SET_SAVE_SHFT                                         0

#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CTL_SET_ADDR                                  (MSS_QDSP6V671SS_CSR_REG_BASE            + 0xc1a8)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CTL_SET_RMSK                                        0x3d
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CTL_SET_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CTL_SET_ADDR,v)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CTL_SET_AXIM2_FIFO_ARESET_BMSK                      0x20
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CTL_SET_AXIM2_FIFO_ARESET_SHFT                         5
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CTL_SET_AXIM2_HALTREQ_BMSK                          0x10
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CTL_SET_AXIM2_HALTREQ_SHFT                             4
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CTL_SET_AXIM_FIFO_ARESET_BMSK                        0x8
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CTL_SET_AXIM_FIFO_ARESET_SHFT                          3
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CTL_SET_AXIM_HALTREQ_BMSK                            0x4
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CTL_SET_AXIM_HALTREQ_SHFT                              2
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CTL_SET_BUSM_HALTREQ_BMSK                            0x1
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CTL_SET_BUSM_HALTREQ_SHFT                              0

#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CLAMP_SET_ADDR                                (MSS_QDSP6V671SS_CSR_REG_BASE            + 0xc1ac)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CLAMP_SET_RMSK                                      0xff
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CLAMP_SET_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CLAMP_SET_ADDR,v)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CLAMP_SET_AXIM2_CLAMP_E_CLEAR_BMSK                  0x80
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CLAMP_SET_AXIM2_CLAMP_E_CLEAR_SHFT                     7
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CLAMP_SET_AXIM2_CLAMP_E_SET_BMSK                    0x40
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CLAMP_SET_AXIM2_CLAMP_E_SET_SHFT                       6
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CLAMP_SET_AXIM2_CLAMP_L_CLEAR_BMSK                  0x20
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CLAMP_SET_AXIM2_CLAMP_L_CLEAR_SHFT                     5
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CLAMP_SET_AXIM2_CLAMP_L_SET_BMSK                    0x10
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CLAMP_SET_AXIM2_CLAMP_L_SET_SHFT                       4
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CLAMP_SET_AXIM_CLAMP_E_CLEAR_BMSK                    0x8
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CLAMP_SET_AXIM_CLAMP_E_CLEAR_SHFT                      3
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CLAMP_SET_AXIM_CLAMP_E_SET_BMSK                      0x4
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CLAMP_SET_AXIM_CLAMP_E_SET_SHFT                        2
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CLAMP_SET_AXIM_CLAMP_L_CLEAR_BMSK                    0x2
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CLAMP_SET_AXIM_CLAMP_L_CLEAR_SHFT                      1
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CLAMP_SET_AXIM_CLAMP_L_SET_BMSK                      0x1
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CLAMP_SET_AXIM_CLAMP_L_SET_SHFT                        0

#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CLK_SET_ADDR                                  (MSS_QDSP6V671SS_CSR_REG_BASE            + 0xc1b0)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CLK_SET_RMSK                                         0x7
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CLK_SET_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CLK_SET_ADDR,v)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CLK_SET_AXIM2_BMSK                                   0x4
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CLK_SET_AXIM2_SHFT                                     2
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CLK_SET_AXIM_BMSK                                    0x2
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CLK_SET_AXIM_SHFT                                      1
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CLK_SET_CORE_BMSK                                    0x1
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CLK_SET_CORE_SHFT                                      0

#define HWIO_MSS_QDSP6SS_RSCCTL_EN_L2MEM_RET_SET_ADDR                                    (MSS_QDSP6V671SS_CSR_REG_BASE            + 0xc1b4)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_L2MEM_RET_SET_RMSK                                           0xf
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_L2MEM_RET_SET_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_RSCCTL_EN_L2MEM_RET_SET_ADDR,v)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_L2MEM_RET_SET_L2DATA_BMSK                                    0xf
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_L2MEM_RET_SET_L2DATA_SHFT                                      0

#define HWIO_MSS_QDSP6SS_RSCCTL_EN_PRSC_SET_ADDR                                         (MSS_QDSP6V671SS_CSR_REG_BASE            + 0xc1b8)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_PRSC_SET_RMSK                                             0x7fff
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_PRSC_SET_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_RSCCTL_EN_PRSC_SET_ADDR,v)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_PRSC_SET_FAL10_VETO_BMSK                                  0x4000
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_PRSC_SET_FAL10_VETO_SHFT                                      14
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_PRSC_SET_SPARE_BMSK                                       0x3c00
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_PRSC_SET_SPARE_SHFT                                           10
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_PRSC_SET_TCS_HW_TRIGGER_BMSK                               0x3fc
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_PRSC_SET_TCS_HW_TRIGGER_SHFT                                   2
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_PRSC_SET_BRINGUP_REQ_BMSK                                    0x2
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_PRSC_SET_BRINGUP_REQ_SHFT                                      1
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_PRSC_SET_SHUTDOWN_REQ_BMSK                                   0x1
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_PRSC_SET_SHUTDOWN_REQ_SHFT                                     0

#define HWIO_MSS_QDSP6SS_RSCCTL_EN_L1SMEM_RET_SET_ADDR                                   (MSS_QDSP6V671SS_CSR_REG_BASE            + 0xc1c0)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_L1SMEM_RET_SET_RMSK                                       0x3f1f
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_L1SMEM_RET_SET_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_RSCCTL_EN_L1SMEM_RET_SET_ADDR,v)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_L1SMEM_RET_SET_L1S2DATA_BMSK                              0x3f00
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_L1SMEM_RET_SET_L1S2DATA_SHFT                                   8
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_L1SMEM_RET_SET_L1SDATA_BMSK                                 0x1f
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_L1SMEM_RET_SET_L1SDATA_SHFT                                    0

#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_STAGGER_SET_ADDR                                  (MSS_QDSP6V671SS_CSR_REG_BASE            + 0xc1c4)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_STAGGER_SET_RMSK                                         0x7
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_STAGGER_SET_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_STAGGER_SET_ADDR,v)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_STAGGER_SET_STAGGER_RESTORE_BMSK                         0x4
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_STAGGER_SET_STAGGER_RESTORE_SHFT                           2
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_STAGGER_SET_STAGGER_ALT_ARES_BMSK                        0x2
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_STAGGER_SET_STAGGER_ALT_ARES_SHFT                          1
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_STAGGER_SET_STAGGER_FUNC_ARES_BMSK                       0x1
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_STAGGER_SET_STAGGER_FUNC_ARES_SHFT                         0

#define HWIO_MSS_QDSP6SS_RSCCTL_EN_VTCMEM_RET_SET_ADDR                                   (MSS_QDSP6V671SS_CSR_REG_BASE            + 0xc1c8)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_VTCMEM_RET_SET_RMSK                                          0x1
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_VTCMEM_RET_SET_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_RSCCTL_EN_VTCMEM_RET_SET_ADDR,v)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_VTCMEM_RET_SET_VTCM_BMSK                                     0x1
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_VTCMEM_RET_SET_VTCM_SHFT                                       0

#define HWIO_MSS_QDSP6SS_RSCCTL_EN_L2ITCM_RET_SET_ADDR                                   (MSS_QDSP6V671SS_CSR_REG_BASE            + 0xc1cc)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_L2ITCM_RET_SET_RMSK                                          0x3
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_L2ITCM_RET_SET_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_RSCCTL_EN_L2ITCM_RET_SET_ADDR,v)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_L2ITCM_RET_SET_L2ITCM_BMSK                                   0x3
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_L2ITCM_RET_SET_L2ITCM_SHFT                                     0

#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_CLEAR_ADDR                                       (MSS_QDSP6V671SS_CSR_REG_BASE            + 0xc200)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_CLEAR_RMSK                                          0x1ffff
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_CLEAR_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_CLEAR_ADDR,v)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_CLEAR_CLADE2_BMSK                                   0x10000
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_CLEAR_CLADE2_SHFT                                        16
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_CLEAR_VPE_VU_BMSK                                    0x8000
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_CLEAR_VPE_VU_SHFT                                        15
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_CLEAR_NRET_BMSK                                      0x4000
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_CLEAR_NRET_SHFT                                          14
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_CLEAR_MNOC_BMSK                                      0x2000
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_CLEAR_MNOC_SHFT                                          13
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_CLEAR_APB_BMSK                                       0x1000
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_CLEAR_APB_SHFT                                           12
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_CLEAR_AXIS2_BMSK                                      0x800
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_CLEAR_AXIS2_SHFT                                         11
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_CLEAR_CSI1_BMSK                                       0x400
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_CLEAR_CSI1_SHFT                                          10
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_CLEAR_CSI0_BMSK                                       0x200
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_CLEAR_CSI0_SHFT                                           9
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_CLEAR_ARCH_BMSK                                       0x100
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_CLEAR_ARCH_SHFT                                           8
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_CLEAR_CORE_BMSK                                        0x80
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_CLEAR_CORE_SHFT                                           7
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_CLEAR_AXIM2_BMSK                                       0x40
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_CLEAR_AXIM2_SHFT                                          6
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_CLEAR_AXIS_BMSK                                        0x20
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_CLEAR_AXIS_SHFT                                           5
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_CLEAR_ATBM_BMSK                                        0x10
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_CLEAR_ATBM_SHFT                                           4
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_CLEAR_AHBM_BMSK                                         0x8
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_CLEAR_AHBM_SHFT                                           3
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_CLEAR_AXIM_BMSK                                         0x4
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_CLEAR_AXIM_SHFT                                           2
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_CLEAR_ISDB_ETM_BMSK                                     0x2
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_CLEAR_ISDB_ETM_SHFT                                       1
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_CLEAR_RET_BMSK                                          0x1
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ARES_CLEAR_RET_SHFT                                            0

#define HWIO_MSS_QDSP6SS_RSCCTL_EN_CLAMP_CLEAR_ADDR                                      (MSS_QDSP6V671SS_CSR_REG_BASE            + 0xc204)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_CLAMP_CLEAR_RMSK                                             0x3
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_CLAMP_CLEAR_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_RSCCTL_EN_CLAMP_CLEAR_ADDR,v)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_CLAMP_CLEAR_CLAMP_MSS2CX_BMSK                                0x2
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_CLAMP_CLEAR_CLAMP_MSS2CX_SHFT                                  1
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_CLAMP_CLEAR_CLAMP_IO_BMSK                                    0x1
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_CLAMP_CLEAR_CLAMP_IO_SHFT                                      0

#define HWIO_MSS_QDSP6SS_RSCCTL_EN_CLK_CLEAR_ADDR                                        (MSS_QDSP6V671SS_CSR_REG_BASE            + 0xc208)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_CLK_CLEAR_RMSK                                             0x7ff
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_CLK_CLEAR_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_RSCCTL_EN_CLK_CLEAR_ADDR,v)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_CLK_CLEAR_VPE_VU_BMSK                                      0x400
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_CLK_CLEAR_VPE_VU_SHFT                                         10
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_CLK_CLEAR_AXIS2_BMSK                                       0x200
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_CLK_CLEAR_AXIS2_SHFT                                           9
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_CLK_CLEAR_CSI1_BMSK                                        0x100
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_CLK_CLEAR_CSI1_SHFT                                            8
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_CLK_CLEAR_CSI0_BMSK                                         0x80
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_CLK_CLEAR_CSI0_SHFT                                            7
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_CLK_CLEAR_AXIM2_BMSK                                        0x40
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_CLK_CLEAR_AXIM2_SHFT                                           6
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_CLK_CLEAR_AXIS_BMSK                                         0x20
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_CLK_CLEAR_AXIS_SHFT                                            5
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_CLK_CLEAR_ATBM_BMSK                                         0x10
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_CLK_CLEAR_ATBM_SHFT                                            4
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_CLK_CLEAR_AHBM_BMSK                                          0x8
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_CLK_CLEAR_AHBM_SHFT                                            3
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_CLK_CLEAR_AXIM_BMSK                                          0x4
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_CLK_CLEAR_AXIM_SHFT                                            2
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_CLK_CLEAR_APB_BMSK                                           0x2
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_CLK_CLEAR_APB_SHFT                                             1
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_CLK_CLEAR_CORE_BMSK                                          0x1
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_CLK_CLEAR_CORE_SHFT                                            0

#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_RET_CLEAR_ADDR                                    (MSS_QDSP6V671SS_CSR_REG_BASE            + 0xc20c)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_RET_CLEAR_RMSK                                        0xffff
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_RET_CLEAR_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_RET_CLEAR_ADDR,v)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_RET_CLEAR_SPARE_BMSK                                  0xc000
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_RET_CLEAR_SPARE_SHFT                                      14
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_RET_CLEAR_L2CLADE_BMSK                                0x2000
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_RET_CLEAR_L2CLADE_SHFT                                    13
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_RET_CLEAR_L2PLRU_BMSK                                 0x1000
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_RET_CLEAR_L2PLRU_SHFT                                     12
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_RET_CLEAR_L2TAG_BMSK                                   0x800
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_RET_CLEAR_L2TAG_SHFT                                      11
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_RET_CLEAR_L2STBUF_BMSK                                 0x400
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_RET_CLEAR_L2STBUF_SHFT                                    10
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_RET_CLEAR_L1DU_TAG_BMSK                                0x200
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_RET_CLEAR_L1DU_TAG_SHFT                                    9
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_RET_CLEAR_L1DU_BMSK                                    0x100
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_RET_CLEAR_L1DU_SHFT                                        8
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_RET_CLEAR_JU_TLB64_BMSK                                 0x80
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_RET_CLEAR_JU_TLB64_SHFT                                    7
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_RET_CLEAR_JU_TLB128_BMSK                                0x40
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_RET_CLEAR_JU_TLB128_SHFT                                   6
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_RET_CLEAR_JU_CAM_BMSK                                   0x20
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_RET_CLEAR_JU_CAM_SHFT                                      5
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_RET_CLEAR_ETB_BMSK                                      0x10
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_RET_CLEAR_ETB_SHFT                                         4
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_RET_CLEAR_L1IU_TAG_BMSK                                  0x8
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_RET_CLEAR_L1IU_TAG_SHFT                                    3
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_RET_CLEAR_L1IU_PDEC_BMSK                                 0x4
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_RET_CLEAR_L1IU_PDEC_SHFT                                   2
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_RET_CLEAR_L1IU_BMSK                                      0x2
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_RET_CLEAR_L1IU_SHFT                                        1
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_RET_CLEAR_L1IU_BTB_BMSK                                  0x1
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_RET_CLEAR_L1IU_BTB_SHFT                                    0

#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_CTL_CLEAR_ADDR                                    (MSS_QDSP6V671SS_CSR_REG_BASE            + 0xc210)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_CTL_CLEAR_RMSK                                           0xa
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_CTL_CLEAR_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_CTL_CLEAR_ADDR,v)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_CTL_CLEAR_CLAMP_QMC_MEM_BMSK                             0x8
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_CTL_CLEAR_CLAMP_QMC_MEM_SHFT                               3
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_CTL_CLEAR_MEM_PERIPH_BMSK                                0x2
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_CTL_CLEAR_MEM_PERIPH_SHFT                                  1

#define HWIO_MSS_QDSP6SS_RSCCTL_EN_BHS_CLEAR_ADDR                                        (MSS_QDSP6V671SS_CSR_REG_BASE            + 0xc214)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_BHS_CLEAR_RMSK                                               0x1
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_BHS_CLEAR_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_RSCCTL_EN_BHS_CLEAR_ADDR,v)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_BHS_CLEAR_BHS_BMSK                                           0x1
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_BHS_CLEAR_BHS_SHFT                                             0

#define HWIO_MSS_QDSP6SS_RSCCTL_EN_LDO_CLEAR_ADDR                                        (MSS_QDSP6V671SS_CSR_REG_BASE            + 0xc218)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_LDO_CLEAR_RMSK                                              0x7f
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_LDO_CLEAR_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_RSCCTL_EN_LDO_CLEAR_ADDR,v)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_LDO_CLEAR_BG_OFF_BMSK                                       0x40
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_LDO_CLEAR_BG_OFF_SHFT                                          6
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_LDO_CLEAR_FORCEOFF_BMSK                                     0x20
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_LDO_CLEAR_FORCEOFF_SHFT                                        5
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_LDO_CLEAR_SPARE_BITS4_2_BMSK                                0x1c
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_LDO_CLEAR_SPARE_BITS4_2_SHFT                                   2
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_LDO_CLEAR_UPDATE_BMSK                                        0x2
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_LDO_CLEAR_UPDATE_SHFT                                          1
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_LDO_CLEAR_BYPASS_OPEN_BMSK                                   0x1
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_LDO_CLEAR_BYPASS_OPEN_SHFT                                     0

#define HWIO_MSS_QDSP6SS_RSCCTL_EN_EXT_CLEAR_ADDR                                        (MSS_QDSP6V671SS_CSR_REG_BASE            + 0xc21c)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_EXT_CLEAR_RMSK                                              0x1f
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_EXT_CLEAR_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_RSCCTL_EN_EXT_CLEAR_ADDR,v)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_EXT_CLEAR_PLL_STBY_BMSK                                     0x10
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_EXT_CLEAR_PLL_STBY_SHFT                                        4
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_EXT_CLEAR_PLL_OUTDIS_BMSK                                    0x8
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_EXT_CLEAR_PLL_OUTDIS_SHFT                                      3
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_EXT_CLEAR_PLL_OPMODE1_BMSK                                   0x4
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_EXT_CLEAR_PLL_OPMODE1_SHFT                                     2
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_EXT_CLEAR_PLL_RESET_BMSK                                     0x2
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_EXT_CLEAR_PLL_RESET_SHFT                                       1
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_EXT_CLEAR_XO_ENABLE_BMSK                                     0x1
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_EXT_CLEAR_XO_ENABLE_SHFT                                       0

#define HWIO_MSS_QDSP6SS_RSCCTL_EN_PLL_CLEAR_ADDR                                        (MSS_QDSP6V671SS_CSR_REG_BASE            + 0xc220)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_PLL_CLEAR_RMSK                                              0x17
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_PLL_CLEAR_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_RSCCTL_EN_PLL_CLEAR_ADDR,v)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_PLL_CLEAR_PLL_STBY_BMSK                                     0x10
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_PLL_CLEAR_PLL_STBY_SHFT                                        4
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_PLL_CLEAR_PLL_OUTDIS_BMSK                                    0x4
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_PLL_CLEAR_PLL_OUTDIS_SHFT                                      2
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_PLL_CLEAR_PLL_OPMODE1_BMSK                                   0x2
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_PLL_CLEAR_PLL_OPMODE1_SHFT                                     1
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_PLL_CLEAR_PLL_RESET_BMSK                                     0x1
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_PLL_CLEAR_PLL_RESET_SHFT                                       0

#define HWIO_MSS_QDSP6SS_RSCCTL_EN_STATERET_CLEAR_ADDR                                   (MSS_QDSP6V671SS_CSR_REG_BASE            + 0xc224)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_STATERET_CLEAR_RMSK                                          0xf
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_STATERET_CLEAR_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_RSCCTL_EN_STATERET_CLEAR_ADDR,v)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_STATERET_CLEAR_PROF_IRQ_BMSK                                 0x8
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_STATERET_CLEAR_PROF_IRQ_SHFT                                   3
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_STATERET_CLEAR_WAKE_IRQ_BMSK                                 0x4
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_STATERET_CLEAR_WAKE_IRQ_SHFT                                   2
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_STATERET_CLEAR_RESTORE_BMSK                                  0x2
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_STATERET_CLEAR_RESTORE_SHFT                                    1
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_STATERET_CLEAR_SAVE_BMSK                                     0x1
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_STATERET_CLEAR_SAVE_SHFT                                       0

#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CTL_CLEAR_ADDR                                (MSS_QDSP6V671SS_CSR_REG_BASE            + 0xc228)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CTL_CLEAR_RMSK                                      0x3d
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CTL_CLEAR_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CTL_CLEAR_ADDR,v)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CTL_CLEAR_AXIM2_FIFO_ARESET_BMSK                    0x20
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CTL_CLEAR_AXIM2_FIFO_ARESET_SHFT                       5
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CTL_CLEAR_AXIM2_HALTREQ_BMSK                        0x10
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CTL_CLEAR_AXIM2_HALTREQ_SHFT                           4
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CTL_CLEAR_AXIM_FIFO_ARESET_BMSK                      0x8
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CTL_CLEAR_AXIM_FIFO_ARESET_SHFT                        3
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CTL_CLEAR_AXIM_HALTREQ_BMSK                          0x4
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CTL_CLEAR_AXIM_HALTREQ_SHFT                            2
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CTL_CLEAR_BUSM_HALTREQ_BMSK                          0x1
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CTL_CLEAR_BUSM_HALTREQ_SHFT                            0

#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CLAMP_CLEAR_ADDR                              (MSS_QDSP6V671SS_CSR_REG_BASE            + 0xc22c)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CLAMP_CLEAR_RMSK                                    0xff
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CLAMP_CLEAR_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CLAMP_CLEAR_ADDR,v)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CLAMP_CLEAR_AXIM2_CLAMP_E_CLEAR_BMSK                0x80
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CLAMP_CLEAR_AXIM2_CLAMP_E_CLEAR_SHFT                   7
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CLAMP_CLEAR_AXIM2_CLAMP_E_SET_BMSK                  0x40
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CLAMP_CLEAR_AXIM2_CLAMP_E_SET_SHFT                     6
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CLAMP_CLEAR_AXIM2_CLAMP_L_CLEAR_BMSK                0x20
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CLAMP_CLEAR_AXIM2_CLAMP_L_CLEAR_SHFT                   5
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CLAMP_CLEAR_AXIM2_CLAMP_L_SET_BMSK                  0x10
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CLAMP_CLEAR_AXIM2_CLAMP_L_SET_SHFT                     4
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CLAMP_CLEAR_AXIM_CLAMP_E_CLEAR_BMSK                  0x8
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CLAMP_CLEAR_AXIM_CLAMP_E_CLEAR_SHFT                    3
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CLAMP_CLEAR_AXIM_CLAMP_E_SET_BMSK                    0x4
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CLAMP_CLEAR_AXIM_CLAMP_E_SET_SHFT                      2
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CLAMP_CLEAR_AXIM_CLAMP_L_CLEAR_BMSK                  0x2
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CLAMP_CLEAR_AXIM_CLAMP_L_CLEAR_SHFT                    1
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CLAMP_CLEAR_AXIM_CLAMP_L_SET_BMSK                    0x1
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CLAMP_CLEAR_AXIM_CLAMP_L_SET_SHFT                      0

#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CLK_CLEAR_ADDR                                (MSS_QDSP6V671SS_CSR_REG_BASE            + 0xc230)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CLK_CLEAR_RMSK                                       0x7
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CLK_CLEAR_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CLK_CLEAR_ADDR,v)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CLK_CLEAR_AXIM2_BMSK                                 0x4
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CLK_CLEAR_AXIM2_SHFT                                   2
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CLK_CLEAR_AXIM_BMSK                                  0x2
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CLK_CLEAR_AXIM_SHFT                                    1
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CLK_CLEAR_CORE_BMSK                                  0x1
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_ISOINTF_CLK_CLEAR_CORE_SHFT                                    0

#define HWIO_MSS_QDSP6SS_RSCCTL_EN_L2MEM_RET_CLEAR_ADDR                                  (MSS_QDSP6V671SS_CSR_REG_BASE            + 0xc234)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_L2MEM_RET_CLEAR_RMSK                                         0xf
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_L2MEM_RET_CLEAR_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_RSCCTL_EN_L2MEM_RET_CLEAR_ADDR,v)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_L2MEM_RET_CLEAR_L2DATA_BMSK                                  0xf
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_L2MEM_RET_CLEAR_L2DATA_SHFT                                    0

#define HWIO_MSS_QDSP6SS_RSCCTL_EN_PRSC_CLEAR_ADDR                                       (MSS_QDSP6V671SS_CSR_REG_BASE            + 0xc238)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_PRSC_CLEAR_RMSK                                           0x7fff
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_PRSC_CLEAR_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_RSCCTL_EN_PRSC_CLEAR_ADDR,v)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_PRSC_CLEAR_FAL10_VETO_BMSK                                0x4000
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_PRSC_CLEAR_FAL10_VETO_SHFT                                    14
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_PRSC_CLEAR_SPARE_BMSK                                     0x3c00
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_PRSC_CLEAR_SPARE_SHFT                                         10
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_PRSC_CLEAR_TCS_HW_TRIGGER_BMSK                             0x3fc
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_PRSC_CLEAR_TCS_HW_TRIGGER_SHFT                                 2
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_PRSC_CLEAR_BRINGUP_REQ_BMSK                                  0x2
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_PRSC_CLEAR_BRINGUP_REQ_SHFT                                    1
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_PRSC_CLEAR_SHUTDOWN_REQ_BMSK                                 0x1
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_PRSC_CLEAR_SHUTDOWN_REQ_SHFT                                   0

#define HWIO_MSS_QDSP6SS_RSCCTL_EN_L1SMEM_RET_CLEAR_ADDR                                 (MSS_QDSP6V671SS_CSR_REG_BASE            + 0xc240)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_L1SMEM_RET_CLEAR_RMSK                                     0x3f1f
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_L1SMEM_RET_CLEAR_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_RSCCTL_EN_L1SMEM_RET_CLEAR_ADDR,v)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_L1SMEM_RET_CLEAR_L1S2DATA_BMSK                            0x3f00
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_L1SMEM_RET_CLEAR_L1S2DATA_SHFT                                 8
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_L1SMEM_RET_CLEAR_L1SDATA_BMSK                               0x1f
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_L1SMEM_RET_CLEAR_L1SDATA_SHFT                                  0

#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_STAGGER_CLEAR_ADDR                                (MSS_QDSP6V671SS_CSR_REG_BASE            + 0xc244)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_STAGGER_CLEAR_RMSK                                       0x7
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_STAGGER_CLEAR_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_STAGGER_CLEAR_ADDR,v)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_STAGGER_CLEAR_STAGGER_RESTORE_BMSK                       0x4
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_STAGGER_CLEAR_STAGGER_RESTORE_SHFT                         2
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_STAGGER_CLEAR_STAGGER_ALT_ARES_BMSK                      0x2
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_STAGGER_CLEAR_STAGGER_ALT_ARES_SHFT                        1
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_STAGGER_CLEAR_STAGGER_FUNC_ARES_BMSK                     0x1
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_MEM_STAGGER_CLEAR_STAGGER_FUNC_ARES_SHFT                       0

#define HWIO_MSS_QDSP6SS_RSCCTL_EN_VTCMEM_RET_CLEAR_ADDR                                 (MSS_QDSP6V671SS_CSR_REG_BASE            + 0xc248)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_VTCMEM_RET_CLEAR_RMSK                                        0x1
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_VTCMEM_RET_CLEAR_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_RSCCTL_EN_VTCMEM_RET_CLEAR_ADDR,v)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_VTCMEM_RET_CLEAR_VTCM_BMSK                                   0x1
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_VTCMEM_RET_CLEAR_VTCM_SHFT                                     0

#define HWIO_MSS_QDSP6SS_RSCCTL_EN_L2ITCM_RET_CLEAR_ADDR                                 (MSS_QDSP6V671SS_CSR_REG_BASE            + 0xc24c)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_L2ITCM_RET_CLEAR_RMSK                                        0x3
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_L2ITCM_RET_CLEAR_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_RSCCTL_EN_L2ITCM_RET_CLEAR_ADDR,v)
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_L2ITCM_RET_CLEAR_L2ITCM_BMSK                                 0x3
#define HWIO_MSS_QDSP6SS_RSCCTL_EN_L2ITCM_RET_CLEAR_L2ITCM_SHFT                                   0

/*----------------------------------------------------------------------------
 * MODULE: MSS_QDSP6V671SS_PUB
 *--------------------------------------------------------------------------*/

#define MSS_QDSP6V671SS_PUB_REG_BASE                                              (MSS_TOP_BASE            + 0x00080000)
#define MSS_QDSP6V671SS_PUB_REG_BASE_SIZE                                         0x10000
#define MSS_QDSP6V671SS_PUB_REG_BASE_USED                                         0x204c

#define HWIO_MSS_QDSP6SS_VERSION_ADDR                                             (MSS_QDSP6V671SS_PUB_REG_BASE            + 0x0)
#define HWIO_MSS_QDSP6SS_VERSION_RMSK                                             0xffffffff
#define HWIO_MSS_QDSP6SS_VERSION_IN                    \
                in_dword(HWIO_MSS_QDSP6SS_VERSION_ADDR)
#define HWIO_MSS_QDSP6SS_VERSION_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_VERSION_ADDR, m)
#define HWIO_MSS_QDSP6SS_VERSION_MAJOR_BMSK                                       0xf0000000
#define HWIO_MSS_QDSP6SS_VERSION_MAJOR_SHFT                                               28
#define HWIO_MSS_QDSP6SS_VERSION_MINOR_BMSK                                        0xfff0000
#define HWIO_MSS_QDSP6SS_VERSION_MINOR_SHFT                                               16
#define HWIO_MSS_QDSP6SS_VERSION_STEP_BMSK                                            0xffff
#define HWIO_MSS_QDSP6SS_VERSION_STEP_SHFT                                                 0

#define HWIO_MSS_QDSP6SS_RST_EVB_ADDR                                             (MSS_QDSP6V671SS_PUB_REG_BASE            + 0x10)
#define HWIO_MSS_QDSP6SS_RST_EVB_RMSK                                              0xffffff0
#define HWIO_MSS_QDSP6SS_RST_EVB_IN                    \
                in_dword(HWIO_MSS_QDSP6SS_RST_EVB_ADDR)
#define HWIO_MSS_QDSP6SS_RST_EVB_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_RST_EVB_ADDR, m)
#define HWIO_MSS_QDSP6SS_RST_EVB_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_RST_EVB_ADDR,v)
#define HWIO_MSS_QDSP6SS_RST_EVB_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_QDSP6SS_RST_EVB_ADDR,m,v,HWIO_MSS_QDSP6SS_RST_EVB_IN)
#define HWIO_MSS_QDSP6SS_RST_EVB_EVB_BMSK                                          0xffffff0
#define HWIO_MSS_QDSP6SS_RST_EVB_EVB_SHFT                                                  4

#define HWIO_MSS_QDSP6SS_CLADE2_ARES_CFG_ADDR                                     (MSS_QDSP6V671SS_PUB_REG_BASE            + 0x14)
#define HWIO_MSS_QDSP6SS_CLADE2_ARES_CFG_RMSK                                            0x1
#define HWIO_MSS_QDSP6SS_CLADE2_ARES_CFG_IN                    \
                in_dword(HWIO_MSS_QDSP6SS_CLADE2_ARES_CFG_ADDR)
#define HWIO_MSS_QDSP6SS_CLADE2_ARES_CFG_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_CLADE2_ARES_CFG_ADDR, m)
#define HWIO_MSS_QDSP6SS_CLADE2_ARES_CFG_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_CLADE2_ARES_CFG_ADDR,v)
#define HWIO_MSS_QDSP6SS_CLADE2_ARES_CFG_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_QDSP6SS_CLADE2_ARES_CFG_ADDR,m,v,HWIO_MSS_QDSP6SS_CLADE2_ARES_CFG_IN)
#define HWIO_MSS_QDSP6SS_CLADE2_ARES_CFG_CLADE2_ARES_ENA_BMSK                            0x1
#define HWIO_MSS_QDSP6SS_CLADE2_ARES_CFG_CLADE2_ARES_ENA_SHFT                              0

#define HWIO_MSS_QDSP6SS_DBG_CFG_ADDR                                             (MSS_QDSP6V671SS_PUB_REG_BASE            + 0x18)
#define HWIO_MSS_QDSP6SS_DBG_CFG_RMSK                                             0xffff0002
#define HWIO_MSS_QDSP6SS_DBG_CFG_IN                    \
                in_dword(HWIO_MSS_QDSP6SS_DBG_CFG_ADDR)
#define HWIO_MSS_QDSP6SS_DBG_CFG_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_DBG_CFG_ADDR, m)
#define HWIO_MSS_QDSP6SS_DBG_CFG_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_DBG_CFG_ADDR,v)
#define HWIO_MSS_QDSP6SS_DBG_CFG_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_QDSP6SS_DBG_CFG_ADDR,m,v,HWIO_MSS_QDSP6SS_DBG_CFG_IN)
#define HWIO_MSS_QDSP6SS_DBG_CFG_DBG_SW_REG_BMSK                                  0xff000000
#define HWIO_MSS_QDSP6SS_DBG_CFG_DBG_SW_REG_SHFT                                          24
#define HWIO_MSS_QDSP6SS_DBG_CFG_DBG_SPARE_BMSK                                     0xff0000
#define HWIO_MSS_QDSP6SS_DBG_CFG_DBG_SPARE_SHFT                                           16
#define HWIO_MSS_QDSP6SS_DBG_CFG_CTI_TIHS_ENA_BMSK                                       0x2
#define HWIO_MSS_QDSP6SS_DBG_CFG_CTI_TIHS_ENA_SHFT                                         1

#define HWIO_MSS_QDSP6SS_RET_CFG_ADDR                                             (MSS_QDSP6V671SS_PUB_REG_BASE            + 0x1c)
#define HWIO_MSS_QDSP6SS_RET_CFG_RMSK                                                    0x3
#define HWIO_MSS_QDSP6SS_RET_CFG_IN                    \
                in_dword(HWIO_MSS_QDSP6SS_RET_CFG_ADDR)
#define HWIO_MSS_QDSP6SS_RET_CFG_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_RET_CFG_ADDR, m)
#define HWIO_MSS_QDSP6SS_RET_CFG_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_RET_CFG_ADDR,v)
#define HWIO_MSS_QDSP6SS_RET_CFG_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_QDSP6SS_RET_CFG_ADDR,m,v,HWIO_MSS_QDSP6SS_RET_CFG_IN)
#define HWIO_MSS_QDSP6SS_RET_CFG_NRET_ARES_ENA_BMSK                                      0x2
#define HWIO_MSS_QDSP6SS_RET_CFG_NRET_ARES_ENA_SHFT                                        1
#define HWIO_MSS_QDSP6SS_RET_CFG_RET_ARES_ENA_BMSK                                       0x1
#define HWIO_MSS_QDSP6SS_RET_CFG_RET_ARES_ENA_SHFT                                         0

#define HWIO_MSS_QDSP6SS_CORE_CBCR_ADDR                                           (MSS_QDSP6V671SS_PUB_REG_BASE            + 0x20)
#define HWIO_MSS_QDSP6SS_CORE_CBCR_RMSK                                           0x80000001
#define HWIO_MSS_QDSP6SS_CORE_CBCR_IN                    \
                in_dword(HWIO_MSS_QDSP6SS_CORE_CBCR_ADDR)
#define HWIO_MSS_QDSP6SS_CORE_CBCR_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_CORE_CBCR_ADDR, m)
#define HWIO_MSS_QDSP6SS_CORE_CBCR_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_CORE_CBCR_ADDR,v)
#define HWIO_MSS_QDSP6SS_CORE_CBCR_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_QDSP6SS_CORE_CBCR_ADDR,m,v,HWIO_MSS_QDSP6SS_CORE_CBCR_IN)
#define HWIO_MSS_QDSP6SS_CORE_CBCR_CLKOFF_BMSK                                    0x80000000
#define HWIO_MSS_QDSP6SS_CORE_CBCR_CLKOFF_SHFT                                            31
#define HWIO_MSS_QDSP6SS_CORE_CBCR_CLKEN_BMSK                                            0x1
#define HWIO_MSS_QDSP6SS_CORE_CBCR_CLKEN_SHFT                                              0

#define HWIO_MSS_QDSP6SS_CORE_CMD_RCGR_ADDR                                       (MSS_QDSP6V671SS_PUB_REG_BASE            + 0x28)
#define HWIO_MSS_QDSP6SS_CORE_CMD_RCGR_RMSK                                       0x80000013
#define HWIO_MSS_QDSP6SS_CORE_CMD_RCGR_IN                    \
                in_dword(HWIO_MSS_QDSP6SS_CORE_CMD_RCGR_ADDR)
#define HWIO_MSS_QDSP6SS_CORE_CMD_RCGR_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_CORE_CMD_RCGR_ADDR, m)
#define HWIO_MSS_QDSP6SS_CORE_CMD_RCGR_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_CORE_CMD_RCGR_ADDR,v)
#define HWIO_MSS_QDSP6SS_CORE_CMD_RCGR_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_QDSP6SS_CORE_CMD_RCGR_ADDR,m,v,HWIO_MSS_QDSP6SS_CORE_CMD_RCGR_IN)
#define HWIO_MSS_QDSP6SS_CORE_CMD_RCGR_ROOT_OFF_BMSK                              0x80000000
#define HWIO_MSS_QDSP6SS_CORE_CMD_RCGR_ROOT_OFF_SHFT                                      31
#define HWIO_MSS_QDSP6SS_CORE_CMD_RCGR_DIRTY_CFG_RCGR_BMSK                              0x10
#define HWIO_MSS_QDSP6SS_CORE_CMD_RCGR_DIRTY_CFG_RCGR_SHFT                                 4
#define HWIO_MSS_QDSP6SS_CORE_CMD_RCGR_ROOT_EN_BMSK                                      0x2
#define HWIO_MSS_QDSP6SS_CORE_CMD_RCGR_ROOT_EN_SHFT                                        1
#define HWIO_MSS_QDSP6SS_CORE_CMD_RCGR_UPDATE_BMSK                                       0x1
#define HWIO_MSS_QDSP6SS_CORE_CMD_RCGR_UPDATE_SHFT                                         0

#define HWIO_MSS_QDSP6SS_CORE_CFG_RCGR_ADDR                                       (MSS_QDSP6V671SS_PUB_REG_BASE            + 0x2c)
#define HWIO_MSS_QDSP6SS_CORE_CFG_RCGR_RMSK                                         0x11071f
#define HWIO_MSS_QDSP6SS_CORE_CFG_RCGR_IN                    \
                in_dword(HWIO_MSS_QDSP6SS_CORE_CFG_RCGR_ADDR)
#define HWIO_MSS_QDSP6SS_CORE_CFG_RCGR_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_CORE_CFG_RCGR_ADDR, m)
#define HWIO_MSS_QDSP6SS_CORE_CFG_RCGR_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_CORE_CFG_RCGR_ADDR,v)
#define HWIO_MSS_QDSP6SS_CORE_CFG_RCGR_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_QDSP6SS_CORE_CFG_RCGR_ADDR,m,v,HWIO_MSS_QDSP6SS_CORE_CFG_RCGR_IN)
#define HWIO_MSS_QDSP6SS_CORE_CFG_RCGR_HW_CLK_CONTROL_BMSK                          0x100000
#define HWIO_MSS_QDSP6SS_CORE_CFG_RCGR_HW_CLK_CONTROL_SHFT                                20
#define HWIO_MSS_QDSP6SS_CORE_CFG_RCGR_RCGLITE_DISABLE_BMSK                          0x10000
#define HWIO_MSS_QDSP6SS_CORE_CFG_RCGR_RCGLITE_DISABLE_SHFT                               16
#define HWIO_MSS_QDSP6SS_CORE_CFG_RCGR_SRC_SEL_BMSK                                    0x700
#define HWIO_MSS_QDSP6SS_CORE_CFG_RCGR_SRC_SEL_SHFT                                        8
#define HWIO_MSS_QDSP6SS_CORE_CFG_RCGR_SRC_DIV_BMSK                                     0x1f
#define HWIO_MSS_QDSP6SS_CORE_CFG_RCGR_SRC_DIV_SHFT                                        0

#define HWIO_MSS_QDSP6SS_XO_CBCR_ADDR                                             (MSS_QDSP6V671SS_PUB_REG_BASE            + 0x38)
#define HWIO_MSS_QDSP6SS_XO_CBCR_RMSK                                             0x80000001
#define HWIO_MSS_QDSP6SS_XO_CBCR_IN                    \
                in_dword(HWIO_MSS_QDSP6SS_XO_CBCR_ADDR)
#define HWIO_MSS_QDSP6SS_XO_CBCR_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_XO_CBCR_ADDR, m)
#define HWIO_MSS_QDSP6SS_XO_CBCR_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_XO_CBCR_ADDR,v)
#define HWIO_MSS_QDSP6SS_XO_CBCR_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_QDSP6SS_XO_CBCR_ADDR,m,v,HWIO_MSS_QDSP6SS_XO_CBCR_IN)
#define HWIO_MSS_QDSP6SS_XO_CBCR_CLKOFF_BMSK                                      0x80000000
#define HWIO_MSS_QDSP6SS_XO_CBCR_CLKOFF_SHFT                                              31
#define HWIO_MSS_QDSP6SS_XO_CBCR_CLKEN_BMSK                                              0x1
#define HWIO_MSS_QDSP6SS_XO_CBCR_CLKEN_SHFT                                                0

#define HWIO_MSS_QDSP6SS_SLEEP_CBCR_ADDR                                          (MSS_QDSP6V671SS_PUB_REG_BASE            + 0x3c)
#define HWIO_MSS_QDSP6SS_SLEEP_CBCR_RMSK                                          0x80000001
#define HWIO_MSS_QDSP6SS_SLEEP_CBCR_IN                    \
                in_dword(HWIO_MSS_QDSP6SS_SLEEP_CBCR_ADDR)
#define HWIO_MSS_QDSP6SS_SLEEP_CBCR_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_SLEEP_CBCR_ADDR, m)
#define HWIO_MSS_QDSP6SS_SLEEP_CBCR_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_SLEEP_CBCR_ADDR,v)
#define HWIO_MSS_QDSP6SS_SLEEP_CBCR_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_QDSP6SS_SLEEP_CBCR_ADDR,m,v,HWIO_MSS_QDSP6SS_SLEEP_CBCR_IN)
#define HWIO_MSS_QDSP6SS_SLEEP_CBCR_CLKOFF_BMSK                                   0x80000000
#define HWIO_MSS_QDSP6SS_SLEEP_CBCR_CLKOFF_SHFT                                           31
#define HWIO_MSS_QDSP6SS_SLEEP_CBCR_CLKEN_BMSK                                           0x1
#define HWIO_MSS_QDSP6SS_SLEEP_CBCR_CLKEN_SHFT                                             0

#define HWIO_MSS_QDSP6SS_NMI_ADDR                                                 (MSS_QDSP6V671SS_PUB_REG_BASE            + 0x40)
#define HWIO_MSS_QDSP6SS_NMI_RMSK                                                        0x3
#define HWIO_MSS_QDSP6SS_NMI_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_NMI_ADDR,v)
#define HWIO_MSS_QDSP6SS_NMI_CLEAR_STATUS_BMSK                                           0x2
#define HWIO_MSS_QDSP6SS_NMI_CLEAR_STATUS_SHFT                                             1
#define HWIO_MSS_QDSP6SS_NMI_SET_NMI_BMSK                                                0x1
#define HWIO_MSS_QDSP6SS_NMI_SET_NMI_SHFT                                                  0

#define HWIO_MSS_QDSP6SS_NMI_STATUS_ADDR                                          (MSS_QDSP6V671SS_PUB_REG_BASE            + 0x44)
#define HWIO_MSS_QDSP6SS_NMI_STATUS_RMSK                                                 0x7
#define HWIO_MSS_QDSP6SS_NMI_STATUS_IN                    \
                in_dword(HWIO_MSS_QDSP6SS_NMI_STATUS_ADDR)
#define HWIO_MSS_QDSP6SS_NMI_STATUS_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_NMI_STATUS_ADDR, m)
#define HWIO_MSS_QDSP6SS_NMI_STATUS_EXT_DBG_TRIG_BMSK                                    0x4
#define HWIO_MSS_QDSP6SS_NMI_STATUS_EXT_DBG_TRIG_SHFT                                      2
#define HWIO_MSS_QDSP6SS_NMI_STATUS_WDOG_TRIG_BMSK                                       0x2
#define HWIO_MSS_QDSP6SS_NMI_STATUS_WDOG_TRIG_SHFT                                         1
#define HWIO_MSS_QDSP6SS_NMI_STATUS_PUBCSR_TRIG_BMSK                                     0x1
#define HWIO_MSS_QDSP6SS_NMI_STATUS_PUBCSR_TRIG_SHFT                                       0

#define HWIO_MSS_QDSP6SS_SPDM_MON_CBCR_ADDR                                       (MSS_QDSP6V671SS_PUB_REG_BASE            + 0x48)
#define HWIO_MSS_QDSP6SS_SPDM_MON_CBCR_RMSK                                       0x80000001
#define HWIO_MSS_QDSP6SS_SPDM_MON_CBCR_IN                    \
                in_dword(HWIO_MSS_QDSP6SS_SPDM_MON_CBCR_ADDR)
#define HWIO_MSS_QDSP6SS_SPDM_MON_CBCR_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_SPDM_MON_CBCR_ADDR, m)
#define HWIO_MSS_QDSP6SS_SPDM_MON_CBCR_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_SPDM_MON_CBCR_ADDR,v)
#define HWIO_MSS_QDSP6SS_SPDM_MON_CBCR_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_QDSP6SS_SPDM_MON_CBCR_ADDR,m,v,HWIO_MSS_QDSP6SS_SPDM_MON_CBCR_IN)
#define HWIO_MSS_QDSP6SS_SPDM_MON_CBCR_CLKOFF_BMSK                                0x80000000
#define HWIO_MSS_QDSP6SS_SPDM_MON_CBCR_CLKOFF_SHFT                                        31
#define HWIO_MSS_QDSP6SS_SPDM_MON_CBCR_CLKEN_BMSK                                        0x1
#define HWIO_MSS_QDSP6SS_SPDM_MON_CBCR_CLKEN_SHFT                                          0

#define HWIO_MSS_QDSP6SS_INTF_HALTREQ_ADDR                                        (MSS_QDSP6V671SS_PUB_REG_BASE            + 0x88)
#define HWIO_MSS_QDSP6SS_INTF_HALTREQ_RMSK                                               0x7
#define HWIO_MSS_QDSP6SS_INTF_HALTREQ_IN                    \
                in_dword(HWIO_MSS_QDSP6SS_INTF_HALTREQ_ADDR)
#define HWIO_MSS_QDSP6SS_INTF_HALTREQ_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_INTF_HALTREQ_ADDR, m)
#define HWIO_MSS_QDSP6SS_INTF_HALTREQ_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_INTF_HALTREQ_ADDR,v)
#define HWIO_MSS_QDSP6SS_INTF_HALTREQ_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_QDSP6SS_INTF_HALTREQ_ADDR,m,v,HWIO_MSS_QDSP6SS_INTF_HALTREQ_IN)
#define HWIO_MSS_QDSP6SS_INTF_HALTREQ_AXIM2_BMSK                                         0x4
#define HWIO_MSS_QDSP6SS_INTF_HALTREQ_AXIM2_SHFT                                           2
#define HWIO_MSS_QDSP6SS_INTF_HALTREQ_AXIM_BMSK                                          0x2
#define HWIO_MSS_QDSP6SS_INTF_HALTREQ_AXIM_SHFT                                            1
#define HWIO_MSS_QDSP6SS_INTF_HALTREQ_ALL_BMSK                                           0x1
#define HWIO_MSS_QDSP6SS_INTF_HALTREQ_ALL_SHFT                                             0

#define HWIO_MSS_QDSP6SS_INTF_HALTACK_ADDR                                        (MSS_QDSP6V671SS_PUB_REG_BASE            + 0x8c)
#define HWIO_MSS_QDSP6SS_INTF_HALTACK_RMSK                                               0x7
#define HWIO_MSS_QDSP6SS_INTF_HALTACK_IN                    \
                in_dword(HWIO_MSS_QDSP6SS_INTF_HALTACK_ADDR)
#define HWIO_MSS_QDSP6SS_INTF_HALTACK_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_INTF_HALTACK_ADDR, m)
#define HWIO_MSS_QDSP6SS_INTF_HALTACK_AXIM2_BMSK                                         0x4
#define HWIO_MSS_QDSP6SS_INTF_HALTACK_AXIM2_SHFT                                           2
#define HWIO_MSS_QDSP6SS_INTF_HALTACK_AXIM_BMSK                                          0x2
#define HWIO_MSS_QDSP6SS_INTF_HALTACK_AXIM_SHFT                                            1
#define HWIO_MSS_QDSP6SS_INTF_HALTACK_ALL_BMSK                                           0x1
#define HWIO_MSS_QDSP6SS_INTF_HALTACK_ALL_SHFT                                             0

#define HWIO_MSS_QDSP6SS_INTFCLAMP_SET_ADDR                                       (MSS_QDSP6V671SS_PUB_REG_BASE            + 0x90)
#define HWIO_MSS_QDSP6SS_INTFCLAMP_SET_RMSK                                              0xf
#define HWIO_MSS_QDSP6SS_INTFCLAMP_SET_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_INTFCLAMP_SET_ADDR,v)
#define HWIO_MSS_QDSP6SS_INTFCLAMP_SET_AXIM2_CLAMP_E_BMSK                                0x8
#define HWIO_MSS_QDSP6SS_INTFCLAMP_SET_AXIM2_CLAMP_E_SHFT                                  3
#define HWIO_MSS_QDSP6SS_INTFCLAMP_SET_AXIM2_CLAMP_L_BMSK                                0x4
#define HWIO_MSS_QDSP6SS_INTFCLAMP_SET_AXIM2_CLAMP_L_SHFT                                  2
#define HWIO_MSS_QDSP6SS_INTFCLAMP_SET_AXIM_CLAMP_E_BMSK                                 0x2
#define HWIO_MSS_QDSP6SS_INTFCLAMP_SET_AXIM_CLAMP_E_SHFT                                   1
#define HWIO_MSS_QDSP6SS_INTFCLAMP_SET_AXIM_CLAMP_L_BMSK                                 0x1
#define HWIO_MSS_QDSP6SS_INTFCLAMP_SET_AXIM_CLAMP_L_SHFT                                   0

#define HWIO_MSS_QDSP6SS_INTFCLAMP_CLEAR_ADDR                                     (MSS_QDSP6V671SS_PUB_REG_BASE            + 0x94)
#define HWIO_MSS_QDSP6SS_INTFCLAMP_CLEAR_RMSK                                            0xf
#define HWIO_MSS_QDSP6SS_INTFCLAMP_CLEAR_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_INTFCLAMP_CLEAR_ADDR,v)
#define HWIO_MSS_QDSP6SS_INTFCLAMP_CLEAR_AXIM2_CLAMP_E_BMSK                              0x8
#define HWIO_MSS_QDSP6SS_INTFCLAMP_CLEAR_AXIM2_CLAMP_E_SHFT                                3
#define HWIO_MSS_QDSP6SS_INTFCLAMP_CLEAR_AXIM2_CLAMP_L_BMSK                              0x4
#define HWIO_MSS_QDSP6SS_INTFCLAMP_CLEAR_AXIM2_CLAMP_L_SHFT                                2
#define HWIO_MSS_QDSP6SS_INTFCLAMP_CLEAR_AXIM_CLAMP_E_BMSK                               0x2
#define HWIO_MSS_QDSP6SS_INTFCLAMP_CLEAR_AXIM_CLAMP_E_SHFT                                 1
#define HWIO_MSS_QDSP6SS_INTFCLAMP_CLEAR_AXIM_CLAMP_L_BMSK                               0x1
#define HWIO_MSS_QDSP6SS_INTFCLAMP_CLEAR_AXIM_CLAMP_L_SHFT                                 0

#define HWIO_MSS_QDSP6SS_INTFCLAMP_STATUS_ADDR                                    (MSS_QDSP6V671SS_PUB_REG_BASE            + 0x98)
#define HWIO_MSS_QDSP6SS_INTFCLAMP_STATUS_RMSK                                           0xf
#define HWIO_MSS_QDSP6SS_INTFCLAMP_STATUS_IN                    \
                in_dword(HWIO_MSS_QDSP6SS_INTFCLAMP_STATUS_ADDR)
#define HWIO_MSS_QDSP6SS_INTFCLAMP_STATUS_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_INTFCLAMP_STATUS_ADDR, m)
#define HWIO_MSS_QDSP6SS_INTFCLAMP_STATUS_AXIM2_CLAMP_E_BMSK                             0x8
#define HWIO_MSS_QDSP6SS_INTFCLAMP_STATUS_AXIM2_CLAMP_E_SHFT                               3
#define HWIO_MSS_QDSP6SS_INTFCLAMP_STATUS_AXIM2_CLAMP_L_BMSK                             0x4
#define HWIO_MSS_QDSP6SS_INTFCLAMP_STATUS_AXIM2_CLAMP_L_SHFT                               2
#define HWIO_MSS_QDSP6SS_INTFCLAMP_STATUS_AXIM_CLAMP_E_BMSK                              0x2
#define HWIO_MSS_QDSP6SS_INTFCLAMP_STATUS_AXIM_CLAMP_E_SHFT                                1
#define HWIO_MSS_QDSP6SS_INTFCLAMP_STATUS_AXIM_CLAMP_L_BMSK                              0x1
#define HWIO_MSS_QDSP6SS_INTFCLAMP_STATUS_AXIM_CLAMP_L_SHFT                                0

#define HWIO_MSS_QDSP6SS_INTF_FIFO_RESET_ADDR                                     (MSS_QDSP6V671SS_PUB_REG_BASE            + 0x9c)
#define HWIO_MSS_QDSP6SS_INTF_FIFO_RESET_RMSK                                            0x3
#define HWIO_MSS_QDSP6SS_INTF_FIFO_RESET_IN                    \
                in_dword(HWIO_MSS_QDSP6SS_INTF_FIFO_RESET_ADDR)
#define HWIO_MSS_QDSP6SS_INTF_FIFO_RESET_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_INTF_FIFO_RESET_ADDR, m)
#define HWIO_MSS_QDSP6SS_INTF_FIFO_RESET_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_INTF_FIFO_RESET_ADDR,v)
#define HWIO_MSS_QDSP6SS_INTF_FIFO_RESET_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_QDSP6SS_INTF_FIFO_RESET_ADDR,m,v,HWIO_MSS_QDSP6SS_INTF_FIFO_RESET_IN)
#define HWIO_MSS_QDSP6SS_INTF_FIFO_RESET_AXIM2_BMSK                                      0x2
#define HWIO_MSS_QDSP6SS_INTF_FIFO_RESET_AXIM2_SHFT                                        1
#define HWIO_MSS_QDSP6SS_INTF_FIFO_RESET_AXIM_BMSK                                       0x1
#define HWIO_MSS_QDSP6SS_INTF_FIFO_RESET_AXIM_SHFT                                         0

#define HWIO_MSS_QDSP6SS_STATERET_CTL_ADDR                                        (MSS_QDSP6V671SS_PUB_REG_BASE            + 0xa0)
#define HWIO_MSS_QDSP6SS_STATERET_CTL_RMSK                                               0xf
#define HWIO_MSS_QDSP6SS_STATERET_CTL_IN                    \
                in_dword(HWIO_MSS_QDSP6SS_STATERET_CTL_ADDR)
#define HWIO_MSS_QDSP6SS_STATERET_CTL_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_STATERET_CTL_ADDR, m)
#define HWIO_MSS_QDSP6SS_STATERET_CTL_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_STATERET_CTL_ADDR,v)
#define HWIO_MSS_QDSP6SS_STATERET_CTL_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_QDSP6SS_STATERET_CTL_ADDR,m,v,HWIO_MSS_QDSP6SS_STATERET_CTL_IN)
#define HWIO_MSS_QDSP6SS_STATERET_CTL_WAKEUP_IN_BMSK                                     0x8
#define HWIO_MSS_QDSP6SS_STATERET_CTL_WAKEUP_IN_SHFT                                       3
#define HWIO_MSS_QDSP6SS_STATERET_CTL_WAKE_IRQ_BMSK                                      0x4
#define HWIO_MSS_QDSP6SS_STATERET_CTL_WAKE_IRQ_SHFT                                        2
#define HWIO_MSS_QDSP6SS_STATERET_CTL_RESTORE_BMSK                                       0x2
#define HWIO_MSS_QDSP6SS_STATERET_CTL_RESTORE_SHFT                                         1
#define HWIO_MSS_QDSP6SS_STATERET_CTL_SAVE_BMSK                                          0x1
#define HWIO_MSS_QDSP6SS_STATERET_CTL_SAVE_SHFT                                            0

#define HWIO_MSS_QDSP6SS_CGC_OVERRIDE_ADDR                                        (MSS_QDSP6V671SS_PUB_REG_BASE            + 0xb0)
#define HWIO_MSS_QDSP6SS_CGC_OVERRIDE_RMSK                                            0xffff
#define HWIO_MSS_QDSP6SS_CGC_OVERRIDE_IN                    \
                in_dword(HWIO_MSS_QDSP6SS_CGC_OVERRIDE_ADDR)
#define HWIO_MSS_QDSP6SS_CGC_OVERRIDE_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_CGC_OVERRIDE_ADDR, m)
#define HWIO_MSS_QDSP6SS_CGC_OVERRIDE_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_CGC_OVERRIDE_ADDR,v)
#define HWIO_MSS_QDSP6SS_CGC_OVERRIDE_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_QDSP6SS_CGC_OVERRIDE_ADDR,m,v,HWIO_MSS_QDSP6SS_CGC_OVERRIDE_IN)
#define HWIO_MSS_QDSP6SS_CGC_OVERRIDE_CP0_CLK_EN_BMSK                                 0x8000
#define HWIO_MSS_QDSP6SS_CGC_OVERRIDE_CP0_CLK_EN_SHFT                                     15
#define HWIO_MSS_QDSP6SS_CGC_OVERRIDE_SPARE_BIT14_BMSK                                0x4000
#define HWIO_MSS_QDSP6SS_CGC_OVERRIDE_SPARE_BIT14_SHFT                                    14
#define HWIO_MSS_QDSP6SS_CGC_OVERRIDE_AXIS_CLK_EN_BMSK                                0x2000
#define HWIO_MSS_QDSP6SS_CGC_OVERRIDE_AXIS_CLK_EN_SHFT                                    13
#define HWIO_MSS_QDSP6SS_CGC_OVERRIDE_SPARE_BIT12_BMSK                                0x1000
#define HWIO_MSS_QDSP6SS_CGC_OVERRIDE_SPARE_BIT12_SHFT                                    12
#define HWIO_MSS_QDSP6SS_CGC_OVERRIDE_AHBM_CLK_EN_BMSK                                 0x800
#define HWIO_MSS_QDSP6SS_CGC_OVERRIDE_AHBM_CLK_EN_SHFT                                    11
#define HWIO_MSS_QDSP6SS_CGC_OVERRIDE_AHBS_CLK_EN_BMSK                                 0x400
#define HWIO_MSS_QDSP6SS_CGC_OVERRIDE_AHBS_CLK_EN_SHFT                                    10
#define HWIO_MSS_QDSP6SS_CGC_OVERRIDE_PRIV_AHBS_CLK_EN_BMSK                            0x300
#define HWIO_MSS_QDSP6SS_CGC_OVERRIDE_PRIV_AHBS_CLK_EN_SHFT                                8
#define HWIO_MSS_QDSP6SS_CGC_OVERRIDE_SPARE_BIT7_BMSK                                   0x80
#define HWIO_MSS_QDSP6SS_CGC_OVERRIDE_SPARE_BIT7_SHFT                                      7
#define HWIO_MSS_QDSP6SS_CGC_OVERRIDE_L2VIC_AHBS_CLK_EN_BMSK                            0x60
#define HWIO_MSS_QDSP6SS_CGC_OVERRIDE_L2VIC_AHBS_CLK_EN_SHFT                               5
#define HWIO_MSS_QDSP6SS_CGC_OVERRIDE_TEST_AHBS_CLK_EN_BMSK                             0x10
#define HWIO_MSS_QDSP6SS_CGC_OVERRIDE_TEST_AHBS_CLK_EN_SHFT                                4
#define HWIO_MSS_QDSP6SS_CGC_OVERRIDE_PUB_AHBS_CLK_EN_BMSK                               0x8
#define HWIO_MSS_QDSP6SS_CGC_OVERRIDE_PUB_AHBS_CLK_EN_SHFT                                 3
#define HWIO_MSS_QDSP6SS_CGC_OVERRIDE_CORE_BUS_EN_BMSK                                   0x4
#define HWIO_MSS_QDSP6SS_CGC_OVERRIDE_CORE_BUS_EN_SHFT                                     2
#define HWIO_MSS_QDSP6SS_CGC_OVERRIDE_CORE_RCLK_EN_BMSK                                  0x2
#define HWIO_MSS_QDSP6SS_CGC_OVERRIDE_CORE_RCLK_EN_SHFT                                    1
#define HWIO_MSS_QDSP6SS_CGC_OVERRIDE_CORE_CLK_EN_BMSK                                   0x1
#define HWIO_MSS_QDSP6SS_CGC_OVERRIDE_CORE_CLK_EN_SHFT                                     0

#define HWIO_MSS_QDSP6SS_CORE_BHS_CTL_ADDR                                        (MSS_QDSP6V671SS_PUB_REG_BASE            + 0xc0)
#define HWIO_MSS_QDSP6SS_CORE_BHS_CTL_RMSK                                           0x3f3ff
#define HWIO_MSS_QDSP6SS_CORE_BHS_CTL_IN                    \
                in_dword(HWIO_MSS_QDSP6SS_CORE_BHS_CTL_ADDR)
#define HWIO_MSS_QDSP6SS_CORE_BHS_CTL_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_CORE_BHS_CTL_ADDR, m)
#define HWIO_MSS_QDSP6SS_CORE_BHS_CTL_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_CORE_BHS_CTL_ADDR,v)
#define HWIO_MSS_QDSP6SS_CORE_BHS_CTL_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_QDSP6SS_CORE_BHS_CTL_ADDR,m,v,HWIO_MSS_QDSP6SS_CORE_BHS_CTL_IN)
#define HWIO_MSS_QDSP6SS_CORE_BHS_CTL_DELAY_PROG_BMSK                                0x3f000
#define HWIO_MSS_QDSP6SS_CORE_BHS_CTL_DELAY_PROG_SHFT                                     12
#define HWIO_MSS_QDSP6SS_CORE_BHS_CTL_ENF_PROG_BMSK                                    0x3f0
#define HWIO_MSS_QDSP6SS_CORE_BHS_CTL_ENF_PROG_SHFT                                        4
#define HWIO_MSS_QDSP6SS_CORE_BHS_CTL_DRIVE_SEL_BMSK                                     0xf
#define HWIO_MSS_QDSP6SS_CORE_BHS_CTL_DRIVE_SEL_SHFT                                       0

#define HWIO_MSS_QDSP6SS_CPR_OVERRIDE_ADDR                                        (MSS_QDSP6V671SS_PUB_REG_BASE            + 0xd0)
#define HWIO_MSS_QDSP6SS_CPR_OVERRIDE_RMSK                                               0x3
#define HWIO_MSS_QDSP6SS_CPR_OVERRIDE_IN                    \
                in_dword(HWIO_MSS_QDSP6SS_CPR_OVERRIDE_ADDR)
#define HWIO_MSS_QDSP6SS_CPR_OVERRIDE_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_CPR_OVERRIDE_ADDR, m)
#define HWIO_MSS_QDSP6SS_CPR_OVERRIDE_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_CPR_OVERRIDE_ADDR,v)
#define HWIO_MSS_QDSP6SS_CPR_OVERRIDE_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_QDSP6SS_CPR_OVERRIDE_ADDR,m,v,HWIO_MSS_QDSP6SS_CPR_OVERRIDE_IN)
#define HWIO_MSS_QDSP6SS_CPR_OVERRIDE_EXT_ABYP_DIS_BMSK                                  0x2
#define HWIO_MSS_QDSP6SS_CPR_OVERRIDE_EXT_ABYP_DIS_SHFT                                    1
#define HWIO_MSS_QDSP6SS_CPR_OVERRIDE_LOCAL_BYP_DIS_BMSK                                 0x1
#define HWIO_MSS_QDSP6SS_CPR_OVERRIDE_LOCAL_BYP_DIS_SHFT                                   0

#define HWIO_MSS_QDSP6SS_STRAP_TCM_ADDR                                           (MSS_QDSP6V671SS_PUB_REG_BASE            + 0x100)
#define HWIO_MSS_QDSP6SS_STRAP_TCM_RMSK                                           0xfffc0000
#define HWIO_MSS_QDSP6SS_STRAP_TCM_IN                    \
                in_dword(HWIO_MSS_QDSP6SS_STRAP_TCM_ADDR)
#define HWIO_MSS_QDSP6SS_STRAP_TCM_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_STRAP_TCM_ADDR, m)
#define HWIO_MSS_QDSP6SS_STRAP_TCM_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_STRAP_TCM_ADDR,v)
#define HWIO_MSS_QDSP6SS_STRAP_TCM_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_QDSP6SS_STRAP_TCM_ADDR,m,v,HWIO_MSS_QDSP6SS_STRAP_TCM_IN)
#define HWIO_MSS_QDSP6SS_STRAP_TCM_BASE_ADDR_BMSK                                 0xfffc0000
#define HWIO_MSS_QDSP6SS_STRAP_TCM_BASE_ADDR_SHFT                                         18

#define HWIO_MSS_QDSP6SS_STRAP_AHBUPPER_ADDR                                      (MSS_QDSP6V671SS_PUB_REG_BASE            + 0x104)
#define HWIO_MSS_QDSP6SS_STRAP_AHBUPPER_RMSK                                      0xffffc000
#define HWIO_MSS_QDSP6SS_STRAP_AHBUPPER_IN                    \
                in_dword(HWIO_MSS_QDSP6SS_STRAP_AHBUPPER_ADDR)
#define HWIO_MSS_QDSP6SS_STRAP_AHBUPPER_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_STRAP_AHBUPPER_ADDR, m)
#define HWIO_MSS_QDSP6SS_STRAP_AHBUPPER_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_STRAP_AHBUPPER_ADDR,v)
#define HWIO_MSS_QDSP6SS_STRAP_AHBUPPER_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_QDSP6SS_STRAP_AHBUPPER_ADDR,m,v,HWIO_MSS_QDSP6SS_STRAP_AHBUPPER_IN)
#define HWIO_MSS_QDSP6SS_STRAP_AHBUPPER_UPPER_ADDR_BMSK                           0xffffc000
#define HWIO_MSS_QDSP6SS_STRAP_AHBUPPER_UPPER_ADDR_SHFT                                   14

#define HWIO_MSS_QDSP6SS_STRAP_AHBLOWER_ADDR                                      (MSS_QDSP6V671SS_PUB_REG_BASE            + 0x108)
#define HWIO_MSS_QDSP6SS_STRAP_AHBLOWER_RMSK                                      0xffffc000
#define HWIO_MSS_QDSP6SS_STRAP_AHBLOWER_IN                    \
                in_dword(HWIO_MSS_QDSP6SS_STRAP_AHBLOWER_ADDR)
#define HWIO_MSS_QDSP6SS_STRAP_AHBLOWER_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_STRAP_AHBLOWER_ADDR, m)
#define HWIO_MSS_QDSP6SS_STRAP_AHBLOWER_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_STRAP_AHBLOWER_ADDR,v)
#define HWIO_MSS_QDSP6SS_STRAP_AHBLOWER_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_QDSP6SS_STRAP_AHBLOWER_ADDR,m,v,HWIO_MSS_QDSP6SS_STRAP_AHBLOWER_IN)
#define HWIO_MSS_QDSP6SS_STRAP_AHBLOWER_LOWER_ADDR_BMSK                           0xffffc000
#define HWIO_MSS_QDSP6SS_STRAP_AHBLOWER_LOWER_ADDR_SHFT                                   14

#define HWIO_MSS_QDSP6SS_DCC_CTRL_ADDR                                            (MSS_QDSP6V671SS_PUB_REG_BASE            + 0x118)
#define HWIO_MSS_QDSP6SS_DCC_CTRL_RMSK                                            0xffffffff
#define HWIO_MSS_QDSP6SS_DCC_CTRL_IN                    \
                in_dword(HWIO_MSS_QDSP6SS_DCC_CTRL_ADDR)
#define HWIO_MSS_QDSP6SS_DCC_CTRL_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_DCC_CTRL_ADDR, m)
#define HWIO_MSS_QDSP6SS_DCC_CTRL_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_DCC_CTRL_ADDR,v)
#define HWIO_MSS_QDSP6SS_DCC_CTRL_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_QDSP6SS_DCC_CTRL_ADDR,m,v,HWIO_MSS_QDSP6SS_DCC_CTRL_IN)
#define HWIO_MSS_QDSP6SS_DCC_CTRL_CTRL_BMSK                                       0xffffffff
#define HWIO_MSS_QDSP6SS_DCC_CTRL_CTRL_SHFT                                                0

#define HWIO_MSS_QDSP6SS_STRAP_AXIM2UPPER_ADDR                                    (MSS_QDSP6V671SS_PUB_REG_BASE            + 0x11c)
#define HWIO_MSS_QDSP6SS_STRAP_AXIM2UPPER_RMSK                                    0xffffc000
#define HWIO_MSS_QDSP6SS_STRAP_AXIM2UPPER_IN                    \
                in_dword(HWIO_MSS_QDSP6SS_STRAP_AXIM2UPPER_ADDR)
#define HWIO_MSS_QDSP6SS_STRAP_AXIM2UPPER_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_STRAP_AXIM2UPPER_ADDR, m)
#define HWIO_MSS_QDSP6SS_STRAP_AXIM2UPPER_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_STRAP_AXIM2UPPER_ADDR,v)
#define HWIO_MSS_QDSP6SS_STRAP_AXIM2UPPER_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_QDSP6SS_STRAP_AXIM2UPPER_ADDR,m,v,HWIO_MSS_QDSP6SS_STRAP_AXIM2UPPER_IN)
#define HWIO_MSS_QDSP6SS_STRAP_AXIM2UPPER_UPPER_ADDR_BMSK                         0xffffc000
#define HWIO_MSS_QDSP6SS_STRAP_AXIM2UPPER_UPPER_ADDR_SHFT                                 14

#define HWIO_MSS_QDSP6SS_STRAP_AXIM2LOWER_ADDR                                    (MSS_QDSP6V671SS_PUB_REG_BASE            + 0x120)
#define HWIO_MSS_QDSP6SS_STRAP_AXIM2LOWER_RMSK                                    0xffffc000
#define HWIO_MSS_QDSP6SS_STRAP_AXIM2LOWER_IN                    \
                in_dword(HWIO_MSS_QDSP6SS_STRAP_AXIM2LOWER_ADDR)
#define HWIO_MSS_QDSP6SS_STRAP_AXIM2LOWER_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_STRAP_AXIM2LOWER_ADDR, m)
#define HWIO_MSS_QDSP6SS_STRAP_AXIM2LOWER_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_STRAP_AXIM2LOWER_ADDR,v)
#define HWIO_MSS_QDSP6SS_STRAP_AXIM2LOWER_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_QDSP6SS_STRAP_AXIM2LOWER_ADDR,m,v,HWIO_MSS_QDSP6SS_STRAP_AXIM2LOWER_IN)
#define HWIO_MSS_QDSP6SS_STRAP_AXIM2LOWER_LOWER_ADDR_BMSK                         0xffffc000
#define HWIO_MSS_QDSP6SS_STRAP_AXIM2LOWER_LOWER_ADDR_SHFT                                 14

#define HWIO_MSS_QDSP6SS_QMC_SVS_CTL_ADDR                                         (MSS_QDSP6V671SS_PUB_REG_BASE            + 0x130)
#define HWIO_MSS_QDSP6SS_QMC_SVS_CTL_RMSK                                                0x3
#define HWIO_MSS_QDSP6SS_QMC_SVS_CTL_IN                    \
                in_dword(HWIO_MSS_QDSP6SS_QMC_SVS_CTL_ADDR)
#define HWIO_MSS_QDSP6SS_QMC_SVS_CTL_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_QMC_SVS_CTL_ADDR, m)
#define HWIO_MSS_QDSP6SS_QMC_SVS_CTL_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_QMC_SVS_CTL_ADDR,v)
#define HWIO_MSS_QDSP6SS_QMC_SVS_CTL_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_QDSP6SS_QMC_SVS_CTL_ADDR,m,v,HWIO_MSS_QDSP6SS_QMC_SVS_CTL_IN)
#define HWIO_MSS_QDSP6SS_QMC_SVS_CTL_QMC_MEM_SVS_SEL_BMSK                                0x2
#define HWIO_MSS_QDSP6SS_QMC_SVS_CTL_QMC_MEM_SVS_SEL_SHFT                                  1
#define HWIO_MSS_QDSP6SS_QMC_SVS_CTL_QMC_MEM_SVS_BMSK                                    0x1
#define HWIO_MSS_QDSP6SS_QMC_SVS_CTL_QMC_MEM_SVS_SHFT                                      0

#define HWIO_MSS_QDSP6SS_CORE_MEM_STAGGER_CTL_ADDR                                (MSS_QDSP6V671SS_PUB_REG_BASE            + 0x140)
#define HWIO_MSS_QDSP6SS_CORE_MEM_STAGGER_CTL_RMSK                                       0xf
#define HWIO_MSS_QDSP6SS_CORE_MEM_STAGGER_CTL_IN                    \
                in_dword(HWIO_MSS_QDSP6SS_CORE_MEM_STAGGER_CTL_ADDR)
#define HWIO_MSS_QDSP6SS_CORE_MEM_STAGGER_CTL_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_CORE_MEM_STAGGER_CTL_ADDR, m)
#define HWIO_MSS_QDSP6SS_CORE_MEM_STAGGER_CTL_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_CORE_MEM_STAGGER_CTL_ADDR,v)
#define HWIO_MSS_QDSP6SS_CORE_MEM_STAGGER_CTL_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_QDSP6SS_CORE_MEM_STAGGER_CTL_ADDR,m,v,HWIO_MSS_QDSP6SS_CORE_MEM_STAGGER_CTL_IN)
#define HWIO_MSS_QDSP6SS_CORE_MEM_STAGGER_CTL_DIV_BMSK                                   0xf
#define HWIO_MSS_QDSP6SS_CORE_MEM_STAGGER_CTL_DIV_SHFT                                     0

#define HWIO_MSS_QDSP6SS_PLL_RCG_UPDATE_STATUS_ADDR                               (MSS_QDSP6V671SS_PUB_REG_BASE            + 0x260)
#define HWIO_MSS_QDSP6SS_PLL_RCG_UPDATE_STATUS_RMSK                                      0x3
#define HWIO_MSS_QDSP6SS_PLL_RCG_UPDATE_STATUS_IN                    \
                in_dword(HWIO_MSS_QDSP6SS_PLL_RCG_UPDATE_STATUS_ADDR)
#define HWIO_MSS_QDSP6SS_PLL_RCG_UPDATE_STATUS_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_PLL_RCG_UPDATE_STATUS_ADDR, m)
#define HWIO_MSS_QDSP6SS_PLL_RCG_UPDATE_STATUS_RCG2PLL_UPD_STATUS_BMSK                   0x2
#define HWIO_MSS_QDSP6SS_PLL_RCG_UPDATE_STATUS_RCG2PLL_UPD_STATUS_SHFT                     1
#define HWIO_MSS_QDSP6SS_PLL_RCG_UPDATE_STATUS_PLL2RCG_UPD_STATUS_BMSK                   0x1
#define HWIO_MSS_QDSP6SS_PLL_RCG_UPDATE_STATUS_PLL2RCG_UPD_STATUS_SHFT                     0

#define HWIO_MSS_QDSP6SS_PLL_RCG_UPDATE_CFG_ADDR                                  (MSS_QDSP6V671SS_PUB_REG_BASE            + 0x264)
#define HWIO_MSS_QDSP6SS_PLL_RCG_UPDATE_CFG_RMSK                                         0x3
#define HWIO_MSS_QDSP6SS_PLL_RCG_UPDATE_CFG_IN                    \
                in_dword(HWIO_MSS_QDSP6SS_PLL_RCG_UPDATE_CFG_ADDR)
#define HWIO_MSS_QDSP6SS_PLL_RCG_UPDATE_CFG_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_PLL_RCG_UPDATE_CFG_ADDR, m)
#define HWIO_MSS_QDSP6SS_PLL_RCG_UPDATE_CFG_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_PLL_RCG_UPDATE_CFG_ADDR,v)
#define HWIO_MSS_QDSP6SS_PLL_RCG_UPDATE_CFG_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_QDSP6SS_PLL_RCG_UPDATE_CFG_ADDR,m,v,HWIO_MSS_QDSP6SS_PLL_RCG_UPDATE_CFG_IN)
#define HWIO_MSS_QDSP6SS_PLL_RCG_UPDATE_CFG_RCG2PLL_UPD_EN_BMSK                          0x2
#define HWIO_MSS_QDSP6SS_PLL_RCG_UPDATE_CFG_RCG2PLL_UPD_EN_SHFT                            1
#define HWIO_MSS_QDSP6SS_PLL_RCG_UPDATE_CFG_PLL2RCG_UPD_EN_BMSK                          0x1
#define HWIO_MSS_QDSP6SS_PLL_RCG_UPDATE_CFG_PLL2RCG_UPD_EN_SHFT                            0

#define HWIO_MSS_QDSP6SS_PLL_RCG_UPDATE_DLYCTL_ADDR                               (MSS_QDSP6V671SS_PUB_REG_BASE            + 0x268)
#define HWIO_MSS_QDSP6SS_PLL_RCG_UPDATE_DLYCTL_RMSK                                     0x1f
#define HWIO_MSS_QDSP6SS_PLL_RCG_UPDATE_DLYCTL_IN                    \
                in_dword(HWIO_MSS_QDSP6SS_PLL_RCG_UPDATE_DLYCTL_ADDR)
#define HWIO_MSS_QDSP6SS_PLL_RCG_UPDATE_DLYCTL_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_PLL_RCG_UPDATE_DLYCTL_ADDR, m)
#define HWIO_MSS_QDSP6SS_PLL_RCG_UPDATE_DLYCTL_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_PLL_RCG_UPDATE_DLYCTL_ADDR,v)
#define HWIO_MSS_QDSP6SS_PLL_RCG_UPDATE_DLYCTL_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_QDSP6SS_PLL_RCG_UPDATE_DLYCTL_ADDR,m,v,HWIO_MSS_QDSP6SS_PLL_RCG_UPDATE_DLYCTL_IN)
#define HWIO_MSS_QDSP6SS_PLL_RCG_UPDATE_DLYCTL_MATCH_VALUE_BMSK                         0x1f
#define HWIO_MSS_QDSP6SS_PLL_RCG_UPDATE_DLYCTL_MATCH_VALUE_SHFT                            0

#define HWIO_MSS_QDSP6SS_PLL_RCG_UPDATE_CMD_ADDR                                  (MSS_QDSP6V671SS_PUB_REG_BASE            + 0x26c)
#define HWIO_MSS_QDSP6SS_PLL_RCG_UPDATE_CMD_RMSK                                         0x1
#define HWIO_MSS_QDSP6SS_PLL_RCG_UPDATE_CMD_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_PLL_RCG_UPDATE_CMD_ADDR,v)
#define HWIO_MSS_QDSP6SS_PLL_RCG_UPDATE_CMD_CMD_BMSK                                     0x1
#define HWIO_MSS_QDSP6SS_PLL_RCG_UPDATE_CMD_CMD_SHFT                                       0

#define HWIO_MSS_QDSP6SS_DBG_NMI_CFG_ADDR                                         (MSS_QDSP6V671SS_PUB_REG_BASE            + 0x300)
#define HWIO_MSS_QDSP6SS_DBG_NMI_CFG_RMSK                                                0xf
#define HWIO_MSS_QDSP6SS_DBG_NMI_CFG_IN                    \
                in_dword(HWIO_MSS_QDSP6SS_DBG_NMI_CFG_ADDR)
#define HWIO_MSS_QDSP6SS_DBG_NMI_CFG_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_DBG_NMI_CFG_ADDR, m)
#define HWIO_MSS_QDSP6SS_DBG_NMI_CFG_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_DBG_NMI_CFG_ADDR,v)
#define HWIO_MSS_QDSP6SS_DBG_NMI_CFG_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_QDSP6SS_DBG_NMI_CFG_ADDR,m,v,HWIO_MSS_QDSP6SS_DBG_NMI_CFG_IN)
#define HWIO_MSS_QDSP6SS_DBG_NMI_CFG_DBG_NMI_EN_IN_APCR_BMSK                             0x8
#define HWIO_MSS_QDSP6SS_DBG_NMI_CFG_DBG_NMI_EN_IN_APCR_SHFT                               3
#define HWIO_MSS_QDSP6SS_DBG_NMI_CFG_DBG_NMI_EN_IN_PWR_COLLAPSE_BMSK                     0x4
#define HWIO_MSS_QDSP6SS_DBG_NMI_CFG_DBG_NMI_EN_IN_PWR_COLLAPSE_SHFT                       2
#define HWIO_MSS_QDSP6SS_DBG_NMI_CFG_DBG_NMI_DISABLE_BMSK                                0x2
#define HWIO_MSS_QDSP6SS_DBG_NMI_CFG_DBG_NMI_DISABLE_SHFT                                  1
#define HWIO_MSS_QDSP6SS_DBG_NMI_CFG_DBG_NMI_WAKEUP_DISABLE_BMSK                         0x1
#define HWIO_MSS_QDSP6SS_DBG_NMI_CFG_DBG_NMI_WAKEUP_DISABLE_SHFT                           0

#define HWIO_MSS_QDSP6SS_DBG_NMI_PWR_STATUS_ADDR                                  (MSS_QDSP6V671SS_PUB_REG_BASE            + 0x304)
#define HWIO_MSS_QDSP6SS_DBG_NMI_PWR_STATUS_RMSK                                     0x7000f
#define HWIO_MSS_QDSP6SS_DBG_NMI_PWR_STATUS_IN                    \
                in_dword(HWIO_MSS_QDSP6SS_DBG_NMI_PWR_STATUS_ADDR)
#define HWIO_MSS_QDSP6SS_DBG_NMI_PWR_STATUS_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_DBG_NMI_PWR_STATUS_ADDR, m)
#define HWIO_MSS_QDSP6SS_DBG_NMI_PWR_STATUS_RSC_IN_SLEEP_BMSK                        0x40000
#define HWIO_MSS_QDSP6SS_DBG_NMI_PWR_STATUS_RSC_IN_SLEEP_SHFT                             18
#define HWIO_MSS_QDSP6SS_DBG_NMI_PWR_STATUS_QDSP6_CLK_GATING_BMSK                    0x20000
#define HWIO_MSS_QDSP6SS_DBG_NMI_PWR_STATUS_QDSP6_CLK_GATING_SHFT                         17
#define HWIO_MSS_QDSP6SS_DBG_NMI_PWR_STATUS_QDSP6_CORE_STATE_BMSK                    0x10000
#define HWIO_MSS_QDSP6SS_DBG_NMI_PWR_STATUS_QDSP6_CORE_STATE_SHFT                         16
#define HWIO_MSS_QDSP6SS_DBG_NMI_PWR_STATUS_NMI_CAUSE_BMSK                               0xc
#define HWIO_MSS_QDSP6SS_DBG_NMI_PWR_STATUS_NMI_CAUSE_SHFT                                 2
#define HWIO_MSS_QDSP6SS_DBG_NMI_PWR_STATUS_QDSP6_PWR_COLLAPSE_BMSK                      0x2
#define HWIO_MSS_QDSP6SS_DBG_NMI_PWR_STATUS_QDSP6_PWR_COLLAPSE_SHFT                        1
#define HWIO_MSS_QDSP6SS_DBG_NMI_PWR_STATUS_QDSP6_IN_SLEEP_BMSK                          0x1
#define HWIO_MSS_QDSP6SS_DBG_NMI_PWR_STATUS_QDSP6_IN_SLEEP_SHFT                            0

#define HWIO_MSS_QDSP6SS_BOOT_CORE_START_ADDR                                     (MSS_QDSP6V671SS_PUB_REG_BASE            + 0x400)
#define HWIO_MSS_QDSP6SS_BOOT_CORE_START_RMSK                                            0x1
#define HWIO_MSS_QDSP6SS_BOOT_CORE_START_IN                    \
                in_dword(HWIO_MSS_QDSP6SS_BOOT_CORE_START_ADDR)
#define HWIO_MSS_QDSP6SS_BOOT_CORE_START_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_BOOT_CORE_START_ADDR, m)
#define HWIO_MSS_QDSP6SS_BOOT_CORE_START_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_BOOT_CORE_START_ADDR,v)
#define HWIO_MSS_QDSP6SS_BOOT_CORE_START_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_QDSP6SS_BOOT_CORE_START_ADDR,m,v,HWIO_MSS_QDSP6SS_BOOT_CORE_START_IN)
#define HWIO_MSS_QDSP6SS_BOOT_CORE_START_START_BMSK                                      0x1
#define HWIO_MSS_QDSP6SS_BOOT_CORE_START_START_SHFT                                        0

#define HWIO_MSS_QDSP6SS_BOOT_CMD_ADDR                                            (MSS_QDSP6V671SS_PUB_REG_BASE            + 0x404)
#define HWIO_MSS_QDSP6SS_BOOT_CMD_RMSK                                                   0x1
#define HWIO_MSS_QDSP6SS_BOOT_CMD_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_BOOT_CMD_ADDR,v)
#define HWIO_MSS_QDSP6SS_BOOT_CMD_CMD_BMSK                                               0x1
#define HWIO_MSS_QDSP6SS_BOOT_CMD_CMD_SHFT                                                 0

#define HWIO_MSS_QDSP6SS_BOOT_STATUS_ADDR                                         (MSS_QDSP6V671SS_PUB_REG_BASE            + 0x408)
#define HWIO_MSS_QDSP6SS_BOOT_STATUS_RMSK                                                0x1
#define HWIO_MSS_QDSP6SS_BOOT_STATUS_IN                    \
                in_dword(HWIO_MSS_QDSP6SS_BOOT_STATUS_ADDR)
#define HWIO_MSS_QDSP6SS_BOOT_STATUS_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_BOOT_STATUS_ADDR, m)
#define HWIO_MSS_QDSP6SS_BOOT_STATUS_STATUS_BMSK                                         0x1
#define HWIO_MSS_QDSP6SS_BOOT_STATUS_STATUS_SHFT                                           0

#define HWIO_MSS_QDSP6SS_CORE_BHS_CFG_ADDR                                        (MSS_QDSP6V671SS_PUB_REG_BASE            + 0x410)
#define HWIO_MSS_QDSP6SS_CORE_BHS_CFG_RMSK                                               0x1
#define HWIO_MSS_QDSP6SS_CORE_BHS_CFG_IN                    \
                in_dword(HWIO_MSS_QDSP6SS_CORE_BHS_CFG_ADDR)
#define HWIO_MSS_QDSP6SS_CORE_BHS_CFG_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_CORE_BHS_CFG_ADDR, m)
#define HWIO_MSS_QDSP6SS_CORE_BHS_CFG_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_CORE_BHS_CFG_ADDR,v)
#define HWIO_MSS_QDSP6SS_CORE_BHS_CFG_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_QDSP6SS_CORE_BHS_CFG_ADDR,m,v,HWIO_MSS_QDSP6SS_CORE_BHS_CFG_IN)
#define HWIO_MSS_QDSP6SS_CORE_BHS_CFG_BHS_ON_BMSK                                        0x1
#define HWIO_MSS_QDSP6SS_CORE_BHS_CFG_BHS_ON_SHFT                                          0

#define HWIO_MSS_QDSP6SS_CORE_BHS_CMD_ADDR                                        (MSS_QDSP6V671SS_PUB_REG_BASE            + 0x414)
#define HWIO_MSS_QDSP6SS_CORE_BHS_CMD_RMSK                                               0x1
#define HWIO_MSS_QDSP6SS_CORE_BHS_CMD_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_CORE_BHS_CMD_ADDR,v)
#define HWIO_MSS_QDSP6SS_CORE_BHS_CMD_UPDATE_BMSK                                        0x1
#define HWIO_MSS_QDSP6SS_CORE_BHS_CMD_UPDATE_SHFT                                          0

#define HWIO_MSS_QDSP6SS_CORE_BHS_STATUS_ADDR                                     (MSS_QDSP6V671SS_PUB_REG_BASE            + 0x418)
#define HWIO_MSS_QDSP6SS_CORE_BHS_STATUS_RMSK                                            0x1
#define HWIO_MSS_QDSP6SS_CORE_BHS_STATUS_IN                    \
                in_dword(HWIO_MSS_QDSP6SS_CORE_BHS_STATUS_ADDR)
#define HWIO_MSS_QDSP6SS_CORE_BHS_STATUS_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_CORE_BHS_STATUS_ADDR, m)
#define HWIO_MSS_QDSP6SS_CORE_BHS_STATUS_BHS_ON_BMSK                                     0x1
#define HWIO_MSS_QDSP6SS_CORE_BHS_STATUS_BHS_ON_SHFT                                       0

#define HWIO_MSS_QDSP6SS_LDO_CTL_CFG_ADDR                                         (MSS_QDSP6V671SS_PUB_REG_BASE            + 0x420)
#define HWIO_MSS_QDSP6SS_LDO_CTL_CFG_RMSK                                                0x7
#define HWIO_MSS_QDSP6SS_LDO_CTL_CFG_IN                    \
                in_dword(HWIO_MSS_QDSP6SS_LDO_CTL_CFG_ADDR)
#define HWIO_MSS_QDSP6SS_LDO_CTL_CFG_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_LDO_CTL_CFG_ADDR, m)
#define HWIO_MSS_QDSP6SS_LDO_CTL_CFG_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_LDO_CTL_CFG_ADDR,v)
#define HWIO_MSS_QDSP6SS_LDO_CTL_CFG_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_QDSP6SS_LDO_CTL_CFG_ADDR,m,v,HWIO_MSS_QDSP6SS_LDO_CTL_CFG_IN)
#define HWIO_MSS_QDSP6SS_LDO_CTL_CFG_LDO_PWR_UP_BG_BMSK                                  0x4
#define HWIO_MSS_QDSP6SS_LDO_CTL_CFG_LDO_PWR_UP_BG_SHFT                                    2
#define HWIO_MSS_QDSP6SS_LDO_CTL_CFG_LDO_PWR_UP_BMSK                                     0x2
#define HWIO_MSS_QDSP6SS_LDO_CTL_CFG_LDO_PWR_UP_SHFT                                       1
#define HWIO_MSS_QDSP6SS_LDO_CTL_CFG_LDO_BYP_BMSK                                        0x1
#define HWIO_MSS_QDSP6SS_LDO_CTL_CFG_LDO_BYP_SHFT                                          0

#define HWIO_MSS_QDSP6SS_LDO_CTL_CMD_ADDR                                         (MSS_QDSP6V671SS_PUB_REG_BASE            + 0x424)
#define HWIO_MSS_QDSP6SS_LDO_CTL_CMD_RMSK                                                0x7
#define HWIO_MSS_QDSP6SS_LDO_CTL_CMD_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_LDO_CTL_CMD_ADDR,v)
#define HWIO_MSS_QDSP6SS_LDO_CTL_CMD_LDO_PWR_UP_BG_UPDATE_BMSK                           0x4
#define HWIO_MSS_QDSP6SS_LDO_CTL_CMD_LDO_PWR_UP_BG_UPDATE_SHFT                             2
#define HWIO_MSS_QDSP6SS_LDO_CTL_CMD_LDO_PWR_UP_UPDATE_BMSK                              0x2
#define HWIO_MSS_QDSP6SS_LDO_CTL_CMD_LDO_PWR_UP_UPDATE_SHFT                                1
#define HWIO_MSS_QDSP6SS_LDO_CTL_CMD_LDO_BYP_UPDATE_BMSK                                 0x1
#define HWIO_MSS_QDSP6SS_LDO_CTL_CMD_LDO_BYP_UPDATE_SHFT                                   0

#define HWIO_MSS_QDSP6SS_LDO_CTL_STATUS_ADDR                                      (MSS_QDSP6V671SS_PUB_REG_BASE            + 0x428)
#define HWIO_MSS_QDSP6SS_LDO_CTL_STATUS_RMSK                                             0x7
#define HWIO_MSS_QDSP6SS_LDO_CTL_STATUS_IN                    \
                in_dword(HWIO_MSS_QDSP6SS_LDO_CTL_STATUS_ADDR)
#define HWIO_MSS_QDSP6SS_LDO_CTL_STATUS_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_LDO_CTL_STATUS_ADDR, m)
#define HWIO_MSS_QDSP6SS_LDO_CTL_STATUS_LDO_PWR_UP_BG_BMSK                               0x4
#define HWIO_MSS_QDSP6SS_LDO_CTL_STATUS_LDO_PWR_UP_BG_SHFT                                 2
#define HWIO_MSS_QDSP6SS_LDO_CTL_STATUS_LDO_PWR_UP_BMSK                                  0x2
#define HWIO_MSS_QDSP6SS_LDO_CTL_STATUS_LDO_PWR_UP_SHFT                                    1
#define HWIO_MSS_QDSP6SS_LDO_CTL_STATUS_LDO_BYP_BMSK                                     0x1
#define HWIO_MSS_QDSP6SS_LDO_CTL_STATUS_LDO_BYP_SHFT                                       0

#define HWIO_MSS_QDSP6SS_MEM_CFG_ADDR                                             (MSS_QDSP6V671SS_PUB_REG_BASE            + 0x430)
#define HWIO_MSS_QDSP6SS_MEM_CFG_RMSK                                               0x1f001f
#define HWIO_MSS_QDSP6SS_MEM_CFG_IN                    \
                in_dword(HWIO_MSS_QDSP6SS_MEM_CFG_ADDR)
#define HWIO_MSS_QDSP6SS_MEM_CFG_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_MEM_CFG_ADDR, m)
#define HWIO_MSS_QDSP6SS_MEM_CFG_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_MEM_CFG_ADDR,v)
#define HWIO_MSS_QDSP6SS_MEM_CFG_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_QDSP6SS_MEM_CFG_ADDR,m,v,HWIO_MSS_QDSP6SS_MEM_CFG_IN)
#define HWIO_MSS_QDSP6SS_MEM_CFG_L2MEM_SLP_RET_N_BMSK                               0x1e0000
#define HWIO_MSS_QDSP6SS_MEM_CFG_L2MEM_SLP_RET_N_SHFT                                     17
#define HWIO_MSS_QDSP6SS_MEM_CFG_MEM_SLP_RET_N_BMSK                                  0x10000
#define HWIO_MSS_QDSP6SS_MEM_CFG_MEM_SLP_RET_N_SHFT                                       16
#define HWIO_MSS_QDSP6SS_MEM_CFG_L2MEM_SLP_NRET_N_BMSK                                  0x1e
#define HWIO_MSS_QDSP6SS_MEM_CFG_L2MEM_SLP_NRET_N_SHFT                                     1
#define HWIO_MSS_QDSP6SS_MEM_CFG_MEM_SLP_NRET_N_BMSK                                     0x1
#define HWIO_MSS_QDSP6SS_MEM_CFG_MEM_SLP_NRET_N_SHFT                                       0

#define HWIO_MSS_QDSP6SS_MEM_CMD_ADDR                                             (MSS_QDSP6V671SS_PUB_REG_BASE            + 0x434)
#define HWIO_MSS_QDSP6SS_MEM_CMD_RMSK                                                    0x3
#define HWIO_MSS_QDSP6SS_MEM_CMD_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_MEM_CMD_ADDR,v)
#define HWIO_MSS_QDSP6SS_MEM_CMD_UPDATE_SLP_RET_N_BMSK                                   0x2
#define HWIO_MSS_QDSP6SS_MEM_CMD_UPDATE_SLP_RET_N_SHFT                                     1
#define HWIO_MSS_QDSP6SS_MEM_CMD_UPDATE_SLP_NRET_N_BMSK                                  0x1
#define HWIO_MSS_QDSP6SS_MEM_CMD_UPDATE_SLP_NRET_N_SHFT                                    0

#define HWIO_MSS_QDSP6SS_MEM_STATUS_ADDR                                          (MSS_QDSP6V671SS_PUB_REG_BASE            + 0x438)
#define HWIO_MSS_QDSP6SS_MEM_STATUS_RMSK                                            0x1f001f
#define HWIO_MSS_QDSP6SS_MEM_STATUS_IN                    \
                in_dword(HWIO_MSS_QDSP6SS_MEM_STATUS_ADDR)
#define HWIO_MSS_QDSP6SS_MEM_STATUS_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_MEM_STATUS_ADDR, m)
#define HWIO_MSS_QDSP6SS_MEM_STATUS_L2MEM_SLP_RET_N_BMSK                            0x1e0000
#define HWIO_MSS_QDSP6SS_MEM_STATUS_L2MEM_SLP_RET_N_SHFT                                  17
#define HWIO_MSS_QDSP6SS_MEM_STATUS_MEM_SLP_RET_N_BMSK                               0x10000
#define HWIO_MSS_QDSP6SS_MEM_STATUS_MEM_SLP_RET_N_SHFT                                    16
#define HWIO_MSS_QDSP6SS_MEM_STATUS_L2MEM_SLP_NRET_N_BMSK                               0x1e
#define HWIO_MSS_QDSP6SS_MEM_STATUS_L2MEM_SLP_NRET_N_SHFT                                  1
#define HWIO_MSS_QDSP6SS_MEM_STATUS_MEM_SLP_NRET_N_BMSK                                  0x1
#define HWIO_MSS_QDSP6SS_MEM_STATUS_MEM_SLP_NRET_N_SHFT                                    0

#define HWIO_MSS_QDSP6SS_RESET_CFG_ADDR                                           (MSS_QDSP6V671SS_PUB_REG_BASE            + 0x440)
#define HWIO_MSS_QDSP6SS_RESET_CFG_RMSK                                              0x1ffff
#define HWIO_MSS_QDSP6SS_RESET_CFG_IN                    \
                in_dword(HWIO_MSS_QDSP6SS_RESET_CFG_ADDR)
#define HWIO_MSS_QDSP6SS_RESET_CFG_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_RESET_CFG_ADDR, m)
#define HWIO_MSS_QDSP6SS_RESET_CFG_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_RESET_CFG_ADDR,v)
#define HWIO_MSS_QDSP6SS_RESET_CFG_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_QDSP6SS_RESET_CFG_ADDR,m,v,HWIO_MSS_QDSP6SS_RESET_CFG_IN)
#define HWIO_MSS_QDSP6SS_RESET_CFG_CLADE2_BMSK                                       0x10000
#define HWIO_MSS_QDSP6SS_RESET_CFG_CLADE2_SHFT                                            16
#define HWIO_MSS_QDSP6SS_RESET_CFG_VPEVU_BMSK                                         0x8000
#define HWIO_MSS_QDSP6SS_RESET_CFG_VPEVU_SHFT                                             15
#define HWIO_MSS_QDSP6SS_RESET_CFG_NRET_BMSK                                          0x4000
#define HWIO_MSS_QDSP6SS_RESET_CFG_NRET_SHFT                                              14
#define HWIO_MSS_QDSP6SS_RESET_CFG_MNOC_BMSK                                          0x2000
#define HWIO_MSS_QDSP6SS_RESET_CFG_MNOC_SHFT                                              13
#define HWIO_MSS_QDSP6SS_RESET_CFG_APB_BMSK                                           0x1000
#define HWIO_MSS_QDSP6SS_RESET_CFG_APB_SHFT                                               12
#define HWIO_MSS_QDSP6SS_RESET_CFG_AXIS2_BMSK                                          0x800
#define HWIO_MSS_QDSP6SS_RESET_CFG_AXIS2_SHFT                                             11
#define HWIO_MSS_QDSP6SS_RESET_CFG_CSI1_BMSK                                           0x400
#define HWIO_MSS_QDSP6SS_RESET_CFG_CSI1_SHFT                                              10
#define HWIO_MSS_QDSP6SS_RESET_CFG_CSI0_BMSK                                           0x200
#define HWIO_MSS_QDSP6SS_RESET_CFG_CSI0_SHFT                                               9
#define HWIO_MSS_QDSP6SS_RESET_CFG_ARCH_BMSK                                           0x100
#define HWIO_MSS_QDSP6SS_RESET_CFG_ARCH_SHFT                                               8
#define HWIO_MSS_QDSP6SS_RESET_CFG_CORE_BMSK                                            0x80
#define HWIO_MSS_QDSP6SS_RESET_CFG_CORE_SHFT                                               7
#define HWIO_MSS_QDSP6SS_RESET_CFG_AXIM2_BMSK                                           0x40
#define HWIO_MSS_QDSP6SS_RESET_CFG_AXIM2_SHFT                                              6
#define HWIO_MSS_QDSP6SS_RESET_CFG_AXIS_BMSK                                            0x20
#define HWIO_MSS_QDSP6SS_RESET_CFG_AXIS_SHFT                                               5
#define HWIO_MSS_QDSP6SS_RESET_CFG_ATBM_BMSK                                            0x10
#define HWIO_MSS_QDSP6SS_RESET_CFG_ATBM_SHFT                                               4
#define HWIO_MSS_QDSP6SS_RESET_CFG_AHBM_BMSK                                             0x8
#define HWIO_MSS_QDSP6SS_RESET_CFG_AHBM_SHFT                                               3
#define HWIO_MSS_QDSP6SS_RESET_CFG_AXIM_BMSK                                             0x4
#define HWIO_MSS_QDSP6SS_RESET_CFG_AXIM_SHFT                                               2
#define HWIO_MSS_QDSP6SS_RESET_CFG_ISDB_ETM_BMSK                                         0x2
#define HWIO_MSS_QDSP6SS_RESET_CFG_ISDB_ETM_SHFT                                           1
#define HWIO_MSS_QDSP6SS_RESET_CFG_RET_BMSK                                              0x1
#define HWIO_MSS_QDSP6SS_RESET_CFG_RET_SHFT                                                0

#define HWIO_MSS_QDSP6SS_RESET_CMD_ADDR                                           (MSS_QDSP6V671SS_PUB_REG_BASE            + 0x444)
#define HWIO_MSS_QDSP6SS_RESET_CMD_RMSK                                                  0x1
#define HWIO_MSS_QDSP6SS_RESET_CMD_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_RESET_CMD_ADDR,v)
#define HWIO_MSS_QDSP6SS_RESET_CMD_UPDATE_BMSK                                           0x1
#define HWIO_MSS_QDSP6SS_RESET_CMD_UPDATE_SHFT                                             0

#define HWIO_MSS_QDSP6SS_RESET_STATUS_ADDR                                        (MSS_QDSP6V671SS_PUB_REG_BASE            + 0x448)
#define HWIO_MSS_QDSP6SS_RESET_STATUS_RMSK                                           0x1ffff
#define HWIO_MSS_QDSP6SS_RESET_STATUS_IN                    \
                in_dword(HWIO_MSS_QDSP6SS_RESET_STATUS_ADDR)
#define HWIO_MSS_QDSP6SS_RESET_STATUS_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_RESET_STATUS_ADDR, m)
#define HWIO_MSS_QDSP6SS_RESET_STATUS_CLADE2_BMSK                                    0x10000
#define HWIO_MSS_QDSP6SS_RESET_STATUS_CLADE2_SHFT                                         16
#define HWIO_MSS_QDSP6SS_RESET_STATUS_VPEVU_BMSK                                      0x8000
#define HWIO_MSS_QDSP6SS_RESET_STATUS_VPEVU_SHFT                                          15
#define HWIO_MSS_QDSP6SS_RESET_STATUS_NRET_BMSK                                       0x4000
#define HWIO_MSS_QDSP6SS_RESET_STATUS_NRET_SHFT                                           14
#define HWIO_MSS_QDSP6SS_RESET_STATUS_MNOC_BMSK                                       0x2000
#define HWIO_MSS_QDSP6SS_RESET_STATUS_MNOC_SHFT                                           13
#define HWIO_MSS_QDSP6SS_RESET_STATUS_APB_BMSK                                        0x1000
#define HWIO_MSS_QDSP6SS_RESET_STATUS_APB_SHFT                                            12
#define HWIO_MSS_QDSP6SS_RESET_STATUS_AXIS2_BMSK                                       0x800
#define HWIO_MSS_QDSP6SS_RESET_STATUS_AXIS2_SHFT                                          11
#define HWIO_MSS_QDSP6SS_RESET_STATUS_CSI1_BMSK                                        0x400
#define HWIO_MSS_QDSP6SS_RESET_STATUS_CSI1_SHFT                                           10
#define HWIO_MSS_QDSP6SS_RESET_STATUS_CSI0_BMSK                                        0x200
#define HWIO_MSS_QDSP6SS_RESET_STATUS_CSI0_SHFT                                            9
#define HWIO_MSS_QDSP6SS_RESET_STATUS_ARCH_BMSK                                        0x100
#define HWIO_MSS_QDSP6SS_RESET_STATUS_ARCH_SHFT                                            8
#define HWIO_MSS_QDSP6SS_RESET_STATUS_CORE_BMSK                                         0x80
#define HWIO_MSS_QDSP6SS_RESET_STATUS_CORE_SHFT                                            7
#define HWIO_MSS_QDSP6SS_RESET_STATUS_AXIM2_BMSK                                        0x40
#define HWIO_MSS_QDSP6SS_RESET_STATUS_AXIM2_SHFT                                           6
#define HWIO_MSS_QDSP6SS_RESET_STATUS_AXIS_BMSK                                         0x20
#define HWIO_MSS_QDSP6SS_RESET_STATUS_AXIS_SHFT                                            5
#define HWIO_MSS_QDSP6SS_RESET_STATUS_ATBM_BMSK                                         0x10
#define HWIO_MSS_QDSP6SS_RESET_STATUS_ATBM_SHFT                                            4
#define HWIO_MSS_QDSP6SS_RESET_STATUS_AHBM_BMSK                                          0x8
#define HWIO_MSS_QDSP6SS_RESET_STATUS_AHBM_SHFT                                            3
#define HWIO_MSS_QDSP6SS_RESET_STATUS_AXIM_BMSK                                          0x4
#define HWIO_MSS_QDSP6SS_RESET_STATUS_AXIM_SHFT                                            2
#define HWIO_MSS_QDSP6SS_RESET_STATUS_ISDB_ETM_BMSK                                      0x2
#define HWIO_MSS_QDSP6SS_RESET_STATUS_ISDB_ETM_SHFT                                        1
#define HWIO_MSS_QDSP6SS_RESET_STATUS_RET_BMSK                                           0x1
#define HWIO_MSS_QDSP6SS_RESET_STATUS_RET_SHFT                                             0

#define HWIO_MSS_QDSP6SS_CLAMP_CFG_ADDR                                           (MSS_QDSP6V671SS_PUB_REG_BASE            + 0x450)
#define HWIO_MSS_QDSP6SS_CLAMP_CFG_RMSK                                                  0xd
#define HWIO_MSS_QDSP6SS_CLAMP_CFG_IN                    \
                in_dword(HWIO_MSS_QDSP6SS_CLAMP_CFG_ADDR)
#define HWIO_MSS_QDSP6SS_CLAMP_CFG_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_CLAMP_CFG_ADDR, m)
#define HWIO_MSS_QDSP6SS_CLAMP_CFG_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_CLAMP_CFG_ADDR,v)
#define HWIO_MSS_QDSP6SS_CLAMP_CFG_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_QDSP6SS_CLAMP_CFG_ADDR,m,v,HWIO_MSS_QDSP6SS_CLAMP_CFG_IN)
#define HWIO_MSS_QDSP6SS_CLAMP_CFG_CLAMP_MSS2CX_BMSK                                     0x8
#define HWIO_MSS_QDSP6SS_CLAMP_CFG_CLAMP_MSS2CX_SHFT                                       3
#define HWIO_MSS_QDSP6SS_CLAMP_CFG_CLAMP_QMC_MEM_BMSK                                    0x4
#define HWIO_MSS_QDSP6SS_CLAMP_CFG_CLAMP_QMC_MEM_SHFT                                      2
#define HWIO_MSS_QDSP6SS_CLAMP_CFG_CLAMP_IO_BMSK                                         0x1
#define HWIO_MSS_QDSP6SS_CLAMP_CFG_CLAMP_IO_SHFT                                           0

#define HWIO_MSS_QDSP6SS_CLAMP_CMD_ADDR                                           (MSS_QDSP6V671SS_PUB_REG_BASE            + 0x454)
#define HWIO_MSS_QDSP6SS_CLAMP_CMD_RMSK                                                  0xd
#define HWIO_MSS_QDSP6SS_CLAMP_CMD_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_CLAMP_CMD_ADDR,v)
#define HWIO_MSS_QDSP6SS_CLAMP_CMD_UPDATE_CLAMP_MSS2CX_BMSK                              0x8
#define HWIO_MSS_QDSP6SS_CLAMP_CMD_UPDATE_CLAMP_MSS2CX_SHFT                                3
#define HWIO_MSS_QDSP6SS_CLAMP_CMD_UPDATE_CLAMP_QMC_MEM_BMSK                             0x4
#define HWIO_MSS_QDSP6SS_CLAMP_CMD_UPDATE_CLAMP_QMC_MEM_SHFT                               2
#define HWIO_MSS_QDSP6SS_CLAMP_CMD_UPDATE_CLAMP_IO_BMSK                                  0x1
#define HWIO_MSS_QDSP6SS_CLAMP_CMD_UPDATE_CLAMP_IO_SHFT                                    0

#define HWIO_MSS_QDSP6SS_CLAMP_STATUS_ADDR                                        (MSS_QDSP6V671SS_PUB_REG_BASE            + 0x458)
#define HWIO_MSS_QDSP6SS_CLAMP_STATUS_RMSK                                               0xd
#define HWIO_MSS_QDSP6SS_CLAMP_STATUS_IN                    \
                in_dword(HWIO_MSS_QDSP6SS_CLAMP_STATUS_ADDR)
#define HWIO_MSS_QDSP6SS_CLAMP_STATUS_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_CLAMP_STATUS_ADDR, m)
#define HWIO_MSS_QDSP6SS_CLAMP_STATUS_CLAMP_MSS2CX_BMSK                                  0x8
#define HWIO_MSS_QDSP6SS_CLAMP_STATUS_CLAMP_MSS2CX_SHFT                                    3
#define HWIO_MSS_QDSP6SS_CLAMP_STATUS_CLAMP_QMC_MEM_BMSK                                 0x4
#define HWIO_MSS_QDSP6SS_CLAMP_STATUS_CLAMP_QMC_MEM_SHFT                                   2
#define HWIO_MSS_QDSP6SS_CLAMP_STATUS_CLAMP_IO_BMSK                                      0x1
#define HWIO_MSS_QDSP6SS_CLAMP_STATUS_CLAMP_IO_SHFT                                        0

#define HWIO_MSS_QDSP6SS_CLK_CFG_ADDR                                             (MSS_QDSP6V671SS_PUB_REG_BASE            + 0x460)
#define HWIO_MSS_QDSP6SS_CLK_CFG_RMSK                                                  0x7ff
#define HWIO_MSS_QDSP6SS_CLK_CFG_IN                    \
                in_dword(HWIO_MSS_QDSP6SS_CLK_CFG_ADDR)
#define HWIO_MSS_QDSP6SS_CLK_CFG_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_CLK_CFG_ADDR, m)
#define HWIO_MSS_QDSP6SS_CLK_CFG_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_CLK_CFG_ADDR,v)
#define HWIO_MSS_QDSP6SS_CLK_CFG_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_QDSP6SS_CLK_CFG_ADDR,m,v,HWIO_MSS_QDSP6SS_CLK_CFG_IN)
#define HWIO_MSS_QDSP6SS_CLK_CFG_VPEVU_BMSK                                            0x400
#define HWIO_MSS_QDSP6SS_CLK_CFG_VPEVU_SHFT                                               10
#define HWIO_MSS_QDSP6SS_CLK_CFG_AXIS2_BMSK                                            0x200
#define HWIO_MSS_QDSP6SS_CLK_CFG_AXIS2_SHFT                                                9
#define HWIO_MSS_QDSP6SS_CLK_CFG_CSI1_BMSK                                             0x100
#define HWIO_MSS_QDSP6SS_CLK_CFG_CSI1_SHFT                                                 8
#define HWIO_MSS_QDSP6SS_CLK_CFG_CSI0_BMSK                                              0x80
#define HWIO_MSS_QDSP6SS_CLK_CFG_CSI0_SHFT                                                 7
#define HWIO_MSS_QDSP6SS_CLK_CFG_AXIM2_BMSK                                             0x40
#define HWIO_MSS_QDSP6SS_CLK_CFG_AXIM2_SHFT                                                6
#define HWIO_MSS_QDSP6SS_CLK_CFG_AXIS_BMSK                                              0x20
#define HWIO_MSS_QDSP6SS_CLK_CFG_AXIS_SHFT                                                 5
#define HWIO_MSS_QDSP6SS_CLK_CFG_ATBM_BMSK                                              0x10
#define HWIO_MSS_QDSP6SS_CLK_CFG_ATBM_SHFT                                                 4
#define HWIO_MSS_QDSP6SS_CLK_CFG_AHBM_BMSK                                               0x8
#define HWIO_MSS_QDSP6SS_CLK_CFG_AHBM_SHFT                                                 3
#define HWIO_MSS_QDSP6SS_CLK_CFG_AXIM_BMSK                                               0x4
#define HWIO_MSS_QDSP6SS_CLK_CFG_AXIM_SHFT                                                 2
#define HWIO_MSS_QDSP6SS_CLK_CFG_APB_BMSK                                                0x2
#define HWIO_MSS_QDSP6SS_CLK_CFG_APB_SHFT                                                  1
#define HWIO_MSS_QDSP6SS_CLK_CFG_CORE_BMSK                                               0x1
#define HWIO_MSS_QDSP6SS_CLK_CFG_CORE_SHFT                                                 0

#define HWIO_MSS_QDSP6SS_CLK_CMD_ADDR                                             (MSS_QDSP6V671SS_PUB_REG_BASE            + 0x464)
#define HWIO_MSS_QDSP6SS_CLK_CMD_RMSK                                                    0x1
#define HWIO_MSS_QDSP6SS_CLK_CMD_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_CLK_CMD_ADDR,v)
#define HWIO_MSS_QDSP6SS_CLK_CMD_UPDATE_CLK_BMSK                                         0x1
#define HWIO_MSS_QDSP6SS_CLK_CMD_UPDATE_CLK_SHFT                                           0

#define HWIO_MSS_QDSP6SS_CLK_STATUS_ADDR                                          (MSS_QDSP6V671SS_PUB_REG_BASE            + 0x468)
#define HWIO_MSS_QDSP6SS_CLK_STATUS_RMSK                                               0x7ff
#define HWIO_MSS_QDSP6SS_CLK_STATUS_IN                    \
                in_dword(HWIO_MSS_QDSP6SS_CLK_STATUS_ADDR)
#define HWIO_MSS_QDSP6SS_CLK_STATUS_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_CLK_STATUS_ADDR, m)
#define HWIO_MSS_QDSP6SS_CLK_STATUS_VPEVU_BMSK                                         0x400
#define HWIO_MSS_QDSP6SS_CLK_STATUS_VPEVU_SHFT                                            10
#define HWIO_MSS_QDSP6SS_CLK_STATUS_AXIS2_BMSK                                         0x200
#define HWIO_MSS_QDSP6SS_CLK_STATUS_AXIS2_SHFT                                             9
#define HWIO_MSS_QDSP6SS_CLK_STATUS_CSI1_BMSK                                          0x100
#define HWIO_MSS_QDSP6SS_CLK_STATUS_CSI1_SHFT                                              8
#define HWIO_MSS_QDSP6SS_CLK_STATUS_CSI0_BMSK                                           0x80
#define HWIO_MSS_QDSP6SS_CLK_STATUS_CSI0_SHFT                                              7
#define HWIO_MSS_QDSP6SS_CLK_STATUS_AXIM2_BMSK                                          0x40
#define HWIO_MSS_QDSP6SS_CLK_STATUS_AXIM2_SHFT                                             6
#define HWIO_MSS_QDSP6SS_CLK_STATUS_AXIS_BMSK                                           0x20
#define HWIO_MSS_QDSP6SS_CLK_STATUS_AXIS_SHFT                                              5
#define HWIO_MSS_QDSP6SS_CLK_STATUS_ATBM_BMSK                                           0x10
#define HWIO_MSS_QDSP6SS_CLK_STATUS_ATBM_SHFT                                              4
#define HWIO_MSS_QDSP6SS_CLK_STATUS_AHBM_BMSK                                            0x8
#define HWIO_MSS_QDSP6SS_CLK_STATUS_AHBM_SHFT                                              3
#define HWIO_MSS_QDSP6SS_CLK_STATUS_AXIM_BMSK                                            0x4
#define HWIO_MSS_QDSP6SS_CLK_STATUS_AXIM_SHFT                                              2
#define HWIO_MSS_QDSP6SS_CLK_STATUS_APB_BMSK                                             0x2
#define HWIO_MSS_QDSP6SS_CLK_STATUS_APB_SHFT                                               1
#define HWIO_MSS_QDSP6SS_CLK_STATUS_CORE_BMSK                                            0x1
#define HWIO_MSS_QDSP6SS_CLK_STATUS_CORE_SHFT                                              0

#define HWIO_MSS_QDSP6SS_MEM_STAGGER_RESET_CFG_ADDR                               (MSS_QDSP6V671SS_PUB_REG_BASE            + 0x470)
#define HWIO_MSS_QDSP6SS_MEM_STAGGER_RESET_CFG_RMSK                                      0x3
#define HWIO_MSS_QDSP6SS_MEM_STAGGER_RESET_CFG_IN                    \
                in_dword(HWIO_MSS_QDSP6SS_MEM_STAGGER_RESET_CFG_ADDR)
#define HWIO_MSS_QDSP6SS_MEM_STAGGER_RESET_CFG_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_MEM_STAGGER_RESET_CFG_ADDR, m)
#define HWIO_MSS_QDSP6SS_MEM_STAGGER_RESET_CFG_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_MEM_STAGGER_RESET_CFG_ADDR,v)
#define HWIO_MSS_QDSP6SS_MEM_STAGGER_RESET_CFG_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_QDSP6SS_MEM_STAGGER_RESET_CFG_ADDR,m,v,HWIO_MSS_QDSP6SS_MEM_STAGGER_RESET_CFG_IN)
#define HWIO_MSS_QDSP6SS_MEM_STAGGER_RESET_CFG_STAGGER_ALT_ARES_BMSK                     0x2
#define HWIO_MSS_QDSP6SS_MEM_STAGGER_RESET_CFG_STAGGER_ALT_ARES_SHFT                       1
#define HWIO_MSS_QDSP6SS_MEM_STAGGER_RESET_CFG_STAGGER_FUNC_ARES_BMSK                    0x1
#define HWIO_MSS_QDSP6SS_MEM_STAGGER_RESET_CFG_STAGGER_FUNC_ARES_SHFT                      0

#define HWIO_MSS_QDSP6SS_MEM_STAGGER_RESET_CMD_ADDR                               (MSS_QDSP6V671SS_PUB_REG_BASE            + 0x474)
#define HWIO_MSS_QDSP6SS_MEM_STAGGER_RESET_CMD_RMSK                                      0x1
#define HWIO_MSS_QDSP6SS_MEM_STAGGER_RESET_CMD_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_MEM_STAGGER_RESET_CMD_ADDR,v)
#define HWIO_MSS_QDSP6SS_MEM_STAGGER_RESET_CMD_UPDATE_BMSK                               0x1
#define HWIO_MSS_QDSP6SS_MEM_STAGGER_RESET_CMD_UPDATE_SHFT                                 0

#define HWIO_MSS_QDSP6SS_MEM_STAGGER_RESET_STATUS_ADDR                            (MSS_QDSP6V671SS_PUB_REG_BASE            + 0x478)
#define HWIO_MSS_QDSP6SS_MEM_STAGGER_RESET_STATUS_RMSK                                   0x3
#define HWIO_MSS_QDSP6SS_MEM_STAGGER_RESET_STATUS_IN                    \
                in_dword(HWIO_MSS_QDSP6SS_MEM_STAGGER_RESET_STATUS_ADDR)
#define HWIO_MSS_QDSP6SS_MEM_STAGGER_RESET_STATUS_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_MEM_STAGGER_RESET_STATUS_ADDR, m)
#define HWIO_MSS_QDSP6SS_MEM_STAGGER_RESET_STATUS_STAGGER_ALT_ARES_BMSK                  0x2
#define HWIO_MSS_QDSP6SS_MEM_STAGGER_RESET_STATUS_STAGGER_ALT_ARES_SHFT                    1
#define HWIO_MSS_QDSP6SS_MEM_STAGGER_RESET_STATUS_STAGGER_FUNC_ARES_BMSK                 0x1
#define HWIO_MSS_QDSP6SS_MEM_STAGGER_RESET_STATUS_STAGGER_FUNC_ARES_SHFT                   0

#define HWIO_MSS_QDSP6SS_MEM_STAGGER_RESTORE_CMD_ADDR                             (MSS_QDSP6V671SS_PUB_REG_BASE            + 0x484)
#define HWIO_MSS_QDSP6SS_MEM_STAGGER_RESTORE_CMD_RMSK                                    0x1
#define HWIO_MSS_QDSP6SS_MEM_STAGGER_RESTORE_CMD_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_MEM_STAGGER_RESTORE_CMD_ADDR,v)
#define HWIO_MSS_QDSP6SS_MEM_STAGGER_RESTORE_CMD_RESTORE_CMD_BMSK                        0x1
#define HWIO_MSS_QDSP6SS_MEM_STAGGER_RESTORE_CMD_RESTORE_CMD_SHFT                          0

#define HWIO_MSS_QDSP6SS_MEM_STAGGER_RESTORE_STATUS_ADDR                          (MSS_QDSP6V671SS_PUB_REG_BASE            + 0x488)
#define HWIO_MSS_QDSP6SS_MEM_STAGGER_RESTORE_STATUS_RMSK                                 0x1
#define HWIO_MSS_QDSP6SS_MEM_STAGGER_RESTORE_STATUS_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_MEM_STAGGER_RESTORE_STATUS_ADDR,v)
#define HWIO_MSS_QDSP6SS_MEM_STAGGER_RESTORE_STATUS_RESTORE_BUSY_BMSK                    0x1
#define HWIO_MSS_QDSP6SS_MEM_STAGGER_RESTORE_STATUS_RESTORE_BUSY_SHFT                      0

#define HWIO_MSS_QDSP6SS_L2MEM_EFUSE_STATUS_ADDR                                  (MSS_QDSP6V671SS_PUB_REG_BASE            + 0x490)
#define HWIO_MSS_QDSP6SS_L2MEM_EFUSE_STATUS_RMSK                                         0xf
#define HWIO_MSS_QDSP6SS_L2MEM_EFUSE_STATUS_IN                    \
                in_dword(HWIO_MSS_QDSP6SS_L2MEM_EFUSE_STATUS_ADDR)
#define HWIO_MSS_QDSP6SS_L2MEM_EFUSE_STATUS_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_L2MEM_EFUSE_STATUS_ADDR, m)
#define HWIO_MSS_QDSP6SS_L2MEM_EFUSE_STATUS_EFUSE_L2TCM_EN_BMSK                          0xf
#define HWIO_MSS_QDSP6SS_L2MEM_EFUSE_STATUS_EFUSE_L2TCM_EN_SHFT                            0

#define HWIO_MSS_QDSP6SS_CP_CLK_CTL_ADDR                                          (MSS_QDSP6V671SS_PUB_REG_BASE            + 0x508)
#define HWIO_MSS_QDSP6SS_CP_CLK_CTL_RMSK                                                 0x1
#define HWIO_MSS_QDSP6SS_CP_CLK_CTL_IN                    \
                in_dword(HWIO_MSS_QDSP6SS_CP_CLK_CTL_ADDR)
#define HWIO_MSS_QDSP6SS_CP_CLK_CTL_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_CP_CLK_CTL_ADDR, m)
#define HWIO_MSS_QDSP6SS_CP_CLK_CTL_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_CP_CLK_CTL_ADDR,v)
#define HWIO_MSS_QDSP6SS_CP_CLK_CTL_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_QDSP6SS_CP_CLK_CTL_ADDR,m,v,HWIO_MSS_QDSP6SS_CP_CLK_CTL_IN)
#define HWIO_MSS_QDSP6SS_CP_CLK_CTL_CLK_ENA_BMSK                                         0x1
#define HWIO_MSS_QDSP6SS_CP_CLK_CTL_CLK_ENA_SHFT                                           0

#define HWIO_MSS_QDSP6SS_CPMEM_CFG_ADDR                                           (MSS_QDSP6V671SS_PUB_REG_BASE            + 0x520)
#define HWIO_MSS_QDSP6SS_CPMEM_CFG_RMSK                                                  0x3
#define HWIO_MSS_QDSP6SS_CPMEM_CFG_IN                    \
                in_dword(HWIO_MSS_QDSP6SS_CPMEM_CFG_ADDR)
#define HWIO_MSS_QDSP6SS_CPMEM_CFG_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_CPMEM_CFG_ADDR, m)
#define HWIO_MSS_QDSP6SS_CPMEM_CFG_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_CPMEM_CFG_ADDR,v)
#define HWIO_MSS_QDSP6SS_CPMEM_CFG_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_QDSP6SS_CPMEM_CFG_ADDR,m,v,HWIO_MSS_QDSP6SS_CPMEM_CFG_IN)
#define HWIO_MSS_QDSP6SS_CPMEM_CFG_VTCM_SLP_RET_N_BMSK                                   0x2
#define HWIO_MSS_QDSP6SS_CPMEM_CFG_VTCM_SLP_RET_N_SHFT                                     1
#define HWIO_MSS_QDSP6SS_CPMEM_CFG_VTCM_SLP_NRET_N_BMSK                                  0x1
#define HWIO_MSS_QDSP6SS_CPMEM_CFG_VTCM_SLP_NRET_N_SHFT                                    0

#define HWIO_MSS_QDSP6SS_CPMEM_CMD_ADDR                                           (MSS_QDSP6V671SS_PUB_REG_BASE            + 0x524)
#define HWIO_MSS_QDSP6SS_CPMEM_CMD_RMSK                                                  0x3
#define HWIO_MSS_QDSP6SS_CPMEM_CMD_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_CPMEM_CMD_ADDR,v)
#define HWIO_MSS_QDSP6SS_CPMEM_CMD_UPDATE_VTCM_SLP_RET_N_BMSK                            0x2
#define HWIO_MSS_QDSP6SS_CPMEM_CMD_UPDATE_VTCM_SLP_RET_N_SHFT                              1
#define HWIO_MSS_QDSP6SS_CPMEM_CMD_UPDATE_VTCM_SLP_NRET_N_BMSK                           0x1
#define HWIO_MSS_QDSP6SS_CPMEM_CMD_UPDATE_VTCM_SLP_NRET_N_SHFT                             0

#define HWIO_MSS_QDSP6SS_CPMEM_STATUS_ADDR                                        (MSS_QDSP6V671SS_PUB_REG_BASE            + 0x528)
#define HWIO_MSS_QDSP6SS_CPMEM_STATUS_RMSK                                               0x3
#define HWIO_MSS_QDSP6SS_CPMEM_STATUS_IN                    \
                in_dword(HWIO_MSS_QDSP6SS_CPMEM_STATUS_ADDR)
#define HWIO_MSS_QDSP6SS_CPMEM_STATUS_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_CPMEM_STATUS_ADDR, m)
#define HWIO_MSS_QDSP6SS_CPMEM_STATUS_VTCM_SLP_RET_N_BMSK                                0x2
#define HWIO_MSS_QDSP6SS_CPMEM_STATUS_VTCM_SLP_RET_N_SHFT                                  1
#define HWIO_MSS_QDSP6SS_CPMEM_STATUS_VTCM_SLP_NRET_N_BMSK                               0x1
#define HWIO_MSS_QDSP6SS_CPMEM_STATUS_VTCM_SLP_NRET_N_SHFT                                 0

#define HWIO_MSS_QDSP6SS_L2ITCM_CFG_ADDR                                          (MSS_QDSP6V671SS_PUB_REG_BASE            + 0x530)
#define HWIO_MSS_QDSP6SS_L2ITCM_CFG_RMSK                                             0x30003
#define HWIO_MSS_QDSP6SS_L2ITCM_CFG_IN                    \
                in_dword(HWIO_MSS_QDSP6SS_L2ITCM_CFG_ADDR)
#define HWIO_MSS_QDSP6SS_L2ITCM_CFG_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_L2ITCM_CFG_ADDR, m)
#define HWIO_MSS_QDSP6SS_L2ITCM_CFG_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_L2ITCM_CFG_ADDR,v)
#define HWIO_MSS_QDSP6SS_L2ITCM_CFG_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_QDSP6SS_L2ITCM_CFG_ADDR,m,v,HWIO_MSS_QDSP6SS_L2ITCM_CFG_IN)
#define HWIO_MSS_QDSP6SS_L2ITCM_CFG_L2ITCM_SLP_RET_N_BMSK                            0x30000
#define HWIO_MSS_QDSP6SS_L2ITCM_CFG_L2ITCM_SLP_RET_N_SHFT                                 16
#define HWIO_MSS_QDSP6SS_L2ITCM_CFG_L2ITCM_SLP_NRET_N_BMSK                               0x3
#define HWIO_MSS_QDSP6SS_L2ITCM_CFG_L2ITCM_SLP_NRET_N_SHFT                                 0

#define HWIO_MSS_QDSP6SS_L2ITCM_CMD_ADDR                                          (MSS_QDSP6V671SS_PUB_REG_BASE            + 0x534)
#define HWIO_MSS_QDSP6SS_L2ITCM_CMD_RMSK                                                 0x3
#define HWIO_MSS_QDSP6SS_L2ITCM_CMD_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_L2ITCM_CMD_ADDR,v)
#define HWIO_MSS_QDSP6SS_L2ITCM_CMD_UPDATE_L2ITCM_SLP_RET_N_BMSK                         0x2
#define HWIO_MSS_QDSP6SS_L2ITCM_CMD_UPDATE_L2ITCM_SLP_RET_N_SHFT                           1
#define HWIO_MSS_QDSP6SS_L2ITCM_CMD_UPDATE_L2ITCM_SLP_NRET_N_BMSK                        0x1
#define HWIO_MSS_QDSP6SS_L2ITCM_CMD_UPDATE_L2ITCM_SLP_NRET_N_SHFT                          0

#define HWIO_MSS_QDSP6SS_L2ITCM_STATUS_ADDR                                       (MSS_QDSP6V671SS_PUB_REG_BASE            + 0x538)
#define HWIO_MSS_QDSP6SS_L2ITCM_STATUS_RMSK                                          0x30003
#define HWIO_MSS_QDSP6SS_L2ITCM_STATUS_IN                    \
                in_dword(HWIO_MSS_QDSP6SS_L2ITCM_STATUS_ADDR)
#define HWIO_MSS_QDSP6SS_L2ITCM_STATUS_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_L2ITCM_STATUS_ADDR, m)
#define HWIO_MSS_QDSP6SS_L2ITCM_STATUS_L2ITCM_SLP_RET_N_BMSK                         0x30000
#define HWIO_MSS_QDSP6SS_L2ITCM_STATUS_L2ITCM_SLP_RET_N_SHFT                              16
#define HWIO_MSS_QDSP6SS_L2ITCM_STATUS_L2ITCM_SLP_NRET_N_BMSK                            0x3
#define HWIO_MSS_QDSP6SS_L2ITCM_STATUS_L2ITCM_SLP_NRET_N_SHFT                              0

#define HWIO_MSS_QDSP6SS_L1SMEM_CFG_ADDR                                          (MSS_QDSP6V671SS_PUB_REG_BASE            + 0x540)
#define HWIO_MSS_QDSP6SS_L1SMEM_CFG_RMSK                                          0x3f1f3f1f
#define HWIO_MSS_QDSP6SS_L1SMEM_CFG_IN                    \
                in_dword(HWIO_MSS_QDSP6SS_L1SMEM_CFG_ADDR)
#define HWIO_MSS_QDSP6SS_L1SMEM_CFG_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_L1SMEM_CFG_ADDR, m)
#define HWIO_MSS_QDSP6SS_L1SMEM_CFG_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_L1SMEM_CFG_ADDR,v)
#define HWIO_MSS_QDSP6SS_L1SMEM_CFG_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_QDSP6SS_L1SMEM_CFG_ADDR,m,v,HWIO_MSS_QDSP6SS_L1SMEM_CFG_IN)
#define HWIO_MSS_QDSP6SS_L1SMEM_CFG_L1S2_SLP_RET_N_BMSK                           0x3f000000
#define HWIO_MSS_QDSP6SS_L1SMEM_CFG_L1S2_SLP_RET_N_SHFT                                   24
#define HWIO_MSS_QDSP6SS_L1SMEM_CFG_L1S_SLP_RET_N_BMSK                              0x1f0000
#define HWIO_MSS_QDSP6SS_L1SMEM_CFG_L1S_SLP_RET_N_SHFT                                    16
#define HWIO_MSS_QDSP6SS_L1SMEM_CFG_L1S2_SLP_NRET_N_BMSK                              0x3f00
#define HWIO_MSS_QDSP6SS_L1SMEM_CFG_L1S2_SLP_NRET_N_SHFT                                   8
#define HWIO_MSS_QDSP6SS_L1SMEM_CFG_L1S_SLP_NRET_N_BMSK                                 0x1f
#define HWIO_MSS_QDSP6SS_L1SMEM_CFG_L1S_SLP_NRET_N_SHFT                                    0

#define HWIO_MSS_QDSP6SS_L1SMEM_CMD_ADDR                                          (MSS_QDSP6V671SS_PUB_REG_BASE            + 0x544)
#define HWIO_MSS_QDSP6SS_L1SMEM_CMD_RMSK                                                 0xf
#define HWIO_MSS_QDSP6SS_L1SMEM_CMD_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_L1SMEM_CMD_ADDR,v)
#define HWIO_MSS_QDSP6SS_L1SMEM_CMD_UPDATE_L1S2_SLP_RET_N_BMSK                           0x8
#define HWIO_MSS_QDSP6SS_L1SMEM_CMD_UPDATE_L1S2_SLP_RET_N_SHFT                             3
#define HWIO_MSS_QDSP6SS_L1SMEM_CMD_UPDATE_L1S2_SLP_NRET_N_BMSK                          0x4
#define HWIO_MSS_QDSP6SS_L1SMEM_CMD_UPDATE_L1S2_SLP_NRET_N_SHFT                            2
#define HWIO_MSS_QDSP6SS_L1SMEM_CMD_UPDATE_L1S_SLP_RET_N_BMSK                            0x2
#define HWIO_MSS_QDSP6SS_L1SMEM_CMD_UPDATE_L1S_SLP_RET_N_SHFT                              1
#define HWIO_MSS_QDSP6SS_L1SMEM_CMD_UPDATE_L1S_SLP_NRET_N_BMSK                           0x1
#define HWIO_MSS_QDSP6SS_L1SMEM_CMD_UPDATE_L1S_SLP_NRET_N_SHFT                             0

#define HWIO_MSS_QDSP6SS_L1SMEM_STATUS_ADDR                                       (MSS_QDSP6V671SS_PUB_REG_BASE            + 0x548)
#define HWIO_MSS_QDSP6SS_L1SMEM_STATUS_RMSK                                       0x3f1f3f1f
#define HWIO_MSS_QDSP6SS_L1SMEM_STATUS_IN                    \
                in_dword(HWIO_MSS_QDSP6SS_L1SMEM_STATUS_ADDR)
#define HWIO_MSS_QDSP6SS_L1SMEM_STATUS_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_L1SMEM_STATUS_ADDR, m)
#define HWIO_MSS_QDSP6SS_L1SMEM_STATUS_L1S2_SLP_RET_N_BMSK                        0x3f000000
#define HWIO_MSS_QDSP6SS_L1SMEM_STATUS_L1S2_SLP_RET_N_SHFT                                24
#define HWIO_MSS_QDSP6SS_L1SMEM_STATUS_L1S_SLP_RET_N_BMSK                           0x1f0000
#define HWIO_MSS_QDSP6SS_L1SMEM_STATUS_L1S_SLP_RET_N_SHFT                                 16
#define HWIO_MSS_QDSP6SS_L1SMEM_STATUS_L1S2_SLP_NRET_N_BMSK                           0x3f00
#define HWIO_MSS_QDSP6SS_L1SMEM_STATUS_L1S2_SLP_NRET_N_SHFT                                8
#define HWIO_MSS_QDSP6SS_L1SMEM_STATUS_L1S_SLP_NRET_N_BMSK                              0x1f
#define HWIO_MSS_QDSP6SS_L1SMEM_STATUS_L1S_SLP_NRET_N_SHFT                                 0

#define HWIO_MSS_QDSP6SS_VU0_PWR_VOTE_SET_ADDR                                    (MSS_QDSP6V671SS_PUB_REG_BASE            + 0x600)
#define HWIO_MSS_QDSP6SS_VU0_PWR_VOTE_SET_RMSK                                          0xff
#define HWIO_MSS_QDSP6SS_VU0_PWR_VOTE_SET_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_VU0_PWR_VOTE_SET_ADDR,v)
#define HWIO_MSS_QDSP6SS_VU0_PWR_VOTE_SET_SET_BMSK                                      0xff
#define HWIO_MSS_QDSP6SS_VU0_PWR_VOTE_SET_SET_SHFT                                         0

#define HWIO_MSS_QDSP6SS_VU0_PWR_VOTE_CLR_ADDR                                    (MSS_QDSP6V671SS_PUB_REG_BASE            + 0x604)
#define HWIO_MSS_QDSP6SS_VU0_PWR_VOTE_CLR_RMSK                                         0xfff
#define HWIO_MSS_QDSP6SS_VU0_PWR_VOTE_CLR_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_VU0_PWR_VOTE_CLR_ADDR,v)
#define HWIO_MSS_QDSP6SS_VU0_PWR_VOTE_CLR_CLR_BMSK                                     0xfff
#define HWIO_MSS_QDSP6SS_VU0_PWR_VOTE_CLR_CLR_SHFT                                         0

#define HWIO_MSS_QDSP6SS_VU0_PWR_VOTE_MASK_ADDR                                   (MSS_QDSP6V671SS_PUB_REG_BASE            + 0x608)
#define HWIO_MSS_QDSP6SS_VU0_PWR_VOTE_MASK_RMSK                                        0xfff
#define HWIO_MSS_QDSP6SS_VU0_PWR_VOTE_MASK_IN                    \
                in_dword(HWIO_MSS_QDSP6SS_VU0_PWR_VOTE_MASK_ADDR)
#define HWIO_MSS_QDSP6SS_VU0_PWR_VOTE_MASK_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_VU0_PWR_VOTE_MASK_ADDR, m)
#define HWIO_MSS_QDSP6SS_VU0_PWR_VOTE_MASK_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_VU0_PWR_VOTE_MASK_ADDR,v)
#define HWIO_MSS_QDSP6SS_VU0_PWR_VOTE_MASK_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_QDSP6SS_VU0_PWR_VOTE_MASK_ADDR,m,v,HWIO_MSS_QDSP6SS_VU0_PWR_VOTE_MASK_IN)
#define HWIO_MSS_QDSP6SS_VU0_PWR_VOTE_MASK_HW_VOTE_EN_BMSK                             0xf00
#define HWIO_MSS_QDSP6SS_VU0_PWR_VOTE_MASK_HW_VOTE_EN_SHFT                                 8
#define HWIO_MSS_QDSP6SS_VU0_PWR_VOTE_MASK_VOTE_EN_BMSK                                 0xff
#define HWIO_MSS_QDSP6SS_VU0_PWR_VOTE_MASK_VOTE_EN_SHFT                                    0

#define HWIO_MSS_QDSP6SS_VU0_PWR_STATUS_ADDR                                      (MSS_QDSP6V671SS_PUB_REG_BASE            + 0x60c)
#define HWIO_MSS_QDSP6SS_VU0_PWR_STATUS_RMSK                                         0xfff1f
#define HWIO_MSS_QDSP6SS_VU0_PWR_STATUS_IN                    \
                in_dword(HWIO_MSS_QDSP6SS_VU0_PWR_STATUS_ADDR)
#define HWIO_MSS_QDSP6SS_VU0_PWR_STATUS_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_VU0_PWR_STATUS_ADDR, m)
#define HWIO_MSS_QDSP6SS_VU0_PWR_STATUS_VOTE_STAT_BMSK                               0xfff00
#define HWIO_MSS_QDSP6SS_VU0_PWR_STATUS_VOTE_STAT_SHFT                                     8
#define HWIO_MSS_QDSP6SS_VU0_PWR_STATUS_WAIT_NEXT_BMSK                                  0x10
#define HWIO_MSS_QDSP6SS_VU0_PWR_STATUS_WAIT_NEXT_SHFT                                     4
#define HWIO_MSS_QDSP6SS_VU0_PWR_STATUS_WAIT_PREV_BMSK                                   0x8
#define HWIO_MSS_QDSP6SS_VU0_PWR_STATUS_WAIT_PREV_SHFT                                     3
#define HWIO_MSS_QDSP6SS_VU0_PWR_STATUS_PWR_DOWN_BMSK                                    0x4
#define HWIO_MSS_QDSP6SS_VU0_PWR_STATUS_PWR_DOWN_SHFT                                      2
#define HWIO_MSS_QDSP6SS_VU0_PWR_STATUS_PWR_REQ_BMSK                                     0x2
#define HWIO_MSS_QDSP6SS_VU0_PWR_STATUS_PWR_REQ_SHFT                                       1
#define HWIO_MSS_QDSP6SS_VU0_PWR_STATUS_PWR_ON_BMSK                                      0x1
#define HWIO_MSS_QDSP6SS_VU0_PWR_STATUS_PWR_ON_SHFT                                        0

#define HWIO_MSS_QDSP6SS_VU0_PWR_DEBUG_ADDR                                       (MSS_QDSP6V671SS_PUB_REG_BASE            + 0x610)
#define HWIO_MSS_QDSP6SS_VU0_PWR_DEBUG_RMSK                                             0xf7
#define HWIO_MSS_QDSP6SS_VU0_PWR_DEBUG_IN                    \
                in_dword(HWIO_MSS_QDSP6SS_VU0_PWR_DEBUG_ADDR)
#define HWIO_MSS_QDSP6SS_VU0_PWR_DEBUG_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_VU0_PWR_DEBUG_ADDR, m)
#define HWIO_MSS_QDSP6SS_VU0_PWR_DEBUG_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_VU0_PWR_DEBUG_ADDR,v)
#define HWIO_MSS_QDSP6SS_VU0_PWR_DEBUG_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_QDSP6SS_VU0_PWR_DEBUG_ADDR,m,v,HWIO_MSS_QDSP6SS_VU0_PWR_DEBUG_IN)
#define HWIO_MSS_QDSP6SS_VU0_PWR_DEBUG_SEQ_DELAY_BMSK                                   0xf0
#define HWIO_MSS_QDSP6SS_VU0_PWR_DEBUG_SEQ_DELAY_SHFT                                      4
#define HWIO_MSS_QDSP6SS_VU0_PWR_DEBUG_CLAMP_IO_BMSK                                     0x4
#define HWIO_MSS_QDSP6SS_VU0_PWR_DEBUG_CLAMP_IO_SHFT                                       2
#define HWIO_MSS_QDSP6SS_VU0_PWR_DEBUG_CP_ARES_BMSK                                      0x2
#define HWIO_MSS_QDSP6SS_VU0_PWR_DEBUG_CP_ARES_SHFT                                        1
#define HWIO_MSS_QDSP6SS_VU0_PWR_DEBUG_CP_CLK_EN_BMSK                                    0x1
#define HWIO_MSS_QDSP6SS_VU0_PWR_DEBUG_CP_CLK_EN_SHFT                                      0

#define HWIO_MSS_QDSP6SS_VU_QOS_ADDR                                              (MSS_QDSP6V671SS_PUB_REG_BASE            + 0x6a0)
#define HWIO_MSS_QDSP6SS_VU_QOS_RMSK                                                    0x70
#define HWIO_MSS_QDSP6SS_VU_QOS_IN                    \
                in_dword(HWIO_MSS_QDSP6SS_VU_QOS_ADDR)
#define HWIO_MSS_QDSP6SS_VU_QOS_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_VU_QOS_ADDR, m)
#define HWIO_MSS_QDSP6SS_VU_QOS_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_VU_QOS_ADDR,v)
#define HWIO_MSS_QDSP6SS_VU_QOS_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_QDSP6SS_VU_QOS_ADDR,m,v,HWIO_MSS_QDSP6SS_VU_QOS_IN)
#define HWIO_MSS_QDSP6SS_VU_QOS_QOS_AXIS_BMSK                                           0x70
#define HWIO_MSS_QDSP6SS_VU_QOS_QOS_AXIS_SHFT                                              4

#define HWIO_MSS_QDSP6SS_VPE_VU_CBCR_ADDR                                         (MSS_QDSP6V671SS_PUB_REG_BASE            + 0x6b0)
#define HWIO_MSS_QDSP6SS_VPE_VU_CBCR_RMSK                                         0x80000001
#define HWIO_MSS_QDSP6SS_VPE_VU_CBCR_IN                    \
                in_dword(HWIO_MSS_QDSP6SS_VPE_VU_CBCR_ADDR)
#define HWIO_MSS_QDSP6SS_VPE_VU_CBCR_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_VPE_VU_CBCR_ADDR, m)
#define HWIO_MSS_QDSP6SS_VPE_VU_CBCR_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_VPE_VU_CBCR_ADDR,v)
#define HWIO_MSS_QDSP6SS_VPE_VU_CBCR_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_QDSP6SS_VPE_VU_CBCR_ADDR,m,v,HWIO_MSS_QDSP6SS_VPE_VU_CBCR_IN)
#define HWIO_MSS_QDSP6SS_VPE_VU_CBCR_CLKOFF_BMSK                                  0x80000000
#define HWIO_MSS_QDSP6SS_VPE_VU_CBCR_CLKOFF_SHFT                                          31
#define HWIO_MSS_QDSP6SS_VPE_VU_CBCR_CLKEN_BMSK                                          0x1
#define HWIO_MSS_QDSP6SS_VPE_VU_CBCR_CLKEN_SHFT                                            0

#define HWIO_MSS_QDSP6SS_TEST_BUS_CTL_ADDR                                        (MSS_QDSP6V671SS_PUB_REG_BASE            + 0x2000)
#define HWIO_MSS_QDSP6SS_TEST_BUS_CTL_RMSK                                        0xffffffff
#define HWIO_MSS_QDSP6SS_TEST_BUS_CTL_IN                    \
                in_dword(HWIO_MSS_QDSP6SS_TEST_BUS_CTL_ADDR)
#define HWIO_MSS_QDSP6SS_TEST_BUS_CTL_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_TEST_BUS_CTL_ADDR, m)
#define HWIO_MSS_QDSP6SS_TEST_BUS_CTL_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_TEST_BUS_CTL_ADDR,v)
#define HWIO_MSS_QDSP6SS_TEST_BUS_CTL_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_QDSP6SS_TEST_BUS_CTL_ADDR,m,v,HWIO_MSS_QDSP6SS_TEST_BUS_CTL_IN)
#define HWIO_MSS_QDSP6SS_TEST_BUS_CTL_SPARE_CFG_BMSK                              0xff000000
#define HWIO_MSS_QDSP6SS_TEST_BUS_CTL_SPARE_CFG_SHFT                                      24
#define HWIO_MSS_QDSP6SS_TEST_BUS_CTL_HWE_INV_BMSK                                  0x800000
#define HWIO_MSS_QDSP6SS_TEST_BUS_CTL_HWE_INV_SHFT                                        23
#define HWIO_MSS_QDSP6SS_TEST_BUS_CTL_HWE_SEL_BMSK                                  0x7c0000
#define HWIO_MSS_QDSP6SS_TEST_BUS_CTL_HWE_SEL_SHFT                                        18
#define HWIO_MSS_QDSP6SS_TEST_BUS_CTL_TESTBUS_EN_BMSK                                0x20000
#define HWIO_MSS_QDSP6SS_TEST_BUS_CTL_TESTBUS_EN_SHFT                                     17
#define HWIO_MSS_QDSP6SS_TEST_BUS_CTL_Q6_SEL_BMSK                                    0x10000
#define HWIO_MSS_QDSP6SS_TEST_BUS_CTL_Q6_SEL_SHFT                                         16
#define HWIO_MSS_QDSP6SS_TEST_BUS_CTL_SEL_BMSK                                        0xffff
#define HWIO_MSS_QDSP6SS_TEST_BUS_CTL_SEL_SHFT                                             0

#define HWIO_MSS_QDSP6SS_TEST_BUS_VALUE_ADDR                                      (MSS_QDSP6V671SS_PUB_REG_BASE            + 0x2004)
#define HWIO_MSS_QDSP6SS_TEST_BUS_VALUE_RMSK                                      0xffffffff
#define HWIO_MSS_QDSP6SS_TEST_BUS_VALUE_IN                    \
                in_dword(HWIO_MSS_QDSP6SS_TEST_BUS_VALUE_ADDR)
#define HWIO_MSS_QDSP6SS_TEST_BUS_VALUE_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_TEST_BUS_VALUE_ADDR, m)
#define HWIO_MSS_QDSP6SS_TEST_BUS_VALUE_VALUE_BMSK                                0xffffffff
#define HWIO_MSS_QDSP6SS_TEST_BUS_VALUE_VALUE_SHFT                                         0

#define HWIO_MSS_QDSP6SS_TEST_BUS_WDATA_ADDR                                      (MSS_QDSP6V671SS_PUB_REG_BASE            + 0x2008)
#define HWIO_MSS_QDSP6SS_TEST_BUS_WDATA_RMSK                                      0xffffffff
#define HWIO_MSS_QDSP6SS_TEST_BUS_WDATA_IN                    \
                in_dword(HWIO_MSS_QDSP6SS_TEST_BUS_WDATA_ADDR)
#define HWIO_MSS_QDSP6SS_TEST_BUS_WDATA_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_TEST_BUS_WDATA_ADDR, m)
#define HWIO_MSS_QDSP6SS_TEST_BUS_WDATA_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_TEST_BUS_WDATA_ADDR,v)
#define HWIO_MSS_QDSP6SS_TEST_BUS_WDATA_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_QDSP6SS_TEST_BUS_WDATA_ADDR,m,v,HWIO_MSS_QDSP6SS_TEST_BUS_WDATA_IN)
#define HWIO_MSS_QDSP6SS_TEST_BUS_WDATA_VALUE_BMSK                                0xffffffff
#define HWIO_MSS_QDSP6SS_TEST_BUS_WDATA_VALUE_SHFT                                         0

#define HWIO_MSS_QDSP6SS_PWRDBG_CTL_ADDR                                          (MSS_QDSP6V671SS_PUB_REG_BASE            + 0x200c)
#define HWIO_MSS_QDSP6SS_PWRDBG_CTL_RMSK                                                 0x1
#define HWIO_MSS_QDSP6SS_PWRDBG_CTL_IN                    \
                in_dword(HWIO_MSS_QDSP6SS_PWRDBG_CTL_ADDR)
#define HWIO_MSS_QDSP6SS_PWRDBG_CTL_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_PWRDBG_CTL_ADDR, m)
#define HWIO_MSS_QDSP6SS_PWRDBG_CTL_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_PWRDBG_CTL_ADDR,v)
#define HWIO_MSS_QDSP6SS_PWRDBG_CTL_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_QDSP6SS_PWRDBG_CTL_ADDR,m,v,HWIO_MSS_QDSP6SS_PWRDBG_CTL_IN)
#define HWIO_MSS_QDSP6SS_PWRDBG_CTL_ENA_BMSK                                             0x1
#define HWIO_MSS_QDSP6SS_PWRDBG_CTL_ENA_SHFT                                               0

#define HWIO_MSS_QDSP6SS_TEST_CLK_CTL_ADDR                                        (MSS_QDSP6V671SS_PUB_REG_BASE            + 0x2010)
#define HWIO_MSS_QDSP6SS_TEST_CLK_CTL_RMSK                                              0x7f
#define HWIO_MSS_QDSP6SS_TEST_CLK_CTL_IN                    \
                in_dword(HWIO_MSS_QDSP6SS_TEST_CLK_CTL_ADDR)
#define HWIO_MSS_QDSP6SS_TEST_CLK_CTL_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_TEST_CLK_CTL_ADDR, m)
#define HWIO_MSS_QDSP6SS_TEST_CLK_CTL_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_TEST_CLK_CTL_ADDR,v)
#define HWIO_MSS_QDSP6SS_TEST_CLK_CTL_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_QDSP6SS_TEST_CLK_CTL_ADDR,m,v,HWIO_MSS_QDSP6SS_TEST_CLK_CTL_IN)
#define HWIO_MSS_QDSP6SS_TEST_CLK_CTL_Q6_DBG_CLK_EN_BMSK                                0x40
#define HWIO_MSS_QDSP6SS_TEST_CLK_CTL_Q6_DBG_CLK_EN_SHFT                                   6
#define HWIO_MSS_QDSP6SS_TEST_CLK_CTL_Q6_DBG_CLK_INV_BMSK                               0x20
#define HWIO_MSS_QDSP6SS_TEST_CLK_CTL_Q6_DBG_CLK_INV_SHFT                                  5
#define HWIO_MSS_QDSP6SS_TEST_CLK_CTL_DBG_CLK_SEL_BMSK                                  0x1f
#define HWIO_MSS_QDSP6SS_TEST_CLK_CTL_DBG_CLK_SEL_SHFT                                     0

#define HWIO_MSS_QDSP6SS_BHS_TEST_ADDR                                            (MSS_QDSP6V671SS_PUB_REG_BASE            + 0x2014)
#define HWIO_MSS_QDSP6SS_BHS_TEST_RMSK                                                   0x1
#define HWIO_MSS_QDSP6SS_BHS_TEST_IN                    \
                in_dword(HWIO_MSS_QDSP6SS_BHS_TEST_ADDR)
#define HWIO_MSS_QDSP6SS_BHS_TEST_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_BHS_TEST_ADDR, m)
#define HWIO_MSS_QDSP6SS_BHS_TEST_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_BHS_TEST_ADDR,v)
#define HWIO_MSS_QDSP6SS_BHS_TEST_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_QDSP6SS_BHS_TEST_ADDR,m,v,HWIO_MSS_QDSP6SS_BHS_TEST_IN)
#define HWIO_MSS_QDSP6SS_BHS_TEST_INSTANT_ON_BMSK                                        0x1
#define HWIO_MSS_QDSP6SS_BHS_TEST_INSTANT_ON_SHFT                                          0

#define HWIO_MSS_QDSP6SS_ZMEAS_CONFIG_ADDR                                        (MSS_QDSP6V671SS_PUB_REG_BASE            + 0x2018)
#define HWIO_MSS_QDSP6SS_ZMEAS_CONFIG_RMSK                                          0xffffff
#define HWIO_MSS_QDSP6SS_ZMEAS_CONFIG_IN                    \
                in_dword(HWIO_MSS_QDSP6SS_ZMEAS_CONFIG_ADDR)
#define HWIO_MSS_QDSP6SS_ZMEAS_CONFIG_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_ZMEAS_CONFIG_ADDR, m)
#define HWIO_MSS_QDSP6SS_ZMEAS_CONFIG_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_ZMEAS_CONFIG_ADDR,v)
#define HWIO_MSS_QDSP6SS_ZMEAS_CONFIG_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_QDSP6SS_ZMEAS_CONFIG_ADDR,m,v,HWIO_MSS_QDSP6SS_ZMEAS_CONFIG_IN)
#define HWIO_MSS_QDSP6SS_ZMEAS_CONFIG_ZMEAS_COUNT_BMSK                              0xffffe0
#define HWIO_MSS_QDSP6SS_ZMEAS_CONFIG_ZMEAS_COUNT_SHFT                                     5
#define HWIO_MSS_QDSP6SS_ZMEAS_CONFIG_MUX_SEL_BMSK                                      0x10
#define HWIO_MSS_QDSP6SS_ZMEAS_CONFIG_MUX_SEL_SHFT                                         4
#define HWIO_MSS_QDSP6SS_ZMEAS_CONFIG_ZMEAS_CLK_R_ENA_BMSK                               0x8
#define HWIO_MSS_QDSP6SS_ZMEAS_CONFIG_ZMEAS_CLK_R_ENA_SHFT                                 3
#define HWIO_MSS_QDSP6SS_ZMEAS_CONFIG_ZMEAS_CGC_EN_BMSK                                  0x4
#define HWIO_MSS_QDSP6SS_ZMEAS_CONFIG_ZMEAS_CGC_EN_SHFT                                    2
#define HWIO_MSS_QDSP6SS_ZMEAS_CONFIG_ZMEAS_BYPASS_BMSK                                  0x2
#define HWIO_MSS_QDSP6SS_ZMEAS_CONFIG_ZMEAS_BYPASS_SHFT                                    1
#define HWIO_MSS_QDSP6SS_ZMEAS_CONFIG_ZMEAS_SW_RESET_BMSK                                0x1
#define HWIO_MSS_QDSP6SS_ZMEAS_CONFIG_ZMEAS_SW_RESET_SHFT                                  0

#define HWIO_MSS_QDSP6SS_CORE_STATUS_ADDR                                         (MSS_QDSP6V671SS_PUB_REG_BASE            + 0x2028)
#define HWIO_MSS_QDSP6SS_CORE_STATUS_RMSK                                               0x3f
#define HWIO_MSS_QDSP6SS_CORE_STATUS_IN                    \
                in_dword(HWIO_MSS_QDSP6SS_CORE_STATUS_ADDR)
#define HWIO_MSS_QDSP6SS_CORE_STATUS_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_CORE_STATUS_ADDR, m)
#define HWIO_MSS_QDSP6SS_CORE_STATUS_VU1_VX_IDLE_BMSK                                   0x20
#define HWIO_MSS_QDSP6SS_CORE_STATUS_VU1_VX_IDLE_SHFT                                      5
#define HWIO_MSS_QDSP6SS_CORE_STATUS_VU0_VX_IDLE_BMSK                                   0x10
#define HWIO_MSS_QDSP6SS_CORE_STATUS_VU0_VX_IDLE_SHFT                                      4
#define HWIO_MSS_QDSP6SS_CORE_STATUS_VU_OVERALL_IDLE_BMSK                                0x8
#define HWIO_MSS_QDSP6SS_CORE_STATUS_VU_OVERALL_IDLE_SHFT                                  3
#define HWIO_MSS_QDSP6SS_CORE_STATUS_CP0_IDLE_BMSK                                       0x4
#define HWIO_MSS_QDSP6SS_CORE_STATUS_CP0_IDLE_SHFT                                         2
#define HWIO_MSS_QDSP6SS_CORE_STATUS_CORE_IN_SLEEP_BMSK                                  0x2
#define HWIO_MSS_QDSP6SS_CORE_STATUS_CORE_IN_SLEEP_SHFT                                    1
#define HWIO_MSS_QDSP6SS_CORE_STATUS_CORE_STATE_BMSK                                     0x1
#define HWIO_MSS_QDSP6SS_CORE_STATUS_CORE_STATE_SHFT                                       0

#define HWIO_MSS_QDSP6SS_MEM_DEBUG_ADDR                                           (MSS_QDSP6V671SS_PUB_REG_BASE            + 0x2030)
#define HWIO_MSS_QDSP6SS_MEM_DEBUG_RMSK                                               0xffff
#define HWIO_MSS_QDSP6SS_MEM_DEBUG_IN                    \
                in_dword(HWIO_MSS_QDSP6SS_MEM_DEBUG_ADDR)
#define HWIO_MSS_QDSP6SS_MEM_DEBUG_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_MEM_DEBUG_ADDR, m)
#define HWIO_MSS_QDSP6SS_MEM_DEBUG_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_MEM_DEBUG_ADDR,v)
#define HWIO_MSS_QDSP6SS_MEM_DEBUG_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_QDSP6SS_MEM_DEBUG_ADDR,m,v,HWIO_MSS_QDSP6SS_MEM_DEBUG_IN)
#define HWIO_MSS_QDSP6SS_MEM_DEBUG_OFF_BMSK                                           0xffff
#define HWIO_MSS_QDSP6SS_MEM_DEBUG_OFF_SHFT                                                0

#define HWIO_MSS_QDSP6SS_L2MEM_DEBUG0_ADDR                                        (MSS_QDSP6V671SS_PUB_REG_BASE            + 0x2034)
#define HWIO_MSS_QDSP6SS_L2MEM_DEBUG0_RMSK                                        0xffffffff
#define HWIO_MSS_QDSP6SS_L2MEM_DEBUG0_IN                    \
                in_dword(HWIO_MSS_QDSP6SS_L2MEM_DEBUG0_ADDR)
#define HWIO_MSS_QDSP6SS_L2MEM_DEBUG0_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_L2MEM_DEBUG0_ADDR, m)
#define HWIO_MSS_QDSP6SS_L2MEM_DEBUG0_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_L2MEM_DEBUG0_ADDR,v)
#define HWIO_MSS_QDSP6SS_L2MEM_DEBUG0_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_QDSP6SS_L2MEM_DEBUG0_ADDR,m,v,HWIO_MSS_QDSP6SS_L2MEM_DEBUG0_IN)
#define HWIO_MSS_QDSP6SS_L2MEM_DEBUG0_OFF_BMSK                                    0xffffffff
#define HWIO_MSS_QDSP6SS_L2MEM_DEBUG0_OFF_SHFT                                             0

#define HWIO_MSS_QDSP6SS_L2MEM_DEBUG1_ADDR                                        (MSS_QDSP6V671SS_PUB_REG_BASE            + 0x2038)
#define HWIO_MSS_QDSP6SS_L2MEM_DEBUG1_RMSK                                        0xffffffff
#define HWIO_MSS_QDSP6SS_L2MEM_DEBUG1_IN                    \
                in_dword(HWIO_MSS_QDSP6SS_L2MEM_DEBUG1_ADDR)
#define HWIO_MSS_QDSP6SS_L2MEM_DEBUG1_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_L2MEM_DEBUG1_ADDR, m)
#define HWIO_MSS_QDSP6SS_L2MEM_DEBUG1_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_L2MEM_DEBUG1_ADDR,v)
#define HWIO_MSS_QDSP6SS_L2MEM_DEBUG1_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_QDSP6SS_L2MEM_DEBUG1_ADDR,m,v,HWIO_MSS_QDSP6SS_L2MEM_DEBUG1_IN)
#define HWIO_MSS_QDSP6SS_L2MEM_DEBUG1_OFF_BMSK                                    0xffffffff
#define HWIO_MSS_QDSP6SS_L2MEM_DEBUG1_OFF_SHFT                                             0

#define HWIO_MSS_QDSP6SS_CPMEM_DEBUG_ADDR                                         (MSS_QDSP6V671SS_PUB_REG_BASE            + 0x203c)
#define HWIO_MSS_QDSP6SS_CPMEM_DEBUG_RMSK                                         0xffffffff
#define HWIO_MSS_QDSP6SS_CPMEM_DEBUG_IN                    \
                in_dword(HWIO_MSS_QDSP6SS_CPMEM_DEBUG_ADDR)
#define HWIO_MSS_QDSP6SS_CPMEM_DEBUG_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_CPMEM_DEBUG_ADDR, m)
#define HWIO_MSS_QDSP6SS_CPMEM_DEBUG_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_CPMEM_DEBUG_ADDR,v)
#define HWIO_MSS_QDSP6SS_CPMEM_DEBUG_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_QDSP6SS_CPMEM_DEBUG_ADDR,m,v,HWIO_MSS_QDSP6SS_CPMEM_DEBUG_IN)
#define HWIO_MSS_QDSP6SS_CPMEM_DEBUG_OFF_BMSK                                     0xffffffff
#define HWIO_MSS_QDSP6SS_CPMEM_DEBUG_OFF_SHFT                                              0

#define HWIO_MSS_QDSP6SS_L1SMEM_DEBUG0_ADDR                                       (MSS_QDSP6V671SS_PUB_REG_BASE            + 0x2040)
#define HWIO_MSS_QDSP6SS_L1SMEM_DEBUG0_RMSK                                       0xffffffff
#define HWIO_MSS_QDSP6SS_L1SMEM_DEBUG0_IN                    \
                in_dword(HWIO_MSS_QDSP6SS_L1SMEM_DEBUG0_ADDR)
#define HWIO_MSS_QDSP6SS_L1SMEM_DEBUG0_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_L1SMEM_DEBUG0_ADDR, m)
#define HWIO_MSS_QDSP6SS_L1SMEM_DEBUG0_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_L1SMEM_DEBUG0_ADDR,v)
#define HWIO_MSS_QDSP6SS_L1SMEM_DEBUG0_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_QDSP6SS_L1SMEM_DEBUG0_ADDR,m,v,HWIO_MSS_QDSP6SS_L1SMEM_DEBUG0_IN)
#define HWIO_MSS_QDSP6SS_L1SMEM_DEBUG0_OFF_BMSK                                   0xffffffff
#define HWIO_MSS_QDSP6SS_L1SMEM_DEBUG0_OFF_SHFT                                            0

#define HWIO_MSS_QDSP6SS_L1SMEM_DEBUG1_ADDR                                       (MSS_QDSP6V671SS_PUB_REG_BASE            + 0x2044)
#define HWIO_MSS_QDSP6SS_L1SMEM_DEBUG1_RMSK                                       0xffffffff
#define HWIO_MSS_QDSP6SS_L1SMEM_DEBUG1_IN                    \
                in_dword(HWIO_MSS_QDSP6SS_L1SMEM_DEBUG1_ADDR)
#define HWIO_MSS_QDSP6SS_L1SMEM_DEBUG1_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_L1SMEM_DEBUG1_ADDR, m)
#define HWIO_MSS_QDSP6SS_L1SMEM_DEBUG1_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_L1SMEM_DEBUG1_ADDR,v)
#define HWIO_MSS_QDSP6SS_L1SMEM_DEBUG1_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_QDSP6SS_L1SMEM_DEBUG1_ADDR,m,v,HWIO_MSS_QDSP6SS_L1SMEM_DEBUG1_IN)
#define HWIO_MSS_QDSP6SS_L1SMEM_DEBUG1_OFF_BMSK                                   0xffffffff
#define HWIO_MSS_QDSP6SS_L1SMEM_DEBUG1_OFF_SHFT                                            0

#define HWIO_MSS_QDSP6SS_L1SMEM_DEBUG2_ADDR                                       (MSS_QDSP6V671SS_PUB_REG_BASE            + 0x2048)
#define HWIO_MSS_QDSP6SS_L1SMEM_DEBUG2_RMSK                                       0xffffffff
#define HWIO_MSS_QDSP6SS_L1SMEM_DEBUG2_IN                    \
                in_dword(HWIO_MSS_QDSP6SS_L1SMEM_DEBUG2_ADDR)
#define HWIO_MSS_QDSP6SS_L1SMEM_DEBUG2_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_L1SMEM_DEBUG2_ADDR, m)
#define HWIO_MSS_QDSP6SS_L1SMEM_DEBUG2_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_L1SMEM_DEBUG2_ADDR,v)
#define HWIO_MSS_QDSP6SS_L1SMEM_DEBUG2_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_QDSP6SS_L1SMEM_DEBUG2_ADDR,m,v,HWIO_MSS_QDSP6SS_L1SMEM_DEBUG2_IN)
#define HWIO_MSS_QDSP6SS_L1SMEM_DEBUG2_OFF_BMSK                                   0xffffffff
#define HWIO_MSS_QDSP6SS_L1SMEM_DEBUG2_OFF_SHFT                                            0

#define HWIO_MSS_QDSP6SS_L1SMEM_DEBUG3_ADDR                                       (MSS_QDSP6V671SS_PUB_REG_BASE            + 0x204c)
#define HWIO_MSS_QDSP6SS_L1SMEM_DEBUG3_RMSK                                       0xffffffff
#define HWIO_MSS_QDSP6SS_L1SMEM_DEBUG3_IN                    \
                in_dword(HWIO_MSS_QDSP6SS_L1SMEM_DEBUG3_ADDR)
#define HWIO_MSS_QDSP6SS_L1SMEM_DEBUG3_INM(m)            \
                in_dword_masked(HWIO_MSS_QDSP6SS_L1SMEM_DEBUG3_ADDR, m)
#define HWIO_MSS_QDSP6SS_L1SMEM_DEBUG3_OUT(v)            \
                out_dword(HWIO_MSS_QDSP6SS_L1SMEM_DEBUG3_ADDR,v)
#define HWIO_MSS_QDSP6SS_L1SMEM_DEBUG3_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_QDSP6SS_L1SMEM_DEBUG3_ADDR,m,v,HWIO_MSS_QDSP6SS_L1SMEM_DEBUG3_IN)
#define HWIO_MSS_QDSP6SS_L1SMEM_DEBUG3_OFF_BMSK                                   0xffffffff
#define HWIO_MSS_QDSP6SS_L1SMEM_DEBUG3_OFF_SHFT                                            0

/*----------------------------------------------------------------------------
 * MODULE: MSS_PERPH_MSS_PERPH
 *--------------------------------------------------------------------------*/

#define MSS_PERPH_MSS_PERPH_REG_BASE                                                  (MSS_TOP_BASE            + 0x00181770)
#define MSS_PERPH_MSS_PERPH_REG_BASE_SIZE                                             0xd8b0
#define MSS_PERPH_MSS_PERPH_REG_BASE_USED                                             0xd890

#define HWIO_MSS_PERPH_MSS_ENABLE_ADDR                                                (MSS_PERPH_MSS_PERPH_REG_BASE            + 0x0)
#define HWIO_MSS_PERPH_MSS_ENABLE_RMSK                                                0x80000000
#define HWIO_MSS_PERPH_MSS_ENABLE_IN                    \
                in_dword(HWIO_MSS_PERPH_MSS_ENABLE_ADDR)
#define HWIO_MSS_PERPH_MSS_ENABLE_INM(m)            \
                in_dword_masked(HWIO_MSS_PERPH_MSS_ENABLE_ADDR, m)
#define HWIO_MSS_PERPH_MSS_ENABLE_MODEM_ARES_IN_BMSK                                  0x80000000
#define HWIO_MSS_PERPH_MSS_ENABLE_MODEM_ARES_IN_SHFT                                          31

#define HWIO_MSS_PERPH_MSS_CLAMP_MEM_ADDR                                             (MSS_PERPH_MSS_PERPH_REG_BASE            + 0x4)
#define HWIO_MSS_PERPH_MSS_CLAMP_MEM_RMSK                                                    0x7
#define HWIO_MSS_PERPH_MSS_CLAMP_MEM_IN                    \
                in_dword(HWIO_MSS_PERPH_MSS_CLAMP_MEM_ADDR)
#define HWIO_MSS_PERPH_MSS_CLAMP_MEM_INM(m)            \
                in_dword_masked(HWIO_MSS_PERPH_MSS_CLAMP_MEM_ADDR, m)
#define HWIO_MSS_PERPH_MSS_CLAMP_MEM_OUT(v)            \
                out_dword(HWIO_MSS_PERPH_MSS_CLAMP_MEM_ADDR,v)
#define HWIO_MSS_PERPH_MSS_CLAMP_MEM_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_PERPH_MSS_CLAMP_MEM_ADDR,m,v,HWIO_MSS_PERPH_MSS_CLAMP_MEM_IN)
#define HWIO_MSS_PERPH_MSS_CLAMP_MEM_SPARE_BMSK                                              0x4
#define HWIO_MSS_PERPH_MSS_CLAMP_MEM_SPARE_SHFT                                                2
#define HWIO_MSS_PERPH_MSS_CLAMP_MEM_UNCLAMP_ALL_BMSK                                        0x2
#define HWIO_MSS_PERPH_MSS_CLAMP_MEM_UNCLAMP_ALL_SHFT                                          1
#define HWIO_MSS_PERPH_MSS_CLAMP_MEM_HM_CLAMP_BMSK                                           0x1
#define HWIO_MSS_PERPH_MSS_CLAMP_MEM_HM_CLAMP_SHFT                                             0

#define HWIO_MSS_PERPH_MSS_CLAMP_IO_ADDR                                              (MSS_PERPH_MSS_PERPH_REG_BASE            + 0x8)
#define HWIO_MSS_PERPH_MSS_CLAMP_IO_RMSK                                                    0xd0
#define HWIO_MSS_PERPH_MSS_CLAMP_IO_IN                    \
                in_dword(HWIO_MSS_PERPH_MSS_CLAMP_IO_ADDR)
#define HWIO_MSS_PERPH_MSS_CLAMP_IO_INM(m)            \
                in_dword_masked(HWIO_MSS_PERPH_MSS_CLAMP_IO_ADDR, m)
#define HWIO_MSS_PERPH_MSS_CLAMP_IO_OUT(v)            \
                out_dword(HWIO_MSS_PERPH_MSS_CLAMP_IO_ADDR,v)
#define HWIO_MSS_PERPH_MSS_CLAMP_IO_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_PERPH_MSS_CLAMP_IO_ADDR,m,v,HWIO_MSS_PERPH_MSS_CLAMP_IO_IN)
#define HWIO_MSS_PERPH_MSS_CLAMP_IO_SPARE_7_BMSK                                            0x80
#define HWIO_MSS_PERPH_MSS_CLAMP_IO_SPARE_7_SHFT                                               7
#define HWIO_MSS_PERPH_MSS_CLAMP_IO_UNCLAMP_ALL_BMSK                                        0x40
#define HWIO_MSS_PERPH_MSS_CLAMP_IO_UNCLAMP_ALL_SHFT                                           6
#define HWIO_MSS_PERPH_MSS_CLAMP_IO_BBRX_ADC_BMSK                                           0x10
#define HWIO_MSS_PERPH_MSS_CLAMP_IO_BBRX_ADC_SHFT                                              4

#define HWIO_MSS_PERPH_MSS_BUS_AHB2AHB_CFG_ADDR                                       (MSS_PERPH_MSS_PERPH_REG_BASE            + 0xc)
#define HWIO_MSS_PERPH_MSS_BUS_AHB2AHB_CFG_RMSK                                              0x1
#define HWIO_MSS_PERPH_MSS_BUS_AHB2AHB_CFG_IN                    \
                in_dword(HWIO_MSS_PERPH_MSS_BUS_AHB2AHB_CFG_ADDR)
#define HWIO_MSS_PERPH_MSS_BUS_AHB2AHB_CFG_INM(m)            \
                in_dword_masked(HWIO_MSS_PERPH_MSS_BUS_AHB2AHB_CFG_ADDR, m)
#define HWIO_MSS_PERPH_MSS_BUS_AHB2AHB_CFG_OUT(v)            \
                out_dword(HWIO_MSS_PERPH_MSS_BUS_AHB2AHB_CFG_ADDR,v)
#define HWIO_MSS_PERPH_MSS_BUS_AHB2AHB_CFG_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_PERPH_MSS_BUS_AHB2AHB_CFG_ADDR,m,v,HWIO_MSS_PERPH_MSS_BUS_AHB2AHB_CFG_IN)
#define HWIO_MSS_PERPH_MSS_BUS_AHB2AHB_CFG_POST_EN_AHB2AHB_BMSK                              0x1
#define HWIO_MSS_PERPH_MSS_BUS_AHB2AHB_CFG_POST_EN_AHB2AHB_SHFT                                0

#define HWIO_MSS_PERPH_MSS_CUSTOM_MEM_ARRSTBYN_ADDR                                   (MSS_PERPH_MSS_PERPH_REG_BASE            + 0x14)
#define HWIO_MSS_PERPH_MSS_CUSTOM_MEM_ARRSTBYN_RMSK                                         0xff
#define HWIO_MSS_PERPH_MSS_CUSTOM_MEM_ARRSTBYN_IN                    \
                in_dword(HWIO_MSS_PERPH_MSS_CUSTOM_MEM_ARRSTBYN_ADDR)
#define HWIO_MSS_PERPH_MSS_CUSTOM_MEM_ARRSTBYN_INM(m)            \
                in_dword_masked(HWIO_MSS_PERPH_MSS_CUSTOM_MEM_ARRSTBYN_ADDR, m)
#define HWIO_MSS_PERPH_MSS_CUSTOM_MEM_ARRSTBYN_OUT(v)            \
                out_dword(HWIO_MSS_PERPH_MSS_CUSTOM_MEM_ARRSTBYN_ADDR,v)
#define HWIO_MSS_PERPH_MSS_CUSTOM_MEM_ARRSTBYN_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_PERPH_MSS_CUSTOM_MEM_ARRSTBYN_ADDR,m,v,HWIO_MSS_PERPH_MSS_CUSTOM_MEM_ARRSTBYN_IN)
#define HWIO_MSS_PERPH_MSS_CUSTOM_MEM_ARRSTBYN_CTL_BMSK                                     0xff
#define HWIO_MSS_PERPH_MSS_CUSTOM_MEM_ARRSTBYN_CTL_SHFT                                        0

#define HWIO_MSS_PERPH_MSS_ANALOG_IP_TEST_CTL_ADDR                                    (MSS_PERPH_MSS_PERPH_REG_BASE            + 0x18)
#define HWIO_MSS_PERPH_MSS_ANALOG_IP_TEST_CTL_RMSK                                           0x3
#define HWIO_MSS_PERPH_MSS_ANALOG_IP_TEST_CTL_IN                    \
                in_dword(HWIO_MSS_PERPH_MSS_ANALOG_IP_TEST_CTL_ADDR)
#define HWIO_MSS_PERPH_MSS_ANALOG_IP_TEST_CTL_INM(m)            \
                in_dword_masked(HWIO_MSS_PERPH_MSS_ANALOG_IP_TEST_CTL_ADDR, m)
#define HWIO_MSS_PERPH_MSS_ANALOG_IP_TEST_CTL_OUT(v)            \
                out_dword(HWIO_MSS_PERPH_MSS_ANALOG_IP_TEST_CTL_ADDR,v)
#define HWIO_MSS_PERPH_MSS_ANALOG_IP_TEST_CTL_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_PERPH_MSS_ANALOG_IP_TEST_CTL_ADDR,m,v,HWIO_MSS_PERPH_MSS_ANALOG_IP_TEST_CTL_IN)
#define HWIO_MSS_PERPH_MSS_ANALOG_IP_TEST_CTL_EXTERNAL_IQDATA_EN_BMSK                        0x2
#define HWIO_MSS_PERPH_MSS_ANALOG_IP_TEST_CTL_EXTERNAL_IQDATA_EN_SHFT                          1
#define HWIO_MSS_PERPH_MSS_ANALOG_IP_TEST_CTL_EXTERNAL_Y1Y2_EN_BMSK                          0x1
#define HWIO_MSS_PERPH_MSS_ANALOG_IP_TEST_CTL_EXTERNAL_Y1Y2_EN_SHFT                            0

#define HWIO_MSS_PERPH_MSS_ATB_ID_ADDR                                                (MSS_PERPH_MSS_PERPH_REG_BASE            + 0x1c)
#define HWIO_MSS_PERPH_MSS_ATB_ID_RMSK                                                    0x3fff
#define HWIO_MSS_PERPH_MSS_ATB_ID_IN                    \
                in_dword(HWIO_MSS_PERPH_MSS_ATB_ID_ADDR)
#define HWIO_MSS_PERPH_MSS_ATB_ID_INM(m)            \
                in_dword_masked(HWIO_MSS_PERPH_MSS_ATB_ID_ADDR, m)
#define HWIO_MSS_PERPH_MSS_ATB_ID_OUT(v)            \
                out_dword(HWIO_MSS_PERPH_MSS_ATB_ID_ADDR,v)
#define HWIO_MSS_PERPH_MSS_ATB_ID_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_PERPH_MSS_ATB_ID_ADDR,m,v,HWIO_MSS_PERPH_MSS_ATB_ID_IN)
#define HWIO_MSS_PERPH_MSS_ATB_ID_MODEM_RXFE_ATB_ID_BMSK                                  0x3f80
#define HWIO_MSS_PERPH_MSS_ATB_ID_MODEM_RXFE_ATB_ID_SHFT                                       7
#define HWIO_MSS_PERPH_MSS_ATB_ID_ATB_ID_BMSK                                               0x7f
#define HWIO_MSS_PERPH_MSS_ATB_ID_ATB_ID_SHFT                                                  0

#define HWIO_MSS_PERPH_MSS_DBG_BUS_CTL_ADDR                                           (MSS_PERPH_MSS_PERPH_REG_BASE            + 0x20)
#define HWIO_MSS_PERPH_MSS_DBG_BUS_CTL_RMSK                                                0x7e3
#define HWIO_MSS_PERPH_MSS_DBG_BUS_CTL_IN                    \
                in_dword(HWIO_MSS_PERPH_MSS_DBG_BUS_CTL_ADDR)
#define HWIO_MSS_PERPH_MSS_DBG_BUS_CTL_INM(m)            \
                in_dword_masked(HWIO_MSS_PERPH_MSS_DBG_BUS_CTL_ADDR, m)
#define HWIO_MSS_PERPH_MSS_DBG_BUS_CTL_OUT(v)            \
                out_dword(HWIO_MSS_PERPH_MSS_DBG_BUS_CTL_ADDR,v)
#define HWIO_MSS_PERPH_MSS_DBG_BUS_CTL_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_PERPH_MSS_DBG_BUS_CTL_ADDR,m,v,HWIO_MSS_PERPH_MSS_DBG_BUS_CTL_IN)
#define HWIO_MSS_PERPH_MSS_DBG_BUS_CTL_AHB2AHB_SEL_BMSK                                    0x600
#define HWIO_MSS_PERPH_MSS_DBG_BUS_CTL_AHB2AHB_SEL_SHFT                                        9
#define HWIO_MSS_PERPH_MSS_DBG_BUS_CTL_MSS_DBG_BUS_TOP_SEL_BMSK                            0x1e0
#define HWIO_MSS_PERPH_MSS_DBG_BUS_CTL_MSS_DBG_BUS_TOP_SEL_SHFT                                5
#define HWIO_MSS_PERPH_MSS_DBG_BUS_CTL_MSS_DBG_GPIO_ATB_SEL_BMSK                             0x3
#define HWIO_MSS_PERPH_MSS_DBG_BUS_CTL_MSS_DBG_GPIO_ATB_SEL_SHFT                               0

#define HWIO_MSS_PERPH_MSS_AHB_ACCESS_ERR_IRQ_EN_ADDR                                 (MSS_PERPH_MSS_PERPH_REG_BASE            + 0x24)
#define HWIO_MSS_PERPH_MSS_AHB_ACCESS_ERR_IRQ_EN_RMSK                                        0x1
#define HWIO_MSS_PERPH_MSS_AHB_ACCESS_ERR_IRQ_EN_IN                    \
                in_dword(HWIO_MSS_PERPH_MSS_AHB_ACCESS_ERR_IRQ_EN_ADDR)
#define HWIO_MSS_PERPH_MSS_AHB_ACCESS_ERR_IRQ_EN_INM(m)            \
                in_dword_masked(HWIO_MSS_PERPH_MSS_AHB_ACCESS_ERR_IRQ_EN_ADDR, m)
#define HWIO_MSS_PERPH_MSS_AHB_ACCESS_ERR_IRQ_EN_OUT(v)            \
                out_dword(HWIO_MSS_PERPH_MSS_AHB_ACCESS_ERR_IRQ_EN_ADDR,v)
#define HWIO_MSS_PERPH_MSS_AHB_ACCESS_ERR_IRQ_EN_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_PERPH_MSS_AHB_ACCESS_ERR_IRQ_EN_ADDR,m,v,HWIO_MSS_PERPH_MSS_AHB_ACCESS_ERR_IRQ_EN_IN)
#define HWIO_MSS_PERPH_MSS_AHB_ACCESS_ERR_IRQ_EN_EN_BMSK                                     0x1
#define HWIO_MSS_PERPH_MSS_AHB_ACCESS_ERR_IRQ_EN_EN_SHFT                                       0

#define HWIO_MSS_PERPH_MSS_AHB_ACCESS_ERR_IRQ_STATUS_ADDR                             (MSS_PERPH_MSS_PERPH_REG_BASE            + 0x28)
#define HWIO_MSS_PERPH_MSS_AHB_ACCESS_ERR_IRQ_STATUS_RMSK                                    0x1
#define HWIO_MSS_PERPH_MSS_AHB_ACCESS_ERR_IRQ_STATUS_IN                    \
                in_dword(HWIO_MSS_PERPH_MSS_AHB_ACCESS_ERR_IRQ_STATUS_ADDR)
#define HWIO_MSS_PERPH_MSS_AHB_ACCESS_ERR_IRQ_STATUS_INM(m)            \
                in_dword_masked(HWIO_MSS_PERPH_MSS_AHB_ACCESS_ERR_IRQ_STATUS_ADDR, m)
#define HWIO_MSS_PERPH_MSS_AHB_ACCESS_ERR_IRQ_STATUS_STATUS_BMSK                             0x1
#define HWIO_MSS_PERPH_MSS_AHB_ACCESS_ERR_IRQ_STATUS_STATUS_SHFT                               0

#define HWIO_MSS_PERPH_MSS_AHB_ACCESS_ERR_IRQ_CLR_ADDR                                (MSS_PERPH_MSS_PERPH_REG_BASE            + 0x2c)
#define HWIO_MSS_PERPH_MSS_AHB_ACCESS_ERR_IRQ_CLR_RMSK                                       0x1
#define HWIO_MSS_PERPH_MSS_AHB_ACCESS_ERR_IRQ_CLR_OUT(v)            \
                out_dword(HWIO_MSS_PERPH_MSS_AHB_ACCESS_ERR_IRQ_CLR_ADDR,v)
#define HWIO_MSS_PERPH_MSS_AHB_ACCESS_ERR_IRQ_CLR_CMD_BMSK                                   0x1
#define HWIO_MSS_PERPH_MSS_AHB_ACCESS_ERR_IRQ_CLR_CMD_SHFT                                     0

#define HWIO_MSS_PERPH_MSS_BUS_CTL_CFG_ADDR                                           (MSS_PERPH_MSS_PERPH_REG_BASE            + 0x30)
#define HWIO_MSS_PERPH_MSS_BUS_CTL_CFG_RMSK                                                  0x1
#define HWIO_MSS_PERPH_MSS_BUS_CTL_CFG_IN                    \
                in_dword(HWIO_MSS_PERPH_MSS_BUS_CTL_CFG_ADDR)
#define HWIO_MSS_PERPH_MSS_BUS_CTL_CFG_INM(m)            \
                in_dword_masked(HWIO_MSS_PERPH_MSS_BUS_CTL_CFG_ADDR, m)
#define HWIO_MSS_PERPH_MSS_BUS_CTL_CFG_OUT(v)            \
                out_dword(HWIO_MSS_PERPH_MSS_BUS_CTL_CFG_ADDR,v)
#define HWIO_MSS_PERPH_MSS_BUS_CTL_CFG_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_PERPH_MSS_BUS_CTL_CFG_ADDR,m,v,HWIO_MSS_PERPH_MSS_BUS_CTL_CFG_IN)
#define HWIO_MSS_PERPH_MSS_BUS_CTL_CFG_Q6_FORCE_UNBUFFERED_BMSK                              0x1
#define HWIO_MSS_PERPH_MSS_BUS_CTL_CFG_Q6_FORCE_UNBUFFERED_SHFT                                0

#define HWIO_MSS_PERPH_MSS_MSA_ADDR                                                   (MSS_PERPH_MSS_PERPH_REG_BASE            + 0x34)
#define HWIO_MSS_PERPH_MSS_MSA_RMSK                                                          0x7
#define HWIO_MSS_PERPH_MSS_MSA_IN                    \
                in_dword(HWIO_MSS_PERPH_MSS_MSA_ADDR)
#define HWIO_MSS_PERPH_MSS_MSA_INM(m)            \
                in_dword_masked(HWIO_MSS_PERPH_MSS_MSA_ADDR, m)
#define HWIO_MSS_PERPH_MSS_MSA_OUT(v)            \
                out_dword(HWIO_MSS_PERPH_MSS_MSA_ADDR,v)
#define HWIO_MSS_PERPH_MSS_MSA_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_PERPH_MSS_MSA_ADDR,m,v,HWIO_MSS_PERPH_MSS_MSA_IN)
#define HWIO_MSS_PERPH_MSS_MSA_FORCE_Q6_MSA_BMSK                                             0x4
#define HWIO_MSS_PERPH_MSS_MSA_FORCE_Q6_MSA_SHFT                                               2
#define HWIO_MSS_PERPH_MSS_MSA_MBA_OK_BMSK                                                   0x2
#define HWIO_MSS_PERPH_MSS_MSA_MBA_OK_SHFT                                                     1
#define HWIO_MSS_PERPH_MSS_MSA_CONFIG_LOCK_BMSK                                              0x1
#define HWIO_MSS_PERPH_MSS_MSA_CONFIG_LOCK_SHFT                                                0

#define HWIO_MSS_PERPH_MSS_HW_VERSION_ADDR                                            (MSS_PERPH_MSS_PERPH_REG_BASE            + 0x38)
#define HWIO_MSS_PERPH_MSS_HW_VERSION_RMSK                                            0xffffffff
#define HWIO_MSS_PERPH_MSS_HW_VERSION_IN                    \
                in_dword(HWIO_MSS_PERPH_MSS_HW_VERSION_ADDR)
#define HWIO_MSS_PERPH_MSS_HW_VERSION_INM(m)            \
                in_dword_masked(HWIO_MSS_PERPH_MSS_HW_VERSION_ADDR, m)
#define HWIO_MSS_PERPH_MSS_HW_VERSION_MAJOR_BMSK                                      0xf0000000
#define HWIO_MSS_PERPH_MSS_HW_VERSION_MAJOR_SHFT                                              28
#define HWIO_MSS_PERPH_MSS_HW_VERSION_MINOR_BMSK                                       0xfff0000
#define HWIO_MSS_PERPH_MSS_HW_VERSION_MINOR_SHFT                                              16
#define HWIO_MSS_PERPH_MSS_HW_VERSION_STEP_BMSK                                           0xffff
#define HWIO_MSS_PERPH_MSS_HW_VERSION_STEP_SHFT                                                0

#define HWIO_MSS_PERPH_MSS_BBRX_CTL_ADDR                                              (MSS_PERPH_MSS_PERPH_REG_BASE            + 0x40)
#define HWIO_MSS_PERPH_MSS_BBRX_CTL_RMSK                                                     0x3
#define HWIO_MSS_PERPH_MSS_BBRX_CTL_IN                    \
                in_dword(HWIO_MSS_PERPH_MSS_BBRX_CTL_ADDR)
#define HWIO_MSS_PERPH_MSS_BBRX_CTL_INM(m)            \
                in_dword_masked(HWIO_MSS_PERPH_MSS_BBRX_CTL_ADDR, m)
#define HWIO_MSS_PERPH_MSS_BBRX_CTL_OUT(v)            \
                out_dword(HWIO_MSS_PERPH_MSS_BBRX_CTL_ADDR,v)
#define HWIO_MSS_PERPH_MSS_BBRX_CTL_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_PERPH_MSS_BBRX_CTL_ADDR,m,v,HWIO_MSS_PERPH_MSS_BBRX_CTL_IN)
#define HWIO_MSS_PERPH_MSS_BBRX_CTL_BBRX_HS_TEST_MUX_CTL_BMSK                                0x3
#define HWIO_MSS_PERPH_MSS_BBRX_CTL_BBRX_HS_TEST_MUX_CTL_SHFT                                  0

#define HWIO_MSS_PERPH_MSS_DEBUG_CTL_ADDR                                             (MSS_PERPH_MSS_PERPH_REG_BASE            + 0x44)
#define HWIO_MSS_PERPH_MSS_DEBUG_CTL_RMSK                                                    0x7
#define HWIO_MSS_PERPH_MSS_DEBUG_CTL_IN                    \
                in_dword(HWIO_MSS_PERPH_MSS_DEBUG_CTL_ADDR)
#define HWIO_MSS_PERPH_MSS_DEBUG_CTL_INM(m)            \
                in_dword_masked(HWIO_MSS_PERPH_MSS_DEBUG_CTL_ADDR, m)
#define HWIO_MSS_PERPH_MSS_DEBUG_CTL_OUT(v)            \
                out_dword(HWIO_MSS_PERPH_MSS_DEBUG_CTL_ADDR,v)
#define HWIO_MSS_PERPH_MSS_DEBUG_CTL_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_PERPH_MSS_DEBUG_CTL_ADDR,m,v,HWIO_MSS_PERPH_MSS_DEBUG_CTL_IN)
#define HWIO_MSS_PERPH_MSS_DEBUG_CTL_DAC_DISABLE_ON_Q6_DBG_BMSK                              0x4
#define HWIO_MSS_PERPH_MSS_DEBUG_CTL_DAC_DISABLE_ON_Q6_DBG_SHFT                                2
#define HWIO_MSS_PERPH_MSS_DEBUG_CTL_GRFC_DISABLE_Q6_DBG_BMSK                                0x2
#define HWIO_MSS_PERPH_MSS_DEBUG_CTL_GRFC_DISABLE_Q6_DBG_SHFT                                  1
#define HWIO_MSS_PERPH_MSS_DEBUG_CTL_GRFC_DISABLE_Q6_WDOG_BMSK                               0x1
#define HWIO_MSS_PERPH_MSS_DEBUG_CTL_GRFC_DISABLE_Q6_WDOG_SHFT                                 0

#define HWIO_MSS_PERPH_MSS_MSA_NC_HM_ADDR                                             (MSS_PERPH_MSS_PERPH_REG_BASE            + 0x4c)
#define HWIO_MSS_PERPH_MSS_MSA_NC_HM_RMSK                                             0xfffffeff
#define HWIO_MSS_PERPH_MSS_MSA_NC_HM_IN                    \
                in_dword(HWIO_MSS_PERPH_MSS_MSA_NC_HM_ADDR)
#define HWIO_MSS_PERPH_MSS_MSA_NC_HM_INM(m)            \
                in_dword_masked(HWIO_MSS_PERPH_MSS_MSA_NC_HM_ADDR, m)
#define HWIO_MSS_PERPH_MSS_MSA_NC_HM_OUT(v)            \
                out_dword(HWIO_MSS_PERPH_MSS_MSA_NC_HM_ADDR,v)
#define HWIO_MSS_PERPH_MSS_MSA_NC_HM_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_PERPH_MSS_MSA_NC_HM_ADDR,m,v,HWIO_MSS_PERPH_MSS_MSA_NC_HM_IN)
#define HWIO_MSS_PERPH_MSS_MSA_NC_HM_RESERVE_31_9_BMSK                                0xfffffe00
#define HWIO_MSS_PERPH_MSS_MSA_NC_HM_RESERVE_31_9_SHFT                                         9
#define HWIO_MSS_PERPH_MSS_MSA_NC_HM_RESERVE_7_4_BMSK                                       0xf0
#define HWIO_MSS_PERPH_MSS_MSA_NC_HM_RESERVE_7_4_SHFT                                          4
#define HWIO_MSS_PERPH_MSS_MSA_NC_HM_CRYPTO_BMSK                                             0xf
#define HWIO_MSS_PERPH_MSS_MSA_NC_HM_CRYPTO_SHFT                                               0

#define HWIO_MSS_PERPH_MSS_CXM_ADDR                                                   (MSS_PERPH_MSS_PERPH_REG_BASE            + 0x50)
#define HWIO_MSS_PERPH_MSS_CXM_RMSK                                                   0xffffffff
#define HWIO_MSS_PERPH_MSS_CXM_IN                    \
                in_dword(HWIO_MSS_PERPH_MSS_CXM_ADDR)
#define HWIO_MSS_PERPH_MSS_CXM_INM(m)            \
                in_dword_masked(HWIO_MSS_PERPH_MSS_CXM_ADDR, m)
#define HWIO_MSS_PERPH_MSS_CXM_OUT(v)            \
                out_dword(HWIO_MSS_PERPH_MSS_CXM_ADDR,v)
#define HWIO_MSS_PERPH_MSS_CXM_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_PERPH_MSS_CXM_ADDR,m,v,HWIO_MSS_PERPH_MSS_CXM_IN)
#define HWIO_MSS_PERPH_MSS_CXM_RESERVE_31_12_BMSK                                     0xfffff000
#define HWIO_MSS_PERPH_MSS_CXM_RESERVE_31_12_SHFT                                             12
#define HWIO_MSS_PERPH_MSS_CXM_USE_EXTERNAL_WLAN_BMSK                                      0x800
#define HWIO_MSS_PERPH_MSS_CXM_USE_EXTERNAL_WLAN_SHFT                                         11
#define HWIO_MSS_PERPH_MSS_CXM_BT_TX_ACT_LST_SUBFRM_BMSK                                   0x400
#define HWIO_MSS_PERPH_MSS_CXM_BT_TX_ACT_LST_SUBFRM_SHFT                                      10
#define HWIO_MSS_PERPH_MSS_CXM_WLAN_TX_ACT_LST_SUBFRM_BMSK                                 0x200
#define HWIO_MSS_PERPH_MSS_CXM_WLAN_TX_ACT_LST_SUBFRM_SHFT                                     9
#define HWIO_MSS_PERPH_MSS_CXM_BT_TX_S_WAN_BMSK                                            0x100
#define HWIO_MSS_PERPH_MSS_CXM_BT_TX_S_WAN_SHFT                                                8
#define HWIO_MSS_PERPH_MSS_CXM_WLAN_TX_S_WAN_BMSK                                           0x80
#define HWIO_MSS_PERPH_MSS_CXM_WLAN_TX_S_WAN_SHFT                                              7
#define HWIO_MSS_PERPH_MSS_CXM_BT_TX_REGISTERED_BMSK                                        0x40
#define HWIO_MSS_PERPH_MSS_CXM_BT_TX_REGISTERED_SHFT                                           6
#define HWIO_MSS_PERPH_MSS_CXM_WLAN_TX_REGISTERED_BMSK                                      0x20
#define HWIO_MSS_PERPH_MSS_CXM_WLAN_TX_REGISTERED_SHFT                                         5
#define HWIO_MSS_PERPH_MSS_CXM_WLAN_TX_STICKY_BMSK                                          0x10
#define HWIO_MSS_PERPH_MSS_CXM_WLAN_TX_STICKY_SHFT                                             4
#define HWIO_MSS_PERPH_MSS_CXM_WLAN_TX_STICKY_CLR_BMSK                                       0x8
#define HWIO_MSS_PERPH_MSS_CXM_WLAN_TX_STICKY_CLR_SHFT                                         3
#define HWIO_MSS_PERPH_MSS_CXM_CXM_WAKEUP_CLR_BMSK                                           0x4
#define HWIO_MSS_PERPH_MSS_CXM_CXM_WAKEUP_CLR_SHFT                                             2
#define HWIO_MSS_PERPH_MSS_CXM_BT_TX_ON_BLANKING_EN_BMSK                                     0x2
#define HWIO_MSS_PERPH_MSS_CXM_BT_TX_ON_BLANKING_EN_SHFT                                       1
#define HWIO_MSS_PERPH_MSS_CXM_WLAN_TX_ON_BLANKING_EN_BMSK                                   0x1
#define HWIO_MSS_PERPH_MSS_CXM_WLAN_TX_ON_BLANKING_EN_SHFT                                     0

#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_00_ADDR                                   (MSS_PERPH_MSS_PERPH_REG_BASE            + 0x54)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_00_RMSK                                   0xffffffff
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_00_IN                    \
                in_dword(HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_00_ADDR)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_00_INM(m)            \
                in_dword_masked(HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_00_ADDR, m)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_00_OUT(v)            \
                out_dword(HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_00_ADDR,v)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_00_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_00_ADDR,m,v,HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_00_IN)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_00_RELAY_MSG_SHADOW_DATA_00_BMSK          0xffffffff
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_00_RELAY_MSG_SHADOW_DATA_00_SHFT                   0

#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_01_ADDR                                   (MSS_PERPH_MSS_PERPH_REG_BASE            + 0x58)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_01_RMSK                                   0xffffffff
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_01_IN                    \
                in_dword(HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_01_ADDR)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_01_INM(m)            \
                in_dword_masked(HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_01_ADDR, m)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_01_OUT(v)            \
                out_dword(HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_01_ADDR,v)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_01_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_01_ADDR,m,v,HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_01_IN)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_01_RELAY_MSG_SHADOW_DATA_01_BMSK          0xffffffff
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_01_RELAY_MSG_SHADOW_DATA_01_SHFT                   0

#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_02_ADDR                                   (MSS_PERPH_MSS_PERPH_REG_BASE            + 0x5c)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_02_RMSK                                   0xffffffff
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_02_IN                    \
                in_dword(HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_02_ADDR)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_02_INM(m)            \
                in_dword_masked(HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_02_ADDR, m)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_02_OUT(v)            \
                out_dword(HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_02_ADDR,v)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_02_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_02_ADDR,m,v,HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_02_IN)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_02_RELAY_MSG_SHADOW_DATA_02_BMSK          0xffffffff
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_02_RELAY_MSG_SHADOW_DATA_02_SHFT                   0

#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_03_ADDR                                   (MSS_PERPH_MSS_PERPH_REG_BASE            + 0x60)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_03_RMSK                                   0xffffffff
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_03_IN                    \
                in_dword(HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_03_ADDR)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_03_INM(m)            \
                in_dword_masked(HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_03_ADDR, m)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_03_OUT(v)            \
                out_dword(HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_03_ADDR,v)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_03_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_03_ADDR,m,v,HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_03_IN)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_03_RELAY_MSG_SHADOW_DATA_03_BMSK          0xffffffff
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_03_RELAY_MSG_SHADOW_DATA_03_SHFT                   0

#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_04_ADDR                                   (MSS_PERPH_MSS_PERPH_REG_BASE            + 0x64)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_04_RMSK                                   0xffffffff
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_04_IN                    \
                in_dword(HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_04_ADDR)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_04_INM(m)            \
                in_dword_masked(HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_04_ADDR, m)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_04_OUT(v)            \
                out_dword(HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_04_ADDR,v)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_04_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_04_ADDR,m,v,HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_04_IN)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_04_RELAY_MSG_SHADOW_DATA_04_BMSK          0xffffffff
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_04_RELAY_MSG_SHADOW_DATA_04_SHFT                   0

#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_05_ADDR                                   (MSS_PERPH_MSS_PERPH_REG_BASE            + 0x68)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_05_RMSK                                   0xffffffff
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_05_IN                    \
                in_dword(HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_05_ADDR)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_05_INM(m)            \
                in_dword_masked(HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_05_ADDR, m)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_05_OUT(v)            \
                out_dword(HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_05_ADDR,v)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_05_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_05_ADDR,m,v,HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_05_IN)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_05_RELAY_MSG_SHADOW_DATA_05_BMSK          0xffffffff
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_05_RELAY_MSG_SHADOW_DATA_05_SHFT                   0

#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_06_ADDR                                   (MSS_PERPH_MSS_PERPH_REG_BASE            + 0x6c)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_06_RMSK                                   0xffffffff
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_06_IN                    \
                in_dword(HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_06_ADDR)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_06_INM(m)            \
                in_dword_masked(HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_06_ADDR, m)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_06_OUT(v)            \
                out_dword(HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_06_ADDR,v)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_06_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_06_ADDR,m,v,HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_06_IN)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_06_RELAY_MSG_SHADOW_DATA_06_BMSK          0xffffffff
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_06_RELAY_MSG_SHADOW_DATA_06_SHFT                   0

#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_07_ADDR                                   (MSS_PERPH_MSS_PERPH_REG_BASE            + 0x70)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_07_RMSK                                   0xffffffff
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_07_IN                    \
                in_dword(HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_07_ADDR)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_07_INM(m)            \
                in_dword_masked(HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_07_ADDR, m)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_07_OUT(v)            \
                out_dword(HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_07_ADDR,v)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_07_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_07_ADDR,m,v,HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_07_IN)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_07_RELAY_MSG_SHADOW_DATA_07_BMSK          0xffffffff
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_07_RELAY_MSG_SHADOW_DATA_07_SHFT                   0

#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_08_ADDR                                   (MSS_PERPH_MSS_PERPH_REG_BASE            + 0x74)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_08_RMSK                                   0xffffffff
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_08_IN                    \
                in_dword(HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_08_ADDR)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_08_INM(m)            \
                in_dword_masked(HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_08_ADDR, m)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_08_OUT(v)            \
                out_dword(HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_08_ADDR,v)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_08_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_08_ADDR,m,v,HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_08_IN)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_08_RELAY_MSG_SHADOW_DATA_08_BMSK          0xffffffff
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_08_RELAY_MSG_SHADOW_DATA_08_SHFT                   0

#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_09_ADDR                                   (MSS_PERPH_MSS_PERPH_REG_BASE            + 0x78)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_09_RMSK                                   0xffffffff
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_09_IN                    \
                in_dword(HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_09_ADDR)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_09_INM(m)            \
                in_dword_masked(HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_09_ADDR, m)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_09_OUT(v)            \
                out_dword(HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_09_ADDR,v)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_09_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_09_ADDR,m,v,HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_09_IN)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_09_RELAY_MSG_SHADOW_DATA_09_BMSK          0xffffffff
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_09_RELAY_MSG_SHADOW_DATA_09_SHFT                   0

#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_10_ADDR                                   (MSS_PERPH_MSS_PERPH_REG_BASE            + 0x7c)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_10_RMSK                                   0xffffffff
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_10_IN                    \
                in_dword(HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_10_ADDR)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_10_INM(m)            \
                in_dword_masked(HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_10_ADDR, m)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_10_OUT(v)            \
                out_dword(HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_10_ADDR,v)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_10_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_10_ADDR,m,v,HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_10_IN)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_10_RELAY_MSG_SHADOW_DATA_10_BMSK          0xffffffff
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_10_RELAY_MSG_SHADOW_DATA_10_SHFT                   0

#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_11_ADDR                                   (MSS_PERPH_MSS_PERPH_REG_BASE            + 0x80)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_11_RMSK                                   0xffffffff
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_11_IN                    \
                in_dword(HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_11_ADDR)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_11_INM(m)            \
                in_dword_masked(HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_11_ADDR, m)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_11_OUT(v)            \
                out_dword(HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_11_ADDR,v)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_11_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_11_ADDR,m,v,HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_11_IN)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_11_RELAY_MSG_SHADOW_DATA_11_BMSK          0xffffffff
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_11_RELAY_MSG_SHADOW_DATA_11_SHFT                   0

#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_12_ADDR                                   (MSS_PERPH_MSS_PERPH_REG_BASE            + 0x84)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_12_RMSK                                   0xffffffff
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_12_IN                    \
                in_dword(HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_12_ADDR)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_12_INM(m)            \
                in_dword_masked(HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_12_ADDR, m)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_12_OUT(v)            \
                out_dword(HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_12_ADDR,v)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_12_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_12_ADDR,m,v,HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_12_IN)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_12_RELAY_MSG_SHADOW_DATA_12_BMSK          0xffffffff
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_12_RELAY_MSG_SHADOW_DATA_12_SHFT                   0

#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_13_ADDR                                   (MSS_PERPH_MSS_PERPH_REG_BASE            + 0x88)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_13_RMSK                                   0xffffffff
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_13_IN                    \
                in_dword(HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_13_ADDR)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_13_INM(m)            \
                in_dword_masked(HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_13_ADDR, m)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_13_OUT(v)            \
                out_dword(HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_13_ADDR,v)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_13_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_13_ADDR,m,v,HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_13_IN)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_13_RELAY_MSG_SHADOW_DATA_13_BMSK          0xffffffff
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_13_RELAY_MSG_SHADOW_DATA_13_SHFT                   0

#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_14_ADDR                                   (MSS_PERPH_MSS_PERPH_REG_BASE            + 0x8c)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_14_RMSK                                   0xffffffff
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_14_IN                    \
                in_dword(HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_14_ADDR)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_14_INM(m)            \
                in_dword_masked(HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_14_ADDR, m)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_14_OUT(v)            \
                out_dword(HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_14_ADDR,v)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_14_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_14_ADDR,m,v,HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_14_IN)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_14_RELAY_MSG_SHADOW_DATA_14_BMSK          0xffffffff
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_14_RELAY_MSG_SHADOW_DATA_14_SHFT                   0

#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_15_ADDR                                   (MSS_PERPH_MSS_PERPH_REG_BASE            + 0x90)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_15_RMSK                                   0xffffffff
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_15_IN                    \
                in_dword(HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_15_ADDR)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_15_INM(m)            \
                in_dword_masked(HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_15_ADDR, m)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_15_OUT(v)            \
                out_dword(HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_15_ADDR,v)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_15_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_15_ADDR,m,v,HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_15_IN)
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_15_RELAY_MSG_SHADOW_DATA_15_BMSK          0xffffffff
#define HWIO_MSS_PERPH_MSS_RELAY_MSG_SHADOW_15_RELAY_MSG_SHADOW_DATA_15_SHFT                   0

#define HWIO_MSS_PERPH_EXT_GPIO_SEL_CFG_ADDR                                          (MSS_PERPH_MSS_PERPH_REG_BASE            + 0x94)
#define HWIO_MSS_PERPH_EXT_GPIO_SEL_CFG_RMSK                                                0xff
#define HWIO_MSS_PERPH_EXT_GPIO_SEL_CFG_IN                    \
                in_dword(HWIO_MSS_PERPH_EXT_GPIO_SEL_CFG_ADDR)
#define HWIO_MSS_PERPH_EXT_GPIO_SEL_CFG_INM(m)            \
                in_dword_masked(HWIO_MSS_PERPH_EXT_GPIO_SEL_CFG_ADDR, m)
#define HWIO_MSS_PERPH_EXT_GPIO_SEL_CFG_OUT(v)            \
                out_dword(HWIO_MSS_PERPH_EXT_GPIO_SEL_CFG_ADDR,v)
#define HWIO_MSS_PERPH_EXT_GPIO_SEL_CFG_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_PERPH_EXT_GPIO_SEL_CFG_ADDR,m,v,HWIO_MSS_PERPH_EXT_GPIO_SEL_CFG_IN)
#define HWIO_MSS_PERPH_EXT_GPIO_SEL_CFG_EXT_GPIO_SEL_CFG_BMSK                               0xff
#define HWIO_MSS_PERPH_EXT_GPIO_SEL_CFG_EXT_GPIO_SEL_CFG_SHFT                                  0

#define HWIO_MSS_PERPH_MSS_BIMC_THROTTLE_ADDR                                         (MSS_PERPH_MSS_PERPH_REG_BASE            + 0x9c)
#define HWIO_MSS_PERPH_MSS_BIMC_THROTTLE_RMSK                                                0x7
#define HWIO_MSS_PERPH_MSS_BIMC_THROTTLE_IN                    \
                in_dword(HWIO_MSS_PERPH_MSS_BIMC_THROTTLE_ADDR)
#define HWIO_MSS_PERPH_MSS_BIMC_THROTTLE_INM(m)            \
                in_dword_masked(HWIO_MSS_PERPH_MSS_BIMC_THROTTLE_ADDR, m)
#define HWIO_MSS_PERPH_MSS_BIMC_THROTTLE_OUT(v)            \
                out_dword(HWIO_MSS_PERPH_MSS_BIMC_THROTTLE_ADDR,v)
#define HWIO_MSS_PERPH_MSS_BIMC_THROTTLE_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_PERPH_MSS_BIMC_THROTTLE_ADDR,m,v,HWIO_MSS_PERPH_MSS_BIMC_THROTTLE_IN)
#define HWIO_MSS_PERPH_MSS_BIMC_THROTTLE_MSS_DANGER_BMSK                                     0x6
#define HWIO_MSS_PERPH_MSS_BIMC_THROTTLE_MSS_DANGER_SHFT                                       1
#define HWIO_MSS_PERPH_MSS_BIMC_THROTTLE_MSS_SAFE_BMSK                                       0x1
#define HWIO_MSS_PERPH_MSS_BIMC_THROTTLE_MSS_SAFE_SHFT                                         0

#define HWIO_MSS_PERPH_MSS_TCSR_ACC_SEL_ADDR                                          (MSS_PERPH_MSS_PERPH_REG_BASE            + 0xd890)
#define HWIO_MSS_PERPH_MSS_TCSR_ACC_SEL_RMSK                                                 0x3
#define HWIO_MSS_PERPH_MSS_TCSR_ACC_SEL_IN                    \
                in_dword(HWIO_MSS_PERPH_MSS_TCSR_ACC_SEL_ADDR)
#define HWIO_MSS_PERPH_MSS_TCSR_ACC_SEL_INM(m)            \
                in_dword_masked(HWIO_MSS_PERPH_MSS_TCSR_ACC_SEL_ADDR, m)
#define HWIO_MSS_PERPH_MSS_TCSR_ACC_SEL_OUT(v)            \
                out_dword(HWIO_MSS_PERPH_MSS_TCSR_ACC_SEL_ADDR,v)
#define HWIO_MSS_PERPH_MSS_TCSR_ACC_SEL_OUTM(m,v) \
                out_dword_masked_ns(HWIO_MSS_PERPH_MSS_TCSR_ACC_SEL_ADDR,m,v,HWIO_MSS_PERPH_MSS_TCSR_ACC_SEL_IN)
#define HWIO_MSS_PERPH_MSS_TCSR_ACC_SEL_ACC_MEM_SEL_BMSK                                     0x3
#define HWIO_MSS_PERPH_MSS_TCSR_ACC_SEL_ACC_MEM_SEL_SHFT                                       0


#endif /* __SLEEP_HALHWIO_H__ */
