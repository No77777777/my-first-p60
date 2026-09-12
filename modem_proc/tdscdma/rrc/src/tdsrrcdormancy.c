#include "tdscdma_variation.h"
#ifndef RRCDORMANCY_H
#define RRCDORMANCY_H
/*===========================================================================
                          R R C  FAST  DORMANCY

DESCRIPTION

 This module defines rrc fast dormancy feature in RRC.

 This module has functionality related to 3GPP defined Fast 
 Dormancy implementation & switch back to proprietary Fast 
 Dormancy in case NW doesnt support.

EXTERNALIZED FUNCTIONS


INITIALIZATION AND SEQUENCING REQUIREMENTS

  Queues must be initialized by calling tdsrrc_init_command_queues()
  at RRC task startup.

Copyright (c) 2013 Qualcomm Technologies, Inc.  All Rights Reserved.
Qualcomm Technologies Proprietary and Confidential.
Export of this technology or software is regulated by the U.S. Government. 
Diversion contrary to U.S. law prohibited.

===========================================================================*/


/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/tdscdma.mpss/4.3/rrc/src/tdsrrcdormancy.c#1 $ $DateTime: 2019/08/19 10:51:23 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ---------------------------------------------------------- 
08/15/11   hx      Enable protection for TDS receiving TDSRRC_FAST_DORMANCY_REQ from NAS 
03/10/11   yzh     Merged with VU_MODEM_WCDMA_RRC_OSYS.01.93.53
02/15/11   rl      Merged with VU_MODEM_WCDMA_RRC.01.91.50
01/31/11   ad      Fixed compilation warnings
01/28/11   rl      Merged with VU_MODEM_WCDMA_RRC.01.89.57
01/12/11   ad      Added changes for RRC SW decoupling
12/31/10   rl      Merged with VU_MODEM_WCDMA_RRC.01.86.50
11/29/10   rl      Merged with VU_MODEM_WCDMA_RRC.01.85.50
11/16/10   as      Added code to fill as_id for fast dormancy cnf
11/09/10   rl      Merged with VU_MODEM_WCDMA_RRC.01.84.50
09/27/10   ss      Added code to support as_id changes for Fast Dormancy
08/27/10   rm      Made changes to index SIB with correct SIB type
08/21/10   ss      Fixed compilation warnings.
===========================================================================*/


/* ==========================================================================
** Includes and Variable Definitions
** ========================================================================*/

/* -----------------------------------------------------------------------
** Include Files
** ----------------------------------------------------------------------- */
#include "tdsuecomdef.h"
#include "comdef.h"

#include "tdsrrcmm_v.h"
#include "tdsrrccmd_v.h"
#include "tdsrrcdata.h"
#include "tdsrrcdt.h"
#include "tdsrrcdormancy.h"
#include "tdsrrcccm.h"
#include "tdsrrcintif.h"
#include "tdsrrcrce.h"
#include "tdsrrcrcr.h"
#include "tdsrrcsend.h"
#include "tdsrrclcm.h"
#include "tdsrrctmr.h"
#include "tdsrrccu.h"
#include "tdsrrcscmgr.h"
#include "tdsrrciho.h"
#include "tdsrrccho.h"
#include "tdsrrcsmc.h"
#include "tdsrrcsibdb.h"
#include "tdsrrcllc.h"
#ifdef FEATURE_TDSCDMA_3GPP_FAST_DORMANCY_ENHANCEMENT
#include "tdsrlcrrcif.h"
#include "tdsrrcdata.h"
#endif

#ifdef TEST_FRAMEWORK
#error code not present
#endif

#include "tdsCFAParser_i.h"
#include "tdsrrcnv.h"

#ifdef FEATURE_TDSCDMA_3GPP_FAST_DORMANCY_ENHANCEMENT
/* Macro which define max SCRI re tries */
#define TDSRRC_DORMANCY_MAX_SCRI_RETRY 5

/* Macro which define timer value to be started in case of PCH to FACH 
 * transition which pending FD
 */
#define TDSRRC_DORMANCY_T323_RETRY_TIMER 3000

uint8  tdsrrc_max_scri_for_fd = TDSRRC_DORMANCY_MAX_SCRI_RETRY;
#endif


#if   defined(FEATURE_3GPP_FAST_DORMANCY)
/* -----------------------------------------------------------------------
** Constant / Define Declarations
** ----------------------------------------------------------------------- */


/* -----------------------------------------------------------------------
** Type Declarations
** ----------------------------------------------------------------------- */

/* -----------------------------------------------------------------------
** Global Constant Data Declarations
** ----------------------------------------------------------------------- */
tdsrrcdormancy_substate_e_type tdsrrcdormancy_substate;
tdsrrcdormancy_fast_dormancy_req_type tdsrrcdormancy_saved_fast_dormancy_req;
  
/*Stores T323 timer and N/W support Status */ 
tdsrrcdormancy_conn_timers_and_constants_type tdsrrcdormancy_conn_timers_and_constants;

/* Stores the count that guard sending of SCRI in CELL_PCH/URA_PCH states */
unsigned short tdsrrc_v_316;

/* -----------------------------------------------------------------------
** Global Data Declarations
** ----------------------------------------------------------------------- */

/* -----------------------------------------------------------------------
** Local Object Definitions
** ----------------------------------------------------------------------- */

/* -----------------------------------------------------------------------
** static function declarations
** ----------------------------------------------------------------------- */

/*===========================================================================
  
  FUNCTION 
    tdsrrcdormancy_get_T323_timer_value  

  DESCRIPTION
    This function check returns the value of T323 timer provided by Network
    in milliseconds.

  DEPENDENCIES
    none.

  RETURN VALUE
    none 
===========================================================================*/
static uint32 tdsrrcdormancy_get_T323_timer_value
(
  void
);

/*===========================================================================
  
  FUNCTION tdsrrcdormancy_process_pending_fast_dormancy_req
  
  DESCRIPTION
    This function processes tdsrrcdormancy_process_pending_fast_dormancy_req
  
  DEPENDENCIES
    None.
  
  RETURN VALUE
    None.
  
 ===========================================================================*/
static boolean tdsrrcdormancy_process_pending_fast_dormancy_req
(
  void
);


/* -----------------------------------------------------------------------
** Forward Declarations
** ----------------------------------------------------------------------- */
/* Any forward declarations go here */


/* =======================================================================
**                            Function Definitions
** ======================================================================= */
  
/*===========================================================================
    
    FUNCTION tdsrrcdormancy_init_saved_fast_dormancy_req  
    
    DESCRIPTION
      This function init tdsrrcdormancy_saved_fast_dormancy_req
    
    DEPENDENCIES
      None.
    
    RETURN VALUE
      None.
  ===========================================================================*/
static void tdsrrcdormancy_init_saved_fast_dormancy_req
(
  void
)
{
  TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRCDORM:init tdsrrcdormancy_saved_fast_dormancy_req");
  tdsrrcdormancy_saved_fast_dormancy_req.fast_dormancy_req_status = FALSE;
  tdsrrcdormancy_saved_fast_dormancy_req.mui = 0;
  tdsrrcdormancy_saved_fast_dormancy_req.mui_status = TDSRRCDORMANCY_MUI_STATUS_INVALID;
  tdsrrcdormancy_substate = TDSRRCDORMANCY_INITIAL;
}

/*===========================================================================
  
  FUNCTION tdsrrcdormancy_save_fast_dormancy_req  
  
  DESCRIPTION
    This function save TDSRRC_FAST_DORMANCY_REQ command received from MM task.
  
  DEPENDENCIES
    None.
  
  RETURN VALUE
  
===========================================================================*/
static tdsrrcdormancy_status_e_type tdsrrcdormancy_save_fast_dormancy_req
(
  tdsrrc_cmd_type *cmd_ptr                 /* Pointer to the RRC Command */
)
{
  tdsrrcdormancy_status_e_type save_status = TDSRRCDORMANCY_FAILURE;

  if(cmd_ptr->cmd_hdr.cmd_id == TDSRRC_FAST_DORMANCY_REQ)
  {
    if(tdsrrcdormancy_saved_fast_dormancy_req.fast_dormancy_req_status == FALSE)
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRCDORM:Save fast_dormancy_req");
      tdsrrcdormancy_saved_fast_dormancy_req.fast_dormancy_req_status = TRUE;
      tdsrrcdormancy_saved_fast_dormancy_req.mui_status = TDSRRCDORMANCY_MUI_STATUS_INVALID;
      tdsrrcdormancy_saved_fast_dormancy_req.mui = 0;
      save_status = TDSRRCDORMANCY_SUCCESS;
    }
    else
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRCDORM:Multiple Dormancy Requests not allowed");  
    }
  }
  else
  {
    TDSRRC_MSG1(MSG_LEGACY_ERROR,"RRCDORM:Invalid cmd 0x%x",cmd_ptr->cmd_hdr.cmd_id);
  }
  
  return save_status;
}

/*===========================================================================

FUNCTION tdsrrcdormancy_send_cell_update_initiate_req

DESCRIPTION
  This function send the cell update initiate request if we are in
  URA_PCH or CELL_PCH states.

DEPENDENCIES
  None.

RETURN VALUE
  None.

===========================================================================*/
static void tdsrrcdormancy_send_cell_update_initiate_req
(
  tdsrrc_proc_e_type rrcdormancy_procedure
)
{
  tdsrrc_cmd_type *int_cmd_ptr;
  //This initiates a CELL Update procedure and enters the substate
  //WAIT_FOR_CELL_UPDATE_COMP_CNF. 
  if( (int_cmd_ptr =  tdsrrc_get_int_cmd_buf()) != NULL)
  {     
    /* Fill in the command parameters for TDSRRC_CELL_UPDATE_INITIATE_REQ */
    int_cmd_ptr->cmd_hdr.cmd_id = TDSRRC_CELL_UPDATE_INITIATE_REQ;
    int_cmd_ptr->cmd.initiate_cell_update.cause = TDSRRC_UL_DATA_TRANSMISSION;
    int_cmd_ptr->cmd.initiate_cell_update.cu_complete_cnf_required = TRUE;
    int_cmd_ptr->cmd.initiate_cell_update.procedure = rrcdormancy_procedure;
        
    tdsrrc_put_int_cmd(int_cmd_ptr);             
  }
  else
  {
    ERR_FATAL("Unable to allocate memmory for: %d cmd",TDSRRC_CELL_UPDATE_INITIATE_REQ,0,0);
  }
}/*tdsrrcdt_send_cell_update_initiate_req*/

