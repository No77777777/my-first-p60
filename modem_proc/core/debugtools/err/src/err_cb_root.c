/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

         E R R O R     R E P O R T I N G    S E R V I C E S

GENERAL DESCRIPTION
  This module provides error reporting services for both fatal and
  non-fatal errors.  This module is not a task, but rather a set of
  callable procedures which run in the context of the calling task.

Copyright (c) 2018 by Qualcomm Technologies, Inc.  All Rights Reserved.

*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        Edit History

$Header: //components/rel/core.mpss/10.0/debugtools/err/src/err_cb_root.c#1 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
08/27/15   din     Adapted from err.c

===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/

#include "erri.h"
#include "tms_utils.h"

/*===========================================================================

                              Function definitions

===========================================================================*/

/**
 * Execute external callbacks section
 */
void err_execute_external_cb(err_cb_bucket_t bucket, uint32 data, uint32 crumb_bits)
{
  int i = 0;

  ERR_CRUMB_TRAIL_BMSK(crumb_bits);
  
  for (i=0; i<ERR_CB_MAX; i++)
  {
    if(coredump.err.err_cb[i].info.err_cb.cb == NULL
        || coredump.err.err_cb[i].info.bucket != bucket)
      continue;

    /* Continue, if this callback was already attempted */
    if ( coredump.err.err_cb[i].start_time != 0 )
      continue;

    tms_get_timetick(&(coredump.err.err_cb[i].start_time));
    ERR_FLUSH_ADDR(coredump.err.err_cb[i].start_time);

    coredump.err.err_cb_current = coredump.err.err_cb[i].info.err_cb.cb;
    ERR_FLUSH_ADDR(coredump.err.err_cb_current);

    dog_force_kick();
    if(ERRCB_TYPE_VOID == coredump.err.err_cb[i].info.type)
      coredump.err.err_cb[i].info.err_cb.cb();
    else
      coredump.err.err_cb[i].info.err_cb.cb_u32(data);
    dog_force_kick();

    if(ERRCB_BUCKET_LAST == coredump.err.err_cb[i].info.bucket)
      break; // Need to execute for loop only once (i.e. current one)
  }

  /* Set coredump.err.err_cb_current to NULL signifying completion to the current bucket callbacks */
  coredump.err.err_cb_current = NULL;
  ERR_FLUSH_ADDR(coredump.err.err_cb_current);
}