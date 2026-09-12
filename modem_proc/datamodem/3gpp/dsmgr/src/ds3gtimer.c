/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

               3 G   D A T A   S E R V I C E S   M A N A G E R

                        T I M E R   F U N C T I O N S

GENERAL DESCRIPTION
  This software unit contains wrapper functions for REX timer services.

EXTERNALIZED FUNCTIONS
  ds3g_timer_init()
    Initializes timers

  ds3g_process_timer_expired_cmd()
    Called to process the DS_TIMER_EXPIRED_CMD

  ds3g_start_timer()
    Starts a specified timer

  ds3g_stop_timer()
    Stops a specified timer

  ds3g_is_timer_running()
    Queries a specified timer's state
  
INITIALIZATION AND SEQUENCING REQUIREMENTS
  ds3g_timer_init() must be called once, during DS task startup, to
  initialize the timer services.


  Copyright (c) 2001 - 2014 by Qualcomm Technologies Incorporated. All Rights Reserved.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $PVCSPath: L:/src/asw/MM_DATA/vcs/ds3gtimer.c_v   1.1   13 Aug 2002 16:24:52   sramacha  $
  $Header: //components/rel/data.mpss/3.5..2.1/3gpp/dsmgr/src/ds3gtimer.c#2 $ $DateTime: 2019/10/22 02:53:17 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ---------------------------------------------------------- 
08/27/14   tk      Fixed issue in RING URC reporting for MT CSD call.
01/02/12   msh     Coan: Feature cleanup
05/26/11   dvk     Global Variables Cleanup
03/23/11   sn      Globals cleanup for free floating of tasks.
09/09/10   op      Migrated to MSG 2.0 macros
22/10/09   ca      Fixed Lint Errors.
05/05/09   ss      Fixed Compiler Warnings.
02/25/09   sn      Fixed off-target lint errors.
07/15/08   ar      Featurized for multiprocessor build.
01/04/08   psng    Fixed lint errors.
02/14/07   vd      Lint error fixes.
07/23/06   ar      Use ds3g_siolib_current_ring_type for ring timer.  Add
                   ds3g_is_timer_running().
01/19/06   sl      Serializer related changes.
03/01/02   smp     Added DCD wink timer.
12/14/01   smp     Renamed ds3g_ring_serial_port() to ds3g_alert_user().
07/10/01   smp     Initial version.

===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/

#include "datamodem_variation.h"
#include "comdef.h"
#include "customer.h"

#include "amssassert.h"
#include "msg.h"
#include "err.h"
#include "timer.h"
#include "dstask.h"
#include "dsutil.h"
#include "ds3gmgr.h"
#include "ds3gmgrint.h"
#include "ds3gtimer.h"
#include "ds3gsiolib.h"
#include "data_err_suppr.h"
#include "data_msg.h"
#include "ds3gsiolib_ex.h"

/*===========================================================================

            LOCAL DEFINITIONS AND DECLARATIONS FOR MODULE

This section contains local definitions for constants, macros, types,
variables and other items needed by this module.

===========================================================================*/

/*---------------------------------------------------------------------------
       TIMERS
---------------------------------------------------------------------------*/
typedef struct
{
  void    *timer_ptr;          /*! timer object>*/
  boolean  is_periodic_timer;  /*!Flag to indicate Periodic timer>*/
}ds3g_timer_type;


static ds3g_timer_type ds3g_timer_tbl[DS3GSUBSMGR_SUBS_ID_MAX][DS3G_TIMER_MAX] ={{{NULL}}};


/*===========================================================================
                    Forward declarations/prototypes
===========================================================================*/
/*===========================================================================

                      INTERNAL FUNCTION DEFINITIONS

===========================================================================*/


/*===========================================================================

                      EXTERNAL FUNCTION DEFINITIONS

===========================================================================*/

