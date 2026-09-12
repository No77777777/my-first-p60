/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                        S I G N A L   H A N D L E R

                  U S E R   I D E N T I T Y   M O D U L E

GENERAL DESCRIPTION
  This module contains all of the functions that handle signals to the UIM
  task.  In general there is one signal handling function per signal.

EXTERNALIZED FUNCTIONS

  uim_signal_handler
    Calls the signal handling functions for each UIM task signal that needs to be
    handled.

INITIALIZATION AND SEQUENCING REQUIREMENTS
  uim_signal_handler shall be called after rex_wait returns a mask of signals that
  are set.

Copyright (c) 2001 - 2018, 2020 QUALCOMM Technologies, Inc (QTI) and its licensors. All Rights Reserved.


*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

$Header: //components/rel/uim.mpss/6.1.0/uimdrv/src/uim_sigs.c#4 $
$DateTime: 2020/08/17 07:06:55 $
$Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
08/17/20   rkg     Reset poll timer after recovery for External command
02/11/20   rkg     Correction of F3s reduction logic
09/17/18   ng      Correcting the initiliasation of Static buffer
08/30/18   ng      Do not open channel other than 1 for non spec card
08/30/18   ng      Do not reset poll timer when flaky poll-1 is in progress
07/23/18   ku      Do not start debounce in case of extended recovery
03/12/18   ku      Mask inserted interrupt during power_up
01/10/18   ku      Retry PIN commands in case of command response timeout
12/28/17   gm      Waiting for diag indication before commiting cache
12/21/17   gs      Do not pet the dog after the task power down
11/09/17   ku      Updating the max attempts  of recovery
07/06/17   ku      Poll card for twice when detecting flaky removal interupt
06/16/17   ks      Consider no_switch_ins NV check for card_inserted_f 
05/09/17   gm      Boot-up APDU logging
04/14/17   kv      Added pass-through mode support
03/30/17   gm      Re-start polling after cold reset 
03/13/17   ku      Common path for local and remote card
11/24/16   ks      Adding support for spcom interface
08/16/16   ks      Re-storing the file path after successful recovery
07/25/16   ks      Considering SWITCH_TO_PASIVE_MODE scenarios
07/07/16   ks      Reset UIM parameters during AP ON/OFF
07/05/16   ks      New global to count PPS attempts
07/01/16   gm      Remove done_q_ptr implementation
06/29/16   gm      Stub out Hotswap related functionality
06/23/16   sam     F3 messages optimization
06/10/16   gm      Remove the test code of Simulate Null byte
05/25/16   na      Do not clear voltage class upon receipt of RESET_F
05/09/16   sam     Reject open channel command if it failed during powerup
04/29/16   sam     Report error for current command if the command processing is 
                   stopped in cmd_response_signal_handling
04/20/16   sam     Support for enhanced subscription manager
03/28/16   sam     Support for SAM card
03/21/16   na      Initializing status as UIM_UNINITIALIZED_S
02/22/16   sam     Remove check for channel ID of previous command when  
                   get resposne is received in stream command
01/20/16   sam     Resetting the command transacted flag if the 
                   response is not received from card
01/17/16   sam     Boolean to check if a T=1 block is received from card
01/05/16   na      Fix to support the Dir\Inv convention card on Reset request
12/10/15   ks      Rel12 polling enhancement
11/24/15   ks      Compiler warning fix
11/04/15   ks      Clearing timeout signal before processing response
10/14/15   vv      Use mcs wrappers for rcevt APIs
10/13/15   ks      Allowing streaming APDU to card in passive mode.
09/25/15   sam     Remove the logic to store timeout info in EFS
09/10/15    na     Restricting EFS APDU Logging with Feature
08/25/15   sam     Stop recovery process only if card removal debounce is succesfull
08/11/15   hyo     usb uicc removal
08/03/15   sam     Delay recovery in the case of card removal, 
                   based on card reoval sample count and max retry count
07/20/15   ks      Setting the cmd transacted flag to TRUE incase 6F00 is sent
                   to mmgsdi during GET RESPONSE in STREAM APDU scenario 
07/09/15   ks      Sending err report for the on-going cmd during 
                   task stop signal handling
06/29/15   ks      Allowing streaming GET RESPONSE apdu if there is 
                   no interleaving command
06/25/15   ks      Not clearing the card parameters during UIM_POWER_UP_UIM_PASSIVE_F
06/08/15   ks      Handling BTSAP connection unavailable signal
05/29/15   kv      Migrated to new MCFG Refresh Done API
05/20/15   na      CW error fix
05/14/15   ks      Fix to make hotswap work after passive power up
04/27/15   ssr     Fix to stop recovery process on card removal 
04/15/15   ks      De-Featurizing the FEATURE_UIM_REMOTE/BTSAP_UIM
08/04/15   sam     FR24498: Powerup logging new changes
04/06/15   hyo     Disable polling when RF is inactive
04/02/15   ssr     Support to check the pending proactive command on card
03/17/15   sam     Report 68 81 as SW if a command is recieved on a closed channel
02/24/15   ll      Splitting the slot specific NVs from MCFG refreshed NVs
02/13/15   sam     Decoupling of FEATURE_UIM_T_1_SUPPORT 
01/19/15   ks      Clearing internal_command_used flag appropriately
01/07/14   sam     Modification of msg macros for FR24498-UIM Powerup Logging
01/05/15   ks      UIM handling task stop signal when interface not used
12/12/14   ks      UIM to vote for sleep during AP mode OFF and card removed
11/17/14   nmb     Remove dead directory maintenance code
11/17/14   ssr     Fixed to ensure that UIM is not overwriting the ICC\UICC
                   protocol in Reset request.
11/12/14   js      Fixed compiler warning
11/10/14   akv     Support for user to customize LDO settings
10/31/14   ks      Reset protocol and Voltage for RESET_F command
10/30/14   ks      Fix issue with BTSAP compilation
10/13/14   akv     Using 3.8Mhz clk while toggling INST class during timeouts
10/01/14   ll      KW errors
09/18/14   akv     UIMDRV UT enhancements
08/27/14   sam     Adding support to trigger recovery on reciept of bad status word
08/14/14   ks      Hotswap enhancement for flaky mechanical switches
08/14/14   sam     Support for extended recovery
07/25/14   sam     retry powerup when card timeouts on RESET_F command when
                   Hotswap disabled
07/18/14   ssr     Fix to clear the busy and transaction timer after getting
                   response for 0x88 INS of stream APDU request when NULL
                   simulation feature flag is disabled
07/15/14   ll      NV refresh upon hotswap feature
07/08/14   ssr     Fixed to send Card Removed notification when card is not
                   present on Reset request
06/23/14   ssr     Removed tmc.h header, no longer required  due to RCINIT framework
06/11/14   ll      Add interruptible time delay and return value for update params
05/22/14   sam     Correcting Fix to notify card error when timeout occurs
                   on RESET_F command in the case of Hotswap off scenario.
05/08/14   ks      Initialising the max attempts value for every command
04/23/14   lxu     Need to send response for the command that was de-queued from the
                   queue and was currently in progress before doing iso fallback.
04/16/14   lxu     Add usb init timer for waiting connection available event
04/14/14   lxu     Fix hot-swap failed in slot2 when enable USB UICC for all slots
04/14/14   lxu     Set me_powerup to TRUE before fallback to ISO mode
04/08/14   ssr     Support of CLA change on interface error
03/31/14   ll      Change stop at infinite NULL flag name for readability
03/31/14   lxu     Fix USB UICC recovery, hotswap and iso card issue
03/27/14   lxu     USB UICC modem implement
03/27/14   ll      SIM BUSY log reduction and remove dog rpt print
03/21/14   ll      Remove return_ptr and remove/change uim_free to UIM_FREE
03/18/14   sam     Fix to notify CARD_ERROR if timeout occurs on RESET_F command
03/07/14   ks      Command re-queue count not to be cleared off during
                   time out on INTERNAL FETCH
02/25/14   na      Support of select next api for non-prov session
02/07/14   akv     Recover within recovery for timeouts after ATR and PPS
01/29/14   akv     RX ISR refactoring changes
01/14/14   ak      Created for uim task re-factoring/cleanup

===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "rex.h"
#include "uimdrv_msg.h"
#include "uim_p.h"
#include "uimglobals.h"
#include "uim_p.h"
#include "uimi.h"
#include "uimdrv.h"
#include "uimdrv_hal_iso.h"
#include "uimdrv_clk.h"
#include "uimdrv_main.h"
#include "uimdrv_uartdm.h"
#include "dog_hb_rex.h"
#include "uim_hotswap.h"
#include "uimsub_manager.h"
#include "uim_simulate_null.h"
#include "diag.h"

#include "uim_logging.h"
#include "uim_remote.h"

#include "mcs_wrappers.h"

/* Number of times wait for diag log change mask indication 
   with 0x19B7 log mask enabled */
#define UIM_NUM_DIAG_IND_TO_WAIT        5

/* Function prototype for the table based signal handlers */
typedef boolean (*uim_sig_handler_type)(rex_sigs_type *sig_mask_ptr, rex_sigs_type *imask_ptr, uim_instance_global_type *uim_ptr);

/* This struct is used to create the table which maps signals to their handler functions */
typedef struct
{
  rex_sigs_type sigMask;
  uim_sig_handler_type handler;
}uim_sig_map_type;

extern void uim_efslog_process_purge_sig(uim_instance_global_type *uim_ptr);
/* Forward declarations */
static boolean uim_handle_dog_sig(rex_sigs_type *sig_mask_ptr, rex_sigs_type *imask_ptr, uim_instance_global_type *uim_ptr);
static boolean uim_handle_task_stop_sig(rex_sigs_type *sig_mask_ptr, rex_sigs_type *imask_ptr, uim_instance_global_type *uim_ptr);
static boolean uim_handle_poll_timer_sig(rex_sigs_type *sig_mask_ptr, rex_sigs_type *imask_ptr, uim_instance_global_type *uim_ptr);
static boolean uim_handle_state_toggle_sig(rex_sigs_type *sig_mask_ptr, rex_sigs_type *imask_ptr, uim_instance_global_type *uim_ptr);
static boolean uim_handle_cmd_q_sig(rex_sigs_type *sig_mask_ptr, rex_sigs_type *imask_ptr, uim_instance_global_type *uim_ptr);
static boolean uim_handle_cmd_rsp_sig(rex_sigs_type *sig_mask_ptr, rex_sigs_type *imask_ptr, uim_instance_global_type *uim_ptr);
static boolean uim_telecom_handle_cmd_q_sig(rex_sigs_type *sig_mask_ptr, rex_sigs_type *imask_ptr, uim_instance_global_type *uim_ptr);
static boolean uim_telecom_handle_cmd_rsp_sig(rex_sigs_type *sig_mask_ptr, rex_sigs_type *imask_ptr, uim_instance_global_type *uim_ptr);
static boolean uim_handle_efslog_purge_sig(rex_sigs_type *sig_mask_ptr, rex_sigs_type *imask_ptr, uim_instance_global_type *uim_ptr);
static boolean uim_handle_cmd_rsp_timeout_sig(rex_sigs_type *sig_mask_ptr, rex_sigs_type *imask_ptr, uim_instance_global_type *uim_ptr);
#ifdef FEATURE_SUPPORTS_NON_REMOVABLE_UICC
#error code not present
#endif
static boolean uim_telecom_handle_cmd_rsp_timeout_sig(rex_sigs_type *sig_mask_ptr, rex_sigs_type *imask_ptr, uim_instance_global_type *uim_ptr);
static boolean uim_handle_simulate_null_timer_exp_sig(rex_sigs_type *sig_mask_ptr, rex_sigs_type *imask_ptr, uim_instance_global_type *uim_ptr);
static boolean uim_handle_busy_ind_timer_exp_sig(rex_sigs_type *sig_mask_ptr, rex_sigs_type *imask_ptr, uim_instance_global_type *uim_ptr);
static boolean uim_handle_trans_timer_exp_sig(rex_sigs_type *sig_mask_ptr, rex_sigs_type *imask_ptr, uim_instance_global_type *uim_ptr);
static boolean uim_handle_mcgf_nv_refresh_sig(rex_sigs_type *sig_mask_ptr, rex_sigs_type *imask_ptr, uim_instance_global_type *uim_ptr);
static boolean uim_handle_extended_recovery_timer_exp_sig(rex_sigs_type *sig_mask_ptr, rex_sigs_type *imask_ptr, uim_instance_global_type *uim_ptr);
static boolean uim_is_streaming_cmd_allowed(uim_instance_global_type *uim_ptr);
static boolean uim_handle_fetch_pending_sig(rex_sigs_type *sig_mask_ptr, rex_sigs_type *imask_ptr, uim_instance_global_type *uim_ptr);
static boolean uim_handle_cm_task_ready_sig(rex_sigs_type *sig_mask_ptr, rex_sigs_type *imask_ptr, uim_instance_global_type *uim_ptr);
static boolean uim_handle_remote_connection_unavailable_sig(rex_sigs_type *sig_mask_ptr, rex_sigs_type *imask_ptr, uim_instance_global_type *uim_ptr);
static boolean uim_handle_diag_log_mask_change_sig(rex_sigs_type *sig_mask_ptr, rex_sigs_type *imask_ptr, uim_instance_global_type *uim_ptr);

