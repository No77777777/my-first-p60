/*===========================================================================

                                  R R C T M R

GENERAL DESCRIPTION
 The RRCTMR module consists of RRC timers implementation.

EXTERNALIZED FUNCTIONS



Copyright (c) 2000-2009 Qualcomm Technologies, Inc.  
All Rights Reserved.QUALCOMM Proprietary.  Export of this technology or software is regulated
by the U.S. Government. Diversion contrary to U.S. law prohibited.
===========================================================================*/


/*===========================================================================
                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$PVCSPath:  L:/src/asw/MSM5200/RRC/vcs/rrctmr.c_v   1.10   30 May 2002 09:06:26   krishnan  $
$Header: //components/rel/wcdma.mpss/8.4.0/rrc/src/rrctmr.c#6 $ $DateTime: 2020/07/14 01:35:01 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
07/14/20   ak      Made changes for FR 61612
04/20/20   gcs     FR59710: Made changes to delay FRLTE and WtoL Resel for MAV
06/24/19   gcs     Made changes for FR 57381
11/20/18   ak      Made changes for FR 53313
22/09/16   sg      Made change to handle back to back SMC
04/03/18   sg      Made change to handle back to back SMC
02/21/18   sp      Added code to support LTA during activation time
01/30/18   sas     Supervision timer changes in LTA gap.
01/18/18   vs      Added NV for allowing LTA during access priority
05/12/16   sn      f3 reduction changes BPLMN scenarios
05/03/16   sp      Made changes to have a maximum wait time for allowing LTAs despite of pending L2 acks
02/10/16   sr      Changes to indicate duty cycle change to WL1
02/11/16   gv      Made changes to allow handling of LTA gap immediately and prevent creation of back to back LTA
07/08/15   sr      TRM-U changes for NAS-RRC interface
07/01/15   ad      Made changes to make unlock by timer as non-deferable
06/29/15   sn      Changes to remember that a hole is introduced and include it in
                   subsequent schedule table till poke timer is running
06/10/15   vi      Made changes to correctly clear the SIB7 Expiration timer.
05/11/15   sn      WCDMA RRC changes for FR 24494
02/26/15   sr      Made changes to convert BPLMN_LOCK_REL_OFFSET_TIMER to a non-deferrable one
12/31/14   bc      Cahgnes to replace event_report_payload with WRRC_EVENT_REPORT_PAYLOAD.
11/21/14   gv      Removal of code changes under FEATURE_EOOS
06/23/14   vi      Made changes to skip SIB7 reading if SIB7 expiration timer is stil running
06/11/14   bc      Made changes to reduce DCH to FACH cell selection time.
06/11/14   as      Made changes to delay reselection while waiting for L2 ACK for PCH transition
04/02/14   vg      Made changes to Systematically log all the internal and external primitive handled in RRC and corresponding top level action.
11/05/13   rmn     Changes for displaying Partial PLMN Search Results during Manual Search
01/02/14   rmn     Reduction of unwanted F3's
09/16/13   sn      G2W TA feature changes
01/03/13   ad      Made changes to reduce the redundant F3s
12/25/12   db      Made changes to requeue transition to disconnected triggered by CU when there is no response
12/07/12   sn      CSG feature changes
10/18/12   gv      Replace FEATURE_DUAL_SIM_WCDMA_TUNEAWAY with FEATURE_DUAL_SIM
09/21/12   gv      Added code to provide remaining time for RRCTMR_LTE_TO_WCDMA_REDIR_SRCH_TIMER
07/24/12   geg     Upmerge WCDMA.MPSS.1.0 to WCDMA.MPSS.2.0
05/01/12   sks     Changes were made to start a delay timer if pre-emption cannot be handled, and handle it
                   once the timer expires.
05/01/12   sks     Made changes to create delay_preemption_timer using the correct timer ptr.
03/29/12   sks     Made changes to get the TA duration and pass it to L1.
04/09/12   gv      Added code to prevent CPU hogging during G2W redirection
03/14/11   rl      Merged with VU_MODEM_WCDMA_RRC.01.93.50
03/10/11   sks     Made changes to implement 5s guard timer for LTOW redirection.
02/07/11   rl      Merged with VU_MODEM_WCDMA_RRC.01.90.50
01/27/11   rm      Added code to support WCDMA to LTE events
01/28/11   rl      Merged with VU_MODEM_WCDMA_RRC.01.89.57
01/12/11   ad      Added changes for RRC SW decoupling
12/31/10   rl      Merged with VU_MODEM_WCDMA_RRC.01.86.50
11/09/10   rl      Merged with VU_MODEM_WCDMA_RRC.01.84.50
10/18/10   rm      Added code to support EOOS Events
09/22/10   kp      Added code for Qchat timer that delay HSPA status indication
09/16/10   ss      Added missing code for Fast Dormancy feature under feature flag
                   FEATURE_3GPP_FAST_DORMANCY
10/30/10   rl      Objective systems migration changes check in 
                   on code base VU_MODEM_WCDMA_RRC.01.80.00
09/13/10   as      Added code to support FEATURE_DUAL_SIM
08/17/10   rm      Added code to return remaining time for RRCTMR_DED_PRI_VALIDITY_TIMER
06/29/10   rm      Added code to support LTE<->WCDMA Inter RAT
06/25/10   rm      Added code to support DUAL SIM BPLMN
06/07/10   as      Added code for EOOS under feature flag FEATURE_EOOS
04/26/10   ss      Added code changes for Fast Dormancy feature under feature flag
                   FEATURE_3GPP_FAST_DORMANCY
03/15/10   gkg     Made changes to put changes for "Multiple DRX Cycles" and 
                   "CU wait Timer" under feature R7. 
03/12/10   rm      Added code to support FEATURE_DUAL_SIM
11/02/09   sks     Added code to suspend handling of STOP_MODE_REQ, until current 
                   WTOG process is aborted.
05/10/09   kp      Added support for demand paging RRC code using pragma.
05/08/09   ss      Updated Copyright Information
02/02/09   rm      Fixed Lint errors
01/19/08   ps      Made changes for ASn1 migration 25.331v7.9  
                   i.e.May 2008 version  (2008-05) 
12/29/08   rm      Added support for new Events
12/10/08   ps      Made changes to not allocate memory from static buffers
                   only for the timer expiry Ind, it should be allocated 
                   from heap only
11/06/08   sk      Added RRCTMR_WTOG_BPLMN_ABORT_GUARD_TIMER which guards against 
                   RRC waiting indefinitely for W2G BPLMN to be aborted
06/09/08   pk      Added support for FEATURE_GTOW_REDIRECTION_AFTER_CONN_RELEASE.
03/06/08   da      Changed FEATURE_MBMS to FEATURE_MODEM_MBMS
12/21/07   da      Support for FEATURE_MBMS
10/25/07   kp      Corrected right pointer name in rex_delete_timer_ex 
                   for BCCH Modification timer.
09/10/07   pk      Added support for Frequency redirection received in 
                   FACH->FACH and FACH->PCH cases under feature flag
                   FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU
05/23/07   pk      Added support for new timer RRCTMR_MANUAL_PLMN_FREQ_SCAN_TIMER
                   under the feature flag FEATURE_MANUAL_SEARCH_FREQ_SCAN_SKIP_TIMER.
                   Full frequency scans are not done when this timer is active.
03/28/07   kp      Added support for new timer RRC_BCCH_MODIFICATION_TIMER 
                   which  is used to implemented support for BCCH Mod Timer 
                   I.E in BCCH ModInfo.
09/25/06   kp      Added support for new timer RRC_SIB_CHANGE_GUARD_TIMER which 
                   prevents bplmn search during sib_change_ind handling
08/25/06   vm      Added support for FEATURE_DEEP_SLEEP_REL6_OUT_OF_SERVICE_ENHANCEMENT.
07/21/06   vm      Added support for the feature UOOSI.
06/07/06   kp      Added support for six hours sib update
11/08/05   vm      Added support for WTOW and WTOG Background PLMN Search. 
                   Added two new timers WTOW DRX search timer and WTOW/WTOG 
                   max guard timer.
11/04/05  da/vr    Made changes to detect UE inactivity and take appropriate actions.
07/08/05   vm      FEATURE_OUT_OF_SERVICE_ENHANCEMENT: Added support for OOS in 
                   Connected Mode feature.
                   Added support for new timer RRCTMR_CONN_MODE_OOS_TIMER.
07/07/05   da      Added new func rrctmr_get_remaining_time() that returns
                   remaining tics of timer passed in.
03/03/05   vm      Added support for inter-frequency redirection feature.
                   Added new timer RRCTMR_FREQ_REDIRECT_WAIT_TIMER.
11/22/04   vm      Added support for RRCTMR_BPLMN_SRCH_TIMER and 
                   RRCTMR_BPLMN_FREQ_SCAN_TIMER.
11/24/04   da      Added support for RRCTMR_PHY_CHAN_FAILURE_TIMER
11/18/04   vm      Added support for RRCTMR_DEEP_SLEEP_NO_SVC_TIMER timer
11/18/04   vk      Added support to process RRCTMR_RRC_CON_DELAY_TIMER timer
09/15/04   sk      Added support for RRCTMR_T_OOS_TIMER.
08/30/04   sgk     Typecast third param of rex_def_timer_ex to uint32 in
                   function rrctmr_create_timers to fix multiple lint errors
                   Converting enum to int.
07/26/04   sk      Added debug prints for start, stop and expiry of timers.
05/05/04   bu      Added support for RRCTMR_FREQ_SCAN_TIMER.
03/19/04   kc      Added timer support for GtoW PCCO. T3174
11/20/03   bu      Added timer support for cell selection while going from
                   CELL_DCH to CELL_PCH/URA_PCH.
09/03/03   bu      Added support for a new timer to delay ERR_FATALs.
06/23/03   vn      Added support for a new timer to detect L1 deadlocks.
02/11/03   vn      Added code to support a timer during cell selection
                   while going from CELL_DCH->CELL_FACH.
01/15/03   sk      Added lint directive to suppress 'ignoring function return
                   value' error no534.
11/04/02   rj      Added implementaion for a timer to Ignore the BCCH 
                   Modification Info if RRC already processing one.
05/29/02   ram     Added implementation for SIB7 Expiration timer. This timer
                   is set to the expiration time for SIB7 in a cell. When the
                   timer expires, SIB7 has to be read again.
02/14/02   rj      Added implementation for timers T302, T305, T316, T317,
                   T307, T314 and T315. These timers are required for Cell
                   Update procedure implemenation.
10/15/01   ram     Added implementation for a SIB wait timer. This is the maximum
                   time to wait for SIBs in a cell. Changed all timers to use
                   rex_def_timer_ex instead of rex_create_timer_ex. The function
                   rex_def_timer_ex is the correct one to use since these
                   timers exist all the time and are not deleted until powerdown.
09/18/01   kmp     Featurized the RRC timer multiplication for debugging only.
08/28/01   xfg     Added a timer for reporting RSSI and BER to CM
08/08/01   rj      Multiplied all RRC timers by 10 for debugging purpose.
07/27/01   rj      Fixed a typo in function rrctmr_create_timers.
07/20/01   rj      Added implemenation for timer T304 for UE Capability
                   Information procedure.
05/02/01   jrs     Added semicolons after MSG_HIGHs.
02/12/01   rj      Added implementattion for Timer T308 and L2ACK timer for
                   RRC Connection Release procedure. The enum for RCE
                   L2ACK timer is changed to RRCTMR_RCE_L2ACK_TIMER from
                   RRCTMR_WAIT_FOR_L2_ACK_TIMER. The pointer variable for
                   RCE L2ACK timer is changed to rrc_t_rce_l2ack_ptr.
01/19/01   rj      Created file.

===========================================================================*/


/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/

#include "wcdma_variation.h"
#include "rex.h"
#include "err.h"
#include "rrccmd_v.h"
#include "rrctmr.h"
#include "rrclogging.h"
#ifdef FEATURE_DUAL_SIM
#include "rrcsigs.h"
#include "task.h"
#endif
#ifdef FEATURE_QCHAT
#include "rrc_qchatupk.h"
#endif /* FEATURE_QCHAT*/

#include "rrcdata_v.h"

/*following lint directive is added to suppress the "ignoring return value" error
only for this file */
/*lint -e534*/
/*===========================================================================

                DEFINITIONS AND DECLARATIONS FOR MODULE

===========================================================================*/
#ifdef FEATURE_DUAL_WCDMA 
LOCAL rrctmr_int_data_type rrctmr_int_data_sim[MAX_RRC_STACKS];
#define rrctmr_int_data rrctmr_int_data_sim[WAS_ID_M]
#else
LOCAL rrctmr_int_data_type rrctmr_int_data;
#endif

#ifdef FEATURE_DUAL_SIM
/*Define non deferrable timer group for RRC*/
timer_group_type rrc_timer_group_non_deferrable;
#endif
/*===========================================================================
** -----------------------------------------------------------------------------------
**-- ----------NON-Demand paging section Srart--------------------------------------------
** -----------------------------------------------------------------------------------
  This section will have code section that will not be demand paged. Function which should be in this section are
  -RRC functions that are called by L1/L2 which are higher priority then RRC
  -RRC functions that are called in ISR context or RTOS timer call back
  -RRC functions that indirectly gets called by L1/L2 /ISR/timer call back   
  For example: Fun1() gets called by L1. Fun2() is called by Fun1(). Now both  Fun1() & Fun2() should be NON demand paged, 
  so both should be added in this section  
  Usage of pragma 
  __WCDMA_RRC_CODE_SEGMENT_NON_DEMANDPAGED__
  void foo(void)
  {
    function body here
  }
        __WCDMA_RRC_CODE_SEGMENT_NON_DEMANDPAGED_END__
  If you have another function to be added in the same section then again it needs to be wrapped with these pragma 
  for example:- function bar() is just below function foo() then function bar() needs to be written like this …. 
  __WCDMA_RRC_CODE_SEGMENT_NON_DEMANDPAGED__
  void bar(void)
  {
    function body here
  }
  __WCDMA_RRC_CODE_SEGMENT_NON_DEMANDPAGED_END__
** ----------------------------------------------------------------------- */

#ifdef FEATURE_DUAL_SIM
/*===========================================================================

FUNCTION rrc_def_non_defer_timer

DESCRIPTION
  Define non deferrable timer for RRC

DEPENDENCIES
  None

RETURN VALUE
  None
SIDE EFFECTS
  None
===========================================================================*/
void rrc_def_non_defer_timer(timer_type *p_timer, rrc_non_def_timer_cb_ptr timer_cb_ptr, unsigned long param)
{
   timer_group_set_deferrable( &rrc_timer_group_non_deferrable, FALSE );
   timer_def(p_timer, &rrc_timer_group_non_deferrable, NULL, 0, timer_cb_ptr, param);
}
/*===========================================================================

FUNCTION rrc_set_non_defer_timer

DESCRIPTION
  Set non deferrable timer for RRC

DEPENDENCIES
  None

RETURN VALUE
  None
SIDE EFFECTS
  None
===========================================================================*/
void rrc_set_non_defer_timer
(
   timer_type     *p_timer,  /* pointer to timer to set  */
   timetick_type  cnt       /* value to set the timer   */
)
{
   timer_set(p_timer, cnt, 0, T_MSEC);
}
/*===========================================================================

FUNCTION rrc_clr_non_defer_timer

DESCRIPTION
  Clear non deferrable timer for RRC

DEPENDENCIES
  None

RETURN VALUE
  None
SIDE EFFECTS
  None
===========================================================================*/
void rrc_clr_non_defer_timer(timer_type *p_timer)
{
   timer_clr(p_timer, T_MSEC);
}
/*===========================================================================

FUNCTION tdsrrc_get_non_defer_timer

DESCRIPTION
  Get remaining time of non deferrable timer for RRC

DEPENDENCIES
  None

RETURN VALUE
  None
SIDE EFFECTS
  None
===========================================================================*/
timetick_type rrc_get_non_defer_timer
(
  timer_type *p_timer       /* pointer to the timer to get */
)
{
  return timer_get(p_timer, T_MSEC);
}
#endif

