/**
  @file vs_voc.c
  @brief This file contains the definitions of the internal functions of VS.
*/

/**
   Copyright (C) 2018, 2020, 2021 QUALCOMM Technologies, Inc.
   All Rights Reserved.
   Confidential and Proprietary - Qualcomm Technologies, Inc.

   $Header: //components/rel/avs.mpss/9.0.c51/vsd/vs/src/vs_state_control.c#1 $
   $Author: pwbldsvc $
*/

/****************************************************************************
 * HEADER FILES                                                     *
 ****************************************************************************/
#include "vs_i.h"

/****************************************************************************
 * GLOBAL VARIABLES                                                         *
 ****************************************************************************/

static char_t *vs_voc_state_enum_table[VS_VOC_STATE_ENUM_MAX] = 
{
  "UNINITIALIZED",

  "RESET_ENTRY  ",
  "RESET        ",
  "IDLE_ENTRY   ",
  "IDLE         ",
  "RUN_ENTRY    ",
  "RUN          ",

  "INVALID      "
};

static char_t *vs_voc_goal_enum_table[VS_VOC_GOAL_ENUM_MAX] = 
{
  "UNINITIALIZED",
  "NONE         ",

  "CREATE       ",
  "DESTROY      ",
  "CONFIGURE    ",
  "ENABLE       ",
  "DISABLE      ",

  "INVALID"
};

static char_t *vs_voc_action_enum_table[VS_VOC_ACTION_ENUM_MAX] = 
{
  "UNINITIALIZED",
  "NONE",
  "COMPLETE",
  "CONTINUE",

  "START_VOICE",
  "PREPARE_GRAPH",
  "STOP_VOICE",
  "CREATE_MVM",
  "DESTROY_MVM",
  "SET_NETWORK",
  "SET_VOCODER_CLASS_ON_VCPM",
  "SET_VOICE_TIMING",
  "MEMORY_MAP",
  "MEMORY_UNMAP",
  "SET_TIMING_PARAMS",
  "SET_TIMING_REFERENCE",
  "SET_MAX_SAMPLE_RATE",
  "REGISTER_OPERATING_MODE_UPDATE",
  "DEREGISTER_OPERATING_MODE_UPDATE",
  "SET_VOCODER_MODULE_ID",
  "SET_MEDIA_FMT",
  "SET_CACHED_STREAM_PROPERTIES",
  "RESYNC_CTM",
  "SET_MBOX_CONFIG",
  "REGISTER_OOBTTY",
  "DEREGISTER_OOBTTY",
  "RESET_MODULE_ID",
  "ENABLE_HPCM",
  "DISABLE_HPCM",
  "CONFIGURE_VFR",
  "RELEASE_VFR",

  "INVALID"
};

/****************************************************************************
 * VS VOICE SESSION HELPER FUNCTIONS                                        *
 ****************************************************************************/

static int32_t vs_voc_helper_create_new_goal_control (
  vs_voc_goal_enum_t new_goal,
  vs_session_object_t* session_obj,
  vs_object_t** ret_job_obj
)
{
  int32_t rc;
  vs_simple_job_object_t* job_obj;

  if ( ( ret_job_obj == NULL ) || ( session_obj == NULL ) )
  {
    MSG_2( MSG_SSID_DFLT, MSG_LEGACY_ERROR,
           "vs_voc_helper_create_new_goal_control(): NULL params passed "
           "ctrl=(0x%08x) and session_obj=(0x%08x)", ret_job_obj,
           session_obj );
    VS_REPORT_FATAL_ON_ERROR ( VS_EUNEXPECTED );
    return VS_EUNEXPECTED;
  }

  if ( session_obj->session_ctrl.goal != VS_VOC_GOAL_ENUM_NONE )
  {
    /* New Goal cannot be created until previous GOAL is completed. */
    MSG_1( MSG_SSID_DFLT, MSG_LEGACY_ERROR,
           "vs_voc_helper_create_new_goal_control(): Cannot create new Goal "
           "current goal=(0x%08x)", session_obj->session_ctrl.goal );
    VS_PANIC_ON_ERROR( VS_EPENDING );
  }

  session_obj->session_ctrl.goal = new_goal;

  rc = vs_create_simple_job_object( session_obj->header.handle, &job_obj );

    /* The goal is reset by the state machine on goal completion. */
  session_obj->session_ctrl.pendjob_handle = job_obj->header.handle;
    /* The pendjob_handle will signal completion. The pendjob_handle is
     * reset by the state machine on goal completion.
     */
  *ret_job_obj = ( ( vs_object_t* ) job_obj );
    /* sequencer_job.subjob_obj stores the job_obj to be checked for
     * goal completion and to be freed by the current gating command
     * control.
     */

  return VS_EOK;
}

