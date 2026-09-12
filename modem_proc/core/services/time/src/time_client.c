/*=============================================================================

                   T I M E   S E R V I C E   C L I E N T   S U B S Y S T E M

GENERAL DESCRIPTION
  Implements time-keeping functions using various sources for user PD.


EXTERNALIZED FUNCTIONS

  time_to_ms( )
    Converts a time stamp into milliseconds.

  time_get( )
    Retrieves time according to CDMA, HDR, GSM, or slow-clock Time-of-Day

  time_get_ms( )
    Retrieves system time in milliseconds

  time_get_sec( )
    Retrieves system time, in seconds

  time_get_uptime_ms( )
    Retrieves the up-time, in milliseconds

  time_get_uptime_secs( )
    Retrieves the up-time, in seconds

  time_to_ms_native()
    Convert the current time to milliseconds in native uint64 format.

  time_get_ms_native()
    Get the current time in milliseconds in native uint64 format.    

REGIONAL FUNCTIONS
  
INITIALIZATION AND SEQUENCING REQUIREMENTS


Copyright (c) 2003 - 2014 by QUALCOMM Technologies Incorporated.  All Rights Reserved.

=============================================================================*/


/*=============================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/core.mpss/10.0/services/time/src/time_client.c#1 $

when         who     what, where, why
--------   ---     ------------------------------------------------------------
05/01/14   abh     File Created

=============================================================================*/



/*=============================================================================

                           INCLUDE FILES

=============================================================================*/

#include <stringl/stringl.h>
#include <string.h>
#include <stdio.h>
#include "time_svc.h"
#include "time_conv.h"
#include "timetick_sclk64.h"
#include "ats_v.h"
#include "time_jul.h"
#include "err.h"
#include "qw.h"
#include "dll_global_def.h"

#ifdef FEATURE_TIME_QDSP6
#include "qurt.h"
#endif /*FEATURE_TIME_QDSP6*/

#include "time_genoff_client_v.h"

/*=============================================================================

                           DATA DEFINITIONS

=============================================================================*/

#define ATS_INTLOCK()
#define ATS_INTFREE()

/* Declaration for the time_genoff client initilizer flag */
extern boolean ptime_genoff_client_initialized;

/* Flag to indicate if the system time bases are registered for callback function with time genoff */
static boolean time_genoff_register_system_time_base = FALSE;

/* Typedef for the structure for registering time update callback functions */
typedef struct time_registered_cb_func_s
{
  time_genoff_t2_cb_type cb_func;
  uint32 cb_data;
  struct time_registered_cb_func_s * next;
}time_registered_cb_func_type;

time_registered_cb_func_type * time_cb_func_list = NULL;

/*-----------------------------------------------------------------------------

                      SYSTEM TIMESTAMP FORMAT


   |<------------ 48 bits --------------->|<----- 16 bits ------->|
   +--------------------------------------+-----------------------+
   |      1.25 ms counter                 |   1/32 chip counter   |
   +--------------------------------------+-----------------------+
         (11K years of dynamic range)          (1.25 ms dynamic
                                                range. Rolls over
                                                at count 49152)

-----------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
  Useful constants
-----------------------------------------------------------------------------*/

/* To convert 1.25ms units to TS units, shift in cx32 bits of timestamp */
#define TIME_1p25MS_TO_TS_SHIFT   16

/*-----------------------------------------------------------------------------
  Time static information
-----------------------------------------------------------------------------*/

/*=============================================================================

                           FUNCTION DEFINITIONS

=============================================================================*/



/*=============================================================================

FUNCTION TIME_INIT

DESCRIPTION
  Initialize Timekeeping Subsystem

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  Installs timekeeping ISR, timers, etc.

=============================================================================*/

void time_init( void )
{
	//STUBBED
} /* time_init */



/*=============================================================================

FUNCTION TIME_INIT_CONFIG

DESCRIPTION
  Registers for Callback Registration for Time_tod updates remote updates

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

=============================================================================*/

void time_init_config( void )
{
	//STUBBED
} /*  time_init_config */



