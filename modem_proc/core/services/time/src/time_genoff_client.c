/*=============================================================================

                Time_genoff_Client.c

GENERAL DESCRIPTION
      Time Genoff Client Process Code

EXTERNAL FUNCTIONS
  time_genoff_opr
   Does time set/get operation on the basis of Inputs
   
INITIALIZATION AND SEQUENCING REQUIREMENTS
   None.

      Copyright (c) 2009 - 2017
      by QUALCOMM Technologies Incorporated.  All Rights Reserved.

=============================================================================*/

/*=============================================================================

                        EDIT HISTORY FOR MODULE

 This section contains comments describing changes made to the module.
 Notice that changes are listed in reverse chronological order.


$Header: //components/rel/core.mpss/10.0/services/time/src/time_genoff_client.c#1 $ 
$DateTime: 2019/04/24 00:03:26 $ $Author: pwbldsvc $

when         who     what, where, why
--------   ---     ------------------------------------------------------------
08/19/15   abh     Changes for properly handling ATS_PRIVATE base operations.
05/01/13   abh     Created the file
=============================================================================*/
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stringl/stringl.h>
#include "time_svc.h"
#include "time_jul.h"
#include "time_conv.h"
#include "timetick_sclk64.h"
#include "time_genoff_client_v.h"
#include "osal.h"
#include "time_genoff_qdi_v.h"
#include "ats_v.h"
#include "err.h"
#include "rcinit.h"
#include "dll_global_def.h"


/*****************************************************************************/
/*                          DATA DECLARATIONS & DEFINES                            */
/*****************************************************************************/

/* Qdi Time Genoff Client Handle to communicate with Guest os layer */
int time_genoff_client_qdi_handle = -1;

/* Flag to indicate if time genoff client is initialized */
boolean ptime_genoff_client_initialized = FALSE;

qurt_thread_t      time_genoff_worker_thread_id;	/* Thread ID of worker thread */

/*-----------------------------------------------------------------------------
  Time Bases static information
-----------------------------------------------------------------------------*/

time_genoff_client_struct_type              ats_client_bases[ATS_MAX];
time_genoff_client_ptr                      ats_client_private_bases[ATS_PRIVATE_MAX];

/* List of external time genoff pointers */


/*==============================================================================

                            A P I   D E F I N I T I O N S

=============================================================================*/

/*=============================================================================

FUNCTION TIME_GENOFF_ENCODE_INDEX

DESCRIPTION
  Encodes the passed index value.

DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None
  
=============================================================================*/
static inline boolean time_genoff_decode_private_base
(
  uint32            base,
  uint32          * ret_val
)
{
   uint32 temp1 =0;
   uint32 temp2 =0;

   /* Subtract the private base offset from this decoded 
		value to get the index for ats_private_bases array. */ 
   base -= ATS_PRIVATE;

   /* Get the decode values for checking sanity */
   temp1 = ( (base >> 8) ^ TIME_GENOFF_ENCODE_CONST );
   temp2 = (base & 0xFF);

   /* Check if the value passed is proper or not */
   if (temp1 != temp2)
   {
     /* Return FALSE */
     return FALSE;
   }
   else
   {
    if (ret_val != NULL)
    	{
    *ret_val = temp2;
    	}
     return TRUE;
   }
}


/*=============================================================================

FUNCTION TIME_GENOFF_GET_CLIENT_POINTER                                 

DESCRIPTION
  Returns the Pointer to different time bases 

DEPENDENCIES
  None

RETURN VALUE
  The pointer to time_genoff_struct_type

SIDE EFFECTS
  None

=============================================================================*/

time_genoff_client_ptr time_genoff_get_client_pointer
( 
  /* Time bases whose pointer needs to be returned */
  time_bases_type               base
)
{
  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
  time_genoff_client_ptr  temp_base_ptr = NULL;
  uint32                  temp_base = 0;

  /* Return NULL in case the base is a private base */
  if ((uint32)base < ATS_MAX)
  {
    return &(ats_client_bases[base]);
  }
  else if (base > ATS_PRIVATE)
  {
    if (FALSE == time_genoff_decode_private_base(base,&temp_base))
    {
      ERR_FATAL("Private base=%d is incorrect",base,0,0);
	  return NULL;
    }
    else
    {
      if (temp_base < ATS_PRIVATE_MAX)
      {
        temp_base_ptr = (time_genoff_client_ptr)ats_client_private_bases[temp_base];

        /* ERR FATAL if the base is not proper for external genoff pointer */
        if (temp_base_ptr)
        {
          return temp_base_ptr;
        }
        else
        {
          ERR_FATAL("Private base=%d is incorrect",base,0,0);
		  return NULL;
        }
      }
	  return NULL;
    }
  }
  else
  {
    ERR_FATAL("base=%d is incorrect",base,0,0);
	return NULL;
  }
     
} /* time_genoff_get_client_pointer */


