/*==============================================================================
  FILE:         uim_hotswap.c

  OVERVIEW:     FIle conatins the functions for UIM HOTSWAP feature.

  DEPENDENCIES: N/A


                Copyright (c) 2014 - 2018 QUALCOMM Technologies, Inc (QTI) and its licensors.
                All Rights Reserved.
                QUALCOMM Technologies Confidential and Proprietary
==============================================================================*/

/*=============================================================================
  EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.  Please
  use ISO format for dates.

$Header: //components/rel/uim.mpss/6.1.0/uimdrv/src/uim_hotswap.c#2 $
$DateTime: 2019/10/30 06:56:05 $
$Author: pwbldsvc $

when        who        what, where, why
------      ----       ---------------------------------------------------------
03/27/18   gm      Start extended recovery when card is present
02/06/18    ku         Not starting second poll timer at power up
02/01/18    ku         abandon extended recovery in case of card inserted
12/05/17    gm         Update only active slots in uim_get_hw_slot_info
07/06/17    ku         Poll card for twice when detecting flaky removal interupt
11/24/16    ks         Adding support for spcom interface
10/24/16    ku         KW errors fix 
07/14/16    ks         Enhancing the extended recovery feature
07/01/16    gm         Remove done_q_ptr implementation
06/17/16    gm         Stub out Hotswap related functionality
06/16/16    ks         Correction in few F3's
06/10/16    gm         F3 message optimization
04/20/16    sam        Support for enhanced subscription manager
04/19/16    hyo        RUMI emulation check for One-time hotswap for UIM2
03/28/16    sam        Support for SAM card
03/26/16    ssr        Disable Hotswap functionality for IoE flavor
02/06/16    ll         Support for one time HOTSWAP on UIM2
01/20/16    sam        Resetting the command transacted flag if the 
                       response is not received from card
11/17/15    ssr        Fixed KW error
09/02/15    hyo        Drivers and state machine abstraction
08/14/15    ks         Making sure hotswap_card_status has proper value
08/11/15    hyo        usb uicc removal
06/05/15    ks         Clear BTSAP cmd response timer upon remote card removal
05/11/15    sam        When flaky hotswap feature is enabled then mask
                       hotswap interrupt while handling card_suspicious signal
04/27/15    ssr        Fix to stop recovery process on card removal
04/23/15    ks         Clearing total recovery count upon new card insertion
04/15/15    ks         De-Featurizing the FEATURE_UIM_REMOTE/BTSAP_UIM
08/04/15    sam        FR24498: Powerup logging changes
01/07/14    sam        Modification of msg macros for FR24498-UIM Powerup Logging
12/25/14    xj         Add DISABLE_CARD_STATUS_CHECK_AT_POWER_UP in features NV
12/05/14    akv        Fixed compiler warnings
12/04/14    akv        Fixed compiler warnings
12/03/14    sam        Fix for KW errors
12/02/14    ks         Feature to support GPIO based hotswap
11/24/14    kr         Fixed Compilation warnings
11/18/14    ks         BTSAP connection establishment even when local SIM
                       is absent
10/15/14    ssr        Not unmasking the Hotswap interrupt when
                       IgnoreHotSwapSwitchTlv Tag is used in QMI_UIM_POWER_UP req
09/18/14    akv        UIMDRV UT enhancements
09/05/14    sam        check for command_requeued flag while processing hotswap
08/26/14    ks         Clearing path position during card insertion
08/14/14    ks         Hotswap enhancement for flaky mechanical switches
08/14/14    sam        Support for extended recovery
08/04/14    ll         Fix compile warnings
07/16/14    lm         Fix for offtarget related to hotswap
06/16/14    ll         Switch to new string based TLMM APIs
06/11/14    ll         KW errors
04/25/14    sam        Fix for KW issue: possible out of bound array.
02/08/14    sam        Fix for Klocwork issues
01/31/14    ak         Safety checks for disabled UIM instances
01/25/14    yk         The first revision
==============================================================================*/
#include "queue.h"
#include "nv_items.h"
#include "timer.h"
#include "DDITlmm.h"
#include "DDIGPIOInt.h"
#include "uimdrv_msg.h"
#include "uim_p.h"
#include "uimi.h"
#include "uim_common_efs.h"
#include "uimglobals.h"
#include "uim_logging.h"
#include "uim_nvcfg.h"
#include "uimdrv.h"
#include "uimdrv_hal_iso.h"
#include "uimdrv_main.h"
#include "uimdrv_uartdm.h"
#include "uimdrv_gpio.h"
#include "uimdrv_clk.h"
#include "uim_hotswap.h"
#include "uim_remote.h"
#include "uimdrv_rumi.h"

/**
 * DECLARATIONS OF INTERNAL FUNCTIONS
 */
static boolean uim_hotswap_send_card_inserted_cmd(
  uim_instance_global_type *uim_ptr);
static void uim_hotswap_define_debounce_timer(uim_instance_global_type *uim_ptr);
static void uim_hotswap_reset_debounce_timer(
  uim_card_swap_enum_type    hos_new_card_status,
  uim_instance_global_type  *uim_ptr);
static void uim_hotswap_start_debounce_timer(
  uim_card_swap_enum_type        hos_new_card_status,
  uim_instance_global_type      *uim_ptr);
static void uim_hotswap_clear_debounce_timer(uim_instance_global_type *uim_ptr);
static void uim_hotswap_debounce_timer_expiry_cb(unsigned long instance_ptr);
static uim_hotswap_slot_capability uim_get_hotswap_enabled_slot(
  uim_instance_global_type *uim_ptr);
static void uim_config_hotswap_gpios(uim_instance_global_type *uim_ptr);


/** 
 * Reads the hotswap debounce config from NV
 *
 * @param uim_ptr Pointer to the global data common.
*/
void uim_read_hotswap_debounce_config(uim_instance_global_type *uim_ptr)
{
  if(uim_ptr == NULL)
  {
    return;
  }
 
  if (UIM_COMMON_EFS_SUCCESS != uim_common_efs_read(UIM_COMMON_EFS_UIMDRV_HOTSWAP_DEBOUNCE_CONFIG,
                                                    UIM_COMMON_EFS_ITEM_FILE_TYPE,
                                                    UIM_COMMON_EFS_DEVICE,
                                                   (uint8 *)&(uim_ptr->hotswap.hotswap_me_conf),
                                                    sizeof(uim_ptr->hotswap.hotswap_me_conf)))
  {
    uim_ptr->hotswap.hotswap_me_conf.auxiliary_period_for_card_detect = UIM_HOTSWAP_AUX_SLOT_PERIOD;
    uim_ptr->hotswap.hotswap_me_conf.maximum_debounce_retry_counter   = UIM_HOTSWAP_MAX_DEBOUNCE_RETRY;
    uim_ptr->hotswap.hotswap_me_conf.num_of_sample_for_insertion      = UIM_HOTSWAP_NUM_OF_SAMPLES_TO_DETECT_CARD_INSERTION;
    uim_ptr->hotswap.hotswap_me_conf.num_of_sample_for_removal        = UIM_HOTSWAP_NUM_OF_SAMPLES_TO_DETECT_CARD_REMOVAL;
	
    if (UIM_COMMON_EFS_SUCCESS != uim_common_efs_write(UIM_COMMON_EFS_UIMDRV_HOTSWAP_DEBOUNCE_CONFIG,
                                                       UIM_COMMON_EFS_ITEM_FILE_TYPE,
                                                       UIM_COMMON_EFS_DEVICE,
                                                      (char *)&(uim_ptr->hotswap.hotswap_me_conf),
                                                       sizeof(uim_ptr->hotswap.hotswap_me_conf)))
    {
      UIMDRV_MSG_HIGH_0(uim_ptr->id,"Writing default values to Hotswap Debounce Config EFS FAILED");
    }
  }
}/*uim_read_hotswap_debounce_config*/


/**
 * DEFINITIONS OF EXTERNAL FUNTIONS
 */

/**
 *This function configures the GPIO that is connected 
 *to the card detection switch.
 *
 * @param uim_ptr Pointer to the global data common.
 */
static void uim_config_hotswap_gpios(uim_instance_global_type *uim_ptr)
{
  uim_instance_enum_type uim_instance;

  if((NULL == uim_ptr)                  ||
     (uim_ptr->id >= UIM_MAX_INSTANCES) ||
     (FALSE == uim_hw_if.intctrl[uim_ptr->id].m_hs_enable))
  {
    return;
  }
  
  uim_instance = uim_ptr->id;
  
#ifndef T_RUMI_EMULATION 
  
  if( NULL == uim_hw_if.gpio[uim_instance].m_TlmmHandle_ptr ||
      0 == uim_hw_if.gpio[uim_instance].m_PresentGpioId )
  {
    return;
  }

  if(uim_hotswap_nature[uim_instance] != UIM_CONTROLLER_BASED)
  {
    DalTlmm_SelectGpioIdMode(uim_hw_if.gpio[uim_instance].m_TlmmHandle_ptr,
                           uim_hw_if.gpio[uim_instance].m_PresentGpioId,
                           UIM_TLMM_IO_MODE,
                           &(uim_hw_if.gpio[uim_instance].m_PresentGpioConfigId));
  }
  else
  {
    DalTlmm_SelectGpioIdMode(uim_hw_if.gpio[uim_instance].m_TlmmHandle_ptr,
                           uim_hw_if.gpio[uim_instance].m_PresentGpioId,
                           UIM_TLMM_UIMCTRL_MODE,
                           &(uim_hw_if.gpio[uim_instance].m_PresentGpioConfigId));
  }

  if(uim_nv_is_feature_enabled(UIMDRV_FEATURE_SIMTRAY_WITH_GPIOS_TIED,
                               uim_ptr) == TRUE)
  {
    if((uim_hw_slot_info.slot_status[UIM_INSTANCE_1].slot_available == FALSE)
	  ||(uim_hw_slot_info.slot_status[UIM_INSTANCE_2].slot_available == FALSE)) 
    {
      DalTlmm_SelectGpioIdMode(uim_hw_if.gpio[uim_instance].m_TlmmHandle_ptr,
							    disabled_uim_card_presence_gpio_id,
							    UIM_TLMM_IO_MODE,
							    &(uim_hw_if.gpio[uim_instance].m_PresentGpioConfigId));
    }
  }

#else
  if(UIM_INSTANCE_1 == uim_instance)
  {
    (void)DalTlmm_ConfigGpio(uim_hw_if.gpio[uim_instance].m_TlmmHandle_ptr,
                           (DALGpioSignalType)UIM_HOTSWAP_GPIO,
                            DAL_TLMM_GPIO_ENABLE);
  }
  else
  {
    (void)DalTlmm_ConfigGpio(uim_hw_if.gpio[uim_instance].m_TlmmHandle_ptr,
                            (DALGpioSignalType)UIM2_HOTSWAP_GPIO,
                             DAL_TLMM_GPIO_ENABLE);
  }
#endif
  return;
} /* uim_config_hotswap_gpios */


