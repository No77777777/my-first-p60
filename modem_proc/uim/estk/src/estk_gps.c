/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                          ESTK GPS File

GENERAL DESCRIPTION 
This file contains the GPS APIs needed by the ESTK module. 
  These APIs internally call the QMI_LOC APIs

EXTERNALIZED FUNCTIONS

INITIALIZATION AND SEQUENCING REQUIREMENTS

Copyright (c) 2015-2017, 2019 - 2021 Qualcomm Technologies Incorporated.
All Rights Reserved.
Qualcomm Confidential and Proprietary.
 
$Header: //components/rel/uim.mpss/6.1.0/estk/src/estk_gps.c#8 $
$DateTime: 2021/02/10 01:37:18 $
$Author: pwbldsvc $
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================
                        EDIT HISTORY FOR MODULE

$Header: //components/rel/uim.mpss/6.1.0/estk/src/estk_gps.c#8 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
01/21/21   rm      Fixed function prototype issue
06/18/20   rm      Added handling of null nmea indication
05/19/20   rkg     Update engine lock status indication for NFW
02/12/20   ks      Update engine lock status for NFW client
01/24/20   ks      Handle get engine lock ind during powerup 
01/24/20   ks      Add support to check NFW lock status
01/24/20   ks      Remove handling of ESTK_LOC_SET_NMEA_IND
10/22/19   ks      NFW client registration support
06/07/17   gm      Do not send GEO location envelope when failure
05/31/16   gs      F3 Logging inprovements
05/27/16   shr     F3 reduction: Malloc Failure, Memcpy, Null Ptr checks
08/26/15   gm      Using message response result
06/25/15   gm      L4 compiler warning
06/08/15   gm      Split ME and slot specific NV
05/20/15   gm      Fix compilation warning
05/14/15   gm      Fix memory corruption
05/01/15   gm      Fixed memory leak
03/26/15   gm      Initial Version

===========================================================================*/


/*===========================================================================

          C O M M O N   I N C L U D E   F I L E S   A N D   D A T A

===========================================================================*/
#include "estk_gps.h"

/*=============================================================================

                       DATA DECLARATIONS

=============================================================================*/

/* Client Name is predefined by QMI_LOC */
#define ESTK_QMI_LOC_CLIENT        "SIM"

/*===========================================================================

          C O M M O N   S T R U C T U R E   D E C L A R A T I O N S

===========================================================================*/

void estk_qmi_loc_client_ind_cb
(
  qmi_client_type   user_handle,
  unsigned int      msg_id,
  void             *ind_buf_ptr,
  unsigned int      ind_buf_len,
  void             *ind_cb_data
);

void estk_qmi_loc_client_error_cb
(
  qmi_client_type         user_handle,
  qmi_client_error_type   error,
  void                   *err_cb_data
);

#ifdef FEATURE_GSTK_QMI_LOC
/*---------------------------------------------------------------------------
   QMI Notifier handler
---------------------------------------------------------------------------*/
static qmi_client_type notifier_handle = NULL;

/*---------------------------------------------------------------------------
   QMI Client handler
---------------------------------------------------------------------------*/
static qmi_client_type client_handle = NULL;

/*---------------------------------------------------------------------------
   Notifier OS param
---------------------------------------------------------------------------*/
static qmi_client_os_params          notifier_os_params;

/*---------------------------------------------------------------------------
   Client OS param
---------------------------------------------------------------------------*/
static qmi_client_os_params          client_os_params;

/*---------------------------------------------------------------------------
   Local Variable
---------------------------------------------------------------------------*/
static  uint32                       geo_loc_pref_max_resp_time;
static  uint8                        geo_loc_slot_mask; 
static  nmea_cache_type              nmea_cache[ESTK_NMEA_MAX_TYPE];
#endif


/*===========================================================================

                        EXTERNAL FUNCTION DEFINITION

===========================================================================*/

/*===========================================================================
FUNCTION ESTK_QMI_LOC_CLIENT_INIT

DESCRIPTION 
  Initializes the QMI-LOC client during power-up and register
  with QMI LOC service for system information indication

DEPENDENCIES 
  FEATURE_ESTK_HAS_QMI_LOC 

RETURN VALUE 
  None. 

SIDE EFFECTS 
  None. 
===========================================================================*/
boolean estk_qmi_loc_client_init(void)
{
#ifdef FEATURE_GSTK_QMI_LOC
  qmi_client_error_type         retval;
  qmi_idl_service_object_type   loc_svc_obj = NULL;
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  UIM_MSG_HIGH_0("estk_qmi_loc_client_init()" );

  /*-----------------------------------------------------------------------
    This function will be called during powerup. Initialize notifier
    and client handles
  -----------------------------------------------------------------------*/
  notifier_handle = NULL;
  client_handle = NULL;

  /*-----------------------------------------------------------------------
     Get LOC service object
  -----------------------------------------------------------------------*/
  loc_svc_obj = loc_get_service_object_v02();
  if(!loc_svc_obj)
  {
    UIM_MSG_HIGH_0("Unable to get LOC service object" );
    return FALSE;
  }
  UIM_MSG_HIGH_0("Get loc service object success" );

  /*-----------------------------------------------------------------------
    Initialize OS Signal required by QCCI to notify estk_app_task when LOC
    service comes up
  -----------------------------------------------------------------------*/
  memset(&notifier_os_params,0,sizeof(qmi_client_os_params));
  notifier_os_params.tcb = rex_self();
  notifier_os_params.sig = ESTK_NOTIFY_QMI_LOC_SIG;

  /*-----------------------------------------------------------------------
    Send QMI notifier init
  -----------------------------------------------------------------------*/
  retval = qmi_client_notifier_init(loc_svc_obj,
                                    &notifier_os_params,
                                    &notifier_handle);
  if((QMI_NO_ERR != retval) || (!notifier_handle))
  {
    UIM_MSG_ERR_0( "Error during qmi_client_notifier_init error");
    notifier_handle = NULL;
    return FALSE;
  }

  UIM_MSG_HIGH_0("estk_qmi_loc_client_init(): Initialization successful" );
#endif  
  return TRUE;
}/* ESTK_GPS_INIT */

/*===========================================================================
FUNCTION ESTK_GEO_LOC_SLOT_ID_INIT

DESCRIPTION
  This function assigns slot_id based on geo_loc_slot_mask.

PARAMETERS
  None

DEPENDENCIES
  None

RETURN VALUE
  gstk_slot_id_enum_type

COMMENTS
  None

SIDE EFFECTS
  None

SEE ALSO
  None
===========================================================================*/
gstk_slot_id_enum_type estk_geo_loc_slot_id_init(void)
{
#ifdef FEATURE_GSTK_QMI_LOC
  gstk_slot_id_enum_type slot_id = GSTK_SLOT_1;
  
  if (geo_loc_slot_mask & ESTK_MASK_SLOT_1)
  {
    slot_id = GSTK_SLOT_1;
  }
  else if (geo_loc_slot_mask & ESTK_MASK_SLOT_2)
  {
    slot_id = GSTK_SLOT_2;
  }
  else if (geo_loc_slot_mask & ESTK_MASK_SLOT_3)
  {
    slot_id = GSTK_SLOT_3;
  }
  else
  {
    slot_id = GSTK_SLOT_1;
  }
  return slot_id;
#endif
return GSTK_SLOT_1;
}/* estk_geo_loc_slot_id_init */

