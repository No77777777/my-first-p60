/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

            TM Location Processing Client

General Description
  This file contains implementations for TM location processing clients.
  
  Copyright (c) 2016 - 2020 Qualcomm Technologies, Inc. 
  All Rights Reserved.
  Confidential and Proprietary - Qualcomm Technologies, Inc.

Export of this technology or software is regulated by the U.S. Government.
Diversion contrary to U.S. law prohibited.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/gnss8.mpss/11.1/gnss/sm/tm/src/tm_loc_processing_client.c#14 $ 
  $DateTime: 2024/06/17 23:30:31 $ 
  $Author: pwbldsvc $ 


when         who     what, where, why
--------     ---     ----------------------------------------------------------
06/05/24     skc     Added changes to report TDM measurement.
07/08/20   skm     ShortCodes SpecialRequest support for START_REQ and GET_BEST_POS 
08/26/20    jeev     Fix stack overflow while inject aiding data
06/02/20     mj      Handle E911 CPI fix qualification when E911 switches to main
01/22/20     mj      Support to request CPI during simulation E911 through NV
10/16/19     mj      Start CPI request when E911 client is enabled
10/03/19     nath    Changes related to NV74210 when NV74191 is 1
08/12/19     sk      Added changes for second boundary aligned NMEA fix report when NV is set 
06/26/19     mj      Changes to check if NV enabled before shipping out CPI for E911 secondary
05/31/19     nath    Destroy LPC timer where control data is cleared
01/14/19     mj      SV filter support for E911 MSA main client during concurrency
01/10/19     skm     PRM QOS should be saved in the client info
12/17/2018   nath    Avoid starting LPC timer with 0 value.
11/06/18     py      In QMI Measurement report, Cno reported will be at antenna(addRFloss)
08/28/18     py      QMI SV info SNR will consider RF loss when cno>0
08/10/18     sum     Added DTF implementation for Meas report issue
06/19/2018   sj      Fix timer resource leakage
04/02/2018   mj      E911 Apptracking Concurrency Support Phase 2
09/15/17     sum     Added Call to GPDTM Nmea.
08/05/2017   ak      Add changes to reset PRM sequence number.
07/25/2017   ak      Add API to clean up LPC/LDM. 
07/06/2017   ak      Call LDM API to update client subscription for measurement/fix reports on session continue
05/24/2017   ak      Code optimization and code review comments addressed for E911 concurrency feature.
11/17/2016   ak      Intial release: Location processsing client
====================================================================================================================*/
#include "tm_loc_processing_client.h"
#include "tm_pdapi_client.h"
#include "mgp_api.h"
#include <math.h>
#ifdef FEATURE_GNSS_DTF
#include "tm_dtf.h"
#endif


/*List for clients registered*/

/*Static functions*/
static void lpc_generate_log_packet(const sm_GpsFixRptStructType *p_fixReport);

/*Static variable*/

#ifdef FEATURE_GNSS_DTF
boolean b_dropMeasBlock = FALSE;
uint16 u_prevMaxPartCount = 0;
#endif

static tm_loc_processing_client_info lp_clients[LPC_NUM_CLIENT_MAX] = { 0 };
static uint32 q_prevRefFCount = 0;
tm_lpc_control_data_s_type *z_lpc_control_data = NULL;

ldm_client_cb_s_type z_cb_functions = { 0 };
tm_lpc_serving_client_data_s_type z_serving_client_data = { 0 };

/*
Defines
*/
/*Macro to verify if LDM client id is valid*/
#define LPC_VERIFY_CLIENT_ID(e_client_id) ((e_client_id <= LDM_CLIENT_ID_LAST) && (e_client_id >= LDM_CLIENT_ID_E911))

/*Time to wait before resetting the sequence number in PRM reports to QMI, in msec*/
#define TM_CORE_WAITIME_FOR_SEQUENCE_NUM_RESET 2000

/* Maximum allowable time in milliseconds to consider second boundary aligned */
#define TM_CORE_MAX_ALLOWABLE_MS_FOR_SECOND_BOUNDARY 25

/* MACROS for NMEA output */
#define TM_CORE_OUTPUT_NMEA_1(bit, str, meas_rpt) \
          if(tm_core_info.config_info.subs_cfgs[tm_core_info.config_info.active_subs_cfg].nmea_output_format & (bit)) \
          { \
            tm_nmea_##str##_put(meas_rpt); \
          }
#define TM_CORE_OUTPUT_NMEA_2(bit, str, fix_rpt, is_no_fix) \
          if(tm_core_info.config_info.subs_cfgs[tm_core_info.config_info.active_subs_cfg].nmea_output_format & (bit)) \
          { \
            tm_nmea_##str##_put(fix_rpt, is_no_fix); \
          }

#define TM_CORE_OUTPUT_NMEA_GPGSV(meas_rpt)  TM_CORE_OUTPUT_NMEA_1(PDAPI_NMEA_GPGSV, gpgsv, meas_rpt)
#define TM_CORE_OUTPUT_NMEA_GLGSV(meas_rpt)  TM_CORE_OUTPUT_NMEA_1(PDAPI_NMEA_GLGSV, glgsv, meas_rpt)
#define TM_CORE_OUTPUT_NMEA_GAGSV(meas_rpt)  TM_CORE_OUTPUT_NMEA_1(PDAPI_NMEA_GAGSV, gagsv, meas_rpt)
#define TM_CORE_OUTPUT_NMEA_PQGSV(meas_rpt)  TM_CORE_OUTPUT_NMEA_1(PDAPI_NMEA_PQGSV, pqgsv, meas_rpt)
#define TM_CORE_OUTPUT_NMEA_GBGSV(meas_rpt)  TM_CORE_OUTPUT_NMEA_1(PDAPI_NMEA_GBGSV, gbgsv, meas_rpt)
#define TM_CORE_OUTPUT_NMEA_GIGSV(meas_rpt)  TM_CORE_OUTPUT_NMEA_1(PDAPI_NMEA_GIGSV, gigsv, meas_rpt)
#define TM_CORE_OUTPUT_NMEA_GQGSV(meas_rpt)  TM_CORE_OUTPUT_NMEA_1(PDAPI_NMEA_GQGSV, gqgsv, meas_rpt)

#define TM_CORE_OUTPUT_NMEA_GPGGA(fix_rpt, is_no_fix) TM_CORE_OUTPUT_NMEA_2(PDAPI_NMEA_GPGGA, gpgga, fix_rpt, is_no_fix)
#define TM_CORE_OUTPUT_NMEA_GAGGA(fix_rpt, is_no_fix) TM_CORE_OUTPUT_NMEA_2(PDAPI_NMEA_GAGGA, gagga, fix_rpt, is_no_fix)
#define TM_CORE_OUTPUT_NMEA_GBGGA(fix_rpt, is_no_fix) TM_CORE_OUTPUT_NMEA_2(PDAPI_NMEA_GBGGA, gbgga, fix_rpt, is_no_fix)
#define TM_CORE_OUTPUT_NMEA_GNGGA(fix_rpt, is_no_fix) TM_CORE_OUTPUT_NMEA_2(PDAPI_NMEA_GNGGA, gngga, fix_rpt, is_no_fix)

#define TM_CORE_OUTPUT_NMEA_GPDTM(fix_rpt, is_no_fix) TM_CORE_OUTPUT_NMEA_2(PDAPI_NMEA_GPDTM, gpdtm, fix_rpt, is_no_fix)
#define TM_CORE_OUTPUT_NMEA_GNDTM(fix_rpt, is_no_fix) TM_CORE_OUTPUT_NMEA_2(PDAPI_NMEA_GNDTM, gndtm, fix_rpt, is_no_fix)


#ifdef FEATURE_GNSS_NON_STANDARD_SUPPORT
#define TM_CORE_OUTPUT_NMEA_PQXFI(fix_rpt, is_no_fix) TM_CORE_OUTPUT_NMEA_2(PDAPI_NMEA_PQXFI, pqxfi, fix_rpt, is_no_fix)
#else
#define TM_CORE_OUTPUT_NMEA_PQXFI(fix_rpt, is_no_fix)
#endif /* FEATURE_GNSS_NON_STANDARD_SUPPORT */

#define TM_CORE_OUTPUT_NMEA_GNGNS(fix_rpt, is_no_fix) TM_CORE_OUTPUT_NMEA_2(PDAPI_NMEA_GNGNS, gngns, fix_rpt, is_no_fix)
#define TM_CORE_OUTPUT_NMEA_GPVTG(fix_rpt) TM_CORE_OUTPUT_NMEA_1(PDAPI_NMEA_GPVTG, gpvtg, fix_rpt)
#define TM_CORE_OUTPUT_NMEA_GAVTG(fix_rpt) TM_CORE_OUTPUT_NMEA_1(PDAPI_NMEA_GAVTG, gavtg, fix_rpt)
#define TM_CORE_OUTPUT_NMEA_GBVTG(fix_rpt) TM_CORE_OUTPUT_NMEA_1(PDAPI_NMEA_GBVTG, gbvtg, fix_rpt)
#define TM_CORE_OUTPUT_NMEA_GNVTG(fix_rpt) TM_CORE_OUTPUT_NMEA_1(PDAPI_NMEA_GNVTG, gnvtg, fix_rpt)

#define TM_CORE_OUTPUT_NMEA_GPRMC(fix_rpt) TM_CORE_OUTPUT_NMEA_1(PDAPI_NMEA_GPRMC, gprmc, fix_rpt)
#define TM_CORE_OUTPUT_NMEA_GARMC(fix_rpt) TM_CORE_OUTPUT_NMEA_1(PDAPI_NMEA_GARMC, garmc, fix_rpt)
#define TM_CORE_OUTPUT_NMEA_GBRMC(fix_rpt) TM_CORE_OUTPUT_NMEA_1(PDAPI_NMEA_GBRMC, gbrmc, fix_rpt)
#define TM_CORE_OUTPUT_NMEA_GNRMC(fix_rpt) TM_CORE_OUTPUT_NMEA_1(PDAPI_NMEA_GNRMC, gnrmc, fix_rpt)

#define TM_CORE_OUTPUT_NMEA_GPGSA(fix_rpt) TM_CORE_OUTPUT_NMEA_1(PDAPI_NMEA_GPGSA, gpgsa, fix_rpt)
#define TM_CORE_OUTPUT_NMEA_GAGSA(fix_rpt) TM_CORE_OUTPUT_NMEA_1(PDAPI_NMEA_GAGSA, gagsa, fix_rpt)
#define TM_CORE_OUTPUT_NMEA_GBGSA(fix_rpt) TM_CORE_OUTPUT_NMEA_1(PDAPI_NMEA_GBGSA, gbgsa, fix_rpt)
#define TM_CORE_OUTPUT_NMEA_GNGSA(fix_rpt) TM_CORE_OUTPUT_NMEA_1(PDAPI_NMEA_GNGSA, gngsa, fix_rpt)
#define TM_CORE_OUTPUT_NMEA_PQGSA(fix_rpt) TM_CORE_OUTPUT_NMEA_1(PDAPI_NMEA_PQGSA, pqgsa, fix_rpt)

#define TM_LOC_INS_HEPE_THRESHOLD 250
#define TM_LOC_FIX_RESPONSE_TIME_MS_DEFAULT      (255*1000)   /* 255 seconds */

/*===========================================================================

FUNCTION lpc_generate_log_packet

DESCRIPTION
      This function generates 1370 and 1476 log packets

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/
static void lpc_generate_log_packet(const sm_GpsFixRptStructType *p_fixReport)
{
#ifdef FEATURE_GEN7_SM_PE_API
  #error code not present
#else
  gnss_NavSolutionStructType z_navSolution = { 0 };
#endif

  if (p_fixReport == NULL)
  {
    MSG(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "LPC:NULL p_fixReport ");
    return;
  }

#ifdef FEATURE_GEN7_SM_PE_API
  #error code not present
#else /* FEATURE_GEN7_SM_PE_API */
  /* log the 0x1476 log packet from LM. */
  memset((void *)&z_navSolution, 0, sizeof(z_navSolution));
  if (sm_translate_sm_fix_report_to_gnss_nav_solution(&z_navSolution, p_fixReport, FALSE,
                                                      FALSE))
  {
    gnss_LogPositionReport(&z_navSolution, NULL);
    /* count = NULL because we're not keeping track of how many messages we've sent out */
  }
#endif /* FEATURE_GEN7_SM_PE_API */
}

/*===========================================================================

FUNCTION lpc_add_loc_processing_client

DESCRIPTION 
      This function is used to add a new client information to the database.
      The information like session_handle, aiding data, qos etc are stored
      for each clients.

DEPENDENCIES 
      NONE 
 
INPUT PARAMS
      e_client_id       Client ID 
      sess_handle       session handle for the session.
      sess_start_param  Session parameters including timeout, qos, number of
                        fixes etc.
 
RETURN VALUE 
      TRUE   When info successfully added
      FALSE  on failure.

SIDE EFFECTS


===========================================================================*/
static boolean lpc_add_loc_processing_client(const ldm_client_id_e_type  e_client_id,
                                             tm_prtl_type                    client_prtl_type,
                                             tm_sess_handle_type             sess_handle,
                                             tm_sess_req_start_param_s_type  *sess_start_param,
                                             pdsm_client_id_type             pd_client_id,
                                             ldm_client_e_type               e_client_type)
{
  MSG_2(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LPC:Adding client %d to the list for location processing as client type %d", e_client_id, e_client_type);

  if (NULL == sess_start_param)
  {
    MSG(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "LPC:Client ADD failed, NULL start params.");
    return FALSE;
  }

  MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LPC:Adding client %d to the list for location processing as client type %d, specialReq:%d", 
        e_client_id, e_client_type,sess_start_param->specialReqType.e_SpecialReq);

  /*Check if client is already present in database 
    else allocate memory and copy the session parameters*/
  if (FALSE == lp_clients[e_client_id].is_enabled)
  {
    lp_clients[e_client_id].is_enabled = TRUE;
    lp_clients[e_client_id].e_clientId = e_client_id;
    lp_clients[e_client_id].prtl_type = client_prtl_type;
    lp_clients[e_client_id].sessionHandle = sess_handle;
    lp_clients[e_client_id].pd_clientId   = pd_client_id;
    lp_clients[e_client_id].aa_received_glo_svs = 0;
    lp_clients[e_client_id].aa_received_gps_svs = 0;
    lp_clients[e_client_id].e_clientId_type = e_client_type;

    /*Copy session start parameters*/
    memscpy((void *)&(lp_clients[e_client_id].sess_params), sizeof(tm_sess_req_start_param_s_type),
            (void *)sess_start_param, sizeof(tm_sess_req_start_param_s_type));

    return TRUE;
  }
  else
  {
    MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "LPC:Client %d already present in the list, cannot add another one.", e_client_id);
    return FALSE;
  }
}


/*===========================================================================

FUNCTION lpc_del_loc_processing_client

DESCRIPTION 
      This function deletes the client from the client database.

DEPENDENCIES 
      NONE
 
INPUT PARAMS
      e_client_id       Client ID 

RETURN VALUE
      boolean

SIDE EFFECTS


===========================================================================*/
static boolean lpc_del_loc_processing_client(const ldm_client_id_e_type  e_client_id)
{

  MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LPC:Deleting %d client from the list for location processing ", e_client_id);

  if (TRUE == lp_clients[e_client_id].is_enabled)
  {
    MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LPC:Loc Processing Client %d present, going to delete", e_client_id);
    memset(&(lp_clients[e_client_id]), 0, sizeof(lp_clients[e_client_id]));
    lp_clients[e_client_id].is_enabled = FALSE;
    return TRUE;
  }
  else
  {
    MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "LPC:Loc Processing Client %d not found", e_client_id);
    return FALSE;
  }
}

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
tm_loc_processing_client_info* lpc_get_loc_processing_client(const ldm_client_id_e_type  e_client_id)
{
  if (TRUE == lp_clients[e_client_id].is_enabled)
  {
    MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LPC:Fetch %d location processing client from the list.", e_client_id);
    return &(lp_clients[e_client_id]);
  }
  else
  {
    MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LPC:%d location processing client not enabled.", e_client_id);
    return NULL;
  }
}


/*===========================================================================
FUNCTION lpc_update_loc_processing_client

DESCRIPTION 
    Function to update the client info in database.


DEPENDENCIES 
      NONE 
 
INPUT PARAMS
      ldm_client_id_e_type       Client ID 
      tm_prtl_type      Protocol type.
      tm_sess_handle_type       session handle for the session.
      tm_sess_req_continue_param_s_type  Session parameters including timeout, qos, number of
                        fixes etc.
 
RETURN VALUE 
      TRUE   When info successfully added
      FALSE  on failure.

SIDE EFFECTS
===========================================================================*/
static boolean lpc_update_loc_processing_client(const ldm_client_id_e_type  e_client_id,
                                                tm_prtl_type                    client_prtl_type,
                                                tm_sess_handle_type             sess_handle,
                                                tm_sess_req_continue_param_s_type  *sess_cont_param)
{
  tm_loc_processing_client_info *pz_client_info;
  boolean                       v_specialReqValid = FALSE;

  MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LPC:Updating %d clients information ", e_client_id);

  pz_client_info = lpc_get_loc_processing_client(e_client_id);

  if ((NULL == pz_client_info) || (NULL == sess_cont_param))
  {
    MSG(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "LPC:Client not found to update OR NULL ptr sent for sess continue");
    return FALSE;
  }

  pz_client_info->prtl_type = client_prtl_type;
  pz_client_info->sessionHandle = sess_handle;
  pz_client_info->sess_params.accuracy_threshold = sess_cont_param->accuracy_threshold;
  pz_client_info->sess_params.dynamic_qos_enabled = sess_cont_param->dynamic_qos_enabled;
  pz_client_info->sess_params.lr_qos = sess_cont_param->lr_qos;
  pz_client_info->sess_params.multi_report_msa = sess_cont_param->multi_report_msa;
  pz_client_info->sess_params.num_fixes = sess_cont_param->num_fixes;
  pz_client_info->sess_params.op_mode = sess_cont_param->op_mode;
  pz_client_info->sess_params.op_req = sess_cont_param->op_req;
  pz_client_info->sess_params.ppm_qos = sess_cont_param->ppm_qos;
  pz_client_info->sess_params.prm_qos = sess_cont_param->prm_qos;
  pz_client_info->sess_params.prm_prq = sess_cont_param->prm_prq;
  pz_client_info->sess_params.tbf_ms = sess_cont_param->tbf_ms;
  pz_client_info->sess_params.lppm_mode = sess_cont_param->lppm_mode;

  pz_client_info->sess_params.specialReqType.e_SpecialReq = PDSM_SESSION_DONOTUSE_SPECIAL_REQ_MAX;
  if (PDSM_SESSION_SPECIAL_REQ_SHORT_CODES == sess_cont_param->specialReqType.e_SpecialReq) 
  {
     pz_client_info->sess_params.specialReqType.e_SpecialReq = sess_cont_param->specialReqType.e_SpecialReq;
     v_specialReqValid = TRUE;
  }

  MSG_2(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LPC: Updated %d Clients information, specialReq Valid: %d", 
           e_client_id, v_specialReqValid);

  return TRUE;
}


/*===========================================================================
FUNCTION lpc_init_control_data

DESCRIPTION 
      When in concurrent mode the fix qualification and measurement qualification
      for secondary client sessions are performed in LPC. This function initializes the control 
      data like opmode, best fix for session, timer etc.

DEPENDENCIES 
      NONE
 
INPUT PARAMS
      e_client_id       Client ID 

RETURN VALUE
      TRUE if intialization was successful
      FALSE if initilaization fails

SIDE EFFECTS
===========================================================================*/
static boolean lpc_init_control_data(ldm_client_id_e_type  e_client_id)
{
  MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LPC:Initializing control data for client %d in concurrent mode", e_client_id);

  if (NULL != z_lpc_control_data)
  {
    MSG(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "LPC:Control data pointer not NULL. Currently being used");
    return FALSE;
  }
  else
  {
    MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LPC:Allocate memory for LPC control data");

    z_lpc_control_data = (tm_lpc_control_data_s_type *)os_MemAlloc(sizeof(tm_lpc_control_data_s_type), OS_MEM_SCOPE_TASK);

    if (NULL == z_lpc_control_data)
    {
      MSG(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "LPC:Failed to allocate memory for LPC control data");
      return FALSE;
    }

    /*Intialize the parameters*/
    z_lpc_control_data->q_reportsPending = 0;
    z_lpc_control_data->q_sessHandle = 0;
    z_lpc_control_data->u_sessBestFixValid = FALSE;
    z_lpc_control_data->f_sessBestHepe = 0;
    memset((void *)&z_lpc_control_data->z_sessBestFixRpt, 0, sizeof(sm_FixInfoStructType));
    z_lpc_control_data->u_lastFixRptValid = FALSE;
    z_lpc_control_data->u_lastFixWasNoFixRpt = FALSE;
    memset((void *)&z_lpc_control_data->z_lastFixRpt, 0, sizeof(sm_FixInfoStructType));
    z_lpc_control_data->v_reflocReceived = FALSE;

    /*Create session timers*/
    z_lpc_control_data->p_location_Timer = os_TimerCreateInCgpsTimerGroup((uint32)TM_LPC_TIMER_ID_LOC_TIMEOUT, (uint32)THREAD_ID_SM_TM);
    return TRUE;
  }
}

/*===========================================================================
FUNCTION lpc_mo_mt_mode

DESCRIPTION
       This function decides whether the session is for Mobile Originated
       or Mobile Terminated Request

DEPENDENCIES 
      NONE
 
INPUT PARAMS 
    q_sessHandle TM session handle

RETURN VALUE 
    lm_mo_mt_mode_e_type 

SIDE EFFECTS
===========================================================================*/
static lm_mo_mt_mode_e_type lpc_mo_mt_mode(uint32 q_sessHandle)
{

  /* LPP CP Session handle may conflict with the MO session handle if included in the next if condition, hence check seperately */
  if (q_sessHandle == TM_SESS_HANDLE_LPP_CP_NI)
  {
    return LM_MO_MT_MODE_MT;
  }
  /* Set the call type to MO/MT based on the session handle. E911 session type should be considered as MT so the HEPE checks at
   timeout are ignored */
  if (0 != (q_sessHandle &
               (TM_SESS_HANDLE_1X_CP_MT | TM_SESS_HANDLE_1X_UP_NI |
                   TM_SESS_HANDLE_UMTS_CP_NI | TM_SESS_HANDLE_UMTS_UP_NI |
                   TM_SESS_HANDLE_E911)))
  {
    return LM_MO_MT_MODE_MT;
  }

  return LM_MO_MT_MODE_MO;
}


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
uint32 lpc_map_client_id_to_status_mask(ldm_client_id_e_type  e_req_client_id)
{
  uint32 client_mask = 0xFF;

  switch (e_req_client_id)
  {
    case LDM_CLIENT_ID_E911:
      client_mask = LDM_CLIENT_E911_MASK;
      break;
    case LDM_CLIENT_ID_APPTRK:
      client_mask = LDM_CLIENT_APPTRK_MASK;
      break;
    case LDM_CLIENT_ID_SINGLE_SHOT:
      client_mask = LDM_CLIENT_SINGLE_SHOT_MASK;
      break;
    default:
      break;
  }

  MSG_2(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LPC: Mask for LDM client %d is %d", e_req_client_id, client_mask);
  return client_mask;
}

/*===========================================================================

FUNCTION lpc_dispatch_ppm_rpt_to_prtl

DESCRIPTION
  This function sends a PPM report to a protocol, using the provided
  sess_info function pointer.

DEPENDENCIES

RETURN VALUE
  FALSE: Protocol callback does not accept the ppm
  TRUE: all other cases

SIDE EFFECTS

===========================================================================*/
static boolean lpc_dispatch_ppm_rpt_to_prtl
(
   prtl_session_info_f_type          *sess_info_fp,
   prtl_sess_info_param_u_type       *sess_info_param_ptr,
   srch_ppm_rpt_type                 *ppm_rpt,
   tm_sess_handle_type                sess_handle
   )
{
  boolean retVal = FALSE;

  if (sess_info_fp == NULL)
  {
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "LPC:No protocol callback installed 6", 0, 0, 0);
    return TRUE;
  }

  if (sess_info_param_ptr == NULL || ppm_rpt == NULL)
  {
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "LPC:Null ptrs passed", 0, 0, 0);
    return TRUE;
  }

  memscpy((void *)&(sess_info_param_ptr->ppm_data),
          sizeof(sess_info_param_ptr->ppm_data),
          (void *)ppm_rpt,
          sizeof(*ppm_rpt));

  retVal = sess_info_fp(
     sess_handle,
     PRTL_SESS_INFO_PPM,
     sess_info_param_ptr);

  if (retVal == FALSE)
  {
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "LPC:Protocol rejects PPM", 0, 0, 0);
    return FALSE;
  }

  return TRUE;
}


/*===========================================================================

FUNCTION tm_core_xlate_svpoly_to_ext_svpoly_rpt

DESCRIPTION: Translates internal GNSS SV polynomial structure in a form suitable for transmission over QMI

DEPENDENCIES

RETURN VALUE
    TRUE is translation is successful
    FALSE otherwise
SIDE EFFECTS

===========================================================================*/
#define     FEATURE_GNSS_RAW_SVPOLY_OUTPUT_DEBUG

static boolean tm_core_xlate_svpoly_to_ext_svpoly_rpt
(
   pdsm_pd_ext_SvPoly_s_type       *dest_buf_ptr,
   const sm_SvPolyReportType  *source_buf_ptr
   )
{
  int8 i;
  uint16 w_EnabledConstell = tm_core_info.config_info.sv_poly_rep_constell_config;

#ifdef FEATURE_GNSS_RAW_SVPOLY_OUTPUT_DEBUG
  MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LPC:Entered tm_core_xlate_svpoly_to_ext_svpoly_rpt", 0, 0, 0);
#endif

  if ((dest_buf_ptr == NULL) ||
      (source_buf_ptr == NULL))
  {
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "LPC:tm_core_xlate_svpoly_to_ext_svpoly_rpt received NULL pointer", 0, 0, 0);
    return (FALSE);
  }

  dest_buf_ptr->b_FreqNum = 0;

  /* Report polynomials for only the SVs for which reporting is configured */
  if (PRN_OK(source_buf_ptr->z_SvPolyReport.w_Sv))
  {
    if (!(w_EnabledConstell & PDSM_GPS_MEAS_REPORT_ENABLED))
      return (FALSE);
  }
  else if (BDS_ID_OK(source_buf_ptr->z_SvPolyReport.w_Sv))
  {
    if (!(w_EnabledConstell & PDSM_BDS_MEAS_REPORT_ENABLED))
      return (FALSE);
  }
  else if (GAL_ID_OK(source_buf_ptr->z_SvPolyReport.w_Sv))
  {
    if (!(w_EnabledConstell & PDSM_GAL_MEAS_REPORT_ENABLED))
      return (FALSE);
  }
  else if (QZSS_PRN_OK(source_buf_ptr->z_SvPolyReport.w_Sv))
  {
    if (!(w_EnabledConstell & PDSM_QZSS_MEAS_REPORT_ENABLED))
      return (FALSE);
  }
  else if (SLOTID_OK(source_buf_ptr->z_SvPolyReport.w_Sv) ||
           ((GLO_SLOTID_UNKNOWN == source_buf_ptr->z_SvPolyReport.w_Sv) &&
            !GLO_SIGNED_FREQ_INVALID(source_buf_ptr->z_SvPolyReport.b_FreqNum)))
  {
    if (!(w_EnabledConstell & PDSM_GLO_MEAS_REPORT_ENABLED))
      return (FALSE);
  }
#ifdef FEATURE_GNSS_RAW_SVPOLY_OUTPUT_DEBUG
  MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LPC:Qualified SV in tm_core_xlate_svpoly_to_ext_svpoly_rpt", 0, 0, 0);
#endif

  /* Map signed glonass frequency to postive mapping (between 1 to 14) */
  dest_buf_ptr->b_FreqNum = source_buf_ptr->z_SvPolyReport.b_FreqNum + GLO_FREQ_NUMBER_OFFSET + 1;
  dest_buf_ptr->w_Sv = source_buf_ptr->z_SvPolyReport.w_Sv;
  dest_buf_ptr->w_SvPolyFlags = source_buf_ptr->z_SvPolyReport.w_SvPolyFlags;
  dest_buf_ptr->w_Iode = source_buf_ptr->z_SvPolyReport.w_Iode;
  dest_buf_ptr->q_enhancedIOD = source_buf_ptr->z_SvPolyReport.q_EnhIOD;
  dest_buf_ptr->d_T0 = source_buf_ptr->z_SvPolyReport.d_T0;
  for (i = 0; i < 3; i++)
  {
    dest_buf_ptr->pd_xyz0[i] = source_buf_ptr->z_SvPolyReport.pd_xyz0[i];
  }
  for (i = 0; i < 9; i++)
  {
    dest_buf_ptr->pd_xyzN[i] = source_buf_ptr->z_SvPolyReport.pd_xyzN[i];
  }
  for (i = 0; i < 4; i++)
  {
    dest_buf_ptr->pf_other[i] = source_buf_ptr->z_SvPolyReport.pf_other[i];
  }

  dest_buf_ptr->f_PosUnc = source_buf_ptr->z_SvPolyReport.f_PosUnc;
  dest_buf_ptr->f_IonoDelay = source_buf_ptr->z_SvPolyReport.f_IonoDelay;
  dest_buf_ptr->f_IonoDot = source_buf_ptr->z_SvPolyReport.f_IonoDot;
  dest_buf_ptr->f_sbasIonoDelay = source_buf_ptr->z_SvPolyReport.f_sbasIonoDelay;
  dest_buf_ptr->f_sbasIonoDot = source_buf_ptr->z_SvPolyReport.f_sbasIonoDot;
  dest_buf_ptr->f_TropoDelay = source_buf_ptr->z_SvPolyReport.f_TropoDelay;
  dest_buf_ptr->f_Elevation = source_buf_ptr->z_SvPolyReport.f_Elevation;
  dest_buf_ptr->f_ElevDot = source_buf_ptr->z_SvPolyReport.f_ElevDot;
  dest_buf_ptr->f_ElevUnc = source_buf_ptr->z_SvPolyReport.f_ElevUnc;
  dest_buf_ptr->w_MbHealth = source_buf_ptr->z_SvPolyReport.u_MbHealth;

  memset(dest_buf_ptr->d_VelCoef, 0, sizeof(dest_buf_ptr->d_VelCoef));
  if (SLOTID_OK(source_buf_ptr->z_SvPolyReport.w_Sv))
  {
    DBL *pd_VelCoefOut = dest_buf_ptr->d_VelCoef;
    const DBL *pd_XyzN = source_buf_ptr->z_SvPolyReport.pd_xyzN;
    for (i = 0; i < 3; i++)
    {
      *pd_VelCoefOut++ = (*pd_XyzN++);
      *pd_VelCoefOut++ = (*pd_XyzN++) * 2.0;
      *pd_VelCoefOut++ = (*pd_XyzN++) * 3.0;
      pd_VelCoefOut++;
    }
  }

  /*Both TGD fields are identical at the moment, so memscpy would have sufficed
    However, in case PE changes the structure we will have issues.
    So be safe, and do a field-to-field copy */

  if (PRN_OK(source_buf_ptr->z_SvPolyReport.w_Sv))
  {
    if (source_buf_ptr->z_SvPolyReport.z_Tgd.z_GpsTgdIsc.w_TgdFlag & GNSS_MB_TGD_ISC_L1CA_VALID)
    {
      dest_buf_ptr->t_SvPolyValidityMask |= PDSM_GPS_ISC_L1CA_VALID;
      dest_buf_ptr->f_gpsIscL1ca = source_buf_ptr->z_SvPolyReport.z_Tgd.z_GpsTgdIsc.f_IscL1caMsec;
    }

    if (source_buf_ptr->z_SvPolyReport.z_Tgd.z_GpsTgdIsc.w_TgdFlag & GNSS_MB_TGD_ISC_L2C_VALID)
    {
      dest_buf_ptr->t_SvPolyValidityMask |= PDSM_GPS_ISC_L2C_VALID;
      dest_buf_ptr->f_gpsIscL2c = source_buf_ptr->z_SvPolyReport.z_Tgd.z_GpsTgdIsc.f_IscL2cMsec;
    }

    if (source_buf_ptr->z_SvPolyReport.z_Tgd.z_GpsTgdIsc.w_TgdFlag & GNSS_MB_TGD_ISC_L5Q5_VALID)
    {
      dest_buf_ptr->t_SvPolyValidityMask |= PDSM_GPS_ISC_L5Q5_VALID;
      dest_buf_ptr->f_gpsIscL5Q5 = source_buf_ptr->z_SvPolyReport.z_Tgd.z_GpsTgdIsc.f_IscL5Q5Msec;
    }

    if (source_buf_ptr->z_SvPolyReport.z_Tgd.z_GpsTgdIsc.w_TgdFlag & GNSS_MB_TGD_ISC_L5I5_VALID)
    {
      dest_buf_ptr->t_SvPolyValidityMask |= PDSM_GPS_ISC_L5I5_VALID;
      dest_buf_ptr->f_gpsIscL5I5 = source_buf_ptr->z_SvPolyReport.z_Tgd.z_GpsTgdIsc.f_IscL5I5Msec;
    }

    if (source_buf_ptr->z_SvPolyReport.z_Tgd.z_GpsTgdIsc.w_TgdFlag & GNSS_MB_TGD1_VALID)
    {
      dest_buf_ptr->t_SvPolyValidityMask |= PDSM_GPS_TGD_VALID;
      dest_buf_ptr->f_gpsTgd = source_buf_ptr->z_SvPolyReport.z_Tgd.z_GpsTgdIsc.f_TgdMsec;
    }
  }
  else if (SLOTID_OK(source_buf_ptr->z_SvPolyReport.w_Sv))
  {
    if (source_buf_ptr->z_SvPolyReport.z_Tgd.z_GloL2L1.w_TgdFlag & GNSS_MB_DELTA_TN_VALID)
    {
      dest_buf_ptr->t_SvPolyValidityMask |= PDSM_GLO_TGD_G1G2_VALID;
      dest_buf_ptr->f_gloTgdG1G2 = source_buf_ptr->z_SvPolyReport.z_Tgd.z_GloL2L1.f_L1L2Msec;
    }
  }
  else if (GAL_ID_OK(source_buf_ptr->z_SvPolyReport.w_Sv))
  {
    if (source_buf_ptr->z_SvPolyReport.z_Tgd.z_GalBgd.w_TgdFlag & GNSS_MB_BGD_E1E5A_VALID)
    {
      dest_buf_ptr->t_SvPolyValidityMask |= PDSM_GAL_BGD_E1E5A_VALID;
      dest_buf_ptr->f_galBgdE1E5a = source_buf_ptr->z_SvPolyReport.z_Tgd.z_GalBgd.f_BgdE1E5aMsec;
    }

    if (source_buf_ptr->z_SvPolyReport.z_Tgd.z_GalBgd.w_TgdFlag & GNSS_MB_BGD_E1E5B_VALID)
    {
      dest_buf_ptr->t_SvPolyValidityMask |= PDSM_GAL_BGD_E1E5B_VALID;
      dest_buf_ptr->f_galBgdE1E5b = source_buf_ptr->z_SvPolyReport.z_Tgd.z_GalBgd.f_BgdE1E5bMsec;
    }
  }
  else if (BDS_ID_OK(source_buf_ptr->z_SvPolyReport.w_Sv))
  {
    if (source_buf_ptr->z_SvPolyReport.z_Tgd.z_BdsTgd.w_TgdFlag & GNSS_MB_TGD1_VALID)
    {
      dest_buf_ptr->t_SvPolyValidityMask |= PDSM_BDS_TGD_B1_VALID;
      dest_buf_ptr->f_bdsTgdB1 = source_buf_ptr->z_SvPolyReport.z_Tgd.z_BdsTgd.f_Tgd1Msec;
    }

    if (source_buf_ptr->z_SvPolyReport.z_Tgd.z_BdsTgd.w_TgdFlag & GNSS_MB_TGD2_VALID)
  {
      dest_buf_ptr->t_SvPolyValidityMask |= PDSM_BDS_TGD_B2_VALID;
      dest_buf_ptr->f_bdsTgdB2 = source_buf_ptr->z_SvPolyReport.z_Tgd.z_BdsTgd.f_Tgd2Msec;
  }

    if (source_buf_ptr->z_SvPolyReport.z_Tgd.z_BdsTgd.w_TgdFlag & GNSS_MB_TGD_B2A_VALID)
    {
      dest_buf_ptr->t_SvPolyValidityMask |= PDSM_BDS_TGD_B2A_VALID;
      dest_buf_ptr->f_bdsTgdB2a = source_buf_ptr->z_SvPolyReport.z_Tgd.z_BdsTgd.f_TgdB2aMsec;
    }

    if (source_buf_ptr->z_SvPolyReport.z_Tgd.z_BdsTgd.w_TgdFlag & GNSS_MB_ISC_B2A_VALID)
    {
      dest_buf_ptr->t_SvPolyValidityMask |= PDSM_BDS_ISC_B2A_VALID;
      dest_buf_ptr->f_bdsIscB2a = source_buf_ptr->z_SvPolyReport.z_Tgd.z_BdsTgd.f_IscB2aMsec;
    }
    }
  else if (NAVIC_PRN_OK(source_buf_ptr->z_SvPolyReport.w_Sv))
  {
    if (source_buf_ptr->z_SvPolyReport.z_Tgd.z_NavicTgd.w_TgdFlag & GNSS_MB_TGD1_VALID)
    {
      dest_buf_ptr->t_SvPolyValidityMask |= PDSM_NAVIC_TGD_L5_VALID;
      dest_buf_ptr->f_navicTgdL5 = source_buf_ptr->z_SvPolyReport.z_Tgd.z_NavicTgd.f_Tgd1Msec;
    }
  }
  return (TRUE);
}


