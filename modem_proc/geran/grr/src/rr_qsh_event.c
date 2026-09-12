/*!
 * \file rr_qsh_event.c
 *
 * This module contains functionality to interface to QSH via EVENTs.
 *
 *              Copyright (c) 2016 Qualcomm Technologies, Inc.
 *              All Rights Reserved.
 *              Qualcomm Confidential and Proprietary
 */
/* $Header: //components/rel/geran.mpss/7.4.0/grr/src/rr_qsh_event.c#1 $ */
/* $DateTime: 2019/06/06 23:27:31 $$Author: mplcsds1 $ */

/*----------------------------------------------------------------------------
 * Include Files
 * -------------------------------------------------------------------------*/

#include "geran_variation.h"

#ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH

#include "qsh.h"
#include "rr_qsh_event.h"
#include "geran_multi_sim.h"
#include "rr_defs.h"
#include "rr_nv.h"
#include "sys.h"
#include "qsh_util.h"
#include "rr_gprs_defs.h"
#include "rr_l1_send.h"
#include "rr_conn_establish.h"
#include "rr_general.h"
#include "rr_mac_send.h"

/*----------------------------------------------------------------------------
 * Preprocessor Definitions and Constants
 * -------------------------------------------------------------------------*/
  
typedef struct
{
  uint32 event_bitmap;
}rr_qsh_event_data_t;

/*----------------------------------------------------------------------------
 * Variable Definitions
 * -------------------------------------------------------------------------*/
rr_qsh_event_data_t rr_qsh_event_data[NUM_GERAN_DATA_SPACES] = {0};

boolean rr_event_id = FALSE;

static uint32 rr_qsh_failure_event_enum[] = {
                                               RR_QSH_EVENT_ASSIGNMENT_FAILURE, 
                                               RR_QSH_EVENT_HANDOVER_FAILURE,                        
                                               RR_QSH_EVENT_DTM_ASSIGNMENT_FAILURE,           
                                               RR_QSH_EVENT_G2W_HO_FAILURE,                         
                                               RR_QSH_EVENT_RESEL_FAILURE,                               
                                               RR_QSH_EVENT_CON_REL_L2_RESET,                                          
                                               RR_QSH_EVENT_RACH_FAILURE,                               
                                               RR_QSH_EVENT_MPLMN_TIMEOUT,                               
                                               RR_QSH_EVENT_G2X_RESEL_FAILED,                           
                                               RR_QSH_EVENT_GSM_SYS_INFO_TIMER_EXPIRY,          
                                               RR_QSH_EVENT_GPRS_SYS_INFO_TIMER_EXPIRY,
                                               RR_QSH_EVENT_RLF,
                                               RR_QSH_EVENT_PS_SIGNALLING,
                                               RR_QSH_EVENT_PS_DATA,
                                               RR_QSH_EVENT_G2G_HO_STARTED,
                                               RR_QSH_EVENT_FIELD_DEBUG_ANY_FAILURE
                                             };

/*----------------------------------------------------------------------------
 * Externalized Function Definitions
 * -------------------------------------------------------------------------*/

/*!
 * \brief Performs the EVENT CONFIG call-back action
 *
 * \param cb_params_ptr (in)
 */
