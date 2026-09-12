/*===========================================================================
            COPYRIGHT INFORMATION

Copyright (c) 2001,2002 Qualcomm Technologies, Incorporated and its licensors.  All Rights
Reserved.  QUALCOMM Proprietary.  Export of this technology or software
is regulated by the U.S. Government. Diversion contrary to U.S. law prohibited.

            EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$PVCSPath:  L:/src/asw/MSM5200/mm/vcs/mmtimers.c_v   1.14   03 Jul 2002 15:10:38   jault  $
$Header: //components/rel/mmcp.mpss/7.9.0/nas/mm/src/mmtimers.c#1 $    $DateTime: 2021/06/15 22:40:27 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
05/14/01   jca     Added support for GMM timers.
06/03/01   sbk     Global variables are renamed to use mm_ prefix
06/20/01   sbk     WINNT builds has longer timeouts
09/21/01   sbk     MM timers are set to normal values unless FEATURE_TIMER_DEBUG is defined
02/12/02   jca     Removed TIMER_RR_ABORT.
04/30/02   ks      Added support for TIMER_DETACH_REL in mm_get_timeout.
07/01/02   jca     Deleted function mm_query_timer().
07/03/02   jca     Deleted timers TIMER_PLMN_SELECT and TIMER_PLMN_LIST.
08/09/02   jca     Deleted functions mm_pause_timer() and mm_resume_timer().
09/23/02   mks     Updated the default timeout for T3213 timer
07/16/03   jca     No need to call gs_cancel_timer() if timer_status == TIMER_STOPPED.
12/09/03   ks      Added check for Timer_id validation in mm_start_timer() and
                   mm_stop_timer() functions.
04/06/04   cd      LCS support
04/19/04   vdr     Fixed compilation warnings
08/12/04   cd      Replaced FEATURE_LCS by FEATURE_MMGPS
12/17/04   vdr     Added default timer values for TIMER_T3218 & TIMER_T3316
07/14/06   pk     Updated for Feature DSAC
06/30/11   rajesh    Changes made for implementing PS detach timer in NAS.

===========================================================================*/


/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "mmcp_variation.h"
#include "customer.h"
#include "rex.h"
#include "err.h"
#include "msg.h"
#include "sys_type.h"
#include "sys_cnst_v.h"
#include "sys_stru_v.h"
#include "timers_v.h"
#include "gs_v.h"
#include "mm_v.h"

#ifdef FEATURE_LTE
#include "event.h"
#include "naslog_v.h"
#include "event_defs.h"
#include "emm_security.h"
#include "lte_rrc_ext_api.h"
#endif

#ifdef FEATURE_NAS_ECALL
#ifdef FEATURE_DUAL_SIM
extern  uint32 mm_ecall_inactivity_timer_sim[MAX_NAS_STACKS];
#define mm_ecall_inactivity_timer mm_ecall_inactivity_timer_sim[mm_as_id]

extern  boolean mm_ecall_profile_sim[MAX_NAS_STACKS];
#define mm_ecall_profile mm_ecall_profile_sim[mm_as_id]
#else
extern uint32  mm_ecall_inactivity_timer;
extern boolean mm_ecall_profile;
#endif 

#endif

#ifdef FEATURE_DUAL_SIM
extern uint8  mm_emm_t3247_test_val_sim[MAX_AS_IDS];
#define mm_emm_t3247_test_val mm_emm_t3247_test_val_sim[mm_sub_id]
extern dword  mm_gsm_sel_resel_block_timer_sim[MAX_AS_IDS];
#define mm_gsm_sel_resel_block_timer mm_gsm_sel_resel_block_timer_sim[mm_sub_id]
#else
extern uint8  mm_emm_t3247_test_val;
extern dword  mm_gsm_sel_resel_block_timer;
#endif

