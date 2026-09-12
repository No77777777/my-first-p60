/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

              L 1  I D L E  M O D E  T A S K  G P R S  M E S S A G E
                              P R O C E S S I N G

GENERAL DESCRIPTION
   This module contains the procedures to handle events associated with GPRS
   while the L1 task is in the IDLE mode.  Events can originate from other
   layers or from the L1 ISR.

EXTERNALIZED FUNCTIONS

INITIALIZATION AND SEQUENCING REQUIREMENTS

Copyright (c) 2001 - 2015 Qualcomm Technologies, Inc.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.


$Header: //components/rel/geran.mpss/7.4.0/gl1/src/gpl1_gprs_error_recovery.c#2 $
$DateTime: 2020/01/27 07:43:27 $ $Author: pwbldsvc $

when       who       what, where, why
--------   -----  ----------------------------------------------------------
27/01/20   rks     CR2079539 FR 42539: GERAN changes for Mini Dump Phase - 2 support [GL1]
07/02/18   rc      CR2184167 Sending MAC_L1_RECOVERY_CNF after L1 moved to Idle State.
1/9/17     rc      CR2102233 Registering and deregistering pkt error recovery events when not in DTM and DED modes
16/03/17   mn      CR2020307 FR40846:Packet transfer Error Recovery mechanism.

===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "geran_variation.h"
#include "customer.h"

#ifdef FEATURE_GERAN_PTM_ERR_RECOVERY

#include "msg.h"
#include "ms.h"
#include "l1_sc.h"
#include "l1_sc_int.h"
#include "l1_drx.h"
#include "l1_log.h"
#include "l1_fm.h"
#include "gmactransfer_if.h"
#include "gpl1_gprs_task.h"
#include "gmacl1sig.h"
#include "gmacl1sig_g.h"
#include "gmacutil.h"
#include "gmacl1.h"
#include "l1i.h"
#include "l1_isr.h"
#include "l1_utils.h"     /* gl1_get_FN() prtotype */
#include "l1_task.h"      /* l1_transfer_data extern */
#include "gl1_sys_algo_gprs.h" /* GPRS AGC API */
#include "gl1_msg_pdch.h" /* NPL1 message layer API */
#include "ms.h"           /* for inter-task Message set's */
#include "l1_sc.h"
#include "gpl1_gprs_ptcch.h"
#include "gpl1_gprs_utils.h"
#include "gmactransfer_if.h"
#include "l1_sc_irat.h" /*INVALID_FN macro*/
#include "gpl1_gprs_error_recovery.h"
#if defined ( FEATURE_QSH_EVENT_NOTIFY_TO_QSH ) || defined ( FEATURE_QSH_EVENT_NOTIFY_HANDLER )
#include "l1_log_qsh.h"
#endif


/*========================== #defines =====================================*/

gpl1_recovery_data_t GPl1_recovery_data[NUM_GERAN_DATA_SPACES];

uint8 gl1_gprs_pkt_rec_support = 0;

/*========================== global function declarations ==================*/

/*======================== global Data =====================================*/

/*========================== Local types/enums ============================*/

/*========================== Local function declarations ==================*/

/*===========================================================================

FUNCTION      gpl1_gprs_check_for_recovery

DESCRIPTION   Check whether error recovery should be initiated or not.

RETURN VALUE  None

===========================================================================*/

static boolean gpl1_gprs_check_for_recovery(gas_id_t gas_id);

/*======================== Local Data =====================================*/

/*===========================================================================

    DEFINITIONS AND DECLARATIONS FOR MODULE

    This section contains definitions for constants, macros, types, variables
    and other items needed by this module.

===========================================================================*/