/**
 * This procedure initializes the  queues and timers for UIM
 * HOTSWAP. It should be called only once, at powerup time.
 *
 * @param uim_ptr Pointer to the global data common.
 */
void uim_hotswap_init(uim_instance_global_type *uim_ptr)
{
  int i = 0;
  boolean hotswap_enabled = FALSE;
  
  if(uim_ptr == NULL)
  {
    return;
  }

  RETURN_IF_INSTANCE_INVALID(uim_ptr->id);

  uim_ptr->hotswap.is_masked = TRUE;

  if( (TRUE == m_HWConfig.uim_slot_config[uim_ptr->id].disableUim) || 
      (uim_ptr->setting.phy_layer != UIM_ISO) )
  {
    /* Slot is disabled */
    return;
  }

  /* Ini all hs queue */
  (void) q_init( &uim_ptr->hotswap.hotswap_free_q );
  (void) q_init( &uim_ptr->hotswap.hotswap_ins_cmd_q );
  (void) q_init( &uim_ptr->hotswap.hotswap_rem_cmd_q );

  /* Function uim_uartdm_uim_controller_config is called twice as the first time we set the
        bits in the UIM CFG register that enable Battery alarm trigger, uimremoved trigger, mode18,
        pmic alarm.
        If hotswap is enabled then we set the uim_card_events _enable bit which if cleared results in
        the uim controller not reacting to any card event (insertion/removal)
    */
  uim_uartdm_uim_controller_config(uim_ptr, hotswap_enabled);

  for (i = 0; i < UIM_HOTSWAP_NUM_CMD_BUFS; i++)
  {
    /* Init the buffer with SLOT AUTOMATIC */
    uim_ptr->hotswap.hotswap_cmd_pool[i].hdr.slot = UIM_SLOT_AUTOMATIC;
    uim_ptr->hotswap.hotswap_cmd_pool[i].hdr.cmd_hdr.done_q_ptr = &uim_ptr->hotswap.hotswap_free_q;
    q_put( &uim_ptr->hotswap.hotswap_free_q, q_link( &uim_ptr->hotswap.hotswap_cmd_pool[i],
           &uim_ptr->hotswap.hotswap_cmd_pool[i].hdr.cmd_hdr.link));
  }

  uim_ptr->hotswap.hotswap_first_ins_class = uim_ptr->state.first_inst_class;
  uim_ptr->hotswap.retry_power_up_count = uim_ptr->extended_recovery.recovery_config.no_of_extended_power_up_attempts;

  if(uim_hw_if.intctrl[uim_ptr->id].m_hs_enable == TRUE)
  {
    hotswap_enabled = TRUE;
    uim_ptr->hotswap.hotswap_capability   =  uim_get_hotswap_enabled_slot(uim_ptr);
    /* Configure hotswap gpios */
    uim_config_hotswap_gpios(uim_ptr);
    uim_uartdm_uim_controller_config(uim_ptr, hotswap_enabled);
    if(uim_nv_is_feature_enabled(UIMDRV_FEATURE_DISABLE_CARD_STATUS_CHECK_AT_POWER_UP,
								 uim_ptr) == TRUE)
    {
      /* set card status as inserted */
      uim_ptr->hotswap.card_status = UIM_CARD_INSERTED;
    }
    else
    {
      for(i = 0; i < UIM_HOTSWAP_MAX_CHECK_FOR_CARD_INSERTION_ON_POWER_UP; i++)
      {
        uim_ptr->hotswap.card_status  = uim_hotswap_query_card_status(uim_ptr);
        if((UIM_CARD_INSERTED == uim_ptr->hotswap.card_status) ||
           (UIM_CARD_UNCHANGED == uim_ptr->hotswap.card_status))
        {
          break;
        }
        /* Waiting 200us after write of UART_DM_UIM_CFG does not ensure the status
           of the card will be updated immediately, but it ensures the command
           has reached the UIM controller internal logic and new command might
           be executed . After a configuration of ‘UIM_CARD_EVENTS_ENABLE’
           bit  UIM_controller will start counting for EVENT_DEBOUNCE_TIME and
           only then it will update the status to card present.

           That is why polling on UART_DM_UIM_IO_STATUS is required. A single read
           of IO_STATUS register after 100,200us will not ensure the status will
           be CARD_INSERTED.

           Here in worse case the delay would be 2 ms.
         */
        uim_clk_busy_wait(200);
      }
    }
    if(uim_ptr->hotswap.card_status == UIM_CARD_UNCHANGED)
    {
      uim_ptr->hotswap.card_status = UIM_CARD_INSERTED;
    } 
    uim_ptr->hotswap.hotswap_card_status = uim_ptr->hotswap.card_status;
    uim_hotswap_define_debounce_timer(uim_ptr);

#ifndef T_RUMI_EMULATION
#ifdef FEATURE_UIM_ONETIME_HOTSWAP_UIM2
    if ( BADGER_MSS_UIM1_UART_DM == m_HWConfig.uim_slot_config[uim_ptr->id].uimUart)
    {
      /*
      Configure as below for potential saving on leakage current:
      1) Card detected : reconfigure to INPUT NO PULL.
      2) Card not detected : leave as is INPUT PD
      */
      if(UIM_CARD_REMOVED == uim_ptr->hotswap.hotswap_card_status )
      {
        /* No external pull up for detect line in this case */
        uim_hw_if.gpio[uim_ptr->id].m_PresentGpioConfigId.ePull = DAL_GPIO_PULL_DOWN;
      }
      else
      {
        /* Has external pull up for detect line in this case */
        uim_hw_if.gpio[uim_ptr->id].m_PresentGpioConfigId.ePull = DAL_GPIO_NO_PULL;
      }
      /* NOTE: This feature is for UIM controller based hotswap */
      DalTlmm_SelectGpioIdMode(uim_hw_if.gpio[uim_ptr->id].m_TlmmHandle_ptr,
                               uim_hw_if.gpio[uim_ptr->id].m_PresentGpioId,
                               UIM_TLMM_UIMCTRL_MODE,
                               &(uim_hw_if.gpio[uim_ptr->id].m_PresentGpioConfigId));
    }
#endif /*FEATURE_UIM_ONETIME_HOTSWAP_UIM2*/
#endif /*T_RUMI_EMULATION*/
    uim_hotswap_unmask_interrupt(uim_ptr);
  }
} /* uim_hotswap_init */


/**
 * Set hotswap signal to UIM task.
 *
 * @param hos_new_card_status New HOTSWAP status.
 * @param uim_ptr Pointer to the global data common.
 *
 * @return boolean Success or Fail.
 */
boolean  uim_hotswap_set_signal(
  uim_card_swap_enum_type   hos_new_card_status,
  uim_instance_global_type *uim_ptr)
{
  if(uim_ptr == NULL)
  {
    return FALSE;
  }
 
  if (uim_ptr->id >= UIM_MAX_INSTANCES)
  {
    return FALSE;
  }

  if( uim_hw_if.intctrl[uim_ptr->id].m_hs_enable == TRUE
      || IS_BTSAP_HANDLE_ACTIVE(uim_ptr)
    )
  {
    if (hos_new_card_status == UIM_CARD_INSERTED)
    {
      uim_ptr->hotswap.card_status = hos_new_card_status;
      uim_ptr->hotswap.hotswap_card_status = hos_new_card_status;
      return uim_hotswap_send_card_inserted_cmd(uim_ptr);
    }
    else
    {
      /*Flaky switch not supported if the current task is in SAM mode*/
      if (uim_ptr->hotswap.hotswap_flaky_switch_support &&
          (UIM_CARD_TELECOM  == uim_ptr->card_mode) &&
          (UIM_SLOT_STATE_ACTIVE == uim_ptr->activity_status))
      {
        /* Set card removed suspicious signal instead of card rem signal */
        UIMDRV_MSG_HIGH_0(uim_ptr->id,"Sending suspicious card removed signal");
        (void) rex_set_sigs( uim_ptr->tcb_ptr, UIM_SUSPICIOUS_CARD_REM_SIG);
        return TRUE;
      }
      else
      {
        uim_ptr->hotswap.card_status = hos_new_card_status;
        uim_ptr->hotswap.hotswap_card_status = hos_new_card_status;
        return uim_hotswap_send_card_removed_cmd(uim_ptr);
      }
    }
  }
  else
  {
    return FALSE;
  }
} /* uim_hotswap_set_signal */