/*===========================================================================
FUNCTION  ESTK_QMI_LOC_REQUEST_RESP

DESCRIPTION 
  Handle callbacks response of QMI_LOC_START_REQ_V02 and
  QMI_LOC_REG_EVENTS_REQ_V02.

DEPENDENCIES 
  FEATURE_ESTK_HAS_QMI_LOC

RETURN VALUE 
  None 

SIDE EFFECTS 
  Release the memory assigned for response
===========================================================================*/
void estk_qmi_loc_request_resp
(
 qmi_client_type                user_handle,
 unsigned int                   msg_id,
 void                          *resp_c_struct,
 unsigned int                   resp_c_struct_len,
 void                          *resp_cb_data,
 qmi_client_error_type          transp_err
)
{
#ifdef FEATURE_GSTK_QMI_LOC
  estk_geo_loc_resp_msg_v02         *loc_resp_ptr = NULL;
  estk_cmd_type                     *task_cmd_ptr = NULL; 
  gstk_slot_id_enum_type             slot_id;

  (void)user_handle;
  (void)resp_c_struct_len;
  (void)transp_err;
  
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  UIM_MSG_HIGH_1("estk_qmi_loc_request_resp() MSG_ID : 0x%x",msg_id);

  /*---------------------------------------------------------------------
     Check if response pointer is valid
  ---------------------------------------------------------------------*/
  if((resp_c_struct == NULL) || (resp_cb_data == NULL))
  {
    UIM_MSG_ERR_0("Null pointer passed" );
    return;
  }
  
  slot_id = estk_geo_loc_slot_id_init();

  /* Memory is allocated by GSTK in QMI_LOC request for response */
  loc_resp_ptr = (estk_geo_loc_resp_msg_v02*)resp_c_struct;

  switch(msg_id)
  {
    case QMI_LOC_START_RESP_V02:
      task_cmd_ptr = estk_get_cmd_buf();
      if (task_cmd_ptr != NULL)
      {      
        /* Post a command to the eCall App task to process this GPS fix */
        task_cmd_ptr->hdr.cmd   = ESTK_GEO_LOC_REQUEST_RSP;
        task_cmd_ptr->hdr.sim_slot_id = slot_id;
        if(loc_resp_ptr->resp.result == QMI_RESULT_SUCCESS_V01)
        {
          task_cmd_ptr->cmd_data.gps_data.request_rsp_status = TRUE;
        }
        else
        {
          task_cmd_ptr->cmd_data.gps_data.request_rsp_status = FALSE;        
        }
        estk_put_cmd(task_cmd_ptr);      
      }
      break;      
    case QMI_LOC_REG_EVENTS_RESP_V02:
    case QMI_LOC_GET_ENGINE_LOCK_RESP_V02:
    case QMI_LOC_STOP_RESP_V02:
      break;
    default:
      UIM_MSG_ERR_2("Invalid msg_id=%u. Free mem 0x%x",
                                 msg_id,
                                 loc_resp_ptr );
      break;
  }
    
  if(loc_resp_ptr->resp.result != QMI_RESULT_SUCCESS_V01)
  {
    UIM_MSG_ERR_2("Error in get_loc_resp error=%d. Free mem 0x%x",
                               loc_resp_ptr->resp.error,
                               loc_resp_ptr );
  }

  gstk_free(loc_resp_ptr);
  loc_resp_ptr = NULL;
#endif
}/*estk_qmi_loc_request_resp()*/

/*===========================================================================
FUNCTION ESTK_QMI_LOC_CLIENT_REGISTER_EVENT

DESCRIPTION
  Register for location events with QMI_LOC

DEPENDENCIES 
  FEATURE_ESTK_HAS_QMI_LOC 

RETURN VALUE 
  None

SIDE EFFECTS 
  None
===========================================================================*/
void estk_qmi_loc_client_register_event
(
  void
)
{
#ifdef FEATURE_GSTK_QMI_LOC
  qmi_txn_handle                 get_sys_txn_hdl;
  qmiLocRegEventsReqMsgT_v02    *LocRegEventsReqMsg_ptr  = NULL;
  estk_geo_loc_resp_msg_v02     *loc_ind_reg_resp_msg_ptr  = NULL;
  int                            client_cb_data = 0;
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  UIM_MSG_HIGH_0("estk_qmi_loc_client_register_event()" );
  
  /*-----------------------------------------------------------------------
    Allocate dynamic memory for response message. Memory will be freed
    during response message callback handler
  ------------------------------------------------------------------------*/
  loc_ind_reg_resp_msg_ptr
    = (estk_geo_loc_resp_msg_v02*)
        GSTK_CALLOC(sizeof(estk_geo_loc_resp_msg_v02));

  if(loc_ind_reg_resp_msg_ptr == NULL)
  {
    UIM_MSG_ERR_0("Couldn't get data_block_ptr memory" );
    return;
  }

  LocRegEventsReqMsg_ptr
    = (qmiLocRegEventsReqMsgT_v02*)
        GSTK_CALLOC(sizeof(qmiLocRegEventsReqMsgT_v02));

  if(LocRegEventsReqMsg_ptr == NULL)
  {
    UIM_MSG_HIGH_0("Couldn't get data_block_ptr memory" );
    gstk_free(loc_ind_reg_resp_msg_ptr);
    loc_ind_reg_resp_msg_ptr = NULL;
    return;
  }

  memset(LocRegEventsReqMsg_ptr,
         0,
         sizeof(qmiLocRegEventsReqMsgT_v02));

  /* POSITION_REPORT and NMEA sentence*/
  LocRegEventsReqMsg_ptr->eventRegMask = ((QMI_LOC_EVENT_MASK_POSITION_REPORT_V02)|
                                          (QMI_LOC_EVENT_MASK_NMEA_V02)|
                                          (QMI_LOC_EVENT_MASK_ENGINE_LOCK_STATE_V02));

  /* Set the Client String */
  LocRegEventsReqMsg_ptr->clientStrId_valid = TRUE;
  gstk_memcpy(LocRegEventsReqMsg_ptr->clientStrId, ESTK_QMI_LOC_CLIENT,
              strlen(ESTK_QMI_LOC_CLIENT), 
              QMI_LOC_MAX_CLIENT_ID_STRING_LENGTH_V02,
              strlen(ESTK_QMI_LOC_CLIENT));

  /* Set the Client Type, UIM is always NFW client */
  LocRegEventsReqMsg_ptr->clientType_valid = TRUE;
  LocRegEventsReqMsg_ptr->clientType = eQMI_LOC_CLIENT_NFW_V02;

  /* Set the Pos Request Notification */
  LocRegEventsReqMsg_ptr->enablePosRequestNotification_valid = TRUE;
  LocRegEventsReqMsg_ptr->enablePosRequestNotification = FALSE;
  /* Set to TRUE if AP wants to know the information that card 
     has requested positioning information */
  if (gstk_nv_get_me_feature_status(
        GSTK_CFG_FEATURE_ME_ENABLE_QMI_LOC_NOTIFY_SUPPORT) == TRUE)
  {
    LocRegEventsReqMsg_ptr->enablePosRequestNotification = TRUE;
  }

  memset(loc_ind_reg_resp_msg_ptr,
         0,
         sizeof(estk_geo_loc_resp_msg_v02));

  /*-----------------------------------------------------------------------
    Send request QMI_loc_GET_SYS_INFO_REQ_MSG to QMI-loc service.
    QMI_loc_GET_SYS_INFO_REQ message has no TLV.
  ------------------------------------------------------------------------*/
  if(QMI_NO_ERR !=
       qmi_client_send_msg_async(client_handle,
                                 QMI_LOC_REG_EVENTS_REQ_V02,
                                 LocRegEventsReqMsg_ptr,
                                 sizeof(qmiLocRegEventsReqMsgT_v02),
                                 loc_ind_reg_resp_msg_ptr,
                                 sizeof(estk_geo_loc_resp_msg_v02),
                                 estk_qmi_loc_request_resp,
                                 &client_cb_data,
                                 &get_sys_txn_hdl))
  {
   UIM_MSG_ERR_1( "Send loc get sys info request failed. Free mem 0x%x",
                               loc_ind_reg_resp_msg_ptr );
    gstk_free(loc_ind_reg_resp_msg_ptr);
    gstk_free(LocRegEventsReqMsg_ptr);
    LocRegEventsReqMsg_ptr = NULL;
    loc_ind_reg_resp_msg_ptr = NULL;
    return;
  }
  UIM_MSG_HIGH_0("Send loc start request successful" ); 
  gstk_free(LocRegEventsReqMsg_ptr);
  LocRegEventsReqMsg_ptr = NULL;

  /* Send QMI_LOC_GET_ENGINE_LOCK_REQ_V02 to know the status of geoloc engine */
  estk_get_engine_lock_req();
#endif  
} /* ESTK_QMI_LOC_CLIENT_REGISTER_EVENT */

