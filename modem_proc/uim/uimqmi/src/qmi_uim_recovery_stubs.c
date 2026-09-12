/*===========================================================================

                         Q M I _ U I M _ R E C O V E R Y.S T U B S C

DESCRIPTION

 The Data Services Qualcomm MSM Interface Device Management Services source
 file.

Copyright (c) 2016 QUALCOMM Technologies, Inc(QTI).
All Rights Reserved.
QUALCOMM Technologies Confidential and Proprietary
===========================================================================*/
/*===========================================================================

                      EDIT HISTORY FOR FILE

  $Header: //components/rel/uim.mpss/6.1.0/uimqmi/src/qmi_uim_recovery_stubs.c#1 $

when        who    what, where, why
--------    ---    ----------------------------------------------------------
07/08/16    ar     Move qmi_uimi_recovery and unify fdn/acl get_status func.
05/26/16    vdc    Move internal enums, structs to new internal header file
04/13/16    vdc    Initial Revision.
===========================================================================*/

/*===========================================================================

                          INCLUDE FILES FOR MODULE

===========================================================================*/
#include "uim_variation.h"
#include "uim_p.h"
#include "qmi_uim.h"
#include "mmgsdilib_common.h"
#include "qmi_uim_recovery.h"

/*===========================================================================

                             EXTERNAL FUNCTIONS

===========================================================================*/


/*===========================================================================
  FUNCTION QMI_UIMI_RECOVERY_CALLBACK()

  DESCRIPTION
    This function is executed as callback of UIM Recovery API. This
    is executed in the context of UIM task and needs to make a deep
    copy of the results and post a command to the QMI task to be
    processed.

  PARAMETERS

  RETURN VALUE
    None.

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
void qmi_uimi_recovery_callback
(
  uim_slot_type                     slot,
  const void                       *user_data,
  uimdrv_recovery_stage_enum_type   recovery_stage
)
{
  (void)slot;
  (void)user_data;
  (void)recovery_stage;
} /* qmi_uimi_recovery_callback */


/*===========================================================================
  FUNCTION  QMI_UIM_RECOVERY_PROCESS_CALLBACK()

  DESCRIPTION
    Generic QMI processing for external commands

  PARAMETERS
    uim_message_ptr : message to the internal qmi_uim_message
                      containing an external command

  RETURN VALUE
    None

  DEPENDENCIES
    QMI UIM must already have been initialized and registered with Framework

  SIDE EFFECTS
    None
===========================================================================*/
void qmi_uim_recovery_process_callback
(
  qmi_uim_message_type * uim_message_ptr
)
{
  (void)uim_message_ptr;
} /* qmi_uim_recovery_process_callback */


/*===========================================================================
  FUNCTION QMI_UIM_RECOVERY_PROCESS_COMPLETE_EVT()

  DESCRIPTION
    This function processes the recovery complete indication from UIM drivers.
    It indicates to all registered QMI clients and to QMI CAT that recovery
    has completed successfully.

  PARAMETERS
    mmgsdi_slot:  Slot in which the recovery was performed

  RETURN VALUE
    None

  DEPENDENCIES
    QMI UIM must already have been initialized and registered with Framework

  SIDE EFFECTS
    None
===========================================================================*/
void qmi_uim_recovery_process_complete_evt
(
  mmgsdi_slot_id_enum_type        mmgsdi_slot
)
{
  (void)mmgsdi_slot;
} /* qmi_uim_recovery_process_complete_evt */


/*===========================================================================
  FUNCTION QMI_UIMI_RECOVERY()

  DESCRIPTION
    Stub function

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
dsm_item_type * qmi_uimi_recovery
(
  void *            sp,
  void *            cmd_buf_p,
  void *            cl_sp,
  dsm_item_type **  sdu_in
)
{
  dsm_item_type      * response = NULL;
  boolean              retval   = FALSE;

  (void)sp;
  (void)cmd_buf_p;
  (void)cl_sp;
  (void)sdu_in;

  retval = qmi_svc_put_result_tlv(
             &response,
             QMI_RESULT_FAILURE,
             QMI_ERR_NOT_SUPPORTED);

  CHECK_RETVAL();

  return response;
} /* qmi_uim_recovery_process */

