#if !defined(DIAG_MP)

/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                     Mobile Diagnostic Message Services

General Description
  This file contains the implementation for the msg mask handlers in standalone configuration.

Copyright (c) 2018 by QUALCOMM Technologies, Incorporated.  All Rights Reserved.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*==========================================================================

                           Edit History 

$Header: //components/rel/core.mpss/10.0/services/diag/DCM/common/src/msg_standalone.c#1 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
04/29/18   ph      Enable diag boot up logging.
07/04/18    gn     Created new file for standalone functionality of messages.

===========================================================================*/

#include "msg.h"        /* Message typedefs and prototypes */
#include "msg_qsr.h"
#include "diagdiag_v.h"
#include "diagi_v.h"      /* Needed for sleep vote. */
#include "msg_arrays_i.h"  /* For msg_mask_tbl etc */
#include "msgi.h"          /* For msg_mask_read_buffer_type etc */
#include "stringl.h" /* For memscpy */
#include "diagcomm_cmd.h"  /* For diagcomm_cmd_status */

/* Legacy values for level are used to index this array. Legacy values were:
   #define MSG_LVL_LOW     0
   #define MSG_LVL_MED     1
   #define MSG_LVL_HIGH    2
   #define MSG_LVL_ERROR   3
   #define MSG_LVL_FATAL   4
   #define MSG_LVL_NONE    255 */

static const uint32 msg_legacy_rt_masks[] = {
  MSG_LVL_LOW, MSG_LVL_MED, MSG_LVL_HIGH, MSG_LVL_ERROR, MSG_LVL_FATAL
};

extern uint8 diag_cur_preset_id;
extern uint32 msg_total;
extern uint32 msg_dropped;
extern boolean msg_set_rt_mask_adv (uint16 ssid, uint32 mask, byte stream_id, uint8 preset_id);
/*===========================================================================

FUNCTION MSG_PKT_GET_SSID_RANGES

DESCRIPTION
  This function will return the valid SSID range.

DEPENDENCIES

RETURN VALUE
  Pointer to response packet.

SIDE EFFECTS
  None.
===========================================================================*/
static void *
msg_pkt_get_ssid_ranges (void *req_pkt, word req_len)
{
  msg_get_ssid_ranges_req_type *req =
    (msg_get_ssid_ranges_req_type *) req_pkt;
  msg_get_ssid_ranges_rsp_type *rsp = NULL;
  unsigned int i = 0, rsp_len = 0;

/*-------------------------------------------------------------------------*/

  if (req && req->sub_cmd == (uint8) MSG_EXT_SUBCMD_GET_RANGES)
  {
    /*-------------------------------------------------------------------------
      calculate number of valid ssids and the length of the response packet.
    -------------------------------------------------------------------------*/
    rsp_len = FPOS (msg_get_ssid_ranges_rsp_type, ssid_ranges) +
      MSG_MASK_TBL_CNT * FSIZ (msg_get_ssid_ranges_rsp_type, ssid_ranges);

    /*-------------------------------------------------------------------------
      allocate space for the response packet
    -------------------------------------------------------------------------*/
    rsp = (msg_get_ssid_ranges_rsp_type *)
      diagpkt_alloc (DIAG_EXT_MSG_CONFIG_F, rsp_len);

    if (rsp)
    {
      rsp->sub_cmd = req->sub_cmd;
      rsp->status = TRUE;
      rsp->range_cnt = MSG_MASK_TBL_CNT;
      rsp->rsvd = 0;

      /*-------------------------------------------------------------------------
        make aux_access_ptr point to the end of the msg_get_ssid_ranges_rsp_type
        which is where the SSIDd should start.
      -------------------------------------------------------------------------*/

      for (i = 0; i < MSG_MASK_TBL_CNT; i++)
      {
        rsp->ssid_ranges[i].ssid_first = msg_mask_tbl[i].ssid_first;
        rsp->ssid_ranges[i].ssid_last = msg_mask_tbl[i].ssid_last;
      }
    }
  }

  else
  {
    rsp = (void *) diagpkt_err_rsp (DIAG_BAD_PARM_F, req_pkt, req_len);
  }

  return (rsp);
}               /* msg_pkt_get_ssid_ranges () */