/*=============================================================================

FUNCTION TIME_TO_MS

DESCRIPTION
  Convert a timestamp from System Time to millisecond units

DEPENDENCIES
  None

RETURN VALUE
  The converted value is stored in the 'time' parameter.

SIDE EFFECTS
  None

=============================================================================*/

void time_to_ms
(
  /* In: Time in time-stamp format;  Out: Time in milliseconds */
  time_type                       ts_val
)
{
  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  /* Redirected call to time_conv_to_ms */
  time_conv_to_ms (ts_val);
  
} /* time_to_ms */



/*=============================================================================

FUNCTION TIME_TO_SEC

DESCRIPTION
  Convert a timestamp from System Time to seconds

DEPENDENCIES
  None

RETURN VALUE
  Seconds, corresponding to the time stamp

SIDE EFFECTS
  Input argument value is destroyed.

=============================================================================*/

uint32 time_to_sec
(
  /* In: Time in time-stamp format;  Out: Garbage */
  time_type                       ts_val
)
{
  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  /* Redirected call to time_conv_to_sec */
  return time_conv_to_sec(ts_val);
  
} /* time_to_sec */


/*=============================================================================

FUNCTION TIME_GET_SUM_GENOFF_TS

DESCRIPTION
  Returns sum generic offset value of system base in timestamp format

DEPENDENCIES
  None

RETURN VALUE
  boolean as return value
  Provides sum generic ts offset value as part of inputs arguments

SIDE EFFECTS
  None

=============================================================================*/
/* Comment out since time_genoff_get_sum_genoff_ts is not compiled for User PDs */
/*
boolean time_get_sum_genoff_ts
( 
    time_type        sum_genoff_ts_val
)
{
  return time_genoff_get_sum_genoff_ts(
                                       ((0 != time_genoff_get_client_offset(ATS_TOD)) ? ATS_TOD : ATS_USER), 
									   sum_genoff_ts_val
									  );
} */
/*  time_get_sum_genoff_ts */


/*=============================================================================

FUNCTION TIME_GET

DESCRIPTION
  Returns the current time

DEPENDENCIES
  A valid CDMA time, HDR time, GSM time, or a Time-of-Day, etc.

RETURN VALUE
  Time-stamp returned to ts_val parameter.
  Time source used to determine time is returned

SIDE EFFECTS
  None

=============================================================================*/

time_source_enum_type time_get
(
  /* OUT: The current time */
  time_type                       ts_val
)
{
  
  /* Agrument to time_genoff_opr */
  time_genoff_args_struct_type    temp_genoff_args;

  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  temp_genoff_args.base        = (0 != time_genoff_get_client_offset(ATS_TOD)) ? ATS_TOD : ATS_USER;
  temp_genoff_args.base_source = TIME_SCLK;
  temp_genoff_args.ts_val      = ts_val;
  temp_genoff_args.unit        = TIME_STAMP;
  temp_genoff_args.operation   = T_GET;
  temp_genoff_args.dal_timetick_val_valid = 0;
  temp_genoff_args.dal_timetick_val       = 0;
  time_genoff_opr( &temp_genoff_args );

  return TIME_SOURCE_32KHZ;

} /* time_get */

/*=============================================================================

FUNCTION TIME_GET_OPTIMIZED_TILL_MSEC

DESCRIPTION
  Returns the current time in msecs resolution.

DEPENDENCIES
  A valid CDMA time, HDR time, GSM time, or a Time-of-Day, etc.

RETURN VALUE
  Time-stamp returned to ts_val parameter.
  Time source used to determine time is returned

SIDE EFFECTS
  None

=============================================================================*/

