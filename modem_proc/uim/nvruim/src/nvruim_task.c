/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*


            N V    R - U I M    S U B S Y S T E M    TASK


GENERAL DESCRIPTION

  This source file contains the main NVRUIM task along with supporting functions.

EXTERNALIZED FUNCTIONS


INITIALIZATION AND SEQUENCING REQUIREMENTS


                        COPYRIGHT INFORMATION

Copyright (c) 2013 - 2016 QUALCOMM Technologies, Incorporated and its licensors.
All Rights Reserved.  QUALCOMM Technologies Proprietary.  Export of this technology or software
is regulated by the U.S. Government. Diversion contrary to U.S. law prohibited.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/* <EJECT> */
/*===========================================================================
                        EDIT HISTORY FOR MODULE

$Header: //components/rel/uim.mpss/6.1.0/nvruim/src/nvruim_task.c#2 $ $DateTime: 2019/10/30 06:56:05 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
09/21/16   bcho    Handling of parallel requests in NVRUIM
09/01/16   vdc     Reduce frequent logging in NVRUIM
04/13/16   ar      CDMA/HDR support on 2nd SUB
03/09/16   bcho    Process requests after NVRUIM task has started
10/14/15   vv      Use mcs wrappers for rcevt APIs
10/02/15   kk      Redirect requests to NV during QCN Restore operation
06/29/15   lm      Enable memory leak detection
05/20/15   vdc     Fix compilation warnings
03/17/15   tl      Include rcinit_rex.h to prevent strict compiler errors
03/11/15   tl      Add caching of the module tcb pointer
02/20/15   hh      Clear signals before handshake with RCINIT
02/02/15   ar      Mechanism to provide lock to lower priority task like diag
08/28/14   av      Change nvruim crit sect to mutex-based from pimutex-based
07/15/14   yt      Support for NV refresh
07/14/14   tl      Remove always ON feature FEATURE_MODEM_RCINIT
04/28/14   av      Fix to return appropriate error code if access denied
04/03/14   am      Fix compile error on SBM due to assert_on_crit_sect_owner
04/01/14   av      NVRUIM headers cleanup
02/20/14   av      Support for async processing of NV_RTRE_OP_CONFIG_I
01/28/14   df      Use q_cnt() instead directly accessing cnt
01/10/14   df      Use v2.0 diag macros
11/01/13   tkl     deregister watch dog when break from main task
11/01/13   at      Support for guaranteed response for each NVRUIM request
10/28/13   kb      Add offtarget support to check mutex is owned or not
10/17/13   at      SIM busy support in NVRUIM
10/01/13   vr      Move break statement of TASK_STOP_SIG to
                   FEATURE_MODEM_RCINIT flag
09/23/13   vr      qtf-crm MOB migration
07/02/13   vs      Reworked return values to NV API and clients
05/13/13   vs      Initial version

===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/

#include "rex.h"
#include "task.h"
#include "dog_hb_rex.h"
#include "modem_mem.h"
#include "queue.h"
#include "nvruimi.h"
#include "nvruim_p.h"
#include "nvruim.h"
#include "nvruim_task.h"
#include "nvruim_request.h"
#include "uim_msg.h"
#include "rcevt_rex.h"
#include "rcinit.h"
#include "rcinit_rex.h"
#include "mcs_wrappers.h"
#include "mcfg_fs.h"
#include "fs_diag_access.h"
#include "fs_stdlib.h"
#include <stringl/stringl.h>

#ifdef FEATURE_UIM_TEST_FRAMEWORK
#error code not present
#endif /* FEATURE_UIM_TEST_FRAMEWORK */

/* <EJECT> */
/*===========================================================================

            DEFINITIONS AND DECLARATIONS FOR MODULE

This section contains local definitions for constants, macros, types,
variables and other items needed by this module.

===========================================================================*/

/* Queue to hold commands for NVRUIM */
q_type                     nvruim_task_cmd_q;

/* Global structure to general info for NVRUIM task */
nvruim_generic_data_type  *nvruim_generic_data_ptr  = NULL;

/*  NVRUIM Dog variables */
static dog_report_type     nvruim_dog_rpt_var       = 0;

/* NVRUIM task TCB pointers. Value is initialized when task is being initialized. */
rex_tcb_type             * nvruim_tcb_ptr           = NULL;

