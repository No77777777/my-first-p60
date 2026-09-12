/*=============================================================================

                 time_genoff_qdi.c

GENERAL DESCRIPTION
   Implements QDI layer for Time services
   
INITIALIZATION AND SEQUENCING REQUIREMENTS
   None.

      Copyright (c) 2010 - 2017
   by QUALCOMM Technologies Incorporated.  All Rights Reserved.

=============================================================================*/


/*=============================================================================

                        EDIT HISTORY FOR MODULE

 This section contains comments describing changes made to the module.
 Notice that changes are listed in reverse chronological order.
 
$Header: //components/rel/core.mpss/10.0/services/time/src/time_genoff_qdi.c#1 $ 
$DateTime: 2019/04/24 00:03:26 $ $Author: pwbldsvc $

when         who     what, where, why
--------   ---     ------------------------------------------------------------
09/01/15   abh     Changes for correctly handling ATS_PRIVATE bases.
05/01/14   abh     File Created

=============================================================================*/


/*****************************************************************************/
/*                           INCLUDE FILES                                   */
/*****************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <stringl/stringl.h>
#include "qurt.h"
#include "qurt_qdi.h"
#include "qurt_qdi_driver.h"
#include "qurt_pimutex.h"
#include "time_genoff_qdi_v.h"
#include "msg.h"
#include "err.h"

/*===========================================================================
Data Declarations
===========================================================================*/

/* Pointers to the qdi_structure for user PD client */
static time_genoff_qdi_opener *time_genoff_clients[TIME_GENOFF_QDI_MAX_CLIENTS];

/* Counter for the number of user PD clients */
uint32 time_genoff_clients_cntr = 0;


/* Mutex for use in time QDI related operations */
time_osal_mutex_t   time_genoff_qdi_mutex;


/*===========================================================================
Declarations
===========================================================================*/
static int time_genoff_qdi_invoke(int client_handle,
                          qurt_qdi_obj_t *pobj,
                          int method,
                          qurt_qdi_arg_t a1,
                          qurt_qdi_arg_t a2,
                          qurt_qdi_arg_t a3,
                          qurt_qdi_arg_t a4,
                          qurt_qdi_arg_t a5,
                          qurt_qdi_arg_t a6,
                          qurt_qdi_arg_t a7,
                          qurt_qdi_arg_t a8,
                          qurt_qdi_arg_t a9);
                          


/*===========================================================================
Pipe Apis
===========================================================================*/
int time_genoff_qdi_create_pipe(time_genoff_qdi_opener *clntobj)
{
   qurt_pipe_attr_t pipe_attr;
   int ret;
   
   //Create Pipe for the pid
   qurt_pipe_attr_init(&pipe_attr);
   qurt_pipe_attr_set_elements(&pipe_attr, TIME_GENOFF_QDI_CLIENT_PIPE_SIZE);
   ret = qurt_pipe_create(&clntobj->pipe, &pipe_attr);

   return ret;
} /* time_genoff_qdi_create_pipe */


/*===========================================================================
Function Definitions
===========================================================================*/


/*=============================================================================

FUNCTION TIME_GENOFF_UPDATE_TO_PIPE

DESCRIPTION
 Updates the passed structure pointer to the pipe for respective client PD object.

DEPENDENCIES
  None.

RETURN VALUE
 None.

SIDE EFFECTS
 None.

=============================================================================*/
void time_genoff_update_to_pipe(time_genoff_ptr ptime_genoff, int client_index)
{
   int index = 0;

   qurt_pipe_data_t data = (qurt_pipe_data_t)0;

   data = (uint32)ptime_genoff;

   if (client_index == -1)
    {
     for (index = 0 ; index<time_genoff_clients_cntr ; index++)
      {
        qurt_pipe_send(time_genoff_clients[index]->pipe,data);
      }
    }
   else if (client_index < time_genoff_clients_cntr)
   	{
      qurt_pipe_send(time_genoff_clients[client_index]->pipe,data);
   	}
   return;
} /* time_genoff_qdi_report_out_of_coverage */

