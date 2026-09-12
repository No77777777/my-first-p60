/*===========================================================================

  Copyright (c) 2018 Qualcomm Technologies Incorporated. All Rights Reserved

  Qualcomm Proprietary

  Export of this technology or software is regulated by the U.S. Government.
  Diversion contrary to U.S. law prohibited.

  All ideas, data and information contained in or disclosed by
  this document are confidential and proprietary information of
  Qualcomm Technologies Incorporated and all rights therein are expressly reserved.
  By accepting this material the recipient agrees that this material
  and the information contained therein are held in confidence and in
  trust and will not be used, copied, reproduced in whole or in part,
  nor its contents revealed in any manner to others without the express
  written permission of Qualcomm Technologies Incorporated.

===========================================================================*/

#ifndef EMM_SSGCCS_H
#define EMM_SSGCCS_H

#include "lte_nas_emm_message_type.h"

/*========================================================================
  FUNCTION emm_ssgccs_handle_incoming_msg

  DESCRIPTION
    Process EMM incoming message

  PARAMETERS
    incoming_msg [in] - EMM incoming message

  RETURN VALUE
    None
===========================================================================*/
void emm_ssgccs_handle_incoming_msg
(
  lte_nas_emm_incoming_msg_type* incoming_msg
);

/*========================================================================
  FUNCTION emm_ssgccs_handle_outgoing_msg

  DESCRIPTION
    Process EMM outgoing message

  PARAMETERS
    outgoing_msg [in] - EMM outgoing message

  RETURN VALUE
    None
===========================================================================*/
void emm_ssgccs_handle_outgoing_msg
(
  lte_nas_emm_outgoing_msg_type* outgoing_msg
);

/*========================================================================
  FUNCTION emm_ssgccs_send_tau_failed_ind

  DESCRIPTION
    Process EMM outgoing message

  PARAMETERS
    outgoing_msg [in] - EMM outgoing message

  RETURN VALUE
    None
===========================================================================*/
void emm_ssgccs_send_tau_failed_ind(lte_emm_connection_cause_type                cause);

/*===========================================================================

FUNCTION  emm_ssgccs_send_attach_failed_ind

DESCRIPTION
  This function handles the case of connection is released before ATTACH complete

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void emm_ssgccs_send_attach_failed_ind(lte_emm_connection_cause_type                cause);

/*========================================================================
  FUNCTION emm_ssgccs_send_sec_ctxt_del_ind

  DESCRIPTION
    update the csfb call status

  PARAMETERS
    call_status [in] - uint8

  RETURN VALUE
    None
===========================================================================*/
void emm_ssgccs_send_sec_ctxt_del_ind(emm_nas_ssg_context_type   ctxt_est);
/*===========================================================================

FUNCTION  emm_send_ssg_sec_ctxt_set_ind

DESCRIPTION
  This function handles the case of connection is released before ATTACH complete

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void emm_send_ssg_sec_ctxt_set_ind(emm_nas_ssg_context_type   ctxt_est);

/*========================================================================
  FUNCTION emm_update_ssgccs_csfb_call_status

  DESCRIPTION
    update the csfb call status

  PARAMETERS
    call_status [in] - uint8

  RETURN VALUE
    None
===========================================================================*/
void emm_update_ssgccs_csfb_call_status
(
  uint8 call_status
);

/*===========================================================================

FUNCTION  emm_ssgccs_cell_change_ind

DESCRIPTION
  This function sends an indication if there is a cell change

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void emm_ssgccs_cell_change_ind
(
lte_rrc_global_cell_id_t cell_identity,
sys_plmn_id_s_type	 plmn
);

#endif /* EMM_SSGCCS_H */