/*===========================================================================

FUNCTION tm_core_xlate_gnss_prms_to_pd_gnss_meas

DESCRIPTION: Helper function to translate internal SV measurement in to raw data output QMI PD

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/
static boolean tm_core_xlate_gnss_prms_to_pd_gnss_meas
(
   const gnss_MeasStructType *p_MeasSrc,
   pdsm_LocSVMeasurementStructType *p_pdsmSVMeasDst,
   uint32 e_MeasBlkSrc
   )
{
  DBL d_BoundMsecs, d_SvMeas;
  const gnss_TimeSpeedStructType *p_SVTimeSpeed = NULL;
  uint32 eph_svmask = 0, alm_svmask = 0;
  uint16 u_svIdMinVal = 0;
  uint32 q_MeasStat = 0, q_MeasStat2 = 0;
  gnss_MeasBlkSrcEnumType e_PrimaryConstellation = GNSS_MEAS_BLK_SRC_MAX;
  if ((p_MeasSrc == NULL) ||
      (p_pdsmSVMeasDst == NULL))
  {
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "LPC:tm_core_xlate_gnss_prms_to_pd_gnss_meas received NULL pointer", 0, 0, 0);
    return (FALSE);
  }
  memset((void *)p_pdsmSVMeasDst, 0, sizeof(*p_pdsmSVMeasDst));

  p_SVTimeSpeed = &p_MeasSrc->z_UnfiltMeas;

  /* GNSS SV ID */
  p_pdsmSVMeasDst->w_gnssSvId = p_MeasSrc->w_Sv;
  e_PrimaryConstellation = sm_util_get_master_constellation(e_MeasBlkSrc);

  /* Glo Frequency */
  if (e_MeasBlkSrc == GNSS_MEAS_BLK_SRC_GLO)
  {
    /* Map signed glonass frequency to postive mapping (between 1 to 14) */
    p_pdsmSVMeasDst->u_gloFrequency = p_MeasSrc->b_FreqNum + GLO_FREQ_NUMBER_OFFSET + 1;
  }

  /* SV State */
  p_pdsmSVMeasDst->e_svStatus = tm_core_xlate_sv_state(p_MeasSrc->q_SvState);

  /* DON'T USE flags needs special handling. As per QMI definition all don't use flags are moved up in status2 word.
      So, mask them from q_MeasStatus of measurement block and mark them up in q_MeasStatus2 bit position */
  q_MeasStat = p_MeasSrc->q_MeasStatus;

  /* Top 10 MS bits of q_MeasStat2 are ear-marked as Reserved bits for DON'T use flags (per QMI definition). */
  q_MeasStat2 &= ~(PD_LOC_MEAS_STATUS2_DONT_USE_BITS);

  /* Evalute DONT_USE bit in q_MeasStat, if set, lit corresponding reserved DONT_USE bit in q_MeasStat2
     And clear the original DONT_USE bit from q_MeasStat
     Code is intentionally constructed to evaluate individual bits, to support any possibility of discontinuity
     in bit position assignment */
  if (q_MeasStat & PD_LOC_MEAS_STATUS_DONT_USE_X)
  {
    q_MeasStat &= ~PD_LOC_MEAS_STATUS_DONT_USE_X;
    q_MeasStat2 |= PD_LOC_MEAS_STATUS2_DONT_USE_X;
  }
  if (q_MeasStat & PD_LOC_MEAS_STATUS_DONT_USE_M)
  {
    q_MeasStat &= ~PD_LOC_MEAS_STATUS_DONT_USE_M;
    q_MeasStat2 |= PD_LOC_MEAS_STATUS2_DONT_USE_M;
  }
  if (q_MeasStat & PD_LOC_MEAS_STATUS_DONT_USE_D)
  {
    q_MeasStat &= ~PD_LOC_MEAS_STATUS_DONT_USE_D;
    q_MeasStat2 |= PD_LOC_MEAS_STATUS2_DONT_USE_D;
  }
  if (q_MeasStat & PD_LOC_MEAS_STATUS_DONT_USE_P)
  {
    q_MeasStat &= ~PD_LOC_MEAS_STATUS_DONT_USE_P;
    q_MeasStat2 |= PD_LOC_MEAS_STATUS2_DONT_USE_P;
  }
  if (q_MeasStat & PD_LOC_MEAS_STATUS_DONT_USE_I_J)
  {
    q_MeasStat &= ~PD_LOC_MEAS_STATUS_DONT_USE_I_J;
    q_MeasStat2 |= PD_LOC_MEAS_STATUS2_DONT_USE_I_J;
  }
  if (q_MeasStat & PD_LOC_MEAS_STATUS_DONT_USE_L_J)
  {
    q_MeasStat &= ~PD_LOC_MEAS_STATUS_DONT_USE_L_J;
    q_MeasStat2 |= PD_LOC_MEAS_STATUS2_DONT_USE_L_J;
  }
  if (q_MeasStat & PD_LOC_MEAS_STATUS_DONT_USE_S_M)
  {
    q_MeasStat &= ~PD_LOC_MEAS_STATUS_DONT_USE_S_M;
    q_MeasStat2 |= PD_LOC_MEAS_STATUS2_DONT_USE_S_M;
  }


  /* Only setting the bits from measStatus which are needed and have one to one mapping to QMI */

  /* MSB bits of measstatus2 that are set for DONT USE needs to be forward to QMI to indicate the client must not
     use the measurement */
     
  p_pdsmSVMeasDst->t_measurementStatus |= (uint64)(q_MeasStat2 & PD_LOC_MEAS_STATUS2_DONT_USE_BIT_MASK);  


  p_pdsmSVMeasDst->t_measurementStatus <<= 32;

  /* Bits 0-7, 9,10 and 27th from measstaus are taken and send to QMI where they have one to one mapping */

  p_pdsmSVMeasDst->t_measurementStatus |= (uint64)(q_MeasStat & PD_LOC_MEAS_STATUS_BIT_MASK); 

  /* Set validity bits corresponding to status2 */
  p_pdsmSVMeasDst->t_measurementStatusValid     = (uint64)(PD_LOC_MEAS_STATUS2_DONT_USE_X | PD_LOC_MEAS_STATUS2_DONT_USE_M |
                                                              PD_LOC_MEAS_STATUS2_DONT_USE_D | PD_LOC_MEAS_STATUS2_DONT_USE_S |
                                                              PD_LOC_MEAS_STATUS2_DONT_USE_P | PD_LOC_MEAS_STATUS2_DONT_USE_I_J |
                                                              PD_LOC_MEAS_STATUS2_DONT_USE_L_J | PD_LOC_MEAS_STATUS2_DONT_USE_S_M);
  /* Shift the bits up by 32 in status2 position */
  p_pdsmSVMeasDst->t_measurementStatusValid <<= 32;

  /* Insert validity bits of status mask position */
  p_pdsmSVMeasDst->t_measurementStatusValid |= (uint64)(PD_LOC_MEAS_STATUS_SM_VALID | PD_LOC_MEAS_STATUS_SB_VALID |
                                                           PD_LOC_MEAS_STATUS_MS_VALID | PD_LOC_MEAS_STATUS_BE_CONFIRM |
                                                           PD_LOC_MEAS_STATUS_VELOCITY_VALID | PD_LOC_MEAS_STATUS_VELOCITY_FINE |
                                                           PD_LOC_MEAS_STATUS_LP_VALID | PD_LOC_MEAS_STATUS_LP_POS_VALID |
                                                           PD_LOC_MEAS_STATUS_FROM_RNG_DIFF | PD_LOC_MEAS_STATUS_FROM_VE_DIFF |
                                                           PD_LOC_MEAS_STATUS_GNSS_FRESH_MEAS_VALID);

  switch (e_PrimaryConstellation)
  {
    case GNSS_MEAS_BLK_SRC_GPS:
      eph_svmask = tm_core_info.session_info.eph_svmask;
      alm_svmask = tm_core_info.session_info.alm_svmask;
      u_svIdMinVal = PDSM_PD_SV_ID_GPS_MIN;
      break;
    case GNSS_MEAS_BLK_SRC_GLO:
      eph_svmask = tm_core_info.session_info.glo_eph_svmask;
      alm_svmask = tm_core_info.session_info.glo_alm_svmask;
      u_svIdMinVal = PDSM_PD_SV_ID_GLO_MIN;
      break;
    case GNSS_MEAS_BLK_SRC_BDS:
      eph_svmask = tm_core_info.session_info.bds_eph_svmask;
      alm_svmask = tm_core_info.session_info.bds_alm_svmask;
      u_svIdMinVal = PDSM_PD_SV_ID_BDS_MIN;
      break;

    case GNSS_MEAS_BLK_SRC_QZSS_SBAS:
      eph_svmask = tm_core_info.session_info.qzss_eph_svmask;
      alm_svmask = tm_core_info.session_info.qzss_alm_svmask;
      u_svIdMinVal = PDSM_PD_SV_ID_QZSS_MIN;
      break;

    case GNSS_MEAS_BLK_SRC_GAL:
      eph_svmask = tm_core_info.session_info.gal_eph_svmask;
      alm_svmask = tm_core_info.session_info.gal_alm_svmask;
      u_svIdMinVal = PDSM_PD_SV_ID_GAL_MIN;
      break;

    case GNSS_MEAS_BLK_SRC_NAVIC:
      eph_svmask = tm_core_info.session_info.navic_eph_svmask;
      alm_svmask = tm_core_info.session_info.navic_alm_svmask;
      u_svIdMinVal = PDSM_PD_SV_ID_NAVIC_MIN;
      break;

    default:
      MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_HIGH,
            "tm_core_xlate_gnss_prms_to_pd_gnss_meas Invalid MeasBlKSrc %d",
            e_MeasBlkSrc);

      return (FALSE);
      break;
  }

  /* Populate ephemeris, Almanac status */
  p_pdsmSVMeasDst->e_svInfoMask = 0;
  if ((eph_svmask & (1 << (p_MeasSrc->w_Sv - u_svIdMinVal))) != 0)
  {
    p_pdsmSVMeasDst->e_svInfoMask |= PD_LOC_SVINFO_MASK_HAS_EPHEMERIS;
  }

  if ((alm_svmask & (1 << (p_MeasSrc->w_Sv - u_svIdMinVal))) != 0)
  {
    p_pdsmSVMeasDst->e_svInfoMask |= PD_LOC_SVINFO_MASK_HAS_ALMANAC;
  }

  /* For Measurement Reporting, Health is always VALID i.e SV is Healthy
     Measurement will not be reported for unhealthy SVs */
  p_pdsmSVMeasDst->v_healthStatus_valid = TRUE;
  p_pdsmSVMeasDst->u_healthStatus = PD_LOC_MEAS_SV_HEALTH_STATUS_HEALTHY;

  p_pdsmSVMeasDst->w_CNo = p_MeasSrc->w_Cno;
#ifndef FEATURE_GNSS_NON_STANDARD_SUPPORT
  /* Need to consider RF loss only if Cno is positive.
     SVs with Cno=0 will be filtered out later and won't be reported at QMI */
  if (p_MeasSrc->w_Cno > 0)
    {
      p_pdsmSVMeasDst->w_CNo = p_MeasSrc->w_Cno + p_MeasSrc->w_RfLoss;     
    }
#endif // !FEATURE_GNSS_NON_STANDARD_SUPPORT

  p_pdsmSVMeasDst->w_gloRfLoss = p_MeasSrc->w_RfLoss;

  p_pdsmSVMeasDst->v_lossOfLockValid = FALSE;
  p_pdsmSVMeasDst->b_lossOfLock = 0; // Loss of lock Logic to be added here

  p_pdsmSVMeasDst->x_measLatency = (int16)p_MeasSrc->l_LatencyMs;

  /* Measurement */
  p_pdsmSVMeasDst->z_svTimeSpeed.q_svMs = p_MeasSrc->z_UnfiltMeas.z_SvTime.q_Ms;
  p_pdsmSVMeasDst->z_svTimeSpeed.f_svSubMs = p_MeasSrc->z_UnfiltMeas.z_SvTime.f_SubMs;
  p_pdsmSVMeasDst->z_svTimeSpeed.f_svTimeUncMs = p_MeasSrc->z_UnfiltMeas.f_SvTimeUncMs;
  p_pdsmSVMeasDst->z_svTimeSpeed.f_dopplerShift = p_MeasSrc->z_UnfiltMeas.f_SvSpeed;
  p_pdsmSVMeasDst->z_svTimeSpeed.f_dopplerShiftUnc = p_MeasSrc->z_UnfiltMeas.f_SvSpeedUnc;

  p_pdsmSVMeasDst->v_multipathEstValid = FALSE;

  if ((p_MeasSrc->q_MeasStatus & MEAS_STATUS_VE_FINE) &&
      (p_MeasSrc->q_MeasStatus & MEAS_STATUS_VE_VALID))
  {
    p_pdsmSVMeasDst->v_fineSpeedValid = TRUE;
    p_pdsmSVMeasDst->f_fineSpeed = p_MeasSrc->f_FineSpeed;
    p_pdsmSVMeasDst->v_fineSpeedUncValid = TRUE;
    p_pdsmSVMeasDst->f_fineSpeedUnc = p_MeasSrc->f_FineSpeedUnc;
    p_pdsmSVMeasDst->v_dopplerAccelValid = FALSE;

    /* Carrier Phase is valid when VE_FINE is valid.
       LP_VALID may not be set, and AP needs to check
       for that and set Half cycle ambiguity accordingly */
    p_pdsmSVMeasDst->v_carrierPhaseValid = TRUE;
    p_pdsmSVMeasDst->d_carrierPhase = p_MeasSrc->d_CarrierPhase;
    p_pdsmSVMeasDst->v_carrierPhaseUncValid = TRUE;
    p_pdsmSVMeasDst->f_CarrierPhaseUnc = p_MeasSrc->f_CarrierPhaseUnc;
  }

  /* cycle slip count is always valid. because MEAS_STATUS_LP_VALID
     being false OVERRIDES everything to ensure we cannot use the carrier phase */
  p_pdsmSVMeasDst->v_cycleSlipCountValid = TRUE;
    p_pdsmSVMeasDst->u_cycleSlipCount = p_MeasSrc->u_CSlipCount;

  if (p_MeasSrc->u_SvDirValid)
  {
    p_pdsmSVMeasDst->v_svDirectionValid = TRUE;
    p_pdsmSVMeasDst->f_svAzimuth = p_MeasSrc->f_Azimuth;
    p_pdsmSVMeasDst->f_svElevation = p_MeasSrc->f_Elevation;
  }

  p_pdsmSVMeasDst->u_GoodObs = p_MeasSrc->u_GoodObs;
  p_pdsmSVMeasDst->u_Observe = p_MeasSrc->u_Observe;
  p_pdsmSVMeasDst->u_FilterN = p_MeasSrc->u_FilterN;
  p_pdsmSVMeasDst->u_PreInt = p_MeasSrc->u_PreInt;
  p_pdsmSVMeasDst->w_PostInt = p_MeasSrc->w_PostInt;
  p_pdsmSVMeasDst->w_ParityErrorCount = p_MeasSrc->w_ParityErrorCount;

  //NOT SUPPORTED by ME. To be populated by LBS
  p_pdsmSVMeasDst->v_SnrValid = FALSE;
  p_pdsmSVMeasDst->f_Snr = 0;

  //NOT SUPPORTED by ME
  p_pdsmSVMeasDst->v_AbsSigPowerValid = FALSE;
  p_pdsmSVMeasDst->f_AbsSigPower = 0;

#ifdef FEATURE_GNSS_RAW_MEAS_OUTPUT_DEBUG
  {
    char buf[1024];
    snprintf(buf, sizeof(buf), "RAW_CLK_MEAS:%d,%.2u,%llx,%.4d,%.4lu,%.4f,%.4f,%.4f,%.4f\n",
             p_pdsmSVMeasDst->w_gnssSvId, p_pdsmSVMeasDst->w_CNo, p_pdsmSVMeasDst->t_measurementStatus,
             p_pdsmSVMeasDst->x_measLatency, p_pdsmSVMeasDst->z_svTimeSpeed.q_svMs, p_pdsmSVMeasDst->z_svTimeSpeed.f_svSubMs,
             p_pdsmSVMeasDst->z_svTimeSpeed.f_svTimeUncMs, p_pdsmSVMeasDst->z_svTimeSpeed.f_dopplerShift, p_pdsmSVMeasDst->z_svTimeSpeed.f_dopplerShiftUnc);

    MSG_SPRINTF_1(MSG_SSID_GPSSM, MSG_LEGACY_MED, "%s", buf);
  }
#endif /* FEATURE_GNSS_RAW_MEAS_OUTPUT_DEBUG */
  return (TRUE);
} /*tm_core_xlate_gnss_prms_to_pd_gnss_meas */


/*===========================================================================

FUNCTION tm_core_xlate_gnss_prm_to_pd_meas

DESCRIPTION
    Helper function to translate internal PRM report in to measurement report
    suitable for output on QMI (SV Info Indication)
DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/
static uint8 tm_core_xlate_gnss_prm_to_pd_meas
(
   pdsm_LocGnssSignalEnumType    e_GnssSignal,
   pdsm_pd_meas_s_type           *report,
   const gnss_MeasStructType     *p_GnssMeas,
   uint8                         u_NumSvs,
   uint8                         max_svs
   )
{
  uint8  i;
  uint8  valid_sv_count = 0;
  uint32 q_prm;
  uint8  u_x;
  FLT    f_prm_sigma = 0.0, f_prm_limit = 0.0, f_y = 0.0;

  if ((NULL == report) || (NULL == p_GnssMeas) || (u_NumSvs > N_ACTIVE_CHAN))
  {
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR,
          "LPC:Invalid Arg: NULL Ptr Checks %d %d, NumSvs=%u",
          NULL == report, NULL == p_GnssMeas, u_NumSvs);
    return 0;
  }

  for (i = 0; i < u_NumSvs && valid_sv_count < max_svs; i++)
  {
    if (FALSE == gnss_SvIsBlacklisted(&tm_core_info.config_info.z_GnssPersistSV, p_GnssMeas[i].w_Sv))
  {
      /* Report only valid measurements, validity check already performed in lm_mgp.c */
      report->flags = 1;
      report->sv_id = p_GnssMeas[i].w_Sv;
      report->c_no = p_GnssMeas[i].w_Cno;
      report->rf_loss = p_GnssMeas[i].w_RfLoss;

#ifndef FEATURE_GNSS_NON_STANDARD_SUPPORT
      /* Need to consider RF loss only if Cno is positive.
         SVs with Cno=0 will be filtered out later and won't be reported at QMI */
      if (p_GnssMeas[i].w_Cno > 0)
  {
        report->c_no        = p_GnssMeas[i].w_Cno + p_GnssMeas[i].w_RfLoss;
  }
#endif //!FEATURE_GNSS_NON_STANDARD_SUPPORT
      report->elevation = 0;
      report->azimuth = 0;
      report->elevation_rad = 0;
      report->azimuth_rad = 0;

      if (p_GnssMeas[i].u_SvDirValid && (p_GnssMeas[i].f_Elevation > 0))
  {
        report->elevation = p_GnssMeas[i].f_Elevation * C_RAD_TO_DEG; /*lint !e524 */
        report->azimuth = p_GnssMeas[i].f_Azimuth   * C_RAD_TO_DEG; /*lint !e524 */
        report->elevation_rad = p_GnssMeas[i].f_Elevation;
        report->azimuth_rad = p_GnssMeas[i].f_Azimuth;
  }

      report->latency_ms = (int16)p_GnssMeas[i].l_LatencyMs;
      report->sv_state = tm_core_xlate_sv_state((gnss_SvStateEnumType)p_GnssMeas[i].q_SvState);

      report->chan_rpt_type.ps_doppler = (int32)(p_GnssMeas[i].z_FiltMeas.f_SvSpeed * C_MsToL1Hz * (-5.0));
      report->gnss_signal_type = PD_LOC_GNSS_SIGNAL_TYPE_INVALID;

      if (PD_LOC_GNSS_SIGNAL_TYPE_GPS_L1CA == e_GnssSignal || PD_LOC_GNSS_SIGNAL_TYPE_GPS_L1C == e_GnssSignal ||
          PD_LOC_GNSS_SIGNAL_TYPE_GPS_L2C_L == e_GnssSignal || PD_LOC_GNSS_SIGNAL_TYPE_GPS_L5_Q == e_GnssSignal ||
          PD_LOC_GNSS_SIGNAL_TYPE_QZSS_L1CA == e_GnssSignal || PD_LOC_GNSS_SIGNAL_TYPE_QZSS_L1S == e_GnssSignal ||
          PD_LOC_GNSS_SIGNAL_TYPE_QZSS_L2C_L == e_GnssSignal || PD_LOC_GNSS_SIGNAL_TYPE_QZSS_L5_Q == e_GnssSignal)
      {
        report->sv_freq_num = 0xff; /* No frequency number for GPS/QZSS */

        q_prm = (uint32)((1.0 - p_GnssMeas[i].z_FiltMeas.z_SvTime.f_SubMs) * 1047552.0); // 1023*1024
        report->chan_rpt_type.sv_code_ph_wh = (uint16)(q_prm >> 10);

        if (report->chan_rpt_type.sv_code_ph_wh == (CA_CHIPS_MSEC * CA_PERIOD_MSEC)) // GPS # of C/A chips per msec
        {
          report->chan_rpt_type.sv_code_ph_wh = 0;
        }
  
        report->chan_rpt_type.sv_code_ph_fr = (int16)(q_prm & 0x3ff);
        report->gnss_signal_type = e_GnssSignal;
  }
      else if (PD_LOC_GNSS_SIGNAL_TYPE_GLONASS_G1 == e_GnssSignal || PD_LOC_GNSS_SIGNAL_TYPE_GLONASS_G2 == e_GnssSignal)
  {
        report->sv_freq_num = p_GnssMeas[i].b_FreqNum; /* GLONASS frequency numbers [-7:6] */

        q_prm = (uint32)((1.0 - p_GnssMeas[i].z_FiltMeas.z_SvTime.f_SubMs) * 261632.0); // 511*512
        report->chan_rpt_type.sv_code_ph_wh = (uint16)(q_prm >> 9);

        if (report->chan_rpt_type.sv_code_ph_wh == (GLO_CA_CHIPS_MSEC * GLO_CA_PERIOD_MSEC)) // GLO # of C/A chips per msec
  {
          report->chan_rpt_type.sv_code_ph_wh = 0;
  }

        report->chan_rpt_type.sv_code_ph_fr = (int16)(q_prm & 0x1ff);
        report->gnss_signal_type = e_GnssSignal;
  }
      else if (PD_LOC_GNSS_SIGNAL_TYPE_BEIDOU_B1_I == e_GnssSignal || PD_LOC_GNSS_SIGNAL_TYPE_BEIDOU_B2_I == e_GnssSignal ||
               PD_LOC_GNSS_SIGNAL_TYPE_BEIDOU_B2A_Q == e_GnssSignal)
  {
        report->sv_freq_num = 0xff; /* No frequency number for BDS */

        q_prm = (uint32)((1.0 - p_GnssMeas[i].z_FiltMeas.z_SvTime.f_SubMs) * 4190208.0); // 2046*2048
        report->chan_rpt_type.sv_code_ph_wh = (uint16)(q_prm >> 11);

        if (report->chan_rpt_type.sv_code_ph_wh == (BDS_CA_CHIPS_MSEC * BDS_CA_PERIOD_MSEC)) // BDS # of C/A chips per msec
  {
          report->chan_rpt_type.sv_code_ph_wh = 0;
  }

        report->chan_rpt_type.sv_code_ph_fr = (int16)(q_prm & 0x7ff);
        report->gnss_signal_type = e_GnssSignal;
  }
      else if (PD_LOC_GNSS_SIGNAL_TYPE_GALILEO_E1_C == e_GnssSignal || PD_LOC_GNSS_SIGNAL_TYPE_GALILEO_E5A_Q == e_GnssSignal ||
               PD_LOC_GNSS_SIGNAL_TYPE_GALILEO_E5B_Q == e_GnssSignal)
  {
        report->sv_freq_num = 0xff; /* No frequency number for GAL */
        q_prm = (uint32)((1.0 - p_GnssMeas[i].z_FiltMeas.z_SvTime.f_SubMs) * 1047552.0); // 1023*1024
        q_prm += (3 - (p_GnssMeas[i].z_FiltMeas.z_SvTime.q_Ms % 4)) * 1023 * 1024;
        report->chan_rpt_type.sv_code_ph_wh = (uint16)(q_prm >> 10);

        if (report->chan_rpt_type.sv_code_ph_wh == (GAL_CA_CHIPS_MSEC * GAL_CA_PERIOD_MSEC)) // GAL # of C/A chips per msec
  {
          report->chan_rpt_type.sv_code_ph_wh = 0;
  }

        report->chan_rpt_type.sv_code_ph_fr = (int16)(q_prm & 0x3ff);
        report->gnss_signal_type = e_GnssSignal;
  }
      else if (PD_LOC_GNSS_SIGNAL_TYPE_NAVIC_L5 == e_GnssSignal)
  {
        report->sv_freq_num = 0xff; /* No frequency number for GPS/QZSS */

        q_prm = (uint32)((1.0 - p_GnssMeas[i].z_FiltMeas.z_SvTime.f_SubMs) * 1047552.0); // 1023*1024
        report->chan_rpt_type.sv_code_ph_wh = (uint16)(q_prm >> 10);

        if (report->chan_rpt_type.sv_code_ph_wh == (NAVIC_CA_CHIPS_MSEC * NAVIC_CA_PERIOD_MSEC)) // GPS # of C/A chips per msec
  {
          report->chan_rpt_type.sv_code_ph_wh = 0;
  }

        report->chan_rpt_type.sv_code_ph_fr = (int16)(q_prm & 0x3ff);
        report->gnss_signal_type = e_GnssSignal;
  }
  
      if ((p_GnssMeas[i].u_Observe == 1) && (p_GnssMeas[i].u_GoodObs == 1))
  {
        /* Early exit passed after first sniff */
        report->chan_rpt_type.mul_path_ind = 0;
  }
      else if ((p_GnssMeas[i].u_PreInt == 160) &&
               (PD_LOC_GNSS_SIGNAL_TYPE_GLONASS_G1 != e_GnssSignal) && (PD_LOC_GNSS_SIGNAL_TYPE_GLONASS_G2 != e_GnssSignal))
  {
        /* SA Data was used */
        report->chan_rpt_type.mul_path_ind = 7;
  }
      else
      {
        uint32 q_MeasSrchTime;
  
        q_MeasSrchTime = p_GnssMeas[i].u_PreInt * p_GnssMeas[i].w_PostInt;

        if (q_MeasSrchTime < 2560)
  {
          report->chan_rpt_type.mul_path_ind = 4;
        }
        else if (q_MeasSrchTime < 5120)
  {
          report->chan_rpt_type.mul_path_ind = 5;
  }
        else
  {
          report->chan_rpt_type.mul_path_ind = 6;
  }
  }

      /* Calculate range uncertainty. */
      f_prm_sigma = p_GnssMeas[i].z_FiltMeas.f_SvTimeUncMs * (FLT)LIGHT_MSEC;

      /* Following legacy code, report XCorrs with Saturated RMSE */
      if (p_GnssMeas[i].q_MeasStatus & SM_MEAS_STATUS_XCORR)
  {
        report->chan_rpt_type.ps_range_rms_er = 63;
  }
      /* First do settings at low and high end of the allowed pseudorange unc. range */
      else if (f_prm_sigma < 0.5)
  {
        report->chan_rpt_type.ps_range_rms_er = 0;
  }
      else if ((f_prm_sigma < 112.0) && (f_prm_sigma >= 104.0))
  {
        report->chan_rpt_type.ps_range_rms_er = 62;
  }
      else if (f_prm_sigma >= 112.0)
  {
        /* Any value greater than 112m should be mapped to the 62 because this
           will give the PDM the possible chance of using this measurement with a
           lower weight */
        report->chan_rpt_type.ps_range_rms_er = 62;
  }
      else
  {
        for (report->chan_rpt_type.ps_range_rms_er = 1;
             report->chan_rpt_type.ps_range_rms_er < 62;
             report->chan_rpt_type.ps_range_rms_er++)
    {
          /* Get the exponent part */
          u_x = report->chan_rpt_type.ps_range_rms_er >> 3;
          /* Get the mantissa part. */
          f_y = (FLT)(report->chan_rpt_type.ps_range_rms_er & 7);
          f_prm_limit = (0.5F + f_y * 0.0625F) * (FLT)(1 << u_x);

          if (f_prm_sigma < f_prm_limit)
  {
            break;
      }
  }
  }

#ifdef FEATURE_GNSS_RAW_MEAS_OUTPUT_DEBUG
      MSG_2(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "tm_core_xlate_gnss_prm_to_pd_meas: svid=%d signal_type=%d",
            report->sv_id, report->gnss_signal_type);
#endif
    valid_sv_count++;
    report++;
    }
  }   /* end for loop iterating over SV prms */
  return valid_sv_count;
}