time_source_enum_type time_get_optimized_till_msec
(
  /* OUT: The current time */
  time_type                       ts_val
)
{
  static time_type ts_val_msecs = {0,0};
  uint64 tt_now = 0; 
  static uint64 tt_msec_boundry = 0;

  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  /* get the current timetick */
  tt_now = timetick_sclk64_get();

  /* 
   * Check to see if the msecs boundary has been crossed or if there
   * is a time update in between, then we need to re-calculate the 
   * latest time using time_get(). 
   */
  if ( tt_now >= tt_msec_boundry )
    {
	/* Get the time */
    time_get(ts_val);
    
    /* Update the msecs ts_val value */
	ts_val_msecs[0] = ts_val[0];
	ts_val_msecs[1] = ts_val[1];
    
	/* Advance the timetick boundary to the nearest 1 msecs timetick value */
	tt_msec_boundry += 19200;
	
	/* In case the 1 msecs increment was not enough, recalculate freshly */
	if(tt_msec_boundry <= tt_now)
		{
		/* Recalculate the new boundary */
		tt_now /= 19200;
		tt_now *= 19200;
		tt_now += 19200;
		tt_msec_boundry = tt_now;
		}
    }
  else
    {
    /* we have not yet crossed the msecs boundary, so the same time can be used */
	ts_val[0] = ts_val_msecs[0];
	ts_val[1] = ts_val_msecs[1];
    }

	return TIME_SOURCE_32KHZ;

} /* time_get_optimized_till_msec */


/*=============================================================================

FUNCTION TIME_GET_MS

DESCRIPTION
  Get the system time, in # of milliseconds since "the beginning of time".

DEPENDENCIES
  A valid CDMA time, or a valid HDR time, or a valid Time-of-Day, etc.

RETURN VALUE
  Milliseconds since 6 Jan 1980 00:00:00 returned to the time parameter

SIDE EFFECTS
  None

=============================================================================*/

void time_get_ms
(
  /* Out: Time in milliseconds since 6 Jan 1980 00:00:00 */
  time_type                       ts_val
)
{
  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  /* Read the current time of day */
  (void) time_get( ts_val );

  /* Convert time-stamp format to milliseconds */
  time_conv_to_ms( ts_val );

} /* time_get_ms */



/*=============================================================================

FUNCTION TIME_SET_MS

DESCRIPTION
  Number of milliseconds since "the beginning of time" that the system time
  needs to be set to

DEPENDENCIES
  A valid CDMA time, or a valid HDR time, or a valid Time-of-Day, etc.

RETURN VALUE
  None

SIDE EFFECTS
  Timestamp changed to the a_time sent as parameter to this function.

=============================================================================*/

void time_set_ms
(
  /* In: Time in milliseconds since 6 Jan 1980 00:00:00 */
  time_type                       a_time
)
{
#ifdef FEATURE_TIME_UMTS_TARGETS 
  /* Timestamp */
  time_type                       timestamp;

  /* Arguments to be sent to set  ATS_TOD */
  time_genoff_args_struct_type    temp_genoff_args;
  

  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  /* Convert from ms to 1.25 ms units */
  qw_equ(timestamp, a_time);
  qw_mul(timestamp, timestamp, 4);
  (void) qw_div(timestamp, timestamp, 5);

  /* Shift up by 16 to convert to get timestamp */
  qw_shift(timestamp, TIME_1p25MS_TO_TS_SHIFT);


#if (defined( FEATURE_TIME_UMTS_TARGETS ) && defined(FEATURE_SEC_TIME_V2) \
      && !defined(BUILD_BOOT_CHAIN))
  time_secure_set_user_time(timestamp);
#else /* FEATURE_TIME_UMTS_TARGETS && FEATURE_SEC_TIME_V2 && !BUILD_BOOT_CHAIN */
    
  temp_genoff_args.base        = ATS_TOD; 
  temp_genoff_args.base_source = TIME_SCLK ;
  temp_genoff_args.ts_val      = &a_time;
  temp_genoff_args.unit        = TIME_MSEC;
  temp_genoff_args.operation   = T_SET;
  temp_genoff_args.dal_timetick_val_valid = 0;
  temp_genoff_args.dal_timetick_val       = 0;
  time_genoff_opr( &temp_genoff_args );
    
#endif /* FEATURE_TIME_UMTS_TARGETS && FEATURE_SEC_TIME_V2 && !BUILD_BOOT_CHAIN */

#else
  ATS_MSG("time_set_ms(): Not implemented");
#endif /* FEATURE_TIME_UMTS_TARGETS */
  
} /* time_set_ms */



/*=============================================================================

FUNCTION TIME_GET_SECS

DESCRIPTION
  Get the system time, in # of seconds since "the beginning of time".
  136 year range, beginning 6 Jan 1980 00:00:00.

DEPENDENCIES
  A valid CDMA time, or a valid HDR time, or a valid Time-of-Day, etc.

RETURN VALUE
  Seconds since 6 Jan 1980 00:00:00

SIDE EFFECTS
  None

=============================================================================*/

