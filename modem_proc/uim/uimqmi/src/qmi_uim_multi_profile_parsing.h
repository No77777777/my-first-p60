#ifndef QMI_UIM_MULTI_PROFILE_PARSING_H
#define QMI_UIM_MULTI_PROFILE_PARSING_H
/*==============================================================================

             Q M I _ U I M _ M U L T I _ P R O F I L E _ P A R S I N G _ H

DESCRIPTION

 The Data Services Qualcomm MSM Interface Device Management Services source
 file.

Copyright (c) 2016-2017 QUALCOMM Technologies, Inc(QTI).
All Rights Reserved.
QUALCOMM Technologies Confidential and Proprietary
==============================================================================*/
/*==============================================================================

                      EDIT HISTORY FOR FILE

  $Header: //components/rel/uim.mpss/6.1.0/uimqmi/src/qmi_uim_multi_profile_parsing.h#1 $

when        who    what, where, why
--------    ---    ------------------------------------------------------------
04/12/17    ar     Add support for retrieve/configure default server address 
12/07/16    av     LPA phase2 initial changes
06/22/16    sp     Initial Revision.
==============================================================================*/

/*==============================================================================

                          INCLUDE FILES FOR MODULE

==============================================================================*/
#include "qmi_uim_parsing.h"

#include "mmgsdilib_common.h"
#include "lpalib.h"

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
    lpa_event_ptr    : Pointer to LPA event data

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
);

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
);

/*===========================================================================
  FUNCTION QMI_UIM_RESPONSE_EUICC_DOWNLOAD_STATUS_DATA()

  DESCRIPTION
    Composes the TLV content for download progress

  PARAMETERS
    tlv_id           : TLV id to be sent
    response_pptr    : Pointer to response pointer
    lpa_event_ptr    : Pointer to LPA event data
    lpa_status       : LPA download status


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
);

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
);

/*===========================================================================
  FUNCTION QMI_UIM_RESPONSE_GET_ALL_PROFILE_INFO()

  DESCRIPTION
    Composes the TLV content of profile information

  PARAMETERS
    tlv_id                  : id to be used for the TLV
    response_pptr           : output sdu
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
);

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
);

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
);

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
);

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
);

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
);

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
);

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
);

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
);

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
);

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
);

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
);

#endif /* QMI_UIM_MULTI_PROFILE_PARSING_H*/

