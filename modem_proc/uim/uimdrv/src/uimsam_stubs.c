/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*
FILE:		uimsam_stubs.c


GENERAL DESCRIPTION
  This is a stubs file for uimsam.c


Copyright (c) 2016 - 2017 QUALCOMM Technologies, Inc (QTI) and its licensors. All Rights Reserved.

*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/


/*===========================================================================

                        EDIT HISTORY FOR MODULE

$Header: //components/rel/uim.mpss/6.1.0/uimdrv/src/uimsam_stubs.c#1 $
$DateTime: 2019/10/30 06:56:05 $
$Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
04/14/17   kv      Added pass-through mode support
06/23/16   sam     F3 messages optimization
03/28/16   sam     Created module.  This module contains the SAM state
                   machine.

===========================================================================*/


/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "uim_variation.h"
#include "intconv.h"
#include "customer.h"
#include "comdef.h"
#include "uimdrv_msg.h"
#include "err.h"
#include "uimdrv_main.h"
#include "uimdrv.h"
#include "uimi.h"
#include "uimisim.h"
#include "uimutil.h"
#include <memory.h>
#include "uimglobals.h"
#include "mmgsdilib_p.h"
#include "uimsam.h"


/*============================================================================= 
  STUBS FUNCTION uim_sam_stream_apdu_success
=============================================================================*/
boolean uim_sam_stream_apdu_success
(
  uim_instance_global_type *uim_ptr,
  uim_req_buf_type         *uim_req_buf_static_ptr,
  uim_rsp_buf_type         *rsp_ptr
)
{
  (void)rsp_ptr;
  (void)uim_req_buf_static_ptr;
  (void)uim_ptr;
  return FALSE;
}/*uim_sam_stream_apdu_success*/


/*============================================================================= 
  STUBS FUNCTION uim_sam_process_card_response
=============================================================================*/
void uim_sam_process_card_response
(
  rex_sigs_type            *i_mask,
  uim_instance_global_type *uim_ptr
)
{
  (void)i_mask;
  (void)uim_ptr;
  return;
}/*uim_sam_process_card_response*/


/*============================================================================= 
  STUBS FUNCTION uim_sam_process_command
=============================================================================*/
void uim_sam_process_command
(
  rex_sigs_type            *mask,
  uim_instance_global_type *uim_ptr
)
{
  (void)mask;
  (void)uim_ptr;
  return;
}/*uim_sam_process_command*/


/*============================================================================= 
  STUBS FUNCTION uim_sam_process_cmd_err
=============================================================================*/
void uim_sam_process_cmd_err
(
  rex_sigs_type            *i_mask,
  uim_instance_global_type *uim_ptr
)
{
  (void)i_mask;
  (void)uim_ptr;
  return;
}/*uim_sam_process_cmd_err*/


/*============================================================================= 
  STUBS FUNCTION uim_sam_process_response
=============================================================================*/
void uim_sam_process_response
(
  uim_rsp_buf_type const   *rsp_ptr,
  uim_instance_global_type *uim_ptr
)
{
  (void)rsp_ptr;
  (void)uim_ptr;
  return;
}/*uim_sam_process_response*/


/*============================================================================= 
  STUBS FUNCTION uim_sam_command
=============================================================================*/
void uim_sam_command
(
  uim_instance_global_type *uim_ptr
)
{
  (void)uim_ptr;
  return;
}/*uim_sam_command*/


/*============================================================================= 
  STUBS FUNCTION uim_sam_command_response
=============================================================================*/
uim_cmd_status_type uim_sam_command_response
(
  uim_rsp_buf_type    *rsp_ptr,
  uim_instance_global_type *uim_ptr
)
{
  (void)rsp_ptr;
  (void)uim_ptr;
  return UIM_CMD_ERR;
}/*uim_sam_command_response*/


/*============================================================================= 
  STUBS FUNCTION uim_sam_handle_cmd_rsp_timeout_sig
=============================================================================*/
boolean uim_sam_handle_cmd_rsp_timeout_sig
(
  rex_sigs_type *sig_mask_ptr,
  rex_sigs_type *imask_ptr,
  uim_instance_global_type *uim_ptr
)
{
  (void)sig_mask_ptr;
  (void)imask_ptr;
  (void)uim_ptr;
  return FALSE;
}/*uim_sam_handle_cmd_rsp_timeout_sig*/


/*============================================================================= 
  STUBS FUNCTION uim_sam_handle_cmd_q_sig
=============================================================================*/
boolean uim_sam_handle_cmd_q_sig
(
  rex_sigs_type            *sig_mask_ptr,
  rex_sigs_type            *imask_ptr,
  uim_instance_global_type *uim_ptr
)
{
  (void)sig_mask_ptr;
  (void)imask_ptr;
  (void)uim_ptr;
  return FALSE;
}/*uim_sam_handle_cmd_q_sig*/


/*============================================================================= 
  STUBS FUNCTION uim_telecom_modify_imask
=============================================================================*/
void uim_telecom_modify_imask(uim_instance_global_type *uim_ptr, rex_sigs_type *imask_ptr)
{
  (void)imask_ptr;
  (void)uim_ptr;
  return;
}/*uim_telecom_modify_imask*/


/*============================================================================= 
  STUBS FUNCTION uim_sam_modify_imask
=============================================================================*/
void uim_sam_modify_imask(uim_instance_global_type *uim_ptr, rex_sigs_type *imask_ptr)
{
  (void)imask_ptr;
  (void)uim_ptr;
  return;
}/*uim_sam_modify_imask*/


/*============================================================================= 
  STUBS FUNCTION uim_sam_handle_cmd_rsp_sig
=============================================================================*/
boolean uim_sam_handle_cmd_rsp_sig
(
  rex_sigs_type *sig_mask_ptr,
  rex_sigs_type *imask_ptr,
  uim_instance_global_type *uim_ptr
)
{
  (void)sig_mask_ptr;
  (void)imask_ptr;
  (void)uim_ptr;
  return FALSE;
}/*uim_sam_handle_cmd_rsp_sig*/


/*============================================================================= 
  STUBS FUNCTION uim_sam_set_card_mode
=============================================================================*/
void uim_sam_set_card_mode
(
  rex_sigs_type            *imask_ptr,
  uim_instance_global_type *uim_ptr
)
{
  (void)imask_ptr;
  uim_ptr->card_mode = UIM_CARD_TELECOM;
}/*uim_sam_set_card_mode*/


/*============================================================================= 
  STUBS FUNCTION uim_sam_reset_card_mode
=============================================================================*/
void uim_sam_reset_card_mode
(
  rex_sigs_type            *imask_ptr,
  uim_instance_global_type *uim_ptr
)
{
  (void)imask_ptr;
  uim_ptr->card_mode = UIM_CARD_TELECOM;
}/*uim_sam_reset_card_mode*/


/*============================================================================= 
  STUBS FUNCTION uim_is_feature_sam_enabled
=============================================================================*/
boolean uim_is_feature_sam_enabled(void)
{
#ifdef FEATURE_SAM_CARD
  return TRUE;
#else
  return FALSE;
#endif
}/*uim_is_feature_sam_enabled*/


/*============================================================================= 
  STUBS FUNCTION uim_sam_change_mode
=============================================================================*/
void uim_sam_change_mode(
    uim_instance_global_type *uim_ptr
)
{
  (void)uim_ptr;
  return;
}/* uim_sam_change_mode */