uint32 time_get_secs( void )
{
  /* Timestamp */
  time_type                       ts_val;

  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  /* Read the current time of day */
  (void) time_get( ts_val );

  /* Convert to seconds, and return */
  return time_conv_to_sec( ts_val );

} /* time_get_secs */


/*=============================================================================

FUNCTION TIME_GET_JULIAN

DESCRIPTION
  Get the system time, in julian format

DEPENDENCIES
  A valid CDMA time, or a valid HDR time, or a valid Time-of-Day, etc.

RETURN VALUE
  None

SIDE EFFECTS
  Modified input parameter and store julian time

=============================================================================*/
void time_get_julian
(
  /* Time value prior to updating to the correct time */
  time_julian_type *jul_time
)
{
  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  time_jul_from_secs( time_get_secs(), jul_time );

} /* time_get_julian */



/*=============================================================================

FUNCTION TIME_SET_JULIAN

DESCRIPTION
  Set the system time

DEPENDENCIES
  A valid CDMA time, or a valid HDR time, or a valid Time-of-Day, etc.

RETURN VALUE
  None

SIDE EFFECTS
  Modifies system timestamp and also updates PMIC time

=============================================================================*/
void time_set_julian
(
  /* Time value to set time to */
  time_julian_type                *jul_time
)
{
  /* Arguments to be sent to set  ATS_TOD */
  time_genoff_args_struct_type    temp_genoff_args;

  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  #if (defined( FEATURE_TIME_UMTS_TARGETS ) && defined(FEATURE_SEC_TIME_V2) \
      && !defined(BUILD_BOOT_CHAIN))
     time_secure_set_user_time_secs(secs);
  #else /* FEATURE_TIME_UMTS_TARGETS && FEATURE_SEC_TIME_V2 && !BUILD_BOOT_CHAIN */
 
      temp_genoff_args.base        = ATS_TOD; 
      temp_genoff_args.base_source = TIME_SCLK ;
      temp_genoff_args.ts_val      = jul_time;
      temp_genoff_args.unit        = TIME_JULIAN;
      temp_genoff_args.operation   = T_SET;
      temp_genoff_args.dal_timetick_val_valid = 0;
      temp_genoff_args.dal_timetick_val       = 0;
      time_genoff_opr( &temp_genoff_args );
    
  #endif /* FEATURE_TIME_UMTS_TARGETS && FEATURE_SEC_TIME_V2 && !BUILD_BOOT_CHAIN */

} /* time_set_julian */



/*=============================================================================

FUNCTION TIME_GET_UPTIME

DESCRIPTION
  Returns the uptime in timestamp format

DEPENDENCIES
  
RETURN VALUE
  Source of the time

SIDE EFFECTS
  None

=============================================================================*/

time_source_enum_type time_get_uptime
(
  time_type                       ts_val
)
{
  qw_set( ts_val, 0, 0 );
  
    /* Check if the time_genoff_client is initialized */
  if (ptime_genoff_client_initialized == TRUE)
  {
  	  /* Get uptime from sclk_time */
    time_sclk_get( ts_val, (uint64)-1);
  }

  return TIME_SOURCE_32KHZ;
} /* time_get_uptime */


/*=============================================================================

FUNCTION TIME_GET_UPTIME_SECS

DESCRIPTION
  Get time the phone has been powered on for

DEPENDENCIES
  A valid CDMA time, or a valid HDR time, or a valid Time-of-Day, etc.

RETURN VALUE
  Number of seconds phone has been powered on for.

SIDE EFFECTS
  None

=============================================================================*/

uint32 time_get_uptime_secs( void )
{
  /* System time in seconds */
  uint32                          system_time;

  /* Milliseconds phone has been powered on for */
  time_type                       uptime_ms;

  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  /* Get uptime in ms */
  time_get_uptime_ms( uptime_ms );

  /* Convert to seconds */
  (void)qw_div( uptime_ms, uptime_ms, 1000 );
  system_time = qw_lo( uptime_ms );

  /* Return calculated value */
  return system_time;

} /* time_get_uptime_secs */



