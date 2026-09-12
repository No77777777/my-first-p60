/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*


                      ESTK  SHORT  MESSAGE


GENERAL DESCRIPTION : ENHANCED STK layer Send SMS support

EXTERNALIZED FUNCTIONS

INITIALIZATION AND SEQUENCING REQUIREMENTS


                        COPYRIGHT INFORMATION

Copyright (c) 2008, 2010 - 2011, 2013-2017, 2020 QUALCOMM Technologies, Inc (QTI) and
its licensors.  All Rights Reserved.  QUALCOMM Technologies Proprietary.
Export of this technology or software
is regulated by the U.S. Government. Diversion contrary to U.S. law prohibited.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================
                        EDIT HISTORY FOR MODULE

$Header: //components/rel/uim.mpss/6.1.0/estk/src/estk_sms.c#10 $

when       who     what, where, why
--------   ---     --------------------------------------------------------------------
07/15/20   rm      Fixed issue of wrong slot id for reading block_sms_over_ims NV
11/06/20   rm      Send sms failing when IMS is being selected over CS
06/05/20   srk     Retry SMS on CS only if failed on IMS by checking report status
05/04/20   dt      Fix slot id issue for 3gpp2-SMS
04/26/20   dt      Add return after rejecting 3GPP2 SMS
10/24/19   dt      Retry on CS when Permanent error is received on IMS
03/18/20   ks      Reject 3GPP2-SMS if transport type is IMS
02/10/20   ks      Check for IMS reg status before sending SMS over IMS
03/01/18   ks      Changes to avoid double mem free of wms_msg_ptr 
02/05/18   dt      Get slot from as_id for processing ACK Report
12/28/17   dt      Retry SMS over CS, if IMS is in Limited service
12/28/17   dt      GSTK F3 reduction
08/02/17   gs      Cache WMS transport type
06/28/17   dt      Hold SEND SMS till CP ACK for SMS PP is received
04/24/17   gs      Change in signal for WMS Ready
04/05/17   gs      Don't check for transport capability while sending SMS
02/23/17   gs      After sending the TR in No Service, don't send request to WMS
02/13/17   gs      Check for No Service before sending SMS on CS
12/26/16   gs      Register for SMS Transport Type
05/31/16   gs      F3 Logging improvements
05/27/16   shr     F3 reduction: Malloc Failure, Memcpy, Null Ptr checks
05/11/16   gm      Keeping 3gpp2 code under FEATURE_CDMA
04/29/16   gs      Minor F3 changes
03/07/16   gm      Retry call control when no proactive session on all slot
06/25/15   gm      L4 compiler warning
06/17/15   bd      Changes to support concatenated SMSs
06/05/15   gm      Split ME and slot specific NV
01/12/15   vr      Correction in SEND SMS TR result values
01/02/15   vr      Check WMS ready before sending SMS to WMS
10/08/14   gm      Remove GSTK registration with WMS if FEATURE_ESTK is enabled
09/09/14   vr      Reduce stack usage
09/09/14   bd      Features clean up
08/07/14   dy      Process WMS events in GSTK task context 
07/21/14   sw      Wait for display confirmation before sending terminal response
07/11/14   shr     Add Support for NV Refresh
06/27/14   bd      Work-around for TR problem when FEATURE_ESTK_QMI is disabled but
                   QMI CAT is rgeistered to pick up proactive commands
05/27/14   bd      Changes to hold null alpha and no alpha indications
05/06/14   gs      Removed the ESTK featurization
04/03/14   vr      Do not send Alpha to client when IGNORE_ALPHA is enabled
01/16/14   hn      Replace MSG_* with UIM_MSG_* macros to save memory
01/09/14   kb      Cleanup global variables while handling STOP signal
10/04/13   gm      F3 message reduction
09/13/13   gm      Support for Recovery and hot-swap
08/06/13   vr      Centralized Call Control changes
06/20/13   vr      Triton TSTS changes
04/09/13   vr      Use safer versions of memcpy() and memmove()
06/13/11   sg      Move FEATURE_ESTK_IGNORE_ALPHA to NV
04/28/11   sg      Fix SMS over NAS on SVLTE Type II
01/12/11   adp     Fixing Compiler Error, file got renamed.
09/29/10   xz      KW Fixes
08/18/10   dd      Fix KW error
06/30/10   dd      Icon display confirmation
06/04/10   bd      Fixed the issue of handling null data object in ALPHA TLV
05/24/10   sg      Ignore Alpha TLV for targets without UI
03/09/10   bd      Fixed terminal response for different WMS status
02/15/10   nb      Moved Dual Slot Changes
07/15/09   xz      1) Fix issue of setting wrong DCS in SMS TPDU after pack
                   2) Send error TR if packing fails
12/16/08   sk      Fixed featurization
11/03/08   sk      Featurized ESTK based on external interface support
09/16/08   sk      Added lint fixes
08/21/08   xz      Initial version

===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/

#include "uim_variation.h"
#include "target.h"
#include "customer.h"
#include "comdef.h"
#include "intconv.h"
#include <stringl/stringl.h>
#include "uim_msg.h"
#ifdef FEATURE_UIM_TEST_FRAMEWORK
#error code not present
#endif

#ifdef FEATURE_ESTK
#include "estk_sms.h"
#include "sys.h"
#include "wms.h"

static void estk_wms_cmd_cb (wms_cmd_id_e_type  cmd, const void *user_data_ptr,
                             wms_cmd_err_e_type cmd_err);

static boolean estk_sms_validate_transport_type(wms_client_message_s_type *wms_msg_ptr, 
	                            wms_msg_transport_type_e_type *transport_type_ptr);

extern boolean estk_switch_to_instance(gstk_slot_id_enum_type slot_id);
/*===========================================================================
FUNCTION: estk_sms_send_sms_on_cs

DESCRIPTION:
  Sends the SMS over CS

PARAMETERS:
  slot_id : Slot Identifier

DEPENDENCIES:
  None

RETURN VALUE:
  ESTK_SUCCESS : SMS request sent successfully to WMS
  ESTK_ERROR   : SMS not sent

COMMENTS:
  None

SIDE EFFECTS:
  None

===========================================================================*/
static estk_result_enum_type estk_sms_send_sms_on_cs(gstk_slot_id_enum_type  slot_id)
{
  wms_status_e_type                   wms_status = WMS_OK_S;
  gstk_location_status_enum_type      loc_status = GSTK_NORMAL_SERVICE;
  sys_modem_as_id_e_type              as_id      = SYS_MODEM_AS_ID_1;

  if(!(ESTK_IS_VALID_SLOT_ID(slot_id)) || estk_instances_ptr[(uint8)slot_id - 1] == NULL)
  {
    return ESTK_ERROR;
  }

  if (gstk_io_ctrl(
         GSTK_IO_SLOT_ID_TO_MODEM_AS_ID,
         &slot_id,
         &as_id) != GSTK_SUCCESS)
  {
    return ESTK_ERROR;
  }
  
  (void) gstk_io_ctrl(GSTK_IO_GET_CURR_LOC_STATUS,
                     (void *)&slot_id,
                     (void *)&loc_status);
  
  if(loc_status != GSTK_NO_SERVICE)
  {
    estk_instances_ptr[(uint8)slot_id - 1]->last_trans_type = WMS_MSG_TRANSPORT_TYPE_CS;
    UIM_MSG_HIGH_0("Sending SMS on CS");
    wms_status = wms_msg_ms_send_ext(
                        estk_shared_info.wms_client_id,
                        as_id,
                        (wms_cmd_cb_type)estk_wms_cmd_cb,
                        (const void*)estk_instances_ptr[(uint8)slot_id - 1]->curr_cmd.hdr_cmd.cmd_detail_reference,
                        WMS_SEND_MODE_CLIENT_MESSAGE,
                        estk_instances_ptr[(uint8)slot_id - 1]->wms_msg_cfg.wms_msg_ptr,
                        WMS_MSG_TRANSPORT_TYPE_CS);
  
    /* If WMS returns failure use the report_status to send the error*/
    if (wms_status == WMS_OK_S)
    {
      return ESTK_SUCCESS;
    }
  }
  return ESTK_ERROR;
}

