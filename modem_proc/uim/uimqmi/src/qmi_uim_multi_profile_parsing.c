/*==============================================================================

             Q M I _ U I M _ M U L T I _ P R O F I L E _ P A R S I N G _ C

DESCRIPTION
  The Data Services Qualcomm MSM Interface Device Management Services source
  file.

Copyright (c) 2016-2017, 2019 - 2020 QUALCOMM Technologies, Inc(QTI).
All Rights Reserved.
QUALCOMM Technologies Confidential and Proprietary
==============================================================================*/
/*==============================================================================

                      EDIT HISTORY FOR FILE

$Header: //components/rel/uim.mpss/6.1.0/uimqmi/src/qmi_uim_multi_profile_parsing.c#3 $

when        who    what, where, why
--------    ---    -------------------------------------------------------------
08/20/20    sg     Added UT for LPA functionality through QMI UIM interface
12/04/19    vgd    LPA phase2.1 changes
04/12/17    ar     Add support for retrieve/configure default server address
12/07/16    av     LPA phase2 initial changes
11/04/16    me     Combined multi-line ASSERT in one line
06/22/16    sp     Initial Revision
==============================================================================*/

/*==============================================================================

                          INCLUDE FILES FOR MODULE

==============================================================================*/
#include "uim_variation.h"
#include "comdef.h"
#include "uim_msg.h"
#include "qmi_uim_cat_common.h"
#include "amssassert.h"
#include "modem_mem.h"
#include <stringl/stringl.h>

#include "qmi_uim_multi_profile_parsing.h"
#include "qmi_uim_parsing.h"
#include "qmi_uim_util.h"

#ifdef FEATURE_UIM_TEST_FRAMEWORK
#error code not present
#endif /* FEATURE_UIM_TEST_FRAMEWORK */

/*==============================================================================

                             EXTERNAL FUNCTIONS

==============================================================================*/

/*===========================================================================
  FUNCTION QMI_UIM_RESPONSE_PROFILE_PERCENTAGE()

  DESCRIPTION
    Composes the TLV content for percentage downloaded or installed during
    add profile


  PARAMETERS
    tlv_id           : TLV id to be sent
    response_pptr    : Pointer to response pointer
    lpa_event_ptr     : Pointer to LPA event data

  RETURN VALUE
    TRUE on success. FALSE on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
boolean qmi_uim_response_profile_percentage
(
  byte                             tlv_id,
  dsm_item_type                 ** response_pptr,
  uint8                            download_percent
)
{
  PACKED struct PACKED_POST
  {
    uint8   download_percent;
  } tlv_data;

  ASSERT(response_pptr);

  tlv_data.download_percent = download_percent;

  return qmi_svc_put_param_tlv(response_pptr,
                               tlv_id,
                               sizeof(tlv_data),
                               (void *)&tlv_data);
} /* qmi_uim_response_profile_percentage */


/*===========================================================================
  FUNCTION QMI_UIM_RESPONSE_DOWNLOAD_ERROR_CAUSE()

  DESCRIPTION
    Composes the TLV content for error cause during add profile


  PARAMETERS
    tlv_id           : TLV id to be sent
    response_pptr    : Pointer to response pointer
    lpa_status       : LPA status for the add_profile event

  RETURN VALUE
    TRUE on success. FALSE on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
boolean qmi_uim_response_download_error_cause
(
  byte                             tlv_id,
  dsm_item_type                 ** response_pptr,
  lpa_result_enum_type             lpa_status
)
{
  PACKED struct PACKED_POST
  {
    uint32   error_cause;
  } tlv_data;

  ASSERT(response_pptr);

  switch (lpa_status)
  {
    case LPA_ERROR_HEAP_EXHAUSTED:
      tlv_data.error_cause = UIM_DOWNLOAD_ERROR_CAUSE_MEMORY_V01;
      break;

    case LPA_ERROR_NETWORK:
      tlv_data.error_cause = UIM_DOWNLOAD_ERROR_CAUSE_NETWORK_V01;
      break;

    case LPA_ERROR_SIM_NOT_READY:
      tlv_data.error_cause = UIM_DOWNLOAD_ERROR_CAUSE_SIM_V01;
      break;

    case LPA_ERROR_UNSUPPORTED_PROFILE_CLASS:
      tlv_data.error_cause = UIM_DOWNLOAD_ERROR_CAUSE_UNSUPPORTED_PROFILE_CLASS_V01;
      break;

    case LPA_ERROR_PROFILE_INSTALLATION_PPR_ID_NOT_ALLOWED:
      tlv_data.error_cause = UIM_DOWNLOAD_ERROR_CAUSE_PPR_NOT_ALLOWED_V01;
      break;

    default:
      tlv_data.error_cause = UIM_DOWNLOAD_ERROR_CAUSE_GENERIC_V01;
      break;
  }

  return qmi_svc_put_param_tlv(response_pptr,
                               tlv_id,
                               sizeof(tlv_data),
                               (void *)&tlv_data);
} /* qmi_uim_response_download_error_cause */


