   /**
  @file bamtgtcfgdata.h
  @brief
  This file contains configuration data for the BAM driver for the 
  9x45 mpss system.

*/
/*
===============================================================================

                             Edit History

 

when       who     what, where, why
--------   ---     ------------------------------------------------------------
03/07/17   wjz     Updated for SDX24
07/29/16   ss      Updated for 845
03/04/16   ss      Updated for 8998
04/12/15   ss      Updated for 9x55
04/24/13   rl      branched from 9x35

===============================================================================
                   Copyright (c) 2013-2016 QUALCOMM Technologies Incorporated.
                          All Rights Reserved.
                        Qualcomm Confidential and Proprietary.
===============================================================================
*/

/** Configs supported for
    CE_M
    SLIMBUS
    BAM_TGT_CFG_LAST
          */

#define BAM_CNFG_BITS_VAL 0xFFFFF004

const bam_target_config_type  bam_tgt_config[] = {

        {                     // BLSP2_BAM
         /* .bam_pa     */    0x804000,
         /* .options    */    (BAM_TGT_CFG_SHARABLE|BAM_TGT_CFG_NO_INIT),
         /* .cfg_bits   */    BAM_CNFG_BITS_VAL,
         /* .ee         */    2,
         /* .sec_config */    NULL,
         /* .size       */    BAM_MAX_MMAP
        },

        {                     //QPIC_BAM
         /* .bam_pa     */    0x1B04000,
         /* .options    */    BAM_TGT_CFG_NO_INIT,
         /* .cfg_bits   */    BAM_CNFG_BITS_VAL,
         /* .ee         */    2,
         /* .sec_config */    NULL,
         /* .size       */    BAM_MAX_MMAP
        },


        {                     // CE_M_BAM
         /* .bam_pa     */    0x41C4000,
         /* .options    */    0x0,      
         /* .cfg_bits   */    BAM_CNFG_BITS_VAL,
         /* .ee         */    0,               
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


