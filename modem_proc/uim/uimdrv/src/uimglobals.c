/*===========================================================================

                            I N T E R N A L   U I M
                       G L O B A L   D E F I N I T I O N S

DESCRIPTION
  This contains all the global definitions for UIM

Copyright (c) 2013-2020 QUALCOMM Technologies, Inc (QTI) and its licensors.
All Rights Reserved.
===========================================================================*/

/*===========================================================================

                      EDIT HISTORY FOR FILE

$Header: //components/rel/uim.mpss/6.1.0/uimdrv/src/uimglobals.c#3 $
$DateTime: 2020/02/23 22:25:47 $
$Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
02/20/20   rkg     Add new TLV in Suscription manager
09/17/19   mm      Add EID to SimSlotStatus radio config HAL
04/08/19   ng      Updating slot status check via ATR
08/17/18   rps     Add support for mini dump phase 2
07/06/17   ku      Poll card for twice when detecting flaky removal interupt
06/22/17   ku      Fix for UIM Drivers KW errors
05/15/17   gm      Reset clock on power_down
05/10/17   ku      Switch  card mode if no MF is indicated in ATR
04/14/17   kv      Added pass-through mode support
01/23/17   gm      Handle parity error for ATR during every power up 
09/30/16   gm      Re-set globals during power-up
07/05/16   ks      New global to count PPS attempts
05/17/16   ssr     Remove msg for malloc failure
05/09/16   sam     Queue a open channel command internally after terminal profile
04/11/16   sam     Notify if there is any pending silent recovery callback 
                   while resetiing globals
04/11/16   ks      Changes for LPA support
03/28/16   sam     Support for SAM card
12/10/15   ks      Rel12 polling enhancement
11/23/15   ks      Recovery upon failure of close channel 
                   where ARAM app is selected
11/02/15   sam     Added support to queue Explicit ADF selection
07/07/15   sam     Added low limit check for id in uim_alloc_global_instance
06/08/15   sam     FR25935: power negotitation through terminal capability
04/02/15   ssr     Support to check the pending proactive command on card
02/26/15   ks      Explicit MF selection logic revisited
12/05/14   ks      Clearing path and POLL voting upon card insertion
12/02/14   ks      Feature to support GPIO based hotswap
11/10/14   akv     Support for user to customize LDO settings
10/14/14   av      Handle appropriately zero ICCID reported by uimdrv
09/18/14   akv     UIMDRV UT enhancements
07/25/14   ll      Fix link errors for ASSERT and ERR_FATAL
02/20/14   ks      Provision for explicit MF or ADF selction
                   And provision for inverse convention bootup
01/29/14   sj      ASSERT on Memory Leaks in UIMDRV
10/16/13   akv     Fixed KW errors
09/30/13   akv     Removal of multiple efs reads performed by uim tasks
05/13/13   nmb     Initial Revision
===========================================================================*/
#include "assert.h"                /* ASSERT utilities                     */

#include "uimglobals.h"
#include "uim_v.h"
#include "uimdrv_main.h"

#ifdef FEATURE_QSH_MDUMP
#include "qsh.h"
#endif /* FEATURE_QSH_MDUMP */

uim_instance_global_type *uim_global_instances_ptr[UIM_MAX_INSTANCES];

/* Note - uim_shared_globals to be used only in sections of code guarded
   by critical sections.*/
uim_shared_global_type   uim_shared_globals;
uim_shared_global_type   *uim_shared_globals_ptr = &uim_shared_globals;

 /* Global to store the hotswap type NV(73600) value */
uim_hotswap_enum_type	  uim_hotswap_nature[UIM_MAX_NUMBER_INSTANCES];
/*===========================================================================

FUNCTION UIM_GET_INSTANCE_PTR

DESCRIPTION
  This procedure returns a pointer to the global data structure for a given
  UIM instance.

DEPENDENCIES
  The global data structure must have been previously allocated by the
  uim_task main function through a call to uim_alloc_global_instance

RETURN VALUE
  Pointer to the instance global data

SIDE EFFECTS
  None
===========================================================================*/
uim_instance_global_type *uim_get_instance_ptr
(
  uim_instance_enum_type id
)
{
  if(id >= UIM_INSTANCE_INVALID || id < UIM_INSTANCE_1)
  {
    return NULL;
  }
  return uim_global_instances_ptr[id];
} /* uim_get_instance_ptr */