/*=============================================================================

FUNCTION TIME_GENOFF_QDI_REPORT_OUT_OF_COVERAGE

DESCRIPTION
 For reporting out of coverage.

DEPENDENCIES
  None

RETURN VALUE
 None

SIDE EFFECTS
 None

=============================================================================*/
static inline int time_genoff_qdi_report_out_of_coverage(time_bases_type base)
{
   time_genoff_report_out_of_coverage(base);
   return 0;
} /* time_genoff_qdi_report_out_of_coverage */


/*=============================================================================

FUNCTION TIME_GENOFF_QDI_GET_UPDATE_INFO

DESCRIPTION
  Function invoked by client PD requesting the updates to time offsets.
 
 
DEPENDENCIES
  None

RETURN VALUE
  Error value chosen as per the operation.

SIDE EFFECTS
  None

=============================================================================*/
static inline int time_genoff_qdi_get_update_info( int client_handle, 
                                            time_genoff_qdi_opener * client_obj,
                                            time_genoff_qdi_update_container * remote_pd_ptr )
{
  /* return value from copy_to_user calls */
  int                            result;

  /* Container for the pipe_receive */
  qurt_pipe_data_t               data;
  uint32                         data_val;

  /* Pointer to the genoff base for which the update needs to be passed */
  time_genoff_ptr                ptime_genoff;

  /* Temp container for the client genoff update values */
  time_genoff_qdi_update_container temp_update_container = { 0 };
      
  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
  
   /* Receive the pointer for time base updated information from the GUEST PD */
   if( qurt_pipe_receive_cancellable(client_obj->pipe, &data) == QURT_ECANCEL )
   {
      return TIME_ERR_CANCEL_WAIT;
   }   

   /* Pointer to the genoff from which update needs to be copied */
   data_val = (uint32)data;    // Suppressing compiler warning
   ptime_genoff = (time_genoff_ptr)(data_val);

   /* Pipe data check */
   if (ptime_genoff == NULL)
   	  return -1;
  
   /* Populate the temp update container structure */
   temp_update_container.base =    ptime_genoff->bases_type;   
   temp_update_container.time_genoff_client.generic_offset = 
                                   ptime_genoff->generic_offset;
   temp_update_container.time_genoff_client.sum_generic_offset = 
   	                               ptime_genoff->sum_generic_offset;
   temp_update_container.time_genoff_client.sum_generic_offset_ts_is_negative = 
   	                               ptime_genoff->sum_generic_offset_ts_is_negative;
   qw_equ(temp_update_container.time_genoff_client.sum_generic_offset_ts,
   	                               ptime_genoff->sum_generic_offset_ts);

   /* Copy values to the remote PD */
   result = qurt_qdi_copy_to_user(client_handle, 
                                  remote_pd_ptr, 
                                  &temp_update_container, 
                                  sizeof(time_genoff_qdi_update_container));
   /* Check for successful copy */
   if(result < 0)
   {
      return TIME_ERR_QDI_CPY_FAIL;
   }								  


   /* Return SUCCESS */
   return TIME_ERR_NONE;   

} /* time_genoff_qdi_get_update_info */

/*=============================================================================

                   TIME GENOFF API QDI Functions

=============================================================================*/
static inline int time_genoff_qdi_opr(int                  client_handle,
	                                     time_genoff_args_ptr  pargs)
{
   int result;
   time_genoff_args_struct_type temp_args;

   /* Copy args structure locally to pass to time_genoff_opr */  
   result = qurt_qdi_copy_from_user(client_handle, 
   	                                &temp_args,
   	                                pargs,
   	                                sizeof(time_genoff_args_struct_type));
   if(result)
     return result;

   /* Call the root PD time_genoff_opr API. */
   time_genoff_opr(&temp_args);

   /* We don't need to copy back the pargs structure
       * from root Pd to User PD, since this QDI call to
       * root PD will only be called in case of SET operation.
       */
 	   
   return result;
} /* time_genoff_qdi_opr */


