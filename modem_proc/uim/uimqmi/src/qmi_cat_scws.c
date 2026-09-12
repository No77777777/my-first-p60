/*==========================================================================

DESCRIPTION
Handle QMI CAT SCWS commands

Copyright (c) 2015-2016 QUALCOMM Technologies, Inc(QTI).
All Rights Reserved.
QUALCOMM Technologies Confidential and Proprietary

  $Header: //components/rel/uim.mpss/6.1.0/uimqmi/src/qmi_cat_scws.c#2 $ $DateTime: 2019/10/30 06:56:05 $ $Author: pwbldsvc $

when        who    what, where, why
--------    ---    ----------------------------------------------------------
11/04/16    me     Combined multi-line ASSERT in one line
05/20/16    vdc    Remove F3 messages for memory allocation failure
03/03/16    tkl    Add indication support for TR and Envelope cmd
01/13/16    vr     Common function for result tlv and send response
11/09/15    vv     Remove unneeded F3's
10/27/15    vr     Multi-profile switch handling
08/04/15    vr     Initial version
===========================================================================*/

/*===========================================================================

                          INCLUDE FILES FOR MODULE

===========================================================================*/
#include "uim_variation.h"
#include "comdef.h"
#include "customer.h"
#include "amssassert.h"

#include <stringl/stringl.h>

#include "qmi_framework.h"
#include "qmi_svc_utils.h"

#include "ds_qmi_fw_common.h"

#include "qmi_uim_cat_common.h"
#include "qmi_cat_common.h"
#include "qmi_cat_scws.h"
#include "qmi_cat_scws_parsing.h"

#include "uim_msg.h"

/*===========================================================================

                               INTERNAL DATA

===========================================================================*/
/*---------------------------------------------------------------------------
  Values used for cache (decoded)
---------------------------------------------------------------------------*/
#define QMI_CAT_GSTK_EVT_MASK_SCWS_DISPLAY (GSTK_REG_SCWS_EVT)

static gstk_toolkit_cmd_reg_info_type  qmi_cati_scws_reg_list[] =
{
  { { GSTK_REG_CATEGORY_E_CMD, QMI_CAT_GSTK_EVT_MASK_SCWS_DISPLAY}, GSTK_HANDLE_DISPLAY_FUNC_ONLY}
};


/*===========================================================================

                       FORWARD FUNCTION DECLARATIONS

===========================================================================*/


/*===========================================================================

                             LOCAL FUNCTIONS

===========================================================================*/
/*===========================================================================
  FUNCTION QMI_CATI_CONVERT_QMI_CHANNEL_STATE_TO_GSTK_CHANNEL_STATE

  DESCRIPTION
    to get gstk channel state from qmi channel state

  PARAMETERS
    qmi_ch_state
    gstk_ch_state_ptr

  RETURN VALUE
    qmi_error_e_type

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static qmi_error_e_type qmi_cati_convert_qmi_channel_state_to_gstk_channel_state
(
  uint8                                 qmi_ch_state,
  gstk_scws_ch_state_enum_type        * gstk_ch_state_ptr
)
{
  ASSERT (gstk_ch_state_ptr != NULL);

  /* Retrieve channel state */
  switch(qmi_ch_state)
  {
    case CAT_SCWS_CHANNEL_CLOSED_STATE_V02:
      *gstk_ch_state_ptr = GSTK_SCWS_TCP_IN_CLOSED_STATE;
      break;
    case CAT_SCWS_CHANNEL_LISTEN_STATE_V02:
      *gstk_ch_state_ptr = GSTK_SCWS_TCP_IN_LISTEN_STATE;
      break;
    case CAT_SCWS_CHANNEL_ESTABLISHED_STATE_V02:
      *gstk_ch_state_ptr = GSTK_SCWS_TCP_IN_ESTABLISHED_STATE;
      break;
    default:
      UIM_MSG_ERR_1("Invalid qmi channel state : 0x%x", qmi_ch_state);
      return QMI_ERR_INVALID_ARG;
  }
  return QMI_ERR_NONE;
} /* qmi_cati_convert_qmi_channel_state_to_gstk_channel_state*/


/*===========================================================================
  FUNCTION QMI_CATI_SCWS_IND()

  DESCRIPTION
    Is called when scws data event is set in set_event_report becomes true
    Sends an indication to the client

  PARAMETERS
    clid            : client id
    gstk_scws_cmd   : pointer to gstk scws command
    event_id        : SCWS event mask

  RETURN VALUE
    void

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static void qmi_cati_scws_ind
(
  uint8                           clid,
  const gstk_cmd_from_card_type * gstk_scws_cmd,
  uint32                          event_id
)
{
  dsm_item_type                * ind                 = NULL;
  qmi_error_e_type               errval              = QMI_ERR_NONE;
  qmi_cati_cmd_val_e_type        cmd_val             = QMI_CAT_RESET_REQ_V02;
  uint16                         data_len            = 0;
  uint16                         i                   = 0;
  uint32                         num_of_ind          = 0;

  ASSERT(gstk_scws_cmd != NULL);

  ind = NULL;

  if ((event_id & QMI_CAT_DEC_EVT_REPORT_REQ_SCWS_EVENT_MASK_V02) == 0)
  {
    UIM_MSG_ERR_0("Unsupported mask in qmi_cati_scws_ind!");
    return;
  }

  switch (gstk_scws_cmd->hdr_cmd.command_id)
  {
    case GSTK_SCWS_OPEN_CH_REQ:
      cmd_val = CATI_CMD_VAL_SCWS_OPEN_CH;
      break;

    case GSTK_SCWS_CLOSE_CH_REQ:
      cmd_val = CATI_CMD_VAL_SCWS_CLOSE_CH;
      break;

    case GSTK_SCWS_SEND_DATA_REQ:
      cmd_val = CATI_CMD_VAL_SCWS_SEND_DATA;
      break;

    default:
      /* Unhandled GSTK event */
      errval = QMI_ERR_INTERNAL;
      break;
  }

  if(gstk_scws_cmd->hdr_cmd.command_id == GSTK_SCWS_SEND_DATA_REQ)
  {
    data_len = (uint16)gstk_scws_cmd->cmd.scws_send_data_cmd_req.ch_data.data_len;

    /* number of ind to be sent for scws send data */
    num_of_ind = ((data_len % CAT_TLV_SCWS_SEND_DATA_MAX > 0) ? 1 : 0)
                  + data_len / CAT_TLV_SCWS_SEND_DATA_MAX;

    for(i = 0; i < num_of_ind; i++)
    {
      ind = NULL;
      errval = qmi_cat_response_scws_command(&ind,
                                             gstk_scws_cmd,
                                             i * CAT_TLV_SCWS_SEND_DATA_MAX);

      if (errval != QMI_ERR_NONE)
      {
        UIM_MSG_MED_0("Unable to generate decoded Event Report indication!");
        dsm_free_packet(&ind);
        return;
      }

      (void)uimqmi_send_indication(clid, cmd_val, ind, QMUX_SERVICE_CAT);
      UIM_MSG_HIGH_0("qmi_cati_scws_ind : sent scws indication");
    }
  }
  else
  {
    errval = qmi_cat_response_scws_command(&ind, gstk_scws_cmd, 0);

    if (errval != QMI_ERR_NONE)
    {
      UIM_MSG_MED_0("Unable to generate decoded Event Report indication!");
      dsm_free_packet(&ind);
      return;
    }

    (void)uimqmi_send_indication(clid, cmd_val, ind, QMUX_SERVICE_CAT);
    UIM_MSG_HIGH_0("qmi_cati_scws_ind : sent scws indication");
  }
} /* qmi_cati_scws_ind */


