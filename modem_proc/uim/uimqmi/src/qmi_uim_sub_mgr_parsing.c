/*================================================================================

                         Q M I _ U I M _ S U B _ M G R _ P A R S I N G. C


DESCRIPTION
  This file contains functions for QMI UIM service's Subscription Manager TLV parsing functionality.


Copyright (c) 2016-2020 QUALCOMM Technologies, Inc(QTI).
All Rights Reserved.
QUALCOMM Technologies Confidential and Proprietary

$Header: //components/rel/uim.mpss/6.1.0/uimqmi/src/qmi_uim_sub_mgr_parsing.c#2 $

when       who     what, where, why
--------   ---     -----------------------------------------------------------
02/20/20    rkg    Add new TLV in Suscription manager
09/17/19    mm     Add EID to SimSlotStatus radio config HAL
06/12/19    ykr    Add extended card status to SimSlotStatus
04/08/19    ng     Updating slot status check via ATR
09/05/18    ng     Add new TLV in Suscription manager
04/11/17    ks     Updating the card hotswap status based on ICCID
02/24/17    ks     Supporitng logical slot none in slot status indication
11/04/16    me     Combined multi-line ASSERT in one line
06/16/16    vdc    Initial Revision
=============================================================================*/

#include "uim_variation.h"
#include "qmi_framework.h"
#include "qmi_uim_parsing.h"
#include "qmi_uim_cat_common.h"
#include "qmi_uim_sub_mgr_parsing.h"
#include "qmi_svc_utils.h"
#include "user_identity_module_v01.h"
#include "amssassert.h"
#include <stringl/stringl.h>

/*===========================================================================
  FUNCTION QMI_UIM_TLV_PHYSICAL_SLOT()

  DESCRIPTION
    Parses the TLV item of physical slot

  PARAMETERS
    sdu_in   : input sdu
    len      : length of the TLV item
    slot_ptr : output with physical slot value

  RETURN VALUE
    QMI_ERR_NONE on success. Other error code on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
qmi_error_e_type qmi_uim_tlv_physical_slot
(
  dsm_item_type **                 sdu_in,
  uint16                           len,
  qmi_uim_tlv_physical_slot_type * slot_ptr
)
{
  int32             temp;

  PACKED struct PACKED_POST
  {
    uint32 physical_slot;
  } tlv_data;

  ASSERT(sdu_in && slot_ptr);

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

  /* Physical Slot */
  switch(tlv_data.physical_slot)
  {
    case UIM_PHY_SLOT_1_V01:
      slot_ptr->slot = UIM_SLOT_1;
      break;
    case UIM_PHY_SLOT_2_V01:
      slot_ptr->slot = UIM_SLOT_2;
      break;
    case UIM_PHY_SLOT_3_V01:
      slot_ptr->slot = UIM_SLOT_3;
      break;
    case UIM_PHY_SLOT_4_V01:
    case UIM_PHY_SLOT_5_V01:
      return QMI_ERR_NOT_SUPPORTED;
    default:
      return QMI_ERR_INVALID_ARG;
  }

  return QMI_ERR_NONE;
} /* qmi_uim_tlv_physical_slot */


