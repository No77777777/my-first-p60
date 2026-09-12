/**
@file tms_utils_timetick.c
@brief This file contains the API for the TMS Utilities API 0.x
*/
/*=============================================================================
NOTE: The @brief description above does not appear in the PDF.
The tms_mainpage.dox file contains the group/module descriptions that
are displayed in the output PDF generated using Doxygen and LaTeX. To
edit or update any of the group/module text in the PDF, edit the
tms_mainpage.dox file or contact Tech Pubs.
===============================================================================*/
/*=============================================================================
Copyright (c) 2018-2019 QUALCOMM Technologies Incorporated.
All rights reserved.
QUALCOMM Confidential and Proprietary.
=============================================================================*/
/*=============================================================================
Edit History
$Header: //components/rel/core.mpss/10.0/debugtools/utils/src/tms_utils_task_qurt.c#2 $
$DateTime: 2019/11/07 04:24:43 $
$Change: 21281462 $
$Author: pwbldsvc $
===============================================================================*/

#include "qurt.h"
#include "tms_utils.h"
#include "tms_utils_msg.h"
#include "stringl/stringl.h"
#include "stdlib.h"
#include "err.h"

TMS_UTILS_STATUS tms_utils_launch_qurt_task(char * name, uint32 stack_size, uint32 priority,  void (*entrypoint) (void *), void *arg, int crash_on_failure )
{
   qurt_thread_t       tid;
   qurt_thread_attr_t  tattr;
   int * stackbase = NULL;
   
   stackbase = calloc(1, stack_size);

   if ( stackbase != NULL )
   {
     qurt_thread_attr_init (&tattr);
     qurt_thread_attr_set_stack_size (&tattr, stack_size);
     qurt_thread_attr_set_stack_addr (&tattr, (void*)stackbase );
     qurt_thread_attr_set_priority (&tattr, priority);
     qurt_thread_attr_set_name(&tattr, name);
     if ( QURT_EOK ==  qurt_thread_create(&tid, &tattr, entrypoint , arg))
     {
       TMS_STARTUP_MSG_SPRINTF_HIGH_2("Task %s started: Prio: 0x%lx", name, priority);
       return TMS_UTILS_STATUS_SUCCESS;       
     }
     else
       free(stackbase);
   }

   if ( crash_on_failure == TRUE )
   {
     ERR_FATAL_AUX_MSG("Failed to Launch task, details in aux_msg", name, strnlen(name, QURT_THREAD_ATTR_NAME_MAXLEN));
   }
   
   return TMS_UTILS_STATUS_ERROR;
}