/*===========================================================================

FUNCTION tm_core_xlate_prms_to_ext_status

DESCRIPTION

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/
static void tm_core_xlate_prms_to_ext_status
       (
   pdsm_pd_ext_meas_s_type       *dest_buf_ptr,
   const tm_core_param_s_type    *source_buf_ptr
)
  {
  uint8 valid_sv_count = 0;

  if ((dest_buf_ptr == NULL) || (source_buf_ptr == NULL))
  {
    return;
  }


  dest_buf_ptr->flags         = 0;
  dest_buf_ptr->eph_svmask    = tm_core_info.session_info.eph_svmask;
  dest_buf_ptr->alm_svmask    = tm_core_info.session_info.alm_svmask;
  dest_buf_ptr->health_svmask = tm_core_info.session_info.health_svmask;

  dest_buf_ptr->iono_valid    = 1;
  if (tm_core_info.session_info.iono)
  {
    dest_buf_ptr->iono_valid = 0;
  }

  dest_buf_ptr->time_unc_ms   = (uint32)source_buf_ptr->gps_meas_blk.z_GnssHeader.z_MeasClock.z_Time.z_GpsTime.f_ClkTimeUncMs;
  if (dest_buf_ptr->time_unc_ms < 20)
  {
    dest_buf_ptr->time_unc_ms = 20;
  }
  dest_buf_ptr->glo_eph_svmask = tm_core_info.session_info.glo_eph_svmask;
  dest_buf_ptr->glo_alm_svmask = tm_core_info.session_info.glo_alm_svmask;

  dest_buf_ptr->bds_eph_svmask = tm_core_info.session_info.bds_eph_svmask;
  dest_buf_ptr->bds_alm_svmask = tm_core_info.session_info.bds_alm_svmask;

  dest_buf_ptr->qzss_eph_svmask = tm_core_info.session_info.qzss_eph_svmask;
  dest_buf_ptr->qzss_alm_svmask = tm_core_info.session_info.qzss_alm_svmask;
  dest_buf_ptr->qzss_iono = tm_core_info.session_info.qzss_iono;
  dest_buf_ptr->qzss_health_svmask = tm_core_info.session_info.qzss_health_svmask;
  dest_buf_ptr->bds_health_svmask = tm_core_info.session_info.bds_health_svmask;
  dest_buf_ptr->gal_health_svmask = tm_core_info.session_info.gal_health_svmask;
  dest_buf_ptr->glo_health_svmask = tm_core_info.session_info.glo_health_svmask;

  dest_buf_ptr->gal_eph_svmask = tm_core_info.session_info.gal_eph_svmask;
  dest_buf_ptr->gal_alm_svmask = tm_core_info.session_info.gal_alm_svmask;

  dest_buf_ptr->navic_eph_svmask = tm_core_info.session_info.navic_eph_svmask;
  dest_buf_ptr->navic_alm_svmask = tm_core_info.session_info.navic_alm_svmask;
  dest_buf_ptr->navic_health_svmask = tm_core_info.session_info.navic_health_svmask;

  /**********************************************************************************
   *  The order of the two following function calls is important, do not swap without
   *  changing parameters
   *********************************************************************************/

  /*
   * Translate gps prms to pdsm_pd_meas_s_type, and store in ext_meas_report_type array
   * starting from index 0.
   */
  valid_sv_count += tm_core_xlate_gnss_prm_to_pd_meas
     (
     PD_LOC_GNSS_SIGNAL_TYPE_GPS_L1CA,
     &dest_buf_ptr->ext_meas_report_type[0],
     source_buf_ptr->gps_meas_blk.z_Gnss,
     source_buf_ptr->gps_meas_blk.z_GnssHeader.u_NumSvs,
     PDSM_EXT_MEAS_NUM_SVS
     );  /* Number of entries available for gps is PDSM_EXT_MEAS_NUM_SVS */

  if (valid_sv_count < PDSM_EXT_MEAS_NUM_SVS)
  {
    valid_sv_count += tm_core_xlate_gnss_prm_to_pd_meas
       (
       PD_LOC_GNSS_SIGNAL_TYPE_GPS_L2C_L,
       &dest_buf_ptr->ext_meas_report_type[valid_sv_count],
       source_buf_ptr->gps_l2c_meas_blk.z_Gnss,
       source_buf_ptr->gps_l2c_meas_blk.z_GnssHeader.u_NumSvs,
       PDSM_EXT_MEAS_NUM_SVS - valid_sv_count
       );  /* Number of entries available for gps is PDSM_EXT_MEAS_NUM_SVS */
  }
  else
  {
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "LPC:No space for GPS L5Q prms: GPS SVs > PDSM_EXT_MEAS_NUM_SVS", 0, 0, 0);
  }

  if (valid_sv_count < PDSM_EXT_MEAS_NUM_SVS)
  {
    valid_sv_count += tm_core_xlate_gnss_prm_to_pd_meas
       (
       PD_LOC_GNSS_SIGNAL_TYPE_GPS_L5_Q,
       &dest_buf_ptr->ext_meas_report_type[valid_sv_count],
       source_buf_ptr->gps_l5q_meas_blk.z_Gnss,
       source_buf_ptr->gps_l5q_meas_blk.z_GnssHeader.u_NumSvs,
       PDSM_EXT_MEAS_NUM_SVS - valid_sv_count
       );  /* Number of entries available for gps is PDSM_EXT_MEAS_NUM_SVS */
  }
  else
  {
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "LPC:No space for GPS L5Q prms: GPS SVs > PDSM_EXT_MEAS_NUM_SVS", 0, 0, 0);
  }

  /*
   * Translate glo prms to pdsm_pd_meas_s_type, and store in ext_meas_report_type array
   * starting from the first free index after gps prms were used up.
   */
  if (valid_sv_count < PDSM_EXT_MEAS_NUM_SVS)
  {
    valid_sv_count += tm_core_xlate_gnss_prm_to_pd_meas
       (
       PD_LOC_GNSS_SIGNAL_TYPE_GLONASS_G1,
       &dest_buf_ptr->ext_meas_report_type[valid_sv_count],
       source_buf_ptr->glo_meas_blk.z_Gnss,
       source_buf_ptr->glo_meas_blk.z_GnssHeader.u_NumSvs,
       PDSM_EXT_MEAS_NUM_SVS - valid_sv_count
       ); /* Number of entries available for glo is PDSM_EXT_MEAS_NUM_SVS -
             Number of entries used by GPS */
  }
  else
  {
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "LPC:No space for GLO prms: GPS SVs > PDSM_EXT_MEAS_NUM_SVS", 0, 0, 0);
  }

  if (valid_sv_count < PDSM_EXT_MEAS_NUM_SVS)
  {
    valid_sv_count += tm_core_xlate_gnss_prm_to_pd_meas
       (
       PD_LOC_GNSS_SIGNAL_TYPE_GLONASS_G2,
       &dest_buf_ptr->ext_meas_report_type[valid_sv_count],
       source_buf_ptr->glo_g2_meas_blk.z_Gnss,
       source_buf_ptr->glo_g2_meas_blk.z_GnssHeader.u_NumSvs,
       PDSM_EXT_MEAS_NUM_SVS - valid_sv_count
       ); /* Number of entries available for glo is PDSM_EXT_MEAS_NUM_SVS -
             Number of entries used by GPS */
  }
  else
  {
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "LPC:No space for GLO G2 prms: GPS SVs > PDSM_EXT_MEAS_NUM_SVS", 0, 0, 0);
  }

  /*
   * Translate bds prms to pdsm_pd_meas_s_type, and store in ext_meas_report_type array
   * starting from the first free index after glo prms were used up.
   */
  if (valid_sv_count < PDSM_EXT_MEAS_NUM_SVS)
  {
    valid_sv_count += tm_core_xlate_gnss_prm_to_pd_meas
       (
       PD_LOC_GNSS_SIGNAL_TYPE_BEIDOU_B1_I,
       &dest_buf_ptr->ext_meas_report_type[valid_sv_count],
       source_buf_ptr->bds_meas_blk.z_Gnss,
       source_buf_ptr->bds_meas_blk.z_GnssHeader.u_NumSvs,
       PDSM_EXT_MEAS_NUM_SVS - valid_sv_count
       ); /* Number of entries available for bds is PDSM_EXT_MEAS_NUM_SVS -
             Number of entries used by GPS & GLO */
  }
  else
  {
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "LPC:No space for BDS prms: GPS/GLO SVs > PDSM_EXT_MEAS_NUM_SVS", 0, 0, 0);
  }

  if (valid_sv_count < PDSM_EXT_MEAS_NUM_SVS)
  {
    valid_sv_count += tm_core_xlate_gnss_prm_to_pd_meas
       (
       PD_LOC_GNSS_SIGNAL_TYPE_BEIDOU_B2_I,
       &dest_buf_ptr->ext_meas_report_type[valid_sv_count],
       source_buf_ptr->bds_b2_meas_blk.z_Gnss,
       source_buf_ptr->bds_b2_meas_blk.z_GnssHeader.u_NumSvs,
       PDSM_EXT_MEAS_NUM_SVS - valid_sv_count
       ); /* Number of entries available for bds is PDSM_EXT_MEAS_NUM_SVS -
             Number of entries used by GPS & GLO */
  }
  else
  {
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "LPC:No space for BDS B2 prms: GPS/GLO SVs > PDSM_EXT_MEAS_NUM_SVS", 0, 0, 0);
  }

  /*
   * Translate qzss prms to pdsm_pd_meas_s_type, and store in ext_meas_report_type array
   * starting from the first free index after bds prms were used up.
   */
  if (valid_sv_count < PDSM_EXT_MEAS_NUM_SVS)
  {
    valid_sv_count += tm_core_xlate_gnss_prm_to_pd_meas
       (
       PD_LOC_GNSS_SIGNAL_TYPE_QZSS_L1CA,
       &dest_buf_ptr->ext_meas_report_type[valid_sv_count],
       source_buf_ptr->qzss_meas_blk.z_Gnss,
       source_buf_ptr->qzss_meas_blk.z_GnssHeader.u_NumSvs,
       PDSM_EXT_MEAS_NUM_SVS - valid_sv_count
       ); /* Number of entries available for qzss is PDSM_EXT_MEAS_NUM_SVS -
             Number of entries used by GPS & GLO & BDS */
  }
  else
  {
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "LPC:No space for QZSS prms: GPS/GLO/BDS SVs > PDSM_EXT_MEAS_NUM_SVS", 0, 0, 0);
  }

  if (valid_sv_count < PDSM_EXT_MEAS_NUM_SVS)
  {
    valid_sv_count += tm_core_xlate_gnss_prm_to_pd_meas
       (
       PD_LOC_GNSS_SIGNAL_TYPE_QZSS_L2C_L,
       &dest_buf_ptr->ext_meas_report_type[valid_sv_count],
       source_buf_ptr->qzss_l2c_meas_blk.z_Gnss,
       source_buf_ptr->qzss_l2c_meas_blk.z_GnssHeader.u_NumSvs,
       PDSM_EXT_MEAS_NUM_SVS - valid_sv_count
       ); /* Number of entries available for qzss is PDSM_EXT_MEAS_NUM_SVS -
             Number of entries used by GPS & GLO & BDS */
   }
  else
  {
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "LPC:No space for QZSS L2 prms: GPS/GLO/BDS SVs > PDSM_EXT_MEAS_NUM_SVS", 0, 0, 0);
  }

  /*
  * Translate qzss prms to pdsm_pd_meas_s_type, and store in ext_meas_report_type array
  * starting from the first free index after bds prms were used up.
  */
  if (valid_sv_count < PDSM_EXT_MEAS_NUM_SVS)
  {
    valid_sv_count += tm_core_xlate_gnss_prm_to_pd_meas
       (
       PD_LOC_GNSS_SIGNAL_TYPE_QZSS_L5_Q,
       &dest_buf_ptr->ext_meas_report_type[valid_sv_count],
       source_buf_ptr->qzss_l5q_meas_blk.z_Gnss,
       source_buf_ptr->qzss_l5q_meas_blk.z_GnssHeader.u_NumSvs,
       PDSM_EXT_MEAS_NUM_SVS - valid_sv_count
       ); /* Number of entries available for qzss is PDSM_EXT_MEAS_NUM_SVS -
             Number of entries used by GPS & GLO & BDS */
  }
  else
  {
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "LPC:No space for QZSS L5 prms: GPS/GLO/BDS SVs > PDSM_EXT_MEAS_NUM_SVS", 0, 0, 0);
  }


  /*
 * Translate gal prms to pdsm_pd_meas_s_type, and store in ext_meas_report_type array
 * starting from the first free index after glo prms were used up.
 */
  if (valid_sv_count < PDSM_EXT_MEAS_NUM_SVS)
  {
    valid_sv_count += tm_core_xlate_gnss_prm_to_pd_meas
       (
       PD_LOC_GNSS_SIGNAL_TYPE_GALILEO_E1_C,
       &dest_buf_ptr->ext_meas_report_type[valid_sv_count],
       source_buf_ptr->gal_meas_blk.z_Gnss,
       source_buf_ptr->gal_meas_blk.z_GnssHeader.u_NumSvs,
       PDSM_EXT_MEAS_NUM_SVS - valid_sv_count
       ); /* Number of entries available for gal is PDSM_EXT_MEAS_NUM_SVS -
             Number of entries used by GPS, GLO, BDS & QZSS*/
  }
  else
  {
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "LPC:No space for GAL prms: GPS/GLO/BDS/QZSS SVs > PDSM_EXT_MEAS_NUM_SVS", 0, 0, 0);
  }

  if (valid_sv_count < PDSM_EXT_MEAS_NUM_SVS)
  {
    valid_sv_count += tm_core_xlate_gnss_prm_to_pd_meas
(
       PD_LOC_GNSS_SIGNAL_TYPE_GALILEO_E5A_Q,
       &dest_buf_ptr->ext_meas_report_type[valid_sv_count],
       source_buf_ptr->gal_e5a_meas_blk.z_Gnss,
       source_buf_ptr->gal_e5a_meas_blk.z_GnssHeader.u_NumSvs,
       PDSM_EXT_MEAS_NUM_SVS - valid_sv_count
       ); /* Number of entries available for gal is PDSM_EXT_MEAS_NUM_SVS -
             Number of entries used by GPS, GLO, BDS & QZSS*/
  }
  else
  {
    MSG(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "LPC:No space for GAL E5a prms: GPS/GAL/GLO/BDS/QZSS SVs > PDSM_EXT_MEAS_NUM_SVS");
  }

  if (valid_sv_count < PDSM_EXT_MEAS_NUM_SVS)
  {
    valid_sv_count += tm_core_xlate_gnss_prm_to_pd_meas
       (
       PD_LOC_GNSS_SIGNAL_TYPE_BEIDOU_B2A_Q,
       &dest_buf_ptr->ext_meas_report_type[valid_sv_count],
       source_buf_ptr->bds_b2aq_meas_blk.z_Gnss,
       source_buf_ptr->bds_b2aq_meas_blk.z_GnssHeader.u_NumSvs,
       PDSM_EXT_MEAS_NUM_SVS - valid_sv_count
       ); /* Number of entries available for BDS B2A_Q is PDSM_EXT_MEAS_NUM_SVS -
             Number of entries used by GPS, GLO, GAL & QZSS*/
  }
  else
  {
    MSG(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "LPC:No space for BDS B2A prms: GPS/GAL/GLO/BDS/QZSS SVs > PDSM_EXT_MEAS_NUM_SVS");
  }

  if (valid_sv_count < PDSM_EXT_MEAS_NUM_SVS)
  {
    valid_sv_count += tm_core_xlate_gnss_prm_to_pd_meas
       (
       PD_LOC_GNSS_SIGNAL_TYPE_NAVIC_L5,
       &dest_buf_ptr->ext_meas_report_type[valid_sv_count],
       source_buf_ptr->navic_meas_blk.z_Gnss,
       source_buf_ptr->navic_meas_blk.z_GnssHeader.u_NumSvs,
       PDSM_EXT_MEAS_NUM_SVS - valid_sv_count
       ); /* Number of entries available for NavIC is PDSM_EXT_MEAS_NUM_SVS -
             Number of entries used by GPS, GAL, GLO, BDS & QZSS*/
  }
  else
  {
    MSG(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "LPC:No space for NavIC L5 prms: GPS/GLO/BDS/QZSS SVs > PDSM_EXT_MEAS_NUM_SVS");
  }

  dest_buf_ptr->num_svs = valid_sv_count;
} /* tm_core_xlate_prm_to_ext_status() */

/*===========================================================================

FUNCTION lpc_convert_gnss_meas_blk_src_to_pd_type

DESCRIPTION 
       Function to convert a gnss_MeasBlkSrcEnumType to pdsm_LocGnssSignalEnumType.

DEPENDENCIES

INPUT PARAMS
        gnss_MeasBlkSrcEnumType The GNSS MeasBlkSrcEnum to be converted.

RETURN VALUE
        NONE

===========================================================================*/
static pdsm_LocGnssSignalEnumType lpc_convert_gnss_meas_blk_src_to_pd_type
(
   const gnss_MeasBlkSrcEnumType e_MeasBlkSrc
   )
{
  switch (e_MeasBlkSrc)
  {
    case GNSS_MEAS_BLK_SRC_GPS:
      return PD_LOC_GNSS_SIGNAL_TYPE_GPS_L1CA;
    case GNSS_MEAS_BLK_SRC_GLO:
      return PD_LOC_GNSS_SIGNAL_TYPE_GLONASS_G1;
    case GNSS_MEAS_BLK_SRC_BDS:
      return PD_LOC_GNSS_SIGNAL_TYPE_BEIDOU_B1_I;
    case GNSS_MEAS_BLK_SRC_GAL:
      return PD_LOC_GNSS_SIGNAL_TYPE_GALILEO_E1_C;
    case GNSS_MEAS_BLK_SRC_QZSS_SBAS:
      return PD_LOC_GNSS_SIGNAL_TYPE_QZSS_L1CA;
    case GNSS_MEAS_BLK_SRC_GPS_L2C:
      return PD_LOC_GNSS_SIGNAL_TYPE_GPS_L2C_L;
    case GNSS_MEAS_BLK_SRC_GPS_L5Q:
      return PD_LOC_GNSS_SIGNAL_TYPE_GPS_L5_Q;
    case GNSS_MEAS_BLK_SRC_GLO_G2:
      return PD_LOC_GNSS_SIGNAL_TYPE_GLONASS_G2;
    case GNSS_MEAS_BLK_SRC_BDS_B2:
      return PD_LOC_GNSS_SIGNAL_TYPE_BEIDOU_B2_I;
    case GNSS_MEAS_BLK_SRC_BDS_B2AQ:
      return PD_LOC_GNSS_SIGNAL_TYPE_BEIDOU_B2A_Q;
    case GNSS_MEAS_BLK_SRC_GAL_E5A:
      return PD_LOC_GNSS_SIGNAL_TYPE_GALILEO_E5A_Q;
    case GNSS_MEAS_BLK_SRC_QZSS_L2C:
      return PD_LOC_GNSS_SIGNAL_TYPE_QZSS_L2C_L;
    case GNSS_MEAS_BLK_SRC_QZSS_L5Q:
      return PD_LOC_GNSS_SIGNAL_TYPE_QZSS_L5_Q;
    default:
      TM_ERROR_1("Invalid MeasBlkSrc %d", e_MeasBlkSrc);
      return PD_LOC_GNSS_SIGNAL_TYPE_INVALID;
  }
}

/*===========================================================================

FUNCTION tm_core_xlate_prms_to_ext_raw_clock_meas

DESCRIPTION: Translates internal GNSS raw clock & measurement structure in a form suitable for transmission over QMI

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/
//#define   FEATURE_GNSS_RAW_MEAS_OUTPUT_DEBUG

static boolean tm_core_xlate_prms_to_ext_raw_clock_meas
(
   pdsm_pd_ext_gnss_meas_s_type       *dest_buf_ptr,
   const gnss_MeasBlkStructType  *source_buf_ptr,
   ldm_client_e_type e_ClientType
)
{

  uint8 u_NumSvs, index, u_GoodSVCount = 0;

  pdsm_LocSVMeasurementStructType *p_pdsmSVMeasDst;
  const gnss_MeasStructType     *p_MeasSrc;

  static tm_loc_processing_client_meas_counts z_ClntMeasCounts[LDM_CLIENT_TYPE_LAST + 1] = { 0 };
  boolean measRejected = FALSE;
  uint8 u_MaxPartCount = 0;
  uint16 w_EnabledConstell = tm_core_info.config_info.gnss_meas_rep_constell_config;
  gnss_MeasBlkSrcEnumType e_PrimaryConstellation = GNSS_MEAS_BLK_SRC_MAX;
  const gnss_IntraGnssSigTimeBias *p_IntraSystemTbSrc = NULL;

#ifdef FEATURE_GNSS_NON_STANDARD_SUPPORT
  /*Unpropagate the Meas before sending it to LBS*/
  //(void)gnss_UnrollMeasBlkProp(source_buf_ptr);
#endif

#ifdef FEATURE_GNSS_DTF
  /*Scenario cant be reproduced in case we have only one constellation in View*/
  TM_MED("TM_DTF_DEBUG: seqNo : %d   maxSeqNo : %d   b_dropMeasBlock : %d", (source_buf_ptr->z_GnssHeader.u_SeqNum), (source_buf_ptr->z_GnssHeader.u_MaxMessageNum), b_dropMeasBlock);
  if (b_dropMeasBlock && (source_buf_ptr->z_GnssHeader.u_MaxMessageNum >= u_prevMaxPartCount))
  {
    return TRUE;
  }
  else
  {
    b_dropMeasBlock = FALSE;
  }
#endif

#ifdef FEATURE_GNSS_RAW_MEAS_OUTPUT_DEBUG
  TM_MED_1("LPC:Entered tm_core_xlate_prms_to_ext_raw_clock_meas,op_mode %d", tm_core_info.session_info.op_mode);
#endif

  if ((dest_buf_ptr == NULL) ||
      (source_buf_ptr == NULL))
  {
    TM_HIGH_0("LPC:tm_core_xlate_prms_to_ext_raw_clock_meas received NULL pointer");
    return (FALSE);
  }

  if (e_ClientType > LDM_CLIENT_TYPE_LAST)
  {
    TM_ERROR_1("LPC:tm_core_xlate_prms_to_ext_raw_clock_meas received invalid ClientType %d", e_ClientType);
    return FALSE;
  }

  e_PrimaryConstellation = sm_util_get_master_constellation(source_buf_ptr->z_GnssHeader.e_MeasBlkSrc);

  /* If the operation mode is MSA don't send out GNSS measurement block. This is to protect from
       unpropagated measurement (received from ME) from going out. 
  */
  if (tm_core_info.session_info.op_mode == TM_SESS_OPERATION_MODE_MSA)
  {
    TM_MED_0("LPC:tm_core_xlate_prms_to_ext_raw_clock_meas:MSA mode, No GNSS measurement report");
    return (FALSE);
  }

  /* Start with all parameters NULLed out, especially validity flags */
  memset(dest_buf_ptr, 0, sizeof(*dest_buf_ptr));

  /* Evaluate constellations enabled for raw measurement reporting and determine max message part
      Note that this can be different from maxMessage number reported by SM, as enabled constellation
      for reporting can be different.
  */

  if (w_EnabledConstell & PDSM_GPS_MEAS_REPORT_ENABLED)
  {
    u_MaxPartCount += 3; // 1 sequence number each for L1/L2/L5
  }
  if (w_EnabledConstell & PDSM_GLO_MEAS_REPORT_ENABLED)
  {
    u_MaxPartCount += 2; // 1 sequence number each for G1/G2
  }
  if (w_EnabledConstell & PDSM_BDS_MEAS_REPORT_ENABLED)
  {
    u_MaxPartCount += 2; // 1 sequence number each for B1/B2
  }
  if (w_EnabledConstell & PDSM_GAL_MEAS_REPORT_ENABLED)
  {
    u_MaxPartCount += 2; // 1 sequence number each for E1/E5
  }
  if (w_EnabledConstell & PDSM_QZSS_MEAS_REPORT_ENABLED)
  {
    u_MaxPartCount += 3; // 1 sequence number each for L1/L2/L5
  }
  if (w_EnabledConstell & PDSM_NAVIC_MEAS_REPORT_ENABLED)
  {
    u_MaxPartCount += 1; // 1 sequence number  for NavIC L5
  }

  /* Populate message sequence number of maximum number of messages of upcoming transmission.
      Populate with minimum of max parts count determined by this function and max message number
      as received by this function. This is to protect from differences between enabled constellation to ME
      vs enabled constellation for measurement reporting.
  */
  dest_buf_ptr->u_maxMessageNum = MIN(u_MaxPartCount, source_buf_ptr->z_GnssHeader.u_MaxMessageNum);

  /* Will set sequence number after evaluating if constellation is enabled for output or not */

  /* Evaluate if present constellation is of interest for reporting */
  switch (e_PrimaryConstellation)
  {
    case GNSS_MEAS_BLK_SRC_GPS:
      if (!(w_EnabledConstell & PDSM_GPS_MEAS_REPORT_ENABLED))
      {
        measRejected = TRUE;
      }
      break;
    case GNSS_MEAS_BLK_SRC_GLO:
      if (!(w_EnabledConstell & PDSM_GLO_MEAS_REPORT_ENABLED))
      {
        measRejected = TRUE;
      }
      break;
    case GNSS_MEAS_BLK_SRC_BDS:
      if (!(w_EnabledConstell & PDSM_BDS_MEAS_REPORT_ENABLED))
      {
        measRejected = TRUE;
      }
      break;
    case GNSS_MEAS_BLK_SRC_QZSS_SBAS:
      if (!(w_EnabledConstell & PDSM_QZSS_MEAS_REPORT_ENABLED))
      {
        measRejected = TRUE;
      }
      break;
    case GNSS_MEAS_BLK_SRC_GAL:
      if (!(w_EnabledConstell & PDSM_GAL_MEAS_REPORT_ENABLED))
      {
        measRejected = TRUE;
      }
      break;

    case GNSS_MEAS_BLK_SRC_NAVIC:
      if (!(w_EnabledConstell & PDSM_NAVIC_MEAS_REPORT_ENABLED))
      {
        measRejected = TRUE;
      }
      break;

    default:
      MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "Invalid MeasBlkSrc %d", source_buf_ptr->z_GnssHeader.e_MeasBlkSrc);
      return (FALSE);
  }

  /*Keep Count of rejected meas Block and verify if Source is going to send More meas
  If all the Meas has been received, then make partcount and rejectcount as 0*/
  TM_LOW("Part count : %d Part count rejected : %d  Maximum part count : %d",
         (z_ClntMeasCounts[e_ClientType].u_partCount),
         (z_ClntMeasCounts[e_ClientType].u_partCountReject),
         (source_buf_ptr->z_GnssHeader.u_MaxMessageNum));
  if (((z_ClntMeasCounts[e_ClientType].u_partCount + z_ClntMeasCounts[e_ClientType].u_partCountReject) >= source_buf_ptr->z_GnssHeader.u_MaxMessageNum) ||
      /*if the meas block received is for same constellation as before reset the count*/
      (0 != (z_ClntMeasCounts[e_ClientType].q_maskMeasSrc & (1 << source_buf_ptr->z_GnssHeader.e_MeasBlkSrc))) ||
      /*if the prev FCount is greater than 2 sec then reset the count*/
      ((source_buf_ptr->z_GnssHeader.z_MeasClock.q_RefFCount - z_ClntMeasCounts[e_ClientType].q_prevRefFCount) >= TM_CORE_WAITIME_FOR_SEQUENCE_NUM_RESET)
      )
  {
    z_ClntMeasCounts[e_ClientType].u_partCount = 0;
    z_ClntMeasCounts[e_ClientType].u_partCountReject = 0;
    z_ClntMeasCounts[e_ClientType].q_maskMeasSrc = 0;
  }
#ifdef FEATURE_GNSS_DTF
  /* Trigger a DTF event when TM core is waiting for last meas block, 
     Set flag b_dropMeasBlock to drop all the meas block for which u_MaxMessageNum count is greater than the previous u_MaxMessageNum count,
     From DTF event handler send a meas block with u_MaxMessageNum count less than the previous count and unblock all the next meas */
  else if ((z_ClntMeasCounts[e_ClientType].u_partCount + z_ClntMeasCounts[e_ClientType].u_partCountReject + 1) == source_buf_ptr->z_GnssHeader.u_MaxMessageNum)
  {
    if (TRUE == tm_dtf_events_db.meas_started.status)
    {
      b_dropMeasBlock = TRUE;
      u_prevMaxPartCount = source_buf_ptr->z_GnssHeader.u_MaxMessageNum;
      tm_dtf_events_db.meas_started.status = FALSE;
      tm_qsh_report_event(SYS_MODEM_AS_ID_1, (tm_qsh_event_e)TM_DTF_EVENT_MEAS_STARTED);
    }
    else
    {
      MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_LOW, "TM_DTF_DEBUG: tm_dtf_events_db.meas_started.status not enabled   %d ", (tm_dtf_events_db.meas_started.status));
    }
   }
#endif

  /* For every enabled constellation, increment part count by one
     For Every rejected Constellation, Increment u_partCountReject by one */
  if (measRejected)
  {
    z_ClntMeasCounts[e_ClientType].u_partCountReject++;
    TM_ERROR("Xlate failed MeasRejected %d RejectCount %d. MeasBlkSrc %d", measRejected, z_ClntMeasCounts[e_ClientType].u_partCountReject,
             source_buf_ptr->z_GnssHeader.e_MeasBlkSrc);
    return (FALSE);
  }
  else
  {
    z_ClntMeasCounts[e_ClientType].u_partCount++;
  }

  dest_buf_ptr->u_seqNum = z_ClntMeasCounts[e_ClientType].u_partCount;
  /*Maintain a mask for constellations we received measurements*/
  z_ClntMeasCounts[e_ClientType].q_maskMeasSrc |= (1 << source_buf_ptr->z_GnssHeader.e_MeasBlkSrc);
  /*Maintain FCount for measurement received*/
  z_ClntMeasCounts[e_ClientType].q_prevRefFCount = source_buf_ptr->z_GnssHeader.z_MeasClock.q_RefFCount;

#ifdef FEATURE_GNSS_RAW_MEAS_OUTPUT_DEBUG

  TM_MED("LPC:RAW_CLK_MEAS: MeasBlk for constellation [%d], Seq#[%d] of MaxNum[%d]",
        source_buf_ptr->z_GnssHeader.e_MeasBlkSrc, source_buf_ptr->z_GnssHeader.u_SeqNum, source_buf_ptr->z_GnssHeader.u_MaxMessageNum);
#endif

  // Process leap second, Inter-system time bias information when 1st measurement block is received
  // Information is redundant across all measBlks

  if (dest_buf_ptr->u_seqNum == 1)
  {
    dest_buf_ptr->b_LeapSecValid = TRUE;
    dest_buf_ptr->z_LeapSec.u_leapSec = source_buf_ptr->z_GnssHeader.z_MeasClock.z_Time.z_GnssTimeBias.u_DeltaLeapSec;
    dest_buf_ptr->z_LeapSec.u_leapSecUnc = source_buf_ptr->z_GnssHeader.z_MeasClock.z_Time.z_GnssTimeBias.u_DeltaLeapSecUnc;


    /* Process Inter-system time report. Output this information, irrespective of current meas block source, but conditioned on if constellations of
       interest are enabled for reporting */

    /* GPS, GLO Inter-system bias */
    dest_buf_ptr->z_gpsGloInterSystemBias.u_validMask = 0;
    if ((w_EnabledConstell & PDSM_GPS_MEAS_REPORT_ENABLED) && (w_EnabledConstell & PDSM_GLO_MEAS_REPORT_ENABLED))
    {
      const gnss_InterGnssTimeBias *p_src_ptr = &source_buf_ptr->z_GnssHeader.z_MeasClock.z_Time.z_GnssTimeBias;
      pdsm_InterSystemBiasStructType *p_dst_ptr = &dest_buf_ptr->z_gpsGloInterSystemBias;

      p_dst_ptr->u_validMask = (PD_LOC_SYS_TIME_BIAS_VALID | PD_LOC_SYS_TIME_BIAS_UNC_VALID);
      p_dst_ptr->f_sys1Sys2TB    = p_src_ptr->z_GpsGlo.f_TBMs;
      p_dst_ptr->f_sys1Sys2TBUnc = p_src_ptr->z_GpsGlo.f_TBUncMs;
    }

    /* GPS, BDS Inter-system bias */
    dest_buf_ptr->z_gpsBdsInterSystemBias.u_validMask = 0;
    if ((w_EnabledConstell & PDSM_GPS_MEAS_REPORT_ENABLED) && (w_EnabledConstell  & PDSM_BDS_MEAS_REPORT_ENABLED))
    {
      const gnss_InterGnssTimeBias *p_src_ptr = &source_buf_ptr->z_GnssHeader.z_MeasClock.z_Time.z_GnssTimeBias;
      pdsm_InterSystemBiasStructType *p_dst_ptr = &dest_buf_ptr->z_gpsBdsInterSystemBias;

      p_dst_ptr->u_validMask = (PD_LOC_SYS_TIME_BIAS_VALID | PD_LOC_SYS_TIME_BIAS_UNC_VALID);
      p_dst_ptr->f_sys1Sys2TB = p_src_ptr->z_GpsBds.f_TBMs;
      p_dst_ptr->f_sys1Sys2TBUnc = p_src_ptr->z_GpsBds.f_TBUncMs;
    }

    /* BDS, GLO Inter-system bias */
    dest_buf_ptr->z_bdsGloInterSystemBias.u_validMask = 0;
    if ((w_EnabledConstell & PDSM_BDS_MEAS_REPORT_ENABLED) && (w_EnabledConstell &  PDSM_GLO_MEAS_REPORT_ENABLED))
    {
      const gnss_InterGnssTimeBias *p_src_ptr = &source_buf_ptr->z_GnssHeader.z_MeasClock.z_Time.z_GnssTimeBias;
      pdsm_InterSystemBiasStructType *p_dst_ptr = &dest_buf_ptr->z_bdsGloInterSystemBias;

      p_dst_ptr->u_validMask = (PD_LOC_SYS_TIME_BIAS_VALID | PD_LOC_SYS_TIME_BIAS_UNC_VALID);
      p_dst_ptr->f_sys1Sys2TB    = p_src_ptr->z_BdsGlo.f_TBMs;
      p_dst_ptr->f_sys1Sys2TBUnc = p_src_ptr->z_BdsGlo.f_TBUncMs;
    }

    /* GPS, GAL Inter-system bias */
    dest_buf_ptr->z_gpsGalInterSystemBias.u_validMask = 0;
    if ((w_EnabledConstell & PDSM_GPS_MEAS_REPORT_ENABLED) && (w_EnabledConstell & PDSM_GAL_MEAS_REPORT_ENABLED))
    {
      const gnss_InterGnssTimeBias *p_src_ptr = &source_buf_ptr->z_GnssHeader.z_MeasClock.z_Time.z_GnssTimeBias;
      pdsm_InterSystemBiasStructType *p_dst_ptr = &dest_buf_ptr->z_gpsGalInterSystemBias;

      p_dst_ptr->u_validMask = (PD_LOC_SYS_TIME_BIAS_VALID | PD_LOC_SYS_TIME_BIAS_UNC_VALID);
      p_dst_ptr->f_sys1Sys2TB = p_src_ptr->z_GpsGal.f_TBMs;
      p_dst_ptr->f_sys1Sys2TBUnc = p_src_ptr->z_GpsGal.f_TBUncMs;
    }

    /* GAL, GLO Inter-system bias */
    dest_buf_ptr->z_galGloInterSystemBias.u_validMask = 0;
    if ((w_EnabledConstell & PDSM_GPS_MEAS_REPORT_ENABLED) && (w_EnabledConstell &  PDSM_BDS_MEAS_REPORT_ENABLED))
    {
      const gnss_InterGnssTimeBias *p_src_ptr = &source_buf_ptr->z_GnssHeader.z_MeasClock.z_Time.z_GnssTimeBias;
      pdsm_InterSystemBiasStructType *p_dst_ptr = &dest_buf_ptr->z_galGloInterSystemBias;

      p_dst_ptr->u_validMask = (PD_LOC_SYS_TIME_BIAS_VALID | PD_LOC_SYS_TIME_BIAS_UNC_VALID);
      p_dst_ptr->f_sys1Sys2TB = p_src_ptr->z_GalGlo.f_TBMs;
      p_dst_ptr->f_sys1Sys2TBUnc = p_src_ptr->z_GalGlo.f_TBUncMs;
    }

    /* GAL, BDS Inter-system bias */
    dest_buf_ptr->z_galBdsInterSystemBias.u_validMask = 0;
    if ((w_EnabledConstell & PDSM_GPS_MEAS_REPORT_ENABLED) && (w_EnabledConstell & PDSM_BDS_MEAS_REPORT_ENABLED))
    {
      const gnss_InterGnssTimeBias *p_src_ptr = &source_buf_ptr->z_GnssHeader.z_MeasClock.z_Time.z_GnssTimeBias;
      pdsm_InterSystemBiasStructType *p_dst_ptr = &dest_buf_ptr->z_galBdsInterSystemBias;

      p_dst_ptr->u_validMask = (PD_LOC_SYS_TIME_BIAS_VALID | PD_LOC_SYS_TIME_BIAS_UNC_VALID);
      p_dst_ptr->f_sys1Sys2TB = p_src_ptr->z_GalBds.f_TBMs;
      p_dst_ptr->f_sys1Sys2TBUnc = p_src_ptr->z_GalBds.f_TBUncMs;
    }

    /*NavIC, GPS time bias*/
    dest_buf_ptr->z_gpsNavicInterSystemBias.u_validMask = 0;
    if ((w_EnabledConstell & PDSM_NAVIC_MEAS_REPORT_ENABLED) && (w_EnabledConstell & PDSM_GPS_MEAS_REPORT_ENABLED))
    {
      const gnss_InterGnssTimeBias *p_src_ptr = &source_buf_ptr->z_GnssHeader.z_MeasClock.z_Time.z_GnssTimeBias;
      pdsm_InterSystemBiasStructType *p_dst_ptr = &dest_buf_ptr->z_gpsNavicInterSystemBias;

      p_dst_ptr->u_validMask = (PD_LOC_SYS_TIME_BIAS_VALID | PD_LOC_SYS_TIME_BIAS_UNC_VALID);
      p_dst_ptr->f_sys1Sys2TB = p_src_ptr->z_GpsNavic.f_TBMs;
      p_dst_ptr->f_sys1Sys2TBUnc = p_src_ptr->z_GpsNavic.f_TBUncMs;
    }

    /*NavIC, GAL time bias*/
    dest_buf_ptr->z_galNavicInterSystemBias.u_validMask = 0;
    if ((w_EnabledConstell & PDSM_NAVIC_MEAS_REPORT_ENABLED) && (w_EnabledConstell & PDSM_GAL_MEAS_REPORT_ENABLED))
    {
      const gnss_InterGnssTimeBias *p_src_ptr = &source_buf_ptr->z_GnssHeader.z_MeasClock.z_Time.z_GnssTimeBias;
      pdsm_InterSystemBiasStructType *p_dst_ptr = &dest_buf_ptr->z_galNavicInterSystemBias;

      p_dst_ptr->u_validMask = (PD_LOC_SYS_TIME_BIAS_VALID | PD_LOC_SYS_TIME_BIAS_UNC_VALID);
      p_dst_ptr->f_sys1Sys2TB = p_src_ptr->z_GalNavic.f_TBMs;
      p_dst_ptr->f_sys1Sys2TBUnc = p_src_ptr->z_GalNavic.f_TBUncMs;
    }

    /*NavIC, GLO time bias*/
    dest_buf_ptr->z_gloNavicInterSystemBias.u_validMask = 0;
    if ((w_EnabledConstell & PDSM_NAVIC_MEAS_REPORT_ENABLED) && (w_EnabledConstell & PDSM_GLO_MEAS_REPORT_ENABLED))
    {
      const gnss_InterGnssTimeBias *p_src_ptr = &source_buf_ptr->z_GnssHeader.z_MeasClock.z_Time.z_GnssTimeBias;
      pdsm_InterSystemBiasStructType *p_dst_ptr = &dest_buf_ptr->z_gloNavicInterSystemBias;

      p_dst_ptr->u_validMask = (PD_LOC_SYS_TIME_BIAS_VALID | PD_LOC_SYS_TIME_BIAS_UNC_VALID);
      p_dst_ptr->f_sys1Sys2TB = p_src_ptr->z_GloNavic.f_TBMs;
      p_dst_ptr->f_sys1Sys2TBUnc = p_src_ptr->z_GloNavic.f_TBUncMs;
    }

    /*NavIC, BDS time bias*/
    dest_buf_ptr->z_bdsNavicInterSystemBias.u_validMask = 0;
    if ((w_EnabledConstell & PDSM_NAVIC_MEAS_REPORT_ENABLED) && (w_EnabledConstell & PDSM_BDS_MEAS_REPORT_ENABLED))
    {
      const gnss_InterGnssTimeBias *p_src_ptr = &source_buf_ptr->z_GnssHeader.z_MeasClock.z_Time.z_GnssTimeBias;
      pdsm_InterSystemBiasStructType *p_dst_ptr = &dest_buf_ptr->z_bdsNavicInterSystemBias;

      p_dst_ptr->u_validMask = (PD_LOC_SYS_TIME_BIAS_VALID | PD_LOC_SYS_TIME_BIAS_UNC_VALID);
      p_dst_ptr->f_sys1Sys2TB = p_src_ptr->z_BdsNavic.f_TBMs;
      p_dst_ptr->f_sys1Sys2TBUnc = p_src_ptr->z_BdsNavic.f_TBUncMs;
    }
  }
  /* Clock Frequency information. This will be sent, irrespective of constellation */
  dest_buf_ptr->z_clockFreq.f_clockDrift = source_buf_ptr->z_GnssHeader.z_MeasClock.z_Freq.f_ClkFreqBias;
  dest_buf_ptr->z_clockFreq.f_clockDriftUnc = source_buf_ptr->z_GnssHeader.z_MeasClock.z_Freq.f_ClkFreqUnc;

  /* Map source of frequency internal to external */
  switch (source_buf_ptr->z_GnssHeader.z_MeasClock.z_Freq.e_SourceOfFreq)
  {
    case C_FREQ_SRC_INVALID:
      dest_buf_ptr->z_clockFreq.e_sourceOfFreq = PD_LOC_FREQ_SOURCE_INVALID;
      break;
    case C_FREQ_SRC_EXTERNAL:
      dest_buf_ptr->z_clockFreq.e_sourceOfFreq = PD_LOC_FREQ_SOURCE_EXTERNAL;
      break;
    case C_FREQ_SRC_PE_CLK_REPORT:
      dest_buf_ptr->z_clockFreq.e_sourceOfFreq = PD_LOC_FREQ_SOURCE_PE_CLK_REPORT;
      break;
    default:
      dest_buf_ptr->z_clockFreq.e_sourceOfFreq = PD_LOC_FREQ_SOURCE_UNKNOWN;
      break;
  }

  /* Process constellation specific information */
  switch (e_PrimaryConstellation)
  {
    case GNSS_MEAS_BLK_SRC_GPS:
      dest_buf_ptr->z_GnssMeas.e_system = PDSM_CONSTELLATION_TYPE_GPS;

      /* Clock processing */
      dest_buf_ptr->z_GnssMeas.z_systemTime.w_systemWeek = source_buf_ptr->z_GnssHeader.z_MeasClock.z_Time.z_GpsTime.w_GpsWeek;
      dest_buf_ptr->z_GnssMeas.z_systemTime.q_systemMsec = source_buf_ptr->z_GnssHeader.z_MeasClock.z_Time.z_GpsTime.q_GpsMsec;
      dest_buf_ptr->z_GnssMeas.z_systemTime.f_systemClkTimeBias = source_buf_ptr->z_GnssHeader.z_MeasClock.z_Time.z_GpsTime.f_ClkTimeBias;
      dest_buf_ptr->z_GnssMeas.z_systemTime.f_systemClkTimeUncMs = source_buf_ptr->z_GnssHeader.z_MeasClock.z_Time.z_GpsTime.f_ClkTimeUncMs;
      dest_buf_ptr->z_GnssMeas.z_systemTime.e_SourceOfTime = source_buf_ptr->z_GnssHeader.z_MeasClock.z_Time.z_GpsTime.e_SourceOfTime;

      break;
    case GNSS_MEAS_BLK_SRC_GLO:
      dest_buf_ptr->z_GnssMeas.e_system = PDSM_CONSTELLATION_TYPE_GLONASS;

      /* Clock processing */
      /* Mark GPS week as unknow for GLONASS constellation */
      dest_buf_ptr->z_GnssMeas.z_systemTime.w_systemWeek = C_GPS_WEEK_UNKNOWN;
      dest_buf_ptr->z_GnssMeas.z_systemTime.u_gloFourYear = source_buf_ptr->z_GnssHeader.z_MeasClock.z_Time.z_GloTime.u_FourYear;
      dest_buf_ptr->z_GnssMeas.z_systemTime.w_gloDays = source_buf_ptr->z_GnssHeader.z_MeasClock.z_Time.z_GloTime.w_Days;
      dest_buf_ptr->z_GnssMeas.z_systemTime.q_systemMsec = source_buf_ptr->z_GnssHeader.z_MeasClock.z_Time.z_GloTime.q_GloMsec;
      dest_buf_ptr->z_GnssMeas.z_systemTime.f_systemClkTimeBias = source_buf_ptr->z_GnssHeader.z_MeasClock.z_Time.z_GloTime.f_ClkTimeBias;
      dest_buf_ptr->z_GnssMeas.z_systemTime.f_systemClkTimeUncMs = source_buf_ptr->z_GnssHeader.z_MeasClock.z_Time.z_GloTime.f_ClkTimeUncMs;
      dest_buf_ptr->z_GnssMeas.z_systemTime.e_SourceOfTime = source_buf_ptr->z_GnssHeader.z_MeasClock.z_Time.z_GloTime.e_SourceOfTime;
      break;

    case GNSS_MEAS_BLK_SRC_BDS:
      dest_buf_ptr->z_GnssMeas.e_system = PDSM_CONSTELLATION_TYPE_BDS;

      /* Clock processing */
      dest_buf_ptr->z_GnssMeas.z_systemTime.w_systemWeek = source_buf_ptr->z_GnssHeader.z_MeasClock.z_Time.z_BdsTime.w_BdsWeek;
      dest_buf_ptr->z_GnssMeas.z_systemTime.q_systemMsec = source_buf_ptr->z_GnssHeader.z_MeasClock.z_Time.z_BdsTime.q_BdsMsec;
      dest_buf_ptr->z_GnssMeas.z_systemTime.f_systemClkTimeBias = source_buf_ptr->z_GnssHeader.z_MeasClock.z_Time.z_BdsTime.f_ClkTimeBias;
      dest_buf_ptr->z_GnssMeas.z_systemTime.f_systemClkTimeUncMs = source_buf_ptr->z_GnssHeader.z_MeasClock.z_Time.z_BdsTime.f_ClkTimeUncMs;
      dest_buf_ptr->z_GnssMeas.z_systemTime.e_SourceOfTime = source_buf_ptr->z_GnssHeader.z_MeasClock.z_Time.z_BdsTime.e_SourceOfTime;
      break;

    case GNSS_MEAS_BLK_SRC_QZSS_SBAS:
      dest_buf_ptr->z_GnssMeas.e_system = PDSM_CONSTELLATION_TYPE_QZSS;
      /* Clock processing  */
      dest_buf_ptr->z_GnssMeas.z_systemTime.w_systemWeek = source_buf_ptr->z_GnssHeader.z_MeasClock.z_Time.z_GpsTime.w_GpsWeek;
      dest_buf_ptr->z_GnssMeas.z_systemTime.q_systemMsec = source_buf_ptr->z_GnssHeader.z_MeasClock.z_Time.z_GpsTime.q_GpsMsec;
      dest_buf_ptr->z_GnssMeas.z_systemTime.f_systemClkTimeBias = source_buf_ptr->z_GnssHeader.z_MeasClock.z_Time.z_GpsTime.f_ClkTimeBias;
      dest_buf_ptr->z_GnssMeas.z_systemTime.f_systemClkTimeUncMs = source_buf_ptr->z_GnssHeader.z_MeasClock.z_Time.z_GpsTime.f_ClkTimeUncMs;
      dest_buf_ptr->z_GnssMeas.z_systemTime.e_SourceOfTime = source_buf_ptr->z_GnssHeader.z_MeasClock.z_Time.z_GpsTime.e_SourceOfTime;
      break;

    case GNSS_MEAS_BLK_SRC_GAL:
      dest_buf_ptr->z_GnssMeas.e_system = PDSM_CONSTELLATION_TYPE_GALILEO;

      /* Clock processing */
      dest_buf_ptr->z_GnssMeas.z_systemTime.w_systemWeek = source_buf_ptr->z_GnssHeader.z_MeasClock.z_Time.z_GalTime.w_GalWeek;
      dest_buf_ptr->z_GnssMeas.z_systemTime.q_systemMsec = source_buf_ptr->z_GnssHeader.z_MeasClock.z_Time.z_GalTime.q_GalMsec;
      dest_buf_ptr->z_GnssMeas.z_systemTime.f_systemClkTimeBias = source_buf_ptr->z_GnssHeader.z_MeasClock.z_Time.z_GalTime.f_ClkTimeBias;
      dest_buf_ptr->z_GnssMeas.z_systemTime.f_systemClkTimeUncMs = source_buf_ptr->z_GnssHeader.z_MeasClock.z_Time.z_GalTime.f_ClkTimeUncMs;
      dest_buf_ptr->z_GnssMeas.z_systemTime.e_SourceOfTime = source_buf_ptr->z_GnssHeader.z_MeasClock.z_Time.z_GalTime.e_SourceOfTime;
      break;

    case GNSS_MEAS_BLK_SRC_NAVIC:
      dest_buf_ptr->z_GnssMeas.e_system = PDSM_CONSTELLATION_TYPE_NAVIC;

      /* Clock processing */
      dest_buf_ptr->z_GnssMeas.z_systemTime.w_systemWeek = source_buf_ptr->z_GnssHeader.z_MeasClock.z_Time.z_NavicTime.w_NavicWeek;
      dest_buf_ptr->z_GnssMeas.z_systemTime.q_systemMsec = source_buf_ptr->z_GnssHeader.z_MeasClock.z_Time.z_NavicTime.q_NavicMsec;
      dest_buf_ptr->z_GnssMeas.z_systemTime.f_systemClkTimeBias = source_buf_ptr->z_GnssHeader.z_MeasClock.z_Time.z_NavicTime.f_ClkTimeBias;
      dest_buf_ptr->z_GnssMeas.z_systemTime.f_systemClkTimeUncMs = source_buf_ptr->z_GnssHeader.z_MeasClock.z_Time.z_NavicTime.f_ClkTimeUncMs;
      dest_buf_ptr->z_GnssMeas.z_systemTime.e_SourceOfTime = source_buf_ptr->z_GnssHeader.z_MeasClock.z_Time.z_NavicTime.e_SourceOfTime;
      break;

    default:
      TM_ERROR_1("Invalid MeasBlkSrc %d", source_buf_ptr->z_GnssHeader.e_MeasBlkSrc);
      return (FALSE);
  }

  /* Process Extended Time Information */
  dest_buf_ptr->z_GnssMeas.b_isSystemTimeValid = TRUE;
  dest_buf_ptr->z_GnssMeas.z_systemTime.q_refFCount = source_buf_ptr->z_GnssHeader.z_MeasClock.q_RefFCount;
  dest_buf_ptr->z_GnssMeas.z_systemTime.b_SystemRtcValid = source_buf_ptr->z_GnssHeader.z_MeasClock.u_SystemRtcValid;
  dest_buf_ptr->z_GnssMeas.z_systemTime.t_systemRtcMs = source_buf_ptr->z_GnssHeader.z_MeasClock.t_SystemRtcMs;
  dest_buf_ptr->z_GnssMeas.z_systemTime.q_NumClockResets = source_buf_ptr->z_GnssHeader.z_MeasClock.q_NumClockResets;
  dest_buf_ptr->e_GnssSignalType = lpc_convert_gnss_meas_blk_src_to_pd_type(source_buf_ptr->z_GnssHeader.e_MeasBlkSrc);

