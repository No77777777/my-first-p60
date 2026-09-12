/*==========================================================================

DESCRIPTION
Handles QMI CAT SCWS parsing


Copyright (c) 2015 - 2016 QUALCOMM Technologies, Inc(QTI).
All Rights Reserved.
QUALCOMM Technologies Confidential and Proprietary

  $Header: //components/rel/uim.mpss/6.1.0/uimqmi/src/qmi_cat_scws_parsing.c#2 $ $DateTime: 2019/10/30 06:56:05 $ $Author: pwbldsvc $

when        who    what, where, why
--------    ---    ----------------------------------------------------------
11/04/16    me     Combined multi-line ASSERT in one line
09/08/15    vr     Initial version
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
#include "qmi_cat_parsing.h"
#include "qmi_cat_scws_parsing.h"

#include "uim_msg.h"
#include "card_application_toolkit_v02.h"

/* These includes need to always be at bottom - offtarget use only */
#ifdef FEATURE_UIM_TEST_FRAMEWORK
#error code not present
#endif /* FEATURE_UIM_TEST_FRAMEWORK */

/*===========================================================================

                               INTERNAL DATA

===========================================================================*/
/*---------------------------------------------------------------------------
  Tags used for scws commands in decoded format
---------------------------------------------------------------------------*/
#define CATI_PRM_TYPE_SCWS_OPEN_CHANNEL_INFO          (0x10)
#define CATI_PRM_TYPE_SCWS_SLOT_ID                    (0x11)
#define CATI_PRM_TYPE_SCWS_ALPHA                      (0x12)
#define CATI_PRM_TYPE_SCWS_CHANNEL_STATE              (0x10)
#define CATI_PRM_TYPE_SCWS_CHANNEL_DATA               (0x10)

/*---------------------------------------------------------------------------
  TLV tag ids for QMI_CAT_SCWS_OPEN_CHANNEL
  TLV tag ids for QMI_CAT_SCWS_CLOSE_CHANNEL
  TLV tag ids for QMI_CAT_SCWS_CHANNEL_STATUS
---------------------------------------------------------------------------*/
#define CATI_SCWS_CHANNEL_STATE_TLV                   (0x01)
#define CATI_SCWS_CHANNEL_STATE_SLOT_TLV              (0x10)

/*---------------------------------------------------------------------------
  TLV tag ids for QMI_CAT_SCWS_SEND_DATA
---------------------------------------------------------------------------*/
#define CATI_SCWS_SEND_DATA_CHANNEL_DATA_RESULT_TLV   (0x01)
#define CATI_SCWS_SEND_DATA_SLOT_TLV                  (0x10)

/*---------------------------------------------------------------------------
  TLV tag ids for QMI_CAT_SCWS_DATA_AVAILABLE
---------------------------------------------------------------------------*/
#define CATI_SCWS_DATA_AVAILABLE_CHANNEL_DATA_TLV     (0x01)
#define CATI_SCWS_DATA_AVAILABLE_REMAIN_DATA_LEN_TLV  (0x02)
#define CATI_SCWS_DATA_AVAILABLE_SLOT_TLV             (0x10)

/*===========================================================================

                       FORWARD FUNCTION DECLARATIONS

===========================================================================*/