/*===========================================================================

FUNCTION UIM_INIT_COMMON_GLOBALS

DESCRIPTION
  This procedure initializes the global data structure commom
  across all UIM instances.

DEPENDENCIES
   None.

RETURN VALUE
  None.

SIDE EFFECTS
  None
===========================================================================*/
void uim_init_common_globals(void)
{
  memset(uim_shared_globals_ptr, 0x00, sizeof(uim_shared_global_type));

  /*initialize the slot mapping global*/
  memset(&uim_slot_mapping, 0x00, sizeof(uim_slot_mapping_global_type));
} /* uim_init_common_globals */

/*===========================================================================

FUNCTION UIM_GET_COMMON_GLOBALS_PTR

DESCRIPTION
  This procedure returns a pointer to the global data structure commom
  across all UIM instances.

DEPENDENCIES
   uim_shared_globals to be used only in sections of code guarded
   by critical sections.

RETURN VALUE
  Pointer to the global data common across all uim instances.

SIDE EFFECTS
  None
===========================================================================*/
uim_shared_global_type *uim_get_common_globals_ptr(void)
{
  return uim_shared_globals_ptr;
} /* uim_get_common_globals_ptr */

/*===========================================================================

FUNCTION UIM_SET_INSTANCE_PTR_NULL

DESCRIPTION
  This sets the global data structure for a given  UIM instance to NULL

DEPENDENCIES
  The global data structure must have been previously allocated by the
  uim_task main function through a call to uim_alloc_global_instance

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void uim_set_instance_ptr_null
(
  uim_instance_enum_type id
)
{
   if(id >= UIM_INSTANCE_INVALID || id < UIM_INSTANCE_1)
  {
    return;
  }
  uim_global_instances_ptr[id] = NULL;
} /* uim_set_instance_ptr_null */


/*===========================================================================

FUNCTION UIM_ALLOC_GLOBAL_INSTANCE

DESCRIPTION
  This procedure allocates the memory of the global data structure for
  a given UIM instance.

DEPENDENCIES
  This function should be called only once for each task very early in the
  task main function

RETURN VALUE
  None.

SIDE EFFECTS
  Internally sets global uim_instances_ptr to the newly allocated memory
===========================================================================*/
void uim_alloc_global_instance
(
  uim_instance_enum_type id
)
{
  ASSERT(id < UIM_MAX_INSTANCES);
  ASSERT(id >= 0);

  uim_global_instances_ptr[id] = uim_malloc(sizeof(uim_instance_global_type));
  if (NULL != uim_global_instances_ptr[id])
  {
    memset(uim_global_instances_ptr[id], 0x00, sizeof(uim_instance_global_type));
    uim_global_instances_ptr[id]->id = id;
  }
} /* uim_alloc_global_instance */