#ifdef FEATURE_GNSS_RAW_MEAS_OUTPUT_DEBUG
  TM_HIGH_3("MultiBandSignal MGPType %d PDSMType System %d, Signal %d",
        source_buf_ptr->z_GnssHeader.e_MeasBlkSrc,
            dest_buf_ptr->z_GnssMeas.e_system,
        dest_buf_ptr->e_GnssSignalType);
#endif

  /* Jammer Indicator. Always valid */
  dest_buf_ptr->z_JammerIndicator.l_agcMetricDb = source_buf_ptr->z_GnssHeader.z_MeasBlkHwMetrics.l_AgcMetric;
  dest_buf_ptr->z_JammerIndicator.l_bpMetricDb = source_buf_ptr->z_GnssHeader.z_MeasBlkHwMetrics.l_BpJammerMetric;

  /* GLO RF Group delay and Unc handling. These fields are always valid.
  The RF group delay will be sent as part of MEAS BLK even when we have no GPS/GLO measurements.
  In this case the f_GrpDelayUncNs will be very large but f_GrpDelayNs will be "0" */

  dest_buf_ptr->z_GloRFGroupDelay.q_validMask |= PDSM_MEAS_RF_GROUP_DELAY_VALID;
  dest_buf_ptr->z_GloRFGroupDelay.d_RfGroupDelay =
     (double)(((source_buf_ptr->z_GnssHeader.z_MeasClock.z_Time.z_GnssTimeBias.z_GpsGlo.f_GrpDelayNs) * (NSECS_TO_SECS)) * LIGHT_SEC);

  dest_buf_ptr->z_GloRFGroupDelay.q_validMask |= PDSM_MEAS_RF_GROUP_DELAY_UNC_VALID;
  dest_buf_ptr->z_GloRFGroupDelay.d_RfGroupDelayUnc =
     (double)(((source_buf_ptr->z_GnssHeader.z_MeasClock.z_Time.z_GnssTimeBias.z_GpsGlo.f_GrpDelayUncNs) * (NSECS_TO_SECS)) * LIGHT_SEC);

  /*Cumulative XO Phase correction in nano-seconds. Always valid */
  dest_buf_ptr->d_CumulativeXoPhaseCorr = source_buf_ptr->z_GnssHeader.z_MeasBlkHwMetrics.d_CumulativeXoPhaseCorr;
  dest_buf_ptr->d_XoFreqCorr = source_buf_ptr->z_GnssHeader.z_MeasBlkHwMetrics.d_XoFreqCorr;

  /* Measurement processing */
  u_NumSvs = MIN(source_buf_ptr->z_GnssHeader.u_NumSvs, PDSM_EXT_GNSS_MEAS_NUM_SVS);

#ifdef FEATURE_GNSS_RAW_MEAS_OUTPUT_DEBUG
  {
    char buf[1024];

    for (index = 0; index < u_NumSvs; index++)
    {
      snprintf(buf, sizeof(buf), "SV_TIME_AF:%u,%lu,%.4f,%ld\n", source_buf_ptr->z_Gnss[index].w_Sv, source_buf_ptr->z_Gnss[index].z_UnfiltMeas.z_SvTime.q_Ms, source_buf_ptr->z_Gnss[index].z_UnfiltMeas.z_SvTime.f_SubMs, source_buf_ptr->z_Gnss[index].l_LatencyMs);
      MSG_SPRINTF_1(MSG_SSID_GPSSM, MSG_LEGACY_MED, "%s", buf);
    }

    snprintf(buf, sizeof(buf), "GPS_TIME_AF:%lu,%.4f\n", source_buf_ptr->z_GnssHeader.z_MeasClock.z_Time.z_GpsTime.q_GpsMsec, source_buf_ptr->z_GnssHeader.z_MeasClock.z_Time.z_GpsTime.f_ClkTimeBias);
    MSG_SPRINTF_1(MSG_SSID_GPSSM, MSG_LEGACY_MED, "%s", buf);
  }
#endif

  for (index = 0, u_GoodSVCount = 0; index < u_NumSvs; index++)
  {
    p_MeasSrc = &source_buf_ptr->z_Gnss[index];
    p_pdsmSVMeasDst = &dest_buf_ptr->z_GnssMeas.z_svMeasurement[u_GoodSVCount];

    /* To reduce data traffic and transfer, filter for good SV measurement for reporting */
    if ((GNSS_ID_OK(p_MeasSrc->w_Sv) || ((SLOTID_OK(p_MeasSrc->w_Sv) ||
                                          ((GLO_SLOTID_UNKNOWN == p_MeasSrc->w_Sv) && !GLO_SIGNED_FREQ_INVALID(p_MeasSrc->b_FreqNum)))
                                         && p_MeasSrc->w_Cno > 0))
        && (FALSE == gnss_SvIsBlacklisted(&tm_core_info.config_info.z_GnssPersistSV, p_MeasSrc->w_Sv))
        && (~p_MeasSrc->q_MeasStatus & MEAS_STATUS_ACQ_OR_PROBATION)
        && (~p_MeasSrc->q_MeasStatus & MEAS_STATUS_FROM_RNG_DIFF)
        && (~p_MeasSrc->q_MeasStatus & MEAS_STATUS_DONT_USE)
        && (~p_MeasSrc->q_MeasStatus & MEAS_STATUS_XCORR))
    {
      if (FALSE == tm_core_xlate_gnss_prms_to_pd_gnss_meas(p_MeasSrc, p_pdsmSVMeasDst, source_buf_ptr->z_GnssHeader.e_MeasBlkSrc))
      {

#ifdef FEATURE_GNSS_RAW_MEAS_OUTPUT_DEBUG
        TM_ERROR("FilterMBMeas: Xlate failed for Sys %d Seq#[%d] of MaxNum[%d]",
              source_buf_ptr->z_GnssHeader.e_MeasBlkSrc,
              source_buf_ptr->z_GnssHeader.u_SeqNum,
              source_buf_ptr->z_GnssHeader.u_MaxMessageNum);
#endif
        return (FALSE);
      }
      u_GoodSVCount++;
    }
#ifdef FEATURE_GNSS_RAW_MEAS_OUTPUT_DEBUG
    else
    {
      TM_HIGH_4("FilterMBMeas: MeasStatus 0x%X SvId %d Constellation %d Count %d", p_MeasSrc->q_MeasStatus,
            p_MeasSrc->w_Sv,
            source_buf_ptr->z_GnssHeader.e_MeasBlkSrc, u_GoodSVCount);
          }
#endif
  }

  dest_buf_ptr->z_GnssMeas.u_NumSvs = u_GoodSVCount;

  //Intra-GNSS Time Bias information is redundant across all measBlks
  //To reduce traffic over QMI, only send it with the 1st measurement
  if (dest_buf_ptr->u_seqNum == 1)
  {
    /* Intra-System Time Bias */
    p_IntraSystemTbSrc = &source_buf_ptr->z_GnssHeader.z_MeasClock.z_Time.z_IntraGnssSigTb;
    if (w_EnabledConstell & PDSM_GPS_MEAS_REPORT_ENABLED)
	   {
      dest_buf_ptr->z_GpsL1L5TimeBias.u_validMask = (PD_LOC_SYS_TIME_BIAS_VALID | PD_LOC_SYS_TIME_BIAS_UNC_VALID);
      dest_buf_ptr->z_GpsL1L5TimeBias.f_sys1Sys2TB = p_IntraSystemTbSrc->z_GpsL1caL5.f_TBMs;
      dest_buf_ptr->z_GpsL1L5TimeBias.f_sys1Sys2TBUnc = p_IntraSystemTbSrc->z_GpsL1caL5.f_TBUncMs;
      dest_buf_ptr->z_GpsL1L2cTimeBias.u_validMask = 0; // Not supported
       }
    if (w_EnabledConstell & PDSM_GLO_MEAS_REPORT_ENABLED)
	   {
      dest_buf_ptr->z_GloG1G2TimeBias.u_validMask = 0; // Not supported
    }
    if (w_EnabledConstell & PDSM_BDS_MEAS_REPORT_ENABLED)
	    {
      dest_buf_ptr->z_BdsB1iB2aTimeBias.u_validMask      = (PD_LOC_SYS_TIME_BIAS_VALID | PD_LOC_SYS_TIME_BIAS_UNC_VALID);
      dest_buf_ptr->z_BdsB1iB2aTimeBias.f_sys1Sys2TB     = p_IntraSystemTbSrc->z_BdsB1iB2a.f_TBMs;
      dest_buf_ptr->z_BdsB1iB2aTimeBias.f_sys1Sys2TBUnc  = p_IntraSystemTbSrc->z_BdsB1iB2a.f_TBUncMs;
      dest_buf_ptr->z_BdsB1iB1cTimeBias.u_validMask = 0; // Not supported
	    }
    if (w_EnabledConstell & PDSM_GAL_MEAS_REPORT_ENABLED)
	    {
      dest_buf_ptr->z_GalE1E5aTimeBias.u_validMask = (PD_LOC_SYS_TIME_BIAS_VALID | PD_LOC_SYS_TIME_BIAS_UNC_VALID);
      dest_buf_ptr->z_GalE1E5aTimeBias.f_sys1Sys2TB = p_IntraSystemTbSrc->z_GalE1E5a.f_TBMs;
      dest_buf_ptr->z_GalE1E5aTimeBias.f_sys1Sys2TBUnc = p_IntraSystemTbSrc->z_GalE1E5a.f_TBUncMs;
      dest_buf_ptr->z_GalE1E5bTimeBias.u_validMask = 0; // Not supported
	  }
    }

  //GNSS Measurement Code Type
  dest_buf_ptr->e_GnssMeasCodeType = source_buf_ptr->z_GnssHeader.e_GnssMeasCodeType;

  /* NHz Indication*/
  if (source_buf_ptr->z_GnssHeader.e_GnssMeasRptreason == GNSS_MEAS_RPT_PERIODIC_NHZ_SM)
  {
    dest_buf_ptr->u_NhzMeasurement = 1;
  }

  TM_MED_5("RAW_CLK_MEAS: MeasBlk for constellation [%d], Seq#[%d] of MaxNum[%d], NumSvs[%d], FC[%d]",
           source_buf_ptr->z_GnssHeader.e_MeasBlkSrc, dest_buf_ptr->u_seqNum, dest_buf_ptr->u_maxMessageNum,
           dest_buf_ptr->z_GnssMeas.u_NumSvs, source_buf_ptr->z_GnssHeader.z_MeasClock.q_RefFCount);

  return (TRUE);
}

/*===========================================================================

FUNCTION lpc_handle_final_fix_report

DESCRIPTION: Function to handle the final fix report for a session.
             In non-concurrent mode this function gets called directly from the
             callback ; however in case of concurrent mode and E911 client 
             lpc_handle_fix_report_secondary will be called first.

DEPENDENCIES

INPUT PARAMS:
    pz_final_fixreport   Pointer to final fix report.
    e_ldm_client_id      LDM client ID.
RETURN VALUE
    NONE

SIDE EFFECTS

===========================================================================*/
static void lpc_handle_final_fix_report(sm_FixInfoStructType *pz_final_fixreport,
                                        ldm_client_id_e_type e_ldm_client_id)
{
  prtl_sess_info_e_type          sess_info_type = PRTL_SESS_INFO_NONE;
  prtl_sess_info_param_u_type    *sess_info_param_ptr = NULL;
  tm_loc_processing_client_info  *pz_loc_processing_client = lpc_get_loc_processing_client(e_ldm_client_id);
  tm_sess_handle_type            sess_handle;
  int                            prtlTableIndex;
  sm_gm_pos_fix_info             fix_info;
  boolean                        retVal = FALSE;
  tm_cm_phone_state_info_s_type  phone_state_info;

  MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LPC: Handling final fix report for client %d", e_ldm_client_id);
  if (NULL == pz_final_fixreport)
  {
    MSG(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "NULL Fix report");
    return;
  }

  if (NULL == pz_loc_processing_client)
  {
    MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "LPC:LDM client %d not enabled in LPC", e_ldm_client_id);
    return;
  }

  sess_info_param_ptr = (prtl_sess_info_param_u_type *)os_MemAlloc(sizeof(prtl_sess_info_param_u_type), OS_MEM_SCOPE_TASK);

  if (sess_info_param_ptr == NULL)
  {
    MSG(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "LDM:Failed to get buffer for sess_info_param_ptr");
    return;
  }

  prtlTableIndex = tm_core_get_prtl_idx_by_type(pz_loc_processing_client->prtl_type);
  sess_handle = pz_loc_processing_client->sessionHandle;

  /*Get Phone state*/
  memset(&phone_state_info, 0, sizeof(tm_cm_phone_state_info_s_type));
  tm_cm_iface_get_phone_state(&phone_state_info);

  if ((e_ldm_client_id >= LDM_CLIENT_ID_FIRST) &&
      (e_ldm_client_id <= LDM_CLIENT_ID_LAST))
  {
    MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LPC: Handling final fix; serving LDM client id %d valid", e_ldm_client_id);

    sess_info_type = PRTL_SESS_INFO_LR;

    if ((prtlTableIndex > -1) && (prtlTableIndex < TM_PRTL_NUM))
    {
      /* Set reason to general error for now, if return value */
      if (tm_core_info.prtl_func_cb_table[prtlTableIndex].sess_info_fp)
      {
        tm_core_trigger_fix_related_NMEA(pz_final_fixreport, FALSE);

        MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LPC: Handling final fix;generat ext pd_data");
        /* Generate extended pd data */
        tm_core_generate_ext_pd_data(pz_final_fixreport);

        memscpy((void *)&(sess_info_param_ptr->lr_data), sizeof(sess_info_param_ptr->lr_data),
                (void *)pz_final_fixreport, sizeof(*pz_final_fixreport));

        if (LDM_CLIENT_TYPE_MAIN == z_serving_client_data.e_client_type)
        {
          tm_core_info.session_info.op_requested &= ~TM_OP_REQ_LR;
        }

        MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LPC: Handling final fix;send info to protocol");
        retVal = tm_core_info.prtl_func_cb_table[prtlTableIndex].sess_info_fp(sess_handle, sess_info_type, sess_info_param_ptr);

        /*when the first final fix is received mark the flag to true. This is for the use of NV74210 when NV74191 is set to 1*/
        if ((tm_core_info.config_info.u_disable_Dynamic_HEPE_usage & TM_DYN_HEPE_DISABLED_FOR_BETTER_TTFF) &&
            (LDM_CLIENT_ID_APPTRK == e_ldm_client_id) && (TRUE == retVal))
        {
          lp_clients[e_ldm_client_id].v_isFinalFix = TRUE;
        }

        if (retVal == FALSE)
        {
          MSG(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "LPC:Protocol rejects LR");
          if (TM_CM_E911_STATE_CONCURRENT != phone_state_info.e911_state)
          {
            tm_core_abort_recover(TRUE, TM_STOP_REASON_GENERAL_ERROR); /*lint !e506 !e730 */
          }
          else if ((TM_CM_E911_STATE_CONCURRENT == phone_state_info.e911_state) &&
                   (LDM_CLIENT_TYPE_SECONDARY == pz_loc_processing_client->e_clientId_type))
          {
            MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "LPC:In concurrent mode send error for secondary client %d", pz_loc_processing_client->e_clientId);
            tm_lpc_report_error_secondary(TM_STOP_REASON_GENERAL_ERROR, pz_loc_processing_client->e_clientId);
          }
        }
      }
      else
      {
        MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "LPC:No protocol callback installed for %d", prtlTableIndex, 0, 0);
      }
    }
    else
    {
      if ((TM_CM_E911_STATE_CONCURRENT != phone_state_info.e911_state) &&
          (tm_core_info.session_info.active_sess_handle == TM_SESS_HANDLE_E911))
      {
        /* E911 is a dummy protocol, no protocol call backs are needed. Just clean up the session*/
        MSG(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "LPC:E911 ended without network request. Cleanup session");
        if (LPC_OP_STATUS_REQUEST_SUCCESS == tm_lpc_disable_client(LDM_CLIENT_ID_E911))
        {
          MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LPC:E911 client (early rcvr)cleanup: client %d disabled", LDM_CLIENT_ID_E911);
        }
        else
        {
          MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "LPC: E911 client (early rcvr)cleanup: client %d failed to disabled", LDM_CLIENT_ID_E911);
        }

        tm_core_abort_recover(TRUE, TM_STOP_REASON_COMPLETED); /*lint !e506 !e730 */
      }
      else
      {
        MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "LPC:Invalid prtl index 0X%x;can not report Final Position",
              prtlTableIndex);
      }
    }

    if (!((phone_state_info.e911_state == TM_CM_E911_STATE_CONCURRENT) &&
          (e_ldm_client_id == LDM_CLIENT_ID_E911)))
    {
      fix_info.fix_type = GEOFENCE_POS_FIX_TYPE_FINAL;
      fix_info.y_pos_fix_ext_info.z_gnss_fix_ext_info.b_is_nw_asst_fix = FALSE;
      fix_info.fix_src = GEOFENCE_POS_FIX_SRC_GNSS;
      fix_info.pos_fix = *(&(sess_info_param_ptr->lr_data));

      /*Send the LR to Geofence module*/
      (void)gm_position_update(&fix_info);

      (void)techSel_ReportPosFix(&(sess_info_param_ptr->lr_data));
    }

    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "LPC:Timesource Final Fix: 0x%x Tunc(ns): %d Wk: %d",
          (uint8)(sess_info_param_ptr->lr_data.z_NavPos.u_InitGpsTimeSrc),
          (uint32)(sess_info_param_ptr->lr_data.z_NavPos.f_ClockBiasUncMs * 1000),
          (sess_info_param_ptr->lr_data.z_NavPos.w_GpsWeek));

  }
  else
  {
    MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "LPC: Invalid LDM client id %d",
          e_ldm_client_id);
  }

  (void)os_MemFree((void **)&sess_info_param_ptr);
  return;
}

/*===========================================================================

FUNCTION lpc_handle_intermediate_fix_report

DESCRIPTION: Function to handle the intermediate fix report for a session.
             In non-concurrent mode this function gets called directly from the
             callback ; however in case of concurrent mode and E911 client 
             lpc_handle_fix_report_secondary will be called first.
             
DEPENDENCIES

INPUT PARAMS
    pz_im_fixreport Intermediate fix report

RETURN VALUE
    NONE

SIDE EFFECTS
===========================================================================*/
static void lpc_handle_intermediate_fix_report(tm_fix_report_s_type *pz_im_fixreport)
{
  boolean bBestAvailPos              = FALSE;
  sm_FixInfoStructType          *p_zFix       = NULL;
  prtl_sess_info_e_type         sess_info_type = PRTL_SESS_INFO_NONE;
  sm_gm_pos_fix_info            fix_info;
  sm_FixInfoStructType          *p_zUTCFix = NULL;
  tm_cm_phone_state_info_s_type phone_state_info;
  prtl_sess_info_e_type         session_info_type = PRTL_SESS_INFO_NONE;
  pdsm_client_id_type           pd_clientId;

  MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LPC:Handling intermediate fix report");
  if (pz_im_fixreport == NULL)
  {
    MSG(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "LPC:NULL Fix report");
    return;
  }

  if (FALSE == lp_clients[z_serving_client_data.serving_client_id].is_enabled)
  {
    MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "LPC:LDM client %d not enabled", z_serving_client_data.serving_client_id);
    return;
  }

  /*Make a copy of pd client ID*/
  pd_clientId = lp_clients[z_serving_client_data.serving_client_id].pd_clientId;

  /*Get Phone state*/
  memset(&phone_state_info, 0, sizeof(tm_cm_phone_state_info_s_type));
  tm_cm_iface_get_phone_state(&phone_state_info);

  if ((z_serving_client_data.serving_client_id >= LDM_CLIENT_ID_FIRST) &&
      (z_serving_client_data.serving_client_id <= LDM_CLIENT_ID_LAST))
  {
    MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LPC:Handling intermediate fix report; LDM client id %d valid", z_serving_client_data.serving_client_id);
    if (NULL == z_serving_client_data.pz_pd_ext_status_info)
    {
      MSG(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "LPC:NULL pdsm_ext_status_info_s_type in lpc_handle_intermediate_fix_report");

      if (TM_CM_E911_STATE_CONCURRENT != phone_state_info.e911_state)
      {
        tm_core_abort_recover(TRUE, TM_STOP_REASON_GENERAL_ERROR); /*lint !e506 !e730 */
      }
      else if ((TM_CM_E911_STATE_CONCURRENT == phone_state_info.e911_state) &&
               (LDM_CLIENT_TYPE_SECONDARY == z_serving_client_data.e_client_type))
      {
        MSG(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "LPC:In concurrent mode send error for secondary client");
        tm_lpc_report_error_secondary(TM_STOP_REASON_GENERAL_ERROR, z_serving_client_data.serving_client_id);
      }
      return;
    }


    p_zFix = &(pz_im_fixreport->z_GpsFixRpt);
    sess_info_type = PRTL_SESS_INFO_INTERM_LR;

    if (p_zFix->z_NavPos.z_SmPosFlags.b_IsPosBestAvail)
    {
      bBestAvailPos = TRUE; /* original NO_FIX report: CellDB fix, WiFi fix etc non-GNSS fix */
    }

    if (p_zFix->z_NavPos.w_GpsWeek != C_GPS_WEEK_UNKNOWN)
    {
      /* Acco�rding to PE, if GPSWeek is valid, UTC time is guaranteed to be valid, 
        can output UTC time in NMEA, otherwise empty UTC */
      p_zUTCFix = p_zFix;
    }

    /*Do we need this?*/
    memset(&(z_serving_client_data.pz_pd_ext_status_info->ext_status_info.ext_pos_report_type), 0, sizeof(pdsm_pd_ext_pos_data_s_type));

    if (tm_core_info.config_info.nmea_config_type.pd_nmea_reporting_type == PDSM_PA_NMEA_REPORT_AT_1HZ_INTERVAL)
    {
      /* Generate NULL NMEA strings for intermediate fixes atleast for now */
      /* Send this as a NO-FIX to NMEA so that only UTC filed is populated in GGA */
      tm_core_trigger_fix_related_NMEA(p_zUTCFix, TRUE);
    }

    /*Update fix time remaining only for main client running on LM */

    if (LDM_CLIENT_TYPE_MAIN == z_serving_client_data.e_client_type)
    {
      tm_core_info.session_info.q_FixTimeRemainingMSec = pz_im_fixreport->q_FixTimeRemainingMSec;
    }

    /** not all procotol submodules consumes INTERM_LR: UMTS does */
    if ((z_serving_client_data.prtlTableIndex > -1) && (z_serving_client_data.prtlTableIndex < TM_PRTL_NUM))
    {
      //prtl_sess_info_e_type session_info_type;

      memscpy((void *)&(z_serving_client_data.sess_info_param_ptr->lr_data), sizeof(z_serving_client_data.sess_info_param_ptr->lr_data),
              (void *)p_zFix, sizeof(*p_zFix));

      /* Check for E911 and LTE status. If so, set the session type as interim */
      if (TRUE == tm_E911LTEOn())
      {
        session_info_type = PRTL_SESS_INFO_INTERM_LR;
      }
      else
      {
        /* for BestAvailPos (originally NO_FIX), let AGPS handle it like NO_FIX since it's not GNSS fix */
        session_info_type = (bBestAvailPos ? PRTL_SESS_INFO_NOFIX : PRTL_SESS_INFO_INTERM_LR);
      }

      MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LPC:Handling intermediate fix report;send intermediate LR to protocol");
      (void)tm_core_info.prtl_func_cb_table[z_serving_client_data.prtlTableIndex].sess_info_fp(
         z_serving_client_data.serving_sess_handle,
         session_info_type,
         z_serving_client_data.sess_info_param_ptr);
    }
    else
    {
      MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "LPC:Invalid prtl index 0X%x, can not report TM_INFO_INTERMEDIATE_FIX_REPORT ",
            z_serving_client_data.prtlTableIndex);
    }

    /* Send extra pdapi event for intermediate fix report */
    z_serving_client_data.pz_pd_ext_status_info->client_id       = pd_clientId;
    z_serving_client_data.pz_pd_ext_status_info->ext_status_type = PDSM_EXT_STATUS_POS_REPORT;
    MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LPC:Handling intermediate fix report;generate PDSM_EXT_STATUS_POS_REPORT;bes pos %d", bBestAvailPos);
    MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LPC:Handling intermediate fix report;PDSM clien id %d", pd_clientId);

    if (FALSE == bBestAvailPos)
    {
      tm_core_xlate_pos_to_ext_status(&(z_serving_client_data.pz_pd_ext_status_info->ext_status_info.ext_pos_report_type),
                                      p_zFix,
                                      pz_im_fixreport->q_FixTimeRemainingMSec);
      tm_pdapi_ext_status_event_callback(PDSM_EXT_STATUS_POS_REPORT,
                                         z_serving_client_data.pz_pd_ext_status_info);
    }
    else
    {
      tm_core_xlate_best_avail_pos_to_ext_status(&(z_serving_client_data.pz_pd_ext_status_info->ext_status_info.ext_pos_report_type),
                                                 p_zFix,
                                                 pz_im_fixreport->q_FixTimeRemainingMSec);

      tm_pdapi_ext_status_event_callback(PDSM_EXT_STATUS_POS_REPORT,
                                         z_serving_client_data.pz_pd_ext_status_info);
    }

    if (!((phone_state_info.e911_state == TM_CM_E911_STATE_CONCURRENT) &&
          (z_serving_client_data.serving_client_id == LDM_CLIENT_ID_E911)))
    {
      (void)techSel_ReportPosFix(&(z_serving_client_data.sess_info_param_ptr->lr_data));

      if (bBestAvailPos == FALSE)
      {
        /* Send the LR to Geofence module */
        fix_info.fix_type = GEOFENCE_POS_FIX_TYPE_INTERMEDIATE;
        fix_info.y_pos_fix_ext_info.z_gnss_fix_ext_info.b_is_nw_asst_fix = FALSE;
        fix_info.fix_src = GEOFENCE_POS_FIX_SRC_GNSS;
        fix_info.pos_fix = *(&(z_serving_client_data.sess_info_param_ptr->lr_data));
        (void)gm_position_update(&fix_info);
      }
    }

    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "LPC:Timesource Int Fix: 0x%x Tunc(ns): %d Wk: %d",
          (uint8)p_zFix->z_NavPos.u_InitGpsTimeSrc,
          (uint32)(p_zFix->z_NavPos.f_ClockBiasUncMs * 1000),
          p_zFix->z_NavPos.w_GpsWeek);
  }
  else
  {
    MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "LPC: client %d not enabled for fix processing.",
          z_serving_client_data.serving_client_id);
  }
  return;
}