/*===========================================================================

                             LOCAL FUNCTIONS

===========================================================================*/
/*===========================================================================
  FUNCTION QMI_CATI_SCWS_OPEN_CHANNEL_INFO_TLV

  DESCRIPTION
    to send the scws open channel info tlv

  PARAMETERS
    gstk_ch_id       : CHANNEL ID
    gstk_port        : PORT NUMBER
    gstk_buffer_size : BUFFER SIZE
    ind              : TLV

  RETURN VALUE
    qmi_error_e_type

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static qmi_error_e_type qmi_cati_scws_open_channel_info_tlv
(
  uint32            gstk_ch_id,
  uint32            gstk_port,
  int32             gstk_buffer_size,
  dsm_item_type  ** ind
)
{
  PACKED struct PACKED_POST
  {
    uint32 ch_id;
    uint16 port;
    uint16 buffer_size;
  } open_ch_info_tlv;

  open_ch_info_tlv.ch_id = gstk_ch_id;
  open_ch_info_tlv.port = (uint16)gstk_port;
  open_ch_info_tlv.buffer_size = (uint16)gstk_buffer_size;

  if( FALSE == qmi_svc_put_param_tlv(ind,
                                     CATI_PRM_TYPE_SCWS_OPEN_CHANNEL_INFO,
                                     sizeof(open_ch_info_tlv),
                                     (void *)&open_ch_info_tlv) )
  {
    UIM_MSG_MED_0("Unable to send CATI_PRM_TYPE_SCWS_OPEN_CHANNEL_INFO!");
    return QMI_ERR_NO_MEMORY;
  }

  return QMI_ERR_NONE;
} /* qmi_cati_scws_open_channel_info_tlv */


/*===========================================================================
  FUNCTION QMI_CATI_SCWS_CHANNEL_STATE_TLV

  DESCRIPTION
    to send the scws channel state tlv

  PARAMETERS
    gstk_ch_id       : CHANNEL ID
    gstk_state       : CHANNEL STATE
    ind              : tlv

  RETURN VALUE
    qmi_error_e_type

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static qmi_error_e_type qmi_cati_scws_channel_state_tlv
(
  uint32                        gstk_ch_id,
  gstk_scws_ch_state_enum_type  gstk_state,
  dsm_item_type              ** ind
)
{
  PACKED struct PACKED_POST
  {
    uint32 ch_id;
    uint8  state;
  } channel_state_tlv;

  channel_state_tlv.ch_id = gstk_ch_id;

  switch(gstk_state)
  {
    case GSTK_SCWS_TCP_IN_CLOSED_STATE:
      channel_state_tlv.state = CAT_SCWS_CHANNEL_CLOSED_STATE_V02;
      break;

    case GSTK_SCWS_TCP_IN_LISTEN_STATE:
      channel_state_tlv.state = CAT_SCWS_CHANNEL_LISTEN_STATE_V02;
      break;

    case GSTK_SCWS_TCP_IN_ESTABLISHED_STATE:
      channel_state_tlv.state = CAT_SCWS_CHANNEL_ESTABLISHED_STATE_V02;
      break;

    default:
      return QMI_ERR_INTERNAL;
  }
  if( FALSE == qmi_svc_put_param_tlv(ind,
                                     CATI_PRM_TYPE_SCWS_CHANNEL_STATE,
                                     sizeof(channel_state_tlv),
                                     (void *)&channel_state_tlv) )
  {
    UIM_MSG_MED_0("Unable to send CATI_PRM_TYPE_SCWS_CHANNEL_STATE!");
    return QMI_ERR_NO_MEMORY;
  }

  return QMI_ERR_NONE;
} /* qmi_cati_scws_channel_state_tlv */


