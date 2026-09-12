#if !defined(DIAG_MP)

/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                          Logging Services

General Description
  This file contains the implementation for the log mask handlers in standalone configuration.

Copyright (c) 2018 by QUALCOMM Technologies, Incorporated.  All Rights Reserved.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*==========================================================================

                              Edit History

$Header: //components/rel/core.mpss/10.0/services/diag/DCM/common/src/diaglog_standalone.c#1 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
07/04/18    gn     Created new file for standalone functionality of logs

===========================================================================*/

#include "diaglogi.h"
#include "diagi_v.h" /* For diagcomm_apps_only_config */
#include "diagdiag_v.h" /* For mask command request/response structures */
#include "diagcomm_cmd.h"  /* For diagcomm_cmd_status */
#include "log_arrays_i.h" /* For log_last_item_tbl */

static const byte log_valid_mask[LOG_MASK_SIZE];

extern uint8 log_on_demand_reg_count;
extern uint8 diag_cur_preset_id; /* from diag.c */
extern uint16 log_mask_offset_tbl[LOG_EQUIP_ID_LAST+1];
extern byte * log_mask[DIAG_MAX_STREAM_ID];
extern byte diag_log_preset_mask[DIAG_MAX_PRESET_ID][LOG_MASK_SIZE];    /* Stream 1 QXDM masks. */
extern log_on_demand_list_item_type log_on_demand_list[LOG_ON_DEMAND_SIZE];
extern diagcomm_io_conn_type diagcomm_io_ctrl_conn[NUM_SMD_PORT_TYPES][NUM_SMD_PORTS]; /* From diagcomm_io.c */


/*===========================================================================

FUNCTION LOG_PROCESS_EXT_LOGMASK_PKT

DESCRIPTION
  This procedure processes a request to change the logmask.

RETURN VALUE
  Pointer to response packet.

WARNING
  This function support Obsolete cmd 93, which is replaced by cmd 115.
  However, support for 93 is left in place to support Legacy tools

  When num_bits == 0 in the request, the command queries the log valid
  mask. However, looking at the code below, log_valid_mask[] is never set.
  Therefore the "get log mask" request for cmd 93 is likely broken.

===========================================================================*/
PACK(void *) log_process_ext_logmask_pkt (
  PACK(void *) req_pkt_ptr, /* pointer to request packet */
  uint16 pkt_len            /* length of request packet  */
)
{
  DIAG_EXT_LOGMASK_F_req_type *req_ptr = (DIAG_EXT_LOGMASK_F_req_type *) req_pkt_ptr;
  DIAG_EXT_LOGMASK_F_rsp_type *rsp_ptr = NULL;
  const unsigned int rsp_len = sizeof(DIAG_EXT_LOGMASK_F_rsp_type);

  uint32 i;
  const byte *valid_mask_ptr = NULL;
  boolean enabled;
  boolean send_rsp = TRUE;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Quick sanity check */
  if (pkt_len != FPOS(DIAG_EXT_LOGMASK_F_req_type, mask) +
                 (req_ptr->num_bits + 7) / 8) {
    return( diagpkt_err_rsp(DIAG_BAD_PARM_F, req_pkt_ptr, pkt_len));
  }

  if (req_ptr->num_bits == 0) {
    /*-------------------------------------------------------
      Report the number of valid bits as well as the mask of
      valid bits for this build
    -------------------------------------------------------*/
    rsp_ptr = (DIAG_EXT_LOGMASK_F_rsp_type *) diagpkt_alloc(DIAG_EXT_LOGMASK_F, rsp_len);
    if(rsp_ptr){
    rsp_ptr->num_valid_bits = log_last_item_tbl[1] + 1;

    valid_mask_ptr = &log_valid_mask[log_mask_offset_tbl[1]];

    for (i = 0; i < DIAG_EXT_LOGMASK_NUM_BYTES; i++) {
      rsp_ptr->valid_mask[i] = 0;
    }

    for (i = 0; i < DIAG_EXT_LOGMASK_NUM_BITS; i++) {
      enabled = (log_mask_enabled(log_valid_mask, 1, i)) ? TRUE : FALSE;

      if (enabled) {
        rsp_ptr->valid_mask[i >> 3] |= 0x80 >> (i & 7);
      }
    }
   }
  } else {

   (void) valid_mask_ptr; /* to supress lint (Warning) 550 */

    /* Process the logmask */
    log_process_ext_logmask(&req_ptr->mask[0], req_ptr->num_bits);

    /* Only return a command code */
    rsp_ptr = (DIAG_EXT_LOGMASK_F_rsp_type *) diagpkt_alloc(DIAG_EXT_LOGMASK_F,
      FPOS(DIAG_EXT_LOGMASK_F_rsp_type, num_valid_bits));
  }


  /* if master processor on multi processor build, don't send response */
  #if !defined(DIAG_RSP_SEND)
  if(!diagcomm_apps_only_config)
    send_rsp = FALSE;
  #endif

  #if defined(DIAG_MP_MASTER)
    /* MP Master now responds to "set mask" requests.
       If we're in buffering mode, Master needs to send out the "get mask" responses.
       If Modem is not up, Master needs to send out responses as well.
    */
    send_rsp = TRUE;
  #endif

/* if diag is not connected, don't send response */
  if (!diagcomm_cmd_status(DIAGCOMM_PORT_1))
  {
    send_rsp = FALSE;
  }

/* return response */
  if (!send_rsp)
  {
    diagpkt_free(rsp_ptr);
    return NULL;
  }
  else
  {
    return ((void *) rsp_ptr);
  }
} /* log_process_ext_logmask_pkt */