/*=============================================================================

FUNCTION TIME_GENOFF_QDI_GET_INIT_UPDATES

DESCRIPTION
  This will be called by the client PD when it's initialization finishes. 
  Once called, this function will pass the update information from Guest OS's ats_bases to the inited client PD. 
 
DEPENDENCIES
  None

RETURN VALUE
  Zero.

SIDE EFFECTS
  None

=============================================================================*/

static inline int time_genoff_qdi_get_init_updates( int client_index )
      	{
   time_genoff_get_init_updates(client_index);
   return 0;
     	}
	  
/*=============================================================================

FUNCTION TIME_GENOFF_QDI_PRIVATE_BASE_INIT

DESCRIPTION
  Completes the initialization of external genoff base in the root PD.
 
DEPENDENCIES
  None

RETURN VALUE
  Assigned private base index number from root PD.

SIDE EFFECTS
  None

=============================================================================*/

static inline int time_genoff_qdi_private_base_init(int client_handle,
                                              char * f_name,
                                              uint32 * ret_base_pd,
                                              time_genoff_client_ptr ptime_genoff,
                                              unsigned int process_idx)
      	{
   int result;
   uint32    ret_base;
   char      f_name_local[MAX_EFS_FILE_NAME_SIZE] = {0};
   time_genoff_client_struct_type temp_ptime_genoff;
   time_genoff_ptr ptime_genoff_ext;
   ats_error_type err_ret = ATS_FAILURE;
	  
   /* Copy args structure locally to pass to time_genoff_opr */  
	  result = qurt_qdi_copy_from_user(client_handle, 
   	                                &f_name_local[0],
   	                                f_name,
   	                                MAX_EFS_FILE_NAME_SIZE);

   /* Check if the copy happened successfully */
	  if(result)
     return (int)ATS_QDI_COPY_FAILED;

   /* Call the time genoff private base init function */
   if (ATS_SUCCESS != (err_ret = time_genoff_private_base_init(f_name_local,&ret_base)))
	  	{
   	  return (int)err_ret;
	  	}

   /* Get pointer to the external genoff structure */
   ptime_genoff_ext = time_genoff_get_pointer(ret_base);

  if(ptime_genoff_ext == NULL)
	  return 0;

   /* Copy the offset values to the temp_ptime_genoff structure */
   temp_ptime_genoff.generic_offset = ptime_genoff_ext->generic_offset;
   temp_ptime_genoff.sum_generic_offset = ptime_genoff_ext->sum_generic_offset;
   qw_equ(temp_ptime_genoff.sum_generic_offset_ts,
   	                               ptime_genoff_ext->sum_generic_offset_ts);
   temp_ptime_genoff.sum_generic_offset_ts_is_negative = ptime_genoff_ext->sum_generic_offset_ts_is_negative;

   /* Copy values to the remote PD */
   result = qurt_qdi_copy_to_user(client_handle, 
                                  ptime_genoff, 
                                  &temp_ptime_genoff, 
                                  sizeof(time_genoff_client_struct_type));

   /* Check if the copy happened successfully */
   if(result < 0)
   {
      /* De-Init the external time genoff base */
      (void)time_genoff_private_base_deinit(ret_base,f_name_local);
      return (int)ATS_QDI_COPY_FAILED;
   }

   /* Assign the PD number */
   if (ATS_SUCCESS != time_genoff_ext_add_pd_num(ret_base,process_idx))
  {
     /* De-Init the external time genoff base */
     (void)time_genoff_private_base_deinit(ret_base,f_name_local);	 
	   
     return (int)ATS_FAILURE;
   }

   /* Copy the index back to remote PD */
    result = qurt_qdi_copy_to_user(client_handle, 
                                  ret_base_pd, 
                                  &ret_base, 
                                  sizeof(uint32));

   /* Check if the copy happened successfully */
   if(result < 0)
   {
     /* De-Init the external time genoff base */
	 (void)time_genoff_private_base_deinit(ret_base,f_name_local);
	 return (int)ATS_QDI_COPY_FAILED;
  }
 	   
   return (int)ATS_SUCCESS;


}