/*===========================================================================
  FUNCTION QMI_UIM_SUB_MGR_RESPONSE_PHYSICAL_SLOTS_STATUS()

  DESCRIPTION
    Composes the TLV content for slots status indication

  PARAMETERS
    tlv_id             : id to be used for the TLV
    response_pptr      : output sdu
    slot_status_ptr    : Status of physical slots
    num_slots          : number of physical slots

  RETURN VALUE
    TRUE on success. FALSE on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
boolean qmi_uim_sub_mgr_response_physical_slots_status
(
  byte                               tlv_id,
  dsm_item_type                   ** response_pptr,
  const uim_phy_slot_status_type   * slot_status_ptr,
  uint8                              num_slots
)
{
  uint8   i                           = 0;
  uint16  tlv_len                     = 0;
  uint8*  tlv_data_ptr                = NULL;
  uint8*  curr_ptr                    = NULL;
  boolean ret_value                   = FALSE;

  PACKED struct PACKED_POST
  {
    uint32 card_hotswap_status;
    uint32 card_state;
    uint8  logical_slot;
    uint8  iccid_len;
  } tlv_top_data;

  ASSERT(response_pptr && slot_status_ptr);

  /* calculate memory requirement */
  tlv_len = sizeof(num_slots);

  for (i = 0; i < num_slots && i < UIM_MAX_NUMBER_INSTANCES; i++)
  {
    /* Fail if iccid length is greater than max iccid size, that is, 10 */
    if(slot_status_ptr[i].iccid_len > QMI_UIM_ICCID_LEN_MAX_V01)
    {
      return FALSE;
    }
    tlv_len += sizeof(tlv_top_data) + (uint16) slot_status_ptr[i].iccid_len;
  }

  /* Memory requirement calculation is done. Lets proceed with allocating memory */
  tlv_data_ptr = uimqmi_malloc(tlv_len);
  if (tlv_data_ptr == NULL)
  {
    return FALSE;
  }

  curr_ptr = tlv_data_ptr;

  /* Copy number of physical slots in the TLV */
  ASSERT(curr_ptr + sizeof(num_slots) <= tlv_data_ptr + tlv_len);
  (void)memscpy(curr_ptr,
                tlv_len - (curr_ptr - tlv_data_ptr),
                (void*)&num_slots,
                sizeof(num_slots));
  curr_ptr += sizeof(num_slots);

  /* Copy physical slots status in the TLV */
  for (i = 0; i < num_slots && i < UIM_MAX_NUMBER_INSTANCES; i++)
  {
    memset(&tlv_top_data, 0, sizeof(tlv_top_data));

    if(slot_status_ptr[i].atr_len > 0)
    {
      tlv_top_data.card_hotswap_status = UIM_PHYSICAL_CARD_STATE_PRESENT_V01;
    }
    else
    {
      tlv_top_data.card_hotswap_status = UIM_PHYSICAL_CARD_STATE_ABSENT_V01;
    }

    switch (slot_status_ptr[i].activity_status)
    {
      case UIM_SLOT_STATE_INACTIVE:
        tlv_top_data.card_state = UIM_PHYSICAL_SLOT_STATE_INACTIVE_V01;
        break;
      case UIM_SLOT_STATE_ACTIVE:
        tlv_top_data.card_state = UIM_PHYSICAL_SLOT_STATE_ACTIVE_V01;
        break;
      default:
        tlv_top_data.card_state = UIM_PHYSICAL_SLOT_STATE_ACTIVE_V01;
        break;
    }

    /* Logical slot is valid only if the slot status is ACTIVE */
    if( slot_status_ptr[i].activity_status == UIM_SLOT_STATE_ACTIVE)
    {
      switch (slot_status_ptr[i].logical_slot)
      {
        case UIM_SLOT_1:
          tlv_top_data.logical_slot = UIM_SLOT_1_V01;
          break;
        case UIM_SLOT_2:
          tlv_top_data.logical_slot = UIM_SLOT_2_V01;
          break;
        case UIM_SLOT_3:
          tlv_top_data.logical_slot = UIM_SLOT_3_V01;
          break;
        case UIM_SLOT_NONE:
        default:
          ret_value = FALSE;
          goto return_label;
      }
    }
    else
    {
      /* Set the logical slot to UIM_SLOT_1, anyways this should be ignored 
         by client as the slot is inactive */
      tlv_top_data.logical_slot = UIM_SLOT_1_V01;
    }

    tlv_top_data.iccid_len = slot_status_ptr[i].iccid_len;

    ASSERT(curr_ptr + sizeof(tlv_top_data) <= tlv_data_ptr + tlv_len);
    (void)memscpy(curr_ptr,
                  tlv_len - (curr_ptr - tlv_data_ptr),
                  (void*)&tlv_top_data,
                  sizeof(tlv_top_data));
    curr_ptr += sizeof(tlv_top_data);

    if(tlv_top_data.iccid_len > 0)
    {
      /* Copy ICCID (if valid) of the physical slot */
      ASSERT(curr_ptr + tlv_top_data.iccid_len <= tlv_data_ptr + tlv_len);
      (void)memscpy(curr_ptr,
                    tlv_len - (curr_ptr - tlv_data_ptr),
                    (void*)slot_status_ptr[i].iccid,
                    tlv_top_data.iccid_len);
      curr_ptr += tlv_top_data.iccid_len;
    }
  }

  ret_value = qmi_svc_put_param_tlv(response_pptr,
                                    tlv_id,
                                    tlv_len,
                                    (void *)tlv_data_ptr);

return_label:
  if(tlv_data_ptr != NULL)
  {
    uimqmi_free(tlv_data_ptr);
    tlv_data_ptr = NULL;
  }

  return ret_value;
} /* qmi_uim_sub_mgr_response_physical_slots_status */


