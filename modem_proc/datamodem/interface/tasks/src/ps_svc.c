/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                               P S _ S V C . C

GENERAL DESCRIPTION
  This is the header file for the Data protocol Task. Contained herein are
  all the definitions, functions, and structures for other software tasks
  to communicate with the Data Protocol Task.

EXTERNALIZED FUNCTIONS
  ps_enable_sig()
    Allow a signal to be processed

  ps_set_sig_handler()
    Set a handler to process a signal in PS context

  ps_send_cmd()
    Post a command to the Data Protocol Task

  ps_set_cmd_handler()
    Set a handler to process a cmd in PS context

Copyright (c) 2002-2012 Qualcomm Technologies Incorporated.
All Rights Reserved.
Qualcomm Confidential and Proprietary
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  $PVCSPath: L:/src/asw/MM_DATA/vcs/ps_svc.c_v   1.2   19 Nov 2002 17:29:10   jeffd  $
  $Header: //components/rel/data.mpss/3.5..2.1/interface/tasks/src/ps_svc.c#2 $ $DateTime: 2019/10/22 02:53:17 $ $Author: pwbldsvc $

when        who    what, where, why
--------    ---    ---------------------------------------------------------- 
05/18/12    msh    RCINIT PHASE 2 feature change in ps_wait
05/06/11    hm     Use ps_task_crit_section for all PS_task operations. 
01/10/11    ss     Cleaning up of Globals for thread safety in Q6 Free 
                   Floating environment.
09/27/10    ss     DYNAMIC DOG support for DATA tasks.
08/11/10    ss     Rearchitectured PS extension signal mechanism. Any number
                   of signals supported now.
04/27/09    pp     Protect ps_ext1_sig_mask under critical section always.
09/03/09    pp     Added ps_wait, ps_wait_ext APIs.
03/26/09    pp     CMI De-featurization.
11/28/08    pp     ps_send_cmd: Use of PS_ENTER_CRIT_SECTION/UNLOCK macros.
10/24/08    pp     Removed unused variable.
09/07/07    scb    Added REX signal ext code under FEATURE_REX_SIGS_EXT
09/19/06    msr    Using atomic operations in ps_send_cmd()
09/12/06    jd     Support for multiplexing RX signals 32-63 on generic rx sig
04/30/04    mct    Fixed some lint errors.
11/19/02    jd     Added global ps_nv_status for use in PS task
11/14/02    jd     Added global ps_nv_item for use in PS task
03/18/02    usb    Created Initial Version

===========================================================================*/


/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/

#include "datamodem_variation.h"
#include "comdef.h"
#include "customer.h"       /* Customer Specific Features */

#include "amssassert.h"
#include "queue.h"
#include "msg.h"
#include "err.h"
#include "ps_svc.h"
#include "ps.h"
#include "psi.h"
#include "dog_hb_rex.h"
#include "ps_utils.h"
#include "ps_crit_sect.h"
#include "ds_Utils_DebugMsg.h"
#include "stringl.h"
#include "rcinit_rex.h"


/*===========================================================================

                  LOCAL DEFINITIONS AND DECLARATIONS FOR MODULE

===========================================================================*/
#ifdef __cplusplus
extern "C" rex_tcb_type  ps_tcb;
#endif

extern rex_tcb_type * rex_ps_tcb;

/*===========================================================================

                           FUNCTION DEFINITIONS FOR MODULE

===========================================================================*/

/*===========================================================================

FUNCTION PS_ENABLE_SIG()

DESCRIPTION
  This function enables the specified signal by adding it to the mask
  which the PS task uses for rex_wait()

DEPENDENCIES
  A valid signal handler should be registered with PS before enabling the
  signal.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void ps_enable_sig
(
  ps_sig_enum_type sig               /* Signal to be enabled               */
)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  PS_ENTER_CRIT_SECTION(&ps_task_crit_section);
  if ( psi_sig_handler[sig].sig_handler == default_sig_handler )
  {
    LOG_MSG_ERROR_1("ps_enable_sig(): "
                    "Signal %d enabled without a handler", sig );
  }

  PS_SET_SIG(psi_enabled_sig_mask, sig);
  ps_set_signal(PS_SIG_MASK_CHANGE_SIGNAL);

  PS_LEAVE_CRIT_SECTION(&ps_task_crit_section);
} /* ps_enable_sig() */


/*===========================================================================

FUNCTION PS_SET_SIG_HANDLER()

DESCRIPTION
  This function sets a handler for the specified signal

DEPENDENCIES
  None.

RETURN VALUE
  Previously registered signal handler

SIDE EFFECTS
  None.

===========================================================================*/
ps_sig_handler_type ps_set_sig_handler
(
  ps_sig_enum_type sig,              /* Signal assocaited with handler     */
  ps_sig_handler_type sig_handler,   /* Signal handler function pointer    */
  void *user_data_ptr                /* Param to be passed to the handler  */
)
{
  ps_sig_handler_type old_handler;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  PS_ENTER_CRIT_SECTION(&ps_task_crit_section);

  ASSERT( sig_handler != NULL );

  old_handler = psi_sig_handler[sig].sig_handler;

  psi_sig_handler[sig].sig_handler = sig_handler;
  psi_sig_handler[sig].user_data_ptr = user_data_ptr;
  PS_LEAVE_CRIT_SECTION(&ps_task_crit_section);

  return old_handler;
} /* ps_set_sig_handler() */