/*===========================================================================

FUNCTION MM_TIMER_GET_RANDOM_T3247_VALUE

DESCRIPTION
  This function generates a random timeout value for T3247 Timer

DEPENDENCIES
  None

RETURN VALUE
  T3247 Timer Value

SIDE EFFECTS
  None

===========================================================================*/
dword mm_timer_get_random_t3247_value(void)
{
  dword ran_temp, t3247_timer;

  /* T3247 can have a random value between 30 to 60 minutes */
  mm_secapi_get_random( (uint8 *)&ran_temp, sizeof(dword) );
      
  t3247_timer = (30 + ((int32)(ran_temp % 30)))* 60 * 1000;

  return t3247_timer;
}

/*===========================================================================

FUNCTION MM_GET_TIMEOUT

DESCRIPTION
  This function gets the timeout value for the corresponding timer

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

 dword mm_get_timeout( timer_id_T timer_id )
 {
   dword    return_value = 0 ;

   switch ( timer_id )
   {
      case TIMER_MM_FORBIDDEN_LAI_CLEAR:
         return_value = 43200000;
         break;

      case TIMER_T3210:
         return_value = 20000;
         break;

      case TIMER_T3211:
         return_value = 15000;
         break;

      case TIMER_T3213:
         /* Per section 4.4.4.9 of TS24.008, if at the expiry of timer T3213 a
            new cell has not been selected, the mobile station may as an option
            delay the repeated LU attempt for up to 8 seconds to allow cell
            re-selection to take place. However, if optional wait is implemented, 
            GCF 26.7.4.3.1 fails as it allows wait for 4 s to 9 s only.
          */
#ifdef FEATURE_T3213_OPTIONAL_WAIT          
         return_value = 4000 + 8000;
#else
         return_value = 4000;
#endif   
         break;

      case TIMER_T3214:
         return_value = 20000;
         break;

      case TIMER_T3216:
         return_value = 15000;
         break;

      case TIMER_T3220:
         return_value = 5000;
         break;

      case TIMER_T3230:
         return_value = 15000;
         break;
      case TIMER_SCRI_PAGING:
      case TIMER_T3240:
         return_value = 10000;
         break;

      case TIMER_CSMT_GUARD:
         return_value = 4000;
         break;


#if defined ( FEATURE_MMGPS ) || defined ( FEATURE_CGPS_UMTS_CP )
      case TIMER_T3241:
         return_value = 300000;
         break;
#endif /* end of (FEATURE_MMGPS || FEATURE_CGPS_UMTS_CP) */
#ifdef FEATURE_NAS_ECALL
      case TIMER_T3242:
         if(mm_ecall_profile == FALSE)
         {
           /* for Non-eCall profile, defualt value is 12hrs 3GPP*/
         return_value = 43200000;
         }
         else
         {
           /* eCall Profile, ERA-GLONASS, configurable timer */
           return_value = mm_ecall_inactivity_timer *60*1000;
         }
         break;
      case TIMER_T3243:
         if(mm_ecall_profile == FALSE)
         {
           /* for Non-eCall profile, defualt value is 12hrs 3GPP*/
         return_value = 43200000;
         }
         else
         {
           /* eCall Profile, ERA-GLONASS, configurable timer */
           return_value = mm_ecall_inactivity_timer *60*1000;
         }
         break;
#endif
      case TIMER_MM_REL:
         return_value = 60000;
         break;

      case TIMER_T3302:
         /* ----------------------------
         ** Default timeout = 12 minutes
         ** ---------------------------- */
         return_value = 720000;
         break;

      case TIMER_T3310:
         return_value = 15000;
         break;

      case TIMER_T3311:
         return_value = 15000;
         break;

      case TIMER_T3312:
         /* ----------------------------
         ** Default timeout = 54 minutes
         ** ---------------------------- */
         return_value = 3240000;
         break;

      case TIMER_T3314:
         return_value = 44000;
         break;

      case TIMER_T3317:
         return_value = 15000;
         break;

#ifdef FEATURE_NAS_REL12_SPEC_CHANGE

      /* Spec 24.008, Spec CRs 2488, 2937 
       * If the service request attempt counter is greater than or equal to 5, 
       * the MS shall not attempt service request for TIMER_T3325 unless it 
       * is in response to paging from the network.
       */

      case TIMER_T3325:
         return_value = mm_emm_service_req_backoff_timer_value;
         break; 