/*  This table associates each UIM signal to its appropriate handler function */
static uim_sig_map_type uim_sig_map[] =
{
  {UIM_DOG_HB_RPT_SIG, uim_handle_dog_sig},
  {UIM_TASK_STOP_SIG, uim_handle_task_stop_sig},
  {UIM_RCEVT_DIAG_LOG_MASK_CHANGE_SIG, uim_handle_diag_log_mask_change_sig},  
  {UIM_SWITCH_LOGICAL_SLOT_SIG , uim_handle_switch_logical_slot_sig},
  {UIM_SUSPICIOUS_CARD_REM_SIG,uim_handle_card_removed_suspicious_sig},
  {UIM_POLL_TIMER_SIG, uim_handle_poll_timer_sig},
  {UIM_HOTSWAP_CMD_CARD_REM_SIG, uim_handle_card_removed_sig},
  {UIM_HOTSWAP_CMD_CARD_INS_SIG, uim_handle_card_inserted_sig},
  {UIM_STATE_TOGGLE_SIG, uim_handle_state_toggle_sig},
  {UIM_FETCH_PENDING_SIG, uim_handle_fetch_pending_sig},
  {UIM_REMOTE_CONNECTION_UNAVAILABLE_SIG, uim_handle_remote_connection_unavailable_sig},
  {UIM_CMD_Q_SIG, uim_handle_cmd_q_sig},
  {UIM_CMD_RSP_SIG, uim_handle_cmd_rsp_sig},
  {UIM_EFSLOG_PURGE_SIG, uim_handle_efslog_purge_sig},
  {UIM_CMD_RSP_TIMEOUT_SIG, uim_handle_cmd_rsp_timeout_sig},
  {UIM_TRANSACTION_SIG, uim_handle_cmd_rsp_timeout_sig},
  {UIM_SIMULATE_NULL_TIMER_EXP_SIG, uim_handle_simulate_null_timer_exp_sig},
  {UIM_BUSY_IND_TIMER_EXP_SIG, uim_handle_busy_ind_timer_exp_sig},
  {UIM_TRANS_TIMER_EXP_SIG, uim_handle_trans_timer_exp_sig},
  {UIM_MCGF_NV_REFRESH_SIG, uim_handle_mcgf_nv_refresh_sig},
  {UIM_EXT_RECOVERY_TIMER_EXP_SIG, uim_handle_extended_recovery_timer_exp_sig},
  {UIM_RCEVT_CM_TASK_READY_SIG, uim_handle_cm_task_ready_sig},
};


/**
*  This function handles all the signals that are set for the UIM task by
*  calling the associated handler function for each signal that
*  is set.
*
*  SIDE EFFECTS
*  Can potentially change the value of the imask_ptr(set of
*  signals to that UIM task is interested in) depending on logic
*  in the handlers.
*
*  @param sig_mask_ptr Pointer to the OR'd set of signals to
*                      handle.
*  @param imask_ptr Pointer to the mask of signals that the uim task is interested
*                   in handling.   The mask may be changed by this function.
*  @param uim_ptr Pointer to the current instance of the UIM
*                 global structure
*  @return boolean Return TRUE to indicate that immediately
*          going back to the top of the main loop is desired.
*
*/
boolean uim_signal_handler(rex_sigs_type *sig_mask_ptr,
                           rex_sigs_type *imask_ptr,
                           uim_instance_global_type *uim_ptr)
{
  int i = 0;
  boolean b_goto_top = FALSE;

  if(uim_ptr->id >= UIM_INSTANCE_INVALID)
  {
    return FALSE;
  }
  /* Turn on clock if it is stopped and there is a cmd in the Q */
  uim_checkq_and_turn_on_clk(uim_ptr);

  for (i=0;i<ARR_SIZE(uim_sig_map);i++)
  {
    if (*sig_mask_ptr & uim_sig_map[i].sigMask)
    {
      /* Dog report/command_Q  sig print msg is not useful, skip printing to reduce log */
      if ( !((UIM_DOG_HB_RPT_SIG == uim_sig_map[i].sigMask) || 
         (UIM_CMD_Q_SIG == uim_sig_map[i].sigMask)))
      {
        UIMDRV_MSG_MED_1(uim_ptr->id, "uim_sigs: calling handler number %d", i);
      }
      /* The handler may change the value of *sig_mask_ptr */
      b_goto_top = uim_sig_map[i].handler(sig_mask_ptr, imask_ptr, uim_ptr);

      if (TRUE == b_goto_top)
      {
        /* Goto the top of the main loop to execute rex_wait() */
        return TRUE;
      }
    }
  }
  return FALSE;
} /* uim_signal_handler */


/**
*  This function handles the UIM_DOG_HB_RPT_SIG signal.
*
*  @param sig_mask_ptr  Pointer to the OR's set of signals that UIM task needs tohandle.  Unused in this function.
*  @param imask_ptr Pointer to the mask of signals that the uim task is interested
*                   in handling.   Unused in this function.
*  @param uim_ptr Pointer to the current instance of the UIM global structure
*  @return boolean Return TRUE to indicate that immediately
*          going back to the top of the main loop is desired.
*/
static boolean uim_handle_dog_sig(rex_sigs_type *sig_mask_ptr,
                                  rex_sigs_type *imask_ptr,
                                  uim_instance_global_type *uim_ptr)
{
  /* DOG HeartBeat
  1. A single shared timer resource among all clients is used to register for dog.
  2. Dog will send the signals to the registered tasks ("ping") instead of going through timer,
     through dog's own heartbeat, and the tasks will respond to wdog ("pong")
     Voluntary dog reporting is supported (pongs without pings).
  3. Hence we use dog_hb_report() to report to the dog and in turn pet it.
  */
  (void)sig_mask_ptr;
  (void)imask_ptr;
  if(uim_ptr != NULL)
  {
    (void) rex_clr_sigs(uim_ptr->tcb_ptr, UIM_DOG_HB_RPT_SIG );
    /* Makes sure that Dog will not be pet when the dog variable is assigned to ZERO after Task Stop */
    if(uim_ptr->task.dog_rpt_var != 0)
    {
      dog_hb_report(uim_ptr->task.dog_rpt_var);
    }
  }
  return FALSE;
}/* uim_handle_dog_sig */


/**
*  This function handles the UIM_TASK_STOP_SIG signal.
*
*
*  @param sig_mask_ptr  Pointer to the OR's set of signals that UIM task needs tohandle.  Unused in this function.
*  @param imask_ptr Pointer to the mask of signals that the uim task is interested
*                   in handling.   Unused in this function.
*  @param uim_ptr Pointer to the current instance of the UIM global structure
*  @return boolean Return TRUE to indicate that immediately
*          going back to the top of the main loop is desired.
*/
static boolean uim_handle_task_stop_sig(rex_sigs_type *sig_mask_ptr,
                                        rex_sigs_type *imask_ptr,
                                        uim_instance_global_type *uim_ptr)
{
  UIMDRV_MSG_MED_0(uim_ptr->id, "UIM_TASK_STOP_SIG received" );
  (void) rex_clr_sigs( uim_ptr->tcb_ptr, UIM_TASK_STOP_SIG );
  (void)sig_mask_ptr;
  (void)imask_ptr;

  /* Make sure to power down the interface when not in use. */
  uim_ptr->flag.powering_down_task = TRUE;
  (void)uim_power_control_ds((uim_slot_type)(uim_ptr->id + 1),
                             (uim_voter_type)((int)UIM_AUTH | (int)UIM_MC |
                             (int)UIM_PROACTIVE_UIM | (int)UIM_UI |
                             (int)UIM_UICC_SESSION | (int)UIM_SESSION |
                              UIM_TRAF_CHAN),
                              FALSE,
                              UIM_CALL_TYPE_NOT_APPLICABLE);

  /* change the status to ERROR so that we will send back any commands that
   * might come our way after the task_stop signal
   */
  uim_ptr->state.status = UIM_ERR_S;

  /* RESET all the timers */
  rex_clr_timer( &uim_ptr->command.cmd_rsp_timer);
  rex_clr_timer(&uim_ptr->atr.atr_40k_timer);
  rex_clr_timer(&uim_ptr->poll.wake_up_poll_timer);
  rex_clr_timer(&uim_ptr->poll.no_wake_up_poll_timer);

  if((TRUE == uim_ptr->extended_recovery.extended_recovery_enabled) &&
     (TRUE == uim_ptr->extended_recovery.extended_recovery_in_progress))
  {
    /*task stop signal has come. abandon the extended recovery and clear the relavent globals*/
    uim_extended_recovery_event_handler(uim_ptr, UIM_EXTENDED_RECOVERY_EVT_TERMINATE);
  }

   /* UIM_STATE_TOGGLE_SIG will take care of powering down the
      UIM interface and flushing all queues */
  (void) rex_set_sigs( uim_ptr->tcb_ptr, UIM_STATE_TOGGLE_SIG);

  return FALSE;

} /* uim_handle_task_stop_sig */


/**
*  This function handles the UIM_POLL_TIMER_SIG signal.
*
*
*  @param sig_mask_ptr  Pointer to the OR's set of signals that UIM task needs to handle.
*                       Unused in this function.
*  @param imask_ptr Pointer to the mask of signals that the uim task is interested
*                   in handling.   Unused in this function.
*  @param uim_ptr Pointer to the current instance of the UIM global structure
*  @return boolean Return TRUE to indicate that immediately
*          going back to the top of the main loop is desired.
*/
static boolean uim_handle_poll_timer_sig(rex_sigs_type *sig_mask_ptr,
                                         rex_sigs_type *imask_ptr,
                                         uim_instance_global_type *uim_ptr)
{
  (void)sig_mask_ptr;
  (void)imask_ptr;
  
  /* clear the timer since for dual timer solution, one more timer
   * might still be running.  And for single timer solution, this
   * would not affect any functionality.
   */
  /* clear the timer since for dual timer solution, one more timer
   * might still be running.  And for single timer solution, this
   * would not affect any functionality.
   */
  uim_clear_poll_timer(uim_ptr);

  /* clear the signal */
  (void) rex_clr_sigs( uim_ptr->tcb_ptr, UIM_POLL_TIMER_SIG);
  
  /* If the poll occurs due to flaky card removal if an existing command is in progress */
  if( uim_ptr->poll.flaky_polling_state == UIM_FLAKY_POLLING_ATTEMPT_1)
  {
    /* If the command is still in progress do not poll again and return */
    if(uim_ptr->flag.command_in_progress)
    {
      /* Don't reset the poll time if the command in progress is poll */
      if((uim_ptr->flag.static_buffer_used) &&
         (uim_ptr->command.static_cmd_buf.hdr.command != UIM_INTERNAL_POLL_F))
      {
      uim_ptr->setting.poll_time_delay       = uim_ptr->poll.orig_poll_time;
      uim_ptr->poll.flaky_polling_state      = UIM_FLAKY_POLLING_OFF;
      }
      return TRUE;
    }
    else
    {
      uim_ptr->poll.flaky_polling_state = UIM_FLAKY_POLLING_ATTEMPT_2;
    }
  }

  /* Set the internal flag as poll pending only if GSTK has
   * not disabled Polling
   */
  uim_ptr->flag.poll_pending = TRUE;

  return FALSE;
} /* uim_handle_poll_timer_sig */


/**
*  This function checks the UIM command queue and turns the UIM clock on
*  if there is a command in the queue.
*
*
*  @param uim_ptr Pointer to the current instance of the UIM global structure
*/
void uim_checkq_and_turn_on_clk(uim_instance_global_type *uim_ptr)
{

  if(uim_ptr->setting.phy_layer != UIM_ISO)
  {
    return;
  }

  if (uim_ptr->state.status == UIM_CLK_OFF_S)
  {
    /* we have a command in the q */
    if (q_cnt(& uim_ptr->task.cmd_q) != 0)
    {
      uim_stop_power_save(uim_ptr);
      /* Clock started */
      UIMDRV_MSG_MED_0(uim_ptr->id,"Clock started");

      /* Set the state back to Initialized state */
      uim_ptr->state.status = UIM_INITIALIZED_S;
    } /* end if - Check if clock should be turned back on */
  } /* end if - Check if clock is stopped */
} /* uim_checkq_and_turn_on_clk */


/**
*  This function handles the UIM_STATE_TOGGLE_SIG signal.
*
*
*  @param sig_mask_ptr Pointer to the OR's set of signals that UIM task needs to
*                      handle.  Unused in this function.
*  @param imask_ptr Pointer to the mask of signals that the uim task is interested
*                   in handling.  Unused in this function.
*  @param uim_ptr Pointer to the current instance of the UIM global structure
*  @return boolean Return TRUE to indicate that immediately
*          going back to the top of the main loop is desired.
*/
static boolean uim_handle_state_toggle_sig(rex_sigs_type *sig_mask_ptr,
                                           rex_sigs_type *imask_ptr,
                                           uim_instance_global_type *uim_ptr)
{
  (void)sig_mask_ptr;
  (void)imask_ptr;
  
  /* clear the signal */
  (void) rex_clr_sigs(uim_ptr->tcb_ptr, UIM_STATE_TOGGLE_SIG);
  return FALSE;
} /* uim_handle_state_toggle_sig */


