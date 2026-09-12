/*=============================================================================

FILE:         wcss_dbg_csr.c

DESCRIPTION:  

================================================================================
            Copyright (c) 2015 Qualcomm Technologies, Inc.
                         All Rights Reserved.
          Qualcomm Technologies Proprietary and Confidential
==============================================================================*/

#include "wcss_dbg_csr.h"
#include "wcss_dbg_csr_hwio.h"



uint32 WCSS_HM_A_WCSS_DBG_CSR_CSR_WCSS_DBG_CSR_REG_BASE;            

#define WCSS_DBG_INIT_CHECK()  if (0==WCSS_HM_A_WCSS_DBG_CSR_CSR_WCSS_DBG_CSR_REG_BASE) return -1

void wcss_dbg_csr_HalConfigInit(uint32 base_addr)
{
   WCSS_HM_A_WCSS_DBG_CSR_CSR_WCSS_DBG_CSR_REG_BASE = base_addr;
}


int wcss_dbg_csr_set_cmb_owner(uint8 val)
{
   WCSS_DBG_INIT_CHECK();

   HWIO_OUTF(WCSS_HM_A_WCSS_DBG_CSR_WCSSDBGCSR_TRC_CFG,CMB_OWNER,val);
   return 0;
}

static wcss_dbg_mem_layout_change_handler g_wcss_dbg_mem_handler = NULL;


int wcss_dbg_register_mem_handler(wcss_dbg_mem_layout_change_handler handler)
{
   g_wcss_dbg_mem_handler = handler;
   return 0;
}



int wcss_dbg_csr_set_mem_layout(uint8 val)
{
   WCSS_DBG_INIT_CHECK();

   if (g_wcss_dbg_mem_handler) {
      g_wcss_dbg_mem_handler(val);
   }

   HWIO_OUTF(WCSS_HM_A_WCSS_DBG_CSR_WCSSDBGCSR_TRC_CFG,MEM_LAYOUT,val);
   return 0;
}

int wcss_dbg_csr_set_mem_owner(uint8 val)
{
   WCSS_DBG_INIT_CHECK();

   HWIO_OUTF(WCSS_HM_A_WCSS_DBG_CSR_WCSSDBGCSR_TRC_CFG,MEM_OWNER,val);
   return 0;
}


int wcss_dbg_csr_set_timestamp_source(uint8 source)
{
   WCSS_DBG_INIT_CHECK();

   HWIO_OUTF(WCSS_HM_A_WCSS_DBG_CSR_WCSSDBGCSR_TSTMP_CFG,SOURCE,source);
   return 0;
}            


int wcss_dbg_csr_set_sleep_correction(uint8 bEnable)
{
   WCSS_DBG_INIT_CHECK();

   HWIO_OUTF(WCSS_HM_A_WCSS_DBG_CSR_WCSSDBGCSR_TSTMP_CFG,SLP_CORR_EN,bEnable);
   return 0;
}            


int wcss_dbg_csr_set_cxo_freq(uint32 freq_khz)
{
   uint32 freq_hz = freq_khz * 1000;
   WCSS_DBG_INIT_CHECK();

   HWIO_OUTF(WCSS_HM_A_WCSS_DBG_CSR_WCSSDBGCSR_CXO_FREQ_IN_KHZ,FREQ,freq_khz);
   HWIO_OUTF(WCSS_HM_A_WCSS_DBG_CSR_WCSSDBGCSR_CXO_FREQ,FREQ,freq_hz);

   return 0;
}            




int wcss_dbg_csr_GenDsbEvents(uint32 events_mask)
{
   WCSS_DBG_INIT_CHECK();

   HWIO_OUT(WCSS_HM_A_WCSS_DBG_CSR_WCSSDBGCSR_SWGEN_DSB,(events_mask&0xff));
   return 0;
}            


int wcss_dbg_csr_GenDsbEvent(uint8 event_num)
{
   WCSS_DBG_INIT_CHECK();

   HWIO_OUT(WCSS_HM_A_WCSS_DBG_CSR_WCSSDBGCSR_SWGEN_DSB,(1<<event_num));
   return 0;
}            


int wcss_dbg_csr_set_apbmon_match_ctrl(uint32 match_ctrl)
{
   WCSS_DBG_INIT_CHECK();

   HWIO_OUT(WCSS_HM_A_WCSS_DBG_CSR_APBMON_MATCH_CTRL,match_ctrl);
   return 0;
}     

int wcss_dbg_csr_set_apbmon_match_details(uint32 match_details)
{
   WCSS_DBG_INIT_CHECK();

   HWIO_OUT(WCSS_HM_A_WCSS_DBG_CSR_APBMON_MATCH_DETAILS,match_details);
   return 0;
}     



int wcss_dbg_csr_set_apbmon_latency(uint32 latency)
{
   WCSS_DBG_INIT_CHECK();

   HWIO_OUT(WCSS_HM_A_WCSS_DBG_CSR_APBMON_MAX_LATENCY,(latency&0xffff));
   return 0;
}     



int wcss_dbg_csr_set_apbmon_match_addr_low(uint32 addr)
{
   WCSS_DBG_INIT_CHECK();

   HWIO_OUT(WCSS_HM_A_WCSS_DBG_CSR_APBMON_MATCH_PADDR_LO,addr);
   return 0;
}     


int wcss_dbg_csr_set_apbmon_match_addr_high(uint32 addr)
{
   WCSS_DBG_INIT_CHECK();

   HWIO_OUT(WCSS_HM_A_WCSS_DBG_CSR_APBMON_MATCH_PADDR_HI,addr);
   return 0;
}     


int wcss_dbg_csr_set_apbmon_cmb_trace(uint8 state)
{
   WCSS_DBG_INIT_CHECK();

   HWIO_OUTF(WCSS_HM_A_WCSS_DBG_CSR_APBMON_CMB_CFG,ENABLE,state);
   return 0;
}            

int wcss_dbg_csr_set_apbmon_cmb_data(uint8 state)
{
   WCSS_DBG_INIT_CHECK();

   HWIO_OUTF(WCSS_HM_A_WCSS_DBG_CSR_APBMON_CMB_CFG,DATA_EN,state);
   return 0;
}            




uint32 wcss_dbg_swgen_cmb_base = 0;
void  wcss_dbg_swgen_cmb_BaseInit(uint32 base_addr)
{
   wcss_dbg_swgen_cmb_base = base_addr;
}

int  wcss_dbg_gen_cmb(uint32 offset,uint32 value)
{
   if ( (0==wcss_dbg_swgen_cmb_base) || (offset >= 0x1000 ) ) {
      return -1;
   }
   
   offset &= ~0x3; //make 32 bit aligned
   outpdw((wcss_dbg_swgen_cmb_base+offset),value);
   return 0;
}



