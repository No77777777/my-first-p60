
/*===*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                   HIT Test Application Task for hdr and wcdma

GENERAL DESCRIPTION
  This file contains the task implementation for HIT Test App for hdr and wcdma

EXTERNALIZED FUNCTIONS
  hitapp_wait
  hitapp_cmd_handler
  
REGIONAL FUNCTIONS
  hitapp_init
  hitapp_kick_watchdog
  hitapp_process_command
  hitapp_task

INITIALIZATION AND SEQUENCING REQUIREMENTS

  Copyright (c) 2006 - 2015 by Qualcomm Technologies, Inc.  All Rights Reserved.
    
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

============================================================================

                        EDIT HISTORY FOR MODULE

$Header: //components/rel/hdr.mpss/6.0/hit/src/hitapp.c#1 $ $DateTime: 2019/06/19 22:12:26 $ $Author: pwbldsvc $

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

when         who     what, where, why
----------   ---     -------------------------------------------------------- 
08/17/2015   vlc     Added code to handle Run Control Event Notification errors.
                     Mainlined FEATURE_MODEM_RCINIT.
06/19/2015   tnp     Fix compiler warnings
10/17/2014   ljl     Included amssassert.h. 
07/29/2014   tnp     FR22375: Fix Medium compiler warnings in HDR
07/09/2014   cnx     Support NV refresh without reset.
01/15/2013   smd     Featurized hit cmd and hit diag.
08/08/2013   rmv     Featurized tmc APIs with ifndef FEATURE_MODEM_RCINIT
05/06/2013   mbs     Fixed compiler errors for BOLT build
03/04/2013   vko     Fixed compiler warnings
03/01/2013   dsp     Moved watchdog timer code to watchdog heart-beat model.
02/14/2013   dsp     Removed in hitapp task the wait for hdrbc task to be ready.
04/30/2012   dsp     Fixed compiler warnings on Dime. 
03/30/2012   ljl     Used tcm_task_* APIs. 
03/22/2012   arm     Fixed featurization bug for RCINIT.  
03/20/2012   cnx     Resolved TCB compiler errors for Dime. 
03/16/2012   arm     Resolve TCB errors for Dime. 
03/13/2012   smd     Added RCINIT support. 
02/27/2012   smd     Added updates of FTM msgr interface changes.
08/23/2007   vh      Added critical section around command handling
05/07/2007   rmg     Changed dynamic memory allocation to static one
03/28/2007   vh      Created

==========================================================================*/

/*==========================================================================

                     INCLUDE FILES FOR MODULE

==========================================================================*/
#include "hdr_variation.h"
#include "customer.h"
#include "amssassert.h"

#ifdef FEATURE_HIT

#include "comdef.h"
#include "rex.h"
#include "task.h"
#include "dog.h"
#include "dog_hb_rex.h"
#include "nv.h"
#include "diagpkt.h"
#include "diagcmd.h"
#include "hitapp.h"
#include "hdrhitmsg.h"
#include "hdrutil.h"

/*==========================================================================

                     DEFINITIONS AND DECLARATIONS FOR MODULE

This section contains local definitions for constants, macros, typesdefs,
and other items needed by this module.

==========================================================================*/

rex_tcb_type *hdrhitapp_tcb_ptr;

#include "rcinit.h"
#define HITAPP_RCEVT_PREFIX "HITAPP:"
#define HITAPP_RCEVT_READY HITAPP_RCEVT_PREFIX "ready"
#define HDRBC_RCEVT_PREFIX "HDRBC:"
#define HDRBC_RCEVT_READY HDRBC_RCEVT_PREFIX "ready"

#define HITAPP_CMD_QUEUE_SIZE     30 
  /* Max number of commands that can be queued */

#ifdef FEATURE_HDR_HIT
extern hitcmd_ret_type hithdrcmd_handler ( hitcmd_buf_type* );
extern rex_crit_sect_type hithdrcmd_crit_sect;
  /* function that processes a hithdr command */
#endif


#ifdef FEATURE_HDR_HIT
extern void hdrhitcmd_enable ( uint8 hit_enable );
  /* conveys the status of the NV item - hit_enable to HDR */
#endif


/*==========================================================================

                     GLOBAL VARIABLES FOR MODULE

==========================================================================*/


static rex_timer_type hitapp_delay_timer;
  /* timer for delaying command execution as specified by the command */

#if defined( DOG_DYNAMIC_API ) && defined( FEATURE_HDR_WDOG_DYNAMIC )
dog_report_type   hitapp_dog_rpt_var = 0;
  /* Dog rpt */

