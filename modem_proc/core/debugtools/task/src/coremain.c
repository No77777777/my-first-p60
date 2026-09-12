/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                        M A I N   M O D U L E

GENERAL DESCRIPTION
  This module contains the Main procedure for the AMSS Mobile software.

EXTERNALIZED FUNCTIONS
  None

INITIALIZATION AND SEQUENCING REQUIREMENTS
  None

Copyright (c) 2012 - 2018 by Qualcomm Technologies, Incorporated.  All Rights Reserved.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  $Header: //components/rel/core.mpss/10.0/debugtools/task/src/coremain.c#1 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------

===========================================================================*/

#include "err.h"
#include "rcinit.h"
#include "rcinit_workloop.h"
#include "rcevt_qurt.h"
#include "procinfo.h"

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

extern void bootstrap_rcinit_task(void); // forward reference
extern void bootstrap_exception_handler(void);
extern rcinit_autogen_internal_t rcinit_autogen_internal;
extern void tms_utils_msg_init( void );

rcinit_workloop_handle_t coremain_workloop_handle;


/**
API, coremain_main
@return None.
*/

#define SIG_ACK 0x1
DLL_API_GLOBAL void coremain_main(void)
{
   RCEVT_SIGEX_SIGQURT rcevt_sigex;
   qurt_anysignal_t sig;
    
   bootstrap_exception_handler();
	   
   qurt_anysignal_init(&sig);

   rcevt_sigex.signal = &sig;
   rcevt_sigex.mask = SIG_ACK;
   
   rcinit_onetime_init();
   
   (void)rcevt_register_sigex_name(RCINIT_RCEVT_INITGROUPS, RCEVT_SIGEX_TYPE_SIGQURT, &rcevt_sigex);

   rcinit_handle_init(&coremain_workloop_handle, RCINIT_RCECB_INITGROUPS, RCINIT_RCESN_INITGROUPS, RCINIT_RCEVT_INITGROUPS, INIT_PROCESS_TABLE, &rcinit_autogen_internal );
   
   rcinit_workloop_update_rcinit_internal(&coremain_workloop_handle);
	   
   bootstrap_rcinit_task(); // RCINIT Framework, Returns (following successful init completion)

   rcinit_process_autogen_table(&coremain_workloop_handle);

   qurt_anysignal_wait(&sig, SIG_ACK);

   rcinit_handle_deinit(&coremain_workloop_handle);
}

#if !defined(TASK_EXCLUDE_MAIN)
/**
API, main
@return None.
*/
int main(void)
{
   qurt_anysignal_t sig;

   qurt_anysignal_init(&sig);

   #ifdef WLAN_MAIN
   procinfo_qdi_client_init();
   procinfo_qdi_invoke_op( PROCINFO_OP_SET_NAME, PROCINFO_DOMAIN_NAME_TYPE, "msm/modem/wlan_pd", strlen("msm/modem/wlan_pd") );
   #endif
   
   coremain_main(); // when main is not delivered here, this is the entry to start cbsp

   qurt_anysignal_wait(&sig, SIG_ACK);
   
   /* Not expected to return */
   return (0);
}
#endif

