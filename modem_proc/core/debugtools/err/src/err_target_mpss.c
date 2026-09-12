/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

         E R R O R     R E P O R T I N G    S E R V I C E S

GENERAL DESCRIPTION
  This module provides mpss related error reporting services for both 
  fatal and non-fatal errors.  This module is not a task, but rather 
  a set of callable procedures which run in the context of the calling 
  task.

Copyright (c) 2015 - 2018 by Qualcomm Technologies, Inc.  All Rights Reserved.

*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        Edit History

$Header: //components/rel/core.mpss/10.0/debugtools/err/src/err_target_mpss.c#1 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
11/19/14   din     Adapted from err.c

===========================================================================*/


/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/

#include "erri.h"
#include "err_decompress.h"


/*===========================================================================

                              Function definitions

===========================================================================*/

/*===========================================================================

FUNCTION       ERR_TARGET_DECOMPRESS_AND_F3_TRACE

DESCRIPTION
  Decompresses the error fatal message and pushes it through F3 trace.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

void err_target_update_coredump_and_f3_trace( err_fatal_params_type * params )
{
  err_decompress_struct_type err_decomp_struct;
  
  /* Decompress err struct */
  err_decompress_msg_const(params->msg_const_ptr, &err_decomp_struct);

  /* kick the dog */
  dog_force_kick();

  err_fatal_jettison_core (err_decomp_struct.msg_blk.line,
                          err_decomp_struct.msg_blk.fname,
                          err_decomp_struct.msg_blk.fmt,
                          params->param1, 
                          params->param2, 
                          params->param3);

} /* err_target_update_coredump_and_f3_trace */

