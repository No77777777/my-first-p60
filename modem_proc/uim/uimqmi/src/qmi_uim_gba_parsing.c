/*===========================================================================

                         Q M I _ U I M _ G B A _ P A R S I N G . C

DESCRIPTION
  This file contains functions for QMI UIM service's GBA TLV parsing functionality.

Copyright (c) 2016 QUALCOMM Technologies, Inc(QTI).
All Rights Reserved.
QUALCOMM Technologies Confidential and Proprietary
===========================================================================*/
/*===========================================================================

                      EDIT HISTORY FOR FILE

$Header: //components/rel/uim.mpss/6.1.0/uimqmi/src/qmi_uim_gba_parsing.c#1 $

when        who    what, where, why
--------    ---    ----------------------------------------------------------
11/04/16    me     Combined multi line ASSERT in one line
07/18/16    dd     Added feature FEATURE_UIM_TEST_FRAMEWORK
06/16/16    vdc    Initial Revision
===========================================================================*/

/*===========================================================================

                          INCLUDE FILES FOR MODULE

===========================================================================*/
#include "uim_variation.h"
#include "comdef.h"
#include "qmi_framework.h"
#include "qmi_uim_parsing.h"
#include "qmi_uim_cat_common.h"
#include "amssassert.h"

/* These includes need to always be at bottom - offtarget use only */
#ifdef FEATURE_UIM_TEST_FRAMEWORK
#error code not present
#endif /* FEATURE_UIM_TEST_FRAMEWORK */

/*===========================================================================

                             EXTERNAL FUNCTIONS

===========================================================================*/

/* Security protocol length */
#define QMI_UIM_SECURITY_PROTOCOL_LEN          (5)

/* Maximum APN length */
#define QMI_UIM_APN_NAME_MAX                   100

/*===========================================================================
  FUNCTION QMI_UIM_TLV_NAF_ID()

  DESCRIPTION
    Parses the TLV item for NAF ID request

  PARAMETERS
    sdu_in                     : input sdu
    len                        : length of the TLV item
    gba_naf_id_ptr             : NAF ID data pointer

  RETURN VALUE
    QMI_ERR_NONE on success. Other error code on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
qmi_error_e_type qmi_uim_tlv_naf_id
(
  dsm_item_type **            sdu_in,
  uint16                      len,
  qmi_uim_tlv_naf_id_type *   gba_naf_id_ptr
)
{
  int32             temp;

  PACKED struct PACKED_POST
  {
    uint8  fqdn_len;
  } tlv_fqdn_len_data;

  PACKED struct PACKED_POST
  {
    uint8  security_protocol_id[QMI_UIM_SECURITY_PROTOCOL_LEN];
  } tlv_sec_protocol_data;

  ASSERT(sdu_in && gba_naf_id_ptr);

  /* Check length for fqdn length */
  if (len < sizeof(tlv_fqdn_len_data) + sizeof(tlv_sec_protocol_data))
  {
    return QMI_ERR_MALFORMED_MSG;
  }

  /* Extract fqdn length data */
  temp = dsm_pullup(sdu_in, (void*)&tlv_fqdn_len_data, sizeof(tlv_fqdn_len_data));
  if (temp != sizeof(tlv_fqdn_len_data))
  {
    return QMI_ERR_MALFORMED_MSG;
  }

  /* Check the total length. Check for fqdn_len is not required as the maximum
     value it can have is QMI_UIM_FQDN_MAX_V01(255) */
  if(len != sizeof(tlv_fqdn_len_data) +
            tlv_fqdn_len_data.fqdn_len +
            sizeof(tlv_sec_protocol_data))
  {
    return QMI_ERR_MALFORMED_MSG;
  }

  gba_naf_id_ptr->naf_id_ptr = (gba_naf_id_type *)uimqmi_malloc(sizeof(gba_naf_id_type));
  if (gba_naf_id_ptr->naf_id_ptr == NULL)
  {
    return QMI_ERR_NO_MEMORY;
  }

  /* fqdn length */
  gba_naf_id_ptr->naf_id_ptr->fqdn.data_len = tlv_fqdn_len_data.fqdn_len;

  if(tlv_fqdn_len_data.fqdn_len > 0)
  {
    /* Extract fqdn data */
    temp = dsm_pullup(sdu_in,
                      (void*)gba_naf_id_ptr->naf_id_ptr->fqdn.data,
                      tlv_fqdn_len_data.fqdn_len);
    if (temp != tlv_fqdn_len_data.fqdn_len)
    {
      return QMI_ERR_MALFORMED_MSG;
    }
  }

  /* Extract security protocol id */
  temp = dsm_pullup(sdu_in,
                    (void*)gba_naf_id_ptr->naf_id_ptr->security_protocol,
                    sizeof(tlv_sec_protocol_data));
  if (temp != sizeof(tlv_sec_protocol_data))
  {
    return QMI_ERR_MALFORMED_MSG;
  }

  return QMI_ERR_NONE;
} /* qmi_uim_tlv_naf_id */