/*===========================================================================
  
  FUNCTION tdsrrcdormancy_map_internal_cause_to_fast_dormancy_cnf_cause  
  
  DESCRIPTION
    This function maps internal cause of type tdsrrcdormancy_status_e_type
    to cause to be sent in RRC_FAST_DORMANCY_CNF command to NAS
  
  DEPENDENCIES
    none.
  
  RETURN VALUE
    none 
===========================================================================*/
static rrc_fast_dormancy_cnf_status_e_type 
tdsrrcdormancy_map_internal_cause_to_fast_dormancy_cnf_cause
(
  tdsrrcdormancy_validation_status_e_type dormancy_cause
)
{
  rrc_fast_dormancy_cnf_status_e_type rrc_fast_dormancy_cnf_cause;
  switch(dormancy_cause)
  {
    case TDSRRCDORMANCY_VALIDATION_SUCCESS:
    case TDSRRCDORMANCY_VALIDATION_FAIL_CS_DOMAIN_OPEN:
    case TDSRRCDORMANCY_VALIDATION_FAIL_UE_OOS:
    case TDSRRCDORMANCY_VALIDATION_FAIL_T323_TIMER_RUNNING:
    case TDSRRCDORMANCY_VALIDATION_FAIL:
    case TDSRRCDORMANCY_VALIDATION_FAIL_UE_LEAVING_CONN_MODE_SCAN:
      rrc_fast_dormancy_cnf_cause = RRC_FAST_DORMANCY_FAILURE_APP_RETRY;
      break;

    case TDSRRCDORMANCY_VALIDATION_FAIL_DOMAIN_NOT_OPEN:
    case TDSRRCDORMANCY_VALIDATION_FAIL_RRC_CON_NOT_OPEN:
      rrc_fast_dormancy_cnf_cause = RRC_FAST_DORMANCY_SUCCESS_APP_NO_RETRY;
      break;
           
    default:
      rrc_fast_dormancy_cnf_cause = RRC_FAST_DORMANCY_FAILURE_APP_RETRY;
      break;
  }
  TDSRRC_MSG2(MSG_LEGACY_HIGH,"RRCDORM:dormancy_cause=%d,rrc_fast_dormancy_cnf_cause=%d",dormancy_cause,rrc_fast_dormancy_cnf_cause);
  return rrc_fast_dormancy_cnf_cause;
}
/*===========================================================================

FUNCTION tdsrrcdormancy_send_scri_for_fast_dormancy

DESCRIPTION
  Process SCRI request as a result of processing Fast Dormancy.

DEPENDENCIES
  None.

RETURN VALUE
  void

===========================================================================*/
static void tdsrrcdormancy_send_scri_for_fast_dormancy
(
boolean l2_ack_processing_required
)
{
  tdsrrc_UL_DCCH_Message *msg_ptr;                 /* Pointer to uplink DCCH message */  
  rlc_lc_id_type ul_lc_id;                      /* Uplink logical channel ID */
  tdsrrcsend_status_e_type send_status;            /* send status */
  uint8 msg_mui;                                /* MUI for the SCRR */

/*Here need TDS RLC give function call*/  
#ifdef FEATURE_TDSCDMA_3GPP_FAST_DORMANCY_ENHANCEMENT
  /* Reset RLC activity_flag as we are sending SCRI now */
  tdsrlc_reset_activity_flag();
#endif  
  /* Check to see if there is any saved message and send SigConnReq */
  if(tdsrrcdormancy_saved_fast_dormancy_req.fast_dormancy_req_status == TRUE)
  {
    /* Determine the logical channel id for the Uplink DCCH */
    ul_lc_id = tdsrrclcm_check_ul_rlc_lc_id(UE_LOGCHAN_DCCH,
      DCCH_AM_RADIO_BEARER_ID, UE_MODE_ACKNOWLEDGED_DATA);
      
    if (ul_lc_id == TDSRRCLCM_RLC_LC_ID_NOT_FOUND)
    {
      /* The logical channel id was not found */
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"RB 2 not found by LCM");
      tdsrrcdormancy_reset_n_send_fast_dormancy_cnf(RRC_FAST_DORMANCY_FAILURE_APP_RETRY);
      return;
    }
    else
    {
      /* Build a Signalling Connection Release Request Message. Start by allocating
       * an Uplink DCCH message. This memory is released by tdsrrcsend_send_ul_sdu().
       */
      msg_ptr = (tdsrrc_UL_DCCH_Message *)tdsrrc_malloc(sizeof(struct tdsrrc_UL_DCCH_Message));
      
      /* Call error fatal if the memory allocation failed */
      if (msg_ptr == NULL)
      {
        ERR_FATAL("malloc(%d) failure",sizeof(struct tdsrrc_UL_DCCH_Message),0,0);
      }
      
      /* Identify this message as a Signalling Connection Release Indication Message */
      msg_ptr->message.t = T_tdsrrc_UL_DCCH_MessageType_signallingConnectionReleaseIndication;
      
      /* Initialize the bit mask for Signalling Connection Release Indication Message */
      TDSRRC_RESET_MSG_IE_PRESENT(msg_ptr->message.u.signallingConnectionReleaseIndication);
      
      /* Set the CN Domain ID from the MM primitive */
      msg_ptr->message.u.signallingConnectionReleaseIndication.cn_DomainIdentity = tdsrrc_CN_DomainIdentity_ps_domain;

      /*setting the IE "Signalling Connection Release Indication Cause" to "UE Requested PS Data session end"*/

      msg_ptr->message.u.signallingConnectionReleaseIndication.m.laterNonCriticalExtensionsPresent =1;

      TDSRRC_RESET_MSG_IE_PRESENT(msg_ptr->message.u.signallingConnectionReleaseIndication.laterNonCriticalExtensions);

      TDSRRC_MSG_COMMON_SET_BITMASK_IE_TYPE2(msg_ptr->message.u.signallingConnectionReleaseIndication.laterNonCriticalExtensions, v860nonCriticalExtentions);

      TDSRRC_RESET_MSG_IE_PRESENT(msg_ptr->message.u.signallingConnectionReleaseIndication.laterNonCriticalExtensions.v860nonCriticalExtentions);
  
      msg_ptr->message.u.signallingConnectionReleaseIndication.laterNonCriticalExtensions.
        v860nonCriticalExtentions.signallingConnectionReleaseIndication_v860ext.
        signallingConnectionReleaseIndicationCause = tdsrrc_SignallingConnectionReleaseIndication_v860ext_signallingConnectionReleaseIndicationCause_uERequestedPSDataSessionEnd;
  
      /* Send the Signalling Connection Release Request Message on the Uplink DCCH */
      send_status = tdsrrcsend_send_ul_sdu_with_mui(TDSRRC_PROCEDURE_DORMANCY,msg_ptr,ul_lc_id,
                                                 TDSRRCSEND_L2ACK_REQUIRED, &msg_mui);
#ifdef FEATURE_TDSCDMA_3GPP_FAST_DORMANCY_ENHANCEMENT
      if(tdsrrc_max_scri_for_fd != 0)
      {
        /* Decrement rrc_max_scri_for_fd as we have sent one SCRI */
        tdsrrc_max_scri_for_fd --;
      }
#endif
      if(FALSE == l2_ack_processing_required)
      {
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"SigConRel for Pending Fast dormancy sent cause %d ",tdsrrcdormancy_saved_fast_dormancy_req.mui_status);

        tdsrrcdormancy_init_saved_fast_dormancy_req();
        
        return;
      }

      if (send_status == TDSRRCSEND_SUCCESS)
      {
        tdsrrcdormancy_substate = TDSRRCDORMANCY_WAIT_FOR_L2ACK;
        tdsrrcdormancy_saved_fast_dormancy_req.mui= msg_mui;
  
        /* update the mui_status cause and also the flag to indicate that close_sess_req
               * or sig_con_rel_req has been already sent */
        tdsrrcdormancy_saved_fast_dormancy_req.mui_status = TDSRRCDORMANCY_MUI_SCRI_CNF; 

        TDSRRC_MSG1(MSG_LEGACY_HIGH,"SigConRel for Fast dormancy sent cause %d ",tdsrrcdormancy_saved_fast_dormancy_req.mui_status);
      }
      else
      {
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"Error %d sending SCRI for Fast dormancy",send_status);
        tdsrrcdormancy_reset_n_send_fast_dormancy_cnf(RRC_FAST_DORMANCY_FAILURE_APP_RETRY);
      }
    }
  }
}/*tdsrrcscrr_send_sig_conn_rel_req*/

/*===========================================================================
  
  FUNCTION tdsrrcdormancy_reset_n_send_fast_dormancy_cnf  
  
  DESCRIPTION
    This function sends RRC_FAST_DORMANCY_CNF command to NAS and reset dormancy
  
  DEPENDENCIES
    none.
  
  RETURN VALUE
    none 
===========================================================================*/
void tdsrrcdormancy_reset_n_send_fast_dormancy_cnf
(
  rrc_fast_dormancy_cnf_status_e_type cnf_cause
)
{
  mm_cmd_type *cmd_ptr;                      /* Pointer to the MM Command */

  /* Allocate memory for RRC_FAST_DORMANCY_CNF command */
  if( (cmd_ptr =  mm_tdsrrc_get_cmd_buf(RRC_FAST_DORMANCY_CNF)) != NULL)
  {
    /* Fill in parameters fro RRC_FAST_DORMANCY_CNF command */
    cmd_ptr->cmd.rrc_fast_dormancy_cnf.status = cnf_cause ;
#ifdef FEATURE_DUAL_SIM
    cmd_ptr->cmd.rrc_fast_dormancy_cnf.as_id = tdsrrc_get_as_id();
#endif

    /* Sends the command to MM */
    if(TDSCHECK_OPTIONAL_FEATURE_CFA_SUPPORTED)
    {
      tdscfa_mm_put_cmd(cmd_ptr);
    }
    mm_put_cmd(cmd_ptr);

    TDSRRC_MSG1(MSG_LEGACY_HIGH,"RRCDORM:Sent RRC_FAST_DORMANCY_CNF to MM with status =%d",cnf_cause);

    tdsrrcdormancy_init_saved_fast_dormancy_req();

  }
  else   /* RRC Task failed to allocate memory */
  {
    ERR_FATAL("Unable to allocate memory for MM Cmd: %d",RRC_FAST_DORMANCY_CNF,0,0);
  }     
}

/*===========================================================================
  
  FUNCTION tdsrrcdormancy_send_fast_dormancy_cnf  
  
  DESCRIPTION
    This function sends RRC_FAST_DORMANCY_CNF command to NAS without resetting dormancy.
  
  DEPENDENCIES
    none.
  
  RETURN VALUE
    none 
===========================================================================*/
static void tdsrrcdormancy_send_fast_dormancy_cnf
(
  rrc_fast_dormancy_cnf_status_e_type cnf_cause
)
{
  mm_cmd_type *cmd_ptr;                      /* Pointer to the MM Command */

  /* Allocate memory for RRC_FAST_DORMANCY_CNF command */
  if( (cmd_ptr =  mm_tdsrrc_get_cmd_buf(RRC_FAST_DORMANCY_CNF)) != NULL)
  {
    /* Fill in parameters fro RRC_FAST_DORMANCY_CNF command */
    cmd_ptr->cmd.rrc_fast_dormancy_cnf.status = cnf_cause ;

#ifdef FEATURE_DUAL_SIM
    cmd_ptr->cmd.rrc_fast_dormancy_cnf.as_id = tdsrrc_get_as_id();
#endif

    /* Sends the command to MM */
    if(TDSCHECK_OPTIONAL_FEATURE_CFA_SUPPORTED)
    {
      tdscfa_mm_put_cmd(cmd_ptr);
    }
    mm_put_cmd(cmd_ptr);

    TDSRRC_MSG1(MSG_LEGACY_HIGH,"RRCDORM:Sent RRC_FAST_DORMANCY_CNF to MM with status =%d",cnf_cause);
  }
  else   /* RRC Task failed to allocate memory */
  {
    ERR_FATAL("Unable to allocate memory for MM Cmd: %d",RRC_FAST_DORMANCY_CNF,0,0);
  }     
}


/*===========================================================================
  
  FUNCTION 
    tdsrrcdormancy_check_if_nw_support_fast_dormancy  

  DESCRIPTION
    This function check dormancy support of N/W

  DEPENDENCIES
    none.

  RETURN VALUE
    none 
===========================================================================*/
boolean tdsrrcdormancy_check_if_nw_support_fast_dormancy
(
  void
)
{
  if(tdsrrcdormancy_conn_timers_and_constants.T323_nw_support == TRUE)
    return TRUE;
  else
    return FALSE;  
}

