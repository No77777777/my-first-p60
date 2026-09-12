/**
  @file vs_event_handlers.c
  @brief This file contains the definitions of the internal functions of VS.
*/

/*
  Copyright (C) 2018-2021 QUALCOMM Technologies, Inc.
  All Rights Reserved.
  Confidential and Proprietary - Qualcomm Technologies, Inc.

  $Header: //components/rel/avs.mpss/9.0.c51/vsd/vs/src/vs_event_handlers.c#1 $
  $Author: pwbldsvc $
*/

/****************************************************************************
 * HEADER FILES
 ****************************************************************************/
#include "vs_i.h"


/****************************************************************************
 * VS EVENT HANDLERS
 ****************************************************************************/

uint32_t vs_module_event_handler (
  gpr_packet_t* packet
)
{
  uint32_t rc = VS_EOK;
  apm_module_event_t* event_ptr = (apm_module_event_t*) (packet + 1);
  switch (event_ptr->event_id)
  {
  case EVENT_ID_VOCODER_OPERATING_MODE:
    rc = vs_stream_vocoder_mode_change_event_handler( packet );
    break;
  
  default:
    rc = __gpr_cmd_free( packet );
    VS_REPORT_FATAL_ON_ERROR(rc);
    MSG_1( MSG_SSID_DFLT, MSG_LEGACY_ERROR,
            "VS: vs_module_event_handler: Unsupported event_id=(0x%08X)", event_ptr->event_id );
    break;
  }

  return rc;
}


uint32_t vs_stream_ready_event_handler (
  gpr_packet_t* packet
)
{
  uint32_t rc = VS_EOK;
  vs_session_object_t* session_obj = NULL;

  for ( ;; )
  {
    MSG_3( MSG_SSID_DFLT, MSG_LEGACY_HIGH,
           "VS: VSS_ISTREAM_EVT_READY: src_port[0x%08x], dst_port[0x%08x], token[0x%08x]",
            packet->src_port, packet->dst_port, packet->token );

    /* Get the session object with respect to the handle */
    rc = vs_get_typed_object( packet->dst_port, VS_OBJECT_TYPE_ENUM_SESSION,
                            (vs_object_t**)&session_obj );
    if ( NULL == session_obj )
    {
      VS_REPORT_FATAL_ON_ERROR( rc );
      break;
    }

    VS_ACQUIRE_LOCK( session_obj->session_lock );

    session_obj->is_stream_ready = TRUE;
    vs_common_send_event( session_obj->header.handle, VS_VOC_EVENT_READY, NULL, 0 );

    /* Reapply cached params. */
     if( NULL !=  session_obj->vocoder.set_cached_param_fn )
    {
      rc = session_obj->vocoder.set_cached_param_fn( session_obj );
    }

    VS_RELEASE_LOCK( session_obj->session_lock );

    break;
  }

  rc =  __gpr_cmd_free( packet );
  VS_REPORT_FATAL_ON_ERROR( rc );

  return VS_EOK;
}


uint32_t vs_stream_not_ready_event_handler (
  gpr_packet_t* packet
)
{
  uint32_t rc = VS_EOK;
  vs_session_object_t* session_obj = NULL;

  for ( ;; )
  {
    MSG_3( MSG_SSID_DFLT, MSG_LEGACY_HIGH,
           "VS: STREAM_NOT_READY: src_port[0x%08x], dst_port[0x%08x], token[0x%08x]",
           packet->src_port, packet->dst_port, packet->token );

    /* Get the session object with respect to the handle */
    rc = vs_get_typed_object( packet->dst_port, VS_OBJECT_TYPE_ENUM_SESSION,
                              (vs_object_t**)&session_obj);
    if ( NULL == session_obj )
    {
      VS_REPORT_FATAL_ON_ERROR( rc );
      break;
    }

    VS_ACQUIRE_LOCK( session_obj->session_lock );
    session_obj->is_stream_ready = FALSE;
    vs_common_send_event( session_obj->header.handle, VS_VOC_EVENT_NOT_READY, NULL, 0 ) ;
    VS_RELEASE_LOCK( session_obj->session_lock );

    break;
  }

  rc =  __gpr_cmd_free( packet );
  VS_REPORT_FATAL_ON_ERROR( rc );

  return VS_EOK;
}

uint32_t vs_stream_vocoder_mode_change_event_handler (
  gpr_packet_t* packet
)
{
  uint32_t rc = VS_EOK;
  vs_session_object_t* session_obj = NULL;
  vs_simple_job_object_t* simple_obj = NULL;
  vs_voc_event_operating_mode_update_t mode;
  apm_module_event_t* payload = NULL; 

  for ( ;; )
  {
    MSG_3( MSG_SSID_DFLT, MSG_LEGACY_HIGH,
           "VS: VOCODER_MODE_CHANGED: src_port[0x%08x] dst_port[0x%08x] token[0x%08x]",
           packet->src_port, packet->dst_port, packet->token );
    
    payload = GPR_PKT_GET_PAYLOAD( apm_module_event_t, packet );
    
    /* Get the session object corresponding to the VSID received */
    for(uint32_t index = 0; index < VS_MAX_NUM_SESSIONS_V; index++)
    {
      session_obj = vs_session_obj_list[index];
      if ( ( session_obj != NULL ) && (((event_id_vocoder_operating_mode_t*)(payload + 1))->client_data == session_obj->vsid) ) /* mailbox will send VSID as token in GPR packet */
      {
        break;
      }
      else
      {
        session_obj = NULL;
      }
    }

    if( NULL == session_obj )
    {
      MSG_1( MSG_SSID_DFLT, MSG_LEGACY_ERROR,
              "VS: vs_stream_vocoder_mode_change_event_handler(): session not found, VSID received is (0x%08X)", packet->token );
      break;
    }

    mode.rx_mode = ((event_id_vocoder_operating_mode_t*)(payload + 1))->operating_mode;
    
    session_obj->vocoder.decoder_bw = ((event_id_vocoder_operating_mode_t*)(payload + 1))->operating_mode;

    if ( ( mode.rx_mode == VOCODER_OPERATING_MODE_NB ) ||
         ( mode.rx_mode == VOCODER_OPERATING_MODE_WB ) )
    {
      vs_common_send_event( session_obj->header.handle, VS_VOC_EVENT_OPERATING_MODE_UPDATE,
                            &mode, sizeof( vs_voc_event_operating_mode_update_t ) );
    }
    else
    {
      MSG_1( MSG_SSID_DFLT, MSG_LEGACY_ERROR,
             "VS: VOCODER_MODE_CHANGE: Invalid mode[0x%08x] for AMR", mode.rx_mode );
    }

    if (NULL != session_obj->vocoder.publish_param_fn)
    {
      session_obj->vocoder.publish_param_fn(session_obj);
    }
    else
    {
      MSG(MSG_SSID_DFLT, MSG_LEGACY_ERROR,
        "VS: VOCODER_MODE_CHANGED: failed to publish params - publish_param_fn is NULL");
    }

    break;
  }

  rc =  __gpr_cmd_free( packet );
  VS_REPORT_FATAL_ON_ERROR( rc );

  return VS_EOK;
}

