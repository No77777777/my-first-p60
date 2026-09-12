/**
  @file vs_vocevs.c
  @brief This file contains the definitions of the interfaces for EVS vocoder.
*/

/*
  ============================================================================
  Copyright (C) 2016-2021 Qualcomm Technologies, Inc.
  All Rights Reserved.
  Confidential and Proprietary - Qualcomm Technologies, Inc.
  ============================================================================
  $Header: //components/rel/avs.mpss/9.0.c51/vsd/vs/src/vs_vocevs.c#1 $
  $Author: pwbldsvc $
  ============================================================================
*/

/****************************************************************************
 * INCLUDE HEADER FILES                                                     *
 ****************************************************************************/

#include "vs_i.h"

#include "event.h"

/****************************************************************************
 * FUNCTIONS                                                                *
 ****************************************************************************/

static uint32_t vs_vocevs_action_set_channel_awareness_mode ( 
   vs_session_object_t* session_obj
)
{
   uint32_t rc =VS_EOK;
   gpr_cmd_alloc_ext_t ca_params;
   gpr_packet_t* packet = NULL;
   apm_cmd_header_t *cmd_header;
   apm_module_param_data_t *param_data;
   param_id_evs_enc_channel_aware_mode_t *ch_mode;
    
   ca_params.src_domain_id = GPR_IDS_DOMAIN_ID_MODEM_V ;
   ca_params.src_port = VS_MODULE_INSTANCE_ID;
   ca_params.dst_domain_id = GPR_IDS_DOMAIN_ID_ADSP_V ; 
   ca_params.dst_port = session_obj->mod_iid_tag_info[VS_MOD_ENCODER].mod_iid ;
   ca_params.token = session_obj->header.handle;
   ca_params.opcode = APM_CMD_SET_CFG;
   ca_params.payload_size = sizeof(apm_cmd_header_t) + sizeof(apm_module_param_data_t) + sizeof(param_id_evs_enc_channel_aware_mode_t);
   ca_params.client_data = 0;
   ca_params.ret_packet =&packet ;
  
   rc = __gpr_cmd_alloc_ext(&ca_params);

   if( packet == NULL )
    {
      VS_REPORT_FATAL_ON_ERROR( rc );
      return VS_EFAILED;
    }
  
   cmd_header = GPR_PKT_GET_PAYLOAD( apm_cmd_header_t, packet );
   cmd_header->mem_map_handle = 0;
   cmd_header->payload_address_lsw = 0;
   cmd_header->payload_address_msw = 0;
   cmd_header->payload_size = sizeof(apm_module_param_data_t) + sizeof(param_id_evs_enc_channel_aware_mode_t);
  
   param_data = (apm_module_param_data_t*)( (uint8_t*)cmd_header + sizeof(apm_cmd_header_t) );
   param_data->module_instance_id = session_obj->mod_iid_tag_info[VS_MOD_ENCODER].mod_iid;
   param_data->param_id = PARAM_ID_EVS_ENC_CHANNEL_AWARE_MODE;
   param_data->param_size = sizeof(param_id_evs_enc_channel_aware_mode_t);

   ch_mode = (param_id_evs_enc_channel_aware_mode_t*)( (uint8_t*)param_data + sizeof(apm_module_param_data_t));
   ch_mode->enable = session_obj->vocoder.channel_awareness_mode;
   ch_mode->fec_offset = session_obj->vocoder.fec_offset;
   ch_mode->fer_rate = session_obj->vocoder.fer_rate;

   rc = __gpr_cmd_async_send(packet);
    if ( rc )
    {
    VS_REPORT_FATAL_ON_ERROR(rc);
    MSG_1( MSG_SSID_DFLT, MSG_LEGACY_ERROR,
           "VS:vs_vocevs_action_set_channel_awareness_mode(): Failed to communicate with encoder module while setting CA mode: rc=0x%08x", rc );
    ( void )__gpr_cmd_free(packet);
    }
  return rc;
}

