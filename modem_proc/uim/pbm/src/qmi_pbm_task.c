/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                              Q M I _ P B M _ T A S K . C

DESCRIPTION
  This is the source file for the QMI PBM Task. Contained herein are the
  functions needed to initialize all the modules that execute in QMI task
  context and the main task processing loop.

EXTERNALIZED FUNCTIONS
  qmi_pbm_task()
    QMI Task entry point and main processing loop.

INITIALIZATION AND SEQUENCING REQUIREMENTS
  None of the QMI services can be used unless MC has created the QMI task
  and qmi_pbm_task() has been invoked.

Copyright (c) 2014 - 2015 QUALCOMM Technologies, Inc(QTI).
All Rights Reserved.
QUALCOMM Technologies Confidential and Proprietary.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  $Header: //components/rel/uim.mpss/6.1.0/pbm/src/qmi_pbm_task.c#2 $ $DateTime: 2019/10/30 06:56:05 $ $Author: pwbldsvc $

when        who    what, where, why
--------    ---    ----------------------------------------------------------
30/12/15    dd     Place braces for all code blocks
11/12/15    lm     Remove compiler warnings
10/14/15    vv     Use mcs wrappers for rcevt APIs
08/26/15    lm     Free memory allocated for globals memory on task exit
07/28/14    nr     Remove FEATURE_REX_SIGS_EXT
05/03/15    kv     Fix tcb ptr initialization for offtarget
04/30/15    kv     Deprecate usage of TASK_HANDLE
04/25/15    tl     Remove always ON FEATURE_MODEM_RCINIT & FEATURE_UIM_USES_TASK_HANDLE
07/25/14    NR     Use v2.0 diag macros
04/01/14    NR     QTF CRM MOB Changes
03/17/14    kb     Add offtarget support to check mutex is owned or not
01/12/14    NR     TMC header deprecation changes
11/26/13    nr     QTF CRM MOB changes
01/25/11    rk     Created module.
===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/

#include "uim_variation.h"
#include "comdef.h"
#include "customer.h"       /* Customer Specific Features */
#include "uim_msg.h"
#include "task.h"
#include "rex.h"
#include "queue.h"
#include "dog_hb_rex.h"
#include "rcinit.h"
#include "qmi_pbm_task.h"
#include "qmi_pbm_task_svc.h"
#include "qmi_pbm_taski.h"
#include "qmi_pbm_task_cmd.h"
#include "qmi_pbm_svc.h"
#include "amssassert.h"
#include "stringl/stringl.h"
#include "mcs_wrappers.h"

#ifdef FEATURE_UIM_TEST_FRAMEWORK
#error code not present
#endif /* FEATURE_UIM_TEST_FRAMEWORK */

/*===========================================================================

                  GLOBAL DEFINITIONS AND DECLARATIONS FOR MODULE

===========================================================================*/

rex_tcb_type   *qmi_pbm_tcb_ptr; // pointer for myself


  /*--------------------------------------------------------------------------
    Timers needed for the QMI Ctrl Task.
  ---------------------------------------------------------------------------*/
  /*--------------------------------------------------------------------------
    Dynamic dog variable
  ---------------------------------------------------------------------------*/
dog_report_type  qmi_pbm_dog_rpt_var            = 0;        /* Dog report ID       */

extern void qmi_pbm_svc_init
(
  void
);

/*===========================================================================

                  LOCAL DEFINITIONS AND DECLARATIONS FOR MODULE

===========================================================================*/

/*===========================================================================

                           LOCAL FUNCTION DEFINITIONS FOR MODULE

===========================================================================*/

/*===========================================================================
FUNCTION QMI_PBMI_INIT()

DESCRIPTION
  Initialization function for the various QMI services.

DEPENDENCIES
  QMI Modem task should have been started already.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
static void qmi_pbmi_init
(
  void
)
{
  /*-------------------------------------------------------------------------
    Initialize the QMI-Services here
  -------------------------------------------------------------------------*/
  qmi_pbm_svc_init();

/*-------------------------------------------------------------------------
    Other initializations can go here
-------------------------------------------------------------------------*/


} /* qmi_pbmi_init() */

