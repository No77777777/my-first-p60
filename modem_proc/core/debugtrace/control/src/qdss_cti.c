/*=============================================================================

FILE:         qdss_cti.c

DESCRIPTION:  

================================================================================
            Copyright (c) 2016 Qualcomm Technologies, Inc.
                         All Rights Reserved.
          Qualcomm Technologies Proprietary and Confidential
==============================================================================*/
#include "qdss_control.h"
#include "qdss_control_priv.h"
#include "qdss_diag.h"

static __inline uint32 endian_swap32(uint32 x)
{
    return (x>>24) |
        ((x<<8) & 0x00FF0000) |
        ((x>>8) & 0x0000FF00) |
        (x<<24);
}


int qdss_control_map_cti(const char *block,
                            uint8 type,
                            uint8 trigger,
                            uint8 channel)
{
   int nErr = QDSS_CONTROL_UNSUPPORTED;

   if (0 == qdss.hSync) {
       return QDSS_CONTROL_BAD_STATE;
   }

   DALSYS_SyncEnter(qdss.hSync);
   TRY(nErr, qdss_ensure_hw_ready());

   THROW_IF(nErr,DAL_SUCCESS !=
            DALCTI_Map(qdss.hCTI, block, type, trigger, channel),
            QDSS_CONTROL_ERROR);

   CATCH(nErr) {}

   DALSYS_SyncLeave(qdss.hSync);
   return nErr;
}

/*-------------------------------------------------------------------------*/

int qdss_control_unmap_cti(const char *block,
                            uint8 type,
                            uint8 trigger,
                            uint8 channel)
{
   int nErr;

   if (0 == qdss.hSync) {
       return QDSS_CONTROL_BAD_STATE;
   }

   DALSYS_SyncEnter(qdss.hSync);
   TRY(nErr, qdss_ensure_hw_ready());

   THROW_IF(nErr,DAL_SUCCESS !=
            DALCTI_Unmap(qdss.hCTI, block, type, trigger, channel),
            QDSS_CONTROL_ERROR);

   CATCH(nErr) {}

   DALSYS_SyncLeave(qdss.hSync);
   return nErr;
}

/*-------------------------------------------------------------------------*/

int qdss_control_reset_cti(const char *block)
{
   int nErr;

   if (0 == qdss.hSync) {
       return QDSS_CONTROL_BAD_STATE;
   }

   DALSYS_SyncEnter(qdss.hSync);
   TRY(nErr, qdss_ensure_hw_ready());

   THROW_IF(nErr,DAL_SUCCESS !=
            DALCTI_Reset(qdss.hCTI, block),
            QDSS_CONTROL_ERROR);

   CATCH(nErr) {}

   DALSYS_SyncLeave(qdss.hSync);
   return nErr;
}

/*-------------------------------------------------------------------------*/

int qdss_control_query_cti(const char *block,
                            uint8 type,
                            uint8 max_triggers,
                            uint8 *length,
                            __packed uint32 *mappings)
{
   int nErr;
   uint32 i;

   if (0 == qdss.hSync) {
       return QDSS_CONTROL_BAD_STATE;
   }

   DALSYS_SyncEnter(qdss.hSync);
   TRY(nErr, qdss_ensure_hw_ready());

   memset((void *) mappings, 0x0, MAX_TRIGGERS * sizeof(uint32));

   THROW_IF(nErr,DAL_SUCCESS !=
            DALCTI_Query(qdss.hCTI,
                         block,
                         type,
                         max_triggers,
                         length,
                         mappings),
            QDSS_CONTROL_ERROR);

   /* Response expects each uint32 to be big-endian. */
   for (i = 0; i < *length; i++) {
       mappings[i] = endian_swap32(mappings[i]);
   }

   CATCH(nErr) {}

   DALSYS_SyncLeave(qdss.hSync);
   return nErr;
}


int qdss_control_trigger_cti(const char *block,
                           uint8 channel,
                           uint8 trigger_type)
{
   int nErr;

   if (0 == qdss.hSync) {
       return QDSS_CONTROL_BAD_STATE;
   }

   DALSYS_SyncEnter(qdss.hSync);
   TRY(nErr, qdss_ensure_hw_ready());

   THROW_IF(nErr,DAL_SUCCESS !=
            DALCTI_Trigger(qdss.hCTI, block, channel,trigger_type),
            QDSS_CONTROL_ERROR);

   CATCH(nErr) {}

   DALSYS_SyncLeave(qdss.hSync);
   return nErr;
}

/*-------------------------------------------------------------------------*/

int qdss_map_cti_handler(qdss_map_cti_req *pReq,
                          int req_len,
                          qdss_map_cti_rsp *pRsp,
                          int rsp_len)
{
  int nErr = QDSS_CONTROL_UNSUPPORTED;


  /* Ensure string is null-terminated */
  pReq->block[CTI_BLOCK_SIZE - 1] = 0;

  TRY(nErr, qdss_control_map_cti(pReq->block,
                                    pReq->type,
                                    pReq->trigger,
                                    pReq->channel));

  CATCH(nErr) {}
  pRsp->result = nErr;
  return nErr;
}

int qdss_unmap_cti_handler(qdss_unmap_cti_req *pReq,
                          int req_len,
                          qdss_unmap_cti_rsp *pRsp,
                          int rsp_len)
{

  int nErr = QDSS_CONTROL_UNSUPPORTED;

  /* Ensure string is null-terminated */
  pReq->block[CTI_BLOCK_SIZE - 1] = 0;

  TRY(nErr, qdss_control_unmap_cti(pReq->block,
                                    pReq->type,
                                    pReq->trigger,
                                    pReq->channel));

  CATCH(nErr) {}
  pRsp->result = nErr;
  return nErr;
}

int qdss_reset_cti_handler(qdss_reset_cti_req *pReq,
                          int req_len,
                          qdss_reset_cti_rsp *pRsp,
                          int rsp_len)
{
  int nErr = QDSS_CONTROL_UNSUPPORTED;

  /* Ensure string is null-terminated */
  pReq->block[CTI_BLOCK_SIZE - 1] = 0;

  TRY(nErr, qdss_control_reset_cti(pReq->block));

  CATCH(nErr) {}
  pRsp->result = nErr;
  return nErr;
}

int qdss_query_cti_handler(qdss_query_cti_req *pReq,
                          int req_len,
                          qdss_query_cti_rsp *pRsp,
                          int rsp_len)
{
  int nErr = QDSS_CONTROL_UNSUPPORTED;

  /* Ensure string is null-terminated */
  pReq->block[CTI_BLOCK_SIZE - 1] = 0;

  TRY(nErr, qdss_control_query_cti(pReq->block,
                                    pReq->type,
                                    MAX_TRIGGERS,
                                    &pRsp->length,
                                    pRsp->mappings));

  CATCH(nErr) {}
  pRsp->result = nErr;
  return nErr;
}

#ifdef CTI_TRIGGER_DIAG_COMMAND
int qdss_trigger_cti_handler(qdss_trigger_cti_req *pReq,
                          int req_len,
                          qdss_trigger_cti_rsp *pRsp,
                          int rsp_len)
{

  int nErr = QDSS_CONTROL_UNSUPPORTED;

  /* Ensure string is null-terminated */
  pReq->block[CTI_BLOCK_SIZE - 1] = 0;

  TRY(nErr, qdss_control_trigger_cti(pReq->block,
                                   pReq->channel,
                                   pReq->trigger_type));

  CATCH(nErr) {}
  pRsp->result = nErr;
  return nErr;
}
#endif