#if 0
uint32_t vs_stream_dec_buf_request_event_handler (
  gpr_packet_t* packet
)
{
  uint32_t rc = VS_EOK;
  vs_session_object_t* session_obj = NULL;

  for ( ;; )
  {
    /* Get the session object with respect to the handle */
    rc = vs_get_typed_object( packet->dst_port, VS_OBJECT_TYPE_ENUM_SESSION,
                              (vs_object_t**)&session_obj );
    if ( NULL == session_obj )
    {
       VS_REPORT_FATAL_ON_ERROR( rc );
       break;
    }

    VS_ACQUIRE_LOCK( session_obj->session_lock );

    vs_common_send_event( session_obj->header.handle, VS_PKTEXG_EVENT_DECODER_BUFFER_REQUEST, NULL, 0 ) ;

    VS_RELEASE_LOCK( session_obj->session_lock );

    break;
  }

  rc =  __gpr_cmd_free( packet );
  VS_REPORT_FATAL_ON_ERROR( rc );

  return VS_EOK;
}


uint32_t vs_stream_enc_buf_ready_event_handler (
  gpr_packet_t* packet
)
{
  uint32_t rc = VS_EOK;
  vs_session_object_t* session_obj = NULL;

  for ( ;; )
  {
    /* Get the session object with respect to the handle */
    rc = vs_get_typed_object( packet->dst_port, VS_OBJECT_TYPE_ENUM_SESSION,
                              (vs_object_t**)&session_obj );
    if ( NULL == session_obj )
    {
       VS_REPORT_FATAL_ON_ERROR( rc );
       break;
    }

    VS_ACQUIRE_LOCK( session_obj->session_lock );

    ( void ) vs_common_send_event( session_obj->header.handle, VS_PKTEXG_EVENT_ENCODER_BUFFER_AVAILABLE, NULL, 0 ) ;

    VS_RELEASE_LOCK( session_obj->session_lock );

    break;
  }

  rc =  __gpr_cmd_free( packet );
  VS_REPORT_FATAL_ON_ERROR( rc );

  return VS_EOK;
}
#endif

uint32_t vs_stream_oobtty_char_notify_event_handler (
  gpr_packet_t* packet
)
{
  uint32_t rc = VS_EOK;
  vs_session_object_t* session_obj = NULL;
  vs_oobtty_event_notify_char_t event_Param;

  for ( ;; )
  {
    /* Get the session object with respect to the handle */
    rc = vs_get_typed_object( packet->token, VS_OBJECT_TYPE_ENUM_SESSION,
                              (vs_object_t**)&session_obj );  //this logic will fail
    if ( NULL == session_obj )
    {
       VS_REPORT_FATAL_ON_ERROR( rc );
       break;
    }

    event_Param.tty_char = GPR_PKT_GET_PAYLOAD( event_id_ltetty_char_t, packet )->tty_char;

    VS_ACQUIRE_LOCK( session_obj->session_lock );

    ( void ) vs_common_send_event( session_obj->header.handle,
                                   VS_OOBTTY_EVENT_NOTIFY_CHAR, &event_Param, sizeof(event_Param) ) ;

    VS_RELEASE_LOCK( session_obj->session_lock );

    break;
  }

  rc =  __gpr_cmd_free( packet );
  VS_REPORT_FATAL_ON_ERROR( rc );

  return VS_EOK;
}

uint32_t vs_path_delay_notify_event_handler (
  gpr_packet_t* packet
)
{
  uint32_t rc = VS_EOK;
  vs_session_object_t* session_obj = NULL;
  vcpm_event_path_delay_payload_t* payload;
  vs_voc_event_path_delay_t delay;
  
  for ( ;; )
  {
    /* Get the session object corresponding to the VSID received */
    for(uint32_t index = 0; index < VS_MAX_NUM_SESSIONS_V; index++)
    {
      session_obj = vs_session_obj_list[index];
      if ( ( session_obj != NULL ) && (packet->token == session_obj->vsid) ) /* VCPM will send VSID as token in GPR packet */
      {
        break;
      }
      else
      {
        session_obj = NULL;
      }
    }

    if( NULL == session_obj )
    {
      MSG_1( MSG_SSID_DFLT, MSG_LEGACY_ERROR,
              "VS: vs_path_delay_notify_event_handler(): session not found, VSID received is (0x%08X)", packet->token );
      break;
    }


    payload = GPR_PKT_GET_PAYLOAD( vcpm_event_path_delay_payload_t, packet );
    session_obj->path_delay.tx_path_delay_us =  payload->tx_path_delay_us + VS_VOC_TIMING_ENC_OFFSET_MARGIN_V;
    session_obj->path_delay.rx_path_delay_us =  payload->rx_path_delay_us + VS_VOC_TIMING_DEC_OFFSET_MARGIN_V;
    
    delay.tx_path_delay_us =  session_obj->path_delay.tx_path_delay_us;
    delay.rx_path_delay_us =  session_obj->path_delay.rx_path_delay_us;
    MSG_2( MSG_SSID_DFLT, MSG_LEGACY_HIGH,
             "VS: PATH_DELAY_NOTIFICATION: tx_delay_us:[%d] rx_delay_us:[%d]", 
			  delay.tx_path_delay_us, delay.rx_path_delay_us );
    ( void ) vs_common_send_event( session_obj->header.handle,
                                   VS_VOC_EVENT_PATH_DELAY, &delay, sizeof(delay) ) ;
    break;
  }

  rc =  __gpr_cmd_free( packet );
  VS_REPORT_FATAL_ON_ERROR( rc );
  return VS_EOK;
}

