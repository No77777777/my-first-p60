/**
  @file bamtgtcfgdata.h
  @brief
  This file contains configuration data for the BAM driver for the 
  9x45 mpss system.

*/
/*
===============================================================================

                             Edit History

 $Header: //components/rel/core.mpss/10.0/settings/hwengines/bam/config/kamorta/bamtgtcfgdata.h#2 $

when       who     what, where, why
--------   ---     ------------------------------------------------------------
06/07/19   ay      Updated for Kamorta
09/25/18   mks     Updated for nicobar
06/16/17   ks      Updated for 670
07/29/16   ss      Updated for 845
03/04/16   ss      Updated for 8998
04/12/15   ss      Updated for 9x55
04/24/13   rl      branched from 9x35

===============================================================================
                   Copyright (c) 2013-2016, 2019 QUALCOMM Technologies Incorporated.
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
        {                     // CE_M_BAM
         /* .bam_pa     */    0x61C4000,
         /* .options    */    BAM_TGT_CFG_CHECK_DESC_EOT | BAM_TGT_CFG_ENABLE_MD,
         /* .cfg_bits   */    BAM_CNFG_BITS_VAL,
         /* .ee         */    0,               
         /* .sec_config */    NULL,           
         /* .size       */    BAM_MAX_MMAP
        },
        {                     //AUD_SLIMBUS_BAM
         /* .bam_pa     */    0x0A584000,
         /* .options    */    BAM_TGT_CFG_NO_INIT,
         /* .cfg_bits   */    BAM_CNFG_BITS_VAL,
         /* .ee         */    2,
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


