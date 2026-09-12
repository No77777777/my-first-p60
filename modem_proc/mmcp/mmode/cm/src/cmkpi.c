/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

 C A L L   M A N A G E R   K E Y   P E R F O R M A N C E   I N D I C A T O R


GENERAL DESCRIPTION
  This module makes up the Call Manager key performance indicator software.

EXTERNALIZED FUNCTIONS


INITIALIZATION AND SEQUENCING REQUIREMENTS


Copyright (c) 1998 - 2015 by Qualcomm Technologies INCORPORATED. All Rights Reserved.

Export of this technology or software is regulated by the U.S. Government.
Diversion contrary to U.S. law prohibited.

*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/


/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

===========================================================================*/



/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
/* Customer configuration file
*/
#include "cmkpi.h"
#ifdef FEATURE_DISABLED_HABANERO
#include "qevent.h"
#include "cafi_kpi_client.h"
#endif
/*==========================================================================

FUNCTION: cmkpi_oprt_mode

DESCRIPTION
  This function calls into the QSH API to start/stop power-up/airplane mode
  exit to attach measurements.
  KPI: Power-up/airplane mode exit to attach.
  Trigger: LPM exit.
  Ending: 

DEPENDENCIES
  

RETURN VALUE
 

SIDE EFFECTS
  None
==========================================================================*/
 extern void cmkpi_oprt_mode(sys_oprt_mode_e_type oprt_mode,
							 sys_modem_as_id_e_type asubs_id)
{
  #ifdef FEATURE_DISABLED_HABANERO
  if(SYS_OPRT_MODE_ONLINE == oprt_mode)
  {
    /*THis event starts the measurements.*/
  	QEVENT( QSH_CLT_CM, 
            CM_QSH_EVENT_ONLINE,
      	    CM_QSH_EVENT_ONLINE_VER,
            asubs_id,
            "pwrup_event"
          );
  }
  else if(SYS_OPRT_MODE_LPM == oprt_mode)
  {
    QEVENT( QSH_CLT_CM, 
            CM_QSH_EVENT_LPM,
            CM_QSH_EVENT_LPM_VER,
            asubs_id,
            "lpm_event");
  }
  #endif
  CM_MSG_HIGH_1("cmkpi_oprt_mode = %d",oprt_mode);
}

/*==========================================================================

FUNCTION: cmkpi_service_status_handler

DESCRIPTION
  The cmkpi_service_status_handler helps :
  1. Trigger the end and fail events for Power-up/airplane mode exit to 
     attach KPI.
  2. Trigger start and end for Time in OOS KPI.
  3. Trigger start and end for Time in Limited Service KPI.

DEPENDENCIES

RETURN VALUE
 

SIDE EFFECTS
  None
==========================================================================*/
extern void cmkpi_service_status_handler(sys_srv_status_e_type srv_status,
							             sys_modem_as_id_e_type asubs_id,
	                                     sys_roam_status_e_type roaming_status,
     multimode_stack_e_type stk_id,
     sys_sys_mode_e_type sys_mode)
{
  cm_qsh_srv_status service_status = CM_QSH_SRV_NONE;

  switch(srv_status)
  {
    case SYS_SRV_STATUS_SRV:
	  service_status = CM_QSH_FULL_SRV;
	  break;
	case SYS_SRV_STATUS_NO_SRV:
	  service_status = CM_QSH_NO_SRV;
	  break;
	case SYS_SRV_STATUS_LIMITED:
	  service_status = CM_QSH_LTD_SRV;
	  break;
	case SYS_SRV_STATUS_LIMITED_REGIONAL:
	  service_status = CM_QSH_LTD_REGIONAL_SRV;
	  break;
	default:
	  break;
  }

 /*TODO: CM should check service status on all stacks and report the highest service status to QSH.*/

  CM_MSG_HIGH_3("cmkpi_service_status_handler = %d, stack_id = %d, sys_mode=%d",service_status, stk_id, sys_mode);
  #ifdef FEATURE_DISABLED_HABANERO
  QEVENT( QSH_CLT_CM, 
          CM_QSH_EVENT_SERVICE_STATUS,
	        CM_QSH_EVENT_SERVICE_STATUS_VER,
          asubs_id,
          "srv status: %d, roaming_status = %d, sys_mode=%d",
          service_status,
          roaming_status,
          sys_mode
        );
  #endif
}/*cmkpi_service_status_handler()*/


/*==========================================================================

FUNCTION: cmkpi_sim_ready

DESCRIPTION
  This function calls into the QSH API to start/stop sim ready measurements.
  KPI: SIM ready.
  Trigger: online.
  Ending: Sim ready.

DEPENDENCIES
  

RETURN VALUE
 

SIDE EFFECTS
  None
==========================================================================*/
extern void cmkpi_sim_ready(sys_modem_as_id_e_type asubs_id)
{
  #ifdef FEATURE_DISABLED_HABANERO
    QEVENT( QSH_CLT_CM, 
          CM_QSH_EVENT_SIM_READY,
          CM_QSH_EVENT_SIM_READY_VER,
          asubs_id,
          "sim_ready_event"
          );
  #endif
}/*cmkpi_sim_ready()*/

