/*===========================================================================

  Copyright (c) 2013 - 2020 Qualcomm Technologies, Inc.
  All Rights Reserved Qualcomm Proprietary

  Export of this technology or software is regulated by the U.S. Government.
  Diversion contrary to U.S. law prohibited.

  All ideas, data and information contained in or disclosed by
  this document are confidential and proprietary information of
  Qualcomm Technologies, Inc. and all rights therein are expressly reserved.
  By accepting this material the recipient agrees that this material
  and the information contained therein are held in confidence and in
  trust and will not be used, copied, reproduced in whole or in part,
  nor its contents revealed in any manner to others without the express
  written permission of Qualcomm Technologies, Inc.

===========================================================================*/

/*===========================================================================

                        EDIT HISTORY FOR FILE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/1x.mpss/8.0/drivers/src/m1x_stmr.c#2 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
01/22/20   vaa     Implement QSH MDUMP framework
11/29/16   srk     Added new events for paging and sync SW DEMBACK decode ISRs.
07/14/16   bb/ab   Changes to provide snapshot for LTE_D
04/21/16   srk     1x2GTA cleanup and TX power shoot fixes.
04/14/16   srk     Optimized TX Trigger fixes and code cleanup.
05/05/15   eye     Added VFR_SEL_1 register change when 1x is on sub-2.
05/05/15   pap     Ensure registering the same event and client with new
                   callback is updated to m1x_stmr event_callback array.
04/27/15   pap     Resolving m1x_stmr race condition to ensure critical
                   section will not lock with deregister request
04/21/15   srk     DRDSDS and BOLT REVISIT Feature cleanup
04/12/15   pap     Adding changes to ensure that variable subscription ID are
                   supported by m1x_stmr correctly.
10/29/14   pap     Adding changes for Jolokia in order to support VFR_SEL reg.
10/11/14   pap     Improve logging for m1x_stmr module.
10/10/14   pap     Resolving FO being incorrectly set for VFR.
06/28/14   ssh     Resolving issue to schedule new frame events when frame
                   offset is present.
06/09/14   pap     Cleaning the m1x_stmr module.
04/23/14   pap     Adding changes to correct call to API vstmr_vfr_allocate.
04/17/14   cjb     Clean-up related to using 1x MSG macros for printing F3s
04/04/14   pap     Fixing Klockwork issues.
03/18/14   pap     Fixing deinit function to release mutex before deleting
                   critical section.
03/13/14   cjb     New API that returns entire scomb value read from vstmr
03/11/14   ssh     BO CGPS changes for TTR.
02/04/14   pap     Frame offset is not being correctly set when no frame events
                   are active.
02/03/14   pap     Resolving a race condition between 'srch' task and ISR call.
01/29/14   pap     Adding changes to convert PN roll event to a periodic event
                   and an API to re-register all active events in order to
                   stay in sync with FW after a view offset change.
01/27/14   pap     Get functions for current PCG number need to be adjusted
                   for any applied frame offset correctly.
01/14/14   pap     Modify 1x for chipx8 compensation due to updated MCS API.
12/17/13   pap     Initialized dynamic memory inside init to avoid crash.
12/10/13   pap     Added API to provide LTE and 1x synchronized time.
11/16/13   bb      Changes to fix M1X STMR mutex leaks
11/14/13   bb      Changes to register ROLL interrupts as single shot
                   instead of periodic to avoid issue during combiner
                   slam scenarios
11/08/13   cjb     Fixed DEINIT function. Never de-initialize m1x_stmr.
11/07/13   pap     Correcting code to avoid spurious events from being
                   registered.
10/23/13   pap     Correcting value returned from get RTC function to unadjusted
                   RTC value.
10/17/13   pap     Adding changes for VFR allocations.
10/14/13   pap     Corrected error in PCG number get functions.
10/02/13   pap     Removing critical section locks inside 'get' functions.
09/30/13   pap     Updating the design of the module to have more manageable
                   functions.
09/30/13   cjb     Avoid mutex lock-up with M1X_TIME
09/20/13   pap     Modifications to fix heartbeat functionality and creating
                   related function.
09/10/13   pap     Added changes for a heartbeat check and triggering PCG
                   offset at the start of the frame from 1x layer1 perspective
09/09/13   pap     Added functions to get various current time parameters.
08/29/13   pap     Fixed error allocate cbf_array correctly during init.
07/07/13   pap     Initial revision.

===========================================================================*/


/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/

/*Common*/
#include "1x_variation.h"
#include "comdef.h"
#include "customer.h"
#include "modem_1x_defs.h"
#include "modem_1x_utils.h"

/*1x driver*/
#include "m1x_hwio_mpss.h"
#include "m1x_stmr_i.h"

/*MCS*/
#include "vstmr.h"
#include "vstmr_1x.h"
#include "vstmr_lte.h"

/*Other*/
#include "rex.h"
#include "modem_mem.h"
#include "m1x_diag.h"
#include "err.h"
#ifdef FEATURE_1XCP_QSH_SUPPORT
#include "mccqsh_i.h"
#endif /* FEATURE_1XCP_QSH_SUPPORT */


/*===========================================================================

                DEFINITIONS AND DECLARATIONS FOR MODULE

This section contains definitions for constants, macros, types, variables
and other items needed by this module.

===========================================================================*/

/*===========================================================================

                   CONSTANTS AND MACROS

===========================================================================*/
#define TOTAL_PERIODIC_EVENTS            10
#define INVALID                 -1
#define M1X_STMR_MAX_SLACK_TIME 1000
#define M1X_STMR_PRIMARY_VFR    0
#define M1X_STMR_SECONDARY_VFR  1
#define M1X_STMR_6_SYM_IN_CX1           384

/*===========================================================================

                   INTERNAL TYPES

===========================================================================*/
/* internal type for callback function */
typedef void (*event_callbk)(m1x_stmr_event_type);

/* global variables */
typedef struct
{
  vstmr_1x_view_s           *m1x_stmr_rtc_view;
  vstmr_lte_view_s          *m1x_stmr_lte_view;
  vstmr_1x_view_s           *m1x_stmr_view;
  vstmr_1x_view_s           *m1x_stmr_tx_view;
  vstmr_1xframe_event_tbl_s *m1x_stmr_frame_tbl;
  vstmr_1xframe_event_tbl_s *frame_tbl;
  vstmr_1xscomb_event_tbl_s *m1x_stmr_scomb_tbl;
  vstmr_1xscomb_event_tbl_s *scomb_tbl;
  event_callbk              **cbf_array;
  rex_crit_sect_type        m1x_stmr_crit_sect;
  vstmr_rtc_id              vstmr_sub_id;
  int                       event_id_tbl[TOTAL_PERIODIC_EVENTS];
  uint32                    m1x_stmr_client_usage[M1X_STMR_CLIENT_MAX];
  uint16                    total_frame_event_used;
  uint16                    total_scomb_event_used;
  uint16                    pcg_offset;
  uint16                    new_pcg_offset;
  boolean                   m1x_stmr_initialized;
  boolean                   vfr_active;
} m1x_stmr_vars_type;

static m1x_stmr_vars_type m1x_stmr_data = {0};

#define M1X_STMR_ENTER_CRIT_SECT()                        \
          rex_enter_crit_sect( &m1x_stmr_data.m1x_stmr_crit_sect )

#define M1X_STMR_LEAVE_CRIT_SECT()                        \
          rex_leave_crit_sect( &m1x_stmr_data.m1x_stmr_crit_sect )

/*===========================================================================

                    INTERNAL FUNCTION DEFINITIONS

===========================================================================*/
/*===========================================================================

FUNCTION      M1X_STMR_INITIALIZED_OR_ASSERT

DESCRIPTION
  This function will check if M1X_STMR was initialized. If not, it would do
  an ERR_FATAL.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
static void m1x_stmr_initialized_or_assert( void )
{
  /*-----------------------------------------------------------------------*/

  if( !m1x_stmr_data.m1x_stmr_initialized )
  {
    /* The module is not initialized */
    ONEX_ERR_FATAL( "M1x STMR module is not initialized", 0, 0, 0 );
  }

}/* m1x_stmr_initialized_or_assert */

/*===========================================================================

FUNCTION      M1X_STMR_GET_SCOMB

DESCRIPTION
  This function returns the current symbol combiner time.

DEPENDENCIES
  None.

RETURN VALUE
  The current scomb time.

SIDE EFFECTS
  None.
===========================================================================*/
vstmr_1xscomb_t m1x_stmr_get_scomb( void )
{
  vstmr_1xscomb_t curr_scomb = { 0 };

  /*-----------------------------------------------------------------------*/

  /* Sanity Check to do ERR_FATAL if the m1x_stmr module was not initialized */
  m1x_stmr_initialized_or_assert();

  curr_scomb = vstmr_1x_get_current_scomb( m1x_stmr_data.m1x_stmr_view );

  return curr_scomb;

} /* m1x_stmr_get_scomb */

/*===========================================================================

FUNCTION      M1X_STMR_GET_RTC_COUNT_CX8

DESCRIPTION
  This function returns current RTC value unadjusted in chip 8x.

DEPENDENCIES
  None.

RETURN VALUE
  The current RTC value.

SIDE EFFECTS
  None.
===========================================================================*/
uint32 m1x_stmr_get_rtc_count_cx8( void )
{
  m1x_stmr_rtc_type curr_rtc;

  /*-----------------------------------------------------------------------*/

  /* Sanity Check to do ERR_FATAL if the m1x_stmr module was not initialized */
  m1x_stmr_initialized_or_assert();

  curr_rtc.rtc_full = vstmr_1x_get_rtc( m1x_stmr_data.m1x_stmr_rtc_view );

  return curr_rtc.rtc_cx8;

} /* m1x_stmr_get_rtc_count_cx8 */