/**
*  This function handles the UIM_SUSPICIOUS_CARD_REM_SIG signal.
*
*
*  @param sig_mask_ptr Pointer to the OR's set of signals that UIM task needs to
*                      handle.  Can be updated in this function.
*  @param imask_ptr Pointer to the mask of signals that the uim task is interested
*                   in handling.   The mask may be changed by this function.
*  @param uim_ptr Pointer to the current instance of the UIM global structure
*  @return boolean Return TRUE to indicate that immediately
*          going back to the top of the main loop is desired.
*/
boolean uim_handle_card_removed_suspicious_sig(rex_sigs_type *sig_mask_ptr,
                                            rex_sigs_type *imask_ptr,
                                            uim_instance_global_type *uim_ptr)
{
  (void)sig_mask_ptr;
  (void)imask_ptr;

  if(uim_ptr == NULL)
  {
   return FALSE;
  }
 
  (void) rex_clr_sigs( uim_ptr->tcb_ptr, UIM_SUSPICIOUS_CARD_REM_SIG);

  /* If already the card removed suspicious flag is TRUE, then simply return */
  if(uim_ptr->hotswap.hotswap_card_removed_suspicious == FALSE)
  {
    /* If already in error state or passive mode, we can not send POLL command
       So treat this as genuine card removal */
    if((uim_ptr->state.status != UIM_ERR_S)&&(uim_ptr->command.mode != UIM_PASSIVE_MODE))
    {
      uim_ptr->hotswap.hotswap_card_removed_suspicious = TRUE;
      uim_ptr->poll.flaky_polling_state                = UIM_FLAKY_POLLING_ATTEMPT_1;
      /* Set the POLL pending flag to confirm the card status
         when there is no command in progress  */
      if (!uim_ptr->flag.command_in_progress)
      {
        uim_ptr->flag.poll_pending = TRUE;
      }
      else
      {
        if(uim_ptr->command.static_cmd_buf.hdr.command == UIM_INTERNAL_ME_PUP_F ||
           uim_ptr->command.static_cmd_buf.hdr.command == UIM_INTERNAL_WAKE_UP_F ||
           uim_ptr->command.static_cmd_buf.hdr.command == UIM_HOTSWAP_CARD_INS_F )
        {
          uim_ptr->poll.flaky_polling_state = UIM_FLAKY_POLLING_OFF;		
        }
        else
        {
          /* If an existing command is in progress set a poll timer to configurable value 
             to check card status*/
          uim_ptr->poll.orig_poll_time = uim_ptr->setting.poll_time_delay;
          uim_set_poll_timer(uim_ptr->poll.poll_time_for_flaky,
                             uim_ptr->setting.wakeup_poll_offset,
                             uim_ptr);
        }
      }
    }
    else
    {
      UIMDRV_MSG_HIGH_0(uim_ptr->id,"Sending card rem sig as UIM is already in state UIM_ERR_S " );
      (void) rex_clr_sigs( uim_ptr->tcb_ptr, UIM_CMD_RSP_TIMEOUT_SIG );
      (void) rex_clr_sigs( uim_ptr->tcb_ptr, UIM_TRANSACTION_SIG );
      uim_ptr->hotswap.card_status         = UIM_CARD_REMOVED;
      uim_ptr->hotswap.hotswap_card_status = UIM_CARD_REMOVED;
      uim_hotswap_send_card_removed_cmd(uim_ptr);
      /* Unmask the card insert interrupt */
      uim_hotswap_unmask_interrupt(uim_ptr);
    }
  }
  return FALSE;
}


/**
*  This function handles the UIM_HOTSWAP_CMD_CARD_REM_SIG signal.
*
*  SIDE EFFECTS
*    This function may change the value of *imask_ptr or *sig_mask_ptr.
*
*  @param sig_mask_ptr Pointer to the OR's set of signals that UIM task needs to
*                      handle.  Can be updated in this function.
*  @param imask_ptr Pointer to the mask of signals that the uim task is interested
*                   in handling.   The mask may be changed by this function.
*  @param uim_ptr Pointer to the current instance of the UIM global structure
*  @return boolean Return TRUE to indicate that immediately
*          going back to the top of the main loop is desired.
*/
boolean uim_handle_card_removed_sig(rex_sigs_type *sig_mask_ptr,
                                           rex_sigs_type *imask_ptr,
                                           uim_instance_global_type *uim_ptr)
{
  (void)imask_ptr;
  (void)sig_mask_ptr;

  if (uim_ptr == NULL || uim_ptr->id >= UIM_MAX_INSTANCES)
  {
    return FALSE;
  }
  
  if((TRUE == uim_hw_if.intctrl[uim_ptr->id].m_hs_enable)
       || ( IS_BTSAP_HANDLE_ACTIVE(uim_ptr) )
     )
  {
    (void) rex_clr_sigs( uim_ptr->tcb_ptr, UIM_HOTSWAP_CMD_CARD_REM_SIG);
    if (q_cnt(& uim_ptr->hotswap.hotswap_rem_cmd_q) != 0)
    {
      if (TRUE == uim_hotswap_process_card_removed_sig(uim_ptr))
      {
        uim_process_command(imask_ptr, uim_ptr);
        *imask_ptr &= ~UIM_CMD_Q_SIG;
        *sig_mask_ptr  &= ~UIM_CMD_Q_SIG;
        /* Update the UIM command pointer to current Hot Swap command,
           so that old cmd_ptr is not re-used*/
        uim_ptr->command.cmd_ptr = &uim_ptr->command.static_cmd_buf;
        (void) rex_clr_sigs(uim_ptr->tcb_ptr, UIM_CMD_Q_SIG );
      }
    }
  }

  return FALSE;
} /* uim_handle_card_removed_sig */


/**
*  This function handles the UIM_HOTSWAP_CMD_CARD_INS_SIG signal.
*
*  SIDE EFFECTS
*    This function may change the value of *imask_ptr or *sig_mask_ptr.
*
*
*  @param sig_mask_ptr Pointer to the OR's set of signals that UIM task needs to
*                      handle.  Can be updated in this function.
*  @param imask_ptr Pointer to the mask of signals that the uim task is interested
*                   in handling.   The mask may be changed by this function.
*  @param uim_ptr Pointer to the current instance of the UIM global structure
*  @return boolean Return TRUE to indicate that immediately
*          going back to the top of the main loop is desired.
*/
boolean uim_handle_card_inserted_sig(rex_sigs_type *sig_mask_ptr,
                                            rex_sigs_type *imask_ptr,
                                            uim_instance_global_type *uim_ptr)
{
  (void)sig_mask_ptr;
  (void)imask_ptr;

  if (uim_ptr == NULL || uim_ptr->id >= UIM_MAX_INSTANCES)
  {
    return FALSE;
  }
  
  /* Card removed signal has a higher priority, so do nothing if */
  /* the removed signal is set */
  if (0 == (*sig_mask_ptr & UIM_HOTSWAP_CMD_CARD_REM_SIG))
  {
    if((TRUE == uim_hw_if.intctrl[uim_ptr->id].m_hs_enable)
       || ( IS_BTSAP_HANDLE_ACTIVE(uim_ptr) )
       )
    {
      (void) rex_clr_sigs( uim_ptr->tcb_ptr, UIM_HOTSWAP_CMD_CARD_INS_SIG );
      if (q_cnt(& uim_ptr->hotswap.hotswap_ins_cmd_q) != 0)
      {
        /* command pointer need to end the last pending request if needed */
        if (TRUE == uim_hotswap_process_card_inserted_sig(uim_ptr))
        {
          uim_process_command(imask_ptr, uim_ptr);
          *imask_ptr &= ~UIM_CMD_Q_SIG;
          *sig_mask_ptr  &= ~UIM_CMD_Q_SIG;
          /* Update the UIM command pointer to current Hot Swap command,
             so that old cmd_ptr is not re-used  */
          uim_ptr->command.cmd_ptr = &uim_ptr->command.static_cmd_buf;
          (void) rex_clr_sigs(uim_ptr->tcb_ptr, UIM_CMD_Q_SIG );
        }
      }
    }/* if(TRUE == uim_hw_if.intctrl[uim_ptr->id].m_hs_enable) */
  }

  return FALSE;
} /* uim_handle_card_inserted_sig */


/**
 * Reset the UIM global to its intial value caused by HOTSWAP
 * and re-configure the UARTDM for HOTSWAP.
 *
 * @param uim_ptr Pointer to the global data common.
 */
void uim_hotswap_reset_slot(uim_instance_global_type *uim_ptr)
{
  /* Reset the necessary UIM globals */
  uim_reset_globals(uim_ptr);

  /* Re-configure the UARTDM */
  uimdrv_iso_reconfig_uartdm(uim_ptr);
}/* uim_hotswap_reset_slot */


/**
 * Process card inserted signal in UIM task.
 *
 * @param uim_ptr Pointer to the global data common.
 *
 * @return boolean Success or Fail.
 */
