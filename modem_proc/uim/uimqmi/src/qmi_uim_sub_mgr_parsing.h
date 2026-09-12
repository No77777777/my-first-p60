/*================================================================================

                         Q M I _ U I M _ S U B _M G R _ P A R S I N G. H


DESCRIPTION
  This file contains functions for QMI UIM service's Subscription Manager TLV parsing functionality.

Copyright (c) 2016, 2019 - 2020 QUALCOMM Technologies, Inc(QTI).
All Rights Reserved.
QUALCOMM Technologies Confidential and Proprietary

$Header: //components/rel/uim.mpss/6.1.0/uimqmi/src/qmi_uim_sub_mgr_parsing.h#2 $

when       who     what, where, why
--------   ---     -----------------------------------------------------------
02/20/20    rkg    Add new TLV in Suscription manager
09/17/19    mm     Add EID to SimSlotStatus radio config HAL
06/12/19    ykr    Add extended card status to SimSlotStatus
06/16/16    vdc    Initial Revision
=============================================================================*/

#include "uim_variation.h"
#include "customer.h"
#include "uim_msg.h"
#include "qmi_uim_parsing.h"

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
);

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
);


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
);

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
);

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
);