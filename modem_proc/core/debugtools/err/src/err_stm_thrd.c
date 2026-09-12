/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

         E R R O R     R E P O R T I N G    S E R V I C E S

GENERAL DESCRIPTION
  Maximize chances of a clean shutdown right before STM mode

Copyright (c) 2018 by Qualcomm Technologies, Inc.  All Rights Reserved.

*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

  EDIT HISTORY FOR FILE

  This section contains comments describing changes made to this file.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/core.mpss/10.0/debugtools/err/src/err_stm_thrd.c#1 $

  
===========================================================================*/

/*==========================================================================

  INCLUDE FILES FOR MODULE

==========================================================================*/

#include "tms_utils.h"
#include "erri.h"

extern int QURT_MAX_HTHREADS;

#define NUM_ERR_STM_THRDS        (QURT_MAX_HTHREADS - 1)
#define ERR_STM_THRD_NAME        "err_stm_thrd_%d"
#define SIZEOF_ERR_STM_THRD_NAME (15)   /* name_base + n + \0 */
#define ERR_STM_THRD_MASK        0x1

enum err_stm_thrd_status_e
{
  ERR_STM_THRD_STATUS_INIT  = 0,
  ERR_STM_THRD_STATUS_WAIT  = 1,
  ERR_STM_THRD_STATUS_READY = 2,
  ERR_STM_THRD_STATUS_RUN   = 3,
  ERR_STM_THRD_MAX          = 0xFFFFFFFF,
};

	
struct err_stm_thrd_data_s 
{
  qurt_anysignal_t            signal;
  enum err_stm_thrd_status_e  status;
} ;

struct err_stm_thrd_s 
{
  struct err_stm_thrd_data_s * data;
  int                          sig_rcvd;
  qurt_mutex_t                 mutex;
} ;

static struct err_stm_thrd_s err_stm_thrd_internal;

void err_stm_thrd_signal_and_wait(void) 
{
    unsigned int i;
    unsigned long long start_ticks;
    
    for (i = 0; i < NUM_ERR_STM_THRDS; ++i) {
        err_stm_thrd_internal.data[i].status = ERR_STM_THRD_STATUS_READY;
        ERR_FLUSH_ADDR(err_stm_thrd_internal.data[i].status);
        qurt_anysignal_set(&err_stm_thrd_internal.data[i].signal, ERR_STM_THRD_MASK);
    }

    start_ticks = qurt_sysclock_get_hw_ticks();

    /* while not reaching timeout of 100 usecs in ticks */
    while (qurt_sysclock_get_hw_ticks() - start_ticks < ((100 * 192)/10))
    {
        /* If all err stm threads are running, return */
	/* This along with disabling external interrupts and bumping the priority of DLPager thread ( Expected to be done in PRESTM callback) 
	 * would ensure that any dlpager thread or ISR is not running */
        for (i = 0; i < NUM_ERR_STM_THRDS; ++i)
          if (err_stm_thrd_internal.data[i].status != ERR_STM_THRD_STATUS_RUN ) 
            break;

        if (i == NUM_ERR_STM_THRDS)
        {
          return;
        }
    }
}

void err_stm_thrd_wait(void* arg)
{
    int i;
    volatile int dummy = 1;

    /* Make sure a unique value from 0 to NUM_ERR_STM_THRDS -1 is assigned to 'i'.
     * This wait call should be called NUM_ERR_STM_THRDS -1 times */
    
    qurt_pimutex_lock(&err_stm_thrd_internal.mutex);
    i = err_stm_thrd_internal.sig_rcvd;
    err_stm_thrd_internal.sig_rcvd++;
    qurt_pimutex_unlock(&err_stm_thrd_internal.mutex);
    
    while (1)
    {
      err_stm_thrd_internal.data[i].status = ERR_STM_THRD_STATUS_WAIT;
      qurt_anysignal_wait(&err_stm_thrd_internal.data[i].signal, ERR_STM_THRD_MASK);
      err_stm_thrd_internal.data[i].status = ERR_STM_THRD_STATUS_RUN;
      ERR_FLUSH_ADDR(err_stm_thrd_internal.data[i].status);
      while(1) 
      {
        /* Kick Dog */
        dog_force_kick();
        dummy++; 
      }
    }
}

void err_stm_thrd_init(void) 
{
    int i;
    char tname[SIZEOF_ERR_STM_THRD_NAME]; 

    qurt_pimutex_init(&err_stm_thrd_internal.mutex);
    err_stm_thrd_internal.data = calloc(NUM_ERR_STM_THRDS, sizeof(struct err_stm_thrd_data_s));

    if ( err_stm_thrd_internal.data == NULL )
    {
      TMS_STARTUP_MSG_ERROR("Allocation failed ");
      return;
    }

    for (i = 0; i < NUM_ERR_STM_THRDS; ++i) {
        qurt_anysignal_init(&err_stm_thrd_internal.data[i].signal);

        (void)tms_utils_fmt(tname, SIZEOF_ERR_STM_THRD_NAME, ERR_STM_THRD_NAME, i);
        
	(void)tms_utils_launch_qurt_task(tname, TMS_TASK_STACK_256, TMS_TASK_PRIO_EXCEPTION+1,  err_stm_thrd_wait, NULL, TRUE);
    }
}