/*===========================================================================
  FUNCTION QMI_CATI_SCWS_CHANNEL_DATA_TLV

  DESCRIPTION
    to send the scws channel data tlv

  PARAMETERS
    gstk_ch_id          : CHANNEL ID
    gstk_channel_data   : CHANNEL DATA
    ind                 : tlv

  RETURN VALUE
    qmi_error_e_type

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static qmi_error_e_type qmi_cati_scws_channel_data_tlv
(
  uint32                        gstk_ch_id,
  gstk_ch_data_type             gstk_channel_data,
  uint16                        data_index,
  dsm_item_type              ** ind
)
{
  PACKED struct PACKED_POST
  {
    uint32          ch_id;
    uint8           total_pkg;
    uint8           current_pkg;
    uint16          data_len;
    uint8           data[CAT_TLV_SCWS_SEND_DATA_MAX];
  } ch_data_tlv;

  ch_data_tlv.ch_id = gstk_ch_id;
  ch_data_tlv.total_pkg = (uint8) (gstk_channel_data.data_len / CAT_TLV_SCWS_SEND_DATA_MAX
                          + (((gstk_channel_data.data_len % CAT_TLV_SCWS_SEND_DATA_MAX) > 0) ? 1 : 0));
  ch_data_tlv.current_pkg = (uint8)(data_index / CAT_TLV_SCWS_SEND_DATA_MAX + 1);
  if(gstk_channel_data.data_len == 0)
  {
    ch_data_tlv.data_len = (uint16) gstk_channel_data.data_len;
  }
  if ((gstk_channel_data.data_len - data_index) > CAT_TLV_SCWS_SEND_DATA_MAX)
  {
    ch_data_tlv.data_len = CAT_TLV_SCWS_SEND_DATA_MAX;
  }
  else
  {
    ch_data_tlv.data_len = (uint16) gstk_channel_data.data_len - data_index;
  }

  if (ch_data_tlv.data_len> CAT_TLV_SCWS_SEND_DATA_MAX)
  {
    UIM_MSG_HIGH_0("qmi_cati_scws_channel_data_tlv: invalid data len!");
    return QMI_ERR_NO_MEMORY;
  }

  if(ch_data_tlv.data_len > 0)
  {
    (void)memscpy((void*)&ch_data_tlv.data[0],
                  sizeof(ch_data_tlv.data),
                  &gstk_channel_data.data[data_index],
                  ch_data_tlv.data_len);
  }

  if( FALSE == qmi_svc_put_param_tlv(ind,
                                     CATI_PRM_TYPE_SCWS_CHANNEL_DATA,
                                     sizeof(uint32) + sizeof(uint16) + 2 * sizeof(uint8) + ch_data_tlv.data_len,
                                     (void *)&ch_data_tlv) )
  {
    UIM_MSG_MED_0("Unable to send CATI_PRM_TYPE_SCWS_CHANNEL_DATA!");
    return QMI_ERR_NO_MEMORY;
  }

  return QMI_ERR_NONE;
} /* qmi_cati_scws_channel_data_tlv */


/*===========================================================================
  FUNCTION QMI_CATI_POPULATE_SCWS_OPEN_CHANNEL_COMMAND()

  DESCRIPTION
    to populate the tlvs from the send data proactive command

  PARAMETERS
    scws_open_ch_cmd     : Decoded scws command
    resp                 : pointer to tlv

  RETURN VALUE
    qmi_error_e_type

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static qmi_error_e_type qmi_cati_populate_scws_open_channel_command
(
  const gstk_scws_open_ch_req_type     * scws_open_ch_cmd,
  dsm_item_type                       ** resp
)
{
  qmi_error_e_type errval = QMI_ERR_NONE;

  ASSERT(scws_open_ch_cmd != NULL);

  errval = qmi_cati_scws_open_channel_info_tlv(scws_open_ch_cmd->ch_id,
                                               scws_open_ch_cmd->port_number,
                                               scws_open_ch_cmd->buffer_size,
                                               resp);
  if (errval != QMI_ERR_NONE)
  {
    return errval;
  }

  if(scws_open_ch_cmd->alpha.alphabet_given)
  {
    errval = qmi_cati_alphabet_tlv(&scws_open_ch_cmd->alpha,
                                   resp,
                                   CATI_PRM_TYPE_SCWS_ALPHA);
  }

  return errval;
} /* qmi_cati_populate_scws_open_channel_command */