static void *
msg_pkt_get_ssid_ranges_adv (void *req_pkt, word req_len)
{
  msg_get_ssid_ranges_adv_req_type *req =
    (msg_get_ssid_ranges_adv_req_type *) req_pkt;
  msg_get_ssid_ranges_adv_rsp_type *rsp = NULL;
  unsigned int i = 0, rsp_len = 0;

/*-------------------------------------------------------------------------*/

  if (req && (req_len == sizeof(msg_get_ssid_ranges_adv_req_type)) && (req->opcode == (uint8)MSG_EXT_SUBCMD_GET_RANGES) )
  {
    /*-------------------------------------------------------------------------
      calculate number of valid ssids and the length of the responce packet.
    -------------------------------------------------------------------------*/
    rsp_len = FPOS (msg_get_ssid_ranges_adv_rsp_type, ssid_ranges) +
      MSG_MASK_TBL_CNT * FSIZ (msg_get_ssid_ranges_adv_rsp_type, ssid_ranges);

    /*-------------------------------------------------------------------------
      allocate space for the responce packet
    -------------------------------------------------------------------------*/
    rsp = (msg_get_ssid_ranges_adv_rsp_type *)
      diagpkt_subsys_alloc (DIAG_SUBSYS_DIAG_SERV, DIAG_EXT_MSG_CONFIG_ADV, rsp_len);

    if (rsp)
    {
      rsp->header.cmd_code = req->cmd_code;
      rsp->header.cmd_version = req->cmd_version;
      rsp->header.opcode = req->opcode;
      rsp->status = TRUE;
      rsp->range_cnt = MSG_MASK_TBL_CNT;
      rsp->rsvd = 0;

      /*-------------------------------------------------------------------------
        make aux_access_ptr point to the end of the msg_get_ssid_ranges_rsp_type
        which is where the SSIDd should start.
      -------------------------------------------------------------------------*/

      for (i = 0; i < MSG_MASK_TBL_CNT; i++)
      {
        rsp->ssid_ranges[i].ssid_first = msg_mask_tbl[i].ssid_first;
        rsp->ssid_ranges[i].ssid_last = msg_mask_tbl[i].ssid_last;
      }
    }
  }
  else
  {
    rsp = (void *) diagpkt_err_rsp (DIAG_BAD_PARM_F, req_pkt, req_len);
  }

  return (rsp);
}               /* msg_pkt_get_ssid_ranges_adv () */


/*===========================================================================

FUNCTION MSG_PKT_GET_MASKS

DESCRIPTION
  This function will retrieve an array of masks of specified type for the
  range of SSIDs provided.

DEPENDENCIES
  req is not NULL and points to a valid  request packet

RETURN VALUE
  Pointer to response packet.

SIDE EFFECTS
  None.
===========================================================================*/
static void *
msg_pkt_get_masks (void *req_pkt, word req_len, msg_mask_type mask_type)
{
  msg_get_mask_req_type *req = (msg_get_mask_req_type *) req_pkt;
  msg_get_mask_rsp_type *rsp = NULL;
  unsigned int rsp_len = 0;

/*-------------------------------------------------------------------------*/

  if (req && req->ssid_start <= req->ssid_end &&
      (req->sub_cmd == (uint8) MSG_EXT_SUBCMD_GET_BUILD_MASK ||
       req->sub_cmd == (uint8) MSG_EXT_SUBCMD_GET_RT_MASK))
  {
    rsp_len = FPOS (msg_get_mask_rsp_type, bld_mask) +
      ((req->ssid_end - req->ssid_start + 1) *
       FSIZ (msg_get_mask_rsp_type, bld_mask));

    rsp = (msg_get_mask_rsp_type *)
      diagpkt_alloc (DIAG_EXT_MSG_CONFIG_F, rsp_len);

    if (rsp)
    {
      rsp->sub_cmd = req->sub_cmd;
      rsp->ssid_start = req->ssid_start;
      rsp->ssid_end = req->ssid_end;
      rsp->pad = 0;

      rsp->status = msg_get_ssid_masks_adv (mask_type, req->ssid_start,
                    req->ssid_end, rsp->bld_mask, DIAG_STREAM_1, diag_cur_preset_id);
    }
  }
  else
  {
    rsp = (msg_get_mask_rsp_type *)
      diagpkt_err_rsp (DIAG_BAD_PARM_F, req_pkt, req_len);
  }

  return (rsp);

}               /* msg_pkt_get_masks() */


