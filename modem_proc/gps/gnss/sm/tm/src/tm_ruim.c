/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                  RUIM interface of Position Determination Module

GENERAL DESCRIPTION
  This module makes up the RUIM interface of Position Determination subscriber
  station software.

EXTERNALIZED FUNCTIONS


INITIALIZATION AND SEQUENCING REQUIREMENTS


  Copyright (c) 2000 - 2012 Qualcomm Technologies Incorporated.
  Qualcomm Confidential and Proprietary. All Rights Reserved.
  Copyright (c) 2013 - 2014 Qualcomm Atheros, Inc.
  Qualcomm Atheros Confidential and Proprietary. All Rights Reserved. 
  Copyright (c) 2013 - 2019 Qualcomm Technologies, Inc.
  Qualcomm Technologies Confidential and Proprietary. All Rights Reserved. 
 
Export of this technology or software is regulated by the U.S. Government.
Diversion contrary to U.S. law prohibited.

*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/
/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/gnss8.mpss/11.1/gnss/sm/tm/src/tm_ruim.c#2 $

when       who     what, where, why
--------   ---     ---------------------------------------------------------- 
07/05/19   mj      Changes to send notification to read new ready subs IMSI
03/09/18   py     Remove "FEATURE_MMGSDI_SESSION_LIB" dependency. True always.
02/27/18   py      On sub session deactivation,we reset the session entry in tm_mmgsdi_session_id_table
02/21/17   kpn     Removal of switch GPP2_SUPPORT_ON_SEC_SUB
01/04/17   sum     Added call to function tle_MMGSDIReady() and removed call to tm_gsdi_handle_sim_card_inserted_evt()  and tm_gsdi_handle_session_closed_evt().
10/24/16   kpn     Calls to tm_mmgsdi_open_session() to be out of feature flags for LTE-only scenario.
08/30/16   kpn     Add slot id into mmsgsdi global session id table to support pbm_session_record_read()
05/07/16   gk      cache and use the right subid when refresh is received
01/04/16   mj      Add changes to check sub id sanity at call back function
11/25/15   mc      Removed debug message in tm_ruim_event_cb()
11/23/15   mj      Support sending as_id as part of NVRUIM API
10/20/15   rh      Register only on MCFG sub refresh, not slot 
07/08/15   rh      Changed NV refresh to be done on one specifc sub 
11/11/14   rh      Added DSDS support for SIM hot swap 
06/24/14   rh      Add SIM hot swap support 
03/14/15   rh      Added support for MultiSIM NV  
05/13/14   mj      Add featurization to fix SMART compile/link issues
03/04/14   mj      Remove rcevt_wait_name as its leading to a crash waiting on it.
12/23/13   mj	      Remove FEATURE_DUAL_SIM and FEATURE_TRIPLE_SIM
09/27/13   mj      TSTS Support: Open tertiary mmgsdi session for triple SIM
03/03/10   ns      DSDS Supprt
10/22/08   ank     Invoke GSDI API to read CDMA service table only after client reg is complete. 
07/02/08   ank     Added LBS support for CDG OMH R-UIM Spec

===========================================================================*/

/* <EJECT> */
/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "gps_variation.h"
#include "comdef.h"
#include "customer.h"

#ifdef FEATURE_GPSONE
#include "err.h"
#include "tm_common.h" 
#include "tm_ruim.h"
#include "mmgsdisessionlib.h"
#include "nvruimi.h"
#include "aries_os_api.h"
#include "tm_data.h"
#ifdef FEATURE_UIM_SUPPORT_LBS
#include "tm_1x_up.h"
#endif /*FEATURE_UIM_SUPPORT_LBS*/
#include "tm_vx.h"
#include "tm_cm_iface.h"
#ifdef FEATURE_MODEM_CONFIG_REFRESH
#include "mcfg_refresh.h"
#endif /* FEATURE_MODEM_CONFIG_REFRESH */
#ifdef FEATURE_CGPS_XTRA_T
#include "tle_api.h"
#endif /* FEATURE_CGPS_XTRA_T */

/*===========================================================================

            GLOBAL DEFINITIONS AND DECLARATIONS

This section contains global definitions for constants, macros, types,
variables

===========================================================================*/


/*===========================================================================

            LOCAL DEFINITIONS AND DECLARATIONS FOR MODULE

This section contains local definitions for constants, macros, types,
variables and other items needed by this module.

===========================================================================*/

mmgsdi_client_id_type tm_ruim_gsdi_client_id = (mmgsdi_client_id_type)0;
boolean tm_ruim_client_id_registered = FALSE;

#ifdef FEATURE_MODEM_CONFIG_REFRESH
/* TRUE if received at least one SIM inserted event during E911 */
boolean tm_ruim_got_card_inserted_evt = FALSE; 
mmgsdi_event_data_type tm_ruim_cached_card_inserted_event;

/* TRUE if received at least one 1x subscription ready event during E911 */
boolean tm_ruim_got_sub_ready_evt = FALSE; 
mmgsdi_event_data_type tm_ruim_cached_sub_ready_event;

/* TRUE if received at least one 1x session close event during E911 */
boolean tm_ruim_got_session_close_evt = FALSE; 
mmgsdi_event_data_type tm_ruim_cached_session_close_event;

/* we don't need a full cached event queue, only need order information
   for sub_ready & session_close, sim_inserted can always replay at the end */
boolean tm_ruim_last_cached_event_is_session_close = FALSE;

/* MCFG refresh reference ID passed in to GNSS callback */
uint32 tm_ruim_mcfg_refresh_reference_id[SM_CONFIG_MAX_SUBS] = {0};
#endif  /* FEATURE_MODEM_CONFIG_REFRESH */