/**
*  This function handles the UIM_CMD_Q_SIG signal.
*
*
*  @param sig_mask_ptr Pointer to the OR's set of signals that UIM task needs to
*                      handle.  Unused in this function.
*  @param imask_ptr Pointer to the mask of signals that the uim task is interested
*                   in handling.  The value may be updated in this function.
*  @param uim_ptr Pointer to the current instance of the UIM global structure
*  @return boolean Return TRUE to indicate that immediately
*          going back to the top of the main loop is desired.
*/
static boolean uim_handle_cmd_q_sig(rex_sigs_type *sig_mask_ptr,
                                    rex_sigs_type *imask_ptr,
                                    uim_instance_global_type *uim_ptr)
{
  if(UIM_CARD_SAM  == uim_ptr->card_mode)
  {
    return uim_sam_handle_cmd_q_sig(sig_mask_ptr, imask_ptr, uim_ptr);
  }
  else
  {
    return uim_telecom_handle_cmd_q_sig(sig_mask_ptr, imask_ptr, uim_ptr);
  }
}
/**
*  This function handles the UIM_CMD_Q_SIG signal.
*
*
*  @param sig_mask_ptr Pointer to the OR's set of signals that UIM task needs to
*                      handle.  Unused in this function.
*  @param imask_ptr Pointer to the mask of signals that the uim task is interested
*                   in handling.  The value may be updated in this function.
*  @param uim_ptr Pointer to the current instance of the UIM global structure
*  @return boolean Return TRUE to indicate that immediately
*          going back to the top of the main loop is desired.
*/
static boolean uim_telecom_handle_cmd_q_sig(rex_sigs_type *sig_mask_ptr,
                                    rex_sigs_type *imask_ptr,
                                    uim_instance_global_type *uim_ptr)
{
  (void) rex_clr_sigs( uim_ptr->tcb_ptr, UIM_CMD_Q_SIG );
  if( q_cnt(&uim_ptr->task.cmd_q) == 0)
  {
    UIMDRV_MSG_ERR_0(uim_ptr->id,"q_cnt is 0 before q_get");
  }
  if ((uim_ptr->command.cmd_ptr = (uim_cmd_type *)q_get( &uim_ptr->task.cmd_q )) != NULL)
  {
    /* Defensive code to turn on clocks */
    if(uim_ptr->state.status == UIM_CLK_OFF_S)
    {
      UIMDRV_PUP_MSG_ERR_0(UIMLOG_MSG_178,uim_ptr->id,"CLOCK OFF when processing command");
      uim_stop_power_save(uim_ptr);
      /* Set the state back to Initialized state */
      uim_ptr->state.status = UIM_INITIALIZED_S;
    }
    /* Command has not been attempted yet */
    uim_ptr->command.rpt_buf.cmd_transacted = FALSE;
    /* If a command is queued when UIM is in an erroneous state allow the
       command to be processed only if it is a reset, power down or
       passive power up command type */
    if ( ( (uim_ptr->state.status == UIM_ERR_S) &&
           (uim_ptr->command.cmd_ptr->hdr.command  != UIM_POWER_UP_UIM_PASSIVE_F) &&
           (uim_ptr->command.cmd_ptr->hdr.command  != UIM_RESET_SWITCH_UIM_PASSIVE_F) &&
           (uim_ptr->command.cmd_ptr->hdr.command  != UIM_POWER_DOWN_F) &&
           (uim_ptr->command.cmd_ptr->hdr.command  != UIM_RESET_F) ) ||
           (! UIM_IS_CMD_VALID_IN_MODE(uim_ptr->command.cmd_ptr, uim_ptr)) ||
           (uim_is_reset_cmd_not_allowed(uim_ptr)) ||
           /* check whether logical channel is opened or not */
           (FALSE == uim_is_streaming_cmd_allowed(uim_ptr)) ||
           (UIM_SLOT_STATE_INACTIVE == uim_ptr->activity_status)
       )
    {
      /* Set the status words to default*/
      uim_ptr->command.rpt_buf.sw1 = (uim_sw1_type) 0;
      uim_ptr->command.rpt_buf.sw2 = (uim_sw2_type) 0;
      if(UIM_SLOT_STATE_INACTIVE == uim_ptr->activity_status)
      {
        UIM_MSG_ERR_0("Slot inactive. retrning error");

        if(UIM_RESET_F == uim_ptr->command.cmd_ptr->hdr.command)
        {
          /*Reset the logical slot power down flag. Just in case if it was set*/
          uim_set_logical_slot_power_down_flag(uim_ptr, FALSE);
        }
        else if(UIM_POWER_DOWN_F == uim_ptr->command.cmd_ptr->hdr.command)
        {
          /*Reset the logical slot power down flag. Just in case if it was set*/
          uim_set_logical_slot_power_down_flag(uim_ptr, TRUE);
        }
      }
      else if (uim_ptr->state.status == UIM_ERR_S)
      {
        UIMDRV_PUP_MSG_HIGH_0(UIMLOG_MSG_179,uim_ptr->id, "SIM in power down state");
      }
      else if(FALSE == uim_is_streaming_cmd_allowed(uim_ptr))
      {
        UIMDRV_PUP_MSG_HIGH_1(UIMLOG_MSG_180,uim_ptr->id, "STREAMING COMMAND is not allowed for channel 0x%x",
                          uim_ptr->command.cmd_ptr->hdr.channel);
        
        /* Set the status words to logical channel not supported */
        uim_ptr->command.rpt_buf.sw1 = SW1_CLA_NOT_SUPPORTED;
        uim_ptr->command.rpt_buf.sw2 = SW2_LOGICAL_CHAN_NOT_SUPPORTED;
      }
      else
      {
        UIM_PUP_ERROR_MSG_0(UIMLOG_MSG_181,"Invalid cmd UIM in passive mode ret error");
      }
      uim_ptr->command.cmd_ptr->hdr.status = UIM_ERR_S;

      if (uim_ptr->command.cmd_ptr->hdr.cmd_hdr.task_ptr != NULL)
      {
        (void) rex_set_sigs( uim_ptr->command.cmd_ptr->hdr.cmd_hdr.task_ptr,
                             uim_ptr->command.cmd_ptr->hdr.cmd_hdr.sigs );
      }

      /* Get the mode of the command */
      uim_ptr->state.cmd_mode = (uim_command_mode_type)
                     UIM_GET_CMD_MODE((int)uim_ptr->command.cmd_ptr->hdr.command);

      /* If always reporting & have a rpt function, report failure
         uim_send_err_rpt internally releases the buffer */
      if (((int)uim_ptr->command.cmd_ptr->hdr.options & (int)UIM_OPTION_ALWAYS_RPT) != 0)
      {
        uim_send_err_rpt(uim_ptr->command.cmd_ptr, uim_ptr);
      }
    }
    else
    {
      if  ( (uim_ptr->state.status == UIM_ERR_S) &&
           ((uim_ptr->command.cmd_ptr->hdr.command  == UIM_POWER_UP_UIM_PASSIVE_F) ||
            (uim_ptr->command.cmd_ptr->hdr.command == UIM_RESET_SWITCH_UIM_PASSIVE_F) ||
           (uim_ptr->command.cmd_ptr->hdr.command  == UIM_POWER_DOWN_F) ||
           (uim_ptr->command.cmd_ptr->hdr.command  == UIM_RESET_F)))
      {
        if((TRUE == uim_ptr->extended_recovery.extended_recovery_enabled) && (TRUE == uim_ptr->extended_recovery.extended_recovery_in_progress))
        {
          /*abandon the extended recovery and clear the relavent globals*/
          uim_extended_recovery_event_handler(uim_ptr, UIM_EXTENDED_RECOVERY_EVT_TERMINATE);
        }
      }
      /* We do not want to look into the command queue until we are
      done processing this command */

      (*imask_ptr) &= ~UIM_CMD_Q_SIG;
      uim_ptr->flag.static_buffer_used = FALSE;

      if (uim_ptr->command.cmd_ptr->hdr.cmd_count == 0)
      {
        /* Set Count to indicate that this is the first attempt.. once
         * set to 1, it would not be reset to 0
         */
        uim_ptr->command.cmd_ptr->hdr.cmd_count = 1;
        /* Intialising here. In uim_process_generic_cmd this takes proper value */
        uim_ptr->setting.max_num_cmd_attempts = UIM_MAX_NUM_CMD_ATTEMPTS(uim_ptr);
      }
      /* Re-intatialize internal command used since we have popped a
       * fresh command out of the command queue.  If an internal
       * command has to be used, it would be set again
       */
      uim_ptr->flag.internal_command_used = FALSE;
      uim_ptr->card_cmd.work_waiting_time =
            uim_ptr->card_cmd.original_work_waiting_time;

      uim_ptr->card_cmd.work_waiting_time_etus =
            uim_ptr->card_cmd.original_work_waiting_time_etus;

      if ( (uim_ptr->command.cmd_ptr->hdr.cmd_count > uim_ptr->setting.max_num_cmd_attempts) ||
           (uim_ptr->command.cmd_ptr->hdr.cmd_recovery_count > uim_ptr->setting.max_num_cmd_attempts))
      {
        /* include the command Q sig to the mask as we are going
           to process the command again */
        (*imask_ptr) |= UIM_CMD_Q_SIG;

        /* Get the mode of the command */
        uim_ptr->state.cmd_mode = (uim_command_mode_type)
                         UIM_GET_CMD_MODE((int)uim_ptr->command.cmd_ptr->hdr.command);
        /* reset the static buffer used flag */
        uim_ptr->flag.static_buffer_used = FALSE;

        /* reset the internal command used flag */
        uim_ptr->flag.internal_command_used = FALSE;

        /* This command has already got enough chances but did not go
         * through so send an appropriate error report
         */
        uim_send_err_rpt(uim_ptr->command.cmd_ptr, uim_ptr);
      }
      else
      {
        uim_process_command(imask_ptr, uim_ptr);
      }
    } /* uim status is error */
  }  /* (uim_ptr->command.cmd_ptr != NULL) */

  return FALSE;
} /* uim_handle_cmd_q_sig */


/**
*  This function handles the UIM_CMD_RSP_SIG signal.
*
*
*  @param sig_mask_ptr Pointer to the OR's set of signals that UIM task needs to
*                      handle.  Unused in this function.
*  @param imask_ptr Pointer to the mask of signals that the uim task is interested
*                   in handling.  The value may be updated in this function.
*  @param uim_ptr Pointer to the current instance of the UIM global structure
*  @return boolean Return TRUE to indicate that immediately
*          going back to the top of the main loop is desired.
*/
static boolean uim_handle_cmd_rsp_sig(rex_sigs_type *sig_mask_ptr,
                                      rex_sigs_type *imask_ptr,
                                      uim_instance_global_type *uim_ptr)
{
  /*switch to SAM signal handler if current task is in SAM mode*/
  if(UIM_CARD_SAM  == uim_ptr->card_mode)
  {
    return uim_sam_handle_cmd_rsp_sig(sig_mask_ptr, imask_ptr, uim_ptr);
  }
  else
  {
    return uim_telecom_handle_cmd_rsp_sig(sig_mask_ptr, imask_ptr, uim_ptr);
  }
}

/**
*  This function handles the UIM_CMD_RSP_SIG signal.
*
*
*  @param sig_mask_ptr Pointer to the OR's set of signals that UIM task needs to
*                      handle.  Unused in this function.
*  @param imask_ptr Pointer to the mask of signals that the uim task is interested
*                   in handling.  The value may be updated in this function.
*  @param uim_ptr Pointer to the current instance of the UIM global structure
*  @return boolean Return TRUE to indicate that immediately
*          going back to the top of the main loop is desired.
*/
static boolean uim_telecom_handle_cmd_rsp_sig(rex_sigs_type *sig_mask_ptr,
                                      rex_sigs_type *imask_ptr,
                                      uim_instance_global_type *uim_ptr)
{
  (void) rex_clr_sigs( uim_ptr->tcb_ptr, UIM_CMD_RSP_SIG );

  if ((uim_ptr->hotswap.hotswap_flaky_switch_support)
      && (TRUE == uim_ptr->hotswap.hotswap_card_removed_suspicious)
      && (*uim_ptr->command.generic_state_ptr != UIM_POWER_UP_ST)
      && (uim_ptr->poll.flaky_polling_state != UIM_FLAKY_POLLING_ATTEMPT_1))
  {
    /* It got confirmed that card is inserted as we received response signal */
    uim_ptr->hotswap.card_status = UIM_CARD_INSERTED;
    uim_ptr->hotswap.hotswap_card_status = UIM_CARD_INSERTED;
    /* Clearng and unmasking card remove interrupt,
       Clearing the suscipious flag happens in uim_hotswap_unmask_interrupt()*/
    uim_hotswap_clear_interrupt(uim_ptr);
    uim_hotswap_unmask_interrupt(uim_ptr);
  }

  /* Defensive check, in case simulate timer set the CMD_RSP_SIG without having a cmd_ptr */
  if( uim_ptr->command.cmd_ptr != NULL )
  {
    /* Check for simulate NULL enable*/
    if( (uim_ptr->command.cmd_ptr->hdr.command == UIM_STREAM_ISO7816_APDU_F) &&
        (uim_ptr->command.cmd_ptr->stream_iso7816_apdu.cmd_data[UIM_7816_APDU_INSTRN_OFFSET] ==
         uim_ptr->debug.uim_feature_busy_response_simulate_null.uim_simulate_null_ins) &&
        (uim_ptr->debug.uim_feature_busy_response_simulate_null.uim_disable_simulate_null == FALSE)
      )
    {
      UIMDRV_MSG_HIGH_0(uim_ptr->id, "Simulating null busy and not clearing timers");
    }
    else
    {
      /* UIM BUSY RESPONSE FEATURE: Call mmgsdi API to clear uim busy */
      uim_busy_ind_timer_state_handler(uim_ptr, UIM_BUSY_RESPONSE_EVT_CMD_RSP_SIG);
      uim_trans_timer_state_handler(uim_ptr, UIM_BUSY_RESPONSE_EVT_CMD_RSP_SIG);
    }
  }
  /* Check to see if we have already flushed out all the commands and
     are not in a state to process commands.
     If a command response is available when UIM is in an erroneous state
     allow the response to be processed only if it is a power down command */
  if (((uim_ptr->state.status == UIM_ERR_S)  &&
       ((uim_ptr->command.cmd_ptr != NULL) && (uim_ptr->command.cmd_ptr->hdr.command  != UIM_POWER_DOWN_F))) ||
      (!uim_ptr->flag.static_buffer_used && uim_ptr->command.cmd_ptr == NULL) )
  {
    UIMDRV_PUP_MSG_ERR_3(UIMLOG_MSG_182,uim_ptr->id,
                      "CMD_RSP Sig Rcvd uim_status=0x%x, uim_st_bf_us=0x%x, cmd_ptr=0x%x",
                      uim_ptr->state.status,
                      uim_ptr->flag.static_buffer_used,
                      uim_ptr->command.cmd_ptr);
    /* we have received the response */
    uim_ptr->flag.command_in_progress = FALSE;
    /* If always reporting & have a rpt function, report failure
        uim_send_err_rpt internally releases the buffer */
    if ( (uim_ptr->command.cmd_ptr != NULL) && 
       (((int)uim_ptr->command.cmd_ptr->hdr.options & (int)UIM_OPTION_ALWAYS_RPT) != 0) )
    {
      uim_send_err_rpt(uim_ptr->command.cmd_ptr, uim_ptr);
    }
    if (uim_ptr->flag.powering_down_task)
    {
      /* Need not set the cmd_q_sig in i_mask as task stop is called */
      (void) rex_set_sigs(uim_ptr->tcb_ptr, UIM_STATE_TOGGLE_SIG);
      return TRUE;
    }
    /* Go back to the beginning of the main task loop */
    (*imask_ptr) |= UIM_CMD_Q_SIG;
    return TRUE;
  }

  if ((uim_ptr->command.rsp_buf.cmd_status == UIM_REMOTE_STATUS) &&
      IS_BTSAP_HANDLE_ACTIVE(uim_ptr))
  {
    /* uim receives only sw1 sw2 from QMI remote client
       This func gets the command status for the status words*/
    uim_remote_get_command_status(uim_ptr);
    if(uim_ptr->remote.intermediate_done  == TRUE)
    {
      return TRUE;
    }
  }

  /* Check to see if we have a timeout signal and clear it since
   * we are processing the response.
   */
  if ( uim_ptr->flag.clear_timeout_on_response )
  {
    (void) rex_clr_sigs( uim_ptr->tcb_ptr, UIM_CMD_RSP_TIMEOUT_SIG );
    (*sig_mask_ptr) &= ~UIM_CMD_RSP_TIMEOUT_SIG;
  }
  else
  {
    /* reset the flag for next use */
    uim_ptr->flag.clear_timeout_on_response = TRUE;
  }

  #if defined( FEATURE_UIM_T_1_SUPPORT )
  if (UIM_T_1_PROTOCOL == uim_ptr->state.current_protocol)
  {
    if(TRUE == uim_ptr->t1.t_1_block_received)
    {
      uim_ptr->t1.t_1_block_received = FALSE;
      /* Process the card response for the T=1 protocol */
      uim_process_t_1_block( &uim_ptr->command.rsp_buf, imask_ptr, uim_ptr);
    }
    else
    {
      uim_process_card_response(imask_ptr, uim_ptr);
    }
  }
  else
  #endif /* FEATURE_UIM_T_1_SUPPORT */
  {
    uim_process_card_response(imask_ptr, uim_ptr);

    if( uim_ptr->setting.pdown_on_tech_prob.to_pdown &&
        uim_ptr->card_cmd.inc_tech_prob_count >= uim_ptr->setting.pdown_on_tech_prob.cnt_tech_problem &&
        uim_ptr->state.status == UIM_ERR_S )
    {
      uim_ptr->card_cmd.inc_tech_prob_count = 0;

      /* Power down the interface */
      uim_power_down(uim_ptr);

      /* Notify the higher layers that UIM is declared faulty */
      uim_notify_error(UIM_TECHNICAL_PROBLEM_S, uim_ptr);

      /* Go back to the beginning of the main task loop */
      return TRUE;
    }
  } /* end if - the protocol is T=1 */

  return FALSE;
} /* uim_handle_cmd_rsp_sig */


