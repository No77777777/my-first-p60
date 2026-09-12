/** vi: tw=128 ts=3 sw=3 et :
@file rcinit_init.c
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
=============================================================================*/
/*=============================================================================
Edit History
$Header: //components/rel/core.mpss/10.0/debugtools/rcinit/src/rcinit_init.c#2 $
$DateTime: 2020/04/26 23:35:58 $
$Change: 23583302 $
$Author: pwbldsvc $
===============================================================================*/

#define DLL_API_BUILD_RCINIT

#include "DALSysTypes.h"
#include "DALPropDef.h"

#include "err.h"
#include "stdint.h"
#include "stringl/stringl.h"

#include "tms_utils.h"
#include "rcinit_dal.h"
#include "tms_rcinit.h"

#if defined(RCINIT_BOOTTIME_PROFILING)
#if !defined(RCINIT_EXCLUDE_KERNEL_QURT)
#include "qurt_sclk.h"
#endif
#endif

#if !defined(RCINIT_EXCLUDE_TRACER_SWEVT)
#include "tracer.h"
#include "rcinit_tracer_swe.h"
#endif

#if !defined(RCINIT_EXCLUDE_MSG_SWEVT)
#include "tms_utils_msg.h"
#endif

#if !defined(RCINIT_EXCLUDE_ULOG_SWEVT)
#endif

#define BASE_ALIGN_8BYTE      0x00000007
#define BASE_ALIGN_16BYTE     0x0000000f
#define BASE_ALIGN_32BYTE     0x0000001f
#define BASE_ALIGN_1KBYTE     0x000003ff
#define BASE_ALIGN_4KBYTE     0x00000fff

#define stack_base_alignment  BASE_ALIGN_16BYTE
#define stack_base_pad(n)     (((unsigned long)(n)+stack_base_alignment)&~stack_base_alignment)

#if !defined(RCINIT_EXCLUDE_TRACER_SWEVT)
extern const tracer_event_id_t rcinit_swe_event_init[RCINIT_GROUP_MAX];          // internal
#endif

#if !defined(RCINIT_EXCLUDE_RCINIT_WORKER)

#define dwMaxNumEvents        2
#define RCINIT_WORKER_TASK_NAME         "rcinit_worker"
//#define RCINIT_WORKER_TRIGGER_MASK       0x1
//#define RCINIT_WORKLOOP_ACK_MASK         0x2

#if defined(RCINIT_BOOTTIME_PROFILING)
static volatile unsigned int initfn_time_log_idx;
#endif

/** QuRT TASK 
INTERNAL, Worker Creation
@param
None
@return
None

static void rcinit_worker(void * unused)
{
   unsigned int    sigs;

   for(;;)
   {
     // Wait for RCINIT_WORKER_TRIGGER_MASK  
     sigs = qurt_anysignal_wait(&rcinit_internal_static.worker_sync_sig,
                      RCINIT_WORKER_TRIGGER_MASK );
      
     if ( sigs & RCINIT_WORKER_TRIGGER_MASK )
     {
       qurt_anysignal_clear( &rcinit_internal_static.worker_sync_sig, RCINIT_WORKER_TRIGGER_MASK );

       // Process Init functions
       rcinit_info_t** rcinit_group_p = rcinit_internal->worker_argv;

       while (RCINIT_NULL != *rcinit_group_p)
       {
          const rcinit_info_t* rcinit_p = *rcinit_group_p;

          rcinit_internal_tls_set_specific(rcinit_internal_static.tls_key, (void*)rcinit_p);

          rcinit_internal_tls_set_specific(rcinit_internal_static.tls_key_autogen_struct, (void*)rcinit_internal);
          
          if (IS_INITFN(rcinit_p))
          {
             // STALLING HERE?

             // BLOCKING HERE OCCURS ONLY WHEN THE INITFN IS USING KERNEL BLOCKING
             // MECHANISMS. CHECK WITH THE TECH AREA OWNING THE CALLBACK.

             rcinit_internal->initfn_curr = (void (*)(void))(rcinit_p->entry);

#if !defined(RCINIT_EXCLUDE_TRACER_SWEVT)
             tracer_event_simple_vargs(RCINIT_SWE_INIT_FUNC_RN, 2, rcinit_internal->group_curr, rcinit_internal->initfn_curr);
#endif
#if !defined(RCINIT_EXCLUDE_MSG_SWEVT)
             TMS_STARTUP_MSG_SPRINTF_HIGH_3("Func %s # %lx enter grp %x", rcinit_p->name, rcinit_p->hash, rcinit_internal->group_curr);
#endif
#if !defined(RCINIT_EXCLUDE_ULOG_SWEVT)
#endif

             // Initialization functions are triggered for execution at this point. They
             // are considered callbacks, and executed within the context of the 'rcinit_worker'
             // task. The 'rcinit_worker' task by itself does not supply any heap management, so
             // heap allocations tracked to 'rcinit_worker' context are actually performed by
             // other tech area supplied callbacks.

             // TODO: save current nhlos context name. set nhlos context name to rcinit_p->name. supply better runtime
             // tracking instruments based on context name. This might require a temporary context be established and
             // reclaimed.

             rcinit_internal->initfn_curr();

             // TODO: restore nhlos context name.

#if !defined(RCINIT_EXCLUDE_TRACER_SWEVT)
                 tracer_event_simple_vargs(RCINIT_SWE_INIT_FUNC_XT, 2, rcinit_internal->group_curr, rcinit_internal->initfn_curr);
#endif
#if !defined(RCINIT_EXCLUDE_MSG_SWEVT)
                 TMS_STARTUP_MSG_HIGH_2("Func # %x exit grp %x", rcinit_p->hash, rcinit_internal->group_curr);
#endif
#if !defined(RCINIT_EXCLUDE_ULOG_SWEVT)
#endif

             rcinit_internal->initfn_curr = (void (*)(void))RCINIT_NULL;
          }

          rcinit_group_p++;
       }
       
       // Init functions processing ended
       // Signal workloop using RCINIT_WORKLOOP_ACK_MASK
       qurt_anysignal_set( &rcinit_internal_static.workloop_ack_sig, RCINIT_WORKLOOP_ACK_MASK );
     }
   }
}
*/