mmgsdi_session_id_type tm_ruim_gsdi_session_id = (mmgsdi_session_id_type)0;
boolean tm_ruim_session_id_allocated = FALSE; 

/*-------------------------------------------------------------------------
  Local variables
-------------------------------------------------------------------------*/


/*-------------------------------------------------------------------------
    Protypes for local functions:
-------------------------------------------------------------------------*/
LOCAL void tm_ruim_cmd_cb
( 
  mmgsdi_return_enum_type status,
  mmgsdi_cnf_enum_type    cnf,
  const mmgsdi_cnf_type   *cnf_ptr
);

LOCAL void tm_ruim_event_cb
(
  const mmgsdi_event_data_type *event
);

/*====================================================================

FUNCTION tm_mmgsdi_open_session

DESCRIPTION
  Internal function which calls mmgsdi_session_open for the specified
  session type, and generates an error if unsuccessful.

DEPENDENCIES
  This function is to be used only with the session based MMGSDI. 
  Call after client registration with MMGSDI is complete, and CM
  has a valid client ID.

RETURN VALUE
  none

SIDE EFFECTS
  Request to open a session enqueued with MMGSDI.
  ERROR if unsuccessful.

=====================================================================*/
LOCAL void tm_mmgsdi_open_session(
  	uint32 session_type_mask
)
{
   mmgsdi_return_enum_type        mmgsdi_status = MMGSDI_SUCCESS;

   mmgsdi_status = mmgsdi_session_open_ext(
                                        tm_ruim_gsdi_client_id, 
                                        session_type_mask,
                                        tm_ruim_event_cb,
                                        0,
                                        tm_ruim_cmd_cb, 
                                        0);

   /* This should not happen, raise an error */
   if(mmgsdi_status != MMGSDI_SUCCESS)
   {
     MSG_1( MSG_SSID_GPSSM, MSG_LEGACY_MED, "MMGSDI session open failed: STATUS %d" , mmgsdi_status);
   }
} /* tm_mmgsdi_open_session */

/*===========================================================================

FUNCTION  TM_RUIM_IPC_SEND

DESCRIPTION
  
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
boolean tm_ruim_ipc_send 
( 
  uint32  q_MsgId,	
  uint32  q_DestThreadId,
  uint8   data_sz,
  uint8   data[]
)
{
  os_IpcMsgType              *ipc_msg_ptr;

  ipc_msg_ptr = os_IpcCreate(data_sz, IPC_ALLOCATION_DYNAMIC, q_DestThreadId);

  if (ipc_msg_ptr == NULL)
  {
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR,"Failed to create IPC mesage of sz %d",data_sz,0,0); 
    return FALSE;
  }

  ipc_msg_ptr->q_MsgId          = q_MsgId;
  ipc_msg_ptr->q_DestThreadId   = q_DestThreadId;
  ipc_msg_ptr->q_Size           = 0;

  memscpy((void*) ipc_msg_ptr->p_Data, data_sz, (void*) data, data_sz);

  if (!os_IpcSend(ipc_msg_ptr, q_DestThreadId))
  {
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR,"Failed to send IPC mesage %d to Thread %d",
                 q_MsgId, q_DestThreadId, 0);     
    (void)os_IpcDelete(ipc_msg_ptr);   
    return FALSE;
  }

  return TRUE;
}

/*===========================================================================

FUNCTION  tm_gsdi_handle_lbs_svc_available

DESCRIPTION
  
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void tm_gsdi_handle_lbs_svc_available(void)
{

  #ifdef FEATURE_UIM_SUPPORT_LBS
  nvruim_lbs_init_support(TRUE, SYS_MODEM_AS_ID_1);
  nvruim_lbs_init_support(TRUE, SYS_MODEM_AS_ID_2);
  nvruim_lbs_init_support(TRUE, SYS_MODEM_AS_ID_3);

  #if 0 /* Not reading UIM NV as temporary workaround for CR625878 */
  /* re-Read LBS param's from RUIM card */
  tm_core_ruim_read(); // Dynamic mode
  #if defined (FEATURE_CGPS_USES_CDMA) && !defined (FEATURE_GNSS_LOW_MEMORY)
    tm_1x_up_ruim_read();
  #endif /* FEATURE_CGPS_USES_CDMA && !FEATURE_GNSS_LOW_MEMORY*/

  #if defined(FEATURE_GPSONE_VX_LCS_AGENT) && !defined(FEATURE_GNSS_LOW_MEMORY)
    tm_vx_ruim_read();
  #endif /* FEATURE_GPSONE_VX_LCS_AGENT  & !FEATURE_GNSS_LOW_MEMORY */

  #ifdef FEATURE_CGPS_XTRA
    tm_xtra_ruim_init_and_read();
  #endif /* FEATURE_CGPS_XTRA */
  #endif /* CR625878 workaround */

  #endif /*FEATURE_UIM_SUPPORT_LBS*/
}


