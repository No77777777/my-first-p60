/*==========================================================================

DESCRIPTION

 The header file for QMI CAT SCWS commands parsing and populate.

Copyright (c) 2015 QUALCOMM Technologies, Inc(QTI).
All Rights Reserved.
QUALCOMM Technologies Confidential and Proprietary

  $Header: //components/rel/uim.mpss/6.1.0/uimqmi/src/qmi_cat_scws_parsing.h#2 $ $DateTime: 2019/10/30 06:56:05 $ $Author: pwbldsvc $

when        who    what, where, why
--------    ---    ----------------------------------------------------------
09/08/15    vr     Initial version
===========================================================================*/

#ifndef QMI_CAT_SCWS_PARSING_H
#define QMI_CAT_SCWS_PARSING_H


/*===========================================================================

                          INCLUDE FILES FOR MODULE

===========================================================================*/
#include "customer.h"
#include "comdef.h"
#include "dsm.h"
#include "card_application_toolkit_v02.h"

/* These includes need to always be at bottom - offtarget use only */
#ifdef FEATURE_UIM_TEST_FRAMEWORK
#error code not present
#endif /* FEATURE_UIM_TEST_FRAMEWORK */

/*===========================================================================

                               INTERNAL DATA

===========================================================================*/
/*---------------------------------------------------------------------------
  Max size used in QMI requests
---------------------------------------------------------------------------*/
#define CAT_TLV_SCWS_SEND_DATA_MAX                     (500)

/*---------------------------------------------------------------------------
  Minimum size of TLVs
---------------------------------------------------------------------------*/
#define QMI_CAT_SCWS_CHANNEL_DATA_TLV_MIN_SIZE         (6)

/*---------------------------------------------------------------------------
  Structure used to parse the request for scws open channel state
---------------------------------------------------------------------------*/
typedef struct
{
  boolean                                    channel_state_present;
  boolean                                    slot_present;

  PACKED struct PACKED_POST
  {
    uint32                                   ch_id;
    uint8                                    ch_state;
  }                                          channel_state;
  PACKED struct PACKED_POST
  {
    uint8                                    slot;
  }                                          slot;
} qmi_cat_in_scws_open_channel_state_type;

/*---------------------------------------------------------------------------
  Structure used to parse the request for scws close channel state
---------------------------------------------------------------------------*/
typedef struct
{
  boolean                                    channel_state_present;
  boolean                                    slot_present;

  PACKED struct PACKED_POST
  {
    uint32                                   ch_id;
    uint8                                    ch_state;
  }                                          channel_state;
  PACKED struct PACKED_POST
  {
    uint8                                    slot;
  }                                          slot;
} qmi_cat_in_scws_close_channel_state_type;

/*---------------------------------------------------------------------------
  Structure used to parse the request for scws channel status state
---------------------------------------------------------------------------*/
typedef struct
{
  boolean                                    channel_state_present;
  boolean                                    slot_present;

  PACKED struct PACKED_POST
  {
    uint32                                   ch_id;
    uint8                                    ch_state;
  }                                          channel_state;
  PACKED struct PACKED_POST
  {
    uint8                                    slot;
  }                                          slot;
} qmi_cat_in_scws_channel_status_state_type;

/*---------------------------------------------------------------------------
  Structure used to parse the request for scws send data
---------------------------------------------------------------------------*/
typedef struct
{
  boolean                                    channel_data_result_present;
  boolean                                    slot_present;

  PACKED struct PACKED_POST
  {
    uint32                                   ch_id;
    uint8                                    result;
  }                                          channel_data_result;
  PACKED struct PACKED_POST
  {
    uint8                                    slot;
  }                                          slot;
} qmi_cat_in_scws_send_data_type;

/*---------------------------------------------------------------------------
  Structure used to parse the request for scws data available
---------------------------------------------------------------------------*/
typedef struct
{
  boolean                                    channel_data_present;
  boolean                                    remain_data_len_present;
  boolean                                    slot_present;

  PACKED struct PACKED_POST
  {
    uint32                ch_id;
    uint16                data_len;
    uint8                 data[QMI_CAT_SCWS_DATA_MAX_LENGTH_V02];
  }                       channel_data;
  PACKED struct PACKED_POST
  {
    uint16                                   data_len;
  }                                          remain_data_len;
  PACKED struct PACKED_POST
  {
    uint8                                    slot;
  }                                          slot;
} qmi_cat_in_scws_data_available_type;

/*===========================================================================

                             FUNCTIONS

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
);

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
);

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
);

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
);

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
  dsm_item_type                           ** sdu_in,
  qmi_cat_in_scws_data_available_type      * parsed_data_ptr
);

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
);

#ifdef FEATURE_UIM_TEST_FRAMEWORK
#error code not present
#endif /* FEATURE_UIM_TEST_FRAMEWORK */

#endif /* QMI_CAT_SCWS_PARSING_H */

