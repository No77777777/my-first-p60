/*
#============================================================================
#  Name:
#    sr_log_trace.c
#
#  Description:
#    Service Registry logger trace functions.
#
# Copyright (c) 2017 by Qualcomm Technologies, Inc.  All Rights Reserved.
#============================================================================
*/
/*=============================================================================
Edit History
$Header: //components/rel/core.mpss/10.0/debugtools/sr_log/src/sr_log_trace.c#1 $
$DateTime: 2019/04/24 00:03:26 $
$Change: 18983508 $
$Author: pwbldsvc $
===============================================================================*/

#include "sr_log_trace.h"

/* Declare the log structure */
static volatile srt_log_type servreg_log_trace;

/** =====================================================================
 * Function:
 *     srt_log_event
 *
 * Description:
 *     Add a log trace event entry
 *
 * Parameters:
 *     srt_log_event_type event
 *     void** data
 *
 * Returns:
 *     	none
 * =====================================================================  */
void srt_log_event(srt_log_event_type event, void** data)
{
   srt_log_data_type *trace = (srt_log_data_type*) &servreg_log_trace.events[servreg_log_trace.index];
   trace->event = event;

   //currently data is binary (0 or 2) if we need more variety add a param count to the macro
   if (data != NULL)
   {
      trace->data0 = data[0];
      trace->data1 = data[1];
   }
   else
   {
      trace->data0 = 0;
      trace->data1 = 0;
   }

  /* Go to the next index */
  servreg_log_trace.index++;
  if (servreg_log_trace.index >= SRT_LOG_LENGTH)
    servreg_log_trace.index = 0;
}
