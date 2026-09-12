/**
  @file vs_actions.c
  @brief This file contains the definitions of the internal functions of VS.
*/

/*
  Copyright (C) 2018-2022 QUALCOMM Technologies, Inc.
  All Rights Reserved.
  Confidential and Proprietary - Qualcomm Technologies, Inc.

  $Header: //components/rel/avs.mpss/9.0.c51/vsd/vs/src/vs_actions.c#1 $
  $Author: pwbldsvc $
*/

/****************************************************************************
 * HEADER FILES                                                     *
 ****************************************************************************/
#include "mmstd.h"
#include "voicecfg_api.h"
#include "voicecfg_items.h"
#include "vs_i.h"

#if !defined ( MCPM_VSTMR_STUB ) && !defined ( WINSIM ) && !defined (AVS_MPSS_TEST)
#include "mcpm_api.h"
#endif

/****************************************************************************
 * STATIC DEFINIIONS                                                      *
 ****************************************************************************/
#if !defined ( MCPM_VSTMR_STUB ) && !defined ( WINSIM ) && !defined (AVS_MPSS_TEST)
static vstmr_vfr_e vs_voc_get_vfr_id (
 uint32_t vsid
)
{
  vstmr_vfr_e vfr_id = VSTMR_VFR_AUTO;
  uint32_t mux_id = 0;
  
  /* 18 to 21 bit positions in VSID constitute the Mux ID. */
  mux_id = ( vsid >> VS_MUX_ID_MASK_BIT_POS ) & VS_MUX_ID_MASK_VAL;
  
  switch( mux_id )
  {
    case 0:
      vfr_id = VSTMR_VFR_PRI;
      break;

    case 7:
      vfr_id = VSTMR_VFR_SEC;
      break;

    default:
      MSG_1( MSG_SSID_DFLT, MSG_LEGACY_ERROR,
           "vs_voc_get_vfr_id(): Invalid "
           "mux_id=%d", mux_id );
      break;
  }
  return vfr_id;
}
#endif  

static uint32_t vs_voc_get_vcpm_network_id (
  uint32_t vs_client_id
)
{
  uint32_t vcpm_network_id = VS_PARAM_UINT32_NOT_SET;

  switch ( vs_client_id )
  {
    case VS_COMMON_NETWORK_ID_CDMA:
      vcpm_network_id = VCPM_CAL_KEY_NETWORK_CDMA;
      break;

    case VS_COMMON_NETWORK_ID_GSM:
      vcpm_network_id = VCPM_CAL_KEY_NETWORK_GSM;
      break;

    case VS_COMMON_NETWORK_ID_WCDMA:
      vcpm_network_id = VCPM_CAL_KEY_NETWORK_WCDMA;
      break;

    case VS_COMMON_NETWORK_ID_TDSCDMA:
      vcpm_network_id = VCPM_CAL_KEY_NETWORK_TDSCDMA;
      break;

    case VS_COMMON_NETWORK_ID_LTE:
    case VS_COMMON_NETWORK_ID_WLAN:
    case VS_COMMON_NETWORK_ID_NR:
    case VS_COMMON_NETWORK_ID_CIWLAN:
      vcpm_network_id = VCPM_CAL_KEY_NETWORK_LTE;
      break;

    default:
      break;
  }

  return vcpm_network_id;
}

/****************************************************************************
 * FUNCTION DEFINIIONS                                                      *
 ****************************************************************************/
uint32_t vs_voc_action_set_dtmf_gen_to_mailbox_tx (
  vs_session_object_t* session_obj
)
{
  int32_t rc = VS_EOK;
  vs_simple_job_object_t* job_obj;
  gpr_cmd_alloc_ext_t mbox_params;
  gpr_packet_t* packet = NULL;
  apm_cmd_header_t *cmd_header;
  apm_module_param_data_t *param_data;
  vs_vocdtmf_frame_data_t *dtmf_config;
  
  rc = vs_create_simple_job_object( session_obj->header.handle, &job_obj );
  job_obj->fn_table[ VS_RESPONSE_FN_ENUM_RESULT ] = vs_voc_transition_result_rsp_fn;
  session_obj->session_ctrl.transition_job_handle = job_obj->header.handle;

  mbox_params.src_domain_id = GPR_IDS_DOMAIN_ID_MODEM_V ;
  mbox_params.src_port = VS_MODULE_INSTANCE_ID;
  mbox_params.dst_domain_id = GPR_IDS_DOMAIN_ID_ADSP_V ; 
  mbox_params.dst_port = session_obj->mod_iid_tag_info[VS_MOD_TX_EP].mod_iid ;
  mbox_params.token = job_obj->header.handle;
  mbox_params.opcode = APM_CMD_SET_CFG;
  mbox_params.payload_size = sizeof(apm_cmd_header_t) + sizeof(apm_module_param_data_t) + 
                             sizeof(vs_vocdtmf_frame_data_t) ;
  mbox_params.client_data = 0;
  mbox_params.ret_packet =&packet ;


  rc = __gpr_cmd_alloc_ext(&mbox_params);

  if( packet == NULL )
  {
    VS_REPORT_FATAL_ON_ERROR( rc );
    return VS_EFAILED;
  } 

  cmd_header = GPR_PKT_GET_PAYLOAD( apm_cmd_header_t, packet );
  cmd_header->mem_map_handle = 0;
  cmd_header->payload_address_lsw = 0;
  cmd_header->payload_address_msw = 0;
  cmd_header->payload_size = sizeof(apm_module_param_data_t) + sizeof(payload_media_fmt_dtmf_tone_t);
  
  param_data = (apm_module_param_data_t*)( (uint8_t*)(cmd_header) + sizeof(apm_cmd_header_t) );
  param_data->module_instance_id = session_obj->mod_iid_tag_info[VS_MOD_TX_EP].mod_iid;
  param_data->param_id = PARAM_ID_DTMF_CFG;
  param_data->param_size = sizeof(payload_media_fmt_dtmf_tone_t);

  dtmf_config = (vs_vocdtmf_frame_data_t*)( (uint8_t*)param_data + sizeof(apm_module_param_data_t) );


  /* Set the configuration information for Tx DTMF Generation */
  *dtmf_config = session_obj->dtmf_info;

  rc = __gpr_cmd_async_send(packet);
  if ( rc )
  {
    VS_REPORT_FATAL_ON_ERROR(rc);
    session_obj->session_ctrl.status |= rc;
    ( void ) vs_mem_free_object( ( vs_object_t* ) job_obj );
    ( void )__gpr_cmd_free( packet);
    return VS_EIMMEDIATE;
  }

  session_obj->cmd_ctrl.cmd_cnt ++;
  return VS_EOK;
}
uint32_t vs_voc_action_request_mailbox_memory_config (
  vs_session_object_t* session_obj
)
{
  int32_t rc = VS_EOK;
  vs_simple_job_object_t* job_obj = NULL;
  gpr_cmd_alloc_ext_t mbox_params;
  gpr_packet_t* packet = NULL;
  apm_cmd_header_t *cmd_header;
  apm_module_param_data_t *param_data;

  rc = vs_create_simple_job_object( session_obj->header.handle, &job_obj );
  job_obj->fn_table[ VS_RESPONSE_FN_ENUM_RESULT ] = vs_voc_transition_result_rsp_fn;
  job_obj->fn_table[ VS_RESPONSE_FN_ENUM_MBOX_MEMORY_CONFIG ] = vs_voc_mailbox_memory_config_rsp_fn;
  
  session_obj->session_ctrl.transition_job_handle = job_obj->header.handle;

  mbox_params.src_domain_id = GPR_IDS_DOMAIN_ID_MODEM_V;
  mbox_params.src_port = VS_MODULE_INSTANCE_ID ;
  mbox_params.dst_domain_id = GPR_IDS_DOMAIN_ID_ADSP_V;
  mbox_params.dst_port = VCPM_MODULE_INSTANCE_ID;
  mbox_params.token = job_obj->header.handle;
  mbox_params.opcode = APM_CMD_GET_CFG;
  mbox_params.payload_size = sizeof(apm_cmd_header_t) + sizeof(apm_module_param_data_t) + 
                                  sizeof(vcpm_param_id_mailbox_memory_config_t);
  mbox_params.client_data = 0;
  mbox_params.ret_packet = &packet ;

  rc = __gpr_cmd_alloc_ext(&mbox_params);

  if( packet == NULL )
  {
    VS_REPORT_FATAL_ON_ERROR( rc );
    return VS_EFAILED;
  }

  cmd_header = GPR_PKT_GET_PAYLOAD( apm_cmd_header_t, packet );
  cmd_header->mem_map_handle = 0;
  cmd_header->payload_address_lsw = 0;
  cmd_header->payload_address_msw = 0;
  cmd_header->payload_size = sizeof(apm_module_param_data_t) + sizeof(vcpm_param_id_mailbox_memory_config_t);

  param_data = (apm_module_param_data_t*)( (uint8_t*)cmd_header + sizeof(apm_cmd_header_t) );
  param_data->module_instance_id = VCPM_MODULE_INSTANCE_ID;
  param_data->param_id = VCPM_PARAM_ID_MAILBOX_MEMORY_CONFIG;
  param_data->param_size = sizeof(vcpm_param_id_mailbox_memory_config_t);

  rc = __gpr_cmd_async_send(packet);

  if ( rc )
  {
    VS_REPORT_FATAL_ON_ERROR(rc);
    session_obj->session_ctrl.status |= rc;
    ( void ) vs_mem_free_object( ( vs_object_t* ) job_obj );
    ( void )__gpr_cmd_free( packet);
    return VS_EIMMEDIATE;
  }

  session_obj->cmd_ctrl.cmd_cnt ++;
  return rc;
}

uint32_t vs_voc_action_register_operating_mode_update (
  vs_session_object_t* session_obj
)
{
  int32_t rc = VS_EOK;
  vs_simple_job_object_t* job_obj = NULL;
  voicecfg_cmd_get_bool_item_t item;
  apm_cmd_header_t* cmd_header;
  apm_module_register_events_t *register_payload;
  uint32_t *client_data_payload;
  gpr_cmd_alloc_ext_t evt_params;
  gpr_packet_t* packet = NULL;

  item.id = VOICECFG_CFG_IS_EAMR_REPORT_MODE_CHANGE_ENABLED;
  item.ret_value = FALSE;
  rc = voicecfg_call( VOICECFG_CMD_GET_BOOL_ITEM, &item, sizeof ( item ) );


  if ( (( !rc ) && ( item.ret_value == TRUE )) || (MEDIA_FMT_ID_EVS == session_obj->vocoder.media_id) )
  {
    rc = vs_create_simple_job_object( session_obj->header.handle, &job_obj );
    job_obj->fn_table[ VS_RESPONSE_FN_ENUM_RESULT ] = vs_voc_transition_result_rsp_fn;
    session_obj->session_ctrl.transition_job_handle = job_obj->header.handle;

    evt_params.src_domain_id = GPR_IDS_DOMAIN_ID_MODEM_V ;
    evt_params.src_port = VS_MODULE_INSTANCE_ID;
    evt_params.dst_domain_id = GPR_IDS_DOMAIN_ID_ADSP_V ; 
    evt_params.dst_port = session_obj->mod_iid_tag_info[VS_MOD_DECODER].mod_iid ; ;
    evt_params.token = job_obj->header.handle;
    evt_params.opcode = APM_CMD_REGISTER_MODULE_EVENTS;
    evt_params.payload_size = sizeof(apm_cmd_header_t) + sizeof(apm_module_register_events_t) + sizeof(uint32_t);
    evt_params.client_data = 0;
    evt_params.ret_packet =&packet ;

    rc = __gpr_cmd_alloc_ext(&evt_params);

    if( packet == NULL )
    {
      VS_REPORT_FATAL_ON_ERROR( rc );
      return VS_EFAILED;
    }

    cmd_header = GPR_PKT_GET_PAYLOAD( apm_cmd_header_t, packet );
    cmd_header->mem_map_handle = 0;
    cmd_header->payload_address_lsw = 0;
    cmd_header->payload_address_msw = 0;
    cmd_header->payload_size = sizeof(apm_module_register_events_t)  + sizeof(uint32_t);

    register_payload = (apm_module_register_events_t*)( (uint8_t*)cmd_header + sizeof(apm_cmd_header_t) );
    register_payload->module_instance_id = session_obj->mod_iid_tag_info[VS_MOD_DECODER].mod_iid ;
    register_payload->event_id = EVENT_ID_VOCODER_OPERATING_MODE;
    register_payload->is_register = TRUE;
    register_payload->event_config_payload_size = sizeof(uint32_t);
    register_payload->reserved = 0;

    client_data_payload = (uint32_t*) (register_payload + 1);
    *client_data_payload = (uint32_t) session_obj->vsid;

    rc = __gpr_cmd_async_send(packet);

    if (rc)
    {
      VS_REPORT_FATAL_ON_ERROR(rc);
      session_obj->session_ctrl.status |= rc;
      ( void ) vs_mem_free_object( ( vs_object_t* ) job_obj );
      ( void )__gpr_cmd_free( packet);
      return VS_EIMMEDIATE;
    }
  }
  else
  {
    MSG_2( MSG_SSID_DFLT, MSG_LEGACY_MED,"VS:vs_voc_action_register_operating_mode_update(): Not registering for operating mode update, E-AMR enabled %d or Media ID 0x%08x ",
	       item.ret_value,session_obj->vocoder.media_id );
    return VS_EIMMEDIATE;
  }

  session_obj->cmd_ctrl.cmd_cnt++;
  return rc;
}


uint32_t vs_voc_action_deregister_operating_mode_update (
  vs_session_object_t* session_obj
)
{
  int32_t rc = VS_EOK;
  vs_simple_job_object_t* job_obj = NULL;
  voicecfg_cmd_get_bool_item_t item;
  apm_cmd_header_t* cmd_header;
  apm_module_register_events_t *register_payload;
  gpr_cmd_alloc_ext_t evt_params;
  gpr_packet_t* packet = NULL;

  item.id = VOICECFG_CFG_IS_EAMR_REPORT_MODE_CHANGE_ENABLED;
  item.ret_value = FALSE;
  rc = voicecfg_call( VOICECFG_CMD_GET_BOOL_ITEM, &item, sizeof ( item ) );

  if ( (( !rc ) && ( item.ret_value == TRUE )) || (MEDIA_FMT_ID_EVS == session_obj->vocoder.media_id))
  {
    rc = vs_create_simple_job_object( session_obj->header.handle, &job_obj );
    job_obj->fn_table[ VS_RESPONSE_FN_ENUM_RESULT ] = vs_voc_transition_result_rsp_fn;
    session_obj->session_ctrl.transition_job_handle = job_obj->header.handle;

    evt_params.src_domain_id = GPR_IDS_DOMAIN_ID_MODEM_V ;
    evt_params.src_port = VS_MODULE_INSTANCE_ID;
    evt_params.dst_domain_id = GPR_IDS_DOMAIN_ID_ADSP_V ; 
    evt_params.dst_port = session_obj->mod_iid_tag_info[VS_MOD_DECODER].mod_iid ; ;
    evt_params.token = job_obj->header.handle;
    evt_params.opcode = APM_CMD_REGISTER_MODULE_EVENTS;
    evt_params.payload_size = sizeof(apm_cmd_header_t) + sizeof(apm_module_register_events_t);
    evt_params.client_data = 0;
    evt_params.ret_packet =&packet ;

    rc = __gpr_cmd_alloc_ext(&evt_params);

    if( packet == NULL )
    {
      VS_REPORT_FATAL_ON_ERROR( rc );
      return VS_EFAILED;
    }

    cmd_header = GPR_PKT_GET_PAYLOAD( apm_cmd_header_t, packet );
    cmd_header->mem_map_handle = 0;
    cmd_header->payload_address_lsw = 0;
    cmd_header->payload_address_msw = 0;
    cmd_header->payload_size = sizeof(apm_module_register_events_t);

    register_payload = (apm_module_register_events_t*)( (uint8_t*)cmd_header + sizeof(apm_cmd_header_t) );
    register_payload->module_instance_id = session_obj->mod_iid_tag_info[VS_MOD_DECODER].mod_iid ;
    register_payload->event_id = EVENT_ID_VOCODER_OPERATING_MODE;
    register_payload->is_register = FALSE;
    register_payload->event_config_payload_size = 0;
    register_payload->reserved = 0;

    rc = __gpr_cmd_async_send(packet);
    if (rc)
    {
      VS_REPORT_FATAL_ON_ERROR(rc);
      session_obj->session_ctrl.status |= rc;
      ( void ) vs_mem_free_object( ( vs_object_t* ) job_obj );
      ( void )__gpr_cmd_free( packet);
      return VS_EIMMEDIATE;
    }
  }
  else
  {
    MSG_2( MSG_SSID_DFLT, MSG_LEGACY_MED,
         "VS:vs_voc_action_deregister_operating_mode_update(): Not de-registering for operating mode update,E-AMR enabled %d or Media ID 0x%08x",
		   item.ret_value, session_obj->vocoder.media_id );
    return VS_EIMMEDIATE;
  }

  session_obj->cmd_ctrl.cmd_cnt++;
  return rc;
}

static uint32_t vs_voc_get_evs_sampling_rate (
  uint16_t bandwidth
)
{
  uint32_t sampling_rate = VS_PARAM_UINT32_NOT_SET;

  switch ( bandwidth )
  {
    case VS_VOC_BANDWIDTH_NB:
         sampling_rate = 8000;
		 break;
		 
    case VS_VOC_BANDWIDTH_WB:
         sampling_rate = 16000;
		 break;
  
    case VS_VOC_BANDWIDTH_SWB:
         sampling_rate = 32000;
		 break;

    case VS_VOC_BANDWIDTH_FB:
         sampling_rate = 48000;
		 break;
		 
    default:
      MSG_1( MSG_SSID_DFLT, MSG_LEGACY_ERROR,
         "VS:vs_voc_get_evs_sampling_rate(): Received unsupported bandwidth[%d]",
         bandwidth );
      break;
  }

  return sampling_rate;

}

static uint32_t vs_voc_get_cal_value (
  vs_session_object_t* session_object, 
  uint32_t cal_key_id
)
{
  uint32_t vocoder_class = VS_PARAM_UINT32_NOT_SET;
  uint32_t sampling_rate = VS_PARAM_UINT32_NOT_SET;

  switch ( session_object->vocoder.media_id )
  {
    case MEDIA_FMT_ID_AMRNB_MODEM:
    case MEDIA_FMT_ID_EFR:
    case MEDIA_FMT_ID_FR:
    case MEDIA_FMT_ID_HR:
    {
      vocoder_class = VCPM_CAL_KEY_VOCODER_CLASS_AMR ;
      sampling_rate = 8000;
    }
    break;

    case MEDIA_FMT_ID_AMRWB:
    case MEDIA_FMT_ID_EAMR:
    {
      vocoder_class = VCPM_CAL_KEY_VOCODER_CLASS_AMR;
      sampling_rate = 16000;
    }
    break;

    case VSS_MEDIA_ID_AMR_WB_2:
    {
      vocoder_class = VCPM_CAL_KEY_VOCODER_CLASS_EVS;
      sampling_rate = 16000;
    }
    break;

    case MEDIA_FMT_ID_EVRC:
    case MEDIA_FMT_ID_4GV_NB_MODEM:
    case MEDIA_FMT_ID_V13K:
    {
      vocoder_class = VCPM_CAL_KEY_VOCODER_CLASS_EVRC;
      sampling_rate = 8000;
    }
    break;

    case MEDIA_FMT_ID_4GV_WB_MODEM:
    case MEDIA_FMT_ID_4GV_NW_MODEM:
    case MEDIA_FMT_ID_EVRC_NW_2K:
    {
      vocoder_class = VCPM_CAL_KEY_VOCODER_CLASS_EVRC;
      sampling_rate = 16000;
    }
    break;

    case MEDIA_FMT_ID_G711_ALAW_V2:
    case MEDIA_FMT_ID_G711_MLAW_V2:
    {
      vocoder_class = VCPM_CAL_KEY_VOCODER_CLASS_EVS;
      sampling_rate = 8000;
    }
    break;

    case MEDIA_FMT_ID_EVS:
    {
      vocoder_class = VCPM_CAL_KEY_VOCODER_CLASS_EVS;
      if( session_object->vocoder.encoder_bw != VS_PARAM_UINT8_NOT_SET )
      {
        sampling_rate = vs_voc_get_evs_sampling_rate( session_object->vocoder.encoder_bw );
      }
      else
        sampling_rate = 8000;
    }
    break;
    
    default:
      MSG_1( MSG_SSID_DFLT, MSG_LEGACY_ERROR,
         "VS:vs_voc_get_cal_value(): Received unsupported Media ID[0x%08x]",
         session_object->vocoder.media_id );
      break;
  }

  MSG_2( MSG_SSID_DFLT, MSG_LEGACY_MED,
         "VS:vs_voc_get_cal_value(): Vocoder class: %d, sampling_rate: %d",
         vocoder_class, sampling_rate );

  if(VCPM_CAL_KEY_ID_VOCODER_CLASS == cal_key_id)
    return vocoder_class;
  else
    return sampling_rate;

}

/*Set network key */
uint32_t vs_voc_action_set_network_key (
  vs_session_object_t* session_obj
)
{
  int32_t rc = VS_EOK;
  vs_simple_job_object_t* job_obj = NULL;
  gpr_cmd_alloc_ext_t vocoder_params;
  gpr_packet_t* packet = NULL;
  apm_cmd_header_t *cmd_header;
  apm_module_param_data_t *param_data;
  vcpm_param_cal_keys_payload_t *cal_payload;
  vcpm_ckv_pair_t *ckv_pair;

  rc = vs_create_simple_job_object( session_obj->header.handle, &job_obj );
  job_obj->fn_table[ VS_RESPONSE_FN_ENUM_RESULT ] = vs_voc_transition_result_rsp_fn;
  session_obj->session_ctrl.transition_job_handle = job_obj->header.handle;

  vocoder_params.src_domain_id = GPR_IDS_DOMAIN_ID_MODEM_V;
  vocoder_params.src_port = VS_MODULE_INSTANCE_ID ;
  vocoder_params.dst_domain_id = GPR_IDS_DOMAIN_ID_ADSP_V;
  vocoder_params.dst_port = VCPM_MODULE_INSTANCE_ID;
  vocoder_params.token = job_obj->header.handle;
  vocoder_params.opcode = APM_CMD_SET_CFG;
  vocoder_params.payload_size = sizeof(apm_cmd_header_t) + sizeof(apm_module_param_data_t) + 
                             sizeof(vcpm_param_cal_keys_payload_t) + (sizeof(vcpm_ckv_pair_t));
  vocoder_params.client_data = 0;
  vocoder_params.ret_packet =&packet ;

  rc = __gpr_cmd_alloc_ext(&vocoder_params);

  if( packet == NULL )
  {
    VS_REPORT_FATAL_ON_ERROR( rc );
    return VS_EFAILED;
  }

  cmd_header = GPR_PKT_GET_PAYLOAD( apm_cmd_header_t, packet );
  cmd_header->mem_map_handle = 0;
  cmd_header->payload_address_lsw = 0;
  cmd_header->payload_address_msw = 0;
  cmd_header->payload_size = sizeof(apm_module_param_data_t) + sizeof(vcpm_param_cal_keys_payload_t) + sizeof(vcpm_ckv_pair_t);

  param_data = (apm_module_param_data_t*)( (uint8_t*)cmd_header + sizeof(apm_cmd_header_t) );
  param_data->module_instance_id = VCPM_MODULE_INSTANCE_ID;
  param_data->param_id = VCPM_PARAM_ID_CAL_KEYS;
  param_data->param_size = sizeof(vcpm_param_cal_keys_payload_t) + sizeof(vcpm_ckv_pair_t);

  cal_payload = (vcpm_param_cal_keys_payload_t*)( (uint8_t*)param_data + sizeof(apm_module_param_data_t) );
  cal_payload->vsid = session_obj->vsid;
  cal_payload->num_ckv_pairs = 1;

  ckv_pair = (vcpm_ckv_pair_t*)( (uint8_t*)cal_payload + sizeof(vcpm_param_cal_keys_payload_t) );
  ckv_pair->cal_key_id = VCPM_CAL_KEY_ID_NETWORK;
  ckv_pair->value = vs_voc_get_vcpm_network_id(session_obj->vocoder.client_id);

  rc = __gpr_cmd_async_send(packet);

  if ( rc )
  {
    VS_REPORT_FATAL_ON_ERROR(rc);
    session_obj->session_ctrl.status |= rc;
    ( void ) vs_mem_free_object( ( vs_object_t* ) job_obj );
    ( void )__gpr_cmd_free( packet);
    return VS_EIMMEDIATE;
  }

  session_obj->cmd_ctrl.cmd_cnt ++;
  return rc;
}
/**
 * Set vocoder class on vcpm
 */