/*===========================================================================
FUNCTION      DS3G_TIMER_INIT

DESCRIPTION   This function initializes the timer services. This function must 
              be called once, at Data Services Task startup.

DEPENDENCIES  None

RETURN VALUE  None

SIDE EFFECTS  None
===========================================================================*/
void  ds3g_timer_init( void )
{
  unsigned long  i;
  ds3gsubsmgr_subs_id_e_type  index;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  /*-------------------------------------------------------------------------
    Initialize timers
  -------------------------------------------------------------------------*/
  for(index = DS3GSUBSMGR_SUBS_ID_MIN; index < DS3GSUBSMGR_SUBS_ID_MAX; index++)
  {
  for (i = 0; i <(unsigned long)DS3G_TIMER_MAX; i++)
  {
      ds3g_timer_tbl[index][i].timer_ptr         = NULL;
      ds3g_timer_tbl[index][i].is_periodic_timer = FALSE;
    }
  }
  return;
} /* ds3g_timer_init() */
/*===========================================================================
FUNCTION      DS3G_TIMER_REGISTER_EX

DESCRIPTION   Clients register their timer info with timer services.
              The passed in ID and sub_id must come from the ds3g_timer_enum_type. Once
              registered, the timers cannot be de-registered (though they can
              be stopped/started).
 
              Registers the timer for correpsonding subscription. Timer is not started during
              registration

DEPENDENCIES  None

RETURN VALUE  None

SIDE EFFECTS  None
===========================================================================*/
void ds3g_timer_register_ex
(
  void                        *timer_ptr,
  void                        *timer_cb_f_ptr,
  ds3g_timer_enum_type         timer_id,
  ds3gsubsmgr_subs_id_e_type   subs_id
)
{
  if (!ds3gsubsmgr_is_ds3g_subs_index_valid(subs_id))
  {
    return;
  }

  /*-------------------------------------------------------------------------
    Validate Timer ID
  -------------------------------------------------------------------------*/
  if( (timer_id <= DS3G_TIMER_MIN) || (timer_id >= DS3G_TIMER_MAX) )
  {
    DATA_3GMGR_MSG1(MSG_LEGACY_LOW, "Invalid timer_id:%d", timer_id);
    return;
  }

  /*-------------------------------------------------------------------------
    Check if a timer object is already registered for the timer ID
  -------------------------------------------------------------------------*/
  if (ds3g_timer_tbl[subs_id][timer_id].timer_ptr != NULL)
  {
    DATA_3GMGR_MSG1(MSG_LEGACY_LOW, "Timer:%d already registered", timer_id);
    return;
  }

  /*-------------------------------------------------------------------------
    Define/Create timers based on the timer type
  -------------------------------------------------------------------------*/
  ds3g_timer_tbl[subs_id][timer_id].timer_ptr = timer_ptr;

  if( TE_SUCCESS != timer_def( timer_ptr,
                               NULL,
                               NULL,
                               0,
                               (timer_t1_cb_type)timer_cb_f_ptr,
                               (unsigned long) DS3G_TIMER_ID_FOR_SUBS_ID(subs_id,timer_id) ) )
  {
    DS_3GMGR_ERROR_LOG_1_EX(MSG_LEGACY_ERROR, subs_id, "Could not register Timer:d", timer_id);
    return;
  }

  return;
} /* ds3g_timer_register_ex() */

