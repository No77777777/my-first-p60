/**
  @file vs.c
  @brief This file contains the definitions of the public interfaces of VS.
  This file also includes some common function defintions.

*/

/* 
   Copyright (C) 2013-2021 Qualcomm Technologies, Inc.
   All Rights Reserved.
   Confidential and Proprietary - Qualcomm Technologies, Inc. 

$Header: //components/rel/avs.mpss/9.0.c51/vsd/vs/src/vs.c#1 $
   $Author: pwbldsvc $
*/

/******************************************************************************
 * INCLUDE HEADER FILES                                                     *
 ****************************************************************************/

#include "rcinit.h"
#include "qurt_memory.h"

#include "vs_task.h"
#include "voicecfg_api.h"
#include "voicecfg_items.h"
#include "vs.h"

//Private header files should be last
#include "vs_i.h"


/****************************************************************************
 * DEFINE
 ****************************************************************************/
/* Timeout is set to 5 seconds (5000ms) to give sufficient time for PD
 * restart w/ PD dump enabled to complete.
 */
#define VS_GATING_CMD_TIMEOUT_DURATION_NS ( 5000000000 )


/****************************************************************************
 * EXTERN variables across VS should be defined here.
 ****************************************************************************/

uint16_t vs_my_addr = VS_NULL_V; //This will be removed
uint32_t vs_gpr_handle = VS_NULL_V;

/* flag that indicates the state of vs task. */
bool_t vs_is_initialized = FALSE;

/**< VS service lock to prevent concurrent client access. */
ar_osal_mutex_t vs_service_lock;

vs_session_object_t* vs_session_obj_list[VS_MAX_NUM_SESSIONS_V];

/** Modem-ADSP shared memory related data. */
uint64_t vs_shared_mem_table_virt_addr_start;
uint64_t vs_shared_mem_table_phy_addr_start;
uint32_t vs_shared_mem_table_size;
uint32_t vs_shared_mem_table_handle;

ar_osal_signal_t vs_thread_control_event;

/* flag that indicates the state of audio service (CVD/ADSP). */
bool_t vs_cvd_service_is_up = FALSE;

/* debug count for ADSP SSR and ADSP Audio PDR */
uint32_t vs_adsp_ssr_count = 0;
uint32_t vs_adsp_audio_pdr_count = 0;

/**
 * Should be made false during initialization.
 *
 * vs_cvd_cleanup_is_done is common for cleanup during ADSP SSR/PDR.
 * and during ADSP Timeout.
 */
bool_t vs_cvd_cleanup_is_done = TRUE;

/* Flag that indicates if the adsp mbox memory config received. */
bool_t vs_adsp_mbox_memory_config_is_received = FALSE;

/* Flag that indicates if its a fusion platform. */
bool_t vs_platform_is_fusion = FALSE;

#if defined (VS_PROFILING)
unsigned long long vs_cmd_ticks_in_us = 0;
#endif /* VS_PROFILING */

#if defined (VS_PROFILING_DATA_PCIE)
unsigned long long vs_data_begin_ticks_in_us = 0;
unsigned long long vs_data_end_ticks_in_us = 0;
#endif /* VS_PROFILING_DATA_PCIE */

/* Default VSIDs used by voice services */
uint32_t vs_vsid_table[] = {
  0x11C05000,
  0x11DC5000
};

/****************************************************************************
 * STATIC variables
 ****************************************************************************/

/**
 * This variable indicates the ADSP/CVD timeout has been
 * triggred.
 *
 * This would only be set to false after Mproc reboots,
 * ADSP SSR/AUDIO-PD DOWN noitification or ADSP recovers and
 * provide response to VS over which it is waiting.
 */
bool_t vs_adsp_timeout_is_triggered = FALSE;
vs_timer_t vs_adsp_timeout_timer;

/* VS address is set at initialization. */
static char_t vs_my_dns[] =  "qcom.audio.vs";

/* Lock Management */
static ar_osal_mutex_t vs_objmgr_lock;
  /**< VS objmgr lock. */
static ar_osal_mutex_t vs_cmdq_lock;
  /**<. VS commandQ (gating/non-gating) Lock. */

/* Heap Management */
static vs_memmgr_type vs_heapmgr;
static uint8_t vs_heap_pool[ VS_HEAP_SIZE_V ];

/* Object Management */
static vs_objmgr_object_t vs_object_table[ VS_MAX_OBJECTS_V ];
vs_objmgr_t vs_objmgr;

/* WORK Packet Queue management 
 * Gating/Non-gating queues for all type of incoming packets GPR/CMD. */
static vs_work_item_t vs_work_pkt_pool[ VS_NUM_WORK_PKTS_V ];
static gpr_list_t vs_free_work_pkt_q;
static gpr_list_t vs_work_pkt_q;
static vs_gating_control_t vs_gating_packet_q;

/* The following are used to track the VS objects with type
 * VS_OBJECT_TYPE_ENUM_SIMPLE_JOB and VS_OBJECT_TYPE_ENUM_HANDLE, in order to
 * free them in case of ADSP SSR. */
static vs_generic_item_t vs_obj_tracking_pool[ VS_MAX_OBJECTS_V ];
static gpr_list_t vs_obj_tracking_free_q;
static gpr_list_t vs_obj_tracking_used_q;

/* Thread management. */
static ar_osal_signal_t vs_thread_process_event;
static ar_osal_thread_t vs_thread_handle;
static vs_thread_state_enum_t vs_thread_state = VS_THREAD_STATE_ENUM_INIT;
static uint8_t vs_thread_stack[ VS_THREAD_STACK_SIZE ];

#if !defined ( AVS_MPSS_TEST ) && !defined ( WINSIM ) && !defined ( SLEEP_NPA_STUB )
/* This sleep latency vote is to prevent Q6 from going to deepest low power
 * modes during voice packet exchange window (about 1ms per exchange). 
 * Q6 entering such low power modes would result in delay in servicing 
 * IPA DMA read/write complete interrupts which would lead to packet 
 * delay or miss causing audio quality issues (ex: MOS failure). 
*/
npa_client_handle vs_npa_sleep_latency_node_handle;
#endif

/****************************************************************************
 * COMMON INTERNAL ROUTINES                                                 *
 ****************************************************************************/

static void vs_objmgr_lock_fn ( 
  void 
)
{
  ( void )ar_osal_mutex_lock( vs_objmgr_lock );
  return;
}


static void vs_objmgr_unlock_fn ( 
  void 
)
{
  ( void )ar_osal_mutex_unlock( vs_objmgr_lock );
  return;
}


void vs_cmdq_lock_fn ( 
  void
)
{
  ( void )ar_osal_mutex_lock( vs_cmdq_lock );
  return;
}


void vs_cmdq_unlock_fn ( 
  void
)
{
  ( void )ar_osal_mutex_unlock( vs_cmdq_lock );
  return;
}


void vs_signal_run (
  void
)
{
  ar_osal_signal_set( vs_thread_process_event );
  return;
}


/**
 * Timer callback indicating the ADSP timeout, as VS has not recived the 
 * expected response with VS_VOC_CVD_RESPONSE_TIMEOUT_DURATION_NS.
 *
 * TBD: trigger VS cleanup and recovery if the awaited response is recieved 
 *      eventually without SSR/PDR.
 */
static void vs_gating_cmd_timeout_handler (
 void* client_token
)
{

  MSG( MSG_SSID_DFLT, MSG_LEGACY_ERROR, "VS: gating command timeout triggered !!!" );
  /* TBD: Remove this crash for external/production builds (needs discussion). */
  vs_adsp_timeout_is_triggered = TRUE;
  VS_PANIC_ON_ERROR(VS_EPANIC);
  return;

}


/**
 * Queues the gpr_packet_t (queues both incoming and pending packets to 
 * corresponding queues). In case of failure to queue a gpr packet, packet 
 * will be automatically freed.
 */