/*===========================================================================
  
  FUNCTION 
    tdsrrcdormancy_get_nw_support_of_fast_dormancy_from_sib  

  DESCRIPTION
    This function check dormancy support of N/W after SIB reading 

  DEPENDENCIES
    none.

  RETURN VALUE
    none 
===========================================================================*/
boolean tdsrrcdormancy_get_nw_support_of_fast_dormancy_from_sib
(
  void
)
{
  tdsrrc_SysInfoType1 *sib1_ptr;
  boolean status = FALSE;
  
  sib1_ptr = (tdsrrc_SysInfoType1 *) tdsrrcsibdb_return_sib_for_srv_cell(tdsrrc_SIB1);
  if(sib1_ptr != NULL)
  {
    if(sib1_ptr->m.v3a0NonCriticalExtensionsPresent)
    {
      if(sib1_ptr->v3a0NonCriticalExtensions.m.v860NonCriticalExtentionsPresent)
      {
        if(sib1_ptr->v3a0NonCriticalExtensions.v860NonCriticalExtentions.sysInfoType1_v860ext.
          ue_ConnTimersAndConstants.m.t_323Present)
        {
          status = TRUE;
        }
      }
    }
  }
  return status;
}


/*===========================================================================
  
  FUNCTION 
    tdsrrcdormancy_stop_n_update_t323_timer_status  

  DESCRIPTION
    This function stops gurard timer for SRNS Relocation and update status of
    T323 to IDLE/RUNNING

  DEPENDENCIES
    none.

  RETURN VALUE
    none 
===========================================================================*/
tdsrrcdormancy_timer_status_e_type tdsrrcdormancy_stop_n_update_t323_timer_status
(
  void
)
{
  tdsrrcdormancy_timer_status_e_type status = TDSRRCDORMANCY_T323_TIMER_INVALID;

  TDSRRC_MSG2(MSG_LEGACY_HIGH,"Current T323 VALUE: %d STATUS: %d",tdsrrcdormancy_conn_timers_and_constants.T323_duration,tdsrrcdormancy_conn_timers_and_constants.T323_current_status);  

  switch(tdsrrcdormancy_conn_timers_and_constants.T323_current_status)
  {
    case TDSRRCDORMANCY_SRNS_GUARD_TIMER_RUNNING:
      tdsrrctmr_stop_timer(TDSRRCTMR_T_323_TIMER);

    /* Do nothing for other status */ 
    case TDSRRCDORMANCY_T323_TIMER_INVALID:
    case TDSRRCDORMANCY_T323_TIMER_IDLE:
      status = TDSRRCDORMANCY_T323_TIMER_IDLE;
      break;
      
    case TDSRRCDORMANCY_T323_TIMER_RUNNING:
      status = TDSRRCDORMANCY_T323_TIMER_RUNNING;
      break;      
  }  
  
  TDSRRC_MSG2(MSG_LEGACY_HIGH,"Updated T323 VALUE: %d STATUS: %d",tdsrrcdormancy_conn_timers_and_constants.T323_duration,tdsrrcdormancy_conn_timers_and_constants.T323_current_status);  

  return status;
}

/*===========================================================================
  
  FUNCTION 
    tdsrrcdormancy_disable_fast_dormancy  

  DESCRIPTION
    This function disables dormancy support of network

  DEPENDENCIES
    none.

  RETURN VALUE
    none 
===========================================================================*/
void tdsrrcdormancy_disable_fast_dormancy
(
void
)
{
  tdsrrcdormancy_conn_timers_and_constants.T323_nw_support = FALSE;
  tdsrrcdormancy_conn_timers_and_constants.T323_duration = tdsrrc_T_323_s0;
  tdsrrcdormancy_conn_timers_and_constants.T323_current_status = TDSRRCDORMANCY_T323_TIMER_INVALID;
}

/*===========================================================================
  
  FUNCTION 
    tdsrrcdormancy_reset_v316  

  DESCRIPTION
    This function reset V316 counter.

  DEPENDENCIES
    none.

  RETURN VALUE
    none 
===========================================================================*/
void tdsrrcdormancy_reset_v316
(
void
)
{
  TDSRRC_MSG0(MSG_LEGACY_HIGH,"Resetting V316");
  tdsrrc_v_316 = 0;
}

/*===========================================================================
  
  FUNCTION 
    tdsrrcdormancy_check_drx_cycle_n_v316_to_send_scri  

  DESCRIPTION
    This function checks drx cycle and decides whether scri needs to be send or not. The function returns TRUE
    if scri needs to be sended else returns FALSE.

  DEPENDENCIES
    none.

  RETURN VALUE
    boolean
===========================================================================*/
boolean tdsrrcdormancy_check_drx_cycle_n_v316_to_send_scri
(
void
)
{
  uint32 cs_drx=0, ps_drx=0, drx_in_use=0, shorter_cn_drx=0;

  tdsrrcllc_get_drx_info_values(&cs_drx, &ps_drx, &drx_in_use);

  /* We may not have a UTRAN DRX cycle length, so we can send SCRI */
  if(drx_in_use == 0)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"drx_in_use is 0,returning TRUE");
    return TRUE;
  }

  if(cs_drx < ps_drx)
    shorter_cn_drx = cs_drx;
  else
    shorter_cn_drx = ps_drx;

  TDSRRC_MSG2(MSG_LEGACY_HIGH,"shorter_cn_drx:%d tdsrrc_v_316:%d",shorter_cn_drx,tdsrrc_v_316);

  /* DRX cycle length in use is shorter than the shorter CN domain specific DRX 
     * cycle length for the PS domain and CS domain */
  if(drx_in_use < shorter_cn_drx)
  {
    return TRUE;
  }
  else 
  {
    /* DRX cycle length in use is equal to or longer than the shorter CN domain specific 
         * DRX cycle length for the PS domain and CS domain, and V316 < 1*/
    if(tdsrrc_v_316 < 1)
    {
      /* Increment V316 by 1*/
      tdsrrc_v_316++;
      return TRUE;
    }
    else
    {
      return FALSE;
    }
  }
}

/*===========================================================================
  
  FUNCTION 
    tdsrrcdormancy_check_n_reset_v316  

  DESCRIPTION
    This function checks the conditions for resetting v316 counter and reset
    if required.

  DEPENDENCIES
    none.

  RETURN VALUE
    none
===========================================================================*/
void tdsrrcdormancy_check_n_reset_v316
(
  tdsrrc_proc_e_type rrc_procedure
)
{
  uint8 rab_idx = 0;
  uint32 rb_count = 0;
  uint32 local_rlc_wm_size=0;
  tdsrrc_RB_Identity local_rb_id=0;
  uint8 local_lc_id=0;
  tdsrrc_state_e_type curr_state = tdsrrc_get_state();
  
  if((curr_state == TDSRRC_STATE_CELL_PCH) ||
      (curr_state == TDSRRC_STATE_URA_PCH ))
  {
    for(rab_idx = 0; rab_idx < MAX_RAB_TO_SETUP; rab_idx++)
    {
      if((tdsrrc_est_rabs.rabs[rab_idx].rab_id != TDSRRC_INVALID_RAB_ID) && 
          (tdsrrc_est_rabs.rabs[rab_idx].cn_domain == RRC_PS_DOMAIN_CN_ID) &&
          (tdsrrc_est_rabs.rabs[rab_idx].num_rbs_for_rab > 0))
      {
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"PS domain RAB:%d",tdsrrc_est_rabs.rabs[rab_idx].rab_id);
        for (rb_count=0; rb_count<tdsrrc_est_rabs.rabs[rab_idx].num_rbs_for_rab; rb_count++)
        {
          local_rb_id = tdsrrc_est_rabs.rabs[rab_idx].rb_for_rab[rb_count].rb_id;
          local_lc_id = tdsrrclcm_get_ul_rlc_lc_id(local_rb_id);
          local_rlc_wm_size = tdsrlc_rrc_get_wm_info_req(local_lc_id);
          
          TDSRRC_MSG3(MSG_LEGACY_HIGH,"RB:%d LC_ID:%d WM_SIZE:%d",local_rb_id,local_lc_id,local_rlc_wm_size);

          if((rrc_procedure == TDSRRC_PROCEDURE_IDT) || (local_rlc_wm_size > 0))
          {
            /* Reset V316 and return */ 
            tdsrrcdormancy_reset_v316();
            return;
          }
        }
      }
    }
  }
}

/*===========================================================================
  
  FUNCTION 
    tdsrrcdormancy_update_dormancy_for_srns_hho_cco  

  DESCRIPTION
    This function updates dormacy procedure if SRNS relocation is in progress. 
    If so it will abort & send RRC_FAST_DORMANCY_CNF command to NAS.

  DEPENDENCIES
    none.

  RETURN VALUE
    none 
===========================================================================*/
void tdsrrcdormancy_update_dormancy_for_srns_hho_cco
(
  tdsrrc_proc_e_type rrc_proc
)
{
  TDSRRC_MSG1(MSG_LEGACY_HIGH,"tdsrrcdormancy_update_dormancy_for_srns_hho_cco from PROC:%d",rrc_proc);

  /* Reset V316 */ 
  tdsrrcdormancy_reset_v316();

  switch(rrc_proc)
  {
    case TDSRRC_PROCEDURE_UMI: 
      {
        if(FALSE == tdsrrcdormancy_check_if_nw_support_fast_dormancy())        
        {
          /* Falling back to legacy SCRI. Stop T323 timer if it is running */
          tdsrrcdormancy_stop_or_restart_timer_T323(TDSRRCDORM_STOP_T323_TIMER_VALUE,
            TDSRRCDORMANCY_T323_TIMER_INVALID);

          tdsrrcdormancy_check_and_abort_dormancy_req(RRC_FAST_DORMANCY_FAILURE_APP_RETRY);    
        }
      }
      break;  
    case TDSRRC_PROCEDURE_RBE:      
    case TDSRRC_PROCEDURE_RBRC:
    case TDSRRC_PROCEDURE_PCR:       
    case TDSRRC_PROCEDURE_TCR: 
    case TDSRRC_PROCEDURE_RBR:        
    case TDSRRC_PROCEDURE_CU: 
    case TDSRRC_PROCEDURE_CHO:      
    case TDSRRC_PROCEDURE_IHO:      
      {
        if (rrc_proc == TDSRRC_PROCEDURE_CHO)
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"CCO in progress");
        }
        else if (rrc_proc == TDSRRC_PROCEDURE_IHO)
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"W2G HHO in progress");
        }
        else
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"SRNS in progress");
        }

        if((rrc_proc != TDSRRC_PROCEDURE_CHO) &&
           (rrc_proc != TDSRRC_PROCEDURE_IHO) &&
           (TDSRRC_STATE_CELL_FACH == tdsrrc_get_state())&&
           (TRUE == tdsrrcdormancy_get_nw_support_of_fast_dormancy_from_sib()))
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"SRNS NEW RNC supports 3GPP Dormancy");
        }
        else
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"SRNS NEW RNC support for 3GPP Dormancy Unknown");
          tdsrrcdormancy_check_and_abort_dormancy_req(RRC_FAST_DORMANCY_FAILURE_APP_RETRY);    
          /* Disable Fast Dormancy till N/W indicates support of Fast Dormancy */
          tdsrrcdormancy_disable_fast_dormancy();
          /* If T323 is not running START it with SRNS guard timer duration */
          tdsrrcdormancy_stop_or_restart_timer_T323(TDSRRCDORM_SRNS_GUARD_TIMER_VALUE,
            TDSRRCDORMANCY_SRNS_GUARD_TIMER_RUNNING);
        }
      }      
      break;

    default:
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"Invalid Proc %d in update_dormancy_for_srns",rrc_proc);
  }
}