/**
*   This function handles the UIM_EFSLOG_PURGE_SIG signal.
*
*
*  @param sig_mask_ptr Pointer to the OR's set of signals that UIM task needs to
                       handle.  Unused in this function.
*  @param imask_ptr Pointer to the mask of signals that the uim task is interested
                    in handling.  Unused in this function.
*  @param uim_ptr Pointer to the current instance of the UIM global structure
*  @return boolean Return TRUE to indicate that immediately
*          going back to the top of the main loop is desired.
*/
static boolean uim_handle_efslog_purge_sig(rex_sigs_type *sig_mask_ptr,
                                        rex_sigs_type *imask_ptr,
                                        uim_instance_global_type *uim_ptr)
{
  /* Suppress the compiler warnings */
  (void)imask_ptr;
  (void)sig_mask_ptr;
  /* Turn on clock if it is stopped and there is a cmd in the Q */
  uim_checkq_and_turn_on_clk(uim_ptr);
  (void)uim_efslog_process_purge_sig(uim_ptr);

  return FALSE;
} /* handle_efslog_purge_sig */


/**
*  This function handles the UIM_TRANSACTION_SIG signal.
*
*  SIDE EFFECTS
*    This function may change the value of *imask_ptr or *sig_mask_ptr.
*
*
*  @param sig_mask_ptr  Pointer to the OR's set of signals that UIM task needs to
*                       handle.  The value may be updated by this function.
*  @param imask_ptr Pointer to the mask of signals that the uim task is interested
*                   in handling.  Unused in this function.
*  @param uim_ptr Pointer to the current instance of the UIM global structure
*  @return boolean Return TRUE to indicate that immediately
*          going back to the top of the main loop is desired.
*/
static boolean uim_handle_cmd_rsp_timeout_sig(rex_sigs_type *sig_mask_ptr,
                                              rex_sigs_type *imask_ptr,
                                              uim_instance_global_type *uim_ptr)
{
  /*switch to SAM signal handler if current task is in SAM mode*/
  if(UIM_CARD_SAM  == uim_ptr->card_mode)
  {
    return (uim_sam_handle_cmd_rsp_timeout_sig(sig_mask_ptr, imask_ptr, uim_ptr));
  }
  else
  {
    return (uim_telecom_handle_cmd_rsp_timeout_sig(sig_mask_ptr, imask_ptr, uim_ptr));
  }
}

