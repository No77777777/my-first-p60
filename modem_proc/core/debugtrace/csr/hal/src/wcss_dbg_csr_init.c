/*=============================================================================

FILE:         qdss_csr_config.c

DESCRIPTION:  

================================================================================
            Copyright (c) 2014 Qualcomm Technologies, Inc.
                         All Rights Reserved.
          Qualcomm Technologies Proprietary and Confidential
==============================================================================*/
#include "qdss_utils.h"
#include "DALSys.h"
#include "qdss_csr_config.h"
#include "halqdss_csr.h"
#include "wcss_dbg_csr.h"


struct qdss_csr_context {
   DALSYSMemHandle chip_csr_mem_handle;
   DALSYSMemInfo   chip_csr_mem_info;

   DALSYSMemHandle wcss_csr_mem_handle;
   DALSYSMemInfo   wcss_csr_mem_info;

   DALSYSMemHandle wcss_tsgen_mem_handle;
   DALSYSMemInfo   wcss_tsgen_mem_info;

   DALSYSMemHandle swgen_cmb_mem_handle;
   DALSYSMemInfo   swgen_cmb_mem_info;

};

static struct qdss_csr_context qdss_csr;


void wcss_dbg_csr_HalConfigInit(uint32 base_addr);
void wcss_dbg_swgen_cmb_BaseInit(uint32 base_addr);
void wcss_dbg_tsgen_BaseInit(uint32 base_addr);


DALResult qdss_csr_dal_memmap(uint32 physAddr,
                          uint32 size,
                          uint32 dwAttribs,
                          DALSYSMemHandle *phMem,
                          DALSYSMemInfo *pMemInfo)
{
   int nErr;

   TRY(nErr,DALSYS_MemRegionAlloc(dwAttribs,
                                  DALSYS_MEM_ADDR_NOT_SPECIFIED,
                                  physAddr,
                                  size, phMem, NULL ));
   DALSYS_MemInfo(*phMem, pMemInfo);

   CATCH(nErr) {}
   return nErr;
}



int wcss_dbg_csr_init(void) 
{
   int nErr;


   TRY(nErr,qdss_csr_dal_memmap(QDSS_QDSSCSR_REG_BASE_PHYS,
                            0x1000,
                            DALSYS_MEM_PROPS_HWIO,
                            &qdss_csr.chip_csr_mem_handle,
                            &qdss_csr.chip_csr_mem_info));
   HAL_qdss_csr_HalConfigInit(qdss_csr.chip_csr_mem_info.VirtualAddr);


   TRY(nErr,qdss_csr_dal_memmap(WCSS_DBG_CSR_REG_BASE_PHYS,
                                0x1000,
                                DALSYS_MEM_PROPS_HWIO,
                                &qdss_csr.wcss_csr_mem_handle,
                                &qdss_csr.wcss_csr_mem_info));
   wcss_dbg_csr_HalConfigInit(qdss_csr.wcss_csr_mem_info.VirtualAddr);



   TRY(nErr,qdss_csr_dal_memmap(WCSS_DBG_TSGEN_BASE_PHYS,
                                0x1000,
                                DALSYS_MEM_PROPS_HWIO,
                                &qdss_csr.wcss_tsgen_mem_handle,
                                &qdss_csr.wcss_tsgen_mem_info));
   wcss_dbg_tsgen_BaseInit(qdss_csr.wcss_tsgen_mem_info.VirtualAddr);



   TRY(nErr,qdss_csr_dal_memmap(WCSS_DBG_SWGEN_CMB_BASE_PHYS,
                                0x1000,
                                DALSYS_MEM_PROPS_HWIO,
                                &qdss_csr.swgen_cmb_mem_handle,
                                &qdss_csr.swgen_cmb_mem_info));
   wcss_dbg_swgen_cmb_BaseInit(qdss_csr.swgen_cmb_mem_info.VirtualAddr);


   wcss_dbg_csr_set_mem_layout(WCSSDBG_MEM_LAYOUT_ETB);
   wcss_dbg_csr_set_mem_owner(WCSSDBG_MEM_OWNER_ETB);

   wcss_dbg_csr_set_timestamp_source(WCSSDBGCSR_TSTMP_SOURCE_WCSSDBG);

   wcss_dbg_csr_set_cxo_freq(19200);

   //Enable timestamp generator
   wcss_dbg_tsgen_set_ts_counter(1);


   CATCH(nErr) {}

   return nErr;
}