/*===========================================================================
  FUNCTION QMI_UIM_RESPONSE_EUICC_DOWNLOAD_STATUS_DATA()

  DESCRIPTION
    Composes the TLV content for download progress

  PARAMETERS
    tlv_id           : TLV id to be sent
    response_pptr    : Pointer to response pointer
    lpa_event_ptr     : Pointer to LPA event data
    lpa_status         : LPA download status


  RETURN VALUE
    TRUE on success. FALSE on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
boolean qmi_uim_response_euicc_download_status_data
(
  byte                             tlv_id,
  dsm_item_type                 ** response_pptr,
  const lpa_event_data_type      * lpa_event_ptr,
  lpa_result_enum_type             lpa_status
)
{
  PACKED struct PACKED_POST
  {
    uint32   download_status;
  } tlv_data;

  ASSERT(response_pptr && lpa_event_ptr);

  if (lpa_status == LPA_SUCCESS)
  {
    /* If event type is "LPA_REQUEST_USER_CONSENT_EVENT", it means QMI UIM
       is sending this indication to send the PPR info and the user consent */
    if (lpa_event_ptr->evt_type == LPA_REQUEST_USER_CONSENT_EVENT)
    {
      tlv_data.download_status = UIM_PROFILE_USER_CONSENT_REQUIRED_V01;
    }
    else if (lpa_event_ptr->evt_type == LPA_PROFILE_DOWNLOAD_PROGRESS_EVENT)
    {
      if (lpa_event_ptr->evt_data.profile_download_progress.installation_complete)
      {
        tlv_data.download_status = UIM_PROFILE_INSTALLATION_COMPLETE_V01;
      }
      else if (lpa_event_ptr->evt_data.profile_download_progress.percent_downloaded < 100)
      {
        tlv_data.download_status = UIM_PROFILE_DOWNLOAD_IN_PROGRESS_V01;
      }
      else if (lpa_event_ptr->evt_data.profile_download_progress.percent_downloaded == 100 &&
               lpa_event_ptr->evt_data.profile_download_progress.installation_complete == FALSE)
      {
        tlv_data.download_status = UIM_PROFILE_DOWNLOAD_COMPLETE_INSTALLATION_IN_PROGRESS_V01;
      }
      else
      {
        tlv_data.download_status = UIM_PROFILE_DOWNLOAD_ERROR_V01;
      }
    }
    else
    {
      tlv_data.download_status = UIM_PROFILE_DOWNLOAD_ERROR_V01;
      return FALSE;
    }
  }
  else
  {
    tlv_data.download_status = UIM_PROFILE_DOWNLOAD_ERROR_V01;
  }

  return qmi_svc_put_param_tlv(response_pptr,
                               tlv_id,
                               sizeof(tlv_data),
                               (void *)&tlv_data);
} /* qmi_uim_response_euicc_download_status_data */