#endif

      case TIMER_T3318:
         return_value = 20000;
         break;

      case TIMER_T3319:
         return_value = 30000;
         break;

      case TIMER_T3320:
         return_value = 15000;
         break;

      case TIMER_T3321:
         return_value = 15000;
         break;

      case TIMER_T3330:
         return_value = 15000;
         break;

      case TIMER_POWERDOWN:
         return_value = 5000;
         break;

      case TIMER_DETACH_REL:
         return_value = 10000;
         break;

      case TIMER_T3218 :
        return_value = 20000 ;
        break ;

      case TIMER_T3316 :
        return_value = 30000 ;
        break ;
      case TIMER_T3350:
        /* Default value is 30sec.
        keeping buffer and making it 36S */
        return_value = 36000;
        break;   
#ifdef FEATURE_DUAL_SIM
      case TIMER_MM_RADIO_RETRY :
        return_value = 2000 ;
        break; 
      case TIMER_GMM_RADIO_RETRY :
        return_value = 2000 ;
        break ;
	  case  TIMER_MM_DUAL_SWITCH:
	  	return_value = 15000;
        break ;		
#endif
      case TIMER_FRLTE :
      case TIMER_MM_PAGE_RETRY :
          return_value = 24000;
          break;
      case TIMER_T3340:
      case TIMER_PS_CONN_REL:
        return_value = 10000;
        break;

      case TIMER_FORBID_TDS_RESELECTION:
          return_value = 10000;
          break;

#ifdef FEATURE_LTE
      case TIMER_T3417 :
        return_value = 5000;
        break ;
      case TIMER_T3421 :
        return_value = 15000;
        break ;
      case TIMER_T3430 :
        return_value = 15000;
        break;
      case TIMER_T3418:
        return_value = 20000;
        break;
      case TIMER_T3416:
        return_value = 30000;
        break;
      case TIMER_T3411 :
        return_value  = 10000;
        break;
      case TIMER_T3402 :
        return_value = 720000;
        break;
      case TIMER_T3410 :
        return_value = 15000;
        break;
      case TIMER_T3440 :
        return_value = 10000;
        break;
      case EMM_POWEROFF_DETACH_TIMER:
        return_value = 5000; 
        break;

      case TIMER_T3420:
        return_value = 15000;
        break;  

      case TIMER_T3323:
        return_value = 3240000;
        break;

#endif /*FEATURE_LTE*/

      case TIMER_MM_PS_DETACH:
        return_value = 78000;
        break;

#ifdef FEATURE_FEMTO_CSG
       case TIMER_FORBIDDEN_OPERATOR_CSG_LIST:
         return_value = 3600000;
         break;
#endif 

#if defined FEATURE_DUAL_SIM || defined FEATURE_1XSRLTE
       case TIMER_EMM_RADIO_RETRY:
        return_value = 2000 ;
        break;
#endif 

       case TIMER_POLICY_CHANGE_PENDING:
         return_value = 1000 ;
         break;

#ifdef FEATURE_DUAL_SIM
       case TIMER_TDS_RAT_DEACTIVATION:
         return_value = 1000 ;
         break;
 
       case EMM_MT_PAGE_EXT_PRI_TIMER:
         if(MM_IS_NON_DDS_SUB == FALSE)
         {
         /*For DDS start timer for 1 sec */
           return_value = 1000;
		 }
		 else
		 {
           return_value = 300;
		 }
         break;
#endif

#ifdef FEATURE_CRAT_MSIM_TEST_FRAMEWORK
      case TIMER_ELS_CONN_REL:
         return_value = 500;
         break;
#endif

      case TIMER_HOLD_TRM_LOCAL_REL:
         return_value = 100;
         break;
#ifdef FEATURE_DUAL_DATA
      case TIMER_ESR_CONN_REL:
        return_value = (dword)lte_rrc_get_hold_timer_1st_tick_value((sys_modem_as_id_e_type)mm_as_id) + 200;
        break;