uint32 hitapp_dog_rpt_time_var = 0xffff;
  /* Dog rpt time */
#endif
/*===========================================================================

FUNCTION hitapp_kick_watchdog

DESCRIPTION
  This procedure kicks the watch dog and resets the watch dog timer

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
static void hitapp_kick_watchdog(void)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
  (void) rex_clr_sigs( hdrhitapp_tcb_ptr,  HITAPP_RPT_TIMER_SIG );
  dog_hb_report( hitapp_dog_rpt_var );
}

/*===========================================================================

FUNCTION hitapp_wait

DESCRIPTION
  This procedure performs a REX wait on a signal that the caller specifies. 
  In additions to that the function must handle watchdog timer expiry and 
  STOP and offline signals

DEPENDENCIES
  None

PARAMETERS
  sigs: which signal to wait for

RETURN VALUE
  sigs

SIDE EFFECTS
  None

===========================================================================*/

uint16 hitapp_wait
(
  uint16 sigs
    /* which signal to wait for */
)
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  uint16 rex_signals_mask;

  do
  {
    /* set signal mask */
    rex_signals_mask = rex_wait( sigs | HITAPP_RPT_TIMER_SIG | 
                                 TASK_STOP_SIG | TASK_OFFLINE_SIG );

    if( ( rex_signals_mask & TASK_STOP_SIG ) != 0 )
    {
      MSG( MSG_SSID_HIT,  MSG_LEGACY_MED, "TASK_STOP_SIG received" );
        /*  */

      (void) rex_clr_sigs( hdrhitapp_tcb_ptr, TASK_STOP_SIG );
        /*  */

    }

    if ( ( rex_signals_mask & TASK_OFFLINE_SIG ) != 0 ) 
    {
     (void) rex_clr_sigs( hdrhitapp_tcb_ptr, TASK_OFFLINE_SIG );
       /*  */

     MSG( MSG_SSID_HIT,  MSG_LEGACY_MED, "TASK_OFFLINE_SIG received" );
       /*  */

    }

    if ( ( rex_signals_mask & HITAPP_RPT_TIMER_SIG ) != 0 ) 
    {
     (void) rex_clr_sigs( hdrhitapp_tcb_ptr, HITAPP_RPT_TIMER_SIG );
     hitapp_kick_watchdog(); 
    }

    if ( ( rex_signals_mask & sigs ) != 0 )
    {
      (void) rex_clr_sigs( hdrhitapp_tcb_ptr, sigs );
      break;
    }

  }while( 1 );

  return rex_signals_mask;

}

/*===========================================================================

FUNCTION hitapp_process_command

DESCRIPTION
  This procedure processes each command

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

static void hitapp_process_command( void )
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
  MSG( MSG_SSID_HIT, MSG_LEGACY_MED, 
       "HITAPP_PROCESS_CMD: hit cmd processing is not supported" );
} /* end hitapp_process_command() */


/*===========================================================================

FUNCTION HITAPP_READ_NV_ITEM

DESCRIPTION
  This function reads an item from NV

DEPENDENCIES
  None.

RETURN VALUE
  NV command return value

SIDE EFFECTS
  ERR_FATAL if the command return is not DONE or NOTACTIVE

===========================================================================*/
nv_stat_enum_type hitapp_read_nv_item
(
  nv_items_enum_type item_code, /* item to read */
  nv_item_type * item_ptr /* pointer to where to put the item */
)
{
  static nv_cmd_type hit_nv_cmd_buf; /* Command buffer to NV */

  ASSERT(rex_self() == hdrhitapp_tcb_ptr);

  /* Prepare command buffer to NV */

  hit_nv_cmd_buf.cmd = NV_READ_F;
  hit_nv_cmd_buf.tcb_ptr = hdrhitapp_tcb_ptr;
  hit_nv_cmd_buf.sigs = HITAPP_NV_READ_SIG;
  hit_nv_cmd_buf.done_q_ptr = NULL;
  hit_nv_cmd_buf.item = item_code;
  hit_nv_cmd_buf.data_ptr = item_ptr;

  /* Clear signal, issue the command, wait for the response */
  (void)rex_clr_sigs( hdrhitapp_tcb_ptr, HITAPP_NV_READ_SIG );
  nv_cmd( &hit_nv_cmd_buf );
  (void)hitapp_wait( HITAPP_NV_READ_SIG );
  (void)rex_clr_sigs( hdrhitapp_tcb_ptr, HITAPP_NV_READ_SIG );
  
  /* check and return status */

  if ((hit_nv_cmd_buf.status == NV_DONE_S) ||
      (hit_nv_cmd_buf.status == NV_NOTACTIVE_S))
  {
  }
  else
  {
    ERR_FATAL( "Bad NV read status %d", hit_nv_cmd_buf.status, 0, 0);
  }

  return (hit_nv_cmd_buf.status);

} /* hitapp_read_nv_item */