static uint32_t vs_queue_gpr_packet (
  gpr_packet_t* packet,
  gpr_list_t* cmd_q
)
{

  uint32_t rc = VS_EOK;
  vs_work_item_t* work_item = VS_NULL_V;
  
  if( packet == VS_NULL_V )
  {
    VS_REPORT_FATAL_ON_ERROR(VS_EBADPARAM);
    return VS_EFAILED;
  }

  for ( ;; )
  {
    /* Get a free command structure. */
    rc = gpr_list_remove_head( &vs_free_work_pkt_q, (gpr_list_node_t**)&work_item );
    if ( VS_NULL_V == work_item )
    {
      VS_REPORT_FATAL_ON_ERROR(VS_ENORESOURCE);
      break;
    }

    work_item->packet = packet;
    rc = gpr_list_add_tail( cmd_q, &work_item->link );
    if ( rc )
    {
      MSG_2( MSG_SSID_DFLT, MSG_LEGACY_ERROR,
             "vs_queue_gpr_packet() - ERROR: opcode=0x%08x, rc=0x%08x",
             packet->opcode, rc );
      /* Add back to vs_free_work_pkt_q */
      work_item->packet = VS_NULL_V;
      ( void ) gpr_list_add_tail( &vs_free_work_pkt_q, &work_item->link );
    }
    else
    {
      /* Signal VS_HIGH thread. */
      vs_signal_run( );
    }

    break;
  } /* for loop ends. */

  return rc;
}

static uint32_t vs_isr_dispatch_fn (
  gpr_packet_t* packet,
  void* dispatch_data
)
{

  uint32_t rc = VS_EOK;

#if defined (VS_PROFILING)
  vs_cmd_ticks_in_us = (uint64_t)((qurt_sysclock_get_hw_ticks()) *10ull/192ull);
  MSG_2( MSG_SSID_DFLT, MSG_LEGACY_HIGH,
         "VS_PROFILING:DEFAULT vs_isr_dispatch_fn : opcode=(0x%08x); vs_cmd_ticks_in_us=(%lu)",
        packet->opcode, vs_cmd_ticks_in_us);
#endif /* VS_PROFILING */

  rc = vs_queue_gpr_packet( packet, &vs_work_pkt_q );
  if ( rc )
  {
    MSG_1( MSG_SSID_DFLT, MSG_LEGACY_ERROR, "VS: cmd pkt queuing failed. rc[0x%08x]", rc );
    rc = __gpr_cmd_free( packet );
    VS_REPORT_FATAL_ON_ERROR(rc);
  }

  return VS_EOK;
}


VS_INTERNAL uint32_t vs_prepare_and_dispatch_cmd_packet (
  uint32_t cmd_id,
  void* p_params,
  uint32_t params_size
)
{
  uint32_t rc = VS_EOK;
  gpr_packet_t* p_cmd_packet = VS_NULL_V;
  void* p_params_1;
  gpr_cmd_alloc_ext_t cmd_params;

  for ( ;; )
  {
    cmd_params.src_domain_id = VS_NULL_V ;
    cmd_params.src_port = VS_NULL_V;
    cmd_params.dst_domain_id = VS_NULL_V ; 
    cmd_params.dst_port = VS_NULL_V ;
    cmd_params.token = VS_NULL_V;
    cmd_params.opcode = cmd_id;
    cmd_params.payload_size = params_size;
    cmd_params.client_data = 0;
    cmd_params.ret_packet =&p_cmd_packet ;

    rc = __gpr_cmd_alloc_ext(&cmd_params);

    if ( rc )
    {
      MSG_1( MSG_SSID_DFLT, MSG_LEGACY_ERROR,
             "VS: GPR packet allocation failed with rc=(0x%08x)", rc );
      break;
    }

    if ( ( params_size > 0 ) && ( p_params != VS_NULL_V ) )
    {
      p_params_1 = GPR_PKT_GET_PAYLOAD( void, p_cmd_packet);
      mmstd_memcpy( p_params_1, params_size, p_params, params_size );
    }

    /* Queue the command packet for processing. */
    rc = vs_queue_gpr_packet( p_cmd_packet, &vs_work_pkt_q );
    if ( rc )
    {
      MSG_1( MSG_SSID_DFLT, MSG_LEGACY_ERROR, "VS: cmd pkt queuing failed. rc[0x%08x]", rc );
      rc = __gpr_cmd_free( p_cmd_packet );
      VS_REPORT_FATAL_ON_ERROR(rc);
    }

    break;
  }

  return rc;
}

/****************************************************************************
 * VS OBJECT ROUTINES.
 ****************************************************************************/

static uint32_t vs_typecast_object (
  vs_objmgr_object_t* p_store,
  vs_object_type_enum_t type,
  vs_object_t** p_ret_obj
)
{

  vs_object_t* p_obj = VS_NULL_V;

  if ( ( p_store == VS_NULL_V ) || ( p_ret_obj == VS_NULL_V ) )
  {
    return VS_EBADPARAM;
  }

  p_obj = ( ( vs_object_t* ) p_store->ptr );

  if ( p_obj->header.type != type )
  {
    return VS_EFAILED;
  }

  *p_ret_obj = p_obj;

  return VS_EOK;

}  /* end of vs_typecast_object () */

uint32_t vs_get_object (
  uint32_t handle,
  vs_object_t** ret_obj
)
{

  uint32_t rc = VS_EOK;
  vs_objmgr_object_t* objmgr_obj = VS_NULL_V;

  for ( ;; )
  {
    if ( ret_obj == VS_NULL_V )
    {
      rc = VS_EBADPARAM;
      break;
    }

    rc = vs_objmgr_find_object( &vs_objmgr, handle, &objmgr_obj );
    if ( rc )
    {
      rc = VS_EFAILED;
      break;
    }

    *ret_obj = ( ( vs_object_t* ) objmgr_obj->ptr );
    break;
  }

  return rc;

}

uint32_t vs_get_list_peak_head (
  gpr_list_t* list,
  gpr_list_node_t** ret_node
)
{

  uint32_t rc = VS_EOK;
  if ( ( list == NULL ) || ( ret_node == NULL ) )
  {
    return VS_EBADPARAM;
  }
  if ( list->lock_fn == NULL )
  {
    return VS_EBADPARAM;
  }
  list->lock_fn( );

  if ( list->dummy.next == &list->dummy )
  {
    list->unlock_fn( );

    return VS_EFAILED;
  }

  *ret_node = list->dummy.next;

  list->unlock_fn( );

  return rc;

}


/**
 *Track the allocated VS objects with type VS_OBJECT_TYPE_ENUM_SIMPLE_JOB and
 * VS_OBJECT_TYPE_ENUM_HANDLE, in order to free them in case of ADSP SSR. VS
 * allocates job object with one of the above types when communicating with the
 * ADSP and frees the object once a response is received from the ADSP. If VS
 * has issued commands to the ADSP and ADSP SSR occurs, VS will not get
 * responses from the ADSP and thus the objects will not be freed. Therefore
 * VS needs to track these types of allocated objects and free them upon ADSP
 * SSR.
 */
static uint32_t vs_track_object (
  uint32_t obj_handle
)
{
  uint32_t rc = VS_EOK;
  vs_generic_item_t* generic_item = VS_NULL_V;

  rc = gpr_list_remove_head( &vs_obj_tracking_free_q, (gpr_list_node_t**)&generic_item );
  VS_PANIC_ON_ERROR( rc );

  generic_item->handle = obj_handle;
  rc = gpr_list_add_tail( &vs_obj_tracking_used_q, &generic_item->link );

  return rc;
}

static uint32_t vs_untrack_object (
  uint32_t obj_handle
)
{
  uint32_t rc = VS_EOK;
  vs_generic_item_t* generic_item = NULL;
  gpr_list_node_t* node;

  generic_item = (vs_generic_item_t*)&vs_obj_tracking_used_q.dummy;

  for ( ;; )
  {
    node= (( gpr_list_node_t* ) generic_item)->next;

    if(node == &vs_obj_tracking_used_q.dummy)
      break;

     generic_item = (vs_generic_item_t*)node;
   /* rc = apr_list_get_next( &vs_obj_tracking_used_q,
                            ( ( gpr_list_node_t* ) generic_item ),
                            ( ( gpr_list_node_t** ) &generic_item ) );
    if( rc ) break;*/

    if ( generic_item->handle == obj_handle )
    {
      rc = gpr_list_delete( &vs_obj_tracking_used_q, &generic_item->link );
      rc = gpr_list_add_tail( &vs_obj_tracking_free_q, &generic_item->link );
      break;
    }
  }

  return rc;
}


/****************************************************************************
 * VS OBJECT CREATION AND DESTRUCTION ROUTINES
 ****************************************************************************/