static int32_t vs_voc_do_complete_goal (
  vs_session_object_t* session_obj
)
{
  int32_t rc =  VS_EOK;
  gpr_ibasic_rsp_result_t res;
  gpr_cmd_alloc_send_t goal_params;

  /* Complete the pending command and stay in the same state. */
  res.opcode = VS_UNDEFINED_ID_V;
  /* Lock not needed as response is not expected */
  res.status = session_obj->session_ctrl.status;

  goal_params.src_domain_id = GPR_IDS_DOMAIN_ID_MODEM_V ;
  goal_params.src_port = VS_MODULE_INSTANCE_ID;
  goal_params.dst_domain_id = GPR_IDS_DOMAIN_ID_MODEM_V ;
  goal_params.dst_port = VS_MODULE_INSTANCE_ID; ;
  goal_params.token = session_obj->session_ctrl.pendjob_handle;
  goal_params.opcode = GPR_IBASIC_RSP_RESULT;
  goal_params.payload_size = sizeof( res );
  goal_params.client_data = 0;
  goal_params.payload = &res;
  
  rc = __gpr_cmd_alloc_send(&goal_params);
  if ( rc )
  {
    MSG_1( MSG_SSID_DFLT, MSG_LEGACY_ERROR,
           "vs_voc_do_complete_goal(): Sending response failed with "
           "result=0x%08X", rc );
    /* Inject ERR_FATAL as the VS gets into non recoverable state. */
    VS_PANIC_ON_ERROR( rc );
  }

  session_obj->session_ctrl.transition_job_handle = VS_NULL_V;
  session_obj->session_ctrl.pendjob_handle = VS_NULL_V;
  session_obj->session_ctrl.goal = VS_VOC_GOAL_ENUM_NONE;
  session_obj->session_ctrl.status = VS_EOK;
  session_obj->session_ctrl.action = VS_VOC_ACTION_ENUM_NONE;

  return VS_EOK;
}

static void vs_voc_log_state_info(
  vs_session_object_t* session_obj
)
{
  static vs_voc_state_enum_t prev_state = VS_VOC_STATE_ENUM_UNINITIALIZED;
  static vs_voc_goal_enum_t prev_goal =  VS_VOC_GOAL_ENUM_UNINITIALIZED;
  static vs_voc_action_enum_t prev_action = VS_VOC_ACTION_ENUM_UNINITIALIZED;
  vs_voc_state_enum_t cur_state;
  vs_voc_goal_enum_t cur_goal;
  vs_voc_action_enum_t cur_action;

  if ( session_obj == NULL) return;

  cur_state = session_obj->session_ctrl.state;
  cur_goal = session_obj->session_ctrl.goal;
  cur_action = session_obj->session_ctrl.action;

  /* Show new state information. */
  if ( ( prev_state != cur_state ) ||
       ( prev_goal != cur_goal ) ||
       ( prev_action != cur_action ) )
  {
    MSG_SPRINTF_4( MSG_SSID_DFLT, MSG_LEGACY_MED,
                   "VS:(0x%08X)  STATE: %s  GOAL: %s  ACTION: %s",
                   session_obj->header.handle,
                   vs_voc_state_enum_table[session_obj->session_ctrl.state],
                   vs_voc_goal_enum_table[session_obj->session_ctrl.goal],
                   vs_voc_action_enum_table[session_obj->session_ctrl.action] );
    /* Update information. */
    prev_state = cur_state;
    prev_goal = cur_goal;
    prev_action = cur_action;
  }

  return;
}

/****************************************************************************
 * VS VOCODER STATE MACHINE CONTROL FUNCTIONS                               *
 ****************************************************************************/

static int32_t vs_voc_state_reset_entry (
  vs_session_object_t* session_obj
)
{
  int32_t rc = VS_EOK;

  /**
   * End the goal the current for below Scenarios:
   * - Audio-PD is DOWN
   * - ADSP SSR triggered.
   * - last action failed
   */
  if ( FALSE == vs_is_cvd_up() )
  {
    MSG( MSG_SSID_DFLT, MSG_LEGACY_HIGH, "VS: VOC_STATE_RESET_ENTRY: CVD is not up");
    session_obj->session_ctrl.status = VS_EFAILED;
    rc = vs_voc_do_complete_goal( session_obj );
  }
  else if ( session_obj->cmd_ctrl.cmd_cnt > session_obj->cmd_ctrl.rsp_cnt )
  {
    /* Stay in this state till response for last action is available. */
    return VS_EOK;
  }
  else if ( VS_EOK != session_obj->session_ctrl.status)
  {
    MSG_1( MSG_SSID_DFLT, MSG_LEGACY_ERROR,
           "VS: VOC_STATE_RESET_ENTRY: last action failed with result=(0x%08X)",
           session_obj->session_ctrl.status );
    rc = vs_voc_do_complete_goal( session_obj );
  }

  session_obj->session_ctrl.state = VS_VOC_STATE_ENUM_RESET;
  return VS_EIMMEDIATE;
}