/*===========================================================================

                     EXTERNAL FUNCTION DEFINITIONS FOR MODULE

===========================================================================*/
  /*===========================================================================

  FUNCTION qmi_pbm_init_before_task_start

  DESCRIPTION
    Initialize qmi pbm cmd queues and other objects that needs to be
    initialized before the task is started

    NOTE! Any initialization that must be done before other tasks or clients
    tries to contact QMI PBM, must be done in this initialization function.

  DEPENDENCIES
    none

  RETURN VALUE
    none

  SIDE EFFECTS
    none

  ===========================================================================*/
  void qmi_pbm_init_before_task_start( void )
  {


    /*-------------------------------------------------------------------------
      Do power-up initializtion then wait for start signal (from the MC task,
      which controls task startup sequencing)
    -------------------------------------------------------------------------*/
      qmi_pbmi_powerup();
  }

#ifdef FEATURE_UIM_TEST_FRAMEWORK
#error code not present
#endif /* FEATURE_UIM_TEST_FRAMEWORK */


/*===========================================================================
FUNCTION QMI_PBM_TASK()

DESCRIPTION
  This function is the entry point and main processing loop for the QMI task.

DEPENDENCIES
  Does not Return.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void qmi_pbm_task(
  uint32 ignored    /* Parameter received from Main Control task - ignored */
)
{
  rex_sigs_type     loop_sigs = 0;      /* Signals to be processed within a task
                                       loop */
  rex_sigs_type     set_sigs  = 0;   /* Currently set signals               */
  uint8             loop_cnt  = 0;   /* # of times task loop is iterated
                                       on a signal mask                    */
  uint8             i         = 0;

#ifdef FEATURE_UIM_TEST_FRAMEWORK
  #error code not present
#endif /* FEATURE_UIM_TEST_FRAMEWORK */

  qmi_pbm_tcb_ptr = rex_self();

  rcinit_handshake_startup();

  /* register with dog*/
  qmi_pbm_dog_rpt_var = dog_hb_register_rex((rex_sigs_type)(1<< QMI_PBM_DOG_RPT_TIMER_SIGNAL));

  qmi_pbmi_init();

#ifdef FEATURE_UIM_TEST_FRAMEWORK
  #error code not present
#endif /* FEATURE_UIM_TEST_FRAMEWORK */

  /*-------------------------------------------------------------------------
    Main task loop. Never exit!
  -------------------------------------------------------------------------*/
  for( ;;)
  {
#ifdef FEATURE_UIM_TEST_FRAMEWORK
    #error code not present
#endif
    /*-----------------------------------------------------------------------
      Wait for work to do. No return from wait until one of the enabled
      signals is set. Even if we come out of wait, process only those
      signals which are currently enabled.
    -----------------------------------------------------------------------*/
    set_sigs |= rex_get_sigs( rex_self() );
    set_sigs &= qmi_pbmi_enabled_sig_mask;

    /*-----------------------------------------------------------------------
      Enter into the Wait only if no requested signal events have occurred
    -----------------------------------------------------------------------*/
    if( set_sigs == 0 )
    {
      set_sigs = rex_wait( qmi_pbmi_enabled_sig_mask );
      set_sigs &= qmi_pbmi_enabled_sig_mask;
    }
    (void)rex_clr_sigs( rex_self(), set_sigs );


    loop_sigs = set_sigs & ~( (rex_sigs_type) 1
                             << (rex_sigs_type) QMI_PBM_SIG_MASK_CHANGE_SIGNAL );
    set_sigs = 0;

    /*-----------------------------------------------------------------------
      Now handle the active signals one by one in the order of priority.
      If the loop iterates for more than QMI_PBM_MAX_SIGNALS times, some signal
      is not handled by this loop. So instead of going in to infinite loop
      perform a check
    -----------------------------------------------------------------------*/
    loop_cnt = 0;
    while( loop_sigs && loop_cnt++ < (rex_sigs_type) QMI_PBM_MAX_SIGNALS )
    {
      /*---------------------------------------------------------------------
        If watchdog timer expired, kick the pooch and set timer for next.
      ---------------------------------------------------------------------*/
      if( loop_sigs & ( (rex_sigs_type) 1 <<
                        (rex_sigs_type) QMI_PBM_DOG_RPT_TIMER_SIGNAL ) )
      {
        loop_sigs &= ~( (rex_sigs_type) 1 <<
                        (rex_sigs_type)QMI_PBM_DOG_RPT_TIMER_SIGNAL );
        dog_hb_report(qmi_pbm_dog_rpt_var );
      }
      /*---------------------------------------------------------------------
       *
        QMI Task command queue signal
      ---------------------------------------------------------------------*/
      else if( loop_sigs & ( (rex_sigs_type) 1 <<
                             (rex_sigs_type) QMI_PBM_CMD_Q_SIGNAL ) )
      {
        loop_sigs &= ~( (rex_sigs_type) 1 <<
                        (rex_sigs_type) QMI_PBM_CMD_Q_SIGNAL );

        if ( qmi_pbmi_process_cmd() == FALSE )
        {
          set_sigs |= ( (rex_sigs_type) 1 <<
                        (rex_sigs_type) QMI_PBM_CMD_Q_SIGNAL );
        }
      }

      /*---------------------------------------------------------------------
        Process other signals with registered handlers
      ---------------------------------------------------------------------*/
      else
      {
        /*----------------------------------------------------------------------
          Handle the QMI Service Signal if set
        -----------------------------------------------------------------------*/
        for( i = QMI_PBM_SERVICE_PBM_SIGNAL; i < QMI_PBM_MAX_SIGNALS; i++)
        {

          if( loop_sigs & ( (rex_sigs_type) 1 <<
                                 (rex_sigs_type) i ) )
          {
            loop_sigs &= ~( (rex_sigs_type) 1 <<
                            (rex_sigs_type) i );

            if ( qmi_pbmi_sig_handler[i].sig_handler
                 (
                   i ,
                   qmi_pbmi_sig_handler[i].user_data_ptr
                 ) == FALSE )
            {
              UIM_MSG_HIGH_1("qmi_pbm_task(): QMI_PBM_SERVICE_%d_SIGNAL %d", i);
              set_sigs |= ( (rex_sigs_type) 1 <<
                            (rex_sigs_type) i);
            }
            /*-----------------------------------------------------------------
              allowing one signal type at a time to process which will avoid
              the watchdog timer expiration
            --------------------------------------------------------------------*/
            break;
          }
        }
      }

#ifdef FEATURE_UIM_TEST_FRAMEWORK
      #error code not present
#endif /* FEATURE_UIM_TEST_FRAMEWORK */

#ifndef FEATURE_MODEM_RCINIT_PHASE2

      /*---------------------------------------------------------------------
        Check if powerdown command signal was set.  If set then clear signal,
        and process task stop procedures.
      ---------------------------------------------------------------------*/
      if( loop_sigs & ( (rex_sigs_type) 1 <<
                             (rex_sigs_type) QMI_PBM_TASK_STOP_SIGNAL ) )
      {
        UIM_MSG_HIGH_1("qmi_pbm_task(): QMI_PBM_TASK_STOP_SIGNAL received %d", ignored);

        loop_sigs &= ~( (rex_sigs_type) 1 <<
                        (rex_sigs_type) QMI_PBM_TASK_STOP_SIGNAL );

        task_stop();

      }
      /*---------------------------------------------------------------------
        Check if offline command signal was set.  If set then clear signal,
        and process task offline procedures.
      ---------------------------------------------------------------------*/
      if( loop_sigs & ( (rex_sigs_type) 1 <<
                             (rex_sigs_type) QMI_PBM_TASK_OFFLINE_SIGNAL ) )
      {
        UIM_MSG_HIGH_0("qmi_pbm_task(): QMI_PBM_TASK_OFFLINE_SIGNAL received");

        loop_sigs &= ~( (rex_sigs_type) 1 <<
                        (rex_sigs_type) QMI_PBM_TASK_OFFLINE_SIGNAL );

        task_offline();

      }
#endif
    }

    ASSERT(0 == loop_sigs);
  } /* forever */
#ifdef FEATURE_UIM_TEST_FRAMEWORK
  #error code not present
#endif /* FEATURE_UIM_TEST_FRAMEWORK */

} /* qmi_pbm_task() */
