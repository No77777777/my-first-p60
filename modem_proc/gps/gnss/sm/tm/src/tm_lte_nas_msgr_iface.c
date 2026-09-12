/*===========================================================================

                            TM_LTE_MSGR_IFACE

DESCRIPTION


===========================================================================

  Copyright (c) 2012 Qualcomm Technologies Incorporated.
  Qualcomm Confidential and Proprietary. All Rights Reserved.
  Copyright (c) 2013 - 2014 Qualcomm Atheros, Inc.
  Qualcomm Atheros Confidential and Proprietary. All Rights Reserved. 
  Qualcomm Atheros Confidential and Proprietary. All Rights Reserved. 
  Copyright (c) 2017 - 2019 Qualcomm Technologies, Inc.
  All Rights Reserved.
  Confidential and Proprietary - Qualcomm Technologies, Inc.

  Export of this technology or software is regulated by the U.S. Government.
  Diversion contrary to U.S. law prohibited.

  All ideas, data and information contained in or disclosed by
  this document are confidential and proprietary information of
  Qualcomm Technologies Incorporated and all rights therein are expressly reserved.
  By accepting this material the recipient agrees that this material
  and the information contained therein are held in confidence and in
  trust and will not be used, copied, reproduced in whole or in part,
  nor its contents revealed in any manner to others without the express
  written permission of Qualcomm Technologies Incorporated.

===========================================================================*/

/*===========================================================================

                      EDIT HISTORY FOR FILE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/gnss8.mpss/11.1/gnss/sm/tm/src/tm_lte_nas_msgr_iface.c#3 $

  when        who     what, where, why
  --------    ---     ----------------------------------------------------------
11/02/17     sj     Remove ECBM state, rely on CM MSG Router Emergency Mode status messages
10/27/15     mj     Modify OTA reset functions to make it general across UMTS  
11/07/17    kpn    L+L: Handling 3rd party E911 call
08/01/17    kpn    L+L: Use AS_ID while calling tm_cm_process_ext_emergency_enter()
06/15/17    kpn    L+L: Pass AS ID to tm_handle_ota_reset()
02/01/17    kpn    L+L: Strip off variant Id while receving NAS messages
02/01/17    kpn    L+L: Register NAS messages for number of SUBs configured
02/01/17    kpn    Add as_id to OTDOA OTA reset
09/19/16    kpn    Migration from Inst Id to Variant
10/27/15    mj     Modify OTA reset functions to make it general across UMTS  
3/12/14     gk     Handle OTA reset when session is active. consider as end of the session
6/20/12     gk     Initial checkin
===========================================================================*/

/*=============================================================================
                                INCLUDE FILES
=============================================================================*/
#include "comdef.h"
 
#include "tm_lte_nas_msgr_iface.h"
#include "pdapi.h"
#include "sm_loc_lte_otdoa.h"
#include "tm_cm_iface.h"

/*=============================================================================

       Constants

=============================================================================*/

/*=============================================================================

       Macros & Static

=============================================================================*/


#ifdef  FEATURE_CGPS_LTE_CELLDB
  /* LTE OTA reset message type identifier */
  #define MSG_IDEN_LTE_OTA_GPS_RESET 0
  #define MSG_IDEN_LTE_OTA_OTDOA_RESET 1

  static boolean tm_msgr_is_client_created = FALSE;
#endif /* FEATURE_CGPS_LTE_CELLDB */

/* Buffer to receive messages over MSGR */
static tm_msgr_rcv_buffer_type  tm_msgr_rcv;

/*=============================================================================

       Typedefs

=============================================================================*/


/*=============================================================================

       Function Declarations

=============================================================================*/

/*===========================================================================

FUNCTION       TM_MSGR_INIT_QUEUE

DESCRIPTION    This function initializes the receive queue

DEPENDENCIES   None.

RETURN VALUE   None.

SIDE EFFECTS   None.

===========================================================================*/
static void tm_msgr_init_queue( void )
{
   uint8 i;
    /* Initialize the Command Queue, and the Command Free Queue */
  (void) q_init( &tm_msgr_rcv.msg_q );
  (void) q_init( &tm_msgr_rcv.free_q );

  /* Link the Command Queue Items onto the Command Free Queue */
  for (i=0; i<TM_MSGR_RCV_QUEUE_SIZE; i++)
  {
    q_put( &tm_msgr_rcv.free_q,
    q_link( &tm_msgr_rcv.q_items[i],
           &tm_msgr_rcv.q_items[i].msg_hdr.qlink ) );
  }/* init_queue */

}


