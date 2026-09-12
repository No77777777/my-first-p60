/*==============================================================================
@file  DALSYSWorkLoop_posix.c

DALSYS Work Loop implementation using POSIX thread

        Copyright © 2018 Qualcomm Technologies Incorporated.
        All Rights Reserved.
        Qualcomm Confidential and Proprietary
==============================================================================*/
#include "DALSys.h"
#include "DALStdDef.h"
#include "DALSysCmn.h"
#include "qurt.h"
#include "pthread.h"

extern void * _WorkLoopExecute(void *pArg);

DALResult 
DALSYS_WorkLoopThreadCreate(_DALSYSWorkLoopObj *pWorkLoopObj, 
                             char * pszname, 
                             unsigned int dwStackSize, 
                             unsigned int dwPriority )
{
   int status;
   pthread_attr_t attr;
   struct sched_param  param;
   
   pWorkLoopObj->hThread = (void *)~0;
   // set the attribute for workloop.  
   status = pthread_attr_init(&attr);
   if(0 != status)
   {
      goto thread_create_abort;
   }
   
   if(0 != pthread_attr_setstacksize(&attr, dwStackSize))
   {
      goto thread_create_abort;
   }
   if(0 != pthread_attr_setstackaddr(&attr, 
                     (unsigned char * )pWorkLoopObj->pStackBuffer))
   {
      goto thread_create_abort;
   }
   param.sched_priority = dwPriority;
   if(0 != pthread_attr_setschedparam(&attr, &param))
   {
      goto thread_create_abort;   
   }
   
   if(0 != pthread_attr_setthreadname(&attr, pszname))
   {
      goto thread_create_abort;   
   }

   //create posix thread
   if(0 == pthread_create((pthread_t *)&pWorkLoopObj->hThread,&attr, (void *)_WorkLoopExecute, (void *)pWorkLoopObj))
   {
      return DAL_SUCCESS;
   }

thread_create_abort:
   DALSYS_LOG_ERROR_EVENT(
      "Failed to create thread- pWorkLoopObj:0x%x pszname:%s", 
      pWorkLoopObj, pszname); 
   return DAL_ERROR;
}

void DALSYS_WorkLoopThreadExit(_DALSYSWorkLoopObj * pWorkLoopObj)
{
   // kill myself
   pWorkLoopObj->hThread = (void *)~0; // indicate this thread is terminated
   pthread_exit(0);
}

void DALSYS_WorkLoopThreadJoin(_DALSYSWorkLoopObj * pWorkLoopObj)
{
   if (!pthread_equal(pthread_self(), (pthread_t)pWorkLoopObj->hThread))
   {
      void *status;
      pthread_join((unsigned int)pWorkLoopObj->hThread, &status);
   }
}
