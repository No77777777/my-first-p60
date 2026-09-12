/*==============================================================================
@file npa_remote_qdi_protocol.c

NPA QDI Remote protocol

Copyright (c) 2012-2020 Qualcomm Technologies Incorporated.
All Rights Reserved.
QUALCOMM Proprietary/GTDR

$Header: //components/rel/core.mpss/10.0/power/npa/src/remote/npa_remote_qdi_protocol.c#2 $
==============================================================================*/
#include <qurt.h>
#include "CoreVerify.h"
#include "CoreHeap.h"
#include "npa.h"
#include "npa_resource.h"
#include "npa_remote_resource.h"
#include "npa_remote_qdi_protocol.h"
#include "npa_inlines.h"

#define NPA_QDI_REMOTE_PROTOCOL_NAME "/protocol/qurt/qdi"

typedef struct npa_qdi_remote_resource_data
{
  char name[NPA_MAX_STR_LEN + 1];
  char units[NPA_QDI_REMOTE_MAX_RESOURCE_UNITS_LEN + 1];
} npa_qdi_remote_resource_data;

typedef struct npa_qdi_remote_client
{
  int handle;
  void *vector;
} npa_qdi_remote_client;

static int npa_qdi_drv_hdl;

static npa_resource_state 
npa_qdi_remote_update_fcn( npa_resource *resource, npa_client* client )
{
  int ret;
  npa_resource_state state;
  npa_qdi_remote_client *remote;
  
  CORE_VERIFY_PTR( resource );
  CORE_VERIFY_PTR( client );
  
  remote = (npa_qdi_remote_client *)client->resource_data;
  
  state = NPA_PENDING_REQUEST(client).state;

  switch ( client->type )
  {
  case NPA_CLIENT_REQUIRED:
  case NPA_CLIENT_SUPPRESSIBLE:
    {
	
	  if( client->request_attr & NPA_REQUEST_UNCONDITIONAL)
      {  
        ret = qurt_qdi_handle_invoke( remote->handle,
                                    NPA_QDI_ISSUE_SCALAR_REQUEST_UNCONDITIONAL, 
                                    state, client->request_attr );
      }
      else 
      {
        ret = qurt_qdi_handle_invoke( remote->handle,
                                    NPA_QDI_ISSUE_SCALAR_REQUEST, 
                                    state, client->request_attr );
	  }		  
      CORE_VERIFY( ret == 0 );
      break;
    }
  case NPA_CLIENT_LIMIT_MAX:
    {
      ret = qurt_qdi_handle_invoke( remote->handle,
                                    NPA_QDI_ISSUE_LIMIT_MAX_REQUEST, 
                                    state, client->request_attr );
      CORE_VERIFY( ret == 0 );
      state = resource->request_state;
      break;
    }
  case NPA_CLIENT_VECTOR:
  case NPA_CLIENT_SUPPRESSIBLE_VECTOR:
    {
      npa_resource_state *vector = NPA_PENDING_REQUEST(client).pointer.vector;
      
      if ( state )
      {
        if ( remote->vector && state <= NPA_QDI_USERMALLOC_VECTOR_LEN )
        { /* We can use the usermalloc-d buffer */
          memmove( remote->vector, vector, state * sizeof(npa_resource_state) );
          vector = remote->vector;
        }
      }
      
      ret = qurt_qdi_handle_invoke( remote->handle, 
                                    NPA_QDI_ISSUE_VECTOR_REQUEST, 
                                    state, vector, client->request_attr );
      CORE_VERIFY( ret == 0 );
      break;
    }
  default:
    {
      CORE_UNIMPLEMENTED();
    }
  }
  return state;
}

static unsigned int 
npa_qdi_remote_create_client_ex_fcn( npa_client   *client,
                                     unsigned int  client_value, 
                                     void         *client_ref )
{
  int ret, remote_handle;
  npa_qdi_remote_client *remote;

  CORE_VERIFY_PTR( client );
  
  ret = qurt_qdi_handle_invoke( npa_qdi_drv_hdl, 
                                NPA_QDI_CREATE_SYNC_CLIENT_EX, 
                                client->resource_name,
                                strlen( client->resource_name ),
                                client->name, strlen( client->name ),
                                client->type, client_value, client_ref,
                                &remote_handle );
  if ( ret != 0 )
  {
    return NPA_ERROR;
  }
  
  remote = \
    (npa_qdi_remote_client *) Core_Malloc( sizeof(npa_qdi_remote_client) );
  CORE_VERIFY_PTR( remote );
  
  remote->handle = remote_handle;
  remote->vector = NULL;
  
  if ( npa_resource_has_attribute( client->resource, 
                                   NPA_RESOURCE_VECTOR_STATE ) )
  {
    qurt_qdi_handle_invoke( remote_handle,
                            NPA_QDI_GET_USERMALLOC_VECTOR,
                            &remote->vector );
  }
  
  client->resource_data = remote;
  return NPA_SUCCESS;
}