/*===========================================================================

FUNCTION lpc_handle_ppm_report

DESCRIPTION 
       This is the callback handler registered with LDM. 

DEPENDENCIES

INPUT PARAMS
      pz_ppm_rpt_from_ldm PPM report from LDM

RETURN VALUE


SIDE EFFECTS
z_serving_client_data

===========================================================================*/
static void lpc_handle_ppm_report(tm_ppm_report_s_type *pz_ppm_rpt_from_ldm)
{
  tm_ppm_report_s_type *pz_ppm_rpt = pz_ppm_rpt_from_ldm;
  tm_cm_phone_state_info_s_type phone_state_info;
  boolean retVal = FALSE;

  MSG(MSG_SSID_GPSSM, MSG_LEGACY_LOW, "LPC:Handle PPM Rpt");

  memset(&phone_state_info, 0, sizeof(tm_cm_phone_state_info_s_type));
  tm_cm_iface_get_phone_state(&phone_state_info);

  if (TM_CM_E911_STATE_CONCURRENT != phone_state_info.e911_state)
  {
    if ((z_serving_client_data.prtlTableIndex > -1) && (z_serving_client_data.prtlTableIndex < TM_PRTL_NUM))
    {
      MSG(MSG_SSID_GPSSM, MSG_LEGACY_LOW, "LPC:Despatch PPM Rpt");

      /* Set reason to general error for now, if return value */
      retVal = lpc_dispatch_ppm_rpt_to_prtl(
         tm_core_info.prtl_func_cb_table[z_serving_client_data.prtlTableIndex].sess_info_fp,
         z_serving_client_data.sess_info_param_ptr,
         &(pz_ppm_rpt->z_PpmRpt),
         z_serving_client_data.serving_sess_handle);

      if (retVal == FALSE)
      {
        tm_core_abort_recover(TRUE, TM_STOP_REASON_GENERAL_ERROR); /*lint !e506 !e730 */
      }
    }
    else
    {
      MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "LPC:Invalid prtl index 0X%x, can not report PPM", z_serving_client_data.prtlTableIndex);
    }

#ifdef FEATURE_GNSS_PERIODIC_PPM
    #error code not present
#endif
  }
  else
  {
    MSG(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "LPC:Concurrency not supported for 1x mode");
  }
}


/*===========================================================================

FUNCTION lpc_handle_late_prm_report

DESCRIPTION 
       Function to handle the late PRM report for a session.
       In non-concurrent mode this function gets called directly from the
       callback ; however in case of concurrent mode and E911 client 
       lpc_handle_prm_report_secondary will be called first.

DEPENDENCIES

INPUT PARAMS
    p_MeasRptSrc_from_ldm  GNSS Measurement Report structure

RETURN VALUE
    NONE

SIDE EFFECTS
===========================================================================*/
static void lpc_handle_late_prm_report(sm_GpsMeasRptStructType *p_MeasRptSrc_from_ldm)
{
  sm_GpsMeasRptStructType       *p_MeasRptSrc = NULL;
  gnss_MeasBlkStructType        *p_MeasBlkSrc = NULL;
  tm_cm_phone_state_info_s_type  phone_state_info;
  boolean                        retVal = FALSE;
  pdsm_client_id_type            pd_clientId;

  if (NULL == p_MeasRptSrc_from_ldm)
  {
    TM_ERROR_0("lpc_handle_prm_report() MeasReport NULL");
    return;
  }
  MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LPC:Handle late PRM Rpt");

  memset(&phone_state_info, 0, sizeof(tm_cm_phone_state_info_s_type));
  tm_cm_iface_get_phone_state(&phone_state_info);

  p_MeasRptSrc = p_MeasRptSrc_from_ldm;
  p_MeasBlkSrc = &(p_MeasRptSrc_from_ldm->z_MeasBlk);

  if (FALSE == (lp_clients[z_serving_client_data.serving_client_id].is_enabled))
  {
    MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "LPC:LDM client %d not enabled", z_serving_client_data.serving_client_id);
    return;
  }

  /*Make a copy of pd client ID*/
  pd_clientId = (lp_clients[z_serving_client_data.serving_client_id].pd_clientId);

  /* Set reason to general error for now, if return value */
  if (tm_core_info.prtl_func_cb_table[z_serving_client_data.prtlTableIndex].sess_info_fp)
  {
    sm_GnssMeas_memscpy((void *)&(z_serving_client_data.sess_info_param_ptr->prm_data),
                        sizeof(z_serving_client_data.sess_info_param_ptr->prm_data), p_MeasRptSrc);

    if (TM_CM_E911_STATE_CONCURRENT != phone_state_info.e911_state)
    {
      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_LOW, "LPC:Check for FTEST Sess: %d, Prot: %d, Meas Sys: %d (GPS/GLO/BDS)", tm_core_info.session_info.active_sess_handle,
            tm_core_info.session_info.from_protocol, p_MeasBlkSrc->z_GnssHeader.e_MeasBlkSrc);
      if ((tm_core_info.session_info.active_sess_handle == TM_SESS_HANDLE_FTEST) &&
          (tm_core_info.session_info.from_protocol == TM_PRTL_TYPE_DIAG))
      {
        tm_core_info.session_info.op_requested &= ~TM_OP_REQ_PRM;
      }
    }

    MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LPC:Handle late PRM Rpt; copy meas rpt to tm_core sturct");
    switch (p_MeasBlkSrc->z_GnssHeader.e_MeasBlkSrc)
    {
      case GNSS_MEAS_BLK_SRC_GPS:
        sm_GnssMeas_memscpy((void *)&(tm_core_info.gps_meas_blk), sizeof(tm_core_info.gps_meas_blk), p_MeasRptSrc);
        break;

      case GNSS_MEAS_BLK_SRC_GPS_L2C:
        sm_GnssMeas_memscpy((void *)&(tm_core_info.gps_l2c_meas_blk), sizeof(tm_core_info.gps_l2c_meas_blk), p_MeasRptSrc);
        break;

      case GNSS_MEAS_BLK_SRC_GPS_L5Q:
        sm_GnssMeas_memscpy((void *)&(tm_core_info.gps_l5q_meas_blk), sizeof(tm_core_info.gps_l5q_meas_blk), p_MeasRptSrc);
        break;

      case GNSS_MEAS_BLK_SRC_GLO:
        sm_GnssMeas_memscpy((void *)&(tm_core_info.glo_meas_blk), sizeof(tm_core_info.glo_meas_blk), p_MeasRptSrc);
        break;

      case GNSS_MEAS_BLK_SRC_GLO_G2:
        sm_GnssMeas_memscpy((void *)&(tm_core_info.glo_g2_meas_blk), sizeof(tm_core_info.glo_g2_meas_blk), p_MeasRptSrc);
        break;

      case GNSS_MEAS_BLK_SRC_BDS:
        sm_GnssMeas_memscpy((void *)&(tm_core_info.bds_meas_blk), sizeof(tm_core_info.bds_meas_blk), p_MeasRptSrc);
        break;

      case GNSS_MEAS_BLK_SRC_BDS_B2:
        sm_GnssMeas_memscpy((void *)&(tm_core_info.bds_b2_meas_blk), sizeof(tm_core_info.bds_b2_meas_blk), p_MeasRptSrc);
        break;

      case GNSS_MEAS_BLK_SRC_QZSS_SBAS:
        sm_GnssMeas_memscpy((void *)&(tm_core_info.qzss_meas_blk), sizeof(tm_core_info.qzss_meas_blk), p_MeasRptSrc);
        break;

      case GNSS_MEAS_BLK_SRC_QZSS_L2C:
        sm_GnssMeas_memscpy((void *)&(tm_core_info.qzss_l2c_meas_blk), sizeof(tm_core_info.qzss_l2c_meas_blk), p_MeasRptSrc);
        break;

      case GNSS_MEAS_BLK_SRC_QZSS_L5Q:
        sm_GnssMeas_memscpy((void *)&(tm_core_info.qzss_l5q_meas_blk), sizeof(tm_core_info.qzss_l5q_meas_blk), p_MeasRptSrc);
        break;

      case GNSS_MEAS_BLK_SRC_GAL:
        sm_GnssMeas_memscpy((void *)&(tm_core_info.gal_meas_blk), sizeof(tm_core_info.gal_meas_blk), p_MeasRptSrc);
        break;

      case GNSS_MEAS_BLK_SRC_NAVIC:
        sm_GnssMeas_memscpy((void *)&(tm_core_info.navic_meas_blk), sizeof(tm_core_info.navic_meas_blk), p_MeasRptSrc);
        break;

      case GNSS_MEAS_BLK_SRC_GAL_E5A:
        sm_GnssMeas_memscpy((void *)&(tm_core_info.gal_e5a_meas_blk), sizeof(tm_core_info.gal_e5a_meas_blk), p_MeasRptSrc);
        break;

      case GNSS_MEAS_BLK_SRC_BDS_B2AQ:
        sm_GnssMeas_memscpy((void *)&(tm_core_info.bds_b2aq_meas_blk), sizeof(tm_core_info.bds_b2aq_meas_blk), p_MeasRptSrc);
        break;

      default:
        TM_ERROR_1("LPC: lpc_handle_late_prm_report: Unknown PRM source %d",
                   p_MeasBlkSrc->z_GnssHeader.e_MeasBlkSrc);
        tm_core_abort_recover(TRUE, TM_STOP_REASON_GENERAL_ERROR); /*lint !e506 !e730 */
    }

    retVal = TRUE;

    if (GNSS_MEAS_BLK_SRC_GPS == p_MeasBlkSrc->z_GnssHeader.e_MeasBlkSrc || GNSS_MEAS_BLK_SRC_GLO == p_MeasBlkSrc->z_GnssHeader.e_MeasBlkSrc ||
        GNSS_MEAS_BLK_SRC_BDS == p_MeasBlkSrc->z_GnssHeader.e_MeasBlkSrc || GNSS_MEAS_BLK_SRC_QZSS_SBAS == p_MeasBlkSrc->z_GnssHeader.e_MeasBlkSrc)
    {
      /* Send all measurements (GPS/GLO/BDS) to AGNSS protocols to decide which one to use 
      Note: GAL does not support AGNSS */
      retVal = tm_core_info.prtl_func_cb_table[z_serving_client_data.prtlTableIndex].sess_info_fp(z_serving_client_data.serving_sess_handle,
                                                                                                  PRTL_SESS_INFO_PRM,
                                                                                                  z_serving_client_data.sess_info_param_ptr);
    }

    if (retVal == FALSE)
    {
      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "LPC:Protocol rejects PRM", 0, 0, 0);
      if (TM_CM_E911_STATE_CONCURRENT != phone_state_info.e911_state)
      {
        tm_core_abort_recover(TRUE, TM_STOP_REASON_GENERAL_ERROR); /*lint !e506 !e730 */
      }
      else if ((TM_CM_E911_STATE_CONCURRENT == phone_state_info.e911_state) &&
               (LDM_CLIENT_TYPE_SECONDARY == z_serving_client_data.e_client_type))
      {
        MSG(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "LPC:In concurrent mode send error for secondary client");
        tm_lpc_report_error_secondary(TM_STOP_REASON_GENERAL_ERROR, z_serving_client_data.serving_client_id);
      }
    }

    /* Output GNSS Measurement report only if NV is enabled */
    if (tm_core_info.config_info.gnss_meas_rep_qmi_out_control & SM_GNSS_BMAP_QMI_MEAS_REPORTING)
    {
      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "LPC:tm_core memset", 0, 0, 0);
      memset(z_serving_client_data.pz_pd_ext_status_info, 0, sizeof(*(z_serving_client_data.pz_pd_ext_status_info)));
      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "LPC:tm_core Late PDSM_EXT_STATUS_GNSS_MEASUREMENT", 0, 0, 0);

      z_serving_client_data.pz_pd_ext_status_info->client_id  = pd_clientId;

      z_serving_client_data.pz_pd_ext_status_info->ext_status_type = PDSM_EXT_STATUS_GNSS_MEASUREMENT;
      memset(&(z_serving_client_data.pz_pd_ext_status_info->ext_status_info.ext_gnss_meas_report), 0, sizeof(pdsm_pd_ext_gnss_meas_s_type));

      if (TRUE == tm_core_xlate_prms_to_ext_raw_clock_meas(&(z_serving_client_data.pz_pd_ext_status_info->ext_status_info.ext_gnss_meas_report),
                                                           p_MeasBlkSrc, z_serving_client_data.e_client_type))
      {
        tm_pdapi_ext_status_event_callback(PDSM_EXT_STATUS_GNSS_MEASUREMENT,
                                           z_serving_client_data.pz_pd_ext_status_info);
      }
    }
  } /* close "if(tm_core_info.prtl_func_cb_table[prtlTableIndex].sess_info_fp)" */
  else
  {
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "LPC:No protocol callback installed 9", 0, 0, 0);
}
}

/*===========================================================================

FUNCTION lpc_handle_prm_report

DESCRIPTION 
       Function to handle the PRM report for a session.
       In non-concurrent mode this function gets called directly from the
       callback ; however in case of concurrent mode and E911 client 
       lpc_handle_prm_report_secondary will be called first.

DEPENDENCIES

INPUT PARAMS
        p_MeasRptSrc_from_ldm GNSS Measurement Report structure

RETURN VALUE
        NONE

===========================================================================*/
static void lpc_handle_prm_report(sm_GpsMeasRptStructType *p_MeasRptSrc_from_ldm)
{
  sm_GpsMeasRptStructType       *p_MeasRptSrc = NULL;
  gnss_MeasBlkStructType        *p_MeasBlkSrc = NULL;
  tm_cm_phone_state_info_s_type  phone_state_info;
  boolean                        retVal = FALSE;
  pdsm_client_id_type            pd_clientId;

  if (NULL == p_MeasRptSrc_from_ldm)
  {
    TM_ERROR_0("lpc_handle_prm_report() MeasReport NULL");
    return;
  }

  memset(&phone_state_info, 0, sizeof(tm_cm_phone_state_info_s_type));
  tm_cm_iface_get_phone_state(&phone_state_info);

  p_MeasRptSrc = p_MeasRptSrc_from_ldm;
  p_MeasBlkSrc = &(p_MeasRptSrc_from_ldm->z_MeasBlk);

  MSG_4(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LPC:Handle PRM Rpt. Constellation[%d] Seq#[%d] of MaxNum[%d]. NHz %d",
        p_MeasBlkSrc->z_GnssHeader.e_MeasBlkSrc,
        p_MeasBlkSrc->z_GnssHeader.u_SeqNum,
        p_MeasBlkSrc->z_GnssHeader.u_MaxMessageNum,
        (GNSS_MEAS_RPT_PERIODIC_NHZ_SM == p_MeasBlkSrc->z_GnssHeader.e_GnssMeasRptreason));

  if (FALSE == (lp_clients[z_serving_client_data.serving_client_id].is_enabled))
  {
    MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "LPC:LDM client %d not enabled", z_serving_client_data.serving_client_id);
    return;
  }

  /*Make a copy of pd client ID*/
  pd_clientId = lp_clients[z_serving_client_data.serving_client_id].pd_clientId;

  MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LPC:Handle PRM client Id: %d", pd_clientId);

    tm_nmea_set_gnss_meas_update(p_MeasRptSrc);

  /* Set reason to general error for now, if return value */
  if (tm_core_info.prtl_func_cb_table[z_serving_client_data.prtlTableIndex].sess_info_fp)
  {
  memset((void *)&(z_serving_client_data.sess_info_param_ptr->prm_data), 0,
    sizeof(z_serving_client_data.sess_info_param_ptr->prm_data));
  sm_GnssMeas_memscpy((void *)&(z_serving_client_data.sess_info_param_ptr->prm_data),
    sizeof(z_serving_client_data.sess_info_param_ptr->prm_data), p_MeasRptSrc);

    if (!((phone_state_info.e911_state == TM_CM_E911_STATE_CONCURRENT) &&
          (pd_clientId == LDM_CLIENT_ID_E911)))
    {
      MSG_5(MSG_SSID_GPSSM, MSG_LEGACY_LOW, "LPC:Check for FTEST Sess: %d, Prot: %d, Sys: %d Seq#[%d] of MaxNum[%d]",
            tm_core_info.session_info.active_sess_handle,
            tm_core_info.session_info.from_protocol, p_MeasBlkSrc->z_GnssHeader.e_MeasBlkSrc,
            p_MeasBlkSrc->z_GnssHeader.u_SeqNum,
            p_MeasBlkSrc->z_GnssHeader.u_MaxMessageNum);


      if ((tm_core_info.session_info.active_sess_handle == TM_SESS_HANDLE_FTEST) &&
          (tm_core_info.session_info.from_protocol == TM_PRTL_TYPE_DIAG))
      {
        tm_core_info.session_info.op_requested &= ~TM_OP_REQ_PRM;
      }
    }

    /* Send all measurements (GPS/GLO/BDS/GAL) to AGNSS protocols to decide which
    one to use & report to server */
    MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LPC:Handle PRM Rpt; send all measurements to protocols");
    retVal = (GNSS_MEAS_RPT_PERIODIC_NHZ_SM == p_MeasBlkSrc->z_GnssHeader.e_GnssMeasRptreason) ?
       TRUE :
       tm_core_info.prtl_func_cb_table[z_serving_client_data.prtlTableIndex].sess_info_fp(z_serving_client_data.serving_sess_handle,
                                                                                                PRTL_SESS_INFO_PRM,
                                                                                                z_serving_client_data.sess_info_param_ptr);

    MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LPC:Handle PRM serving_sess_handle = %d", z_serving_client_data.serving_sess_handle);

    if (retVal == FALSE)
    {
      MSG(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "LPC:Protocol rejects PRM");
      if (TM_CM_E911_STATE_CONCURRENT != phone_state_info.e911_state)
      {
        tm_core_abort_recover(TRUE, TM_STOP_REASON_GENERAL_ERROR); /*lint !e506 !e730 */
      }
      else if ((TM_CM_E911_STATE_CONCURRENT == phone_state_info.e911_state) &&
               (LDM_CLIENT_TYPE_SECONDARY == z_serving_client_data.e_client_type))
      {
        MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "LPC:In concurrent mode send error for secondary client %d", z_serving_client_data.serving_client_id);
        tm_lpc_report_error_secondary(TM_STOP_REASON_GENERAL_ERROR, z_serving_client_data.serving_client_id);
      }
    }
    else
    {
      MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LPC:Handle PRM Rpt;save measurement to tm_core_info Src : %d", (p_MeasBlkSrc->z_GnssHeader.e_MeasBlkSrc));
    /* save measurement to tm_core_info */
    switch (p_MeasBlkSrc->z_GnssHeader.e_MeasBlkSrc)
    {
      case GNSS_MEAS_BLK_SRC_GPS:
        tm_core_info.w_RecvdMeasBlkMask |= (1 << GNSS_MEAS_BLK_SRC_GPS);
        sm_GnssMeas_memscpy((void *)&(tm_core_info.gps_meas_blk), sizeof(tm_core_info.gps_meas_blk), p_MeasRptSrc);
        break;

      case GNSS_MEAS_BLK_SRC_GPS_L2C:
        tm_core_info.w_RecvdMeasBlkMask |= (1 << GNSS_MEAS_BLK_SRC_GPS_L2C);
        sm_GnssMeas_memscpy((void *)&(tm_core_info.gps_l2c_meas_blk), sizeof(tm_core_info.gps_l2c_meas_blk), p_MeasRptSrc);
        break;

      case GNSS_MEAS_BLK_SRC_GPS_L5Q:
        tm_core_info.w_RecvdMeasBlkMask |= (1 << GNSS_MEAS_BLK_SRC_GPS_L5Q);
        sm_GnssMeas_memscpy((void *)&(tm_core_info.gps_l5q_meas_blk), sizeof(tm_core_info.gps_l5q_meas_blk), p_MeasRptSrc);
        break;

      case GNSS_MEAS_BLK_SRC_GLO:
        tm_core_info.w_RecvdMeasBlkMask |= (1 << GNSS_MEAS_BLK_SRC_GLO);
        sm_GnssMeas_memscpy((void *)&(tm_core_info.glo_meas_blk), sizeof(tm_core_info.glo_meas_blk), p_MeasRptSrc);
        break;

      case GNSS_MEAS_BLK_SRC_GLO_G2:
        tm_core_info.w_RecvdMeasBlkMask |= (1 << GNSS_MEAS_BLK_SRC_GLO_G2);
        sm_GnssMeas_memscpy((void *)&(tm_core_info.glo_g2_meas_blk), sizeof(tm_core_info.glo_g2_meas_blk), p_MeasRptSrc);
        break;

      case GNSS_MEAS_BLK_SRC_BDS:
        tm_core_info.w_RecvdMeasBlkMask |= (1 << GNSS_MEAS_BLK_SRC_BDS);
        sm_GnssMeas_memscpy((void *)&(tm_core_info.bds_meas_blk), sizeof(tm_core_info.bds_meas_blk), p_MeasRptSrc);
        break;

      case GNSS_MEAS_BLK_SRC_BDS_B2:
        tm_core_info.w_RecvdMeasBlkMask |= (1 << GNSS_MEAS_BLK_SRC_BDS_B2);
        sm_GnssMeas_memscpy((void *)&(tm_core_info.bds_b2_meas_blk), sizeof(tm_core_info.bds_b2_meas_blk), p_MeasRptSrc);
        break;

      case GNSS_MEAS_BLK_SRC_QZSS_SBAS:
        tm_core_info.w_RecvdMeasBlkMask |= (1 << GNSS_MEAS_BLK_SRC_QZSS_SBAS);
        sm_GnssMeas_memscpy((void *)&(tm_core_info.qzss_meas_blk), sizeof(tm_core_info.qzss_meas_blk), p_MeasRptSrc);
        break;

      case GNSS_MEAS_BLK_SRC_QZSS_L2C:
        tm_core_info.w_RecvdMeasBlkMask |= (1 << GNSS_MEAS_BLK_SRC_QZSS_L2C);
        sm_GnssMeas_memscpy((void *)&(tm_core_info.qzss_l2c_meas_blk), sizeof(tm_core_info.qzss_l2c_meas_blk), p_MeasRptSrc);
        break;

      case GNSS_MEAS_BLK_SRC_QZSS_L5Q:
        tm_core_info.w_RecvdMeasBlkMask |= (1 << GNSS_MEAS_BLK_SRC_QZSS_L5Q);
        sm_GnssMeas_memscpy((void *)&(tm_core_info.qzss_l5q_meas_blk), sizeof(tm_core_info.qzss_l5q_meas_blk), p_MeasRptSrc);
        break;

      case GNSS_MEAS_BLK_SRC_GAL:
        tm_core_info.w_RecvdMeasBlkMask |= (1 << GNSS_MEAS_BLK_SRC_GAL);
        sm_GnssMeas_memscpy((void *)&(tm_core_info.gal_meas_blk), sizeof(tm_core_info.gal_meas_blk), p_MeasRptSrc);
        break;

      case GNSS_MEAS_BLK_SRC_NAVIC:
        tm_core_info.w_RecvdMeasBlkMask |= (1 << GNSS_MEAS_BLK_SRC_NAVIC);
        sm_GnssMeas_memscpy((void *)&(tm_core_info.navic_meas_blk), sizeof(tm_core_info.navic_meas_blk), p_MeasRptSrc);
        break;

      case GNSS_MEAS_BLK_SRC_GAL_E5A:
        tm_core_info.w_RecvdMeasBlkMask |= (1 << GNSS_MEAS_BLK_SRC_GAL_E5A);
        sm_GnssMeas_memscpy((void *)&(tm_core_info.gal_e5a_meas_blk), sizeof(tm_core_info.gal_e5a_meas_blk), p_MeasRptSrc);
        break;

      case GNSS_MEAS_BLK_SRC_BDS_B2AQ:
        tm_core_info.w_RecvdMeasBlkMask |= (1 << GNSS_MEAS_BLK_SRC_BDS_B2AQ);
        sm_GnssMeas_memscpy((void *)&(tm_core_info.bds_b2aq_meas_blk), sizeof(tm_core_info.bds_b2aq_meas_blk), p_MeasRptSrc);
        break;

      default:
        TM_ERROR_1("LPC: lpc_handle_prm_report: Unknown PRM source %d",
          p_MeasBlkSrc->z_GnssHeader.e_MeasBlkSrc);
        tm_core_abort_recover(TRUE, TM_STOP_REASON_GENERAL_ERROR); /*lint !e506 !e730 */
    }

    if ((p_MeasRptSrc->z_MeasBlk.z_GnssHeader.e_GnssMeasRptreason == GNSS_MEAS_RPT_MEAS_DONE_SM) ||
        ((p_MeasRptSrc->z_MeasBlk.z_GnssHeader.e_GnssMeasRptreason == GNSS_MEAS_RPT_PERIODIC_SM) &&
         (tm_core_info.config_info.nmea_config_type.pd_nmea_reporting_type == PDSM_PA_NMEA_REPORT_AT_1HZ_INTERVAL)))
      {
        TM_MED_1("LPC:Handle PRM RptReason %d", p_MeasRptSrc->z_MeasBlk.z_GnssHeader.e_GnssMeasRptreason);
        /*Generate NMEA*/
        tm_core_trigger_meas_related_NMEA(p_MeasRptSrc);
      }
        else
        {
        TM_MED_0("LPC:Handle PRM Rpt; No NMEA for NHz meas");
      } /* close "else ( if meas. is not of MEAS_DONE )" */

    /* Send extra pdapi event for PRM */
    if (p_MeasBlkSrc->z_GnssHeader.u_SeqNum == p_MeasBlkSrc->z_GnssHeader.u_MaxMessageNum)
      {
      /* Send extra pdapi event for intermediate fix report */
        MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LPC:Send extra pdapi event for intermediate fix report for LDMClient %d; PDSMClient %d Seq %d",
              z_serving_client_data.serving_client_id,
              pd_clientId,
              p_MeasBlkSrc->z_GnssHeader.u_SeqNum);

        z_serving_client_data.pz_pd_ext_status_info->client_id    = pd_clientId;
        z_serving_client_data.pz_pd_ext_status_info->ext_status_type = PDSM_EXT_STATUS_MEASUREMENT;

        memset(&(z_serving_client_data.pz_pd_ext_status_info->ext_status_info.ext_meas_report_type),
               0, sizeof(pdsm_pd_ext_meas_s_type));

      /* Conditionally clear the constellation meas blk validity if the corresponding meas blk is not received. */
      if (!(tm_core_info.w_RecvdMeasBlkMask & (1 << GNSS_MEAS_BLK_SRC_GPS)))
      {
        tm_core_info.gps_meas_blk.z_GnssHeader.u_NumSvs = 0;
      }
      if (!(tm_core_info.w_RecvdMeasBlkMask & (1 << GNSS_MEAS_BLK_SRC_GPS_L2C)))
      {
        tm_core_info.gps_l2c_meas_blk.z_GnssHeader.u_NumSvs = 0;
      }
      if (!(tm_core_info.w_RecvdMeasBlkMask & (1 << GNSS_MEAS_BLK_SRC_GPS_L5Q)))
      {
        tm_core_info.gps_l5q_meas_blk.z_GnssHeader.u_NumSvs = 0;
      }
      if (!(tm_core_info.w_RecvdMeasBlkMask & (1 << GNSS_MEAS_BLK_SRC_GLO)))
      {
        tm_core_info.glo_meas_blk.z_GnssHeader.u_NumSvs = 0;
      }
      if (!(tm_core_info.w_RecvdMeasBlkMask & (1 << GNSS_MEAS_BLK_SRC_GLO_G2)))
      {
        tm_core_info.glo_g2_meas_blk.z_GnssHeader.u_NumSvs = 0;
      }
      if (!(tm_core_info.w_RecvdMeasBlkMask & (1 << GNSS_MEAS_BLK_SRC_BDS)))
      {
        tm_core_info.bds_meas_blk.z_GnssHeader.u_NumSvs = 0;
      }
      if (!(tm_core_info.w_RecvdMeasBlkMask & (1 << GNSS_MEAS_BLK_SRC_BDS_B2)))
      {
        tm_core_info.bds_b2_meas_blk.z_GnssHeader.u_NumSvs = 0;
      }
      if (!(tm_core_info.w_RecvdMeasBlkMask & (1 << GNSS_MEAS_BLK_SRC_BDS_B2AQ)))
      {
        tm_core_info.bds_b2aq_meas_blk.z_GnssHeader.u_NumSvs = 0;
      }
      if (!(tm_core_info.w_RecvdMeasBlkMask & (1 << GNSS_MEAS_BLK_SRC_GAL)))
      {
        tm_core_info.gal_meas_blk.z_GnssHeader.u_NumSvs = 0;
      }
      if (!(tm_core_info.w_RecvdMeasBlkMask & (1 << GNSS_MEAS_BLK_SRC_NAVIC)))
      {
        tm_core_info.navic_meas_blk.z_GnssHeader.u_NumSvs = 0;
      }
      if (!(tm_core_info.w_RecvdMeasBlkMask & (1 << GNSS_MEAS_BLK_SRC_GAL_E5A)))
      {
        tm_core_info.gal_e5a_meas_blk.z_GnssHeader.u_NumSvs = 0;
      }
      if (!(tm_core_info.w_RecvdMeasBlkMask & (1 << GNSS_MEAS_BLK_SRC_QZSS_SBAS)))
      {
        tm_core_info.qzss_meas_blk.z_GnssHeader.u_NumSvs = 0;
      }
      if (!(tm_core_info.w_RecvdMeasBlkMask & (1 << GNSS_MEAS_BLK_SRC_QZSS_L2C)))
      {
        tm_core_info.qzss_l2c_meas_blk.z_GnssHeader.u_NumSvs = 0;
      }
      if (!(tm_core_info.w_RecvdMeasBlkMask & (1 << GNSS_MEAS_BLK_SRC_QZSS_L5Q)))
      {
        tm_core_info.qzss_l5q_meas_blk.z_GnssHeader.u_NumSvs = 0;
      }
      /* Clear the measblk mask as well. */
      tm_core_info.w_RecvdMeasBlkMask = 0;

      tm_core_xlate_prms_to_ext_status(&(z_serving_client_data.pz_pd_ext_status_info->ext_status_info.ext_meas_report_type),
        &tm_core_info);

      tm_pdapi_ext_status_event_callback(PDSM_EXT_STATUS_MEASUREMENT, z_serving_client_data.pz_pd_ext_status_info); 
    } 

      /* Output GNSS Measurement report only if NV is enabled */
      if (tm_core_info.config_info.gnss_meas_rep_qmi_out_control & SM_GNSS_BMAP_QMI_MEAS_REPORTING)
      {
        /* Send extra pdapi event for Raw measurement, as individual constellation measurements arrive */
        //tm_loc_processing_client_info *pz_client_info = NULL;
        memset(z_serving_client_data.pz_pd_ext_status_info, 0, sizeof(*z_serving_client_data.pz_pd_ext_status_info));

        MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LPC:Send extra pdapi event for Raw measurement for LDMClient %d; PDSMClient is %d Seq %d",
              z_serving_client_data.serving_client_id,
              pd_clientId,
              p_MeasBlkSrc->z_GnssHeader.u_SeqNum);
        z_serving_client_data.pz_pd_ext_status_info->client_id       = pd_clientId;

        z_serving_client_data.pz_pd_ext_status_info->ext_status_type = PDSM_EXT_STATUS_GNSS_MEASUREMENT;

        memset(&(z_serving_client_data.pz_pd_ext_status_info->ext_status_info.ext_gnss_meas_report),
        0, sizeof(pdsm_pd_ext_gnss_meas_s_type));

        if (TRUE == tm_core_xlate_prms_to_ext_raw_clock_meas(&(z_serving_client_data.pz_pd_ext_status_info->ext_status_info.ext_gnss_meas_report),
        p_MeasBlkSrc,
        z_serving_client_data.e_client_type))
        {
          tm_pdapi_ext_status_event_callback(PDSM_EXT_STATUS_GNSS_MEASUREMENT, z_serving_client_data.pz_pd_ext_status_info);
        }
#ifdef FEATURE_GNSS_RAW_MEAS_OUTPUT_DEBUG
        else
        {
        MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "LPC:Dropping MeasReportInd System[%d] Seq#[%d] of MaxNum[%d]",
                p_MeasBlkSrc->z_GnssHeader.e_MeasBlkSrc,
                p_MeasBlkSrc->z_GnssHeader.u_SeqNum,
                p_MeasBlkSrc->z_GnssHeader.u_MaxMessageNum);
         }
#endif
      }
    } /* close "else ( if protocol module accepted sess_info )" */
  } /* close "if(tm_core_info.prtl_func_cb_table[prtlTableIndex].sess_info_fp)" */
  else
  {
    MSG(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "LPC:No protocol callback installed 8");
}
}

/*===========================================================================
FUNCTION lpc_check_for_aiding_recieved

DESCRIPTION 
       In concurrent mode; LPC maintains list of SVs for which aiding was provided.
       This function checks if given SV is present in the aiding list.
       
DEPENDENCIES

INPUT PARAMS:
    u_sv            SV ID
    e_MeasBlkSrc    Measurement source.

RETURN VALUE
    TRUE if SV is present in aiding list
    FALSE otherwise


SIDE EFFECTS
===========================================================================*/
static boolean lpc_check_for_aiding_recieved(uint16 u_sv, gnss_MeasBlkSrcEnumType e_MeasBlkSrc)
{
  boolean retVal = FALSE;
  tm_loc_processing_client_info *p_z_client_info = lpc_get_loc_processing_client(z_serving_client_data.serving_client_id);

  if (NULL != p_z_client_info)
  {
    MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LPC:Found the client in list validate aiding info.");
    if (GNSS_MEAS_BLK_SRC_GPS == e_MeasBlkSrc)
    {
      MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LPC:Validate if AA received for GPS SV %d", u_sv);
      if (((p_z_client_info->aa_received_gps_svs) & (1 << (u_sv - GPS_LOWEST_PRN))) != 0)
      {
        MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_MED, "AA received for GPS SV %d", u_sv);
        retVal = TRUE;
      }
    }
    else if (GNSS_MEAS_BLK_SRC_GLO == e_MeasBlkSrc)
    {
      MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LPC:Validate if AA received for GLO SV %d", u_sv);
      if (((p_z_client_info->aa_received_glo_svs) & (1 << (u_sv - GLO_LOWEST_ID))) != 0)
      {
        MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LPC:AA received for GLO SV %d", u_sv);
        retVal = TRUE;
      }
    }
    else if (GNSS_MEAS_BLK_SRC_BDS == e_MeasBlkSrc)
    {
      MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LPC:Validate if AA received for BDS SV %d", u_sv);
      if (((p_z_client_info->aa_received_bds_svs) & (1 << (u_sv - BDS_LOWEST_ID))) != 0)
      {
        MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LPC:AA received for BDS SV %d", u_sv);
        retVal = TRUE;
      }
    }
    else if (GNSS_MEAS_BLK_SRC_GAL == e_MeasBlkSrc)
    {
      MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LPC:Validate if AA received for GAL SV %d", u_sv);
      if (((p_z_client_info->aa_received_gal_svs) & (1 << (u_sv - GAL_LOWEST_ID))) != 0)
      {
        MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LPC:AA received for GAL SV %d", u_sv);
        retVal = TRUE;
      }
    }
    else if (GNSS_MEAS_BLK_SRC_NAVIC == e_MeasBlkSrc)
    {
      MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LPC:Validate if AA received for NAVIC SV %d", u_sv);
      if (((p_z_client_info->aa_received_navic_svs) & (1 << (u_sv - NAVIC_LOWEST_ID))) != 0)
      {
        MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LPC:AA received for GAL SV %d", u_sv);
        retVal = TRUE;
      }
    }
    }
  else
  {
    MSG(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "LPC:Client not found in the list");
  }

  return retVal;
}