#endif
    case TIMER_LOCAL_REL_MSEARCH:
        return_value = 16000;
        break;
   case TIMER_T3247:
        if (mm_emm_t3247_test_val > 0)
        {
           return_value =mm_emm_t3247_test_val * 60 * 1000;
           MSG_HIGH_DS_2( MM_SUB, "=MM= Test TIMER_T3247 - %u:%u",
               (word) ( return_value /1000L), (word)( return_value % 1000L) );
        }
        else
		{
		   return_value = mm_timer_get_random_t3247_value();
		   MSG_HIGH_DS_2( MM_SUB, "=MM= Random timeout for TIMER_T3247 - %u:%u",
               (word) ( return_value /1000L), (word)( return_value % 1000L) );
		}
        break;


  case GSM_EXTENDED_BLOCK_TIMER:
        return_value = mm_gsm_sel_resel_block_timer * 60 * 1000; // have checks in mmglbl
       break;

     case TIMER_PROHIBIT_DS_DRX:
        return_value = 3600000;
        break;
#ifdef FEATURE_HA_MAINLINE_ONLY_ENABLE
      case TIMER_DELAY_TAU:
        return_value = 1000;
        break;
#endif		
      default:
         MSG_FATAL_DS(MM_SUB, "=MM= Unexpected timer id: %d, setting timout to 0", timer_id,0,0);
   }

#ifdef FEATURE_TIMER_DEBUG
   #error code not present
#else
   return return_value;
#endif
}

/*===========================================================================

FUNCTION MM_TIMER_NON_DEFERRABLE

DESCRIPTION
  This function takes the timer id as input and returns TRUE if timer should
  be non-deferrable

DEPENDENCIES
  None

RETURN VALUE
  TRUE: Timer is non-deferrable
  FALSE: Timer is deferrable

SIDE EFFECTS
  None

===========================================================================*/

boolean mm_timer_non_deferrable( timer_id_T timer_id )
{
  switch(timer_id)
  {
    case TIMER_T3211:
    case TIMER_T3212:
    case TIMER_T3246:
    case TIMER_T3302:
    case TIMER_T3311:
    case TIMER_T3312:
    case TIMER_T3323:
    case TIMER_T3346:
    case TIMER_T3402:
    case TIMER_T3411:
    case TIMER_T3412:
    case TIMER_T3423:
    case TIMER_T3442:
    case TIMER_SRLTE_ESR:
    case TIMER_MM_REL:
    case TIMER_MM_PS_DETACH:
    case TIMER_POWERDOWN:
    case EMM_POWEROFF_DETACH_TIMER:
    case TIMER_EMM_PS_DETACH:
      return TRUE;
      break;

    default:
      return FALSE;
  }
}

/*===========================================================================

FUNCTION MM_START_TIMER

DESCRIPTION
  This function starts the mm timer specified by timer_id

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void mm_start_timer( timer_id_T timer_id, dword num_millisecs )
{
   gs_status_T    status = GS_FAILURE;
#ifdef FEATURE_LTE
   log_lte_nas_emm_timer_event_type event;
#endif

   /* Check if the Timer Id is in the valid Range of MM timers */
   if ((timer_id < MIN_MM_TIMER) || (timer_id > MAX_MM_TIMER))
   {
     MSG_FATAL_DS(MM_SUB, "=MM= Unknown MM timer ID to start : %d",timer_id,0,0);
   }
   
   if (num_millisecs == 0)
   {
     MSG_HIGH_DS_1(MM_SUB ,"=MM= Cannot Start Timer %d with zero time out value",timer_id);
     return;
   }
   
   /*
    * if using default timeout, look up the value now
    */

   if ( num_millisecs == DEFAULT_TIMEOUT )
   {
      num_millisecs = mm_get_timeout( timer_id );
   }