static const char        * nvruim_diag_access_list[] = { NVRUIM_EFS_DIR };

/*===========================================================================
   FUNCTION:      NVRUIM_CMD

   DESCRIPTION:
     This function allows putting of command into the NVRUIM command queue

   DEPENDENCIES:
     None

   LIMITATIONS:
     Parameter check for specific commands are not performed

   RETURN VALUE:
     nv_stat_enum_type

   SIDE EFFECTS:
     None
===========================================================================*/
nv_stat_enum_type nvruim_cmd
(
  nvruim_req_type *nvruim_req_ptr
)
{
  nv_items_enum_type item = NV_ITEMS_ENUM_MAX;

  if(nvruim_req_ptr == NULL)
  {
    return NV_BADPARM_S;
  }

  if (nvruim_req_ptr->nvruim_req == NVRUIM_NV_ACCESS)
  {
    if (nvruim_req_ptr->cmd.nvruim_access_req.nv_cmd_ptr == NULL)
    {
      return NV_BADPARM_S;
    }
    item = nvruim_req_ptr->cmd.nvruim_access_req.nv_cmd_ptr->item;
  }

  /* Do not queue command if task has been stopped */
  if(nvruim_generic_data_ptr != NULL && nvruim_generic_data_ptr->task_state == NVRUIM_TASK_ENDED)
  {
    UIM_MSG_ERR_2("NVRUIM Task not init or stopped, cannot queue req: 0x%x for 0x%x",
                  nvruim_req_ptr->nvruim_req, item);
    return NV_FAIL_S;
  }

  if (q_cnt(&nvruim_task_cmd_q) >= NVRUIM_CMD_QUEUE_MAX_SIZE)
  {
    UIM_MSG_ERR_2("Maximum number of commands have been put onto cmd_q. Failed for req: 0x%x, item 0x%x",
                  nvruim_req_ptr->nvruim_req, item);
    return NV_FAIL_S;
  }

  (void)q_link(nvruim_req_ptr, &nvruim_req_ptr->link);

  /* Put the message on the queue */
  q_put(&nvruim_task_cmd_q, &nvruim_req_ptr->link);

  /* Set the command queue signal */
  (void) rex_set_sigs(UIM_NVRUIM_TCB, NVRUIM_TASK_CMD_Q_SIG);
  UIM_MSG_HIGH_2("Successfully queued req: 0x%x, item 0x%x",
                 nvruim_req_ptr->nvruim_req, item);
  return NV_DONE_S;
} /* nvruim_cmd */


/*===========================================================================
FUNCTION NVRUIM_WAIT

DESCRIPTION
  Main wait routine for the NVRUIM task.

DEPENDENCIES
  None

RETURN VALUE
  A rex_sigs_type value indicating all signals set (the current state
  of all signals). The calling unit can then check for the reason for
  exiting the wait.

SIDE EFFECTS
  None
===========================================================================*/
rex_sigs_type nvruim_wait
(
  rex_sigs_type requested_mask
)
{
  rex_sigs_type rex_signals_mask = 0x00;  /* Mask of signals returned by REX */

  if (rex_self() != UIM_NVRUIM_TCB)
  {
    return rex_signals_mask;
  }

  /* Stop listening to signals corresponding to the NVRUIM command
     queue if NVRUIM task has been stopped */
  if(nvruim_generic_data_ptr != NULL)
  {
    if(nvruim_generic_data_ptr->task_state == NVRUIM_TASK_ENDED)
    {
      requested_mask &= ~NVRUIM_TASK_CMD_Q_SIG ;
    }
  }

  /* --------------------------------------------------------------------
  Loop until the current signals from REX match with one or more of
  the requested signals.
  -------------------------------------------------------------------- */
  do
  {
    (void) rex_clr_sigs( rex_self(), NVRUIM_RPT_TIMER_SIG );
    dog_hb_report(nvruim_dog_rpt_var);

    /* ---------------------------------------------------------------
    Perform the real wait to wait for a command...or to force a
     context switch to allow higher priority tasks to run.
    ---------------------------------------------------------------- */
    rex_signals_mask = rex_wait(requested_mask | NVRUIM_RPT_TIMER_SIG);

  /* do while mask requested does not match current mask */
  } while ((rex_signals_mask & requested_mask) == 0);


  return (rex_signals_mask);
} /* nvruim_wait() */


