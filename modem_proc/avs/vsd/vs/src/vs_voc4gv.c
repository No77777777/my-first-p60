/**
  @file vs_vocevrcb.c
  @brief This file contains the definitions of the interfaces for EVRC-B vocoder.

*/

/*
  ============================================================================
  Copyright (C) 2014-2016, 2019 QUALCOMM Technologies, Inc.
  All Rights Reserved.
  Confidential and Proprietary - Qualcomm Technologies, Inc.
  ============================================================================
  $Header: //components/rel/avs.mpss/9.0.c51/vsd/vs/src/vs_voc4gv.c#1 $
  $Author: pwbldsvc $
 ============================================================================
*/

/****************************************************************************
 * INCLUDE HEADER FILES                                                     *
 ****************************************************************************/

#include "vs_i.h"

/*GPR APIs*/
#include "gpr_api.h"

/*APM APIs*/
#include "apm_api.h"
#include "vocoder_cmn_api.h"


/****************************************************************************
 * FUNCTIONS                                                                *
 ****************************************************************************/

static bool_t vs_voc4gv_is_frame_rate_valid (
  vs_session_object_t* session_obj,
  vs_voc4gv_frame_rate_t rate
)
{
  bool_t rc = FALSE;
  
  switch ( rate )
  {
    case VS_VOC4GV_FRAME_BLANK_RATE:
    case VS_VOC4GV_FRAME_EIGHTH_RATE:
    case VS_VOC4GV_FRAME_QUARTER_RATE:
    case VS_VOC4GV_FRAME_HALF_RATE:
    case VS_VOC4GV_FRAME_FULL_RATE:
      rc = TRUE;
      break;

    case VS_VOC4GV_FRAME_ERASURE_RATE:
    {
      if( session_obj->vocoder.media_id == MEDIA_FMT_ID_4GV_NB_MODEM )
         rc = TRUE;
    }
    break;

    case VS_VOC4GV_FRAME_EIGHTH_RATE_NONCRITICAL:
    {
      if( session_obj->vocoder.media_id == MEDIA_FMT_ID_4GV_NW_MODEM || 
          session_obj->vocoder.media_id == MEDIA_FMT_ID_EVRC_NW_2K )
         rc = TRUE;
    }
    break;
  }

  return rc;
}

static bool_t vs_voc4gv_is_encoder_rate_valid (
  vs_session_object_t* session_obj,
  vs_voc4gv_avg_enc_rate_t rate
)
{

  if(session_obj->vocoder.media_id != MEDIA_FMT_ID_4GV_WB_MODEM )
  {
    if( rate <= VS_VOC4GV_AVG_RATE_COP_7 && rate >= VS_VOC4GV_AVG_RATE_COP_0 )
        return TRUE;
  }
  else
  {
    if ( ( VS_VOC4GV_AVG_RATE_COP_0 == rate ) ||
         ( VS_VOC4GV_AVG_RATE_COP_4 == rate ) ||
         ( VS_VOC4GV_AVG_RATE_COP_7 == rate ) )
    {
      return TRUE;
    }
  }

  return FALSE;
}

static uint32_t vs_voc4gv_publish_params (
  void* this
)
{
  return VS_EOK;
}


static uint32_t vs_voc4gv_cache_encoder_rate (
  vs_session_object_t* session_obj,
  vs_param_enc_rate_t* enc_rate
)
{
  uint32_t rc = VS_EOK;
  vs_voc4gv_avg_enc_rate_t rate = (vs_voc4gv_avg_enc_rate_t)enc_rate->rate;

  MSG_2( MSG_SSID_DFLT, MSG_LEGACY_HIGH,
        "SET_PARAM: VS_PARAM_ENC_RATE(EVRCB): rate=(%d) (0 - COP 0, ... 7 - COP 7),"
        "is_stream_ready=(%d)", rate, session_obj->is_stream_ready  );

  if (  FALSE == vs_voc4gv_is_encoder_rate_valid(session_obj,rate) ) return VS_EBADPARAM;

  session_obj->vocoder.encoder_rate = enc_rate->rate;

  if( TRUE == session_obj->is_stream_ready )
  {
    ( void ) vs_voc_action_set_enc_rate( session_obj );
    ( void ) vs_voc4gv_publish_params( session_obj );
  }

  return rc;
}


static uint32_t vs_voc4gv_cache_minmax_rate (
  vs_session_object_t* session_obj,
  vs_param_enc_minmax_rate_t* minmax
)
{
  uint32_t rc = VS_EOK;
  uint8_t min_rate = minmax->min_rate;
  uint8_t max_rate = minmax->max_rate;

  MSG_3( MSG_SSID_DFLT, MSG_LEGACY_HIGH,
        "SET_PARAM: VS_PARAM_ENC_MINMAX_FRAME_RATE(EVRCB): min_rate=(%d), max_rate=(%d)"
        " (0-blank, 1-eighth, 2-quarter, 3-half, 4-full, 14-erasure),"
        "is_stream_ready=(%d)", min_rate, max_rate, session_obj->is_stream_ready  );

  if( ( FALSE == vs_voc4gv_is_frame_rate_valid(session_obj,min_rate) ) ||
      ( FALSE == vs_voc4gv_is_frame_rate_valid(session_obj,max_rate) ) )
    return VS_EBADPARAM;

  session_obj->vocoder.min_rate = min_rate;
  session_obj->vocoder.max_rate = max_rate;

  if( TRUE == session_obj->is_stream_ready )
  {
    ( void ) vs_voc_action_set_min_max_rate( session_obj );
    ( void ) vs_voc4gv_publish_params( session_obj );
  }

  return rc;
}


static uint32_t vs_voc4gv_set_cached_params_handler (
  void* this
)
{
  vs_session_object_t* session_obj = (vs_session_object_t*)this;

  ( void ) vs_voc_action_set_dtx_v2( session_obj );
  ( void ) vs_voc_action_set_min_max_rate( session_obj );
  ( void ) vs_voc_action_set_enc_rate( session_obj );

  return VS_EOK;
}


static uint32_t vs_voc4gv_set_param_handler (
 void* this,
 uint32_t param_id,
 void* param_data
)
{
  uint32_t rc = VS_EOK;
  vs_session_object_t* session_obj = (vs_session_object_t*)this;

  switch( param_id )
  {
    case VS_PARAM_ENC_RATE:
     rc = vs_voc4gv_cache_encoder_rate( session_obj, (vs_param_enc_rate_t*) param_data ) ;
     break;

    case VS_PARAM_ENC_MINMAX_FRAME_RATE:
      rc = vs_voc4gv_cache_minmax_rate( session_obj, (vs_param_enc_minmax_rate_t*)param_data );
      break;

    default:
      rc = VS_EUNSUPPORTED;
      break;
  }

  return rc;
}


VS_INTERNAL uint32_t vs_voc4gv_init (
  vs_session_object_t* session_obj
)
{
  uint32_t rc = VS_EOK;

  session_obj->vocoder.enc_module_id = VS_VOC4GV_ENC_MODULE_ID;
  session_obj->vocoder.dec_module_id = VS_VOC4GV_DEC_MODULE_ID;
  session_obj->vocoder.set_param_fn = vs_voc4gv_set_param_handler;
  session_obj->vocoder.set_cached_param_fn = vs_voc4gv_set_cached_params_handler;
  session_obj->vocoder.publish_param_fn = vs_voc4gv_publish_params;

  return rc;
}