/*===========================================================================
FUNCTION tm_ruim_is_1x_session_id

DESCRIPTION
  This function check whether a session_id is MMGSDI_1X_PROV_PRI_SESSION
  
DEPENDENCIES
  None

DEPENDENCIES
  None

RETURN VALUE
  TRUE if session_id is 1x MMGSDI_1X_PROV_PRI_SESSION, FALSE otherwise

SIDE EFFECTS
  None
===========================================================================*/
LOCAL boolean tm_ruim_is_1x_session_id
(
  mmgsdi_session_id_type    session_id
)
{
#ifdef FEATURE_UIM_SUPPORT_LBS
  mmgsdi_session_id_type      *tm_mmgsdi_ruim_session_id_ptr = NULL; 

  /* If Dual SIM, use the mmgsdi session info table */
  if((tm_mmgsdi_get_session_id_ptr( MMGSDI_1X_PROV_PRI_SESSION,
      &tm_mmgsdi_ruim_session_id_ptr)) &&
      (*tm_mmgsdi_ruim_session_id_ptr == session_id))
  {
    return TRUE;
  }
#endif /* FEATURE_UIM_SUPPORT_LBS */

  return FALSE;
}

/*===========================================================================
FUNCTION tm_ruim_is_session_id_dd_subs

DESCRIPTION
  This function check whether a session_id is current DD subscription
  (Designated Data)
  
DEPENDENCIES
  None

DEPENDENCIES
  None

RETURN VALUE
  TRUE if session_id is DD subs, FALSE otherwise

SIDE EFFECTS
  None
===========================================================================*/
LOCAL boolean tm_ruim_is_session_id_dd_subs
(
  mmgsdi_session_id_type    session_id
)
{
  sys_modem_as_id_e_type     dd_subs_id, sess_sub_id = SYS_MODEM_AS_ID_NONE;

  /* get SUB ID for this session_id */
  if (tm_mmgsdi_get_subscription_id(session_id, &sess_sub_id) == FALSE)
  {
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "SessID=0x%x no SUBS ID!", session_id, 0, 0);
    return FALSE;
  } 

  dd_subs_id = tm_core_get_current_dd_subs_id();

  MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED, "SessID=0x%x DD_SUB=%d SESS_SUB=%d",
                session_id, (int)dd_subs_id, (int)sess_sub_id );

  if (sess_sub_id == dd_subs_id && sess_sub_id != SYS_MODEM_AS_ID_NONE)
  {
    return TRUE;
  }

  return FALSE;
}


/*===========================================================================
FUNCTION tm_ruim_cmd_cb

DESCRIPTION
  This function handles the MMGSDI client ID registration command callback.  
  It will assign the global client ID value which will be used in mmgsdi 
  function calls

ARGUMENTS
  

DEPENDENCIES
  None

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
LOCAL void tm_ruim_cmd_cb
( 
  mmgsdi_return_enum_type status,
  mmgsdi_cnf_enum_type    cnf,
  const mmgsdi_cnf_type   *cnf_ptr
)
{
  os_IpcMsgType              *ipc_msg_ptr;
  uint32 index = 0;
  uint32                    session_type_mask    = 0;

  if (status == MMGSDI_SUCCESS)
  {
    if (cnf == MMGSDI_CLIENT_ID_AND_EVT_REG_CNF)
    {
      tm_ruim_gsdi_client_id = cnf_ptr->client_id_and_evt_reg_cnf.response_header.client_id;
      MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_MED,"GSDI LBS Client ID 0x%x", tm_ruim_gsdi_client_id);
      tm_ruim_client_id_registered = TRUE;

      ipc_msg_ptr = os_IpcCreate(0, IPC_ALLOCATION_DYNAMIC, (uint32)THREAD_ID_SM_TM);

      if (ipc_msg_ptr != NULL)
      {
        ipc_msg_ptr->q_MsgId          = (uint32)TM_CORE_MSG_ID_MMGSDI_CLIENT_REG;
        ipc_msg_ptr->q_DestThreadId   = (uint32)THREAD_ID_SM_TM;
        ipc_msg_ptr->q_Size           = 0;

        if (!os_IpcSend(ipc_msg_ptr, (uint32)THREAD_ID_SM_TM))
        {
          MSG(MSG_SSID_GPSSM, MSG_LEGACY_ERROR,"Failed to send IPC mesage");     
          (void)os_IpcDelete(ipc_msg_ptr);   
          return;
        }
        return; /*lint !e506 */
      } /* End ipc_msg_ptr */
      else
      {
        MSG(MSG_SSID_GPSSM, MSG_LEGACY_ERROR,"Failed to create IPC mesage"); 
        return;
      } /* End ipc_msg_ptr */
    }
    else if (cnf == MMGSDI_SESSION_OPEN_EXT_CNF)
    {
      /* Set session id for the right session */
      /* If Dual SIM, use the mmgsdi session info table */

    for(index = 0; index < cnf_ptr->session_open_ext_cnf.num_sessions; index++)
    {
        tm_mmgsdi_set_session_id(
        cnf_ptr->session_open_ext_cnf.session_info_ptr[index].session_type,
        cnf_ptr->session_open_ext_cnf.session_info_ptr[index].session_id);


        switch(cnf_ptr->session_open_ext_cnf.session_info_ptr[index].session_type)
        {
           case MMGSDI_1X_PROV_PRI_SESSION:
               session_type_mask = session_type_mask | MMGSDI_1X_PROV_PRI_SESSION_MASK;
              break;
           case MMGSDI_GW_PROV_PRI_SESSION:
               session_type_mask = session_type_mask | MMGSDI_GW_PROV_PRI_SESSION_MASK;
              break;
           case MMGSDI_GW_PROV_SEC_SESSION:
               session_type_mask = session_type_mask | MMGSDI_GW_PROV_SEC_SESSION_MASK;
              break;
           case MMGSDI_GW_PROV_TER_SESSION:
               session_type_mask = session_type_mask | MMGSDI_GW_PROV_TER_SESSION_MASK;
              break;
           default:
               MSG(MSG_SSID_GPSSM, MSG_LEGACY_ERROR,"Error Encountered ");
              break;
         }
    }
    
    MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_MED,"GSDI LBS Opened Session Mask 0x%x",session_type_mask);
    
   }
    else if (cnf == MMGSDI_SRV_AVAILABLE_CNF) 
    {
      if (cnf_ptr->srv_available_cnf.srv_available == TRUE && 
          /* Check session id for RUIM*/   
          tm_ruim_is_1x_session_id(cnf_ptr->srv_available_cnf.response_header.session_id))
      {
          ipc_msg_ptr = os_IpcCreate(0, IPC_ALLOCATION_DYNAMIC, (uint32)THREAD_ID_SM_TM);

          if (ipc_msg_ptr != NULL)
          {
            ipc_msg_ptr->q_MsgId          = (uint32)TM_CORE_MSG_ID_MMGSDI_LBS_SRV_AVAILABLE;
            ipc_msg_ptr->q_DestThreadId   = (uint32)THREAD_ID_SM_TM;
            ipc_msg_ptr->q_Size           = 0;

            if (!os_IpcSend(ipc_msg_ptr, (uint32)THREAD_ID_SM_TM))
            {
              MSG(MSG_SSID_GPSSM, MSG_LEGACY_ERROR,"Failed to send IPC mesage");     
              (void)os_IpcDelete(ipc_msg_ptr);   
              return;
            }
            return; /*lint !e506 */
          } /* End ipc_msg_ptr */
          else
          {
            MSG(MSG_SSID_GPSSM, MSG_LEGACY_ERROR,"Failed to create IPC mesage"); 
            return;
          } /* End ipc_msg_ptr */
      }
      else
      {
        MSG(MSG_SSID_GPSSM, MSG_LEGACY_ERROR,"LBS Svc not available");
      }
    }
    else
    {
      MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_HIGH,"cnf %d not handled", cnf);
    }
  }
  else
  {
    MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_ERROR,"Status =%d",status);
  }
} /* tm_ruim_cmd_cb */


