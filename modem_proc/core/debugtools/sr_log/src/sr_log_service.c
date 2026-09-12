/*
#============================================================================
#  Name:
#    sr_log_service.c
#
#  Description:
#    Service Registry logger file for root image. This module serves as a servreg
#    PDR/SSR notifications logger from other service registry users in the system.
#
# Copyright (c) 2017-2018 by Qualcomm Technologies, Inc.  All Rights Reserved.
#============================================================================
*/
/*=============================================================================
Edit History
$Header: //components/rel/core.mpss/10.0/debugtools/sr_log/src/sr_log_service.c#1 $
$DateTime: 2019/04/24 00:03:26 $
$Change: 18983508 $
$Author: pwbldsvc $
===============================================================================*/

/**
TODO: Add timer profiles
*/

#include "sr_log_service.h"

void sr_log_task(void * unused_param );

/* SR log internal */
sr_log_internal_t sr_log_internal;
sr_log_client_internal_t sr_log_client_internal;

/* extern functions */
#ifdef SERVREG_LOG_DIAG_ENABLE
extern void sr_log_diag_init(void);
#endif

/** =====================================================================
 * Function:
 *     sr_log_get_state_string
 *
 * Description:
 *     Gets a string equivalent for its state enum entry
 *
 * Parameters:
 *     state entry SERVREG_SERVICE_STATE, pointer to string of atleast 8 chars
 *
 * Returns:
 *     	none
 * =====================================================================  */
void sr_log_get_state_string(SERVREG_SERVICE_STATE state_int, char* state_str)
{
  if (state_int == SERVREG_SERVICE_STATE_DOWN)
  {
    strlcpy(state_str, "DOWN", 5);
  }
  else if (state_int == SERVREG_SERVICE_STATE_UP)
  {
    strlcpy(state_str, "UP", 3);
  }
  else if (state_int == SERVREG_SERVICE_STATE_UNINIT)
  {
    strlcpy(state_str, "UNINIT", 7);
  }
  else /* UNDEF */
  {
    strlcpy(state_str, "UNDEF", 6);
  }
}

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
void sr_log_req_start(void)
{
  qurt_anysignal_set(&sr_log_internal.signal, SR_LOG_START_LOGGER_MASK);
}

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
void sr_log_req_reg_clients(void)
{
  qurt_anysignal_set(&sr_log_internal.signal, SR_LOG_ADD_CLIENT_SET_MASK);
}

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
void sr_log_req_dereg_clients(void)
{
  qurt_anysignal_set(&sr_log_internal.signal, SR_LOG_REMOVE_CLIENT_SET_MASK);
}

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
void sr_log_req_stop(void)
{
  qurt_anysignal_set(&sr_log_internal.signal, SR_LOG_STOP_LOGGER_MASK);
}

/** =====================================================================
 * Function:
 *     sr_log_loc_availabile_start
 *
 * Description:
 *     Function called when SR locator becomes availabile
 *
 * Parameters:
 *     SERVREG_CB_PARAM data
 *     SERVREG_CB_PARAM ignore
 *
 * Returns:
 *     None
 * =====================================================================  */
static void sr_log_loc_availabile_start(SERVREG_CB_PARAM data, SERVREG_CB_PARAM ignore)
{
  (void)tms_utils_launch_qurt_task(SR_LOG_TASK_NAME, TMS_TASK_STACK_3K, TMS_TASK_PRIO_DEFAULT, sr_log_task, NULL, TRUE);
  
  /* Signal task to start logger */
  sr_log_req_start();
}

/** =====================================================================
 * Function:
 *     sr_log_state_change
 *
 * Description:
 *     Function called when a notification for state change is signalled
 *
 * Parameters:
 *     uint32_t q_mask
 *     sr_log_clientset_p curr_set
 *
 * Returns:
 *     None
 * =====================================================================  */
