/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                          GL1 MSGR IF

GENERAL DESCRIPTION
   This module contains the entry point for the GL1 Msgr IF task.

EXTERNALIZED FUNCTIONS
  gsm_msgrif_task
    GSM MSGR IF 'task' entry point.

INITIALIZATION AND SEQUENCING REQUIREMENTS

Copyright (c) 2001 - 2015 Qualcomm Technologies, Inc.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/geran.mpss/7.4.0/gmsgrif/src/gl1_msgrif_task.c#1 $ $DateTime: 2019/06/06 23:27:31 $ $Author: mplcsds1 $

when       who       what, where, why
--------   ---       ---------------------------------------------------------
29/10/17   km      CR2133917 GL1 support for L+L
23/06/15   nm      CR836193 dont clear rex signal twice from msgrif task
27/04/15   ws      CR815011 Featurise cfcm_cpu_monitor handling uder FEATURE_DUAL_DATA
09/04/15   jj      CR819956 Thor 2.x RF API change, G2W use MSGR
21/01/15   jj      CR 784462 remove FEATURE_THOR_G2X_MSGR_REWORK 
17/10/14   jj      CR741205 moving  G2X CNF from l1 task to MSGR
07/04/14   cja     CR644445 Mainline feature FEATURE_GSM_ASYNC_FW_INTF
14/10/13   pa      CR536820: Added GL1 Msgr If Task. Initial version.
===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "customer.h"
#include "gl1_msgrif_task.h"
#include "geran_tasks.h"
#include "rex_types.h"
#include "rcinit.h"
#include "msg.h"
#include "task.h"
#include "gl1_msgrif_os.h"
#include "gl1_msgrif_init.h"
#include "gl1_msgrif_rfmsg.h"
#include "gl1_msgrif_fwmsg.h"
#include "geran_dual_sim_g.h"

/*===========================================================================

                     GLOBAL DATA

===========================================================================*/
const geran_task_id_t gl1_msgrif_task_id[NUM_GERAN_DATA_SPACES] =
  { INITIAL_VALUE_DIFFERENT(GERAN_TASK_ID_GL1_MSGR_IF_1,
                            GERAN_TASK_ID_GL1_MSGR_IF_2,
                            GERAN_TASK_ID_GL1_MSGR_IF_3) };

q_type  gl1_msgrif_rf_task_msgr_q[NUM_GERAN_DATA_SPACES];
q_type  gl1_msgrif_rf_task_msgr_free_q[NUM_GERAN_DATA_SPACES];
gl1_msgrif_rf_task_msgr_t  gl1_msgrif_rf_task_msgr[NUM_GERAN_DATA_SPACES][GL1_MSGRIF_RF_TASK_MSGR_QUEUE_SIZE];

q_type gl1_msgrif_gfw_async_message_router_q[NUM_GERAN_DATA_SPACES];
q_type gl1_msgrif_gfw_async_message_router_free_q[NUM_GERAN_DATA_SPACES];
gl1_msgrif_gfw_async_msgr_t  gl1_msgrif_gfw_async_msgr[NUM_GERAN_DATA_SPACES][GL1_MSGRIF_GFW_ASYNC_MSGR_QUEUE_SIZE];

#if defined  (FEATURE_GSM_TO_LTE) || defined (FEATURE_GSM_TDS)
q_type gl1_msgrif_g2x_message_router_q[NUM_GERAN_DATA_SPACES];
q_type gl1_msgrif_g2x_message_router_free_q[NUM_GERAN_DATA_SPACES];
geran_gl1_msgr_t   gl1_msgrif_g2x_msgr[NUM_GERAN_DATA_SPACES][ GL1_MSGR_QUEUE_SIZE ];                               // To look at ----------
#endif

q_type gl1_msgrif_vfrmap_message_router_q[NUM_GERAN_DATA_SPACES];
q_type gl1_msgrif_vfrmap_message_router_free_q[NUM_GERAN_DATA_SPACES];
geran_gl1_vfrmap_msgr_t gl1_msgrif_vfrmap_msgr[NUM_GERAN_DATA_SPACES][ GL1_MSGR_QUEUE_SIZE ];


#ifdef FEATURE_DUAL_DATA
q_type gl1_msgrif_cfcm_cpu_monitor_router_q[NUM_GERAN_DATA_SPACES];
q_type gl1_msgrif_cfcm_cpu_monitor_router_free_q[NUM_GERAN_DATA_SPACES];
geran_gl1_msgr_t   gl1_msgrif_cfcm_cpu_monitor_msgr[NUM_GERAN_DATA_SPACES][ GL1_MSGR_QUEUE_SIZE ];
#endif

#if defined(FEATURE_DUAL_SIM) || defined(FEATURE_TRIPLE_SIM)
extern rex_tcb_type gsm_gl1_msgrif_tcb_2;

#if defined ( FEATURE_TRIPLE_SIM )
extern rex_tcb_type gsm_gl1_msgrif_tcb_3;
#endif /* FEATURE_TRIPLE_SIM */
#endif /*defined(FEATURE_DUAL_SIM) || defined(FEATURE_TRIPLE_SIM)*/

