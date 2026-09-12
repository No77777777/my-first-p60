/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                Diagnostics Qshrink4 Handler Routines

General Description
  Functions and command handlers related to Qshrink4 database retreival and
  maintenance.

Copyright (c) 2000-2018 by QUALCOMM Technologies, Incorporated.  All Rights Reserved.
All Rights Reserved.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                           Edit History

$Header: //components/rel/core.mpss/10.0/services/diag/DCM/common/src/diag_qshrink4.c#1 $

when       who     what, where, why
--------   ---     ---------------------------------------------------------- 
09/03/18   gn      Diag timers categorization into deferrable and non-deferrable
02/15/17   rs      Added API diagpkt_max_delayed_rsp_size() and diagpkt_total_delayed_bytes_queued() and exposed them public.
01/08/16   tbg     Pulled in related functions from diagdiag_common.c, and
                   added support for v2 (delayed) Qshrink4 database commands.
===========================================================================*/

 
#include "core_variation.h"
#include "comdef.h"
#include "diag_v.h"
#include "diagdiag_v.h"
#include "diagcomm_v.h"
#include "stringl.h"
#include "assert.h"
#include <stdlib.h> 
#include "qw.h"
#include "queue.h"
#include "rfs_api.h"   /*for Qshrink_4 file retrieval*/
#include "rfs_errno.h" /*for qshrink_4_error_code()*/
#include "osal.h"
#include "ULogFront.h"
#include "diagpkti.h"
#include "diag_diagIDi.h"

extern ULogHandle diag_err_log_handle; 

static osal_thread_attr_t qshrink4_delayed_rsp_attr;
extern osal_mutex_arg_t diag_delayed_response_mutex;
extern uint8 msg_v4_dbase_guid[QSHRINK_4_DATABASE_GUID_LENGTH];
extern msg_v4_dbase_name_type msg_v4_dbase_name;
diag_qshrink_4_database_file_type diag_qshrink_database_file_list[MAX_QSHRINK_DATABASE_FILE_NUMBER];
uint8 diag_qshrink4_guid_count  = 0;
static osal_mutex_arg_t diag_qshrink4_database_mutex;
static int thread_exit_status = 0;

/* Queue for diag qshrink task */
q_type diag_qshrink_4_task_q;

/* Stores info, whether Q is initialised or not */
boolean diag_qshrink_4_task_q_init = FALSE;

/*Current diag_qshrink_4 tasks count*/
uint32 diag_qshrink_4_task_cnt = 0;

/*Maximum diag_qshrink_4 tasks allowed */
#define DIAG_MAX_QSHRINK_4_TASK_CNT 5

static void* diag_qshrink4_file_list_handler(uint16 version, boolean delayed_flag, uint32 delayed_rsp_id);
static void* diag_qshrink4_file_open_handler(uint16 version, boolean delayed_flag, uint32 delayed_rsp_id, uint32 guid_index, diag_guid_type *guid_ptr);
static void* diag_qshrink4_file_close_handler(uint16 version, boolean delayed_flag, uint32 delayed_rsp_id, int fd);


/*===========================================================================

FUNCTION DIAG_QSHRINK_4_CLEANUP_CB

DESCRIPTION
  This procedure is a cleanup function. It goes through the
  diag_qshrink_4_task_q, checks if any data block is ready to cleaned.
  If yes it removes the entry from the Q and frees the memory.

============================================================================*/
void  diag_qshrink_4_cleanup_cb(void *param)
{
  diag_qshrink_4_task_q_item_type *q_cur_ptr = NULL;
  diag_qshrink_4_task_q_item_type *q_next_ptr = NULL;
  int status;

  q_cur_ptr = (diag_qshrink_4_task_q_item_type *)q_check(&diag_qshrink_4_task_q);
  while (q_cur_ptr  != NULL)
  {
    /* If the element in the queue is ready to be deleted */
    if (q_cur_ptr->data.status == DIAG_TASK_DONE)
    {
      /* mapped to no-ops for all OSes except BLAST */
      osal_thread_join(&(q_cur_ptr->data.tcb), &status);

      /* cleanup OSAL related memory, queues, channel etc */
      osal_delete_thread(&(q_cur_ptr->data.tcb));

      /* store the next link */
      q_next_ptr = (diag_qshrink_4_task_q_item_type *)q_next
           (&diag_qshrink_4_task_q, &q_cur_ptr->link);

      /* Remove the data block from the Q */
#ifdef FEATURE_Q_NO_SELF_QPTR
      q_delete(&diag_qshrink_4_task_q, &q_cur_ptr->link);
#else
      q_delete(&q_cur_ptr->link);
#endif

      /* Free the memory */
      free(q_cur_ptr);

      if (q_cur_ptr != NULL)
      {
        q_cur_ptr = NULL;
      }
      q_cur_ptr = q_next_ptr;
    }
    else
    {
      /* store the next link */
      q_cur_ptr = (diag_qshrink_4_task_q_item_type *)q_next
           (&diag_qshrink_4_task_q, &q_cur_ptr->link);
    }
  }

  /* Deregister's when the count is 0(Q is empty) */
  if (diag_qshrink_4_task_q.cnt == 0)
  {
    (void)diag_idle_processing_unregister(&diag_qshrink_4_cleanup_cb,
         &diag_qshrink_4_task_q);
  }

  return;
}


/*===========================================================================

FUNCTION DIAG_QSHRINK_4_STATUS_ENUM_TYPE

DESCRIPTION
  Convert RFS error code into Qshrink4 error code

============================================================================*/
static diag_qshrink_4_status_enum_type qshrink_4_error_code(int error_code)
{
  switch (error_code)
  {
  case RFS_ENOERROR:                /* No error */
    return (DIAG_QS4_SUCCESS);
  case RFS_ENOENT:                  /*No such file or directory */
    return (DIAG_QS4_ENOENT);
  case RFS_EACCESS:                 /* Access denied */
    return (DIAG_QS4_EACCESS);
  case RFS_ENOMEM:                  /*Ran out of memory */
    return (DIAG_QS4_ENOMEM);
  case RFS_EPERM:                   /* Operation not permitted */
    return (DIAG_QS4_EACCESS);
  case RFS_EBADF:                   /* Bad file descriptor */
    return (DIAG_QS4_EBADF);
  case RFS_EEXISTS:                 /* Entry exists */
    return (DIAG_QS4_EACCESS);
  case RFS_ENODEV:                  /* No RFS device found. */
    return (DIAG_QS4_EACCESS);
  case RFS_EINVAL:                  /* Invalid argument */
    return (DIAG_QS4_EINVAL);
  case RFS_EMFILE:                  /* Too many open files */
    return (DIAG_QS4_EMFILE);
  case RFS_ENOSPC:                  /* No space left on device */
    return (DIAG_QS4_ENOMEM);
  case RFS_ENAMETOOLONG:           /* File name too long */
    return (DIAG_QS4_EINVAL);
  case RFS_EBUSY:                  /* Resource is busy */
    return (DIAG_QS4_ELOOP);
  case RFS_ESYSTEM:                /* System Error */
    return (DIAG_QS4_EINTR);
  case RFS_ETIMEOUT:              /*Request timed-out  */
    return (DIAG_QS4_EAGAIN);
  case RFS_ECONFIG:               /* Build-config error  */
    return (DIAG_QS4_EPERM);
  case RFS_ENOTSUPPORTED:         /* Operation not supported  */
    return (DIAG_QS4_EPERM);
  default:
    return (DIAG_QS4_EIO);          /*by default return I/O error*/
  }
}