/*===========================================================================

FUNCTION     TM_MSGR_GET_QUEUE_ID

DESCRIPTION    This function retuns the msgr queue id.

DEPENDENCIES   None.

RETURN VALUE   None.

SIDE EFFECTS   None.

===========================================================================*/
static msgr_id_t*  tm_msgr_get_queue_id( void )
{
  // MSGR Queue ID
  static msgr_id_t tm_msgr_queue_id;

  return &tm_msgr_queue_id;

} /*  tm_msgr_get_queue_id */


/*===========================================================================

FUNCTION tm_msgr_init

DESCRIPTION
  This function is to initialize MSGR interface .
  
DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/
void tm_msgr_init(void)
{

#ifndef FEATURE_GNSS_LEAN_BUILD
  uint8 u_index = 0;
  sys_modem_as_id_e_type e_as_id;
  static msgr_client_t tm_msgr_client_id = 0;
  uint8 u_num_subs = tm_cm_get_num_of_subs();
  errno_enum_type  estatus;
  uint8 u_num_msgs_registered = 0;

  tm_msgr_init_queue();

  estatus = msgr_client_create( &tm_msgr_client_id );

  if (estatus == E_SUCCESS)
  {
    estatus = msgr_client_add_rex_q ( &tm_msgr_client_id,
                                      rex_self(),  // will return own TCB
                                      C_OS_FLAG_MSGR_MSG,  // OSAPI will convert this to IPC
                                      &tm_msgr_rcv.msg_q,
                                      &tm_msgr_rcv.free_q,
                                      offsetof( tm_msgr_rcv_msg_type, msg ),
                                      MSGR_NO_OFFSET,
                                      (uint16)sizeof(tm_msgr_rcv_msg_type ),
                                      tm_msgr_get_queue_id() );

    if (estatus != E_SUCCESS)
    {
      MSG(MSG_SSID_GPSSM, MSG_LEGACY_ERROR,"GNSS MSGR: Failed to init tm_msgr");
      msgr_client_delete(&tm_msgr_client_id);	
      return;
    }
        
    /* Registering for all Subs irrespective of the current active profile [single-SIM/Multi-SIM profile]
       Only the corresponding registered messages are entitled to be received on respective active Sub(s). */
    for (u_index = 0; u_index < u_num_subs; u_index ++)
    {
      /* Variant ID here is nothing but AS ID. Explicilty assigning AS ID constants for variant ID while registering NAS messages
         so as to be inline with the implementation while sending and receiving NAS messages. This is to avoid any porting issues later. */
      e_as_id = AS_ID(u_index);

#ifdef  FEATURE_CGPS_LTE_CELLDB   
      estatus = msgr_register_variant(MSGR_GPS_SM_TM, &tm_msgr_client_id, MSGR_ID_REX, (int)LTE_TLB_DL_INFO_IND, (uint8) e_as_id);
      if (estatus != E_SUCCESS)
      {
        MSG(MSG_SSID_GPSSM, MSG_LEGACY_ERROR,"GNSS MSGR: Failed to register LTE TLB Info IND message");
      }
      else
      {
        u_num_msgs_registered++;
      }
#endif
      
#ifndef FEATURE_GNSS_SA
      estatus = msgr_register_variant(MSGR_GPS_SM_TM, &tm_msgr_client_id, MSGR_ID_REX, (int)MM_CM_EMERGENCY_MODE_STATUS_IND, (uint8) e_as_id);
      if (estatus != E_SUCCESS)
      {
        MSG(MSG_SSID_GPSSM, MSG_LEGACY_ERROR,"GNSS MSGR: Failed to register Emergency Mode Status IND message");
      }
      else
      {
        u_num_msgs_registered++;
      }
#endif
    }
  }
  else
  {
    MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_ERROR,"TM_LTE:MSGR_CREATE failed %d", estatus);
  }
#ifdef  FEATURE_CGPS_LTE_CELLDB 
  if(u_num_msgs_registered == 0)
  {
    MSG(MSG_SSID_GPSSM, MSG_LEGACY_ERROR,"GNSS MSGR: No Msgs registered! Deleting client!!");
    msgr_client_delete(&tm_msgr_client_id);	    
    tm_msgr_is_client_created = FALSE;
  }
  else
  {
    tm_msgr_is_client_created = TRUE;
  } 
#endif /*   FEATURE_CGPS_LTE_CELLDB */
#endif
}

