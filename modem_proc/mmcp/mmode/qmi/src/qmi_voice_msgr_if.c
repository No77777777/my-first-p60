/*===========================================================================

                         D S _ Q M I _ V O I C E _MSGR_IF . C

DESCRIPTION

 The Data Services Qualcomm Voice service Messenger interface source file.

EXTERNALIZED FUNCTIONS

Copyright (c) 2004-2015 Qualcomm Technologies Incorporated.
All Rights Reserved.
Qualcomm Confidential and Proprietary.
===========================================================================*/
/*===========================================================================
                      EDIT HISTORY FOR FILE

  $Header: //components/rel/mmcp.mpss/7.9.0/mmode/qmi/src/qmi_voice_msgr_if.c#2 $ $DateTime: 2022/11/09 00:17:53 $ $Author: pwbldsvc $

when        who    what, where, why
--------    ---    ----------------------------------------------------------
05/2313         Created Module.
===========================================================================*/

/*===========================================================================

  INCLUDE FILES FOR MODULE

===========================================================================*/


#include "cm.h"
#include "qmi_voice_msgr_if.h"
#include "qmi_mmode_msgr_msg.h"
#include "qmi_voice_call_list.h"
#include "qmi_voice_cm_util.h"
#include "qmi_voice_cmd_list.h"
#include "modem_mem.h"
#include "qm_util.h"

#ifdef FEATURE_DUAL_SIM
#include  "cm_dualsim.h"
#endif /*FEATURE_DUAL_SIM*/

/*===========================================================================

                            CONSTANT DEFINITIONS

===========================================================================*/

/*===========================================================================

                                DATA TYPES

===========================================================================*/


/*===========================================================================

                               INTERNAL DATA

===========================================================================*/


/*===========================================================================

                    EXTERNAL FUNCTION PROTOTYPES

===========================================================================*/
/*===========================================================================
  FUNCTION qmi_voice_ims_extn_handle_srvcc_handover_complete()

  DESCRIPTION
    Handle the SRVCC complete for other IMS cases

  PARAMETERS
    call_info_ptr:  cm_mm_call_info_s_type

  RETURN VALUE
    None

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
boolean qmi_voice_ims_extn_handle_srvcc_handover_complete
(
  cm_mid_srvcc_ho_comp_list_s_type *cm_data,
  uint8 *as_id
);

/*===========================================================================
  FUNCTION qmi_voice_msgr_conf_participant_status_ind()

  DESCRIPTION
    Send the status of add participant operation

  PARAMETERS

  RETURN VALUE
    None

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static void qmi_voice_msgr_conf_participant_status_ind
(
  mmode_qmi_voice_participant_status_cmd_msg_type *participant_status_info
);

/*===========================================================================
  FUNCTION qmi_voice_msgr_get_wwan_911_timer_resp()

  DESCRIPTION
    Send tty info indication based on the call event

  PARAMETERS

  RETURN VALUE
    None

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static void qmi_voice_msgr_get_wwan_911_timer_resp
(
  cm_mm_get_t_wwan_911_rsp_s_type *cm_data,
  sys_modem_as_id_e_type asubs_id
);


/*===========================================================================

                    INTERNAL FUNCTION PROTOTYPES

===========================================================================*/

static void qmi_voice_msgr_parsed_conf_info_ind
(
  mmode_qmi_voice_conf_participants_info_cmd_msg_type *conf_cmd_info
);

static void qmi_voice_msgr_srvcc_ho_complete
(
  cm_mid_srvcc_ho_comp_list_s_type *cm_data
);

static void qmi_voice_msgr_drvcc_ho_complete
(
  cm_mid_srvcc_ho_comp_list_s_type *cm_data
);

static void qmi_voice_msgr_tty_info_ind
(
  mmode_qmi_voice_tty_mode_info_cmd_msg_type *tty_cmd_info
);

static void qmi_voice_msgr_audio_rat_change_info_ind
(
  mmode_qmi_voice_audio_session_rat_change_cmd_msg_type *audio_rat_change_cmd_info
);


static void qmi_voice_msgr_auto_reject_incoming_call_end_ind
(
  cm_mt_call_end_auto_reject_modem_ind_s_type *cm_data
);


/*===========================================================================

                        EXTERNAL FUNCTION DEFINITIONS

===========================================================================*/

/*========================================================================
  FUNCTION qmi_voice_msgr_handler

  DESCRIPTION
    process messages received from msgr

  PARAMETERS
    msg pointer in *void. cast to qmi_mmodei_msgr_type before use.
    *void type is to avoid circular reference of headers

  RETURN VALUE
    None
===========================================================================*/
void qmi_voice_msgr_handler( void *param )
{

  qmi_mmodei_msgr_type       *msg = NULL;
  sys_modem_as_id_e_type asubs_id = SYS_MODEM_AS_ID_NONE;
  QM_ASSERT( param );

  msg = (qmi_mmodei_msgr_type *)param;
  QM_MSG_MED_1("qmi_voice_msgr_handler cmd id = %d", msg->cmd.hdr.id);

  asubs_id = qmi_voice_cm_util_get_variant(&msg->cmd.hdr);

  switch ( msg->cmd.hdr.id )
  {
    case QMI_VOICE_CONF_PARTICIPANTS_INFO_CMD:
      qmi_voice_msgr_parsed_conf_info_ind( &msg->cmd.qmi_voice.parsed_conf_cmd);
      break;

    case MM_CM_SRVCC_HO_COMPLETE_IND:
      qmi_voice_msgr_srvcc_ho_complete( &msg->cmd.qmi_voice.mid_call_srvcc_handover_ind);
      break;

    case MM_CM_DRVCC_HO_COMPLETE_IND:
      QM_MSG_HIGH("DRVCC_HO_COMPLETE");
      qmi_voice_msgr_drvcc_ho_complete( &msg->cmd.qmi_voice.mid_call_srvcc_handover_ind);
      break;

    case QMI_VOICE_TTY_MODE_INFO_CMD:
      qmi_voice_msgr_tty_info_ind( &msg->cmd.qmi_voice.tty_info_cmd);
      break;

    case QMI_VOICE_AUDIO_RAT_CHANGE_INFO_CMD:
      qmi_voice_msgr_audio_rat_change_info_ind( &msg->cmd.qmi_voice.audio_rat_change_info_cmd);
      break;

    case QMI_VOICE_CONF_PARTICIPANT_STATUS_INFO_CMD:
      qmi_voice_msgr_conf_participant_status_ind( &msg->cmd.qmi_voice.participant_status_info_cmd);
      break;

    case QMI_VOICE_ECALL_STATUS_IND:
      qmi_voice_msgr_ecall_status_ind( &msg->cmd.qmi_voice.ecall_status_ind );
      break;

    case MM_CM_GET_T_WWAN_911_RSP:
      qmi_voice_msgr_get_wwan_911_timer_resp( &msg->cmd.qmi_voice.get_wwan_911_timer_resp, asubs_id);
      break;

    case QMI_VOICE_VICE_DIALOG_IND:
      qmi_voice_msgr_vice_dialog_info_ind( &msg->cmd.qmi_voice.vice_dialog_info_cmd, asubs_id);
      break;

    case MM_CM_MT_CALL_END_AUTO_REJECT_IND: 
      qmi_voice_msgr_auto_reject_incoming_call_end_ind( 
          &msg->cmd.qmi_voice.mt_call_end_auto_reject_modem_ind);
      break;

    default:
      /*not a QMI_VOICE handled msg */
      break;
  }
}

