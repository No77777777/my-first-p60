#ifndef TM_LOC_PROCESSING_CLIENT_H
#define TM_LOC_PROCESSING_CLIENT_H

/*===========================================================================
  
                            TM_LOC_PROCESSING_CLIENT  h e a d e r
                   
DESCRIPTION
  This file contains implementations for TM location processing clients.
  
  Copyright (c) 2016 - 2020 Qualcomm Technologies, Inc. 
  All Rights Reserved.
  Confidential and Proprietary - Qualcomm Technologies, Inc.

===========================================================================*/

/*===========================================================================

                      EDIT HISTORY FOR FILE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/gnss8.mpss/11.1/gnss/sm/tm/src/tm_loc_processing_client.h#6 $ 
  $DateTime: 2021/05/10 21:13:37 $ 
  $Author: pwbldsvc $ 


when        who     what, where, why
--------    ---     ---------------------------------------------------------- 
06/02/20     mj      Handle E911 CPI fix qualification when E911 switches to main
10/03/2019   nath    Changes related to NV74210 when NV74191 is 1
01/14/2019   mj      SV filter support for E911 MSA main client during concurrency
04/02/2018   mj      E911 Apptracking Concurrency Support Phase 2
07/25/2017   ak      Add API to clean up LPC/LDM.  
05/24/2017   ak     Code optimization and code review comments addressed for E911 concurrency feature.
11/17/16    ak      Initial Release: Location processing client
===========================================================================*/

#include "aries_os_api.h"
#include "sys.h"
#include "gps_common.h"
#include "tm_prtl_iface.h"
#include "pdapi.h"
#include "tm_pdapi_iface.h"
#include "tm_api.h"
#include "tm_data.h"
#include "lm_api.h"
#include "gm_api.h"
#include "ldm_api.h"
#include "sm_util.h"
#include "tm_nmea.h"
#include "pdapibuf.h"
#include "tech_sel_api.h"

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/*                               DEFINES                                   */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/*  The maximum number of clients supported by LDM */
#define LPC_NUM_CLIENT_MAX LDM_CLIENT_ID_LAST+1

/* Macro to indicate MGP about start of E911 session in concurrent mode */
#define LPC_CONCURRENT_E911_SESSION_START 1

#define TM_CORE_PRTL_CB_ARR_INDEX_STANDALONE         0
#define TM_CORE_PRTL_CB_ARR_INDEX_1X_UP              1
#define TM_CORE_PRTL_CB_ARR_INDEX_1X_CP              2
#define TM_CORE_PRTL_CB_ARR_INDEX_UMTS_UP            3
#define TM_CORE_PRTL_CB_ARR_INDEX_UMTS_CP            4
#define TM_CORE_PRTL_CB_ARR_INDEX_DIAG               5
#define TM_CORE_PRTL_CB_ARR_INDEX_PPM                6
#define TM_CORE_PRTL_CB_ARR_INDEX_GFC_QMI            7
#define TM_CORE_PRTL_CB_ARR_INDEX_XTRA_T             8
#define TM_CORE_PRTL_CB_ARR_INDEX_LPP_CP             9
#define TM_CORE_PRTL_CB_ARR_INDEX_E911               10
#define TM_CORE_PRTL_CB_ARR_INDEX_CSM                11
#define TM_CORE_PRTL_CB_ARR_INDEX_MAX                11 /* Must be set to      */
/* same value as above */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/*                              DATA TYPES                                 */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

typedef enum
{
  LPC_STATE_NO_SESSION,
  LPC_STATE_IN_SESSION_REPORT_PENDING,
  LPC_STATE_IN_SESSION_NO_REPORT_PENDING,
  LPC_STATE_IN_SESSION_ERROR_REPORTED
} lpc_session_state_e_type;

typedef enum
{
  LPC_LOC_TIMER_ID
} lpc_timer_ids_e_type;

#define TM_LPC_TIMER_ID_LOC_TIMEOUT  (0x20000000)

/* LDM operation status */
typedef enum
{
  /* LDM operation status FAILURE for an unknown reason.  */
  LPC_OP_STATUS_ERROR_UNKNOWN,
  /* LDM operation completed succesfully. */
  LPC_OP_STATUS_REQUEST_SUCCESS,
  /* LDM operation resulted in a failure */
  LPC_OP_STATUS_REQUEST_FAILURE,
  LPC_OP_STATUS_ERROR_MAX = 0xFFFFFFFF
} lpc_op_status_e_type;

