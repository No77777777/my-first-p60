/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*


       U I M   BTSAP in Client mode IMPLEMENTATION

GENERAL DESCRIPTION
  This module contains the procedures for the BTSAP in client mode implementation.
  The module receives the event from the QMI REMOTE CLIENT. The module handles the 
  power up and power down sequence of the remote sim card.  

Copyright (c) 2013-2017, 2019 QUALCOMM Technologies, Inc (QTI) and its licensors. All Rights Reserved.

*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/* <EJECT> */
/*===========================================================================

                        EDIT HISTORY FOR MODULE

$Header: //components/rel/uim.mpss/6.1.0/uimdrv/src/uim_btsap.c#3 $
$DateTime: 2020/07/30 04:25:47 $
$Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
07/23/20   rkg     Setting poll time to 28 sec for BTSAP connection available
08/23/17   ku      Improved handling of QMI_UIM_REMOTE_EVT_CARD_REMOVED event in BTSAP
05/10/17   ku      Handle unexpected command for remote card
03/13/17   ku      Common path for local and remote card
10/17/16   ks      Proper fall back to last succesful UIM interface
08/04/16   gm      Adding Support to increase poll interval for Remote SIM
06/23/16   sam     F3 messages optimization
04/20/16   sam     Support for enhanced subscription manager
03/28/16   sam     Support for SAM card
12/16/15   ssr     Delay local card power up in Modem SSR in BTSAP mode
09/02/15   hyo     Drivers and state machine abstraction
08/04/15   ks      Adding the static declarations for internal functions
07/03/15   ks      Adding report call bcak for remote/local cards power-up
06/16/15   ks      Clearing the uim_hotswap_notify_card_removed flag
                   while switching between remote card to local card
06/08/15   ks      Handling BTSAP connection unavailable signal
04/15/15   ks      De-Featurizing the FEATURE_UIM_REMOTE/BTSAP_UIM
08/04/15   sam     FR24498: Powerup logging new changes
01/07/14   sam     Modification of msg macros for FR24498-UIM Powerup Logging
03/21/14   ll      Remove return_ptr and remove/change uim_free to UIM_FREE
01/29/14   sam     Usage of newer style diag macros
01/31/14   ak      Safety checks for disabled UIM instances
12/12/13   ks      BTSAP in client mode, intial implementation
===========================================================================*/

#include "uim_remote.h"
#include "uim_hotswap.h"
#include "uim_common_efs.h" 

/*===========================================================================
FUNCTION UIM_BTSAP_REPORT_CB

DESCRIPTION:              
 This function is the report call back for the remote/local cards POWERUP.
 This function is needed because UIM drivers frees cmd memory if and only if
 there is a report call back associated with the command.

LIMITATIONS:               
 None

RETURN VALUE:
NONE

SIDE EFFECTS:
 None.
===========================================================================*/  
static void uim_btsap_report_cb(uim_rpt_type *report_ptr)
{
  (void) report_ptr;
  return;
}/* uim_btsap_report_cb */

/*===========================================================================
FUNCTION UIM_BTSAP_TERMINATE_CONNECTION

DESCRIPTION
  This procedure frees the BTSAP connection.
  After this API is called, CONNECTION_UNAVAILABLE event is expected

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void uim_btsap_terminate_connection(uim_instance_global_type *uim_ptr)
{
  UIMDRV_MSG_HIGH_1(uim_ptr->id,"uim release with QMI REMOTE 0x%x", 
  qmi_uim_remote_card_disconnect((qmi_uim_remote_slot_type)uim_ptr->id));
}/* uim_btsap_terminate_connection */