uint32_t vs_stream_hpcm_notify_event_handler (
  gpr_packet_t* packet
)
{
  uint32_t rc = VS_EOK;
  vs_session_object_t* session_obj = NULL;
  vs_hpcm_event_notify_buffer_t eventParam;
  vss_ivpcm_evt_notify_v2_t* payload = NULL;
  voicemem_cmd_cache_invalidate_t cmdPayload;
#if (!defined AVS_MPSS_TEST)   
  ipa_dma_req_t req;
#endif  
  voicemem_cmd_get_iova_t params;
  /* Host represents the APQ and device represents the SDX. 
   * So host IOVA represents the APQ DDR address and 
   * device address represent the SMEM address.
   */
  uint64_t host_iova;
  uint64_t device_addr;

  for ( ;; )
  {
    /* Get the session object with respect to the handle */
    rc = vs_get_typed_object( packet->dst_port, VS_OBJECT_TYPE_ENUM_SESSION,
                              (vs_object_t**)&session_obj );
    if ( NULL == session_obj )
    {
       VS_REPORT_FATAL_ON_ERROR( rc );
       break;
    }

    payload = GPR_PKT_GET_PAYLOAD( vss_ivpcm_evt_notify_v2_t, packet );

    VS_ACQUIRE_LOCK( session_obj->session_lock );

    if( VSS_IVPCM_TAP_POINT_RX_DEFAULT == payload->tap_point )
    {
      eventParam.notify_mask = payload->notify_mask;
      eventParam.tap_point_id = payload->tap_point;
      eventParam.read_buffer = &session_obj->hpcm.rxReadBuf;
      eventParam.write_buffer = &session_obj->hpcm.rxWriteBuf;

      //outbuffer is filled
      if( payload->notify_mask & VSS_HPCM_MASK_READ_BUFFER )
      {
        eventParam.write_buffer->data_size = payload->request_buf_size;

        eventParam.read_buffer->data_size = payload->filled_out_size;
        eventParam.read_buffer->num_channels =  payload->num_out_channels;
        eventParam.read_buffer->sample_rate = payload->sampling_rate;

        if( vs_platform_is_fusion )
        {
          device_addr = session_obj->hpcm.rxReadBlk.phyAddr;
          params.smem_addr = device_addr;
          params.mem_handle = session_obj->voicemem_handle;
          params.adsp_iova = NULL;
          params.pcie_iova = &host_iova;
          ( void )voicemem_call ( VOICEMEM_CMD_GET_IOVA, &params, sizeof(params) );
#if (!defined AVS_MPSS_TEST)
          req.device_addr = device_addr;
          req.host_addr = host_iova;
          req.size = session_obj->hpcm.rxReadBlk.size;
          req.options = 0;
          req.user_data = ( void* )NULL;
          ipa_dma_host_to_device(session_obj->ipa_dl_handle, &req);
#else
          mmstd_memcpy( ( void* )session_obj->hpcm.rxReadBlk.phyAddr, session_obj->hpcm.rxReadBlk.size, 
                        ( void* )host_iova , session_obj->hpcm.rxReadBlk.size );
#endif

        }
        cmdPayload.voicemem_handle = session_obj->voicemem_handle;
        cmdPayload.virt_addr = (void*)session_obj->hpcm.rxReadBlk.virtAddr;
        cmdPayload.size = session_obj->hpcm.rxReadBlk.size;
        ( void ) voicemem_call( VOICEMEM_CMD_CACHE_INVALIDATE, &cmdPayload, sizeof( cmdPayload ) );
      }
    }
    else if( VSS_IVPCM_TAP_POINT_TX_DEFAULT == payload->tap_point )
    {
      eventParam.notify_mask = payload->notify_mask;
      eventParam.tap_point_id = payload->tap_point;
      eventParam.read_buffer = &session_obj->hpcm.txReadBuf;
      eventParam.write_buffer = &session_obj->hpcm.txWriteBuf;

      //outbuffer is filled
      if( payload->notify_mask & VSS_HPCM_MASK_READ_BUFFER )
      {
        eventParam.write_buffer->data_size = payload->request_buf_size;

        eventParam.read_buffer->data_size = payload->filled_out_size;
        eventParam.read_buffer->num_channels =  payload->num_out_channels;
        eventParam.read_buffer->sample_rate = payload->sampling_rate;
        if( vs_platform_is_fusion )
        {
          device_addr = session_obj->hpcm.txReadBlk.phyAddr;
          params.smem_addr = device_addr;
          params.mem_handle = session_obj->voicemem_handle;
          params.adsp_iova = NULL;
          params.pcie_iova = &host_iova;
          ( void )voicemem_call ( VOICEMEM_CMD_GET_IOVA, &params, sizeof(params) );
#if (!defined AVS_MPSS_TEST)
          req.device_addr = device_addr;
          req.host_addr = host_iova;
          req.size = session_obj->hpcm.txReadBlk.size;
          req.options = 0;
          req.user_data = ( void* )NULL;
          ipa_dma_host_to_device(session_obj->ipa_ul_handle, &req);
#else
          mmstd_memcpy( ( void* )session_obj->hpcm.txReadBlk.phyAddr, session_obj->hpcm.txReadBlk.size, 
                       ( void* ) host_iova , session_obj->hpcm.txReadBlk.size );
#endif

        }
        cmdPayload.voicemem_handle = session_obj->voicemem_handle;
        cmdPayload.virt_addr = (void*)session_obj->hpcm.txReadBlk.virtAddr;
        cmdPayload.size = session_obj->hpcm.txReadBlk.size;
        ( void ) voicemem_call( VOICEMEM_CMD_CACHE_INVALIDATE, &cmdPayload, sizeof( cmdPayload ) );
      }
    }
    else
    {
      VS_REPORT_FATAL_ON_ERROR( VS_EUNEXPECTED );
      break;
    }

    ( void ) vs_common_send_event( session_obj->header.handle,
                                   VS_HPCM_EVENT_NOTIFY_BUFFER, &eventParam, sizeof(eventParam) ) ;

    VS_RELEASE_LOCK( session_obj->session_lock );

    break;
  }

  rc =  __gpr_cmd_free( packet );
  VS_REPORT_FATAL_ON_ERROR( rc );

  return VS_EOK;
}

uint32_t vs_stream_no_data_pkt_event_handler (
  gpr_packet_t* packet
)
{
  uint32_t rc = VS_EOK;
  vs_session_object_t* session_obj = NULL;

  for ( ;; )
  {
    MSG_3( MSG_SSID_DFLT, MSG_LEGACY_HIGH,
           "MAILBOX_PKTEXG_EVT_NO_DATA_PACKET received from: src_port[0x%08x], dst_port[0x%08x], token[0x%08x]",
            packet->src_port, packet->dst_port, packet->token );

    /* Get the session object corresponding to the VSID received */
    for(uint32_t index = 0; index < VS_MAX_NUM_SESSIONS_V; index++)
    {
      session_obj = vs_session_obj_list[index];
      if ( ( session_obj != NULL ) && (packet->token == session_obj->vsid) ) /* mailbox will send VSID as token in GPR packet */
      {
        break;
      }
      else
      {
        session_obj = NULL;
      }
    }

    if( NULL == session_obj )
    {
      MSG_1( MSG_SSID_DFLT, MSG_LEGACY_ERROR,
              "VS: vs_stream_no_data_pkt_event_handler(): session not found, VSID received is (0x%08X)", packet->token );
      break;
    }

    vs_common_send_event( session_obj->header.handle, VS_PKTEXG_EVENT_NO_DATA_PACKET, NULL, 0 );

    break;

  }

  rc =  __gpr_cmd_free( packet );
  VS_REPORT_FATAL_ON_ERROR( rc );

  return VS_EOK;
}

uint32_t vs_stream_resync_done_event_handler (
  gpr_packet_t* packet
)
{
  uint32_t rc = VS_EOK;
  vs_session_object_t* session_obj = NULL;
  vcpm_event_resync_done_t* payload = NULL; 
  vs_voc_event_resync_done_t event_param;

  for ( ;; )
  {
    MSG_3( MSG_SSID_DFLT, MSG_LEGACY_HIGH,
           "VS_VOC_EVENT_RESYNC_DONE received from: src_port[0x%08x], dst_port[0x%08x], token[0x%08x]",
            packet->src_port, packet->dst_port, packet->token );

    /* Get the session object corresponding to the VSID received */
    for(uint32_t index = 0; index < VS_MAX_NUM_SESSIONS_V; index++)
    {
      session_obj = vs_session_obj_list[index];
      if ( ( session_obj != NULL ) && (packet->token == session_obj->vsid) ) /* VCPM will send VSID as token in GPR packet */
      {
        break;
      }
      else
      {
        session_obj = NULL;
      }
    }

    if( NULL == session_obj )
    {
      MSG_1( MSG_SSID_DFLT, MSG_LEGACY_ERROR,
              "VS: vs_stream_resync_done_event_handler(): session not found, VSID received is (0x%08X)", packet->token );
      break;
    }
    payload = GPR_PKT_GET_PAYLOAD( vcpm_event_resync_done_t, packet );
    event_param.resync_type = payload->token;
    event_param.status = payload->status;
    MSG_2( MSG_SSID_DFLT, MSG_LEGACY_HIGH,
        "VS: vs_stream_resync_done_event_handler(): resync_type(0x%08X) status(0x%08X)", event_param.resync_type, event_param.status );
    vs_common_send_event( session_obj->header.handle, VS_VOC_EVENT_RESYNC_DONE, &event_param, sizeof(vs_voc_event_resync_done_t));
    break;

  }

  rc =  __gpr_cmd_free( packet );
  VS_REPORT_FATAL_ON_ERROR( rc );

  return VS_EOK;
}