static void *
msg_pkt_get_masks_adv (void *req_pkt, word req_len, msg_mask_type mask_type)
{
  msg_get_mask_adv_req_type *req = (msg_get_mask_adv_req_type *) req_pkt;
  msg_get_mask_adv_rsp_type *rsp = NULL;
  msg_get_rt_mask_adv_req_type *req_rt = (msg_get_rt_mask_adv_req_type *) req_pkt;
  msg_get_rt_mask_adv_rsp_type *rsp_rt = NULL;

  unsigned int rsp_len = 0;

/*-------------------------------------------------------------------------*/

  if (req &&
      (req->header.opcode == (uint8) MSG_EXT_SUBCMD_GET_BUILD_MASK ||
       req->header.opcode == (uint8) MSG_EXT_SUBCMD_GET_RT_MASK))
  {

    if(req->header.opcode == (uint8) MSG_EXT_SUBCMD_GET_BUILD_MASK)
    {
      if(req->ssid_first > req->ssid_last)
      {
        return ((void *) diagpkt_err_rsp (DIAG_BAD_PARM_F, req_pkt, req_len));
      }
         rsp_len = FPOS (msg_get_mask_adv_rsp_type, bld_mask) +
        ((req->ssid_last - req->ssid_first + 1) *
         FSIZ (msg_get_mask_adv_rsp_type, bld_mask));

      rsp = (msg_get_mask_adv_rsp_type *)
            diagpkt_subsys_alloc (DIAG_SUBSYS_DIAG_SERV, DIAG_EXT_MSG_CONFIG_ADV, rsp_len);

      if (rsp)
      {
        rsp->header = req->header;
        rsp->ssid_first = req->ssid_first;
        rsp->ssid_last = req->ssid_last;
        rsp->rsvd = 0;

        rsp->status = msg_get_ssid_masks_adv (mask_type, req->ssid_first,
                      req->ssid_last, (uint32 *)(rsp->bld_mask),
                      DIAG_STREAM_1, diag_cur_preset_id);  // stream_id and preset_id doesn't matter here.
      }

      return (void *)rsp;
    }
    else
    {
      if( (req_rt->header.cmd_version == 1) && (req_rt->id.stream_id >=DIAG_MIN_STREAM_ID) &&
          (req_rt->id.stream_id <= DIAG_MAX_STREAM_ID) && (req_rt->ssid_last >= req_rt->ssid_first) )
      {
        rsp_len = FPOS (msg_get_rt_mask_adv_rsp_type, rt_mask) +
        ((req_rt->ssid_last - req_rt->ssid_first + 1) *
         FSIZ (msg_get_rt_mask_adv_rsp_type, rt_mask));

        rsp_rt = (msg_get_rt_mask_adv_rsp_type *)
                  diagpkt_subsys_alloc (DIAG_SUBSYS_DIAG_SERV, DIAG_EXT_MSG_CONFIG_ADV, rsp_len);

        if( rsp_rt )
        {
          rsp_rt->header = req_rt->header;
          rsp_rt->id.stream_id = req_rt->id.stream_id;
          rsp_rt->ssid_first = req_rt->ssid_first;
          rsp_rt->ssid_last = req_rt->ssid_last;

          rsp_rt->status = msg_get_ssid_masks_adv( mask_type, req_rt->ssid_first,
                                                   req_rt->ssid_last, (uint32 *)(rsp_rt->rt_mask),
                                                   req_rt->id.stream_id, diag_cur_preset_id );
        }
        return (void *)rsp_rt;
      }
      else if( (req_rt->header.cmd_version == 2) && (req_rt->id.preset_id >=DIAG_MIN_PRESET_ID) &&
               (req_rt->id.preset_id <= DIAG_MAX_PRESET_ID) && (req_rt->ssid_last >= req_rt->ssid_last) )
      {
        rsp_len = FPOS (msg_get_rt_mask_adv_rsp_type, rt_mask) +
        ((req_rt->ssid_last - req_rt->ssid_first + 1) *
         FSIZ (msg_get_rt_mask_adv_rsp_type, rt_mask));

        rsp_rt = (msg_get_rt_mask_adv_rsp_type *)
                  diagpkt_subsys_alloc (DIAG_SUBSYS_DIAG_SERV, DIAG_EXT_MSG_CONFIG_ADV, rsp_len);

        if( rsp_rt )
        {
          rsp_rt->header = req_rt->header;
          rsp_rt->id.preset_id = req_rt->id.preset_id;
          rsp_rt->ssid_first = req_rt->ssid_first;
          rsp_rt->ssid_last = req_rt->ssid_last;

          rsp_rt->status = msg_get_ssid_masks_adv( mask_type, req_rt->ssid_first,
                                                   req_rt->ssid_last, (uint32 *)(rsp_rt->rt_mask),
                                                   DIAG_STREAM_1, req_rt->id.preset_id );
        }
        return (void *)rsp_rt;
      }
      else
      {
        return ( (void *)diagpkt_err_rsp (DIAG_BAD_PARM_F, req_pkt, req_len) );
      }
    }
  }
  else
  {
     return ((void *) diagpkt_err_rsp (DIAG_BAD_PARM_F, req_pkt, req_len));
  }
}               /* msg_pkt_get_masks_adv() */