/*===========================================================================

FUNCTION rrctmr_timers_callback

DESCRIPTION
  This function will be called when ever RRC timers expire.

DEPENDENCIES
  None

RETURN VALUE
  None
SIDE EFFECTS
  None
===========================================================================*/
 /***/ __WCDMA_RRC_CODE_SEGMENT_NON_DEMANDPAGED__ /***/
void rrctmr_timers_callback
(
  sys_modem_as_id_e_type wrrc_as_id, 
  unsigned long param                   /* Parameter to Identify timer */                               
)
{
  rrc_cmd_type *cmd_ptr;                     /* Pointer to RRC Command */

  rrclog_rrc_timer_expired_event_type rrc_timer_expired_event;  
#ifdef FEATURE_FEMTO_CSG
  rrclog_wcdma_asf_timer_status_event_type asf_timer_status;
#endif

#ifdef FEATURE_DUAL_SIM
  if(param == RRCTMR_WRM_LOCK_TIMER)
  {
    /*Set TRM Signal, so that RRC is unblocked*/
    (void) rex_set_sigs( wcdma_rrc_get_tcb(wrrc_as_id), RRC_WRM_SIG);
    WRRC_MSG1_HIGH("Expired timer:%d",param);
    return ;
  }
#endif

  if( (cmd_ptr =  rrc_get_int_cmd_buf(wrrc_as_id)) != NULL)
  {   
    switch(param)
    { 
      case RRCTMR_T_300_TIMER: 
        /* Fill in the command parameters to RRC_T300_EXPIRED_IND Command */
        cmd_ptr->cmd_hdr.cmd_id = RRC_T300_EXPIRED_IND;
        break;

      case RRCTMR_T_308_TIMER: 
        /* Fill in the command parameters to RRC_T308_EXPIRED_IND Command */
        cmd_ptr->cmd_hdr.cmd_id = RRC_T308_EXPIRED_IND;
        break;

      case RRCTMR_T_304_TIMER: 
        /* Fill in the command parameters to RRC_T304_EXPIRED_IND Command */
        cmd_ptr->cmd_hdr.cmd_id = RRC_T304_EXPIRED_IND;
        break;

      case RRCTMR_WAIT_TIME_TIMER:
        /* Fill in the command parameters to RRC_WAIT_TIMER_EXPIRED_IND Command */
        cmd_ptr->cmd_hdr.cmd_id = RRC_WAIT_TIMER_EXPIRED_IND;
        break;

      case RRCTMR_PHY_CHAN_FAILURE_TIMER:
        /* Fill in the command parameters to RRC_WAIT_TIMER_EXPIRED_IND Command */
        cmd_ptr->cmd_hdr.cmd_id = RRC_PHY_CHAN_FAILURE_TIMER_EXPIRED_IND;
        break;

      case RRCTMR_UE_TO_CAMP_ON_TIMER:
        /* Fill in the command parameters to RRC_TIMER_TO_CAMP_ON_EXPIRED_IND Command */
        cmd_ptr->cmd_hdr.cmd_id = RRC_TIMER_TO_CAMP_ON_EXPIRED_IND;
        break;

      case RRCTMR_RRC_CON_DELAY_TIMER:
        /* Fill in the command parameters to RRC_SIG_CON_DELAY_TIMER_EXPIRED_IND Command */
        cmd_ptr->cmd_hdr.cmd_id = RRC_RRC_CON_DELAY_TIMER_EXPIRED_IND;
        break;
      case RRCTMR_RCE_L2ACK_TIMER:
         /* Fill in the command parameters to RRC_TIMER_FOR_L2ACK_EXPIRED_IND Command */
        cmd_ptr->cmd_hdr.cmd_id = RRC_TIMER_FOR_RCE_L2ACK_EXPIRED_IND;
        break;

      case RRCTMR_RCR_L2ACK_TIMER:
         /* Fill in the command parameters to RRC_TIMER_FOR_L2ACK_EXPIRED_IND Command */
        cmd_ptr->cmd_hdr.cmd_id = RRC_TIMER_FOR_RCR_L2ACK_EXPIRED_IND;
        break;

      case RRCTMR_RSSI_BER_TIMER:
         /* Fill in the Command Id to RRC_TIMER_FOR_RSSI_BER_EXPIRED_IND */
        cmd_ptr->cmd_hdr.cmd_id = RRC_TIMER_FOR_RSSI_BER_EXPIRED_IND;
        break;

      case RRCTMR_SIB_WAIT_TIMER:
         /* Fill in the Command Id to RRC_SIB_WAIT_TIMER_EXPIRED_IND */
        cmd_ptr->cmd_hdr.cmd_id = RRC_SIB_WAIT_TIMER_EXPIRED_IND;
        break;

#ifdef FEATURE_DUAL_SIM
      case RRCTMR_SIB_STOP_PARTITION_TIMER:
        cmd_ptr->cmd_hdr.cmd_id = RRC_SIB_STOP_PARTITION_TMR_EXPIRED_IND;
        break;
#endif

      case RRCTMR_T_302_TIMER: 
        /* Fill in the command parameters to RRC_T302_EXPIRED_IND Command */
        cmd_ptr->cmd_hdr.cmd_id = RRC_T302_EXPIRED_IND;
        break;

      case RRCTMR_T_305_TIMER: 
        /* Fill in the command parameters to RRC_T305_EXPIRED_IND Command */
        cmd_ptr->cmd_hdr.cmd_id = RRC_T305_EXPIRED_IND;
        break;

      case RRCTMR_T_316_TIMER: 
        /* Fill in the command parameters to RRC_T316_EXPIRED_IND Command */
        cmd_ptr->cmd_hdr.cmd_id = RRC_T316_EXPIRED_IND;
        break;

      case RRCTMR_T_317_TIMER: 
        /* Fill in the command parameters to RRC_T317_EXPIRED_IND Command */
        cmd_ptr->cmd_hdr.cmd_id = RRC_T317_EXPIRED_IND;
        break;

      case RRCTMR_T_307_TIMER: 
        /* Fill in the command parameters to RRC_T307_EXPIRED_IND Command */
        cmd_ptr->cmd_hdr.cmd_id = RRC_T307_EXPIRED_IND;
        break;

      case RRCTMR_T_314_TIMER: 
        /* Fill in the command parameters to RRC_T314_EXPIRED_IND Command */
        cmd_ptr->cmd_hdr.cmd_id = RRC_T314_EXPIRED_IND;

        //EVENT: Event_Timer_Expired
        rrc_timer_expired_event.rrc_timer_id = (uint8) T_314_TIMER;
        WRRC_EVENT_REPORT_PAYLOAD(EVENT_WCDMA_RRC_TIMER_EXPIRED, sizeof(rrc_timer_expired_event),
                       (void *)&rrc_timer_expired_event);


        break;

      case RRCTMR_T_315_TIMER: 
        /* Fill in the command parameters to RRC_T315_EXPIRED_IND Command */
        cmd_ptr->cmd_hdr.cmd_id = RRC_T315_EXPIRED_IND;

        //EVENT: Event_Timer_Expired
        rrc_timer_expired_event.rrc_timer_id = (uint8) T_315_TIMER;
        WRRC_EVENT_REPORT_PAYLOAD(EVENT_WCDMA_RRC_TIMER_EXPIRED, sizeof(rrc_timer_expired_event),
                       (void *)&rrc_timer_expired_event);

        break;

      case RRCTMR_SIB7_EXP_TIMER: 
        /* Fill in the command parameters to RRC_SIB7_EXP_TIMER_EXPIRED_IND Command */
        cmd_ptr->cmd_hdr.cmd_id = RRC_SIB7_EXP_TIMER_EXPIRED_IND;
        break;

      case RRCTMR_IGNORE_BCCH_MOD_TIMER: 
        /* Fill in the command parameters to RRC_IGNORE_BCCH_MOD_TIMER_EXPIRED_IND
           Command */
        cmd_ptr->cmd_hdr.cmd_id = RRC_IGNORE_BCCH_MOD_TIMER_EXPIRED_IND;
        break;

      case RRCTMR_DCH_FACH_CELL_SELECTION_TIMER:
        /* Fill in the command parameters to RRC_DCH_FACH_CELL_SELECTION_TIMER_EXPIRED_IND
           Command */
        cmd_ptr->cmd_hdr.cmd_id = RRC_DCH_FACH_CELL_SELECTION_TIMER_EXPIRED_IND;
        break;

      case RRCTMR_FACH_TO_FACH_PCH_CELL_SELECTION_TIMER:
        /* Fill in the command parameters to RRC_FACH_TO_FACH_PCH_CELL_SELECTION_TIMER_EXPIRED_IND
           Command */
        cmd_ptr->cmd_hdr.cmd_id = RRC_FACH_TO_FACH_PCH_CELL_SELECTION_TIMER_EXPIRED_IND;
        break;

      case RRCTMR_DCH_PCH_CELL_SELECTION_TIMER:
        /* Fill in the command parameters to RRC_DCH_PCH_CELL_SELECTION_TIMER_EXPIRED_IND
           Command */
        cmd_ptr->cmd_hdr.cmd_id = RRC_DCH_PCH_CELL_SELECTION_TIMER_EXPIRED_IND;
        break;

      case RRCTMR_L1_DEADLOCK_DETECT_TIMER:
        /* Fill in the command parameters to RRC_DCH_FACH_CELL_SELECTION_TIMER_EXPIRED_IND
           Command */
        cmd_ptr->cmd_hdr.cmd_id = RRCTMR_L1_DEADLOCK_DETECT_TIMER_EXPIRED_IND;
        break;

      case RRCTMR_DELAY_ERR_FATAL_TIMER:
        /* Fill in the command parameters to RRCTMR_DELAY_ERR_FATAL_TIMER_EXPIRED_IND
           Command */
        cmd_ptr->cmd_hdr.cmd_id = RRCTMR_DELAY_ERR_FATAL_TIMER_EXPIRED_IND;
        break;

      case RRCTMR_T_3174_TIMER: 
        /* Fill in the command parameters to RRC_T3174_EXPIRED_IND Command */
        cmd_ptr->cmd_hdr.cmd_id = RRC_T3174_EXPIRED_IND;
        break;

      case RRCTMR_FREQ_SCAN_TIMER: 
        /* Fill in the command parameters to RRC_FREQ_SCAN_TIMER__EXPIRED_IND Command */
        cmd_ptr->cmd_hdr.cmd_id = RRC_FREQ_SCAN_TIMER_EXPIRED_IND;
        break;

      case RRCTMR_T_OOS_TIMER: 
        /* Fill in the command parameters to RRC_FREQ_SCAN_TIMER__EXPIRED_IND Command */
        cmd_ptr->cmd_hdr.cmd_id = RRC_OOS_TIMER_EXPIRED_IND;
        break;

      case RRCTMR_DEEP_SLEEP_NO_SVC_TIMER: 
          /* Fill in the command parameters to RRC_DEEP_SLEEP_NO_SVC_TIMER_EXPIRED_IND Command */
          cmd_ptr->cmd_hdr.cmd_id = RRC_DEEP_SLEEP_NO_SVC_TIMER_EXPIRED_IND;
          break;

      case RRCTMR_BPLMN_SRCH_TIMER:
        /* Fill in the command parameters to RRC_BPLMN_SRCH_TIMER_EXPIRED_IND Command */
        cmd_ptr->cmd_hdr.cmd_id = RRC_BPLMN_SRCH_TIMER_EXPIRED_IND;

        //EVENT: Event_BPLMN_Timer_Expired
        rrc_timer_expired_event.rrc_timer_id = (uint8) BPLMN_SRCH_TIMER;
        WRRC_EVENT_REPORT_PAYLOAD(EVENT_WCDMA_RRC_TIMER_EXPIRED, sizeof(rrc_timer_expired_event),
                       (void *)&rrc_timer_expired_event);

        break;
        
      case RRCTMR_BPLMN_FREQ_SCAN_TIMER:
        /* Fill in the command parameters to RRC_BPLMN_FREQ_SCAN_TIMER_EXPIRED_IND Command */
        cmd_ptr->cmd_hdr.cmd_id = RRC_BPLMN_FREQ_SCAN_TIMER_EXPIRED_IND;
        break;

      case RRCTMR_MANUAL_PLMN_FREQ_SCAN_TIMER:
        /* Fill in the command parameters to RRC_MANUAL_PLMN_FREQ_SCAN_TIMER_EXPIRED_IND Command */
        cmd_ptr->cmd_hdr.cmd_id = RRC_MANUAL_PLMN_FREQ_SCAN_TIMER_EXPIRED_IND;
        break;


      case RRCTMR_FREQ_REDIRECT_WAIT_TIMER:
        /* Fill in the command parameters to RRC_FREQ_REDIRECTION_WAIT_TIMER_EXPIRED_IND Command */
        cmd_ptr->cmd_hdr.cmd_id = RRC_FREQ_REDIRECT_WAIT_TIMER_EXPIRED_IND;
        break;

      case RRCTMR_CONN_MODE_OOS_TIMER:
        /* Fill in the command parameters to RRC_CONN_MODE_OOS_TIMER_EXPIRED_IND Command */
        cmd_ptr->cmd_hdr.cmd_id = RRC_CONN_MODE_OOS_TIMER_EXPIRED_IND;
        break;

#ifdef FEATURE_INACTIVITY_HANDLING
      case RRCTMR_INACTIVITY_TIMER:
        /* Fill in the command parameters to RRC_CONN_MODE_OOS_TIMER_EXPIRED_IND Command */
        cmd_ptr->cmd_hdr.cmd_id = RRC_INACTIVITY_TIMER_EXPIRED_IND;
        break;
#endif /* FEATURE_INTACTIVITY_HANDLING */

      case RRCTMR_WTOW_BPLMN_DRX_SRCH_TIMER:
        /* Fill in the command parameters to RRC_WTOW_BPLMN_SRCH_TIMER_EXPIRED_IND Command */
        cmd_ptr->cmd_hdr.cmd_id = RRC_WTOW_BPLMN_DRX_SRCH_TIMER_EXPIRED_IND;
        break;
  
      case RRCTMR_BPLMN_GUARD_SRCH_TIMER:
        /* Fill in the command parameters to RRC_WTOW_BPLMN_GUARD_SRCH_TIMER Command */
        cmd_ptr->cmd_hdr.cmd_id = RRC_BPLMN_GUARD_SRCH_TIMER_EXPIRED_IND;
        break;

      case  RRCTMR_SIXHR_SIB_UPDATE_TIMER:
        /* Fill in the command parameters to RRC_SIX_HRS_SIB_TIMER_UPDATE_IND Command */
        cmd_ptr->cmd_hdr.cmd_id = RRC_SIX_HRS_SIB_TIMER_UPDATE_IND;
        break;

      case  RRCTMR_TENMIN_SIB_UPDATE_TIMER:
        cmd_ptr->cmd_hdr.cmd_id = RRC_TEN_MIN_SIB_TIMER_UPDATE_IND;
        break;

#ifdef FEATURE_DUAL_SIM 
      case RRCTMR_WRM_UNLOCK_BY_TIMER:
        /* Fill in the command parameters to RRCTMR_EOOS_FULL_SCAN_TIMER Command */
        cmd_ptr->cmd_hdr.cmd_id = RRC_DUAL_SIM_WRM_UNLOCK_BY_TIMER_EXP_IND;
        break;
#endif


      case RRCTMR_CONN_MODE_OOS_INITIAL_NO_DS_TIMER:
        /* Fill in the command parameters to RRC_CONN_MODE_OOS_INITIAL_NO_DS_TIMER_EXPIRED_IND Command */
        cmd_ptr->cmd_hdr.cmd_id = RRC_CONN_MODE_OOS_INITIAL_NO_DS_TIMER_EXPIRED_IND;
        break;

      case RRCTMR_CONN_MODE_OOS_DS_TIMER:
        /* Fill in the command parameters to RRC_CONN_MODE_OOS_DS_TIMER_EXPIRED_IND Command */
        cmd_ptr->cmd_hdr.cmd_id = RRC_CONN_MODE_OOS_DS_TIMER_EXPIRED_IND;
        break;      

      case  RRC_SIB_CHANGE_GUARD_TIMER:
        /* Fill in the command parameters to RRC_SIB_CHANGE_GUARD_TIMER_EXPIRED_IND Command */
        cmd_ptr->cmd_hdr.cmd_id = RRC_SIB_CHANGE_GUARD_TIMER_EXPIRED_IND;
        break;

      case RRC_BCCH_MODIFICATION_TIMER:
        /* Fill in the command parameters to RRC_BCCH_MODIFICATION_TIMER_EXPIRED_IND Command */
        cmd_ptr->cmd_hdr.cmd_id = RRC_BCCH_MODIFICATION_TIMER_EXPIRED_IND;
        break;

      case RRCTMR_GTOW_REDIRECTION_WAIT_TIMER:
        /* Fill in the command parameters to RRC_GTOW_REDIRECTION_WAIT_TIMER_EXPIRED_IND Command */
        cmd_ptr->cmd_hdr.cmd_id = RRC_GTOW_REDIRECTION_WAIT_TIMER_EXPIRED_IND;
        break;

      case RRCTMR_GTOW_REDIRECTION_LFS_TIMER:
        /* Fill in the command parameters to RRC_GTOW_REDIRECTION_LFS_TIMER_EXPIRED_IND Command */
        cmd_ptr->cmd_hdr.cmd_id = RRC_GTOW_REDIRECTION_LFS_TIMER_EXPIRED_IND;
        break;

      case RRCTMR_WTOG_ABORT_GUARD_TIMER:
        cmd_ptr->cmd_hdr.cmd_id = RRC_WTOG_ABORT_GUARD_TIMER_EXPIRED_IND;
        break;

      case RRCTMR_T_319_TIMER:
        cmd_ptr->cmd_hdr.cmd_id = RRC_T319_EXPIRED_IND;
        break;


      case RRCTMR_T_320_TIMER:
        cmd_ptr->cmd_hdr.cmd_id = RRC_T320_EXPIRED_IND;
        break;	

#ifdef FEATURE_WCDMA_REL7_SPECRS
      case RRCTMR_COUEC_TIMER:
        cmd_ptr->cmd_hdr.cmd_id = RRC_TIMER_COUEC_EXPIRED_IND;
        break;	
#endif /*FEATURE_WCDMA_REL7_SPECRS*/

      case RRCTMR_T_323_TIMER:
        cmd_ptr->cmd_hdr.cmd_id = RRC_T323_EXPIRED_IND;
        break;

#ifdef FEATURE_DUAL_SIM
      case RRCTMR_DSIM_BPLMN_LOCK_REL_OFFSET_TIMER:
        cmd_ptr->cmd_hdr.cmd_id = RRC_BPLMN_SEARCH_RESUME_IND;
        WRRC_MSG0_HIGH("RRC_BPLMN_SEARCH_RESUME_IND sent to CSP");
        break;
#endif

#ifdef FEATURE_WCDMA_ABSOLUTE_PRIORITY
      case RRCTMR_DED_PRI_VALIDITY_TIMER:
        cmd_ptr->cmd_hdr.cmd_id = RRC_DED_PRI_VALDITY_TIMER_EXPIRED_IND;
        WRRC_EVENT_REPORT(EVENT_WCDMA_DED_PRIORITIES_VALIDITY_TIMER_EXPIRED);
        break;
#endif
#ifdef FEATURE_QCHAT
      case RRCTMR_QCHAT_FA_RRC_TIMER:
        cmd_ptr->cmd_hdr.cmd_id = RRC_QCHAT_FA_RRC_TIMER_EXPIRED_IND;
        break; 
#endif /* FEATURE_QCHAT */
#ifdef FEATURE_LTE_TO_WCDMA
      case RRCTMR_LTE_TO_WCDMA_REDIR_SRCH_TIMER:
        cmd_ptr->cmd_hdr.cmd_id = RRC_LTE_TO_WCDMA_SRCH_TIMER_EXPIRY_IND;
        WRRC_MSG0_HIGH("RRC_LTE_TO_WCDMA_SRCH_TIMER_EXPIRY_IND posted to CSP");
        break;
#endif

#ifdef FEATURE_DUAL_SIM
      case RRCTMR_WRM_DELAY_PREEMPTION_TIMER:
        cmd_ptr->cmd_hdr.cmd_id = RRC_WRM_PREEMPT_IND;
        WRRC_MSG0_HIGH("DSIM: RRC_WRM_PREEMPT_IND posted to CSP");
        break;

      case RRCTMR_TRM_PRIORITY_CHANGE_TIMER:
        cmd_ptr->cmd_hdr.cmd_id = RRC_TRM_PRIORITY_CHANGE_IND;
        WRRC_MSG0_HIGH("DSIM: RRC_TRM_PRIORITY_CHANGE_IND posted to dispatcher");
        break;

      case RRCTMR_ACQ_DTY_CYC_CHANGE_TIMER:
       cmd_ptr->cmd_hdr.cmd_id = RRC_ACQ_DUTY_CYCLE_CHANGE_IND;	  	
       WRRC_MSG0_HIGH("DSIM: RRC_ACQ_DUTY_CYCLE_CHANGE_IND posted to dispatcher");
       break;

      case RRCTMR_DELAY_LTA_TIMER:
        WRRC_MSG0_HIGH("DSIM: Delay LTA timer expired");
        break;

      case RRCTMR_MAX_TA_DURATION_TIMER:
        cmd_ptr->cmd_hdr.cmd_id = RRC_MAX_TA_DURATION_TIMER_EXPIRED_IND;
        break;
#endif

#ifdef FEATURE_FEMTO_CSG
      case RRCTMR_CSG_ASF_SRCH_TIMER:
        cmd_ptr->cmd_hdr.cmd_id = RRC_CSG_ASF_SRCH_TIMER_EXPIRY_IND;
        WRRC_MSG0_HIGH("RRC_CSG_ASF_SRCH_TIMER_EXPIRY_IND posted to CSP");
        asf_timer_status.timer_value = 0;
        asf_timer_status.timer_state = 2;
        WRRC_EVENT_REPORT_PAYLOAD(EVENT_WCDMA_RRC_ASF_TIMER_STATUS, sizeof(rrclog_wcdma_asf_timer_status_event_type),
                     (void*)&asf_timer_status);
        break;
#endif
      case RRCTMR_RRCCU_TRANS_TO_DISCON_TIMER: 
        cmd_ptr->cmd_hdr.cmd_id = RRC_CU_TRANS_TO_DISCON_TIMER_EXPIRED;
        break;

      case RRCTMR_BPLMN_PARTIAL_REPORT_TIMER:
        cmd_ptr->cmd_hdr.cmd_id = RRC_BPLMN_PARTIAL_REPORT_TIMER_EXPIRED_IND;
        break;

      case RRCTMR_WAIT_FOR_L2_ACK_FOR_PCH_TRANSITION_TIMER: 
        WRRC_MSG0_HIGH("RRCTMR_WAIT_FOR_L2_ACK_FOR_PCH_TRANSITION_TIMER expired");
        cmd_ptr->cmd_hdr.cmd_id = RRC_WT_FOR_L2_ACK_PCH_TRANSITION_OR_SMC_COMPLETE_TMR_EXP_IND;
        break;

      case RRCTMR_BLOCK_LTA_TIMER:
        /* Need not post any command */
        WRRC_MSG0_HIGH("DSIM: Block LTA timer expired");
        break;
#ifdef FEATURE_WCDMA_ALLOW_LTA_DURING_ACT_TIME
      case RRCTMR_OTA_BUFFER_ACTIVATION_TIMER: 
        if (RRC_CSP_UNLOCK_BY_SCAN == rrc_csp_int_data.curr_scan)
        {
          WRRC_MSG0_HIGH("BUF_OTA:Activation timer expiry is pended as unlock by scan is in progress");
          rrc_buffer_ota_info.rrc_buffered_ota_activation_time_expired_during_lta = TRUE;
        }
        cmd_ptr->cmd_hdr.cmd_id = RRC_ACTIVATION_TIMER_EXPIRY_FOR_BUFFER_OTA;
        break;
 #endif  /*FEATURE_WCDMA_ALLOW_LTA_DURING_ACT_TIME*/        
#ifdef FEATURE_WCDMA_W2L_PSEUDO_MEAS
      case RRCTMR_PSEUDO_MEAS_TIMER:
        cmd_ptr->cmd_hdr.cmd_id = RRC_PSEUDO_MEAS_TIMER_EXPIRED_IND;
        break;
#endif
#ifdef ELEVATOR_TRIGGER
      case RRCTMR_PSEUDO_MEAS_RXAGC_TIMER:
      cmd_ptr->cmd_hdr.cmd_id = RRC_PSEUDO_MEAS_RXAGC_TIMER_EXPIRED_IND;
        break;
#endif
#ifdef FEATURE_WCDMA_W2L_PSEUDO_MEAS
      case RRCTMR_ALLOW_EARFCN_FOR_PSEUDO_MEAS:
        cmd_ptr->cmd_hdr.cmd_id = RRCTMR_ALLOW_EARFCN_FOR_PSEUDO_MEAS_EXPIRED_IND;
        break;
#endif

      case RRCTMR_DELAY_WTOL_RESEL_TIMER:
        cmd_ptr->cmd_hdr.cmd_id = RRCTMR_DELAY_WTOL_RESEL_TIMER_EXPIRED_IND;
        break;
      case RRCTMR_CELL_DEPRI_TIMER:
        cmd_ptr->cmd_hdr.cmd_id = RRCTMR_CELL_DEPRI_TIMER_EXPIRED_IND;
        break;
  
      case RRCTMR_CELL_DEPRI_NBR_TIMER:
        cmd_ptr->cmd_hdr.cmd_id = RRCTMR_CELL_DEPRI_NBR_TIMER_EXPIRED_IND;
        break;

      default:
        WRRC_MSG0_ERROR("ERR Unknown RRC timer received!");
        break;
    }
 
  /* sends an internal command to RRC */
    WRRC_MSG1_HIGH("Expired timer:%d",param);
    rrc_put_int_cmd(wrrc_as_id, cmd_ptr);
  }
} /* rrctmr_timers_callback */