/*===========================================================================
  FUNCTION QMI_UIM_RESPONSE_GET_EUICC_PROFILE_INFO()

  DESCRIPTION
    Composes the TLV content of profile information

  PARAMETERS
    tlv_id                     : id to be used for the TLV
    response_pptr              : output sdu
    get_profiles_info_resp_ptr : profile information pointer

  RETURN VALUE
    TRUE on success. FALSE on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
boolean qmi_uim_response_get_euicc_profile_info
(
  byte                                   tlv_id,
  dsm_item_type **                       response_pptr,
  const lpa_get_profiles_info_resp_type *get_profiles_info_resp_ptr
)
{
  uint8   profile_index = 0;
  uint16  tlv_len       = 0;

  PACKED struct PACKED_POST
  {
    uint8 num_profiles;
    PACKED struct PACKED_POST
    {
      uint32 profile_id;
      uint32 profile_type;
    }profile_data[QMI_UIM_PROFILES_MAX_V01];
  }tlv_data_profile_data;

  ASSERT(response_pptr && get_profiles_info_resp_ptr);

  tlv_data_profile_data.num_profiles = MIN(get_profiles_info_resp_ptr->num_profiles,
                                           QMI_UIM_PROFILES_MAX_V01);

  for(profile_index = 0;
      profile_index < tlv_data_profile_data.num_profiles;
      profile_index++)
  {
    tlv_data_profile_data.profile_data[profile_index].profile_id =
                          qmi_uim_get_profile_id_from_index(profile_index);
    tlv_data_profile_data.profile_data[profile_index].profile_type =
                          UIM_PROFILE_TYPE_REGULAR_V01;
  }

  tlv_len = sizeof(uint8) +
            (sizeof(tlv_data_profile_data.profile_data[0]) *
             tlv_data_profile_data.num_profiles);

  return qmi_svc_put_param_tlv(response_pptr,
                               tlv_id,
                               tlv_len,
                               (void *)&tlv_data_profile_data);
} /* qmi_uim_response_get_euicc_profile_info */


/*===========================================================================
  FUNCTION QMI_UIM_RESPONSE_GET_ALL_PROFILE_INFO()

  DESCRIPTION
    Composes the TLV content of profile information

  PARAMETERS
    tlv_id              : id to be used for the TLV
    response_pptr       : output sdu
    get_sim_profile_cnf_ptr : profile confirmation pointer

  RETURN VALUE
    TRUE on success. FALSE on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
boolean qmi_uim_response_get_all_profile_info
(
  byte                                   tlv_id,
  dsm_item_type **                       response_pptr,
  const mmgsdi_get_sim_profile_cnf_type *get_sim_profile_cnf_ptr
)
{
  uint8   profile_index = 0;
  uint16  tlv_len       = 0;
  uint8*  tlv_data_ptr  = NULL;
  uint8*  curr_ptr      = NULL;
  boolean ret_value     = TRUE;
  uint8   num_profiles  = 0;

  PACKED struct PACKED_POST
  {
    uint8 num_profiles;
  }tlv_data_profiles;

  PACKED struct PACKED_POST
  {
    uint32 profile_id;
    uint32 profile_type;
  }tlv_data_profile_data;

  ASSERT(response_pptr && get_sim_profile_cnf_ptr);

  num_profiles = MIN(get_sim_profile_cnf_ptr->num_profile, MMGSDI_MAX_PROFILE);

  /* calculate memory requirement */
  tlv_len = sizeof(tlv_data_profiles);
  tlv_len += (sizeof(tlv_data_profile_data) * num_profiles);

  /* Allocate memory for TLV */
  tlv_data_ptr = (uint8*)modem_mem_alloc(tlv_len, MODEM_MEM_CLIENT_DATACOMMON);
  if (tlv_data_ptr == NULL)
  {
    return FALSE;
  }
  curr_ptr = tlv_data_ptr;

  tlv_data_profiles.num_profiles = num_profiles;

  /* Copy num of profiles in the TLV */
  ASSERT(curr_ptr + sizeof(tlv_data_profiles) <= tlv_data_ptr + tlv_len);
  memscpy(curr_ptr,
          tlv_len - (curr_ptr - tlv_data_ptr),
          &tlv_data_profiles,
          sizeof(tlv_data_profiles));
  curr_ptr += sizeof(tlv_data_profiles);

  for(profile_index = 0;
      profile_index < num_profiles;
      profile_index++)
  {
    tlv_data_profile_data.profile_id =
      get_sim_profile_cnf_ptr->profile_list[profile_index].profile_id;
    tlv_data_profile_data.profile_type =
      get_sim_profile_cnf_ptr->profile_list[profile_index].profile_type;

    /* Copy profile data in the TLV */
    ASSERT(curr_ptr + sizeof(tlv_data_profile_data) <= tlv_data_ptr + tlv_len);
    memscpy(curr_ptr,
            tlv_len - (curr_ptr - tlv_data_ptr),
            &tlv_data_profile_data,
            sizeof(tlv_data_profile_data));
    curr_ptr += sizeof(tlv_data_profile_data);
  }

  tlv_len = (uint16)(curr_ptr - tlv_data_ptr);

  ret_value = qmi_svc_put_param_tlv(response_pptr,
                                    tlv_id,
                                    tlv_len,
                                    (void *)tlv_data_ptr);

  modem_mem_free(tlv_data_ptr, MODEM_MEM_CLIENT_DATACOMMON);

  return ret_value;
} /* qmi_uim_response_get_all_profile_info */