static int32_t vs_voc_state_reset (
  vs_session_object_t* session_obj
)
{
  int32_t rc = VS_EOK;

  switch ( session_obj->session_ctrl.goal )
  {
    case VS_VOC_GOAL_ENUM_NONE:
      break;

    case VS_VOC_GOAL_ENUM_CREATE:
    {
      switch ( session_obj->session_ctrl.action )
      {
        case VS_VOC_ACTION_ENUM_NONE:
        case VS_VOC_ACTION_ENUM_REQUEST_MAILBOX_MEMORY_CONFIG:
        {
          session_obj->session_ctrl.state = VS_VOC_STATE_ENUM_RESET_ENTRY;
          session_obj->session_ctrl.action = VS_VOC_ACTION_ENUM_MEMORY_MAP;
          session_obj->session_ctrl.status = VS_EOK;
          if ( (FALSE == vs_adsp_mbox_memory_config_is_received )
                && ( TRUE == vs_platform_is_fusion ) )
          {
            rc  = vs_voc_action_request_mailbox_memory_config( session_obj );
            return rc;
          }                 
          else
          {
              MSG_2( MSG_SSID_DFLT, MSG_LEGACY_HIGH,
                     "VS: VOC_STATE_RESET: mailbox memory config is not requested from ADSP, "
                     "is_fusion=(%d), is_adsp_mbox_memory_config_received=(%d) ",
                     vs_platform_is_fusion, vs_adsp_mbox_memory_config_is_received );
          }
        }        
        /* Fall through */
        case VS_VOC_ACTION_ENUM_MEMORY_MAP:
        {
          session_obj->session_ctrl.state = VS_VOC_STATE_ENUM_RESET_ENTRY;
          session_obj->session_ctrl.action = VS_VOC_ACTION_ENUM_COMPLETE;

          rc  = vs_voc_action_map_memory(session_obj );
        }
        return rc;

       case VS_VOC_ACTION_ENUM_COMPLETE:
       {
         /* Complete the GOAL as we already have the CVD resource handles. */
         session_obj->session_ctrl.status = VS_EOK;
         session_obj->session_ctrl.state = VS_VOC_STATE_ENUM_IDLE;
         rc = vs_voc_do_complete_goal( session_obj );
       }
       break;

       default:
         break;
      }
    }
    break;

    case VS_VOC_GOAL_ENUM_DESTROY:
    {
      switch ( session_obj->session_ctrl.action)
      {
        case VS_VOC_ACTION_ENUM_CONTINUE:
        case VS_VOC_ACTION_ENUM_MEMORY_UNMAP:
        {
          session_obj->session_ctrl.state = VS_VOC_STATE_ENUM_RESET_ENTRY;
          session_obj->session_ctrl.action = VS_VOC_ACTION_ENUM_COMPLETE;
          session_obj->session_ctrl.status = VS_EOK;

           rc = vs_voc_action_unmap_memory( session_obj );
        }
        return rc;

        case VS_VOC_ACTION_ENUM_COMPLETE:
        {
          session_obj->session_ctrl.status = VS_EOK;
          rc = vs_voc_do_complete_goal( session_obj );
        }
        return rc;

        default:
          break;
      }
    }
    break;

    default:
      VS_PANIC_ON_ERROR( VS_EUNEXPECTED );
      break;
  }

  return VS_EOK;
}


static int32_t vs_voc_state_idle_entry (
  vs_session_object_t* session_obj
)
{
  int32_t rc = VS_EOK;

  /**
   * End the goal the current for below Scenarios:
   * - Audio-PD is DOWN
   * - ADSP SSR triggered.
   * - last action failed
   */
   if ( FALSE == vs_is_cvd_up() )
  {
    MSG( MSG_SSID_DFLT, MSG_LEGACY_HIGH, "VS: VOC_STATE_IDLE_ENTRY: CVD is not up");
    session_obj->session_ctrl.status = VS_EFAILED;
    rc = vs_voc_do_complete_goal( session_obj );
  }
  else if ( session_obj->cmd_ctrl.cmd_cnt > session_obj->cmd_ctrl.rsp_cnt )
  {
    /* Stay in this state till response for last action is available. */
    return VS_EOK;
  }
  else if ( VS_EOK != session_obj->session_ctrl.status )
  {
    MSG_1( MSG_SSID_DFLT, MSG_LEGACY_ERROR,
           "VS: VOC_STATE_IDLE_ENTRY: last action failed with result[0x%08X]",
           session_obj->session_ctrl.status );
    rc = vs_voc_do_complete_goal( session_obj );
  }

  session_obj->session_ctrl.state = VS_VOC_STATE_ENUM_IDLE;
  return VS_EIMMEDIATE;
}


