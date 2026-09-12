#ifndef IPA_SECURITY_DEFS_H
#define IPA_SECURITY_DEFS_H

/**---------------------------------------------------------------------------
 * @file     ipa_security_defs.h
 *
 * @brief    IPA Security definitions per specific IPA core flavor.
 *           Configuration from:
 *           TBD
 * 
 * Copyright (c) 2016 Qualcomm Technologies Incorporated.
 * All Rights Reserved.
 * Qualcomm Confidential and Proprietary
 *--------------------------------------------------------------------------*/

/* -----------------------------------------------------------------------
**                          INCLUDES
** ----------------------------------------------------------------------- */


/* -----------------------------------------------------------------------
**                           DEFINES
** ----------------------------------------------------------------------- */
/** 
 * @brief   The number of the Resource Group (BAM/GSI) to test the xPU (APU) of IPA
 */
#define IPA_NUMBER_OF_BAM_XPU_RG     0
#define IPA_NUMBER_OF_GSI_XPU_RG     21

/** 
 * @brief   Step in bytes between two sequential addresses into the RG
 */
#define IPA_APU_RG_STEP           4 

/** 
 * @brief   Indexes of SRAM RGs (BAM/GSI)
 */
#define IPA_BAM_SRAM_RG_INDEX                    0
#define IPA_GSI_SRAM_RG_INDEX                    14

/** 
 * @brief   Indexes of the RG that is used for MEE Communication (BAM/GSI)
 */
#define IPA_BAM_APU_RG_FOR_MEE_COMMUNICATION     IPA_BAM_SRAM_RG_INDEX
#define IPA_GSI_APU_RG_FOR_MEE_COMMUNICATION     IPA_GSI_SRAM_RG_INDEX

/**
 * @brief   Index of the pipe0 BAM RG
*/
#define IPA_BAM_IPA_APU_RG_PIPE_0_INDEX              0 

/**
 * @brief   Index of the GSI RG for AP channel context
*/
#define IPA_GSI_AP_CH_CNTXT_RG_INDEX                 3 
/**
 * @brief   Maximum value of VMIDMT Entry
*/
#define VIDRV_IPA_MAX_VMIDMT_ENTRY_ID    32

/**
 * @brief   Length in bits of VMIDMT SID 
*/
#define VIDRV_IPA_VMIDMT_SID_LEN_IN_BITS  7

/**
 * @brief   The SID that represents in VMIDMT GSI EE_0 and EE_1
 * @note    See pipe id for GSI in
 *          https://sharepoint.qualcomm.com/qct/DHW/Cores/WiredPeripherals/FG/Architecture/Documents/IPA/IPA4.0/9x75%20Endpoint%20Configuration.xlsx
*/
#define VIDRV_IPA_VMIDMT_GSI_EE_0_SID     0x20 /* = 32, pipe ID for GSI AP EE accesses */
#define VIDRV_IPA_VMIDMT_GSI_EE_1_SID     0x21 /* = 33, pipe ID for GSI Q6 EE accesses */

/** 
 * @brief   List of RG purpose's tags
 *
 * @note    The format of the name for a purpose's tag is   : IPA_RG_PTAG_xxx
 * @note    The format of the name for a RGn (n=nn) tag is  : IPA_RG_nn_PTAG
 * @note    EACH tag should be an original mask (up to 32-bits)
 *          These tags will be used to filter the using of RG in accordance to the tests logic. 
 *          So, the tag of a RG can be a combined tag (logical sum of several predefined tags). 
*/

#define IPA_RG_PTAG_WR_SHIFT           16
#define IPA_RG_PTAG_EMPTY              (0)            

#define IPA_RG_PTAG_SRAM               (1<<0)
#define IPA_RG_PTAG_IMM_CMD_EN         (1<<1)