#if defined FEATURE_DUAL_SIM || defined FEATURE_SGLTE 


   mm_timer_status[ timer_id ] = TIMER_ACTIVE;

   MSG_HIGH_DS_3( MM_SUB, "=MM= Start %d, timeout %u:%u",
             timer_id, (word) (num_millisecs /1000L), (word)(num_millisecs % 1000L) );

   //timer_id = MM_TIMER_TAG_AS_ID(timer_id);

   if(timer_id > MAXNO_TIMERS)
   {
     MSG_FATAL_DS(MM_SUB, "=MM= Timer ID + SIM ID exceeds max value timer_id=%d AS_ID=%d", timer_id,mm_as_id+1,0);
   }

   if(mm_as_id == (mm_as_id_e_type)SYS_MODEM_AS_ID_1)
   {
     status = gs_start_timer_ext( GS_QUEUE_MM,
                                  timer_id,
                                  mm_timer_expiry,
                                  num_millisecs,
                                  (byte) 0,
                                  mm_timer_non_deferrable(timer_id));
   }
   else if(mm_as_id == (mm_as_id_e_type)SYS_MODEM_AS_ID_2)
   {
     status = gs_start_timer_ext( GS_QUEUE_MM_SUB1,
                                  timer_id,
                                  mm_timer_expiry_sub1,
                                  num_millisecs,
                                  (byte) 0,
                                  mm_timer_non_deferrable(timer_id));
   }
#if defined(FEATURE_TRIPLE_SIM) || defined(FEATURE_SGLTE_DUAL_SIM)
   else if(mm_as_id == (mm_as_id_e_type)SYS_MODEM_AS_ID_3)
   {
     status = gs_start_timer_ext( GS_QUEUE_MM_SUB2,
                                  timer_id,
                                  mm_timer_expiry_sub2,
                                  num_millisecs,
                                  (byte) 0,
                                  mm_timer_non_deferrable(timer_id));
   }
#endif

#else

   status = gs_start_timer_ext( GS_QUEUE_MM,
                                timer_id,
                                mm_timer_expiry,
                                num_millisecs,
                                (byte) 0,
                                mm_timer_non_deferrable(timer_id));

   mm_timer_status[ timer_id ] = TIMER_ACTIVE;

   MSG_HIGH_DS_3( MM_SUB, "=MM= Start %d, timeout %u:%u",
             timer_id, (word) (num_millisecs /1000L), (word)(num_millisecs % 1000L) );
#endif

   if ( status != GS_SUCCESS )
   {
      MSG_ERROR_DS(MM_SUB, "=MM= Start timer failure: Status %d", status,0,0);
   }
#ifdef FEATURE_LTE
   else if(status == GS_SUCCESS)
   {
     event.emm_timer_event = (byte)timer_id;
     /* log the event for ConnectionRequest being sent */
     nas_event_report_payload(EVENT_LTE_EMM_TIMER_START,sizeof(event), (void*)&event,mm_message_as_id);
   }
#endif

   if(timer_id == TIMER_T3346)
   {
     mm_send_mmr_t3346_timer_ind(SYS_NAS_T3346_STATUS_START);
   }
}


/*===========================================================================

FUNCTION MM_STOP_TIMER

DESCRIPTION
  This function stops the mm timer specified by timer_id


DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void mm_stop_timer( timer_id_T timer_id )
{
   gs_status_T gs_status = GS_FAILURE;
#ifdef FEATURE_LTE
   log_lte_nas_emm_timer_event_type event;
   emm_ctrl_data_type *emm_ctrl_data_ptr = NULL;
   emm_ctrl_data_ptr = emm_db_get_ctrl_data();
#endif
   /* Check if the Timer Id is in the valid Range of MM timers */
   if ((timer_id < MIN_MM_TIMER) || (timer_id > MAX_MM_TIMER))
   {
     MSG_FATAL_DS(MM_SUB, "=MM= Unknown MM timer ID to stop : %d",timer_id,0,0);
   }
#ifdef FEATURE_LTE
   if(timer_id == TIMER_T3402 && mm_timer_status[TIMER_T3402] != TIMER_STOPPED)
   {
	/* Reset TAU attempt counter */
	emm_ctrl_data_ptr->emm_tau_attempt_counter = 0;
	emm_ctrl_data_ptr->emm_attach_attempt_counter =0;
	MSG_HIGH_DS_0(MM_SUB, "=EMM= Reset TAU attempt counter and Attach Counter");
   }