/*************************************************************
 * GENERIC RESPONSE handlers
 *************************************************************/

void vs_response_fn_trampoline (
  uint32_t fn_index,
  gpr_packet_t* packet
)
{
  uint32_t rc = VS_EOK;
  vs_object_t* object = NULL;

  rc = vs_get_object( packet->token, &object );
 
  if ( NULL != object )
  {
    switch ( object->header.type )
    {
      case VS_OBJECT_TYPE_ENUM_HANDLE:
        object->handle.fn_table[ fn_index ]( packet );
        return;

      case VS_OBJECT_TYPE_ENUM_SIMPLE_JOB:
        object->simple_job.fn_table[ fn_index ]( packet );
        return;

      default:
        break;
    }
  }

  rc =  __gpr_cmd_free( packet );
  VS_REPORT_FATAL_ON_ERROR( rc );
}

/****************************************************************************
 * VS CUSTOM RESPONSE HANDLERS
 ****************************************************************************/

/**
 * This response function is associated to VS_OBJECT_TYPE_ENUM_SIMPLE_JOB
 * created for facilitating the vocoder state transition of a VS SESSION.
 *
 * Some VS commands needs CVD response to procees further with the
 * vocoder state machine, hence the ADSP/CVD timeout management and recovery
 * handling has to be done.
 */
void vs_voc_transition_result_rsp_fn (
  gpr_packet_t* packet
)
{
  uint32_t rc = VS_EOK;
  vs_simple_job_object_t* simple_obj = NULL;
  vs_session_object_t* session_obj = NULL;
  gpr_ibasic_rsp_result_t* rsp_result = NULL;

  for ( ;; )
  {
    rc = vs_get_typed_object( packet->token, VS_OBJECT_TYPE_ENUM_SIMPLE_JOB,
                              ( ( vs_object_t** ) &simple_obj ) );
    if ( NULL == simple_obj )
    {
      VS_REPORT_FATAL_ON_ERROR( rc );
      break;
    }

    rsp_result = GPR_PKT_GET_PAYLOAD( gpr_ibasic_rsp_result_t, packet );
    simple_obj->status = rsp_result->status;
    simple_obj->is_completed = TRUE;
    rc = vs_get_typed_object( simple_obj->context_handle, VS_OBJECT_TYPE_ENUM_SESSION,
                              ( ( vs_object_t** ) &session_obj ) );
    if ( NULL == session_obj )
    {
      VS_REPORT_FATAL_ON_ERROR( rc );
      break;
    }

    session_obj->cmd_ctrl.rsp_cnt++;
    session_obj->session_ctrl.status |= simple_obj->status;
    if ( VS_EOK != simple_obj->status )
    {
      MSG_2( MSG_SSID_DFLT, MSG_LEGACY_ERROR,
             "vs_voc_transition_result_rsp_fn(): command (0x%08x) failed "
             "with rc=(0x%08x)", rsp_result->opcode, rsp_result->status  );
    }
    else
    {
      MSG_2( MSG_SSID_DFLT, MSG_LEGACY_MED,
             "vs_voc_transition_result_rsp_fn(): command (0x%08x) rc=(0x%08x)",
             rsp_result->opcode, rsp_result->status  );
    }

    /* free the simple job object. */
    ( void ) vs_mem_free_object( (vs_object_t*)simple_obj );

    break;
  }  

  /* Free the apr cmd response packet. */
  rc = __gpr_cmd_free( packet );
  VS_REPORT_FATAL_ON_ERROR( rc );

  if( NULL != session_obj )
  {
    if( session_obj->cmd_ctrl.cmd_cnt == session_obj->cmd_ctrl.rsp_cnt )
    {
      session_obj->cmd_ctrl.cmd_cnt =0;
      session_obj->cmd_ctrl.rsp_cnt = 0;
      vs_signal_run( );
    }
  }
  
  return;
}

void vs_voc_start_voice_result_rsp_fn (
  gpr_packet_t* packet
)
{
  uint32_t rc = VS_EOK;
  vs_simple_job_object_t* simple_obj = NULL;
  vs_session_object_t* session_obj = NULL;
  gpr_ibasic_rsp_result_t* rsp_result = NULL;

  for ( ;; )
  {
    rc = vs_get_typed_object( packet->token, VS_OBJECT_TYPE_ENUM_SIMPLE_JOB,
                              ( ( vs_object_t** ) &simple_obj ) );
    if ( NULL == simple_obj )
    {
      VS_REPORT_FATAL_ON_ERROR( rc );
      break;
    }

    rsp_result = GPR_PKT_GET_PAYLOAD( gpr_ibasic_rsp_result_t, packet );
    simple_obj->status = rsp_result->status;
    simple_obj->is_completed = TRUE;
    rc = vs_get_typed_object( simple_obj->context_handle, VS_OBJECT_TYPE_ENUM_SESSION,
                              ( ( vs_object_t** ) &session_obj ) );
    if ( NULL == session_obj )
    {
      VS_REPORT_FATAL_ON_ERROR( rc );
      break;
    }

    session_obj->cmd_ctrl.rsp_cnt++;
    session_obj->session_ctrl.status |= simple_obj->status;
    if ( VS_EOK != simple_obj->status )
    {
      MSG_2( MSG_SSID_DFLT, MSG_LEGACY_ERROR,
             "vs_voc_transition_result_rsp_fn(): command (0x%08x) failed "
             "with rc=(0x%08x)", rsp_result->opcode, rsp_result->status  );
    }
    else
    {
      MSG_2( MSG_SSID_DFLT, MSG_LEGACY_MED,
             "vs_voc_transition_result_rsp_fn(): command (0x%08x) rc=(0x%08x)",
             rsp_result->opcode, rsp_result->status  );

      /* since start voice is successful, setting TRUE that all subgraphs have started. */
      for ( int index = 0; index< session_obj->num_sub_graphs ; index++)
      {
        session_obj->subgraph_info[index].is_subgraph_started = TRUE;
      }	  
      session_obj->is_stream_ready = TRUE;
      vs_common_send_event( session_obj->header.handle, VS_VOC_EVENT_READY, NULL, 0 );
    }

    /* free the simple job object. */
    ( void ) vs_mem_free_object( (vs_object_t*)simple_obj );

    break;
  }  

  /* Free the apr cmd response packet. */
  rc = __gpr_cmd_free( packet );
  VS_REPORT_FATAL_ON_ERROR( rc );

  if( NULL != session_obj )
  {
    if( session_obj->cmd_ctrl.cmd_cnt == session_obj->cmd_ctrl.rsp_cnt )
    {
       session_obj->cmd_ctrl.cmd_cnt =0;
       session_obj->cmd_ctrl.rsp_cnt = 0;
       vs_signal_run( );
    }
  }
  
  return;
}