/*===========================================================================
FUNCTION: estk_process_wms_msg_submit_rpt_event_cmd

DESCRIPTION :
  This function will be called by ESTK to process WMS Message Submit
  Report event

PARAMETERS:
  cmd_ptr : [Input] ESTK command pointer

DEPENDENCIES:
  None

RETURN VALUE:
  estk_result_enum_type
    ESTK_SUCCESS : If processing of WMS event is successful
    ESTK_ERROR   : If processing results in error

COMMENTS:
  None

SIDE EFFECTS:
  None

===========================================================================*/
estk_result_enum_type estk_process_wms_msg_submit_rpt_event_cmd(
  const estk_cmd_type *cmd_ptr
)
{
  gstk_additional_info_ptr_type       additional_result       = {0x00};
  uint8                               additional_result_data  = 0x00;
  uint32                              cmd_ref_id              = 0x00;
  gstk_general_result_enum_type       general_result          = GSTK_COMMAND_PERFORMED_SUCCESSFULLY;
  gstk_additional_info_ptr_type      *additional_result_ptr   = NULL;

  if (cmd_ptr == NULL)
  {
    return ESTK_BAD_INPUT_PARAM;
  }

  ESTK_RETURN_ERROR_IF_NULL_PTR(estk_curr_inst_ptr, ESTK_ERROR);

  cmd_ref_id =
    estk_curr_inst_ptr->curr_cmd.hdr_cmd.cmd_detail_reference;

  UIM_MSG_HIGH_3("Fail Type: 0x%x, Last Trans Type: 0x%x, report_status: 0x%x",
           cmd_ptr->cmd_data.wms_msg_sumbit_rpt_evt.delivery_failure_permanent,
           estk_curr_inst_ptr->last_trans_type,
           cmd_ptr->cmd_data.wms_msg_sumbit_rpt_evt.report_status);

  /* Retry SMS on CS, if it is last tried on IMS and failed */
  if (cmd_ptr->cmd_data.wms_msg_sumbit_rpt_evt.report_status != WMS_RPT_OK && 
      estk_curr_inst_ptr->last_trans_type == WMS_MSG_TRANSPORT_TYPE_SMS_OVER_IMS &&
      estk_curr_inst_ptr->wms_msg_cfg.wms_msg_ptr != NULL)
  {    
    if(estk_sms_send_sms_on_cs(estk_curr_inst_ptr->slot_id) == ESTK_SUCCESS)
      {
        return ESTK_SUCCESS;
      }
    }

  switch(cmd_ptr->cmd_data.wms_msg_sumbit_rpt_evt.report_status)
  {
    case WMS_RPT_OK:
      general_result = GSTK_COMMAND_PERFORMED_SUCCESSFULLY;
      additional_result_ptr = NULL;
      break;

    case WMS_RPT_MO_CONTROL_DISALLOW:
    case WMS_RPT_MO_CONTROL_ERROR:
      /* Call control did not allow mo sms send or Call control Error*/
      additional_result.length = 1;
      additional_result_data = int32touint8((int32)GSTK_ACTION_NOT_ALLOWED);
      additional_result.additional_info_ptr = &additional_result_data;
      general_result = GSTK_INTERACTION_WITH_CC_BY_SIM_OR_MO_SM_CONTROL_BY_SIM_PROBLEM;
      additional_result_ptr = &additional_result;
      break;

    case WMS_RPT_RP_ERROR:
      general_result = GSTK_SMS_RP_ERROR;
      additional_result.length = 1;
      additional_result_data =
        0x7F & cmd_ptr->cmd_data.wms_msg_sumbit_rpt_evt.cause_info.cause_value;
      additional_result.additional_info_ptr = &additional_result_data;
      additional_result_ptr = &additional_result;
      break;

    case WMS_RPT_CP_ERROR:
    case WMS_RPT_NO_RESPONSE_FROM_NETWORK:
    case WMS_RPT_NO_RESPONSE_FROM_NEWTWORK_FOR_SUBMIT_REPORT:
    case WMS_RPT_NETWORK_NOT_READY:
    case WMS_RPT_CDMA_TL_ERROR:
    case WMS_RPT_NO_ACK:
      general_result = GSTK_NETWORK_CURRENTLY_UNABLE_TO_PROCESS_COMMAND;
      additional_result.length = 1;
      additional_result_data = 
        int32touint8((int32)GSTK_NO_SPECIFIC_CAUSE_CAN_BE_GIVEN);
      additional_result.additional_info_ptr = &additional_result_data;
      additional_result_ptr = &additional_result;
      break;

    case WMS_RPT_1X_AND_VOICE_NOT_ALLOWED:
      general_result = GSTK_ACCESS_TECH_UNABLE_TO_PROCESS_CMD;
      additional_result_ptr = NULL;
      break;

    default:
      additional_result.length = 1;
      additional_result_data =
        int32touint8((int32)GSTK_NO_SPECIFIC_CAUSE_CAN_BE_GIVEN);
      additional_result.additional_info_ptr = &additional_result_data;
      general_result = GSTK_ME_CURRENTLY_UNABLE_TO_PROCESS_COMMAND;
      additional_result_ptr = &additional_result;
      break;
  }

  estk_curr_inst_ptr->additional_result.additional_info_ptr = NULL;
  estk_curr_inst_ptr->additional_result.length = 0;

   /* SMS message has been sent, this event received from wms event callback */
  if(estk_curr_inst_ptr->state == ESTK_WAIT_FOR_DISPLAY_ALPHA_CNF_ST)
  {
    UIM_MSG_HIGH_0("Saving SEND SMS TR: Waiting for alpha display rsp");      
    estk_curr_inst_ptr->state = ESTK_PENDING_TR_ST;
    estk_curr_inst_ptr->result = general_result;

    if(additional_result_ptr != NULL)
    {
      estk_cache_additional_info(&additional_result);
    }
#ifdef FEATURE_UIM_TEST_FRAMEWORK
    #error code not present
#endif /* FEATURE_UIM_TEST_FRAMEWORK */   
  }
  else
  {
    (void)estk_send_terminal_response(
            cmd_ref_id,                          /* command_details_ref */
            GSTK_SEND_SMS_CNF,                   /* command response    */
            general_result,                      /* general result      */
            additional_result_ptr,               /* additional result   */
            NULL);                               /* extra param         */
  }
  return ESTK_SUCCESS;
} /* estk_process_wms_msg_submit_rpt_event_cmd */


/*===========================================================================
FUNCTION: estk_process_wms_event_ack_report_cmd

DESCRIPTION :
  This function will be called by ESTK to process WMS ACK report event

PARAMETERS:
  cmd_ptr : [Input] ESTK command pointer

DEPENDENCIES:
  None

RETURN VALUE:
  estk_result_enum_type
    ESTK_SUCCESS : If processing of WMS event is successful
    ESTK_ERROR   : If processing results in error

COMMENTS:
  None

SIDE EFFECTS:
  None

===========================================================================*/
estk_result_enum_type estk_process_wms_event_ack_report_cmd(
  const estk_cmd_type *cmd_ptr
)
{
  wms_status_e_type              wms_status        = WMS_OK_S;
  gstk_slot_id_enum_type         slot_id           = GSTK_SLOT_ID_MAX;
  
  if (cmd_ptr == NULL)
  {
    return ESTK_BAD_INPUT_PARAM;
  }

  /* WMS ACK event would only be sent to GSTK w.r.t. MT SMS directed to the card. */
  if((estk_curr_inst_ptr->wms_msg_cfg.wms_msg_ptr != NULL) && 
     (gstk_nv_get_feature_2_status(
      GSTK_CFG_FEATURE_2_SLOT_DISABLE_HOLDING_SEND_SMS_TILL_SMS_PP_ACK,
      slot_id) == FALSE))
  {
    wms_status = wms_msg_ms_send_ext(
                            estk_shared_info.wms_client_id,
                            cmd_ptr->hdr.as_id,
                            (wms_cmd_cb_type)estk_wms_cmd_cb,
                            (const void*)estk_curr_inst_ptr->curr_cmd.hdr_cmd.cmd_detail_reference,
                            WMS_SEND_MODE_CLIENT_MESSAGE,
                            estk_curr_inst_ptr->wms_msg_cfg.wms_msg_ptr,
                            estk_curr_inst_ptr->trans_type);
      
    if (wms_status != WMS_OK_S)
    {
      UIM_MSG_ERR_1("wms_msg_send failed 0x%x", wms_status);
      gstk_free(estk_curr_inst_ptr->wms_msg_cfg.wms_msg_ptr);
      estk_curr_inst_ptr->wms_msg_cfg.wms_msg_ptr = NULL;
      (void)estk_send_terminal_response(
              estk_curr_inst_ptr->curr_cmd.hdr_cmd.cmd_detail_reference,  
              GSTK_SEND_SMS_CNF,           /* command response */
              GSTK_ME_CURRENTLY_UNABLE_TO_PROCESS_COMMAND, /* general_result */
              NULL,               /* additional result */
              NULL);              /* extra param */

    }
  }

  return ESTK_SUCCESS;
} /* estk_process_wms_event_ack_report_cmd */