/*========================================================================
  FUNCTION qmi_voice_msgr_register

  DESCRIPTION
    register to msgr messages

  PARAMETERS
    msgr client object pointer

  RETURN VALUE
    None
===========================================================================*/
void qmi_voice_msgr_register( msgr_client_t *msgr_client )
{
  errno_enum_type err = E_FAILURE;
  enum qmi_voice_subs_e sub = QMI_VOICE_SUBS_NONE;

  for(sub = QMI_VOICE_SUBS_PRIMARY; sub < QMI_VOICE_SUBS_MAX; sub++)
  {
    if((err = msgr_register_variant( MSGR_QMI_VOICE, msgr_client, MSGR_ID_REX, QMI_VOICE_CONF_PARTICIPANTS_INFO_CMD, sub )) != E_SUCCESS)
    {
      QM_MSG_HIGH_1("QMI_VOICE_CONF_PARTICIPANTS_INFO_CMD register ret %d", err);
    }
    
    if((err = msgr_register_variant( MSGR_QMI_VOICE, msgr_client, MSGR_ID_REX, MM_CM_SRVCC_HO_COMPLETE_IND, sub )) != E_SUCCESS)
    {
      QM_MSG_HIGH_1("SRVCC_HO_COMPLETE register ret %d", err);
    }
    
    if((err = msgr_register_variant( MSGR_QMI_VOICE, msgr_client, MSGR_ID_REX, QMI_VOICE_TTY_MODE_INFO_CMD, sub )) != E_SUCCESS)
    {
      QM_MSG_HIGH_1("QMI_VOICE_TTY_MODE_INFO_CMD register ret %d", err);
    }
    
    if((err = msgr_register_variant( MSGR_QMI_VOICE, msgr_client, MSGR_ID_REX, QMI_VOICE_AUDIO_RAT_CHANGE_INFO_CMD, sub )) != E_SUCCESS)
    {
      QM_MSG_HIGH_1("QMI_VOICE_AUDIO_RAT_CHANGE_INFO_CMD register ret %d", err);
    }
    
    if((err = msgr_register_variant( MSGR_QMI_VOICE, msgr_client, MSGR_ID_REX, QMI_VOICE_CONF_PARTICIPANT_STATUS_INFO_CMD, sub )) != E_SUCCESS)
    {
      QM_MSG_HIGH_1("QMI_VOICE_CONF_PARTICIPANT_STATUS_INFO_CMD register ret %d", err);
    }
    
    if((err = msgr_register_variant( MSGR_QMI_VOICE, msgr_client, MSGR_ID_REX, QMI_VOICE_ECALL_STATUS_IND, sub )) != E_SUCCESS)
    {
      QM_MSG_HIGH_1("QMI_VOICE_ECALL_STATUS_IND register ret %d", err);
    }
    
    if((err = msgr_register_variant( MSGR_QMI_VOICE, msgr_client, MSGR_ID_REX, MM_CM_GET_T_WWAN_911_RSP, sub)) != E_SUCCESS)
    {
      QM_MSG_HIGH_1("GET_T_WWAN_911_RSP register ret %d", err);
    }
    
    if((err = msgr_register_variant( MSGR_QMI_VOICE, msgr_client, MSGR_ID_REX, QMI_VOICE_VICE_DIALOG_IND, sub)) != E_SUCCESS)
    {
      QM_MSG_HIGH_1("VICE Dialog info %d", err);
    }

    if((err = msgr_register_variant( MSGR_QMI_VOICE, msgr_client, MSGR_ID_REX, MM_CM_DRVCC_HO_COMPLETE_IND, sub )) != E_SUCCESS)
    {
      QM_MSG_HIGH_1("DRVCC_HO_COMPLETE register ret %d", err);
    }

    if((err = msgr_register_variant( MSGR_QMI_VOICE, msgr_client, MSGR_ID_REX, MM_CM_MT_CALL_END_AUTO_REJECT_IND, sub )) != E_SUCCESS)
    {
      QM_MSG_HIGH_1("MT_CALL_END_AUTO_REJECT ret %d", err);
    }


  }

  QM_ASSERT( err == E_SUCCESS );
}