extern unsigned int  cur_delayed_rsp_memory_used;
#define DELAYED_RSP_MEMORY_USE_CAP 0x6000
#define QSHRINK_DATABASE_RETRIEVE_SLEEP_DURATION 5


/*===========================================================================

FUNCTION DIAG_QSHRINK4_DATABASE_STREAM

DESCRIPTION
  Function which streams out the requested pieces of the qshrink4 database
  using delayed responses.

  Input parameters:
  fd - file_descriptor
  offset - offset into the file to start from
  length - length of data to read
  delayed_rsp_id - ID to use for delayed responses

============================================================================*/
static void diag_qshrink4_database_stream(uint16 version, int fd, uint32 offset, uint32 length, uint16 delayed_rsp_id)
{
  int i = 0;
  uint32 file_length = 0;
  uint32 current_offset = offset;
  uint32 remaining_length = 0;
  uint32 read_length = 0;
  uint32 copy_size = 0;
  int rsp_len = 0;
  int data_size = QSHRINK_DATABASE_READ_DATABLOCK_SIZE;
  diag_qshrink_4_database_file_retrieve_rsp_type *rsp = NULL;
  int result = -1;
  uint16 response_count = 0x8001;
  boolean timer_init = FALSE;       /* timer initialized or not */
  osal_timer_t sleep_timer;
  boolean return_val = FALSE;
  osal_sigs_t return_sigs;
  int buffer_index = 0;
  int remaining_data_in_buffer;
  uint32 read_size = 0;
  byte *qshrink_buffer = NULL;
  int guid_count = diag_qshrink4_guid_count;

  uint32 diag_delayed_rsp_mem_used = 0;
  for (i = 0; i < guid_count; i++)
  {
    if (diag_qshrink_database_file_list[i].fd == fd)
    {
      file_length = diag_qshrink_database_file_list[i].file_len;
      break;
    }
  }

  read_length = (offset + length) > file_length ? (file_length - offset) : length;
  remaining_length = read_length;

  result = rfs_seek(fd, offset, RFS_SEEK_SET);
  if (result != offset)
  {
    ULOG_RT_PRINTF_2( diag_err_log_handle,"Error in diag_qshrink4_database_stream. rfs_seek returned %d insetad of expected %d",result,offset);
    return;
  }
  
  qshrink_buffer = malloc(QSHRINK_BUFFER_SIZE);
  if (qshrink_buffer == NULL) 
  {
    ULOG_RT_PRINTF_1( diag_err_log_handle,"Error in diag_qshrink4_database_stream. malloc failure for qshrink_buffer (size %d)",QSHRINK_BUFFER_SIZE);    
    return;
  }
  
  while (remaining_length > 0)
  {
    if (remaining_length > QSHRINK_BUFFER_SIZE)
    {
      read_size = QSHRINK_BUFFER_SIZE;
    }
    else
    {
      read_size = remaining_length;
    }
    remaining_length -= read_size;
        
    memset(qshrink_buffer, 0, QSHRINK_BUFFER_SIZE);
    result = rfs_read(fd, (byte *)qshrink_buffer, read_size);

    if (result == read_size)
    {
      remaining_data_in_buffer  = read_size;
      buffer_index = 0;
      while (remaining_data_in_buffer > 0)
      {
        if (remaining_data_in_buffer > data_size)
        {
          copy_size = data_size;
        }
        else
        {
          copy_size = remaining_data_in_buffer;
        }
        rsp_len = FPOS(diag_qshrink_4_database_file_retrieve_rsp_type, operation_data) + FPOS(diag_qshrink_4_file_read_rsp_type, data_block);
        rsp_len += copy_size;
        osal_lock_mutex(&diag_delayed_response_mutex);
		diag_delayed_rsp_mem_used = diagpkt_total_delayed_bytes_queued();
        if ((diag_delayed_rsp_mem_used + (unsigned int)rsp_len) < DELAYED_RSP_MEMORY_USE_CAP)
        { //DIAG_MAX_RX_PKT_SIZ *6
          osal_unlock_mutex(&diag_delayed_response_mutex);
          ULogFront_RealTimePrintf(diag_err_log_handle, 2, "1:diag_delayed_rsp_mem_used is %d rsp_len is %d ", diag_delayed_rsp_mem_used, rsp_len );
          rsp = (diag_qshrink_4_database_file_retrieve_rsp_type *)diagpkt_subsys_alloc_v2_delay(DIAG_SUBSYS_DIAG_SERV, DIAGDIAG_QSHRINK_4_RETRIEVE_DATABASE_MODEM,
            delayed_rsp_id, rsp_len);
        }
        else
        {
          osal_unlock_mutex(&diag_delayed_response_mutex);
        }
        
        if (rsp)
        {
          remaining_data_in_buffer -= copy_size;
          /*if multiple delayed response, update the res_cnt field in the header, otherwise keep it 1*/
          if (read_length > data_size)
          {
            if (remaining_length == 0 && remaining_data_in_buffer == 0)
            { /*last delayed response*/
              response_count = response_count & (0x0FFF);
            }
            rsp->header.rsp_cnt = response_count;
            response_count++;
          }
          rsp->version = version;
          rsp->op_code = DIAG_QSHRINK_4_FILE_READ;
          rsp->operation_data.file_read_rsp.fd = fd;
          rsp->operation_data.file_read_rsp.offset = current_offset;
          rsp->operation_data.file_read_rsp.num_read = copy_size;
          rsp->operation_data.file_read_rsp.status = DIAG_QS4_SUCCESS;
          memscpy((byte *)(rsp->operation_data.file_read_rsp.data_block), copy_size, (byte *)qshrink_buffer + buffer_index, copy_size);
          diagpkt_delay_commit(rsp);
          rsp = NULL;
          buffer_index += copy_size;
          current_offset += copy_size;
        }
        else
        {
          if (timer_init == FALSE)
          {
            /* Defines a timer */
            osal_create_timer(&sleep_timer, osal_thread_self(), DIAG_QSHRINK_4_SLEEP_SIG);
            timer_init = TRUE;
          }
          
          /* Wait for sleep_duration. This blocks the current task, and
           different task with which is ready to run starts executing */
          (void)osal_timed_wait(osal_thread_self(), DIAG_QSHRINK_4_SLEEP_SIG, &sleep_timer,
            QSHRINK_DATABASE_RETRIEVE_SLEEP_DURATION);
          
          /* Clear the signals. The block on the current task is cleared */
          return_val = osal_reset_sigs(osal_thread_self(), DIAG_QSHRINK_4_SLEEP_SIG, &return_sigs);
          ASSERT(OSAL_SUCCESS == return_val);
        }
      }
    }
  }

  free(qshrink_buffer);
  
  /* delete the timer to free the memory allocated when the timer was created */
  if (timer_init == TRUE)
  {
    return_val = osal_delete_timer(&sleep_timer);
    ASSERT(OSAL_SUCCESS == return_val);
  }
  
  return;
}

