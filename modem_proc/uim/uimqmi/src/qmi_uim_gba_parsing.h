#ifndef QMI_UIM_GBA_PARSING_H
#define QMI_UIM_GBA_PARSING_H
/*===========================================================================

                         Q M I _ U I M _ G B A _ P A R S I N G. H

DESCRIPTION
  This file contains functions for QMI UIM service's GBA TLV parsing functionality.

Copyright (c) 2016 QUALCOMM Technologies, Inc(QTI).
All Rights Reserved.
QUALCOMM Technologies Confidential and Proprietary
===========================================================================*/
/*===========================================================================

                      EDIT HISTORY FOR FILE

$Header: //components/rel/uim.mpss/6.1.0/uimqmi/src/qmi_uim_gba_parsing.h#1 $

when        who    what, where, why
--------    ---    ----------------------------------------------------------
06/16/16    vdc    Initial Revision
===========================================================================*/

/*===========================================================================

                          INCLUDE FILES FOR MODULE

===========================================================================*/
#include "uim_variation.h"


/*===========================================================================

                             EXTERNAL FUNCTIONS

===========================================================================*/

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
);

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
);

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
);
#endif /* QMI_UIM_GBA_PARSING_H */