typedef struct
{
  boolean                  is_enabled;
  ldm_client_id_e_type     e_clientId;
  tm_sess_handle_type      sessionHandle;
  tm_prtl_type             prtl_type;
  pdsm_client_id_type      pd_clientId;
  ldm_client_e_type        e_clientId_type;
  tm_sess_req_start_param_s_type sess_params;
  boolean                  v_filterAA;
  boolean                  v_first_fix_recieved;
  /*Add aiding structure pointer also*/
  uint32                   aa_received_gps_svs;
  uint32                   aa_received_glo_svs;
  uint64                   aa_received_bds_svs;
  uint64                   aa_received_gal_svs;
  uint16                   aa_received_navic_svs;
  /*This flag is set to TRUE after first final fix arrives
  and remains TRUE until the client is deleted*/
  boolean                  v_isFinalFix;
}tm_loc_processing_client_info;

typedef struct
{
  uint8 u_partCount;
  uint8 u_partCountReject;
  uint32 q_maskMeasSrc;
  uint32 q_prevRefFCount;
}tm_loc_processing_client_meas_counts;

typedef struct
{
  uint32                  q_reportsPending;
  uint32                  q_sessHandle;

  /*Session request params*/
  uint32                  e_sessionState;

  /*Session related timers*/
  os_TimerMsgType *p_location_Timer;

  /*NMEA state*/
  uint32                 q_NmeaBitmask; /*flags defined in lm_request_nmea_flag_e_type*/

  /*Last position reported by MGP for this session*/
  boolean                       u_lastFixWasNoFixRpt;
  boolean                       u_lastFixRptValid;
  sm_FixInfoStructType          z_lastFixRpt;

  /*Best HEPE fix for the session*/
  boolean                       u_sessBestFixValid;
  sm_FixInfoStructType          z_sessBestFixRpt;
  float                         f_sessBestHepe;

  uint32           q_HepeThreshold;
  /* Tracks if a GNSS fix is received in this session compared to "best available pos" from PE */
  boolean          b_gnssFixReceivedThisSession;
  /* Tracks if RefLoc was received during E911 session for secondary client */
  boolean          v_reflocReceived;
}tm_lpc_control_data_s_type;

typedef struct
{
  ldm_client_id_e_type serving_client_id;
  ldm_client_e_type    e_client_type;
  tm_sess_handle_type serving_sess_handle;
  tm_prtl_type serving_prtl_type;
  int prtlTableIndex;
  boolean is_in_progress;
  pdsm_ext_status_info_s_type   *pz_pd_ext_status_info;
  prtl_sess_info_param_u_type   *sess_info_param_ptr;
  boolean v_filterAA;
}tm_lpc_serving_client_data_s_type;


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/*                              FUNCTION DECLARATIONS                      */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/*===========================================================================

FUNCTION tm_lpc_init

DESCRIPTION 
    This function is called by TM to initialize the LPC module.
    Called at TM task init.
    This function will inturn call init for LDM

DEPENDENCIES
NONE 
 
INPUT PARAMS
    NONE

RETURN VALUE 
   TRUE   if initialization is successful
   FALSE  if initialization fails
 
SIDE EFFECTS
    NONE
===========================================================================*/
boolean tm_lpc_init();

/*===========================================================================

FUNCTION tm_lpc_register_session_client

DESCRIPTION
   This function is called by TM to register client with LDM.

DEPENDENCIES
NONE 

INPUT PARAMS
    ldm_client_id_e_type Client ID to be registered.
 
RETURN VALUE
   TRUE  if registration is successful
   FALSE if registration fails

SIDE EFFECTS
===========================================================================*/
boolean tm_lpc_register_session_client(const ldm_client_id_e_type e_client_id);


/*===========================================================================

FUNCTION tm_lpc_deregister_session_client

DESCRIPTION
   This function is called by TM to deregister client with LDM.

DEPENDENCIES
NONE 

INPUT PARAMS
    ldm_client_id_e_type Client ID to be deregistered.
 
RETURN VALUE
   TRUE  if deregistration is successful
   FALSE if deregistration fails

SIDE EFFECTS
===========================================================================*/
boolean tm_lpc_deregister_session_client(const ldm_client_id_e_type e_client_id);