/*===========================================================================
FUNCTION  tm_ruim_event_cb_card_inserted_evt

DESCRIPTION
  Handle MMGSDI_CARD_INSERTED_EVT Callback

DEPENDENCIES
  None

RETURNS
  None

SIDE EFFECTS
  
===========================================================================*/
LOCAL boolean tm_ruim_event_cb_card_inserted_evt
(
    const mmgsdi_event_data_type *event
)
{
  if (event == NULL)
  {
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR,"Null SIM inserted evt!", 0, 0, 0);
    return FALSE;
  }

#ifdef FEATURE_MODEM_CONFIG_REFRESH
  if (tm_cm_is_phone_in_e911_state())
  {
    // save the event (in case we need any other info), we'll keep only the latest such event
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED,"SIM inserted evt cached during E911", 0, 0, 0);
    tm_ruim_got_card_inserted_evt = TRUE;
    tm_ruim_cached_card_inserted_event = *event;  
    return FALSE; 
  }
#endif /* FEATURE_MODEM_CONFIG_REFRESH */

  return TRUE;
}


/*===========================================================================
FUNCTION  tm_ruim_event_cb_sub_ready_evt

DESCRIPTION
  Handle MMGSDI_SUBSCRIPTION_READY_EVT Callback

DEPENDENCIES
  None

RETURNS
  None

SIDE EFFECTS
  
===========================================================================*/
LOCAL boolean tm_ruim_event_cb_sub_ready_evt
(
    const mmgsdi_event_data_type *event
)
{
  if (event == NULL)
  {
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR,"Null Subs Ready evt!", 0, 0, 0);
    return FALSE;
  }

#ifdef FEATURE_UIM_SUPPORT_LBS
#ifdef FEATURE_MODEM_CONFIG_REFRESH
    if (tm_cm_is_phone_in_e911_state())
    {
      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED,"Sub_Ready evt cached during E911", 0, 0, 0);
      tm_ruim_got_sub_ready_evt = TRUE;
      // save the event, we'll keep only the latest such event (for 1x sub ready)
      tm_ruim_cached_sub_ready_event = *event;
      tm_ruim_last_cached_event_is_session_close = FALSE;
      return FALSE; 
    }
#endif /* FEATURE_MODEM_CONFIG_REFRESH */

    if (tm_ruim_ipc_send( TM_CORE_MSG_ID_SIM_SUBSCRIPTION_READY, THREAD_ID_SM_TM, 
                          sizeof(event->data.subscription_ready), 
                          (void *)&event->data.subscription_ready) != TRUE)
    {
      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR," Failed to send IPC for Subs Ready evt!", 0, 0, 0);
      return FALSE;
    }
#endif /* FEATURE_UIM_SUPPORT_LBS */

  return TRUE;
}