/*===========================================================================

FUNCTION rrctmr_timers_callback1

DESCRIPTION
  This function will be called when ever RRC timers expire.

DEPENDENCIES
  None

RETURN VALUE
  None
SIDE EFFECTS
  None
===========================================================================*/
void rrctmr_timers_callback1
(
  unsigned long param                   /* Parameter to Identify timer */
)
{
  rrctmr_timers_callback(SYS_MODEM_AS_ID_1,param);
}

#ifdef FEATURE_DUAL_WCDMA 
/*===========================================================================

FUNCTION rrctmr_timers_callback2

DESCRIPTION
  This function will be called when ever RRC timers expire.

DEPENDENCIES
  None

RETURN VALUE
  None
SIDE EFFECTS
  None
===========================================================================*/
void rrctmr_timers_callback2
(
  unsigned long param                   /* Parameter to Identify timer */
)
{
  rrctmr_timers_callback(SYS_MODEM_AS_ID_2,param);
}
#endif
/***/ __WCDMA_RRC_CODE_SEGMENT_NON_DEMANDPAGED_END__ /***/
/*===========================================================================

FUNCTION rrctmr_create_timers

DESCRIPTION
  This function will create all RRC  related timers.
DEPENDENCIES
  None

RETURN VALUE
  None
SIDE EFFECTS
  None
===========================================================================*/
 /***/ __WCDMA_RRC_CODE_SEGMENT_NON_DEMANDPAGED__ /***/
void rrctmr_create_timers
(
  sys_modem_as_id_e_type wrrc_as_id
)
 