/*===========================================================================
  FUNCTION QMI_UIM_RESPONSE_GET_ACTIVE_PROFILE_INFO()

  DESCRIPTION
    Composes the TLV content of active profile information

  PARAMETERS
    tlv_id              : id to be used for the TLV
    response_pptr       : output sdu
    get_sim_profile_cnf_ptr : profile confirmation pointer

  RETURN VALUE
    TRUE on success. FALSE on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
boolean qmi_uim_response_get_active_profile_info
(
  byte                                   tlv_id,
  dsm_item_type **                       response_pptr,
  const mmgsdi_get_sim_profile_cnf_type *get_sim_profile_cnf_ptr
)
{
  PACKED struct PACKED_POST
  {
    uint32 active_profile_id;
    uint32 active_profile_type;
  }tlv_data_active_profile;

  ASSERT(response_pptr && get_sim_profile_cnf_ptr);

  tlv_data_active_profile.active_profile_id =
    get_sim_profile_cnf_ptr->active_profile.profile_id;
  tlv_data_active_profile.active_profile_type =
    get_sim_profile_cnf_ptr->active_profile.profile_type;

  return qmi_svc_put_param_tlv(response_pptr,
                               tlv_id,
                               sizeof(tlv_data_active_profile),
                               (void *)&tlv_data_active_profile);
} /* qmi_uim_response_get_active_profile_info */


/*===========================================================================
  FUNCTION QMI_UIM_RESPONSE_PROFILE_STATE()

  DESCRIPTION
    Composes the TLV content of profile state

  PARAMETERS
    tlv_id              : id to be used for the TLV
    response_pptr       : output sdu
    profile_state       : Profile State

  RETURN VALUE
    TRUE on success. FALSE on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
boolean qmi_uim_response_profile_state
(
  byte                                   tlv_id,
  dsm_item_type **                       response_pptr,
  lpa_profile_state_enum_type            profile_state
)
{
  PACKED struct PACKED_POST
  {
    uint32 profile_state;
  }tlv_data;

  ASSERT(response_pptr);

  switch (profile_state)
  {
    case LPA_PROFILE_STATE_ENABLED:
      tlv_data.profile_state = UIM_PROFILE_STATE_ACTIVE_V01;
      break;
    case LPA_PROFILE_STATE_DISABLED:
      tlv_data.profile_state = UIM_PROFILE_STATE_INACTIVE_V01;
      break;
    default:
      return FALSE;
  }

  return qmi_svc_put_param_tlv(response_pptr,
                               tlv_id,
                               sizeof(tlv_data),
                               (void *)&tlv_data);
} /* qmi_uim_response_profile_state */