/*===========================================================================

FUNCTION LOG_PROCESS_LEGACY_LOGMASK

DESCRIPTION
  This procedure processes a request to change the logmask.
  Note: This is for the orignal 32-bit logmask that the DM used.

DEPENDENCIES
  None.

RETURN VALUE
  Pointer to response packet.

SIDE EFFECTS
  None.

===========================================================================*/
void * log_process_legacy_logmask (
  void * req_pkt_ptr,
  uint16 pkt_len
)
{
  uint32 i;                /* index to loop through logmask */
  boolean code_enabled; /* Used to determine if a code is enabled. */

  DIAG_LOGMASK_F_req_type *req_ptr = (DIAG_LOGMASK_F_req_type *) req_pkt_ptr;
  DIAG_LOGMASK_F_rsp_type *rsp_ptr = NULL;
  boolean send_rsp = TRUE;
  const unsigned int rsp_len = sizeof(DIAG_LOGMASK_F_rsp_type);

  unsigned int mask = req_ptr->mask;
  const uint32 numbits = log_last_item_tbl[1] + 1;

  (void) pkt_len; /* suppress compiler warning */
 /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* For each bit allowed in the logging mask, configure that log condition
   * active if the bit is 1 and configure that log condition inactive if
   * the bit is 0. */

  /* Tell lint it is OK to have a constant used as a boolean here */
  /*lint -e506 */
  for( i = 0; i < MIN(numbits, 32); i++ )
  {
  /*lint +e506 */

    code_enabled = (boolean) (mask & 0x01);

    /* Configure logmask */
    (void) log_config_mask(log_set_equip_id(i, 1), code_enabled, DIAG_STREAM_1, diag_cur_preset_id); //ToDo: save to master?

    mask >>= 1;
  }

  /* Set any codes above 32 to zero. */
  for(; i < numbits; i++)
  {
    (void) log_config_mask (log_set_equip_id(i, 1), FALSE, DIAG_STREAM_1, diag_cur_preset_id); //ToDo: save to master?
  }

  /* Vote on sleep for the logging service. */
  (void) log_sleep_vote ();

  /* if master processor on multi processor build, don't send response */
  #if !defined(DIAG_RSP_SEND)
  if (!diagcomm_apps_only_config)
    send_rsp = FALSE;
  #endif

/* if diag is not connected, don't send response */
  if (!diagcomm_cmd_status(DIAGCOMM_PORT_1))
  {
    send_rsp = FALSE;
  }
  if (send_rsp)
    rsp_ptr = (DIAG_LOGMASK_F_rsp_type *) diagpkt_alloc (DIAG_LOGMASK_F, rsp_len);

  (void)diag_client_maskchange(DIAG_LOGSVC_MASK_CHANGE);
  return (rsp_ptr);

} /* log_process_legacy_logmask */