/*===========================================================================

                             EXTERNAL FUNCTIONS

===========================================================================*/
/*===========================================================================
  FUNCTION  QMI_CAT_PROCESS_SCWS_COMMAND()

  DESCRIPTION
    To process the scws command from GSTK in QMI context

  PARAMETERS

   scws_cmd : Pointer to scws command

  RETURN VALUE
    None

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
void qmi_cat_process_scws_command
(
  const qmi_cat_message_type * cat_msg_ptr
)
{
  const gstk_cmd_from_card_type      * scws_cmd              = NULL;
  qmi_cati_client_state_type         * cl_sp                 = NULL;
  uint8                                i                     = 0;
  boolean                              ind_sent              = FALSE;
  uint32                               event_id              = 0;
  boolean                              clone_status          = FALSE;
  uint8                                slot_index            = 0;
  gstk_status_enum_type                gstk_status           = GSTK_SUCCESS;
  qmi_cmd_buf_type *                   cmd_buf_p             = NULL;
  dsm_item_type *                      response              = NULL;
  qmi_error_e_type                     errval                = QMI_ERR_NONE;
  qmi_cati_cmd_buf_cache_type        * current_node_ptr      = NULL;
  qmi_cati_cmd_buf_cache_type        * tail_ptr              = NULL;
  uint8                                clid                  = QMI_SVC_CLID_UNUSED;
  uint32                               client_reg_count      = 0;
  qmi_cati_userdata_type             * cb_userdata_ptr       = NULL;

  UIM_MSG_HIGH_0("qmi_cat_process_scws_command");

  ASSERT(cat_msg_ptr != NULL && qmi_cati_global_ptr && qmi_cat_state_ptr);

  scws_cmd = &(cat_msg_ptr->data.scws_cmd.scws_cmd);

  /* Retrieve the slot index */
  if (qmi_cati_convert_gstk_slot_id_to_qmi_slot_index(scws_cmd->hdr_cmd.sim_slot_id,
                                                      &slot_index) != QMI_ERR_NONE)
  {
    UIM_MSG_ERR_2("process_scws_command: fail to covert slot_id, 0x%x for cmd id, 0x%x",
                  scws_cmd->hdr_cmd.sim_slot_id, scws_cmd->hdr_cmd.command_id);
    return;
  }

  /* set last GSTK cmd per slot */
  qmi_cati_global_ptr->last_gstk_cmd[slot_index] = scws_cmd->hdr_cmd.command_id;

  /* Get mask of the scws command */
  switch(scws_cmd->hdr_cmd.command_id)
  {
    case GSTK_SCWS_OPEN_CH_REQ:
    case GSTK_SCWS_CLOSE_CH_REQ:
    case GSTK_SCWS_SEND_DATA_REQ:
      if(qmi_cati_global_ptr->card_state[slot_index] == QMI_CAT_CARD_STATE_PRESENT)
      {
        event_id = QMI_CAT_DEC_EVT_REPORT_REQ_SCWS_EVENT_MASK_V02;
  
        /* loop thru all clients until indication is sent */
        for (i = 0; i < UIMQMI_SVC_MAX_CLIDS && !ind_sent; i++)
        {
          cl_sp = qmi_cat_state_ptr->client[i];
  
          /* check if this service instance is bound to the qmi device */
          if(NULL == cl_sp)
          {
            continue;
          }
  
          if( (cl_sp->common.clid != QMI_SVC_CLID_UNUSED) &&
              (cl_sp->decoded_evt_report_mask[slot_index] & event_id ) )
          {
            qmi_cati_scws_ind(cl_sp->common.clid,
                              scws_cmd,
                              event_id);
            ind_sent = TRUE;
          }
        }
      }

      /* Handle the case where the command is not sent */
      if (!ind_sent)
      {
        /* Don't cache the command if the card state is invalid or error */
        if(qmi_cati_global_ptr->card_state[slot_index] != QMI_CAT_CARD_STATE_INVALID ||
           qmi_cati_global_ptr->card_state[slot_index] != QMI_CAT_CARD_STATE_ERROR)
        {
          UIM_MSG_HIGH_1("QMI-CAT: No client to send indication: 0x%x",
                         scws_cmd->hdr_cmd.command_id);
          if (scws_cmd->hdr_cmd.command_id ==  GSTK_SCWS_OPEN_CH_REQ &&
              qmi_cati_global_ptr->decoded_cache[slot_index][CATI_CACHED_EVT_DECODED_SCWS_OPEN_CHANNEL] == NULL)
          {
            qmi_cati_global_ptr->decoded_cache[slot_index][CATI_CACHED_EVT_DECODED_SCWS_OPEN_CHANNEL] = (qmi_cati_decoded_cache_type *)uimqmi_malloc(
                                                                                                            sizeof(qmi_cati_decoded_cache_type));
            if (qmi_cati_global_ptr->decoded_cache[slot_index][CATI_CACHED_EVT_DECODED_SCWS_OPEN_CHANNEL] == NULL)
            {
              return;
            }
            clone_status = qmi_cati_clone_scws_command(
                 &qmi_cati_global_ptr->decoded_cache[slot_index][CATI_CACHED_EVT_DECODED_SCWS_OPEN_CHANNEL]->cached_event,
                 scws_cmd);
            if (!clone_status)
            {
              uimqmi_free(qmi_cati_global_ptr->decoded_cache[slot_index][CATI_CACHED_EVT_DECODED_SCWS_OPEN_CHANNEL]);
              qmi_cati_global_ptr->decoded_cache[slot_index][CATI_CACHED_EVT_DECODED_SCWS_OPEN_CHANNEL] = NULL;
            }
          }
        }
        if (!clone_status)
        {
          UIM_MSG_HIGH_0("QMI-CAT: Indication not cloned... send open ch rsp");

          /* If the indication is not sent and the event is not cached, send
             error rsp with ch state closed */
          if (scws_cmd->hdr_cmd.command_id == GSTK_SCWS_OPEN_CH_REQ)
          {
            gstk_status = gstk_scws_open_ch_rsp(
                             qmi_cati_global_ptr->cat_gstk_client_id[slot_index],
                             scws_cmd->hdr_cmd.cmd_detail_reference,
                             scws_cmd->cmd.scws_open_ch_cmd_req.ch_id,
                             GSTK_SCWS_TCP_IN_CLOSED_STATE);
            if (gstk_status != GSTK_SUCCESS)
            {
              UIM_MSG_ERR_0("QMI-CAT : Rsp not sent!");
            }
            else
            {
              UIM_MSG_HIGH_1("QMI-CAT : Rsp sent out: 0x%x", gstk_status);
            }
            qmi_cati_global_ptr->last_gstk_cmd[slot_index]  = GSTK_END_PROACTIVE_CMD_REQ;
            break;
          }
        }
      }

      /* Store reference command id for scws request */
      qmi_cati_global_ptr->ref_command_id[slot_index] = scws_cmd->hdr_cmd.cmd_detail_reference;
      break;

    case GSTK_SCWS_DATA_AVAIL_RSP_REQ:
      UIM_MSG_HIGH_0("Sending response for scws data avail rsp");

      current_node_ptr = qmi_cati_global_ptr->scws_cmd_buf[slot_index][scws_cmd->cmd.scws_data_avail_rsp_cmd_req.ch_id];
      if(current_node_ptr == NULL)
      {
        UIM_MSG_ERR_1("GSTK_SCWS_DATA_AVAIL_RSP_REQ : no cmd_buf for ch_id 0x%x in cache",
                      scws_cmd->cmd.scws_data_avail_rsp_cmd_req.ch_id);
        return;
      }

      if(current_node_ptr->next_ptr == NULL)
      {
        /* for case that only 1 pending data avail req for given channel id */
        UIM_MSG_HIGH_0("Pending scws data avail rsp = 1");
        tail_ptr = current_node_ptr;
        qmi_cati_global_ptr->scws_cmd_buf[slot_index][scws_cmd->cmd.scws_data_avail_rsp_cmd_req.ch_id] = NULL;
      }
      else
      {
        tail_ptr = current_node_ptr->next_ptr;
        while (tail_ptr->next_ptr != NULL)
        {
          current_node_ptr = tail_ptr;
          tail_ptr = tail_ptr->next_ptr;
        }
      }

      /* Retrieve userdata */
      cb_userdata_ptr = (qmi_cati_userdata_type *)tail_ptr->cb_userdata_ptr;
      if (cb_userdata_ptr == NULL)
      {
        UIM_MSG_HIGH_0("Null userdata ptr in response");
        return;
      }

      /* Retrieve info */
      clid             = cb_userdata_ptr->clid;
      client_reg_count = cb_userdata_ptr->client_reg_count;
      cmd_buf_p        = (qmi_cmd_buf_type*)cb_userdata_ptr->cmd_buf_ptr;

      /* Free userdata since it is no longer needed */
      qmi_cat_free_cb_userdata(cb_userdata_ptr);
      cb_userdata_ptr = NULL;

      /* Check for client's command buffer pointers */
      if (cmd_buf_p == NULL ||  cmd_buf_p->x_p == NULL || cmd_buf_p->x_p->cl_sp == NULL)
      {
        UIM_MSG_HIGH_0("Null cmd_buf pointers in response");
        return;
      }

      /* Check if the requested client is still valid */
      if (qmi_cat_check_clid(clid, client_reg_count) == FALSE)
      {
        UIM_MSG_HIGH_0("Invalid Client, not sending a response");
        return;
      }

      /* Build the response from scws cmd */
      errval = ((scws_cmd->cmd.scws_data_avail_rsp_cmd_req.success) ? QMI_ERR_NONE
                                                                    : QMI_ERR_INTERNAL);

      /* Update result TLV and Send response */
      uimqmi_put_result_send_response_ind(cmd_buf_p,
                                          response,
                                          errval,
                                          QMUX_SERVICE_CAT,
                                          FALSE,
                                          QMI_SVC_CLID_UNUSED,
                                          (uint16)CATI_CMD_VAL_WIDTH);

      /* free the command buffer */
      UIM_MSG_HIGH_0("Free cached scws data avail rsp");
      current_node_ptr->next_ptr = NULL;
      uimqmi_free(tail_ptr);
      tail_ptr = NULL;
      break;

    default:
      UIM_MSG_HIGH_1("unhandled scws cmd 0x%x", scws_cmd->hdr_cmd.command_id);
      qmi_cati_global_ptr->last_gstk_cmd[slot_index] = GSTK_END_PROACTIVE_CMD_REQ;
      return;
  }
} /* qmi_cat_process_scws_command */


