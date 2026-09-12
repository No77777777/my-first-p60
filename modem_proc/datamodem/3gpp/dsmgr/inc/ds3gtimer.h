#ifndef DS3GTIMER_H
#define DS3GTIMER_H
/*===========================================================================

               3 G   D A T A   S E R V I C E S   M A N A G E R

                        T I M E R   F U N C T I O N S

                            H E A D E R    F I L E

DESCRIPTION
  This file contains wrapper functions for REX timer services. These functions
  may be used by software components within the Data Services Task.

  In order to add a new timer, the timer id must be defined below, the timer
  must be initialized in ds3g_timer_init(), and logic to handle the timer
  expiration must be added to ds3g_process_timer_expired_cmd().


  Copyright (c) 2001 - 2018 by Qualcomm Technologies Incorporated. All Rights Reserved.
===========================================================================*/


/*===========================================================================

                      EDIT HISTORY FOR FILE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $PVCSPath: L:/src/asw/MM_DATA/vcs/ds3gtimer.h_v   1.0   08 Aug 2002 11:19:50   akhare  $
  $Header: //components/rel/data.mpss/3.5..2.1/3gpp/dsmgr/inc/ds3gtimer.h#2 $ $DateTime: 2019/10/22 02:53:17 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
11/08/12   ssb     Dun Call Entitlement support using PS_SYS Interface
01/02/12   msh     Coan: Feature cleanup
10/17/11   ssh     eHRPD Always On support
06/06/11   ash     Added support for PPP Partial Context
22/10/09   ca      Fixed Lint Errors.
03/09/09   sa      AU level CMI modifications.
01/10/07   sq      Added DS3G_TIMER_RETRY_DELAY_ORIG
07/26/06   ar      Add ds3g_is_timer_running().
04/05/05   gr      Integrated EPZID functionality into main line
03/01/05   kvd     Added timers for RDUD module.
11/30/04   kvd     Added timers for CAM module.
03/22/04   gr      Added Abort Dormant PPP timer
10/01/03   ak      Added PZID delay timer.
09/02/03   sy      Added pzid hysteresis timer.
07/13/02   atp     Added 707 async timers.
03/05/02   smp     Added definition for DCD wink timer.
07/10/01   smp     Initial version.

===========================================================================*/


/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/

#include "comdef.h"
#include "customer.h"

#include "dstask_v.h"
#include "ds3gsubsmgr.h"

/*===========================================================================

                      PUBLIC DATA DECLARATIONS

===========================================================================*/


/*---------------------------------------------------------------------------
  Macro to get timer id for subscription id
    timer_id per subs_id = (subs_id * 1000)+timer_id
---------------------------------------------------------------------------*/
#define DS3G_TIMER_ID_FOR_SUBS_ID(subs_id, timer_id) \
          ((unsigned long)(subs_id*1000) + timer_id)

/*---------------------------------------------------------------------------
  Macro to get subscription id from timer id for 
    subs_id = (timer_id / 1000)
---------------------------------------------------------------------------*/
#define DS3G_SUBS_ID_FROM_TIMER_ID(timer_id) (timer_id/1000)

/*---------------------------------------------------------------------------
  Macro to get timer enum from timer id for 
    subs_id = (timer_id / 1000)
---------------------------------------------------------------------------*/
#define DS3G_TIMER_ENUM_FROM_TIMER_ID(timer_id,subs_id) \
    (timer_id - 1000*subs_id)

/*--------------------------------------------------------------------------
Timer duration for OTASN timer in secs (30 is default)
----------------------------------------------------------------------------*/
#ifdef TEST_FRAMEWORK
#error code not present
#else
#define OTASN_PDN_TIMER_VALUE  30 
#endif /* TEST_FRAMEWORK */