/*===========================================================================

FUNCTION  MSG_PKT_SET_RT_MASKS

DESCRIPTION
  This function will SET the  value of the "tool" mask for the
  specific SubSystem ID and prepare a responce packet. If the
  SSID is not valid it will return Status = false.

DEPENDENCIES
  req is not NULL and points to a valid  request packet

RETURN VALUE
  Pointer to response packet.

SIDE EFFECTS
  None.
===========================================================================*/
static void *
msg_pkt_set_rt_masks (void *req_pkt, word req_len)
{
  msg_set_rt_mask_req_type *req = (msg_set_rt_mask_req_type *) req_pkt;
  msg_set_rt_mask_rsp_type *rsp = NULL;
  uint32 *mask = NULL;
  unsigned int rsp_len = req_len;   /* Same as request */
  int i;
  boolean success = TRUE;

/*-------------------------------------------------------------------------*/

  if (req && req->sub_cmd == MSG_EXT_SUBCMD_SET_RT_MASK &&
      req->ssid_start <= req->ssid_end)
  {
    rsp = (msg_set_rt_mask_rsp_type *)
      diagpkt_alloc (DIAG_EXT_MSG_CONFIG_F, rsp_len);

    if (rsp)
    {
      memscpy ((void *) rsp, ((FPOS(msg_set_rt_mask_req_type,rt_mask)) + (4 * ((req->ssid_end - req->ssid_start)+1))), (void *) req, rsp_len);

      rsp->status = TRUE;   /* Initialize */

      mask = req->rt_mask;

      for (i = req->ssid_start; i <= req->ssid_end; i++)
      {
        success = msg_set_rt_mask_adv ((uint16) i, *mask, DIAG_STREAM_1, diag_cur_preset_id);

        if (success == FALSE)
        {
          rsp->status = FALSE;
        }

        mask++;         /* Advance to next entry in array */
      }

#if defined(DIAG_MP_MASTER)
      /* Propagate masks to Slaves. */
      diagcomm_mask_update( DIAG_UPDATE_MASK_PARTIAL_F3, DIAG_STREAM_1, diag_cur_preset_id, TRUE );
#endif

      /* Send the updated mask to every LSM registered with DCM. */
      (void)diag_client_maskchange(DIAG_MSGSVC_MASK_CHANGE);

      (void) msg_sleep_vote ();
    }
  }
  else
  {
    rsp = (msg_set_rt_mask_rsp_type *)
      diagpkt_err_rsp (DIAG_BAD_PARM_F, req_pkt, req_len);
  }

  return (rsp);

}               /* msg_pkt_set_rt_masks() */

/* Function to set run-time masks based on stream_id */