/*=============================================================================

FUNCTION TIME_GENOFF_GET_CLIENT_OFFSET

DESCRIPTION
  Returns generic offset value of a given time base

DEPENDENCIES
  None

RETURN VALUE
  int64 value

SIDE EFFECTS
  None

=============================================================================*/
int64 time_genoff_get_client_offset
( 
    time_bases_type  base  
)
{
  /* Pointer to 'base' in consideration */
  time_genoff_client_ptr    ptime_genoff;
  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
  
  if ( base >= ATS_MAX ) 
    return 0;
 
  /* Get pointer to base in consideration */
  ptime_genoff = time_genoff_get_client_pointer(base);
  return ptime_genoff->generic_offset;
} /*  time_genoff_get_client_offset */


/*=============================================================================

FUNCTION TIME_GENOFF_GET_SUM_GENOFF_TS

DESCRIPTION
  Returns sum generic offset value of a given time base in timestamp format 
  in input variable. Return value is boolean which will indicate offset is 
  +ve(True) or -ve(False)

DEPENDENCIES
  None

RETURN VALUE
  boolean as return value which indicates offset if +ve or -ve
  Provides sum generic ts offset value as part of inputs arguments

SIDE EFFECTS
  None

=============================================================================*/
boolean time_genoff_get_sum_genoff_ts
( 
    time_bases_type  base,
    time_type        sum_genoff_ts_val
)
{
  /* Pointer to 'base' in consideration */
  time_genoff_client_ptr ptime_genoff;
  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  qw_set(sum_genoff_ts_val, 0, 0);
  
  /* Get pointer to base in consideration */
  ptime_genoff = time_genoff_get_client_pointer(base);
  
  if(ptime_genoff == NULL)
     return FALSE;
  
  qw_equ(sum_genoff_ts_val, ptime_genoff->sum_generic_offset_ts);
  return ptime_genoff->sum_generic_offset_ts_is_negative;
} /*  time_genoff_get_offset */


/*=============================================================================

FUNCTION TIME_GENOFF_GET_TS_FROM_OFFSET_AND_TIMETICK

DESCRIPTION
  Calculates timestamp from offset and timetick that are passed as inptu args.

DEPENDENCIES
  None

RETURN VALUE
  time in timestamp format

SIDE EFFECTS
  None

=============================================================================*/

void time_genoff_get_ts_from_offset_and_timetick
(
  /* Generic offset in timestamp format */
  time_type           sum_genoff_ts,
  /* Sign of offset. FALSE if +ve, True if -ve */
  boolean             is_offset_negative,
  /* timetick at which time is required */
  uint64              timetick,
  /* Calculated timestamp is returned */
  time_type           ts_val  
)
{

   /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
  
   qw_set( ts_val, 0, 0 );
  
   /* If offset is not negative, directly use it for
      addition of timetick by copying offset to ts_val */
   if(is_offset_negative == FALSE)
      qw_equ(ts_val, sum_genoff_ts);

   /* Advance the output timestamp by the elapsed slow clocks */
   time_sclk_get(ts_val, timetick);

   /* add / sub sum_generic_offset_ts from uptime based
	  on the state of sum_generic_offset_ts_is_negative */
   if(is_offset_negative == TRUE)
   {
      time_conv_ts_sub(ts_val, sum_genoff_ts);
   }

   return;
} /* time_genoff_get_ts_from_offset_and_timetick */


/*=============================================================================

FUNCTION TIME_GENOFF_CLIENT_GET_OPTIMIZED_TS

DESCRIPTION
  Gets Genoff Time for the time base in consideration in 64 bit Timestamp format, only if time_genoff is 
  initialized 

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

=============================================================================*/


