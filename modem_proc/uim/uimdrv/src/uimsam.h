#ifndef UIMSAM_H
#define UIMSAM_H
/*===========================================================================

           I N T E R N A L   SAM    S T A T E   M A C H I N E

                      D E F I N I T I O N S

DESCRIPTION
  This contains all the declarations for the SAM state machine.

Copyright (c) 2016 - 2017 QUALCOMM Technologies, Inc (QTI) and its licensors. All Rights Reserved.

===========================================================================*/

/*===========================================================================

                      EDIT HISTORY FOR FILE

$Header: //components/rel/uim.mpss/6.1.0/uimdrv/src/uimsam.h#1 $
$DateTime: 2019/10/30 06:56:05 $
$Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
04/14/17   kv      Added pass-through mode support
03/28/16   sam     Created module.  This module contains the sam state
                   machine definitions.

===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "uim_variation.h"
#include "target.h"
#include "comdef.h"


/*===========================================================================

FUNCTION UIM_SAM_PROCESS_CARD_RESPONSE                

DESCRIPTION
  This procedure processes the response received from the SAM card and changes
  the UIM server control variables based upon the processing status.

DEPENDENCIES
  This function uses global variables defined for the UIM server to determine
  the proper processing of the card response.

RETURN VALUE
  None.

SIDE EFFECTS
  The control variables of the UIM server are adjusted to reflect the change
  in the operation of the UIM server as a result of the processing on the
  response.

===========================================================================*/
void uim_sam_process_card_response
(
  rex_sigs_type            *i_mask,    /* Mask of signals interested in */
  uim_instance_global_type *uim_ptr
);

/*===========================================================================

FUNCTION UIM_SAM_PROCESS_CMD_ERR                                       

DESCRIPTION
  This procedure processes the error response received from the SAM card

DEPENDENCIES
  This function uses global variables defined for the UIM server to determine
  the proper processing of the card response.

RETURN VALUE
  None.

SIDE EFFECTS
  The control variables of the UIM server are adjusted to reflect the change
  in the operation of the UIM server as a result of the processing on the
  response.

===========================================================================*/
void uim_sam_process_cmd_err
(
  rex_sigs_type            *i_mask,   /* Mask of signals interested in */
  uim_instance_global_type *uim_ptr
);

/*===========================================================================

FUNCTION UIM_SAM_PROCESS_COMMAND

DESCRIPTION
  This procedure processes a command sent to the UIM task. It sets the UIM
  state variable and calls the SAM card state machine to process the command.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS

===========================================================================*/
void uim_sam_process_command
(
  /* Pointer to received command */
  rex_sigs_type            *mask,
  /* rex signals type mask */
  uim_instance_global_type *uim_ptr
);

/*===========================================================================

FUNCTION uim_sam_process_response

DESCRIPTION
  This procedure processes the response that has been recd from the SAM card
  at the completion of the command processing and releases the command
  buffer for the current command.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  Releases the command buffer.

===========================================================================*/
void uim_sam_process_response
(
  uim_rsp_buf_type const   *rsp_ptr,
  uim_instance_global_type *uim_ptr
);

/*===========================================================================

FUNCTION UIM_SAM_COMMAND                                      

DESCRIPTION
  This procedure processes the error response received from the SAM card

DEPENDENCIES
  This procedure is the main state machine for command processing of a SAM card.

RETURN VALUE
  None.

SIDE EFFECTS
  Sends a command to the SAM card.

===========================================================================*/
void uim_sam_command
(
  uim_instance_global_type *uim_ptr
);


/*===========================================================================

FUNCTION UIM_SAM_COMMAND_RESPONSE

DESCRIPTION
  This procedure processes the response to a comamnd that has been
  recd from the SAM card.

DEPENDENCIES
  None

RETURN VALUE
  Status which is of cmd_status_type

SIDE EFFECTS
  Changes the UIM state variable uim_sam_generic_state.

===========================================================================*/
uim_cmd_status_type uim_sam_command_response
(
  uim_rsp_buf_type    *rsp_ptr,
  uim_instance_global_type *uim_ptr
);

/**
*  This function handles the UIM_CMD_Q_SIG signal for SAM card.
*
*
*  @param sig_mask_ptr Pointer to the OR's set of signals that UIM task needs to
*                      handle.  Unused in this function.
*  @param imask_ptr Pointer to the mask of signals that the uim task is interested
*                   in handling.  The value may be updated in this function.
*  @param uim_ptr Pointer to the current instance of the UIM global structure
*  @return boolean Return TRUE to indicate that immediately
*          going back to the top of the main loop is desired.
*/
boolean uim_sam_handle_cmd_q_sig
(
  rex_sigs_type            *sig_mask_ptr,
  rex_sigs_type            *imask_ptr,
  uim_instance_global_type *uim_ptr
);

