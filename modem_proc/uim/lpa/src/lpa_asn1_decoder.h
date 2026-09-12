#ifndef LPA_ASN1_DECODER_H
#define LPA_ASN1_DECODER_H
/*===========================================================================


            L P A   A S N 1   D E C O D E R S


===========================================================================*/

/*===========================================================================
                        COPYRIGHT INFORMATION

Copyright (c) 2016, 2018 QUALCOMM Technologies, Inc (QTI) and its licensors.
All Rights Reserved.  QUALCOMM Technologies Proprietary.
Export of this technology or software
is regulated by the U.S. Government. Diversion contrary to U.S. law prohibited.

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/uim.mpss/6.1.0/lpa/src/lpa_asn1_decoder.h#3 $ $DateTime: 2019/12/27 02:07:35 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
03/29/18   bcho    LPA phase2.1 changes
04/12/17   av      Add support to configure/retrieve default server address
12/07/16   ll      LPA phase2 initial changes
09/22/16   ll      Peek into profile installation result
03/28/16   av      Initial revision
===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "uim_variation.h"

#include "lpa.h"
#include "lpa_util.h"
#include "mmgsdilib_common.h"
#include "lpa_mmgsdi.h"
#include "lpa_lpd.h"
#include "lpa_notification.h"

/*=============================================================================

                     FUNCTION DECLARATIONS

=============================================================================*/
/*===========================================================================
FUNCTION LPA_ASN1_DECODE_SELECT_ISDR_RESP

DESCRIPTION
  Function parses the ASN formatted response received as a result of
  select ISDR and decodes the SVN and lpae related info.
DEPENDENCIES
  None

RETURN VALUE
  lpa_result_enum_type

SIDE EFFECTS
  None
===========================================================================*/
lpa_result_enum_type lpa_asn1_decode_select_isdr_resp (
  uint32                                data_len,
  uint8                                 *data_ptr,
  lpa_mmgsdi_select_isdr_resp_type      *resp_ptr
);

/*===========================================================================
FUNCTION LPA_ASN1_DECODE_EUICC_CONFIG_ADDRESS_RESP

DESCRIPTION
  Function parses the ASN formatted response received as a result of
  get or set server address (configured on the eUICC) request and decodes:
  - defaultDpAddress(optional) and rootDsAddress if it is a GET request
  - Result if it is a SET request
DEPENDENCIES
  None

RETURN VALUE
  lpa_result_enum_type

SIDE EFFECTS
  None
===========================================================================*/
lpa_result_enum_type lpa_asn1_decode_euicc_config_address_resp (
  uint32                                           data_len,
  uint8                                           *data_ptr,
  lpa_default_server_address_resp_type            *resp_ptr
);

/*===========================================================================
FUNCTION LPA_ASN1_DECODE_GET_EUICC_CHALLENGE_RESP

DESCRIPTION
  Function parses the ASN formatted response received as a result of
  get euicc challenge request response.

DEPENDENCIES
  None

RETURN VALUE
  lpa_result_enum_type

SIDE EFFECTS
  None
===========================================================================*/
lpa_result_enum_type lpa_asn1_decode_get_euicc_challenge_resp (
  uint32                                data_len,
  uint8                                 *data_ptr,
  lpa_lpd_get_euicc_challenge_resp_type *resp_ptr
);

/*===========================================================================
FUNCTION LPA_ASN1_DECODE_GET_EUICC_INFO_RESP

DESCRIPTION
  Function parses the ASN formatted response received as a result of
  get euicc challenge1/2 request response and decodes svn and corresponding
  euicc info 1/2.

DEPENDENCIES
  None

RETURN VALUE
  lpa_result_enum_type

SIDE EFFECTS
  None
===========================================================================*/
lpa_result_enum_type lpa_asn1_decode_get_euicc_info_resp (
  uint32                                data_len,
  uint8                                 *data_ptr,
  lpa_lpd_get_euicc_info_resp_type      *resp_ptr
);