void vs_voc_stop_voice_result_rsp_fn (
  gpr_packet_t* packet
)
{
  uint32_t rc = VS_EOK;
  vs_simple_job_object_t* simple_obj = NULL;
  vs_session_object_t* session_obj = NULL;
  gpr_ibasic_rsp_result_t* rsp_result = NULL;

  for ( ;; )
  {
    rc = vs_get_typed_object( packet->token, VS_OBJECT_TYPE_ENUM_SIMPLE_JOB,
                              ( ( vs_object_t** ) &simple_obj ) );
    if ( NULL == simple_obj )
    {
      VS_REPORT_FATAL_ON_ERROR( rc );
      break;
    }

    rsp_result = GPR_PKT_GET_PAYLOAD( gpr_ibasic_rsp_result_t, packet );
    simple_obj->status = rsp_result->status;
    simple_obj->is_completed = TRUE;
    rc = vs_get_typed_object( simple_obj->context_handle, VS_OBJECT_TYPE_ENUM_SESSION,
                              ( ( vs_object_t** ) &session_obj ) );
    if ( NULL == session_obj )
    {
      VS_REPORT_FATAL_ON_ERROR( rc );
      break;
    }

    session_obj->cmd_ctrl.rsp_cnt++;
    session_obj->session_ctrl.status |= simple_obj->status;
    if ( VS_EOK != simple_obj->status )
    {
      MSG_2( MSG_SSID_DFLT, MSG_LEGACY_ERROR,
             "vs_voc_transition_result_rsp_fn(): command (0x%08x) failed "
             "with rc=(0x%08x)", rsp_result->opcode, rsp_result->status  );
    }
    else
    {
      MSG_2( MSG_SSID_DFLT, MSG_LEGACY_MED,
             "vs_voc_transition_result_rsp_fn(): command (0x%08x) rc=(0x%08x)",
             rsp_result->opcode, rsp_result->status  );

      /* since start voice is successful, setting FALSE that all subgraphs have stopped. */
      for ( int index = 0; index< session_obj->num_sub_graphs ; index++)
      {
        session_obj->subgraph_info[index].is_subgraph_started = FALSE;
      }
	  session_obj->is_stream_ready = FALSE;
      vs_common_send_event( session_obj->header.handle, VS_VOC_EVENT_NOT_READY, NULL, 0 );
    }

    /* free the simple job object. */
    ( void ) vs_mem_free_object( (vs_object_t*)simple_obj );

    break;
  }  

  /* Free the apr cmd response packet. */
  rc = __gpr_cmd_free( packet );
  VS_REPORT_FATAL_ON_ERROR( rc );

  if( NULL != session_obj )
  {
    if( session_obj->cmd_ctrl.cmd_cnt == session_obj->cmd_ctrl.rsp_cnt )
    {
      session_obj->cmd_ctrl.cmd_cnt =0;
      session_obj->cmd_ctrl.rsp_cnt = 0;
      vs_signal_run( );
    }
  }
  
  return;
}

/**
 * This response function is associated to VS_OBJECT_TYPE_ENUM_SIMPLE_JOB
 * created for facilitating the tracking responses for non-gating CVD APIs.
 */
void vs_voc_track_result_rsp_fn (
  gpr_packet_t* packet
)
{
  uint32_t rc = VS_EOK;
  vs_simple_job_object_t* simple_obj = NULL;
  vs_session_object_t* session_obj = NULL;
  gpr_ibasic_rsp_result_t* rsp_result = NULL;
  vs_common_event_cmd_response_t cmdResp;

  for ( ;; )
  {
    rc = vs_get_typed_object( packet->token, VS_OBJECT_TYPE_ENUM_SIMPLE_JOB,
                              ( ( vs_object_t** ) &simple_obj ) );
    if ( NULL == simple_obj )
    {
      VS_REPORT_FATAL_ON_ERROR( rc );
      break;
    }

    rsp_result = GPR_PKT_GET_PAYLOAD( gpr_ibasic_rsp_result_t, packet );
    simple_obj->is_completed = TRUE;
    rc = vs_get_typed_object( simple_obj->context_handle, VS_OBJECT_TYPE_ENUM_SESSION,
                              ( ( vs_object_t** ) &session_obj ) );
    if ( NULL == session_obj )
    {
      VS_REPORT_FATAL_ON_ERROR( rc );
      break;
    }

    if ( VS_EOK != simple_obj->status )
    {
      MSG_2( MSG_SSID_DFLT, MSG_LEGACY_ERROR,
             "VS: track result result failed for command[0x%08x] with rc[0x%08x",
             rsp_result->opcode, rsp_result->status  );
    }

    if( VSS_ITTYOOB_CMD_SEND_RX_CHAR == rsp_result->opcode )
    {
      cmdResp.cmd_id = VS_OOBTTY_CMD_PUSH_CHAR;
      cmdResp.status = simple_obj->status;
      cmdResp.token = NULL;
      ( void ) vs_common_send_event( session_obj->header.handle,
                                     VS_COMMON_EVENT_CMD_RESPONSE, &cmdResp, sizeof(cmdResp) );
    }

    /* free the simple job object. */
    ( void ) vs_mem_free_object( (vs_object_t*)simple_obj );

    break;
  }  

  /* Free the apr cmd response packet. */
  rc = __gpr_cmd_free( packet );
  VS_REPORT_FATAL_ON_ERROR( rc );
  vs_signal_run();
  return;
}