/*==========================================================================

FUNCTION: cmkpi_qsh_event_handler

DESCRIPTION
  This function calls into the QSH API for VOLTE/VOWIFI call event and CS call end event handling
  KPI: VOLTE MO/MT, VOWIFI MO/MT, VOLTE/VOWIFI call end, SRVCC, CS call release

DEPENDENCIES
  

RETURN VALUE
 none

SIDE EFFECTS
  None
==========================================================================*/

void cmkpi_qsh_event_handler
(
    cm_call_event_e_type call_event, 
    cmcall_s_type* call_ptr
)
{
  cm_qsh_evt_call_state qsh_call_state = CM_QSH_CALL_STATE_NONE;
  sys_sys_mode_e_type sys_mode = SYS_SYS_MODE_NONE;


  CM_MSG_HIGH_1("cmkpi_qsh_event_handler: event %d",call_event);
  switch(call_event)
  {
  /*handle both VOLTE/VOWIFi MO and MT(INVITE)*/
  case CM_CALL_EVENT_ORIG:
    qsh_call_state = CM_QSH_CALL_START;   
    
    if(call_ptr->is_vowlan)
    {
      #ifdef FEATURE_DISABLED_HABANERO
      QEVENT( QSH_CLT_CM, 
              CM_QSH_EVENT_VOWIFI_CALL_STATUS,
        	    CM_QSH_EVENT_VOWIFI_CALL_STATUS_VER,
              call_ptr->mm_id.asubs_id,
              "call_state: %d, call_direction: %d",
              qsh_call_state,
              call_ptr->direction);
      #endif
    }
    else if(call_ptr->is_volte || call_ptr->cmcall_mode_info.info_type == CM_CALL_MODE_INFO_IP)
    {
      #ifdef FEATURE_DISABLED_HABANERO
      QEVENT( QSH_CLT_CM, 
              CM_QSH_EVENT_VOLTE_CALL_STATUS,
        	    CM_QSH_EVENT_VOLTE_CALL_STATUS_VER,
              call_ptr->mm_id.asubs_id,
              "call_state: %d, call_direction: %d",
              qsh_call_state,
              call_ptr->direction);
      #endif
    }
    else
    {
      CM_MSG_HIGH_0("call KPI not supported");
      return;
    }

    break;
    
  /*RING conf*/
  case CM_CALL_EVENT_INCOM:
  case CM_CALL_EVENT_PROGRESS_INFO_IND:
    if(call_event == CM_CALL_EVENT_INCOM)
      qsh_call_state = CM_QSH_CALL_RINGING;
    else if(call_event == CM_CALL_EVENT_PROGRESS_INFO_IND)
      qsh_call_state = CM_QSH_CALL_IN_PROGRESS;

    if(call_ptr->is_volte)
    {
      #ifdef FEATURE_DISABLED_HABANERO
      QEVENT( QSH_CLT_CM, 
          CM_QSH_EVENT_VOLTE_CALL_STATUS,
    	    CM_QSH_EVENT_VOLTE_CALL_STATUS_VER,
          call_ptr->mm_id.asubs_id,
          "call_state: %d",
          qsh_call_state);
      #endif
    }
    else if(call_ptr->is_vowlan)
    {
      #ifdef FEATURE_DISABLED_HABANERO
      QEVENT( QSH_CLT_CM, 
          CM_QSH_EVENT_VOWIFI_CALL_STATUS,
    	    CM_QSH_EVENT_VOWIFI_CALL_STATUS_VER,
          call_ptr->mm_id.asubs_id,
          "call_state: %d",
          qsh_call_state);
      #endif
    }
    else
    {
      CM_MSG_HIGH_0("call KPI not supported");
      return;
    }

    break;

  case CM_CALL_EVENT_END:
  {
    sys_call_end_event_e_type call_end_evt_type = SYS_CALL_END_EVENT_NONE;
    cm_qsh_call_end_reason call_end_reason=CM_QSH_CALL_END_NORMAL;
    

    #ifdef CM_GW_SUPPORTED
    if( call_ptr->cmcall_mode_info.info_type == CM_CALL_MODE_INFO_GW_CS )
    {
      call_end_evt_type = cmdbg_qsh_determine_3gpp_call_end_evt_type(call_ptr);

    }
    else
    #endif
    if (call_ptr->cmcall_mode_info.info_type == CM_CALL_MODE_INFO_CDMA )
    {
      call_end_evt_type = cmdbg_qsh_determine_3gpp2_call_end_evt_type(call_ptr);
    }
    else if(call_ptr->cmcall_mode_info.info_type == CM_CALL_MODE_INFO_IP)
    {
      call_end_evt_type = cmdbg_qsh_determine_ims_call_end_evt_type(call_ptr);
    }

    switch(call_end_evt_type)
    {
    case SYS_CALL_END_EVENT_NORMAL_CALL_END:
      call_end_reason = CM_QSH_CALL_END_NORMAL;
      break;
      
    case SYS_CALL_END_EVENT_RX_DISCONNECT:
    case SYS_CALL_END_EVENT_RX_RELEASE:
      call_end_reason = CM_QSH_CALL_END_NW_REL;
      break;

    case SYS_CALL_END_EVENT_TX_DISCONNECT:
    //case SYS_CALL_END_EVENT_TX_RELEASE:
      call_end_reason = CM_QSH_CALL_END_UE_REL;
      break;

    case SYS_CALL_END_EVENT_HANDOVER_FAIL:
    case SYS_CALL_END_EVENT_RLF_OR_WEAK_SIGNAL:
    case SYS_CALL_END_EVENT_RX_RRC_RELEASE:
      call_end_reason = CM_QSH_CALL_END_AS_FAIL;
      break;

    case SYS_CALL_END_EVENT_OTHERS:
      call_end_reason = CM_QSH_CALL_END_OTHER;
      break;

    default:
      break;
    }

    if(call_ptr->sys_mode == SYS_SYS_MODE_LTE)
    {
      #ifdef FEATURE_DISABLED_HABANERO
     QEVENT( QSH_CLT_CM, 
        CM_QSH_EVENT_VOLTE_CALL_STATUS,
        CM_QSH_EVENT_VOLTE_CALL_STATUS_VER,
        call_ptr->mm_id.asubs_id,
        "call_state: %d, end_reason %d",
        CM_QSH_CALL_END,
        call_end_reason);
      #endif
    }
    else if(call_ptr->sys_mode == SYS_SYS_MODE_WLAN)
    {
      #ifdef FEATURE_DISABLED_HABANERO
      QEVENT( QSH_CLT_CM, 
        CM_QSH_EVENT_VOWIFI_CALL_STATUS,
        CM_QSH_EVENT_VOWIFI_CALL_STATUS_VER,
        call_ptr->mm_id.asubs_id,
        "call_state: %d, end_reason %d",
        CM_QSH_CALL_END,
        call_end_reason);
      #endif
    }
    else
    {
      #ifdef FEATURE_DISABLED_HABANERO
      QEVENT( QSH_CLT_CM, 
        CM_QSH_EVENT_CS_CALL_END,
        CM_QSH_EVENT_CS_CALL_END_VER,
        call_ptr->mm_id.asubs_id,
        "end_reason %d",
        call_end_reason);
      #endif
    }
    break;

  }
    
  case CM_CALL_EVENT_HO_START:
  case CM_CALL_EVENT_HO_COMPLETE:
  case CM_CALL_EVENT_HO_FAIL:
  case CM_CALL_EVENT_HO_CANCEL:
  {
    if(call_ptr->voice_ho_type != SYS_VOICE_HO_SRVCC_L_2_G &&
      call_ptr->voice_ho_type != SYS_VOICE_HO_SRVCC_L_2_W)
    {
      CM_MSG_HIGH_0("HO KPI not supported");
      return;
    }

    if(call_ptr->voice_ho_type == SYS_VOICE_HO_SRVCC_L_2_G)
      sys_mode = SYS_SYS_MODE_GSM;
    else
      sys_mode = SYS_SYS_MODE_WCDMA;
    
    if(call_event == CM_CALL_EVENT_HO_START)
    {
      #ifdef FEATURE_DISABLED_HABANERO
      QEVENT( QSH_CLT_CM, 
        CM_QSH_EVENT_SRVCC,
        CM_QSH_EVENT_SRVCC_VER,
        call_ptr->mm_id.asubs_id,
        "srvcc evt: %d, ho_rat_type: %d",
	CAFI_KPI_EVENT_IND_START,
        sys_mode
        );
      #endif
    }
    else if(call_event == CM_CALL_EVENT_HO_COMPLETE)
    {
      #ifdef FEATURE_DISABLED_HABANERO
      QEVENT( QSH_CLT_CM, 
          CM_QSH_EVENT_SRVCC,
          CM_QSH_EVENT_SRVCC_VER,
          call_ptr->mm_id.asubs_id,
          "srvcc_evt: %d, end_reason %d, ho_rat_type: %d",
		      CAFI_KPI_EVENT_IND_SUCCESS,
          call_event,
          sys_mode
          );
      #endif
    }
    else
    {
      #ifdef FEATURE_DISABLED_HABANERO
      QEVENT( QSH_CLT_CM,
          CM_QSH_EVENT_SRVCC,
          CM_QSH_EVENT_SRVCC_VER,
          call_ptr->mm_id.asubs_id,
          "srvcc_evt: %d, end_reason %d, ho_rat_type: %d",
		      CAFI_KPI_EVENT_IND_FAIL,
          call_event,
          sys_mode
          );
      #endif
    }
    break;
   }

   default:
     break;
  }
  return;
}