/*===========================================================================

FUNCTION      M1X_STMR_GET_FCW

DESCRIPTION
  This function returns current FCW value in uint32.

DEPENDENCIES
  None.

RETURN VALUE
  FCW value in Q30 format where 2 MSBs represent the decimal value and the
  remaining bits represent the CDMA chip rate cx8/XO freq.

  For example, with 19.2 MHz XO frequency, the value be calculated as below.

  CDMA Cx8: (1.2288 MHz * 8) / 19.2e6 = 0.512 * 2^30 = 0x20c49ba6 (rounded)

SIDE EFFECTS
  None.
===========================================================================*/
uint32 m1x_stmr_get_fcw( void )
{
  uint32 curr_fcw = 0;

  /*-----------------------------------------------------------------------*/

  /* Sanity Check to do ERR_FATAL if the m1x_stmr module was not initialized */
  m1x_stmr_initialized_or_assert();

  curr_fcw = vstmr_1x_get_fcw( m1x_stmr_data.m1x_stmr_rtc_view );

  /* FCW is in Q30 format. */
  return curr_fcw;

} /* m1x_stmr_get_fcw */

/*===========================================================================

FUNCTION      M1X_STMR_GET_CURRENT_FRAME

DESCRIPTION
  This function returns the current frame values.

DEPENDENCIES
  None.

RETURN VALUE
  The current frame values.

SIDE EFFECTS
  None.
===========================================================================*/
vstmr_1xframe_t m1x_stmr_get_current_frame( void )
{
  vstmr_1xframe_t curr_frame = { 0 };

  /*-----------------------------------------------------------------------*/

  /* Sanity Check to do ERR_FATAL if the m1x_stmr module was not initialized */
  m1x_stmr_initialized_or_assert();

  curr_frame = vstmr_1x_get_current_frame( m1x_stmr_data.m1x_stmr_view );

  return curr_frame;

} /* m1x_stmr_get_current_frame */

/*===========================================================================

FUNCTION      M1X_STMR_I_HEARTBEAT_CHECK

DESCRIPTION
  This function checks if any previous frame PCG events are pending to complete
  before the new frame boundary.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
static void m1x_stmr_i_heartbeat_check( void )
{
  uint32          slack_time_chips;
  vstmr_1xframe_t curr_frame = { 0 };

  /*--------------------------------------------------------*/

  /* get the current frame values */
  curr_frame = m1x_stmr_get_current_frame();

  slack_time_chips = curr_frame.chip_num;

  /* the current time should not exceed the slack time mark and we assume that the
     callback does not get delayed for exactly an entire frame while processing,
     pcg_offset tells where the frame is supposed to begin hence if the current
     pcg_num does not match the offset we have taken too much time calling the
     heartbeat. */
  if((slack_time_chips > M1X_STMR_MAX_SLACK_TIME) ||
     (curr_frame.pcg_num != m1x_stmr_data.pcg_offset))
  {

    /* time exceeds by too much */
    M1X_MSG( MUX, LEGACY_ERROR,
      "callback failure timing: curr PCG: %d , expected time: PCG: %d,"\
      "slack chips: %d",
      curr_frame.pcg_num,
      m1x_stmr_data.pcg_offset,
      slack_time_chips );
  }
} /* m1x_stmr_i_heartbeat_check */

/*===========================================================================

FUNCTION      M1X_STMR_I_PCG_OFFSET_UPDATE

DESCRIPTION
  This function updates the pcg_offset if it has been upated from the previous
  value.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
static void m1x_stmr_i_pcg_offset_update( void )
{
  int                 offset_move;
  uint32              new_event_trig;
  uint32              index        = 0;
  m1x_stmr_event_type curr_event   = M1X_STMR_EVENT_MAX;
  vstmr_1xframe_t     curr_frame   = { 0 };
  vstmr_1xframe_t     expiry_frame = { 0 };
  vstmr_1xframe_t     period_frame = { 0 };

  /*--------------------------------------------------------*/

  M1X_STMR_ENTER_CRIT_SECT();

  /* get the current frame values */
  curr_frame = m1x_stmr_get_current_frame();

  /* update the offset at this point */
  offset_move = (int)m1x_stmr_data.pcg_offset - (int)m1x_stmr_data.new_pcg_offset;

  /* set the pcg_offset to the new value */
  m1x_stmr_data.pcg_offset = m1x_stmr_data.new_pcg_offset;

  for( index = 0; index < TOTAL_PERIODIC_EVENTS; index++ )
  {
    /* Check for a valid event scheduled here */
    if( ( m1x_stmr_data.event_id_tbl[index] != INVALID ) &&
        ( m1x_stmr_data.event_id_tbl[index] <= M1X_STMR_PCG_15 ) )
    {
      curr_event = m1x_stmr_data.event_id_tbl[index];

      /* add the frame offset to adjust the PCG event */
      new_event_trig = ( m1x_stmr_data.pcg_offset + curr_event ) % VSTMR_1X_FRAME_PCG_NUM;

      /* deregister the event registered with older offset */
      vstmr_1xframe_event_cancel(m1x_stmr_data.m1x_stmr_frame_tbl, index);

      /* reset the expiry_frame variable */
      expiry_frame.all = 0;

      /* set the expiry_frame as next frame from now */
      expiry_frame.pcg_num = new_event_trig;

      /* if the new offset is greater in value */
      if( offset_move < 0 )
      {
        /* moving forward in time due to offset change, the next frame can be
           scheduled for the timer to keep working */
        expiry_frame.frame_num = curr_frame.frame_num;
      }
      else
      {
        /* moving back in time due to offset change */
        expiry_frame.frame_num = curr_frame.frame_num + 1;
      }

      if( ( m1x_stmr_data.pcg_offset + curr_event ) >= VSTMR_1X_FRAME_PCG_NUM )
      {
        /* if its a wraparound push the event to the next frame */
        expiry_frame.frame_num++;
      }

      /* reset the period_frame variable */
      period_frame.all = 0;

      /* set the period to 1 frame */
      period_frame.frame_num = 1;

      vstmr_1xframe_event_sched_periodic( m1x_stmr_data.m1x_stmr_frame_tbl,
                                          index,
                                          expiry_frame,
                                          period_frame );

      M1X_MSG( MUX, LEGACY_MED,
        "Event scheduled for PCG %d on evnt id: %d",
        curr_event,
        index );
    } /* if */
  } /* for */

  if( m1x_stmr_data.vfr_active )
  {
    /* release and allocate VFR to update pcg_offset */
    (void)m1x_stmr_release_vfr();
    (void)m1x_stmr_allocate_vfr( m1x_stmr_data.vstmr_sub_id );
  }

  M1X_STMR_LEAVE_CRIT_SECT();

} /* m1x_stmr_i_pcg_offset_update */

/*===========================================================================

FUNCTION      FRAME_EVENT_CALLBK

DESCRIPTION
  This function serves as the callback function for event table associated with
  frame format.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
static void m1x_stmr_i_frame_event_cb
(
  struct vstmr_1xframe_event_tbl_s_ *event_tbl, /* Event table for interrupt */
  void                              *arg,       /* arg passed during reg     */
  uint32                            pending_mask/* Event mask                */
)
{
  int             triggered_event;
  uint32          event_index, client_index, events_issued = 0;
  vstmr_1xframe_t curr_frame = { 0 };
  event_callbk    buff_callback_function;

  /*-----------------------------------------------------------------------*/

  M1X_STMR_ENTER_CRIT_SECT();

  /* Check which of the bit is set */
  for( event_index = 0; event_index < TOTAL_PERIODIC_EVENTS; event_index++ )
  {
    if( pending_mask & ( 0x0001 << event_index ) )
    {
      curr_frame = m1x_stmr_get_current_frame();

      triggered_event = m1x_stmr_data.event_id_tbl[event_index];

      if( events_issued > 0 )
      {
        M1X_MSG( MUX, LEGACY_ERROR,
          "multiple events issued, pending mask: %d",
          pending_mask );
      }

      M1X_MSG( MUX, LEGACY_MED,
        "Event index %d called, Event PCG %d, Offset %d, Adjusted PCG %d",
        event_index,
        triggered_event,
        m1x_stmr_data.pcg_offset,
        curr_frame.pcg_num );

      if( triggered_event == M1X_STMR_PCG_0 )
      {
        /* On PCG_0 call the heartbeat function */
        m1x_stmr_i_heartbeat_check();

        /* Check if the new_pcg_offset has been updated */
        if( m1x_stmr_data.new_pcg_offset != m1x_stmr_data.pcg_offset )
        {
          /* Call the update offset function to change events */
          M1X_MSG( MUX, LEGACY_MED,
            "Update Frame Offset, Old: %d, New: %d",
             m1x_stmr_data.pcg_offset,
             m1x_stmr_data.new_pcg_offset );

          m1x_stmr_i_pcg_offset_update();
        }
      }

      if(((triggered_event+m1x_stmr_data.pcg_offset)%VSTMR_1X_FRAME_PCG_NUM) !=
         curr_frame.pcg_num)
      {
        M1X_MSG( MUX, LEGACY_ERROR,
           "Late Event: index %d called, Event PCG %d, Offset %d, Adjusted PCG %d",
           event_index,
           triggered_event,
           m1x_stmr_data.pcg_offset,
           curr_frame.pcg_num );
      }

      /* Find all registered functions for the event */
      for( client_index=0; client_index < M1X_STMR_CLIENT_MAX; client_index++ )
      {
        /* Callback the registered function after checking for NULL */
        if( m1x_stmr_data.cbf_array[event_index][client_index] != NULL )
        {
          buff_callback_function =
                 (m1x_stmr_data.cbf_array[event_index][client_index]);
          /* Don't hold on to critical section while calling the callback */
          M1X_STMR_LEAVE_CRIT_SECT();
          (buff_callback_function)( triggered_event );
          M1X_STMR_ENTER_CRIT_SECT();
        }
      }
      events_issued++;
    }
  }

  M1X_STMR_LEAVE_CRIT_SECT();

} /* m1x_stmr_i_frame_event_callbk */