/*===========================================================================
FUNCTION UIM_BTSAP_SAVE_CURRENT_MODE_TO_EFS

DESCRIPTION
  This procedure write the connection mode (ISO\Remote) and Delay time for
  remote connection in EFs file.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void uim_btsap_save_current_mode_to_efs (uim_instance_global_type *uim_ptr ,  
                                          uim_slot_connection_enum_type uim_mode)
{
  uim_last_mode_and_config_delay_for_remote_connect_type current_cntx;
  current_cntx.connection_mode  = uim_mode;
  current_cntx.delay_time_insec = uim_ptr->state.local_card_pup_delay;
  if (UIM_COMMON_EFS_SUCCESS == uim_common_efs_write(UIM_COMMON_EFS_LAST_MODE_AND_CONFIG_DELAY_FOR_REMOTE_CONNECT,
                                                     UIM_COMMON_EFS_ITEM_FILE_TYPE,
                                                     (uim_common_efs_context_type)uim_ptr->id,
                                                     (const char*)&(current_cntx),
                                                     sizeof(current_cntx)))
  {
    UIMDRV_MSG_HIGH_2(uim_ptr->id, "Written mode 0x%x and wait time %d to UIM_COMMON_EFS_LAST_MODE_AND_CONFIG_DELAY_FOR_REMOTE_CONNECT",
                                    current_cntx.connection_mode, current_cntx.delay_time_insec);
  }
  else
  {
    UIMDRV_MSG_HIGH_2(uim_ptr->id, "Failed to write mode 0x%x and wait time %d to UIM_COMMON_EFS_LAST_MODE_AND_CONFIG_DELAY_FOR_REMOTE_CONNECT",
	                            current_cntx.connection_mode, current_cntx.delay_time_insec);
  }
}/* uim_btsap_save_current_mode_to_efs*/


/*===========================================================================
FUNCTION UIM_BTSAP_ESTABLISH_CONNECTION_REQ

DESCRIPTION
  This procedure sets the handle and state of BTSAP appropriately before establishing connection.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void uim_btsap_establish_connection_req(uim_instance_global_type *uim_ptr)
{
  uim_global_remote_type  *remote_data_ptr = NULL;
  if(uim_ptr == NULL)
  {
    return;
  }
  remote_data_ptr                   = &(uim_ptr->remote);
  remote_data_ptr->btsap.handle     = (remote_data_ptr->btsap.handle | UIM_BTSAP_ACTIVE_ENABLE_MASK) ;
  remote_data_ptr->state            = WAIT_FOR_NOTHING;
  uim_ptr->setting.stored_phy_layer = uim_ptr->setting.phy_layer;
  uim_ptr->setting.phy_layer        = UIM_REMOTE;
  remote_data_ptr->extended_data.no_need_send_get_response_to_qmi = FALSE;
  uim_btsap_save_current_mode_to_efs (uim_ptr, UIM_CONNECTION_BTSAP);
} /* uim_btsap_establish_connection_req */

/*===========================================================================
FUNCTION UIM_BTSAP_RELEASE_CONNECTION_REQ

DESCRIPTION
  This procedure sets the handle and state of BTSAP appropriately before releasing connection.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void uim_btsap_release_connection_req(uim_instance_global_type *uim_ptr)
{
  uim_global_remote_type  *remote_data_ptr = NULL;

  remote_data_ptr               = &(uim_ptr->remote);
  remote_data_ptr->state        = WAIT_FOR_CONNECTION_AVAILABLE;
  remote_data_ptr->btsap.handle = 0x00;
  uim_ptr->setting.phy_layer    = uim_ptr->setting.stored_phy_layer;

  /* This is needed to reset all the globals and UART before restarting the local sim */
  uim_ptr->hotswap.hotswap_notify_card_removed = FALSE;
  uim_hotswap_reset_slot(uim_ptr);
  uim_btsap_save_current_mode_to_efs (uim_ptr, UIM_CONNECTION_ISO);
}/* uim_btsap_release_connection_req */