/*===========================================================================
  
  FUNCTION 
    tdsrrcdormancy_get_T323_timer_value  

  DESCRIPTION
    This function check returns the value of T323 timer provided by Network
    in milliseconds.

  DEPENDENCIES
    none.

  RETURN VALUE
    none 
===========================================================================*/
static uint32 tdsrrcdormancy_get_T323_timer_value
(
  void
)
{
  uint32 ret_val = 0;

  if(tdsrrcdormancy_conn_timers_and_constants.T323_nw_support == TRUE)
  {
    switch(tdsrrcdormancy_conn_timers_and_constants.T323_duration)
    {
      case tdsrrc_T_323_s0:
        ret_val = 0;
        break;

      case tdsrrc_T_323_s5:
        ret_val = 5000;
        break;

      case tdsrrc_T_323_s10:
        ret_val = 10000;
        break;

      case tdsrrc_T_323_s20:
        ret_val = 20000;
        break;

      case tdsrrc_T_323_s30:
        ret_val = 30000;
        break;

      case tdsrrc_T_323_s60:
        ret_val = 60000;
        break;

      case tdsrrc_T_323_s90:
        ret_val = 90000;
        break;

      case tdsrrc_T_323_s120:
        ret_val = 120000;
        break;        

      default:
        ret_val = 0;
    }
  }
  return ret_val;
}

/*===========================================================================
  
  FUNCTION tdsrrcdormancy_process_T323_expiry  
  
  DESCRIPTION
 This function process T323 timer expiry.
  
  DEPENDENCIES
    none.
  
  RETURN VALUE
    none 
===========================================================================*/
void tdsrrcdormancy_process_T323_expiry
(
  tdsrrc_cmd_type *cmd_ptr
)
{
  TDSRRC_MSG0(MSG_LEGACY_HIGH,"T323 Expiry");
#ifdef FEATURE_TDSCDMA_3GPP_FAST_DORMANCY_ENHANCEMENT
/*tdsrlc_query_data_channel_activity need RLC provide function call.  */
  if(tdsrlc_query_data_channel_activity() == NO_DATA_TRANSFERRED)
  {
    /* No data activity, mark num_pending_dormancy_req to TRUE */
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"RRCDORM: RRC state %d",tdsrrc_get_state());
    if(((tdsrrc_get_state() == TDSRRC_STATE_CELL_DCH) || 
        (tdsrrc_get_state() == TDSRRC_STATE_CELL_FACH)) &&
       (tdsrrc_max_scri_for_fd != 0)
      )
    {
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"RRCDORM: in %d (FACH/DCH) increment pending ",tdsrrc_get_state());
      tdsrrcdormancy_saved_fast_dormancy_req.num_pending_dormancy_req++;
    }
    else if(((tdsrrc_get_state() == TDSRRC_STATE_URA_PCH) || 
             (tdsrrc_get_state() == TDSRRC_STATE_CELL_PCH)) &&
             (tdsrrc_max_scri_for_fd != 0)
           )
    {
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"RRCDORM: in %d(PCH) increment pending and set max_scri ",tdsrrc_get_state());
      tdsrrcdormancy_saved_fast_dormancy_req.num_pending_dormancy_req++;
      return;
    }
  }
  else
  {
    /* There is some data activity on PS RB, reset num_pending_dormancy_req */
    tdsrrcdormancy_saved_fast_dormancy_req.num_pending_dormancy_req =0;
  }
#endif
  if(tdsrrcdormancy_saved_fast_dormancy_req.num_pending_dormancy_req != 0)
  {
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"Pending Dormancy requests:%d",tdsrrcdormancy_saved_fast_dormancy_req.num_pending_dormancy_req);
    /* If all validation goes well then process fast dormancy req and start T323 timer*/
    if(TRUE == tdsrrcdormancy_process_pending_fast_dormancy_req())
    {
      if(TRUE == tdsrrcdormancy_check_if_nw_support_fast_dormancy())
      {
        tdsrrcdormancy_stop_or_restart_timer_T323(tdsrrcdormancy_get_T323_timer_value(),
          TDSRRCDORMANCY_T323_TIMER_RUNNING); 
        return;
      }
      else
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"N/W dint support T323. Do Not Start timer");
      } 
    }
  }
  
  if(TRUE == tdsrrcdormancy_check_if_nw_support_fast_dormancy())
    tdsrrcdormancy_conn_timers_and_constants.T323_current_status = TDSRRCDORMANCY_T323_TIMER_IDLE;
  else
    tdsrrcdormancy_conn_timers_and_constants.T323_current_status = TDSRRCDORMANCY_T323_TIMER_INVALID;

  TDSRRC_MSG1(MSG_LEGACY_HIGH,"Resetting T323 Status: %d",tdsrrcdormancy_conn_timers_and_constants.T323_current_status);
}

/*===========================================================================
  
  FUNCTION tdsrrcdormancy_stop_or_restart_timer_T323  

  DESCRIPTION
    This function check if T323 is already running then STOP it and restart with the given time.
    IF timer_value == 0, then it STOPS it.
    
  DEPENDENCIES
    none.

  RETURN VALUE
    none 
===========================================================================*/
void tdsrrcdormancy_stop_or_restart_timer_T323
(
  unsigned long timer_value,
  tdsrrcdormancy_timer_status_e_type timer_status
)
{
  uint64 remaining_value = 0;
  
  TDSRRC_MSG2(MSG_LEGACY_HIGH,"Current T323 VALUE: %d STATUS: %d",tdsrrcdormancy_conn_timers_and_constants.T323_duration,tdsrrcdormancy_conn_timers_and_constants.T323_current_status);  

  if(timer_value == TDSRRCDORM_STOP_T323_TIMER_VALUE)
  {
    if(0 != tdsrrctmr_get_remaining_time(TDSRRCTMR_T_323_TIMER))
    {
       tdsrrctmr_stop_timer(TDSRRCTMR_T_323_TIMER);
    }
    tdsrrcdormancy_conn_timers_and_constants.T323_current_status = timer_status;

    TDSRRC_MSG1(MSG_LEGACY_HIGH,"STOP T323,STATUS: %d",timer_status);  
    return;
  }
    
  remaining_value = tdsrrctmr_get_remaining_time(TDSRRCTMR_T_323_TIMER);

  if(remaining_value < timer_value)
  {
    if(remaining_value != 0)
    {
      tdsrrctmr_stop_timer(TDSRRCTMR_T_323_TIMER);
    }
    tdsrrctmr_start_timer(TDSRRCTMR_T_323_TIMER, timer_value);
    
   tdsrrcdormancy_conn_timers_and_constants.T323_current_status = timer_status;    

   TDSRRC_MSG2(MSG_LEGACY_HIGH,"START T323,VALUE: %d STATUS: %d",timer_value,timer_status);  
  } 
  
}

/*===========================================================================
  
  FUNCTION tdsrrcdormancy_check_and_abort_dormancy_req  
  
  DESCRIPTION
 This function check if dormancy req is in progress. if so it will abort & send 
 RRC_FAST_DORMANCY_CNF command to NAS.
  
  DEPENDENCIES
    none.
  
  RETURN VALUE
    none 
===========================================================================*/
void tdsrrcdormancy_check_and_abort_dormancy_req
(
  rrc_fast_dormancy_cnf_status_e_type abort_type
)
{
  if((tdsrrcdormancy_saved_fast_dormancy_req.fast_dormancy_req_status == TRUE) ||
     (tdsrrcdormancy_saved_fast_dormancy_req.mui_status != TDSRRCDORMANCY_MUI_STATUS_INVALID))
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Dormancy_req is in progress,lets abort");
    tdsrrcdormancy_reset_n_send_fast_dormancy_cnf(abort_type);
  }
  else
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"No dormancy_req in progress");
  }
}

/*===========================================================================
  
  FUNCTION tdsrrcdormancy_check_dormancy_initiated_scrr_active  
  
  DESCRIPTION
 This function check whether dormancy initiated SCRR is active or NOT.
  
  DEPENDENCIES
    none.
  
  RETURN VALUE
    none 
===========================================================================*/
boolean tdsrrcdormancy_check_dormancy_initiated_scrr_active
(
  void
)
{
  if((tdsrrcdormancy_saved_fast_dormancy_req.fast_dormancy_req_status == TRUE) &&
     (tdsrrcdormancy_substate == TDSRRCDORMANCY_WAIT_FOR_SIG_CON_REL_COMPL) &&
     (tdsrrcdormancy_saved_fast_dormancy_req.mui_status == TDSRRCDORMANCY_MUI_SIG_CON_REL_CNF))
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Dormancy initiated SCRI in progress");    
    return TRUE;
  }
  else
    return FALSE;
}

/*===========================================================================
  
  FUNCTION tdsrrcdormancy_is_dormancy_active  
  
  DESCRIPTION
 This function check if dormancy req is ACTIVE or NOT.
  
  DEPENDENCIES
    none.
  
  RETURN VALUE
    none 
===========================================================================*/
boolean tdsrrcdormancy_is_dormancy_active
(
  void
)
{
  if((tdsrrcdormancy_saved_fast_dormancy_req.fast_dormancy_req_status == TRUE) ||
     (tdsrrcdormancy_saved_fast_dormancy_req.mui_status != TDSRRCDORMANCY_MUI_STATUS_INVALID))
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Dormancy is Active");
    return TRUE;
  }
  else
  {
    return FALSE;
  }
}

/*===========================================================================
  
  FUNCTION tdsrrcdormancy_check_n_send_cnf_for_failure_conditions  
  
  DESCRIPTION
 This function check if dormancy req is in progress. if so it will abort & send 
 RRC_FAST_DORMANCY_CNF command to NAS.
  
  DEPENDENCIES
    none.
  
  RETURN VALUE
    boolean 
===========================================================================*/
boolean tdsrrcdormancy_check_n_send_cnf_for_failure_conditions
(
  void
)
{
  tdsrrc_state_e_type rrc_state = tdsrrc_get_state();
  
  if((TRUE == tdsrrcrb_is_srns_in_progress()) ||
     (TRUE == tdsrrcsmc_is_rlc_re_est_for_srns_active()))
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"SRNS in progress. Send FAILURE for Dormancy");
    
    tdsrrcdormancy_reset_n_send_fast_dormancy_cnf(RRC_FAST_DORMANCY_FAILURE_APP_RETRY);    
    return TRUE;    
  }

  if(FALSE == tdsrrcscrr_is_cn_domain_open(RRC_PS_DOMAIN_CN_ID))
  {
    TDSRRC_MSG2(MSG_LEGACY_HIGH,"SCRR active for PS domain. Aborting Dormancy dorm-state:%d rrc-state:%d",tdsrrcdormancy_substate,rrc_state);
    tdsrrcdormancy_reset_n_send_fast_dormancy_cnf(RRC_FAST_DORMANCY_SUCCESS_APP_NO_RETRY);    
    return TRUE;
  }
  else if(TRUE == tdsrrciho_is_iho_td2g_active())
  {
    TDSRRC_MSG2(MSG_LEGACY_HIGH,"IRAT-HO active. Aborting Dormancy dorm-state:%d rrc-state:%d",tdsrrcdormancy_substate,rrc_state);
   
    tdsrrcdormancy_reset_n_send_fast_dormancy_cnf(RRC_FAST_DORMANCY_SUCCESS_APP_NO_RETRY);    
    return TRUE;
  }
  else if(TRUE == tdsrrcidt_is_idt_active(RRC_PS_DOMAIN_CN_ID))
  {
    TDSRRC_MSG2(MSG_LEGACY_HIGH,"IDT-PS is active.  Aborting Dormancy dorm-state:%d rrc-state:%d",tdsrrcdormancy_substate,rrc_state);
    tdsrrcdormancy_reset_n_send_fast_dormancy_cnf(RRC_FAST_DORMANCY_FAILURE_APP_RETRY);
    return TRUE;
  }
  else if(TRUE == tdsrrcidt_is_idt_active(RRC_CS_DOMAIN_CN_ID))
  {
    TDSRRC_MSG2(MSG_LEGACY_HIGH,"IDT-CS is active.  Aborting Dormancy dorm-state:%d rrc-state:%d",tdsrrcdormancy_substate,rrc_state);
    tdsrrcdormancy_reset_n_send_fast_dormancy_cnf(RRC_FAST_DORMANCY_FAILURE_APP_RETRY);
    return TRUE;
  }
  else if(TRUE == tdsrrccho_is_td2g_cco_active())
  {
    TDSRRC_MSG2(MSG_LEGACY_HIGH,"CCO is active.  Aborting Dormancy dorm-state:%d rrc-state:%d",tdsrrcdormancy_substate,rrc_state);
    
    tdsrrcdormancy_reset_n_send_fast_dormancy_cnf(RRC_FAST_DORMANCY_FAILURE_APP_RETRY);
    return TRUE;    
  }

  return FALSE;
}