__attribute__((section(".uncompressible.text")))
void rr_qsh_event_config_perform_cb_action(qsh_client_cb_params_s *cb_params_ptr)
{
  // Short-cut to config params
  qsh_client_cfg_s *config_ptr = &cb_params_ptr->action_params.cfg;

  // Extract action, event, and gas_id from the config params
  qsh_event_action_e action = (uint8)(config_ptr->cmd_code & 0x000000FF);
  rr_qsh_event_id_t event = (uint8)((config_ptr->cmd_code >> 8) & 0x000000FF);
  gas_id_t gas_id = geran_map_nas_id_to_gas_id((sys_modem_as_id_e_type)((config_ptr->cmd_code >> 16) & 0x000000FF));

  uint32 rr_failure_events = (sizeof(rr_qsh_failure_event_enum)/sizeof(uint32));

  if ((action == QSH_EVENT_ACTION_ENABLE) && (RR_QSH_EVENT_FIELD_DEBUG_ANY_FAILURE == event))
  {
    while (rr_failure_events--)
    {
      rr_qsh_event_data[gas_id].event_bitmap |=  (1 << rr_qsh_failure_event_enum[rr_failure_events]);
    }
    rr_event_id = TRUE;
  }  
  else if ((action == QSH_EVENT_ACTION_DISABLE) && (RR_QSH_EVENT_FIELD_DEBUG_ANY_FAILURE == event))
  {
    if (rr_event_id == TRUE)
    {
      while (rr_failure_events--)
      {
        rr_qsh_event_data[gas_id].event_bitmap &=  ~(1 << rr_qsh_failure_event_enum[rr_failure_events]);	    
      }
    }
  }
  else if (action == QSH_EVENT_ACTION_ENABLE)
  {
    rr_qsh_event_data[gas_id].event_bitmap |=  (1 << event);
  }
  else if (action == QSH_EVENT_ACTION_DISABLE)
  {
    rr_qsh_event_data[gas_id].event_bitmap &= ~(1 << event);
  }

  // Indicate call-back action done
  {
    qsh_client_action_done_s action_done;

    qsh_client_action_done_init(&action_done);

    action_done.cb_params_ptr = cb_params_ptr;
    action_done.action_mode_done = QSH_ACTION_MODE_DONE_ASYNC;

    qsh_client_action_done(&action_done);
  }

  return;
}

/*!
 * \brief Notifies an event to QSH if the event notification was enabled by QSH
 *
 * \param event_id(in), gas_id (in)
 */
__attribute__((section(".uncompressible.text")))
void rr_qsh_event_notify(rr_qsh_event_id_t event_id, const gas_id_t gas_id)
{
  qsh_client_event_notify_params_s  event_notify_params;

  /* Check if QSH has enabled corresponding event id */
  if(rr_qsh_event_data[gas_id].event_bitmap & (1 << event_id))
  {
    qsh_client_event_notify_init(&event_notify_params);
    
    event_notify_params.client = QSH_CLT_GRR;

    if(rr_nv_qsh_debug_enabled(RR_GAS_ID_TO_AS_ID) == TRUE)
    {
      MSG_GERAN_HIGH_1_G("GRR QSH: Event Notified: %d", event_id);
    }

    QSH_LOG( QSH_CLT_GRR, QSH_CAT_EVENT, QSH_MSG_TYPE_HIGH,"gs%d: GRR QSH Event Notify %d", gas_id, event_id);

    if (rr_event_id == TRUE)
    {
      event_id = RR_QSH_EVENT_FIELD_DEBUG_ANY_FAILURE;
    }

    event_notify_params.id = event_id;
    event_notify_params.event_data = NULL;
    event_notify_params.subs_id = geran_map_gas_id_to_nas_id(gas_id);
    
    qsh_client_event_notify(&event_notify_params);
  }
}

/*!
 * \brief Maps OTA failure message to QSH event and notifies QSH the same
 *
 * \param msg_type(in), gas_id (in)
 */
__attribute__((section(".uncompressible.text")))
void rr_qsh_event_notify_from_ota_failure_msg(uint8 msg_type, const gas_id_t gas_id)
{
  switch(msg_type)
  {
    case ASSIGNMENT_FAILURE :
    {
       rr_qsh_event_notify(RR_QSH_EVENT_ASSIGNMENT_FAILURE, gas_id);
       break;
    }

    case HANDOVER_FAILURE  :
    {
       rr_qsh_event_notify(RR_QSH_EVENT_HANDOVER_FAILURE, gas_id);
       break;
    }

    case DTM_ASSIGNMENT_FAILURE :
    {
      rr_qsh_event_notify(RR_QSH_EVENT_DTM_ASSIGNMENT_FAILURE, gas_id);
      break;
    }
    
    default:
      break;
  }
}

/*!
 * \brief Initialisation function called from RR-QSH when event notification functionality is required.
 *
  * \param void
 */
__attribute__((section(".uncompressible.text")))
void rr_qsh_event_init(void)
{
  int as_index;
  
  for (as_index = 0; as_index < NUM_GERAN_DATA_SPACES; as_index++)
  {
    /* By default all the GRR events are disabled */
    rr_qsh_event_data[as_index].event_bitmap =  0 ;
  }
}

#endif // FEATURE_QSH_EVENT_NOTIFY_TO_QSH

/* EOF */