/*===========================================================================
  FUNCTION QMI_UIM_RESPONSE_PROFILE_ICON_TYPE()

  DESCRIPTION
    Composes the TLV content of 32 bit icon enum

  PARAMETERS
    tlv_id              : id to be used for the TLV
    response_pptr       : output sdu
    profile_icon        : Profile Icon

  RETURN VALUE
    TRUE on success. FALSE on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
boolean qmi_uim_response_profile_icon_type
(
  byte                                   tlv_id,
  dsm_item_type **                       response_pptr,
  lpa_icon_type_enum_type                profile_icon
)
{
  PACKED struct PACKED_POST
  {
    uint32 icon_type;
  }tlv_data;

  ASSERT(response_pptr);

  switch (profile_icon)
  {
    case LPA_ICON_TYPE_JPEG:
      tlv_data.icon_type = UIM_PROFILE_ICON_TYPE_JPEG_V01;
      break;
    case LPA_ICON_TYPE_PNG:
      tlv_data.icon_type = UIM_PROFILE_ICON_TYPE_PNG_V01;
      break;
    default:
      tlv_data.icon_type = UIM_PROFILE_ICON_TYPE_NONE_V01;
      break;
  }

  return qmi_svc_put_param_tlv(response_pptr,
                               tlv_id,
                               sizeof(tlv_data),
                               (void *)&tlv_data);
} /* qmi_uim_response_profile_icon_type */


/*===========================================================================
  FUNCTION QMI_UIM_RESPONSE_PROFILE_CLASS()

  DESCRIPTION
    Composes the TLV content of profile class

  PARAMETERS
    tlv_id              : id to be used for the TLV
    response_pptr       : output sdu
    profile_class       : Profile Class

  RETURN VALUE
    TRUE on success. FALSE on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
boolean qmi_uim_response_profile_class
(
  byte                                   tlv_id,
  dsm_item_type **                       response_pptr,
  lpa_profile_class_enum_type            profile_class
)
{
  PACKED struct PACKED_POST
  {
    uint32 profile_class;
  }tlv_data;

  ASSERT(response_pptr);

  switch (profile_class)
  {
    case LPA_PROFILE_CLASS_TEST:
      tlv_data.profile_class = UIM_PROFILE_CLASS_TEST_V01;
      break;
    case LPA_PROFILE_CLASS_PROVISIONING:
      tlv_data.profile_class = UIM_PROFILE_CLASS_PROVISIONING_V01;
      break;
    case LPA_PROFILE_CLASS_OPERATIONAL:
      tlv_data.profile_class = UIM_PROFILE_CLASS_OPERATIONAL_V01;
      break;
    default:
      return FALSE;
  }

  return qmi_svc_put_param_tlv(response_pptr,
                               tlv_id,
                               sizeof(tlv_data),
                               (void *)&tlv_data);
} /* qmi_uim_response_profile_class */


/*===========================================================================
  FUNCTION QMI_UIM_RESPONSE_PPR_INFO()

  DESCRIPTION
    Composes the TLV content of profile policy rules with
    user consent for each profile

  PARAMETERS
    tlv_id                 : id to be used for the TLV
    response_pptr          : output sdu
    ppr                    : profile policy rules

  RETURN VALUE
    TRUE on success. FALSE on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
boolean qmi_uim_response_ppr_info
(
  byte                                   tlv_id,
  dsm_item_type **                       response_pptr,
  lpa_ppr_ids_type                       ppr_ids
)
{
  PACKED struct PACKED_POST
  {
    uint64 ppr_mask;
  }tlv_data;

  ASSERT(response_pptr);

  tlv_data.ppr_mask = 0;

  if (ppr_ids.ppr1)
  {
    tlv_data.ppr_mask |= UIM_PPR_PROFILE_DISABLE_NOT_ALLOWED_V01;
  }

  if (ppr_ids.ppr2)
  {
    tlv_data.ppr_mask |= UIM_PPR_PROFILE_DELETE_NOT_ALLOWED_V01;
  }

  if (ppr_ids.ppr3)
  {
    tlv_data.ppr_mask |= UIM_PPR_PROFILE_DELETE_ON_DISABLE_V01;
  }

  return qmi_svc_put_param_tlv(response_pptr,
                               tlv_id,
                               sizeof(tlv_data),
                               (void *)&tlv_data);

} /* qmi_uim_response_ppr_info */