static uint32_t vs_mem_alloc_object (
  uint32_t size,
  vs_object_t** p_ret_object
)
{
  uint32_t rc = VS_EOK;
  vs_object_t* p_vs_obj = VS_NULL_V;
  vs_objmgr_object_t* p_objmgr_obj = VS_NULL_V;

  if ( p_ret_object == VS_NULL_V )
  {
    return VS_EBADPARAM;
  }

  { /* Allocate memory for the VS object. */
    p_vs_obj = vs_memmgr_malloc( &vs_heapmgr, size );
    if ( p_vs_obj == VS_NULL_V )
    {
      rc = VS_ENORESOURCE;
      return rc;
    }

    /* Allocate a new handle for the VS object. */
    rc = vs_objmgr_alloc_object( &vs_objmgr, &p_objmgr_obj );
    if ( rc )
    {
      vs_memmgr_free ( &vs_heapmgr, p_vs_obj );
      rc = VS_ENORESOURCE;
      return rc;
    }

    /* Link the VS object to the handle. */
    p_objmgr_obj->ptr = p_vs_obj;

    /* Initialize the base VS object header. */
    p_vs_obj->header.handle = p_objmgr_obj->handle;
    p_vs_obj->header.type = VS_OBJECT_TYPE_ENUM_UNINITIALIZED;
  }

  *p_ret_object = p_vs_obj;

  return VS_EOK;
}  /* end of vs_mem_alloc_object () */


static void vs_default_event_handler_fn (
  gpr_packet_t* packet
)
{

  uint32_t rc = VS_EOK;

#if defined (VS_PROFILING)
  vs_cmd_ticks_in_us = (uint64_t)((qurt_sysclock_get_hw_ticks()) *10ull/192ull);
  MSG_2( MSG_SSID_DFLT, MSG_LEGACY_HIGH,
         "VS_PROFILING:DEFAULT GPR EVENT HANDLER(): opcode=(0x%08x); vs_cmd_ticks_in_us=(%ld)",
        packet->opcode, vs_cmd_ticks_in_us);
#endif /* VS_PROFILING */

  /* The default event handler just drops the packet. A specific event
   * handler routine should be written to something more useful. */
  rc =  __gpr_cmd_free( packet );
  VS_REPORT_FATAL_ON_ERROR( rc );

}


static void vs_set_default_response_table (
  vs_response_fn_table_t table
)
{
  int index = 0;

  if ( table == VS_NULL_V ) return;

  /* Initialize the state response handler function table. */
  for ( ; index < VS_RESPONSE_FN_ENUM_MAX; ++index )
  {
    table[ index ] = vs_default_event_handler_fn;
  }

  return;
}


/**
 * VS_OBJECT_TYPE_ENUM_SIMPLE_JOB created with below response
 * fucntion does not have the object reference destroyed.
 *
 * Once responce the available the simple object has to be
 * destoyred explicitly.
 */
static void vs_simple_result_rsp_fn (
  gpr_packet_t* packet
)
{
  uint32_t rc = VS_EOK;
  vs_simple_job_object_t* simple_obj = VS_NULL_V;

  for ( ;; )
  {
    rc = vs_get_typed_object( packet->token, VS_OBJECT_TYPE_ENUM_SIMPLE_JOB,
                              (vs_object_t**)&simple_obj );
    if ( VS_NULL_V == simple_obj )
    {
      VS_REPORT_FATAL_ON_ERROR( rc );
      break;
    }

    simple_obj->is_completed = TRUE;
    simple_obj->status = GPR_PKT_GET_PAYLOAD( gpr_ibasic_rsp_result_t, packet )->status;

    break;
  }

  rc = __gpr_cmd_free( packet );
  VS_REPORT_FATAL_ON_ERROR( rc );

  vs_signal_run( );
  return;
}  /* end of vs_simple_result_rsp_fn ()  */

#if 0
static void vs_simple_self_destruct_result_rsp_fn (
  gpr_packet_t* packet
)
{
  uint32_t rc = VS_EOK;
  vs_token_object_t* token_obj = NULL;

  rc = vs_get_typed_object( packet->token, VS_OBJECT_TYPE_ENUM_HANDLE,
                            ( (vs_object_t**) &token_obj ) );
  if ( rc == VS_EOK )
  {
    ( void ) vs_mem_free_object( (vs_object_t*) token_obj );
    token_obj = NULL;
  }

  rc = __gpr_cmd_free( packet );
  VS_REPORT_FATAL_ON_ERROR( rc );

  vs_signal_run( );
}

VS_INTERNAL uint32_t vs_create_autofree_token_object (
  vs_token_object_t** ret_obj
)
{
  uint32_t rc = VS_EOK;
  vs_token_object_t* vs_obj = VS_NULL_V;

  if ( ret_obj == VS_NULL_V )
  {
    return VS_EBADPARAM;
  }

  rc = vs_mem_alloc_object( sizeof( vs_token_object_t ), (vs_object_t**)&vs_obj );
  VS_PANIC_ON_ERROR( rc );

  /* Initialize the handle object. */
  vs_obj->header.type = VS_OBJECT_TYPE_ENUM_HANDLE;
  vs_set_default_response_table( vs_obj->fn_table );
  vs_obj->fn_table[ VS_RESPONSE_FN_ENUM_RESULT ] = vs_simple_self_destruct_result_rsp_fn;


  /* Track the allocated object in order to free in case of ADSP SSR. */
   rc = vs_track_object( vs_obj->header.handle );

  *ret_obj = vs_obj;

  return VS_EOK;
}
#endif

VS_INTERNAL uint32_t vs_create_simple_job_object (
  uint32_t context_handle,
  vs_simple_job_object_t** ret_obj
)
{
  uint32_t rc = VS_EOK;
  vs_simple_job_object_t* vs_obj = VS_NULL_V;

  if ( ret_obj == VS_NULL_V )
  {
    return VS_EBADPARAM;
  }

  rc = vs_mem_alloc_object( sizeof( vs_simple_job_object_t ), (vs_object_t**)&vs_obj );
  VS_PANIC_ON_ERROR( rc );

  /* Initialize the simple job object. */
  vs_obj->header.type = VS_OBJECT_TYPE_ENUM_SIMPLE_JOB;
  vs_obj->context_handle = context_handle;
  ( void ) vs_set_default_response_table( vs_obj->fn_table );
  vs_obj->fn_table[ VS_RESPONSE_FN_ENUM_RESULT ] = vs_simple_result_rsp_fn;
  vs_obj->is_accepted = FALSE;
  vs_obj->is_completed = FALSE;

  /* Track the allocated object in order to free in case of ADSP SSR. */
  rc = vs_track_object( vs_obj->header.handle );

  *ret_obj = vs_obj;

  return VS_EOK;
}

VS_INTERNAL uint32_t vs_create_sequencer_job_object (
  vs_sequencer_job_object_t** ret_obj
)
{
  uint32_t rc = VS_EOK;
  vs_sequencer_job_object_t* job_obj = VS_NULL_V;

  if ( ret_obj == VS_NULL_V )
  {
    return VS_EBADPARAM;
  }

  rc = vs_mem_alloc_object( sizeof( vs_sequencer_job_object_t ), (vs_object_t**)&job_obj );
  VS_PANIC_ON_ERROR( rc );

  /* Initialize the pending job object. */
  job_obj->header.type = VS_OBJECT_TYPE_ENUM_SEQUENCER_JOB;
  job_obj->state = VS_NULL_V;
  job_obj->subjob_obj = VS_NULL_V;
  job_obj->status = VS_UNDEFINED_ID_V;

  *ret_obj = job_obj;

  return VS_EOK;
}