/*===========================================================================
  FUNCTION QMI_CATI_POPULATE_SCWS_CLOSE_CHANNEL_COMMAND()

  DESCRIPTION
    to populate the tlvs from the send data proactive command

  PARAMETERS
    send_data_cmd        : Decoded proactive command
    resp                 : pointer to tlv

  RETURN VALUE
    qmi_error_e_type

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static qmi_error_e_type qmi_cati_populate_scws_close_channel_command
(
  const gstk_scws_close_ch_req_type     * scws_close_ch_cmd,
  dsm_item_type                        ** resp
)
{
  qmi_error_e_type errval = QMI_ERR_NONE;

  ASSERT(scws_close_ch_cmd != NULL);

  errval = qmi_cati_scws_channel_state_tlv(scws_close_ch_cmd->ch_id,
                                           scws_close_ch_cmd->channel_state,
                                           resp);

  return errval;
} /* qmi_cati_populate_scws_close_channel_command */


/*===========================================================================
  FUNCTION QMI_CATI_POPULATE_SCWS_SEND_DATA_COMMAND()

  DESCRIPTION
    to populate the tlvs from the send data proactive command

  PARAMETERS
    send_data_cmd        : Decoded proactive command
    resp                 : pointer to tlv

  RETURN VALUE
    qmi_error_e_type

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static qmi_error_e_type qmi_cati_populate_scws_send_data_command
(
  const gstk_scws_send_data_req_type     * scws_send_data_cmd,
  dsm_item_type                         ** resp,
  uint16                                   index
)
{
  qmi_error_e_type errval = QMI_ERR_NONE;

  ASSERT(scws_send_data_cmd != NULL);

  errval = qmi_cati_scws_channel_data_tlv(scws_send_data_cmd->ch_id,
                                          scws_send_data_cmd->ch_data,
                                          index,
                                          resp);

  return errval;
} /* qmi_cati_populate_scws_send_data_command */


/*===========================================================================

                             EXTERNAL FUNCTIONS

===========================================================================*/
/*===========================================================================
  FUNCTION QMI_CAT_PARSE_REQUEST_SCWS_OPEN_CHANNEL_STATE()

  DESCRIPTION
    Parses the TLV content of QMI_CAT message

  PARAMETERS
    sdu_in      : input sdu
    parsed_data : output with parsed values

  RETURN VALUE
    QMI_ERR_NONE on success. Other error code on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
qmi_error_e_type qmi_cat_parse_request_scws_open_channel_state
(
  dsm_item_type                           ** sdu_in,
  qmi_cat_in_scws_open_channel_state_type  * parsed_data_ptr
)
{
  qmi_error_e_type  result = QMI_ERR_NONE;

  ASSERT(sdu_in && parsed_data_ptr);

  /* Initialize the output */
  memset(parsed_data_ptr, 0, sizeof(qmi_cat_in_scws_open_channel_state_type));

  while (*sdu_in)
  {
    uint8     type   = 0;
    uint16    len    = 0;

    if( !qmi_svc_get_tl( sdu_in, &type, &len ) )
    {
      continue;
    }

    switch(type)
    {
      case CATI_SCWS_CHANNEL_STATE_TLV:
        result = qmi_cat_extract_packed_tlv(sdu_in, len,
                                            &parsed_data_ptr->channel_state_present,
                                            sizeof(parsed_data_ptr->channel_state),
                                            sizeof(parsed_data_ptr->channel_state),
                                            (void *)&parsed_data_ptr->channel_state);
        break;

      case CATI_SCWS_CHANNEL_STATE_SLOT_TLV:
        result = qmi_cat_extract_packed_tlv(sdu_in, len,
                                            &parsed_data_ptr->slot_present,
                                            sizeof(parsed_data_ptr->slot),
                                            sizeof(parsed_data_ptr->slot),
                                            (void *)&parsed_data_ptr->slot);
        break;

      default:
        QMI_CAT_SKIP_TLV(sdu_in, len, result);
        break;
    }

    if (result != QMI_ERR_NONE)
    {
      UIM_MSG_ERR_2("Error 0x%x parsing TLV type 0x%x", result, type);
      break;
    }
  }

  /* Check that mandatory TLV is received */
  if (result == QMI_ERR_NONE)
  {
    if (!parsed_data_ptr->channel_state_present)
    {
      result = QMI_ERR_MISSING_ARG;
    }
  }

  /* Additional checks on data */
  if (result == QMI_ERR_NONE)
  {
    /* Check on channel state */
    if (parsed_data_ptr->channel_state_present &&
        parsed_data_ptr->channel_state.ch_state != CAT_SCWS_CHANNEL_CLOSED_STATE_V02 &&
        parsed_data_ptr->channel_state.ch_state != CAT_SCWS_CHANNEL_LISTEN_STATE_V02 &&
        parsed_data_ptr->channel_state.ch_state != CAT_SCWS_CHANNEL_ESTABLISHED_STATE_V02)
    {
      result = QMI_ERR_INVALID_ARG;
    }

    /* Check on slot */
    QMI_CAT_CHECK_SLOT_ID_TLV(parsed_data_ptr, result);
  }

  return result;
} /* qmi_cat_parse_request_scws_open_channel_state */