/*===========================================================================
FUNCTION UIM_RESET_POWER_UP_GLOBALS

DESCRIPTION
  This function resets necessary UIM globals during power up.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void uim_reset_power_up_globals
(
  uim_instance_global_type *uim_ptr
)
{
  if(uim_ptr == NULL)
  {
    return;
  }

  /* Flag to indicate if the driver has encountered an overrun error */
  uim_ptr->flag.overrun_error                = FALSE;
  /* Flag to indicate that un-known status words have been received */
  uim_ptr->flag.bad_status_words_error       = FALSE;
  /* Flag to indicate if the driver has encountered too many overrun errors */
  uim_ptr->flag.max_overrun_error            = FALSE;
  /* Flag to indicate if the driver has encountered too many rx break errors */
  uim_ptr->flag.max_rx_break_error           = FALSE;
  /* Flag to indicate if the driver has maxed the parity error count */
  uim_ptr->flag.max_parity_error             = FALSE;

  uim_ptr->flag.interface_gone_bad           = FALSE;
  uim_ptr->flag.fetch_pending                = FALSE;

  /* Reset the counters */
  uim_ptr->debug.convention_change_count     = 1;

  /* Reset the poll related flags */
  uim_ptr->flag.poll_pending                 = FALSE;
  uim_ptr->poll.is_proactive_polling_enabled = TRUE;
  uim_ptr->poll.presence_detection_state     = UIM_PD_POLLING_OFF;
  (void) uim_clear_poll_timer(uim_ptr);

  /* Initialize the warm reset count to zero for each powerup */
  uim_ptr->debug.warm_reset_count            = 0;
  uim_ptr->command.rsp_buf.sw1               = SW1_NORMAL_END;
  uim_ptr->command.rsp_buf.sw2               = SW2_NORMAL_END;
  uim_ptr->flag.uim_auth_cdma_df_first       = FEATURE_UIM_AUTH_CDMA_DF_FIRST_DEFAULT;
  uim_ptr->atr.is_parity_in_atr              = FALSE;

  /* The path elements are used by the UIM_CHECK_CHARACTERISTICS_ST
     clear the path here so that it's ready when we enter this state. */
  if(uim_ptr->command.cmd_ptr != NULL)
  { 
    memset(&uim_ptr->command.cmd_ptr->select.path, 0x00, sizeof(uim_path_type));
  }
  memset(&uim_ptr->command.rsp_buf, 0x00, sizeof(uim_ptr->command.rsp_buf));
} /* uim_reset_power_up_globals */


/*===========================================================================
FUNCTION UIM_RESET_NOTIFY_ERROR_GLOBALS

DESCRIPTION
  This function resets necessary UIM globals during notify error.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void uim_reset_notify_error_globals
(
  uim_instance_global_type *uim_ptr
)
{
  if(uim_ptr == NULL)
  {
    return;
  }

  /* Reset the ATR cache */
  memset(&uim_ptr->atr.atr_buf, 0x00, sizeof(uim_atr_buf_type));

  /* Reset the iccid cache in case the next card does not have an ICCID */  
  memset(&uim_ptr->cache.iccid_buf, 0x00, sizeof(uim_iccid_buf_type));  
 /* Reset the number of apps cache */  
  memset(&uim_ptr->cache.num_apps, 0x00, sizeof(uim_ptr->cache.num_apps)); 

  /* Reset the card protocol */  
  uim_ptr->card_cmd.uimdrv_protocol = UIM_MAXIMUM_MODE;

  /* Reset the eUICC flag */
  uim_ptr->flag.is_euicc            = FALSE;  
}


/*===========================================================================
FUNCTION UIM_RESET_GLOBALS

DESCRIPTION
  This function resets necessary UIM globals if UIM encounters an
  unexpected error during power up or an internal wake up and chooses to
  notify its clients by calling uim_notify_error.
  It is also called in the event of a card removal if hotswap is enabled.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void uim_reset_globals
(
  uim_instance_global_type *uim_ptr
)
{
  byte num_ch = 0;

  if(uim_ptr == NULL)
  {
    return;
  }

  /*return if instance is invalid*/
  RETURN_IF_INSTANCE_INVALID(uim_ptr->id);

  /* Flag to indicate if the driver has encountered an overrun error */
  uim_ptr->flag.overrun_error                              = FALSE;
  /* Flag to indicate that un-known status words have been received */
  uim_ptr->flag.bad_status_words_error                     = FALSE;
  /* Flag to indicate if the driver has encountered too many overrun errors */
  uim_ptr->flag.max_overrun_error                          = FALSE;
  /* Flag to indicate if the driver has encountered too many rx break errors */
  uim_ptr->flag.max_rx_break_error                         = FALSE;
  /* Flag to indicate if the driver has maxed the parity error count */
  uim_ptr->flag.max_parity_error                           = FALSE;

  uim_ptr->flag.recovery_triggered                         = FALSE;
  uim_ptr->flag.interface_gone_bad                         = FALSE;
  uim_ptr->flag.fetch_pending                              = FALSE;

  /* Reset the counters */
  uim_ptr->debug.convention_change_count                   = 1;

  /* The power up mode will be set after parsing the historical characters
     from the ATR */
  /* Set the count of technical problems to zero */
  uim_ptr->card_cmd.inc_tech_prob_count                    = 0;

  uim_ptr->state.mf_selection.required                     = 0;
  uim_ptr->state.mf_selection.channel                      = 0;

  uim_ptr->state.adf_selection.required                    = 0;
  uim_ptr->state.adf_selection.channel                     = 0;

  uim_ptr->state.aram_app.select_aram_failed               = FALSE;
  uim_ptr->state.aram_app.channel                          = 0xFF;

  uim_ptr->state.last_selected.required                    = FALSE;
  uim_ptr->state.last_selected.channel                     = 0;

  uim_ptr->card_cmd.instrn_toggle_count                    = 1;
  uim_ptr->flag.toggle_instrn_class                        = FALSE;
  uim_ptr->atr.is_parity_in_atr                            = FALSE;