/*===========================================================================
FUNCTION      DS3G_TIMER_REGISTER

DESCRIPTION   Clients register their timer info with timer services.
              The passed in ID must come from the ds3g_timer_enum_type. Once
              registered, the timers cannot be de-registered (though they can
              be stopped/started).

              Registers the timer . Timer is not started during
              registration

DEPENDENCIES  None

RETURN VALUE  None

SIDE EFFECTS  None
===========================================================================*/
void  ds3g_timer_register
(
  void                  *timer_ptr,
  void                  *timer_cb_f_ptr,
  ds3g_timer_enum_type   timer_id
)
{
  ds3g_timer_register_ex(timer_ptr,
                         timer_cb_f_ptr,
                         timer_id,
                         DS3GSUBSMGR_SUBS_ID_1);

} /* ds3g_timer_register() */
/*===========================================================================
FUNCTION      DS3G_TIMER_START_EX

DESCRIPTION   This function starts the specified timer on a correspodning subscription 
              for the specified  duration.

DEPENDENCIES  A valid registered timer id, subs_id must be specified, otherwise the 
              timer is not started.

RETURN VALUE  TRUE:  Start timer successful
              FALSE: Start timer failed

SIDE EFFECTS  None
===========================================================================*/
boolean ds3g_timer_start_ex
(
  ds3g_timer_enum_type  timer_id,
  dword                        duration,
  ds3gsubsmgr_subs_id_e_type   subs_id
)
{
  boolean  ret_val = FALSE;
  uint8    periodicity = 0;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  do
  {
    if (!ds3gsubsmgr_is_ds3g_subs_index_valid(subs_id))
    {
      ret_val = FALSE;
      break;
    }

    /*-----------------------------------------------------------------------
        Validate Timer ID
    -----------------------------------------------------------------------*/
    if( (timer_id <= DS3G_TIMER_MIN) || (timer_id >= DS3G_TIMER_MAX) )
    {
      DATA_3GMGR_MSG1(MSG_LEGACY_LOW, "Invalid timer_id:%d", timer_id);
      ret_val = FALSE;
      break;
    }

    /*-----------------------------------------------------------------------
      Validate Timer object
    -----------------------------------------------------------------------*/
    if (ds3g_timer_tbl[subs_id][timer_id].timer_ptr == NULL)
    {
      DATA_3GMGR_MSG1(MSG_LEGACY_LOW, "Trying to start unregistered timer:%d",
                      timer_id);
      ret_val = FALSE;
      break;
    }

    /*-----------------------------------------------------------------------
      Start timer
    -----------------------------------------------------------------------*/
    periodicity = (ds3g_timer_tbl[subs_id][timer_id].is_periodic_timer  == FALSE)?0:1;
    if( TE_SUCCESS != timer_set( ds3g_timer_tbl[subs_id][timer_id].timer_ptr,
                                 (timetick_type)duration,
                                 (timetick_type)duration * periodicity,
                                 T_MSEC) )
    {
      DS_3GMGR_ERROR_LOG_1_EX(MSG_LEGACY_ERROR, subs_id, "Timer:d start failed", timer_id);
      ret_val = FALSE;
      break;
    }

    DATA_3GMGR_MSG1_EX(MSG_LEGACY_MED, 
                       subs_id,
                       "Started Timer %d", 
                       timer_id);
    ret_val = TRUE;
  }while(0);

  return ret_val;
} /* ds3g_timer_start_ex() */