/*===========================================================================
  FUNCTION QMI_CAT_PARSE_REQUEST_SCWS_CLOSE_CHANNEL_STATE()

  DESCRIPTION
    Parses the TLV content of QMI_CAT message

  PARAMETERS
    sdu_in      : input sdu
    parsed_data : output with parsed values

  RETURN VALUE
    QMI_ERR_NONE on success. Other error code on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
qmi_error_e_type qmi_cat_parse_request_scws_close_channel_state
(
  dsm_item_type                            ** sdu_in,
  qmi_cat_in_scws_close_channel_state_type  * parsed_data_ptr
)
{
  qmi_error_e_type  result = QMI_ERR_NONE;

  ASSERT(sdu_in && parsed_data_ptr);

  /* Initialize the output */
  memset(parsed_data_ptr, 0, sizeof(qmi_cat_in_scws_close_channel_state_type));

  while (*sdu_in)
  {
    uint8     type   = 0;
    uint16    len    = 0;

    if( !qmi_svc_get_tl( sdu_in, &type, &len ) )
    {
      continue;
    }

    switch(type)
    {
      case CATI_SCWS_CHANNEL_STATE_TLV:
        result = qmi_cat_extract_packed_tlv(sdu_in, len,
                                            &parsed_data_ptr->channel_state_present,
                                            sizeof(parsed_data_ptr->channel_state),
                                            sizeof(parsed_data_ptr->channel_state),
                                            (void *)&parsed_data_ptr->channel_state);
        break;

      case CATI_SCWS_CHANNEL_STATE_SLOT_TLV:
        result = qmi_cat_extract_packed_tlv(sdu_in, len,
                                            &parsed_data_ptr->slot_present,
                                            sizeof(parsed_data_ptr->slot),
                                            sizeof(parsed_data_ptr->slot),
                                            (void *)&parsed_data_ptr->slot);
        break;

      default:
        QMI_CAT_SKIP_TLV(sdu_in, len, result);
        break;
    }

    if (result != QMI_ERR_NONE)
    {
      UIM_MSG_ERR_2("Error 0x%x parsing TLV type 0x%x", result, type);
      break;
    }
  }

  /* Check that mandatory TLV is received */
  if (result == QMI_ERR_NONE)
  {
    if (!parsed_data_ptr->channel_state_present)
    {
      result = QMI_ERR_MISSING_ARG;
    }
  }

  /* Additional checks on data */
  if (result == QMI_ERR_NONE)
  {
    /* Check on channel state */
    if (parsed_data_ptr->channel_state_present &&
        parsed_data_ptr->channel_state.ch_state != CAT_SCWS_CHANNEL_CLOSED_STATE_V02 &&
        parsed_data_ptr->channel_state.ch_state != CAT_SCWS_CHANNEL_LISTEN_STATE_V02 &&
        parsed_data_ptr->channel_state.ch_state != CAT_SCWS_CHANNEL_ESTABLISHED_STATE_V02)

    {
      result = QMI_ERR_INVALID_ARG;
    }

    /* Check on slot */
    QMI_CAT_CHECK_SLOT_ID_TLV(parsed_data_ptr, result);
  }

  return result;
} /* qmi_cat_parse_request_scws_close_channel_state */


