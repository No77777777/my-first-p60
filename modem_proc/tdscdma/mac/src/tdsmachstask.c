/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*===

        M A C    H S D P A    D O W N L I N K    T A S K

GENERAL DESCRIPTION
  This file contains the Downlink MAC HSDPA task functions.

Copyright (c) 2002-2012 by Qualcomm Technologies, Incorporated.  All Rights Reserved.


=============================================================================================*/
/*=============================================================================================

                      EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/tdscdma.mpss/4.3/mac/src/tdsmachstask.c#1 $ $DateTime: 2019/08/19 10:51:23 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------------------------
06/02/14  sarao   Removed the inclusion of tmc.h as its being discontinued in core.
10/24/10   guo     Initial release Upmerged WCDMA to TDS

=============================================================================================*/


/*===============================================================================================

              I N C L U D E    F I L E S

=============================================================================================*/

#include "tdscdma_variation.h"
#include "comdef.h"

#ifdef CUST_H
#include "customer.h"
#endif


#include  "rex.h"
#include  "task.h"
//#include  "dog.h"
#include "dog_hb_rex.h"   //using DOG HB mechanism
#include  "tdsmachstask.h"
#include  "tdsmacdlhs.h"
#include  "log_codes_tdscdma.h"
#ifdef FEATURE_MODEM_RCINIT 
#include "rcinit.h"
#endif

#ifdef FEATURE_SEGMENT_LOADING
#include "mcfg_seg_load.h"
#endif /*FEATURE_SEGMENT_LOADING*/

/*=============================================================================================

                DEFINITIONS AND DECLARATIONS FOR MODULE

This section contains definitions for constants, macros, types, variables
and other items needed by this module.

=============================================================================================*/
#if 0 //using DOG HB mechanism
/* MAC_HS DL watchdog report timer block  */
rex_timer_type  tdsmac_hs_dl_dog_rpt_timer_blk;

  #undef  DOG_TDS_MAC_HS_RPT
  #define DOG_TDS_MAC_HS_RPT tds_mac_hs_dog_rpt_var
  dog_report_type   tds_mac_hs_dog_rpt_var           = 0;      /* Initial Safety */

  #undef  DOG_TDS_MAC_HS_RPT_TIME
  #define DOG_TDS_MAC_HS_RPT_TIME tds_mac_hs_dog_rpt_time_var
  uint32  tds_mac_hs_dog_rpt_time_var      = 0xFFFF; /* Initial Safety */
#endif
/*--------------------------------------------------------------------------
 *  Mutex to protect the header read ISR and 
 *  MAC DL recfg flushing the partial/complete PDUs 
 ---------------------------------------------------------------------------*/
extern rex_crit_sect_type  tdsmac_ehs_recfg_mutex;

#ifdef FEATURE_SEGMENT_LOADING
extern void tds_task_null (void);
#endif /*FEATURE_SEGMENT_LOADING*/

/*=============================================================================================

FUNCTION  tds_mac_hs_dl_wait

DESCRIPTION
  This function waits for any signal in a specified signal mask to be set.
  If this procedure detects that any signal in the specified signal mask is
  set, it will return the entire mask of signals set in the task's tcb.

DEPENDENCIES
  None.


RETURN VALUE
  Mask of signals returned by the rex_wait call.

SIDE EFFECTS
  None.

=============================================================================================*/
uint32  tds_mac_hs_dl_wait
   (
   /* signals to wait on */
   uint32  wait_sigs
   )
{
   /* signals returned by rex_wait call */
   uint32  sigs;

   /* Wait for requested signals */
   while (((sigs = rex_get_sigs( rex_self()) ) & wait_sigs) == 0 )
   {

      (void)rex_wait(wait_sigs);

   }
   return( sigs );

} /* mac_hs_dl_wait */

/*=============================================================================================

FUNCTION  tds_mac_hs_dl_task

DESCRIPTION
  This task performs MAC HS DL processing.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

=============================================================================================*/
#ifdef FEATURE_SEGMENT_LOADING
void tds_mac_hs_dl_task (dword dummy) __attribute__((section(".task_text")));
#endif /*FEATURE_SEGMENT_LOADING*/