static uint32_t vs_create_session_object (
  vs_session_object_t** ret_obj
)
{
  uint32_t rc = VS_EOK;
  vs_session_object_t* session_obj = VS_NULL_V;
  void* virt_addr = VS_NULL_V;
  voicemem_cmd_alloc_default_t cmdPayload;
  uint32_t index = 0;

  if ( ret_obj == VS_NULL_V )
  {
    return VS_EBADPARAM;
  }

  rc = vs_mem_alloc_object( sizeof( vs_session_object_t ), (vs_object_t**)&session_obj );
  VS_REPORT_FATAL_ON_ERROR( rc );

  /* Initialize the session object. */
  ( void ) mmstd_memset( (void*)(&session_obj->session_name), 0, sizeof(session_obj->session_name) );

  rc = ar_osal_mutex_create( &session_obj->session_lock );
  VS_PANIC_ON_ERROR( rc );

  /* TODO the initialization of session control. */
  session_obj->session_ctrl.transition_job_handle = VS_NULL_V;
  session_obj->session_ctrl.pendjob_handle = VS_NULL_V;
  session_obj->session_ctrl.state = VS_VOC_STATE_ENUM_RESET;
  session_obj->session_ctrl.status = VS_UNDEFINED_ID_V;
  session_obj->session_ctrl.action = VS_VOC_ACTION_ENUM_NONE;
  session_obj->session_ctrl.goal = VS_VOC_GOAL_ENUM_NONE;

  session_obj->is_stream_ready = FALSE;
  session_obj->is_vcpm_opened = FALSE;
  session_obj->is_vs_enabled = FALSE;
  session_obj->cvs_port = VS_NULL_V;
  session_obj->mvm_port =  VS_NULL_V;
  session_obj->memmap_handle = VS_NULL_V;
  session_obj->is_vfr_allocated = FALSE;

  ( void ) mmstd_memset( (void*)(&session_obj->vocoder.mbox_config), 0, sizeof(vs_mbox_config_t) );
  session_obj->vocoder.client_id = VSS_ICOMMON_CAL_NETWORK_ID_NONE;
  session_obj->vocoder.client_cb = NULL;
  session_obj->vocoder.client_state = VS_CLIENT_STATE_CLOSED;
  session_obj->vocoder.session_context = NULL;
  session_obj->vocoder.pktexg_event_enable = FALSE;

  session_obj->hpcm.client_cb = NULL;
  session_obj->hpcm.client_state = VS_CLIENT_STATE_CLOSED;
  session_obj->hpcm.session_context = NULL;

  { /** Allocating memory block.*/

#if(defined VS_PCM_VOCODER)
    cmdPayload.size = 6 * VS_PAGE_ALIGN_SIZE;
#else
    cmdPayload.size = 4 * VS_PAGE_ALIGN_SIZE;
#endif

    cmdPayload.ret_voicemem_handle = &session_obj->voicemem_handle;
    cmdPayload.ret_virt_addr = &virt_addr;
    cmdPayload.client_name[0] = 'V'; cmdPayload.client_name[1] = 'S'; cmdPayload.client_name[2] = '\0';
    rc = voicemem_call( VOICEMEM_CMD_ALLOC_DEFAULT , &cmdPayload, sizeof(cmdPayload) );
    VS_PANIC_ON_ERROR(rc);

    session_obj->mem_virt_addr_start = (uint64_t) virt_addr;
    session_obj->mem_size = cmdPayload.size;

#if (!defined AVS_MPSS_TEST)
    session_obj->mem_phy_addr_start = (uint64_t) qurt_lookup_physaddr_64( (qurt_addr_t) session_obj->mem_virt_addr_start );
#else
    session_obj->mem_phy_addr_start = (uint64_t) virt_addr;
#endif

  }

  session_obj->vs_ckv_key_value_table[0] = VCPM_CAL_KEY_ID_VOCODER_CLASS;
  session_obj->vs_ckv_key_value_table[1] = VCPM_CAL_KEY_ID_SAMPLING_RATE;

  session_obj->mod_iid_tag_info[VS_MOD_ENCODER].tag_id = VOICE_MOD_TAG_ID_ENCODER;
  session_obj->mod_iid_tag_info[VS_MOD_DECODER].tag_id = VOICE_MOD_TAG_ID_DECODER;
  session_obj->mod_iid_tag_info[VS_MOD_TX_EP].tag_id = VOICE_MOD_TAG_ID_TX_MAILBOX;
  session_obj->mod_iid_tag_info[VS_MOD_RX_EP].tag_id = VOICE_MOD_TAG_ID_RX_MAILBOX;
  session_obj->mod_iid_tag_info[VS_MOD_TX_HPCM].tag_id = VOICE_MOD_TAG_ID_TX_STREAM_HPCM;
  session_obj->mod_iid_tag_info[VS_MOD_RX_HPCM].tag_id = VOICE_MOD_TAG_ID_RX_STREAM_HPCM;
  session_obj->mod_iid_tag_info[VS_MOD_TX_LTETTY].tag_id = VOICE_MOD_TAG_ID_LTETTY_TX;
  session_obj->mod_iid_tag_info[VS_MOD_RX_LTETTY].tag_id = VOICE_MOD_TAG_ID_LTETTY_RX;
  session_obj->mod_iid_tag_info[VS_MOD_TX_CTMTTY].tag_id = VOICE_MOD_TAG_ID_CTMTTY_TX;
  session_obj->mod_iid_tag_info[VS_MOD_RX_CTMTTY].tag_id = VOICE_MOD_TAG_ID_CTMTTY_RX;

  for ( index = 0; index< VS_VCPM_MAX_NUM_MODULES ; index++)
  {
     //session_obj->mod_iid_tag_info[index].tag_id =  VOICE_MOD_TAG_ID_ENCODER + index;
     session_obj->mod_iid_tag_info[index].mod_iid = VS_VOICE_MOD_DEFAULT_IID;
  }

  session_obj->num_sub_graphs = 0;

  for(index = 0; index < VS_VCPM_MAX_NUM_SUBGRAPHS ; index++)
  {
     session_obj->subgraph_info[index].is_stream_subgraph = FALSE;
     session_obj->subgraph_info[index].is_subgraph_started = FALSE;
     session_obj->subgraph_info[index].sub_graph_id = 0xFFFFFFFF;
  }

  /* Complete initialization. */
  session_obj->header.type = VS_OBJECT_TYPE_ENUM_SESSION;

  *ret_obj = session_obj;

  return VS_EOK;
}

/* if this is required more than one place, use this 
uint32_t vs_get_session_from_vsid (
  uint32_t vsid,
  vs_session_object_t** p_ret_obj
)
{
  uint32_t index;\
  vs_session_object_t* session_obj = NULL;

  for(index = 0; index < VS_MAX_NUM_SESSIONS_V; index++)
  {
    session_obj = vs_session_obj_list[index];
    
  }
}
*/

VS_INTERNAL uint32_t vs_mem_free_object (
  vs_object_t* p_object
)
{
  int32_t rc = VS_EOK;
  if ( p_object == NULL )
  {
    return VS_EBADPARAM;
  }

  if ( ( p_object->header.type == VS_OBJECT_TYPE_ENUM_SIMPLE_JOB ) ||
       ( p_object->header.type == VS_OBJECT_TYPE_ENUM_HANDLE ) )
  {
    rc = vs_untrack_object( p_object->header.handle );

    if (rc)
    {
      MSG_1( MSG_SSID_DFLT, MSG_LEGACY_ERROR,
             "vs_mem_free_object(): No tracking object with handle 0x%x",
             p_object->header.handle );
    }
  }

  /* Free the object memory and object handle. */
  ( void ) vs_objmgr_free_object( &vs_objmgr, p_object->header.handle );

  vs_memmgr_free( &vs_heapmgr, p_object );

  return VS_EOK;
}  /* end of vs_mem_free_object () */


VS_INTERNAL uint32_t vs_get_typed_object (
  uint32_t handle,
  vs_object_type_enum_t type,
  vs_object_t** p_ret_obj
)
{

  uint32_t rc = VS_EOK;
  vs_objmgr_object_t* p_store = NULL;

  if ( handle == 0 )
  {
    return VS_EHANDLE;
  }

  rc = vs_objmgr_find_object( &vs_objmgr, handle, &p_store );
  if ( rc )
  {
    return ( uint32_t )rc;
  }

  rc = vs_typecast_object( p_store, type, p_ret_obj );
  if ( rc )
  {
    return ( uint32_t )rc;
  }

  return VS_EOK;

}