/*===========================================================================

FUNCTION tm_lpc_register_session_client_all

DESCRIPTION
   This function call tm_lpc_register_session_client for all valid LDM clients.
   Currently below three client IDs are supported.
   LDM_CLIENT_ID_E911
   LDM_CLIENT_ID_APPTRK
   LDM_CLIENT_ID_SINGLE_SHOT

DEPENDENCIES
   NONE

RETURN VALUE
   TRUE if registration of all clients is successful
   FALSE if registration for altleast one client fails.s

SIDE EFFECTS
===========================================================================*/
boolean tm_lpc_register_session_client_all(void);

/*===========================================================================
FUNCTION tm_lpc_deregister_session_client_all

DESCRIPTION
   This function call tm_lpc_deregister_session_client for all valid LDM clients.
   Currently below three client IDs are supported.
   LDM_CLIENT_ID_E911
   LDM_CLIENT_ID_APPTRK
   LDM_CLIENT_ID_SINGLE_SHOT

DEPENDENCIES
   NONE

RETURN VALUE
   TRUE if deregistration of all clients is successful
   FALSE if deregistration for altleast one client fails.s

SIDE EFFECTS
===========================================================================*/
boolean tm_lpc_deregister_session_client_all(void);

/*===========================================================================
FUNCTION tm_lpc_enable_client

DESCRIPTION
    This function is called at start of each positioning session to enable client in LDM.
    This function:
    1. Sets subscription mask for fixes or measurements or both.
    2. Adds the client info to the client database in LPC.
    3. If client enabled in E911 then notifies MGP about E911 session start.
    4. If in concurrent mode and client is E911; initializes the control data structure for E911 fix/measurement handling
       and start the timer.

DEPENDENCIES
   NONE

INPUT PARAMS
      ldm_client_id_e_type              LDM Client ID 
      tm_prtl_type                      Protocol type for this session
      tm_sess_handle_type               session handle for the session.
      tm_sess_req_start_param_s_type*   Session start parameters including timeout, qos, number of
                                        fixes etc.
      pdsm_client_id_type               PD client id.
      ldm_client_e_type                  LDM main/secondary client: Main clients starts an LM session and secondary clients
                                                  do not.

RETURN VALUE
   ONe of the following results:
    LPC_OP_STATUS_ERROR_UNKNOWN
    LPC_OP_STATUS_REQUEST_SUCCESS,
    LPC_OP_STATUS_REQUEST_FAILURE,

SIDE EFFECTS
===========================================================================*/
lpc_op_status_e_type tm_lpc_enable_client(const ldm_client_id_e_type  e_req_client_id,
                                          tm_prtl_type                    prtl_type,
                                          tm_sess_handle_type             sess_handle,
                                          tm_sess_req_start_param_s_type  *sess_start_param,
                                          pdsm_client_id_type              pd_client_id,
                                          ldm_client_e_type                e_client_type);

/*===========================================================================
FUNCTION tm_lpc_update_client

DESCRIPTION
   This function is called during session continue to update client info in LPC.
   It performs below tasks:
   1. Update client info in LPC client database.
   2. Update client subscription mask in LDM.
   3. If client is E911 and in concurrent mode the restart the timer.

DEPENDENCIES
   The LDM client should already be enabled in LDM.

INPUT PARAMS
      ldm_client_id_e_type                  LDM Client ID 
      tm_prtl_type                          Protocol type for this session
      tm_sess_handle_type                   session handle for the session.
      tm_sess_req_continue_param_s_type*    Session continue parameters including new timeout, new qos  etc.

RETURN VALUE
   ONe of the following results:
    LPC_OP_STATUS_ERROR_UNKNOWN
    LPC_OP_STATUS_REQUEST_SUCCESS,
    LPC_OP_STATUS_REQUEST_FAILURE,

SIDE EFFECTS
===========================================================================*/
lpc_op_status_e_type tm_lpc_update_client(const ldm_client_id_e_type  e_req_client_id,
                                          tm_prtl_type                    prtl_type,
                                          tm_sess_handle_type             sess_handle,
                                          tm_sess_req_continue_param_s_type  *sess_cont_param);