/*===========================================================================
  
  FUNCTION tdsrrcdormancy_validate_fast_dormancy_req  
  
  DESCRIPTION
    This function validates TDSRRC_FAST_DORMANCY_REQ command received from MM task.
  
  DEPENDENCIES
    This function should be called only if the command is TDSRRC_FAST_DORMANCY_REQ .
  
  RETURN VALUE
    tdsrrcdormancy_status_e_type: It returns TDSRRCDORMANCY_VALIDATION_SUCCESS if it is successfully validates 
    the TDSRRC_FAST_DORMANCY_REQ command. Otherwise it returns fail cause
  
===========================================================================*/
static tdsrrcdormancy_validation_status_e_type tdsrrcdormancy_validate_fast_dormancy_req
(
  tdsrrc_cmd_type *cmd_ptr                 /* Pointer to the RRC Command */
)
{
  tdsrrcdormancy_validation_status_e_type status = TDSRRCDORMANCY_VALIDATION_SUCCESS; 

  if(cmd_ptr->cmd_hdr.cmd_id != TDSRRC_FAST_DORMANCY_REQ)
  {
    TDSRRC_MSG1(MSG_LEGACY_ERROR,"RRCDORM:Invalid cmd 0x%x",cmd_ptr->cmd_hdr.cmd_id);
    return (TDSRRCDORMANCY_VALIDATION_FAIL);
  }

  /* Check to see if a signalling session to the requested PS domain is open */
  if ((status == TDSRRCDORMANCY_VALIDATION_SUCCESS) &&
      (tdsrrcscrr_is_cn_domain_open(RRC_PS_DOMAIN_CN_ID) == FALSE))
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"RRCDORM:PS domain not open");
    status = TDSRRCDORMANCY_VALIDATION_FAIL_DOMAIN_NOT_OPEN;
  }
  
  /* Check to see if a signalling session to the requested PS domain is open */
  if ((status == TDSRRCDORMANCY_VALIDATION_SUCCESS) &&
      (tdsrrcscrr_is_cn_domain_open(RRC_CS_DOMAIN_CN_ID) == TRUE))
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"RRCDORM:CS domain is also open");
    status = TDSRRCDORMANCY_VALIDATION_FAIL_CS_DOMAIN_OPEN;
  }
  
  /* Lets check if RRC is in IDLE mode */
  if ((status == TDSRRCDORMANCY_VALIDATION_SUCCESS) &&
       (tdsrrc_get_state() == TDSRRC_STATE_DISCONNECTED))
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"RRCDORM:RRC in IDLE");
    status = TDSRRCDORMANCY_VALIDATION_FAIL_RRC_CON_NOT_OPEN;
  }

  /*Lets check if RRC connection is getting release */
  if ((status == TDSRRCDORMANCY_VALIDATION_SUCCESS) &&
       ((tdsrrcrce_get_rrc_connection_status() == TDSRRCRCE_RRC_CONN_NOT_ESTABLISHED)||
        (tdsrrcrcr_is_proc_active() == TRUE)))
  {
    TDSRRC_MSG2(MSG_LEGACY_ERROR,"RRCDORM:RRC connection not open[RCE state-%d,RCR state-%d]",tdsrrcrce_get_rrc_connection_status(),tdsrrcrcr_is_proc_active());
    status = TDSRRCDORMANCY_VALIDATION_FAIL_RRC_CON_NOT_OPEN;
  }
    
  if((status == TDSRRCDORMANCY_VALIDATION_SUCCESS) &&
     (TDSRRC_NOT_CAMPED_ON == tdsrrcccm_get_curr_camping_status(TDSRRC_PROCEDURE_DORMANCY)))
  {
    /* For FAST  DORMANCY feature, If UE is in OOS, 
        * Then return RRC_FAST_DORMANCY_CNF with failuer cause 
        * NAS needs to return the FAST DORMANCY status Immediately to upper layer
        */
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"RRCDORM:UE in OOS");
    status = TDSRRCDORMANCY_VALIDATION_FAIL_UE_OOS;
  }

  if((status == TDSRRCDORMANCY_VALIDATION_SUCCESS) &&
     (tdsrrccsp_get_curr_scan() == TDSRRC_CSP_LEAVING_CONN_MODE_SCAN))
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"UE is LeavingConnModeScan");
    status = TDSRRCDORMANCY_VALIDATION_FAIL_UE_LEAVING_CONN_MODE_SCAN;
  }

  if((status == TDSRRCDORMANCY_VALIDATION_SUCCESS) &&
     (0 != tdsrrctmr_get_remaining_time(TDSRRCTMR_T_323_TIMER))
    )
  {
    /* For FAST  DORMANCY feature, If T323 timer is running, 
        * Then return RRC_FAST_DORMANCY_CNF with failure cause 
        * NAS needs to return the FAST DORMANCY status Immediately to upper layer
        */
    TDSRRC_MSG2(MSG_LEGACY_ERROR,"Timer T323 is running RRC State:%d Dormancy Substate",tdsrrc_get_state(),tdsrrcdormancy_substate);

    tdsrrcdormancy_saved_fast_dormancy_req.num_pending_dormancy_req++;

    status = TDSRRCDORMANCY_VALIDATION_FAIL_T323_TIMER_RUNNING;
  }

  if(tdsrrcdormancy_saved_fast_dormancy_req.fast_dormancy_req_status == TRUE)
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"RRCDORM:One fast_dormancy_req is in progress,2nd req not supported");
    status = TDSRRCDORMANCY_VALIDATION_FAIL;
  }

  return (status);
} /* tdsrrcdormancy_validate_fast_dormancy_req */

/*===========================================================================
  
  FUNCTION tdsrrcdormancy_send_sig_conn_rel_req
  
  DESCRIPTION
    This function sends TDSRRC_SIG_CONN_RELEASE_REQ to DT
  
  DEPENDENCIES
    None.
  
  RETURN VALUE
    None.
  
  ===========================================================================*/
void tdsrrcdormancy_send_sig_conn_rel_req
(
  void
)
{
  tdsrrc_cmd_type *rrcdormancy_sig_conn_req_ptr; 

  TDSRRC_MSG0(MSG_LEGACY_HIGH,"Sending TDSRRC_SIG_CONN_RELEASE_REQ");

  /* Allocates the buffer to RRC Internal command */
  if((rrcdormancy_sig_conn_req_ptr =  tdsrrc_get_int_cmd_buf()) != NULL)
  { 
    rrcdormancy_sig_conn_req_ptr->cmd_hdr.cmd_id = TDSRRC_SIG_CONN_RELEASE_REQ;
    rrcdormancy_sig_conn_req_ptr->cmd.rrc_sig_conn_req.cn_domain_id =
      RRC_PS_DOMAIN_CN_ID;
    rrcdormancy_sig_conn_req_ptr->cmd.rrc_sig_conn_req.sig_rel_complete_cnf_required = 
      TRUE;
    rrcdormancy_sig_conn_req_ptr->cmd.rrc_sig_conn_req.rrc_proc = 
      TDSRRC_PROCEDURE_DORMANCY;
    /* sends the command to Signalling Connection Release Request Proc */
    tdsrrc_put_int_cmd(rrcdormancy_sig_conn_req_ptr);   
  }
  else
  {
    ERR_FATAL("Mem alloc Failed",0,0,0);
  }

  tdsrrcdormancy_substate = TDSRRCDORMANCY_WAIT_FOR_SIG_CON_REL_COMPL;
  tdsrrcdormancy_saved_fast_dormancy_req.mui_status = TDSRRCDORMANCY_MUI_SIG_CON_REL_CNF;
}


/*===========================================================================
  
  FUNCTION tdsrrcdormancy_process_fast_dormancy_req
  
  DESCRIPTION
    This function processes fast_dormancy_req
  
  DEPENDENCIES
    None.
  
  RETURN VALUE
    None.
  
 ===========================================================================*/
static boolean tdsrrcdormancy_process_fast_dormancy_req
(
  void
)
{
  boolean status = FALSE;
  
  if(TRUE == tdsrrcdormancy_check_n_send_cnf_for_failure_conditions())
  {
    /* Code to send _DORMANCY_CNF is inside the function */
    return FALSE;
  }

  if(FALSE == tdsrrcdormancy_check_if_nw_support_fast_dormancy())
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"NW Didn't suppport Dormancy.");
    tdsrrcdormancy_send_sig_conn_rel_req();
    return FALSE;
  }

  TDSRRC_MSG0(MSG_LEGACY_HIGH,"NW Suppport Fast Dormancy.");

  switch(tdsrrc_get_state())
  {
	case TDSRRC_STATE_URA_PCH:
	case TDSRRC_STATE_CELL_PCH:
    if(FALSE == tdsrrcdormancy_check_drx_cycle_n_v316_to_send_scri())
    {
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"Drop SCRI in rrc_state:%d ",tdsrrc_get_state());
      tdsrrcdormancy_reset_n_send_fast_dormancy_cnf(RRC_FAST_DORMANCY_FAILURE_APP_RETRY);
      return FALSE;
    }
    if(TDSRRCCU_NOT_COMPLETED == tdsrrccu_get_cell_update_complete_status(TDSRRC_PROCEDURE_DORMANCY))
    {
      /* Wait until Cell update procedure is completed */
      tdsrrcdormancy_substate = TDSRRCDORMANCY_WAIT_FOR_CU_COMP_CNF;  
    }
    else /* Cell Update procedure is inactive */
    {
#ifdef FEATURE_TDSCDMA_REL7_SPECRS 
      /*Check if T320 timer is running then send cnf with failure 
           * Till this timer is running, CU wont be initiated */
      if(0 != tdsrrctmr_get_remaining_time(TDSRRCTMR_T_320_TIMER))
      {
        /* CU guard Timer is running */
        TDSRRC_MSG2(MSG_LEGACY_ERROR,"CU Guard Timer T320 is running RRC State:%d Dormancy Substate",tdsrrc_get_state(),tdsrrcdormancy_substate);        
        tdsrrcdormancy_reset_n_send_fast_dormancy_cnf(RRC_FAST_DORMANCY_FAILURE_APP_RETRY);
      }
      else 
#endif
      {
        /* If Dormancy request is sent recently then defer next dormancy till T323 expiry */
        if(tdsrrctmr_get_remaining_time(TDSRRCTMR_T_323_TIMER))
        {
          tdsrrcdormancy_reset_n_send_fast_dormancy_cnf(RRC_FAST_DORMANCY_FAILURE_APP_RETRY);
          tdsrrcdormancy_saved_fast_dormancy_req.num_pending_dormancy_req++;
        }
        else
        {
          /* Cell Update with uplink data transmission */
          tdsrrcdormancy_send_cell_update_initiate_req(TDSRRC_PROCEDURE_DORMANCY);
          tdsrrcdormancy_substate = TDSRRCDORMANCY_WAIT_FOR_CU_COMP_CNF;  
        }
      }
    }    
    break;
  
  case TDSRRC_STATE_CELL_DCH:
  case TDSRRC_STATE_CELL_FACH:
    if(TDSRRCCU_NOT_COMPLETED == tdsrrccu_get_cell_update_complete_status(TDSRRC_PROCEDURE_DORMANCY))
    {
      /* Wait until Cell update procedure is completed */
      tdsrrcdormancy_substate = TDSRRCDORMANCY_WAIT_FOR_CU_COMP_CNF;  
    }
    else /* Cell Update procedure is inactive */
    {
      /* Build and send the Signalling Connection Release Request to Send Chain */
#ifdef FEATURE_TDSCDMA_3GPP_FAST_DORMANCY_ENHANCEMENT
      /* Re try SCRI only if rrc_max_scri_for_fd is non zero */
      if(tdsrrc_max_scri_for_fd != 0)
#endif
      {
        tdsrrcdormancy_send_scri_for_fast_dormancy(TRUE);
      }
      status = TRUE;
    }    
    break;

  default:
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"SCRR:Invalid Event in rrc state: %d",tdsrrc_get_state());
    break;  
  }  

  return status;
}


