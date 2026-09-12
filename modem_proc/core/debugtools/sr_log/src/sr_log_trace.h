#ifndef SR_LOG_TRACE_H
#define SR_LOG_TRACE_H
/*
#============================================================================
#  Name:
#    sr_log_trace.h 
#
#  Description:
#     Common header file for Service Registry logger trace feature
#
# Copyright (c) 2017-18 by Qualcomm Technologies, Inc.  All Rights Reserved.
#============================================================================
*/

#if defined(__cplusplus)
extern "C"
{
#endif

#ifdef SERVREG_LOG_TRACE_ENABLE

#include "stdlib.h"
#include "comdef.h"
#include "tms_utils.h"

/* Type of events to log */
typedef enum
{
  SRT_LOG_NOEVENT,            // no data
  SRT_LOG_SUCCESS,            // no data
  SRT_LOG_FAIL,               // no data
  SRT_LOG_INIT,               // no data
  SRT_LOG_UNINIT,             // no data
  SRT_LOG_GET_DMN_ENTRY_FAIL, // no data
  SRT_LOG_NULL_MON_HANDLE,    // no data
  SRT_LOG_END_OF_LIST,        // no data
  SRT_LOG_CHECK_STATE_MASK,   // no data
  SRT_LOG_STATE_CHANGE_ACKD,  // no data
  SRT_LOG_START_REQ,          // no data
  SRT_LOG_ADD_CLIENT_SET_REQ, // no data
  SRT_LOG_REM_CLIENT_SET_REQ, // no data
  SRT_LOG_STOP_REQ,           // no data
  SRT_LOG_MAX_SET_LIMIT_HIT,  // [task_count, 0]
  SRT_LOG_STATE_CHANGE,       // [q_mask, qurt_thread_get_id()]
  SRT_LOG_RESULT,             // [return value, 0]
  SRT_LOG_CLIENT_ADD,         // [servreg_mon_node_p, SERVREG_SERVICE_STATE] 
  SRT_LOG_CLIENT_ADD_FAIL,    // [servreg_mon_node_p, 0] 
  SRT_LOG_CLIENT_ADD_MASK,    // [mask, 0]
  SRT_LOG_CLIENT_REM,         // [servreg_mon_node_p, 0] 
  SRT_LOG_NEW_STATE,          // [servreg_mon_node_p, SERVREG_SERVICE_STATE] 
  SRT_LOG_ACK_SENT,           // [servreg_mon_node_p, transaction id]
  SRT_LOG_ACK_COUNT,          // [uint32_t, qurt_thread_get_id()]
  SRT_LOG_SET_ADDED,          // [set number, 0]
  SRT_LOG_SET_REMOVED,        // [set number, 0]
  SRT_LOG_TIMESTAMP,          // no data
  SRT_LOG_ERROR,              // no data
  SRT_LOG_WAIT_EXIT           // [q_mask, qurt_thread_get_id()]
} srt_log_event_type;

/* A single event trace */
typedef struct
{
  srt_log_event_type  event;
  void* data0;
  void* data1;
} srt_log_data_type;


/* Structure of all timer log events */
#define SRT_LOG_LENGTH  200
typedef struct
{
  uint32_t              index;
  srt_log_data_type events[SRT_LOG_LENGTH];
} srt_log_type;

void srt_log_event(srt_log_event_type  event, void** data);

/* Macros to perform logging */
#define SERVREG_LOG_TRACE_ERR(srt0)              \
do {                                         \
     void *srt[2] = {(void*)srt0, NULL};     \
     srt_log_event(SRT_LOG_ERROR, srt);              \
} while(0)
 
#define SERVREG_LOG_TRACE_0(event)               \
do {                                         \
     srt_log_event(SRT_LOG_##event, NULL);           \
} while(0)

#define SERVREG_LOG_TRACE_2(event, srt0, srt1)          \
do {                                                \
  void *srt[2] = {(void*)srt0, (void*)srt1};        \
  srt_log_event(SRT_LOG_##event, srt);                      \
} while(0)

#define SERVREG_LOG_TRACE_TIME()                        \
do {                                                \
  uint64 timestamp64;                               \
  uint32 timestamp32 = timestamp64 & 0xFFFFFFFF;    \
  void *srt[2] = {(void*)timestamp32, NULL};        \
  tms_get_timetick(&timestamp64);                   \
  srt_log_event(SRT_LOG_TIMESTAMP, srt);                \
} while(0)

#else //!SERVREG_LOG_TRACE_ENABLE

#define SERVREG_LOG_TRACE_ERR(srt0) do{}while(0)
#define SERVREG_LOG_TRACE_TIME() do{}while(0)  
#define SERVREG_LOG_TRACE_0(event) do{}while(0)
#define SERVREG_LOG_TRACE_2(event, srt0, srt1) do{}while(0)

#endif //SERVREG_LOG_TRACE_ENABLE

#if defined(__cplusplus)
}
#endif

#endif
