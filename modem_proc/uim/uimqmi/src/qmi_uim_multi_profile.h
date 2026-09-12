#ifndef QMI_UIM_MULTI_PROFILE_H
#define QMI_UIM_MULTI_PROFILE_H
/*==============================================================================

             Q M I _ U I M _ M U L T I _ P R O F I L E _ P A R S I N G _ H

DESCRIPTION

 The Data Services Qualcomm MSM Interface Device Management Services source
 file.

Copyright (c) 2016 - 2017, 2020 QUALCOMM Technologies, Inc(QTI).
All Rights Reserved.
QUALCOMM Technologies Confidential and Proprietary
==============================================================================*/
/*==============================================================================

                      EDIT HISTORY FOR FILE

  $Header: //components/rel/uim.mpss/6.1.0/uimqmi/src/qmi_uim_multi_profile.h#2 $

when        who    what, where, why
--------    ---    ------------------------------------------------------------
08/17/20    vgd    Added logic to get EID using both M2M and LPA methods
04/19/17    ku     GET EID for M2M cards
04/12/17    ar     Add support for retrieve/configure default server address 
12/07/16    av     LPA phase2 initial changes
07/28/16    ar     Query internally for profile info in case of add profile
06/22/16    sp     Initial Revision.
==============================================================================*/

/*==============================================================================

                          INCLUDE FILES FOR MODULE

==============================================================================*/
#include "uim_p.h"
#include "mmgsdilib_common.h"
#include "lpalib.h"
#include "qmi_uim_internal.h"

/*==============================================================================

                             EXTERNAL FUNCTIONS

==============================================================================*/

