/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*


                          PHONEBOOK MANAGER UIM ENTRIES

GENERAL DESCRIPTION
  This file contains the functions used to manage the phonebook task and
  queues

  Copyright (c) 2002 - 2019 by QUALCOMM Technologies, Inc(QTI).
  All Rights Reserved.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/


/*===========================================================================
                        EDIT HISTORY FOR MODULE

$Header: //components/rel/uim.mpss/6.1.0/pbm/src/pbmtask.c#2 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
03/12/18   nr      Notify fdn_status to clients
04/19/17   bcho    Moving QCRIL ECC DB to PBM
10/03/16   dd      Removed lint comments
05/24/16   sp      Fix low level compiler warning
05/19/16   nr      Logging improvements
04/18/16   sp      ECC only support in PBM
04/13/16   sp      Reduce global memory used by PBM
30/12/15   dd      Place braces for all code blocks
12/17/15   vdc     Move event data to heap to reduce stack size
11/12/15   lm      Remove compiler warnings
10/30/15   gm      Handling TASK STOP signal
10/14/15   vv      Use mcs wrappers for rcevt APIs
10/01/15   lm      Fix compilation issues on linux
08/26/15   lm      Free memory allocated for globals at task exit
09/30/15   nr      Cmd_ptr freeing twice
09/26/15   nr      Fix for pbm crash while MCFG NV refresh handling
08/05/15   nr      Operator specifc NV ECC numbers configuration
05/03/15   kv      Fix tcb ptr initialization for offtarget
04/30/15   kv      Deprecate usage of TASK_HANDLE
04/25/15   tl      Remove always ON FEATURE_MODEM_RCINIT & FEATURE_UIM_USES_TASK_HANDLE
09/25/14   kr      Compilation error removal
07/07/14   NR      Use v2.0 diag macros
05/01/14   NR      FR 19033: Reduce Code Duplication pbm
04/04/14   NR      QTF CRM MOB Changes
03/17/14   kb      Add offtarget support to check mutex is owned or not
01/12/14   NR      TMC header deprecation changes
12/19/13   NR      PBM: LLVM compiler warnings
03/18/10   krishna Used pbmtask_i.h instead if pbmtask.h as part of CMI activity
07/10/09  ashwanik Support for CSIM and Dual Sim Features
04/01/09   kcp     Fixed CR 169091 - Terminal does not read PB files
                   during power up. Only the EF-PBR is read.
08/28/08   sg      Featurize MMGSDI seek/search calls based on FEATURE_MMGSDI_ENHANCED_SEARCH
07/10/08   sg      Handle the PBM_GSDI_SEEK_CB commands from GSDI
05/08/08   clm     Queue up pbm notify requests to avoid calling them while in critical sections of pbm.
01/28/08   clm     Remove posibility of entering infinite loop, if gargabe collect fails
                   to clean up properly when EFS is FULL.
01/17/08   clm     Add FEATURE_PBM_USIM_SUPPORT to featurize USIM features.
11/02/07   clm     Add refresh okay and init complete support to pbm for mmgsdi.
10/10/06   cvs     Move messages to PBM's diag msg ID
05/03/06   cvs     convert to MMGSDI APIs.  Lint cleanup.
02/23/06   cvs     rework memory allocation macros
01/09/06   cvs     Lint fixes
11/16/05   cvs     Lint fixes
08/22/05   AT      Moved contents inside of a FEATURE_PBM_TASK.
08/12/05   AT      Added UCS2 support for strings.
05/26/05   cvs     support for API changes to make APIs more multiprocessor friendly
03/08/05   PA      Support for PBM notifications
02/08/05   PA      Support for read lock
01/18/05   PA      Fixed compilation issues in non-MMGSDI builds
01/12/05   PA      PBM USIM Support
01/10/05   PA      Kick dog before NV writes.
12/08/04   PA      Kick dog before init to be safe.
11/19/04   PA      Support non-GSDI builds
11/18/04   PA      Support for write lock.
09/29/04   PA      Initial Revision
===========================================================================*/
#include "uim_variation.h"
#include "customer.h"
#include "pbm.h"
#include "pbmtask_i.h"
#include "pbmtask.h"
#include "pbmefs.h"
#include "pbmutils.h"
#include "rcinit.h"
#include "pbmgsdi.h"
#include "pbmuim.h"
#include "uim_msg.h"
#include "pbm_conversion.h"
#include "pbm_nv_refresh.h"
#include "mcs_wrappers.h"
#ifdef FEATURE_UIM_TEST_FRAMEWORK
#error code not present
#endif /* FEATURE_UIM_TEST_FRAMEWORK */
#include "stringl/stringl.h"