void time_genoff_client_get_optimized_ts
(
  /* Pointer to time_genoff_args_type with genoff_ptr in consideration */
  time_genoff_args_ptr         pargs
)
{
  /* Pointer to genoff in consideration */
  time_genoff_client_ptr       ptime_genoff;

  /*to store the generic offset in a temp variable*/
  uint32     old_generic_offset=0;  

  /*temporary timestamp value*/
  time_type  ts_val_temp;
    
  uint64     timetick;

  /* This variable is used to erase TIME_GENOFF_TIMETICK_VALID from getting onto stack */
  volatile uint32 timetick_valid_flag = TIME_GENOFF_TIMETICK_VALID;  

  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  /* If ptime_genoff is not initilazed */
  if ( ptime_genoff_client_initialized == FALSE )
  {
    qw_set( pargs->ts_val, 0, 0 );
    return;
  }  
  
  qw_set( ts_val_temp, 0, 0 );
  
  /* Get pointer to genoff */
    ptime_genoff = time_genoff_get_client_pointer( pargs->base );
	
	if( ptime_genoff == NULL )
     {
        return;
     }

  do
  {    

	/*record the generic offset before we begin*/
	old_generic_offset = ptime_genoff->sum_generic_offset>>32;

	/* Get time from sclk */
	if ( pargs->dal_timetick_val_valid != timetick_valid_flag )
	{
	  /* Erase TIME_GENOFF_TIMETICK_VALID from getting onto stack.
         This will make sure there will not be any issues if clients
		 doesnt explicitly set dal_timetick_val_valid to 0 if they dont use it */
	  timetick_valid_flag = 0;
	  
	  timetick = (uint64)-1;
	}
	else
	{
      timetick_valid_flag = 0;	
	  
	  timetick = pargs->dal_timetick_val;
	}

    time_genoff_get_ts_from_offset_and_timetick(ptime_genoff->sum_generic_offset_ts,
	                                            ptime_genoff->sum_generic_offset_ts_is_negative,
												timetick,
												ts_val_temp);

  }while( FALSE == osal_atomic_compare_and_set(
                                  (osal_atomic_word_t *)&old_generic_offset,
                                  (ptime_genoff->sum_generic_offset)>>32,
                                  (old_generic_offset)) );

     qw_equ(pargs->ts_val, ts_val_temp);
     
} /* time_genoff_client_get_optimized_ts */


/*=============================================================================

FUNCTION TIME_GENOFF_PRIVATE_BASE_INIT

DESCRIPTION
  Does initialization of the external genoff pointer.

DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None

=============================================================================*/
ats_error_type time_genoff_private_base_init
(
  /* Filename to associate with the external genoff pointer */
  char *                       f_name,

  /* Pointer where the base value will be returned */
  uint32 *                     ret_base
)
{

  /* Pointer to ats_bases in consideration */
  time_genoff_client_ptr       ptime_genoff;

  /* Array index iterator */
  uint32                          index;

  /* temporary decoded index */
  uint32                          temp_index;

  /* error return for the QDI call to init function */
  ats_error_type                  ret = ATS_FAILURE;


  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  /* The passed filename should not exceed the destination size */
  if  (f_name != NULL)
  {
    if ((strlen(f_name) > MAX_EFS_FILE_NAME_SIZE))
   	{
      ATS_MSG_SPRINTF_1("time_genoff_private_base_init: file length exceeded, f_name=%s",f_name);
      return ATS_INVALID_INPUT;
    }
  }
  else
  {
    ATS_ERR("time_genoff_private_base_init: NULL argument in f_name");
    return ATS_INVALID_INPUT;
  }

  /* Input validation */
  if(ret_base == NULL)
  {
    ATS_MSG_SPRINTF_1("time_genoff_private_base_init: NULL pointer in args ,f_name=%s",f_name);
    return ATS_INVALID_INPUT;
  }

  /* Init the ret_base with ATS_INVALID value */
  *ret_base = ATS_INVALID;

  /* Alloc structure for the private base */
  ptime_genoff = malloc(sizeof(time_genoff_client_struct_type));

  if ( ptime_genoff == NULL )
  {
    ATS_MSG_SPRINTF_1("time_genoff_private_base_init: Unable to allocate memory f_name=%s",f_name);
	return ATS_MALLOC_FAIL;
  }

  /* Memset this malloced memory */
  memset(ptime_genoff,0x0,sizeof(time_genoff_client_struct_type));

  /* Call the Root PD init function to complete the initialization */
  ret = time_genoff_invoke_private_base_init(time_genoff_client_qdi_handle,f_name,&index,ptime_genoff);
  if (ATS_SUCCESS != ret)
  {
    /* Free the malloced memory */
    free(ptime_genoff);
	
    ATS_MSG_SPRINTF_1("time_genoff_private_base_init: QDI Call failed, f_name=%s",f_name);
    return ret;
  }

  /* Decode this obtained index to assign to ats_private_bases array */
  if(FALSE == time_genoff_decode_private_base(index,&temp_index))
  {
    ERR_FATAL("Corrupted index=0x%.8x;ret_index=0x%.8x;f_name=%s",temp_index,index,f_name);
	return ATS_FAILURE;
  }


  /* Add this external genoff pointer to ats_private_bases array */
  if (ats_client_private_bases[temp_index] == NULL)
  {
    ats_client_private_bases[temp_index] = ptime_genoff;
  }
  else
  {
    ERR_FATAL("ats_client_private_bases!=NULL for index=0x%.8x;ret_index=0x%.8x;f_name=%s",temp_index,index,f_name);
	return ATS_FAILURE;
  }

  /* Return the index */
  *ret_base = index;

  /* Return SUCCESS */
  return ATS_SUCCESS;

} /* time_genoff_private_base_init */