/*===========================================================================

                     FUNCTIONS DEFINITION

===========================================================================*/
/*===========================================================================

FUNCTION  gl1_msgrif_task_tcb_read

DESCRIPTION
  Returns a pointer to the tcb of gl1 msgr tasks

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
rex_tcb_type* gl1_msgrif_task_tcb_read(gas_id_t gas_id)
{
 gas_id = check_gas_id(gas_id);
 return geran_tcb_read(gl1_msgrif_task_id[gas_id]);
}
/*===========================================================================

FUNCTION  gsm_msgrif_task

DESCRIPTION
  This function calls initialisation functions to perform task initialisation
  for the GSM Msgr IF task, then sits in an infinite loop waiting for messages
  on the input queue.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gl1_msgrif_main (dword unused)
{
  rex_sigs_type sigs = 0;        /* This task's signals */
  gas_id_t gas_id;

  NOTUSED(unused); /* silence compiler warnings of unused parameter */

  /* The gas_id handled by l1 tasks is determined by the tcb name
  of the task. GERAN_ACCESS_STRATUM_ID_1 will be handled by l1_task
  with the tcb gsm_l1_tcb_1, and GERAN_ACCESS_STRATUM_ID_2 is handled
  by the second instance of the task */
#if defined(FEATURE_DUAL_SIM) || defined (FEATURE_TRIPLE_SIM)
  if ( rex_self() == &gsm_gl1_msgrif_tcb_2 )
  {
    gas_id = GERAN_ACCESS_STRATUM_ID_2;
  }
#if defined ( FEATURE_TRIPLE_SIM )
  else if ( rex_self() == &gsm_gl1_msgrif_tcb_3 )
  {
    gas_id = GERAN_ACCESS_STRATUM_ID_3;
  }
#endif /* FEATURE_TRIPLE_SIM */
  else
#endif
  {
    gas_id = GERAN_ACCESS_STRATUM_ID_1;
  }

  /* Perform startup init here. Satisfy dependency of other tasks
  provided by this task. Do not linger here, startup latency is
  impacted. Higher priority tasks in the same group will preempt
  this task at this point. Do not rely upon priority alone as the
  sole mechanism to arbitrate resource contention with other tasks. */

  rcinit_handshake_startup(); /* required rcinit handshake */

/* Kill 2nd Thread if it's not Triton Modem */
#ifndef FEATURE_DUAL_SIM
  if (gas_id == GERAN_ACCESS_STRATUM_ID_2) {
   MSG_GERAN_HIGH_0_G("Existing GL1 MsgrIf GERAN_ACCESS_STRATUM_ID_2");
    return;
  }
#endif

#ifndef FEATURE_TRIPLE_SIM
  if (gas_id == GERAN_ACCESS_STRATUM_ID_3) {
   MSG_GERAN_HIGH_0_G("Existing GL1 MsgrIf GERAN_ACCESS_STRATUM_ID_3");
   return;
  }