rex_crit_sect_type pbm_crit_sect;

static dog_report_type    pbm_reg_dog_rpt_var  = 0;
/* this is used during registration with dog */

q_type pbm_client_cmd_q;       /* Queue to manage client commands */

q_type pbm_gsdi_resp_q;        /* Queue to manage GSDI responses */

q_type pbm_notify_evt_q;       /* Queue to manage notify events */

/*===========================================================================
FUNCTION PBM_KICK_DOG

DESCRIPTION
  Kick the watchdog and set the watchdog timer.

DEPENDENCIES
  None.

RETURN VALUE

SIDE EFFECTS

===========================================================================*/
void pbm_kick_dog(void)
{
   dog_hb_report( pbm_reg_dog_rpt_var );
} /* pbm_kick_dog() */



/*===========================================================================

FUNCTION PBM_CMD_ALLOC

DESCRIPTION
  Allocate a PBM command buffer. The size of command is provided as input.

DEPENDENCIES
  Heap must have been initialized.

RETURN VALUE
  A pointer to the allocated command buffer.
  If Heap is empty a NULL is returned.

SIDE EFFECTS
==========================================================================*/
void *pbm_cmd_alloc(uint16 size)
{
   pbm_cmd_s_type *cmd = NULL;

   if (size < sizeof(pbm_cmd_s_type))
   {
      UIM_MSG_ERR_1("Invalid size for a PBM command %d", size);
      return NULL;
   }

   PBM_MEM_ALLOC(cmd, size);
   PBM_CHECK_PTR_RET(cmd, NULL);

   memset(cmd, 0, size);

   cmd->size = size;

   return(void *)cmd;
}


/*===========================================================================
FUNCTION PBM_CMD_FREE

DESCRIPTION
  Deallocate a PBM command buffer.

DEPENDENCIES
  Heap must have already been initialized.

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
void pbm_cmd_free(void *ptr)
{
   PBM_MEM_FREEIF(ptr);
}


/*===========================================================================
FUNCTION PBM_CMD_Q_PUT

DESCRIPTION
  Add an item to the specified queue.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS

===========================================================================*/
static void pbm_cmd_q_put( q_type *q_ptr, pbm_cmd_s_type *cmd_ptr)
{
   PBM_CHECK_PTR2_RET(q_ptr, cmd_ptr, VOID);

   (void) q_link(cmd_ptr, &cmd_ptr->link);
   q_put(q_ptr, &cmd_ptr->link);
}


/*===========================================================================
FUNCTION PBM_CMD_Q_GET

DESCRIPTION
  Obtain the first item from the specified queue

DEPENDENCIES
  None

RETURN VALUE
  NULL if queue is empty
  pbm_cmd_s_type pointer if successful

SIDE EFFECTS

===========================================================================*/
pbm_cmd_s_type * pbm_cmd_q_get(q_type *q_ptr)
{
  PBM_CHECK_PTR_RET(q_ptr, NULL);
  if(q_cnt(q_ptr) != 0)
  {
    return(pbm_cmd_s_type *) q_get(q_ptr);
  }
  else
  {
    return NULL;
  }
} /* pbm_cmd_q_get */


/*===========================================================================
FUNCTION PBM_CLIENT_CMD_Q_PUT

DESCRIPTION
  Add an item to the client command queue and send a signal.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS

===========================================================================*/
void pbm_client_cmd_q_put(pbm_cmd_s_type *cmd_ptr)
{
   PBM_CHECK_PTR_RET(cmd_ptr, VOID);

   pbm_cmd_q_put(&pbm_client_cmd_q, cmd_ptr);
   (void) rex_set_sigs(PBM_TCB, PBM_CLIENT_CMD_SIG);
}


