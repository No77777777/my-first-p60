/*=============================================================================

FILE:         qdss_wcss.c

DESCRIPTION:  

================================================================================
            Copyright (c) 2016 Qualcomm Technologies, Inc.
                         All Rights Reserved.
          Qualcomm Technologies Proprietary and Confidential
==============================================================================*/
#include "wcss_dbg_csr.h"
#include "qdss_control.h"
#include "qdss_control_priv.h"

#define QDSS_WCSS_ENTER                         \
   int nErr=DAL_ERROR;                          \
   do  {                                        \
      if (0==qdss.hSync) {                      \
         nErr=QDSS_CONTROL_BAD_STATE;           \
         return nErr;                           \
      }                                         \
      DALSYS_SyncEnter(qdss.hSync);             \
      TRY(nErr,qdss_ensure_hw_ready());         \
   } while (0) 


#define QDSS_WCSS_LEAVE                         \
   do {                                         \
      CATCH(nErr) {}                            \
      DALSYS_SyncLeave(qdss.hSync);             \
      return nErr;                              \
   } while (0)  


int qdss_wcss_set_param(uint8 param_id, uint32 val)
{
   QDSS_WCSS_ENTER;

   switch(param_id)  {
      case WCSS_DBG_CSR_SET_CMB_OWNER:
         TRY(nErr,wcss_dbg_csr_set_cmb_owner((uint8)val));
         break;
      case WCSS_DBG_CSR_SET_MEM_LAYOUT:
         TRY(nErr,wcss_dbg_csr_set_mem_layout((uint8)val));
         break;
      case WCSS_DBG_CSR_SET_MEM_OWNER:
         TRY(nErr,wcss_dbg_csr_set_mem_owner((uint8)val));
         break;
      case WCSS_DBG_CSR_SET_TIMESTAMP_SOURCE:
         TRY(nErr,wcss_dbg_csr_set_timestamp_source((uint8)val));
         break;
      case WCSS_DBG_CSR_SET_SLEEP_CORRECTION:
         TRY(nErr,wcss_dbg_csr_set_sleep_correction((uint8)val));
         break;
      case WCSS_DBG_CSR_SET_APBMON_MATCH_CTRL:
         TRY(nErr,wcss_dbg_csr_set_apbmon_match_ctrl(val));
         break;
      case WCSS_DBG_CSR_SET_APBMON_MATCH_DETAILS:
         TRY(nErr,wcss_dbg_csr_set_apbmon_match_details(val));
         break;
      case WCSS_DBG_CSR_SET_APBMON_LATENCY:
         TRY(nErr,wcss_dbg_csr_set_apbmon_latency(val));
         break;
      case WCSS_DBG_CSR_SET_APBMON_MATCH_ADDR_LOW:
         TRY(nErr,wcss_dbg_csr_set_apbmon_match_addr_low(val));
         break;
      case WCSS_DBG_CSR_SET_APBMON_MATCH_ADDR_HIGH:
         TRY(nErr,wcss_dbg_csr_set_apbmon_match_addr_high(val));
         break;
      case WCSS_DBG_CSR_SET_APBMON_CMB_TRACE:
         TRY(nErr,wcss_dbg_csr_set_apbmon_cmb_trace((uint8)val));
         break;
      case WCSS_DBG_CSR_SET_APBMON_CMB_DATA:
         TRY(nErr,wcss_dbg_csr_set_apbmon_cmb_data((uint8)val));
         break;
      case WCSS_DBG_TSGEN_SET_TS_COUNTER:
         TRY(nErr,wcss_dbg_tsgen_set_ts_counter((uint8)val));
         break;
      default:
         break;
      }

   QDSS_WCSS_LEAVE;
}

int qdss_wcss_set_param_handler(qdss_wcss_set_param_req *pReq,
                                qdss_wcss_set_param_rsp *pRsp)
{
   return qdss_wcss_set_param(pReq->param_id,pReq->val);
}



#define QDSS_HANDLE_WCSS_CMD(cmd)                                       \
   if (pkt_len < (sizeof(qdss_wcss_req) + sizeof(cmd##_req) -1 )) {     \
      pRsp = diagpkt_err_rsp(DIAG_BAD_LEN_F, pReq, pkt_len);            \
   }                                                                    \
   else {                                                               \
      pRsp =  diagpkt_subsys_alloc                                      \
         (DIAG_SUBSYS_QDSS,                                             \
          pReq->hdr.subsysCmdCode,                                      \
          (sizeof(qdss_wcss_rsp)+sizeof(cmd##_rsp) -1) );               \
      if (NULL != pRsp) {                                               \
         nErr =  cmd##_handler((cmd##_req *)pReq->args,                 \
                               (cmd##_rsp *)(((qdss_wcss_rsp *)pRsp)->args)); \
      }                                                                 \
   }


/*-------------------------------------------------------------------------*/

/**
  @brief Handler for all WCSS diag commandds


  @return pointer to a the DIAG response packet
 */
PACK(void *)  qdss_wcss_handler(qdss_wcss_req  *pReq,
                               int pkt_len)
{
   int nErr = QDSS_CONTROL_ERROR;
   PACK(void *) pRsp = NULL;

   if (0==qdss.hSync) {
      return diagpkt_err_rsp(QDSS_CONTROL_BAD_STATE, pReq, pkt_len);
   }

   switch(pReq->id) {
   case  QDSS_WCSS_SET_PARAM:
      QDSS_HANDLE_WCSS_CMD(qdss_wcss_set_param);
      break;
   default:
      nErr=QDSS_CONTROL_ERROR;
      break;
   }


   if  (NULL == pRsp) {
      pRsp = diagpkt_err_rsp(nErr, pReq, pkt_len);
   }
   else {
      ((qdss_wcss_rsp*)pRsp)->id = pReq->id;
      ((qdss_wcss_rsp *)pRsp)->result = nErr;
   }
   return pRsp;

}
