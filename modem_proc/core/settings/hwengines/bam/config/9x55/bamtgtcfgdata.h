/**
  @file bamtgtcfgdata_mpss.h
  @brief
  This file contains configuration data for the BAM driver for the 
  9x45 mpss system.

*/
/*
===============================================================================

                             Edit History

 $Header: //components/rel/core.mpss/10.0/settings/hwengines/bam/config/9x55/bamtgtcfgdata.h#1 $

when       who     what, where, why
--------   ---     ------------------------------------------------------------
04/12/15   ss      Updated for 9x55
04/24/13   rl      branched from 9x35

===============================================================================
                   Copyright (c) 2013-2015 QUALCOMM Technologies Incorporated.
                          All Rights Reserved.
                        Qualcomm Confidential and Proprietary.
===============================================================================
*/

/** Configs supported for
    CE_M
    BLSP1
    QPIC
    IPA
    SLIMBUS
    BAM_TGT_CFG_LAST
 */

#define BAM_CNFG_BITS_VAL 0xFFFFF004

const bam_target_config_type  bam_tgt_config[] = {
        {                     // CE_M_BAM
         /* .bam_pa     */    0x41C4000,
         /* .options    */    0x0,      
         /* .cfg_bits   */    BAM_CNFG_BITS_VAL,
         /* .ee         */    0,               
         /* .sec_config */    NULL,           
         /* .size       */    BAM_MAX_MMAP
        },
        {                     // BLSP_BAM
         /* .bam_pa     */    0x7884000,
         /* .options    */    (BAM_TGT_CFG_SHARABLE|BAM_TGT_CFG_NO_INIT),
         /* .cfg_bits   */    BAM_CNFG_BITS_VAL,
         /* .ee         */    2,
         /* .sec_config */    NULL,
         /* .size       */    BAM_MAX_MMAP
        },
        {                     // QPIC_BAM
         /* .bam_pa     */    0x7984000,
         /* .options    */    0x0,
         /* .cfg_bits   */    BAM_CNFG_BITS_VAL,
         /* .ee         */    1,
         /* .sec_config */    NULL,
         /* .size       */    BAM_MAX_MMAP
        },
        {                     //IPA_BAM
         /* .bam_pa     */    0x7B84000,
         /* .options    */    0,
         /* .cfg_bits   */    BAM_CNFG_BITS_VAL,
         /* .ee         */    1,
         /* .sec_config */    NULL,
         /* .size       */    BAM_MAX_MMAP
        },
        {                     //SLIMBUS_BAM
         /* .bam_pa     */    0x7784000,
         /* .options    */    BAM_TGT_CFG_NO_INIT,
         /* .cfg_bits   */    BAM_CNFG_BITS_VAL,
         /* .ee         */    1,
         /* .sec_config */    NULL,
         /* .size       */    BAM_MAX_MMAP
        },
        {                     //LAST
         /* .bam_pa     */    BAM_TGT_CFG_LAST,
         /* .options    */    0,
         /* .cfg_bits   */    0,
         /* .ee         */    0,
         /* .sec_config */    NULL,
         /* .size       */    0
        },
};