/**
*  This function handles the UIM_CMD_RSP_SIG signal for SAM card.
*
*
*  @param sig_mask_ptr Pointer to the OR's set of signals that UIM task needs to
*                      handle.  Unused in this function.
*  @param imask_ptr Pointer to the mask of signals that the uim task is interested
*                   in handling.  The value may be updated in this function.
*  @param uim_ptr Pointer to the current instance of the UIM global structure
*  @return boolean Return TRUE to indicate that immediately
*          going back to the top of the main loop is desired.
*/
boolean uim_sam_handle_cmd_rsp_sig
(
  rex_sigs_type *sig_mask_ptr,
  rex_sigs_type *imask_ptr,
  uim_instance_global_type *uim_ptr
);

/**
*  This function handles the UIM_TRANSACTION_SIG/CMD_RSP_TIMEOUT signal for SAM card.
*
*  SIDE EFFECTS
*    This function may change the value of *imask_ptr or *sig_mask_ptr.
*
*
*  @param sig_mask_ptr  Pointer to the OR's set of signals that UIM task needs to
*                       handle.  The value may be updated by this function.
*  @param imask_ptr Pointer to the mask of signals that the uim task is interested
*                   in handling.  Unused in this function.
*  @param uim_ptr Pointer to the current instance of the UIM global structure
*  @return boolean Return TRUE to indicate that immediately
*          going back to the top of the main loop is desired.
*/
boolean uim_sam_handle_cmd_rsp_timeout_sig
(
  rex_sigs_type *sig_mask_ptr,
  rex_sigs_type *imask_ptr,
  uim_instance_global_type *uim_ptr
);


/**
*  This function checks if the command recieved is valid or not 
*  when the task is in SAM phase.
*
*
*  @param uim_ptr   Pointer to the current instance of the UIM global structure

*  @return boolean   TRUE: if command is valid in SAM phase
*                    FALSE: if command is invalid in SAM phase
*/
boolean uim_is_cmd_valid_in_sam_mode
(
  uim_instance_global_type *uim_ptr
);


/**
*  This function modifies the imask to only check for signals 
*  that are applicabel to SAM card
*
*
*  @param uim_ptr   Pointer to the current instance of the UIM global structure
*  @param imask_ptr Pointer to the mask of signals that the uim task is interested
*                   in handling.  The value will be updated in this function.

*/
void uim_sam_modify_imask
(
  uim_instance_global_type *uim_ptr,
  rex_sigs_type *imask_ptr
);


/**
*  This function resets the imask to the default signals 
*  since we are moving fro mSAM phase ot telcom phase
*
*
*  @param uim_ptr   Pointer to the current instance of the UIM global structure
*  @param imask_ptr Pointer to the mask of signals that the uim task is interested
*                   in handling.  The value will be updated in this function.

*/
void uim_telecom_modify_imask
(
  uim_instance_global_type *uim_ptr,
  rex_sigs_type *imask_ptr
);


/**
*  This procedure handles a succesful Stream APDU response for SAM card
*
*
*  @param uim_ptr   Pointer to the current instance of the UIM global structure
*  @param req_ptr   Pointer to the request buffer with the current APDu
*  @param rsp_ptr   Pointer to the response buffer with the APDU response

*  @return boolean   TRUE: Stram APDU success
*                    FALSE: Stram APDU fail
*/
boolean uim_sam_stream_apdu_success
(
  uim_instance_global_type *uim_ptr,
  uim_req_buf_type         *uim_req_buf_static_ptr,
  uim_rsp_buf_type         *rsp_ptr
);


/**
*  The function is used to set the card mdoe of the current task based on teh logical ID
*
*
*  @param uim_ptr   Pointer to the current instance of the UIM global structure
*  @param imask_ptr Pointer to the mask of signals that the uim task is interested
*                   in handling.  The value will be updated in this function.

*/
void uim_sam_set_card_mode
(
  rex_sigs_type            *imask_ptr,
  uim_instance_global_type *uim_ptr
);


/**
*  The function is used to reset card mdoe of the logical ID 1 to telecom
* if dual sim enabled then logical ID 2 is also set to telecom
*
*  @param uim_ptr   Pointer to the current instance of the UIM global structure
*  @param imask_ptr Pointer to the mask of signals that the uim task is interested
*                   in handling.  The value will be updated in this function.

*/
void uim_sam_reset_card_mode
(
  rex_sigs_type            *imask_ptr,
  uim_instance_global_type *uim_ptr
);


/**
*  The checks if SAM feature is enabeld or not
*  @return boolean 
           TRUE if Featre SAM card enabled
           FALSE if Featre SAM card disabled
*/
boolean uim_is_feature_sam_enabled();


/**
* Change the mode based on the command, NV setting and the current card mode
*  @param uim_ptr   Pointer to the current instance of the UIM global structure
*  @return void 
*/
void uim_sam_change_mode
(
  uim_instance_global_type *uim_ptr
);

#endif /* UIMSAM_H */