/*===========================================================================
  FUNCTION QMI_CAT_PARSE_REQUEST_SCWS_CHANNEL_STATUS_STATE()

  DESCRIPTION
    Parses the TLV content of QMI_CAT message

  PARAMETERS
    sdu_in      : input sdu
    parsed_data : output with parsed values

  RETURN VALUE
    QMI_ERR_NONE on success. Other error code on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
qmi_error_e_type qmi_cat_parse_request_scws_channel_status_state
(
  dsm_item_type                         ** sdu_in,
  qmi_cat_in_scws_channel_status_state_type   * parsed_data_ptr
)
{
  qmi_error_e_type  result = QMI_ERR_NONE;

  ASSERT(sdu_in && parsed_data_ptr);

  /* Initialize the output */
  memset(parsed_data_ptr, 0, sizeof(qmi_cat_in_scws_channel_status_state_type));

  while (*sdu_in)
  {
    uint8     type   = 0;
    uint16    len    = 0;

    if( !qmi_svc_get_tl( sdu_in, &type, &len ) )
    {
      continue;
    }

    switch(type)
    {
      case CATI_SCWS_CHANNEL_STATE_TLV:
        result = qmi_cat_extract_packed_tlv(sdu_in, len,
                                            &parsed_data_ptr->channel_state_present,
                                            sizeof(parsed_data_ptr->channel_state),
                                            sizeof(parsed_data_ptr->channel_state),
                                            (void *)&parsed_data_ptr->channel_state);
        break;

      case CATI_SCWS_CHANNEL_STATE_SLOT_TLV:
        result = qmi_cat_extract_packed_tlv(sdu_in, len,
                                            &parsed_data_ptr->slot_present,
                                            sizeof(parsed_data_ptr->slot),
                                            sizeof(parsed_data_ptr->slot),
                                            (void *)&parsed_data_ptr->slot);
        break;

      default:
        QMI_CAT_SKIP_TLV(sdu_in, len, result);
        break;
    }

    if (result != QMI_ERR_NONE)
    {
      UIM_MSG_ERR_2("Error 0x%x parsing TLV type 0x%x", result, type);
      break;
    }
  }

  /* Check that mandatory TLV is received */
  if (result == QMI_ERR_NONE)
  {
    if (!parsed_data_ptr->channel_state_present)
    {
      result = QMI_ERR_MISSING_ARG;
    }
  }

  /* Additional checks on data */
  if (result == QMI_ERR_NONE)
  {
    /* Check on channel state */
    if (parsed_data_ptr->channel_state_present &&
        parsed_data_ptr->channel_state.ch_state != CAT_SCWS_CHANNEL_CLOSED_STATE_V02 &&
        parsed_data_ptr->channel_state.ch_state != CAT_SCWS_CHANNEL_LISTEN_STATE_V02 &&
        parsed_data_ptr->channel_state.ch_state != CAT_SCWS_CHANNEL_ESTABLISHED_STATE_V02)

    {
      result = QMI_ERR_INVALID_ARG;
    }

    /* Check on slot */
    QMI_CAT_CHECK_SLOT_ID_TLV(parsed_data_ptr, result);
  }

  return result;
} /* qmi_cat_parse_request_scws_channel_status_state */