/*===========================================================================
  FUNCTION QMI_CATI_SCWS_OPEN_CHANNEL()

  DESCRIPTION
    Send the confirmation with channel state for scws open channel cmd

  PARAMETERS
    sp            : service provided state pointer (user data)
    cmd_buf_p     : incoming request command buffer
    cl_sp         : client state pointer
    sdu_in        : incoming request

  RETURN VALUE
    ptr to response

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
dsm_item_type * qmi_cati_scws_open_channel
(
  void *              sp,
  void *              cmd_buf_p,
  void *              cl_sp,
  dsm_item_type **    sdu_in
)
{
  dsm_item_type                          * response            = NULL;
  qmi_error_e_type                         errval              = QMI_ERR_NONE;
  boolean                                  retval              = TRUE;
  qmi_result_e_type                        result              = QMI_RESULT_SUCCESS;

  gstk_status_enum_type                    gstk_status         = GSTK_SUCCESS;
  qmi_cat_in_scws_open_channel_state_type  tlv_data;
  uint8                                    slot_index          = 0;
  gstk_scws_ch_state_enum_type             gstk_channel_state  = GSTK_SCWS_TCP_STATE_MAX;

  ASSERT(sp && cmd_buf_p && cl_sp && sdu_in && qmi_cati_global_ptr);

  memset(&tlv_data, 0, sizeof(qmi_cat_in_scws_open_channel_state_type));

  /* Parse TLV */
  errval = qmi_cat_parse_request_scws_open_channel_state(sdu_in, &tlv_data);
  if (errval != QMI_ERR_NONE)
  {
    goto send_result;
  }

  /* Retrieve the slot index */
  if(tlv_data.slot_present)
  {
    errval = qmi_cati_convert_gstk_slot_id_to_qmi_slot_index(
                            (gstk_slot_id_enum_type)tlv_data.slot.slot,
                            &slot_index);
    if (errval != QMI_ERR_NONE)
    {
      goto send_result;
    }
  }

  /* Send command to GSTK */
  if (tlv_data.channel_state_present)
  {
    /* Update channel state TLV */
    errval = qmi_cati_convert_qmi_channel_state_to_gstk_channel_state(
                            tlv_data.channel_state.ch_state,
                            &gstk_channel_state);
    if (errval != QMI_ERR_NONE)
    {
      goto send_result;
    }

    if(tlv_data.channel_state.ch_id >= QMI_CATI_MAX_SCWS_CHANNEL)
    {
      errval = QMI_ERR_INVALID_ARG;
      UIM_MSG_ERR_1("channel id out of range 0x%x",
                    tlv_data.channel_state.ch_id);
      goto send_result;
    }

    gstk_status = gstk_scws_open_ch_rsp(
                           qmi_cati_global_ptr->cat_gstk_client_id[slot_index],
                           qmi_cati_global_ptr->ref_command_id[slot_index],
                           tlv_data.channel_state.ch_id,
                           gstk_channel_state);
  }
  else
  {
    UIM_MSG_HIGH_1(" qmi_cati_scws_open_channel : channel state present = 0x%x,",
                   tlv_data.channel_state_present);
    errval = QMI_ERR_MISSING_ARG;
    goto send_result;
  }

  if (gstk_status != GSTK_SUCCESS)
  {
    UIM_MSG_HIGH_1("qmi_cati_scws_open_channel : gstk_status = status %d",
                   gstk_status);
    errval = qmi_cati_map_gstk_status_to_errval(gstk_status);
  }


