#ifndef ERR_SMEM_LOG_H
#define ERR_SMEM_LOG_H

/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

         E R R O R     S M E M    L O G G I N G

GENERAL DESCRIPTION
  This module provides extended smem logging in case of fatal and 
  non-fatal errors.  This module is not a task, but rather a set of
  procedures which run in the context of the calling task.

Copyright (c) 2014 - 2018 by Qualcomm Technologies, Inc.  All Rights Reserved.

*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        Edit History

$Header: //components/rel/core.mpss/10.0/debugtools/err/src/err_smem_log.h#1 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
04/01/15   abh     File created for error extended smem logging

===========================================================================*/


/*===========================================================================
                     INCLUDE FILES FOR MODULE
===========================================================================*/


/*===========================================================================
			  FUNCTION PROTOTYPES
===========================================================================*/
#include "qurt.h"
#include "comdef.h"

/*===========================================================================

FUNCTION err_smem_log_crash

DESCRIPTION
  Logs error information in shared memory buffer to be read by HLOS kernel
   
DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS

===========================================================================*/

void err_smem_log_crash
(
  uint32 tid,
  char * pfr,
  qurt_sysevent_error_t * sys_err,
  boolean disable_further_writes,
  unsigned int stack_base,
  unsigned int stack_end
);


/*===========================================================================
FUNCTION ERR_SMEM_WRITE_TIMESTAMP_STRING

DESCRIPTION
  Writes the timestamp string from the Root PD.

DEPENDENCIES
  Needs to be called only from Root PD.

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void err_smem_write_timestamp_string(const char * engg_timestampstring_string,
                                     const char * qcom_timestampstring_string);

#endif /* ERR_SMEM_LOG_H */