/*===========================================================================

FUNCTION tm_lpc_get_client_info

DESCRIPTION
    This function returns the client inforamation like protocol type, session handle etc 
    from client DB if the LDM client is enabled in LDM.

DEPENDENCIES

INPUT PARAMS
      ldm_client_id_e_type          LDM Client ID
      tm_prtl_type*             Pointer to Protocol type
      tm_sess_handle_type*              Pointer to session handle
      tm_sess_req_start_param_s_type*   Pointer to session start parameters for requested client ID.

RETURN VALUE
     TRUE   if client is enabled and found in database. All the pointers 
            passed as input params updated.
     FALSE  if client not enabled.

SIDE EFFECTS
===========================================================================*/
boolean tm_lpc_get_client_info(const ldm_client_id_e_type  e_req_client_id,
                               tm_prtl_type                    *p_prtl_type,
                               tm_sess_handle_type             *p_sess_handle,
                               tm_sess_req_start_param_s_type  *p_z_sess_start_param);



/*===========================================================================

FUNCTION tm_lpc_disable_client

DESCRIPTION
    This function is called to disable the client and performs following:
    1. Send request to LDM to remove client from distribution list.
    2. Delete the client from LPC client database.
    3. If the client to be disabled in E911, notify MGP about E911 session end
    4. If in concurrent mode and client is E911 stop the Fix/Meas timer.

DEPENDENCIES

INPUT PARAMS
      ldm_client_id_e_type          LDM Client ID

RETURN VALUE
   ONe of the following results:
    LPC_OP_STATUS_ERROR_UNKNOWN
    LPC_OP_STATUS_REQUEST_SUCCESS,
    LPC_OP_STATUS_REQUEST_FAILURE,

SIDE EFFECTS
===========================================================================*/
lpc_op_status_e_type tm_lpc_disable_client(const ldm_client_id_e_type  e_client_id);

/*===========================================================================
FUNCTION tm_lpc_cb_handler

DESCRIPTION
    Callback handler for handling the fix and measurement reports distributed by LDM.

DEPENDENCIES

INPUT PARAMS
    ldm_client_cb_data_s_type*  Pointer to Data structure containing fix report or PRM report from LM 
                                and client id.

RETURN VALUE
    NONE

SIDE EFFECTS
===========================================================================*/
void tm_lpc_cb_handler(const ldm_client_cb_data_s_type *p_client_cb_data);

/*===========================================================================

FUNCTION tm_lpc_timer_expiry_handler

DESCRIPTION
    In concurrent mode a fix/measurement is started for E911 client; this function
    handles the timeout behaviour of the timer.

DEPENDENCIES

INPUT PARAMS
    os_TimerExpiryType Timer data.

RETURN VALUE
    NONE

SIDE EFFECTS
===========================================================================*/
void tm_lpc_timer_expiry_handler(const uint32 q_TimerId);

/*===========================================================================

FUNCTION tm_lpc_update_for_aiding_data

DESCRIPTION 
    LPC needs to maintian list of SVs for which aiding was 
    provided. This function is called when aiding data is recieved from protocols
    and it updates a mask for the SVs in aiding list.
       
DEPENDENCIES

INPUT PARAMS
    ldm_client_id_e_type    LDM client ID.
    gnss_SvDirStructType    Pointer to SV direction information structure.

RETURN VALUE
    NONE.

SIDE EFFECTS
===========================================================================*/
void tm_lpc_update_for_aiding_data(ldm_client_id_e_type              e_client_id,
                                   mgp_GnssType                      q_GnssType,
                                   const gnss_SvAcqAssistCommonStructType  *pz_SvAcqAssistInfo);


/*===========================================================================
FUNCTION tm_lpc_get_pdsm_client_id_for_ldm_client

DESCRIPTION 
    Function to get pd client id for a given LDM client enabled in LPC.
       
DEPENDENCIES

INPUT PARAMS
    ldm_client_id_e_type LDM client id

RETURN VALUE
    pdsm_client_id_type pdsm client id for given LDM client.

SIDE EFFECTS
===========================================================================*/
pdsm_client_id_type tm_lpc_get_pdsm_client_id_for_ldm_client(ldm_client_id_e_type  e_client_id);