boolean uim_hotswap_process_card_inserted_sig(
  uim_instance_global_type *uim_ptr)
{
  uim_cmd_type   *hos_cmd_ptr      = NULL; /* Pointer to received command */
  uim_slot_type   hos_curr_slot    = UIM_SLOT_NONE;

  if (uim_ptr == NULL)
  {
   return FALSE;
  }
 
  /* Received card inserted request , take the requested command pointer */
  hos_cmd_ptr = (uim_cmd_type *)q_check(&uim_ptr->hotswap.hotswap_ins_cmd_q);
  if (hos_cmd_ptr == NULL)
  {
    return FALSE;
  }

  /* Check the slot */
  if ((hos_cmd_ptr->hdr.slot != UIM_SLOT_1) &&
      (hos_cmd_ptr->hdr.slot != UIM_SLOT_2) &&
      (hos_cmd_ptr->hdr.slot != UIM_SLOT_3))
  {
    hos_cmd_ptr = (uim_cmd_type *)q_get(&uim_ptr->hotswap.hotswap_ins_cmd_q);
    if (hos_cmd_ptr != NULL)
    {
      /* Free the UIM buffer for next request */
      hos_cmd_ptr ->hdr.slot = UIM_SLOT_AUTOMATIC;
      q_put( &uim_ptr->hotswap.hotswap_free_q, q_link(hos_cmd_ptr,&hos_cmd_ptr->hdr.cmd_hdr.link));
    }
    return FALSE;
  }

  /* Check the current card status before processing the command */
  if (uim_ptr->hotswap.card_status == UIM_CARD_REMOVED || uim_ptr->hotswap.hotswap_me_power_down == TRUE)
  {
    UIMDRV_PUP_MSG_HIGH_0(UIMLOG_MSG_140,uim_ptr->id,"HOTSWAP: Drop Card inserted signal as card is already removed or me is in power down state");
    /* Detect the card insert */
    hos_cmd_ptr = (uim_cmd_type *)q_get(&uim_ptr->hotswap.hotswap_ins_cmd_q);
    if (hos_cmd_ptr != NULL)
    {
      /* Free the UIM buffer for next request */
      hos_cmd_ptr ->hdr.slot = UIM_SLOT_AUTOMATIC;
      q_put( &uim_ptr->hotswap.hotswap_free_q, q_link(hos_cmd_ptr,&hos_cmd_ptr->hdr.cmd_hdr.link));
    }
    return FALSE;
  }

  /* check command is in progress */
  if (uim_ptr->flag.command_in_progress == TRUE ||  uim_ptr->flag.command_requeued == TRUE)
  {
  }

  /* Now UIM is good to process the card inserted , fetch the card inserted command */
  hos_cmd_ptr = (uim_cmd_type *)q_get(&uim_ptr->hotswap.hotswap_ins_cmd_q);
  if (hos_cmd_ptr == NULL)
  {
     return FALSE;
  }

  /* Read the slot form card inserted data */
  hos_curr_slot = hos_cmd_ptr ->hdr.slot;

  /* Free the UIM buffer for next request */
  hos_cmd_ptr ->hdr.slot = UIM_SLOT_AUTOMATIC;
  q_put( &uim_ptr->hotswap.hotswap_free_q, q_link(hos_cmd_ptr,&hos_cmd_ptr->hdr.cmd_hdr.link));

  if ((hos_curr_slot != UIM_SLOT_1) &&
      (hos_curr_slot != UIM_SLOT_2) &&
      (hos_curr_slot != UIM_SLOT_3))
  {
    return FALSE;
  }

  /* Check the Command pointer which is running and notify error for that command */
  if ((uim_ptr->command.cmd_ptr != NULL) && (uim_ptr->flag.command_in_progress == TRUE) && (uim_ptr->command.cmd_ptr->hdr.slot ==hos_curr_slot) &&
      (!uim_ptr->flag.static_buffer_used)&& (uim_ptr->flag.command_requeued == FALSE))
  {
    UIMDRV_PUP_MSG_HIGH_2(UIMLOG_MSG_141,uim_ptr->id,"HOTSWAP: Reporting err for current cmd: 0x%x status: 0x%x",
                      uim_ptr->command.cmd_ptr->hdr.command,
                      uim_ptr->command.cmd_ptr->hdr.status);

    if (((int)uim_ptr->command.cmd_ptr->hdr.options & (int)UIM_OPTION_ALWAYS_RPT) != 0)
    {
      /*reset the command transacted flag as response 
      is not succesfully received from card*/
      uim_ptr->command.rpt_buf.cmd_transacted = FALSE;

      /* Set the status words to default before sending the rpt */
      uim_ptr->command.rpt_buf.sw1 = (uim_sw1_type) 0;
      uim_ptr->command.rpt_buf.sw2 = (uim_sw2_type) 0;
      uim_send_err_rpt(uim_ptr->command.cmd_ptr, uim_ptr);
    }
    /* return buffer to done queue */
    else if (uim_ptr->command.cmd_ptr->hdr.cmd_hdr.done_q_ptr != NULL)
    {
      /* place command on requested queue */
      q_put( uim_ptr->command.cmd_ptr->hdr.cmd_hdr.done_q_ptr, &uim_ptr->command.cmd_ptr->hdr.cmd_hdr.link);
    }
  }

  uim_hotswap_reset_slot(uim_ptr);

  if (uim_hw_if.clkreg[uim_ptr->id].m_simMaxFreqSupported == UIMDRV_CLK_FREQ_4_8_MHZ)
  {
    UIM_RESET_CLK_FREQ_TO_DEFAULT_4_8_MHZ(uim_ptr->id);
    SET_UIM_BAUD_RATE_SLOT ( uim_ptr->id, UIM_CRCF_372_1, UIM_BRAF_1 );
  }

  uim_ptr->flag.static_buffer_used             = TRUE;
  uim_ptr->command.static_cmd_buf.hdr.slot   = hos_curr_slot;

  /* command to power up the UIM due to task start up */
  /* Command has not been attempted yet */
  uim_ptr->command.rpt_buf.cmd_transacted                   = FALSE;
  uim_ptr->command.static_cmd_buf.hdr.command               = UIM_HOTSWAP_CARD_INS_F;

  if ( uim_ptr->hotswap.hotswap_first_ins_class == NV_UIM_FIRST_INST_CLASS_GSM_SIM)
  {
    uim_ptr->command.static_cmd_buf.hdr.protocol              = UIM_ICC;
  }
  else
  {
    uim_ptr->command.static_cmd_buf.hdr.protocol              = UIM_UICC;
  }
  uim_ptr->command.static_cmd_buf.hdr.channel               = UIM_CHANNEL0;
  uim_ptr->command.static_cmd_buf.hdr.cmd_hdr.task_ptr      = uim_ptr->tcb_ptr;
  uim_ptr->command.static_cmd_buf.hdr.cmd_hdr.done_q_ptr    = NULL;
  uim_ptr->command.static_cmd_buf.hdr.cmd_hdr.sigs          = (rex_sigs_type)NULL;
  uim_ptr->command.static_cmd_buf.hdr.cmd_hdr.link.next_ptr = NULL;
  uim_ptr->command.static_cmd_buf.hdr.rpt_function          = NULL;
  uim_ptr->command.static_cmd_buf.select.path_position      = 0;
  /* This is the first attempt for this command  */
  uim_ptr->command.static_cmd_buf.hdr.cmd_count             = 1;
  uim_ptr->hotswap.hotswap_notify_card_removed              = FALSE;
  uim_ptr->flag.internal_command_used                       = FALSE;
  uim_ptr->flag.command_in_progress                         = TRUE;

  if((TRUE == uim_ptr->extended_recovery.extended_recovery_enabled) &&
     (TRUE == uim_ptr->extended_recovery.extended_recovery_in_progress))
  {
    /*abandon the extended recovery and clear the relevent globals*/
    uim_extended_recovery_event_handler(uim_ptr, UIM_EXTENDED_RECOVERY_EVT_TERMINATE);
  }

  // flush all pending commands in queue
  uim_flush_command(UIM_FLUSH_ALL, uim_ptr);

  uim_ptr->debug.tot_recovery_count = 0;

  UIMDRV_PUP_MSG_HIGH_0(UIMLOG_MSG_142,uim_ptr->id,"HOTSWAP:executing UIM_HOTSWAP_CARD_INS_F");

  /* Make command pointer point to static command buffer */
  uim_ptr->command.cmd_ptr = &uim_ptr->command.static_cmd_buf;

  return TRUE;
} /* uim_hotswap_process_card_inserted_sig */


/**
 * Process card removed signal in UIM task.
 *
 * @param uim_ptr Pointer to the global data common.
 *
 * @return boolean Success or Fail.
 */