send_result:
  result = (errval == QMI_ERR_NONE ? QMI_RESULT_SUCCESS
                                   : QMI_RESULT_FAILURE);

  retval = qmi_svc_put_result_tlv(&response, result, errval);
  CHECK_RETVAL();

  return response;
} /* qmi_cati_scws_open_channel() */


/*===========================================================================
  FUNCTION QMI_CATI_SCWS_CLOSE_CHANNEL()

  DESCRIPTION
    Send the confirmation with channel state for scws close channel cmd

  PARAMETERS
    sp            : service provided state pointer (user data)
    cmd_buf_p     : incoming request command buffer
    cl_sp         : client state pointer
    sdu_in        : incoming request

  RETURN VALUE
    ptr to response

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
dsm_item_type * qmi_cati_scws_close_channel
(
  void *              sp,
  void *              cmd_buf_p,
  void *              cl_sp,
  dsm_item_type **    sdu_in
)
{
  dsm_item_type                           * response            = NULL;
  qmi_error_e_type                          errval              = QMI_ERR_NONE;
  boolean                                   retval              = TRUE;
  qmi_result_e_type                         result              = QMI_RESULT_SUCCESS;

   gstk_status_enum_type                     gstk_status         = GSTK_SUCCESS;
  qmi_cat_in_scws_close_channel_state_type  tlv_data;
  uint8                                     slot_index          = 0;
  gstk_scws_ch_state_enum_type              gstk_channel_state  = GSTK_SCWS_TCP_STATE_MAX;

  ASSERT(sp && cmd_buf_p && cl_sp && sdu_in && qmi_cati_global_ptr);

  memset(&tlv_data, 0, sizeof(qmi_cat_in_scws_close_channel_state_type));

  /* Parse TLV */
  errval = qmi_cat_parse_request_scws_close_channel_state(sdu_in, &tlv_data);
  if (errval != QMI_ERR_NONE)
  {
    goto send_result;
  }

  UIM_MSG_HIGH_2(" qmi_cati_scws_close_channel : channel_state_present = 0x%x, slot_present = 0x%x",
                 tlv_data.channel_state_present, tlv_data.slot_present);

  /* Retrieve the slot index */
  if(tlv_data.slot_present)
  {
    errval = qmi_cati_convert_gstk_slot_id_to_qmi_slot_index(
                            (gstk_slot_id_enum_type)tlv_data.slot.slot,
                            &slot_index);
    if (errval != QMI_ERR_NONE)
    {
      goto send_result;
    }
  }

  /* Send command to GSTK */
  if (tlv_data.channel_state_present)
  {
    /* Update channel state TLV */
    errval = qmi_cati_convert_qmi_channel_state_to_gstk_channel_state(
                            tlv_data.channel_state.ch_state,
                            &gstk_channel_state);
    if (errval != QMI_ERR_NONE)
    {
      goto send_result;
    }

    if(tlv_data.channel_state.ch_id >= QMI_CATI_MAX_SCWS_CHANNEL)
    {
      errval = QMI_ERR_INVALID_ARG;
      UIM_MSG_ERR_1("channel id out of range 0x%x",
                    tlv_data.channel_state.ch_id);
      goto send_result;
    }

    gstk_status = gstk_scws_close_ch_rsp(
                           qmi_cati_global_ptr->cat_gstk_client_id[slot_index],
                           qmi_cati_global_ptr->ref_command_id[slot_index],
                           tlv_data.channel_state.ch_id,
                           gstk_channel_state);
  }
  else
  {
    errval = QMI_ERR_MISSING_ARG;
    goto send_result;
  }

  if (gstk_status != GSTK_SUCCESS)
  {
    errval = qmi_cati_map_gstk_status_to_errval(gstk_status);
  }

