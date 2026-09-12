#ifndef TM_PDAPI_IFACE_H
#define TM_PDAPI_IFACE_H

/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                  TM - PD API Interface Module

GENERAL DESCRIPTION
  This module handles IS801 specific routines

EXTERNALIZED FUNCTIONS


INITIALIZATION AND SEQUENCING REQUIREMENTS
 

  Copyright (c) 2006 - 2012 Qualcomm Technologies Incorporated.
  Qualcomm Confidential and Proprietary. All Rights Reserved.
  Copyright (c) 2013 - 2014 Qualcomm Atheros, Inc.
  Qualcomm Atheros Confidential and Proprietary. All Rights Reserved. 
  Copyright (c) 2016 - 2017 Qualcomm Technologies, Inc. 
  All Rights Reserved.
  Confidential and Proprietary - Qualcomm Technologies, Inc.

Export of this technology or software is regulated by the U.S. Government.
Diversion contrary to U.S. law prohibited.

*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/gnss8.mpss/11.1/gnss/sm/tm/inc/tm_pdapi_iface.h#2 $  
  
when       who     what, where, why
--------   ---     ----------------------------------------------------------
02/17/17   skm     BLE and Barometer support for LPPE Phase 2 
04/06/15   mj      Modify "tm_core_param_check" to make it a general PARAM check (SET and GET) function 
05/29/12   ss      Removed tm_core_handle_best_avail_pos  
04/18/12   ssu     Added tm_core_handle_best_avail_pos  
01/06/09   atien   Support XTRA-T
06/12/08   lt      Support for external coarse position injection.
01/21/08   cl      Allow overlapping dload and fix (fix and dload) sessions
11/13/07   gk      Add supl certificate injection throuh PDAPI
08/01/07   rw      Change for XTRA feature
04/3 /07   gk      Time injection API and new struct to report PD events
01/31/07   cl      Modified function for LCS handling
12/07/06   cl      Added set/get parameter handling
12/6/06    gk      modified the set param structure name
12/4/06    gk      modified the set param structure name
09/15/06   cl      Initial version

============================================================================*/
#include "gps_variation.h"
#include "pdapi.h"
#include "pdapiclient.h"
#include "sm_nv.h"
#include "tm_auxtech_iface.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum
{
  MT_NA, /* getpos not through NI client */
  MT_CP, /* getpos through NI client from a control plane protocol */
  MT_UP  /* getpos through NI client from a user plance protocol */
} pdsm_mt_initiator_e_type;  /* this is not an NV_item and not exposed to client */


/* Struct for get pos request*/ 
typedef struct
{
  pdsm_client_type_e_type       client_type;
  pdsm_client_id_type           client_id;
  pdsm_pd_option_s_type         *option_ptr;        /* PD option type */
  pdsm_pd_qos_type              *qos_ptr;           /* Ptr to user QOS for this request */
  pdsm_mt_initiator_e_type      mt_initiator;       /* 0 (not applicable), (1) CP, or (2) UP */
  boolean                       internal_session;   /*If the session is internal*/
} tm_pdapi_get_pos_param_s_type;

/* Struct for end session request*/ 
typedef struct
{
  pdsm_client_id_type           client_id;
  pdsm_pd_end_session_e_type    session_type;      /* End session type */
  boolean                       receiver_off;      /*Receiver off flag*/
  boolean                       v_reason_service_interaction; 
                                                   /* End due to service interaction */
} tm_pdapi_end_session_param_s_type;

typedef struct
{
  q_link_type                   link;	
  pdsm_client_id_type           client_id;
  pdsm_pa_e_type                e_pa_type;          /* Parameter to be set */
  pdsm_pa_info_type             *pa_info_type_ptr;
} tm_pdapi_param_s_type;

typedef struct
{
  pdsm_pd_info_s_type  pd_info_type;
  tm_pdapi_get_pos_param_s_type pd_get_pos_data;
} tm_pdapi_pd_event_payload_s_type;