/**
*  This function handles the UIM_TRANSACTION_SIG signal.
*
*  SIDE EFFECTS
*    This function may change the value of *imask_ptr or *sig_mask_ptr.
*
*
*  @param sig_mask_ptr  Pointer to the OR's set of signals that UIM task needs to
*                       handle.  The value may be updated by this function.
*  @param imask_ptr Pointer to the mask of signals that the uim task is interested
*                   in handling.  Unused in this function.
*  @param uim_ptr Pointer to the current instance of the UIM global structure
*  @return boolean Return TRUE to indicate that immediately
*          going back to the top of the main loop is desired.
*/
static boolean uim_telecom_handle_cmd_rsp_timeout_sig(rex_sigs_type *sig_mask_ptr,
                                              rex_sigs_type *imask_ptr,
                                              uim_instance_global_type *uim_ptr)
{
  int              i                      = 0;
  rex_sigs_type    orig_sig_mask          = 0;
  uim_status_type  notification_reason    = UIM_ERR_S;
  uint32           uim_recovery_wait_time = 0;

  if(sig_mask_ptr == NULL || imask_ptr == NULL || uim_ptr == NULL)
  {
    return FALSE;  
  }
  
  orig_sig_mask = *sig_mask_ptr;

#ifdef FEATURE_SUPPORTS_NON_REMOVABLE_UICC
  #error code not present
#endif
  /* Turn on clock if it is stopped and there is a cmd in the Q */
  uim_checkq_and_turn_on_clk(uim_ptr);

  /* clear UIM_TRANSACTION_SIG from the current signal to ensure
     handle_cmd_rsp_timeout_sig() does not get called again */
  (*sig_mask_ptr)  &= ~UIM_TRANSACTION_SIG;
  if(IS_BTSAP_HANDLE_ACTIVE(uim_ptr))
  {
    UIMDRV_MSG_HIGH_0(uim_ptr->id, "Received timeout signal when remote client is active");
    uim_ptr->remote.state = WAIT_FOR_NOTHING;
  }

  if( orig_sig_mask & UIM_CMD_RSP_SIG )
  {
    UIMDRV_MSG_ERR_0(uim_ptr->id,"WARNING: Command response and Command response timeout happened at the same time");
    /* Clear the command response timer */
    (void) rex_clr_sigs( uim_ptr->tcb_ptr, UIM_CMD_RSP_TIMEOUT_SIG );

    /* Go back to the top of the main loop */
    return TRUE;
  }

  /* Check hotswap is enable for current slot */
  if(uim_hw_if.intctrl[uim_ptr->id].m_hs_enable == TRUE)
  {
    if  (uim_ptr->hotswap.hotswap_capability.hos_enabled == TRUE)
    {
      /* Time-out handling when we suspect the card removal or in case of extended recovery */
      if (((uim_ptr->hotswap.hotswap_flaky_switch_support)             &&
          (uim_ptr->hotswap.hotswap_card_removed_suspicious == TRUE))  || 
          ((uim_ptr->extended_recovery.extended_recovery_enabled)      &&
           (uim_ptr->extended_recovery.extended_recovery_in_progress)  && 
           (uim_ptr->hotswap.card_status ==  UIM_CARD_REMOVED)))
      {
         /* Usually when there is timeout, we can confirm it to be
            a genuine card removal case, but give a chance to try with 3v
            in case ATR_PPS is not done (power-up case, but not during recovery) */
        if(  (!uim_ptr->atr.atr_pps_done
              && (uim_ptr->state.current_voltage_class < uim_ptr->state.max_possible_voltage_class)
              && (!uim_ptr->flag.recovery_triggered)))
        {
          UIMDRV_PUP_MSG_HIGH_0(UIMLOG_MSG_186,uim_ptr->id, "Attempting power up at 3V");
          uim_ptr->state.current_voltage_class++;
          uim_ptr->command.static_cmd_buf.hdr.cmd_count = 0;
          uim_reset_uim (imask_ptr, uim_ptr->flag.me_powerup, uim_ptr);
        }
        else
        {
          /* Confirmed that there is no card.
             So send card rem sig and unmasking the card inserted interrupt */
          (void) rex_clr_sigs( uim_ptr->tcb_ptr, UIM_CMD_RSP_TIMEOUT_SIG );
          (void) rex_clr_sigs( uim_ptr->tcb_ptr, UIM_TRANSACTION_SIG );
          uim_ptr->hotswap.card_status = UIM_CARD_REMOVED;
          uim_ptr->hotswap.hotswap_card_status = UIM_CARD_REMOVED;
          uim_hotswap_send_card_removed_cmd(uim_ptr);
          uim_hotswap_unmask_interrupt(uim_ptr);
        }
        return TRUE;
      }
      /*  Check the card status, if card is not removed then the debounce will end with no change
          in status and recovery logic will continue. */
      UIMDRV_PUP_MSG_HIGH_1(UIMLOG_MSG_189,uim_ptr->id,"HOTSWAP: card_status-0x%x during cmd_rsp_timeout",
                            uim_ptr->hotswap.card_status);

      if(uim_ptr->hotswap.card_status == UIM_CARD_REMOVED)
      {
        (void) rex_clr_sigs( uim_ptr->tcb_ptr, (( (*sig_mask_ptr) & UIM_TRANSACTION_SIG) |
                            ((*sig_mask_ptr) & UIM_CMD_RSP_TIMEOUT_SIG)));

        if ((!uim_ptr->flag.static_buffer_used) && uim_ptr->command.cmd_ptr != NULL)
        {
          UIMDRV_PUP_MSG_HIGH_2(UIMLOG_MSG_188,uim_ptr->id,"Reporting err for current cmd: 0x%x Status: 0x%x",
                            uim_ptr->command.cmd_ptr->hdr.command,
                            uim_ptr->command.cmd_ptr->hdr.status);

          if (uim_ptr->command.cmd_ptr->hdr.cmd_hdr.task_ptr != NULL)
          {
            (void) rex_set_sigs( uim_ptr->command.cmd_ptr->hdr.cmd_hdr.task_ptr,
            uim_ptr->command.cmd_ptr->hdr.cmd_hdr.sigs );
          }
          /* Get the mode of the command */
          uim_ptr->state.cmd_mode = (uim_command_mode_type)
                          UIM_GET_CMD_MODE((int)uim_ptr->command.cmd_ptr->hdr.command);

          if (((int)uim_ptr->command.cmd_ptr->hdr.options & (int)UIM_OPTION_ALWAYS_RPT) != 0)
          {
            /* Set the status words to default before sending the rpt */
            uim_ptr->command.rpt_buf.sw1 = (uim_sw1_type) 0;
            uim_ptr->command.rpt_buf.sw2 = (uim_sw2_type) 0;
            /* It indicate that Command is not successfully send to the card */
            uim_ptr->command.rpt_buf.cmd_transacted = FALSE;
            uim_send_err_rpt(uim_ptr->command.cmd_ptr, uim_ptr);
          }
        }
        else
        {
          /* This command has been completed */
          uim_ptr->flag.command_in_progress    = FALSE;
          /* Reset the command re-queued flag */
          uim_ptr->flag.command_requeued        = FALSE;
          uim_ptr->flag.internal_command_used = FALSE;
        }
       /* The status of UIM is set to Removed state */
       uim_ptr->state.status  = UIM_ERR_S;
       (*imask_ptr) |= UIM_CMD_Q_SIG;

       /* Go back to the top of the main loop */
       return TRUE;
      }
      else
      {
        /* In case of extended recovery do not start the debounce */
        if(!uim_ptr->extended_recovery.extended_recovery_in_progress)
        {
          UIM_DETECT_CARD_STATUS(uim_ptr);
          uim_recovery_wait_time = (UIM_HOTSWAP_CARD_REMVOVAL_SAMPLE_INTERVAL *
                                    uim_ptr->hotswap.hotswap_me_conf.num_of_sample_for_removal *
                                    uim_ptr->hotswap.hotswap_me_conf.maximum_debounce_retry_counter) + 1 ;

          if(uim_recovery_wait_time < UIM_HOTSWAP_MINTIME_HW_INTERRUPT)
          {
            uim_recovery_wait_time = UIM_HOTSWAP_MINTIME_HW_INTERRUPT;
          }
          else if(UIM_HOTSWAP_MAXTIME_HW_INTERRUPT < uim_recovery_wait_time)
          {
            uim_recovery_wait_time = UIM_HOTSWAP_MAXTIME_HW_INTERRUPT;
          }                               
          UIMDRV_MSG_HIGH_1(uim_ptr->id, 
                 "Delaying the recovery process by %dms to wait for Card Removed interrupt ", uim_recovery_wait_time);
              
          uim_timed_sleep(
                 uim_recovery_wait_time,
                 uim_ptr,
                 UIM_HOTSWAP_CMD_CARD_REM_SIG);

          if (UIM_CARD_REMOVED == uim_ptr->hotswap.card_status)
          {
            UIMDRV_MSG_HIGH_0(uim_ptr->id,
                "Recovery is not allowed, card is either not inserted or not detected on this slot");
            if (uim_ptr->hotswap.hotswap_flaky_switch_support)
            {
              UIMDRV_MSG_HIGH_0(uim_ptr->id," Setting suspicious card removed signal during power-up ");
              (void) rex_set_sigs( uim_ptr->tcb_ptr, UIM_SUSPICIOUS_CARD_REM_SIG);
            }
            (void) rex_clr_sigs( uim_ptr->tcb_ptr, UIM_CMD_RSP_TIMEOUT_SIG );
            (void) rex_clr_sigs( uim_ptr->tcb_ptr, UIM_TRANSACTION_SIG );
            return TRUE;
          }
        }
      }
    }
  }

  /* clear the signal as well just in case if it was set */
  (void) rex_clr_sigs( uim_ptr->tcb_ptr, UIM_TRANSACTION_SIG );

  if(uim_nv_is_feature_enabled(UIMDRV_FEATURE_DISABLE_RECOVERY_UPON_INFINITE_NULL,
                               uim_ptr) == TRUE)
  {
    if((orig_sig_mask & UIM_TRANSACTION_SIG)!= 0)
    {
      /* Check UIM is in middle of processing of any command */
      if (TRUE == uim_ptr->flag.command_in_progress)
      {
        /*
          1. We will need to report at this point only for an
             external command that was not re-queued.
          2. If the external command was re-queued, then in
             uim_flush_commands(), we do a q_get() and then report
             for the re-queued command.
          This check for uim_command_requeued avoids double reporting.
         */
        if ((!uim_ptr->flag.static_buffer_used)
            &&(!uim_ptr->flag.command_requeued)
            && uim_ptr->command.cmd_ptr != NULL)
        {
          uim_ptr->command.static_cmd_buf.hdr.slot = uim_ptr->command.cmd_ptr->hdr.slot;
         /*reset the command transacted flag as response 
           is not succesfully received from card*/
         uim_ptr->command.rpt_buf.cmd_transacted = FALSE;

          /* Sending error report for the current command */
          uim_send_err_rpt(uim_ptr->command.cmd_ptr, uim_ptr);
        }
      }
      /* Set the command in progress flag to FALSE */
      uim_ptr->flag.command_in_progress = FALSE;

      /* Toggle the poll pending param not to proceed any poll pending */
      uim_ptr->flag.poll_pending = FALSE;
      /* set the status to ERROR */
      uim_ptr->state.status = UIM_ERR_S;
      /* set the command count to MAX */
      uim_ptr->command.static_cmd_buf.hdr.cmd_count = UIM_MAX_NUM_ATTEMPTS;
      /* Flush all pending command */
      uim_flush_command(UIM_FLUSH_ALL, uim_ptr);
      /* UIM power down */
      uim_power_down(uim_ptr);
      /* Notify the higher layers that UIM is declared faulty */
      uim_notify_error(UIM_NULL_BYTE_ERR_S, uim_ptr);
      /* Reset all the flags and include the imask */
      (*imask_ptr) |= UIM_CMD_Q_SIG;
      /* Reset the internal command used flag */
      uim_ptr->flag.internal_command_used = FALSE;
      /* Reset the static_buffer */
      uim_ptr->flag.static_buffer_used = FALSE;

      UIMDRV_PUP_MSG_ERR_0(UIMLOG_MSG_190,uim_ptr->id, "Shut down UIM due to the infinite NULL responses");

      /* Go back to the top of the main loop */
      return TRUE;
    }
  }

  UIMDRV_PUP_MSG_ERR_0(UIMLOG_MSG_191,uim_ptr->id, "Timed out on the command response");
  (void) rex_clr_sigs( uim_ptr->tcb_ptr, UIM_CMD_RSP_TIMEOUT_SIG );
  (void) rex_clr_sigs( uim_ptr->tcb_ptr, UIM_TRANSACTION_SIG );

#if defined( FEATURE_UIM_T_1_SUPPORT )
  if (UIM_T_1_PROTOCOL == uim_ptr->state.current_protocol)
  {
    /* For T=1, the handling of CWT timeout does not lead to
       power-down of the SIM interface always. */
    uim_ptr->flag.clear_timeout_on_response = TRUE;
    if ( uim_can_ignore_timeout(orig_sig_mask, uim_ptr) )
    {
      /* This is a spurious timeout signal.  We have bytes in the FIFO
         Return from this point as though nothing happened */
      UIMDRV_MSG_HIGH_0(uim_ptr->id,"Ignoring the spurious timeout signal for T=1");

      /* Go back to the top of the main loop */
      return TRUE;
    }

    /* Process the timeout for the T=1 protocol */
    if (uim_process_t_1_timeout(uim_ptr))
    {
      /* The timeout has been processed.  No need to process it
         further here */
      /* Go back to the top of the main loop */
      return TRUE;
    } /* end if - the timeout has been processed by T=1 code */
  } /* end if - operating in the T=1 protocol */
#endif /* FEATURE_UIM_T_1_SUPPORT */

  if ( uim_ptr->flag.max_parity_error   ||
       uim_ptr->flag.max_rx_break_error ||
       uim_ptr->flag.max_overrun_error )
  {
    /* We need to reset the interface so clear the values
     * for these error conditions since they would no longer
     * be right and could interfere in the sub-sequent time-out
     */
    uim_ptr->flag.overrun_error          = FALSE;
    uim_ptr->flag.bad_status_words_error = FALSE;
  }
  /* If an overrun error is encountered or bad status words
   * are received
   */
  if ( uim_ptr->flag.overrun_error ||
      (TRUE == uim_ptr->flag.bad_status_words_error &&
       FALSE == uim_nv_is_feature_enabled(UIMDRV_FEATURE_RECOVERY_ON_BAD_STATUS_WORD, uim_ptr)))
  {
    if ( uim_ptr->flag.overrun_error )
    {
      UIMDRV_PUP_MSG_ERR_0(UIMLOG_MSG_192,uim_ptr->id,"Processing UART Overrun Error in UIM task");
    }
    if ( uim_ptr->flag.bad_status_words_error )
    {
      UIMDRV_PUP_MSG_ERR_0(UIMLOG_MSG_193,uim_ptr->id,"Processing reception of bad status words. not triggering recovery");
    }

    /* Process the status as a command error */
    uim_process_cmd_err(imask_ptr, uim_ptr);

    /* Set the flag to FALSE */
    uim_ptr->flag.overrun_error          = FALSE;
    uim_ptr->flag.bad_status_words_error = FALSE;
  }
  else
  {
    /* Reset and try to recover from these error cases */
    if ( uim_ptr->flag.max_parity_error   ||
         uim_ptr->flag.max_rx_break_error ||
         uim_ptr->flag.max_overrun_error ||
         uim_ptr->flag.bad_status_words_error)
    {
      UIMDRV_MSG_ERR_4(uim_ptr->id,"Reset due to Parity error-0x%x, rx break error-0x%x, overrun error-0x%x, bad status word-0x%x",
                           uim_ptr->flag.max_parity_error,
                           uim_ptr->flag.max_rx_break_error,
                           uim_ptr->flag.max_overrun_error,
                           uim_ptr->flag.bad_status_words_error);


      /* Clear all the flags irrespective of which one
       * got us here
       */
      uim_ptr->flag.max_parity_error   = FALSE;
      uim_ptr->flag.max_rx_break_error = FALSE;
      uim_ptr->flag.max_overrun_error  = FALSE;
      uim_ptr->flag.bad_status_words_error = FALSE;
      uim_ptr->flag.interface_gone_bad = TRUE;
    }
    else if ( uim_can_ignore_timeout(orig_sig_mask, uim_ptr) )
    {
      /* This is a spurious timeout signal.  We have bytes in the FIFO
         Return from this point as though nothing happened */
      UIMDRV_PUP_MSG_HIGH_0(UIMLOG_MSG_198,uim_ptr->id,"Ignoring the spurious timeout signal for T=0");
      /* Go back to the top of the main loop */
      return TRUE;
    }

    /* Any timeout while in passive mode would be reported to the client
     * both internal or external commands
     */
    if (uim_ptr->command.mode == UIM_PASSIVE_MODE)
    {
      /* Set the flag to false since the command is completed */
      uim_ptr->flag.command_in_progress = FALSE;

      /* Setting the status to ERROR prior to invoking power down.
         While shutting the LDO down we evaluate the uim status to
         cater to the user customized LDO settings if needed. */
      uim_ptr->state.status = UIM_ERR_S;

      uim_power_down(uim_ptr);

      /* Clear out any votes to keep UIM powered on */
      uim_ptr->state.ctl = UIM_NONE;

      uim_ptr->flag.powerdown = TRUE;

      /* internal command timed-out for max attempts */

      /*Send the Error Report*/
      if(!uim_ptr->flag.static_buffer_used && uim_ptr->command.cmd_ptr != NULL)
      {
        /*reset the command transacted flag as response 
        is not succesfully received from card*/
        uim_ptr->command.rpt_buf.cmd_transacted = FALSE;

        uim_send_err_rpt(uim_ptr->command.cmd_ptr, uim_ptr);
      }

      /* flush all the commands in the command queue */
      uim_flush_command(UIM_FLUSH_ALL, uim_ptr);

      /* Notify the higher layers that UIM is declared faulty */
      uim_notify_error(UIM_CMD_TIMED_OUT_IN_PASSIVE_MODE_S, uim_ptr);

      /* Whenver we receive a command, send an error report as the
      SIM is to be powered down */
      (*imask_ptr) |= UIM_CMD_Q_SIG;

      /* reset the directory struct and power down to UIM */
      uim_reset_dir(uim_ptr);

      /* Reset the static buffer used flag */
      uim_ptr->flag.static_buffer_used = FALSE;

      /* Reset the static command count */
      uim_ptr->command.static_cmd_buf.hdr.cmd_count = 0;
    }
    else if ((uim_ptr->flag.static_buffer_used) && uim_ptr->command.static_cmd_buf.hdr.command == UIM_INTERNAL_FETCH_F)
    {
      UIMDRV_PUP_MSG_HIGH_0(UIMLOG_MSG_199,uim_ptr->id,"Timeout on an internal fetch");
      /* Set the flag to false since the command is completed */
      uim_ptr->flag.command_in_progress = FALSE;
      /* Reset the internal command used flag */
      uim_ptr->flag.internal_command_used = FALSE;
      /* Reset static buffer used*/
      uim_ptr->flag.static_buffer_used = FALSE;
      uim_ptr->flag.command_requeued = FALSE;
      uim_reset_uim (imask_ptr, uim_ptr->flag.me_powerup,uim_ptr);
    }
    /* If we are in the middle of processing an external command */
    else if (!uim_ptr->flag.static_buffer_used)
    {
      UIMDRV_PUP_MSG_HIGH_0(UIMLOG_MSG_200,uim_ptr->id, "UIM timeout in external command");

      /* Save GPIO and clock state in globals */
      uim_save_gpio_info(uim_ptr, FALSE);

      if (uim_ptr->command.cmd_ptr != NULL)
      {
        /* This makes sure that we do not send the status again to
           the task that sent the command */
        uim_ptr->command.cmd_ptr->hdr.cmd_hdr.task_ptr = NULL;

        /* Reset requeue counter to 1 as UIM is entering recovery */
        uim_ptr->command.command_requeue_counter = 1;

        /* When recovery is triggered during UIM_VERIFY_CHV_F command and it
           should not be retried as it drops the number of incorrect pin attempts
           if the pin is entered wrong.
           In general we should not retry for a VERIFY CHV command and hence we
           send an error report to MMGSDI and we prompt the user again.
           All the pin related commands such as CHANGE, ENABLE, VERIFY, UNBLOCK,
           DISABLE are not retried and an error report is sent to MMGSDI. */

         if (uim_ptr->command.cmd_ptr->hdr.command != UIM_RESET_F          &&
             uim_ptr->command.cmd_ptr->hdr.command != UIM_CHANGE_CHV_F     &&
             uim_ptr->command.cmd_ptr->hdr.command != UIM_DISABLE_CHV_F    &&
             uim_ptr->command.cmd_ptr->hdr.command != UIM_ENABLE_CHV_F     &&
             uim_ptr->command.cmd_ptr->hdr.command != UIM_AUTHENTICATE_F   &&
           (((uim_ptr->command.cmd_ptr->hdr.command != UIM_VERIFY_CHV_F)    ^   
            (uim_ptr->command.cmd_ptr->hdr.command == UIM_VERIFY_CHV_F     &&
             uim_ptr->command.cmd_ptr->verify_chv.chv_digits[0] == '\0'))  &&
           ((uim_ptr->command.cmd_ptr->hdr.command != UIM_UNBLOCK_CHV_F)   ^
            (uim_ptr->command.cmd_ptr->hdr.command == UIM_UNBLOCK_CHV_F    && 
             uim_ptr->command.cmd_ptr->unblk_chv.chv_digits[0] == '\0' ))) && 
             /* Do not re-attempt this comamnd if it so end-up infinite loop */
             uim_ptr->command.cmd_ptr->hdr.command != UIM_SILENT_RECOVER_UICC_F)
        {
          uim_ptr->flag.command_requeued = TRUE;
 
          if(uim_ptr->command.cmd_ptr->select.reselect.is_failed_select_reselect == TRUE)
          {
            UIMDRV_MSG_HIGH_0(uim_ptr->id,"Re-Storing the actual path received in external cmd");
            uim_ptr->command.cmd_ptr->select.path.len = uim_ptr->command.cmd_ptr->select.reselect.original_select_path.len;
            uim_ptr->command.cmd_ptr->select.file_type = uim_ptr->command.cmd_ptr->select.reselect.original_file_type;
            for(i = 0; i < uim_ptr->command.cmd_ptr->select.reselect.original_select_path.len; i++)
            {
              uim_ptr->command.cmd_ptr->select.path.path[i] = uim_ptr->command.cmd_ptr->select.reselect.original_select_path.path[i];
            }
            uim_ptr->command.cmd_ptr->select.reselect.is_failed_select_reselect = FALSE;
          }
          /* Put the command back in the head of the queue so that it can be
            processed again */
          if ( q_cnt( &uim_ptr->task.cmd_q ) == 0 )
          {
            q_put( &uim_ptr->task.cmd_q, &uim_ptr->command.cmd_ptr->hdr.cmd_hdr.link );
          }
          else
          {
            q_insert( & uim_ptr->task.cmd_q,
                      & uim_ptr->command.cmd_ptr->hdr.cmd_hdr.link,
                      & ((uim_cmd_type*)q_check( &uim_ptr->task.cmd_q ))->hdr.cmd_hdr.link);
          }

          /* Increment the number of times the command is executed
           * instead of flushing right away, it would be flushed when it is
           * taken out of the que for processing to avoid giving an error
           * response while in recovery mode
           */
          ++uim_ptr->command.cmd_ptr->hdr.cmd_recovery_count;
        }
        else
        {
          if(uim_ptr->command.cmd_ptr->hdr.command == UIM_RESET_F)
          {

            if (uim_ptr->state.max_possible_voltage_class == uim_ptr->state.current_voltage_class
                ||(IS_BTSAP_HANDLE_ACTIVE(uim_ptr))
               )
            {
              /* Set UIM signal mask to start accepting commands*/
              (*imask_ptr) |= UIM_CMD_Q_SIG;
            }

            /* If RESET_F times out when hotswap is enabled and if card is inserted then
                     we need to try powering up on the next votage class by calling uim_reset_uim */
            UIMDRV_PUP_MSG_HIGH_0(UIMLOG_MSG_202,uim_ptr->id,"Reset_F timedout.Report to client and try a fresh powerup");
            uim_process_generic_response(&uim_ptr->command.rsp_buf, uim_ptr);
            uim_ptr->flag.command_in_progress = FALSE;
            uim_ptr->state.status = UIM_UNINITIALIZED_S;
          }
          else if(uim_ptr->command.cmd_ptr->hdr.command == UIM_VERIFY_CHV_F  ||
                  uim_ptr->command.cmd_ptr->hdr.command == UIM_CHANGE_CHV_F  ||
                  uim_ptr->command.cmd_ptr->hdr.command == UIM_DISABLE_CHV_F ||
                  uim_ptr->command.cmd_ptr->hdr.command == UIM_ENABLE_CHV_F  ||
                  uim_ptr->command.cmd_ptr->hdr.command == UIM_AUTHENTICATE_F  ||
                  uim_ptr->command.cmd_ptr->hdr.command == UIM_UNBLOCK_CHV_F)
          {
            /* When recovery is triggered during UIM_VERIFY_CHV_F command and it
               should not be retried as it drops the number of incorrect pin attempts
               if the pin is entered wrong.
               In general we should not retry for a VERIFY CHV command and hence we
               send an error report to MMGSDI and we prompt the user again.
               All the pin related commands such as CHANGE, ENABLE, VERIFY, UNBLOCK,
               DISABLE are not retried and an error report is sent to MMGSDI. */
           UIMDRV_PUP_MSG_HIGH_1(UIMLOG_MSG_201,uim_ptr->id,"UIM Command Re-queue did not happen for cmd = 0x%x",
                                 uim_ptr->command.cmd_ptr->hdr.command);

            /*reset the command transacted flag as response 
            is not succesfully received from card*/
            uim_ptr->command.rpt_buf.cmd_transacted = FALSE;
 
            uim_send_err_rpt(uim_ptr->command.cmd_ptr, uim_ptr);
          }
          /* Added for SILENT RECOVERY */
          else if (UIM_SILENT_RECOVER_UICC_F == uim_ptr->command.cmd_ptr->hdr.command)
          {
            UIMDRV_PUP_MSG_HIGH_0(UIMLOG_MSG_205,uim_ptr->id,"UIM SILENT Recovery ");
            /*Copying command buffer to report buffer to have it freed */
            uim_ptr->command.rpt_buf.user_data = uim_ptr->command.cmd_ptr->hdr.user_data;
            uim_ptr->command.cmd_ptr->hdr.rpt_function(&uim_ptr->command.rpt_buf);
            /*Start Processing the next command */
            (*imask_ptr) |= UIM_CMD_Q_SIG;
          }
        }
      }  /* uim_ptr->command.cmd_ptr != NULL */

      /* reset the card and we do not want to include the command q sig */
      UIMDRV_PUP_MSG_HIGH_2(UIMLOG_MSG_206,uim_ptr->id,"Reset after timeout Rx-state 0x%x Tx-state 0x%x",
                        uim_ptr->rxtx_state_machine.rx_state,
                        uim_ptr->rxtx_state_machine.tx_state);

      uim_reset_uim (imask_ptr, uim_ptr->flag.me_powerup,uim_ptr);
      /*Poll timer is not restarted after recovery when UIMDRV_FEATURE_UIM_POLLING_ONLY_AT_POLLING_TIMER_EXPIRY
        featrure is enabled, restartin the timer here after recovery */
      if(uim_nv_is_feature_enabled(UIMDRV_FEATURE_UIM_POLLING_ONLY_AT_POLLING_TIMER_EXPIRY, uim_ptr) == TRUE)
      {
        uim_set_poll_timer( uim_ptr->setting.poll_time_delay, uim_ptr->setting.wakeup_poll_offset, uim_ptr);
      }
    }  /* if it was an internal command */
    else
    {
      UIMDRV_PUP_MSG_HIGH_0(UIMLOG_MSG_207,uim_ptr->id, "UIM timeout in internal command");

      /* Save GPIO and clock state in globals */
      uim_save_gpio_info(uim_ptr, TRUE);

      if (uim_ptr->atr.atr_pps_done)
      {
        UIMDRV_PUP_MSG_HIGH_0(UIMLOG_MSG_209,uim_ptr->id, "ATR_PPS Done");

        if(uim_nv_is_feature_enabled(UIMDRV_FEATURE_NO_SWITCH_INST_ON_WWT_EXPIRY,
                                     uim_ptr) == TRUE)
        {
          if (uim_ptr->command.static_cmd_buf.hdr.command == UIM_INTERNAL_ME_PUP_F ||
              uim_ptr->command.static_cmd_buf.hdr.command == UIM_HOTSWAP_CARD_INS_F ||
              uim_ptr->command.static_cmd_buf.hdr.command == UIM_INTERNAL_WAKE_UP_F)
          {
            uim_reset_uim (imask_ptr, uim_ptr->flag.me_powerup,uim_ptr);
            uim_set_poll_timer( uim_ptr->setting.poll_time_delay,
                                uim_ptr->setting.wakeup_poll_offset,
                                uim_ptr);
          }
        }
        else if( TRUE == uim_ptr->flag.interface_gone_bad )
        {
          UIMDRV_PUP_MSG_HIGH_0(UIMLOG_MSG_210,uim_ptr->id,"Timeout happened on interface error"
                                         "interface_gone_bad ");
          uim_reset_uim (imask_ptr, uim_ptr->flag.me_powerup,uim_ptr);
          uim_set_poll_timer( uim_ptr->setting.poll_time_delay,
                              uim_ptr->setting.wakeup_poll_offset,
                              uim_ptr);
        }
        else /* ! UIMDRV_FEATURE_NO_SWITCH_INST_ON_WWT_EXPIRY */
        {
          if(((uim_hw_if.intctrl[uim_ptr->id].m_hs_enable == TRUE
               && (uim_ptr->command.static_cmd_buf.hdr.command == UIM_INTERNAL_ME_PUP_F
                   || uim_ptr->command.static_cmd_buf.hdr.command == UIM_HOTSWAP_CARD_INS_F))
              || (uim_hw_if.intctrl[uim_ptr->id].m_hs_enable == FALSE
                  && uim_ptr->command.static_cmd_buf.hdr.command == UIM_INTERNAL_ME_PUP_F))
             && (uim_ptr->command.static_cmd_buf.hdr.cmd_count == 1)
             && (uim_ptr->state.first_inst_class == NV_UIM_FIRST_INST_CLASS_UMTS_SIM)
  #if defined( FEATURE_UIM_T_1_SUPPORT )
              && (UIM_T_1_PROTOCOL != uim_ptr->state.current_protocol)
  #endif /* FEATURE_UIM_T_1_SUPPORT */
             )
          {
            /* Power down the interface to force a cold reset */
            uim_power_down(uim_ptr);
            if (!uim_ptr->flag.invalid_pps_received)
            {
              /* Change the instruction class to GSM. */
              uim_ptr->flag.toggle_instrn_class = TRUE;
              if(UIM_IS_CLK_FREQ_AT_4_8_MHZ(uim_ptr->id))
              {
                UIMDRV_PUP_MSG_HIGH_0(UIMLOG_MSG_211,uim_ptr->id, "Toggling INST CLA and dropping freq to 3.8MHz");
                UIM_SWITCH_CLK_FREQ_TO_3_8_MHZ(uim_ptr->id);
                SET_UIM_BAUD_RATE_SLOT ( uim_ptr->id, UIM_CRCF_372_1, UIM_BRAF_1 );
              }
            }
          }
          else
          {
            uim_reset_uim (imask_ptr, uim_ptr->flag.me_powerup,uim_ptr);
            uim_set_poll_timer( uim_ptr->setting.poll_time_delay,
                                uim_ptr->setting.wakeup_poll_offset,
                                uim_ptr);
          }
        }/* !UIMDRV_FEATURE_NO_SWITCH_INST_ON_WWT_EXPIRY */
      } /*atr pps done */
     else if (!uim_ptr->atr.atr_received)/* No ATR received */
     {
        /* Check if there is a higher voltage class */
        if(uim_ptr->command.static_cmd_buf.hdr.command == UIM_INTERNAL_WAKE_UP_F)
        {
          uim_reset_uim (imask_ptr, uim_ptr->flag.me_powerup,uim_ptr);
          uim_set_poll_timer( uim_ptr->setting.poll_time_delay,
                              uim_ptr->setting.wakeup_poll_offset,
                              uim_ptr);
        }
        else
        {
          if (uim_ptr->state.max_possible_voltage_class == uim_ptr->state.current_voltage_class)
          {
            if(UIM_IS_CLK_FREQ_AT_4_8_MHZ(uim_ptr->id))
            {
              UIMDRV_PUP_MSG_HIGH_0(UIMLOG_MSG_212,uim_ptr->id, "Trying 3.84MHz CLK freq as a safety measure for old cards");
              uim_power_down(uim_ptr);
              uim_hotswap_reset_slot(uim_ptr);
              UIM_SWITCH_CLK_FREQ_TO_3_8_MHZ(uim_ptr->id);
              SET_UIM_BAUD_RATE_SLOT ( uim_ptr->id, UIM_CRCF_372_1, UIM_BRAF_1 );
              uim_reset_uim (imask_ptr, uim_ptr->flag.me_powerup,uim_ptr);
              /* Go back to the top of the main loop */
              return TRUE;
            }
            else
            {
              /* There are no more voltages to try */
              /* Clk Freq is at 3.84MHz, the lowest possible freq */
              /* Stop interfacing with the card */
              uim_ptr->command.static_cmd_buf.hdr.cmd_count = UIM_MAX_NUM_ATTEMPTS;
            }
          }
          else if (uim_ptr->state.current_voltage_class < uim_ptr->state.max_possible_voltage_class )
          {
            /* There are more voltages to try */
            /* Power down the interface */
            uim_power_down(uim_ptr);
            UIMDRV_PUP_MSG_HIGH_0(UIMLOG_MSG_213,uim_ptr->id, "Attempting power up at 3V");
            uim_timed_sleep(UIM_ISO7816_VOLTAGE_SWITCH_PWR_DOWN_DELAY,
                            uim_ptr,
                            UIM_ALL_ZERO_SIG);
            /* Try the next voltage class */
            uim_ptr->state.current_voltage_class++;
            /* Reset the error count */
            uim_ptr->command.static_cmd_buf.hdr.cmd_count = 0;
          } /* end if - there are no more voltages to try */
        }  /* uim_ptr->command.static_cmd_buf.hdr.command == UIM_INTERNAL_WAKE_UP_F */
      } /* No ATR received */
      else if(uim_ptr->atr.atr_received &&
              !uim_ptr->atr.atr_pps_done)
      {
        if((uim_ptr->flag.interface_gone_bad == TRUE) ||
            (uim_ptr->command.static_cmd_buf.hdr.command == UIM_INTERNAL_WAKE_UP_F))
        {
          uim_reset_uim (imask_ptr, uim_ptr->flag.me_powerup, uim_ptr);
          UIMDRV_PUP_MSG_HIGH_0(UIMLOG_MSG_214,uim_ptr->id,"Bad PPS in WAKE_UP_F -> Resetting + UIM POLL timer reset" );
          uim_set_poll_timer( uim_ptr->setting.poll_time_delay,
                              uim_ptr->setting.wakeup_poll_offset,
                              uim_ptr);
        }
        else
        {
          if(uim_ptr->atr.no_pps_attempts >= UIM_MAX_NUM_ATTEMPTS)
          {
            uim_ptr->command.static_cmd_buf.hdr.cmd_count = UIM_MAX_NUM_ATTEMPTS; 
          }
        }
      }
      if (++uim_ptr->command.static_cmd_buf.hdr.cmd_count > UIM_MAX_NUM_ATTEMPTS)
      {
        UIMDRV_PUP_MSG_HIGH_0(UIMLOG_MSG_215,uim_ptr->id,"Entering UIM_MAX_NUM_ATTEMPTS" );
        (void) rex_clr_sigs( uim_ptr->tcb_ptr, UIM_CMD_RSP_SIG);
        if (uim_ptr->atr.atr_pps_done)
        {
          /* internal command failed after the PPS procedure */
          notification_reason = UIM_INTERNAL_CMD_TIMED_OUT_AFTER_PPS_S;
        }
        else if ( !uim_ptr->atr.atr_received )
        {
          /* We have not yet received the ATR */
          if ( uim_ptr->command.static_cmd_buf.hdr.command == UIM_INTERNAL_ME_PUP_F )
          {
            notification_reason = UIM_NO_ATR_RECEIVED_WITH_MAX_VOLTAGE_S;
          }
          else
          {
            notification_reason = UIM_NO_ATR_RECEIVED_AFTER_INT_RESET_S;
          }
        }
        else
        {
          /* Atr is received but PPS procedure is not done and we are done
           * with all the re-tries
           */
          notification_reason = UIM_PPS_TIMED_OUT_MAX_TIMES_S;
        }

        /* Set the flag to false since the command is completed */
        uim_ptr->flag.command_in_progress = FALSE;

      /* Setting the status to ERROR prior to invoking power down.
         While shutting the LDO down we evaluate the uim status to
         cater to the user customized LDO settings if needed. */
        uim_ptr->state.status = UIM_ERR_S;

        uim_power_down(uim_ptr);

        /* Clear out any votes to keep UIM powered on */
        uim_ptr->state.ctl = UIM_NONE;

        uim_ptr->flag.powerdown = TRUE;

        /* flush all the commands in the command queue */
        uim_flush_command(UIM_FLUSH_ALL, uim_ptr);

        if((uim_ptr->extended_recovery.extended_recovery_enabled == TRUE &&
               uim_ptr->command.static_cmd_buf.hdr.command == UIM_INTERNAL_WAKE_UP_F)  ||
           (uim_ptr->extended_recovery.extended_recovery_enabled == TRUE &&
               uim_ptr->extended_recovery.extended_recovery_in_progress == TRUE &&
               uim_ptr->command.static_cmd_buf.hdr.command == UIM_INTERNAL_ME_PUP_F)   )
        {
           /*1. all recoveries exhausted kick start the extended recovery
             2. extended powerup retry failed, check if further attempts are left and start the timer
             3. Donot kick start for internal_me_powerup failure*/
          uim_extended_recovery_event_handler(uim_ptr, UIM_EXTENDED_RECOVERY_EVT_TIMER_START);
        }

        /* Notify the higher layers that UIM is declared faulty */
        uim_notify_error(notification_reason, uim_ptr);

        /* Whenver we receive a command, send an error report as the
        SIM is to be powered down */
        (*imask_ptr) |= UIM_CMD_Q_SIG;

        uim_ptr->flag.me_powerup = FALSE;
        /* reset the directory struct and power down to UIM */
        uim_reset_dir(uim_ptr);

        /* Reset the static buffer used flag */
        uim_ptr->flag.static_buffer_used = FALSE;

        /* Reset the static command count */
        uim_ptr->command.static_cmd_buf.hdr.cmd_count = 0;
        /* Reset recovery triggered flag */
        uim_ptr->flag.recovery_triggered = FALSE;
        if(uim_ptr->command.mode == UIM_RECOVERY_MODE)
        {
          uim_ptr->command.mode = UIM_NORMAL_MODE;
        }
      }
      else
      {
        /* One slot only */
        if(!uim_ptr->flag.recovery_triggered)
        {
          /* Power down the interface to force a cold reset */
          uim_power_down(uim_ptr);
          uim_timed_sleep(UIM_ISO7816_VOLTAGE_SWITCH_PWR_DOWN_DELAY,
                          uim_ptr,
                          UIM_ALL_ZERO_SIG);
          UIMDRV_PUP_MSG_HIGH_2(UIMLOG_MSG_216,uim_ptr->id,"Cold Reset after timeout Rx-state 0x%x Tx-state 0x%x",
                            uim_ptr->rxtx_state_machine.rx_state,
                            uim_ptr->rxtx_state_machine.tx_state);
          uim_reset_uim (imask_ptr, uim_ptr->flag.me_powerup, uim_ptr);
          uim_set_poll_timer(uim_ptr->setting.poll_time_delay,
                             uim_ptr->setting.wakeup_poll_offset,
                             uim_ptr);          
        }
      }
    }  /* internal or external command */
  } /* uim_overrun_error */
  /* Resetting uim_interface_gone_bad to FALSE
     after processing cmd rsp timeout sig */
  uim_ptr->flag.interface_gone_bad = FALSE;

  return FALSE;
} /* uim_handle_cmd_rsp_timeout_sig */