/**
INTERNAL, DAL WorkLoop Creation
@param
None
@return
None
*/
static DALResult rcinit_worker(DALSYSEventHandle hUnused, void* tid)
{
   rcinit_info_t** rcinit_group_p = rcinit_internal->worker_argv;

   while (RCINIT_NULL != *rcinit_group_p)
   {
      const rcinit_info_t* rcinit_p = *rcinit_group_p;

      rcinit_internal_tls_set_specific(rcinit_internal_static.tls_key, (void*)rcinit_p);

      rcinit_internal_tls_set_specific(rcinit_internal_static.tls_key_autogen_struct, (void*)rcinit_internal);
      
      if (IS_INITFN(rcinit_p))
      {
         // STALLING HERE?

         // BLOCKING HERE OCCURS ONLY WHEN THE INITFN IS USING KERNEL BLOCKING
         // MECHANISMS. CHECK WITH THE TECH AREA OWNING THE CALLBACK.

         rcinit_internal->initfn_curr = (void (*)(void))(rcinit_p->entry);

#if !defined(RCINIT_EXCLUDE_TRACER_SWEVT)
         tracer_event_simple_vargs(RCINIT_SWE_INIT_FUNC_RN, 2, rcinit_internal->group_curr, rcinit_internal->initfn_curr);
#endif
#if !defined(RCINIT_EXCLUDE_MSG_SWEVT)
         TMS_STARTUP_MSG_SPRINTF_HIGH_3("Func %s # %lx enter grp %x", rcinit_p->name, rcinit_p->hash, rcinit_internal->group_curr);
#endif
#if !defined(RCINIT_EXCLUDE_ULOG_SWEVT)
#endif

         // Initialization functions are triggered for execution at this point. They
         // are considered callbacks, and executed within the context of the 'rcinit_worker'
         // task. The 'rcinit_worker' task by itself does not supply any heap management, so
         // heap allocations tracked to 'rcinit_worker' context are actually performed by
         // other tech area supplied callbacks.

         // TODO: save current nhlos context name. set nhlos context name to rcinit_p->name. supply better runtime
         // tracking instruments based on context name. This might require a temporary context be established and
         // reclaimed.

#if defined(RCINIT_BOOTTIME_PROFILING)
         rcinit_internal->initfn_time_log[initfn_time_log_idx].group = rcinit_p->group;
         rcinit_internal->initfn_time_log[initfn_time_log_idx].name = rcinit_p->name;
#if !defined(RCINIT_EXCLUDE_KERNEL_QURT)
         rcinit_internal->initfn_time_log[initfn_time_log_idx].start_time = qurt_sysclock_get_hw_ticks_32();
#endif
#endif

         rcinit_internal->initfn_curr();

#if defined(RCINIT_BOOTTIME_PROFILING)
#if !defined(RCINIT_EXCLUDE_KERNEL_QURT)
         rcinit_internal->initfn_time_log[initfn_time_log_idx].end_time = qurt_sysclock_get_hw_ticks_32();
#endif
         initfn_time_log_idx++; 
#endif

         // TODO: restore nhlos context name.

#if !defined(RCINIT_EXCLUDE_TRACER_SWEVT)
         tracer_event_simple_vargs(RCINIT_SWE_INIT_FUNC_XT, 2, rcinit_internal->group_curr, rcinit_internal->initfn_curr);
#endif
#if !defined(RCINIT_EXCLUDE_MSG_SWEVT)
         TMS_STARTUP_MSG_HIGH_2("Func # %x exit grp %x", rcinit_p->hash, rcinit_internal->group_curr);
#endif
#if !defined(RCINIT_EXCLUDE_ULOG_SWEVT)
#endif

         rcinit_internal->initfn_curr = (void (*)(void))RCINIT_NULL;
      }

      rcinit_group_p++;
   }

   if (DAL_SUCCESS != DALSYS_EventCtrl(rcinit_internal_static.hEventWorkLoopAck, DALSYS_EVENT_CTRL_TRIGGER))
   {
      ERR_FATAL("worker ack", 0, 0, 0);
   }

   if (hUnused) {;}
   if (tid) {;}

   return DAL_SUCCESS;
}

/**
INTERNAL, QuRT Worker Creation
@param
None
@return
None

static void rcinit_worker_create(void)
{
   static boolean rcinit_worker_flag = FALSE;

   if ( rcinit_worker_flag == TRUE )
    return;

   rcinit_worker_flag = TRUE;
   qurt_anysignal_init(&rcinit_internal_static.worker_sync_sig);
   qurt_anysignal_init(&rcinit_internal_static.workloop_ack_sig);
   
   (void)tms_utils_launch_qurt_task(RCINIT_WORKER_TASK_NAME, TMS_TASK_STACK_6K, TMS_TASK_PRIO_RCINIT_WORKER, rcinit_worker, NULL, TRUE);
}
*/