static void npa_qdi_remote_destroy_client_fcn( npa_client *client )
{
  npa_qdi_remote_client *remote;
  CORE_VERIFY_PTR( client );
  
  remote = (npa_qdi_remote_client *)client->resource_data;
  CORE_VERIFY_PTR(remote);
  
  qurt_qdi_close( remote->handle );
  Core_Free( remote );
}

const npa_resource_plugin npa_qdi_remote_plugin =
{
  npa_qdi_remote_update_fcn,
  NPA_CLIENT_REQUIRED|NPA_CLIENT_SUPPRESSIBLE|NPA_CLIENT_LIMIT_MAX|
  NPA_CLIENT_VECTOR|NPA_CLIENT_SUPPRESSIBLE_VECTOR,
  NULL,
  npa_qdi_remote_destroy_client_fcn,
  npa_qdi_remote_create_client_ex_fcn,
  NULL
};

static npa_resource_state 
npa_qdi_remote_driver_fcn( npa_resource      *resource,
                           npa_client_handle  client,
                           npa_resource_state state )
{
  CORE_VERIFY_PTR( resource );
  CORE_VERIFY_PTR( client );
  
  if ( client->type == NPA_CLIENT_INITIALIZE )
  {
    if ( 0 == strncmp( NPA_QDI_REMOTE_PROTOCOL_NAME,
                       resource->definition->name,
                       sizeof(NPA_QDI_REMOTE_PROTOCOL_NAME) ) )
    {
      npa_qdi_drv_hdl = qurt_qdi_open( "/dev/npa" );
      CORE_VERIFY( npa_qdi_drv_hdl > 0 );
    }
    else
    {
      /* Create a query handle to be able to query the remote resource
         when requested */
      int remote_handle;
      CORE_VERIFY( 0 == qurt_qdi_handle_invoke( npa_qdi_drv_hdl, 
                                                NPA_QDI_CREATE_QUERY_HANDLE, 
                                                resource->definition->name,
                                                &remote_handle ) );
      resource->definition->data = (npa_user_data)remote_handle;
      resource->active_plugin = &npa_qdi_remote_plugin;
    }
  }
  
  return state;
}

static npa_query_status 
npa_qdi_remote_query_fcn( npa_resource   *resource,
                          unsigned int    query_id, 
                          npa_query_type *query_result )
{
  int ret;
  
  CORE_VERIFY_PTR( resource );
  CORE_VERIFY_PTR( query_result );

  if ( query_id == NPA_QUERY_REMOTE_RESOURCE_AVAILABLE )
  {
    /* The Kernel/RootPD boots first, so all of its resources are available */
    npa_remote_resource_available( query_result->data.reference );
    return NPA_QUERY_SUCCESS;
  }
  
  ret = qurt_qdi_handle_invoke( (int)resource->definition->data,
                                NPA_QDI_QUERY,
                                query_id, query_result );
  CORE_VERIFY( ret >= 0 );
  
  return (npa_query_status)ret;
}

static npa_resource_definition npa_qdi_remote_protocol_node_resource[] = 
{ 
  {
    NPA_QDI_REMOTE_PROTOCOL_NAME,
    "na",
    0,
    &npa_no_client_plugin,
    NPA_RESOURCE_DEFAULT,
    NULL,
    npa_qdi_remote_query_fcn
  }
};

static npa_node_definition npa_qdi_remote_protocol_node = 
{ 
  "/node/protocol/qurt/qdi",
  npa_qdi_remote_driver_fcn, 
  NPA_NODE_DEFAULT,
  NULL,
  NPA_EMPTY_ARRAY,
  NPA_ARRAY(npa_qdi_remote_protocol_node_resource)
};

void npa_qdi_meta_event_cb(void         *context,
                          unsigned int event_type,
                          void         *data,
                          unsigned int data_size)
{
  int qdi_eh;
  int ret;
  npa_qdi_remote_client *remote;
  
  npa_event *event = (npa_event *)data;
  
  CORE_VERIFY_PTR( event );
  
  if (!NPA_EVENT_IS_DESTROYED(event))
  {	  
	ret = qurt_qdi_handle_invoke( npa_qdi_drv_hdl, NPA_QDI_CREATE_EVENT_HANDLE, 
                          event->resource->node->name, event->name, event->trigger_type, event, &qdi_eh );
	CORE_VERIFY(ret == 0);
	
	remote = (npa_qdi_remote_client *) Core_Malloc( sizeof(npa_qdi_remote_client) );
	CORE_VERIFY_PTR( remote );
	remote->handle = qdi_eh;
	remote->vector = NULL;
    event->meta_event_data = (void *)remote;					  
  }
  else
  {
    remote = (npa_qdi_remote_client *)event->meta_event_data;
    CORE_VERIFY_PTR(remote);
    qurt_qdi_close( remote->handle );
    Core_Free( remote );  
  }
  
  return;
}