/*=============================================================================

FUNCTION TIME_GET_UPTIME_MS

DESCRIPTION
  Get time the phone has been powered on for

DEPENDENCIES
  A valid CDMA time, or a valid HDR time, or a valid Time-of-Day, etc.

RETURN VALUE
  Number of milliseconds phone has been powered on for returned to 'uptime_ms'

SIDE EFFECTS
  None

=============================================================================*/

void time_get_uptime_ms
(
  /* Output: Milliseconds phone has been powered on for */
  time_type                       uptime_ms
)
{
  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  qw_set(uptime_ms, 0, 0);

  /* Get uptime in timestamp format */
  time_get_uptime( uptime_ms );

  /* Convert uptime in millisecond */
  time_conv_to_ms( uptime_ms );

} /* time_get_uptime_ms */



/*=============================================================================

FUNCTION TIME_SYNC_TIME_OF_DAY

DESCRIPTION
  Synchronize the slow-clock based time-of-day to the current "ts" time.

DEPENDENCIES
  A valid CDMA, HDR, GSM, GPS, WCDMA (etc) time.  time_get( ) must not be
  using time_tod_get( ) when this function is called.

RETURN VALUE
  None

SIDE EFFECTS
  An diagnostic message is generated if there is a sudden jump in time-of-day.

=============================================================================*/

void time_sync_time_of_day( void )
{
	//STUBBED
} /* time_sync_time_of_day */



/*=============================================================================

FUNCTION TIME_SET_FROM_PMIC

DESCRIPTION
  Initialize the slow-clock based time-of-day to the PMIC's RTC time.

DEPENDENCIES
  Time-of-day must not have been initialized be another time source.

RETURN VALUE
  None

SIDE EFFECTS
  None

=============================================================================*/

void time_set_from_pmic( void )
{
	//STUBBED 
} /* time_set_from_pmic */



/*=============================================================================

FUNCTION TIME_TO_MS_NATIVE

DESCRIPTION
  Convert a timestamp from System Time to millisecond units in native uint64
  format.

DEPENDENCIES
  None

RETURN VALUE
  The converted value.

SIDE EFFECTS
  None

=============================================================================*/

uint64 time_to_ms_native
(
  /* Time in timestamp format. */
  time_type                       time_stamp 
)
{
  /* Time in quadword format (time_type). */
  time_type                       temp_time;   

  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
   
  qw_equ( temp_time, time_stamp );

  /* Use the helper function to convert the time to milliseconds. */
  time_conv_to_ms( temp_time );

  /* Then convert to uint64 format. */
  return QW_CVT_Q2N( temp_time );

} /* time_to_ms_native */



/*=============================================================================

FUNCTION TIME_GET_MS_NATIVE

DESCRIPTION
  Get a timestamp from System Time in millisecond units from 6 Jan 1980 
  00:00:00.

DEPENDENCIES
  None

RETURN VALUE
  The time in ms from 6 Jan 1980 00:00:00.

SIDE EFFECTS
  None

=============================================================================*/

uint64 time_get_ms_native( void )
{
  /* Time in quadword format (time_type). */
  time_type                       ts_val;

  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  /* Get the current time in ms and return it in native uint64 format. */
  time_get_ms( ts_val );

  return QW_CVT_Q2N( ts_val );

} /* time_get_ms_native */

/*=============================================================================

FUNCTION TIME_GET_APPS_USER_TIME

DESCRIPTION
  Get Apps user time.

DEPENDENCIES
  Used only for WM . This will return same as time_get on Brew.

RETURN VALUE
  None

SIDE EFFECTS
  None

=============================================================================*/
void time_get_apps_user_time
(
  /* OUT: The current time */
  time_type                       ts_val
)
{
  //STUBBED  TODO
} /* time_get_apps_user_time */