/*===========================================================================
FUNCTION lpc_filter_meas_rpt

DESCRIPTION 
       Function to filter out the SVs in PRM report as per the aiding data
       recieved.
       In concurrent mode measurement processing for E911 session is performed
       in LPC. This function is part of handling the measurement for E911 session
       in concurrent mode.
       
DEPENDENCIES

INPUT PARAMS
    p_measReport GNSS measurement report

RETURN VALUE
    NONE

SIDE EFFECTS
===========================================================================*/
static void lpc_filter_meas_rpt(sm_GpsMeasRptStructType *p_measReport)
{
  /*Get the aiding info and filter the measurement report*/
  gnss_MeasStructType temp[N_ACTIVE_BDS_CHAN];
  uint8 temp_u_NumSvs = 0;
  int itr = 0;
  gnss_MeasBlkSrcEnumType e_PrimaryConstellation = GNSS_MEAS_BLK_SRC_MAX;

  MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LPC:Filtering PRM report receieved as per the AA data.");

  if (p_measReport == NULL || (p_measReport->z_MeasBlk.z_GnssHeader.u_NumSvs == 0))
  {
    MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LPC:Measurement Report is empty.");
    return;
  }
  memset((void *)temp, 0, sizeof(temp));
  e_PrimaryConstellation = sm_util_get_master_constellation((p_measReport->z_MeasBlk).z_GnssHeader.e_MeasBlkSrc);
  MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LPC:Original number of SVs = %d", p_measReport->z_MeasBlk.z_GnssHeader.u_NumSvs);

  for (itr = 0; itr < p_measReport->z_MeasBlk.z_GnssHeader.u_NumSvs; itr++)
  {
    if (lpc_check_for_aiding_recieved(p_measReport->z_MeasBlk.z_Gnss[itr].w_Sv, e_PrimaryConstellation))
    {
      MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LPC:SVID %d qualified", p_measReport->z_MeasBlk.z_Gnss[itr].w_Sv);
      temp[temp_u_NumSvs++] = p_measReport->z_MeasBlk.z_Gnss[itr];
      }
      else
      {
      MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LPC: SVID %d not present in aiding list", p_measReport->z_MeasBlk.z_Gnss[itr].w_Sv);
      }
    }
    /*Copy all the qualified SVs to final SVs list in PRM report*/
  memscpy((void *)p_measReport->z_MeasBlk.z_Gnss,
          sizeof(gnss_MeasStructType) * p_measReport->z_MeasBlk.z_GnssHeader.u_NumSvs,
          (void *)temp,
            sizeof(gnss_MeasStructType) * temp_u_NumSvs);

  if (temp_u_NumSvs != p_measReport->z_MeasBlk.z_GnssHeader.u_NumSvs)
    {
    p_measReport->z_MeasBlk.z_GnssHeader.u_NumSvs = temp_u_NumSvs;
    MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_LOW, "LPC:New u_NumSvs = %d", p_measReport->z_MeasBlk.z_GnssHeader.u_NumSvs);
      }
      }

/*===========================================================================
FUNCTION lpc_handle_prm_report_secondary

DESCRIPTION 
       Function to perform PRM handling for secondary client in concurrent mode.
       In concurrent mode measurement processing for secondary client is performed
       in LPC.

DEPENDENCIES

INPUT PARAMS
    sm_GpsMeasRptStructType* GNSS measurement report

RETURN VALUE
    NONE

SIDE EFFECTS
===========================================================================*/
static void lpc_handle_prm_report_secondary(sm_GpsMeasRptStructType *p_MeasRptSrc)
{
  boolean    multi_report;

  MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LPC:Handle Secondary client PRM report in concurrency");

  if (NULL == p_MeasRptSrc)
  {
    MSG(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "LPC: NULL measurement report recieved");
    return;
  }

  if (FALSE == (lp_clients[z_serving_client_data.serving_client_id].is_enabled))
  {
    MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "LPC:LDM client %d not enabled", z_serving_client_data.serving_client_id);
    return;
  }

  multi_report = lp_clients[z_serving_client_data.serving_client_id].sess_params.multi_report_msa;

  /*Filter the MEAS report only for E911 MSA in concurrent mode as per AA recieved for secondary client */
  if ((LDM_CLIENT_ID_E911 == z_serving_client_data.serving_client_id) &&
      (TM_SESS_OPERATION_MODE_MSA == lp_clients[z_serving_client_data.serving_client_id].sess_params.op_mode))
  {
    lpc_filter_meas_rpt(p_MeasRptSrc);
  }

  if (p_MeasRptSrc->z_MeasBlk.z_GnssHeader.e_GnssMeasRptreason == GNSS_MEAS_RPT_MEAS_DONE_SM)
  {
    MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LPC:Recieved done-meas");

    if ((NULL != z_lpc_control_data) &&
        (z_lpc_control_data->e_sessionState == LPC_STATE_IN_SESSION_REPORT_PENDING))
    {
      if (!(multi_report) &&
          p_MeasRptSrc->z_MeasBlk.z_GnssHeader.u_SeqNum == p_MeasRptSrc->z_MeasBlk.z_GnssHeader.u_MaxMessageNum)
      {
        /*Stop PRM timer*/
        os_TimerStop(z_lpc_control_data->p_location_Timer);
        z_lpc_control_data->e_sessionState = LPC_STATE_IN_SESSION_NO_REPORT_PENDING;
      }

      MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LPC:send done-meas to protocols;for secondary client in concurrency mode");
      lpc_handle_prm_report(p_MeasRptSrc);
    }
    else if (z_serving_client_data.v_filterAA)
    {
      MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LPC:send done-meas to protocols;for main client in non-concurrent mode");
      lpc_handle_prm_report(p_MeasRptSrc);
    }
    else
    {
      MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LPC:send late PRM report; for secondary client in concurrency mode");
      lpc_handle_late_prm_report(p_MeasRptSrc);
    }
  }
  else
  {
    MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LPC:send periodic report; for secondary client in concurrency mode");
    lpc_handle_prm_report(p_MeasRptSrc);
  }
}

/*===========================================================================
FUNCTION lpc_forward_position_report

DESCRIPTION: 
    Function to forward the position report as final/intermediate fix.

DEPENDENCIES

INPUT PARAMS
    tm_fix_report_s_type* Fix report structure
    boolean               If TRUE forward as final fix else intermediate/
RETURN VALUE
    NONE

SIDE EFFECTS
===========================================================================*/
static void lpc_forward_position_report(tm_fix_report_s_type *pz_FixReport,
                                        boolean u_finalReport)
{
  MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LPC: Fix forwarding to session");

  if (NULL == z_lpc_control_data)
  {
    MSG(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "LPC:LPC control data not initialized or client not found");
    return;
  }

  pz_FixReport->q_FixTimeRemainingMSec = os_TimerCheck(z_lpc_control_data->p_location_Timer);

  if (u_finalReport)
  {
    MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LPC: Sending FINAL fix. Time remaining %d", pz_FixReport->q_FixTimeRemainingMSec);
    lpc_handle_final_fix_report(&(pz_FixReport->z_GpsFixRpt), z_serving_client_data.serving_client_id);
  }
  else
  {
    MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LPC: Sending INTERMEDIATE fix. Time remaining %d", pz_FixReport->q_FixTimeRemainingMSec);
    lpc_handle_intermediate_fix_report(pz_FixReport);
  }
}


/*===========================================================================
FUNCTION lpc_handle_final_position_report_secondary

DESCRIPTION: 
    Function to handle final fix for secondary client  in concurrent mode.
    This function stops the timer and call lpc_forward_position_report

DEPENDENCIES

INPUT PARAMS
    tm_fix_report_s_type*    Fix report

RETURN VALUE
    NONE

SIDE EFFECTS
===========================================================================*/
static void lpc_handle_final_position_report_secondary(tm_fix_report_s_type *p_FinalfixReport)
{
  MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LPC: Stop timer and forward final fix to protocols");

  if (NULL == z_lpc_control_data)
  {
    MSG(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "LPC:LPC control data not initialized or client not found");
    return;
  }

  /*Stop fix timer*/
  os_TimerStop(z_lpc_control_data->p_location_Timer);

  z_lpc_control_data->q_reportsPending &= ~((uint32)LM_SESSION_REQUEST_FIX);

  lpc_forward_position_report(p_FinalfixReport, (boolean)TRUE);

  MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LPC: Done with final fix forwarding");
}

/*===========================================================================
FUNCTION lpc_is_fix_source_reportable

DESCRIPTION: 
    This function decides if a fix recieved from LDM is reportable as intermediate/final to protocols or not.
    All GNSS fixes are reportable. CPI fixes are reportable as final fixes, if NV for injecting CPIs is enabled AND
    if in E911 session AND if reliabilty is medium or better.  For intermediate fixes, reliability is not checked
    This function is used E911 session fix processing when in concurrent mode.

DEPENDENCIES

INPUT PARAMS
    p_fixReport - Pointer to structure containing the fix report from PE
    v_reportableFinalfix:
        TRUE: This fix is considered for final fix
        FALSE: This fix is considered for intermediate fix

RETURN VALUE
    TRUE: If the fix is reportable as intermediate/final fix (based on input param v_reportableFinalfix)
    FALSE: If the fix is NOT reportable as intermediate/final (based on input param v_reportableFinalfix)

SIDE EFFECTS
===========================================================================*/
static boolean lpc_is_fix_source_reportable(const sm_FixInfoStructType *const p_fixReport, boolean v_reportableFinalfix)
{
  boolean retVal = FALSE;
  boolean b_e911_session = FALSE;

  MSG(MSG_SSID_GPSSM, MSG_LEGACY_LOW, "LPC: Check if fix is reportable.");
  if (NULL == p_fixReport)
  {
    MSG(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "LPC: NULL fix report received.");
    return FALSE;
  }

  if (NULL == z_lpc_control_data)
  {
    MSG(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "LPC: LPC Control data not set for secondary client");
    return FALSE;
  }

  if (LDM_CLIENT_ID_E911 == z_serving_client_data.serving_client_id)
  {
    MSG(MSG_SSID_GPSSM, MSG_LEGACY_LOW, "LPC: E911 Session in progress.");
    b_e911_session = TRUE;
  }

  /* Checks if the fix is GNSS */
  if (!(p_fixReport->z_NavPos.z_SmPosFlags.b_IsPosBestAvail)) /*Checks if its GNSS fix */
  {
    retVal = TRUE;
  }
  /* CPI fixes are reportable if NV for injecting CPIs is enabled, if in E911 session and if reliabilty is medium or better */
  else if ((tm_core_get_emergency_support_config() & TM_E911_CPI_INJECTIONS_ENABLE) && /* If NV is set to enabled to report CPI fixes */
           (b_e911_session == TRUE) && /* If we are in E911 session */
           ((p_fixReport->z_NavPos.z_SmPosFlags.b_IsPosBestAvail) && /*Checks if its BestPos and CPI */
            (p_fixReport->z_NavPos.z_SmPosFlags.b_IsCoarsePos))) /* Checks of its CPI */
  {
    if (TRUE == v_reportableFinalfix)
    {
      if ((p_fixReport->z_NavPos.e_HoriRelIndicator >= PDSM_POSITION_RELIABILITY_MEDIUM) && /* Only reliabilty of medium or better is shipped */
          (p_fixReport->z_NavPos.e_HoriRelIndicator < PDSM_POSITION_RELIABILITY_MAX) &&
          (NULL != z_lpc_control_data) &&
          (TRUE == z_lpc_control_data->v_reflocReceived)) /* Fix reportable only if RefLoc received in this session for E911*/
      {
        retVal = TRUE;
      }
    }
    else
    {
      /* This fix is considered for intermediate fix. Reliabilty check not needed */
      retVal = TRUE;
    }
  }

  return retVal;

}

/*===========================================================================
FUNCTION lpc_calculate_mag_deviation

DESCRIPTION: 
    This function calculated the Mag deviation from Fix report.
    This function is used E911 session fix processing when in concurrent mode.

DEPENDENCIES

INPUT PARAMS
    p_fixReport - Pointer to Fix Report Structure

RETURN VALUE
    NONE

SIDE EFFECTS
===========================================================================*/
static void lpc_calculate_mag_deviation(sm_FixInfoStructType *p_fixReport)
{
  FLT   f_MagDev = 0.0;

  if (p_fixReport->z_NavPos.u_MagDevGood)
  {
    f_MagDev = p_fixReport->z_NavPos.f_MagDeviation;
  }
  else
  {
    /* If Mag Dev is not available, attempt to compute it */
    if (cgps_MagDeviation(p_fixReport->z_NavPos.d_PosLla[0], p_fixReport->z_NavPos.d_PosLla[1], &f_MagDev) == TRUE)
    {
      /*Calculation of Mag Dev Succesfull*/
      p_fixReport->z_NavPos.u_MagDevGood = TRUE;
    }
    else
    {
      p_fixReport->z_NavPos.u_MagDevGood = FALSE;
      MSG(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "LPC:Mag Dev was not present and could not be calculated");
    }
  }

  p_fixReport->z_NavPos.f_MagDeviation = f_MagDev;

}

/*===========================================================================
FUNCTION lpc_set_sess_best_fix_report

DESCRIPTION: 
    This function saves the lowest HEPE fix for a session.
    This function is used E911 session fix processing when in concurrent mode.
 
DEPENDENCIES

INPUT PARAMS:
    pFixReport - fix report received from PE

RETURN VALUE
    NONE

SIDE EFFECTS
===========================================================================*/
static void lpc_set_sess_best_fix_report(const sm_FixInfoStructType *const p_fixReport)
{
  FLT f_hepe;

  if (NULL == z_lpc_control_data)
  {
    MSG(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "LPC:LPC control data not initialized or client not found");
    return;
  }

  if (NULL == p_fixReport)
  {
    MSG(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "LPC:Fix with no data, ignored for session best.");
    return;
  }

  MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LPC:Set Best Sess Fix: [%d]", z_lpc_control_data->u_sessBestFixValid);

  if (C_GPS_WEEK_UNKNOWN == p_fixReport->z_NavPos.w_GpsWeek)
  {
    MSG(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "LPC:Fix with no GPS time, ignored for session best.");
    return;
  }

  MSG_2(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "LPC:Fix Report received, PositionFlags: IsBest %d, isCPI %d",
        p_fixReport->z_NavPos.z_SmPosFlags.b_IsPosBestAvail,
        p_fixReport->z_NavPos.z_SmPosFlags.b_IsCoarsePos);

  /*Cache only fixes which are CPI or GNSS sourced. This will avoid sending RefLoc position back to the network */
  if (!lpc_is_fix_source_reportable(p_fixReport, FALSE))
  {
    MSG(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "LPC:Fix not CPI or GNSS, ignored for session best.");
    return;
  }

  if (!z_lpc_control_data->u_sessBestFixValid)
  {
    z_lpc_control_data->z_sessBestFixRpt = *p_fixReport;
    z_lpc_control_data->u_sessBestFixValid = TRUE;

    z_lpc_control_data->f_sessBestHepe = (FLT)sqrt((p_fixReport->z_NavPos.f_ErrorEllipse[1] *
                                                    p_fixReport->z_NavPos.f_ErrorEllipse[1]) +
                                                   (p_fixReport->z_NavPos.f_ErrorEllipse[2] *
                                                    p_fixReport->z_NavPos.f_ErrorEllipse[2]));

    if (msg_status(MSG_SSID_GPSSM, MSG_LEGACY_MED))
    {
      char b_Buffer[250];
      (void)snprintf(b_Buffer, sizeof(b_Buffer), "LPC:New fix saved as best. hepe: %f", z_lpc_control_data->f_sessBestHepe);
      MSG_SPRINTF_1(MSG_SSID_GPSSM, MSG_LEGACY_MED, "%s", b_Buffer);
    }

    if (p_fixReport->z_NavPos.z_SmPosFlags.b_IsPosBestAvail)
    {
      MSG(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "LPC:New Best Pos fix saved as best.");
    }
    return;
  }

  f_hepe = (FLT)sqrt((p_fixReport->z_NavPos.f_ErrorEllipse[1] *
                      p_fixReport->z_NavPos.f_ErrorEllipse[1]) +
                     (p_fixReport->z_NavPos.f_ErrorEllipse[2] *
                      p_fixReport->z_NavPos.f_ErrorEllipse[2]));

  if (msg_status(MSG_SSID_GPSSM, MSG_LEGACY_HIGH))
  {
    char b_Buffer[250];
    (void)snprintf(b_Buffer, sizeof(b_Buffer), "LPC:HEPE New: %f, Old: %f", f_hepe, z_lpc_control_data->f_sessBestHepe);
    MSG_SPRINTF_1(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "%s", b_Buffer);
  }

  if (f_hepe < z_lpc_control_data->f_sessBestHepe)
  {
    if (msg_status(MSG_SSID_GPSSM, MSG_LEGACY_MED))
    {
      char b_Buffer[250];
      (void)snprintf(b_Buffer, sizeof(b_Buffer), "LPC:New fix saved as best. hepe: %f", f_hepe);
      MSG_SPRINTF_1(MSG_SSID_GPSSM, MSG_LEGACY_MED, "%s", b_Buffer);
    }

    z_lpc_control_data->z_sessBestFixRpt = *p_fixReport;
    z_lpc_control_data->f_sessBestHepe = f_hepe;

    if (p_fixReport->z_NavPos.z_SmPosFlags.b_IsPosBestAvail)
    {
      MSG(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "LPC:New Best Pos fix saved as best.");
    }
  }
}


/*===========================================================================
FUNCTION lpc_position_accuracy_check

DESCRIPTION: 
    This function checks if the position fix reported by LM qualifies the 
    QoS accuracy threshold for the session.
    This function is used E911 session fix processing when in concurrent mode.

DEPENDENCIES

INPUT PARAMS:
    pFixReport - fix report received from PE

RETURN VALUE
    NONE

SIDE EFFECTS
===========================================================================*/
static boolean lpc_position_accuracy_check(const sm_FixInfoStructType *const p_fixReport,
                                           ldm_client_id_e_type e_client_id)
{
  FLT f_hepe = 0;
  uint32 q_current_hepe_limit;

  if ((p_fixReport == NULL))
  {
    MSG(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "LPC:NULL fix report.");
    return FALSE;
  }

  if (lp_clients[e_client_id].is_enabled == TRUE)
  {
    q_current_hepe_limit = lp_clients[e_client_id].sess_params.accuracy_threshold;
    if (q_current_hepe_limit == 0)
    {
      MSG(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "LPC:current hepe limit not set.");
      return FALSE;
    }
    f_hepe = (FLT)sqrt((p_fixReport->z_NavPos.f_ErrorEllipse[1] *
                        p_fixReport->z_NavPos.f_ErrorEllipse[1]) +
                       (p_fixReport->z_NavPos.f_ErrorEllipse[2] *
                        p_fixReport->z_NavPos.f_ErrorEllipse[2]));

    if (msg_status(MSG_SSID_GPSSM, MSG_LEGACY_MED))
    {
      char b_Buffer[250];
      (void)snprintf(b_Buffer, sizeof(b_Buffer), "Hepe=%f Current Hepe Limit=%ld\r\n",
                     f_hepe, q_current_hepe_limit);
      MSG_SPRINTF_1(MSG_SSID_GPSSM, MSG_LEGACY_MED, "%s", b_Buffer);
    }

    if (f_hepe <= q_current_hepe_limit)
    {
      MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LPC:Current HEPE met QOS limit.");
      return TRUE;
    }
    else
    {
      MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LPC:Current HEPE didn't meet QOS limit.");
      return FALSE;
    }
  }
  else
  {
    MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "LPC:Client %d not initialised in LPC.", e_client_id);
    return FALSE;
  }
}

/*===========================================================================
FUNCTION lpc_handle_e911_fix_report_secondary

DESCRIPTION: 
    During concurrent mode; fix processing for secondary client session is handled in
    LPC. This function performs the fix qualification.

DEPENDENCIES

INPUT PARAMS
    p_zFixReport Pointer to fix report from LM

RETURN VALUE
    NONE

SIDE EFFECTS
===========================================================================*/
static void lpc_handle_fix_report_secondary(tm_fix_report_s_type *p_zFixReport)
{
  sm_FixInfoStructType        *p_fixReport;
  boolean                     bBestAvailPos = FALSE;
  boolean                     b_e911_session = FALSE;
  lm_gps_mode_e_type          e_mode;
  lm_mo_mt_mode_e_type        e_mo_mt;
  ldm_client_id_e_type        serving_client_id;
  boolean bSensorPropogatedGNSSFix = FALSE;
  float INSOnlyFixHepe = 0;

  if (NULL == p_zFixReport)
  {
    MSG(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "LPC: NULL fix report send by LDM");
    return;
  }

  if (NULL == z_lpc_control_data)
  {
    MSG(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "LPC: Control data not intialized, not is correct mode.");
    return;
  }

  p_fixReport = &(p_zFixReport->z_GpsFixRpt);

  serving_client_id = z_serving_client_data.serving_client_id;
  e_mode = tm_core_convert_op_mode(lp_clients[serving_client_id].sess_params.op_mode, lp_clients[serving_client_id].prtl_type);
  e_mo_mt = lpc_mo_mt_mode(z_serving_client_data.serving_sess_handle);

  if (LDM_CLIENT_ID_E911 == serving_client_id)
  {
    b_e911_session = TRUE;
  }

  MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "LPC: In concurrent mode; perform fix qualificatioin for secondary client %d", serving_client_id);
  MSG_2(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "LPC: e_mode = %d and e_mo_mt = %d", e_mode, e_mo_mt);

  if (p_fixReport->z_NavPos.z_SmPosFlags.b_IsPosBestAvail)
  {
    bBestAvailPos = TRUE; /* original NO_FIX report: CellDB fix, WiFi fix etc non-GNSS fix */
  }
  else
  {
    z_lpc_control_data->b_gnssFixReceivedThisSession = TRUE;
  }

  /* If NV is set to enabled to inject CPI fixes */
  if (tm_core_get_emergency_support_config() & TM_E911_CPI_INJECTIONS_ENABLE)
  {
    /* Check if LDM sent an inconsistent/faulty CPI fix to LM before */
    if (p_fixReport->z_NavPos.b_CpiInconsistent == TRUE)
    {
      MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LPC: Received inconsistent/faulty CPI indication from MGP");

      /* Clear cache only if an CPI fix has been cached */
      if ((z_lpc_control_data->z_sessBestFixRpt.z_NavPos.z_SmPosFlags.b_IsPosBestAvail) && /*Checks if its BestPos and CPI for cached fix*/
          (z_lpc_control_data->z_sessBestFixRpt.z_NavPos.z_SmPosFlags.b_IsCoarsePos))
      {
        z_lpc_control_data->u_sessBestFixValid = FALSE;
        MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LPC: Found cached fix as CPI and cleared LPC bestfix cache");
      }
    }
  }

  if (p_fixReport->z_NavPos.z_SmPosFlags.b_IsSensorPropogatedGNSSFix)
  {
    bSensorPropogatedGNSSFix = TRUE; /* INS/Dead Reckoning ONLY Fix */
  }

  MSG_2(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LPC: Received FIX REPORT from LDM BestAvailPos=%d INSOnlyFix=%d", (uint32)bBestAvailPos, (uint32)bSensorPropogatedGNSSFix);

  if (z_lpc_control_data->q_reportsPending & (uint32)LM_SESSION_REQUEST_FIX)
  {
    if (lpc_is_fix_source_reportable(p_fixReport, FALSE))
    {
      MSG(MSG_SSID_GPSSM, MSG_LEGACY_LOW, "LPC: Secondary client fix processing; fix reportable.");
      /*Save the fix report to be used when session timesout*/
      z_lpc_control_data->z_lastFixRpt = *p_fixReport;
      z_lpc_control_data->u_lastFixRptValid = TRUE;
      z_lpc_control_data->u_lastFixWasNoFixRpt = FALSE;

      /*Calculate Mag Deviation*/
      lpc_calculate_mag_deviation(p_fixReport);

      /*Save the lowest HEPE fix for a session*/
      lpc_set_sess_best_fix_report(p_fixReport);

      /* If E911 NILR MSB */
      if ((tm_core_get_emergency_support_config() & TM_E911_EARLY_EXIT_ENABLE) && /*If NV enabled for early exit logic for E911*/
          (b_e911_session == TRUE) &&
          (e_mode == LM_GPS_MODE_MSB) &&
          (e_mo_mt == LM_MO_MT_MODE_MT))
      {
        MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LPC: E911 NILR MSB fix processing in progress");

        /* Check if we qualify for an early exit */
        if ((z_lpc_control_data->b_gnssFixReceivedThisSession == TRUE) &&
            ((p_fixReport->z_NavPos.e_HoriRelIndicator >= PDSM_POSITION_RELIABILITY_MEDIUM) &&
             (p_fixReport->z_NavPos.e_HoriRelIndicator < PDSM_POSITION_RELIABILITY_MAX)) &&
            /* Send final fix only if its CPI or GNSS  */
            lpc_is_fix_source_reportable(p_fixReport, TRUE) &&
            /*HEPE of at least 50m should be met for E911 NILR MSB */
            lpc_position_accuracy_check(p_fixReport, LDM_CLIENT_ID_E911))
        {
          MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LPC: E911 NILR MSB early exit condition met. Reporting final position");
          /*Call Final fix handling?*/
          lpc_handle_final_position_report_secondary(p_zFixReport);
        }
        else
        {
          lpc_generate_log_packet(p_fixReport);
          /*Send diag position report to TM and generate NMEA if enabled*/
          /*Call intermediate fix handling?*/
          MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LPC: E911 fix processing. Reporting intemediate position");
          lpc_forward_position_report(p_zFixReport, FALSE);
        }
      }
      else if ((p_fixReport->z_NavPos.e_HoriRelIndicator != PDSM_POSITION_RELIABILITY_VERY_LOW) &&
               /* Verify position accuracy check only for non E911 cases */
               ((FALSE == b_e911_session) &&
                lpc_is_fix_source_reportable(p_fixReport, TRUE))
               )
      {
        if (lpc_position_accuracy_check(p_fixReport, serving_client_id))
        {
          /* handle final report (will do 0x1476 logging and do post-fix handling there) */
          lpc_handle_final_position_report_secondary(p_zFixReport);
        }
        else if (TRUE == bSensorPropogatedGNSSFix)
        {
          /* Calculate HEPE for INS ONLY Fix */
          INSOnlyFixHepe = (FLT)sqrt((p_fixReport->z_NavPos.f_ErrorEllipse[1] *
                                      p_fixReport->z_NavPos.f_ErrorEllipse[1]) +
                                     (p_fixReport->z_NavPos.f_ErrorEllipse[2] *
                                      p_fixReport->z_NavPos.f_ErrorEllipse[2]));

          MSG_2(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LPC:INS ONLY fix HEPE %d and INS/DR HEPE threshold %d", INSOnlyFixHepe, TM_LOC_INS_HEPE_THRESHOLD);
          /* If INS ONLY fix and HEPE <= 250m, send it out as final fix, even if it does not meet the dynamic HEPE check */
          if (INSOnlyFixHepe <= TM_LOC_INS_HEPE_THRESHOLD)
          {
            MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LPC:Reporting INS ONLY fix as FINAL Fix with HEPE %d, even if INS HEPE does not meet dynamic HEPE accuracy", INSOnlyFixHepe);
            /* handle final report (will do 0x1476 logging and do post-fix handling there) */
            lpc_handle_final_position_report_secondary(p_zFixReport);
          }
          else
          {
            MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LPC: Secondary client fix processing. generate log packets");
            lpc_generate_log_packet(p_fixReport);
            /*Send diag position report to TM and generate NMEA if enabled*/
            lpc_forward_position_report(p_zFixReport, FALSE);
          }
        }
        else
        {
          MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LPC: Secondary client fix processing. generate log packets");
          lpc_generate_log_packet(p_fixReport);
          /*Send diag position report to TM and generate NMEA if enabled*/
          lpc_forward_position_report(p_zFixReport, FALSE);
        }
      }
      else
      {
        MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LPC: Secondary client fix processing. generate log packets");
        lpc_generate_log_packet(p_fixReport);
        /*Send diag position report to TM and generate NMEA if enabled*/
        lpc_forward_position_report(p_zFixReport, FALSE);
      }
    }
    else
    {
      /* original NO_FIX report: Should not come here in concurrent mode */
      MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LPC: No fix report");
      lpc_generate_log_packet(p_fixReport);
      /*Send diag position report to TM and generate NMEA if enabled*/
      lpc_forward_position_report(p_zFixReport, FALSE);
    }
  }
  else
  {
    MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LPC: Secondary client FIX processing; no report pending");
  }
  return;
}

/*===========================================================================
FUNCTION lpc_timeout_finalfix_no_hepe_check

DESCRIPTION
    Determines if it is ok to send the final fix at fix timeout
    without any HEPE check constraint.
    This function is used during E911 session timeout handling
    when in concurrent mode.

DEPENDENCIES

INPUT PARAMS
    NONE

RETURN VALUE
    NONE

SIDE EFFECTS
===========================================================================*/
static boolean lpc_timeout_finalfix_no_hepe_check()
{
  lm_gps_mode_e_type            e_mode;
  lm_mo_mt_mode_e_type          e_mo_mt;
  /*Currently only E911 concurrency is allowed.*/
  tm_loc_processing_client_info *pz_loc_processing_client = lpc_get_loc_processing_client(LDM_CLIENT_ID_E911);

  if (NULL == pz_loc_processing_client)
  {
    MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LPC: NO HEPE check only for E911 secondary client");
    return FALSE;
  }

  if (NULL == z_lpc_control_data)
  {
    MSG(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "LPC:LPC control data not initialized or client not found");
    return FALSE;
  }

  MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LPC: Fix Timeout, op_mode %d,prtl_type %d, sess_handle %d]",
        pz_loc_processing_client->sess_params.op_mode,
        pz_loc_processing_client->prtl_type,
        pz_loc_processing_client->sessionHandle);

  e_mode = tm_core_convert_op_mode(pz_loc_processing_client->sess_params.op_mode,
                                   pz_loc_processing_client->prtl_type);
  e_mo_mt = lpc_mo_mt_mode(pz_loc_processing_client->sessionHandle);

  MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LPC: Fix Timeout, FinalFix No HEPE check [%d,%d,%d]",
        z_lpc_control_data->u_sessBestFixValid,
        e_mode,
        e_mo_mt);

  /*If there was no fix received in this session*/
  if (FALSE == (z_lpc_control_data->u_sessBestFixValid))
  {
    MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LPC: lpc_timeout_finalfix_no_hepe_check returned false");
    return FALSE;
  }

  /*Only for MSB session which are originated by the N/W*/
  if ((LM_GPS_MODE_MSB != e_mode) ||
      (LM_MO_MT_MODE_MT != e_mo_mt))
  {
    MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LPC: lpc_timeout_finalfix_no_hepe_check returned false");
    return FALSE;
  }

  MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LPC: lpc_timeout_finalfix_no_hepe_check returned TRUE");
  return TRUE;
}