/*===========================================================================

FUNCTION DIAG_QSHRINK4_DELAYED_TASK

DESCRIPTION
  This task is spawned to generated delayed responses to different
  Qshrink4 diag commands

============================================================================*/
void diag_qshrink4_delayed_task(uint32 params)
{
  diag_qshrink_4_delayed_tcb_stack_type *local_data = (diag_qshrink_4_delayed_tcb_stack_type *)params;
  diag_qshrink_4_database_file_retrieve_rsp_type *rsp = NULL;
  int opcode = local_data->task_info.opcode;
  uint32 diag_qshrink_4_task_cnt_old=0;
  uint32 diag_qshrink_4_task_cnt_new=0;  

  /* op_code determines the actions needed by the task */
  switch(opcode)
  {
    case DIAG_QSHRINK_4_FILE_LIST:
      rsp = diag_qshrink4_file_list_handler(local_data->task_info.version, TRUE,
        local_data->task_info.delayed_response_id);
      if(rsp)
        diagpkt_delay_commit(rsp);
      break;

    case DIAG_QSHRINK_4_FILE_OPEN:
      rsp = diag_qshrink4_file_open_handler(local_data->task_info.version, TRUE,
        local_data->task_info.delayed_response_id,
        local_data->task_info.file_info, NULL);
      if(rsp)
        diagpkt_delay_commit(rsp);      
      break;

    case DIAG_QSHRINK_4_FILE_CLOSE:
      rsp = diag_qshrink4_file_close_handler(local_data->task_info.version, TRUE,
        local_data->task_info.delayed_response_id,
        local_data->task_info.file_info);
      if(rsp)
        diagpkt_delay_commit(rsp);      
      break;

    case DIAG_QSHRINK_4_FILE_READ:
      diag_qshrink4_database_stream(local_data->task_info.version,
        local_data->task_info.file_info,
        local_data->task_info.offset,
        local_data->task_info.length,
        local_data->task_info.delayed_response_id);
      break;

    default:
      break;
  }

  if (osal_thread_get_pri() <= GET_DIAG_PRI())
  {
    /* This is done to make sure that the task gets killed before the
     cleanup function, running in diag context, removes the data from the Q */
    (void)osal_thread_set_pri(GET_DIAG_PRI() + 1);
  }

  /* Decrement task counter */
  do{
    diag_qshrink_4_task_cnt_old = diag_qshrink_4_task_cnt;
    diag_qshrink_4_task_cnt_new = diag_qshrink_4_task_cnt_old - 1;
  }while (FALSE == osal_atomic_compare_and_set((osal_atomic_word_t *)&diag_qshrink_4_task_cnt, diag_qshrink_4_task_cnt_old, diag_qshrink_4_task_cnt_new));
  
  /* mark the task to be cleaned */
  local_data->status = DIAG_TASK_DONE;
  
  /* mapped to no-ops for all OSes except BLAST */
  osal_thread_exit(thread_exit_status);
}


/*===========================================================================

FUNCTION DIAG_QSHRINK4_REGISTER_ENTRY

DESCRIPTION
  Used to safely register a new entry into the Qshrink4 database list

RETURN VALUE
  TRUE if successful
  FALSE if unsuccessful

============================================================================*/

boolean diag_qshrink4_register_entry(diag_qshrink_4_database_file_type *db_entry)
{
  boolean rval = FALSE;
  
  osal_lock_mutex(&diag_qshrink4_database_mutex);
  
  if (diag_qshrink4_guid_count < MAX_QSHRINK_DATABASE_FILE_NUMBER)
  {
    memscpy(&diag_qshrink_database_file_list[diag_qshrink4_guid_count],
      sizeof(diag_qshrink_4_database_file_type),
      db_entry,
      sizeof(diag_qshrink_4_database_file_type));
    diag_qshrink4_guid_count++;
    rval = TRUE;
  }
  else
  {
    MSG(MSG_SSID_DIAG, MSG_LEGACY_ERROR, "Qshrink4 database registration attemtped when array already full");
  }
  
  osal_unlock_mutex(&diag_qshrink4_database_mutex);
  
  return rval;
}