/*========================================================================
  FUNCTION qmi_voice_msgr_send_srvcc_config_req

  DESCRIPTION
    to send the srvcc call context info request

  PARAMETERS
    Params ptr which has all the other call context data received from clients

  RETURN VALUE
    IxErrnoType : Success or Failure of operation
===========================================================================*/
IxErrnoType qmi_voice_msgr_send_srvcc_config_req
(
  qmi_voice_cm_if_set_srvcc_call_context_s *const in_data_ptr,
  qmi_voice_cm_if_as_id_e_type as_id
)
{
  IxErrnoType result = E_SUCCESS;
  cm_srvcc_call_context_rsp_s_type *srvcc_context_info_ptr = NULL;
  uint8 i=0, cm_conf_idx=0, cm_participant_idx=0;
  boolean is_cm_conf_info_filled=FALSE;
  cm_call_substate_type call_sub_state;
  cm_mid_srvcc_participant_info  *conf_participant = NULL;

  srvcc_context_info_ptr = (cm_srvcc_call_context_rsp_s_type *)QM_MEM_ALLOC(sizeof(cm_srvcc_call_context_rsp_s_type));
  if ( NULL == srvcc_context_info_ptr )
  {
    return E_NO_MEMORY;
  }

  memset(srvcc_context_info_ptr,0,sizeof(cm_srvcc_call_context_rsp_s_type));

  srvcc_context_info_ptr->num_of_calls = 0;

  /* copy call contexts from qmi_voice to cm structure*/
  if(in_data_ptr->num_calls > 0)
  {
    for(i=0; i<in_data_ptr->num_calls; i++)
    {
      if(in_data_ptr->srvcc_calls[i].is_mpty_call != 1)
      {
        cm_mid_srvcc_ip_call_info  *ip_call = &srvcc_context_info_ptr->call_context[srvcc_context_info_ptr->num_of_calls].call_info.ip_call;

        /*copy IP call info */
        srvcc_context_info_ptr->call_context[srvcc_context_info_ptr->num_of_calls].is_conf_call = FALSE;
        ip_call->call_id = in_data_ptr->srvcc_calls[i].instance_id;
        ip_call->call_type = qmi_voice_cm_util_map_qmi_to_cm_call_type((qmi_voice_cm_if_call_type_e_type)in_data_ptr->srvcc_calls[i].call_type);
        qmi_voice_util_map_qmi_to_cm_call_state_substate(&ip_call->call_state,
                                                         &ip_call->call_sub_state,
                                                         (qmi_voice_cm_if_call_state_e_type)in_data_ptr->srvcc_calls[i].call_state,
                                                         (qmi_voice_cm_if_call_substate_e_type)in_data_ptr->srvcc_calls[i].call_substate);
        ip_call->call_direction = in_data_ptr->srvcc_calls[i].direction;

        if(in_data_ptr->srvcc_calls[i].is_caller_name_type_valid)
        {
          ip_call->srvcc_caller_info.caller_name_pi = in_data_ptr->srvcc_calls[i].name_pi;
          ip_call->srvcc_caller_info.caller_len     = in_data_ptr->srvcc_calls[i].name_len;

          /*Allocating memory for Caller Name, which will be freed by CM once they copy it to their call list*/
          ip_call->srvcc_caller_info.caller_name = QM_MEM_ALLOC(2* in_data_ptr->srvcc_calls[i].name_len);
          if(NULL == ip_call->srvcc_caller_info.caller_name)
          {
            if(i > 0)
            {
              int j = i - 1;
              for(;  j>=0; j--)
              {
                /*Freeing the already allocated memory blocks*/
                cm_mid_srvcc_ip_call_info  *ip_call = &srvcc_context_info_ptr->call_context[j].call_info.ip_call;
                QM_MEM_FREE(ip_call->srvcc_caller_info.caller_name);
              }
            }
            QM_MEM_FREE(srvcc_context_info_ptr);
            return E_NO_MEMORY;
          }
          QM_MSG_HIGH_2("in_data_ptr->srvcc_calls[%d].name_len = %d", i, in_data_ptr->srvcc_calls[i].name_len);
          memscpy(ip_call->srvcc_caller_info.caller_name,
                  in_data_ptr->srvcc_calls[i].name_len*2,
                  in_data_ptr->srvcc_calls[i].caller_name,
                  in_data_ptr->srvcc_calls[i].name_len*2);
        }

        if(in_data_ptr->srvcc_calls[i].is_alerting_type_valid)
        {
          ip_call->call_alert_media_type = qmi_voice_cm_util_map_qmi_to_cm_alerting_type(in_data_ptr->srvcc_calls[i].alerting_type);
        }
		else
        {
          ip_call->call_alert_media_type = CM_SRVCC_ALERTING_NONE;
        }

        if(in_data_ptr->srvcc_calls[i].is_num_pi_valid)
        {
          ip_call->call_num.pi = in_data_ptr->srvcc_calls[i].num_pi;
        }

        ip_call->call_num.len = in_data_ptr->srvcc_calls[i].num_len;
        memscpy(ip_call->call_num.buf,
                sizeof(ip_call->call_num.buf),
                in_data_ptr->srvcc_calls[i].num,
                in_data_ptr->srvcc_calls[i].num_len);

        srvcc_context_info_ptr->num_of_calls++;
      }
      else
      {
        if(!is_cm_conf_info_filled)
        {
          is_cm_conf_info_filled = TRUE;
          cm_conf_idx = srvcc_context_info_ptr->num_of_calls;
          srvcc_context_info_ptr->num_of_calls++;
          srvcc_context_info_ptr->call_context[cm_conf_idx].call_info.ip_conf_call.num_of_participants =0;

          /* Fill CM overall conference call info*/
          srvcc_context_info_ptr->call_context[cm_conf_idx].is_conf_call = TRUE;
          srvcc_context_info_ptr->call_context[cm_conf_idx].call_info.ip_conf_call.call_id = CM_CALL_ID_UNASSIGNED;
          qmi_voice_util_map_qmi_to_cm_call_state_substate(&srvcc_context_info_ptr->call_context[cm_conf_idx].call_info.ip_conf_call.conf_call_state,
                                                           &srvcc_context_info_ptr->call_context[cm_conf_idx].call_info.ip_conf_call.conf_call_substate,
                                                           (qmi_voice_cm_if_call_state_e_type)in_data_ptr->srvcc_calls[i].call_state,
                                                           (qmi_voice_cm_if_call_substate_e_type)in_data_ptr->srvcc_calls[i].call_substate);
        }

        conf_participant = &srvcc_context_info_ptr->call_context[cm_conf_idx].call_info.ip_conf_call.participant_list[cm_participant_idx];

        /* Fill CM conference participant info*/
        conf_participant->participant_id = in_data_ptr->srvcc_calls[i].instance_id;
        conf_participant->call_type = qmi_voice_cm_util_map_qmi_to_cm_call_type((qmi_voice_cm_if_call_type_e_type)in_data_ptr->srvcc_calls[i].call_type);
        qmi_voice_util_map_qmi_to_cm_call_state_substate(&conf_participant->call_state,
                                                         &call_sub_state,
                                                         (qmi_voice_cm_if_call_state_e_type)in_data_ptr->srvcc_calls[i].call_state,
                                                         (qmi_voice_cm_if_call_substate_e_type)in_data_ptr->srvcc_calls[i].call_substate);

        conf_participant->call_direction = in_data_ptr->srvcc_calls[i].direction;
        conf_participant->participant_num.len = in_data_ptr->srvcc_calls[i].num_len;
        memscpy(conf_participant->participant_num.buf,
                sizeof(conf_participant->participant_num.buf),
                in_data_ptr->srvcc_calls[i].num,
                in_data_ptr->srvcc_calls[i].num_len);

        cm_participant_idx++;
        srvcc_context_info_ptr->call_context[cm_conf_idx].call_info.ip_conf_call.num_of_participants++;
      }
    }
    srvcc_context_info_ptr->as_id = as_id;
  }

  msgr_init_hdr_variant( (msgr_hdr_struct_type *)srvcc_context_info_ptr, MSGR_QMI_VOICE, MM_CM_SRVCC_CONTEXT_RSP, as_id );

  result = msgr_send((msgr_hdr_struct_type *)srvcc_context_info_ptr, sizeof(cm_srvcc_call_context_rsp_s_type));

  QM_MSG_HIGH_1("SRVCC call context msgr_send ret=%d", result);

  QM_MEM_FREE(srvcc_context_info_ptr);

  return result;
}