/*===========================================================================
FUNCTION: estk_wms_event_cb

DESCRIPTION:
  This function is the event callback function for WMS.

PARAMETERS:
  wms_event:  [Input] Command whose status is being reported
  info_ptr:   [Input] WMS info buffer
  shared_ptr: [Input] WMS indication of whether the info can be passed to
                      other clients.

DEPENDENCIES:
  None

RETURN VALUE:
  None

COMMENTS:
  None

SIDE EFFECTS:
  None

===========================================================================*/
static void estk_wms_event_cb(
  wms_msg_event_e_type              wms_event,
  const wms_msg_event_info_s_type  *info_ptr,
  const boolean                    *shared_ptr)
{
  estk_cmd_type          *task_cmd_ptr      = NULL;
  gstk_slot_id_enum_type  slot_id           = GSTK_SLOT_ID_MAX;  
  boolean                 is_cp_ack_pending = FALSE;

  (void)shared_ptr;

  if (info_ptr == NULL)
  {
    return;
  }

  if(wms_event == WMS_MSG_EVENT_MT_MESSAGE_ERROR)
  {
    gstk_io_ctrl(
       GSTK_IO_SEND_WMS_MT_MESSAGE_ERROR_EVENT, 
       (void *)info_ptr, 
       &slot_id);
    return;    
  }

  switch(wms_event)
  {
    case WMS_MSG_EVENT_SUBMIT_REPORT:
      if(info_ptr->submit_report_info.client_id == WMS_CLIENT_TYPE_CAT)
      {
        slot_id = estk_shared_info.slot_owns_wms_cli;
        if (!ESTK_IS_VALID_SLOT_ID(slot_id) || slot_id == GSTK_SLOT_ID_MAX)
        {
           UIM_MSG_ERR_1("estk_wms_event_cb, invalid slot id 0x%x", slot_id);
           return;
        }

        if (estk_instances_ptr[(uint8)slot_id - 1]->curr_cmd.hdr_cmd.command_id !=
                                  GSTK_SEND_SMS_REQ)
        {
          UIM_MSG_ERR_1("estk_wms_event_cb, slot 0x%x, No WMS command", slot_id);
          return;
        }

        task_cmd_ptr = estk_get_cmd_buf();
        
        if(task_cmd_ptr == NULL)
        {
          return;
        }
		
        UIM_MSG_HIGH_3("In estk_wms_event_cb, event: 0x%x, report_status: 0x%x, slot_id: 0x%x",
                          wms_event, info_ptr->submit_report_info.report_status, slot_id);
        
        /* Set the command header */
        task_cmd_ptr->hdr.cli_user_data    = 0;
        task_cmd_ptr->hdr.cmd              = ESTK_WMS_MSG_SUBMIT_RPT_EVT_CMD;
        task_cmd_ptr->hdr.sim_slot_id      = slot_id;
        
        /* Set the report status */   
        task_cmd_ptr->cmd_data.wms_msg_sumbit_rpt_evt.report_status =
          info_ptr->submit_report_info.report_status;
        task_cmd_ptr->cmd_data.wms_msg_sumbit_rpt_evt.cause_info =
          info_ptr->submit_report_info.cause_info;
        task_cmd_ptr->cmd_data.wms_msg_sumbit_rpt_evt.delivery_failure_permanent =
          info_ptr->submit_report_info.delivery_failure_permanent;

        /* Queue the command to ESTK */
        estk_put_cmd(task_cmd_ptr);
      }
      break;

    case WMS_MSG_EVENT_TRANSPORT_REG:
      if(!ESTK_IS_VALID_AS_ID(info_ptr->transport_reg_info.as_id))
      {
        UIM_MSG_ERR_1("estk_wms_event_cb, invalid as_id 0x%x", 
			                info_ptr->transport_reg_info.as_id);
        return;
      }
      
      rex_enter_crit_sect(&estk_shared_info.estk_crit_sect);
      estk_shared_info.wms_info[info_ptr->transport_reg_info.as_id].trans_changed = TRUE;
      estk_shared_info.wms_info[info_ptr->transport_reg_info.as_id].as_id =
	  	            info_ptr->transport_reg_info.as_id;
      estk_shared_info.wms_info[info_ptr->transport_reg_info.as_id].wms_transport_type.reg_status = 
                            info_ptr->transport_reg_info.is_registered;
      estk_shared_info.wms_info[info_ptr->transport_reg_info.as_id].wms_transport_type.trans_type = 
                            info_ptr->transport_reg_info.transport_type;
      rex_leave_crit_sect(&estk_shared_info.estk_crit_sect);
      break;

    case WMS_MSG_EVENT_ACK_REPORT:
      if(gstk_io_ctrl(GSTK_IO_GET_WMS_IS_CP_ACK_PENDING_FLAG,
           &info_ptr->ack_report_info.as_id, &is_cp_ack_pending) != GSTK_SUCCESS)
      {
        return;
      }

      if(is_cp_ack_pending == TRUE)
      {
        (void)gstk_io_ctrl(GSTK_IO_RESET_WMS_IS_CP_ACK_PENDING_FLAG,
                           &info_ptr->ack_report_info.as_id, &is_cp_ack_pending);
        
        task_cmd_ptr = estk_get_cmd_buf();
        
        if(task_cmd_ptr == NULL || 
           (gstk_io_ctrl(GSTK_IO_MODEM_AS_ID_TO_SLOT_ID,
                         &info_ptr->ack_report_info.as_id,
                         &slot_id) != GSTK_SUCCESS))
        {
          return;
        }
      
        /* Set the command header */
        task_cmd_ptr->hdr.cli_user_data    = 0;
        task_cmd_ptr->hdr.cmd              = ESTK_WMS_MSG_EVENT_ACK_REPORT;
        task_cmd_ptr->hdr.as_id            = info_ptr->ack_report_info.as_id;
        task_cmd_ptr->hdr.sim_slot_id      = slot_id;

        /* Queue the command to ESTK */
        estk_put_cmd(task_cmd_ptr);
      }
      else
      {
        UIM_MSG_LOW_1("CP_ACK received is not for MT SMS sent to card 0x%x", is_cp_ack_pending);
      }
      break;

    default:
      UIM_MSG_MED_1("estk_wms_event_cb, Unhandled event 0x%x", wms_event);
      break;
  }
} /* estk_wms_event_cb */