/*===========================================================================
FUNCTION NVRUIM_HANDLE_SIM_BUSY_SIG

DESCRIPTION
  This function, called in nvruim_task, processes the SIM busy signal.

DEPENDENCIES
  mmgsdi_task must be finished initialization.

LIMITATIONS
  NONE

RETURN VALUE
  void

SIDE EFFECTS
  NONE
===========================================================================*/
static void nvruim_handle_sim_busy_sig(rex_sigs_type sigs)
{
  nvruim_nv_context_type   current_context = NVRUIM_NV_CONTEXT_INVALID;

  NVRUIM_RETURN_IF_NULL(nvruim_generic_data_ptr);

  /* Clear signal */
  (void) rex_clr_sigs(rex_self(), sigs);

  if(sigs == NVRUIM_SIM_BUSY_PRIMARY_SIG)
  {
    current_context = NVRUIM_NV_CONTEXT_PRIMARY;
  }
  else if(sigs == NVRUIM_SIM_BUSY_SECONDARY_SIG)
  {
    current_context = NVRUIM_NV_CONTEXT_SECONDARY;
  }
  else if(sigs == NVRUIM_SIM_BUSY_TERTIARY_SIG)
  {
    current_context = NVRUIM_NV_CONTEXT_TERTIARY;
  }
  else
  {
    return;
  }

  UIM_MSG_HIGH_1("NVRUIM: GOT SIM_BUSY_SIG for context 0x%x", current_context);

  if(nvruim_generic_data_ptr->current_commands_in_context[current_context] > 0)
  {
    /* Also tell MMGSDI to cancel any pending requests */
    nvruim_send_mmgsdi_cancel(current_context);
  }
} /* nvruim_handle_sim_busy_sig */


/*===========================================================================
FUNCTION NVRUIM_WAIT_FOR_MMGSDI_ACCESS

DESCRIPTION
  Wait routine called when MMGSDI access is in progress

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void nvruim_wait_for_mmgsdi_access (
  void
)
{
  rex_sigs_type rex_signals_mask = 0x00;  /* Mask of signals returned by REX */

  /* --------------------------------------------------------------------
  Loop until the current signals from REX match with one or more of
  the signals of interest
  -------------------------------------------------------------------- */
  do
  {
    (void) rex_clr_sigs( rex_self(), NVRUIM_RPT_TIMER_SIG );
    dog_hb_report(nvruim_dog_rpt_var);

    rex_signals_mask = rex_wait(NVRUIM_MMGSDI_ACCESS_SIG      |
                                NVRUIM_RPT_TIMER_SIG          |
                                NVRUIM_SIM_BUSY_PRIMARY_SIG   |
                                NVRUIM_SIM_BUSY_SECONDARY_SIG |
                                NVRUIM_SIM_BUSY_TERTIARY_SIG);

    /* If there was a SIM_BUSY event while waiting for the MMGSDI response,
       we need to cancel that request and expect an error callback */
    if (rex_signals_mask & NVRUIM_SIM_BUSY_PRIMARY_SIG)
    {
      nvruim_handle_sim_busy_sig(NVRUIM_SIM_BUSY_PRIMARY_SIG);
    }
    else if(rex_signals_mask & NVRUIM_SIM_BUSY_SECONDARY_SIG)
    {
      nvruim_handle_sim_busy_sig(NVRUIM_SIM_BUSY_SECONDARY_SIG);
    }
    else if(rex_signals_mask & NVRUIM_SIM_BUSY_TERTIARY_SIG)
    {
      nvruim_handle_sim_busy_sig(NVRUIM_SIM_BUSY_TERTIARY_SIG);
    }

  /* do while mask requested does not match current mask */
  } while ((rex_signals_mask & NVRUIM_MMGSDI_ACCESS_SIG) == 0);
} /* nvruim_wait_for_mmgsdi_access */


/*===========================================================================
FUNCTION NVRUIM_HANDLE_OFFLINE_SIG

DESCRIPTION
  This function, called in nvruim_task, is called to process an offline signal.

DEPENDENCIES
  mmgsdi_task must be finished initialization.

LIMITATIONS
  NONE

RETURN VALUE
  void

SIDE EFFECTS
  NONE
===========================================================================*/
static void nvruim_handle_offline_sig(void)
{
  UIM_MSG_HIGH_0("NVRUIM: GOT TASK_OFFLINE_SIG");

  /* Clear signal */
  (void) rex_clr_sigs(rex_self(), TASK_OFFLINE_SIG);
} /* nvruim_handle_offline_sig */