send_result:
  result = (errval == QMI_ERR_NONE ? QMI_RESULT_SUCCESS
                                   : QMI_RESULT_FAILURE);

  retval = qmi_svc_put_result_tlv(&response, result, errval);
  CHECK_RETVAL();

  return response;
} /* qmi_cati_scws_close_channel() */


/*===========================================================================
  FUNCTION QMI_CATI_SCWS_CHANGE_STATUS()

  DESCRIPTION
    Send the confirmation with channel state for scws change status cmd

  PARAMETERS
    sp            : service provided state pointer (user data)
    cmd_buf_p     : incoming request command buffer
    cl_sp         : client state pointer
    sdu_in        : incoming request

  RETURN VALUE
    ptr to response

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
dsm_item_type * qmi_cati_scws_change_status
(
  void *              sp,
  void *              cmd_buf_p,
  void *              cl_sp,
  dsm_item_type **    sdu_in
)
{
  dsm_item_type                             * response            = NULL;
  qmi_error_e_type                            errval              = QMI_ERR_NONE;
  boolean                                     retval              = TRUE;
  qmi_result_e_type                           result              = QMI_RESULT_SUCCESS;

  gstk_status_enum_type                       gstk_status         = GSTK_SUCCESS;
  qmi_cat_in_scws_channel_status_state_type   tlv_data;
  uint8                                       slot_index          = 0;
  gstk_scws_ch_state_enum_type                gstk_channel_state  = GSTK_SCWS_TCP_STATE_MAX;

  ASSERT(sp && cmd_buf_p && cl_sp && sdu_in && qmi_cati_global_ptr);

  memset(&tlv_data, 0, sizeof(qmi_cat_in_scws_channel_status_state_type));

  /* Parse TLV */
  errval = qmi_cat_parse_request_scws_channel_status_state(sdu_in, &tlv_data);
  if (errval != QMI_ERR_NONE)
  {
    goto send_result;
  }

  UIM_MSG_HIGH_2(" qmi_cati_scws_close_channel : channel_state_present = 0x%x, slot_present = 0x%x",
                 tlv_data.channel_state_present, tlv_data.slot_present);

  /* Retrieve the slot index */
  if(tlv_data.slot_present)
  {
    errval = qmi_cati_convert_gstk_slot_id_to_qmi_slot_index(
                            (gstk_slot_id_enum_type)tlv_data.slot.slot,
                            &slot_index);
    if (errval != QMI_ERR_NONE)
    {
      goto send_result;
    }
  }

  /* Send command to GSTK */
  if (tlv_data.channel_state_present)
  {
    /* Update channel state TLV */
    errval = qmi_cati_convert_qmi_channel_state_to_gstk_channel_state(
                            tlv_data.channel_state.ch_state,
                            &gstk_channel_state);
    if (errval != QMI_ERR_NONE)
    {
      goto send_result;
    }

    if(tlv_data.channel_state.ch_id >= QMI_CATI_MAX_SCWS_CHANNEL)
    {
      errval = QMI_ERR_INVALID_ARG;
      UIM_MSG_ERR_1("channel id out of range 0x%x",
                    tlv_data.channel_state.ch_id);
      goto send_result;
    }

    gstk_status = gstk_scws_ch_status_req(
                           qmi_cati_global_ptr->cat_gstk_client_id[slot_index],
                           tlv_data.channel_state.ch_id,
                           gstk_channel_state);
  }
  else
  {
    errval = QMI_ERR_MISSING_ARG;
    goto send_result;
  }

  if (gstk_status != GSTK_SUCCESS)
  {
    errval = qmi_cati_map_gstk_status_to_errval(gstk_status);
  }

send_result:
  result = (errval == QMI_ERR_NONE ? QMI_RESULT_SUCCESS
                                   : QMI_RESULT_FAILURE);

  retval = qmi_svc_put_result_tlv(&response, result, errval);
  CHECK_RETVAL();

  return response;
} /* qmi_cati_scws_change_status() */


