/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                              P S _ R M _ T A S K . C

DESCRIPTION
  This is the source file for the RM Data processing Task. Contained herein
  are the functions needed to initialize all the modules that execute in RM
  task context and the main task processing loop.


EXTERNALIZED FUNCTIONS
  ps_rm_task()
    RM Task entry point and main processing loop.

INITIALIZATION AND SEQUENCING REQUIREMENTS
  None of the RM services can be used unless MC has created the RM task
  and ps_rm_task() has been invoked.

Copyright (c) 2010-2016 Qualcomm Technologies Incorporated.
All Rights Reserved.
Qualcomm Confidential and Proprietary.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  $Header: //components/rel/data.mpss/3.5..2.1/interface/tasks/src/ps_rm_task.c#2 $ $DateTime: 2019/10/22 02:53:17 $ $Author: pwbldsvc $

when        who    what, where, why
--------    ---    ----------------------------------------------------------
06/01/16    ds     Removed API check whether ps rm task is initialized or not.
05/04/16    ds     Added an api check whether ps rm task is initialized or not.
11/09/14    yl     IP data path re-architecture
10/01/11   bvd     Adding rcinit featurization 
10/07/10    am     created module.
===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/

#include "datamodem_variation.h"
#include "comdef.h"
#include "customer.h"       /* Customer Specific Features */


#include "queue.h"
#include "err.h"
#include "ps_utils.h"
#include "dog_hb_rex.h"
#include "ps_rm_task.h"
#include "ps_rm_taski.h"
#include "ps_rm_svc.h"
#include "ps_rm_defs.h"

#include "rcinit_rex.h"

#include "ds_Utils_DebugMsg.h"

rex_tcb_type    * rex_ps_rm_tcb;

/*--------------------------------------------------------------------------
  Defines for DOG. Assuming DOG_DYNAMIC_API feat is ON, since they (core bsp)
  dont allow anymore additiions to dog files.
---------------------------------------------------------------------------*/
dog_report_type   ps_rm_dog_rpt_id           = 0;      /* Initial Safety */

/*===========================================================================

                           LOCAL FUNCTION DEFINITIONS FOR MODULE

===========================================================================*/

/*===========================================================================

FUNCTION PS_RM_DOG_HB_SIG_HANDLER()

DESCRIPTION
  This function is registered as the PS_RM_DOG_HB_REPORT_SIGNAL signal handler.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

static boolean ps_rm_dog_hb_sig_handler
(
  ps_rm_sig_enum_type sig,                   /* Signal to be processed       */
  void *user_data_ptr                      /* Parameter carrying user data */
)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  dog_hb_report( ps_rm_dog_rpt_id );

  return TRUE;
} /* ps_rm_dog_hb_sig_handler() */

/*===========================================================================

FUNCTION PS_RM_CMD_Q_SIG_HANDLER()

DESCRIPTION
  This function is registered as the PS_RM_CMD_Q_SIGNAL signal handler.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

static boolean ps_rm_cmd_q_sig_handler
(
  ps_rm_sig_enum_type sig,                   /* Signal to be processed       */
  void *user_data_ptr                      /* Parameter carrying user data */
)
{
  boolean ret = FALSE;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  ret = ps_rm_process_cmd();

  return ret;
} /* ps_rm_cmd_q_sig_handler() */

/*===========================================================================

FUNCTION PS_RM_OFFLINE_SIG_HANDLER()

DESCRIPTION
  This function is registered as the PS_RM_TASK_OFFLINE_SIGNAL signal handler.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

static boolean ps_rm_offline_sig_handler
(
  ps_rm_sig_enum_type sig,                   /* Signal to be processed       */
  void *user_data_ptr                      /* Parameter carrying user data */
)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  LOG_MSG_INFO2_0("ps_rm_task(): "
                  "PS_RM_TASK_OFFLINE_SIGNAL received");

  return TRUE;
} /* ps_rm_offline_sig_handler() */