/*===========================================================================

FUNCTION PS_RESET_SIG_HANDLER()

DESCRIPTION
  This function resets a handler for the specified signal

DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None.

===========================================================================*/
void ps_reset_sig_handler
(
  ps_sig_enum_type sig
)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  PS_ENTER_CRIT_SECTION(&ps_task_crit_section);

  psi_sig_handler[sig].sig_handler = default_sig_handler;
  psi_sig_handler[sig].user_data_ptr = NULL;

  PS_LEAVE_CRIT_SECTION(&ps_task_crit_section);
} /* ps_set_sig_handler() */


/*===========================================================================

FUNCTION PS_SEND_CMD()

DESCRIPTION
  This function posts a cmd for processing in PS task context.  The cmd is
  processed by calling the registered cmd handler, if any.

  NOTE: The passed command will be copied to a PS task
  command item local to the PS Task.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void ps_send_cmd
(
  ps_cmd_enum_type cmd,          /* Actual command to be processed         */
  void *user_data_ptr            /* Command specific user parameters       */
)
{
  psi_cmd_type *cmd_ptr;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /*-------------------------------------------------------------------------
    Get a cmd buffer from the free queue, call error routine if no links
    available, else copy buffer to cmd queue and signal PS task
  -------------------------------------------------------------------------*/
  if( ( cmd_ptr = ( psi_cmd_type * ) q_get( &psi_cmd_free_q ) ) == NULL )
  {
    LOG_MSG_ERROR_0("ps_send_cmd(): "
                    "PS task cmd buffers unavailable" );
  }
  else
  {
    cmd_ptr->cmd = cmd;
    cmd_ptr->user_data_ptr = user_data_ptr;

    /*-----------------------------------------------------------------------
      Making the operation atomic so that race conditions do not occur
    -----------------------------------------------------------------------*/
    PS_ENTER_CRIT_SECTION(&ps_task_crit_section);
    q_put( &psi_cmd_q, &cmd_ptr->link );
    PS_SET_SIGNAL( PS_CMD_Q_SIGNAL );
    PS_LEAVE_CRIT_SECTION(&ps_task_crit_section);
  }
} /* ps_send_cmd() */


/*===========================================================================

FUNCTION PS_SET_CMD_HANDLER()

DESCRIPTION
  This function sets a handler for the specified cmd type.  If a NULL
  function pointer is passed the cmd handling is disabled.

DEPENDENCIES
  None.

RETURN VALUE
  Previously registered cmd handler

SIDE EFFECTS
  None.

===========================================================================*/
ps_cmd_handler_type ps_set_cmd_handler
(
  ps_cmd_enum_type cmd,              /* Command name                      */
  ps_cmd_handler_type cmd_handler    /* Cmd handler function pointer      */
)
{
  ps_cmd_handler_type old_handler;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  PS_ENTER_CRIT_SECTION(&ps_task_crit_section);
  old_handler = psi_cmd_handler[cmd];

  psi_cmd_handler[cmd] = ( cmd_handler != NULL) ?
                          cmd_handler :
                          default_cmd_handler;
  PS_LEAVE_CRIT_SECTION(&ps_task_crit_section);

  return old_handler;
} /* ps_set_cmd_handler() */

/*===========================================================================

FUNCTION PS_SET_SIGNAL()

DESCRIPTION
  This function sets a signal to PS task (ps_tcb) for processing.
  - Recommended API for all signals including ext signals.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void ps_set_signal
(
  ps_sig_enum_type sig
)
{
  rex_sigs_type sig_arr[PS_SIG_ARR_LEN];
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /*-----------------------------------------------------------------------
    If PS TCB is not set, just ignore the signal!
  -----------------------------------------------------------------------*/  
  if (rex_ps_tcb == NULL)
  {
    return;
  }
  
  memset(sig_arr, 0, sizeof(sig_arr));

  PS_ENTER_CRIT_SECTION(&ps_task_crit_section);

  PS_SET_SIG(sig_arr, sig);

  if(FALSE == rex_set_sigs_ext(rex_ps_tcb, sig_arr))
  {
    LOG_MSG_ERROR_1("ps_set_signal(): "
                    "Failure to set signal %d in ps_tcb", sig);
    ASSERT(0);
  }

  PS_LEAVE_CRIT_SECTION(&ps_task_crit_section);
} /* ps_set_signal */