/*===========================================================================
FUNCTION NVRUIM_HANDLE_STOP_SIG

DESCRIPTION
  This function, called in nvruim_task, is called to process an stop signal.

DEPENDENCIES
  mmgsdi_task must be finished initialization.

LIMITATIONS
  NONE

RETURN VALUE
  void

SIDE EFFECTS
  NONE
===========================================================================*/
static void nvruim_handle_stop_sig(void)
{
  UIM_MSG_HIGH_0("NVRUIM: GOT TASK_STOP_SIG");

   /* Clear signal */
  (void) rex_clr_sigs(rex_self(), TASK_STOP_SIG);

  /* Don't process items in Queue */
  /* Sort of flushes the Queue    */
  if(nvruim_generic_data_ptr != NULL)
  {
    nvruim_generic_data_ptr->task_state = NVRUIM_TASK_ENDED;
  }
} /* nvruim_handle_stop_sig */


/*===========================================================================
FUNCTION NVRUIM_HANDLE_QUEUE_SIG

DESCRIPTION
  This function, called in nvruim_task, is called to process a signals of
  command queue. This function is called only for the processing ASYNC
  NV requests.

DEPENDENCIES
  nvruim_task must be finished initialization.

LIMITATIONS
  NONE

RETURN VALUE
  void

SIDE EFFECTS
===========================================================================*/
static void nvruim_handle_queue_sig(rex_sigs_type sigs_rcvd)
{
  nvruim_req_type              *nvruim_req_ptr                    = NULL;

  if(sigs_rcvd & NVRUIM_TASK_CMD_Q_SIG)
  {
    /* Process ASYNC NVRUIM Command */
    nvruim_req_ptr = (nvruim_req_type*)(q_get(&nvruim_task_cmd_q));

    /* If there is a command on the queues */
    if(nvruim_req_ptr != NULL)
    {
      /* If the req is SWITCH, then switch to the requested session */
      if (nvruim_req_ptr->nvruim_req == NVRUIM_SWITCH_SESSION)
      {
        nvruim_process_switch_1x_session(nvruim_req_ptr->nvruim_req,
                                         nvruim_req_ptr->cmd.nvruim_session);
      }
      else if (nvruim_req_ptr->nvruim_req == NVRUIM_NV_ACCESS)
      {
        nv_rtre_control_type          dummy_rtre_control_value;
        nv_rtre_polling_control_type  dummy_rtre_polling_control_value;
        nv_cmd_type                  *nv_cmd_ptr    = NULL;
        nvruim_nv_context_type        context       = NVRUIM_NV_CONTEXT_INVALID;

        nv_cmd_ptr = nvruim_req_ptr->cmd.nvruim_access_req.nv_cmd_ptr;
        context    = nvruim_req_ptr->cmd.nvruim_access_req.context;

        if (nv_cmd_ptr != NULL)
        {
          switch (nv_cmd_ptr->cmd)
          {
            case NV_RTRE_OP_CONFIG_F:
              (void)nvruim_process_rtre_configuration(nv_cmd_ptr,
                                                      &dummy_rtre_control_value,
                                                      &dummy_rtre_polling_control_value,
                                                      context);
              break;
            case NV_READ_F:
            case NV_WRITE_F:
                  (void)nvruim_process_nv_access(nv_cmd_ptr, context);
              break;
            default:
              UIM_MSG_ERR_1("NVRUIM does not support 0x%x cmd type", nv_cmd_ptr->cmd);
              nv_cmd_ptr->status = NV_BADPARM_S;
              break;
          }

          if (nv_cmd_ptr->done_q_ptr)
          {
            /* Put the data on done q provided by async client */
            (void)q_link(nv_cmd_ptr, &nv_cmd_ptr->link);
            q_put(nv_cmd_ptr->done_q_ptr, &nv_cmd_ptr->link);
          }

          if (nv_cmd_ptr->tcb_ptr)
          {
            /* Let the client know the NV command/request completed */
            (void) rex_set_sigs(nv_cmd_ptr->tcb_ptr, nv_cmd_ptr->sigs);
          }
        }
      }
      /* Since we do not own/allocate nv_cmd_ptr, DO NOT TRY TO FREE
       it here */
    }

    /* Free nvruim_req_ptr as we allocated it internally during queuing req */
    nvruim_mem_free_null_ok(nvruim_req_ptr);

    (void) rex_clr_sigs(UIM_NVRUIM_TCB, NVRUIM_TASK_CMD_Q_SIG);

    /* Set the signal if there are one or more commands to look at */
    if (q_cnt(&nvruim_task_cmd_q) > 0)
    {
      /*  Items on the queue. Set the signal */
      (void) rex_set_sigs(UIM_NVRUIM_TCB, NVRUIM_TASK_CMD_Q_SIG);
    }
  } /* end if NVRUIM_TASK_CMD_Q_SIG */
} /* nvruim_handle_queue_sig */