boolean uim_hotswap_process_card_removed_sig(
  uim_instance_global_type  *uim_ptr)
{
  uim_slot_type   hos_curr_slot    = UIM_SLOT_NONE;
  uim_cmd_type   *hos_cmd_ptr      = NULL; /* Pointer to received command */
  /* Received card removed signal */

  if (uim_ptr == NULL)
  {
   return FALSE;
  }
 
  hos_cmd_ptr = (uim_cmd_type *)q_check(&uim_ptr->hotswap.hotswap_rem_cmd_q);
  if (hos_cmd_ptr == NULL)
  {
    return FALSE;
  }

  /* Check the slot value */
  if ((hos_cmd_ptr->hdr.slot != UIM_SLOT_1) &&
      (hos_cmd_ptr->hdr.slot != UIM_SLOT_2) &&
      (hos_cmd_ptr->hdr.slot != UIM_SLOT_3))
  {
    hos_cmd_ptr = (uim_cmd_type *)q_get(&uim_ptr->hotswap.hotswap_rem_cmd_q);
    if (hos_cmd_ptr != NULL)
    {
      /* Free the UIM buffer for next request */
      hos_cmd_ptr ->hdr.slot = UIM_SLOT_AUTOMATIC;
      q_put( &uim_ptr->hotswap.hotswap_free_q,
             q_link(hos_cmd_ptr,&hos_cmd_ptr->hdr.cmd_hdr.link));
    }
    return FALSE;
  }

  /* Check the current card status before processing the command */
  if (uim_ptr->hotswap.card_status == UIM_CARD_INSERTED || uim_ptr->hotswap.hotswap_me_power_down == TRUE)
  {
    UIMDRV_PUP_MSG_HIGH_0(UIMLOG_MSG_143,uim_ptr->id,"HOTSWAP: Drop Card removed signal as card is inserted or ME is in power down state");
    hos_cmd_ptr = (uim_cmd_type *)q_get(&uim_ptr->hotswap.hotswap_rem_cmd_q);
    if (hos_cmd_ptr != NULL)
    {
      /* Free the UIM buffer for next request */
      hos_cmd_ptr ->hdr.slot = UIM_SLOT_AUTOMATIC;
      q_put( &uim_ptr->hotswap.hotswap_free_q, q_link(hos_cmd_ptr,&hos_cmd_ptr->hdr.cmd_hdr.link));
    }
    return FALSE;
  }

  /* Now UIM is good to process the card removed, get the card removed command */
  hos_cmd_ptr = (uim_cmd_type *)q_get(&uim_ptr->hotswap.hotswap_rem_cmd_q);
  if (hos_cmd_ptr == NULL)
  {
     return FALSE;
  }
  /* Get the slot information */
  hos_curr_slot = hos_cmd_ptr ->hdr.slot;

  /* Free the buffer for next request */
  hos_cmd_ptr ->hdr.slot = UIM_SLOT_AUTOMATIC;
  q_put( &uim_ptr->hotswap.hotswap_free_q, q_link(hos_cmd_ptr,&hos_cmd_ptr->hdr.cmd_hdr.link));

  /* check for valid slot */
  if ((hos_curr_slot != UIM_SLOT_1) &&
      (hos_curr_slot != UIM_SLOT_2) &&
      (hos_curr_slot != UIM_SLOT_3))
  {
    return FALSE;
  }

  /* Following checks are needed to send error report for current command in progress
     1. Command Pointer is not null
     2. Command is currently in progress
     3. Slot mentioned in the command is same as the one for which we received hotswap
        card removed indication
     4. External commands. In the case of internal SELECT command it will be handle by flushing.
     5. check uim_command_requeued for similiar purpose as 4*/

  /* Reporting error of current command, if any */
  if ( (uim_ptr->command.cmd_ptr != NULL) &&  (uim_ptr->flag.command_in_progress == TRUE) && ( uim_ptr->command.cmd_ptr->hdr.slot == hos_curr_slot )  &&
       (!uim_ptr->flag.static_buffer_used) && (uim_ptr->flag.command_requeued == FALSE))
  {
    UIMDRV_PUP_MSG_HIGH_2(UIMLOG_MSG_144,uim_ptr->id,"HOTSWAP: Reporting err for current cmd: 0x%x status: 0x%x",
                      uim_ptr->command.cmd_ptr->hdr.command,
                      uim_ptr->command.cmd_ptr->hdr.status);

    if (((int)uim_ptr->command.cmd_ptr->hdr.options & (int)UIM_OPTION_ALWAYS_RPT) != 0)
    {
      /*reset the command transacted flag as response 
      is not succesfully received from card*/
      uim_ptr->command.rpt_buf.cmd_transacted = FALSE;

      /* Set the status words to default before sending the rpt */
      uim_ptr->command.rpt_buf.sw1 = (uim_sw1_type) 0;
      uim_ptr->command.rpt_buf.sw2 = (uim_sw2_type) 0;
      uim_send_err_rpt(uim_ptr->command.cmd_ptr, uim_ptr);
    }
  }

  /* Flush immediately all pending command on that slot */
  uim_flush_command(UIM_FLUSH_ALL, uim_ptr);

  uim_hotswap_reset_slot(uim_ptr);
  uim_ptr->flag.static_buffer_used             = TRUE;
  uim_ptr->command.static_cmd_buf.hdr.slot   = hos_curr_slot;


  /* Command has not been attempted yet */
  uim_ptr->command.rpt_buf.cmd_transacted                   = FALSE;
  /* command to power up the UIM due to task start up */
  uim_ptr->command.static_cmd_buf.hdr.command               = UIM_HOTSWAP_CARD_REM_F;
  uim_ptr->command.static_cmd_buf.hdr.protocol              = UIM_UICC;
  uim_ptr->command.static_cmd_buf.hdr.channel               = UIM_CHANNEL0;
  uim_ptr->command.static_cmd_buf.pdown.pdown_option        = UIM_OPTION_NOTIFY_PDOWN;
  uim_ptr->command.static_cmd_buf.hdr.cmd_hdr.task_ptr      = uim_ptr->tcb_ptr;
  uim_ptr->command.static_cmd_buf.hdr.cmd_hdr.done_q_ptr    = NULL;
  uim_ptr->command.static_cmd_buf.hdr.cmd_hdr.sigs          = (rex_sigs_type)NULL;
  uim_ptr->command.static_cmd_buf.hdr.cmd_hdr.link.next_ptr = NULL;
  uim_ptr->command.static_cmd_buf.hdr.rpt_function          = NULL;
  /* The status of UIM is set to Removed state */
  uim_ptr->state.status                                        = UIM_ERR_S;
  /* !!!!! Do not clear the ATCSIM directory structure !!!!! */

  if((TRUE == uim_ptr->extended_recovery.extended_recovery_enabled) &&
     (TRUE == uim_ptr->extended_recovery.extended_recovery_in_progress))
  {
    /*abandon the extended recovery and clear the relevent globals*/
    uim_extended_recovery_event_handler(uim_ptr, UIM_EXTENDED_RECOVERY_EVT_TERMINATE);
  }

  uim_notify_error(UIM_CARD_REMOVED_S, uim_ptr);

  if((uim_ptr->extended_recovery.recovery_config.extended_powerup_attempts_upon_card_removal) &&
     (uim_ptr->hotswap.hotswap_flaky_switch_support == TRUE) &&
     (uim_ptr->extended_recovery.extended_recovery_enabled == TRUE) &&
     (uim_ptr->hotswap.retry_power_up_count > 0))
  {
    if((uim_ptr->atr.is_first_atr_received) ||
       (uim_ptr->extended_recovery.recovery_config.extended_powerup_attempts_from_boot_up == TRUE))
    {
      UIMDRV_MSG_HIGH_1(uim_ptr->id,"Extended recovery triggering with attempt left 0x%x", uim_ptr->hotswap.retry_power_up_count);
      uim_ptr->hotswap.retry_power_up_count-- ;
      uim_extended_recovery_event_handler(uim_ptr, UIM_EXTENDED_RECOVERY_EVT_TIMER_START); 
    }
  }

  uim_ptr->flag.internal_command_used                         = FALSE;
  uim_ptr->flag.command_in_progress                           = TRUE;
  UIMDRV_PUP_MSG_HIGH_0(UIMLOG_MSG_145,uim_ptr->id,"HOTSWAP: executing UIM_HOTSWAP_CARD_REM_F");

  /* Make command pointer point to static command buffer */
  uim_ptr->command.cmd_ptr = &uim_ptr->command.static_cmd_buf;

  return TRUE;
} /* uim_hotswap_process_card_removed_sig */


/**
 * HOTSWAP ISR for the PMIC\GPIO used to communicate with the
 * UIM.
 *
 * @param uim_ptr Pointer to the global data common.
 * @param hos_need_to_query Request the current status.
 * @param hos_new_card_status New HOTSWAP status.
 */
void uim_hotswap_isr(uim_instance_global_type *uim_ptr,
                     boolean                   hos_need_to_query,
                     uim_card_swap_enum_type   hos_new_card_status)
{
  if(NULL == uim_ptr)
  {
    return;
  }

  RETURN_IF_INSTANCE_INVALID(uim_ptr->id);

  if(uim_hw_if.intctrl[uim_ptr->id].m_hs_enable == TRUE)
  {
    uim_card_swap_enum_type uim_query_card_status = UIM_CARD_UNCHANGED;

    /* Check if we are already in debounce logic then do nothing */
    if (uim_ptr->hotswap.hotswap_debounce_in_progress == TRUE)
    {
      return;
    }

    /* Mask hotswap isr */
    uim_hotswap_mask_interrupt(uim_ptr);
    /* Get the query status */
    if (hos_need_to_query)
    {
      uim_query_card_status = uim_hotswap_query_card_status(uim_ptr);
    }
    else
    {
      uim_query_card_status = hos_new_card_status;
    }
    if(uim_query_card_status == uim_ptr->hotswap.card_status)
    {
      if(UIM_CARD_INSERTED == uim_query_card_status)
      {
        uim_query_card_status = UIM_CARD_REMOVED;
      }
      else
      {
        uim_query_card_status = UIM_CARD_INSERTED;
      }
    }

    /* Clear the interrupt */
    uim_hotswap_clear_interrupt(uim_ptr);

    /* Check if there is any change in the card detection status */
    if(uim_query_card_status != UIM_CARD_UNCHANGED)
    {
      if (uim_query_card_status == UIM_CARD_INSERTED)
      {
        UIMDRV_PUP_MSG_HIGH_0(UIMLOG_MSG_146,uim_ptr->id,"HOTSWAP: Start, For CARD INSERTED Interrupt ");
        if((uim_ptr->extended_recovery.extended_recovery_enabled) &&
           (uim_ptr->extended_recovery.extended_recovery_in_progress))
        {
          /*abandon the extended recovery and clear the relevent globals*/
          uim_extended_recovery_event_handler(uim_ptr, UIM_EXTENDED_RECOVERY_EVT_TERMINATE);
        }
      }
      else
      {
        UIMDRV_PUP_MSG_HIGH_0(UIMLOG_MSG_147,uim_ptr->id, "HOTSWAP: Start, For CARD REMOVED Interrupt");
      }
      uim_hotswap_start_debounce_timer(uim_query_card_status, uim_ptr);
    }
    else
    {
      /* The card detection status has not changed,
         enable card detect interrupt again */
      uim_hotswap_unmask_interrupt (uim_ptr);
    }
  }
} /* uim_hotswap_isr */


/**
 * HOTSWAP ISR for GPIO interrupt.
 *
 * @param uim_ptr Pointer to the global data common.
 */
void uim_hotswap_gpio_isr(uim_instance_global_type *uim_ptr)
{
  uim_instance_global_type *uim_ptr_temp;
  uint8 i = 0;

  if (uim_ptr == NULL || uim_ptr->id >= UIM_MAX_INSTANCES)
  {
    return;
  }

  /* For non UIM CTRL based hotswap, call ISR for all the instance
     that shares the GPIO */
  if(uim_hotswap_nature[uim_ptr->id] != UIM_CONTROLLER_BASED)
  {
    for(i=0;i<UIM_MAX_NUMBER_INSTANCES;i++)
    {
      if( uim_hotswap_nature[uim_ptr->id] == uim_hotswap_nature[i])
      {
        uim_ptr_temp = uim_get_instance_ptr((uim_instance_enum_type) i);
        if((uim_ptr_temp != NULL) && (uim_ptr_temp->hotswap.is_masked == 0))
        {
          /* The last parameter is ignored when the second parameter is TRUE  */
          uim_hotswap_isr(uim_ptr_temp, TRUE, UIM_CARD_UNCHANGED);
        }
      }
    }
  }
  else
  {
    /* The last parameter is ignored when the second parameter is TRUE  */
    uim_hotswap_isr(uim_ptr, TRUE, UIM_CARD_UNCHANGED);
  }
} /* uim_hotswap_gpio_isr */