/*===========================================================================
FUNCTION UIM_BTSAP_POWER_DOWN_LOCAL_SIM_CB

DESCRIPTION
  This function is the callback func for the POWER_DOWN_F cmd
  send to local sim 
  This procedure sends the RESET_F command to remote sim.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
static void uim_btsap_power_down_local_sim_cb( uim_rpt_type *report_ptr)
{
  uim_cmd_type*                   uim_cmd_ptr    = NULL;
  uim_instance_global_type       *uim_ptr        = NULL;

  if(report_ptr == NULL)
  {
    return;
  }

  uim_cmd_ptr = (uim_cmd_type*)(report_ptr->user_data);
  
  if(uim_cmd_ptr == NULL)
  {
    return;
  }
  
  uim_ptr = uim_get_instance_ptr(uim_cmd_ptr->hdr.slot - 1);

  if(uim_ptr == NULL)
  {
    return;
  }
  uim_cmd_ptr = (uim_cmd_type*)uim_malloc(sizeof(uim_cmd_type));
  
  if(uim_cmd_ptr == NULL)
  {
    return;
  } 

  /*Send RESET_F cmd to power-up the remote sim */
  uim_cmd_ptr->hdr.command            = UIM_RESET_F;
  uim_cmd_ptr->hdr.slot               = (uim_ptr->id + 1);
  uim_cmd_ptr->hdr.channel            = UIM_DEFAULT_CHANNEL;
  uim_cmd_ptr->hdr.cmd_hdr.task_ptr   = NULL;
  uim_cmd_ptr->hdr.cmd_hdr.sigs       = 0;
  uim_cmd_ptr->hdr.cmd_hdr.done_q_ptr = NULL;
  uim_cmd_ptr->hdr.options            = UIM_OPTION_ALWAYS_RPT;
  uim_cmd_ptr->hdr.rpt_function       = uim_btsap_report_cb;
  uim_cmd_ptr->hdr.user_data          = (uint32)uim_cmd_ptr;
  uim_cmd_ptr->hdr.cmd_count          = 0;

  uim_btsap_establish_connection_req(uim_ptr);

  UIMDRV_MSG_HIGH_0(uim_ptr->id,"Send Card UIM_RESET_F command for remote sim");
  /* Posting command on uim queue */
  /* init link */
  ( void )q_link( uim_cmd_ptr, &uim_cmd_ptr->hdr.cmd_hdr.link );
  /* and queue it */
  q_put( &uim_ptr->task.cmd_q, &uim_cmd_ptr->hdr.cmd_hdr.link );
  /* and signal the UIM task */
  (void)rex_set_sigs( uim_ptr->tcb_ptr, UIM_CMD_Q_SIG );
}/* uim_btsap_power_down_local_sim_cb */

/*===========================================================================
FUNCTION UIM_BTSAP_SEND_POWER_DOWN_LOCAL_SIM

DESCRIPTION
  This procedure sends the power_down_f command to local sim.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
static void uim_btsap_send_power_down_local_sim(uim_instance_global_type *uim_ptr)
{
  uim_cmd_type*                      uim_cmd_ptr   = NULL;

  if(uim_ptr == NULL)
  {
    return;
  }
  uim_cmd_ptr = (uim_cmd_type*)uim_malloc(sizeof(uim_cmd_type));
  
  if(uim_cmd_ptr == NULL)
  {
    return;
  } 

  uim_cmd_ptr->hdr.command            = UIM_POWER_DOWN_F;
  uim_cmd_ptr->hdr.slot	              = (uim_ptr->id + 1);
  uim_cmd_ptr->hdr.channel            = UIM_DEFAULT_CHANNEL;
  uim_cmd_ptr->hdr.cmd_hdr.task_ptr   = NULL;
  uim_cmd_ptr->hdr.cmd_hdr.sigs       = 0;
  uim_cmd_ptr->hdr.cmd_hdr.done_q_ptr = NULL;
  uim_cmd_ptr->hdr.options            = UIM_OPTION_ALWAYS_RPT;
  uim_cmd_ptr->hdr.rpt_function       = uim_btsap_power_down_local_sim_cb;
  uim_cmd_ptr->hdr.user_data          = (uint32)uim_cmd_ptr;
  uim_cmd_ptr->hdr.cmd_count          = 0;
  uim_cmd_ptr->pdown.pdown_option     = UIM_OPTION_NOTIFY_PDOWN;
	
  UIMDRV_MSG_HIGH_0(uim_ptr->id,"Send Card UIM_POWER_DOWN_F command for local sim");
  /* Posting command on uim queue */
  /* init link */
  ( void )q_link( uim_cmd_ptr, &uim_cmd_ptr->hdr.cmd_hdr.link );
  /* and queue it */
  q_put( &uim_ptr->task.cmd_q, &uim_cmd_ptr->hdr.cmd_hdr.link );
  /* and signal the UIM task */
  (void)rex_set_sigs( uim_ptr->tcb_ptr, UIM_CMD_Q_SIG );
}/* uim_btsap_send_power_down_local_sim */