static uint32_t vs_vocevs_action_set_operating_mode ( 
   vs_session_object_t* session_obj
)
{
  uint32_t rc = VS_EOK;
   gpr_cmd_alloc_ext_t mode_params;
   gpr_packet_t* packet = NULL;
   apm_cmd_header_t *cmd_header;
   apm_module_param_data_t *param_data;
   param_id_evs_enc_mode_t *enc_mode;
  
   if( VS_PARAM_UINT32_NOT_SET == session_obj->vocoder.dtx )
   {
      return VS_EOK;
   }
  
   mode_params.src_domain_id = GPR_IDS_DOMAIN_ID_MODEM_V ;
   mode_params.src_port = VS_MODULE_INSTANCE_ID;
   mode_params.dst_domain_id = GPR_IDS_DOMAIN_ID_ADSP_V ; 
   mode_params.dst_port = session_obj->mod_iid_tag_info[VS_MOD_ENCODER].mod_iid ;
   mode_params.token = session_obj->header.handle;
   mode_params.opcode = APM_CMD_SET_CFG;
   mode_params.payload_size = sizeof(apm_cmd_header_t) + sizeof(apm_module_param_data_t) + sizeof(param_id_evs_enc_mode_t);
   mode_params.client_data = 0;
   mode_params.ret_packet =&packet ;
  
   rc = __gpr_cmd_alloc_ext(&mode_params);

   if( packet == NULL )
   {
      VS_REPORT_FATAL_ON_ERROR( rc );
      return VS_EFAILED;
   }
  
   cmd_header = GPR_PKT_GET_PAYLOAD( apm_cmd_header_t, packet );
   cmd_header->mem_map_handle = 0;
   cmd_header->payload_address_lsw = 0;
   cmd_header->payload_address_msw = 0;
   cmd_header->payload_size = sizeof(apm_module_param_data_t) + sizeof(param_id_evs_enc_mode_t);
  
   param_data = (apm_module_param_data_t*)( (uint8_t*)cmd_header + sizeof(apm_cmd_header_t) );
   param_data->module_instance_id = session_obj->mod_iid_tag_info[VS_MOD_ENCODER].mod_iid;
   param_data->param_id = PARAM_ID_EVS_ENC_MODE;
   param_data->param_size = sizeof(param_id_evs_enc_mode_t);

   enc_mode = (param_id_evs_enc_mode_t*)( (uint8_t*)param_data + sizeof(apm_module_param_data_t));
   enc_mode->bit_rate = session_obj->vocoder.encoder_rate;
   enc_mode->bandwidth = session_obj->vocoder.encoder_bw;

   rc = __gpr_cmd_async_send(packet);
    if ( rc )
    {
    VS_REPORT_FATAL_ON_ERROR(rc);
    MSG_1( MSG_SSID_DFLT, MSG_LEGACY_ERROR,
           "VS:vs_vocevs_action_set_operating_mode(): Failed to communicate with encoder module while setting operating mode: rc=0x%08x", rc );
    ( void )__gpr_cmd_free(packet);
    }
  return rc;
}


static uint32_t vs_vocevs_publish_params (
  void* this
)
{
  vs_session_object_t* session_obj = (vs_session_object_t*)this;
  vs_evs_params_t event_payload;

  /* populate and publish EVS params. */
  event_payload.vsid = session_obj->vsid;
  event_payload.encoder_bandwidth = session_obj->vocoder.encoder_bw;
  event_payload.decoder_bandwidth = session_obj->vocoder.decoder_bw;
  event_payload.encoder_dtx = (uint8) session_obj->vocoder.dtx;
  event_payload.encoder_rate = session_obj->vocoder.encoder_rate;
  event_payload.channel_awareness_mode = session_obj->vocoder.channel_awareness_mode;
  event_payload.fec_offset = session_obj->vocoder.fec_offset;
  event_payload.fer_rate = session_obj->vocoder.fer_rate;

  ( void ) event_report_payload( /*(event_id_enum_type)*/ VS_EVENT_VOICE_EVS_PARAMS,
                                  sizeof(event_payload), &event_payload );

  return VS_EOK;
}


static bool_t vs_vocevs_is_codec_mode_and_bandwidth_valid (
  vs_vocevs_codec_mode_t codec_mode,
  vs_voc_bandwidth_t bandwidth
)
{
  switch ( bandwidth )
  {
    case VS_VOC_BANDWIDTH_NB:
    {
      if ( codec_mode < VS_VOCEVS_CODEC_MODE_0590 || codec_mode > VS_VOCEVS_CODEC_MODE_2440 )
      {
        return FALSE;
      }
    }
    break;

   case VS_VOC_BANDWIDTH_WB:
   {
     if ( codec_mode < VS_VOCEVS_CODEC_MODE_0660 || codec_mode > VS_VOCEVS_CODEC_MODE_12800 )
     {
       return FALSE;
     }
   }
   break;

   case VS_VOC_BANDWIDTH_SWB:
   {
     if ( codec_mode < VS_VOCEVS_CODEC_MODE_0960 || codec_mode > VS_VOCEVS_CODEC_MODE_12800 )
     {
       return FALSE;
     }
   }
   break;

   case VS_VOC_BANDWIDTH_FB:
   {
     if ( codec_mode < VS_VOCEVS_CODEC_MODE_1640 || codec_mode > VS_VOCEVS_CODEC_MODE_12800 )
     {
       return FALSE;
     }
   }
   break;

   default:
     return FALSE;
     break;
  }

  return TRUE;
} 