/* reset the voltage_class_known_from_atr varialbe */
  uim_ptr->flag.voltage_class_known_from_atr               = FALSE;

  if (uim_ptr->hardware.uim_ldo.customize_ldo_setting &&
      uim_ptr->hardware.uim_ldo.desired_voltage_class != UIM_VOLTAGE_CLASS_INVALID)
  {
    /* Set the UIM interface voltage to desired voltage class */
    uim_ptr->state.current_voltage_class                   = uim_ptr->hardware.uim_ldo.desired_voltage_class;
  }
  else
  {
    /* Set the UIM interface voltage to the minimum voltage class */
    uim_ptr->state.current_voltage_class                   = UIM_VOLTAGE_CLASS_C;
  }

  /* Reset frequency */
  if (uim_hw_if.clkreg[uim_ptr->id].m_simMaxFreqSupported == UIMDRV_CLK_FREQ_4_8_MHZ)
  {
    UIM_RESET_CLK_FREQ_TO_DEFAULT_4_8_MHZ(uim_ptr->id);
    SET_UIM_BAUD_RATE_SLOT ( uim_ptr->id, UIM_CRCF_372_1, UIM_BRAF_1 );
  }

  uim_ptr->state.WI                                        = 10;

  /* Indicate that we did not receive the ATR */
  uim_ptr->atr.atr_received                                = FALSE;
  uim_ptr->command.mode                                    = UIM_NORMAL_MODE;
  /* Reset the flag */
  uim_ptr->flag.poll_pending                              = FALSE;
  (void) uim_clear_poll_timer(uim_ptr);
  uim_ptr->state.dfs_1.cdma_df_present                     = FALSE;
  uim_ptr->state.dfs_1.gsm_df_present                      = FALSE;
  uim_ptr->state.dfs_1.protocol                            = UIM_NO_SUCH_MODE;

  /* Clear pending UIM Command and response signal */
  (void) rex_clr_sigs( uim_ptr->tcb_ptr, UIM_CMD_RSP_SIG );
  (void) rex_clr_sigs( uim_ptr->tcb_ptr, UIM_CMD_RSP_TIMEOUT_SIG );
  /* clear the signal as well just in case if it was set */
  (void) rex_clr_sigs( uim_ptr->tcb_ptr, UIM_TRANSACTION_SIG );
  /* Re-setting to NULL so that in erroneous condition call back is not called */

  /*Notify if there is any pending silent recovery callback*/  
  if(NULL != uim_ptr->silent_recover.uim_silent_recovery_callback)
  {
    uim_ptr->silent_recover.uim_silent_recovery_callback((uim_slot_type)(uim_ptr->id	+1),
						   uim_ptr->silent_recover.user_data,UIMDRV_RECOVERY_STAGE_END);
  }
  
  uim_ptr->silent_recover.user_data                        = NULL;
  uim_ptr->silent_recover.uim_silent_recovery_callback     = NULL;
  uim_ptr->flag.uim_is_silent_recovery_in_progress         = FALSE;

  /* Reset the iccid cache in case the next card does not have an ICCID */
  memset(&uim_ptr->cache.iccid_buf, 0x00, sizeof(uim_iccid_buf_type));

  /* Reset the ATR cache */
  memset(&uim_ptr->atr.atr_buf, 0x00, sizeof(uim_atr_buf_type));

  /* Reset the number of apps cache */  
  memset(&uim_ptr->cache.num_apps, 0x00, sizeof(uim_ptr->cache.num_apps)); 

  /* Reset the card protocol */  
  uim_ptr->card_cmd.uimdrv_protocol = UIM_MAXIMUM_MODE;  

  /*Reset the EF-UMPC related data*/
  uim_ptr->flag.is_rel_12_uicc = FALSE;
  memset(&uim_ptr->cache.ef_umpc_data, 0x00, sizeof (uim_ef_umpc_data_type));
  
  uim_ptr->state.ctl =
   (uim_voter_type) ((int)UIM_NONE
                   | (int)UIM_MC
                   | (int)UIM_AUTH
                   | (int)UIM_UICC_SESSION
                     );
  uim_ptr->poll.is_proactive_polling_enabled               = TRUE;
  uim_ptr->poll.presence_detection_state                   = UIM_PD_POLLING_OFF;
  uim_ptr->flag.is_euicc                                   = FALSE;
  uim_ptr->poll.flaky_polling_state                        = UIM_FLAKY_POLLING_OFF;
  uim_ptr->poll.orig_poll_time                             = 0;
  
  /* Reset Channel Open information
   * Default channel is always open - not changing its status */
  for(num_ch = 1; num_ch < UIM_MAX_CHANNELS; num_ch++)
  {
    uim_ptr->state.is_ch_open[num_ch] = FALSE;
    uim_ptr->state.ch_apdu_behavior[num_ch] = UIM_APDU_BEHAVIOR_FOR_TELECOM_APP;	  
  }

  uim_reset_dir(uim_ptr);

  uim_ptr->state.channel_info.manage_channel_allowed       = TRUE;
  uim_ptr->state.channel_info.unallocated_channel          = 0xFF;
  uim_ptr->atr.no_pps_attempts = 0;

  /* Reset the card mode based on the current NV settings */
  uim_sam_set_card_mode(NULL,uim_ptr);
  
  /* Clear cached EID */
  memset(uim_ptr->cache.eid_buf.data, 0x00, sizeof(uim_ptr->cache.eid_buf.data));
  
  /* Clear the flags associated with SELECT ISDR ST in power up state */
  uim_ptr->flag.is_select_isdr         = FALSE;
  uim_ptr->select_isdr_ch_num.channel  = 0;
} /* uim_reset_globals */