typedef struct tm_pdapi_pd_event_s
{
  pdsm_pd_event_type               e_pd_event;
  tm_pdapi_pd_event_payload_s_type pd_event_data;
} tm_pdapi_pd_event_s_type;


/*===========================================================================

FUNCTION tm_core_periodic_ppm_req_check

DESCRIPTION
   Check the current TM-Core state to determine if XTRA-T request is allowed.
  
DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/
pdsm_pd_cmd_err_e_type tm_core_periodic_ppm_req_check
(
  const pdsm_periodic_ppm_cmd_s_type *periodic_ppm_clnt_req_ptr
);

/*===========================================================================

FUNCTION tm_core_handle_periodic_ppm_cmd

DESCRIPTION
  
DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/
boolean tm_core_handle_periodic_ppm_cmd
(
  const pdsm_periodic_ppm_cmd_s_type *ppm_cmd
);

/*===========================================================================

FUNCTION tm_core_get_pos_para_check

DESCRIPTION
  
DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/
extern pdsm_pd_cmd_err_e_type tm_core_get_pos_para_check
(
  const tm_pdapi_get_pos_param_s_type *get_pos_param_ptr
);

/*===========================================================================

FUNCTION tm_core_resume_get_pos

DESCRIPTION
  
DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/
extern boolean tm_core_resume_get_pos(void);

/*===========================================================================

FUNCTION tm_core_handle_get_pos

DESCRIPTION
  
DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/
extern boolean tm_core_handle_get_pos
(
  const tm_pdapi_get_pos_param_s_type *get_pos_param_ptr
);

/*===========================================================================

FUNCTION tm_core_handle_use_pos

DESCRIPTION
  
DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/
#ifdef FEATURE_POSITION_SHOPPING
#error code not present
#endif

/*===========================================================================

FUNCTION tmCore_EndSessionParaCheck

DESCRIPTION
  
DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/
extern pdsm_pd_cmd_err_e_type tm_core_end_session_para_check
(
  const tm_pdapi_end_session_param_s_type  *end_session_param_ptr                                   
);


/*===========================================================================

FUNCTION tm_core_handle_end_session

DESCRIPTION
  
DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/
extern boolean tm_core_handle_end_session
(
  const tm_pdapi_end_session_param_s_type  *end_session_param_ptr                                          
);

/*===========================================================================

FUNCTION tm_core_handle_inject_ext_time

DESCRIPTION
  
DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/
extern boolean tm_core_handle_inject_ext_time
(
  pdsm_pd_external_time_info_type *ext_time_ptr
);


/*===========================================================================

FUNCTION tm_core_handle_inject_ext_pos

DESCRIPTION
  
DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/
extern boolean tm_core_handle_inject_ext_pos
(
  pdsm_cmd_s_type *p_cmd_type
);

/*===========================================================================

FUNCTION tm_core_handle_get_gps_state

DESCRIPTION
  
DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/
extern boolean tm_core_handle_get_gps_state
(
  pdsm_cmd_s_type *p_cmd_type
);

/*===========================================================================

FUNCTION tm_core_handle_lcs_response

DESCRIPTION
  
DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/
extern boolean tm_core_handle_lcs_response
(
  pdsm_lcs_notification_resp_s_type *resp_info_ptr
);

/*===========================================================================

FUNCTION tm_core_handle_lcs_client_init

DESCRIPTION
  
DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/
extern boolean tm_core_handle_lcs_client_init
(
  pdsm_lcs_agent_client_init_s_type *clnt_init_info_ptr
);

/*===========================================================================

FUNCTION tm_core_handle_lcs_client_release

DESCRIPTION
  
DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/
extern boolean tm_core_handle_lcs_client_release
(
  pdsm_lcs_agent_client_release_s_type *clnt_release_info_ptr
);

/*===========================================================================

FUNCTION tm_core_handle_lcs_query_vx_config

DESCRIPTION
  
DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/
extern boolean tm_core_handle_lcs_query_vx_config
(
  pdsm_client_type_e_type client_type
);

/*===========================================================================

FUNCTION tm_core_handle_lcs_set_vx_config

DESCRIPTION
  
DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/
extern boolean tm_core_handle_lcs_set_vx_config
(
  pdsm_lcs_agent_set_vx_config_s_type *vx_config_info_ptr
);

/*===========================================================================

FUNCTION tm_core_param_always_allowed

DESCRIPTION: Allow commands to go through based on type of command and on-going sessio
  
DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/
boolean tm_core_param_always_allowed
(
   pdsm_pa_e_type e_pa_type, 
   pdsm_pa_cmd_e_type pa_cmd, 
   tm_pdapi_param_s_type *param_ptr
);

/*===========================================================================

FUNCTION tm_core_handle_set_param

DESCRIPTION
  This function handles pdsm_set_parameters requests from PDAPI clients in TM 
  Tasks context. Before this function is called, all error checking specific to
  a parameter is already performed and the only failure mode for the parameter
  set is for an NV update to fail. The function updates the RAM copies of the 
  NV item in TM Core and also all protocols that the parameter applies to. The
  parameter update is then queued for an asynchronous write to the NV. 

DEPENDENCIES
  All error checks for the parameter are already done.
  The GPS engine is in a state when parameter updates can be made (No E911, 
  No external session). 

RETURN VALUE
  True  : Ram copy updated, NV write queued.
  False : Something went wrong

SIDE EFFECTS
  May abort internal sessions.

===========================================================================*/
boolean tm_core_handle_set_param(tm_pdapi_param_s_type *set_param_ptr);