/*========================================================================
  FUNCTION qmi_voice_msgr_send_set_wwan_911_timer_req

  DESCRIPTION
    To send the set WWAN 911 Timer Request to CM.

  PARAMETERS
    Params ptr which has all the other call context data received from clients

  RETURN VALUE
    IxErrnoType : Success or Failure of operation
===========================================================================*/
IxErrnoType qmi_voice_msgr_send_set_wwan_911_timer_req
(
  qmi_voice_cm_if_set_wwan_911_timer_s  *const in_data_ptr,
  qmi_voice_cm_if_as_id_e_type as_id
)
{
  IxErrnoType result = E_SUCCESS;
  cm_mm_set_t_wwan_911_req_s_type  set_wwan_911_timer_info;

  memset(&set_wwan_911_timer_info,0,sizeof(set_wwan_911_timer_info));
  set_wwan_911_timer_info.t_wwan_911_val = in_data_ptr->wwan_911_timer_value;

  msgr_init_hdr_variant( (msgr_hdr_struct_type *)&set_wwan_911_timer_info, MSGR_QMI_VOICE,
                         MM_CM_SET_T_WWAN_911_REQ, as_id );

  result = msgr_send((msgr_hdr_struct_type *)&set_wwan_911_timer_info, sizeof(set_wwan_911_timer_info));

  QM_MSG_HIGH_3("SET WWAN 911 TIMER timer value %d, as_id = %d msgr_send ret=%d",
                set_wwan_911_timer_info.t_wwan_911_val, as_id, result);

  return result;
}

/*========================================================================
  FUNCTION qmi_voice_msgr_send_get_wwan_911_timer_req

  DESCRIPTION
    To send the get WWAN 911 Timer Request to CM.

  PARAMETERS
    Params ptr which has all the other call context data received from clients

  RETURN VALUE
    IxErrnoType : Success or Failure of operation
===========================================================================*/
IxErrnoType qmi_voice_msgr_send_get_wwan_911_timer_req
(
  qmi_voice_cm_if_as_id_e_type           as_id
)
{
  IxErrnoType result = E_SUCCESS;
  cm_mm_get_t_wwan_911_req_s_type  get_wwan_911_timer_info;

  memset(&get_wwan_911_timer_info,0,sizeof(get_wwan_911_timer_info));

  msgr_init_hdr_variant( (msgr_hdr_struct_type *)&get_wwan_911_timer_info, MSGR_QMI_VOICE,
                         MM_CM_GET_T_WWAN_911_REQ, as_id );

  result = msgr_send((msgr_hdr_struct_type *)&get_wwan_911_timer_info, sizeof(get_wwan_911_timer_info));

  QM_MSG_HIGH_2("GET WWAN 911 TIMER as_id = %d, msgr_send ret=%d", as_id, result);

  return result;
}


/*===========================================================================
  FUNCTION qmi_voice_msgr_parsed_conf_info_ind()

  DESCRIPTION
    Send handover indication based on the call event

  PARAMETERS

  RETURN VALUE
    None

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static void qmi_voice_msgr_parsed_conf_info_ind
(
  mmode_qmi_voice_conf_participants_info_cmd_msg_type *conf_cmd_info
)
{
  voice_conf_participants_info_ind_msg_v02 *ind = NULL;
  uint8 i=0,j=0;
  mmode_qmi_conference_call_info_s *conf_info = NULL;
  uint8 as_id = QMI_VOICE_CM_IF_AS_ID_PRIMARY;

  if(conf_cmd_info == NULL)
  {
    QM_MSG_ERROR("qmi_voice_msgr_parsed_conf_info_ind: conf_cmd_info field is NULL");
    return;
  }

  // Allocate memory to the qmi voice indication structure
  ind = QM_MEM_ALLOC(sizeof(*ind));
  if( NULL == ind )
  {
    return;
  }

  conf_info = &(conf_cmd_info->conf_call_info);

  //Inside qmi_voice_msgr_parsed_conf_info_ind
  ind->conf_call_info.update_type =  (voice_update_type_enum_v02) conf_info->type;
  ind->conf_call_info.conf_participant_info_len = conf_info->conference_call_count;
  QM_MSG_MED_2("msgr_parsed_conf_info_ind(): Update type = %d, call_count=%d", ind->conf_call_info.update_type, ind->conf_call_info.conf_participant_info_len);

  for(i=0; i<MIN(conf_info->conference_call_count,MMODE_QMI_NUM_CONF_PARTICIPANTS_MAX); i++)
  {
    mmode_qmi_conference_participant_info  *ims_part = &conf_info->call_info[i];
    voice_conf_participant_call_info_type_v02   *qmi_part = &(ind->conf_call_info.conf_participant_info[i]);


    qmi_part->user_uri_len = ims_part->user_uri_len;
    memscpy(qmi_part->user_uri, sizeof(qmi_part->user_uri), ims_part->user_uri, ims_part->user_uri_len*2);
    //Printing USR_URI received
    for(j=0; j<ims_part->user_uri_len; j++)
    {
      QM_MSG_HIGH_2("USR_URI: char [%d] is 0x%x", j, ims_part->user_uri[j]);
    }

    qmi_part->audio_attributes = (voice_call_attribute_type_mask_v02)ims_part->audio_attrib;
    qmi_part->video_attributes = (voice_call_attribute_type_mask_v02)ims_part->video_attrib;
    qmi_part->disconnection_method = (voice_conf_part_disconnection_method_enum_v02)ims_part->disconnection_method;
    qmi_part->status = (voice_conf_part_call_status_enum_v02)ims_part->status;

    QM_MSG_MED_7("Call %d, uri_len= %d, disc_len= %d, Audio = %d, Video=%d, Disc Method = %d, Status=%d", i, ims_part->user_uri_len,
                 ims_part->disconnection_info_len, qmi_part->audio_attributes, qmi_part->video_attributes, qmi_part->disconnection_method,
                 qmi_part->status);

    qmi_part->disconnection_info_len = ims_part->disconnection_info_len;
    memscpy(qmi_part->disconnection_info, sizeof(qmi_part->disconnection_info), ims_part->disconnection_info, ims_part->disconnection_info_len);
    //Printing DISC_INFO received
    for(j=0; j<ims_part->disconnection_info_len; j++)
    {
      QM_MSG_HIGH_2("DISC_INFO: char [%d] is 0x%x", j, ims_part->disconnection_info[j]);
    }
  }

  qmi_voice_cm_if_unsol_response(QMI_VOICE_CM_IF_CMD_CONF_PARTICIPANTS_INFO_IND, (void *) ind, sizeof(*ind),as_id);
  QM_MEM_FREE(ind);
} /* qmi_voice_msgr_parsed_conf_info_ind() */

