/*
  Copyright (c) 2019 Qualcomm Technologies, Inc.
  All Rights Reserved.
  Confidential and Proprietary - Qualcomm Technologies, Inc.

  $Header: //components/rel/avs.mpss/9.0.c51/vsd/utils/src/vs_timer.c#1 $
  $Author: pwbldsvc $
*/

#include "vs_timer.h"

#define VS_TIMER_MASK_RESERVED ( 0x1 )
#define VS_TIMER_MASK_EXPIRED  ( 0x2 )
#define VS_TIMER_MASK_DESTROY  ( 0x4 )
#define VS_TIMER_MASK_EXIT     ( 0x8 )
#define VS_TIMER_THREAD_PRI_ORDER ( 50 )
#define VS_TIMER_MASK_THREAD_WAIT (VS_TIMER_MASK_EXPIRED|\
                                    VS_TIMER_MASK_DESTROY)
#define VS_TIMER_THREAD_STACK_SIZE ( 16384 ) //TBD: can be reduced further
#define VS_TIMER_THREAD_NAME ( "VS_TIMER" )

typedef struct vs_timer_internal_t
{
  timer_type handle;
  qurt_anysignal_t  signal;
  unsigned int      mask;
  qurt_mutex_t      vs_timer_lock;
  vs_timer_fn_t timer_cb;
  void* client_token;
  bool_t            is_thread_active;
  struct
  {
    unsigned int handle;
    char_t* name;
    uint8_t stack[ VS_TIMER_THREAD_STACK_SIZE ];
    qurt_thread_attr_t attr;
    qurt_thread_t tid;
    void* param;
    int32_t result;
  }timer_thread;
}
  vs_timer_internal_t;

static void vs_timer (
  void* param
)
{
  vs_timer_internal_t* the_timer = ( ( vs_timer_internal_t* ) param );
  unsigned int ret_mask;

  if ( the_timer != NULL )
  {
      for(;;)
      {
        ret_mask = qurt_anysignal_wait(
                     &the_timer->signal, VS_TIMER_MASK_THREAD_WAIT);
        if( ret_mask&(VS_TIMER_MASK_DESTROY) )
        {
          the_timer->is_thread_active=FALSE;
          qurt_anysignal_clear(&the_timer->signal, VS_TIMER_MASK_THREAD_WAIT);
          qurt_thread_exit( 0 ); 
        }        
        qurt_anysignal_clear(&the_timer->signal, VS_TIMER_MASK_EXPIRED); 
        the_timer->timer_cb( the_timer->client_token );
      }
   the_timer->is_thread_active=FALSE;
  }
  qurt_thread_exit( 0 );
}

uint32_t vs_timer_create (
  vs_timer_t* ret_timer,
  vs_timer_fn_t timer_cb,
  void* client_token
)
{
  vs_timer_internal_t* the_timer;
  uint32_t rc = VS_EOK;
  DALResult dal_rc;
  timer_group_ptr group_ptr = &timer_non_defer_group;

  if ( ret_timer == NULL )
  {
    return VS_EBADPARAM;
  }

  dal_rc = DALSYS_Malloc( sizeof( vs_timer_internal_t ), ( ( void** ) &the_timer ) ); //Is using DAL APIs fine here
  if ( dal_rc != DAL_SUCCESS )
  {
    MSG_1( MSG_SSID_DFLT, MSG_LEGACY_ERROR, "vs_timer_create(): Allocating memory for timer failed with rc %d", dal_rc );
    return VS_ENORESOURCE;
  }
  mmstd_memset( the_timer, 0, sizeof( vs_timer_internal_t ) );

  rc = (uint32_t)timer_def2( &the_timer->handle, group_ptr);  

  if(rc)
  {
    MSG_1( MSG_SSID_DFLT, MSG_LEGACY_ERROR, "vs_timer_create(): Initialization of timer failed with rc %d", rc );
    DALSYS_Free( the_timer );
    return VS_EFAILED;
  }

  qurt_anysignal_init(&the_timer->signal);
  the_timer->mask= VS_TIMER_MASK_EXIT;
  qurt_mutex_init(&the_timer->vs_timer_lock);

  the_timer->timer_cb = timer_cb;
  the_timer->client_token = client_token;

  *ret_timer = ( ( vs_timer_t* ) the_timer );
  return VS_EOK;

}

 uint32_t vs_timer_destroy (
  vs_timer_t timer
)
{
  int thrd_join_status = 0;
  vs_timer_internal_t* the_timer = ( ( vs_timer_internal_t* ) timer );
  DALResult dal_rc;
  uint32_t rc = VS_EOK;

  if ( timer == NULL )
  {
    return VS_EBADPARAM;
  }

  qurt_mutex_lock (&the_timer->vs_timer_lock);
//clear timer before destroying
  timer_clr(&the_timer->handle, T_NONE);

  if(the_timer->is_thread_active)
  {
    the_timer->mask = VS_TIMER_MASK_DESTROY;
    rc = qurt_anysignal_set(&the_timer->signal, VS_TIMER_MASK_DESTROY); 
    if(rc)
    {
      MSG_1( MSG_SSID_DFLT, MSG_LEGACY_ERROR, "vs_timer_destroy(): Destroying timer failed with rc %d", rc );
      qurt_mutex_unlock (&the_timer->vs_timer_lock);
      return VS_EFAILED;
    }
  }
  qurt_mutex_unlock (&the_timer->vs_timer_lock);

  ( void ) qurt_thread_join(the_timer->timer_thread.tid, &thrd_join_status);

  rc = (uint32_t)timer_undef(&the_timer->handle);

  if(rc)
  {
    MSG_1( MSG_SSID_DFLT, MSG_LEGACY_ERROR, "vs_timer_destroy(): undefining timer failed with rc %d", rc );
  }

  qurt_anysignal_destroy(&the_timer->signal);
  qurt_mutex_destroy(&the_timer->vs_timer_lock);
  
  dal_rc = DALSYS_Free( the_timer );
  if ( dal_rc != DAL_SUCCESS )
  {
    MSG_1( MSG_SSID_DFLT, MSG_LEGACY_ERROR, "vs_timer_destroy(): Freeing memory for timer failed with rc %d", dal_rc );
    return VS_EFAILED;
  }

  return rc;

}