/*===========================================================================

FUNCTION tm_core_param_check

DESCRIPTION
  This function decides if a PA command is allowed or not based on the type of command and 
  if there is an ongoing session in progress 
DEPENDENCIES
 
PARAMETERS
 set_param_ptr: Pointer to SET/GET PARAM command structure
 pa_cmd       : PA command Type

RETURN VALUE
  TRUE: The requested PA command is allowed to execute
  FALSE: The requested PA command is denied
  
SIDE EFFECTS

===========================================================================*/
extern boolean tm_core_param_check(tm_pdapi_param_s_type *set_param_ptr, pdsm_pa_cmd_e_type pa_cmd);

/*===========================================================================

FUNCTION tm_core_handle_get_param

DESCRIPTION
  
DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/
extern boolean tm_core_handle_get_param(tm_pdapi_param_s_type *get_param_ptr);


/*===========================================================================

FUNCTION tm_core_handle_get_param

DESCRIPTION
  
DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/
extern pdsm_xtra_cmd_err_e_type tm_core_xtra_req_check
(
  const pdsm_xtra_cmd_s_type *xtra_clnt_req_ptr
);


/*===========================================================================

FUNCTION tm_core_handle_xtra_client_cmd

DESCRIPTION
  
DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/
extern boolean tm_core_handle_xtra_client_cmd
(
  pdsm_xtra_cmd_s_type *cmd_ptr
);

/*===========================================================================

FUNCTION tm_core_xtra_t_req_check

DESCRIPTION
   Check the current TM-Core state to determine if XTRA-T request is allowed.
  
DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/
extern pdsm_xtra_t_cmd_err_e_type tm_core_xtra_t_req_check
(
  const pdsm_xtra_t_cmd_s_type *xtra_t_clnt_req_ptr
);


/*===========================================================================

FUNCTION tm_core_handle_xtra_t_client_cmd

DESCRIPTION
  
DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/
extern boolean tm_core_handle_xtra_t_client_cmd
(
  pdsm_xtra_t_cmd_s_type *cmd_ptr
);

/*===========================================================================
FUNCTION tm_core_handle_reset_location_service

DESCRIPTION
  This function handles reset locaiton service request from PDAPI.
  This will bring down any ongoing sessions and will also clear XTRA state.
  This function will then pass the request to MGP.
  
DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/
extern void tm_core_handle_reset_location_service(void);
/*===========================================================================
FUNCTION tm_core_handle_inject_ni_msg

DESCRIPTION

DEPENDENCIES

RETURN VALUE
  TRUE if NI message has been injected successfully

SIDE EFFECTS

===========================================================================*/
extern boolean tm_core_handle_inject_ni_msg(
  pdsm_ni_message_s_type *ni_msg
);