VS_INTERNAL uint32_t vs_free_all_tracked_objects (
  void
)
{
  uint32_t vs_rc = VS_EOK;
  vs_generic_item_t* generic_item = NULL;
  vs_object_t* obj = NULL;
  gpr_list_t* temp_list = NULL;

  for ( ;; )
  {
   /* temp_list= &vs_obj_tracking_used_q;
    generic_item = (vs_generic_item_t*)(temp_list->dummy.next);*/

    vs_rc = vs_get_list_peak_head( &vs_obj_tracking_used_q,
                                 ( ( gpr_list_node_t** ) &generic_item ) );
    if ( vs_rc ) break;

    vs_rc = vs_get_object( generic_item->handle, &obj );
    VS_PANIC_ON_ERROR( vs_rc );

    /* Note that vs_mem_free_object will untrack the object. */
    ( void ) vs_mem_free_object ( obj );
  }

  return VS_EOK;
}


VS_INTERNAL uint32_t vs_common_send_event (
  uint32_t handle,
  uint32_t event_id,
  void* payload,
  uint32_t size
)
{
  uint32_t rc = VS_EOK;
  vs_session_object_t* session_obj = NULL;
  vs_common_event_cmd_response_t* cmd_resp = NULL;

  rc = vs_get_typed_object( handle, VS_OBJECT_TYPE_ENUM_SESSION,
                            (vs_object_t**)&session_obj );
  if ( NULL == session_obj )
  {
    VS_REPORT_FATAL_ON_ERROR ( rc );
    return VS_EFAILED;
  }

  switch ( event_id )
  {
    case VS_HPCM_EVENT_NOTIFY_BUFFER:
      if ( NULL != session_obj->hpcm.client_cb )
      {
        rc = session_obj->hpcm.client_cb( event_id, payload, size, session_obj->hpcm.session_context );
      }
      else
      {
        MSG_1( MSG_SSID_DFLT, MSG_LEGACY_ERROR,
               "VS_HPCM_EVENT_NOTIFY_BUFFER is NOT sent due to NULL hpcm.client_cb. handle=0x%08x",
                handle );
      }
      break;

    case VS_VOC_EVENT_READY:
    case VS_VOC_EVENT_NOT_READY:
    case VS_OOBTTY_EVENT_NOTIFY_CHAR:
    case VS_PKTEXG_EVENT_ENCODER_BUFFER_AVAILABLE:
    case VS_PKTEXG_EVENT_DECODER_BUFFER_REQUEST:
    case VS_VOC_EVENT_OPERATING_MODE_UPDATE:
    case VS_VOC_EVENT_PATH_DELAY:
    case VS_PKTEXG_EVENT_NO_DATA_PACKET:
    case VS_VOC_EVENT_RESYNC_DONE:
      if ( NULL != session_obj->vocoder.client_cb )
      {
        rc = session_obj->vocoder.client_cb( event_id, payload, size, session_obj->vocoder.session_context );
      }
      else
      {
        MSG_2( MSG_SSID_DFLT, MSG_LEGACY_ERROR,
               "vs_common_send_event(event=0x%08x) is NOT sent due to NULL vocoder.client_cb. handle=0x%08x",
                event_id, handle );
      }
      break;

    case VS_COMMON_EVENT_CMD_RESPONSE:
    {
      cmd_resp = ( (vs_common_event_cmd_response_t*) payload );
      if( ( VS_HPCM_CMD_CLOSE == cmd_resp->cmd_id ) || 
          ( VS_HPCM_CMD_ENABLE == cmd_resp->cmd_id ) ||
          ( VS_HPCM_CMD_DISABLE == cmd_resp->cmd_id ) )
      {
        if ( NULL != session_obj->hpcm.client_cb )
        {
          session_obj->hpcm.client_cb( event_id, payload, size, session_obj->hpcm.session_context );
        }
        else
        {
          MSG_3( MSG_SSID_DFLT, MSG_LEGACY_ERROR,
                 "VS_COMMON_EVENT_CMD_RESPONSE for cmd_id=(0x%08x) is NOT sent due to NULL hpcm.client_cb. "
                 "status=0x%08x, handle=0x%08x", cmd_resp->cmd_id, cmd_resp->status, handle );
        }
      }
      else
      {
        if ( NULL != session_obj->vocoder.client_cb )
        {
          session_obj->vocoder.client_cb( event_id, payload, size, session_obj->vocoder.session_context );
        }
        else
        {
          MSG_3( MSG_SSID_DFLT, MSG_LEGACY_ERROR,
                 "VS_COMMON_EVENT_CMD_RESPONSE for cmd_id=(0x%08x) is NOT sent due to NULL vocoder.client_cb. "
                 "status=0x%08x, handle=0x%08x", cmd_resp->cmd_id, cmd_resp->status, handle );
        }
      }

      MSG_3( MSG_SSID_DFLT, MSG_LEGACY_MED,
             "VS_COMMON_EVENT_CMD_RESPONSE sent for cmd_id=0x%08x status=0x%08x handle=0x%08x",
             cmd_resp->cmd_id, cmd_resp->status, handle );
    }
    break;

    default:
    {
      MSG_2( MSG_SSID_DFLT, MSG_LEGACY_ERROR,
             "SUD_DBG: vs_common_send_event(event=0x%08x) is NOT handled. handle=0x%08x",
             event_id, handle );
    }
    break;
  }

  return rc;
}


/****************************************************************************
 * NON GATING REQUEST( CMDs/EVENTs ) PROCESSING FUNCTIONS.                   *
 ****************************************************************************/

static void vs_process_nongating_requests (
  void
)
{
  uint32_t rc = VS_EOK;
  uint32_t request_id = 0;
  vs_work_item_t* work_item = NULL;
  gpr_packet_t* packet = NULL;

  while( VS_EOK == gpr_list_remove_head( &vs_work_pkt_q, (gpr_list_node_t**)&work_item ) )
  {
    packet = work_item->packet;

    /* Add back to vs_free_work_pkt_q. */
    work_item->packet = NULL;
    ( void ) gpr_list_add_tail( &vs_free_work_pkt_q, &work_item->link );

    if ( VS_NULL_V == packet )
    {
      MSG( MSG_SSID_DFLT, MSG_LEGACY_ERROR,
            "vs_process_nongating_requests(): Invalid GPR packet " );
      continue;
    }

    request_id = packet->opcode;
    MSG_1( MSG_SSID_DFLT, MSG_LEGACY_HIGH,
           "vs_process_nongating_requests(): Processing opcode=(0x%08X)",
           request_id );

    switch ( request_id )
    {
      case GPR_IBASIC_EVT_ACCEPTED:
        ( void ) vs_response_fn_trampoline( VS_RESPONSE_FN_ENUM_ACCEPTED, packet );
        break;

      case GPR_IBASIC_RSP_RESULT:
        ( void ) vs_response_fn_trampoline( VS_RESPONSE_FN_ENUM_RESULT, packet );
        break;

      case APM_CMD_RSP_SHARED_MEM_MAP_REGIONS:
       ( void ) vs_response_fn_trampoline( VS_RESPONSE_FN_ENUM_MEMORY_MAP, packet );
       break;


      case APM_CMD_RSP_GET_CFG:
       ( void ) vs_response_fn_trampoline( VS_RESPONSE_FN_ENUM_MBOX_MEMORY_CONFIG, packet );
       break;

      case VSS_ISTREAM_EVT_READY:
        rc = vs_stream_ready_event_handler( packet );
        break;

      case VSS_ISTREAM_EVT_NOT_READY:
        rc = vs_stream_not_ready_event_handler( packet );
        break;

      case EVENT_ID_VOCODER_OPERATING_MODE:
        rc = vs_stream_vocoder_mode_change_event_handler( packet );
        break;

      case EVENT_ID_LTETTY_CHAR:
        rc = vs_stream_oobtty_char_notify_event_handler( packet );
        break;

      case VSS_IVPCM_EVT_NOTIFY_V2:
        rc = vs_stream_hpcm_notify_event_handler( packet );
        break;
        
      case VCPM_EVENT_ID_PATH_DELAY:
        rc = vs_path_delay_notify_event_handler( packet );
        break;

      case VCPM_EVENT_ID_GRAPH_CLOSE:
        rc = vs_voc_vcpm_graph_close_event_handler(packet);
        break;

      case MAILBOX_PKTEXG_EVT_NO_DATA_PACKET:
        rc = vs_stream_no_data_pkt_event_handler( packet );
        break;

      case VCPM_EVENT_ID_RESYNC_DONE:
        rc = vs_stream_resync_done_event_handler( packet );
        break;
      
      case APM_EVENT_MODULE_TO_CLIENT:
        rc = vs_module_event_handler( packet );
        break;
      
      case VS_VOC_CMD_ENABLE:
      case VS_VOC_CMD_DISABLE:
      case VS_VOC_CMD_CLOSE:
      case VS_HPCM_CMD_ENABLE:
      case VS_HPCM_CMD_DISABLE:
      case VS_HPCM_CMD_CLOSE:
      case VS_VOC_CMDI_CLEANUP_CVD_REFERENCES:
      case VCPM_EVENT_ID_GRAPH_OPEN:
        rc = vs_queue_gpr_packet( packet, &vs_gating_packet_q.cmd_q );
        if ( rc )
        {
          MSG_1( MSG_SSID_DFLT, MSG_LEGACY_ERROR,
                 "VS: NON-GATING: cmd pkt queuing failed rc[0x%08x]", rc );
          rc = __gpr_cmd_free( packet );
          VS_REPORT_FATAL_ON_ERROR(rc);
        }
        break;


      default:
        rc = __gpr_cmd_free( packet );
        VS_REPORT_FATAL_ON_ERROR(rc);
        MSG_1( MSG_SSID_DFLT, MSG_LEGACY_ERROR,
               "VS: NON-GATING: Unsupported request_id=(0x%08X)", request_id );
        break;
    } /* switch case ends */
  }

  return;
}

