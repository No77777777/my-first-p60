/*
  Copyright (C) 2009-2011, 2012 QUALCOMM Technologies, Inc.
  All rights reserved.
  Confidential and Proprietary - QUALCOMM Technologies, Inc.

  $Header: //components/rel/avs.mpss/9.0.c51/vsd/utils/src/vs_objmgr.c#1 $
  $Author: pwbldsvc $
*/

/****************************************************************************
 * INCLUDE HEADER FILES                                                     *
 ****************************************************************************/

#include "mmdefs.h" /* NULL */
#include "vs_errcodes.h" 
#include "vs_objmgr.h"

/****************************************************************************
 * EXPORTED ROUTINES                                                        *
 ****************************************************************************/

 int32_t vs_objmgr_construct (
  vs_objmgr_t* objmgr,
  vs_objmgr_setup_params_t* params
)
{
  int32_t rc;
  uint32_t i;
  vs_objmgr_object_t* obj;

  if ( ( objmgr == NULL ) ||
       ( params == NULL ) ||
       ( params->index_bits == 0 ) ||
       ( params->total_bits < ( params->index_bits + 1 /* version */ ) ) ||
         /* Handle must have sufficient space to hold mandatory fields. */
       ( ( params->lock_fn == NULL ) || ( params->unlock_fn == NULL ) )
         /* Must provide lock routines even if they're not used. */
     )
  {
    return VS_EBADPARAM;
  }

  rc = gpr_list_init_v2( &objmgr->free_q, params->lock_fn, params->unlock_fn );
  if ( rc )
  {
    return VS_EFAILED;
  }

  objmgr->table = params->table;
  objmgr->total_num = ( 1 << params->index_bits );
  objmgr->index_mask = ( objmgr->total_num - 1 );
  objmgr->overflow_mask = ( ( 1 << params->total_bits ) - 1 );
  objmgr->lock_fn = params->lock_fn;
  objmgr->unlock_fn = params->unlock_fn;

  for ( i = 0; i < objmgr->total_num; ++i )
  {
    obj = &objmgr->table[ i ];
    obj->handle = i;
      /* TODO: Initialize the version field to a random value. */
    obj->type = 0;
    obj->ptr = NULL;
    (void) gpr_list_init_node( ( gpr_list_node_t* ) &obj->link);
    rc = gpr_list_add_tail( &objmgr->free_q, &obj->link );
  }

  return VS_EOK;
}

int32_t vs_objmgr_destruct (
  vs_objmgr_t* objmgr
)
{
  if ( objmgr == NULL )
  {
    return VS_EBADPARAM;
  }

  ( void ) gpr_list_clear( &objmgr->free_q );

  return VS_EOK;
}

/* TODO: The L4 platform running on an SMT (Q6) architecture could end up with
*        application threads and interrupt threads in parallel. Acquiring the
*        lock is therefore required to accessed shared resources. The lifetime
*        of objects is typically controlled by the task context. An ISR may
*        need to hold onto the ISR lock until it's done with the object to
*        prevent the task from deleting it. We should use different locks to
*        reduce ISR and task contentions in the applications.
*/

 int32_t vs_objmgr_alloc_object (
  vs_objmgr_t* objmgr,
  vs_objmgr_object_t** ret_object
)
{
  int32_t rc;

  if ( ( objmgr == NULL ) || ( ret_object == NULL ) )
  {
    return VS_EBADPARAM;
  }

  rc = gpr_list_remove_head( &objmgr->free_q, ( ( gpr_list_node_t** ) ret_object ) );
  if ( rc )
  {
    return VS_EFAILED;
  }

  ( *ret_object )->handle |= objmgr->total_num;
    /* Claim the free object by setting the visible flag. */

  return VS_EOK;
}

 int32_t vs_objmgr_find_object (
  vs_objmgr_t* objmgr,
  uint32_t handle,
  vs_objmgr_object_t** ret_object
)
{
  int32_t rc = VS_EOK;
  vs_objmgr_object_t* obj;

  if ( ( objmgr == NULL ) || ( ret_object == NULL ) )
  {
    return VS_EBADPARAM;
  }

  if ( ( handle & objmgr->total_num ) == 0 )
  { /* The client supplied an inactive handle. */
    return VS_EHANDLE;
  }

  obj = &objmgr->table[ handle & objmgr->index_mask ];

  /* objmgr->lock_fn( ); */
    /* The caller should hold the lock to guaranty safety in concurrent
     * systems. The caller doesn't need to use any locks when system is a
     * single writer and multiple readers setup.
     */

  if ( obj->handle == handle )
  {
    *ret_object = obj;
  }
  else
  {
    rc = VS_EHANDLE;
  }

  /* objmgr->unlock_fn( ); */

  return rc;
}

int32_t vs_objmgr_free_object (
  vs_objmgr_t* objmgr,
  uint32_t handle
)
{
  int32_t rc = VS_EOK;
  vs_objmgr_object_t* obj;

  if ( objmgr == NULL )
  {
    return VS_EBADPARAM;
  }

  if ( handle == 0 )
  {
    return VS_EHANDLE;
  }

  obj = &objmgr->table[ handle & objmgr->index_mask ];

  objmgr->lock_fn( );

  if ( obj->handle == handle )
  {
    obj->handle += objmgr->total_num;
      /* Increment the version and clear the visible flag in one-shot. */
    obj->handle &= objmgr->overflow_mask;
      /* Perform manual wrap-around for handles less than 32 bits. */
    obj->type = 0;
    obj->ptr = NULL;
    ( void ) gpr_list_add_tail( &objmgr->free_q, &obj->link );
  }
  else
  {
    rc = VS_EHANDLE;
  }

  objmgr->unlock_fn( );

  return rc;
}