/*===========================================================================

FUNCTION LOG_ON_DEMAND_PKT

DESCRIPTION
  This procedure processes a log on demand request. It scans in the
  log_on_demand_list to match the log_code. If it find the log_code it
  processes the function pointer associated with it.

DEPENDENCIES
  None.

RETURN VALUE
  Pointer to response packet.

SIDE EFFECTS
  None.

===========================================================================*/
void * log_on_demand_pkt (
  void * req_pkt_ptr,
  uint16 pkt_len
)
{
  DIAG_LOG_ON_DEMAND_F_req_type *req_ptr = (DIAG_LOG_ON_DEMAND_F_req_type *) req_pkt_ptr;
  DIAG_LOG_ON_DEMAND_F_rsp_type *rsp_ptr = NULL;
  boolean send_rsp = TRUE;
  int i;
   const unsigned int rsp_len = sizeof( DIAG_LOG_ON_DEMAND_F_rsp_type );

  if ((req_ptr == NULL) || ((int)(req_ptr->xx_header.opaque_header) != DIAG_LOG_ON_DEMAND_F))
  {
    return NULL;
  }
   (void) pkt_len; // suppress compiler warning
  rsp_ptr = (DIAG_LOG_ON_DEMAND_F_rsp_type *) diagpkt_alloc( DIAG_LOG_ON_DEMAND_F, rsp_len );

  if( rsp_ptr == NULL ) {
    // If we can't allocate, diagpkt_err_rsp() can't either.
    return NULL;
  }

  rsp_ptr->log_code = req_ptr->log_code;
  rsp_ptr->status = LOG_ON_DEMAND_ACKNOWLEDGE_S;

  if (log_on_demand_reg_count != 0)
    {
     for( i=0; i<LOG_ON_DEMAND_SIZE; i++ )
     {
      if ( ( log_on_demand_list[i].log_on_demand_funct_ptr.ptr != NULL ) && (log_on_demand_list[i].log_code == req_ptr->log_code) )
        {
            rsp_ptr->status=(*log_on_demand_list[i].log_on_demand_funct_ptr.ptr)
              (log_on_demand_list[i].log_code);
            break;
        }
     }
    }

#if defined(DIAG_MP_MASTER)
  // Log on demand should be responded by Modem if feature mask bit (2 for log on demand)
  //   i.e. log on demand bit is not set by modem.
  //   Since send_rsp is init to TRUE above, following condition says that since the bit is set on modem APPS should respond

    if ( !DIAG_IS_INT_FEATURE_BIT_SET(diagcomm_io_ctrl_conn[DIAGCOMM_PORT_SMD][DIAGCOMM_PORT_1].feature_mask,(diag_feature_query_enum_type)F_DIAG_LOG_ON_DEMAND_RSP_ON_MASTER))
#else
  // This will execute on modem, or other peripherals. It will work for 'modem only' configuration as feature mask will not be set
    if ( DIAG_IS_INT_FEATURE_BIT_SET(diagcomm_io_ctrl_conn[DIAGCOMM_PORT_SIO][DIAGCOMM_PORT_1].feature_mask,(diag_feature_query_enum_type)F_DIAG_LOG_ON_DEMAND_RSP_ON_MASTER))
#endif
  {
      send_rsp = FALSE; // if this execute it says the bit is not set on modem, modem should respond, APPS should not respond.
  }

  // if diag is not connected, don't send response
  if (!diagcomm_cmd_status(DIAGCOMM_PORT_1))
  {
    if(!diagcomm_apps_only_config)
    send_rsp = FALSE;
  }

  // return response
  if (!send_rsp)
  {
    diagpkt_free(rsp_ptr);
    return NULL;
  }
  else
  {
    return ((void *) rsp_ptr);
  }

}  /*log_on_demand_pkt */