/*===========================================================================
  FUNCTION QMI_VOICE_MSGR_SRVCC_HO_COMPLETE()

  DESCRIPTION
    Send handover indication based on the call event

  PARAMETERS
    event          : Call event received
    call_info_ptr  : Call info from CM
    as_id          : Subscription id

  RETURN VALUE
    None

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static void qmi_voice_msgr_srvcc_ho_complete
(
  cm_mid_srvcc_ho_comp_list_s_type *cm_data
)
{
  boolean success = TRUE;
  uint8 as_id = (uint8)QMI_VOICE_CM_IF_AS_ID_PRIMARY;
  qmi_voice_call_list_call_ids_list_type incom_list;
  memset(&incom_list, 0, sizeof(incom_list));

  QM_MSG_MED_2("msgr_srvcc_ho_complete() cm_data:%d and MID-SRVCC: Num of CS calls = %d",
               cm_data, cm_data ? cm_data->number_calls: 0);

  if(cm_data == NULL)
  {
    return;
  }

  if(cm_data->is_int_ims)
  {
    success = qmi_voice_call_list_update_srvcc_calls_info(cm_data, &as_id);
  }
  else
  {
    success = qmi_voice_ims_extn_handle_srvcc_handover_complete(cm_data, &as_id);
  }

  if(success)
  {
    // post processing the call list to update incoming to waiting is already active call is present
    qmi_voice_call_list_query_voice_call_ids_list_by_state(QMI_VOICE_CALL_LIST_STATE_INCOMING, &incom_list, as_id);
    QM_MSG_HIGH_1("Number of calls in incoming state: %d", incom_list.num_of_call_ids);

    if(incom_list.num_of_call_ids !=0)
    {
      if(qmi_voice_call_list_count_num_of_voice_calls_by_state((uint16)(QMI_VOICE_CALL_LIST_STATE_ACTIVE | QMI_VOICE_CALL_LIST_STATE_HOLDING), as_id ) > 0 )
      {
        if (qmi_voice_call_list_update_call_state(incom_list.call_id[0], QMI_VOICE_CALL_LIST_STATE_WAITING) != E_SUCCESS)
        {
          QM_MSG_ERROR_1("Fail to update call list entry state for incom:  call_id %d", incom_list.call_id[0]);
        }
      }
    }

    qmi_voice_call_list_report_all_call_status_changed(as_id);
  }

  qmi_voice_call_list_reset_srvcc_available_flags();
  //The is_srvcc flag is only to be sent in the first call indication following the SRVCC.

  qmi_voice_call_list_reset_srvcc_caller_name_flags();
  //The SRVCC caller name info should be sent only once when the Handover is complete. It should not be sent after that.

  qmi_voice_call_list_reset_parent_id_valid_flags();
  //The parent_id is only to be sent in the first call indication following the SRVCC.
} /* qmi_voice_msgr_srvcc_ho_complete() */

/*===========================================================================
  FUNCTION QMI_VOICE_MSGR_DRVCC_HO_COMPLETE()

  DESCRIPTION
    Send handover indication based on the call event

  PARAMETERS
    event          : Call event received
    call_info_ptr  : Call info from CM
    as_id          : Subscription id

  RETURN VALUE
    None

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static void qmi_voice_msgr_drvcc_ho_complete
(
  cm_mid_srvcc_ho_comp_list_s_type *cm_data
)
{
  boolean success = TRUE;
  uint8 as_id = (uint8)QMI_VOICE_CM_IF_AS_ID_PRIMARY;

  QM_MSG_MED("Inside qmi_voice_drvcc_handover_complete():");

  if(cm_data == NULL)
  {
    QM_MSG_ERROR("qmi_voice_msgr_drvcc_ho_complete: cm_data field is NULL");
    return;
  }

  QM_MSG_MED_1("DRVCC: Num of CS calls = %d", cm_data->number_calls);

  if(cm_data->is_int_ims)
  {
    success = qmi_voice_call_list_update_drvcc_calls_info(cm_data, &as_id);
  }
  else
  {
    QM_MSG_ERROR("Unsupported is_int_ims = 0 for DRVCC");
  }

  if(success)
  {
    qmi_voice_call_list_report_all_call_status_changed(as_id);
  }

  qmi_voice_call_list_reset_drvcc_available_flags();
  //The is_drvcc flag is only to be sent in the first call indication following the DRVCC.

  qmi_voice_call_list_reset_drvcc_parent_id_valid_flags();
  //The drvcc_parent_id is only to be sent in the first call indication following the DRVCC.
} /* qmi_voice_msgr_drvcc_ho_complete() */

/*===========================================================================
  FUNCTION qmi_voice_msgr_tty_info_ind()

  DESCRIPTION
    Send tty info indication based on the call event

  PARAMETERS

  RETURN VALUE
    None

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static void qmi_voice_msgr_tty_info_ind
(
  mmode_qmi_voice_tty_mode_info_cmd_msg_type *tty_cmd_info
)
{
  voice_tty_ind_msg_v02 tty_ind;
  uint8 as_id = QMI_VOICE_CM_IF_AS_ID_PRIMARY;

  memset(&tty_ind,0,sizeof(tty_ind));

  tty_ind.tty_mode = (tty_mode_enum_v02)tty_cmd_info->tty_mode;

  qmi_voice_cm_if_unsol_response(QMI_VOICE_CM_IF_CMD_TTY_INFO_IND,(void *) &tty_ind, sizeof(tty_ind),as_id);

} /* qmi_voice_msgr_tty_info_ind() */


/*===========================================================================
  FUNCTION qmi_voice_msgr_audio_rat_change_info_ind()

  DESCRIPTION
    Send audio RAT change info indication based on the call event

  PARAMETERS

  RETURN VALUE
    None

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static void qmi_voice_msgr_audio_rat_change_info_ind
(
  mmode_qmi_voice_audio_session_rat_change_cmd_msg_type *audio_rat_change_cmd_info
)
{
  voice_audio_rat_change_info_ind_msg_v02  audio_rat_change_ind;
  uint8 as_id;

  memset(&audio_rat_change_ind,0,sizeof(audio_rat_change_ind));

  as_id                                   = audio_rat_change_cmd_info->audio_rat_change.asid;
  audio_rat_change_ind.audio_session_info_valid = TRUE;
  audio_rat_change_ind.audio_session_info = (audio_session_enum_v02)audio_rat_change_cmd_info->audio_rat_change.audio_session;
  audio_rat_change_ind.rat_info_valid = TRUE;
  audio_rat_change_ind.rat_info = (call_mode_enum_v02)audio_rat_change_cmd_info->audio_rat_change.rat;

  QM_MSG_MED_3("msgr_audio_rat_change_info_ind(): as_id=%d, audio_session_info=%d, rat_info=%d", as_id, audio_rat_change_ind.audio_session_info,
               audio_rat_change_ind.rat_info);

  qmi_voice_cm_if_unsol_response(QMI_VOICE_CM_IF_CMD_AUDIO_RAT_CHANGE_INFO_IND,(void *) &audio_rat_change_ind, sizeof(audio_rat_change_ind),as_id);

} /* qmi_voice_msgr_audio_rat_change_info_ind() */