/*===========================================================================
FUNCTION lpc_session_timeout_handler

DESCRIPTION
    Timeout handler function for LPC fix/measurement timer.
    This function is used when in concurrent mode.

DEPENDENCIES

INPUT PARAMS
    NONE

RETURN VALUE
    NONE
SIDE EFFECTS
===========================================================================*/
static void lpc_session_timeout_handler()
{
  boolean b_reportError = FALSE;
  int prtlTableIndex;
  tm_sess_stop_reason_e_type stop_reason;
  ldm_client_id_e_type e_main_client_id = LDM_CLIENT_ID_MAX;
  ldm_client_id_e_type e_secondary_client_id = LDM_CLIENT_ID_MAX;

  MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LPC timer (FIX/GPS) timer expired");

  if (NULL != z_lpc_control_data)
  {
    /*If there is no PRM/FIX report pending, this timeout was queued just when
      timer was stopped. Do nothing in this case*/
    if (z_lpc_control_data->q_reportsPending & (uint32)LM_SESSION_REQUEST_FIX)
    {
      /*Update fix session related state*/
      z_lpc_control_data->q_reportsPending &= ~((uint32)LM_SESSION_REQUEST_FIX);

      tm_lpc_get_main_secondary_clients(&e_main_client_id, &e_secondary_client_id);

      if (LDM_CLIENT_ID_APPTRK == e_secondary_client_id)
      {
        MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LPC timer expired for Apptrack as secondary");
        return;
      }

      if ((z_lpc_control_data->u_lastFixRptValid) &&
          lpc_position_accuracy_check(&(z_lpc_control_data->z_lastFixRpt), z_serving_client_data.serving_client_id) &&
          /*Do not send RefLoc position back as final fix for E911 NILR MSB. Only CPI/GNSS fix should be sent */
          lpc_is_fix_source_reportable(&(z_lpc_control_data->z_lastFixRpt), TRUE))
      {
        MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LPC:LPC timeout handling: Sending last fix as final at timeout");
        /*This fix report should not be used again once it is reported*/
        z_lpc_control_data->u_lastFixRptValid = FALSE;
        z_lpc_control_data->u_lastFixWasNoFixRpt = FALSE;
        /* session has ended, so reset this flag */
        z_lpc_control_data->b_gnssFixReceivedThisSession = FALSE;
        lpc_handle_final_fix_report(&(z_lpc_control_data->z_lastFixRpt), z_serving_client_data.serving_client_id);
      }
      else if (lpc_timeout_finalfix_no_hepe_check())
      {
        MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LPC:LPC timeout handling: Sending MT MSB best fix as final at timeout");
        z_lpc_control_data->u_sessBestFixValid = FALSE;
        /* session has ended, so reset this flag */
        z_lpc_control_data->b_gnssFixReceivedThisSession = FALSE;
        lpc_handle_final_fix_report(&(z_lpc_control_data->z_sessBestFixRpt), z_serving_client_data.serving_client_id);
      }
      else
      {
        b_reportError = TRUE;
      }
    }
    else if (z_lpc_control_data->q_reportsPending & (uint32)LM_SESSION_REQUEST_GPS)
    {
      z_lpc_control_data->q_reportsPending &= ~((uint32)LM_SESSION_REQUEST_GPS);
      b_reportError = TRUE;
    }
    else
    {
      MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LPC:LPC timer timeout: timeout queued just when timer was stopped");
    }

    if (TRUE == b_reportError)
    {
      MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LPC:LPC timer timeout: report timeout error");
      if (lp_clients[z_serving_client_data.serving_client_id].is_enabled == TRUE)
      {
        MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LPC timer timeout: Secondary client enabled");
        prtlTableIndex = tm_core_get_prtl_idx_by_type(lp_clients[z_serving_client_data.serving_client_id].prtl_type);

        if ((prtlTableIndex > -1) && (prtlTableIndex < TM_PRTL_NUM))
        {
          /*Set stop reason and call protocol stop.*/
          if (tm_core_info.prtl_func_cb_table[prtlTableIndex].stop_sess_req_fp)
          {
            MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LPC:Sending TIMEOUT error to protocol %d", lp_clients[z_serving_client_data.serving_client_id].prtl_type);

            if ((lp_clients[z_serving_client_data.serving_client_id].sess_params.op_req) & TM_OP_REQ_PRM)
            {
              MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LPC:PRM timer popped");
              stop_reason = TM_STOP_REASON_PRM_TIMEOUT;
            }
            else if ((lp_clients[z_serving_client_data.serving_client_id].sess_params.op_req) & TM_OP_REQ_LR)
            {
              MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LPC:FIX timer popped");
              stop_reason = TM_STOP_REASON_FIX_TIMEOUT;
            }
            else
            {
              MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LPC:Unknown error");
              stop_reason = TM_STOP_REASON_GENERAL_ERROR;
            }
            tm_lpc_report_error_secondary(stop_reason, z_serving_client_data.serving_client_id);
          }
          else
          {
            MSG(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "LPC:No protocol callback installed 4");
            tm_lpc_report_error_secondary(TM_STOP_REASON_GENERAL_ERROR, z_serving_client_data.serving_client_id);
          }
        }
        else
        {
          MSG(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "Invalid prtl index");
        }
      }
      else
      {
        MSG(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "LPC timer timeout: Secondary client not enabled");
      }
    }
  }
  else
  {
    MSG(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "LPC:LPC timer timeout: concurrent session not initialized");
    return;
  }

}

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
boolean tm_lpc_init()
{
  boolean retVal = FALSE;
  ldm_client_id_e_type e_client_id = LDM_CLIENT_ID_MAX;

  MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LPC:processing client init called ");

  if (LDM_OP_STATUS_REQUEST_SUCCESS == ldm_init())
  {
    z_cb_functions.p_cb_func = tm_lpc_cb_handler;
    retVal = TRUE;
  }

  for (e_client_id = LDM_CLIENT_ID_FIRST; e_client_id <= LDM_CLIENT_ID_LAST; e_client_id++)
  {
    memset(&(lp_clients[e_client_id]), 0, sizeof(lp_clients[e_client_id]));
    lp_clients[e_client_id].is_enabled = FALSE;
  }

  MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LPC:processing client init done :%d ", retVal);
  return retVal;
}


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
boolean tm_lpc_register_session_client(const ldm_client_id_e_type e_client_id)
{
  ldm_op_status_e_type e_result;

  e_result = ldm_client_register(&z_cb_functions, e_client_id);

  if (LDM_OP_STATUS_REQUEST_SUCCESS == e_result)
  {
    MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LPC:Client %d registered successfully ", e_client_id);
    return TRUE;
  }
  else if (LDM_OP_STATUS_ERROR_CLIENT_ALREADY_REGISTERED == e_result)
  {
    MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LPC:Client %d already registered", e_client_id);
    return FALSE;
  }
  else
  {
    MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LPC:Client %d registeration failed", e_client_id);
    return FALSE;
  }
}

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
boolean tm_lpc_deregister_session_client(const ldm_client_id_e_type e_client_id)
{
  ldm_op_status_e_type e_result;

  e_result = ldm_client_deregister(e_client_id);

  if (LDM_OP_STATUS_REQUEST_SUCCESS == e_result)
  {
    MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LPC:Client %d deregisterated", e_client_id);
    return TRUE;
  }
  else
  {
    MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LPC:Client %d de-registeration failed", e_client_id);
    return FALSE;
  }

}


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
=======================
===========================================================================*/
boolean tm_lpc_register_session_client_all(void)
{
  ldm_client_id_e_type e_client_id = LDM_CLIENT_ID_MAX;
  boolean v_regFailed = FALSE;

  for (e_client_id = LDM_CLIENT_ID_FIRST; e_client_id <= LDM_CLIENT_ID_LAST; e_client_id++)
  {
    v_regFailed = tm_lpc_register_session_client(e_client_id);

    if (!v_regFailed)
    {
      MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "LPC:tm_lpc_register_session_client: client %u registration failed", e_client_id);
    }
  }

  return v_regFailed;
}

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
boolean tm_lpc_deregister_session_client_all(void)
{
  ldm_client_id_e_type e_client_id = LDM_CLIENT_ID_MAX;
  boolean v_regFailed = FALSE;

  for (e_client_id = LDM_CLIENT_ID_FIRST; e_client_id <= LDM_CLIENT_ID_LAST; e_client_id++)
  {
    v_regFailed = tm_lpc_deregister_session_client(e_client_id);

    if (!v_regFailed)
    {
      MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "LPC:tm_lpc_deregister_session_client: client %u registration failed", e_client_id);
    }
  }

  return v_regFailed;
}

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
pdsm_client_id_type tm_lpc_get_pdsm_client_id_for_ldm_client(ldm_client_id_e_type  e_client_id)
{
  MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LPC:Get pdsm client id corresponding to LDM client %d", e_client_id);
  if (TRUE == (lp_clients[e_client_id].is_enabled))
  {
    MSG_2(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LPC:LDM client %d found in DB. Return PDSM client id %d", e_client_id, lp_clients[e_client_id].pd_clientId);
    return (lp_clients[e_client_id].pd_clientId);
  }
  else
  {
    MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LPC:LDM client %d not found in DB", e_client_id);
    return (PDSM_CLIENT_TYPE_NONE);
  }
}

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
tm_sess_handle_type tm_lpc_get_sess_handle_for_ldm_client(ldm_client_id_e_type  e_client_id)
{
  MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LPC:Get session handle corresponding to LDM client %d", e_client_id);
  if (TRUE == (lp_clients[e_client_id].is_enabled))
  {
    MSG_2(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LPC:LDM client %d found in DB. Return sessionHandle %d", e_client_id, lp_clients[e_client_id].sessionHandle);
    return (lp_clients[e_client_id].sessionHandle);
  }
  else
  {
    MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LPC:LDM client %d not found in DB", e_client_id);
    return (TM_SESS_HANDLE_NONE);
  }
}

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
                               tm_sess_req_start_param_s_type  *p_z_sess_start_param)
{
  tm_loc_processing_client_info *pz_client_info;

  MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LPC:Return the client info for client ID %d", e_req_client_id);

  pz_client_info = lpc_get_loc_processing_client(e_req_client_id);

  if (NULL == pz_client_info)
  {
    MSG(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "LPC:Client not found in database");
    return FALSE;
  }

  if ((NULL == p_prtl_type) ||
      (NULL == p_sess_handle) ||
      (NULL == p_z_sess_start_param))
  {
    MSG(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "LPC:NULL pointer passed");
    return FALSE;
  }

  *p_prtl_type = pz_client_info->prtl_type;
  *p_sess_handle = pz_client_info->sessionHandle;
  memscpy((void *)p_z_sess_start_param, sizeof(*p_z_sess_start_param),
          (void *)(&(pz_client_info->sess_params)), sizeof(pz_client_info->sess_params));

  MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LPC:Found LDM client %d; sending required info", e_req_client_id);
  return TRUE;

}

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
                                          ldm_client_e_type                e_client_type)
{
  uint8 q_request_mask = 0;
  uint32 q_ldm_cli_state_mask;
  lpc_op_status_e_type e_req_status = LPC_OP_STATUS_REQUEST_FAILURE;
  boolean retVal = FALSE;
  tm_cm_phone_state_info_s_type phone_state_info;
  uint32 q_timeout = 0;

  /*Get the phone state to check for concurrency*/
  memset(&phone_state_info, 0, sizeof(tm_cm_phone_state_info_s_type));
  tm_cm_iface_get_phone_state(&phone_state_info);


  q_ldm_cli_state_mask = lpc_map_client_id_to_status_mask(e_req_client_id);

  if ((ldm_client_query_state() & q_ldm_cli_state_mask) != 0)
  {
    MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LPC: Client %d already enabled in LDM", e_req_client_id);
    /*If client is already enabled and the request is for apptrack 
      update the Apptrack client info in database*/
    if (LDM_CLIENT_ID_APPTRK == e_req_client_id)
    {
      tm_sess_req_continue_param_s_type z_sess_continue_req_param;
      MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LPC: Call update for AppTrack Client %d", e_req_client_id);
      z_sess_continue_req_param.op_mode = sess_start_param->op_mode;
      z_sess_continue_req_param.op_req = sess_start_param->op_req;
      z_sess_continue_req_param.num_fixes = sess_start_param->num_fixes;
      z_sess_continue_req_param.tbf_ms = sess_start_param->tbf_ms;
      z_sess_continue_req_param.accuracy_threshold = sess_start_param->accuracy_threshold;
      z_sess_continue_req_param.ppm_qos = sess_start_param->ppm_qos;
      z_sess_continue_req_param.prm_qos = sess_start_param->prm_qos;
      z_sess_continue_req_param.lr_qos = sess_start_param->lr_qos;
      z_sess_continue_req_param.prm_prq = sess_start_param->prm_prq;
      z_sess_continue_req_param.dynamic_qos_enabled = sess_start_param->dynamic_qos_enabled;
      z_sess_continue_req_param.multi_report_msa = sess_start_param->multi_report_msa;
      z_sess_continue_req_param.specialReqType = sess_start_param->specialReqType;

      if (LPC_OP_STATUS_REQUEST_SUCCESS == tm_lpc_update_client(e_req_client_id,
                                                                prtl_type,
                                                                sess_handle,
                                                                &(z_sess_continue_req_param)))
      {
        MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LPC: Client %d successfully update in client DB", e_req_client_id);
        e_req_status = LPC_OP_STATUS_REQUEST_SUCCESS;
      }
      else
      {
        MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "LPC: Client %d update failed", e_req_client_id);
        e_req_status = LPC_OP_STATUS_REQUEST_FAILURE;
      }
    }
    else
    {
      MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "LPC: Return error as client %d already enabled", e_req_client_id);
      e_req_status = LPC_OP_STATUS_REQUEST_FAILURE;
    }
  }
  else
  {
    MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LPC: New Client %d; to be enabled in LDM", e_req_client_id);

    /*Get subscription mask to be update in LDM for this client*/
    if ((TM_SESS_OPERATION_MODE_MSB == sess_start_param->op_mode) ||
        (TM_SESS_OPERATION_MODE_STANDALONE == sess_start_param->op_mode) ||
        (TM_SESS_OPERATION_MODE_FTM == sess_start_param->op_mode) ||
        (TM_SESS_OPERATION_MODE_ODP == sess_start_param->op_mode) ||
        (TM_SESS_OPERATION_MODE_MSB_OPT == sess_start_param->op_mode))
    {
      MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_MED,
            "LPC: Opmode = %d. Subscribing for measurements and fixes", sess_start_param->op_mode);
      q_request_mask |= LDM_CLIENT_MEAS_MASK;
      q_request_mask |= LDM_CLIENT_FIX_MASK;
    }
    else if ((TM_SESS_OPERATION_MODE_MSA == sess_start_param->op_mode) ||
             (TM_SESS_OPERATION_MODE_PERIODIC_PPM == sess_start_param->op_mode) ||
             (TM_SESS_OPERATION_MODE_OTDOA == sess_start_param->op_mode))
    {
      MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_MED,
            "LPC: Opmode = %d. Subscribing for measurements only", sess_start_param->op_mode);
      q_request_mask |= LDM_CLIENT_MEAS_MASK;
    }

    /*Enable the client at LDM*/
    if (LDM_OP_STATUS_ACCESS_GRANT == ldm_client_enable_request(e_req_client_id, q_request_mask, e_client_type))
    {
      /*Successfully enabled the client for LR/PRM request*/
      MSG_2(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LPC: Enabled %d client for %d request", e_req_client_id, q_request_mask);
      /*Add client info to database*/
      if (TRUE == lpc_add_loc_processing_client(e_req_client_id,
                                                prtl_type,
                                                sess_handle,
                                                sess_start_param,
                                                pd_client_id,
                                                e_client_type))
      {
        MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LPC: Client %d successfully added to client DB", e_req_client_id);
        e_req_status = LPC_OP_STATUS_REQUEST_SUCCESS;
      }
      else
      {
        MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "LPC: Client %d cannot be added to client DB", e_req_client_id);
        e_req_status = LPC_OP_STATUS_REQUEST_FAILURE;
      }

      if (LPC_OP_STATUS_REQUEST_SUCCESS == e_req_status)
      {
        if (LDM_CLIENT_ID_E911 == e_req_client_id)
        {
          /*Notify MGP about the E911 session start*/
          MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LPC: Notify MGP about the E911 session start");
          (void)mgp_E911SessionIndPut(LPC_CONCURRENT_E911_SESSION_START);

          /* Start CPI Request for E911 (main and secondary clients) */
          tm_core_start_CPI(prtl_type, sess_handle, sess_start_param->specialReqType.e_SpecialReq);
        }

        if ((LDM_CLIENT_ID_SINGLE_SHOT == e_req_client_id) &&
            (TRUE == tm_agps_emergency_call_simulate()) &&
            ((TM_SESS_HANDLE_LPP_CP_NI == sess_handle) ||
             (TM_SESS_HANDLE_UMTS_UP_NI == sess_handle)))
        {
          MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LPC: Request CPI when simulation E911 NV enabled for handle %d", sess_handle);
          /* Start CPI Request for Single Shot client when simulation E911 NV enabled */
          tm_core_start_CPI(prtl_type, sess_handle, sess_start_param->specialReqType.e_SpecialReq);
        }

        /*Special Reqs are for APPTRACKING and SINGLE SHOT clients only 
        For ShortCodes SpecialRequest we initiate CPI injection*/
        if ( PDSM_SESSION_SPECIAL_REQ_SHORT_CODES == sess_start_param->specialReqType.e_SpecialReq )
        {
          MSG_2(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LPC: Sess Handle: %d, Request CPI Special Request Handling %d", 
                sess_handle,sess_start_param->specialReqType.e_SpecialReq);
          tm_core_start_CPI(prtl_type, sess_handle, sess_start_param->specialReqType.e_SpecialReq);
        }

        /*If we are in concurrent mode and the client to be enabled is secondary; 
                    intilize all the session parameters and fix and measurement qualifications. 
                    For concurrent mode we will mimic LM session here*/
        if ((TM_CM_E911_STATE_CONCURRENT == phone_state_info.e911_state) && (LDM_CLIENT_TYPE_SECONDARY == e_client_type))
        {
          MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LPC: In concurrent mode, initialize session params for secondary client %d", e_req_client_id);

          if ((TRUE == lpc_init_control_data(e_req_client_id)) &&
              (NULL != z_lpc_control_data))
          {
            /*Successfully allocated memory for control data structure*/
            /*Update it with the session info*/

            z_lpc_control_data->q_sessHandle = sess_handle;

            if (LDM_CLIENT_ID_E911 == e_req_client_id)
            {
              if (sess_start_param->op_req & TM_OP_REQ_PRM)
              {
                q_timeout = sess_start_param->prm_qos * 1000;
              }

              if (sess_start_param->op_req & TM_OP_REQ_LR)
              {
                q_timeout = sess_start_param->lr_qos * 1000;
              }
            }
            else if (LDM_CLIENT_ID_APPTRK == e_req_client_id)
            {
              q_timeout = TM_LOC_FIX_RESPONSE_TIME_MS_DEFAULT;
            }
            else
            {
              MSG(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "LPC: Failed to get client id");
            }
            if(q_timeout != 0)
            {
              MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LPC: Start fix/PRM timer with %d", q_timeout);
              if (!os_TimerStart(z_lpc_control_data->p_location_Timer, q_timeout, 0))
              {
                MSG(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "LPC: Failed starting Fix timer");
                return FALSE;
              }
            }
            else
            {
              MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LPC: Do not start timer with value 0");
            }
            if ((TM_SESS_OPERATION_MODE_MSB == sess_start_param->op_mode) ||
                (TM_SESS_OPERATION_MODE_STANDALONE == sess_start_param->op_mode) ||
                (TM_SESS_OPERATION_MODE_FTM == sess_start_param->op_mode) ||
                (TM_SESS_OPERATION_MODE_ODP == sess_start_param->op_mode) ||
                (TM_SESS_OPERATION_MODE_MSB_OPT == sess_start_param->op_mode))
            {
              z_lpc_control_data->q_reportsPending |= (uint32)LM_SESSION_REQUEST_FIX;
            }
            else if ((TM_SESS_OPERATION_MODE_MSA == sess_start_param->op_mode) ||
                     (TM_SESS_OPERATION_MODE_PERIODIC_PPM == sess_start_param->op_mode) ||
                     (TM_SESS_OPERATION_MODE_OTDOA == sess_start_param->op_mode))
            {
              z_lpc_control_data->q_reportsPending |= (uint32)LM_SESSION_REQUEST_GPS;
            }
          } /*end if (TRUE == lpc_init_control_data(e_req_client_id))*/
          else
          {
            MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "LPC: In concurrent mode failed to initialize session params for Secondary client %d", e_req_client_id);
            e_req_status = LPC_OP_STATUS_REQUEST_FAILURE;
          }
        } /*end if(TM_CM_E911_STATE_CONCURRENT == phone_state_info.e911_state)*/
        else
        {
          /*Not in concurrent mode; fix and measurement handling done in LM*/
          MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LPC: Not In concurrent mode. LPC control data not updated");
        }

      }
    }
    else
    {
      MSG_2(MSG_SSID_GPSSM, MSG_LEGACY_MED,
            "LPC: Enabling %d client for %d request failed", e_req_client_id, q_request_mask);
      e_req_status = LPC_OP_STATUS_REQUEST_FAILURE;
    }
  }

  return e_req_status;
}

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
lpc_op_status_e_type tm_lpc_disable_client(const ldm_client_id_e_type  e_client_id)
{
  lpc_op_status_e_type e_req_status = LPC_OP_STATUS_REQUEST_FAILURE;
  uint32 q_ldm_cli_state_mask;
  tm_cm_phone_state_info_s_type phone_state_info;
  tm_loc_processing_client_info *pz_client_info = lpc_get_loc_processing_client(e_client_id);
  ldm_client_e_type  e_clientId_type = LDM_CLIENT_TYPE_MAX;

  if (NULL == pz_client_info)
  {
    MSG(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "LPC:Client not found to in database");
    return FALSE;
  }

  e_clientId_type =  pz_client_info->e_clientId_type;

  MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LPC: Disable %d client", e_client_id);

  memset(&phone_state_info, 0, sizeof(tm_cm_phone_state_info_s_type));
  tm_cm_iface_get_phone_state(&phone_state_info);

  q_ldm_cli_state_mask = lpc_map_client_id_to_status_mask(e_client_id);

#if !defined ( FEATURE_GNSS_SA ) && !defined ( FEATURE_GNSS_LEAN_BUILD )
  if ( PDSM_SESSION_SPECIAL_REQ_SHORT_CODES == pz_client_info->sess_params.specialReqType.e_SpecialReq)
  {
    MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LPC: Stopping CPI Special Code ");
    /* Stop ODCPI inject if started earlier */
    tm_stopCPI(TM_AUXTECH_REQ_ID_EMERGENCY_ORIG);
  }
#endif /* ! FEATURE_GNSS_SA && ! FEATURE_GNSS_LEAN_BUILD */

  if ((ldm_client_query_state() & q_ldm_cli_state_mask) != 0)
  {
    MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LPC: Client %d  enabled in LDM; go and disable", e_client_id);

    if (LDM_OP_STATUS_REQUEST_SUCCESS == ldm_client_disable_request(e_client_id))
    {
      MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LPC: Successfully disabled %d client frpm LDM", e_client_id);
      if (TRUE == lpc_del_loc_processing_client(e_client_id))
      {
        MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LPC: %d client deleted from location processing client list", e_client_id);
        e_req_status = LPC_OP_STATUS_REQUEST_SUCCESS;
      }
      else
      {
        MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "LPC:deleting %d client from LP list returned error", e_client_id);
      }
    }

    if (LDM_CLIENT_ID_E911 == e_client_id)
    {
      /*Notify MGP about E911 session end in concurrent mode*/
      MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LPC: Notify MGP about E911 session end in concurrent mode!");
      (void)mgp_E911SessionIndPut(0);
    }

    if (LDM_CLIENT_TYPE_SECONDARY == e_clientId_type)
    {
      if (NULL != z_lpc_control_data)
      {
        MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LPC: Stop timer and clear control data  for Secondary client!");
        /*Stop timer*/
        os_TimerStop(z_lpc_control_data->p_location_Timer);
        os_TimerDestroy(z_lpc_control_data->p_location_Timer);
        (void)os_MemFree((void **)&z_lpc_control_data);
        z_lpc_control_data = NULL;
      }
    }
  }
  else
  {
    MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "LPC: Client %d  is already disabled in LDM, returning success", e_client_id);
    e_req_status = LPC_OP_STATUS_REQUEST_SUCCESS;
  }
  return e_req_status;
}

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
                                          tm_sess_req_continue_param_s_type  *sess_cont_param)
{
  tm_loc_processing_client_info    *p_z_lpc_client;
  lpc_op_status_e_type e_req_status = LPC_OP_STATUS_REQUEST_FAILURE;
  boolean retVal = FALSE;
  tm_cm_phone_state_info_s_type   phone_state_info;
  uint8                           q_request_mask = 0;
  uint32                          q_timeout;
  ldm_client_e_type               e_client_type = LDM_CLIENT_TYPE_MAX;

  MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LPC:Update request for %d LDM client", e_req_client_id);
  p_z_lpc_client = lpc_get_loc_processing_client(e_req_client_id);

  if (NULL == p_z_lpc_client)
  {
    MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "LPC: client %d not found for update", e_req_client_id);
    return e_req_status;
  }

  e_client_type = p_z_lpc_client->e_clientId_type;

  memset(&phone_state_info, 0, sizeof(tm_cm_phone_state_info_s_type));
  tm_cm_iface_get_phone_state(&phone_state_info);

  if ((TM_SESS_OPERATION_MODE_MSB == sess_cont_param->op_mode) ||
      (TM_SESS_OPERATION_MODE_STANDALONE == sess_cont_param->op_mode) ||
      (TM_SESS_OPERATION_MODE_FTM == sess_cont_param->op_mode) ||
      (TM_SESS_OPERATION_MODE_ODP == sess_cont_param->op_mode) ||
      (TM_SESS_OPERATION_MODE_MSB_OPT == sess_cont_param->op_mode))
  {
    MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LPC: Opmode = %d. Subscribing for measurements and fixes", sess_cont_param->op_mode);
    q_request_mask |= LDM_CLIENT_MEAS_MASK;
    q_request_mask |= LDM_CLIENT_FIX_MASK;
  }
  else if ((TM_SESS_OPERATION_MODE_MSA == sess_cont_param->op_mode) ||
           (TM_SESS_OPERATION_MODE_PERIODIC_PPM == sess_cont_param->op_mode) ||
           (TM_SESS_OPERATION_MODE_OTDOA == sess_cont_param->op_mode))
  {
    MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LPC: Opmode = %d. Subscribing for measurements only", sess_cont_param->op_mode);
    q_request_mask |= LDM_CLIENT_MEAS_MASK;
  }

  if (LDM_OP_STATUS_REQUEST_SUCCESS == ldm_client_update_subscription(e_req_client_id, q_request_mask))
  {
    /*Successfully enabled the client for LR/PRM request*/
    MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LPC: Calling update for client %d", e_req_client_id);

    /*Now maintian the copy of session parameters*/

    if (TRUE == lpc_update_loc_processing_client(e_req_client_id,
                                                 prtl_type,
                                                 sess_handle,
                                                 sess_cont_param))
    {
      MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LPC: Client %d successfully updated in client DB", e_req_client_id);
      e_req_status = LPC_OP_STATUS_REQUEST_SUCCESS;

      if (LDM_CLIENT_ID_E911 == e_req_client_id)
      {
        /* Start CPI Request for E911 (main and secondary clients) */
        tm_core_start_CPI(prtl_type, sess_handle, sess_cont_param->specialReqType.e_SpecialReq);
      }
          
      /*Special Reqs are for APPTRACKING and SINGLE SHOT clients only 
        For ShortCodes SpecialRequest we initiate CPI injection*/
      if ( ( sess_cont_param->specialReqType.v_specialReqValid ) &&
             (PDSM_SESSION_SPECIAL_REQ_SHORT_CODES == sess_cont_param->specialReqType.e_SpecialReq ) )
      {
        tm_core_start_CPI(prtl_type, sess_handle, sess_cont_param->specialReqType.e_SpecialReq);
      }
        }
    else
    {
      MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "LPC: Client %d cannot be updated", e_req_client_id);
      e_req_status = LPC_OP_STATUS_REQUEST_FAILURE;
    }

    /*If we are in concurrent mode and the client to be enabled is secondary client; 
            intilize all the session parameters for this client for fix and measurement qualifications.
         */
    if ((LPC_OP_STATUS_REQUEST_SUCCESS == e_req_status) &&
        (TM_CM_E911_STATE_CONCURRENT == phone_state_info.e911_state) &&
        (LDM_CLIENT_TYPE_SECONDARY == e_client_type) &&
        (NULL != z_lpc_control_data))
    {

      MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LPC: In concurrent mode update session params for %d client", e_req_client_id);

      z_lpc_control_data->q_sessHandle = sess_handle;

      if (sess_cont_param->op_req & TM_OP_REQ_PRM)
      {
        MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LPC: Session update; got TM_OP_REQ_PRM request;qos %u", sess_cont_param->prm_qos);
        q_timeout = sess_cont_param->prm_qos * 1000;
      }

      if (sess_cont_param->op_req & TM_OP_REQ_LR)
      {
        MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LPC: Session update; got TM_OP_REQ_LR request;qos %u", sess_cont_param->lr_qos);
        q_timeout = sess_cont_param->lr_qos * 1000;
      }

      if (LDM_CLIENT_ID_APPTRK == e_req_client_id)
      {
        q_timeout = TM_LOC_FIX_RESPONSE_TIME_MS_DEFAULT;
        sess_cont_param->op_req = TM_OP_REQ_LR;
      }

      if ((sess_cont_param->op_req & TM_OP_REQ_LR) ||
          (sess_cont_param->op_req & TM_OP_REQ_PRM))
      {
        if (q_timeout != 0)
        {
          MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LPC: ReStart timer with %d", q_timeout);
          if (TRUE == os_TimerStop(z_lpc_control_data->p_location_Timer))
        {
            MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LPC: Stopped timer; start again! ");
          if (!os_TimerStart(z_lpc_control_data->p_location_Timer, q_timeout, 0))
          {
              MSG(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "LPC: Failed restarting timer");
            return LPC_OP_STATUS_REQUEST_FAILURE;
          }
          z_lpc_control_data->q_reportsPending |= (uint32)LM_SESSION_REQUEST_FIX;
        }
        else
        {
            MSG(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "LPC: Failed to stop timer");
            return LPC_OP_STATUS_REQUEST_FAILURE;
          }
        }
        else
        {
          MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LPC: Do not start timer with 0 value");
        }
      }
        }
      }
        else
        {
    MSG(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "LPC: failed to update client subscription in LDM");
    e_req_status = LPC_OP_STATUS_REQUEST_FAILURE;
        }

  return e_req_status;

    }

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
                                      boolean is_no_fix)
{
  tm_CalendarTimeT z_Time;
  uint32 q_TotalDays = 0;
  boolean b_reportNMEA = TRUE;
  uint32 q_GnssMask = sm_get_supported_constellation_mask();

  TM_MED_1("LPC:Generate FixRelatedNMEA. Constellation mask:%d", q_GnssMask);
  if (z_fix == NULL)
  {
    return;
  }
    if (TRUE == tm_core_info.config_info.u_gnss_second_boundary_aligned_NMEA) 
    {
    MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LPC:NV for second boundary aligned NMEA is set");
    if ((FALSE == z_fix->z_NavPos.u_FirstFix))
      {        
         if (tm_util_get_fix_time(z_fix, &z_Time, &q_TotalDays))
         {
        if (z_Time.q_Millisecond > TM_CORE_MAX_ALLOWABLE_MS_FOR_SECOND_BOUNDARY)
            {
          MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LPC:dont report non-second boundary aligned");
                b_reportNMEA = FALSE;	
            }
         }
         else
         {
        MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LPC:tm_util_get_fix_time is failed");
            return;
         }
      }
    }
  if (FALSE == b_reportNMEA)
  {
     return;
  }  


  //DTM is sent one at the beginning
  if (q_GnssMask & C_RCVR_GNSS_CONFIG_GPS_ENABLED)
  {
    TM_CORE_OUTPUT_NMEA_GPDTM(z_fix, is_no_fix);
  }

  TM_CORE_OUTPUT_NMEA_GNDTM(z_fix, is_no_fix);

  //GGA Sentences
  if (q_GnssMask & C_RCVR_GNSS_CONFIG_GPS_ENABLED)
  {
    TM_CORE_OUTPUT_NMEA_GPGGA(z_fix, is_no_fix);
  }

  if (q_GnssMask & C_RCVR_GNSS_CONFIG_GAL_ENABLED)
  {
    TM_CORE_OUTPUT_NMEA_GAGGA(z_fix, is_no_fix);
  }

  if (q_GnssMask & C_RCVR_GNSS_CONFIG_BDS_ENABLED)
  {
    TM_CORE_OUTPUT_NMEA_GBGGA(z_fix, is_no_fix);
  }

  TM_CORE_OUTPUT_NMEA_GNGGA(z_fix, is_no_fix);

  /*If reporting position in P90 Datum, DTM is sent prior to every positional sentence.
    GGA, GNS and RMC are the positional sentences */
  if ((FALSE == is_no_fix) && (TM_CORE_CONFIG_DD(u_control_nmea_sentence) & PDSM_PA_PZ90_DATUM_NMEA_SENTENCE))
  {
    if (q_GnssMask & C_RCVR_GNSS_CONFIG_GPS_ENABLED)
    {
      TM_CORE_OUTPUT_NMEA_GPDTM(z_fix, is_no_fix);
    }

    TM_CORE_OUTPUT_NMEA_GNDTM(z_fix, is_no_fix);
    }

  //GNS Sentences
  TM_CORE_OUTPUT_NMEA_GNGNS(z_fix, is_no_fix);

  if (q_GnssMask & (C_RCVR_GNSS_CONFIG_BDS_ENABLED | C_RCVR_GNSS_CONFIG_QZSS_ENABLED))
  {
    TM_CORE_OUTPUT_NMEA_PQXFI(z_fix, is_no_fix);
  }

  //VTG Sentences
  if (q_GnssMask & C_RCVR_GNSS_CONFIG_GPS_ENABLED)
  {
    TM_CORE_OUTPUT_NMEA_GPVTG((is_no_fix == TRUE) ? NULL : z_fix);
  }

  if (q_GnssMask & C_RCVR_GNSS_CONFIG_GAL_ENABLED)
  {
    TM_CORE_OUTPUT_NMEA_GAVTG((is_no_fix == TRUE) ? NULL : z_fix);
  }

  if (q_GnssMask & C_RCVR_GNSS_CONFIG_BDS_ENABLED)
  {
    TM_CORE_OUTPUT_NMEA_GBVTG((is_no_fix == TRUE) ? NULL : z_fix);
  }

  TM_CORE_OUTPUT_NMEA_GNVTG((is_no_fix == TRUE) ? NULL : z_fix);

  /*If reporting position in P90 Datum, DTM is sent prior to every positional sentence.
    GGA, GNS and RMC are the positional sentences */
  if ((FALSE == is_no_fix) && (TM_CORE_CONFIG_DD(u_control_nmea_sentence) & PDSM_PA_PZ90_DATUM_NMEA_SENTENCE))
    {
    if (q_GnssMask & C_RCVR_GNSS_CONFIG_GPS_ENABLED)
    {
		  TM_CORE_OUTPUT_NMEA_GNDTM(z_fix, is_no_fix);
    }
      TM_CORE_OUTPUT_NMEA_GPDTM(z_fix, is_no_fix);
  }

  if (q_GnssMask & C_RCVR_GNSS_CONFIG_GPS_ENABLED)
  {
    TM_CORE_OUTPUT_NMEA_GPRMC((is_no_fix == TRUE) ? NULL : z_fix);
  }

  //RMC Sentences
  if (q_GnssMask & C_RCVR_GNSS_CONFIG_GAL_ENABLED)
  {
    TM_CORE_OUTPUT_NMEA_GARMC((is_no_fix == TRUE) ? NULL : z_fix);
  }

  if (q_GnssMask & C_RCVR_GNSS_CONFIG_BDS_ENABLED)
  {
    TM_CORE_OUTPUT_NMEA_GBRMC((is_no_fix == TRUE) ? NULL : z_fix);
  }

  TM_CORE_OUTPUT_NMEA_GNRMC((is_no_fix == TRUE) ? NULL : z_fix);

  //GSA Sentences
  if (q_GnssMask & C_RCVR_GNSS_CONFIG_GPS_ENABLED)
  {
    TM_CORE_OUTPUT_NMEA_GPGSA((is_no_fix == TRUE) ? NULL : z_fix);
  }

  if (q_GnssMask & C_RCVR_GNSS_CONFIG_GAL_ENABLED)
  {
    TM_CORE_OUTPUT_NMEA_GAGSA((is_no_fix == TRUE) ? NULL : z_fix);
  }

  if (q_GnssMask & (C_RCVR_GNSS_CONFIG_BDS_ENABLED | C_RCVR_GNSS_CONFIG_QZSS_ENABLED))
  {
    TM_CORE_OUTPUT_NMEA_PQGSA((is_no_fix == TRUE) ? NULL : z_fix);
  }

  
  if (q_GnssMask & C_RCVR_GNSS_CONFIG_BDS_ENABLED)
{
    TM_CORE_OUTPUT_NMEA_GBGSA((is_no_fix == TRUE) ? NULL : z_fix);
  }

  TM_CORE_OUTPUT_NMEA_GNGSA((is_no_fix == TRUE) ? NULL : z_fix);
  }
      

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
void tm_core_trigger_meas_related_NMEA(sm_GpsMeasRptStructType  *z_meas)
{
  uint32 q_GnssMask = sm_get_supported_constellation_mask();

  TM_MED_0("LPC:Generate MeasRelatedNMEA");

  if (q_GnssMask & C_RCVR_GNSS_CONFIG_GPS_ENABLED)
  {
    TM_CORE_OUTPUT_NMEA_GPGSV(z_meas);
  }

  if (q_GnssMask & C_RCVR_GNSS_CONFIG_GLO_ENABLED)
  {
    TM_CORE_OUTPUT_NMEA_GLGSV(z_meas);
  }

  if (q_GnssMask & C_RCVR_GNSS_CONFIG_GAL_ENABLED)
  {
    TM_CORE_OUTPUT_NMEA_GAGSV(z_meas);
  }

  if (q_GnssMask & (C_RCVR_GNSS_CONFIG_BDS_ENABLED | C_RCVR_GNSS_CONFIG_QZSS_ENABLED))
  {
    TM_CORE_OUTPUT_NMEA_PQGSV(z_meas);
  }

  if (q_GnssMask & C_RCVR_GNSS_CONFIG_BDS_ENABLED)
  {
    TM_CORE_OUTPUT_NMEA_GBGSV(z_meas);
  }

  if (q_GnssMask & C_RCVR_GNSS_CONFIG_QZSS_ENABLED)
  {
    TM_CORE_OUTPUT_NMEA_GQGSV(z_meas);
  }

  if (q_GnssMask & C_RCVR_GNSS_CONFIG_NAVIC_ENABLED)
  {
    TM_CORE_OUTPUT_NMEA_GIGSV(z_meas);
  }
}

