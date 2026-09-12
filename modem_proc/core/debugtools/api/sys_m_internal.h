#ifndef SYS_M_INTERNAL_H
#define SYS_M_INTERNAL_H
/*===========================================================================

           SYS_M_INTERNAL . H

DESCRIPTION

Copyright (c) 2015-2016 by Qualcomm Technologies, Inc.  All Rights Reserved.

===========================================================================
DESCRIPTION
High level system monitor
===========================================================================

                           EDIT HISTORY FOR FILE

$Header: //components/rel/core.mpss/10.0/debugtools/api/sys_m_internal.h#1 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------


===========================================================================*/

#include "sys_m_reset.h"

#define SYS_M_ACK                            "ssr:ack"
#define SYS_M_SHUTDOWN                       "ssr:shutdown"
#define SYS_M_SHUTDOWN_ACK                   SYS_M_ACK

//function for error handler to initialize SFR buffer
void *sys_m_init_sfr_buffer(void);

/**
  Initiates restart of the complete system using SSREQ QMI
  And notify about the indication responce status to 
  the user of this API via its callback function (ind_cb_fn)  if provided

  @return
   SSREQ_SUCCESS
   SSREQ_FAILURE
   SSREQ_IN_PROCESS

  @dependencies
  None.
*/
ssreq_status_t sys_m_initiate_restart_ssreq(ssreq_qmi_ind_cb_fn_type *ind_cb_fn);

#endif  /* SYS_M_INTERNAL_H */