/*=============================================================================

FUNCTION TIME_GENOFF_QDI_PRIVATE_BASE_INIT

DESCRIPTION
  Completes the initialization of external genoff base in the root PD.
 
DEPENDENCIES
  None

RETURN VALUE
  Assigned private base index number from root PD.

SIDE EFFECTS
  None

=============================================================================*/

static inline int time_genoff_qdi_ext_deinit(int     client_handle,
                                                uint32   base_num,
                                                char *   f_name)
{

  int       result;
  char		f_name_local[MAX_EFS_FILE_NAME_SIZE] = {0};

  /* Copy args structure locally to pass to time_genoff_opr */	
  result = qurt_qdi_copy_from_user(client_handle, 
								   &f_name_local[0],
								   f_name,
								   MAX_EFS_FILE_NAME_SIZE);
  
  /* Check if the copy happened successfully */
  if(result)
	return (int)ATS_FAILURE;

  /* Deinitialize this base */
  return (int)time_genoff_private_base_deinit(base_num,f_name_local);

}


/*=============================================================================

FUNCTION TIME_GENOFF_QDI_RELEASE

DESCRIPTION
 Releases the Qdi object that corresponding to a process that got destroyed

DEPENDENCIES
  None

RETURN VALUE
 None

SIDE EFFECTS
  None

=============================================================================*/
void time_genoff_qdi_release(qurt_qdi_obj_t *obj)
{
   time_genoff_qdi_opener *clntobj = (time_genoff_qdi_opener *)obj;
   uint32 process_idx = clntobj->process_idx;
   
   /* Note: Remove INTLOCK and use LOCK */
   TIME_OSAL_MUTEX_LOCK(&time_genoff_qdi_mutex);
   if(clntobj->pipe)
   {
      qurt_pipe_delete(clntobj->pipe);
   }

   /* Free the Private bases related with this PD */
   (void)time_genoff_ext_delete_pd_base(process_idx);

   /* Free the object related to this process */
   free(time_genoff_clients[process_idx]);
   time_genoff_clients[process_idx] = NULL;

   time_genoff_clients_cntr--;
   
   TIME_OSAL_MUTEX_UNLOCK(&time_genoff_qdi_mutex);
   return;
} /* time_genoff_qdi_release */


/*=============================================================================

FUNCTION TIME_GENOFF_QDI_OPEN

DESCRIPTION
 Creates time_genoff driver object for the client and initializes required details

DEPENDENCIES
  None

RETURN VALUE
 QURT_EOK if the time_genoff is defined and set correctly.

SIDE EFFECTS
  None

=============================================================================*/
int time_genoff_qdi_open(int client_handle, time_genoff_qdi_opener *me)
{
   time_genoff_qdi_opener *clntobj;
   int index;
   int status;
   
   TIME_OSAL_MUTEX_LOCK(&time_genoff_qdi_mutex);

   /* Get a free pd slot */
   for(index=0; index<TIME_GENOFF_QDI_MAX_CLIENTS && time_genoff_clients[index] != NULL; index++);
   
   if(index == TIME_GENOFF_QDI_MAX_CLIENTS)
   {
      TIME_OSAL_MUTEX_UNLOCK(&time_genoff_qdi_mutex);
      ERR_FATAL("Max Processes %d reached", time_genoff_clients_cntr, 0, 0);
      return -1;
   }
   clntobj = malloc(sizeof(time_genoff_qdi_opener));
   
   if(NULL == clntobj)
   {
      TIME_OSAL_MUTEX_UNLOCK(&time_genoff_qdi_mutex);
      printf("malloc err for %d bytes", sizeof(time_genoff_qdi_opener));
      return -1;
   }
   
   clntobj->qdiobj.invoke = time_genoff_qdi_invoke;
   clntobj->qdiobj.refcnt = QDI_REFCNT_INIT;
   clntobj->qdiobj.release = time_genoff_qdi_release;
   
   /* Create Pipe */
   status = time_genoff_qdi_create_pipe(clntobj);
   if(status == QURT_EFAILED)
     {
	   TIME_OSAL_MUTEX_UNLOCK(&time_genoff_qdi_mutex);
       free(clntobj);
       return -1;
     }
   
   time_genoff_clients[index] = clntobj;
   clntobj->process_idx = index;
   time_genoff_clients_cntr++;
   TIME_OSAL_MUTEX_UNLOCK(&time_genoff_qdi_mutex);

   return qurt_qdi_handle_create_from_obj_t(client_handle, &clntobj->qdiobj);
} /* time_genoff_qdi_open */