static void vs_process_gating_requests (
  void
)
{
  uint32_t rc = VS_EOK;
  uint32_t request_id = 0;
  vs_work_item_t* work_item = NULL;
  gpr_packet_t* gpr_packet = NULL;
  vs_gating_control_t* ctrl = &vs_gating_packet_q;

  for ( ;; )
  {
    switch ( ctrl->state )
    {
      case VS_GATING_CMD_STATE_ENUM_FETCH:
      {
        /* Fetch the next gating command to execute. */
        rc = gpr_list_remove_head( &ctrl->cmd_q, (gpr_list_node_t**)&work_item );
        if ( rc ) return;

        if ( NULL == work_item->packet )
        {
          VS_PANIC_ON_ERROR ( VS_EUNEXPECTED );
        }

        /* Trigger gating command timeout. */
        vs_adsp_timeout_is_triggered = FALSE;
        rc = vs_timer_start ( vs_adsp_timeout_timer, VS_GATING_CMD_TIMEOUT_DURATION_NS );
        VS_REPORT_FATAL_ON_ERROR(rc);

        ctrl->packet = work_item->packet;
        ctrl->state = VS_GATING_CMD_STATE_ENUM_EXECUTE;
        MSG_1( MSG_SSID_DFLT, MSG_LEGACY_LOW,
               "VS: PROCESS_GATING_CMDs(): OPCODE:(0x%08X) STARTED",
               work_item->packet->opcode );

        work_item->packet = NULL;
        ( void ) gpr_list_add_tail( &vs_free_work_pkt_q, &work_item->link );
      }
      break;

      case VS_GATING_CMD_STATE_ENUM_EXECUTE:
      case VS_GATING_CMD_STATE_ENUM_CONTINUE:
      {
        gpr_packet = ctrl->packet;
        request_id = gpr_packet->opcode;

        switch ( request_id )
        {
          case VS_HPCM_CMD_ENABLE:
            rc = vs_hpcm_enable_cmd_ctrl( ctrl );
            break;
          
          case VS_HPCM_CMD_DISABLE:
            rc = vs_hpcm_disable_cmd_ctrl( ctrl );
            break;
          
          case VS_HPCM_CMD_CLOSE:
            rc  = vs_hpcm_close_cmd_ctrl( ctrl );
            break;

          case VS_VOC_CMD_ENABLE:
            rc = vs_voc_enable_cmd_ctrl( ctrl );
            break;

          case VS_VOC_CMD_DISABLE:
            rc = vs_voc_disable_cmd_ctrl( ctrl );
            break;

          case VS_VOC_CMD_CLOSE:
            rc  = vs_voc_close_cmd_ctrl( ctrl );
            break;

          case VS_VOC_CMDI_CLEANUP_CVD_REFERENCES:
            rc  = vs_voc_cleanup_adsp_reference_cmd_ctrl( ctrl );
            break;

          case VCPM_EVENT_ID_GRAPH_OPEN:
            rc = vs_voc_vcpm_graph_open_event_handler(ctrl);
            break;

          default:
            MSG_1( MSG_SSID_DFLT, MSG_LEGACY_ERROR,
                   "vs_process_gating_requests(): OPCODE:(0x%08X) "
                   "UNSUPPORTED", request_id );
              /* set to VS_EOK to fetch the next command in queue. */
            rc = VS_EOK;
            break;
        }

        /* Evaluate the pending command completion status. */
        switch ( rc )
        {
          case VS_EOK:
          {
            MSG_1( MSG_SSID_DFLT, MSG_LEGACY_LOW,
                   "vs_process_gating_requests(): OPCODE:(0x%08X) ENDED", request_id );
            ctrl->packet = NULL;
            ctrl->state = VS_GATING_CMD_STATE_ENUM_FETCH;

            /* NOTE: Add CVD disable and restore hanlding on ADSP recovery
             *       post timeout (needs discussion) */
            rc = vs_timer_stop( vs_adsp_timeout_timer );
            VS_REPORT_FATAL_ON_ERROR(rc);

            rc = __gpr_cmd_free( gpr_packet );
            VS_REPORT_FATAL_ON_ERROR(rc);
          }
          break;

          case VS_EPENDING:
            /**
             * Assuming the current pending command control routine returns
             * VS_EPENDING the overall progress stalls until one or more
             * external events or responses are received.
             */
            ctrl->state = VS_GATING_CMD_STATE_ENUM_CONTINUE;
            /**
             * Return from here so as to avoid unecessary looping till reponse
             * is recived.
             */
            return;

          default:
            VS_REPORT_FATAL_ON_ERROR(rc);
            VS_PANIC_ON_ERROR( VS_EUNEXPECTED );
            break;
        }
      }
      break;

      default:
        VS_PANIC_ON_ERROR( rc );
        break;
    }/* switch case ends. */
  } /* for loop ends. */

  return;
}


/*********************************************************************
 * Supporting subroutines for intialization and de-initialization.
 *********************************************************************/

static uint32_t vs_gating_control_init (
  vs_gating_control_t* p_ctrl
)
{
  uint32_t rc = VS_EOK;

  if ( p_ctrl == NULL )
  {
    return VS_EBADPARAM;
  }

  rc = gpr_list_init_v2( &p_ctrl->cmd_q, vs_cmdq_lock_fn, vs_cmdq_unlock_fn );
  if ( rc )
  {
    return VS_EFAILED;
  }

  p_ctrl->state = VS_GATING_CMD_STATE_ENUM_FETCH;
  p_ctrl->packet = NULL;
  p_ctrl->rootjob_obj = NULL;

  return VS_EOK;
}  /* end of vs_gating_control_init () */


static uint32_t vs_gating_control_destroy (
  vs_gating_control_t* p_ctrl
)
{
  if ( p_ctrl == NULL )
  {
    return VS_EBADPARAM;
  }

  ( void ) gpr_list_clear( &p_ctrl->cmd_q );

  return VS_EOK;
}  /* end of vs_gating_control_destroy () */

static uint32_t vs_obj_tracking_init (
  void
)
{
  int32_t rc = VS_EOK;
  uint32_t index = 0;

  /* Initialize the SSR/PDR object tracking list. */
  rc = gpr_list_init_v2( &vs_obj_tracking_used_q, vs_cmdq_lock_fn, vs_cmdq_unlock_fn );
  VS_REPORT_FATAL_ON_ERROR( rc );

  rc = gpr_list_init_v2( &vs_obj_tracking_free_q, vs_cmdq_lock_fn, vs_cmdq_unlock_fn );
  VS_REPORT_FATAL_ON_ERROR( rc );

  for ( index = 0; index < VS_MAX_OBJECTS_V; ++index )
  {
    ( void ) gpr_list_init_node(
               ( gpr_list_node_t* ) &vs_obj_tracking_pool[ index ] );
    rc = gpr_list_add_tail( &vs_obj_tracking_free_q,
           ( ( gpr_list_node_t* ) &vs_obj_tracking_pool[ index ] ) );
    VS_REPORT_FATAL_ON_ERROR( rc );
  }

  return rc;
}
 
