#ifndef QMI_UIM_SUB_MGR_H
#define QMI_UIM_SUB_MGR_H
/*===========================================================================

                         Q M I _ U I M _ S U B _ M G R. H

DESCRIPTION

 The Data Services Qualcomm MSM Interface Device Management Services source
 file.

Copyright (c) 2016, 2019 QUALCOMM Technologies, Inc(QTI).
All Rights Reserved.
QUALCOMM Technologies Confidential and Proprietary
===========================================================================*/
/*===========================================================================

                      EDIT HISTORY FOR FILE

  $Header: //components/rel/uim.mpss/6.1.0/uimqmi/src/qmi_uim_sub_mgr.h#1 $

when        who    what, where, why
--------    ---    ----------------------------------------------------------
06/12/19    ykr    Add extended card status to SimSlotStatus
07/26/16    sp     Move qmi_uimi_ functions to corresponding files
06/16/16    vdc    Split QMI UIM parsing files for supporting IOE
05/26/16    vdc    Move internal enums, structs to new internal header file
04/13/16    vdc    Initial Revision.
===========================================================================*/

/*===========================================================================

                          INCLUDE FILES FOR MODULE

===========================================================================*/
#include "uim_variation.h"
#include "qmi_uim_internal.h"
#include "uim_p.h"

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
);


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
);

/*===========================================================================
  FUNCTION QMI_UIM_SUB_PROCESS_PHYSICAL_SLOTS_STATUS_MSG()

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
);
#endif /* FEATURE_UIM_DS_SUBSCRIPTION_MANAGER */

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
);

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
);
#endif /* QMI_UIM_SUB_MGR_H */