uint32_t vs_voc_action_set_vocoder_class_on_vcpm (
  vs_session_object_t* session_obj
)
{
  int32_t rc = VS_EOK;
  vs_simple_job_object_t* job_obj = NULL;
  gpr_cmd_alloc_ext_t vocoder_params;
  gpr_packet_t* packet = NULL;
  apm_cmd_header_t *cmd_header;
  apm_module_param_data_t *param_data;
  vcpm_param_cal_keys_payload_t *cal_payload;
  vcpm_ckv_pair_t *ckv_pair;

  rc = vs_create_simple_job_object( session_obj->header.handle, &job_obj );
  job_obj->fn_table[ VS_RESPONSE_FN_ENUM_RESULT ] = vs_voc_transition_result_rsp_fn;
  session_obj->session_ctrl.transition_job_handle = job_obj->header.handle;

  vocoder_params.src_domain_id = GPR_IDS_DOMAIN_ID_MODEM_V;
  vocoder_params.src_port = VS_MODULE_INSTANCE_ID ;
  vocoder_params.dst_domain_id = GPR_IDS_DOMAIN_ID_ADSP_V;
  vocoder_params.dst_port = VCPM_MODULE_INSTANCE_ID;
  vocoder_params.token = job_obj->header.handle;
  vocoder_params.opcode = APM_CMD_SET_CFG;
  vocoder_params.payload_size = sizeof(apm_cmd_header_t) + sizeof(apm_module_param_data_t) + 
                             sizeof(vcpm_param_cal_keys_payload_t) + ((VS_MAX_NUM_CKV_PAIRS)*sizeof(vcpm_ckv_pair_t));
  vocoder_params.client_data = 0;
  vocoder_params.ret_packet =&packet ;

  rc = __gpr_cmd_alloc_ext(&vocoder_params);

  if( packet == NULL )
  {
    VS_REPORT_FATAL_ON_ERROR( rc );
    return VS_EFAILED;
  }

  cmd_header = GPR_PKT_GET_PAYLOAD( apm_cmd_header_t, packet );
  cmd_header->mem_map_handle = 0;
  cmd_header->payload_address_lsw = 0;
  cmd_header->payload_address_msw = 0;
  cmd_header->payload_size = sizeof(apm_module_param_data_t) + sizeof(vcpm_param_cal_keys_payload_t) + ((VS_MAX_NUM_CKV_PAIRS)*sizeof(vcpm_ckv_pair_t));

  param_data = (apm_module_param_data_t*)( (uint8_t*)cmd_header + sizeof(apm_cmd_header_t) );
  param_data->module_instance_id = VCPM_MODULE_INSTANCE_ID;
  param_data->param_id = VCPM_PARAM_ID_CAL_KEYS;
  param_data->param_size = sizeof(vcpm_param_cal_keys_payload_t) + (VS_MAX_NUM_CKV_PAIRS)*sizeof(vcpm_ckv_pair_t);

  cal_payload = (vcpm_param_cal_keys_payload_t*)( (uint8_t*)param_data + sizeof(apm_module_param_data_t) );
  cal_payload->vsid = session_obj->vsid;
  cal_payload->num_ckv_pairs = VS_MAX_NUM_CKV_PAIRS;

  ckv_pair = (vcpm_ckv_pair_t*)( (uint8_t*)cal_payload + sizeof(vcpm_param_cal_keys_payload_t) );

  for ( uint32_t i = 0; i< cal_payload->num_ckv_pairs; i++)
  {
    ckv_pair->cal_key_id =session_obj->vs_ckv_key_value_table[i];
    ckv_pair->value = vs_voc_get_cal_value(session_obj, session_obj->vs_ckv_key_value_table[i]);

    ckv_pair++ ;
  }

  rc = __gpr_cmd_async_send(packet);

  if ( rc )
  {
    VS_REPORT_FATAL_ON_ERROR(rc);
    session_obj->session_ctrl.status |= rc;
    ( void ) vs_mem_free_object( ( vs_object_t* ) job_obj );
    ( void )__gpr_cmd_free( packet);
    return VS_EIMMEDIATE;
  }

  session_obj->cmd_ctrl.cmd_cnt ++;
  return rc;
}

uint32_t vs_voc_action_set_voice_timing_offsets (
  vs_session_object_t* session_obj
)
{
  uint32_t rc = VS_EOK;
  vs_simple_job_object_t* job_obj = NULL;
  gpr_cmd_alloc_ext_t timing_params;
  gpr_packet_t* packet = NULL;
  apm_cmd_header_t *cmd_header;
  apm_module_param_data_t *param_data;
  vcpm_param_timing_params_t *offset_params;

  rc = vs_create_simple_job_object( session_obj->header.handle, &job_obj );
  job_obj->fn_table[ VS_RESPONSE_FN_ENUM_RESULT ] = vs_voc_transition_result_rsp_fn;
  session_obj->session_ctrl.transition_job_handle = job_obj->header.handle;

  timing_params.src_domain_id = GPR_IDS_DOMAIN_ID_MODEM_V ;
  timing_params.src_port = VS_MODULE_INSTANCE_ID;
  timing_params.dst_domain_id = GPR_IDS_DOMAIN_ID_ADSP_V ; 
  timing_params.dst_port = VCPM_MODULE_INSTANCE_ID;
  timing_params.token = job_obj->header.handle;
  timing_params.opcode = APM_CMD_SET_CFG;
  timing_params.payload_size = sizeof(apm_cmd_header_t) + sizeof(apm_module_param_data_t) + 
                             sizeof(vcpm_param_timing_params_t) ;
  timing_params.client_data = 0;
  timing_params.ret_packet =&packet ;
  
  rc = __gpr_cmd_alloc_ext(&timing_params);
  
  if( packet == NULL )
  {
    VS_REPORT_FATAL_ON_ERROR( rc );
    return VS_EFAILED;
  }

  cmd_header = GPR_PKT_GET_PAYLOAD( apm_cmd_header_t, packet );
  cmd_header->mem_map_handle = 0;
  cmd_header->payload_address_lsw = 0;
  cmd_header->payload_address_msw = 0;
  cmd_header->payload_size = sizeof(apm_module_param_data_t) + sizeof(vcpm_param_timing_params_t) ;
  
  param_data = (apm_module_param_data_t*)( (uint8_t*)cmd_header + sizeof(apm_cmd_header_t) );
  param_data->module_instance_id = VCPM_MODULE_INSTANCE_ID;
  param_data->param_id = VCPM_PARAM_ID_TIMING_PARAMS;
  param_data->param_size = sizeof(vcpm_param_timing_params_t) ;

  offset_params =  (vcpm_param_timing_params_t*)( (uint8_t*)param_data + sizeof(apm_module_param_data_t) );
  offset_params->vfr_mode = session_obj->vocoder.vfr_config.vfr_mode;
  offset_params->vsid = session_obj->vsid;
  offset_params->tx_delivery_offset_us = session_obj->vocoder.timing_config.enc_offset_us;
  offset_params->rx_start_offset_us = session_obj->vocoder.timing_config.dec_offset_us;
  offset_params->vfr_cycle_us = session_obj->vocoder.vfr_config.vfr_cycle_duration_ms*1000;

  MSG_5( MSG_SSID_DFLT, MSG_LEGACY_MED,
         "VS: TIMING_PARAMS: handle[0x%08x], vsid[0x%08x], vfr_mode[%d], enc_off[%d],dec_off[%d]",
          job_obj->header.handle, offset_params->vsid, 
         offset_params->vfr_mode, offset_params->tx_delivery_offset_us, offset_params->rx_start_offset_us );

  rc = __gpr_cmd_async_send(packet);

  if ( rc )
  {
    VS_REPORT_FATAL_ON_ERROR(rc);
    session_obj->session_ctrl.status |= rc;
    ( void ) vs_mem_free_object( ( vs_object_t* ) job_obj );
    ( void )__gpr_cmd_free( packet);
    return VS_EIMMEDIATE;
  }

  session_obj->cmd_ctrl.cmd_cnt++;
  return rc;
}

uint32_t vs_voc_action_calculate_timing_offsets_from_timing_references (
  vs_session_object_t* session_obj
)
{
  uint32_t enc_offset, dec_req_offset, dec_offset;
  uint32_t vfr_offset, client_tx_offset, client_rx_offset;
  uint32_t vfr_cycle_duration_us;

  vfr_cycle_duration_us = session_obj->vocoder.vfr_config.vfr_cycle_duration_ms * 1000;
#if ( defined AVS_MPSS_TEST )
  session_obj->vocoder.timing_config.vfr_start_timestamp_us = session_obj->vocoder.timing_config.tx_ref_timestamp_us + 3000;
#endif  
  vfr_offset = session_obj->vocoder.timing_config.vfr_start_timestamp_us % vfr_cycle_duration_us;
  client_tx_offset = session_obj->vocoder.timing_config.tx_ref_timestamp_us % vfr_cycle_duration_us;
  client_rx_offset = session_obj->vocoder.timing_config.rx_ref_timestamp_us % vfr_cycle_duration_us;

  MSG_3( MSG_SSID_DFLT, MSG_LEGACY_MED,         
         "VS: TIMING_OFFSETS: vfr_start_timestamp_us:(%lu) tx_ref_timestamp_us:(%lu) rx_ref_timestamp_us:(%lu)",
           session_obj->vocoder.timing_config.vfr_start_timestamp_us,
           session_obj->vocoder.timing_config.tx_ref_timestamp_us,
           session_obj->vocoder.timing_config.rx_ref_timestamp_us );

  MSG_3( MSG_SSID_DFLT, MSG_LEGACY_MED,         
         "VS: TIMING_OFFSETS: vfr_offset:(%d) client_tx_offset:(%d) client_rx_offset:(%d)",
           vfr_offset, client_tx_offset, client_rx_offset );

  { /* Encoder offset calculation. */
    enc_offset = ( vfr_cycle_duration_us + client_tx_offset - vfr_offset ) %
                    vfr_cycle_duration_us;

    /* Adding margin to encoder offset. */
    enc_offset = ( enc_offset + vfr_cycle_duration_us - VS_VOC_TIMING_ENC_OFFSET_MARGIN_V ) %
                    vfr_cycle_duration_us;

  /**
   * Encoder offset should not be less than MIN timing offset value.
   */
    enc_offset = ( enc_offset >= VS_VOC_MIN_TIMING_OFFSET_V )? enc_offset : ( vfr_cycle_duration_us - VS_VOC_MIN_TIMING_OFFSET_V);
  }

  { /* Decoder request offset calculation. */
    dec_req_offset = ( vfr_cycle_duration_us + client_rx_offset - vfr_offset ) %
                       vfr_cycle_duration_us;
  /**
   * Decoder request offset should not be less than MIN timing offset value.
   */
    dec_req_offset = ( dec_req_offset >= VS_VOC_MIN_TIMING_OFFSET_V )? dec_req_offset : (dec_req_offset + VS_VOC_MIN_TIMING_OFFSET_V);
  }

  { /* Decoder offset calculation. */
    dec_offset = ( dec_req_offset + VS_VOC_TIMING_DEC_OFFSET_MARGIN_V ) % vfr_cycle_duration_us;
    
  /**
   * Decoder offset should not be less than MIN timing offset value.
   */
    dec_offset = ( dec_offset >= VS_VOC_MIN_TIMING_OFFSET_V )? dec_offset : (dec_offset + VS_VOC_MIN_TIMING_OFFSET_V);
  }

  /* Populate the evaluated timing params. */
  session_obj->vocoder.timing_config.enc_offset_us = enc_offset;
  session_obj->vocoder.timing_config.dec_req_offset_us = dec_req_offset;
  session_obj->vocoder.timing_config.dec_offset_us = dec_offset;

  MSG_3( MSG_SSID_DFLT, MSG_LEGACY_MED,         
         "VS: TIMING_OFFSETS: enc_offset:(%d) dec_req_offset:(%d) dec_offset:(%d)",
         enc_offset, dec_req_offset, dec_offset );

  return VS_EOK;
}


uint32_t vs_voc_action_set_voice_timing_references (
  vs_session_object_t* session_obj
)
{
  uint32_t rc = VS_EOK;
  vs_simple_job_object_t* job_obj = NULL;
  gpr_cmd_alloc_ext_t timing_params;
  gpr_packet_t* packet = NULL;
  apm_cmd_header_t *cmd_header;
  apm_module_param_data_t *param_data;
  vcpm_param_timing_references_t *reference_params;

  rc = vs_create_simple_job_object( session_obj->header.handle, &job_obj );
  job_obj->fn_table[ VS_RESPONSE_FN_ENUM_RESULT ] = vs_voc_transition_result_rsp_fn;
  session_obj->session_ctrl.transition_job_handle = job_obj->header.handle;

  timing_params.src_domain_id = GPR_IDS_DOMAIN_ID_MODEM_V ;
  timing_params.src_port = VS_MODULE_INSTANCE_ID;
  timing_params.dst_domain_id = GPR_IDS_DOMAIN_ID_ADSP_V ; 
  timing_params.dst_port = VCPM_MODULE_INSTANCE_ID;
  timing_params.token = job_obj->header.handle;
  timing_params.opcode = APM_CMD_SET_CFG;
  timing_params.payload_size = sizeof(apm_cmd_header_t) + sizeof(apm_module_param_data_t) + 
                             sizeof(vcpm_param_timing_references_t) ;
  timing_params.client_data = 0;
  timing_params.ret_packet =&packet ;
  
  rc = __gpr_cmd_alloc_ext(&timing_params);
  
  if( packet == NULL )
  {
    VS_REPORT_FATAL_ON_ERROR( rc );
    return VS_EFAILED;
  }

  cmd_header = GPR_PKT_GET_PAYLOAD( apm_cmd_header_t, packet );
  cmd_header->mem_map_handle = 0;
  cmd_header->payload_address_lsw = 0;
  cmd_header->payload_address_msw = 0;
  cmd_header->payload_size = sizeof(apm_module_param_data_t) + sizeof(vcpm_param_timing_references_t) ;
  
  param_data = (apm_module_param_data_t*)( (uint8_t*)cmd_header + sizeof(apm_cmd_header_t) );
  param_data->module_instance_id = VCPM_MODULE_INSTANCE_ID;
  param_data->param_id = VCPM_PARAM_ID_TIMING_REFERENCES;
  param_data->param_size = sizeof(vcpm_param_timing_references_t) ;

  reference_params =  (vcpm_param_timing_references_t*)( (uint8_t*)param_data + sizeof(apm_module_param_data_t) );
  reference_params->vfr_mode = session_obj->vocoder.vfr_config.vfr_mode;
  reference_params->vsid = session_obj->vsid;
  reference_params->tx_ref_timstamp_us = session_obj->vocoder.timing_config.tx_ref_timestamp_us ;
  reference_params->rx_ref_timstamp_us = session_obj->vocoder.timing_config.rx_ref_timestamp_us ;
  reference_params->vfr_cycle_us = session_obj->vocoder.vfr_config.vfr_cycle_duration_ms*1000;

  MSG_6( MSG_SSID_DFLT, MSG_LEGACY_MED,
         "VS: TIMING_REFERENCES: handle[0x%08x], vsid[0x%08x], vfr_mode[%d], tx_ref_timstamp_us[0x%08x],rx_ref_timstamp_us[0x%08x], vfr_cycle_us[0x%08x]",
          job_obj->header.handle, reference_params->vsid, 
          reference_params->vfr_mode, reference_params->tx_ref_timstamp_us, reference_params->rx_ref_timstamp_us, reference_params->vfr_cycle_us);

  rc = __gpr_cmd_async_send(packet);

  if ( rc )
  {
    VS_REPORT_FATAL_ON_ERROR(rc);
    session_obj->session_ctrl.status |= rc;
    ( void ) vs_mem_free_object( ( vs_object_t* ) job_obj );
    ( void )__gpr_cmd_free( packet);
    return VS_EIMMEDIATE;
  }

  session_obj->cmd_ctrl.cmd_cnt++;
  return rc;
}

uint32_t vs_voc_action_set_voice_timing_references_v2 (
  vs_session_object_t* session_obj
)
{
  uint32_t rc = VS_EOK;
  vs_simple_job_object_t* job_obj = NULL;
  gpr_cmd_alloc_ext_t timing_params;
  gpr_packet_t* packet = NULL;
  apm_cmd_header_t *cmd_header;
  apm_module_param_data_t *param_data;
  vcpm_param_timing_references_v2_t *reference_params;

  rc = vs_create_simple_job_object( session_obj->header.handle, &job_obj );
  job_obj->fn_table[ VS_RESPONSE_FN_ENUM_RESULT ] = vs_voc_transition_result_rsp_fn;
  session_obj->session_ctrl.transition_job_handle = job_obj->header.handle;

  timing_params.src_domain_id = GPR_IDS_DOMAIN_ID_MODEM_V ;
  timing_params.src_port = VS_MODULE_INSTANCE_ID;
  timing_params.dst_domain_id = GPR_IDS_DOMAIN_ID_ADSP_V ; 
  timing_params.dst_port = VCPM_MODULE_INSTANCE_ID;
  timing_params.token = job_obj->header.handle;
  timing_params.opcode = APM_CMD_SET_CFG;
  timing_params.payload_size = sizeof(apm_cmd_header_t) + sizeof(apm_module_param_data_t) + 
                             sizeof(vcpm_param_timing_references_v2_t) ;
  timing_params.client_data = 0;
  timing_params.ret_packet =&packet ;
  
  rc = __gpr_cmd_alloc_ext(&timing_params);
  
  if( packet == NULL )
  {
    VS_REPORT_FATAL_ON_ERROR( rc );
    return VS_EFAILED;
  }

  cmd_header = GPR_PKT_GET_PAYLOAD( apm_cmd_header_t, packet );
  cmd_header->mem_map_handle = 0;
  cmd_header->payload_address_lsw = 0;
  cmd_header->payload_address_msw = 0;
  cmd_header->payload_size = sizeof(apm_module_param_data_t) + sizeof(vcpm_param_timing_references_v2_t) ;
  
  param_data = (apm_module_param_data_t*)( (uint8_t*)cmd_header + sizeof(apm_cmd_header_t) );
  param_data->module_instance_id = VCPM_MODULE_INSTANCE_ID;
  param_data->param_id = VCPM_PARAM_ID_TIMING_REFERENCES_V2;
  param_data->param_size = sizeof(vcpm_param_timing_references_v2_t) ;

  reference_params =  (vcpm_param_timing_references_v2_t*)( (uint8_t*)param_data + sizeof(apm_module_param_data_t) );
  reference_params->vfr_mode = session_obj->vocoder.vfr_config.vfr_mode;
  reference_params->vsid = session_obj->vsid;
  reference_params->tx_ref_timstamp_us = session_obj->vocoder.timing_config.tx_ref_timestamp_us ;
  reference_params->rx_ref_timstamp_us = session_obj->vocoder.timing_config.rx_ref_timestamp_us ;
  reference_params->vfr_cycle_us = session_obj->vocoder.vfr_config.vfr_cycle_duration_ms*1000;
  reference_params->is_frame_type_event_enabled = session_obj->vocoder.pktexg_event_enable;
  reference_params->frame_type_ref_timestamp_us = session_obj->vocoder.timing_config.frame_type_ref_timestamp_us;
  reference_params->enc_packet_ready_margin_us = session_obj->vocoder.timing_config.enc_packet_ready_margin_us;

  MSG_6( MSG_SSID_DFLT, MSG_LEGACY_MED,
         "VS: TIMING_REFERENCES_V2: handle[0x%08x], vsid[0x%08x], vfr_mode[%d], tx_ref_timstamp_us[0x%08x],rx_ref_timstamp_us[0x%08x], vfr_cycle_us[0x%08x]",
          job_obj->header.handle, reference_params->vsid, 
          reference_params->vfr_mode, reference_params->tx_ref_timstamp_us, reference_params->rx_ref_timstamp_us, reference_params->vfr_cycle_us);

  MSG_3( MSG_SSID_DFLT, MSG_LEGACY_MED,
         "VS: TIMING_REFERENCES_V2: Frame_type_event_enabled: [%d], frame_type_ref_timestamp_us[0x%08x],enc_packet_ready_margin_us %d",
          reference_params->is_frame_type_event_enabled, reference_params->frame_type_ref_timestamp_us, reference_params->enc_packet_ready_margin_us);

  rc = __gpr_cmd_async_send(packet);

  if ( rc )
  {
    VS_REPORT_FATAL_ON_ERROR(rc);
    session_obj->session_ctrl.status |= rc;
    ( void ) vs_mem_free_object( ( vs_object_t* ) job_obj );
    ( void )__gpr_cmd_free( packet);
    return VS_EIMMEDIATE;
  }

  session_obj->cmd_ctrl.cmd_cnt++;
  return rc;
}

uint32_t vs_voc_action_set_voice_timeline_resync (
  vs_session_object_t* session_obj
)
{
  uint32_t rc = VS_EOK;
  vs_simple_job_object_t* job_obj = NULL;
  gpr_cmd_alloc_ext_t timing_params;
  gpr_packet_t* packet = NULL;
  apm_cmd_header_t *cmd_header;
  apm_module_param_data_t *param_data;
  vcpm_param_id_timeline_resync_t *resync_params;

  rc = vs_create_simple_job_object( session_obj->header.handle, &job_obj );
  job_obj->fn_table[ VS_RESPONSE_FN_ENUM_RESULT ] = vs_voc_transition_result_rsp_fn;
  session_obj->session_ctrl.transition_job_handle = job_obj->header.handle;

  timing_params.src_domain_id = GPR_IDS_DOMAIN_ID_MODEM_V ;
  timing_params.src_port = VS_MODULE_INSTANCE_ID;
  timing_params.dst_domain_id = GPR_IDS_DOMAIN_ID_ADSP_V ; 
  timing_params.dst_port = VCPM_MODULE_INSTANCE_ID;
  timing_params.token = job_obj->header.handle;
  timing_params.opcode = APM_CMD_SET_CFG;
  timing_params.payload_size = sizeof(apm_cmd_header_t) + sizeof(apm_module_param_data_t) + 
                             sizeof(vcpm_param_id_timeline_resync_t) ;
  timing_params.client_data = 0;
  timing_params.ret_packet =&packet ;
  
  rc = __gpr_cmd_alloc_ext(&timing_params);
  
  if( packet == NULL )
  {
    VS_REPORT_FATAL_ON_ERROR( rc );
    return VS_EFAILED;
  }

  cmd_header = GPR_PKT_GET_PAYLOAD( apm_cmd_header_t, packet );
  cmd_header->mem_map_handle = 0;
  cmd_header->payload_address_lsw = 0;
  cmd_header->payload_address_msw = 0;
  cmd_header->payload_size = sizeof(apm_module_param_data_t) + sizeof(vcpm_param_id_timeline_resync_t) ;
  
  param_data = (apm_module_param_data_t*)( (uint8_t*)cmd_header + sizeof(apm_cmd_header_t) );
  param_data->module_instance_id = VCPM_MODULE_INSTANCE_ID;
  param_data->param_id = VCPM_PARAM_ID_TIMELINE_RESYNC;
  param_data->param_size = sizeof(vcpm_param_id_timeline_resync_t) ;

  resync_params =  (vcpm_param_id_timeline_resync_t*)( (uint8_t*)param_data + sizeof(apm_module_param_data_t) );
  resync_params->resync_type = session_obj->vocoder.timing_config.resync_type;
  resync_params->vfr_mode = session_obj->vocoder.vfr_config.vfr_mode;
  resync_params->vsid = session_obj->vsid;
  resync_params->tx_ref_timestamp_us = session_obj->vocoder.timing_config.tx_ref_timestamp_us ;
  resync_params->rx_ref_timestamp_us = session_obj->vocoder.timing_config.rx_ref_timestamp_us ;
  resync_params->vfr_cycle_us = session_obj->vocoder.vfr_config.vfr_cycle_duration_ms*1000;
  resync_params->is_frame_type_event_enabled = session_obj->vocoder.pktexg_event_enable;
  resync_params->frame_type_ref_timestamp_us = session_obj->vocoder.timing_config.frame_type_ref_timestamp_us;
  resync_params->enc_packet_ready_margin_us = session_obj->vocoder.timing_config.enc_packet_ready_margin_us;
  resync_params->network_id = vs_voc_get_vcpm_network_id(session_obj->vocoder.client_id);

  MSG_6( MSG_SSID_DFLT, MSG_LEGACY_MED,
         "VS: TIMING_REFERENCES_V2: handle[0x%08x], vsid[0x%08x], vfr_mode[%d], tx_ref_timstamp_us[0x%08x],rx_ref_timstamp_us[0x%08x], vfr_cycle_us[0x%08x]",
          job_obj->header.handle, resync_params->vsid, 
          resync_params->vfr_mode, resync_params->tx_ref_timestamp_us, resync_params->rx_ref_timestamp_us, resync_params->vfr_cycle_us);

  MSG_3( MSG_SSID_DFLT, MSG_LEGACY_MED,
         "VS: TIMING_REFERENCES_V2: Frame_type_event_enabled: [%d], frame_type_ref_timestamp_us[0x%08x],enc_packet_ready_margin_us %d",
          resync_params->is_frame_type_event_enabled, resync_params->frame_type_ref_timestamp_us, resync_params->enc_packet_ready_margin_us);

  rc = __gpr_cmd_async_send(packet);

  if ( rc )
  {
    VS_REPORT_FATAL_ON_ERROR(rc);
    session_obj->session_ctrl.status |= rc;
    ( void ) vs_mem_free_object( ( vs_object_t* ) job_obj );
    ( void )__gpr_cmd_free( packet);
    return VS_EIMMEDIATE;
  }

  session_obj->cmd_ctrl.cmd_cnt++;
  return rc;
}