/*===========================================================================
FUNCTION      DS3G_TIMER_START

DESCRIPTION   This function starts the specified timer for the specified 
               duration.

DEPENDENCIES  A valid registered timer id must be specified, otherwise the 
              timer is not started.

RETURN VALUE  TRUE:  Start timer successful
              FALSE: Start timer failed

SIDE EFFECTS  None
===========================================================================*/
boolean ds3g_timer_start
(
  ds3g_timer_enum_type  timer_id,
  dword                 duration
)
{
  boolean  ret_val = FALSE;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  ret_val = ds3g_timer_start_ex(timer_id,
                                duration,
                                DS3GSUBSMGR_SUBS_ID_1);
  return ret_val;
}
/*===========================================================================
FUNCTION      DS3G_TIMER_STOP_EX

DESCRIPTION   This function stops the specified timer for a particular 
              subscription.

DEPENDENCIES  A valid registered timer id must be specified, otherwise 
              no action is taken.

RETURN VALUE  TRUE:  Stop timer successful
              FALSE: Stop timer failed

SIDE EFFECTS  None
===========================================================================*/
boolean ds3g_timer_stop_ex
(
  ds3g_timer_enum_type        timer_id,
  ds3gsubsmgr_subs_id_e_type  subs_id
)
{
  uint32   tmsec_left = 0;
  boolean  ret_val = FALSE;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  do
  {
    if (!ds3gsubsmgr_is_ds3g_subs_index_valid(subs_id))
    {
      ret_val = FALSE;
      break;
    }

    /*-----------------------------------------------------------------------
      Validate Timer ID
    -----------------------------------------------------------------------*/
    if( (timer_id <= DS3G_TIMER_MIN) || (timer_id >= DS3G_TIMER_MAX) )
    {
      DATA_3GMGR_MSG1(MSG_LEGACY_LOW, "Invalid timer id:%d", timer_id);
      ret_val = FALSE;
      break;
    }

    /*-----------------------------------------------------------------------
      Validate Timer object
    -----------------------------------------------------------------------*/
    if( ds3g_timer_tbl[subs_id][timer_id].timer_ptr == NULL )
    {
      DATA_3GMGR_MSG1(MSG_LEGACY_LOW, "Trying stop unregistered timer:%d",
                      timer_id);
      ret_val = FALSE;
      break;
    }

    /*-----------------------------------------------------------------------
      Check if the timer has already stopped
    -----------------------------------------------------------------------*/
    if (ds3g_timer_get_status_ex(timer_id,subs_id) == FALSE)
    {
      DATA_3GMGR_MSG1(MSG_LEGACY_LOW,
                      "Timer not running Skip stopping timer %d",timer_id);
      ret_val = TRUE;
      break;
    }

    /*-----------------------------------------------------------------------
      Stop the timer
    -----------------------------------------------------------------------*/
    tmsec_left = (uint32)timer_clr( ds3g_timer_tbl[subs_id][timer_id].timer_ptr,
                                    T_MSEC );
    DATA_3GMGR_MSG2_EX(MSG_LEGACY_MED,
                       subs_id,
                       "Timer ID %d stopped before %d msec to expire",
                       timer_id, 
                       tmsec_left);
    ret_val = TRUE;
  }while(0);

  return ret_val;
} /* ds3g_timer_stop_ex() */

/*===========================================================================
FUNCTION      DS3G_TIMER_STOP

DESCRIPTION   This function stops the specified timer.

DEPENDENCIES  A valid registered timer id must be specified, otherwise 
              no action is taken.

RETURN VALUE  TRUE:  Stop timer successful
              FALSE: Stop timer failed

SIDE EFFECTS  None
===========================================================================*/
boolean ds3g_timer_stop
(
  ds3g_timer_enum_type  timer_id
)
{
  boolean  ret_val = FALSE;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  ret_val = ds3g_timer_stop_ex(timer_id,
                               DS3GSUBSMGR_SUBS_ID_1);
  return ret_val;
}
/*===========================================================================
FUNCTION      DS3G_TIMER_GET_STATUS_EX

DESCRIPTION   This function reports status of specified timer for the 
              corresponding subscription

DEPENDENCIES  A valid timer id, subd_id must be specified, otherwise no action is taken.

RETURN VALUE  TRUE:  timer is active and running
              FALSE: otherwise

SIDE EFFECTS  None
===========================================================================*/
boolean ds3g_timer_get_status_ex
(
  ds3g_timer_enum_type  timer_id,
  ds3gsubsmgr_subs_id_e_type   subs_id
)
{
  boolean ret_val = FALSE;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  do
  {
    if (!ds3gsubsmgr_is_ds3g_subs_index_valid(subs_id))
    {
      ret_val = FALSE;
      break;
    }

    /*-----------------------------------------------------------------------
        Validate Timer ID
    -----------------------------------------------------------------------*/
    if( (timer_id <= DS3G_TIMER_MIN) ||(timer_id >= DS3G_TIMER_MAX) )
    {
      DATA_3GMGR_MSG1(MSG_LEGACY_LOW, "Invalid timer id: %d", timer_id);
      ret_val = FALSE;
      break;
    }

    /*-----------------------------------------------------------------------
      Validate Timer object
    -----------------------------------------------------------------------*/
    if( ds3g_timer_tbl[subs_id][timer_id].timer_ptr == NULL )
    {
      DATA_3GMGR_MSG1(MSG_LEGACY_LOW,
                      "Cannot check status of unregistered timer:%d",
                      timer_id);
      ret_val = FALSE;
      break;
    }

    /*-------------------------------------------------------------------------
      Check if the timer is running at present
    -------------------------------------------------------------------------*/
      ret_val = (0 != rex_get_timer(ds3g_timer_tbl[subs_id][timer_id].timer_ptr));
  }while(0);

  return ret_val;
} /* ds3g_timer_get_status() */