static void *
msg_pkt_set_rt_masks_adv (void *req_pkt, word req_len)
{
  msg_set_rt_mask_adv_req_type *req = (msg_set_rt_mask_adv_req_type *) req_pkt;
  msg_set_rt_mask_adv_rsp_type *rsp = NULL;
  uint32 *mask = NULL;
  unsigned int rsp_len = req_len;   /* Same as request */
  int i;
  boolean success = TRUE;
  boolean send_error = TRUE;
  byte stream_id;
  uint8 preset_id;

/*-------------------------------------------------------------------------*/

  if (req && (req->header.opcode == MSG_EXT_SUBCMD_SET_RT_MASK) && (req->ssid_first <= req->ssid_last) )
  {
    if( req->header.cmd_version == 1 )
    {
      /* stream_id = 0 is not valid here */
      if( (req->id.stream_id >= DIAG_MIN_STREAM_ID) && (req->id.stream_id <= DIAG_MAX_STREAM_ID) )
      {
        send_error = FALSE;
        stream_id = req->id.stream_id;
        preset_id = diag_cur_preset_id;
      }
    }
    else if( req->header.cmd_version == 2 )
    {
      /* preset_id = 0 is not valid here */
      if( (req->id.preset_id >= DIAG_MIN_PRESET_ID) && (req->id.preset_id <= DIAG_MAX_PRESET_ID) )
      {
        send_error = FALSE;
        stream_id = DIAG_STREAM_1;
        preset_id = req->id.preset_id;
      }
    }
  }

  if( send_error )
  {
    rsp = (msg_set_rt_mask_adv_rsp_type *)
      diagpkt_err_rsp (DIAG_BAD_PARM_F, req_pkt, req_len);
  }
  else
  {
    rsp = (msg_set_rt_mask_adv_rsp_type *)
          diagpkt_subsys_alloc (DIAG_SUBSYS_DIAG_SERV, DIAG_EXT_MSG_CONFIG_ADV, rsp_len);

    if (rsp)
    {
      memscpy ((void *) rsp, ((FPOS(msg_set_rt_mask_adv_req_type,rt_mask)) + (4 * ((req->ssid_last - req->ssid_first)+1))), (void *) req, rsp_len);

      rsp->status = TRUE;   /* Initialize */

      mask = (uint32 *)(req->rt_mask);

      for (i = req->ssid_first; i <= req->ssid_last; i++)
      {
        success = msg_set_rt_mask_adv ((uint16) i, *mask, stream_id, preset_id);

        if (success == FALSE)
        {
          rsp->status = FALSE;
        }

        mask++;         /* Advance to next entry in array */
      }

#if defined(DIAG_MP_MASTER)
      /* Propagate masks to Slaves. */
      diagcomm_mask_update( DIAG_UPDATE_MASK_PARTIAL_F3, stream_id, preset_id, TRUE );
#endif

      /* Send the updated mask to every LSM registered with DCM. */
      (void)diag_client_maskchange(DIAG_MSGSVC_MASK_CHANGE);

      (void) msg_sleep_vote ();
    }
  }

  return (rsp);

} /* msg_pkt_set_rt_masks_adv */


/*===========================================================================

FUNCTION   MSG_PKT_SET_ALL_MASKS

DESCRIPTION
  This function will SET the  values of the "tool" masks for all
  the valid SubSystem IDs and prepare a responce packet.

DEPENDENCIES
  req is not NULL and points to a valid  request packet

RETURN VALUE
  Pointer to response packet.

SIDE EFFECTS
  None.
===========================================================================*/
static void *
msg_pkt_set_all_masks (void *req_pkt, word req_len)
{
  msg_set_all_masks_req_type *req = (msg_set_all_masks_req_type *) req_pkt;
  msg_set_all_masks_rsp_type *rsp = NULL;
  const unsigned int rsp_len = sizeof (msg_set_all_masks_rsp_type);

/*-------------------------------------------------------------------------*/

  if (req && req->sub_cmd == MSG_EXT_SUBCMD_SET_ALL_RT_MASKS)
  {
    rsp = (msg_set_all_masks_rsp_type *)
      diagpkt_alloc (DIAG_EXT_MSG_CONFIG_F, rsp_len);

    /*-------------------------------------------------------------------------
      fill in the fields of the responce packet
    -------------------------------------------------------------------------*/
    if(rsp)
    {
        rsp->sub_cmd = req->sub_cmd;
        rsp->status = TRUE;
        rsp->rsvd = 0;
        rsp->rt_mask = req->rt_mask;
    }
    /*-------------------------------------------------------------------------
      set all the real time masks to the new requested value.
    -------------------------------------------------------------------------*/
    msg_set_all_rt_masks_adv (req->rt_mask, DIAG_STREAM_1, diag_cur_preset_id);
  }
  else
  {
    rsp = (msg_set_all_masks_rsp_type *)
      diagpkt_err_rsp (DIAG_BAD_PARM_F, req_pkt, req_len);
  }

  return (rsp);

}               /* msg_pkt_set_all_masks() */