/*===========================================================================
FUNCTION NVRUIM_INIT_POST_STARTUP

DESCRIPTION
  Function called to define timers, dog registrations and other initialization
  that requires NVRUIM task to be up as a prerequisite.

DEPENDENCIES
  None

RETURN VALUE
  Void

SIDE EFFECTS
  None.  Function should only be called once during task initialization.
===========================================================================*/
static void nvruim_init_post_startup(
  void
)
{
  /* Set NVRUIM task as ready */
  if(nvruim_generic_data_ptr != NULL)
  {
    nvruim_generic_data_ptr->task_state = NVRUIM_TASK_ACTIVE;
  }

  /* --------------------------------------------------------
      Register for dog heartbeat mechanism
  -------------------------------------------------------- */
  nvruim_dog_rpt_var =
    dog_hb_register_rex((rex_sigs_type) NVRUIM_RPT_TIMER_SIG);
} /* nvruim_init_post_startup */


/*===========================================================================
FUNCTION NVRUIM_INIT

DESCRIPTION
  Function called to initialize NVRUIM queues and global variables.

DEPENDENCIES
  None

RETURN VALUE
  Void

SIDE EFFECTS
  None.  Function should only be called once during task initialization.
  On targets with RCInit enabled in place of TMC, this function is called
  in RCInit context. Therefore, it should not include references to TCBs, NV
  operations, etc.
===========================================================================*/
void nvruim_init (
  void
)
{
  uint8 index = 0;

  /* If for whatever reasons this init funtion is called more than
     once, we put this safeguard in place to avoid initializing
     multiple times */
  if(nvruim_generic_data_ptr != NULL)
  {
    return;
  }
  /* ---------------------------------------------------------
      INITIALIZE ALL QUEUES
  --------------------------------------------------------- */
  (void) q_init(&nvruim_task_cmd_q);

  nvruim_tcb_ptr = rcinit_lookup_rextask("nvruim");

  /* --------------------------------------------------------
      INITIALIZE GLOBALS
  -------------------------------------------------------- */
  nvruim_generic_data_ptr =
    modem_mem_calloc(1, sizeof(nvruim_generic_data_type), MODEM_MEM_CLIENT_UIM);
  if (nvruim_generic_data_ptr == NULL)
  {
    UIM_MSG_ERR_0("NVRUIM_INIT: Error allocating memory; not registering with NV");
    return;
  }

  nvruim_generic_data_ptr->task_state                        = NVRUIM_TASK_INACTIVE;
  nvruim_generic_data_ptr->nvruim_active_session           = NVRUIM_1X_PROV_PRI_SESSION;

  memset(nvruim_generic_data_ptr->current_commands_in_context,
         0x00,
         sizeof(nvruim_generic_data_ptr->current_commands_in_context));

  for (index = 0; index < NVRUIM_NUM_1X_SESSION_MAX; index++)
  {
    nvruim_generic_data_ptr->rtre_config_init[index]           = FALSE;
    nvruim_generic_data_ptr->rtre_control_value[index]         = NV_RTRE_CONTROL_NO_RUIM;
    nvruim_generic_data_ptr->rtre_polling_control_value[index] = NV_RTRE_POLLING_CONTROL_NO_POLL;
    nvruim_session_data_ptr[index] = 
      modem_mem_calloc(1, sizeof(nvruim_generic_data_type), MODEM_MEM_CLIENT_UIM);
    if (nvruim_session_data_ptr[index] != NULL)
    {
      nvruim_session_data_ptr[index]->ext_support_3gpd               = FALSE;
      nvruim_session_data_ptr[index]->esn_usage                      = NV_RUIM_USE_ESN;
      nvruim_session_data_ptr[index]->bcsms_svc                      = FALSE;
      nvruim_session_data_ptr[index]->smscap_svc                     = FALSE;
      nvruim_session_data_ptr[index]->control_3gpd                   = NVRUIM_3GPD_MIP_NV_SIP_NV;
      nvruim_session_data_ptr[index]->non_3gpd_cdma_card             = NVRUIM_NO_CARD;
      nvruim_session_data_ptr[index]->uim_instr_prot                 = MMGSDI_NO_PROTOCOL;
      nvruim_session_data_ptr[index]->session_id                     = 0;
      nvruim_session_data_ptr[index]->mmgsdi_session_active          = FALSE;
      nvruim_session_data_ptr[index]->mmgsdi_slot_id                 = MMGSDI_MAX_SLOT_ID_ENUM;
      nvruim_session_data_ptr[index]->hrpd_cdma_svc                  = FALSE;
      nvruim_session_data_ptr[index]->hrpd_control                   = NVRUIM_AN_HRPD_NO_SUPPORT;
      nvruim_session_data_ptr[index]->hrpd_allow_to_fallback_to_cave = FALSE;
      nvruim_session_data_ptr[index]->hrpd_disabled_card_flag        = FALSE;
      nvruim_session_data_ptr[index]->uim_use_cave_default           = FALSE;
      nvruim_session_data_ptr[index]->mip_svc_3gpd                   = FALSE;
      nvruim_session_data_ptr[index]->sip_svc_3gpd                   = FALSE;
      nvruim_session_data_ptr[index]->nvruim_lbs_support             = FALSE;
    }
  }

  qurt_mutex_init(&(nvruim_generic_data_ptr->crit_sect));
  rex_init_crit_sect(&(nvruim_generic_data_ptr->timer_crit_sect));

  nv_ruim_register(nvruim_process_nv_req);
} /* nvruim_init */


