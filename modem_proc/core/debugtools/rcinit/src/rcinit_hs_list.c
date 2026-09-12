/** vi: tw=128 ts=3 sw=3 et :
@file rcinit_hs_list.c
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
$Header: //components/rel/core.mpss/10.0/debugtools/rcinit/src/rcinit_hs_list.c#2 $
$DateTime: 2020/04/26 23:35:58 $
$Change: 23583302 $
$Author: pwbldsvc $
===============================================================================*/

#define DLL_API_BUILD_RCINIT

#include "stringl/stringl.h"

#if !defined(RCINIT_EXCLUDE_KERNEL_DAL)
#include "rcinit_dal.h"
#endif
#if !defined(RCINIT_EXCLUDE_KERNEL_POSIX)
#include "rcinit_posix.h"
#endif
#if !defined(RCINIT_EXCLUDE_KERNEL_QURT)
#include "rcinit_qurt.h"
#endif
#if defined(RCINIT_BOOTTIME_PROFILING)
#include "qurt_sclk.h"
#endif
#if !defined(RCINIT_EXCLUDE_KERNEL_REX)
#include "rcinit_rex.h"
#endif

#include "tms_rcinit.h"

/////////////////////////////////////////////////////////////////////
// Localized Type Declarations
/////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////
// Localized Storage
/////////////////////////////////////////////////////////////////////

typedef struct rcinit_hs_list_internal_s
{
  #if !defined(RCINIT_EXCLUDE_KERNEL_DAL)
  DALSYSSyncHandle hs_mutex;
  #elif !defined(RCINIT_EXCLUDE_KERNEL_QURT)
  qurt_mutex_t hs_mutex;
  #elif !defined(RCINIT_EXCLUDE_KERNEL_REX)
  rex_crit_sect_type hs_mutex;
  #endif
  boolean hs_mutex_init;
} rcinit_hs_list_internal_t;

static rcinit_hs_list_internal_t rcinit_hs_list_internal;


/////////////////////////////////////////////////////////////////////
// Init Function Support
/////////////////////////////////////////////////////////////////////

unsigned long rcinit_internal_task_name(char* task_name, int buflen)
{
#if !defined(RCINIT_EXCLUDE_KERNEL_DAL)
   qurt_thread_get_name(task_name, buflen);
#elif !defined(RCINIT_EXCLUDE_KERNEL_QURT)
   qurt_thread_get_name(task_name, buflen);
#elif !defined(RCINIT_EXCLUDE_KERNEL_REX)
   rex_get_task_name(rex_self(), task_name, buflen, NULL);
#endif

   return rcinit_internal_nmehash(task_name);
}

void rcinit_hs_list_mutex_init_lock()
{
  if ( rcinit_hs_list_internal.hs_mutex_init == FALSE )
  {
     rcinit_hs_list_internal.hs_mutex_init = TRUE;
#if !defined(RCINIT_EXCLUDE_KERNEL_DAL)
     if (DAL_SUCCESS != DALSYS_SyncCreate(DALSYS_SYNC_ATTR_RESOURCE, &rcinit_hs_list_internal.hs_mutex, NULL)) { ; }
#elif !defined(RCINIT_EXCLUDE_KERNEL_QURT)
     qurt_mutex_init(&rcinit_hs_list_internal.hs_mutex);
#elif !defined(RCINIT_EXCLUDE_KERNEL_REX)
     rex_init_crit_sect(&rcinit_hs_list_internal.hs_mutex);
#endif
  }

#if !defined(RCINIT_EXCLUDE_KERNEL_DAL)
   DALSYS_SyncEnter(rcinit_hs_list_internal.hs_mutex);
#elif !defined(RCINIT_EXCLUDE_KERNEL_QURT)
   qurt_mutex_lock(&rcinit_hs_list_internal.hs_mutex);
#elif !defined(RCINIT_EXCLUDE_KERNEL_REX)
   rex_enter_crit_sect(&rcinit_hs_list_internal.hs_mutex);
#endif
}

void rcinit_hs_list_mutex_unlock()
{
#if !defined(RCINIT_EXCLUDE_KERNEL_DAL)
   DALSYS_SyncLeave(rcinit_hs_list_internal.hs_mutex);
#elif !defined(RCINIT_EXCLUDE_KERNEL_QURT)
   qurt_mutex_unlock(&rcinit_hs_list_internal.hs_mutex);
#elif !defined(RCINIT_EXCLUDE_KERNEL_REX)
   rex_leave_crit_sect(&rcinit_hs_list_internal.hs_mutex);
#endif
}