/*===========================================================================
FUNCTION tm_lpc_update_for_aiding_data

DESCRIPTION 
    LPC needs to maintian list of SVs for which aiding was 
    provided. This function is called when aiding data is recieved from protocols
    and it updates a mask for the SVs in aiding list.

DEPENDENCIES

INPUT PARAMS
    ldm_client_id_e_type LDM client ID.
    gnss_SvDirStructType Pointer to SV direction information structure.

RETURN VALUE
    NONE.

SIDE EFFECTS
===========================================================================*/
void tm_lpc_update_for_aiding_data(ldm_client_id_e_type              e_client_id,
                                   mgp_GnssType                      q_GnssType,
                                   const gnss_SvAcqAssistCommonStructType  *pz_SvAcqAssistInfo)
{
  uint16  w_Sv;
  int     itr;
  tm_loc_processing_client_info *p_z_client_info = NULL;

  MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LPC:UPdating aiding info for client %d", e_client_id);

  if (NULL == pz_SvAcqAssistInfo)
  {
    MSG(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "LPC:Received NULL SVDir info");
    return;
  }
  if (e_client_id >= LDM_CLIENT_ID_FIRST && e_client_id <= LDM_CLIENT_ID_LAST)
  {
    /*Get client info from DB*/
    p_z_client_info = lpc_get_loc_processing_client(e_client_id);

    if (NULL == p_z_client_info)
    {
      MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "LPC:Client %d not registered.", e_client_id);
      return;
    }

    /*Update the Aiding bit mask for the recieved SvDir info*/
    if (MGP_GNSS_TYPE_GPS == q_GnssType)
    {
      MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LPC:Received aiding info is for GPS");
      MSG_2(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LPC:GPS bitmask for client %d is %x before updating", e_client_id, p_z_client_info->aa_received_gps_svs);
      for (itr = 0; ((itr < pz_SvAcqAssistInfo->u_NumSvs) && (itr < N_MAX_VISIBLE_GNSS_SV)); itr++)
      {
        w_Sv = (pz_SvAcqAssistInfo->z_SvSteerInfo[itr]).w_Sv;
        p_z_client_info->aa_received_gps_svs |= (1 << (w_Sv - GPS_LOWEST_PRN));
      }
      MSG_2(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LPC:GPS bitmask for client %d is %x after updating", e_client_id, p_z_client_info->aa_received_gps_svs);
    }
    else if (MGP_GNSS_TYPE_GLO == q_GnssType)
    {
      MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LPC:Received aiding info is for GLO");
      MSG_2(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LPC:GLO bitmask for client %d is %x before updating", e_client_id, p_z_client_info->aa_received_glo_svs);
      for (itr = 0; ((itr < pz_SvAcqAssistInfo->u_NumSvs) && (itr < N_MAX_VISIBLE_GNSS_SV)); itr++)
      {
        w_Sv = (pz_SvAcqAssistInfo->z_SvSteerInfo[itr]).w_Sv;
        p_z_client_info->aa_received_glo_svs |= (1 << (w_Sv - GLO_LOWEST_ID));
      }
      MSG_2(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LPC:GPS bitmask for client %d is %x after updating", e_client_id, p_z_client_info->aa_received_glo_svs);
    }
    else if (MGP_GNSS_TYPE_BDS == q_GnssType)
    {
      MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LPC:Received aiding info is for BDS");
      MSG_2(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LPC:BDS bitmask for client %d is %x before updating", e_client_id, p_z_client_info->aa_received_bds_svs);
      for (itr = 0; ((itr < pz_SvAcqAssistInfo->u_NumSvs) && (itr < N_MAX_VISIBLE_GNSS_SV)); itr++)
      {
        w_Sv = (pz_SvAcqAssistInfo->z_SvSteerInfo[itr]).w_Sv;
        p_z_client_info->aa_received_glo_svs |= (1 << (w_Sv - BDS_LOWEST_ID));
      }
      MSG_2(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LPC:BDS bitmask for client %d is %x after updating", e_client_id, p_z_client_info->aa_received_bds_svs);
    }
    else if (MGP_GNSS_TYPE_GAL == q_GnssType)
    {
      MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LPC:Received aiding info is for GAL");
      MSG_2(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LPC:GAL bitmask for client %d is %x before updating", e_client_id, p_z_client_info->aa_received_gal_svs);
      for (itr = 0; ((itr < pz_SvAcqAssistInfo->u_NumSvs) && (itr < N_MAX_VISIBLE_GNSS_SV)); itr++)
      {
        w_Sv = (pz_SvAcqAssistInfo->z_SvSteerInfo[itr]).w_Sv;
        p_z_client_info->aa_received_glo_svs |= (1 << (w_Sv - GAL_LOWEST_ID));
      }
      MSG_2(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LPC:BDS bitmask for client %d is %x after updating", e_client_id, p_z_client_info->aa_received_gal_svs);
    }
    else
    {
      MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LPC:Received aiding info is %d GNSS type, not expected", q_GnssType);
    }
  }
  else
  {
    MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "LPC:Unknown client id %d", e_client_id);
  }
}

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
void tm_lpc_timer_expiry_handler(const uint32 q_TimerId)
{
  if ((q_TimerId & TM_CORE_TIMER_DISPATCH_MASK) == TM_LPC_TIMER_ID_RANGE)
  {
    switch (q_TimerId)
    {
      case TM_LPC_TIMER_ID_LOC_TIMEOUT:
        MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LPC:TM_LPC_TIMER_ID_LOC_TIMEOUT timer expired");
        lpc_session_timeout_handler();
        break;

      default:
        MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LPC:Unknown timer expired", 0, 0, 0);
        break;
    }
  }
  else /* timer id range check */
  {
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "LPC:Can not proess; invalid timer event", 0, 0, 0);
  }
}

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
void tm_lpc_cb_handler(const ldm_client_cb_data_s_type *p_client_cb_data)
{
  tm_session_update_info_s_type *sess_update_ptr = NULL;
  prtl_sess_info_param_u_type   *sess_info_param_ptr = NULL;
  boolean                       retVal = FALSE;
  pdsm_ext_status_info_s_type   *pz_pd_ext_status_info;
  tm_loc_processing_client_info *pz_loc_processing_client = NULL;

  sm_GpsMeasRptStructType       *p_MeasRptSrc = NULL;
  gnss_MeasBlkStructType        *p_MeasBlkSrc = NULL;
  tm_fix_report_s_type          *pz_fix_rpt = NULL;
  tm_ppm_report_s_type          *pz_ppm_rpt = NULL;
  prtl_sess_info_e_type         sess_info_type = PRTL_SESS_INFO_NONE;
  sm_SvPolyReportType           *p_SvPolySrc = NULL;
  tm_cm_phone_state_info_s_type  phone_state_info;


  
  if (NULL == p_client_cb_data)
  {
    MSG(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "LPC:LDM send  NULL pointer.");
    return;
  }

  pz_loc_processing_client = lpc_get_loc_processing_client(p_client_cb_data->e_client_id);

  if (NULL == pz_loc_processing_client)
  {
    MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "LPC:LDM client %d not enabled.", p_client_cb_data->e_client_id);
    return;
  }

  memset(&phone_state_info, 0, sizeof(tm_cm_phone_state_info_s_type));
  tm_cm_iface_get_phone_state(&phone_state_info);

  if ((TM_CM_E911_STATE_CONCURRENT == phone_state_info.e911_state) &&
      (LDM_CLIENT_ID_APPTRK == p_client_cb_data->e_client_id) &&
      (LDM_CLIENT_TYPE_MAIN == p_client_cb_data->e_client_type) &&
      (!(tm_core_session_active())))
  {
    MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LPC:Recieved request for AppTrack (LDM client %d) during no session period, in concurrent mode. Do nothing",
          p_client_cb_data->e_client_id);
    return;
  }

  sess_update_ptr = (tm_session_update_info_s_type *)os_MemAlloc(sizeof(tm_session_update_info_s_type), OS_MEM_SCOPE_TASK);
  if (NULL == sess_update_ptr)
  {
    MSG(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "LPC:Cannot allocate memory for sess_update_ptr in tm_lpc_cb_handler");
    tm_core_abort_recover(TRUE, TM_STOP_REASON_GENERAL_ERROR); /*lint !e506 !e730 */
    return;
  }

  memscpy((void *)sess_update_ptr, sizeof(tm_session_update_info_s_type),
          (void *)&(p_client_cb_data->z_sess_update_info), sizeof(tm_session_update_info_s_type));

  MSG_2(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LPC:Processing %d request for client %d",
        sess_update_ptr->e_update_type, p_client_cb_data->e_client_id);

  pz_pd_ext_status_info = (pdsm_ext_status_info_s_type *)os_MemAlloc(sizeof(pdsm_ext_status_info_s_type), OS_MEM_SCOPE_TASK);
  if (NULL == pz_pd_ext_status_info)
  {
    MSG(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "Cannot allocate memory for pdsm_ext_status_info_s_type in tm_lpc_cb_handler");
    (void)os_MemFree((void **)&sess_update_ptr);
    tm_core_abort_recover(TRUE, TM_STOP_REASON_GENERAL_ERROR); /*lint !e506 !e730 */
    return;
  }
  memset(pz_pd_ext_status_info, 0, sizeof(*pz_pd_ext_status_info));

  memset(&z_serving_client_data, 0, sizeof(z_serving_client_data));


  if (sess_update_ptr)
  {
    if ((sess_update_ptr->e_update_type == TM_INFO_SV_POLY_REPORT) ||
       ((TM_CM_E911_STATE_CONCURRENT != phone_state_info.e911_state) && (sess_update_ptr->q_handle == tm_core_info.session_info.active_sess_handle)) ||
       ((TM_CM_E911_STATE_CONCURRENT == phone_state_info.e911_state) && (p_client_cb_data->e_client_id <= LDM_CLIENT_ID_LAST)))
    {
      /*Get the protocol index from LPC database*/
      //prtlTableIndex = tm_core_get_prtl_idx_by_type(pz_loc_processing_client->prtl_type);
      z_serving_client_data.serving_client_id = p_client_cb_data->e_client_id;
      z_serving_client_data.serving_prtl_type = pz_loc_processing_client->prtl_type;
      z_serving_client_data.serving_sess_handle = pz_loc_processing_client->sessionHandle;
      z_serving_client_data.prtlTableIndex = tm_core_get_prtl_idx_by_type(pz_loc_processing_client->prtl_type);
      z_serving_client_data.is_in_progress = TRUE;
      z_serving_client_data.pz_pd_ext_status_info = pz_pd_ext_status_info;
      z_serving_client_data.e_client_type = p_client_cb_data->e_client_type;
      z_serving_client_data.v_filterAA = pz_loc_processing_client->v_filterAA;

      MSG_4(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LPC: Serving LDM client info;client id %d, prtl index %d, sess handle %d, client type %d",
            z_serving_client_data.serving_client_id,
            z_serving_client_data.prtlTableIndex,
            z_serving_client_data.serving_sess_handle,
            z_serving_client_data.e_client_type);

      sess_info_param_ptr = (prtl_sess_info_param_u_type *)os_MemAlloc(sizeof(prtl_sess_info_param_u_type), OS_MEM_SCOPE_TASK);

      if (sess_info_param_ptr == NULL)
      {
        MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "Failed to get buffer for sess_info_param_ptr", 0, 0, 0);
        (void)os_MemFree((void **)&pz_pd_ext_status_info);
        (void)os_MemFree((void **)&sess_update_ptr);
        return;
      }

      z_serving_client_data.sess_info_param_ptr = sess_info_param_ptr;

      switch (sess_update_ptr->e_update_type)
      {
        case TM_INFO_STATUS_REPORT:
          {
            MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LPC: Recieved TM_INFO_STATUS_REPORT from LDM for client %d", p_client_cb_data->e_client_id);
            if (LDM_CLIENT_TYPE_MAIN == p_client_cb_data->e_client_type)
            {
              MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LPC: Sending STATUS report for main client with ID %d", p_client_cb_data->e_client_id);
              if (sess_update_ptr->z_update.z_status_rpt.e_status == TM_STATUS_NACK)
              {
                MSG(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "Got NACK from LM, abort fix");
                tm_core_abort_recover(TRUE, TM_STOP_REASON_GENERAL_ERROR); /*lint !e506 !e730 */
              }
            }
            else
            {
              MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "LPC: Cannot send STATUS REPORT for secondary client id %d", p_client_cb_data->e_client_id);
            }
          }
          break;

        case TM_INFO_PPM_REPORT:
          {
            MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LPC: Recieved TM_INFO_PPM_REPORT from LDM for client %d", p_client_cb_data->e_client_id);
            if (TM_CM_E911_STATE_CONCURRENT != phone_state_info.e911_state)
            {
              pz_ppm_rpt = &(sess_update_ptr->z_update.z_ppm_rpt);
              (void)lpc_handle_ppm_report(pz_ppm_rpt);
            }
          }
          break;

        case TM_INFO_LATE_PRM_REPORT:
        case TM_INFO_PRM_REPORT:
          {
            MSG_6(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LPC: Received PRM_REPORT from LDM for client %d. Late %d, Sys[%d] Seq#[%d] of MaxNum[%d] NHz %d",
                  p_client_cb_data->e_client_id,
                  (TM_INFO_LATE_PRM_REPORT == sess_update_ptr->e_update_type),
                  sess_update_ptr->z_update.pz_measure_rpt->z_GpsMeasRpt.z_MeasBlk.z_GnssHeader.e_MeasBlkSrc,
                  sess_update_ptr->z_update.pz_measure_rpt->z_GpsMeasRpt.z_MeasBlk.z_GnssHeader.u_SeqNum,
                  sess_update_ptr->z_update.pz_measure_rpt->z_GpsMeasRpt.z_MeasBlk.z_GnssHeader.u_MaxMessageNum,
                  (GNSS_MEAS_RPT_PERIODIC_NHZ_SM == sess_update_ptr->z_update.pz_measure_rpt->z_GpsMeasRpt.z_MeasBlk.z_GnssHeader.e_GnssMeasRptreason));
            if ((z_serving_client_data.prtlTableIndex <= -1) ||
                (z_serving_client_data.prtlTableIndex >= TM_PRTL_NUM))
            {
              MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "Invalid prtl index 0X%x, can not report PRM", z_serving_client_data.prtlTableIndex);
            }
            else
            {
              p_MeasRptSrc = &(sess_update_ptr->z_update.pz_measure_rpt->z_GpsMeasRpt);
              p_MeasBlkSrc = &p_MeasRptSrc->z_MeasBlk;

              if ((TM_CM_E911_STATE_CONCURRENT == phone_state_info.e911_state) &&
                  (LDM_CLIENT_TYPE_SECONDARY == p_client_cb_data->e_client_type))
              {
                MSG_2(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LPC: Processing PRM for Secondary client in concurrent mode. Late %d NHz %d",
                      (TM_INFO_LATE_PRM_REPORT == sess_update_ptr->e_update_type),
                      (GNSS_MEAS_RPT_PERIODIC_NHZ_SM == sess_update_ptr->z_update.pz_measure_rpt->z_GpsMeasRpt.z_MeasBlk.z_GnssHeader.e_GnssMeasRptreason));
                (void)lpc_handle_prm_report_secondary(p_MeasRptSrc);
              }
              else if (pz_loc_processing_client->v_filterAA)
              {
                MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LPC: Processing PRM for Main client and filter as per AA");
                (void)lpc_handle_prm_report_secondary(p_MeasRptSrc);
              }
              else if ((LDM_CLIENT_TYPE_MAIN == p_client_cb_data->e_client_type))
              {
                MSG_2(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LPC: Processing PRM for Main client (concurr/non-concurr). Late %d NHz %d",
                      (TM_INFO_LATE_PRM_REPORT == sess_update_ptr->e_update_type),
                      (GNSS_MEAS_RPT_PERIODIC_NHZ_SM == sess_update_ptr->z_update.pz_measure_rpt->z_GpsMeasRpt.z_MeasBlk.z_GnssHeader.e_GnssMeasRptreason));
                (void)lpc_handle_prm_report(p_MeasRptSrc);
              }
              else
              {
                MSG(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "LPC: Client not supported");
              }

            }
          }
          break;

        case TM_INFO_FINAL_FIX_REPORT:
          {
            pz_fix_rpt = &sess_update_ptr->z_update.z_fix_rpt;
            MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LPC: Recieved TM_INFO_FINAL_FIX_REPORT from LDM for client %d", p_client_cb_data->e_client_id);
			if (TRUE == lp_clients[p_client_cb_data->e_client_id].v_first_fix_recieved)
			{
			   pz_fix_rpt->z_GpsFixRpt.z_NavPos.u_FirstFix = FALSE;
			}
			else
			{
			   lp_clients[p_client_cb_data->e_client_id].v_first_fix_recieved = TRUE;
			   pz_fix_rpt->z_GpsFixRpt.z_NavPos.u_FirstFix = TRUE;
			}           

            if ((TM_CM_E911_STATE_CONCURRENT == phone_state_info.e911_state) &&
                (LDM_CLIENT_TYPE_SECONDARY == p_client_cb_data->e_client_type))
            {
              MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LPC: Processing fix report for Secondary client in concurrent mode");
              /*Call the fix processing to qualify the fix report received*/
              (void)lpc_handle_fix_report_secondary(pz_fix_rpt);
			  }
            else if ((LDM_CLIENT_TYPE_MAIN == p_client_cb_data->e_client_type))
            {
              MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LPC: Processing Final fix report for Main client (concurr/non-concurr)");
              (void)lpc_handle_final_fix_report(&(pz_fix_rpt->z_GpsFixRpt), z_serving_client_data.serving_client_id);
              }
            else
            {
              MSG(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "LPC: Client not supported");
            }
          }
          break;

        case TM_INFO_INTERMEDIATE_FIX_REPORT:
          {
            pz_fix_rpt = &sess_update_ptr->z_update.z_fix_rpt;
            MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LPC: Recieved TM_INFO_INTERMEDIATE_FIX_REPORT from LDM for client %d", p_client_cb_data->e_client_id);
			if (TRUE == lp_clients[p_client_cb_data->e_client_id].v_first_fix_recieved)
			{
			   pz_fix_rpt->z_GpsFixRpt.z_NavPos.u_FirstFix = FALSE;
			}
			else
			{
			   lp_clients[p_client_cb_data->e_client_id].v_first_fix_recieved = TRUE;
			   pz_fix_rpt->z_GpsFixRpt.z_NavPos.u_FirstFix = TRUE;
			}           

            if ((TM_CM_E911_STATE_CONCURRENT == phone_state_info.e911_state) &&
                (LDM_CLIENT_TYPE_SECONDARY == p_client_cb_data->e_client_type))
            {
              MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LPC: Processing fix report for Secondary client in concurrent mode");
              /*Call the fix processing to qualify the fix report received*/
              (void)lpc_handle_fix_report_secondary(pz_fix_rpt);
			      }		     
            else if ((LDM_CLIENT_TYPE_MAIN == p_client_cb_data->e_client_type))
            {
              MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LPC: Processing intermediate fix report for Main client (concurr/non-concurr)");
              (void)lpc_handle_intermediate_fix_report(pz_fix_rpt);
              }
            else
            {
              MSG(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "LPC: Client not supported");
            }
          }
          break;

        case TM_INFO_UNPROPAGATED_FIX_REPORT:
        {
          if (LDM_CLIENT_ID_APPTRK == p_client_cb_data->e_client_id)
          {
              pz_pd_ext_status_info->client_id = tm_core_info.session_info.pd_param.client_id;
              pz_pd_ext_status_info->ext_status_type = PDSM_EXT_STATUS_UNPROPAGATED_POS_REPORT;

              tm_core_xlate_pos_to_ext_status(&(pz_pd_ext_status_info->ext_status_info.ext_pos_report_type),
                                            &sess_update_ptr->z_update.z_fix_rpt.z_GpsFixRpt,
                                              sess_update_ptr->z_update.z_fix_rpt.q_FixTimeRemainingMSec);

              tm_pdapi_ext_status_event_callback(PDSM_EXT_STATUS_UNPROPAGATED_POS_REPORT, pz_pd_ext_status_info);
            } 
          }
        break;

        case TM_INFO_SV_POLY_REPORT:
          {
            MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LPC: Recieved TM_INFO_SV_POLY_REPORT from LDM for client %d", p_client_cb_data->e_client_id);

            if ((LDM_CLIENT_TYPE_MAIN == p_client_cb_data->e_client_type))
            {
              if (tm_core_info.config_info.gnss_meas_rep_qmi_out_control & SM_GNSS_BMAP_QMI_SV_POLY_REPORTING)
              {
                p_SvPolySrc = &sess_update_ptr->z_update.z_sv_poly_rpt.z_SvPolyRpt;
                MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LPC:Received SV_POLY_REPORT in TM", 0, 0, 0);
                memset(pz_pd_ext_status_info, 0, sizeof(*pz_pd_ext_status_info));
                pz_pd_ext_status_info->client_id    = tm_core_info.session_info.pd_param.client_id;
                pz_pd_ext_status_info->ext_status_type = PDSM_EXT_STATUS_SV_POLY_REPORT;

                if (TRUE == tm_core_xlate_svpoly_to_ext_svpoly_rpt(&(pz_pd_ext_status_info->ext_status_info.ext_sv_poly_type), p_SvPolySrc))
                {
                  tm_pdapi_ext_status_event_callback(PDSM_EXT_STATUS_SV_POLY_REPORT, pz_pd_ext_status_info);
                }
              }
            }
          }
          break;

        default:
      {
            MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "LPC: Recieved wrong request type from LDM for client %d", p_client_cb_data->e_client_id);
          }
          break;
         
              }              
      (void)os_MemFree((void **)&sess_info_param_ptr);
          }
      }
#ifdef FEATURE_GNSS_PERIODIC_PPM
  #error code not present
#endif
  else
  {
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LPC:Rejecting Invalid Handle %d ActiveHandle %d UpdateType %d",
          sess_update_ptr->q_handle,
          tm_core_info.session_info.active_sess_handle,
          sess_update_ptr->e_update_type);
  }
  memset(&z_serving_client_data, 0, sizeof(z_serving_client_data));
  (void)os_MemFree((void **)&pz_pd_ext_status_info);
  (void)os_MemFree((void **)&sess_update_ptr);
}

/*===========================================================================
FUNCTION tm_lpc_generate_pd_event

DESCRIPTION
    In concurrent mode PD event needs to be generated when E911 session ends.
    This function generated the PD done event.

DEPENDENCIES
    NONE
INPUT PARAMS
    ldm_client_id_e_type LDM client ID
    tm_sess_stop_reason_e_type Abort reason

RETURN VALUE
    NONE

SIDE EFFECTS
===========================================================================*/
void tm_lpc_generate_pd_event(ldm_client_id_e_type e_ldm_cli_id,
                              tm_sess_stop_reason_e_type abort_reason)
{
  tm_pdapi_pd_event_s_type       *pz_pd_event_type = NULL;
  pdsm_pd_end_e_type            e_end_status = PDSM_PD_END_SESS_NONE;
  int                            prtlTableIndex = -1;
  prtl_event_u_type              event_payload;
  pdsm_client_id_type             pd_client_id = tm_lpc_get_pdsm_client_id_for_ldm_client(e_ldm_cli_id);

  MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LPC:Generate PD event for %d reason", abort_reason);
  MSG_2(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LPC:Generate PD event stored pdsm client id %d for LDM client %d",
        tm_lpc_get_pdsm_client_id_for_ldm_client(e_ldm_cli_id), e_ldm_cli_id);

  pz_pd_event_type = (tm_pdapi_pd_event_s_type *)os_MemAlloc(sizeof(tm_pdapi_pd_event_s_type), OS_MEM_SCOPE_TASK);
  if (NULL == pz_pd_event_type)
  {
    MSG(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "LPC:Cannot allocate memory for tm_pdapi_pd_event_s_type in tm_core_abort_recover");
    //MSG(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "Falling back on stack memory");
    return;
  }
  else
  {
    memset(pz_pd_event_type, 0, sizeof(*pz_pd_event_type));
  }

  if (FALSE == lp_clients[e_ldm_cli_id].is_enabled)
  {
    MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "LPC: LDM Client %d not found", e_ldm_cli_id);
    return;
  }
  prtlTableIndex = tm_core_get_prtl_idx_by_type(lp_clients[e_ldm_cli_id].prtl_type);

  pz_pd_event_type->pd_event_data.pd_info_type.client_id = pd_client_id;
  pz_pd_event_type->pd_event_data.pd_info_type.pd_info.end_status = PDSM_PD_END_SESS_NONE;

  if ((abort_reason != TM_STOP_REASON_COMPLETED))
  {
    pz_pd_event_type->e_pd_event = PDSM_PD_EVENT_END;
    pz_pd_event_type->pd_event_data.pd_info_type.pd_info.end_status = tm_core_translate_end_status(abort_reason);
    MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LPC:Fire PDAPI event;client %d", pd_client_id);
    tm_pdapi_pd_event_callback(pz_pd_event_type, TM_PDAPI_SESS_TYPE_NONE);
  }
  e_end_status = pz_pd_event_type->pd_event_data.pd_info_type.pd_info.end_status;

  /* Fire EVENT_GPS_PD_FIX_END */
  sm_report_event_gps_fix_end(e_end_status);

  /* Fire Done event */
  pz_pd_event_type->pd_event_data.pd_info_type.client_id = pd_client_id;
  pz_pd_event_type->e_pd_event = PDSM_PD_EVENT_DONE;
  MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LPC:Fire PDAPI PDSM_PD_EVENT_DONE event;client %d", pd_client_id);
  tm_pdapi_pd_event_callback(pz_pd_event_type, TM_PDAPI_SESS_TYPE_NONE);

  /* Check prtl_func_cb_table bounds */
  if ((prtlTableIndex >= 0) && (prtlTableIndex < TM_PRTL_NUM))
  {
    /* This is added for internal Vx NI client */
    if (NULL != tm_core_info.prtl_func_cb_table[prtlTableIndex].event_cb_fp)
    {
      event_payload.pd_event_payload.pd_event = PDSM_PD_EVENT_DONE;
      (void)tm_core_info.prtl_func_cb_table[prtlTableIndex].event_cb_fp(PRTL_EVENT_TYPE_PD,
                                                                        &event_payload);
    }
  }
  else
  {
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "LPC:Attempt for an out of bound access to prtl_func_cb_table with index = %d", prtlTableIndex, 0, 0);
  }
  tm_core_info.pz_concurrent_client_info.client_id = -1;
  (void)os_MemFree((void **)&pz_pd_event_type);
}

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
void tm_lpc_report_error_secondary(tm_sess_stop_reason_e_type e_reason, ldm_client_id_e_type e_secondary_client_id)
{
  int                            prtlTableIndex = -1;
  tm_cm_phone_state_info_s_type  phone_state_info;
  boolean                        prtl_abort_session = FALSE;
  prtl_sess_stop_type            e_prtl_sess_stop = PRTL_SESS_STOP_SINGLE_FIX;

  MSG_2(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LPC: Report error %d to protocol for secondary client_id %d in concurrent mode", e_reason, e_secondary_client_id);
  memset(&phone_state_info, 0, sizeof(phone_state_info));
  tm_cm_iface_get_phone_state(&phone_state_info);

  if (LDM_CLIENT_ID_APPTRK == e_secondary_client_id)
  {
    e_prtl_sess_stop = PRTL_SESS_STOP_TRACKING_SESSION;
  }
  else
  {
    e_prtl_sess_stop = PRTL_SESS_STOP_SINGLE_FIX;
  }

  prtlTableIndex = tm_core_get_prtl_idx_by_type(lp_clients[e_secondary_client_id].prtl_type);

  /*Check if we have valid protocol index*/
  if ((prtlTableIndex > -1) && (prtlTableIndex < TM_PRTL_NUM))
  {
    if (tm_core_info.prtl_func_cb_table[prtlTableIndex].stop_sess_req_fp)
  {
      prtl_abort_session = tm_core_info.prtl_func_cb_table[prtlTableIndex].stop_sess_req_fp(
         lp_clients[e_secondary_client_id].sessionHandle,
         e_prtl_sess_stop,
         e_reason);
      if (prtl_abort_session == TRUE)
      {
        MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LPC: Protocol aborted the session for secondary client %d", e_secondary_client_id);
      }
      else
      {
        MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LPC: Protocol decided to continue the session for secondary client %d", e_secondary_client_id);
      }
    }
    else
    {
      MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "LPC: No protocol callback installed for secondary client %d", e_secondary_client_id);
    }
  }
  else
  {
    MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "LPC: Invalid protocol index for secondary client %d", e_secondary_client_id);
  }
    }

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
void tm_lpc_stop_timer_request()
{
  MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LPC: TM requested to stop the FIX/PRM timer");

  if (NULL == z_lpc_control_data)
  {
    MSG(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "LPC: The control data not initialized for secondary client");
    return;
  }

  /*Stop the LPC timer.*/
  os_TimerStop(z_lpc_control_data->p_location_Timer);
}

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
void tm_lpc_disable_client_no_active_protocol()
{
  ldm_client_id_e_type           e_ldm_client_id = LDM_CLIENT_ID_MAX;
  tm_cm_phone_state_info_s_type  phone_state_info;
  uint32                         q_ldm_query_mask = 0;

  memset(&phone_state_info, 0, sizeof(phone_state_info));
  tm_cm_iface_get_phone_state(&phone_state_info);

  /*In non concurrent mode only one client will be enabled in LDM*/
  if (TM_CM_E911_STATE_CONCURRENT != phone_state_info.e911_state)
  {
    q_ldm_query_mask = ldm_client_query_state();

    if ((q_ldm_query_mask & LDM_CLIENT_E911_MASK) != 0)
    {
      e_ldm_client_id = LDM_CLIENT_ID_E911;
    }
    else if ((q_ldm_query_mask & LDM_CLIENT_APPTRK_MASK) != 0)
    {
      e_ldm_client_id = LDM_CLIENT_ID_APPTRK;
    }
    else if ((q_ldm_query_mask & LDM_CLIENT_SINGLE_SHOT_MASK) != 0)
    {
      e_ldm_client_id = LDM_CLIENT_ID_SINGLE_SHOT;
    }

    if (LDM_CLIENT_ID_MAX != e_ldm_client_id)
    {
      if (LPC_OP_STATUS_REQUEST_SUCCESS == tm_lpc_disable_client(e_ldm_client_id))
      {
        MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LPC:cleanup;client %d disabled", e_ldm_client_id);
      }
    }
    else
    {
      MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LPC:cleanup;no client in LDM");
    }
  } /*if ( TM_CM_E911_STATE_CONCURRENT != phone_state_info.e911_state) */
    else
    {
    /*In concurrent mode, active session handle will not be TM_SESS_HANDLE_E911|TM_SESS_HANDLE_LPP_CP_NI|TM_SESS_HANDLE_UMTS_UP_NI 
      hence this function will never be called.
      In other words, in concurrent mode we dont have early reciever start, hence this should never be called.*/
    MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LPC:cleanup;concurrent mode;do nothing");
    }

  return;
}


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
void tm_lpc_get_main_secondary_clients(ldm_client_id_e_type *main_client, ldm_client_id_e_type *secondary_client)
{
  ldm_client_id_e_type e_i;
  *main_client = LDM_CLIENT_ID_MAX;
  *secondary_client = LDM_CLIENT_ID_MAX;

  for (e_i = LDM_CLIENT_ID_FIRST; e_i <= LDM_CLIENT_ID_LAST; e_i = (ldm_client_id_e_type)(e_i + 1))
  {
    if (TRUE == lp_clients[e_i].is_enabled)
    {
      if (LDM_CLIENT_TYPE_MAIN == lp_clients[e_i].e_clientId_type)
      {
        *main_client = lp_clients[e_i].e_clientId;
      }
      else if (LDM_CLIENT_TYPE_SECONDARY == lp_clients[e_i].e_clientId_type)
      {
        *secondary_client = lp_clients[e_i].e_clientId;
      }
    }
  }
  MSG_2(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LDM: Main client = %d, Secondary client = %d", *main_client, *secondary_client);
}

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
void tm_lpc_clear_control_data()
{
  if (NULL != z_lpc_control_data)
  {
    MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LPC: LPC control data and timers for secondary client cleared");
    /*Stop timer*/
    os_TimerStop(z_lpc_control_data->p_location_Timer);
    os_TimerDestroy(z_lpc_control_data->p_location_Timer);
    (void)os_MemFree((void **)&z_lpc_control_data);
    z_lpc_control_data = NULL;
  }
  else
  {
    MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LPC: LPC control data already cleared");
  }
}

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
boolean tm_lpc_client_update_client_type(ldm_client_id_e_type e_client_id, ldm_client_e_type e_client_type)
{
  tm_loc_processing_client_info *pz_client_info;

  pz_client_info = lpc_get_loc_processing_client(e_client_id);

  if (NULL == pz_client_info)
  {
    MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "LPC:Client switch failed. Client %d not found in database", e_client_id);
    return FALSE;
  }

  pz_client_info->e_clientId_type = e_client_type;

  if (LDM_OP_STATUS_REQUEST_SUCCESS == ldm_client_update_client_type(e_client_id, e_client_type))
  {
    MSG_2(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LDM:Client %d switched to client type %d!", e_client_id, e_client_type);
    return TRUE;
  }
  else
  {
    MSG_2(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "LDM:Client %d failed to switch to client type %d!", e_client_id, e_client_type);
    return FALSE;
  }

}

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
boolean tm_lpc_client_update_aa_filter_pref(ldm_client_id_e_type e_client_id, boolean v_aa_filter_pref)
{
  tm_loc_processing_client_info *pz_client_info;

  pz_client_info = lpc_get_loc_processing_client(e_client_id);

  if (NULL == pz_client_info)
  {
    MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "LPC: AA filter preference update failed. Client %d not found in database", e_client_id);
    return FALSE;
  }

  pz_client_info->v_filterAA = v_aa_filter_pref;
  MSG_2(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LDM:Client %d AA filter pref mode set to %d!", e_client_id, v_aa_filter_pref);

  return TRUE;
}
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
void tm_lpc_refloc_received(const tm_cm_e911_state_e_type e911_state)
{
  ldm_client_id_e_type e_main_client_id = LDM_CLIENT_ID_MAX;
  ldm_client_id_e_type e_secondary_client_id = LDM_CLIENT_ID_MAX;

  if (TM_CM_E911_STATE_CONCURRENT == e911_state)
  {
    tm_lpc_get_main_secondary_clients(&e_main_client_id, &e_secondary_client_id);
    if (LDM_CLIENT_ID_E911 == e_secondary_client_id)
    {
      /* If there is an ongoing E911 client as secondary, update lpc control with refloc received status */

      if (NULL == z_lpc_control_data)
      {
        MSG(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "LPC:LPC control data not initialized");
        return;
      }
      else
      {
        z_lpc_control_data->v_reflocReceived = TRUE;
        MSG(MSG_SSID_GPSSM, MSG_LEGACY_LOW, "LPC: RefLoc received status updated for E911 client");
      }
    }
  }
}

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
void tm_lpc_update_lm_with_E911_fix_qualification(const ldm_client_id_e_type e_client_id)
{
  
  tm_loc_processing_client_info *pz_client_info = NULL;
  lm_request_info_s_type  lm_info_req_param;
  
  memset(&lm_info_req_param, 0, sizeof(lm_info_req_param));
  

  if (NULL != z_lpc_control_data)
  {
    pz_client_info = lpc_get_loc_processing_client(e_client_id);
    if (NULL == pz_client_info)
    {
      MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "LPC: E911 fix qualification update to LM failed. Client %d not found", e_client_id);
      return;
    }

    if (TM_SESS_OPERATION_MODE_MSA == pz_client_info->sess_params.op_mode)
    {
      MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LPC: Not updating LM with E911 fix qual for E911 MSA Client %d", e_client_id);
      return;
    }

    lm_info_req_param.z_request.pz_e911_fix_qual = (lm_request_update_e911_fix_qual_type *)os_MemAlloc(sizeof(lm_request_update_e911_fix_qual_type), OS_MEM_SCOPE_TASK);

    if(NULL == lm_info_req_param.z_request.pz_e911_fix_qual)
    {
      TM_ERROR_0("Cannot allocate memory for lm_request_update_e911_fix_qual_type");
      return;
    }

    lm_info_req_param.e_req_type = LM_REQUEST_UPDATE_E911_FIX_QUALIFICATION;
    lm_info_req_param.z_request.pz_e911_fix_qual->v_gnssFixReceivedThisSession = z_lpc_control_data->b_gnssFixReceivedThisSession;
    lm_info_req_param.z_request.pz_e911_fix_qual->v_reflocReceived = z_lpc_control_data->v_reflocReceived;
    lm_info_req_param.z_request.pz_e911_fix_qual->v_bestFixValid = z_lpc_control_data->u_sessBestFixValid;

    if (z_lpc_control_data->u_sessBestFixValid)
    {
      lm_info_req_param.z_request.pz_e911_fix_qual->z_sessBestFixRpt = z_lpc_control_data->z_sessBestFixRpt;
    }
    else
    {
      MSG(MSG_SSID_GPSSM, MSG_LEGACY_LOW, "LPC: Best fix not valid. Not sent to LM");
    }
    (void)lm_request(&lm_info_req_param);
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LPC: Sent E911 fix qualification to LM: BestfixValid %d, gnssrcvd %d, reflocrcvd %d",
        lm_info_req_param.z_request.pz_e911_fix_qual->v_bestFixValid, lm_info_req_param.z_request.pz_e911_fix_qual->v_gnssFixReceivedThisSession,
        lm_info_req_param.z_request.pz_e911_fix_qual->v_reflocReceived);
  }
  else
  {
    MSG(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "LPC: Could not update LM with fix qualification since LPC control is NULL");
  }
 os_MemFree((void**)&lm_info_req_param.z_request.pz_e911_fix_qual);
}

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
boolean tm_lpc_client_special_req_check(void)
{
  boolean                         v_RetVal = FALSE;
  tm_loc_processing_client_info   *pz_main_client_info      = NULL;
  tm_loc_processing_client_info   *pz_secondary_client_info = NULL;
  ldm_client_id_e_type            e_main_client_id = LDM_CLIENT_ID_MAX;
  ldm_client_id_e_type            e_secondary_client_id = LDM_CLIENT_ID_MAX;


  tm_lpc_get_main_secondary_clients(&e_main_client_id, &e_secondary_client_id);

  pz_main_client_info      = lpc_get_loc_processing_client(e_main_client_id);
  pz_secondary_client_info = lpc_get_loc_processing_client(e_secondary_client_id);


  if (((NULL != pz_main_client_info) && 
       (PDSM_SESSION_SPECIAL_REQ_SHORT_CODES == pz_main_client_info->sess_params.specialReqType.e_SpecialReq)) ||
      ((NULL != pz_secondary_client_info) && 
       (PDSM_SESSION_SPECIAL_REQ_SHORT_CODES == pz_secondary_client_info->sess_params.specialReqType.e_SpecialReq)))
  {
       /*Stop CPI started for SpecialRequest for Main/Secondary Clients*/
       MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "SpecialReq active, Dont Stop CPI");
       v_RetVal = TRUE;
  }

  return (v_RetVal);
}