static void sr_log_state_change(uint32_t q_mask, sr_log_clientset_p curr_set)
{
  sr_log_client_p       curr_client_head                    = SERVREG_NULL;
  uint32_t              ack_count                           = 0;
  char                  old_state_str[SR_LOG_STATE_STR_LEN] = {0};
  char                  new_state_str[SR_LOG_STATE_STR_LEN] = {0};

  /* NULL check */
  if (curr_set == SERVREG_NULL)
  {
    TMS_MSG_ERROR("NULL set of clients");
    return;
  }
  
  curr_client_head = curr_set->list_head;
  
  /* For number of listeners to send ACK */
  do{    
    /* Find client for mask */
    while (SERVREG_NULL != curr_client_head)
    {
      if (curr_client_head->state_mask & q_mask)
        break; /* Inner while loop */
      
      curr_client_head = curr_client_head->next;
    }
    
    /* Reached end of list */
    if (SERVREG_NULL == curr_client_head)
    {
      SERVREG_LOG_TRACE_2(END_OF_LIST, q_mask, qurt_thread_get_id());
      TMS_MSG_ERROR("Reached end of list");
      
      break; /* exit */
    }
    
    /* Proceed to send ack if valid */
    if (SERVREG_NULL == curr_client_head->mon_handle)
    {
      SERVREG_LOG_TRACE_0(NULL_MON_HANDLE);
      q_mask = q_mask & ~(curr_client_head->state_mask); /* Clear serviced bit */
      
      continue; /* check condition and proceed */
    }
 
    /* Send ack to service based on transaction_id */
    uint32_t transaction_id = servreg_get_transaction_id(curr_client_head->mon_handle);
    SERVREG_RESULT res = servreg_set_ack(curr_client_head->mon_handle, transaction_id);
    if(res != SERVREG_SUCCESS)
    {
      SERVREG_LOG_TRACE_2(RESULT, res, 0);
      TMS_MSG_ERROR("Logger set ack failed");
    }
    else
      SERVREG_LOG_TRACE_2(ACK_SENT, curr_client_head->mon_handle, transaction_id);

    /* print PD name whose state has changed */
    SERVREG_NAME pd_name = servreg_get_pd_name(curr_client_head->mon_handle);
    if(SERVREG_NULL == pd_name)
    {
       SERVREG_LOG_TRACE_0(FAIL);
       TMS_MSG_ERROR("pd_name is null");
       return;
    }
      
    /* print old and new state info */
    sr_log_get_state_string(curr_client_head->curr_state, old_state_str);

    SERVREG_SERVICE_STATE new_state = servreg_get_service_curr_state(curr_client_head->mon_handle);
    sr_log_get_state_string(new_state, new_state_str);

    SERVREG_LOG_TRACE_2(NEW_STATE, curr_client_head->mon_handle, new_state);
    TMS_MSG_SPRINTF_HIGH_3("%s went from %s to %s", pd_name, old_state_str, new_state_str);

    /* Update state for log info */
    curr_client_head->curr_state = new_state;
    free(pd_name);
    
    /* Clear serviced bit */
    q_mask = q_mask & ~(curr_client_head->state_mask);
        
    ack_count++;
  }while (q_mask); /* while q_mask exists */
  
  /* Check count versus ACK_COUNT and ACK_SENT numbers for discrepancy */
  SERVREG_LOG_TRACE_2(ACK_COUNT, ack_count, qurt_thread_get_id());
}

/** =====================================================================
 * Function:
 *     sr_log_dereg_clients
 *
 * Description:
 *     Deregisters listeners for the tms/servreg service
 *
 * Parameters:
 *     sr_log_clientset_p curr_set
 *
 * Returns:
 *     SERVREG_SUCCESS (0) or SERVREG_FAILURE(-1)
 * =====================================================================  */