/*===========================================================================

FUNCTION hitapp_init

DESCRIPTION
  HITAPP task initializtion function

DEPENDENCIES
  None

PARAMETERS
  dummy

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void hitapp_init( void )
{

#ifdef FEATURE_HDR_HIT
  /* initilize critical section */
  rex_init_crit_sect( &hithdrcmd_crit_sect );
#endif /* FEATURE_HDR_HIT */

} /* hitapp_init */

/*===========================================================================

FUNCTION hitapp_refresh_nv

DESCRIPTION
  This procedure reads NV item and control the availablity of HIT module.

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void hitapp_refresh_nv(void)
{

}

/*===========================================================================

FUNCTION hitapp_task

DESCRIPTION
  This procedure is the entry procedure for the task. It calls the init function,
  kicks the watchdog and waits for commands. When a command arrives it is processed.

DEPENDENCIES
  None

PARAMETERS
  dummy

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void hitapp_task
(
  uint32 dummy
    /* dummy variable required to define a task */
)
{
  nv_item_type      nv_item;   /* To read the nv item */
  rex_sigs_type     sigs;      /* Active task signals */
    
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */ 

  rcinit_handshake_startup();

  /* Run time synch with other tasks is done here if needed, following APIs
  ** used: rcinit_lookup(),  rcevt_wait_name()
  */
#ifdef FEATURE_HDR_BCMCS
  if ( rcevt_wait_name(HDRBC_RCEVT_READY) == RCEVT_NULL )
  {
    MSG( MSG_SSID_HIT,  MSG_LEGACY_ERROR, "HIT: Unexpected Run Control Event Notification err" );
  }
#endif
  if ( rcevt_signal_name(HITAPP_RCEVT_READY) == RCEVT_NULL )
  {
    MSG( MSG_SSID_HIT,  MSG_LEGACY_ERROR, "HIT: Unexpected Run Control Event Notification err" );
  }

  /* RCINIT calls init function defined in task scons file in rcinit context
     All tcb related intializations should be done after rcinit starts our
     task */
  hdrhitapp_tcb_ptr = rex_self();

  /* define CMD Delay Timer */
  rex_def_timer( &hitapp_delay_timer, hdrhitapp_tcb_ptr, HITAPP_TIMER_SIG );

  /* Initialize message router functionalities */
  hdrhitmsg_init();

  hitapp_dog_rpt_var = dog_hb_register_rex( (rex_sigs_type) HITAPP_RPT_TIMER_SIG );

#if defined(FEATURE_RUMI_BRINGUP) || defined(FEATURE_RUMI3_BRINGUP)
  nv_item.hit_enable = 1;
  /* Bypass nv item */
#else
  /* Read HIT NV item to see if it is enabled */
  (void) hitapp_read_nv_item( NV_HIT_ENABLE_I, &nv_item);
#endif /* FEATURE_RUMI_BRINGUP */


#ifdef FEATURE_HDR_HIT
  /* convey the status of the NV item to HDR */
  hdrhitcmd_enable( nv_item.hit_enable );
#endif

  hitapp_kick_watchdog();
    /*Initially kick the watch dog */

  /* Wait in a loop for commands*/
  for(;;)
  {
    sigs = hitapp_wait( HITAPP_CMD_Q_SIG | HITAPP_MSG_Q_SIG ); 
      /* wait for CMD in the queue */

    if ( sigs & HITAPP_CMD_Q_SIG )
    {
    
      hitapp_process_command();
      /* process the command */
    }
    else if ( sigs & HITAPP_MSG_Q_SIG )
    {
      hdrhitmsg_process_rcvd_msgs();
        /* process the message */
    }
    else
    {
      MSG( MSG_SSID_HIT,  MSG_LEGACY_ERROR, "HIT: unrecognized signal" );
    }
  }
}


/*===========================================================================

FUNCTION hitapp_cmd_handler

DESCRIPTION
  This procedure is the external interface for other tasks to send commands to the task.
  This function uses the hit common routine to put the packet in the queue and then 
  it signals the task.

DEPENDENCIES
  None

PARAMETERS
  cmd_buf_ptr
  signal: If set to true only then the task (test application) is signalled

RETURN VALUE
  status

SIDE EFFECTS
  None

===========================================================================*/
#endif /* FEATURE_HIT */
