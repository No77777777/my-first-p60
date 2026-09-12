#if !defined(DIAG_MP)

/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                            Event Service

General Description
  This file contains the implementation for the event mask handlers in standalone configuration.

Copyright (c) 2018-2019 by QUALCOMM Technologies, Incorporated.
All Rights Reserved.
Qualcomm Confidential and Proprietary

*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*==========================================================================

                              Edit History

$Header: //components/rel/core.mpss/10.0/services/diag/DCM/common/src/event_standalone.c#2 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
07/04/18    gn     Created new file for standalone functionality of events

==========================================================================*/

#include "event.h"
#include "eventi.h" 
#include "diagdiag_v.h"
#include "diagi_v.h"
#include "diagcomm_cmd.h" /* For diagcomm_cmd_status */
#include "stringl.h" /* For memscpy */

extern unsigned char * diag_event_mask[DIAG_MAX_STREAM_ID];
extern uint8 event_preset_status[DIAG_MAX_PRESET_ID];
extern uint8 diag_cur_preset_id;
extern unsigned char diag_event_preset_mask[DIAG_MAX_PRESET_ID][EVENT_MASK_SIZE];

/*===========================================================================

FUNCTION EVENT_INBOUND_PKT

DESCRIPTION
  This is the packet handler for the event service configuration packet.

===========================================================================*/
void *
event_inbound_pkt (void * req_pkt, uint16 pkt_len)
{
  event_cfg_req_type *req = (event_cfg_req_type *) req_pkt;
  event_cfg_rsp_type *rsp = NULL;
  const unsigned int rsp_len = sizeof (event_cfg_rsp_type);
  uint8 config;
  uint8 set_mask;
  int i =0;
  boolean send_rsp = TRUE;

  (void) pkt_len; /* suppress compiler warning */

  if(req)
  {
    config = (req->enable) ? EVENT_CONFIG_COMM_MASK : 0;

    event_control (config, EVENT_CONFIG_COMM_MASK);

    /* if master processor on multi processor build, don't send response */
    #if !defined(DIAG_RSP_SEND)
    if(!diagcomm_apps_only_config)
      send_rsp = FALSE;
    #endif

    #if defined(DIAG_MP_MASTER)
    /* MP Master now responds to "set mask" requests */
    send_rsp = TRUE;
    #endif

   /* if diag is not connected, don't send response */
    if (!diagcomm_cmd_status(DIAGCOMM_PORT_1))
    {
      send_rsp = FALSE;
    }

    if(send_rsp)
    {
      rsp = (event_cfg_rsp_type *) diagpkt_alloc (DIAG_EVENT_REPORT_F, rsp_len);

      if(rsp)
      {
        rsp->length = 0;
      }
    }

    set_mask = (config) ? 0xFF : 0;

    /* Set all the complete bytes to set_mask. Then set the
    ** bits in the last incomplete byte, if there is one.
    */
    for(i = 0; i < EVENT_MASK_SIZE; i++)
      diag_event_mask[DIAG_STREAM_1-1][i] = set_mask;

#if defined(DIAG_MP_MASTER)
    /* Save master event mask */
    for( i = 0; i < MSTR_EVENT_MASK_SIZE; i++ )
      diag_event_mask_mstr[DIAG_STREAM_1-1]->mask[i] = set_mask;

    if( set_mask )
      diag_event_mask_mstr[DIAG_STREAM_1-1]->status = DIAG_CTRL_MASK_ALL_ENABLED;
    else
      diag_event_mask_mstr[DIAG_STREAM_1-1]->status = DIAG_CTRL_MASK_ALL_DISABLED;
#else
    if( set_mask )
      event_preset_status[diag_cur_preset_id-1] = DIAG_CTRL_MASK_ALL_ENABLED;
    else
      event_preset_status[diag_cur_preset_id-1] = DIAG_CTRL_MASK_ALL_DISABLED;
#endif
  }

  (void)diag_client_maskchange(DIAG_EVENTSVC_MASK_CHANGE); /* MAD: TODO: send an err response if this fails? */

  return((void *) rsp);

} /* event_inbound_pkt */