/*===========================================================================
FUNCTION  tm_ruim_event_cb_session_changed_evt

DESCRIPTION
  Handle MMGSDI_SESSION_CHANGED_EVT Callback
  (session activated or non-activated switch)
  Usually SIM removal and insertion will trigger this event (active<=>inactive)
 
DEPENDENCIES
  None

RETURNS
  None

SIDE EFFECTS
  
===========================================================================*/
LOCAL void tm_ruim_event_cb_session_changed_evt
(
    const mmgsdi_event_data_type *event
)
{
  if (event == NULL)
  {
    MSG(MSG_SSID_GPSSM, MSG_LEGACY_ERROR,"Null Session Changed evt!");
    return;
  }

#ifdef FEATURE_UIM_SUPPORT_LBS
  /* We will only care about SessionChanged event on DD subs,
     should terminate DD sub session upon this event */
  if (tm_ruim_is_session_id_dd_subs(event->session_id))
  {
    if (tm_ruim_ipc_send( TM_CORE_MSG_ID_SIM_SESSION_CHANGED, THREAD_ID_SM_TM, 0, NULL) != TRUE)
    {
      MSG(MSG_SSID_GPSSM, MSG_LEGACY_ERROR," Failed to send IPC for Session Changed evt!");      
    }
  }
#endif /* FEATURE_UIM_SUPPORT_LBS */

/*When a particular session gets deactivated, the [AppType, Slot and Sub Id] associated is also invalidated 
  so that it will never result in a wrong SessionID to be extracted to retrieve IMSI.*/
  if((uint32)event->data.session_changed.activated == FALSE)
  {  
    tm_mmgsdi_set_app_type(event->session_id, MMGSDI_APP_NONE);
    tm_mmgsdi_set_slot_id(event->session_id, MMGSDI_SLOT_NONE);
    tm_mmgsdi_set_subscription_id(event->session_id, SYS_MODEM_AS_ID_NONE);
  }

  return;
}

/*===========================================================================
FUNCTION  tm_ruim_event_cb_session_close_evt

DESCRIPTION
  Handle MMGSDI_SESSION_CLOSE_EVT Callback
 
DEPENDENCIES
  None

RETURNS
  None

SIDE EFFECTS
  
===========================================================================*/
LOCAL boolean tm_ruim_event_cb_session_close_evt
(
    const mmgsdi_event_data_type *event
)
{
	if (event == NULL)
	{
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR,"Null Session Closed evt!", 0, 0, 0);
		return FALSE;
	}
 tm_mmgsdi_close_session_id(event->session_id);
	return TRUE;
}


/*===========================================================================
FUNCTION  tm_ruim_event_cb 

DESCRIPTION
  GSDI event CB function

DEPENDENCIES
  None

RETURNS
  None

SIDE EFFECTS
  Adds command in LBS command buffer

===========================================================================*/
LOCAL void tm_ruim_event_cb
(
    const mmgsdi_event_data_type *event
)
{
  if (event==NULL)
  {
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR,"tm_ruim_event_cb: NULL ptr!",0,0,0);
    return;
  }

  MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED,"tm_ruim_event_cb called with event: %d session_id 0x%x", event->evt, event->session_id, 0);

  /* Filter events of interest to LBS */
  switch (event->evt)
  {
    case MMGSDI_CARD_INSERTED_EVT:
      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED,"SIM inserted slot=%d aids=%d",
          event->data.card_inserted.slot, event->data.card_inserted.num_aids_avail, 0);
      (void)tm_ruim_event_cb_card_inserted_evt(event);
      break;

    case MMGSDI_SUBSCRIPTION_READY_EVT:
      tm_mmgsdi_set_app_type(event->session_id, event->data.subscription_ready.app_info.app_data.app_type);
      tm_mmgsdi_set_slot_id(event->session_id, event->data.subscription_ready.app_info.slot);
      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED,"Sub App type set SessionID 0x%x, app type %d (slot %d)", event->session_id,
             event->data.subscription_ready.app_info.app_data.app_type,
             event->data.subscription_ready.app_info.slot);
	  #ifdef FEATURE_CGPS_XTRA_T
		/* Notify TLE the MMGSDI session initiation complete */
		tle_MMGSDIReady();
	  #endif
      (void)tm_ruim_event_cb_sub_ready_evt(event);
      /*Inform XTRA about the Subscription ready event*/
      tm_core_set_xtra_uim_subscription((void*)event);
      break;

    case MMGSDI_SESSION_CHANGED_EVT:
      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED,"Session Changed SessionID=0x%x slot=%d active=%d",
            event->session_id, event->data.session_changed.app_info.slot, (uint32)event->data.session_changed.activated);
      /* if activated==FALSE, session is closed */
	  
      tm_ruim_event_cb_session_changed_evt(event);
      break;

    case MMGSDI_SESSION_CLOSE_EVT:
      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED,"Session Closed SessionID=0x%x slot=%d cause=%d",
            event->session_id, event->data.session_close.slot, event->data.session_close.cause);
      (void)tm_ruim_event_cb_session_close_evt(event);
      break;

    case MMGSDI_CARD_ERROR_EVT:   /* will get CARD_ERROR for SIM removal */
      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED,"SIM error slot=%d", (uint32)event->data.card_error.slot, 0, 0);
      break;
    case MMGSDI_CARD_REMOVED_EVT: /* usually won't get this for SIM removal */
      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED,"SIM removed slot=%d", (uint32)event->data.card_removed.slot, 0, 0);
      break;

    default:
      break;
  }
} /* tm_ruim_event_cb */