static SERVREG_RESULT sr_log_dereg_clients(sr_log_clientset_p curr_set)
{
  sr_log_client_p curr_client_head  = SERVREG_NULL;
  sr_log_client_p next_client       = SERVREG_NULL;

  /* NULL check */
  if (curr_set == SERVREG_NULL)
  {
    return SERVREG_FAILURE;
  }
  
  /* Iterate till registered clients are removed */
  curr_client_head = curr_set->list_head;
  while (SERVREG_NULL != curr_client_head)
  {
    /* Deregister listener */
    if (SERVREG_NULL == curr_client_head->mon_handle)
    {
      SERVREG_LOG_TRACE_0(NULL_MON_HANDLE);
      curr_client_head = curr_client_head->next;
      continue;
    }
      
    if(SERVREG_SUCCESS != servreg_deregister_listener_qurt(curr_client_head->mon_handle, curr_set->signal_ptr, curr_client_head->state_mask))
    {
      TMS_MSG_HIGH_2("Failed to deregister log listener for mon_handle %x, state_mask %x", curr_client_head->mon_handle, curr_client_head->state_mask);
    }
    
    /* Free monitor handle */
    if(SERVREG_SUCCESS != servreg_free_monitor_handle(curr_client_head->mon_handle))
      TMS_MSG_ERROR("Could not free mon_handle");
    else
      SERVREG_LOG_TRACE_2(CLIENT_REM, curr_client_head->mon_handle, curr_client_head->curr_state);
    
    /* Advance by saving next node as head pointer */
    next_client = curr_client_head->next; 
    free(curr_client_head);
    curr_client_head = next_client;
  }
   
  /* Move back to SR_LOG_INIT_STATE_MASK */
  curr_set->curr_state_mask = SR_LOG_INIT_STATE_MASK;
    
  return SERVREG_SUCCESS;
}

/** =====================================================================
 * Function:
 *     sr_log_reg_clients
 *
 * Description:
 *     Registers listeners for the tms/servreg service on all other PDs  
 *
 * Parameters:
 *     sr_log_clientset_p curr_set
 *
 * Returns:
 *     SERVREG_SUCCESS (0) or SERVREG_FAILURE(-1)
 * =====================================================================  */
static SERVREG_RESULT sr_log_reg_clients(sr_log_clientset_p curr_set)
{
  SERVREG_NAME       domain            = SERVREG_NULL;
  SERVREG_NAME       mydomain          = SERVREG_NULL;
  sr_log_client_p    curr_client_head  = SERVREG_NULL;
  sr_log_client_p    new_client        = SERVREG_NULL;
  uint32_t           iter              = 0;
  uint32_t           add_count         = 0;
  
  /* NULL check */
  if (curr_set == SERVREG_NULL)
  {
    return SERVREG_FAILURE;
  }
  
  curr_client_head = curr_set->list_head;
  
  /* Allocate set of clients for tms/servreg as per sr_log_internal.domain_count */
  mydomain = servreg_get_local_domain();

  /* Iterate domain_count+1 to consider all services and skip self */
  for (iter = 0; iter<sr_log_internal.domain_count+1; iter++)
  {
    if(SERVREG_SUCCESS != servreg_get_entry(sr_log_internal.dlhandle, (iter + 1), &domain))
    {
      TMS_MSG_ERROR("Failed to get domain entry");
      SERVREG_LOG_TRACE_0(GET_DMN_ENTRY_FAIL);
      return SERVREG_FAILURE;
    }
    
    /* Cover all other services */
    if(strncmp(domain, mydomain, strlen(mydomain)) == 0)
      continue;

    /* Allocate memory for a usable client */
    new_client = (sr_log_client_p)calloc(1, sizeof(sr_log_client_t));
    if(SERVREG_NULL == new_client)
    {
      TMS_MSG_ERROR("list_head calloc failed");
      SERVREG_LOG_TRACE_0(FAIL);
      return SERVREG_FAILURE;
    }

    /* Register listener */
    new_client->mon_handle = servreg_alloc_monitor_handle(domain, TMS_SERVREG_SERVICE_NAME);
    if(SERVREG_NULL == new_client->mon_handle)
    {
      TMS_MSG_ERROR("Could not allocate mon_handle");
      SERVREG_LOG_TRACE_0(FAIL);
      free(new_client);
      return SERVREG_FAILURE;
    }

    SERVREG_RESULT res = servreg_register_listener_qurt(new_client->mon_handle, curr_set->signal_ptr, curr_set->curr_state_mask);
    if(SERVREG_SUCCESS != res)
    {
      SERVREG_LOG_TRACE_2(CLIENT_ADD_FAIL, new_client->mon_handle, 0);
      TMS_MSG_SPRINTF_ERROR_1("Failed to register log listener for %s", domain);
      
      /* Free monitor handle */
      if(SERVREG_SUCCESS != servreg_free_monitor_handle(new_client->mon_handle))
        TMS_MSG_ERROR("Could not free mon_handle");
      
      free(new_client);
      continue;
    }
    TMS_MSG_SPRINTF_HIGH_1("Added %s listener", domain);
    add_count++;
    
    new_client->state_mask = curr_set->curr_state_mask;
    SERVREG_LOG_TRACE_2(CLIENT_ADD_MASK, new_client->state_mask, 0);

    if (curr_set->curr_state_mask != SR_LOG_MAX_STATE_MASK)
      curr_set->curr_state_mask = curr_set->curr_state_mask << 1; // left shift to move to next mask
    
    new_client->curr_state = servreg_get_service_curr_state(new_client->mon_handle);
    SERVREG_LOG_TRACE_2(CLIENT_ADD, new_client->mon_handle, new_client->curr_state);
    
    /* Save pointers before advancing to allocate new client */
    new_client->next = curr_client_head;     // Save old head pointer as next for new pointer
    curr_client_head = new_client;           // New node becomes head pointer
  }
  
  TMS_MSG_HIGH_1("Added %d listeners", add_count);
  
  /* Save original head pointer */
  curr_set->list_head = curr_client_head;
    
  return SERVREG_SUCCESS;
}