/*===========================================================================
  FUNCTION qmi_voice_msgr_conf_participant_status_ind()

  DESCRIPTION
    Send the status of add participant operation

  PARAMETERS

  RETURN VALUE
    None

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static void qmi_voice_msgr_conf_participant_status_ind
(
  mmode_qmi_voice_participant_status_cmd_msg_type *participant_status_info
)
{
  voice_conf_participant_status_info_ind_msg_v02  part_status_ind;
  uint8 qmi_call_id;
  uint8 as_id = QMI_VOICE_CM_IF_AS_ID_PRIMARY;

  memset(&part_status_ind,0,sizeof(voice_conf_participant_status_info_ind_msg_v02));

  if(qmi_voice_call_list_get_conn_idx_from_call_id(participant_status_info->participant_status.cm_call_id, &qmi_call_id) == E_SUCCESS)
  {
    part_status_ind.call_id = qmi_call_id;
    part_status_ind.is_qmi_voice_transfer = participant_status_info->participant_status.is_qmi_voice_transfer;
    part_status_ind.op_status.operation = (conf_participant_operation_enum_v02) participant_status_info->participant_status.operation;
    part_status_ind.op_status.sip_status = participant_status_info->participant_status.status_sip_code;
    memscpy(part_status_ind.participant_uri,
            sizeof(part_status_ind.participant_uri),
            participant_status_info->participant_status.participant_uri,
            participant_status_info->participant_status.participant_uri_len);

    part_status_ind.op_status_valid = TRUE;
    part_status_ind.is_qmi_voice_transfer_valid = TRUE;

    /*Get as id of the call */
    qmi_voice_call_list_get_as_id(participant_status_info->participant_status.cm_call_id, &as_id);
  }
  else
  {
    //qmi_voice_msgr_conf_participant_status_ind: Invalid call ID from IMS
    return;
  }

  QM_MSG_MED_3("conf_participant_status_ind(): Call ID =%d, operation=%d, sip_status=%d", part_status_ind.call_id, part_status_ind.op_status.operation, part_status_ind.op_status.sip_status);

  qmi_voice_cm_if_unsol_response(QMI_VOICE_CM_IF_CMD_CONF_PARTICIPANT_STATUS_IND,
                                 (void *) &part_status_ind,
                                 sizeof(part_status_ind),
                                 as_id);

} /* qmi_voice_msgr_conf_participant_status_ind() */

/*===========================================================================
  FUNCTION qmi_voice_msgr_ecall_status_ind()

  DESCRIPTION
    Process the eCall status indication from eCall thru message router.

  PARAMETERS

  RETURN VALUE
    None

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
void qmi_voice_msgr_ecall_status_ind
(
  mmode_qmi_voice_ecall_status_ind_msg_type *ecall_status_info
)
{
  voice_ecall_status_ind_msg_v02 ecall_status_ind;
  uint8 qmi_call_id;
  uint8 as_id = QMI_VOICE_CM_IF_AS_ID_PRIMARY;
  ecall_msd_transmission_status_enum_v02 msd_transmission_status = VOICE_ECALL_MSD_TRANSMISSION_STATUS_FAILURE_V02;
  IxErrnoType result = E_FAILURE;

  memset(&ecall_status_ind,0,sizeof(ecall_status_ind));

  /* Get the conn_idx from CM call_id */
  if(qmi_voice_call_list_get_conn_idx_from_call_id(ecall_status_info->ecall_status.cm_call_id, &qmi_call_id) == E_SUCCESS)
  {
    ecall_status_ind.call_id = qmi_call_id;

    /* Map the MSD Transmission Status */

    QM_MSG_MED_3("ecall_status_ind(): CM call_id = %d, QMI conn_idx %d, msd_transmission_status = %d",
                 ecall_status_info->ecall_status.cm_call_id, ecall_status_ind.call_id,
                 ecall_status_info->ecall_status.ecall_msd_transmission_status);

    switch(ecall_status_info->ecall_status.ecall_msd_transmission_status)
    {
      case MMODE_QMI_VOICE_ECALL_MSD_TRANSMISSION_STATUS_NONE:
        //ecall_msd_trans_status cannot be NONE
        break;

      case MMODE_QMI_VOICE_ECALL_MSD_TRANSMISSION_STATUS_SUCCESS:
        msd_transmission_status = VOICE_ECALL_MSD_TRANSMISSION_STATUS_SUCCESS_V02;
        result = E_SUCCESS;
        break;

      case MMODE_QMI_VOICE_ECALL_MSD_TRANSMISSION_STATUS_FAILURE:
        msd_transmission_status = VOICE_ECALL_MSD_TRANSMISSION_STATUS_FAILURE_V02;
        result = E_SUCCESS;
        break;

      default:
        //Unsupported eCall MSD Transmission Status
        break;
    }

    /* Valid MSD Transmission Status*/
    ecall_status_ind.msd_transmission_status_valid = TRUE;
    ecall_status_ind.msd_transmission_status = msd_transmission_status;

    /*Get as id of the call */
    qmi_voice_call_list_get_as_id(ecall_status_info->ecall_status.cm_call_id, &as_id);
  }
  else
  {
    // Invalid call ID from eCall
    return;
  }


  qmi_voice_cm_if_unsol_response(QMI_VOICE_CM_IF_CMD_ECALL_STATUS_IND,
                                 (void *) &ecall_status_ind,
                                 sizeof(ecall_status_ind),
                                 as_id);

} /* qmi_voice_msgr_ecall_status_ind() */