#if 1
void vs_voc_mailbox_memory_config_rsp_fn (
  gpr_packet_t* packet
)
{
  uint32_t rc = VS_EOK;
  vs_simple_job_object_t* simple_obj = NULL;
  vs_session_object_t* session_obj = NULL;
  vcpm_param_id_mailbox_memory_config_t* mbox_cfg = NULL;
  apm_cmd_rsp_get_cfg_t* get_cfg_rsp = NULL;
  voicemem_cmd_set_mailbox_memory_config_t mem_cfg;
  uint64_t temp_addr1 = 0, temp_addr2 = 0;
  for ( ;; )
  {
    rc = vs_get_typed_object( packet->token, VS_OBJECT_TYPE_ENUM_SIMPLE_JOB,
                              (vs_object_t**)&simple_obj );
    if ( NULL == simple_obj )
    {
      VS_REPORT_FATAL_ON_ERROR( rc );
      break;
    }
    get_cfg_rsp = GPR_PKT_GET_PAYLOAD( apm_cmd_rsp_get_cfg_t, packet );
    if (get_cfg_rsp->status == VS_EOK)
    {
       apm_module_param_data_t *param_data_ptr =
          (apm_module_param_data_t *)((uint8_t *)get_cfg_rsp + sizeof(apm_cmd_rsp_get_cfg_t));
       if (VCPM_PARAM_ID_MAILBOX_MEMORY_CONFIG == param_data_ptr->param_id)
       {
          mbox_cfg = (vcpm_param_id_mailbox_memory_config_t *)((uint8_t *)get_cfg_rsp + sizeof(apm_cmd_rsp_get_cfg_t) +
                                                      sizeof(apm_module_param_data_t));
       }
       else
       {
         ( void ) vs_mem_free_object( (vs_object_t*)simple_obj );
          break;
       }
    }

    simple_obj->is_completed = TRUE;
    if( VS_EOK != get_cfg_rsp->status )
    {
       VS_REPORT_FATAL_ON_ERROR( get_cfg_rsp->status );
       ( void ) vs_mem_free_object( (vs_object_t*)simple_obj );
       break;
    }

    rc = vs_get_typed_object( simple_obj->context_handle,
                              VS_OBJECT_TYPE_ENUM_SESSION,
                              (vs_object_t**)&session_obj );
    if ( NULL == session_obj )
    {
      VS_REPORT_FATAL_ON_ERROR( rc );
      break;
    }
    session_obj->session_ctrl.status = VS_EOK;
    session_obj->cmd_ctrl.rsp_cnt++;
    
    MSG_5( MSG_SSID_DFLT, MSG_LEGACY_MED,
             "VS: MBOX_CFG_RSP: mem_address_adsp[MSW] = 0x%08x, mem_address_adsp[LSW] = 0x%08x, "
             "mem_address_pcie[MSW] = 0x%08x, mem_address_pcie[LSW] = 0x%08x,  size bytes[%d]",
             (uint32_t)(mbox_cfg->mailbox_mem_address_adsp_msw), (uint32_t)(mbox_cfg->mailbox_mem_address_adsp_lsw), 
             (uint32_t)(mbox_cfg->mailbox_mem_address_pcie_msw), (uint32_t)(mbox_cfg->mailbox_mem_address_pcie_lsw), mbox_cfg->mem_size );
    
#if defined (VS_PROFILING)
    vs_cmd_ticks_in_us = (uint64_t)((qurt_sysclock_get_hw_ticks()) *10ull/192ull);
    MSG_1( MSG_SSID_DFLT, MSG_LEGACY_HIGH,
           "VS_PROFILING:VSS_IPKTEXG_CMD_REQ_MAILBOX_MEMORY_CONFIG-RSP(): vs_cmd_ticks_in_us=(%ld)",
           vs_cmd_ticks_in_us );
#endif /* VS_PROFILING */

    temp_addr1 = (uint64_t)  ( mbox_cfg->mailbox_mem_address_adsp_msw);
    temp_addr2 = (uint64_t)  ( mbox_cfg->mailbox_mem_address_adsp_lsw);
    mem_cfg.mailbox_mem_address_adsp = ( ( temp_addr1 << 32 ) | temp_addr2 );

    temp_addr1 = (uint64_t)  ( mbox_cfg->mailbox_mem_address_pcie_msw );
    temp_addr2 = (uint64_t)  ( mbox_cfg->mailbox_mem_address_pcie_lsw );
    mem_cfg.mailbox_mem_address_pcie = ( ( temp_addr1 << 32 ) | temp_addr2 );
    mem_cfg.mem_size = mbox_cfg->mem_size;
    rc = voicemem_call( VOICEMEM_CMD_SET_MAILBOX_MEMORY_CONFIG, &mem_cfg, 
                       sizeof(voicemem_cmd_set_mailbox_memory_config_t));
                       
    if ( rc == VS_EOK ) 
    {
      vs_adsp_mbox_memory_config_is_received = TRUE;
      session_obj->session_ctrl.status = VS_EOK;
    }
    else
    {    
      session_obj->session_ctrl.status = VS_EFAILED;
    }

    /* Free the simple job object. */
    ( void ) vs_mem_free_object( (vs_object_t*)simple_obj );
    break;
  }
  /* Free the apr cmd response packet. */
  rc = __gpr_cmd_free( packet );
  VS_REPORT_FATAL_ON_ERROR( rc );

  if( NULL != session_obj )
  {
    if( session_obj->cmd_ctrl.cmd_cnt == session_obj->cmd_ctrl.rsp_cnt )
    {
       session_obj->cmd_ctrl.cmd_cnt =0;
       session_obj->cmd_ctrl.rsp_cnt = 0;
       vs_signal_run( );
    }
  }

  return;
}
#endif

void vs_voc_memory_map_result_rsp_fn (
  gpr_packet_t* packet
)
{
  uint32_t rc = VS_EOK;
  vs_simple_job_object_t* simple_obj = NULL;
  vs_session_object_t* session_obj = NULL;
  apm_cmd_rsp_shared_mem_map_regions_t* map_rsp = NULL;

  for ( ;; )
  {
    rc = vs_get_typed_object( packet->token, VS_OBJECT_TYPE_ENUM_SIMPLE_JOB,
                              (vs_object_t**)&simple_obj );
    if ( NULL == simple_obj )
    {
      VS_REPORT_FATAL_ON_ERROR( rc );
      break;
    }

    map_rsp = GPR_PKT_GET_PAYLOAD( apm_cmd_rsp_shared_mem_map_regions_t, packet );
    simple_obj->is_completed = TRUE;
    rc = vs_get_typed_object( simple_obj->context_handle,
                              VS_OBJECT_TYPE_ENUM_SESSION,
                              (vs_object_t**)&session_obj );
    if ( NULL == session_obj )
    {
      VS_REPORT_FATAL_ON_ERROR( rc );
      break;
    }

    session_obj->cmd_ctrl.rsp_cnt++;
    session_obj->session_ctrl.status |= VS_EOK;
    session_obj->memmap_handle = map_rsp->mem_map_handle;
    MSG_1( MSG_SSID_DFLT, MSG_LEGACY_MED,
             "VS: MEM_MAP_RSP: mem_handle[0x%08x]", map_rsp->mem_map_handle );

    break;
  }

  /* Free the simple job object. */
  ( void ) vs_mem_free_object( (vs_object_t*)simple_obj );

  /* Free the apr cmd response packet. */
  rc = __gpr_cmd_free( packet );
  VS_REPORT_FATAL_ON_ERROR( rc );

  if( NULL != session_obj )
  {
    if( session_obj->cmd_ctrl.cmd_cnt == session_obj->cmd_ctrl.rsp_cnt )
    {
       session_obj->cmd_ctrl.cmd_cnt =0;
       session_obj->cmd_ctrl.rsp_cnt = 0;
       vs_signal_run( );
    }
  }

  return;
}

static bool_t vs_voc_is_sgid_in_session (
  vs_session_object_t* session_obj,
  uint32_t sg_id
)
{
  bool_t is_sgid_present = FALSE;
  uint32_t index = 0;

  for( index = 0; index< VS_VCPM_MAX_NUM_SUBGRAPHS; index++)
  {
    if(session_obj->subgraph_info[index].sub_graph_id == sg_id)
    {
      MSG_2( MSG_SSID_DFLT, MSG_LEGACY_LOW, 
                  "vs_voc_is_sgid_in_session(): Subgraph ID already exists in session, session SG ID %d and query SG ID %d", session_obj->subgraph_info[index].sub_graph_id, sg_id );
     is_sgid_present = TRUE;
     break;
    }
  }

  return is_sgid_present;
}