/** =====================================================================
 * Function:
 *     sr_log_client_task
 *
 * Description:
 *     New task entry point for on-demand listeners to tms/servreg service  
 *
 * Parameters:
 *     void* arg
 *
 * Returns:
 *     None
 * =====================================================================  */
void sr_log_client_task(void* arg)
{
  uint32_t q_mask;
  qurt_anysignal_t signal;
  uint32_t set_num = (uint32_t) arg;
  
  sr_log_clientset_t set_internal;
  set_internal.signal_ptr = &signal;
  set_internal.list_head = SERVREG_NULL;
  set_internal.curr_state_mask = SR_LOG_INIT_STATE_MASK;
  
  sr_log_client_internal.client_signal[set_num] = &signal;   // Save pointer to new task signal
  
  qurt_anysignal_init(&signal);
  qurt_anysignal_set(&signal, SR_LOG_ADD_CLIENT_SET_MASK); // Signal self to add clients
  
  /* Task forever loop */
  for (;;)
  {
     q_mask = qurt_anysignal_wait(&signal, SR_LOG_ADD_CLIENT_SET_MASK | SR_LOG_REMOVE_CLIENT_SET_MASK | SR_LOG_STATE_CHANGE_MASK_SET);

     if (q_mask & SR_LOG_ADD_CLIENT_SET_MASK)
     {
        qurt_anysignal_clear(&signal, SR_LOG_ADD_CLIENT_SET_MASK);
        
        sr_log_reg_clients(&set_internal); 
     }
     
     if (q_mask & SR_LOG_REMOVE_CLIENT_SET_MASK)
     {
        qurt_anysignal_clear(&signal, SR_LOG_REMOVE_CLIENT_SET_MASK);
        
        SERVREG_RESULT ret = sr_log_dereg_clients(&set_internal); 
        SERVREG_LOG_TRACE_2(RESULT, ret, 0);
        
        qurt_thread_exit(0);
     }
     
     if (q_mask & SR_LOG_STATE_CHANGE_MASK_SET)
     {
        qurt_anysignal_clear(&signal, q_mask);
        
        TMS_MSG_HIGH_2("sr_log_state_change: 0x%x, 0x%x", q_mask, qurt_thread_get_id());
        SERVREG_LOG_TRACE_2(STATE_CHANGE, q_mask, qurt_thread_get_id());  
        
        sr_log_state_change(q_mask, &set_internal);
     }
  }
}