static int32_t vs_voc_state_idle (
  vs_session_object_t* session_obj
)
{
  int32_t rc = VS_EOK;

  switch ( session_obj->session_ctrl.goal )
  {
    case VS_VOC_GOAL_ENUM_NONE:
      break;

    case VS_VOC_GOAL_ENUM_CONFIGURE:
    {
      switch ( session_obj->session_ctrl.action )
      {
        case VS_VOC_ACTION_ENUM_NONE:
        case VS_VOC_ACTION_ENUM_SET_VOCODER_MODULE_ID:/*encoder and decoder are placeholder modules, so need to set module ID*/
        {
          session_obj->session_ctrl.state = VS_VOC_STATE_ENUM_IDLE_ENTRY;
          session_obj->session_ctrl.action = VS_VOC_ACTION_ENUM_SET_MEDIA_FMT;
          session_obj->session_ctrl.status = VS_EOK; 

          rc  = vs_voc_action_set_encoder_module_id( session_obj );
          rc |= vs_voc_action_set_decoder_module_id( session_obj );
        }
        return rc;

        case VS_VOC_ACTION_ENUM_SET_MEDIA_FMT: /* setting media format in tx and rx can also be made two different actions but avoided it*/
        {
          session_obj->session_ctrl.state = VS_VOC_STATE_ENUM_IDLE_ENTRY;
          session_obj->session_ctrl.action = VS_VOC_ACTION_ENUM_REGISTER_OPERATING_MODE_UPDATE;
          session_obj->session_ctrl.status = VS_EOK;
          rc  = vs_voc_action_set_encoder_media_fmt( session_obj );
          rc |= vs_voc_action_set_decoder_media_fmt( session_obj );
        }
        return rc;

        case VS_VOC_ACTION_ENUM_REGISTER_OPERATING_MODE_UPDATE:
        {
          session_obj->session_ctrl.state = VS_VOC_STATE_ENUM_IDLE_ENTRY;
          session_obj->session_ctrl.action = VS_VOC_ACTION_ENUM_REGISTER_OOBTTY;
          session_obj->session_ctrl.status = VS_EOK;

          rc  = vs_voc_action_register_operating_mode_update( session_obj );
        }
        return rc;

        case VS_VOC_ACTION_ENUM_REGISTER_OOBTTY:
        {
          session_obj->session_ctrl.state = VS_VOC_STATE_ENUM_IDLE_ENTRY;
          session_obj->session_ctrl.action = VS_VOC_ACTION_ENUM_SET_NETWORK_ON_VCPM;
          session_obj->session_ctrl.status = VS_EOK;

          rc  = vs_voc_action_register_oobtty(session_obj );
        }
        return rc;

        case VS_VOC_ACTION_ENUM_SET_NETWORK_ON_VCPM:
        {
          session_obj->session_ctrl.state = VS_VOC_STATE_ENUM_IDLE_ENTRY;
          session_obj->session_ctrl.action = VS_VOC_ACTION_ENUM_SET_VOCODER_CLASS_ON_VCPM;
          session_obj->session_ctrl.status = VS_EOK;

          rc  = vs_voc_action_set_network_key(session_obj );
        }
        return rc;

        case VS_VOC_ACTION_ENUM_SET_VOCODER_CLASS_ON_VCPM:
        {
          session_obj->session_ctrl.state = VS_VOC_STATE_ENUM_IDLE_ENTRY;
          session_obj->session_ctrl.action = VS_VOC_ACTION_ENUM_SET_CACHED_STREAM_PROPERTIES;
          session_obj->session_ctrl.status = VS_EOK;

          rc = vs_voc_action_set_vocoder_class_on_vcpm( session_obj );
        }
        return rc;

        case VS_VOC_ACTION_ENUM_SET_CACHED_STREAM_PROPERTIES:
        {
          session_obj->session_ctrl.state = VS_VOC_STATE_ENUM_IDLE_ENTRY;
          session_obj->session_ctrl.action = VS_VOC_ACTION_ENUM_SET_MAX_SAMPLE_RATE;
          session_obj->session_ctrl.status = VS_EOK;
          rc = VS_EIMMEDIATE;

          if( NULL !=  session_obj->vocoder.set_cached_param_fn )
          {
            ( void ) session_obj->vocoder.set_cached_param_fn( session_obj );
          }
          else
          {
            MSG( MSG_SSID_DFLT, MSG_LEGACY_HIGH, "VS: VOC_STATE_IDLE: "
                                                 "cached_param_fn is not initialized");
          }
        }
        return rc;

        case VS_VOC_ACTION_ENUM_SET_MAX_SAMPLE_RATE:
        {
          session_obj->session_ctrl.state = VS_VOC_STATE_ENUM_IDLE_ENTRY;
          session_obj->session_ctrl.action = VS_VOC_ACTION_ENUM_SET_MBOX_CONFIG;
          session_obj->session_ctrl.status = VS_EOK;

          rc = vs_voc_action_set_max_sample_rate( session_obj );
        }
        return rc;

        case VS_VOC_ACTION_ENUM_SET_MBOX_CONFIG:
        {
          session_obj->session_ctrl.state = VS_VOC_STATE_ENUM_IDLE_ENTRY;
          session_obj->session_ctrl.action = VS_VOC_ACTION_ENUM_VFR_ALLOCATE_AND_SYNC;

          if( TRUE == session_obj->is_vfr_allocated )
          {
            MSG( MSG_SSID_DFLT, MSG_LEGACY_MED,         
                 "VS: VS_VOC_ACTION_ENUM_SET_MBOX_CONFIG: VFR is already allocated: releasing and allocating with new timings" );
            session_obj->session_ctrl.action = VS_VOC_ACTION_ENUM_VFR_RELEASE;
          }

          session_obj->session_ctrl.status = VS_EOK;
          ( void ) vs_voc_action_initialize_mbox( session_obj );
          rc = vs_voc_action_set_tx_mbox_config( session_obj );
          rc |= vs_voc_action_set_rx_mbox_config( session_obj );
        }
        return rc; 

        case VS_VOC_ACTION_ENUM_VFR_RELEASE:
        {
          session_obj->session_ctrl.state = VS_VOC_STATE_ENUM_IDLE_ENTRY;
          session_obj->session_ctrl.action = VS_VOC_ACTION_ENUM_VFR_ALLOCATE_AND_SYNC;
          rc = vs_voc_action_release_vfr( session_obj );
          if( rc ) return rc;
        }
        /* Fall through */

        case VS_VOC_ACTION_ENUM_VFR_ALLOCATE_AND_SYNC:
        {
          session_obj->session_ctrl.state = VS_VOC_STATE_ENUM_IDLE_ENTRY;

          if ( TRUE == vs_platform_is_fusion ) /*For Hana or Kona Fusion*/
          {
             session_obj->session_ctrl.action = VS_VOC_ACTION_ENUM_SET_VOICE_TIMING_OFFSETS;
             /* IMS calls always uses voice timing references. Required to configure VSTMR/VFR
              * using timing references values and also calcualte timing offsets from timing 
              * references on Hana55 or Kona55 fusion.
              * CS voice calls always uses timing offsets - so no need to configure VSTMR/VFR 
              * and no conversion required.
              */
              if( TRUE == session_obj->vocoder.timing_config.is_timing_ref_available )	
              {
                 rc = vs_voc_action_configure_vfr( session_obj );
              }
          }
          else
          {
             /* IMS calls always uses voice timing references. On standalone chipsets, timing
              * references can be directly sent to CVD. Since the QTimer timeticks available to 
              * VS and CVD is same and aligned, timing referneces would be used by CVD directly 
              * to derive timing offsets. No VSTMR/VFR configuration is required. 
              * CS voice calls always uses timing offsets - so no need to configure VSTMR/VFR 
              * and no conversion required.
              */
              if( TRUE == session_obj->vocoder.timing_config.is_timing_ref_available )
               {
              session_obj->session_ctrl.action = VS_VOC_ACTION_ENUM_SET_TIMING_REFERENCE;
               }
          else
               {
                  session_obj->session_ctrl.action = VS_VOC_ACTION_ENUM_SET_VOICE_TIMING_OFFSETS;
               }
           }
          session_obj->session_ctrl.status = VS_EOK;
        }
        return rc;
        
        case VS_VOC_ACTION_ENUM_SET_VOICE_TIMING_OFFSETS:
        {
          session_obj->session_ctrl.state = VS_VOC_STATE_ENUM_IDLE_ENTRY;
          session_obj->session_ctrl.action = VS_VOC_ACTION_ENUM_COMPLETE;
          if ( TRUE == session_obj->vocoder.timing_config.is_timing_ref_available )
          {
            ( void )vs_voc_action_calculate_timing_offsets_from_timing_references( session_obj );
          }
         else
         {
           MSG( MSG_SSID_DFLT, MSG_LEGACY_MED,         
                "vs_voc_state_idle: Timing references are not available. Timing offsets should have been set." );
         }
          rc = vs_voc_action_set_voice_timing_offsets( session_obj );
        }
        return rc;

        case VS_VOC_ACTION_ENUM_SET_TIMING_REFERENCE:
        {
          session_obj->session_ctrl.state = VS_VOC_STATE_ENUM_IDLE_ENTRY;
          session_obj->session_ctrl.action = VS_VOC_ACTION_ENUM_COMPLETE;
          session_obj->session_ctrl.status = VS_EOK;

          if ( session_obj->vocoder.pktexg_event_enable == TRUE )
          {
             rc = vs_voc_action_set_voice_timing_references_v2( session_obj );
          }
          else
          {
             rc = vs_voc_action_set_voice_timing_references( session_obj );
          }
        }
        return rc;

        case VS_VOC_ACTION_ENUM_COMPLETE:
        {
          session_obj->session_ctrl.status = VS_EOK;
          rc = vs_voc_do_complete_goal( session_obj );
        }
        return rc;

      default:
        break;
      }
    }
    break;

    case VS_VOC_GOAL_ENUM_ENABLE:
    {
      switch ( session_obj->session_ctrl.action )
      {
        case VS_VOC_ACTION_ENUM_NONE:
        case VS_VOC_ACTION_ENUM_RESYNC_CTM:
        {
          session_obj->session_ctrl.state = VS_VOC_STATE_ENUM_IDLE_ENTRY;
          session_obj->session_ctrl.action = VS_VOC_ACTION_ENUM_PREPARE_GRAPH;
          session_obj->session_ctrl.status = VS_EOK;

          rc = vs_voc_action_send_resync_tx_ctm( session_obj );
          rc |= vs_voc_action_send_resync_rx_ctm( session_obj );
        }
        return rc;

        case VS_VOC_ACTION_ENUM_PREPARE_GRAPH:
        {
          session_obj->session_ctrl.state = VS_VOC_STATE_ENUM_IDLE_ENTRY;
          session_obj->session_ctrl.action = VS_VOC_ACTION_ENUM_START_VOICE;
          session_obj->session_ctrl.status = VS_EOK;

          rc = vs_voc_action_send_prepare_voice( session_obj );
        }
        return rc;

        case VS_VOC_ACTION_ENUM_START_VOICE:
        {
          session_obj->session_ctrl.state = VS_VOC_STATE_ENUM_IDLE_ENTRY;
          session_obj->session_ctrl.action = VS_VOC_ACTION_ENUM_ENABLE_HPCM;
          session_obj->session_ctrl.status = VS_EOK;

          rc = vs_voc_action_send_start_voice( session_obj );
        }
        return rc;

        case VS_VOC_ACTION_ENUM_ENABLE_HPCM:
        {
          session_obj->session_ctrl.state = VS_VOC_STATE_ENUM_IDLE_ENTRY;
          session_obj->session_ctrl.action = VS_VOC_ACTION_ENUM_COMPLETE;
          session_obj->session_ctrl.status = VS_EOK;
          rc = VS_EIMMEDIATE;

          if( VS_CLIENT_STATE_ENABLED == session_obj->hpcm.client_state )
          {
            rc = vs_voc_action_hpcm_start( session_obj );
          }
          else
          {
            MSG_1( MSG_SSID_DFLT, MSG_LEGACY_HIGH,
                   "VS: VOC_STATE_IDLE: client has not enabled HPCM, state=(%d)",
                   session_obj->hpcm.client_state );
          }
        }
        return rc;

        case VS_VOC_ACTION_ENUM_COMPLETE:
        {
          /* Complete GOAL_ENABLE move to RUN STATE state. */
          session_obj->session_ctrl.status = VS_EOK;
          session_obj->session_ctrl.state = VS_VOC_STATE_ENUM_RUN;
          rc = vs_voc_do_complete_goal( session_obj );
        }
        return rc;

        default:
          break;
      }
    }
    break;

    case VS_VOC_GOAL_ENUM_DISABLE:
    {
      switch( session_obj->session_ctrl.action )
      {
        case VS_VOC_ACTION_ENUM_CONTINUE:
        case VS_VOC_ACTION_ENUM_VFR_RELEASE:
        {
          session_obj->session_ctrl.state = VS_VOC_STATE_ENUM_IDLE_ENTRY;
          session_obj->session_ctrl.action = VS_VOC_ACTION_ENUM_DISABLE_HPCM;
          session_obj->session_ctrl.status = VS_EOK;

          rc = vs_voc_action_release_vfr( session_obj );
        }
        //Fallthrough 
        case VS_VOC_ACTION_ENUM_DISABLE_HPCM:
        {
          /* Lock not needed to state as goal isn't none */
          session_obj->session_ctrl.state = VS_VOC_STATE_ENUM_IDLE_ENTRY;
          session_obj->session_ctrl.action = VS_VOC_ACTION_ENUM_DEREGISTER_OPERATING_MODE_UPDATE;
          rc = VS_EIMMEDIATE;

          if( VS_CLIENT_STATE_ENABLED == session_obj->hpcm.client_state )
          {
            rc = vs_voc_action_hpcm_stop( session_obj );
          }
          else
          {
            MSG_1( MSG_SSID_DFLT, MSG_LEGACY_HIGH,
                   "VS: VOC_STATE_IDLE: HPCM is not enabled by the client, state=(%d)",
                   session_obj->hpcm.client_state );
          }
        }
        return rc;

        case VS_VOC_ACTION_ENUM_DEREGISTER_OPERATING_MODE_UPDATE:
        {
          session_obj->session_ctrl.state = VS_VOC_STATE_ENUM_IDLE_ENTRY;
          session_obj->session_ctrl.action = VS_VOC_ACTION_ENUM_DEREGISTER_OOBTTY;
          session_obj->session_ctrl.status = VS_EOK;

          rc = vs_voc_action_deregister_operating_mode_update( session_obj );
        }
        return rc;

        case VS_VOC_ACTION_ENUM_DEREGISTER_OOBTTY:
        {
          session_obj->session_ctrl.state = VS_VOC_STATE_ENUM_IDLE_ENTRY;
          session_obj->session_ctrl.action = VS_VOC_ACTION_ENUM_STOP_VOICE;
          session_obj->session_ctrl.status = VS_EOK;

          rc = vs_voc_action_deregister_oobtty( session_obj );
        }
        return rc;

        case VS_VOC_ACTION_ENUM_STOP_VOICE:
        {
          /* Lock not needed to state as goal isn't none */
          session_obj->session_ctrl.state = VS_VOC_STATE_ENUM_IDLE_ENTRY;
          session_obj->session_ctrl.action = VS_VOC_ACTION_ENUM_RESET_MODULE_ID;
          session_obj->session_ctrl.status = VS_EOK;

          rc = vs_voc_action_send_stop_voice( session_obj );
        }
        return rc;


        case VS_VOC_ACTION_ENUM_RESET_MODULE_ID:
        {
          session_obj->session_ctrl.state = VS_VOC_STATE_ENUM_IDLE_ENTRY;
          session_obj->session_ctrl.action = VS_VOC_ACTION_ENUM_COMPLETE;
          session_obj->session_ctrl.status = VS_EOK;

          rc = vs_voc_action_reset_encoder_module_id( session_obj );
          rc |= vs_voc_action_reset_decoder_module_id(session_obj);
        }
        return rc;

        case VS_VOC_ACTION_ENUM_COMPLETE:
        {
          /* GOAL DISABLE could get trigger in either of the
           * vocoder state RUN or IDLE. */
          session_obj->session_ctrl.status = VS_EOK;
          rc = vs_voc_do_complete_goal( session_obj );
        }
        return rc;

        default:
          VS_PANIC_ON_ERROR( VS_EUNEXPECTED );
          break;
      }
    }
    break;

    case VS_VOC_GOAL_ENUM_DESTROY:
    {
      switch ( session_obj->session_ctrl.action )
      {
        /* Transition to Reset as after below action stream will be no
         * more attached to MVM.  */
        case VS_VOC_ACTION_ENUM_NONE:
        {
          session_obj->session_ctrl.state = VS_VOC_STATE_ENUM_RESET;
          session_obj->session_ctrl.action = VS_VOC_ACTION_ENUM_CONTINUE;
          rc = VS_EIMMEDIATE;
        }
        return rc;

        default:
          VS_PANIC_ON_ERROR( VS_EUNEXPECTED );
          break;
      }
    }
    break;

    default:
      VS_PANIC_ON_ERROR( VS_EUNEXPECTED );
      break;
  }

  return VS_EOK;
}