/*===========================================================================
FUNCTION UIM_BTSAP_POWER_DOWN_REMOTE_SIM_CB

DESCRIPTION
  This function is the callback func for the POWER_DOWN_F cmd
  send to remote sim 
  This procedure sends the RESET_F command to local sim.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
static void uim_btsap_power_down_remote_sim_cb(uim_rpt_type *report_ptr)
{
  uim_cmd_type*                   uim_cmd_ptr = NULL;
  uim_instance_global_type       *uim_ptr     = NULL;
  
  if(report_ptr == NULL)
  {
    return;
  }

  uim_cmd_ptr = (uim_cmd_type*)(report_ptr->user_data);

  if(uim_cmd_ptr == NULL)
  {
    return;
  }
  
  uim_ptr = uim_get_instance_ptr(uim_cmd_ptr->hdr.slot - 1);

  if(uim_ptr == NULL)
  {
    return;
  }
  uim_cmd_ptr = (uim_cmd_type*)uim_malloc(sizeof(uim_cmd_type));
  
  if(uim_cmd_ptr == NULL)
  {
    return;
  } 

  uim_btsap_release_connection_req(uim_ptr);
  /*Send RESET_F cmd to power-up the local sim */
  uim_cmd_ptr->hdr.command            = UIM_RESET_F;
  uim_cmd_ptr->hdr.slot	              = (uim_ptr->id + 1);
  uim_cmd_ptr->hdr.channel            = UIM_DEFAULT_CHANNEL;
  uim_cmd_ptr->hdr.cmd_hdr.task_ptr   = NULL;
  uim_cmd_ptr->hdr.cmd_hdr.sigs       = 0;
  uim_cmd_ptr->hdr.cmd_hdr.done_q_ptr = NULL;
  uim_cmd_ptr->hdr.options            = UIM_OPTION_ALWAYS_RPT;
  uim_cmd_ptr->hdr.rpt_function       = uim_btsap_report_cb;
  uim_cmd_ptr->hdr.user_data          = (uint32)uim_cmd_ptr;
  uim_cmd_ptr->hdr.cmd_count          = 0;
  UIMDRV_MSG_HIGH_0(uim_ptr->id,"Send Card UIM_RESET_F command for local sim ");
  /* Posting command on uim queue */
  /* init link */
  ( void )q_link( uim_cmd_ptr, &uim_cmd_ptr->hdr.cmd_hdr.link );
   /* and queue it */
  q_put( &uim_ptr->task.cmd_q, &uim_cmd_ptr->hdr.cmd_hdr.link );
  /* and signal the UIM task */
  (void)rex_set_sigs( uim_ptr->tcb_ptr, UIM_CMD_Q_SIG );
}/* uim_btsap_power_down_remote_sim_cb */