/*===========================================================================

FUNCTION  gpl1_gprs_recovery_map_tbf_type()

DESCRIPTION

      Map Gl1 alloc type to corresponding MAC TBF type . 
  
DEPENDENCIES
  None

RETURN VALUE
  MAC TBF type

SIDE EFFECTS
  None

===========================================================================*/
active_tbf_t gpl1_gprs_recovery_map_tbf_type(gas_id_t gas_id)
{
   active_tbf_t mapped_tbf_type= NO_TBF;
   volatile ISRTIM_CMD_BLK *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
   transfer_data_T *transfer_data_ptr = l1_tskisr_blk->current_params.L1Data.pTransfer_data;
   
   if(L1_DYNAMIC_DOWN_ALLOC == transfer_data_ptr->current_alloc_type)
   {
        mapped_tbf_type = UL_AND_DL;
   }
   else if(((UPLINK_TBF_ESTABLISHED == l1_transfer_data[gas_id].tbf_est_type) && (L1_SINGLE_BLOCK_ALLOC == transfer_data_ptr->current_alloc_type))||(L1_DYNAMIC_ALLOC == transfer_data_ptr->current_alloc_type))
   {
      mapped_tbf_type = UL_ONLY;
   } 
   else if(((DOWNLINK_TBF_ESTABLISHED == l1_transfer_data[gas_id].tbf_est_type) && (L1_SINGLE_BLOCK_ALLOC == transfer_data_ptr->current_alloc_type))||(L1_DOWNLINK_ALLOC == transfer_data_ptr->current_alloc_type))
   {
      mapped_tbf_type = DL_ONLY;
   }
  
  return mapped_tbf_type; 
}



/*===========================================================================

FUNCTION  gpl1_gprs_recovery_l1_mac_state_check

DESCRIPTION
   Checks whether state mismatch has occured between GL1/MAC .
   
DEPENDENCIES
  None

RETURN VALUE
  boolean 

SIDE EFFECTS
  None

===========================================================================*/
boolean gpl1_gprs_recovery_l1_mac_state_check(gas_id_t gas_id)
{
   active_tbf_t mac_tbf_state = NO_TBF;
   uint8 mac_main_state = 0;
   boolean state_not_matched = FALSE;
   active_tbf_t gpl1_gprs_tbf_state = gpl1_gprs_recovery_map_tbf_type(gas_id);
   
   gmac_get_tbf_status(&mac_main_state,&mac_tbf_state,gas_id);
   
   if(mac_tbf_state != gpl1_gprs_tbf_state)
   { 
     MSG_GERAN_HIGH_2_G("REC:gpl1_gprs_recovery_l1_mac_state_check mac_tbf_state = %d gl1_gprs_tbf_state %d", mac_tbf_state, gpl1_gprs_tbf_state);
     state_not_matched = TRUE;
   }
   return state_not_matched;
}


/*===========================================================================

FUNCTION  gpl1_gprs_recovery_send_l1_mac_recovery_ind

DESCRIPTION
  Creates L1_MAC_RECOVERY_IND message and sends to the GS_QUEUE_MAC
  queue.

  The function informs MAC that GL1 detected error recovery .Gl1 Needs to be resetted.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gpl1_gprs_recovery_send_l1_mac_recovery_ind(gas_id_t gas_id)
{
  l1_mac_sig_t   message_buffer,*message;
  MSG_GERAN_HIGH_0_G( "REC:gpl1_gprs_recovery_send_l1_mac_recovery_ind");
  message = &message_buffer;

  /* Set up the message header */
  message->task_id = MS_MAC_L1;
  message->sig_id =  L1_MAC_RECOVERY_IND;
  message->msg.recovery_ind.gas_id = gas_id;
  /* Send the message */
  L1_send_gprs_gmac_message(message, NULL,gl1_get_FN(gas_id), gas_id);
}

/*===========================================================================

FUNCTION  gpl1_gprs_recovery_send_l1_mac_recovery_cnf

DESCRIPTION
  Creates L1_MAC_RECOVERY_CNF message and sends to the GS_QUEUE_MAC
  queue.

  The function informs MAC that GL1 reseted and moved to idle.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gpl1_gprs_recovery_send_l1_mac_recovery_cnf(gas_id_t gas_id)
{
  l1_mac_sig_t   message_buffer,*message;
  MSG_GERAN_HIGH_0_G( "REC:gpl1_gprs_recovery_send_l1_mac_recovery_cnf");
  message = &message_buffer;

  /* Set up the message header */
  message->task_id = MS_MAC_L1;
  message->sig_id =  L1_MAC_RECOVERY_CNF;
  message->msg.recovery_cnf.gas_id = gas_id;
  /* Send the message */
  L1_send_gprs_gmac_message(message, NULL,gl1_get_FN(gas_id), gas_id);
}



/*===========================================================================

FUNCTION gpl1_gprs_check_for_recovery

DESCRIPTION
             Check whether error recovery should be initiated or not.

PARAMS       gas_id - geran access stratum id.

DEPENDENCIES

RETURN VALUE Boolean

===========================================================================*/