/*=============================================================================

FUNCTION TIME_GENOFF_EXT_DEINIT

DESCRIPTION
  Does initialization of the external genoff pointer.

DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None

=============================================================================*/
ats_error_type time_genoff_private_base_deinit
(
  /* Pointer to time_genoff_args_type with genoff_ptr in consideration */
  uint32                base,

  /* Filename for the associated base */
  char *				f_name	

)
{
  ats_error_type            ret_val = ATS_FAILURE ;

  /* Pointer to the private genoff base in consideration */
  time_genoff_client_ptr    ptime_genoff;
  
  
  /* Index value for the private base */
  uint32                       index = 0;
	
  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  if (f_name == NULL)
  {
    ATS_ERR("time_genoff_private_base_deinit: NULL string passed");
    return ATS_FAILURE;
  }

  if (FALSE == time_genoff_decode_private_base(base,&index))
  {
    ATS_ERR_1("time_genoff_private_base_deinit: Corrupted base=0x%.8x",base);
	return ATS_FAILURE;
  }

  /* De-init the root PD private base structure */
  ret_val = time_genoff_invoke_private_base_deinit(time_genoff_client_qdi_handle,base,f_name);


  if (ret_val != ATS_SUCCESS)
  {
    ATS_MSG_SPRINTF_2("time_genoff_private_base_deinit Failed, base=0x%.8x;f_name=%s",
		               base,f_name);
    return ret_val;
  }

  /* Get the pointer to time_base instance */
  ptime_genoff = (time_genoff_client_ptr)time_genoff_get_client_pointer((time_bases_type)base);

  /* Remove this external genoff pointer from the array of extern pointers */
  ats_client_private_bases[index] = NULL;

  /* Free the private genoff structure */
  free(ptime_genoff);

  /* return */
  return ret_val;

} /* time_genoff_private_base_deinit */


/*=============================================================================

FUNCTION TIME_GENOFF_OPR

DESCRIPTION
  Does time set/get operation on the basis of Inputs.

DEPENDENCIES
  Time to be set should never be less than uptime of the UE.

RETURN VALUE
  None

SIDE EFFECTS
  None

=============================================================================*/

void time_genoff_opr
(
  /* Pointer to time_genoff_args_type with genoff_ptr in consideration */
  time_genoff_args_ptr         pargs
)
{
  /* Arguments to be passed to base_genoff */
  time_genoff_args_struct_type temp_genoff_args;

  /* Genoff time, in secs */
  time_type                    ts_val_secs;

  /* Genoff time in secs temp variable */
  uint32                       lsecs;

  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  /* GET operation */
  if ( pargs->operation & TIME_GENOFF_OPR_GET_MASK )
  {

    /* unit of time */
    switch( pargs->unit )
    {
      case TIME_STAMP:
      {
        /* Get time in timestamp format */
        time_genoff_client_get_optimized_ts( pargs );
      }
	  break;
      case TIME_MSEC:
      {
        /* Get time in timestamp format */
        time_genoff_client_get_optimized_ts( pargs );
		/* Convert time into msecs */
		time_conv_to_ms( pargs->ts_val );
      }
	  break;
      case TIME_SECS:
      {
        /* Get time in timestamp format */
        time_genoff_client_get_optimized_ts( pargs );

        /* Convert time into secs */
        lsecs = time_conv_to_sec( pargs->ts_val );

        /* Set the lower 32 bits as the value in seconds */
        qw_set(pargs->ts_val, 0, lsecs);
      }
	  break;
      case TIME_JULIAN:
      {
        /* Get time in secs */
        temp_genoff_args.base                   = pargs->base;
        temp_genoff_args.base_source            = pargs->base_source;
        temp_genoff_args.ts_val                 = &ts_val_secs;
        temp_genoff_args.unit                   = TIME_SECS;
          temp_genoff_args.operation   = T_GET;

        time_genoff_opr( &temp_genoff_args );

        lsecs = ts_val_secs[0];

        /* Convert time into julian */
        time_jul_from_secs( lsecs, pargs->ts_val);
      }
	  break;
      case TIME_20MS_FRAME:
      {
        /* Get time in timestamp format */
		time_genoff_client_get_optimized_ts( pargs );

		/* Convert time into 20ms frame time */
        time_conv_to_20ms_frame_time( pargs->ts_val );
      }
	  break;
      default:
        ATS_ERR_1("Invalid time unit %d", pargs->unit);
        break;
    }
	
  }
  else if ( pargs->operation & TIME_GENOFF_OPR_SET_MASK ) /* SET operation */
  {
    /* Invoke QDI call to perform the set operation */
    time_genoff_invoke_opr(time_genoff_client_qdi_handle, pargs);
  }

} /* time_genoff_opr */