/*=============================================================================
FUNCTION TIME_UPDATE_NOTIFY_CB_HELPER

DESCRIPTION
  Callback function registered with time genoff for time updates on the TOD & USER base.
  This function then calls the client callback function only for updates on the particular base
  where system time is taken from.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
=============================================================================*/
static void time_update_notify_cb_helper( uint32 data, int64 delta_ms )
{
	/* Pointer to the node element for registered callback functions */
	time_registered_cb_func_type * cb_func_node_ptr = NULL;
	time_bases_type time_get_base = ATS_MAX;
	
	time_get_base = (0 != time_genoff_get_client_offset(ATS_TOD)) ? ATS_TOD : ATS_USER;

	/* Check if the update notification callback is called for the base which is in use for system time */
	if (time_get_base != (time_bases_type)data)
		return;

	/* Call all the registered callback functions from the list of registered callback functions */
	cb_func_node_ptr = time_cb_func_list;

	while (cb_func_node_ptr != NULL)
		{
		cb_func_node_ptr->cb_func((cb_func_node_ptr->cb_data),delta_ms);
		cb_func_node_ptr = cb_func_node_ptr->next;
		}
	
	/* Return from the function */
	return;

}

/*=============================================================================
FUNCTION TIME_REGISTER_CB_FUNC

DESCRIPTION
  Registers a callback function with time services, which is 
  called whenever there is any system time update.

DEPENDENCIES
  None

RETURN VALUE
  Error return types (refer to time_svc.h for details)

SIDE EFFECTS
  None
=============================================================================*/
time_error_type time_register_cb_func (
  time_genoff_t2_cb_type cb_func,
  uint32 data )
{
	/* Pointer to the node element for registered callback functions */
	time_registered_cb_func_type * cb_func_node_ptr = NULL;
	time_registered_cb_func_type * cb_func_node_ptr_temp = NULL;

	/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

	/* Input Validation */
	if (cb_func ==  NULL)
		return TIME_FAILURE;

	ATS_INTLOCK();

	/* Register the system time bases with the time genoff to receive update notification via callback function */
	if (time_genoff_register_system_time_base == FALSE)
		{
		(void)time_genoff_register_cb(ATS_TOD, time_update_notify_cb_helper, (uint32)ATS_TOD);
		(void)time_genoff_register_cb(ATS_USER, time_update_notify_cb_helper, (uint32)ATS_USER);
		time_genoff_register_system_time_base = TRUE;
		}

	ATS_INTFREE();

	/* Allocate memory for the list element */
	if ( (cb_func_node_ptr = malloc(sizeof(time_registered_cb_func_type))) == NULL )
		return TIME_FAILURE;

	/* Memset this malloc'ed structure */
	memset(cb_func_node_ptr, 0x0, sizeof(time_registered_cb_func_type));

	/* Update details of the callback */
	cb_func_node_ptr->cb_func = cb_func;
	cb_func_node_ptr->cb_data = data;
	cb_func_node_ptr->next = NULL;

	/* Acquire mutex */
	ATS_INTLOCK();

	/* Insert into the list */
	if (time_cb_func_list == NULL)
		{
		/* The list was empty & this is the 1st element of the list */
		time_cb_func_list = cb_func_node_ptr;
		}
	else /* Traverse the list to find the appropriate place for new callback detail */
		{
		cb_func_node_ptr_temp = time_cb_func_list;
		
		while(cb_func_node_ptr_temp->next != NULL) {
			cb_func_node_ptr_temp = cb_func_node_ptr_temp->next;
			}

		/* Insert the list element */
		cb_func_node_ptr_temp->next = cb_func_node_ptr;
		}

	/* Release the mutex */	
	ATS_INTFREE();

	/* Return SUCCESS for this operation */
	return TIME_SUCCESS;
}

/* DISABLING THE DEREGISTRATION FUNCTION FOR NOW TO AVOID HANDLING OF 
CONCURRENCY CASES WHILE ACCESSING THE LIST OF REGISTERED CALLBACKS. */
#if 0
/*=============================================================================
FUNCTION TIME_DEREGISTER_CB_FUNC

DESCRIPTION
  De-registers the callback function with time services, which was 
  registered for getting called whenever there is any system time 
  update.

DEPENDENCIES
  None

RETURN VALUE
  Error return types (refer to time_svc.h for details)

SIDE EFFECTS
  None
=============================================================================*/
ats_error_type time_deregister_cb_func ( time_genoff_t2_cb_type cb_func )
{
// DISABLED
}
#endif