static boolean gpl1_gprs_check_for_recovery(gas_id_t gas_id)
{
   uint32 current_FN = GSTMR_GET_FN_GERAN(gas_id);
   boolean recovery_required = FALSE;
   uint8 event;
   uint32 recovery_fn;

   

   for(event=GPL1_UL_CONNECT_EVENT; event< GPL1_MAX_EVENT; event++)
   {
      switch(event)
      {
      
          case GPL1_UL_CONNECT_EVENT:
          case GPL1_DL_CONNECT_EVENT:
          case GPL1_LOW_TS_EVENT:
          {
            MSG_GERAN_HIGH_2_G("REC:gpl1_gprs_check_for_recovery event=%d recovery_required=%d",event,recovery_required);
            if(TRUE == GPl1_recovery_data[gas_id].event_table[event].valid) 
            {
               recovery_fn =ADD_FN(GPl1_recovery_data[gas_id].event_table[event].starting_time,GPL1_FRAME_COUNT_FOR_RECOVERY_100);
               if(IS_FRAME_NUM_LATER(current_FN,recovery_fn))
               {
                   if(GPl1_recovery_data[gas_id].event_table[event].recovery_check_count==GPL1_WAIT_COUNT_TRIGGER_RECOVERY)
                   {
                        recovery_required=TRUE;
                        MSG_GERAN_HIGH_2_G("REC:gpl1_gprs_check_for_recovery event=%d recovery_required=%d",event,recovery_required);
                   }
                   else
                   {
                        GPl1_recovery_data[gas_id].event_table[event].recovery_check_count++;
                   }
                }
             }
           }
          break;
          case GPL1_UL_RELEASE_EVENT:
          case GPL1_DL_RELEASE_EVENT:
          {
            MSG_GERAN_HIGH_2_G("REC:gpl1_gprs_check_for_recovery event=%d recovery_required=%d",event,recovery_required);
            if(TRUE == GPl1_recovery_data[gas_id].event_table[event].valid) 
            {
               recovery_fn =ADD_FN(GPl1_recovery_data[gas_id].event_table[event].registered_fn,GPL1_FRAME_COUNT_FOR_RECOVERY_100);
               if(IS_FRAME_NUM_LATER(current_FN,recovery_fn))
               {
                   if(GPl1_recovery_data[gas_id].event_table[event].recovery_check_count==GPL1_WAIT_COUNT_TRIGGER_RECOVERY)
                   {
                        recovery_required=TRUE;
                        MSG_GERAN_HIGH_2_G("REC:gpl1_gprs_check_for_recovery event=%d recovery_required=%d",event,recovery_required);
                   }
                   else
                   {
                        GPl1_recovery_data[gas_id].event_table[event].recovery_check_count++;
                   }
                }
             }
           }
           break;
           default:
#ifdef FEATURE_QSH_MDUMP
                QSH_MDUMP_SET(QSH_MDUMP_CRASH_CAT_UNKNOWN,QSH_CLT_GL1,QSH_CLT_GL1)
#endif

                ERR_GERAN_FATAL_0_G("Invalid event");
           break;
       }
    }
    /*checks state mismatch between Gl1 and GMAC and returns True if theres is change in state which indirectly indicates some thing went wrong between GL1 and GMAC*/
   if((!recovery_required) && (TRUE == gpl1_gprs_recovery_l1_mac_state_check(gas_id)))
   {
      if(GPl1_recovery_data[gas_id].state_mismatch_count == GPL1_WAIT_COUNT_TRIGGER_RECOVERY_STATE_MISMATCH)
      {
         recovery_required =TRUE;
         MSG_GERAN_HIGH_1_G("REC:gpl1_gprs_check_for_recovery state_mismatch recovery_required=%d",recovery_required);
      }
      else
      {
         GPl1_recovery_data[gas_id].state_mismatch_count++;
      }
   }
   else
   {
      GPl1_recovery_data[gas_id].state_mismatch_count = 0;
   }

 return recovery_required;

}

/*===========================================================================

FUNCTION gpl1_gprs_recovery_register_event

DESCRIPTION
             Register the transfer mode events for which error recovery is handled.

PARAMS       gas_id - geran access stratum id.

DEPENDENCIES

RETURN VALUE None


===========================================================================*/