/**
 * Retrieves the real time card status of given slot.
 *
 * @param uim_slot Slot number.
 *
 * @return uim_rt_status_enum Real time HOTSWAP status.
 */
uim_rt_status_enum  uim_get_rt_status(uim_slot_type  uim_slot)
{
  /* Since this function gets called from QMI map uim slot
     to instance id */
  uim_instance_global_type *uim_ptr = NULL;
  uim_card_swap_enum_type   uim_rt_status;

  /*convert logical to physical slot ID*/
  uim_slot = uim_convert_logical_to_physical_slot_id(uim_slot);

  uim_ptr = uim_get_instance_ptr((uim_instance_enum_type) (uim_slot - 1));

  if(uim_ptr == NULL)
  {
    UIM_MSG_ERR_0("uim_get_rt_status: uim_ptr is NULL due to bad uim_slot");
    return UIM_RT_STATUS_NOT_SUPPORTED;
  }

  if(uim_ptr->id >= UIM_INSTANCE_INVALID)
  {
    UIM_MSG_ERR_1("uim_get_rt_status: Invalid uim instance id - 0x%x",
                   uim_ptr->id);
    return UIM_RT_STATUS_NOT_SUPPORTED;
  }

  if(uim_hw_if.intctrl[uim_ptr->id].m_hs_enable == TRUE)
  {
    uim_rt_status =  uim_hotswap_query_card_status (uim_ptr);
    switch (uim_rt_status)
    {
      case UIM_CARD_REMOVED:
        return UIM_RT_STATUS_CARD_ABSENT;
      case UIM_CARD_INSERTED:
        return UIM_RT_STATUS_CARD_PRESENT;
      default:
        UIM_MSG_ERR_1("Unknown status received for slot 0x%x", uim_slot);
    }
  }
  return UIM_RT_STATUS_NOT_SUPPORTED;
}/* uim_get_rt_status */


/**
 * Disable the hotswap interrupt for given slot
 *
 * @param uim_slot Slot number
 */
void uim_disable_hotswap_interrupt(uim_slot_type uim_slot)
{
  uim_instance_global_type *uim_ptr = NULL;

  /*convert logical to physical slot ID*/
  uim_slot = uim_convert_logical_to_physical_slot_id(uim_slot);

  uim_ptr = uim_get_instance_ptr((uim_instance_enum_type) (uim_slot - 1));

  if(uim_ptr == NULL)
  {
    UIM_MSG_ERR_0("uim_disable_hotswap_interrupt: uim_ptr is NULL due to bad uim_slot");
    return;
  }

  RETURN_IF_INSTANCE_INVALID(uim_ptr->id);

  if(uim_hw_if.intctrl[uim_ptr->id].m_hs_enable == TRUE)
  {
    uim_ptr->hotswap.hotswap_disabled_uicc_seq_off = TRUE;

    UIMDRV_PUP_MSG_HIGH_0(UIMLOG_MSG_148,uim_ptr->id, "Disabled hotswap");
  }
}/* uim_disable_hotswap_interrupt */


/**
 * Query the real-time HOTSWAP status from GPIO registers
 *
 * @param uim_ptr Pointer to the global data common.
 *
 * @return uim_card_swap_enum_type
 */
uim_card_swap_enum_type uim_query_gpio_based_card_status (
  uim_instance_global_type *uim_ptr )
{
  DALGpioValueType nVal;

  if(uim_ptr == NULL || uim_ptr->id >= UIM_MAX_INSTANCES)
  {
    return UIM_CARD_INSERTED;
  }

  DalTlmm_SelectGpioIdMode(uim_hw_if.gpio[uim_ptr->id].m_TlmmHandle_ptr,
							uim_hw_if.gpio[uim_ptr->id].m_PresentGpioId,
							UIM_TLMM_IO_MODE,
							&(uim_hw_if.gpio[uim_ptr->id].m_PresentGpioConfigId));

  DalTlmm_GpioIdIn(uim_hw_if.gpio[uim_ptr->id].m_TlmmHandle_ptr, uim_hw_if.gpio[uim_ptr->id].m_PresentGpioId, &nVal);

  if (nVal == uim_ptr->hotswap.hotswap_capability.nGPIOInsertedLevel)
  {
    return UIM_CARD_INSERTED;
  }
  else
  {
    return UIM_CARD_REMOVED;
  }
}/* uim_query_gpio_based_card_status */


/**
 * Query the real-time HOTSWAP status
 *
 * @param uim_ptr Pointer to the global data common.
 *
 * @return uim_card_swap_enum_type
 */
uim_card_swap_enum_type uim_hotswap_query_card_status(
  uim_instance_global_type *uim_ptr)
{

  uim_card_swap_enum_type card_status =  UIM_CARD_INSERTED;
  /* The default value of query card status is always set to UIM_CARD_INSERTED */
  if (uim_ptr == NULL || uim_ptr->id >= UIM_MAX_INSTANCES)
  {
    return UIM_CARD_INSERTED;
  }

  if(uim_hw_if.intctrl[uim_ptr->id].m_hs_enable == FALSE
      || IS_BTSAP_HANDLE_ACTIVE(uim_ptr)
     )
  {
    return card_status;
  }
  if(uim_hotswap_nature[uim_ptr->id] != UIM_CONTROLLER_BASED)
  {
    card_status = uim_query_gpio_based_card_status(uim_ptr);
  }
  else
  {
    /* uim_hos_disabled_uicc_seq_off flag is used to disable the hotswap
       capability at runtime, UIM will not allow to make a card status query,
       when this flag is TRUE and this function return as card inserted */
    if (uim_ptr->hotswap.hotswap_disabled_uicc_seq_off == FALSE)
    {
      card_status = uim_uartdm_uim_controller_card_presence(uim_ptr);
    }
  }
  UIMDRV_MSG_LOW_1(uim_ptr->id,"HOTSWAP: uim_hotswap_query_card_status 0x%x",
                   card_status);
  return card_status;
} /* uim_hotswap_query_card_status */


/**
 * Mask the hotswap interrupt.
 *
 * @param uim_ptr Pointer to the global data common.
 */
void uim_hotswap_mask_interrupt(uim_instance_global_type *uim_ptr)
{
  if (uim_ptr == NULL || uim_ptr->id >= UIM_MAX_INSTANCES)
  {
    return;
  }

  if(uim_hw_if.intctrl[uim_ptr->id].m_hs_enable == FALSE)
  {
    return;
  }

  uim_hotswap_clear_debounce_timer(uim_ptr);

  if(uim_hotswap_nature[uim_ptr->id] != UIM_CONTROLLER_BASED)
  {
    uim_ptr->hotswap.is_masked = TRUE;
    UIMDRV_MSG_HIGH_0(uim_ptr->id,"Masked GPIO based hotswap interrupt");
    /*
      DalTlmm_GetGpioIdSettings((uim_hw_if.gpio[uim_ptr->id].m_TlmmHandle_ptr),
                                (uim_hw_if.gpio[uim_ptr->id].m_PresentGpioId),
                                &GpioSettings);
      ngpio = GpioSettings.nGpioNumber;
      GPIOInt_DeregisterIsr((uim_hw_if.intctrl[uim_ptr->id].m_UimCtrl_handle_ptr),
                             ngpio, (GPIOINTISR)uimUimctrlIsr); */

    /* Ideally, during un-masking the above de-regisrtation function can be called
       But the irq is allowed and the mask flag is checked to allow it to server or not */
  }
  else
  {
    UIMDRV_MSG_LOW_0(uim_ptr->id,"Masked UIM controller hotswap interrupt");
    uim_uartdm_uim_controller_mask_carddet_int(uim_ptr);
  }
} /* uim_hotswap_mask_interrupt */


/**
 * Clear the hotswap interrupt.
 *
 * @param uim_ptr Pointer to the global data common.
 */
void uim_hotswap_clear_interrupt(uim_instance_global_type *uim_ptr)
{
  if (uim_ptr == NULL || uim_ptr->id >= UIM_MAX_INSTANCES)
  {
    return;
  }

  if(uim_hw_if.intctrl[uim_ptr->id].m_hs_enable == FALSE)
  {
    return;
  }
  uim_uartdm_uim_controller_clear_carddet_int(uim_ptr);
  UIMDRV_MSG_LOW_0(uim_ptr->id,"Cleared GPIO interrupt");
} /* uim_hotswap_clear_interrupt  */


/**
 * Unmask the hotswap interrupt.
 *
 * @param uim_ptr Pointer to the global data common.
 */