/*===========================================================================

FUNCTION      M1X_STMR_I_SCOMB_EVENT_CALLBK

DESCRIPTION
  This function serves as the callback function for event table associated with
  symbol combiner format.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
static void m1x_stmr_i_scomb_event_cb
(
  struct vstmr_1xscomb_event_tbl_s_ *event_tbl, /* Event table for interrupt*/
  void                              *arg,       /* arg passed during reg    */
  uint32                            pending_mask/* Event mask               */
)
{
  uint32 event_index, client_index, triggered_event;
  event_callbk    buff_callback_function;

  /*-----------------------------------------------------------------------*/

  m1x_stmr_initialized_or_assert();

  M1X_STMR_ENTER_CRIT_SECT();

  /* Check which of the bit is set */
  for( event_index =0; event_index < TOTAL_PERIODIC_EVENTS; event_index++ )
  {
    if( pending_mask & ( 0x0001 << event_index ) )
    {
      M1X_MSG( MUX, LEGACY_MED,
        " Triggered event %d in scomb event callback",
        event_index );

      triggered_event = m1x_stmr_data.event_id_tbl[event_index];

      /* find all registered functions for the event */
      for( client_index=0; client_index < M1X_STMR_CLIENT_MAX; client_index++ )
      {
        /* callback the registered function after checking for NULL */
        if( m1x_stmr_data.cbf_array[event_index][client_index] != NULL )
        {
          buff_callback_function =
                    (m1x_stmr_data.cbf_array[event_index][client_index]);
          /* Don't hold on to critical section while calling the callback */
          M1X_STMR_LEAVE_CRIT_SECT();
          (buff_callback_function)(triggered_event);
          M1X_STMR_ENTER_CRIT_SECT();
        }
      }
    }
  }

  M1X_STMR_LEAVE_CRIT_SECT();

} /* m1x_stmr_i_scomb_event_callbk */

/*===========================================================================

FUNCTION      M1X_STMR_I_DEREGISTER_HEARTBEAT

DESCRIPTION
  This function deregisters the heartbeat event functionality.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
static void m1x_stmr_i_deregister_heartbeat( void )
{
  int index, client_index;
  int pcg_registered_flag = 0;

  /*-----------------------------------------------------------------------*/

  /* this if condition indicates whether event being unregistered is last
     remaining event in case of total_frame_event_used = 2 or a PCG 0 event
     is the last event that had been registered with m1x_stmr  */
  if( m1x_stmr_data.total_frame_event_used <= 2 )
  {
    /* find where the PCG_0 is scheduled */
    for( index = 0; index < TOTAL_PERIODIC_EVENTS; index++ )
    {
      if( m1x_stmr_data.event_id_tbl[index] == M1X_STMR_PCG_0 )
      {
        /* found the pcg 0 index */
        /* check if any event callbacks are registered for PCG_0 */
        for( client_index = 0; client_index < M1X_STMR_CLIENT_MAX; client_index++ )
        {
          if( m1x_stmr_data.cbf_array[index][client_index] != NULL )
          {
            /* there is at least a client associated with PCG_0 */
            pcg_registered_flag = 1;
            break;
          }
        }
        break;
      }
    }

    if( index == TOTAL_PERIODIC_EVENTS )
    {
      ONEX_ERR_FATAL("Unable to find heartbeat event, problem with m1x_stmr",0,0,0);
    }

    if( pcg_registered_flag == 0 )
    {
      vstmr_1xframe_event_cancel(m1x_stmr_data.m1x_stmr_frame_tbl, index);
      m1x_stmr_data.total_frame_event_used--;
      m1x_stmr_data.event_id_tbl[index] = INVALID;
    }
  }

} /* m1x_stmr_i_deregister_heartbeat */

/*===========================================================================

FUNCTION      M1X_STMR_I_REGISTER_FRAME_EVENT

DESCRIPTION
  This function will register the frame event to the vstmr.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
static void m1x_stmr_i_register_frame_event
(
  m1x_stmr_event_type new_event,
  m1x_stmr_event_type available_id
)
{
  vstmr_1xframe_t curr_frame   = { 0 };
  vstmr_1xframe_t expiry_frame = { 0 };
  vstmr_1xframe_t period_frame = { 0 };
  uint32          event_pcg_num = 0;

  /*------------------------------------------------------------------*/

  /* get the current frame values for the view */
  curr_frame = m1x_stmr_get_current_frame();

  if( new_event == M1X_STMR_DECODE_PCH )
  {
    event_pcg_num = M1X_STMR_PCG_0;
  }
  else
  {
    event_pcg_num = new_event;
  }

  /* clear the expiry */
  expiry_frame.all = 0;

  /* Since due to frame offset, our notion & VSTMR notion of frame is
   * different. We need to correct the expiry frame based on the
   * VSTMR notion
   * Consider a, b, c as the consecutive frames from VSTMR notion. Similarly,
   * Consider a', b', c' as the consecutive frames from M1X_STMR notion.
   * The VSTMR & 1x notion of frame is related as a' = a + frame offset.
   *
   * Consider, we are at a' frame. This could mean we are either in a'a or a'b
   * region. Our design is to not schedule any events in this frame a'.
   * Hence, based on the below events, we could trigger an event for b'b or b'c
   * region.
   * 1) Current_PCG = a'a & expected event at b'b, then the vstmr frame should be
   * incremented by one from a to b.
   * 2) Current_PCG = a'a & expected event at b'c, then the vstmr frame should be
   * incremented by two from a to c.
   * 3) Current_PCG = a'b & expected event at b'b, then the vstmr frame should be
   * incremented by zero from b to b.
   * 4) Current_PCG = a'b & expected event at b'c, then the vstmr frame should be
   * incremented by one from b to c.
   * Based on this, derive the expiry frame number as per VSTMR notion.
   */
  if(curr_frame.pcg_num >= m1x_stmr_data.pcg_offset) /*a'a*/
  {
    if( event_pcg_num < ( VSTMR_1X_FRAME_PCG_NUM - m1x_stmr_data.pcg_offset ) ) /* b'b */
    {
      expiry_frame.frame_num = curr_frame.frame_num + 1;
    }
    else  /*b'c*/
    {
      expiry_frame.frame_num = curr_frame.frame_num + 2;
    }
  }
  else /*a'b*/
  {
    if( event_pcg_num < ( VSTMR_1X_FRAME_PCG_NUM - m1x_stmr_data.pcg_offset ) ) /* b'b */
    {
      expiry_frame.frame_num = curr_frame.frame_num;
    }
    else  /*b'c*/
    {
      expiry_frame.frame_num = curr_frame.frame_num+1;
    }
  }

  M1X_MSG( MUX, LEGACY_MED,
    "cur_pcg %d, FO %d, events %d, frame %d, expiry frame %d",
    curr_frame.pcg_num,
    m1x_stmr_data.pcg_offset,
    event_pcg_num,
    curr_frame.frame_num,
    expiry_frame.frame_num );

  if( new_event <= M1X_STMR_PCG_15 )
  {
  /* adjust the event value with the pcg_offset value */
    new_event = ( new_event + m1x_stmr_data.pcg_offset ) % VSTMR_1X_FRAME_PCG_NUM;
    expiry_frame.pcg_num = new_event;
  }
  else if( new_event == M1X_STMR_DECODE_PCH )
  {
    /* Decode PCH event is PCG 0 + 6 Symbols to make sure that L1 is reading
       only after FW has finished writing */
    expiry_frame.pcg_num  = M1X_STMR_PCG_0;
    expiry_frame.chip_num = M1X_STMR_6_SYM_IN_CX1;
  }
  else
  {
    ONEX_ERR_FATAL( "Trying to register a frame based event with a"
               "non frame event id, %d",
               new_event, 0, 0 );
  }

  /* give the periodicity of the timer which is 1 frame */
  period_frame.all = 0;
  period_frame.frame_num = 1;

  vstmr_1xframe_event_sched_periodic(m1x_stmr_data.m1x_stmr_frame_tbl,
                                      available_id,
                                      expiry_frame,
                                     period_frame);

  #ifdef FEATURE_MODEM_1X_DEBUG_MSGS
  M1X_MSG( MUX, LEGACY_HIGH,
    "Event %d scheduled for PCG %d on evnt id: %d",
    new_event,
    event_pcg_num,
    available_id );
  #endif /* FEATURE_MODEM_1X_DEBUG_MSGS */

  m1x_stmr_data.total_frame_event_used++;

} /* m1x_stmr_i_register_frame_event */

/*===========================================================================

FUNCTION      M1X_STMR_I_REGISTER_SCOMB_EVENT

DESCRIPTION
  This function will register the scomb event to the vstmr.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
static void m1x_stmr_i_register_scomb_event
(
  m1x_stmr_event_type new_event,
  m1x_stmr_event_type available_id
)
{
  vstmr_1xscomb_t curr_scomb   = { 0 };
  vstmr_1xscomb_t expiry_scomb = { 0 };
  vstmr_1xscomb_t period_scomb = { 0 };

  /*----------------------------------------------------------------------*/

  /* Check if the scomb event table is currently registered with VSTMR */
  if( m1x_stmr_data.m1x_stmr_scomb_tbl == NULL )
  {
    m1x_stmr_data.m1x_stmr_scomb_tbl = m1x_stmr_data.scomb_tbl;

    if( m1x_stmr_data.m1x_stmr_scomb_tbl )
    {
      vstmr_1xscomb_event_tbl_register(m1x_stmr_data.m1x_stmr_scomb_tbl,
                                       m1x_stmr_data.m1x_stmr_view,
                                       VSTMR_INT_NORMAL);
      vstmr_1xscomb_interrupt_register(m1x_stmr_data.m1x_stmr_scomb_tbl,
                                       m1x_stmr_i_scomb_event_cb, NULL);
    }
    else
    {
      ONEX_ERR_FATAL( "Hit a null pointer, incorrect driver op", 0, 0, 0 );
    }
  }

  /* get the current scomb values for m1x_stmr view */
  curr_scomb = m1x_stmr_get_scomb();

  /* reset the expiry variable */
  expiry_scomb.all = 0;

  /* We need to restrict the value of pnroll_num below 3 to
     maintain consistency for MCS VSTMR */
  expiry_scomb.pnroll_num = (curr_scomb.pnroll_num+1)%
                             VSTMR_1X_SYNC80_PNROLL_CNT;

  if( curr_scomb.pnroll_num + 1 > 2 )
  {
    /* As pnroll_num has a range of (0,2) the sync80_cnt has be incremented
       to ensure we account for it when (pnroll_num+1) = 3 */
    expiry_scomb.sync80_cnt = curr_scomb.sync80_cnt+1;
  }
  else
  {
    expiry_scomb.sync80_cnt = curr_scomb.sync80_cnt;
  }

  if( new_event == M1X_STMR_DECODE_SYNCH )
  {
    /* Decode Sync event is PN Roll + 6 Symbols to make sure that L1 is reading
       only after FW has finished writing */
    expiry_scomb.chips = M1X_STMR_6_SYM_IN_CX1;
  }

  /* reset the period variable */
  period_scomb.all = 0;

  /* set the period for the timer */
  period_scomb.pnroll_num = 1;

  vstmr_1xscomb_event_sched_periodic( m1x_stmr_data.m1x_stmr_scomb_tbl,
                                      available_id,
                                      expiry_scomb,
                                      period_scomb );

  #ifdef FEATURE_MODEM_1X_DEBUG_MSGS
  M1X_MSG( MUX, LEGACY_HIGH,
    "Symbol Combiner Event %d scheduled on evnt id: %d",
    new_event,
    available_id );
  #endif /* FEATURE_MODEM_1X_DEBUG_MSGS */

  /* increment the total number of events in use */
  m1x_stmr_data.total_scomb_event_used++;

} /* m1x_stmr_i_register_scomb_event */