/*===========================================================================
  FUNCTION qmi_voice_msgr_auto_reject_incoming_call_end_ind()

  DESCRIPTION
    Process the incoming call end auto reject indicatio from CM thru message router.

  PARAMETERS

  RETURN VALUE
    None

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
void qmi_voice_msgr_auto_reject_incoming_call_end_ind
(
  cm_mt_call_end_auto_reject_modem_ind_s_type *cm_data
)
{
  uint32 copy_size = 0;
  auto_rejected_incoming_call_end_ind_msg_v02 mt_call_end_auto_reject_ind;

  memset(&mt_call_end_auto_reject_ind,0,sizeof(mt_call_end_auto_reject_ind));
  
  mt_call_end_auto_reject_ind.sip_error_code          =  cm_data->sip_error_code;

  mt_call_end_auto_reject_ind.call_type 
    = (call_type_enum_v02)qmi_voice_cm_util_map_cm_to_qmi_call_type(
           cm_data->call_type, 
           cm_data->mode_info_type);

  mt_call_end_auto_reject_ind.call_end_reason 
    = (call_end_reason_enum_v02)qmi_voice_cm_util_map_cm_ip_end_cause_to_qmi_end_reason(
           cm_data->end_cause_to_client);
  
#ifdef FEATURE_HA_MAINLINE_ONLY_ENABLE
  mt_call_end_auto_reject_ind.mt_call_verstat_info_valid = TRUE;
  mt_call_end_auto_reject_ind.mt_call_verstat_info = (mt_ims_verstat_enum_v02)cm_data->call_verstat;
#endif
  if(cm_data->num.len > 0)
  {
    mt_call_end_auto_reject_ind.num_valid = TRUE;

     copy_size =   memscpy(mt_call_end_auto_reject_ind.num,
		  sizeof(mt_call_end_auto_reject_ind.num),
		  cm_data->num.buf,
		  cm_data->num.len);

    mt_call_end_auto_reject_ind.num_len =copy_size;

  }

  #if defined(FEATURE_HA_MAINLINE_ONLY_ENABLE)
  /* Update call composer TLVs 
  */
  if (cm_data->call_composer_info.info_included)
  {
    if (cm_data->call_composer_info.importance == CM_CALL_COMP_IMP_NORMAL)
    {
      mt_call_end_auto_reject_ind.call_composer_importance_valid = TRUE;
      mt_call_end_auto_reject_ind.call_composer_importance = CALL_COMP_IMP_NORMAL_V02;
    }
    else if (cm_data->call_composer_info.importance == CM_CALL_COMP_IMP_URGENT)
    {
      mt_call_end_auto_reject_ind.call_composer_importance_valid = TRUE;
      mt_call_end_auto_reject_ind.call_composer_importance = CALL_COMP_IMP_URGENT_V02;
    }

    if (cm_data->call_composer_info.subject.subject_length != 0 &&
       cm_data->call_composer_info.subject.subject_length 
           < QMI_VOICE_CALL_COMP_SUBJECT_MAX_LEN_V02  &&
       cm_data->call_composer_info.subject.subject_text
    )
    {
      mt_call_end_auto_reject_ind.call_composer_subject_valid = TRUE;
      mt_call_end_auto_reject_ind.call_composer_subject_len
        = cm_data->call_composer_info.subject.subject_length;
      memscpy(mt_call_end_auto_reject_ind.call_composer_subject, 
               2*QMI_VOICE_CALL_COMP_SUBJECT_MAX_LEN_V02,
               cm_data->call_composer_info.subject.subject_text, 
               2*cm_data->call_composer_info.subject.subject_length);
    }
    
    if (cm_data->call_composer_info.picture_url.pic_url)
    {
      mt_call_end_auto_reject_ind.call_composer_picture_url_valid = TRUE;

      memscpy(mt_call_end_auto_reject_ind.call_composer_picture_url, 
               QMI_VOICE_CALL_COMP_PIC_URL_MAX_LEN_V02,
               cm_data->call_composer_info.picture_url.pic_url, 
               QMI_VOICE_CALL_COMP_PIC_URL_MAX_LEN_V02);
    }

    if (cm_data->call_composer_info.geo_location.location_included)
    {
      if (cm_data->call_composer_info.geo_location.geo_shape 
             == CM_CALL_COMP_GEO_POINT)
      {
        mt_call_end_auto_reject_ind.call_composer_point_location_valid = TRUE;
        mt_call_end_auto_reject_ind.call_composer_point_location.coordinates.latitude
          = cm_data->call_composer_info.geo_location.geo_info.geo_point.latitude;
        mt_call_end_auto_reject_ind.call_composer_point_location.coordinates.longitude
          = cm_data->call_composer_info.geo_location.geo_info.geo_point.longitude;
      }

      if (cm_data->call_composer_info.geo_location.geo_shape 
             == CM_CALL_COMP_GEO_CIRCLE)
      {
        mt_call_end_auto_reject_ind.call_composer_circle_location_valid = TRUE;
        mt_call_end_auto_reject_ind.call_composer_circle_location.coordinates.latitude
          = cm_data->call_composer_info.geo_location.geo_info.geo_circle.latitude;
        mt_call_end_auto_reject_ind.call_composer_circle_location.coordinates.longitude
          = cm_data->call_composer_info.geo_location.geo_info.geo_circle.longitude;
        mt_call_end_auto_reject_ind.call_composer_circle_location.radius
          = cm_data->call_composer_info.geo_location.geo_info.geo_circle.radius;
      }
    }
  if (cm_data->call_composer_info.org_header.org_header_length != 0 &&
		  cm_data->call_composer_info.org_header.org_header_length 
			 < QMI_VOICE_CALL_COMP_ORG_HDR_MAX_LEN_V02	&&
		  cm_data->call_composer_info.org_header.org_header_text
	  )
	  {
		QM_MSG_HIGH_1("cm_data->org_header_length %d",cm_data->call_composer_info.org_header.org_header_length);
		mt_call_end_auto_reject_ind.call_composer_organization_header_valid = TRUE;
		mt_call_end_auto_reject_ind.call_composer_organization_header_len
		  = cm_data->call_composer_info.org_header.org_header_length;
		memscpy(mt_call_end_auto_reject_ind.call_composer_organization_header, 
				 2*QMI_VOICE_CALL_COMP_ORG_HDR_MAX_LEN_V02,
				 cm_data->call_composer_info.org_header.org_header_text, 
				 2*cm_data->call_composer_info.org_header.org_header_length);
	  }
  }

  #endif
#ifdef FEATURE_HA_MAINLINE_ONLY_ENABLE	 
  QM_MSG_HIGH_6("auto_reject_incoming_call_end_ind, as_id = %d, call_type = %d, end_cause_to_client = %d, sip_error_code = %d num_len %d call_verstat %d",
  	            cm_data->as_id,
  	            mt_call_end_auto_reject_ind.call_type, 
  	            mt_call_end_auto_reject_ind.call_end_reason,
  	            mt_call_end_auto_reject_ind.sip_error_code,
  	            copy_size,
  	            mt_call_end_auto_reject_ind.mt_call_verstat_info);
#else    
  QM_MSG_HIGH_4("qmi_voice_msgr_auto_reject_incoming_call_end_ind, as_id = %d, call_type = %d, end_cause_to_client = %d, sip_error_code = %d",
                cm_data->as_id,
                mt_call_end_auto_reject_ind.call_type, 
                mt_call_end_auto_reject_ind.call_end_reason,
                mt_call_end_auto_reject_ind.sip_error_code);
