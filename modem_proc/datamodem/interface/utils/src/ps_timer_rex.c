/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*
       D A T A   S E R V I C E S   A P I  T I M E R

GENERAL DESCRIPTION
  This is the platform specific source file for managing the PS timer API. 
  The implementation utilizes the REX Timer APIs.

Copyright (c) 2009 Qualcomm Technologies Incorporated.
All Rights Reserved.
Qualcomm Confidential and Proprietary
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                      INCLUDE FILES FOR MODULE

===========================================================================*/
#include "amssassert.h"
#include "comdef.h"
#include "ps_platform_timer.h"
#include "pstimer.h"
#include "timetick.h"
#include "timer.h"
#include "ps_system_heap.h"
#include "ds_Utils_DebugMsg.h"

/*=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

                        LOCAL DECLARATIONS FOR MODULE

=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=*/
/*----------------------------------------------------------------------
Initialize the max rex timer value

The maximum supported REX timer length is determined by calling the
slow clock API. This function determines the number of slow clocks
based on the current slow clock frequency.

PS timers use a value of 0xFD000000 of the maximum value. This prevents
an overflow from occuring when the clock frequency changes. Drift of
.1% can result in 86 secs over 24 hours if the mobile does not have
access to network time. Using this % helps to reduce that drift.
----------------------------------------------------------------------*/
#define MAX_REX_TIMER_PERCENTAGE 0xFD000000

/*=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

                         LOCAL FUNCTION DECLARATIONS

=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=*/
/*===========================================================================
FUNCTION PS_PLATFORM_TIMER_DEF()

DESCRIPTION
  This function takes a timer structure, callback pointer, and timer handle
  from the common layer.
  Defines/Initializes timer in platform specific layer.

DEPENDENCIES
  None

PARAMETERS
  timer: timer structure holding parameters required for platform timer APIs   
  ps_timer_handler_cb_ptr: callback pointer
  timer_handle: timer handle used in platform specific layer

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void ps_platform_timer_def
(
  void *   timer_ptr,
  void *   ps_timer_expire_cb,
  void *   cb_data
)
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if (NULL == ps_timer_expire_cb)
  {
    DATA_ERR_FATAL("ps_platform_timer_def(): NULL arguments specified");
    return;
  }

  /*-------------------------------------------------------------------------
  Notify type is set to TIMER_FUNC2_CB_TYPE since it supports client 
  call back data in callback function
  -------------------------------------------------------------------------*/
  timer_def_osal ((timer_ptr_type)timer_ptr,
                   NULL, 
                   TIMER_FUNC2_CB_TYPE,
                   ps_timer_expire_cb,
                  (time_osal_notify_data)cb_data);
    

}/*ps_platform_timer_def*/

/*===========================================================================
FUNCTION PS_PLATFORM_TIMER_SET()

DESCRIPTION
  This function takes a timer structure from the common layer.  
  Calls platform specific timer set API.  The void pointer is casted 
  to platform specific timer structure in platform layer.

DEPENDENCIES
  None

PARAMETERS
  timer: timer structure holding parameters required for platform timer APIs   
  tiemr_val: event delay value

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void ps_platform_timer_set
(
  void *                  timer_handle, 
  uint64                  timer_val,
  void *                  cb_data
)
{
  timer_attrib_struct_type         timer_attrib;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  memset (&timer_attrib , 0x0, sizeof(timer_attrib));

  /*-------------------------------------------------------------------------
  Get timer attribute corresponding to timer handle
  -------------------------------------------------------------------------*/
  timer_get_params((timer_type *) timer_handle,
                                  &timer_attrib);
  
  /*-------------------------------------------------------------------------
    Set timer value into call back user data
    -------------------------------------------------------------------------*/
  timer_attrib.data = (timer_cb_data_type) cb_data;
    
  timer_set_params((timer_type *) timer_handle,
                                  TIMER_UPDATE_NOTIFICATION_PARAMS,
                                  &timer_attrib);

  /*-------------------------------------------------------------------------
    Periodicity is set to 0 since PS timers don't support setting a 
    periodic timer. Call timer API for MicroSecond granularity 
  -------------------------------------------------------------------------*/
  
  timer_set_64( (timer_type *) timer_handle,
                               timer_val,
                               0,
                               T_USEC );

  LOG_PSTIMER_MSG_INFO1_1("ps_platform_timer_set  %ld", timer_val);

}/*ps_platform_timer_set*/