void rcinit_internal_hs_list_init(struct rcinit_internal_s * client_internal)
{
   unsigned long cnt = RCINIT_TASK_MAX;

   rcinit_hs_list_mutex_init_lock();

   for (cnt = 0; cnt < RCINIT_TASK_MAX; cnt++)
   {
      client_internal->hs_table[cnt].group = RCINIT_GROUP_NONE;
      strlcpy(client_internal->hs_table[cnt].task_name, "", sizeof(rcinit_internal->hs_table[cnt].task_name));
      client_internal->hs_table[cnt].hs = 0;
      client_internal->hs_table[cnt].hs_dup = 0;
      client_internal->hs_table[cnt].hs_oob = 0;
      client_internal->hs_table[cnt].tid = RCINIT_NULL;
#if defined(RCINIT_BOOTTIME_PROFILING)
      client_internal->hs_table[cnt].start_time = 0;
      client_internal->hs_table[cnt].end_time = 0;
#endif
   }

   rcinit_hs_list_mutex_unlock();
}

unsigned long rcinit_internal_hs_list_add(rcinit_internal_hs_p array, RCINIT_GROUP group, RCINIT_NAME task_name)
{
   void * tid;

   unsigned long cnt = RCINIT_TASK_MAX;

   rcinit_hs_list_mutex_init_lock();
  
   if (RCINIT_GROUP_7 >= group)
   {

#if !defined(RCINIT_EXCLUDE_KERNEL_DAL)
      tid = (void*)qurt_thread_get_id();
#elif !defined(RCINIT_EXCLUDE_KERNEL_QURT)
      tid = (void*)qurt_thread_get_id();
#elif !defined(RCINIT_EXCLUDE_KERNEL_REX)
      tid = (void*)rex_self();
#endif

      for (cnt = 0; cnt < RCINIT_TASK_MAX; cnt++)                                // find existing
      {
         if (tid == (void*)array[cnt].tid)
         {
            if (group == array[cnt].group)
            {
               break;
            }
         }
      }

      if (cnt < RCINIT_TASK_MAX)                                                 // update existing
      {
         // already set array[cnt].group = group;
         // already set strlcpy(array[cnt].task_name, task_name, sizeof(array[cnt].task_name));
         // already set array[cnt].tid = GET_SELF();
         array[cnt].hs = 0;
      }

      else
      {
         for (cnt = 0; cnt < RCINIT_TASK_MAX; cnt++)                             // find empty
         {
            if (array[cnt].group == RCINIT_GROUP_NONE)
            {
               break;
            }
         }

         if (cnt < RCINIT_TASK_MAX)                                              // update empty
         {
            array[cnt].group = group;
            strlcpy(array[cnt].task_name, task_name, sizeof(array[cnt].task_name));
            array[cnt].tid = tid;
            array[cnt].hs = 0;
#if defined(RCINIT_BOOTTIME_PROFILING)
#if !defined(RCINIT_EXCLUDE_KERNEL_QURT)
            array[cnt].start_time = qurt_sysclock_get_hw_ticks_32();
#endif
#endif
         }
         else
         {
            cnt = RCINIT_TASK_MAX;
         }
      }
   }
   rcinit_hs_list_mutex_unlock();

   return cnt;                                                                   // RCINIT_TASK_MAX == Not Present, !RCINIT_TASK_MAX = Present & Added
}

unsigned long rcinit_internal_hs_list_del(rcinit_internal_hs_p array, RCINIT_GROUP group, RCINIT_NAME task_name)
{
   unsigned long cnt = RCINIT_TASK_MAX;

   rcinit_hs_list_mutex_init_lock();

   if (RCINIT_GROUP_7 >= group)
   {
      for (cnt = 0; cnt < RCINIT_TASK_MAX; cnt++)                                // find existing
      {
#if !defined(RCINIT_EXCLUDE_KERNEL_DAL)
         void* tid = (void*)qurt_thread_get_id();
#elif !defined(RCINIT_EXCLUDE_KERNEL_QURT)
         void* tid = (void*)qurt_thread_get_id();
#elif !defined(RCINIT_EXCLUDE_KERNEL_REX)
         void* tid = (void*)rex_self();
#endif

         if (tid == (void*)array[cnt].tid)
         {
            if (group == array[cnt].group)
            {
               break;
            }
         }
      }

      if (cnt < RCINIT_TASK_MAX)                                                 // update existing
      {
         array[cnt].group = RCINIT_GROUP_NONE;
         strlcpy(array[cnt].task_name, "", sizeof(array[cnt].task_name));
         array[cnt].tid = RCINIT_NULL;
         array[cnt].hs = 0;
      }
   }

   rcinit_hs_list_mutex_unlock();

   return cnt;                                                                   // RCINIT_TASK_MAX == Not Present, !RCINIT_TASK_MAX = Present & Removed
}