/*===========================================================================
FUNCTION: estk_wms_cmd_cb

DESCRIPTION:
  This is the command callback function for the wms_send_msg() call.

PARAMETERS:
  cmd           : [Input] Command to which the callback is invoked
  user_data_ptr : [Input] user data, used as cmd_reference_id
  cmd_err       : [Input] command result

DEPENDENCIES:
  None

RETURN VALUE:
  None

COMMENTS:
  None

SIDE EFFECTS:
  None

===========================================================================*/
static void estk_wms_cmd_cb (
  wms_cmd_id_e_type  cmd,
  const void        *user_data_ptr,
  wms_cmd_err_e_type cmd_err
)
{
  uint32                    cmd_ref_id = GSTK_MAX_PROACTIVE_COMMAND;
  gstk_slot_id_enum_type    slot_id    = GSTK_SLOT_ID_MAX;  

  UIM_MSG_HIGH_3("estk_wms_cmd_cb: cmd=0x%x, user_data_ptr=0x%x, cmd_err=0x%x",
                 cmd, user_data_ptr, cmd_err);

  slot_id = estk_shared_info.slot_owns_wms_cli;
  if (!ESTK_IS_VALID_SLOT_ID(slot_id) || slot_id == GSTK_SLOT_ID_MAX)
  {
    UIM_MSG_ERR_1("invalid slot id 0x%x!", slot_id);
    return;
  }

  if ((estk_instances_ptr[(uint8)slot_id - 1] == NULL) ||
      (estk_instances_ptr[(uint8)slot_id - 1]->curr_cmd.hdr_cmd.command_id != GSTK_SEND_SMS_REQ) ||
      (estk_instances_ptr[(uint8)slot_id - 1]->wms_msg_cfg.wms_msg_ptr == NULL))
  {
    UIM_MSG_ERR_0("estk_wms_cmd_cb: No SM cmd!");
    return;
  }

  switch (cmd)
  {
    case WMS_CMD_MSG_SEND:
      if(cmd_err == WMS_CMD_ERR_BUSY)
      {
        UIM_MSG_HIGH_0("WMS is not ready, wait for WMS ready signal then send SMS");
        break;
      }
      /* Retry SMS on CS, if the SMS fails due to (Eg: Limited service) Error when sent over IMS */
      else if(cmd_err == WMS_CMD_ERR_TRANSPORT_NOT_ALLOWED &&
              estk_instances_ptr[(uint8)slot_id - 1]->last_trans_type == WMS_MSG_TRANSPORT_TYPE_SMS_OVER_IMS)
      {
         if(estk_sms_send_sms_on_cs(slot_id) == ESTK_SUCCESS)
         {
           UIM_MSG_HIGH_0("SMS Sent successfully");
           return;
         }
      }

      if(cmd_err != WMS_CMD_ERR_NONE)
      {
        cmd_ref_id = (uint32)((void*)user_data_ptr);
        if(estk_instances_ptr[(uint8)slot_id - 1]->state == ESTK_WAIT_FOR_DISPLAY_ALPHA_CNF_ST)
        {
          estk_instances_ptr[(uint8)slot_id - 1]->state = ESTK_PENDING_TR_ST;
          estk_instances_ptr[(uint8)slot_id - 1]->result = GSTK_ME_CURRENTLY_UNABLE_TO_PROCESS_COMMAND;
        }
        else
        { /* The error in this call back is because of internal ME error */
          (void)estk_send_terminal_response(
                  cmd_ref_id,                          /* command_details_ref */
                  GSTK_SEND_SMS_CNF,                   /* command response    */
                  GSTK_ME_CURRENTLY_UNABLE_TO_PROCESS_COMMAND,
                                                       /* general result      */
                  NULL,                                /* additional result   */
                  NULL);                               /* extra param         */
        }
      }
      break;
    case WMS_CMD_CFG_SET_LINK_CONTROL:
      if(cmd_err != WMS_CMD_ERR_NONE)
      {
        UIM_MSG_ERR_0("Unable to set link ctrl timer");
      }
      break;
    default:
      break;
  }
  /*if no error was recieved its okay, wait for event callback to be called */
} /*estk_wms_cmd_cb */

/*===========================================================================
FUNCTION: estk_wms_cfg_cb

DESCRIPTION:
  This function is the cfg callback function for WMS.

PARAMETERS:
  wms_event:  [Input] Configuration event type
  info_ptr:   [Input] Configuration event information


DEPENDENCIES:
  None

RETURN VALUE:
  None

COMMENTS:
  None

SIDE EFFECTS:
  None

===========================================================================*/
static void estk_wms_cfg_cb(
  wms_cfg_event_e_type              wms_event,
  wms_cfg_event_info_s_type        *info_ptr)
{
  estk_cmd_type          *task_cmd_ptr = NULL;
  gstk_slot_id_enum_type  slot_id      = GSTK_SLOT_ID_MAX;

  if (info_ptr == NULL)
  {
    UIM_MSG_ERR_1("estk_wms_cfg_cb, NULL PTR: info_ptr = 0x%x",
              info_ptr);
    return;    
  }

  if (wms_event != WMS_CFG_EVENT_MS_READY_STATUS)
  {
    return;
  }

  if (gstk_io_ctrl(
        GSTK_IO_MODEM_AS_ID_TO_SLOT_ID,
        &info_ptr->ms_ready_status.as_id,
        &slot_id) != GSTK_SUCCESS)
  {
    return;
  }
  if (!ESTK_IS_VALID_SLOT_ID(slot_id) || slot_id == GSTK_SLOT_ID_MAX)
  {
    return;
  }

  UIM_MSG_HIGH_3("In estk_wms_cfg_cb, event: 0x%x, sys type: 0x%x, slot: 0x%x",
           wms_event,
           info_ptr->ms_ready_status.sys_type,
           slot_id);

  if (estk_instances_ptr[(uint8)slot_id - 1] == NULL)
  {
    UIM_MSG_ERR_1("estk_wms_cfg_cb, estk_instances_ptr[%d] is NULL",
              ((uint8)slot_id - 1));
    return;
  }

  task_cmd_ptr = estk_get_cmd_buf();
  
  if(task_cmd_ptr == NULL)
  {
    return;
  }
  
  /* Set the command header */
  task_cmd_ptr->hdr.cli_user_data    = 0;
  task_cmd_ptr->hdr.cmd              = ESTK_WMS_CFG_EVT_CMD;
  task_cmd_ptr->hdr.sim_slot_id      = slot_id;
  
  /* Set the report status */   
  task_cmd_ptr->cmd_data.wms_cfg_evt.sys_type = info_ptr->ms_ready_status.sys_type;
  task_cmd_ptr->cmd_data.wms_cfg_evt.ready_status = info_ptr->ms_ready_status.ready_status;

  /* Queue the command to ESTK */
  estk_put_cmd(task_cmd_ptr);

} /* estk_wms_cfg_cb */

/*===========================================================================
FUNCTION: estk_msg_transport_reg_status_cb

DESCRIPTION:
  This function is the event callback function for WMS to know IMS registration status.

PARAMETERS:
  status_ptr:  [Input] Info pointer with IMS reg status

DEPENDENCIES:
  None

RETURN VALUE:
  None

COMMENTS:
  None

SIDE EFFECTS:
  None

===========================================================================*/
static void estk_msg_transport_reg_status_cb(
  wms_msg_transport_status_s_type   *status_ptr
)
{
  estk_cmd_type          *task_cmd_ptr = NULL;
  gstk_slot_id_enum_type  slot_id      = GSTK_SLOT_ID_MAX;  

  if (status_ptr == NULL)
  {
    return;
  }

  if (gstk_io_ctrl(
      GSTK_IO_MODEM_AS_ID_TO_SLOT_ID,
      &status_ptr->as_id,
      &slot_id) != GSTK_SUCCESS)
  {
    return;
  }
  if (!ESTK_IS_VALID_SLOT_ID(slot_id) || slot_id == GSTK_SLOT_ID_MAX)
  {
    return;
  }

  UIM_MSG_HIGH_2("In estk_ims_reg_status_cb, IMS reg status: 0x%x, slot: 0x%x",
         status_ptr->transport_status,
         slot_id);
  
  rex_enter_crit_sect(&estk_shared_info.estk_crit_sect);
  estk_shared_info.wms_info[slot_id-1].transport_reg_ims_status = status_ptr->transport_status;
  rex_leave_crit_sect(&estk_shared_info.estk_crit_sect);

} /* estk_msg_transport_reg_status_cb */