void gpl1_gprs_recovery_register_event(gpl1_recovery_events_t event_type , uint32 start_time, gas_id_t gas_id)
{
  uint32 registered_fn = gl1_get_FN(gas_id);
  volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = NULL;
  l1_tskisr_blk = &l1_tsk_buffer[gas_id];
  GL1_ISR_LOCK(gas_id);
  if((l1_tskisr_blk->l1_state != L1_DEDICATED_MODE) && (l1_tskisr_blk->l1_state != L1_DTM_MODE))
   {
        MSG_GERAN_HIGH_3_G("REC:gpl1_gprs_recovery_register_event event_type=%d,start_time=%d,registered_fn=%d",event_type,start_time,registered_fn);
        GPl1_recovery_data[gas_id].event_table[event_type].starting_time= start_time; 
        GPl1_recovery_data[gas_id].event_table[event_type].registered_fn= registered_fn; 
        GPl1_recovery_data[gas_id].event_table[event_type].valid = TRUE; 
  
        if(event_type == GPL1_UL_RELEASE_EVENT && GPl1_recovery_data[gas_id].event_table[GPL1_UL_CONNECT_EVENT].valid)
        {
           gpl1_gprs_recovery_deregister_event(GPL1_UL_CONNECT_EVENT, gas_id);
        }  
       
        if(event_type == GPL1_DL_RELEASE_EVENT && GPl1_recovery_data[gas_id].event_table[GPL1_DL_CONNECT_EVENT].valid)
        {
           gpl1_gprs_recovery_deregister_event(GPL1_DL_CONNECT_EVENT, gas_id);
         }
    }
  else
    {
        MSG_GERAN_HIGH_0_G("REC:gpl1_gprs_recovery_register_event Ignore event registartion");
    }
  GL1_ISR_UNLOCK(gas_id);
}
/*===========================================================================

FUNCTION gpl1_gprs_recovery_deregister_event

DESCRIPTION
             Deregister the transfer mode events which registered from gpl1_gprs_err_recovery_register_event.

PARAMS       gas_id - geran access stratum id.

DEPENDENCIES

RETURN VALUE None

===========================================================================*/

void gpl1_gprs_recovery_deregister_event(gpl1_recovery_events_t event_type,gas_id_t gas_id)
{
    volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = NULL;
    l1_tskisr_blk = &l1_tsk_buffer[gas_id];
    GL1_ISR_LOCK(gas_id);
    if((l1_tskisr_blk->l1_state != L1_DEDICATED_MODE) && (l1_tskisr_blk->l1_state != L1_DTM_MODE))
    {
        MSG_GERAN_HIGH_1_G("REC:gpl1_gprs_recovery_deregister_event event_type=%d",event_type);
        GPl1_recovery_data[gas_id].event_table[event_type].valid = FALSE; 
        GPl1_recovery_data[gas_id].event_table[event_type].starting_time = INVALID_FN; 
        GPl1_recovery_data[gas_id].event_table[event_type].registered_fn = INVALID_FN; 
        GPl1_recovery_data[gas_id].event_table[event_type].recovery_check_count = 0;
     }
   else
    {
        MSG_GERAN_HIGH_0_G("REC:gpl1_gprs_recovery_register_event Ignore event registartion");
    }
    GL1_ISR_UNLOCK(gas_id);
}

/*===========================================================================
FUNCTION gpl1_gprs_set_recovery_cnf_pending

DESCRIPTION
             Gl1 set recovery cnf pending .

PARAMS       gas_id - geran access stratum id.
             Boolean    pending value.

DEPENDENCIES

RETURN VALUE None

===========================================================================*/

void gpl1_gprs_set_recovery_cnf_pending(boolean pending,gas_id_t  gas_id)
{
    GL1_ISR_LOCK(gas_id);
    GPl1_recovery_data[gas_id].recovery_cnf_pending = pending;
    GL1_ISR_UNLOCK(gas_id);
}

/*===========================================================================
FUNCTION gpl1_gprs_get_recovery_cnf_pending

DESCRIPTION
             Gl1 get recovery cnf pending .

PARAMS       gas_id - geran access stratum id.

DEPENDENCIES

RETURN VALUE  substate

===========================================================================*/

boolean gpl1_gprs_get_recovery_cnf_pending(gas_id_t  gas_id)
{
    boolean recovery_cnf;
    GL1_ISR_LOCK(gas_id);
    recovery_cnf = GPl1_recovery_data[gas_id].recovery_cnf_pending;
    GL1_ISR_UNLOCK(gas_id);
    return recovery_cnf;
}