/*===========================================================================

FUNCTION tm_gsdi_sim_handle_subscription_ready_evt
                                        
DESCRIPTION
  
PARAMETERS

DEPENDENCIES
  None.

RETURN VALUE

                                                 
SIDE EFFECTS


===========================================================================*/ 
void tm_gsdi_sim_handle_subscription_ready_evt(void* p_data)
{
#ifdef FEATURE_UIM_SUPPORT_LBS

  mmgsdi_session_id_type   *tm_mmgsdi_ruim_session_id_ptr = NULL; 
  mmgsdi_session_id_type tm_1x_prov_pri_session_id = (mmgsdi_session_id_type)MMGSDI_INVALID_SESSION_ID;
  boolean                v_sub_ready_event_success = FALSE;
  mmgsdi_slot_id_enum_type  e_slot_id = 0;
  sys_modem_as_id_e_type    e_as_id   = SYS_MODEM_AS_ID_NONE;
  
  tm_ruim_sub_event_info_s_type   *info_ptr;
  MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH,"tm_gsdi_sim_handle_subscription_ready_evt",0,0,0);
  
  if (p_data == NULL)
  {
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR,"TM_RUIM: Null pointer to function ", 0,0,0); 
    return;
  }
  info_ptr = (tm_ruim_sub_event_info_s_type*)p_data;

  /* If Dual SIM, use the mmgsdi session info table */
  if(tm_mmgsdi_get_session_id_ptr( MMGSDI_1X_PROV_PRI_SESSION,
        &tm_mmgsdi_ruim_session_id_ptr) == TRUE)
  {
    tm_1x_prov_pri_session_id = *tm_mmgsdi_ruim_session_id_ptr;
  }

  if(tm_mmgsdi_get_session_id_status(MMGSDI_1X_PROV_PRI_SESSION) == TRUE)  
  {
    if(info_ptr->subscription_ready.app_info.app_data.app_type == MMGSDI_APP_CSIM)
    {
        if ( MMGSDI_SUCCESS == mmgsdi_session_is_service_available(
                                                    tm_1x_prov_pri_session_id,
                                                    MMGSDI_CSIM_SRV_LBS, 
                                                    tm_ruim_cmd_cb,
                                                    NULL 
           ))
        {
          MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH,"Succesfully posted the Session Is Svc Available for CSIM card to MMGSDI",0,0,0);
          v_sub_ready_event_success = TRUE;
        }
        else
        {
          MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR," MMGSDI rejected Session Is Svc Available request for CSIM card ",0,0,0);
        }
    }
    else if(info_ptr->subscription_ready.app_info.app_data.app_type == MMGSDI_APP_RUIM)
    {
        if ( MMGSDI_SUCCESS == mmgsdi_session_is_service_available(
                                                    tm_1x_prov_pri_session_id,
                                                    MMGSDI_CDMA_SRV_LBS,  
                                                    tm_ruim_cmd_cb,
                                                    NULL
           ))
        {
          MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH,"Succesfully posted  Session Is Svc Available for RUIM card to MMGSDI",0,0,0);
          v_sub_ready_event_success = TRUE;
        }
        else
        {
          MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR,"MMGSDI rejected the Session Is Svc Available request for RUIM card",0,0,0);
        }
    }
    else
    {
      /* Handle sub ready event anyway*/
      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH," App type not CSIM or RUIM. Update IMSI anyway",0,0,0);
      v_sub_ready_event_success = TRUE;
    }
  }
  else
  {
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH," Session id not yet allocated. Update IMSI anyway",0,0,0);
    v_sub_ready_event_success = TRUE;
  }

#ifdef FEATURE_MMGSDI_SESSION_LIB
  if (TRUE == v_sub_ready_event_success)
  {
    e_slot_id = info_ptr->subscription_ready.app_info.slot;
    e_as_id   = tm_ruim_map_mmgsdi_slot_id_to_sys_sub_id(e_slot_id);

    if (e_as_id == tm_core_get_current_dd_subs_id())
    {
      MSG_1( MSG_SSID_GPSSM, MSG_LEGACY_MED, "Sub ready event received for DD sub %d. Notify SUPL for IMSI read", e_as_id);
      /* send IPC to notify SUPL to re-read the newly ready SUBs IMSI */
      if (tm_ruim_ipc_send( TM_CORE_READ_NEW_IMSI, 
                       THREAD_ID_SM_TM, 0, NULL) != TRUE)
      {
        MSG(MSG_SSID_GPSSM, MSG_LEGACY_ERROR,"Failed to send IPC for reading new IMSI!");
      }
    }
    else
    {
      MSG_1( MSG_SSID_GPSSM, MSG_LEGACY_MED, "Sub ready event received for non-dd sub %d. Not updating SUPL", e_as_id);
    }
  }
#endif /* FEATURE_MMGSDI_SESSION_LIB*/

#endif //FEATURE_UIM_SUPPORT_LBS
} /* tm_gsdi_sim_handle_subscription_ready_evt */

/*===========================================================================

FUNCTION tm_ruim_abort_non_e911_agps_sessioni
                                        
DESCRIPTION
  
PARAMETERS

DEPENDENCIES
  None.

RETURN VALUE

                                                 
SIDE EFFECTS


===========================================================================*/ 
LOCAL void tm_ruim_abort_non_e911_agps_session(void)
{
  boolean is_in_e911 = tm_cm_is_phone_in_e911_state(); 

  /* phone may be outside E911 state, if location session is started as E911
     let it continue */
  if ( is_in_e911 || tm_core_info.session_info.active_sess_handle == TM_SESS_HANDLE_E911 )
  {
    /* If it's in E911, let E911 session run till end */
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED,"tm_ruim_abort_non_e911_agps_session: e911=%d handle=%d!",
            is_in_e911, tm_core_info.session_info.active_sess_handle, 0);
  }
  else
  {
    /* Do not abort the stand alone sesison. */
    if(tm_core_session_active() && (tm_core_is_standalone_active() == FALSE) )
    {
      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED,"tm_ruim_abort_non_e911_agps_session",0,0,0);
      /* It is not a stand alone session but an AGPS session , abort it.  */
      tm_core_abort_recover(TRUE, TM_STOP_REASON_GENERAL_ERROR);
    }
  }

} /* tm_ruim_abort_non_e911_agps_session */