/*===========================================================================
FUNCTION: estk_process_wms_cfg_event_cmd

DESCRIPTION :
  This function will be called by ESTK to process WMS Configuration event

PARAMETERS:
  cmd_ptr : [Input] ESTK command pointer

DEPENDENCIES:
  None

RETURN VALUE:
  estk_result_enum_type
    ESTK_SUCCESS : If processing of WMS event is successful
    ESTK_ERROR   : If processing results in error

COMMENTS:
  None

SIDE EFFECTS:
  None

===========================================================================*/
estk_result_enum_type estk_process_wms_cfg_event_cmd(
  const estk_cmd_type *cmd_ptr
)
{
  wms_msg_transport_type_e_type transport_type  = WMS_MSG_TRANSPORT_TYPE_CS;
  
  if (cmd_ptr == NULL)
  {
    return ESTK_BAD_INPUT_PARAM;
  }

  ESTK_RETURN_ERROR_IF_NULL_PTR(estk_curr_inst_ptr, ESTK_ERROR);

  switch(cmd_ptr->cmd_data.wms_cfg_evt.ready_status)
  {
    case WMS_CFG_READY_STATUS_LIMITED:
    case WMS_CFG_READY_STATUS_FULL:
      if(cmd_ptr->cmd_data.wms_cfg_evt.sys_type == WMS_CFG_SYS_GW)
      {
        estk_curr_inst_ptr->wms_msg_cfg.wms_ready_gw = TRUE;
      }
      else
      {
        estk_curr_inst_ptr->wms_msg_cfg.wms_ready_cdma = TRUE;
      }
      break;
    case WMS_CFG_READY_STATUS_NONE:
    case WMS_CFG_READY_STATUS_NO_SRV:
      if(cmd_ptr->cmd_data.wms_cfg_evt.sys_type == WMS_CFG_SYS_GW)
      {
        estk_curr_inst_ptr->wms_msg_cfg.wms_ready_gw = FALSE;
      }
      else
      {
        estk_curr_inst_ptr->wms_msg_cfg.wms_ready_cdma = FALSE;
      }
      break;
    default:
      break;
  }

  UIM_MSG_HIGH_4("estk_process_wms_cfg_event_cmd, GW ready: 0x%x, CDMA ready: 0x%x,"
                  "Ready Status: 0x%x, wms_msg_ptr: 0x%x", 
                  estk_curr_inst_ptr->wms_msg_cfg.wms_ready_gw,
                  estk_curr_inst_ptr->wms_msg_cfg.wms_ready_cdma, 
                  cmd_ptr->cmd_data.wms_cfg_evt.ready_status,
                  estk_curr_inst_ptr->wms_msg_cfg.wms_msg_ptr);

  /* Check if any cached wms msg is present */
  if(estk_curr_inst_ptr->wms_msg_cfg.wms_msg_ptr != NULL)
  {
    wms_client_message_s_type *wms_msg_ptr = estk_curr_inst_ptr->wms_msg_cfg.wms_msg_ptr;
    sys_modem_as_id_e_type     as_id       = SYS_MODEM_AS_ID_1;
    wms_status_e_type          wms_status  = WMS_OK_S;

    if(estk_curr_inst_ptr->curr_cmd.hdr_cmd.command_id != GSTK_SEND_SMS_REQ)
    {
      UIM_MSG_ERR_0("No SEND SMS proactive command pending");
      gstk_free(estk_curr_inst_ptr->wms_msg_cfg.wms_msg_ptr);
      estk_curr_inst_ptr->wms_msg_cfg.wms_msg_ptr = NULL;
      return ESTK_ERROR;
    }

    if (gstk_io_ctrl(
          GSTK_IO_SLOT_ID_TO_MODEM_AS_ID,
          &estk_shared_info.slot_owns_wms_cli,
          &as_id) != GSTK_SUCCESS)
    {
      gstk_free(estk_curr_inst_ptr->wms_msg_cfg.wms_msg_ptr);
      estk_curr_inst_ptr->wms_msg_cfg.wms_msg_ptr = NULL;
      return ESTK_ERROR;
    }

    /* If returned TRUE means TR is already sent */
    if(estk_sms_validate_transport_type(wms_msg_ptr, &transport_type) == TRUE)
    {
      return ESTK_SUCCESS;
    }

    wms_status = wms_msg_ms_send_ext(
      estk_shared_info.wms_client_id,
      as_id,
      (wms_cmd_cb_type)estk_wms_cmd_cb,
      (const void*)estk_curr_inst_ptr->curr_cmd.hdr_cmd.cmd_detail_reference,
      WMS_SEND_MODE_CLIENT_MESSAGE,
      wms_msg_ptr,
      transport_type);
  
    if (wms_status != WMS_OK_S)
    {
      UIM_MSG_ERR_1("wms_msg_send failed 0x%x", wms_status);
      gstk_free(estk_curr_inst_ptr->wms_msg_cfg.wms_msg_ptr);
      estk_curr_inst_ptr->wms_msg_cfg.wms_msg_ptr = NULL;
    }
  }
  return ESTK_SUCCESS;
} /* estk_process_wms_cfg_event_cmd */

#if defined(FEATURE_WCDMA) || defined(FEATURE_GSM) || defined(FEATURE_LTE)
/*=========================================================================
FUNCTION:  estk_ts_decode_address

DESCRIPTION:
  Decode address data into a structure.

PARAMETERS:
  data_ptr : [Input] Input data to be decode
  addr_ptr : [Ouput] WMS Address data structure to be populated

DEPENDENCIES:
  None

RETURN VALUE:
  Number of bytes decoded.

COMMENTS:
  None

SIDE EFFECTS:
  None

=========================================================================*/
static uint8 estk_ts_decode_address (
  const uint8               * data_ptr,
  wms_address_s_type        * addr_ptr
)
{
  uint8   i, pos = 0;

  if((data_ptr == NULL) || addr_ptr == NULL)
  {
    /* data is NULL */
    UIM_MSG_ERR_0("Parameter for estk_ts_decode_address is NULL");
    return 0;
  }

  /* Len field: number of digits */
  i = data_ptr[pos];

  if( i > WMS_GW_ADDRESS_MAX )
  {
    /* Address is too long */
    UIM_MSG_ERR_1("Addr len too long: %d", i);
    return 0;
  }

  addr_ptr->number_of_digits = i;

  pos ++;

  /* TON & NPI: */
  addr_ptr->digit_mode  = WMS_DIGIT_MODE_4_BIT;

  addr_ptr->number_type =
    (wms_number_type_e_type)(( data_ptr[pos] & 0x70 ) >> 4);

  addr_ptr->number_plan = (wms_number_plan_e_type)(data_ptr[pos] & 0x0F);
  pos ++;

  if (addr_ptr->number_type == WMS_NUMBER_ALPHANUMERIC )
  {
    uint8 bytes_increment = 0;

    /* Alphanumberic Number Type */
    addr_ptr->digit_mode = WMS_DIGIT_MODE_8_BIT;

    /* length = number of BCD digits */
    bytes_increment = (addr_ptr->number_of_digits+1)/2;

    pos += bytes_increment;
  }
  else
  {
    /* the digits: */
    for( i = 0; i < addr_ptr->number_of_digits; i++ )
    {
      /* unpack two digits each time */
      addr_ptr->digits[i++] = data_ptr[pos] & 0x0F;
      addr_ptr->digits[i]   = ( data_ptr[pos] & 0xF0 ) >> 4;
      pos ++;
    }
  }
  return pos;
} /* estk_ts_decode_address() */
#endif /* #if defined(FEATURE_WCDMA) || defined(FEATURE_GSM) || defined(FEATURE_LTE)*/