uint32_t vs_voc_action_send_resync_tx_ctm (
  vs_session_object_t* session_obj
)
{
  int32_t rc = VS_EOK;
  vs_simple_job_object_t* job_obj = NULL;
  gpr_cmd_alloc_ext_t ctm_params;
  gpr_packet_t* packet = NULL;
  apm_cmd_header_t *cmd_header;
  apm_module_param_data_t *param_data;

  if ( VS_VOICE_MOD_DEFAULT_IID ==  session_obj->mod_iid_tag_info[VS_MOD_TX_CTMTTY].mod_iid )
  {
    MSG_1( MSG_SSID_DFLT, MSG_LEGACY_HIGH,
           "VS:vs_voc_action_send_resync_tx_ctm(): Returning..., TTY module instance id 0x%08x ", session_obj->mod_iid_tag_info[VS_MOD_TX_CTMTTY].mod_iid );
    session_obj->session_ctrl.status = VS_EOK;
    return VS_EIMMEDIATE;
  }

  rc = vs_create_simple_job_object( session_obj->header.handle, &job_obj );
  job_obj->fn_table[ VS_RESPONSE_FN_ENUM_RESULT ] = vs_voc_transition_result_rsp_fn;
  session_obj->session_ctrl.transition_job_handle = job_obj->header.handle;

  ctm_params.src_domain_id = GPR_IDS_DOMAIN_ID_MODEM_V;
  ctm_params.src_port = VS_MODULE_INSTANCE_ID ;
  ctm_params.dst_domain_id = GPR_IDS_DOMAIN_ID_ADSP_V;
  ctm_params.dst_port = session_obj->mod_iid_tag_info[VS_MOD_TX_CTMTTY].mod_iid;
  ctm_params.token = job_obj->header.handle;
  ctm_params.opcode = APM_CMD_SET_CFG;
  ctm_params.payload_size = sizeof(apm_cmd_header_t) + sizeof(apm_module_param_data_t) ;
  ctm_params.client_data = 0;
  ctm_params.ret_packet =&packet ;

  rc = __gpr_cmd_alloc_ext(&ctm_params);

  if( packet == NULL )
  {
    VS_REPORT_FATAL_ON_ERROR( rc );
    return VS_EFAILED;
  }

  cmd_header = GPR_PKT_GET_PAYLOAD( apm_cmd_header_t, packet );
  cmd_header->mem_map_handle = 0;
  cmd_header->payload_address_lsw = 0;
  cmd_header->payload_address_msw = 0;
  cmd_header->payload_size = sizeof(apm_module_param_data_t);

  param_data = (apm_module_param_data_t*)( (uint8_t*)cmd_header + sizeof(apm_cmd_header_t) );
  param_data->module_instance_id = session_obj->mod_iid_tag_info[VS_MOD_TX_CTMTTY].mod_iid;
  param_data->param_id = PARAM_ID_CTM_RESYNC;
  param_data->param_size = 0;

  rc = __gpr_cmd_async_send(packet);
  if ( rc )
  {
    VS_REPORT_FATAL_ON_ERROR(rc);
    session_obj->session_ctrl.status |= rc;
    ( void ) vs_mem_free_object( ( vs_object_t* ) job_obj );
    ( void )__gpr_cmd_free( packet);
    return VS_EIMMEDIATE;
  }


  MSG( MSG_SSID_DFLT, MSG_LEGACY_HIGH,
          "VS:vs_voc_action_send_resync_tx_ctm(): Sent resync for TX CTM module" );

  session_obj->cmd_ctrl.cmd_cnt++;
  return rc;
}

uint32_t vs_voc_action_send_resync_rx_ctm (
  vs_session_object_t* session_obj
)
{
  int32_t rc = VS_EOK;
  vs_simple_job_object_t* job_obj = NULL;
  gpr_cmd_alloc_ext_t ctm_params;
  gpr_packet_t* packet = NULL;
  apm_cmd_header_t *cmd_header;
  apm_module_param_data_t *param_data;

  if ( VS_VOICE_MOD_DEFAULT_IID ==  session_obj->mod_iid_tag_info[VS_MOD_RX_CTMTTY].mod_iid )
  {
    MSG_1( MSG_SSID_DFLT, MSG_LEGACY_HIGH,
           "VS:vs_voc_action_send_resync_rx_ctm(): Returning..., TTY module instance id 0x%08x ", session_obj->mod_iid_tag_info[VS_MOD_RX_CTMTTY].mod_iid );
    session_obj->session_ctrl.status = VS_EOK;
    return VS_EIMMEDIATE;
  }

  rc = vs_create_simple_job_object( session_obj->header.handle, &job_obj );
  job_obj->fn_table[ VS_RESPONSE_FN_ENUM_RESULT ] = vs_voc_transition_result_rsp_fn;
  session_obj->session_ctrl.transition_job_handle = job_obj->header.handle;

  ctm_params.src_domain_id = GPR_IDS_DOMAIN_ID_MODEM_V;
  ctm_params.src_port = VS_MODULE_INSTANCE_ID ;
  ctm_params.dst_domain_id = GPR_IDS_DOMAIN_ID_ADSP_V;
  ctm_params.dst_port = session_obj->mod_iid_tag_info[VS_MOD_RX_CTMTTY].mod_iid;
  ctm_params.token = job_obj->header.handle;
  ctm_params.opcode = APM_CMD_SET_CFG;
  ctm_params.payload_size = sizeof(apm_cmd_header_t) + sizeof(apm_module_param_data_t) ;
  ctm_params.client_data = 0;
  ctm_params.ret_packet =&packet ;

  rc = __gpr_cmd_alloc_ext(&ctm_params);

  if( packet == NULL )
  {
    VS_REPORT_FATAL_ON_ERROR( rc );
    return VS_EFAILED;
  }

  cmd_header = GPR_PKT_GET_PAYLOAD( apm_cmd_header_t, packet );
  cmd_header->mem_map_handle = 0;
  cmd_header->payload_address_lsw = 0;
  cmd_header->payload_address_msw = 0;
  cmd_header->payload_size = sizeof(apm_module_param_data_t);

  param_data = (apm_module_param_data_t*)( (uint8_t*)cmd_header + sizeof(apm_cmd_header_t) );
  param_data->module_instance_id = session_obj->mod_iid_tag_info[VS_MOD_RX_CTMTTY].mod_iid;
  param_data->param_id = PARAM_ID_CTM_RESYNC;
  param_data->param_size = 0;

  rc = __gpr_cmd_async_send(packet);

  if ( rc )
  {
    VS_REPORT_FATAL_ON_ERROR(rc);
    session_obj->session_ctrl.status |= rc;
    ( void ) vs_mem_free_object( ( vs_object_t* ) job_obj );
    ( void )__gpr_cmd_free( packet);
    return VS_EIMMEDIATE;
  }

  MSG( MSG_SSID_DFLT, MSG_LEGACY_HIGH,
          "VS:vs_voc_action_send_resync_rx_ctm(): Sent resync for RX CTM module" );

  session_obj->cmd_ctrl.cmd_cnt++;
  return rc;
}

#if !defined ( MCPM_VSTMR_STUB ) && !defined ( WINSIM ) && !defined ( AVS_MPSS_TEST )
uint32_t vs_check_vfr_allocated(vstmr_vfr_e vfr_id)
{
  uint32_t num_alloc = 0;
  vstmr_int_types_e int_type = (vfr_id ? VSTMR_INT_VFR_SEC: VSTMR_INT_VFR_PRI);
  vstmr_get_comparator_usage( int_type , &num_alloc, NULL);
  return num_alloc;
}
#endif

uint32_t vs_voc_action_configure_vfr (
  vs_session_object_t* session_obj
)
{
#if !defined ( MCPM_VSTMR_STUB ) && !defined ( WINSIM ) && !defined ( AVS_MPSS_TEST )
  vstmr_vfr_e vfr_id = VSTMR_VFR_AUTO;
  vstmr_vfr_e vfr_id_allocated = VSTMR_VFR_AUTO;
  uint64_t vfr_xo_cnt = 0;
    uint32_t vfr_offset_us, txref_offset_us, adjusted_vfr_offset_us;
    uint32_t vfr_cycle_duration_us;
    uint64_t current_xo_timestamp_us;
#endif

  if ( VS_VOC_VFR_MODE_HARD_V == session_obj->vocoder.vfr_config.vfr_mode )
  {
    if ( ( VS_COMMON_NETWORK_ID_LTE == (session_obj->vocoder.client_id) )   ||
         ( VS_COMMON_NETWORK_ID_WLAN == (session_obj->vocoder.client_id) )  ||
         ( VS_COMMON_NETWORK_ID_NR == (session_obj->vocoder.client_id) )    ||
         ( VS_COMMON_NETWORK_ID_CIWLAN == (session_obj->vocoder.client_id) )
       )
	{
	  MSG_1( MSG_SSID_DFLT, MSG_LEGACY_HIGH,
	           "vs_voc_action_configure_vfr() Calling MCPM_Config_Modem(VSTMR_START) and vstmr_xo_vfr_allocate(vsid=0x%08x)",
	           session_obj->vsid );
	}
	else
	{
	  return VS_EIMMEDIATE;
	}
  }
  else
  {
    return VS_EIMMEDIATE;
  }
  
#if !defined ( MCPM_VSTMR_STUB ) && !defined ( WINSIM ) && !defined ( AVS_MPSS_TEST )
    MCPM_Config_Modem(MCPM_LPASS_START_REQ, NULL);
  vfr_id = vs_voc_get_vfr_id( session_obj->vsid );
   if(vs_check_vfr_allocated(vfr_id))
   {
      MSG_1( MSG_SSID_DFLT, MSG_LEGACY_ERROR,
      "vs_voc_action_configure_vfr(): vs_voc_action_configure_vfr failed. vfr_id(=%d) not available", vfr_id );
       session_obj->vocoder.timing_config.vfr_start_timestamp_us = VSTMR_XO_TO_US( VSTMR_XO_READ_FULL() );
       return VS_EIMMEDIATE;
   }
  vfr_id_allocated = vstmr_xo_vfr_allocate (  vfr_id );
  if ( vfr_id != vfr_id_allocated )
  {
    session_obj->session_ctrl.status = VS_EFAILED;
      MSG_2( MSG_SSID_DFLT, MSG_LEGACY_ERROR,
            "vs_voc_action_configure_vfr(): vstmr_xo_vfr_allocate(vfr_id=%d) failed. vfr_id_allocated(=%d)", vfr_id, vfr_id_allocated );
    return VS_EFAILED;
  }

    if ( TRUE == session_obj->vocoder.timing_config.is_timing_ref_available )
    {
      vfr_cycle_duration_us = session_obj->vocoder.vfr_config.vfr_cycle_duration_ms * 1000;
  
     /* Reading current XO TS */
     current_xo_timestamp_us = VSTMR_XO_TO_US( VSTMR_XO_READ_FULL() );
  
     /* calculating vfr_offset with respect to vfr cycle duration */
     vfr_offset_us = ( current_xo_timestamp_us ) % ( vfr_cycle_duration_us );
  
     /* Calculating txref_offset with respect to vfr cycle duration. */
     txref_offset_us =  ( session_obj->vocoder.timing_config.tx_ref_timestamp_us ) % ( vfr_cycle_duration_us );
  
     /* Align vfr timestamp to "txref_offset + 500us". 500us margin is added to account for the latency
      * involved in programming VSTMR and VSTMR firing the VFR interrupt.
      */
     if ( vfr_offset_us > txref_offset_us )
     {
        session_obj->vocoder.timing_config.vfr_start_timestamp_us = 
        current_xo_timestamp_us + (vfr_cycle_duration_us - (vfr_offset_us - txref_offset_us)) + VS_VOC_TIMING_VFR_START_MARGIN_IN_US;
     }
     else if ( vfr_offset_us < txref_offset_us)
     {
        session_obj->vocoder.timing_config.vfr_start_timestamp_us = 
        current_xo_timestamp_us + (txref_offset_us - vfr_offset_us) + VS_VOC_TIMING_VFR_START_MARGIN_IN_US;
     }
     else
     {
        session_obj->vocoder.timing_config.vfr_start_timestamp_us = current_xo_timestamp_us + VS_VOC_TIMING_VFR_START_MARGIN_IN_US;
     }
  
     vfr_xo_cnt = VSTMR_US_TO_XO (session_obj->vocoder.timing_config.vfr_start_timestamp_us );
    vstmr_xo_vfr_start( vfr_id, vfr_xo_cnt,
                         VSTMR_MS_TO_XO( session_obj->vocoder.vfr_config.vfr_cycle_duration_ms ) );
    session_obj->is_vfr_allocated = TRUE;
  
     /* calculate adjusted vfr offset for debugging purposes */
     adjusted_vfr_offset_us = ( session_obj->vocoder.timing_config.vfr_start_timestamp_us ) % ( vfr_cycle_duration_us );
  
      MSG_4( MSG_SSID_DFLT, MSG_LEGACY_HIGH,
           "VS: vs_voc_action_configure_vfr(): vfr_cycle_duration_us(=%d), txref_offset_us(=%d), vfr_offset_us(=%d), adjusted_vfr_offset_us(=%d)", 
            vfr_cycle_duration_us, txref_offset_us, vfr_offset_us, adjusted_vfr_offset_us  );

      MSG_3( MSG_SSID_DFLT, MSG_LEGACY_HIGH,
             "VS: vs_voc_action_configure_vfr(): current_xo_timestamp_us(=%lu), tx_ref_timestamp_us(=%lu), calcualted vfr_timestamp_us(=%lu)", 
             current_xo_timestamp_us, session_obj->vocoder.timing_config.tx_ref_timestamp_us, session_obj->vocoder.timing_config.vfr_start_timestamp_us );
    }
    else
    {
      /* Reading the current XO TS and adding margin (0.5ms) to derive the VFR Start TS. */
  session_obj->vocoder.timing_config.vfr_start_timestamp_us = VSTMR_XO_TO_US( VSTMR_XO_READ_FULL() ) + VS_VOC_TIMING_VFR_START_MARGIN_IN_US;
  vfr_xo_cnt = VSTMR_US_TO_XO (session_obj->vocoder.timing_config.vfr_start_timestamp_us );
  vstmr_xo_vfr_start( vfr_id, vfr_xo_cnt,
                      VSTMR_MS_TO_XO( session_obj->vocoder.vfr_config.vfr_cycle_duration_ms ) );
      session_obj->is_vfr_allocated = TRUE;    
      MSG_1( MSG_SSID_DFLT, MSG_LEGACY_HIGH,
            "VS: vs_voc_action_configure_vfr(): vfr_timestamp_us(=%lu)", session_obj->vocoder.timing_config.vfr_start_timestamp_us );
    }
#endif /* #if !defined ( MCPM_VSTMR_STUB ) && !defined ( WINSIM ) && !defined ( AVS_MPSS_TEST ) */

  return VS_EIMMEDIATE;

}

uint32_t vs_voc_action_release_vfr (
  vs_session_object_t* session_obj
)
{
#if !defined ( MCPM_VSTMR_STUB ) && !defined ( WINSIM ) && !defined ( AVS_MPSS_TEST )
  vstmr_vfr_e vfr_id = VSTMR_VFR_AUTO;
  vfr_id = vs_voc_get_vfr_id( session_obj->vsid );
  
  if( TRUE == session_obj->is_vfr_allocated )
  {
    vstmr_xo_vfr_release( vfr_id ); 
    MCPM_Config_Modem( MCPM_LPASS_STOP_REQ, NULL );
    session_obj->is_vfr_allocated = FALSE;
    MSG_4( MSG_SSID_DFLT, MSG_LEGACY_HIGH,
            "vs_voc_action_release_vfr() Called vstmr_xo_vfr_release. vsid=(0x%08x), vfr_mode=(0x%08x), client_id=(0x%08x), is_vfr_allocated=(%d)", 
             session_obj->vsid, session_obj->vocoder.vfr_config.vfr_mode, session_obj->vocoder.client_id, session_obj->is_vfr_allocated );
  }
#endif  

  return VS_EIMMEDIATE;
}

uint32_t vs_voc_action_send_prepare_voice (
  vs_session_object_t* session_obj
)
{
  int32_t rc = VS_EOK;
  vs_simple_job_object_t* job_obj = NULL;
  gpr_cmd_alloc_ext_t start_params;
  gpr_packet_t* packet = NULL;
  apm_cmd_header_t* cmd_header = NULL;
  apm_module_param_data_t* param_data = NULL;
  apm_param_id_sub_graph_list_t* num_sgs = NULL;
  uint32_t* sg_list = NULL;
  uint32_t param_size = sizeof(apm_param_id_sub_graph_list_t) + ((session_obj->num_sub_graphs)*sizeof(apm_sub_graph_id_t));

  rc = vs_create_simple_job_object( session_obj->header.handle, &job_obj );
  job_obj->fn_table[ VS_RESPONSE_FN_ENUM_RESULT ] = vs_voc_transition_result_rsp_fn;
  session_obj->session_ctrl.transition_job_handle = job_obj->header.handle;

  start_params.src_domain_id = GPR_IDS_DOMAIN_ID_MODEM_V ;
  start_params.src_port = VS_MODULE_INSTANCE_ID;
  start_params.dst_domain_id = GPR_IDS_DOMAIN_ID_ADSP_V ; 
  start_params.dst_port = VCPM_MODULE_INSTANCE_ID ;
  start_params.token = job_obj->header.handle;
  start_params.opcode = APM_CMD_GRAPH_PREPARE;
  start_params.payload_size = sizeof(apm_cmd_header_t) + sizeof(apm_module_param_data_t) + param_size;
  start_params.client_data = 0;
  start_params.ret_packet =&packet ;

  rc = __gpr_cmd_alloc_ext(&start_params);

  if( packet == NULL )
  {
    VS_REPORT_FATAL_ON_ERROR( rc );
    return VS_EFAILED;
  }

  cmd_header = GPR_PKT_GET_PAYLOAD( apm_cmd_header_t, packet );
  cmd_header->mem_map_handle = 0;
  cmd_header->payload_address_lsw = 0;
  cmd_header->payload_address_msw = 0;
  cmd_header->payload_size = sizeof(apm_module_param_data_t) + param_size;

  param_data = (apm_module_param_data_t*)( (uint8_t*)cmd_header + sizeof(apm_cmd_header_t) );
  param_data->module_instance_id = VCPM_MODULE_INSTANCE_ID;
  param_data->param_id = APM_PARAM_ID_SUB_GRAPH_LIST;
  param_data->param_size = param_size;

  num_sgs = (apm_param_id_sub_graph_list_t*)((uint8_t*)param_data + sizeof(apm_module_param_data_t));
  num_sgs->num_sub_graphs = session_obj->num_sub_graphs;
  
  sg_list = (uint32_t*)((uint8_t*)num_sgs + sizeof(uint32_t));

  for ( int index = 0; index< session_obj->num_sub_graphs ; index++)
  {
    *sg_list = session_obj->subgraph_info[index].sub_graph_id;
    sg_list++;
  }

  rc = __gpr_cmd_async_send(packet);

  if ( rc )
  {
    VS_REPORT_FATAL_ON_ERROR(rc);
    session_obj->session_ctrl.status |= rc;
    ( void ) vs_mem_free_object( ( vs_object_t* ) job_obj );
    ( void )__gpr_cmd_free( packet);
    return VS_EIMMEDIATE;
  }

  session_obj->cmd_ctrl.cmd_cnt ++;
  return rc;
}

uint32_t vs_voc_action_send_start_voice (
  vs_session_object_t* session_obj
)
{
  int32_t rc = VS_EOK;
  vs_simple_job_object_t* job_obj = NULL;
  gpr_cmd_alloc_ext_t start_params;
  gpr_packet_t* packet = NULL;
  apm_cmd_header_t* cmd_header = NULL;
  apm_module_param_data_t* param_data = NULL;
  apm_param_id_sub_graph_list_t* num_sgs = NULL;
  uint32_t* sg_list = NULL;
  uint32_t param_size = sizeof(apm_param_id_sub_graph_list_t) + ((session_obj->num_sub_graphs)*sizeof(apm_sub_graph_id_t));

  rc = vs_create_simple_job_object( session_obj->header.handle, &job_obj );
  job_obj->fn_table[ VS_RESPONSE_FN_ENUM_RESULT ] = vs_voc_start_voice_result_rsp_fn;
  session_obj->session_ctrl.transition_job_handle = job_obj->header.handle;

  start_params.src_domain_id = GPR_IDS_DOMAIN_ID_MODEM_V ;
  start_params.src_port = VS_MODULE_INSTANCE_ID;
  start_params.dst_domain_id = GPR_IDS_DOMAIN_ID_ADSP_V ; 
  start_params.dst_port = VCPM_MODULE_INSTANCE_ID ;
  start_params.token = job_obj->header.handle;
  start_params.opcode = APM_CMD_GRAPH_START;
  start_params.payload_size = sizeof(apm_cmd_header_t) + sizeof(apm_module_param_data_t) + param_size;
  start_params.client_data = 0;
  start_params.ret_packet =&packet ;

  rc = __gpr_cmd_alloc_ext(&start_params);

  if( packet == NULL )
  {
    VS_REPORT_FATAL_ON_ERROR( rc );
    return VS_EFAILED;
  }

  cmd_header = GPR_PKT_GET_PAYLOAD( apm_cmd_header_t, packet );
  cmd_header->mem_map_handle = 0;
  cmd_header->payload_address_lsw = 0;
  cmd_header->payload_address_msw = 0;
  cmd_header->payload_size = sizeof(apm_module_param_data_t) + param_size;

  param_data = (apm_module_param_data_t*)( (uint8_t*)cmd_header + sizeof(apm_cmd_header_t) );
  param_data->module_instance_id = VCPM_MODULE_INSTANCE_ID;
  param_data->param_id = APM_PARAM_ID_SUB_GRAPH_LIST;
  param_data->param_size = param_size;

  num_sgs = (apm_param_id_sub_graph_list_t*)((uint8_t*)param_data + sizeof(apm_module_param_data_t));
  num_sgs->num_sub_graphs = session_obj->num_sub_graphs;
  
  sg_list = (uint32_t*)((uint8_t*)num_sgs + sizeof(uint32_t));

  for ( int index = 0; index< session_obj->num_sub_graphs ; index++)
  {
    if( session_obj->subgraph_info[index].is_subgraph_started == FALSE)
    {
      *sg_list = session_obj->subgraph_info[index].sub_graph_id;
       session_obj->subgraph_info[index].is_subgraph_started = TRUE;
    }
    sg_list++;
  }

  rc = __gpr_cmd_async_send(packet);

  if ( rc )
  {
    VS_REPORT_FATAL_ON_ERROR(rc);
    session_obj->session_ctrl.status |= rc;
    ( void ) vs_mem_free_object( ( vs_object_t* ) job_obj );
    ( void )__gpr_cmd_free( packet);
    return VS_EIMMEDIATE;
  }

  session_obj->cmd_ctrl.cmd_cnt ++;
  return rc;
}