/*---------------------------------------------------------------------------
  List of timer ids across Data.  No enum's as the actual timers are
  defined in the technology-specific modules, and then registered into an
  array that is kept centrally.
---------------------------------------------------------------------------*/
typedef enum
{
  DS3G_TIMER_MIN = -1,       /* minimum value - used for bounds checking   */

  /*-------------------------------------------------------------------------
     GENERIC TIMERS
  -------------------------------------------------------------------------*/
  DS3G_TIMER_RING_TE_1,        /* timer used to ring the laptop            */
  DS3G_TIMER_WINK_DCD_1,       /* timer used to wink DCD                   */

  DS3G_TIMER_RING_TE_2,        /* timer used to ring the laptop            */
  DS3G_TIMER_WINK_DCD_2,       /* timer used to wink DCD                   */

  /*-------------------------------------------------------------------------
     IS707 TIMERS
  -------------------------------------------------------------------------*/
  DS3G_TIMER_CTA,
  DS3G_TIMER_HOLDDOWN,
  DS3G_TIMER_EPZID_HYSTERESIS,
  DS3G_TIMER_EPZID_DELAY,
  DS3G_TIMER_RETRY_DELAY_ORIG,
  DS3G_TIMER_PKT_AUTO_REDIAL,
  DS3G_DELAY_OTASN_TEARDOWN_TIMER,
  /*-------------------------------------------------------------------------
     IS707 ASYNC TIMERS
  -------------------------------------------------------------------------*/
  DS3G_TIMER_ASYNC_PTCL_OPENING,

  /*-------------------------------------------------------------------------
     DORMANT PPP ABORT TIMER
  -------------------------------------------------------------------------*/
  DS3G_TIMER_ABORT_DORMANT_PPP,

  /*-------------------------------------------------------------------------
    Timers for CAM module-CAM timer for CAM to kick-in and flow ctrl timer
  -------------------------------------------------------------------------*/
  DS3G_TIMER_CAM,
  DS3G_TIMER_CAM_FLOW_CTRL,

  /*-------------------------------------------------------------------------
    Timers for RDUD module-RDUD timer started to revert to default CTA timeou
    after certian time period.
  -------------------------------------------------------------------------*/
  DS3G_TIMER_RDUD,

  /*-------------------------------------------------------------------------
    eHRPD partial context timers
  -------------------------------------------------------------------------*/
  DS3G_TIMER_MPIT,
  DS3G_TIMER_PCM,
  
  /*-------------------------------------------------------------------------
    eHRPD Always On Link status determination timer
  -------------------------------------------------------------------------*/
  DS3G_TIMER_LSD,

  /*-------------------------------------------------------------------------
    DUN Control Entitlement Response Waiting Timer
  -------------------------------------------------------------------------*/
  DS3G_TIMER_DUN_CTRL_ENTL,

  /*-------------------------------------------------------------------------
    Timers to generate Throughput Information per subscription
  -------------------------------------------------------------------------*/
  DS3G_TIMER_REPORT_TPUT_INFO_IND,

  /*-------------------------------------------------------------------------
   Timers to refresh actual throughput Information per subscription
  -------------------------------------------------------------------------*/
  DS3G_TIMER_REFRESH_ACTUAL_TPUT,

  /*-------------------------------------------------------------------------
   Timers to downlink throughput estimation Information per subscription
  -------------------------------------------------------------------------*/
  DS3G_TIMER_DOWNLINK_THROUGHPUT_ESTIMATION,

  /*-------------------------------------------------------------------------
   Timers for uplink downlink throughput  Information per subscription
  -------------------------------------------------------------------------*/  
  DS3G_TIMER_ULDL_THROUGHPUT,

  /*-------------------------------------------------------------------------
   Timers for uplink throughput  Information per subscription
  -------------------------------------------------------------------------*/
  DS3G_TIMER_UPLINK_THROUGHPUT,

  /*-------------------------------------------------------------------------
   Timers for downlink throughput threshold report per subscription
  -------------------------------------------------------------------------*/
  DS3G_TIMER_DOWNLINK_THRESHOLD_REPORT,

  /*-------------------------------------------------------------------------
   Timers for uplink throughput threshold report per subscription
  -------------------------------------------------------------------------*/
  DS3G_TIMER_UPLINK_THRESHOLD_REPORT,

  /*-------------------------------------------------------------------------
   Timers for MT CSFB call to establish.  Information per subscription
  -------------------------------------------------------------------------*/
  DS3G_TIMER_MT_CSFB_ESTAB_HYSTERESIS,

  DS3G_TIMER_MAX             /* maximum value - used for bounds checking   */
} ds3g_timer_enum_type;