/*===========================================================================
FUNCTION: estk_sms_reg

DESCRIPTION:
  ESTK registration of WMS client

PARAMETERS:
  None

DEPENDENCIES:
  None

RETURN VALUE:
  None

COMMENTS:
  None

SIDE EFFECTS:
  None

===========================================================================*/
void estk_sms_reg( void )
{
  wms_client_err_e_type wms_client_err = WMS_CLIENT_ERR_MAX;

  UIM_MSG_HIGH_0("In estk_sms_reg()");

  wms_client_err = wms_client_init(WMS_CLIENT_TYPE_CAT,
                                   &estk_shared_info.wms_client_id );
  if (wms_client_err != WMS_CLIENT_ERR_NONE)
  {
    UIM_MSG_ERR_1("Failed to init ESTK WMS client. (0x%x)", wms_client_err);
    return;
  }

  wms_client_err = wms_client_activate( estk_shared_info.wms_client_id );

  if (wms_client_err != WMS_CLIENT_ERR_NONE)
  {
    (void)wms_client_release( estk_shared_info.wms_client_id );
    UIM_MSG_ERR_1("Failed to activate ESTK WMS client. (0x%x)",
                  wms_client_err);
    return;
  }

  wms_client_err = wms_client_reg_msg_cb(
    estk_shared_info.wms_client_id,
    (wms_msg_event_cb_type)estk_wms_event_cb);

  if (wms_client_err != WMS_CLIENT_ERR_NONE)
  {
    (void)wms_client_release( estk_shared_info.wms_client_id );
    UIM_MSG_ERR_1("Failed to reg ESTK WMS evt cb. (0x%x)", wms_client_err);
  }

  wms_client_err = wms_client_reg_transport_status_cb(
    estk_shared_info.wms_client_id,
    (wms_msg_transport_status_cb_type)estk_msg_transport_reg_status_cb);

  wms_client_err = wms_client_reg_cfg_cb(
    estk_shared_info.wms_client_id,
    (wms_cfg_event_cb_type)estk_wms_cfg_cb);

  if (wms_client_err != WMS_CLIENT_ERR_NONE)
  {
    (void)wms_client_release( estk_shared_info.wms_client_id );
    UIM_MSG_ERR_1("Failed to reg ESTK WMS evt cb. (0x%x)", wms_client_err);
  }
  estk_shared_info.slot_owns_wms_cli = GSTK_SLOT_ID_MAX;
} /* estk_sms_reg */