/*=============================================================================

FUNCTION TIME_GENOFF_SYNC_WITH_TIME_TOD

DESCRIPTION
  This function syncs is called from time_tod_set. The purpose of the 
  function is to sync the time maintained by time_tod module and the genoff
  module.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

=============================================================================*/
void time_genoff_sync_with_tod
(
  time_type ts_val
)
{
	//STUBBED
}


/*=============================================================================

FUNCTION TIME_GENOFF_SET_REMOTE_MODEM_UPDATE

DESCRIPTION
  This function sets the static variable which is used to indicate whether
  the remote modem should be updated or not

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

=============================================================================*/
void time_genoff_set_remote_modem_update
(
  boolean update_remote_modem
)
{
  /* To supress compiler warning */
  update_remote_modem = TRUE;

 
  /* Return, since for remote PD's this is not supported */
  return;
}

/*=============================================================================

FUNCTION TIME_GENOFF_GET_REMOTE_MODEM_UPDATE

DESCRIPTION
  This function gets the static variable which is used to indicate whether
  the remote modem should be updated or not

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

=============================================================================*/
boolean time_genoff_get_remote_modem_update
(
  void
)
{
  /* STUBBED */
  return FALSE;
}


#ifdef FEATURE_QMI_TIME_REMOTE_CLNT
/*=============================================================================

FUNCTION TIME_GENOFF_GET_SYS_TIME_IN_SEC_WITH_LP_SEC

DESCRIPTION
  This function sets gets the system time alongwith leap second info

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

=============================================================================*/
uint32 time_genoff_get_sys_time_in_sec_with_lp_sec
(
  void
)
{
   /* STUBBED */
   return (uint32)0;
}
#endif /*FEATURE_QMI_TIME_REMOTE_CLNT*/

/*=============================================================================

FUNCTION TIME_GENOFF_REPORT_OUT_OF_COVERAGE_TIME

DESCRIPTION
  This function records when a radio technology goes out of coverage

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

=============================================================================*/
void time_genoff_report_out_of_coverage
(
  time_bases_type base
)
{
  if(base < ATS_MAX) 
  {
    time_genoff_invoke_report_out_of_coverage(time_genoff_client_qdi_handle, base);
  }  
}

/*==============================================================================

                     CLIENT WORKER THREAD & INITIALIZATION ROUTINES

=============================================================================*/


/*===========================================================================
FUNCTION    TIME_GENOFF_CLIENT_WORKER_FUNCTION

DESCRIPTION
  Function executed after thread is created in the client PD.
  Sets the offsets for clients of time-services.
  
DEPENDENCIES
  None.

RETURN VALUE
  None.
  
SIDE EFFECTS
  None.
===========================================================================*/

