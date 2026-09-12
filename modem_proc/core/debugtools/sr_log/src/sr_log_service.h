#ifndef SR_LOG_SERVICE_H
#define SR_LOG_SERVICE_H
/*
#============================================================================
#  Name:
#    sr_log_service.h 
#
#  Description:
#    Header file for Service Registry logger feature
#
# Copyright (c) 2017 - 2018 by Qualcomm Technologies, Inc.  All Rights Reserved.
#============================================================================
*/

#if defined(__cplusplus)
extern "C"
{
#endif

#include "string.h"
#include "servreg_common.h"
#include "servreg_locator.h"
#include "servreg_monitor_qurt.h"
#include "tms_servreg.h"
#include "sr_log_trace.h"
#include "err.h"
#include "tms_dll_api.h"
#include "tms_utils_msg.h"
#include "tms_utils.h"
#include "qurt.h"
#include "rcinit.h"

#define TMS_SERVREG_SERVICE_NAME       "tms/servreg"
#define SR_LOG_TASK_NAME               "sr_log_task"

#define SR_LOG_MAX_CLIENTS             28 //32 - 4 general purpose
#define SR_LOG_MAX_SERVICES            SR_LOG_MAX_CLIENTS
#define SR_LOG_STATE_STR_LEN           8

#define SR_LOG_START_LOGGER_MASK       0x1
#define SR_LOG_ADD_CLIENT_SET_MASK     0x2
#define SR_LOG_REMOVE_CLIENT_SET_MASK  0x4
#define SR_LOG_STOP_LOGGER_MASK        0x8
#define SR_LOG_STATE_CHANGE_MASK_SET   0xFFFFFFF0

#define SR_LOG_INIT_STATE_MASK         0x10
#define SR_LOG_MAX_STATE_MASK          0x80000000

typedef enum
{
  SR_LOGGER_DOWN          = 0x0,            /* Logger not running*/
  SR_LOGGER_UP            = 0x1,            /* Logger running */
  SR_LOGGER_MAX_LIMIT_HIT = 0x2,            /* Logger hit max limit */
}SR_LOGGER_STATE;

struct sr_log_client_s
{
  SERVREG_MON_HANDLE     mon_handle;        /* monitor handle for current client */
  SERVREG_SERVICE_STATE  curr_state;        /* state for current client */
  uint32_t               state_mask;        /* state mask for current client */
  
  struct sr_log_client_s* next;
};
typedef struct sr_log_client_s sr_log_client_t, *sr_log_client_p;

struct sr_log_clientset_s
{
  qurt_anysignal_t* signal_ptr;             /* Pointer to client_signals */
  sr_log_client_p list_head;                /* Linked list head for its clients */
  uint32_t curr_state_mask;                 /* Current state change mask */
};
typedef struct sr_log_clientset_s sr_log_clientset_t , *sr_log_clientset_p;

struct sr_log_client_internal_s
{
  qurt_anysignal_t* client_signal[SR_LOG_MAX_TASKS];  /* Task signals */
  void* stack_ptr[SR_LOG_MAX_TASKS];                  /* Stack pointers for tasks */ 
  qurt_thread_t tid[SR_LOG_MAX_TASKS];                /* Thread IDs for tasks */
  uint32_t task_count;                                 /* Mapped to set_num for tasks (starts from 0) */
};
typedef struct sr_log_client_internal_s sr_log_client_internal_t;

struct sr_log_internal_s
{
  qurt_anysignal_t signal;                             /* QXDM and first set SR log signal */
  uint32_t domain_count;                               /* Number of other domains with tms/servreg FW */
  SERVREG_DL_HANDLE dlhandle;                          /* Domain list handle for tms/servreg FW */
  SR_LOGGER_STATE state;                               /* UP / DOWN / MAX_LIMIT */
  sr_log_clientset_t set;                              /* First set of listeners and QXDM commands */
};
typedef struct sr_log_internal_s sr_log_internal_t;

/*** PRIVATE API: Can be exposed if needed ***/

/** =====================================================================
 * Function:
 *     sr_log_req_start
 *
 * Description:
 *     Signal task to request starting of the SR logger task
 *
 * Parameters:
 *     None
 *
 * Returns:
 *     None
 * =====================================================================  */
void sr_log_req_start(void);

/** =====================================================================
 * Function:
 *     sr_log_req_reg_clients
 *
 * Description:
 *     Signal task to request adding a listener task 
 *
 * Parameters:
 *     None
 *
 * Returns:
 *     None
 * =====================================================================  */
void sr_log_req_reg_clients(void);

/** =====================================================================
 * Function:
 *     sr_log_req_dereg_clients
 *
 * Description:
 *     Signal task to request removing a listener task 
 *
 * Parameters:
 *     None
 *
 * Returns:
 *     None
 * =====================================================================  */
void sr_log_req_dereg_clients(void);

/** =====================================================================
 * Function:
 *     sr_log_req_stop
 *
 * Description:
 *     Signal task to request stopping of the SR logger task
 *
 * Parameters:
 *     None
 *
 * Returns:
 *     None
 * =====================================================================  */
void sr_log_req_stop(void);


#if defined(__cplusplus)
}
#endif

#endif