static void *
msg_pkt_set_all_masks_adv (void *req_pkt, word req_len)
{
  byte stream_id;
  uint8 preset_id;
  boolean send_error = TRUE;
  unsigned int rsp_len=0;
  msg_set_all_masks_adv_rsp_type *rsp = NULL;
  msg_set_all_masks_adv_req_type *req = (msg_set_all_masks_adv_req_type *) req_pkt;

  rsp_len = sizeof (msg_set_all_masks_adv_rsp_type);

  /*-------------------------------------------------------------------------*/

  if (req && (req->header.opcode == MSG_EXT_SUBCMD_SET_ALL_RT_MASKS) )
  {
    if( req->header.cmd_version == 1 )
    {
      if(req->id.stream_id <= DIAG_MAX_STREAM_ID)
      {
        send_error = FALSE;
        stream_id = req->id.stream_id;
        preset_id = diag_cur_preset_id;
      }
    }
    else if( req->header.cmd_version == 2 )
    {
      if(req->id.preset_id <= DIAG_MAX_PRESET_ID)
      {
        send_error = FALSE;
        stream_id = DIAG_STREAM_1;
        preset_id = req->id.preset_id;
      }
    }
  }

  if( send_error )
  {
    rsp = (msg_set_all_masks_adv_rsp_type *)
      diagpkt_err_rsp (DIAG_BAD_PARM_F, req_pkt, req_len);
  }
  else
  {
    rsp = (msg_set_all_masks_adv_rsp_type *)
          diagpkt_subsys_alloc (DIAG_SUBSYS_DIAG_SERV, DIAG_EXT_MSG_CONFIG_ADV, rsp_len);

    /*-------------------------------------------------------------------------
      fill in the fields of the responce packet
    -------------------------------------------------------------------------*/
    if(rsp)
    {
      rsp->header = req->header;
      rsp->status = TRUE;
      rsp->rt_mask = req->rt_mask;

      if( req->header.cmd_version == 1 )
        rsp->id.stream_id = req->id.stream_id;
      else if( req->header.cmd_version == 2 )
        rsp->id.preset_id = req->id.preset_id;
    }
    /*-------------------------------------------------------------------------
      set all the real time masks to the new requested value.
    -------------------------------------------------------------------------*/
    msg_set_all_rt_masks_adv (req->rt_mask, stream_id, preset_id);
  }

  return (rsp);

} /* msg_pkt_set_all_masks_adv() */