/*===========================================================================
FUNCTION UIM_BTSAP_SEND_POWER_DOWN_REMOTE_SIM

DESCRIPTION
  This procedure sends the POWER_DOWN_F command to remote sim.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
static void uim_btsap_send_power_down_remote_sim(uim_instance_global_type *uim_ptr)
{
  uim_cmd_type*  uim_cmd_ptr = NULL;

  if(uim_ptr == NULL)
  {
    return;
  }
 
  uim_cmd_ptr = (uim_cmd_type*)uim_malloc(sizeof(uim_cmd_type));
 
  if(uim_cmd_ptr == NULL)
  {
    return;
  }
 
  uim_ptr->remote.btsap.handle        = ( uim_ptr->remote.btsap.handle | UIM_BTSAP_CARD_CONN_UNAVAIL_ENABLE_MASK );
  uim_cmd_ptr->hdr.command            = UIM_POWER_DOWN_F;
  uim_cmd_ptr->hdr.slot               = (uim_ptr->id + 1) ;
  uim_cmd_ptr->hdr.channel            = UIM_DEFAULT_CHANNEL;
  uim_cmd_ptr->hdr.cmd_hdr.task_ptr   = NULL;
  uim_cmd_ptr->hdr.cmd_hdr.sigs       = 0;
  uim_cmd_ptr->hdr.cmd_hdr.done_q_ptr = NULL;
  uim_cmd_ptr->hdr.options            = UIM_OPTION_ALWAYS_RPT;
  uim_cmd_ptr->hdr.rpt_function       = uim_btsap_power_down_remote_sim_cb;
  uim_cmd_ptr->hdr.user_data          = (uint32)uim_cmd_ptr;
  uim_cmd_ptr->hdr.cmd_count          = 0;
  uim_cmd_ptr->pdown.pdown_option     = UIM_OPTION_NOTIFY_PDOWN;
	
  UIMDRV_MSG_HIGH_0(uim_ptr->id,"Send Card UIM_POWER_DOWN_F command for remote sim ");
   /* Posting command on uim queue */
  /* init link */
  ( void )q_link( uim_cmd_ptr, &uim_cmd_ptr->hdr.cmd_hdr.link );
  /* and queue it */
  q_put( &uim_ptr->task.cmd_q, &uim_cmd_ptr->hdr.cmd_hdr.link );
  /* and signal the UIM task */
  (void)rex_set_sigs( uim_ptr->tcb_ptr, UIM_CMD_Q_SIG );
  (void)rex_set_sigs( uim_ptr->tcb_ptr, UIM_REMOTE_CONNECTION_UNAVAILABLE_SIG );
}/* uim_btsap_send_power_down_remote_sim */


/*===========================================================================
FUNCTION UIM_BTSAP_RECONNECT_REMOTE_SIM_CB

DESCRIPTION
  This function is the callback func for the POWER_DOWN_F cmd
  send to remote sim 
  This procedure sends the RESET_F command to remote sim.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
static void uim_btsap_reconnect_remote_sim_cb(uim_rpt_type *report_ptr)
{
  uim_cmd_type*              uim_cmd_ptr  = NULL;
  uim_instance_global_type  *uim_ptr      = NULL;

  if(report_ptr == NULL)
  {
    return;
  }
  
  uim_cmd_ptr = (uim_cmd_type*)(report_ptr->user_data);

  if(uim_cmd_ptr == NULL)
  {
    return;
  }
  
  uim_ptr = uim_get_instance_ptr(uim_cmd_ptr->hdr.slot - 1);
 
  if(uim_ptr == NULL)
  {
    return;
  }
  
  uim_cmd_ptr = (uim_cmd_type*)uim_malloc(sizeof(uim_cmd_type));
  
  if(uim_cmd_ptr == NULL)
  {
    return;
  } 

  /*Send RESET_F cmd to power-up the remote sim */
  
  uim_ptr->remote.btsap.handle        = (UIM_BTSAP_ACTIVE_ENABLE_MASK) ;
  uim_cmd_ptr->hdr.command            = UIM_RESET_F;
  uim_cmd_ptr->hdr.slot               = (uim_ptr->id + 1);
  uim_cmd_ptr->hdr.channel            = UIM_DEFAULT_CHANNEL;
  uim_cmd_ptr->hdr.cmd_hdr.task_ptr   = NULL;
  uim_cmd_ptr->hdr.cmd_hdr.sigs       = 0;
  uim_cmd_ptr->hdr.cmd_hdr.done_q_ptr = NULL;
  uim_cmd_ptr->hdr.options            = UIM_OPTION_ALWAYS_RPT;
  uim_cmd_ptr->hdr.rpt_function       = uim_btsap_report_cb;
  uim_cmd_ptr->hdr.user_data          = (uint32)uim_cmd_ptr;
  uim_cmd_ptr->hdr.cmd_count          = 0;
  uim_ptr->remote.state               = WAIT_FOR_NOTHING;
  
  UIMDRV_MSG_HIGH_0(uim_ptr->id,"Send Card UIM_RESET_F command for reconnecting remote sim");
  /* Posting command on uim queue */
  /* init link */
  ( void )q_link( uim_cmd_ptr, &uim_cmd_ptr->hdr.cmd_hdr.link );
  /* and queue it */
  q_put( &uim_ptr->task.cmd_q, &uim_cmd_ptr->hdr.cmd_hdr.link );
  /* and signal the UIM task */
  (void)rex_set_sigs( uim_ptr->tcb_ptr, UIM_CMD_Q_SIG );

}/* uim_btsap_reconnect_remote_sim_cb */