#ifdef FEATURE_QSH_MDUMP
/*===========================================================================
FUNCTION UIM_QSH_MDUMP_DRIVER_SAVE_UIM_GLOBAL_INSTANCE

DESCRIPTION
  This function is used to dump uim_global_instance_ptr.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
QSH_MDUMP_FN_ATTR void uim_qsh_mdump_driver_save_uim_global_instance(void)
{
  uint8 index = 0;

  qsh_mdump_collect_high(uim_global_instances_ptr, sizeof(uim_global_instances_ptr));
  for(index = 0; index < UIM_MAX_INSTANCES; index++)
  {
    if(uim_global_instances_ptr[index] != NULL)
    {
      qsh_mdump_collect_high(&uim_global_instances_ptr[index]->flag.command_in_progress, sizeof(uim_global_instances_ptr[index]->flag.command_in_progress));
      qsh_mdump_collect_high(&uim_global_instances_ptr[index]->flag.max_rx_break_error, sizeof(uim_global_instances_ptr[index]->flag.max_rx_break_error));
      qsh_mdump_collect_high(&uim_global_instances_ptr[index]->state.voltage_class, sizeof(uim_global_instances_ptr[index]->state.voltage_class));
      qsh_mdump_collect_high(&uim_global_instances_ptr[index]->state.operating_protocol, sizeof(uim_global_instances_ptr[index]->state.operating_protocol));
    }
  }
}/* uim_qsh_mdump_driver_save_uim_global_instance */
#endif /* FEATURE_QSH_MDUMP */