static uint32_t vs_vocevs_cache_max_pp_sample_rate (
  vs_session_object_t* session_obj,
  vs_param_max_pp_sample_rate_t* sampleRate
)
{
  uint32_t rc = VS_EOK;
  uint32_t tx_sr = sampleRate->tx_pp_sample_rate;
  uint32_t rx_sr = sampleRate->rx_pp_sample_rate;

  MSG_3( MSG_SSID_DFLT, MSG_LEGACY_HIGH,
        "SET_PARAM: VS_PARAM_MAX_PP_SAMPLE_RATE(EVS): tx_pp_sample_rate=(%d), rx_pp_sample_rate=(%d) "
        "(8k, 16k, 32k, 48K), is_stream_ready=(%d)", tx_sr, rx_sr, session_obj->is_stream_ready  );

  switch ( rx_sr)
  {
    case 8000:
    case 16000:
    case 32000:
    case 48000:
      break;

    default:
      return VS_EBADPARAM;
      break;
  }

  switch ( tx_sr)
  {
    case 8000:
    case 16000:
    case 32000:
    case 48000:
      break;

    default:
      return VS_EBADPARAM;
      break;
  }

  session_obj->vocoder.tx_sample_rate = tx_sr;
  session_obj->vocoder.rx_sample_rate = rx_sr;

  return rc;
}


static uint32_t vs_vocevs_cache_enc_operating_mode (
  vs_session_object_t* session_obj,
  vs_param_enc_operating_mode_t* encOperatingMode
)
{
  uint32_t rc = VS_EOK;
  uint8_t bandwidth = encOperatingMode->bandwidth;
  uint8_t codecMode = encOperatingMode->rate;

  MSG_3( MSG_SSID_DFLT, MSG_LEGACY_HIGH,
        "SET_PARAM: VS_PARAM_ENC_OPERATING_MODE(EVS): bandwidth=(%d) (0-3), codec_mode=(%d) (0-20)"
        "is_stream_ready=(%d)", bandwidth, codecMode, session_obj->is_stream_ready  );

  if ( FALSE == vs_vocevs_is_codec_mode_and_bandwidth_valid ( codecMode, bandwidth  ) )
  {
    MSG_2( MSG_SSID_DFLT, MSG_LEGACY_ERROR,
           "VS: EVS: Invalid codecMode=(%d) for BandWidth=(%d)", codecMode, bandwidth );
    session_obj->vocoder.encoder_bw = VS_PARAM_UINT8_NOT_SET;
    session_obj->vocoder.encoder_rate = VS_PARAM_UINT8_NOT_SET;
    return VS_EBADPARAM;
  }

  /* Channel awareness is not applicable to non 13.2 kbps mode. */
  if( VS_VOCEVS_CODEC_MODE_1320 != session_obj->vocoder.encoder_rate )
  {
    session_obj->vocoder.channel_awareness_mode = VS_PARAM_UINT8_NOT_APPLICABLE;
    session_obj->vocoder.fec_offset = VS_PARAM_UINT8_NOT_APPLICABLE;
    session_obj->vocoder.fer_rate = VS_PARAM_UINT8_NOT_APPLICABLE;
  }

  session_obj->vocoder.encoder_rate = codecMode;
  session_obj->vocoder.encoder_bw = bandwidth;

  if( TRUE == session_obj->is_stream_ready )
  {
    ( void ) vs_vocevs_action_set_operating_mode( session_obj );
    ( void ) vs_vocevs_publish_params( session_obj );
  }

  return rc;
}