unsigned long rcinit_internal_hs_list_query(rcinit_internal_hs_p array, RCINIT_GROUP group, RCINIT_NAME task_name)
{
   unsigned long cnt = RCINIT_TASK_MAX;

   rcinit_hs_list_mutex_init_lock();

   if (RCINIT_GROUP_7 >= group)
   {
      for (cnt = 0; cnt < RCINIT_TASK_MAX; cnt++)                                // find existing
      {
#if !defined(RCINIT_EXCLUDE_KERNEL_DAL)
         void* tid = (void*)qurt_thread_get_id();
#elif !defined(RCINIT_EXCLUDE_KERNEL_QURT)
         void* tid = (void*)qurt_thread_get_id();
#elif !defined(RCINIT_EXCLUDE_KERNEL_REX)
         void* tid = (void*)rex_self();
#endif

         if (tid == (void*)array[cnt].tid)
         {
            if (group == array[cnt].group)
            {
               break;
            }
         }
      }
   }

   rcinit_hs_list_mutex_unlock();
   return cnt;                                                                   // RCINIT_TASK_MAX == Not Present, !RCINIT_TASK_MAX = Present
}

unsigned long rcinit_internal_hs_list_query_hs(rcinit_internal_hs_p array, RCINIT_GROUP group, RCINIT_NAME task_name)
{
   unsigned long cnt = RCINIT_TASK_MAX;

   rcinit_hs_list_mutex_init_lock();

   if (RCINIT_GROUP_7 >= group)
   {
      for (cnt = 0; cnt < RCINIT_TASK_MAX; cnt++)                                // find existing
      {
#if !defined(RCINIT_EXCLUDE_KERNEL_DAL)
         void* tid = (void*)qurt_thread_get_id();
#elif !defined(RCINIT_EXCLUDE_KERNEL_QURT)
         void* tid = (void*)qurt_thread_get_id();
#elif !defined(RCINIT_EXCLUDE_KERNEL_REX)
         void* tid = (void*)rex_self();
#endif

         if (tid == (void*)array[cnt].tid)
         {
            if (group == array[cnt].group)
            {
               if (1 == array[cnt].hs)
               {
                  break;
               }
            }
         }
      }
   }

   rcinit_hs_list_mutex_unlock();

   return cnt;                                                                   // RCINIT_TASK_MAX == Not Present or No HS, !RCINIT_TASK_MAX = HS
}

unsigned long rcinit_internal_hs_list_hs(rcinit_internal_hs_p array, RCINIT_GROUP group, RCINIT_NAME task_name)
{
   unsigned long cnt = 0;
   unsigned long rc = RCINIT_TASK_MAX;

#if !defined(RCINIT_EXCLUDE_KERNEL_DAL)
   void* tid = (void*)qurt_thread_get_id();
#elif !defined(RCINIT_EXCLUDE_KERNEL_QURT)
   void* tid = (void*)qurt_thread_get_id();
#elif !defined(RCINIT_EXCLUDE_KERNEL_REX)
   void* tid = (void*)rex_self();
#endif

   rcinit_hs_list_mutex_init_lock();

   if (RCINIT_GROUP_7 >= group)
   {
      while ( cnt < RCINIT_TASK_MAX)                                // find existing
      {
         if (tid == (void*)array[cnt].tid)
         {
            if (group == array[cnt].group)                          // check matching group
            {
               if (0 == array[cnt].hs)                              // check first handshake
               {
                  // accept the handshake, it matches the group and is the first one
#if defined(RCINIT_BOOTTIME_PROFILING)
#if !defined(RCINIT_EXCLUDE_KERNEL_QURT)
                  array[cnt].end_time = qurt_sysclock_get_hw_ticks_32();
#endif
#endif
                  array[cnt].hs = 1;
                  rc = cnt;
               }
               else
               {
                  // statistics indicates this is duplicate; a task is not allowed to
                  // duplicate its handshake within the same group. this is not fatal,
                  // but it must not be counted in the total HS required for the group.
                  array[cnt].hs_dup++;
               }
               break; /*exit from the loop once we find right hs entry */
            }
            else
            {
               // statistics indicates this is out of band; a task that registers for multiple
               // groups will have the OOB counters incremented for any group not current, the
               // behavior is not a problem, it is not fatal, but it must not be counted in the
               // total HS required for the group.
               array[cnt].hs_oob++;
            }
         }
         cnt++;
      }
   }

   rcinit_hs_list_mutex_unlock();
   return rc;                                                       // RCINIT_TASK_MAX == Not Present or Spurious HS, !RCINIT_TASK_MAX = HS
}