/*===========================================================================

FUNCTION MSG_PKT_PROCESS_CONFIG

DESCRIPTION
  This function will figure out the type of extended message request
  by examining the message command and then call the appropriate
  function to handle the request.

DEPENDENCIES

RETURN VALUE
  Pointer to response packet.

SIDE EFFECTS
  None.
===========================================================================*/
void *
msg_pkt_process_config (void * req_pkt, word req_len)
{
  msg_get_ssid_ranges_req_type *req =
    (msg_get_ssid_ranges_req_type *) req_pkt;
  void *rsp = NULL;
  boolean valid_cmd = TRUE;
  boolean send_rsp = TRUE;
  #if defined(DIAG_MP_MASTER)
  boolean set_mask_req = FALSE;
  #endif

  /*-------------------------------------------------------------------------*/

  if (req && req->cmd_code == DIAG_EXT_MSG_CONFIG_F)
  {
    /* dispatch acording to subcommand */
    switch (req->sub_cmd)
    {
    case MSG_EXT_SUBCMD_GET_RANGES:

    /*---------------------------------------------------------------------
          This will prepare a packet with all valid SubSystem IDs.
        ---------------------------------------------------------------------*/
      rsp = msg_pkt_get_ssid_ranges (req, req_len);
      break;

    case MSG_EXT_SUBCMD_GET_BUILD_MASK:

    /*---------------------------------------------------------------------
          Will return a packet with the "build" mask for a specific SSID
        ---------------------------------------------------------------------*/
      rsp = msg_pkt_get_masks (req, req_len, MSG_MASK_BT);
      break;

    case MSG_EXT_SUBCMD_GET_RT_MASK:

    /*---------------------------------------------------------------------
          Returns a packet with the "tool" mask for a specific SSID
        ---------------------------------------------------------------------*/
      rsp = msg_pkt_get_masks (req, req_len, MSG_MASK_RT);

      break;

    case MSG_EXT_SUBCMD_SET_RT_MASK:

    /*---------------------------------------------------------------------
          forces mobile to set the "tool" mask for a specific SSID
        ---------------------------------------------------------------------*/
      rsp = msg_pkt_set_rt_masks (req, req_len);
      #if defined(DIAG_MP_MASTER)
      set_mask_req = TRUE;
      #endif
      break;

    case MSG_EXT_SUBCMD_SET_ALL_RT_MASKS:

    /*---------------------------------------------------------------------
          set "tool" masks for all valid SSIDs to a given value.
        ---------------------------------------------------------------------*/
      rsp = msg_pkt_set_all_masks (req, req_len);
      #if defined(DIAG_MP_MASTER)
      set_mask_req = TRUE;
      #endif
      break;

    default:            /* nothing to do */
      rsp = (void *) diagpkt_err_rsp (DIAG_BAD_PARM_F, req, req_len);
      valid_cmd = TRUE;
      break;
    }

    if (valid_cmd)
    {
      msg_mode = MSG_MODE_STD;
    }
  }
  else
  {
    rsp = (void *) diagpkt_err_rsp (DIAG_BAD_PARM_F, req, req_len);
  }

 /* if master processor on multi processor build, don't send response */
  #if !defined(DIAG_RSP_SEND)
  if (!diagcomm_apps_only_config)
    send_rsp = FALSE;
  #endif

  #if defined(DIAG_MP_MASTER)
    /* MP Master now responds to "set mask" requests */
    if( set_mask_req )
      send_rsp = TRUE;
  #endif

 /* if diag is not connected, don't send response */
  if (!diagcomm_cmd_status(DIAGCOMM_PORT_1))
  {
    send_rsp = FALSE;
  }

  #if defined (FEATURE_DIAG_ADSP_CALLBACK)
      adsp_dspdiag125_callback(req_pkt, req_len);
  #endif /* FEATURE_DIAG_ADSP_CALLBACK */

  /* return response */
  if (!send_rsp)
  {
    diagpkt_free(rsp);
    return NULL;
  }
  else
  {
    return ((void *) rsp);
  }

} /* msg_pkt_process_config */


/*===========================================================================

FUNCTION MSG_PKT_LEGACY_REQ

DESCRIPTION
  This procedure processes a request to retreive the accumulated msgs in
  the mobile's msg buffer.

===========================================================================*/
void *
msg_pkt_legacy_req (void * req_ptr, word pkt_len)
{
  msg_legacy_req_type *req = (msg_legacy_req_type *) req_ptr;
  msg_legacy_rsp_type *rsp = NULL;
  const unsigned int rsp_len = sizeof (msg_legacy_rsp_type);
  boolean send_rsp = TRUE;

  (void) pkt_len; /* suppress compiler warning */


#if defined (MSG_LEVEL) && (MSG_LEVEL != MSG_LVL_NONE)
  /* Suppress msg_file compiler warning. ** This compiler optimizes this to
     nothing, ** but msg_file is now referenced. */
#endif

  /* Legacy values for level are used to index this array. Legacy values were:
     #define MSG_LVL_LOW     0 #define MSG_LVL_MED     1 #define MSG_LVL_HIGH 2
     #define MSG_LVL_ERROR   3 #define MSG_LVL_FATAL   4 #define MSG_LVL_NONE
     255 */

  if (req->msg_level <
      sizeof (msg_legacy_rt_masks) / sizeof (msg_legacy_rt_masks[0]))
  {
    msg_set_all_rt_masks_adv (msg_legacy_rt_masks[req->msg_level], DIAG_STREAM_1, diag_cur_preset_id);
  }
  else
  {
    msg_set_all_rt_masks_adv (0, DIAG_STREAM_1, diag_cur_preset_id);
  }

  (void) msg_sleep_vote ();

  /* Set mode to legacy mode. */
  msg_mode = MSG_MODE_LEGACY;

  /* if master processor on multi processor build, don't send response */
  #if !defined(DIAG_RSP_SEND)
  if (!diagcomm_apps_only_config)
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


  /*----------------------------------------------------------------------
    Format the response packet.
  ----------------------------------------------------------------------*/
  if (send_rsp)
  {
    rsp = (msg_legacy_rsp_type *) diagpkt_alloc (DIAG_MSG_F, rsp_len);

    if (rsp)
    {
      rsp->total_msgs = msg_total;

      /*----------------------------------------------------
      Since we are streaming, this response is an ACK.
      Set qty to 0.
      ----------------------------------------------------*/
      rsp->qty = 0;

      rsp->drop_cnt = msg_dropped;

    }
  }

  return ((void *) rsp);


}               /* msg_pkt_legacy_req */


