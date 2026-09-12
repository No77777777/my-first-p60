/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                        M A I N   M O D U L E

GENERAL DESCRIPTION
  This module contains the Main procedure for the AMSS Mobile software.

EXTERNALIZED FUNCTIONS
  None

INITIALIZATION AND SEQUENCING REQUIREMENTS
  None

Copyright (c) 2017-2018 by Qualcomm Technologies, Incorporated.  All Rights Reserved.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  $Header: //components/rel/core.mpss/10.0/debugtools/task/src/main.c#1 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------

===========================================================================*/

#include "rcinit.h"
#include "rcinit_workloop.h"
#include "rcevt_qurt.h"
#include "procinfo.h"

#ifdef TASK_MAIN_TESTPD
#include "test_init.h"
#endif

/*===========================================================================
FUNCTION MAIN

DESCRIPTION
  This procedure uses the rex_init procedure to bootstrap tasking
  This procedure context is then reused as the err_exception_handler

DEPENDENCIES
  None

RETURN VALUE
  None, this routine does not return

SIDE EFFECTS
  None
===========================================================================*/

extern void coremain_main(void);
extern void coreapi_report_init(void);

extern char *servreg_local_domain;

#define SIG_ACK 0x1

/**
API, main
@return None.
*/
int main(void)
{
   qurt_anysignal_t sig;

   qurt_anysignal_init(&sig);

   procinfo_qdi_client_init();
   procinfo_qdi_invoke_op( PROCINFO_OP_SET_NAME, PROCINFO_DOMAIN_NAME_TYPE, servreg_local_domain, strlen(servreg_local_domain) );
   
   coremain_main(); // when main is not delivered here, this is the entry to start cbsp

   coreapi_report_init();
   
   #ifdef TASK_MAIN_TESTPD
   test_init();     // invoke test_init() for Test PD
   #endif

   qurt_anysignal_wait(&sig, SIG_ACK);
   
   /* Not expected to return */
   return (0);
}