/*===========================================================================
FUNCTION PBM_GSDI_RESP_Q_PUT

DESCRIPTION
  Add an item to the GSDI response queue and send a signal.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS

===========================================================================*/
void pbm_mmgsdi_resp_q_put(pbm_cmd_s_type *cmd_ptr)
{
   PBM_CHECK_PTR_RET(cmd_ptr, VOID);

   pbm_cmd_q_put(&pbm_gsdi_resp_q, cmd_ptr);
   (void) rex_set_sigs(PBM_TCB, PBM_GSDI_RESPONSE_SIG);
}


/*===========================================================================
FUNCTION PBM_NOTIFY_EVT_Q_PUT

DESCRIPTION
  Add an item to the NOTIFY event queue and send a signal.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS

===========================================================================*/
void pbm_notify_evt_q_put(pbm_cmd_s_type *cmd_ptr)
{
   PBM_CHECK_PTR_RET(cmd_ptr, VOID);

   pbm_cmd_q_put(&pbm_notify_evt_q, cmd_ptr);
   (void) rex_set_sigs(PBM_TCB, PBM_NOTIFY_EVT_SIG);
}


/*===========================================================================
FUNCTION PBM_NOTIFY_EVT_HANDLE

DESCRIPTION
  Read the Notify events from the queue and handle it.

DEPENDENCIES
  None

RETURN VALUE

SIDE EFFECTS

===========================================================================*/
static void pbm_notify_evt_handle(void)
{
   pbm_cmd_s_type        *cmd_ptr = NULL;
   pbm_notify_evt_s_type *notify_evt = NULL;

   while ((cmd_ptr = pbm_cmd_q_get(&pbm_notify_evt_q)) != NULL)
   {
      if (PBM_CMD_NOTIFY_EVT == cmd_ptr->cmd_type)
      {
         /* PBM Notify Event */
         PBM_CMD_CHECK_SIZE(cmd_ptr, pbm_notify_evt_s_type, break);
         notify_evt = (pbm_notify_evt_s_type*)cmd_ptr;
         pbm_notify(&(notify_evt->notify_data));
      } else
      {
         UIM_MSG_ERR_1("Unknown command %d", cmd_ptr->cmd_type);
      }
      pbm_cmd_free(cmd_ptr);
   }
}


/*===========================================================================
FUNCTION PBM_INIT_BEFORE_TASK_START

DESCRIPTION
  Initialization of PBM task before the task has been started
  critical sections heap and globals for conversion arrays are initialized.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS

===========================================================================*/
void pbm_init_before_task_start( void )
{
   rex_init_crit_sect(&pbm_crit_sect);


   /* Initialize the Conversion tables. */
   pbmconvert_init_conversion();

   /* Initialize queues */
   (void) q_init(&pbm_client_cmd_q);
   (void) q_init(&pbm_gsdi_resp_q);
   (void) q_init(&pbm_notify_evt_q);
}


/*===========================================================================
FUNCTION PBM_WAIT

DESCRIPTION
  Suspend PBM from executing until at least one of the specified signals
  in the input mask is set.

DEPENDENCIES
  pbm_rpt_timer must have been initialized

RETURN VALUE
  void

SIDE EFFECTS
  None

===========================================================================*/
rex_sigs_type pbm_wait(
  rex_sigs_type mask
)
{
  rex_sigs_type    sigs = 0;

  for (;;)
  {
    /* Wait for specified signal or watchdog timer */
    sigs = rex_wait(mask | PBM_RPT_TIMER_SIG);

    if (sigs & PBM_RPT_TIMER_SIG)
    {
      sigs &= ~PBM_RPT_TIMER_SIG;
      (void) rex_clr_sigs(PBM_TCB, PBM_RPT_TIMER_SIG);
      pbm_kick_dog();
    }

    if (sigs & mask)
    {
      return sigs;
    }

  } /* forever */
} /* pbm_wait */


#ifdef FEATURE_UIM_TEST_FRAMEWORK
#error code not present
#endif /* FEATURE_UIM_TEST_FRAMEWORK */