#endif
   if (mm_timer_status[timer_id] != TIMER_STOPPED)
   {
#if defined FEATURE_DUAL_SIM || defined FEATURE_SGLTE 
      MSG_HIGH_DS_1(MM_SUB ,"=MM= Stopping %d", timer_id);

      mm_timer_status[timer_id] = TIMER_STOPPED;

      //timer_id = MM_TIMER_TAG_AS_ID(timer_id);

      if(timer_id > MAXNO_TIMERS)
      {
        MSG_FATAL_DS(MM_SUB, "=MM= Timer ID + SIM ID exceeds max value timer_id=%d mm_as_id=%d", timer_id,mm_as_id+1,0);
      }

      gs_status = gs_cancel_timer_ext( (byte)get_mm_timer_queue(),
                                       timer_id,
                                       mm_timer_non_deferrable(timer_id));

      if (gs_status != GS_SUCCESS)
      {
         MSG_ERROR_DS(MM_SUB, "=MM= Stop timer failure", 0,0,0);
      }
 
#else
      MSG_HIGH_DS_1(MM_SUB ,"=MM= Stopping %d", timer_id);

      gs_status = gs_cancel_timer_ext( GS_QUEUE_MM,
                                       timer_id,
                                       mm_timer_non_deferrable(timer_id));

      if (gs_status != GS_SUCCESS)
      {
         MSG_ERROR_DS(MM_SUB,"=MM= Stop timer failure", 0,0,0);
      }
      mm_timer_status[timer_id] = TIMER_STOPPED;
#endif
#ifdef FEATURE_LTE
      if(gs_status == GS_SUCCESS)
      {
        event.emm_timer_event = (byte)timer_id;
        /* log the event for ConnectionRequest being sent */
        nas_event_report_payload(EVENT_LTE_EMM_TIMER_STOP,sizeof(event),(void*)&event,mm_message_as_id);
      } 
#endif
      if(timer_id == TIMER_T3346)
      {
        mm_send_mmr_t3346_timer_ind(SYS_NAS_T3346_STATUS_STOP);
#ifdef FEATURE_LTE
        emm_ctrl_data_ptr->t3346_started_emc_call_pending = FALSE;
#endif
      }
   }

#ifdef FEATURE_LTE
   /* Deactivate Attach(T3410), TAU(T3430), Service Request(T3417) or Detach(T3421) timer in
      Auth Server if it was running and stopped during Authentication failure to prevent the
      EMM procedure timer to be restarted upon Authentication timer (MAC/SQN)expiry. It is
      only applicable when emergency session is active. */

   if((emm_ctrl_data_ptr != NULL) &&
      (emm_ctrl_data_ptr->emc_srv_status != LTE_NAS_EMC_SRV_NONE) &&
      ((timer_id == TIMER_T3410) ||
       (timer_id == TIMER_T3417) ||
       (timer_id == TIMER_T3421) ||
       (timer_id == TIMER_T3430)))
   {
     emm_deact_retx_timer(timer_id);
   }
#endif
#ifdef FEATURE_NAS_ECALL
    if(timer_id == TIMER_T3242)
    {
      mm_timer_expiry_info.timer_bitmask &= ~SYS_MM_TIMER_T3242_INFO_MASK;
    }

    if(timer_id == TIMER_T3243)
    {
      mm_timer_expiry_info.timer_bitmask &= ~SYS_MM_TIMER_T3243_INFO_MASK;
    }
#endif
}

#ifdef FEATURE_DUAL_SIM