/*===========================================================================
FUNCTION gpl1_gprs_set_recovery_state

DESCRIPTION
             Gl1 set recovery sub state .

PARAMS       gas_id - geran access stratum id.
             state    recovery state to be assigned.

DEPENDENCIES

RETURN VALUE None

===========================================================================*/

void gpl1_gprs_set_recovery_state(gpl1_recovery_substate  state,gas_id_t  gas_id)
{
    GL1_ISR_LOCK(gas_id);
    GPl1_recovery_data[gas_id].sub_state = state;
    GL1_ISR_UNLOCK(gas_id);
}

/*===========================================================================
FUNCTION gpl1_gprs_get_recovery_sub_state

DESCRIPTION
             Gl1 get recovery sub state .

PARAMS       gas_id - geran access stratum id.

DEPENDENCIES

RETURN VALUE  substate

===========================================================================*/

gpl1_recovery_substate gpl1_gprs_get_recovery_sub_state(gas_id_t  gas_id)
{
     gpl1_recovery_substate sub_state;
     GL1_ISR_LOCK(gas_id);
     sub_state = GPl1_recovery_data[gas_id].sub_state;
     GL1_ISR_UNLOCK(gas_id);
     return sub_state;

}

/*===========================================================================

FUNCTION gpl1_gprs_recovery_reset

DESCRIPTION
             Reset varaibles used during error recovery..

PARAMS       gas_id - geran access stratum id.

DEPENDENCIES

RETURN VALUE None


===========================================================================*/

void gpl1_gprs_recovery_reset(gas_id_t gas_id)
{
    uint8 i;
    gpl1_gprs_set_recovery_state(GPL1_RECOVERY_NULL_STATE,gas_id);
    GPl1_recovery_data[gas_id].state_mismatch_count = 0;
    for(i=0;i<GPL1_MAX_EVENT;i++)
    {
        GPl1_recovery_data[gas_id].event_table[i].starting_time = INVALID_FN; 
        GPl1_recovery_data[gas_id].event_table[i].registered_fn = INVALID_FN; 
        GPl1_recovery_data[gas_id].event_table[i].valid = FALSE;
        GPl1_recovery_data[gas_id].event_table[i].recovery_check_count = 0;
    }
    MSG_GERAN_HIGH_0_G("REC:gpl1_gprs_recovery_reset");  
}

/*===========================================================================

FUNCTION gpl1_gprs_handle_recovery_req

DESCRIPTION
            Handle error recovery Req .

PARAMS       gas_id - geran access stratum id.

DEPENDENCIES

RETURN VALUE None


===========================================================================*/


void gpl1_gprs_handle_recovery_req(gas_id_t gas_id)
{
   volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
   GL1_ISR_LOCK(gas_id);
   l1_transfer_data[gas_id].wait_for_pca_valid = FALSE;
   l1_transfer_data[gas_id].pending_alloc_type = L1_NULL_ALLOC;
   l1_tskisr_blk->main_command = L1_NULL_COMMAND;
   /* set release type for release confirm which is sent we
     get back to IDLE mode      */
   l1_transfer_data[gas_id].release_pending = L1_NO_RELEASE;
   /* Set starting time valid to false as we need to release ASAP */
   l1_transfer_data[gas_id].starting_time_valid = FALSE;
   /* set starting time elapsed to true as we do not want the MAC_PH_CONNECT_IND
   to be sent when we restart transfer_mode      */
   l1_transfer_data[gas_id].starting_time_elapsed  = TRUE;
   l1_transfer_data[gas_id].confirm_pending = SENT;
#ifdef FEATURE_GSM_DTM
   /* We don't want to kill the SCE on transitions back to dedicated */
   if(l1_tskisr_blk->l1_state != L1_DTM_MODE)
#endif
   {
     l1_sc_stop(gas_id);
   }
   gpl1_gprs_setup_start_idle((void *)0, gas_id);
   GL1_ISR_UNLOCK(gas_id);
}

/*===========================================================================
FUNCTION gpl1_gprs_recovery_tick

DESCRIPTION
             Called in every ISR in transfer mode . This function Handles error recovery based on the states .

PARAMS       gas_id - geran access stratum id.

DEPENDENCIES

RETURN VALUE None

===========================================================================*/