/*===========================================================================
  FUNCTION QMI_UIM_SUB_MGR_RESPONSE_EXTENDED_CARD_STATE()

  DESCRIPTION
    Composes the TLV content for extended card state

  PARAMETERS
    tlv_id             : id to be used for the TLV
    response_pptr      : output sdu
    slot_status_ptr    : Status of physical slots
    num_slots          : number of physical slots

  RETURN VALUE
    TRUE on success. FALSE on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
boolean qmi_uim_sub_mgr_response_extended_card_state
(
  byte                               tlv_id,
  dsm_item_type                   ** response_pptr,
  const uim_phy_slot_status_type   * slot_status_ptr,
  uint8                              num_slots
)
{
  uint8   i                           = 0;
  uint16  tlv_len                     = 0;
  uint8*  tlv_data_ptr                = NULL;
  uint8*  curr_ptr                    = NULL;
  boolean ret_value                   = FALSE;

  /*------------------------------------------
    Structure used for TLV composition of card status
  --------------------------------------------*/
  PACKED struct PACKED_POST
  {
    uint8  card_state;
    uint8  error_code;
  } tlv_data;

  ASSERT(response_pptr && slot_status_ptr);

  /* calculate memory requirement */
  tlv_len = sizeof(num_slots);

  /* calculate memory requirement */
  tlv_len += (sizeof(tlv_data) * num_slots);

  /* Memory requirement calculation is done. Lets proceed with allocating memory */
  tlv_data_ptr = uimqmi_malloc(tlv_len);
  if (tlv_data_ptr == NULL)
  {
    return FALSE;
  }

  curr_ptr = tlv_data_ptr;

  /* Copy number of physical slots in the TLV */
  ASSERT(curr_ptr + sizeof(num_slots) <= tlv_data_ptr + tlv_len);

  *curr_ptr = num_slots;
  curr_ptr += sizeof(num_slots);

  /* Copy physical slots status in the TLV */
  for (i = 0; i < num_slots && i < UIM_MAX_NUMBER_INSTANCES; i++)
  {
    memset(&tlv_data, 0, sizeof(tlv_data));

    if(slot_status_ptr[i].atr_len > 0)
    {
      tlv_data.card_state = UIM_EXTENDED_CARD_STATE_PRESENT_V01;
      tlv_data.error_code = UIM_CARD_ERROR_CODE_UNKNOWN_V01;
    }
    else
    {
      tlv_data.card_state = UIM_EXTENDED_CARD_STATE_ERROR_V01;

      /* Update error indication for the card */
      switch(slot_status_ptr[i].uim_status)
      {
        case UIM_POWER_DOWN_CMD_NOTIFICATION_S:
          tlv_data.error_code = UIM_CARD_ERROR_CODE_POWER_DOWN_V01;
          break;

        case UIM_POLL_ERR_S:
          tlv_data.error_code = UIM_CARD_ERROR_CODE_POLL_ERROR_V01;
          break;

        case UIM_NO_ATR_RECEIVED_WITH_MAX_VOLTAGE_S:
        case UIM_NO_ATR_RECEIVED_AFTER_INT_RESET_S:
        case UIM_CORRUPT_ATR_RCVD_MAX_TIMES_S:
          tlv_data.error_code = UIM_CARD_ERROR_CODE_NO_ATR_RECEIVED_V01;
          break;

        case UIM_VOLTAGE_MISMATCH_S:
          tlv_data.error_code = UIM_CARD_ERROR_CODE_VOLT_MISMATCH_V01;
          break;

        case UIM_MAXED_PARITY_ERROR_S:
        case UIM_MAXED_RX_BREAK_ERROR_S:
        case UIM_MAXED_OVERRUN_ERROR_S:
        case UIM_MAX_PARITY_IN_PASSIVE_S:
        case UIM_MAX_RXBRK_IN_PASSIVE_S:
        case UIM_MAX_OVERRUN_IN_PASSIVE_S:
          tlv_data.error_code = UIM_CARD_ERROR_CODE_PARITY_ERROR_V01;
          break;

        case UIM_TECHNICAL_PROBLEM_S:
          tlv_data.error_code = UIM_CARD_ERROR_CODE_SIM_TECHNICAL_PROBLEMS_V01;
          break;

        case UIM_NULL_BYTE_ERR_S:
          tlv_data.error_code = UIM_CARD_ERROR_CODE_NULL_BYTES_V01;
          break;

        case UIM_CMD_TIMEOUT_ERR_S:
          tlv_data.error_code = UIM_CARD_ERROR_CODE_CMD_TIMEOUT_V01;
          break;

        case UIM_CARD_REMOVED_S:
          tlv_data.card_state = UIM_EXTENDED_CARD_STATE_ABSENT_V01;
          tlv_data.error_code = UIM_CARD_ERROR_CODE_POSSIBLY_REMOVED_V01;
          break;

        case UIM_ERR_S:
        default:
          tlv_data.error_code = UIM_CARD_ERROR_CODE_UNKNOWN_V01;
          break;
      }
    }

    ASSERT(curr_ptr + sizeof(tlv_data) <= tlv_data_ptr + tlv_len);
    (void)memscpy(curr_ptr,
                  tlv_len - (curr_ptr - tlv_data_ptr),
                  (void*)&tlv_data,
                   sizeof(tlv_data));
    curr_ptr +=  sizeof(tlv_data);
  }

  ret_value = qmi_svc_put_param_tlv(response_pptr,
                                    tlv_id,
                                    tlv_len,
                                    (void *)tlv_data_ptr);

  uimqmi_free(tlv_data_ptr);
  tlv_data_ptr = NULL;

  return ret_value;
} /* qmi_uim_sub_mgr_response_extended_card_state */