/*===========================================================================
FUNCTION       ESTK_GET_NMEA_TYPE 

DESCRIPTION
  This function will send the QMI_LOC_SET_NMEA_TYPES_REQ_V02 command to QMI_LOC
  to start the location session.

DEPENDENCIES 
  FEATURE_ESTK_HAS_QMI_LOC 

RETURN VALUE 
  None

SIDE EFFECTS 
  None
===========================================================================*/
uint8 estk_get_pref_nmea_sentence_index (uint8 pref_nmea_type)
{
#ifdef FEATURE_GSTK_QMI_LOC
  uint8 i = 0;

  if(pref_nmea_type & 0x01)
  {
    if(nmea_cache[(uint8)ESTK_NMEA_RMC_TYPE].nmea_len > 0)
    {
      return (uint8)ESTK_NMEA_RMC_TYPE;
    }
  }
  else if(pref_nmea_type & 0x02)
  {
    if(nmea_cache[(uint8)ESTK_NMEA_GGA_TYPE].nmea_len > 0)
    {
      return (uint8)ESTK_NMEA_GGA_TYPE;
    }
  }
  else if(pref_nmea_type & 0x04)
  {
    if(nmea_cache[(uint8)ESTK_NMEA_RMC_TYPE].nmea_len > 0)
    {
      return (uint8)ESTK_NMEA_RMC_TYPE;
    }
  }  
  else if(pref_nmea_type & 0x08)
  {
    if(nmea_cache[(uint8)ESTK_NMEA_GNS_TYPE].nmea_len > 0)
    {
      return (uint8)ESTK_NMEA_GNS_TYPE;
    }
  }    

  while(i < ESTK_NMEA_MAX_TYPE)
  {
    if(nmea_cache[i].nmea_len > 0)
    {
      return i;
    }
    i++;
  }
#endif   
  return 0;
}

/*===========================================================================
FUNCTION       ESTK_GET_NMEA_TYPE 

DESCRIPTION
  This function will send the QMI_LOC_SET_NMEA_TYPES_REQ_V02 command to QMI_LOC
  to start the location session.

DEPENDENCIES 
  FEATURE_ESTK_HAS_QMI_LOC 

RETURN VALUE 
  None

SIDE EFFECTS 
  None
===========================================================================*/
void estk_cache_nmea_sentence (const uint8 *nmea, uint8 nmea_len)
{
#ifdef FEATURE_GSTK_QMI_LOC
  uint8 index = 0;
  uint8 i = 0;

  while((i < nmea_len) && (nmea[i] != ','))
  {
    i++;
  }

  if(!memcmp(nmea,"$GPRMC", i))
  {
    /* Comapring with ",,V" to avoid void NMEA sentence */
    if(!memcmp(&(nmea[i]),",,V", 3))
    {
      return;
    }
    index = 0;
  }
  else if(!memcmp(nmea,"$GPGGA", i))
  {
    /* Comapring with ",,,,,," to avoid void NMEA sentence */  
    if(!memcmp(&(nmea[i]),",,,,,,", 6))
    {
      return;
    }  
    index = 1;
  }
  else if(!memcmp(nmea,"$GNGNS", i))
  {
    i++;
    while((i < nmea_len) && (nmea[i] != ','))
    {
      i++;
    }
    /* Comapring with ",,,," to avoid void NMEA sentence */      
    if(!memcmp(&(nmea[i]),",,,,", 4))
    {
      return;
    }
    index = 2;
  }
  else
  {
    return;
  }
  
  if(nmea_cache[index].nmea_ptr == NULL)
  {
    nmea_cache[index].nmea_ptr = GSTK_CALLOC(GSTK_NMEA_LEN);
  }
  
  if(nmea_cache[index].nmea_ptr != NULL)
  {
    nmea_cache[index].nmea_type = (estk_nmea_sentence_type)index;
    nmea_cache[index].nmea_len = nmea_len;
    gstk_memcpy(nmea_cache[index].nmea_ptr,
                            nmea,
                            nmea_len,
                            GSTK_NMEA_LEN,
                            nmea_len);
  }
#endif
}

/*===========================================================================
FUNCTION       ESTK_GET_POSITION

DESCRIPTION
  This function will send the QMI_LOC_START_REQ_V02 command to QMI_LOC
  to start the location session.

DEPENDENCIES 
  FEATURE_ESTK_HAS_QMI_LOC 

RETURN VALUE 
  None

SIDE EFFECTS 
  None
===========================================================================*/
void estk_get_position
(
  uint32 gnss_update_time_ms
)
{
#ifdef FEATURE_GSTK_QMI_LOC
  qmi_txn_handle                    get_sys_txn_hdl;
  estk_geo_loc_resp_msg_v02        *loc_resp_msg_ptr = NULL;
  qmiLocStartReqMsgT_v02           *loc_req_msg_ptr = NULL;
  int                               client_cb_data = 0;

  UIM_MSG_HIGH_0("estk_get_position()" );

  client_cb_data = (int)estk_curr_inst_ptr->slot_id;
  
  /*-----------------------------------------------------------------------
    Allocate dynamic memory for response message. Memory will be freed
    during response message callback handler
  ------------------------------------------------------------------------*/
  loc_resp_msg_ptr
    = (estk_geo_loc_resp_msg_v02*)
        GSTK_CALLOC(sizeof(estk_geo_loc_resp_msg_v02));

  if(loc_resp_msg_ptr == NULL)
  {
    UIM_MSG_HIGH_0("Couldn't get data_block_ptr memory" );
    return;
  }

  loc_req_msg_ptr
    = (qmiLocStartReqMsgT_v02*)
        GSTK_CALLOC(sizeof(qmiLocStartReqMsgT_v02));

  if(loc_req_msg_ptr == NULL)
  {
    UIM_MSG_HIGH_0("Couldn't get data_block_ptr memory" );
    gstk_free(loc_resp_msg_ptr);
    loc_resp_msg_ptr = NULL;
    return;
  }
  memset(loc_req_msg_ptr,
         0,
         sizeof(qmiLocStartReqMsgT_v02));

  loc_req_msg_ptr->sessionId = 0xFF;
  /* Request for periodic Fixes */
  loc_req_msg_ptr->fixRecurrence_valid = FALSE;  /* Single Fix */
  /* Request for intermediate  fixes only */
  loc_req_msg_ptr->intermediateReportState_valid = TRUE;
  loc_req_msg_ptr->intermediateReportState = 0x00000001; /* 0x00000002 -- OFF; default: 0x00000001 -- ON */
  /* Set the minimum interval between fixes */
  loc_req_msg_ptr->minInterval_valid = TRUE;
  loc_req_msg_ptr->minInterval = gnss_update_time_ms;
  /* Set horizontal accuracy to HIGH */
  loc_req_msg_ptr->horizontalAccuracyLevel_valid = TRUE;
  loc_req_msg_ptr->horizontalAccuracyLevel = 0x00000003; /* 0x00000003 -- HIGH; default: 0x00000001 -- LOW */

  memset(loc_resp_msg_ptr,
         0,
         sizeof(estk_geo_loc_resp_msg_v02));

  /*-----------------------------------------------------------------------
    Send request QMI_loc_GET_SYS_INFO_REQ_MSG to QMI-loc service.
    QMI_loc_GET_SYS_INFO_REQ message has no TLV.
  ------------------------------------------------------------------------*/
  if(QMI_NO_ERR !=
       qmi_client_send_msg_async(client_handle,
                                 QMI_LOC_START_REQ_V02,
                                 loc_req_msg_ptr,
                                 sizeof(qmiLocStartReqMsgT_v02),
                                 loc_resp_msg_ptr,
                                 sizeof(estk_geo_loc_resp_msg_v02),
                                 estk_qmi_loc_request_resp,
                                 &client_cb_data,
                                 &get_sys_txn_hdl))
  {
    UIM_MSG_ERR_1( "Send loc get sys info request failed. Free mem 0x%x",
                               loc_resp_msg_ptr );
    (void)estk_send_terminal_response(
            estk_curr_inst_ptr->curr_cmd.hdr_cmd.cmd_detail_reference,         /* command_details_ref */
            GSTK_GEO_LOC_CNF,           /* command response */
            GSTK_ME_CURRENTLY_UNABLE_TO_PROCESS_COMMAND, /* general_result */
            NULL,               /* additional result */
            NULL);                                            
    gstk_free(loc_resp_msg_ptr);
    gstk_free(loc_req_msg_ptr);
    loc_resp_msg_ptr = NULL;
    loc_req_msg_ptr = NULL;
    return;
  }
  UIM_MSG_HIGH_0("Send loc start request successful" );
  gstk_free(loc_req_msg_ptr);
  loc_req_msg_ptr = NULL;
#endif  
} /* ESTK_GET_POSITION */