#define IPA_RG_PTAG_RD_NONE            (1<<2)
#define IPA_RG_PTAG_WR_NONE            (IPA_RG_PTAG_RD_NONE<<IPA_RG_PTAG_WR_SHIFT)

#define IPA_RG_PTAG_RD_ALL             (0xFFF0)
#define IPA_RG_PTAG_WR_ALL             (IPA_RG_PTAG_RD_ALL<<IPA_RG_PTAG_WR_SHIFT)
#define IPA_RG_PTAG_RDWR_ALL           (IPA_RG_PTAG_RD_ALL | IPA_RG_PTAG_WR_ALL)

#define IPA_RG_PTAG_RD_AP              (1<<4)
#define IPA_RG_PTAG_WR_AP              (IPA_RG_PTAG_RD_AP<<IPA_RG_PTAG_WR_SHIFT)
#define IPA_RG_PTAG_RDWR_AP            (IPA_RG_PTAG_RD_AP | IPA_RG_PTAG_WR_AP)

#define IPA_RG_PTAG_RD_IPA_AP          (1<<5)
#define IPA_RG_PTAG_WR_IPA_AP          (IPA_RG_PTAG_RD_IPA_AP<<IPA_RG_PTAG_WR_SHIFT)
#define IPA_RG_PTAG_RDWR_IPA_AP        (IPA_RG_PTAG_RD_IPA_AP | IPA_RG_PTAG_WR_IPA_AP)

#define IPA_RG_PTAG_RD_MSA             (1<<6)
#define IPA_RG_PTAG_WR_MSA             (IPA_RG_PTAG_RD_MSA<<IPA_RG_PTAG_WR_SHIFT)
#define IPA_RG_PTAG_RDWR_MSA           (IPA_RG_PTAG_RD_MSA | IPA_RG_PTAG_WR_MSA)

#define IPA_RG_PTAG_RD_TZ              (1<<7)
#define IPA_RG_PTAG_WR_TZ              (IPA_RG_PTAG_RD_TZ<<IPA_RG_PTAG_WR_SHIFT)
#define IPA_RG_PTAG_RDWR_TZ            (IPA_RG_PTAG_RD_TZ | IPA_RG_PTAG_WR_TZ)

#define IPA_RG_PTAG_RD_IPA_UC          (1<<8)
#define IPA_RG_PTAG_WR_IPA_UC          (IPA_RG_PTAG_RD_IPA_UC<<IPA_RG_PTAG_WR_SHIFT)
#define IPA_RG_PTAG_RDWR_IPA_UC        (IPA_RG_PTAG_RD_IPA_UC | IPA_RG_PTAG_WR_IPA_UC)

#define IPA_RG_PTAG_RD_PCIE_AP         (1<<9)
#define IPA_RG_PTAG_WR_PCIE_AP         (IPA_RG_PTAG_RD_PCIE_AP<<IPA_RG_PTAG_WR_SHIFT)
#define IPA_RG_PTAG_RDWR_PCIE_AP       (IPA_RG_PTAG_RD_PCIE_AP | IPA_RG_PTAG_WR_PCIE_AP)

#define IPA_RG_PTAG_RD_RPM             (1<<10)
#define IPA_RG_PTAG_WR_RPM             (IPA_RG_PTAG_RD_RPM<<IPA_RG_PTAG_WR_SHIFT)
#define IPA_RG_PTAG_RDWR_RPM           (IPA_RG_PTAG_RD_RPM | IPA_RG_PTAG_WR_RPM)