void uim_hotswap_unmask_interrupt(uim_instance_global_type *uim_ptr)
{
  uint32 ngpio;
  DalTlmm_GpioIdSettingsType GpioSettings;
  uim_hotswap_slot_capability  uim_hos_capability     = {FALSE, FALSE, 0, FALSE, 0, 0};

  if(uim_ptr == NULL || uim_ptr->id >= UIM_MAX_INSTANCES)
  {
    return;
  }
  uim_hos_capability = uim_ptr->hotswap.hotswap_capability;

  RETURN_IF_INSTANCE_INVALID(uim_ptr->id);

  if(uim_hw_if.intctrl[uim_ptr->id].m_hs_enable == FALSE ||
     uim_ptr->hotswap.hotswap_disabled_uicc_seq_off == TRUE
      || IS_BTSAP_HANDLE_ACTIVE(uim_ptr)
     )
  {
    return;
  }

  if(uim_hotswap_nature[uim_ptr->id] != UIM_CONTROLLER_BASED)
  {
    DalTlmm_GetGpioIdSettings((uim_hw_if.gpio[uim_ptr->id].m_TlmmHandle_ptr),(uim_hw_if.gpio[uim_ptr->id].m_PresentGpioId)
                               , &GpioSettings);
    ngpio = GpioSettings.nGpioNumber;
    uim_ptr->hotswap.is_masked = 0;
    GPIOInt_RegisterIsr((uim_hw_if.intctrl[uim_ptr->id].m_UimCtrl_handle_ptr), ngpio, GPIOINT_TRIGGER_DUAL_EDGE, (GPIOINTISR)uimUimctrlIsr,
                                           (GPIOINTISRCtx) uim_ptr);
    UIMDRV_MSG_HIGH_1(uim_ptr->id,"unmasked the GPIO 0x%x based hotswap interrupt", ngpio);
    return;
  }
  /* When card_removed_suspicious flag is TRUE, check the card_status to un-mask the interrupt */
  if((uim_ptr->hotswap.hotswap_flaky_switch_support)
      && (uim_ptr->hotswap.hotswap_card_removed_suspicious == TRUE))
  {

    uim_ptr->hotswap.hotswap_card_removed_suspicious = FALSE;
    if(UIM_CARD_REMOVED == uim_ptr->hotswap.card_status)
    {
      uim_uartdm_uim_controller_card_ins_int(uim_ptr->id,
                                           uim_hos_capability.nGPIOInsertedLevel);
    }
    else if (UIM_CARD_INSERTED == uim_ptr->hotswap.card_status)
    {
      uim_uartdm_uim_controller_card_rem_int(uim_ptr->id,
                                           uim_hos_capability.nGPIOInsertedLevel);
    }
    return;
  }

  if (UIM_CARD_REMOVED == uim_hotswap_query_card_status(uim_ptr))
  {
    /* Do not unmask card inserted interrupt until it is confirmed that
       the card is really removed */
    if((!uim_ptr->hotswap.hotswap_flaky_switch_support)
        ||((UIM_CARD_REMOVED == uim_ptr->hotswap.card_status)) )
    {
      uim_uartdm_uim_controller_card_ins_int(uim_ptr->id,
                                             uim_hos_capability.nGPIOInsertedLevel);
    }
  }
  else if (UIM_CARD_INSERTED == uim_hotswap_query_card_status(uim_ptr))
  {
    uim_uartdm_uim_controller_card_rem_int(uim_ptr->id,
                                           uim_hos_capability.nGPIOInsertedLevel);
  }
} /* uim_hotswap_unmask_interrupt */


/**
 * Query the hotswap capability.
 *
 * @param uim_slot Slot number.
 *
 * @return boolean True if slot have the hotswap capability
 */
boolean uim_is_hotswap_enabled_slot( uim_slot_type uim_slot )
{
  uim_instance_global_type *uim_ptr = uim_get_instance_ptr((uim_instance_enum_type)(uim_slot - 1));

  /*convert logical to physical slot ID*/
  uim_slot = uim_convert_logical_to_physical_slot_id(uim_slot);

  if(uim_ptr == NULL)
  {
    UIM_MSG_ERR_1("Invalid input param uim_slot 0x%x", uim_slot);
    return FALSE;
  }

  if((uim_ptr->id < UIM_MAX_INSTANCES)
      &&(uim_hw_if.intctrl[uim_ptr->id].m_hs_enable == TRUE))
  {
    uim_hotswap_slot_capability  uim_hos_capability = {FALSE, FALSE, 0, FALSE, 0, 0};

    uim_hos_capability = uim_get_hotswap_enabled_slot(uim_ptr);

    return uim_hos_capability.hos_enabled;
  }
  else
  {
    return FALSE;
  }
} /* uim_is_hotswap_enabled_slot */


/**
 * DEFINITIONS OF INTERNAL FUNTIONS
 *
 */
/**
 * Send the card removed cmd to uim task.
 *
 * @param uim_ptr Pointer to the global data common.
 *
 * @return boolean Success or Fail.
 */
boolean uim_hotswap_send_card_removed_cmd(
  uim_instance_global_type *uim_ptr)
{
  uim_cmd_type *hos_cmd_ptr = NULL;

  if(uim_ptr == NULL || uim_ptr->id >= UIM_MAX_INSTANCES)
  {
    return FALSE;
  }

  if( uim_hw_if.intctrl[uim_ptr->id].m_hs_enable == TRUE
      || IS_BTSAP_HANDLE_ACTIVE(uim_ptr)
    )
  {
    /* Allocate command buffer space */
    if((q_cnt(& uim_ptr->hotswap.hotswap_rem_cmd_q) * UIM_NUM_DRV_SLOTS) >= UIM_HOTSWAP_NUM_CMD_BUFS)
    {
      UIMDRV_MSG_LOW_0(uim_ptr->id,"HOTSWAP:UIM was not able to process the card removed req" );
    }
    else if ((hos_cmd_ptr = (uim_cmd_type *)q_get( &uim_ptr->hotswap.hotswap_free_q)) != NULL)
    {
      hos_cmd_ptr->hdr.slot      = (uim_slot_type)(uim_ptr->id + 1);
      hos_cmd_ptr->hdr.channel   = UIM_DEFAULT_CHANNEL;
      hos_cmd_ptr->hdr.cmd_count = 0;
      q_put(&uim_ptr->hotswap.hotswap_rem_cmd_q, q_link(hos_cmd_ptr, &hos_cmd_ptr->hdr.cmd_hdr.link));
      UIMDRV_MSG_HIGH_0(uim_ptr->id,
                        "HOTSWAP: uim_hotswap_send_card_removed_cmd is successfully send to uim");
      (void) rex_set_sigs( uim_ptr->tcb_ptr, UIM_HOTSWAP_CMD_CARD_REM_SIG);
      if(IS_BTSAP_HANDLE_ACTIVE(uim_ptr))
      {
        (void) rex_clr_timer( &uim_ptr->remote.remote_cmd_rsp_timer);
      }
      return TRUE;
    }
  }

  UIMDRV_MSG_HIGH_0(uim_ptr->id,
                    "HOTSWAP: uim_hotswap_send_card_removed_cmd is failed to send the UIM");
  return FALSE;
}/* uim_hotswap_send_card_removed_cmd */


/**
 * Send the card inserted cmd to uim task.
 *
 * @param uim_ptr Pointer to the global data common.
 *
 * @return boolean Success or Fail.
 */
static boolean uim_hotswap_send_card_inserted_cmd(
  uim_instance_global_type *uim_ptr)
{
  uim_cmd_type *hos_cmd_ptr = NULL;

  if(uim_ptr == NULL)
  {
   return FALSE;
  }

  if(uim_ptr->id >= UIM_MAX_INSTANCES)
  {
    return FALSE;
  }

  if( uim_hw_if.intctrl[uim_ptr->id].m_hs_enable == TRUE
      || IS_BTSAP_HANDLE_ACTIVE(uim_ptr)
    )
  {
    /* Allocate command buffer space */
    if ((q_cnt(& uim_ptr->hotswap.hotswap_ins_cmd_q) *  UIM_NUM_DRV_SLOTS) >= UIM_HOTSWAP_NUM_CMD_BUFS)
    {
      UIMDRV_MSG_ERR_0(uim_ptr->id,"HOTSWAP:Card inserted req failed" );
    }
    else if ((hos_cmd_ptr = (uim_cmd_type *)q_get( &uim_ptr->hotswap.hotswap_free_q)) != NULL)
    {
      hos_cmd_ptr->hdr.slot      = (uim_slot_type)(uim_ptr->id + 1);
      hos_cmd_ptr->hdr.channel   = UIM_DEFAULT_CHANNEL;
      hos_cmd_ptr->hdr.cmd_count = 0;
      q_put(&uim_ptr->hotswap.hotswap_ins_cmd_q, q_link(hos_cmd_ptr, &hos_cmd_ptr->hdr.cmd_hdr.link));
      (void) rex_set_sigs( uim_ptr->tcb_ptr, UIM_HOTSWAP_CMD_CARD_INS_SIG);
      return TRUE;
    }
  }
  return FALSE;
}/* uim_hotswap_send_card_inserted_cmd */


/**
 * Define the HOTSWAP debounce timer.
 *
 * @param uim_ptr Pointer to the global data common.
 */
static void uim_hotswap_define_debounce_timer(uim_instance_global_type *uim_ptr)
{
  if(uim_ptr == NULL || uim_ptr->id >= UIM_MAX_INSTANCES)
  {
    return;
  }

  if(uim_hw_if.intctrl[uim_ptr->id].m_hs_enable == TRUE)
  {
    timer_def(&(uim_ptr->hotswap.hotswap_slot_debounce_timer), &uim_ptr->task.always_on_timer_group, NULL, 0,
              uim_hotswap_debounce_timer_expiry_cb, (timer_cb_data_type)uim_ptr);
  }
} /* uim_hotswap_define_debounce_timer */


/**
 * Reset the HOTSWAP debounce timer.
 *
 * @param hos_new_card_status New HOTSWAP status.
 * @param uim_ptr Pointer to the global data common.
 */
static void uim_hotswap_reset_debounce_timer(
  uim_card_swap_enum_type    hos_new_card_status,
  uim_instance_global_type  *uim_ptr)
{
  if(uim_ptr == NULL || uim_ptr->id >= UIM_MAX_INSTANCES)
  {
    return;
  }

  if(uim_hw_if.intctrl[uim_ptr->id].m_hs_enable == FALSE)
  {
    return;
  }

  timer_clr(&(uim_ptr->hotswap.hotswap_slot_debounce_timer), T_SCLK);

#ifdef FEATURE_UIM_TEST_FRAMEWORK
    #error code not present
#endif /* FEATURE_UIM_TEST_FRAMEWORK */

  if(hos_new_card_status == UIM_CARD_INSERTED )
  {
    timer_set( & uim_ptr->hotswap.hotswap_slot_debounce_timer,
               UIM_HOTSWAP_CARD_INSERTED_SAMPLE_PERIOD, 0, T_SCLK );
  }
  else
  {
    timer_set( &uim_ptr->hotswap.hotswap_slot_debounce_timer,
               UIM_HOTSWAP_CARD_REMOVED_SAMPLE_PERIOD, 0, T_SCLK );
  }
} /* uim_hotswap_reset_debounce_timer */


/**
 * Start the sample count and reset the debounce timer.
 *
 * @param hos_new_card_status New HOTSWAP status
 * @param uim_ptr Pointer to the global data common.
 */