/*===========================================================================

FUNCTION EXT_MSG_PKT_CONFIG_ADV

DESCRIPTION
  Function to handle dual mask msg config requests.

DEPENDENCIES

RETURN VALUE
  Pointer to response packet.

SIDE EFFECTS
  None.
===========================================================================*/
void *
ext_msg_pkt_config_adv (void * req_pkt, word req_len)
{
  ext_config_header *req = (ext_config_header *) req_pkt;
  void *rsp = NULL;
  boolean valid_cmd = TRUE;
  boolean send_rsp = TRUE;

  if ( req && (req->subsys_cmd_code == DIAG_EXT_MSG_CONFIG_ADV)
           && ((req->cmd_version == 1) || (req->cmd_version == 2)) )
  {

    /* dispatch acording to subcommand */
    switch (req->opcode)
    {
    case MSG_EXT_SUBCMD_GET_RANGES:

    /*---------------------------------------------------------------------
          This will prepare a packet with all valid SubSystem IDs.
        ---------------------------------------------------------------------*/
      rsp = msg_pkt_get_ssid_ranges_adv (req, req_len);
      break;

    case MSG_EXT_SUBCMD_GET_BUILD_MASK:

    /*---------------------------------------------------------------------
          Will return a packet with the "build" mask for a specific SSID
        ---------------------------------------------------------------------*/
      rsp = msg_pkt_get_masks_adv (req, req_len, MSG_MASK_BT);
      break;

    case MSG_EXT_SUBCMD_GET_RT_MASK:

    /*---------------------------------------------------------------------
          Returns a packet with the "tool" mask for a specific SSID
        ---------------------------------------------------------------------*/
      rsp = msg_pkt_get_masks_adv (req, req_len, MSG_MASK_RT);

      break;

    case MSG_EXT_SUBCMD_SET_RT_MASK:

    /*---------------------------------------------------------------------
          forces mobile to set the "tool" mask for a specific SSID
        ---------------------------------------------------------------------*/
      rsp = msg_pkt_set_rt_masks_adv (req, req_len);
      break;

    case MSG_EXT_SUBCMD_SET_ALL_RT_MASKS:

    /*---------------------------------------------------------------------
          set "tool" masks for all valid SSIDs to a given value.
        ---------------------------------------------------------------------*/
      rsp = msg_pkt_set_all_masks_adv (req, req_len);
      break;

    default:            /* nothing to do */
      rsp = (void *) diagpkt_err_rsp (DIAG_BAD_PARM_F, req, req_len);
      valid_cmd = TRUE;
      break;
    }

    if (valid_cmd)
    {
      msg_mode = MSG_MODE_STD;
    }
  }
  else
  {
    rsp = (void *) diagpkt_err_rsp (DIAG_BAD_PARM_F, req, req_len);
  }

  /* if master processor on multi processor build, don't send response */
  #if !defined(DIAG_RSP_SEND)
  if (!diagcomm_apps_only_config)
    send_rsp = FALSE;
  #endif

  #if defined(DIAG_MP_MASTER)
    /* MP Master now responds to "set mask" requests.
       If we're in buffering mode, Master needs to send out the responses.
       If Modem is not up, Master needs to send out responses as well. */
    send_rsp = TRUE;
  #endif

 /* if diag is not connected, don't send response */
  if (!diagcomm_cmd_status(DIAGCOMM_PORT_1))
  {
    send_rsp = FALSE;
  }

  #if defined (FEATURE_DIAG_ADSP_CALLBACK)
      adsp_dspdiag125_callback(req_pkt, req_len);
  #endif /* FEATURE_DIAG_ADSP_CALLBACK */

  /* return response */
  if (!send_rsp)
  {
    diagpkt_free(rsp);
    return NULL;
  }
  else
  {
    return ((void *) rsp);
  }
} /* ext_msg_pkt_config_adv */

#endif