/*===========================================================================

FUNCTION QSHRINK4_CREATE_DELAYED_THREAD

DESCRIPTION
  Helper function to create and spawn a thread to handle delayed reposnes
  for a qshirnk4 database command.

============================================================================*/
static void qshrink4_create_delayed_thread(uint16 version, uint16 delayed_response_id, int file_info, uint32 offset, uint32 length, uint16 opcode)
{
  diag_qshrink_4_task_q_item_type *q_item = NULL;
  int diag_qshrink4_pri_array[32] = { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 };


  
  /* Get memory from the system heap */
  q_item = (diag_qshrink_4_task_q_item_type *)malloc(
    sizeof(diag_qshrink_4_task_q_item_type));
  
  if (q_item != NULL)
  {
    memset(q_item, 0x0, sizeof(diag_qshrink_4_task_q_item_type));
    /* Initialize the link field */
    (void)q_link(q_item, &(q_item->link));

    /* Fill the data */
    q_item->data.status = DIAG_TASK_INITIALIZED;
    q_item->data.task_info.priority.pri = GET_DIAG_PRI() + 1;
    q_item->data.task_info.version = version;
    q_item->data.task_info.delayed_response_id = delayed_response_id;
    q_item->data.task_info.file_info = file_info;
    q_item->data.task_info.opcode = opcode;    
    q_item->data.task_info.offset = offset;
    q_item->data.task_info.length = length;
    /* Place q_item on the diag_qshrink_4_task_q */
    q_put(&diag_qshrink_4_task_q, &(q_item->link));
  }
  else /* Failed  system malloc, handle this error */
  {
    MSG(MSG_SSID_DIAG, MSG_LEGACY_HIGH, "Failed to allocate q_item for Qshrink helper task.\n");
    return;
  }
  
  /* Start the task */
  /* stack is of type unsigned long long' */
  qshrink4_delayed_rsp_attr.stack_size = (STRESS_QSHRINK_4_STACK_SIZE * sizeof(unsigned long long));
  qshrink4_delayed_rsp_attr.start_func = diag_qshrink4_delayed_task;
  qshrink4_delayed_rsp_attr.arg = (void *)(&(q_item->data));
  qshrink4_delayed_rsp_attr.name = "qshrink4_delayed_rsp_attr";
  qshrink4_delayed_rsp_attr.priority = q_item->data.task_info.priority.pri;
  qshrink4_delayed_rsp_attr.stack_address = q_item->data.stack;
  qshrink4_delayed_rsp_attr.p_tskname = "DIAG_QSHRINK_4";
  qshrink4_delayed_rsp_attr.suspended = FALSE;
  qshrink4_delayed_rsp_attr.dog_report_val = 0;
  
  /* Mark the task is running BEFORE creating the thread for multi-threaded case */
  if (q_item->data.status == DIAG_TASK_INITIALIZED)
  {
    q_item->data.status = DIAG_TASK_RUNNING;
  }

  if ((osal_create_thread(&(q_item->data.tcb), &qshrink4_delayed_rsp_attr, diag_qshrink4_pri_array)) != 0)
  {
    ULOG_RT_PRINTF_1( diag_err_log_handle,"Failed to start delayed qshrink task (for opcode %d)",opcode);
    
    /* Delete queue item */
    #ifdef FEATURE_Q_NO_SELF_QPTR
    q_delete( &diag_qshrink_4_task_q, &q_item->link );
    #else
    q_delete(&q_item->link);
    #endif
    free(q_item);
  }
  else
  {
    int diag_qshrink_4_task_cnt_old=0;
    int diag_qshrink_4_task_cnt_new=0;    

    /* Increment count for delayed processing threads */
    do{
      diag_qshrink_4_task_cnt_old = diag_qshrink_4_task_cnt;
      diag_qshrink_4_task_cnt_new = diag_qshrink_4_task_cnt_old + 1;
    } while(FALSE == osal_atomic_compare_and_set((osal_atomic_word_t *)&diag_qshrink_4_task_cnt, diag_qshrink_4_task_cnt_old, diag_qshrink_4_task_cnt_new));

    /* Register that the Q has items,  and needs to be cleaned */
    (void)diag_idle_processing_register(diag_qshrink_4_cleanup_cb, &diag_qshrink_4_task_q);
  }
  
  return;
}

/*===========================================================================

FUNCTION DIAG_QSHRINK4_FILE_LIST_HANDLER

DESCRIPTION
  Process Qshrink4 FILE_LIST database commands. Supports immediate and delayed
  responses, and version 1 and 2 of the command.
  
PARAMETERS
  version - version of the command
  delayed_flag - TRUE if a delayed reponse, FALSE is it is the first response
  delayed_rsp_id - ID to use for delayed responses
  
DEPENDENCIES
  None
  
RETURN VALUE
  Pointer to response packet
  
===========================================================================*/