/*===========================================================================
FUNCTION NVRUIM_TASK_ENDED

DESCRIPTION
  Function called to cleanup when task is ended.

DEPENDENCIES
  None

RETURN VALUE
  Void

SIDE EFFECTS
  None.  Function should only be called when task ends and break from the
  task loop.
===========================================================================*/
static void nvruim_task_ended (
  void
)
{
  /* Deregister dog if leaving the task so that we don't ERR_FATAL */
  dog_hb_deregister(nvruim_dog_rpt_var);
} /* nvruim_task_ended */


/*===========================================================================

FUNCTION nvruim_diag_access_check_cb

DESCRIPTION
  Diag EFS callback function to monitor updates under /nvruim efs location.
  The diag access is to only monitor the updates in this folder, this
  callback function should always return TRUE.

RETURN VALUE
  Boolean

DEPENDENCIES/SIDE EFFECTS/NOTES
  None

===========================================================================*/
static boolean nvruim_diag_access_check_cb
(
  char      * file_name_ptr,
  uint32      op_type
)
{
  uint16                       filename_length = 0;
  char                       * path_buf_ptr    = NULL;
  char                       * input_path_ptr  = NULL;

  if (file_name_ptr == NULL ||
      nvruim_generic_data_ptr == NULL)
  {
    /* Not sure what the file is, so allow this diag request */
    return TRUE;
  }

  if ( op_type !=  DIAG_FS_CHECK_WRITE  &&
       op_type !=  DIAG_FS_CHECK_DELETE &&
       op_type !=  DIAG_FS_CHECK_UNLINK)
  {
    return TRUE;
  }


  path_buf_ptr = (char*)nvruim_mem_malloc(FS_DIAG_ACCESS_PATH_MAX+1);
  if (path_buf_ptr == NULL)
  {
    return TRUE;
  }

  /* efs_realpath() will return NULL upon error but an error can mean a number
     of things, for example, if it is not able to find the file path specified
     in the efs which is not really an error in this case. Even in case of error
     however, the second parameter will return the parsed file path or at least
     the best it was able to do until it ran into an error. For our purposes we
     can ignore the return value because the subsequent comparisons will fail
     anyways if the path was not parsed correctly. */
  (void)efs_realpath (file_name_ptr, path_buf_ptr);
  filename_length = (uint16)strnlen(path_buf_ptr, FS_DIAG_ACCESS_PATH_MAX);

  input_path_ptr = (char*)nvruim_mem_malloc(filename_length+1);
  if (input_path_ptr == NULL)
  {
    nvruim_mem_free_null_ok(path_buf_ptr);
    return TRUE;
  }

  (void)memscpy(input_path_ptr,
                filename_length+1,
                path_buf_ptr,
                filename_length+1);

  memset(path_buf_ptr, 0x00, (FS_DIAG_ACCESS_PATH_MAX+1));
  (void)efs_realpath (NVRUIM_QCN_RESTORE_IN_PROGRESS_EFS, path_buf_ptr);
  
  if (filename_length == (uint16)strnlen(path_buf_ptr, FS_DIAG_ACCESS_PATH_MAX) &&
      (memcmp((void*)path_buf_ptr,
              (void*)input_path_ptr,
              filename_length) == 0))
  {
    /* call back for "/nvruim/qcn_restore_inprogress"  
       reset the status flag to unknown */
    UIM_MSG_LOW_1("nvruim_diag_access_check_cb efs matched op:0x%x",op_type);
    nvruim_generic_data_ptr->nvruim_qcn_restore_status = NV_RUIM_QCN_RESTORE_UNKNOWN;
  }

  nvruim_mem_free_null_ok(path_buf_ptr);
  path_buf_ptr = NULL;
  nvruim_mem_free_null_ok(input_path_ptr);
  input_path_ptr = NULL;

  return TRUE;
}/* nvruim_diag_access_check_cb */