/*===========================================================================
  FUNCTION QMI_CATI_SCWS_SEND_DATA()

  DESCRIPTION
    Send the confirmation with channel state for scws send data cmd

  PARAMETERS
    sp            : service provided state pointer (user data)
    cmd_buf_p     : incoming request command buffer
    cl_sp         : client state pointer
    sdu_in        : incoming request

  RETURN VALUE
    ptr to response

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
dsm_item_type * qmi_cati_scws_send_data
(
  void *              sp,
  void *              cmd_buf_p,
  void *              cl_sp,
  dsm_item_type **    sdu_in
)
{
  dsm_item_type                     * response     = NULL;
  qmi_error_e_type                    errval       = QMI_ERR_NONE;
  boolean                             retval       = TRUE;
  qmi_result_e_type                   result       = QMI_RESULT_SUCCESS;

   gstk_status_enum_type               gstk_status  = GSTK_SUCCESS;
  qmi_cat_in_scws_send_data_type      tlv_data;
  uint8                               slot_index   = 0;

  ASSERT(sp && cmd_buf_p && cl_sp && sdu_in && qmi_cati_global_ptr);

  memset(&tlv_data, 0, sizeof(qmi_cat_in_scws_send_data_type));

  /* Parse TLV */
  errval = qmi_cat_parse_request_scws_send_data(sdu_in, &tlv_data);
  if (errval != QMI_ERR_NONE)
  {
    goto send_result;
  }

  UIM_MSG_HIGH_2("qmi_cati_scws_close_channel : channel_data_result_present = 0x%x, slot_present = 0x%x",
                 tlv_data.channel_data_result_present, tlv_data.slot_present);

  /* Retrieve the slot index */
  if(tlv_data.slot_present)
  {
    errval = qmi_cati_convert_gstk_slot_id_to_qmi_slot_index(
                            (gstk_slot_id_enum_type)tlv_data.slot.slot,
                            &slot_index);
    if (errval != QMI_ERR_NONE)
    {
      goto send_result;
    }
  }

  /* Send command to GSTK */
  if (tlv_data.channel_data_result_present)
  {
    if(tlv_data.channel_data_result.ch_id >= QMI_CATI_MAX_SCWS_CHANNEL)
    {
      errval = QMI_ERR_INVALID_ARG;
      UIM_MSG_ERR_1("channel id out of range 0x%x",
                    tlv_data.channel_data_result.ch_id);
      goto send_result;
    }

    gstk_status = gstk_scws_send_data_rsp(
                           qmi_cati_global_ptr->cat_gstk_client_id[slot_index],
                           qmi_cati_global_ptr->ref_command_id[slot_index],
                           tlv_data.channel_data_result.ch_id,
                           tlv_data.channel_data_result.result);
  }
  else
  {
    errval = QMI_ERR_MISSING_ARG;
    goto send_result;
  }

  if (gstk_status != GSTK_SUCCESS)
  {
    errval = qmi_cati_map_gstk_status_to_errval(gstk_status);
  }

send_result:
  result = (errval == QMI_ERR_NONE ? QMI_RESULT_SUCCESS
                                   : QMI_RESULT_FAILURE);

  retval = qmi_svc_put_result_tlv(&response, result, errval);
  CHECK_RETVAL();

  return response;
} /* qmi_cati_scws_send_data() */


/*===========================================================================
  FUNCTION QMI_CATI_SCWS_DATA_AVAILABLE()

  DESCRIPTION
    Send the confirmation with channel state for scws data available cmd

  PARAMETERS
    sp            : service provided state pointer (user data)
    cmd_buf_p     : incoming request command buffer
    cl_sp         : client state pointer
    sdu_in        : incoming request

  RETURN VALUE
    ptr to response

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
dsm_item_type * qmi_cati_scws_data_available
(
  void *              sp,
  void *              cmd_buf_p,
  void *              cl_sp,
  dsm_item_type **    sdu_in
)
{
  dsm_item_type                     * response                  = NULL;
  qmi_error_e_type                    errval                    = QMI_ERR_NONE;
  boolean                             retval                    = TRUE;
  qmi_result_e_type                   result                    = QMI_RESULT_SUCCESS;

   gstk_status_enum_type               gstk_status               = GSTK_SUCCESS;
  qmi_cat_in_scws_data_available_type tlv_data;
  gstk_generic_data_type              channel_data;
  uint8                               slot_index                = 0;
  qmi_cati_cmd_buf_cache_type       * new_node_ptr              = NULL;
  qmi_cati_userdata_type            * cb_userdata_ptr           = NULL;
  qmi_cati_ind_token_info_type        ind_token                 = {FALSE, 0};

  ASSERT(sp && cmd_buf_p && cl_sp && sdu_in && qmi_cati_global_ptr);

  memset(&tlv_data, 0, sizeof(qmi_cat_in_scws_data_available_type));
  memset(&channel_data, 0, sizeof(gstk_generic_data_type));

  /* Parse TLV */
  errval = qmi_cat_parse_request_scws_data_available(sdu_in, &tlv_data);
  if (errval != QMI_ERR_NONE)
  {
    goto send_result;
  }

  UIM_MSG_HIGH_2(" qmi_cati_scws_close_channel : channel_data_present = 0x%x, slot_present = 0x%x",
                 tlv_data.channel_data_present, tlv_data.slot_present);

  /* Send Data Available channel data */
  channel_data.data_len = tlv_data.channel_data.data_len;
  channel_data.data_buffer_ptr = (uint8 *) &(tlv_data.channel_data.data[0]);

  /* Retrieve the slot index */
  if(tlv_data.slot_present)
  {
    errval = qmi_cati_convert_gstk_slot_id_to_qmi_slot_index(
                            (gstk_slot_id_enum_type)tlv_data.slot.slot,
                            &slot_index);
    if (errval != QMI_ERR_NONE)
    {
      goto send_result;
    }
  }

  /* Send command to GSTK */
  if (tlv_data.channel_data_present)
  {
    if(tlv_data.channel_data.ch_id >= QMI_CATI_MAX_SCWS_CHANNEL)
    {
      errval = QMI_ERR_INVALID_ARG;
      UIM_MSG_ERR_1("channel id out of range 0x%x",
                    tlv_data.channel_data.ch_id);
      goto send_result;
    }

    gstk_status = gstk_scws_data_avail_req_type(
                           qmi_cati_global_ptr->cat_gstk_client_id[slot_index],
                           tlv_data.channel_data.ch_id,
                           channel_data,
                           tlv_data.remain_data_len.data_len);
  }
  else
  {
    errval = QMI_ERR_MISSING_ARG;
    goto send_result;
  }

  if (gstk_status != GSTK_SUCCESS)
  {
    errval = qmi_cati_map_gstk_status_to_errval(gstk_status);
    goto send_result;
  }

  /* Caching scws data avail req command buffer*/
  new_node_ptr = uimqmi_malloc(sizeof(qmi_cati_cmd_buf_cache_type));

  if (new_node_ptr == NULL)
  {
    errval = QMI_ERR_NO_MEMORY;
    goto send_result;
  }

  /* Create the userdata */
  cb_userdata_ptr = qmi_cat_get_cb_userdata(CATI_CMD_VAL_SCWS_DATA_AVAILABLE, cmd_buf_p, ind_token);
  if (cb_userdata_ptr == NULL)
  {
    errval = QMI_ERR_NO_MEMORY;
    goto send_result;
  }

  memset(new_node_ptr, 0, sizeof(qmi_cati_cmd_buf_cache_type));
  new_node_ptr->next_ptr = qmi_cati_global_ptr->scws_cmd_buf[slot_index][tlv_data.channel_data.ch_id];
  new_node_ptr->cb_userdata_ptr = cb_userdata_ptr;

  qmi_cati_global_ptr->scws_cmd_buf[slot_index][tlv_data.channel_data.ch_id] = new_node_ptr;

  return QMI_SVC_RESPONSE_PENDING;