/*===========================================================================

FUNCTION      M1X_STMR_I_DEREGISTER_FRAME_EVENT

DESCRIPTION
  This function will deregister the frame event to the vstmr.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
static void m1x_stmr_i_deregister_frame_event( int index )
{
  vstmr_1xframe_event_tbl_s *temp_event_tbl = NULL;

  /*----------------------------------------------------------------------*/

  vstmr_1xframe_event_cancel(m1x_stmr_data.m1x_stmr_frame_tbl, index);
  m1x_stmr_data.total_frame_event_used--;

  M1X_MSG( MUX, LEGACY_MED,
    "De-register event: %d",
    m1x_stmr_data.event_id_tbl[index] );

  m1x_stmr_data.event_id_tbl[index] = INVALID;

  if( m1x_stmr_data.total_frame_event_used == 0 )
  {
    temp_event_tbl = m1x_stmr_data.m1x_stmr_frame_tbl;
    M1X_STMR_LEAVE_CRIT_SECT();

    /*
       A race condition between srch task and callback is resulting in a crash.
       Scenario:
       1. the ISR callback is triggered and calls m1x_stmr callback
       2. mutex is grabbed by the callback and one of the client_callback is called
       3. before calling client_callback, mutex is released to avoid other race
          conditions
       4. deregister for the last event in the event table is issued from
          another task 'srch' which grabs the mutex
       5. being the last event in the event table tries to deregister the
          event table itself
       6. vstmr_1xframe_interrupt_deregister used to deregister event interrupt
          has to wait for all the callbacks to finish
       7. the client_callback to complete needs to grab the mutex again to return
          but is unable to due to task 'srch' having grabbed it earlier
       8. an infinite wait between ISR and task ensues and a watchdog expires
          causing the crash

       Assumption: There are multiple hardware threads which will resolve the
                   VSTMR infinite wait inside vstmr_1xframe_interrupt_deregister
    */
    vstmr_1xframe_interrupt_deregister(temp_event_tbl);
    vstmr_1xframe_event_tbl_deregister(temp_event_tbl);

    M1X_STMR_ENTER_CRIT_SECT();
    m1x_stmr_data.m1x_stmr_frame_tbl = NULL;
  }

} /* m1x_stmr_i_deregister_frame_event */

/*===========================================================================

FUNCTION      M1X_STMR_I_DEREGISTER_SCOMB_EVENT

DESCRIPTION
  This function deregisters a scomb event to vstmr.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
static void m1x_stmr_i_deregister_scomb_event( int index )
{
  vstmr_1xscomb_event_tbl_s *temp_event_tbl = NULL;

  /*----------------------------------------------------------------------*/

  vstmr_1xscomb_event_cancel(m1x_stmr_data.m1x_stmr_scomb_tbl, index);
  m1x_stmr_data.total_scomb_event_used--;
  m1x_stmr_data.event_id_tbl[index] = INVALID;

  if(m1x_stmr_data.total_scomb_event_used == 0)
  {
    temp_event_tbl = m1x_stmr_data.m1x_stmr_scomb_tbl;
    M1X_STMR_LEAVE_CRIT_SECT();
    /* Explaination provided in m1x_stmr_i_deregister_frame_event() */

    vstmr_1xscomb_interrupt_deregister(temp_event_tbl);
    vstmr_1xscomb_event_tbl_deregister(temp_event_tbl);

    M1X_STMR_ENTER_CRIT_SECT();
    m1x_stmr_data.m1x_stmr_scomb_tbl = NULL;
  }

} /* m1x_stmr_i_deregister_scomb_event */

/*===========================================================================

                    EXTERNAL FUNCTION DEFINITIONS

===========================================================================*/

/*===========================================================================

FUNCTION      M1X_STMR_INIT

DESCRIPTION
  This function initializes the view assigned by VSTMR without an offset.
  Function proceeds to allocate memory for all the data variables required by
  the module in this function. Also resets data arrays.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void m1x_stmr_init( void )
{
  int index = 0, client_index = 0;

  /*-----------------------------------------------------------------------*/

  if( m1x_stmr_data.m1x_stmr_initialized )
  {
    /*the module is already initialized*/
    M1X_MSG( MUX, LEGACY_MED,
      "M1x STMR module is already initialized" );
    return;
  }

  /* Remember that the module has been initialized */
  m1x_stmr_data.m1x_stmr_initialized = TRUE;

  #if defined( FEATURE_MODEM_1X_THOR_REVISIT ) || defined( FEATURE_MODEM_1X_TABASCO_REVISIT )
  /* REVISIT: This should be removed once REX fixes the delete API */
  /* Reset the crit section memory for init -> del -> init sequence */
  memset( &m1x_stmr_data.m1x_stmr_crit_sect,
          0,
          sizeof(m1x_stmr_data.m1x_stmr_crit_sect) );
  #endif /* FEATURE_MODEM_1X_THOR_REVISIT || FEATURE_MODEM_1X_TABASCO_REVISIT */

  /* Initialize critical section */
  rex_init_crit_sect( &m1x_stmr_data.m1x_stmr_crit_sect );

  M1X_STMR_ENTER_CRIT_SECT();

  if( m1x_stmr_data.m1x_stmr_view == NULL )
  {
    m1x_stmr_data.m1x_stmr_view = vstmr_1x_get_view_handle( VSTMR_RTC_1X_SUB0,
                                                            VSTMR_1X_RX_VIEW );
  }
  else
  {
    M1X_MSG( MUX, LEGACY_HIGH,
      "the m1x_stmr module already inited" );
  }

  if( m1x_stmr_data.m1x_stmr_rtc_view == NULL )
  {
    m1x_stmr_data.m1x_stmr_rtc_view = vstmr_1x_get_view_handle( VSTMR_RTC_1X_SUB0,
                                                                VSTMR_1X_RTC_VIEW );
  }
  else
  {
    M1X_MSG( MUX, LEGACY_HIGH,
      "the m1x_stmr_rtc_view already inited" );
  }

  if( m1x_stmr_data.m1x_stmr_lte_view == NULL )
  {
    m1x_stmr_data.m1x_stmr_lte_view = vstmr_lte_get_view_handle( VSTMR_RTC_LTE_SUB0,
                                                                 VSTMR_LTE_OSTMR_VIEW );
  }
  else
  {
    M1X_MSG( MUX, LEGACY_HIGH,
      "the m1x_stmr_lte_view already inited" );
  }

  if( m1x_stmr_data.m1x_stmr_tx_view == NULL )
  {
    m1x_stmr_data.m1x_stmr_tx_view = vstmr_1x_get_view_handle( VSTMR_RTC_1X_SUB0,
                                                               VSTMR_1X_TX_VIEW );
  }
  else
  {
    M1X_MSG( MUX, LEGACY_HIGH,
      "the m1x_stmr_tx_view module already inited" );
  }

  /*allocate and initialize the callback function array*/
  if(m1x_stmr_data.cbf_array == NULL)
  {
    m1x_stmr_data.cbf_array = (event_callbk **)
      modem_mem_alloc( sizeof(event_callbk *) * TOTAL_PERIODIC_EVENTS,
                       MODEM_MEM_CLIENT_1X_CRIT);
    if(m1x_stmr_data.cbf_array != NULL)
    {
      for(index = 0; index < TOTAL_PERIODIC_EVENTS; index++)
      {
        m1x_stmr_data.cbf_array[index] = (event_callbk*)
          modem_mem_alloc( sizeof(event_callbk) * M1X_STMR_CLIENT_MAX,
                           MODEM_MEM_CLIENT_1X_CRIT );

        if(m1x_stmr_data.cbf_array[index] == NULL)
        {
          ONEX_ERR_FATAL(" Problem with allocating cbf_array index %d",index,0,0);

          m1x_stmr_data.cbf_array = NULL;
          break;
        }

        /*initialize all callback function sub array*/
        for(client_index = 0; client_index < M1X_STMR_CLIENT_MAX; client_index++)
        {
          m1x_stmr_data.cbf_array[index][client_index] = 0;
        }
      } /* for */
    } /* m1x_stmr_data.cbf_array != NULL */
  } /* m1x_stmr_data.cbf_array == NULL */

  /*allocate frame event table*/
  if(m1x_stmr_data.frame_tbl == NULL)
  {
    m1x_stmr_data.frame_tbl = (vstmr_1xframe_event_tbl_s *)modem_mem_alloc(
                                    sizeof(vstmr_1xframe_event_tbl_s),
                                    MODEM_MEM_CLIENT_1X_CRIT);
  }

  /*allocate scomb event table*/
  if(m1x_stmr_data.scomb_tbl == NULL)
  {
    m1x_stmr_data.scomb_tbl = (vstmr_1xscomb_event_tbl_s *)modem_mem_alloc(
                                    sizeof(vstmr_1xscomb_event_tbl_s),
                                    MODEM_MEM_CLIENT_1X_CRIT);
  }

  if( m1x_stmr_data.scomb_tbl == NULL ||
      m1x_stmr_data.frame_tbl == NULL ||
     m1x_stmr_data.cbf_array == NULL)
  {
    ONEX_ERR_FATAL("Unable to allocate memory for m1x_stmr "\
              "frame_tbl 0x%x, scomb_tbl 0x%x, cbf_array 0x%x",
              m1x_stmr_data.frame_tbl,
              m1x_stmr_data.scomb_tbl,
              m1x_stmr_data.cbf_array);
  }

  /* reset frame_tbl */
  memset(m1x_stmr_data.frame_tbl, 0, sizeof(vstmr_1xframe_event_tbl_s));

  /* reset scomb_tbl */
  memset(m1x_stmr_data.scomb_tbl, 0, sizeof(vstmr_1xscomb_event_tbl_s));

  /* reset event_id_tbl */
  memset(m1x_stmr_data.event_id_tbl,INVALID, TOTAL_PERIODIC_EVENTS*sizeof(int));

  /* reset client usage */
  memset( m1x_stmr_data.m1x_stmr_client_usage,
          0,
          sizeof(m1x_stmr_data.m1x_stmr_client_usage) );

  /* reset offset related vars */
  m1x_stmr_data.pcg_offset = 0;
  m1x_stmr_data.new_pcg_offset = 0;

  /* reset vfr_active */
  m1x_stmr_data.vfr_active = 0;

  M1X_STMR_LEAVE_CRIT_SECT();

} /*m1x_stmr_init*/