/*===========================================================================
FUNCTION NVRUIM_INIT_QCN_RESTORE_FLAG

DESCRIPTION
  Function called to setup the diag access call back and initialize the 
  qcn restore flag.

DEPENDENCIES
  None

RETURN VALUE
  Void

SIDE EFFECTS
  None.  
===========================================================================*/
static void nvruim_init_qcn_restore_flag (
  void
)
{
  struct fs_stat            file_stat;

  /*
    1. Delete the QCN RESTORE EFS File if present
    2. Create the /nvruim directory if not present
    3. Register for access cb for nvruim directory
    4. set the qcn restore status to not in progress
  */
  (void)mcfg_fs_delete((const char *)NVRUIM_QCN_RESTORE_IN_PROGRESS_EFS, MCFG_FS_TYPE_EFS, MCFG_FS_SUBID_NONE);

  memset(&file_stat, 0x00, sizeof(file_stat));

  /* Create the /nvruim directory if not present */
  if(mcfg_fs_stat((const char *)NVRUIM_EFS_DIR, &file_stat,
       MCFG_FS_TYPE_EFS, MCFG_FS_SUBID_NONE) != MCFG_FS_STATUS_OK)
  {
    (void)mcfg_mkdir((const char *)NVRUIM_EFS_DIR, 0777, MCFG_FS_TYPE_EFS);
  }

  /* Try to setup Diag access check callback */
  diag_fs_register_access_check((const char **)nvruim_diag_access_list,
                                sizeof(nvruim_diag_access_list) / sizeof(nvruim_diag_access_list[0]),
                                nvruim_diag_access_check_cb);

  /* set the default restore status */
  if(nvruim_generic_data_ptr != NULL)
  {
    nvruim_generic_data_ptr->nvruim_qcn_restore_status = NV_RUIM_QCN_RESTORE_NOT_IN_PROGRESS;
  }
}/*nvruim_init_qcn_restore_flag */