/**
 * This is state is just a place holder for completeness. IDLE
 * to RUN transition doesn't involves this state.
 */
static int32_t vs_voc_state_run_entry (
  vs_session_object_t* session_obj
)
{
  return VS_EIMMEDIATE;
}

static int32_t vs_voc_state_run (
  vs_session_object_t* session_obj
)
{
  int32_t rc;

  switch( session_obj->session_ctrl.goal )
  {
    case VS_VOC_GOAL_ENUM_NONE:
      break;

    case VS_VOC_GOAL_ENUM_DISABLE:
    {
      switch ( session_obj->session_ctrl.action )
      {
        case VS_VOC_ACTION_ENUM_NONE:
        {
          session_obj->session_ctrl.state = VS_VOC_STATE_ENUM_IDLE;
          session_obj->session_ctrl.action = VS_VOC_ACTION_ENUM_CONTINUE;
          rc = VS_EIMMEDIATE;
        }
        return rc;

        default:
          VS_PANIC_ON_ERROR( VS_EUNEXPECTED );
          break;
      }
    }
    break;

    default:
      VS_PANIC_ON_ERROR( VS_EUNEXPECTED );
      break;
  }

  return VS_EIMMEDIATE;
}

static uint32_t vs_voc_state_control (
  vs_session_object_t* session_obj
)
{
  uint32_t rc = VS_EOK;

  if ( session_obj == NULL )
  {
    MSG( MSG_SSID_DFLT, MSG_LEGACY_ERROR,
         "vs_voc_state_control(): session_obj is NULL " );
    return VS_EBADPARAM;
  }

  do
  {
     switch ( session_obj->session_ctrl.state )
     {
     case VS_VOC_STATE_ENUM_RESET_ENTRY:
       rc = vs_voc_state_reset_entry( session_obj );
       break;

     case VS_VOC_STATE_ENUM_RESET:
       vs_voc_log_state_info( session_obj );
       rc = vs_voc_state_reset( session_obj );
       break;

     case VS_VOC_STATE_ENUM_IDLE_ENTRY:
       rc = vs_voc_state_idle_entry( session_obj );
       break;

     case VS_VOC_STATE_ENUM_IDLE:
       vs_voc_log_state_info( session_obj );
       rc = vs_voc_state_idle( session_obj );
       break;

     case VS_VOC_STATE_ENUM_RUN_ENTRY:
       rc = vs_voc_state_run_entry( session_obj );
       break;

     case VS_VOC_STATE_ENUM_RUN:
       vs_voc_log_state_info( session_obj );
       rc = vs_voc_state_run( session_obj );
       break;

     default:
       VS_PANIC_ON_ERROR( VS_EUNEXPECTED );
     }
  }
  while ( rc == VS_EIMMEDIATE );

  return rc;
}