{
  void *cb_ptr = rrctmr_timers_callback1;

#ifdef FEATURE_DUAL_WCDMA 
  if (WAS_ID_M == SYS_MODEM_AS_ID_2) 
  {
    cb_ptr = rrctmr_timers_callback2;
  }
#endif
  rrctmr_int_data.print_rem_msg = TRUE;

  /* Create Timer T300 */
  rrctmr_int_data.rrc_t_300_ptr = &rrctmr_int_data.rrc_t_300;
  rex_def_timer_ex( rrctmr_int_data.rrc_t_300_ptr,
                    cb_ptr,
                    (uint32)RRCTMR_T_300_TIMER );

  /* Create Timer T308 */
  rrctmr_int_data.rrc_t_308_ptr = &rrctmr_int_data.rrc_t_308;
  rex_def_timer_ex( rrctmr_int_data.rrc_t_308_ptr,
                    cb_ptr,
                    (uint32)RRCTMR_T_308_TIMER );

  /* Create Timer T304 */
  rrctmr_int_data.rrc_t_304_ptr = &rrctmr_int_data.rrc_t_304;
  rex_def_timer_ex( rrctmr_int_data.rrc_t_304_ptr,
                    cb_ptr,
                   (uint32) RRCTMR_T_304_TIMER );

  /* Create Timer for Wait time */
  rrctmr_int_data.rrc_t_wait_time_ptr = &rrctmr_int_data.rrc_t_wait_time;
  rex_def_timer_ex( rrctmr_int_data.rrc_t_wait_time_ptr,
                    cb_ptr,
                   (uint32) RRCTMR_WAIT_TIME_TIMER );

  /* Create Timer for UE to camp on */ 
  rrctmr_int_data.rrc_t_ue_to_camp_on_ptr = &rrctmr_int_data.rrc_t_ue_to_camp_on;
  rex_def_timer_ex( rrctmr_int_data.rrc_t_ue_to_camp_on_ptr,
                    cb_ptr,
                   (uint32) RRCTMR_UE_TO_CAMP_ON_TIMER );

  /* Create Timer for UE to camp on */ 
  rrctmr_int_data.rrc_t_rrc_con_delay_ptr = &rrctmr_int_data.rrc_t_rrc_con_delay;
  rex_def_timer_ex( rrctmr_int_data.rrc_t_rrc_con_delay_ptr,
                    cb_ptr,
                    (uint32) RRCTMR_RRC_CON_DELAY_TIMER );
  /* Create RCE timer for L2 ACK */
  rrctmr_int_data.rrc_t_rce_l2ack_ptr = &rrctmr_int_data.rrc_t_rce_l2ack;
  rex_def_timer_ex( rrctmr_int_data.rrc_t_rce_l2ack_ptr,
                    cb_ptr,
                   (uint32) RRCTMR_RCE_L2ACK_TIMER );

  /* Create RCR timer for L2 ACK */
  rrctmr_int_data.rrc_t_rcr_l2ack_ptr = &rrctmr_int_data.rrc_t_rcr_l2ack;
  rex_def_timer_ex( rrctmr_int_data.rrc_t_rcr_l2ack_ptr,
                    cb_ptr,
                    (uint32)RRCTMR_RCR_L2ACK_TIMER );

  /* Create reporting RSSI_BER timer */
  rrctmr_int_data.rrc_t_rssi_ber_ptr = &rrctmr_int_data.rrc_t_rssi_ber;
  rex_def_timer_ex( rrctmr_int_data.rrc_t_rssi_ber_ptr,
                    cb_ptr,
                   (uint32) RRCTMR_RSSI_BER_TIMER );

  /* Create reporting SIB wait timer */
  rrctmr_int_data.rrc_t_sib_wait_ptr = &rrctmr_int_data.rrc_t_sib_wait;
  rex_def_timer_ex( rrctmr_int_data.rrc_t_sib_wait_ptr,
                    cb_ptr,
                   (uint32) RRCTMR_SIB_WAIT_TIMER );

  /*Create SIB7 expiration timer*/
  rrctmr_int_data.rrc_t_sib7_expiration_ptr = &rrctmr_int_data.rrc_t_sib7_expiration;
  rex_def_timer_ex( rrctmr_int_data.rrc_t_sib7_expiration_ptr,
                    NULL,
                   (uint32) RRCTMR_SIB7_EXPIRATION_TIMER);

  /* Create Timer T302 */
  rrctmr_int_data.rrc_t_302_ptr = &rrctmr_int_data.rrc_t_302;
  rex_def_timer_ex( rrctmr_int_data.rrc_t_302_ptr,
                    cb_ptr,
                   (uint32) RRCTMR_T_302_TIMER );

  /* Create Timer T305 */
  rrctmr_int_data.rrc_t_305_ptr = &rrctmr_int_data.rrc_t_305;
  rex_def_timer_ex( rrctmr_int_data.rrc_t_305_ptr,
                    cb_ptr,
                  (uint32)  RRCTMR_T_305_TIMER );

  /* Create Timer T316 */
  rrctmr_int_data.rrc_t_316_ptr = &rrctmr_int_data.rrc_t_316;
  rex_def_timer_ex( rrctmr_int_data.rrc_t_316_ptr,
                    cb_ptr,
                    (uint32)RRCTMR_T_316_TIMER );

  /* Create Timer T317 */
  rrctmr_int_data.rrc_t_317_ptr = &rrctmr_int_data.rrc_t_317;
  rex_def_timer_ex( rrctmr_int_data.rrc_t_317_ptr,
                    cb_ptr,
                   (uint32) RRCTMR_T_317_TIMER );

  /* Create Timer T307 */
  rrctmr_int_data.rrc_t_307_ptr = &rrctmr_int_data.rrc_t_307;
  rex_def_timer_ex( rrctmr_int_data.rrc_t_307_ptr,
                    cb_ptr,
                    (uint32)RRCTMR_T_307_TIMER );

  /* Create Timer T314 */
  rrctmr_int_data.rrc_t_314_ptr = &rrctmr_int_data.rrc_t_314;
  rex_def_timer_ex( rrctmr_int_data.rrc_t_314_ptr,
                    cb_ptr,
                  (uint32)  RRCTMR_T_314_TIMER );

  /* Create Timer T315 */
  rrctmr_int_data.rrc_t_315_ptr = &rrctmr_int_data.rrc_t_315;
  rex_def_timer_ex( rrctmr_int_data.rrc_t_315_ptr,
                    cb_ptr,
                   (uint32) RRCTMR_T_315_TIMER );

  /* Create Timer for SIB7 Expiration */
  rrctmr_int_data.rrc_t_sib7_exp_ptr = &rrctmr_int_data.rrc_t_sib7_exp;
  rex_def_timer_ex( rrctmr_int_data.rrc_t_sib7_exp_ptr,
                    cb_ptr,
                   (uint32) RRCTMR_SIB7_EXP_TIMER );

  /* Create Timer to Ignore BCCH Modification Info */
  rrctmr_int_data.rrc_t_ignore_bcch_mod_exp_ptr = &rrctmr_int_data.rrc_t_ignore_bcch_mod_exp;
  rex_def_timer_ex( rrctmr_int_data.rrc_t_ignore_bcch_mod_exp_ptr,
                    cb_ptr,
                  (uint32)  RRCTMR_IGNORE_BCCH_MOD_TIMER );

  /* Create Timer during Cell Selection, going DCH->FACH */
  rrctmr_int_data.rrc_t_dch_fach_cell_selection_exp_ptr = &rrctmr_int_data.rrc_t_dch_fach_cell_selection_exp;
  rex_def_timer_ex( rrctmr_int_data.rrc_t_dch_fach_cell_selection_exp_ptr,
                    cb_ptr,
                   (uint32) RRCTMR_DCH_FACH_CELL_SELECTION_TIMER );

  /* Create Timer during Cell Selection, going FACH->FACH/PCH */
  rrctmr_int_data.rrc_t_fach_to_fach_pch_cell_selection_exp_ptr = &rrctmr_int_data.rrc_t_fach_to_fach_pch_cell_selection_exp;
  rex_def_timer_ex( rrctmr_int_data.rrc_t_fach_to_fach_pch_cell_selection_exp_ptr,
                    cb_ptr,
                   (uint32) RRCTMR_FACH_TO_FACH_PCH_CELL_SELECTION_TIMER );

  /* Create Timer to detect L1 deadlock */
  rrctmr_int_data.rrc_t_l1_deadlock_detection_exp_ptr = &rrctmr_int_data.rrc_t_l1_deadlock_detection_exp;
  rex_def_timer_ex( rrctmr_int_data.rrc_t_l1_deadlock_detection_exp_ptr,
                    cb_ptr,
                  (uint32)  RRCTMR_L1_DEADLOCK_DETECT_TIMER );

  /* Create Timer to delay an ERR_FATAL */
  rrctmr_int_data.rrc_t_delay_err_fatal_exp_ptr = &rrctmr_int_data.rrc_t_delay_err_fatal_exp;
  rex_def_timer_ex( rrctmr_int_data.rrc_t_delay_err_fatal_exp_ptr,
                    cb_ptr,
                   (uint32) RRCTMR_DELAY_ERR_FATAL_TIMER );

  /* Create Timer during Cell Selection, going DCH->PCH */
  rrctmr_int_data.rrc_t_dch_pch_cell_selection_exp_ptr = &rrctmr_int_data.rrc_t_dch_pch_cell_selection_exp;
  rex_def_timer_ex( rrctmr_int_data.rrc_t_dch_pch_cell_selection_exp_ptr,
                    cb_ptr,
                 (uint32)   RRCTMR_DCH_PCH_CELL_SELECTION_TIMER );

  /* Create Timer for Wait time */
  rrctmr_int_data.rrc_t_phy_chan_failure_ptr = &rrctmr_int_data.rrc_t_phy_chan_failure;
  rex_def_timer_ex( rrctmr_int_data.rrc_t_phy_chan_failure_ptr,
                    cb_ptr,
                 (uint32)   RRCTMR_PHY_CHAN_FAILURE_TIMER );

  /* Create Timer T3174 */
  rrctmr_int_data.rrc_t_3174_ptr = &rrctmr_int_data.rrc_t_3174;
  rex_def_timer_ex( rrctmr_int_data.rrc_t_3174_ptr,
                    cb_ptr,
                  (uint32)  RRCTMR_T_3174_TIMER );

  /* Create Timer for frequency scan */
  rrctmr_int_data.rrc_t_freq_scan_exp_ptr = &rrctmr_int_data.rrc_t_freq_scan_exp;
  rex_def_timer_ex( rrctmr_int_data.rrc_t_freq_scan_exp_ptr,
                    cb_ptr,
                 (uint32)   RRCTMR_FREQ_SCAN_TIMER );

  /* Create Timer for frequency scan */
  rrctmr_int_data.rrc_t_oos_ptr = &rrctmr_int_data.rrc_t_oos;
  rex_def_timer_ex( rrctmr_int_data.rrc_t_oos_ptr,
                    cb_ptr,
                    (uint32)RRCTMR_T_OOS_TIMER );

  /* Create Timer for returning no service if consecutive service requests
   * are received in deep sleep before this timer expires.
   */
  rrctmr_int_data.rrc_t_deep_sleep_no_svc_ptr = &rrctmr_int_data.rrc_t_deep_sleep_no_svc;
  rex_def_timer_ex( rrctmr_int_data.rrc_t_deep_sleep_no_svc_ptr,
                    cb_ptr,
                    (uint32)RRCTMR_DEEP_SLEEP_NO_SVC_TIMER );

   /* Create Timer for BPLMN full freq scan */
  rrctmr_int_data.rrc_t_bplmn_srch_ptr = &rrctmr_int_data.rrc_t_bplmn_srch;
  rex_def_timer_ex( rrctmr_int_data.rrc_t_bplmn_srch_ptr,
                    cb_ptr,
                    (uint32)RRCTMR_BPLMN_SRCH_TIMER );

  /* Create Timer for BPLMN full frequency scan */
  rrctmr_int_data.rrc_t_bplmn_full_scan_srch_ptr = &rrctmr_int_data.rrc_t_bplmn_full_scan_srch;
  rex_def_timer_ex( rrctmr_int_data.rrc_t_bplmn_full_scan_srch_ptr,
                    cb_ptr,
                    (uint32)RRCTMR_BPLMN_FREQ_SCAN_TIMER );

  /* Create Timer for Manual search full frequency scan */
  rrctmr_int_data.rrc_t_manual_plmn_full_scan_srch_ptr = &rrctmr_int_data.rrc_t_manual_plmn_full_scan_srch;
  rex_def_timer_ex( rrctmr_int_data.rrc_t_manual_plmn_full_scan_srch_ptr,
                    cb_ptr,
                    (uint32)RRCTMR_MANUAL_PLMN_FREQ_SCAN_TIMER );


  /* Create Timer for Freq Redirect Wait Timer */
  rrctmr_int_data.rrc_t_freq_redirect_wait_timer_ptr = &rrctmr_int_data.rrc_t_freq_redirect_wait_timer;
  rex_def_timer_ex( rrctmr_int_data.rrc_t_freq_redirect_wait_timer_ptr,
                    cb_ptr,
                    (uint32)RRCTMR_FREQ_REDIRECT_WAIT_TIMER );

  /* Create Timer for OOS in Connected Mode handling */
  rrctmr_int_data.rrc_t_conn_mode_oos_timer_ptr = &rrctmr_int_data.rrc_t_conn_mode_oos_timer;
  rex_def_timer_ex( rrctmr_int_data.rrc_t_conn_mode_oos_timer_ptr,
                    cb_ptr,
                    (uint32)RRCTMR_CONN_MODE_OOS_TIMER );

#ifdef FEATURE_INACTIVITY_HANDLING
  rrctmr_int_data.rrc_t_inactivity_timer_ptr = &rrctmr_int_data.rrc_t_inactivity_timer;
  rex_def_timer_ex( rrctmr_int_data.rrc_t_inactivity_timer_ptr,
                    cb_ptr,
                    (uint32)RRCTMR_INACTIVITY_TIMER );
#endif /* FEATURE_INTACTIVITY_HANDLING */

  /* Create Timer for WTOW BPLMN drx search */
  rrctmr_int_data.rrc_t_wtow_bplmn_srch_ptr = &rrctmr_int_data.rrc_t_wtow_bplmn_srch;
  rrc_def_non_defer_timer( rrctmr_int_data.rrc_t_wtow_bplmn_srch_ptr,
                    cb_ptr,
                    (uint32)RRCTMR_WTOW_BPLMN_DRX_SRCH_TIMER );

  /* Create Timer for WTOW/WTOG MAX BPLMN allowed search */
  rrctmr_int_data.rrc_t_wtow_bplmn_guard_srch_ptr = &rrctmr_int_data.rrc_t_wtow_bplmn_guard_srch;
  rex_def_timer_ex( rrctmr_int_data.rrc_t_wtow_bplmn_guard_srch_ptr,
                    cb_ptr,
                    (uint32)RRCTMR_BPLMN_GUARD_SRCH_TIMER );

  /* Create Timer for Updating SIX Hours SIB Clean Up Timer*/
  rrctmr_int_data.rrc_sixhr_sib_update_timer_ptr = &rrctmr_int_data.rrc_sixhr_sib_update_timer;
  rex_def_timer_ex( rrctmr_int_data.rrc_sixhr_sib_update_timer_ptr,
                    cb_ptr,
                    (uint32)RRCTMR_SIXHR_SIB_UPDATE_TIMER);

  rrctmr_int_data.rrc_tenmin_sib_update_timer_ptr = &rrctmr_int_data.rrc_tenmin_sib_update_timer;
  rex_def_timer_ex( rrctmr_int_data.rrc_tenmin_sib_update_timer_ptr,
                    cb_ptr,
                    (uint32)RRCTMR_TENMIN_SIB_UPDATE_TIMER);

  /* Create Timer for Deep Sleep OOS in Connected Mode handling */
  rrctmr_int_data.rrc_t_conn_mode_oos_initial_no_ds_timer_ptr = &rrctmr_int_data.rrc_t_conn_mode_oos_initial_no_ds_timer;
  rex_def_timer_ex( rrctmr_int_data.rrc_t_conn_mode_oos_initial_no_ds_timer_ptr,
                    cb_ptr,
                    (uint32)RRCTMR_CONN_MODE_OOS_INITIAL_NO_DS_TIMER );

  /* Create Timer for Deep Sleep OOS in Connected Mode handling */
  rrctmr_int_data.rrc_t_conn_mode_oos_ds_timer_ptr = &rrctmr_int_data.rrc_t_conn_mode_oos_ds_timer;
  rex_def_timer_ex( rrctmr_int_data.rrc_t_conn_mode_oos_ds_timer_ptr,
                    cb_ptr,
                    (uint32)RRCTMR_CONN_MODE_OOS_DS_TIMER );


  /* Create Timer for SIB CHANGE GUARD Timer*/
  rrctmr_int_data.rrc_sib_change_guard_timer_ptr = &rrctmr_int_data.rrc_sib_change_guard_timer;
  rex_def_timer_ex( rrctmr_int_data.rrc_sib_change_guard_timer_ptr,
                    cb_ptr,
                    (uint32)RRC_SIB_CHANGE_GUARD_TIMER);

  /* Create Timer for BCCH Modification Time*/
  rrctmr_int_data.rrc_bcch_mod_timer_ptr = &rrctmr_int_data.rrc_bcch_mod_timer;
  rex_def_timer_ex( rrctmr_int_data.rrc_bcch_mod_timer_ptr,
                    cb_ptr,
                    (uint32)RRC_BCCH_MODIFICATION_TIMER);

  /* Create Timer for GtoW Redirection handling */
  rrctmr_int_data.rrc_t_gtow_redirection_timer_ptr = &rrctmr_int_data.rrc_t_gtow_redirection_timer;
  rex_def_timer_ex( rrctmr_int_data.rrc_t_gtow_redirection_timer_ptr,
                    cb_ptr,
                    (uint32)RRCTMR_GTOW_REDIRECTION_WAIT_TIMER );

  rrctmr_int_data.rrc_t_gtow_redir_lfs_timer_ptr = &rrctmr_int_data.rrc_t_gtow_redir_lfs_timer;
  rex_def_timer_ex( rrctmr_int_data.rrc_t_gtow_redir_lfs_timer_ptr,
                    cb_ptr,
                    (uint32)RRCTMR_GTOW_REDIRECTION_LFS_TIMER );


  rrctmr_int_data.rrc_t_wtog_abort_guard_timer_ptr = &rrctmr_int_data.rrc_t_wtog_abort_guard_timer;
  rex_def_timer_ex( rrctmr_int_data.rrc_t_wtog_abort_guard_timer_ptr,
                    cb_ptr,
                    (uint32)RRCTMR_WTOG_ABORT_GUARD_TIMER );


  rrctmr_int_data.rrc_t_319_ptr = &rrctmr_int_data.rrc_t_319;
  rex_def_timer_ex( rrctmr_int_data.rrc_t_319_ptr,
                    cb_ptr,
                    (uint32)RRCTMR_T_319_TIMER);


  rrctmr_int_data.rrc_t_320_ptr = &rrctmr_int_data.rrc_t_320;
  rex_def_timer_ex( rrctmr_int_data.rrc_t_320_ptr,
                    cb_ptr,
                    (uint32)RRCTMR_T_320_TIMER);
  
#ifdef FEATURE_WCDMA_REL7_SPECRS
 
  rrctmr_int_data.rrc_t_couec_timer_ptr = &rrctmr_int_data.rrc_t_couec_timer;
  rex_def_timer_ex( rrctmr_int_data.rrc_t_couec_timer_ptr,
                    cb_ptr,
                    (uint32)RRCTMR_COUEC_TIMER);
#endif /*FEATURE_WCDMA_REL7_SPECRS*/

#ifdef FEATURE_DUAL_SIM
  rrctmr_int_data.rrc_wrm_lock_timer_ptr = &rrctmr_int_data.rrc_wrm_lock_timer;
  rex_def_timer_ex( rrctmr_int_data.rrc_wrm_lock_timer_ptr,
                    cb_ptr,
                    (uint32)RRCTMR_WRM_LOCK_TIMER);

  rrctmr_int_data.rrc_wrm_delay_preemption_timer_ptr = &rrctmr_int_data.rrc_wrm_delay_preemption_timer;
  rex_def_timer_ex( rrctmr_int_data.rrc_wrm_delay_preemption_timer_ptr,
                    cb_ptr,
                    (uint32)RRCTMR_WRM_DELAY_PREEMPTION_TIMER);


  rrctmr_int_data.rrc_dsim_bplmn_lock_rel_off_timer_ptr = &rrctmr_int_data.rrc_dsim_bplmn_lock_rel_off_timer;
  rrc_def_non_defer_timer( rrctmr_int_data.rrc_dsim_bplmn_lock_rel_off_timer_ptr,
                    cb_ptr, 
                    (uint32)RRCTMR_DSIM_BPLMN_LOCK_REL_OFFSET_TIMER);

  rrctmr_int_data.rrc_dsim_wrm_unlock_by_timer_ptr = &rrctmr_int_data.rrc_dsim_wrm_unlock_by_timer;
  rrc_def_non_defer_timer( rrctmr_int_data.rrc_dsim_wrm_unlock_by_timer_ptr,
                    cb_ptr, 
                    (uint32)RRCTMR_WRM_UNLOCK_BY_TIMER);

  rrctmr_int_data.rrc_max_ta_duration_timer_ptr = &rrctmr_int_data.rrc_max_ta_duration_timer;
  rex_def_timer_ex( rrctmr_int_data.rrc_max_ta_duration_timer_ptr,
                    cb_ptr, 
                    (uint32)RRCTMR_MAX_TA_DURATION_TIMER);

  rrctmr_int_data.rrc_t_trm_prio_change_timer_ptr= &rrctmr_int_data.rrc_t_trm_prio_change_timer;
  rex_def_timer_ex( rrctmr_int_data.rrc_t_trm_prio_change_timer_ptr,
                    cb_ptr,
                    (uint32) RRCTMR_TRM_PRIORITY_CHANGE_TIMER);

  rrctmr_int_data.rrc_t_acq_duty_cycle_change_timer_ptr= &rrctmr_int_data.rrc_t_acq_duty_cycle_change_timer;
  rex_def_timer_ex( rrctmr_int_data.rrc_t_acq_duty_cycle_change_timer_ptr,
	                cb_ptr,
				    (uint32) RRCTMR_ACQ_DTY_CYC_CHANGE_TIMER);


  rrctmr_int_data.rrc_t_sib_stop_part_ptr = &rrctmr_int_data.rrc_t_sib_stop_part;
  rex_def_timer_ex( rrctmr_int_data.rrc_t_sib_stop_part_ptr,
                    cb_ptr,
                   (uint32) RRCTMR_SIB_STOP_PARTITION_TIMER );

  rrctmr_int_data.rrc_t_sib_poke_timer_ptr= &rrctmr_int_data.rrc_t_sib_poke_timer;
  rex_def_timer_ex( rrctmr_int_data.rrc_t_sib_poke_timer_ptr,
                    NULL,
                    (uint32)RRCTMR_SIB_POKE_TIMER );

  rrctmr_int_data.rrc_t_delay_lta_timer_ptr= &rrctmr_int_data.rrc_t_delay_lta_timer;
  rex_def_timer_ex( rrctmr_int_data.rrc_t_delay_lta_timer_ptr,
                    NULL,
                    (uint32)RRCTMR_DELAY_LTA_TIMER );

  rrctmr_int_data.rrc_t_allow_lta_during_access_prio_timer_ptr= &rrctmr_int_data.rrc_t_allow_lta_during_access_prio_timer;
  rex_def_timer_ex( rrctmr_int_data.rrc_t_allow_lta_during_access_prio_timer_ptr,
                    NULL,
                    (uint32)RRCTMR_ALLOW_LTA_DURING_ACCESS_PRIO_TIMER );

#endif

  rrctmr_int_data.rrc_t_323_ptr = &rrctmr_int_data.rrc_t_323;
  rex_def_timer_ex( rrctmr_int_data.rrc_t_323_ptr,
                    cb_ptr,
                    (uint32)RRCTMR_T_323_TIMER);

#ifdef FEATURE_WCDMA_ABSOLUTE_PRIORITY
  rrctmr_int_data.rrc_t_ded_pri_validity_timer_ptr = &rrctmr_int_data.rrc_t_ded_pri_validity_timer;
  rex_def_timer_ex( rrctmr_int_data.rrc_t_ded_pri_validity_timer_ptr,
                    cb_ptr,
                    (uint32)RRCTMR_DED_PRI_VALIDITY_TIMER);
#endif

#ifdef FEATURE_QCHAT
  rrctmr_int_data.qchat_fa_rrc_timer_ptr = &rrctmr_int_data.qchat_fa_rrc_timer;
  rex_def_timer_ex( rrctmr_int_data.qchat_fa_rrc_timer_ptr,
                    cb_ptr,
                    (uint32)RRCTMR_QCHAT_FA_RRC_TIMER);
#endif
#ifdef FEATURE_LTE_TO_WCDMA
  rrctmr_int_data.rrc_t_lte_to_wcdma_redir_search_timer_ptr = &rrctmr_int_data.rrc_t_lte_to_wcdma_redir_search_timer;
  rex_def_timer_ex( rrctmr_int_data.rrc_t_lte_to_wcdma_redir_search_timer_ptr,
                    cb_ptr,
                    (uint32)RRCTMR_LTE_TO_WCDMA_REDIR_SRCH_TIMER);
#endif

#ifdef FEATURE_FEMTO_CSG
  rrctmr_int_data.rrc_t_csg_asf_srch_timer_ptr = &rrctmr_int_data.rrc_t_csg_asf_srch_timer;
  rex_def_timer_ex( rrctmr_int_data.rrc_t_csg_asf_srch_timer_ptr,
                    cb_ptr,
                    (uint32)RRCTMR_CSG_ASF_SRCH_TIMER);
#endif

  rrctmr_int_data.rrc_t_rrccu_trans_to_disconn_debug_timer_ptr= &rrctmr_int_data.rrc_t_rrccu_trans_to_disconn_debug_timer;
  rex_def_timer_ex( rrctmr_int_data.rrc_t_rrccu_trans_to_disconn_debug_timer_ptr,
                    cb_ptr,
                    (uint32)RRCTMR_RRCCU_TRANS_TO_DISCON_TIMER );

  /* Create Timer for BPLMN Reporting*/
  rrctmr_int_data.rrc_t_bplmn_partial_report_timer_ptr = &rrctmr_int_data.rrc_t_bplmn_partial_report_timer;
  rex_def_timer_ex( rrctmr_int_data.rrc_t_bplmn_partial_report_timer_ptr,
                    cb_ptr,
                    (uint32)RRCTMR_BPLMN_PARTIAL_REPORT_TIMER );

  rrctmr_int_data.rrc_t_rrc_waiting_for_L2_ack_for_pch_ptr= &rrctmr_int_data.rrc_t_rrc_waiting_for_L2_ack_for_pch;
  rex_def_timer_ex( rrctmr_int_data.rrc_t_rrc_waiting_for_L2_ack_for_pch_ptr,
                    cb_ptr,
                    (uint32)RRCTMR_WAIT_FOR_L2_ACK_FOR_PCH_TRANSITION_TIMER );

  rrctmr_int_data.rrc_t_rrc_block_lta_ptr = &rrctmr_int_data.rrc_t_rrc_block_lta;
  rex_def_timer_ex( rrctmr_int_data.rrc_t_rrc_block_lta_ptr,
                    cb_ptr,
                    (uint32)RRCTMR_BLOCK_LTA_TIMER );

#ifdef FEATURE_WCDMA_ALLOW_LTA_DURING_ACT_TIME
  rrctmr_int_data.rrc_t_rrc_dch_activation_time_buffer_ota_ptr= &rrctmr_int_data.rrc_t_rrc_dch_activation_time_buffer_ota;
  rex_def_timer_ex( rrctmr_int_data.rrc_t_rrc_dch_activation_time_buffer_ota_ptr,
                    cb_ptr,
                  (uint32)RRCTMR_OTA_BUFFER_ACTIVATION_TIMER );
#endif  /*FEATURE_WCDMA_ALLOW_LTA_DURING_ACT_TIME*/
#ifdef FEATURE_WCDMA_W2L_PSEUDO_MEAS

  rrctmr_int_data.rrc_pseudo_meas_timer_ptr= &rrctmr_int_data.rrc_pseudo_meas_timer;
  rrc_def_non_defer_timer( rrctmr_int_data.rrc_pseudo_meas_timer_ptr,
                           cb_ptr,
                           (uint32)RRCTMR_PSEUDO_MEAS_TIMER);
#endif
#ifdef ELEVATOR_TRIGGER
  rrctmr_int_data.rrc_pseudo_meas_rxagc_timer_ptr= &rrctmr_int_data.rrc_pseudo_meas_rxagc_timer;
  rrc_def_non_defer_timer( rrctmr_int_data.rrc_pseudo_meas_rxagc_timer_ptr,
                           cb_ptr,
                               (uint32)RRCTMR_PSEUDO_MEAS_RXAGC_TIMER);
#endif
/* create a timer for allowing forbiddern earfcn for pseudo measuremnet */
#ifdef FEATURE_WCDMA_W2L_PSEUDO_MEAS
  rrctmr_int_data.rrc_t_allow_earfcn_for_pseudo_meas_timer_ptr = &rrctmr_int_data.rrc_t_allow_earfcn_for_pseudo_meas_timer;
  rex_def_timer_ex(rrctmr_int_data.rrc_t_allow_earfcn_for_pseudo_meas_timer_ptr,cb_ptr,(uint32)RRCTMR_ALLOW_EARFCN_FOR_PSEUDO_MEAS);
#endif

  rrctmr_int_data.rrc_t_delay_wtol_resel_timer_ptr = &rrctmr_int_data.rrc_t_delay_wtol_resel_timer;
  rex_def_timer_ex( rrctmr_int_data.rrc_t_delay_wtol_resel_timer_ptr,
        cb_ptr,
        (uint32)RRCTMR_DELAY_WTOL_RESEL_TIMER);

  rrctmr_int_data.rrc_t_cell_depri_timer_ptr = &rrctmr_int_data.rrc_t_cell_depri_timer;
  rex_def_timer_ex( rrctmr_int_data.rrc_t_cell_depri_timer_ptr,
                    cb_ptr,
                    (uint32)RRCTMR_CELL_DEPRI_TIMER );

  rrctmr_int_data.rrc_t_cell_depri_nbr_timer_ptr = &rrctmr_int_data.rrc_t_cell_depri_nbr_timer;
  rex_def_timer_ex( rrctmr_int_data.rrc_t_cell_depri_nbr_timer_ptr,
                     cb_ptr,
                       (uint32)RRCTMR_CELL_DEPRI_NBR_TIMER );
  
} /* rrctmr_create_timers */