/*===========================================================================

FUNCTION      M1X_STMR_DEINIT

DESCRIPTION
  This function will release all the event tables associated with the view and
  release all memory associated with event tables.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void m1x_stmr_deinit( void )
{
  int                 index, client_index;
  m1x_stmr_event_type active_event;

  /*-----------------------------------------------------------------------*/

  /* Sanity Check to do ERR_FATAL if the m1x_stmr module was not initialized */
  m1x_stmr_initialized_or_assert();

  M1X_STMR_ENTER_CRIT_SECT();

  /* check for all active events and cancel them */
  for(index = 0; index < TOTAL_PERIODIC_EVENTS; index++)
  {
    /* active event if not INVALID */
    if(m1x_stmr_data.event_id_tbl[index] != INVALID)
    {
      active_event = m1x_stmr_data.event_id_tbl[index];

      M1X_MSG( MUX, LEGACY_MED,
        "Event: %d was left active",
        active_event );

      /* Check the event type */
      if( active_event <= M1X_STMR_DECODE_PCH )
      {
        m1x_stmr_i_deregister_frame_event(index);
      }
      else if( active_event <= M1X_STMR_DECODE_SYNCH )
      {
        m1x_stmr_i_deregister_scomb_event(index);
      }
      else
      {
        ONEX_ERR_FATAL( "Trying to de-register an invalid event %d",
                   active_event, 0, 0 );
      }

      for(client_index = 0; client_index < M1X_STMR_CLIENT_MAX; client_index++)
      {
        m1x_stmr_data.cbf_array[index][client_index] = 0;
      }
    }
  }

  /* any active events means catastrophic failure */
  if(m1x_stmr_data.total_frame_event_used > 0 ||
     m1x_stmr_data.total_scomb_event_used > 0 ||
     m1x_stmr_data.m1x_stmr_frame_tbl != NULL ||
     m1x_stmr_data.m1x_stmr_scomb_tbl != NULL)
  {
    ONEX_ERR_FATAL("Invalid m1x_stmr behavior,"\
              "total_events %d, frame_tbl 0x%x, scomb_tbl 0x%x",
              m1x_stmr_data.total_frame_event_used + m1x_stmr_data.total_scomb_event_used,
              m1x_stmr_data.m1x_stmr_frame_tbl,
              m1x_stmr_data.m1x_stmr_scomb_tbl);
  }

  if(m1x_stmr_data.frame_tbl != NULL)
  {
    modem_mem_free(m1x_stmr_data.frame_tbl, MODEM_MEM_CLIENT_1X_CRIT);
    m1x_stmr_data.frame_tbl = NULL;
  }

  if(m1x_stmr_data.scomb_tbl != NULL)
  {
    modem_mem_free(m1x_stmr_data.scomb_tbl, MODEM_MEM_CLIENT_1X_CRIT);
    m1x_stmr_data.scomb_tbl = NULL;
  }

  /* free all the cbf_array memory */
  if(m1x_stmr_data.cbf_array)
  {
    for(index = 0; index < TOTAL_PERIODIC_EVENTS; index++)
    {
      if(m1x_stmr_data.cbf_array[index])
      {
        modem_mem_free(m1x_stmr_data.cbf_array[index],MODEM_MEM_CLIENT_1X_CRIT);
        m1x_stmr_data.cbf_array[index] = NULL;
      }
    }

    /* release the higher level array to pointers*/
    modem_mem_free(m1x_stmr_data.cbf_array,MODEM_MEM_CLIENT_1X_CRIT);
    m1x_stmr_data.cbf_array = NULL;
  }

  /* reset client usage */
  memset(m1x_stmr_data.m1x_stmr_client_usage,
         0,
         sizeof(m1x_stmr_data.m1x_stmr_client_usage));

  /* reset pcg_offset */
  m1x_stmr_data.pcg_offset = 0;
  m1x_stmr_data.new_pcg_offset = 0;

  /* reset vfr_active */
  m1x_stmr_data.vfr_active = 0;
  m1x_stmr_data.m1x_stmr_initialized = FALSE;

  M1X_STMR_LEAVE_CRIT_SECT();

  /* Delete critical section only after releasing it */
  rex_del_crit_sect( &m1x_stmr_data.m1x_stmr_crit_sect );

} /*m1x_stmr_deinit*/

/*=============================================================================

FUNCTION      M1X_STMR_REGISTER

DESCRIPTION
  This function will start an event in the VSTMR.It will register the event
  and will allocate the event table in case it hasnt already been done.

DEPENDENCIES
  None.

RETURN VALUE
  M1X_STMR_SUCCESS        - Started and registered event and callback function.
  M1X_STMR_ERR_NO_SLOTS   - The registration was not successful due to all
                            periodic slots being full
  M1X_STMR_ERR_BAD_PARAM  - The registration was not successful because of
                            incorrect parameters being passed
  M1X_STMR_ERR_REG_DUP    - The event is already registered for the client,
                            should not register such an event.
  M1X_STMR_ERR_BAD_DRV_OP - Incorrect driver operation, debug driver

SIDE EFFECTS
  None.
=============================================================================*/
m1x_stmr_err_type m1x_stmr_register
(
  m1x_stmr_client_type client,                              /*Calling client  */
  m1x_stmr_event_type  new_event,                           /* Event to be set */
  void                 (*event_callbk)(m1x_stmr_event_type) /*Callbk function */
)
{
  int                 index;
  m1x_stmr_err_type   result = M1X_STMR_SUCCESS;
  m1x_stmr_event_type available_event_id = 0, heartbeat_id = 0;

  /*-----------------------------------------------------------------------*/

  /* Sanity Check to do ERR_FATAL if the m1x_stmr module was not initialized */
  m1x_stmr_initialized_or_assert();

  M1X_STMR_ENTER_CRIT_SECT();

  do
  {
    /* Incorrect count of events in the driver, internal driver error */
    if(m1x_stmr_data.total_frame_event_used +
         m1x_stmr_data.total_scomb_event_used > TOTAL_PERIODIC_EVENTS)
    {
      result = M1X_STMR_ERR_BAD_DRV_OP;
      ONEX_ERR_FATAL("Incorrect event count in drivers"\
                "frame_events %d, scomb_events %d",
                m1x_stmr_data.total_frame_event_used,
                m1x_stmr_data.total_scomb_event_used,
                0 );
      break;
    }

    /* Too many events have been scheduled */
    if(m1x_stmr_data.total_frame_event_used +
       m1x_stmr_data.total_scomb_event_used == TOTAL_PERIODIC_EVENTS)
    {
      M1X_MSG( MUX, LEGACY_HIGH,
        "Max events registered, frame_events %d, scomb_events %d",
        m1x_stmr_data.total_frame_event_used,
        m1x_stmr_data.total_scomb_event_used );
      result = M1X_STMR_ERR_NO_SLOTS;
      break;
    }

    /* Parameters passed are out of range */
    if( new_event < M1X_STMR_PCG_0      ||
        new_event >= M1X_STMR_EVENT_MAX ||
        client >= M1X_STMR_CLIENT_MAX   ||
        client < M1X_STMR_CLIENT_SRCH   ||
       event_callbk == NULL)
    {
      result = M1X_STMR_ERR_BAD_PARAM;
      ONEX_ERR_FATAL( "Paramters are out of range,"\
                 "event %d, client %d, event_callbk 0x%x",
                 new_event,
                 client,
                 event_callbk );
      break;
    }

    /* update the usage logs */
    m1x_stmr_data.m1x_stmr_client_usage[client] |= ( 0x0001 << new_event );

    /* find if the event has already been scheduled */
    for(index = 0; index < TOTAL_PERIODIC_EVENTS; index++)
    {
      /* if there is already an event_id associated */
      if( m1x_stmr_data.event_id_tbl[index] == new_event )
      {
        /* the event is already registered in the event table */
        if(m1x_stmr_data.cbf_array[index][client] != NULL)
        {
          M1X_MSG( MUX, LEGACY_HIGH,
            "Updating event_callback for event %d, client %d",
            new_event,
            client);
          m1x_stmr_data.cbf_array[index][client] = event_callbk;
          M1X_STMR_LEAVE_CRIT_SECT();

          return M1X_STMR_ERR_REG_DUP;
        }

        m1x_stmr_data.cbf_array[index][client] = event_callbk;
        M1X_MSG( MUX, LEGACY_HIGH,
          "Event already scheduled, Success, event %d, client %d",
          new_event,
          client );
        M1X_STMR_LEAVE_CRIT_SECT();

        return M1X_STMR_SUCCESS;
      }

      /* find an empty slot while going through the list of events */
      if(m1x_stmr_data.event_id_tbl[index] == INVALID)
      {
        available_event_id = index;
      }
    } /* end of for */

    /* we dont need to check validity of available_event_id because the
       total_event_used confirms the existence of an empty slot in the event
       table to schedule an event. */
    m1x_stmr_data.event_id_tbl[available_event_id]      = new_event;
    m1x_stmr_data.cbf_array[available_event_id][client] = event_callbk;

    M1X_MSG( MUX, LEGACY_MED,
      "Register for event: %d, for client: %d",
      new_event,
      client );

    /* Check and register for the correct event type. The enum ordering is
       important here. The ordering of PCG related events should be maintained.
       Any non-PCG related events should be added below PN ROLL event */
    if( new_event >= M1X_STMR_PNROLL )
    {
      m1x_stmr_i_register_scomb_event( new_event, available_event_id );
    }
    else
    {
      if(m1x_stmr_data.m1x_stmr_frame_tbl == NULL)
      {
        m1x_stmr_data.m1x_stmr_frame_tbl = m1x_stmr_data.frame_tbl;
        if(m1x_stmr_data.m1x_stmr_frame_tbl)
        {
          vstmr_1xframe_event_tbl_register(m1x_stmr_data.m1x_stmr_frame_tbl,
                                           m1x_stmr_data.m1x_stmr_view,
                                           VSTMR_INT_NORMAL);
          vstmr_1xframe_interrupt_register(m1x_stmr_data.m1x_stmr_frame_tbl,
                                           m1x_stmr_i_frame_event_cb, NULL);
        }
        else
        {
          ONEX_ERR_FATAL("Frame table not allocated, incorrect driver op",0,0,0);
          result = M1X_STMR_ERR_BAD_DRV_OP;
          break;
        }

        /* As this is the first frame callback event being registered we need to
           start the heartbeat at this point and as this is the first frame
           callback we can expect event table to have space for the event */
        if( new_event != M1X_STMR_PCG_0 )
        {
          /* find a suitable event_id and register it */
          for(index = 0; index < TOTAL_PERIODIC_EVENTS; index++)
          {
            /* find an empty slot while going through the list of events */
            if(m1x_stmr_data.event_id_tbl[index] == INVALID)
            {
              heartbeat_id = index;
              break;
            }
          } /* end of for */

          /* the event_id_tbl needs to be updated for the usage of PCG_0 */
          m1x_stmr_data.event_id_tbl[heartbeat_id] = M1X_STMR_PCG_0;

          m1x_stmr_i_register_frame_event(M1X_STMR_PCG_0, heartbeat_id);
        }
      }

      /* schedule the actual event */
      m1x_stmr_i_register_frame_event( new_event, available_event_id );
    }
  }while(0);

  M1X_STMR_LEAVE_CRIT_SECT();

  return result;

} /*m1x_stmr_register*/