/*===========================================================================
  
  FUNCTION tdsrrcdormancy_process_pending_fast_dormancy_req
  
  DESCRIPTION
    This function processes tdsrrcdormancy_process_pending_fast_dormancy_req
  
  DEPENDENCIES
    None.
  
  RETURN VALUE
    boolean.
  
 ===========================================================================*/
static boolean tdsrrcdormancy_process_pending_fast_dormancy_req
(
  void
)
{
  boolean status = FALSE;
  
  TDSRRC_MSG2(MSG_LEGACY_HIGH,"Pending SCRR: %d RRC State:%d",tdsrrcdormancy_saved_fast_dormancy_req.num_pending_dormancy_req,tdsrrc_get_state());

  if( (TRUE == tdsrrcrb_is_srns_in_progress()) 
      || (TRUE == tdsrrcsmc_is_rlc_re_est_for_srns_active())
      || (FALSE == tdsrrcscrr_is_cn_domain_open(RRC_PS_DOMAIN_CN_ID))
      || (TRUE == tdsrrciho_is_iho_td2g_active())
      || (TRUE == tdsrrcidt_is_idt_active(RRC_PS_DOMAIN_CN_ID))
      || (TRUE == tdsrrcidt_is_idt_active(RRC_CS_DOMAIN_CN_ID))
      || (TRUE == tdsrrccho_is_td2g_cco_active())
      || (FALSE == tdsrrcdormancy_check_if_nw_support_fast_dormancy())
      || (TRUE == tdsrrcscrr_is_cn_domain_open(RRC_CS_DOMAIN_CN_ID))
    )
  {
    tdsrrcdormancy_saved_fast_dormancy_req.num_pending_dormancy_req = 0;
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Aborting pending SCRR");
    return FALSE;
  }

  switch(tdsrrc_get_state())
  {
    case TDSRRC_STATE_URA_PCH:
    case TDSRRC_STATE_CELL_PCH:
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"RRC State %d Aborting pending SCRR",tdsrrc_get_state());
      tdsrrcdormancy_saved_fast_dormancy_req.num_pending_dormancy_req = 0;
      break;
      
    case TDSRRC_STATE_CELL_DCH:
    case TDSRRC_STATE_CELL_FACH:
      if(TDSRRCCU_STARTED == tdsrrccu_get_cell_update_started_status())
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"CU Started Aborting pending SCRR");
        tdsrrcdormancy_saved_fast_dormancy_req.num_pending_dormancy_req = 0;
      }
      else 
      {
        /* Check if dormancy already in progress */
        if((tdsrrcdormancy_saved_fast_dormancy_req.fast_dormancy_req_status == TRUE) ||
            (tdsrrcdormancy_saved_fast_dormancy_req.mui_status != TDSRRCDORMANCY_MUI_STATUS_INVALID))
        {
          /* Do not process pending dormancy requests */
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Fast Dormancy Active. Aborting pending SCRR");
          tdsrrcdormancy_saved_fast_dormancy_req.num_pending_dormancy_req = 0;
        }
        else
        {
          /* Build and send the Signalling Connection Release Request to Send Chain */
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Sending pending SCRR");
          tdsrrcdormancy_saved_fast_dormancy_req.fast_dormancy_req_status = TRUE;
#ifdef FEATURE_TDSCDMA_3GPP_FAST_DORMANCY_ENHANCEMENT
        /* Re try SCRI only if rrc_max_scri_for_fd is non zero */
          if(tdsrrc_max_scri_for_fd != 0)
#endif
         {
           tdsrrcdormancy_send_scri_for_fast_dormancy(FALSE);
         }
          status = TRUE;
        }
      }    
      break;

    default:
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"Pending SCRR:Invalid Event in rrc state: %d",tdsrrc_get_state());
      tdsrrcdormancy_saved_fast_dormancy_req.num_pending_dormancy_req = 0;
      break;
  }  
  tdsrrcdormancy_saved_fast_dormancy_req.num_pending_dormancy_req = 0;
  return status;
}

/*===========================================================================
  
  FUNCTION tdsrrcdormancy_validate_and_process_fast_dormancy_req
  
  DESCRIPTION
    This function processes tdsrrcdormancy_validate_and_process_fast_dormancy_req
  
  DEPENDENCIES
    None.
  
  RETURN VALUE
    None.
  
  ===========================================================================*/
  
static void tdsrrcdormancy_validate_and_process_fast_dormancy_req
(
  tdsrrc_cmd_type *cmd_ptr
)
{
  tdsrrcdormancy_validation_status_e_type validation_status;
#ifdef FEATURE_TDSCDMA_3GPP_FAST_DORMANCY_ENHANCEMENT
  /* Set rrc_max_scri_for_fd to MAX */
  tdsrrc_max_scri_for_fd = TDSRRC_DORMANCY_MAX_SCRI_RETRY;
#endif
  TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRCDORM:Received RRC_FAST_DORMANCY_REQ");
/* Here need RLC provide function call */
#ifdef FEATURE_TDSCDMA_3GPP_FAST_DORMANCY_ENHANCEMENT
  tdsrlc_reset_activity_flag();
#endif
  if((validation_status = tdsrrcdormancy_validate_fast_dormancy_req(cmd_ptr)) != 
     TDSRRCDORMANCY_VALIDATION_SUCCESS)
  {
    tdsrrcdormancy_reset_n_send_fast_dormancy_cnf(
    tdsrrcdormancy_map_internal_cause_to_fast_dormancy_cnf_cause(validation_status)
    );
    return;
  }

  if(TDSRRCDORMANCY_SUCCESS != tdsrrcdormancy_save_fast_dormancy_req(cmd_ptr))
  {
    /* If there are multiple dormancy requests, then just trash it */
    TDSRRC_MSG2(MSG_LEGACY_HIGH,"RRCDORM:Failed to process Fast Dormancy Req in: %d,%d",tdsrrc_get_state(),tdsrrcdormancy_substate);
    tdsrrcdormancy_reset_n_send_fast_dormancy_cnf(RRC_FAST_DORMANCY_SUCCESS_APP_NO_RETRY);
    return;
  }

  /* If all validation goes well then process fast dormancy req and start T323 timer*/
  if(TRUE == tdsrrcdormancy_process_fast_dormancy_req())
  {
    if(TRUE == tdsrrcdormancy_check_if_nw_support_fast_dormancy())
    {
      tdsrrcdormancy_stop_or_restart_timer_T323(tdsrrcdormancy_get_T323_timer_value(),
        TDSRRCDORMANCY_T323_TIMER_RUNNING); 
    }
    else
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"N/W dint support T323. Do Not Start timer");
    }    
  }
}
/*===========================================================================
  
  FUNCTION tdsrrcdormancy_process_wait_for_l2_ack_substate
  
  DESCRIPTION
    This function processes all events that are  dispatched
    to fast dormancy  procedure. in TDSRRCDORMANCY_WAIT_FOR_L2ACK
    substate
  
  DEPENDENCIES
    None.
  
  RETURN VALUE
    None.
  
  ===========================================================================*/
static void tdsrrcdormancy_process_wait_for_l2_ack_substate
(
  tdsrrc_cmd_type *cmd_ptr
)
{
  switch(cmd_ptr->cmd_hdr.cmd_id)
  {
    case TDSRRC_DOWNLINK_L2ACK_IND:
      if((tdsrrcdormancy_saved_fast_dormancy_req.mui_status == TDSRRCDORMANCY_MUI_SCRI_CNF) && 
         (cmd_ptr->cmd.downlink_l2ack_ind.mui == tdsrrcdormancy_saved_fast_dormancy_req.mui))
      {
        TDSRRC_MSG3(MSG_LEGACY_HIGH,"L2 ACK for SCRI Status:%d, mui_status:%d, mui=%d",
          cmd_ptr->cmd.downlink_l2ack_ind.status,
          tdsrrcdormancy_saved_fast_dormancy_req.mui_status,
          tdsrrcdormancy_saved_fast_dormancy_req.mui);

        //send fast-dormancy-cnf status based on l2-ack status
        if(SUCCESS == cmd_ptr->cmd.downlink_l2ack_ind.status)
        {
          tdsrrcdormancy_reset_n_send_fast_dormancy_cnf(RRC_FAST_DORMANCY_SUCCESS_APP_NO_RETRY);
        }
        else
        {          
          tdsrrcdormancy_reset_n_send_fast_dormancy_cnf(RRC_FAST_DORMANCY_FAILURE_APP_RETRY);
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"L2 ACK Failed for SCRI. Mark Pending to be sent later");
          tdsrrcdormancy_saved_fast_dormancy_req.num_pending_dormancy_req++;
        }    
      }
      else
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"Invalid L2 ACK for Fast Dormancy SCRI is rcvd");
      }
      break;

    case TDSRRC_FAST_DORMANCY_REQ:
      TDSRRC_MSG2(MSG_LEGACY_HIGH,"RRCDORM:Invalid Event: 0x%x for dormancy in substate:%d",cmd_ptr->cmd_hdr.cmd_id,tdsrrcdormancy_substate);
      /* Do not process next dormancy req till first one complets */
      tdsrrcdormancy_send_fast_dormancy_cnf(RRC_FAST_DORMANCY_FAILURE_APP_RETRY);
      break;

    default:
      TDSRRC_MSG2(MSG_LEGACY_HIGH,"RRCDORM:Invalid Event: %d for dormancy in substate:%d",cmd_ptr->cmd_hdr.cmd_id,tdsrrcdormancy_substate);
      break;

  }/*end of switch*/
} /* tdsrrcdormancy_process_wait_for_l2_ack_substate */

/*===========================================================================

FUNCTION tdsrrcdormancy_process_wait_for_cu_cnf_substate

DESCRIPTION
  This function processes all events that are  dispatched
  to fast dormancy  procedure. in RRCDORMANCY_WAIT_FOR_CU_CNF
  substate

DEPENDENCIES
  None.

RETURN VALUE
  None.

===========================================================================*/