#define IPA_GSI_XPU_RG_0_PTAG          ( IPA_RG_PTAG_RDWR_AP | IPA_RG_PTAG_RDWR_MSA | IPA_RG_PTAG_RDWR_TZ                                                       )
#define IPA_GSI_XPU_RG_1_PTAG          ( IPA_RG_PTAG_RDWR_AP | IPA_RG_PTAG_RDWR_MSA | IPA_RG_PTAG_RDWR_TZ                                                       )
#define IPA_GSI_XPU_RG_2_PTAG          ( IPA_RG_PTAG_RDWR_MSA | IPA_RG_PTAG_RDWR_TZ                                                                             )
#define IPA_GSI_XPU_RG_3_PTAG          ( IPA_RG_PTAG_RDWR_AP | IPA_RG_PTAG_RDWR_IPA_AP |  IPA_RG_PTAG_RD_MSA | IPA_RG_PTAG_RD_TZ                                )
#define IPA_GSI_XPU_RG_4_PTAG          ( IPA_RG_PTAG_RDWR_MSA | IPA_RG_PTAG_RD_AP                                                                               )
#define IPA_GSI_XPU_RG_5_PTAG          ( IPA_RG_PTAG_RDWR_MSA | IPA_RG_PTAG_RD_AP                                                                               )
#define IPA_GSI_XPU_RG_6_PTAG          ( IPA_RG_PTAG_RDWR_MSA | IPA_RG_PTAG_RDWR_AP                                                                             )
#define IPA_GSI_XPU_RG_7_PTAG          ( IPA_RG_PTAG_RDWR_MSA                                                                                                   )
#define IPA_GSI_XPU_RG_8_PTAG          ( IPA_RG_PTAG_RDWR_MSA                                                                                                   )
#define IPA_GSI_XPU_RG_9_PTAG          ( IPA_RG_PTAG_RD_MSA | IPA_RG_PTAG_RDWR_AP                                                                               )
#define IPA_GSI_XPU_RG_10_PTAG         ( IPA_RG_PTAG_RDWR_MSA                                                                                                   )
#define IPA_GSI_XPU_RG_11_PTAG         ( IPA_RG_PTAG_RDWR_MSA                                                                                                   )
#define IPA_GSI_XPU_RG_12_PTAG         ( IPA_RG_PTAG_RDWR_AP | IPA_RG_PTAG_RDWR_IPA_AP | IPA_RG_PTAG_RDWR_MSA | IPA_RG_PTAG_RDWR_TZ                             )
#define IPA_GSI_XPU_RG_13_PTAG         ( IPA_RG_PTAG_RDWR_MSA | IPA_RG_PTAG_RDWR_TZ                                                                             )
#define IPA_GSI_XPU_RG_14_PTAG         ( IPA_RG_PTAG_RDWR_AP | IPA_RG_PTAG_RDWR_IPA_AP | IPA_RG_PTAG_RDWR_MSA | IPA_RG_PTAG_RDWR_TZ                             )
#define IPA_GSI_XPU_RG_15_PTAG         ( IPA_RG_PTAG_RDWR_MSA | IPA_RG_PTAG_RDWR_TZ | IPA_RG_PTAG_RDWR_AP                                                       )
#define IPA_GSI_XPU_RG_16_PTAG         ( IPA_RG_PTAG_RD_MSA | IPA_RG_PTAG_WR_NONE                                                                               )
#define IPA_GSI_XPU_RG_17_PTAG         ( IPA_RG_PTAG_RDWR_MSA | IPA_RG_PTAG_RDWR_AP | IPA_RG_PTAG_RDWR_TZ                                                       )
#define IPA_GSI_XPU_RG_18_PTAG         ( IPA_RG_PTAG_RDWR_MSA | IPA_RG_PTAG_RD_AP                                                                               )
#define IPA_GSI_XPU_RG_19_PTAG         ( IPA_RG_PTAG_RDWR_MSA                                                                                                   )
#define IPA_GSI_XPU_RG_20_PTAG         ( IPA_RG_PTAG_RDWR_IPA_AP | IPA_RG_PTAG_RDWR_MSA | IPA_RG_PTAG_RDWR_TZ | IPA_RG_PTAG_RDWR_PCIE_AP | IPA_RG_PTAG_RDWR_RPM )

#endif /* IPA_SECURITY_DEFS_H */