/*===========================================================================
  FUNCTION QMI_UIM_TLV_APN()

  DESCRIPTION
    Parses the TLV item for Access Point Name request

  PARAMETERS
    sdu_in                     : input sdu
    len                        : length of the TLV item
    apn_ptr                    : APN data pointer

  RETURN VALUE
    QMI_ERR_NONE on success. Other error code on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
qmi_error_e_type qmi_uim_tlv_apn
(
  dsm_item_type **        sdu_in,
  uint16                  len,
  qmi_uim_tlv_apn_type  * apn_ptr
)
{
  int32             temp;

  PACKED struct PACKED_POST
  {
    uint8  apn_len;
  } tlv_apn_len_data;

  ASSERT(sdu_in && apn_ptr);

  /* Check length for APN length */
  if (len < sizeof(tlv_apn_len_data))
  {
    return QMI_ERR_MALFORMED_MSG;
  }

  /* Extract APN length data */
  temp = dsm_pullup(sdu_in, (void*)&tlv_apn_len_data, sizeof(tlv_apn_len_data));
  if (temp != sizeof(tlv_apn_len_data))
  {
    return QMI_ERR_MALFORMED_MSG;
  }

  /* Check the total length */
  if(tlv_apn_len_data.apn_len > QMI_UIM_APN_NAME_MAX ||
     len != sizeof(tlv_apn_len_data) + tlv_apn_len_data.apn_len)
  {
    return QMI_ERR_MALFORMED_MSG;
  }

  if(tlv_apn_len_data.apn_len > 0)
  {
    apn_ptr->apn_data_ptr = (char *)uimqmi_malloc(tlv_apn_len_data.apn_len + 1);
    if (apn_ptr->apn_data_ptr == NULL)
    {
      return QMI_ERR_NO_MEMORY;
    }

    /* Extract APN data */
    temp = dsm_pullup(sdu_in,
                      (void*)apn_ptr->apn_data_ptr,
                      tlv_apn_len_data.apn_len);
    if (temp != tlv_apn_len_data.apn_len)
    {
      return QMI_ERR_MALFORMED_MSG;
    }

    /* Terminate the APN with NULL */
    apn_ptr->apn_data_ptr[tlv_apn_len_data.apn_len] = '\0';
  }

  return QMI_ERR_NONE;
} /* qmi_uim_tlv_apn */


/*===========================================================================
  FUNCTION QMI_UIM_TLV_BSF_INFO()

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
qmi_error_e_type qmi_uim_tlv_bsf_info
(
  dsm_item_type **                              sdu_in,
  uint16                                        len,
  qmi_uim_tlv_bsf_info_type                  *  gba_bsf_info_ptr
)
{
  int32     temp      = 0;

  PACKED struct PACKED_POST
  {
    uint32 protocol;
    uint16 port;
    uint8  address_len;
  } tlv_data;

  ASSERT(sdu_in && gba_bsf_info_ptr);

  /* Check length */
  if (len < sizeof(tlv_data))
  {
    return QMI_ERR_MALFORMED_MSG;
  }

  /* Extract data and ensure it is formed properly */
  temp = dsm_pullup(sdu_in, (void*)&tlv_data, sizeof(tlv_data));
  if (temp != sizeof(tlv_data))
  {
    return QMI_ERR_MALFORMED_MSG;
  }

  if (tlv_data.address_len == 0 ||
      len != (sizeof(tlv_data) + tlv_data.address_len))
  {
    return QMI_ERR_MALFORMED_MSG;
  }

  gba_bsf_info_ptr->bsf_ptr = (gba_bsf_info_type *)uimqmi_malloc(sizeof(gba_bsf_info_type));
  if (gba_bsf_info_ptr->bsf_ptr == NULL)
  {
    return QMI_ERR_NO_MEMORY;
  }

  switch (tlv_data.protocol)
  {
    case UIM_ADDRESS_PROTOCOL_HTTP_V01:
      gba_bsf_info_ptr->bsf_ptr->protocol = GBA_BSF_PROTOCOL_HTTP;
      break;
    case UIM_ADDRESS_PROTOCOL_HTTPS_V01:
      gba_bsf_info_ptr->bsf_ptr->protocol = GBA_BSF_PROTOCOL_HTTPS;
      break;
    default:
      return QMI_ERR_MALFORMED_MSG;
  }

  gba_bsf_info_ptr->bsf_ptr->port = tlv_data.port;

  /* Extract the config data */
  temp = dsm_pullup(sdu_in,
                    (void*)gba_bsf_info_ptr->bsf_ptr->address.data,
                    tlv_data.address_len);
  if (temp != tlv_data.address_len)
  {
    return QMI_ERR_MALFORMED_MSG;
  }

  gba_bsf_info_ptr->bsf_ptr->address.data_len = tlv_data.address_len;

  return QMI_ERR_NONE;
} /* qmi_uim_tlv_bsf_info */