/*===========================================================================
FUNCTION LPA_ASN1_DECODE_RETRIEVE_NOTIFICATION_LIST_RESP

DESCRIPTION
  Function parses the ASN formatted response received as a result of
  retrieve notification list request response

DEPENDENCIES
  None

RETURN VALUE
  lpa_result_enum_type

SIDE EFFECTS
  None
===========================================================================*/
lpa_result_enum_type lpa_asn1_decode_retrieve_notification_list_resp (
  uint32                                                 data_len,
  uint8                                                  *data_ptr,
  lpa_notification_retrieve_notification_list_resp_type  *resp_ptr
);

/*===========================================================================
FUNCTION LPA_ASN1_DECODE_REMOVE_NOTIFICATION_LIST_RESP

DESCRIPTION
  Function parses the ASN formatted response received as a result of
  remove notification request response and extract the remove notification
  result as return value
DEPENDENCIES
  None

RETURN VALUE
  lpa_result_enum_type

SIDE EFFECTS
  None
===========================================================================*/
lpa_result_enum_type lpa_asn1_decode_remove_notification_from_list_resp (
  uint32                                               data_len,
  uint8                                                *data_ptr
);

/*===========================================================================
FUNCTION LPA_ASN1_DECODE_GET_PROFILES_INFO_RESP

DESCRIPTION
  Function parses the ASN formatted response received as a result of
  get profiles info request response
DEPENDENCIES
  None

RETURN VALUE
  lpa_result_enum_type

SIDE EFFECTS
  None
===========================================================================*/
lpa_result_enum_type lpa_asn1_decode_get_profiles_info_resp (
  uint32                                data_len,
  uint8                                 *data_ptr,
  lpa_get_profiles_info_resp_type       *resp_ptr
);

/*===========================================================================
FUNCTION LPA_ASN1_DECODE_ENABLE_PROFILE_RESP

DESCRIPTION
  Function parses the ASN formatted response received as a result of
  profile management request response for enable
DEPENDENCIES
  None

RETURN VALUE
  lpa_result_enum_type

SIDE EFFECTS
  None
===========================================================================*/
lpa_result_enum_type lpa_asn1_decode_enable_profile_resp (
  uint32                                data_len,
  uint8                                 *data_ptr
);

/*===========================================================================
FUNCTION LPA_ASN1_DECODE_DISABLE_PROFILE_RESP

DESCRIPTION
  Function parses the ASN formatted response received as a result of
  profile management request response for disable
DEPENDENCIES
  None

RETURN VALUE
  lpa_result_enum_type

SIDE EFFECTS
  None
===========================================================================*/
lpa_result_enum_type lpa_asn1_decode_disable_profile_resp (
  uint32                                data_len,
  uint8                                 *data_ptr
);

/*===========================================================================
FUNCTION LPA_ASN1_DECODE_DELETE_PROFILE_RESP

DESCRIPTION
  Function parses the ASN formatted response received as a result of
  profile management request response for delete
DEPENDENCIES
  None

RETURN VALUE
  lpa_result_enum_type

SIDE EFFECTS
  None
===========================================================================*/
lpa_result_enum_type lpa_asn1_decode_delete_profile_resp (
  uint32                                data_len,
  uint8                                 *data_ptr
);

/*===========================================================================
FUNCTION LPA_ASN1_DECODE_EUICC_MEMORY_RESET_RESP

DESCRIPTION
  Function parses the ASN formatted response received as a result of
  euicc memory reset request response
DEPENDENCIES
  None

RETURN VALUE
  lpa_result_enum_type

SIDE EFFECTS
  None
===========================================================================*/
lpa_result_enum_type lpa_asn1_decode_euicc_memory_reset_resp (
  uint32                                data_len,
  uint8                                 *data_ptr
);

/*===========================================================================
FUNCTION LPA_ASN1_DECODE_GET_EID_RESP

DESCRIPTION
  Function parses the ASN formatted response received as a result of
  get eid request response and extract the eid
DEPENDENCIES
  None

RETURN VALUE
  lpa_result_enum_type

SIDE EFFECTS
  None
===========================================================================*/
lpa_result_enum_type lpa_asn1_decode_get_eid_resp (
  uint32                                data_len,
  uint8                                 *data_ptr,
  lpa_get_eid_resp_type                 *resp_ptr
);

