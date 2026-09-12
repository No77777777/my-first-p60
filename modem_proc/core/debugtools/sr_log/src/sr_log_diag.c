/*
#============================================================================
#  Name:
#    sr_log_diag.c
#
#  Description:
#    Service Registry logger Diag file root image. Enables DIAG commands for 
#    Service Registry logger tests.
#
# Copyright (c) 2017 by Qualcomm Technologies, Inc.  All Rights Reserved.
#============================================================================
*/
/*=============================================================================
Edit History
$Header: //components/rel/core.mpss/10.0/debugtools/sr_log/src/sr_log_diag.c#1 $
$DateTime: 2019/04/24 00:03:26 $
$Change: 18983508 $
$Author: pwbldsvc $
===============================================================================*/

#include "sr_log_service.h"

#include "diagpkt.h"
#include "diagcmd.h"
#include "tms_diag.h"

#define SR_LOG_DIAG_PROC_ID              DIAG_QDSP6_PROC
#define SR_LOG_DIAG_PROFILE_DEFAULT      0

typedef enum
{
  SR_LOG_REQ_START_LOGGER,
  SR_LOG_REQ_ADD_CLIENT_SET,
  SR_LOG_REQ_REM_CLIENT_SET,
  SR_LOG_REQ_STOP_LOGGER,
  SIZEOF_SR_LOG_REQ_TYPE
} sr_log_req_type;

typedef PACK(struct) {
  diagpkt_subsys_header_type  hdr;
  sr_log_req_type             req_type;
  uint32_t                    profile;
}sr_log_diag_req_type;

PACK(void *) sr_log_diag_req (PACK(void *) req_ptr, uint16 pkt_len);
static diagpkt_user_table_entry_type sr_log_tbl_diag[2];

/** =====================================================================
 * Function:
 *     sr_log_diag_req
 *
 * Description:
 *     Callback registered with diag: send_data 75 37 13 <subsys> <cmd> <TBD>
 *
 * Parameters:
 *     PACK(void *) req_ptr
 *     uint16 pkt_len
 *
 * Returns:
 *     PACK(void *)
 * =====================================================================  */
PACK(void *) sr_log_diag_req (PACK(void *) req_ptr, uint16 pkt_len)
{
  sr_log_diag_req_type *req;
  sr_log_req_type type;
  uint32_t profile = 0;

  /* Build response */
  tms_diag_send_generic_response(req_ptr);
  
  /* Decode request */
  req = (sr_log_diag_req_type *) req_ptr;
  
  if (pkt_len <= (sizeof(req->hdr)+sizeof(req->req_type))) 
  {
    type = req->req_type;
  }
  else
  {
  	type = req->req_type;
    profile = req->profile; // unused
  }

  /* Execute request */
  switch(type)
  {
    case SR_LOG_REQ_START_LOGGER: 
      sr_log_req_start();
      break;
    case SR_LOG_REQ_ADD_CLIENT_SET:
      sr_log_req_reg_clients();
      break;
    case SR_LOG_REQ_REM_CLIENT_SET: 
      sr_log_req_dereg_clients();
      break;
    case SR_LOG_REQ_STOP_LOGGER: 
      sr_log_req_stop();
      break;
    default:
      TMS_MSG_ERROR_1("Unknown request: %#x",type);
  }

  return(NULL);
}

/** =====================================================================
 * Function:
 *     sr_log_diag_init
 *
 * Description:
 *     Initialize Diag for Servreg Logger
 *
 * Parameters:
 *     None
 *
 * Returns:
 *     None
 * =====================================================================  */
void sr_log_diag_init(void)
{
  sr_log_tbl_diag[0] = tms_diag_build_table_entry(TMS_DIAGPKT_SR_LOG_REQ, sr_log_diag_req);
  DIAGPKT_DISPATCH_TABLE_REGISTER_PROC(SR_LOG_DIAG_PROC_ID, DIAG_SUBSYS_DEBUG, sr_log_tbl_diag);
}