send_result:
  result = (errval == QMI_ERR_NONE ? QMI_RESULT_SUCCESS
                                   : QMI_RESULT_FAILURE);

  retval = qmi_svc_put_result_tlv(&response, result, errval);
  CHECK_RETVAL();

  return response;
} /* qmi_cati_scws_data_available() */


/*===========================================================================
FUNCTION: qmi_cati_scws_cmd_reg

DESCRIPTION:
  SCWS event registration.

PARAMETERS:
  slot_index   : slot index
  client_cmd_cb: GSTK client command callback
  client_evt_cb: GSTK client event callback

DEPENDENCIES:
  None

RETURN VALUE:
  None

COMMENTS:
  None

SIDE EFFECTS:
  None
===========================================================================*/
void qmi_cati_scws_cmd_reg
(
  uint8                            slot_index,
  gstk_reg_rsp_cmd_cb_func_type    client_cmd_cb,
  gstk_toolkit_evt_cb_func_type    client_evt_cb
)
{
  gstk_status_enum_type  gstk_status = GSTK_SUCCESS;

  gstk_status = gstk_client_toolkit_cmd_reg(
                        qmi_cati_global_ptr->cat_gstk_client_id[slot_index],
                        0x00,
                        GSTK_GSTK_FORMAT,
                        sizeof(qmi_cati_scws_reg_list)/sizeof(gstk_toolkit_cmd_reg_info_type),
                        qmi_cati_scws_reg_list,
                        client_cmd_cb,
                        client_evt_cb);

  if (GSTK_SUCCESS != gstk_status)
  {
    UIM_MSG_LOW_1("QMI-CAT gstk reg for scws failed 0x%x", gstk_status);
  }
}/* qmi_cati_scws_cmd_reg */


/*===========================================================================
  FUNCTION QMI_CATI_CLONE_SCWS_COMMAND

  DESCRIPTION
    Makes a deep copy of a scws command

  PARAMETERS
    gstk_cmd_from_card_type: source procative command
    gstk_cmd_from_card_type: destination procative command

  RETURN VALUE
    boolean: result of the copy

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
boolean qmi_cati_clone_scws_command
(
  gstk_cmd_from_card_type       * dst,
  const gstk_cmd_from_card_type * src
)
{
  ASSERT(src != NULL && dst != NULL);

  /* Copy header */
  (void)memscpy(&dst->hdr_cmd,
                sizeof(gstk_exp_hdr_type),
                &src->hdr_cmd,
                sizeof(gstk_exp_hdr_type));

  switch(src->hdr_cmd.command_id)
  {
    case GSTK_SCWS_OPEN_CH_REQ:
      (void)memscpy(&dst->cmd.scws_open_ch_cmd_req,
                    sizeof(gstk_scws_open_ch_req_type),
                    &src->cmd.scws_open_ch_cmd_req,
                    sizeof(gstk_scws_open_ch_req_type));
      /* alphabet is type uint16, byte size for copying is alphabet.length * 2 */
      uimqmi_deep_copy((void**)&dst->cmd.scws_open_ch_cmd_req.alpha.alphabet,
                       (void*)src->cmd.scws_open_ch_cmd_req.alpha.alphabet,
                       (src->cmd.scws_open_ch_cmd_req.alpha.length * 2));
      break;
    case GSTK_SCWS_CLOSE_CH_REQ:
      (void)memscpy(&dst->cmd.scws_close_ch_cmd_req,
                    sizeof(gstk_scws_close_ch_req_type),
                    &src->cmd.scws_close_ch_cmd_req,
                    sizeof(gstk_scws_close_ch_req_type));
      break;
    case GSTK_SCWS_SEND_DATA_REQ:
      (void)memscpy(&dst->cmd.scws_send_data_cmd_req,
                    sizeof(gstk_scws_send_data_req_type),
                    &src->cmd.scws_send_data_cmd_req,
                    sizeof(gstk_scws_send_data_req_type));
      uimqmi_deep_copy((void**)&dst->cmd.scws_send_data_cmd_req.ch_data.data,
                       (void*)src->cmd.scws_send_data_cmd_req.ch_data.data,
                       src->cmd.scws_send_data_cmd_req.ch_data.data_len);
      break;
    case GSTK_SCWS_DATA_AVAIL_RSP_REQ:
      (void)memscpy(&dst->cmd.scws_data_avail_rsp_cmd_req,
                    sizeof(gstk_scws_data_avail_rsp_req_type),
                    &src->cmd.scws_data_avail_rsp_cmd_req,
                    sizeof(gstk_scws_data_avail_rsp_req_type));
      break;
    default:
      UIM_MSG_ERR_1("QMI_CAT: Invalid proactive command: 0x%x",
                    src->hdr_cmd.command_id);
      return FALSE;
  }

  return TRUE;
} /* qmi_cati_clone_scws_command */