/*===========================================================================
FUNCTION LPA_ASN1_DECODE_SET_NICKNAME_RESP

DESCRIPTION
  Function parses the ASN formatted response received as a result of
  set nickname request response
DEPENDENCIES
  None

RETURN VALUE
  lpa_result_enum_type

SIDE EFFECTS
  None
===========================================================================*/
lpa_result_enum_type lpa_asn1_decode_set_nickname_resp (
  uint32                                data_len,
  uint8                                 *data_ptr
);

/*===========================================================================
FUNCTION LPA_ASN1_DECODE_GET_RAT_RESP

DESCRIPTION
  Function parses the ASN formatted response received as a result of
  get rat request response and populate the lpa_get_rat_resp_type

DEPENDENCIES
  None

RETURN VALUE
  lpa_result_enum_type

SIDE EFFECTS
  None
===========================================================================*/
lpa_result_enum_type lpa_asn1_decode_get_rat_resp (
  uint32                                data_len,
  uint8                                 *data_ptr,
  lpa_get_rat_resp_type                 *resp_ptr
);

/*===========================================================================
FUNCTION LPA_ASN1_DECODE_PROFILE_INSTALLATION_RESULT_PHASE1

DESCRIPTION
  Function parses the ASN1 formatted response received as a result of
  LoadBoundProfilePackage or GetProfileInstallationResult request.

DEPENDENCIES
  None

RETURN VALUE
  lpa_result_enum_type

SIDE EFFECTS
  None
===========================================================================*/
lpa_result_enum_type lpa_asn1_decode_profile_installation_result_phase1 (
  uint32                                              data_len,
  uint8                                               *data_ptr,
  lpa_lpd_profile_installation_result_code_array_type result_code_array
);

/*===========================================================================
FUNCTION LPA_ASN1_DECODE_PROFILE_INSTALLATION_RESULT

DESCRIPTION
  Function parses the ASN1 formatted APDU response received as a result of
  LoadBoundProfilePackage or GetProfileInstallationResult request.

DEPENDENCIES
  None

RETURN VALUE
  lpa_result_enum_type

SIDE EFFECTS
  None
===========================================================================*/
lpa_result_enum_type lpa_asn1_decode_profile_installation_result (
  uint32                                              data_len,
  uint8                                               *data_ptr,
  lpa_lpd_profile_installation_result_code_array_type result_code_array
);

/*===========================================================================
FUNCTION LPA_ASN1_DECODE_AUTHENTICATE_CLIENT_RESP

DESCRIPTION
  Function parses the ASN1 formatted response received as a result of
  AuthenticateClient request from SMDP. it returns ppr_ids and operator_ids
  in profileMetadata.

DEPENDENCIES
  None

RETURN VALUE
  lpa_result_enum_type

SIDE EFFECTS
  None
===========================================================================*/
lpa_result_enum_type lpa_asn1_decode_authenticate_client_resp (
  uint32                                              data_len,
  uint8                                               *data_ptr,
  lpa_ppr_ids_type                                    *ppr_ids,
  lpa_operator_id_type                                *operator_id
);

/*===========================================================================
FUNCTION LPA_ASN1_DECODE_PROFILE_METADATA

DESCRIPTION
  Function parses the ASN1 formatted response received as a result of
  ES9+ES9+.InitiateAuthentication JSON request. it returns operator_id
  and ppr_ids in profileMetadata.

DEPENDENCIES
  None

RETURN VALUE
  lpa_result_enum_type

SIDE EFFECTS
  None
===========================================================================*/
lpa_result_enum_type lpa_asn1_decode_profile_metadata (
  uint32                                              data_len,
  uint8                                               *data_ptr,
  lpa_operator_id_type                                *operator_id,
  lpa_ppr_ids_type                                    *ppr_ids
);

/*===========================================================================
FUNCTION LPA_ASN1_DECODE_BPP

DESCRIPTION
  Function parses the ASN1 formatted BoundProfilePackage .

DEPENDENCIES

RETURN VALUE
  lpa_result_enum_type

SIDE EFFECTS
  None
===========================================================================*/
lpa_result_enum_type lpa_asn1_decode_bpp(
  uint32                                             data_len,
  uint8                                              *data_ptr,
  lpa_bpp_tlvs_data_type                             *bpp_tlv_ptr,
  uint8                                              *meta_data_start_index_ptr,
  uint8                                              *meta_data_total_index_ptr
);
#endif /* LPA_ASN1_DECODER_H */