static uint32_t vs_vocevs_cache_channel_awareness_mode (
  vs_session_object_t* session_obj,
  vs_param_enc_channel_aware_mode_t* chAwarenessMode
)
{
  uint32_t rc = VS_EOK;
  uint8_t fer_rate = chAwarenessMode->fer_rate;
  uint8_t fec_offset = chAwarenessMode->fec_offset;
  uint8_t ch_enable = chAwarenessMode->channel_awareness_mode;

  MSG_4( MSG_SSID_DFLT, MSG_LEGACY_HIGH,
        "SET_PARAM: VS_PARAM_ENC_CHANNEL_AWARE_MODE(EVS): ch_enable=(%d) (0,1), fer_rate=(%d) (0,1)"
        "fec_offset=(%d) (2,3,5,7), is_stream_ready=(%d)", ch_enable, fer_rate, fec_offset, session_obj->is_stream_ready  );

  switch ( ch_enable )
  {
    /* CHANNEL_AWARE_DISABLE */
    case 0:
    {
      /* Do nothing */
      break;
    }

    /* CHANNEL_AWARE_ENABLE */
    case 1:
    {
      /* Validate the channel aware parameters */

  switch ( fer_rate )
  {
    case 0:
    case 1:
      break;

    default:
      session_obj->vocoder.fer_rate = VS_PARAM_UINT8_NOT_SET;
          MSG_2( MSG_SSID_DFLT, MSG_LEGACY_ERROR,
                 "VS: EVS: Invalid Channel Aware parameters: fer_rate=(%d), fec_offset=(%d)",
                 fer_rate, fec_offset );
      return VS_EBADPARAM;
      break;
  }

  switch ( fec_offset )
  {
    case 2:
    case 3:
    case 5:
    case 7:
      break;

    default:
      session_obj->vocoder.fec_offset = VS_PARAM_UINT8_NOT_SET;
          MSG_2( MSG_SSID_DFLT, MSG_LEGACY_ERROR,
                 "VS: EVS: Invalid Channel Aware parameters: fer_rate=(%d), fec_offset=(%d)",
                 fer_rate, fec_offset );
          return VS_EBADPARAM;
          break;
      }

      break;
    }

    default:
      session_obj->vocoder.channel_awareness_mode = VS_PARAM_UINT8_NOT_SET;
      MSG_3( MSG_SSID_DFLT, MSG_LEGACY_ERROR,
             "VS: EVS: Invalid Channel Aware parameters: ch_enable=(%d), "
             "fer_rate=(%d), fec_offset=(%d)", ch_enable, fer_rate, fec_offset );
      return VS_EBADPARAM;
      break;
  }

  session_obj->vocoder.channel_awareness_mode = ch_enable;
  session_obj->vocoder.fec_offset = fec_offset;
  session_obj->vocoder.fer_rate = fer_rate;

  if( FALSE == session_obj->vocoder.channel_awareness_mode )
  { //reset the vocoder properties to default.
    session_obj->vocoder.fec_offset = VS_PARAM_UINT8_NOT_SET;
    session_obj->vocoder.fer_rate = VS_PARAM_UINT8_NOT_SET;
  }

  if( TRUE == session_obj->is_stream_ready )
  {
    ( void ) vs_vocevs_action_set_channel_awareness_mode( session_obj );
    ( void ) vs_vocevs_publish_params( session_obj );
  }

  return rc;
}


static uint32_t vs_vocevs_set_cached_params_handler (
   void* this
)
{
  vs_session_object_t* session_obj = (vs_session_object_t*)this;

  ( void ) vs_voc_action_set_evs_dtx( session_obj );
  ( void ) vs_vocevs_action_set_operating_mode( session_obj );
  ( void ) vs_vocevs_action_set_channel_awareness_mode( session_obj );

  return VS_EOK;
}
 
 
static uint32_t vs_vocevs_set_param_handler (
 void* this,
 uint32_t param_id,
 void* param_data
)
{
  uint32_t rc = VS_EOK;
  vs_session_object_t* session_obj = (vs_session_object_t*)this;

  switch( param_id )
  {
    case VS_PARAM_ENC_OPERATING_MODE:
     rc = vs_vocevs_cache_enc_operating_mode( session_obj, (vs_param_enc_operating_mode_t*)param_data );
     break;

    case VS_PARAM_ENC_CHANNEL_AWARE_MODE:
     rc =  vs_vocevs_cache_channel_awareness_mode( session_obj, (vs_param_enc_channel_aware_mode_t*)param_data );
     break;

    case VS_PARAM_MAX_PP_SAMPLE_RATE:
     rc = vs_vocevs_cache_max_pp_sample_rate( session_obj, (vs_param_max_pp_sample_rate_t*)param_data );
     break;

    default:
      rc = VS_EUNSUPPORTED;
      break;
  }

  return rc;
}


VS_INTERNAL uint32_t vs_vocevs_init (
 vs_session_object_t* session_obj
)
{
  uint32_t rc = VS_EOK;

  session_obj->vocoder.enc_module_id = VS_VOCEVS_ENC_MODULE_ID;
  session_obj->vocoder.dec_module_id = VS_VOCEVS_DEC_MODULE_ID;
  session_obj->vocoder.set_param_fn = vs_vocevs_set_param_handler;
  session_obj->vocoder.set_cached_param_fn = vs_vocevs_set_cached_params_handler;
  session_obj->vocoder.publish_param_fn = vs_vocevs_publish_params;

  return rc;
}