#endif

  gl1_msgrif_initialise(gas_id);


  while (TRUE) {

     /* Set Signal mask*/
     sigs |= GL1_MSGRIF_RF_TASK_SIG;
     sigs |= GL1_MSGRIF_GFW_ASYNC_INTF_ROUTER_SIG;
#if defined (FEATURE_GSM_TO_LTE) || defined (FEATURE_GSM_TDS)|| defined (FEATURE_GSM_TO_WCDMA_MSGR)
     sigs |= GL1_MSGRIF_G2X_INTF_ROUTER_SIG;
#endif
#ifdef FEATURE_DUAL_DATA
     sigs |= GL1_MSGRIF_CFCM_CPU_MONITOR_SIG;
#endif
     sigs |= GL1_MGSRIF_VFRMAP_ROUTER_SIG;

     sigs |= TASK_STOP_SIG;

     /* Wait for any signal*/
     sigs = rex_wait(sigs);
     MSG_GERAN_LOW_1_G(" sigs = %d ",sigs);
     /* Received signal from RF task and handle it*/
     if (sigs & GL1_MSGRIF_RF_TASK_SIG){
        gl1_msgrif_handle_rf_task_msgr_msgs(gas_id);
      }

     /* Received signal from FW task and handle it*/
     if (sigs & GL1_MSGRIF_GFW_ASYNC_INTF_ROUTER_SIG) {
        gl1_msgrif_handle_fw_async_msgr_msgs(gas_id);
      }
#if defined (FEATURE_GSM_TO_LTE) || defined (FEATURE_GSM_TDS) || defined (FEATURE_GSM_TO_WCDMA_MSGR)
     if ( sigs & GL1_MSGRIF_G2X_INTF_ROUTER_SIG )
     {
      gl1_msgrif_handle_message_router_msgs ( gas_id);
     }
#endif
#ifdef FEATURE_DUAL_DATA
     if ( sigs & GL1_MSGRIF_CFCM_CPU_MONITOR_SIG )
     {
      gl1_msgrif_handle_cfcm_cpu_monitor_message_router_msgs (gas_id);
     }
#endif
     if (sigs & GL1_MGSRIF_VFRMAP_ROUTER_SIG)
     {
      gl1_msgrif_handle_vfrmap_msg(gas_id);
     }
     if (sigs & TASK_STOP_SIG)
     {
       (void)rex_clr_sigs( rex_self(), TASK_STOP_SIG );
       break;
     }
   }

  gl1_msgrif_init_teardown(gas_id);
  return;
}
/*===========================================================================

FUNCTION  gsm_msgrif_task

DESCRIPTION
  This function is the entry point for the GSM Msgr IF task. It calls
  initialisation functions to perform task initialisation for the GSM Msgr IF
  task, then sits in an infinite loop waiting for messages on the input
  queue.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gsm_gl1_msgrif_task (dword dummy)
{
  gl1_msgrif_main(dummy);
  return;
}
/*===========================================================================

FUNCTION  gl1_msgrif_handle_message_router_msgs

DESCRIPTION
  This function is the entry point for the GSM Msgr IF task. It calls
  initialisation functions to perform task initialisation for the GSM Msgr IF
  task, then sits in an infinite loop waiting for messages on the input
  queue.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
#if defined (FEATURE_GSM_TO_LTE) || defined (FEATURE_GSM_TDS)|| defined (FEATURE_GSM_TO_WCDMA_MSGR)
void gl1_msgrif_handle_message_router_msgs (gas_id_t gas_id)
{

 geran_gl1_msgr_t*  cmd_ptr = NULL;

 /* Clear the OS signal */
 (void)rex_clr_sigs( gl1_msgrif_task_tcb_read(gas_id), GL1_MSGRIF_G2X_INTF_ROUTER_SIG);

   while ((cmd_ptr = (geran_gl1_msgr_t *) q_get(&gl1_msgrif_g2x_message_router_q[gas_id])) != NULL)
   {
    MSG_GERAN_HIGH_1_G(" GSM MSGRIF gl1_msgrif_g2x_message_router_q queue msg 0x%x ", cmd_ptr->msg.hdr.id);

    gl1_handle_irat_mgsr_msgs (cmd_ptr ,gas_id );
    /* Return message to the free queue */
    q_put( &gl1_msgrif_g2x_message_router_free_q[gas_id], &cmd_ptr->link);

    } /* End of while loop */


}
#endif /* (FEATURE_GSM_TO_LTE) || defined (FEATURE_GSM_TDS) || defined (FEATURE_GSM_TO_WCDMA_MSGR)*/


/*===========================================================================

FUNCTION  gl1_msgrif_handle_cfcm_cpu_monitor_message_router_msgs

DESCRIPTION


DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
#ifdef FEATURE_DUAL_DATA
void gl1_msgrif_handle_cfcm_cpu_monitor_message_router_msgs (gas_id_t gas_id)
{

 geran_gl1_msgr_t*  cmd_ptr = NULL;

 /* Clear the OS signal */
 (void)rex_clr_sigs( gl1_msgrif_task_tcb_read(gas_id), GL1_MSGRIF_CFCM_CPU_MONITOR_SIG);
   while ((cmd_ptr = (geran_gl1_msgr_t *) q_get(&gl1_msgrif_cfcm_cpu_monitor_router_q[gas_id])) != NULL)
   {
         MSG_GERAN_HIGH_1_G(" GERAN_GL1_CFCM_CPU_MONITOR_IND %d ",cmd_ptr->msg.hdr.id);
        l1_handle_cfcm_cpu_monitor(&cmd_ptr->msg.cfcm_monitor_ind_msg);
    /* Return message to the free queue */
    q_put( &gl1_msgrif_cfcm_cpu_monitor_router_free_q[gas_id], &cmd_ptr->link);

    } /* End of while loop */


}
#endif /* FEATURE_DUAL_DATA */

void gl1_msgrif_handle_vfrmap_msg (gas_id_t gas_id)
{

 geran_gl1_vfrmap_msgr_t*  cmd_ptr = NULL;

 /* Clear the OS signal */
 (void)rex_clr_sigs( gl1_msgrif_task_tcb_read(gas_id), GL1_MGSRIF_VFRMAP_ROUTER_SIG);
   while ((cmd_ptr = (geran_gl1_vfrmap_msgr_t *) q_get(&gl1_msgrif_vfrmap_message_router_q[gas_id])) != NULL)
   {
        MSG_GERAN_HIGH_1_G(" GL1_MGSRIF_VFRMAP_ROUTER_SIG %d ",cmd_ptr->msg.msg_hdr.id);
        l1_handle_vfrmap_msg(cmd_ptr);
    /* Return message to the free queue */
    q_put( &gl1_msgrif_vfrmap_message_router_free_q[gas_id], &cmd_ptr->link);

    } /* End of while loop */

}
/* EOF */