/*===========================================================================
FUNCTION tm_core_handle_inject_wifi_scans

DESCRIPTION
  Handle the WiFi measurement report from APSS/LocMw. This function is called
  to report both free and on-demand wifi scan reports.

DEPENDENCIES

RETURN VALUE
  TRUE if WiFi measurement report has been injected successfully

SIDE EFFECTS

===========================================================================*/
extern boolean tm_core_handle_inject_wifi_scans(
  pdsm_wifi_on_demand_info_list_s_type *wifi_on_demand_msg
);

/*===========================================================================
FUNCTION tm_core_handle_inject_srn_scan_msg

DESCRIPTION
  Handle Srn ap scan report. This function is called to report
  Srn scans (BT,BTLE,NFC..).
 
DEPENDENCIES

RETURN VALUE
  TRUE if Srn Scan message has been injected successfully

SIDE EFFECTS

===========================================================================*/
extern boolean tm_core_handle_inject_srn_scan_msg(
  pdsm_srn_scan_info_list_s_type *srn_scan_msg
);

/*===========================================================================
FUNCTION  tm_ack_inject_wifi_scans

DESCRIPTION
  Send an Acknowledgement for wifi report messages.

DEPENDENCIES
  This function should be called before re-request PDSM_PD_EVENT_REQUEST_WIFI_INFO_ON_DEMAND_INJECTION message is send

PARAMETERS
  e_ackStatus - Ack status. 
  l_clientId - Client id. 

RETURN VALUE
  none
SIDE EFFECTS
  None
===========================================================================*/
extern void tm_ack_inject_wifi_scans(locEngStatusEnumT_v01 e_ackStatus, pdsm_client_id_type l_clientId);

/*===========================================================================

FUNCTION tm_core_queue_set_param

DESCRIPTION
  This function handles a PDAPI Command IPC in TM Tasks context by queueing
  it up in the tm_core_info.sm_set_param_q to be processed in a FIFO manner at
  a time when the engine state allows the engine configuration to be changed.
  As the PDAPI command is processed from an IPC, there is no need for a mutex.

  The function checks that the set param queue is initialized and calls the 
  PA event callback to let the calling client know that its command has been
  accepted by the engine. 

PARAMETERS
  p_set_param: pointer to the parameter to be set. Must be allocated on the heap
  using os_MemAlloc, will be freed when the parameter is set using os_MemFree.

DEPENDENCIES
  tm_core_info.sm_set_param_q must be initialized.
  All error checks for the parameter must already be complete.

RETURN VALUE
  True: Set parameter command queued for processing.
  False: Could not queue the set parameter command.

SIDE EFFECTS
  Calls the PDAPI client Event Callback.

===========================================================================*/
pdsm_pa_cmd_err_e_type tm_core_queue_set_param(tm_pdapi_param_s_type *p_set_param);

/*===========================================================================

FUNCTION tm_core_process_set_param_queue

DESCRIPTION
  This function tries to process all the set_parameter commands queued up in 
  the tm_core_info.sm_set_param_q, provided the engine state allows the engine
  configuration to be changed. This function can only be called from the TM_Tasks
  context, hence there is no need for a mutex.

  The function checks that the engine is not in E911 state and that no external 
  session is active right now before processing the entire set_parameter queue
  in a FIFO manner. 

PARAMETERS

DEPENDENCIES
  All error checks for the parameter must already be complete.

RETURN VALUE

SIDE EFFECTS
  Changes engine behavior, may cause writes to NV/EFS.

===========================================================================*/
void tm_core_process_set_param_queue(void);

#ifdef __cplusplus
}
#endif

#endif /* TM_PDAPI_IFACE_H */