static void tdsrrcdormancy_process_wait_for_cu_cnf_substate
(
  tdsrrc_cmd_type *cmd_ptr
)
{
  switch(cmd_ptr->cmd_hdr.cmd_id)
  {
    case TDSRRC_CELL_UPDATE_COMPLETE_CNF:
      if((tdsrrcdormancy_saved_fast_dormancy_req.fast_dormancy_req_status == TRUE) &&
         (tdsrrcdormancy_saved_fast_dormancy_req.mui_status == TDSRRCDORMANCY_MUI_STATUS_INVALID))
      {
        if(tdsrrcdormancy_saved_fast_dormancy_req.fast_dormancy_req_status == TRUE)
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Processing dormancy after CU_COMP_CNF");
          /* If all validation goes well then process fast dormancy req and start T323 timer*/
          if(TRUE == tdsrrcdormancy_process_fast_dormancy_req())
          {
            if(TRUE == tdsrrcdormancy_check_if_nw_support_fast_dormancy())
            {
              tdsrrcdormancy_stop_or_restart_timer_T323(tdsrrcdormancy_get_T323_timer_value(),
                TDSRRCDORMANCY_T323_TIMER_RUNNING); 
            }
            else
            {
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"N/W dint support T323. Do Not Start timer");
            }            
          }
        }
      }
      else
      {
         TDSRRC_MSG3(MSG_LEGACY_HIGH,"RRCDORM:No Dormancy to process substate:%d REQ:%d MUI:%d", 
            tdsrrcdormancy_substate,
            tdsrrcdormancy_saved_fast_dormancy_req.fast_dormancy_req_status,
            tdsrrcdormancy_saved_fast_dormancy_req.mui_status);
      }
      break;

    case TDSRRC_FAST_DORMANCY_REQ:
      /* Do not process next dormancy req till first one complets */
      tdsrrcdormancy_send_fast_dormancy_cnf(RRC_FAST_DORMANCY_FAILURE_APP_RETRY);
      break;

    default:
      TDSRRC_MSG2(MSG_LEGACY_HIGH,"RRCDORM:Invalid Event: %d for dormancy in substate:%d",cmd_ptr->cmd_hdr.cmd_id,tdsrrcdormancy_substate);
      break;

  }  
   
} /* tdsrrcdormancy_process_wait_for_cu_cnf_substate */

/*===========================================================================
FUNCTION tdsrrcrce_process_rce_wait_for_sig_con_rel_compl_substate

DESCRIPTION
  This function processes all events that are dispatched to RRC Connection
  Establishment procedure in TDSRRCRCE_WAIT_FOR_SIG_CON_REL_COMPL sub state either 
  CELL_FACH State or CELL_DCH state.
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
static void tdsrrcdormancy_process_wait_for_sig_con_rel_compl_substate
(
  tdsrrc_cmd_type *cmd_ptr                      /* Pointer to command */
)
{
  switch (cmd_ptr->cmd_hdr.cmd_id)
  {
    case TDSRRC_SIG_CONN_RELEASE_CNF:
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"Status rcvd from SCRR: %d",cmd_ptr->cmd.rrc_sig_conn_cnf.status);
      //send fast-dormancy-cnf status based on l2-ack status
      if(SUCCESS == cmd_ptr->cmd.rrc_sig_conn_cnf.status)
      {
        tdsrrcdormancy_reset_n_send_fast_dormancy_cnf(RRC_FAST_DORMANCY_SUCCESS_APP_NO_RETRY);
      }
      else
      {
        tdsrrcdormancy_reset_n_send_fast_dormancy_cnf(RRC_FAST_DORMANCY_FAILURE_APP_RETRY);
      }
      /*PS doamin would have been closed by now...so send CLOSE_SESSION_IND to NAS now*/
      tdsrrcscr_send_close_session_ind(RRC_PS_DOMAIN_CN_ID, RRC_MAX_SESSION_CLOSE_CAUSE);
      break;

    case TDSRRC_FAST_DORMANCY_REQ:
      /* Do not process next dormancy req till first one complets */
      tdsrrcdormancy_send_fast_dormancy_cnf(RRC_FAST_DORMANCY_SUCCESS_APP_RETRY);
      break;

    default:
      TDSRRC_MSG2(MSG_LEGACY_HIGH,"RRCDORM:Invalid Event: %d for dormancy in substate:%d",cmd_ptr->cmd_hdr.cmd_id,tdsrrcdormancy_substate);
  }
}

/*===========================================================================

FUNCTION tdsrrcdormancy_process_dormancy_initial_substate

DESCRIPTION
  This function processes all events that are  dispatched
  to fast dormancy  procedure. in TDSRRCDORMANCY_INITIAL
  substate

DEPENDENCIES
  None.

RETURN VALUE
  None.

===========================================================================*/
static void tdsrrcdormancy_process_dormancy_initial_substate
(
  tdsrrc_cmd_type *cmd_ptr
)
{
  switch(cmd_ptr->cmd_hdr.cmd_id)
  {
    case TDSRRC_FAST_DORMANCY_REQ:
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"DORMANCY_REQ rcvd in %d substate",tdsrrcdormancy_substate);
      tdsrrcdormancy_validate_and_process_fast_dormancy_req(cmd_ptr);
      break;

    default:
      TDSRRC_MSG2(MSG_LEGACY_HIGH,"RRCDORM:Invalid Event: %d for dormancy in substate:%d",cmd_ptr->cmd_hdr.cmd_id,tdsrrcdormancy_substate);
      break;
  }/*end of switch*/
} /* tdsrrcdormancy_process_dormancy_initial_substate */

/*===========================================================================

FUNCTION tdsrrcdormancy_process_cell_fach_state

DESCRIPTION
  This function processes all events that are dispatched to for the fast dormancy  procedure.
  in CELL_FACH state of Connected mode.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
static void tdsrrcdormancy_process_cell_fach_state
(
  tdsrrc_cmd_type *cmd_ptr                      /* Pointer to RRC command */
)
{
  switch (tdsrrcdormancy_substate)
  {
    case TDSRRCDORMANCY_INITIAL:
      switch(cmd_ptr->cmd_hdr.cmd_id)
      {
        case TDSRRC_FAST_DORMANCY_REQ:
          tdsrrcdormancy_process_dormancy_initial_substate(cmd_ptr);
          break;
        
        default:
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"RRCDORM:Invalid event in Substate: %d",tdsrrcdormancy_substate);
         break;
      }
      break;

    case TDSRRCDORMANCY_WAIT_FOR_CU_COMP_CNF: 
      switch(cmd_ptr->cmd_hdr.cmd_id)
      {
        /* we can either get a complete confirmation or get a close session request
         * for a different CN domain.  Process both in wait for cell update comp cnf
         */
        case TDSRRC_CELL_UPDATE_COMPLETE_CNF:
        case TDSRRC_FAST_DORMANCY_REQ:
          tdsrrcdormancy_process_wait_for_cu_cnf_substate(cmd_ptr);
          break;

        default:
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"RRCDORM:Invalid event in Substate: %d",tdsrrcdormancy_substate);

          break;
      }
      break;

    case TDSRRCDORMANCY_WAIT_FOR_L2ACK: 
      switch(cmd_ptr->cmd_hdr.cmd_id)
      {
        /* we can either get a complete confirmation or get a close session request
         * for a different CN domain.  Process both in wait for cell update comp cnf
         */
        case TDSRRC_DOWNLINK_L2ACK_IND:
        case TDSRRC_FAST_DORMANCY_REQ:
          tdsrrcdormancy_process_wait_for_l2_ack_substate(cmd_ptr);
          break;

        default:
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"RRCDORM:Invalid event in Substate: %d",tdsrrcdormancy_substate);

          break;
      }
      break;

    case TDSRRCDORMANCY_WAIT_FOR_SIG_CON_REL_COMPL:
      switch(cmd_ptr->cmd_hdr.cmd_id)
      {
        case TDSRRC_SIG_CONN_RELEASE_CNF:
        case TDSRRC_FAST_DORMANCY_REQ:
          tdsrrcdormancy_process_wait_for_sig_con_rel_compl_substate(cmd_ptr);
          break;

        default:
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"RRCDORM:Invalid event in Substate: %d",tdsrrcdormancy_substate);
          break;
      }
      break;
      
    default:   
      /*lint -e793*/        
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"RRCDORM:Invalid substate: %d in CELL FACH",tdsrrcdormancy_substate);
      /*lint +e793*/
      break;
  }  
}/* tdsrrcdormancy_process_cell_fach_state */
/*===========================================================================

FUNCTION tdsrrcdormancy_process_cell_dch_state

DESCRIPTION
  This function processes all events that are dispatched to for the fast dormancy  procedure.
  in CELL_DCH state of Connected mode.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
static void tdsrrcdormancy_process_cell_dch_state
(
  tdsrrc_cmd_type *cmd_ptr                      /* Pointer to RRC command */
)
{
  switch (tdsrrcdormancy_substate)
  {
    case TDSRRCDORMANCY_INITIAL:
      switch(cmd_ptr->cmd_hdr.cmd_id)
      {
        case TDSRRC_FAST_DORMANCY_REQ:
          tdsrrcdormancy_process_dormancy_initial_substate(cmd_ptr);
          break;
        
        default:
          TDSRRC_MSG2(MSG_LEGACY_HIGH,"RRCDORM:Invalid event %d in Substate: %d",cmd_ptr->cmd_hdr.cmd_id,tdsrrcdormancy_substate);
         break;
      }
      break;

    case TDSRRCDORMANCY_WAIT_FOR_CU_COMP_CNF: 
      switch(cmd_ptr->cmd_hdr.cmd_id)
      {
        /* we can either get a complete confirmation or get a close session request
         * for a different CN domain.  Process both in wait for cell update comp cnf
         */
        case TDSRRC_CELL_UPDATE_COMPLETE_CNF:
        case TDSRRC_FAST_DORMANCY_REQ:
          tdsrrcdormancy_process_wait_for_cu_cnf_substate(cmd_ptr);
          break;

        default:
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"RRCDORM:Invalid event in Substate: %d",tdsrrcdormancy_substate);

          break;
      }
      break;

    case TDSRRCDORMANCY_WAIT_FOR_L2ACK: 
      switch(cmd_ptr->cmd_hdr.cmd_id)
      {
        /* we can either get a complete confirmation or get a close session request
         * for a different CN domain.  Process both in wait for cell update comp cnf
         */
        case TDSRRC_DOWNLINK_L2ACK_IND:
        case TDSRRC_FAST_DORMANCY_REQ:
          tdsrrcdormancy_process_wait_for_l2_ack_substate(cmd_ptr);
          break;

        default:
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"RRCDORM:Invalid event in Substate: %d",tdsrrcdormancy_substate);

          break;
      }
      break;
      
    case TDSRRCDORMANCY_WAIT_FOR_SIG_CON_REL_COMPL:
      switch(cmd_ptr->cmd_hdr.cmd_id)
      {
        case TDSRRC_SIG_CONN_RELEASE_CNF:
        case TDSRRC_FAST_DORMANCY_REQ:
          tdsrrcdormancy_process_wait_for_sig_con_rel_compl_substate(cmd_ptr);
          break;
    
        default:
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"RRCDORM:Invalid event in Substate: %d",tdsrrcdormancy_substate);
          break;
      }
      break;

    default:   
      /*lint -e793*/        
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"RRCDORM:Invalid substate: %d in CELL FACH",tdsrrcdormancy_substate);
      /*lint +e793*/
      break;
  }  
}/* tdsrrcdormancy_process_cell_dch_state */