void mm_stop_timer_per_subs(timer_id_T timer_id_local, mm_as_id_e_type mm_local_as_id) 
{
    gs_status_T gs_status = GS_FAILURE;
    byte gs_queue= GS_QUEUE_MM;
#if defined FEATURE_DUAL_SIM || defined FEATURE_SGLTE 
	  if(mm_local_as_id == (mm_as_id_e_type)SYS_MODEM_AS_ID_1)
	  
	  {
		gs_queue = GS_QUEUE_MM;
	  }
	  else if(mm_local_as_id == (mm_as_id_e_type)SYS_MODEM_AS_ID_2)
	  {
		gs_queue = GS_QUEUE_MM_SUB1;
	  }
#if defined(FEATURE_TRIPLE_SIM) || defined(FEATURE_SGLTE_DUAL_SIM)
	  else if(mm_local_as_id == (mm_as_id_e_type)SYS_MODEM_AS_ID_3)
	  {
		gs_queue = GS_QUEUE_MM_SUB2;
	  }
#endif
	  else
	  {
		gs_queue = GS_QUEUE_MM_SUB1;
	  }
#else
	  gs_queue = GS_QUEUE_MM;
#endif
     if (mm_timer_status_sim[mm_local_as_id][timer_id_local] != TIMER_STOPPED)
     {
       MSG_HIGH_DS_1(MM_SUB ,"=MM= Stopping %d", timer_id_local);
       mm_timer_status_sim[mm_local_as_id][timer_id_local] = TIMER_STOPPED;
     }
      gs_status = gs_cancel_timer_ext( (byte)gs_queue,
                                       timer_id_local,
                                       mm_timer_non_deferrable(timer_id_local));

      if (gs_status != GS_SUCCESS)
      {
         MSG_ERROR_DS(MM_SUB, "=MM= Stop timer failure", 0,0,0);
      }
}

#endif

#ifdef FEATURE_ENHANCED_NW_SELECTION

/*===========================================================================

FUNCTION MM_START_FOREGROUND_SEARCH_TIMER

DESCRIPTION
  This function starts the foreground search timer

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void mm_start_foreground_search_timer( dword num_millisecs)
{
   dword timeout_value;
   byte i;

   timeout_value = (dword) gs_enquire_timer(  (byte)get_mm_timer_queue(), TIMER_FOREGROUND_SEARCH );

   if (mm_foreground_search_lai_timer_list.length ==1 )
   {
      mm_start_timer(TIMER_FOREGROUND_SEARCH,num_millisecs);
      mm_foreground_search_lai_timer_list.lai_timer_list[mm_foreground_search_lai_timer_list.length-1].timer_count = 0;
   }
   else if ( num_millisecs < timeout_value )
   {
      for(i = 0; (i < (byte)(mm_foreground_search_lai_timer_list.length-1)) && (mm_foreground_search_lai_timer_list.length <= MAX_FOREGROUND_SEARCH_LAI_TIMER_LIST_LENGTH); i++)
      {
         mm_foreground_search_lai_timer_list.lai_timer_list[mm_foreground_search_lai_timer_list.length-1].timer_count += 
              (timeout_value-num_millisecs);
      }

      if (mm_timer_status[TIMER_FOREGROUND_SEARCH] == TIMER_ACTIVE)
      {
         mm_stop_timer(TIMER_FOREGROUND_SEARCH);
      }

      mm_start_timer(TIMER_FOREGROUND_SEARCH, num_millisecs);
      mm_foreground_search_lai_timer_list.lai_timer_list[mm_foreground_search_lai_timer_list.length-1].timer_count = 0;
    }
    else
    {
      mm_foreground_search_lai_timer_list.lai_timer_list[mm_foreground_search_lai_timer_list.length-1].timer_count  = (num_millisecs-timeout_value);
    }
}
#endif

byte get_mm_timer_queue( void )
{
#if defined FEATURE_DUAL_SIM || defined FEATURE_SGLTE 
  if(mm_as_id == (mm_as_id_e_type)SYS_MODEM_AS_ID_1)
  
  {
    return GS_QUEUE_MM;
  }
  else if(mm_as_id == (mm_as_id_e_type)SYS_MODEM_AS_ID_2)
  {
    return GS_QUEUE_MM_SUB1;
  }
#if defined(FEATURE_TRIPLE_SIM) || defined(FEATURE_SGLTE_DUAL_SIM)
  else if(mm_as_id == (mm_as_id_e_type)SYS_MODEM_AS_ID_3)
  {
    return GS_QUEUE_MM_SUB2;
    }
#endif
  else
  {
    return GS_QUEUE_MM_SUB1;
  }
#else
    return GS_QUEUE_MM;
#endif
}