/*===========================================================================

FUNCTION     tm_msgr_check_nas_ota_event

DESCRIPTION    This function checks if the LTE OTA message is a OTA RESET message

DEPENDENCIES   None.

RETURN VALUE   None.

SIDE EFFECTS   None.

===========================================================================*/
void tm_msgr_check_event(void)
{
  uint8 inst_id = 0;
  tm_msgr_rcv_msg_type* msg_ptr = NULL;
  sys_modem_as_id_e_type e_as_id = SYS_MODEM_AS_ID_NONE;

  // receive MSGR message from Queue
  msg_ptr = (tm_msgr_rcv_msg_type*)q_get(&tm_msgr_rcv.msg_q);
  if (msg_ptr == NULL)
  {
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR,"TM: No MSGR message",0,0,0);
    return;
  }

  /* Variant extracted is the AS ID. */
  e_as_id = (sys_modem_as_id_e_type) MSGR_GET_VARIANT(&msg_ptr->msg.hdr);

  /* check what kind of OTA test message this is */
  MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_MED,"MSGR message id [with variant mask] %d", msg_ptr->msg.hdr.id);

  /* Strip off the variant mask to retreive the actual msg ID received. */
  MSGR_CLEAR_VARIANT(msg_ptr->msg.hdr.id);

  switch (msg_ptr->msg.hdr.id)
  {
    
#ifdef  FEATURE_CGPS_LTE_CELLDB   
    case LTE_TLB_DL_INFO_IND:
      MSG_4(MSG_SSID_GPSSM, MSG_LEGACY_MED,"MSGR message id %d payload[1]=%d  payload[2]=%d received on Sub %d", 
              msg_ptr->msg.hdr.id,
              msg_ptr->msg.dl_info.nas_msg_container.msg_payload[1],
              msg_ptr->msg.dl_info.nas_msg_container.msg_payload[2],
              e_as_id);
      if (msg_ptr->msg.dl_info.nas_msg_container.msg_payload[1] == 0x88)
      {
        if (msg_ptr->msg.dl_info.nas_msg_container.msg_size -1 > 0)
        {
          if (msg_ptr->msg.dl_info.nas_msg_container.msg_payload[2] == MSG_IDEN_LTE_OTA_GPS_RESET)
          {
            tm_handle_ota_reset(PRTL_EVENT_TYPE_LTE_OTA_GPS_RESET, e_as_id);
          }

          /* LTE OTA OTDOA reset ? */
          if ( msg_ptr->msg.dl_info.nas_msg_container.msg_payload[2] == MSG_IDEN_LTE_OTA_OTDOA_RESET )
          {
            /* send the LTE OTA OTDOA reset to the OTDOA engine */
            MSG(MSG_SSID_GPSSM, MSG_LEGACY_HIGH,"OTDOA OTA received. Send to LOC_LTE ");
            if (FALSE == sm_loc_lte_ota_reset(e_as_id))
            {
              MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH,"OTDOA OTA reset failed to be sent to LOC_LTE ",0,0,0);
            }
            /* Send LTE OTA OTDOA reset to TM */
            tm_handle_ota_reset(PRTL_EVENT_TYPE_LTE_OTA_OTDOA_RESET, e_as_id);
          }
        }
      }
      break;
#endif
    #ifndef FEATURE_GNSS_SA
    case MM_CM_EMERGENCY_MODE_STATUS_IND:
      if (msg_ptr->msg.emergency_mode_status.emerg_mode_status == CM_EMERGENCY_MODE_STARTED)
      {
        tm_cm_process_ext_emergency_enter(TM_CM_LTE_EMERGENCY_START, e_as_id);
      }
      else if (msg_ptr->msg.emergency_mode_status.emerg_mode_status == CM_EMERGENCY_MODE_ENDED)
      {
        tm_cm_process_ext_emergency_enter(TM_CM_LTE_EMERGENCY_STOP, e_as_id);
      }
      else
      {
        MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_ERROR,"TM_CORE: UNhandled Emergency Mode Status %d",
              msg_ptr->msg.emergency_mode_status.emerg_mode_status);
      }
      break;
    #endif
    default:
      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED,"TM_CORE: UNhandled  MSGR message",0,0,0);
      break;
  }

  if (msg_ptr != NULL)
  {
    // return item to free queue
    q_put( &tm_msgr_rcv.free_q, &msg_ptr->msg_hdr.qlink );
  }
}

/*===========================================================================

FUNCTION     tm_handle_msgr_msg

DESCRIPTION    This function handles msgr messages to TM

DEPENDENCIES   None.

RETURN VALUE   None.

SIDE EFFECTS   None.

===========================================================================*/
void tm_handle_msgr_msg(void )
{

  uint32 msg_cnt = (uint32)q_cnt(&tm_msgr_rcv.msg_q);
  if (msg_cnt == 0)
  {
    return;
  }
 
  /* Process all messages in queue */
  while (q_check(&tm_msgr_rcv.msg_q) != NULL)
  {
    tm_msgr_check_event();
  }
}