/*=============================================================================

FUNCTION TIME_GENOFF_QDI_INVOKE

DESCRIPTION
 This function contains the switch which maps to all methods

DEPENDENCIES
  None

RETURN VALUE
  Error codes from the respective functions.

SIDE EFFECTS
  None

=============================================================================*/
static int time_genoff_qdi_invoke(int client_handle,
                          qurt_qdi_obj_t *pobj,
                          int method,
                          qurt_qdi_arg_t a1,
                          qurt_qdi_arg_t a2,
                          qurt_qdi_arg_t a3,
                          qurt_qdi_arg_t a4,
                          qurt_qdi_arg_t a5,
                          qurt_qdi_arg_t a6,
                          qurt_qdi_arg_t a7,
                          qurt_qdi_arg_t a8,
                          qurt_qdi_arg_t a9)
{
   time_genoff_qdi_opener *clntobj = (time_genoff_qdi_opener *)pobj;
   unsigned int      process_idx = clntobj->process_idx;
   
   switch(method)
   {
      case QDI_OPEN: 
         return time_genoff_qdi_open(client_handle, clntobj);
      
      case TIME_GENOFF_QDI_OPR: 
         return time_genoff_qdi_opr(client_handle, a1.ptr);

      case TIME_GENOFF_QDI_PRIVATE_BASE_INIT: 
         return time_genoff_qdi_private_base_init(client_handle, a1.ptr, a2.ptr, a3.ptr, process_idx);

      case TIME_GENOFF_QDI_PRIVATE_BASE_DEINIT: 
         return time_genoff_qdi_ext_deinit(client_handle,a1.num,a2.ptr);

      case TIME_GENOFF_QDI_GET_UPDATE_INFO: 
         return time_genoff_qdi_get_update_info(client_handle, clntobj, a1.ptr);

      case TIME_GENOFF_QDI_REPORT_OUT_OF_COVERAGE:
         return time_genoff_qdi_report_out_of_coverage(a1.num);	   

	  case TIME_GENOFF_QDI_GET_INIT_UPDATES:
         return time_genoff_qdi_get_init_updates(process_idx);	  	

      default:
         return qurt_qdi_method_default(client_handle, pobj, method,
                                     a1, a2, a3, a4, a5, a6, a7, a8, a9);
   }
} /* time_genoff_qdi_invoke */


/*=============================================================================

FUNCTION TIME_GENOFF_QDI_INIT

DESCRIPTION
 Registers with QDI Framework for Time
 
DEPENDENCIES
  None

RETURN VALUE
 None

SIDE EFFECTS
  None

=============================================================================*/
void time_genoff_qdi_init 
(
  void
) 
{
  time_genoff_qdi_opener *p_opener;
  int i;

  /* initialize the mutex here */
  TIME_OSAL_MUTEX_INIT(&time_genoff_qdi_mutex);

  /* Initialize the time genoff processes */
  for(i=0; i<TIME_GENOFF_QDI_MAX_CLIENTS; i++)
     time_genoff_clients[i] = NULL;  

   p_opener = (time_genoff_qdi_opener *)malloc(sizeof(time_genoff_qdi_opener));
   if(NULL == p_opener)
   {
      printf("malloc err for %d bytes", sizeof(time_genoff_qdi_opener));
      return;
   }
   
   p_opener->qdiobj.invoke = time_genoff_qdi_invoke;
   p_opener->qdiobj.refcnt = QDI_REFCNT_INIT;
   p_opener->qdiobj.release = time_genoff_qdi_release;
   qurt_qdi_register_devname(TIME_GENOFF_DRIVER_NAME, p_opener);

   /* Register the pipe update function */
   time_genoff_register_client_update_func(time_genoff_update_to_pipe);

} /* time_genoff_qdi_init */