static uint32_t vs_obj_tracking_deinit (
  void
)
{
  ( void ) gpr_list_clear( &vs_obj_tracking_used_q );
  ( void ) gpr_list_clear( &vs_obj_tracking_free_q );

  return VS_EOK;
}

/* when servreg service locator state is UNINIT, then one of the following assumptions
 * is considered TRUE and audio service (CVD) state is reported as UP.
 *  - service registry framework is not supported in the product and CVD is part of the
 *  MPSS software image
 *  - service registry framework is not enabled end-to-end and CVD is part of the
 *  MPSS software image
 *  - service registry framework is not supported in the product and CVD is part of the
 *  ADSP software image - no boot up synchronization
 *  - service registry framework is not enabled end-to-end and CVD is part of the
 *  ADSP software image - no boot up synchronization
 *  
 * This is a temporary workaround. Need to revisit this logic in the future.
 */
bool_t vs_is_cvd_up(
  void
)
{
  #if ( !defined AVS_MPSS_TEST )
  return ( ( TRUE == vs_cvd_service_is_up ) ||
           ( SERVREG_SERVICE_STATE_UNINIT == vs_servreg_locator_service_state )
         );
  #else
  return ( TRUE == vs_cvd_service_is_up );
  #endif
}

/*****************************************************************
 * VS BOOTUP initialization
 *****************************************************************/

void vs_thread (
  void* param
)
{
  uint32_t rc = VS_EOK;

  rc = ( uint32_t ) ar_osal_signal_create( &vs_thread_process_event );
  VS_PANIC_ON_ERROR( rc );

  vs_thread_state = VS_THREAD_STATE_ENUM_READY;
  ( void )ar_osal_signal_set( vs_thread_control_event );

  do
  {
    rc = ( uint32_t )ar_osal_signal_wait( vs_thread_process_event );
    rc = ( uint32_t )ar_osal_signal_clear( vs_thread_process_event );
    ( void ) vs_process_nongating_requests( );
    ( void ) vs_process_gating_requests( );
  }
  while ( rc == VS_EOK );

  rc = ( uint32_t )ar_osal_signal_destroy( vs_thread_process_event );
  VS_REPORT_FATAL_ON_ERROR( rc );

  vs_thread_state = VS_THREAD_STATE_ENUM_EXIT;
  ( void )ar_osal_signal_set( vs_thread_control_event );

  return ;
}

static uint32_t vs_init (
  void
)
{
  uint32_t rc = VS_EOK;
  uint32_t index;
  RCINIT_INFO info_handle = NULL;
  RCINIT_PRIO priority = 0;
  unsigned long stack_size = 0;
  ar_osal_thread_attr_t thread_attr;

  MSG( MSG_SSID_DFLT, MSG_LEGACY_HIGH, 
       "VS: DRV_CMD_INIT: Initialization started!!" );

  /** TBD. */
  rc = ( uint32_t )ar_osal_signal_create( &vs_thread_control_event );
  rc |= ( uint32_t )ar_osal_mutex_create( &vs_objmgr_lock );
  rc |= ( uint32_t )ar_osal_mutex_create( &vs_cmdq_lock );
  rc |= ( uint32_t )ar_osal_mutex_create( &vs_service_lock);
  VS_PANIC_ON_ERROR( rc );


  /* Initialize the custom heap. Memheap must not be called 
     from interrupt context. No locking is required in task context because
     all commands are serialized.*/
  vs_memmgr_init_heap( &vs_heapmgr, (void*)&vs_heap_pool, VS_HEAP_SIZE_V, NULL );

  /* Initialize the object manager. */
  {
    vs_objmgr_setup_params_t params;
    params.table = vs_object_table;
    params.total_bits = VS_HANDLE_TOTAL_BITS_V;
    params.index_bits = VS_HANDLE_INDEX_BITS_V;
    params.lock_fn = vs_objmgr_lock_fn;
    params.unlock_fn = vs_objmgr_unlock_fn;
    rc = vs_objmgr_construct( &vs_objmgr, &params );
  }

  /* Initialize the command queue management. */
  {
    rc = gpr_list_init_v2( &vs_free_work_pkt_q, vs_cmdq_lock_fn, vs_cmdq_unlock_fn );
    for ( index = 0; index < VS_NUM_WORK_PKTS_V; ++index )
    {
      ( void ) gpr_list_init_node( ( gpr_list_node_t* ) &vs_work_pkt_pool[index] );
      vs_work_pkt_pool[index].packet = NULL;
      rc = gpr_list_add_tail( &vs_free_work_pkt_q, ( ( gpr_list_node_t* ) &vs_work_pkt_pool[index] ) );
    }

    rc = gpr_list_init_v2( &vs_work_pkt_q, vs_cmdq_lock_fn, vs_cmdq_unlock_fn );
    rc = vs_gating_control_init( &vs_gating_packet_q );
  }

  rc = __gpr_cmd_register( VS_MODULE_INSTANCE_ID,vs_isr_dispatch_fn, NULL );
  VS_PANIC_ON_ERROR( rc );

  /* Creating VS worker threads */
  {
#if ( !defined AVS_MPSS_TEST )
    info_handle = rcinit_lookup( VS_THREAD_TASK_NAME );
    if ( NULL != info_handle ) 
    {
      priority = rcinit_lookup_prio_info( info_handle );
      stack_size = rcinit_lookup_stksz_info( info_handle );
    }
#endif

    if ( NULL == info_handle ) 
    {
      /* Use the default priority & stack_size*/
      MSG( MSG_SSID_DFLT, MSG_LEGACY_LOW,
           "vs_init(): VS_THREAD task not registered with RCINIT" );
      priority = VS_THREAD_TASK_PRIORITY;
      stack_size = VS_THREAD_STACK_SIZE;
    }

    if ( ( priority > 255 ) || ( stack_size == 0 ) ) 
    {
      ERR_FATAL( "vs_init(): Invalid priority: %d or stack size: %d",
                 priority, stack_size, 0 );
    }

   /* thread attributes*/
    thread_attr.thread_name = VS_THREAD_TASK_NAME;
    thread_attr.priority = TASK_PRIORITY( priority );
    thread_attr.stack_size = stack_size;

    rc = (uint32_t)ar_osal_thread_create( &vs_thread_handle, &thread_attr,vs_thread, NULL );
    VS_PANIC_ON_ERROR( rc );

    /* Wait for service thread to be created. */
    ( void ) ar_osal_signal_wait( vs_thread_control_event );
  }

  /* Initialize the SSR/PDR object tracking list. */
  rc = vs_obj_tracking_init();
  /* VS initialzes for SSR/PDR events. */
  rc = vs_ssr_pdr_init( );
#if ( !defined AVS_MPSS_TEST )
  if ( TRUE == DalPlatformInfo_IsFusion() ) 
  {
    vs_platform_is_fusion = TRUE;
  }    
#else
  vs_platform_is_fusion = FALSE;
#endif
  vs_is_initialized = TRUE;

  MSG_1( MSG_SSID_DFLT, MSG_LEGACY_HIGH,
         "VS: DRV_CMD_INIT: Initialization completed, rc=0x%08x", rc );

  return rc;
}  /* end of vs_init () */


