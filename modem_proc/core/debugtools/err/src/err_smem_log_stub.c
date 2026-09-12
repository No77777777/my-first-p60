/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

         E R R O R     S M E M    L O G G I N G

GENERAL DESCRIPTION
  This module provides extended smem logging in case of fatal and 
  non-fatal errors.  This module is not a task, but rather a set of
  procedures which run in the context of the calling task.

Copyright (c) 2015-2018 by Qualcomm Technologies, Inc.  All Rights Reserved.

*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        Edit History

$Header: //components/rel/core.mpss/10.0/debugtools/err/src/err_smem_log_stub.c#1 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
04/01/15   abh     File created for error extended smem logging

===========================================================================*/


/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/

void err_smem_write_timestamp_string(const char * engg_timestampstring_string,
                                           const char * qcom_timestampstring_string)
{
	/* STUBBED */
}

void err_smem_log_init (void)
{
	/* STUBBED */
}

void err_smem_log_crash
(
  uint32 tid,
  char * pfr,
  qurt_sysevent_error_t * sys_err,
  boolean disable_further_writes
)
{
	/* STUBBED */
}