#ifdef FEATURE_SUPPORTS_NON_REMOVABLE_UICC
#error code not present
#endif

/**
*  This function handles the UIM_SIMULATE_NULL_TIMER_EXP_SIG signal.
*
*
*  @param sig_mask_ptr Pointer to the OR's set of signals that UIM task needs to
                       handle.  Unused in this function.
*  @param imask_ptr Pointer to the mask of signals that the uim task is interested
                    in handling.  Unused in this function.
*  @param uim_ptr Pointer to the current instance of the UIM global structure
*  @return boolean Return TRUE to indicate that immediately
*          going back to the top of the main loop is desired.
*/
static boolean uim_handle_simulate_null_timer_exp_sig(rex_sigs_type *sig_mask_ptr,
                                                      rex_sigs_type *imask_ptr,
                                                      uim_instance_global_type *uim_ptr)
{
  (void)sig_mask_ptr;
  (void)imask_ptr;
  
  /* Simulate receiving response after uim_simulate_null_timer_val_s sec of NULL bytes */
  uim_simulate_null_timer_state_handler(uim_ptr, UIM_BUSY_RESPONSE_EVT_SIMULATE_NULL_TIMER_EXP_SIG);
  return FALSE;
} /* uim_handle_simulate_null_timer_exp_sig */


