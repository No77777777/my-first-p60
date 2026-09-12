/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                          GSM MSGR Interface
                          FW Messages Handler

GENERAL DESCRIPTION
   This module handles messages received from FW.

EXTERNALIZED FUNCTIONS


INITIALIZATION AND SEQUENCING REQUIREMENTS

Copyright (c) 2001 - 2015 Qualcomm Technologies, Inc.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/geran.mpss/7.4.0/gmsgrif/src/gl1_msgrif_fwmsg.c#3 $ $DateTime: 2020/07/28 11:01:41 $ $Author: pwbldsvc $

when       who       what, where, why
--------   ---       --------------------------------------------------------- 
24/07/20   sal      CR2736787 Added G2X cleanup done handler to issue rf_meas_exit immediately on cleanup confirmation.
18//11/19   nv      CR2567070 FR51554: Recovery mechanism for RF scripting delays in X2Y IRAT scenarios for Habanero
11/08/17   rc      CR2138578  ML logging - GL1 Changes
07/09/15   npt     CR898493 GL1 NV control of T2G acquisition GFW error recovery
20/08/15   npt     CR878988 GL1 NV control of GFW recovery features
28/07/15   pa      CR880706 GL1 Dynamic RxD: Wait for FW exit command before enabling it. 
13/05/15   npt     CR837344 Change f3 logging for RF timeline error recovery feature 
29/04/15   npt     CR804870 RF timeline error recovery mechanism
27/02/15   am      CR646584 WLAN coex
13/11/14   pa      CR755557 Support for granted band/rf device in Idle mode RxD.
26/09/14   pg      CR729410 make DRX exit mode MSGR RSP handling non-blocking 
07/04/14   cja     CR644445 Mainline feature FEATURE_GSM_ASYNC_FW_INTF
14/10/13   pa      CR536820 Added GL1 Msgr If Task. Initial version.
===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "gl1_msgrif_fwmsg.h"
#include "rfcom.h"
#include "gl1_msgrif_init.h"
#include "gl1_msgrif_os.h"
#include "gl1_msgrif_task.h"
#include "rfgsm_msg.h"
#include "rex.h"
#include "geran_tasks.h"
#include "gtmrs.h"
#include "gtmrs_g.h"
#include "gl1_hw.h"
#include "mdsp_intf_g.h"
#include "geran_dual_sim_g.h"
#ifdef FEATURE_WLAN_COEX_SW_CXM
#include "l1i.h"
#include "gl1_arbitrator_cxm.h"
#endif

#ifdef FEATURE_GSM_RX_DIVERSITY
#include "gl1_hw_rxdiversity.h"
#endif /*FEATURE_GSM_RX_DIVERSITY*/

/*===========================================================================

                     GLOBAL DATA

===========================================================================*/


/*===========================================================================

                     FUNCTIONS DEFINITION

===========================================================================*/