void time_genoff_client_worker_function(void *parameter)
{
   int result;
   time_genoff_qdi_update_container      update_params;
   time_genoff_client_ptr                ptime_genoff_client;
   int64                                 delta_ms = 0;

   /* Pointer for the callback notification structure */
   time_genoff_client_cb_struct_ptr                  cb_struct_ptr = NULL; 

   /* Indicate to Guest OS that we are ready to receive time updates */
   time_genoff_invoke_get_init_updates(time_genoff_client_qdi_handle);
   
   while(1)
   {
     /* memset the structure */
	 memset(&update_params, 0, sizeof(time_genoff_qdi_update_container));

     /* Make qdi call to get the genoff update data from pipe */
     result = time_genoff_invoke_get_update_info(time_genoff_client_qdi_handle, &update_params);

     /* If there was a cancel signal or copy fail, try again */
     if( result == TIME_ERR_CANCEL_WAIT ||
         result == TIME_ERR_QDI_CPY_FAIL )	 
     {
       //MSG_ERROR("time_genoff qdi server returned %d result", result, 0, 0);
       continue;
     }

	 ptime_genoff_client = time_genoff_get_client_pointer(update_params.base);


     if (ptime_genoff_client!=NULL)
     {
	 /* Get the delat msecs difference */
	 delta_ms = ptime_genoff_client->generic_offset - update_params.time_genoff_client.generic_offset;
	 
     /* Update the client side time genoff details */
       ptime_genoff_client->generic_offset = 
                 update_params.time_genoff_client.generic_offset;
       ptime_genoff_client->sum_generic_offset = 
                 update_params.time_genoff_client.sum_generic_offset;
       qw_equ(ptime_genoff_client->sum_generic_offset_ts,
                 update_params.time_genoff_client.sum_generic_offset_ts);
       ptime_genoff_client->sum_generic_offset_ts_is_negative = 
                 update_params.time_genoff_client.sum_generic_offset_ts_is_negative;
     }
    else 
	  return;
	 
	 /* Get the 1st callback function from the list of cb functions */
     cb_struct_ptr = ptime_genoff_client->cb_list_ptr;  
	 
	 /* Check if there are registered callback functions with this base */
	 while (cb_struct_ptr != NULL)
	 	{
	   /* Get the details for the callback */
  	   cb_struct_ptr->cb_func(cb_struct_ptr->cb_data,delta_ms);
	   cb_struct_ptr = cb_struct_ptr->next;
	 	}
	 
   }

} /* time_genoff_client_worker_function */



/*===========================================================================
FUNCTION    TIMEGENOFFCREATETHREAD

DESCRIPTION
  Creates worker thread that waits inside Guest OS for time setting details.
  
DEPENDENCIES
  None.

RETURN VALUE
  Error value from the thread creation function.
  
SIDE EFFECTS
  None.
===========================================================================*/

static int TimeGenoffCreateThread(unsigned int process_idx)
{
   int ret_value = 0;
   qurt_thread_attr_t tattr;
   unsigned int stackbase;
   char thread_name[QURT_THREAD_ATTR_NAME_MAXLEN];
   RCINIT_INFO info_handle = NULL;
   RCINIT_PRIO prio = 0;
   unsigned long stksz = 0;

   /* Init the thread name based on the process index */  
   snprintf(thread_name, sizeof(thread_name), "TIME_CLIENT_%u", process_idx);

   /* Look up thread info: 
      rcinit_task_prio.csv must be updated as new clients are added */
   info_handle = rcinit_lookup(thread_name);
	   
   if (!info_handle) 
   {
      ERR_FATAL("TIME_CLIENT_%u task info not found",process_idx , 0, 0);
   }
   else
   {
     prio = rcinit_lookup_prio_info(info_handle);
     stksz = rcinit_lookup_stksz_info(info_handle);
     if ((prio > 255) || (stksz == 0)) 
     {
       ERR_FATAL("Invalid Priority:%d or Stack Size: %d",prio, stksz, 0);
     }
   
   /* Alocate memory for stack */
     stackbase = (unsigned int)qurt_malloc(stksz);

   /* Set the thread attributes */
   qurt_thread_attr_init (&tattr);
   qurt_thread_attr_set_name(&tattr, thread_name);   
   qurt_thread_attr_set_priority (&tattr, 255-prio);
   qurt_thread_attr_set_stack_size (&tattr, (stksz - 8));
   qurt_thread_attr_set_stack_addr (&tattr, (void*)((stackbase + 7) & (~7)) );

   /* Create the thread */
   ret_value =  qurt_thread_create(&time_genoff_worker_thread_id,
                                   &tattr, 
                                   time_genoff_client_worker_function, 
                                   NULL);
   }
  
   return ret_value;
}  /* TimeGenoffCreateThread() */



/*===========================================================================
FUNCTION    TIME_GENOFF_CLIENT_INIT

DESCRIPTION
  Initialize the Time Genoff Client service.

DEPENDENCIES
  None.

RETURN VALUE
  None.
  
SIDE EFFECTS
  None.
===========================================================================*/