/**
INTERNAL, DAL WorkLoop Creation
@param
None
@return
None
*/
static void rcinit_dal_loop_worker_create(void)
{
   static boolean rcinit_dal_workloop_flag = FALSE;

   if ( rcinit_dal_workloop_flag == TRUE )
    return;

   rcinit_dal_workloop_flag = TRUE;

   if (DAL_SUCCESS != DALSYS_EventCreate(DALSYS_EVENT_ATTR_WORKLOOP_EVENT, &rcinit_internal_static.hEventWorkLoop, NULL))
   {
      ERR_FATAL("worker event creation", 0, 0, 0);
   }

   if (DAL_SUCCESS != DALSYS_EventCreate(DALSYS_EVENT_ATTR_NORMAL, &rcinit_internal_static.hEventWorkLoopAck, NULL))
   {
      ERR_FATAL("worker create event initfn", 0, 0, 0);
   }

   if (0 == dwMaxNumEvents ||
       DAL_SUCCESS != DALSYS_RegisterWorkLoopEx(RCINIT_WORKER_TASK_NAME, TMS_TASK_STACK_6K, TMS_TASK_PRIO_RCINIT_WORKER, dwMaxNumEvents, &rcinit_internal_static.hWorkLoop, NULL))
   {
      ERR_FATAL("worker work loop registration", 0, 0, 0);
   }

   if (DAL_SUCCESS != DALSYS_AddEventToWorkLoop(rcinit_internal_static.hWorkLoop, rcinit_worker, RCINIT_NULL, rcinit_internal_static.hEventWorkLoop, NULL))
   {
      ERR_FATAL("worker work loop event addition", 0, 0, 0);
   }
}

#endif

/**
API, Service Handshake Init
@param
None
@return
None
*/
void rcinit_handshake_init(void)                                                 // preferred API
{
   rcinit_info_t* rcinit_p = (rcinit_info_t*)rcinit_internal_tls_get_specific(rcinit_internal_static.tls_key);

   if (RCINIT_NULL == rcinit_p)
   {
      unsigned long task_hash;
      char task_name[RCINIT_NAME_MAX];

      task_hash = rcinit_internal_task_name(task_name, sizeof(task_name));

#if !defined(RCINIT_EXCLUDE_TRACER_SWEVT)
#endif
#if !defined(RCINIT_EXCLUDE_MSG_SWEVT)
      TMS_STARTUP_MSG_SPRINTF_ERROR_2("TASK %s # %lx NOT IN RCINIT FRAMEWORK MAY NOT HANDSHAKE GRP", task_name, task_hash);
#endif
#if !defined(RCINIT_EXCLUDE_ULOG_SWEVT)
#endif
   }

   else
   {
      if ( rcinit_p->type == RCINIT_TASK_INITFN )
      {
        /* bail out */
      }
      // edge case group 0, initfn starting a task, must call rcinit_handshake
      else if (RCINIT_GROUP_0 == rcinit_internal->group_curr &&                       // processing group 0 (only)
          rcinit_process_group_init_fn == rcinit_internal->process_state)            // processing initfns (only)
      {
         rcinit_internal_hs_list_hs(rcinit_internal->hs_table, rcinit_internal->group_curr, rcinit_p->name);

#if !defined(RCINIT_EXCLUDE_TRACER_SWEVT)
         tracer_event_simple_vargs(RCINIT_SWE_INIT_TASK_HS, 2, rcinit_internal->group_curr, rcinit_p->hash); // HS accepted
#endif
#if !defined(RCINIT_EXCLUDE_MSG_SWEVT)
         TMS_STARTUP_MSG_HIGH_2("Handshake # %lx grp %x", rcinit_p->hash, rcinit_internal->group_curr);
#endif
#if !defined(RCINIT_EXCLUDE_ULOG_SWEVT)
#endif

         if (DAL_SUCCESS != DALSYS_EventCtrl(rcinit_internal->hEventInitFnSpawn, DALSYS_EVENT_CTRL_TRIGGER))
         {
            ERR_FATAL("worker event initfn trigger", 0, 0, 0);
         }
      }

      // all other use case collect and wait for the defineack

      else
      {
         RCEVT_THRESHOLD count = rcevt_getcount_handle(rcinit_internal_static.defineack) + 1;

         rcinit_internal_hs_list_hs(rcinit_internal->hs_table, rcinit_internal->group_curr, rcinit_p->name);

#if !defined(RCINIT_EXCLUDE_TRACER_SWEVT)
         tracer_event_simple_vargs(RCINIT_SWE_INIT_TASK_HS, 2, rcinit_internal->group_curr, rcinit_p->hash); // HS accepted
#endif
#if !defined(RCINIT_EXCLUDE_MSG_SWEVT)
         TMS_STARTUP_MSG_HIGH_2("Handshake # %lx grp %x", rcinit_p->hash, rcinit_internal->group_curr);
#endif
#if !defined(RCINIT_EXCLUDE_ULOG_SWEVT)
#endif

         if (rcevt_signal_handle(rcinit_internal_static.define)) {;}                               // signals rcinit_task this task init complete

         if (rcevt_wait_count_handle(rcinit_internal_static.defineack, count)) {;}
      }
   }
}

/**
API, Service Handshake Init
@param
None
@return
None
*/
DLL_API_GLOBAL void rcinit_handshake_startup(void)
{
   rcinit_handshake_init();                                                      // use preferred API
}