void gpl1_gprs_recovery_tick(gas_id_t gas_id)
{

uint32 current_fn=GSTMR_GET_FN_GERAN(gas_id);

      switch(GPl1_recovery_data[gas_id].sub_state)
      {
            case GPL1_RECOVERY_NULL_STATE:
                   GPl1_recovery_data[gas_id].sub_state ++;
            case GPL1_RECOVERY_CHECK_STATE:
                   if(( current_fn % GPL1_FRAME_COUNT_FOR_RECOVERY_CHECK == 0)&& 
                        (TRUE == gpl1_gprs_check_for_recovery(gas_id))) 
                   { 
                      gpl1_gprs_recovery_send_l1_mac_recovery_ind(gas_id);
                      gpl1_gprs_set_recovery_state(GPL1_RECOVERY_IND_SENT_STATE,gas_id);
                   } 
                 break;
            case GPL1_RECOVERY_IND_SENT_STATE:
                 break;
            case GPL1_RECOVERY_STARTED_STATE:
                 break;
            default:
#ifdef FEATURE_QSH_MDUMP
                   QSH_MDUMP_SET(QSH_MDUMP_CRASH_CAT_UNKNOWN,QSH_CLT_GL1,QSH_CLT_GL1)
#endif
                   ERR_GERAN_FATAL_0_G("Invalid Recovery state");
                 break;
        }
}



/*===========================================================================

FUNCTION gpl1_gprs_register_release_recovery_event

DESCRIPTION :.Register recovery release events based on release tbf type 

PARAMS       gas_id - geran access stratum id.
             Release TBF type
DEPENDENCIES

RETURN VALUE None


===========================================================================*/


void gpl1_gprs_register_release_recovery_event(release_tbf_t release_type,gas_id_t  gas_id)
{

    switch (release_type)
    {
      case  UL_TBF_NORMAL:
      case  UL_TBF_ABNORMAL:
        {
            gpl1_gprs_recovery_register_event(GPL1_UL_RELEASE_EVENT,INVALID_FN, gas_id);
            break;
        }

      case DL_TBF_NORMAL:
      case DL_TBF_ABNORMAL:
        {
            gpl1_gprs_recovery_register_event(GPL1_DL_RELEASE_EVENT,INVALID_FN, gas_id);
            break;
        }

      case UL_AND_DL_TBF_NORMAL:
      case UL_AND_DL_TBF_ABNORMAL:
        {
            gpl1_gprs_recovery_register_event(GPL1_UL_RELEASE_EVENT,INVALID_FN, gas_id);
            gpl1_gprs_recovery_register_event(GPL1_DL_RELEASE_EVENT,INVALID_FN, gas_id);
            break;
        }
       default:
#ifdef FEATURE_QSH_MDUMP
           QSH_MDUMP_SET(QSH_MDUMP_CRASH_CAT_UNKNOWN,QSH_CLT_GL1,QSH_CLT_GL1)
#endif
           ERR_GERAN_FATAL_0_G("Invalid release type");
        break;

      }

}


/*===========================================================================

FUNCTION      gl1_gprs_recovery_read_nv_data

DESCRIPTION   Read the NV from EFS

RETURN VALUE

SIDE EFFECTS

===========================================================================*/
void gl1_gprs_recovery_read_nv_data(gas_id_t  gas_id)
{
  int efs_error =geran_efs_read_primary( GL1_EFS_PKT_RECOVERY,&gl1_gprs_pkt_rec_support,sizeof( gl1_gprs_pkt_rec_support ) );
  if (efs_error == -1)
    {
       gl1_gprs_pkt_rec_support = 0;
    }
  
   MSG_GERAN_HIGH_1_G("REC:Pkt recovery NV is %d",gl1_gprs_pkt_rec_support);
} // gl1_gprs_recovery_read_nv_data

/*===========================================================================

FUNCTION      gl1_gprs_rec_efs_enabled

DESCRIPTION   return stored recovery efs value

RETURN Boolean

SIDE EFFECTS

===========================================================================*/
uint8 gl1_gprs_rec_efs_enabled(gas_id_t  gas_id)
{
  return gl1_gprs_pkt_rec_support;
} // gl1_gprs_rec_efs_enabled


#endif /*FEATURE_GERAN_PTM_ERR_RECOVERY*/