static void* diag_qshrink4_file_list_handler(uint16 version, boolean delayed_flag, uint32 delayed_rsp_id)
{
  diag_qshrink_4_database_file_retrieve_rsp_type *rsp = NULL;
  int rsp_len = FPOS(diag_qshrink_4_database_file_retrieve_rsp_type, operation_data);
  int i;
  int guid_count = diag_qshrink4_guid_count;      
  boolean call_rfs = FALSE;
  int num_files = 0;
  boolean rfs_timeout_flag = FALSE;
  int result = -1;  
  uint32 file_size;
  boolean found = FALSE;
  struct rfs_stat_buf stat_buf;
  
  if(version==DIAG_QSHRINK_CMD_VERSION_3)
    rsp_len += FPOS(diag_qshrink_4_file_list_rsp_v3_type, file_info);
  else
    rsp_len += FPOS(diag_qshrink_4_file_list_rsp_type, file_info);

  for (i = 0; i < guid_count; i++)
  {
    if (strlen(diag_qshrink_database_file_list[i].file_name) != 0)
    {
      num_files++;
      if(version==DIAG_QSHRINK_CMD_VERSION_3)
        rsp_len += sizeof(diag_qshrink_4_file_info_v3_type);
      else
        rsp_len += sizeof(diag_qshrink_4_file_info_type);        
    }

  }

  /* Create immediate response, which will not contain file size information.
   * This is to avoid stalling due to potential RFS timeouts */
  if(!delayed_flag)
  {
    rsp = (diag_qshrink_4_database_file_retrieve_rsp_type *)
        diagpkt_subsys_alloc_v2(DIAG_SUBSYS_DIAG_SERV, DIAGDIAG_QSHRINK_4_RETRIEVE_DATABASE_MODEM,
          rsp_len);
  }
  else
  {
    rsp = (diag_qshrink_4_database_file_retrieve_rsp_type *)diagpkt_subsys_alloc_v2_delay(DIAG_SUBSYS_DIAG_SERV, DIAGDIAG_QSHRINK_4_RETRIEVE_DATABASE_MODEM, delayed_rsp_id, rsp_len);
  }
  
  if (rsp == NULL)
  {
    ULOG_RT_PRINTF_0( diag_err_log_handle,"Error in diag_qshrink4_file_list_handler. Failure to allocate rsp from heap");
    return (rsp);
  }

  /* Need to version this and adjust accordingly
   * v1 = immedaite response with RFS calls
   * v2 = delayed response (two responses)
   */

  rsp->version = version;
  rsp->op_code = DIAG_QSHRINK_4_FILE_LIST;
  rsp->operation_data.file_list_v3_rsp.num_files = num_files;  

  if(version == DIAG_QSHRINK_CMD_VERSION_1)
  {
    rsp->header.delayed_rsp_id = 0; // '0' indicates single response (immediate)
    rsp->header.rsp_cnt = 0;        // '0' indicates single response (immediate)
    call_rfs = TRUE;
  }
  else /* v2, v3 */
  {
    if(!delayed_flag)
    {
      rsp->header.rsp_cnt = 0; /* First (immediate) response */
      call_rfs = FALSE;        /* No RFS on first response   */
    }
    else
    {
      rsp->header.rsp_cnt = 1; /* Second and final response    */
      call_rfs = TRUE;         /* Call RFS for second response */
    }
  }

  /* Prepare response */
  for (i = 0; i < guid_count; i++)
  {
    if (strlen(diag_qshrink_database_file_list[i].file_name) != 0)
    {
      if(version==DIAG_QSHRINK_CMD_VERSION_3)
      {
        memscpy((void *)rsp->operation_data.file_list_v3_rsp.file_info[i].guid, QSHRINK_4_DATABASE_GUID_LENGTH, diag_qshrink_database_file_list[i].guid, QSHRINK_4_DATABASE_GUID_LENGTH);
        rsp->operation_data.file_list_v3_rsp.file_info[i].file_len = 0;
        rsp->operation_data.file_list_v3_rsp.status = DIAG_QS4_SUCCESS;
        /* Use the guid to get the diagID, only for v3 commands */
        rsp->operation_data.file_list_v3_rsp.file_info[i].diagID = diag_get_diagID_from_GUID(diag_qshrink_database_file_list[i].guid);
      }
      else
      {
        memscpy((void *)rsp->operation_data.file_list_rsp.file_info[i].guid, QSHRINK_4_DATABASE_GUID_LENGTH, diag_qshrink_database_file_list[i].guid, QSHRINK_4_DATABASE_GUID_LENGTH);
        rsp->operation_data.file_list_rsp.file_info[i].file_len = 0;
        rsp->operation_data.file_list_rsp.status = DIAG_QS4_SUCCESS;        
      }

      /* Only call RFS if call_rfs flag is set
       (part of immediate response for v1, but delayed response for v2+) */

      if(call_rfs)
      {
        /* get file len info by using rfs apis*/
        if(!rfs_timeout_flag)
        {
          result = rfs_stat(diag_qshrink_database_file_list[i].file_name, &stat_buf);
          
          /* Check for RFS Timeout to avoid repeated calls */
          if(
            (result == RFS_ETIMEOUT) ||
            (result == RFS_ENODEV)
            )
          {
            rfs_timeout_flag = TRUE;
          }
        }
        
        if (result == 0)
        {
          file_size = (uint32)stat_buf.st_size;

          if(version == DIAG_QSHRINK_CMD_VERSION_3)
            rsp->operation_data.file_list_v3_rsp.file_info[i].file_len = file_size;
          else
            rsp->operation_data.file_list_rsp.file_info[i].file_len = file_size;          
          diag_qshrink_database_file_list[i].file_len = file_size;
        }
        found = TRUE;
      }
    }
  }

  if ((call_rfs == TRUE) && (found == FALSE))
  {
    if(version == DIAG_QSHRINK_CMD_VERSION_3)    
      rsp->operation_data.file_list_v3_rsp.status = DIAG_QS4_ENOENT;
    else
      rsp->operation_data.file_list_rsp.status = DIAG_QS4_ENOENT;    
  }

  if(((version == DIAG_QSHRINK_CMD_VERSION_2) || (version == DIAG_QSHRINK_CMD_VERSION_3)) && (!delayed_flag))
  {
    /* Check that number of processing threads hasn't reached the maximum
     * If not, send response and create thread.
     * If it has, change the response to error and do not spawn a thread.
     */
    
    if(diag_qshrink_4_task_cnt <= DIAG_MAX_QSHRINK_4_TASK_CNT)
    {
      /* Okay to create the thread and send response as-is */
      int rsp_id = rsp->header.delayed_rsp_id;
      diagpkt_commit(rsp);
      rsp = NULL;
      qshrink4_create_delayed_thread(version, rsp_id, 0, 0, 0, DIAG_QSHRINK_4_FILE_LIST);
    }
    else
    {
      /* Do not create the thread; change response to immediate/error */
      rsp->header.delayed_rsp_id = 0; // '0' indicates single response (immediate)
      rsp->header.rsp_cnt = 0;        // '0' indicates single response (immediate)
      rsp->operation_data.file_list_rsp.status = DIAG_QS4_EAGAIN;
      
      MSG(MSG_SSID_DIAG, MSG_LEGACY_ERROR, "Maximum task limit reached, sending error for Qshrink4 File List request");      
    }
  }

  return (void*)rsp;
}