/**
API, Service Spawn Task Pre RCINIT_GROUP_0, Specialized Use Case

required wrapper for an init function to spawn a task (very specialized edge
case found during sequence group 0, this should be the exception)

Requires existing RCINIT task database information where the task entry
point is NULL. The static data is preallocated, and this call binds the
entry point to the context and starts it. Edge case; only allowed from
Group 0 initfn processing. all other use cases are not supported and
are considered a bug.

@param
None
@return
None
*/
void rcinit_initfn_spawn_task(RCINIT_INFO info_p, void* entry)
{
   boolean started = FALSE;
   rcinit_info_t* rcinit_p = (rcinit_info_t*)info_p;

   if (RCINIT_NULL != rcinit_p &&
       RCINIT_GROUP_0 == rcinit_lookup_group(rcinit_p->name) &&                  // must be in group 0 (only)
       RCINIT_GROUP_0 == rcinit_internal->group_curr &&                           // processing group 0 (only)
       rcinit_process_group_init_fn == rcinit_internal->process_state &&          // processing initfns (only)
       RCINIT_NULL == rcinit_p->entry &&                                         // must not have an entry (specialized)
       RCINIT_NULL != entry)                                                     // must have an entry point argument
   {
      // DAL Event Objects Are Protected on Context Allowed to Wait (Create/Wait/Destroy)
      if (DAL_SUCCESS != DALSYS_EventCreate(DALSYS_EVENT_ATTR_NORMAL, &rcinit_internal->hEventInitFnSpawn, NULL))
      {
         ERR_FATAL("initfnspawn event", 0, 0, 0);
      }

      if (RCINIT_NULL != rcinit_p->handle && RCINIT_ENTRY_NONE != rcinit_p->handle->entry)
      {

#if !defined(RCINIT_EXCLUDE_KERNEL_DAL)
         if (RCINIT_TASK_DALTASK == rcinit_p->type)
         {
            rcinit_internal_start_daltask(rcinit_p, entry);                      // launches task and fills in overhead

            started = TRUE;
         }
#endif

#if !defined(RCINIT_EXCLUDE_KERNEL_POSIX)
         if (RCINIT_TASK_POSIX == rcinit_p->type)
         {
            if (RCINIT_STACK_NULL == rcinit_p->handle->stack)
            {
               rcinit_p->handle->stack = (rcinit_stack_p)stack_base_pad(malloc(stack_base_pad(rcinit_p->stksz)));

               if (RCINIT_STACK_NULL == rcinit_p->handle->stack)
               {
                  ERR_FATAL("initfnspawn posix stack", 0, 0, 0);
               }
            }

            rcinit_internal_start_posix(rcinit_p, entry);                        // launches task and fills in overhead

            started = TRUE;
         }
#endif

#if !defined(RCINIT_EXCLUDE_KERNEL_QURT)
         if (RCINIT_TASK_QURTTASK == rcinit_p->type)
         {
            if (RCINIT_STACK_NULL == rcinit_p->handle->stack)
            {
               rcinit_p->handle->stack = (rcinit_stack_p)stack_base_pad(malloc(stack_base_pad(rcinit_p->stksz)));

               if (RCINIT_STACK_NULL == rcinit_p->handle->stack)
               {
                  ERR_FATAL("initfnspawn qurt stack", 0, 0, 0);
               }
            }

            rcinit_internal_start_qurttask(rcinit_p, entry);                     // launches task and fills in overhead

            started = TRUE;
         }
#endif

#if !defined(RCINIT_EXCLUDE_KERNEL_REX)
         if (RCINIT_TASK_REXTASK == rcinit_p->type || RCINIT_TASK_LEGACY == rcinit_p->type)
         {
            if (RCINIT_STACK_NULL == rcinit_p->handle->stack)
            {
               rcinit_p->handle->stack = (rcinit_stack_p)stack_base_pad(malloc(stack_base_pad(rcinit_p->stksz)));

               if (RCINIT_STACK_NULL == rcinit_p->handle->stack)
               {
                  ERR_FATAL("initfnspawn rex stack", 0, 0, 0);
               }
            }

            rcinit_internal_start_rextask(rcinit_p, entry);                      // launches task and fills in overhead

            started = TRUE;
         }
#endif

      }

      // STALLING HERE?

      // BLOCKING HERE OCCURS UNTIL THE SINGLE TASK STARTED PERFORMS ITS HANDSHAKE.
      // THIS IS THE NORMAL MECHANISM. CHECK WITH THE TECH AREA OWNING THE CONTEXT
      // WHEN BLOCKING DURATION BECOMES EXCESSIVE WAITING FOR THE HANDSHAKE.

      if (TRUE == started)
      {
         // DAL Event Objects Are Protected on Context Allowed to Wait (Create/Wait/Destroy)
         if (DAL_SUCCESS != DALSYS_EventWait(rcinit_internal->hEventInitFnSpawn))
         {
            ERR_FATAL("initfnspawn event", 0, 0, 0);
         }
      }
      // DAL Event Objects Are Protected on Context Allowed to Wait (Create/Wait/Destroy)
      if (DAL_SUCCESS != DALSYS_DestroyObject(rcinit_internal->hEventInitFnSpawn))
      {
         ERR_FATAL("initfnspawn event", 0, 0, 0);
      }
   }
   else
   {
      ERR_FATAL("client does not meet requirements", 0, 0, 0);
   }
}