/*===========================================================================
  FUNCTION QMI_UIM_RESPONSE_PROFILE_POLICY_RULES()

  DESCRIPTION
    Composes the TLV content of profile policy rules

  PARAMETERS
    tlv_id                 : id to be used for the TLV
    response_pptr          : output sdu
    profile_policy_rules   : Profile Policy Rules

  RETURN VALUE
    TRUE on success. FALSE on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
boolean qmi_uim_response_profile_policy_rules
(
  byte                                   tlv_id,
  dsm_item_type **                       response_pptr,
  lpa_request_user_consent_event_type    event_data
)
{
  PACKED struct PACKED_POST
  {
    uint64 ppr_mask;
    uint8  user_consent_needed;
  }tlv_data;

  ASSERT(response_pptr);

  tlv_data.ppr_mask = 0;
  tlv_data.user_consent_needed = 0;

  if (event_data.ppr_ids.ppr1)
  {
    tlv_data.ppr_mask |= UIM_PPR_PROFILE_DISABLE_NOT_ALLOWED_V01;
  }

  if (event_data.ppr_ids.ppr2)
  {
    tlv_data.ppr_mask |= UIM_PPR_PROFILE_DELETE_NOT_ALLOWED_V01;
  }

  if (event_data.ppr_ids.ppr3)
  {
    tlv_data.ppr_mask |= UIM_PPR_PROFILE_DELETE_ON_DISABLE_V01;
  }

  tlv_data.user_consent_needed = (event_data.provide_user_consent) ? 1 : 0;

  return qmi_svc_put_param_tlv(response_pptr,
                               tlv_id,
                               sizeof(tlv_data),
                               (void *)&tlv_data);
} /* qmi_uim_response_profile_policy_rules */


/*===========================================================================
  FUNCTION QMI_UIM_TLV_PROFILE_ID()

  DESCRIPTION
    Parses the TLV item of sim_profile

  PARAMETERS
    sdu_in                   : input sdu
    len                      : length of the TLV item
    extended_card_status_ptr : output indicating if extended card status
                               is requested
  RETURN VALUE
    QMI_ERR_NONE on success. Other error code on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
qmi_error_e_type qmi_uim_tlv_profile_id
(
  dsm_item_type **                   sdu_in,
  uint16                             len,
  qmi_uim_tlv_sim_profile_type      *sim_profile_ptr
)
{
  int32             temp;

  PACKED struct PACKED_POST
  {
    uint32  profile_id;
  } tlv_data;

  ASSERT(sdu_in && sim_profile_ptr);

  /* Check length */
  if (len != sizeof(tlv_data))
  {
    return QMI_ERR_MALFORMED_MSG;
  }

  /* Extract data */
  temp = dsm_pullup(sdu_in, (void*)&tlv_data, len);
  if (temp != len)
  {
    return QMI_ERR_MALFORMED_MSG;
  }

  /* Extended card status flag */
  switch (tlv_data.profile_id)
  {
    case UIM_PROFILE_ID_1_V01:
      sim_profile_ptr->profile_id = MMGSDI_PROFILE_1;
      break;
    case UIM_PROFILE_ID_2_V01:
      sim_profile_ptr->profile_id = MMGSDI_PROFILE_2;
      break;
    case UIM_PROFILE_ID_3_V01:
      sim_profile_ptr->profile_id = MMGSDI_PROFILE_3;
      break;
    case UIM_PROFILE_ID_4_V01:
      sim_profile_ptr->profile_id = MMGSDI_PROFILE_4;
      break;
    case UIM_PROFILE_ID_5_V01:
      sim_profile_ptr->profile_id = MMGSDI_PROFILE_5;
      break;
    case UIM_PROFILE_ID_6_V01:
      sim_profile_ptr->profile_id = MMGSDI_PROFILE_6;
      break;
    case UIM_PROFILE_ID_7_V01:
      sim_profile_ptr->profile_id = MMGSDI_PROFILE_7;
      break;
    case UIM_PROFILE_ID_8_V01:
      sim_profile_ptr->profile_id = MMGSDI_PROFILE_8;
      break;
    default:
      return QMI_ERR_INVALID_ARG;
  }

  return QMI_ERR_NONE;
} /* qmi_uim_tlv_profile_id */