/*===========================================================================

FUNCTION      M1X_STMR_DEREGISTER

DESCRIPTION
  This function will stop the event that had been scheduled using
  m1x_stmr_register and if successful return 0 otherwise an error code.

DEPENDENCIES
  None.

RETURN VALUE
  M1X_STMR_SUCCESS         - Successfully stopped the event.
  M1X_STMR_ERR_UNREG_EVENT - The event had not been registered by the client.
  M1X_STMR_ERR_BAD_PARAM   - Incorrect parameters passed to the function.
  M1X_STMR_ERR_BAD_DRV_OP  - Incorrect driver operation, debug driver

SIDE EFFECTS
  None.
===========================================================================*/
m1x_stmr_err_type m1x_stmr_deregister
(
  m1x_stmr_client_type client,  /*calling client*/
  m1x_stmr_event_type  curr_event /* event ID to be cancelled */
)
{
  int result = M1X_STMR_SUCCESS, index, client_index;

  /*-----------------------------------------------------------------------*/

  /* Sanity Check to do ERR_FATAL if the m1x_stmr module was not initialized */
  m1x_stmr_initialized_or_assert();

  /*check if the current event has been scheduled or not and then proceed to
    actually unschedule the event*/

  M1X_STMR_ENTER_CRIT_SECT();

  do
  {

    /* something wrong with driver counts */
    if(m1x_stmr_data.total_frame_event_used +
         m1x_stmr_data.total_scomb_event_used >= TOTAL_PERIODIC_EVENTS)
    {
      ONEX_ERR_FATAL("Incorrect event count in drivers"\
                "frame_events %d, scomb_events %d",
                m1x_stmr_data.total_frame_event_used,
                m1x_stmr_data.total_scomb_event_used,
                0 );
      result = M1X_STMR_ERR_BAD_DRV_OP;
      break;
    }

    /* parameters passed are out of range */
    if( curr_event < M1X_STMR_PCG_0       ||
        curr_event >= M1X_STMR_EVENT_MAX  ||
        client >= M1X_STMR_CLIENT_MAX     ||
        client < M1X_STMR_CLIENT_SRCH )
    {
      ONEX_ERR_FATAL("Paramters are out of range, event %d, client %d",
                 curr_event, client, 0 );
      result = M1X_STMR_ERR_BAD_PARAM;
      break;
    }

    for(index = 0; index < TOTAL_PERIODIC_EVENTS; index++)
    {
      if( m1x_stmr_data.event_id_tbl[index] == curr_event )
      {

        /* an incorrect event has been sent */
        if(m1x_stmr_data.cbf_array[index][client] == NULL)
        {
          result = M1X_STMR_ERR_UNREG_EVENT;
          M1X_MSG( MUX, LEGACY_MED,
            "Event %d for Client %d was not reg, ret %d",
            curr_event,
            client,
            result);
          break;
        }

        /*reset array value*/
        m1x_stmr_data.cbf_array[index][client] = NULL;

        /*update the usage logs*/
        m1x_stmr_data.m1x_stmr_client_usage[client] &= ~( 0x0001 << curr_event );

        M1X_MSG( MUX, LEGACY_MED,
          "De-register event: %d for client: %d",
          curr_event,
          client );

        /* need to check the other clients for registration hence start
           client_index = 1*/
        for(client_index = 1; client_index < M1X_STMR_CLIENT_MAX; client_index++)
        {
          if(m1x_stmr_data.cbf_array[index][((client+client_index)%M1X_STMR_CLIENT_MAX)] !=
               NULL)
          {
            M1X_MSG( MUX, LEGACY_MED,
              "Other clients are also using the event: %d, client: %d",
              curr_event,
              client );
            M1X_STMR_LEAVE_CRIT_SECT();

            return M1X_STMR_SUCCESS;
          }
        }

        /* Check and de-register for the correct event type. The enum ordering is
           important here. The ordering of PCG related events should be maintained.
           Any non-PCG related events should be added below PN ROLL event */
        if( curr_event >= M1X_STMR_PNROLL )
        {
          m1x_stmr_i_deregister_scomb_event(index);
        }
        else
        {
          /* In case its PCG_0 dont de-register until we are about to
             de-register the frame event table */
          if( curr_event == M1X_STMR_PCG_0 )
          {
            if(m1x_stmr_data.total_frame_event_used >= 2)
            {
              result = M1X_STMR_SUCCESS;
              break;
            }
          }
          else
          {
            m1x_stmr_i_deregister_heartbeat();
          }

          m1x_stmr_i_deregister_frame_event(index);
        }

        result = M1X_STMR_SUCCESS;
        break;
      }
    }/* end of for */
  /* WARNING:do not put any statement due to use of 'break' inside 'for' loop */
  }while(0);

  M1X_STMR_LEAVE_CRIT_SECT();

  return result;

} /* m1x_stmr_deregister */

/*===========================================================================

FUNCTION      M1X_STMR_DEREGISTER_CLIENT

DESCRIPTION
  This function deregisters all the events from the client passed as parameter.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void m1x_stmr_deregister_client
(
  m1x_stmr_client_type client  /* Caller identity */
)
{
  int index = 0, client_index;

  /*-----------------------------------------------------------------------*/

  /* Sanity Check to do ERR_FATAL if the m1x_stmr module was not initialized */
  m1x_stmr_initialized_or_assert();

  /* Parameters passed are out of range */
  if(client >= M1X_STMR_CLIENT_MAX || client < M1X_STMR_CLIENT_SRCH)
  {
    ONEX_ERR_FATAL( "Invalid client %d to deregister", client, 0, 0 );
    return;
  }

  M1X_STMR_ENTER_CRIT_SECT();

  /* update the usage log */
  m1x_stmr_data.m1x_stmr_client_usage[client] = 0;

  for(index = 0; index < TOTAL_PERIODIC_EVENTS; index++)
  {
    if(m1x_stmr_data.event_id_tbl[index] != INVALID)
    {

      /* an incorrect event has been sent */
      if(m1x_stmr_data.cbf_array[index][client] == NULL)
      {
        continue;
      }

      /* reset array element */
      m1x_stmr_data.cbf_array[index][client] = NULL;

      M1X_MSG( MUX, LEGACY_MED,
        "Deregistering event: %d for client: %d", index, client );

      for(client_index = 1; client_index < M1X_STMR_CLIENT_MAX; client_index++)
      {
        if(m1x_stmr_data.cbf_array[index]
                                  [((client+client_index) % M1X_STMR_CLIENT_MAX)] !=
             NULL)
        {
          M1X_MSG( MUX, LEGACY_MED,
            "Other clients are also using the event: %d, client: %d",
            index,
            (client+client_index) % M1X_STMR_CLIENT_MAX );
          break;
        }
      }

      if(client_index < M1X_STMR_CLIENT_MAX)
      {
        continue;
      }

      /* Check and de-register for the correct event type. The enum ordering is
         important here. The ordering of PCG related events should be maintained.
         Any non-PCG related events should be added below PN ROLL event */
      if( m1x_stmr_data.event_id_tbl[index] >= M1X_STMR_PNROLL )
      {
        m1x_stmr_i_deregister_scomb_event(index);
      }
      else
      {
        /* In case its PCG_0 don't de-register until we are about to
           de-register the frame event table */
        if(m1x_stmr_data.event_id_tbl[index] == M1X_STMR_PCG_0)
        {
          if(m1x_stmr_data.total_frame_event_used >= 2)
          {
            continue;
          }
        }
        else
        {
          m1x_stmr_i_deregister_heartbeat();
        }

        m1x_stmr_i_deregister_frame_event(index);
      }
    }
  }

  M1X_STMR_LEAVE_CRIT_SECT();

} /* m1x_stmr_deregister_client */