/*===========================================================================

FUNCTION DIAG_QSHRINK4_FILE_CLOSE_HANDLER

DESCRIPTION
  Process Qshrink4 FILE_CLOSE database commands. Supports immediate and delayed
  responses, and version 1 and 2 of the command.
  
PARAMETERS
  version - version of the command
  delayed_flag - TRUE if a delayed reponse, FALSE is it is the first response
  delayed_rsp_id - ID to use for delayed responses
  fd - file descriptior of the Qshrink file to close
  
DEPENDENCIES
  None
  
RETURN VALUE
  Pointer to response packet
  
===========================================================================*/
static void* diag_qshrink4_file_close_handler(uint16 version, boolean delayed_flag, uint32 delayed_rsp_id, int fd)
{
  diag_qshrink_4_database_file_retrieve_rsp_type *rsp = NULL;
  int rsp_len = FPOS(diag_qshrink_4_database_file_retrieve_rsp_type, operation_data);
  int i;
  int guid_count = diag_qshrink4_guid_count;      
  boolean call_rfs = FALSE;
  boolean found_fd = FALSE;
  int fd_index = 0;
  int result = -1;

  rsp_len += sizeof(diag_qshrink_4_file_close_rsp_type);
  
  if(!delayed_flag)
  {
    rsp = (diag_qshrink_4_database_file_retrieve_rsp_type *)
        diagpkt_subsys_alloc_v2(DIAG_SUBSYS_DIAG_SERV, DIAGDIAG_QSHRINK_4_RETRIEVE_DATABASE_MODEM,
          rsp_len);
  }
  else
  {
    rsp = (diag_qshrink_4_database_file_retrieve_rsp_type *)diagpkt_subsys_alloc_v2_delay(DIAG_SUBSYS_DIAG_SERV, DIAGDIAG_QSHRINK_4_RETRIEVE_DATABASE_MODEM, delayed_rsp_id, rsp_len);
  }
  
  if (rsp == NULL)
  {
    ULOG_RT_PRINTF_0( diag_err_log_handle,"Error in diag_qshrink4_file_close_handler. Failure to allocate rsp from heap");    
    return (rsp);
  }

  rsp->version = version;
  rsp->op_code = DIAG_QSHRINK_4_FILE_CLOSE;
  
  if(version == DIAG_QSHRINK_CMD_VERSION_1)
  {
    rsp->header.delayed_rsp_id = 0; /* '0' indicates single response (immediate) */
    rsp->header.rsp_cnt = 0;        /* '0' indicates single response (immediate) */
    call_rfs = TRUE;                /* We will do the RFS call for the immediate response */
  }
  else /* v2 packet */
  {
    if(!delayed_flag)
    {
      rsp->header.rsp_cnt = 0; /* First (immediate) response */
      call_rfs = FALSE;        /* No RFS on first response   */
    }
    else
    {
      rsp->header.rsp_cnt = 1; /* Second and final response    */
      call_rfs = TRUE;         /* Call RFS for second response */
    }
  }

  /* Search for the FD in the GUID array */
  for (i = 0; i < guid_count; i++)
  {
    if (diag_qshrink_database_file_list[i].fd == fd)
    {
      found_fd = TRUE;
      fd_index = i;
      break;
    }
  }

  if(found_fd)
  {
    rsp->operation_data.file_close_rsp.status = DIAG_QS4_SUCCESS;
    rsp->operation_data.file_close_rsp.fd = fd;
    if(call_rfs)
    {
      result = rfs_close(fd);
      if (result == 0)
      {
        diag_qshrink_database_file_list[fd_index].fd = -1;      
      }
      else
      {
        rsp->operation_data.file_close_rsp.status = qshrink_4_error_code(result);
      }
    }
  }
  else
  {
    /* No match, so return bad file descriptor error */
    rsp->operation_data.file_close_rsp.status = DIAG_QS4_EBADF;
    rsp->operation_data.file_close_rsp.fd = 0;
    if(!delayed_flag)
    {
      rsp->header.delayed_rsp_id = 0; /* '0' indicates single response (immediate) */
      rsp->header.rsp_cnt = 0;        /* '0' indicates single response (immediate) */
    }
  }
  
  if((version == DIAG_QSHRINK_CMD_VERSION_2) && (!delayed_flag))
  {
    /* Check that number of processing threads hasn't reached the maximum
     * If not, send response and create thread.
     * If it has, change the response to error and do not spawn a thread.
     */
    if(diag_qshrink_4_task_cnt <= DIAG_MAX_QSHRINK_4_TASK_CNT)
    {
      /* Okay to create the thread and send response as-is */
      int rsp_id = rsp->header.delayed_rsp_id;
      diagpkt_commit(rsp);
      rsp = NULL;
      qshrink4_create_delayed_thread(version, rsp_id, fd, 0, 0, DIAG_QSHRINK_4_FILE_CLOSE);      
    }
    else
    {
      /* Do not create the thread; change response to immediate/error */
      rsp->header.delayed_rsp_id = 0; // '0' indicates single response (immediate)
      rsp->header.rsp_cnt = 0;        // '0' indicates single response (immediate)
      rsp->operation_data.file_close_rsp.status = DIAG_QS4_EAGAIN;
      MSG(MSG_SSID_DIAG, MSG_LEGACY_ERROR, "Maximum task limit reached, sending error for Qshrink4 File Close request");
    }
  }

  
  return rsp;
}


/*===========================================================================

FUNCTION DIAG_QSHRINK4_FILE_OPEN_HANDLER

DESCRIPTION
  Process Qshrink4 FILE_OPEN database commands. Supports immediate and delayed
  responses, and version 1 and 2 of the command.
  
PARAMETERS
  version - version of the command
  delayed_flag - TRUE if a delayed reponse, FALSE is it is the first response
  delayed_rsp_id - ID to use for delayed responses
  guid_index - index to the entry in the qshrink database array
  guid_ptr - poitner to the GUID value 
  
DEPENDENCIES
  None
  
RETURN VALUE
  Pointer to response packet
  
===========================================================================*/
static void* diag_qshrink4_file_open_handler(uint16 version, boolean delayed_flag, uint32 delayed_rsp_id, uint32 guid_index, diag_guid_type *guid_ptr)
{
  diag_qshrink_4_database_file_retrieve_rsp_type *rsp = NULL;
  int rsp_len = FPOS(diag_qshrink_4_database_file_retrieve_rsp_type, operation_data);
  int i;
  int fd = -1;
  int guid_count = diag_qshrink4_guid_count;      
  boolean call_rfs = FALSE;
  boolean found = FALSE;
  
  rsp_len += sizeof(diag_qshrink_4_file_open_rsp_type);
  
  if(!delayed_flag)
  {
    rsp = (diag_qshrink_4_database_file_retrieve_rsp_type *)
        diagpkt_subsys_alloc_v2(DIAG_SUBSYS_DIAG_SERV, DIAGDIAG_QSHRINK_4_RETRIEVE_DATABASE_MODEM,
          rsp_len);
  }
  else
  {
    rsp = (diag_qshrink_4_database_file_retrieve_rsp_type *)diagpkt_subsys_alloc_v2_delay(DIAG_SUBSYS_DIAG_SERV, DIAGDIAG_QSHRINK_4_RETRIEVE_DATABASE_MODEM, delayed_rsp_id, rsp_len);
  }
  
  if (rsp == NULL)
  {
    ULOG_RT_PRINTF_0( diag_err_log_handle,"Error in diag_qshrink4_file_open_handler. Failure to allocate rsp from heap");        
    return (rsp);
  }

  
  if(guid_ptr == NULL)
  {
    if(guid_index < guid_count) 
      guid_ptr = (diag_guid_type*)&(diag_qshrink_database_file_list[guid_index].guid);
    else
    {
      /* If we hit this case then we didn't call the function correctly */
      ASSERT(TRUE);
      return NULL;
    }
  }  

  /* Copy GUID to response packet */
  memscpy(rsp->operation_data.file_open_rsp.guid, QSHRINK_4_DATABASE_GUID_LENGTH,
    guid_ptr, QSHRINK_4_DATABASE_GUID_LENGTH);  
  
  /* Need to version this and adjust accordingly
   * v1 = immedaite response with RFS calls
   * v2 = delayed response (two responses)
   */

  rsp->version = version;
  rsp->op_code = DIAG_QSHRINK_4_FILE_OPEN;
  /* need to add the guid here */
  
  if(version == DIAG_QSHRINK_CMD_VERSION_1)
  {
    rsp->header.delayed_rsp_id = 0; /* '0' indicates single response (immediate) */
    rsp->header.rsp_cnt = 0;        /* '0' indicates single response (immediate) */
    call_rfs = TRUE;                /* We will do the RFS call for the immediate response */
  }
  else /* v2 packet */
  {
    if(!delayed_flag)
    {
      rsp->header.rsp_cnt = 0; /* First (immediate) response */
      call_rfs = FALSE;        /* No RFS on first response   */
    }
    else
    {
      rsp->header.rsp_cnt = 1; /* Second and final response    */
      call_rfs = TRUE;         /* Call RFS for second response */
    }
  }

  if(!delayed_flag)
  {
    /* Perform a GUID search on initial response */
    for (i = 0; i < guid_count; i++)
    {
      if (memcmp(guid_ptr, diag_qshrink_database_file_list[i].guid,
          QSHRINK_4_DATABASE_GUID_LENGTH) == 0)
      {
        found = TRUE;  // match found for the GUID 
        guid_index = i;
        rsp->operation_data.file_open_rsp.status = DIAG_QS4_SUCCESS;
        rsp->operation_data.file_open_rsp.fd = 0;      
        break;
      }
    }
  }
  else
  {
    /* For delayed response, we already know the index of the matching database (guid_index). */
    found = TRUE;
    rsp->operation_data.file_open_rsp.status = DIAG_QS4_SUCCESS;
    rsp->operation_data.file_open_rsp.fd = 0; 
  }

  if(found && call_rfs)
  {
    /* Check to see if file is already open. If so, re-use the file descriptor */
    fd = diag_qshrink_database_file_list[guid_index].fd;
    if (fd == -1)
    {
      /* Open the file and return the assigned file descriptor */
      fd = rfs_open(diag_qshrink_database_file_list[guid_index].file_name, RFS_O_RDONLY, 0);      
    }

    if (fd >= 0)
    {
      rsp->operation_data.file_open_rsp.fd = fd;
      diag_qshrink_database_file_list[guid_index].fd = fd;
    }
    else
    {
      rsp->operation_data.file_open_rsp.status = DIAG_QS4_EIO;  /* open file failed */
    }
  }

  if (!found)
  {
    rsp->operation_data.file_open_rsp.status = DIAG_QS4_ENOENT; //cannot find this database file
    /* Ensure rsp flags are set correctly. There will be no second response, even for v2. */
    rsp->header.delayed_rsp_id = 0; /* '0' indicates single response (immediate) */
    rsp->header.rsp_cnt = 0;        /* '0' indicates single response (immediate) */
  }    
  
  if((version == DIAG_QSHRINK_CMD_VERSION_2) && (!delayed_flag))
  {
    /* Check that number of processing threads hasn't reached the maximum
     * If not, send response and create thread.
     * If it has, change the response to error and do not spawn a thread.
     */    
    if(diag_qshrink_4_task_cnt <= DIAG_MAX_QSHRINK_4_TASK_CNT)
    {
      /* Okay to create the thread and send response as-is */
      int rsp_id = rsp->header.delayed_rsp_id;
      diagpkt_commit(rsp);
      rsp = NULL;
      qshrink4_create_delayed_thread(version, rsp_id, guid_index, 0, 0, DIAG_QSHRINK_4_FILE_OPEN);      
    }
    else
    {
      /* Do not create the thread; change response to immediate/error */
      rsp->header.delayed_rsp_id = 0; // '0' indicates single response (immediate)
      rsp->header.rsp_cnt = 0;        // '0' indicates single response (immediate)
      rsp->operation_data.file_close_rsp.status = DIAG_QS4_EAGAIN;
      MSG(MSG_SSID_DIAG, MSG_LEGACY_ERROR, "Maximum task limit reached, sending error for Qshrink4 File Open request");
    }
  }
  
  return rsp;
}