/*===========================================================================
FUNCTION PS_PLATFORM_TIMER_IS_ACTIVE()

DESCRIPTION
  This function takes a timer structure from the common layer.
  Returns true if timer is still running, returns false otherwise.

DEPENDENCIES
  None

PARAMETERS
  timer: timer structure holding parameters required for platform timer APIs   

RETURN VALUE
  True: Timer is running
  False: Timer is stopped

SIDE EFFECTS
  None
===========================================================================*/
int ps_platform_timer_is_active
(
  void *                  timer
)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  return TIMER_IS_ACTIVE ((timer_type *)timer);

}/*ps_platform_timer_is_active*/

/*===========================================================================
FUNCTION PS_PLATFORM_TIMER_CLR()

DESCRIPTION
  This function takes a timer structure from the common layer.
  Stops specified timer and remove it from the active timer list.

DEPENDENCIES
  None

PARAMETERS
  timer: timer structure holding parameters required for platform timer APIs

RETURN VALUE
  Number of ticks, remaining before expiry.

SIDE EFFECTS
  None
===========================================================================*/
uint64 ps_platform_timer_clr
(
  void *                  timer
)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  LOG_PSTIMER_MSG_INFO1_0("ps_platform_timer_clr ");

  return timer_clr_64 ( (timer_type *)timer,
                         T_USEC );
}/*ps_platform_timer_clr*/

/*=============================================================================
FUNCTION PS_PLATFORM_TIMER_GET_MAX_SUPPORTED_TIMER()

DESCRIPTION
  Converts the slow clock time value to the given unit

DEPENDENCIES
  Valid sclk estimate

RETURN VALUE
  Time in the unit requested

SIDE EFFECTS
  None

=============================================================================*/
uint64 ps_platform_timer_get_max_supported_timer
(
)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
        
  return timetick_cvt_from_sclk64(MAX_REX_TIMER_PERCENTAGE, T_USEC);
  
}/*ps_platform_timer_get_max_supported_timer*/

/*===========================================================================
FUNCTION PS_PLATFORM_TIMER_GET()

DESCRIPTION
  This function takes a timer structure from the common layer.
  Checks the timer to determine how much time is left.

DEPENDENCIES
  None

PARAMETERS
  timer: timer structure holding parameters required for platform timer APIs

RETURN VALUE
  returns time left in the specified timer.

SIDE EFFECTS
  None
===========================================================================*/
uint64 ps_platform_timer_get
(
  void *    timer
)
{
   uint64  ret_val = 0;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
   ret_val = timer_get_64( (timer_type *)timer, T_USEC );

   LOG_PSTIMER_MSG_INFO1_1("ps_platform_timer_get  %ld", ret_val);

   return ret_val;
}/*ps_platform_timer_get*/

/*===========================================================================
FUNCTION PS_PLATFORM_TIMER_FREE()

DESCRIPTION
  This function takes a timer structure from the common layer.
  Timer structure will be deallocated.
  
DEPENDENCIES
  None

PARAMETERS
  timer: timer structure holding parameters required for platform timer APIs

RETURN VALUE
  None

SIDE EFFECTS
  NOne
===========================================================================*/
void ps_platform_timer_free
(
  void *      timer_ptr
)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  if (ps_platform_timer_is_active( (timer_type *) (timer_ptr) ))
  {
    (void)ps_platform_timer_clr(timer_ptr);
  }
  
  timer_undef((timer_type *) (timer_ptr));
  
}/*ps_platform_timer_free*/