/*===========================================================================
FUNCTION PS_RM_INIT()

DESCRIPTION
  Initialization function for the rm data processing task. This function
  performs the functions needed for the data task to exit disabled state.

DEPENDENCIES
  RM task should have been started.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void ps_rm_init
(
  void
)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /*------------------------------------------------------------------------
    Set signal hanlder for operation of all signals internal to PS_RM Task
  -------------------------------------------------------------------------*/
  PS_ENTER_CRIT_SECTION(&global_ps_rm_crit_section);

  ps_rm_set_sig_handler(PS_RM_DOG_HB_REPORT_SIGNAL, ps_rm_dog_hb_sig_handler, NULL);
  ps_rm_set_sig_handler(PS_RM_CMD_Q_SIGNAL, ps_rm_cmd_q_sig_handler, NULL);
  ps_rm_set_sig_handler(PS_RM_TASK_OFFLINE_SIGNAL, ps_rm_offline_sig_handler, NULL);

  PS_LEAVE_CRIT_SECTION(&global_ps_rm_crit_section);

} /* ps_rm_init() */

/*===========================================================================

                     EXTERNAL FUNCTION DEFINITIONS FOR MODULE

===========================================================================*/

/*===========================================================================
FUNCTION PS_RM_TASK()

DESCRIPTION
  This function is the entry point and main processing loop for the RM data task.

DEPENDENCIES
  Does not Return.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void ps_rm_task
(
  uint32 ignored    /* Parameter received from Main Control task - ignored */
)
{
  rex_sigs_type     loop_sigs;      /* Signals to be processed within a task
                                       loop */
  rex_sigs_type     set_sigs = 0;   /* Currently set signals               */
  uint32            i;

  /* variables for the bit field conversion */
  uint32 sig_buffer[PS_RM_MAX_SIGNALS];
  ps_rm_sig_enum_type curr_sig;
  uint32 num = 0;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
#ifdef TEST_FRAMEWORK
  #error code not present
#endif
  
  rex_ps_rm_tcb = rex_self();

#ifdef TEST_FRAMEWORK
  #error code not present
#endif
  rcinit_handshake_startup();

  ps_rm_dog_rpt_id = dog_hb_register_rex(
                (rex_sigs_type)1 << (rex_sigs_type)PS_RM_DOG_HB_REPORT_SIGNAL);

  ps_rm_init();

  /*-------------------------------------------------------------------------
    Main task loop. Never exit!
  -------------------------------------------------------------------------*/
  for( ;;)
  {
    /*-----------------------------------------------------------------------
      Wait for work to do. No return from wait until one of the enabled
      signals is set. Even if we come out of wait, process only those
      signals which are currently enabled.
    -----------------------------------------------------------------------*/

    set_sigs |= rex_get_sigs( rex_ps_rm_tcb );

    set_sigs &= ps_rm_enabled_sig_mask;

    /*-----------------------------------------------------------------------
      Enter into the Wait only if no requested signal events have occurred
    -----------------------------------------------------------------------*/
    if( set_sigs == 0 )
    {
      set_sigs = rex_wait( ps_rm_enabled_sig_mask );
      set_sigs &= ps_rm_enabled_sig_mask;
    }

    (void)rex_clr_sigs( rex_ps_rm_tcb, set_sigs );

    loop_sigs = set_sigs & ~( (rex_sigs_type) 1
                             << (rex_sigs_type) PS_RM_SIG_MASK_CHANGE_SIGNAL );
    set_sigs = 0;

#ifdef TEST_FRAMEWORK
      #error code not present
#endif /* TEST_FRAMEWORK */
    /*-----------------------------------------------------------------------
      Now handle the active signals one by one.
    -----------------------------------------------------------------------*/
    num = ps_find_set_bits(&loop_sigs, 1, sig_buffer);
    for( i=0; i < num; i++)
    {
      curr_sig = (ps_rm_sig_enum_type)sig_buffer[i];
      loop_sigs &= ~( (rex_sigs_type) 1 <<
                      (rex_sigs_type) curr_sig );

      if ( ps_rm_sig_handler[curr_sig].sig_handler
            (
              curr_sig ,
              ps_rm_sig_handler[curr_sig].user_data_ptr
            ) == FALSE )
      {
        set_sigs |= ( (rex_sigs_type) 1 <<
                      (rex_sigs_type) curr_sig);
      }
    }

    ASSERT(0 == loop_sigs);
  } /* forever */

} /* ps_rm_task() */