/*===========================================================================

FUNCTION DIAG_QSHRINK4_FILE_READ_HANDLER

DESCRIPTION
  Process Qshrink4 FILE_READ database commands. This function supports
  the immediate response.
  
PARAMETERS
  version - version of the command
  fd - file descriptor
  offset - offset into the database
  requested_bytes - length to be read
   
DEPENDENCIES
  None
  
RETURN VALUE
  Pointer to response packet
  
===========================================================================*/
static void* diag_qshrink4_file_read_handler(uint16 version, int fd, uint32 offset, uint32 requested_bytes)
{
  diag_qshrink_4_database_file_retrieve_rsp_type *rsp = NULL;
  int rsp_len = FPOS(diag_qshrink_4_database_file_retrieve_rsp_type, operation_data);
  int i;
  int guid_count = diag_qshrink4_guid_count;      
  boolean found_fd = FALSE;

  /* Create immediate response to file read in this function.
     Delayed resposnes will be handled by a separate task and function.
   */

  rsp_len += FPOS(diag_qshrink_4_file_read_rsp_type, data_block);    

  rsp = (diag_qshrink_4_database_file_retrieve_rsp_type *)
      diagpkt_subsys_alloc_v2(DIAG_SUBSYS_DIAG_SERV, DIAGDIAG_QSHRINK_4_RETRIEVE_DATABASE_MODEM,
        rsp_len);

  if (rsp == NULL)
  {
    ULOG_RT_PRINTF_0( diag_err_log_handle,"Error in diag_qshrink4_file_read_handler. Failure to allocate rsp from heap");            
    return (rsp);
  }

  rsp->version = version;
  rsp->op_code = DIAG_QSHRINK_4_FILE_READ;
  rsp->operation_data.file_read_rsp.fd = fd;
  rsp->operation_data.file_read_rsp.offset = offset;
  rsp->operation_data.file_read_rsp.num_read = 0;
  rsp->operation_data.file_read_rsp.status = DIAG_QS4_SUCCESS;

  if (requested_bytes > QSHRINK_DATABASE_READ_DATABLOCK_SIZE)
  {
    rsp->header.rsp_cnt = 0x8000; /* There will be more than one delayed response */
  }
  else
  {
    rsp->header.rsp_cnt = 0;      /* There will be only one delayed response */
  }

  /* Search for the FD in the GUID array */
  for (i = 0; i < guid_count; i++)
  {
    if (diag_qshrink_database_file_list[i].fd == fd)
    {
      found_fd = TRUE;
      break;
    }
  }

  if(!found_fd)
  {
    /* File descriptor not found */
    rsp->operation_data.file_read_rsp.status = DIAG_QS4_EBADF;
    rsp->header.delayed_rsp_id = 0;
    rsp->header.rsp_cnt = 0;    
  }
  else
  {
    /* Check that number of processing threads hasn't reached the maximum
     * If not, send response and create thread.
     * If it has, change the response to error and do not spawn a thread.
     */      
    if(diag_qshrink_4_task_cnt <= DIAG_MAX_QSHRINK_4_TASK_CNT)
    {
      /* Okay to create the thread and send response as-is */
      int rsp_id = rsp->header.delayed_rsp_id;
      diagpkt_commit(rsp);
      rsp = NULL;
      qshrink4_create_delayed_thread(version, rsp_id, fd, offset,
        requested_bytes, DIAG_QSHRINK_4_FILE_READ);      
    }
    else
    {
      /* Do not create the thread; change response to immediate/error */
      rsp->header.delayed_rsp_id = 0; // '0' indicates single response (immediate)
      rsp->header.rsp_cnt = 0;        // '0' indicates single response (immediate)
      rsp->operation_data.file_read_rsp.status = DIAG_QS4_EAGAIN;
      MSG(MSG_SSID_DIAG, MSG_LEGACY_ERROR, "Maximum task limit reached, sending error for Qshrink4 File Read request");
    }
  }

  return rsp;
}