/*===========================================================================
FUNCTION      DS3G_TIMER_GET_STATUS

DESCRIPTION   This function reports status of specified timer.

DEPENDENCIES  A valid timer id must be specified, otherwise no action is taken.

RETURN VALUE  TRUE:  timer is active and running
              FALSE: otherwise

SIDE EFFECTS  None
===========================================================================*/
boolean ds3g_timer_get_status
( 
  ds3g_timer_enum_type  timer_id
)
{
  boolean  ret_val = FALSE;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  ret_val= ds3g_timer_get_status_ex(timer_id,
                                    DS3GSUBSMGR_SUBS_ID_1);
  return ret_val;

}
/*===========================================================================
FUNCTION      DS3G_TIMER_CONFIGURE_PERIODICITY_EX

DESCRIPTION   This utility can be used to set (or) unset the periodicity of 
              the timer. (one time (or) recurring timer runs)

DEPENDENCIES  Currently used for MCS timer types only

RETURN VALUE  TRUE:  Timer periodicity configured succesful
              FALSE: Timer periodicity configured failed

SIDE EFFECTS  None
===========================================================================*/
boolean ds3g_timer_configure_periodicity_ex
( 
  ds3g_timer_enum_type  timer_id,
  boolean               is_periodic_tmr,
  ds3gsubsmgr_subs_id_e_type   subs_id
)
{
  boolean ret_val = FALSE;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  do
  {
    if (!ds3gsubsmgr_is_ds3g_subs_index_valid(subs_id))
    {
      ret_val = FALSE;
      break;
    }
    /*-----------------------------------------------------------------------
      Validate Timer ID
    -----------------------------------------------------------------------*/
    if( (timer_id <= DS3G_TIMER_MIN) ||
        (timer_id >= DS3G_TIMER_MAX) )
    {
      DATA_3GMGR_MSG1(MSG_LEGACY_LOW, "Invalid timer id:%d", timer_id);
      ret_val = FALSE;
    }

    /*-----------------------------------------------------------------------
      Validate Timer object
    -----------------------------------------------------------------------*/
    if( ds3g_timer_tbl[subs_id][timer_id].timer_ptr == NULL )
    {
      DATA_3GMGR_MSG1(MSG_LEGACY_LOW,
                      "Trying to set periodicity for unregistered timer:%d",
                      timer_id);
      ret_val = FALSE;
      break;
    }

    /*-----------------------------------------------------------------------
      Set the periodicity flag
    -----------------------------------------------------------------------*/
    DS_3GMGR_ERROR_LOG_2_EX(MSG_LEGACY_ERROR, subs_id, "Setting periodicity:d for timer:d",
                    is_periodic_tmr, timer_id);
    ds3g_timer_tbl[subs_id][timer_id].is_periodic_timer = is_periodic_tmr;
    ret_val = TRUE;
  }while(0);

  return ret_val;
} /* ds3g_timer_configure_periodicity() */

/*===========================================================================
FUNCTION      DS3G_TIMER_CONFIGURE_PERIODICITY

DESCRIPTION   This utility can be used to set (or) unset the periodicity of 
              the timer. (one time (or) recurring timer runs)

DEPENDENCIES  Currently used for MCS timer types only

RETURN VALUE  TRUE:  Timer periodicity configured succesful
              FALSE: Timer periodicity configured failed

SIDE EFFECTS  None
===========================================================================*/
boolean ds3g_timer_configure_periodicity
( 
  ds3g_timer_enum_type  timer_id,
  boolean               is_periodic_tmr
)
{
  boolean  ret_val = FALSE;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  ret_val = ds3g_timer_configure_periodicity_ex(timer_id,
                                      is_periodic_tmr,
                                      DS3GSUBSMGR_SUBS_ID_1);

  return ret_val;
} /* ds3g_timer_configure_periodicity() */