/***/ __WCDMA_RRC_CODE_SEGMENT_NON_DEMANDPAGED_END__ /***/

/*===========================================================================

FUNCTION rrctmr_start_timer

DESCRIPTION
  This function will set the timer value in milli seconds and starts the timer
  timers.
DEPENDENCIES
  None

RETURN VALUE
  None
SIDE EFFECTS
  None
===========================================================================*/
/***/ __WCDMA_RRC_CODE_SEGMENT_NON_DEMANDPAGED__ /***/
void rrctmr_start_timer
(
  sys_modem_as_id_e_type wrrc_as_id, 
  rrctmr_timer_e_type timer,                /* Timer to start */
  unsigned long timer_value                 /* Value for timer */
)
{

#ifdef FEATURE_FEMTO_CSG
  rrclog_wcdma_asf_timer_status_event_type asf_timer_status;
#endif

#ifdef FEATURE_TIMER_DEBUG
  #error code not present
#endif

  switch(timer)
  {
    case RRCTMR_T_300_TIMER:
      rex_set_timer(rrctmr_int_data.rrc_t_300_ptr, timer_value); 
      break;
    case RRCTMR_T_308_TIMER:
      rex_set_timer(rrctmr_int_data.rrc_t_308_ptr, timer_value); 
      break;
    case RRCTMR_T_304_TIMER:
      rex_set_timer(rrctmr_int_data.rrc_t_304_ptr, timer_value); 
      break;
    case RRCTMR_WAIT_TIME_TIMER:
      rex_set_timer(rrctmr_int_data.rrc_t_wait_time_ptr, timer_value);
      break;
    case RRCTMR_UE_TO_CAMP_ON_TIMER:
      rex_set_timer(rrctmr_int_data.rrc_t_ue_to_camp_on_ptr, timer_value);
      break;
    case RRCTMR_RRC_CON_DELAY_TIMER:
      rex_set_timer(rrctmr_int_data.rrc_t_rrc_con_delay_ptr, timer_value);
      break;
    case RRCTMR_RCE_L2ACK_TIMER:
      rex_set_timer(rrctmr_int_data.rrc_t_rce_l2ack_ptr, timer_value);
      break;
    case RRCTMR_RCR_L2ACK_TIMER:
      rex_set_timer(rrctmr_int_data.rrc_t_rcr_l2ack_ptr, timer_value);
      break;
    case RRCTMR_RSSI_BER_TIMER:
      rex_set_timer(rrctmr_int_data.rrc_t_rssi_ber_ptr, timer_value);
      break;
    case RRCTMR_SIB_WAIT_TIMER:
      rex_set_timer(rrctmr_int_data.rrc_t_sib_wait_ptr, timer_value);
      break;
    /*Case to hanndle SIB 7 expiration*/
    case RRCTMR_SIB7_EXPIRATION_TIMER:
      rex_set_timer(rrctmr_int_data.rrc_t_sib7_expiration_ptr, timer_value);
      break;

    case RRCTMR_T_302_TIMER:
      rex_set_timer(rrctmr_int_data.rrc_t_302_ptr, timer_value); 
      break;
    case RRCTMR_T_305_TIMER:
      rex_set_timer(rrctmr_int_data.rrc_t_305_ptr, timer_value); 
      break;
    case RRCTMR_T_316_TIMER:
      rex_set_timer(rrctmr_int_data.rrc_t_316_ptr, timer_value); 
      break;
    case RRCTMR_T_317_TIMER:
      rex_set_timer(rrctmr_int_data.rrc_t_317_ptr, timer_value); 
      break;
    case RRCTMR_T_307_TIMER:
      rex_set_timer(rrctmr_int_data.rrc_t_307_ptr, timer_value); 
      break;
    case RRCTMR_T_314_TIMER:
      rex_set_timer(rrctmr_int_data.rrc_t_314_ptr, timer_value); 
      break;
    case RRCTMR_T_315_TIMER:
      rex_set_timer(rrctmr_int_data.rrc_t_315_ptr, timer_value); 
      break;
    case RRCTMR_SIB7_EXP_TIMER:
      rex_set_timer(rrctmr_int_data.rrc_t_sib7_exp_ptr, timer_value); 
      break;
    case RRCTMR_IGNORE_BCCH_MOD_TIMER:
      rex_set_timer(rrctmr_int_data.rrc_t_ignore_bcch_mod_exp_ptr, timer_value); 
      break;
    case RRCTMR_DCH_FACH_CELL_SELECTION_TIMER:
      rex_set_timer(rrctmr_int_data.rrc_t_dch_fach_cell_selection_exp_ptr, timer_value); 
      break;
    case RRCTMR_FACH_TO_FACH_PCH_CELL_SELECTION_TIMER:
      rex_set_timer(rrctmr_int_data.rrc_t_fach_to_fach_pch_cell_selection_exp_ptr, timer_value); 
      break;
    case RRCTMR_DCH_PCH_CELL_SELECTION_TIMER:
      rex_set_timer(rrctmr_int_data.rrc_t_dch_pch_cell_selection_exp_ptr, timer_value); 
      break;
    case RRCTMR_L1_DEADLOCK_DETECT_TIMER:
      rex_set_timer(rrctmr_int_data.rrc_t_l1_deadlock_detection_exp_ptr, timer_value); 
      break;
    case RRCTMR_DELAY_ERR_FATAL_TIMER:
      rex_set_timer(rrctmr_int_data.rrc_t_delay_err_fatal_exp_ptr, timer_value); 
      break;
    case RRCTMR_T_3174_TIMER:
      rex_set_timer(rrctmr_int_data.rrc_t_3174_ptr, timer_value); 
      break;
    case RRCTMR_FREQ_SCAN_TIMER:
      rex_set_timer(rrctmr_int_data.rrc_t_freq_scan_exp_ptr, timer_value); 
      break;
    case RRCTMR_T_OOS_TIMER:
      rex_set_timer(rrctmr_int_data.rrc_t_oos_ptr, timer_value); 
      break;

    case RRCTMR_DEEP_SLEEP_NO_SVC_TIMER:
      rex_set_timer(rrctmr_int_data.rrc_t_deep_sleep_no_svc_ptr, timer_value); 
      break;                                   

    case RRCTMR_PHY_CHAN_FAILURE_TIMER:
      rex_set_timer(rrctmr_int_data.rrc_t_phy_chan_failure_ptr, timer_value);
      break;

    case RRCTMR_BPLMN_SRCH_TIMER:
      rex_set_timer(rrctmr_int_data.rrc_t_bplmn_srch_ptr, timer_value); 
      break;

    case RRCTMR_BPLMN_FREQ_SCAN_TIMER:
      rex_set_timer(rrctmr_int_data.rrc_t_bplmn_full_scan_srch_ptr, timer_value); 
      break;       

    case RRCTMR_MANUAL_PLMN_FREQ_SCAN_TIMER:
      rex_set_timer(rrctmr_int_data.rrc_t_manual_plmn_full_scan_srch_ptr, timer_value); 
      break;


    case RRCTMR_FREQ_REDIRECT_WAIT_TIMER:
      rex_set_timer(rrctmr_int_data.rrc_t_freq_redirect_wait_timer_ptr, timer_value); 
      break; 

    case RRCTMR_CONN_MODE_OOS_TIMER:
      rex_set_timer(rrctmr_int_data.rrc_t_conn_mode_oos_timer_ptr, timer_value); 
      break;       

#ifdef FEATURE_INACTIVITY_HANDLING

    case RRCTMR_INACTIVITY_TIMER:
      rex_set_timer(rrctmr_int_data.rrc_t_inactivity_timer_ptr, timer_value); 
      break;  

#endif /* FEATURE_INTACTIVITY_HANDLING */

    case RRCTMR_WTOW_BPLMN_DRX_SRCH_TIMER:
      rex_set_timer(rrctmr_int_data.rrc_t_wtow_bplmn_srch_ptr, timer_value); 
      break;
  
    case RRCTMR_BPLMN_GUARD_SRCH_TIMER:
      rex_set_timer(rrctmr_int_data.rrc_t_wtow_bplmn_guard_srch_ptr, timer_value); 
      break;

    case RRCTMR_SIXHR_SIB_UPDATE_TIMER:
      rex_set_timer(rrctmr_int_data.rrc_sixhr_sib_update_timer_ptr, timer_value); 
      break;
      
    case RRCTMR_TENMIN_SIB_UPDATE_TIMER:
      rex_set_timer(rrctmr_int_data.rrc_tenmin_sib_update_timer_ptr, timer_value);
      break;

    case RRCTMR_CONN_MODE_OOS_INITIAL_NO_DS_TIMER:
      rex_set_timer(rrctmr_int_data.rrc_t_conn_mode_oos_initial_no_ds_timer_ptr, timer_value); 
      break;       

    case RRCTMR_CONN_MODE_OOS_DS_TIMER:
      rex_set_timer(rrctmr_int_data.rrc_t_conn_mode_oos_ds_timer_ptr, timer_value); 
      break;       

    case RRC_SIB_CHANGE_GUARD_TIMER:
      rex_set_timer(rrctmr_int_data.rrc_sib_change_guard_timer_ptr, timer_value); 
      break; 

    case RRC_BCCH_MODIFICATION_TIMER:
      rex_set_timer(rrctmr_int_data.rrc_bcch_mod_timer_ptr, timer_value); 
      break;

    case RRCTMR_GTOW_REDIRECTION_WAIT_TIMER:
      rex_set_timer(rrctmr_int_data.rrc_t_gtow_redirection_timer_ptr, timer_value); 
      break;

    case RRCTMR_GTOW_REDIRECTION_LFS_TIMER:
      rex_set_timer(rrctmr_int_data.rrc_t_gtow_redir_lfs_timer_ptr, timer_value); 
      break;

    case RRCTMR_WTOG_ABORT_GUARD_TIMER:
      rex_set_timer(rrctmr_int_data.rrc_t_wtog_abort_guard_timer_ptr, timer_value); 
      break;


    case RRCTMR_T_319_TIMER:
      rex_set_timer(rrctmr_int_data.rrc_t_319_ptr, timer_value); 
      break;

    case RRCTMR_T_320_TIMER:
      rex_set_timer(rrctmr_int_data.rrc_t_320_ptr, timer_value);
      break;

#ifdef FEATURE_WCDMA_REL7_SPECRS

    case RRCTMR_COUEC_TIMER:
      rex_set_timer(rrctmr_int_data.rrc_t_couec_timer_ptr, timer_value);
      break;
#endif /*FEATURE_WCDMA_REL7_SPECRS*/


    case RRCTMR_T_323_TIMER:
      rex_set_timer(rrctmr_int_data.rrc_t_323_ptr, timer_value);
      break;

#ifdef FEATURE_DUAL_SIM
    case RRCTMR_WRM_LOCK_TIMER:
      rex_set_timer(rrctmr_int_data.rrc_wrm_lock_timer_ptr, timer_value);
      break;

    case RRCTMR_WRM_DELAY_PREEMPTION_TIMER:
      rex_set_timer(rrctmr_int_data.rrc_wrm_delay_preemption_timer_ptr, timer_value);
      break;

    case RRCTMR_DSIM_BPLMN_LOCK_REL_OFFSET_TIMER:
      rrc_set_non_defer_timer(rrctmr_int_data.rrc_dsim_bplmn_lock_rel_off_timer_ptr,timer_value);
      break;

    case RRCTMR_WRM_UNLOCK_BY_TIMER:
      rrc_set_non_defer_timer(rrctmr_int_data.rrc_dsim_wrm_unlock_by_timer_ptr, timer_value);
      break;

    case RRCTMR_MAX_TA_DURATION_TIMER:
      rex_set_timer(rrctmr_int_data.rrc_max_ta_duration_timer_ptr, timer_value);
      break;

    case RRCTMR_SIB_POKE_TIMER:
      rex_set_timer(rrctmr_int_data.rrc_t_sib_poke_timer_ptr, timer_value);
      break;

    case RRCTMR_TRM_PRIORITY_CHANGE_TIMER:
      rex_set_timer(rrctmr_int_data.rrc_t_trm_prio_change_timer_ptr, timer_value);
      break;

    case RRCTMR_ACQ_DTY_CYC_CHANGE_TIMER:
      rex_set_timer(rrctmr_int_data.rrc_t_acq_duty_cycle_change_timer_ptr, timer_value);
      break;

    case RRCTMR_SIB_STOP_PARTITION_TIMER:
      rex_set_timer(rrctmr_int_data.rrc_t_sib_stop_part_ptr, timer_value);
      break;

    case RRCTMR_DELAY_LTA_TIMER:
      rex_set_timer(rrctmr_int_data.rrc_t_delay_lta_timer_ptr, timer_value);
      break;

    case RRCTMR_ALLOW_LTA_DURING_ACCESS_PRIO_TIMER:
      rex_set_timer(rrctmr_int_data.rrc_t_allow_lta_during_access_prio_timer_ptr, timer_value);
      break;
#endif

#ifdef FEATURE_WCDMA_ABSOLUTE_PRIORITY
    case RRCTMR_DED_PRI_VALIDITY_TIMER:
      rex_set_timer(rrctmr_int_data.rrc_t_ded_pri_validity_timer_ptr,timer_value);
      break;
#endif
#ifdef FEATURE_QCHAT
    case RRCTMR_QCHAT_FA_RRC_TIMER:
      rex_set_timer(rrctmr_int_data.qchat_fa_rrc_timer_ptr, timer_value); 
      break;
#endif
#ifdef FEATURE_LTE_TO_WCDMA
    case RRCTMR_LTE_TO_WCDMA_REDIR_SRCH_TIMER:
      rex_set_timer(rrctmr_int_data.rrc_t_lte_to_wcdma_redir_search_timer_ptr,timer_value);
      break;
#endif

#ifdef FEATURE_FEMTO_CSG
    case RRCTMR_CSG_ASF_SRCH_TIMER:
      rex_set_timer(rrctmr_int_data.rrc_t_csg_asf_srch_timer_ptr, timer_value); 
      asf_timer_status.timer_value = timer_value;
      asf_timer_status.timer_state = 0;
      WRRC_EVENT_REPORT_PAYLOAD(EVENT_WCDMA_RRC_ASF_TIMER_STATUS, sizeof(rrclog_wcdma_asf_timer_status_event_type),
                     (void*)&asf_timer_status);
      break;
#endif
    case RRCTMR_RRCCU_TRANS_TO_DISCON_TIMER:
      rex_set_timer(rrctmr_int_data.rrc_t_rrccu_trans_to_disconn_debug_timer_ptr,timer_value);
      break;

    case RRCTMR_BPLMN_PARTIAL_REPORT_TIMER:
      rex_set_timer(rrctmr_int_data.rrc_t_bplmn_partial_report_timer_ptr, timer_value); 
      break;

    case RRCTMR_WAIT_FOR_L2_ACK_FOR_PCH_TRANSITION_TIMER:
      rex_set_timer(rrctmr_int_data.rrc_t_rrc_waiting_for_L2_ack_for_pch_ptr,timer_value);
      break;

    case RRCTMR_BLOCK_LTA_TIMER:
      rex_set_timer(rrctmr_int_data.rrc_t_rrc_block_lta_ptr,timer_value);
      break;
#ifdef FEATURE_WCDMA_ALLOW_LTA_DURING_ACT_TIME
    case RRCTMR_OTA_BUFFER_ACTIVATION_TIMER:
      rex_set_timer(rrctmr_int_data.rrc_t_rrc_dch_activation_time_buffer_ota_ptr,timer_value);
      break;
#endif  /*FEATURE_WCDMA_ALLOW_LTA_DURING_ACT_TIME*/
#ifdef FEATURE_WCDMA_W2L_PSEUDO_MEAS
    case RRCTMR_PSEUDO_MEAS_TIMER:
      rex_set_timer(rrctmr_int_data.rrc_pseudo_meas_timer_ptr,timer_value);
      break;
#endif
#ifdef ELEVATOR_TRIGGER
    case RRCTMR_PSEUDO_MEAS_RXAGC_TIMER:
      rex_set_timer(rrctmr_int_data.rrc_pseudo_meas_rxagc_timer_ptr,timer_value);
      break;
#endif
#ifdef FEATURE_WCDMA_W2L_PSEUDO_MEAS
    case RRCTMR_ALLOW_EARFCN_FOR_PSEUDO_MEAS:
      rex_set_timer(rrctmr_int_data.rrc_t_allow_earfcn_for_pseudo_meas_timer_ptr,timer_value);
      break;
#endif

    case RRCTMR_DELAY_WTOL_RESEL_TIMER:
      rex_set_timer(rrctmr_int_data.rrc_t_delay_wtol_resel_timer_ptr,timer_value);
      break;

    case RRCTMR_CELL_DEPRI_TIMER:
      rex_set_timer(rrctmr_int_data.rrc_t_cell_depri_timer_ptr,timer_value);
      break;

    case RRCTMR_CELL_DEPRI_NBR_TIMER:
      rex_set_timer(rrctmr_int_data.rrc_t_cell_depri_nbr_timer_ptr,timer_value);
      break;

    default:
      WRRC_MSG0_HIGH("Unsupported timer is attempted to start");
      break; 
  }
  WRRC_MSG2_HIGH("Started timer:rrctmr_timer_e_type_value%d val:%d ms",timer,timer_value);
}/* rrctmr_start_timer */
/***/ __WCDMA_RRC_CODE_SEGMENT_NON_DEMANDPAGED_END__ /***/
/*===========================================================================

FUNCTION rrctmr_stop_timer

DESCRIPTION
  This function will clears the timer value and stops the timer
  timers.
DEPENDENCIES
  None

RETURN VALUE
  None
SIDE EFFECTS
  None
===========================================================================*/
/***/ __WCDMA_RRC_CODE_SEGMENT_NON_DEMANDPAGED__ /***/
void rrctmr_stop_timer
(
  sys_modem_as_id_e_type wrrc_as_id, 
  rrctmr_timer_e_type timer                 /* Timer to stop */
)       
{
#ifdef FEATURE_FEMTO_CSG
  rrclog_wcdma_asf_timer_status_event_type asf_timer_status;
#endif
  /*To avoid printing in get remaning time func*/
  rrctmr_int_data.print_rem_msg = FALSE;
  if(rrctmr_get_remaining_time(wrrc_as_id,timer) == 0)
  {
    rrctmr_int_data.print_rem_msg = TRUE;
    return;
  }
  rrctmr_int_data.print_rem_msg = TRUE;

  switch(timer)
  {
    case RRCTMR_T_300_TIMER:
      rex_clr_timer(rrctmr_int_data.rrc_t_300_ptr);
      break;
    case RRCTMR_T_308_TIMER:
      rex_clr_timer(rrctmr_int_data.rrc_t_308_ptr);
      break;
    case RRCTMR_T_304_TIMER:
      rex_clr_timer(rrctmr_int_data.rrc_t_304_ptr); 
      break;
    case RRCTMR_WAIT_TIME_TIMER:
      rex_clr_timer(rrctmr_int_data.rrc_t_wait_time_ptr);
      break;
    case RRCTMR_UE_TO_CAMP_ON_TIMER:
      rex_clr_timer(rrctmr_int_data.rrc_t_ue_to_camp_on_ptr);
      break;
    case RRCTMR_RRC_CON_DELAY_TIMER:
      rex_clr_timer(rrctmr_int_data.rrc_t_rrc_con_delay_ptr);
      break;
    case RRCTMR_RCE_L2ACK_TIMER:
      rex_clr_timer(rrctmr_int_data.rrc_t_rce_l2ack_ptr);
      break;
    case RRCTMR_RCR_L2ACK_TIMER:
      rex_clr_timer(rrctmr_int_data.rrc_t_rcr_l2ack_ptr);
      break;
    case RRCTMR_RSSI_BER_TIMER:
      rex_clr_timer(rrctmr_int_data.rrc_t_rssi_ber_ptr);
      break;
    case RRCTMR_SIB_WAIT_TIMER:
      rex_clr_timer(rrctmr_int_data.rrc_t_sib_wait_ptr);
      break;
    /*Case to handle SIB 7 Expiration*/
    case RRCTMR_SIB7_EXPIRATION_TIMER:
      rex_clr_timer(rrctmr_int_data.rrc_t_sib7_expiration_ptr);
      break;

    case RRCTMR_T_302_TIMER:
      rex_clr_timer(rrctmr_int_data.rrc_t_302_ptr);
      break;
    case RRCTMR_T_305_TIMER:
      rex_clr_timer(rrctmr_int_data.rrc_t_305_ptr);
      break;
    case RRCTMR_T_316_TIMER:
      rex_clr_timer(rrctmr_int_data.rrc_t_316_ptr);
      break;
    case RRCTMR_T_317_TIMER:
      rex_clr_timer(rrctmr_int_data.rrc_t_317_ptr);
      break;
    case RRCTMR_T_307_TIMER:
      rex_clr_timer(rrctmr_int_data.rrc_t_307_ptr);
      break;
    case RRCTMR_T_314_TIMER:
      rex_clr_timer(rrctmr_int_data.rrc_t_314_ptr);
      break;
    case RRCTMR_T_315_TIMER:
      rex_clr_timer(rrctmr_int_data.rrc_t_315_ptr);
      break;
    case RRCTMR_SIB7_EXP_TIMER:
      rex_clr_timer(rrctmr_int_data.rrc_t_sib7_exp_ptr); 
      break;
    case RRCTMR_IGNORE_BCCH_MOD_TIMER:
      rex_clr_timer(rrctmr_int_data.rrc_t_ignore_bcch_mod_exp_ptr); 
      break;
    case RRCTMR_DCH_FACH_CELL_SELECTION_TIMER:
      rex_clr_timer(rrctmr_int_data.rrc_t_dch_fach_cell_selection_exp_ptr); 
      break;
    case RRCTMR_FACH_TO_FACH_PCH_CELL_SELECTION_TIMER:
      rex_clr_timer(rrctmr_int_data.rrc_t_fach_to_fach_pch_cell_selection_exp_ptr); 
      break;
    case RRCTMR_DCH_PCH_CELL_SELECTION_TIMER:
      rex_clr_timer(rrctmr_int_data.rrc_t_dch_pch_cell_selection_exp_ptr); 
      break;
    case RRCTMR_L1_DEADLOCK_DETECT_TIMER:
      rex_clr_timer(rrctmr_int_data.rrc_t_l1_deadlock_detection_exp_ptr); 
      break;
    case RRCTMR_DELAY_ERR_FATAL_TIMER:
      rex_clr_timer(rrctmr_int_data.rrc_t_delay_err_fatal_exp_ptr); 
      break;
    case RRCTMR_T_3174_TIMER:
      rex_clr_timer(rrctmr_int_data.rrc_t_3174_ptr);
      break;
    case RRCTMR_FREQ_SCAN_TIMER:
      rex_clr_timer(rrctmr_int_data.rrc_t_freq_scan_exp_ptr); 
      break;
    case RRCTMR_T_OOS_TIMER:
      rex_clr_timer(rrctmr_int_data.rrc_t_oos_ptr); 
      break;

    case RRCTMR_DEEP_SLEEP_NO_SVC_TIMER:
      rex_clr_timer(rrctmr_int_data.rrc_t_deep_sleep_no_svc_ptr); 
      break;

     case RRCTMR_PHY_CHAN_FAILURE_TIMER:
      rex_clr_timer(rrctmr_int_data.rrc_t_phy_chan_failure_ptr);
      break;  

    case RRCTMR_BPLMN_SRCH_TIMER:
      rex_clr_timer(rrctmr_int_data.rrc_t_bplmn_srch_ptr); 
      break;

    case RRCTMR_BPLMN_FREQ_SCAN_TIMER:
      rex_clr_timer(rrctmr_int_data.rrc_t_bplmn_full_scan_srch_ptr); 
      break;     

    case RRCTMR_MANUAL_PLMN_FREQ_SCAN_TIMER:
      rex_clr_timer(rrctmr_int_data.rrc_t_manual_plmn_full_scan_srch_ptr); 
      break;


    case RRCTMR_FREQ_REDIRECT_WAIT_TIMER:
      rex_clr_timer(rrctmr_int_data.rrc_t_freq_redirect_wait_timer_ptr); 
      break;   

    case RRCTMR_CONN_MODE_OOS_TIMER:
      rex_clr_timer(rrctmr_int_data.rrc_t_conn_mode_oos_timer_ptr);
      break;       

#ifdef FEATURE_INACTIVITY_HANDLING
    case RRCTMR_INACTIVITY_TIMER:
      rex_clr_timer(rrctmr_int_data.rrc_t_inactivity_timer_ptr);
      break;  
#endif /* FEATURE_INTACTIVITY_HANDLING */

    case RRCTMR_WTOW_BPLMN_DRX_SRCH_TIMER:
      rex_clr_timer(rrctmr_int_data.rrc_t_wtow_bplmn_srch_ptr); 
      break;
  
    case RRCTMR_BPLMN_GUARD_SRCH_TIMER:
      rex_clr_timer(rrctmr_int_data.rrc_t_wtow_bplmn_guard_srch_ptr); 
      break;

    case RRCTMR_SIXHR_SIB_UPDATE_TIMER:
      rex_clr_timer(rrctmr_int_data.rrc_sixhr_sib_update_timer_ptr);
     break;

    case RRCTMR_TENMIN_SIB_UPDATE_TIMER:
      rex_clr_timer(rrctmr_int_data.rrc_tenmin_sib_update_timer_ptr);
      break;

    case RRCTMR_CONN_MODE_OOS_INITIAL_NO_DS_TIMER:
      rex_clr_timer(rrctmr_int_data.rrc_t_conn_mode_oos_initial_no_ds_timer_ptr);
      break;       

    case RRCTMR_CONN_MODE_OOS_DS_TIMER:
      rex_clr_timer(rrctmr_int_data.rrc_t_conn_mode_oos_ds_timer_ptr);
      break;       

    case RRC_SIB_CHANGE_GUARD_TIMER:
      rex_clr_timer(rrctmr_int_data.rrc_sib_change_guard_timer_ptr); 
      break; 

    case RRC_BCCH_MODIFICATION_TIMER:
      rex_clr_timer(rrctmr_int_data.rrc_bcch_mod_timer_ptr); 
      break;

    case RRCTMR_GTOW_REDIRECTION_WAIT_TIMER:
      rex_clr_timer(rrctmr_int_data.rrc_t_gtow_redirection_timer_ptr);
      break;

    case RRCTMR_GTOW_REDIRECTION_LFS_TIMER:
      rex_clr_timer(rrctmr_int_data.rrc_t_gtow_redir_lfs_timer_ptr);
      break;

    case RRCTMR_WTOG_ABORT_GUARD_TIMER:
      rex_clr_timer(rrctmr_int_data.rrc_t_wtog_abort_guard_timer_ptr); 
      break;


    case RRCTMR_T_319_TIMER:
      rex_clr_timer(rrctmr_int_data.rrc_t_319_ptr);
      break;

    case RRCTMR_T_320_TIMER:
      rex_clr_timer(rrctmr_int_data.rrc_t_320_ptr);
      break;	 

#ifdef FEATURE_WCDMA_REL7_SPECRS
    case RRCTMR_COUEC_TIMER:
      rex_clr_timer(rrctmr_int_data.rrc_t_couec_timer_ptr);
      break;
#endif /*FEATURE_WCDMA_REL7_SPECRS*/

    case RRCTMR_T_323_TIMER:
      rex_clr_timer(rrctmr_int_data.rrc_t_323_ptr);   
      break;  

#ifdef FEATURE_DUAL_SIM
    case RRCTMR_WRM_LOCK_TIMER:
      rex_clr_timer(rrctmr_int_data.rrc_wrm_lock_timer_ptr);
      break;

    case RRCTMR_WRM_DELAY_PREEMPTION_TIMER:
      rex_clr_timer(rrctmr_int_data.rrc_wrm_delay_preemption_timer_ptr);
      break;

    case RRCTMR_DSIM_BPLMN_LOCK_REL_OFFSET_TIMER:
      rrc_clr_non_defer_timer(rrctmr_int_data.rrc_dsim_bplmn_lock_rel_off_timer_ptr);
      break;

    case RRCTMR_WRM_UNLOCK_BY_TIMER:
      rrc_clr_non_defer_timer(rrctmr_int_data.rrc_dsim_wrm_unlock_by_timer_ptr);
      break;

    case RRCTMR_MAX_TA_DURATION_TIMER:
      rex_clr_timer(rrctmr_int_data.rrc_max_ta_duration_timer_ptr);
      break;

    case RRCTMR_SIB_POKE_TIMER:
      rex_clr_timer(rrctmr_int_data.rrc_t_sib_poke_timer_ptr);

    case RRCTMR_TRM_PRIORITY_CHANGE_TIMER:
      rex_clr_timer(rrctmr_int_data.rrc_t_trm_prio_change_timer_ptr);
      break;

    case RRCTMR_ACQ_DTY_CYC_CHANGE_TIMER:
      rex_clr_timer(rrctmr_int_data.rrc_t_acq_duty_cycle_change_timer_ptr);
      break;

    case RRCTMR_SIB_STOP_PARTITION_TIMER:
      rex_clr_timer(rrctmr_int_data.rrc_t_sib_stop_part_ptr);
      break;

    case RRCTMR_DELAY_LTA_TIMER:
      rex_clr_timer(rrctmr_int_data.rrc_t_delay_lta_timer_ptr);
      break;
      
    case RRCTMR_ALLOW_LTA_DURING_ACCESS_PRIO_TIMER:
      rex_clr_timer(rrctmr_int_data.rrc_t_allow_lta_during_access_prio_timer_ptr);
      break;
#endif

#ifdef FEATURE_WCDMA_ABSOLUTE_PRIORITY
    case RRCTMR_DED_PRI_VALIDITY_TIMER:
      rex_clr_timer(rrctmr_int_data.rrc_t_ded_pri_validity_timer_ptr);
      break;
#endif
#ifdef FEATURE_QCHAT
    case RRCTMR_QCHAT_FA_RRC_TIMER:
      rex_clr_timer(rrctmr_int_data.qchat_fa_rrc_timer_ptr); 
      break; 
#endif
#ifdef FEATURE_LTE_TO_WCDMA
    case RRCTMR_LTE_TO_WCDMA_REDIR_SRCH_TIMER:
      rex_clr_timer(rrctmr_int_data.rrc_t_lte_to_wcdma_redir_search_timer_ptr);
      break;
#endif

#ifdef FEATURE_FEMTO_CSG
    case RRCTMR_CSG_ASF_SRCH_TIMER:
      rex_clr_timer(rrctmr_int_data.rrc_t_csg_asf_srch_timer_ptr);
      asf_timer_status.timer_value = 0;
      asf_timer_status.timer_state = 1;
      WRRC_EVENT_REPORT_PAYLOAD(EVENT_WCDMA_RRC_ASF_TIMER_STATUS, sizeof(rrclog_wcdma_asf_timer_status_event_type),
                     (void*)&asf_timer_status);
      break;
#endif
    case RRCTMR_RRCCU_TRANS_TO_DISCON_TIMER:
      rex_clr_timer(rrctmr_int_data.rrc_t_rrccu_trans_to_disconn_debug_timer_ptr);
      break;

    case RRCTMR_BPLMN_PARTIAL_REPORT_TIMER:
      rex_clr_timer(rrctmr_int_data.rrc_t_bplmn_partial_report_timer_ptr); 
      break;      
     
    case RRCTMR_WAIT_FOR_L2_ACK_FOR_PCH_TRANSITION_TIMER:
      rex_clr_timer(rrctmr_int_data.rrc_t_rrc_waiting_for_L2_ack_for_pch_ptr);
      break;     

    case RRCTMR_BLOCK_LTA_TIMER:
      rex_clr_timer(rrctmr_int_data.rrc_t_rrc_block_lta_ptr);
      break;
#ifdef FEATURE_WCDMA_ALLOW_LTA_DURING_ACT_TIME
    case RRCTMR_OTA_BUFFER_ACTIVATION_TIMER:
      rex_clr_timer(rrctmr_int_data.rrc_t_rrc_dch_activation_time_buffer_ota_ptr);
      break;    
#endif  /*FEATURE_WCDMA_ALLOW_LTA_DURING_ACT_TIME*/
#ifdef FEATURE_WCDMA_W2L_PSEUDO_MEAS
      case RRCTMR_PSEUDO_MEAS_TIMER:
      rex_clr_timer(rrctmr_int_data.rrc_pseudo_meas_timer_ptr);
      break;
#endif
#ifdef ELEVATOR_TRIGGER
    case RRCTMR_PSEUDO_MEAS_RXAGC_TIMER:
    rex_clr_timer(rrctmr_int_data.rrc_pseudo_meas_rxagc_timer_ptr);
      break;
#endif
#ifdef FEATURE_WCDMA_W2L_PSEUDO_MEAS
      case RRCTMR_ALLOW_EARFCN_FOR_PSEUDO_MEAS:
        rex_clr_timer(rrctmr_int_data.rrc_t_allow_earfcn_for_pseudo_meas_timer_ptr);
        break;
#endif

    case RRCTMR_DELAY_WTOL_RESEL_TIMER:
      rex_clr_timer(rrctmr_int_data.rrc_t_delay_wtol_resel_timer_ptr);
      break;

    case RRCTMR_CELL_DEPRI_TIMER:
      rex_clr_timer(rrctmr_int_data.rrc_t_cell_depri_timer_ptr);
      break;

    case RRCTMR_CELL_DEPRI_NBR_TIMER:
      rex_clr_timer(rrctmr_int_data.rrc_t_cell_depri_nbr_timer_ptr);
      break;

    default:
      WRRC_MSG0_ERROR("ERR Unsupported timer is attempted to stop");
      break; 
  }
  WRRC_MSG1_HIGH("Stopped timer:rrctmr_timer_e_type_value%d",timer);
} /* rrctmr_stop_timer */
/***/ __WCDMA_RRC_CODE_SEGMENT_NON_DEMANDPAGED_END__ /***/
/*===========================================================================

FUNCTION rrctmr_get_remaining_time

DESCRIPTION
  This function will return the remaining tics for the Timer passed in.
  
DEPENDENCIES
  None

RETURN VALUE
  Unsigned Long.  Returns 0 if times is not recognized.
  
SIDE EFFECTS
  None
===========================================================================*/
/***/ __WCDMA_RRC_CODE_SEGMENT_NON_DEMANDPAGED__ /***/
uint64 rrctmr_get_remaining_time
(
  sys_modem_as_id_e_type wrrc_as_id, 
  rrctmr_timer_e_type timer                 /* Timer to stop */
)
{
  uint64 remaining_time = 0; 

  switch(timer)
  {
    case RRCTMR_T_300_TIMER:
      remaining_time = rex_get_timer(rrctmr_int_data.rrc_t_300_ptr);
      break;
    case RRCTMR_T_308_TIMER:
      remaining_time = rex_get_timer(rrctmr_int_data.rrc_t_308_ptr);
      break;
    case RRCTMR_T_304_TIMER:
      remaining_time = rex_get_timer(rrctmr_int_data.rrc_t_304_ptr); 
      break;
    case RRCTMR_WAIT_TIME_TIMER:
      remaining_time = rex_get_timer(rrctmr_int_data.rrc_t_wait_time_ptr);
      break;
    case RRCTMR_UE_TO_CAMP_ON_TIMER:
      remaining_time = rex_get_timer(rrctmr_int_data.rrc_t_ue_to_camp_on_ptr);
      break;
    case RRCTMR_RRC_CON_DELAY_TIMER:
      remaining_time = rex_get_timer(rrctmr_int_data.rrc_t_rrc_con_delay_ptr);
      break;
    case RRCTMR_RCE_L2ACK_TIMER:
      remaining_time = rex_get_timer(rrctmr_int_data.rrc_t_rce_l2ack_ptr);
      break;
    case RRCTMR_RCR_L2ACK_TIMER:
      remaining_time = rex_get_timer(rrctmr_int_data.rrc_t_rcr_l2ack_ptr);
      break;
    case RRCTMR_RSSI_BER_TIMER:
      remaining_time = rex_get_timer(rrctmr_int_data.rrc_t_rssi_ber_ptr);
      break;
    case RRCTMR_SIB_WAIT_TIMER:
      remaining_time = rex_get_timer(rrctmr_int_data.rrc_t_sib_wait_ptr);
      break;
    /*Case to handle SIB7 expiry timer*/
    case RRCTMR_SIB7_EXPIRATION_TIMER:
       remaining_time = rex_get_timer(rrctmr_int_data.rrc_t_sib7_expiration_ptr);
        break;

    case RRCTMR_T_302_TIMER:
      remaining_time = rex_get_timer(rrctmr_int_data.rrc_t_302_ptr);
      break;
    case RRCTMR_T_305_TIMER:
      remaining_time = rex_get_timer(rrctmr_int_data.rrc_t_305_ptr);
      break;
    case RRCTMR_T_316_TIMER:
      remaining_time = rex_get_timer(rrctmr_int_data.rrc_t_316_ptr);
      break;
    case RRCTMR_T_317_TIMER:
      remaining_time = rex_get_timer(rrctmr_int_data.rrc_t_317_ptr);
      break;
    case RRCTMR_T_307_TIMER:
      remaining_time = rex_get_timer(rrctmr_int_data.rrc_t_307_ptr);
      break;
    case RRCTMR_T_314_TIMER:
      remaining_time = rex_get_timer(rrctmr_int_data.rrc_t_314_ptr);
      break;
    case RRCTMR_T_315_TIMER:
      remaining_time = rex_get_timer(rrctmr_int_data.rrc_t_315_ptr);
      break;
    case RRCTMR_SIB7_EXP_TIMER:
      remaining_time = rex_get_timer(rrctmr_int_data.rrc_t_sib7_exp_ptr); 
      break;
    case RRCTMR_IGNORE_BCCH_MOD_TIMER:
      remaining_time = rex_get_timer(rrctmr_int_data.rrc_t_ignore_bcch_mod_exp_ptr); 
      break;
    case RRCTMR_DCH_FACH_CELL_SELECTION_TIMER:
      remaining_time = rex_get_timer(rrctmr_int_data.rrc_t_dch_fach_cell_selection_exp_ptr); 
      break;
    case RRCTMR_FACH_TO_FACH_PCH_CELL_SELECTION_TIMER:
      remaining_time = rex_get_timer(rrctmr_int_data.rrc_t_fach_to_fach_pch_cell_selection_exp_ptr); 
      break;
    case RRCTMR_DCH_PCH_CELL_SELECTION_TIMER:
      remaining_time = rex_get_timer(rrctmr_int_data.rrc_t_dch_pch_cell_selection_exp_ptr); 
      break;
    case RRCTMR_L1_DEADLOCK_DETECT_TIMER:
      remaining_time = rex_get_timer(rrctmr_int_data.rrc_t_l1_deadlock_detection_exp_ptr); 
      break;
    case RRCTMR_DELAY_ERR_FATAL_TIMER:
      remaining_time = rex_get_timer(rrctmr_int_data.rrc_t_delay_err_fatal_exp_ptr); 
      break;
    case RRCTMR_T_3174_TIMER:
      remaining_time = rex_get_timer(rrctmr_int_data.rrc_t_3174_ptr);
      break;
    case RRCTMR_FREQ_SCAN_TIMER:
      remaining_time = rex_get_timer(rrctmr_int_data.rrc_t_freq_scan_exp_ptr); 
      break;
    case RRCTMR_T_OOS_TIMER:
      remaining_time = rex_get_timer(rrctmr_int_data.rrc_t_oos_ptr); 
      break;

    case RRCTMR_DEEP_SLEEP_NO_SVC_TIMER:
      remaining_time = rex_get_timer(rrctmr_int_data.rrc_t_deep_sleep_no_svc_ptr); 
      break;

     case RRCTMR_PHY_CHAN_FAILURE_TIMER:
      remaining_time = rex_get_timer(rrctmr_int_data.rrc_t_phy_chan_failure_ptr);
      break;  

    case RRCTMR_BPLMN_SRCH_TIMER:
      remaining_time = rex_get_timer(rrctmr_int_data.rrc_t_bplmn_srch_ptr); 
      break;

    case RRCTMR_BPLMN_FREQ_SCAN_TIMER:
      remaining_time = rex_get_timer(rrctmr_int_data.rrc_t_bplmn_full_scan_srch_ptr); 
      break;     

    case RRCTMR_MANUAL_PLMN_FREQ_SCAN_TIMER:
      remaining_time = rex_get_timer(rrctmr_int_data.rrc_t_manual_plmn_full_scan_srch_ptr); 
      break; 


    case RRCTMR_FREQ_REDIRECT_WAIT_TIMER:
      remaining_time = rex_get_timer(rrctmr_int_data.rrc_t_freq_redirect_wait_timer_ptr); 
      break;   

    case RRCTMR_CONN_MODE_OOS_TIMER:
      remaining_time = rex_get_timer(rrctmr_int_data.rrc_t_conn_mode_oos_timer_ptr); 
      break;   

#ifdef FEATURE_INACTIVITY_HANDLING
    case RRCTMR_INACTIVITY_TIMER:
      remaining_time = rex_get_timer(rrctmr_int_data.rrc_t_inactivity_timer_ptr);
      break;  
#endif /* FEATURE_INTACTIVITY_HANDLING */

    case RRCTMR_WTOW_BPLMN_DRX_SRCH_TIMER:
      remaining_time = rex_get_timer(rrctmr_int_data.rrc_t_wtow_bplmn_srch_ptr); 
      break;                                                 
  
    case RRCTMR_BPLMN_GUARD_SRCH_TIMER:
      remaining_time = rex_get_timer(rrctmr_int_data.rrc_t_wtow_bplmn_guard_srch_ptr); 
      break;         

    case RRCTMR_SIXHR_SIB_UPDATE_TIMER:
      remaining_time = rex_get_timer(rrctmr_int_data.rrc_sixhr_sib_update_timer_ptr);
     break;

    case RRCTMR_TENMIN_SIB_UPDATE_TIMER:
      remaining_time = rex_get_timer(rrctmr_int_data.rrc_tenmin_sib_update_timer_ptr);
      break;

    case RRCTMR_CONN_MODE_OOS_INITIAL_NO_DS_TIMER:
      remaining_time = rex_get_timer(rrctmr_int_data.rrc_t_conn_mode_oos_initial_no_ds_timer_ptr);
     break;

    case RRCTMR_CONN_MODE_OOS_DS_TIMER:
      remaining_time = rex_get_timer(rrctmr_int_data.rrc_t_conn_mode_oos_ds_timer_ptr);
     break;

    case RRC_SIB_CHANGE_GUARD_TIMER:
      remaining_time = rex_get_timer(rrctmr_int_data.rrc_sib_change_guard_timer_ptr); 
      break; 

    case RRC_BCCH_MODIFICATION_TIMER:
      remaining_time = rex_get_timer(rrctmr_int_data.rrc_bcch_mod_timer_ptr); 
      break;

    case RRCTMR_GTOW_REDIRECTION_WAIT_TIMER:
      remaining_time = rex_get_timer(rrctmr_int_data.rrc_t_gtow_redirection_timer_ptr);
     break;

    case RRCTMR_GTOW_REDIRECTION_LFS_TIMER:
      remaining_time = rex_get_timer(rrctmr_int_data.rrc_t_gtow_redir_lfs_timer_ptr);
     break;

    case RRCTMR_WTOG_ABORT_GUARD_TIMER:
      remaining_time = rex_get_timer(rrctmr_int_data.rrc_t_wtog_abort_guard_timer_ptr);
      break;

 
    case RRCTMR_T_319_TIMER:
      remaining_time = rex_get_timer(rrctmr_int_data.rrc_t_319_ptr);
      break;

    case RRCTMR_T_320_TIMER:
      remaining_time = rex_get_timer(rrctmr_int_data.rrc_t_320_ptr);
      break;

#ifdef FEATURE_WCDMA_REL7_SPECRS

    case RRCTMR_COUEC_TIMER:
      remaining_time = rex_get_timer(rrctmr_int_data.rrc_t_couec_timer_ptr);
      break;
#endif /* FEATURE_WCDMA_REL7_SPECRS */


    case RRCTMR_T_323_TIMER:
      remaining_time = rex_get_timer(rrctmr_int_data.rrc_t_323_ptr);
      break;

#ifdef FEATURE_DUAL_SIM
    case RRCTMR_WRM_LOCK_TIMER:
      remaining_time = rex_get_timer(rrctmr_int_data.rrc_wrm_lock_timer_ptr);
      break;

    case RRCTMR_WRM_DELAY_PREEMPTION_TIMER:
      remaining_time = rex_get_timer(rrctmr_int_data.rrc_wrm_delay_preemption_timer_ptr);
      break;

    case RRCTMR_DSIM_BPLMN_LOCK_REL_OFFSET_TIMER:
      remaining_time = rrc_get_non_defer_timer(rrctmr_int_data.rrc_dsim_bplmn_lock_rel_off_timer_ptr);
      break;

    case RRCTMR_WRM_UNLOCK_BY_TIMER:
      remaining_time = rrc_get_non_defer_timer(rrctmr_int_data.rrc_dsim_wrm_unlock_by_timer_ptr);
      break;

    case RRCTMR_MAX_TA_DURATION_TIMER:
      remaining_time = rex_get_timer(rrctmr_int_data.rrc_max_ta_duration_timer_ptr);
      break;

    case RRCTMR_SIB_POKE_TIMER:
      remaining_time = rex_get_timer(rrctmr_int_data.rrc_t_sib_poke_timer_ptr);
      break;

    case RRCTMR_TRM_PRIORITY_CHANGE_TIMER:
      remaining_time = rex_get_timer(rrctmr_int_data.rrc_t_trm_prio_change_timer_ptr);
      break;

    case RRCTMR_ACQ_DTY_CYC_CHANGE_TIMER:
      remaining_time = rex_get_timer(rrctmr_int_data.rrc_t_acq_duty_cycle_change_timer_ptr);
      break;

    case RRCTMR_SIB_STOP_PARTITION_TIMER:
      remaining_time = rex_get_timer(rrctmr_int_data.rrc_t_sib_stop_part_ptr);
      break;

    case RRCTMR_DELAY_LTA_TIMER:
      remaining_time = rex_get_timer(rrctmr_int_data.rrc_t_delay_lta_timer_ptr);
      break;

    case RRCTMR_ALLOW_LTA_DURING_ACCESS_PRIO_TIMER:
      remaining_time = rex_get_timer(rrctmr_int_data.rrc_t_allow_lta_during_access_prio_timer_ptr);
      break;

#endif

#ifdef FEATURE_WCDMA_ABSOLUTE_PRIORITY
    case RRCTMR_DED_PRI_VALIDITY_TIMER:
      remaining_time = rex_get_timer(rrctmr_int_data.rrc_t_ded_pri_validity_timer_ptr);
      break;
#endif

#ifdef FEATURE_QCHAT
    case RRCTMR_QCHAT_FA_RRC_TIMER:
      remaining_time = rex_get_timer(rrctmr_int_data.qchat_fa_rrc_timer_ptr); 
      break;
#endif
    case RRCTMR_RRCCU_TRANS_TO_DISCON_TIMER:
      remaining_time = rex_get_timer(rrctmr_int_data.rrc_t_rrccu_trans_to_disconn_debug_timer_ptr);
      break;


#ifdef FEATURE_LTE_TO_WCDMA
    case RRCTMR_LTE_TO_WCDMA_REDIR_SRCH_TIMER:
      remaining_time = rex_get_timer(rrctmr_int_data.rrc_t_lte_to_wcdma_redir_search_timer_ptr); 
      break;
#endif

#ifdef FEATURE_FEMTO_CSG
    case RRCTMR_CSG_ASF_SRCH_TIMER:
      remaining_time = rex_get_timer(rrctmr_int_data.rrc_t_csg_asf_srch_timer_ptr); 
      break;
#endif
    case RRCTMR_WAIT_FOR_L2_ACK_FOR_PCH_TRANSITION_TIMER:
      remaining_time = rex_get_timer(rrctmr_int_data.rrc_t_rrc_waiting_for_L2_ack_for_pch_ptr);
      break;
      
    case RRCTMR_BLOCK_LTA_TIMER:
      remaining_time = rex_get_timer(rrctmr_int_data.rrc_t_rrc_block_lta_ptr);
      break;

    case RRCTMR_BPLMN_PARTIAL_REPORT_TIMER:
      remaining_time = rex_get_timer(rrctmr_int_data.rrc_t_bplmn_partial_report_timer_ptr);
      break;
#ifdef FEATURE_WCDMA_ALLOW_LTA_DURING_ACT_TIME
    case RRCTMR_OTA_BUFFER_ACTIVATION_TIMER:
      remaining_time = rex_get_timer(rrctmr_int_data.rrc_t_rrc_dch_activation_time_buffer_ota_ptr);
      break;
#endif  /*FEATURE_WCDMA_ALLOW_LTA_DURING_ACT_TIME*/
#ifdef FEATURE_WCDMA_W2L_PSEUDO_MEAS
    case RRCTMR_PSEUDO_MEAS_TIMER:
      remaining_time = rex_get_timer(rrctmr_int_data.rrc_pseudo_meas_timer_ptr);
      break;
#endif
#ifdef ELEVATOR_TRIGGER
    case RRCTMR_PSEUDO_MEAS_RXAGC_TIMER:
    remaining_time = rex_get_timer(rrctmr_int_data.rrc_pseudo_meas_rxagc_timer_ptr);
      break;
#endif
#ifdef FEATURE_WCDMA_W2L_PSEUDO_MEAS
    case RRCTMR_ALLOW_EARFCN_FOR_PSEUDO_MEAS:
      remaining_time = rex_get_timer(rrctmr_int_data.rrc_t_allow_earfcn_for_pseudo_meas_timer_ptr);
      break;
#endif

    case RRCTMR_DELAY_WTOL_RESEL_TIMER:
      remaining_time = rex_get_timer(rrctmr_int_data.rrc_t_delay_wtol_resel_timer_ptr);
      break;

    case RRCTMR_CELL_DEPRI_TIMER:
      remaining_time = rex_get_timer(rrctmr_int_data.rrc_t_cell_depri_timer_ptr);
      break;
    case RRCTMR_CELL_DEPRI_NBR_TIMER:
      remaining_time = rex_get_timer(rrctmr_int_data.rrc_t_cell_depri_nbr_timer_ptr);
      break;
    default:
      WRRC_MSG1_ERROR("ERR Getting remaining time for timer %d is not supported",timer);
      break; 
  }

  if((remaining_time)&&(rrctmr_int_data.print_rem_msg))
  {
    WRRC_MSG2_HIGH("Remaining time %d for timer rrctmr_timer_e_type_value%d",remaining_time, timer);
  }

  return remaining_time;
}

/*===========================================================================

FUNCTION rrctmr_set_print_rem_msg

DESCRIPTION
  This function will set if remaining timer F3 need to be printed or not
  
DEPENDENCIES
  None

RETURN VALUE
  None.
  
SIDE EFFECTS
  None
===========================================================================*/
void rrctmr_set_print_rem_msg(sys_modem_as_id_e_type wrrc_as_id,boolean set_value)
{
  rrctmr_int_data.print_rem_msg = set_value;
}

/***/ __WCDMA_RRC_CODE_SEGMENT_NON_DEMANDPAGED_END__ /***/

/*The following lint directive limits the scope of this directive to 
 this file only */
/*lint +e534*/