/*===========================================================================
FUNCTION       ESTK_END_POSITION_SESSION

DESCRIPTION
  This function will send the QMI_LOC_STOP_REQ_V02 command to QMI_LOC
  to stop the location session.

DEPENDENCIES 
  FEATURE_ESTK_HAS_QMI_LOC 

RETURN VALUE 
  None

SIDE EFFECTS 
  None
===========================================================================*/
void estk_end_position_session
(
  void
)
{
#ifdef FEATURE_GSTK_QMI_LOC
  qmi_txn_handle             get_sys_txn_hdl;
  estk_geo_loc_resp_msg_v02 *loc_resp_msg_ptr = NULL;
  qmiLocStopReqMsgT_v02     *loc_req_msg_ptr  = NULL;
  int                        client_cb_data   = 0;
  uint8 index = 0;
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  UIM_MSG_HIGH_0("estk_end_position_session()" );

  for(index = 0; index < ESTK_NMEA_MAX_TYPE; index++)
  {
    if(nmea_cache[index].nmea_ptr != NULL)
    {
      gstk_free(nmea_cache[index].nmea_ptr);
      nmea_cache[index].nmea_ptr = NULL;
    }
    nmea_cache[index].nmea_len = 0;
    nmea_cache[index].nmea_type = ESTK_NMEA_MAX_TYPE;
  }
  
  /*-----------------------------------------------------------------------
    Allocate dynamic memory for response message. Memory will be freed
    during response message callback handler
  ------------------------------------------------------------------------*/
  loc_resp_msg_ptr
    = (estk_geo_loc_resp_msg_v02*)
        GSTK_CALLOC(sizeof(estk_geo_loc_resp_msg_v02));

  if(loc_resp_msg_ptr == NULL)
  {
    UIM_MSG_HIGH_0("Couldn't get data_block_ptr memory" );
    return;
  }

  loc_req_msg_ptr
    = (qmiLocStopReqMsgT_v02*)
        GSTK_CALLOC(sizeof(qmiLocStopReqMsgT_v02));

  if(loc_req_msg_ptr == NULL)
  {
    UIM_MSG_HIGH_0("Couldn't get data_block_ptr memory" );
    gstk_free(loc_resp_msg_ptr);
    loc_resp_msg_ptr = NULL;
    return;
  }
  memset(loc_req_msg_ptr,
         0,
         sizeof(qmiLocStopReqMsgT_v02));

  loc_req_msg_ptr->sessionId = 0xFF;

  memset(loc_resp_msg_ptr,
         0,
         sizeof(estk_geo_loc_resp_msg_v02));

  /*-----------------------------------------------------------------------
    Send request QMI_LOC_STOP_REQ_V02 to QMI-loc service.
    QMI_loc_GET_SYS_INFO_REQ message has no TLV.
  ------------------------------------------------------------------------*/
  if(QMI_NO_ERR !=
       qmi_client_send_msg_async(client_handle,
                                 QMI_LOC_STOP_REQ_V02,
                                 loc_req_msg_ptr,
                                 sizeof(qmiLocStopReqMsgT_v02),
                                 loc_resp_msg_ptr,
                                 sizeof(estk_geo_loc_resp_msg_v02),
                                 estk_qmi_loc_request_resp,
                                 &client_cb_data,
                                 &get_sys_txn_hdl))
  {
   UIM_MSG_ERR_1( "Send loc stop failed. Free mem 0x%x",
                               loc_resp_msg_ptr );
    gstk_free(loc_resp_msg_ptr);
    gstk_free(loc_req_msg_ptr);
    loc_resp_msg_ptr = NULL;
    loc_req_msg_ptr = NULL;
    return;
  }
  UIM_MSG_HIGH_0("Send loc stop request successful" );
  gstk_free(loc_req_msg_ptr);
  loc_req_msg_ptr = NULL;
#endif  
} /* ESTK_END_POSITION_SESSION */

/*==========================================================================
FUNCTION ESTK_QMI_LOC_CLIENT_PROCESS_SIGNAL

DESCRIPTION 
  Funtion to process LOC service notify signal. This function is
  used to process signal sent to estk_ivs_task by QCCI notifying LOC
  service. 
    
DEPENDENCIES
  FEATURE_ESTK_HAS_QMI_LOC

RETURN VALUE
  None

SIDE EFFECTS 
  None 
===========================================================================*/
void estk_qmi_loc_client_process_signal
(
  void
)
{
#ifdef FEATURE_GSTK_QMI_LOC
  qmi_client_error_type                       retval;
  qmi_service_info                            info;
  unsigned int                                num_entries         = 1;
  unsigned int                                num_services;
  qmi_idl_service_object_type                 loc_service_obj     = NULL;
  int                                         client_cb_data      = 0;

  UIM_MSG_HIGH_0("estk_qmi_loc_client_process_signal()" );

  /*-----------------------------------------------------------------------
     If client_handle!=NULL, then client_init aleady done. The signal might
     be false signal. Return immediatley.
  -----------------------------------------------------------------------*/
  if(client_handle != NULL)
  {
   UIM_MSG_ERR_1( "False signal, client handle: 0x%x not NULL. RETURN",
                               client_handle );
    return;
  }

  /*-----------------------------------------------------------------------
     Get LOC service object
  -----------------------------------------------------------------------*/
  loc_service_obj = loc_get_service_object_v02();
  if(!loc_service_obj)
  {
    UIM_MSG_HIGH_0("Unable to get loc service object" );
    return;
  }

  /*------------------------------------------------------------------
     Check if the loc service is up, if not return. Else,
     Get addressing information for accessing loc service
  ------------------------------------------------------------------*/
  if(QMI_NO_ERR != qmi_client_get_service_list(loc_service_obj,
                                               &info,
                                               &num_entries,
                                               &num_services))
  {
    UIM_MSG_HIGH_0("QMI-loc service not up, continue to wait for signal" );
    return;
  }

  UIM_MSG_HIGH_0("QMI-loc Service up" );
  
  /*-----------------------------------------------------------------------
    Release the notifier handlle. QMI Service notify signal need not be
    handled anymore.
    QCCI uses notify signal during power-up to signal the
    availability of QMI service. Once signal is processed, release the
    notifier_handle, so that QCCI will not send anymore signals.
  ------------------------------------------------------------------------*/
  if(QMI_NO_ERR != qmi_client_release(notifier_handle))
  {
    UIM_MSG_HIGH_0("Notifier release failed" );
    notifier_handle = NULL;
    return;
  }

  /*-----------------------------------------------------------------------
    Initialize the arguments for QMI Client Init
  ------------------------------------------------------------------------*/
  memset(&client_os_params,0, sizeof(qmi_client_os_params));
  client_os_params.tcb = rex_self();
  client_os_params.sig = ESTK_WAIT_QMI_LOC_MSG_SIG;

  /*-----------------------------------------------------------------------
       Initialize connection to the loc service
  -----------------------------------------------------------------------*/
  retval =  qmi_client_init(&info,
                            loc_service_obj,
                            estk_qmi_loc_client_ind_cb,
                            &client_cb_data,
                            &client_os_params,
                            &client_handle);

  if((retval != QMI_NO_ERR) || (!client_handle))
  {
     UIM_MSG_HIGH_0("QMI loc client init failed" );
     client_handle = NULL;
     return;
  }
  UIM_MSG_HIGH_0("QMI loc client init successful" );

  /*-----------------------------------------------------------------------
      Register for Error callback
  ------------------------------------------------------------------------*/
  if(QMI_NO_ERR != qmi_client_register_error_cb(client_handle,
                                                estk_qmi_loc_client_error_cb,
                                                &client_cb_data))
  {
    UIM_MSG_HIGH_0("QMI loc register error callback failed" );
  }
  UIM_MSG_HIGH_0("QMI loc register error callback successful" );

  /*-----------------------------------------------------------------------
      Register for the QMI_LOC events
  ------------------------------------------------------------------------*/
  estk_qmi_loc_client_register_event();
#endif  
}/*estk_qmi_loc_client_process_signals()*/

