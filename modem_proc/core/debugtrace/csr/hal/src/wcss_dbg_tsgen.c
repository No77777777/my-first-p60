/*=============================================================================

FILE:         wcss_dbg_tsgen.c

DESCRIPTION:  

================================================================================
            Copyright (c) 2016 Qualcomm Technologies, Inc.
                         All Rights Reserved.
          Qualcomm Technologies Proprietary and Confidential
==============================================================================*/
#include "wcss_dbg_tsgen_hwio.h"


uint32 WCSS_HM_A_WCSS_DBG_TSGEN_TSGEN_CXTSGEN_REG_BASE;

void wcss_dbg_tsgen_BaseInit(uint32 base_addr) 
{
   WCSS_HM_A_WCSS_DBG_TSGEN_TSGEN_CXTSGEN_REG_BASE=base_addr;
}

int wcss_dbg_tsgen_set_ts_counter(uint8 bEnable) 
{
   if (0==WCSS_HM_A_WCSS_DBG_TSGEN_TSGEN_CXTSGEN_REG_BASE) {
      return -1;
   }
	HWIO_OUTF(WCSS_HM_A_WCSS_DBG_TSGEN_CNTCR,CNTCR_EN, bEnable);
   return 0;
}