/*===========================================================================
FUNCTION: estk_process_send_sms_req

DESCRIPTION:
  GSTK calls this ESTK function to process the SEND SMS request.

PARAMETERS:
  gstk_req_ptr : [Input] gstk SEND SMS cmd pointer

DEPENDENCIES:
  None

RETURN VALUE:
  estk_status_enum_type
    ESTK_SUCCESS: - When SMS is successfully sent
    ESTK_ERROR:   - When there is failure in sending SMS to WMS module

COMMENTS:
  None

SIDE EFFECTS:
  None

===========================================================================*/
estk_result_enum_type estk_process_send_sms_req(
  gstk_cmd_from_card_type *gstk_req_ptr
)
{
  gstk_send_sms_req_type        *send_sms_req_ptr  = NULL;
  wms_status_e_type              wms_status        = WMS_OK_S;
  uint32                         dcs_pos           = 2;
  /* Cannot do {0} due to compilation warning on enumerated type
     mixed with another type */
  wms_client_message_s_type     *wms_msg_ptr       = NULL;
  wms_address_s_type             dummy_address;
  sys_modem_as_id_e_type         as_id             = SYS_MODEM_AS_ID_1;
  boolean                        is_concatenated   = FALSE;
  gstk_status_enum_type          result            = GSTK_SUCCESS;
  gstk_nv_items_data_type        nv_data           = {0};
  uint8                          timer_val         = 0;
  wms_msg_transport_type_e_type  transport_type    = WMS_MSG_TRANSPORT_TYPE_CS;
  boolean                        is_cp_ack_pending = FALSE;
  uint8                          i                = 0;

  ESTK_RETURN_ERROR_IF_NULL_PTR(estk_curr_inst_ptr, ESTK_ERROR);
  
  estk_curr_inst_ptr->state = ESTK_PENDING_TR_ST;

  memset(&dummy_address, 0x00, sizeof(wms_address_s_type));

  /* Check input parameter */
  if (gstk_req_ptr == NULL)
  {
    UIM_MSG_ERR_0("NULL ptr in estk_process_send_sms_req()");
    return ESTK_ERROR;
  }

  if (estk_shared_info.slot_owns_wms_cli != GSTK_SLOT_ID_MAX)
  {
    UIM_MSG_ERR_1("WMS client is busy (0x%x)!",
                  estk_shared_info.slot_owns_wms_cli);
    return ESTK_ERROR;
  }
  else
  {
    estk_shared_info.slot_owns_wms_cli = gstk_req_ptr->hdr_cmd.sim_slot_id;
  }

  if (gstk_io_ctrl(
        GSTK_IO_SLOT_ID_TO_MODEM_AS_ID,
        &estk_shared_info.slot_owns_wms_cli,
        &as_id) != GSTK_SUCCESS)
  {
    UIM_MSG_ERR_1("failed to get as_id for slot 0x%x!",
                  estk_shared_info.slot_owns_wms_cli);
    return ESTK_ERROR;
  }

  wms_msg_ptr = (wms_client_message_s_type *)GSTK_CALLOC(sizeof(wms_client_message_s_type));
  if(wms_msg_ptr == NULL)
  {
    return ESTK_ERROR;
  }

  send_sms_req_ptr = &gstk_req_ptr->cmd.send_sms_pro_cmd_req;

  estk_curr_inst_ptr->is_3gpp2_sms = send_sms_req_ptr->is_cdma_sms;

  for(i = SYS_MODEM_AS_ID_1; i < GSTK_MAX_NUM_OF_INSTANCES; i++)
  { 
    /* Check if the transport type been changed */
    if((estk_shared_info.wms_info[i].trans_changed == TRUE) &&
       /* Match the Slot Id */
       (estk_shared_info.slot_owns_wms_cli == estk_curr_inst_ptr->slot_id) &&
       (as_id == (sys_modem_as_id_e_type)i))
    {
      if ((estk_shared_info.wms_info[i].wms_transport_type.trans_type ==
                      WMS_MSG_TRANSPORT_TYPE_SMS_OVER_IMS) &&
          (estk_shared_info.wms_info[i].wms_transport_type.reg_status == TRUE)&&
          (estk_shared_info.wms_info[i].transport_reg_ims_status == WMS_MSG_TRANSPORT_NW_REG_STATUS_FULL_SRV))
      {
        estk_curr_inst_ptr->trans_type = 
              WMS_MSG_TRANSPORT_TYPE_SMS_OVER_IMS;
      }
      else
      {
        estk_curr_inst_ptr->trans_type = WMS_MSG_TRANSPORT_TYPE_CS;
      }
      break;
    }
  }
  
  /* If UICC triggers 3GPP2-SMS and transport type is IMS, block 3GPP2-SMS and
   * send error TR to card*/
  if (!((estk_curr_inst_ptr->trans_type == WMS_MSG_TRANSPORT_TYPE_SMS_OVER_IMS) &&
      (estk_curr_inst_ptr->is_3gpp2_sms == TRUE) &&
      (gstk_nv_get_feature_2_status(
         GSTK_CFG_FEATURE_2_SLOT_BLOCK_3GPP2_SMS_OVER_IMS,
         estk_shared_info.slot_owns_wms_cli) == TRUE)))
  {

  if(gstk_nv_get_me_feature_status(GSTK_CFG_FEATURE_ME_ESTK_IGNORE_ALPHA) == FALSE)
  {
    /* Check Alpha availability */
    /* Even in case of null data object in ALPHA (i.e. length = '00' and no value part),
       GSTK make sures at least NULL char is present in alpha text and length includes
       that NULL character. So the min length is one and hence we need to check for
       length more than one */
    if(((send_sms_req_ptr->alpha.length) > 1 && (send_sms_req_ptr->alpha.text)) ||
       (gstk_nv_get_me_feature_status(
           GSTK_CFG_FEATURE_ME_ESTK_QMI_SEND_NO_AND_NULL_ALPHA_IND) == TRUE))
    {
      if (estk_send_alpha_to_display_client(gstk_req_ptr, TRUE) != ESTK_SUCCESS)
      {
        UIM_MSG_ERR_0("Send SMS Alpha Display failed");
        /* priv data allocated is freed when a TR is sent or
           when a End Proactive Session is received */
        gstk_free(wms_msg_ptr);
        wms_msg_ptr = NULL;
        return ESTK_ERROR;
      }
    }
  }
  }
  wms_msg_ptr->msg_hdr.tag               = WMS_TAG_MO_NOT_SENT;
  wms_msg_ptr->msg_hdr.mem_store         = WMS_MEMORY_STORE_NONE;
  wms_msg_ptr->msg_hdr.index             = 0;
#if defined(FEATURE_WCDMA) || defined(FEATURE_GSM) || defined(FEATURE_LTE)
  /* system mode expected to be GSM WCDMA here */
  if( send_sms_req_ptr->is_cdma_sms == FALSE )
  {
    /*building wms structure for a UMTS SMS*/
    wms_msg_ptr->msg_hdr.message_mode      = WMS_MESSAGE_MODE_GW;
    wms_msg_ptr->u.gw_message.is_broadcast = FALSE;

    wms_msg_ptr->u.gw_message.sc_address.digit_mode  = WMS_DIGIT_MODE_4_BIT;
    wms_msg_ptr->u.gw_message.sc_address.number_type =
        (wms_number_type_e_type)send_sms_req_ptr->address.TON;
    wms_msg_ptr->u.gw_message.sc_address.number_plan = WMS_NUMBER_PLAN_TELEPHONY;

    wms_msg_ptr->u.gw_message.sc_address.number_of_digits =
                     send_sms_req_ptr->address.length;
    if (wms_msg_ptr->u.gw_message.sc_address.number_of_digits > 0)
    {
      (void) (void)memscpy(wms_msg_ptr->u.gw_message.sc_address.digits,
      	            sizeof(wms_msg_ptr->u.gw_message.sc_address.digits),
                    send_sms_req_ptr->address.address,
                    wms_msg_ptr->u.gw_message.sc_address.number_of_digits);
    }

    wms_msg_ptr->u.gw_message.raw_ts_data.format    = WMS_FORMAT_GW_PP;
    wms_msg_ptr->u.gw_message.raw_ts_data.tpdu_type = WMS_TPDU_SUBMIT;
    wms_msg_ptr->u.gw_message.raw_ts_data.len       = send_sms_req_ptr->sms_tpdu.length;

    UIM_MSG_HIGH_3("building wms structure for a UMTS SMS format = 0x%x, tpdu_type = 0x%x, len = 0x%x",
                   wms_msg_ptr->u.gw_message.raw_ts_data.format,
                   wms_msg_ptr->u.gw_message.raw_ts_data.tpdu_type,
                   wms_msg_ptr->u.gw_message.raw_ts_data.len);

    if (send_sms_req_ptr->sms_tpdu.length > 0)
    {
      (void)memscpy(wms_msg_ptr->u.gw_message.raw_ts_data.data,
                    sizeof(wms_msg_ptr->u.gw_message.raw_ts_data.data),
                    send_sms_req_ptr->sms_tpdu.tpdu,
                    send_sms_req_ptr->sms_tpdu.length);
    }
    else
    {
      UIM_MSG_ERR_0("tpdu len is 0!");
      gstk_free(wms_msg_ptr);
      wms_msg_ptr = NULL;
      return ESTK_ERROR;
    }

    wms_msg_ptr->u.gw_message.raw_ts_data.format    = WMS_FORMAT_GW_PP;
    wms_msg_ptr->u.gw_message.raw_ts_data.tpdu_type = WMS_TPDU_SUBMIT;

    if(send_sms_req_ptr->packing_required)
    {
      /* decode and pack the user data */
      /* find the DCS byte */
      dcs_pos += estk_ts_decode_address(&(wms_msg_ptr->u.gw_message.raw_ts_data.data[dcs_pos]),
                                        &dummy_address);
      dcs_pos++;
      if(dcs_pos < WMS_MAX_LEN && dcs_pos < send_sms_req_ptr->sms_tpdu.length)
      {
        UIM_MSG_HIGH_1("Packing Required, dcs = 0x%x",
                       wms_msg_ptr->u.gw_message.raw_ts_data.data[dcs_pos]);
        if(wms_msg_ptr->u.gw_message.raw_ts_data.data[dcs_pos] !=  0x08 ) /* i.e not UCS2_DCS */
        {
          /* check if DCS is 8-bit data */
          if (wms_msg_ptr->u.gw_message.raw_ts_data.data[dcs_pos] & 0x04)
          {
            /* reset DCS 8-bit data bit - to be 7-bit default now */
            wms_msg_ptr->u.gw_message.raw_ts_data.data[dcs_pos] &= 0xFB;
          }

          if (WMS_OK_S != wms_ts_pack_gw_user_data(
                            &wms_msg_ptr->u.gw_message.raw_ts_data))
          {
            UIM_MSG_ERR_0("Packing Message failed");
            gstk_free(wms_msg_ptr);
            wms_msg_ptr = NULL;
            return ESTK_ERROR;
          }
        }
      }
      else
      {
        UIM_MSG_ERR_1("Packing failed, dcs_pos is 0x%x", dcs_pos);
        gstk_free(wms_msg_ptr);
        wms_msg_ptr = NULL;
        return ESTK_ERROR;
      }
    }
      if (WMS_OK_S == (wms_status = wms_ts_submit_msg_is_concatenated(wms_msg_ptr, &is_concatenated)))
      {
        if (TRUE == is_concatenated)
        {
          if (estk_curr_inst_ptr->sms_link_ctrl_timer == ESTK_SMS_LINK_CTRL_TIMER_MAX)
          {
            /* Read the toolkit SMS link control timer */
            result = gstk_nv_access_read(GSTK_NV_SLOT_SMS_LINK_CTRL_TIMER,
                                              estk_curr_inst_ptr->slot_id,
                                              &nv_data);
            if (result == GSTK_SUCCESS)
            {
            estk_curr_inst_ptr->sms_link_ctrl_timer = timer_val = nv_data.sms_link_ctrl_timer;
            }
            else
            {
              UIM_MSG_ERR_0("Unable to get the link ctrl timer value from efs");
              /* use the default as 5s for concatended SMSs*/
            timer_val = (uint8)ESTK_DEFAULT_SMS_LINK_CTRL_TIMER;
            }
          } /* If link control timer value is not set yet */
        else
        {
          timer_val = estk_curr_inst_ptr->sms_link_ctrl_timer;
        }
        UIM_MSG_HIGH_1("ESTK SMS link ctrl timer value: 0x%x", timer_val);
          /* set the link control timer */
          if (WMS_OK_S != (wms_status = wms_cfg_ms_set_link_control(
                                          estk_shared_info.wms_client_id,
                                          as_id,
                                          (wms_cmd_cb_type)estk_wms_cmd_cb,
                                          (const void*)estk_curr_inst_ptr->curr_cmd.hdr_cmd.cmd_detail_reference,
                                          WMS_LINK_CONTROL_ENABLED_ONE,
                                          timer_val)))
          {
            UIM_MSG_ERR_1("ESTK SMS: wms_cfg_ms_set_link_control() failed %d", wms_status);
          }
        } /* if (TRUE == is_concatenated) */
      }
      else
      {
        UIM_MSG_ERR_1("wms_ts_submit_msg_is_concatenated() returned %d", wms_status);
      } /* is concatenated? */
  } /* if !cdma_sms */
#endif /*F_WCDMA || GSM || LTE*/

  if( send_sms_req_ptr->is_cdma_sms == TRUE )
  {
#ifdef FEATURE_CDMA
    UIM_MSG_HIGH_0("building wms structure for a CDMA SMS ");
    wms_msg_ptr->msg_hdr.message_mode = WMS_MESSAGE_MODE_CDMA;
    if(wms_ts_cdma_OTA2cl(send_sms_req_ptr->sms_tpdu.tpdu,
                          (uint16)send_sms_req_ptr->sms_tpdu.length,
                          wms_msg_ptr) != WMS_OK_S)
    {
      UIM_MSG_ERR_0("Conversion from OTA to cl failed");
      gstk_free(wms_msg_ptr);
      wms_msg_ptr = NULL;
      return ESTK_ERROR;
    }
    else if(send_sms_req_ptr->packing_required)
    {
      /* OTA conversion to WMS format ok, pack if necessary */
      if(wms_ts_pack_cdma_user_data(
           &wms_msg_ptr->u.cdma_message.raw_ts) != WMS_OK_S)
      {
        UIM_MSG_ERR_0("Packing Message failed");
        gstk_free(wms_msg_ptr);
        wms_msg_ptr = NULL;
        return ESTK_ERROR;
      }
    }
#endif /* FEATURE_CDMA */
  } /* if cdma_sms */

  UIM_MSG_HIGH_3("gw ready: 0x%x, cdma ready: 0x%x, wms ptr: 0x%x",
                  estk_curr_inst_ptr->wms_msg_cfg.wms_ready_gw,
                  estk_curr_inst_ptr->wms_msg_cfg.wms_ready_cdma,
                  wms_msg_ptr);

  estk_curr_inst_ptr->wms_msg_cfg.wms_msg_ptr = wms_msg_ptr;

  /* If returned TRUE means TR is already sent */
  if(estk_sms_validate_transport_type(wms_msg_ptr, &transport_type) == TRUE)
  {
    return ESTK_SUCCESS;
  }

  /* used to read is_cp_ack_pending flag */
  (void)gstk_io_ctrl(GSTK_IO_GET_WMS_IS_CP_ACK_PENDING_FLAG,
                     &as_id, &is_cp_ack_pending);
  
  /* check if CP_ACK is not pending, only then send SMS */
  if(is_cp_ack_pending == FALSE)
  {
    wms_status = wms_msg_ms_send_ext(
                      estk_shared_info.wms_client_id,
                      as_id,
                      (wms_cmd_cb_type)estk_wms_cmd_cb,
                      (const void*)estk_curr_inst_ptr->curr_cmd.hdr_cmd.cmd_detail_reference,
                      WMS_SEND_MODE_CLIENT_MESSAGE,
                      wms_msg_ptr,
                      transport_type);
  
    if (wms_status != WMS_OK_S)
    {
      UIM_MSG_ERR_1("wms_msg_send failed 0x%x", wms_status);
      return ESTK_ERROR;
    }
  }

  return ESTK_SUCCESS;
} /* estk_process_send_sms_req */