/*===========================================================================
FUNCTION       ESTK_QMI_LOC_CLIENT_IND_CB

DESCRIPTION 
  QMI-LOC indication callback handler. This function is called
  by QCCI whenver there is a LOC indication message.

DEPENDENCIES  

RETURN VALUE 
  None

SIDE EFFECTS 
  None
===========================================================================*/
void  estk_qmi_loc_client_ind_cb
(
  qmi_client_type                user_handle,
  unsigned int                   msg_id,
  void                          *ind_buf_ptr,
  unsigned int                   ind_buf_len,
  void                          *ind_cb_ptr
)
{
#ifdef FEATURE_GSTK_QMI_LOC
  qmi_client_error_type   retval          = QMI_NO_ERR;
  void                   *LocEventIndPtr  = NULL;
  estk_cmd_type          *task_cmd_ptr    = NULL;
  gstk_slot_id_enum_type  slot_id         = GSTK_SLOT_AUTO;
  size_t                  ind_size        = 0;

  UIM_MSG_HIGH_1("In estk_qmi_loc_client_ind_cb(), msg_id is 0x%x", msg_id);

  /* Check if indication buffer pointer is valid */
  if (ind_buf_ptr == NULL)
  {
    UIM_MSG_HIGH_0("Indication buffer pointer is NULL");
    return;
  }

  /* Check if indication callback pointer is valid */
  if (ind_cb_ptr == NULL)
  {
    UIM_MSG_HIGH_0("Indication callback pointer is NULL");
    return;
  }

  /* Check if msg id is correct */
  if (msg_id != QMI_LOC_EVENT_POSITION_REPORT_IND_V02 &&
      msg_id != QMI_LOC_EVENT_NMEA_IND_V02 &&
      msg_id != QMI_LOC_EVENT_ENGINE_LOCK_STATE_IND_V02 &&
      msg_id != QMI_LOC_GET_ENGINE_LOCK_IND_V02)
  {
    UIM_MSG_ERR_1("estk_qmi_loc_client_ind_cb(): Invalid msg_id: %u",
                   msg_id);
    return;
  }
  else
  {
    if (msg_id == QMI_LOC_EVENT_POSITION_REPORT_IND_V02)
    {
      ind_size = sizeof(qmiLocEventPositionReportIndMsgT_v02);
    }
    else if (msg_id == QMI_LOC_EVENT_NMEA_IND_V02)
    {
      ind_size = sizeof(qmiLocEventNmeaIndMsgT_v02);
    }
    else if (msg_id == QMI_LOC_EVENT_ENGINE_LOCK_STATE_IND_V02)
    {
      ind_size = sizeof(qmiLocEventEngineLockStateIndMsgT_v02);
    }
    else if (msg_id == QMI_LOC_GET_ENGINE_LOCK_IND_V02)
    {
      ind_size = sizeof(qmiLocGetEngineLockIndMsgT_v02);
    }
  }

  slot_id = estk_geo_loc_slot_id_init();

  /* Allocate dynamic memory for indication message */
  LocEventIndPtr = (void *)gstk_mem_alloc(ind_size);

  /* Decode the ESTK Indication message using QCCI api */
  retval = qmi_client_message_decode(user_handle,
                                     QMI_IDL_INDICATION,
                                     msg_id,
                                     ind_buf_ptr,
                                     ind_buf_len,
                                     LocEventIndPtr,
                                     ind_size);

  if (QMI_NO_ERR != retval)
  {
    gstk_free(LocEventIndPtr);
    return;
  }

  task_cmd_ptr = estk_get_cmd_buf();
  if (task_cmd_ptr == NULL)
  {
    gstk_free(LocEventIndPtr);
    return;       
  }  

  if (msg_id == QMI_LOC_EVENT_POSITION_REPORT_IND_V02)
  {
    qmiLocEventPositionReportIndMsgT_v02 *LocEventPositionReportInd_ptr  = (qmiLocEventPositionReportIndMsgT_v02 *)LocEventIndPtr;
    UIM_MSG_HIGH_1("loc event position report status : 0x%x", LocEventPositionReportInd_ptr->sessionStatus);

    /* Post a command to the eCall App task to process this GPS fix */
    task_cmd_ptr->hdr.cmd   = ESTK_LOC_EVENT_POSITION_REPORT_IND;
    task_cmd_ptr->hdr.sim_slot_id = slot_id;
    switch (LocEventPositionReportInd_ptr->sessionStatus)
    {
      case eQMI_LOC_SESS_STATUS_SUCCESS_V02:
        task_cmd_ptr->cmd_data.gps_data.geo_loc_status = ESTK_GEO_LOC_SESS_STATUS_SUCCESS;
        break;
      case eQMI_LOC_SESS_STATUS_IN_PROGRESS_V02:
        task_cmd_ptr->cmd_data.gps_data.geo_loc_status = ESTK_GEO_LOC_SESS_STATUS_IN_PROGRESS;
        break;          
      default:
        task_cmd_ptr->cmd_data.gps_data.geo_loc_status = ESTK_GEO_LOC_SESS_STATUS_GENERAL_FAILURE;
        break;            
    }
  }

  else if (msg_id == QMI_LOC_EVENT_NMEA_IND_V02)
  {
    qmiLocEventNmeaIndMsgT_v02 *LocEventNmeaInd_ptr = (qmiLocEventNmeaIndMsgT_v02 *)LocEventIndPtr;

    /* Post a command to the eCall App task to process this GPS fix */
    task_cmd_ptr->hdr.cmd = ESTK_LOC_EVENT_NMEA_IND;
    task_cmd_ptr->hdr.sim_slot_id = slot_id;
    task_cmd_ptr->cmd_data.gps_data.nmea_len = (uint8)strlen(LocEventNmeaInd_ptr->nmea);
    UIM_MSG_HIGH_1("loc event NMEA indication, nmea length : 0x%x", task_cmd_ptr->cmd_data.gps_data.nmea_len);
    if(task_cmd_ptr->cmd_data.gps_data.nmea_len == 0)
    {
      estk_free_cmd_buf(task_cmd_ptr);
      gstk_free(LocEventIndPtr);
      return;
    }
    else
    {
      gstk_memcpy(task_cmd_ptr->cmd_data.gps_data.nmea,
                  LocEventNmeaInd_ptr->nmea,
                  task_cmd_ptr->cmd_data.gps_data.nmea_len,
                  sizeof(task_cmd_ptr->cmd_data.gps_data.nmea),
                  task_cmd_ptr->cmd_data.gps_data.nmea_len);
    }
  }

  else if (msg_id == QMI_LOC_EVENT_ENGINE_LOCK_STATE_IND_V02)
  {
    qmiLocEventEngineLockStateIndMsgT_v02 *LocEventEngineLockStateInd_ptr = (qmiLocEventEngineLockStateIndMsgT_v02 *)LocEventIndPtr;
    UIM_MSG_HIGH_1("get engine lock state indication lockclient : 0x%x", LocEventEngineLockStateInd_ptr->lockClient);
    
    /* Post a command to the eCall App task to process this GPS fix */
    task_cmd_ptr->hdr.cmd = ESTK_LOC_EVENT_ENGINE_LOCK_STATE_IND;
    task_cmd_ptr->hdr.sim_slot_id = slot_id;

    /* Check for NFW Lock Status. If location engine is locked for NFW client then update the lock_state as locked. */
    if (LocEventEngineLockStateInd_ptr->lockClient & QMI_LOC_LOCK_CLIENT_MASK_NFW_V02)
    {
      task_cmd_ptr->cmd_data.gps_data.geo_loc_engine_lock_state =  ESTK_GEO_LOC_ENGINE_LOCK_STATE_DISABLED;
    }
    else
    {
      task_cmd_ptr->cmd_data.gps_data.geo_loc_engine_lock_state =  ESTK_GEO_LOC_ENGINE_LOCK_STATE_ENABLED;
    }
  }

  else if (msg_id == QMI_LOC_GET_ENGINE_LOCK_IND_V02)
  {
    qmiLocGetEngineLockIndMsgT_v02 *LocEventGetEngineLockInd_ptr = (qmiLocGetEngineLockIndMsgT_v02 *) LocEventIndPtr;
    UIM_MSG_HIGH_1("get engine lock status is : 0x%x", LocEventGetEngineLockInd_ptr->lockClient);

    /* Post a command to the eCall App task to process this GPS fix */
    task_cmd_ptr->hdr.cmd = ESTK_LOC_GET_ENGINE_LOCK_IND;
    task_cmd_ptr->hdr.sim_slot_id = slot_id;

    /* Check for NFW Lock Status. If location engine is locked for NFW client then update the lock_state as locked. */
    if (LocEventGetEngineLockInd_ptr->lockClient & QMI_LOC_LOCK_CLIENT_MASK_NFW_V02)
    {
      task_cmd_ptr->cmd_data.gps_data.geo_loc_engine_lock_state =  ESTK_GEO_LOC_ENGINE_LOCK_STATE_DISABLED;
    }
    else
    {
      task_cmd_ptr->cmd_data.gps_data.geo_loc_engine_lock_state =  ESTK_GEO_LOC_ENGINE_LOCK_STATE_ENABLED;
    }
  }

  /* Release the memory allocated for indication message */
  gstk_free(LocEventIndPtr);

  estk_put_cmd(task_cmd_ptr);
#endif  
  return;
}/*estk_qmi_loc_client_ind_cb()*/