/*===========================================================================
FUNCTION NVRUIM_TASK

DESCRIPTION
  Entry point for the NVRUIM task.  This function performs task initialization,
  then sits in an infinite loop, waiting on an input queue, and responding
  to messages received.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void nvruim_task
(
  dword dummy
)
{
  rex_sigs_type             sigs =  NVRUIM_TASK_CMD_Q_SIG         |
                                    NVRUIM_SIM_BUSY_PRIMARY_SIG   |
                                    NVRUIM_SIM_BUSY_SECONDARY_SIG |
                                    NVRUIM_SIM_BUSY_TERTIARY_SIG  |
                                    TASK_STOP_SIG                 |
                                    TASK_OFFLINE_SIG;
  rex_sigs_type             sigs_rcvd = 0x00;
  RCEVT_SIGEX_SIGREX        stop_sig;
  RCINIT_GROUP              nvruim_group;

  (void)dummy;

  /* --------------------------------------------------------
      Clear all signals
  -------------------------------------------------------- */
  (void) rex_clr_sigs(UIM_NVRUIM_TCB, (rex_sigs_type)~0);

  stop_sig.signal = UIM_NVRUIM_TCB;
  stop_sig.mask   = TASK_STOP_SIG;

  nvruim_group = rcinit_lookup_group_rextask(UIM_NVRUIM_TCB);
  rcinit_register_term_group(nvruim_group, RCEVT_SIGEX_TYPE_SIGREX, &stop_sig);

  /* Handshake with RCInit framework */
  rcinit_handshake_startup();

  mcs_rcevt_signal_name("NVRUIM:READY");

  nvruim_init_post_startup();

  /* setup the diag callback and qcn restore flag status */
  nvruim_init_qcn_restore_flag();

  /* ------------------------------------------------------------------------
  ** Main Task Loop
  ** ----------------------------------------------------------------------*/
  for (;;)
  {
    /*-----------------------------------------------------------------------------
                            Wait on signal
    -----------------------------------------------------------------------------*/
    sigs_rcvd = nvruim_wait( sigs );

    /*-----------------------------------------------------------------------------
                          Process Signal - TASK_OFFLINE_SIG
    -----------------------------------------------------------------------------*/

    if (sigs_rcvd & TASK_OFFLINE_SIG)
    {
      nvruim_handle_offline_sig();
    } /* end if TASK_OFFLINE_SIG */

    /*-----------------------------------------------------------------------------
                          Process Signal - TASK_STOP_SIG
    -----------------------------------------------------------------------------*/
    else if (sigs_rcvd & TASK_STOP_SIG)
    {
      nvruim_handle_stop_sig();
      rcinit_unregister_term_group(nvruim_group, RCEVT_SIGEX_TYPE_SIGREX, &stop_sig);
      rcinit_handshake_term();
      break;
    } /* end if TASK_STOP_SIG */

    /*-----------------------------------------------------------------------------
            Process Signal - NVRUIM_TASK_CMD_Q_SIG
    -----------------------------------------------------------------------------*/
    else if (sigs_rcvd & sigs & NVRUIM_TASK_CMD_Q_SIG)
    {
      /* Do not handle command if NVRUIM task has been stopped */
      if(nvruim_generic_data_ptr->task_state == NVRUIM_TASK_ACTIVE)
      {
        nvruim_handle_queue_sig(sigs_rcvd);
      }
    } /* end if NVRUIM_TASK_CMD_Q_SIG */
    /*-----------------------------------------------------------------------------
            Process Signal - NVRUIM_SIM_BUSY_SIG
    -----------------------------------------------------------------------------*/
    else if (sigs_rcvd & NVRUIM_SIM_BUSY_PRIMARY_SIG)
    {
      nvruim_handle_sim_busy_sig(NVRUIM_SIM_BUSY_PRIMARY_SIG);
    } /* end if NVRUIM_SIM_BUSY_PRIMARY_SIG */
    else if(sigs_rcvd & NVRUIM_SIM_BUSY_SECONDARY_SIG)
    {
      nvruim_handle_sim_busy_sig(NVRUIM_SIM_BUSY_SECONDARY_SIG);
    }/* end if NVRUIM_SIM_BUSY_SECONDARY_SIG */
    else if(sigs_rcvd & NVRUIM_SIM_BUSY_TERTIARY_SIG)
    {
      nvruim_handle_sim_busy_sig(NVRUIM_SIM_BUSY_TERTIARY_SIG);
    }/* end if NVRUIM_SIM_BUSY_TERTIARY_SIG */
    else
    {
      UIM_MSG_ERR_1("NVRUIM TASK INVALID SIGS: %x", sigs_rcvd);

      /* Reset the Sigs by Masking out Good Sigs and
      ** Clearing out Bad SIGS
      */
      (void)rex_clr_sigs(UIM_NVRUIM_TCB, (sigs_rcvd & ~sigs) );
    } /* end else - Invalid signal in NVRUIM task */
  } /* end for(;;) */

#ifdef FEATURE_UIM_TEST_FRAMEWORK
  #error code not present
#endif /* FEATURE_UIM_TEST_FRAMEWORK */

  nvruim_task_ended();
} /* nvruim_task */