/*===========================================================================

FUNCTION LOG_PROCESS_CONFIG_PKT

DESCRIPTION
  This procedure processes a request to set the log mask.

  Once again the log service config packet has evolved.  This time it has
  evolved to be able to handle multiple equipment IDs.  In the past, the MSM
  always assumed the equipment ID is 1.  Multi-mode products, different
  technologies can log with the same service using equipment ID to specify the
  technology.  This means we have 16 masks, one for each equipment ID.

  The extended logmask packet was designed using a very odd bit ordering scheme
  that created a lot of confusion.  To alleviate this confusion, the logmask is
  now a byte array, with each bit being ordered LSB to MSB.  Macros are defined
  to perform these operations.

PARAMETERS
  req_pkt_ptr - Pointer to request packet
  pkt_len - Length of request packet

RETURN VALUE
  Pointer to response packet.

  This command effects what type of entries are logged.
===========================================================================*/
void * log_process_config_pkt (
  void * req_pkt_ptr,
  uint16 pkt_len
)
{
  /* For cmd DIAG_LOG_CONFIG_F 115 - Supports Stream 1 */
  DIAG_LOG_CONFIG_F_req_type *req_ptr = (DIAG_LOG_CONFIG_F_req_type *) req_pkt_ptr;
  DIAG_LOG_CONFIG_F_rsp_type *rsp_ptr = NULL;
  /* For Diag subsys cmd DIAG_EXT_LOG_CONFIG 75 18 98 - Supports all streams */
  log_ext_config_req_type *req_dm_ptr = NULL;
  log_ext_config_rsp_type *rsp_dm_ptr = NULL;

  /* Pointers to unions for operation specific data (For non-dual-mask Packets)*/
  log_config_op_req_type *req_op_ptr = NULL;
  log_config_op_rsp_type *rsp_op_ptr = NULL;

  /* Local working variables */
  unsigned int rsp_len, operation, last_item, equip_id = 0;
  uint16 i;
  byte stream_id = 1;
  int dual_mask;
  byte *mask_rd_ptr = NULL;
  byte *mask_wr_ptr = NULL;
  boolean send_rsp = TRUE;
  log_config_status_enum_type status = LOG_CONFIG_SUCCESS_S;

  /* Sanity checks */
  if( (req_pkt_ptr == NULL) || (pkt_len == 0) )
  {
    return NULL;
  }

  dual_mask = (req_ptr->xx_header.opaque_header == DIAG_LOG_CONFIG_F)? 0:1;

  if( dual_mask )
  {
    req_dm_ptr = (log_ext_config_req_type *) req_pkt_ptr;

    if( req_dm_ptr->header.cmd_version == 2 )
    {
      return diaglog_process_ext_config_req( req_pkt_ptr, pkt_len ); //ToDo: separate function for now
    }
    else if( req_dm_ptr->header.cmd_version != 1 || req_dm_ptr->header.opcode == LOG_CONFIG_RETRIEVE_VALID_MASK_OP )
    {
      return diagpkt_err_rsp(DIAG_BAD_PARM_F, req_dm_ptr, pkt_len);
    }
  }
  else
  {
    req_op_ptr = (log_config_op_req_type *) req_ptr->op_data;
  }

  rsp_len = FPOS(DIAG_LOG_CONFIG_F_rsp_type, op_data);
  operation = (dual_mask)? req_dm_ptr->header.opcode : req_ptr->operation;

  /* First, determine the length of the response.
   * Second, allocate the response.
   * Last, fill in the response. */

  /* This switch statement computes rsp_len for the operation */
  switch (operation)
  {
    case LOG_CONFIG_DISABLE_OP:
      rsp_len = dual_mask ? sizeof(log_ext_config_disable_op_rsp): rsp_len;
      if(dual_mask) {
        // Stream ID 0 is acceptable in this case (to disable all the streams).
          if(req_dm_ptr->disable_op.id.stream_id > DIAG_MAX_STREAM_ID) {
          status = LOG_CONFIG_INVALID_STREAM_ID;
          return diagpkt_err_rsp(DIAG_BAD_PARM_F, req_dm_ptr, pkt_len);
          }
        }
      /* No response op data */
      break;

    case LOG_CONFIG_RETRIEVE_ID_RANGES_OP:
      rsp_len = dual_mask ? sizeof(log_ext_config_id_ranges_rsp) : (rsp_len + sizeof(log_config_ranges_rsp_type));
      break;

    case LOG_CONFIG_RETRIEVE_VALID_MASK_OP:
      equip_id = req_op_ptr->valid_mask.equip_id;
      if (equip_id > LOG_EQUIP_ID_MAX) {
        status = LOG_CONFIG_INVALID_EQUIP_ID_S;
        return diagpkt_err_rsp(DIAG_BAD_PARM_F, req_ptr, pkt_len);
      }
      rsp_len += FPOS(log_config_valid_mask_rsp_type, mask) +
        (LOG_MASK_ARRAY_INDEX(log_last_item_tbl[equip_id]) + 1) * sizeof(byte);
      break;

    case LOG_CONFIG_SET_MASK_OP:
        equip_id = dual_mask? req_dm_ptr->set_mask.code_range.equip_id : req_op_ptr->set_mask.code_range.equip_id;
      if (equip_id > LOG_EQUIP_ID_MAX) {
        status = LOG_CONFIG_INVALID_EQUIP_ID_S;
          return diagpkt_err_rsp(DIAG_BAD_PARM_F, req_pkt_ptr, pkt_len);
        }

        if(dual_mask) {
          if(req_dm_ptr->set_mask.id.stream_id < 1 || req_dm_ptr->set_mask.id.stream_id > DIAG_MAX_STREAM_ID) {
          status = LOG_CONFIG_INVALID_STREAM_ID;
          return diagpkt_err_rsp(DIAG_BAD_PARM_F, req_dm_ptr, pkt_len);
          }
      }
      rsp_len =  (dual_mask ? FPOS(log_ext_config_set_mask_rsp, mask) : (rsp_len + FPOS(log_config_valid_mask_rsp_type, mask))) +
        (LOG_MASK_ARRAY_INDEX(log_last_item_tbl[equip_id]) + 1) * sizeof(byte);

      break;

    case LOG_CONFIG_GET_LOGMASK_OP:
      equip_id = dual_mask ? req_dm_ptr->get_mask.equip_id : req_op_ptr->valid_mask.equip_id;
      if (equip_id > LOG_EQUIP_ID_MAX) {
        status = LOG_CONFIG_INVALID_EQUIP_ID_S;
        return diagpkt_err_rsp(DIAG_BAD_PARM_F, req_pkt_ptr, pkt_len);
      }
      if(dual_mask) {
        if(req_dm_ptr->set_mask.id.stream_id < 1 || req_dm_ptr->set_mask.id.stream_id > DIAG_MAX_STREAM_ID) {
          status = LOG_CONFIG_INVALID_STREAM_ID;
          return diagpkt_err_rsp(DIAG_BAD_PARM_F, req_dm_ptr, pkt_len);
        }
      }
      rsp_len = (dual_mask ? FPOS(log_ext_config_get_mask_rsp, mask):(rsp_len + FPOS(log_config_valid_mask_rsp_type, mask))) +
         (LOG_MASK_ARRAY_INDEX(log_last_item_tbl[equip_id]) + 1) * sizeof(byte);

      break;

    default:
      return diagpkt_err_rsp(DIAG_BAD_PARM_F,  req_pkt_ptr, pkt_len);
  } /* end switch for rsp_len computation */


  rsp_ptr = dual_mask ? rsp_ptr : (DIAG_LOG_CONFIG_F_rsp_type *) diagpkt_alloc(DIAG_LOG_CONFIG_F, rsp_len);
  rsp_dm_ptr = dual_mask ? (log_ext_config_rsp_type *) diagpkt_subsys_alloc (DIAG_SUBSYS_DIAG_SERV, DIAG_EXT_LOG_CONFIG, rsp_len) : rsp_dm_ptr;


  /* For cmd DIAG_LOG_CONFIG_F 115 - Supports Stream 1 */
  if( !dual_mask ) {
    if(rsp_ptr == NULL)
    {
      return rsp_ptr;
    }

    rsp_ptr->pad[0] = 0;
    rsp_ptr->pad[1] = 0;
    rsp_ptr->pad[2] = 0;

    rsp_ptr->operation = operation;

    rsp_op_ptr = (log_config_op_rsp_type *) rsp_ptr->op_data;

  /* This switch statement actually performs the operation */
  switch (operation) {
    case LOG_CONFIG_DISABLE_OP:
           (void) log_config_mask(-1, FALSE, DIAG_STREAM_1, diag_cur_preset_id);
          log_config_mstr_mask(-1, FALSE, DIAG_STREAM_1, diag_cur_preset_id);
      break;

      case LOG_CONFIG_RETRIEVE_ID_RANGES_OP:
        for (i = 0; i <= LOG_EQUIP_ID_MAX; i++) {
          rsp_op_ptr->ranges.last_item[i] = (uint32) log_last_item_tbl[i];
        }
        break;

    case LOG_CONFIG_RETRIEVE_VALID_MASK_OP:
      rsp_op_ptr->valid_mask.code_range.equip_id = equip_id;
      rsp_op_ptr->valid_mask.code_range.last_item = log_last_item_tbl[equip_id];

      if (equip_id > LOG_EQUIP_ID_LAST_DEFAULT)
      {
        return diagpkt_err_rsp(DIAG_BAD_PARM_F, req_ptr, pkt_len);
      }

      mask_rd_ptr = (byte *) &log_valid_mask[log_mask_offset_tbl[equip_id]];
      mask_wr_ptr = (byte *) rsp_op_ptr->valid_mask.mask;

      for (i = 0; i <= (unsigned int)
          LOG_MASK_ARRAY_INDEX(log_last_item_tbl[equip_id]); i++) {

          *mask_wr_ptr = *mask_rd_ptr;
          mask_wr_ptr++;
          mask_rd_ptr++;
        }
        break;

    case LOG_CONFIG_SET_MASK_OP:
      rsp_op_ptr->set_mask.code_range.equip_id = equip_id;
      rsp_op_ptr->set_mask.code_range.last_item = log_last_item_tbl[equip_id];

      if (equip_id > LOG_EQUIP_ID_LAST_DEFAULT)
      {
        return diagpkt_err_rsp(DIAG_BAD_PARM_F, req_ptr, pkt_len);
      }

#if defined(DIAG_MP_MASTER)
      /* Save master copy of log mask */
      mask_rd_ptr = (byte *) req_op_ptr->set_mask.mask;
      mask_wr_ptr = diag_log_mask_mstr[DIAG_STREAM_1-1]->mask[equip_id]; //&log_mask_mstr[DIAG_STREAM_1-1][equip_id][0];
      last_item = MIN(MSTR_LOG_MAX_LAST_ITEM, req_op_ptr->set_mask.code_range.last_item);
      diag_log_last_item_mstr_tbl[equip_id] = last_item;
      diag_log_mask_mstr[DIAG_STREAM_1-1]->status = DIAG_CTRL_MASK_VALID;

      for (i = 0; i <= LOG_MASK_ARRAY_INDEX(last_item); i++) {
        *mask_wr_ptr = *mask_rd_ptr;
        mask_wr_ptr++;
        mask_rd_ptr++;
      }
      /* Zero all bits that are after the last item in the set mask */
      mask_wr_ptr--;
      i--;

      /* (The next boundary of 8) - 1 - (last_item) gives the number of bits
       * at the end of the last byte that are not specified.
       * This line 0's out the bits that remain in the mask that are not
       * included in last_item. */
/*lint -save -e504 Unusual shift operation (unusually formed right argument)*/
/*lint -save -e817 */
      *mask_wr_ptr &= 0xFF >> (((last_item + 8) & ~7) - 1 - last_item);

      /* Master needs to keep track to send log masks to Slaves */
      for( i=0; i < NUM_SMD_PORTS; i++ )
      {
        diagcomm_io_ctrl_conn[DIAGCOMM_PORT_SMD][i].log_mask_update[0][equip_id] = TRUE;
      }
#endif

      /* Set the log mask */
      mask_rd_ptr = (byte *) req_op_ptr->set_mask.mask;
      mask_wr_ptr = &log_mask[0][log_mask_offset_tbl[equip_id]];
      last_item = MIN(log_last_item_tbl[equip_id], req_op_ptr->set_mask.code_range.last_item);

      for (i = 0; i <= LOG_MASK_ARRAY_INDEX(last_item); i++) {
        *mask_wr_ptr = *mask_rd_ptr;
         rsp_op_ptr->set_mask.mask[i] = *mask_rd_ptr;
        mask_wr_ptr++;
        mask_rd_ptr++;
      }

        /* Zero all bits that are after the last item in the set mask */
        mask_wr_ptr--;
        i--;

      /* (The next boundary of 8) - 1 - (last_item) gives the number of bits
       * at the end of the last byte that are not specified.
       * This line 0's out the bits that remain in the mask that are not
       * included in last_item. */
/*lint -save -e504 Unusual shift operation (unusually formed right argument)*/
/*lint -save -e817 */
        *mask_wr_ptr &= 0xFF >> (((last_item + 8) & ~7) - 1 - last_item);
        rsp_op_ptr->set_mask.mask[i] = *mask_wr_ptr;

        /* Zero out all items that were not specified in the request. */
        for (i = last_item + 1; i <= log_last_item_tbl[equip_id]; i++) {
              (void) log_config_mask(log_set_equip_id(i, equip_id), FALSE, DIAG_STREAM_1, diag_cur_preset_id);
        }

      for( i = LOG_MASK_ARRAY_INDEX(last_item) + 1; i <=
           LOG_MASK_ARRAY_INDEX(log_last_item_tbl[equip_id]); i++)
      {
         rsp_op_ptr->set_mask.mask[i] = 0;
      }

      /* Update composite mask */
      log_update_composite_mask(log_mask_offset_tbl[equip_id],
                                log_mask_offset_tbl[equip_id] + LOG_MASK_ARRAY_INDEX(last_item));
      break;

  case LOG_CONFIG_GET_LOGMASK_OP:
      rsp_op_ptr->get_mask.code_range.equip_id = equip_id;
      rsp_op_ptr->get_mask.code_range.last_item = log_last_item_tbl[equip_id];

      if (equip_id > LOG_EQUIP_ID_LAST_DEFAULT)
      {
        return diagpkt_err_rsp(DIAG_BAD_PARM_F, req_ptr, pkt_len);
      }
      mask_rd_ptr = (byte *) &log_mask[0][log_mask_offset_tbl[equip_id]];
      mask_wr_ptr = (byte *) rsp_op_ptr->valid_mask.mask;

      for (i = 0; i <= (unsigned int)
          LOG_MASK_ARRAY_INDEX(log_last_item_tbl[equip_id]); i++) {

          *mask_wr_ptr = *mask_rd_ptr;
          mask_wr_ptr++;
          mask_rd_ptr++;

        }
        break;
    default:
        break;

    } /* end switch */


    if((LOG_CONFIG_SUCCESS_S == status) &&
      (LOG_CONFIG_SET_MASK_OP == operation))
    {
       (void)diag_client_maskchange(DIAG_LOGSVC_MASK_CHANGE);
    }

    /* Config may have changed.  Run sleep vote routine. */
    (void) log_sleep_vote ();

    rsp_ptr->status = status;

    /* if master processor on multi processor build, don't send response */
    #if !defined(DIAG_RSP_SEND)
     if (!diagcomm_apps_only_config)
      send_rsp = FALSE;
    #endif

    #if defined(DIAG_MP_MASTER)
    if( (operation == LOG_CONFIG_SET_MASK_OP) || (operation == LOG_CONFIG_DISABLE_OP) )
    {
      /* MP Master now responds to "set mask" requests */
      send_rsp = TRUE;
    }
      else if( (operation == LOG_CONFIG_RETRIEVE_ID_RANGES_OP) ||
             (operation == LOG_CONFIG_RETRIEVE_VALID_MASK_OP) ||
             (operation == LOG_CONFIG_GET_LOGMASK_OP) )
      {
        /* If we're in buffering mode, Master needs to send out the responses.
         If Modem is not up, Master needs to send out responses as well. */
        send_rsp = TRUE;
      }
    #endif

  /* if diag is not connected, don't send response */
    if (!diagcomm_cmd_status(DIAGCOMM_PORT_1))
    {
      send_rsp = FALSE;
    }

  /* return response */
    if (!send_rsp)
    {
      diagpkt_free(rsp_ptr);
      return NULL;
    }
    else
    {
      return ((void *) rsp_ptr);
    }
  }
  else {
    /* Dual mask */
    if(req_dm_ptr == NULL || rsp_dm_ptr == NULL) {
      return NULL;
    }
    rsp_dm_ptr->header = req_dm_ptr->header;
    switch (operation) {
        case LOG_CONFIG_DISABLE_OP:
          rsp_dm_ptr->disable_op.status = status;
          if ((rsp_dm_ptr->disable_op.id.stream_id = req_dm_ptr->disable_op.id.stream_id) == 0) {
            // Disable all streams
            (void) log_config_mask(-1, FALSE, DIAG_STREAM_ALL, diag_cur_preset_id);
            log_config_mstr_mask(-1, FALSE, DIAG_STREAM_ALL, diag_cur_preset_id);
          }
          else {
                stream_id = req_dm_ptr->disable_op.id.stream_id;
                // Disable a specific stream given by stream_id
                (void) log_config_mask (-1, FALSE, stream_id, diag_cur_preset_id);
                log_config_mstr_mask(-1, FALSE, stream_id, diag_cur_preset_id);
          }
          break;

        case LOG_CONFIG_RETRIEVE_ID_RANGES_OP:
          rsp_dm_ptr->id_ranges.status = status;
          rsp_dm_ptr->id_ranges.rsvd = 0;
          for (i = 0; i <= LOG_EQUIP_ID_MAX; i++) {
            rsp_dm_ptr->id_ranges.last_item[i] = (uint32) log_last_item_tbl[i];
          }
          break;

        case LOG_CONFIG_SET_MASK_OP:
          rsp_dm_ptr->set_mask.id.stream_id = stream_id = req_dm_ptr->set_mask.id.stream_id;
          rsp_dm_ptr->set_mask.status = status;
          rsp_dm_ptr->set_mask.code_range.equip_id = equip_id;
          rsp_dm_ptr->set_mask.code_range.last_item = log_last_item_tbl[equip_id];

          if (equip_id > LOG_EQUIP_ID_LAST_DEFAULT)
          {
            return diagpkt_err_rsp(DIAG_BAD_PARM_F, req_dm_ptr, pkt_len);
          }

          mask_rd_ptr = (byte *) req_dm_ptr->set_mask.mask;
          mask_wr_ptr = &log_mask[req_dm_ptr->set_mask.id.stream_id - 1][log_mask_offset_tbl[equip_id]];
          last_item = MIN(log_last_item_tbl[equip_id], req_dm_ptr->set_mask.code_range.last_item);

          for (i = 0; i <= LOG_MASK_ARRAY_INDEX(last_item); i++) {
            *mask_wr_ptr = *mask_rd_ptr;
            rsp_dm_ptr->set_mask.mask[i] = *mask_rd_ptr;

            mask_wr_ptr++;
            mask_rd_ptr++;
          }
          /* Zero all bits that are after the last item in the set mask */
          mask_wr_ptr--;
          i--;

          /* (The next boundary of 8) - 1 - (last_item) gives the number of bits
           * at the end of the last byte that are not specified.
           * This line 0's out the bits that remain in the mask that are not
           * included in last_item. */
    /*lint -save -e504 Unusual shift operation (unusually formed right argument)*/
    /*lint -save -e817 */
          *mask_wr_ptr &= 0xFF >> (((last_item + 8) & ~7) - 1 - last_item);
          rsp_dm_ptr->set_mask.mask[i] = *mask_wr_ptr;

          /* Zero out all items that were not spefcified in the request. */
          for (i = last_item + 1; i <= log_last_item_tbl[equip_id]; i++) {
            (void) log_config_mask(log_set_equip_id(i, equip_id), FALSE, stream_id, diag_cur_preset_id);
          }

          for( i = LOG_MASK_ARRAY_INDEX(last_item) + 1; i <=
               LOG_MASK_ARRAY_INDEX(log_last_item_tbl[equip_id]); i++)
          {
             rsp_dm_ptr->set_mask.mask[i] = 0;
          }

          /* Update composite mask */
          log_update_composite_mask(log_mask_offset_tbl[equip_id],
                                    log_mask_offset_tbl[equip_id] + LOG_MASK_ARRAY_INDEX(last_item));
          break;

        case LOG_CONFIG_GET_LOGMASK_OP:
          rsp_dm_ptr->get_mask.id.stream_id = req_dm_ptr->get_mask.id.stream_id;
          rsp_dm_ptr->get_mask.status = status;
          rsp_dm_ptr->get_mask.code_range.equip_id = equip_id;
          rsp_dm_ptr->get_mask.code_range.last_item = log_last_item_tbl[equip_id];

          if (equip_id > LOG_EQUIP_ID_LAST_DEFAULT)
          {
            return diagpkt_err_rsp(DIAG_BAD_PARM_F, req_dm_ptr, pkt_len);
          }
          mask_rd_ptr = (byte *) &log_mask[req_dm_ptr->get_mask.id.stream_id - 1][log_mask_offset_tbl[equip_id]];
          mask_wr_ptr = (byte *) rsp_dm_ptr->get_mask.mask;

          for (i = 0; i <= (unsigned int)
              LOG_MASK_ARRAY_INDEX(log_last_item_tbl[equip_id]); i++) {

            *mask_wr_ptr = *mask_rd_ptr;
            mask_wr_ptr++;
            mask_rd_ptr++;

          }
          break;
      default:
          break;

      } /* end switch */

    if((LOG_CONFIG_SUCCESS_S == status) &&
      (LOG_CONFIG_SET_MASK_OP == operation))
    {
       (void)diag_client_maskchange(DIAG_LOGSVC_MASK_CHANGE);
    }

    /* Config may have changed.  Run sleep vote routine. */
    (void) log_sleep_vote ();

    /* if master processor on multi processor build, don't send response */
    #if !defined(DIAG_RSP_SEND)
     if (!diagcomm_apps_only_config)
      send_rsp = FALSE;
    #endif

  /* if diag is not connected, don't send response */
    if (!diagcomm_cmd_status(DIAGCOMM_PORT_1))
    {
      send_rsp = FALSE;
    }

  /* return response */
    if (!send_rsp)
    {
      diagpkt_free(rsp_dm_ptr);
      return NULL;
    }
    else
    {
      return ((void *) rsp_dm_ptr);
    }
  }
} /* log_process_config_pkt */


#endif