uint32_t vs_voc_action_send_stop_voice (
  vs_session_object_t* session_obj
)
{
  int32_t rc = VS_EOK;
  vs_simple_job_object_t* job_obj;
  gpr_cmd_alloc_ext_t stop_params;
  gpr_packet_t* packet = NULL;
  apm_cmd_header_t* cmd_header = NULL;
  apm_module_param_data_t* param_data = NULL;
  apm_param_id_sub_graph_list_t* num_sgs = NULL;
  uint32_t* sg_list = NULL;
  uint32_t param_size = sizeof(apm_param_id_sub_graph_list_t) + ((session_obj->num_sub_graphs)*sizeof(apm_sub_graph_id_t));

  rc = vs_create_simple_job_object( session_obj->header.handle, &job_obj );
  job_obj->fn_table[ VS_RESPONSE_FN_ENUM_RESULT ] = vs_voc_stop_voice_result_rsp_fn;
  session_obj->session_ctrl.transition_job_handle = job_obj->header.handle;


  stop_params.src_domain_id = GPR_IDS_DOMAIN_ID_MODEM_V ;
  stop_params.src_port = VS_MODULE_INSTANCE_ID;
  stop_params.dst_domain_id = GPR_IDS_DOMAIN_ID_ADSP_V ; 
  stop_params.dst_port = VCPM_MODULE_INSTANCE_ID ;
  stop_params.token = job_obj->header.handle;
  stop_params.opcode = APM_CMD_GRAPH_STOP;
  stop_params.payload_size = sizeof(apm_cmd_header_t) + sizeof(apm_module_param_data_t) + param_size;
  stop_params.client_data = 0;
  stop_params.ret_packet =&packet ;

  rc = __gpr_cmd_alloc_ext(&stop_params);

  if( packet == NULL )
  {
    VS_REPORT_FATAL_ON_ERROR( rc );
    return VS_EFAILED;
  }

  cmd_header = GPR_PKT_GET_PAYLOAD( apm_cmd_header_t, packet );
  cmd_header->mem_map_handle = 0;
  cmd_header->payload_address_lsw = 0;
  cmd_header->payload_address_msw = 0;
  cmd_header->payload_size = sizeof(apm_module_param_data_t) + param_size;

  param_data = (apm_module_param_data_t*)( (uint8_t*)cmd_header + sizeof(apm_cmd_header_t) );
  param_data->module_instance_id = VCPM_MODULE_INSTANCE_ID;
  param_data->param_id = APM_PARAM_ID_SUB_GRAPH_LIST;
  param_data->param_size = param_size;

  num_sgs = (apm_param_id_sub_graph_list_t*)((uint8_t*)param_data + sizeof(apm_module_param_data_t));
  num_sgs->num_sub_graphs = session_obj->num_sub_graphs;
  
  sg_list = (uint32_t*)((uint8_t*)num_sgs + sizeof(uint32_t));

  for ( int index = 0; index< session_obj->num_sub_graphs ; index++)
  {
    if( session_obj->subgraph_info[index].is_subgraph_started == TRUE)
    {
      *sg_list = session_obj->subgraph_info[index].sub_graph_id;
       session_obj->subgraph_info[index].is_subgraph_started = FALSE;
    }
    sg_list++;
  }

  rc = __gpr_cmd_async_send(packet);

  if ( rc )
  {
    VS_REPORT_FATAL_ON_ERROR(rc);
    session_obj->session_ctrl.status |= rc;
    ( void ) vs_mem_free_object( ( vs_object_t* ) job_obj );
    ( void )__gpr_cmd_free( packet);
    return VS_EIMMEDIATE;
  }

  MSG( MSG_SSID_DFLT, MSG_LEGACY_HIGH,"VS: vs_voc_action_send_stop_voice(): sent stop voice");
  session_obj->cmd_ctrl.cmd_cnt ++;
  return rc;
}

 uint32_t vs_voc_action_set_encoder_module_id ( 
  vs_session_object_t* session_obj
)
{
   uint32_t rc = VS_EOK;
   gpr_cmd_alloc_ext_t module_Params;
   gpr_packet_t* packet = NULL;
   apm_cmd_header_t *cmd_header;
   apm_module_param_data_t *param_data;
   param_id_placeholder_real_module_id_t *module_ID;
   vs_simple_job_object_t* job_obj;

   rc = vs_create_simple_job_object( session_obj->header.handle, &job_obj );
   session_obj->session_ctrl.transition_job_handle = job_obj->header.handle;
   job_obj->fn_table[ VS_RESPONSE_FN_ENUM_RESULT] = vs_voc_transition_result_rsp_fn; 
 
   //Set module ID on encoder module
   module_Params.src_domain_id = GPR_IDS_DOMAIN_ID_MODEM_V ;
   module_Params.src_port = VS_MODULE_INSTANCE_ID;
   module_Params.dst_domain_id = GPR_IDS_DOMAIN_ID_ADSP_V ; 
   module_Params.dst_port = session_obj->mod_iid_tag_info[VS_MOD_ENCODER].mod_iid ;
   module_Params.token = job_obj->header.handle;
   module_Params.opcode = APM_CMD_SET_CFG;
   module_Params.payload_size = sizeof(apm_cmd_header_t) + sizeof(apm_module_param_data_t) +
                              sizeof(param_id_placeholder_real_module_id_t);
   module_Params.client_data = 0;
   module_Params.ret_packet =&packet ;
   
   rc = __gpr_cmd_alloc_ext(&module_Params);
   
   if( packet == NULL )
   {
	 VS_REPORT_FATAL_ON_ERROR( rc );
	 return VS_EFAILED;
   }
   
   cmd_header = GPR_PKT_GET_PAYLOAD( apm_cmd_header_t, packet );
   cmd_header->mem_map_handle = 0;
   cmd_header->payload_address_lsw = 0;
   cmd_header->payload_address_msw = 0;
   cmd_header->payload_size = sizeof(apm_module_param_data_t) + sizeof(param_id_encoder_output_config_t) ;
   
   param_data = (apm_module_param_data_t*)( (uint8_t*)cmd_header + sizeof(apm_cmd_header_t) );
   param_data->module_instance_id = session_obj->mod_iid_tag_info[VS_MOD_ENCODER].mod_iid;
   param_data->param_id = PARAM_ID_REAL_MODULE_ID;
   param_data->param_size = sizeof(param_id_placeholder_real_module_id_t) ;
   
   module_ID = (param_id_placeholder_real_module_id_t*)( (uint8_t*)param_data + sizeof(apm_module_param_data_t) );
   module_ID->real_module_id = session_obj->vocoder.enc_module_id;
 
   rc = __gpr_cmd_async_send(packet);
   
   if ( rc )
   {
    VS_REPORT_FATAL_ON_ERROR(rc);
    session_obj->session_ctrl.status |= rc;
    ( void ) vs_mem_free_object( ( vs_object_t* ) job_obj );
    ( void )__gpr_cmd_free( packet);
    return VS_EIMMEDIATE;
   }

   session_obj->cmd_ctrl.cmd_cnt ++;
   return rc;
}

 uint32_t vs_voc_action_set_decoder_module_id ( 
   vs_session_object_t* session_obj
 )
 {
   uint32_t rc = VS_EOK;
   gpr_cmd_alloc_ext_t module_Params;
   gpr_packet_t* packet = NULL;
   apm_cmd_header_t *cmd_header;
   apm_module_param_data_t *param_data;
   param_id_placeholder_real_module_id_t *module_ID;
   vs_simple_job_object_t* job_obj;

   rc = vs_create_simple_job_object( session_obj->header.handle, &job_obj );
   session_obj->session_ctrl.transition_job_handle = job_obj->header.handle;
   job_obj->fn_table[ VS_RESPONSE_FN_ENUM_RESULT] = vs_voc_transition_result_rsp_fn; 
 
   //Set module ID on decoder module
   module_Params.src_domain_id = GPR_IDS_DOMAIN_ID_MODEM_V ;
   module_Params.src_port = VS_MODULE_INSTANCE_ID;
   module_Params.dst_domain_id = GPR_IDS_DOMAIN_ID_ADSP_V ; 
   module_Params.dst_port = session_obj->mod_iid_tag_info[VS_MOD_DECODER].mod_iid ;
   module_Params.token = job_obj->header.handle;
   module_Params.opcode = APM_CMD_SET_CFG;
   module_Params.payload_size = sizeof(apm_cmd_header_t) + sizeof(apm_module_param_data_t) +
                              sizeof(param_id_placeholder_real_module_id_t);
   module_Params.client_data = 0;
   module_Params.ret_packet =&packet ;
   
   rc = __gpr_cmd_alloc_ext(&module_Params);
   
   if( packet == NULL )
   {
     VS_REPORT_FATAL_ON_ERROR( rc );
     return VS_EFAILED;
   }
   
   cmd_header = GPR_PKT_GET_PAYLOAD( apm_cmd_header_t, packet );
   cmd_header->mem_map_handle = 0;
   cmd_header->payload_address_lsw = 0;
   cmd_header->payload_address_msw = 0;
   cmd_header->payload_size = sizeof(apm_module_param_data_t) + sizeof(param_id_encoder_output_config_t) ;
   
   param_data = (apm_module_param_data_t*)( (uint8_t*)cmd_header + sizeof(apm_cmd_header_t) );
   param_data->module_instance_id = session_obj->mod_iid_tag_info[VS_MOD_DECODER].mod_iid;
   param_data->param_id = PARAM_ID_REAL_MODULE_ID;
   param_data->param_size = sizeof(param_id_placeholder_real_module_id_t) ;
   
   module_ID = (param_id_placeholder_real_module_id_t*)( (uint8_t*)param_data + sizeof(apm_module_param_data_t) );
   module_ID->real_module_id = session_obj->vocoder.dec_module_id;
 
   rc = __gpr_cmd_async_send(packet);
   
   if ( rc )
   {
     VS_REPORT_FATAL_ON_ERROR(rc);
     session_obj->session_ctrl.status |= rc;
    ( void ) vs_mem_free_object( ( vs_object_t* ) job_obj );
    ( void )__gpr_cmd_free( packet);
    return VS_EIMMEDIATE;
   }

   session_obj->cmd_ctrl.cmd_cnt ++;
   return rc;
 }

uint32_t vs_voc_action_reset_encoder_module_id ( 
   vs_session_object_t* session_obj
 )
 {
    uint32_t rc = VS_EOK;
    gpr_cmd_alloc_ext_t module_Params;
    gpr_packet_t* packet = NULL;
    apm_cmd_header_t *cmd_header;
    apm_module_param_data_t *param_data;
    vs_simple_job_object_t* job_obj;
 
    rc = vs_create_simple_job_object( session_obj->header.handle, &job_obj );
    session_obj->session_ctrl.transition_job_handle = job_obj->header.handle;
    job_obj->fn_table[ VS_RESPONSE_FN_ENUM_RESULT] = vs_voc_transition_result_rsp_fn; 
  
    //Reset module ID on encoder module
    module_Params.src_domain_id = GPR_IDS_DOMAIN_ID_MODEM_V ;
    module_Params.src_port = VS_MODULE_INSTANCE_ID;
    module_Params.dst_domain_id = GPR_IDS_DOMAIN_ID_ADSP_V ; 
    module_Params.dst_port = session_obj->mod_iid_tag_info[VS_MOD_ENCODER].mod_iid ;
    module_Params.token = job_obj->header.handle;
    module_Params.opcode = APM_CMD_SET_CFG;
    module_Params.payload_size = sizeof(apm_cmd_header_t) + sizeof(apm_module_param_data_t) ;
    module_Params.client_data = 0;
    module_Params.ret_packet =&packet ;

   rc = __gpr_cmd_alloc_ext(&module_Params);

   if( packet == NULL )
   {
      VS_REPORT_FATAL_ON_ERROR( rc );
      return VS_EFAILED;
   }

    cmd_header = GPR_PKT_GET_PAYLOAD( apm_cmd_header_t, packet );
    cmd_header->mem_map_handle = 0;
    cmd_header->payload_address_lsw = 0;
    cmd_header->payload_address_msw = 0;
    cmd_header->payload_size = sizeof(apm_module_param_data_t);

    param_data = (apm_module_param_data_t*)( (uint8_t*)cmd_header + sizeof(apm_cmd_header_t) );
    param_data->module_instance_id = session_obj->mod_iid_tag_info[VS_MOD_ENCODER].mod_iid;
    param_data->param_id = PARAM_ID_RESET_PLACEHOLDER_MODULE;
    param_data->param_size = 0 ;

    rc = __gpr_cmd_async_send(packet);

    if ( rc )
    {
     VS_REPORT_FATAL_ON_ERROR(rc);
     session_obj->session_ctrl.status |= rc;
     ( void ) vs_mem_free_object( ( vs_object_t* ) job_obj );
     ( void )__gpr_cmd_free( packet);
      return VS_EIMMEDIATE;
    }

    session_obj->cmd_ctrl.cmd_cnt ++;
    MSG( MSG_SSID_DFLT, MSG_LEGACY_MED, "VS: vs_voc_action_reset_encoder_module_id(): resetting encoder module ID ");
    return rc;
 }

uint32_t vs_voc_action_reset_decoder_module_id ( 
vs_session_object_t* session_obj
  )
  {
     uint32_t rc = VS_EOK;
     gpr_cmd_alloc_ext_t module_Params;
     gpr_packet_t* packet = NULL;
     apm_cmd_header_t *cmd_header;
     apm_module_param_data_t *param_data;
     param_id_placeholder_real_module_id_t *module_ID;
     vs_simple_job_object_t* job_obj;

     rc = vs_create_simple_job_object( session_obj->header.handle, &job_obj );
     session_obj->session_ctrl.transition_job_handle = job_obj->header.handle;
     job_obj->fn_table[ VS_RESPONSE_FN_ENUM_RESULT] = vs_voc_transition_result_rsp_fn; 

     //Reset module ID on decoder module
     module_Params.src_domain_id = GPR_IDS_DOMAIN_ID_MODEM_V ;
     module_Params.src_port = VS_MODULE_INSTANCE_ID;
     module_Params.dst_domain_id = GPR_IDS_DOMAIN_ID_ADSP_V ; 
     module_Params.dst_port = session_obj->mod_iid_tag_info[VS_MOD_DECODER].mod_iid ;
     module_Params.token = job_obj->header.handle;
     module_Params.opcode = APM_CMD_SET_CFG;
     module_Params.payload_size = sizeof(apm_cmd_header_t) + sizeof(apm_module_param_data_t) ;
     module_Params.client_data = 0;
     module_Params.ret_packet =&packet ;

    rc = __gpr_cmd_alloc_ext(&module_Params);
 
    if( packet == NULL )
    {
      VS_REPORT_FATAL_ON_ERROR( rc );
      return VS_EFAILED;
    }

    cmd_header = GPR_PKT_GET_PAYLOAD( apm_cmd_header_t, packet );
    cmd_header->mem_map_handle = 0;
    cmd_header->payload_address_lsw = 0;
    cmd_header->payload_address_msw = 0;
    cmd_header->payload_size = sizeof(apm_module_param_data_t);
 
     param_data = (apm_module_param_data_t*)( (uint8_t*)cmd_header + sizeof(apm_cmd_header_t) );
     param_data->module_instance_id = session_obj->mod_iid_tag_info[VS_MOD_DECODER].mod_iid;
     param_data->param_id = PARAM_ID_RESET_PLACEHOLDER_MODULE;
     param_data->param_size = 0 ;

     rc = __gpr_cmd_async_send(packet);
 
    if ( rc )
    {
      VS_REPORT_FATAL_ON_ERROR(rc);
      session_obj->session_ctrl.status |= rc;
      ( void ) vs_mem_free_object( ( vs_object_t* ) job_obj );
      ( void )__gpr_cmd_free( packet);
      return VS_EIMMEDIATE;
    }
 
     session_obj->cmd_ctrl.cmd_cnt ++;
     MSG( MSG_SSID_DFLT, MSG_LEGACY_MED, "VS: vs_voc_action_reset_decoder_module_id(): resetting decoder module ID ");
     return rc;
  }

 uint32_t vs_voc_action_set_encoder_media_fmt ( 
   vs_session_object_t* session_obj
 )
 {
   uint32_t rc = VS_EOK;
   gpr_cmd_alloc_ext_t media_Params;
   gpr_packet_t* packet = NULL;
   apm_cmd_header_t *cmd_header;
   apm_module_param_data_t *param_data;
   param_id_encoder_output_config_t *media_ID;
   vs_simple_job_object_t* job_obj;

   rc = vs_create_simple_job_object( session_obj->header.handle, &job_obj );
   session_obj->session_ctrl.transition_job_handle = job_obj->header.handle;
   job_obj->fn_table[ VS_RESPONSE_FN_ENUM_RESULT] = vs_voc_transition_result_rsp_fn; 
   
   //Set media ID on encoder module
   media_Params.src_domain_id = GPR_IDS_DOMAIN_ID_MODEM_V ;
   media_Params.src_port = VS_MODULE_INSTANCE_ID;
   media_Params.dst_domain_id = GPR_IDS_DOMAIN_ID_ADSP_V ; 
   media_Params.dst_port = session_obj->mod_iid_tag_info[VS_MOD_ENCODER].mod_iid ;
   media_Params.token = job_obj->header.handle;
   media_Params.opcode = APM_CMD_SET_CFG;
   media_Params.payload_size = sizeof(apm_cmd_header_t) + sizeof(apm_module_param_data_t) +
                              sizeof(param_id_encoder_output_config_t);
   media_Params.client_data = 0;
   media_Params.ret_packet =&packet ;
   
   rc = __gpr_cmd_alloc_ext(&media_Params);
   
   if( packet == NULL )
   {
	 VS_REPORT_FATAL_ON_ERROR( rc );
	 return VS_EFAILED;
   }
   
   cmd_header = GPR_PKT_GET_PAYLOAD( apm_cmd_header_t, packet );
   cmd_header->mem_map_handle = 0;
   cmd_header->payload_address_lsw = 0;
   cmd_header->payload_address_msw = 0;
   cmd_header->payload_size = sizeof(apm_module_param_data_t) + sizeof(param_id_encoder_output_config_t) ;
   
   param_data = (apm_module_param_data_t*)( (uint8_t*)cmd_header + sizeof(apm_cmd_header_t) );
   param_data->module_instance_id = session_obj->mod_iid_tag_info[VS_MOD_ENCODER].mod_iid;
   param_data->param_id = PARAM_ID_ENCODER_OUTPUT_CONFIG;
   param_data->param_size = sizeof(param_id_encoder_output_config_t) ;
   
   media_ID = (param_id_encoder_output_config_t*)( (uint8_t*)param_data + sizeof(apm_module_param_data_t) );
   media_ID->data_format = DATA_FORMAT_FIXED_POINT;
   
   if( VS_VOCAMRWB_2_MEDIA_ID == session_obj->vocoder.media_id )
   {
     media_ID->fmt_id = MEDIA_FMT_ID_AMRWB; //For Hybrid vocoder, encoder and decoder are different.
   }
   else
   {
     media_ID->fmt_id = session_obj->vocoder.media_id;
   }
   media_ID->payload_size = 0;
 
   rc = __gpr_cmd_async_send(packet);
   
   if ( rc )
   {
     VS_REPORT_FATAL_ON_ERROR(rc);
     session_obj->session_ctrl.status |= rc;
     ( void ) vs_mem_free_object( ( vs_object_t* ) job_obj );
     ( void )__gpr_cmd_free( packet);
     return VS_EIMMEDIATE;
   }

   session_obj->cmd_ctrl.cmd_cnt ++;
   return rc;
 }


 uint32_t vs_voc_action_set_decoder_media_fmt ( 
  vs_session_object_t* session_obj
)
{
  uint32_t rc = VS_EOK;
  gpr_cmd_alloc_ext_t media_params;
  gpr_packet_t* packet = NULL;
  apm_cmd_header_t *cmd_header;
  apm_module_param_data_t *param_data;
  media_format_t *media_ID;
  vs_simple_job_object_t* job_obj;
  payload_media_fmt_pcm_t* pcm_format;
  
  rc = vs_create_simple_job_object( session_obj->header.handle, &job_obj );
  session_obj->session_ctrl.transition_job_handle = job_obj->header.handle;
  job_obj->fn_table[ VS_RESPONSE_FN_ENUM_RESULT] = vs_voc_transition_result_rsp_fn; 

  //Set media ID on encoder module
  media_params.src_domain_id = GPR_IDS_DOMAIN_ID_MODEM_V ;
  media_params.src_port = VS_MODULE_INSTANCE_ID;
  media_params.dst_domain_id = GPR_IDS_DOMAIN_ID_ADSP_V ; 
  media_params.dst_port = session_obj->mod_iid_tag_info[VS_MOD_RX_EP].mod_iid ;
  media_params.token = job_obj->header.handle;
  media_params.opcode = APM_CMD_SET_CFG;
  media_params.payload_size = sizeof(apm_cmd_header_t) + sizeof(apm_module_param_data_t) + 
                             sizeof(media_format_t) + sizeof(payload_media_fmt_pcm_t);
  media_params.client_data = 0;
  media_params.ret_packet =&packet ;

  rc = __gpr_cmd_alloc_ext(&media_params);

  if( packet == NULL )
  {
    VS_REPORT_FATAL_ON_ERROR( rc );
    return VS_EFAILED;
  }

  cmd_header = GPR_PKT_GET_PAYLOAD( apm_cmd_header_t, packet );
  cmd_header->mem_map_handle = 0;
  cmd_header->payload_address_lsw = 0;
  cmd_header->payload_address_msw = 0;
  cmd_header->payload_size = sizeof(apm_module_param_data_t) + sizeof(media_format_t) + sizeof(payload_media_fmt_pcm_t);

  param_data = (apm_module_param_data_t*)( (uint8_t*)cmd_header + sizeof(apm_cmd_header_t) );
  param_data->module_instance_id = session_obj->mod_iid_tag_info[VS_MOD_RX_EP].mod_iid;
  param_data->param_id = PARAM_ID_MEDIA_FORMAT;
  param_data->param_size = sizeof(media_format_t) + sizeof(payload_media_fmt_pcm_t) ;

  media_ID = (media_format_t*)( (uint8_t*)param_data + sizeof(apm_module_param_data_t) );
  media_ID->data_format = 6; //DATA_FORMAT_RAW_COMPRESSED

  if( VS_VOCAMRWB_2_MEDIA_ID == session_obj->vocoder.media_id )
  {
     media_ID->fmt_id = MEDIA_FMT_ID_EVS; //For Hybrid vocoder, encoder and decoder are different.
  }
  else
  {
    media_ID->fmt_id = session_obj->vocoder.media_id;
  }

  media_ID->payload_size =  sizeof(payload_media_fmt_pcm_t);

  pcm_format = (payload_media_fmt_pcm_t*)( (uint8_t*)media_ID + sizeof(media_format_t) );
  pcm_format->sample_rate = 16000;
  pcm_format->bit_width = 16;
  pcm_format->alignment = 1;
  pcm_format->bits_per_sample = 16;
  pcm_format->q_factor = 15;
  pcm_format->endianness = 1;
  pcm_format->num_channels = 1;

  rc = __gpr_cmd_async_send(packet);

  if ( rc )
  {
     VS_REPORT_FATAL_ON_ERROR(rc);
     session_obj->session_ctrl.status |= rc;
     ( void ) vs_mem_free_object( ( vs_object_t* ) job_obj );
     ( void )__gpr_cmd_free( packet);
     return VS_EIMMEDIATE;
  }

  session_obj->cmd_ctrl.cmd_cnt ++;
  return rc;
}

uint32_t vs_voc_action_set_plc ( 
  vs_session_object_t* session_obj
)
{
  uint32_t rc =VS_EOK;
  gpr_cmd_alloc_ext_t plc_params;
  gpr_packet_t* packet = NULL;
  apm_cmd_header_t *cmd_header;
  apm_module_param_data_t *param_data;
  param_id_g711_dec_plc_mode_t *plc;

  if( VS_PARAM_UINT8_NOT_SET == session_obj->vocoder.plc )
  {
    return VS_EOK;
  }

  plc_params.src_domain_id = GPR_IDS_DOMAIN_ID_MODEM_V ;
  plc_params.src_port = VS_MODULE_INSTANCE_ID;
  plc_params.dst_domain_id = GPR_IDS_DOMAIN_ID_ADSP_V ; 
  plc_params.dst_port = session_obj->mod_iid_tag_info[VS_MOD_DECODER].mod_iid ;
  plc_params.token = session_obj->header.handle;
  plc_params.opcode = APM_CMD_SET_CFG;
  plc_params.payload_size = sizeof(apm_cmd_header_t) + sizeof(apm_module_param_data_t) + sizeof(param_id_g711_dec_plc_mode_t);
  plc_params.client_data = 0;
  plc_params.ret_packet =&packet ;

  rc = __gpr_cmd_alloc_ext(&plc_params);

  if( packet == NULL )
  {
    VS_REPORT_FATAL_ON_ERROR( rc );
    return VS_EFAILED;
  }

  cmd_header = GPR_PKT_GET_PAYLOAD( apm_cmd_header_t, packet );
  cmd_header->mem_map_handle = 0;
  cmd_header->payload_address_lsw = 0;
  cmd_header->payload_address_msw = 0;
  cmd_header->payload_size = sizeof(apm_module_param_data_t) + sizeof(param_id_g711_dec_plc_mode_t);

  param_data = (apm_module_param_data_t*)( (uint8_t*)cmd_header + sizeof(apm_cmd_header_t) );
  param_data->module_instance_id = session_obj->mod_iid_tag_info[VS_MOD_DECODER].mod_iid;
  param_data->param_id = PARAM_ID_G711_DEC_PLC_MODE;
  param_data->param_size = sizeof(param_id_g711_dec_plc_mode_t);

  plc = (param_id_g711_dec_plc_mode_t*)( (uint8_t*)param_data + sizeof(apm_module_param_data_t) );
  plc->plc_mode = session_obj->vocoder.plc;

  MSG_1( MSG_SSID_DFLT, MSG_LEGACY_MED, "VS: vs_voc_action_set_plc(): PLC mode [%d] is set ", plc->plc_mode );

  rc = __gpr_cmd_async_send( packet );
  if (rc)
  {
    VS_REPORT_FATAL_ON_ERROR(rc);
    MSG_1( MSG_SSID_DFLT, MSG_LEGACY_ERROR,
           "VS:vs_voc_action_set_plc: Failed to communicate with decoder module while setting PLC: rc=0x%08x", rc );
    ( void ) __gpr_cmd_free( packet );
  }

  return VS_EOK;
}