uint32_t vs_voc_helper_vocoder_setup_ctrl (
  vs_sequencer_job_object_t* seq_obj,
  vs_session_object_t* session_obj
)
{
  uint32_t rc = VS_EOK;
  vs_simple_job_object_t* simple_obj = NULL;

  for ( ;; )
  {
    switch ( seq_obj->state )
    {
      case VS_HELPER_SEQUENCER_ENUM_GOAL_CREATE:
      {
        seq_obj->state = VS_HELPER_SEQUENCER_ENUM_GOAL_CREATE_WAIT;
        rc = vs_voc_helper_create_new_goal_control(
               VS_VOC_GOAL_ENUM_CREATE, session_obj, &seq_obj->subjob_obj );
        if ( rc )
        {
          seq_obj->state = VS_HELPER_SEQUENCER_ENUM_COMPLETE;
          seq_obj->status = rc;
        }
      }
      continue;

      case VS_HELPER_SEQUENCER_ENUM_GOAL_CREATE_WAIT:
      {
        simple_obj = &seq_obj->subjob_obj->simple_job;
		/* Trigger state machine only if the Graph exists. */
        if ( ( simple_obj->is_completed == FALSE ) && 
			 (session_obj->is_vcpm_opened == TRUE) )
        {
          ( void ) vs_voc_state_control( session_obj );
          rc = VS_EPENDING;
          break;
        }

        MSG_2( MSG_SSID_DFLT, MSG_LEGACY_HIGH,
               "VS: VOCODER_SETUP: sequencer state[%d], status[0x%08x]",
               seq_obj->state, simple_obj->status );
        seq_obj->status = simple_obj->status;
        seq_obj->state = VS_HELPER_SEQUENCER_ENUM_GOAL_CONFIGURE;
		/* complete the sequencer if the graphs are closed or ADSP is DOWN*/
        if ( ( FALSE == vs_is_cvd_up() ) || ( VS_EOK != seq_obj->status ) || (session_obj->is_vcpm_opened == FALSE) )
        {
          seq_obj->state = VS_HELPER_SEQUENCER_ENUM_COMPLETE;
        }

        ( void ) vs_mem_free_object( ( vs_object_t* ) simple_obj );
      }
      continue;

      case VS_HELPER_SEQUENCER_ENUM_GOAL_CONFIGURE:
      {
        seq_obj->state = VS_HELPER_SEQUENCER_ENUM_GOAL_CONFIGURE_WAIT;
        rc = vs_voc_helper_create_new_goal_control(
               VS_VOC_GOAL_ENUM_CONFIGURE, session_obj, &seq_obj->subjob_obj );
        if ( rc )
        {
          seq_obj->state = VS_HELPER_SEQUENCER_ENUM_COMPLETE;
          seq_obj->status = rc;
        }
      }
      continue;

      case VS_HELPER_SEQUENCER_ENUM_GOAL_CONFIGURE_WAIT:
      {
        simple_obj = &seq_obj->subjob_obj->simple_job;
		/* Trigger state machine only if the Graph exists. */
        if ( ( simple_obj->is_completed == FALSE ) && 
			 (session_obj->is_vcpm_opened == TRUE) )
        {
          ( void ) vs_voc_state_control( session_obj );
          rc = VS_EPENDING;
          break;
        }

        MSG_2( MSG_SSID_DFLT, MSG_LEGACY_HIGH,
               "VS: VOCODER_SETUP: sequencer state[%d], status[0x%08x]",
               seq_obj->state, simple_obj->status );
        seq_obj->status = simple_obj->status;
        seq_obj->state = VS_HELPER_SEQUENCER_ENUM_GOAL_ENABLE;
		/* complete the sequencer if the graphs are closed or ADSP is DOWN*/
        if ( ( FALSE == vs_is_cvd_up() ) || ( VS_EOK != seq_obj->status ) || (session_obj->is_vcpm_opened == FALSE) )
        {
          seq_obj->state = VS_HELPER_SEQUENCER_ENUM_COMPLETE;
        }

        ( void ) vs_mem_free_object( ( vs_object_t* ) simple_obj );
      }
      continue;

      case VS_HELPER_SEQUENCER_ENUM_GOAL_ENABLE:
      {
        seq_obj->state = VS_HELPER_SEQUENCER_ENUM_GOAL_ENABLE_WAIT;
        rc = vs_voc_helper_create_new_goal_control(
               VS_VOC_GOAL_ENUM_ENABLE, session_obj, &seq_obj->subjob_obj );
        if ( rc )
        {
          seq_obj->state = VS_HELPER_SEQUENCER_ENUM_COMPLETE;
          seq_obj->status = rc;
        }
      }
      continue;

      case VS_HELPER_SEQUENCER_ENUM_GOAL_ENABLE_WAIT:
      {
        simple_obj = &seq_obj->subjob_obj->simple_job;
		/* Trigger state machine only if the Graph exists. */
        if ( ( simple_obj->is_completed == FALSE ) && 
			 (session_obj->is_vcpm_opened == TRUE) )
        {
          ( void ) vs_voc_state_control( session_obj );
          rc = VS_EPENDING;
          break;
        }

        MSG_2( MSG_SSID_DFLT, MSG_LEGACY_HIGH,
               "VS: VOCODER_SETUP: sequencer state[%d], status[0x%08x]",
               seq_obj->state, simple_obj->status );
        seq_obj->status = simple_obj->status;
        seq_obj->state = VS_HELPER_SEQUENCER_ENUM_COMPLETE;

        ( void ) vs_mem_free_object( ( vs_object_t* ) simple_obj );
      }
      continue;

      case VS_HELPER_SEQUENCER_ENUM_GOAL_DISABLE:
      {
        seq_obj->state = VS_HELPER_SEQUENCER_ENUM_GOAL_DISABLE_WAIT;
        rc = vs_voc_helper_create_new_goal_control(
               VS_VOC_GOAL_ENUM_DISABLE, session_obj, &seq_obj->subjob_obj );
        if ( rc )
        {
          seq_obj->state = VS_HELPER_SEQUENCER_ENUM_COMPLETE;
          seq_obj->status = rc;
        }
      }
      continue;

      case VS_HELPER_SEQUENCER_ENUM_GOAL_DISABLE_WAIT:
      {
        simple_obj = &seq_obj->subjob_obj->simple_job;
		/* Trigger state machine only if the Graph exists. */
        if ( ( simple_obj->is_completed == FALSE ) && 
              (session_obj->is_vcpm_opened == TRUE)
           )
        {
          ( void ) vs_voc_state_control( session_obj );
          rc = VS_EPENDING;
          break;
        }
        seq_obj->status = simple_obj->status;
        seq_obj->state = VS_HELPER_SEQUENCER_ENUM_COMPLETE;
        if ( ( VS_CLIENT_STATE_CLOSED == session_obj->vocoder.client_state ) && ( session_obj->is_vcpm_opened == TRUE ) )
        {
		  /* Create the next goal only if the Graph exists. */
          seq_obj->state = VS_HELPER_SEQUENCER_ENUM_GOAL_DESTROY;
        }
		MSG_2( MSG_SSID_DFLT, MSG_LEGACY_HIGH,
               "VS: VOCODER_SETUP: sequencer state[%d], status[0x%08x]",
               seq_obj->state, simple_obj->status );

        ( void ) vs_mem_free_object( (vs_object_t*)simple_obj );
      }
      continue;

      case VS_HELPER_SEQUENCER_ENUM_GOAL_DESTROY:
      {
        seq_obj->state = VS_HELPER_SEQUENCER_ENUM_GOAL_DESTROY_WAIT;
        rc = vs_voc_helper_create_new_goal_control(
               VS_VOC_GOAL_ENUM_DESTROY, session_obj, &seq_obj->subjob_obj );
        if ( rc )
        {
          seq_obj->state = VS_HELPER_SEQUENCER_ENUM_COMPLETE;
          seq_obj->status = rc;
        }
      }
      continue;

      case VS_HELPER_SEQUENCER_ENUM_GOAL_DESTROY_WAIT:
      {
        simple_obj = &seq_obj->subjob_obj->simple_job;
		/* Trigger state machine only if the Graph exists. */
        if ( ( simple_obj->is_completed == FALSE ) && 
              (session_obj->is_vcpm_opened == TRUE)
           )
        {
          ( void ) vs_voc_state_control( session_obj );
          rc = VS_EPENDING;
          break;
        }

        MSG_2( MSG_SSID_DFLT, MSG_LEGACY_HIGH,
               "VS: VOCODER_SETUP: sequencer state[%d], status[0x%08x]",
               seq_obj->state, simple_obj->status );
        seq_obj->status = simple_obj->status;
        seq_obj->state = VS_HELPER_SEQUENCER_ENUM_COMPLETE;

        ( void ) vs_mem_free_object( (vs_object_t*) simple_obj );
      }
      continue;

      case VS_HELPER_SEQUENCER_ENUM_COMPLETE:
      {
        MSG_1( MSG_SSID_DFLT, MSG_LEGACY_HIGH,
               "VS: VOCODER_SETUP: status=(0x%08X)", seq_obj->status );
        rc = ( FALSE == vs_is_cvd_up() ) ? VS_EFAILED : VS_EOK ;
      }
      break;

      default:
        VS_PANIC_ON_ERROR( VS_EUNEXPECTED );
    }

    break;
  }

  return rc;
}