/*===========================================================================
FUNCTION UIM_BTSAP_SEND_RECONNECT_REMOTE_SIM

DESCRIPTION
  This procedure sends the power down command command to remote sim.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
static void uim_btsap_send_reconnect_remote_sim(uim_instance_global_type *uim_ptr)
{
  uim_cmd_type*  uim_cmd_ptr = NULL;

  if(uim_ptr == NULL)
  {
    return;
  }

  uim_cmd_ptr = (uim_cmd_type*)uim_malloc(sizeof(uim_cmd_type));

  if(uim_cmd_ptr == NULL)
  {
    return;
  }

  uim_ptr->remote.btsap.handle        = ( uim_ptr->remote.btsap.handle | UIM_BTSAP_CARD_CONN_UNAVAIL_ENABLE_MASK );
  uim_cmd_ptr->hdr.command            = UIM_POWER_DOWN_F;
  uim_cmd_ptr->hdr.slot               = (uim_ptr->id + 1) ;
  uim_cmd_ptr->hdr.channel            = UIM_DEFAULT_CHANNEL;
  uim_cmd_ptr->hdr.cmd_hdr.task_ptr   = NULL;
  uim_cmd_ptr->hdr.cmd_hdr.sigs       = 0;
  uim_cmd_ptr->hdr.cmd_hdr.done_q_ptr = NULL;
  uim_cmd_ptr->hdr.options            = UIM_OPTION_ALWAYS_RPT;
  uim_cmd_ptr->hdr.rpt_function       = uim_btsap_reconnect_remote_sim_cb;
  uim_cmd_ptr->hdr.user_data          = (uint32)uim_cmd_ptr;
  uim_cmd_ptr->hdr.cmd_count          = 0;
  uim_cmd_ptr->pdown.pdown_option     = UIM_OPTION_NOTIFY_PDOWN;

  if(WAIT_FOR_ATR == uim_ptr->remote.state)
  {
    (void)rex_clr_timer(&uim_ptr->remote.remote_cmd_rsp_timer);
    /* Set the command response timeout signal */
    (void) rex_set_sigs( uim_ptr->tcb_ptr, UIM_CMD_RSP_TIMEOUT_SIG );
  }
  
  UIMDRV_MSG_HIGH_0(uim_ptr->id,"Send Card UIM_POWER_DOWN_F command for reconnect remote sim ");
  /* Posting command on uim queue */
  /* init link */
  ( void )q_link( uim_cmd_ptr, &uim_cmd_ptr->hdr.cmd_hdr.link );
  /* and queue it */
  q_put( &uim_ptr->task.cmd_q, &uim_cmd_ptr->hdr.cmd_hdr.link );
  /* and signal the UIM task */
  (void)rex_set_sigs( uim_ptr->tcb_ptr, UIM_CMD_Q_SIG );
  (void)rex_set_sigs( uim_ptr->tcb_ptr, UIM_REMOTE_CONNECTION_UNAVAILABLE_SIG );
} /* uim_btsap_send_reconnect_remote_sim */