uint32_t vs_voc_action_set_evs_dtx ( 
  vs_session_object_t* session_obj
)
{
  uint32_t rc =VS_EOK;
  gpr_cmd_alloc_ext_t dtx_params;
  gpr_packet_t* packet = NULL;
  apm_cmd_header_t *cmd_header;
  apm_module_param_data_t *param_data;
  param_id_evs_enc_dtx_mode_t *dtx;

  if( VS_PARAM_UINT32_NOT_SET == session_obj->vocoder.dtx )
  {
    return VS_EOK;
  }

  dtx_params.src_domain_id = GPR_IDS_DOMAIN_ID_MODEM_V ;
  dtx_params.src_port = VS_MODULE_INSTANCE_ID;
  dtx_params.dst_domain_id = GPR_IDS_DOMAIN_ID_ADSP_V ; 
  dtx_params.dst_port = session_obj->mod_iid_tag_info[VS_MOD_ENCODER].mod_iid ;
  dtx_params.token = session_obj->header.handle;
  dtx_params.opcode = APM_CMD_SET_CFG;
  dtx_params.payload_size = sizeof(apm_cmd_header_t) + sizeof(apm_module_param_data_t) + sizeof(param_id_evs_enc_dtx_mode_t);
  dtx_params.client_data = 0;
  dtx_params.ret_packet =&packet ;

  rc = __gpr_cmd_alloc_ext(&dtx_params);

  if( packet == NULL )
  {
    VS_REPORT_FATAL_ON_ERROR( rc );
    return VS_EFAILED;
  }

  cmd_header = GPR_PKT_GET_PAYLOAD( apm_cmd_header_t, packet );
  cmd_header->mem_map_handle = 0;
  cmd_header->payload_address_lsw = 0;
  cmd_header->payload_address_msw = 0;
  cmd_header->payload_size = sizeof(apm_module_param_data_t) + sizeof(param_id_evs_enc_dtx_mode_t);

  param_data = (apm_module_param_data_t*)( (uint8_t*)cmd_header + sizeof(apm_cmd_header_t) );
  param_data->module_instance_id = session_obj->mod_iid_tag_info[VS_MOD_ENCODER].mod_iid;
  param_data->param_id = PARAM_ID_EVS_ENC_DTX_MODE;
  param_data->param_size = sizeof(param_id_evs_enc_dtx_mode_t);

  dtx = (param_id_evs_enc_dtx_mode_t*)( (uint8_t*)param_data + sizeof(apm_module_param_data_t) );
  if( VS_CLIENT_STATE_ENABLED == session_obj->hpcm.client_state )
  {
    dtx->dtx_mode = FALSE;
  }
  else
  {
    dtx->dtx_mode = session_obj->vocoder.dtx;
  }

  MSG_1( MSG_SSID_DFLT, MSG_LEGACY_MED, "VS: vs_voc_action_set_dtx(): DTX mode[%d] is set", dtx->dtx_mode );

  rc = __gpr_cmd_async_send(packet);

  if ( rc )
  {
    VS_REPORT_FATAL_ON_ERROR(rc);
    MSG_1( MSG_SSID_DFLT, MSG_LEGACY_ERROR,
           "VS:vs_voc_action_set_dtx(): Failed to communicate with encoder module while setting DTX mode: rc=0x%08x", rc );
    ( void )__gpr_cmd_free(packet);
  }

  return VS_EOK;
}

uint32_t vs_voc_action_set_dtx_v2 ( 
  vs_session_object_t* session_obj
)
{
  uint32_t rc =VS_EOK;
  gpr_cmd_alloc_ext_t dtx_params;
  gpr_packet_t* packet = NULL;
  apm_cmd_header_t *cmd_header;
  apm_module_param_data_t *param_data;
  param_id_vocoder_enable_dtx_mode_t *dtx;

  if( VS_PARAM_UINT32_NOT_SET == session_obj->vocoder.dtx )
  {
    return VS_EOK;
  }

  dtx_params.src_domain_id = GPR_IDS_DOMAIN_ID_MODEM_V ;
  dtx_params.src_port = VS_MODULE_INSTANCE_ID;
  dtx_params.dst_domain_id = GPR_IDS_DOMAIN_ID_ADSP_V ; 
  dtx_params.dst_port = session_obj->mod_iid_tag_info[VS_MOD_ENCODER].mod_iid ;
  dtx_params.token = session_obj->header.handle;
  dtx_params.opcode = APM_CMD_SET_CFG;
  dtx_params.payload_size = sizeof(apm_cmd_header_t) + sizeof(apm_module_param_data_t) + sizeof(param_id_vocoder_enable_dtx_mode_t);
  dtx_params.client_data = 0;
  dtx_params.ret_packet =&packet ;

  rc = __gpr_cmd_alloc_ext(&dtx_params);

  if( packet == NULL )
  {
    VS_REPORT_FATAL_ON_ERROR( rc );
    return VS_EFAILED;
  }

  cmd_header = GPR_PKT_GET_PAYLOAD( apm_cmd_header_t, packet );
  cmd_header->mem_map_handle = 0;
  cmd_header->payload_address_lsw = 0;
  cmd_header->payload_address_msw = 0;
  cmd_header->payload_size = sizeof(apm_module_param_data_t) + sizeof(param_id_vocoder_enable_dtx_mode_t);

  param_data = (apm_module_param_data_t*)( (uint8_t*)cmd_header + sizeof(apm_cmd_header_t) );
  param_data->module_instance_id = session_obj->mod_iid_tag_info[VS_MOD_ENCODER].mod_iid;
  param_data->param_id = PARAM_ID_VOCODER_ENABLE_DTX_MODE;
  param_data->param_size = sizeof(param_id_vocoder_enable_dtx_mode_t);

  dtx = (param_id_vocoder_enable_dtx_mode_t*)( (uint8_t*)param_data + sizeof(apm_module_param_data_t) );
  
  if( VS_CLIENT_STATE_ENABLED == session_obj->hpcm.client_state )
  {
    dtx->enable = FALSE;
  }
  else
  {
    dtx->enable = session_obj->vocoder.dtx;
  }

  MSG_1( MSG_SSID_DFLT, MSG_LEGACY_MED, "VS: vs_voc_action_set_dtx(): DTX mode[%d] is set", dtx->enable );

  rc = __gpr_cmd_async_send(packet);

  if ( rc )
  {
    VS_REPORT_FATAL_ON_ERROR(rc);
    MSG_1( MSG_SSID_DFLT, MSG_LEGACY_ERROR,
           "VS:vs_voc_action_set_dtx(): Failed to communicate with encoder module while setting DTX mode: rc=0x%08x", rc );
    ( void )__gpr_cmd_free(packet);
  }

  return VS_EOK;
}

uint32_t vs_voc_action_set_enc_rate ( 
  vs_session_object_t* session_obj
)
{
  uint32_t rc = VS_EOK;
  gpr_cmd_alloc_ext_t rateParams;
  gpr_packet_t* packet = NULL;
  apm_cmd_header_t *cmd_header;
  apm_module_param_data_t *param_data;
  param_id_enc_bitrate_param_t *enc_rate;
  uint32_t paramSize = sizeof(param_id_enc_bitrate_param_t);


  if( VS_PARAM_UINT8_NOT_SET == session_obj->vocoder.encoder_rate ) return VS_EOK;

  rateParams.src_domain_id = GPR_IDS_DOMAIN_ID_MODEM_V ;
  rateParams.src_port = VS_MODULE_INSTANCE_ID;
  rateParams.dst_domain_id = GPR_IDS_DOMAIN_ID_ADSP_V ;
  rateParams.dst_port = session_obj->mod_iid_tag_info[VS_MOD_ENCODER].mod_iid ;
  rateParams.token = session_obj->header.handle;
  rateParams.opcode = APM_CMD_SET_CFG;
  rateParams.payload_size = sizeof(apm_cmd_header_t) + sizeof(apm_module_param_data_t) + paramSize;
  rateParams.client_data = 0;
  rateParams.ret_packet =&packet ;

  rc = __gpr_cmd_alloc_ext(&rateParams);

  if( packet == NULL )
  {
    VS_REPORT_FATAL_ON_ERROR( rc );
    return VS_EFAILED;
  }

  cmd_header = GPR_PKT_GET_PAYLOAD( apm_cmd_header_t, packet );
  cmd_header->mem_map_handle = 0;
  cmd_header->payload_address_lsw = 0;
  cmd_header->payload_address_msw = 0;
  cmd_header->payload_size = sizeof(apm_module_param_data_t) + paramSize;

  param_data = (apm_module_param_data_t*)( (uint8_t*)cmd_header + sizeof(apm_cmd_header_t) );
  param_data->module_instance_id = session_obj->mod_iid_tag_info[VS_MOD_ENCODER].mod_iid;
  param_data->param_id = PARAM_ID_ENC_BITRATE;
  param_data->param_size = paramSize;

  enc_rate = (param_id_enc_bitrate_param_t*)( (uint8_t*)param_data + sizeof(apm_module_param_data_t) );
  enc_rate->bitrate = session_obj->vocoder.encoder_rate ;

  rc = __gpr_cmd_async_send(packet);

  if ( rc )
  {
    MSG_1( MSG_SSID_DFLT, MSG_LEGACY_ERROR,
           "vs_voc_action_set_enc_rate(): Failed to communicate with encoder module: rc=0x%08x", rc );
    (void)__gpr_cmd_free(packet);
  }

  MSG_1( MSG_SSID_DFLT, MSG_LEGACY_MED, "VS: vs_voc_action_set_enc_rate(): enc rate[0x%08x] is set", enc_rate->bitrate );
  return VS_EOK;
}

uint32_t vs_voc_action_set_min_max_rate ( 
  vs_session_object_t* session_obj
)
{
  uint32_t rc = VS_EOK;
  gpr_cmd_alloc_ext_t minmax_params;
  gpr_packet_t* packet = NULL;
  apm_cmd_header_t *cmd_header = NULL;
  apm_module_param_data_t *param_data = NULL;
  param_id_vocoder_minmax_rate_param_t *minmax_rate = NULL;

  if( ( VS_PARAM_UINT8_NOT_SET == session_obj->vocoder.min_rate ) ||
      ( VS_PARAM_UINT8_NOT_APPLICABLE == session_obj->vocoder.min_rate ) )
  {
    return VS_EOK;
  }

  minmax_params.src_domain_id = GPR_IDS_DOMAIN_ID_MODEM_V ;
  minmax_params.src_port = VS_MODULE_INSTANCE_ID;
  minmax_params.dst_domain_id = GPR_IDS_DOMAIN_ID_ADSP_V ; 
  minmax_params.dst_port = session_obj->mod_iid_tag_info[VS_MOD_ENCODER].mod_iid ;
  minmax_params.token = session_obj->header.handle;
  minmax_params.opcode = APM_CMD_SET_CFG;
  minmax_params.payload_size = sizeof(apm_cmd_header_t) + sizeof(apm_module_param_data_t) + sizeof(param_id_vocoder_minmax_rate_param_t);
  minmax_params.client_data = 0;
  minmax_params.ret_packet =&packet ;

  rc = __gpr_cmd_alloc_ext(&minmax_params);

  if( packet == NULL )
  {
    VS_REPORT_FATAL_ON_ERROR( rc );
    return VS_EFAILED;
  }

  cmd_header = GPR_PKT_GET_PAYLOAD( apm_cmd_header_t, packet );
  cmd_header->mem_map_handle = 0;
  cmd_header->payload_address_lsw = 0;
  cmd_header->payload_address_msw = 0;
  cmd_header->payload_size = sizeof(apm_module_param_data_t) + sizeof(param_id_vocoder_minmax_rate_param_t);

  param_data = (apm_module_param_data_t*)( (uint8_t*)cmd_header + sizeof(apm_cmd_header_t) );
  param_data->module_instance_id = session_obj->mod_iid_tag_info[VS_MOD_ENCODER].mod_iid ;
  param_data->param_id = PARAM_ID_VOCODER_MINMAX_RATE;
  param_data->param_size = sizeof(param_id_vocoder_minmax_rate_param_t);

  minmax_rate = (param_id_vocoder_minmax_rate_param_t*)( (uint8_t*)param_data + sizeof(apm_module_param_data_t) );
  minmax_rate->min_rate = session_obj->vocoder.min_rate;
  minmax_rate->max_rate = session_obj->vocoder.max_rate;

  rc = __gpr_cmd_async_send(packet);

  if ( rc )
  {
    VS_REPORT_FATAL_ON_ERROR(rc);
    MSG_1( MSG_SSID_DFLT, MSG_LEGACY_ERROR,
           "VS:VOCAMR: Failed to communicate with encoder module: rc=0x%08x", rc );
    ( void )__gpr_cmd_free(packet);
  }

  return rc;
}


uint32_t vs_voc_action_set_max_sample_rate ( 
   vs_session_object_t* session_obj
)
{
  uint32_t rc = VS_EOK;
  vs_simple_job_object_t* job_obj;
  gpr_cmd_alloc_ext_t rate_params;
  gpr_packet_t* packet = NULL;
  apm_cmd_header_t *cmd_header;
  apm_module_param_data_t *param_data;
  param_id_evs_dec_out_sampling_rate_t *sampling_rate;
  
  if ( ( MEDIA_FMT_ID_EVS != session_obj->vocoder.media_id ) && 
       ( VS_VOCAMRWB_2_MEDIA_ID != session_obj->vocoder.media_id ) )
  {
    session_obj->session_ctrl.status = VS_EOK;
    return VS_EIMMEDIATE;
  }

  rc = vs_create_simple_job_object( session_obj->header.handle, &job_obj );
  job_obj->fn_table[ VS_RESPONSE_FN_ENUM_RESULT ] = vs_voc_transition_result_rsp_fn;
  session_obj->session_ctrl.transition_job_handle = job_obj->header.handle;

  //Set media ID on encoder module
  rate_params.src_domain_id = GPR_IDS_DOMAIN_ID_MODEM_V ;
  rate_params.src_port = VS_MODULE_INSTANCE_ID;
  rate_params.dst_domain_id = GPR_IDS_DOMAIN_ID_ADSP_V ; 
  rate_params.dst_port = session_obj->mod_iid_tag_info[VS_MOD_DECODER].mod_iid ;
  rate_params.token = job_obj->header.handle;
  rate_params.opcode = APM_CMD_SET_CFG;
  rate_params.payload_size = sizeof(apm_cmd_header_t) + sizeof(apm_module_param_data_t) + sizeof(param_id_evs_dec_out_sampling_rate_t);
  rate_params.client_data = 0;
  rate_params.ret_packet =&packet ;

  rc = __gpr_cmd_alloc_ext(&rate_params);

  if( packet == NULL )
  {
    VS_REPORT_FATAL_ON_ERROR( rc );
    return VS_EFAILED;
  }

  cmd_header = GPR_PKT_GET_PAYLOAD( apm_cmd_header_t, packet );
  cmd_header->mem_map_handle = 0;
  cmd_header->payload_address_lsw = 0;
  cmd_header->payload_address_msw = 0;
  cmd_header->payload_size = sizeof(apm_module_param_data_t) + sizeof(param_id_evs_dec_out_sampling_rate_t) ;

  param_data = (apm_module_param_data_t*)( (uint8_t*)cmd_header + sizeof(apm_cmd_header_t) );
  param_data->module_instance_id = session_obj->mod_iid_tag_info[VS_MOD_DECODER].mod_iid;
  param_data->param_id = PARAM_ID_EVS_DEC_OUT_SAMPLING_RATE;
  param_data->param_size = sizeof(param_id_evs_dec_out_sampling_rate_t);

  sampling_rate = (param_id_evs_dec_out_sampling_rate_t*)( (uint8_t*)param_data + sizeof(apm_module_param_data_t) );
  sampling_rate->sampling_rate = session_obj->vocoder.rx_sample_rate;
  
  rc = __gpr_cmd_async_send(packet);

  if (rc)
  {
    VS_REPORT_FATAL_ON_ERROR(rc);
    session_obj->session_ctrl.status = rc;
    ( void ) vs_mem_free_object( ( vs_object_t* ) job_obj );
    ( void )__gpr_cmd_free(packet);
    rc = VS_EIMMEDIATE;
  }

  session_obj->cmd_ctrl.cmd_cnt ++;
  return rc;
}

uint32_t vs_voc_action_register_oobtty ( 
   vs_session_object_t* session_obj
)
{
  uint32_t rc = VS_EOK;
  vs_simple_job_object_t* job_obj;
  apm_cmd_header_t* cmd_header;
  apm_module_register_events_t *register_payload;
  gpr_cmd_alloc_ext_t tty_params;
  gpr_packet_t* packet = NULL;

  if ( (FALSE == session_obj->vocoder.tty_enable) || 
          (VS_VOICE_MOD_DEFAULT_IID ==  session_obj->mod_iid_tag_info[VS_MOD_TX_LTETTY].mod_iid) )
  {
    MSG_2( MSG_SSID_DFLT, MSG_LEGACY_HIGH,
           "VS:vs_voc_action_register_oobtty(): Returning...TTY enable %d , TTY module instance id 0x%08x ", session_obj->vocoder.tty_enable, session_obj->mod_iid_tag_info[VS_MOD_TX_LTETTY].mod_iid );
    session_obj->session_ctrl.status = VS_EOK;
    return VS_EIMMEDIATE;
  }

  rc = vs_create_simple_job_object( session_obj->header.handle, &job_obj );
  job_obj->fn_table[ VS_RESPONSE_FN_ENUM_RESULT ] = vs_voc_transition_result_rsp_fn;
  session_obj->session_ctrl.transition_job_handle = job_obj->header.handle;

  tty_params.src_domain_id = GPR_IDS_DOMAIN_ID_MODEM_V ;
  tty_params.src_port = VS_MODULE_INSTANCE_ID;
  tty_params.dst_domain_id = GPR_IDS_DOMAIN_ID_ADSP_V ; 
  tty_params.dst_port = session_obj->mod_iid_tag_info[VS_MOD_TX_LTETTY].mod_iid ;
  tty_params.token = job_obj->header.handle;
  tty_params.opcode = APM_CMD_REGISTER_MODULE_EVENTS;
  tty_params.payload_size = sizeof(apm_cmd_header_t) + sizeof(apm_module_register_events_t) ;
  tty_params.client_data = 0;
  tty_params.ret_packet =&packet ;
  
  rc = __gpr_cmd_alloc_ext(&tty_params);
  
  if( packet == NULL )
  {
    VS_REPORT_FATAL_ON_ERROR( rc );
    return VS_EFAILED;
  }

  cmd_header = GPR_PKT_GET_PAYLOAD( apm_cmd_header_t, packet );
  cmd_header->mem_map_handle = 0;
  cmd_header->payload_address_lsw = 0;
  cmd_header->payload_address_msw = 0;
  cmd_header->payload_size = sizeof(apm_module_register_events_t);

  register_payload = (apm_module_register_events_t*)( (uint8_t*)cmd_header + sizeof(apm_cmd_header_t) );
  register_payload->module_instance_id = session_obj->mod_iid_tag_info[VS_MOD_TX_LTETTY].mod_iid ;
  register_payload->event_id = EVENT_ID_LTETTY_CHAR;
  register_payload->is_register = TRUE;
  register_payload->event_config_payload_size = 0;
  register_payload->reserved = 0;

  rc = __gpr_cmd_async_send(packet);

  if ( rc )
  {
    VS_REPORT_FATAL_ON_ERROR(rc);
    MSG_1( MSG_SSID_DFLT, MSG_LEGACY_ERROR,
           "VS:vs_voc_action_register_oobtty(): Failed to communicate with TTY module while registering for event: rc=0x%08x", rc );
    session_obj->session_ctrl.status |= rc;
    ( void ) vs_mem_free_object( ( vs_object_t* ) job_obj );
    ( void )__gpr_cmd_free( packet);

  }

  MSG_1( MSG_SSID_DFLT, MSG_LEGACY_HIGH,
          "VS:vs_voc_action_register_oobtty(): Sent registration command for LTE TTY events: rc=0x%08x", rc );

  session_obj->cmd_ctrl.cmd_cnt ++;
  return rc;
}


uint32_t vs_voc_action_deregister_oobtty ( 
   vs_session_object_t* session_obj
)
{
  uint32_t rc = VS_EOK;
  vs_simple_job_object_t* job_obj;
  apm_cmd_header_t* cmd_header;
  apm_module_register_events_t *deregister_payload;
  gpr_cmd_alloc_ext_t tty_params;
  gpr_packet_t* packet = NULL;

  if ( FALSE == session_obj->vocoder.tty_enable || 
          (VS_VOICE_MOD_DEFAULT_IID ==  session_obj->mod_iid_tag_info[VS_MOD_TX_LTETTY].mod_iid) )
  {
    MSG_2( MSG_SSID_DFLT, MSG_LEGACY_HIGH,
           "VS:vs_voc_action_deregister_oobtty(): Returning...TTY enable %d , TTY module instance id 0x%08x ", session_obj->vocoder.tty_enable, session_obj->mod_iid_tag_info[VS_MOD_TX_LTETTY].mod_iid );
    session_obj->session_ctrl.status = VS_EOK;
    return VS_EIMMEDIATE;
  }

  rc = vs_create_simple_job_object( session_obj->header.handle, &job_obj );
  job_obj->fn_table[ VS_RESPONSE_FN_ENUM_RESULT ] = vs_voc_transition_result_rsp_fn;
  session_obj->session_ctrl.transition_job_handle = job_obj->header.handle;

  tty_params.src_domain_id = GPR_IDS_DOMAIN_ID_MODEM_V ;
  tty_params.src_port = VS_MODULE_INSTANCE_ID;
  tty_params.dst_domain_id = GPR_IDS_DOMAIN_ID_ADSP_V ; 
  tty_params.dst_port = session_obj->mod_iid_tag_info[VS_MOD_TX_LTETTY].mod_iid ; ;
  tty_params.token = job_obj->header.handle;
  tty_params.opcode = APM_CMD_REGISTER_MODULE_EVENTS;
  tty_params.payload_size = sizeof(apm_cmd_header_t) + sizeof(apm_module_register_events_t) ;
  tty_params.client_data = 0;
  tty_params.ret_packet =&packet ;
  
  rc = __gpr_cmd_alloc_ext(&tty_params);
  
  if( packet == NULL )
  {
    VS_REPORT_FATAL_ON_ERROR( rc );
    return VS_EFAILED;
  }

  cmd_header = GPR_PKT_GET_PAYLOAD( apm_cmd_header_t, packet );
  cmd_header->mem_map_handle = 0;
  cmd_header->payload_address_lsw = 0;
  cmd_header->payload_address_msw = 0;
  cmd_header->payload_size = sizeof(apm_module_register_events_t);

  deregister_payload = (apm_module_register_events_t*)( (uint8_t*)cmd_header + sizeof(apm_cmd_header_t) );
  deregister_payload->module_instance_id = session_obj->mod_iid_tag_info[VS_MOD_TX_LTETTY].mod_iid ;
  deregister_payload->event_id = EVENT_ID_LTETTY_CHAR;
  deregister_payload->is_register = FALSE;
  deregister_payload->event_config_payload_size = 0;
  deregister_payload->reserved = 0;

  rc = __gpr_cmd_async_send(packet);

  if ( rc )
  {
    VS_REPORT_FATAL_ON_ERROR(rc);
    MSG_1( MSG_SSID_DFLT, MSG_LEGACY_ERROR,
           "VS:vs_voc_action_register_oobtty(): Failed to communicate with TTY module while deregistering for event: rc=0x%08x", rc );
    session_obj->session_ctrl.status |= rc;
    ( void ) vs_mem_free_object( ( vs_object_t* ) job_obj );
    ( void )__gpr_cmd_free( packet);

  }

  MSG_1( MSG_SSID_DFLT, MSG_LEGACY_HIGH,
          "VS:vs_voc_action_deregister_oobtty(): Sent de-registration command for LTE TTY events: rc=0x%08x", rc );

  session_obj->cmd_ctrl.cmd_cnt ++;
  return rc;
}