/*===========================================================================
  FUNCTION QMI_UIM_SUB_MGR_RESPONSE_EID_INFORMATION()

  DESCRIPTION
    Composes the TLV content for slots information indication

  PARAMETERS
    tlv_id             : id to be used for the TLV
    response_pptr      : output sdu
    slot_status_ptr    : Status of physical slots
    num_slots          : number of physical slots

  RETURN VALUE
    TRUE on success. FALSE on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
boolean qmi_uim_sub_mgr_response_eid_information
(
  byte                               tlv_id,
  dsm_item_type                   ** response_pptr,
  const uim_phy_slot_status_type   * slot_status_ptr,
  uint8                              num_slots
)
{
  uint8   i                           = 0;
  uint16  tlv_len                     = 0;
  uint8*  tlv_data_ptr                = NULL;
  uint8*  curr_ptr                    = NULL;
  boolean ret_value                   = FALSE;

  PACKED struct PACKED_POST
  {
    uint8  eid_len;
  } tlv_data;

  ASSERT(response_pptr && slot_status_ptr);

  /* calculate memory requirement */
  tlv_len = sizeof(num_slots);

  for (i = 0; i < num_slots && i < UIM_MAX_NUMBER_INSTANCES; i++)
  {
    /* Fail if eid length is greater than max eid size */
    if(slot_status_ptr[i].eid_len > QMI_UIM_EID_LEN_V01)
    {
      return FALSE;
    }
    tlv_len += sizeof(tlv_data) + slot_status_ptr[i].eid_len;
  }

  /* Memory requirement calculation is done. Lets proceed with allocating memory */
  tlv_data_ptr = uimqmi_malloc(tlv_len);
  if (tlv_data_ptr == NULL)
  {
    return FALSE;
  }

  curr_ptr = tlv_data_ptr;
  
  /* Copy number of physical slots in the TLV */
  ASSERT(curr_ptr + sizeof(num_slots) <= tlv_data_ptr + tlv_len);
  *curr_ptr = num_slots;
  curr_ptr += sizeof(num_slots);

  /* Copy physical slots status in the TLV */
  for (i = 0; i < num_slots && i < UIM_MAX_NUMBER_INSTANCES; i++)
  {
    memset(&tlv_data, 0, sizeof(tlv_data));

    tlv_data.eid_len = slot_status_ptr[i].eid_len;

    ASSERT(curr_ptr + sizeof(tlv_data.eid_len) <= tlv_data_ptr + tlv_len);
    (void)memscpy(curr_ptr,
                  tlv_len - (curr_ptr - tlv_data_ptr),
                  (void*)&tlv_data,
                   sizeof(tlv_data.eid_len));
    curr_ptr +=  sizeof(tlv_data.eid_len);

    if(tlv_data.eid_len > 0)
    {
      ASSERT(curr_ptr + tlv_data.eid_len <= tlv_data_ptr + tlv_len);
      (void)memscpy(curr_ptr,
                    tlv_len - (curr_ptr - tlv_data_ptr),
                    (void*)slot_status_ptr[i].eid,
                    tlv_data.eid_len);
      curr_ptr += tlv_data.eid_len;
    }
  }

  ret_value = qmi_svc_put_param_tlv(response_pptr,
                                    tlv_id,
                                    tlv_len,
                                    (void *)tlv_data_ptr);

  uimqmi_free(tlv_data_ptr);
  tlv_data_ptr = NULL;

  return ret_value;
} /* qmi_uim_sub_mgr_response_eid_information */