/*===========================================================================
FUNCTION ESTK_QMI_LOC_CLIENT_ERROR_CB

DESCRIPTION 
  QMI_LOC error callback handler. This callback is called by QCCI
  to notify error.

DEPENDENCIES 
  FEATURE_ESTK_HAS_QMI_LOC 

RETURN VALUE 
  None 

SIDE EFFECTS 
  None 
===========================================================================*/
void estk_qmi_loc_client_error_cb
(
  qmi_client_type user_handle,
  qmi_client_error_type error,
  void *err_cb_data
)
{
#ifdef FEATURE_GSTK_QMI_LOC
  (void)user_handle;
  UIM_MSG_HIGH_0("estk_qmi_loc_client_error_cb()");

  /*---------------------------------------------------------------------
     Check if err cb data pointer is valid
  ---------------------------------------------------------------------*/
  if(err_cb_data == NULL)
  {
    UIM_MSG_ERR_0("Null pointer passed" );
    return;
  }

  switch(error)
  {
    /*-----------------------------------------------------------------------
      In case of service error, client will be released. No attempt to recover
      the QMI connection will be made.
    ------------------------------------------------------------------------*/
    case QMI_SERVICE_ERR:
      UIM_MSG_ERR_0("QMI_LOC service is down" );
      /*-----------------------------------------------------------------------
        Release the client.
      ------------------------------------------------------------------------*/
      if(QMI_NO_ERR != qmi_client_release(client_handle))
      {
        UIM_MSG_ERR_0("Client release failed" );
        return;
      }

      client_handle = NULL;
      break;

    default:
      UIM_MSG_ERR_0("estk_qmi_loc_client_error_cb()");
  }
#endif  
  return;
}

/*===========================================================================
FUNCTION: ESTK_PROCESS_GEO_LOC_REQ

DESCRIPTION:
  ESTK calls this ESTK function to process the Geographical location request.

PARAMETERS:
  estk_req_ptr : [Input] estk geographical location cmd pointer

DEPENDENCIES:
  None

RETURN VALUE:
  estk_status_enum_type
    ESTK_SUCCESS: - When GEO lcoation is successfully handled
    ESTK_ERROR:   - When there is failure in handling GEO location

COMMENTS:
  None

SIDE EFFECTS:
  None

===========================================================================*/
estk_result_enum_type estk_process_geo_loc_req
(
  gstk_cmd_from_card_type *gstk_req_ptr
)
{
#ifdef FEATURE_GSTK_QMI_LOC
  uint8       pref_max_rsp_time           = 07;
  uint8       index                       = 0;
  boolean     send_tr                     = FALSE;
  boolean     start_timer                 = FALSE;
  
  ESTK_RETURN_ERROR_IF_NULL_PTR(estk_curr_inst_ptr, ESTK_ERROR);

  UIM_MSG_HIGH_1("estk_process_geo_loc_req: estk_req_ptr=0x%x", gstk_req_ptr);

  /* Check input parameter */
  if (gstk_req_ptr == NULL)
  {
    return ESTK_ERROR;
  }

  /* If location engine is locked, send out error TR */
  if (estk_curr_inst_ptr->estk_geo_loc_engine_lock_status == TRUE)
  {
    (void)estk_send_terminal_response(
            estk_curr_inst_ptr->curr_cmd.hdr_cmd.cmd_detail_reference,
            GSTK_GEO_LOC_CNF,
            GSTK_ME_CURRENTLY_UNABLE_TO_PROCESS_COMMAND,
            NULL,
            NULL);
    return ESTK_SUCCESS;
  }

  estk_curr_inst_ptr->state = ESTK_PENDING_TR_ST;

  if(gstk_nv_get_me_feature_status(GSTK_CFG_FEATURE_ME_ESTK_IGNORE_ALPHA) == FALSE)
  {
    /* Check Alpha availability */
    /* Even in case of null data object in ALPHA (i.e. length = '00' and no value part),
       ESTK make sures at least NULL char is present in alpha text and length includes
       that NULL character. So the min length is one and hence we need to check for
       length more than one */
    if(((gstk_req_ptr->cmd.geo_loc_pro_cmd_req.alpha.length) > 1 &&
        (gstk_req_ptr->cmd.geo_loc_pro_cmd_req.alpha.alphabet)) ||
       (gstk_nv_get_me_feature_status(
           GSTK_CFG_FEATURE_ME_ESTK_QMI_SEND_NO_AND_NULL_ALPHA_IND) == TRUE))
    {
      if (estk_send_alpha_to_display_client(gstk_req_ptr, TRUE) != ESTK_SUCCESS)
      {
        UIM_MSG_ERR_0("Send Geographical location request Display failed");
        /* priv data allocated is freed when a TR is sent or
                    when a End Proactive Session is received */
        return ESTK_ERROR;
      }
    }
  }

  /* inialized with 1000 to convert second to milisecond */
  geo_loc_pref_max_resp_time = 1000;
  
  pref_max_rsp_time = gstk_req_ptr->cmd.geo_loc_pro_cmd_req.geo_loc_para.pref_max_rsp_time;
  
  pref_max_rsp_time = (pref_max_rsp_time > 7)? 7 : ((pref_max_rsp_time < 2)? 2 : pref_max_rsp_time);
  
  for(index = 0; index < pref_max_rsp_time; index++)
  {
    geo_loc_pref_max_resp_time *= 2;
  }

  estk_curr_inst_ptr->pref_nmea_types = 
    gstk_req_ptr->cmd.geo_loc_pro_cmd_req.geo_loc_para.pref_NMEA_sentences;

  if(geo_loc_slot_mask != 0)
  {
    send_tr = TRUE;
  }
  
  switch(estk_curr_inst_ptr->slot_id)
  {
    case GSTK_SLOT_1:
      if(!(geo_loc_slot_mask & ESTK_MASK_SLOT_1))
      {
        start_timer = TRUE;
      }    
      geo_loc_slot_mask |= ESTK_MASK_SLOT_1;  
      break;
    case GSTK_SLOT_2:
      if(!(geo_loc_slot_mask & ESTK_MASK_SLOT_2))
      {
        start_timer = TRUE;                                                             
      }    
      geo_loc_slot_mask |= ESTK_MASK_SLOT_2;
      break;
    case GSTK_SLOT_3:
      if(!(geo_loc_slot_mask & ESTK_MASK_SLOT_3))
      {
        start_timer = TRUE;                                 
      }    
      geo_loc_slot_mask |= ESTK_MASK_SLOT_3;
      break;
    default:
      break;
  }

  if(send_tr == TRUE)
  {
    (void)estk_send_terminal_response(
            estk_curr_inst_ptr->curr_cmd.hdr_cmd.cmd_detail_reference,         /* command_details_ref */
            GSTK_GEO_LOC_CNF,           /* command response */
            GSTK_COMMAND_PERFORMED_SUCCESSFULLY, /* general_result */
            NULL,               /* additional result */
            NULL);
     if(start_timer == TRUE)
     {
       (void)rex_clr_timer(&(estk_curr_inst_ptr->geo_loc_rsp_timer));
       (void)rex_set_timer(&(estk_curr_inst_ptr->geo_loc_rsp_timer),
                            geo_loc_pref_max_resp_time);
     }                          
     return ESTK_SUCCESS;
  }

  estk_get_position(ESTK_QMI_LOC_REPORT_INTERVAL);
#endif    
  return ESTK_SUCCESS;
}