/**
*  This function handles the UIM_BUSY_IND_TIMER_EXP_SIG signal.
*
*  Process busy_ind_timer before trans_timer to prevent the
*  possibility of notify in twice of the uim_busy_ind_timer_val.
*
*
*  @param sig_mask_ptr Pointer to the OR's set of signals that UIM task needs to
*                      handle.  Unused in this function.
*  @param imask_ptr Pointer to the mask of signals that the uim task is interested
*                   in handling.  Unused in this function.
*  @param uim_ptr Pointer to the current instance of the UIM global structure
*  @return boolean Return TRUE to indicate that immediately
*          going back to the top of the main loop is desired.
*/
static boolean uim_handle_busy_ind_timer_exp_sig(rex_sigs_type *sig_mask_ptr,
                                                 rex_sigs_type *imask_ptr,
                                                 uim_instance_global_type *uim_ptr)
{
  /* Notify uim busy to mmgsdi */
  uim_busy_ind_timer_state_handler(uim_ptr, UIM_BUSY_RESPONSE_EVT_BUSY_IND_TIMER_EXP_SIG);
  return FALSE;
} /* uim_handle_busy_ind_timer_exp_sig */


/**
*  This function handles the UIM_TRANS_TIMER_EXP_SIG signal.
*
*
*  @param sig_mask_ptr Pointer to the OR's set of signals that UIM task needs to
*                      handle.  Unused in this function.
*  @param imask_ptr Pointer to the mask of signals that the uim task is interested
*                   in handling.  Unused in this function.
*  @param uim_ptr Pointer to the current instance of the UIM global structure
*  @return boolean Return TRUE to indicate that immediately
*          going back to the top of the main loop is desired.
*/
static boolean uim_handle_trans_timer_exp_sig(rex_sigs_type *sig_mask_ptr,
                                              rex_sigs_type *imask_ptr,
                                              uim_instance_global_type *uim_ptr)
{
  (void)sig_mask_ptr;
  (void)imask_ptr;
  
  /* Trigger force recovery */
  uim_trans_timer_state_handler(uim_ptr, UIM_BUSY_RESPONSE_EVT_TRANS_TIMER_EXP_SIG);
  return FALSE;
} /* uim_handle_trans_timer_exp_sig */


/**
*  This function handles the UIM_MCGF_NV_REFRESH_SIG signal.
*
*  Refresh the NV and EFS items that are carrier specific.
*
*
*  @param sig_mask_ptr Pointer to the OR's set of signals that UIM task needs to
*                      handle.  Unused in this function.
*  @param imask_ptr Pointer to the mask of signals that the uim task is interested
*                   in handling.  Unused in this function.
*  @param uim_ptr Pointer to the current instance of the UIM global structure
*  @return boolean Return TRUE to indicate that immediately
*          going back to the top of the main loop is desired.
*/
static boolean uim_handle_mcgf_nv_refresh_sig(rex_sigs_type *sig_mask_ptr,
                                              rex_sigs_type *imask_ptr,
                                              uim_instance_global_type *uim_ptr)
{
  (void)sig_mask_ptr;
  (void)imask_ptr;
  
  /* Clear the sig */
  (void) rex_clr_sigs(uim_ptr->tcb_ptr, UIM_MCGF_NV_REFRESH_SIG );

  /* Invoke NV refresh procedures here */
  uimdrv_read_mcfg_refreshed_nv(uim_ptr);

#ifdef FEATURE_MODEM_CONFIG_REFRESH
  /* Call MCFG call back that the refresh is done */
  (void)mcfg_refresh_done_w_status(uim_ptr->state.nv_refresh_reference_id,
                                   MCFG_REFRESH_SUCCESS);
#endif /* FEATURE_MODEM_CONFIG_REFRESH */
  /* Do not need to return to the top immediately */
  return FALSE;
} /* uim_handle_mcgf_nv_refresh_sig */

static boolean uim_handle_extended_recovery_timer_exp_sig(rex_sigs_type *sig_mask_ptr,
                                              rex_sigs_type *imask_ptr,
                                              uim_instance_global_type *uim_ptr)
{
  (void)sig_mask_ptr;
  (void)imask_ptr;
  
  (void) rex_clr_sigs( uim_ptr->tcb_ptr, UIM_EXT_RECOVERY_TIMER_EXP_SIG);
  /*extended recovery timer expierd. post a fresh powerup command*/
  UIMDRV_MSG_HIGH_0(uim_ptr->id, "uim_handle_enhanced_recovery_timer_exp_sig");
  uim_extended_recovery_event_handler(uim_ptr, UIM_EXTENDED_RECOVERY_EVT_TIMER_EXP);
  uim_reset_uim(imask_ptr, uim_ptr->flag.me_powerup, uim_ptr);
  return FALSE;
} /* uim_handle_extended_recovery_timer_exp_sig */


/**
*  This procedure returns TRUE if Reset cmd is not allowed there
*  else it return false.
*  If the reset command is allowed, this function also does
*  some pre-processing like resetting the globals.
*
*
*  @param uim_ptr Pointer to the current instance of the UIM global structure
*
*  @return boolean returns TRUE if Reset cmd is not allowed
*/
boolean uim_is_reset_cmd_not_allowed(uim_instance_global_type *uim_ptr)
{
  if(uim_ptr->id >= UIM_MAX_INSTANCES)
  {
    return TRUE;
  }

  if(uim_hw_if.intctrl[uim_ptr->id].m_hs_enable == FALSE)
  {
    if(uim_ptr->command.cmd_ptr->hdr.command   == UIM_RESET_F &&
       uim_ptr->command.cmd_ptr->hdr.cmd_count == 0 &&
       uim_ptr->state.status == UIM_ERR_S)
    {
      /* When hotswap is disabled, clear globals upon AP mode OFF
         for the 1st time. When the RESET_F is re-queued for switching class/protocol
         we should not clear the globals */
      uim_reset_globals(uim_ptr);
      uimdrv_iso_reconfig_uartdm(uim_ptr);
      if ( uim_ptr->hotswap.hotswap_first_ins_class == NV_UIM_FIRST_INST_CLASS_GSM_SIM ||
           uim_ptr->command.cmd_ptr->hdr.protocol   == UIM_ICC )
      {
        uim_ptr->command.cmd_ptr->hdr.protocol = UIM_ICC;
      }
      else
      {
        uim_ptr->command.cmd_ptr->hdr.protocol = UIM_UICC;
      }

      /* Change the card mode based on the reset_f param */
      uim_sam_change_mode (uim_ptr);
    }
    return FALSE;
  }

  if (((uim_ptr->command.cmd_ptr->hdr.command == UIM_RESET_F) 
       ||(uim_ptr->command.cmd_ptr->hdr.command == UIM_RESET_SWITCH_UIM_PASSIVE_F)
       ||(uim_ptr->command.cmd_ptr->hdr.command == UIM_POWER_UP_UIM_PASSIVE_F)) &&
      (uim_hw_if.intctrl[uim_ptr->id].m_hs_enable == TRUE))
  {
    /* Enable the hot swap interrupt*/
    UIMDRV_PUP_MSG_HIGH_1(UIMLOG_MSG_217,uim_ptr->id,"UIM received the RESET F command with protocol 0x%x",
                      uim_ptr->command.cmd_ptr->hdr.protocol);
    if (uim_ptr->hotswap.hotswap_me_power_down == TRUE &&
        uim_hw_if.intctrl[uim_ptr->id].m_hs_enable == TRUE)
    {
      /* UIM does not know the current card status as hotswap interrupt is masked at power down,
         it may be older card or may be a new card so UIM will always reset the protocol and voltage
         before processing the Reset command */
      UIMDRV_PUP_MSG_HIGH_0(UIMLOG_MSG_218,uim_ptr->id,
                        "Reset command come after the power down, UIM does not know the current card status");
      uim_ptr->hotswap.hotswap_me_power_down = FALSE;
      uim_ptr->hotswap.card_status = uim_hotswap_query_card_status(uim_ptr);
      uim_ptr->hotswap.hotswap_card_status = uim_ptr->hotswap.card_status;
      uim_ptr->hotswap.hotswap_notify_card_removed = FALSE;
      if(uim_ptr->command.cmd_ptr->hdr.command != UIM_POWER_UP_UIM_PASSIVE_F ||
         uim_ptr->command.cmd_ptr->hdr.command != UIM_RESET_SWITCH_UIM_PASSIVE_F)
      {
        uim_hotswap_reset_slot(uim_ptr);
        if ( uim_ptr->hotswap.hotswap_first_ins_class == NV_UIM_FIRST_INST_CLASS_GSM_SIM)
        {
          uim_ptr->command.cmd_ptr->hdr.protocol = UIM_ICC;
        }
        else
        {
          uim_ptr->command.cmd_ptr->hdr.protocol = UIM_UICC;
        }
      }
       /* Clear the interrupt */
       uim_hotswap_clear_interrupt(uim_ptr);
       uim_hotswap_unmask_interrupt(uim_ptr);
    }
    UIMDRV_PUP_MSG_HIGH_2(UIMLOG_MSG_219,uim_ptr->id,"UIM current card status 0x%x  HS status 0x%x",
                      uim_ptr->hotswap.card_status,
                      uim_ptr->hotswap.hotswap_card_status);
    if (uim_ptr->hotswap.card_status == UIM_CARD_REMOVED)
    {
      UIMDRV_PUP_MSG_HIGH_0(UIMLOG_MSG_220,uim_ptr->id,"Card is already removed, UIM does not need to RESET the card");
      uim_power_down(uim_ptr);
      uim_notify_error(UIM_CARD_REMOVED_S, uim_ptr);
      uim_ptr->state.status = UIM_ERR_S;

      /*set the logical slot power down flag*/
      uim_set_logical_slot_power_down_flag(uim_ptr, FALSE);
      return TRUE;
    }

    /* Change the card mode based on the reset_f param */
    uim_sam_change_mode (uim_ptr);
  }
  return FALSE;
}/* uim_is_reset_cmd_not_allowed */