static uint32_t vs_post_init (
  void
)
{
  uint32_t rc = VS_EOK;
  uint32_t index =0;
  void* table_virt_addr = NULL;
  voicemem_cmd_alloc_default_t cmdPayload;

  { /** Allocating memory table physical mem-mapping. */
    cmdPayload.size = VS_PAGE_ALIGN_SIZE;
    cmdPayload.ret_voicemem_handle = &vs_shared_mem_table_handle;
    cmdPayload.ret_virt_addr = &table_virt_addr;
    cmdPayload.client_name[0] = 'V'; cmdPayload.client_name[1] = 'S'; cmdPayload.client_name[2] = '\0';
    rc = voicemem_call( VOICEMEM_CMD_ALLOC_DEFAULT , &cmdPayload, sizeof(cmdPayload) );
    VS_PANIC_ON_ERROR(rc);

    vs_shared_mem_table_virt_addr_start = (uint64_t) table_virt_addr;
    vs_shared_mem_table_size = VS_PAGE_ALIGN_SIZE;

#if (!defined AVS_MPSS_TEST)
    vs_shared_mem_table_phy_addr_start =  (uint64_t) qurt_lookup_physaddr_64( (qurt_addr_t) vs_shared_mem_table_virt_addr_start );
#else
    vs_shared_mem_table_phy_addr_start = (uint64_t) table_virt_addr;
#endif
  }

  /* Initialize the mapping info and open onex voice instance. */
  for( index = 0; index < VS_MAX_NUM_SESSIONS_V ; ++index )
  {
    /* Create and initialize VS session object. */
    rc =  vs_create_session_object ( &vs_session_obj_list[index] );
    vs_session_obj_list[index]->vsid = vs_vsid_table[index];
    VS_PANIC_ON_ERROR( rc );
  }

  /** Initialize the adsp/cvd timeout timer. */
  vs_adsp_timeout_is_triggered = FALSE;
  rc = vs_timer_create( &vs_adsp_timeout_timer,
                         vs_gating_cmd_timeout_handler, NULL );
  VS_PANIC_ON_ERROR(rc);

#if !defined ( AVS_MPSS_TEST ) && !defined ( WINSIM ) && !defined ( SLEEP_NPA_STUB )
  if( TRUE == vs_platform_is_fusion )
  {
    vs_npa_sleep_latency_node_handle = 
          npa_create_sync_client_ex( SLEEP_USEC_NORMAL_LATENCY_NODE_NAME, "VOICE_SERVICES", NPA_CLIENT_REQUIRED, 0, NULL );

    if ( NULL != vs_npa_sleep_latency_node_handle )
    {
      MSG_1( MSG_SSID_DFLT, MSG_LEGACY_HIGH,
           "VS: vs_post_init: npa_create_sync_client_ex successful, handle=(0x%08x)", vs_npa_sleep_latency_node_handle );
    }
    else
    {
      MSG( MSG_SSID_DFLT, MSG_LEGACY_ERROR,
           "VS: vs_post_init: npa_create_sync_client_ex failed" );
    }
  }
#endif

  return rc;
}

static uint32_t vs_run_thread (
  void
)
{
  return VS_EOK;
}

static uint32_t vs_pre_deinit (
  void
)
{
  uint32_t rc = VS_EOK;
  uint32_t index =0;

#if !defined ( AVS_MPSS_TEST ) && !defined ( WINSIM ) && !defined ( SLEEP_NPA_STUB )
  if( TRUE == vs_platform_is_fusion )
  {
    npa_destroy_client( vs_npa_sleep_latency_node_handle );
  }
#endif

  /** Destroy the timers. */
  ( void ) vs_timer_destroy( vs_adsp_timeout_timer );

  /* Close VS Session instance. */
  for( index = 0; index < VS_MAX_NUM_SESSIONS_V; ++index )
  {
    /* Free VS session object. */
    ( void ) ar_osal_mutex_destroy( vs_session_obj_list[index]->session_lock );
    ( void ) vs_mem_free_object( (vs_object_t*)vs_session_obj_list[ index ] );
    vs_session_obj_list[index] = NULL;
  }

  return rc;
}


static uint32_t vs_deinit (
  void
)
{
  vs_is_initialized = FALSE;

  //TBD: free the memtable memory to voicemem, allocated to memory mapping.

  /* Release the GPR resource. */
  ( void ) __gpr_cmd_deregister( VS_MODULE_INSTANCE_ID );

  ( void ) ar_osal_signal_destroy( vs_thread_process_event );

  ( void ) ar_osal_signal_wait( vs_thread_control_event );
  ( void ) ar_osal_thread_join_destroy( vs_thread_handle );

  ( void ) vs_ssr_pdr_deinit();

  ( void ) vs_obj_tracking_deinit( );

  /* Release work packet queue structures. */
  ( void ) gpr_list_clear( &vs_work_pkt_q );
  ( void ) gpr_list_clear( &vs_free_work_pkt_q );
  ( void ) vs_gating_control_destroy( &vs_gating_packet_q );

  /* Release the object management. */
  ( void ) vs_objmgr_destruct( &vs_objmgr );
  ( void ) ar_osal_mutex_destroy( vs_service_lock );
  ( void ) ar_osal_mutex_destroy( vs_cmdq_lock );
  ( void ) ar_osal_mutex_destroy( vs_objmgr_lock );
  ( void ) ar_osal_signal_destroy( vs_thread_control_event );

  MSG( MSG_SSID_DFLT, MSG_LEGACY_HIGH, "VS: DRV_CMD_DEINIT: completed!!" );

  return VS_EOK;
}



/**
 * Single Entry point of VS for clients
 */
VS_EXTERNAL uint32_t vs_call (
  uint32_t cmd_id,
  void* params,
  uint32_t size
)
{
  uint32_t rc = VS_EOK;

  switch ( cmd_id )
  {
  case DRV_CMDID_INIT:
    rc = vs_init( );
    break;

  case DRV_CMDID_POSTINIT:
    rc = vs_post_init( );
    break;

  case DRV_CMDID_PREDEINIT:
    rc = vs_pre_deinit( );
    break;

  case DRV_CMDID_DEINIT:
    rc = vs_deinit( );
    break;

  case DRV_CMDID_RUN:
    rc = vs_run_thread( );
    break;

  case VS_HPCM_CMD_ENABLE:
  case VS_HPCM_CMD_DISABLE:
  case VS_HPCM_CMD_CLOSE:
  case VS_VOC_CMD_ENABLE:
  case VS_VOC_CMD_DISABLE:
  case VS_VOC_CMD_CLOSE:
    MSG_1( MSG_SSID_DFLT, MSG_LEGACY_HIGH,
           "VS: vs_call(): cmd_id=(0x%08x)", cmd_id);
    rc = vs_prepare_and_dispatch_cmd_packet( cmd_id, params, size );
    break;

  case VS_VOC_CMD_MEM_USE_STAT:
    rc = vs_cmd_mem_use_stat_cmd_ctrl( ( vs_voc_mem_use_stat_t* ) params );
    break;

  case VS_VOC_CMD_OPEN:
    rc = vs_voc_open_cmd_ctrl( ( vs_voc_cmd_open_t* ) params );
    break;

  case VS_HPCM_CMD_OPEN:
    rc = vs_hpcm_open_cmd_ctrl( ( vs_hpcm_cmd_open_t* ) params );
    break;

  case VS_COMMON_CMD_SET_PARAM:
    rc = vs_set_param_cmd_ctrl( (vs_common_cmd_set_param_t*) params );
    break;

  case VS_PKTEXG_CMD_GET_ENCODER_BUFFER:
    rc = vs_get_encoder_buffer_cmd_ctrl( (vs_pktexg_cmd_get_encoder_buffer_t*) params );
    break;

  case VS_PKTEXG_CMD_RETURN_ENCODER_BUFFER:
    rc = vs_return_encoder_buffer_cmd_ctrl( (vs_pktexg_cmd_return_encoder_buffer_t*) params );
    break;

  case VS_PKTEXG_CMD_GET_DECODER_BUFFER:
    rc = vs_get_decoder_buffer_cmd_ctrl( (vs_pktexg_cmd_get_decoder_buffer_t*) params );
    break;

  case VS_PKTEXG_CMD_PUT_DECODER_BUFFER:
    rc = vs_put_decoder_buffer_cmd_ctrl( (vs_pktexg_cmd_put_decoder_buffer_t*) params );
    break;

  case VS_PKTEXG_CMD_FLUSH_BUFFERS:
    rc = vs_flush_buffer_cmd_ctrl( (vs_pktexg_cmd_flush_buffers_t*) params );
    break;

  case VS_HPCM_CMD_PUSH_BUFFER:
    rc = vs_hpcm_push_buffer_cmd_ctrl( (vs_hpcm_cmd_push_buffer_t*) params );
    break;

  case VS_OOBTTY_CMD_PUSH_CHAR:
    rc = vs_push_oobtty_char_cmd_ctrl( (vs_oobtty_cmd_push_char_t*) params );
    break;

  default:
    rc = VS_EUNSUPPORTED;
    MSG_1( MSG_SSID_DFLT, MSG_LEGACY_ERROR, "Unsupported CMD-ID(%x)", cmd_id );
    break;
 }

  return rc;
}