/*===========================================================================

                      PUBLIC FUNCTION DECLARATIONS

===========================================================================*/

/*===========================================================================
FUNCTION      DS3G_TIMER_INIT

DESCRIPTION   This function initializes the timer services. This function must 
              be called once, at Data Services Task startup.

DEPENDENCIES  None

RETURN VALUE  None

SIDE EFFECTS  None
===========================================================================*/
void ds3g_timer_init( void );

/*===========================================================================
FUNCTION      DS3G_TIMER_REGISTER_EX

DESCRIPTION   Clients register their timer info with timer services.
              The passed in ID and sub_id must come from the
              ds3g_timer_enum_type. Once registered, the timers cannot be
              de-registered (though they can be stopped/started).
 
              Registers the timer for correpsonding subscription.
              Timer is not started during registration

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
);

/*===========================================================================
FUNCTION      DS3G_TIMER_REGISTER

DESCRIPTION   Clients register their timer info with timer services.
              The passed in ID must come from the ds3g_timer_enum_type. Once
              registered, the timers cannot be de-registered (though they can
              be stopped/started).
 
              Registers the timer with REX . Timer is not started during
              registration

DEPENDENCIES  None

RETURN VALUE  None

SIDE EFFECTS  None
===========================================================================*/
void ds3g_timer_register
(
  void                  *timer_ptr,
  void                  *timer_cb_f_ptr,
  ds3g_timer_enum_type   timer_id
);

/*===========================================================================
FUNCTION      DS3G_TIMER_START_EX

DESCRIPTION   This function starts the specified timer on a correspodning 
              subscription for the specified  duration.

DEPENDENCIES  A valid registered timer id, subs_id must be specified, 
              otherwise the timer is not started.

RETURN VALUE  TRUE:  Start timer successful
              FALSE: Start timer failed

SIDE EFFECTS  None
===========================================================================*/
boolean ds3g_timer_start_ex
(
  ds3g_timer_enum_type         timer_id,
  dword                        duration,
  ds3gsubsmgr_subs_id_e_type   subs_id
);

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
);
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
  ds3g_timer_enum_type         timer_id,
  ds3gsubsmgr_subs_id_e_type   subs_id
);

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
);

/*===========================================================================
FUNCTION      DS3G_TIMER_GET_STATUS_EX

DESCRIPTION   This function reports status of specified timer for the 
              corresponding subscription

DEPENDENCIES  A valid timer id, subd_id must be specified, 
              otherwise no action is taken.

RETURN VALUE  TRUE:  timer is active and running
              FALSE: otherwise

SIDE EFFECTS  None
===========================================================================*/
boolean ds3g_timer_get_status_ex
(
  ds3g_timer_enum_type  timer_id,
  ds3gsubsmgr_subs_id_e_type   subs_id
);

/*===========================================================================
FUNCTION      DS3G_TIMER_GET_STATUS

DESCRIPTION   This function reports status of specified timer.

DEPENDENCIES  A valid timer id must be specified, otherwise no action is taken.

RETURN VALUE  TRUE if timer is active, FALSE otherwise

SIDE EFFECTS  None
===========================================================================*/
boolean ds3g_timer_get_status
(
  ds3g_timer_enum_type  timer_id
);
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
  boolean                      is_periodic_tmr,
  ds3gsubsmgr_subs_id_e_type   subs_id
);
/*===========================================================================
FUNCTION      DS3G_TIMER_CONFIGURE_PERIODICITY

DESCRIPTION   This utility can be used to set (or) unset the periodicity of 
              the timer. (one time (or) recurring timer runs)

DEPENDENCIES  None

RETURN VALUE  None

SIDE EFFECTS  None
===========================================================================*/
boolean ds3g_timer_configure_periodicity
( 
  ds3g_timer_enum_type  timer_id,
  boolean               is_periodic_tmr
);

#endif /* DS3GTIMER_H */