/** =====================================================================
 * Function:
 *     sr_log_add_task
 *
 * Description:
 *     Adds a task and register new listeners for it  
 *
 * Parameters:
 *     None
 *
 * Returns:
 *     None
 * =====================================================================  */
void sr_log_add_task(void)
{
  qurt_thread_t tid;
  qurt_thread_attr_t attr;
  char new_task_name[12];
  uint32_t set_num = sr_log_client_internal.task_count;
  void* stack = malloc(TMS_TASK_STACK_3K);
  if(SERVREG_NULL == stack)
  {
    TMS_MSG_ERROR("task stack malloc failed");
    SERVREG_LOG_TRACE_0(FAIL);
    return;
  }
  
  (void) tms_utils_fmt(new_task_name, 12, "%s%lu", SR_LOG_TASK_NAME, set_num);

  /* Qurt task to register listeners for logger */
  qurt_thread_attr_init(&attr);
  qurt_thread_attr_set_name(&attr, new_task_name);
  qurt_thread_attr_set_stack_addr(&attr, stack);
  qurt_thread_attr_set_stack_size(&attr, TMS_TASK_STACK_3K);
  qurt_thread_attr_set_priority(&attr, TMS_TASK_PRIO_DEFAULT);

  qurt_thread_create(&tid, &attr, sr_log_client_task, (void*)set_num);
  
  /* Save new task related info */
  sr_log_client_internal.stack_ptr[set_num] = stack;
  sr_log_client_internal.tid[set_num] = tid;
  TMS_MSG_HIGH_3("Set %d added: stack %x, TID %x", set_num, stack, tid);
  
  /* Increment for next run and check for MAX LIMIT */
  sr_log_client_internal.task_count++;
  if (sr_log_client_internal.task_count == SR_LOG_MAX_TASKS)
  {
    sr_log_internal.state = SR_LOGGER_MAX_LIMIT_HIT;            // Logger hit max limit
    SERVREG_LOG_TRACE_2(MAX_SET_LIMIT_HIT, sr_log_client_internal.task_count, 0);
  }
  else
    SERVREG_LOG_TRACE_2(SET_ADDED, set_num, 0);
}

/** =====================================================================
 * Function:
 *     sr_log_kill_task
 *
 * Description:
 *     Signals a task to deregister listeners, clean up and waits for exit  
 *
 * Parameters:
 *     None
 *
 * Returns:
 *     None
 * =====================================================================  */
void sr_log_kill_task(void)
{
  /* For last remaining set of clients */
  if (sr_log_client_internal.task_count == 0)
  {
    SERVREG_RESULT ret = sr_log_dereg_clients(&sr_log_internal.set); 
    SERVREG_LOG_TRACE_2(RESULT, ret, 0);
    
    sr_log_internal.state = SR_LOGGER_DOWN;
    SERVREG_LOG_TRACE_0(UNINIT);
    servreg_free_domainlist_handle(sr_log_internal.dlhandle);
  }
  else if (sr_log_client_internal.task_count > 0)
  {
    int status;
    sr_log_client_internal.task_count--;
    
    TMS_MSG_HIGH_3("Removing set %d: stack %x, TID %x", sr_log_client_internal.task_count, sr_log_client_internal.stack_ptr[sr_log_client_internal.task_count], sr_log_client_internal.tid[sr_log_client_internal.task_count]);
    
    qurt_anysignal_set(sr_log_client_internal.client_signal[sr_log_client_internal.task_count], SR_LOG_REMOVE_CLIENT_SET_MASK);
    
    /* Wait for thread to exit */
    qurt_thread_join(sr_log_client_internal.tid[sr_log_client_internal.task_count], &status);
    SERVREG_LOG_TRACE_2(RESULT, status, 0);
    
    /* Free allocated stack */
    if (SERVREG_NULL != sr_log_client_internal.stack_ptr[sr_log_client_internal.task_count])
      free(sr_log_client_internal.stack_ptr[sr_log_client_internal.task_count]);
    
    TMS_MSG_HIGH_1("Removed set %d", sr_log_client_internal.task_count);    
    SERVREG_LOG_TRACE_2(SET_REMOVED, sr_log_client_internal.task_count, sr_log_client_internal.tid[sr_log_client_internal.task_count]);

    /* Clean-up */
    sr_log_client_internal.tid[sr_log_client_internal.task_count] = 0;
    sr_log_client_internal.stack_ptr[sr_log_client_internal.task_count] = SERVREG_NULL;
    sr_log_client_internal.client_signal[sr_log_client_internal.task_count] = SERVREG_NULL;
    
    if (sr_log_internal.state == SR_LOGGER_MAX_LIMIT_HIT)
      sr_log_internal.state = SR_LOGGER_UP;
  }
}