void npa_qdi_remote_define_proxy_resources(void)
{
  int pid, ret;
  unsigned count = 0;
  npa_remote_resource_definition *resource_defns = NULL;
  npa_qdi_remote_resource_data *resource_data = NULL;

  /* The driver handle should already be initialized */
  CORE_VERIFY( npa_qdi_drv_hdl > 0 );
  
  pid = qurt_getpid();
  CORE_VERIFY( pid > 0 && pid <= 32 );
  
  /* Since the kernel PD boots first, we assume that all kernel resources have
     been created and published */
  ret = qurt_qdi_handle_invoke( npa_qdi_drv_hdl,
                                NPA_QDI_GET_PUBLISHED_RESOURCE_COUNT,
                                pid, &count );
  CORE_VERIFY( ret == 0 );
  
  if ( count > 0 )
  {
    int i;
    resource_defns = (npa_remote_resource_definition *)
      Core_Malloc( sizeof(npa_remote_resource_definition) * count );
    CORE_VERIFY_PTR( resource_defns );
    
    resource_data = (npa_qdi_remote_resource_data *)
                    Core_Malloc( sizeof(npa_qdi_remote_resource_data) * count );
    CORE_VERIFY_PTR( resource_data );
    
    for ( i = 0; i < count; i++ )
    {
      resource_defns[i].local_resource_name = resource_data[i].name;
      resource_defns[i].remote_resource_name = resource_data[i].name;
      resource_defns[i].protocol_type = NPA_QDI_REMOTE_PROTOCOL_NAME;
      resource_defns[i].plugin = &npa_qdi_remote_plugin;
      resource_defns[i].units = resource_data[i].units;
    }
  
    ret = qurt_qdi_handle_invoke( npa_qdi_drv_hdl,
                                  NPA_QDI_GET_PUBLISHED_RESOURCE_DEFINITION,
                                  pid, count, resource_defns );
    CORE_VERIFY( ret == 0 );
    
    for ( i = 0; i < count; i++ )
    {
      npa_remote_define_resource_cb( &resource_defns[i], 0, NULL, NULL );
      npa_create_meta_event(resource_defns[i].local_resource_name, "npa_qdi", npa_qdi_meta_event_cb, NULL);
    }
  }
}

static void npa_qdi_dispatch_events( void *arg )
{
  int ret = -1;
  npa_event_data event_data;
  npa_event_handle event;
  unsigned int event_type;
  
  while (1)
  {
    event = NULL;
    event_type = 0;
    memset(&event_data, 0, sizeof(npa_event_data));
    
    ret = qurt_qdi_handle_invoke( npa_qdi_drv_hdl,
                                  NPA_QDI_ANY_RESOURCE_EVENT,
                                  &event,
                                  &event_type,
                                  &event_data );
    
    if ( ret == 0 && event && NPA_EVENT_IS_ACTIVE(event) )
    {
      npa_dispatch_event_callback( &event->callback, event_type,
                                   &event_data, sizeof(npa_event_data) );
    }
    }
  }

#define NPA_QDI_EVENT_DISPATCHER_STKSIZE 1024
extern npa_config npa_config_data;

void npa_qdi_create_event_dispatcher()
{
  qurt_thread_t tid;
  qurt_thread_attr_t attr;
  void *stack;
  char name[QURT_THREAD_ATTR_NAME_MAXLEN];

  qurt_thread_attr_init(&attr);
  stack = Core_Malloc(NPA_QDI_EVENT_DISPATCHER_STKSIZE);
  CORE_VERIFY_PTR( stack );
  
  qurt_thread_attr_set_stack_size(&attr, NPA_QDI_EVENT_DISPATCHER_STKSIZE);
  qurt_thread_attr_set_stack_addr(&attr, stack);
  qurt_thread_attr_set_priority(&attr, 
                                npa_config_data.async_event_workloop.priority);
                                
  snprintf( name, sizeof(name), "npaqdi_evth%d", qurt_getpid() );
  qurt_thread_attr_set_name(&attr, name);

  CORE_VERIFY( 
    QURT_EOK == qurt_thread_create(&tid, &attr, npa_qdi_dispatch_events, NULL));
}

void npa_qdi_remote_init( void )
{
  npa_define_node_cb( &npa_qdi_remote_protocol_node, NULL, NULL, NULL );
  npa_qdi_remote_define_proxy_resources();
  
  npa_qdi_create_event_dispatcher();
}