/*===========================================================================

FUNCTION EVENT_MASK_GET_HANDLER
DESCRIPTION
  Handles the EVENT_MASK_GET packet. Returns the current event mask.
============================================================================*/
void *
event_mask_get_handler (void * req_ptr, uint16 pkt_len)
{
  event_mask_get_rsp_type *mask_get_rsp_ptr = NULL;
  dword rsp_len = FPOS(event_mask_get_rsp_type,mask);
  boolean send_rsp = TRUE;

  (void) req_ptr; /* suppress compiler warning */
  (void) pkt_len; /* suppress compiler warning */

  /* if master processor on multi processor build, don't send response */
  #if !defined(DIAG_RSP_SEND)
  if(!diagcomm_apps_only_config)
    send_rsp = FALSE;
  #endif

  #if defined(DIAG_MP_MASTER)
    /* For "get mask" requests, Master Diag needs to respond if we're in buffering mode
       or if Modem is not up yet. This response sending logic will be done in diagpkt_process_request(). */
    send_rsp = TRUE;
  #endif

/* if diag is not connected, don't send response */
  if (!diagcomm_cmd_status(DIAGCOMM_PORT_1))
  {
    send_rsp = FALSE;
  }

  if(send_rsp)
  {
    rsp_len += EVENT_MASK_SIZE;

    mask_get_rsp_ptr = (event_mask_get_rsp_type *)
                       diagpkt_alloc (DIAG_EVENT_MASK_GET_F, rsp_len);
    if(mask_get_rsp_ptr == NULL)
      return NULL;

    mask_get_rsp_ptr->error_code = 0;
    mask_get_rsp_ptr->reserved = 0;
    mask_get_rsp_ptr->numbits = EVENT_LAST_ID + 1;

  memscpy (mask_get_rsp_ptr->mask,EVENT_MASK_SIZE, diag_event_mask[DIAG_STREAM_1-1], EVENT_MASK_SIZE);
  }

  return((void *) mask_get_rsp_ptr);

} /* END event_mask_get_handler */


/*===========================================================================

FUNCTION EVENT_MASK_SET_HANDLER

DESCRIPTION
  Handles the EVENT_MASK_SET packet. Sets the event mask.
============================================================================*/
void *
event_mask_set_handler (void * req_ptr, uint16 pkt_len)
{
  event_mask_set_req_type *mask_set_req_ptr;
  event_mask_set_rsp_type *mask_set_rsp_ptr = NULL;
  dword rsp_len = FPOS(event_mask_set_rsp_type,mask);
  unsigned int mask_len = (unsigned int) pkt_len -
                          FPOS (event_mask_set_req_type, mask);
  boolean send_rsp = TRUE;
  uint16 numbits = 0;
  uint32 rsp_mask_len = 0;
  unsigned char * rsp_mask = NULL;

  mask_set_req_ptr = (event_mask_set_req_type *) req_ptr;

  /* if master processor on multi processor build, don't send response */
  #if !defined(DIAG_RSP_SEND)
  if(!diagcomm_apps_only_config)
    send_rsp = FALSE;
  #endif

  #if defined(DIAG_MP_MASTER)
  /* MP Master now responds to "set mask" requests */
  send_rsp = TRUE;
  #endif

  /* if diag is not connected, don't send response */
  if (!diagcomm_cmd_status(DIAGCOMM_PORT_1))
  {
    send_rsp = FALSE;
  }

  event_mask_set_internal_handler( DIAG_OP_SET_EVENT_STREAM, DIAG_STREAM_1, mask_set_req_ptr->numbits,
                                   mask_len, (unsigned char *)mask_set_req_ptr->mask );

  if(send_rsp)
  {
    #if defined(DIAG_MP_MASTER)
      rsp_mask_len = diag_event_mask_mstr_preset[diag_cur_preset_id-1].mask_len;
      numbits = diag_event_mask_mstr_preset[diag_cur_preset_id-1].numbits;
      rsp_mask = diag_event_mask_mstr_preset[diag_cur_preset_id-1].mask;
    #else
      rsp_mask_len = EVENT_MASK_SIZE;
      numbits = EVENT_LAST_ID + 1;
      rsp_mask = diag_event_mask[DIAG_STREAM_1-1];
    #endif

    rsp_len += rsp_mask_len;
    mask_set_rsp_ptr = (event_mask_set_rsp_type *)
                       diagpkt_alloc (DIAG_EVENT_MASK_SET_F, rsp_len);
    if(mask_set_rsp_ptr == NULL)
      return NULL;

    mask_set_rsp_ptr->error_code = 0;
    mask_set_rsp_ptr->reserved = 0;
    mask_set_rsp_ptr->numbits = numbits;

    memscpy( mask_set_rsp_ptr->mask,EVENT_MASK_SIZE, rsp_mask, rsp_mask_len );
  }

  (void)diag_client_maskchange(DIAG_EVENTSVC_MASK_CHANGE); /* TODO: send an err response if this fails */

  return((void *) mask_set_rsp_ptr);

} /* event_mask_set_handler */