/** =====================================================================
 * Function:
 *     sr_log_stop
 *
 * Description:
 *     Stops all listeners to tms/servreg service by killing all tasks
 *
 * Parameters:
 *     None
 *
 * Returns:
 *     None
 * =====================================================================  */
static void sr_log_stop(void)
{
  do{
    sr_log_kill_task();
  }while(sr_log_internal.state != SR_LOGGER_DOWN);
}

/** =====================================================================
 * Function:
 *     sr_log_start
 *
 * Description:
 *     Initialize the logger service by creating a first set of listeners
 *
 * Parameters:
 *     None
 *
 * Returns:
 *     None
 * =====================================================================  */
void sr_log_start(void)
{
  sr_log_internal.state = SR_LOGGER_DOWN;
  sr_log_internal.dlhandle = SERVREG_NULL;
  sr_log_internal.domain_count = 0;

  sr_log_client_internal.task_count = 0;

  /* Create first set of clients */
  sr_log_internal.set.list_head = SERVREG_NULL;
  sr_log_internal.set.curr_state_mask = SR_LOG_INIT_STATE_MASK;
  sr_log_internal.set.signal_ptr = &(sr_log_internal.signal);  // reusing this task
  
  /* Get domain list */
  sr_log_internal.dlhandle = servreg_alloc_domainlist_handle();
  if(SERVREG_NULL == sr_log_internal.dlhandle)
  {
    TMS_MSG_ERROR("Failed to allocate domain list handle");
    return;
  }
  
  if(SERVREG_SUCCESS != servreg_get_domainlist(TMS_SERVREG_SERVICE_NAME, sr_log_internal.dlhandle))
  {
    TMS_MSG_ERROR("Failed to get domain list handle");
    servreg_free_domainlist_handle(sr_log_internal.dlhandle);
    return;
  }

  /* Get domain_count */
  sr_log_internal.domain_count = servreg_get_num_entries(sr_log_internal.dlhandle);
  if(sr_log_internal.domain_count <= 0)
  {
    TMS_MSG_ERROR_1("Invalid domain count: %d", sr_log_internal.domain_count);
    servreg_free_domainlist_handle(sr_log_internal.dlhandle);
    return;
  }
  else if (sr_log_internal.domain_count > SR_LOG_MAX_SERVICES)
    sr_log_internal.domain_count = SR_LOG_MAX_SERVICES;
  
  /* Decrement domain_count as we count ourselves out */
  sr_log_internal.domain_count = sr_log_internal.domain_count - 1;

  /* Register first set of clients */
  if (SERVREG_SUCCESS != sr_log_reg_clients(&sr_log_internal.set))
    TMS_MSG_ERROR("Error in registering first set of clients");
  else
  {
    sr_log_internal.state = SR_LOGGER_UP;
    SERVREG_LOG_TRACE_0(INIT);
  }
}

/** =====================================================================
 * Task:
 *     sr_log_task
 *
 * Description:
 *     Task used for processing logger requests
 *
 * Parameters:
 *     None
 *
 * Returns:
 *     None
 * =====================================================================  */