/* function called during timer expiry*/
static void vs_timer_interrupt_handler (
  int32 time_ms,
  timer_cb_data_type data
)
{
  uint32_t rc = VS_EOK;
  vs_timer_internal_t* the_timer = ( ( vs_timer_internal_t* ) data );
  
  if ( the_timer != NULL )
  {
      rc = (uint32_t)qurt_anysignal_set(&the_timer->signal, VS_TIMER_MASK_EXPIRED);
      if ( rc )
      {
         MSG_1( MSG_SSID_DFLT, MSG_LEGACY_ERROR, "vs_timer_interrupt_handler(): Failed to set signal with rc %d", rc );
      }
  }
  return;
}  

uint32_t vs_timer_start (
  vs_timer_t timer,
  uint64_t time_ns
)
{
  uint32_t rc = VS_EOK;
  vs_timer_internal_t* the_timer = ( ( vs_timer_internal_t* ) timer );
  typedef void (*timer_handler_func_t) (void *);
  void * pStack;
  void * pStackAlligned;
  unsigned int alligned_stack_size;
  uint64_t time_ms = time_ns/1000000; 
  
   if ( the_timer == NULL )
  {
    return VS_EBADPARAM;
  }
  
   qurt_mutex_lock (&the_timer->vs_timer_lock);
   if(!the_timer->is_thread_active)
   {
      ( void ) mmstd_memset( &the_timer->timer_thread, 0, sizeof( the_timer->timer_thread ) );

      the_timer->timer_thread.param = the_timer;
      the_timer->timer_thread.name = VS_TIMER_THREAD_NAME;
  
      /* Align the stack pointer. */
      pStack = ( void* )( &the_timer->timer_thread.stack );
      pStackAlligned = ( void* )( (uint32_t)((uint32_t)pStack + 127) & (~127) );
  
     /* Calculate alligned stack size. */
     alligned_stack_size = (unsigned int)(VS_TIMER_THREAD_STACK_SIZE-((uint32_t)pStackAlligned - (uint32_t)pStack));
     alligned_stack_size -= alligned_stack_size % 128;
   
     qurt_thread_attr_init (&the_timer->timer_thread.attr);
     qurt_thread_attr_set_stack_size(&the_timer->timer_thread.attr, alligned_stack_size );
     qurt_thread_attr_set_stack_addr(&the_timer->timer_thread.attr, pStackAlligned );
     qurt_thread_attr_set_priority (&the_timer->timer_thread.attr, VS_TIMER_THREAD_PRI_ORDER );
     qurt_thread_attr_set_name(&the_timer->timer_thread.attr, the_timer->timer_thread.name );
   
     rc = qurt_thread_create(&the_timer->timer_thread.tid, &the_timer->timer_thread.attr, (timer_handler_func_t)vs_timer,
           (void *) the_timer);
     if( rc )
     {
        MSG_1( MSG_SSID_DFLT, MSG_LEGACY_ERROR, "vs_timer_start(): failed to create thread with rc %d", rc );
        qurt_mutex_unlock (&the_timer->vs_timer_lock);
        return VS_EFAILED;
     }
     the_timer->is_thread_active=TRUE;
   }
  
   rc = timer_reg(&the_timer->handle, vs_timer_interrupt_handler,
             ( ( timer_cb_data_type ) timer ), ( ( uint32 ) ( time_ms ) ), 0);
   if(rc)
   {
       MSG_1( MSG_SSID_DFLT, MSG_LEGACY_ERROR, "vs_timer_start(): failed to register with rc %d", rc );
       qurt_mutex_unlock (&the_timer->vs_timer_lock);
       return VS_EFAILED;
   }
   qurt_mutex_unlock (&the_timer->vs_timer_lock);
   return VS_EOK;

}

 uint32_t vs_timer_stop (
  vs_timer_t  timer
)
{
  vs_timer_internal_t* the_timer = ( ( vs_timer_internal_t* ) timer );
  
  if ( timer == NULL )
  {
    return VS_EBADPARAM;
  }
  
  timer_clr(&the_timer->handle, T_NONE);

  return VS_EOK;
}