/*===========================================================================

FUNCTION tm_gsdi_handle_session_changed
                                        
DESCRIPTION
  
PARAMETERS

DEPENDENCIES
  None.

RETURN VALUE

                                                 
SIDE EFFECTS


===========================================================================*/ 
void tm_gsdi_handle_session_changed_evt(void)
{
  MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH,"tm_gsdi_handle_session_changed_evt",0,0,0);

  /* abort non-E911 AGPS session */
  tm_ruim_abort_non_e911_agps_session();

} /* tm_gsdi_handle_session_changed_evt */

/*===========================================================================

FUNCTION tm_gsdi_handle_client_reg_cb
                                        
DESCRIPTION
  
PARAMETERS

DEPENDENCIES
  None.

RETURN VALUE

                                                 
SIDE EFFECTS


===========================================================================*/ 
void tm_gsdi_handle_client_reg_cb(void)
{
	uint32                    session_type_mask    = 0;
	mmgsdi_return_enum_type        mmgsdi_status = MMGSDI_SUCCESS;
	
	MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH,"tm_gsdi_handle_client_reg_cb",0,0,0);

	
/* Open new session */
#if (defined(FEATURE_CDMA_800) || defined(FEATURE_CDMA_1900))
     /* Open session for 1x default provisioning app */
	    session_type_mask = MMGSDI_GW_PROV_PRI_SESSION_MASK |
                         MMGSDI_GW_PROV_SEC_SESSION_MASK |
                         MMGSDI_GW_PROV_TER_SESSION_MASK |
                         MMGSDI_1X_PROV_PRI_SESSION_MASK;
#else
     /* Open session for default provisioning app */
     session_type_mask = MMGSDI_GW_PROV_PRI_SESSION_MASK |
                         MMGSDI_GW_PROV_SEC_SESSION_MASK |
                         MMGSDI_GW_PROV_TER_SESSION_MASK ;
#endif /* #if (defined(FEATURE_CDMA_800) || defined(FEATURE_CDMA_1900)) */

    MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_MED,"GSDI LBS Requested Session Mask 0x%x",session_type_mask);
    tm_mmgsdi_open_session(session_type_mask);
    	
}

#ifdef FEATURE_MODEM_CONFIG_REFRESH
/*===========================================================================

FUNCTION  TM_RUIM_MCFG_REFRESH_CB

DESCRIPTION
  Client task is expected to copy over the p_info and put the refresh update into 
its own command queue 
  
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
LOCAL boolean tm_ruim_mcfg_refresh_cb(mcfg_refresh_info_s_type *p_info)
{
  uint8 sub = 0;

  if (p_info == NULL)
  {
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR,"GNSS MCFG_REFRESH got NULL ptr!", 0, 0, 0);
    return FALSE;
  }

  MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED,"GNSS MCFG_REFRESH got type=%d index=%d slot=%d", 
          (uint32)p_info->type, (uint32)p_info->slot_index, (uint32)p_info->sub_index);

  sub = (uint8)p_info->sub_index; /* value 0,1 or 2 */

  if ((sub < SM_CONFIG_MIN_SUB_ID) || 
      (sub > tm_core_info.config_info.max_valid_subs_id) || (sub > SM_CONFIG_MAX_SUB_ID))
  {
    MSG_1( MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "NV Refresh Invalid sub %u. Abort Refresh on this SUB", sub);
    return FALSE;
  }
  // save reference ID passed in by MCFG REFRESH, need it when reporting DONE
  tm_ruim_mcfg_refresh_reference_id[sub] = p_info->reference_id;
  
  /* send IPC to let TM refresh carrier-specific NV asynchronously, don't block MCFG_REFRESH */
  if (tm_ruim_ipc_send( TM_CORE_MSG_ID_REFRESH_CARRIER_SPECIFIC_NV, 
                       THREAD_ID_SM_TM, sizeof(uint8), &sub) != TRUE)
  {
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR,"Failed to send IPC for carrier specific NV refresh!", 0, 0, 0);
	return FALSE;
  }

  return TRUE;
}


/*===========================================================================

FUNCTION  TM_RUIM_MCFG_REFRESH_REGISTER

DESCRIPTION
  This function is used to register with MCFG to receive refresh update through
  registered callback function for MCFG switching
  
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
LOCAL void tm_ruim_mcfg_refresh_register(void)
{
  mcfg_refresh_registration_s_type gnss_mcfg_refresh = 
  {
    MCFG_REFRESH_ORDER_50,        /**< Refresh order ID registed for refresh */
    MCFG_REFRESH_INDEX_MASK_NONE, /**< Mask for slot(s) to recieve refresh update */
    MCFG_REFRESH_INDEX_MASK_ALL,  /**< Mask for sub(s) to recieve refresh update */
    0,                            /**< Reserved. */
    tm_ruim_mcfg_refresh_cb       /**< Callback function to receive refresh update */
  };

  boolean status = mcfg_refresh_register(&gnss_mcfg_refresh);

  if (status == TRUE)
  {
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED,"GNSS MCFG_REFRESH registration success", 0, 0, 0);
  }
  else
  {
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR,"GNSS MCFG_REFRESH registration fail!", 0, 0, 0);
  }
}
#endif /* FEATURE_MODEM_CONFIG_REFRESH */

/*===========================================================================

FUNCTION  TM_RUIM_CARRIER_NV_REFRESH_DONE

DESCRIPTION
  Called by TM task to notify TM-RUIM that Carrier-specific NV refresh done
  
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void tm_ruim_carrier_nv_refresh_done ( sys_modem_as_id_e_type sub )
{
#ifdef FEATURE_MODEM_CONFIG_REFRESH
  /* This function is used to notify MCFG task that registered client has
  completed refresh operation. The client is expected to return the same
  reference id passed through registered callback */
  (void)mcfg_refresh_done(tm_ruim_mcfg_refresh_reference_id[sub]);  