/**
INTERNAL, Process Groups for Initialization

init function processing
task define and start signaling

@param
None
@return
None
*/
void rcinit_internal_process_groups(void)
{
   const rcinit_info_t* rcinit_p;

   rcinit_internal->policy_base = rcinit_internal->autogen_internal->group_map[rcinit_internal->policy_curr]; // initialize before a transition

   ////////////////////////////////////////
   // Process Groups
   ////////////////////////////////////////

   while (rcinit_internal->group_curr < RCINIT_GROUP_MAX)
   {
      unsigned long resource_size;

#if !defined(RCINIT_EXCLUDE_TRACER_SWEVT)
      tracer_event_simple(rcinit_swe_event_init[rcinit_internal->group_curr]);    // software event
#endif
#if !defined(RCINIT_EXCLUDE_MSG_SWEVT)
      TMS_STARTUP_MSG_HIGH_1("Init begins grp %x", rcinit_internal->group_curr);
#endif
#if !defined(RCINIT_EXCLUDE_ULOG_SWEVT)
#endif

      tms_get_timetick(&rcinit_internal->group_start_time[rcinit_internal->group_curr]);

      ////////////////////////////////////////
      // Policy Filter Application
      ////////////////////////////////////////

      rcinit_internal->process_state = rcinit_process_group_policy_fn;

      rcinit_internal->group_base = rcinit_internal->policy_base[rcinit_internal->group_curr]; // initialize before a transition

#if !defined(RCINIT_EXCLUDE_POLICY_EXTENSION)

      for (rcinit_internal->group_curr_idx = RCINIT_GROUP_0; RCINIT_NULL != rcinit_internal->group_base[rcinit_internal->group_curr_idx]; rcinit_internal->group_curr_idx++)
      {
         rcinit_p = rcinit_internal->group_base[rcinit_internal->group_curr_idx];

         if (RCINIT_NULL != rcinit_p && RCINIT_NULL == rcinit_p->handle &&
             RCINIT_STKSZ_ZERO == rcinit_p->stksz &&
             RCINIT_TASK_POLICYFN == rcinit_p->type &&
             RCINIT_NULL != rcinit_p->entry)
         {
            static boolean policy_oneshot = FALSE;

            if (FALSE == policy_oneshot)
            {
               RCINIT_NAME(*policyfn)(const RCINIT_NAME[]);
               RCINIT_NAME policy_name;
               RCINIT_POLICY policy;

               policyfn = (RCINIT_NAME(*)(const RCINIT_NAME[]))rcinit_p->entry;
               policy_name = policyfn(rcinit_internal->autogen_internal->policy_list);
               policy = rcinit_lookup_policy(policy_name);

               if (RCINIT_POLICY_NONE != policy)
               {
                  policy_oneshot = TRUE;                                         // only one policy transition is allowed

                  rcinit_internal->policy_curr = policy;

                  rcinit_internal->policy_base = rcinit_internal->autogen_internal->group_map[rcinit_internal->policy_curr];
               }
            }

            break;
         }
      }

      rcinit_internal->group_base = rcinit_internal->policy_base[rcinit_internal->group_curr]; // after transition

#endif

      ////////////////////////////////////////
      // Static Resource Sizing
      ////////////////////////////////////////

      rcinit_internal->process_state = rcinit_process_group_sizing;

      resource_size = 0;                                                         // initial group resource allocation (scale is rcinit_stack_t)

      for (rcinit_internal->group_curr_idx = RCINIT_GROUP_0; RCINIT_NULL != rcinit_internal->group_base[rcinit_internal->group_curr_idx]; rcinit_internal->group_curr_idx++)
      {
         rcinit_p = rcinit_internal->group_base[rcinit_internal->group_curr_idx];

         if (RCINIT_NULL != rcinit_p && RCINIT_NULL != rcinit_p->handle &&       // must have a context handle
             RCINIT_STKSZ_ZERO != rcinit_p->stksz &&                             // must have a stack size
             RCINIT_STACK_NULL == rcinit_p->handle->stack &&                     // must not have an static stack
             RCINIT_STACK_TYPE_DYNAMIC == rcinit_p->stktype &&                   // must have a dynamic stack  
             RCINIT_NULL != rcinit_p->entry)                                     // must have an entry
         {
#if !defined(RCINIT_EXCLUDE_KERNEL_DAL)
            if (RCINIT_TASK_DALTASK == rcinit_p->type)
            {
               // NO STACK ALLOCATIONS; DAL WORKLOOPS INTERNALLY MANAGED
            }
#endif

#if !defined(RCINIT_EXCLUDE_KERNEL_POSIX)
            if (RCINIT_TASK_POSIX == rcinit_p->type)
            {
               resource_size += rcinit_p->stksz;                                 // tech area specified
               resource_size += sizeof(_rcxh_scope_t);                           // handlers overhead
               resource_size = stack_base_pad(resource_size);                    // pad alignment
            }
#endif

#if !defined(RCINIT_EXCLUDE_KERNEL_QURT)
            if (RCINIT_TASK_QURTTASK == rcinit_p->type)
            {
               resource_size += rcinit_p->stksz;                                 // tech area specified
               resource_size += sizeof(_rcxh_scope_t);                           // handlers overhead
               resource_size = stack_base_pad(resource_size);                    // pad alignment
            }
#endif

#if !defined(RCINIT_EXCLUDE_KERNEL_REX)
            if (RCINIT_TASK_REXTASK == rcinit_p->type || RCINIT_TASK_LEGACY == rcinit_p->type)
            {
               resource_size += rcinit_p->stksz;                                 // tech area specified
               resource_size += sizeof(_rcxh_scope_t);                           // handlers overhead
               resource_size = stack_base_pad(resource_size);                    // pad alignment
            }
#endif

         }
      }

      ////////////////////////////////////////
      // Static Resource Allocation
      ////////////////////////////////////////

      // One chunck per group to aviod excessive heap fragmentation. We cannot know what
      // the group configuration is until the policy function has been run, so it is not
      // possible to perform an allocation any larger than group based granularity. Less
      // fragmentation comes with a larger allocation.

      // The stack resource allocation is performed as heap is marked as NO EXECUTE space
      // with all products. We are not allowed to supply stacks in areas that potentially
      // will be in EXECUTE capable areas. Stack smashing by buffer overflow should not be
      // able to leave executable code in the stack.

      rcinit_internal->process_state = rcinit_process_group_allocation;

      if (0 != resource_size)                                                    // do not allocate a zero size resource
      {
         resource_size = stack_base_pad(resource_size);                          // pad alignment

         // TODO: save current nhlos context name. set nhlos context name to "STACKS_GROUPx". supply better runtime
         // tracking instruments based on context name. This might require a temporary context be established and
         // reclaimed.

         rcinit_internal->allocs[rcinit_internal->group_curr] = (unsigned char*)malloc(resource_size);

         // TODO: restore nhlos context name.

         if (RCINIT_NULL == rcinit_internal->allocs[rcinit_internal->group_curr])
         {
            ERR_FATAL("static resource allocation failure", 0, 0, 0);
         }

         // set for location of next allocation
         rcinit_internal->stacks[rcinit_internal->group_curr] = (rcinit_stack_p)stack_base_pad(rcinit_internal->allocs[rcinit_internal->group_curr]);

         rcinit_internal->stacks_size += resource_size; // measured in KB
      }

      ////////////////////////////////////////
      // Static Resource Assignments
      ////////////////////////////////////////

      // Stack resource allocations are handed out to the tasks; the pointer math is performed bother
      // over rcinit_stack_t sizes and unsigned char* so that at the end, an warning check can be computed
      // and insure that everything matches. Diagnostic instrumets output issues with computed sizes when
      // they are not matching.

      // The small pad location between stacks, when available, is initialized to a known canary. If
      // the small pad location between stacks, when available, has the canary overwritten, then that
      // will indicate a stack overflow from the higher address into the lower address. Triage via
      // Trace32 to identify this specific condition.

      rcinit_internal->process_state = rcinit_process_group_assignment;

      if (0 != resource_size)                                                    // do not allocate a zero size resource
      {
         for (rcinit_internal->group_curr_idx = RCINIT_GROUP_0; RCINIT_NULL != rcinit_internal->group_base[rcinit_internal->group_curr_idx]; rcinit_internal->group_curr_idx++)
         {
            rcinit_p = rcinit_internal->group_base[rcinit_internal->group_curr_idx];

            if (RCINIT_NULL != rcinit_p && RCINIT_NULL != rcinit_p->handle &&    // must have a context handle
                RCINIT_STKSZ_ZERO != rcinit_p->stksz &&                          // must have a stack size
                RCINIT_STACK_NULL == rcinit_p->handle->stack &&                  // must not have an static stack
                RCINIT_STACK_TYPE_DYNAMIC == rcinit_p->stktype &&                // must have a dynamic stack
                RCINIT_NULL != rcinit_p->entry)                                  // must have an entry
            {
               boolean stack_res_err = FALSE;
               
#if !defined(RCINIT_EXCLUDE_KERNEL_DAL)
               if (RCINIT_TASK_DALTASK == rcinit_p->type)                        // check and handout previously computed allocation
               {
                  // NO STACK ALLOCATIONS; DAL WORKLOOPS INTERNALLY MANAGED
               }
#endif

#if !defined(RCINIT_EXCLUDE_KERNEL_POSIX)
               if (RCINIT_TASK_POSIX == rcinit_p->type)                     // check and handout previously computed allocation
               {
                  rcinit_p->handle->stack = rcinit_internal->stacks[rcinit_internal->group_curr];

                  rcinit_internal->stacks[rcinit_internal->group_curr] += (rcinit_p->stksz / sizeof(rcinit_stack_t));
                  rcinit_internal->stacks[rcinit_internal->group_curr] += (sizeof(_rcxh_scope_t) / sizeof(rcinit_stack_t));

                  // set for location of next allocation

                  rcinit_internal->stacks[rcinit_internal->group_curr] = (rcinit_stack_p)stack_base_pad(rcinit_internal->stacks[rcinit_internal->group_curr]);

                  if ((unsigned char*)rcinit_internal->stacks[rcinit_internal->group_curr] > rcinit_internal->allocs[rcinit_internal->group_curr] + resource_size)
                  {
                    stack_res_err = TRUE;
                  }
               }
#endif

#if !defined(RCINIT_EXCLUDE_KERNEL_QURT)
               if (RCINIT_TASK_QURTTASK == rcinit_p->type)                  // check and handout previously computed allocation
               {
                  rcinit_p->handle->stack = rcinit_internal->stacks[rcinit_internal->group_curr];

                  rcinit_internal->stacks[rcinit_internal->group_curr] += (rcinit_p->stksz / sizeof(rcinit_stack_t));
                  rcinit_internal->stacks[rcinit_internal->group_curr] += (sizeof(_rcxh_scope_t) / sizeof(rcinit_stack_t));

                  // set for location of next allocation

                  rcinit_internal->stacks[rcinit_internal->group_curr] = (rcinit_stack_p)stack_base_pad(rcinit_internal->stacks[rcinit_internal->group_curr]);

                  if ((unsigned char*)rcinit_internal->stacks[rcinit_internal->group_curr] > rcinit_internal->allocs[rcinit_internal->group_curr] + resource_size)
                  {
                    stack_res_err = TRUE;
                  }
               }
#endif

#if !defined(RCINIT_EXCLUDE_KERNEL_REX)
               if (RCINIT_TASK_REXTASK == rcinit_p->type || RCINIT_TASK_LEGACY == rcinit_p->type) // check and handout previously computed allocation
               {
                  rcinit_p->handle->stack = rcinit_internal->stacks[rcinit_internal->group_curr];

                  rcinit_internal->stacks[rcinit_internal->group_curr] += (rcinit_p->stksz / sizeof(rcinit_stack_t));
                  rcinit_internal->stacks[rcinit_internal->group_curr] += (sizeof(_rcxh_scope_t) / sizeof(rcinit_stack_t));

                  // set for location of next allocation

                  rcinit_internal->stacks[rcinit_internal->group_curr] = (rcinit_stack_p)stack_base_pad(rcinit_internal->stacks[rcinit_internal->group_curr]);

                  if ((unsigned char*)rcinit_internal->stacks[rcinit_internal->group_curr] > rcinit_internal->allocs[rcinit_internal->group_curr] + resource_size)
                  {
                    stack_res_err = TRUE;
                  }
               }
#endif

                if (stack_res_err)
                {
#if !defined(RCINIT_EXCLUDE_TRACER_SWEVT)
#endif
#if !defined(RCINIT_EXCLUDE_MSG_SWEVT)
                   TMS_STARTUP_MSG_SPRINTF_ERROR_2("Stack pad check in grp %x with %s", rcinit_internal->group_curr, rcinit_p->name);
#endif
#if !defined(RCINIT_EXCLUDE_ULOG_SWEVT)
#endif                  
               }

            }
         }
      }

      ////////////////////////////////////////
      // Function Process
      ////////////////////////////////////////

      // Initialization functions are triggered for execution at this point. They
      // are considered callbacks, and executed within the context of the 'rcinit_worker'
      // task. The 'rcinit_worker' task by itself does not supply any heap management, so
      // heap allocations tracked to 'rcinit_worker' context are actually performed by
      // other tech area supplied callbacks.

      rcinit_internal->process_state = rcinit_process_group_init_fn;

      for (rcinit_internal->group_curr_idx = RCINIT_GROUP_0; RCINIT_NULL != rcinit_internal->group_base[rcinit_internal->group_curr_idx]; rcinit_internal->group_curr_idx++)
      {
         rcinit_p = rcinit_internal->group_base[rcinit_internal->group_curr_idx];

         if (IS_INITFN(rcinit_p))
         {

#if !defined(RCINIT_EXCLUDE_RCINIT_WORKER)

            // Worker should resume once rcinit_internal->worker_argv is updated
            rcinit_internal->worker_argv = rcinit_internal->group_base;

            // Signal worker from workloop (current task context) using RCINIT_WORKER_TRIGGER_MASK
            /* qurt_anysignal_set( &rcinit_internal_static.worker_sync_sig, RCINIT_WORKER_TRIGGER_MASK );

            // Block waiting for worker to signal back so we know it has finished processing init functions 
            qurt_anysignal_wait( &rcinit_internal_static.workloop_ack_sig, RCINIT_WORKLOOP_ACK_MASK );

            // Proceed
            qurt_anysignal_clear( &rcinit_internal_static.workloop_ack_sig, RCINIT_WORKLOOP_ACK_MASK ); */

            if (DAL_SUCCESS != DALSYS_EventCtrl(rcinit_internal_static.hEventWorkLoop, DALSYS_EVENT_CTRL_TRIGGER) ||
                DAL_SUCCESS != DALSYS_EventWait(rcinit_internal_static.hEventWorkLoopAck) ||
                DAL_SUCCESS != DALSYS_EventCtrl(rcinit_internal_static.hEventWorkLoopAck, DALSYS_EVENT_CTRL_RESET))
            {
               ERR_FATAL("rcinit_worker trigger", 0, 0, 0);
            }

#else

            const rcinit_info_t** rcinit_group_p = rcinit_internal->group_base;

            while (RCINIT_NULL != *rcinit_group_p)
            {
               const rcinit_info_t* rcinit_p = *rcinit_group_p;

               if (IS_INITFN(rcinit_p))
               {
                  // STALLING HERE?

                  // BLOCKING HERE OCCURS ONLY WHEN THE INITFN IS USING KERNEL BLOCKING
                  // MECHANISMS. CHECK WITH THE TECH AREA OWNING THE CALLBACK.

                  rcinit_internal->initfn_curr = (void (*)(void))(rcinit_p->entry);

#if !defined(RCINIT_EXCLUDE_TRACER_SWEVT)
                  tracer_event_simple_vargs(RCINIT_SWE_INIT_FUNC_RN, 2, rcinit_internal->group_curr, rcinit_internal->initfn_curr);
#endif
#if !defined(RCINIT_EXCLUDE_MSG_SWEVT)
                  TMS_STARTUP_MSG_SPRINTF_HIGH_3("Func %s # %lx enter grp %x", rcinit_p->name, rcinit_p->hash, rcinit_internal->group_curr);
#endif
#if !defined(RCINIT_EXCLUDE_ULOG_SWEVT)
#endif

                  // Initialization function call
#if defined(RCINIT_BOOTTIME_PROFILING)
         rcinit_internal->initfn_time_log[initfn_time_log_idx].group = rcinit_p->group;
         rcinit_internal->initfn_time_log[initfn_time_log_idx].name = rcinit_p->name;
#if !defined(RCINIT_EXCLUDE_KERNEL_QURT)
         rcinit_internal->initfn_time_log[initfn_time_log_idx].start_time = qurt_sysclock_get_hw_ticks_32();
#endif
#endif
         rcinit_internal->initfn_curr();

#if defined(RCINIT_BOOTTIME_PROFILING)
#if !defined(RCINIT_EXCLUDE_KERNEL_QURT)
         rcinit_internal->initfn_time_log[initfn_time_log_idx].end_time = qurt_sysclock_get_hw_ticks_32();
#endif
         initfn_time_log_idx++; 
         
#endif


                  // Initialization function return

#if !defined(RCINIT_EXCLUDE_TRACER_SWEVT)
                  tracer_event_simple_vargs(RCINIT_SWE_INIT_FUNC_XT, 2, rcinit_internal->group_curr, rcinit_internal->initfn_curr);
#endif
#if !defined(RCINIT_EXCLUDE_MSG_SWEVT)
                  TMS_STARTUP_MSG_SPRINTF_HIGH_3("Func %s # %lx exit grp %x", rcinit_p->name, rcinit_p->hash, rcinit_internal->group_curr);
#endif
#if !defined(RCINIT_EXCLUDE_ULOG_SWEVT)
#endif

                  rcinit_internal->initfn_curr = (void (*)(void))RCINIT_NULL;
               }

               rcinit_group_p++;
            }

#endif

            break;
         }
      }

      ////////////////////////////////////////
      // Task Process
      ////////////////////////////////////////

      // Tasks are now logically concurrently started. It is required for the task to
      // call the rcinit_handshake_startup function to indicate it is ready to block at
      // the logical barrier for all other tasks performing concurrent initialization.

      // It is highly discouraged for a task context to perform any type of blocking
      // during the initialization portion. Blocking calls can occur following the
      // required call to rcinit_handshake_startup. Any other use is not encouraged,
      // and ultimately could lead to race conditions or other latent bugs based on
      // startup timing that is not so deterministic.

      rcinit_internal->process_state = rcinit_process_group_init_task;

      rcinit_internal_static.def.curr = 0;                                              // currently defined
      rcinit_internal_static.def.prev = rcevt_getcount_handle(rcinit_internal_static.define);  // previously defined

      for (rcinit_internal->group_curr_idx = RCINIT_GROUP_0; RCINIT_NULL != rcinit_internal->group_base[rcinit_internal->group_curr_idx]; rcinit_internal->group_curr_idx++)
      {
         rcinit_p = rcinit_internal->group_base[rcinit_internal->group_curr_idx];

         if (IS_TASK(rcinit_p))
         {
            if (RCINIT_ENTRY_NONE != rcinit_p->handle->entry)
            {
               boolean task_err = FALSE;
#if !defined(RCINIT_EXCLUDE_KERNEL_DAL)
               if (RCINIT_TASK_DALTASK == rcinit_p->type)
               {
                  // NO STACK ALLOCATIONS; DAL WORKLOOPS INTERNALLY MANAGED

                  rcinit_internal_start_daltask(rcinit_p, rcinit_p->entry);      // launches task and fills in overhead

                  rcinit_internal_static.def.curr++;
               }
#endif

#if !defined(RCINIT_EXCLUDE_KERNEL_POSIX)
               if (RCINIT_TASK_POSIX == rcinit_p->type)
               {
                  if (RCINIT_STACK_NULL == rcinit_p->handle->stack && RCINIT_STACK_TYPE_DYNAMIC == rcinit_p->stktype)
                  {
                    task_err = TRUE;
                  }

                  else
                  {
                     rcinit_internal_start_posix(rcinit_p, rcinit_p->entry);     // launches task and fills in overhead

                     rcinit_internal_static.def.curr++;
                  }
               }
#endif

#if !defined(RCINIT_EXCLUDE_KERNEL_QURT)
               if (RCINIT_TASK_QURTTASK == rcinit_p->type)
               {
                  if (RCINIT_STACK_NULL == rcinit_p->handle->stack && RCINIT_STACK_TYPE_DYNAMIC == rcinit_p->stktype)
                  {
                    task_err = TRUE;
                  }

                  else
                  {
                     rcinit_internal_start_qurttask(rcinit_p, rcinit_p->entry);  // launches task and fills in overhead

                     rcinit_internal_static.def.curr++;
                  }
               }
#endif

#if !defined(RCINIT_EXCLUDE_KERNEL_REX)
               if (RCINIT_TASK_REXTASK == rcinit_p->type || RCINIT_TASK_LEGACY == rcinit_p->type)
               {
                  if (RCINIT_STACK_NULL == rcinit_p->handle->stack && RCINIT_STACK_TYPE_DYNAMIC == rcinit_p->stktype)
                  {
                    task_err = TRUE;
                  }

                  else
                  {
                     rcinit_internal_start_rextask(rcinit_p, rcinit_p->entry);   // launches task and fills in overhead

                     rcinit_internal_static.def.curr++;
                  }
               }
#endif

               if (task_err)
               {
#if !defined(RCINIT_EXCLUDE_TRACER_SWEVT)
#endif
#if !defined(RCINIT_EXCLUDE_MSG_SWEVT)
                     TMS_STARTUP_MSG_SPRINTF_ERROR_2("Stack checks in grp %x with %s", rcinit_internal->group_curr, rcinit_p->name);
#endif
#if !defined(RCINIT_EXCLUDE_ULOG_SWEVT)
#endif
               }
               
            }
         }
      }

      if (0 != rcinit_internal_static.def.curr)                                         // only wait when tasks were started
      {
         rcinit_internal->process_state = rcinit_process_group_init_blocking;

         // async signal arrival order; wait for all to complete that were started

         // STALLING HERE?

         // BLOCKING HERE OCCURS UNTIL ALL THE TASKS STARTED PERFORM THEIR HANDSHAKES.
         // THIS IS THE NORMAL MECHANISM. CHECK WITH THE TECH AREA OWNING A SPECIFIC
         // CONTEXT WHEN BLOCKING DURATION BECOMES EXCESSIVE WAITING FOR THE HANDSHAKE.
         // TASKS ARE LOGICALLY STARTED IN PARALLEL, AND ALLOWED TO BE SCHEDULED AND
         // RUN CONCURRENTLY.

         if (rcevt_wait_count_handle(rcinit_internal_static.define, rcinit_internal_static.def.prev + rcinit_internal_static.def.curr)) {;} // wait for all defined tasks handshake; this group

         // TASKS BLOCKING ON THIS HANDLE ARE WAITING CORRECTLY; ANY TASK BLOCKING
         // ELSEWHERE IS NOT PERFORMING STARTUP CORRECTLY.

         rcinit_internal->process_state = rcinit_process_group_run;

         if (rcevt_signal_handle(rcinit_internal_static.defineack)) {;}                         // issue start signal to all defined tasks; this group
      }

      else
      {
         // No blocking required, no tasks started
      }

      rcinit_internal_static.def.curr = 0;                                              // reset instruments
      rcinit_internal_static.def.prev = 0;                                              // reset instruments

      rcinit_internal->group_curr++;
   }

   ////////////////////////////////////////
   // Group Processing Complete
   ////////////////////////////////////////

   tms_get_timetick(&rcinit_internal->group_start_time[rcinit_internal->group_curr]);    //End time: rcinit_internal->group_curr == RCINIT_GROUP_MAX

   rcinit_internal->group_curr = RCINIT_GROUP_NONE;                               // current group
   rcinit_internal->process_state = rcinit_process_none;
}

/**
API, Service Initialization
@param
None
@return
None
*/
void rcinit_init(void)
{
   rcinit_internal_tls_set_specific(rcinit_internal_static.tls_key_autogen_struct, (void*)rcinit_internal);

   rcinit_internal->policy_curr = rcinit_lookup_policy(RCINIT_POLICY_NAME_DEFAULT);

   if (RCINIT_POLICY_NONE != rcinit_internal->policy_curr)
   {
      rcinit_internal_hs_list_init(rcinit_internal);

      // internal events

      rcinit_internal->policy_base = rcinit_internal->autogen_internal->group_map[rcinit_internal->policy_curr];

      rcinit_internal->group_curr = RCINIT_GROUP_0;

      rcinit_internal->group_base = rcinit_internal->policy_base[rcinit_internal->group_curr];

#if !defined(RCINIT_EXCLUDE_RCINIT_WORKER)
      //rcinit_worker_create();                                                    // internal QuRT worker thread
      rcinit_dal_loop_worker_create();                                           // internal worker thread
#endif

      rcinit_internal_process_groups();                                          // sequence groups
   }

   else
   {
      ERR_FATAL("default policy not available", 0, 0, 0);
   }
}