uint32_t vs_voc_action_map_memory (
  vs_session_object_t* session_obj
)
{
  uint32_t rc = VS_EOK;
  vs_simple_job_object_t* job_obj = NULL;
  gpr_cmd_alloc_ext_t mem_params;
  gpr_packet_t* packet = NULL;
  apm_cmd_shared_mem_map_regions_t* map_cmd = NULL;
  apm_shared_map_region_payload_t* map_region = NULL;
  voicemem_cmd_get_iova_t params;
  uint64_t host_iova;
  ipa_dma_req_t req;

  rc = vs_create_simple_job_object( session_obj->header.handle, &job_obj );
  session_obj->session_ctrl.transition_job_handle = job_obj->header.handle;
  job_obj->fn_table[ VS_RESPONSE_FN_ENUM_RESULT] = vs_voc_transition_result_rsp_fn;
  job_obj->fn_table[ VS_RESPONSE_FN_ENUM_MEMORY_MAP ] = vs_voc_memory_map_result_rsp_fn;

  mem_params.src_domain_id = GPR_IDS_DOMAIN_ID_MODEM_V ;
  mem_params.src_port = VS_MODULE_INSTANCE_ID;
  mem_params.dst_domain_id = GPR_IDS_DOMAIN_ID_ADSP_V ; 
  mem_params.dst_port = VCPM_MODULE_INSTANCE_ID ;
  mem_params.token = job_obj->header.handle;
  mem_params.opcode = APM_CMD_SHARED_MEM_MAP_REGIONS;
  mem_params.payload_size = sizeof(apm_cmd_shared_mem_map_regions_t) + sizeof(apm_shared_map_region_payload_t) ;
  mem_params.client_data = 0;
  mem_params.ret_packet =&packet ;
  
  rc = __gpr_cmd_alloc_ext(&mem_params);

  if( packet == NULL )
  {
    VS_REPORT_FATAL_ON_ERROR( rc );
    return VS_EFAILED;
  }

  map_cmd = GPR_PKT_GET_PAYLOAD( apm_cmd_shared_mem_map_regions_t, packet );
  map_cmd->mem_pool_id = APM_MEMORY_MAP_SHMEM8_4K_POOL; //same value for test framework
  map_cmd->num_regions = 1;

  /* Bits in property flag represent following :
   * 1) Bit 0 : 1- virtual address. 0 - physical address
   * 2) Bit 1 : 1 - uncached. 0 - cached
   * 3) Bit 2 : 1 - offset mode enabled. 0 - offset mode disabled
   * 4) Bit 3 : 1 - client loaned memory. 0 - client owned memory
   */
  map_cmd->property_flag = 0;

  map_region = (apm_shared_map_region_payload_t*)( (uint8_t*)map_cmd + sizeof(apm_cmd_shared_mem_map_regions_t) );
  map_region->mem_size_bytes = session_obj->mem_size ;
  
#if(defined AVS_MPSS_TEST)
 {
   //Setting bit 0 to virtual
   map_cmd->property_flag |= (APM_MEMORY_MAP_VIRTUAL_MEMORY << APM_MEMORY_MAP_SHIFT_IS_VIRTUAL) ;

   if ( TRUE == vs_platform_is_fusion )
   {
     params.mem_handle = session_obj->voicemem_handle;
     params.smem_addr = session_obj->mem_virt_addr_start;
     params.adsp_iova =  &host_iova;
     params.pcie_iova = NULL;
     ( void )voicemem_call ( VOICEMEM_CMD_GET_IOVA, &params, sizeof(params) );
     map_region->shm_addr_lsw = host_iova;
     map_region->shm_addr_msw = ( host_iova >> 32);
    }
    else
    {
      map_region->shm_addr_lsw = session_obj->mem_virt_addr_start;
      map_region->shm_addr_msw = (session_obj->mem_virt_addr_start >> 32);
    }

    MSG_4( MSG_SSID_DFLT, MSG_LEGACY_MED, 
           "VS: vs_voc_action_map_memory(): Mapped region property flag[0x%08X], MSW[0x%08X], LSW[0x%08X], size[%d]",
           map_cmd->property_flag,map_region->shm_addr_msw, map_region->shm_addr_lsw, map_region->mem_size_bytes);
 }
#else
 {
   if ( TRUE == vs_platform_is_fusion)
   {
     params.mem_handle = session_obj->voicemem_handle;
     params.smem_addr = session_obj->mem_phy_addr_start;
     params.adsp_iova = &host_iova;
     params.pcie_iova = NULL;
     ( void )voicemem_call ( VOICEMEM_CMD_GET_IOVA, &params, sizeof(params) );
     map_region->shm_addr_lsw = host_iova;
     map_region->shm_addr_msw = ( host_iova >> 32);
   }
   else
   {
      map_region->shm_addr_lsw = session_obj->mem_phy_addr_start;
      map_region->shm_addr_msw = (session_obj->mem_phy_addr_start >> 32);
   }

   MSG_4( MSG_SSID_DFLT, MSG_LEGACY_MED, 
         "VS: vs_voc_action_map_memory(): Mapped region property flag[0x%08X], MSW[0x%08X], LSW[0x%08X], size[%d]",
         map_cmd->property_flag,map_region->shm_addr_msw, map_region->shm_addr_lsw, map_region->mem_size_bytes);
 }
#endif
#if 0
  if ( TRUE == vs_platform_is_fusion )
  {
    params.mem_handle = vs_shared_mem_table_handle;
    params.smem_addr = vs_shared_mem_table_phy_addr_start;
    params.adsp_iova = NULL;
    params.pcie_iova = &host_iova;
    ( void )voicemem_call ( VOICEMEM_CMD_GET_IOVA, &params, sizeof(params) );

#if (!defined AVS_MPSS_TEST)
      req.host_addr = host_iova;
      req.device_addr = (uint64_t) vs_shared_mem_table_phy_addr_start;
      req.size = vs_shared_mem_table_size; 
      req.options = 0;
      req.user_data = NULL;
      ipa_dma_device_to_host( session_obj->ipa_dl_handle, &req);
#else
      mmstd_memcpy( (void*)host_iova, vs_shared_mem_table_size,
                  (void*)vs_shared_mem_table_phy_addr_start , vs_shared_mem_table_size );
#endif
  }
#endif
  rc = __gpr_cmd_async_send( packet );
  if (rc)
  {
    VS_REPORT_FATAL_ON_ERROR(rc);
    MSG_1( MSG_SSID_DFLT, MSG_LEGACY_ERROR,
          "VS:vs_voc_action_map_memory: Failed to communicate with APM while mapping memory: rc=0x%08x", rc );
    session_obj->session_ctrl.status = rc;
    ( void ) vs_mem_free_object( ( vs_object_t* ) job_obj );
    ( void ) __gpr_cmd_free( packet );
    return VS_EIMMEDIATE;
  }

  session_obj->cmd_ctrl.cmd_cnt ++;
  return VS_EOK;
}

uint32_t vs_voc_action_unmap_memory (
  vs_session_object_t* session_obj
)
{
  uint32_t rc = VS_EOK;
  vs_simple_job_object_t* job_obj;
  apm_cmd_shared_mem_unmap_regions_t unmap_cmd;
  gpr_cmd_alloc_send_t map_args;

  if(  session_obj->memmap_handle == NULL) return VS_EIMMEDIATE;
  
  rc = vs_create_simple_job_object( session_obj->header.handle, &job_obj );
  session_obj->session_ctrl.transition_job_handle = job_obj->header.handle;
  job_obj->fn_table[ VS_RESPONSE_FN_ENUM_RESULT] = vs_voc_transition_result_rsp_fn;

  unmap_cmd.mem_map_handle = session_obj->memmap_handle;
  session_obj->memmap_handle = NULL;

#if defined (VS_PROFILING)
  vs_cmd_ticks_in_us = (uint64_t)((qurt_sysclock_get_hw_ticks()) *10ull/192ull);
  MSG_1( MSG_SSID_DFLT, MSG_LEGACY_HIGH,
         "VS_PROFILING:VSS_IMEMORY_CMD_UNMAP(): vs_cmd_ticks_in_us=(%lu)",
         vs_cmd_ticks_in_us );
#endif /* VS_PROFILING */

  map_args.src_domain_id = GPR_IDS_DOMAIN_ID_MODEM_V ;
  map_args.src_port = VS_MODULE_INSTANCE_ID;
  map_args.dst_domain_id = GPR_IDS_DOMAIN_ID_ADSP_V ; 
  map_args.dst_port = VCPM_MODULE_INSTANCE_ID ;
  map_args.token = job_obj->header.handle;
  map_args.opcode = APM_CMD_SHARED_MEM_UNMAP_REGIONS;
  map_args.payload_size = sizeof(apm_cmd_shared_mem_unmap_regions_t);
  map_args.client_data = 0;
  map_args.payload = &unmap_cmd;

  rc = __gpr_cmd_alloc_send(&map_args);

  if (rc)
  {
    VS_REPORT_FATAL_ON_ERROR(rc);
    session_obj->session_ctrl.status = rc;
    ( void ) vs_mem_free_object( ( vs_object_t* ) job_obj );
    return VS_EIMMEDIATE;
  }

  session_obj->cmd_ctrl.cmd_cnt ++;
  return VS_EOK;
}

uint32_t vs_voc_action_hpcm_start (
  vs_session_object_t* session_obj
)
{
  //will be implemented later
#if 0
  vs_simple_job_object_t* job_obj = NULL;
  aprv2_packet_t* packet = NULL;
  vss_ivpcm_cmd_start_v2_t* payload = NULL;
  uint8_t payloadSize = 0;
  vss_ivpcm_tap_point_t* tapPointInfo = NULL;

  /* Diable DTX for HPCM client if ENABLED by VOC client. */
  ( void ) vs_voc_action_set_dtx_v2( session_obj );

  rc = vs_create_simple_job_object( session_obj->header.handle, &job_obj );
  session_obj->session_ctrl.transition_job_handle = job_obj->header.handle;
  job_obj->fn_table[ VS_RESPONSE_FN_ENUM_RESULT] = vs_voc_transition_result_rsp_fn;

  /* Update the paylaod size. */
  payloadSize = sizeof(vss_ivpcm_cmd_start_v2_t);
  if( VS_PARAM_UINT8_NOT_SET != session_obj->hpcm.txMode )
  {
    payloadSize += sizeof(vss_ivpcm_tap_point_t);
  }
  if( VS_PARAM_UINT8_NOT_SET != session_obj->hpcm.rxMode )
  {
    payloadSize += sizeof(vss_ivpcm_tap_point_t);
  }

#if defined (VS_PROFILING)
  vs_cmd_ticks_in_us = (uint64_t)((qurt_sysclock_get_hw_ticks()) *10ull/192ull);
  MSG_1( MSG_SSID_DFLT, MSG_LEGACY_HIGH,
         "VS_PROFILING:VSS_IVPCM_CMD_START_V2(): vs_cmd_ticks_in_us=(%lu)",
         vs_cmd_ticks_in_us );
#endif /* VS_PROFILING */

  rc = __aprv2_cmd_alloc_ext(
         vs_apr_handle, APRV2_PKT_MSGTYPE_SEQCMD_V,
         GPR_IDS_DOMAIN_ID_MODEM_V, ( (uint16_t) session_obj->header.handle ),
         vs_cvs_addr, session_obj->cvs_port,
         job_obj->header.handle, VSS_IVPCM_CMD_START_V2,
         payloadSize, &packet );
  VS_REPORT_FATAL_ON_ERROR( rc );
  if( packet == NULL )
  {
    /* Lock not needed as cmd failed & no response expected */
    session_obj->session_ctrl.status = rc;
    ( void ) vs_mem_free_object( ( vs_object_t* ) job_obj );
    return APR_EIMMEDIATE;
  }

  payload = APRV2_PKT_GET_PAYLOAD( vss_ivpcm_cmd_start_v2_t, packet );
  payload->mem_handle = session_obj->memmap_handle;
  tapPointInfo = (vss_ivpcm_tap_point_t*)( (uint8_t*)payload + sizeof(vss_ivpcm_cmd_start_v2_t) );
  payload->num_tap_points = 0;

  if( VS_PARAM_UINT8_NOT_SET != session_obj->hpcm.txMode )
  {
    payload->num_tap_points++;
    tapPointInfo->tap_point = VSS_IVPCM_TAP_POINT_TX_DEFAULT;
    tapPointInfo->direction = session_obj->hpcm.txMode;
    tapPointInfo->sampling_rate = session_obj->hpcm.txSampleRate;
    tapPointInfo->duration = 0;
    tapPointInfo++;
  }

  if( VS_PARAM_UINT8_NOT_SET != session_obj->hpcm.rxMode )
  {
    payload->num_tap_points++;
    tapPointInfo->tap_point = VSS_IVPCM_TAP_POINT_RX_DEFAULT;
    tapPointInfo->direction = session_obj->hpcm.rxMode;
    tapPointInfo->sampling_rate = session_obj->hpcm.rxSampleRate;
    tapPointInfo->duration = 0;
    tapPointInfo++;
  }

  rc = __aprv2_cmd_forward( vs_apr_handle, packet );
  if (rc)
  {
    VS_REPORT_FATAL_ON_ERROR(rc);
    session_obj->session_ctrl.status = rc;
    ( void ) __aprv2_cmd_free( vs_apr_handle, packet );
    ( void ) vs_mem_free_object( ( vs_object_t* ) job_obj );
    return APR_EIMMEDIATE;
  }
#endif
  return VS_EIMMEDIATE;
}


uint32_t vs_voc_action_hpcm_stop (
  vs_session_object_t* session_obj
)
{
//Will be implemented later
#if 0
  uint32_t rc = APR_EOK;
  vs_simple_job_object_t* job_obj = NULL;

  //Reset the DTX setting to default.
  ( void ) vs_voc_action_set_dtx_v2( session_obj );

  rc = vs_create_simple_job_object( session_obj->header.handle, &job_obj );
  session_obj->session_ctrl.transition_job_handle = job_obj->header.handle;
  job_obj->fn_table[ VS_RESPONSE_FN_ENUM_RESULT] = vs_voc_transition_result_rsp_fn;

#if defined (VS_PROFILING)
  vs_cmd_ticks_in_us = (uint64_t)((qurt_sysclock_get_hw_ticks()) *10ull/192ull);
  MSG_1( MSG_SSID_DFLT, MSG_LEGACY_HIGH,
         "VS_PROFILING:VSS_IVPCM_CMD_STOP(): vs_cmd_ticks_in_us=(%lu)",
         vs_cmd_ticks_in_us );
#endif /* VS_PROFILING */

  rc = __aprv2_cmd_alloc_send(
         vs_apr_handle, APRV2_PKT_MSGTYPE_SEQCMD_V,
         GPR_IDS_DOMAIN_ID_MODEM_V, ( (uint16_t) session_obj->header.handle ),
         vs_cvs_addr, session_obj->cvs_port,
         job_obj->header.handle, VSS_IVPCM_CMD_STOP,
         NULL, 0 );
  if (rc)
  {
    VS_REPORT_FATAL_ON_ERROR(rc);
    ( void ) vs_mem_free_object( ( vs_object_t* ) job_obj );
    return APR_EIMMEDIATE;
  }
#endif
  return VS_EIMMEDIATE;
}

uint32_t vs_voc_action_set_tx_mbox_config (
  vs_session_object_t* session_obj
)
{
  int32_t rc = VS_EOK;
  vs_simple_job_object_t* job_obj;
  gpr_cmd_alloc_ext_t mbox_params;
  gpr_packet_t* packet = NULL;
  apm_cmd_header_t *cmd_header;
  apm_module_param_data_t *param_data;
  voicemem_cmd_get_iova_t params;
  uint64_t host_iova;
  mailbox_config_t *mbox_config;

  rc = vs_create_simple_job_object( session_obj->header.handle, &job_obj );
  job_obj->fn_table[ VS_RESPONSE_FN_ENUM_RESULT ] = vs_voc_transition_result_rsp_fn;
  session_obj->session_ctrl.transition_job_handle = job_obj->header.handle;

  mbox_params.src_domain_id = GPR_IDS_DOMAIN_ID_MODEM_V ;
  mbox_params.src_port = VS_MODULE_INSTANCE_ID;
  mbox_params.dst_domain_id = GPR_IDS_DOMAIN_ID_ADSP_V ; 
  mbox_params.dst_port = session_obj->mod_iid_tag_info[VS_MOD_TX_EP].mod_iid ;
  mbox_params.token = job_obj->header.handle;
  mbox_params.opcode = APM_CMD_SET_CFG;
  mbox_params.payload_size = sizeof(apm_cmd_header_t) + sizeof(apm_module_param_data_t) + 
                             sizeof(mailbox_config_t) ;
  mbox_params.client_data = 0;
  mbox_params.ret_packet =&packet ;


  rc = __gpr_cmd_alloc_ext(&mbox_params);

  if( packet == NULL )
  {
    VS_REPORT_FATAL_ON_ERROR( rc );
    return VS_EFAILED;
  } 

  cmd_header = GPR_PKT_GET_PAYLOAD( apm_cmd_header_t, packet );
  cmd_header->mem_map_handle = 0;
  cmd_header->payload_address_lsw = 0;
  cmd_header->payload_address_msw = 0;
  cmd_header->payload_size = sizeof(apm_module_param_data_t) + sizeof(mailbox_config_t);
  
  param_data = (apm_module_param_data_t*)( (uint8_t*)(cmd_header) + sizeof(apm_cmd_header_t) );
  param_data->module_instance_id = session_obj->mod_iid_tag_info[VS_MOD_TX_EP].mod_iid;
  param_data->param_id = PARAM_ID_MAILBOX_CFG;
  param_data->param_size = sizeof(mailbox_config_t);

  mbox_config = (mailbox_config_t*)( (uint8_t*)param_data + sizeof(apm_module_param_data_t) );
  mbox_config->mem_handle = session_obj->memmap_handle;

  /* Set the configuration information for Mailbox packet exchange mode */
  if ( TRUE == vs_platform_is_fusion )
  {
    params.mem_handle = session_obj->voicemem_handle;
    params.smem_addr = session_obj->vocoder.mbox_config.encCircBuf.phyAddr;
    params.adsp_iova = &host_iova;
    params.pcie_iova = NULL;
    ( void )voicemem_call ( VOICEMEM_CMD_GET_IOVA, &params, sizeof(params) );
    mbox_config->circ_buf_mem_address_lsw = host_iova;
    mbox_config->circ_buf_mem_address_msw = (host_iova >> 32);
  }
  else
  {
    mbox_config->circ_buf_mem_address_lsw = session_obj->vocoder.mbox_config.encCircBuf.phyAddr;
    mbox_config->circ_buf_mem_address_msw = ((session_obj->vocoder.mbox_config.encCircBuf.phyAddr) >> 32);
  }
  mbox_config->circ_buf_mem_size = session_obj->vocoder.mbox_config.encCircBuf.size;

  rc = __gpr_cmd_async_send(packet);
  if ( rc )
  {
    VS_REPORT_FATAL_ON_ERROR(rc);
    session_obj->session_ctrl.status |= rc;
    ( void ) vs_mem_free_object( ( vs_object_t* ) job_obj );
    ( void )__gpr_cmd_free( packet);
    return VS_EIMMEDIATE;
  }

  session_obj->cmd_ctrl.cmd_cnt ++;
  return VS_EOK;
}

uint32_t vs_voc_action_set_rx_mbox_config (
  vs_session_object_t* session_obj
)
{
  int32_t rc = VS_EOK;
  vs_simple_job_object_t* job_obj;
  gpr_cmd_alloc_ext_t mbox_params;
  gpr_packet_t* packet = NULL;
  apm_cmd_header_t *cmd_header;
  apm_module_param_data_t *param_data;
  voicemem_cmd_get_iova_t params;
  uint64_t host_iova;
  mailbox_config_t *mbox_config;

  rc = vs_create_simple_job_object( session_obj->header.handle, &job_obj );
  job_obj->fn_table[ VS_RESPONSE_FN_ENUM_RESULT ] = vs_voc_transition_result_rsp_fn;
  session_obj->session_ctrl.transition_job_handle = job_obj->header.handle;

  mbox_params.src_domain_id = GPR_IDS_DOMAIN_ID_MODEM_V ;
  mbox_params.src_port = VS_MODULE_INSTANCE_ID;
  mbox_params.dst_domain_id = GPR_IDS_DOMAIN_ID_ADSP_V ; 
  mbox_params.dst_port = session_obj->mod_iid_tag_info[VS_MOD_RX_EP].mod_iid ;
  mbox_params.token = job_obj->header.handle;
  mbox_params.opcode = APM_CMD_SET_CFG;
  mbox_params.payload_size = sizeof(apm_cmd_header_t) + sizeof(apm_module_param_data_t) + 
                             sizeof(mailbox_config_t) ;
  mbox_params.client_data = 0;
  mbox_params.ret_packet =&packet ;


  rc = __gpr_cmd_alloc_ext(&mbox_params);

  if( packet == NULL )
  {
    VS_REPORT_FATAL_ON_ERROR( rc );
    return VS_EFAILED;
  } 

  cmd_header = GPR_PKT_GET_PAYLOAD( apm_cmd_header_t, packet );
  cmd_header->mem_map_handle = 0;
  cmd_header->payload_address_lsw = 0;
  cmd_header->payload_address_msw = 0;
  cmd_header->payload_size = sizeof(apm_module_param_data_t) + sizeof(mailbox_config_t);
  
  param_data = (apm_module_param_data_t*)( (uint8_t*)cmd_header + sizeof(apm_cmd_header_t) );
  param_data->module_instance_id = session_obj->mod_iid_tag_info[VS_MOD_RX_EP].mod_iid;
  param_data->param_id = PARAM_ID_MAILBOX_CFG ;
  param_data->param_size = sizeof(mailbox_config_t);

  /* Set the configuration information for Mailbox packet exchange mode */
  mbox_config = (mailbox_config_t*)( (uint8_t*)param_data + sizeof(apm_module_param_data_t) );
  mbox_config->mem_handle = session_obj->memmap_handle;
  if ( TRUE == vs_platform_is_fusion )
  {
    params.mem_handle = session_obj->voicemem_handle;
    params.smem_addr = session_obj->vocoder.mbox_config.decCircBuf.phyAddr;
    params.adsp_iova = &host_iova;
    params.pcie_iova = NULL;
    ( void )voicemem_call ( VOICEMEM_CMD_GET_IOVA, &params, sizeof(params) );
    mbox_config->circ_buf_mem_address_lsw = host_iova;
    mbox_config->circ_buf_mem_address_msw = (host_iova >> 32);
  }
  else
  {
    mbox_config->circ_buf_mem_address_lsw = session_obj->vocoder.mbox_config.decCircBuf.phyAddr;
    mbox_config->circ_buf_mem_address_msw = ((session_obj->vocoder.mbox_config.decCircBuf.phyAddr) >> 32);
  }
  mbox_config->circ_buf_mem_size = session_obj->vocoder.mbox_config.decCircBuf.size;

#if defined (VS_PROFILING)
  vs_cmd_ticks_in_us = (uint64_t)((qurt_sysclock_get_hw_ticks()) *10ull/192ull);
  MSG_1( MSG_SSID_DFLT, MSG_LEGACY_HIGH,
         "VS_PROFILING:vs_voc_action_set_rx_mbox_config(): vs_cmd_ticks_in_us=(%lu)",
         vs_cmd_ticks_in_us );
#endif /* VS_PROFILING */

  rc = __gpr_cmd_async_send(packet);
  if ( rc )
  {
    VS_REPORT_FATAL_ON_ERROR(rc);
    session_obj->session_ctrl.status |= rc;
    ( void ) vs_mem_free_object( ( vs_object_t* ) job_obj );
    ( void )__gpr_cmd_free(packet);
    return VS_EIMMEDIATE;
  }

  session_obj->cmd_ctrl.cmd_cnt ++;
  return VS_EOK;
}