uint32_t vs_voc_vcpm_graph_open_event_handler (
  vs_gating_control_t* ctrl
)
{
  uint32_t rc = VS_EOK; 
  vs_session_object_t* session_obj = NULL;
  vcpm_event_graph_open_payload_t* graph_open_payload = NULL;
  vcpm_vs_tag_miid_info_t* mod_iid_tag = NULL;
  vs_sequencer_job_object_t* seq_obj = NULL;
  uint32_t index = 0;
  uint32_t num_sgs = 0;
  uint8_t* temp_ptr = NULL;

  if ( NULL == ctrl->packet  )
  {
    VS_REPORT_FATAL_ON_ERROR( VS_EUNEXPECTED );
    return VS_EFAILED;
  }

  graph_open_payload = GPR_PKT_GET_PAYLOAD( vcpm_event_graph_open_payload_t, ctrl->packet );
  if ( NULL == graph_open_payload )
  {
    VS_REPORT_FATAL_ON_ERROR( VS_EUNEXPECTED );
    return VS_EFAILED;
  }

  /* Get the session object corresponding to the VSID received */
  for(index = 0; index < VS_MAX_NUM_SESSIONS_V; index++)
  {
    session_obj = vs_session_obj_list[index];
    if ( graph_open_payload->vsid == session_obj->vsid )
        break;
    else
       session_obj = NULL;
  }

  if( NULL == session_obj )
  {
     MSG_1( MSG_SSID_DFLT, MSG_LEGACY_ERROR,
             "VS: vs_voc_vcpm_graph_open_event_handler(): session object is NULL, VSID received is (0x%08X)", graph_open_payload->vsid );
    return VS_EFAILED;
  }

  session_obj->is_vcpm_opened = TRUE;
  temp_ptr = (uint8_t*)graph_open_payload;
  temp_ptr += sizeof(vcpm_event_graph_open_payload_t);

  //session_obj->subgraph_info = vs_memmgr_malloc( &vs_heapmgr, (session_obj->num_sub_graphs)*sizeof(vs_vcpm_subgraph_info_t) );
  for(index = 0; index < graph_open_payload->num_tag_info; index++)
   {
     mod_iid_tag = (vcpm_vs_tag_miid_info_t*)temp_ptr;

       switch(mod_iid_tag->tag_id)
       {
         case VOICE_MOD_TAG_ID_ENCODER:
         {
           if(FALSE == vs_voc_is_sgid_in_session(session_obj, mod_iid_tag->sg_id))
           {
             session_obj->subgraph_info[num_sgs].is_stream_subgraph = TRUE;
             session_obj->subgraph_info[num_sgs].sub_graph_id = mod_iid_tag->sg_id;
             num_sgs++;
           }
           session_obj->mod_iid_tag_info[VS_MOD_ENCODER].mod_iid = mod_iid_tag->module_iid;
         }
         break;

         case VOICE_MOD_TAG_ID_DECODER:
         {
            if(FALSE == vs_voc_is_sgid_in_session(session_obj, mod_iid_tag->sg_id))
            {
               session_obj->subgraph_info[num_sgs].is_stream_subgraph = TRUE;
               session_obj->subgraph_info[num_sgs].sub_graph_id = mod_iid_tag->sg_id;
               num_sgs++;
             }		 
           session_obj->mod_iid_tag_info[VS_MOD_DECODER].mod_iid = mod_iid_tag->module_iid;
         }
           break;

         case VOICE_MOD_TAG_ID_TX_MAILBOX:
         {
           if(FALSE == vs_voc_is_sgid_in_session(session_obj, mod_iid_tag->sg_id))
           {
               session_obj->subgraph_info[num_sgs].is_stream_subgraph = FALSE;
               session_obj->subgraph_info[num_sgs].sub_graph_id = mod_iid_tag->sg_id;
               num_sgs++;
           }
           session_obj->mod_iid_tag_info[VS_MOD_TX_EP].mod_iid = mod_iid_tag->module_iid;
         }
           break;

        case VOICE_MOD_TAG_ID_RX_MAILBOX:
        {
          if(FALSE == vs_voc_is_sgid_in_session(session_obj, mod_iid_tag->sg_id))
          {
             session_obj->subgraph_info[num_sgs].is_stream_subgraph = FALSE;
             session_obj->subgraph_info[num_sgs].sub_graph_id = mod_iid_tag->sg_id;
             num_sgs++;
          }
          session_obj->mod_iid_tag_info[VS_MOD_RX_EP].mod_iid = mod_iid_tag->module_iid;
        }
          break;

         case VOICE_MOD_TAG_ID_CTMTTY_TX:
         {
           if(FALSE == vs_voc_is_sgid_in_session(session_obj, mod_iid_tag->sg_id))
           {
               session_obj->subgraph_info[num_sgs].is_stream_subgraph = FALSE;
               session_obj->subgraph_info[num_sgs].sub_graph_id = mod_iid_tag->sg_id;
               num_sgs++;
           }
 
           session_obj->mod_iid_tag_info[VS_MOD_TX_CTMTTY].mod_iid = mod_iid_tag->module_iid;
         }
           break;

         case VOICE_MOD_TAG_ID_CTMTTY_RX:
         {
            if(FALSE == vs_voc_is_sgid_in_session(session_obj, mod_iid_tag->sg_id))
            {
              session_obj->subgraph_info[num_sgs].is_stream_subgraph = FALSE;
              session_obj->subgraph_info[num_sgs].sub_graph_id = mod_iid_tag->sg_id;
              num_sgs++;
            }
          session_obj->mod_iid_tag_info[VS_MOD_RX_CTMTTY].mod_iid = mod_iid_tag->module_iid;
         }
           break;

         case VOICE_MOD_TAG_ID_LTETTY_TX:
         {
            if(FALSE == vs_voc_is_sgid_in_session(session_obj, mod_iid_tag->sg_id))
            {
              session_obj->subgraph_info[num_sgs].is_stream_subgraph = FALSE;
              session_obj->subgraph_info[num_sgs].sub_graph_id = mod_iid_tag->sg_id;
              num_sgs++;
            }
 
           session_obj->mod_iid_tag_info[VS_MOD_TX_LTETTY].mod_iid = mod_iid_tag->module_iid;
          }
           break;

         case VOICE_MOD_TAG_ID_LTETTY_RX:
         {
            if(FALSE == vs_voc_is_sgid_in_session(session_obj, mod_iid_tag->sg_id))
            {
              session_obj->subgraph_info[num_sgs].is_stream_subgraph = FALSE;
              session_obj->subgraph_info[num_sgs].sub_graph_id = mod_iid_tag->sg_id;
              num_sgs++;
            }
          session_obj->mod_iid_tag_info[VS_MOD_RX_LTETTY].mod_iid = mod_iid_tag->module_iid;
         }
          break;

         case VOICE_MOD_TAG_ID_TX_STREAM_HPCM:
         {
            if(FALSE == vs_voc_is_sgid_in_session(session_obj, mod_iid_tag->sg_id))
            {
               session_obj->subgraph_info[num_sgs].is_stream_subgraph = FALSE;
               session_obj->subgraph_info[num_sgs].sub_graph_id = mod_iid_tag->sg_id;
               num_sgs++;
            }
 
           session_obj->mod_iid_tag_info[VS_MOD_TX_HPCM].mod_iid = mod_iid_tag->module_iid;
         }

           break;

         case VOICE_MOD_TAG_ID_RX_STREAM_HPCM:
         {
           if(FALSE == vs_voc_is_sgid_in_session(session_obj, mod_iid_tag->sg_id))
           {
              session_obj->subgraph_info[num_sgs].is_stream_subgraph = FALSE;
              session_obj->subgraph_info[num_sgs].sub_graph_id = mod_iid_tag->sg_id;
              num_sgs++;
           }
          session_obj->mod_iid_tag_info[VS_MOD_RX_HPCM].mod_iid = mod_iid_tag->module_iid;
         }
          break;

         default:
           MSG_1( MSG_SSID_DFLT, MSG_LEGACY_MED, 
                  "vs_voc_vcpm_graph_open_event_handler(): Received unsupported Tag ID (0x%08X)", mod_iid_tag->tag_id );
           break;
       }
	   temp_ptr += sizeof(vcpm_vs_tag_miid_info_t);
   }

   if( num_sgs != 0 )
   {
     session_obj->num_sub_graphs += num_sgs;
   } 

   if ( ctrl->state == VS_GATING_CMD_STATE_ENUM_EXECUTE )
   {
      if ( FALSE == session_obj->is_vs_enabled )
      {
        MSG( MSG_SSID_DFLT, MSG_LEGACY_HIGH,
               "VS: vs_voc_vcpm_graph_open_event_handler(): VS is not enabled, skipping state machine trigger" );
        return VS_EOK;
      }

      /* Create the sequencer job object to track CVD setup. */
      rc = vs_create_sequencer_job_object(
             ( vs_sequencer_job_object_t** ) &ctrl->rootjob_obj );
      VS_PANIC_ON_ERROR( rc );

      seq_obj = &ctrl->rootjob_obj->sequencer_job;
      seq_obj->state = VS_HELPER_SEQUENCER_ENUM_GOAL_CREATE;

      if ( VS_VOC_STATE_ENUM_IDLE == session_obj->session_ctrl.state )
      {
        seq_obj->state = VS_HELPER_SEQUENCER_ENUM_GOAL_CONFIGURE;
      }
    }

    VS_ACQUIRE_LOCK( session_obj->session_lock );

    seq_obj = &ctrl->rootjob_obj->sequencer_job;
    rc = vs_voc_helper_vocoder_setup_ctrl( seq_obj, session_obj );

    VS_RELEASE_LOCK( session_obj->session_lock );

    if( VS_EPENDING != rc )
    {
       ( void ) vs_mem_free_object( ( vs_object_t* ) seq_obj );
       rc = VS_EOK;
    }

  return rc;
}