DLL_API_GLOBAL void time_genoff_client_init(void) 
{
   /* pid of the client process */
   unsigned int time_genoff_client_pid;
   time_genoff_client_pid = qurt_getpid();

   /* Invoke QDI call to get the handle for the time driver */
   time_genoff_client_qdi_handle = qurt_qdi_open(TIME_GENOFF_DRIVER_NAME);

   if(time_genoff_client_qdi_handle < 0)
   {
      printf("time_genoff_client_init: qdi_open failed");
      ERR_FATAL("time_genoff_client_init :qdi_open failed\n", 0, 0, 0);
      return;
   }

   /* Create the worker thread, unique to each client connecting with the time driver */
   if(TimeGenoffCreateThread(time_genoff_client_pid) != 0)
    {
       printf("time_genoff_client_init : TimeGenoffCreateThread failed\n");
       ERR_FATAL("time_genoff_client_init : TimeGenoffCreateThread failed\n", 0, 0, 0);
       return;
    }

   /* Initialize for timetick_sclk64 */
   timetick_sclk64_init();

   /* Mark the client as initialized */
   ptime_genoff_client_initialized = TRUE;

   return;
}  /* time_genoff_client_init */

/*=============================================================================
FUNCTION TIME_GENOFF_REGISTER_CB

DESCRIPTION
  Registers a callback function to the passed base, which is 
  called whenever there is any time updated to that base.

DEPENDENCIES
  None

RETURN VALUE
  Error return types (refer to time_genoff.h for details)

SIDE EFFECTS
  None
=============================================================================*/
ats_error_type time_genoff_register_cb (
  time_bases_type base,
  time_genoff_t2_cb_type cb_func,
  uint32 data )
{
	/* Pointer to the time genoff base */
	time_genoff_client_ptr  ptime_genoff = NULL;

	/* Pointers for the time update notification structure */
	time_genoff_client_cb_struct_ptr cb_struct_ptr_local = NULL;
	time_genoff_client_cb_struct_ptr cb_struct_ptr_local_temp = NULL;

	/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */	

	/* Input Validation */
	if (cb_func ==  NULL)
		return ATS_INVALID_INPUT;

	if ( (base < ATS_MAX) || (TRUE == time_genoff_decode_private_base(base,NULL)) )
		{
		// SUCCESS case, do nothing.
		cb_struct_ptr_local_temp = NULL;
		}
	else
		{
		return ATS_INVALID_BASE_NUM;
		}

	/* Get the time genoff pointer */
	if ( (ptime_genoff = time_genoff_get_client_pointer(base)) == NULL )
		return ATS_FAILURE;

	/* Allocate memory for the list element */
	if ( (cb_struct_ptr_local = malloc(sizeof(time_genoff_client_cb_struct_type))) == NULL )
		return ATS_MALLOC_FAIL;

	/* Memset this malloc'ed structure */
	memset(cb_struct_ptr_local, 0x0, sizeof(time_genoff_client_cb_struct_type));

	/* Update details of the callback */
	cb_struct_ptr_local->cb_func = cb_func;
	cb_struct_ptr_local->cb_data = data;
	cb_struct_ptr_local->next = NULL;


	/* Acquire the lock before procedding */
	//TIME_OSAL_MUTEX_LOCK(ptime_genoff->mutex);

	/* Check if the ATS base is all properly initialized and has room for adding a cb function. */
	if ( (ptime_genoff_client_initialized == FALSE) ||
		 (ptime_genoff->number_of_cb_func >= MAX_CB_FUNC) )
		{
		/* Release  the lock */
		//TIME_OSAL_MUTEX_UNLOCK(ptime_genoff->mutex);

		/* Free malloc'ed memory */
		free(cb_struct_ptr_local);

		/* Return error */
		return ATS_BASE_NOT_INITIALIZED;
		}

	/* Insert into the list */
	if (ptime_genoff->cb_list_ptr == NULL)
		{
		/* The list was empty & this is the 1st element of the list */
		ptime_genoff->cb_list_ptr = cb_struct_ptr_local;
		}
	else /* Traverse the list to find the appropriate place for new callback detail */
		{
		cb_struct_ptr_local_temp = ptime_genoff->cb_list_ptr ;
		
		while(cb_struct_ptr_local_temp->next != NULL) {
			cb_struct_ptr_local_temp = cb_struct_ptr_local_temp->next;
			}

		/* Insert the list element */
		cb_struct_ptr_local_temp->next = cb_struct_ptr_local;
		}

	/* Increase the total number of registered CB function for this ATS base */
	ptime_genoff->number_of_cb_func++;

	/* Release  the lock */
	//TIME_OSAL_MUTEX_UNLOCK(ptime_genoff->mutex);

	/* Return SUCCESS for this operation */
	return ATS_SUCCESS;

}