/*===========================================================================
FUNCTION tm_lpc_get_pdsm_client_id_for_ldm_client

DESCRIPTION 
    Function to get session handle for a given LDM client enabled in LPC.
       
DEPENDENCIES

INPUT PARAMS
    ldm_client_id_e_type LDM client id

RETURN VALUE
    pdsm_client_id_type pdsm client id for given LDM client.

SIDE EFFECTS
===========================================================================*/
tm_sess_handle_type tm_lpc_get_sess_handle_for_ldm_client(ldm_client_id_e_type  e_client_id);

/*===========================================================================

FUNCTION tm_core_trigger_fix_related_NMEA

DESCRIPTION
    Function to generate NMEA for fix reports.

DEPENDENCIES

INPUT PARAMS
    sm_FixInfoStructType*       Pointer to fix report
    boolean   Indicates is it's a no fix

RETURN VALUE

SIDE EFFECTS
===========================================================================*/
void tm_core_trigger_fix_related_NMEA(sm_FixInfoStructType      *z_fix,
                                      boolean is_no_fix);

/*===========================================================================

FUNCTION tm_core_trigger_meas_related_NMEA

DESCRIPTION
    Function to generate NMEA for PRM reports.

DEPENDENCIES

INPUT PARAMS
    sm_GpsMeasRptStructType       Pointer to PRM report

RETURN VALUE
    NONE

SIDE EFFECTS

===========================================================================*/
void tm_core_trigger_meas_related_NMEA(sm_GpsMeasRptStructType  *z_meas);

/*===========================================================================

FUNCTION tm_lpc_generate_pd_event

DESCRIPTION
    In concurrent mode PD event needs to be generated when E911 session ends.
    This function generated the PD done event.

DEPENDENCIES
    NONE
INPUT PARAMS
    ldm_client_id_e_type        LDM client ID
    tm_sess_stop_reason_e_type  Abort reason

RETURN VALUE
    NONE

SIDE EFFECTS

===========================================================================*/
void tm_lpc_generate_pd_event(ldm_client_id_e_type e_ldm_cli_id,
                              tm_sess_stop_reason_e_type abort_reason);

/*===========================================================================
FUNCTION tm_lpc_report_error_secondary

DESCRIPTION
    This function reports error to protocols in case of any failure for secondary client during a 
    concurrent session.

DEPENDENCIES
    NONE
INPUT PARAMS
    tm_sess_stop_reason_e_type   Session stop reason.
    ldm_client_id_e_type               Secondary client id

RETURN VALUE
    NONE
===========================================================================*/
void tm_lpc_report_error_secondary(tm_sess_stop_reason_e_type e_reason, ldm_client_id_e_type e_secondary_client_id);

/*===========================================================================

FUNCTION tm_lpc_stop_timer_request

DESCRIPTION
    Request to stop the timer started in concurrent mode for E911 session.
    This function might be called when the main client/Apptrack client session 
    ends during concurrency and we need to start a new TM-LM session with 
    E911 params.

DEPENDENCIES
    NONE
INPUT PARAMS
    NONE
RETURN VALUE
    NONE

SIDE EFFECTS

===========================================================================*/
void tm_lpc_stop_timer_request();

/*===========================================================================

FUNCTION tm_lpc_disable_client_no_active_protocol

DESCRIPTION 
    This function is called to disable clients in LDM in scenarios like
    no active protocol session or RAT change during early reciever on.

DEPENDENCIES
    NONE
INPUT PARAMS
    NONE
RETURN VALUE
    NONE

SIDE EFFECTS

===========================================================================*/
void tm_lpc_disable_client_no_active_protocol();

/*===========================================================================
FUNCTION lpc_get_loc_processing_client

DESCRIPTION 
      This function is used to get the details of particular client from client
      database. 

DEPENDENCIES 
      NONE
 
INPUT PARAMS
      e_client_id       Client ID 

RETURN VALUE
      boolean

SIDE EFFECTS
===========================================================================*/
tm_loc_processing_client_info* lpc_get_loc_processing_client(const ldm_client_id_e_type  e_client_id);