/*===========================================================================
  FUNCTION QMI_UIM_TLV_EUICC_PROFILE_NAME()

  DESCRIPTION
    Parses the TLV item of profile name or nickname

  PARAMETERS
    sdu_in                      : input sdu
    len                         : length of the TLV item
    euicc_profile_name_data_ptr : output with profile name

  RETURN VALUE
    QMI_ERR_NONE on success. Other error code on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
qmi_error_e_type qmi_uim_tlv_euicc_profile_name
(
  dsm_item_type **                           sdu_in,
  uint16                                     len,
  qmi_uim_tlv_euicc_name_type              * euicc_profile_name_data_ptr
)
{
  int32             temp;

  PACKED struct PACKED_POST
  {
    uint8 length;
  } tlv_data;

  ASSERT(sdu_in && euicc_profile_name_data_ptr);

  /* Check length */
  if (len < sizeof(tlv_data))
  {
    return QMI_ERR_MALFORMED_MSG;
  }

  /* Extract data */
  temp = dsm_pullup(sdu_in, (void*)&tlv_data, sizeof(tlv_data));
  if (temp != sizeof(tlv_data))
  {
    return QMI_ERR_MALFORMED_MSG;
  }

  /* Length */
  euicc_profile_name_data_ptr->euicc_name_data.name_len = tlv_data.length;

  /* Check data length */
  if (len != sizeof(tlv_data) + tlv_data.length)
  {
    return QMI_ERR_MALFORMED_MSG;
  }

  if (tlv_data.length > LPA_NAME_LEN_MAX)
  {
    return QMI_ERR_ARG_TOO_LONG;
  }

  /* Extract body to write. The memory allocated here needs to be freed by
     the caller, after using the data */
  if (tlv_data.length > 0)
  {
    temp = dsm_pullup(sdu_in,
                      (void*)euicc_profile_name_data_ptr->euicc_name_data.name,
                      tlv_data.length);
    if (temp != tlv_data.length)
    {
      return QMI_ERR_MALFORMED_MSG;
    }
  }

  return QMI_ERR_NONE;
} /* qmi_uim_tlv_euicc_profile_name */


/*===========================================================================
  FUNCTION QMI_UIM_TLV_EUICC_DOWNLOAD_CODE()

  DESCRIPTION
    Parses the TLV item of add profile activation or confirmation code

  PARAMETERS
    sdu_in                      : input sdu
    len                         : length of the TLV item
    temporary_identity_mask_ptr : output with temporary identity mask value

  RETURN VALUE
    QMI_ERR_NONE on success. Other error code on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
qmi_error_e_type qmi_uim_tlv_euicc_download_code
(
  dsm_item_type **                           sdu_in,
  uint16                                     len,
  qmi_uim_tlv_download_code_type           * euicc_download_code_ptr
)
{
  int32             temp;

  PACKED struct PACKED_POST
  {
    uint16 length;
  } tlv_data;

  ASSERT(sdu_in && euicc_download_code_ptr);

  /* Check length */
  if (len < sizeof(tlv_data))
  {
    return QMI_ERR_MALFORMED_MSG;
  }

  /* Extract data */
  temp = dsm_pullup(sdu_in, (void*)&tlv_data, sizeof(tlv_data));
  if (temp != sizeof(tlv_data))
  {
    return QMI_ERR_MALFORMED_MSG;
  }

  /* Length */
  euicc_download_code_ptr->code.data_len = tlv_data.length;

  /* Check data length */
  if (len != sizeof(tlv_data) + tlv_data.length)
  {
    return QMI_ERR_MALFORMED_MSG;
  }

  if (tlv_data.length > QMI_UIM_EUICC_DOWNLOAD_CODE_MAX_V01)
  {
    return QMI_ERR_ARG_TOO_LONG;
  }

  /* Extract body to write. The memory allocated here needs to be freed by
     the caller, after using the data */
  if (tlv_data.length > 0)
  {
    euicc_download_code_ptr->code.data_ptr =
          (uint8*)uimqmi_malloc(tlv_data.length);
    if (euicc_download_code_ptr->code.data_ptr == NULL)
    {
      return QMI_ERR_NO_MEMORY;
    }

    temp = dsm_pullup(sdu_in,
                      (void*)euicc_download_code_ptr->code.data_ptr,
                      tlv_data.length);
    if (temp != tlv_data.length)
    {
      return QMI_ERR_MALFORMED_MSG;
    }
  }

  return QMI_ERR_NONE;
} /* qmi_uim_tlv_euicc_download_code */