/*===========================================================================

FUNCTION      M1X_STMR_SET_FRAME_OFFSET

DESCRIPTION
  This function adjusts the PCG offset which will reschdule only the PCG events
  accordingly.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void m1x_stmr_set_frame_offset( uint16 offset )
{
  /*-----------------------------------------------------------------------*/

  /* Sanity Check to do ERR_FATAL if the m1x_stmr module was not initialized */
  m1x_stmr_initialized_or_assert();

  M1X_STMR_ENTER_CRIT_SECT();
  do
  {
    if(offset > VSTMR_1X_FRAME_PCG_NUM)
    {
      M1X_MSG( MUX, LEGACY_HIGH,
        "Incorrect offset sent" );
      break;
    }

    M1X_MSG( MUX, LEGACY_MED,
       "Update FO to %d at the next Frame boundary, num of frame events: %d",
       offset,
      m1x_stmr_data.total_frame_event_used );

    if(m1x_stmr_data.total_frame_event_used == 0)
    {
      /* this is the case when none of the frame events have been registered */
      m1x_stmr_data.pcg_offset = offset;

      if(m1x_stmr_data.vfr_active)
      {
        /* release and allocate VFR to update pcg_offset */
        (void)m1x_stmr_release_vfr();
        (void)m1x_stmr_allocate_vfr( m1x_stmr_data.vstmr_sub_id );
      }
    }

    m1x_stmr_data.new_pcg_offset = offset;
  }while(0);

  M1X_STMR_LEAVE_CRIT_SECT();

} /* m1x_stmr_set_frame_offset */

/*===========================================================================

FUNCTION      M1X_STMR_ALLOCATE_VFR

DESCRIPTION
  This function will request for an allocation of the primary VFR to 1x.

DEPENDENCIES
  None.

RETURN VALUE
  M1X_STMR_SUCCESS        - The VFR was successfully allocated.
  M1X_STMR_ERR_VFR_ALLOC  - The VFR has already been allocated.

SIDE EFFECTS
  None.
===========================================================================*/
m1x_stmr_err_type m1x_stmr_allocate_vfr
(
  vstmr_rtc_id vstmr_id
)
{
  vstmr_1xframe_t vstmr_pcg_offset = { 0 };
  uint32          vfr_int          = M1X_STMR_PRIMARY_VFR;

  /*---------------------------------------------------------------------*/

  M1X_STMR_ENTER_CRIT_SECT();

  if( m1x_stmr_data.vfr_active )
  {
    /* leave critical section */
    M1X_STMR_LEAVE_CRIT_SECT();

    /* the vfr has already been activated issue an error */
    return M1X_STMR_ERR_VFR_ALLOC;
  }

  M1X_MSG( MUX, LEGACY_HIGH,
    "Allocating VFR with FO: %d",
    m1x_stmr_data.pcg_offset );

  /* 1x is setting 4 bits of pcg_num which translates to 0-15 decimal
     count, these set of bits are being used in VSTMR to populate frame_cx1.
     The value of frame_cx1 does not directly translate to an actual flat
     count in cx1 for the PCG offset. This is because lower bits(3-11) is only
     supposed to reach maximum of 1535 instead of 2048 */
  /* Set the pcg_offset frame value */
  vstmr_pcg_offset.pcg_num = m1x_stmr_data.pcg_offset;

  m1x_stmr_data.vstmr_sub_id = vstmr_id;
  if( m1x_stmr_data.vstmr_sub_id == VSTMR_RTC_1X_SUB0 )
  {
    vfr_int = M1X_STMR_PRIMARY_VFR;

     /* send vfr request */
    vstmr_1x_vfr_allocate( vstmr_id, VSTMR_1X_RX_VIEW,
                           vfr_int, vstmr_pcg_offset.frame_cx1 );

    #ifdef FEATURE_MODEM_1X_VFR_SEL_REG
    /* this change will be required as part of Tabasco as well because JO
       shares a similar architecture with regard to VFR related registers */
    HWIO_OUT( VFR_SEL, 1 );
    #endif /* FEATURE_MODEM_1X_VFR_SEL_REG */
  }
  else
  {
    vfr_int = M1X_STMR_SECONDARY_VFR;

    /* send vfr request */
    vstmr_1x_vfr_allocate( vstmr_id, VSTMR_1X_RX_VIEW,
                           vfr_int, vstmr_pcg_offset.frame_cx1 );

    #ifdef FEATURE_MODEM_1X_VFR_SEL_REG
    /* this change will be required as part of Tabasco as well because JO
       shares a similar architecture with regard to VFR related registers */
    HWIO_OUT( VFR_SEL_1, 1 );
    #endif /* FEATURE_MODEM_1X_VFR_SEL_REG */
  }

  /* set the variable to indicate the vfr is allocated */
  m1x_stmr_data.vfr_active = 1;

  M1X_STMR_LEAVE_CRIT_SECT();

  return M1X_STMR_SUCCESS;

} /* m1x_stmr_allocate_vfr */

/*===========================================================================

FUNCTION      M1X_STMR_RELEASE_VFR

DESCRIPTION
  This function will release the primary VFR that has been allocated to 1x.

DEPENDENCIES
  None.

RETURN VALUE
  M1X_STMR_SUCCESS        - The VFR was successfully released.
  M1X_STMR_ERR_VFR_ALLOC  - The VFR has not been requested hence cannot release.

SIDE EFFECTS
  None.
===========================================================================*/
m1x_stmr_err_type m1x_stmr_release_vfr( void )
{
  uint32 vfr_int;

  /*---------------------------------------------------------------------*/

  M1X_STMR_ENTER_CRIT_SECT();

  if(!m1x_stmr_data.vfr_active)
  {
    /* leave critical section */
    M1X_STMR_LEAVE_CRIT_SECT();

    /* the vfr has not been allocated */
    return M1X_STMR_ERR_VFR_ALLOC;
  }

  M1X_MSG( MUX, LEGACY_HIGH, "releasing VFR" );

  if( m1x_stmr_data.vstmr_sub_id == VSTMR_RTC_1X_SUB0 )
  {
    vfr_int = M1X_STMR_PRIMARY_VFR;

    /* release the VFR */
    vstmr_1x_vfr_release( vfr_int, m1x_stmr_data.vstmr_sub_id );

    #ifdef FEATURE_MODEM_1X_VFR_SEL_REG
    /* this change will be required as part of Tabasco as well because JO
       shares a similar architecture with regard to VFR related registers */
    HWIO_OUT(VFR_SEL, 0);
    #endif /* FEATURE_MODEM_1X_VFR_SEL_REG */
  }
  else
  {
    vfr_int = M1X_STMR_SECONDARY_VFR;

    /* release the VFR */
    vstmr_1x_vfr_release( vfr_int, m1x_stmr_data.vstmr_sub_id );

    #ifdef FEATURE_MODEM_1X_VFR_SEL_REG
    /* this change will be required as part of Tabasco as well because JO
       shares a similar architecture with regard to VFR related registers */
    HWIO_OUT(VFR_SEL_1, 0);
    #endif /* FEATURE_MODEM_1X_VFR_SEL_REG */
  }

  /* set the bool variable to indiacate the vfr is inactive */
  m1x_stmr_data.vfr_active = 0;

  M1X_STMR_LEAVE_CRIT_SECT();

  return M1X_STMR_SUCCESS;

} /* m1x_stmr_release_vfr */

/*===========================================================================

FUNCTION      M1X_STMR_GET_FRAME_OFFSET

DESCRIPTION
  The current offset value being used for the PCG events offset is returned.

DEPENDENCIES
  None.

RETURN VALUE
  The current offset value being used for the PCG events offset.

SIDE EFFECTS
  None.
===========================================================================*/
uint16 m1x_stmr_get_frame_offset( void )
{
  uint16 result;

  /*-----------------------------------------------------------------------*/

  /* Sanity Check to do ERR_FATAL if the m1x_stmr module was not initialized */
  m1x_stmr_initialized_or_assert();

  M1X_STMR_ENTER_CRIT_SECT();
  result = m1x_stmr_data.pcg_offset;
  M1X_STMR_LEAVE_CRIT_SECT();

  return result;

} /*m1x_stmr_get_frame_offset*/

/*===========================================================================

FUNCTION      M1X_STMR_GET_SCOMB_PHASE

DESCRIPTION
  This function returns the phase of the SYNC_80 frame.

DEPENDENCIES
  None.

RETURN VALUE
  Returns the current phase within the Sync_80 frame.

SIDE EFFECTS
  None.
===========================================================================*/
uint32 m1x_stmr_get_scomb_phase( void )
{
  vstmr_1xscomb_t curr_scomb = { 0 };

  /*-----------------------------------------------------------------------*/

  curr_scomb = m1x_stmr_get_scomb();

  return curr_scomb.pnroll_num;

} /* m1x_stmr_get_scomb_phase */

/*===========================================================================

FUNCTION      M1X_STMR_GET_SCOMB_CHIPX8_COUNT

DESCRIPTION
  This function returns current number of chips x8 elapsed since the last
  SYNC_80 boundary.

DEPENDENCIES
  None.

RETURN VALUE
  The current number of chips x8 elapsed since the last SYNC_80 boundary.

SIDE EFFECTS
  None.
===========================================================================*/
uint32 m1x_stmr_get_scomb_chipx8_count( void )
{
  vstmr_1xscomb_t curr_scomb = { 0 };

  /*-----------------------------------------------------------------------*/

  curr_scomb = m1x_stmr_get_scomb();

  return curr_scomb.sync80_cx8;

} /* m1x_stmr_get_scomb_chipx8_count */