uint32_t vs_voc_action_put_mailbox_enc_request (
  vs_session_object_t* session_obj
)
{
  uint32_t rc = VS_EOK;
  uint64_t virtAddr;
  /* Host represents the APQ and device represents the SDX. 
   * So host IOVA represents the APQ DDR address and 
   * device address represent the SMEM address.
   */
  uint64_t host_iova;
  uint64_t device_addr;
  mailbox_voc_req_circ_buffer_t* encCircBuf;
  mailbox_voc_request_t* encRequest;
  vs_shmem_block_list_item_t* encItem;
  voicemem_cmd_cache_flush_t cmdPayload;
#if (!defined AVS_MPSS_TEST)
  ipa_dma_req_t req;
#endif  
  voicemem_cmd_get_iova_t params;

  virtAddr = session_obj->vocoder.mbox_config.encCircBuf.virtAddr;
  encCircBuf = (mailbox_voc_req_circ_buffer_t*)virtAddr;

#if VS_PROFILING
  MSG_4( MSG_SSID_DFLT, MSG_LEGACY_LOW,
         "VS: PUT_ENC_REQUEST: encCircBuf:[0x%08x] readOffset[%d] writeOffset[%d] reclaimOffset[%d]",
         encCircBuf, (uint32_t)encCircBuf->read_offset, (uint32_t) encCircBuf->write_offset, 
         (uint32_t)session_obj->vocoder.mbox_config.encReclaimOffset );
#endif /* VS_PROFILING */

  if( VS_IS_CURCULAR_BUF_FULL( encCircBuf, sizeof(mailbox_voc_request_t),
                               session_obj->vocoder.mbox_config.encCircBufDataSize,
                               session_obj->vocoder.mbox_config.encReclaimOffset ) )
  {
    MSG( MSG_SSID_DFLT, MSG_LEGACY_LOW, "VS: encCircBuffer is FULL!!");
    return VS_EOK;
  }

  rc = gpr_list_remove_head( &session_obj->vocoder.mbox_config.freeEncBufferQ,
                             ( ( gpr_list_node_t** ) &encItem ) );
  if ( rc )
  {
    MSG( MSG_SSID_DFLT, MSG_LEGACY_LOW,"VS: freeEncBufferQ is empty" );
    return VS_EOK;
  }
  gpr_list_add_tail( &session_obj->vocoder.mbox_config.usedEncBufferQ, &encItem->link );

  /*  Populate encoder request & Queue the encoder request onto circular buffer. */
  encRequest = (mailbox_voc_request_t*) ( (uint8_t*)encCircBuf +
                                                       sizeof(mailbox_voc_req_circ_buffer_t) +
                                                       encCircBuf->write_offset );
  if ( TRUE == vs_platform_is_fusion )
  {
    params.mem_handle = session_obj->voicemem_handle;
    params.smem_addr = encItem->buf.phyAddr;
    params.adsp_iova = &host_iova;
    params.pcie_iova = NULL;
    ( void )voicemem_call ( VOICEMEM_CMD_GET_IOVA, &params, sizeof(params) );
    //encRequest->mem_address = host_iova;
    encRequest->mem_address_lsw = (uint32_t)host_iova;
    encRequest->mem_address_msw = (uint32_t)(host_iova >>32);
  }
  else
  {
    encRequest->mem_address_lsw = (uint32_t)encItem->buf.phyAddr;
    encRequest->mem_address_msw = (uint32_t)(encItem->buf.phyAddr >>32);
  }
  encRequest->mem_size = VS_MBOX_BUF_SIZE;

  /* Increment circular buffer write offset. */
  VS_MODULO_INCREMENT( encCircBuf->write_offset, sizeof(mailbox_voc_request_t),
                       session_obj->vocoder.mbox_config.encCircBufDataSize );

  cmdPayload.voicemem_handle = session_obj->voicemem_handle;
  cmdPayload.virt_addr = (void*)encRequest;
  cmdPayload.size = sizeof(mailbox_voc_request_t);
  ( void ) voicemem_call( VOICEMEM_CMD_CACHE_FLUSH, &cmdPayload, sizeof( cmdPayload ) );

  cmdPayload.voicemem_handle = session_obj->voicemem_handle;
  cmdPayload.virt_addr = (void*)&encCircBuf->write_offset;
  cmdPayload.size = sizeof(uint64_t);
  ( void ) voicemem_call( VOICEMEM_CMD_CACHE_FLUSH, &cmdPayload, sizeof( cmdPayload ) );
  
  if( vs_platform_is_fusion )
  {
    params.mem_handle = session_obj->voicemem_handle;
    
#if (!defined AVS_MPSS_TEST)
    device_addr = (uint64_t) qurt_lookup_physaddr( (qurt_addr_t) encRequest );
    params.smem_addr = device_addr;
    params.adsp_iova = NULL;
    params.pcie_iova = &host_iova;
    ( void )voicemem_call ( VOICEMEM_CMD_GET_IOVA, &params, sizeof(params) );

    req.device_addr = device_addr;
    req.host_addr  = host_iova;
    req.size = sizeof(mailbox_voc_request_t);
    req.options = 0;
    req.user_data = (void*)NULL;

#if defined (VS_PROFILING_DATA_PCIE)
  vs_data_begin_ticks_in_us = (uint64_t)((qurt_sysclock_get_hw_ticks()) *10ull/192ull);
#endif /* VS_PROFILING_DATA_PCIE */

    ipa_dma_device_to_host( session_obj->ipa_ul_handle, &req);

#if defined (VS_PROFILING_DATA_PCIE)
  vs_data_end_ticks_in_us = (uint64_t)((qurt_sysclock_get_hw_ticks()) *10ull/192ull);
  MSG_4( MSG_SSID_DFLT, MSG_LEGACY_HIGH,
         "VS_PROFILING-PCIe: PUT_ENC_REQUEST(): write encoding_request: MODEM_to_ADSP: tick1_us=(%lu), tick2_us =(%lu), diff_us=(%lu), size=(%d)",
         vs_data_begin_ticks_in_us, vs_data_end_ticks_in_us, 
         (vs_data_end_ticks_in_us - vs_data_begin_ticks_in_us), req.size );
#endif /* VS_PROFILING_DATA_PCIE */

    
    device_addr = (uint64_t) qurt_lookup_physaddr( (qurt_addr_t)(&encCircBuf->write_offset) );
    params.smem_addr = device_addr;
    params.adsp_iova = NULL;
    params.pcie_iova = &host_iova;
    ( void )voicemem_call ( VOICEMEM_CMD_GET_IOVA, &params, sizeof(params) );
    req.device_addr = device_addr;
    req.host_addr  = host_iova;
    req.size = sizeof(uint64_t); 
    req.options = 0;
    req.user_data = (void*)NULL;

#if defined (VS_PROFILING_DATA_PCIE)
  vs_data_begin_ticks_in_us = (uint64_t)((qurt_sysclock_get_hw_ticks()) *10ull/192ull);
#endif /* VS_PROFILING_DATA_PCIE */

    ipa_dma_device_to_host( session_obj->ipa_ul_handle, &req);  

#if defined (VS_PROFILING_DATA_PCIE)
  vs_data_end_ticks_in_us = (uint64_t)((qurt_sysclock_get_hw_ticks()) *10ull/192ull);
  MSG_4( MSG_SSID_DFLT, MSG_LEGACY_HIGH,
         "VS_PROFILING-PCIe: PUT_ENC_REQUEST(): write encCircBuf->write_offset: MAP_MEMORY: MODEM_to_ADSP: tick1_us=(%lu), tick2_us =(%lu), diff_us=(%lu), size=(%d)",
         vs_data_begin_ticks_in_us, vs_data_end_ticks_in_us, 
         (vs_data_end_ticks_in_us - vs_data_begin_ticks_in_us), req.size );
#endif /* VS_PROFILING_DATA_PCIE */


 #else
    device_addr = (uint64_t)encRequest;
    params.smem_addr = device_addr;
    params.adsp_iova = NULL;
    params.pcie_iova = &host_iova;
    ( void )voicemem_call ( VOICEMEM_CMD_GET_IOVA, &params, sizeof(params) );
    mmstd_memcpy( (void*)host_iova, sizeof(mailbox_voc_request_t),
                  ((void*)encRequest ), sizeof(mailbox_voc_request_t) );

    device_addr = (uint64_t)&encCircBuf->write_offset;
    params.smem_addr = device_addr;
    params.adsp_iova = NULL;
    params.pcie_iova = &host_iova;
    ( void )voicemem_call ( VOICEMEM_CMD_GET_IOVA, &params, sizeof(params) );
    mmstd_memcpy( (void*)host_iova, sizeof(uint64_t),
                  (void*)device_addr, sizeof(uint64_t) );
#endif
  }
  return rc;
}


uint32_t vs_voc_action_read_enc_mailbox (
  vs_session_object_t* session_obj,
  vs_pktexg_buffer_t** ret_buffer
)
{
  uint32_t rc = VS_EOK;
  uint64_t virtAddr;
  /* Host represents the APQ and device represents the SDX. 
   * So host IOVA represents the APQ DDR address and 
   * device address represent the SMEM address.
   */
  uint64_t host_iova;
  uint64_t device_addr;
  mailbox_voc_req_circ_buffer_t* encCircBuf;
  vs_shmem_block_list_item_t* encItem;
  voicemem_cmd_cache_invalidate_t cmdPayload;
  gpr_list_t* temp_list = NULL;
#if (!defined AVS_MPSS_TEST)
  ipa_dma_req_t req;
#endif  
  voicemem_cmd_get_iova_t params;

  virtAddr = session_obj->vocoder.mbox_config.encCircBuf.virtAddr;
  encCircBuf = (mailbox_voc_req_circ_buffer_t*)virtAddr;
  if( vs_platform_is_fusion )
  {
#if (!defined AVS_MPSS_TEST)
    device_addr = (uint64_t) qurt_lookup_physaddr( (qurt_addr_t)(&encCircBuf->read_offset) );
    params.smem_addr = device_addr;
    params.mem_handle = session_obj->voicemem_handle;
    params.adsp_iova = NULL;
    params.pcie_iova = &host_iova;
    ( void )voicemem_call ( VOICEMEM_CMD_GET_IOVA, &params, sizeof(params) );

    req.device_addr = device_addr;
    req.host_addr  = host_iova;
    req.size = sizeof(uint64_t);
    req.options = 0;
    req.user_data = (void*)NULL;

#if defined (VS_PROFILING_DATA_PCIE)
  vs_data_begin_ticks_in_us = (uint64_t)((qurt_sysclock_get_hw_ticks()) *10ull/192ull);
#endif /* VS_PROFILING_DATA_PCIE */

    ipa_dma_host_to_device( session_obj->ipa_ul_handle, &req);

#if defined (VS_PROFILING_DATA_PCIE)
  vs_data_end_ticks_in_us = (uint64_t)((qurt_sysclock_get_hw_ticks()) *10ull/192ull);
  MSG_4( MSG_SSID_DFLT, MSG_LEGACY_HIGH,
         "VS_PROFILING-PCIe: READ_ENC_MAILBOX() read encCircBuf->read_offset: MAP_MEMORY: ADSP_to_MODEM: tick1_us=(%lu), tick2_us =(%lu), diff_us=(%lu), size=(%d)",
         vs_data_begin_ticks_in_us, vs_data_end_ticks_in_us, 
         (vs_data_end_ticks_in_us - vs_data_begin_ticks_in_us), req.size );
#endif /* VS_PROFILING_DATA_PCIE */


#else
    device_addr = (uint64_t)&encCircBuf->read_offset;
    params.smem_addr = device_addr;
    params.mem_handle = session_obj->voicemem_handle;
    params.adsp_iova = NULL;
    params.pcie_iova = &host_iova;
    ( void )voicemem_call ( VOICEMEM_CMD_GET_IOVA, &params, sizeof(params) );

    mmstd_memcpy( (void*)&encCircBuf->read_offset, sizeof(uint64_t),
                       (void*)host_iova , sizeof(uint64_t));
#endif
  }
  cmdPayload.voicemem_handle = session_obj->voicemem_handle;
  cmdPayload.virt_addr = (void*)&encCircBuf->read_offset;
  cmdPayload.size = sizeof(uint64_t);
  ( void ) voicemem_call( VOICEMEM_CMD_CACHE_INVALIDATE, &cmdPayload, sizeof( cmdPayload ) );

#if VS_PROFILING
  MSG_4( MSG_SSID_DFLT, MSG_LEGACY_LOW,
         "VS: READ_ENC_MBOX: encCircBuf:[0x%08x] readOffset[%d] writeOffset[%d] reclaimOffset[%d]",
         encCircBuf, (uint32_t)encCircBuf->read_offset, (uint32_t) encCircBuf->write_offset, 
         (uint32_t)session_obj->vocoder.mbox_config.encReclaimOffset );
#endif /* VS_PROFILING */

  if ( session_obj->vocoder.mbox_config.encReclaimOffset != encCircBuf->read_offset )
  {
    /* Re-claim Tx packet. */
   /*
    if(NULL == session_obj->vocoder.mbox_config.usedEncBufferQ )
    {
       MSG( MSG_SSID_DFLT, MSG_LEGACY_LOW,"VS: usedEncBufferQ is empty" );
       return VS_EUNEXPECTED;
    }
    else
    {
      temp_list = &session_obj->vocoder.mbox_config.usedEncBufferQ;
      encItem = (vs_shmem_block_list_item_t*)(temp_list->dummy.next);
    }*/
    rc = vs_get_list_peak_head( &session_obj->vocoder.mbox_config.usedEncBufferQ,
                             ( ( gpr_list_node_t** ) &encItem ) );
    if ( rc )
    {
      MSG( MSG_SSID_DFLT, MSG_LEGACY_LOW,"VS: usedEncBufferQ is empty" );
      return VS_EUNEXPECTED;
    } 
    if( vs_platform_is_fusion )
    {
      device_addr = (uint64_t)encItem->buf.phyAddr;
      params.smem_addr = encItem->buf.phyAddr;
      params.mem_handle = session_obj->voicemem_handle;
      params.adsp_iova = NULL;
      params.pcie_iova = &host_iova;
      ( void )voicemem_call ( VOICEMEM_CMD_GET_IOVA, &params, sizeof(params) );
#if (!defined AVS_MPSS_TEST)
      req.device_addr = device_addr;
      req.host_addr  = host_iova;
      req.size = encItem->buf.size;
      req.options = 0;
      req.user_data = (void*)NULL;

#if defined (VS_PROFILING_DATA_PCIE)
  vs_data_begin_ticks_in_us = (uint64_t)((qurt_sysclock_get_hw_ticks()) *10ull/192ull);
#endif /* VS_PROFILING_DATA_PCIE */

      ipa_dma_host_to_device( session_obj->ipa_ul_handle, &req);

#if defined (VS_PROFILING_DATA_PCIE)
  vs_data_end_ticks_in_us = (uint64_t)((qurt_sysclock_get_hw_ticks()) *10ull/192ull);
  MSG_4( MSG_SSID_DFLT, MSG_LEGACY_HIGH,
         "VS_PROFILING-PCIe: READ_ENC_MAILBOX(): read encoder_packet: MAP_MEMORY: ADSP_to_MODEM: tick1_us=(%lu), tick2_us =(%lu), diff_us=(%lu), size=(%d)",
         vs_data_begin_ticks_in_us, vs_data_end_ticks_in_us, 
         (vs_data_end_ticks_in_us - vs_data_begin_ticks_in_us), req.size );
#endif /* VS_PROFILING_DATA_PCIE */

#else
      mmstd_memcpy( (void*)encItem->buf.phyAddr , encItem->buf.size,
                       (void*)host_iova, encItem->buf.size);
#endif   
    }    
    cmdPayload.voicemem_handle = session_obj->voicemem_handle;
    cmdPayload.virt_addr = (void*)encItem->buf.virtAddr;
    cmdPayload.size = encItem->buf.size;
    ( void ) voicemem_call( VOICEMEM_CMD_CACHE_INVALIDATE, &cmdPayload, sizeof( cmdPayload ) );
    *ret_buffer = (vs_pktexg_buffer_t*)encItem->buf.virtAddr;

    VS_MODULO_INCREMENT( session_obj->vocoder.mbox_config.encReclaimOffset,
                         sizeof(mailbox_voc_request_t),
                         session_obj->vocoder.mbox_config.encCircBufDataSize );

    rc = VS_EOK;
  }
  else
  {
    *ret_buffer = NULL;
    rc = VS_ENOTEXIST;
  }

  return rc;
}

uint32_t vs_voc_action_put_mailbox_dec_request (
  vs_session_object_t* session_obj
)
{
  uint32_t rc = VS_EOK;
  uint64_t virtAddr;
  uint64_t host_iova;
  mailbox_voc_req_circ_buffer_t* decCircBuf;
  mailbox_voc_request_t* decRequest;
  vs_shmem_block_list_item_t* decItem;
  voicemem_cmd_cache_flush_t cmdPayload;
#if (!defined AVS_MPSS_TEST)
  uint64_t device_addr;
  ipa_dma_req_t req;
#endif  
  voicemem_cmd_get_iova_t params;

  virtAddr = session_obj->vocoder.mbox_config.decCircBuf.virtAddr;
  decCircBuf = (mailbox_voc_req_circ_buffer_t*)virtAddr;


  MSG_4( MSG_SSID_DFLT, MSG_LEGACY_LOW,
         "VS: PUT_DEC_REQUEST: decCircBuf:[0x%08x] readOffset[%d] writeOffset[%d] reclaimOffset[%d]",
         decCircBuf, (uint32_t)decCircBuf->read_offset, (uint32_t)decCircBuf->write_offset, 
         (uint32_t)session_obj->vocoder.mbox_config.decReclaimOffset );

  if( VS_IS_CURCULAR_BUF_FULL( decCircBuf, sizeof(mailbox_voc_request_t),
                               session_obj->vocoder.mbox_config.decCircBufDataSize,
                               session_obj->vocoder.mbox_config.decReclaimOffset ) )
  {
    MSG( MSG_SSID_DFLT, MSG_LEGACY_LOW, "VS: decCircBuffer is FULL!!");
    return VS_EOK;
  }

  rc = gpr_list_remove_head( &session_obj->vocoder.mbox_config.freeDecBufferQ,
                             ( ( gpr_list_node_t** ) &decItem ) );
  if ( rc )
  {
    MSG( MSG_SSID_DFLT, MSG_LEGACY_LOW,"VS: freeDecBufferQ is empty" );
    return VS_EOK;
  }
  gpr_list_add_tail( &session_obj->vocoder.mbox_config.usedDecBufferQ, &decItem->link );

  /*  Populate decoder request & Queue the decoder request onto circular buffer. */
  decRequest = (mailbox_voc_request_t*) ( (uint8_t*)decCircBuf +
                                                       sizeof(mailbox_voc_req_circ_buffer_t) +
                                                       decCircBuf->write_offset );

  if ( TRUE == vs_platform_is_fusion )
  {
    params.smem_addr = decItem->buf.phyAddr;
    params.mem_handle = session_obj->voicemem_handle;
    params.adsp_iova = &host_iova;
    params.pcie_iova = NULL;
    ( void )voicemem_call ( VOICEMEM_CMD_GET_IOVA, &params, sizeof(params) );
//    decRequest->mem_address = host_iova;
    decRequest->mem_address_lsw = (uint32_t)host_iova;
    decRequest->mem_address_msw = (uint32_t)(host_iova >>32);
  }
  else
  {
    decRequest->mem_address_lsw = (uint32_t)decItem->buf.phyAddr;
    decRequest->mem_address_msw = (uint32_t)(decItem->buf.phyAddr >>32);
  }
  decRequest->mem_size = VS_MBOX_BUF_SIZE;

  MSG_2( MSG_SSID_DFLT, MSG_LEGACY_LOW,
         "VS: vs_voc_action_put_mailbox_dec_request(): mem_address_lsw:[0x%08x] mem_address_msw[0x%08x] ",
  decRequest->mem_address_lsw, decRequest->mem_address_msw);

  /* Flush the decoder buffer. */
  cmdPayload.voicemem_handle = session_obj->voicemem_handle;
  cmdPayload.virt_addr = (void*)decItem->buf.virtAddr;
  cmdPayload.size = decItem->buf.size;
  ( void ) voicemem_call( VOICEMEM_CMD_CACHE_FLUSH, &cmdPayload, sizeof( cmdPayload ) );

  /* Flush the decoder circular buffer. */
  cmdPayload.voicemem_handle = session_obj->voicemem_handle;
  cmdPayload.virt_addr = (void*)decRequest;
  cmdPayload.size = sizeof(mailbox_voc_request_t);
  ( void ) voicemem_call( VOICEMEM_CMD_CACHE_FLUSH, &cmdPayload, sizeof( cmdPayload ) );

  /* Increment circular buffer write offset. */
  VS_MODULO_INCREMENT( decCircBuf->write_offset, sizeof(mailbox_voc_request_t),
                       session_obj->vocoder.mbox_config.decCircBufDataSize );
  /* Flush the decoder write offset. */
  cmdPayload.voicemem_handle = session_obj->voicemem_handle;
  cmdPayload.virt_addr = (void*)&decCircBuf->write_offset;
  cmdPayload.size = sizeof(uint64_t);
  ( void ) voicemem_call( VOICEMEM_CMD_CACHE_FLUSH, &cmdPayload, sizeof( cmdPayload ) );

  if( vs_platform_is_fusion )
  {
#if (!defined AVS_MPSS_TEST)
    /* Move the decoder buffer from device to host. */
    device_addr = (uint64_t) decItem->buf.phyAddr;
    params.smem_addr = device_addr;
    params.mem_handle = session_obj->voicemem_handle;
    params.adsp_iova = NULL;
    params.pcie_iova = &host_iova;
    ( void )voicemem_call ( VOICEMEM_CMD_GET_IOVA, &params, sizeof(params) );

    req.device_addr = device_addr;
    req.host_addr  = host_iova;
    req.size = decItem->buf.size;
    req.options = 0;
    req.user_data = (void*)NULL;

#if defined (VS_PROFILING_DATA_PCIE)
  vs_data_begin_ticks_in_us = (uint64_t)((qurt_sysclock_get_hw_ticks()) *10ull/192ull);
#endif /* VS_PROFILING_DATA_PCIE */

    ipa_dma_device_to_host( session_obj->ipa_dl_handle, &req);

#if defined (VS_PROFILING_DATA_PCIE)
  vs_data_end_ticks_in_us = (uint64_t)((qurt_sysclock_get_hw_ticks()) *10ull/192ull);
  MSG_4( MSG_SSID_DFLT, MSG_LEGACY_HIGH,
         "VS_PROFILING-PCIe: PUT_DEC_REQUEST(): write decoder_packet: MAP_MEMORY: MODEM_to_ADSP: tick1_us=(%lu), tick2_us =(%lu), diff_us=(%lu), size=(%d)",
         vs_data_begin_ticks_in_us, vs_data_end_ticks_in_us, 
         (vs_data_end_ticks_in_us - vs_data_begin_ticks_in_us), req.size );
#endif /* VS_PROFILING_DATA_PCIE */


    /* Move the decoding request from device to host. */
    device_addr = (uint64_t) qurt_lookup_physaddr( (qurt_addr_t)decRequest );
    params.smem_addr = device_addr;
    params.mem_handle = session_obj->voicemem_handle;
    params.adsp_iova = NULL;
    params.pcie_iova = &host_iova;
    ( void )voicemem_call ( VOICEMEM_CMD_GET_IOVA, &params, sizeof(params) );

    req.device_addr = device_addr;
    req.host_addr  = host_iova;
    req.size = sizeof(mailbox_voc_request_t);
    req.options = 0;
    req.user_data = (void*)NULL;

#if defined (VS_PROFILING_DATA_PCIE)
  vs_data_begin_ticks_in_us = (uint64_t)((qurt_sysclock_get_hw_ticks()) *10ull/192ull);
#endif /* VS_PROFILING_DATA_PCIE */

    ipa_dma_device_to_host( session_obj->ipa_dl_handle, &req );  

#if defined (VS_PROFILING_DATA_PCIE)
  vs_data_end_ticks_in_us = (uint64_t)((qurt_sysclock_get_hw_ticks()) *10ull/192ull);
  MSG_4( MSG_SSID_DFLT, MSG_LEGACY_HIGH,
         "VS_PROFILING-PCIe: PUT_DEC_REQUEST(): write decoding_request: MAP_MEMORY: MODEM_to_ADSP: tick1_us=(%lu), tick2_us =(%lu), diff_us=(%lu), size=(%d)",
         vs_data_begin_ticks_in_us, vs_data_end_ticks_in_us, 
         (vs_data_end_ticks_in_us - vs_data_begin_ticks_in_us), req.size );
#endif /* VS_PROFILING_DATA_PCIE */

    /* Move the write offset update from device to host. */
    device_addr = (uint64_t) qurt_lookup_physaddr( (qurt_addr_t)(&decCircBuf->write_offset) );
    params.smem_addr = device_addr;
    params.mem_handle = session_obj->voicemem_handle;
    params.adsp_iova = NULL;
    params.pcie_iova = &host_iova;
    ( void )voicemem_call ( VOICEMEM_CMD_GET_IOVA, &params, sizeof(params) );

    req.device_addr = device_addr;
    req.host_addr  = host_iova;
    req.size = sizeof(uint64_t);
    req.options = 0;
    req.user_data = (void*)NULL;

#if defined (VS_PROFILING_DATA_PCIE)
  vs_data_begin_ticks_in_us = (uint64_t)((qurt_sysclock_get_hw_ticks()) *10ull/192ull);
#endif /* VS_PROFILING_DATA_PCIE */

    ipa_dma_device_to_host( session_obj->ipa_dl_handle, &req );

#if defined (VS_PROFILING_DATA_PCIE)
  vs_data_end_ticks_in_us = (uint64_t)((qurt_sysclock_get_hw_ticks()) *10ull/192ull);
  MSG_4( MSG_SSID_DFLT, MSG_LEGACY_HIGH,
         "VS_PROFILING-PCIe: PUT_DEC_REQUEST(): write decCircBuf->write_offset: MAP_MEMORY: MODEM_to_ADSP: tick1_us=(%lu), tick2_us =(%lu), diff_us=(%lu), size=(%d)",
         vs_data_begin_ticks_in_us, vs_data_end_ticks_in_us, 
         (vs_data_end_ticks_in_us - vs_data_begin_ticks_in_us), req.size );
#endif /* VS_PROFILING_DATA_PCIE */

#else
    params.smem_addr = ((uint64_t)decItem->buf.virtAddr );
    params.mem_handle = session_obj->voicemem_handle;
    params.adsp_iova = NULL;
    params.pcie_iova = &host_iova;
    ( void )voicemem_call ( VOICEMEM_CMD_GET_IOVA, &params, sizeof(params) );
    /* Move the decoder buffer from device to host. */
    mmstd_memcpy( (void*)host_iova, decItem->buf.size,
                        ((void*)decItem->buf.phyAddr ), decItem->buf.size );
                        
    params.smem_addr = (uint64_t)decRequest;
    params.mem_handle = session_obj->voicemem_handle;
    params.adsp_iova = NULL;
    params.pcie_iova = &host_iova;
    ( void )voicemem_call ( VOICEMEM_CMD_GET_IOVA, &params, sizeof(params) );
    /* Move the decoding request from device to host. */
    mmstd_memcpy( (void*)host_iova, sizeof(mailbox_voc_request_t),
                    ((void*)decRequest ), sizeof(mailbox_voc_request_t) );

    params.smem_addr = (uint64_t)(&decCircBuf->write_offset);
    params.mem_handle = session_obj->voicemem_handle;
    params.adsp_iova = NULL;
    params.pcie_iova = &host_iova;
    ( void )voicemem_call ( VOICEMEM_CMD_GET_IOVA, &params, sizeof(params) );
   /* Move the write offset update from device to host. */
    mmstd_memcpy( (void*)host_iova, sizeof(uint64_t),
                  (((void*)&decCircBuf->write_offset) ), sizeof(uint64_t) );
#endif
  }

  return rc;
}