void  tds_mac_hs_dl_task
   (
   dword dummy
   /* Required for REX, ignore */
   /*lint -esym(715,dummy)
   ** Have lint not complain about the ignored parameter 'dummy' which is
   ** specified to make this routine match the template for rex_def_task().
   */
   )
{
   uint32    sigs, timer_sig, mask = 0x1;
	 dog_report_type		 dogRpt; //using DOG HB mechanism

   uint8 i;
   
#ifdef FEATURE_MODEM_RCINIT
  /* initialize the threshold count for counting Events from TRM */
  RCEVT_THRESHOLD current;
#endif

#ifdef FEATURE_SEGMENT_LOADING
  interface_t *pi_tdscdma_segment = NULL;

  pi_tdscdma_segment = get_tdscdma_interface();
  if(pi_tdscdma_segment == NULL)
  {
    tds_task_null ();
    return;
  }
#endif /*FEATURE_SEGMENT_LOADING*/

   #ifdef FEATURE_MODEM_RCINIT 
   /* Send Handshake to RCINIT */
   rcinit_handshake_startup();
   /* Initilize the parameters for DL mac */
   tdsmac_hs_dl_init();
   #else

   /* Waits for start signal from TMC */
   /* Sets ACK signal when receives the task start signal */
   tmc_task_start();
    /* Initilize the parameters for DL mac */
   tdsmac_hs_dl_init();
   #endif

     /* initialize the tcb */
   tds_mac_hs_dl_init_tcb();
  
#ifdef FEATURE_MODEM_RCINIT
#if !(defined(TEST_FRAMEWORK) || defined(TF_UNIT_TEST))
	current = rcevt_getcount_name("TRM_INIT_COMPLETE");	
	if(0==current) {   
	(void) rcevt_wait_name("TRM_INIT_COMPLETE");
	}
#endif
#endif 

#if 0  //using DOG HB mechanism
         /*Register with DOG, get the Task ID*/
         tds_mac_hs_dog_rpt_var      = dog_register(tds_mac_hs_dl_get_tcb(), \
                                   DOG_DEFAULT_TIMEOUT, DOG_DEADLKCHK_DISABLE);
         /*Get report interval*/
         tds_mac_hs_dog_rpt_time_var = dog_get_report_period(tds_mac_hs_dog_rpt_var);

         /* If the return value from dog_get_report_period() was zero then we MUST
         ** NOT report to dog directly, as DOG will monitor us in a different manner.
         ** Hence we won't initialize or start any timer.
         */
         if ( tds_mac_hs_dog_rpt_time_var > 0 ) 
         { 
           /* Initialize taskname watchdog report timer */
           rex_def_timer(&tdsmac_hs_dl_dog_rpt_timer_blk, tds_mac_hs_dl_get_tcb(), TDSMAC_HS_DL_DOG_RPT_TIMER_SIG);
           (void)rex_set_sigs (tds_mac_hs_dl_get_tcb(), TDSMAC_HS_DL_DOG_RPT_TIMER_SIG);
         }
         else
         {
           TDSMAC_MSG1(MSG_LEGACY_HIGH,"dog report time %d",tds_mac_hs_dog_rpt_time_var);
         }
#else
  /* Register with Dog HB. NOTE: Do not register multiple times per task context! */
  dogRpt = dog_hb_register_rex((rex_sigs_type)TDSMAC_HS_DL_DOG_RPT_TIMER_SIG);
#endif	 
   /* Task loop */
   while (1)
   {
      /* Wait for MAC HS DL signal to be set */
      sigs = tds_mac_hs_dl_wait(
                                 TDSMAC_HS_DL_DOG_RPT_TIMER_SIG |
                                 TDSMAC_HS_DL_DATA_IND_SIG      |
                                 TDSMAC_HS_QUEUE_1_TIMER_SIG    |
                                 TDSMAC_HS_QUEUE_2_TIMER_SIG    |
                                 TDSMAC_HS_QUEUE_3_TIMER_SIG    |
                                 TDSMAC_HS_QUEUE_4_TIMER_SIG    |
                                 TDSMAC_HS_QUEUE_5_TIMER_SIG    |
                                 TDSMAC_HS_QUEUE_6_TIMER_SIG    |
                                 TDSMAC_HS_QUEUE_7_TIMER_SIG    |
                                 TDSMAC_HS_QUEUE_8_TIMER_SIG    |
                                 TDSMAC_HS_DL_CONFIG_REQ_SIG
#ifdef FEATURE_TDSCDMA_MAC_HS_QXDM_LOGGING
                                 |
                                 TDSMAC_HS_DL_STATUS_LOG_TIMER_SIG
#endif
#ifdef FEATURE_Q6_MT
                                 | TDSMAC_HS_P_TO_C_DATA_IND_SIG
#endif
#ifdef FEATURE_TDSCDMA_THROUGHPUT_OPT_AFTER_TA
                                 | TDSMAC_HS_T1_OPT_IND_SIG
#endif                                 
#if defined(TEST_FRAMEWORK) && defined(TF_UNIT_TEST)
                       			#error code not present
#endif/*TEST_FRAMEWORK*/
                                 );

      /* MAC HS DL Command Queue Signal is set */
      if (sigs & TDSMAC_HS_DL_DOG_RPT_TIMER_SIG)
      {
         /* Clear DL watchdog report timer sig */
         (void)rex_clr_sigs (rex_self(), TDSMAC_HS_DL_DOG_RPT_TIMER_SIG);
#if 0 //using DOG HB mechanism
         if (tds_mac_hs_dog_rpt_time_var > 0) 
         {    /* Zero means don't report */
            (void)rex_set_timer(&tdsmac_hs_dl_dog_rpt_timer_blk, tds_mac_hs_dog_rpt_time_var);
            dog_report(tds_mac_hs_dog_rpt_var);
         }
         else
         {
           TDSMAC_MSG1(MSG_LEGACY_HIGH,"dog report time %d",tds_mac_hs_dog_rpt_time_var);
         }
#else
        /* Report to the Dog HB */
        dog_hb_report(dogRpt);
#endif
      }

      /* Received a signal to handle MAC HS config */
      if (sigs & TDSMAC_HS_DL_CONFIG_REQ_SIG)
      {
         (void)rex_clr_sigs(rex_self(), TDSMAC_HS_DL_CONFIG_REQ_SIG);

         tdsmac_hs_dl_config_req_handler();

      }

      /* Received a signal to handle MAC HS data */
      if (sigs & TDSMAC_HS_DL_DATA_IND_SIG)
      {
         (void)rex_clr_sigs (rex_self(), TDSMAC_HS_DL_DATA_IND_SIG);

         tdsmac_hs_dl_data_handler();
      }

#ifdef FEATURE_Q6_MT
#ifdef FEATURE_TDSCDMA_HSDPA_MAC_EHS
      /* Received a signal to handle MAC HS data */
      if (sigs & TDSMAC_HS_P_TO_C_DATA_IND_SIG)
      {
         (void)rex_clr_sigs (rex_self(), TDSMAC_HS_P_TO_C_DATA_IND_SIG);
         REX_ISR_LOCK(&tdsmac_ehs_recfg_mutex);

         tdsmac_ehs_pass_comp_pdu_to_rlc();

         REX_ISR_UNLOCK(&tdsmac_ehs_recfg_mutex);
      }
#endif
#endif

      sigs = rex_get_sigs(rex_self());

      /* Received signal(s) indicating that the queue timer(s) have expired */
      for (i = 0; i < TDSMAC_HS_DL_MAX_QUEUES; i++)
      {
         timer_sig = sigs & (mask << (2+i));

         if (timer_sig)
         {
            (void)rex_clr_sigs (rex_self(), timer_sig);
            tdsmac_hs_dl_timer_expiry_handler( i );
         }
      }

#ifdef FEATURE_TDSCDMA_MAC_HS_QXDM_LOGGING
      if (sigs & TDSMAC_HS_DL_STATUS_LOG_TIMER_SIG)
      {
         (void)rex_clr_sigs (rex_self(), TDSMAC_HS_DL_STATUS_LOG_TIMER_SIG);
         tdsmac_hs_dl_status_log_timer_expiry_handler();
      }
#endif
#ifdef FEATURE_TDSCDMA_THROUGHPUT_OPT_AFTER_TA
      if (sigs & TDSMAC_HS_T1_OPT_IND_SIG)
      {
        (void)rex_clr_sigs (rex_self(), TDSMAC_HS_T1_OPT_IND_SIG);
        tdsmac_hs_dl_timer_opt_handler();
      }
#endif
#if defined(TEST_FRAMEWORK) && defined(TF_UNIT_TEST)
#error code not present
#endif/*TEST_FRAMEWORK*/     

   } /* while loop */

  /* Deregister if leaving the task so that we don't ERR_FATAL */
  dog_hb_deregister(dogRpt);

}/* mac_hs_dl_task */