/*===========================================================================

FUNCTION UIM_BTSAP_EVENT_CALLBACK

DESCRIPTION
  This procedure is the callback that is called for every QMI REMOTE event.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
static void uim_btsap_event_callback(const qmi_uim_remote_event_data_type *evt_ptr)
{
  uim_instance_enum_type    remote_slot      = 0;
  uim_global_remote_type     *remote_data_ptr  = NULL; 
  uim_instance_global_type  *uim_ptr         = NULL;
  
  if(NULL == evt_ptr)
  {
    return;
  }

  remote_slot = evt_ptr->slot;

  /*convert logical to physical slot ID*/
  remote_slot = (uim_instance_enum_type)(uim_convert_logical_to_physical_slot_id((uim_slot_type)(remote_slot + 1)) - 1);

  RETURN_IF_INSTANCE_INVALID(remote_slot);
  uim_ptr = uim_get_instance_ptr(remote_slot);
  if(uim_ptr == NULL)
  {
    return;
  }

  /*Ignore any BTSAP events if current task is in SAM mode*/
  if(UIM_CARD_SAM  == uim_ptr->card_mode || UIM_SLOT_STATE_INACTIVE == uim_ptr->activity_status)
  {
    UIM_MSG_HIGH_2("Ignore BTSAP event- task is in sam mode or inactive. mode-0x%x, activity_status-0x%x",
                    uim_ptr->card_mode, uim_ptr->activity_status);
    return;
  }

  remote_data_ptr = &(uim_ptr->remote);

  if( uim_ptr->remote.enabled  == FALSE)
  {
    return;
  }

 UIMDRV_MSG_HIGH_3(uim_ptr->id," Received BTSAP event -- 0x%x, state 0x%x, mode 0x%x",
  	                evt_ptr->evt, remote_data_ptr->state, uim_ptr->command.mode);

  switch(evt_ptr->evt)
  {
    case QMI_UIM_REMOTE_EVT_CONNECTION_AVAILABLE :
    {
      /* If card is in PASSIVE mode do not do further processing 
         since it is a passthrough mode */
      if(UIM_PASSIVE_MODE == uim_ptr->command.mode)
      {
        break;
      }
      else if(WAIT_FOR_CONNECTION_AVAILABLE != remote_data_ptr->state)
      {
        /* Since the connection was already established hence restarting the remote sim again
           to process unexpected connection available event */
        if(!IS_BTSAP_HANDLE_ACTIVE(uim_ptr))
        {
          break;
        }

        uim_ptr->remote.poll_interval = UIM_POLL_TIME;
		  
        uim_btsap_send_reconnect_remote_sim(uim_ptr);
        if ( TRUE == uim_ptr->flag.task_startup )
        {
           /* This additional signal is required to break the UIM Driver wait loop */
          (void)rex_set_sigs(uim_ptr->tcb_ptr, UIM_REMOTE_CONNECTION_AVAILABLE_SIG);
        }
      }
      else
      {
        uim_ptr->remote.poll_interval = UIM_POLL_TIME;
        /* Power down local sim and power-up remote sim */  
        uim_btsap_send_power_down_local_sim(uim_ptr);
        if ( TRUE == uim_ptr->flag.task_startup )
        {
           /* This additional signal is required to break the UIM Driver wait loop */
          (void)rex_set_sigs(uim_ptr->tcb_ptr, UIM_REMOTE_CONNECTION_AVAILABLE_SIG);
        }
      }
      break;
    }
    
    case QMI_UIM_REMOTE_EVT_CARD_RESET :
    {
      if(!IS_BTSAP_HANDLE_ACTIVE(uim_ptr))
      {
        return;
      }
      if(remote_data_ptr->state == WAIT_FOR_ATR)
      {
        (void) rex_clr_timer( &uim_ptr->remote.remote_cmd_rsp_timer);
        uim_remote_send_atr(uim_ptr,&(evt_ptr->evt_data.card_reset.atr));
      }
#ifdef FEATURE_UIM_TEST_FRAMEWORK
      #error code not present
#endif
      else 
      {
        remote_data_ptr->btsap.handle = (remote_data_ptr->btsap.handle | UIM_BTSAP_TRIGGER_RECOVERY_MASK);
      }
      break;
    }

    case QMI_UIM_REMOTE_EVT_CARD_INSERTED :
    {
      if(!IS_BTSAP_HANDLE_ACTIVE(uim_ptr))
      {
        break;
      }
      /* check the card status before processing processing card inserted event */
      if(uim_ptr->hotswap.card_status == UIM_CARD_INSERTED)
      {
        /* Send card removed signal to uimdrv */
        if (!uim_hotswap_set_signal(UIM_CARD_REMOVED, uim_ptr))   
        {
          UIMDRV_MSG_LOW_0(uim_ptr->id," the card is already in inserted state, Could not able to set the signal"); 
        }
        /* Send card inserted signal again to uimdrv */
        if (!uim_hotswap_set_signal(UIM_CARD_INSERTED, uim_ptr))   
        {
          UIMDRV_MSG_LOW_0(uim_ptr->id," the card is already in inserted state, Could not able to set the signal"); 
        }
      }

    else
    {
      /* Send card inserted signal to uimdrv */
      if (!uim_hotswap_set_signal(UIM_CARD_INSERTED,uim_ptr))   
      {
        UIMDRV_MSG_LOW_0(uim_ptr->id," HOTSWAP: Could not able to set the signal"); 
      }
    }
    break;
  }

    case QMI_UIM_REMOTE_EVT_CARD_REMOVED :
    {
      if(!IS_BTSAP_HANDLE_ACTIVE(uim_ptr))
      {
        break;
      }
      /* Send card removed signal to uimdrv */
      if (!uim_hotswap_set_signal(UIM_CARD_REMOVED, uim_ptr))   
      {
        UIMDRV_MSG_LOW_0(uim_ptr->id," HOTSWAP: Could not able to set the signal"); 
      }
      remote_data_ptr->state  =  WAIT_FOR_NOTHING;
      break;
    }
	 
    case QMI_UIM_REMOTE_EVT_CARD_ERROR :
    {
      if(!IS_BTSAP_HANDLE_ACTIVE(uim_ptr))
      {
        break;
      }
 #ifdef FEATURE_UIM_TEST_FRAMEWORK
      #error code not present
#endif
      remote_data_ptr->btsap.handle = (remote_data_ptr->btsap.handle | UIM_BTSAP_TRIGGER_RECOVERY_MASK);
      remote_data_ptr->state  =  WAIT_FOR_NOTHING;
      break;
    }

    case QMI_UIM_REMOTE_EVT_CARD_WAKEUP :
    {
      /*Not applicable in BTSAP client mode */
      break;
    }
  
    case QMI_UIM_REMOTE_EVT_CONNECTION_UNAVAILABLE :
    {
      if(!IS_BTSAP_HANDLE_ACTIVE(uim_ptr))
      {
        break;
      }
      uim_ptr->remote.poll_interval = UIM_POLL_TIME;      
      /*Power down remote sim and power-up local sim */
      uim_btsap_send_power_down_remote_sim(uim_ptr);
      break;
    }

    case QMI_UIM_REMOTE_EVT_NOT_RECEIVED :
    case QMI_UIM_REMOTE_EVT_NOT_APPLICABLE :
    {
      break;
    }
  }
}/* uim_btsap_event_callback */

