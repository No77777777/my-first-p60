/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                            L1 Logging Facilities

GENERAL DESCRIPTION
   This module contains functions for logging HDR L1 packets.

EXTERNALIZED FUNCTIONS

INITIALIZATION AND SEQUENCING REQUIREMENTS

Copyright (c) 2018 Qualcomm Technologies, Inc.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/hdr.mpss/6.0/common/src/hdr_log_qsh.c#1 $
$DateTime: 2019/06/19 22:12:26 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
02/20/18   svu     Fixed klocwork issue
07/25/16   vko     Add generic QSH event
05/27/16   vko     QSH framework for events capturing
===========================================================================*/

/*===========================================================================
                     INCLUDE FILES FOR MODULE
===========================================================================*/
#include "qsh_util.h"
#include "hdr_log_qsh.h"
#include "hdrdebug.h"

/*===========================================================================

                   INTERNAL DEFINITIONS AND TYPES

===========================================================================*/

#define HDR_SUB_IDS 2
#define HDR_ENABLE_ALL_QSH_EVENTS 0xFFFFFFFF

#ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH

typedef struct
{
  uint32 hdr_qsh_event_id_mask;
}hdr_qsh_event_data_t;

hdr_qsh_event_data_t hdr_qsh_event_data[HDR_SUB_IDS] = {0};

#endif

/*===========================================================================

                    INTERNAL FUNCTION PROTOTYPES

===========================================================================*/

void hdr_qsh_debug_callback(qsh_client_cb_params_s *param);

void hdr_qsh_configure_event_data( qsh_client_cfg_s cfg);

void hdr_qsh_event_notify(hdras_id_t hdras_id, qsh_clt_e hdr_qsh_clt, hsrch_qsh_event_e event_id);


#ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH

static void hdr_qsh_event_data_init(void);

#endif 


/*===========================================================================

FUNCTION  hdr_qsh_register

DESCRIPTION
  hdr Registration with QSH.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void hdr_qsh_register(qsh_clt_e hdr_qsh_clt)
{
  qsh_client_reg_s client_reg;
  qsh_action_mask_t     cb_action_support_mask = 0;
  qsh_client_reg_init(&client_reg);
  client_reg.client = hdr_qsh_clt;
  client_reg.major_ver = HDR_QSH_MAJOR_VER ;
  client_reg.minor_ver = HDR_QSH_MINOR_VER;
  client_reg.client_cb_ptr = hdr_qsh_debug_callback;

#ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
  /* default set all events as rare events, enable all HDR events*/
  hdr_qsh_event_data_init();
  cb_action_support_mask |= ( QSH_ACTION_CFG );
  //cb_action_support_mask |= ( QSH_ACTION_EVENT_CFG | QSH_ACTION_RESET );//might not be needed
#endif

  client_reg.cb_action_support_mask = (cb_action_support_mask);

  qsh_client_reg(&client_reg);
		
} /* hdr_qsh_register */

/*===========================================================================

FUNCTION  hdr_qsh_debug_callback

DESCRIPTION
  QSH CALLBACK for hdr EVENTS collection.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void hdr_qsh_debug_callback(qsh_client_cb_params_s *cb_param)
{
  //ASSERT(cb_param != NULL);

  switch(cb_param->action)
  {
#ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH  
    case QSH_ACTION_CFG:
     {
       qsh_client_action_done_s action_done;  
       hdr_qsh_configure_event_data(cb_param->action_params.cfg);

       /* Notify QSH that the required action is handled */
       qsh_client_action_done_init(&action_done);
       action_done.cb_params_ptr = cb_param;
       action_done.action_mode_done = QSH_ACTION_MODE_DONE_SYNC;
       qsh_client_action_done(&action_done);		   
     }
	 break;
#endif
	  
    default:
      break;
  }
} /* hdr_qsh_debug_callback */

#ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
/*===========================================================================
   FUNCTION     hdr_qsh_event_data_init
  
   DESCRIPTION
   init the event data mask for all the data spaces
  
   DEPENDENCIES
     None.
  
   PARAMETERS
    hdras_id, event_id
  
   RETURN VALUE
     None.
  
   SIDE EFFECTS
  
  ===========================================================================*/