uint32_t vs_voc_action_flush_enc_mailbox (
  vs_session_object_t* session_obj
)
{
  uint32_t rc = VS_EOK;
  uint64_t virtAddr;
  /* Host represents the APQ and device represents the SDX. 
   * So host IOVA represents the APQ DDR address and 
   * device address represent the SMEM address.
   */
  uint64_t host_iova;
  uint64_t device_addr;
  mailbox_voc_req_circ_buffer_t* encCircBuf;
  vs_shmem_block_list_item_t* encItem;
  voicemem_cmd_cache_flush_t cmdPayload;
#if (!defined AVS_MPSS_TEST)
    ipa_dma_req_t req;
#endif  
  voicemem_cmd_get_iova_t params;

  virtAddr = session_obj->vocoder.mbox_config.encCircBuf.virtAddr;
  encCircBuf = (mailbox_voc_req_circ_buffer_t*)virtAddr;
  
  if( NULL == encCircBuf )
  {
     return rc;
  }
  if( vs_platform_is_fusion )
  {
#if (!defined AVS_MPSS_TEST)
    device_addr = (uint64_t) qurt_lookup_physaddr( (qurt_addr_t)(&encCircBuf->read_offset) );
    params.smem_addr = device_addr;
    params.mem_handle = session_obj->voicemem_handle;
    params.adsp_iova = NULL;
    params.pcie_iova = &host_iova;
    ( void )voicemem_call ( VOICEMEM_CMD_GET_IOVA, &params, sizeof(params) );

    /* Move the read offset update from device to host. */
    req.device_addr = device_addr;
    req.host_addr = host_iova;
    req.size = sizeof(uint64_t);
    req.options = 0;
    req.user_data = (void*)NULL;
#if defined (VS_PROFILING_DATA_PCIE)
  vs_data_begin_ticks_in_us = (uint64_t)((qurt_sysclock_get_hw_ticks()) *10ull/192ull);
#endif /* VS_PROFILING_DATA_PCIE */

    ipa_dma_host_to_device( session_obj->ipa_ul_handle, &req);

#if defined (VS_PROFILING_DATA_PCIE)
  vs_data_end_ticks_in_us = (uint64_t)((qurt_sysclock_get_hw_ticks()) *10ull/192ull);
  MSG_4( MSG_SSID_DFLT, MSG_LEGACY_HIGH,
         "VS_PROFILING-PCIe: FLUSH_ENC_MAILBOX(): read encCircBuf->read_offset: MAP_MEMORY: ADSP_to_Modem: tick1_us=(%lu), tick2_us =(%lu), diff_us=(%lu), size=(%d)",
         vs_data_begin_ticks_in_us, vs_data_end_ticks_in_us, 
         (vs_data_end_ticks_in_us - vs_data_begin_ticks_in_us), req.size );
#endif /* VS_PROFILING_DATA_PCIE */

#else
    device_addr = (uint64_t)&encCircBuf->read_offset;
    params.smem_addr = device_addr;
    params.mem_handle = session_obj->voicemem_handle;
    params.adsp_iova = NULL;
    params.pcie_iova = &host_iova;
    ( void )voicemem_call ( VOICEMEM_CMD_GET_IOVA, &params, sizeof(params) );

    mmstd_memcpy( (void*)&encCircBuf->read_offset , sizeof(uint64_t),
                  (void*)host_iova , sizeof(uint64_t) );
#endif
  }
  cmdPayload.voicemem_handle = session_obj->voicemem_handle;
  cmdPayload.virt_addr = (void*)&encCircBuf->read_offset;
  cmdPayload.size = sizeof(uint64_t);
  ( void ) voicemem_call( VOICEMEM_CMD_CACHE_INVALIDATE, &cmdPayload, sizeof( cmdPayload ) );

#if VS_PROFILING
  MSG_4( MSG_SSID_DFLT, MSG_LEGACY_LOW,
         "VS: FLUSH_ENC_MBOX: encCircBuf:[0x%08x] readOffset[%d] writeOffset[%d] reclaimOffset[%d]",
         encCircBuf, (uint32_t)encCircBuf->read_offset, (uint32_t) encCircBuf->write_offset, 
         (uint32_t)session_obj->vocoder.mbox_config.encReclaimOffset );
#endif /* VS_PROFILING */

  //Discard all the completed enc_request which are yet to be read.
  while ( session_obj->vocoder.mbox_config.encReclaimOffset != encCircBuf->read_offset )
  {
    /* discard Encoder packet. */
    rc = gpr_list_remove_head( &session_obj->vocoder.mbox_config.usedEncBufferQ,
                               ( ( gpr_list_node_t** ) &encItem ) );
    if ( rc )
    {
      MSG( MSG_SSID_DFLT, MSG_LEGACY_LOW,"VS: usedEncBufferQ is empty" );
      return VS_EUNEXPECTED;
    }
    gpr_list_add_tail( &session_obj->vocoder.mbox_config.freeEncBufferQ, &encItem->link );

    VS_MODULO_INCREMENT( session_obj->vocoder.mbox_config.encReclaimOffset,
                         sizeof(mailbox_voc_request_t),
                         session_obj->vocoder.mbox_config.encCircBufDataSize );

    // Requeue encoding request.
    ( void ) vs_voc_action_put_mailbox_enc_request ( session_obj );
  }

  return rc;
}

uint32_t vs_voc_action_flush_dec_mailbox (
  vs_session_object_t* session_obj
)
{
  uint32_t rc = VS_EOK;
  uint64_t virtAddr;
  /* Host represents the APQ and device represents the SDX. 
   * So host IOVA represents the APQ DDR address and 
   * device address represent the SMEM address.
   */
  uint64_t host_iova;
  uint64_t device_addr;
#if (!defined AVS_MPSS_TEST)
    ipa_dma_req_t req;
#endif  
  voicemem_cmd_get_iova_t params;
  mailbox_voc_req_circ_buffer_t* decCircBuf;
  vs_shmem_block_list_item_t*decItem;
  voicemem_cmd_cache_flush_t cmdPayload;
  
  virtAddr = session_obj->vocoder.mbox_config.decCircBuf.virtAddr;
  decCircBuf = (mailbox_voc_req_circ_buffer_t*)virtAddr;
  if( NULL == decCircBuf )
  {
     return rc;
  }

#if VS_PROFILING
  MSG_4( MSG_SSID_DFLT, MSG_LEGACY_LOW,
         "VS: FLUSH_DEC_MBOX: decCircBuf:[0x%08x] readOffset[%d] writeOffset[%d] reclaimOffset[%d]",
         decCircBuf, (uint32_t)decCircBuf->read_offset, (uint32_t) decCircBuf->write_offset, 
         (uint32_t)session_obj->vocoder.mbox_config.decReclaimOffset );
#endif /* VS_PROFILING */

  //Discard all the completed dec_request which are yet to be rendered.
  while ( session_obj->vocoder.mbox_config.decReclaimOffset != decCircBuf->write_offset )
  {
    /* discard Encoder packet. */
    rc = gpr_list_remove_head( &session_obj->vocoder.mbox_config.usedDecBufferQ,
                               ( ( gpr_list_node_t** ) &decItem ) );
    if ( rc )
    {
      MSG( MSG_SSID_DFLT, MSG_LEGACY_LOW,"VS: usedEncBufferQ is empty" );
      return VS_EUNEXPECTED;
    }

    gpr_list_add_tail( &session_obj->vocoder.mbox_config.freeDecBufferQ, &decItem->link );
    VS_MODULO_INCREMENT( session_obj->vocoder.mbox_config.decReclaimOffset,
                         sizeof(mailbox_voc_request_t),
                         session_obj->vocoder.mbox_config.decCircBufDataSize );
  }

  decCircBuf->read_offset = decCircBuf->write_offset;
  cmdPayload.voicemem_handle = session_obj->voicemem_handle;
  cmdPayload.virt_addr = (void*)&decCircBuf->read_offset;
  cmdPayload.size = sizeof(uint64_t);
  ( void ) voicemem_call( VOICEMEM_CMD_CACHE_FLUSH, &cmdPayload, sizeof( cmdPayload ) );
  if( vs_platform_is_fusion )
  {
    /* Move the read offset update from device to host. */
#if (!defined AVS_MPSS_TEST)
    device_addr = (uint64_t) qurt_lookup_physaddr( (qurt_addr_t)(&decCircBuf->read_offset) );
    params.smem_addr = device_addr;
    params.mem_handle = session_obj->voicemem_handle;
    params.adsp_iova = NULL;
    params.pcie_iova = &host_iova;
    ( void )voicemem_call ( VOICEMEM_CMD_GET_IOVA, &params, sizeof(params) );

    req.device_addr = device_addr;
    req.host_addr = host_iova;
    req.size = sizeof(uint64_t);
    req.options = 0;
    req.user_data = (void*)NULL;

#if defined (VS_PROFILING_DATA_PCIE)
  vs_data_begin_ticks_in_us = (uint64_t)((qurt_sysclock_get_hw_ticks()) *10ull/192ull);
#endif /* VS_PROFILING_DATA_PCIE */

    ipa_dma_device_to_host( session_obj->ipa_dl_handle, &req);

#if defined (VS_PROFILING_DATA_PCIE)
  vs_data_end_ticks_in_us = (uint64_t)((qurt_sysclock_get_hw_ticks()) *10ull/192ull);
  MSG_4( MSG_SSID_DFLT, MSG_LEGACY_HIGH,
         "VS_PROFILING-PCIe: FLUSH_DEC_MAILBOX(): write decCircBuf->read_offset: MAP_MEMORY: MODEM_to_ADSP: tick1_us=(%lu), tick2_us =(%lu), diff_us=(%lu), size=(%d)",
         vs_data_begin_ticks_in_us, vs_data_end_ticks_in_us, 
         (vs_data_end_ticks_in_us - vs_data_begin_ticks_in_us), req.size );
#endif /* VS_PROFILING_DATA_PCIE */

#else
    device_addr = (uint64_t)&decCircBuf->read_offset;
    params.smem_addr = device_addr;
    params.mem_handle = session_obj->voicemem_handle;
    params.adsp_iova = NULL;
    params.pcie_iova = &host_iova;
    ( void )voicemem_call ( VOICEMEM_CMD_GET_IOVA, &params, sizeof(params) );

    mmstd_memcpy( (void*)host_iova  , sizeof(uint64_t),
                  (void*)&decCircBuf->read_offset , sizeof(uint64_t) );
#endif
  }
  return rc;
}

uint32_t vs_voc_action_initialize_mbox (
  vs_session_object_t* session_obj
)
{
  uint8_t index = 0;
  uint32_t shmem_offset = 0; uint64_t virtAddr = 0;
  uint64_t shmem_virt_addr_start = session_obj->mem_virt_addr_start;
  uint64_t shmem_phy_addr_start = session_obj->mem_phy_addr_start;
  mailbox_voc_req_circ_buffer_t* encCircBuf = NULL;
  mailbox_voc_req_circ_buffer_t* decCircBuf = NULL;
  voicemem_cmd_cache_flush_t cmdPayload;
  uint64_t host_iova;
  voicemem_cmd_get_iova_t params;
#if (!defined AVS_MPSS_TEST)
   ipa_dma_req_t req;
#endif

  mmstd_memset((void*)session_obj->mem_virt_addr_start, 0, session_obj->mem_size);
  //Initialize HPCM shared memory.
  session_obj->hpcm.txReadBlk.size = VS_HPCM_BUF_SIZE;
  session_obj->hpcm.txReadBlk.virtAddr = shmem_virt_addr_start + shmem_offset;
  session_obj->hpcm.txReadBlk.phyAddr = shmem_phy_addr_start + shmem_offset;
  shmem_offset +=VS_ROUND_UP_ALIGN( VS_HPCM_BUF_SIZE, VS_CACHE_LINE_SIZE );
  session_obj->hpcm.txReadBuf.max_size = VS_HPCM_BUF_SIZE;
  session_obj->hpcm.txReadBuf.data = (uint8_t*)session_obj->hpcm.txReadBlk.virtAddr;

  session_obj->hpcm.txWriteBlk.size = VS_HPCM_BUF_SIZE;
  session_obj->hpcm.txWriteBlk.virtAddr = shmem_virt_addr_start + shmem_offset;
  session_obj->hpcm.txWriteBlk.phyAddr = shmem_phy_addr_start + shmem_offset;
  shmem_offset +=VS_ROUND_UP_ALIGN( VS_HPCM_BUF_SIZE, VS_CACHE_LINE_SIZE );
  session_obj->hpcm.txWriteBuf.max_size = VS_HPCM_BUF_SIZE;
  session_obj->hpcm.txWriteBuf.data = (uint8_t*)session_obj->hpcm.txWriteBlk.virtAddr;

  session_obj->hpcm.rxReadBlk.size = VS_HPCM_BUF_SIZE;
  session_obj->hpcm.rxReadBlk.virtAddr = shmem_virt_addr_start + shmem_offset;
  session_obj->hpcm.rxReadBlk.phyAddr = shmem_phy_addr_start + shmem_offset;
  shmem_offset +=VS_ROUND_UP_ALIGN( VS_HPCM_BUF_SIZE, VS_CACHE_LINE_SIZE );
  session_obj->hpcm.rxReadBuf.max_size = VS_HPCM_BUF_SIZE;
  session_obj->hpcm.rxReadBuf.data = (uint8_t*)session_obj->hpcm.rxReadBlk.virtAddr;

  session_obj->hpcm.rxWriteBlk.size = VS_HPCM_BUF_SIZE;
  session_obj->hpcm.rxWriteBlk.virtAddr = shmem_virt_addr_start + shmem_offset;
  session_obj->hpcm.rxWriteBlk.phyAddr = shmem_phy_addr_start + shmem_offset;
  shmem_offset +=VS_ROUND_UP_ALIGN( VS_HPCM_BUF_SIZE, VS_CACHE_LINE_SIZE );
  session_obj->hpcm.rxWriteBuf.max_size = VS_HPCM_BUF_SIZE;
  session_obj->hpcm.rxWriteBuf.data = (uint8_t*)session_obj->hpcm.rxWriteBlk.virtAddr;

  //Initialized shared memory for mailbox
  gpr_list_init_v2( &session_obj->vocoder.mbox_config.usedEncBufferQ,
                    vs_cmdq_lock_fn, vs_cmdq_unlock_fn );
  gpr_list_init_v2( &session_obj->vocoder.mbox_config.freeEncBufferQ,
                    vs_cmdq_lock_fn, vs_cmdq_unlock_fn );
  gpr_list_init_v2( &session_obj->vocoder.mbox_config.usedDecBufferQ,
                    vs_cmdq_lock_fn, vs_cmdq_unlock_fn );
  gpr_list_init_v2( &session_obj->vocoder.mbox_config.freeDecBufferQ,
                    vs_cmdq_lock_fn, vs_cmdq_unlock_fn );

  for( index = 0; index<VS_MAX_NUM_OF_MBOX_BUF_REQUESTS; index++ )
  {
    gpr_list_init_node( &session_obj->vocoder.mbox_config.encBuffers[index].link );
    gpr_list_add_tail( &session_obj->vocoder.mbox_config.freeEncBufferQ,
                       &session_obj->vocoder.mbox_config.encBuffers[index].link);

    gpr_list_init_node( &session_obj->vocoder.mbox_config.decBuffers[index].link );
    gpr_list_add_tail( &session_obj->vocoder.mbox_config.freeDecBufferQ,
                       &session_obj->vocoder.mbox_config.decBuffers[index].link);
  }

   //Carve out memory for Encoder circlar buffer.
  session_obj->vocoder.mbox_config.encCircBufDataSize = 
    ( VS_MAX_NUM_OF_MBOX_DATA_SLOTS * sizeof(mailbox_voc_request_t) );
  session_obj->vocoder.mbox_config.encCircBuf.size = 
  session_obj->vocoder.mbox_config.encCircBufDataSize + sizeof(mailbox_voc_req_circ_buffer_t);
  session_obj->vocoder.mbox_config.encCircBuf.virtAddr = shmem_virt_addr_start + shmem_offset;
  session_obj->vocoder.mbox_config.encCircBuf.phyAddr = shmem_phy_addr_start + shmem_offset;
  shmem_offset +=VS_ROUND_UP_ALIGN( session_obj->vocoder.mbox_config.encCircBuf.size,
                                    VS_CACHE_LINE_SIZE );

  //Carve out memory for Encoder buffers.
  for ( index = 0; index < VS_MAX_NUM_OF_MBOX_BUF_REQUESTS; index++ )
  {
    session_obj->vocoder.mbox_config.encBuffers[index].buf.size = VS_MBOX_BUF_SIZE;
    session_obj->vocoder.mbox_config.encBuffers[index].buf.virtAddr = shmem_virt_addr_start + shmem_offset;
    session_obj->vocoder.mbox_config.encBuffers[index].buf.phyAddr = shmem_phy_addr_start + shmem_offset;
    shmem_offset += VS_ROUND_UP_ALIGN( VS_MBOX_BUF_SIZE, VS_CACHE_LINE_SIZE );
  }

  //Carve out memory for Decoder circlar buffer.
  session_obj->vocoder.mbox_config.decCircBufDataSize = 
    ( VS_MAX_NUM_OF_MBOX_DATA_SLOTS * sizeof(mailbox_voc_request_t) );
  session_obj->vocoder.mbox_config.decCircBuf.size = 
  session_obj->vocoder.mbox_config.decCircBufDataSize + sizeof(mailbox_voc_req_circ_buffer_t);
  session_obj->vocoder.mbox_config.decCircBuf.virtAddr = shmem_virt_addr_start + shmem_offset;
  session_obj->vocoder.mbox_config.decCircBuf.phyAddr = shmem_phy_addr_start + shmem_offset;
  shmem_offset += VS_ROUND_UP_ALIGN( session_obj->vocoder.mbox_config.encCircBuf.size,
                                     VS_CACHE_LINE_SIZE );

  //Carve out memory for Decoder buffers.
  for ( index = 0; index < VS_MAX_NUM_OF_MBOX_BUF_REQUESTS; index++ )
  {
    session_obj->vocoder.mbox_config.decBuffers[index].buf.size = VS_MBOX_BUF_SIZE;
    session_obj->vocoder.mbox_config.decBuffers[index].buf.virtAddr = shmem_virt_addr_start + shmem_offset;
    session_obj->vocoder.mbox_config.decBuffers[index].buf.phyAddr = shmem_phy_addr_start + shmem_offset;
    shmem_offset += VS_ROUND_UP_ALIGN( VS_MBOX_BUF_SIZE, VS_CACHE_LINE_SIZE );
  }

  /* Clear Encoder circular buffer */
  virtAddr = session_obj->vocoder.mbox_config.encCircBuf.virtAddr;
  encCircBuf = (mailbox_voc_req_circ_buffer_t*)virtAddr;
  encCircBuf->read_offset = 0;
  encCircBuf->write_offset = 0;
  session_obj->vocoder.mbox_config.encReclaimOffset = 0;

  cmdPayload.voicemem_handle = session_obj->memmap_handle;
  cmdPayload.virt_addr = (void*)session_obj->vocoder.mbox_config.encCircBuf.virtAddr;
  cmdPayload.size = session_obj->vocoder.mbox_config.encCircBuf.size;
  ( void ) voicemem_call( VOICEMEM_CMD_CACHE_FLUSH, &cmdPayload, sizeof( cmdPayload ) );
  
  if ( TRUE == vs_platform_is_fusion )
  {
	
    params.mem_handle = session_obj->voicemem_handle;
    params.smem_addr = session_obj->vocoder.mbox_config.encCircBuf.phyAddr;
    params.adsp_iova = NULL;
    params.pcie_iova = &host_iova;
    ( void )voicemem_call ( VOICEMEM_CMD_GET_IOVA, &params, sizeof(params) );
  
#if (!defined AVS_MPSS_TEST)
    req.host_addr = host_iova;
    req.device_addr = session_obj->vocoder.mbox_config.encCircBuf.phyAddr;
    req.size = session_obj->vocoder.mbox_config.encCircBuf.size; 
    req.options = 0;
    req.user_data = NULL;

#if defined (VS_PROFILING_DATA_PCIE)
  vs_data_begin_ticks_in_us = (uint64_t)((qurt_sysclock_get_hw_ticks()) *10ull/192ull);
#endif

    ipa_dma_device_to_host( session_obj->ipa_ul_handle, &req);

#if defined (VS_PROFILING_DATA_PCIE)
  vs_data_end_ticks_in_us = (uint64_t)((qurt_sysclock_get_hw_ticks()) *10ull/192ull);
  MSG_4( MSG_SSID_DFLT, MSG_LEGACY_HIGH,
         "VS_PROFILING-PCIe: Initialize_mailobx(): write memmap_table: MODEM_to_ADSP: tick1_us=(%lu), tick2_us =(%lu), diff_us=(%lu), size=(%d)",
         vs_data_begin_ticks_in_us, vs_data_end_ticks_in_us, 
         (vs_data_end_ticks_in_us - vs_data_begin_ticks_in_us), req.size );
#endif 
	
#else
	mmstd_memcpy( (void*)host_iova, session_obj->vocoder.mbox_config.encCircBuf.size,
                  (void*)session_obj->vocoder.mbox_config.encCircBuf.phyAddr , session_obj->vocoder.mbox_config.encCircBuf.size );
#endif
  }
  
  /* Clear Decoder circular buffer */
  virtAddr = session_obj->vocoder.mbox_config.decCircBuf.virtAddr;
  decCircBuf = (mailbox_voc_req_circ_buffer_t*)virtAddr;
  decCircBuf->read_offset = 0;
  decCircBuf->write_offset = 0;
  session_obj->vocoder.mbox_config.decReclaimOffset = 0;
  
  cmdPayload.voicemem_handle = session_obj->voicemem_handle;
  cmdPayload.virt_addr = (void*)session_obj->vocoder.mbox_config.decCircBuf.virtAddr;
  cmdPayload.size = session_obj->vocoder.mbox_config.decCircBuf.size;
  ( void ) voicemem_call( VOICEMEM_CMD_CACHE_FLUSH, &cmdPayload, sizeof( cmdPayload ) );
  
  if ( TRUE == vs_platform_is_fusion )
  {
    params.mem_handle = session_obj->voicemem_handle;
    params.smem_addr = session_obj->vocoder.mbox_config.decCircBuf.phyAddr;
    params.adsp_iova = NULL;
    params.pcie_iova = &host_iova;
    ( void )voicemem_call ( VOICEMEM_CMD_GET_IOVA, &params, sizeof(params) );
  
#if (!defined AVS_MPSS_TEST)
  req.host_addr = host_iova;
  req.device_addr = session_obj->vocoder.mbox_config.decCircBuf.phyAddr;
  req.size = session_obj->vocoder.mbox_config.decCircBuf.size;
  req.options = 0;
  req.user_data = NULL;

#if defined (VS_PROFILING_DATA_PCIE)
  vs_data_begin_ticks_in_us = (uint64_t)((qurt_sysclock_get_hw_ticks()) *10ull/192ull);
#endif

  ipa_dma_device_to_host( session_obj->ipa_dl_handle, &req);
 
#if defined (VS_PROFILING_DATA_PCIE)
  vs_data_end_ticks_in_us = (uint64_t)((qurt_sysclock_get_hw_ticks()) *10ull/192ull);
  MSG_4( MSG_SSID_DFLT, MSG_LEGACY_HIGH,
         "VS_PROFILING-PCIe: Initialize_mailobx(): write memmap_table: MODEM_to_ADSP: tick1_us=(%lu), tick2_us =(%lu), diff_us=(%lu), size=(%d)",
         vs_data_begin_ticks_in_us, vs_data_end_ticks_in_us, 
         (vs_data_end_ticks_in_us - vs_data_begin_ticks_in_us), req.size );
#endif 

#else
  
  mmstd_memcpy( (void*)host_iova, session_obj->vocoder.mbox_config.decCircBuf.size,
                 (void*)session_obj->vocoder.mbox_config.decCircBuf.phyAddr , session_obj->vocoder.mbox_config.decCircBuf.size );
#endif
  }

  for ( index = 0; index < VS_MAX_NUM_OF_MBOX_BUF_REQUESTS; index++ )
  {
    //Queue encoding request after mbox is intialized
    ( void ) vs_voc_action_put_mailbox_enc_request ( session_obj );
  }

  return VS_EOK;
}