/*===========================================================================
FUNCTION: estk_sms_validate_transport_type

DESCRIPTION:
  Validates the transport type before sending the SMS.

PARAMETERS:
  wms_msg_ptr    :  [Input]  WMS SMS Message pointer
  transport_type :  [Output] Transport type 

DEPENDENCIES:
  None

RETURN VALUE:
  boolean
    TRUE  : TR sent to Card, not further processing required
    FALSE : SMS can be processsed further

COMMENTS:
  None

SIDE EFFECTS:
  None

===========================================================================*/
static boolean estk_sms_validate_transport_type(
  wms_client_message_s_type     *wms_msg_ptr,
  wms_msg_transport_type_e_type *transport_type_ptr
)
{
  gstk_location_status_enum_type loc_status       = GSTK_NORMAL_SERVICE;
  gstk_additional_info_ptr_type *addi_info_ptr    = NULL;
  gstk_additional_info_ptr_type  addi_info        = {0x00};
  uint8                          addi_result      = 0x00;
  gstk_slot_id_enum_type         slot_id          = GSTK_SLOT_ID_MAX;
  uint8                          i                = 0;

  rex_enter_crit_sect(&estk_shared_info.estk_crit_sect);

  for(i = SYS_MODEM_AS_ID_1; i < GSTK_MAX_NUM_OF_INSTANCES; i++)
  {
    if (gstk_io_ctrl(
                   GSTK_IO_MODEM_AS_ID_TO_SLOT_ID,
                   &estk_shared_info.wms_info[i].as_id,
           &slot_id) != GSTK_SUCCESS)
    {
      continue;
    }
    
    if (!ESTK_IS_VALID_SLOT_ID(slot_id))
    {
      continue;
    }
    
    /* Check if the transport type been changed */
    if((estk_shared_info.wms_info[i].trans_changed == TRUE) &&
       /* Match the Slot Id */
       (slot_id == estk_curr_inst_ptr->slot_id))
    {
      if ((estk_shared_info.wms_info[i].wms_transport_type.trans_type ==
                      WMS_MSG_TRANSPORT_TYPE_SMS_OVER_IMS) &&
          (estk_shared_info.wms_info[i].wms_transport_type.reg_status == TRUE)&&
          (estk_shared_info.wms_info[i].transport_reg_ims_status == WMS_MSG_TRANSPORT_NW_REG_STATUS_FULL_SRV))
      {
        estk_curr_inst_ptr->trans_type = 
  		      WMS_MSG_TRANSPORT_TYPE_SMS_OVER_IMS;
      }
      else
      {
        estk_curr_inst_ptr->trans_type = WMS_MSG_TRANSPORT_TYPE_CS;
      }
      estk_shared_info.wms_info[i].trans_changed = FALSE;
      break;
    }
  }
  
  rex_leave_crit_sect(&estk_shared_info.estk_crit_sect);
  
  UIM_MSG_HIGH_3("Trans_type: 0x%x slot owns wms client : 0x%x is_3gpp2_sms : 0x%x",
                  estk_curr_inst_ptr->trans_type,
                  estk_shared_info.slot_owns_wms_cli,
				  estk_curr_inst_ptr->is_3gpp2_sms); 
  
  /* First try on IMS if it is available  */
  if (estk_curr_inst_ptr->trans_type ==
            WMS_MSG_TRANSPORT_TYPE_SMS_OVER_IMS) 
  {
    *transport_type_ptr = WMS_MSG_TRANSPORT_TYPE_SMS_OVER_IMS;
  }  
  else if((wms_msg_ptr->msg_hdr.message_mode == WMS_MESSAGE_MODE_GW &&
            estk_curr_inst_ptr->wms_msg_cfg.wms_ready_gw) ||
          (wms_msg_ptr->msg_hdr.message_mode == WMS_MESSAGE_MODE_CDMA &&
            estk_curr_inst_ptr->wms_msg_cfg.wms_ready_cdma))
  {
    *transport_type_ptr = WMS_MSG_TRANSPORT_TYPE_CS;
  }
  
  /* If UICC triggers 3GPP2-SMS and transport type is IMS, block 3GPP2-SMS and
   * send error TR to card*/
  if ((*transport_type_ptr == WMS_MSG_TRANSPORT_TYPE_SMS_OVER_IMS) &&
      (estk_curr_inst_ptr->is_3gpp2_sms == TRUE) &&
      (gstk_nv_get_feature_2_status(
         GSTK_CFG_FEATURE_2_SLOT_BLOCK_3GPP2_SMS_OVER_IMS,
         estk_shared_info.slot_owns_wms_cli) == TRUE))
  {
    UIM_MSG_HIGH_0(" Sending error TR as 3gpp2-sms is rejected. ");
    (void)estk_send_terminal_response(estk_curr_inst_ptr->curr_cmd.hdr_cmd.cmd_detail_reference,
                                      GSTK_SEND_SMS_CNF,
                                      GSTK_ME_CURRENTLY_UNABLE_TO_PROCESS_COMMAND,
                                      NULL,
                                      NULL);
    return TRUE;
  }

  /* While trying SMS on CS Check for No service and send
   appropriate TR with No Service as additional result */
  if (*transport_type_ptr == WMS_MSG_TRANSPORT_TYPE_CS)
  {
     (void) gstk_io_ctrl(GSTK_IO_GET_CURR_LOC_STATUS,
                     (void *)&estk_shared_info.slot_owns_wms_cli,
                     (void *)&loc_status);
  
     if(loc_status == GSTK_NO_SERVICE)
     {
       addi_info.length = 1;
       addi_result = GSTK_NO_SERVICE_AVAILABLE;
       addi_info.additional_info_ptr = &addi_result;
       addi_info_ptr = &addi_info;
       (void)estk_send_terminal_response(
                                 estk_curr_inst_ptr->curr_cmd.hdr_cmd.cmd_detail_reference,                          
                                 GSTK_SEND_SMS_CNF,
                                 GSTK_ME_CURRENTLY_UNABLE_TO_PROCESS_COMMAND,
                                 addi_info_ptr,
                                 NULL);
       return TRUE;
     }
  }
  
  /* Store this as the WMS will not update the transport type in case of failure,
    this helps for retrying on CS */
  estk_curr_inst_ptr->last_trans_type = *transport_type_ptr;
  UIM_MSG_HIGH_1("SMS Sent on transport 0x%x", estk_curr_inst_ptr->last_trans_type);
  return FALSE;
} /* estk_sms_validate_transport_type */
#endif /* FEATURE_ESTK */