#if 0 /* COMMENTING OUT DEREG FUNCTION */
/* COMMENTING OUT THE DEREGISTRATION FUNCTION.
 * Keeping this dereg function would then require proper handling of
 * callbacks inside the mutex protection, before they are getting called
 * from the time_genoff_set_helper function.
 */
/*=============================================================================
FUNCTION TIME_GENOFF_REGISTER_UPDATE_NOTIFICATION_CB

DESCRIPTION
  De-registers the callback function added in the passed ATS base, which was to 
  be called whenever the base received time updates.

DEPENDENCIES
  None

RETURN VALUE
  ATS_SUCCESS  -- The callback was successfully added
  ATS_FAILURE  -- Callback was not added.

SIDE EFFECTS
  None
=============================================================================*/
ats_error_type time_genoff_deregister_cb (
	time_bases_type base,
	time_genoff_t2_cb_type cb_func )
{

	/* Pointer to the time genoff base */
	time_genoff_client_ptr  ptime_genoff = NULL;

	/* Pointers for the time update notification structure */
	time_genoff_client_cb_struct_ptr cb_struct_ptr = NULL;
	time_genoff_client_cb_struct_ptr update_struct_ptr_prev = NULL;

	/* Boolean var to indicate if the cb_func is found or not */
	boolean found_cb_func = FALSE;

	/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */	

	/* Input Validation */
	if (cb_func ==  NULL)
		return ATS_INVALID_INPUT;

	if ( (base < ATS_MAX) || (TRUE == time_genoff_decode_private_base(base,NULL)) )
		{
		// SUCCESS case, do nothing.
		cb_struct_ptr_local_temp = NULL;
		}
	else
		{
		return ATS_INVALID_BASE_NUM;
		}

	/* Get the time genoff pointer */
	if ( (ptime_genoff = time_genoff_get_client_pointer(base)) == NULL )
		return ATS_FAILURE;

	/* Acquire the lock before procedding */
	//TIME_OSAL_MUTEX_LOCK(ptime_genoff->mutex);

	/* Check if the time base is pre initialized befire procedding */
	if (ptime_genoff_client_initialized == FALSE)
		{
		/* Release  the lock */
		//TIME_OSAL_MUTEX_UNLOCK(ptime_genoff->mutex);

		/* Return error */
		return ATS_BASE_NOT_INITIALIZED;
		}

	/* Check if the list is empty */
	if (ptime_genoff->cb_list_ptr == NULL)
		{
		/* Return error */
		return ATS_FAILURE;
		}

	cb_struct_ptr = update_struct_ptr_prev = ptime_genoff->cb_list_ptr;

	/* Traverse the list to find the element with the callback function */
	while(cb_struct_ptr != NULL) {
		if (cb_struct_ptr->cb_func == cb_func)
			{
			found_cb_func = TRUE;
			break;
			}
		update_struct_ptr_prev = cb_struct_ptr;
		cb_struct_ptr = cb_struct_ptr->next;
		}

	/* The CB function was found in the list of callback function */
	if (found_cb_func == TRUE)
		{
		/* If the CB func was in the head of the list itself */
		if (cb_struct_ptr == ptime_genoff->cb_list_ptr)
			{
			ptime_genoff->cb_list_ptr = cb_struct_ptr->next;
			}
		else
			{
			update_struct_ptr_prev->next = cb_struct_ptr->next;
			}
		
		/* Deallocate the memory for the update notifucation structure */
		free(update_struct_ptr_prev);

		/* Decrease the total number of registered CB function for this ATS base */
		ptime_genoff->number_of_cb_func--;

		/* Release  the lock & return SUCCESS */
		//TIME_OSAL_MUTEX_UNLOCK(ptime_genoff->mutex);
		return ATS_SUCCESS;
		}
	else
		{
		/* CB function was not found in the list. */

		/* Release  the lock & return failure */
		//TIME_OSAL_MUTEX_UNLOCK(ptime_genoff->mutex);
		return ATS_FAILURE;
		}

}
#endif /* COMMENTING OUT DEREG FUNCTION */