void sr_log_task(void * unused_param )
{
  uint32_t q_mask;
  
  /* Task forever loop */
  for (;;)
  {
     q_mask = qurt_anysignal_wait(&sr_log_internal.signal, SR_LOG_START_LOGGER_MASK | SR_LOG_ADD_CLIENT_SET_MASK | SR_LOG_REMOVE_CLIENT_SET_MASK | SR_LOG_STOP_LOGGER_MASK | SR_LOG_STATE_CHANGE_MASK_SET);     

     SERVREG_LOG_TRACE_2(WAIT_EXIT, q_mask, qurt_thread_get_id());

     if (q_mask & SR_LOG_START_LOGGER_MASK)
     {
        qurt_anysignal_clear(&sr_log_internal.signal, SR_LOG_START_LOGGER_MASK);
        
        SERVREG_LOG_TRACE_0(START_REQ);

        if (SR_LOGGER_UP == sr_log_internal.state)
          TMS_MSG_ERROR("Logger already running");
        else
          sr_log_start();
     }
     
     if (q_mask & SR_LOG_ADD_CLIENT_SET_MASK)
     {
        qurt_anysignal_clear(&sr_log_internal.signal, SR_LOG_ADD_CLIENT_SET_MASK);

        SERVREG_LOG_TRACE_0(ADD_CLIENT_SET_REQ);
        
        if (SR_LOGGER_UP != sr_log_internal.state)
          TMS_MSG_ERROR("Logger not running or max limit reached");
        else   
          sr_log_add_task(); 
     }
     
     if (q_mask & SR_LOG_REMOVE_CLIENT_SET_MASK)
     {
        qurt_anysignal_clear(&sr_log_internal.signal, SR_LOG_REMOVE_CLIENT_SET_MASK);

        SERVREG_LOG_TRACE_0(REM_CLIENT_SET_REQ);
        
        if (SR_LOGGER_DOWN == sr_log_internal.state)
          TMS_MSG_ERROR("Logger not running");
        else
          sr_log_kill_task();
     }
     
     if (q_mask & SR_LOG_STOP_LOGGER_MASK)
     {
        qurt_anysignal_clear(&sr_log_internal.signal, SR_LOG_STOP_LOGGER_MASK);

        SERVREG_LOG_TRACE_0(STOP_REQ);
        
        if (SR_LOGGER_DOWN == sr_log_internal.state)
          TMS_MSG_ERROR("Logger already stopped");
        else
          sr_log_stop();
     }
     
     if (q_mask & SR_LOG_STATE_CHANGE_MASK_SET)
     {
		 qurt_anysignal_clear(&sr_log_internal.signal, q_mask);

        TMS_MSG_HIGH_2("sr_log_state_change: 0x%x, 0x%x", q_mask, qurt_thread_get_id());
        SERVREG_LOG_TRACE_2(STATE_CHANGE, q_mask, qurt_thread_get_id());

        if (SR_LOGGER_DOWN == sr_log_internal.state)
          TMS_MSG_ERROR("Logger not running");
        else
          sr_log_state_change(q_mask, &sr_log_internal.set);
     }
  } /* for(;;) */
}

/** =====================================================================
 * Init:
 *     sr_log_init
 *
 * Description:
 *     Init function used for processing logger requests
 *
 * Parameters:
 *     None
 *
 * Returns:
 *     None
 * =====================================================================  */
void sr_log_init(void)
{
#ifdef SERVREG_LOG_DIAG_ENABLE
  sr_log_diag_init();
#endif

  /* Initialize signal */
  qurt_anysignal_init(&sr_log_internal.signal);
  
  /* Signal start task upon service locator availability */
  if (SERVREG_SUCCESS != servreg_register_servloc_availability_cb(sr_log_loc_availabile_start, NULL, NULL))
    TMS_MSG_ERROR("Failed to register for locator availability");
  
  if (SERVREG_SERVICE_STATE_UP == servreg_get_servloc_availability())
  {
    if (SERVREG_SUCCESS != servreg_deregister_servloc_availability_cb(sr_log_loc_availabile_start, NULL, NULL))
      TMS_MSG_ERROR("Failed to deregister for locator availability");

    sr_log_loc_availabile_start(0,0);
  }
}
