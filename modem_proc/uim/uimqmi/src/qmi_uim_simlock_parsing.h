/*================================================================================

                         Q M I _ U I M _ S I M L O C K _ P A R S I N G. H


DESCRIPTION
  This file contains functions for QMI UIM service's SIMLOCK TLV parsing functionality.

Copyright (c) 2016-2017, 2019 QUALCOMM Technologies, Inc(QTI).
All Rights Reserved.
QUALCOMM Technologies Confidential and Proprietary

$Header: //components/rel/uim.mpss/6.1.0/uimqmi/src/qmi_uim_simlock_parsing.h#2 $

when       who     what, where, why
--------   ---     -----------------------------------------------------------
09/10/19    vdc    SimLock solution for activation at Point Of Sale
07/13/17    av     Enhanced simlock RSU solution
10/06/16    ar     Added support for SP + EHPLMN simlock
06/16/16    vdc    Initial Revision
=============================================================================*/

#include "uim_variation.h"
#include "customer.h"
#include "uim_msg.h"
#include "qmi_uim_parsing.h"
#include "simlock_common.h"

#ifdef FEATURE_SIMLOCK
/*===========================================================================
  FUNCTION QMI_UIM_TLV_DEPERSONALIZATION()

  DESCRIPTION
    Parses the TLV item of deactivate perso

  PARAMETERS
    sdu_in                : input sdu
    len                   : length of the TLV item
    depersonalization_ptr : output with deactivate perso value

  RETURN VALUE
    QMI_ERR_NONE on success. Other error code on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
qmi_error_e_type qmi_uim_tlv_depersonalization
(
  dsm_item_type **                     sdu_in,
  uint16                               len,
  qmi_uim_tlv_depersonalization_type * depersonalization_ptr
);

/*===========================================================================
  FUNCTION QMI_UIM_TLV_PERSONALIZATION_CK()

  DESCRIPTION
    Parses the CK TLV item of activate perso

  PARAMETERS
    sdu_in                : input sdu
    len                   : length of the TLV item
    personalization_ptr   : output with activate perso value

  RETURN VALUE
    QMI_ERR_NONE on success. Other error code on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
qmi_error_e_type qmi_uim_tlv_personalization_ck
(
  dsm_item_type **                        sdu_in,
  uint16                                  len,
  qmi_uim_tlv_personalization_ck_type   * personalization_ck_ptr
);

/*===========================================================================
  FUNCTION QMI_UIM_TLV_PERSONALIZATION_CODE()

  DESCRIPTION
    Parses the lock code TLV item of activate perso

  PARAMETERS
    sdu_in                : input sdu
    len                   : length of the TLV item
    category              : simlock category
    extended_category     : type of category. normal, ext or ext2
    personalization_ptr   : output with activate perso value

  RETURN VALUE
    QMI_ERR_NONE on success. Other error code on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
qmi_error_e_type qmi_uim_tlv_personalization_code
(
  dsm_item_type **                               sdu_in,
  uint16                                         len,
  simlock_category_enum_type                     category,
  qmi_uim_simlock_extension_category_type        extended_category,
  qmi_uim_tlv_personalization_code_type        * personalization_code_ptr
);

/*===========================================================================
  FUNCTION QMI_UIM_TLV_DECRYPTED_PERSO_SECURE()

  DESCRIPTION
    Parses the TLV item for encrypted perso request

  PARAMETERS
    sdu_in                    : input sdu
    len                       : length of the TLV item
    decoded_perso_ptr         : output decoded perso secure data

  RETURN VALUE
    QMI_ERR_NONE on success. Other error code on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
qmi_error_e_type qmi_uim_tlv_decrypted_perso_secure
(
  dsm_item_type **                           sdu_in,
  uint16                                     len,
  qmi_uim_tlv_decrypted_perso_secure_type *  decoded_perso_ptr
);

/*===========================================================================
  FUNCTION QMI_UIM_TLV_DECRYPTED_DEPERSO_SECURE()

  DESCRIPTION
    Parses the TLV item for encrypted deperso request

  PARAMETERS
    sdu_in                    : input sdu
    len                       : length of the TLV item
    decoded_perso_ptr         : output decoded perso secure data

  RETURN VALUE
    QMI_ERR_NONE on success. Other error code on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
qmi_error_e_type qmi_uim_tlv_decrypted_deperso_secure
(
  dsm_item_type **                             sdu_in,
  uint16                                       len,
  qmi_uim_tlv_decrypted_deperso_secure_type *  decoded_deperso_ptr
);

/*===========================================================================
  FUNCTION QMI_UIM_TLV_DECRYPTED_EMERGENCY_ONLY()

  DESCRIPTION
    Parses the TLV item for encrypted emergency only request

  PARAMETERS
    sdu_in                    : input sdu
    len                       : length of the TLV item
    decoded_perso_ptr         : output decoded perso secure data

  RETURN VALUE
    QMI_ERR_NONE on success. Other error code on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
qmi_error_e_type qmi_uim_tlv_decrypted_emergency_only
(
  dsm_item_type **                             sdu_in,
  uint16                                       len,
  qmi_uim_tlv_decrypted_emergency_only_type *  decoded_emergency_only_ptr
);

/*===========================================================================
  FUNCTION QMI_UIM_TLV_DECRYPTED_SIMLOCK_CONFIG()

  DESCRIPTION
    Parses the TLV item for encrypted simlock config request

  PARAMETERS
    sdu_in                     : input sdu
    len                        : length of the TLV item
    decoded_simlock_config_ptr : output decoded simlock config data

  RETURN VALUE
    QMI_ERR_NONE on success. Other error code on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
qmi_error_e_type qmi_uim_tlv_decrypted_simlock_config
(
  dsm_item_type **                             sdu_in,
  uint16                                       len,
  qmi_uim_tlv_decrypted_simlock_config_type *  decoded_simlock_config_ptr
);

/*===========================================================================
  FUNCTION QMI_UIM_TLV_PERSONALIZATION_CATEGORY()

  DESCRIPTION
    Parses the TLV item of reactivate or reuse code perso feature

  PARAMETERS
    sdu_in            : input sdu
    len               : length of the TLV item
    category_type_ptr : output with perso feature to reactivate or reuse code

  RETURN VALUE
    QMI_ERR_NONE on success. Other error code on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
qmi_error_e_type qmi_uim_tlv_personalization_category
(
  dsm_item_type **                              sdu_in,
  uint16                                        len,
  qmi_uim_tlv_personalization_category_type   * category_type_ptr
);

/*===========================================================================
  FUNCTION QMI_UIM_TLV_REMOTE_UNLOCK_CONFIG()

  DESCRIPTION
    Parses the TLV item of remote unlock request

  PARAMETERS
    sdu_in            : input sdu
    len               : length of the TLV item
    config_data_ptr   : output with simlock config data to set

  RETURN VALUE
    QMI_ERR_NONE on success. Other error code on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
qmi_error_e_type qmi_uim_tlv_remote_unlock_config
(
  dsm_item_type **                              sdu_in,
  uint16                                        len,
  qmi_uim_tlv_remote_unlock_config_type      *  config_data_ptr
);

/*===========================================================================
  FUNCTION QMI_UIM_TLV_REMOTE_UNLOCK_OPERATION

  DESCRIPTION
    Parses the 'operation' TLV item of remote unlock request

  PARAMETERS
    sdu_in                       : input sdu
    len                          : length of the TLV item
    remote_unlock_operation_ptr  : output with remote simlock operation

  RETURN VALUE
    QMI_ERR_NONE on success. Other error code on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
qmi_error_e_type qmi_uim_tlv_remote_unlock_operation
(
  dsm_item_type **                              sdu_in,
  uint16                                        len,
  qmi_uim_tlv_remote_unlock_operation_type    * remote_unlock_operation_ptr
);

/*===========================================================================
  FUNCTION QMI_UIM_TLV_IMSI()

  DESCRIPTION
    Parses the TLV item of verify IMSI request

  PARAMETERS
    sdu_in            : input sdu
    len               : length of the TLV item
    imsi_data_ptr     : output with imsi data

  RETURN VALUE
    QMI_ERR_NONE on success. Other error code on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
qmi_error_e_type qmi_uim_tlv_imsi
(
  dsm_item_type **                              sdu_in,
  uint16                                        len,
  qmi_uim_tlv_imsi_type                      *  imsi_data_ptr
);

/*===========================================================================
  FUNCTION QMI_UIM_RESPONSE_GET_CONFIGURATION_SIMLOCK()

  DESCRIPTION
    Composes the TLV content of personalization in the
    configuration

  PARAMETERS
    tlv_id              : id to be used for the TLV
    response_pptr       : output sdu
    get_status_ptr      : feature status information

  RETURN VALUE
    TRUE on success. FALSE on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
boolean qmi_uim_response_get_configuration_simlock
(
  byte                                      tlv_id,
  dsm_item_type **                          response_pptr,
  const simlock_get_status_msg_resp_type  * get_status_ptr
);

/*===========================================================================
  FUNCTION QMI_UIM_RESPONSE_SIMLOCK_INACTIVE_CONFIG_STATUS()

  DESCRIPTION
    Composes the TLV content of simlock inactive configuration

  PARAMETERS
    tlv_id              : id to be used for the TLV
    response_pptr       : output sdu
    get_status_ptr      : feature status information

  RETURN VALUE
    TRUE on success. FALSE on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
boolean qmi_uim_response_simlock_inactive_config_status
(
  byte                                      tlv_id,
  dsm_item_type **                          response_pptr,
  const simlock_get_status_msg_resp_type  * get_status_ptr
);

/*===========================================================================
  FUNCTION QMI_UIM_RESPONSE_TEMPORARY_UNLOCK_TIME_LEFT()

  DESCRIPTION
    Composes the TLV content of temporary depersonalization time left

  PARAMETERS
    tlv_id              : id to be used for the TLV
    response_pptr       : output sdu
    num_slots           : number of slots supported
    get_status_ptr      : feature status information

  RETURN VALUE
    TRUE on success. FALSE on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
boolean qmi_uim_response_temporary_unlock_time_left
(
  byte                                      tlv_id,
  dsm_item_type **                          response_pptr,
  uint8                                     num_slots,
  const simlock_get_status_msg_resp_type  * get_status_ptr
);

/*===========================================================================
  FUNCTION QMI_UIM_RESPONSE_PERSO_REACTIVATION_STATUS()

  DESCRIPTION
    Composes the TLV content of perso reactivation status

  PARAMETERS
    tlv_id              : id to be used for the TLV
    response_pptr       : output sdu
    num_slots           : number of slots supported
    get_status_ptr      : feature status information

  RETURN VALUE
    TRUE on success. FALSE on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
boolean qmi_uim_response_perso_reactivation_status
(
  byte                                      tlv_id,
  dsm_item_type **                          response_pptr,
  uint8                                     num_slots,
  const simlock_get_status_msg_resp_type  * get_status_ptr
);

/*===========================================================================
  FUNCTION  QMI_UIM_RESPONSE_TEMPORARY_UNLOCK_STATUS()

  DESCRIPTION
    Prepares the temporary unlock indication based on the
    response of get status response

  PARAMETERS
    tlv_id                       : ID to be used for TLV
    indication_pptr              : Pointer to Indication pointer
    temporary_unlock_status_ptr  : Pointer to the SIMLOCK get status information
    num_slots       : Number of slots supported

  RETURN VALUE
    TRUE        : If the indication can be send to client
    FALSE       : If otherwise

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
boolean qmi_uim_response_temporary_unlock_status
(
  byte                                     tlv_id,
  dsm_item_type                          **indication_pptr,
  const simlock_get_status_msg_resp_type  *temporary_unlock_status_ptr,
  uint8                                    num_slots
);

/*===========================================================================
  FUNCTION QMI_UIM_RESPONSE_PERSO_CONTROL_KEY_PRESENCE()

  DESCRIPTION
    Composes the TLV content of perso control key presence

  PARAMETERS
    tlv_id              : id to be used for the TLV
    response_pptr       : output sdu
    num_slots           : number of slots supported
    get_status_ptr      : feature status information

  RETURN VALUE
    TRUE on success. FALSE on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
boolean qmi_uim_response_perso_control_key_presence
(
  byte                                      tlv_id,
  dsm_item_type **                          response_pptr,
  uint8                                     num_slots,
  const simlock_get_status_msg_resp_type  * get_status_ptr
);

/*===========================================================================
  FUNCTION  QMI_UIM_SIMLOCK_REMOTE_SFS_ENCRYPTED_IND()

  DESCRIPTION
    Encrypts remote SFS operation request using Trust Zone application APIs

  PARAMETERS
    operation     : Operation requested (Read/Write)
    request_id    : Request ID for this request
    file          : The file to perform operation on
    write_data    : The data to be written (valid for write operation)
    resposne_pptr : Pointer to command response pointer

  RETURN VALUE
    TRUE        : If the encrypted response is build successfully
    FALSE       : If otherwise

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
boolean qmi_uim_simlock_remote_sfs_encrypted_ind
(
  uint16                              cmd_id,
  simlock_file_operation_enum_type    operation,
  simlock_token_id_type               request_id,
  simlock_file_type                   file,
  simlock_data_type                   write_data,
  dsm_item_type                    ** response_pptr
);
#endif /* FEATURE_SIMLOCK */