/*===========================================================================
  FUNCTION QMI_CAT_PARSE_REQUEST_SCWS_SEND_DATA()

  DESCRIPTION
    Parses the TLV content of QMI_CAT message

  PARAMETERS
    sdu_in      : input sdu
    parsed_data : output with parsed values

  RETURN VALUE
    QMI_ERR_NONE on success. Other error code on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
qmi_error_e_type qmi_cat_parse_request_scws_send_data
(
  dsm_item_type                      ** sdu_in,
  qmi_cat_in_scws_send_data_type      * parsed_data_ptr
)
{
  qmi_error_e_type  result = QMI_ERR_NONE;

  ASSERT(sdu_in && parsed_data_ptr);

  /* Initialize the output */
  memset(parsed_data_ptr, 0, sizeof(qmi_cat_in_scws_send_data_type));

  while (*sdu_in)
  {
    uint8     type   = 0;
    uint16    len    = 0;

    if( !qmi_svc_get_tl( sdu_in, &type, &len ) )
    {
      continue;
    }

    switch(type)
    {
      case CATI_SCWS_SEND_DATA_CHANNEL_DATA_RESULT_TLV:
        result = qmi_cat_extract_packed_tlv(sdu_in, len,
                                            &parsed_data_ptr->channel_data_result_present,
                                            sizeof(parsed_data_ptr->channel_data_result),
                                            sizeof(parsed_data_ptr->channel_data_result),
                                            (void *)&parsed_data_ptr->channel_data_result);
        break;

      case CATI_SCWS_SEND_DATA_SLOT_TLV:
        result = qmi_cat_extract_packed_tlv(sdu_in, len,
                                            &parsed_data_ptr->slot_present,
                                            sizeof(parsed_data_ptr->slot),
                                            sizeof(parsed_data_ptr->slot),
                                            (void *)&parsed_data_ptr->slot);
        break;

      default:
        QMI_CAT_SKIP_TLV(sdu_in, len, result);
        break;
    }

    if (result != QMI_ERR_NONE)
    {
      UIM_MSG_ERR_2("Error 0x%x parsing TLV type 0x%x", result, type);
      break;
    }
  }

  /* Check that mandatory TLV is received */
  if (result == QMI_ERR_NONE)
  {
    if (!parsed_data_ptr->channel_data_result_present)
    {
      result = QMI_ERR_MISSING_ARG;
    }
  }

  /* Additional checks on data */
  if (result == QMI_ERR_NONE)
  {
    /* Check on channel data result */
    if (parsed_data_ptr->channel_data_result_present &&
        parsed_data_ptr->channel_data_result.result != 0 &&
        parsed_data_ptr->channel_data_result.result != 1)
    {
      result = QMI_ERR_INVALID_ARG;
    }

    /* Check on slot */
    QMI_CAT_CHECK_SLOT_ID_TLV(parsed_data_ptr, result);
  }

  return result;
} /* qmi_cat_parse_request_scws_send_data */