/*===========================================================================
FUNCTION PBM_TASK

DESCRIPTION
  The main PBM task.

DEPENDENCIES
  REX is running.

RETURN VALUE
  Never exits.

SIDE EFFECTS

===========================================================================*/
void pbm_task(
  dword dummy
)
{
  rex_sigs_type         sigs;
  rex_tcb_type         *temp_pbm_tcb_ptr = rex_self();
  RCEVT_SIGEX_SIGREX    stop_sig;
  RCINIT_GROUP          pbm_group;

  stop_sig.signal = temp_pbm_tcb_ptr;
  stop_sig.mask   = PBM_TASK_STOP_SIG;
  pbm_group       = rcinit_lookup_group_rextask(temp_pbm_tcb_ptr);
  rcinit_register_term_group(pbm_group, RCEVT_SIGEX_TYPE_SIGREX, &stop_sig);

  (void)dummy;
   /*register to dog*/
  pbm_reg_dog_rpt_var = dog_hb_register_rex((rex_sigs_type) PBM_RPT_TIMER_SIG);

  pbm_tcb_ptr = rex_self();

  rcinit_handshake_startup(); // required rcinit handshake

  mcs_rcevt_signal_name("PBM:READY");

  pbm_kick_dog();

  (void)pbm_init();
  for (;;)
  {
    /* Wait forever for rex signals */
    sigs = pbm_wait( PBM_CLIENT_CMD_SIG |
                     PBM_GSDI_RESPONSE_SIG |
                     TASK_OFFLINE_SIG |
                     PBM_TASK_STOP_SIG |
#ifdef FEATURE_UIM_TEST_FRAMEWORK
                     #error code not present
#endif /* FEATURE_UIM_TEST_FRAMEWORK */
                     PBM_NOTIFY_EVT_SIG
                   );

    /* Kick watchdog */
    if (sigs & PBM_RPT_TIMER_SIG)
    {
      (void) rex_clr_sigs(rex_self(), PBM_RPT_TIMER_SIG);
      pbm_kick_dog();
    }

    /* OFFLINE signal */
    if (sigs & TASK_OFFLINE_SIG)
    {
      (void) rex_clr_sigs(rex_self(), TASK_OFFLINE_SIG);
       UIM_MSG_MED_0(" TASK_OFFLINE_SIG received");
    }
    /* GSDI command */
    if (sigs & PBM_GSDI_RESPONSE_SIG)
    {
      (void) rex_clr_sigs(rex_self(), PBM_GSDI_RESPONSE_SIG);
      UIM_MSG_MED_0(" PBM_GSDI_RESPONSE_SIG received");
      pbm_mmgsdi_resp_handle();
    }
    /* Client command */
    if (sigs & PBM_CLIENT_CMD_SIG)
    {
      (void) rex_clr_sigs(rex_self(), PBM_CLIENT_CMD_SIG);
      UIM_MSG_MED_0(" PBM_CLIENT_CMD_SIG received");
      pbm_client_cmd_handle();
    }
    /* Notify event command */
    if (sigs & PBM_NOTIFY_EVT_SIG)
    {
      (void) rex_clr_sigs(rex_self(), PBM_NOTIFY_EVT_SIG);
      UIM_MSG_MED_0(" PBM_NOTIFY_EVT_SIG received");
      pbm_notify_evt_handle();
    }
    /* STOP signal */
    if (sigs & PBM_TASK_STOP_SIG)
    {
      (void) rex_clr_sigs(rex_self(), PBM_TASK_STOP_SIG);
       UIM_MSG_HIGH_0("PBM_TASK_STOP_SIG received" );

      pbm_ecc_store_mcc();
      rcinit_unregister_term_group(pbm_group, RCEVT_SIGEX_TYPE_SIGREX, &stop_sig);
      rcinit_handshake_term();
    }

#ifdef FEATURE_UIM_TEST_FRAMEWORK
     #error code not present
#endif /* FEATURE_UIM_TEST_FRAMEWORK */
  } /* for (;;) */
#ifdef FEATURE_UIM_TEST_FRAMEWORK
  #error code not present
#endif /* FEATURE_UIM_TEST_FRAMEWORK */
}/* pbm_task */