/*===========================================================================
FUNCTION: ESTK_PROCESS_GPS_DATA_CMD

DESCRIPTION :
  This function will be called by ESTK to process GPS data response from QMI_LOC

PARAMETERS:
  cmd_ptr : [Input] ESTK command pointer

DEPENDENCIES:
  None

RETURN VALUE:
  estk_result_enum_type
    ESTK_SUCCESS : If processing of WMS event is successful
    ESTK_ERROR   : If processing results in error

COMMENTS:
  None

SIDE EFFECTS:
  None

===========================================================================*/
estk_result_enum_type estk_process_gps_data_cmd(
  const estk_cmd_type *cmd_ptr
)
{
#ifdef FEATURE_GSTK_QMI_LOC
  gstk_general_result_enum_type gstk_result         = GSTK_COMMAND_PERFORMED_SUCCESSFULLY;
  uint32                        index               = 0;
  static boolean                is_tr_success       = FALSE;  

  if (cmd_ptr == NULL)
  {
    UIM_MSG_HIGH_0("Null pointer passed");
    return ESTK_ERROR;
  }

  switch(cmd_ptr->hdr.cmd)
  {
    case ESTK_LOC_EVENT_NMEA_IND:
      estk_cache_nmea_sentence(cmd_ptr->cmd_data.gps_data.nmea, 
                               cmd_ptr->cmd_data.gps_data.nmea_len);
      break;
      
    case ESTK_LOC_EVENT_POSITION_REPORT_IND:
      switch (cmd_ptr->cmd_data.gps_data.geo_loc_status)
      {
        case ESTK_GEO_LOC_SESS_STATUS_SUCCESS:
        case ESTK_GEO_LOC_SESS_STATUS_GENERAL_FAILURE:
          for (index = 0; index < estk_num_of_instances; ++index)
          {
            /* If sent ERROR terminal response to card then dont 
               send empty envelope to card 
               If sent SUCCESS terminal response to card but not able to 
               lock to the GPS, send empty envelope to card */
            if((rex_get_timer(&(estk_instances_ptr[index]->geo_loc_rsp_timer)) > 0) &&
               (is_tr_success))
            {        
              estk_slot_send_envelope_geo_loc_report_cmd((gstk_slot_id_enum_type)(index + 1));
              rex_clr_timer(&(estk_instances_ptr[index]->geo_loc_rsp_timer));
            }
          }
          estk_end_position_session();
          break;
        default:
          /* do nothing */
          break;
      }
      break;
      
    case ESTK_GEO_LOC_REQUEST_RSP:
      gstk_result = GSTK_ME_CURRENTLY_UNABLE_TO_PROCESS_COMMAND;
      is_tr_success = FALSE;
      if(cmd_ptr->cmd_data.gps_data.request_rsp_status)
      {
        gstk_result = GSTK_COMMAND_PERFORMED_SUCCESSFULLY;
        is_tr_success = TRUE;
      }     
      
      (void)estk_send_terminal_response(
              estk_curr_inst_ptr->curr_cmd.hdr_cmd.cmd_detail_reference,         /* command_details_ref */
              GSTK_GEO_LOC_CNF,           /* command response */
              gstk_result,           /* general_result */
              NULL,           /* additional result */
              NULL);        
      (void)rex_clr_timer(&(estk_curr_inst_ptr->geo_loc_rsp_timer));
      (void)rex_set_timer(&(estk_curr_inst_ptr->geo_loc_rsp_timer),
                          geo_loc_pref_max_resp_time);
      break;
    case ESTK_LOC_EVENT_ENGINE_LOCK_STATE_IND:
    case ESTK_LOC_GET_ENGINE_LOCK_IND:
      if (cmd_ptr->cmd_data.gps_data.geo_loc_engine_lock_state == ESTK_GEO_LOC_ENGINE_LOCK_STATE_DISABLED)
      {
        estk_curr_inst_ptr->estk_geo_loc_engine_lock_status = TRUE;
      }
      else
      {
        estk_curr_inst_ptr->estk_geo_loc_engine_lock_status = FALSE;
      }
      break;
      
    default:
      break;
  }
#endif  
  return ESTK_SUCCESS;
}

/*===========================================================================
FUNCTION: ESTK_GEO_LOC_RSP_TIMER_CB

DESCRIPTION :
  The callback funciton supplied to when geographical location timer expire

PARAMETERS:
  mask: mask contain slot information.

DEPENDENCIES:
  None

RETURN VALUE:
  None

COMMENTS:
  None

SIDE EFFECTS:
  None

===========================================================================*/
void estk_geo_loc_rsp_timer_cb(uint32 slot)
{
#ifdef FEATURE_GSTK_QMI_LOC
  estk_cmd_type          *cmd_ptr = NULL;
  gstk_slot_id_enum_type  slot_id = GSTK_SLOT_ID_MAX;

  UIM_MSG_HIGH_0("estk_geo_loc_rsp_timer_cb");

  slot_id = (gstk_slot_id_enum_type)slot;

  if (!ESTK_IS_VALID_SLOT_ID(slot_id))
  {
    return;
  }

  cmd_ptr = estk_get_cmd_buf();
  if (cmd_ptr != NULL)
  {
    cmd_ptr->hdr.cmd            = ESTK_GEO_LOC_RSP_TIMER_EXP_CMD;
    cmd_ptr->hdr.sim_slot_id   = slot_id;
    estk_put_cmd(cmd_ptr);
  }
#endif  
} /* estk_bip_keep_ch_open_timer_cb */

/*===========================================================================

FUNCTION: ESTK_PROCESS_GEO_LOC_RSP_TIMEOUT

DESCRIPTION:
  This ESTK function builds a geographical location envelopel with less accuracy if final 
  report is received from QMI_LOC.

PARAMETERS:
  None

DEPENDENCIES:
  None

RETURN VALUE:
  estk_result_enum_type

COMMENTS:
  None

SIDE EFFECTS:
  None

===========================================================================*/
estk_result_enum_type estk_process_geo_loc_rsp_timeout(void)
{
  estk_result_enum_type estk_result = ESTK_SUCCESS;
#ifdef FEATURE_GSTK_QMI_LOC
  boolean  is_geo_loc_rsp_timer_running = FALSE;
  uint32   i                            = 0;

  estk_result = estk_slot_send_envelope_geo_loc_report_cmd(estk_curr_inst_ptr->slot_id);
  
  for (i = 0; i < estk_num_of_instances; ++ i)
  {
    if(rex_get_timer(&(estk_instances_ptr[i]->geo_loc_rsp_timer)) > 0)
    {       
      is_geo_loc_rsp_timer_running = TRUE;
    }
  }
  if(is_geo_loc_rsp_timer_running != TRUE)
  {
    estk_end_position_session();
  }
  rex_clr_timer(&(estk_curr_inst_ptr->geo_loc_rsp_timer));  
#endif  
  return estk_result;
}