/*===========================================================================
  FUNCTION QMI_UIM_CLEAR_PROFILE_INFO()

  DESCRIPTION
    Clear all the profiles data for that slot from QMI UIM cache

  PARAMETERS
    slot_index

  RETURN VALUE
    None

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
void qmi_uim_clear_profile_info
(
  mmgsdi_slot_id_enum_type   mmgsdi_slot
);

/*===========================================================================
  FUNCTION QMI_UIMI_GET_SIM_PROFILE()

  DESCRIPTION
    Retrieves the profile information supported by the card

  PARAMETERS
    sp        : service provided state pointer
    cmd_buf_p : ptr to cmd buffer
    cl_sp     : client state pointer
    sdu_in    : incoming request

  RETURN VALUE
    ptr to response or NULL if none.

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
dsm_item_type * qmi_uimi_get_sim_profile
(
  void *            sp,
  void *            cmd_buf_p,
  void *            cl_sp,
  dsm_item_type **  sdu_in
);

/*===========================================================================
  FUNCTION QMI_UIMI_SET_SIM_PROFILE()

  DESCRIPTION
    Switches the profile in the card

  PARAMETERS
    sp        : service provided state pointer
    cmd_buf_p : ptr to cmd buffer
    cl_sp     : client state pointer
    sdu_in    : incoming request

  RETURN VALUE
    ptr to response or NULL if none.

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
dsm_item_type * qmi_uimi_set_sim_profile
(
  void *            sp,
  void *            cmd_buf_p,
  void *            cl_sp,
  dsm_item_type **  sdu_in
);

/*===========================================================================
  FUNCTION QMI_UIMI_DELETE_PROFILE()

  DESCRIPTION
    Delete the requested profile from card. The command will go to LPA in all
    cases even if card is not marked as eUICC and LPA needs to reject it.

  PARAMETERS
    sp        : service provided state pointer
    cmd_buf_p : ptr to cmd buffer
    cl_sp     : client state pointer
    sdu_in    : incoming request

  RETURN VALUE
    ptr to response or NULL if none.

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
dsm_item_type * qmi_uimi_delete_profile
(
  void *            sp,
  void *            cmd_buf_p,
  void *            cl_sp,
  dsm_item_type **  sdu_in
);

/*===========================================================================
  FUNCTION QMI_UIMI_EUICC_MEMORY_RESET()

  DESCRIPTION
    Reset the eUICC memory for that Slot. The command will go to LPA in all
    scases even if card is not marked as eUICC and LPA needs to reject it.

  PARAMETERS
    sp        : service provided state pointer
    cmd_buf_p : ptr to cmd buffer
    cl_sp     : client state pointer
    sdu_in    : incoming request

  RETURN VALUE
    ptr to response or NULL if none.

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
dsm_item_type * qmi_uimi_euicc_memory_reset
(
  void *            sp,
  void *            cmd_buf_p,
  void *            cl_sp,
  dsm_item_type **  sdu_in
);

/*===========================================================================
  FUNCTION QMI_UIMI_UPDATE_PROFILE_NICKNAME()

  DESCRIPTION
    Updates the nickname of requested profile. The command will go to LPA in
    all cases even if card is not marked as eUICC and LPA needs to reject it.

  PARAMETERS
    sp        : service provided state pointer
    cmd_buf_p : ptr to cmd buffer
    cl_sp     : client state pointer
    sdu_in    : incoming request

  RETURN VALUE
    ptr to response or NULL if none.

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
dsm_item_type * qmi_uimi_update_profile_nickname
(
  void *            sp,
  void *            cmd_buf_p,
  void *            cl_sp,
  dsm_item_type **  sdu_in
);

/*===========================================================================
  FUNCTION QMI_UIMI_GET_PROFILE_INFO()

  DESCRIPTION
    Provide the complete info of the requested profile. The command
    will go to LPA in all cases even if card is not marked as eUICC
    and LPA needs to reject it.


  PARAMETERS
    sp        : service provided state pointer
    cmd_buf_p : ptr to cmd buffer
    cl_sp     : client state pointer
    sdu_in    : incoming request

  RETURN VALUE
    ptr to response or NULL if none.

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
dsm_item_type * qmi_uimi_get_profile_info
(
  void *            sp,
  void *            cmd_buf_p,
  void *            cl_sp,
  dsm_item_type **  sdu_in
);

/*===========================================================================
  FUNCTION QMI_UIMI_ADD_PROFILE()

  DESCRIPTION
    Add a new profile in card. The command will go to LPA in all cases
    even if card is not marked as eUICC and LPA needs to reject it.

  PARAMETERS
    sp        : service provided state pointer
    cmd_buf_p : ptr to cmd buffer
    cl_sp     : client state pointer
    sdu_in    : incoming request

  RETURN VALUE
    ptr to response or NULL if none.

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
dsm_item_type * qmi_uimi_add_profile
(
  void *            sp,
  void *            cmd_buf_p,
  void *            cl_sp,
  dsm_item_type **  sdu_in
);

/*===========================================================================
  FUNCTION QMI_UIMI_PROFILE_USER_CONSENT()

  DESCRIPTION
    Provides the user consent to LPA for the add_profile operation

  PARAMETERS
    sp        : service provided state pointer
    cmd_buf_p : ptr to cmd buffer
    cl_sp     : client state pointer
    sdu_in    : incoming request

  RETURN VALUE
    ptr to response or NULL if none.

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
dsm_item_type * qmi_uimi_profile_user_consent
(
  void *            sp,
  void *            cmd_buf_p,
  void *            cl_sp,
  dsm_item_type **  sdu_in
);

/*===========================================================================
  FUNCTION QMI_UIMI_EUICC_DEFAULT_SERVER_ADDRESS()

  DESCRIPTION
    Get the default SMDS and SMDP address (if configured) from card.
    Also, configure the value of defualt SMDP address in card.

  PARAMETERS
    sp        : service provided state pointer
    cmd_buf_p : ptr to cmd buffer
    cl_sp     : client state pointer
    sdu_in    : incoming request

  RETURN VALUE
    ptr to response or NULL if none.

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
dsm_item_type * qmi_uimi_euicc_default_server_address
(
  void *            sp,
  void *            cmd_buf_p,
  void *            cl_sp,
  dsm_item_type **  sdu_in
);

/*===========================================================================
  FUNCTION QMI_UIMI_GET_EUICC_PROFILE_RESP()

  DESCRIPTION
    Composes the response for get_sim_profile request

  PARAMETERS
    status                     : status of LPA operation
    get_profiles_info_resp_ptr : result of LPA operation
    cb_userdata_ptr            : user_data to retrieve the cmd

  RETURN VALUE
    None

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
void qmi_uimi_get_euicc_profile_resp
(
  lpa_result_enum_type                              status,
  const lpa_get_profiles_info_resp_type           * get_profiles_info_resp_ptr,
  qmi_uimi_userdata_type                          * cb_userdata_ptr
);

/*===========================================================================
  FUNCTION QMI_UIMI_GET_SIM_PROFILE_RESP()

  DESCRIPTION
    Composes the response for get SIM profile request

  PARAMETERS
    status                  : status of MMGSDI operation
    get_sim_profile_cnf_ptr : result of MMGSDI operation

  RETURN VALUE
    None

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
void qmi_uimi_get_sim_profile_resp
(
  mmgsdi_return_enum_type                 status,
  const mmgsdi_get_sim_profile_cnf_type  *get_sim_profile_cnf_ptr
);

/*===========================================================================
  FUNCTION QMI_UIM_QUERY_EUICC_PROFILE_INFO

  DESCRIPTION
    Query LPA module about the profile data

  PARAMETERS
    mmgsdi_slot
    userdata

  RETURN VALUE
    qmi_error_e_type

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
qmi_error_e_type qmi_uim_query_euicc_profile_info
(
  qmi_uimi_userdata_type       * cb_userdata_ptr
);

/*===========================================================================
  FUNCTION  QMI_UIM_PROCESS_LPA_PROFILE_INFO_CB()

  DESCRIPTION
    Processing of LPA response for internally triggered profile fetch

  PARAMETERS
    uim_message_ptr : message to the internal qmi_uim_message
                      containing an external command

  RETURN VALUE
    None

  DEPENDENCIES
    QMI UIM must already have been initialized and registered with Framework

  SIDE EFFECTS
    None
===========================================================================*/
void qmi_uim_process_lpa_profile_info_cb
(
  qmi_uim_message_type * uim_message_ptr
);

