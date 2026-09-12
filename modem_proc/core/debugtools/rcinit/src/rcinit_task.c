/** vi: tw=128 ts=3 sw=3 et :
@file rcinit_dal_task.c
@brief This file contains the API for the Run Control Framework, API 3.0
*/
/*=============================================================================
NOTE: The @brief description above does not appear in the PDF.
The tms_mainpage.dox file contains the group/module descriptions that
are displayed in the output PDF generated using Doxygen and LaTeX. To
edit or update any of the group/module text in the PDF, edit the
tms_mainpage.dox file or contact Tech Pubs.
===============================================================================*/
/*=============================================================================
Copyright (c) 2015-2018,2020 Qualcomm Technologies, Inc.
All Rights Reserved.
Confidential and Proprietary - Qualcomm Technologies, Inc.
===============================================================================*/
/*=============================================================================
Edit History
$Header: //components/rel/core.mpss/10.0/debugtools/rcinit/src/rcinit_task.c#2 $
$DateTime: 2020/04/26 23:35:58 $
$Change: 23583302 $
$Author: pwbldsvc $
===============================================================================*/

#define DLL_API_BUILD_RCINIT

#include "err.h"
#include "rcinit.h"
#include "rcinit_workloop.h"

#if !defined(RCINIT_EXCLUDE_TRACER_SWEVT)
#include "tracer.h"
#include "rcinit_tracer_swe.h"
#endif

#if !defined(RCINIT_EXCLUDE_MSG_SWEVT)
#include "tms_utils_msg.h"
#endif

#if !defined(RCINIT_EXCLUDE_KERNEL_REX)
#include "rcinit_dal.h"
#include "rcinit_rex.h"
#elif !defined(RCINIT_EXCLUDE_KERNEL_DAL)
#include "rcinit_dal.h"
#else
#error NHLOS INTERFACE NOT DEFINED PROPERLY
#endif

#include "tms_rcinit.h"
#include "sys_m_internal.h"

#if defined(RCINIT_BOOTTIME_PROFILING)
#if !defined(RCINIT_EXCLUDE_KERNEL_QURT)
#include "qurt_sclk.h"
#endif
#endif

#define RCINIT_WORKLOOP_TASK_NAME "rcinit"

#if !defined(RCINIT_EXCLUDE_KERNEL_REX)
extern void rcinit_workloop_ctxt(unsigned long arg_p);
#elif !defined(RCINIT_EXCLUDE_KERNEL_DAL)
extern void rcinit_workloop_ctxt(void* arg_p);
#endif 
   
/*===========================================================================

 FUNCTION rcinit_task

 DESCRIPTION
 hlos task thread to process sequnece groups

 DEPENDENCIES
 none

 RETURN VALUE
 none

 SIDE EFFECTS
 none

 ===========================================================================*/

void rcinit_workloop_process_data( rcinit_workloop_handle_t * handle )
{
   /* Cache client_internal in rcinit_internal */
   rcinit_internal = handle->client_internal;
   
   if ( handle->method == INIT_PROCESS_TABLE )
   {
      rcinit_internal->process_state = rcinit_process_init;

#if defined(RCINIT_BOOTTIME_PROFILING)
#if !defined(RCINIT_EXCLUDE_KERNEL_QURT)
   rcinit_internal->start_time = qurt_sysclock_get_hw_ticks_32();
#else
   rcinit_internal->start_time = 0;
#endif
#endif

      // rcinit init
      rcinit_init();

#if !defined(RCINIT_EXCLUDE_TRACER_SWEVT)
      tracer_event_simple(RCINIT_SWE_INITGROUPS);
#endif
#if !defined(RCINIT_EXCLUDE_MSG_SWEVT)
      TMS_STARTUP_MSG_HIGH_1("Group initializations end for handle %x", handle->client_internal);
#endif

   }
   else
   {
      rcinit_internal->process_state = rcinit_process_blocking;
     
#if !defined(RCINIT_EXCLUDE_TRACER_SWEVT)
      tracer_event_simple(RCINIT_SWE_TERMGROUPS);
#endif
#if !defined(RCINIT_EXCLUDE_MSG_SWEVT)
      TMS_SHUTDOWN_MSG_HIGH("Group terminations begin");
#endif

      // rcinit termination
      rcinit_term();

      // termination
      rcinit_internal->process_state = rcinit_process_term;
   }

   rcinit_internal = NULL;

   if (rcecb_signal_handle(handle->rcecb_handle)) {;}
   if (rcesn_signal_handle(handle->rcesn_handle, 1)) {;}
   if (rcevt_signal_handle(handle->rcevt_handle)) {;}
}


#define dwMaxNumEvents        1

#if !defined(RCINIT_EXCLUDE_KERNEL_REX)

void bootstrap_rcinit_task(void)
{
   static rex_tcb_type tcb;                                                      // static allocation for boot strapping
   static rex_stack_word_type stack[TMS_TASK_STACK_6K / sizeof(rex_stack_word_type)];         // static allocation for boot strapping

   rcinit_internal_tls_create_key(&rcinit_internal_static.tls_key, RCINIT_NULL);

   rcinit_internal_tls_create_key(&rcinit_internal_static.tls_key_autogen_struct, RCINIT_NULL);

   rcinit_internal_tls_set_specific(rcinit_internal_static.tls_key_autogen_struct, (void*)rcinit_internal);

   rcinit_internal_static.tls_available = RCINIT_TASK_TLS_AVAILABLE;
   
   // Initialize NHLOS Layers Needed

   // Must initialize the kernel REX API Layer; its not done by kernel
   // even on PL where the interface is available, and we are already
   // multi-tasking at this point by the NHLOS.

#if !defined(RCINIT_EXCLUDE_KERNEL_REX_Q6)

   rex_init(REX_ANY_CPU_AFFINITY_MASK, NULL, 0, &tcb,
            stack, sizeof(stack) / sizeof(rex_stack_word_type), TMS_TASK_PRIO_RCINIT_WORKLOOP,
            rcinit_workloop_ctxt, 0);

#elif !defined(RCINIT_EXCLUDE_KERNEL_REX_ARM)

   rex_init(NULL, 0, &tcb,
            stack, sizeof(stack) / sizeof(rex_stack_word_type), TMS_TASK_PRIO_RCINIT_WORKLOOP,
            rcinit_workloop_ctxt, 0);

#endif

   // ALWAYS RETURNS
}

#elif !defined(RCINIT_EXCLUDE_KERNEL_DAL)

void bootstrap_rcinit_task(void)
{
   // Initialize NHLOS Layers Needed

   DALSYS_InitMod(NULL);                                                         // prerequsite dependency

   rcinit_internal_tls_create_key(&rcinit_internal_static.tls_key, RCINIT_NULL);

   rcinit_internal_tls_create_key(&rcinit_internal_static.tls_key_autogen_struct, RCINIT_NULL);

   rcinit_internal_tls_set_specific(rcinit_internal_static.tls_key_autogen_struct, (void*)rcinit_internal);

   rcinit_internal_static.tls_available = RCINIT_TASK_TLS_AVAILABLE;
   
   (void)tms_utils_launch_qurt_task(RCINIT_WORKLOOP_TASK_NAME, TMS_TASK_STACK_6K, TMS_TASK_PRIO_RCINIT_WORKLOOP, rcinit_workloop_ctxt, NULL, TRUE); 
  
   // ALWAYS RETURNS
}

#endif