/*===========================================================================

FUNCTION tdsrrcdormancy_process_cell_ura_pch_state

DESCRIPTION
  This function processes all events that are dispatched to for the fast dormancy  procedure.
  in CELL/URA_PCH state of Connected mode.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
static void tdsrrcdormancy_process_cell_ura_pch_state
(
  tdsrrc_cmd_type *cmd_ptr                      /* Pointer to RRC command */
)
{
  TDSRRC_MSG1(MSG_LEGACY_HIGH,"RRCDORM:cell_ura_pch_stat handler dormancy Substate: %d",tdsrrcdormancy_substate);

  switch (tdsrrcdormancy_substate)
  {
    case TDSRRCDORMANCY_INITIAL:
      switch(cmd_ptr->cmd_hdr.cmd_id)
      {
        case TDSRRC_FAST_DORMANCY_REQ:
          tdsrrcdormancy_process_dormancy_initial_substate(cmd_ptr);
          break;
        
        default:
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"RRCDORM:Invalid event in Substate: %d",tdsrrcdormancy_substate);
         break;
      }
      break;

    case TDSRRCDORMANCY_WAIT_FOR_CU_COMP_CNF: 
      switch(cmd_ptr->cmd_hdr.cmd_id)
      {
        case TDSRRC_CELL_UPDATE_COMPLETE_CNF:
        case TDSRRC_FAST_DORMANCY_REQ:
          tdsrrcdormancy_process_wait_for_cu_cnf_substate(cmd_ptr);
          break;

        default:
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"RRCDORM:Invalid event in Substate: %d",tdsrrcdormancy_substate);

          break;
      }
      break;

    case TDSRRCDORMANCY_WAIT_FOR_L2ACK: 
      switch(cmd_ptr->cmd_hdr.cmd_id)
      {
        /* we can either get a complete confirmation or get a close session request
         * for a different CN domain.  Process both in wait for cell update comp cnf
         */
        case TDSRRC_DOWNLINK_L2ACK_IND:
        case TDSRRC_FAST_DORMANCY_REQ:
          tdsrrcdormancy_process_wait_for_l2_ack_substate(cmd_ptr);
          break;

        default:
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"RRCDORM:Invalid event in Substate: %d",tdsrrcdormancy_substate);

          break;
      }
      break;

    case TDSRRCDORMANCY_WAIT_FOR_SIG_CON_REL_COMPL:
      switch(cmd_ptr->cmd_hdr.cmd_id)
      {
        case TDSRRC_SIG_CONN_RELEASE_CNF:
        case TDSRRC_FAST_DORMANCY_REQ:
          tdsrrcdormancy_process_wait_for_sig_con_rel_compl_substate(cmd_ptr);
          break;
    
        default:
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"RRCDORM:Invalid event in Substate: %d",tdsrrcdormancy_substate);
          break;
      }
      break;

    default:   
      /*lint -e793*/        
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"RRCDORM:Invalid substate: %d in CELL FACH",tdsrrcdormancy_substate);
      /*lint +e793*/
      break;
  }  
}/* tdsrrcdormancy_process_cell_ura_pch_state */

/*===========================================================================

FUNCTION tdsrrcdormancy_process_idle_state

DESCRIPTION
  This function processes all events that are dispatched to for the fast dormancy  procedure.
  in IDLE state of Connected mode.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
static void tdsrrcdormancy_process_idle_state
(
  tdsrrc_cmd_type *cmd_ptr                      /* Pointer to RRC command */
)
{
  TDSRRC_MSG1(MSG_LEGACY_HIGH,"RRCDORM:IDLE state handler dormancy Substate: %d",tdsrrcdormancy_substate);

  switch(cmd_ptr->cmd_hdr.cmd_id)
  {
    case TDSRRC_FAST_DORMANCY_REQ:
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Not processing Fast Dormancy SCRI");
      tdsrrcdormancy_reset_n_send_fast_dormancy_cnf(RRC_FAST_DORMANCY_SUCCESS_APP_NO_RETRY);
      break;
   
    default:
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"RRCDORM:Invalid cmd_id i: 0X%x",cmd_ptr->cmd_hdr.cmd_id);
      break;
  }
}/* tdsrrcdormancy_process_idle_state */


/*===========================================================================

FUNCTION tdsrrcdormancy_procedure_event_handler

DESCRIPTION
  This procedure is the event handler for the fast dormancy  procedure.
  The following events are handled by this procedure:

    TDSRRC_FAST_DORMANCY_REQ Primitive (received from MM)
    TDSRRC_CELL_UPDATE_COMPLETE_CNF primitive (received from CELL UPDATE Procedure)

DEPENDENCIES
  None.

RETURN VALUE
  None.

===========================================================================*/

void tdsrrcdormancy_procedure_event_handler
(
  tdsrrc_cmd_type *cmd_ptr                       /* Pointer to RRC command */
)
{
#ifdef FEATURE_TDSCDMA_3GPP_FAST_DORMANCY_ENHANCEMENT
  uint64 remaining_value = 0;
#endif
  tdsrrc_state_e_type rrc_state = tdsrrc_get_state();

  if(TDSRRC_STATE_CHANGE_IND == cmd_ptr->cmd_hdr.cmd_id)
  {
    TDSRRC_MSG3(MSG_LEGACY_HIGH,"RRCDORM:State-Change-Ind dorm-state:%d rrc-state:%d new-state:%d",
       tdsrrcdormancy_substate,rrc_state,cmd_ptr->cmd.state_change_ind.new_state);

    switch (cmd_ptr->cmd.state_change_ind.new_state)
    {
      case TDSRRC_STATE_DISCONNECTED:
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRCDORM:State-Change-Ind to IDLE,STOP T_323_TIMER");
        tdsrrctmr_stop_timer(TDSRRCTMR_T_323_TIMER);
        /* Code fall through. Make Lint Happy */
      case TDSRRC_STATE_CONNECTING:
          tdsrrcdormancy_reset_v316();
        /* Code fall through. Make Lint Happy */
      case TDSRRC_STATE_CELL_PCH:
      case TDSRRC_STATE_URA_PCH:
        tdsrrcdormancy_check_and_abort_dormancy_req(RRC_FAST_DORMANCY_SUCCESS_APP_NO_RETRY);
        break;
#ifdef FEATURE_TDSCDMA_3GPP_FAST_DORMANCY_ENHANCEMENT
      case TDSRRC_STATE_CELL_FACH:
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRCDORM:State-Change-Ind to FACH ");
        /* for PCH->FACH, check if no data activity
         * if yes -> num_pending_dormancy_req is TRUE & NW support 3gpp FD start T_323, at expiry we will re try FD
         * if no -> re set num_pending_dormancy_req
         */
         /* need RLC provide function call */
        if((tdsrlc_query_data_channel_activity() == NO_DATA_TRANSFERRED) &&
           (tdsrrc_check_psrab_data_is_present() == FALSE))
        {
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"RRCDORM:Value of max_scri %d",tdsrrc_max_scri_for_fd);
          if(tdsrrcdormancy_saved_fast_dormancy_req.num_pending_dormancy_req != 0)
          {
            TDSRRC_MSG1(MSG_LEGACY_HIGH,"RRCDORM: Pending Dormancy requests:%d in new FACH state",tdsrrcdormancy_saved_fast_dormancy_req.num_pending_dormancy_req);
            if(TRUE == tdsrrcdormancy_check_if_nw_support_fast_dormancy())
            {
              remaining_value = tdsrrctmr_get_remaining_time(TDSRRCTMR_T_323_TIMER);
              if(remaining_value == 0)
              {
                tdsrrctmr_start_timer(TDSRRCTMR_T_323_TIMER, TDSRRC_DORMANCY_T323_RETRY_TIMER);
              }
              return;
            }
            else
            {
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"N/W dint support T323. Do Not Start timer");
              tdsrrcdormancy_saved_fast_dormancy_req.num_pending_dormancy_req =0;
            } 
          }
        }
        else
        {
          tdsrrcdormancy_saved_fast_dormancy_req.num_pending_dormancy_req =0;
        }
        break;
#endif
        
      default:
        TDSRRC_MSG3(MSG_LEGACY_HIGH,"Unexpected StateChangeInd dorm-state:%d rrc-state:%d new-state:%d",
          tdsrrcdormancy_substate,rrc_state,cmd_ptr->cmd.state_change_ind.new_state);
    }
  }
  else if(TDSRRC_T323_EXPIRED_IND == cmd_ptr->cmd_hdr.cmd_id)
  {
    tdsrrcdormancy_process_T323_expiry(cmd_ptr);
  }
  else
  {
    switch (rrc_state)
    {
      case TDSRRC_STATE_CELL_FACH:    
        tdsrrcdormancy_process_cell_fach_state(cmd_ptr); 
        break;

      case TDSRRC_STATE_CELL_DCH:  
        tdsrrcdormancy_process_cell_dch_state(cmd_ptr); 
        break;

      case TDSRRC_STATE_CELL_PCH:
      case TDSRRC_STATE_URA_PCH:
        tdsrrcdormancy_process_cell_ura_pch_state(cmd_ptr);
        break;

      case TDSRRC_STATE_DISCONNECTED:
      case TDSRRC_STATE_CONNECTING:
        tdsrrcdormancy_process_idle_state(cmd_ptr);
        break;

      default:  
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"RRCDORM:Event in an invalid State: %d",rrc_state);
        break;
     }  
  }
} /* tdsrrcdormancy_procedure_event_handler */

/*===========================================================================

FUNCTION tdsrrcdormancy_init

DESCRIPTION

  This function initializes the dormancy  procedure.
  This function needs to be called once only, at powerup.

DEPENDENCIES

  None.

RETURN VALUE

  None.
  
SIDE EFFECTS

  None.

===========================================================================*/
void tdsrrcdormancy_init( void )
{


  tdsrrcdormancy_init_saved_fast_dormancy_req();

  tdsrrcdormancy_saved_fast_dormancy_req.num_pending_dormancy_req = 0;

  /* Disable fast dormancy support during init
     * This will be enabled when N/W indicates support of Timer T323 */
  tdsrrcdormancy_disable_fast_dormancy();
  /* Reset V316 */ 
  tdsrrcdormancy_reset_v316();

 /* Also send registration to state change manager. We need to know when
  RRC goes to Idle mode (disconnected state). The BCCH will be cleared at
  that time so we need to clear our internal variables that keep track of
  the BCCH status. */
  tdsrrcscmgr_register_for_scn( TDSRRC_PROCEDURE_DORMANCY,  /* Procedure name */
                             TDSRRC_STATE_WILDCARD, /* From State     */
                             TDSRRC_STATE_DISCONNECTED /* To State    */
                           );
  tdsrrcscmgr_register_for_scn( TDSRRC_PROCEDURE_DORMANCY,  /* Procedure name */
                             TDSRRC_STATE_DISCONNECTED, /* From State     */
                             TDSRRC_STATE_WILDCARD /* To State    */
                           );
#ifdef FEATURE_TDSCDMA_3GPP_FAST_DORMANCY_ENHANCEMENT
  tdsrrcscmgr_register_for_scn( TDSRRC_PROCEDURE_DORMANCY,  /* Procedure name */
                             TDSRRC_STATE_CELL_PCH, /* From State     */
                             TDSRRC_STATE_CELL_FACH /* To State    */
                           );
  tdsrrcscmgr_register_for_scn( RRC_PROCEDURE_DORMANCY,  /* Procedure name */
                             TDSRRC_STATE_URA_PCH, /* From State     */
                             TDSRRC_STATE_CELL_FACH /* To State    */
                           );
#endif

}

#endif  
#endif  /*RRCDORMANCY_H*/