/*===========================================================================

FUNCTION: ESTK_SLOT_SEND_ENVELOPE_GEO_LOC_REPORT_CMD

DESCRIPTION:
  This ESTK function builds a geographical location envelopel and send it to UICC

PARAMETERS:
  slot_id : [Input] Slot_id

DEPENDENCIES:
  None

RETURN VALUE:
  estk_result_enum_type

COMMENTS:
  None

SIDE EFFECTS:
  None

===========================================================================*/
estk_result_enum_type estk_slot_send_envelope_geo_loc_report_cmd(
  gstk_slot_id_enum_type slot_id
)
{
#ifdef FEATURE_GSTK_QMI_LOC
  gstk_nmea_sentence_type              temp_nmea;
  uint8                                index       = 0;

  UIM_MSG_HIGH_0("estk_slot_send_envelope_geo_loc_report_cmd");

  memset(&temp_nmea, 0, sizeof(temp_nmea));
  
  if(!ESTK_IS_VALID_SLOT_ID(slot_id) || geo_loc_slot_mask == 0)
  {
    return ESTK_ERROR;
  }

  switch (slot_id)
  {
    case GSTK_SLOT_1:
      geo_loc_slot_mask &= ~ESTK_MASK_SLOT_1;
      break;
    case GSTK_SLOT_2:
      geo_loc_slot_mask &= ~ ESTK_MASK_SLOT_2;
      break;
    case GSTK_SLOT_3:
      geo_loc_slot_mask &= ~ESTK_MASK_SLOT_3;
      break;
    default:
      break;
  }

  index = estk_get_pref_nmea_sentence_index(estk_instances_ptr[slot_id-1]->pref_nmea_types);
   
  temp_nmea.length = nmea_cache[index].nmea_len;
  
  if(temp_nmea.length > 0)
  {
    temp_nmea.nmea_ptr = GSTK_CALLOC(temp_nmea.length);
    if(temp_nmea.nmea_ptr == NULL)
    {
      UIM_MSG_HIGH_0("No memory available");
      return ESTK_ERROR;
    }
  
    memset(temp_nmea.nmea_ptr, 0, temp_nmea.length);
    
    gstk_memcpy(temp_nmea.nmea_ptr,
                 nmea_cache[index].nmea_ptr,
                 nmea_cache[index].nmea_len,
                 temp_nmea.length,
                 nmea_cache[index].nmea_len);
  }

  gstk_slot_send_envelope_geo_loc_report_cmd(slot_id,
                                                  0,
                                                  &temp_nmea,
                                                  estk_envelope_geo_loc_rsp_cb);                                          
      
  if(temp_nmea.nmea_ptr != NULL)
  {
    gstk_free(temp_nmea.nmea_ptr);
    temp_nmea.nmea_ptr = NULL;
  }  
#endif  
  return ESTK_SUCCESS;
}

/*===========================================================================

FUNCTION: ESTK_ENVELOPE_GEO_LOC_RSP_CB

DESCRIPTION:
  This ESTK function is a callback for the geographical location report response from UICC

PARAMETERS:
  req_rsp : [Input] GSTK response from UICC

DEPENDENCIES:
  None

RETURN VALUE:
  Npne

COMMENTS:
  None

SIDE EFFECTS:
  None

===========================================================================*/
void estk_envelope_geo_loc_rsp_cb (
  gstk_cmd_from_card_type *         req_rsp)
{
  (void)req_rsp;
  UIM_MSG_HIGH_0("estk_envelope_geo_loc_rsp_cb");  
}

/*===========================================================================
FUNCTION:   estk_gps_full_clean_up_slot

DESCRIPTION :
  Free memory allocated for the GPS datal

PARAMETERS:
  slot_id : [Input] slot on which the gps data to be cleaned

DEPENDENCIES:
  None

RETURN VALUE:
  None

COMMENTS:
  None

SIDE EFFECTS:
  None

===========================================================================*/
void estk_gps_full_clean_up_slot(gstk_slot_id_enum_type slot_id)
{
#ifdef FEATURE_GSTK_QMI_LOC
  boolean  is_geo_loc_rsp_timer_running = FALSE;  
  uint8    i                            = 0;
  
  UIM_MSG_HIGH_1("estk_gps_full_clean_up_slot: slot_id=0x%x", slot_id);

  if (!ESTK_IS_VALID_SLOT_ID(slot_id))
  {
    return;
  }
  if(rex_get_timer(&(estk_instances_ptr[(uint32)slot_id - 1]->geo_loc_rsp_timer))>0)
  {
    switch (slot_id)
    {
      case GSTK_SLOT_1:
        geo_loc_slot_mask &= ~ESTK_MASK_SLOT_1;
        break;
      case GSTK_SLOT_2:
        geo_loc_slot_mask &= ~ ESTK_MASK_SLOT_2;
        break;
      case GSTK_SLOT_3:
        geo_loc_slot_mask &= ~ESTK_MASK_SLOT_3;
        break;
      default:
        break;
    }
    (void)rex_clr_timer(&(estk_instances_ptr[(uint32)slot_id - 1]->geo_loc_rsp_timer));
  }
  
  for (i = 0; i < estk_num_of_instances; ++ i)
  {
    if(rex_get_timer(&(estk_instances_ptr[i]->geo_loc_rsp_timer)) > 0)
    {       
      is_geo_loc_rsp_timer_running = TRUE;
    }
  }
  if(is_geo_loc_rsp_timer_running != TRUE)
  {
    estk_end_position_session();
  }
#endif  
}
/*===========================================================================
FUNCTION       ESTK_GET_ENGINE_LOCK_REQ

DESCRIPTION
  This function will send the QMI_LOC_GET_ENGINE_LOCK_REQ_V02 command to QMI_LOC
  to get the status of location engine.

DEPENDENCIES 
  None

RETURN VALUE 
  None

SIDE EFFECTS 
  None
===========================================================================*/
void estk_get_engine_lock_req
(
  void
)
{
#ifdef FEATURE_GSTK_QMI_LOC
  qmi_txn_handle                       get_sys_txn_hdl;
  estk_geo_loc_resp_msg_v02           *loc_resp_msg_ptr = NULL;
  qmiLocGetEngineLockReqMsgT_v02      *loc_req_msg_ptr  = NULL;
  int                                  client_cb_data   = 0;

  UIM_MSG_HIGH_0("estk_get_engine_lock_req()");

  /* Allocate dynamic memory for request message */
  loc_req_msg_ptr = (qmiLocGetEngineLockReqMsgT_v02 *)gstk_mem_alloc(sizeof(qmiLocGetEngineLockReqMsgT_v02));

  /* Allocate dynamic memory for response message */
  loc_resp_msg_ptr = (estk_geo_loc_resp_msg_v02 *)gstk_mem_alloc(sizeof(estk_geo_loc_resp_msg_v02));

  /* Send QMI_LOC_GET_ENGINE_LOCK_REQ_V02 request to QMI_LOC to know the
     status of location engine */
  if (QMI_NO_ERR !=
       qmi_client_send_msg_async(client_handle,
                                 QMI_LOC_GET_ENGINE_LOCK_REQ_V02,
                                 loc_req_msg_ptr,
                                 sizeof(qmiLocGetEngineLockReqMsgT_v02),
                                 loc_resp_msg_ptr,
                                 sizeof(estk_geo_loc_resp_msg_v02),
                                 estk_qmi_loc_request_resp,
                                 &client_cb_data,
                                 &get_sys_txn_hdl))
  {
    UIM_MSG_ERR_0("Send loc get engine lock request failed.");
    (void)estk_send_terminal_response(
            estk_curr_inst_ptr->curr_cmd.hdr_cmd.cmd_detail_reference,
            GSTK_GEO_LOC_CNF,
            GSTK_ME_CURRENTLY_UNABLE_TO_PROCESS_COMMAND,
            NULL,
            NULL);
    gstk_free(loc_resp_msg_ptr);
    gstk_free(loc_req_msg_ptr);
    loc_resp_msg_ptr = NULL;
    loc_req_msg_ptr = NULL;
    return;
  }

  gstk_free(loc_req_msg_ptr);
  loc_req_msg_ptr = NULL;
#endif
}