/*===========================================================================
  FUNCTION QMI_CAT_PARSE_REQUEST_SCWS_DATA_AVAILABLE()

  DESCRIPTION
    Parses the TLV content of QMI_CAT message

  PARAMETERS
    sdu_in      : input sdu
    parsed_data : output with parsed values

  RETURN VALUE
    QMI_ERR_NONE on success. Other error code on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
qmi_error_e_type qmi_cat_parse_request_scws_data_available
(
  dsm_item_type                      ** sdu_in,
  qmi_cat_in_scws_data_available_type      * parsed_data_ptr
)
{
  qmi_error_e_type  result = QMI_ERR_NONE;

  ASSERT(sdu_in && parsed_data_ptr);

  /* Initialize the output */
  memset(parsed_data_ptr, 0, sizeof(qmi_cat_in_scws_data_available_type));

  while (*sdu_in)
  {
    uint8     type   = 0;
    uint16    len    = 0;

    if( !qmi_svc_get_tl( sdu_in, &type, &len ) )
    {
      continue;
    }

    switch(type)
    {
      case CATI_SCWS_DATA_AVAILABLE_CHANNEL_DATA_TLV:
        result = qmi_cat_extract_packed_tlv(sdu_in, len,
                                            &parsed_data_ptr->channel_data_present,
                                            QMI_CAT_SCWS_CHANNEL_DATA_TLV_MIN_SIZE,
                                            sizeof(parsed_data_ptr->channel_data),
                                            (void *)&parsed_data_ptr->channel_data);
        break;

      case CATI_SCWS_DATA_AVAILABLE_REMAIN_DATA_LEN_TLV:
        result = qmi_cat_extract_packed_tlv(sdu_in, len,
                                            &parsed_data_ptr->remain_data_len_present,
                                            sizeof(parsed_data_ptr->remain_data_len),
                                            sizeof(parsed_data_ptr->remain_data_len),
                                            (void *)&parsed_data_ptr->remain_data_len);
        break;

      case CATI_SCWS_DATA_AVAILABLE_SLOT_TLV:
        result = qmi_cat_extract_packed_tlv(sdu_in, len,
                                            &parsed_data_ptr->slot_present,
                                            sizeof(parsed_data_ptr->slot),
                                            sizeof(parsed_data_ptr->slot),
                                            (void *)&parsed_data_ptr->slot);
        break;

      default:
        QMI_CAT_SKIP_TLV(sdu_in, len, result);
        break;
    }

    if (result != QMI_ERR_NONE)
    {
      UIM_MSG_ERR_2("Error 0x%x parsing TLV type 0x%x", result, type);
      break;
    }
  }

  /* Check that mandatory TLV is received */
  if (result == QMI_ERR_NONE)
  {
    if (!parsed_data_ptr->channel_data_present ||
        !parsed_data_ptr->remain_data_len_present)
    {
      result = QMI_ERR_MISSING_ARG;
    }
  }

  /* Additional checks on data */
  if (result == QMI_ERR_NONE)
  {
    /* Check on channel data */
    if (parsed_data_ptr->channel_data_present &&
        parsed_data_ptr->channel_data.data_len >
        sizeof(parsed_data_ptr->channel_data.data))
    {
      result = QMI_ERR_MALFORMED_MSG;
    }

    /* Check on slot */
    QMI_CAT_CHECK_SLOT_ID_TLV(parsed_data_ptr, result);
  }

  return result;
} /* qmi_cat_parse_request_scws_data_available */


/*===========================================================================
  FUNCTION QMI_CAT_RESPONSE_SCWS_COMMAND()

  DESCRIPTION
    to populate the tlvs from the scws command

  PARAMETERS
    ind             : output sdu
    scws_cmd        : GSTK decoded scws command

  RETURN VALUE
    qmi_error_e_type

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
qmi_error_e_type qmi_cat_response_scws_command
(
  dsm_item_type                 ** ind,
  const gstk_cmd_from_card_type  * scws_cmd,
  uint16                           index
)
{
  qmi_error_e_type  errval  = QMI_ERR_NONE;

  ASSERT(scws_cmd != NULL);

  switch (scws_cmd->hdr_cmd.command_id)
  {
    case GSTK_SCWS_OPEN_CH_REQ:
      errval = qmi_cati_populate_scws_open_channel_command(
                    &scws_cmd->cmd.scws_open_ch_cmd_req,
                    ind);
      break;

    case GSTK_SCWS_CLOSE_CH_REQ:
      errval = qmi_cati_populate_scws_close_channel_command(
                    &scws_cmd->cmd.scws_close_ch_cmd_req,
                    ind);
      break;

    case GSTK_SCWS_SEND_DATA_REQ:
      errval = qmi_cati_populate_scws_send_data_command(
                    &scws_cmd->cmd.scws_send_data_cmd_req,
                    ind, index);
      break;

    default:
      /* Unhandled GSTK event */
      errval = QMI_ERR_INTERNAL;
      break;
  }

  /* In DSDS, insert the slot id */
  if (errval == QMI_ERR_NONE)
  {
    errval = qmi_cati_slot_id_tlv(
                  scws_cmd->hdr_cmd.sim_slot_id,
                  ind,
                  CATI_PRM_TYPE_SCWS_SLOT_ID);
  }

  return errval;
} /* qmi_cat_response_scws_command */

#ifdef FEATURE_UIM_TEST_FRAMEWORK
#error code not present
#endif /* FEATURE_UIM_TEST_FRAMEWORK */