/*===========================================================================

FUNCTION DIAG_QSHRINK_4_CMD_SIZE

DESCRIPTION
  This helper function returns the expected size of a command packet for
  a given op code

RETURN VALUE
  Expected size (in bytes) of the command packet
============================================================================*/

static int diag_qshrink_4_cmd_size(uint16 op_code)
{
  int size = FPOS(diag_qshrink_4_database_file_retrieve_req_type, operation_data);
  /* With v1 and v2, the command sizes are identical */
  switch(op_code)
  {
    case DIAG_QSHRINK_4_FILE_LIST:
      break;
    case DIAG_QSHRINK_4_FILE_OPEN:
      size += sizeof(diag_qshrink_4_file_open_req_type);
      break;
    case DIAG_QSHRINK_4_FILE_READ:
      size += sizeof(diag_qshrink_4_file_read_req_type);
      break;
    case DIAG_QSHRINK_4_FILE_CLOSE:
      size += sizeof(diag_qshrink_4_file_close_req_type);
      break;
  }
  return size;
}


/*===========================================================================

FUNCTION DIAG_QSHRINK_4_DATABASE_RETRIEVAL_CMD

DESCRIPTION
  This procedure process the qshrink 4 database retrieval command.

RETURN VALUE
  Pointer to response packet.

============================================================================*/
void*
diag_qshrink_4_database_retrieval_cmd(void *req_pkt, uint16 pkt_len)
{
  diag_qshrink_4_database_file_retrieve_rsp_type *rsp = NULL;
  diag_qshrink_4_database_file_retrieve_req_type *req = (diag_qshrink_4_database_file_retrieve_req_type *)req_pkt;
  int expected_size=0;
  
  if (pkt_len < FPOS(diag_qshrink_4_database_file_retrieve_req_type, operation_data))
  {
    return (diagpkt_err_rsp(DIAG_BAD_LEN_F, req_pkt, pkt_len));
  }
  if ((req->version != DIAG_QSHRINK_CMD_VERSION_1) && (req->version != DIAG_QSHRINK_CMD_VERSION_2) && (req->version != DIAG_QSHRINK_CMD_VERSION_3))
  {
    return (diagpkt_err_rsp(DIAG_BAD_PARM_F, req_pkt, pkt_len));
  }

  /* Check validity of command length (based on op code) */
  expected_size = diag_qshrink_4_cmd_size(req->op_code);
  if(pkt_len != expected_size)
  {
    return (diagpkt_err_rsp(DIAG_BAD_LEN_F, req_pkt, pkt_len));
  }

  switch (req->op_code)
  {
    case DIAG_QSHRINK_4_FILE_LIST:
      rsp = diag_qshrink4_file_list_handler(req->version,FALSE,0);
      break;

    case DIAG_QSHRINK_4_FILE_OPEN:
      rsp = diag_qshrink4_file_open_handler(req->version,FALSE,0,0,
        (diag_guid_type*)&(req->operation_data.file_open_req.guid));
      break;

    case DIAG_QSHRINK_4_FILE_READ:
      rsp = diag_qshrink4_file_read_handler(req->version,
        req->operation_data.file_read_req.fd,
        req->operation_data.file_read_req.offset,
        req->operation_data.file_read_req.requested_bytes);
      break;
    
    case DIAG_QSHRINK_4_FILE_CLOSE:
      rsp = diag_qshrink4_file_close_handler(req->version,FALSE,0,
        req->operation_data.file_close_req.fd);
      break;

    default:
      /* Return error for unrecognized op_code */
      rsp = diagpkt_err_rsp(DIAG_BAD_PARM_F, req_pkt, pkt_len);
  }      

  return (rsp);

 }  /* diag_qshrink_4_database_retrieval_cmd */


static const diagpkt_user_table_entry_type diagdiag_qshrink4_tbl[] =
{
  {DIAGDIAG_QSHRINK_4_RETRIEVE_DATABASE_MODEM, DIAGDIAG_QSHRINK_4_RETRIEVE_DATABASE_MODEM, diag_qshrink_4_database_retrieval_cmd}
};



/*===========================================================================

FUNCTION DIAG_QSHRINK4_INIT

DESCRIPTION
  This function performs the steps necesary to initialize Qshrink4 processing,
  including the following:
    - Initialize mutex for Qshrink4 registration
    - Register Qshrink4 command handlers
    - Register the rootPD Qshrink4 GUID and database 

RETURN VALUE
  None

============================================================================*/

void diag_qshrink4_init(void)
{
  int return_val;
  diag_qshrink_4_database_file_type core_entry;

  /* Initialize the queue */
  (void)q_init(&diag_qshrink_4_task_q);
  
  DIAGPKT_DISPATCH_TABLE_REGISTER_V2_DELAY_PROC(DIAG_MODEM_PROC, DIAG_SUBSYS_CMD_VER_2_F,
    DIAG_SUBSYS_DIAG_SERV, diagdiag_qshrink4_tbl);
  
  diag_qshrink4_database_mutex.name = "MUTEX_DIAG_QSHRINK4_DATABASE_CS";
  return_val = osal_init_mutex(&diag_qshrink4_database_mutex);
  ASSERT(OSAL_SUCCESS == return_val);

  /* update modem qshrink4 database file information */
  memset((char*)&core_entry,0,sizeof(diag_qshrink_4_database_file_type));
  strlcpy(core_entry.file_name,(char*)msg_v4_dbase_name,MAX_MSG_V4_DBASE_NAME_SIZE);
  memscpy ((void *)(&core_entry.guid),QSHRINK_4_DATABASE_GUID_LENGTH ,
    msg_v4_dbase_guid, QSHRINK_4_DATABASE_GUID_LENGTH);  
  core_entry.fd = -1;
  core_entry.file_len = 0;

  /* 'updated' is a flag to signify if PD notification has been sent upstream
     to the AP processor. This step is unneccesary for the core_entry (AP
     detects core by the opening of the diag channels), so it will be set to
     TRUE so diag task does not unnecesasrily signal the AP for thsi entry */
  core_entry.updated = TRUE; 

  diag_qshrink4_register_entry(&core_entry);
}