/*===========================================================================
FUNCTION UIM_BTSAP_INIT

DESCRIPTION
  This procedure registers uim with the QMI REMOTE client
  After this registration, events from QMI REMOTE client are received

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void uim_btsap_init (uim_instance_global_type *uim_ptr)
{
  if( uim_ptr == NULL || uim_ptr->remote.enabled	== FALSE)
  {
    return;
  }
  
  /* In case of offtarget NV 70210 is not used*/
#ifndef FEATURE_UIM_TEST_FRAMEWORK
  if(m_HWConfig.uim_slot_config[uim_ptr->id].disableUim)
  {
    /* Slot is disabled */
    return;
  }
#endif /* FEATURE_UIM_TEST_FRAMEWORK */
  
  uim_ptr->remote.state        =  WAIT_FOR_CONNECTION_AVAILABLE;
  uim_ptr->remote.btsap.handle =  0x00;
  if(!uim_ptr->remote.btsap.btsap_cmd_rsp_timer_value)
  {
    uim_ptr->remote.btsap.btsap_cmd_rsp_timer_value = UIM_BTSAP_TRANSACTION_TIME_DURATION;
  }
  
  UIMDRV_MSG_HIGH_1(uim_ptr->id,"uim registration with QMI REMOTE status 0x%x", 
  uim_interface_qmi_uim_remote_register(uim_btsap_event_callback, (qmi_uim_remote_slot_type)uim_ptr->id)); 
} /* uim_btsap_init */