void hdr_qsh_event_data_init(void)
{
  hdras_id_t i;
  /*Disable all the hdr Events(Rare and Frequent). 
   We can enable particular event by using CFG command */ 
  for (i = SYS_MODEM_AS_ID_1; i < SYS_MODEM_AS_ID_1 + HDR_SUB_IDS; i++)
  {
    hdr_qsh_event_data[i].hdr_qsh_event_id_mask = 0; /* default disable */
  }
}


/*===========================================================================
   FUNCTION     hdr_qsh_event_notify
  
   DESCRIPTION
   Notifies an event to QSH if configured
  
   DEPENDENCIES
     None.
  
   PARAMETERS
    hdras_id, event_id
  
   RETURN VALUE
     None.
  
   SIDE EFFECTS
  
  ===========================================================================*/
void hdr_qsh_event_notify(hdras_id_t hdras_id, qsh_clt_e hdr_qsh_clt, hsrch_qsh_event_e event_id)
{

  qsh_client_event_notify_params_s  event_notify_params;
  memset(&event_notify_params,0,sizeof(qsh_client_event_notify_params_s));
  if(hdr_qsh_event_data[hdras_id].hdr_qsh_event_id_mask & (1 << event_id))
  {
    HDR_MSG_PROT_1(MSG_LEGACY_HIGH, "QSH DEBUG: EVENT_NOTIFY %d ", event_id);
    qsh_client_event_notify_init(&event_notify_params);
    event_notify_params.client = hdr_qsh_clt;
    event_notify_params.id = event_id;
    event_notify_params.event_data = NULL;
    event_notify_params.subs_id = hdras_id;
    qsh_client_event_notify(&(event_notify_params));

    QSH_LOG(QSH_CLT_HSRCH,QSH_CAT_EVENT,QSH_MSG_TYPE_HIGH,"hdras_id %d: hdr QSH Event Notify %d",hdras_id,event_id);
  }
  else if(hdr_qsh_event_data[hdras_id].hdr_qsh_event_id_mask & (1 << HSRCH_QSH_EVENT_ANY_FAILURE))
  {
    HDR_MSG_PROT_2( MSG_LEGACY_HIGH, 
               "QSH: Sending HSRCH_QSH_EVENT_ANY_FAILURE, QSH_Event_Id:%d, AS_ID:%d",
                event_id, event_notify_params.subs_id );

    qsh_client_event_notify_init(&event_notify_params);
    event_notify_params.client = hdr_qsh_clt;
    event_notify_params.id = HSRCH_QSH_EVENT_ANY_FAILURE;
    event_notify_params.event_data = NULL;
    event_notify_params.subs_id = hdras_id;
    qsh_client_event_notify(&(event_notify_params));  

    QSH_LOG(QSH_CLT_HSRCH,QSH_CAT_EVENT,QSH_MSG_TYPE_HIGH,"hdras_id %d: hdr QSH Generic Event Notify %d",hdras_id,HSRCH_QSH_EVENT_ANY_FAILURE);
  }
  
  
}

/*===========================================================================
   FUNCTION     hdr_qsh_configure_event
  
   DESCRIPTION
     Populate the handler data as given by the QSH
  
   DEPENDENCIES
     None.
  
   PARAMETERS
   hdras_id, event_cfg
  
   RETURN VALUE
     None.
  
   SIDE EFFECTS
  
  ===========================================================================*/

void hdr_qsh_configure_event_data( qsh_client_cfg_s cfg)
{
  qsh_event_action_e action = (byte)(cfg.cmd_code & 0x000000FF);
  byte evt = (byte)((cfg.cmd_code >> 8) & 0x000000FF);
  hdras_id_t hdras_id = ( (byte)((cfg.cmd_code >> 16) & 0x000000FF));
        			   
  if( action == QSH_EVENT_ACTION_ENABLE )
  {
     hdr_qsh_event_data[hdras_id].hdr_qsh_event_id_mask |=  (1 << evt);
  }
  else if ( action == QSH_EVENT_ACTION_DISABLE )
  {
     hdr_qsh_event_data[hdras_id].hdr_qsh_event_id_mask &= ~(1 << evt);
  }
  
}

#endif

/*  EOF */