static void uim_hotswap_start_debounce_timer(
  uim_card_swap_enum_type        hos_new_card_status,
  uim_instance_global_type      *uim_ptr)
{
  if(uim_ptr == NULL || uim_ptr->id >= UIM_MAX_INSTANCES)
  {
    return;
  }
  if(uim_hw_if.intctrl[uim_ptr->id].m_hs_enable == FALSE)
  {
   return;
  }
  uim_ptr->hotswap.hotswap_ins_sample_count       = 0x00;
  uim_ptr->hotswap.hotswap_rem_sample_count       = 0x00;
  uim_ptr->hotswap.hotswap_card_status            = hos_new_card_status;
  uim_ptr->hotswap.hotswap_debounce_retry_count   = 0x00;
  uim_ptr->hotswap.hotswap_debounce_in_progress   = TRUE;
  uim_hotswap_reset_debounce_timer(hos_new_card_status, uim_ptr);
} /* uim_hotswap_start_debounce_timer */


/**
 * Clear the uim hotswap timer
 *
 * @param uim_ptr Pointer to the global data common.
 */
static void uim_hotswap_clear_debounce_timer(uim_instance_global_type *uim_ptr)
{
  if(uim_ptr == NULL || uim_ptr->id >= UIM_MAX_INSTANCES)
  {
    return;
  }
  if(uim_hw_if.intctrl[uim_ptr->id].m_hs_enable == TRUE)
  {
    timer_clr(&(uim_ptr->hotswap.hotswap_slot_debounce_timer), T_SCLK);
    uim_ptr->hotswap.hotswap_debounce_in_progress   = FALSE;
  }
} /* uim_hotswap_clear_debounce_timer */


/**
 * This procedure is registered as a call-back associated with
 * the expiry of uim_hotswap_slot_debounce_timer, where we start
 * card status query for a fixed number of samples to get the
 * stable value of card status. This call-back is called in the
 * context of the high priority timer task. our sampling logic
 * based on the number of the allowed sample for card detection
 * and number of retrying count for this debounce. We have
 * different number for allowed sample count for card inserted
 * and card removed detection but Maximum debounce retry is same
 * for both query.
 *
 * @param instance_ptr Pointer to the global data common.
 */
static void uim_hotswap_debounce_timer_expiry_cb(unsigned long instance_ptr)
{
  uim_card_swap_enum_type   uim_new_query_status                  = UIM_CARD_UNCHANGED;
  uim_card_swap_enum_type   uim_curr_hos_card_status              = UIM_CARD_UNCHANGED;
  uint16                    *uim_hotswap_sample_couter_ptr        = NULL;
  uint16                    *uim_hotswap_debounce_retry_count_ptr = NULL;
  uint16                    uim_hotswap_max_sample_counter        = 0;
  boolean                   uim_hotswap_card_swap_debonce_end     = FALSE;
  boolean                   uim_hotswap_card_swap_debonce_success = FALSE;
  uim_instance_global_type *uim_ptr
    = (uim_instance_global_type*)instance_ptr;

  if(uim_ptr == NULL)
  {
    UIM_MSG_ERR_0("uim_hotswap_slot_debounce_timer_expiry_cb: uim_ptr is NULL");
    return;
  }

  RETURN_IF_INSTANCE_INVALID(uim_ptr->id);

  if(uim_hw_if.intctrl[uim_ptr->id].m_hs_enable == FALSE)
  {
    return;
  }

  /* Query for real time card status   */
  uim_new_query_status     = uim_hotswap_query_card_status (uim_ptr);

  /* Last ISR card status */
  uim_curr_hos_card_status =  uim_ptr->hotswap.hotswap_card_status;

  /* Get the debounce retry count */
  uim_hotswap_debounce_retry_count_ptr = &uim_ptr->hotswap.hotswap_debounce_retry_count;

  /* Get the sample count for query status */
  if (uim_curr_hos_card_status == UIM_CARD_INSERTED)
  {
    uim_hotswap_max_sample_counter = uim_ptr->hotswap.hotswap_me_conf.num_of_sample_for_insertion;
    uim_hotswap_sample_couter_ptr =  &uim_ptr->hotswap.hotswap_ins_sample_count;
  }
  else
  {
    uim_hotswap_max_sample_counter = uim_ptr->hotswap.hotswap_me_conf.num_of_sample_for_removal;
    uim_hotswap_sample_couter_ptr =  &uim_ptr->hotswap.hotswap_rem_sample_count;
  }

  /* Increment the sample counter */
  *uim_hotswap_sample_couter_ptr = *uim_hotswap_sample_couter_ptr +1;

  UIMDRV_MSG_HIGH_3(uim_ptr->id,"HOTSWAP: Debounce logic Sample Count 0x%x, Max Count 0x%x, RT Card Staus = 0x%x [1: Card Inserted]",
                    *uim_hotswap_sample_couter_ptr,
                    uim_hotswap_max_sample_counter,
                    uim_new_query_status);

  /* Check if the card detection status changes since the last ISR */
  if(uim_new_query_status == uim_curr_hos_card_status )
  {
    /* Query status and ISR status is same , continue sampling */
    if (*uim_hotswap_sample_couter_ptr >= uim_hotswap_max_sample_counter)
    {
      /* Debounce successfully completed !!!! */
      /* Store the stable new card detect status */
      /* send the hotswap signal to uim_task */
      uim_hotswap_card_swap_debonce_end = TRUE;
      uim_hotswap_card_swap_debonce_success = TRUE;
      UIMDRV_MSG_HIGH_1(uim_ptr->id,"HOTSWAP: Debounce logic ended successfully, card_status : 0x%x", 
                        uim_curr_hos_card_status);

      if (FALSE == uim_hotswap_set_signal(uim_curr_hos_card_status, uim_ptr))
      {
        UIMDRV_MSG_ERR_0(uim_ptr->id,"HOTSWAP: Failed to set the CARD INSERTED or REMOVED signal");
      }
    }
    else
    {
      /* continue for next sampling */
      uim_hotswap_reset_debounce_timer( uim_curr_hos_card_status, uim_ptr);
    }
  }
  else
  {
    /* Query status and ISR status is not same, continue for next retry,
       It reset the sample count again */
    *uim_hotswap_debounce_retry_count_ptr = *uim_hotswap_debounce_retry_count_ptr + 1;
    if(*uim_hotswap_debounce_retry_count_ptr >= uim_ptr->hotswap.hotswap_me_conf.maximum_debounce_retry_counter)
    {
      /* Debounce is completed no change in card status */
      UIMDRV_PUP_MSG_HIGH_1(UIMLOG_MSG_149,uim_ptr->id,"HOTSWAP: Debounce logic End, Unstable status: %d [0=REMOVED 1=INSERTED]",
                        uim_curr_hos_card_status);

      uim_hotswap_card_swap_debonce_end = TRUE;
    }
    else
    {
      /* Reset and start sampling again */
      *uim_hotswap_sample_couter_ptr  = 0;
      UIMDRV_MSG_HIGH_2(uim_ptr->id,"HOTSWAP: Debounce logic Maximum Retry 0x%x Current Count 0x%x",
                        uim_ptr->hotswap.hotswap_me_conf.maximum_debounce_retry_counter,
                        *uim_hotswap_debounce_retry_count_ptr);
      uim_hotswap_reset_debounce_timer(uim_curr_hos_card_status, uim_ptr);
    }
  }

  if(uim_hotswap_card_swap_debonce_end == TRUE)
  {
    *uim_hotswap_sample_couter_ptr        = 0;
    *uim_hotswap_debounce_retry_count_ptr = 0;
    uim_ptr->hotswap.hotswap_debounce_in_progress               = FALSE;
    if(TRUE == uim_hotswap_card_swap_debonce_success &&
       UIM_CARD_REMOVED == uim_curr_hos_card_status &&
       TRUE == uim_ptr->hotswap.hotswap_flaky_switch_support &&
       (UIM_CARD_TELECOM == uim_ptr->card_mode) &&
       (UIM_SLOT_STATE_ACTIVE == uim_ptr->activity_status))
    {
      UIMDRV_MSG_HIGH_0(uim_ptr->id,
      "Hotswap flaky switch enabled,keep the hotswap interuupts masked till the card state is determined");
    }
    else
    {
      uim_hotswap_unmask_interrupt(uim_ptr);
    }
  }
} /* uim_hotswap_debounce_timer_expiry_cb */


/**
 * Get hotswap capability
 *
 * @param uim_ptr Pointer to the global data common.
 *
 * @return uim_hotswap_slot_capability Hotswap capability
 */
static uim_hotswap_slot_capability uim_get_hotswap_enabled_slot(
  uim_instance_global_type *uim_ptr)
{
  uim_hotswap_slot_capability  uim_hos_capability;
  memset(&uim_hos_capability, 0x00, sizeof(uim_hos_capability));

  if (uim_ptr == NULL || uim_ptr->id >= UIM_MAX_INSTANCES)
  {
    return  uim_hos_capability;
  }

  if(uim_hw_if.intctrl[uim_ptr->id].m_hs_enable == FALSE)
  {
    return uim_hos_capability;
  }
  uim_hos_capability.is_uim_controller   = TRUE;
  uim_hos_capability.hos_enabled         = TRUE;
  uim_hos_capability.nGPIOInsertedLevel  = (DALGpioValueType)uim_hw_if.intctrl[uim_ptr->id].m_hs_polarity;
  uim_set_hotswap_polarity(uim_ptr->id, uim_hos_capability.nGPIOInsertedLevel);
  return uim_hos_capability;
} /* uim_util_get_hotswap_enabled_slot*/


/**
 * Disable the Hotswap functionality in UIM sw.
 * @return boolean 
 */
boolean uim_hotswap_disabled_in_sw (void)
{
  /* True - Means that Hotswap feature is disabled in SW*/
  /* False - Means that Hotswap feature is enabled in SW*/
  return FALSE;
} /* uim_hotswap_disabled_in_sw */