/*===========================================================================
  FUNCTION QMI_CATI_FREE_SCWS_COMMAND

  DESCRIPTION
    Makes a deep copy of a proactive command in decoded format

  PARAMETERS
    gstk_cmd_from_card_type: source procative command
    gstk_cmd_from_card_type: destination procative command

  RETURN VALUE
    boolean: result of the copy

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
void qmi_cati_free_scws_command
(
  gstk_cmd_from_card_type       * command
)
{
  ASSERT(command != NULL);

  switch(command->hdr_cmd.command_id)
  {
    case GSTK_SCWS_CLOSE_CH_REQ:
    case GSTK_SCWS_DATA_AVAIL_RSP_REQ:
      break;
    case GSTK_SCWS_OPEN_CH_REQ:
      QMI_CAT_FREE_IF_NOT_NULL(command->cmd.scws_open_ch_cmd_req.alpha.alphabet);
      break;
    case GSTK_SCWS_SEND_DATA_REQ:
      QMI_CAT_FREE_IF_NOT_NULL(command->cmd.scws_send_data_cmd_req.ch_data.data);
      break;
    default:
      UIM_MSG_ERR_1("QMI_CAT_SCWS: Invalid proactive command: 0x%x",
                    command->hdr_cmd.command_id);
      break;
  }

  /* Zero the command, as it's no longer required */
  memset(command, 0, sizeof(gstk_cmd_from_card_type));
} /* qmi_cati_free_scws_command */


/*===========================================================================
FUNCTION: QMI_CAT_SEND_BUFFERED_SCWS_CMD

DESCRIPTION:
  QMI CAT send buffered commands at the given index to the control point that
  registers for the event

PARAMETERS:
  slot_index  : Index to card slot

DEPENDENCIES:
  None

RETURN VALUE:
  None

COMMENTS:
  None

SIDE EFFECTS:

===========================================================================*/
void qmi_cat_send_buffered_scws_cmd
(
  uint8  slot_index
)
{
  qmi_cat_message_type  * msg_ptr = NULL;

  ASSERT(qmi_cati_global_ptr);

  if (slot_index  >= QMI_CAT_MAX_CARD_COUNT ||
      slot_index  >= qmi_cati_global_ptr->slot_count ||
      CATI_CACHED_EVT_DECODED_SCWS_OPEN_CHANNEL >= CATI_MAX_DECODED_CACHED_EVTS)
  {
    UIM_MSG_ERR_2("Invalid slot_index: 0x%x or cache_index: 0x%x",
                  slot_index, CATI_CACHED_EVT_DECODED_SCWS_OPEN_CHANNEL);
    return;
  }

  if (NULL == qmi_cati_global_ptr->decoded_cache[slot_index][CATI_CACHED_EVT_DECODED_SCWS_OPEN_CHANNEL])
  {
    return;
  }

  msg_ptr = qmi_cat_create_message(QMI_CAT_MESSAGE_SCWS_COMMAND);

  if (NULL == msg_ptr)
  {
    return;
  }

  msg_ptr->data.decoded_proactive_cmd.is_cached_cmd = TRUE;

  /* Clone decoded proactive command */
  (void)qmi_cati_clone_scws_command(
      &(msg_ptr->data.decoded_proactive_cmd.decoded_cmd),
      &qmi_cati_global_ptr->decoded_cache[slot_index][CATI_CACHED_EVT_DECODED_SCWS_OPEN_CHANNEL]->cached_event);
  qmi_cat_post_message(msg_ptr);

  /* Free the cache */
  qmi_cati_free_scws_command(
      &qmi_cati_global_ptr->decoded_cache[slot_index][CATI_CACHED_EVT_DECODED_SCWS_OPEN_CHANNEL]->cached_event);
  uimqmi_free(qmi_cati_global_ptr->decoded_cache[slot_index][CATI_CACHED_EVT_DECODED_SCWS_OPEN_CHANNEL]);
  qmi_cati_global_ptr->decoded_cache[slot_index][CATI_CACHED_EVT_DECODED_SCWS_OPEN_CHANNEL] = NULL;
}/* qmi_cat_send_buffered_scws_cmd */


/*===========================================================================
FUNCTION: qmi_cat_cleanup_scws_globals

DESCRIPTION:
  This function is used to free the qmicat globals

PARAMETERS:
  card_index: slot index

DEPENDENCIES:
  None

RETURN VALUE:
  None

COMMENTS:
  None

SIDE EFFECTS:
  None
===========================================================================*/
void qmi_cat_cleanup_scws_globals
(
  uint8 card_index
)
{
  uint8                  cache_index     = 0;

  if(qmi_cati_global_ptr->decoded_cache[card_index][CATI_CACHED_EVT_DECODED_SCWS_OPEN_CHANNEL] != NULL)
  {
    qmi_cati_free_scws_command(
        &qmi_cati_global_ptr->decoded_cache[card_index][CATI_CACHED_EVT_DECODED_SCWS_OPEN_CHANNEL]->cached_event);
    uimqmi_free(qmi_cati_global_ptr->decoded_cache[card_index][CATI_CACHED_EVT_DECODED_SCWS_OPEN_CHANNEL]);
    qmi_cati_global_ptr->decoded_cache[card_index][CATI_CACHED_EVT_DECODED_SCWS_OPEN_CHANNEL] = NULL;
  }

  for(cache_index = 0; cache_index < QMI_CATI_MAX_SCWS_CHANNEL; cache_index++)
  {
    qmi_cati_cmd_buf_cache_type *current_node_ptr = NULL;

    current_node_ptr = qmi_cati_global_ptr->scws_cmd_buf[card_index][cache_index];
    while (current_node_ptr != NULL)
    {
      qmi_cati_cmd_buf_cache_type *next_node_ptr = NULL;

      next_node_ptr = current_node_ptr->next_ptr;
      qmi_cat_free_cb_userdata((qmi_cati_userdata_type *)current_node_ptr->cb_userdata_ptr);
      uimqmi_free(current_node_ptr);
      current_node_ptr = next_node_ptr;
    }
    qmi_cati_global_ptr->scws_cmd_buf[card_index][cache_index] = NULL;
  }
} /* qmi_cat_cleanup_scws_globals */