/*===========================================================================
  FUNCTION QMI_UIM_SUB_MGR_RESPONSE_PHYSICAL_SLOTS_INFORMATION()

  DESCRIPTION
    Composes the TLV content for slots information indication

  PARAMETERS
    tlv_id             : id to be used for the TLV
    response_pptr      : output sdu
    slot_status_ptr    : Status of physical slots
    num_slots          : number of physical slots

  RETURN VALUE
    TRUE on success. FALSE on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
boolean qmi_uim_sub_mgr_response_physical_slots_information
(
  byte                               tlv_id,
  dsm_item_type                   ** response_pptr,
  const uim_phy_slot_status_type   * slot_status_ptr,
  uint8                              num_slots
)
{
  uint8   i                           = 0;
  uint16  tlv_len                     = 0;
  uint8*  tlv_data_ptr                = NULL;
  uint8*  curr_ptr                    = NULL;
  boolean ret_value                   = FALSE;

  PACKED struct PACKED_POST
  {
    uint32 card_protocol;
    uint8  num_app;
    uint8  atr_len;
  } tlv_data;

  ASSERT(response_pptr && slot_status_ptr);

  /* calculate memory requirement */
  tlv_len = sizeof(num_slots);

  for (i = 0; i < num_slots && i < UIM_MAX_NUMBER_INSTANCES; i++)
  {
    /* Fail if iccid length is greater than max iccid size, that is, 10 */
    if(slot_status_ptr[i].atr_len > QMI_UIM_ATR_MAX_V01)
    {
      return FALSE;
    }
    tlv_len += sizeof(tlv_data) + slot_status_ptr[i].atr_len + sizeof(uint8);
  }

  /* Memory requirement calculation is done. Lets proceed with allocating memory */
  tlv_data_ptr = uimqmi_malloc(tlv_len);
  if (tlv_data_ptr == NULL)
  {
    return FALSE;
  }

  curr_ptr = tlv_data_ptr;

  /* Copy number of physical slots in the TLV */
  ASSERT(curr_ptr + sizeof(num_slots) <= tlv_data_ptr + tlv_len);
  *curr_ptr = num_slots;
  curr_ptr += sizeof(num_slots);

  /* Copy physical slots status in the TLV */
  for (i = 0; i < num_slots && i < UIM_MAX_NUMBER_INSTANCES; i++)
  {
    memset(&tlv_data, 0, sizeof(tlv_data));

    switch (slot_status_ptr[i].uim_protocol)
    {
      case UIM_ICC:
        tlv_data.card_protocol = UIM_CARD_PROTOCOL_ICC_V01;
        break;
      case UIM_UICC:
        tlv_data.card_protocol = UIM_CARD_PROTOCOL_UICC_V01;
        break;
      default:
        tlv_data.card_protocol = UIM_CARD_PROTOCOL_UNKNOWN_V01;
        break;
    }

    tlv_data.num_app = slot_status_ptr[i].num_apps;

    tlv_data.atr_len = slot_status_ptr[i].atr_len;

    ASSERT(curr_ptr + sizeof(tlv_data.card_protocol) + sizeof(tlv_data.num_app) + sizeof(tlv_data.atr_len) <= tlv_data_ptr + tlv_len);
    (void)memscpy(curr_ptr,
                  tlv_len - (curr_ptr - tlv_data_ptr),
                  (void*)&tlv_data,
                   sizeof(tlv_data.card_protocol) + sizeof(tlv_data.num_app) + sizeof(tlv_data.atr_len));
    curr_ptr +=  (sizeof(tlv_data.card_protocol) + sizeof(tlv_data.num_app) + sizeof(tlv_data.atr_len));

    if(tlv_data.atr_len > 0)
    {
      ASSERT(curr_ptr + tlv_data.atr_len <= tlv_data_ptr + tlv_len);
      (void)memscpy(curr_ptr,
                    tlv_len - (curr_ptr - tlv_data_ptr),
                    (void*)slot_status_ptr[i].atr,
                    tlv_data.atr_len);
      curr_ptr += tlv_data.atr_len;
    }

    ASSERT(curr_ptr + sizeof(uint8) <= tlv_data_ptr + tlv_len);
    *curr_ptr = (slot_status_ptr[i].is_euicc ? 1 : 0);
    curr_ptr +=  sizeof(uint8);
  }

  ret_value = qmi_svc_put_param_tlv(response_pptr,
                                    tlv_id,
                                    tlv_len,
                                    (void *)tlv_data_ptr);

  uimqmi_free(tlv_data_ptr);
  tlv_data_ptr = NULL;

  return ret_value;
} /* qmi_uim_sub_mgr_response_physical_slots_information */
