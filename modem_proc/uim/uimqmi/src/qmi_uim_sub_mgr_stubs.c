/*===========================================================================

                         Q M I _ U I M _ S U B _ M G R. C

DESCRIPTION

 The Data Services Qualcomm MSM Interface Device Management Services source
 file.

Copyright (c) 2016, 2020 QUALCOMM Technologies, Inc(QTI).
All Rights Reserved.
QUALCOMM Technologies Confidential and Proprietary
===========================================================================*/
/*===========================================================================

                      EDIT HISTORY FOR FILE

  $Header: //components/rel/uim.mpss/6.1.0/uimqmi/src/qmi_uim_sub_mgr_stubs.c#2 $

when        who    what, where, why
--------    ---    ----------------------------------------------------------
02/20/20    rkg    Add new TLV in Suscription manager
07/26/16    sp     Move qmi_uimi_ functions to corresponding files
06/16/16    vdc    Split QMI UIM parsing files for supporting IOE
05/26/16    vdc    Move internal enums, structs to new internal header file
04/13/16    vdc    Initial Revision.
===========================================================================*/

/*===========================================================================

                          INCLUDE FILES FOR MODULE

===========================================================================*/
#include "uim_variation.h"
#include "uim_p.h"
#include "qmi_uim_sub_mgr.h"
#include "qmi_uim_sub_mgr_parsing.h"
#include "qmi_framework.h"
#include "qmi_svc_utils.h"

/*===========================================================================

                             EXTERNAL FUNCTIONS

===========================================================================*/

 #ifdef FEATURE_UIM_DS_SUBSCRIPTION_MANAGER
/*===========================================================================
FUNCTION QMI_UIM_SUB_MGR_IS_PHYSICAL_SLOT_ACTIVE_FOR_LOGICAL_SLOT()

DESCRIPTION
  Figures out if for the logical slot, the physical slot is active or not.

PARAMETERS
  uim_slot_type        : logical slot id
  uim_slot_type        : physical slot id

RETURN VALUE
  None

DEPENDENCIES
  QMI UIM service must be initialized and registered with Framework

SIDE EFFECTS
  None

===========================================================================*/
boolean qmi_uim_sub_mgr_is_physical_slot_active_for_logical_slot
(
  uim_slot_type                   logical_slot_id,
  uim_slot_type                   physical_slot_id
)
{
  (void)logical_slot_id;
  (void)physical_slot_id;

  return FALSE;
} /* qmi_uim_sub_mgr_is_physical_slot_active_for_logical_slot() */


/*===========================================================================
FUNCTION QMI_UIM_SUB_MGR_NOTIFY_SLOTS_STATUS_CB()

DESCRIPTION
  A callback called by uimdrv when an association of a logical slot to
  a physical slot changes or whenever there is a change to the card state
  in any physical slot.

PARAMETERS
  uint8                   : number of physical slots
  uim_phy_slot_status_type: status of the physical slots

RETURN VALUE
  None

DEPENDENCIES
  QMI UIM service must be initialized and registered with Framework

SIDE EFFECTS
  None

===========================================================================*/
void qmi_uim_sub_mgr_notify_slots_status_cb
(
  uint8                           num_physical_slots,
  const uim_phy_slot_status_type *physical_slot_status_ptr
)
{
  (void)num_physical_slots;
  (void)physical_slot_status_ptr;
} /* qmi_uim_sub_mgr_notify_slots_status_cb() */
#endif /* FEATURE_UIM_DS_SUBSCRIPTION_MANAGER */


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
  (void)tlv_id;
  (void)response_pptr;
  (void)slot_status_ptr;
  (void)num_slots;

  return FALSE;
} /* qmi_uim_sub_mgr_response_physical_slots_status */


/*===========================================================================
  FUNCTION QMI_UIM_SUB_MGR_RESPONSE_PHYSICAL_SLOTS_INFORMATION()

  DESCRIPTION
    Composes the TLV content for slots information indication

  PARAMETERS
    tlv_id             : Id to be used for the TLV
    response_pptr      : Output sdu
    slot_status_ptr    : Status of physical slots
    num_slots          : Number of physical slots

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
  (void)tlv_id;
  (void)response_pptr;
  (void)slot_status_ptr;
  (void)num_slots;

  return FALSE;
} /* qmi_uim_sub_mgr_response_physical_slots_information */


/*===========================================================================
  FUNCTION QMI_UIM_PROCESS_PHYSICAL_SLOTS_STATUS_MSG()

  DESCRIPTION
    This function handles message from the QMI UIM queue for physical slots
    status from drivers

  PARAMETERS
    uim_message_ptr: Pointer to QMI UIM message from message queue

  RETURN VALUE
    None.

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
void qmi_uim_sub_mgr_process_physical_slots_status_msg
(
  qmi_uim_message_type * uim_message_ptr
)
{
  (void)uim_message_ptr;
} /* qmi_uim_sub_mgr_process_physical_slots_status_msg */


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
  (void)sdu_in;
  (void)len;
  (void)slot_ptr;

  return QMI_ERR_INTERNAL;
}/* qmi_uim_tlv_physical_slot */


/*===========================================================================
  FUNCTION QMI_UIMI_SWITCH_SLOT()

  DESCRIPTION
    Switches the physical slot tied to current logical slot such that the
    current logical slot now maps to the physical slot mentioned in the request

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
dsm_item_type* qmi_uimi_switch_slot
(
  void *            sp,
  void *            cmd_buf_p,
  void *            cl_sp,
  dsm_item_type **  sdu_in
)
{
  boolean        retval   = TRUE;
  dsm_item_type *response = NULL;

  (void)sp;
  (void)cmd_buf_p;
  (void)cl_sp;
  (void)sdu_in;

  retval = qmi_svc_put_result_tlv(&response, QMI_RESULT_FAILURE, QMI_ERR_NOT_SUPPORTED);
  CHECK_RETVAL();

  return response;
}/* qmi_uimi_switch_slot */


/*===========================================================================
  FUNCTION qmi_uimi_get_slots_status()

  DESCRIPTION
    Get current physical slots status

  PARAMETERS
    sp        : service provided state pointer
    cmd_buf_p : ptr to cmd buffer
    cl_sp     : client state pointer
    sdu_in    : incoming request

  RETURN VALUE
    ptr to response or NULL if none.

  DEPENDENCIES
    QMI UIM service must be initialized and registered with Framework

  SIDE EFFECTS
    None
===========================================================================*/
dsm_item_type * qmi_uimi_get_slots_status
(
  void *            sp,
  void *            cmd_buf_p,
  void *            cl_sp,
  dsm_item_type **  sdu_in
)
{
  boolean        retval   = TRUE;
  dsm_item_type *response = NULL;
  
  (void)sp;
  (void)cmd_buf_p;
  (void)cl_sp;
  (void)sdu_in;
  
  retval = qmi_svc_put_result_tlv(&response, QMI_RESULT_FAILURE, QMI_ERR_NOT_SUPPORTED);
  CHECK_RETVAL();
  
  return response;
} /* qmi_uimi_get_slots_status() */