/*===========================================================================

FUNCTION PS_CLR_SIGNAL()

DESCRIPTION
  This function clears the signal posted onto PS task (ps_tcb).
  - Recommended API for all signals including ext signals.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void ps_clr_signal 
(
  ps_sig_enum_type sig
)
{
  rex_sigs_type sig_arr[PS_SIG_ARR_LEN];
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  PS_ENTER_CRIT_SECTION(&ps_task_crit_section);
  memset(sig_arr, 0, sizeof(sig_arr));

  /*-------------------------------------------------------------------------
    Set the signal to be cleared by calling REX api
  -------------------------------------------------------------------------*/
  PS_SET_SIG(sig_arr, sig);
  
  if(FALSE == (rex_clr_sigs_ext(rex_ps_tcb, sig_arr))) 
  { 
    LOG_MSG_ERROR_1("ps_clr_signal(): "
                    "Failure to clear signal %d in ps_tcb", sig);
    ASSERT(0);
  } 

  PS_LEAVE_CRIT_SECTION(&ps_task_crit_section);
}/* ps_clr_signal */

/*===========================================================================

FUNCTION PS_SET_EXT1_SIGNAL()

DESCRIPTION
  This function sets a signal to PS task (ps_tcb) for processing. 
  - EXTENSION signals ONLY
  ::Protect ps_ext1_sig_mask under critical section always.

  Not needed after re-arch. Preserved for backward compatibility.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void ps_set_ext_signal
(
  ps_sig_enum_type sig
)
{
  ps_set_signal(sig);
} /* ps_set_ext_signal */

/*===========================================================================

FUNCTION PS_CLR_EXT1_SIGNAL()

DESCRIPTION
  This function clears the signal posted onto PS task (ps_tcb). 
  - EXTENSION signals ONLY
  - Signal must have set using PS_SET_EXT1_SIGNAL macro.
  ::Protect ps_ext1_sig_mask under critical section always.
  
  Not needed after re-arch. Preserved for backward compatibility.
  
DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void ps_clr_ext_signal 
(
  ps_sig_enum_type sig
)
{
  ps_clr_signal(sig);
}/* ps_clr_ext_signal */

/*===========================================================================
FUNCTION PS_WAIT()

DESCRIPTION
  This function performs the PS Task waiting function. This function will 
  not return until at least one signal from the requested mask is set. 
  The watchdog is also kicked at least once.
  
DEPENDENCIES
  requested_mask should be an array of size PS_MAX_SIG_ARR_LEN.

RETURN VALUE
  The signal mask actually set out of the requested mask which terminated 
  the wait.

SIDE EFFECTS
  All the set signals in the requested mask are cleared

NOTE
  Supports REX extension signals.
===========================================================================*/
rex_sigs_type* ps_wait
( 
  rex_sigs_type* requested_mask              /* mask of signals to wait for */
)
{
  /*lint -e506 */
  rex_sigs_type set_sigs[PS_MAX_SIG_ARR_LEN];     /* Currently set signals */
  boolean sig_result =  FALSE;
  uint32 i = 0;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /*-------------------------------------------------------------------------
    First check to see if one of the signals in the mask is already set.  If 
    yes, then clear those signals and return. Otherwise enter into the wait
    only if no requested signal events have occurred
  -------------------------------------------------------------------------*/
  memset(set_sigs, 0, sizeof(set_sigs));

  sig_result = rex_get_sigs_ext(rex_ps_tcb, set_sigs);
  if(sig_result == FALSE)
  {
    DATA_ERR_FATAL("ps_wait_ext(): Failed to obtain the signals from the PS TCB.");
    return NULL;
  }

  /*-------------------------------------------------------------------------
    Wait till one of the requested signals is set.
  -------------------------------------------------------------------------*/
  /*lint -e716 */
  while(TRUE)
  {
    sig_result = FALSE;
    /*lint -e506 */
    for(i = 0; i < PS_MAX_SIG_ARR_LEN; i++)
    {
      if(set_sigs[i] & requested_mask[i])
      {
        sig_result = TRUE;
        break;
      }
    }

    if( FALSE == sig_result )
    {
      PS_SET_SIG(requested_mask, PS_DOG_HB_REPORT_SIGNAL);

      sig_result = rex_wait_ext(requested_mask, set_sigs);
      if( FALSE == sig_result )
      {
        DATA_ERR_FATAL("ps_wait_ext():rex_wait_ext() failed.");
        return NULL;
      }

      if(PS_IS_SET_SIG(set_sigs, PS_DOG_HB_REPORT_SIGNAL))
      {
        PS_CLR_SIGNAL(PS_DOG_HB_REPORT_SIGNAL);
        dog_hb_report( ps_dog_rpt_id );
        PS_CLR_SIG(requested_mask, PS_DOG_HB_REPORT_SIGNAL);
      }
    }
    else
    {
      break;
    }
  }

  /*lint -e506 */
  for(i = 0; i < PS_MAX_SIG_ARR_LEN; i++)
  {
    requested_mask[i] &= set_sigs[i];
  }

  /*-------------------------------------------------------------------------
    Clear the signals being sent back to the user.
  -------------------------------------------------------------------------*/
  sig_result = rex_clr_sigs_ext(rex_ps_tcb, requested_mask);
  if(sig_result == FALSE)
  {
    DATA_ERR_FATAL("ps_wait_ext():rex_clr_sigs_ext() failed.");
    return NULL;
  }

  return requested_mask;
} /* ps_wait() */