/*===========================================================================

FUNCTION  gl1_msgrif_handle_fw_async_msgr_msgs

DESCRIPTION
  This function handles messages received from FW.
  queue.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gl1_msgrif_handle_fw_async_msgr_msgs(gas_id_t gas_id)
{
  gl1_msgrif_gfw_async_msgr_t*  cmd_ptr = NULL;

  /* Clear the OS signal */
  (void)rex_clr_sigs(gl1_msgrif_task_tcb_read(gas_id),
                     GL1_MSGRIF_GFW_ASYNC_INTF_ROUTER_SIG);

  gas_id = check_gas_id(gas_id);

  while ((cmd_ptr = (gl1_msgrif_gfw_async_msgr_t *) q_get(&gl1_msgrif_gfw_async_message_router_q[gas_id])) != NULL)
  {
#ifdef FEATURE_QSH_EVENT_NOTIFY_HANDLER
    #error code not present
#endif /* FEATURE_QSH_EVENT_NOTIFY_HANDLER */	
    MSG_GERAN_LOW_1_G(" GSM L1 MSGR ASYNC MessageRouter queue msg %d ", cmd_ptr->msg.hdr.id);

    switch( cmd_ptr->msg.hdr.id )
    {
      case GERAN_FW_ASYNC_RSP:
#if defined (FEATURE_DUAL_SIM)
      case GERAN_FW2_ASYNC_RSP:
#endif /* FEATURE_DUAL_SIM */
#if defined (FEATURE_TRIPLE_SIM)
      case GERAN_FW3_ASYNC_RSP:
#endif /* FEATURE_TRIPLE_SIM */
      {
        MSG_GERAN_LOW_0_G(" GERAN_FW_ASYNC_RSP ");
        break;
      }

      case GERAN_FW_SLEEP_READY_CMD:
#if defined (FEATURE_DUAL_SIM)
      case GERAN_FW2_SLEEP_READY_CMD:
#endif /* FEATURE_DUAL_SIM */
#if defined (FEATURE_TRIPLE_SIM)
      case GERAN_FW3_SLEEP_READY_CMD:
#endif /* FEATURE_TRIPLE_SIM */
      {
        MSG_GERAN_LOW_0_G(" GERAN_FW_SLEEP_READY_CMD ");
        mdsp_gsm_sleep_cmd_done_isr(gas_id);
        break;
      }
      case GERAN_FW_CLEANUP_DONE_CMD:
#if defined (FEATURE_DUAL_SIM)
      case GERAN_FW2_CLEANUP_DONE_CMD:
#endif
#if defined (FEATURE_TRIPLE_SIM)
      case GERAN_FW3_CLEANUP_DONE_CMD:
#endif
      {
         GfwHostCleanupDoneCmd* data_p = &(cmd_ptr->msg.gfwHostCleanupDoneCmd);
         if (data_p->cleanup == G2X)
         {
           gl1_hw_g2x_cleanup_done_handler(gas_id);
         }
         break;
      }/*Handle GERAN FW CLEANUP DONE CMD corresponding to G2X CLEAN UP*/

#ifdef FEATURE_GSM_RX_DIVERSITY
      case GERAN_FW_RFM_EXIT_MODE_RSP:
#if defined (FEATURE_DUAL_SIM)
      case GERAN_FW2_RFM_EXIT_MODE_RSP:
#endif /* FEATURE_DUAL_SIM */
#if defined (FEATURE_TRIPLE_SIM)
      case GERAN_FW3_RFM_EXIT_MODE_RSP:
#endif /* FEATURE_TRIPLE_SIM */
      {
        MSG_GERAN_MED_1_G(" GERAN_FW_RFM_EXIT_MODE_RSP %d", gl1_hw_get_gfw_enter_status(gas_id));
#ifdef FEATURE_GSM_RX_DIVERSITY_IDLE
        if((FALSE == gl1_get_IMRD_ctl_nv(gas_id)) 
           || ( ftm_get_mode() == FTM_MODE ))
#endif /*FEATURE_GSM_RX_DIVERSITY_IDLE*/
        {
          gl1_hw_deallocate_rxlm_buf_idx_DivRx(gas_id);
        }
        gl1_hw_set_gfw_enter_status(GL1_GFW_PRX_ENTER, gas_id);
        break;
      }
#endif /* FEATURE_GSM_RX_DIVERSITY */

#ifdef FEATURE_WLAN_COEX_SW_CXM
      /* Simply relayed from FW to MCS */
      case GERAN_FW_WLAN_CXM_POLICY_RSP:
#if defined (FEATURE_DUAL_SIM)
      case GERAN_FW2_WLAN_CXM_POLICY_RSP:
#endif /* FEATURE_DUAL_SIM */
#if defined (FEATURE_TRIPLE_SIM)
      case GERAN_FW3_WLAN_CXM_POLICY_RSP:
#endif /* FEATURE_TRIPLE_SIM */
      {
        garb_cxm_fw_priority_fail_report((GfwWlanCxmConfigRsp*)&cmd_ptr->msg.gfwWlanCxmConfigRsp,gas_id);
        break;
      }
#endif

    case GERAN_FW_CCS_STATUS_CMD:
#if defined (FEATURE_DUAL_SIM)
    case GERAN_FW2_CCS_STATUS_CMD:
#endif /* FEATURE_DUAL_SIM */
#if defined (FEATURE_TRIPLE_SIM)
    case GERAN_FW3_CCS_STATUS_CMD:
#endif /* FEATURE_TRIPLE_SIM */
    {
       if ( cmd_ptr->msg.gfwAsyncCcsStatusCmd.errorMask == GFW_CCS_TX_ERROR ) 
       {
         MSG_GERAN_ERROR_0_G("RF timeline error recovery: Tx burst drop");
         gl1_hw_handle_seq_num_error (GL1_GFW_RECOVERY_TX, TRUE, gas_id);

   #ifdef FEATURE_GERAN_RF_TIMELINE_ERROR_RECOVERY
         gl1_hw_tx_drop(gas_id);
   #endif /* FEATURE_GERAN_RF_TIMELINE_ERROR_RECOVERY */
            break;
       } else if ( cmd_ptr->msg.gfwAsyncCcsStatusCmd.errorMask == GFW_CCS_ACQ_LATE_ERROR )
       {
          gl1_hw_acq_late(gas_id);
       } else if ( cmd_ptr->msg.gfwAsyncCcsStatusCmd.errorMask == GFW_CCS_X2G_ACQ_CANCELLED_ERROR )
       {
          gl1_hw_x2g_acq_cancelled(gas_id);
       }
    }
#ifdef FEATURE_GERAN_TIMELINE_ERROR_RECOVERY 
		case GERAN_FW_INVALID_RFLM_TX_INDEX_RSP:
#if defined (FEATURE_DUAL_SIM)
		case GERAN_FW2_INVALID_RFLM_TX_INDEX_RSP: 
#endif
#if defined (FEATURE_TRIPLE_SIM)
		case GERAN_FW3_INVALID_RFLM_TX_INDEX_RSP:
#endif		
		{
		  gl1_hw_handle_seq_num_error (GL1_GFW_RECOVERY_TX, TRUE, gas_id);	  
		}
		break;
#endif

    default:
    {
       MSG_GERAN_ERROR_1_G(" Unknown message router msg id %d ",cmd_ptr->msg.hdr.id);
       break;
    }
   }

    /* Return message to the free queue */
    q_put( &gl1_msgrif_gfw_async_message_router_free_q[gas_id], &cmd_ptr->link);

  } /* End of while loop */

  return;
}
/* EOF */