uint32_t vs_voc_vcpm_graph_close_event_handler (
  gpr_packet_t* packet
)
{
  uint32_t rc = VS_EOK; 
  vs_session_object_t* session_obj = NULL;
  vcpm_event_graph_close_payload_t* graph_close_payload = NULL;
  vs_sequencer_job_object_t* seq_obj = NULL;
  uint32_t index = 0;
  uint32_t num_tags = 0;
  uint8_t* temp_ptr = NULL;

  if ( NULL == packet  )
  {
    VS_REPORT_FATAL_ON_ERROR( VS_EUNEXPECTED );
    rc =  __gpr_cmd_free( packet );
    VS_REPORT_FATAL_ON_ERROR( rc );
    return VS_EFAILED;
  }

  graph_close_payload = GPR_PKT_GET_PAYLOAD( vcpm_event_graph_close_payload_t, packet );
  if ( NULL == graph_close_payload )
  {
    VS_REPORT_FATAL_ON_ERROR( VS_EUNEXPECTED );
    rc =  __gpr_cmd_free( packet );
    VS_REPORT_FATAL_ON_ERROR( rc );
    return VS_EFAILED;
  }

  /* Get the session object corresponding to the VSID received */
  for(index = 0; index < VS_MAX_NUM_SESSIONS_V; index++)
  {
    session_obj = vs_session_obj_list[index];
    if( NULL == session_obj )
    {
       continue;
    }
    if ( graph_close_payload->vsid == session_obj->vsid )
    {
        break;
    }
    else
    {
       session_obj = NULL;
    }
  }

  if( NULL == session_obj )
  {
     MSG_1( MSG_SSID_DFLT, MSG_LEGACY_ERROR,
             "VS: vs_voc_vcpm_graph_close_event_handler(): session not found, VSID received is (0x%08X)", graph_close_payload->vsid );
     rc =  __gpr_cmd_free( packet );
     VS_REPORT_FATAL_ON_ERROR( rc );
     return VS_EFAILED;
  }
  session_obj->is_vcpm_opened = FALSE;
  session_obj->num_sub_graphs = 0;

  //clearing subgraph info and tag ID info
  for ( index = 0; index< VS_VCPM_MAX_NUM_MODULES ; index++)
  {
     session_obj->mod_iid_tag_info[index].mod_iid = VS_VOICE_MOD_DEFAULT_IID;
  }

  for(index = 0; index < VS_VCPM_MAX_NUM_SUBGRAPHS ; index++)
  {
     session_obj->subgraph_info[index].is_stream_subgraph = FALSE;
     session_obj->subgraph_info[index].is_subgraph_started = FALSE;
     session_obj->subgraph_info[index].sub_graph_id = 0xFFFFFFFF;
  }
  if ( TRUE == session_obj->is_vfr_allocated )
  {
     ( void )vs_voc_action_release_vfr( session_obj);
  }
  session_obj->session_ctrl.state = VS_VOC_STATE_ENUM_RESET;
  session_obj->session_ctrl.action = VS_VOC_ACTION_ENUM_NONE;
  session_obj->session_ctrl.goal = VS_VOC_GOAL_ENUM_NONE;
  if ( TRUE == session_obj->is_vs_enabled )
   {
      vs_common_send_event( session_obj->header.handle, VS_VOC_EVENT_NOT_READY, NULL, 0 ) ;
      rc = vs_timer_stop( vs_adsp_timeout_timer );
      MSG( MSG_SSID_DFLT, MSG_LEGACY_HIGH,
             "VS: vs_voc_vcpm_graph_close_event_handler(): VS is enabled, stopping gating command timer, and sending not ready to client ");
      
   }
   if ( session_obj->memmap_handle!= NULL )
   {
       (void)vs_voc_action_unmap_memory(session_obj);
        MSG( MSG_SSID_DFLT, MSG_LEGACY_LOW,"VS: vs_voc_vcpm_graph_close_event_handler(): sent unmap memory ");
   }

  rc =  __gpr_cmd_free( packet );
  VS_REPORT_FATAL_ON_ERROR( rc );
  return rc;
}