/*===========================================================================
  FUNCTION QMI_UIM_TLV_RESET_OPTION_MASK()

  DESCRIPTION
    Parses the TLV item for euicc reset option mask

  PARAMETERS
    sdu_in                    : input sdu
    len                       : length of the TLV item
    reset_option_mask_ptr     : output reset option mask

  RETURN VALUE
    QMI_ERR_NONE on success. Other error code on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
qmi_error_e_type qmi_uim_tlv_euicc_reset_option_mask
(
  dsm_item_type                          ** sdu_in,
  uint16                                    len,
  qmi_uim_tlv_reset_option_mask_type      * reset_option_mask_ptr
)
{
  int32             temp;

  PACKED struct PACKED_POST
  {
    uint64  reset_option_mask;
  } tlv_data;

  ASSERT(sdu_in);
  ASSERT(reset_option_mask_ptr);

  /* Check length */
  if (len != sizeof(tlv_data))
  {
    return QMI_ERR_MALFORMED_MSG;
  }

  /* Extract data */
  temp = dsm_pullup(sdu_in, (void*)&tlv_data, len);
  if (temp != len)
  {
    return QMI_ERR_MALFORMED_MSG;
  }

  reset_option_mask_ptr->euicc_reset_option.reset_test_profiles      =
           (tlv_data.reset_option_mask & UIM_RESET_TEST_PROFILES_V01) ? TRUE : FALSE;
  reset_option_mask_ptr->euicc_reset_option.reset_operational_profiles   =
           (tlv_data.reset_option_mask & UIM_RESET_OPERATIONAL_PROFILES_V01) ? TRUE : FALSE;
  reset_option_mask_ptr->euicc_reset_option.reset_smdp_address =
           (tlv_data.reset_option_mask & UIM_RESET_TO_DEFAULT_SMDP_ADDRESS_V01) ? TRUE: FALSE;

  return QMI_ERR_NONE;
} /* qmi_uim_tlv_euicc_reset_option_mask */


/*===========================================================================
  FUNCTION QMI_UIM_TLV_EUICC_DEFAULT_ADDRESS()

  DESCRIPTION
    Parses the TLV item of default SMDP address

  PARAMETERS
    sdu_in                      : input sdu
    len                         : length of the TLV item
    euicc_default_address_ptr   : output with SM DP address

  RETURN VALUE
    QMI_ERR_NONE on success. Other error code on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
qmi_error_e_type qmi_uim_tlv_euicc_default_address
(
  dsm_item_type **                           sdu_in,
  uint16                                     len,
  qmi_uim_tlv_euicc_default_address_type   * euicc_default_address_ptr
)
{
  int32             temp;

  PACKED struct PACKED_POST
  {
    uint8 length;
  } tlv_data;

  ASSERT(sdu_in && euicc_default_address_ptr);

  /* Check length */
  if (len < sizeof(tlv_data))
  {
    return QMI_ERR_MALFORMED_MSG;
  }

  /* Extract data */
  temp = dsm_pullup(sdu_in, (void*)&tlv_data, sizeof(tlv_data));
  if (temp != sizeof(tlv_data))
  {
    return QMI_ERR_MALFORMED_MSG;
  }

  /* Length */
  euicc_default_address_ptr->smdp_address.data_len = tlv_data.length;

  /* Check data length */
  if (len != sizeof(tlv_data) + tlv_data.length)
  {
    return QMI_ERR_MALFORMED_MSG;
  }

  /* Extract body to write. The memory allocated here needs to be freed by
     the caller, after using the data */
  if (tlv_data.length > 0)
  {
    euicc_default_address_ptr->smdp_address.data_ptr =
          (uint8*)uimqmi_malloc(tlv_data.length);
    if (euicc_default_address_ptr->smdp_address.data_ptr == NULL)
    {
      return QMI_ERR_NO_MEMORY;
    }

    temp = dsm_pullup(sdu_in,
                      (void*)euicc_default_address_ptr->smdp_address.data_ptr,
                      tlv_data.length);
    if (temp != tlv_data.length)
    {
      return QMI_ERR_MALFORMED_MSG;
    }
  }

  return QMI_ERR_NONE;
} /* qmi_uim_tlv_euicc_default_address */