/**
* This function check whether or not we can ignore the timeout signal by
* checking some variables and the uart status to see if an rx_isr would
* eventually fire or the possibility that the interrupt was already fired
* after the rex_wait has exited.
*
*
*  @param sigs The OR's set of signals that UIM task needs to
*              handle.
*  @param uim_ptr Pointer to the current instance of the UIM global structure
*
*  @return boolean TRUE if we can safely ignore the timeout signal
*/
boolean uim_can_ignore_timeout(rex_sigs_type sigs,
                                      uim_instance_global_type *uim_ptr)
{
  boolean ignore_timeout = FALSE;

  if(IS_BTSAP_HANDLE_ACTIVE(uim_ptr))
  {
    uim_ptr->flag.clear_timeout_on_response = FALSE;
    return ignore_timeout;
  }
#if defined( FEATURE_UIM_T_1_SUPPORT )
  if ( (UIM_T_1_PROTOCOL == uim_ptr->state.current_protocol) &&
       (sigs & UIM_TRANSACTION_SIG))
  {
    UIMDRV_PUP_MSG_ERR_0(UIMLOG_MSG_221,uim_ptr->id,"WARNING!!! TRANSACTION RESPONSE TIMED-OUT in T=1 protocol");
  }
#endif /* FEATURE_UIM_T_1_SUPPORT */
  /* Start ignoring the time-outs only if we have an indication not to
   * ignore the timeout signal because of not receiving the start of
   * ATR on time.  Do not ignore the timeout even if there are
   * bytes in the FIFO if the transaction timer expired since there
   * could NULL bytes since a long time.
   */
  if ( uim_ptr->flag.clear_timeout_on_response && !( sigs & UIM_TRANSACTION_SIG ) )
  {
    /* Lock the interrupts to make sure that all the checks are done
     * atomically
     */
    if ( (uim_get_uart_status(uim_ptr) & MSMU_SR_RXRDY) ||
         TIMER_IS_ACTIVE(&uim_ptr->command.cmd_rsp_timer) ||
         (rex_get_sigs(uim_ptr->tcb_ptr) & UIM_CMD_RSP_SIG) )
    {
      /* The following conditions based on which we ignore the time out signal
       *
       * 1. There are bytes in the FIFO indicating that the rx_isr
       *    would eventually be fired setting a new timer or completing
       *    the transaction based on the contents pending in the FIFO
       *
       * 2. After the signals have been extracted, the task was pre-empted
       *    and rx_isr was scheduled which would clear out the reeive FIFO
       *    and re-set the timer.  In this case, there whould be some
       *    count pending in the timer.
       *
       * 3. After the signals have been extracted, the task was pre-empted
       *    and rx_isr was scheduled which ended the transaction.  In This
       *    case, though there is no timer set, the response signal should
       *    be pending.
       */

      ignore_timeout = TRUE;
    }
  }
  /* If we have an indication to not ignore the timeout signal,
   * prepare for the next iteration.
   */
  uim_ptr->flag.clear_timeout_on_response = TRUE;

  return ignore_timeout;
} /* uim_can_ignore_timeout */


/**
* This function check whether or not the ICCID is of non compliant card or not
*  @param uim_ptr Pointer to the current instance of the UIM global structure
*
*  @return boolean TRUE if it is an non compliant card else FALSE
*/
static boolean uim_is_non_spec_complaint_iccid
(
  uim_instance_global_type *uim_ptr
)
{
  /* Buffer to hold ICCID for non spec cards */
  uim_iccid_buf_type const uim_non_spec_compliant_iccid[] = {{10 , {0x98, 0x28, 0x50, 0x31, 0x01, 0x07, 0x02, 0x40, 0x71, 0xF9 }},
                                                             {10 , {0x98, 0x28, 0x50, 0x21, 0x21, 0x07, 0x02, 0x40, 0x71, 0xF9 }}};
  uint8 i  = 0;

  if(uim_ptr == NULL)
  {
    return FALSE;
  }
  
  for(i = 0; i < (sizeof(uim_non_spec_compliant_iccid)/sizeof(uim_non_spec_compliant_iccid[0])); i++)
  {
    if (uim_ptr->cache.iccid_buf.num_bytes == uim_non_spec_compliant_iccid[i].num_bytes &&
       (memcmp(uim_ptr->cache.iccid_buf.data, uim_non_spec_compliant_iccid[i].data, sizeof(uim_ptr->cache.iccid_buf.data)) == 0))
    {
      return TRUE;
    }
  }
  return FALSE;
}/* uim_is_non_spec_complaint_iccid */


/**
*
* This procedure checks whether streaming command for given logical
* channel is allowed or not.
* Case may be that recovery is kicked in at streaming APDU and
* during smart channel recovery, streaming session/channel is closed and
* the error for this command is sent to its client.
*
*
*   @param uim_ptr Pointer to the current instance of the UIM global structure
*
*  @return boolean TRUE if processing of command is allowed else FALSE

*/

static boolean uim_is_streaming_cmd_allowed
(
  uim_instance_global_type *uim_ptr
)
{
  if(uim_ptr->id >= UIM_MAX_INSTANCES)
  {
    return (FALSE);
  }
  
  if ( uim_ptr->command.mode == UIM_PASSIVE_MODE )
  {
    /* Streaming APDU to be sent to card in passive mode */
    return TRUE;
  }

  /* Do not open channel other than 1 if it is non spec compliant card */
  if((uim_ptr->command.cmd_ptr->hdr.command == UIM_MANAGE_CHANNEL_F)      &&
      uim_ptr->state.is_ch_open[1]                                        && 
      (uim_ptr->command.cmd_ptr->manage_channel.mc_action == UIM_MC_OPEN) &&
      uim_is_non_spec_complaint_iccid(uim_ptr))
  {
    return FALSE;
  }

  /* Check for the validity of channel. If this channel is opened then process the command otherwise
     return the error.
     Use case is like this- All non-prov session is closed during smart channel recovery and
     command shall never be attempted if it's streaming APDU.
     SelectNext (Next Occurrence of partial AID) will always belong to streaming APDU.
  */
  if( (uim_ptr->command.cmd_ptr->hdr.channel > UIM_CHANNEL0) &&
      (FALSE == uim_ptr->state.is_ch_open[uim_ptr->command.cmd_ptr->hdr.channel]))
  {
    return(FALSE);
  }

  if(uim_ptr->command.cmd_ptr->hdr.command == UIM_MANAGE_CHANNEL_F &&
     uim_ptr->state.channel_info.manage_channel_allowed  == FALSE)
  {
    UIMDRV_MSG_ERR_0(uim_ptr->id,"open channel not supported");
    return(FALSE);
  }
  return(TRUE);

}/* uim_is_streaming_cmd_allowed */


/**
*  This function handles the UIM_FETCH_PENDING_SIG signal.
*
*  @param sig_mask_ptr  Pointer to the OR's set of signals that UIM task needs tohandle.
                        Unused in this function.
*  @param imask_ptr     Pointer to the mask of signals that the uim task is interested
*                       in handling.   Unused in this function.
*  @param uim_ptr Pointer to the current instance of the UIM global structure
*  @return boolean Return TRUE to indicate that immediately
*          going back to the top of the main loop is desired.
*/
static boolean uim_handle_fetch_pending_sig(rex_sigs_type *sig_mask_ptr,
                                            rex_sigs_type *imask_ptr,
                                            uim_instance_global_type *uim_ptr)
{
  (void)sig_mask_ptr;
  (void)imask_ptr;
  
  uim_ptr->flag.fetch_pending  = TRUE;
  UIMDRV_MSG_LOW_0(uim_ptr->id, "Recevied Fetch pending request");
  (void) rex_clr_sigs(uim_ptr->tcb_ptr, UIM_FETCH_PENDING_SIG );
  return FALSE;
}/* uim_handle_fetch_pending_sig */


/**
*  This function handles the UIM_CONNECTION_UNAVAILABLE_SIG signal.
*
*  @param sig_mask_ptr  Pointer to the OR's set of signals that UIM task needs tohandle.
                        Unused in this function.
*  @param imask_ptr     Pointer to the mask of signals that the uim task is interested
*                       in handling.   Unused in this function.
*  @param uim_ptr Pointer to the current instance of the UIM global structure
*  @return boolean Return TRUE to indicate that immediately
*          going back to the top of the main loop is desired.
*/
static boolean uim_handle_remote_connection_unavailable_sig(rex_sigs_type *sig_mask_ptr,
                                            rex_sigs_type *imask_ptr,
                                            uim_instance_global_type *uim_ptr)
{
  (void)sig_mask_ptr;
  (void)imask_ptr;
  
  UIMDRV_MSG_HIGH_1(uim_ptr->id, "In uim_handle_remote_connection_unavailable_sig function with remote state 0x%x",uim_ptr->remote.state);
  (void) rex_clr_sigs(uim_ptr->tcb_ptr, UIM_REMOTE_CONNECTION_UNAVAILABLE_SIG );

  if(WAIT_FOR_RESPONSE == uim_ptr->remote.state)
  {
    (void)rex_clr_timer(&uim_ptr->remote.remote_cmd_rsp_timer);
    /* Set the command response timeout signal */
    (void) rex_set_sigs( uim_ptr->tcb_ptr, UIM_CMD_RSP_TIMEOUT_SIG );
  }
  
  return FALSE;
}/* uim_handle_remote_connection_unavailable_sig */



/**
*  This function handles the UIM_RCEVT_CM_TASK_READY_SIG signal.
*
*  @param sig_mask_ptr  Pointer to the OR's set of signals
*                       that UIM task needs to handle.
*  @param imask_ptr Pointer to the mask of signals that the uim task
*          is interested in handling.
*  @param uim_ptr Pointer to the current instance of the UIM global structure
*  @return boolean Return TRUE if immediately going back to the
*          top of the main loop is desired. Otherwise, FALSE.
*/
static boolean uim_handle_cm_task_ready_sig(rex_sigs_type *sig_mask_ptr,
                                            rex_sigs_type *imask_ptr,
                                 uim_instance_global_type *uim_ptr)
{
  RCEVT_SIGEX_SIGREX rcevt_siginfo_cm;
  (void)sig_mask_ptr;

  UIM_MSG_HIGH_0("UIM - CM task is ready");
  rcevt_siginfo_cm.signal = uim_ptr->tcb_ptr;
  rcevt_siginfo_cm.mask   = UIM_RCEVT_CM_TASK_READY_SIG;

  (void)mcs_rcevt_unregister_sigex_name("cm:ready",
                                  RCEVT_SIGEX_TYPE_SIGREX,
                                  &rcevt_siginfo_cm);

  rex_clr_sigs(uim_ptr->tcb_ptr, UIM_RCEVT_CM_TASK_READY_SIG);
  *imask_ptr &= ~UIM_RCEVT_CM_TASK_READY_SIG;

  /* Register with CM */
  uim_cm_init();
  return FALSE;
}/* uim_handle_cm_task_ready_sig */


/**
*  This function handles the UIM_RCEVT_DIAG_LOG_MASK_CHANGE_SIG signal.
*
*  @param sig_mask_ptr  Pointer to the OR's set of signals
*                       that UIM task needs to handle.
*  @param imask_ptr Pointer to the mask of signals that the uim task
*          is interested in handling.
*  @param uim_ptr Pointer to the current instance of the UIM global structure
*  @return boolean Return TRUE if immediately going back to the
*          top of the main loop is desired. Otherwise, FALSE.
*/
static boolean uim_handle_diag_log_mask_change_sig(rex_sigs_type            *sig_mask_ptr,
                                                   rex_sigs_type            *imask_ptr,
                                                   uim_instance_global_type *uim_ptr)
{
  uim_instance_global_type     *uim_ptr_local         = NULL;
  RCEVT_SIGEX_SIGREX            rcevt_siginfo_diag;
  static uint8                  diag_indication_count = 0;

  (void)sig_mask_ptr;

  if(uim_ptr == NULL ||
     imask_ptr == NULL)
  {
    return FALSE;
  }
 
  UIM_MSG_HIGH_0("UIM - Received Diag log mask change indication");

  /* Wait till received diag indication for NUM_DIAG_IND_TO_WAIT or 
     LOG_UIM_APDU_LOG_C is enabled in log_mask
     Intentionally proceed to commit() after NUM_DIAG_IND_TO_WAIT 
     even if log_status() is false. So that it will help emptying the queue.*/
  if(!log_status(LOG_UIM_APDU_LOG_C)   && 
     (diag_indication_count < UIM_NUM_DIAG_IND_TO_WAIT))
  {
    diag_indication_count++;
    return FALSE;
  }
  
  rcevt_siginfo_diag.signal = uim_ptr->tcb_ptr;
  rcevt_siginfo_diag.mask   = UIM_RCEVT_DIAG_LOG_MASK_CHANGE_SIG;

  /* Unregistering with DIAG, because only during boot-up 
      need this indication to commit boot-up APDU logs */
  (void)mcs_rcevt_unregister_sigex_name(DIAG_LOG_MASK_CHANGE_NOTIFY,
                                        RCEVT_SIGEX_TYPE_SIGREX,
                                        &rcevt_siginfo_diag);

  rex_clr_sigs(uim_ptr->tcb_ptr, UIM_RCEVT_DIAG_LOG_MASK_CHANGE_SIG);
  *imask_ptr &= ~UIM_RCEVT_DIAG_LOG_MASK_CHANGE_SIG;

  uim_log_boot_up_commit();
  
  return FALSE;
}/* uim_handle_diag_log_mask_change_sig */