/*===========================================================================

FUNCTION tm_lpc_get_main_secondary_clients

DESCRIPTION
    This function is called to get the main and secondary clients in concurrency mode

DEPENDENCIES
    NONE
INPUT PARAMS
    ldm_client_id_e_type* main_client, ldm_client_id_e_type* secondary_client
RETURN VALUE
    VOID 

SIDE EFFECTS

===========================================================================*/
void tm_lpc_get_main_secondary_clients(ldm_client_id_e_type *main_client, ldm_client_id_e_type *secondary_client);

/*===========================================================================
FUNCTION lpc_map_client_id_to_status_mask

DESCRIPTION 
     Map the LDM client ID to different client masks supported by LDM
      
DEPENDENCIES 
      NONE
 
INPUT PARAMS 
    e_req_client_id LDM client ID

RETURN VALUE 
    uint32  client Mask

SIDE EFFECTS
===========================================================================*/
uint32 lpc_map_client_id_to_status_mask(ldm_client_id_e_type  e_req_client_id);

/*===========================================================================

FUNCTION tm_lpc_clear_control_data

DESCRIPTION
    This function is called to clear the control data when a secondary client is switched or disabled

DEPENDENCIES
    NONE
INPUT PARAMS
   NONE
RETURN VALUE
    VOID 

SIDE EFFECTS

===========================================================================*/
void tm_lpc_clear_control_data();

/*===========================================================================

FUNCTION tm_lpc_client_update_client_type

DESCRIPTION
    Sets the client type in LPC and LDM for a given client

DEPENDENCIES
    NONE
INPUT PARAMS
   ldm_client_id_e_type e_client_id
RETURN VALUE
    boolean: TRUE == Success, FALSE == Failed 

SIDE EFFECTS

===========================================================================*/
boolean tm_lpc_client_update_client_type(ldm_client_id_e_type e_client_id, ldm_client_e_type e_client_type);

/*===========================================================================

FUNCTION tm_lpc_client_update_aa_filter_pref

DESCRIPTION
    Sets the AA filter preference in LPC 

DEPENDENCIES
    NONE
INPUT PARAMS
   ldm_client_id_e_type e_client_id, boolean v_aa_pref
   v_aa_filter_pref == TRUE (SVs need to be filtered as per AA)
   v_aa_filter_pref == FALSE (SVs need not be filtered as per AA)
RETURN VALUE
    boolean: TRUE == Success, FALSE == Failed 

SIDE EFFECTS

===========================================================================*/
boolean tm_lpc_client_update_aa_filter_pref(ldm_client_id_e_type e_client_id, boolean v_aa_filter_pref);

/*===========================================================================

FUNCTION tm_lpc_refloc_received

DESCRIPTION
    Updates LPC if RefLoc is received during E911. This is used to qualify
    E911 fixes when only CPI fixes are received

DEPENDENCIES
    NONE
INPUT PARAMS
    tm_cm_e911_state_e_type e911_state
RETURN VALUE
    NONE 

SIDE EFFECTS

===========================================================================*/
void tm_lpc_refloc_received(const tm_cm_e911_state_e_type e911_state);

/*===========================================================================

FUNCTION tm_lpc_update_lm_with_E911_fix_qualification

DESCRIPTION
    When E911 switches from secondary to main, the fix qualificaiton is handled
    in LM. This function updates LM with best fix cached, best fix hepe,
    is refloc received and has gnss fix received during E911 session.

DEPENDENCIES
    NONE
INPUT PARAMS
    ldm_client_id_e_type e_client_id
RETURN VALUE
    NONE 

SIDE EFFECTS

===========================================================================*/
void tm_lpc_update_lm_with_E911_fix_qualification(const ldm_client_id_e_type e_client_id);

/*===========================================================================

FUNCTION tm_lpc_client_special_req_check

DESCRIPTION
    Checks if any client Main or Secondary has a SpecialRequest type session
    running

DEPENDENCIES
    NONE
INPUT PARAMS
    NONE
RETURN VALUE
    boolean: TRUE  == A Special Req client is active.
             FALSE == No Special Req Client is active. 

SIDE EFFECTS

===========================================================================*/
boolean tm_lpc_client_special_req_check(void);


#endif /* TM_LOC_PROCESSING_CLIENT_H */