#endif /* FEATURE_MODEM_CONFIG_REFRESH */
}

/*===========================================================================

FUNCTION  TM_RUIM_HANDLE_POST_E911

DESCRIPTION
  TM RUIM handling after E911 session
  
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void tm_ruim_handle_post_e911 ( void )
{
#ifdef FEATURE_MODEM_CONFIG_REFRESH
  /* to replay all the cached MMGSDI events */

  mmgsdi_events_enum_type cached_events[2];
  uint8 i;

  /* recontruct cached event order */
  if (tm_ruim_last_cached_event_is_session_close == TRUE)
  {
    cached_events[0] = MMGSDI_SESSION_CLOSE_EVT;
    /* skip Sub Ready event since 1x Session Close came later, 1x sub ready is not relevant anymore */
    tm_ruim_got_sub_ready_evt = FALSE;  
    tm_ruim_last_cached_event_is_session_close = FALSE; // reset to initial value
  }
  else
  {
    cached_events[0] = MMGSDI_SUBSCRIPTION_READY_EVT;  /* 1x Sub Ready event came, session should remain open */
  }
  cached_events[1] = MMGSDI_CARD_INSERTED_EVT;  /* backup: to open all remaining not-open sessions */

  // replay up to 3 cached events
  for (i = 0; i < sizeof(cached_events)/sizeof(cached_events[0]); i++)
  {
    switch (cached_events[i])
    {
      case MMGSDI_CARD_INSERTED_EVT:   /* for Card Inserted event */
        if (tm_ruim_got_card_inserted_evt == TRUE)
        {
          if (tm_ruim_event_cb_card_inserted_evt(&tm_ruim_cached_card_inserted_event) == TRUE)
          {
            tm_ruim_got_card_inserted_evt = FALSE;  // have replayed SIM inserted event
          }
        }
        break;

      case MMGSDI_SUBSCRIPTION_READY_EVT:   /* for 1x Sub Ready event */
        if (tm_ruim_got_sub_ready_evt == TRUE)
        {
          if (tm_ruim_event_cb_sub_ready_evt(&tm_ruim_cached_sub_ready_event) == TRUE)
          {
            tm_ruim_got_sub_ready_evt = FALSE;  // have replayed Sub Ready event
          }
        }
        break;

      case MMGSDI_SESSION_CLOSE_EVT:  /* for 1x Session Close event */
        if (tm_ruim_got_session_close_evt == TRUE)
        {
          if (tm_ruim_event_cb_session_close_evt(&tm_ruim_cached_session_close_event) == TRUE)
          {
            tm_ruim_got_session_close_evt = FALSE;  // have replayed Session Close event
          }
        }
        break;
      default: /* All other everts */
        /* Do Nothing */
        break;
    } /* switch */
  } /* for */
#endif /* FEATURE_MODEM_CONFIG_REFRESH */
}


/*===========================================================================

FUNCTION  TM_RUIM_GSDI_INIT

DESCRIPTION
  GSDI (generic SIM driver interface) Initialization function. This function does the following:
  - Registers the client call back function for GSDI events,
  - Register as a client of MMGSDI.
  
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void tm_ruim_gsdi_init ( void )
{
  /* Register for MMGSDI client ID:
   */
  if ( MMGSDI_SUCCESS != mmgsdi_client_id_and_evt_reg( tm_ruim_event_cb,
                                                       tm_ruim_cmd_cb, 
                                                       0 ) )
  {
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR,"MMGSDI client_id_and_event registration failed", 0, 0, 0);
  }

#ifdef FEATURE_MODEM_CONFIG_REFRESH
  tm_ruim_mcfg_refresh_register();
#endif /* FEATURE_MODEM_CONFIG_REFRESH */

  return;
}/* tm_ruim_gsdi_init */


#ifdef FEATURE_MMGSDI_SESSION_LIB
/*===========================================================================

FUNCTION tm_ruim_map_mmgsdi_slot_id_to_sys_sub_id
                                        
DESCRIPTION
  Map from mmgsdi slot id to sys modem as_id
  
PARAMETERS
  mmgsdi_slot_id_enum_type  e_slot_id

DEPENDENCIES
  None

RETURN VALUE
  sys_modem_as_id_e_type
                                                 
SIDE EFFECTS
  None

===========================================================================*/ 
sys_modem_as_id_e_type tm_ruim_map_mmgsdi_slot_id_to_sys_sub_id(mmgsdi_slot_id_enum_type  e_slot_id)
{
  sys_modem_as_id_e_type e_sub_id = SYS_MODEM_AS_ID_NONE;
  
  switch (e_slot_id)  
  {
    case MMGSDI_SLOT_1:
      e_sub_id = SYS_MODEM_AS_ID_1;
      break;

    case MMGSDI_SLOT_2:
      e_sub_id = SYS_MODEM_AS_ID_2;
      break;

    case MMGSDI_SLOT_3:
      e_sub_id = SYS_MODEM_AS_ID_3;
      break;

    default:
      MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_HIGH,"MMGSDI Slot is %d, reset sub slot id to SYS_MODEM_AS_ID_1", e_slot_id);  
      e_sub_id = SYS_MODEM_AS_ID_1;
  }
  return e_sub_id;  
}
#endif /* FEATURE_MMGSDI_SESSION_LIB */

#endif /* FEATURE_GPSONE */

