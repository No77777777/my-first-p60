/*==========================================================================

DESCRIPTION

 The header file for common parsing methods.

Copyright (c) 2015 QUALCOMM Technologies, Inc(QTI).
All Rights Reserved.
QUALCOMM Technologies Confidential and Proprietary

  $Header: //components/rel/uim.mpss/6.1.0/uimqmi/src/qmi_cat_parsing.h#2 $ $DateTime: 2019/10/30 06:56:05 $ $Author: pwbldsvc $

when        who    what, where, why
--------    ---    ----------------------------------------------------------
09/08/15    vr     Initial version
===========================================================================*/

#ifndef QMI_CAT_PARSING_H
#define QMI_CAT_PARSING_H


/*===========================================================================

                          INCLUDE FILES FOR MODULE

===========================================================================*/
#include "customer.h"
#include "comdef.h"
#include "dsm.h"
#include "gstk_exp.h"
#include "card_application_toolkit_v02.h"

/*===========================================================================

                             MACROS

===========================================================================*/

/*===========================================================================
  This macro is used to skip a TLV
===========================================================================*/
#define QMI_CAT_SKIP_TLV(sdu_in, len, result)                                \
        UIM_MSG_HIGH_0("TLV type not required");                             \
        /* We need to pullout this item */                                   \
        if (len != dsm_pullup(sdu_in, NULL, len))                            \
        {                                                                    \
          UIM_MSG_ERR_0("QMI_CAT DSM pullout error");                        \
          result = QMI_ERR_MALFORMED_MSG;                                    \
        }


/*---------------------------------------------------------------------------
  Macro is used to check slot id TLV
---------------------------------------------------------------------------*/
#define QMI_CAT_CHECK_SLOT_ID_TLV(ptr, result)                               \
        /* Check for Slot id TLV range as defined in QMI CAT ICD*/           \
        if (ptr->slot_present &&                                             \
            ptr->slot.slot != 1 &&                                           \
            ptr->slot.slot != 2 &&                                           \
            ptr->slot.slot != 3 &&                                           \
            ptr->slot.slot != 4 &&                                           \
            ptr->slot.slot != 5 )                                            \
        {                                                                    \
          UIM_MSG_ERR_0("QMI_CAT invalid slot id TLV");                      \
          result = QMI_ERR_INVALID_ARG;                                      \
        }


/*===========================================================================

                             EXTERNAL FUNCTIONS

===========================================================================*/
/*===========================================================================
  FUNCTION QMI_CAT_EXTRACT_PACKED_TLV()

  DESCRIPTION
    Parses the TLV content of QMI_UIM message

  PARAMETERS
    sdu_in          : input sdu
    tlv_len         : length of the TLV
    tlv_present_ptr : boolean that indicates if the TLV is already present
    min_buffer_size : minimum size of the TLV
    max_buffer_size : maximum size of the TLV (corresponding to buffer size)
    buffer_ptr      : buffer where TLV is extracted

  RETURN VALUE
    QMI_ERR_NONE on success. Other error code on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
qmi_error_e_type qmi_cat_extract_packed_tlv
(
  dsm_item_type                     ** sdu_in,
  uint16                               tlv_len,
  boolean                            * tlv_present_ptr,
  uint16                               min_buffer_size,
  uint16                               max_buffer_size,
  void                               * buffer_ptr
);

/*===========================================================================
  FUNCTION QMI_CATI_ALPHABET_TLV

  DESCRIPTION
    to send the alphabet tlv

  PARAMETERS
    gstk_alphabet : Decoded alphabet
    ind      : tlv
    prm_type : Type of text

  RETURN VALUE
    qmi_error_e_type

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
qmi_error_e_type qmi_cati_alphabet_tlv
(
  const gstk_alphabet_type  * gstk_alphabet,
  dsm_item_type            ** ind,
  uint8                       prm_type
);

/*===========================================================================
  FUNCTION QMI_CATI_SLOT_ID_TLV

  DESCRIPTION
    Adds the TLV for the slot id

  PARAMETERS
    slot_id : slot id
    ind     :  tlv

  RETURN VALUE
    qmi_error_e_type

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
qmi_error_e_type qmi_cati_slot_id_tlv
(
  gstk_slot_id_enum_type         slot_id,
  dsm_item_type               ** ind,
  uint8                          prm_type
);

#endif /* QMI_CAT_PARSING_H */