/*===========================================================================
  FUNCTION  QMI_UIM_PROCESS_LPA_CB()

  DESCRIPTION
    Processing of LPA response

  PARAMETERS
    uim_message_ptr : message to the internal qmi_uim_message
                      containing an external command

  RETURN VALUE
    None

  DEPENDENCIES
    QMI UIM must already have been initialized and registered with Framework

  SIDE EFFECTS
    None
===========================================================================*/
void qmi_uim_process_lpa_cb
(
  qmi_uim_message_type * uim_message_ptr
);

/*===========================================================================
  FUNCTION  QMI_UIM_PROCESS_LPA_EVENT_CB()

  DESCRIPTION
    This function processes the LPA event

  PARAMETERS
    uim_message_ptr : message to the internal qmi_uim_message
                      containing an external command

  RETURN VALUE
    None

  DEPENDENCIES
    QMI UIM must already have been initialized and registered with Framework

  SIDE EFFECTS
    None
===========================================================================*/
void qmi_uim_process_lpa_event_cb
(
  qmi_uim_message_type * uim_message_ptr
);

/*===========================================================================
  FUNCTION QMI_UIMI_GET_M2M_EID_RESP

  DESCRIPTION
    Generic function to composes response of GET_EID command for MMGSDI.

  PARAMETERS
    uim_message_ptr : Pointer to message from QMI UIM message

  RETURN VALUE
    None

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
void qmi_uimi_get_m2m_eid_resp(
  qmi_uim_message_type * uim_message_ptr
);
#endif /* QMI_UIM_MULTI_PROFILE_H*/