/*===========================================================================

FUNCTION DIAGDIAG_EXT_EVENT_REPORT_CTRL

DESCRIPTION
  This procedure handles the extended event report control subsystem command.

RETURN VALUE
  Pointer to response packet.

============================================================================*/
void * diagdiag_ext_event_report_ctrl(void * req_pkt,
                                            uint16 pkt_len)
{
  event_ext_rpt_ctrl_type *req = (event_ext_rpt_ctrl_type *)req_pkt;
  byte config = 0;
  dword rsp_len = 0;
  boolean send_rsp = TRUE;
  boolean send_err = FALSE;

  /* if master processor on multi processor build, don't send response */
  #if !defined(DIAG_RSP_SEND)
  if(!diagcomm_apps_only_config)
    send_rsp = FALSE;
  #endif

  #if defined(DIAG_MP_MASTER)
    /* MP Master now responds to "set mask" requests.
       For "get mask" requests, Master Diag needs to respond if we're in buffering mode
       or if Modem is not up yet. This response sending logic will be done in diagpkt_process_request(). */
    send_rsp = TRUE;
  #endif

  /* if diag is not connected, don't send response */
  if (!diagcomm_cmd_status(DIAGCOMM_PORT_1))
  {
    send_rsp = FALSE;
  }

  /* Sanity checks */
  if( (req_pkt == NULL) || (pkt_len < sizeof(event_ext_rpt_ctrl_type)) )
  {
    if( send_rsp )
      return diagpkt_err_rsp(DIAG_BAD_PARM_F, req_pkt, pkt_len);
    else
      return NULL;
  }

  switch( req->cmd_version )
  {
    case 1:
    {
      if( (req->id.stream_id > DIAG_MAX_STREAM_ID) ||
          (req->id.stream_id == 0 && req->subcmd != DIAG_EXT_EVENT_REPORT_SET_ALL) )
      {
        /* For command version 1, if stream_id is out of range, send Bad Param error response. */
        send_err = TRUE;
      }
      break;
    }
    case 2:
    {
      if( (req->id.preset_id > DIAG_MAX_PRESET_ID) ||
          (req->id.preset_id == 0 && req->subcmd != DIAG_EXT_EVENT_REPORT_SET_ALL) )
      {
        /* For command version 2, if preset_id is out of range, send Bad Param error response. */
        send_err = TRUE;
      }
      break;
    }
    default:
    {
      /* Send Bad Param error response for unsupported command version. */
      send_err = TRUE;
      break;
    }
  }

  if( send_err == TRUE )
  {
    if( send_rsp )
      return diagpkt_err_rsp(DIAG_BAD_PARM_F, req_pkt, pkt_len);
    else
      return NULL;
  }

  MSG_3(MSG_SSID_DIAG, MSG_LEGACY_LOW, "ext_event_report_ctrl subcmd=%d cmd_ver=%d id=%d",
        req->subcmd, req->cmd_version, req->id.stream_id);

   if(req->subcmd == DIAG_EXT_EVENT_REPORT_GET_MASK) // Get Event Mask
   {
    event_ext_rpt_ctrl_rsp_type *rsp=NULL;
    uint16 numbits = 0;
    uint32 mask_len = 0;
    unsigned char * rsp_mask = NULL;

    if( send_rsp )
    {
      rsp_len = FPOS(event_ext_rpt_ctrl_rsp_type,mask);

      if( req->cmd_version == 1 )
      {
        #if defined(DIAG_MP_MASTER)
          mask_len = diag_event_mask_mstr[req->id.stream_id-1]->mask_len;
          numbits = diag_event_mask_mstr[req->id.stream_id-1]->numbits;
          rsp_mask = diag_event_mask_mstr[req->id.stream_id-1]->mask;
        #else
          mask_len = EVENT_MASK_SIZE;
          numbits = EVENT_LAST_ID + 1;
          rsp_mask = diag_event_mask[req->id.stream_id-1];
        #endif

        rsp_len += mask_len;
        rsp = (event_ext_rpt_ctrl_rsp_type *) diagpkt_subsys_alloc (DIAG_SUBSYS_DIAG_SERV, req->subsys_cmd_code, rsp_len);
        if(rsp == NULL)
          return NULL;

        rsp->header.id.stream_id = req->id.stream_id;
        memscpy( rsp->mask, mask_len, rsp_mask, mask_len );
      }
      else if( req->cmd_version == 2 )
      {
        #if defined(DIAG_MP_MASTER)
          mask_len = diag_event_mask_mstr_preset[req->id.preset_id-1].mask_len;
          numbits = diag_event_mask_mstr_preset[req->id.preset_id-1].numbits;
          rsp_mask = diag_event_mask_mstr_preset[req->id.preset_id-1].mask;
        #else
          mask_len += EVENT_MASK_SIZE;
          numbits = EVENT_LAST_ID + 1;
          rsp_mask = diag_event_preset_mask[req->id.preset_id-1];
        #endif

        rsp_len += mask_len;
        rsp = (event_ext_rpt_ctrl_rsp_type *) diagpkt_subsys_alloc (DIAG_SUBSYS_DIAG_SERV, req->subsys_cmd_code, rsp_len);
        if(rsp == NULL)
          return NULL;

        rsp->header.id.preset_id = req->id.preset_id;
        memscpy( rsp->mask,mask_len, rsp_mask, mask_len );
      }

      // diagpkt_subsys_alloc copies the subsys_id and subsys_cmd_code
      rsp->header.cmd_code = req->cmd_code;
      rsp->header.cmd_version = req->cmd_version;
      rsp->header.subcmd = req->subcmd;
      rsp->error_code = 0;
      rsp->numbits = numbits;

    }
    return (void *)rsp;
  }

  else if(req->subcmd == DIAG_EXT_EVENT_REPORT_SET_MASK) // Set Event Mask
  {
    unsigned int mask_len;
    uint16 numbits = 0;
    unsigned char * rsp_mask = NULL;
    event_ext_rpt_ctrl_rsp_type *rsp=NULL;
    event_ext_rpt_ctrl_set_mask_req_type *req_set = (event_ext_rpt_ctrl_set_mask_req_type *)req_pkt;

    if (pkt_len <= FPOS(event_ext_rpt_ctrl_set_mask_req_type, mask))
    {
      return diagpkt_err_rsp(DIAG_BAD_LEN_F, req_pkt, pkt_len);
    }
    
    /* Get the number of bits to set from the request. Byte offset of numbits field is 8 */
    mask_len = (unsigned int)(pkt_len - FPOS(event_ext_rpt_ctrl_set_mask_req_type, mask));
    config = 0xFF;

    if( req->cmd_version == 1 )
    {
      event_mask_set_internal_handler( DIAG_OP_SET_EVENT_STREAM, req_set->header.id.stream_id,
                                       req_set->numbits_to_set, mask_len, (unsigned char *)req_set->mask );

#if defined(DIAG_MP_MASTER)
      /* Propagate masks to Slaves. */
      if( req_set->header.id.stream_id == DIAG_STREAM_1 )
        diagcomm_mask_update( DIAG_UPDATE_MASK_EVENTS, req_set->header.id.stream_id, diag_cur_preset_id, TRUE );
      else if( req_set->header.id.stream_id == DIAG_STREAM_2 )
        diagcomm_mask_update( DIAG_UPDATE_MASK_EVENTS, req_set->header.id.stream_id, DIAG_DCI_MASK_1, TRUE ); //ToDo: use array?
#endif
    }
    else if( req->cmd_version == 2 )
    {
      event_mask_set_internal_handler( DIAG_OP_SET_EVENT_PRESET, req_set->header.id.preset_id,
                                       req_set->numbits_to_set, mask_len, (unsigned char *)req_set->mask );

      event_control( config, EVENT_CONFIG_COMM_MASK );
#if defined(DIAG_MP_MASTER)
      /* Propagate masks to Slaves. */
      diagcomm_mask_update( DIAG_UPDATE_MASK_EVENTS, DIAG_STREAM_1, req_set->header.id.preset_id, TRUE );
#endif
    }

    (void)diag_client_maskchange(DIAG_EVENTSVC_MASK_CHANGE);

    if( send_rsp )
    {
      rsp_len = FPOS(event_ext_rpt_ctrl_rsp_type,mask);

      /* Copy back the modified mask into the response */
      if( req->cmd_version == 1 )
      {
        #if defined(DIAG_MP_MASTER)
          mask_len = diag_event_mask_mstr[req->id.stream_id-1]->mask_len;
          numbits = diag_event_mask_mstr[req->id.stream_id-1]->numbits;
          rsp_mask = diag_event_mask_mstr[req->id.stream_id-1]->mask;
        #else
          mask_len = EVENT_MASK_SIZE;
          numbits = EVENT_LAST_ID + 1;
          rsp_mask = diag_event_mask[req->id.stream_id-1];
        #endif

        rsp_len += mask_len;
        rsp = (event_ext_rpt_ctrl_rsp_type *)
            diagpkt_subsys_alloc (DIAG_SUBSYS_DIAG_SERV, req_set->header.subsys_cmd_code, rsp_len);
        if(rsp == NULL)
          return NULL;

        rsp->header.id.stream_id = req_set->header.id.stream_id;
        memscpy( rsp->mask, mask_len, rsp_mask, mask_len );
      }
      else if( req->cmd_version == 2 )
      {
        #if defined(DIAG_MP_MASTER)
          mask_len = diag_event_mask_mstr_preset[req->id.preset_id-1].mask_len;
          numbits = diag_event_mask_mstr_preset[req->id.preset_id-1].numbits;
          rsp_mask = diag_event_mask_mstr_preset[req->id.preset_id-1].mask;
        #else
          mask_len = EVENT_MASK_SIZE;
          numbits = EVENT_LAST_ID + 1;
          rsp_mask = diag_event_mask[req->id.stream_id-1];
        #endif

        rsp_len += mask_len;
        rsp = (event_ext_rpt_ctrl_rsp_type *)
            diagpkt_subsys_alloc (DIAG_SUBSYS_DIAG_SERV, req_set->header.subsys_cmd_code, rsp_len);
        if(rsp == NULL)
          return NULL;

        rsp->header.id.preset_id = req_set->header.id.preset_id;
        memscpy( rsp->mask, mask_len, rsp_mask, mask_len );
      }

      // diagpkt_subsys_alloc copies the subsys_id and subsys_cmd_code
      rsp->header.cmd_code = req_set->header.cmd_code;
      rsp->header.cmd_code = req->cmd_code;
      rsp->header.cmd_version = req->cmd_version;
      rsp->header.subcmd = req->subcmd;
      rsp->error_code = 0;
      rsp->numbits = numbits;
    }
    return (void *)rsp;
  }

  else if( (req->subcmd == DIAG_EXT_EVENT_REPORT_SET_ALL) && (req->cmd_version == 1) )  // Sets the entire mask for streams
  {
    int stream_index, mask_index = 0;
    event_ext_set_all_rsp_type *rsp=NULL;
    event_ext_rpt_ctrl_set_all_req_type *req_all = (event_ext_rpt_ctrl_set_all_req_type *)req_pkt;

    config = (req_all->config)? 0xFF : 0;

    if(req_all->header.id.stream_id > 0 && req_all->header.id.stream_id <= DIAG_MAX_STREAM_ID)
    {
      for(mask_index=0; mask_index < EVENT_MASK_SIZE; mask_index++)
      {
        diag_event_mask[req_all->header.id.stream_id-1][mask_index] = config;
      }

#if defined(DIAG_MP_MASTER)
      /* Save master event mask */
      for( mask_index = 0; mask_index < MSTR_EVENT_MASK_SIZE; mask_index++ )
        diag_event_mask_mstr[req_all->header.id.stream_id-1]->mask[mask_index] = config;

      if( config )
        diag_event_mask_mstr[req_all->header.id.stream_id-1]->status = DIAG_CTRL_MASK_ALL_ENABLED;
      else
        diag_event_mask_mstr[req_all->header.id.stream_id-1]->status = DIAG_CTRL_MASK_ALL_DISABLED;

      /* Propagate masks to Slaves. */
      if( req_all->header.id.stream_id == DIAG_STREAM_1 )
        diagcomm_mask_update( DIAG_UPDATE_MASK_EVENTS, req_all->header.id.stream_id, diag_cur_preset_id, TRUE );
      else if( req_all->header.id.stream_id == DIAG_STREAM_2 )
        diagcomm_mask_update( DIAG_UPDATE_MASK_EVENTS, req_all->header.id.stream_id, DIAG_DCI_MASK_1, TRUE ); //ToDo: create array for cur stream preset?
#else
      if( req_all->header.id.stream_id == DIAG_STREAM_1 )
      {
        if( config )
          event_preset_status[diag_cur_preset_id-1] = DIAG_CTRL_MASK_ALL_ENABLED;
        else
          event_preset_status[diag_cur_preset_id-1] = DIAG_CTRL_MASK_ALL_DISABLED;
      }
#endif
    }

    if(req_all->header.id.stream_id == DIAG_STREAM_1)
    {
      event_control (config, EVENT_CONFIG_COMM_MASK);
    }
    else if(req_all->header.id.stream_id == DIAG_STREAM_2)
    {
      event_control_sec (config, EVENT_CONFIG_COMM_MASK);
    }
    else if(!req_all->header.id.stream_id)
    {
      // Set all the streams if stream_id = 0
      for(stream_index=0; stream_index < DIAG_MAX_STREAM_ID; stream_index++)
      {
        for(mask_index=0; mask_index < EVENT_MASK_SIZE; mask_index++)
        {
          diag_event_mask[stream_index][mask_index] = config;
        }

#if defined(DIAG_MP_MASTER)
        /* Save master event mask */
        for( mask_index = 0; mask_index < MSTR_EVENT_MASK_SIZE; mask_index++ )
          diag_event_mask_mstr[stream_index]->mask[mask_index] = config;

        if( config )
          diag_event_mask_mstr[stream_index]->status = DIAG_CTRL_MASK_ALL_ENABLED;
        else
          diag_event_mask_mstr[stream_index]->status = DIAG_CTRL_MASK_ALL_DISABLED;
#else
        if( stream_index == (DIAG_STREAM_1-1) )
        {
          if( config )
            event_preset_status[diag_cur_preset_id-1] = DIAG_CTRL_MASK_ALL_ENABLED;
          else
            event_preset_status[diag_cur_preset_id-1] = DIAG_CTRL_MASK_ALL_DISABLED;
        }
#endif
      }
      event_control(config, EVENT_CONFIG_COMM_MASK);
      event_control_sec(config, EVENT_CONFIG_COMM_MASK);

#if defined(DIAG_MP_MASTER)
      /* Propagate masks to Slaves. */
      diagcomm_mask_update( DIAG_UPDATE_MASK_EVENTS, DIAG_STREAM_1, diag_cur_preset_id, TRUE );
      diagcomm_mask_update( DIAG_UPDATE_MASK_EVENTS, DIAG_STREAM_2, DIAG_DCI_MASK_1, TRUE ); //ToDo: create array?
#endif
    }

    (void)diag_client_maskchange(DIAG_EVENTSVC_MASK_CHANGE);

    if(send_rsp)
    {
      rsp_len = sizeof(event_ext_set_all_rsp_type);
      rsp = (event_ext_set_all_rsp_type *)
            diagpkt_subsys_alloc (DIAG_SUBSYS_DIAG_SERV, req_all->header.subsys_cmd_code, rsp_len);
      if(rsp == NULL)
        return NULL;

      rsp->header.cmd_code = req_all->header.cmd_code;
      // diagpkt_subsys_alloc copies the subsys_id and subsys_cmd_code
      rsp->header.cmd_version = req_all->header.cmd_version;
      rsp->header.subcmd = req_all->header.subcmd;
      rsp->header.id.stream_id = req_all->header.id.stream_id;
      rsp->config = req_all->config;
      rsp->error_code = 0;
    }
    return(void *)rsp;
  }

  else if( (req->subcmd == DIAG_EXT_EVENT_REPORT_SET_ALL) && (req->cmd_version == 2) ) // Sets the entire mask for presets
  {
    uint32 mask_index = 0;
    uint8 preset_index = 0;
    event_ext_set_all_rsp_type *rsp=NULL;
    event_ext_rpt_ctrl_set_all_req_type *req_all = (event_ext_rpt_ctrl_set_all_req_type *)req_pkt;

#if defined(DIAG_MP_MASTER)
    uint32 mstr_mask_index = 0;
#endif

    config = (req_all->config)? 0xFF : 0;

    // Sets the whole mask for a single specified preset
    if( req_all->header.id.preset_id > 0 && req_all->header.id.preset_id <= DIAG_MAX_PRESET_ID )
    {
      for( mask_index=0; mask_index < EVENT_MASK_SIZE; mask_index++ )
      {
        diag_event_preset_mask[req_all->header.id.preset_id-1][mask_index] = config;
      }

      if( req_all->header.id.preset_id == diag_cur_preset_id )
      {
        event_control(config, EVENT_CONFIG_COMM_MASK);
      }

#if defined(DIAG_MP_MASTER)
      /* Save master event mask */
      for( mstr_mask_index = 0; mstr_mask_index < MSTR_EVENT_MASK_SIZE; mstr_mask_index++ )
        diag_event_mask_mstr_preset[req_all->header.id.preset_id-1].mask[mstr_mask_index] = config;

      if( config )
        diag_event_mask_mstr_preset[req_all->header.id.preset_id-1].status = DIAG_CTRL_MASK_ALL_ENABLED;
      else
        diag_event_mask_mstr_preset[req_all->header.id.preset_id-1].status = DIAG_CTRL_MASK_ALL_DISABLED;

      /* Propagate masks to Slaves. */
      diagcomm_mask_update( DIAG_UPDATE_MASK_EVENTS, DIAG_STREAM_1, req_all->header.id.preset_id, TRUE );
#else
      if( config )
        event_preset_status[req_all->header.id.preset_id-1] = DIAG_CTRL_MASK_ALL_ENABLED;
      else
        event_preset_status[req_all->header.id.preset_id-1] = DIAG_CTRL_MASK_ALL_DISABLED;
#endif
    }

    // Set the whole mask for all presets if preset_id = 0
    if( req_all->header.id.preset_id == 0 )
    {
      for( preset_index=0; preset_index < DIAG_MAX_PRESET_ID; preset_index++ )
      {
        for( mask_index=0; mask_index < EVENT_MASK_SIZE; mask_index++ )
        {
          diag_event_preset_mask[preset_index][mask_index] = config;
        }

#if defined(DIAG_MP_MASTER)
        /* Save master event mask */
        for( mstr_mask_index = 0; mstr_mask_index < MSTR_EVENT_MASK_SIZE; mstr_mask_index++ )
          diag_event_mask_mstr_preset[preset_index].mask[mstr_mask_index] = config;

        if( config )
          diag_event_mask_mstr_preset[preset_index].status = DIAG_CTRL_MASK_ALL_ENABLED;
        else
          diag_event_mask_mstr_preset[preset_index].status = DIAG_CTRL_MASK_ALL_DISABLED;

        /* Propagate masks to Slaves. */
        diagcomm_mask_update( DIAG_UPDATE_MASK_EVENTS, DIAG_STREAM_1, preset_index+1, TRUE );
#else
        if( config )
          event_preset_status[preset_index] = DIAG_CTRL_MASK_ALL_ENABLED;
        else
          event_preset_status[preset_index] = DIAG_CTRL_MASK_ALL_DISABLED;
#endif
      }
      event_control(config, EVENT_CONFIG_COMM_MASK);
    }

    (void)diag_client_maskchange(DIAG_EVENTSVC_MASK_CHANGE);

    if(send_rsp)
    {
      rsp_len = sizeof(event_ext_set_all_rsp_type);
      rsp = (event_ext_set_all_rsp_type *)
            diagpkt_subsys_alloc (DIAG_SUBSYS_DIAG_SERV, req_all->header.subsys_cmd_code, rsp_len);
      if(rsp == NULL)
        return NULL;

      rsp->header.cmd_code = req_all->header.cmd_code;
      // diagpkt_subsys_alloc copies the subsys_id and subsys_cmd_code
      rsp->header.cmd_version = req_all->header.cmd_version;
      rsp->header.subcmd = req_all->header.subcmd;
      rsp->header.id.preset_id = req_all->header.id.preset_id;
      rsp->config = req_all->config;
      rsp->error_code = 0;
    }
    return (void *)rsp;
  }

  else
  {
    if(send_rsp)
    {
      return diagpkt_err_rsp(DIAG_BAD_PARM_F, req_pkt, pkt_len);
    }
    else
    {
      return NULL;
    }
  }

} /* diagdiag_ext_event_report_ctrl */


#endif