#endif
  

  qmi_voice_cm_if_unsol_response(QMI_VOICE_CM_IF_CMD_MT_CALL_END_AUTO_REJECT_IND,
                                 (void *) &mt_call_end_auto_reject_ind, 
                                 sizeof(mt_call_end_auto_reject_ind),
                                 cm_data->as_id);

  if (cm_data->call_composer_info.info_included)
  {
    if(cm_data->call_composer_info.picture_url.pic_url)
    {
      QM_MEM_FREE(cm_data->call_composer_info.picture_url.pic_url);
      cm_data->call_composer_info.picture_url.pic_url = NULL;
    }

    if (cm_data->call_composer_info.subject.subject_text)
    {
      QM_MEM_FREE(cm_data->call_composer_info.subject.subject_text);
      cm_data->call_composer_info.picture_url.pic_url = NULL;
    }
    if (cm_data->call_composer_info.org_header.org_header_text)
    {
      QM_MSG_HIGH_1("mem free org_header_text=0x%x", cm_data->call_composer_info.org_header.org_header_text);
      QM_MEM_FREE(cm_data->call_composer_info.org_header.org_header_text);
      cm_data->call_composer_info.org_header.org_header_text= NULL;
    }
 }


} /* qmi_voice_msgr_auto_reject_incoming_call_end_ind */
/*===========================================================================
  FUNCTION qmi_voice_msgr_vice_dialog_info_ind()

  DESCRIPTION
    Process the VICE dialog event from IMS

  PARAMETERS

  RETURN VALUE
    None

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
void qmi_voice_msgr_vice_dialog_info_ind
(
  mmode_qmi_voice_viceDialog_info_ind_msg_type *vice_dialog_info,
  sys_modem_as_id_e_type asubs_id
)
{
  vice_dialog_info_ind_msg_v02 *vice_info_ind;

  uint8 num_indications=0,ind=0;
  uint32 xml_length=0;
  uint8 *xml_ptr=NULL;
  uint8 *ptr_begin = NULL;
  uint32 copy_length=0;
  uint8 as_id = (uint8)asubs_id;

  if(vice_dialog_info== NULL)
  {
    QM_MSG_ERROR("qmi_voice_msgr_vice_dialog_info_ind:vice_dialog_info");
    return;
  }

  vice_info_ind = QM_MEM_ALLOC(sizeof(*vice_info_ind));
  if( NULL == vice_info_ind )
  {
    return;
  }

  if(vice_dialog_info->vice_xml != NULL)
  {
    xml_ptr = vice_dialog_info->vice_xml;
    xml_length = (uint32) strlen( (char *) vice_dialog_info->vice_xml);
    num_indications = (int)(xml_length/QMI_VOICE_VICE_XML_MAX_LEN) + 1;

    QM_MSG_MED_1("num_indications=%d", num_indications);

    for(ind=0; ind<num_indications; ind++)
    {
      memset( vice_info_ind, 0, sizeof(*vice_info_ind) );
      vice_info_ind->sequence = ind;
      if(vice_info_ind->sequence == 0) vice_info_ind->total_size_valid = TRUE;
      vice_info_ind->total_size = xml_length;


      ptr_begin = xml_ptr+(ind*QMI_VOICE_VICE_XML_MAX_LEN);
      if(ind == num_indications-1)
      {
        copy_length = xml_length - (ind*QMI_VOICE_VICE_XML_MAX_LEN);
      }
      else
      {
        copy_length = QMI_VOICE_VICE_XML_MAX_LEN;
      }
      QM_MSG_MED_2("Sending Indication Number = %d, copy_len = %d", ind+1, copy_length);
      vice_info_ind->vice_dialog_xml_len = copy_length;
      memscpy(vice_info_ind->vice_dialog_xml,sizeof(vice_info_ind->vice_dialog_xml),ptr_begin,copy_length);

      //Send indication for every 2k of XMl string
      qmi_voice_cm_if_unsol_response(QMI_VOICE_CM_IF_CMD_VICE_DIALOG_IND,
                                     (void *) vice_info_ind,
                                     sizeof(*vice_info_ind),
                                     as_id);

    }
  }
  else
  {
    vice_info_ind->total_size_valid = TRUE;
    vice_info_ind->vice_dialog_xml_len = 0;
    vice_info_ind->total_size = 0;
    vice_info_ind->sequence = 0;
    qmi_voice_cm_if_unsol_response(QMI_VOICE_CM_IF_CMD_VICE_DIALOG_IND,
                                   (void *) vice_info_ind,
                                   sizeof(*vice_info_ind),
                                   as_id);


  }
  QM_MEM_FREE(vice_info_ind);
} /* qmi_voice_msgr_vice_dialog_info_ind() */

/*===========================================================================
  FUNCTION qmi_voice_msgr_get_wwan_911_timer_resp()

  DESCRIPTION
    Send tty info indication based on the call event

  PARAMETERS

  RETURN VALUE
    None

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static void qmi_voice_msgr_get_wwan_911_timer_resp
(
  cm_mm_get_t_wwan_911_rsp_s_type *cm_data,
  sys_modem_as_id_e_type asubs_id
)
{
  qmi_voice_cmd_list_public_type             cmd_info;
  voice_get_wwan_911_timer_resp_msg_v02      get_wwan_911_timer_resp;
  uint8                                      as_id = (uint8)asubs_id;


  //QM_MSG_MED("Inside qmi_voice_msgr_get_wwan_911_timer_resp():");

  if(cm_data == NULL)
  {
    QM_MSG_ERROR("qmi_voice_msgr_get_wwan_911_timer_resp: CM data field is NULL");
    return;
  }

  //qmi_voice_msgr_get_wwan_911_timer_resp
  memset(&get_wwan_911_timer_resp,0,sizeof(get_wwan_911_timer_resp));

  if ( qmi_voice_cmd_list_query_by_event( QMI_VOICE_CM_IF_EVT_MSGR_GET_WWAN_911_TIMER_CONF, &cmd_info, as_id ) == E_SUCCESS )
  {
    QM_MSG_MED_2("qmi_voice_msgr_get_wwan_911_timer_resp: timer_value = %d, as_id = %d", cm_data->t_wwan_911_val, as_id);

    get_wwan_911_timer_resp.wwan_911_timer_valid = TRUE;
    get_wwan_911_timer_resp.wwan_911_timer = cm_data->t_wwan_911_val;

    qmi_voice_cm_if_response_success( cmd_info.hdl, cmd_info.cmd,
                                      (void *)&get_wwan_911_timer_resp, sizeof(get_wwan_911_timer_resp), TRUE );
  }
  /*Else => There is no cmd list awaiting for QMI_VOICE_CM_IF_EVT_MSGR_GET_WWAN_911_TIMER_CONF*/


} /* qmi_voice_msgr_get_wwan_911_timer_resp() */