/*===========================================================================

FUNCTION      M1X_STMR_GET_SCOMB_CHIP_COUNT

DESCRIPTION
  This function returns the number of chips from the last SYNC_80 boundary.

DEPENDENCIES
  None.

RETURN VALUE
  The current number of chips elapsed since the last SYNC_80 boundary.

SIDE EFFECTS
  None.
===========================================================================*/
uint32 m1x_stmr_get_scomb_chip_count( void )
{
  uint32 scomb_cx8_count = 0;

  /*-----------------------------------------------------------------------*/

  scomb_cx8_count = m1x_stmr_get_scomb_chipx8_count();

  return ( scomb_cx8_count / 8 );

} /* m1x_stmr_get_scomb_chip_count */

/*===========================================================================

FUNCTION      M1X_STMR_GET_FRAME_AND_PCG_NUM_WITH_FRAME_OFFSET

DESCRIPTION
  This function populates the current PCG number and current frame number to
  the parameters passed. The values are Frame Offset adjusted.
  The range of the frame number is 0-511(10.24sec/20ms). The
  range of the PCG number is 0-15.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void m1x_stmr_get_frame_and_pcg_num_with_frame_offset
(
  uint32 *frame_num,
  uint32 *pcg_num
)
{
  vstmr_1xframe_t curr_frame = { 0 };

  /*-----------------------------------------------------------------------*/

  M1X_STMR_ENTER_CRIT_SECT();
  curr_frame = m1x_stmr_get_current_frame();

  if( curr_frame.pcg_num < m1x_stmr_data.pcg_offset )
  {
    /* This is the case when the frame offset causes a wraparound */
    curr_frame.pcg_num = VSTMR_1X_FRAME_PCG_NUM +
               curr_frame.pcg_num - m1x_stmr_data.pcg_offset;
  }
  else
  {
    curr_frame.pcg_num = curr_frame.pcg_num - m1x_stmr_data.pcg_offset;
  }

  /* Adjust the frame number based on current offset */
  if( curr_frame.pcg_num <= m1x_stmr_data.pcg_offset )
  {
    /* This is the case when the 1x layer1 frame hasnt started yet */
    curr_frame.frame_num--;
  }

  if( frame_num != NULL )
  {
    *frame_num = curr_frame.frame_num;
  }

  if( pcg_num != NULL )
  {
    *pcg_num = curr_frame.pcg_num;
  }

  M1X_STMR_LEAVE_CRIT_SECT();

} /* m1x_stmr_get_frame_and_pcg_num_with_frame_offset */

/*===========================================================================

FUNCTION      M1X_STMR_GET_LTE_1X_TIME

DESCRIPTION
  This function provides LTE and 1x time based on the same XO count.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void m1x_stmr_get_lte_1x_time( m1x_stmr_timer_dumps *ret_time )
{
  m1x_stmr_rtc_type  time_1x  = { 0 };
  vstmr_ostmr_t      time_lte = { 0 };
  uint32             time_xo  = 0;

  /*---------------------------------------------------------------------*/

  /* Sanity Check to do ERR_FATAL if the m1x_stmr module was not initialized */
  m1x_stmr_initialized_or_assert();

  /* get the current xo value */
  time_xo = VSTMR_XO_READ();

  /* convert the current xo to 1x RTC value */
  time_1x.rtc_full = vstmr_1x_xo_to_rtc( m1x_stmr_data.m1x_stmr_rtc_view,
                                         time_xo );

  /* convert the 1x value to cx32 */
  time_1x.rtc_full = time_1x.rtc_full >> 30;
  time_1x.rtc_full &= 0x1FFFFFFF;

  /* convert the xo to LTE ostmr value */
  time_lte = vstmr_lte_xo_to_ostmr(m1x_stmr_data.m1x_stmr_lte_view,
                                   time_xo);
  time_lte.w[1] = time_lte.w[1]& (0x3FFFFFFF);

  ret_time->l1x_cx32 = (uint32) time_1x.rtc_full;
  ret_time->lte_cx25 = time_lte.w[1];
  ret_time->xo_ticks = time_xo;

} /* m1x_stmr_get_lte_1x_time */

/*===========================================================================

FUNCTION      M1X_STMR_GET_USTMR_1X_TIME

DESCRIPTION
  This function provides full 1x time (RTC + Phase) based on the
  input XO count.

DEPENDENCIES
  None.

RETURN VALUE
  1X Time (RTC + Phase).

SIDE EFFECTS
  None.
===========================================================================*/
m1x_stmr_rtc_type m1x_stmr_get_ustmr_1x_time( uint32 ustmr )
{
  m1x_stmr_rtc_type time_1x = { 0 };

  /*---------------------------------------------------------------------*/

  /* Sanity Check to do ERR_FATAL if the m1x_stmr module was not initialized */
  m1x_stmr_initialized_or_assert();

  /* convert the current xo to 1x RTC value */
  time_1x.rtc_full = vstmr_1x_xo_to_rtc(m1x_stmr_data.m1x_stmr_rtc_view, ustmr);

  /* Ideally 5 MSB bits are 0. Remove the 5 MSB bits in case it't not zero. */
  time_1x.rtc_full &= 0x07FFFFFFFFFFFFFFULL;

  return time_1x;

} /* m1x_stmr_get_ustmr_1x_time */

/*===========================================================================

FUNCTION      M1X_STMR_GET_SUB_ID

DESCRIPTION
  This function provides VSTMR subscription ID currently used by 1x.

DEPENDENCIES
  None.

RETURN VALUE
  1x subscription ID: VSTMR_RTC_1X_SUB0 .

SIDE EFFECTS
  None.
===========================================================================*/
vstmr_rtc_id m1x_stmr_get_sub_id( void )
{
  /*---------------------------------------------------------------------*/

  return VSTMR_RTC_1X_SUB0;

} /* m1x_stmr_get_sub_id */

/*===========================================================================

FUNCTION      M1X_STMR_RE_REG_EVENTS

DESCRIPTION
  This function re-registers all currently active events in order to update
  them to current view timelines for their next trigger time.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void m1x_stmr_re_reg_events( void )
{
  int                 index = 0;
  m1x_stmr_event_type new_event = INVALID;

  /*---------------------------------------------------------------------*/

  M1X_MSG( MUX, LEGACY_MED,
     "Re-register has been triggered for all current events" );

  M1X_STMR_ENTER_CRIT_SECT();

  /* for loop through the event_id_tbl, find the event and its type */
  for(index = 0; index < TOTAL_PERIODIC_EVENTS; index++)
  {
    if(m1x_stmr_data.event_id_tbl[index] != INVALID)
    {
      /* Check and re-register for the correct event type. The enum ordering is
         important here. The ordering of PCG related events should be maintained.
         Any non-PCG related events should be added below PN ROLL event */
      new_event = m1x_stmr_data.event_id_tbl[index];
      if( new_event < M1X_STMR_PNROLL )
      {
        /* For frame event */
        m1x_stmr_i_deregister_frame_event(index);
        m1x_stmr_i_register_frame_event( new_event, index );
      }
      else
      {
        /* For scomb event */
        m1x_stmr_i_deregister_scomb_event(index);
        m1x_stmr_i_register_scomb_event( new_event, index );
      }

      /* event_id_tbl[index] will be reset in deregister event, we
         need to set it to ensure m1x_stmr is in a consistent
         state again */
      m1x_stmr_data.event_id_tbl[index] = new_event;
    }
  }

  M1X_STMR_LEAVE_CRIT_SECT();

} /* m1x_stmr_re_reg_events */

/*===========================================================================

FUNCTION      M1X_STMR_GET_LAST_SYNC80_RTC

DESCRIPTION
  This function returns the RTC value at the last SYNC80 boundary

DEPENDENCIES
  None.

RETURN VALUE
  RTC at previous sync80

SIDE EFFECTS
  None.
===========================================================================*/
uint32 m1x_stmr_get_last_sync80_rtc( void )
{
  vstmr_1xscomb_t curr_scomb         = { 0 };
  uint64          next_sync80_rtc    = 0;
  uint64          next_sync80_rtc_xo = 0;

  /*---------------------------------------------------------------------*/

  curr_scomb = m1x_stmr_get_scomb();

  curr_scomb.frac       = 0;
  curr_scomb.sync80_cx8 = 0;

  next_sync80_rtc_xo = vstmr_1x_scomb_to_xo( m1x_stmr_data.m1x_stmr_view, curr_scomb );

  next_sync80_rtc = vstmr_1x_xo_to_rtc( m1x_stmr_data.m1x_stmr_rtc_view, next_sync80_rtc_xo );

  return ( (uint32)( ( next_sync80_rtc >> 32 ) & 0xFFFFFFFF ) );

} /* m1x_stmr_get_last_sync80_rtc */

#ifdef FEATURE_QSH_MDUMP
/*===========================================================================
FUNCTION mc_qsh_mdump_collect_m1x_stmr_data

DESCRIPTION
This function dumps the m1x stmr data to QSH after a crash

DEPENDENCIES
  None


RETURN VALUE
None

SIDE EFFECTS
  None

===========================================================================*/
QSH_MDUMP_FN_ATTR
void mc_qsh_mdump_collect_m1x_stmr_data()
{
    qsh_mdump_collect_high(&m1x_stmr_data,sizeof(m1x_stmr_data));
}
#endif /* FEATURE_QSH_MDUMP */

/*===========================================================================
FUNCTION      M1X_STMR_GET_COMB_SNAP_SHOT

DESCRIPTION
  This function returns the COMB time for the given XO

DEPENDENCIES
  None.

RETURN VALUE
   returns COMB time for the given XO

SIDE EFFECTS
  None.
===========================================================================*/
uint32 m1x_stmr_get_comb_snap_shot( uint32 xo_time)
{
  vstmr_1xscomb_t  curr_scomb;

  /* snapshot of combiner count for XO */
  curr_scomb = vstmr_1x_xo_to_scomb(m1x_stmr_data.m1x_stmr_view, xo_time);

  return curr_scomb.sync80_cx8;

} /* m1x_stmr_get_comb_snap_shot */
